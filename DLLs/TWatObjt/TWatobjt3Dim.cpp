// Water temperature object CPP code file
// @ Pedro Duarte 25/03/2005
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"

TWaterTemperatureThreeDimensional::TWaterTemperatureThreeDimensional(
        TEcoDynClass* APEcoDynClass, char* className)
                : TWaterTemperatureTwoDimensionalForRiaFormosa(APEcoDynClass, className)
{
//   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
    if (PReadWrite != NULL){
        int X,Y;
        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            // Read the water temperature values
            if (PReadWrite->FindString("LoadLayers", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRecord[j].LayerCoordinate);
                  ALoadRecord[j].LayerCoordinate = NumberOfLayers - ALoadRecord[j].LayerCoordinate;
               }
            }
            if (PReadWrite->FindString("RiverLoadLayers", X, Y))
            {
               //Debugger(NumberOfRiverLoads);
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRiverRecord[j].LayerCoordinate);
                  ALoadRiverRecord[j].LayerCoordinate = NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate;
               }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

void TWaterTemperatureThreeDimensional::Integrate()
{
    int UpperLayer;
    double MyBoxDepth, AValue;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
        UpperLayer = ceil(AValue);
    }
    else UpperLayer = NumberOfLayers - 1;
    Generic = AverageWaterTemperature; GenericFlux = NetHeatFlux;
    Integration3DSubDomain(UpperLayer);
    AverageWaterTemperature = Generic; NetHeatFlux = GenericFlux;
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Go(AverageWaterTemperature, WaterTemperatureLoad, 0, 0);
        SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
        int i,j, index;
        //UpperLayer = MyPEcoDynClass->GetNumberOfLayers() - 1;
        /*MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue,
                                    0,"UpperLayer",ObjectCode);
        UpperLayer = ceil(AValue);  */
        //Southern boundary
        for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
        {
            for (j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
            {
               //i = pSubDomain->ILine[j];
               for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++) {
                 if (i <= pSubDomain->FLine[j])
                 {
                    index = Get3DIndex(i,j,LayerNr);
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                        index,"Box depth",ObjectCode);
                    if (
                          (MyBoxDepth > 0.0) &&
                          (
                              (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  || //Southern ocean boundary
                              (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) ||//Northern ocean boundary
                              (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) ||//Eastern ocean boundary
                              (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
                          )
                       )
                    {
                       if (DailyOceanTemperature[JulianDay-1] > 0.0)
                           AverageWaterTemperature[index] = DailyOceanTemperature[JulianDay-1];
                    }
                 }
               }
            }
        }
    }
    Loads();
}

void TWaterTemperatureThreeDimensional::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Water temperature") == 0)
		Value = AverageWaterTemperature[MyBoxNumber];
	else if (strcmp(MyParameter, "Water irradiance") == 0)
		Value = WaterIrradiance[MyBoxNumber];
	else if (strcmp(MyParameter, "Net heat flux") == 0)
		Value = NetHeatFlux[MyBoxNumber];
	else if (strcmp(MyParameter, "Latent heat flux") == 0)
		Value = LatentHeatFlux[MyBoxNumber];
	else if (strcmp(MyParameter, "Sensible heat flux") == 0)
		Value = SensibleHeatFlux[MyBoxNumber];
        else if (strcmp(MyParameter, "Relative humidity") == 0)
                Value = RelativeHumidity;
        else if (strcmp(MyParameter, "Relative humidity time series") == 0)
        {
            if ((NumberOfDaysForTimeSeries > 0) || (NumberOfHoursForTimeSeries > 0))
             GetRelativeHumidity(GetLineIndex3D(MyBoxNumber),GetColumnIndex3D(MyBoxNumber));
            Value = RelativeHumidity;
        }
        else
            Value = 0;
}

void TWaterTemperatureThreeDimensional::GetWaterTemperature()
{
    const double WaterSpecificHeat = 4100; // J/kg/k

    double MySalinity = 0.0,
       MyLightAtTop = 0.0,
       MyLightAtBottom = 0.0,
       ABottom = 0.0,
       MyAtmosphericIR = 0.0,
       MySVPAtWaterTemperature = 0.0,
       MySVPAtAirTemperature = 0.0,
       MyAirTemperature = 0.0,
       MyTemperatureVariation = 0.0,
       MyWindSpeed = 0.0, MyExtinctionCoeff = 0.0, ADepth = 0.0;
    TEcoDynClass* MyTAirTemperaturePointer = MyPEcoDynClass->GetAirTemperaturePointer();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
    TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();

    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    int index, UpperLayer;
    UpperLayer = MyPEcoDynClass->GetNumberOfLayers() - 1;
    double AValue;
    if (MyTransportPointer != NULL) {
       MyTransportPointer->Inquiry(GetEcoDynClassName(),AValue,0,"UpperLayer",ObjectCode);
       UpperLayer = ceil(AValue);
    }

    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
       {
          if ((NumberOfDaysForTimeSeries > 0) || (NumberOfHoursForTimeSeries > 0))
            GetRelativeHumidity(i,j);
          for (int LayerNr = UpperLayer; LayerNr >= 0; LayerNr--)
          {
             index = Get3DIndex(i,j,LayerNr);
             
             if (MyTransportPointer != NULL)
             {
                MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,
                                                      index,
                                                      "Salinity",
                                                      ObjectCode);
                MyTransportPointer->Inquiry(GetEcoDynClassName(), ADepth,
                                                        index,
                                                        "Box depth",
                                                        ObjectCode);
                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,
                                                        index,
                                                        "Water density",
                                                        ObjectCode);
             }
             else
             {
                MySalinity = 37;      // local variable
                MyWaterDensity = 1027.29; // seawater at 35psu and 10oC
                ADepth = MyPEcoDynClass->GetBoxDepth(index);
             }

             if (((ADepth > CriticalDepthForLandBoundary) && (MyTransportPointer == NULL)) ||
                ((MyTransportPointer != NULL) &&
                  (ADepth > CriticalDepthForLandBoundary) &&
                  (BoxArray[index].Dummybool1) &&
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                  (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                  (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) &&//No northern ocean boundary
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) //No western ocean boundary
                ))
             {
                  if ((MyTAirTemperaturePointer != NULL) && (LayerNr == UpperLayer))
                     MyTAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature,
                                                          index,
                                                          "Air temperature",
                                                          ObjectCode);

                  TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
                  if (MySPMPointer != NULL)
                  {
                     MySPMPointer->Inquiry(GetEcoDynClassName(), MyExtinctionCoeff,
                                      index,
                                      "Extinction coefficient Ktot",
                                      ObjectCode);
                  }
                  else
                     MyExtinctionCoeff = 1.0;

                  if (MyLightPointer != NULL)
                  {
                     if (LayerNr == UpperLayer)
                     {
                         int index1;
                         index1 = Get3DIndex(i,j,0);  //O liteobjt calcula a radia??o ? superf?cie para a camada 0
                         MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
                                                             index1,
                                                             "Total surface irradiance",
                                                             ObjectCode);
                         MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
                                                             MyLightAtTop,
                                                             ADepth,
                                                             MyExtinctionCoeff,
                                                             index,
                                                             "Sub-surface irradiance",
                                                             ObjectCode);
                         MyLightPointer->Inquiry(GetEcoDynClassName(), MyAtmosphericIR,
                                                             index1, //O liteobjt calcula a radia??o ? superf?cie para a camada 0
                                                             "Atmospheric IR",
                                                             ObjectCode);
                     }
                     else
                     {
                        MyLightAtTop = MyLightAtBottom;
                        MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
                                                             MyLightAtTop,
                                                             ADepth,
                                                             MyExtinctionCoeff,
                                                             index,
                                                             "Sub-surface irradiance",
                                                             ObjectCode);
                     }
                  }
                  else
                  {
                     MyLightAtTop = 0.0; MyLightAtBottom = 0.0; MyAtmosphericIR = 0.0;
                  }

                  if (MyWindPointer != NULL)
                     MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
                                                         index,
                                                         "Wind speed",
                                                         ObjectCode);
                  else
                  {
                     MyWindSpeed = 0.0;
                  }
                  if (LayerNr == UpperLayer)
                  {
                      WaterIrradiance[index] = GetWaterSurfaceRadiation(AverageWaterTemperature[index]);

                      MySVPAtWaterTemperature = GetSaturationVapourPressure(AverageWaterTemperature[index]);

                      MySVPAtAirTemperature = GetSaturationVapourPressure(MyAirTemperature);

                      LatentHeatFlux[index] = GetLatentWaterHeatFlux(MySVPAtWaterTemperature,
                                                                             MySVPAtAirTemperature,
                                                                             MyWindSpeed);

                      SensibleHeatFlux[index] = GetSensibleWaterHeatFlux(AverageWaterTemperature[index],
                                                                                  MyAirTemperature,
                                                                                  MyWindSpeed);
                  }
                  else
                  {
                      MyAtmosphericIR = 0.0;
                      WaterIrradiance[index] = 0.0;
                      MySVPAtWaterTemperature = 0.0;
                      MySVPAtAirTemperature = 0.0;
                      LatentHeatFlux[index] = 0.0;
                      SensibleHeatFlux[index] = 0.0;
                  }
                  NetHeatFlux[index] = MyLightAtTop
                                      - MyLightAtBottom
                                      + MyAtmosphericIR
                                      + WaterIrradiance[index]
                                      + LatentHeatFlux[index]
                                      + SensibleHeatFlux[index];
                  /*if (index == 190585) {

                      Debugger(MyLightAtTop);
                      Debugger(MyLightAtBottom);
                      Debugger(MyAtmosphericIR);
                      Debugger(WaterIrradiance[index]);
                      Debugger(LatentHeatFlux[index]);
                      Debugger(SensibleHeatFlux[index]);
                      Debugger(NetHeatFlux[index]);
                  } */
                  if ((MyWaterDensity == 0.0) || (WaterSpecificHeat == 0.0) || (ADepth == 0.0))
                  {
                    Debugger(MyWaterDensity);
                    Debugger(WaterSpecificHeat);
                    Debugger(ADepth);
                    Debugger(index);
                  }
                  NetHeatFlux[index] = NetHeatFlux[index] / (MyWaterDensity * WaterSpecificHeat * ADepth);
             }
          }
       }
    }
}



void TWaterTemperatureThreeDimensional::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    //Debugger(111);
    //Debugger(NumberOfDaysForLoads); Debugger(NumberOfRiverLoads);
    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    for (int j = 0; j < NumberOfLoads; j++)
    {
        index = Get3DIndex(ALoadRecord[j].LineCoordinate, ALoadRecord[j].ColumnCoordinate, ALoadRecord[j].LayerCoordinate);
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TWaterTemperatureThreeDimensional::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureThreeDimensional::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << ") \n\n Please correct Configuration Files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        //if (Depth > CriticalWaterDepth)
        if (Depth > 0.0)
        {
            //Debugger(1);
            //Debugger(index);
            WaterTemperatureLoad[index] = ALoadRecord[j].TemperatureLoad[ADay-1];
            //Debugger(WaterTemperatureLoad[index]);
        }
    }

    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        /*Debugger(ALoadRiverRecord[j].LineCoordinate);
        Debugger(ALoadRiverRecord[j].ColumnCoordinate);
        Debugger(ALoadRiverRecord[j].LayerCoordinate); */
        index = Get3DIndex(ALoadRiverRecord[j].LineCoordinate, ALoadRiverRecord[j].ColumnCoordinate, ALoadRiverRecord[j].LayerCoordinate);
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TWaterTemperatureThreeDimensional::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureThreeDimensional::Loads (Rivers) - Index " << index << " > Number of Boxes (" << NumberOfBoxes << ") \n\n Please correct Configuration Files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        if (Depth > 0.0){
           AverageWaterTemperature[index] = ALoadRiverRecord[j].TemperatureRiverLoad[ADay-1];
           //Debugger(index); Debugger(AverageWaterTemperature[index]);
        }
    }
}

// AP, 2009.04.08 - imported from TWaterTemperatureTwoDimensionalForRiaFormosa
bool TWaterTemperatureThreeDimensional::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("WaterTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Water temperature");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names
     */
    bool dailyValues = false, waterValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Relative humidity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RelativeHumidity, 8);
        }
        else if (strcmp(VariableNameArray[i], "Daily ocean temperature") == 0)
            dailyValues = true;
        else if (strcmp(VariableNameArray[i], "Water temperature") == 0)
            waterValues = true;
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' initial values
     */
    if (dailyValues) {
        PReadWrite->WriteCell("Daily ocean temperature values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < 365; j++)
        {
            PReadWrite->WriteCell(DailyOceanTemperature[j], 9);
            PReadWrite->WriteSeparator(true);
        }
     }
     if (waterValues) {
        PReadWrite->WriteCell("Water temperature values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            PReadWrite->WriteCell(AverageWaterTemperature[j], 8);
            PReadWrite->WriteSeparator(true);
        }
     }
    /*
     * save temperature loads values
     */
    if (NumberOfDaysForLoads > 0)
    {
        if ((NumberOfLoads > 0) && LoadRecordFilled)
        {
            PReadWrite->WriteCell("LoadLines");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("LoadColumns");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("LoadLayers");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("LoadName");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfLoads; j++)
            {
                PReadWrite->WriteCell(GridLines - ALoadRecord[j].LineCoordinate, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(ALoadRecord[j].ColumnCoordinate + 1, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(NumberOfLayers - ALoadRecord[j].LayerCoordinate, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(ALoadRecord[j].LoadName);
                PReadWrite->WriteSeparator(true);
            }

            PReadWrite->WriteCell("TemperatureLoad");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfLoads; j++)
            {
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                    PReadWrite->WriteCell(ALoadRecord[j].TemperatureLoad[Loads], 8);
                    PReadWrite->WriteSeparator(true);
                }
            }
        }
        if ((NumberOfRiverLoads > 0) && LoadRiverRecordFilled)
        {
            PReadWrite->WriteCell("RiverLoadLines");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("RiverLoadColumns");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("RiverLoadLayers");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("RiverName");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfRiverLoads; j++)
            {
                PReadWrite->WriteCell(GridLines - ALoadRiverRecord[j].LineCoordinate, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(ALoadRiverRecord[j].ColumnCoordinate + 1, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(ALoadRiverRecord[j].RiverName);
                PReadWrite->WriteSeparator(true);
            }

            PReadWrite->WriteCell("TemperatureRiverLoad");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfRiverLoads; j++)
            {
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                    PReadWrite->WriteCell(ALoadRiverRecord[j].TemperatureRiverLoad[Loads], 8);
                    PReadWrite->WriteSeparator(true);
                }
            }
        }
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}


void TWaterTemperatureThreeDimensional::GetRelativeHumidity(int i, int j)
{
    int ADay, AHour, AMoment,index2;

    if  (NumberOfDaysForTimeSeries > 1)
    {
       ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForTimeSeries - 1);
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForTimeSeries > 1)
    {
       ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
    }
    RelativeHumidity = RelativeHumidityTimeSeries[AMoment]; //Por defeito assume que a temperatura na c?lula do modelo = temperatura do primeiro subdom?nio
    for (int k = 0; k < NumberOfSubDomains; k++)
    {
        index2 = k * NumberOfMomentsForTimeSeries + AMoment;
        if (
             (i >= RelativeHumiditySubDomains[k].FirstLine) && (i <= RelativeHumiditySubDomains[k].LastLine) &&
             (j >= RelativeHumiditySubDomains[k].FirstColumn) && (j <= RelativeHumiditySubDomains[k].LastColumn)
           )
           RelativeHumidity = RelativeHumidityTimeSeries[index2];
    }
}

void TWaterTemperatureThreeDimensional::Reinitialize()
{
}



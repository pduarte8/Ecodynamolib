// Water temperature object CPP code file
// @ Pedro Duarte 25/03/2005
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"

TWaterTemperatureTwoDimensionalForRiaFormosa::TWaterTemperatureTwoDimensionalForRiaFormosa(
        TEcoDynClass* APEcoDynClass, char* className)
                : TWaterTemperatureTwoDimensionalForSango(APEcoDynClass, className)
{
    CriticalDepthForLandBoundary = MyPEcoDynClass->GetCriticalDepth();
    NumberOfLoads = 0;
    //NumberOfDaysForLoads = 0;
    //NumberOfRiverLoads = 0;
    NumberOfDaysForBoundaryTimeSeries = 365;
    NumberOfLoads = MyPEcoDynClass->GetNumberOfLoads();
    //Debugger(NumberOfLoads);
    //NumberOfRiverLoads = MyPEcoDynClass->GetNumberOfRivers();
    //NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
    //Debugger(NumberOfRiverLoads);
    //Debugger(NumberOfDaysForLoads);
    if (NumberOfLoads > 0)
        ALoadRecord = new LoadRecord[NumberOfLoads];
    if (NumberOfRiverLoads > 0)
        ALoadRiverRecord = new LoadRiverRecord[NumberOfRiverLoads];
    WaterTemperatureLoad = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
       WaterTemperatureLoad[i] = 0.0;
    for (int i = 0; i < NumberOfLoads; i++)
    {
       ALoadRecord[i].LineCoordinate = 0;
       ALoadRecord[i].ColumnCoordinate = 0;
       ALoadRecord[i].TemperatureLoad = new double[NumberOfDaysForLoads];
       for (int j = 0; j < NumberOfDaysForLoads; j++){
          ALoadRecord[i].TemperatureLoad[j] = 0.0;
       }
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
       ALoadRiverRecord[i].LineCoordinate = 0;
       ALoadRiverRecord[i].ColumnCoordinate = 0;
       ALoadRiverRecord[i].TemperatureRiverLoad = new double[NumberOfDaysForLoads];
       for (int j = 0; j < NumberOfDaysForLoads; j++){
          ALoadRiverRecord[i].TemperatureRiverLoad[j] = 0.0;
       }
    }
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
    if (PReadWrite != NULL){
        int X,Y;
        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            // Read the water temperature values
            char MyVariable[65], LoadName[100], RiverName[100];
    /*
     * replaced in 2007.07.05 by AP ...
     *
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            //Debugger(NumberOfVariables);
            for (int i = 0; i < NumberOfVariables + 11; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                if (strcmp(MyVariable, "LoadLines") == 0)
                {
                   for (int j = 0; j < NumberOfLoads; j++)
                   {
                      PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, ALoadRecord[j].LineCoordinate);
                      ALoadRecord[j].LineCoordinate = GridLines - ALoadRecord[j].LineCoordinate;
                      //Debugger(ALoadRecord[j].LineCoordinate);
                   }
                }
                else if (strcmp(MyVariable, "LoadColumns") == 0)
                {
                   for (int j = 0; j < NumberOfLoads; j++)
                   {
                      PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, ALoadRecord[j].ColumnCoordinate);
                      ALoadRecord[j].ColumnCoordinate = ALoadRecord[j].ColumnCoordinate - 1;
                      //Debugger(ALoadRecord[j].ColumnCoordinate);
                   }
                }
                else if (strcmp(MyVariable, "LoadName") == 0)
                {
                   for (int j = 0; j < NumberOfLoads; j++)
                   {
                      PReadWrite->ReadString(X + 2 + i, Y + 1 + j, LoadName);
                      strcpy(ALoadRecord[j].LoadName, LoadName);
                   }
                }
                else if (strcmp(MyVariable, "TemperatureLoad") == 0)
                {
                   for (int j = 0; j < NumberOfLoads; j++)
                   {
                      for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                      {
                         PReadWrite->ReadNumber(X + 2 + i, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].TemperatureLoad[Loads]);
                      }
                   }
                   //Debugger(ALoadRecord[NumberOfLoads-1].TemperatureLoad[NumberOfDaysForLoads - 1]);
                }
                else if (strcmp(MyVariable, "RiverLoadLines") == 0)
                {
                   for (int j = 0; j < NumberOfRiverLoads; j++)
                   {
                      PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, ALoadRiverRecord[j].LineCoordinate);
                      ALoadRiverRecord[j].LineCoordinate = GridLines - ALoadRiverRecord[j].LineCoordinate;
                      //Debugger(ALoadRiverRecord[j].LineCoordinate);
                   }
                }
                else if (strcmp(MyVariable, "RiverLoadColumns") == 0)
                {
                   for (int j = 0; j < NumberOfRiverLoads; j++)
                   {
                      PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, ALoadRiverRecord[j].ColumnCoordinate);
                      ALoadRiverRecord[j].ColumnCoordinate = ALoadRiverRecord[j].ColumnCoordinate - 1;
                      //Debugger(ALoadRiverRecord[j].ColumnCoordinate);
                   }
                }
                else if (strcmp(MyVariable, "RiverName") == 0)
                {
                   for (int j = 0; j < NumberOfRiverLoads; j++)
                   {
                      PReadWrite->ReadString(X + 2 + i, Y + 1 + j, RiverName);
                      strcpy(ALoadRiverRecord[j].RiverName, RiverName);
                   }
                }
                else if (strcmp(MyVariable, "TemperatureRiverLoad") == 0)
                {
                   for (int j = 0; j < NumberOfRiverLoads; j++)
                   {
                      for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                      {
                         PReadWrite->ReadNumber(X + 2 + i, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].TemperatureRiverLoad[Loads]); //Loads should be in mg / L / s
                      }
                   }
                   //Debugger(ALoadRiverRecord[NumberOfRiverLoads-1].TemperatureRiverLoad[NumberOfDaysForLoads-1]);
                }
            }
     *
     * by the following code ...
     */
            if (PReadWrite->FindString("LoadLines", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRecord[j].LineCoordinate);
                  ALoadRecord[j].LineCoordinate = GridLines - ALoadRecord[j].LineCoordinate;
               }
            }
            if (PReadWrite->FindString("LoadColumns", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRecord[j].ColumnCoordinate);
                  ALoadRecord[j].ColumnCoordinate = ALoadRecord[j].ColumnCoordinate - 1;
               }
            }
            if (PReadWrite->FindString("LoadName", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadString(X, Y + 1 + j, LoadName);
                  strcpy(ALoadRecord[j].LoadName, LoadName);
               }
            }
            if (PReadWrite->FindString("TemperatureLoad", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                  {
                     PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].TemperatureLoad[Loads]);
                  }
               }
               LoadRecordFilled = true;
            }
            if (PReadWrite->FindString("RiverLoadLines", X, Y))
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRiverRecord[j].LineCoordinate);
                  ALoadRiverRecord[j].LineCoordinate = GridLines - ALoadRiverRecord[j].LineCoordinate;
               }
            }
            if (PReadWrite->FindString("RiverLoadColumns", X, Y))
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRiverRecord[j].ColumnCoordinate);
                  ALoadRiverRecord[j].ColumnCoordinate = ALoadRiverRecord[j].ColumnCoordinate - 1;
               }
            }
            if (PReadWrite->FindString("RiverName", X, Y))
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadString(X, Y + 1 + j, RiverName);
                  strcpy(ALoadRiverRecord[j].RiverName, RiverName);
               }
            }
            if (PReadWrite->FindString("TemperatureRiverLoad", X, Y))
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                  {
                     PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].TemperatureRiverLoad[Loads]); //Loads should be in mg / L / s
                  }
               }
               LoadRiverRecordFilled = true;
            }
    /* ...till here */
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TWaterTemperatureTwoDimensionalForRiaFormosa::~TWaterTemperatureTwoDimensionalForRiaFormosa()
{
   freeMemory();
}

void TWaterTemperatureTwoDimensionalForRiaFormosa::freeMemory()
{
  try {
    if ((NumberOfLoads > 0) && (NumberOfDaysForLoads > 0)) {
        for (int i = 0; i < NumberOfLoads; i++){
            delete [] ALoadRecord[i].TemperatureLoad;
        }
        delete [] ALoadRecord;
        LoadRecordFilled = false;
    }
    if ((NumberOfRiverLoads > 0) && (NumberOfDaysForLoads > 0)) {
        for (int i = 0; i < NumberOfRiverLoads; i++){
            delete [] ALoadRiverRecord[i].TemperatureRiverLoad;
        }
        delete [] ALoadRiverRecord;
        LoadRiverRecordFilled = false;
    }
    delete [] WaterTemperatureLoad;
  }
#ifdef __BORLANDC__
  catch (EAccessViolation &e) {
      //Application->ShowException(&e);
  }
  catch (Exception &exception) {
      //Application->ShowException(&exception);
  }
#else  // __BORLANDC__
	catch (exception &exc) {
			cerr << "TWaterTemperatureTwoDimensionalForRiaFormosa::freeMemory - " << exc.what() << endl;
	}
#endif  // __BORLANDC__
}

bool TWaterTemperatureTwoDimensionalForRiaFormosa::SaveVariables()
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
            PReadWrite->WriteCell("LoadName");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfLoads; j++)
            {
                PReadWrite->WriteCell(GridLines - ALoadRecord[j].LineCoordinate, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(ALoadRecord[j].ColumnCoordinate + 1, 0);
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
            PReadWrite->WriteCell("RiverName");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < NumberOfRiverLoads; j++)
            {
                PReadWrite->WriteCell(GridLines - ALoadRiverRecord[j].LineCoordinate, 0);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(ALoadRiverRecord[j].ColumnCoordinate + 1, 0);
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


void TWaterTemperatureTwoDimensionalForRiaFormosa::Integrate()
{


//    Generic = AverageWaterTemperature; GenericFlux = NetHeatFlux;
    Integration(AverageWaterTemperature, NetHeatFlux);
//    AverageWaterTemperature = Generic; NetHeatFlux = GenericFlux;

    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Go(AverageWaterTemperature, WaterTemperatureLoad, 0, 0);
        SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

        int i,j, index;
        double MyBoxDepth;
        //Southern boundary
        for (j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
        {
           i = pSubDomain->ILine[j];
           if (i <= pSubDomain->FLine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                  index,"Box depth",ObjectCode);
              if (
                    (MyBoxDepth > 0.0) &&
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
                 )
              {
                 AverageWaterTemperature[index] = DailyOceanTemperature[JulianDay-1];
              }
           }
        }

        //Northern boundary
        for (j = pSubDomain->FirstColumn;
        j <= pSubDomain->LastColumn; j++)
        {
                //i = GridLines - 1;
           i = pSubDomain->FLine[j] /*- 1*/;
           if (i >= pSubDomain->ILine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > 0.0) &&
                    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
                 )
              {
                 AverageWaterTemperature[index] = DailyOceanTemperature[JulianDay-1];
             }
           }
        }

        //Eastern boundary
        for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
        {
                //j = GridColumns - 1;
           j = pSubDomain->FColumn[i] /*- 1*/;
           if (j >= pSubDomain->IColumn[i])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > 0.0) &&
                    (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
                 )
              {
                 AverageWaterTemperature[index] = DailyOceanTemperature[JulianDay-1];
              }
           }
        }
        //Western boundary
        for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine; i++)
        {
                //j = 0;
           j = pSubDomain->IColumn[i];
           if (j <= pSubDomain->FColumn[i])
           {
              index = i * GridColumns + j;
               MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         index,
                                      "Box depth",
                                   ObjectCode);
              if (
                    (MyBoxDepth > 0.0) &&
                    (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
                 )
              {
                 AverageWaterTemperature[index] = DailyOceanTemperature[JulianDay-1];
              }
           }
        }
        for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
        {
           index = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
           if (AverageWaterTemperature[index]   < 0.0)   AverageWaterTemperature[index] = 0.0;
        }
    }
    Loads();
}

void TWaterTemperatureTwoDimensionalForRiaFormosa::Go()
{
    // Active methods
    MyTime = MyPEcoDynClass->GetTime();
    CurrentTime = MyPEcoDynClass->GetCurrTime();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    TimeBefore = MyTime;
    //Loads();
    GetWaterTemperature();
}

void TWaterTemperatureTwoDimensionalForRiaFormosa::GetWaterTemperature()
{
	const double WaterSpecificHeat = 4100; // J/kg/k

	double MySalinity,
               MyLightAtTop,
               MyLightAtBottom,
               ABottom,
               MyAtmosphericIR,
               MySVPAtWaterTemperature,
               MySVPAtAirTemperature,
               MyAirTemperature,
               MyTemperatureVariation,
               MyWindSpeed, MyExtinctionCoeff;

	TEcoDynClass* MyTAirTemperaturePointer = MyPEcoDynClass->GetAirTemperaturePointer();
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    int index;
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
           index = pSubDomain->BoxNumber[k];

           if (MyTransportPointer != NULL)
           {
              MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,
                                                    index,
                                                    "Salinity",
                                                    ObjectCode);
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,
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
              MixedLayerDepth = 15; // object variable
              MyWaterDensity = 1027.29; // seawater at 35psu and 10oC
              ABottom = MyPEcoDynClass->GetBoxDepth(index);
           }
           if (((ABottom > CriticalDepthForLandBoundary) && (MyTransportPointer == NULL)) ||
              ((MyTransportPointer != NULL) &&
                (ABottom > CriticalDepthForLandBoundary) &&
                (BoxArray[index].Dummybool1) &&
                (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) &&//No northern ocean boundary
                (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) //No western ocean boundary
              ))
           {
              if (MyTAirTemperaturePointer != NULL)
                 MyTAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature,
                                                      index,
                                                      "Air temperature",
                                                      ObjectCode);
              else
              {
                return;
              }
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
                 MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
                                                     index,
                                                     "Total surface irradiance",
                                                     ObjectCode);
                /*MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
                                                     ABottom,
                                                     index,
                                                     "Sub-surface irradiance",
                                                     ObjectCode); */
                MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
                                                     MyLightAtTop,
                                                     ABottom,
                                                     MyExtinctionCoeff,
                                                     index,
                                                     "Sub-surface irradiance",
                                                     ObjectCode);
                MyLightPointer->Inquiry(GetEcoDynClassName(), MyAtmosphericIR,
                                                     index,
                                                     "Atmospheric IR",
                                                     ObjectCode);
              }
              else
              {
                 return;
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



              WaterIrradiance[index] = GetWaterSurfaceRadiation(AverageWaterTemperature[index]);

              MySVPAtWaterTemperature = GetSaturationVapourPressure(AverageWaterTemperature[index]);

              MySVPAtAirTemperature = GetSaturationVapourPressure(MyAirTemperature);

              LatentHeatFlux[index] = GetLatentWaterHeatFlux(MySVPAtWaterTemperature,
                                                                     MySVPAtAirTemperature,
                                                                     MyWindSpeed);

              SensibleHeatFlux[index] = GetSensibleWaterHeatFlux(AverageWaterTemperature[index],
                                                                          MyAirTemperature,
                                                                          MyWindSpeed);


              NetHeatFlux[index] = MyLightAtTop
                                  - MyLightAtBottom
                                  + MyAtmosphericIR
                                  + WaterIrradiance[index]
                                  + LatentHeatFlux[index]
                                  + SensibleHeatFlux[index];

              if ((MyWaterDensity == 0.0) || (WaterSpecificHeat == 0.0) || (ABottom == 0.0))
              {
                Debugger(MyWaterDensity);
                Debugger(WaterSpecificHeat);
                Debugger(ABottom);
                Debugger(index);
              }

              NetHeatFlux[index] = NetHeatFlux[index] / (MyWaterDensity * WaterSpecificHeat * ABottom);
              /*MyTemperatureVariation = NetHeatFlux[index]
                                             * TimeStep
                                             / (MyWaterDensity
                                             * WaterSpecificHeat
                                             * ABottom);

              AverageWaterTemperature[index] = AverageWaterTemperature[index]
                                                  + MyTemperatureVariation;
              if (AverageWaterTemperature[index] < 0.0) AverageWaterTemperature[index] = 0.0;  */
        }
        /*else
        if ((ABottom > CriticalDepthForLandBoundary) && (BoxArray[index].Dummybool1))
           AverageWaterTemperature[index] = DailyOceanTemperature[JulianDay-1];  */
    }
}


void TWaterTemperatureTwoDimensionalForRiaFormosa::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    for (int j = 0; j < NumberOfLoads; j++)
    {
        index = ALoadRecord[j].LineCoordinate * GridColumns
                + ALoadRecord[j].ColumnCoordinate;
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TWaterTemperatureTwoDimensionalForRiaFormosa::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureTwoDimensionalForRiaFormosa::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << ")\n\nPlease correct Configuration Files" << endl;
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
            //Debugger(1); Debugger(index);
            WaterTemperatureLoad[index] = ALoadRecord[j].TemperatureLoad[ADay-1];
        }
    }
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        index = ALoadRiverRecord[j].LineCoordinate * GridColumns
                + ALoadRiverRecord[j].ColumnCoordinate;
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TWaterTemperatureTwoDimensionalForRiaFormosa::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TWaterTemperatureTwoDimensionalForRiaFormosa::Loads (River) - Index " << index << " > Number of Boxes (" << NumberOfBoxes << ")\n\nPlease correct Configuration Files" << endl;
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
        }
    }
}

// Dissolved object CPP code file
// @ Pedro Duarte
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"


TNutrients3D::TNutrients3D(TEcoDynClass* APEcoDynClass, char* className)
            :TRiaF2DNutrients(APEcoDynClass, className)
{
    int X, Y;
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
    if (PReadWrite != NULL)
    {
        char MyVariable[65], LoadName[100], RiverName[100];

        if (PReadWrite->FindString("Nutrients", X, Y))
        {
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
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRiverRecord[j].LayerCoordinate);
                  ALoadRiverRecord[j].LayerCoordinate = NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate;
               }
            }
        }
        else
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in TNutrients3D", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TNutrients3D::TNutrients3D - Variables - undefined object in TNutrients3D" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }
    PChemistry = new TChemistry();
}

TNutrients3D::~TNutrients3D()
{
    freeMemory();
}

void TNutrients3D::freeMemory()
{
  delete PChemistry;
}

void TNutrients3D::Integrate()
{
	int index, UpperLayer;
    double MyBoxDepth, AValue;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
        UpperLayer = ceil(AValue);
    }
    else UpperLayer = NumberOfLayers - 1;
    Generic = Ammonia; GenericFlux = NH4Flux;
    Integration3DSubDomain(UpperLayer);
    Ammonia = Generic; NH4Flux = GenericFlux;
    Generic = Nitrate; GenericFlux = NO3Flux;
    Integration3DSubDomain(UpperLayer);
    Nitrate = Generic; NO3Flux = GenericFlux;
    Generic = Nitrite; GenericFlux = NO2Flux;
    Integration3DSubDomain(UpperLayer);
    Nitrite = Generic; NO2Flux = GenericFlux;
    Generic = Phosphate; GenericFlux = PO4Flux;
    Integration3DSubDomain(UpperLayer);
    Phosphate = Generic; PO4Flux = GenericFlux;
    Generic = Oxygen; GenericFlux = OxygenFlux;
    Integration3DSubDomain(UpperLayer);
    Oxygen = Generic; OxygenFlux = GenericFlux;

    // Transport the nutrients

    if (MyTransportPointer != NULL)
    {
       MyTransportPointer->Go(Ammonia, NH4Load,0.0, 0.0);
       MyTransportPointer->Go(Nitrate, NO3Load,0.0, 0.0);
       MyTransportPointer->Go(Nitrite, NO2Load,0.0, 0.0);
       MyTransportPointer->Go(Phosphate, PO4Load,0.0, 0.0);
       MyTransportPointer->Go(Oxygen, O2Load,0.0, 0.0);
       for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
       {
          for (int j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
          {
              for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
              {
                 if (i <= pSubDomain->FLine[j])
                 {
                     index = Get3DIndex(i,j,LayerNr);
                     if (i <= pSubDomain->FLine[j])
                     {
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
                           Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
                           Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                           Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                           Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                           Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
                        }
                        if (Ammonia[index]   < 0.0)   Ammonia[index] = 0.0;
                        if (Nitrite[index]   < 0.0)   Nitrite[index] = 0.0;
                        if (Nitrate[index]   < 0.0)   Nitrate[index] = 0.0;
                        if (Phosphate[index] < 0.0)   Phosphate[index] = 0.0;
                        if (Oxygen[index]    < 0.0)   Oxygen[index] = 0.0;
                        OxygenSaturation[index] = TemporaryOxygenSaturation[index];
                        if (OxygenSaturation[index] < 0.0) OxygenSaturation[index] = 0.0;
                        RaerationCoef[index] = TemporaryRaerationCoef[index];
                    }    
                 }
              }
          }
       }
    }
    Loads();
}


void TNutrients3D::Go()
{
    double Depth, AValue, MySalinity, MyTemperature, MyParameter;
    int index, UpperLayer;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
    UpperLayer = MyPEcoDynClass->GetNumberOfLayers() - 1;

    if (MyTransportPointer != NULL) {
       MyTransportPointer->Inquiry(GetEcoDynClassName(),AValue,0,"UpperLayer",ObjectCode);
       UpperLayer = ceil(AValue);
    }

    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
       {
          for (int LayerNr = UpperLayer; LayerNr >= 0; LayerNr--)
          {
             index = Get3DIndex(i,j,LayerNr);
             if (MyTransportPointer != NULL)
             {
                 MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                             index,
                                             "Box depth",
                                             ObjectCode);
             }
             else Depth = MyPEcoDynClass->GetBoxDepth(index);
             if (
                    (Depth > CriticalDepthForLandBoundary) &&
                    (BoxArray[index].Dummybool1) &&
                    (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
                    (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) ||  //No northern ocean boundary
                (
                       (Depth > 0.0) &&
                       (MyTransportPointer == NULL)
                    )
                 )
             {
                Nitrification(index);
                DeNitrification(index);
                if (MyTransportPointer != NULL)
                     MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,index,"Salinity",ObjectCode);
                else
                    MySalinity = 0.0;
                if (MyWaterTemperaturePointer != NULL)
                    MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyTemperature,index,"Water temperature",ObjectCode);
                else MyTemperature = 15.0;
                MyParameter = -1;
                bool MyBoolean = 1;
                TemporaryOxygenSaturation[index] = PChemistry->OxygenSaturationConstants(MySalinity, MyTemperature,MyParameter, MyBoolean);
                TemporaryOxygenSaturation[index] = TemporaryOxygenSaturation[index] * OXYGENmlTOmg;//Converto to mg/l
                if (LayerNr == UpperLayer)
                  Raeration(index,Depth);
                NitrogenMineralization(index);
                PhosphorusMineralization(index);
                CarbonMineralization(index);
             }
          }
       }
    }
}


void TNutrients3D::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    //Debugger(NumberOfLoads);
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
                      "EcoDynamo Alert - TNutrients3D::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TNutrients3D::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files." << endl;
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

        //if (Depth > CriticalDepthForLandBoundary)
        if (Depth > 0.0)
        {
            NH4Load[index] = ALoadRecord[j].NH4Load[ADay-1];
            //Debugger(NH4Load[index]);
            NO3Load[index] = ALoadRecord[j].NO3Load[ADay-1];
            NO2Load[index] = ALoadRecord[j].NO2Load[ADay-1];
            PO4Load[index] = ALoadRecord[j].PO4Load[ADay-1];
            SiO2Load[index] = ALoadRecord[j].SiO2Load[ADay-1];
            O2Load[index] = ALoadRecord[j].O2Load[ADay-1];
        }
    }
    //Debugger(NumberOfRiverLoads);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {

        index = Get3DIndex(ALoadRiverRecord[j].LineCoordinate, ALoadRiverRecord[j].ColumnCoordinate, ALoadRiverRecord[j].LayerCoordinate);
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TNutrients3D::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TNutrients3D::Loads (River) - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files." << endl;
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


        //if (Depth > CriticalDepthForLandBoundary)
        if (Depth > 0.0)
        {
            Ammonia[index] = ALoadRiverRecord[j].NH4RiverLoad[ADay-1];
            Nitrate[index] = ALoadRiverRecord[j].NO3RiverLoad[ADay-1];
            Nitrite[index] = ALoadRiverRecord[j].NO2RiverLoad[ADay-1];
            Phosphate[index] = ALoadRiverRecord[j].PO4RiverLoad[ADay-1];
            Silica[index] = ALoadRiverRecord[j].SiO2RiverLoad[ADay-1];
            Oxygen[index] = ALoadRiverRecord[j].O2RiverLoad[ADay-1];
        }
    }
}

// AP, 2009.04.08 - imported from TRiaF2DNutrients
bool TNutrients3D::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Nutrients");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Nutrients");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Boundary NH4 concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary NO3 concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary NO2 concentration values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
    {
        PReadWrite->WriteCell(BoundaryNH4Concentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryNO3Concentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryNO2Concentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("Boundary phosphate concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary oxygen concentration values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(BoundaryPhosphateConcentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryOxygenConcentration[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("Ammonia values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrite values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phosphate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Oxygen values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Ammonia[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrite[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Phosphate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Oxygen[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    // Loads
    PReadWrite->WriteCell("LoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadLayers");          // from TNutrients3D
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
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("NH4Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2Load");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRecord[j].NH4Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].NO3Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].PO4Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].NO2Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    // Rivers
    PReadWrite->WriteCell("RiverLoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadLayers");          // from TNutrients3D
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
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("NH4RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("O2RiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].NH4RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].NO3RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PO4RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].NO2RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].O2RiverLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


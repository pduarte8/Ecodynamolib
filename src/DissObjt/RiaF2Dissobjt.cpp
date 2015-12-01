// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 08.09.2004
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
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"

// TRiaF2DNutrients Class
//
#ifndef _PORT_FORTRAN_
TRiaF2DNutrients::TRiaF2DNutrients(TEcoDynClass* APEcoDynClass, char* className)
							  : TSangoNutrients(APEcoDynClass, className)
{
    // Receive pointer to integrate
    BuildRiaF2DNutrients();
}
#endif
void TRiaF2DNutrients::BuildRiaF2DNutrients()
{
    PChemistry = new TChemistry();
    int X, Y, XV, YV;
    CriticalDepthForLandBoundary = MyPEcoDynClass->GetCriticalDepth(); // AP, 050309
//    CriticalDepthForLandBoundary = 0.05; //m
    knit = 0.2; /*d-1*/
    kdenit = 0.3; /*d-1*/
    knitO2 = 4; /*mg L-1*/
    kdenitO2 = 2; /*mg L-1*/
    kminO2 = 0.0; /*mg L-1*/
    Kt = 0.07;
    ProportionOfNH4FromDenitrification = 1.0; //(Chapelle et al. (1995) - Ecolog. Modell 80: 131-147) - Remaining 0.6 is lost as N2
    ARaerationCoefficient = 1.0;
    minRate = 0.0; //d-1
    NumberOfLoads = 0;
    NumberOfDaysForLoads = 0;
    NumberOfRiverLoads = 0;

    Oxygen = new double[NumberOfBoxes];
    OxygenFlux = new double[NumberOfBoxes];
    OxygenLoad = new double[NumberOfBoxes];
    OxygenSaturation = new double[NumberOfBoxes];
    TemporaryOxygenSaturation = new double[NumberOfBoxes];
    WaterNitrificationFlux = new double[NumberOfBoxes];
    WaterDeNitrificationFlux = new double[NumberOfBoxes];
    RaerationFlux = new double[NumberOfBoxes];
    RaerationCoef = new double[NumberOfBoxes];
    TemporaryRaerationCoef = new double[NumberOfBoxes];
    WaterNitrogenMineralizationFlux = new double[NumberOfBoxes];
    WaterPhosphorusMineralizationFlux = new double[NumberOfBoxes];
    WaterCarbonMineralizationFlux = new double[NumberOfBoxes];

    for (int i = 0; i < NumberOfBoxes; i++)
    {
       Oxygen[i] = 0.0;
       OxygenFlux[i] = 0.0;
       OxygenLoad[i] = 0.0;
       OxygenSaturation[i] = 1.0;
       TemporaryOxygenSaturation[i] = 1.0;
       WaterNitrificationFlux[i] = 0.0;
       WaterDeNitrificationFlux[i] = 0.0;
       RaerationFlux[i] = 0.0;
       RaerationCoef[i] = 0.0;
       TemporaryRaerationCoef[i] = 0.0;
       WaterNitrogenMineralizationFlux[i] = 0.0;
       WaterPhosphorusMineralizationFlux[i] = 0.0;
       WaterCarbonMineralizationFlux[i] = 0.0;
    }

    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Nutrients");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "Nutrients parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::BuildRiaF2DNutrients - Nutrients parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else {
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            char MyParameter[65];

            ParametersNameArray = new VNA[NumberOfParameters];

            NumberOfLoads = MyPEcoDynClass->GetNumberOfLoads();
            //Debugger(NumberOfLoads);
            NumberOfRiverLoads = MyPEcoDynClass->GetNumberOfRivers();
            //Debugger(NumberOfRiverLoads);
            NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
            //Debugger(NumberOfDaysForLoads);

            for (int i = 0; i < NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2+i, Y, MyParameter);
                strcpy(ParametersNameArray[i], MyParameter);
                if (strcmp(MyParameter, "CriticalDepthForLandBoundary") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, CriticalDepthForLandBoundary);
                }
                else if (strcmp(MyParameter, "NumberOfLoads") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfLoads);
                }
                else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfDaysForLoads);
                }
                else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfRiverLoads);
                }
                else if (strcmp(MyParameter, "NumberOfDaysForBoundaryTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfDaysForBoundaryTimeSeries);
                }
                else if (strcmp(MyParameter, "knit") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, knit);
                    //Debugger(knit);
                }
                else if (strcmp(MyParameter, "kdenit") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, kdenit);
                    //Debugger(kdenit);
                }
                else if (strcmp(MyParameter, "knitO2") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, knitO2);
                    //Debugger(knitO2);
                }
                else if (strcmp(MyParameter, "kdenitO2") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, kdenitO2);
                    //Debugger(kdenitO2);
                }
                else if (strcmp(MyParameter, "kminO2") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, kminO2);
                    //Debugger(kminO2);
                }
                else if (strcmp(MyParameter, "Kt") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, Kt);
                    //Debugger(Kt);
                }
                else if (strcmp(MyParameter, "minRate") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, minRate);
                    //Debugger(minRate);
                }
                else if (strcmp(MyParameter, "CriticalDepthForLandBoundary") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, CriticalDepthForLandBoundary);
                }
                else if (strcmp(MyParameter, "ProportionOfNH4FromDenitrification") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, ProportionOfNH4FromDenitrification);
                }
                else if (strcmp(MyParameter, "RaerationCoefficient") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, ARaerationCoefficient);
                }
                else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, FractionMineralizedToAmmonia);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters - Undefined object in Ria Formosa nutrients",
                    "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::BuildRiaF2DNutrients - Parameters - Undefined object in Ria Formosa nutrients" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    BoundaryPhosphateConcentration = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryOxygenConcentration = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryDOCConcentration = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryDONConcentration = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryDOPConcentration = new double[NumberOfDaysForBoundaryTimeSeries];
    for (int i = 0; i< NumberOfDaysForBoundaryTimeSeries; i++)
    {
        BoundaryPhosphateConcentration[i] = 0.0;
        BoundaryOxygenConcentration[i] = 0.0;
        BoundaryDOCConcentration[i] = 0.0;
        BoundaryDONConcentration[i] = 0.0;
        BoundaryDOPConcentration[i] = 0.0;
    }
    if (NumberOfLoads > 0)
        ALoadRecord = new LoadRecord[NumberOfLoads];
    if (NumberOfRiverLoads > 0)
        ALoadRiverRecord = new LoadRiverRecord[NumberOfRiverLoads];
    for (int i = 0; i < NumberOfLoads; i++)
    {
        ALoadRecord[i].NH4Load = new double[NumberOfDaysForLoads];
        ALoadRecord[i].NO3Load = new double[NumberOfDaysForLoads];
        ALoadRecord[i].NO2Load = new double[NumberOfDaysForLoads];
        ALoadRecord[i].PO4Load = new double[NumberOfDaysForLoads];
        ALoadRecord[i].SiO2Load = new double[NumberOfDaysForLoads];
        ALoadRecord[i].O2Load = new double[NumberOfDaysForLoads];
        ALoadRecord[i].DOCLoad = new double[NumberOfDaysForLoads];
        ALoadRecord[i].DONLoad = new double[NumberOfDaysForLoads];
        ALoadRecord[i].DOPLoad = new double[NumberOfDaysForLoads];
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
        ALoadRiverRecord[i].NH4RiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].NO3RiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].NO2RiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].PO4RiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].SiO2RiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].O2RiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].DOCRiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].DONRiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].DOPRiverLoad = new double[NumberOfDaysForLoads];
    }
    for (int i = 0; i < NumberOfLoads; i++)
    {
       ALoadRecord[i].LineCoordinate = 0;
       ALoadRecord[i].ColumnCoordinate = 0;
       for (int j = 0; j < NumberOfDaysForLoads; j++)
       {
          ALoadRecord[i].NH4Load[j] = 0.0;
          ALoadRecord[i].NO3Load[j] = 0.0;
          ALoadRecord[i].NO2Load[j] = 0.0;
          ALoadRecord[i].PO4Load[j] = 0.0;
          ALoadRecord[i].SiO2Load[j] = 0.0;
          ALoadRecord[i].O2Load[j] = 0.0;
          ALoadRecord[i].DOCLoad[j] = 0.0;
          ALoadRecord[i].DONLoad[j] = 0.0;
          ALoadRecord[i].DOPLoad[j] = 0.0;
       }
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
       ALoadRiverRecord[i].LineCoordinate = 0;
       ALoadRiverRecord[i].ColumnCoordinate = 0;
       for (int j = 0; j < NumberOfDaysForLoads; j++)
       {
          ALoadRiverRecord[i].NH4RiverLoad[j] = 0.0;
          ALoadRiverRecord[i].NO3RiverLoad[j] = 0.0;
          ALoadRiverRecord[i].NO2RiverLoad[j] = 0.0;
          ALoadRiverRecord[i].PO4RiverLoad[j] = 0.0;
          ALoadRiverRecord[i].SiO2RiverLoad[j] = 0.0;
          ALoadRiverRecord[i].O2RiverLoad[j] = 0.0;
          ALoadRiverRecord[i].DOCRiverLoad[j] = 0.0;
          ALoadRiverRecord[i].DONRiverLoad[j] = 0.0;
          ALoadRiverRecord[i].DOPRiverLoad[j] = 0.0;
       }
    }
	// Read in the variables
    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "Nutrients variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::BuildRiaF2DNutrients - Nutrients variables file missing." << endl;
#endif  // __BORLANDC__
    }
    else {
        char MyVariable[65], LoadName[100], RiverName[100];

        if (PReadWrite->FindString("Nutrients", X, Y))
        {
    // already initialized in TSangoNutrients
    //		PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
    //		VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                if (strcmp(MyVariable, "Ammonia") == 0)
                {
                    if (!PReadWrite->FindString("Ammonia values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Ammonia[j]);
                        Ammonia[j] = Ammonia[j] * CUBIC;
                    }
                }
                else if (strcmp(MyVariable, "Nitrate") == 0)
                {
                    if (!PReadWrite->FindString("Nitrate values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrate[j]);
                        Nitrate[j] = Nitrate[j] * CUBIC;
                    }
                }
                else if (strcmp(MyVariable, "Nitrite") == 0)
                {
                    if (!PReadWrite->FindString("Nitrite values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrite[j]);
                        Nitrite[j] = Nitrite[j] * CUBIC;
                    }
                }
                else if (strcmp(MyVariable, "Boundary NH4 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NH4 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNH4Concentration[j]);
                        BoundaryNH4Concentration[j] = BoundaryNH4Concentration[j] * CUBIC;
                        //Debugger(BoundaryNH4Concentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary NO3 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NO3 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNO3Concentration[j]);
                        BoundaryNO3Concentration[j] = BoundaryNO3Concentration[j] * CUBIC;
                    }
                }
                else if (strcmp(MyVariable, "Boundary NO2 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NO2 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNO2Concentration[j]);
                        BoundaryNO2Concentration[j] = BoundaryNO2Concentration[j] * CUBIC;
                    }
                }
                else if (strcmp(MyVariable, "Phosphate") == 0)
                {
                    if (!PReadWrite->FindString("Phosphate values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Phosphate[j]);
                        Phosphate[j] = Phosphate[j] * CUBIC;
                    }
                    //Debugger(Phosphate[NumberOfBoxes-1]);
                }
                else if (strcmp(MyVariable, "DOC") == 0)
                {
                    if (!PReadWrite->FindString("DOC values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DOC[j]);
                        DOC[j] = DOC[j] * CUBIC;
                    }
                    //Debugger(DOC[NumberOfBoxes-1]);
                }
                else if (strcmp(MyVariable, "DON") == 0)
                {
                    if (!PReadWrite->FindString("DON values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DON[j]);
                        DON[j] = DON[j] * CUBIC;
                    }
                    //Debugger(DON[NumberOfBoxes-1]);
                }
                else if (strcmp(MyVariable, "DOP") == 0)
                {
                    if (!PReadWrite->FindString("DOP values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DOP[j]);
                        DOP[j] = DOP[j] * CUBIC;
                    }
                    //Debugger(DOP[NumberOfBoxes-1]);
                }
                else if (strcmp(MyVariable, "Oxygen") == 0)
                {
                    if (!PReadWrite->FindString("Oxygen values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Oxygen[j]);
                    }
                    //Debugger(Oxygen[NumberOfBoxes-1]);
                }
                else if (strcmp(MyVariable, "Boundary phosphate concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary phosphate concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                       PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryPhosphateConcentration[j]);
                       BoundaryPhosphateConcentration[j]= BoundaryPhosphateConcentration[j] * CUBIC;
                    }
                }
                else if (strcmp(MyVariable, "Boundary oxygen concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary oxygen concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                       PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryOxygenConcentration[j]);
                    }
                    //Debugger(BoundaryOxygenConcentration[364]);
                }
                else if (strcmp(MyVariable, "Boundary DOC concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary DOC concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                       PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryDOCConcentration[j]);
                       BoundaryDOCConcentration[j]= BoundaryDOCConcentration[j] * CUBIC;
                    }
                    //Debugger(BoundaryDOCConcentration[364]);
                }
                else if (strcmp(MyVariable, "Boundary DON concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary DON concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                       PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryDONConcentration[j]);
                       BoundaryDONConcentration[j]= BoundaryDONConcentration[j] * CUBIC;
                    }
                    //Debugger(BoundaryDONConcentration[364]);
                }
                else if (strcmp(MyVariable, "Boundary DOP concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary DOP concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                       PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryDOPConcentration[j]);
                       BoundaryDOPConcentration[j]= BoundaryDOPConcentration[j] * CUBIC;
                    }
                    //Debugger(BoundaryDOPConcentration[364]);
                }
            }
    /*
            // AP, 2007.07.10
            //Read in loads - get location from TEcoDynClass
            //
            LoadsRecord* myLoads = MyPEcoDynClass->GetALoadRecord();
            for (int j = 0; j < NumberOfLoads; j++)
            {
                 ALoadRecord[j].LineCoordinate = myLoads[j].LineCoordinate;
                 ALoadRecord[j].ColumnCoordinate = myLoads[j].ColumnCoordinate;
                 strcpy(ALoadRecord[j].LoadName, myLoads[j].LoadName);
            }
            RiversRecord* myRivers = MyPEcoDynClass->GetARiverRecord();
            for (int j = 0; j < NumberOfRiverLoads; j++)
            {
                 ALoadRecord[j].LineCoordinate = myRivers[j].LineCoordinate;
                 ALoadRecord[j].ColumnCoordinate = myRivers[j].ColumnCoordinate;
                 strcpy(ALoadRecord[j].RiverName, myRivers[j].RiverName);
            }
      */
            // read loads values from file
            //
            if (PReadWrite->FindString("LoadLines", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRecord[j].LineCoordinate);
                    ALoadRecord[j].LineCoordinate = GridLines - ALoadRecord[j].LineCoordinate;
                    //Debugger(ALoadRecord[j].LineCoordinate);
                }
            }
            if (PReadWrite->FindString("LoadColumns", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRecord[j].ColumnCoordinate);
                    ALoadRecord[j].ColumnCoordinate = ALoadRecord[j].ColumnCoordinate - 1;
                    //Debugger(ALoadRecord[j].ColumnCoordinate);
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
            if (PReadWrite->FindString("NH4Load", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].NH4Load[Loads]); //Loads should be in umol/ m3 / s
                        ALoadRecord[j].NH4Load[Loads] = ALoadRecord[j].NH4Load[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("NO3Load", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].NO3Load[Loads]);
                        ALoadRecord[j].NO3Load[Loads] = ALoadRecord[j].NO3Load[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("NO2Load", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].NO2Load[Loads]);
                        ALoadRecord[j].NO2Load[Loads] = ALoadRecord[j].NO2Load[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("PO4Load", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].PO4Load[Loads]);
                        ALoadRecord[j].PO4Load[Loads] = ALoadRecord[j].PO4Load[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("O2Load", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].O2Load[Loads]);
                    }
                }
            }
            if (PReadWrite->FindString("DONLoad", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].DONLoad[Loads]);
                        ALoadRecord[j].DONLoad[Loads] = ALoadRecord[j].DONLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("DOPLoad", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].DOPLoad[Loads]);
                        ALoadRecord[j].DOPLoad[Loads] = ALoadRecord[j].DOPLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("DOCLoad", X, Y))
            {
                for (int j = 0; j < NumberOfLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRecord[j].DOCLoad[Loads]);
                        ALoadRecord[j].DOCLoad[Loads] = ALoadRecord[j].DOCLoad[Loads] * CUBIC;
                    }
                }
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
            if (PReadWrite->FindString("NH4RiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].NH4RiverLoad[Loads]); //Loads should be in umol/ m3 / s
                        ALoadRiverRecord[j].NH4RiverLoad[Loads] = ALoadRiverRecord[j].NH4RiverLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("NO3RiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].NO3RiverLoad[Loads]);
                        ALoadRiverRecord[j].NO3RiverLoad[Loads] = ALoadRiverRecord[j].NO3RiverLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("NO2RiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].NO2RiverLoad[Loads]);
                        ALoadRiverRecord[j].NO2RiverLoad[Loads] = ALoadRiverRecord[j].NO2RiverLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("PO4RiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].PO4RiverLoad[Loads]);
                        ALoadRiverRecord[j].PO4RiverLoad[Loads] = ALoadRiverRecord[j].PO4RiverLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("O2RiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].O2RiverLoad[Loads]);
                    }
                }
            }
            if (PReadWrite->FindString("DOCRiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].DOCRiverLoad[Loads]);
                        ALoadRiverRecord[j].DOCRiverLoad[Loads] = ALoadRiverRecord[j].DOCRiverLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("DONRiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].DONRiverLoad[Loads]);
                        ALoadRiverRecord[j].DONRiverLoad[Loads] = ALoadRiverRecord[j].DONRiverLoad[Loads] * CUBIC;
                    }
                }
            }
            if (PReadWrite->FindString("DOPRiverLoad", X, Y))
            {
                for (int j = 0; j < NumberOfRiverLoads; j++)
                {
                    for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                    {
                        PReadWrite->ReadNumber(X, Y + 1 + Loads + j * NumberOfDaysForLoads, ALoadRiverRecord[j].DOPRiverLoad[Loads]);
                        ALoadRiverRecord[j].DOPRiverLoad[Loads] = ALoadRiverRecord[j].DOPRiverLoad[Loads] * CUBIC;
                    }
                }
            }
        }
        else
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in Ria Formosa nutrients", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::BuildRiaF2DNutrients - Variables - Undefined object in Ria Formosa nutrients" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }
}



TRiaF2DNutrients::~TRiaF2DNutrients()
{
    freeMemory();
}

void TRiaF2DNutrients::freeMemory()
{
    try {
        if ((NumberOfLoads > 0) && (NumberOfDaysForLoads > 0)) {
            for (int i = 0; i < NumberOfLoads; i++)
            {
                delete [] ALoadRecord[i].NH4Load;
                delete [] ALoadRecord[i].NO3Load;
                delete [] ALoadRecord[i].NO2Load;
                delete [] ALoadRecord[i].PO4Load;
                delete [] ALoadRecord[i].SiO2Load;
                delete [] ALoadRecord[i].O2Load;
                delete [] ALoadRecord[i].DOCLoad;
                delete [] ALoadRecord[i].DONLoad;
                delete [] ALoadRecord[i].DOPLoad;
            }
            delete [] ALoadRecord;
        }

        if ((NumberOfRiverLoads > 0) && (NumberOfDaysForLoads > 0)) {
            for (int i= 0; i < NumberOfRiverLoads; i++)
            {
                delete [] ALoadRiverRecord[i].NH4RiverLoad;
                delete [] ALoadRiverRecord[i].NO3RiverLoad;
                delete [] ALoadRiverRecord[i].NO2RiverLoad;
                delete [] ALoadRiverRecord[i].PO4RiverLoad;
                delete [] ALoadRiverRecord[i].SiO2RiverLoad;
                delete [] ALoadRiverRecord[i].O2RiverLoad;
                delete [] ALoadRiverRecord[i].DOCRiverLoad;
                delete [] ALoadRiverRecord[i].DONRiverLoad;
                delete [] ALoadRiverRecord[i].DOPRiverLoad;
            }
            delete [] ALoadRiverRecord;
        }

        if (NumberOfBoxes > 0) {
            delete [] Oxygen;
            delete [] OxygenFlux;
            delete [] OxygenSaturation;
            delete [] TemporaryOxygenSaturation;
            delete [] OxygenLoad;
            delete [] WaterNitrificationFlux;
            delete [] WaterDeNitrificationFlux;
            delete [] RaerationFlux;
            delete [] RaerationCoef;
            delete [] TemporaryRaerationCoef;
            delete [] WaterNitrogenMineralizationFlux;
            delete [] WaterPhosphorusMineralizationFlux;
            delete [] WaterCarbonMineralizationFlux;
        }
        delete [] BoundaryOxygenConcentration;
        delete [] BoundaryPhosphateConcentration;
        delete [] BoundaryDOCConcentration;
        delete [] BoundaryDONConcentration;
        delete [] BoundaryDOPConcentration;
        delete PChemistry;
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
				cerr << "TRiaF2DNutrients::freeMemory -" << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TRiaF2DNutrients::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "NumberOfDaysForBoundaryTimeSeries") == 0)
    {
        value = NumberOfDaysForBoundaryTimeSeries;
    }
    else if (strcmp(MyParameter, "knit") == 0)
    {
        value = knit;
    }
    else if (strcmp(MyParameter, "kdenit") == 0)
    {
        value = kdenit;
    }
    else if (strcmp(MyParameter, "knitO2") == 0)
    {
        value = knitO2;
    }
    else if (strcmp(MyParameter, "kdenitO2") == 0)
    {
        value = kdenitO2;
    }
    else if (strcmp(MyParameter, "kminO2") == 0)
    {
        value = kminO2;
    }
    else if (strcmp(MyParameter, "Kt") == 0)
    {
        value = Kt;
    }
    else if (strcmp(MyParameter, "minRate") == 0)
    {
        value = minRate;
    }
    else if (strcmp(MyParameter, "CriticalDepthForLandBoundary") == 0)
    {
        value = CriticalDepthForLandBoundary;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
    {
        value = FractionMineralizedToAmmonia;
    }
    else
        value = 0.0;
    return value;
}

bool TRiaF2DNutrients::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "NumberOfDaysForBoundaryTimeSeries") == 0)
    {
        NumberOfDaysForBoundaryTimeSeries = value;
    }
    else if (strcmp(MyParameter, "knit") == 0)
    {
        knit = value;
    }
    else if (strcmp(MyParameter, "kdenit") == 0)
    {
        kdenit = value;
    }
    else if (strcmp(MyParameter, "knitO2") == 0)
    {
        knitO2 = value;
    }
    else if (strcmp(MyParameter, "kdenitO2") == 0)
    {
        kdenitO2 = value;
    }
    else if (strcmp(MyParameter, "kminO2") == 0)
    {
        kminO2 = value;
    }
    else if (strcmp(MyParameter, "Kt") == 0)
    {
        Kt = value;
    }
    else if (strcmp(MyParameter, "minRate") == 0)
    {
        minRate = value;
    }
    else if (strcmp(MyParameter, "CriticalDepthForLandBoundary") == 0)
    {
        CriticalDepthForLandBoundary = value;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
    {
        FractionMineralizedToAmmonia = value;
    }
    else
        rc = false;

    return rc;
}

void TRiaF2DNutrients::Inquiry(char* srcName, double &Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Ammonia") == 0)
		Value = Ammonia[MyBoxNumber] / CUBIC;       //micro mol L-1
	else if (strcmp(MyParameter, "Nitrite") == 0)
		Value = Nitrite[MyBoxNumber] / CUBIC;       //micro mol L-1
	else if (strcmp(MyParameter, "Nitrate") == 0)
		Value = Nitrate[MyBoxNumber] / CUBIC;       //micro mol L-1
	else if (strcmp(MyParameter, "DIN") == 0)
		Value = ( Ammonia[MyBoxNumber]
	    			 + Nitrite[MyBoxNumber]
		    		 + Nitrate[MyBoxNumber]
    			  ) / CUBIC;                        //micro mol L-1
    else if (strcmp(MyParameter, "Phosphate") == 0)
		Value = Phosphate[MyBoxNumber] / CUBIC;      //micro mol L-1
    else if (strcmp(MyParameter, "DOC") == 0)
		Value = DOC[MyBoxNumber] / CUBIC;      //micro mol L-1
    else if (strcmp(MyParameter, "DON") == 0)
		Value = DON[MyBoxNumber] / CUBIC;      //micro mol L-1
    else if (strcmp(MyParameter, "DOP") == 0)
		Value = DOP[MyBoxNumber] / CUBIC;      //micro mol L-1
    else if (strcmp(MyParameter, "Oxygen") == 0)
		Value = Oxygen[MyBoxNumber];                 //mg L-1
    else if (strcmp(MyParameter, "Oxygen saturation") == 0)
		Value = OxygenSaturation[MyBoxNumber];
    else if (strcmp(MyParameter, "WaterNitrificationFlux") == 0)
		Value = WaterNitrificationFlux[MyBoxNumber]/ CUBIC; //micro mol L-1 d-1
    else if (strcmp(MyParameter, "WaterDeNitrificationFlux") == 0)
		Value = WaterDeNitrificationFlux[MyBoxNumber]/ CUBIC;  //micro mol L-1 d-1
    else if (strcmp(MyParameter, "WaterNitrogenMineralizationFlux") == 0)
		Value = WaterNitrogenMineralizationFlux[MyBoxNumber]/ CUBIC;  //micro mol L-1 d-1
    else if (strcmp(MyParameter, "WaterPhosphorusMineralizationFlux") == 0)
		Value = WaterPhosphorusMineralizationFlux[MyBoxNumber]/ CUBIC;  //micro mol L-1 d-1
    else if (strcmp(MyParameter, "WaterCarbonMineralizationFlux") == 0)
		Value = WaterCarbonMineralizationFlux[MyBoxNumber]/ CUBIC;  //micro mol L-1 d-1
    else if (strcmp(MyParameter, "RaerationFlux") == 0)
		Value = RaerationFlux[MyBoxNumber];
    else if (strcmp(MyParameter, "RaerationCoef") == 0)
		Value = RaerationCoef[MyBoxNumber];
    else if (strcmp(MyParameter, "Boundary NH4 concentration") == 0)
		Value = BoundaryNH4Concentration[JulianDay - 1] / CUBIC;
	else if (strcmp(MyParameter, "Boundary NO3 concentration") == 0)
		Value = BoundaryNO3Concentration[JulianDay - 1] / CUBIC;
	else if (strcmp(MyParameter, "Boundary NO2 concentration") == 0)
		Value = BoundaryNO2Concentration[JulianDay - 1] / CUBIC;
    else if (strcmp(MyParameter, "Boundary DOC concentration") == 0)
		Value = BoundaryDOCConcentration[JulianDay - 1] / CUBIC;
    else if (strcmp(MyParameter, "Boundary DON concentration") == 0)
		Value = BoundaryDONConcentration[JulianDay - 1] / CUBIC;
    else if (strcmp(MyParameter, "Boundary DOP concentration") == 0)
		Value = BoundaryDOPConcentration[JulianDay - 1] / CUBIC;
    else if (strcmp(MyParameter, "Boundary oxygen concentration") == 0)
		Value = BoundaryOxygenConcentration[JulianDay - 1];
    else {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 7",
                      MB_OK | MB_ICONHAND);

#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::Inquiry 7 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}



void TRiaF2DNutrients::Update(char* srcName, double Value,
								 int BoxNumber,
								 char* ParameterName,
								 int AnObjectCode)
{

	// Receives value as umol per m3

	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

    if (strcmp(MyParameter, "Ammonia") == 0)
        NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Nitrite") == 0)
		NO2Flux[MyBoxNumber] = NO2Flux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Nitrate") == 0)
		NO3Flux[MyBoxNumber] = NO3Flux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Phosphate") == 0)
		PO4Flux[MyBoxNumber] = PO4Flux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Silica") == 0)
		SiO2Flux[MyBoxNumber] = SiO2Flux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "DIN") == 0)
    {
		DINProportion(MyBoxNumber);
		NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] + Value * NH4_TO_DIN;
		NO2Flux[MyBoxNumber] = NO2Flux[MyBoxNumber] + Value * NO2_TO_DIN;
		NO3Flux[MyBoxNumber] = NO3Flux[MyBoxNumber] + Value * NO3_TO_DIN;
    }
    else if (strcmp(MyParameter, "DON") == 0)
	    DONFlux[MyBoxNumber] = DONFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "DOP") == 0)
	    DOPFlux[MyBoxNumber] = DOPFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "DOC") == 0)
        DOCFlux[MyBoxNumber] = DOCFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Oxygen") == 0)
		OxygenFlux[MyBoxNumber] = OxygenFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
	    char Caption[129];
	    strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 4",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TRiaF2DNutrients::Update 4 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}

bool TRiaF2DNutrients::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    /*
     * From TSangoNutrients
     */
	if (strcmp(VariableName, "Ammonia") == 0)
    {
        Ammonia[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Nitrate") == 0)
    {
        Nitrate[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Nitrite") == 0)
    {
        Nitrite[BoxNumber] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DOC") == 0)
    {
        DOC[BoxNumber] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DON") == 0)
    {
        DON[BoxNumber] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DOP") == 0)
    {
        DOP[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary NH4 concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryNH4Concentration[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary NO3 concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryNO3Concentration[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary NO2 concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryNO2Concentration[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "Boundary DOC concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryDOCConcentration[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "Boundary DON concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryDONConcentration[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "Boundary DOP concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryDOPConcentration[j] = Value * CUBIC;
    }
    /*
     * from TRiaF2DNutrients
     */
	else if (strcmp(VariableName, "Phosphate") == 0)
    {
        Phosphate[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Oxygen") == 0)
    {
        Oxygen[BoxNumber] = Value;
    }
	else if (strcmp(VariableName, "Boundary phosphate concentration") == 0)
    {
        for (int j = 0; j < 365; j++)
            BoundaryPhosphateConcentration[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary oxygen concentration") == 0)
    {
        for (int j = 0; j < 365; j++)
            BoundaryOxygenConcentration[j] = Value;
    }
	else if (strcmp(VariableName, "NH4Load") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].NH4Load[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "NO3Load") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].NO3Load[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "NO2Load") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].NO2Load[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "PO4Load") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].PO4Load[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "O2Load") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].O2Load[j] = Value;
    }
    else if (strcmp(VariableName, "DOCLoad") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].DOCLoad[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DONLoad") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].DONLoad[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DOPLoad") == 0)
    {
        for (int i = 0; i < NumberOfLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRecord[i].DOPLoad[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "NH4RiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].NH4RiverLoad[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "NO3RiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].NO3RiverLoad[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "NO2RiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].NO2RiverLoad[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "PO4RiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].PO4RiverLoad[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "O2RiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].O2RiverLoad[j] = Value;
    }
    else if (strcmp(VariableName, "DOCRiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].DOCRiverLoad[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DONRiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].DONRiverLoad[j] = Value * CUBIC;
    }
    else if (strcmp(VariableName, "DOPRiverLoad") == 0)
    {
        for (int i = 0; i < NumberOfRiverLoads; i++)
            for (int j = 0; j < NumberOfDaysForLoads; j++)
                ALoadRiverRecord[i].DOPRiverLoad[j] = Value * CUBIC;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TRiaF2DNutrients::SaveVariables()
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
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary phosphate concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary oxygen concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary DOC concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary DON concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary DOP concentration values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
    {
        PReadWrite->WriteCell(BoundaryNH4Concentration[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryNO3Concentration[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryNO2Concentration[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryPhosphateConcentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryOxygenConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDOCConcentration[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDONConcentration[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDOPConcentration[j]/ CUBIC, 8);
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
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DON values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOP values");
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
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DOC[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DON[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DOP[j]/ CUBIC, 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    // Loads
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
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("NH4Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("O2Load");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOCLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DONLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOPLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRecord[j].NH4Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].NO3Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].NO2Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].PO4Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].O2Load[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DOCLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DONLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DOPLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    // Rivers
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
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("NH4RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("O2RiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOCRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DONRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOPRiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].NH4RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].NO3RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].NO2RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PO4RiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].O2RiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DOCRiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DONRiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DOPRiverLoad[Loads] / CUBIC, 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TRiaF2DNutrients::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Nutrients");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Nutrients");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save parameters' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfParameters; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "CriticalDepthForLandBoundary") == 0)
        {
            PReadWrite->WriteCell(CriticalDepthForLandBoundary, 6);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfDaysForBoundaryTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForBoundaryTimeSeries, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfLoads, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfDaysForLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForLoads, 0);
        }
        else if (strcmp(ParametersNameArray[i], "knit") == 0)
        {
            PReadWrite->WriteCell(knit, 6);
        }
        else if (strcmp(ParametersNameArray[i], "kdenit") == 0)
        {
            PReadWrite->WriteCell(kdenit, 6);
        }
        else if (strcmp(ParametersNameArray[i], "knitO2") == 0)
        {
            PReadWrite->WriteCell(knitO2, 6);
        }
        else if (strcmp(ParametersNameArray[i], "kdenitO2") == 0)
        {
            PReadWrite->WriteCell(kdenitO2, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Kt") == 0)
        {
            PReadWrite->WriteCell(Kt, 6);
        }
        else if (strcmp(ParametersNameArray[i], "minRate") == 0)
        {
            PReadWrite->WriteCell(minRate, 6);
        }
        else if (strcmp(ParametersNameArray[i], "CriticalDepthForLandBoundary") == 0)
        {
            PReadWrite->WriteCell(CriticalDepthForLandBoundary, 4);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfRiverLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfRiverLoads, 0);
        }
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TRiaF2DNutrients::Integrate()
{
	int i, j, index;
    double MyBoxDepth;


	Generic = DOC; GenericFlux = DOCFlux;
	Integration();
	DOC = Generic; DOCFlux = GenericFlux;

    Generic = DON; GenericFlux = DONFlux;
	Integration();
	DON = Generic; DONFlux = GenericFlux;

    Generic = DOP; GenericFlux = DOPFlux;
	Integration();
	DOP = Generic; DOPFlux = GenericFlux;

    Integration(Ammonia, NH4Flux,
                Nitrate, NO3Flux,
                Nitrite, NO2Flux,
                Phosphate, PO4Flux,
                Oxygen, OxygenFlux);

    // Transport the nutrients
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {

            /*MyTransportPointer->Go(Ammonia, NH4Load,
                               Nitrate, NO3Load,
                               Nitrite, NO2Load,
                               Phosphate, PO4Load,
                               Oxygen, O2Load,
                               0.0, 0.0);*/
            MyTransportPointer->Go(Ammonia,NH4Load,0.0,0.0);
            MyTransportPointer->Go(Nitrate,NO3Load,0.0,0.0);
            MyTransportPointer->Go(Nitrite,NO2Load,0.0,0.0);
            MyTransportPointer->Go(Phosphate,PO4Load,0.0,0.0);
            MyTransportPointer->Go(Oxygen,O2Load,0.0,0.0);
            MyTransportPointer->Go(DOC,DOCLoad,0.0,0.0);
            MyTransportPointer->Go(DON,DONLoad,0.0,0.0);
            MyTransportPointer->Go(DOP,DOPLoad,0.0,0.0);

		//Southern boundary
   	    for (j = pSubDomain->FirstColumn;
            j <= pSubDomain->LastColumn; j++)
   	    {
   		    //i = 0;
               i = pSubDomain->ILine[j];
               if (i <= pSubDomain->FLine[j])
               {
                  index = i * GridColumns + j;
      	          MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
         	                             index,
            	                          "Box depth",
               	                       ObjectCode);
      	          if (
      			(MyBoxDepth > CriticalDepthForLandBoundary) &&
            	        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
      		     )
      	          {
         	         Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
      		         Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
         	         Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                     Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                     Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
                     DOC[index] = BoundaryDOCConcentration[JulianDay-1];
                     DON[index] = BoundaryDONConcentration[JulianDay-1];
                     DOP[index] = BoundaryDOPConcentration[JulianDay-1];
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
      			(MyBoxDepth > CriticalDepthForLandBoundary) &&
            	        (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
      		     )
      	          {
      		         Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
      		         Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                     Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                     Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                     Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
                     DOC[index] = BoundaryDOCConcentration[JulianDay-1];
                     DON[index] = BoundaryDONConcentration[JulianDay-1];
                     DOP[index] = BoundaryDOPConcentration[JulianDay-1];
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
      			(MyBoxDepth > CriticalDepthForLandBoundary) &&
            	        (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
      		     )
      	          {
                     Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
                     Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
                     Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                     Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                     Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
                     DOC[index] = BoundaryDOCConcentration[JulianDay-1];
                     DON[index] = BoundaryDONConcentration[JulianDay-1];
                     DOP[index] = BoundaryDOPConcentration[JulianDay-1];
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
      			(MyBoxDepth > CriticalDepthForLandBoundary) &&
            	        (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
      		     )
      	          {
      		         Ammonia[index] = BoundaryNH4Concentration[JulianDay-1];
      		         Nitrate[index] = BoundaryNO3Concentration[JulianDay-1];
         	         Nitrite[index] = BoundaryNO2Concentration[JulianDay-1];
                     Phosphate[index] = BoundaryPhosphateConcentration[JulianDay-1];
                     Oxygen[index] = BoundaryOxygenConcentration[JulianDay-1];
                     DOC[index] = BoundaryDOCConcentration[JulianDay-1];
                     DON[index] = BoundaryDONConcentration[JulianDay-1];
                     DOP[index] = BoundaryDOPConcentration[JulianDay-1];
      	          }
               }
   	    }
    }
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    double MySalinity, MyTemperature, MyParameter;
    bool MyBoolean;
        for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
        {
           index = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
           if (Ammonia[index]   < 0.0)   Ammonia[index] = 0.0;
           if (Nitrite[index]   < 0.0)   Nitrite[index] = 0.0;
           if (Nitrate[index]   < 0.0)   Nitrate[index] = 0.0;
           if (Phosphate[index] < 0.0)   Phosphate[index] = 0.0;
           if (Oxygen[index]    < 0.0)   Oxygen[index] = 0.0;
           if (DOC[index]    < 0.0)   DOC[index] = 0.0;
           if (DON[index]    < 0.0)   DON[index] = 0.0;
           if (DOP[index]    < 0.0)   DOP[index] = 0.0;
           OxygenSaturation[index] = TemporaryOxygenSaturation[index];
           if (OxygenSaturation[index] < 0.0) OxygenSaturation[index] = 0.0;
           RaerationCoef[index] = TemporaryRaerationCoef[index];
        }
        Loads();
}

void TRiaF2DNutrients::Go()
{
    double Depth; int index;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;

    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
       index = pSubDomain->BoxNumber[k];

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
                Raeration(index,Depth);
                NitrogenMineralization(index);
                PhosphorusMineralization(index);
                CarbonMineralization(index);
              }
//           }
//        }
    }

}


void TRiaF2DNutrients::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (NumberOfDaysForLoads <= 366)
    {
       if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
       else
        ADay = 1;
    }
    else
    {
      ATime = ATime + MyPEcoDynClass->GetTimeStep();
      ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
    }
    //Debugger(ADay);
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
                      "EcoDynamo Alert - TRiaF2DNutrients::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files." << endl;
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

        if (Depth > 0.0)
        {
            NH4Load[index] = ALoadRecord[j].NH4Load[ADay-1];
            NO3Load[index] = ALoadRecord[j].NO3Load[ADay-1];
            NO2Load[index] = ALoadRecord[j].NO2Load[ADay-1];
            PO4Load[index] = ALoadRecord[j].PO4Load[ADay-1];
            SiO2Load[index] = ALoadRecord[j].SiO2Load[ADay-1];
            O2Load[index] = ALoadRecord[j].O2Load[ADay-1];
            DOCLoad[index] = ALoadRecord[j].DOCLoad[ADay-1];
            DONLoad[index] = ALoadRecord[j].DONLoad[ADay-1];
            DOPLoad[index] = ALoadRecord[j].DOPLoad[ADay-1];
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
                      "EcoDynamo Alert - TRiaF2DNutrients::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaF2DNutrients::RiverLoads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files." << endl;
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

        if (Depth > 0.0)
        {
            Ammonia[index] = ALoadRiverRecord[j].NH4RiverLoad[ADay-1];
            Nitrate[index] = ALoadRiverRecord[j].NO3RiverLoad[ADay-1];
            Nitrite[index] = ALoadRiverRecord[j].NO2RiverLoad[ADay-1];
            Phosphate[index] = ALoadRiverRecord[j].PO4RiverLoad[ADay-1];
            Silica[index] = ALoadRiverRecord[j].SiO2RiverLoad[ADay-1];
            Oxygen[index] = ALoadRiverRecord[j].O2RiverLoad[ADay-1];
            DOC[index] = ALoadRiverRecord[j].DOCRiverLoad[ADay-1];
            DON[index] = ALoadRiverRecord[j].DONRiverLoad[ADay-1];
            DOP[index] = ALoadRiverRecord[j].DOPRiverLoad[ADay-1];
        }
    }
}


void TRiaF2DNutrients::Nitrification(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   double OxygenNitrogenRatioInNitrification = 0.064; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (1995) - Ecolog. Modell 80: 131-147)
   WaterNitrificationFlux[MyBoxNumber] = knit * Ammonia[MyBoxNumber] * TemperatureLimitation(MyBoxNumber) * OxygenLimitation(MyBoxNumber, knitO2); //micro mol N-NO3 m-3 d-1
   NO3Flux[MyBoxNumber] = NO3Flux[MyBoxNumber] + WaterNitrificationFlux[MyBoxNumber] / DAYSTOSECONDS;      //micro mol N-NO3 m-3 s-1
   OxygenFlux[MyBoxNumber] = OxygenFlux[MyBoxNumber] - WaterNitrificationFlux[MyBoxNumber] / CUBIC * OxygenNitrogenRatioInNitrification / DAYSTOSECONDS; //mg O2 L-1 s-1
   NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] - WaterNitrificationFlux[MyBoxNumber] / DAYSTOSECONDS;//micro mol N m-3 s-1
}

void TRiaF2DNutrients::DeNitrification(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   WaterDeNitrificationFlux[MyBoxNumber] = kdenit * Nitrate[MyBoxNumber] * TemperatureLimitation(MyBoxNumber) * (1 - OxygenLimitation(MyBoxNumber, kdenitO2)); //micro mol N-NO3 m-3 d-1
   NO3Flux[MyBoxNumber] = NO3Flux[MyBoxNumber] - WaterDeNitrificationFlux[MyBoxNumber] / DAYSTOSECONDS; //micro mol N-NO3 m-3 s-1
   NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] + ProportionOfNH4FromDenitrification * WaterDeNitrificationFlux[MyBoxNumber] / /*CUBIC /*/ DAYSTOSECONDS;//micro mol N m-3 s-1
}

double TRiaF2DNutrients::OxygenLimitation(int ABoxNumber, double K)
{
   int MyBoxNumber; MyBoxNumber = ABoxNumber;
   return (Oxygen[MyBoxNumber] / (Oxygen[MyBoxNumber] + K));
}


void TRiaF2DNutrients::NitrogenMineralization(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   double MyMineralization;
   double OxygenNitrogenRatio = 0.212; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (2000) - Ecolog. Modell 127: 161-181)
   MyMineralization = minRate * DON[MyBoxNumber] * TemperatureLimitation(MyBoxNumber)* OxygenLimitation(MyBoxNumber, kminO2);  //umol m-3 d-1
   WaterNitrogenMineralizationFlux[MyBoxNumber] = MyMineralization;
   DONFlux[MyBoxNumber] = DONFlux[MyBoxNumber] - MyMineralization / DAYSTOSECONDS;  //umol m-3 s-1
   NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] + MyMineralization / DAYSTOSECONDS * FractionMineralizedToAmmonia;  //umol m-3 s-1
   OxygenFlux[MyBoxNumber] = OxygenFlux[MyBoxNumber] - MyMineralization / DAYSTOSECONDS * OxygenNitrogenRatio / CUBIC; //mg l-1 s-1
}

void TRiaF2DNutrients::PhosphorusMineralization(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   double MyMineralization;
   MyMineralization = minRate * DOP[MyBoxNumber] * TemperatureLimitation(MyBoxNumber) * OxygenLimitation(MyBoxNumber, kminO2);//umol m-3 d-1
   WaterPhosphorusMineralizationFlux[MyBoxNumber] = MyMineralization;
   DOPFlux[MyBoxNumber] = DOPFlux[MyBoxNumber] - MyMineralization / DAYSTOSECONDS; //umol m-3 s-1
   PO4Flux[MyBoxNumber] = PO4Flux[MyBoxNumber] + MyMineralization / DAYSTOSECONDS; //umol m-3 s-1
}

void TRiaF2DNutrients::CarbonMineralization(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   double MyMineralization;
   MyMineralization = minRate * DOC[MyBoxNumber] * TemperatureLimitation(MyBoxNumber) * OxygenLimitation(MyBoxNumber, kminO2); //umol m-3 d-1
   WaterCarbonMineralizationFlux[MyBoxNumber] = MyMineralization;
   DOCFlux[MyBoxNumber] = DOCFlux[MyBoxNumber] - MyMineralization / DAYSTOSECONDS;//umol m-3 s-1
   //Ligar ao ciclo do carbono
}

double TRiaF2DNutrients::TemperatureLimitation(int ABoxNumber)
{
   int MyBoxNumber; MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	  double WaterTemperature;
   	  MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      return exp(Kt * WaterTemperature);
   }
   else
   return 1.0;
}

void  TRiaF2DNutrients::Raeration(int ABoxNumber, double ADepth)
{
    double MySalinity, MyTemperature, MyParameter,MyRaerationCoefficient,
        DiffusiveFlux, MyTidalHeight,MyDepth, MyWindSpeed, MyUVelocity, MyVVelocity;
    bool MyBoolean;
    int i;
    i = ABoxNumber;
    MyDepth = ADepth;

    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();

    if (MyWindPointer != NULL)
       MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
                             ABoxNumber,
                             "Wind speed",
                             ObjectCode);
    else
       MyWindSpeed = 0.0;

    if (MyWaterTemperaturePointer != NULL)
    {
        MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyTemperature,i,
      	                                       "Water temperature",ObjectCode);
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,i,"Salinity",ObjectCode);
        }
        else
        {
           MySalinity = 0.0; MyUVelocity = 0.0; MyVVelocity = 0.0;
        }
        MyParameter = -1;
        MyBoolean = 1;
        TemporaryOxygenSaturation[i] = PChemistry->OxygenSaturationConstants(MySalinity, MyTemperature,
                                                             MyParameter, MyBoolean);
        TemporaryOxygenSaturation[i] = TemporaryOxygenSaturation[i] * OXYGENmlTOmg;//Converto to mg/l
        //MyRaerationCoefficient = RaerationCoefficient(sqrt(pow(MyUVelocity, 2.0) + pow(MyVVelocity, 2.0)),MyWindSpeed, MyDepth) / DAYSTOSECONDS; //Convert to ms-1
        if (MyWindPointer != NULL)
           TemporaryRaerationCoef[i] = RaerationCoefficient(MyWindSpeed, MyDepth);  //d-1
        else
           TemporaryRaerationCoef[i] = ARaerationCoefficient;
        MyRaerationCoefficient = TemporaryRaerationCoef[i] / DAYSTOSECONDS; //Convert to s-1
        DiffusiveFlux = MyRaerationCoefficient * (TemporaryOxygenSaturation[i] - Oxygen[i]);
        RaerationFlux[i] = DiffusiveFlux * DAYSTOSECONDS; // mg O2 / L / d
        OxygenFlux[i] = OxygenFlux[i] + DiffusiveFlux;
    }
    else
    {
#ifdef __BORLANDC__
        MessageBox(0,
                    "Water temperature object needed to run TRiaFormosaNutrients::Raeration",
                    "EcoDynamo alert",
                    MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TRiaF2DNutrients::Raeration - Water temperature object needed to run" << endl;
#endif  // __BORLANDC__
    }
}


double TRiaF2DNutrients::RaerationCoefficient(double AWindSpeed, double ADepth)  //d-1
{
    double MyWindSpeed, MyDepth;
    MyWindSpeed = AWindSpeed; //ms-1
    MyDepth = ADepth;         //m
    //return (0.641 + 0.0256 * pow (MyWindSpeed / 0.0447, 2)) / MyDepth; //d-1 see Chapelle et al. (2000) Ecolog Modell. 127: 161-181
    if (MyWindSpeed < 5.5) //ms-1           //EXAMS software, p. 36
       return 4.19 * 0.000001 * sqrt(MyWindSpeed) / MyDepth * DAYSTOSECONDS; //d-1
    else if ((MyWindSpeed >= 5.5) && (MyWindSpeed < 30.0))
       return 3.2 *  0.0000001 * pow(MyWindSpeed,2) / MyDepth * DAYSTOSECONDS;   //d-1
    else
       return 3.2 *  0.0000001 * pow(30.0,2) / MyDepth * DAYSTOSECONDS;   //d-1
}



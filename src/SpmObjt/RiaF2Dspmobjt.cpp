// SPM object CPP code file
// @ Pedro Duarte
// Rev. A 20/06/2005

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
#include "spmobjt.h"
#include "iodll.h"

// TRiaFormosaResuspendDeposit Class
//
// Constructors invoked outside EcoDyn shell...
//

/*TRiaFormosaResuspendDeposit::TRiaFormosaResuspendDeposit(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double totalExtinctionCoefficient,
        double aSPMConcentration[], double aPOMConcentration[],
        double aBoundarySPMConcentration[], double aBoundaryPOMConcentration[],
        int nParams, double karmanConstant, double dragCoefficient,
        double criticalVelocity, double sinkingVelocity, double nContents,
        double detritusBreakdown, double bottomSedimentOrganicContents,
        double aErosionRate[], double aRoughness[])
    :TSangoResuspendDeposit(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildRiaFormosaResuspendDeposit();

    NumberOfParameters = nParams;
    NumberOfVariables = nVars;
    VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
		SPMConcentration[j] = aSPMConcentration[j];
		POMConcentration[j] = aPOMConcentration[j];
		ErosionRate[j] = aErosionRate[j];
		Roughness[j] = aRoughness[j];
		TotalExtinctionCoefficient[j] = totalExtinctionCoefficient;
		BottomSedimentOrganicContents[j] = bottomSedimentOrganicContents;
    }
    for (int j = 0; j < 365; j++)
    {
       BoundarySPMConcentration[j] = aBoundarySPMConcentration[j];
       BoundaryPOMConcentration[j] = aBoundaryPOMConcentration[j];
    }
    KarmanConstant = karmanConstant;
    DragCoefficient = dragCoefficient;
    CriticalVelocity = criticalVelocity;
    SinkingVelocity = sinkingVelocity;
    NContents = nContents;
    DetritusBreakDown = detritusBreakdown;
}

TRiaFormosaResuspendDeposit::TRiaFormosaResuspendDeposit(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
                        :TSangoResuspendDeposit(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildRiaFormosaResuspendDeposit();
} */
// ...

TRiaFormosaResuspendDeposit::TRiaFormosaResuspendDeposit(TEcoDynClass* APEcoDynClass,
        char* className) : TSangoResuspendDeposit(APEcoDynClass, className)
{
    BuildRiaFormosaResuspendDeposit();
}

void TRiaFormosaResuspendDeposit::PreBuildRiaFormosaResuspendDeposit()
{
    CarbonToOrganics = 2.8;  //Calculated from the stochiometric ratio of organic matter (CH2O)106(NH3)16(H3PO4) Chapelle et al. / Ecol. Modell. 127 (2000) 161-181
    NumberOfLoads = 0;
    NumberOfDaysForLoads = 0;
    NumberOfRiverLoads = 0;
    NumberOfDaysForBoundaryTimeSeries = 365;
    OrganicFractionThatDissolves = 0.0;
    DetritalC = new double[NumberOfBoxes];
    DetritalN = new double[NumberOfBoxes];
    DetritalP = new double[NumberOfBoxes];
    LiveC = new double[NumberOfBoxes];
    LiveN = new double[NumberOfBoxes];
    LiveP = new double[NumberOfBoxes];
    DetritalCFlux = new double[NumberOfBoxes];
    DetritalNFlux = new double[NumberOfBoxes];
    DetritalPFlux = new double[NumberOfBoxes];
    LiveCFlux = new double[NumberOfBoxes];
    LiveNFlux = new double[NumberOfBoxes];
    LivePFlux = new double[NumberOfBoxes];
    BoundaryDetritalC = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryDetritalN = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryDetritalP = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryLiveC = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryLiveN = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryLiveP = new double[NumberOfDaysForBoundaryTimeSeries];
    Mineralization_C = new double[NumberOfBoxes];
    Mineralization_N = new double[NumberOfBoxes];
    Mineralization_P = new double[NumberOfBoxes];
    DepositionFlux = new double[NumberOfBoxes];
    ResuspensionFlux = new double[NumberOfBoxes];
    Dissolution_C = new double[NumberOfBoxes];
    Dissolution_N = new double[NumberOfBoxes];
    Dissolution_P = new double[NumberOfBoxes];
    for (int i = 0; i< NumberOfDaysForBoundaryTimeSeries; i++)
    {
        BoundaryDetritalC[i] = 0.0;
        BoundaryDetritalN[i] = 0.0;
        BoundaryDetritalP[i] = 0.0;
        BoundaryLiveC[i] = 0.0;
        BoundaryLiveN[i] = 0.0;
        BoundaryLiveP[i] = 0.0;
    }
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        DetritalC[i] = 0.0;
        DetritalN[i] = 0.0;
        DetritalP[i] = 0.0;
        LiveC[i] = 0.0;
        LiveN[i] = 0.0;
        LiveP[i] = 0.0;
        DetritalCFlux[i] = 0.0;
        DetritalNFlux[i] = 0.0;
        DetritalPFlux[i] = 0.0;
        LiveCFlux[i] = 0.0;
        LiveNFlux[i] = 0.0;
        LivePFlux[i] = 0.0;
        Mineralization_C[i] = 0.0; Mineralization_N[i] = 0.0; Mineralization_P[i] = 0.0;
        DepositionFlux[i] = 0.0; ResuspensionFlux[i] = 0.0;
        Dissolution_N[i] = 0.0; Dissolution_P[i] = 0.0; Dissolution_C[i] = 0.0;
    }
}

void TRiaFormosaResuspendDeposit::BuildRiaFormosaResuspendDeposit()
{
    PreBuildRiaFormosaResuspendDeposit();
    CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();

    // Read in the parameters
    ParameterCount = 0;  int X, Y;
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
           PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //
    // List of parameters initialized in TSangoResuspendDeposit
    //		ParametersNameArray = new VNA[NumberOfParameters];

           char MyParameter[65];
           NumberOfLoads = MyPEcoDynClass->GetNumberOfLoads();
           NumberOfRiverLoads = MyPEcoDynClass->GetNumberOfRivers();
           NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
           NumberOfMomentsForLoads = NumberOfDaysForLoads;
           //NumberOfDaysForBoundaryTimeSeries = MyPEcoDynClass->GetNumberOfDaysForFlowTimeSeries();
           NumberOfDaysForFlowTimeSeries = MyPEcoDynClass->GetNumberOfDaysForFlowTimeSeries();

           for (int i = Y; i < Y + NumberOfParameters; i++)
           {
              PReadWrite->ReadString(X+2, i, MyParameter);
    //
    // List of parameters initialized in TSangoResuspendDeposit
    //            strcpy(ParametersNameArray[i-Y], MyParameter);
              /*
               * this values are already initialised in "MyPEcoDynClass"
               *
               if (strcmp(MyParameter, "NumberOfLoads") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, NumberOfLoads);
                 //Debugger(NumberOfLoads);
              }
              if (strcmp(MyParameter, "NumberOfMomentsForLoads") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, NumberOfMomentsForLoads);
                 //Debugger(NumberOfMomentsForLoads);
              }
              else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
              {
                 PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfRiverLoads);
              }
              else if (strcmp(MyParameter, "NumberOfDaysForBoundaryTimeSeries") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, NumberOfDaysForBoundaryTimeSeries);
                 //Debugger(NumberOfDaysForBoundaryTimeSeries);
              }
              else */
              if (strcmp(MyParameter, "KminO2") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, KminO2);
                 //Debugger(KminO2);
              }
              else if (strcmp(MyParameter, "minCs") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, minCs);
                 //Debugger(minCs);
              }
              else if (strcmp(MyParameter, "minNs") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, minNs);
                 //Debugger(minNs);
              }
              else if (strcmp(MyParameter, "minPs") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, minPs);
                 //Debugger(minPs);
              }
              else if (strcmp(MyParameter, "Kt") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, Kt);
                 //Debugger(Kt);
              }
              else if (strcmp(MyParameter, "POMSinkingVelocity") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, POMSinkingVelocity);
                 //Debugger(POMSinkingVelocity);
              }
              else if (strcmp(MyParameter, "CriticalWaterDepth") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, CriticalWaterDepth);
                 //Debugger(CriticalWaterDepth);
              }
              else if (strcmp(MyParameter, "OrganicFractionThatDissolves") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, OrganicFractionThatDissolves);
                 //Debugger(OrganicFractionThatDissolves);
              }
              else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
              {
                 PReadWrite->ReadNumber(X + 3, i, FractionMineralizedToAmmonia);
                 //Debugger(FractionMineralizedToAmmonia);
              }
           }
        }
        else
        {
#ifdef __BORLANDC__ 
           MessageBox(0, "Parameters - Undefined object in Ria Formosa Suspended matter",
           "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaFormosaResuspendDeposit::BuildRiaFormosaResuspendDeposit - Parameters undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    TPMLoad = new double[NumberOfBoxes];
    POMLoad = new double[NumberOfBoxes];
    DetritalCLoad = new double[NumberOfBoxes];
    DetritalNLoad = new double[NumberOfBoxes];
    DetritalPLoad = new double[NumberOfBoxes];
    TPMRiverLoad = new double[NumberOfBoxes];
    POMRiverLoad = new double[NumberOfBoxes];
    DetritalCRiverLoad = new double[NumberOfBoxes];
    DetritalNRiverLoad = new double[NumberOfBoxes];
    DetritalPRiverLoad = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        TPMLoad[i] = 0.0;
        POMLoad[i] = 0.0;
        DetritalCLoad[i] = 0.0;
        DetritalNLoad[i] = 0.0;
        DetritalPLoad[i] = 0.0;
        TPMRiverLoad[i] = 0.0;
        POMRiverLoad[i] = 0.0;
        DetritalCRiverLoad[i] = 0.0;
        DetritalNRiverLoad[i] = 0.0;
        DetritalPRiverLoad[i] = 0.0;

    }
    if (NumberOfLoads > 0)
        ALoadRecord = new LoadRecord[NumberOfLoads];
    if (NumberOfRiverLoads > 0)
        ALoadRiverRecord = new LoadRiverRecord[NumberOfRiverLoads];
    for (int i = 0; i < NumberOfLoads; i++)
    {
        ALoadRecord[i].TPMLoad = new double[NumberOfMomentsForLoads];
        ALoadRecord[i].POMLoad = new double[NumberOfMomentsForLoads];
        ALoadRecord[i].DetritalCLoad = new double[NumberOfMomentsForLoads];
        ALoadRecord[i].DetritalNLoad = new double[NumberOfMomentsForLoads];
        ALoadRecord[i].DetritalPLoad = new double[NumberOfMomentsForLoads];
    }
    for (int i = 0; i < NumberOfLoads; i++)
    {
       ALoadRecord[i].LineCoordinate = 0;
       ALoadRecord[i].ColumnCoordinate = 0;
       for (int j = 0; j < NumberOfMomentsForLoads; j++)
       {
          ALoadRecord[i].TPMLoad[j] = 0.0;
          ALoadRecord[i].POMLoad[j] = 0.0;
          ALoadRecord[i].DetritalCLoad[j] = 0.0;
          ALoadRecord[i].DetritalNLoad[j] = 0.0;
          ALoadRecord[i].DetritalPLoad[j] = 0.0;
       }
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
        ALoadRiverRecord[i].TPMRiverLoad = new double[NumberOfDaysForFlowTimeSeries];
        ALoadRiverRecord[i].POMRiverLoad = new double[NumberOfDaysForFlowTimeSeries];
        ALoadRiverRecord[i].DetritalCRiverLoad = new double[NumberOfDaysForFlowTimeSeries];
        ALoadRiverRecord[i].DetritalNRiverLoad = new double[NumberOfDaysForFlowTimeSeries];
        ALoadRiverRecord[i].DetritalPRiverLoad = new double[NumberOfDaysForFlowTimeSeries];
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
       ALoadRiverRecord[i].LineCoordinate = 0;
       ALoadRiverRecord[i].ColumnCoordinate = 0;
       for (int j = 0; j < NumberOfDaysForFlowTimeSeries; j++)
       {
          ALoadRiverRecord[i].TPMRiverLoad[j] = 0.0;
          ALoadRiverRecord[i].POMRiverLoad[j] = 0.0;
          ALoadRiverRecord[i].DetritalCRiverLoad[j] = 0.0;
          ALoadRiverRecord[i].DetritalNRiverLoad[j] = 0.0;
          ALoadRiverRecord[i].DetritalPRiverLoad[j] = 0.0;
       }
    }

    char MyVariable[65], LoadName[100], RiverName[100];
    int XV, YV, XL, YL;
	// Read in the variables
//    PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
           PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
           for (int i = 0; i < NumberOfVariables; i++)
           {
              PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
              if (strcmp(MyVariable, "DetritalC") == 0)
              {
                 if (!PReadWrite->FindString("DetritalC values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxes; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, DetritalC[j]);
                    //if (j == 5) Debugger(DetritalC[j]);
                 }
              }
              else if (strcmp(MyVariable, "DetritalN") == 0)
              {
                 if (!PReadWrite->FindString("DetritalN values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxes; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, DetritalN[j]);
                    //if (j == 5) Debugger(DetritalN[j]);
                 }
              }
              else if (strcmp(MyVariable, "DetritalP") == 0)
              {
                 if (!PReadWrite->FindString("DetritalP values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxes; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, DetritalP[j]);
                    //if (j == 5) Debugger(DetritalP[j]);
                 }
              }
              else if (strcmp(MyVariable, "LiveC") == 0)
              {
                 if (!PReadWrite->FindString("LiveC values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxes; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, LiveC[j]);
                 }
              }
              else if (strcmp(MyVariable, "LiveN") == 0)
              {
                 if (!PReadWrite->FindString("LiveN values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxes; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, LiveN[j]);
                 }
              }
              else if (strcmp(MyVariable, "LiveP") == 0)
              {
                 if (!PReadWrite->FindString("LiveP values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxes; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, LiveP[j]);
                 }
              }
              else if (strcmp(MyVariable, "BoundaryDetritalC") == 0)
              {
                 if (!PReadWrite->FindString("BoundaryDetritalC values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryDetritalC[j]);
                    //if (j == 5) Debugger(BoundaryDetritalC[j]);
                 }
              }
              else if (strcmp(MyVariable, "BoundaryDetritalN") == 0)
              {
                 if (!PReadWrite->FindString("BoundaryDetritalN values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryDetritalN[j]);
                    //if (j == 5) Debugger(BoundaryDetritalN[j]);
                 }
              }
              else if (strcmp(MyVariable, "BoundaryDetritalP") == 0)
              {
                 if (!PReadWrite->FindString("BoundaryDetritalP values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryDetritalP[j]);
                    //if (j == 5) Debugger(BoundaryDetritalP[j]);
                 }
              }
              else if (strcmp(MyVariable, "BoundaryLiveC") == 0)
              {
                 if (!PReadWrite->FindString("BoundaryLiveC values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryLiveC[j]);
                 }
              }
              else if (strcmp(MyVariable, "BoundaryLiveN") == 0)
              {
                 if (!PReadWrite->FindString("BoundaryLiveN values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryLiveN[j]);
                 }
              }
              else if (strcmp(MyVariable, "BoundaryLiveP") == 0)
              {
                 if (!PReadWrite->FindString("BoundaryLiveP values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryLiveP[j]);
                 }
              }
              /* already read in TSangoResuspendDeposit
               * ...
              else if (strcmp(MyVariable, "Boundary SPM concentration") == 0)
              {
                 for (int j = 0; j < 365; j++)
                 {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, BoundarySPMConcentration[j]);
                    //if (j == 5) Debugger(BoundarySPMConcentration[j]);
                 }
              }
              else if (strcmp(MyVariable, "Boundary POM concentration") == 0)
              {
                 for (int j = 0; j < 365; j++)
                 {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, BoundaryPOMConcentration[j]);
                    //if (j == 5) Debugger(BoundaryPOMConcentration[j]);
                 }
              }  */
           }

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
          if (PReadWrite->FindString("TPMLoad", X, Y))
          {
             //Debugger(NumberOfLoads); Debugger(NumberOfMomentsForLoads);
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfMomentsForLoads;
                for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRecord[j].TPMLoad[Loads]); //Loads should be in mg / L / s
                   //if ((j == 5) && (Loads == NumberOfMomentsForLoads - 1)) Debugger(ALoadRecord[j].TPMLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("POMLoad", X, Y))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfMomentsForLoads;
                for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRecord[j].POMLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("DetritalCLoad", X, Y))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfMomentsForLoads;
                for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRecord[j].DetritalCLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("DetritalNLoad", X, Y))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfMomentsForLoads;
                for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRecord[j].DetritalNLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("DetritalPLoad", X, Y))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfMomentsForLoads;
                for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRecord[j].DetritalPLoad[Loads]);
                }
             }
          }

          if (PReadWrite->FindString("RiverLoadLines", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRiverRecord[j].LineCoordinate);
                ALoadRiverRecord[j].LineCoordinate = GridLines - ALoadRiverRecord[j].LineCoordinate;
                //Debugger(ALoadRiverRecord[j].LineCoordinate);
             }
          }
          if (PReadWrite->FindString("RiverLoadColumns", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(X, Y + 1 + j, ALoadRiverRecord[j].ColumnCoordinate);
                ALoadRiverRecord[j].ColumnCoordinate = ALoadRiverRecord[j].ColumnCoordinate - 1;
                //Debugger(ALoadRiverRecord[j].ColumnCoordinate);
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
          if (PReadWrite->FindString("TPMRiverLoad", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForFlowTimeSeries;
                for (int Loads = 0; Loads < NumberOfDaysForFlowTimeSeries; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRiverRecord[j].TPMRiverLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
          if (PReadWrite->FindString("POMRiverLoad", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForFlowTimeSeries;
                for (int Loads = 0; Loads < NumberOfDaysForFlowTimeSeries; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRiverRecord[j].POMRiverLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("DetritalCRiverLoad", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForFlowTimeSeries;
                for (int Loads = 0; Loads < NumberOfDaysForFlowTimeSeries; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRiverRecord[j].DetritalCRiverLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("DetritalNRiverLoad", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForFlowTimeSeries;
                for (int Loads = 0; Loads < NumberOfDaysForFlowTimeSeries; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRiverRecord[j].DetritalNRiverLoad[Loads]);
                }
             }
          }
          if (PReadWrite->FindString("DetritalPRiverLoad", X, Y))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForFlowTimeSeries;
                for (int Loads = 0; Loads < NumberOfDaysForFlowTimeSeries; Loads++)
                {
                   PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,
                        ALoadRiverRecord[j].DetritalPRiverLoad[Loads]);
                   //if (Loads == NumberOfMomentsForLoads - 1) Debugger(ALoadRiverRecord[j].DetritalPRiverLoad[Loads]);
                }
             }
          }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "Undefined object - Suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaFormosaResuspendDeposit::BuildRiaFormosaResuspendDeposit - Variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TRiaFormosaResuspendDeposit::~TRiaFormosaResuspendDeposit()
{
    freeMemory();
}

void TRiaFormosaResuspendDeposit::freeMemory()
{
    try {
        if (NumberOfBoxes > 0) {
            delete []DetritalC;
            delete []DetritalN;
            delete []DetritalP;
            delete []LiveC;
            delete []LiveN;
            delete []LiveP;
            delete []DetritalCFlux;
            delete []DetritalNFlux;
            delete []DetritalPFlux;
            delete []LiveCFlux;
            delete []LiveNFlux;
            delete []LivePFlux;
            delete [] Mineralization_C;
            delete [] Mineralization_N;
            delete [] Mineralization_P;
            delete [] Dissolution_C;
            delete [] Dissolution_N;
            delete [] Dissolution_P;
            delete [] DepositionFlux;
            delete [] ResuspensionFlux;

            delete []TPMLoad;
            delete []POMLoad;
            delete []DetritalCLoad;
            delete []DetritalNLoad;
            delete []DetritalPLoad;
            delete []TPMRiverLoad;
            delete []POMRiverLoad;
            delete []DetritalCRiverLoad;
            delete []DetritalNRiverLoad;
            delete []DetritalPRiverLoad;
        }

        delete []BoundaryDetritalC;
        delete []BoundaryDetritalN;
        delete []BoundaryDetritalP;
        delete []BoundaryLiveC;
        delete []BoundaryLiveN;
        delete []BoundaryLiveP;

        if ((NumberOfLoads > 0) && (NumberOfMomentsForLoads > 0)) {
            for (int i = 0; i < NumberOfLoads; i++)
            {
                delete [] ALoadRecord[i].TPMLoad;
                delete [] ALoadRecord[i].POMLoad;
                delete [] ALoadRecord[i].DetritalCLoad;
                delete [] ALoadRecord[i].DetritalNLoad;
                delete [] ALoadRecord[i].DetritalPLoad;
            }
            delete [] ALoadRecord;
        }
        if ((NumberOfRiverLoads > 0) && (NumberOfDaysForFlowTimeSeries > 0)) {
            for (int i = 0; i < NumberOfRiverLoads; i++)
            {
                delete [] ALoadRiverRecord[i].TPMRiverLoad;
                delete [] ALoadRiverRecord[i].POMRiverLoad;
                delete [] ALoadRiverRecord[i].DetritalCRiverLoad;
                delete [] ALoadRiverRecord[i].DetritalNRiverLoad;
                delete [] ALoadRiverRecord[i].DetritalPRiverLoad;
            }
            delete [] ALoadRiverRecord;
        }
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
				cerr << "TRiaFormosaResuspendDeposit::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TRiaFormosaResuspendDeposit::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TSangoResuspendDeposit
    if (strcmp(MyParameter, "Karman constant") == 0)
    {
        value = KarmanConstant;
    }
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
    {
        value = DragCoefficient;
    }
    else if (strcmp(MyParameter, "Critical velocity") == 0)
    {
        value = CriticalVelocity;
    }
    else if (strcmp(MyParameter, "Sinking velocity") == 0)
    {
        value = SinkingVelocity;
    }
    else if (strcmp(MyParameter, "Erosion rate") == 0)
    {
        value = ErosionRate[0];
    }
    else if (strcmp(MyParameter, "Bottom sediment organic contents") == 0)
    {
        value = BottomSedimentOrganicContents[0];
    }
    else if (strcmp(MyParameter, "Mineralization rate") == 0)
    {
        value = DetritusBreakDown;
    }
    else if (strcmp(MyParameter, "N contents") == 0)
    {
        value = NContents;
    }
    else if (strcmp(MyParameter, "Roughness") == 0)
    {
        value = Roughness[0];
    }

    // parameters from TRiaFormosaResuspendDeposit
    else if (strcmp(MyParameter, "KminO2") == 0)
    {
        value = KminO2;
    }
    else if (strcmp(MyParameter, "minCs") == 0)
    {
        value = minCs;
    }
    else if (strcmp(MyParameter, "minNs") == 0)
    {
        value = minNs;
    }
    else if (strcmp(MyParameter, "minPs") == 0)
    {
        value = minPs;
    }
    else if (strcmp(MyParameter, "Kt") == 0)
    {
        value = Kt;
    }
    else if (strcmp(MyParameter, "POMSinkingVelocity") == 0)
    {
        value = POMSinkingVelocity;
    }
    else if (strcmp(MyParameter, "CriticalWaterDepth") == 0)
    {
        value = CriticalWaterDepth;
    }
    else if (strcmp(MyParameter, "OrganicFractionThatDissolves") == 0)
    {
        value = OrganicFractionThatDissolves;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
    {
        value = FractionMineralizedToAmmonia;
    }
    else
        value = 0.0;
    return value;
}

bool TRiaFormosaResuspendDeposit::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TSangoResuspendDeposit
    if (strcmp(MyParameter, "Karman constant") == 0)
    {
        KarmanConstant = value;
    }
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
    {
        DragCoefficient = value;
    }
    else if (strcmp(MyParameter, "Critical velocity") == 0)
    {
        CriticalVelocity = value;
    }
    else if (strcmp(MyParameter, "Sinking velocity") == 0)
    {
        SinkingVelocity = value;
    }
    else if (strcmp(MyParameter, "Erosion rate") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            ErosionRate[j] = value;
    }
    else if (strcmp(MyParameter, "Bottom sediment organic contents") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            BottomSedimentOrganicContents[j] = value;
    }
    else if (strcmp(MyParameter, "Mineralization rate") == 0)
    {
        DetritusBreakDown = value;
    }
    else if (strcmp(MyParameter, "N contents") == 0)
    {
        NContents = value;
    }
    else if (strcmp(MyParameter, "Roughness") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Roughness[j] = value;
    }

    // parameters from TRiaFormosaResuspendDeposit
    else if (strcmp(MyParameter, "KminO2") == 0)
    {
        KminO2 = value;
    }
    else if (strcmp(MyParameter, "minCs") == 0)
    {
        minCs = value;
    }
    else if (strcmp(MyParameter, "minNs") == 0)
    {
        minNs = value;
    }
    else if (strcmp(MyParameter, "minPs") == 0)
    {
        minPs = value;
    }
    else if (strcmp(MyParameter, "Kt") == 0)
    {
        Kt = value;
    }
    else if (strcmp(MyParameter, "POMSinkingVelocity") == 0)
    {
        POMSinkingVelocity = value;
    }
    else if (strcmp(MyParameter, "CriticalWaterDepth") == 0)
    {
        CriticalWaterDepth = value;
    }
    else if (strcmp(MyParameter, "OrganicFractionThatDissolves") == 0)
    {
        value = OrganicFractionThatDissolves;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
    {
        value = FractionMineralizedToAmmonia;
    }
    else
        rc = false;

    return rc;
}


// AP, 2007.07.11
bool TRiaFormosaResuspendDeposit::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
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

    /*
     * save variables' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Extinction coefficient Ktot") == 0)
        {
            PReadWrite->WriteCell(TotalExtinctionCoefficient[0], 8);
            break;
        }
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    /*
     * save other variables' initial values
     */
    PReadWrite->WriteCell("Boundary SPM concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary POM concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryDetritalC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryDetritalN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryDetritalP values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryLiveC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryLiveN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryLiveP values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
    {
        PReadWrite->WriteCell(BoundarySPMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryPOMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDetritalC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDetritalN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDetritalP[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryLiveC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryLiveN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryLiveP[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("TPM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalP values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LiveC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LiveN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LiveP values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(SPMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(POMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DetritalC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DetritalN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DetritalP[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(LiveC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(LiveN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(LiveP[j], 8);
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

    PReadWrite->WriteCell("TPMLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POMLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalCLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalNLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalPLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRecord[j].TPMLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].POMLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DetritalCLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DetritalNLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DetritalPLoad[Loads], 8);
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

    PReadWrite->WriteCell("TPMRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POMRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalCRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalNRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalPRiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForFlowTimeSeries; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].TPMRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].POMRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DetritalCRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DetritalNRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DetritalPRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TRiaFormosaResuspendDeposit::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Karman constant") == 0)
        {
            PReadWrite->WriteCell(KarmanConstant, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Drag coefficient") == 0)
        {
            PReadWrite->WriteCell(DragCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Critical velocity") == 0)
        {
            PReadWrite->WriteCell(CriticalVelocity, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Sinking velocity") == 0)
        {
            PReadWrite->WriteCell(SinkingVelocity, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Erosion rate") == 0)
        {
            PReadWrite->WriteCell(ErosionRate[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Bottom sediment organic contents") == 0)
        {
            PReadWrite->WriteCell(BottomSedimentOrganicContents[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Mineralization rate") == 0)
        {
            PReadWrite->WriteCell(DetritusBreakDown, 8);
        }
        else if (strcmp(ParametersNameArray[i], "N contents") == 0)
        {
            PReadWrite->WriteCell(NContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Roughness") == 0)
        {
            PReadWrite->WriteCell(Roughness[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KminO2") == 0)
        {
            PReadWrite->WriteCell(KminO2, 8);
        }
        else if (strcmp(ParametersNameArray[i], "minCs") == 0)
        {
            PReadWrite->WriteCell(minCs, 8);
        }
        else if (strcmp(ParametersNameArray[i], "minNs") == 0)
        {
            PReadWrite->WriteCell(minNs, 8);
        }
        else if (strcmp(ParametersNameArray[i], "minPs") == 0)
        {
            PReadWrite->WriteCell(minPs, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Kt") == 0)
        {
            PReadWrite->WriteCell(Kt, 8);
        }
        else if (strcmp(ParametersNameArray[i], "POMSinkingVelocity") == 0)
        {
            PReadWrite->WriteCell(POMSinkingVelocity, 8);
        }
        else if (strcmp(ParametersNameArray[i], "CriticalWaterDepth") == 0)
        {
            PReadWrite->WriteCell(CriticalWaterDepth, 8);
        }
        else if (strcmp(ParametersNameArray[i], "OrganicFractionThatDissolves") == 0)
        {
           PReadWrite->WriteCell(OrganicFractionThatDissolves, 8);
        }
        else if (strcmp(ParametersNameArray[i], "FractionMineralizedToAmmonia") == 0)
        {
           PReadWrite->WriteCell(FractionMineralizedToAmmonia, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save other parameters' initial values
     */
    PReadWrite->WriteCell("Roughness values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Roughness[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}



void TRiaFormosaResuspendDeposit::Go()
{
    int index;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    //Loads();
    //Debugger(1);
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
    //Debugger(MyNumberOfBoxesInSubDomain);
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
       index = pSubDomain->BoxNumber[k];
       LiveC[index] = 0.0; LiveN[index] = 0.0; LiveP[index] = 0.0;
       if (MyTransportPointer != NULL)
       {
          MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,index,"Box depth",ObjectCode);
       }
       else
          ABoxDepth = MyPEcoDynClass->GetBoxDepth(index);
       if (
              (ABoxDepth > CriticalWaterDepth) &&
              (BoxArray[index].Dummybool1) &&
      	      (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
              (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
              (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
              (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) ||   //No northern ocean boundary
              (
                 (ABoxDepth > 0.0) &&
                 (MyTransportPointer == NULL)
              )
          )
         {
            Deposit(index);
            Ressuspend(index);
            CarbonMineralization(index);
            NitrogenMineralization(index);
            PhosphorusMineralization(index);
            DissolutionOfSuspendedOrganics(index);
            //ExtinctionCoefficientKTot(index);
         }
    }
}


void TRiaFormosaResuspendDeposit::Update(char* srcName, double Value,
						 int BoxNumber,
						 char* ParameterName,
						 int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);
   for (int i=0; i < NumberOfVariables; i++)
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
         if (strcmp(MyParameter, "TPM") == 0)
            SPMFlux[MyBoxNumber] = SPMFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "POM") == 0)
         {
            POMFlux[MyBoxNumber] = POMFlux[MyBoxNumber] + Value;
            /*tritalCFlux[MyBoxNumber] = DetritalCFlux[MyBoxNumber] + Value * DetritalC[MyBoxNumber] / POMConcentration[MyBoxNumber];
            DetritalNFlux[MyBoxNumber] = DetritalNFlux[MyBoxNumber] + Value * DetritalN[MyBoxNumber] / POMConcentration[MyBoxNumber];
            DetritalPFlux[MyBoxNumber] = DetritalPFlux[MyBoxNumber] + Value * DetritalP[MyBoxNumber] / POMConcentration[MyBoxNumber]; */
         }
         else if (strcmp(MyParameter, "DetritalC") == 0)
            DetritalCFlux[MyBoxNumber] = DetritalCFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "DetritalN") == 0)
            DetritalNFlux[MyBoxNumber] = DetritalNFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "DetritalP") == 0)
            DetritalPFlux[MyBoxNumber] = DetritalPFlux[MyBoxNumber] + Value;
         /*else if (strcmp(MyParameter, "LiveC") == 0)
            LiveCFlux[MyBoxNumber] = LiveCFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveN") == 0)
            LiveNFlux[MyBoxNumber] = LiveNFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveP") == 0)
            LivePFlux[MyBoxNumber] = LivePFlux[MyBoxNumber] + Value; */
         else if (strcmp(MyParameter, "LiveC") == 0)
            LiveC[MyBoxNumber] = LiveC[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveN") == 0)
            LiveN[MyBoxNumber] = LiveN[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveP") == 0)
            LiveP[MyBoxNumber] = LiveP[MyBoxNumber] + Value;
         return;
      }

#ifdef __BORLANDC__
   char Caption[129];
   strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
   MessageBox(0,Caption,"EcoDynamo Alert - Update 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaFormosaResuspendDeposit::Update 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
}


bool TRiaFormosaResuspendDeposit::SetVariableValue(char* srcName, double Value,
						 int BoxNumber,
						 char* VariableName)
{
  bool rc = true;

   LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "TPM") == 0)
      SPMConcentration[BoxNumber] = Value;
    else if (strcmp(VariableName, "POM") == 0)
      POMConcentration[BoxNumber] = Value;
    else if (strcmp(VariableName, "Extinction coefficient Ktot") == 0)
      TotalExtinctionCoefficient[BoxNumber] = Value;
    else if (strcmp(VariableName, "DetritalC") == 0)
      DetritalC[BoxNumber] = Value;
    else if (strcmp(VariableName, "DetritalN") == 0)
      DetritalN[BoxNumber] = Value;
    else if (strcmp(VariableName, "DetritalP") == 0)
      DetritalP[BoxNumber] = Value;
    else if (strcmp(VariableName, "LiveC") == 0)
      LiveC[BoxNumber] = Value;
    else if (strcmp(VariableName, "LiveN") == 0)
      LiveN[BoxNumber] = Value;
    else if (strcmp(VariableName, "LiveP") == 0)
      LiveP[BoxNumber] = Value;
    else if (strcmp(VariableName, "TPM_Mineralization_C") == 0)
      Mineralization_C[BoxNumber] = Value;
    else if (strcmp(VariableName, "TPM_Mineralization_N") == 0)
      Mineralization_N[BoxNumber] = Value;
    else if (strcmp(VariableName, "TPM_Mineralization_P") == 0)
      Mineralization_P[BoxNumber] = Value;
    else if (strcmp(VariableName, "DepositionFlux") == 0)
      DepositionFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "ResuspensionFlux") == 0)
      ResuspensionFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "Dissolution_C") == 0)
      Dissolution_C[BoxNumber] = Value;
    else if (strcmp(VariableName, "Dissolution_N") == 0)
      Dissolution_N[BoxNumber] = Value;
    else if (strcmp(VariableName, "Dissolution_P") == 0)
      Dissolution_P[BoxNumber] = Value;
    else if (strcmp(VariableName, "Boundary SPM concentration") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundarySPMConcentration[j] = Value;
    }
    else if (strcmp(VariableName, "Boundary POM concentration") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryPOMConcentration[j] = Value;
    }
    else if (strcmp(VariableName, "BoundaryDetritalC") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryDetritalC[j] = Value;
    }
    else if (strcmp(VariableName, "BoundaryDetritalN") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryDetritalN[j] = Value;
    }
    else if (strcmp(VariableName, "BoundaryDetritalP") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryDetritalP[j] = Value;
    }
    else if (strcmp(VariableName, "BoundaryLiveC") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryLiveC[j] = Value;
    }
    else if (strcmp(VariableName, "BoundaryLiveN") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryLiveN[j] = Value;
    }
    else if (strcmp(VariableName, "BoundaryLiveP") == 0)
    {
      for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
          BoundaryLiveP[j] = Value;
    }
    else
        rc = false;

    return rc;
}


void TRiaFormosaResuspendDeposit::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   if (strcmp(MyParameter, "TPM") == 0)
      Value = SPMConcentration[MyBoxNumber];
   else if (strcmp(MyParameter, "POM") == 0)
      Value = POMConcentration[MyBoxNumber];
   else if (strcmp(MyParameter, "Extinction coefficient Ktot") == 0)
      Value = TotalExtinctionCoefficient[MyBoxNumber];
   else if (strcmp(MyParameter, "DetritalC") == 0)
      Value = DetritalC[MyBoxNumber];
   else if (strcmp(MyParameter, "DetritalN") == 0)
      Value = DetritalN[MyBoxNumber];
   else if (strcmp(MyParameter, "DetritalP") == 0)
      Value = DetritalP[MyBoxNumber];
   else if (strcmp(MyParameter, "LiveC") == 0)
      Value = LiveC[MyBoxNumber];
   else if (strcmp(MyParameter, "LiveN") == 0)
      Value = LiveN[MyBoxNumber];
   else if (strcmp(MyParameter, "LiveP") == 0)
      Value = LiveP[MyBoxNumber];
   else if (strcmp(MyParameter, "TPM_Mineralization_C") == 0)
      Value = Mineralization_C[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "TPM_Mineralization_N") == 0)
      Value = Mineralization_N[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "TPM_Mineralization_P") == 0)
      Value = Mineralization_P[MyBoxNumber]; //mg L-1 d-1
   else if (strcmp(MyParameter, "Dissolution_C") == 0)
      Value = Dissolution_C[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "Dissolution_N") == 0)
      Value = Dissolution_N[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "Dissolution_P") == 0)
      Value = Dissolution_P[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "DepositionFlux") == 0)
      Value = DepositionFlux[MyBoxNumber];
   else if (strcmp(MyParameter, "ResuspensionFlux") == 0)
      Value = ResuspensionFlux[MyBoxNumber];
   else if (strcmp(MyParameter, "Boundary SPM concentration") == 0)
      Value = BoundarySPMConcentration[JulianDay - 1];
   else if (strcmp(MyParameter, "Boundary POM concentration") == 0)
      Value = BoundaryPOMConcentration[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryDetritalC") == 0)
      Value = BoundaryDetritalC[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryDetritalN") == 0)
      Value = BoundaryDetritalN[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryDetritalP") == 0)
      Value = BoundaryDetritalP[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryLiveC") == 0)
      Value = BoundaryLiveC[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryLiveN") == 0)
      Value = BoundaryLiveN[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryLiveP") == 0)
      Value = BoundaryLiveP[JulianDay - 1];
   else
   {
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
            MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaFormosaResuspendDeposit::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TRiaFormosaResuspendDeposit::Integrate()
{

    double ABoxDepth; int index;
/*
    Generic = SPMConcentration; GenericFlux = SPMFlux;
    Integration();
    SPMConcentration = Generic; SPMFlux = GenericFlux;

    Generic = POMConcentration; GenericFlux = POMFlux;
    Integration();
    POMConcentration = Generic; POMFlux = GenericFlux;

    Generic = DetritalC; GenericFlux = DetritalCFlux;
    Integration();
    DetritalC = Generic; DetritalCFlux = GenericFlux;

    Generic = DetritalN; GenericFlux = DetritalNFlux;
    Integration();
    DetritalN = Generic; DetritalNFlux = GenericFlux;

    Generic = DetritalP; GenericFlux = DetritalPFlux;
    Integration();
    DetritalP = Generic; DetritalPFlux = GenericFlux;
*/
    /* AP, 2006.01.19
     * EcoDynClass: ammonia, nitrate, nitrite, phosphate and oxygen
     */
    Integration(SPMConcentration, SPMFlux,
                POMConcentration, POMFlux,
                DetritalC, DetritalCFlux,
                DetritalN, DetritalNFlux,
                DetritalP, DetritalPFlux);
    
    /*Generic = LiveC; GenericFlux = LiveCFlux;
    Integration();
    LiveC = Generic; LiveCFlux = GenericFlux;

    Generic = LiveN; GenericFlux = LiveNFlux;
    Integration();
    LiveN = Generic; LiveNFlux = GenericFlux;

    Generic = LiveP; GenericFlux = LivePFlux;
    Integration();
    LiveP = Generic; LivePFlux = GenericFlux;*/

    // Transport the SPM

    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        /*MyTransportPointer->Go(SPMConcentration, TPMLoad,
                              POMConcentration, POMLoad,
                              DetritalC, DetritalCLoad,
                              DetritalN, DetritalNLoad,
                              DetritalP, DetritalPLoad,
                              0.0, 0.0); */

        MyTransportPointer->Go(SPMConcentration, TPMLoad,0.0, 0.0);
        MyTransportPointer->Go(POMConcentration, POMLoad,0.0, 0.0);
        MyTransportPointer->Go(DetritalC, DetritalCLoad,0.0, 0.0);
        MyTransportPointer->Go(DetritalN, DetritalNLoad,0.0, 0.0);
        MyTransportPointer->Go(DetritalP, DetritalPLoad,0.0, 0.0);

        JulianDay = MyPEcoDynClass->GetJulianDay();
   	//Southern boundary
   	int i,j;
        for (j = pSubDomain->FirstColumn;
            j <= pSubDomain->LastColumn; j++)
        {
           i = pSubDomain->ILine[j];
           if (i <= pSubDomain->FLine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
              if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    //(BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
                 )
      	      {
                 SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                 POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                 DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                 DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                 DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	      }
           }
        }
      //Northern boundary
   	for (j = pSubDomain->FirstColumn;
            j <= pSubDomain->LastColumn; j++)
        {
           i = pSubDomain->FLine[j] /*- 1*/;
           if (i >= pSubDomain->ILine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
      	      if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    //(BoxArray[index].Dummybool2) &&
            	    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
                 )
      	      {
                 SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                 POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                 DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                 DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                 DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	      }
           }
   	}
      //Eastern boundary
   	for (i = pSubDomain->FirstLine;
            i <= pSubDomain->LastLine; i++)
        {
           j = pSubDomain->FColumn[i] /*- 1*/;
           if (j >= pSubDomain->IColumn[i])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
      	      if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    //(BoxArray[index].Dummybool2) &&
            	    (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
                 )
      	      {
                 SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                 POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                 DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                 DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                 DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	      }
           }
   	}
        //Western boundary
        for (i = pSubDomain->FirstLine;
            i <= pSubDomain->LastLine; i++)
   	    {
               j = pSubDomain->IColumn[i];
               if (j <= pSubDomain->FColumn[i])
               {
                  index = i * GridColumns + j;
                   MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
         	                             index,
            	                          "Box depth",
               	                       ObjectCode);
      	          if (
      			(ABoxDepth > CriticalWaterDepth) &&
                        //(BoxArray[index].Dummybool2) &&
            	        (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
      		     )
      	          {
      		     SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
      		     POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
         	     DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                     DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                     DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	          }
               }
   	    }
    }
    TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
    double PHYORG, MyPhytoBiomass, MyChlorophyllToBiomass;
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
      index = pSubDomain->BoxNumber[k];
      if (SPMConcentration[index] < 0.0) SPMConcentration[index] = 0.0;
      if (POMConcentration[index] < 0.0) POMConcentration[index] = 0.0;
      if (DetritalC[index] < 0.0) DetritalC[index] = 0.0;
      if (DetritalN[index] < 0.0) DetritalN[index] = 0.0;
      if (DetritalP[index] < 0.0) DetritalP[index] = 0.0;
      if (LiveC[index] < 0.0) LiveC[index] = 0.0;
      if (LiveN[index] < 0.0) LiveN[index] = 0.0;
      if (LiveP[index] < 0.0) LiveP[index] = 0.0;
      ExtinctionCoefficientKTot(index);
      //This is to prevent Chl POM to be greater than overall POM. It should not be necessary in normal situations
      /*if (MyPhytoPointer != NULL)
      {
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
                                 index,
                                 "Phytoplankton biomass",
                                 ObjectCode);
          MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToBiomass,
                                  index,
                                  "Chlorophyll to biomass",
                                  ObjectCode);
          // Phytoplankton returns its mass as ug chl a l-1
          // convert to mg l-1 of organic dry matter
          PHYORG = MyPhytoBiomass / CUBIC * MyChlorophyllToBiomass; //mg/l
          if (PHYORG > POMConcentration[index])
          {
             SPMConcentration[index] = SPMConcentration[index] + (PHYORG - POMConcentration[index]);
             POMConcentration[index] = POMConcentration[index] + (PHYORG - POMConcentration[index]);
          }
       }*/
    }
    Loads();
}

void TRiaFormosaResuspendDeposit::ExtinctionCoefficientKTot(int ABox)
{
   int MyBox = ABox;
   double MyPhytoBiomass, MyChlorophyllToCarbon;

   /*TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if (MyPhytoPointer != NULL)
   {
   	MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
            	                   MyBox,
               	                "Phytoplankton biomass",
                  	             ObjectCode);

      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToCarbon,
            	                   MyBox,
               	                "Chlorophyll to Carbon",
                  	             ObjectCode);

      MyPhytoBiomass = MyPhytoBiomass * MyChlorophyllToCarbon / CUBIC; //convertion from ug Chl / l to mg C / l
   }
   else
   	MyPhytoBiomass = 0.0;  */

   //TotalExtinctionCoefficient[MyBox] = 0.4 + 0.00004 * SPMConcentration[MyBox] + 0.0000293 * MyPhytoBiomass;
   //m-1 - Esta rela??o ? baseada numa regress?o estabelecida por Baretta e utilizada na Tese de Mestrado do Fred Gazeau};

   TotalExtinctionCoefficient[MyBox] = 0.0484 * SPMConcentration[MyBox] + 0.0243;
   //Debugger(TotalExtinctionCoefficient[MyBox]);
   //Este regress?o ? baseada numa regress?o estabelecida pelo Cedric Bacher
   // a partir de dados de uma camapanha em Sango em Abril de 99
}


void TRiaFormosaResuspendDeposit::Deposit(int ABox)
{
   TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
   double TotalDeposition, OrganicDeposition, CDeposition, NDeposition, PDeposition;
   double MyPhytoBiomass, MyChlorophyllToBiomass, PHYORG, MySinkingVelocity, MyPOMSinkingVelocity;
   int MyBox;
   MyBox = ABox;
   MySinkingVelocity = SinkingVelocity / DAYSTOSECONDS;
   MyPOMSinkingVelocity = POMSinkingVelocity / DAYSTOSECONDS;
   TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if (MyPhytoPointer != NULL)
   {
      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
                                MyBox,
               	                "Phytoplankton biomass",
                                ObjectCode);

      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToBiomass,
            			             MyBox,
               	                "Chlorophyll to biomass",
                  	             ObjectCode);
      // Phytoplankton returns its mass as ug chl a l-1
      // convert to mg l-1 of organic dry matter
      PHYORG = MyPhytoBiomass / CUBIC * MyChlorophyllToBiomass; //mg/l
   }
   else
      PHYORG = 0.0;

   if (POMConcentration[MyBox] >= PHYORG)
   {
      OrganicDeposition = MyPOMSinkingVelocity * (POMConcentration[MyBox] - PHYORG) / ABoxDepth;
      CDeposition = MyPOMSinkingVelocity * DetritalC[MyBox]/ ABoxDepth;
      NDeposition = MyPOMSinkingVelocity * DetritalN[MyBox]/ ABoxDepth;
      PDeposition = MyPOMSinkingVelocity * DetritalP[MyBox]/ ABoxDepth;
   }
   else
   {
      OrganicDeposition = 0.0;
      CDeposition = 0.0;
      NDeposition = 0.0;
      PDeposition = 0.0;
   }
   if (SPMConcentration[MyBox] > POMConcentration[MyBox])
      TotalDeposition = MySinkingVelocity * (SPMConcentration[MyBox] - PHYORG - POMConcentration[MyBox])  / ABoxDepth +
                        OrganicDeposition;   //mg / L / s
   else
      TotalDeposition = 0.0;

   if (MySedimentPointer != NULL)
   {
      //double SedimentDepth;
      //MySedimentPointer->Inquiry(GetEcoDynClassName(), SedimentDepth,MyBox,"LayerDepth 1",ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), CDeposition * CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "Corganic 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), NDeposition * CUBIC * /*SedimentDepth*/ABoxDepth, MyBox,"Norganic 1",ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), PDeposition * CUBIC * /*SedimentDepth*/ABoxDepth, MyBox, "Porganic 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), OrganicDeposition * CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "OrganicMass 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), (TotalDeposition - OrganicDeposition)* CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "InorganicMass 1", ObjectCode);
   }

   DepositionFlux[MyBox] = TotalDeposition * DAYSTOSECONDS; //mg TPM/L/d
   SPMFlux[MyBox] = SPMFlux[MyBox] - TotalDeposition;
   POMFlux[MyBox] = POMFlux[MyBox] - OrganicDeposition;
   DetritalCFlux[MyBox] = DetritalCFlux[MyBox] - CDeposition;
   DetritalNFlux[MyBox] = DetritalNFlux[MyBox] - NDeposition;
   DetritalPFlux[MyBox] = DetritalPFlux[MyBox] - PDeposition;
}


void TRiaFormosaResuspendDeposit::Ressuspend(int ABox)
{
   int MyBox;
   MyBox = ABox;
   double const TINNY = 0.000001;
   double TotalResuspension, TotalResuspensionPerArea, OrganicResuspension, SedimentCContents, SedimentNContents, SedimentPContents, AProduct,
          CarbonResuspension, NitrogenResuspension, PhosphorusResuspension, InorganicMass, OrganicMass, TotalSedimentMass, SedimentCarbonToOrganics,
          MinimalOrganicResuspension, CarbonLoss, NitrogenLoss, PhophorusLoss;

   CarbonResuspension = 0.0; NitrogenResuspension = 0.0; PhosphorusResuspension = 0.0;
   TotalResuspension = 0.0; OrganicResuspension = 0.0;

   TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
   if (ErosionRate[MyBox] > 0.0)
      TotalResuspensionPerArea = ErosionRate[MyBox] * ShearStress(MyBox);
   else
      TotalResuspensionPerArea = 0.0;
   TotalResuspension = TotalResuspensionPerArea / ABoxDepth; //Erosion rate in g / m2 / s,
                                                             //Resuspension in mg / L / s  or g / m3 / s
   if (MySedimentPointer != NULL)
   {
      MySedimentPointer->Inquiry(GetEcoDynClassName(), InorganicMass,MyBox,"InorganicMass 1"/*g / m2*/,ObjectCode);
      MySedimentPointer->Inquiry(GetEcoDynClassName(), OrganicMass,MyBox,"OrganicMass 1"/*g / m2*/,ObjectCode);
      TotalSedimentMass = InorganicMass + OrganicMass;
      if (TotalResuspensionPerArea * MyPEcoDynClass->GetTimeStep() > TotalSedimentMass)
         TotalResuspension = TotalSedimentMass / MyPEcoDynClass->GetTimeStep() / ABoxDepth;
      if (TotalSedimentMass <= 100.0)
         TotalResuspension = 0.0;
      if (TotalResuspension > 0.0)
      {
        MySedimentPointer->Inquiry(GetEcoDynClassName(), SedimentCContents,MyBox,"Corganic 1"/*microg / g*/,ObjectCode);
        MySedimentPointer->Inquiry(GetEcoDynClassName(), SedimentNContents,MyBox,"Norganic 1"/*microg / g*/,ObjectCode);
        MySedimentPointer->Inquiry(GetEcoDynClassName(), SedimentPContents,MyBox,"Porganic 1"/*microg / g*/,ObjectCode);
        double const ThresholdCarbonContents = 3000; //microg / g - o valor para sedimentos arenosos ? de cerca de 2400  microg / g
        double const ExpectableCarbonContentsInSuspendedMatter = 0.05; //0.1 Proportion
        if (SedimentCContents > TINNY)
           SedimentCarbonToOrganics = OrganicMass / (SedimentCContents / (CUBIC * CUBIC) * TotalSedimentMass);
        else
           SedimentCarbonToOrganics = 0.0;
        if (SedimentCarbonToOrganics > TINNY)
        {
           if  (SedimentCContents < ThresholdCarbonContents){
               OrganicResuspension =  OrganicMass / TotalSedimentMass * TotalResuspension;
               CarbonResuspension = OrganicResuspension / SedimentCarbonToOrganics;
               NitrogenResuspension = CarbonResuspension * SedimentNContents / SedimentCContents;
               PhosphorusResuspension = CarbonResuspension * SedimentPContents / SedimentCContents;
           }
           else {
               MinimalOrganicResuspension = OrganicMass / TotalSedimentMass * TotalResuspension;
               OrganicResuspension = MAX(ExpectableCarbonContentsInSuspendedMatter * SedimentCarbonToOrganics, OrganicMass / TotalSedimentMass)* TotalResuspension;
               CarbonResuspension = OrganicResuspension / SedimentCarbonToOrganics;
               NitrogenResuspension = CarbonResuspension * SedimentNContents / SedimentCContents;
               PhosphorusResuspension = CarbonResuspension * SedimentPContents / SedimentCContents;
               CarbonLoss = (OrganicResuspension - MinimalOrganicResuspension) / SedimentCarbonToOrganics;
               NitrogenLoss = CarbonLoss * SedimentNContents / SedimentCContents;
               PhophorusLoss = CarbonLoss * SedimentPContents / SedimentCContents;
               MySedimentPointer->Update(GetEcoDynClassName(), -CarbonLoss * CUBIC * ABoxDepth/*mg / m2 / s*/, MyBox, "Corganic 1", ObjectCode);
               MySedimentPointer->Update(GetEcoDynClassName(), -NitrogenLoss * CUBIC * ABoxDepth, MyBox,"Norganic 1",ObjectCode);
               MySedimentPointer->Update(GetEcoDynClassName(), -PhophorusLoss * CUBIC * ABoxDepth, MyBox, "Porganic 1", ObjectCode);
           }
        }

        /*if (MyBox == 37148)
        {
           Debugger(OrganicResuspension);
           Debugger(CarbonResuspension);
        } */
        DetritalCFlux[MyBox] = DetritalCFlux[MyBox] + CarbonResuspension;
        DetritalNFlux[MyBox] = DetritalNFlux[MyBox] + NitrogenResuspension;
        DetritalPFlux[MyBox] = DetritalPFlux[MyBox] + PhosphorusResuspension;
        //OrganicResuspension = CarbonResuspension * SedimentCarbonToOrganics;
        POMFlux[MyBox] = POMFlux[MyBox] + OrganicResuspension;
        //MySedimentPointer->Update(GetEcoDynClassName(), -CarbonResuspension * CUBIC * ABoxDepth/*mg / m2 / s*/, MyBox, "Corganic 1", ObjectCode);
        //MySedimentPointer->Update(GetEcoDynClassName(), -NitrogenResuspension * CUBIC * ABoxDepth, MyBox,"Norganic 1",ObjectCode);
        //MySedimentPointer->Update(GetEcoDynClassName(), -PhosphorusResuspension * CUBIC * ABoxDepth, MyBox, "Porganic 1", ObjectCode);
        MySedimentPointer->Update(GetEcoDynClassName(), -OrganicResuspension * CUBIC * ABoxDepth/*mg / m2 / s*/, MyBox, "OrganicMass 1", ObjectCode);
        MySedimentPointer->Update(GetEcoDynClassName(), -(TotalResuspension - OrganicResuspension) * CUBIC * ABoxDepth/*mg / m2 / s*/, MyBox, "InorganicMass 1", ObjectCode);
      }
   }
   SPMFlux[MyBox] = SPMFlux[MyBox] + TotalResuspension;
   ResuspensionFlux[MyBox] = TotalResuspension * DAYSTOSECONDS; //mg TPM/L/d
}




void TRiaFormosaResuspendDeposit::NitrogenMineralization(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   double MyMineralization;
   double OxygenNitrogenRatio = 0.212; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (2000) - Ecolog. Modell 127: 161-181)
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   Mineralization_N[MyBoxNumber] = minNs * DetritalN[MyBoxNumber] * TemperatureLimitation(MyBoxNumber)* OxygenLimitation(MyBoxNumber, KminO2);  //mg l-1 d-1
   DetritalNFlux[MyBoxNumber] = DetritalNFlux[MyBoxNumber] - Mineralization_N[MyBoxNumber] / DAYSTOSECONDS;                          //mg l-1 s-1
   MyMineralization = Mineralization_N[MyBoxNumber] / DAYSTOSECONDS / NITROGENATOMICWEIGHT * CUBIC;  //micro mol N L-1 s-1
   if (MyNutrientPointer != NULL)
   {
      MyNutrientPointer->Update(GetEcoDynClassName(), MyMineralization * CUBIC  * FractionMineralizedToAmmonia,MyBoxNumber,"Ammonia",ObjectCode);      //Return as umol N / m3 / s
      MyNutrientPointer->Update(GetEcoDynClassName(), -MyMineralization * OxygenNitrogenRatio ,MyBoxNumber,"Oxygen",ObjectCode);    //Return as mg O2 / l / s
   }
}


void TRiaFormosaResuspendDeposit::PhosphorusMineralization(int ABoxNumber)
{
   int MyBoxNumber;
   double MyMineralization;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   Mineralization_P[MyBoxNumber] = minPs * DetritalP[MyBoxNumber] * TemperatureLimitation(MyBoxNumber) * OxygenLimitation(MyBoxNumber, KminO2);  //mg l-1 d-1
   DetritalPFlux[MyBoxNumber] = DetritalPFlux[MyBoxNumber] - Mineralization_P[MyBoxNumber] / DAYSTOSECONDS;                         //mg l-1 s-1
   MyMineralization = Mineralization_P[MyBoxNumber] / DAYSTOSECONDS / PHOSPHORUSATOMICWEIGHT * CUBIC;  //micro mol P L-1 s-1
   if (MyNutrientPointer != NULL)
   {
      MyNutrientPointer->Update(GetEcoDynClassName(), MyMineralization * CUBIC ,MyBoxNumber,"Phosphate",ObjectCode);      //Return as umol P / m3 / s
   }
}

void TRiaFormosaResuspendDeposit::CarbonMineralization(int ABoxNumber)
{
   int MyBoxNumber; double MyMineralization;
   double const TINNY = 0.000001;
   MyBoxNumber = ABoxNumber;
   Mineralization_C[MyBoxNumber] = minCs * DetritalC[MyBoxNumber] * TemperatureLimitation(MyBoxNumber) * OxygenLimitation(MyBoxNumber, KminO2); //mg l-1 d-1
   DetritalCFlux[MyBoxNumber] = DetritalCFlux[MyBoxNumber] - Mineralization_C[MyBoxNumber] / DAYSTOSECONDS;//mg l-1 s-1
   if (DetritalC[MyBoxNumber] > TINNY)
      CarbonToOrganics = POMConcentration[MyBoxNumber] / DetritalC[MyBoxNumber];
   POMFlux[MyBoxNumber] = POMFlux[MyBoxNumber] - Mineralization_C[MyBoxNumber] / DAYSTOSECONDS * CarbonToOrganics;                   //mg l-1 s-1
   SPMFlux[MyBoxNumber] = SPMFlux[MyBoxNumber] - Mineralization_C[MyBoxNumber] / DAYSTOSECONDS * CarbonToOrganics;
}

void TRiaFormosaResuspendDeposit::DissolutionOfSuspendedOrganics(int ABoxNumber)
{
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   int MyBoxNumber;
   double POMDissolution, CDissolution, NDissolution, PDissolution, MyOrganicFractionThatDissolves;
   MyBoxNumber = ABoxNumber;

   MyOrganicFractionThatDissolves = OrganicFractionThatDissolves * TemperatureLimitation(MyBoxNumber) / DAYSTOSECONDS;

   POMDissolution = MyOrganicFractionThatDissolves * POMConcentration[MyBoxNumber];

   POMFlux[MyBoxNumber] = POMFlux[MyBoxNumber] - POMDissolution;  //mg POM  L-1 s-1
   SPMFlux[MyBoxNumber] = SPMFlux[MyBoxNumber] - POMDissolution;  //mg SPM  L-1 s-1

   CDissolution = MyOrganicFractionThatDissolves * DetritalC[MyBoxNumber];
   Dissolution_C[MyBoxNumber] = CDissolution * DAYSTOSECONDS; //mg C  L-1 d-1
   DetritalCFlux[MyBoxNumber] = DetritalCFlux[MyBoxNumber] - CDissolution; //mg C  L-1 s-1

   NDissolution = MyOrganicFractionThatDissolves * DetritalN[MyBoxNumber];
   Dissolution_N[MyBoxNumber] = NDissolution * DAYSTOSECONDS; //mg N  L-1 d-1
   DetritalNFlux[MyBoxNumber] = DetritalNFlux[MyBoxNumber] - NDissolution; //mg N  L-1 s-1

   PDissolution = MyOrganicFractionThatDissolves * DetritalP[MyBoxNumber];
   Dissolution_P[MyBoxNumber] = PDissolution * DAYSTOSECONDS; //mg P  L-1 d-1
   DetritalPFlux[MyBoxNumber] = DetritalPFlux[MyBoxNumber] - PDissolution; //mg P  L-1 s-1

   if (MyNutrientPointer != NULL)
   {
      MyNutrientPointer->Update(GetEcoDynClassName(), CDissolution * CUBIC * CUBIC / CARBONATOMICWEIGHT,MyBoxNumber,"DOC",ObjectCode); //Return as umol C / m3 / s
      MyNutrientPointer->Update(GetEcoDynClassName(), NDissolution * CUBIC * CUBIC / NITROGENATOMICWEIGHT,MyBoxNumber,"DON",ObjectCode); //Return as umol N / m3 / s
      MyNutrientPointer->Update(GetEcoDynClassName(), PDissolution * CUBIC * CUBIC / PHOSPHORUSATOMICWEIGHT,MyBoxNumber,"DOP",ObjectCode);   //Return as umol P / m3 / s
   }
}

void TRiaFormosaResuspendDeposit::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (NumberOfMomentsForLoads <= 366)
    {
       if (JulianDay < NumberOfMomentsForLoads)
          ADay  = JulianDay;
       else
          ADay = 1;
    }
    else
    {
      ATime = ATime + MyPEcoDynClass->GetTimeStep();
      ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
    }
    //Debugger(AStartTime);Debugger(ATime);
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
                      "EcoDynamo Alert - TRiaFormosaResuspendDeposit::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaFormosaResuspendDeposit::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration files" << endl;
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
            TPMLoad[index] = ALoadRecord[j].TPMLoad[ADay-1];
            POMLoad[index] = ALoadRecord[j].POMLoad[ADay-1];
            DetritalCLoad[index] = ALoadRecord[j].DetritalCLoad[ADay-1];
            DetritalNLoad[index] = ALoadRecord[j].DetritalNLoad[ADay-1];
            DetritalPLoad[index] = ALoadRecord[j].DetritalPLoad[ADay-1];
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
                      "EcoDynamo Alert - TRiaFormosaResuspendDeposit::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaFormosaResuspendDeposit::Loads (Rivers) - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration files" << endl;
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
            SPMConcentration[index] = ALoadRiverRecord[j].TPMRiverLoad[ADay-1];
            POMConcentration[index] = ALoadRiverRecord[j].POMRiverLoad[ADay-1];
            DetritalC[index] = ALoadRiverRecord[j].DetritalCRiverLoad[ADay-1];
            DetritalN[index] = ALoadRiverRecord[j].DetritalNRiverLoad[ADay-1];
            DetritalP[index] = ALoadRiverRecord[j].DetritalPRiverLoad[ADay-1];
        }
    }
}

double TRiaFormosaResuspendDeposit::OxygenLimitation(int ABoxNumber, double K)
{
   int MyBoxNumber; MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyNutrientPointer != NULL)
   {
      double MyOxygen;
      MyNutrientPointer->Inquiry(GetEcoDynClassName(),MyOxygen,MyBoxNumber,"Oxygen",ObjectCode);
      return (MyOxygen / (MyOxygen + K));
   }
   else
      return 1.0;
}


double TRiaFormosaResuspendDeposit::TemperatureLimitation(int ABoxNumber)
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




// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 00.10.09
/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "zoobobjt.h"
#include "iodll.h"

// TChlamysFarreri Class
//
// Constructors invoked outside EcoDyn shell...
//

TChlamysFarreri::TChlamysFarreri(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aZooShellMass[],
        double aZooIndividualWeight[], double aZooClassDensity[],
        int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
        double allometricCoefficientForRespiration, double meatEnergeticContents,
        double shellEnergeticContents, double intrinsicRateOfMortality,
        double summerIntrinsicRateOfMortality, double maintenanceRespiration,
        double activityRespirationCoefficient, double proportionOfEnergyAllocatedToShell,
        double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
        double softTissueEnergyToTotalEnergy, double numberOfClasses,
        double totalFWToMeatDW)
    :TBivalvesForSango(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildChlamysFarreri();

    DryWeightOfAStandardAnimal = dryWeightOfAStandardAnimal;
    AllometricCoefficientForFeeding = allometricCoefficientForFeeding;
    AllometricCoefficientForRespiration = allometricCoefficientForRespiration;
    MeatEnergeticContents =  meatEnergeticContents;
    ShellEnergeticContents = shellEnergeticContents;
    IntrinsicRateOfMortality = intrinsicRateOfMortality;
    SummerIntrinsicRateOfMortality = summerIntrinsicRateOfMortality;
    MaintenanceRespiration = maintenanceRespiration;
    ActivityRespirationCoefficient = activityRespirationCoefficient;
    ProportionOfEnergyAllocatedToShell = proportionOfEnergyAllocatedToShell;
    ProportionOfEnergyAllocatedToSoftTissue = proportionOfEnergyAllocatedToSoftTissue;
    AmmoniaToJoules = ammoniaToJoules;
    SoftTissueEnergyToTotalEnergy = softTissueEnergyToTotalEnergy;
    NumberOfClasses = numberOfClasses;
    TotalFWToMeatDW = totalFWToMeatDW;

    BuildArrays();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfClasses; i++)
	{
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            int index = i * NumberOfBoxes + j;

    		ZooShellMass[index] = aZooShellMass[index];
    		ShellEnergeticTotal[index] =
                    aZooShellMass[index] * ShellEnergeticContents;
    		ZooIndividualWeight[index] = aZooIndividualWeight[index];
    		SoftTissueEnergeticTotal[index] =
                    aZooIndividualWeight[index] * MeatEnergeticContents;
    		ZooClassDensity[index] = aZooClassDensity[index];
        }
	}

    PosBuildChlamysFarreri();
}

TChlamysFarreri::TChlamysFarreri(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
        :TBivalvesForSango(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildChlamysFarreri();
}
// ...

TChlamysFarreri::TChlamysFarreri(TEcoDynClass* APEcoDynClass, char* className)
							 :	TBivalvesForSango(APEcoDynClass, className)
{

    NumberOfBoxesWithChlamysFarreri = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
            if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Chlamys farreri") == 0)
                    NumberOfBoxesWithChlamysFarreri++;

    }

    ChlamysFarreriRecord = new BenthicSpeciesRecord[NumberOfBoxesWithChlamysFarreri];
    int AIndex = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
            if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Chlamys farreri") == 0)
            {
                    strcpy(ChlamysFarreriRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
                    ChlamysFarreriRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
                    ChlamysFarreriRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
                    AIndex++;
            }
    }
    BuildChlamysFarreri();
}

void TChlamysFarreri::BuildChlamysFarreri()
{
    PreBuildChlamysFarreri();
    ReadParameters();
    BuildArrays();
    ReadVariables();
    PosBuildChlamysFarreri();
}

void TChlamysFarreri::PreBuildChlamysFarreri()
{
	DryWeightOfAStandardAnimal = 1.0;
    ProportionOfEnergyAllocatedToShell = 0.28;
    ProportionOfEnergyAllocatedToSoftTissue = 0.72;
    AmmoniaToJoules = 0.02428;
    SoftTissueEnergyToTotalEnergy = 0.81;

	ParameterCount = 0;
	VariableCount = 0;

    Columns = MyPEcoDynClass->GetNumberOfColumns();
    Lines = MyPEcoDynClass->GetNumberOfLines();
}

void TChlamysFarreri::ReadParameters()
{
    // Read in the parameters
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("ChlamysFarreri");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "ChlamysFarreri parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TChlamysFarreri::ReadParameters - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue;
        int X, Y;
        if (PReadWrite->FindString("Chlamys farreri", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, DryWeightOfAStandardAnimal);
                    //Debugger(DryWeightOfAStandardAnimal);
                }
                else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AllometricCoefficientForFeeding);
                    //Debugger(AllometricCoefficientForFeeding);
                }
                else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AllometricCoefficientForRespiration);
                    //Debugger(AllometricCoefficientForRespiration);
                }
                else if (strcmp(MyParameter, "Meat energetic contents") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MeatEnergeticContents);
                    //Debugger(MeatEnergeticContents);
                }
                else if (strcmp(MyParameter, "Shell energetic contents") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ShellEnergeticContents);
                    //Debugger(ShellEnergeticContents);
                }
                else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, IntrinsicRateOfMortality);
                    //Debugger(IntrinsicRateOfMortality);
                }
                else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SummerIntrinsicRateOfMortality);
                    //Debugger(SummerIntrinsicRateOfMortality);
                }
                else if (strcmp(MyParameter, "Maintenance respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaintenanceRespiration);
                    //Debugger(MaintenanceRespiration);
                }
                else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ActivityRespirationCoefficient	);
                    //Debugger(ActivityRespirationCoefficient);
                }
                else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ProportionOfEnergyAllocatedToShell);
                    //Debugger(ProportionOfEnergyAllocatedToShell);
                }
                else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ProportionOfEnergyAllocatedToSoftTissue);
                    //Debugger(ProportionOfEnergyAllocatedToSoftTissue);
                }
                else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AmmoniaToJoules);
                    //Debugger(AmmoniaToJoules);
                }
                else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SoftTissueEnergyToTotalEnergy);
                    //Debugger(SoftTissueEnergyToTotalEnergy);
                }
                else if (strcmp(MyParameter, "Number of classes") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfClasses);
                    //Debugger(NumberOfClasses);
                }
                else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TotalFWToMeatDW);
                    //Debugger(TotalFWToMeatDW);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "Undefined parameters in C. farreri parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TChlamysFarreri::ReadParameters - Undefined object in C. farreri parameters file" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

void TChlamysFarreri::BuildArrays()
{
/*  Generic                 = new double[NumberOfBoxes * NumberOfClasses];
    GenericFlux             = new double[NumberOfBoxes * NumberOfClasses];*/
    ZooClassBiomass         = new double[NumberOfBoxes * NumberOfClasses];
    ZooClassDensity         = new double[NumberOfBoxes * NumberOfClasses];
    ZooInitialWeight        = new double[NumberOfBoxes * NumberOfClasses];
    ZooIndividualWeight     = new double[NumberOfBoxes * NumberOfClasses];
    ZooClassBiomassFlux     = new double[NumberOfBoxes * NumberOfClasses];
    ZooClassDensityFlux     = new double[NumberOfBoxes * NumberOfClasses];
    ZooIndividualFlux       = new double[NumberOfBoxes * NumberOfClasses];
    ClassMaxLimits          = new double[NumberOfBoxes * NumberOfClasses];
    ClassMinLimits          = new double[NumberOfBoxes * NumberOfClasses];
    //SestonFiltration        = new double[NumberOfBoxes * NumberOfClasses];
    //pseudofaeces_production = new double[NumberOfBoxes * NumberOfClasses];
    //assimilation_efficiency = new double[NumberOfBoxes * NumberOfClasses];
    //transition_number       = new double[NumberOfClasses * NumberOfClasses];
    //transition_biomass      = new double[NumberOfClasses * NumberOfClasses];
    //transition_weight       = new double[NumberOfClasses * NumberOfClasses];
    //GrowthRate              = new double[NumberOfBoxes * NumberOfClasses];
    ClearanceRate           = new double[NumberOfBoxes * NumberOfClasses];
    Mortality               = new double[NumberOfBoxes * NumberOfClasses];
    ZooShellMass            = new double[NumberOfBoxes * NumberOfClasses];
    IRPHY                   = new double[NumberOfBoxes];
    IRDET                   = new double[NumberOfBoxes];
    FRTPM                   = new double[NumberOfBoxes];
    IRTPM                   = new double[NumberOfBoxes];
    RRTPM                   = new double[NumberOfBoxes];
    FRPIM                   = new double[NumberOfBoxes];
    IRPIM                   = new double[NumberOfBoxes];
    RRPIM                   = new double[NumberOfBoxes];
    OIR                     = new double[NumberOfBoxes];
    OCI                     = new double[NumberOfBoxes];
    OCPF                    = new double[NumberOfBoxes];
    NAEIO                   = new double[NumberOfBoxes];
    NEA                     = new double[NumberOfBoxes];
    NEB                     = new double[NumberOfBoxes];
    TOTALFRTPM              = new double[NumberOfBoxes];
    TOTALFRPOM              = new double[NumberOfBoxes];
    TOTALFRPHY              = new double[NumberOfBoxes];

    SoftTissueEnergeticTotal = new double[NumberOfBoxes * NumberOfClasses];
    ShellEnergeticTotal = new double[NumberOfBoxes * NumberOfClasses];
    ZooIndividualFluxForTheShell = new double[NumberOfBoxes * NumberOfClasses];
    OrganicPseudoFaeces   = new double[NumberOfBoxes];
    InorganicPseudofaeces = new double[NumberOfBoxes];
    ShellLength = new double[NumberOfBoxes * NumberOfClasses];

    for (int m = 0; m < NumberOfClasses; m++)
    {
        for (int n = 0; n < NumberOfBoxes; n++)
        {
            /*Generic[m * NumberOfBoxes + n]                 = 0;
            GenericFlux[m * NumberOfBoxes + n]             = 0;*/
            ZooClassBiomass[m * NumberOfBoxes + n] 	     = 0;
            ZooClassDensity[m * NumberOfBoxes + n]         = 0;
            ZooInitialWeight[m * NumberOfBoxes + n]        = 0;
            ZooIndividualWeight[m * NumberOfBoxes + n]     = 0;
            ZooClassBiomassFlux[m * NumberOfBoxes + n]     = 0;
            ZooClassDensityFlux[m * NumberOfBoxes + n]     = 0;
            ZooIndividualFlux[m * NumberOfBoxes + n]       = 0;
            //SestonFiltration[m * NumberOfBoxes + n]        = 0;
            //pseudofaeces_production[m * NumberOfBoxes + n] = 0;
            //assimilation_efficiency[m * NumberOfBoxes + n] = 0;
            //GrowthRate[m * NumberOfBoxes + n] 	           = 0;
            ClearanceRate[m * NumberOfBoxes + n] 	        = 0;
            Mortality[m * NumberOfBoxes + n] 	     		  = 0;
            ZooShellMass[m * NumberOfBoxes + n]            = 0;
          /*IRPHY[m * NumberOfBoxes + n]                   = 0;
            IRDET[m * NumberOfBoxes + n]                   = 0;
            FRTPM[m * NumberOfBoxes + n]                   = 0;
            IRTPM[m * NumberOfBoxes + n]                   = 0;
            RRTPM[m * NumberOfBoxes + n]            		  = 0;
            FRPIM[m * NumberOfBoxes + n]            		  = 0;
            IRPIM[m * NumberOfBoxes + n]            		  = 0;
            RRPIM[m * NumberOfBoxes + n]            		  = 0;
            OIR[m * NumberOfBoxes + n]            			  = 0;
            OCI[m * NumberOfBoxes + n]            			  = 0;
            OCPF[m * NumberOfBoxes + n]            		  = 0;
            NAEIO[m * NumberOfBoxes + n]            		  = 0;
            NEA[m * NumberOfBoxes + n]            			  = 0;
            NEB[m * NumberOfBoxes + n]            			  = 0;*/
            SoftTissueEnergeticTotal[m * NumberOfBoxes + n] = 0;
            ShellEnergeticTotal[m * NumberOfBoxes + n] = 0;
            ZooIndividualFluxForTheShell[m * NumberOfBoxes + n] = 0;
            ShellLength[m * NumberOfBoxes + n] = 0;
        }
    }

    for (int m = 0; m < NumberOfBoxes; m++)
    {
        OrganicPseudoFaeces[m] = 0.0;
        InorganicPseudofaeces[m] = 0.0;
        IRPHY[m]= 0.0;
        IRDET[m]= 0.0;
        FRTPM[m]= 0.0;
        IRTPM[m]= 0.0;
        RRTPM[m] = 0.0;
        FRPIM[m] = 0.0;
        IRPIM[m] = 0.0;
        RRPIM[m] = 0.0;
        OIR[m] = 0.0;
        OCI[m] = 0.0;
        OCPF[m] = 0.0;
        NAEIO[m] = 0.0;
        NEA[m] = 0.0;
        NEB[m] = 0.0;
        TOTALFRTPM[m] = 0.0;
        TOTALFRPOM[m] = 0.0;
        TOTALFRPHY[m] = 0.0;
    }
}

void TChlamysFarreri::ReadVariables()
{
    // Read in the variables
    char MyVariable[65];
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("ChlamysFarreri");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "ChlamysFarreri variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TChlamysFarreri::ReadVariables - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue;
        int X, Y;

        if (PReadWrite->FindString("Chlamys farreri", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
            }
            int k, j, RecordNumber, Index, offset, XV, YV;
            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
               if (strcmp(MyVariable, "C. farreri shell mass 1") == 0)         //g Shell dry weight / individual
               {
                  if (PReadWrite->FindString("C. farreri shell mass 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithChlamysFarreri; j++)
                          {
                              Index = ChlamysFarreriRecord[j].LineCoordinate * NumberOfColumns +
                                      ChlamysFarreriRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooShellMass[offset + Index] = MyValue;
                              ShellEnergeticTotal[offset + Index] = MyValue * ShellEnergeticContents;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithChlamysFarreri; j < k * NumberOfBoxesWithChlamysFarreri + NumberOfBoxesWithChlamysFarreri; j++)
                          {
                            Index = ChlamysFarreriRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    ChlamysFarreriRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooShellMass[offset + Index] = MyValue;
                            ShellEnergeticTotal[offset + Index] = MyValue * ShellEnergeticContents;
                            /*if (j == k * NumberOfBoxesWithChlamysFarreri + NumberOfBoxesWithChlamysFarreri - 1)
                               Debugger(ZooShellMass[k * NumberOfBoxes + j]); */
                            RecordNumber++;
                          }
                      }
                  }
               }
               else if (strcmp(MyVariable, "C. farreri meat weight 1") == 0)        //g Soft tissue dry weight / individual
               {
                  if (PReadWrite->FindString("C. farreri meat weight 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithChlamysFarreri; j++)
                          {
                              Index = ChlamysFarreriRecord[j].LineCoordinate * NumberOfColumns +
                                      ChlamysFarreriRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooIndividualWeight[offset + Index] = MyValue;
                              SoftTissueEnergeticTotal[offset + Index] = MyValue * MeatEnergeticContents;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithChlamysFarreri; j < k * NumberOfBoxesWithChlamysFarreri + NumberOfBoxesWithChlamysFarreri; j++)
                          {
                            Index = ChlamysFarreriRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    ChlamysFarreriRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooIndividualWeight[offset + Index] = MyValue;
                            SoftTissueEnergeticTotal[offset + Index] = MyValue * MeatEnergeticContents;
                            /*if (j == k * NumberOfBoxesWithChlamysFarreri + NumberOfBoxesWithChlamysFarreri - 1)
                               Debugger(ZooIndividualWeight[k * NumberOfBoxes + Index]);  */
                            RecordNumber++;
                          }
                      }
                  }
               }
               else if (strcmp(MyVariable, "C. farreri density 1") == 0)           //g soft + shell tissue DW / m2
               {
                  if (PReadWrite->FindString("C. farreri density 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithChlamysFarreri; j++)
                          {
                              Index = ChlamysFarreriRecord[j].LineCoordinate * NumberOfColumns +
                                      ChlamysFarreriRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooClassDensity[offset + Index] = MyValue;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithChlamysFarreri; j < k * NumberOfBoxesWithChlamysFarreri + NumberOfBoxesWithChlamysFarreri; j++)
                          {
                            Index = ChlamysFarreriRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    ChlamysFarreriRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooClassDensity[offset + Index] = MyValue;
                            /*if (ZooClassDensity[k * NumberOfBoxes + Index] != 0.0)
                               Debugger(ZooClassDensity[k * NumberOfBoxes + Index]); */
                            RecordNumber++;
                          }
                      }
                  }
               }
            }
        }
        else
#ifdef __BORLANDC__
				MessageBox(0, "Variables: Undefined object in C. farreri", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TChlamysFarreri::ReadVariables - Variables: Undefined object in C. farreri" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }
}

void TChlamysFarreri::PosBuildChlamysFarreri()
{

    for (int k = 0; k < NumberOfClasses; k++)
    {
   	    for (int j = 0; j < NumberOfBoxes; j++)
        {
      	    if (ZooIndividualWeight[k * NumberOfBoxes + j] != 0.0)
      		    ZooClassBiomass[k * NumberOfBoxes + j] =
                    ZooClassDensity[k * NumberOfBoxes + j] *
                        (ZooIndividualWeight[k * NumberOfBoxes + j]
                         + ZooShellMass[k * NumberOfBoxes + j]);
            else
                ZooClassBiomass[k * NumberOfBoxes + j] = 0.0;
        }
    }
    if (NumberOfClasses > 1)
   	    ClassIncrement = ClassMinLimits[1] - ClassMinLimits[0];
}


TChlamysFarreri::~TChlamysFarreri()
{
    freeMemory();
}

void TChlamysFarreri::freeMemory()
{
    try {
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfBoxesWithChlamysFarreri > 0) {
            delete [] ChlamysFarreriRecord;
        }

        if (NumberOfBoxes > 0) {
            delete [] ZooClassBiomass;
            delete [] ZooClassDensity;
            delete [] ZooInitialWeight;
            delete [] ZooIndividualWeight;
            delete [] ZooClassBiomassFlux;
            delete [] ZooClassDensityFlux;
            delete [] ZooIndividualFlux;
            delete [] ClassMaxLimits;
            delete [] ClassMinLimits;
           delete [] ClearanceRate;
           delete [] Mortality;
           delete [] ZooShellMass;
           delete [] IRPHY;
           delete [] IRDET;
           delete [] FRTPM;
           delete [] IRTPM;
           delete [] RRTPM;
           delete [] FRPIM;
           delete [] IRPIM;
           delete [] RRPIM;
           delete [] OIR;
           delete [] OCI;
           delete [] OCPF;
           delete [] NAEIO;
           delete [] NEA;
           delete [] NEB;
           delete [] TOTALFRTPM;
           delete [] TOTALFRPOM;
           delete [] TOTALFRPHY;
           delete [] SoftTissueEnergeticTotal;
           delete [] ShellEnergeticTotal;
           delete [] OrganicPseudoFaeces;
           delete [] InorganicPseudofaeces;
           delete [] ZooIndividualFluxForTheShell;
           delete [] ShellLength;
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
				cerr << "TChlamysFarreri::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}



void TChlamysFarreri::Update(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName,
									      int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int j, MyBoxNumber = BoxNumber;
    LogMessage("Update", srcName, VariableName, Value, MyBoxNumber);

    if (strstr(VariableName, "C. farreri shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri shell mass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooShellMass[j * NumberOfBoxes + MyBoxNumber] = Value;
                ShellEnergeticTotal[j * NumberOfBoxes + MyBoxNumber] =
                        Value * ShellEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri meat weight %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualWeight[j * NumberOfBoxes + MyBoxNumber] = Value;
                SoftTissueEnergeticTotal[j * NumberOfBoxes + MyBoxNumber] =
                        Value * MeatEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri density %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooClassDensityFlux[j * NumberOfBoxes + MyBoxNumber] =
                        ZooClassDensityFlux[j * NumberOfBoxes + MyBoxNumber] + Value;
                break;
            }
        }
    }
    else
    {
#ifdef __BORLANDC__
        char Caption[129];
        strcpy(Caption, VariableName);
        strcat(Caption, " does not exist in ");
        strcat(Caption, GetEcoDynClassName());
       //Debugger(AnObjectCode);
        MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Update 1",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TChlamysFarreri::Update 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        Value = 0;
    }
}


bool TChlamysFarreri::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
    bool rc = true;
	char MyParameter[65]; // 64 characters (max) for parameter name

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strstr(VariableName, "C. farreri density ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri density %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooClassDensity[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri meat weight ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri meat weight %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + BoxNumber] = Value;
                SoftTissueEnergeticTotal[NumberOfBoxes * j + BoxNumber] =
                        Value * MeatEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri biomass ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri biomass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri shell mass ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri shell mass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooShellMass[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Individual growth ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "Individual growth %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualFlux[NumberOfBoxes * j + BoxNumber] = Value; // Values in g day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "Scope for growth ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "Scope for growth %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                GrowthRate[NumberOfBoxes * j + BoxNumber] = Value; // Values in j day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri CR ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri CR %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ClearanceRate[NumberOfBoxes * j + BoxNumber] = Value; // Values in litres ind-1 d-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri shell length ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. farreri shell length %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ShellLength[NumberOfBoxes * j + BoxNumber] = Value; // Values in cm
                break;
            }
        }
    }
    else if (strcmp(VariableName, "Zoobenthos DIN Release") == 0)
        ZooDINRelease[BoxNumber] = Value;

    else if (strcmp(VariableName, "Zoobenthos Phyto Uptake") == 0)
        ZooPhytoUptake[BoxNumber] = Value;

    else if (strcmp(VariableName, "Zoobenthos POC Uptake") == 0)
        ZooPOCUptake[BoxNumber] = Value;

    else if (strcmp(VariableName, "IRPHY") == 0)
    {
        IRPHY[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRDET") == 0)
    {
        IRDET[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRPIM") == 0)
    {
        IRPIM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRTPM") == 0)
    {
        IRTPM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "FRTPM") == 0)
    {
        FRTPM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "FRPIM") == 0)
    {
        FRPIM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "RRTPM") == 0)
    {
        RRTPM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "RRPIM") == 0)
    {
        RRPIM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "OIR") == 0)
    {
        OIR[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "OCI") == 0)
    {
        OCI[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "OCPF") == 0)
    {
        OCPF[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NAEIO") == 0)
    {
        NAEIO[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NEA") == 0)
    {
        NEA[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "NEB") == 0)
    {
        NEB[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else
        rc = false;

    return rc;
}



void TChlamysFarreri::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	//Debugger(ClearanceRate[20]);
	int i,j;
	char MyParameter[65], // 64 characters (max) for parameter name
					Myjstring[5],
					MyName[65];
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	for (i = 0; i < NumberOfVariables; i++)
    {
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{
			if (strcmp(MyParameter, "Zoobenthos DIN Release") == 0)
				Value = ZooDINRelease[MyBoxNumber];

			else if (strcmp(MyParameter, "Zoobenthos Phyto Uptake") == 0)
				Value = ZooPhytoUptake[MyBoxNumber];

			else if (strcmp(MyParameter, "Zoobenthos POC Uptake") == 0)
				Value = ZooPOCUptake[MyBoxNumber];

            else if (strcmp(MyParameter, "Total biomass") == 0)
            {
         	    double TotalBiomass;
                TotalBiomass = 0.0;
         	    for (j = 0; j < NumberOfClasses; j++)
				{
					TotalBiomass = TotalBiomass + ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
                }
                Value = TotalBiomass;
            }
            else if (strcmp(MyParameter, "Total density") == 0)
            {
         	    double TotalDensity;
                TotalDensity = 0.0;
         	    for (j = 0; j < NumberOfClasses; j++)
				{
					TotalDensity = TotalDensity + ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
                }
                Value = TotalDensity;
            }
            else if (strcmp(MyParameter, "IRPHY") == 0)
            {
                Value = IRPHY[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "IRDET") == 0)
            {
                Value = IRDET[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "IRPIM") == 0)
            {
                Value = IRPIM[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "IRTPM") == 0)
            {
                Value = IRTPM[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "FRTPM") == 0)
            {
                Value = FRTPM[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "FRPIM") == 0)
            {
                Value = FRPIM[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "RRTPM") == 0)
            {
                Value = RRTPM[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "RRPIM") == 0)
            {
                Value = RRPIM[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "OIR") == 0)
            {
                Value = OIR[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "OCI") == 0)
            {
                Value = OCI[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "OCPF") == 0)
            {
                Value = OCPF[MyBoxNumber];
            }
            else if (strcmp(MyParameter, "NAEIO") == 0)
            {
                Value = NAEIO[MyBoxNumber];
            }
            else if (strcmp(MyParameter, "NEA") == 0)
            {
                Value = NEA[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "NEB") == 0)
            {
                Value = NEB[MyBoxNumber]* DAYSTOSECONDS;
            }
            else if (strstr(MyParameter, "C. farreri density ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "C. farreri density %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "C. farreri meat weight ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "C. farreri meat weight %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ZooIndividualWeight[NumberOfBoxes * j + MyBoxNumber];
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "C. farreri biomass ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "C. farreri biomass %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "C. farreri shell mass ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "C. farreri shell mass %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ZooShellMass[NumberOfBoxes * j + MyBoxNumber];
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "Individual growth ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "Individual growth %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ZooIndividualFlux[NumberOfBoxes * j + MyBoxNumber]; // Values in g day-1 ind-1
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "Scope for growth ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "Scope for growth %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = GrowthRate[NumberOfBoxes * j + MyBoxNumber]; // Values in j day-1 ind-1
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "C. farreri CR ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "C. farreri CR %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ClearanceRate[NumberOfBoxes * j + MyBoxNumber]; // Values in litres ind-1 d-1
                        break;
                    }
                }
            }
            else if (strstr(MyParameter, "C. farreri shell length ") != NULL)
            {
                for (j = 0; j < NumberOfClasses; j++)
                {
                    sprintf( MyName, "C. farreri shell length %i", j + 1 );
                    if (strcmp(MyParameter, MyName) == 0) {
                        Value = ShellLength[NumberOfBoxes * j + MyBoxNumber]; // Values in cm
                        break;
                    }
                }
            }
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
            return;
		}
    }
#ifdef __BORLANDC__
    char Caption[129];
    strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
    MessageBox(0,
              Caption,
              "EcoDynamo Alert - Inquiry 3",
              MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TChlamysFarreri::Inquiry 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    Value = 0;
}


double TChlamysFarreri::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        value = DryWeightOfAStandardAnimal;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        value = AllometricCoefficientForFeeding;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        value = AllometricCoefficientForRespiration;
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        value = MeatEnergeticContents;
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        value = ShellEnergeticContents;
    }
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
    {
        value = IntrinsicRateOfMortality;
    }
    else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
    {
        value = SummerIntrinsicRateOfMortality;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration;
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        value = ActivityRespirationCoefficient;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        value = ProportionOfEnergyAllocatedToShell;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        value = ProportionOfEnergyAllocatedToSoftTissue;
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        value = AmmoniaToJoules;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        value = SoftTissueEnergyToTotalEnergy;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        value = NumberOfClasses;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        value = TotalFWToMeatDW;
    }
    else
        value = 0.0;
    return value;
}

bool TChlamysFarreri::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        DryWeightOfAStandardAnimal = value;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        AllometricCoefficientForFeeding = value;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        AllometricCoefficientForRespiration = value;
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        MeatEnergeticContents = value;
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        ShellEnergeticContents = value;
    }
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
    {
        IntrinsicRateOfMortality = value;
    }
    else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
    {
        SummerIntrinsicRateOfMortality = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        MaintenanceRespiration = value;
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        ActivityRespirationCoefficient = value;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        ProportionOfEnergyAllocatedToShell = value;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        ProportionOfEnergyAllocatedToSoftTissue = value;
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        AmmoniaToJoules = value;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        SoftTissueEnergyToTotalEnergy = value;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        NumberOfClasses = value;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        TotalFWToMeatDW = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.16
bool TChlamysFarreri::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("ChlamysFarreri");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Chlamys farreri");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    /*
     * Crassostrea gigas initial values
     */
    char classText[16];
    PReadWrite->WriteCell("C. farreri density 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. farreri shell mass 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. farreri meat weight 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator(true);

    int offset, Index;
    for (int j = 0; j < NumberOfBoxesWithChlamysFarreri; j++)
    {
        Index = ChlamysFarreriRecord[j].LineCoordinate * NumberOfColumns +
                ChlamysFarreriRecord[j].ColumnCoordinate;

        // density by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooClassDensity[offset + Index], 8);
            PReadWrite->WriteSeparator();
        }

        // shell mass by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooShellMass[offset + Index], 8);
            PReadWrite->WriteSeparator();
        }

        // meat weight by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooIndividualWeight[offset + Index], 8);
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TChlamysFarreri::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("ChlamysFarreri");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Chlamys farreri");
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

        if (strcmp(ParametersNameArray[i], "Number of classes") == 0)
        {
            PReadWrite->WriteCell(NumberOfClasses, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Dry weight of a standard animal") == 0)
        {
            PReadWrite->WriteCell(DryWeightOfAStandardAnimal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for feeding") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForFeeding, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for respiration") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
            PReadWrite->WriteCell(MeatEnergeticContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Shell energetic contents") == 0)
        {
            PReadWrite->WriteCell(ShellEnergeticContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Summer Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(SummerIntrinsicRateOfMortality, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Activity respiration coefficient") == 0)
        {
            PReadWrite->WriteCell(ActivityRespirationCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to shell") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToShell, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to soft tissue") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToSoftTissue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "umol NH4 to Joules") == 0)
        {
            PReadWrite->WriteCell(AmmoniaToJoules, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            PReadWrite->WriteCell(SoftTissueEnergyToTotalEnergy, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TChlamysFarreri::Go()

{
//	double MyYear;
	JulianDay = MyPEcoDynClass->GetJulianDay();
//   MyYear = MyPEcoDynClass->GetYear();

	double MyDepth, MyChlorophyllCarbonRatio;

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooDINRelease[i] = 0;
		ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
      OrganicPseudoFaeces[i] = 0.0;
   	InorganicPseudofaeces[i] = 0.0;
	}

	TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
   			  * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
              * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

   if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
   {
   	int i;
   	for (i = 0; i < NumberOfBoxes; i++)
      {
      	if (MyTransportPointer != NULL)
         	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
			  											  i,
			 											  "Box depth",
														  ObjectCode);
         else
         	MyDepth = MyPEcoDynClass->GetBoxDepth(i);


         if (MyDepth > 0.0)
         {
   			seston_mass = 0.0; phyto_mass = 0.0;
      		PHYORG = 0.0; DETORG = 0.0;
      		TPM = 0.0; POM = 0.0; PIM = 0.0;
      		PHYCONTENTSPOM = 0.0;
      		TOTALFRTPM[i] = 0.0; TOTALFRPOM[i] = 0.0; TOTALFRPHY[i] = 0.0;

            IRPHY[i] = 0.0; IRDET[i] = 0.0; IRTPM[i] = 0.0; IRPIM[i] = 0.0;
            FRTPM[i] = 0.0; FRPIM[i] = 0.0;
            RRTPM[i] = 0.0; RRPIM[i] = 0.0;
            OIR[i] = 0.0;  OCI[i] = 0.0; OCPF[i] = 0.0;
            NAEIO[i] = 0.0;
         	NEA[i] = 0.0; NEB[i] = 0.0;

         	MySPMPointer->Inquiry(GetEcoDynClassName(), TPM,
            	                       i,
 												  "TPM",
                  	                 ObjectCode);
         	MySPMPointer->Inquiry(GetEcoDynClassName(), POM,
            		                    i,
               		                 "POM",
                  		              ObjectCode);

         	PIM = TPM - POM;

      		MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,
            		                      i,
               		                   "Phytoplankton biomass",
                  		                ObjectCode);

         	MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,
            		                      i,
               		                   "Chlorophyll to biomass",
                  		                ObjectCode);

            MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllCarbonRatio,
												    i,
												    "Chlorophyll : Carbon",
												    ObjectCode);

         	// Phytoplankton returns its mass as ug chl a l-1
         	// convert to mg l-1 of organic dry matter
         	PHYORG = phyto_mass / CUBIC * ChlorophyllToBiomass; //mg/l
      		DETORG = POM - PHYORG;
      		if (TPM > 0.0000000000000001)
      			OCS = POM / TPM;
      		else
      			OCS = 0.0;
      		if (POM > 0.0000000000000001)
      			PHYCONTENTSPOM = PHYORG / POM;
            if (PHYCONTENTSPOM > 1.0)
                PHYCONTENTSPOM = 1.0;
            /*if (PHYCONTENTSPOM > 1.0)
            {
            	MessageBox(0,"TBivalvesForSango::Go, PHYCONTENTSPOM > 1 ","EcoDynamo Alert",MB_OK | MB_ICONHAND);
               Debugger(i);
            }
            else
         		 PHYCONTENTSPOM = 0.0;  */

      		for (int k = 0; k < NumberOfClasses; k++)
				{
					if (
				 			( ZooClassBiomass[k * NumberOfBoxes + i] > 0.0) &&
				 			( ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
				 			( ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
						)
					{
               	//if(i == 207) MessageBox(0, "Go","Feed",MB_OK | MB_ICONHAND);

            		Feed(i, k);

                  //if(i == 207) MessageBox(0, "Go","Feed",MB_OK | MB_ICONHAND);

            		TOTALFRTPM[i] = TOTALFRTPM[i] + (FRTPM[i] - IRPHY[i]) * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth / CUBIC;
            		TOTALFRPOM[i] = TOTALFRPOM[i] + (FRTPM[i] - FRPIM[i] - IRPHY[i]) *
                               ZooClassDensity[k * NumberOfBoxes + i] / MyDepth / CUBIC;

                  //Subtra? o IRPHY no c?lculo do TOTALFRTPM e TOTALFRPOM porque o objecto phytoplankton de Sango invoca o Update do objecto de
                  //Mat?ria em suspens?o, enviando-lhe o seu fluxo final. Neste fluxo est? integrada a perda do fito por consumo pelos bivalves
                  // que ? o TOTALFRPHY. Deste modo, a subtrac??o acima referida evita que a massa de fito consumida pelos bivalves, seja
                  //subttra?da 2 vezes ao TPM e ao POM


            		TOTALFRPHY[i] = TOTALFRPHY[i] + (FRTPM[i] - FRPIM[i]) * PHYCONTENTSPOM   //mg Org / ind / day
               	   	                     / ChlorophyllToBiomass   //mg Chl / indiv./ day
                                             / MyChlorophyllCarbonRatio       //mg C / indiv./ day
                                             * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth; //mg C / m3 / day

                  //if(i == 207) MessageBox(0, "Go","Respire",MB_OK | MB_ICONHAND);

            		Respire(i, k);

                  //if(i == 207) MessageBox(0, "Go","Excrete",MB_OK | MB_ICONHAND);

                  Excrete(i, k);

                  //if(i == 207) MessageBox(0, "Go","Pseudofaeces",MB_OK | MB_ICONHAND);

            		Pseudofaeces(i, k);

                  //if(i == 207) MessageBox(0, "Go","Assimilate",MB_OK | MB_ICONHAND);

            		Assimilate(i, k);

                  //if(i == 207) MessageBox(0, "Go","Reproduce",MB_OK | MB_ICONHAND);

            		Reproduce(i, k);

                  //if(i == 207) MessageBox(0, "Go","Shell deposition",MB_OK | MB_ICONHAND);

            		ShellDeposition(i, k);

                  //if(i == 207) MessageBox(0, "Go","SoftTissueGrowth",MB_OK | MB_ICONHAND);

            		SoftTissueGrowth(i, k);

                  //if(i == 207) MessageBox(0, "Go","Die",MB_OK | MB_ICONHAND);

            		Die(i, k);

					}
            }
			}

      	if (MySPMPointer != NULL)
      	{
      		MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRTPM[i],
            	                   i,
               	                "TPM",
                  	             ObjectCode);

         	MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRPOM[i],
            	                   i,
               	                "POM",
                  	             ObjectCode);
      	}
      	if (MyPhytoPointer != NULL)
      	{
      		MyPhytoPointer->Update(GetEcoDynClassName(), -TOTALFRPHY[i], //ug Chla /l
            	                      i,
               	                   "Phytoplankton biomass",
                  	                ObjectCode);
      	}
		}
   }
   else
#ifdef __BORLANDC__
   	MessageBox(0, "Bivalves can not run without phytoplankton and suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TChlamysFarreri::Go - Bivalves can not run without phytoplankton and suspended matter" << endl;
#endif  // __BORLANDC__
}


void TChlamysFarreri::Feed(int ABox, int AClass)
{
   int MyBox, MyClass;
   double MyDepth;
   MyBox = ABox;
   MyClass = AClass;
   TEcoDynClass * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyTransportPointer != NULL)
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
			  											  MyBox,
			 											  "Box depth",
														  ObjectCode);
   else
      MyDepth = MyPEcoDynClass->GetBoxDepth(MyBox);

   if(DETORG > 0.0001)
   {
   	IRDET[MyBox] = (0.326 + 0.376 * DETORG) * TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding) /
               HOURSTOSECONDS/**
               DAYSTOHOURS*/; //mg org / d  / ind

      if (IRDET[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC* ATimeStep > DETORG)
      {
        //Debugger(1);
        IRDET[MyBox]= DETORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC / ATimeStep;
      }
      if (IRDET[MyBox] < 0.0) IRDET[MyBox] = 0.0;
   }
   else
      IRDET[MyBox] = 0.0;



   if((POM > exp(-1)) && (PHYORG > 0.0001))
   {
   	IRPHY[MyBox] = (exp(-0.305+(2.82*log(log(POM)+1.0))+(-2.51*(log(POM)+1.0))+(1.995*PHYORG))*
               TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding))
               / HOURSTOSECONDS
               /**DAYSTOHOURS*/; //mg phy-org / d / ind
      if (IRPHY[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > PHYORG)
      {
        //Debugger(2);
        IRPHY[MyBox]= PHYORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC/ ATimeStep;
      }
      if (IRPHY[MyBox] < 0.0) IRPHY[MyBox] = 0.0;
   }
   else
      IRPHY[MyBox] = 0.0;




   /*if (MyBox == 652)
   {
   	Debugger(IRPHY[MyBox]);
      Debugger(POM); Debugger(PHYORG);
      Debugger(exp(-0.305+(2.82*log(log(POM)+1.0))+(-2.51*(log(POM)+1.0))+(1.995*PHYORG)));
      Debugger(TemperatureEffectsOnFeedingRate(MyBox));
      Debugger(AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding));
   	Debugger(DAYSTOHOURS);
   }*/

   if (PIM > 0.0001)
   {
   	IRPIM[MyBox] = (1.034 + (0.0176 * PIM))* TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding)
               / HOURSTOSECONDS
               /**DAYSTOHOURS*/;  //mg / d / ind
   	if (IRPIM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > PIM)
    {
      //Debugger(3);
      IRPIM[MyBox]= PIM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC / ATimeStep;
    }
      if (IRPIM[MyBox] < 0.0) IRPIM[MyBox] = 0.0;
   }
   else
   	IRPIM[MyBox] = 0.0;

   IRTPM[MyBox] = IRDET[MyBox] + IRPHY[MyBox] + IRPIM[MyBox]; //mg / d / ind

   if (IRTPM[MyBox] > 0.0000000000000001)
   	RRTPM[MyBox] = (1.65 + (3.25 * IRTPM[MyBox]));//mg / d / ind
   else
      RRTPM[MyBox] = 0.0;

   FRTPM[MyBox] = IRTPM[MyBox] + RRTPM[MyBox]; //mg / d / ind

   if (TPM > 0.0000000000000001)
   	ClearanceRate[MyClass * NumberOfBoxes + MyBox] = FRTPM[MyBox] / TPM; //L / d / ind
   else
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;

   //if (MyBox == 652) Debugger(ClearanceRate[MyClass * NumberOfBoxes + MyBox]);

   OIR[MyBox] = IRPHY[MyBox] + IRDET[MyBox]; //mg / d

   if (IRTPM[MyBox] > 0.0000000000000001)
      OCI[MyBox] =  (IRDET[MyBox] + IRPHY[MyBox]) / IRTPM[MyBox]; //fraction
   else
      OCI[MyBox] = 0.0;

   if (OCI[MyBox] > 0.0000000000000001)
      OCPF[MyBox] = OCS * OCS / OCI[MyBox];  //fraction
   else
      OCPF[MyBox] = 0.0;
   if (OCPF[MyBox] > 1.0)
   	OCPF[MyBox] = 1.0;

   RRPIM[MyBox] = RRTPM[MyBox] * (1-OCPF[MyBox]); //mg / d / ind

   FRPIM[MyBox] = IRPIM[MyBox] + RRPIM[MyBox]; //mg / d / ind

   if (OCI[MyBox] > 0.0000000000000001)
   	NAEIO[MyBox] = 1.12 - 0.129 * 1.0 / OCI[MyBox]; //fraction
   else
      NAEIO[MyBox] = 0.0;
   if (NAEIO[MyBox] < 0.0)
   NAEIO[MyBox] = 0.0;
      
   NEA[MyBox] = ((23.5 * IRPHY[MyBox]) +(6.1 * IRDET[MyBox])) * NAEIO[MyBox]; //J /d / ind

}



void TChlamysFarreri::Reproduce(int ABox, int AClass)
{
	int MyBox, MyClass;
   MyBox = ABox;
   MyClass = AClass;
	if (JulianDay == 165)
   	ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox]  = ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] -
                                                                   0.07 * SoftTissueEnergeticTotal[MyClass * NumberOfBoxes + MyBox] / DAYSTOSECONDS;
   else
	if (JulianDay == 250)
   	ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] = ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] -
                                                                  0.04 * SoftTissueEnergeticTotal[MyClass * NumberOfBoxes + MyBox] / DAYSTOSECONDS;
}


void TChlamysFarreri::Die(int ABox, int AClass)
{
	int MyBox, MyClass;
   double MyDay;

   MyBox = ABox;
   MyClass = AClass;

   MyDay = MyPEcoDynClass->GetJulianDay();

   if (
   		(MyDay > 212.0) && (MyDay < 274.0)
      )
   		Mortality[MyClass * NumberOfBoxes + MyBox] = ZooClassDensity[MyClass * NumberOfBoxes + MyBox] -
         	                                      ZooClassDensity[MyClass * NumberOfBoxes + MyBox] *
            	                                   exp(-SummerIntrinsicRateOfMortality / DAYSTOHOURS / HOURSTOSECONDS * 1.0);
   else
   		Mortality[MyClass * NumberOfBoxes + MyBox] = ZooClassDensity[MyClass * NumberOfBoxes + MyBox] -
         	                                      ZooClassDensity[MyClass * NumberOfBoxes + MyBox] *
            	                                   exp(-IntrinsicRateOfMortality / DAYSTOHOURS / HOURSTOSECONDS* 1.0);

   if (Mortality[MyClass * NumberOfBoxes + MyBox] < 0.0)
            	Mortality[MyClass * NumberOfBoxes + MyBox] = 0.0;
   ZooClassDensityFlux[MyClass * NumberOfBoxes + MyBox]
					= ZooClassDensityFlux[MyClass * NumberOfBoxes + MyBox] - Mortality[MyClass * NumberOfBoxes + MyBox];

}

double TChlamysFarreri::TemperatureEffectsOnFeedingRate(int ABoxNumber)
{
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);

      return ((234.7/(7.17*pow((2*M_PI),0.5)))* exp(-0.5*pow(((WaterTemperature - 22.2)/7.17),2))) /
             ((234.7/(7.17*pow((2*M_PI),0.5)))* exp(-0.5*pow(((12.0 - 22.2)/7.17),2)));
   }
   else
      return 1.0;
}




double TChlamysFarreri::TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber)
{
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      /*if ((MyBoxNumber == 232) && (MyPEcoDynClass->GetCurrentTime() > 9.0))
   	{
      	Debugger(1000);
   		Debugger(WaterTemperature);
      	Debugger(exp(0.074*WaterTemperature)/exp(0.074*15));
   	}*/
		return exp(0.074*WaterTemperature)/exp(0.074*15);
   }
   else
   	return 1.0;
}


double TChlamysFarreri::OxygenToNitrogenRatio(int ABoxNumber, int AClass)
{
	int MyBoxNumber, MyClass;
   double WtStandNEA;
   MyBoxNumber = ABoxNumber;
   MyClass = AClass;
 	/*if ((10.0 + 0.038873 * NEA[MyBoxNumber]) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.038873 * NEA[MyBoxNumber]) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.038873 * NEA[MyBoxNumber]); */
   WtStandNEA = NEA[MyBoxNumber] * DAYSTOSECONDS /
   				 AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber],AllometricCoefficientForFeeding);
   //Debugger(WtStandNEA);
   if ((10.0 + 0.0363636 * WtStandNEA) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.0363636 * WtStandNEA) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.0363636 * WtStandNEA);
   //Pode variar entre 10 e 50, na propor??o de WtStandNEA }
}


double TChlamysFarreri::ShellLenghtCalculation(double AShellDryWeight)
{
	return (2.589 * pow(AShellDryWeight,0.343));
}

// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 01.08.05
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

// TCrassostreaGigas6 Class
//
// Constructors invoked outside EcoDyn shell...
//

TCrassostreaGigas6::TCrassostreaGigas6(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aZooShellMass[],
        double aZooIndividualWeight[], double aZooClassDensity[],
        int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
        double allometricCoefficientForRespiration, double meatEnergeticContents,
        double shellEnergeticContents, double intrinsicRateOfMortality,
        double respirationSlope, double respirationAtZero,
        double proportionOfEnergyAllocatedToShell,
        double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
        double numberOfClasses, double totalFWToMeatDW,
        double standardClearanceRate, double saturationTPM, double c1o,
        double c1m, double c2, double pFmaxo, double pFmaxm, double Xkpo,
        double Xkpm, double Xkp2o, double Xkp2m, double oxygenToJoules,
        double softTissueEnergyToTotalEnergy)
    :TBivalvesForSango(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
            aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildCrassostreaGigas6();

    DryWeightOfAStandardAnimal = dryWeightOfAStandardAnimal;
    AllometricCoefficientForFeeding = allometricCoefficientForFeeding;
    AllometricCoefficientForRespiration = allometricCoefficientForRespiration;
    MeatEnergeticContents =  meatEnergeticContents;
    ShellEnergeticContents = shellEnergeticContents;
    IntrinsicRateOfMortality = intrinsicRateOfMortality;
    RespirationSlope = respirationSlope;
    RespirationAtZero = respirationAtZero;
    ProportionOfEnergyAllocatedToShell = proportionOfEnergyAllocatedToShell;
    ProportionOfEnergyAllocatedToSoftTissue = proportionOfEnergyAllocatedToSoftTissue;
    AmmoniaToJoules = ammoniaToJoules;
    NumberOfClasses = numberOfClasses;
    TotalFWToMeatDW = totalFWToMeatDW;
    StandardClearanceRate = standardClearanceRate;
    SaturationTPM = saturationTPM;
    C1o =  c1o;
    C1m = c1m;
    C2 = c2;
    PFmaxo = pFmaxo;
    PFmaxm = pFmaxm;
    xkpo = Xkpo;
    xkpm = Xkpm;
    xkp2o = Xkp2o;
    xkp2m = Xkp2m;
    OxygenToJoules = oxygenToJoules;
    SoftTissueEnergyToTotalEnergy = softTissueEnergyToTotalEnergy;

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

    PosBuildCrassostreaGigas6();
}

TCrassostreaGigas6::TCrassostreaGigas6(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TBivalvesForSango(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildCrassostreaGigas6();
}
// ...


TCrassostreaGigas6::TCrassostreaGigas6(TEcoDynClass* APEcoDynClass, char* className)
						   	 :	TBivalvesForSango(APEcoDynClass, className)
{
    BuildCrassostreaGigas6();
}

void TCrassostreaGigas6::BuildCrassostreaGigas6()
{
    PreBuildCrassostreaGigas6();
    ReadParameters();
    BuildArrays();
    ReadVariables();
    PosBuildCrassostreaGigas6();
}

void TCrassostreaGigas6::PreBuildCrassostreaGigas6()
{
	DryWeightOfAStandardAnimal = 1.0;
    ProportionOfEnergyAllocatedToShell = 0.28;
    ProportionOfEnergyAllocatedToSoftTissue = 0.72;

	ParameterCount = 0;
	VariableCount = 0;

    Columns = MyPEcoDynClass->GetNumberOfColumns();
    Lines = MyPEcoDynClass->GetNumberOfLines();
}

void TCrassostreaGigas6::ReadParameters()
{
    double MyValue;
    int X, Y;

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("CrassostreaGigas");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "CrassostreaGigas parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas6::ReadParameters - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
    /*         if (strcmp(MyParameter, "Columns") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, Columns);
                //Debugger(Columns);
             }
             else
             if (strcmp(MyParameter, "Lines") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, Lines);
                //Debugger(Lines);
             }
             else*/
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
                else if (strcmp(MyParameter, "Respiration slope") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RespirationSlope);
                //Debugger(RespirationSlope);
                }
                else if (strcmp(MyParameter, "Respiration at 0") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RespirationAtZero);
                //Debugger(RespirationAtZero);
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
                else if (strcmp(MyParameter, "Standard clearance rate") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, StandardClearanceRate);
                //Debugger(StandardClearanceRate);
                }
                else if (strcmp(MyParameter, "TPM saturation threshold") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SaturationTPM);
                //Debugger(SaturationTPM);
                }
                else if (strcmp(MyParameter, "C1o") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, C1o);
                //Debugger(C1o);
                }
                else if (strcmp(MyParameter, "C1m") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, C1m);
                //Debugger(C1m);
                }
                else if (strcmp(MyParameter, "C2") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, C2);
                //Debugger(C2);
                }
                else if (strcmp(MyParameter, "PFmaxo") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PFmaxo);
                //Debugger(PFmaxo);
                }
                else if (strcmp(MyParameter, "PFmaxm") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PFmaxm);
                //Debugger(PFmaxm);
                }
                else if (strcmp(MyParameter, "xkpo") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, xkpo);
                //Debugger(xkpo);
                }
                else if (strcmp(MyParameter, "xkpm") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, xkpm);
                //Debugger(xkpm);
                }
                else if (strcmp(MyParameter, "xkp2o") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, xkp2o);
                //Debugger(xkp2o);
                }
                else if (strcmp(MyParameter, "xkp2m") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, xkp2m);
                //Debugger(xkp2m);
                }
                else if (strcmp(MyParameter, "Oxygen To Joules") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OxygenToJoules);
                //Debugger(OxygenToJoules);
                }
                else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SoftTissueEnergyToTotalEnergy);
                //Debugger(SoftTissueEnergyToTotalEnergy);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in C. gigas parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TCrassostreaGigas6::ReadParameters - Undefined parameter in C. gigas parameters file" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

void TCrassostreaGigas6::BuildArrays()
{
/*    Generic                 = new double[NumberOfBoxes * NumberOfClasses];
    GenericFlux             = new double[NumberOfBoxes * NumberOfClasses];*/
	ZooClassBiomass         = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensity         = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualWeight     = new double[NumberOfBoxes * NumberOfClasses];
	ZooInitialWeight        = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassBiomassFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensityFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualFlux       = new double[NumberOfBoxes * NumberOfClasses];
    ClassMaxLimits          = new double[NumberOfBoxes * NumberOfClasses];
    ClassMinLimits          = new double[NumberOfBoxes * NumberOfClasses];
	Mortality               = new double[NumberOfBoxes * NumberOfClasses];
    ZooShellMass            = new double[NumberOfBoxes * NumberOfClasses];
    ClearanceRate           = new double[NumberOfBoxes * NumberOfClasses];

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
    NOSE                    = new double[NumberOfBoxes];
    IRORG                   = new double[NumberOfBoxes];
    FRPHY                   = new double[NumberOfBoxes];
    FRPOM                   = new double[NumberOfBoxes];

    SoftTissueEnergeticTotal = new double[NumberOfBoxes * NumberOfClasses];
    ShellEnergeticTotal = new double[NumberOfBoxes * NumberOfClasses];
    ZooIndividualFluxForTheShell = new double[NumberOfBoxes * NumberOfClasses];
    OrganicPseudoFaeces     = new double[NumberOfBoxes];
    InorganicPseudofaeces   = new double[NumberOfBoxes];
    ShellLength = new double[NumberOfBoxes * NumberOfClasses];

    for (int m = 0; m < NumberOfClasses; m++)
    {
        int offset = m * NumberOfBoxes;
		for (int n = 0; n < NumberOfBoxes; n++)
		{
/*			Generic[offset + n]                 = 0;
            GenericFlux[offset + n]             = 0;*/
			ZooClassBiomass[offset + n] 	     = 0;
			ZooClassDensity[offset + n]         = 0;
			ZooIndividualWeight[offset + n]     = 0;
			ZooInitialWeight[offset + n]     = 0;
			ZooClassBiomassFlux[offset + n]     = 0;
			ZooClassDensityFlux[offset + n]     = 0;
			ZooIndividualFlux[offset + n]       = 0;
			Mortality[offset + n] 	     		  = 0;
            ZooShellMass[offset + n]            = 0;
            SoftTissueEnergeticTotal[offset + n] = 0;
            ShellEnergeticTotal[offset + n] = 0;
            ZooIndividualFluxForTheShell[offset + n] = 0;
            ShellLength[offset + n] = 0;
            ClearanceRate[offset + n] = 0;

            ClassMaxLimits[offset + n] = 0;      // AP, 2006.07.20
            ClassMinLimits[offset + n] = 0;
		}
    }


    for (int m = 0; m < NumberOfBoxes; m++)
    {
   	    OrganicPseudoFaeces[m] = 0.0;
   	    InorganicPseudofaeces[m] = 0.0;
        IRPHY[m]= 0;
        IRDET[m]= 0;
        FRTPM[m]= 0;
        IRTPM[m]= 0;
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
        NOSE[m] = 0.0;
        IRORG[m] = 0.0;
        FRPHY[m] = 0.0;
        FRPOM[m] = 0.0;
    }
}

void TCrassostreaGigas6::ReadVariables()
{
    NumberOfBoxesWithCrassostreaGigas = 0;
    CrassostreaGigasRecord = NULL;

    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
      if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Crassostrea gigas") == 0)
              NumberOfBoxesWithCrassostreaGigas++;
    CrassostreaGigasRecord = new BenthicSpeciesRecord[NumberOfBoxesWithCrassostreaGigas];
    int AIndex = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
      if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Crassostrea gigas") == 0)
      {
        strcpy(CrassostreaGigasRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
        CrassostreaGigasRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
        CrassostreaGigasRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
        AIndex++;
      }
    }
    // Read in the variables
    char MyVariable[65];
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("CrassostreaGigas");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "CrassostreaGigas variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas6::ReadVariables - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue;
        int X, Y, RecordNumber, Index, XV, YV, offset;
        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            int k, j;
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
                strcpy(MyVariable, VariableNameArray[i]);
                if (strcmp(MyVariable, "C. gigas shell mass 1") == 0)         //g Shell dry weight / individual
                {
                  if (PReadWrite->FindString("C. gigas shell mass 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithCrassostreaGigas; j++)
                          {
                              Index = CrassostreaGigasRecord[j].LineCoordinate * NumberOfColumns +
                                      CrassostreaGigasRecord[j].ColumnCoordinate;
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
                          RecordNumber = 0;
                          offset = k * NumberOfBoxes;
                          for (j = k * NumberOfBoxesWithCrassostreaGigas; j < k * NumberOfBoxesWithCrassostreaGigas + NumberOfBoxesWithCrassostreaGigas; j++)
                          {
                            Index = CrassostreaGigasRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    CrassostreaGigasRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooShellMass[offset + Index] = MyValue;
                            ShellEnergeticTotal[offset + Index] = MyValue * ShellEnergeticContents;
                            /*if (j == k * NumberOfBoxesWithCrassostreaGigas + NumberOfBoxesWithCrassostreaGigas - 1)
                               Debugger(ZooShellMass[k * NumberOfBoxes + j]);   */
                            RecordNumber++;
                          }
                      }
                  }
                }
                else if (strcmp(MyVariable, "C. gigas meat weight 1") == 0)        //g Soft tissue dry weight / individual
                {
                  if (PReadWrite->FindString("C. gigas meat weight 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithCrassostreaGigas; j++)
                          {
                              Index = CrassostreaGigasRecord[j].LineCoordinate * NumberOfColumns +
                                      CrassostreaGigasRecord[j].ColumnCoordinate;
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
                          RecordNumber = 0;
                          offset = k * NumberOfBoxes;
                          for (j = k * NumberOfBoxesWithCrassostreaGigas; j < k * NumberOfBoxesWithCrassostreaGigas + NumberOfBoxesWithCrassostreaGigas; j++)
                          {
                            Index = CrassostreaGigasRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    CrassostreaGigasRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooIndividualWeight[offset + Index] = MyValue;
                            SoftTissueEnergeticTotal[offset + Index] = MyValue * MeatEnergeticContents;
                            /*if (j == k * NumberOfBoxesWithCrassostreaGigas + NumberOfBoxesWithCrassostreaGigas - 1)
                               Debugger(ZooIndividualWeight[k * NumberOfBoxes + Index]); */
                            RecordNumber++;
                          }
                      }
                  }
                }
                else if (strcmp(MyVariable, "C. gigas density 1") == 0)           //g soft + shell tissue DW / m2
                {
                  if (PReadWrite->FindString("C. gigas density 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithCrassostreaGigas; j++)
                          {
                              Index = CrassostreaGigasRecord[j].LineCoordinate * NumberOfColumns +
                                      CrassostreaGigasRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooClassDensity[offset + Index] = MyValue;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          RecordNumber = 0;
                          offset = k * NumberOfBoxes;
                          for (j = k * NumberOfBoxesWithCrassostreaGigas; j < k * NumberOfBoxesWithCrassostreaGigas + NumberOfBoxesWithCrassostreaGigas; j++)
                          {
                            Index = CrassostreaGigasRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    CrassostreaGigasRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooClassDensity[offset + Index] = MyValue;
                            /*if (j == k * NumberOfBoxesWithCrassostreaGigas + NumberOfBoxesWithCrassostreaGigas - 1)
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
					MessageBox(0, "Variables: Undefined object in C. gigas for Sango", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TCrassostreaGigas6::ReadVariables - Undefined object in C. gigas for Sango" << endl;
#endif  // __BORLANDC__

        CloseDataFile((void*)PReadWrite);
    }
}

void TCrassostreaGigas6::PosBuildCrassostreaGigas6()
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


TCrassostreaGigas6::~TCrassostreaGigas6()
{
    freeMemory();
}

void TCrassostreaGigas6::freeMemory()
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
       if (NumberOfBoxesWithCrassostreaGigas > 0) {
           delete [] CrassostreaGigasRecord;
       }
       if (NumberOfBoxes > 0) {
           delete [] ZooClassBiomass;
           delete [] ZooClassDensity;
           delete [] ZooIndividualWeight;
           delete [] ZooInitialWeight;
           delete [] ZooClassBiomassFlux;
           delete [] ZooClassDensityFlux;
           delete [] ZooIndividualFlux;
           delete [] ClassMaxLimits;
           delete [] ClassMinLimits;
           delete [] Mortality;
           delete [] ClearanceRate;

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
           delete [] FRPOM;
           delete [] FRPHY;
           delete [] ShellEnergeticTotal;
           delete [] SoftTissueEnergeticTotal;
           delete [] OrganicPseudoFaeces;
           delete [] InorganicPseudofaeces;
           delete [] ZooIndividualFluxForTheShell;
           delete [] ShellLength;

           delete [] NOSE;
           delete [] IRORG;
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
				cerr << "TCrassostreaGigas6::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TCrassostreaGigas6::GetParameterValue(char* MyParameter)
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
    else if (strcmp(MyParameter, "Respiration slope") == 0)
    {
        value = RespirationSlope;
    }
    else if (strcmp(MyParameter, "Respiration at 0") == 0)
    {
        value = RespirationAtZero;
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
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        value = NumberOfClasses;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        value = TotalFWToMeatDW;
    }
    else if (strcmp(MyParameter, "Standard clearance rate") == 0)
    {
        value = StandardClearanceRate;
    }
    else if (strcmp(MyParameter, "TPM saturation threshold") == 0)
    {
        value = SaturationTPM;
    }
    else if (strcmp(MyParameter, "C1o") == 0)
    {
        value = C1o;
    }
    else if (strcmp(MyParameter, "C1m") == 0)
    {
        value = C1m;
    }
    else if (strcmp(MyParameter, "C2") == 0)
    {
        value = C2;
    }
    else if (strcmp(MyParameter, "PFmaxo") == 0)
    {
        value = PFmaxo;
    }
    else if (strcmp(MyParameter, "PFmaxm") == 0)
    {
        value = PFmaxm;
    }
    else if (strcmp(MyParameter, "xkpo") == 0)
    {
        value = xkpo;
    }
    else if (strcmp(MyParameter, "xkpm") == 0)
    {
        value = xkpm;
    }
    else if (strcmp(MyParameter, "xkp2o") == 0)
    {
        value = xkp2o;
    }
    else if (strcmp(MyParameter, "xkp2m") == 0)
    {
        value = xkp2m;
    }
    else if (strcmp(MyParameter, "Oxygen To Joules") == 0)
    {
        value = OxygenToJoules;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        value = SoftTissueEnergyToTotalEnergy;
    }
    else
        value = 0.0;
    return value;
}

bool TCrassostreaGigas6::SetParameterValue(char* MyParameter, double value)
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
    else if (strcmp(MyParameter, "Respiration slope") == 0)
    {
        RespirationSlope = value;
    }
    else if (strcmp(MyParameter, "Respiration at 0") == 0)
    {
        RespirationAtZero = value;
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
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        NumberOfClasses = value;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        TotalFWToMeatDW = value;
    }
    else if (strcmp(MyParameter, "Standard clearance rate") == 0)
    {
        StandardClearanceRate = value;
    }
    else if (strcmp(MyParameter, "TPM saturation threshold") == 0)
    {
        SaturationTPM = value;
    }
    else if (strcmp(MyParameter, "C1o") == 0)
    {
        C1o = value;
    }
    else if (strcmp(MyParameter, "C1m") == 0)
    {
        C1m = value;
    }
    else if (strcmp(MyParameter, "C2") == 0)
    {
        C2 = value;
    }
    else if (strcmp(MyParameter, "PFmaxo") == 0)
    {
        PFmaxo = value;
    }
    else if (strcmp(MyParameter, "PFmaxm") == 0)
    {
        PFmaxm = value;
    }
    else if (strcmp(MyParameter, "xkpo") == 0)
    {
        xkpo = value;
    }
    else if (strcmp(MyParameter, "xkpm") == 0)
    {
        xkpm = value;
    }
    else if (strcmp(MyParameter, "xkp2o") == 0)
    {
        xkp2o = value;
    }
    else if (strcmp(MyParameter, "xkp2m") == 0)
    {
        xkp2m = value;
    }
    else if (strcmp(MyParameter, "Oxygen To Joules") == 0)
    {
        OxygenToJoules = value;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        SoftTissueEnergyToTotalEnergy = value;
    }
    else
        rc = false;

    return rc;
}



void TCrassostreaGigas6::Update(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName,
									      int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int j, MyBoxNumber = BoxNumber;
    LogMessage("Update", srcName, VariableName, Value, MyBoxNumber);

    if (strstr(VariableName, "C. gigas shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas shell mass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooShellMass[NumberOfBoxes * j + MyBoxNumber] = Value;
                ShellEnergeticTotal[NumberOfBoxes * j + MyBoxNumber] =
                        Value * ShellEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas meat weight %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + MyBoxNumber] = Value;
                SoftTissueEnergeticTotal[NumberOfBoxes * j + MyBoxNumber] =
                        Value * MeatEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas density %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooClassDensityFlux[NumberOfBoxes * j + MyBoxNumber] += Value;
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
                      "EcoDynamo Alert - Update 2",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TCrassostreaGigas6::Update 2 - " << VariableName << " does not exist in" << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}

bool TCrassostreaGigas6::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
    bool rc = true;
	char MyParameter[65]; // 64 characters (max) for parameter name
	int j;
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Zoobenthos DIN Release") == 0)
    {
        ZooDINRelease[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "Zoobenthos Phyto Uptake") == 0)
    {
        ZooPhytoUptake[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "Zoobenthos POC Uptake") == 0)
    {
        ZooPOCUptake[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRPHYOyster") == 0)
    {
        IRPHY[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRDETOyster") == 0)
    {
        IRDET[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRPIMOyster") == 0)
    {
        IRPIM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "IRTPMOyster") == 0)
    {
        IRTPM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "FRTPMOyster") == 0)
    {
        FRTPM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "FRPIMOyster") == 0)
    {
        FRPIM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "RRTPMOyster") == 0)
    {
        RRTPM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "RRPIMOyster") == 0)
    {
        RRPIM[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "OIROyster") == 0)
    {
        OIR[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "OCIOyster") == 0)
    {
        OCI[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "OCPFOyster") == 0)
    {
        OCPF[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NAEIOOyster") == 0)
    {
        NAEIO[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NEAOyster") == 0)
    {
        NEA[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strcmp(VariableName, "NEBOyster") == 0)
    {
        NEB[BoxNumber] = Value / DAYSTOSECONDS;
    }
    else if (strstr(VariableName, "C. gigas shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas shell mass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooShellMass[NumberOfBoxes * j + BoxNumber] = Value;
                ShellEnergeticTotal[NumberOfBoxes * j + BoxNumber] =
                        Value * ShellEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas meat weight %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + BoxNumber] = Value;
                SoftTissueEnergeticTotal[NumberOfBoxes * j + BoxNumber] =
                        Value * MeatEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas density %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
           		ZooClassDensity[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas biomass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas biomass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Individual growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "Individual growth %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualFlux[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Scope for growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "Scope for growth %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                GrowthRate[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas CR ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas CR %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ClearanceRate[NumberOfBoxes * j + BoxNumber] =
                        Value / DAYSTOHOURS;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas shell length ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas shell length %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ShellLength[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else
        rc = false;

    return rc;
}

void TCrassostreaGigas6::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	int i,j;
	char MyParameter[65], // 64 characters (max) for parameter name
					MyName[65];
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Zoobenthos DIN Release") == 0)
        Value = ZooDINRelease[MyBoxNumber] * DAYSTOSECONDS;

    else if (strcmp(MyParameter, "Zoobenthos Phyto Uptake") == 0)
        Value = ZooPhytoUptake[MyBoxNumber] * DAYSTOSECONDS;

    else if (strcmp(MyParameter, "Zoobenthos POC Uptake") == 0)
        Value = ZooPOCUptake[MyBoxNumber] * DAYSTOSECONDS;

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
    else if (strcmp(MyParameter, "IRPHYOyster") == 0)
    {
        Value = IRPHY[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "IRDETOyster") == 0)
    {
        Value = IRDET[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "IRPIMOyster") == 0)
    {
        Value = IRPIM[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "IRTPMOyster") == 0)
    {
        Value = IRTPM[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "FRTPMOyster") == 0)
    {
        Value = FRTPM[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "FRPIMOyster") == 0)
    {
        Value = FRPIM[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "RRTPMOyster") == 0)
    {
        Value = RRTPM[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "RRPIMOyster") == 0)
    {
        Value = RRPIM[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "OIROyster") == 0)
    {
        Value = OIR[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "OCIOyster") == 0)
    {
        Value = OCI[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "OCPFOyster") == 0)
    {
        Value = OCPF[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "NAEIOOyster") == 0)
    {
        Value = NAEIO[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "NEAOyster") == 0)
    {
        Value = NEA[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "NEBOyster") == 0)
    {
        Value = NEB[MyBoxNumber] * DAYSTOSECONDS;
    }
    else if (strstr(MyParameter, "C. gigas density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas density %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas meat weight %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooIndividualWeight[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas biomass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas biomass %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas shell mass %i", j + 1 );
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
    else if (strstr(MyParameter, "C. gigas CR ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas CR %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ClearanceRate[NumberOfBoxes * j + MyBoxNumber] * DAYSTOHOURS; // Values in litres ind-1 d-1
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas shell length ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas shell length %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ShellLength[NumberOfBoxes * j + MyBoxNumber]; // Values in cm
                break;
            }
        }
    }
    else {
        Value = 0;
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
                      "EcoDynamo Alert - Inquiry 4",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TCrassostreaGigas6::Inquiry 4 - " << MyParameter << " does not exist in" << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.07.16
bool TCrassostreaGigas6::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("CrassostreaGigas");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Crassostrea gigas");
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
    PReadWrite->WriteCell("C. gigas density 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas shell mass 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas meat weight 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator(true);

    int offset, Index;
    for (int j = 0; j < NumberOfBoxesWithCrassostreaGigas; j++)
    {
        Index = CrassostreaGigasRecord[j].LineCoordinate * NumberOfColumns +
                CrassostreaGigasRecord[j].ColumnCoordinate;

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

bool TCrassostreaGigas6::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("CrassostreaGigas");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Crassostrea gigas");
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
        else if (strcmp(ParametersNameArray[i], "Respiration slope") == 0)
        {
            PReadWrite->WriteCell(RespirationSlope, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration at 0") == 0)
        {
            PReadWrite->WriteCell(RespirationAtZero, 8);
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
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Standard clearance rate") == 0)
        {
            PReadWrite->WriteCell(StandardClearanceRate, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TPM saturation threshold") == 0)
        {
            PReadWrite->WriteCell(SaturationTPM, 8);
        }
        else if (strcmp(ParametersNameArray[i], "C1o") == 0)
        {
            PReadWrite->WriteCell(C1o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "C1m") == 0)
        {
            PReadWrite->WriteCell(C1m, 8);
        }
        else if (strcmp(ParametersNameArray[i], "C2") == 0)
        {
            PReadWrite->WriteCell(C2, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PFmaxo") == 0)
        {
            PReadWrite->WriteCell(PFmaxo, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PFmaxm") == 0)
        {
            PReadWrite->WriteCell(PFmaxm, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpo") == 0)
        {
            PReadWrite->WriteCell(xkpo, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpm") == 0)
        {
            PReadWrite->WriteCell(xkpm, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkp2o") == 0)
        {
            PReadWrite->WriteCell(xkp2o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkp2m") == 0)
        {
            PReadWrite->WriteCell(xkp2m, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen To Joules") == 0)
        {
            PReadWrite->WriteCell(OxygenToJoules, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            PReadWrite->WriteCell(SoftTissueEnergyToTotalEnergy, 8);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

// AP, 2006.07.27...
void TCrassostreaGigas6::Reinitialize()
{
  Queue* seedingQueue;
  BUF* pBuf;
  SEED_DB* pSeed;
  SEED_REGION* pSeedRegion;
  BUF* pAux;
  Queue* pBoxes;
  int i, j, n, k, lineIndex, columnIndex, m, boxClassIndex, kSR;
  REGION_BOX* pBox;
  int aditionalBoxes;
  BenthicSpeciesRecord* oldBenthic;
  int RecordNumber, Index;
  TEcoDynClass* MyFisherPointer;

    MyFisherPointer = MyPEcoDynClass->GetFisherPointer();
    if (MyFisherPointer == NULL)
        return;

    seedingQueue = MyFisherPointer->getSeedingQueue();
    if (seedingQueue == NULL)
        return;

    for (i = 0; i < seedingQueue->size(); i++) {
        pBuf = seedingQueue->getElementAt(i);
        pSeed = (SEED_DB*)pBuf->pData;

        for (kSR = 0; kSR < pSeed->seedRegions->size(); kSR++) {
            pAux = pSeed->seedRegions->getElementAt(kSR);
            pSeedRegion = (SEED_REGION*)pAux->pData;
            if (pSeedRegion->bivalve_s.type == BV_TYPE_OYSTER) {

                // signal TFisherMan that seed already done
                pSeed->seeding = true;

                pBoxes = pSeed->regionList->getRegion(pSeedRegion->regName)->getRegionArea();
                if (pBoxes != NULL) {
                    aditionalBoxes = 0;
                    for (n = 0; n < pBoxes->size(); n++) {
                        pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                        k = GetBoxNumber(pBox->column, pBox->line);
                        if (!boxWithOysters(k)) {   // new box with oysters
                            aditionalBoxes++;
                        }
                    }
                    if (aditionalBoxes > 0) {
                        oldBenthic = CrassostreaGigasRecord;
                        CrassostreaGigasRecord = new BenthicSpeciesRecord[
                                NumberOfBoxesWithCrassostreaGigas + aditionalBoxes];
                        for (m = 0; m < NumberOfBoxesWithCrassostreaGigas; m++)
                        {
                            strcpy(CrassostreaGigasRecord[m].SpeciesName, "Crassostrea gigas");
                            CrassostreaGigasRecord[m].LineCoordinate =
                                    oldBenthic[m].LineCoordinate;
                            CrassostreaGigasRecord[m].ColumnCoordinate =
                                    oldBenthic[m].ColumnCoordinate;
                        }
                        delete [] oldBenthic;

                        // m stands for CrassostreaGigasRecord insert index
                        for (n = 0; n < pBoxes->size(); n++) {
                            pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                            k = GetBoxNumber(pBox->column, pBox->line);
                            if (!boxWithOysters(k)) {   // new box with oysters
                                strcpy(CrassostreaGigasRecord[m].SpeciesName, "Crassostrea gigas");
                                CrassostreaGigasRecord[m].LineCoordinate = GetLineIndex(k);
                                CrassostreaGigasRecord[m].ColumnCoordinate = GetColumnIndex(k);
                                m++;
                            }
                        }
                        // new CrassostreaGigasRecord filled
                        NumberOfBoxesWithCrassostreaGigas += aditionalBoxes;
                    }

                    //
                    // seed now...
                    //
                    for (k = 0; k < NumberOfClasses; k++)
                    {
                        for (n = 0; n < pBoxes->size(); n++) {
                            pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                            Index = GetBoxNumber(pBox->column, pBox->line);
                            boxClassIndex = k * NumberOfBoxes + Index;

                            if (ZooClassDensity[boxClassIndex] < 0.000001) {
                                // AP, 2011.01.31 - following SangoMan code...

                                ZooShellMass[boxClassIndex] = pSeedRegion->bivalve_s.weight1;
                                ShellEnergeticTotal[boxClassIndex] =
                                        pSeedRegion->bivalve_s.weight1 * ShellEnergeticContents;
                                ZooIndividualWeight[boxClassIndex] = pSeedRegion->bivalve_s.weight2;
                                SoftTissueEnergeticTotal[boxClassIndex] =
                                        pSeedRegion->bivalve_s.weight2 * MeatEnergeticContents;
                                ZooClassDensityFlux[boxClassIndex] +=
                                        (pSeedRegion->density / MyPEcoDynClass->GetTimeStep());

                                /* AP, 2011.01.31
                                ZooClassBiomass[boxClassIndex] =
                                    ZooClassDensityFlux[boxClassIndex] *
                                    (ZooIndividualWeight[boxClassIndex] +
                                     ZooShellMass[boxClassIndex]);
                                ZooClassDensity[boxClassIndex] =
                                    ZooClassBiomass[boxClassIndex] /
                                    ZooIndividualWeight[boxClassIndex]; */
                            }
                        }
                    }
                }
            }
        }
    }
}

bool TCrassostreaGigas6::boxWithOysters(int boxNumber)
{
  int lineNumber, columnNumber, m;

    lineNumber = GetLineIndex(boxNumber);
    columnNumber = GetColumnIndex(boxNumber);

    for (m = 0; m < NumberOfBoxesWithCrassostreaGigas; m++)
    {
        if (lineNumber == CrassostreaGigasRecord[m].LineCoordinate &&
                columnNumber == CrassostreaGigasRecord[m].ColumnCoordinate)
            return true;
    }
    return false;
}
// ... AP, 2006.07.27

void TCrassostreaGigas6::Go()
{
//    double MyYear;
    JulianDay = MyPEcoDynClass->GetJulianDay();
//    MyYear = MyPEcoDynClass->GetYear();
    int index;
    double MyDepth, MyChlorophyllCarbonRatio;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
      ZooDINRelease[i] = 0;
      ZooPhytoUptake[i] = 0;
      ZooPOCUptake[i] = 0;
      OrganicPseudoFaeces[i] = 0.0;
      InorganicPseudofaeces[i] = 0.0;
    }
    TEcoDynClass  * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                  * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
                  * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
    {
       int i, j;
       for (int m = 0; m < NumberOfBoxesWithCrassostreaGigas; m++)
       {
         i = CrassostreaGigasRecord[m].LineCoordinate;
         j = CrassostreaGigasRecord[m].ColumnCoordinate;
         if (
                (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
            )
         {
              index = i * NumberOfColumns + j;
              if (MyTransportPointer != NULL)
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,index,"Box depth",ObjectCode);
              else MyDepth = MyPEcoDynClass->GetBoxDepth(index);
              if (MyDepth > CriticalWaterDepth)
              {
                  seston_mass = 0.0; phyto_mass = 0.0;
                  PHYORG = 0.0; DETORG = 0.0;
                  TPM = 0.0; POM = 0.0; PIM = 0.0;
                  PHYCONTENTSPOM = 0.0;
                  TOTALFRTPM[index] = 0.0; TOTALFRPOM[index] = 0.0; TOTALFRPHY[index] = 0.0;
                  IRPHY[index] = 0.0; IRDET[index] = 0.0; IRTPM[index] = 0.0; IRPIM[index] = 0.0;
                  FRTPM[index] = 0.0; FRPIM[index] = 0.0;
                  RRTPM[index] = 0.0; RRPIM[index] = 0.0;
                  OIR[index] = 0.0;  OCI[index] = 0.0; OCPF[index] = 0.0;
                  NAEIO[index] = 0.0;
                  NEA[index] = 0.0; NEB[index] = 0.0;
                  MySPMPointer->Inquiry(GetEcoDynClassName(), TPM,index, "TPM",ObjectCode);
                  MySPMPointer->Inquiry(GetEcoDynClassName(), POM,index,"POM",ObjectCode);
                  PIM = TPM - POM;
                  MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,index,"Phytoplankton biomass",ObjectCode);
                  MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,index,"Chlorophyll to biomass",ObjectCode);
                  MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllCarbonRatio,index,"Chlorophyll : Carbon",ObjectCode);
                  // Phytoplankton returns its mass as ug chl a l-1
                  // convert to mg l-1 of organic dry matter
                  PHYORG = phyto_mass / CUBIC * ChlorophyllToBiomass; //mg/l
                  DETORG = POM - PHYORG;
                  if (TPM > 0.0000000000000001) OCS = POM / TPM;
                  else OCS = 0.0;
                  if (POM > 0.0000000000000001) PHYCONTENTSPOM = PHYORG / POM;
                  if (PHYCONTENTSPOM > 1.0) PHYCONTENTSPOM = 1.0;
                  for (int k = 0; k < NumberOfClasses; k++)
                  {
                     if (
                          ( ZooClassBiomass[k * NumberOfBoxes + index] > 0.0) &&
                          ( ZooClassDensity[k * NumberOfBoxes + index] > 0.0) &&
                          ( ZooIndividualWeight[k * NumberOfBoxes + index] > 0.0)
                       )
                     {
                        Feed(index, k);
                        TOTALFRTPM[index] = TOTALFRTPM[index] + (FRTPM[index] - FRPHY[index]) * ZooClassDensity[k * NumberOfBoxes + index] / MyDepth / CUBIC;
                        TOTALFRPOM[index] = TOTALFRPOM[index] + (FRPOM[index] - FRPHY[index]) * ZooClassDensity[k * NumberOfBoxes + index] / MyDepth / CUBIC;
                        //Subtra? o FRPHY no c?lculo do TOTALFRTPM e TOTALFRPOM porque o objecto phytoplankton de Sango invoca o Update do objecto de
                        //Mat?ria em suspens?o, enviando-lhe o seu fluxo final. Neste fluxo est? integrada a perda do fito por consumo pelos bivalves
                        // que ? o TOTALFRPHY. Deste modo, a subtrac??o acima referida evita que a massa de fito consumida pelos bivalves, seja
                        //subttra?da 2 vezes ao TPM e ao POM
                        TOTALFRPHY[index] = TOTALFRPHY[index] + FRPHY[index]    //mg Org / ind / day
                                             / ChlorophyllToBiomass   //mg Chl / indiv./ day
                                             / MyChlorophyllCarbonRatio       //mg C / indiv./ day
                                             * ZooClassDensity[k * NumberOfBoxes + index] / MyDepth; //mg C / m3 / day
                        Respire(index, k);
                        Excrete(index, k);
                        Pseudofaeces(index, k);
                        Assimilate(index, k);
                        Reproduce(index, k);
                        ShellDeposition(index, k);
                        SoftTissueGrowth(index, k);
                        Die(index, k);
                      }
                  }
              }
              if (MySPMPointer != NULL)
              {
                MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRTPM[index],index,"TPM",ObjectCode);
                MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRPOM[index],index,"POM",ObjectCode);
              }
              if (MyPhytoPointer != NULL)
                      MyPhytoPointer->Update(GetEcoDynClassName(), -TOTALFRPHY[index], //ug Chla /l
                                            index,"Phytoplankton biomass",ObjectCode);
          }                                     
       }
    }
    else
#ifdef __BORLANDC__
   	MessageBox(0, "Bivalves can not run without phytoplankton and suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas6::Go - Bivalves can not run without phytoplankton and suspended matter" << endl;
#endif  // __BORLANDC__
}

void TCrassostreaGigas6::Feed(int ABox, int AClass)
{
	int MyBox, MyClass;
   double Delta, Delta1, Delta2, MaximalClearance, FRDET, PFo, PFm /*Organic and inorganic pseudofaeces rate*/;

   MyBox = ABox;
   MyClass = AClass;



   if ((TPM > 0.001) && (POM > 0.0001) && (PHYORG > 0.0001))
   {
   	Delta = SaturationTPM - TPM;
      if (Delta > 0.0) Delta = 0.0;
      MaximalClearance = StandardClearanceRate * exp(0.07 * Delta);
      MaximalClearance = MaximalClearance - TemperatureEffectsOnFeedingRate(MyBox);
      if (MaximalClearance < 0.0) MaximalClearance = 0.0;
   	ClearanceRate[MyClass * NumberOfBoxes + MyBox] = MaximalClearance *
         AllometricCorrection (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding); //{L/h/individuo}

      FRTPM[MyBox] = ClearanceRate[MyClass * NumberOfBoxes + MyBox] * TPM / CUBIC / HOURSTOSECONDS/** DAYSTOHOURS*/;  //g/d/individual
      if (FRTPM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > TPM)
      FRTPM[MyBox]= TPM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;

      FRPOM[MyBox] = FRTPM[MyBox] * POM / TPM;
      if (FRPOM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > POM)
      FRPOM[MyBox]= POM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;

      //Organic pseudofaeces production rate
      Delta1 = C1o / CUBIC - FRPOM[MyBox] / ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox];
      if (Delta1 > 0.0) Delta1 = 0.0;

      Delta2 = C2 / CUBIC - FRPOM[MyBox] / ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox];
      if (Delta2 > 0.0) Delta2 = 0.0;

      PFo = PFmaxo * (1 - exp(xkpo * Delta1))+ (1.0 - PFmaxo) * (1.0 - exp(xkp2o * Delta2));

      /*if (MyBox == 232)
      {
      	Debugger(PFo);
         Debugger(PFmaxo);
         Debugger(xkpo);
         Debugger(Delta1);
         Debugger(xkp2o);
         Debugger(Delta2);
      } */
      //end of organic pseudofaeces rate calculation

      if (PIM / TPM < 1.0)
      {
      	FRPIM[MyBox] = FRTPM[MyBox] * PIM / TPM;
         if (FRPIM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > PIM)
      	FRPIM[MyBox]= PIM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;
      }
      else
         FRPIM[MyBox] = FRTPM[MyBox];

      //Inorganic pseudofaeces production rate
      Delta1 = C1m / CUBIC - FRPIM[MyBox] / ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox];
      if (Delta1 > 0.0) Delta1 = 0.0;

      Delta2 = C2 / CUBIC - FRPIM[MyBox] / ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox];
      if (Delta2 > 0.0) Delta2 = 0.0;

      PFm = PFmaxm * (1 - exp(xkpm * Delta1))+ (1.0 - PFmaxm) * (1.0 - exp(xkp2m * Delta2));

      /*if (MyBox == 232)
      {
      	Debugger(PFm);
         Debugger(PFmaxm);
         Debugger(xkpm);
         Debugger(Delta1);
         Debugger(xkp2m);
         Debugger(Delta2);
      } */



      //end of inorganic pseudofaeces rate calculation

      //IRTPM[MyBox] = FRTPM[MyBox] * (1 - PFo - PFm); //Ingestion rate; g / d

      IRPIM[MyBox] = FRPIM[MyBox] * (1.0 - PFm);

      if(PHYORG / POM < 1.0)
      {
   		FRPHY[MyBox] = FRPOM[MyBox] * PHYORG / POM;
         if (FRPHY[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > PHYORG)
      	FRPHY[MyBox]= PHYORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;
      }
      else
         FRPHY[MyBox] = FRPOM[MyBox];

      IRPHY[MyBox] = (1.0 - PFo) * FRPHY[MyBox];

      FRDET = FRPOM[MyBox] - FRPHY[MyBox];
      if (FRDET < 0.0) FRDET = 0.0;
      IRDET[MyBox] = (1.0 - PFo) * FRDET;

      IRTPM[MyBox] = IRPIM[MyBox] + IRPHY[MyBox] + IRDET[MyBox];
      if (FRTPM[MyBox] < IRTPM[MyBox])
      	FRTPM[MyBox] = IRTPM[MyBox];

      if (IRPIM[MyBox] < 0.0)
      IRPIM[MyBox] = 0.0;
   }
   else
   {
   	FRTPM[MyBox] = 0.0;
      IRTPM[MyBox] = 0.0;
      RRTPM[MyBox] = 0.0;
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;
      FRPIM[MyBox] = 0.0;
      IRPIM[MyBox] = 0.0;
      RRPIM[MyBox] = 0.0;
      FRPHY[MyBox] = 0.0;
      IRPHY[MyBox] = 0.0;
      FRDET = 0.0;
      IRDET[MyBox] = 0.0;
      FRPOM[MyBox] = 0.0;
   }

   RRTPM[MyBox] = FRTPM[MyBox] - IRTPM[MyBox];
   RRPIM[MyBox] = FRPIM[MyBox] - IRPIM[MyBox];

   if (IRPHY[MyBox] + IRDET[MyBox] + IRPIM[MyBox] > 0.0000000000000001)
   	OCI[MyBox] = (IRPHY[MyBox] + IRDET[MyBox]) / (IRPHY[MyBox] + IRDET[MyBox] + IRPIM[MyBox]); //Organic content of ingested matter; fraction
   else
      OCI[MyBox] = 0.0;

   if (OCI[MyBox] > 0.0000000000000001)
   	NAEIO[MyBox] = 0.74 - (0.084 * (1.0 / OCI[MyBox])); //Net absorption efficiency from ingested organics; fraction
   else
      NAEIO[MyBox] = 0.0;
   if (NAEIO[MyBox] < 0.0)
   NAEIO[MyBox] = 0.0;

   NEA[MyBox] = NAEIO[MyBox] * (23500.0 * IRPHY[MyBox] + 6100.0 * IRDET[MyBox]); //Net energy absorption rate; J/d

   FRTPM[MyBox] = FRTPM[MyBox] * CUBIC; //convers?o para mg/d/indivividual

   FRPIM[MyBox] = FRPIM[MyBox] * CUBIC; //convers?o para mg/d/indivividual


   /*if ((MyPEcoDynClass->GetJulianDay() == 3) && (MyPEcoDynClass->GetCurrentTime() > 3.4)
                && (MyBox >= 195))
                Debugger(57);  */

   FRPOM[MyBox] = FRPOM[MyBox] * CUBIC; //convers?o para mg/d/indivividual

   /*if ((MyPEcoDynClass->GetJulianDay() == 3) && (MyPEcoDynClass->GetCurrentTime() > 3.4)
                && (MyBox >= 195))
                Debugger(58);   */

   FRPHY[MyBox] = FRPHY[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRTPM[MyBox] = IRTPM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRPHY[MyBox] = IRPHY[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRDET[MyBox] = IRDET[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   IRPIM[MyBox] = IRPIM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   RRTPM[MyBox] = RRTPM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   RRPIM[MyBox] = RRPIM[MyBox] * CUBIC; //convers?o para mg/d/indivividual
   OIR[MyBox] = IRPHY[MyBox] + IRDET[MyBox]; //mg/d/individual
}

void TCrassostreaGigas6::Respire(int ABox, int AClass)
{
   int MyBox, MyClass;
   double RespirationOfAStandardAnimal;

   MyBox = ABox;
   MyClass = AClass;

   TotalRespiration = /*DAYSTOHOURS * */ TemperatureEffectsOnMaintenanceHeatLosses(MyBox)/ HOURSTOSECONDS * OxygenToJoules *
                     AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox], AllometricCoefficientForRespiration); // J d-1
}

void TCrassostreaGigas6::Reproduce(int ABox, int AClass)
{

}


double TCrassostreaGigas6::TemperatureEffectsOnFeedingRate(int ABoxNumber)
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
      return 0.013 * pow((WaterTemperature-18.954),2);
      //Equation is from Barill? et al.(1997) Aquat. Living Resources, 10: 31-48
   }
   else
      return 1.0;
}


double TCrassostreaGigas6::TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber)
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
      if(RespirationAtZero + RespirationSlope * WaterTemperature >= 0.0)
			return (RespirationAtZero + RespirationSlope * WaterTemperature); //ml O2 / h
      else
      	return 0.0;
   }
   else
   	return 1.0;
}

double TCrassostreaGigas6::OxygenToNitrogenRatio(int ABoxNumber, int AClass)
{
	int MyBoxNumber, MyClass;
   double WtStandNEA;
   MyBoxNumber = ABoxNumber;
   MyClass = AClass;
   WtStandNEA = NEA[MyBoxNumber] * DAYSTOSECONDS /
   				 AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber],AllometricCoefficientForFeeding);

 	if ((10.0 + 0.025 * WtStandNEA) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.025 * WtStandNEA) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.025 * WtStandNEA);
   //Pode variar entre 10 e 50, na propor??o de NEA }
}

double TCrassostreaGigas6::ShellLenghtCalculation(double AShellDryWeight)
{
	double MyShellDryWeight;
   MyShellDryWeight = AShellDryWeight;
   return 28.21* pow(MyShellDryWeight,0.347) / 10; //This is the natural log of shell dry weight, with length in cm
}




// Man object CPP code file
// @ Pedro Duarte
// Rev. A 2001.05.08
/**
 * NPI work
 * 	20 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
    #include <vcl.h>
    #include <stdio.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>

	using namespace std;
#endif  // __BORLANDC__
#include "manobjt.h"
#include "iodll.h"
#include "ecodyn.rh"

// Sango man class
//
// Constructors invoked outside EcoDyn shell...
//

TSangoMan::TSangoMan(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[],
        int nParams, double kelpFirstSeedingDay, double kelpLastSeedingDay,
        double kelpFirstHarvestDay, double kelpLastHarvestDay,
        double seedWeightOfKelp, double biomassInputOfKelp,
        int aCultivationAreaOfKelp[],
        double oysterFirstSeedingDay, double oysterLastSeedingDay,
        double oysterFirstHarvestDayFirstPeriod, double oysterLastHarvestDayFirstPeriod,
        double oysterFirstHarvestDaySecondPeriod, double oysterLastHarvestDaySecondPeriod,
        double shellSeedWeightOfOyster, double meatSeedWeightOfOyster,
        double seedInputOfOyster, double harvestableSizeOfOyster,
        int aCultivationAreaOfOyster[],
        double scallopFirstSeedingDay, double scallopLastSeedingDay,
        double scallopFirstHarvestDayFirstPeriod, double scallopLastHarvestDayFirstPeriod,
        double scallopFirstHarvestDaySecondPeriod, double scallopLastHarvestDaySecondPeriod,
        double shellSeedWeightOfScallop, double meatSeedWeightOfScallop,
        double seedInputOfScallop, double harvestableSizeOfScallop,
        int aCultivationAreaOfScallop[])
    :TMan(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
            aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildSangoMan();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

    NumberOfParameters = nParams;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
        SeedWeightOfKelp[i] = seedWeightOfKelp;
        BiomassInputOfKelp[i] = biomassInputOfKelp;
        CultivationAreaOfKelp[i] = aCultivationAreaOfKelp[i];
        ShellSeedWeightOfOyster[i] = shellSeedWeightOfOyster;
        MeatSeedWeightOfOyster[i] = meatSeedWeightOfOyster;
        SeedInputOfOyster[i] = seedInputOfOyster;
        HarvestableSizeOfOyster[i] = harvestableSizeOfOyster;
        CultivationAreaOfOyster[i] = aCultivationAreaOfOyster[i];
        ShellSeedWeightOfScallop[i] = shellSeedWeightOfScallop;
        MeatSeedWeightOfScallop[i] = meatSeedWeightOfScallop;
        SeedInputOfScallop[i] = seedInputOfScallop;
        HarvestableSizeOfScallop[i] = harvestableSizeOfScallop;
        CultivationAreaOfScallop[i] = aCultivationAreaOfScallop[i];
	}
    KelpFirstSeedingDay = kelpFirstSeedingDay;
    KelpLastSeedingDay = kelpLastSeedingDay;
    KelpFirstHarvestDay = kelpFirstHarvestDay;
    KelpLastHarvestDay =  kelpLastHarvestDay;
    OysterFirstSeedingDay = oysterFirstSeedingDay;
    OysterLastSeedingDay = oysterLastSeedingDay;
    OysterFirstHarvestDayFirstPeriod = oysterFirstHarvestDayFirstPeriod;
    OysterLastHarvestDayFirstPeriod =  oysterLastHarvestDayFirstPeriod;
    OysterFirstHarvestDaySecondPeriod = oysterFirstHarvestDaySecondPeriod;
    OysterLastHarvestDaySecondPeriod = oysterLastHarvestDaySecondPeriod;
    ScallopFirstSeedingDay = scallopFirstSeedingDay;
    ScallopLastSeedingDay =  scallopLastSeedingDay;
    ScallopFirstHarvestDayFirstPeriod = scallopFirstHarvestDayFirstPeriod;
    ScallopLastHarvestDayFirstPeriod = scallopLastHarvestDayFirstPeriod;
    ScallopFirstHarvestDaySecondPeriod = scallopFirstHarvestDaySecondPeriod;
    ScallopLastHarvestDaySecondPeriod =  scallopLastHarvestDaySecondPeriod;
}

TSangoMan::TSangoMan(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
        :TMan(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildSangoMan();
}
// ...

TSangoMan::TSangoMan(TEcoDynClass* APEcoDynClass, char* className)
							: TMan(APEcoDynClass, className)
{
    BuildSangoMan();
}

void TSangoMan::PreBuildSangoMan()
{
 	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		HarvestBiomassOfKelp               = new double[NumberOfBoxes];
		HarvestFluxOfKelp                  = new double[NumberOfBoxes];
		BiomassInputOfKelp                 = new double[NumberOfBoxes];  //g DW / m2
		SeedWeightOfKelp                   = new double[NumberOfBoxes];  //g DW / individual
		HarvestableBiomassOfKelp           = new double[NumberOfBoxes];
        CultivationAreaOfKelp                = new int[NumberOfBoxes];

        HarvestBiomassOfOyster               = new double[NumberOfBoxes];
		HarvestFluxOfOyster                  = new double[NumberOfBoxes];
		SeedInputOfOyster                    = new double[NumberOfBoxes];
		ShellSeedWeightOfOyster              = new double[NumberOfBoxes];
        MeatSeedWeightOfOyster               = new double[NumberOfBoxes];
		HarvestableBiomassOfOyster           = new double[NumberOfBoxes];
        CultivationAreaOfOyster              = new int[NumberOfBoxes];
        HarvestableSizeOfOyster              = new double[NumberOfBoxes];

        HarvestBiomassOfScallop               = new double[NumberOfBoxes];
		HarvestFluxOfScallop                  = new double[NumberOfBoxes];
		SeedInputOfScallop                    = new double[NumberOfBoxes];
		ShellSeedWeightOfScallop              = new double[NumberOfBoxes];
        MeatSeedWeightOfScallop               = new double[NumberOfBoxes];
		HarvestableBiomassOfScallop           = new double[NumberOfBoxes];
        CultivationAreaOfScallop              = new int[NumberOfBoxes];
        HarvestableSizeOfScallop              = new double[NumberOfBoxes];
		// Create generics

/*		Generic     = new double[NumberOfBoxes];
		GenericFlux = new double[NumberOfBoxes];*/

        if (NumberOfBoxes > 0)
        {
            memset(HarvestBiomassOfKelp, 0, sizeof(HarvestBiomassOfKelp));
            memset(HarvestFluxOfKelp, 0, sizeof(HarvestFluxOfKelp));
            memset(BiomassInputOfKelp, 0, sizeof(BiomassInputOfKelp));
            memset(SeedWeightOfKelp, 0, sizeof(SeedWeightOfKelp));
            memset(HarvestableBiomassOfKelp, 0, sizeof(HarvestableBiomassOfKelp));

            memset(HarvestBiomassOfOyster, 0, sizeof(HarvestBiomassOfOyster));
            memset(HarvestFluxOfOyster, 0, sizeof(HarvestFluxOfOyster));
            memset(SeedInputOfOyster, 0, sizeof(SeedInputOfOyster));
            memset(ShellSeedWeightOfOyster, 0, sizeof(ShellSeedWeightOfOyster));
            memset(MeatSeedWeightOfOyster, 0, sizeof(MeatSeedWeightOfOyster));
            memset(HarvestableBiomassOfOyster, 0, sizeof(HarvestableBiomassOfOyster));
            memset(CultivationAreaOfOyster, 0, sizeof(CultivationAreaOfOyster));
            memset(HarvestableSizeOfOyster, 0, sizeof(HarvestableSizeOfOyster));

            memset(HarvestBiomassOfScallop, 0, sizeof(HarvestBiomassOfScallop));
            memset(HarvestFluxOfScallop, 0, sizeof(HarvestFluxOfScallop));
            memset(SeedInputOfScallop, 0, sizeof(SeedInputOfScallop));
            memset(ShellSeedWeightOfScallop, 0, sizeof(ShellSeedWeightOfScallop));
            memset(MeatSeedWeightOfScallop, 0, sizeof(MeatSeedWeightOfScallop));
            memset(HarvestableBiomassOfScallop, 0, sizeof(HarvestableBiomassOfScallop));
            memset(CultivationAreaOfScallop, 0, sizeof(CultivationAreaOfScallop));
            memset(HarvestableSizeOfScallop, 0, sizeof(HarvestableSizeOfScallop));
        }
/*	    for (int i = 0; i < NumberOfBoxes; i++)
        {
		    HarvestBiomassOfKelp[i] = 0.0;
		    HarvestFluxOfKelp[i] = 0.0;
		    BiomassInputOfKelp[i] = 0.0;
		    SeedWeightOfKelp[i] = 0.0;
		    HarvestableBiomassOfKelp[i] = 0.0;

            HarvestBiomassOfOyster[i] = 0.0;
		    HarvestFluxOfOyster[i] = 0.0;
		    SeedInputOfOyster[i] = 0.0;
            ShellSeedWeightOfOyster[i] = 0.0;
            MeatSeedWeightOfOyster[i] = 0.0;
		    HarvestableBiomassOfOyster[i] = 0.0;
            CultivationAreaOfOyster[i] = 0.0;
            HarvestableSizeOfOyster[i] = 0.0;

            HarvestBiomassOfScallop[i] = 0.0;
		    HarvestFluxOfScallop[i] = 0.0;
		    SeedInputOfScallop[i] = 0.0;
            ShellSeedWeightOfScallop[i] = 0.0;
            MeatSeedWeightOfScallop[i] = 0.0;
		    HarvestableBiomassOfScallop[i] = 0.0;
            CultivationAreaOfScallop[i] = 0.0;
            HarvestableSizeOfScallop[i] = 0.0;
        }
 */
        // AP, 2007.07.16
        InitKelpCultivationArea();
        InitOysterCultivationArea();
        InitScallopCultivationArea();
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Man object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TSangoMan::PreBuildSangoMan - Man object array not dimensioned" << endl;
#endif  // __BORLANDC__
}

void TSangoMan::BuildSangoMan()
{
    PreBuildSangoMan();

    int X,Y;

	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Man");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Man variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TSangoMan::BuildSangoMan - Variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Man", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Sango Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TSangoMan::BuildSangoMan - Variables: undefined object Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Man");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Man parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TSangoMan::BuildSangoMan - Parameters file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Man", X, Y))
        {
            double Value;
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                // Kelp seeding and harvest parameters
                if (strcmp(MyParameter, "Kelp first seeding day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KelpFirstSeedingDay);
                //Debugger(KelpFirstSeedingDay);
                }
                else if (strcmp(MyParameter, "Kelp last seeding day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KelpLastSeedingDay);
                //Debugger(KelpLastSeedingDay);
                }
                else if (strcmp(MyParameter, "Kelp first harvest day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KelpFirstHarvestDay);
                //Debugger(KelpFirstHarvestDay);
                }
                else if (strcmp(MyParameter, "Kelp last harvest day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KelpLastHarvestDay);
                //Debugger(KelpLastHarvestDay);
                }
                else if (strcmp(MyParameter, "Seed weight of Kelp gDW / individual") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        SeedWeightOfKelp[k] = Value;
                }
                else if (strcmp(MyParameter, "Biomass Input Of Kelp gDW / m2") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        BiomassInputOfKelp[k] = Value;
                }
    /*  AP, 2007.07.16
               else if (strcmp(MyParameter, "Kelp Cultivation Area") == 0)
                {
                    for (int j = i; j < NumberOfBoxes + i; j++)
                        PReadWrite->ReadNumber(X + 4, j, CultivationAreaOfKelp[j - i]);
                //Debugger(CultivationAreaOfKelp[946]);
                }*/

             //Crassostrea seeding and harvest parameters
                else if (strcmp(MyParameter, "Oyster first seeding day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OysterFirstSeedingDay);
                //Debugger(OysterFirstSeedingDay);
                }
                else if (strcmp(MyParameter, "Oyster last seeding day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OysterLastSeedingDay);
                //Debugger(OysterLastSeedingDay);
                }
                else if (strcmp(MyParameter, "Oyster first harvest day first period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OysterFirstHarvestDayFirstPeriod);
                //Debugger(OysterFirstHarvestDayFirstPeriod);
                }
                else if (strcmp(MyParameter, "Oyster last harvest day first period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OysterLastHarvestDayFirstPeriod);
                //Debugger(OysterLastHarvestDayFirstPeriod);
                }
                else if (strcmp(MyParameter, "Oyster first harvest day second period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OysterFirstHarvestDaySecondPeriod);
                //Debugger(OysterFirstHarvestDaySecondPeriod);
                }
                else if (strcmp(MyParameter, "Oyster last harvest day second period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OysterLastHarvestDaySecondPeriod);
                //Debugger(OysterLastHarvestDaySecondPeriod);
                }
                else if (strcmp(MyParameter, "Shell seed weight of Oyster gDW/individual") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        ShellSeedWeightOfOyster[k] = Value;
                }
                else if (strcmp(MyParameter, "Meat seed weight of Oyster gDW/individual") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        MeatSeedWeightOfOyster[k] = Value;
                }
                else if (strcmp(MyParameter, "Total Oyster seeds ind / m2 / cultivation period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        SeedInputOfOyster[k] = Value;
                }
    /*  AP, 2007.07.16
                else if (strcmp(MyParameter, "Oyster Cultivation Area") == 0)
                {
                    for (int j = i; j < NumberOfBoxes + i; j++)
                        PReadWrite->ReadNumber(X + 5, j, CultivationAreaOfOyster[j - i]);
                //Debugger(CultivationAreaOfOyster[946]);
                }*/
                else if (strcmp(MyParameter, "Oyster harvestable size") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        HarvestableSizeOfOyster[k] = Value;
                }
                //Chlamys seeding and harvest parameters
                else if (strcmp(MyParameter, "Scallop first seeding day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ScallopFirstSeedingDay);
                //Debugger(ScallopFirstSeedingDay);
                }
                else if (strcmp(MyParameter, "Scallop last seeding day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ScallopLastSeedingDay);
                //Debugger(ScallopLastSeedingDay);
                }
                else if (strcmp(MyParameter, "Scallop first harvest day first period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ScallopFirstHarvestDayFirstPeriod);
                //Debugger(ScallopFirstHarvestDayFirstPeriod);
                }
                else if (strcmp(MyParameter, "Scallop last harvest day first period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ScallopLastHarvestDayFirstPeriod);
                //Debugger(ScallopLastHarvestDayFirstPeriod);
                }
                else if (strcmp(MyParameter, "Scallop first harvest day second period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ScallopFirstHarvestDaySecondPeriod);
                //Debugger(ScallopFirstHarvestDaySecondPeriod);
                }
                else if (strcmp(MyParameter, "Scallop last harvest day second period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ScallopLastHarvestDaySecondPeriod);
                //Debugger(ScallopLastHarvestDaySecondPeriod);
                }
                else if (strcmp(MyParameter, "Shell seed weight of Scallop gDW/individual") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        ShellSeedWeightOfScallop[k] = Value;
                }
                else if (strcmp(MyParameter, "Meat seed weight of Scallop gDW/individual") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        MeatSeedWeightOfScallop[k] = Value;
                }
                else if (strcmp(MyParameter, "Total Scallop seeds ind / m2 / cultivation period") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        SeedInputOfScallop[k] = Value;
                }
                else if (strcmp(MyParameter, "Scallop harvestable size") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Value);
                //Debugger(Value);
                    for (int k = 0; k < NumberOfBoxes; k++)
                        HarvestableSizeOfScallop[k] = Value;
                }
    /*  AP, 2007.07.16
                else if (strcmp(MyParameter, "Scallop Cultivation Area") == 0)
                {
                    for (int j = i; j < NumberOfBoxes + i; j++)
                    {
                        PReadWrite->ReadNumber(X + 6, j, CultivationAreaOfScallop[j - i]);
                   //if (CultivationAreaOfScallop[j - i] == 1)
                   //Debugger(j - i);
                    }
                }  */
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Sango Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TSangoMan::BuildSangoMan - Parameters: undefined object Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}


TSangoMan::~TSangoMan()
{
    freeMemory();
}

void TSangoMan::freeMemory()
{
    try {
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }

        if (NumberOfBoxes > 0)
        {
            delete [] HarvestBiomassOfKelp;
            delete [] HarvestFluxOfKelp;
            delete [] BiomassInputOfKelp;
            delete [] SeedWeightOfKelp;
            delete [] HarvestableBiomassOfKelp;
            delete [] CultivationAreaOfKelp;

            delete [] HarvestBiomassOfOyster;
            delete [] HarvestFluxOfOyster;
            delete [] SeedInputOfOyster;
            delete [] ShellSeedWeightOfOyster;
            delete [] MeatSeedWeightOfOyster;
            delete [] HarvestableBiomassOfOyster;
            delete [] CultivationAreaOfOyster;
            delete [] HarvestableSizeOfOyster;

            delete [] HarvestBiomassOfScallop;
            delete [] HarvestFluxOfScallop;
            delete [] SeedInputOfScallop;
            delete [] ShellSeedWeightOfScallop;
            delete [] MeatSeedWeightOfScallop;
            delete [] HarvestableBiomassOfScallop;
            delete [] CultivationAreaOfScallop;
            delete [] HarvestableSizeOfScallop;

    /*		delete [] Generic;
            delete [] GenericFlux;*/
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
        cerr << "TSangoMan::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

double TSangoMan::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Kelp first seeding day") == 0)
    {
        value = KelpFirstSeedingDay;
    }
    else if (strcmp(MyParameter, "Kelp last seeding day") == 0)
    {
        value = KelpLastSeedingDay;
    }
    else if (strcmp(MyParameter, "Kelp first harvest day") == 0)
    {
        value = KelpFirstHarvestDay;
    }
    else if (strcmp(MyParameter, "Kelp last harvest day") == 0)
    {
        value = KelpLastHarvestDay;
    }
    else if (strcmp(MyParameter, "Seed weight of Kelp gDW / individual") == 0)
    {
        value = SeedWeightOfKelp[0];
    }
    else if (strcmp(MyParameter, "Biomass Input Of Kelp gDW / m2") == 0)
    {
        value = BiomassInputOfKelp[0];
    }
/*  AP, 2007.07.16
    else if (strcmp(MyParameter, "Kelp Cultivation Area") == 0)
    {
        value = CultivationAreaOfKelp[0];
    }
    */
    else if (strcmp(MyParameter, "Oyster first seeding day") == 0)
    {
        value = OysterFirstSeedingDay;
    }
    else if (strcmp(MyParameter, "Oyster last seeding day") == 0)
    {
        value = OysterLastSeedingDay;
    }
    else if (strcmp(MyParameter, "Oyster first harvest day first period") == 0)
    {
        value = OysterFirstHarvestDayFirstPeriod;
    }
    else if (strcmp(MyParameter, "Oyster last harvest day first period") == 0)
    {
        value = OysterLastHarvestDayFirstPeriod;
    }
    else if (strcmp(MyParameter, "Oyster first harvest day second period") == 0)
    {
        value = OysterFirstHarvestDaySecondPeriod;
    }
    else if (strcmp(MyParameter, "Oyster last harvest day second period") == 0)
    {
        value = OysterLastHarvestDaySecondPeriod;
    }
    else if (strcmp(MyParameter, "Shell seed weight of Oyster gDW/individual") == 0)
    {
        value = ShellSeedWeightOfOyster[0];
    }
    else if (strcmp(MyParameter, "Meat seed weight of Oyster gDW/individual") == 0)
    {
        value = MeatSeedWeightOfOyster[0];
    }
    else if (strcmp(MyParameter, "Total Oyster seeds ind / m2 / cultivation period") == 0)
    {
        value = SeedInputOfOyster[0];
    }
/*  AP, 2007.07.16
    else if (strcmp(MyParameter, "Oyster Cultivation Area") == 0)
    {
        value = CultivationAreaOfOyster[0];
    }*/
    else if (strcmp(MyParameter, "Oyster harvestable size") == 0)
    {
        value = HarvestableSizeOfOyster[0];
    }
    else if (strcmp(MyParameter, "Scallop first seeding day") == 0)
    {
        value = ScallopFirstSeedingDay;
    }
    else if (strcmp(MyParameter, "Scallop last seeding day") == 0)
    {
        value = ScallopLastSeedingDay;
    }
    else if (strcmp(MyParameter, "Scallop first harvest day first period") == 0)
    {
        value = ScallopFirstHarvestDayFirstPeriod;
    }
    else if (strcmp(MyParameter, "Scallop last harvest day first period") == 0)
    {
        value = ScallopLastHarvestDayFirstPeriod;
    }
    else if (strcmp(MyParameter, "Scallop first harvest day second period") == 0)
    {
        value = ScallopFirstHarvestDaySecondPeriod;
    }
    else if (strcmp(MyParameter, "Scallop last harvest day second period") == 0)
    {
        value = ScallopLastHarvestDaySecondPeriod;
    }
    else if (strcmp(MyParameter, "Shell seed weight of Scallop gDW/individual") == 0)
    {
        value = ShellSeedWeightOfScallop[0];
    }
    else if (strcmp(MyParameter, "Meat seed weight of Scallop gDW/individual") == 0)
    {
        value = MeatSeedWeightOfScallop[0];
    }
    else if (strcmp(MyParameter, "Total Scallop seeds ind / m2 / cultivation period") == 0)
    {
        value = SeedInputOfScallop[0];
    }
    else if (strcmp(MyParameter, "Scallop harvestable size") == 0)
    {
        value = HarvestableSizeOfScallop[0];
    }
/*  AP, 2007.07.16
    else if (strcmp(MyParameter, "Scallop Cultivation Area") == 0)
    {
        value = CultivationAreaOfScallop[0];
    }*/
    else
        value = 0.0;
    return value;
}


bool TSangoMan::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int k;

    if (strcmp(MyParameter, "Kelp first seeding day") == 0)
    {
        KelpFirstSeedingDay = value;
    }
    else if (strcmp(MyParameter, "Kelp last seeding day") == 0)
    {
        KelpLastSeedingDay = value;
    }
    else if (strcmp(MyParameter, "Kelp first harvest day") == 0)
    {
        KelpFirstHarvestDay = value;
    }
    else if (strcmp(MyParameter, "Kelp last harvest day") == 0)
    {
        KelpLastHarvestDay = value;
    }
    else if (strcmp(MyParameter, "Seed weight of Kelp gDW / individual") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            SeedWeightOfKelp[k] = value;
    }
    else if (strcmp(MyParameter, "Biomass Input Of Kelp gDW / m2") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            BiomassInputOfKelp[k] = value;
    }
/*    else if (strcmp(MyParameter, "Kelp Cultivation Area") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            CultivationAreaOfKelp[k] = value;
    }*/
    else if (strcmp(MyParameter, "Oyster first seeding day") == 0)
    {
        OysterFirstSeedingDay = value;
    }
    else if (strcmp(MyParameter, "Oyster last seeding day") == 0)
    {
        OysterLastSeedingDay = value;
    }
    else if (strcmp(MyParameter, "Oyster first harvest day first period") == 0)
    {
        OysterFirstHarvestDayFirstPeriod = value;
    }
    else if (strcmp(MyParameter, "Oyster last harvest day first period") == 0)
    {
        OysterLastHarvestDayFirstPeriod = value;
    }
    else if (strcmp(MyParameter, "Oyster first harvest day second period") == 0)
    {
        OysterFirstHarvestDaySecondPeriod = value;
    }
    else if (strcmp(MyParameter, "Oyster last harvest day second period") == 0)
    {
        OysterLastHarvestDaySecondPeriod = value;
    }
    else if (strcmp(MyParameter, "Shell seed weight of Oyster gDW/individual") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            ShellSeedWeightOfOyster[k] = value;
    }
    else if (strcmp(MyParameter, "Meat seed weight of Oyster gDW/individual") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            MeatSeedWeightOfOyster[k] = value;
    }
    else if (strcmp(MyParameter, "Total Oyster seeds ind / m2 / cultivation period") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            SeedInputOfOyster[k] = value;
    }
/*    else if (strcmp(MyParameter, "Oyster Cultivation Area") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            CultivationAreaOfOyster[k] = value;
    }*/
    else if (strcmp(MyParameter, "Oyster harvestable size") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            HarvestableSizeOfOyster[k] = value;
    }
    else if (strcmp(MyParameter, "Scallop first seeding day") == 0)
    {
        ScallopFirstSeedingDay = value;
    }
    else if (strcmp(MyParameter, "Scallop last seeding day") == 0)
    {
        ScallopLastSeedingDay = value;
    }
    else if (strcmp(MyParameter, "Scallop first harvest day first period") == 0)
    {
        ScallopFirstHarvestDayFirstPeriod = value;
    }
    else if (strcmp(MyParameter, "Scallop last harvest day first period") == 0)
    {
        ScallopLastHarvestDayFirstPeriod = value;
    }
    else if (strcmp(MyParameter, "Scallop first harvest day second period") == 0)
    {
        ScallopFirstHarvestDaySecondPeriod = value;
    }
    else if (strcmp(MyParameter, "Scallop last harvest day second period") == 0)
    {
        ScallopLastHarvestDaySecondPeriod = value;
    }
    else if (strcmp(MyParameter, "Shell seed weight of Scallop gDW/individual") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            ShellSeedWeightOfScallop[k] = value;
    }
    else if (strcmp(MyParameter, "Meat seed weight of Scallop gDW/individual") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            MeatSeedWeightOfScallop[k] = value;
    }
    else if (strcmp(MyParameter, "Total Scallop seeds ind / m2 / cultivation period") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            SeedInputOfScallop[k] = value;
    }
    else if (strcmp(MyParameter, "Scallop harvestable size") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            HarvestableSizeOfScallop[k] = value;
    }
/*    else if (strcmp(MyParameter, "Scallop Cultivation Area") == 0)
    {
        for (int k = 0; k < NumberOfBoxes; k++)
            CultivationAreaOfScallop[k] = value;
    }*/
    else
        rc = false;

    return rc;
}


void TSangoMan::Inquiry(char* srcName, double &Value,
                            int BoxNumber,
                            char* ParameterName,
                            int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Kelp harvest (TonDW)") == 0)
		Value = HarvestBiomassOfKelp[MyBoxNumber] * pow(MyPEcoDynClass->GetBoxLength(MyBoxNumber),2)/ CUBIC / CUBIC;
    else if (strcmp(MyParameter, "Oyster harvest (TonDW)") == 0)
		Value = HarvestBiomassOfOyster[MyBoxNumber] * pow(MyPEcoDynClass->GetBoxLength(MyBoxNumber),2)/ CUBIC / CUBIC;
    else if (strcmp(MyParameter, "Scallop harvest (TonDW)") == 0)
		Value = HarvestBiomassOfScallop[MyBoxNumber] * pow(MyPEcoDynClass->GetBoxLength(MyBoxNumber),2)/ CUBIC / CUBIC;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TSangoMan::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Kelp harvest (TonDW)") == 0)
		HarvestBiomassOfKelp[BoxNumber] = Value * CUBIC * CUBIC
                / pow(MyPEcoDynClass->GetBoxLength(BoxNumber),2);
    else if (strcmp(VariableName, "Oyster harvest (TonDW)") == 0)
		HarvestBiomassOfOyster[BoxNumber] = Value * CUBIC * CUBIC
                / pow(MyPEcoDynClass->GetBoxLength(BoxNumber),2);
    else if (strcmp(VariableName, "Scallop harvest (TonDW)") == 0)
		HarvestBiomassOfScallop[BoxNumber] = Value * CUBIC * CUBIC
                / pow(MyPEcoDynClass->GetBoxLength(BoxNumber),2);
    else
        rc = false;

    return rc;
}

// AP, 2007.07.16
bool TSangoMan::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Man");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Man");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TSangoMan::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Man");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Man");
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

        if (strcmp(ParametersNameArray[i], "Kelp first seeding day") == 0)
        {
            PReadWrite->WriteCell(KelpFirstSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Kelp last seeding day") == 0)
        {
            PReadWrite->WriteCell(KelpLastSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Kelp first harvest day") == 0)
        {
            PReadWrite->WriteCell(KelpFirstHarvestDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Kelp last harvest day") == 0)
        {
            PReadWrite->WriteCell(KelpLastHarvestDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Seed weight of Kelp gDW / individual") == 0)
        {
            PReadWrite->WriteCell(SeedWeightOfKelp[0], 9);
        }
        else if (strcmp(ParametersNameArray[i], "Biomass Input Of Kelp gDW / m2") == 0)
        {
            PReadWrite->WriteCell(BiomassInputOfKelp[0], 9);
        }
/*        else if (strcmp(ParametersNameArray[i], "Kelp Cultivation Area") == 0)
        {
             PReadWrite->WriteCell(CultivationAreaOfKelp[0], 0);
        }*/

     //Crassostrea seeding and harvest parameters
        else if (strcmp(ParametersNameArray[i], "Oyster first seeding day") == 0)
        {
            PReadWrite->WriteCell(OysterFirstSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Oyster last seeding day") == 0)
        {
            PReadWrite->WriteCell(OysterLastSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Oyster first harvest day first period") == 0)
        {
            PReadWrite->WriteCell(OysterFirstHarvestDayFirstPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Oyster last harvest day first period") == 0)
        {
            PReadWrite->WriteCell(OysterLastHarvestDayFirstPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Oyster first harvest day second period") == 0)
        {
            PReadWrite->WriteCell(OysterFirstHarvestDaySecondPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Oyster last harvest day second period") == 0)
        {
            PReadWrite->WriteCell(OysterLastHarvestDaySecondPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Shell seed weight of Oyster gDW/individual") == 0)
        {
            PReadWrite->WriteCell(ShellSeedWeightOfOyster[0], 9);
        }
        else if (strcmp(ParametersNameArray[i], "Meat seed weight of Oyster gDW/individual") == 0)
        {
            PReadWrite->WriteCell(MeatSeedWeightOfOyster[0], 9);
        }
        else if (strcmp(ParametersNameArray[i], "Total Oyster seeds ind / m2 / cultivation period") == 0)
        {
            PReadWrite->WriteCell(SeedInputOfOyster[0], 9);
        }
/*        else if (strcmp(ParametersNameArray[i], "Oyster Cultivation Area") == 0)
        {
            PReadWrite->WriteCell(CultivationAreaOfOyster[0], 0);
        }*/
        else if (strcmp(ParametersNameArray[i], "Oyster harvestable size") == 0)
        {
            PReadWrite->WriteCell(HarvestableSizeOfOyster[0], 9);
        }

        //Chlamys seeding and harvest parameters
        else if (strcmp(ParametersNameArray[i], "Scallop first seeding day") == 0)
        {
            PReadWrite->WriteCell(ScallopFirstSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Scallop last seeding day") == 0)
        {
            PReadWrite->WriteCell(ScallopLastSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Scallop first harvest day first period") == 0)
        {
            PReadWrite->WriteCell(ScallopFirstHarvestDayFirstPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Scallop last harvest day first period") == 0)
        {
            PReadWrite->WriteCell(ScallopLastHarvestDayFirstPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Scallop first harvest day second period") == 0)
        {
            PReadWrite->WriteCell(ScallopFirstHarvestDaySecondPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Scallop last harvest day second period") == 0)
        {
            PReadWrite->WriteCell(ScallopLastHarvestDaySecondPeriod, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Shell seed weight of Scallop gDW/individual") == 0)
        {
            PReadWrite->WriteCell(ShellSeedWeightOfScallop[0], 9);
        }
        else if (strcmp(ParametersNameArray[i], "Meat seed weight of Scallop gDW/individual") == 0)
        {
            PReadWrite->WriteCell(MeatSeedWeightOfScallop[0], 9);
        }
        else if (strcmp(ParametersNameArray[i], "Total Scallop seeds ind / m2 / cultivation period") == 0)
        {
            PReadWrite->WriteCell(SeedInputOfScallop[0], 9);
        }
        else if (strcmp(ParametersNameArray[i], "Scallop harvestable size") == 0)
        {
            PReadWrite->WriteCell(HarvestableSizeOfScallop[0], 9);
        }
/*        else if (strcmp(ParametersNameArray[i], "Scallop Cultivation Area") == 0)
        {
            PReadWrite->WriteCell(CultivationAreaOfScallop[0], 0);
        }*/
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TSangoMan::Integrate()
{
	// Man
//	Generic = HarvestBiomassOfKelp; GenericFlux = HarvestFluxOfKelp;
	Integration(HarvestBiomassOfKelp, HarvestFluxOfKelp);
//	HarvestBiomassOfKelp = Generic; HarvestFluxOfKelp = GenericFlux;

//   Generic = HarvestBiomassOfOyster; GenericFlux = HarvestFluxOfOyster;
	Integration(HarvestBiomassOfOyster, HarvestFluxOfOyster);
//	HarvestBiomassOfOyster = Generic; HarvestFluxOfOyster = GenericFlux;


//   Generic = HarvestBiomassOfScallop; GenericFlux = HarvestFluxOfScallop;
	Integration(HarvestBiomassOfScallop, HarvestFluxOfScallop);
//	HarvestBiomassOfScallop = Generic; HarvestFluxOfScallop = GenericFlux;

	for (int i = 0; i < NumberOfBoxes; i++)
   {
		if (HarvestBiomassOfKelp[i]  < 0.0)
			HarvestBiomassOfKelp[i] = 0.0;

      if (HarvestBiomassOfOyster[i]  < 0.0)
			HarvestBiomassOfOyster[i] = 0.0;

      if (HarvestBiomassOfScallop[i]  < 0.0)
			HarvestBiomassOfScallop [i] = 0.0;

	}
}

void TSangoMan::Go()
{
	// Active methods
	/*MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrentTime();
	if (MyTime != TimeBefore)*/
		JulianDay = MyPEcoDynClass->GetJulianDay();
	//TimeBefore = MyTime;


   //Kelp go


   //Kelp seed
   if (KelpFirstSeedingDay <= KelpLastSeedingDay)
   {
		if ((JulianDay >= KelpFirstSeedingDay) && (JulianDay < KelpLastSeedingDay + 1))
      {
			KelpSeed();
      	//Debugger(JulianDay);
      }
   }
   else
   {
   	if ((JulianDay >= KelpFirstSeedingDay) || (JulianDay < KelpLastSeedingDay + 1))
   		KelpSeed();
   }


   //Kelp harvest
   if (KelpFirstHarvestDay < KelpLastHarvestDay)
   {
		if ((JulianDay >= KelpFirstHarvestDay) && (JulianDay < KelpLastHarvestDay + 1))
   		KelpHarvest();
	}
   else
   {
   	if ((JulianDay >= KelpFirstHarvestDay) || (JulianDay < KelpLastHarvestDay + 1))
   		KelpHarvest();
   }






   //Oyster go

   //Oyster seed
   if (OysterFirstSeedingDay <= OysterLastSeedingDay)
   {
		if ((JulianDay >= OysterFirstSeedingDay) && (JulianDay < OysterLastSeedingDay + 1))
      {
			OysterSeed();
      	//Debugger(JulianDay);
      }
   }
   else
   {
   	if ((JulianDay >= OysterFirstSeedingDay) || (JulianDay < OysterLastSeedingDay + 1))
   		OysterSeed();
   }

   //Oyster first harvest
   if (OysterFirstHarvestDayFirstPeriod < OysterLastHarvestDayFirstPeriod)
   {
		if ((JulianDay >= OysterFirstHarvestDayFirstPeriod) && (JulianDay < OysterLastHarvestDayFirstPeriod + 1))
   		OysterHarvest();
	}
   else
   {
   	if ((JulianDay >= OysterFirstHarvestDayFirstPeriod) || (JulianDay < OysterLastHarvestDayFirstPeriod + 1))
   		OysterHarvest();
   }

   //Oyster second harvest
   if (OysterFirstHarvestDaySecondPeriod < OysterLastHarvestDaySecondPeriod)
   {
		if ((JulianDay >= OysterFirstHarvestDaySecondPeriod) && (JulianDay < OysterLastHarvestDaySecondPeriod + 1))
   		OysterHarvest();
	}
   else
   {
   	if ((JulianDay >= OysterFirstHarvestDaySecondPeriod) || (JulianDay < OysterLastHarvestDaySecondPeriod + 1))
   		OysterHarvest();
   }


   //Chlamys go

   //Chlamys seed
   if (ScallopFirstSeedingDay <= ScallopLastSeedingDay)
   {
		if ((JulianDay >= ScallopFirstSeedingDay) && (JulianDay < ScallopLastSeedingDay + 1))
      {
			ScallopSeed();
      	//Debugger(JulianDay);
      }
   }
   else
   {
   	if ((JulianDay >= ScallopFirstSeedingDay) || (JulianDay < ScallopLastSeedingDay + 1))
   		ScallopSeed();
   }

   //Chlamys first harvest
   if (ScallopFirstHarvestDayFirstPeriod < ScallopLastHarvestDayFirstPeriod)
   {
		if ((JulianDay >= ScallopFirstHarvestDayFirstPeriod) && (JulianDay < ScallopLastHarvestDayFirstPeriod + 1))
   		ScallopHarvest();
	}
   else
   {
   	if ((JulianDay >= ScallopFirstHarvestDayFirstPeriod) || (JulianDay < ScallopLastHarvestDayFirstPeriod + 1))
   		ScallopHarvest();
   }

   //Chlamys second harvest
   if (ScallopFirstHarvestDaySecondPeriod < ScallopLastHarvestDaySecondPeriod)
   {
		if ((JulianDay >= ScallopFirstHarvestDaySecondPeriod) && (JulianDay < ScallopLastHarvestDaySecondPeriod + 1))
   		ScallopHarvest();
	}
   else
   {
   	if ((JulianDay >= ScallopFirstHarvestDaySecondPeriod) || (JulianDay < ScallopLastHarvestDaySecondPeriod + 1))
   		ScallopHarvest();
   }
}




void TSangoMan::Seed()
{

}

void TSangoMan::KelpSeed()
{
	const int LastDayOfTheYear = 365;
	double ExistingBiomass, ExistingDensity, CurrentKelpWeight,
          DaylyBiomassInput, SeedInputNumber, MyFirstSeedingDay, MyLastSeedingDay, NumberOfSeedDays;
	TEcoDynClass* MyMacrophytePointer = MyPEcoDynClass->GetMacrophytePointer();
   if (MyMacrophytePointer != NULL)
   {
      MyFirstSeedingDay = KelpFirstSeedingDay;
      MyLastSeedingDay = KelpLastSeedingDay;
      if (MyFirstSeedingDay > MyLastSeedingDay)
      	MyLastSeedingDay = KelpLastSeedingDay + LastDayOfTheYear;
      else
         MyLastSeedingDay = KelpLastSeedingDay;
      NumberOfSeedDays = MyLastSeedingDay - (MyFirstSeedingDay - 1); //Both first and last days included in seeding period

      for (int i = 0; i < NumberOfBoxes; i++)
   	  {
      	if (CultivationAreaOfKelp[i] == 1)
        {
            MyMacrophytePointer->Inquiry(GetEcoDynClassName(), ExistingBiomass,    //g DW / m2
                                             i,
                                             "L. japonica biomass density",
                                             ObjectCode);
            MyMacrophytePointer->Inquiry(GetEcoDynClassName(), ExistingDensity,    //indivuals / m2
                                             i,
                                             "L. japonica density",
                                             ObjectCode);
            MyMacrophytePointer->Inquiry(GetEcoDynClassName(), CurrentKelpWeight,    //g DW / individual
                                             i,
                                             "L. japonica individual weight",
                                             ObjectCode);

            DaylyBiomassInput = BiomassInputOfKelp[i] / NumberOfSeedDays;     //g DW / day / m2
            SeedInputNumber = DaylyBiomassInput / SeedWeightOfKelp[i] / DAYSTOSECONDS;        //individuals / s / m2

            MyMacrophytePointer->Update(GetEcoDynClassName(), DaylyBiomassInput / DAYSTOSECONDS,    //g DW / m2 / s
                                         i,
                                         "L. japonica biomass input/output",
                                         ObjectCode);


            CurrentKelpWeight = (
                                   CurrentKelpWeight * ExistingDensity +
                                   SeedWeightOfKelp[i] * SeedInputNumber * MyPEcoDynClass->GetTimeStep()
                                 )  /
                                 (ExistingDensity + SeedInputNumber * MyPEcoDynClass->GetTimeStep());
                                 // g / individual, weighted by the existing and new individuals, that enter at each time step

            MyMacrophytePointer->Update(GetEcoDynClassName(), CurrentKelpWeight,    //g DW / invididual
                                         i,
                                         "L. japonica individual weight",
                                         ObjectCode);

         }
      }
   }
}


void TSangoMan::KelpHarvest()
{
   const int LastDayOfTheYear = 365;
   double ExistingBiomass, BiomassOutputPerSecond, MyFirstHarvestDay, MyLastHarvestDay, NumberOfHarvestDays;
   TEcoDynClass* MyMacrophytePointer = MyPEcoDynClass->GetMacrophytePointer();
   if (MyMacrophytePointer != NULL)
   {
   	  MyFirstHarvestDay = KelpFirstHarvestDay;
      MyLastHarvestDay = KelpLastHarvestDay;
   	  if (MyFirstHarvestDay > MyLastHarvestDay)
      	MyLastHarvestDay = KelpLastHarvestDay + LastDayOfTheYear;
      else
         MyLastHarvestDay = KelpLastHarvestDay;
      NumberOfHarvestDays = MyLastHarvestDay - (MyFirstHarvestDay - 1); //Both first and last days included in harvest period
      for (int i = 0; i < NumberOfBoxes; i++)
   	  {
      	if (CultivationAreaOfKelp[i] == 1)
         {
         	MyMacrophytePointer->Inquiry(GetEcoDynClassName(), ExistingBiomass,
      										 i,
                                             "L. japonica biomass density",
               		                         ObjectCode);
            if (JulianDay < KelpLastHarvestDay)
         		BiomassOutputPerSecond = ExistingBiomass / NumberOfHarvestDays / DAYSTOSECONDS;
            else
               BiomassOutputPerSecond = ExistingBiomass / MyPEcoDynClass->GetTimeStep();
            HarvestFluxOfKelp[i] = BiomassOutputPerSecond;
            MyMacrophytePointer->Update(GetEcoDynClassName(), -BiomassOutputPerSecond,    //g DW / m2 / timestep
                                         i,
                                         "L. japonica biomass input/output",
                                         ObjectCode);
         }
      }
   }
}



void TSangoMan::ScallopSeed()    //Scallop seed takes place during the seeding period in the cultivation areas where
                                 //scallops have already been harvested. At each location seed takes place in one moment
{
   double ExistingDensity, SeedInputNumber;

   TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();
   if (MyZoobenthosPointer != NULL)
   {
      for (int i = 0; i < NumberOfBoxes; i++)
      {
         if (CultivationAreaOfScallop[i] == 1)
         {
            MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), ExistingDensity,    //ind / m2
                                                 i,
                                                 "C. farreri density 1",
                                                 ObjectCode);

            if (ExistingDensity < 0.000001)  //If ExistingBiomass = 0, There was no harvest yet or there was already seeding
            {
               SeedInputNumber = SeedInputOfScallop[i] / MyPEcoDynClass->GetTimeStep(); // Os juvenis são colocados instantaneamente na área de cultivo

               MyZoobenthosPointer->Update(GetEcoDynClassName(), ShellSeedWeightOfScallop[i],    //g DW
               	                          i,
                  	                       "C. farreri shell mass 1",
                     	                    ObjectCode);
               MyZoobenthosPointer->Update(GetEcoDynClassName(), MeatSeedWeightOfScallop[i] ,    //g DW
               	                          i,
                  	                       "C. farreri meat weight 1",
                     	                    ObjectCode);
               MyZoobenthosPointer->Update(GetEcoDynClassName(), SeedInputNumber,    //ind / timestep
               	                          i,
                  	                       "C. farreri density 1",
                     	                    ObjectCode);
               /*MyZoobenthosPointer->Update(GetEcoDynClassName(), SeedInputNumber * (ShellSeedWeightOfScallop[i] + MeatSeedWeightOfScallop[i]),
               	                          i,
                  	                       "C. farreri biomass 1",
                     	                    ObjectCode);           */


            }
         }
      }
   }
}


void TSangoMan::OysterSeed()
{
	double ExistingDensity, SeedInputNumber;

	TEcoDynClass* MyZoobenthosPointer2 = MyPEcoDynClass->GetZoobenthosPointer2();
   if (MyZoobenthosPointer2 != NULL)
   {
		for (int i = 0; i < NumberOfBoxes; i++)
   	{
      	if (CultivationAreaOfOyster[i] == 1)
         {
            MyZoobenthosPointer2->Inquiry(GetEcoDynClassName(), ExistingDensity,    //ind / m2
                                             i,
                                             "C. gigas density 1",
                                             ObjectCode);

            if (ExistingDensity < 0.000001)  //If ExistingBiomass = 0, There was no harvest yet or there was already seeding
            {
               SeedInputNumber = SeedInputOfOyster[i] / MyPEcoDynClass->GetTimeStep(); // Os juvenis são colocados instantaneamente na área de cultivo

            	MyZoobenthosPointer2->Update(GetEcoDynClassName(), ShellSeedWeightOfOyster[i],    //g DW
               	                          i,
                  	                       "C. gigas shell mass 1",
                     	                    ObjectCode);
               MyZoobenthosPointer2->Update(GetEcoDynClassName(), MeatSeedWeightOfOyster[i] ,    //g DW
               	                          i,
                  	                       "C. gigas meat weight 1",
                     	                    ObjectCode);
               MyZoobenthosPointer2->Update(GetEcoDynClassName(), SeedInputNumber,    //ind / timestep
               	                          i,
                  	                       "C. gigas density 1",
                     	                    ObjectCode);
               /*MyZoobenthosPointer2->Update(GetEcoDynClassName(), SeedInputNumber * (ShellSeedWeightOfOyster[i] + MeatSeedWeightOfOyster[i]),
               	                          i,
                  	                       "C. gigas biomass 1",
                     	                    ObjectCode); */


            }
         }
		}
   }
}




void TSangoMan::OysterHarvest()
{
	double ExistingDensity, ShellLength, NumberOfHarvestedOysters, OysterShellWeight, OysterMeatWeight;
	TEcoDynClass* MyZoobenthosPointer2 = MyPEcoDynClass->GetZoobenthosPointer2();
   if (MyZoobenthosPointer2 != NULL)
   {

		for (int i = 0; i < NumberOfBoxes; i++)
   	{
      	if (CultivationAreaOfOyster[i] == 1)
         {
         	MyZoobenthosPointer2->Inquiry(GetEcoDynClassName(), ExistingDensity,
      												   i,
            		                           "C. gigas density 1",
               		                        ObjectCode);

            MyZoobenthosPointer2->Inquiry(GetEcoDynClassName(), ShellLength,
      												   i,
            		                           "C. gigas shell length 1",
               		                        ObjectCode);
            if ((ExistingDensity > 0.000001) && (ShellLength >= HarvestableSizeOfOyster[i]))
            {
            	MyZoobenthosPointer2->Inquiry(GetEcoDynClassName(), OysterShellWeight,    //g DW
               	                               i,
                  	                            "C. gigas shell mass 1",
                     	                         ObjectCode);
               MyZoobenthosPointer2->Inquiry(GetEcoDynClassName(), OysterMeatWeight,    //g DW
               	                               i,
                  	                            "C. gigas meat weight 1",
                     	                         ObjectCode);
            	NumberOfHarvestedOysters = ExistingDensity / MyPEcoDynClass->GetTimeStep();
            	MyZoobenthosPointer2->Update(GetEcoDynClassName(), -NumberOfHarvestedOysters,    //g DW / m2 / timestep  As ostras são todas apanhadas de uma só vez
               	                           i,
                  	                        "C. gigas density 1",
                     	                     ObjectCode);
               HarvestFluxOfOyster[i] = NumberOfHarvestedOysters * (OysterShellWeight + OysterMeatWeight);
            }
         }
       }
   }
}

void TSangoMan::ScallopHarvest()
{
	double ExistingDensity, ShellLength, NumberOfHarvestedScallops, ScallopShellWeight, ScallopMeatWeight;
	TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();
   if (MyZoobenthosPointer != NULL)
   {
		for (int i = 0; i < NumberOfBoxes; i++)
   	{
      	if (CultivationAreaOfScallop[i] == 1)
         {
         	MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), ExistingDensity,
      												   i,
            		                           "C. farreri density 1",
               		                        ObjectCode);

            MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), ShellLength,
      												   i,
            		                           "C. farreri shell length 1",
               		                        ObjectCode);
            if ((ExistingDensity > 0.000001) && (ShellLength >= HarvestableSizeOfScallop[i]))
            {
            	MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), ScallopShellWeight,    //g DW
               	                              i,
                  	                          "C. farreri shell mass 1",
                     	                       ObjectCode);
               MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), ScallopMeatWeight,    //g DW
               	                              i,
                  	                          "C. farreri meat weight 1",
                     	                       ObjectCode);
            	NumberOfHarvestedScallops = ExistingDensity / MyPEcoDynClass->GetTimeStep();
            	MyZoobenthosPointer->Update(GetEcoDynClassName(), -NumberOfHarvestedScallops,    //ind. / m2 / timestep      Os scallopa são apanhados todos de uma vez
               	                          i,
                  	                       "C. farreri density 1",
                     	                    ObjectCode);
               HarvestFluxOfScallop[i] = NumberOfHarvestedScallops * (ScallopShellWeight + ScallopMeatWeight);
            }
         }
		}
   }
}
//=================================================================
void TSangoMan::InitKelpCultivationArea()
{
    int boxNumber = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Laminaria japonica") == 0)
        {
            boxNumber = MyPEcoDynClass->GetBoxIndex(MyBenthicSpeciesRecord[i].ColumnCoordinate,
                    MyBenthicSpeciesRecord[i].LineCoordinate);
            CultivationAreaOfKelp[boxNumber] = 1;
        }
    }
}
//=================================================================
void TSangoMan::InitOysterCultivationArea()
{
    int boxNumber = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Crassostrea gigas") == 0)
        {
            boxNumber = MyPEcoDynClass->GetBoxIndex(MyBenthicSpeciesRecord[i].ColumnCoordinate,
                    MyBenthicSpeciesRecord[i].LineCoordinate);
            CultivationAreaOfOyster[boxNumber] = 1;
        }
    }
}
//=================================================================
void TSangoMan::InitScallopCultivationArea()
{
    int boxNumber = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Chlamys farreri") == 0)
        {
            boxNumber = MyPEcoDynClass->GetBoxIndex(MyBenthicSpeciesRecord[i].ColumnCoordinate,
                    MyBenthicSpeciesRecord[i].LineCoordinate);
            CultivationAreaOfScallop[boxNumber] = 1;
        }
    }
}
//=================================================================



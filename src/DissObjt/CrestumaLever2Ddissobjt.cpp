// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 01.04.27

/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <vcl.h>
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

// TCrestumaLeverVert2DDissobjt Class
//
// Constructors invoked outside EcoDyn shell...
//

TCrestumaLeverVert2DDissobjt::TCrestumaLeverVert2DDissobjt(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[],
        double riverAmmonia, double aAmmonia[], double aNH4Load[],
        double riverNitrite, double aNitrite[], double aNO2Load[],
        double riverNitrate, double aNitrate[], double aNO3Load[],
        double riverPhosphate, double aPhosphate[], double aPO4Load[],
        double riverSilica, double aSilica[], double aSiO2Load[],
        int nParams, double simulationInterval,
        double ammoniaInitialRiverConcentration, double ammoniaFinalRiverConcentration,
        double nitriteInitialRiverConcentration, double nitriteFinalRiverConcentration,
        double nitrateInitialRiverConcentration, double nitrateFinalRiverConcentration,
        double phosphateInitialRiverConcentration, double phosphateFinalRiverConcentration,
        double silicaInitialRiverConcentration, double silicaFinalRiverConcentration)
    :TNutrients(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildCrestumaLeverVert2DNutrients();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int j = 0; j < NumberOfBoxes; j++)
	{
		Ammonia[j] = aAmmonia[j];
		NH4Load[j] = aNH4Load[j];
		Nitrite[j] = aNitrite[j];
		NO2Load[j] = aNO2Load[j];
		Nitrate[j] = aNitrate[j];
		NO3Load[j] = aNO3Load[j];
		Phosphate[j] = aPhosphate[j];
		PO4Load[j] = aPO4Load[j];
		Silica[j] = aSilica[j];
		SiO2Load[j] = aSiO2Load[j];
	}
	RiverAmmonia = riverAmmonia;
	RiverNitrite = riverNitrite;
	RiverNitrate = riverNitrate;
	RiverPhosphate = riverPhosphate;
	RiverSilica = riverSilica;

	NumberOfParameters = nParams;
    SimulationInterval = simulationInterval;
	AmmoniaInitialRiverConcentration = ammoniaInitialRiverConcentration * CUBIC;  // CUBIC factor in river and ocean
    AmmoniaFinalRiverConcentration = ammoniaFinalRiverConcentration * CUBIC;  // CUBIC factor in river and ocean
	NitriteInitialRiverConcentration = nitriteInitialRiverConcentration * CUBIC;
	NitriteFinalRiverConcentration = nitriteFinalRiverConcentration * CUBIC;
	NitrateInitialRiverConcentration = nitrateInitialRiverConcentration * CUBIC;
	NitrateFinalRiverConcentration = nitrateFinalRiverConcentration * CUBIC;
	PhosphateInitialRiverConcentration = phosphateInitialRiverConcentration * CUBIC;
	PhosphateFinalRiverConcentration = phosphateFinalRiverConcentration * CUBIC;
	SilicaInitialRiverConcentration = silicaInitialRiverConcentration * CUBIC;
	SilicaFinalRiverConcentration = silicaFinalRiverConcentration * CUBIC;

    PosBuildCrestumaLeverVert2DNutrients();
}

TCrestumaLeverVert2DDissobjt::TCrestumaLeverVert2DDissobjt(char* className,
                float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
                        :TNutrients(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildCrestumaLeverVert2DNutrients();
}
// ...


TCrestumaLeverVert2DDissobjt::TCrestumaLeverVert2DDissobjt(TEcoDynClass* APEcoDynClass,
        char* className) : TNutrients(APEcoDynClass, className)
{
    BuildCrestumaLeverVert2DNutrients();
}

void TCrestumaLeverVert2DDissobjt::PreBuildCrestumaLeverVert2DNutrients()
{
    AmmoniaInitialRiverConcentration = -99.0;
    AmmoniaFinalRiverConcentration = -99.0;
    NitriteInitialRiverConcentration = -99.0;
    NitriteFinalRiverConcentration = -99.0;
    NitrateInitialRiverConcentration = -99.0;
    NitrateFinalRiverConcentration = -99.0;
    PhosphateInitialRiverConcentration = -99.0;
    PhosphateFinalRiverConcentration = -99.0;
    SilicaInitialRiverConcentration = -99.0;
    SilicaFinalRiverConcentration = -99.0;
    NumberOfBoxes = MyPEcoDynClass->GetGridSize();
    BoxArray = MyPEcoDynClass->BoxArray;

    NumberOfLoads = MyPEcoDynClass->GetNumberOfLoads();
    NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
    NumberOfDaysForRiverLoads = floor(MyPEcoDynClass->GetNumberOfHoursForFlowTimeSeries() / DAYSTOHOURS);

	/* NPI-Work, 03 Oct 2013, AP - memory allocated and cleared here **
	 ***********
	 */
		try {
			NH4OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
			NO2OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
			NO3OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
			PO4OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
			SiO2OldLoad  = (double*) calloc (NumberOfBoxes, sizeof(double));

		  BoundaryNH4Concentration  = (double*) calloc (NumberOfDaysForRiverLoads, sizeof(double));
			BoundaryNO3Concentration = (double*) calloc (NumberOfDaysForRiverLoads, sizeof(double));
			BoundaryNO2Concentration = (double*) calloc (NumberOfDaysForRiverLoads, sizeof(double));
			BoundaryPhosphateConcentration = (double*) calloc (NumberOfDaysForRiverLoads, sizeof(double));
			BoundarySilicaConcentration = (double*) calloc (NumberOfDaysForRiverLoads, sizeof(double));
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
				cerr << "TCrestumaLeverVert2DDissobjt::PreBuildCrestumaLeverVert2DNutrients (1)- " << exc.what() << endl;
		}
#endif  // __BORLANDC__

    ParameterCount = 0;
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();

	VariableCount = 0;
	TimeStep = MyPEcoDynClass->GetTimeStep();
	ObjectCode = NUTROBJECTCODE;

    bdryNH4 = bdryNO3 = bdryNO2 = bdryPO4 = bdrySiO2 = false;
}

void TCrestumaLeverVert2DDissobjt::BuildCrestumaLeverVert2DNutrients()
{
    PreBuildCrestumaLeverVert2DNutrients();

    int X, Y, XV, YV, XL, YL;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrestumaLeverVert2DDissobjt::BuildCrestumaLeverVert2DNutrients - Nutrients parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            char MyParameter[65];

            ParametersNameArray = new VNA[NumberOfParameters];

            for (int i = 0; i < NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2+i, Y, MyParameter);
                strcpy(ParametersNameArray[i], MyParameter);

                if (strcmp(MyParameter, "Ammonia initial river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, AmmoniaInitialRiverConcentration);
                    AmmoniaInitialRiverConcentration = AmmoniaInitialRiverConcentration * CUBIC;  // CUBIC factor in river and ocean
                }
                else if (strcmp(MyParameter, "Ammonia final river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, AmmoniaFinalRiverConcentration);
                    AmmoniaFinalRiverConcentration = AmmoniaFinalRiverConcentration * CUBIC;  // CUBIC factor in river and ocean
                }
                else if (strcmp(MyParameter, "Nitrite initial river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NitriteInitialRiverConcentration);
                    NitriteInitialRiverConcentration = NitriteInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Nitrite final river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NitriteFinalRiverConcentration);
                    NitriteFinalRiverConcentration = NitriteFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Nitrate initial river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NitrateInitialRiverConcentration);
                    NitrateInitialRiverConcentration = NitrateInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Nitrate final river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NitrateFinalRiverConcentration);
                    NitrateFinalRiverConcentration = NitrateFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Phosphate initial river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, PhosphateInitialRiverConcentration);
                    PhosphateInitialRiverConcentration = PhosphateInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Phosphate final river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, PhosphateFinalRiverConcentration);
                    PhosphateFinalRiverConcentration = PhosphateFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Silica initial river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, SilicaInitialRiverConcentration);
                    SilicaInitialRiverConcentration = SilicaInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Silica final river concentration") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, SilicaFinalRiverConcentration);
                    SilicaFinalRiverConcentration = SilicaFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Simulation interval (hours)") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, SimulationInterval);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters - Undefined object in nutrients", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverVert2DDissobjt::BuildCrestumaLeverVert2DNutrients - Parameters - Undefined object Nutrients" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the variables
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrestumaLeverVert2DDissobjt::BuildCrestumaLeverVert2DNutrients - Nutrients variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        char MyVariable[65];
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
                strcpy(MyVariable, VariableNameArray[i]);
                if (strcmp(MyVariable, "Ammonia") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, RiverAmmonia);
                    if (!PReadWrite->FindString("Ammonia values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y + 2;
                    }
                    if (!PReadWrite->FindString("NH4 loads", XL, YL))
                    {
                        XL = X + 7 + i;
                        YL = Y + 2;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Ammonia[j]);
                        PReadWrite->ReadNumber(XL, YL + 1 + j, NH4Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Nitrate") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, RiverNitrate);
                    if (!PReadWrite->FindString("Nitrate values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y + 2;
                    }
                    if (!PReadWrite->FindString("NO3 loads", XL, YL))
                    {
                        XL = X + 7 + i;
                        YL = Y + 2;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrate[j]);
                        PReadWrite->ReadNumber(XL, YL + 1 + j, NO3Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Nitrite") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, RiverNitrite);
                    if (!PReadWrite->FindString("Nitrite values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y + 2;
                    }
                    if (!PReadWrite->FindString("NO2 loads", XL, YL))
                    {
                        XL = X + 7 + i;
                        YL = Y + 2;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrite[j]);
                        PReadWrite->ReadNumber(XL, YL + 1 + j, NO2Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Phosphate") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, RiverPhosphate);
                    if (!PReadWrite->FindString("Phosphate values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y + 2;
                    }
                    if (!PReadWrite->FindString("PO4 loads", XL, YL))
                    {
                        XL = X + 7 + i;
                        YL = Y + 2;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Phosphate[j]);
                        PReadWrite->ReadNumber(XL, YL + 1 + j, PO4Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Silica") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, RiverSilica);
                    if (!PReadWrite->FindString("Silica values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y + 2;
                    }
                    if (!PReadWrite->FindString("SiO2 loads", XL, YL))
                    {
                        XL = X + 7 + i;
                        YL = Y + 2;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Silica[j]);
                        PReadWrite->ReadNumber(XL, YL + 1 + j, SiO2Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary NH4 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NH4 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNH4Concentration[j]);
                        //if (j == 364) Debugger(BoundaryNH4Concentration[j]);
                    }
                    bdryNH4 = true;
                }
                else if (strcmp(MyVariable, "Boundary NO3 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NO3 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNO3Concentration[j]);
                        //if (j == 364) Debugger(BoundaryNO3Concentration[j]);
                    }
                    bdryNO3 = true;
                }
                else if (strcmp(MyVariable, "Boundary NO2 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NO2 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNO2Concentration[j]);
                        //if (j == 364) Debugger(BoundaryNO2Concentration[j]);
                    }
                    bdryNO2 = true;
                }
                else if (strcmp(MyVariable, "Boundary phosphate concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary phosphate concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryPhosphateConcentration[j]);
                        //if (j == 364) Debugger(BoundaryPhosphateConcentration[j]);
                    }
                    bdryPO4 = true;
                }
                else if (strcmp(MyVariable, "Boundary silica concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary silica concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundarySilicaConcentration[j]);
                        //if (j == 364) Debugger(BoundarySilicaConcentration[j]);
                    }
                    bdrySiO2 = true;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in TCrestumaLeverVert2DDissobjt nutrients", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverVert2DDissobjt::BuildCrestumaLeverVert2DNutrients - Variables - Undefined object Nutrients" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    PosBuildCrestumaLeverVert2DNutrients();
}

void TCrestumaLeverVert2DDissobjt::PosBuildCrestumaLeverVert2DNutrients()
{
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    Ammonia[i]   = Ammonia[i] * CUBIC; // umol l-1 * CUBIC = umol m-3
        Nitrite[i]   = Nitrite[i] * CUBIC;
        Nitrate[i]   = Nitrate[i] * CUBIC;
        Phosphate[i] = Phosphate[i] * CUBIC;
        Silica[i] = Silica[i] * CUBIC;

        NO3Load[i] = NO3Load[i] * CUBIC;
   	    NO2Load[i] = NO2Load[i] * CUBIC;
   	    NH4Load[i] = NH4Load[i] * CUBIC;
   	    PO4Load[i] = PO4Load[i] * CUBIC;
   	    SiO2Load[i] = SiO2Load[i] * CUBIC;

   	    NH4OldLoad[i]   = NH4Load[i];
   	    NO2OldLoad[i]   = NO2Load[i];
   	    NO3OldLoad[i]   = NO3Load[i];
   	    PO4OldLoad[i]   = PO4Load[i];
   	    SiO2OldLoad[i]  = SiO2Load[i];
    }
    RiverAmmonia = RiverAmmonia * CUBIC;
    RiverNitrite = RiverNitrite * CUBIC;
    RiverNitrate = RiverNitrate * CUBIC;
    RiverPhosphate = RiverPhosphate * CUBIC;
    RiverSilica = RiverSilica * CUBIC;
    for (int i = 0; i < NumberOfDaysForRiverLoads; i++)
    {
      BoundaryNH4Concentration[i] = BoundaryNH4Concentration[i] * CUBIC;
      BoundaryNO3Concentration[i] = BoundaryNO3Concentration[i] * CUBIC;
      BoundaryNO2Concentration[i] = BoundaryNO2Concentration[i] * CUBIC;
      BoundaryPhosphateConcentration[i] = BoundaryPhosphateConcentration[i] * CUBIC;
      BoundarySilicaConcentration[i] = BoundarySilicaConcentration[i] * CUBIC;
    }
    PastUpperLayer = GridLines - 1;
    CurrentUpperLayer = GridLines - 1;
}

TCrestumaLeverVert2DDissobjt::~TCrestumaLeverVert2DDissobjt()
{
    freeMemory();
}

void TCrestumaLeverVert2DDissobjt::freeMemory()
{
    try {
       free(NH4OldLoad);
       free(NO2OldLoad);
       free(NO3OldLoad);
       free(PO4OldLoad);
       free(SiO2OldLoad);
       free(BoundaryNH4Concentration);
       free(BoundaryNO3Concentration);
       free(BoundaryNO2Concentration);
       free(BoundaryPhosphateConcentration);
       free(BoundarySilicaConcentration);
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
				cerr << "TCrestumaLeverVert2DDissobjt::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TCrestumaLeverVert2DDissobjt::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Ammonia initial river concentration") == 0)
    {
        value = AmmoniaInitialRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Ammonia final river concentration") == 0)
    {
        value = AmmoniaFinalRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrite initial river concentration") == 0)
    {
        value = NitriteInitialRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrite final river concentration") == 0)
    {
        value = NitriteFinalRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrate initial river concentration") == 0)
    {
        value = NitrateInitialRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrate final river concentration") == 0)
    {
        value = NitrateFinalRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Phosphate initial river concentration") == 0)
    {
        value = PhosphateInitialRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Phosphate final river concentration") == 0)
    {
        value = PhosphateFinalRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Silica initial river concentration") == 0)
    {
        value = SilicaInitialRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Silica final river concentration") == 0)
    {
        value = SilicaFinalRiverConcentration / CUBIC;
    }
    else if (strcmp(MyParameter, "Simulation interval (hours)") == 0)
    {
        value = SimulationInterval;
    }
    else
        value = 0.0;
    return value;
}

bool TCrestumaLeverVert2DDissobjt::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Ammonia initial river concentration") == 0)
    {
        AmmoniaInitialRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Ammonia final river concentration") == 0)
    {
        AmmoniaFinalRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrite initial river concentration") == 0)
    {
        NitriteInitialRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrite final river concentration") == 0)
    {
        NitriteFinalRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrate initial river concentration") == 0)
    {
        NitrateInitialRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Nitrate final river concentration") == 0)
    {
        NitrateFinalRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Phosphate initial river concentration") == 0)
    {
        PhosphateInitialRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Phosphate final river concentration") == 0)
    {
        PhosphateFinalRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Silica initial river concentration") == 0)
    {
        SilicaInitialRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Silica final river concentration") == 0)
    {
        SilicaFinalRiverConcentration = value * CUBIC;
    }
    else if (strcmp(MyParameter, "Simulation interval (hours)") == 0)
    {
        SimulationInterval = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TCrestumaLeverVert2DDissobjt::SaveVariables()
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
    /*
     * save variables' initial values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Ammonia") == 0)
        {
            PReadWrite->WriteCell(RiverAmmonia / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Nitrite") == 0)
        {
            PReadWrite->WriteCell(RiverNitrite / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Nitrate") == 0)
        {
            PReadWrite->WriteCell(RiverNitrate / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Phosphate") == 0)
        {
            PReadWrite->WriteCell(RiverPhosphate / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Silica") == 0)
        {
            PReadWrite->WriteCell(RiverSilica / CUBIC, 8);
        }
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("Ammonia values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrite values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phosphate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Silica values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NH4 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("SiO2 loads");
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
        PReadWrite->WriteCell(Silica[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NH4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO3Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PO4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(SiO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    // Boundaries
    if (bdryNH4 || bdryNO2 || bdryNO3 || bdryPO4 || bdrySiO2)
    {
        PReadWrite->WriteCell("Boundary NH4 concentration values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Boundary NO3 concentration values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Boundary NO2 concentration values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Boundary phosphate concentration values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Boundary silica concentration values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfDaysForRiverLoads; j++)
        {
            PReadWrite->WriteCell(BoundaryNH4Concentration[j] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoundaryNO3Concentration[j] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoundaryNO2Concentration[j] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoundaryPhosphateConcentration[j] / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoundarySilicaConcentration[j] / CUBIC, 8);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TCrestumaLeverVert2DDissobjt::SaveParameters()
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
        if (strcmp(ParametersNameArray[i], "Ammonia initial river concentration") == 0)
        {
            PReadWrite->WriteCell(AmmoniaInitialRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Ammonia final river concentration") == 0)
        {
            PReadWrite->WriteCell(AmmoniaFinalRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Nitrite initial river concentration") == 0)
        {
            PReadWrite->WriteCell(NitriteInitialRiverConcentration / CUBIC, 9);
        }
       else if (strcmp(ParametersNameArray[i], "Nitrite final river concentration") == 0)
        {
            PReadWrite->WriteCell(NitriteFinalRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Nitrate initial river concentration") == 0)
        {
            PReadWrite->WriteCell(NitrateInitialRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Nitrate final river concentration") == 0)
        {
            PReadWrite->WriteCell(NitrateFinalRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Phosphate initial river concentration") == 0)
        {
            PReadWrite->WriteCell(PhosphateInitialRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Phosphate final river concentration") == 0)
        {
            PReadWrite->WriteCell(PhosphateFinalRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Silica initial river concentration") == 0)
        {
            PReadWrite->WriteCell(SilicaInitialRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Silica final river concentration") == 0)
        {
            PReadWrite->WriteCell(SilicaFinalRiverConcentration / CUBIC, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Simulation interval (hours)") == 0)
        {
            PReadWrite->WriteCell(SimulationInterval, 6);
        }
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}



void TCrestumaLeverVert2DDissobjt::Go()
{
	double MyBoxDepth, ALayer, MyPreviousSurfVolume, MyCurrentSurfVolume;
	// Active methods
   JulianDay = MyPEcoDynClass->GetJulianDay();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyTransportPointer != NULL)
   {
   	MyTransportPointer->Inquiry(GetEcoDynClassName(), ALayer,
                                      0,
                                      "Upper layer",
                                      ObjectCode);

      CurrentUpperLayer = floor(ALayer);
      if (CurrentUpperLayer != PastUpperLayer)
      {
      	for (int j = 0; j < GridColumns; j++)
      	{
            if (CurrentUpperLayer < PastUpperLayer)
            {
      			MyTransportPointer->Inquiry(GetEcoDynClassName(), MyPreviousSurfVolume,
			  											     PastUpperLayer * GridColumns + j,
               		                          "Past box volume",
                  		                       ObjectCode);

            	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyCurrentSurfVolume,
			  											     CurrentUpperLayer * GridColumns + j,
               		                          "Past box volume",
                  		                       ObjectCode);
            	Ammonia[CurrentUpperLayer * GridColumns + j] =
            		(
            			Ammonia[PastUpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		Ammonia[CurrentUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		) / (MyPreviousSurfVolume + MyCurrentSurfVolume);
               Nitrite[CurrentUpperLayer * GridColumns + j] =
            		(
            			Nitrite[PastUpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		Nitrite[CurrentUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		) / (MyPreviousSurfVolume + MyCurrentSurfVolume);
               Nitrate[CurrentUpperLayer * GridColumns + j] =
            		(
            			Nitrate[PastUpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		Nitrate[CurrentUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		) / (MyPreviousSurfVolume + MyCurrentSurfVolume);
               Phosphate[CurrentUpperLayer * GridColumns + j] =
            		(
            			Phosphate[PastUpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		Phosphate[CurrentUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		) / (MyPreviousSurfVolume + MyCurrentSurfVolume);
               Silica[CurrentUpperLayer * GridColumns + j] =
            		(
            			Silica[PastUpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		Silica[CurrentUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		) / (MyPreviousSurfVolume + MyCurrentSurfVolume);


            }
            else if (CurrentUpperLayer > PastUpperLayer)
            {
            	Ammonia[CurrentUpperLayer * GridColumns + j] = Ammonia[PastUpperLayer * GridColumns + j];
               Nitrite[CurrentUpperLayer * GridColumns + j] = Nitrite[PastUpperLayer * GridColumns + j];
               Nitrate[CurrentUpperLayer * GridColumns + j] = Nitrate[PastUpperLayer * GridColumns + j];
               Phosphate[CurrentUpperLayer * GridColumns + j] = Phosphate[PastUpperLayer * GridColumns + j];
               Silica[CurrentUpperLayer * GridColumns + j] = Silica[PastUpperLayer * GridColumns + j];
            }
      	}
      	PastUpperLayer = CurrentUpperLayer;
      }
   }
   //Debugger(SimulationInterval); Debugger(MyPEcoDynClass->GetTimeStep()); Debugger(HOURSTOSECONDS);
   for (int i = 0; i < NumberOfBoxes; i++)
	{
		/*if (Ammonia[i]   < 0.0)   Ammonia[i] = 0.0;
		if (Nitrite[i]   < 0.0)   Nitrite[i] = 0.0;
		if (Nitrate[i]   < 0.0)   Nitrate[i] = 0.0;
		if (Phosphate[i] < 0.0) Phosphate[i] = 0.0;
		if (Silica[i]    < 0.0)    Silica[i] = 0.0;*/

      NO3Load[i] = NO3OldLoad[i];
      NO2Load[i] = NO2OldLoad[i];
      NH4Load[i] = NH4OldLoad[i];
      PO4Load[i] = PO4OldLoad[i];
      SiO2Load[i] = SiO2OldLoad[i];
	}
   //Debugger(SiO2Load[38]);
	// Active methods
   Nitrification();
   //Debugger(JulianDay);
   int MyDay;
   MyDay = floor(MyPEcoDynClass->GetTime() + 1);
   if ((AmmoniaInitialRiverConcentration >= 0.0) && (AmmoniaFinalRiverConcentration >= 0.0))
   {
   	RiverAmmonia = RiverAmmonia +  (AmmoniaFinalRiverConcentration - AmmoniaInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS/** DAYSTOHOURS*/;

   }
   else
   {
      //if (JulianDay > 365)
      if (MyDay > NumberOfDaysForRiverLoads)
        RiverAmmonia = BoundaryNH4Concentration[0];
      else
        RiverAmmonia = BoundaryNH4Concentration[MyDay - 1];
   }

   if ((NitriteInitialRiverConcentration >= 0.0) && (NitriteFinalRiverConcentration >= 0.0))
   {
   	RiverNitrite = RiverNitrite +  (NitriteFinalRiverConcentration - NitriteInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS/** DAYSTOHOURS*/;

   }
   else
   {
      //if (JulianDay > 365)
      if (MyDay > NumberOfDaysForRiverLoads)
        RiverNitrite = BoundaryNO2Concentration[0];
      else
        RiverNitrite = BoundaryNO2Concentration[MyDay - 1];
   }

   if ((NitrateInitialRiverConcentration >= 0.0) && (NitrateFinalRiverConcentration >= 0.0))
   {
   	RiverNitrate = RiverNitrate +  (NitrateFinalRiverConcentration - NitrateInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS/** DAYSTOHOURS*/;

   }
   else
   {
      //if (JulianDay > 365)
      if (MyDay > NumberOfDaysForRiverLoads)
        RiverNitrate = BoundaryNO3Concentration[0];
      else
        RiverNitrate = BoundaryNO3Concentration[MyDay - 1];
   }

   if ((PhosphateInitialRiverConcentration >= 0.0) && (PhosphateFinalRiverConcentration >= 0.0))
   {
   	RiverPhosphate = RiverPhosphate +  (PhosphateFinalRiverConcentration - PhosphateInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS/** DAYSTOHOURS*/;

   }
   else
   {
      //if (JulianDay > 365)
      if (MyDay > NumberOfDaysForRiverLoads)
        RiverPhosphate = BoundaryPhosphateConcentration[0];
      else
        RiverPhosphate = BoundaryPhosphateConcentration[MyDay - 1];
   }

   if ((SilicaInitialRiverConcentration >= 0.0) && (SilicaFinalRiverConcentration >= 0.0))
   {
   	RiverSilica = RiverSilica +  (SilicaFinalRiverConcentration - SilicaInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS/** DAYSTOHOURS*/;

   }
   else
   {
      //if (JulianDay > 365)
      if (MyDay > NumberOfDaysForRiverLoads)
        RiverSilica = BoundarySilicaConcentration[0];
      else
        RiverSilica = BoundarySilicaConcentration[MyDay - 1];
   }

}



void TCrestumaLeverVert2DDissobjt::Integrate()
{
   int i,j;
   //Debugger(NO3Load[38]); Debugger(NO2Load[38]); Debugger(NH4Load[38]); Debugger(PO4Load[38]); Debugger(SiO2Load[38]);
	// EcoDynClass ammonia within the box

   //Debugger(Ammonia[1]); Debugger(Nitrate[1]); Debugger(Nitrite[1]);
   //Debugger(NH4Flux[1]); Debugger(NO3Flux[1]); Debugger(NO2Flux[1]);
/*
	Generic = Ammonia; GenericFlux = NH4Flux;
	Integration();
	Ammonia = Generic; NH4Flux = GenericFlux;

	// EcoDynClass nitrite within the box
	Generic = Nitrite; GenericFlux = NO2Flux;
	Integration();
	Nitrite = Generic; NO2Flux = GenericFlux;

	// EcoDynClass nitrate within the box
	Generic = Nitrate; GenericFlux = NO3Flux;
	Integration();
	Nitrate = Generic; NO3Flux = GenericFlux;

	// EcoDynClass phosphate within the box
	Generic = Phosphate; GenericFlux = PO4Flux;
	Integration();
	Phosphate = Generic; PO4Flux = GenericFlux;

	// EcoDynClass silica within the box
	Generic = Silica; GenericFlux = SiO2Flux;
	Integration();
	Silica = Generic; SiO2Flux = GenericFlux;
*/
    Integration(Ammonia, NH4Flux,
                Nitrite, NO2Flux,
                Nitrate, NO3Flux,
                Phosphate, PO4Flux,
                Silica, SiO2Flux);

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (NO3Load[i] <= 0.0) NO3Load[i] = Nitrate[i];
      if (NO2Load[i] <= 0.0) NO2Load[i] = Nitrite[i];
      if (NH4Load[i] <= 0.0) NH4Load[i] = Ammonia[i];
      if (PO4Load[i] <= 0.0) PO4Load[i] = Phosphate[i];
      if (SiO2Load[i] <= 0.0) SiO2Load[i] = Silica[i];
   }
	// Transport the nutrients
   //Debugger(SiO2Load[38]); Debugger(PO4Load[38]);//Debugger(Silica[38]);Debugger(SiO2OldLoad[38]);
   //Debugger(NO3Load[38]); Debugger(NO2Load[38]);Debugger(NH4Load[38]);
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{


		MyTransportPointer->Go(Ammonia, NH4Load, RiverAmmonia, OceanAmmonia);

		MyTransportPointer->Go(Nitrite, NO2Load, RiverNitrite, OceanNitrite);

		MyTransportPointer->Go(Nitrate, NO3Load, RiverNitrate, OceanNitrate);

		MyTransportPointer->Go(Phosphate, PO4Load, RiverPhosphate, OceanPhosphate);

		MyTransportPointer->Go(Silica, SiO2Load, RiverSilica, OceanSilica);

	}
   for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (Ammonia[i]   < 0.0)   Ammonia[i] = 0.0;
		if (Nitrite[i]   < 0.0)   Nitrite[i] = 0.0;
		if (Nitrate[i]   < 0.0)   Nitrate[i] = 0.0;
		if (Phosphate[i] < 0.0) Phosphate[i] = 0.0;
		if (Silica[i]    < 0.0)    Silica[i] = 0.0;
	}
}



// Dissolved object CPP code file

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
#include "EcoDynClass.h"

// #include <integral.h>

// TCrestumaLeverDissobjt Class
//
// Constructors invoked outside EcoDyn shell...
//

TCrestumaLeverDissobjt::TCrestumaLeverDissobjt(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[],
        double riverAmmonia, double oceanAmmonia, double aAmmonia[], double aNH4Load[],
        double riverNitrite, double oceanNitrite, double aNitrite[], double aNO2Load[],
        double riverNitrate, double oceanNitrate, double aNitrate[], double aNO3Load[],
        double riverPhosphate, double oceanPhosphate, double aPhosphate[], double aPO4Load[],
        double riverSilica, double oceanSilica, double aSilica[], double aSiO2Load[],
        double riverDON, double oceanDON, double aDON[], double aDONLoad[],
        double riverDOP, double oceanDOP, double aDOP[], double aDOPLoad[],
        double riverDOC, double oceanDOC, double aDOC[], double aDOCLoad[],
        int nParams, double simulationInterval,
        double ammoniaInitialRiverConcentration, double ammoniaFinalRiverConcentration,
        double nitriteInitialRiverConcentration, double nitriteFinalRiverConcentration,
        double nitrateInitialRiverConcentration, double nitrateFinalRiverConcentration,
        double phosphateInitialRiverConcentration, double phosphateFinalRiverConcentration,
        double silicaInitialRiverConcentration, double silicaFinalRiverConcentration)
    :TNutrientsBasic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
                aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, riverAmmonia, oceanAmmonia, aAmmonia, aNH4Load,
                riverNitrite, oceanNitrite, aNitrite, aNO2Load,
                riverNitrate, oceanNitrate, aNitrate, aNO3Load,
                riverPhosphate, oceanPhosphate, aPhosphate, aPO4Load,
                riverSilica, oceanSilica, aSilica, aSiO2Load,
                riverDON, oceanDON, aDON, aDONLoad,
                riverDOP, oceanDOP, aDOP, aDOPLoad,
                riverDOC, oceanDOC, aDOC, aDOCLoad)
{
    PreBuildCrestumaLeverNutrients();

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

    PosBuildCrestumaLeverNutrients();
}

TCrestumaLeverDissobjt::TCrestumaLeverDissobjt(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TNutrientsBasic(className, timeStep, morphologyFilename, variablesFilename)
{
    strcpy(ParsFileName, parametersFilename);
    BuildCrestumaLeverNutrients();
}
// ...

TCrestumaLeverDissobjt::TCrestumaLeverDissobjt(TEcoDynClass* APEcoDynClass,
        char* className) : TNutrientsBasic(APEcoDynClass, className)
{
    BuildCrestumaLeverNutrients();
}

void TCrestumaLeverDissobjt::PreBuildCrestumaLeverNutrients()
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
	/* NPI-Work, 03 Oct 2013, AP - memory allocated and cleared here **
	 ***********
	 */
    NH4OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
    NO2OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
    NO3OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
    PO4OldLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
    SiO2OldLoad  = (double*) calloc (NumberOfBoxes, sizeof(double));
}

void TCrestumaLeverDissobjt::BuildCrestumaLeverNutrients()
{
	int i,j;
    PreBuildCrestumaLeverNutrients();

	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrestumaLeverDissobjt::BuildCrestumaLeverNutrients - Nutrients parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        int X,Y;
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            char MyParameter[65];

            ParametersNameArray = new VNA[NumberOfParameters];
            // read in the parameters names

            for (i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Ammonia initial river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, AmmoniaInitialRiverConcentration);
                    AmmoniaInitialRiverConcentration = AmmoniaInitialRiverConcentration * CUBIC;  // CUBIC factor in river and ocean
                }
                else if (strcmp(MyParameter, "Ammonia final river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, AmmoniaFinalRiverConcentration);
                    AmmoniaFinalRiverConcentration = AmmoniaFinalRiverConcentration * CUBIC;  // CUBIC factor in river and ocean
                }
                else if (strcmp(MyParameter, "Nitrite initial river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, NitriteInitialRiverConcentration);
                    NitriteInitialRiverConcentration = NitriteInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Nitrite final river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, NitriteFinalRiverConcentration);
                    NitriteFinalRiverConcentration = NitriteFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Nitrate initial river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, NitrateInitialRiverConcentration);
                    NitrateInitialRiverConcentration = NitrateInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Nitrate final river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, NitrateFinalRiverConcentration);
                    NitrateFinalRiverConcentration = NitrateFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Phosphate initial river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, PhosphateInitialRiverConcentration);
                    PhosphateInitialRiverConcentration = PhosphateInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Phosphate final river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, PhosphateFinalRiverConcentration);
                    PhosphateFinalRiverConcentration = PhosphateFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Silica initial river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, SilicaInitialRiverConcentration);
                    SilicaInitialRiverConcentration = SilicaInitialRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Silica final river concentration") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, SilicaFinalRiverConcentration);
                    SilicaFinalRiverConcentration = SilicaFinalRiverConcentration * CUBIC;
                }
                else if (strcmp(MyParameter, "Simulation interval (hours)") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SimulationInterval);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters - Undefined object in nutrients", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverDissobjt::BuildCrestumaLeverNutrients - Parameters - Undefined object in nutrients" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    PosBuildCrestumaLeverNutrients();
}

void TCrestumaLeverDissobjt::PosBuildCrestumaLeverNutrients()
{
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    NH4OldLoad[i]   = NH4Load[i];
   	    NO2OldLoad[i]   = NO2Load[i];
   	    NO3OldLoad[i]   = NO3Load[i];
   	    PO4OldLoad[i]   = PO4Load[i];
   	    SiO2OldLoad[i]  = SiO2Load[i];
    }
}

TCrestumaLeverDissobjt::~TCrestumaLeverDissobjt()
{
    freeMemory();
}

void TCrestumaLeverDissobjt::freeMemory()
{
    try {
	/* NPI-Work, 03 Oct 2013, AP - free memory replaces delete [] **
	 ***********
	 */
        free(NH4OldLoad);
        free(NO2OldLoad);
        free(NO3OldLoad);
        free(PO4OldLoad);
        free(SiO2OldLoad);
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else // __BORLANDC__
		catch (exception &exc) {
				cerr << "TCrestumaLeverDissobjt::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TCrestumaLeverDissobjt::GetParameterValue(char* MyParameter)
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

bool TCrestumaLeverDissobjt::SetParameterValue(char* MyParameter, double value)
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
bool TCrestumaLeverDissobjt::SaveParameters()
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
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0)
            PReadWrite->WriteSeparator();

        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
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
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TCrestumaLeverDissobjt::Go()
{
   //Debugger(NO3Load[16]); Debugger(NO2Load[16]); Debugger(NH4Load[16]); Debugger(PO4Load[16]); Debugger(SiO2Load[16]);
	// Active methods
   //Debugger(NO2Flux[1]);
	Nitrification();
   //Debugger(NO2Flux[1]);
   if ((AmmoniaInitialRiverConcentration >= 0.0) && (AmmoniaFinalRiverConcentration >= 0.0))
   {
   	RiverAmmonia = RiverAmmonia +  (AmmoniaFinalRiverConcentration - AmmoniaInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() * DAYSTOHOURS;

   }

   if ((NitriteInitialRiverConcentration >= 0.0) && (NitriteFinalRiverConcentration >= 0.0))
   {
   	RiverNitrite = RiverNitrite +  (NitriteFinalRiverConcentration - NitriteInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() * DAYSTOHOURS;

   }

   if ((NitrateInitialRiverConcentration >= 0.0) && (NitrateFinalRiverConcentration >= 0.0))
   {
   	RiverNitrate = RiverNitrate +  (NitrateFinalRiverConcentration - NitrateInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() * DAYSTOHOURS;

   }

   if ((PhosphateInitialRiverConcentration >= 0.0) && (PhosphateFinalRiverConcentration >= 0.0))
   {
   	RiverPhosphate = RiverPhosphate +  (PhosphateFinalRiverConcentration - PhosphateInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() * DAYSTOHOURS;

   }

   if ((SilicaInitialRiverConcentration >= 0.0) && (SilicaFinalRiverConcentration >= 0.0))
   {

      RiverSilica = RiverSilica +  (SilicaFinalRiverConcentration - SilicaInitialRiverConcentration) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() * DAYSTOHOURS;

   }

}



void TCrestumaLeverDissobjt::Integrate()
{
   int i,j;
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
      if (NH4Load[i] < 0.0) NH4Load[i] = Ammonia[i];
      if (PO4Load[i] <= 0.0) PO4Load[i] = Phosphate[i];
      if (SiO2Load[i] <= 0.0) SiO2Load[i] = Silica[i];

   }
	// Transport the nutrients
   //Debugger(NH4Load[16]); Debugger(Ammonia[16]);
   //Debugger(NO3Load[16]); Debugger(NO2Load[16]);Debugger(NH4Load[16]);Debugger(PO4Load[16]); Debugger(SiO2Load[16]);
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

      NO3Load[i] = NO3OldLoad[i];
      NO2Load[i] = NO2OldLoad[i];
      NH4Load[i] = NH4OldLoad[i];
      PO4Load[i] = PO4OldLoad[i];
      SiO2Load[i] = SiO2OldLoad[i];
	}
   //Debugger(NH4OldLoad[16]);
}




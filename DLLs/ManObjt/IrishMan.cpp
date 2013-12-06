// Man object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.06.25
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

// Carlingford man class
//
// Constructors invoked outside EcoDyn shell...
//

TIrishMan::TIrishMan(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aSeedInput[], double aSeedWeight[],
        double aCrop[], double aZoobLowerLimit[], double aZoobUpperLimit[],
        double salePricePerkgCrassostrea, double aPricePerkgCrassostreaHarvest[],
        double pricePerkgCrassostreaSeed,
        int nParams, double carbonToDryWeight, double harvestableWeight,
        int firstSeedingDay, int lastSeedingDay,
        int firstHarvestDay, int lastHarvestDay)
    :TMan(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
            aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildIrishMan();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
        SeedInput[i] = aSeedInput[i];
        SeedWeight[i] = aSeedWeight[i];
        Crop[i] = aCrop[i];
        ZoobLowerLimit[i] = aZoobLowerLimit[i];
        ZoobUpperLimit[i] = aZoobUpperLimit[i];
        PricePerkgCrassostreaHarvest[i] = aPricePerkgCrassostreaHarvest[i];
	}
    SalePricePerkgCrassostrea = salePricePerkgCrassostrea;
    PricePerkgCrassostreaSeed = pricePerkgCrassostreaSeed;

    NumberOfParameters = nParams;

    CarbonToDryWeight = carbonToDryWeight;
    HarvestableWeight = harvestableWeight;
    FirstSeedingDay = firstSeedingDay;
    LastSeedingDay =  lastSeedingDay;
    FirstHarvestDay = firstHarvestDay;
    LastHarvestDay = lastHarvestDay;
}

TIrishMan::TIrishMan(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
        :TMan(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildIrishMan();
}
// ...

TIrishMan::TIrishMan(TEcoDynClass* APEcoDynClass, char* className)
							: TMan(APEcoDynClass, className)
{
    BuildIrishMan();
}

void TIrishMan::PreBuildIrishMan()
{
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		HarvestBiomass               = new double[NumberOfBoxes];
		HarvestFlux                  = new double[NumberOfBoxes];
		SeedInput                    = new double[NumberOfBoxes];
		SeedWeight                   = new double[NumberOfBoxes];
		Crop                         = new double[NumberOfBoxes];
		ZoobLowerLimit               = new double[NumberOfBoxes];
		ZoobUpperLimit               = new double[NumberOfBoxes];
		HarvestEffort                = new double[NumberOfBoxes];
		HarvestEfficiency            = new double[NumberOfBoxes];
		PricePerkgCrassostreaHarvest = new double[NumberOfBoxes];
		HarvestableBiomass           = new double[NumberOfBoxes];

		// Create generics

/*		Generic     = new double[NumberOfBoxes];
		GenericFlux = new double[NumberOfBoxes];*/

		// Fluxes and conversions
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			HarvestBiomass[i] = HarvestFlux[i] = 0;
			HarvestEffort[i] = HarvestEfficiency[i] = 1;
    		HarvestableBiomass[i] = 0.0;
		}
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Man object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TIrishMan::PreBuildIrishMan - Man object array not dimensioned" << endl;
#endif  // __BORLANDC__
}

void TIrishMan::BuildIrishMan()
{
    PreBuildIrishMan();

    int X,Y, XV, YV;

	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Man");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Man variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TIrishMan::BuildIrishMan - Variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Man", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);

                if (strcmp(MyVariable, "C. gigas seed (tons/day May to June)") == 0)
                {
                    if (PReadWrite->FindString("C. gigas seed (tons/day May to June) values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, SeedInput[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, SeedInput[j]);
        //					SeedInput[j] = Generic[j];
        //					Debugger(SeedInput[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Seed individual weight") == 0)
                {
                    if (PReadWrite->FindString("Seed individual weight values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, SeedWeight[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, SeedWeight[j]);
        //					SeedWeight[j] = Generic[j];
        //					Debugger(SeedWeight[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "C. gigas harvest (tons/day Oct. to Dec.)") == 0)
                {
                    if (PReadWrite->FindString("C. gigas harvest (tons/day Oct. to Dec.) values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Crop[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Crop[j]);
        //					Crop[j] = Generic[j];
                        }
                    }
                }
                else if (strcmp(MyVariable, "C. gigas lower limit (tons)") == 0)
                {
                    if (PReadWrite->FindString("C. gigas lower limit (tons) values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, ZoobLowerLimit[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, ZoobLowerLimit[j]);
        //					ZoobLowerLimit[j] = Generic[j];
                        }
                    }
                }
                else if (strcmp(MyVariable, "C. gigas upper limit (tons)") == 0)
                {
                    if (PReadWrite->FindString("C. gigas upper limit (tons) values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, ZoobUpperLimit[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, ZoobUpperLimit[j]);
        //					ZoobUpperLimit[j] = Generic[j];
                        }
                    }
                }
                else if (strcmp(MyVariable, "C. gigas market price /kg") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X+5+j, i, SalePricePerkgCrassostrea);
    //					SalePricePerkgCrassostrea = Generic[j];
                    }
                }
                else if (strcmp(MyVariable, "C. gigas harvest cost/kg") == 0)
                {
                    if (PReadWrite->FindString("C. gigas harvest cost/kg values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, PricePerkgCrassostreaHarvest[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, PricePerkgCrassostreaHarvest[j]);
        //					PricePerkgCrassostreaHarvest[j] = Generic[j];
                        }
                    }
                }
                else if (strcmp(MyVariable, "C. gigas seed cost/kg") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X+5+j, i, PricePerkgCrassostreaSeed);
    //					PricePerkgCrassostreaSeed = Generic[j];
                    }
                }
                // More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Irish Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TIrishMan::BuildIrishMan - Variables: undefined object Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// New parameters stuff - JGF 96.05.03

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Man");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Man parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TIrishMan::BuildIrishMan - parameters file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Man", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Carbon to dry weight") == 0)
                    PReadWrite->ReadNumber(X+3, i, CarbonToDryWeight);
                else if (strcmp(MyParameter, "Harvestable weight") == 0)
                    PReadWrite->ReadNumber(X+3, i, HarvestableWeight);
                else if (strcmp(MyParameter, "First seeding day") == 0)
                    PReadWrite->ReadNumber(X+3, i, FirstSeedingDay);
                else if (strcmp(MyParameter, "Last seeding day") == 0)
                    PReadWrite->ReadNumber(X+3, i, LastSeedingDay);
                else if (strcmp(MyParameter, "First harvest day") == 0)
                    PReadWrite->ReadNumber(X+3, i, FirstHarvestDay);
                else if (strcmp(MyParameter, "Last harvest day") == 0)
                    PReadWrite->ReadNumber(X+3, i, LastHarvestDay);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Irish Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TIrishMan::BuildIrishMan - Parameeters: undefined object Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TIrishMan::~TIrishMan()
{
    freeMemory();
}

void TIrishMan::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] HarvestBiomass;
            delete [] HarvestFlux;
            delete [] SeedInput;
            delete [] SeedWeight;
            delete [] Crop;
            delete [] ZoobLowerLimit;
            delete [] ZoobUpperLimit;
            delete [] HarvestEffort;
            delete [] HarvestEfficiency;
            delete [] PricePerkgCrassostreaHarvest;

            // put these in TEcoDynClass - set them to null in the constructor
            // delete them in the destructor if not null
    /*		delete [] Generic;
            delete [] GenericFlux;*/
            //delete [] GenericInterfaceExchange;
            delete [] HarvestableBiomass;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
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
        cerr << "TIrishMan::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

double TIrishMan::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Carbon to dry weight") == 0)
        value = CarbonToDryWeight;
    else if (strcmp(MyParameter, "Harvestable weight") == 0)
        value = HarvestableWeight;
    else if (strcmp(MyParameter, "First seeding day") == 0)
        value = FirstSeedingDay;
    else if (strcmp(MyParameter, "Last seeding day") == 0)
        value = LastSeedingDay;
    else if (strcmp(MyParameter, "First harvest day") == 0)
        value = FirstHarvestDay;
    else if (strcmp(MyParameter, "Last harvest day") == 0)
        value = LastHarvestDay;
    else
        value = 0.0;
    return value;
}

bool TIrishMan::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Carbon to dry weight") == 0)
        CarbonToDryWeight = value;
    else if (strcmp(MyParameter, "Harvestable weight") == 0)
        HarvestableWeight = value;
    else if (strcmp(MyParameter, "First seeding day") == 0)
        FirstSeedingDay = value;
    else if (strcmp(MyParameter, "Last seeding day") == 0)
        LastSeedingDay = value;
    else if (strcmp(MyParameter, "First harvest day") == 0)
        FirstHarvestDay = value;
    else if (strcmp(MyParameter, "Last harvest day") == 0)
        LastHarvestDay = value;
    else
        rc = false;

    return rc;
}


// AP, 2007.07.16
bool TIrishMan::SaveVariables()
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
        if (strcmp(VariableNameArray[i], "C. gigas market price /kg") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(SalePricePerkgCrassostrea, 9);
        }
        else if (strcmp(VariableNameArray[i], "C. gigas seed cost/kg") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(PricePerkgCrassostreaSeed, 9);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("C. gigas seed (tons/day May to June) values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Seed individual weight values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas harvest (tons/day Oct. to Dec.) values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas lower limit (tons) values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas upper limit (tons) values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas harvest cost/kg values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(SeedInput[j], 9);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(SeedWeight[j], 9);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Crop[j], 9);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ZoobLowerLimit[j], 9);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ZoobUpperLimit[j], 9);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PricePerkgCrassostreaHarvest[j], 9);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TIrishMan::SaveParameters()
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

        if (strcmp(ParametersNameArray[i], "Carbon to dry weight") == 0)
        {
            PReadWrite->WriteCell(CarbonToDryWeight, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Harvestable weight") == 0)
        {
            PReadWrite->WriteCell(HarvestableWeight, 9);
        }
        else if (strcmp(ParametersNameArray[i], "First seeding day") == 0)
        {
            PReadWrite->WriteCell(FirstSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Last seeding day") == 0)
        {
            PReadWrite->WriteCell(LastSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "First harvest day") == 0)
        {
            PReadWrite->WriteCell(FirstHarvestDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Last harvest day") == 0)
        {
            PReadWrite->WriteCell(LastHarvestDay, 0);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TIrishMan::Go()
{
	const int LastDayOfTheYear = 365;
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	//if (MyTime != TimeBefore)
		JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;

	if ((JulianDay >= FirstSeedingDay) && (JulianDay < LastSeedingDay))
		Seed();
	if ( FirstHarvestDay < LastHarvestDay)
	{
		if (
			 ((JulianDay > FirstHarvestDay) && (JulianDay <= LastHarvestDay))
			 && (MyPEcoDynClass->GetYear() > 1)
			)
			ZooHarvest();
	}
   // new code to allow harvest to continue into the new year - JGF 97.07.29
	else if ( FirstHarvestDay > LastHarvestDay)

   if (
			 (
			  ((JulianDay > FirstHarvestDay) && (JulianDay <= LastDayOfTheYear)) ||
			  ( JulianDay < LastHarvestDay )
			 )
			 && (MyPEcoDynClass->GetYear() > 1)
			)

	{
		if (
			 (
			  ((JulianDay > FirstHarvestDay) && (JulianDay <= LastDayOfTheYear)) &&
			  (MyPEcoDynClass->GetYear() > 1)
			 ) ||
			 (
			  (JulianDay < LastHarvestDay) &&
			  (MyPEcoDynClass->GetYear() > 2)
			 )
			)
			ZooHarvest();
	}
	else
		for (int i = 0; i < NumberOfBoxes; i++)
			HarvestBiomass[i] = 0;
}

void TIrishMan::Inquiry(char* srcName, double &Value,
									 int BoxNumber,
									 char* ParameterName,
									 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Harvest (TonFW)") == 0)
		Value = HarvestBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Harvest gross value") == 0)
		Value = HarvestBiomass[MyBoxNumber]  // millions of pounds
				  * SalePricePerkgCrassostrea
				  / CUBIC;
	else if (strcmp(MyParameter, "Harvest net value") == 0)
		Value = HarvestBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Seed individual weight") == 0)
		Value = SeedWeight[MyBoxNumber];
	else if (strcmp(MyParameter, "C. gigas seed (tons/day May to June)") == 0)
		Value = SeedInput[MyBoxNumber];
	else if (strcmp(MyParameter, "C. gigas harvest (tons/day Oct. to Dec.)") == 0)
		Value = Crop[MyBoxNumber];
	else if (strcmp(MyParameter,"Harvestable biomass (tons)")== 0)
		Value = HarvestableBiomass[MyBoxNumber];
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
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 2",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TIrishMan::Inquiry 2 - " << MyParameter
                << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TIrishMan::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Harvest (TonFW)") == 0)
		HarvestBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Harvest gross value") == 0)
		HarvestBiomass[BoxNumber]  = Value * CUBIC
				  / SalePricePerkgCrassostrea;
	else if (strcmp(VariableName, "Harvest net value") == 0)
		HarvestBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Seed individual weight") == 0)
		SeedWeight[BoxNumber] = Value;
	else if (strcmp(VariableName, "C. gigas seed (tons/day May to June)") == 0)
		SeedInput[BoxNumber] = Value;
	else if (strcmp(VariableName, "C. gigas harvest (tons/day Oct. to Dec.)") == 0)
		Crop[BoxNumber] = Value;
	else if (strcmp(VariableName,"Harvestable biomass (tons)")== 0)
		HarvestableBiomass[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

void TIrishMan::Integrate()
{
	// Man
//	Generic = HarvestBiomass; GenericFlux = HarvestFlux;
	Integration(HarvestBiomass, HarvestFlux);
//	HarvestBiomass = Generic; HarvestFlux = GenericFlux;
	for (int i = 0; i < NumberOfBoxes; i++)
		if (HarvestBiomass[i]   < 0.0)
			HarvestBiomass[i] = 0.0;
}

// Protected methods
void TIrishMan::ZooHarvest()
{
	double
			 Harvest,
			 BivalveSize;

	TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if ( MyZoobenthosPointer != NULL )
		{

			MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), HarvestableBiomass[i],
														i,
														HarvestableWeight,
														ObjectCode);

			if ( HarvestableBiomass[i] == 0 )
				Harvest = 0;
			else
				Harvest = Crop[i];
			if ( HarvestableBiomass[i] < Crop[i] )
				Harvest = HarvestableBiomass[i];
			if ( HarvestableBiomass[i] > ZoobUpperLimit[i] )
				Harvest = Harvest
							 + (HarvestableBiomass[i] - ZoobUpperLimit[i])
								/10;

			MyZoobenthosPointer->Update(GetEcoDynClassName(), (-Harvest * CUBIC * CUBIC)    // Fresh live weight m-3
													/ BoxArray[i].Volume,
													HarvestableWeight, //HarvestableWeight
													i,
													ObjectCode);
			HarvestFlux[i] = Harvest;
		}
	}
}

void TIrishMan::Seed()
{
	double SeedIndividualWeight;
	TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
		if ( MyZoobenthosPointer != NULL )
		{
			SeedIndividualWeight = SeedWeight[i];
			// Debugger(SeedWeight[i]);
			// return positive value because this is added
			MyZoobenthosPointer->Update(GetEcoDynClassName(), (SeedInput[i] * CUBIC * CUBIC)
													/ BoxArray[i].Volume,
													SeedIndividualWeight,
													i,
													0,
													ObjectCode);
			/*
			MyZoobenthosPointer->Update(GetEcoDynClassName(), (SeedInput[i] * CUBIC * CUBIC)
													/ BoxArray[i].Volume,
													SeedIndividualWeight,
													i,
													"C. gigas biomass 1",
													ObjectCode);
			*/
		}
}

/*
void TIrishMan::ZooHarvest()
{
	double BivalveHarvestableMass,
			 Harvest,
			 BivalveSize;

	TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{

		if ( MyZoobenthosPointer != NULL )
		{
			MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), BivalveHarvestableMass,
														i,
														PreySpecies,
														ObjectCode);
//			if (BivalveHarvestableMass != 0)
//				Debugger(BivalveHarvestableMass);
			if ( BivalveHarvestableMass == 0 )
				Harvest = 0;
			else
				Harvest = Crop[i];
			if ( BivalveHarvestableMass < Crop[i] )
				Harvest = BivalveHarvestableMass;
			if ( BivalveHarvestableMass > ZoobUpperLimit[i] )
				Harvest = Harvest
							 + (BivalveHarvestableMass - ZoobUpperLimit[i])
								/10;

		 //	if (Harvest != 0)
		 //	{
		 //		Debugger(Crop[i]);
		 //		Debugger(Harvest);
		 //	}

			MyZoobenthosPointer->Inquiry(GetEcoDynClassName(), BivalveSize,
														i,
														"C. gigas individual weight 3",
														ObjectCode);
			MyZoobenthosPointer->Update(GetEcoDynClassName(),
                                        (-Harvest * CUBIC * CUBIC)    // Fresh live weight m-3
													/ BoxArray[i].Volume,
													BivalveSize,                 //HarvestableWeight
													i,
													2,
													ObjectCode);

			//MyZoobenthosPointer->Update(GetEcoDynClassName(),
                                        (-Harvest * CUBIC * CUBIC)    // Fresh live weight m-3
			//										/ BoxArray[i].Volume,
			//										BivalveSize,                 //HarvestableWeight
			//										i,
			//										"PreySpecies",
			//										ObjectCode);

			HarvestFlux[i] = // HarvestFlux[i] +
								  Harvest;
		}
	}
} */

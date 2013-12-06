// Water temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"

#ifndef _PORT_FORTRAN_
#include "iodll.h"
#endif  // _PORT_FORTRAN_

// TWaterTemperature Class
//
#ifdef _PORT_FORTRAN_
//-----------------------------------------------
// ...Methods invoked from Fortran...
//
//        only one box
//
//-----------------------------------------------

void watertemp_getvalues__(float* watertemperature, float* waterirradiance,
            float* netheatflux, float* latentheatflux,
            float* sensibleheatflux, float* relativehumidity)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   ptr = TWaterTemperature::getWaterTemperature();

   strcpy(MyParameter, "Water temperature\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *watertemperature = Value;

   strcpy(MyParameter, "Water irradiance\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *waterirradiance = Value;

   strcpy(MyParameter, "Net heat flux\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *netheatflux = Value;

   strcpy(MyParameter, "Latent heat flux\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *latentheatflux = Value;

   strcpy(MyParameter, "Sensible heat flux\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *sensibleheatflux = Value;

   strcpy(MyParameter, "Relative humidity\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *relativehumidity = Value;
}

void watertemp_getwatertemperature__(float* watertemperature)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Water temperature\0");

   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *watertemperature = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Water temperature (oC): " << *watertemperature << endl;
}

void watertemp_getwaterirradiance__(float* waterirradiance)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Water irradiance\0");

   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *waterirradiance = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Water irradiance (W/m2): " << *waterirradiance << endl;
}

void watertemp_getnetheatflux__(float* netheatflux)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Net heat flux\0");

   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *netheatflux = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Net heat flux (W/m2): " << *netheatflux << endl;
}

void watertemp_getlatentheatflux__(float* latentheatflux)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Latent heat flux\0");

   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *latentheatflux = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Latent heat flux (W/m2): " << *latentheatflux << endl;
}

void watertemp_getsensibleheatflux__(float* sensibleheatflux)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Sensible heat flux\0");

   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *sensibleheatflux = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Sensible heat flux (W/m2): " << *sensibleheatflux << endl;
}

void watertemp_getrelativehumidity__(float* relativehumidity)
{
   TWaterTemperature* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Relative humidity\0");

   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *relativehumidity = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Relative humidity (%): " << *relativehumidity << endl;
}


void watertemp_setdebug__(char* Debug)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->debug = *Debug;

   if ((ptr->debug =='Y') || (ptr->debug == 'y')) {
      cout << endl << "  TWaterTemperature address: " << ptr << endl;
      cout << "TWO: Debug: " << *Debug << endl;
   }
}

void watertemp_setdepth__(int* box, float* depth)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetBoxDepth(*box, *depth);

   //if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Depth: " << *depth << endl;
}

void watertemp_setjulianday__(float* julianday)
{
   TWaterTemperature* ptr;
   int jd = *julianday;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetJulianDay(jd);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Julian day: " << jd << endl;
}

void watertemp_setcurrenttime__(float* currenttime)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetCurrentTime(*currenttime);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Current time: " << *currenttime << endl;
}
void watertemp_setairtemperature__(float* airtemperature)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetAirTemperature(*airtemperature);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Air temperature: " << *airtemperature << endl;
}

void watertemp_setsalinity__(float* salinity)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetSalinity(*salinity);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Salinity: " << *salinity << endl;
}

void watertemp_setwaterdensity__(float* waterdensity)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetWaterDensity(*waterdensity);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Water Density: " << *waterdensity << endl;
}

void watertemp_settidalheight__(float* height)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetTidalHeight(*height);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Tidal Height: " << *height << endl;
}

void watertemp_settotalsurfaceirradiance__(float* totalir)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetTotalIR(*totalir);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Total surface irradiance: " << *totalir << endl;
}

void watertemp_setsubsurfaceirradiance__(float* subir)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetSubIR(*subir);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Sub-surface irradiance: " << *subir << endl;
}

void watertemp_setatmosphericir__(float* atmosphericir)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetAtmosphericIR(*atmosphericir);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Atmospheric IR: " << *atmosphericir << endl;
}

void watertemp_setwindspeed__(float* speed)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetWindSpeed(*speed);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Wind speed: " << *speed << endl;
}

void watertemp_setaveragewatertemperature__(int* box, float* avtemperature)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetAverageWaterTemperature(*box, *avtemperature);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Average Water Temperature: " << *avtemperature << endl;
}

void watertemp_setrivertemperature__(float* rivertemperature)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetRiverTemperature(*rivertemperature);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: River Temperature: " << *rivertemperature << endl;
}

void watertemp_setrelativehumidity__(float* humidity)
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->SetRelativeHumidity(*humidity);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "TWO: Relative Humidity: " << *humidity << endl;
}

void watertemp_go__()
{
   TWaterTemperature* ptr;
   ptr = TWaterTemperature::getWaterTemperature();
   ptr->Go();
}

//-----------------------------------------------
// ...Constructors adapted to Fortran...
//
//        only one box
//
//-----------------------------------------------

TWaterTemperature* TWaterTemperature::PWaterTemperature = 0;

/*
 * This constructor calls Go() after initialisation
 */
void watertemp_(int* box, float* depth,
            float* currenttime, float* julianday, float* airtemperature,
            float* salinity, float* waterdensity,
            float* totalir, float* subir,
            float* atmosphericir, float* speed, float* avtemperature,
            float* rivertemperature, float* humidity, float* timeStep)
{
   TWaterTemperature* ptr;
   int jd = *julianday;
   ptr = TWaterTemperature::getWaterTemperature();

   ptr->SetBoxDepth(*box, *depth);
   ptr->SetCurrentTime(*currenttime);
   ptr->SetJulianDay(jd);
   ptr->SetAirTemperature(*airtemperature);
   ptr->SetSalinity(*salinity);
   ptr->SetWaterDensity(*waterdensity);
   ptr->SetTotalIR(*totalir);
   ptr->SetSubIR(*subir);
   ptr->SetAtmosphericIR(*atmosphericir);
   ptr->SetWindSpeed(*speed);
   ptr->SetAverageWaterTemperature(*box, *avtemperature);
   ptr->SetRiverTemperature(*rivertemperature);
   ptr->SetRelativeHumidity(*humidity);
   ptr->SetTimeStep(*timeStep);
   ptr->Go();
}

/*
 * singleton provider - TWaterTemperature class method
 */
TWaterTemperature* TWaterTemperature::getWaterTemperature()
{
   if (PWaterTemperature == 0) {
      PWaterTemperature = new TWaterTemperature();
      PWaterTemperature->debug = 'N';      
   }
   return PWaterTemperature;
}


TWaterTemperature::TWaterTemperature():TEcoDynClass()
{
   MyPEcoDynClass = (TEcoDynClass*)this;
   PreBuildWaterTemperature("TWaterTemperature");

   NumberOfVariables = 3;
   VariableNameArray = new VNA[NumberOfVariables];

   strcpy(VariableNameArray[0], "Water temperature");
   strcpy(VariableNameArray[1], "River temperature");
   strcpy(VariableNameArray[2], "Relative humidity");
}

void TWaterTemperature::PreBuildWaterTemperature(char* className)
{
    strcpy(EcoDynClassName, className);
	// Get array size
	NumberOfBoxes = 1; // Size of array - i.e. number of model boxes
    NumberOfLines = 1;
    NumberOfColumns = 1;

    GenericRiver = 0.0;
    GenericOcean = 0.0;
	// Zero variables
	MyWaterDensity = 1026.75; // seawater at 35psu and 10oC
    RelativeHumidity = 1.0;  //range 0 - 1

	MixedLayerDepth = 0;
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variables
	{
		AverageWaterTemperature  = new double[NumberOfBoxes];
		WaterIrradiance   		 = new double[NumberOfBoxes];
		LatentHeatFlux   	     = new double[NumberOfBoxes];
		SensibleHeatFlux 	     = new double[NumberOfBoxes];
		NetHeatFlux   		     = new double[NumberOfBoxes];

		// Fluxes and conversions
        BoxArray = MyPEcoDynClass->BoxArray;
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			AverageWaterTemperature[i] = 0;
			WaterIrradiance[i]         = 0;
			LatentHeatFlux[i]          = 0;
			SensibleHeatFlux[i]        = 0;
			NetHeatFlux[i]             = 0;
		}
	}
	// Get forcing function file name
	VariableCount = 0;
	//TimeStep = 5;
	TimeBefore = -1;
	JulianDay = 266;
	CurrentTime = 12;
	ObjectCode = TWATOBJECTCODE;

    // default values
    sal = 37;
    MixedLayerDepth = 15;
    waterD = 1027.29;
    tidalH = 0;
}
//-----------------------------------------------

#else  // _PORT_FORTRAN_

//
// Constructors invoked outside EcoDyn shell...
//

TWaterTemperature::TWaterTemperature(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity)
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    PreBuildWaterTemperature(className);

	// Read in the variables
	NumberOfVariables = nVars;
    // Create array for state variable names
	VariableNameArray = new VNA[NumberOfVariables];
	// read in the variable names
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
    {
		AverageWaterTemperature[i] = aAverageTemperature[i];
    }
    GenericRiver = genericRiver;
    RelativeHumidity = relativeHumidity;
}

TWaterTemperature::TWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    strcpy(VarsFileName, variablesFilename);
    if (OpenMorphology()) {
        BuildWaterTemperature(className);
    }
}
// ...

TWaterTemperature::TWaterTemperature(TEcoDynClass* APEcodynClass, char* className): TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcodynClass;
    NumberOfSubDomains = 1;
    BuildWaterTemperature(className);
}

void TWaterTemperature::PreBuildWaterTemperature(char* className)
{
    strcpy(EcoDynClassName, className);
	// Get array size
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
    GenericRiver = 0.0;
    GenericOcean = 0.0;
    // Zero variables
    MyWaterDensity = 1026.75; // seawater at 35psu and 10oC
    RelativeHumidity = 1.0;  //range 0 - 1
    MixedLayerDepth = 0;
    // Initialise pointers
    if (NumberOfBoxes > 0)                       // Initialise arrays for variables
    {
        AverageWaterTemperature  = new double[NumberOfBoxes];
        WaterIrradiance = new double[NumberOfBoxes];
        LatentHeatFlux = new double[NumberOfBoxes];
        SensibleHeatFlux = new double[NumberOfBoxes];
        NetHeatFlux = new double[NumberOfBoxes];
        // Fluxes and conversions
        BoxArray = MyPEcoDynClass->BoxArray;
        for (int i = 0; i < NumberOfBoxes; i++)
        {
            AverageWaterTemperature[i] = 0;
            WaterIrradiance[i]         = 0;
            LatentHeatFlux[i]          = 0;
            SensibleHeatFlux[i]        = 0;
            NetHeatFlux[i]             = 0;
        }
    }
    // Get forcing function file name
    VariableCount = 0;
    TimeStep = MyPEcoDynClass->GetTimeStep();
    TimeBefore = MyPEcoDynClass->GetTimeBefore();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    CurrentTime = MyPEcoDynClass->GetCurrTime();
    ObjectCode = TWATOBJECTCODE;

    NumberOfMomentsForTimeSeries = 1;
    NumberOfDaysForTimeSeries = 0;
    NumberOfHoursForTimeSeries = 0;
    NumberOfDaysForLoads = 0;
    NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
    NumberOfRiverLoads = 0;
    NumberOfRiverLoads = MyPEcoDynClass->GetNumberOfRivers();
    LatentFluxReductionFactor = 1.0;
    //Debugger(NumberOfRiverLoads);
}

void TWaterTemperature::BuildWaterTemperature(char* className)
{
    PreBuildWaterTemperature(className);

	int X,Y, XW, YW;
	int i;

	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("WaterTemperature");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "WaterTemperature parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TWaterTemperature::BuildWaterTemperature - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
	else
    {
        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
                if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfDaysForTimeSeries);
                    NumberOfMomentsForTimeSeries = NumberOfDaysForTimeSeries;
                    //Debugger(NumberOfDaysForTimeSeries);
                }
                else
                if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfHoursForTimeSeries);
                    NumberOfMomentsForTimeSeries = NumberOfHoursForTimeSeries;
                    //Debugger(NumberOfHoursForTimeSeries);
                }
                if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfDaysForLoads);
                    //NumberOfMomentsForLoads = NumberOfDaysForLoads;
                    //Debugger(NumberOfDaysForLoads);
                }
                if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfRiverLoads);
                    //Debugger(NumberOfRiverLoads);
                }
                if (strcmp(MyParameter, "LatentFluxReductionFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, LatentFluxReductionFactor);
                    //Debugger(LatentFluxReductionFactor);
                }
                if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfSubDomains); //Debugger(NumberOfSubDomains);
                }
            }
            RelativeHumiditySubDomains = new SubDomain[NumberOfSubDomains];
            for (int j = 0; j < NumberOfSubDomains; j++)
            {
                RelativeHumiditySubDomains[j].FirstLine = 0;                                         //Southern limit
                RelativeHumiditySubDomains[j].LastLine = MyPEcoDynClass->GetNumberOfLines() - 1;     //Northern limit
                RelativeHumiditySubDomains[j].FirstColumn = 0;                                       //Western limit
                RelativeHumiditySubDomains[j].LastColumn = MyPEcoDynClass->GetNumberOfColumns() - 1; //Eastern limit
            }
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
                if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                {
                    int AValue, index = 0;
                    for (int j = i; j < i + NumberOfSubDomains; j++)
                    {
                       //Debugger(j);
                       PReadWrite->ReadNumber(X+4, j, AValue);
                       RelativeHumiditySubDomains[index].FirstLine = NumberOfLines - AValue; //Debugger(AValue);//Southern limit
                       PReadWrite->ReadNumber(X+5, j, AValue);
                       RelativeHumiditySubDomains[index].LastLine = NumberOfLines - AValue; //Debugger(AValue); //Northern limit
                       PReadWrite->ReadNumber(X+6, j, AValue);
                       RelativeHumiditySubDomains[index].FirstColumn = AValue - 1;          //Debugger(AValue); //Western limit
                       PReadWrite->ReadNumber(X+7, j, AValue);
                       RelativeHumiditySubDomains[index].LastColumn = AValue - 1;           //Debugger(AValue); //Eastern limit
                       //Debugger(index);
                       //Debugger(RelativeHumiditySubDomains[index].FirstLine); Debugger(RelativeHumiditySubDomains[index].LastLine);
                       //Debugger(RelativeHumiditySubDomains[index].FirstColumn); Debugger(RelativeHumiditySubDomains[index].LastColumn);
                       index++;
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object WaterTemperature parameters", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWaterTemperature::BuildWaterTemperature - parameters undefined object." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    RelativeHumidityTimeSeries = new double[NumberOfMomentsForTimeSeries*NumberOfSubDomains];
    for (int i = 0; i < NumberOfMomentsForTimeSeries; i++)
    {
       RelativeHumidityTimeSeries[i] = 0.0;
    }

	// Read in the variables
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "WaterTemperature variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TWaterTemperature::BuildWaterTemperature - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else
    {
        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // Read the water temperature values
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);

                if (strcmp(MyVariable, "Water temperature") == 0)
                {
                    if (!PReadWrite->FindString("Water temperature values", XW, YW))
                    {
                        XW = X + 3;
                        YW = i - 1;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XW, YW + 1 + j, AverageWaterTemperature[j]);
                    }
                }
                else if (strcmp(MyVariable, "River temperature") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                }
                else if (strcmp(MyVariable, "Relative humidity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RelativeHumidity);
                }
                else if (strcmp(MyVariable, "Relative humidity time series") == 0)
                {
                    int index;
                    if (!PReadWrite->FindString("Relative humidity time series values", XW, YW))
                    {
                        XW = X + 3;
                        YW = i - 1;
                    }
                    /*for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XW, YW + 1 + j, RelativeHumidityTimeSeries[j]);
                    } */
                    //Debugger(NumberOfSubDomains);Debugger(NumberOfMomentsForTimeSeries);
                    for (int i = 0; i < NumberOfSubDomains; i++)
                    {
                        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                        {
                            index = i * NumberOfMomentsForTimeSeries + j;
                            PReadWrite->ReadNumber(XW + i, YW + 1 + j, RelativeHumidityTimeSeries[index]);
                            /*if ((index == 0) || ((i == NumberOfSubDomains-1) && (j == NumberOfMomentsForTimeSeries-1)))
                               Debugger(RelativeHumidityTimeSeries[index]); */
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object WaterTemperature Variables", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWaterTemperature::BuildWaterTemperature - variables undefined object." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}
#endif  // _PORT_FORTRAN_

TWaterTemperature::~TWaterTemperature()
{
	freeMemory();
}

void TWaterTemperature::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] AverageWaterTemperature;
            delete [] WaterIrradiance;
            delete [] LatentHeatFlux;
            delete [] SensibleHeatFlux;
            delete [] NetHeatFlux;
	        delete [] RelativeHumidityTimeSeries;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            delete [] RelativeHumiditySubDomains;
            NumberOfParameters = 0;
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
				cerr << "TWaterTemperature::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TWaterTemperature::Go()
{
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
	GetWaterTemperature();
}

void TWaterTemperature::Integrate()
{
   // Temporary
   #ifndef _PORT_FORTRAN_
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyTransportPointer != NULL)
   {
      MyTransportPointer->Go(AverageWaterTemperature, NULL, GenericRiver, GenericOcean);
   }
   #endif  // _PORT_FORTRAN_
}

void TWaterTemperature::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Water temperature") == 0)
		Value = AverageWaterTemperature[MyBoxNumber];
	else if (strcmp(MyParameter, "Water irradiance") == 0)
		Value = WaterIrradiance[MyBoxNumber];
	else if (strcmp(MyParameter, "Net heat flux") == 0)
		Value = NetHeatFlux[MyBoxNumber];
	else if (strcmp(MyParameter, "Latent heat flux") == 0)
		Value = LatentHeatFlux[MyBoxNumber];
	else if (strcmp(MyParameter, "Sensible heat flux") == 0)
		Value = SensibleHeatFlux[MyBoxNumber];
    else if (strcmp(MyParameter, "Relative humidity") == 0)
	    Value = RelativeHumidity;
    else if (strcmp(MyParameter, "Relative humidity time series") == 0)
    {
        int ADay, AHour, AMoment;
        if  (NumberOfDaysForTimeSeries >= 1)
        {
           ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForTimeSeries - 1);
           AMoment = ADay;
        }
        else
        if (NumberOfHoursForTimeSeries >= 1)
        {
           ADay = MyPEcoDynClass->GetJulianDay() - 1;
           AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
           AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
        }
        else
#ifdef __BORLANDC__
           MessageBox(0, "TWaterTemperature::Inquiry", "Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries", MB_OK);
#else  // __BORLANDC__
					 cerr << "TWaterTemperature::Inquiry - Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries" << endl;
#endif  // __BORLANDC__
        Value = RelativeHumidityTimeSeries[AMoment];
    }
    else
        Value = 0;

#ifndef _PORT_FORTRAN_
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
#endif
}

bool TWaterTemperature::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

#ifndef _PORT_FORTRAN_
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);
#endif  // _PORT_FORTRAN_

    if (strcmp(VariableName, "Water temperature") == 0)
        AverageWaterTemperature[BoxNumber] = Value;
    else if (strcmp(VariableName, "Water irradiance") == 0)
        WaterIrradiance[BoxNumber] = Value;
    else if (strcmp(VariableName, "Net heat flux") == 0)
        NetHeatFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "Latent heat flux") == 0)
        LatentHeatFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "Sensible heat flux") == 0)
        SensibleHeatFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "Relative humidity") == 0)
		RelativeHumidity = Value;
    else
        rc = false;

    return rc;
}

bool TWaterTemperature::SaveVariables()
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
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Water temperature values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(AverageWaterTemperature[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Relative humidity time series values");

    if (NumberOfMomentsForTimeSeries > 0) {
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
        {
            PReadWrite->WriteCell(RelativeHumidityTimeSeries[j], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

// Protected methods

void TWaterTemperature::GetWaterTemperature()
{
	const double WaterSpecificHeat = 4100; // J/kg/k

	double MySalinity,
			 MySunIrradiance,
			 MyLightAtTop,
			 MyLightAtBottom,
			 ATop,
			 ABottom,
			 MyAtmosphericIR,
			 MySVPAtWaterTemperature,
			 MySVPAtAirTemperature,
			 MyAirTemperature,
			 MyTemperatureVariation,
			 MyWindSpeed,
          MyTidalHeight;

	TEcoDynClass* MyTAirTemperaturePointer = MyPEcoDynClass->GetAirTemperaturePointer();
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
        TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
#ifndef _PORT_FORTRAN_
		if (MyTAirTemperaturePointer != NULL)
			MyTAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature, i,"Air temperature",ObjectCode);
		else
		{
                   //MessageBox(0, "Air temperature object is undefined", "EcoDynamo alert", MB_OK);
                   return;
		}

		if (MyTransportPointer != NULL)
		{
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,i,"Salinity",ObjectCode);
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,i,"Water density",ObjectCode);
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,i,"Box depth",ObjectCode);
		}
		else
		{
                    MySalinity = 37;      // local variable
                    MixedLayerDepth = 15; // object variable
                    MyWaterDensity = 1027.29; // seawater at 35psu and 10oC
                    ABottom = MyPEcoDynClass->GetBoxDepth(i);
		}
                if ((MyTidePointer != NULL) && (i == 0))        //Aten??o!!! Se a profundidade ? devolvida pelo objecto de transporte
                {                                               //ent?o n?o faz sentido somar-lhe a altura da mar?
                    MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,    //Assim, este "void" n?o deve ser usado nos casos em que a profundidade
                                              i,                 //j? vier corrigida pela mar? do objecto de transporte
                                              "Tidal height",
                                              ObjectCode);
                    ABottom = ABottom + MyTidalHeight / CUBIC; //Convert from mm to m
                }

		if (MyLightPointer != NULL)
		{
                    MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,i,"Total surface irradiance",ObjectCode);
                    MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,ABottom,i,"Sub-surface irradiance",ObjectCode);
                    MyLightPointer->Inquiry(GetEcoDynClassName(), MyAtmosphericIR,i,"Atmospheric IR",ObjectCode);
		}
		else
		{
			//MessageBox(0, "Light object is undefined", "EcoDynamo alert", MB_OK);
			return;
		}

		if (MyWindPointer != NULL)
			MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
												 i,
												 "Wind speed",
												 ObjectCode);
		else
		{
			//MessageBox(0, "Wind object is undefined", "EcoDynamo alert", MB_OK);
			return;
		}
#else  // _PORT_FORTRAN_
        MySalinity = sal;
        MyLightAtTop = totalI;
        MyLightAtBottom = subI;
        MyTidalHeight = tidalH;
        ABottom = MyPEcoDynClass->GetBoxDepth(i);
        if (MyTidalHeight != 0)
            ABottom = ABottom + MyTidalHeight / CUBIC; //Convert from mm to m
        MyAtmosphericIR = atIR;
        MyAirTemperature = airT;
        MyWindSpeed = windS;
        MyWaterDensity = waterD;
#endif // _PORT_FORTRAN_

      WaterIrradiance[i] = GetWaterSurfaceRadiation(AverageWaterTemperature[i]);

      MySVPAtWaterTemperature = GetSaturationVapourPressure(AverageWaterTemperature[i]);

      MySVPAtAirTemperature = GetSaturationVapourPressure(MyAirTemperature);

      LatentHeatFlux[i] = GetLatentWaterHeatFlux(MySVPAtWaterTemperature,
																 MySVPAtAirTemperature,
																 MyWindSpeed);

      SensibleHeatFlux[i] = GetSensibleWaterHeatFlux(AverageWaterTemperature[i],
																	  MyAirTemperature,
																	  MyWindSpeed);

      NetHeatFlux[i] = MyLightAtTop
							  - MyLightAtBottom
							  + MyAtmosphericIR
							  + WaterIrradiance[i]
							  + LatentHeatFlux[i]
							  + SensibleHeatFlux[i];

      if ((MyWaterDensity* WaterSpecificHeat* ABottom) == 0.0)
      {
#ifndef _PORT_FORTRAN_
      	Debugger(MyWaterDensity); Debugger(WaterSpecificHeat); Debugger(ABottom);
#endif  // _PORT_FORTRAN_
      }
		MyTemperatureVariation = NetHeatFlux[i]
										 * TimeStep               // Is already in days
										 / (MyWaterDensity
											 * WaterSpecificHeat
											 * ABottom);

		AverageWaterTemperature[i] = AverageWaterTemperature[i]
											  + MyTemperatureVariation;

	}
}

double TWaterTemperature::GetWaterSurfaceRadiation(double AWaterTemperature)
{
	const double Sigma = 0.0000000567, // 5.67e-8, // Stefan-Boltzman constant
					 WaterEmissivity = 0.97;
	double MyWaterTemperature = AWaterTemperature;

	return -WaterEmissivity
			 * Sigma
			 * sqr ( sqr ( ABSOLUTEZERO + MyWaterTemperature ) );
}

double TWaterTemperature::GetLatentWaterHeatFlux(double Aesw,double Aesa,double AWindSpeed)
{
	const double a = 0.075, // W/m2/Pa
					 b = 0.030; // J/m3/Pa

	double esw = Aesw,
			 esa = Aesa,
			 MyWindSpeed = AWindSpeed,
			 x;
	x = ( esw - esa * RelativeHumidity );
	if (x > 0)
		return -( a + b * MyWindSpeed) * x * LatentFluxReductionFactor;
	else
		return 0;
}


/*double TWaterTemperature::GetLatentWaterHeatFluxRHTimeSeries(double Aesw,double Aesa,double AWindSpeed)
{
	const double a = 0.075, // W/m2/Pa
					 b = 0.030; // J/m3/Pa

	double esw = Aesw,
			 esa = Aesa,
			 MyWindSpeed = AWindSpeed,
			 x;
    int ADay, AHour, AMoment;

    if  (NumberOfDaysForTimeSeries >= 1)
    {
       ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForTimeSeries - 1);
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForTimeSeries >= 1)
    {
       ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
    }
    else
       MessageBox(0, "TWaterTemperature::GetLatentWaterHeatFluxRHTimeSeries", "Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries", MB_OK);

	x = ( esw - esa * RelativeHumidityTimeSeries[AMoment] );

	if (x > 0)
		return -( a + b * MyWindSpeed) * x;
	else
		return 0;
}  */

double TWaterTemperature::GetSaturationVapourPressure(double ATemperature)
{
	double MyTemperature = ATemperature;
	return 612
			 *exp ( 17.5 * MyTemperature / ( 241 + MyTemperature ) );
}

double TWaterTemperature::GetSensibleWaterHeatFlux(double AWaterTemperature,
																	double AnAirTemperature,
																	double AWindSpeed)
{
	const double a = 0.075,     // W/m2/Pa
					 b = 0.030,     // J/m3/Pa
					 gamma = 62.7;  // Pa/k - Bowen constant
	double MyWaterTemperature = AWaterTemperature,
			 MyAirTemperature = AnAirTemperature,
			 MyWindSpeed = AWindSpeed;

	return -gamma
			 *( a + b * MyWindSpeed)
			 * (MyWaterTemperature- MyAirTemperature);
}

// AP, 2008.01.16
double TWaterTemperature::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
    {
        value = NumberOfDaysForTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
    {
        value = NumberOfHoursForTimeSeries;
    }
    else
        value = 0.0;
    return value;
}

bool TWaterTemperature::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
    {
        NumberOfDaysForTimeSeries = value;
        NumberOfMomentsForTimeSeries = NumberOfDaysForTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
    {
        NumberOfHoursForTimeSeries = value;
        NumberOfMomentsForTimeSeries = NumberOfHoursForTimeSeries;
    }
    else
        rc = false;

    return rc;
}


bool TWaterTemperature::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("WaterTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Water temperature");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();

    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "NumberOfDaysForTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForTimeSeries, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfHoursForTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfHoursForTimeSeries, 0);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

void TWaterTemperature::Reinitialize()
{
   int ADay, AHour, AMoment;
    RelativeHumidity = 0.0;
    if  (NumberOfDaysForTimeSeries > 1)
    {
       ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForTimeSeries - 1);
       AMoment = ADay;
       RelativeHumidity = RelativeHumidityTimeSeries[AMoment];
    }
    else
    if (NumberOfHoursForTimeSeries > 1)
    {
       ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
       RelativeHumidity = RelativeHumidityTimeSeries[AMoment];
    }
}


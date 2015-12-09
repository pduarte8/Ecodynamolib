// Dissolved object CPP code file
// @ J. Gomes Ferreira
// Rev. A 96.01.22
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>
	#include <cmath> 
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "params.h"
#include "EcoDynClass.h"


// TNutrients Class
//
// Constructors invoked outside EcoDyn shell...

#ifndef _PORT_FORTRAN_
TNutrients::TNutrients(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildNutrients(className);
}

TNutrients::TNutrients(char* className, float timeStep, char* morphologyFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology()) {
        BuildNutrients(className);
    }
}


TNutrients::TNutrients(TEcoDynClass* APEcoDynClass, char* className)
								 : TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcoDynClass;
    BuildNutrients(className);
    JulianDay = MyPEcoDynClass->GetJulianDay();
    NumberOfDaysForBoundaryTimeSeries = 365;
}
#endif

#ifdef _PORT_FORTRAN_
TNutrients::TNutrients(char* className)
{
   BuildNutrients(className);
}
#endif

void TNutrients::BuildNutrients(char* className)
{
    int i,j;
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
#ifndef _PORT_FORTRAN_
    AStartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    ATime = AStartTime;
    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
#endif
    // Get array size
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    //Debugger(NumberOfBoxes);
#ifndef _PORT_FORTRAN_
    TimeStep = MyPEcoDynClass->GetTimeStep();
#endif
    ObjectCode = NUTROBJECTCODE;

    // Initialise pointers
    if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
    {                                            // for boxes and one for loads into boxes
	/* NPI-Work, 03 Oct 2013, AP - memory allocated and cleared here **
	 ***********
	 */
				Ammonia   = (double*) calloc (NumberOfBoxes, sizeof(double)); 	     // Nitrogen
        NH4Load   = (double*) calloc (NumberOfBoxes, sizeof(double));
        NH4Flux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        Nitrite   = (double*) calloc (NumberOfBoxes, sizeof(double));
        NO2Load   = (double*) calloc (NumberOfBoxes, sizeof(double));
        NO2Flux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        Nitrate   = (double*) calloc (NumberOfBoxes, sizeof(double));
        NO3Load   = (double*) calloc (NumberOfBoxes, sizeof(double));
        NO3Flux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        Phosphate = (double*) calloc (NumberOfBoxes, sizeof(double));       // Phosphorus
        PO4Load   = (double*) calloc (NumberOfBoxes, sizeof(double));
        PO4Flux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        Silica    = (double*) calloc (NumberOfBoxes, sizeof(double));       // Silica
        SiO2Load  = (double*) calloc (NumberOfBoxes, sizeof(double));
        SiO2Flux  = (double*) calloc (NumberOfBoxes, sizeof(double));
        DON       = (double*) calloc (NumberOfBoxes, sizeof(double));
        DONLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
        DONFlux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOP       = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOPLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOPFlux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOC       = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOCLoad   = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOCFlux   = (double*) calloc (NumberOfBoxes, sizeof(double));
        NH4RiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        NO2RiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        NO3RiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        DONRiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOPRiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        PO4RiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        SiO2RiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        DOCRiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        O2RiverLoad = (double*) calloc (NumberOfBoxes, sizeof(double));
        O2Load = (double*) calloc (NumberOfBoxes, sizeof(double));

 		/**************/
    }
    else
#ifdef __BORLANDC__
				MessageBox(0,"Dissolved object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TNutrients::BuildNutrients - Dissolved object array not dimensioned" << endl;
#endif  // __BORLANDC__
}

TNutrients::~TNutrients()
{
    freeMemory();
}

void TNutrients::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
	/* NPI-Work, 03 Oct 2013, AP - free memory replaces delete [] **
	 ***********
	 */
            free(Ammonia);
            free(NH4Load);
            free(NH4Flux);
            free(Nitrite);
            free(NO2Load);
            free(NO2Flux);
            free(Nitrate);
            free(NO3Load);
            free(NO3Flux);
            free(Phosphate);
            free(PO4Load);
            free(PO4Flux);
            free(Silica);
            free(SiO2Load);
            free(SiO2Flux);
            free(DON);
            free(DONLoad);
            free(DONFlux);
            free(DOP);
            free(DOPLoad);
            free(DOPFlux);
            free(DOC);
            free(DOCLoad);
            free(DOCFlux);
            free(NH4RiverLoad);
            free(NO2RiverLoad);
            free(NO3RiverLoad);
            free(DONRiverLoad);
            free(DOPRiverLoad);
            free(PO4RiverLoad);
            free(SiO2RiverLoad);
            free(DOCRiverLoad);
            free(O2RiverLoad);
            free(O2Load);
		/**********/
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
				cerr << "TNutrients::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TNutrients::Go()
{
	// Active methods
	Nitrification();
}

void TNutrients::Inquiry(char* srcName, double &Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Ammonia") == 0)
		Value = Ammonia[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "Nitrite") == 0)
		Value = Nitrite[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "Nitrate") == 0)
		Value = Nitrate[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "Phosphate") == 0)
		Value = Phosphate[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "Silica") == 0)
		Value = Silica[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "DIN") == 0)
		Value = ( Ammonia[MyBoxNumber]
				  + Nitrite[MyBoxNumber]
				  + Nitrate[MyBoxNumber]
				) / CUBIC;
    else if (strcmp(MyParameter, "Net DIN Flux") == 0)
		Value = 0;
    else if (strcmp(MyParameter, "Redfield ratio (N/P)") == 0)
		Value = ( Ammonia[MyBoxNumber]
				  + Nitrite[MyBoxNumber]
				  + Nitrate[MyBoxNumber]
				) / Phosphate[MyBoxNumber];

    else if (strcmp(MyParameter, "DON") == 0)
		Value = DON[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "DOP") == 0)
		Value = DON[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "DOC") == 0)
        Value = DOC[MyBoxNumber] /CUBIC;
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
                      "EcoDynamo Alert - Inquiry 1",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TNutrients::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TNutrients::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Ammonia") == 0)
        Ammonia[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Nitrate") == 0)
        Nitrate[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Nitrite") == 0)
        Nitrite[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Phosphate") == 0)
        Phosphate[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Silica") == 0)
        Silica[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DON") == 0)
		DON[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DOP") == 0)
		DOP[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DOC") == 0)
        DOC[BoxNumber] = Value * CUBIC;
    else
        rc = false;

    return rc;
}

void TNutrients::Inquiry(char* srcName, double &Value,
									  int BoxX,
									  int BoxY,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxX * NumberOfBoxes + BoxY;
	strcpy(MyParameter, ParameterName);

    // All exchanges in umol m-3, converted to kg mass flux
    if (strcmp(MyParameter, "DIN advective flux matrix") == 0)
	{
        Value = DINInterfaceExchange[MyBoxNumber].AdvectiveFlow // in kg
									  * TimeStep
									  * DAYSTOSECONDS
									  * NITROGENATOMICWEIGHT
									  / (CUBIC * CUBIC * CUBIC);
	}
	else if (strcmp(MyParameter, "DIN dispersive flux matrix") == 0)
	{
        Value = DINInterfaceExchange[MyBoxNumber].DispersiveFlow // in kg
									  * TimeStep
									  * DAYSTOSECONDS
									  * NITROGENATOMICWEIGHT
									  / (CUBIC * CUBIC * CUBIC);
    }
	else if (strcmp(MyParameter, "DIN net flux matrix") == 0)
    {
        Value = DINInterfaceExchange[MyBoxNumber].InFlow // made into net flow in kg
									  * TimeStep
									  * DAYSTOSECONDS
									  * NITROGENATOMICWEIGHT
									  / (CUBIC * CUBIC * CUBIC);
    }
    else
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
            Value = 0;
#else  // __BORLANDC__
			cerr << "TNutrients::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


void TNutrients::Update(char* srcName, double Value,
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
    else
    {
#ifdef __BORLANDC__
	    char Caption[129];
	    strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 1",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TNutrients::Update 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}


void TNutrients::Integrate()
{
/*	// Integrate ammonia within the box
	Generic = Ammonia; GenericFlux = NH4Flux;
	Integration();
	Ammonia = Generic; NH4Flux = GenericFlux;

	// Integrate nitrite within the box
	Generic = Nitrite; GenericFlux = NO2Flux;
	Integration();
	Nitrite = Generic; NO2Flux = GenericFlux;

	// Integrate nitrate within the box
	Generic = Nitrate; GenericFlux = NO3Flux;
	Integration();
	Nitrate = Generic; NO3Flux = GenericFlux;

	// Integrate phosphate within the box
	Generic = Phosphate; GenericFlux = PO4Flux;
	Integration();
	Phosphate = Generic; PO4Flux = GenericFlux;

	// Integrate silica within the box
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
   	    if (NO3Load[i] < 0.0) NO3Load[i] = Nitrate[i];
        if (NO2Load[i] < 0.0) NO2Load[i] = Nitrite[i];
        if (NH4Load[i] < 0.0) NH4Load[i] = Ammonia[i];
        if (PO4Load[i] < 0.0) PO4Load[i] = Phosphate[i];
        if (SiO2Load[i] < 0.0) SiO2Load[i] = Silica[i];
    }
	// Transport the nutrients
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{

		// New calls for boundary exchange

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

// Protected methods
void TNutrients::RedfieldRatio()
{
}

void TNutrients::DINProportion(int ABoxNumber)
{
	double TotalDIN;
	TotalDIN = ( Ammonia[ABoxNumber]
					 + Nitrite[ABoxNumber]
					 + Nitrate[ABoxNumber] );

	if (TotalDIN > 0)
	{
		NH4_TO_DIN = Ammonia[ABoxNumber] / TotalDIN;
		NO2_TO_DIN = Nitrite[ABoxNumber] / TotalDIN;
		NO3_TO_DIN = Nitrate[ABoxNumber] / TotalDIN;
	}
	else
	{
		NH4_TO_DIN = 0;
		NO2_TO_DIN = 0;
		NO3_TO_DIN = 0;
	}
}

void TNutrients::Nitrification()
{
	// Fixed terms for nitrification
	const double NH4_TO_NO2 = 0.1 / DAYSTOSECONDS; //s-1
	const double NO2_TO_NO3 = 0.1 / DAYSTOSECONDS;

	double TempNH4, TempNO2, TempNO3;

	// Is this finally correct? I think so - 97.07.23 - JGF
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		TempNH4 = Ammonia[i] * -NH4_TO_NO2;
		TempNO2 = Ammonia[i] *  NH4_TO_NO2 + Nitrite[i] *  -NO2_TO_NO3;
		TempNO3 = Nitrite[i] *  NO2_TO_NO3;

		NH4Flux[i] = NH4Flux[i]+ TempNH4;
		NO2Flux[i] = NO2Flux[i]+ TempNO2;
		NO3Flux[i] = NO3Flux[i]+ TempNO3;
	}
}




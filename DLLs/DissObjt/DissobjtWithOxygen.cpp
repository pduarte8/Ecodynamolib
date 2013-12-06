// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 01.04.27
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"

// TNutrientsWithOxygen Class
//
// Constructors invoked outside EcoDyn shell...
//

TNutrientsWithOxygen::TNutrientsWithOxygen(char* className,
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
        double riverOxygen, double oceanOxygen, double oxygen[])
    :TNutrientsBasic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, riverAmmonia, oceanAmmonia, aAmmonia, aNH4Load,
                riverNitrite, oceanNitrite, aNitrite, aNO2Load,
                riverNitrate, oceanNitrate, aNitrate, aNO3Load,
                riverPhosphate, oceanPhosphate, aPhosphate, aPO4Load,
                riverSilica, oceanSilica, aSilica, aSiO2Load,
                riverDON, oceanDON, aDON, aDONLoad,
                riverDOP, oceanDOP, aDOP, aDOPLoad,
                riverDOC, oceanDOC, aDOC, aDOCLoad)
{
    PreBuildNutrientsWithOxygen();
    RiverOxygen = riverOxygen;
    OceanOxygen = oceanOxygen;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        Oxygen[i] = oxygen[i];
    }
}

TNutrientsWithOxygen::TNutrientsWithOxygen(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename)
            :TNutrientsBasic(className, timeStep, morphologyFilename, variablesFilename)
{
    BuildNutrientsWithOxygen();
}
// ...

TNutrientsWithOxygen::TNutrientsWithOxygen(TEcoDynClass* APEcoDynClass,
        char* className) : TNutrientsBasic (APEcoDynClass, className)
{
    BuildNutrientsWithOxygen();
}

void TNutrientsWithOxygen::PreBuildNutrientsWithOxygen()
{
	VariableCount = 0;
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		Oxygen = new double[NumberOfBoxes]; 	             // Oxygen
		OxygenLoad = new double[NumberOfBoxes];
		OxygenSaturation = new double[NumberOfBoxes];
		OxygenFlux = new double[NumberOfBoxes];

		// Fluxes and conversions
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			Oxygen[i] = 0;
			OxygenFlux[i] = 0;
			OxygenLoad[i] = 0;
			OxygenSaturation[i] = 0;
		}
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Dissolved object with oxygen array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TNutrientsWithOxygen::PreBuildNutrientsWithOxygen - Dissolved object with oxygen array not dimensioned" << endl;
#endif  // __BORLANDC__
}

void TNutrientsWithOxygen::BuildNutrientsWithOxygen()
{
	int i,j;
    PreBuildNutrientsWithOxygen();

	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
	if (PReadWrite != NULL)
	{
        int X,Y, XV, YV;

        if (PReadWrite->FindString("Nutrients", X, Y))
        {

            char MyVariable[65];
            for (i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                if (strcmp(MyVariable, "Oxygen") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverOxygen = GenericRiver;  // mg l-1 ou g m-3
                    OceanOxygen = GenericOcean;  // mg l-1 ou g m-3
                    if (!PReadWrite->FindString("Oxygen values", XV, YV))
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Oxygen[j]);
                        }
                    }
                    else
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Oxygen[j]);
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in nutrients with oxygen",
                    "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TNutrientsWithOxygen::BuildNutrientsWithOxygen - Variables: undefined object in nutrients with oxygen" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TNutrientsWithOxygen::~TNutrientsWithOxygen()
{
    freeMemory();
}

void TNutrientsWithOxygen::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] Oxygen;
            delete [] OxygenLoad;
            delete [] OxygenSaturation;
            delete [] OxygenFlux;
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
				cerr << "TNutrientsWithOxygen::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TNutrientsWithOxygen::Inquiry(char* srcName, double &Value,
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
	else if (strcmp(MyParameter, "Oxygen") == 0)
		Value = Oxygen[MyBoxNumber];
	else if (strcmp(MyParameter, "Oxygen saturation") == 0)
		Value = OxygenSaturation[MyBoxNumber];
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
                      "EcoDynamo Alert - Inquiry 6",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TNutrientsWithOxygen::Inquiry 6 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TNutrientsWithOxygen::Update(char* srcName, double Value,
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
	else if (strcmp(MyParameter, "Oxygen") == 0)
		OxygenFlux[MyBoxNumber] = OxygenFlux[MyBoxNumber] + Value;
    else {
#ifdef __BORLANDC__
    	char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
    	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 3",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TNutrientsWithOxygen::Update 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}

bool TNutrientsWithOxygen::SetVariableValue(char* srcName, double Value,
											  int BoxNumber,
											  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    /*
     * from TNutrientsBasic
     */
	if (strcmp(VariableName, "Ammonia") == 0)
		Ammonia[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Nitrite") == 0)
		Nitrite[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Nitrate") == 0)
		Nitrate[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Phosphate") == 0)
		Phosphate[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Silica") == 0)
		Silica[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DON") == 0)
		DON[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DOC") == 0)
		DOC[BoxNumber] = Value * CUBIC;
    /*
     * from TNutrientsWithOxygen
     */
    else if (strcmp(VariableName, "Oxygen") == 0)
		Oxygen[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TNutrientsWithOxygen::SaveVariables()
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
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Ammonia") == 0)
        {
            PReadWrite->WriteCell(RiverAmmonia / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanAmmonia / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Nitrite") == 0)
        {
            PReadWrite->WriteCell(RiverNitrite / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanNitrite / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Nitrate") == 0)
        {
            PReadWrite->WriteCell(RiverNitrate / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanNitrate / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Phosphate") == 0)
        {
            PReadWrite->WriteCell(RiverPhosphate / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanPhosphate / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Silica") == 0)
        {
            PReadWrite->WriteCell(RiverSilica / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanSilica / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "DON") == 0)
        {
            PReadWrite->WriteCell(RiverDON / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanDON / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "DOC") == 0)
        {
            PReadWrite->WriteCell(RiverDOC / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanDOC / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Oxygen") == 0)
        {
            PReadWrite->WriteCell(RiverOxygen, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanOxygen, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */

    PReadWrite->WriteCell("Ammonia values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NH4 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrite values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phosphate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Silica values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("SiO2 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DON values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DON loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOC loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Oxygen values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Ammonia[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NH4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrite[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO3Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Phosphate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PO4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Silica[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(SiO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DON[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DONLoad[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DOC[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DOCLoad[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Oxygen[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TNutrientsWithOxygen::Integrate()
{
	int i,j;
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

	// Integrate oxygen within the box
//	Generic = Oxygen; GenericFlux = OxygenFlux;
	Integration(Oxygen, OxygenFlux);
//	Oxygen = Generic; OxygenFlux = GenericFlux;

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

		MyTransportPointer->Go(Oxygen, OxygenLoad, RiverOxygen, OceanOxygen);

	}

	// recaculate new O2 saturation here based on new oxygen (after transport)
	double WaterColumnTemperature, WaterColumnSalinity;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
	WaterColumnTemperature = 12; // default if water temperature object is null
	WaterColumnSalinity = 35; // default if no transport
	for ( int i = 0; i < NumberOfBoxes; i++ )
	{
		if (MyWaterTemperaturePointer != NULL)
			MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterColumnTemperature,
																i,
																"Water temperature",
																ObjectCode);
		if (MyTransportPointer != NULL)
			MyTransportPointer->Inquiry(GetEcoDynClassName(), WaterColumnSalinity,
													  i,
													  "Salinity",
													  ObjectCode);
		// calculate saturation here
	}

	for (i = 0; i < NumberOfBoxes; i++)
	{
		if (Ammonia[i]   < 0.0)   Ammonia[i] = 0.0;
		if (Nitrite[i]   < 0.0)   Nitrite[i] = 0.0;
		if (Nitrate[i]   < 0.0)   Nitrate[i] = 0.0;
		if (Phosphate[i] < 0.0) Phosphate[i] = 0.0;
		if (Silica[i]    < 0.0)    Silica[i] = 0.0;
		if (Oxygen[i]    < 0.0)    Oxygen[i] = 0.0;
	}
}

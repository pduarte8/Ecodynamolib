// Water temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"

// TWaterTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TWaterTemperatureCrestumaLever::TWaterTemperatureCrestumaLever(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity,
        double aTemperatureOutflow[], int boxWithThermicEffluent)
    :TWaterTemperature(className, timeStep, nLines, nColumns,
                aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
	TemperatureOutflow  = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
        TemperatureOutflow[i] = aTemperatureOutflow[i];
	BoxWithThermicEffluent = boxWithThermicEffluent;
}

TWaterTemperatureCrestumaLever::TWaterTemperatureCrestumaLever(char* className,
        float timeStep, char* morphologyFilename, char* variablesFilename)
              :TWaterTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
    BuildCrestumaWaterTemperature();
}
// ...

TWaterTemperatureCrestumaLever::TWaterTemperatureCrestumaLever(TEcoDynClass* APEcoDynClass,
        char* className)
			 : TWaterTemperature(APEcoDynClass, className)
{
	// Receive pointer to integrate
    BuildCrestumaWaterTemperature();
}

void TWaterTemperatureCrestumaLever::BuildCrestumaWaterTemperature()
{
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variables
	{
		TemperatureOutflow  = new double[NumberOfBoxes];
		for (int i = 0; i < NumberOfBoxes; i++)
			TemperatureOutflow[i] = 0;
	}

	// Get forcing function file name
	VariableCount = 0;

	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
	if (PReadWrite != NULL)
	{
        int X,Y, XO, YO;

        if (PReadWrite->FindString("Water temperature", X, Y))
        {
    /*
     * already read in TWaterTemperature...
     *
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
     */
            // Read the water temperature values
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
    /*
     * already read in TWaterTemperature...
     *
                if (strcmp(MyVariable, "Water temperature") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X+3, i+j, AverageWaterTemperature[j]);
                    }
                }
                else
     */
                if (strcmp(MyVariable, "Outflow temperature") == 0)
                {
                    if (!PReadWrite->FindString("Outflow temperature values", XO, YO))
                    {
                        XO = X + 4;
                        YO = i - 1;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XO, YO - 1+j, TemperatureOutflow[j]);
                    }
                }
                else if (strcmp(MyVariable, "Box with thermic effluent") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, BoxWithThermicEffluent);
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
     }
}

TWaterTemperatureCrestumaLever::~TWaterTemperatureCrestumaLever()
{
    freeMemory();
}

void TWaterTemperatureCrestumaLever::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] TemperatureOutflow;
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
				cerr << "TWaterTemperatureCrestumaLever::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TWaterTemperatureCrestumaLever::Inquiry(char* srcName, double &Value,
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
    else if (strcmp(MyParameter, "Outflow temperature") == 0)
		Value = TemperatureOutflow[MyBoxNumber];
    else if (strcmp(MyParameter, "Relative humidity") == 0)
		Value = RelativeHumidity;
    else
        Value = 0;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TWaterTemperatureCrestumaLever::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

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
    else if (strcmp(VariableName, "Outflow temperature") == 0)
		TemperatureOutflow[BoxNumber] = Value;
    else if (strcmp(VariableName, "Relative humidity") == 0)
		RelativeHumidity = Value;
    else
        rc = false;

    return rc;
}

bool TWaterTemperatureCrestumaLever::SaveVariables()
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
        if (strcmp(VariableNameArray[i], "Box with thermic effluent") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoxWithThermicEffluent, 0);
        }
        else if (strcmp(VariableNameArray[i], "Relative humidity") == 0)
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
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Outflow temperature values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(AverageWaterTemperature[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(TemperatureOutflow[j], 9);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

void TWaterTemperatureCrestumaLever::Go()
{
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
	GetWaterTemperature();
    TemperatureOutflow[BoxWithThermicEffluent - 1] = 1.4 * AverageWaterTemperature[BoxWithThermicEffluent - 1];
}

void TWaterTemperatureCrestumaLever::Integrate()
{
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
		MyTransportPointer->Go(AverageWaterTemperature, TemperatureOutflow, GenericRiver, GenericOcean);
}







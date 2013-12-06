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
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"

// TRiaFormosaWaterTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TRiaFormosaWaterTemperature::TRiaFormosaWaterTemperature(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity, double aDailyOceanTemperature[])
    :TWaterTemperature(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
    DailyOceanTemperature  = new double[365];
    for (int i = 0; i < 365; i++)
    {
        DailyOceanTemperature[i] = aDailyOceanTemperature[i];
    }
}

TRiaFormosaWaterTemperature::TRiaFormosaWaterTemperature(char* className,
        float timeStep, char* morphologyFilename, char* variablesFilename)
              :TWaterTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
    BuildRiaFormosaWaterTemperature();
}
// ...


TRiaFormosaWaterTemperature::TRiaFormosaWaterTemperature(TEcoDynClass* APEcoDynClass,
        char* className)
            : TWaterTemperature(APEcoDynClass, className)
{
    BuildRiaFormosaWaterTemperature();
}

void TRiaFormosaWaterTemperature::BuildRiaFormosaWaterTemperature()
{
    // Initialise array for variable
    DailyOceanTemperature  = new double[365];
    for (int i = 0; i < 365; i++)
   	    DailyOceanTemperature[i] = 0;
	VariableCount = 0;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
	if (PReadWrite != NULL)
	{
        int X,Y, XD, YD;

        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            // Read the water temperature values
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
                if (strcmp(MyVariable, "Daily ocean temperature") == 0)
                {
                    if (!PReadWrite->FindString("Daily ocean temperature values", XD, YD))
                    {
                        XD = X + 4;
                        YD = i - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XD, YD + 1 +j, DailyOceanTemperature[j]);
                    }
                    break;
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TRiaFormosaWaterTemperature::~TRiaFormosaWaterTemperature()
{
	freeMemory();
}

void TRiaFormosaWaterTemperature::freeMemory()
{
    try {
        delete [] DailyOceanTemperature;
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
				cerr << "TRiaFormosaWaterTemperature::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TRiaFormosaWaterTemperature::Inquiry(char* srcName, double &Value,
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
    else if (strcmp(MyParameter, "Daily ocean temperature") == 0)
	    Value = DailyOceanTemperature[JulianDay-1];
    else
        Value = 0;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TRiaFormosaWaterTemperature::SetVariableValue(char* srcName, double Value,
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
    else if (strcmp(VariableName, "Daily ocean temperature") == 0)
    {
        for (int i = 0; i < 365; i++)
    	    DailyOceanTemperature[i] = Value;
    }
    else
        rc = false;

    return rc;
}

bool TRiaFormosaWaterTemperature::SaveVariables()
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
    bool dailyValues = false, waterValues = false;
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
        else if (strcmp(VariableNameArray[i], "Daily ocean temperature") == 0)
            dailyValues = true;
        else if (strcmp(VariableNameArray[i], "Water temperature") == 0)
            waterValues = true;
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' values
     */
    if (dailyValues) {
        PReadWrite->WriteCell("Daily ocean temperature values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < 365; j++)
        {
            PReadWrite->WriteCell(DailyOceanTemperature[j], 9);
            PReadWrite->WriteSeparator(true);
        }
    }
    if (waterValues) {
        PReadWrite->WriteCell("Water temperature values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            PReadWrite->WriteCell(AverageWaterTemperature[j], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

void TRiaFormosaWaterTemperature::Go()
{
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
	GetWaterTemperature();
	GenericRiver = 0;
    GenericOcean = DailyOceanTemperature[JulianDay - 1];
}



void TRiaFormosaWaterTemperature::Integrate()
{
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
        MyTransportPointer->Go(AverageWaterTemperature, NULL, GenericRiver, GenericOcean);
}


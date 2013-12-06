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
#include "iodll.h"
#include "EcoDynClass.h"

// TCalaWaterTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TCalaWaterTemperature::TCalaWaterTemperature(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity,
        int nParams, double modalWaterTemperature, double waterTempAmplitude,
        double waterTempPic, double waterTempPhase, double randomWaterTemp)
    :TWaterTemperature(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
                aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
	// Read in the parameters
	NumberOfParameters = nParams;
    ModalWaterTemperature = modalWaterTemperature;
    WaterTempAmplitude = waterTempAmplitude;
    WaterTempPic = waterTempPic;
    WaterTempPhase = waterTempPhase;
    RandomWaterTemp = randomWaterTemp;
}

TCalaWaterTemperature::TCalaWaterTemperature(char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename, char* parametersFilename)
              :TWaterTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
    strcpy(ParsFileName, parametersFilename);
    BuildCalaWaterTemperature();
}
// ...

TCalaWaterTemperature::TCalaWaterTemperature(TEcoDynClass* APEcoDynClass,
        char* className)
        : TWaterTemperature(APEcoDynClass, className)
{
    BuildCalaWaterTemperature();
}

void TCalaWaterTemperature::BuildCalaWaterTemperature()
{
	ModalWaterTemperature = 0;     // NG 5/6/96
	WaterTempAmplitude = 0;
	WaterTempPic = 0;
	RandomWaterTemp = 0;

	ParameterCount = 0;

	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("WaterTemperature");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "WaterTemperature parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCalaWaterTemperature::BuildCalaWaterTemperature - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        int X , Y;

        if (PReadWrite->FindString("Water Temperature", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "ModalWaterTemperature") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ModalWaterTemperature);
                }
                else if (strcmp(MyParameter, "WaterTempAmplitude") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, WaterTempAmplitude);
                }
                else if (strcmp(MyParameter, "WaterTempPic") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, WaterTempPic);
                }
                else if (strcmp(MyParameter, "WaterTempPhase") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, WaterTempPhase);
                }
                else if (strcmp(MyParameter, "RandomWaterTemp") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RandomWaterTemp);
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TCalaWaterTemperature::~TCalaWaterTemperature()
{
}

double TCalaWaterTemperature::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "ModalWaterTemperature") == 0)
    {
        value = ModalWaterTemperature;
    }
    else if (strcmp(MyParameter, "WaterTempAmplitude") == 0)
    {
        value = WaterTempAmplitude;
    }
    else if (strcmp(MyParameter, "WaterTempPic") == 0)
    {
        value = WaterTempPic;
    }
    else if (strcmp(MyParameter, "WaterTempPhase") == 0)
    {
        value = WaterTempPhase;
    }
    else if (strcmp(MyParameter, "RandomWaterTemp") == 0)
    {
        value = RandomWaterTemp;
    }
    else
        value = 0.0;
    return value;
}

bool TCalaWaterTemperature::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "ModalWaterTemperature") == 0)
    {
        ModalWaterTemperature = value;
    }
    else if (strcmp(MyParameter, "WaterTempAmplitude") == 0)
    {
        WaterTempAmplitude = value;
    }
    else if (strcmp(MyParameter, "WaterTempPic") == 0)
    {
        WaterTempPic = value;
    }
    else if (strcmp(MyParameter, "WaterTempPhase") == 0)
    {
        WaterTempPhase = value;
    }
    else if (strcmp(MyParameter, "RandomWaterTemp") == 0)
    {
        RandomWaterTemp = value;
    }
    else
        rc = false;

    return rc;
}


bool TCalaWaterTemperature::SaveParameters()
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
        if (strcmp(ParametersNameArray[i], "ModalWaterTemperature") == 0)
        {
            PReadWrite->WriteCell(ModalWaterTemperature, 8);
        }
        else if (strcmp(ParametersNameArray[i], "WaterTempAmplitude") == 0)
        {
            PReadWrite->WriteCell(WaterTempAmplitude, 8);
        }
        else if (strcmp(ParametersNameArray[i], "WaterTempPic") == 0)
        {
            PReadWrite->WriteCell(WaterTempPic, 8);
        }
        else if (strcmp(ParametersNameArray[i], "WaterTempPhase") == 0)
        {
            PReadWrite->WriteCell(WaterTempPhase, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RandomWaterTemp") == 0)
        {
            PReadWrite->WriteCell(RandomWaterTemp, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

void TCalaWaterTemperature::Go()
{
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;

	GetWaterTemperature();
}


// Protected methods

void TCalaWaterTemperature::GetWaterTemperature()
{
	//NG 5/6/96

	double MyAverageWaterTemperature;

	MyAverageWaterTemperature =
							ModalWaterTemperature
						 + ( cos ( M_PI * ( JulianDay + WaterTempPic ) / WaterTempPhase))
						 * WaterTempAmplitude
						 + random(RandomWaterTemp);

	for (int i = 0; i < NumberOfBoxes; i++)
		AverageWaterTemperature[i] = MyAverageWaterTemperature ;

}




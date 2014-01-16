// Air temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.08

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "tairobjt.h"
#include "iodll.h"

// TRiaFormosaAirTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TRiaFormosaAirTemperature::TRiaFormosaAirTemperature(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aDailyAirTemperature[])
    :TAirTemperature(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
                aElevation,aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames)
{
    for (int j = 0; j < 365; j++)
        DailyAirTemperature[j] = aDailyAirTemperature[j];
}

TRiaFormosaAirTemperature::TRiaFormosaAirTemperature(char* className, float timeStep,
            char* morphologyFilename, char* variablesFilename)
    :TAirTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
    BuildRiaFormosaAirTemperature();
}
// ...

TRiaFormosaAirTemperature::TRiaFormosaAirTemperature(TEcoDynClass* APEcodynClass,
        char* className) : TAirTemperature(APEcodynClass, className)
{
    BuildRiaFormosaAirTemperature();
}

void TRiaFormosaAirTemperature::BuildRiaFormosaAirTemperature()
{
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("AirTemperature");
	if (PReadWrite != NULL)
	{
        int X,Y, XD, YD;

        if (PReadWrite->FindString("Air temperature", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // read in the parameter names

            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                if (strcmp(MyParameter, "Air temperature") == 0)
                {
                    if (!PReadWrite->FindString("Air temperature values", XD, YD))
                    {
                        XD = X + 3;
                        YD = i - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XD, YD + 1 +j, DailyAirTemperature[j]);
                    }
                    MeanAirTemperature = DailyAirTemperature[JulianDay-1];
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}


bool TRiaFormosaAirTemperature::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("AirTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Air temperature");
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
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (strcmp(VariableNameArray[i], "Air temperature") == 0)
        {
            PReadWrite->WriteCell("Air temperature values");
            PReadWrite->WriteSeparator(true);
            for (int j = 0; j < 365; j++)
            {
                PReadWrite->WriteCell(DailyAirTemperature[j], 8);
                PReadWrite->WriteSeparator(true);
            }
        }
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

void TRiaFormosaAirTemperature::GetAirTemperature()
{
	MeanAirTemperature = DailyAirTemperature[JulianDay-1];
}


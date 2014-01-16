// Wind object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "windobjt.h"
#include "iodll.h"

// TWindDailyTimeSeries Class
//
// Constructors invoked outside EcoDyn shell...
//

TWindDailyTimeSeries::TWindDailyTimeSeries(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aDailyWindSpeed[])
    :TWind(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
	DailyWindSpeed = new double[365];
	for (int j = 0; j < 365; j++)
      	DailyWindSpeed[j] = aDailyWindSpeed[j];
}

TWindDailyTimeSeries::TWindDailyTimeSeries(char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
    :TWind(className, timeStep, morphologyFilename, variablesFilename)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildDailyWind();
}
// ...

TWindDailyTimeSeries::TWindDailyTimeSeries(TEcoDynClass* APEcodynClass, char* className)
         :TWind(APEcodynClass, className)
{
    MyPEcoDynClass = APEcodynClass;
    BuildDailyWind();
}

void TWindDailyTimeSeries::BuildDailyWind()
{
	DailyWindSpeed = new double[365];
    for (int i = 0; i < 365; i++)
        DailyWindSpeed[i] = 0.0;
    VariableCount = 0;
    //TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Wind");
    if (PReadWrite != NULL)
    {
       int X,Y, XD,YD;
        if (PReadWrite->FindString("Wind", X, Y))
        {
           char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
                if (strcmp(MyVariable, "Daily wind speed") == 0)
                {
                    if (!PReadWrite->FindString("Daily wind speed values", XD, YD))
                    {
                        XD = X + 3;
                        YD = i - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XD, YD + 1 +j, DailyWindSpeed[j]);
                    }
                }
            }
            Speed = DailyWindSpeed[JulianDay - 1];
            for (int i = 0; i < NumberOfBoxes; i++)
            {
                AverageWindSpeed[i] = Speed; // ms-1
                AverageWindSpeedX[i] = -Speed * sin(Direction); // ms-1
                AverageWindSpeedY[i] = -Speed * cos(Direction); // ms-1
            }

        }
        CloseDataFile((void*)PReadWrite);
    }
}

TWindDailyTimeSeries::~TWindDailyTimeSeries()
{
	freeMemory();
}

void TWindDailyTimeSeries::freeMemory()
{
    try {
    	delete [] DailyWindSpeed;
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
				cerr << "TWindDailyTimeSeries::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

bool TWindDailyTimeSeries::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Wind");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Wind");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names
     */
    bool dailyValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Daily wind speed") == 0)
            dailyValues = true;
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' values
     */
    if (dailyValues) {
        PReadWrite->WriteCell("Daily wind speed values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < 365; j++)
        {
            PReadWrite->WriteCell(DailyWindSpeed[j], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TWindDailyTimeSeries::CalculateWindSpeed()
{
   Speed = DailyWindSpeed[JulianDay - 1];
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		AverageWindSpeed[i] = Speed; // ms-1
		AverageWindSpeedX[i] = -Speed * sin(Direction); // ms-1
		AverageWindSpeedY[i] = -Speed * cos(Direction); // ms-1
	}
}


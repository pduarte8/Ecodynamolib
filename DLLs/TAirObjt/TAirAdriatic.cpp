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

// TAdriaticAirTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TAdriaticAirTemperature::TAdriaticAirTemperature(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[])
    :TAirTemperature(className, timeStep, nLines, nColumns,
                aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames)
{
}

TAdriaticAirTemperature::TAdriaticAirTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename)
    :TAirTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...

TAdriaticAirTemperature::TAdriaticAirTemperature(TEcoDynClass* APEcodynClass,
        char* className) : TAirTemperature(APEcodynClass, className)
{
}

void TAdriaticAirTemperature::GetAirTemperature()
{
	MeanAirTemperature = 13 * ( 1.1 - cos ( ( 2 * M_PI * ( MyTime  - 380 ) ) / 365 ) );
}


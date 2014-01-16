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

// TAtlanticoAirTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TAtlanticoAirTemperature::TAtlanticoAirTemperature(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[])
    :TAirTemperature(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
                aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames)
{
}

TAtlanticoAirTemperature::TAtlanticoAirTemperature(char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
    :TAirTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...

TAtlanticoAirTemperature::TAtlanticoAirTemperature(TEcoDynClass* APEcodynClass,
        char* className) : TAirTemperature(APEcodynClass, className)
{
}

void TAtlanticoAirTemperature::GetAirTemperature()
{
	MeanAirTemperature = 12 + 0.5 * ( 24-12 )
							* ( 1-cos ( ( 2 * 3.1416
							* (MyTime - 356 ) ) / 365 ) );
}

// Flow object CPP code file
// @  J. Gomes Ferreira
// Rev. A 96.03.11
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "flowobjt.h"
#include "EcoDynClass.h"

// Carlingford Lough flows
//
// Constructors invoked outside EcoDyn shell...
//
TCarlFlow::TCarlFlow(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow)
        :TFlow(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                      nVars, aVarsNames, modalFlow, flowAmplitude, randomFlow)
{
}

TCarlFlow::TCarlFlow(char* className, float timeStep, char* morphologyFilename, char* variablesFilename)
                        :TFlow(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...

TCarlFlow::TCarlFlow(TEcoDynClass* PEcoDynClass, char* className)
							: TFlow(PEcoDynClass, className)
{
}


void TCarlFlow::CalculateFlow()
{

	// Monthly flows
	if ((JulianDay >= 0) && (JulianDay < 25))
		MainRiverFlow = 9; // Autumn & early Winter
	else
	if ((JulianDay >= 25) && (JulianDay < 50))
		MainRiverFlow = 2; // Winter
	else
	if ((JulianDay >= 50) && (JulianDay < 100))
		MainRiverFlow = 6; // Spring thaw
	else
	if ((JulianDay >= 100) && (JulianDay < 300))
		MainRiverFlow = 1; // Summer
	else
	if ((JulianDay >= 300) && (JulianDay < 350))
		MainRiverFlow = 4; // Early Autumn
	else
	if ((JulianDay >= 350) && (JulianDay < 366))
		MainRiverFlow = 9; // Early Winter
}

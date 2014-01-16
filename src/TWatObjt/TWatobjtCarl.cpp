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
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "EcoDynClass.h"

// Carlingford Lough

// Constructors invoked outside EcoDyn shell...
//

TCarlWaterTemperature::TCarlWaterTemperature(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity)
    :TWaterTemperature(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
                aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
}

TCarlWaterTemperature::TCarlWaterTemperature(char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
              :TWaterTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...

TCarlWaterTemperature::TCarlWaterTemperature(TEcoDynClass* APEcoDynClass, char* className)
		 : TWaterTemperature(APEcoDynClass, className)
{
}

void TCarlWaterTemperature:: GetWaterTemperature()
{
	for (int MyBoxNumber = 0; MyBoxNumber < NumberOfBoxes; MyBoxNumber++)
	{
		if (MyBoxNumber == 1)
			AverageWaterTemperature[MyBoxNumber] = 7.403-0.102 * JulianDay +
								 2.397e-3 * exp(2*log(JulianDay)) -
								 1.1683e-5 * exp(3*log(JulianDay)) +
								 1.615e-8 * exp(4*log(JulianDay));
		if (MyBoxNumber == 2)
			AverageWaterTemperature[MyBoxNumber] = 7.065-0.082*(JulianDay)+
												 2.034e-3 * exp(2*log(JulianDay))-
												 1.0014e-5 * exp(3*log(JulianDay))+
												 1.3919e-8 * exp(4*log(JulianDay));
		if (MyBoxNumber == 3)
			AverageWaterTemperature[MyBoxNumber] = 6.44+0.023*(JulianDay)+
												 5.442e-4 * exp(2*log(JulianDay))-
												 3.2e-6 * exp(3*log(JulianDay))+
												 4.1343e-9 * exp(4*log(JulianDay));
	}
}


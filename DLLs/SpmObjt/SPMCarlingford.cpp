// SPM object CPP code file
// @ J. Gomes Ferreira
// Rev. A 96.02.27

/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"

// TCarlingfordSPM Class
//
// Constructors invoked outside EcoDyn shell...
//

TCarlingfordSPM::TCarlingfordSPM(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
        double aSPMConcentration[], double aSPMLoad[],
        int nParams, double localLatitude, double localHeight, double localSalt,
        double localTemperature, double turbulenceFactor, double aSPMRessuspension[])
    :TSPMBasic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, riverSPM, oceanSPM, aSPMConcentration, aSPMLoad,
                nParams, localLatitude, localHeight, localSalt,
                localTemperature, turbulenceFactor, aSPMRessuspension)
{
}

TCarlingfordSPM::TCarlingfordSPM(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
     :TSPMBasic(className, timeStep, morphologyFilename, variablesFilename, parametersFilename)
{
}
// ...


TCarlingfordSPM::TCarlingfordSPM(TEcoDynClass* APEcoDynClass, char* className)
									: TSPMBasic(APEcoDynClass, className)
{
}

void TCarlingfordSPM::Go()
{
	JulianDay = MyPEcoDynClass->GetJulianDay();

	if ((JulianDay < 90) || (JulianDay > 273))
		POCFraction = 0.03;
	else
		POCFraction = 0.05;

	// Active methods
	Flocculate();
	Deposit();
	Turbulence();
	Ressuspend();
	Dissolve();
	ExtinctionCoefficientKTot();
	ExtinctionCoefficientKChl();
}

void TCarlingfordSPM::ExtinctionCoefficientKTot()
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		switch (i)
		{
			case 0 : // Attention - zero based boxes
			{
				TotalExtinctionCoefficient[i] = 0.59;
				break;
			}
			case 1 : // Attention - zero based boxes
			{
				TotalExtinctionCoefficient[i] = 0.46;
				break;
			}
			case 2 : // Attention - zero based boxes
			{
				TotalExtinctionCoefficient[i] = 0.49;
				break;
			}
			default : TotalExtinctionCoefficient[i] = 0.1;
		}
		// Hard coded from old pascal liteobjt - 96.06.18 - JGF
		// SPM - Secchi - only three overlap dates - if we use the correlation
		// between the two for the Tagus, then the Secchi values would be much lower
		// than those measured - From 3-4m to 0.5-1m - Maybe the types of SPM differ
		// substantially - There is not enough Secchi vs. SPM data for Carlingford to explore
		// this - SPM must be decoupled from Secchi - Secchi will be defined as fixed
		// See all Excel calibration files in PXDATA\USERFILE

	}
}


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
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"

// TSPMRiaFormosaRessuspendDeposit Class
//
// Constructors invoked outside EcoDyn shell...
//

TSPMOceanic::TSPMOceanic(char* className,
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

TSPMOceanic::TSPMOceanic(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
     :TSPMBasic(className, timeStep, morphologyFilename, variablesFilename, parametersFilename)
{
}
// ...

TSPMOceanic::TSPMOceanic(TEcoDynClass* APEcoDynClass, char* className)
								 : TSPMBasic(APEcoDynClass, className)
{
}

TSPMOceanic::~TSPMOceanic()
{
    freeMemory();
}

void TSPMOceanic::freeMemory()
{
    try {
        if (NumberOfVariables > 0)
        {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
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
				cerr << "TSPMOceanic::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}



void TSPMOceanic::ExtinctionCoefficientKChl()
{
	double MyChl;

	TEcoDynClass* MyPhytoplanktonPointer = MyPEcoDynClass->GetPhytoplanktonPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{

		if (MyPhytoplanktonPointer != NULL)
			MyPhytoplanktonPointer->Inquiry(GetEcoDynClassName(), MyChl,
															i,
															"Phytoplankton biomass",
															ObjectCode);
		else ChlorophyllExtinctionCoefficient[i] = 0.0;

		if (MyChl <= 0)
		{
//			MessageBox(0, "Chlorophyll a is zero or negative", "EcoDynamo alert", MB_OK);
			ChlorophyllExtinctionCoefficient[i] = 0.0;

		}
      else
      {
			ChlorophyllExtinctionCoefficient[i] = 0.04        // f?rmula tirada de Parsons et al.,
														  + 0.0088    // Biological Oceanographic Processes
														  * MyChl
														  + 0.054
														  * exp (2 / 3.0 * log(MyChl));
         
      }
		TotalExtinctionCoefficient[i] = ChlorophyllExtinctionCoefficient[i];
	}
}


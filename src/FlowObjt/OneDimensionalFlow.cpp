// Flow object CPP code file
// @  J. Gomes Ferreira
// Rev. A 96.03.11

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <string.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "flowobjt.h"
#include "EcoDynClass.h"


// TOneDimensionalFlow Class
//
// Constructors invoked outside EcoDyn shell...
//
TOneDimensionalFlow::TOneDimensionalFlow(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow)
        :TFlow(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                      nVars, aVarsNames, modalFlow, flowAmplitude, randomFlow)
{
}

TOneDimensionalFlow::TOneDimensionalFlow(char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
                :TFlow(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...

TOneDimensionalFlow::TOneDimensionalFlow(TEcoDynClass* PEcoDynClass,
        char* className): TFlow(PEcoDynClass, className)
{
}


void TOneDimensionalFlow::Inquiry(char* srcName, double &Value,
								int BoxNumber,
								char* ParameterName,
								int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	Value = MainRiverFlow;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


void TOneDimensionalFlow::CalculateFlow()
{
	MainRiverFlow = ModalFlow;
}



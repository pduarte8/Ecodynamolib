// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 95.4.23
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "globals.h"

//Forced with continuously variable harmonics between south and north
//This one was designed for a semitimestep driven by the shell not by hydrobjt

//
// Constructor invoked outside EcoDyn shell...
//
TBiDimensionalSango3::TBiDimensionalSango3(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aUFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary, int northernTidalReference,
                   int southernTidalReference)
        :TBiDimensionalSango1(className, timeStep, nLines, nColumns, aDepth,
                      aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                      nVars, aVarsNames, aSalt, aDynamicHeight, aWaterDensity,
                      aUVelocity, aVVelocity, aUFlow, aVFlow,
                      nParams, latitude, aCoefManning, gravity, horizontalDiffCoef, startYear,
                      criticalDepthForLandBoundary, northernTidalReference, southernTidalReference)
{
}

TBiDimensionalSango3::TBiDimensionalSango3(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
            :TBiDimensionalSango1(className, timeStep, morphologyFilename,
                                    variablesFilename, parametersFilename)
{
}
//...

TBiDimensionalSango3::TBiDimensionalSango3(TEcoDynClass* PEcoDynClass, char* className)
							 :TBiDimensionalSango1(PEcoDynClass, className)
{
}

void TBiDimensionalSango3::TideForcing()
{
    //Debugger(MyTime = MyPEcoDynClass->GetCurrTime());
	double MyYear, MyMonth, MyDay;//, MySecondSurfaceHeight;
	TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();
    if (MyTidePointer != NULL)
    {
        MyYear = MyPEcoDynClass->GetYear()/*- 1 + StartYear*/;
        MyMonth = MyPEcoDynClass->GetMonth();
        MyDay = MyPEcoDynClass->GetDate();

   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    if ((BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[1].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[3].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[4].TypeOfBoundary == 2) &&
                (BoxDepth[i * GridColumns + GridColumns - 1] > CriticalDepthForLandBoundary))
            {
                MyTime = MyPEcoDynClass->GetCurrTime();
				MyTidePointer->Inquiry(GetEcoDynClassName(), SurfaceHeight, MyYear, MyMonth, MyDay, MyTime, i);
                SurfaceHeight = SurfaceHeight / CUBIC;  //convert from mm to m
                DynamicHeight[i * GridColumns + GridColumns - 1] = SurfaceHeight;
            }
        }
        for (int j = 0; j < GridColumns - 1; j++)
        {
      	    if ((BoxArray[j].BoxToBoundary[1].TypeOfBoundary == 2) ||
         		(BoxArray[j].BoxToBoundary[2].TypeOfBoundary == 2) ||
         		(BoxArray[j].BoxToBoundary[3].TypeOfBoundary == 2) ||
         		(BoxArray[j].BoxToBoundary[4].TypeOfBoundary == 2) &&
                (BoxDepth[j] > CriticalDepthForLandBoundary))
            {
         	    DynamicHeight[j] = DynamicHeight[GridColumns -1];
                DynamicHeight[(GridLines - 1) * GridColumns + j] =
            			DynamicHeight[(GridLines - 1) * GridColumns + GridColumns - 1];
            }

        }
    }
}


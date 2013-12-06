
// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 16/7/2002
/**
 * NPI work
 * 	14 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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


//
// Constructors invoked outside EcoDyn shell...
//
TCarl2D::TCarl2D(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aUFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary)
        :TBiDimensionalDouro(className, timeStep, nLines, nColumns, aDepth,
                      aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                      nVars, aVarsNames, aSalt, aDynamicHeight, aWaterDensity,
                      aUVelocity, aVVelocity, aUFlow, aVFlow,
                      nParams, latitude, aCoefManning, gravity, horizontalDiffCoef,
                      startYear, criticalDepthForLandBoundary)
{
   BuildBiDimensionalCarl();
}

TCarl2D::TCarl2D(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
    					 :TBiDimensionalDouro(className, timeStep, morphologyFilename,
                                variablesFilename, parametersFilename)
{
   BuildBiDimensionalCarl();
}
// ...

TCarl2D::TCarl2D(TEcoDynClass* PEcoDynClass, char* className)
    							 :TBiDimensionalDouro(PEcoDynClass, className)
{
   BuildBiDimensionalCarl();
}

void TCarl2D::BuildBiDimensionalCarl()
{
   TotalHours = 0.0;
}

void TCarl2D::RiverForcing()
{
   if (MyRiverRecord != NULL)
   {
      int MyHour, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
      double HourlyRiverFlow;
      NumberOfHoursForFlowTimeSeries = MyPEcoDynClass->GetNumberOfHoursForFlowTimeSeries();
      if (TotalHours >= NumberOfHoursForFlowTimeSeries)
         TotalHours = 0.0;
      MyHour = floor(TotalHours);

      for (int River = 0; River < NumberOfRivers; River++)
      {
        HourlyRiverFlow = MyRiverRecord[River].RiverFlow[MyHour] + (TotalHours - MyHour) *
                                (MyRiverRecord[River].RiverFlow[MyHour + 1] - MyRiverRecord[River].RiverFlow[MyHour]) /
                                (1.0);
        LineIndex = MyRiverRecord[River].LineCoordinate;
        ColumnIndex = MyRiverRecord[River].ColumnCoordinate;

        if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
        {
            if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[(LineIndex + 1) * GridColumns + ColumnIndex];
               //if (BoxArray[(LineIndex + 1) * GridColumns + ColumnIndex].Dummybool2)
                  VVelocity[(LineIndex + 1) * GridColumns + ColumnIndex] = HourlyRiverFlow /
                                                               (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Northern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex == GridLines - 1))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the North located at northern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "Hydrodynamic bidimensional object - RiverForcing - River flow to the North located at northern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[(LineIndex - 1) * GridColumns + ColumnIndex];
               //if (BoxArray[(LineIndex - 1) * GridColumns + ColumnIndex].Dummybool2)
                  VVelocity[LineIndex * GridColumns + ColumnIndex] = HourlyRiverFlow /
                                                               (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Southern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex == 0))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the South located at southern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "Hydrodynamic bidimensional object - RiverForcing - River flow to the South located at southern grid limit" << endl;
#endif  // __BORLANDC__
            //Debugger(LineIndex); Debugger(ColumnIndex);
            //Debugger(MyRiverRecord[River].RiverFlow[ADay]);

        }
        else
        if ((MyRiverRecord[River].BoundaryNumber == 2) || (MyRiverRecord[River].BoundaryNumber == 4))
        {
            if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns - 1))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[LineIndex * GridColumns + ColumnIndex + 1];
               //if (BoxArray[LineIndex * GridColumns + ColumnIndex + 1].Dummybool2)
                  UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex + 1] = HourlyRiverFlow /
                                                                     (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Western river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex == GridColumns - 1))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the east located at eastern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "Hydrodynamic bidimensional object - RiverForcing - River flow to the east located at eastern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[LineIndex * GridColumns + ColumnIndex - 1];
               //if (BoxArray[LineIndex * GridColumns + ColumnIndex - 1].Dummybool2)
                  UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex] = HourlyRiverFlow /
                                                                     (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Eastern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex == 0))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the West located at western grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "Hydrodynamic bidimensional object - RiverForcing - River flow to the West located at western grid limit" << endl;
#endif  // __BORLANDC__

        }
      }
   }
   TotalHours = TotalHours + MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS;
}


// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 16/7/2002
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
    #include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"


//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensionalRiaFormosa::TBiDimensionalRiaFormosa(char* className, float timeStep,
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
    BuildBiDimensionalRiaFormosa();
}

TBiDimensionalRiaFormosa::TBiDimensionalRiaFormosa(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
    					 :TBiDimensionalDouro(className, timeStep, morphologyFilename,
                                variablesFilename, parametersFilename)
{
    BuildBiDimensionalRiaFormosa();
}
// ...

TBiDimensionalRiaFormosa::TBiDimensionalRiaFormosa(TEcoDynClass* PEcoDynClass, char* className)
    							 :TBiDimensionalDouro(PEcoDynClass, className)
{
    BuildBiDimensionalRiaFormosa();
}

void TBiDimensionalRiaFormosa::BuildBiDimensionalRiaFormosa()
{
    /*int indice;
	for (int i = 0; i < GridLines; i++)
    {
   	    for (int j = 0; j < GridColumns; j++)
        {
            indice = i * GridColumns + j;
      	    if (i == 0)
         	    BoxArray[indice].BoxToBoundary[3].TypeOfBoundary = 2;  //Southern boundary
            else if (i == GridLines - 1)
         	    BoxArray[indice].BoxToBoundary[1].TypeOfBoundary = 4; //Northern boundary
      	    else if (j == 0)
            {
         	    if (BoxDepth[indice] >= CriticalDepthForLandBoundary)    //Western boundary
            	    BoxArray[indice].BoxToBoundary[4].TypeOfBoundary = 2;
                else
         	        BoxArray[indice].BoxToBoundary[4].TypeOfBoundary = 4;
            }
            else if (j == GridColumns - 1)
            {
         	    if (BoxDepth[indice] >= CriticalDepthForLandBoundary)    //Eastern boundary
            	    BoxArray[indice].BoxToBoundary[2].TypeOfBoundary = 2;
                else
         		    BoxArray[indice].BoxToBoundary[2].TypeOfBoundary = 4;
            }
        }
    } */
    //BoxArray[205 * GridColumns + 358].Dummybool1 = false;
    //BoxArray[204 * GridColumns + 358].BoxToBoundary[1].TypeOfBoundary = 1;  //Northern river boundary
    //caudal = -30; //m3/s
}

/*void TBiDimensionalRiaFormosa::RiverForcing()
{
    UVelocityPast[205 * (GridColumns + 1) + 358] = 0.0;  //Northern river boundary
   	UVelocity[205 * (GridColumns + 1) + 359] = 0.0;
   	BoxDepth[205 * GridColumns + 358] = BoxDepth[204 * GridColumns + 358];
    VVelocity[205 * GridColumns + 358] = caudal / (BoxDepth[204 * GridColumns + 358] * ASpatialStep);
    VVelocity[206 * GridColumns + 358] = 0.0;
} */

void TBiDimensionalRiaFormosa::RiverForcing()
{
   //TEcoDynClass* MyFlowPointer = MyPEcoDynClass->GetFlowPointer();
   //if (MyFlowPointer != NULL)
   if (MyRiverRecord != NULL)
   {
      int ADay;
      int LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
      NumberOfDaysForFlowTimeSeries = MyPEcoDynClass->GetNumberOfDaysForFlowTimeSeries();
      ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,
                 NumberOfDaysForFlowTimeSeries - 1);
      //Debugger(NumberOfRivers);
      for (int River = 0; River < NumberOfRivers; River++)
      {
        /*Debugger(River);

        LineNumber = MyRiverRecord[River].LineCoordinate;
        ColumnNumber = MyRiverRecord[River].ColumnCoordinate;
        BoxNumber = GetBoxNumber(ColumnNumber,LineNumber);
        LineIndex = GetLineIndex(BoxNumber);
        ColumnIndex = GetColumnIndex(BoxNumber);
        Debugger(MyRiverRecord[River].RiverFlow[ADay]);
        Debugger(MyPEcoDynClass->GetJulianDay());       Debugger(ADay); Debugger(NumberOfDaysForFlowTimeSeries);  */
        
        LineIndex = MyRiverRecord[River].LineCoordinate;
        ColumnIndex = MyRiverRecord[River].ColumnCoordinate;
        //Debugger(ADay); Debugger(LineIndex); Debugger(ColumnIndex);
        if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
        {
            if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[(LineIndex + 1) * GridColumns + ColumnIndex];
               //if (BoxArray[(LineIndex + 1) * GridColumns + ColumnIndex].Dummybool2)
                  VVelocity[(LineIndex + 1) * GridColumns + ColumnIndex] = MyRiverRecord[River].RiverFlow[ADay] /
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
                cerr << "TBiDimensionalRiaFormosa::RiverForcing - River flow to the North located at northern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[(LineIndex - 1) * GridColumns + ColumnIndex];
               //if (BoxArray[(LineIndex - 1) * GridColumns + ColumnIndex].Dummybool2)
                  VVelocity[LineIndex * GridColumns + ColumnIndex] = MyRiverRecord[River].RiverFlow[ADay] /
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
                cerr << "TBiDimensionalRiaFormosa::RiverForcing - River flow to the South located at southern grid limit" << endl;
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
                  UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex + 1] = MyRiverRecord[River].RiverFlow[ADay] /
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
                cerr << "TBiDimensionalRiaFormosa::RiverForcing - River flow to the Easst located at eastern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[LineIndex * GridColumns + ColumnIndex - 1];
               //if (BoxArray[LineIndex * GridColumns + ColumnIndex - 1].Dummybool2)
                  UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex] = MyRiverRecord[River].RiverFlow[ADay] /
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
                cerr << "TBiDimensionalRiaFormosa::RiverForcing - River flow to the West located at western grid limit" << endl;
#endif  // __BORLANDC__


        }
      }
   }
}






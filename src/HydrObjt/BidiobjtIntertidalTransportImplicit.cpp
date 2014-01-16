// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 00.1.15
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
    #include <stdlib.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
    #include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "hydrobjt.h"

TBiDimensionalIntertidalTransportImplicit::TBiDimensionalIntertidalTransportImplicit(TEcoDynClass* PEcoDynClass, char* className)
                        :TBiDimensionalRiaFormosa(PEcoDynClass, className)

{
}

void TBiDimensionalIntertidalTransportImplicit::AdvectDiffuse(double* Generic)
{
   double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
   		 MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion;
   int index, index1;
   double *PastGeneric = new double[NumberOfBoxes];

   for (int i = 0; i < NumberOfBoxes; i++)
   	  PastGeneric[i] = Generic[i];

   index1 = 0;

   for (int i = 0; i < GridLines; i++)
   {
   	  for (int j = 0; j < GridColumns; j++)
      {
         index = i * GridColumns + j;

      	 if (
               (BoxArray[index].Dummybool2) &&
               (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[index].Dummybool1)
            )
         {
         	//Advective fluxes
         	MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGeneric);
            MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGeneric);
            MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGeneric);
            MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGeneric);

            //Diffusive fluxes
            if (SemiTimeStep == 2)
            {
                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGeneric);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGeneric);
                r[index1] = r2Calculation(index, MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,MyWestDiffusion, MyEastDiffusion, PastGeneric);
                a[index1] = a2Calculation(i,j, index1);
                b[index1] = b2Calculation(i,j, index1);
                c[index1] = c2Calculation(i,j, index1);
            }
            else if (SemiTimeStep == 1)
            {
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGeneric);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGeneric);
                r[index1] = r1Calculation(index, MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,MyNorthDiffusion, MySouthDiffusion, PastGeneric);
                a[index1] = a1Calculation(i,j, index1);
                b[index1] = b1Calculation(i,j, index1);
                c[index1] = c1Calculation(i,j, index1);
            }

            index1++;
      	 }
      } //j
   } //i

   IntegrationTridag(index1);
   mprovetridag(a, b, c, r, u, index1);
   index1 = 0;

   for (int i = 0; i < GridLines; i++)
   {
   	  for (int j = 0; j < GridColumns; j++)
      {
         index = i * GridColumns + j;

      	 if (
         		//(BoxDepth[i * GridColumns + j] >= CriticalDepthForLandBoundary) &&
               (BoxArray[index].Dummybool2) &&
               (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[index].Dummybool1)
            )
         {
            Generic[index] = u[index1];
            index1++;
         }
      }
   }
   delete [] PastGeneric;
}



double TBiDimensionalIntertidalTransportImplicit::r1Calculation(int index,
                                                                double MyWestUFlow, double MyEastUFlow, double MyNorthVFlow,
                                                                double MySouthVFlow,double MyNorthDiffusion, double MySouthDiffusion,
                                                                double* PastGeneric)
{
    double aR;
    aR = PastGeneric[index] * BoxDepth[index] +
                        (
                           MyWestUFlow -
                           MyEastUFlow -
                           MyNorthVFlow +
                           MySouthVFlow
                        ) /(2.0 * ASpatialStep)+
                        (
                           MyNorthDiffusion +
                           MySouthDiffusion
                        ) / (2.0 * ASpatialStep * ASpatialStep);
     return aR;
}

double TBiDimensionalIntertidalTransportImplicit::r2Calculation(int index,
                                                                double MyWestUFlow, double MyEastUFlow, double MyNorthVFlow, double MySouthVFlow,
                                                                double MyWestDiffusion, double MyEastDiffusion,
                                                                double* PastGeneric)
{
    double aR;
    aR = PastGeneric[index]  * BoxDepth[index] +
                        (
                           MyWestUFlow -
                           MyEastUFlow -
                           MyNorthVFlow +
                           MySouthVFlow
                        ) /(2.0 * ASpatialStep)+
                        (
                           MyWestDiffusion +
                           MyEastDiffusion
                        ) / (2.0 * ASpatialStep * ASpatialStep);
    return aR;
}

void TBiDimensionalIntertidalTransportImplicit ::CorrectRForRiverAndOceanBoundaries(int index1, double Correction)
{
    r[index1] = r[index1] + Correction;
}



double TBiDimensionalIntertidalTransportImplicit :: a1Calculation(int i, int j, int index1)
{
   double a;
   int index;
   index = i * GridColumns + j;
   if (
   		 (j > 0) &&
         (BoxArray[index].Dummybool2)&&
         (BoxArray[i * GridColumns + j - 1].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
   	  )
    {
        a =  -0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[i * GridColumns + j - 1]), UPStreamUVelocity(i, j)) *
      		 	(
             		BoxDepth[index] +
               	    BoxDepth[i * GridColumns + j - 1]
             	)
             ) / (2.0 * ASpatialStep * ASpatialStep);
        if (
              (j > 0) &&
              (BoxArray[i * GridColumns + j - 1].BoxToBoundary[4].TypeOfBoundary != 2)
              //&& (BoxArray[i * GridColumns + j - 1].Dummybool1)
            )
         return a;
         else
         {
            CorrectRForRiverAndOceanBoundaries(index1, -a);
            return 0.0;
         }
    }
    else
   	   return 0.0;
}

double TBiDimensionalIntertidalTransportImplicit ::b1Calculation(int i, int j, int index1)
{
   double b, b1, b2, b3, Correction;
   int index;
   index = i * GridColumns + j;
   b1 = 0.0; b2 = 0.0; b3 = 0.0; Correction = 0.0;

   b1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
   b = b1;

   if (
         (j < GridColumns) &&
         (BoxArray[index].Dummybool2) &&
         (BoxArray[i * GridColumns + j + 1].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1)
      )
   {
      b2 = 0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[i * GridColumns + j + 1]), DOWNStreamUVelocity(i, j)) *
      		 	(
             		BoxDepth[i * GridColumns + j + 1] +
               	    BoxDepth[index]
             	)
             ) / (2.0 * ASpatialStep * ASpatialStep);
      if (BoxArray[i * GridColumns + j + 1].BoxToBoundary[2].TypeOfBoundary != 2)
         b = b + b2;
      else
         Correction = Correction + b2;

   }
   if (
         (j > 0) &&
         (BoxArray[index].Dummybool2) &&
         (BoxArray[i * GridColumns + j - 1].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
      )
   {
      b3 = 0.5 * ATimeStep *
      		 (

                HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[i * GridColumns + j - 1]), UPStreamUVelocity(i, j))*
                (
             		BoxDepth[i * GridColumns + j - 1] +
               	    BoxDepth[index]
             	)
             ) / (2.0 * ASpatialStep * ASpatialStep);
      if (BoxArray[i * GridColumns + j - 1].BoxToBoundary[4].TypeOfBoundary != 2)
         b = b + b3;
      else
         Correction = Correction + b3;
   }
   CorrectRForRiverAndOceanBoundaries(index1, -Correction);
   return b;
}

double TBiDimensionalIntertidalTransportImplicit ::c1Calculation(int i, int j, int index1)
{
   double a; int index;
   index = i * GridColumns + j;
    if (
   		 (j < GridColumns - 1) &&
         (BoxArray[index].Dummybool2) &&
         (BoxArray[i * GridColumns + j + 1].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1)
   	)
   {
   	    a = -0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[i * GridColumns + j + 1]), DOWNStreamUVelocity(i, j)) *
      		 	(
             		BoxDepth[i * GridColumns + j + 1] +
               	    BoxDepth[index]
             	) / (2.0 * ASpatialStep * ASpatialStep)
             );
        if (
              (j < GridColumns - 1) &&
              (BoxArray[i * GridColumns + j + 1].BoxToBoundary[2].TypeOfBoundary != 2)
            )
         return a;
         else
         {
            CorrectRForRiverAndOceanBoundaries(index1, -a);
            return 0.0;
         }
   }
   else
   	return 0.0;
}


double TBiDimensionalIntertidalTransportImplicit ::a2Calculation(int i, int j, int index1)
{
    double a;
    int index;
    index = i * GridColumns + j;
    if (
         (i > 0) &&
         (BoxArray[index].Dummybool2) &&
         (BoxArray[(i - 1) * GridColumns + j].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1)
   	)
    {
   	    a = -0.5 * ATimeStep *
      		 (
               HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[i * GridColumns + j] + BoxDepth[(i - 1) * GridColumns + j]), UPStreamVVelocity(i, j)) *
      		 	(
             		BoxDepth[i * GridColumns + j] +
               	    BoxDepth[(i - 1) * GridColumns + j]
             	) / (2.0 * ASpatialStep * ASpatialStep)
             );
        if (
              (i > 0) &&
              (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[3].TypeOfBoundary != 2)
            )
         return a;
         else
         {
            CorrectRForRiverAndOceanBoundaries(index1, -a);
            return 0.0;
         }
   }
   else
   	return 0.0;
}

double TBiDimensionalIntertidalTransportImplicit ::b2Calculation(int i, int j, int index1)
{
   double b, b1, b2, b3, Correction;
   int index;
   index = i * GridColumns + j;
   b1 = 0.0; b2 = 0.0; b3 = 0.0; Correction = 0.0;

   b1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
   b = b1;

   if (
         (i < GridLines - 1) &&
         (BoxArray[index].Dummybool2) &&
         (BoxArray[(i + 1) * GridColumns + j].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1)
      )
   {
      b2 = 0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]), DOWNStreamVVelocity(i,j)) *
      		 	(
             		BoxDepth[(i + 1) * GridColumns + j] +
               	    BoxDepth[index]
             	)
             ) / (2.0 * ASpatialStep * ASpatialStep);
      if (BoxArray[(i + 1) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary != 2)
         b = b + b2;
      else
         Correction = Correction + b2;

   }
   if (
         (i > 0) &&
         (BoxArray[index].Dummybool2) &&
         (BoxArray[(i - 1) * GridColumns + j].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1)
      )
   {
      b3 = 0.5 * ATimeStep *
      		 (

                HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]), UPStreamVVelocity(i, j))*
                (
             		BoxDepth[(i - 1) * GridColumns + j] +
               	    BoxDepth[index]
             	)
             ) / (2.0 * ASpatialStep * ASpatialStep);
      if (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[3].TypeOfBoundary != 2)
         b = b + b3;
      else
         Correction = Correction + b3;
   }
   CorrectRForRiverAndOceanBoundaries(index1, -Correction);
   return b;
}

double TBiDimensionalIntertidalTransportImplicit::c2Calculation(int i, int j, int index1)
{
    double a;
    int index;
    index = i * GridColumns + j;
    if (
   		  (i < GridLines - 1) &&
          (BoxArray[index].Dummybool2) &&
          (BoxArray[(i + 1) * GridColumns + j].Dummybool2) &&
          (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1)
   	   )
    {
   	    a = -0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]), DOWNStreamVVelocity(i, j)) *
      		 	(
             		BoxDepth[(i + 1) * GridColumns + j] +
               	    BoxDepth[index]
             	) / (2.0 * ASpatialStep * ASpatialStep)
             );
        if (
              (i < GridLines - 1) &&
              (BoxArray[(i + 1) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary != 2)
            )
         return a;
         else
         {
            CorrectRForRiverAndOceanBoundaries(index1, -a);
            return 0.0;
         }
   }
   else
   	return 0.0;
}

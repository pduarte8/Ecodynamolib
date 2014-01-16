// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 00.1.15
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


//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensionalIntertidal::TBiDimensionalIntertidal(char* className,
        float timeStep, char* morphologyFilename)
            :TBiDimensionalNormalTimeStep(className, timeStep, morphologyFilename)
{
    BuildBiDimensionalIntertidal();
}


TBiDimensionalIntertidal::TBiDimensionalIntertidal(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TBiDimensionalNormalTimeStep(className, timeStep, nLines, nColumns,
                      aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    BuildBiDimensionalIntertidal();
}
// ...

TBiDimensionalIntertidal::TBiDimensionalIntertidal(TEcoDynClass* PEcoDynClass,
        char* className) : TBiDimensionalNormalTimeStep(PEcoDynClass, className)

{
    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();
    BuildBiDimensionalIntertidal();
}

void TBiDimensionalIntertidal::BuildBiDimensionalIntertidal()
{
    EnableMultiThread = true;
    CriticalDepthCheck = true;

    /*for (int i = 0; i < GridLines; i++)
    {
       for (int j = 0; j < GridColumns; j++)
       {

          if ((BoxArray[i * GridColumns + j].BoxToBoundary[1].TypeOfBoundary == 1) && (i < GridLines - 1))
             BoxArray[(i + 1) * GridColumns + j].Dummybool1 = false;                              //River to the North
          if ((BoxArray[i * GridColumns + j].BoxToBoundary[2].TypeOfBoundary == 1) && (j < GridColumns - 1))
             BoxArray[i * GridColumns + j + 1].Dummybool1 = false;                                //River to the East
          if ((BoxArray[i * GridColumns + j].BoxToBoundary[3].TypeOfBoundary == 1) && (i > 0))
             BoxArray[(i - 1) * GridColumns + j].Dummybool1 = false;                              //River to the South
          if ((BoxArray[i * GridColumns + j].BoxToBoundary[4].TypeOfBoundary == 1) && (j > 0))
             BoxArray[i * GridColumns + j - 1].Dummybool1 = false;                                //River to the West
       }
    }*/ 
    MinDepthForAdvection = 0.1; //m seguindo a tese do Ramiro 28/1/2004
}

TBiDimensionalIntertidal::~TBiDimensionalIntertidal()
{
    freeMemory();
}

void TBiDimensionalIntertidal::freeMemory()
{
/*	if (NumberOfBoxes > 0)
	{
        delete [] MyLoadRecord;
	}
*/
}


void TBiDimensionalIntertidal::Go()
{
    /*if (samples == 0)
        UpdateMeanSamples(); */
    TideForcing();
    //Debugger(1);
    EquationOfMotion();
    //Debugger(2);
    UpdateOfTidalBoundariesVelocities();
    // to remove from here AP041014
/*
   	Generic = Salt;
   	Continuity();
    Salt = Generic;
*/

    UpdateMeanSamples();
    EnableReinitializeFromGo = true;
}

 /*
void TBiDimensionalIntertidal::EquationOfMotion()    //Implicit scheme
{

    int k, index, indexU, indexV;
    if (SemiTimeStep == 1)
    {
   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
                indexV = (i - 1) * GridColumns + j;
                indexU = (i + 1) * GridColumns + j;
         	    if (BoxArray[index].Dummybool2)
         	    {
         		    //Calculating intermediate depths and velocities
                    AUPStreamU = UPStreamUVelocity(i,j);
            	    ADOWNStreamU = DOWNStreamUVelocity(i,j);

                    if ((j > 0) &&
                        (((BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  		  (DynamicHeight[index - 1] < DynamicHeight[index]))
                         || ((BoxDepth[index - 1] <= CriticalDepthForLandBoundary) &&
                  			 (DynamicHeight[index] < DynamicHeight[index - 1]))))
               	        BoxArray[index].Dummybool3 = false;
            	    else
               	        BoxArray[index].Dummybool3 = true;


                    if ((j > 0) &&
            			((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) //No western boundary
                         || (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1))
                        && (BoxArray[index].Dummybool2) &&
                        (BoxArray[index - 1].Dummybool2) &&
                        ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                         (BoxDepth[index - 1] > CriticalDepthForLandBoundary)) &&
                        (BoxArray[index].Dummybool3) &&
                  	    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) && //No northern ocean boundary
                  	    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                        (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) && //No northern ocean boundary
                        (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2))   //No southern ocean boundary
                    {
               	        //Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                        if ((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                            || (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1))
                  	        rtemporary[index] = UVelocityPast[i * (GridColumns + 1) + j ];
                        else
               		        rtemporary[index] =
                     	        UVelocityPast[i * (GridColumns + 1) + j] +
                                VCoriolis(i,j) +
                                UVerticalDiffusion(i,j) -
                                UVAdvectionOfUEquation(i,j)+
                   		        GravityOfRightSideOfUEquationOfMotion(i,j);

                        // Cálculo do a, b e c
               	        atemporary[index] = aUCalculation(i,j);
                        btemporary[index] = bUCalculation(i,j);
                        ctemporary[index] = cUCalculation(i,j);


                        if (BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Left neighbour cell is a western ocean boundary
                        {
                  	        btemporary[index] = btemporary[index] + atemporary[index]
                                                * (BoxDepth[index - 1] + BoxDepth[index])
                                                / (2.0 * BoxDepth[index - 1]);

                            double Parcel1;
                            Parcel1 = 2.0 * ASpatialStep
                                    * (DynamicHeight[index - 1] - PastHeight[index - 1])
                                    / (ATimeStep * BoxDepth[index - 1]);

                            if ((i < GridLines - 1)
                                && (BoxDepth[indexU - 1] > 0.0))
                             	Parcel1 = Parcel1
                                         + (BoxDepth[index - 1] + BoxDepth[indexU - 1])
                                         * VVelocityPast[indexU - 1]
                           	             / (2.0 * BoxDepth[index - 1]);
                            else
                     	        Parcel1 = Parcel1
                                         + (BoxDepth[index - 1] + BoxDepth[index - 1])
                                         * VVelocityPast[indexU - 1]
                                         / (2.0 * BoxDepth[index - 1]);

                            if ((i > 0)
                                && (BoxDepth[indexV - 1] > 0.0))
                                Parcel1 = Parcel1
                                         - (BoxDepth[index - 1] + BoxDepth[indexV - 1])
                                         * VVelocityPast[index - 1]
                                         / (2.0 * BoxDepth[index - 1]);
                            else
                                Parcel1 = Parcel1
                                         - (BoxDepth[index - 1] + BoxDepth[index - 1])
                                         * VVelocityPast[index - 1]
                                         / (2.0 * BoxDepth[index - 1]);

                            rtemporary[index] = rtemporary[index]
                                               - atemporary[index] * Parcel1;
                            atemporary[index] = 0.0;
                        }
                        else if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Cell is a eastern ocean boundary
                        {
                  	        btemporary[index] = btemporary[index]
                                                + ctemporary[index]
                                                * (BoxDepth[index] + BoxDepth[index - 1])
                                                / (2.0 * BoxDepth[index]);

                            double Parcel2;

                            Parcel2 = - 2.0 * ASpatialStep
                                    * (DynamicHeight[index]  - PastHeight[index])
                                    / (ATimeStep * BoxDepth[index]);

                            if ((i < GridLines - 1)
                                && (BoxDepth[indexU] > 0.0))
                                Parcel2 = Parcel2
                                        - (BoxDepth[index] + BoxDepth[indexU])
                                        * VVelocityPast[indexU]
                                        / (2.0 * BoxDepth[index]);
                            else
                                Parcel2 = Parcel2 - (BoxDepth[index] + BoxDepth[index])
                                        * VVelocityPast[indexU]
                                        / (2.0 * BoxDepth[index]);

                            if ((i > 0)
                                && (BoxDepth[indexV] > 0.0))
                                Parcel2 = Parcel2
                                        + (BoxDepth[index] + BoxDepth[indexV])
                                        * VVelocityPast[index]
                                        / (2.0 * BoxDepth[index]);
                            else
                     	        Parcel2 = Parcel2
                                        + (BoxDepth[index] + BoxDepth[index])
                                        * VVelocityPast[index]
                                        / (2.0 * BoxDepth[index]);

                            rtemporary[index] = rtemporary[index]
                                               - ctemporary[index] * Parcel2;
                            ctemporary[index] = 0.0;
                        }
                    }
                    else
                    {
               	        atemporary[index] = 0.0;
                        btemporary[index] = 0.0;
                        ctemporary[index] = 0.0;
                        rtemporary[index] = 0.0;
                    }

                }
            }
        }
        k = 0;
   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
   			    if ((j > 0) &&
            		((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1))
                    && (BoxArray[index].Dummybool2)
                    && (BoxArray[index - 1].Dummybool2)
                    && ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
                    && (BoxArray[index].Dummybool3) &&
            		(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2)) //&&  //No southern ocean boundary
   			    {

   				    a[k] = atemporary[index];
      			    b[k] = btemporary[index];
      			    c[k] = ctemporary[index];
      			    r[k] = rtemporary[index];
            	    k++;
      		    }
            }
   	    }


        IntegrationTridag(a, b, c, r, k);
        mprovetridag(a, b, c, r, u, k);

   	    k = 0;
   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
                indexU = i * (GridColumns + 1) + j;
   			    if ((j > 0) &&
            		((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1))
                    &&
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[index - 1].Dummybool2) &&
                    ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
                    &&
                    (BoxArray[index].Dummybool3) &&
               	    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               	    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               	    (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2)) //&&  //No southern ocean boundary
   			    {
         		    UVelocity[indexU] = u[k];
            	    if (fabs(UVelocity[indexU]) < 0.00000000001)
            	        UVelocity[indexU] = 0.0;
            	    k++;
      		    }
			}
   	    }
        for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
                indexV = (i - 1) * GridColumns + j;
         	    if (BoxArray[index].Dummybool2)
         	    {
            	    AUPStreamV = UPStreamVVelocity(i,j);
            	    ADOWNStreamV = DOWNStreamVVelocity(i,j);

                    if ((i > 0) &&
                        (((BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[indexV] < DynamicHeight[index])
                     	) ||
                     	((BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
                         (DynamicHeight[index] < DynamicHeight[indexV])
                     	)))
                       	BoxArray[index].Dummybool3 = false;
            	    else
               	        BoxArray[index].Dummybool3 = true;

                    if ((i > 0) &&
                        (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                        ) &&
                        (BoxArray[index].Dummybool2) &&
                        (BoxArray[indexV].Dummybool2) &&
                         ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                         (BoxDepth[indexV] > CriticalDepthForLandBoundary))
                        &&
                         (BoxArray[index].Dummybool3) &&
                  	     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	     (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                         (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                         (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)) //&&  //No eastern ocean boundary

                     //De notar que este conjunto de condições lógicas não impede que seja calculada a velocidade
                     //explícita para uma célula que seja fronteira fluvial. No entanto, no final do passo de cálculo é imposta
                     //nessa célula a velocidade calculada a partir do caudal fluvial. Poderiam haver entretanto problemas no
                     //cálculo da continuidade e do transporte, mas não se passa nada de errado porque os referidos cálculos se baseiam
                     //nos termos explícitos passados (actualizados no final d cada passode cálculo. Os implicitos são os presentes, mas quanto a esses
                     //não há problemas porque o a=0, o b=1 e o c=0 nas fornteiras fluviais pelo que o cálculo implicito da velocidade não
                     //muda o seu valor nas fronteiras fluviais.
                    {
                        VVelocity[index] =
              			  	VVelocityPast[index] -
                     	    VAdvectionOfVEquation(i,j,1)-
                     	    UVAdvectionOfVEquation(i,j) -
                       	    GravityOfVEquation(i,j,1) -
                       	    DragOfVEquation(i,j) * VVelocityPast[index] -
                            UCoriolis(i,j)+
                            VVerticalDiffusion(i,j) +
                            VHorizontalDiffusion(i,j);
                    }
                }
            }
        }
    }
            //Second semi-timestep
    else if (SemiTimeStep == 2)
    {
      for (int i = 0; i < GridLines; i++)
   	{
   		for (int j = 0; j < GridColumns; j++)
      	{
            index = i * GridColumns + j;
            indexV = (i - 1) * GridColumns + j;
         	if (BoxArray[index].Dummybool2)
         	{

               AUPStreamV = UPStreamVVelocity(i,j);
            	ADOWNStreamV = DOWNStreamVVelocity(i,j);
               //vs  implicit

               if (
               	   (i > 0) &&
                     (
                     	(
            					(BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[indexV] < DynamicHeight[index])
                     	) ||
                     	(
            					(BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[index] < DynamicHeight[indexV])
                     	)
                     )
               	)
               	BoxArray[index].Dummybool3 = false;
            	else
               	BoxArray[index].Dummybool3 = true;



               if (
                     (i > 0) &&
                     (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                     ) &&
                     (BoxArray[index].Dummybool2) &&
                     (BoxArray[indexV].Dummybool2) &&
                     (
                     	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                        (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                     ) &&
                     (BoxArray[index].Dummybool3) &&
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                     (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
                     //(BoxArray[index].Dummybool1)
                  )
               {
               	//Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                 if (
                 			(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
                        (BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                     )
                  rtemporary[index] = VVelocityPast[index];
                 else
                 	rtemporary[index] =
                     	VVelocityPast[index] -
                        UCoriolis(i,j) +
                        VHorizontalDiffusion(i,j) -
                        UVAdvectionOfVEquation(i,j) +
                   		GravityOfRightSideOfVEquationOfMotion(i,j);


                 // Cálculo do a, b e c
                 atemporary[index] = aVCalculation(i,j);
                 btemporary[index] = bVCalculation(i,j);
                 ctemporary[index] = cVCalculation(i,j);




                 if (BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) //South neighbour cell is a southern ocean boundary
                 {
                  	btemporary[index] = btemporary[index] + atemporary[index] *
                                        (
                                         BoxDepth[indexV] +
                                         BoxDepth[index]
                                        ) / (2.0 * BoxDepth[indexV]);

                     double Parcel3;

                     Parcel3 = 2.0 * ASpatialStep * (DynamicHeight[indexV]  - PastHeight[indexV]) /
                              (ATimeStep * BoxDepth[indexV]);

                     if ((j > 0) &&
                           (BoxDepth[indexV - 1] > 0.0)
                        )
                        Parcel3 = Parcel3 - (BoxDepth[indexV] + BoxDepth[indexV - 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[indexV]);
                     else
                     	Parcel3 = Parcel3 - (BoxDepth[indexV] + BoxDepth[indexV]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[indexV]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[indexV + 1] > 0.0)
                        )
                        Parcel3 = Parcel3 + (BoxDepth[indexV] + BoxDepth[indexV + 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[indexV]);
                     else
                     	Parcel3 = Parcel3 + (BoxDepth[indexV] + BoxDepth[indexV]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[indexV]);

                     rtemporary[index] = rtemporary[index] - atemporary[index] *  Parcel3;


                     atemporary[index] = 0.0;
                  }
                  else
                  if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Cell is a northern ocean boundary
                  {
                  	btemporary[index] = btemporary[index] + ctemporary[index] *
                                        (BoxDepth[index] + BoxDepth[indexV]) /
                                        (2.0 * BoxDepth[index]);

                     double Parcel4;

                     Parcel4 = - 2.0 * ASpatialStep * (DynamicHeight[index]  - PastHeight[index]) /
                              (ATimeStep * BoxDepth[index]);


                     if ((j > 0) &&
                           (BoxDepth[index - 1] > 0.0)
                        )
                        Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index - 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     else
                     	Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[index + 1] > 0.0)
                        )
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index + 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);
                     else
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);

                     rtemporary[index] = rtemporary[index] - ctemporary[index] * Parcel4;



                     ctemporary[index] = 0.0;
                  }
               }
               else
               {
               	atemporary[index] = 0.0; btemporary[index] = 0.0;
                  ctemporary[index] = 0.0; rtemporary[index] = 0.0;
               }
            }
         }
      }
      k = 0;
      for (int j = 0; j < GridColumns; j++)
   	{
      	for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
            indexV = (i - 1) * GridColumns + j;
   			if ((i > 0) &&
                  (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  ) &&

                  (BoxArray[index].Dummybool2) &&
                  (BoxArray[indexV].Dummybool2) &&
                  (
                  	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                  ) &&
                  (BoxArray[index].Dummybool3) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                  (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
                  //(BoxArray[index].Dummybool1)
          		)
   			{
   				a[k] = atemporary[index];
      			b[k] = btemporary[index];
      			c[k] = ctemporary[index];
      			r[k] = rtemporary[index];
            	k++;
      		}
			}
   	}


      IntegrationTridag(a, b, c, r, k);
      mprovetridag(a, b, c, r, u, k);
   	//IntegrationTridag(k);


      k = 0;

      for (int j = 0; j < GridColumns; j++)
   	{
      	for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
            indexV = (i - 1) * GridColumns + j;
   			if (
      				(i > 0) &&
                  (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  ) &&

                  (BoxArray[index].Dummybool2) &&
                  (BoxArray[indexV].Dummybool2) &&
                  (
                  	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                  ) &&
                  (BoxArray[index].Dummybool3) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                  (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)//&&   //No eastern ocean boundary
                  //(BoxArray[index].Dummybool1)
          		)
   			{
         		VVelocity[index] = u[k];

               if (fabs(VVelocity[index]) < 0.00000000001)
               VVelocity[index] = 0.0;
            	k++;
      		}
			}
   	}
      //us explicit

      for (int i = 0; i < GridLines; i++)
   	{
   		for (int j = 0; j < GridColumns; j++)
      	{
            index = i * GridColumns + j;
         	if (BoxArray[index].Dummybool2)
         	{
               AUPStreamU = UPStreamUVelocity(i,j);
            	ADOWNStreamU = DOWNStreamUVelocity(i,j);

               if ((j > 0) &&
                     (((BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[index - 1] < DynamicHeight[index])
                     	) ||
                     	((BoxDepth[index - 1] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[index] < DynamicHeight[index - 1])
                     	)
                     )
               	)
               	BoxArray[index].Dummybool3 = false;
            	else
               	BoxArray[index].Dummybool3 = true;

               if (
               		(j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&

                     (BoxArray[index].Dummybool2) &&
                     (BoxArray[index - 1].Dummybool2) &&
                     (
                     	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
                     ) &&
                     (BoxArray[index].Dummybool3) &&
                  	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&   //No southern ocean boundary
               	)
               {
                   	  	UVelocity[i * (GridColumns + 1) + j ] =
                       		UVelocityPast[i * (GridColumns + 1) + j ] -
                           UAdvectionOfUEquation(i,j,1) -
                           UVAdvectionOfUEquation(i,j) -
                           GravityOfUEquation(i,j,1)-
                           DragOfUEquation(i,j)  * UVelocityPast[i * (GridColumns + 1) + j ]+
                           VCoriolis(i,j)+
                           UHorizontalDiffusion(i,j) +
                           UVerticalDiffusion(i,j);
               }
           	}
         }
      } //j
	} //i
}  */



void TBiDimensionalIntertidal::EquationOfMotion()    //Implicit scheme
{

    int k, index, indexU, indexV;
    if (SemiTimeStep == 1)
    {
        k = 0;
   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
                indexV = (i - 1) * GridColumns + j;
                indexU = (i + 1) * GridColumns + j;
         	    if (BoxArray[index].Dummybool2)
         	    {
         		    //Calculating intermediate depths and velocities
                    AUPStreamU = UPStreamUVelocity(i,j);
            	    ADOWNStreamU = DOWNStreamUVelocity(i,j);

                    if ((j > 0) &&
                        (((BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  		  (DynamicHeight[index - 1] < DynamicHeight[index]))
                         || ((BoxDepth[index - 1] <= CriticalDepthForLandBoundary) &&
                  			 (DynamicHeight[index] < DynamicHeight[index - 1]))))
               	        BoxArray[index].Dummybool3 = false;
            	    else
               	        BoxArray[index].Dummybool3 = true;


                    if ((j > 0) &&
            			((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) //No western boundary
                         || (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1))
                        && (BoxArray[index].Dummybool2) &&
                        (BoxArray[index - 1].Dummybool2) &&
                        ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                         (BoxDepth[index - 1] > CriticalDepthForLandBoundary)) &&
                        (BoxArray[index].Dummybool3) &&
                  	    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) && //No northern ocean boundary
                  	    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                        (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) && //No northern ocean boundary
                        (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) && //No southern ocean boundary
                        (BoxArray[index].Dummybool1)) //No river output cell
                    {
               	        //Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                        if ((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                            || (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1))
                  	        rtemporary[index] = UVelocityPast[i * (GridColumns + 1) + j ];
                        else
               		        rtemporary[index] =
                     	        UVelocityPast[i * (GridColumns + 1) + j] +
                                VCoriolis(i,j) +
                                UVerticalDiffusion(i,j) -
                                UVAdvectionOfUEquation(i,j)+
                   		        GravityOfRightSideOfUEquationOfMotion(i,j);

                        // Cálculo do a, b e c
               	        atemporary[index] = aUCalculation(i,j);
                        btemporary[index] = bUCalculation(i,j);
                        ctemporary[index] = cUCalculation(i,j);


                        if (BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Left neighbour cell is a western ocean boundary
                        {
                  	        btemporary[index] = btemporary[index] + atemporary[index]
                                                * (BoxDepth[index - 1] + BoxDepth[index])
                                                / (2.0 * BoxDepth[index - 1]);

                            double Parcel1;
                            Parcel1 = 2.0 * ASpatialStep
                                    * (DynamicHeight[index - 1] - PastHeight[index - 1])
                                    / (ATimeStep * BoxDepth[index - 1]);

                            if ((i < GridLines - 1)
                                && (BoxDepth[indexU - 1] > 0.0))
                             	Parcel1 = Parcel1
                                         + (BoxDepth[index - 1] + BoxDepth[indexU - 1])
                                         * VVelocityPast[indexU - 1]
                           	             / (2.0 * BoxDepth[index - 1]);
                            else
                     	        Parcel1 = Parcel1
                                         + (BoxDepth[index - 1] + BoxDepth[index - 1])
                                         * VVelocityPast[indexU - 1]
                                         / (2.0 * BoxDepth[index - 1]);

                            if ((i > 0)
                                && (BoxDepth[indexV - 1] > 0.0))
                                Parcel1 = Parcel1
                                         - (BoxDepth[index - 1] + BoxDepth[indexV - 1])
                                         * VVelocityPast[index - 1]
                                         / (2.0 * BoxDepth[index - 1]);
                            else
                                Parcel1 = Parcel1
                                         - (BoxDepth[index - 1] + BoxDepth[index - 1])
                                         * VVelocityPast[index - 1]
                                         / (2.0 * BoxDepth[index - 1]);

                            rtemporary[index] = rtemporary[index]
                                               - atemporary[index] * Parcel1;
                            atemporary[index] = 0.0;
                        }
                        else if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Cell is a eastern ocean boundary
                        {
                  	        btemporary[index] = btemporary[index]
                                                + ctemporary[index]
                                                * (BoxDepth[index] + BoxDepth[index - 1])
                                                / (2.0 * BoxDepth[index]);

                            double Parcel2;

                            Parcel2 = - 2.0 * ASpatialStep
                                    * (DynamicHeight[index]  - PastHeight[index])
                                    / (ATimeStep * BoxDepth[index]);

                            if ((i < GridLines - 1)
                                && (BoxDepth[indexU] > 0.0))
                                Parcel2 = Parcel2
                                        - (BoxDepth[index] + BoxDepth[indexU])
                                        * VVelocityPast[indexU]
                                        / (2.0 * BoxDepth[index]);
                            else
                                Parcel2 = Parcel2 - (BoxDepth[index] + BoxDepth[index])
                                        * VVelocityPast[indexU]
                                        / (2.0 * BoxDepth[index]);

                            if ((i > 0)
                                && (BoxDepth[indexV] > 0.0))
                                Parcel2 = Parcel2
                                        + (BoxDepth[index] + BoxDepth[indexV])
                                        * VVelocityPast[index]
                                        / (2.0 * BoxDepth[index]);
                            else
                     	        Parcel2 = Parcel2
                                        + (BoxDepth[index] + BoxDepth[index])
                                        * VVelocityPast[index]
                                        / (2.0 * BoxDepth[index]);

                            rtemporary[index] = rtemporary[index]
                                               - ctemporary[index] * Parcel2;
                            ctemporary[index] = 0.0;
                        }
                        a[k] = atemporary[index];
      			        b[k] = btemporary[index];
      			        c[k] = ctemporary[index];
      			        r[k] = rtemporary[index];
            	        k++;
                    }
                    else
                    {
               	        atemporary[index] = 0.0;
                        btemporary[index] = 0.0;
                        ctemporary[index] = 0.0;
                        rtemporary[index] = 0.0;
                    }

                }
            }
        }
        /*k = 0;
   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
   			    if ((j > 0) &&
            		((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1))
                    && (BoxArray[index].Dummybool2)
                    && (BoxArray[index - 1].Dummybool2)
                    && ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
                    && (BoxArray[index].Dummybool3) &&
            		(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2)) //&&  //No southern ocean boundary
   			    {

   				    a[k] = atemporary[index];
      			    b[k] = btemporary[index];
      			    c[k] = ctemporary[index];
      			    r[k] = rtemporary[index];
            	    k++;
      		    }
            }
   	    } */


        IntegrationTridag(a, b, c, r, k);
        //mprovetridag(a, b, c, r, u, k);

   	    k = 0;
   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
                indexU = i * (GridColumns + 1) + j;
   			    if ((j > 0) &&
            		((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1))
                    &&
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[index - 1].Dummybool2) &&
                    ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
                    &&
                    (BoxArray[index].Dummybool3) &&
               	    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
               	    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               	    (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                    (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                    (BoxArray[index].Dummybool1)) //No river output cell
   			    {
         		    UVelocity[indexU] = u[k];
            	    if (fabs(UVelocity[indexU]) < 0.00000000001)
            	        UVelocity[indexU] = 0.0;
            	    k++;
      		    }
			}
   	    }

        // to add AP041014
        DoContinuity();

        for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
      	    {
                index = i * GridColumns + j;
                indexV = (i - 1) * GridColumns + j;
         	    if (BoxArray[index].Dummybool2)
         	    {
            	    AUPStreamV = UPStreamVVelocity(i,j);
            	    ADOWNStreamV = DOWNStreamVVelocity(i,j);

                    if ((i > 0) &&
                        (((BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                			//(DynamicHeight[indexV] < DynamicHeight[index])
                  			(PastHeight[indexV] < PastHeight[index])
                     	) ||
                     	((BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
                         //(DynamicHeight[index] < DynamicHeight[indexV])
                         (PastHeight[index] < PastHeight[indexV])
                     	)))
                       	BoxArray[index].Dummybool3 = false;
            	    else
               	        BoxArray[index].Dummybool3 = true;

                    if ((i > 0) &&
                        (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                        ) &&
                        (BoxArray[index].Dummybool2) &&
                        (BoxArray[indexV].Dummybool2) &&
                         ((BoxDepth[index] > CriticalDepthForLandBoundary) ||
                         (BoxDepth[indexV] > CriticalDepthForLandBoundary))
                        &&
                         (BoxArray[index].Dummybool3) &&
                  	     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	     (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                         (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                         (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                         (BoxArray[index].Dummybool1)) //No river output cell
                     //De notar que este conjunto de condições lógicas não impede que seja calculada a velocidade
                     //explícita para uma célula que seja fronteira fluvial. No entanto, no final do passo de cálculo é imposta
                     //nessa célula a velocidade calculada a partir do caudal fluvial. Poderiam haver entretanto problemas no
                     //cálculo da continuidade e do transporte, mas não se passa nada de errado porque os referidos cálculos se baseiam
                     //nos termos explícitos passados (actualizados no final d cada passode cálculo. Os implicitos são os presentes, mas quanto a esses
                     //não há problemas porque o a=0, o b=1 e o c=0 nas fornteiras fluviais pelo que o cálculo implicito da velocidade não
                     //muda o seu valor nas fronteiras fluviais.
                    {
                        VVelocity[index] =
              			  	VVelocityPast[index] -
                     	    VAdvectionOfVEquation(i,j,1)-
                     	    UVAdvectionOfVEquation(i,j) -
                       	    GravityOfVEquation(i,j,1) -
                       	    DragOfVEquation(i,j) * VVelocityPast[index] -
                            UCoriolis(i,j)+
                            VVerticalDiffusion(i,j) +
                            VHorizontalDiffusion(i,j);
                    }
                }
            }
        }
    }
            //Second semi-timestep
    else if (SemiTimeStep == 2)
    {
      k = 0;
      //for (int i = 0; i < GridLines; i++)
      for (int j = 0; j < GridColumns; j++)
   	  {

   		//for (int j = 0; j < GridColumns; j++)
        for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
            indexV = (i - 1) * GridColumns + j;
         	if (BoxArray[index].Dummybool2)
         	{

               AUPStreamV = UPStreamVVelocity(i,j);
            	ADOWNStreamV = DOWNStreamVVelocity(i,j);
               //vs  implicit

               if (
               	   (i > 0) &&
                     (
                     	(
            					(BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[indexV] < DynamicHeight[index])
                     	) ||
                     	(
            					(BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
                  			(DynamicHeight[index] < DynamicHeight[indexV])
                     	)
                     )
               	)
               	BoxArray[index].Dummybool3 = false;
            	else
               	BoxArray[index].Dummybool3 = true;


               if (
                     (i > 0) &&
                     (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                     ) &&
                     (BoxArray[index].Dummybool2) &&
                     (BoxArray[indexV].Dummybool2) &&
                     (
                     	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                        (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                     ) &&
                     (BoxArray[index].Dummybool3) &&
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                     (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                     (BoxArray[index].Dummybool1)
                  )
               {

               	//Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                 if (
                 			(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
                        (BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                     )
                  rtemporary[index] = VVelocityPast[index];
                 else
                 	rtemporary[index] =
                     	VVelocityPast[index] -
                        UCoriolis(i,j) +
                        VHorizontalDiffusion(i,j) -
                        UVAdvectionOfVEquation(i,j) +
                   		GravityOfRightSideOfVEquationOfMotion(i,j);


                 // Cálculo do a, b e c
                 atemporary[index] = aVCalculation(i,j);
                 btemporary[index] = bVCalculation(i,j);
                 ctemporary[index] = cVCalculation(i,j);



                 if (BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) //South neighbour cell is a southern ocean boundary
                 {
                  	btemporary[index] = btemporary[index] + atemporary[index] *
                                        (
                                         BoxDepth[indexV] +
                                         BoxDepth[index]
                                        ) / (2.0 * BoxDepth[indexV]);

                     double Parcel3;

                     Parcel3 = 2.0 * ASpatialStep * (DynamicHeight[indexV]  - PastHeight[indexV]) /
                              (ATimeStep * BoxDepth[indexV]);

                     if ((j > 0) &&
                           (BoxDepth[indexV - 1] > 0.0)
                        )
                        Parcel3 = Parcel3 - (BoxDepth[indexV] + BoxDepth[indexV - 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[indexV]);
                     else
                     	Parcel3 = Parcel3 - (BoxDepth[indexV] + BoxDepth[indexV]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[indexV]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[indexV + 1] > 0.0)
                        )
                        Parcel3 = Parcel3 + (BoxDepth[indexV] + BoxDepth[indexV + 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[indexV]);
                     else
                     	Parcel3 = Parcel3 + (BoxDepth[indexV] + BoxDepth[indexV]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[indexV]);

                     rtemporary[index] = rtemporary[index] - atemporary[index] *  Parcel3;


                     atemporary[index] = 0.0;
                  }
                  else
                  if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Cell is a northern ocean boundary
                  {
                  	btemporary[index] = btemporary[index] + ctemporary[index] *
                                        (BoxDepth[index] + BoxDepth[indexV]) /
                                        (2.0 * BoxDepth[index]);

                     double Parcel4;

                     Parcel4 = - 2.0 * ASpatialStep * (DynamicHeight[index]  - PastHeight[index]) /
                              (ATimeStep * BoxDepth[index]);


                     if ((j > 0) &&
                           (BoxDepth[index - 1] > 0.0)
                        )
                        Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index - 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     else
                     	Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[index + 1] > 0.0)
                        )
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index + 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);
                     else
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);

                     rtemporary[index] = rtemporary[index] - ctemporary[index] * Parcel4;



                     ctemporary[index] = 0.0;
                  }
                  a[k] = atemporary[index];
                  b[k] = btemporary[index];
      			  c[k] = ctemporary[index];
      			  r[k] = rtemporary[index];
            	k++;
                /*if (k == 2761)
                {
                   Debugger(121212); Debugger(i); Debugger(j);
                } */
               }
               else
               {
               	atemporary[index] = 0.0; btemporary[index] = 0.0;
                  ctemporary[index] = 0.0; rtemporary[index] = 0.0;
               }
            }
         }
      }
       //if ((j > 211) && (i > 225))
       /*Debugger(11); Debugger(k);
       Debugger(a[2761]);
       Debugger(b[2761]);
       Debugger(c[2761]);
       Debugger(r[2761]); */

      /*k = 0;
      for (int j = 0; j < GridColumns; j++)
   	  {
      	for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
            indexV = (i - 1) * GridColumns + j;
   			if ((i > 0) &&
                  (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  ) &&

                  (BoxArray[index].Dummybool2) &&
                  (BoxArray[indexV].Dummybool2) &&
                  (
                  	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                  ) &&
                  (BoxArray[index].Dummybool3) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                  (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
                  //(BoxArray[index].Dummybool1)
          		)
   			{
   				a[k] = atemporary[index];
      			b[k] = btemporary[index];
      			c[k] = ctemporary[index];
      			r[k] = rtemporary[index];
            	k++;
      		}
		  }
   	   }  */


      IntegrationTridag(a, b, c, r, k);
      //mprovetridag(a, b, c, r, u, k);
   	//IntegrationTridag(k);

      //Debugger(12);
      
      k = 0;

      for (int j = 0; j < GridColumns; j++)
   	{
      	for (int i = 0; i < GridLines; i++)
      	{
            index = i * GridColumns + j;
            indexV = (i - 1) * GridColumns + j;
   			if (
      				(i > 0) &&
                  (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                  ) &&

                  (BoxArray[index].Dummybool2) &&
                  (BoxArray[indexV].Dummybool2) &&
                  (
                  	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                  ) &&
                  (BoxArray[index].Dummybool3) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                  (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&&   //No eastern ocean boundary
                  (BoxArray[index].Dummybool1)
                  )
   			{
         		VVelocity[index] = u[k];

               if (fabs(VVelocity[index]) < 0.00000000001)
               VVelocity[index] = 0.0;
            	k++;
      		}
			}
   	}

    // to add AP041014
    DoContinuity();
      //us explicit

      for (int i = 0; i < GridLines; i++)
   	{
   		for (int j = 0; j < GridColumns; j++)
      	{
            index = i * GridColumns + j;
         	if (BoxArray[index].Dummybool2)
         	{
               AUPStreamU = UPStreamUVelocity(i,j);
            	ADOWNStreamU = DOWNStreamUVelocity(i,j);

               if ((j > 0) &&
                     (((BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  			//(DynamicHeight[index - 1] < DynamicHeight[index])
                  			(PastHeight[index - 1] < PastHeight[index])
                     	) ||
                     	((BoxDepth[index - 1] <= CriticalDepthForLandBoundary) &&
                  			//(DynamicHeight[index] < DynamicHeight[index - 1])
                  			(PastHeight[index] < PastHeight[index - 1])
                     	)
                     )
               	)
               	BoxArray[index].Dummybool3 = false;
            	else
               	BoxArray[index].Dummybool3 = true;

               if (
               		(j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&

                     (BoxArray[index].Dummybool2) &&
                     (BoxArray[index - 1].Dummybool2) &&
                     (
                     	(BoxDepth[index] > CriticalDepthForLandBoundary) ||
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
                     ) &&
                     (BoxArray[index].Dummybool3) &&
                  	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&   //No southern ocean boundary
                     (BoxArray[index].Dummybool1) //No river output cell
               	)
               {
                   	  	UVelocity[i * (GridColumns + 1) + j ] =
                       		UVelocityPast[i * (GridColumns + 1) + j ] -
                           UAdvectionOfUEquation(i,j,1) -
                           UVAdvectionOfUEquation(i,j) -
                           GravityOfUEquation(i,j,1)-
                           DragOfUEquation(i,j)  * UVelocityPast[i * (GridColumns + 1) + j ]+
                           VCoriolis(i,j)+
                           UHorizontalDiffusion(i,j) +
                           UVerticalDiffusion(i,j);
               }
           	}
         }
      } //j
	} //i
}


void TBiDimensionalIntertidal::DoContinuity()
{
    if (IsMultiThreadedVersion)
        EnableContinuity = true;
    else
        Continuity();
}


void TBiDimensionalIntertidal::Continuity()
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow;
    int index;

    EnableContinuity = false;       // AP, 2005.09.16 - moved to here

//    Generic = Salt;

   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        index = i * GridColumns + j;
      	if (
               //(BoxArray[index].Dummybool2) &&
               (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[index].Dummybool1)
            )
         {
            //Debugger(10);
         	MyWestUFlow = WesternComponentOfContinuity(i,j);
            //Debugger(11);
            MyEastUFlow = EasternComponentOfContinuity(i,j);
            //Debugger(12);
            MyNorthVFlow = NorthernComponentOfContinuity(i,j);
            //Debugger(13);
            MySouthVFlow = SouthernComponentOfContinuity(i,j);
            //Debugger(14);



            DynamicHeight[index] = (2.0 * ASpatialStep *
                                        PastHeight[index] +
                                        MyWestUFlow -
                                        MyEastUFlow -
                                        MyNorthVFlow +
                                        MySouthVFlow
                                    ) / (2.0 * ASpatialStep);
            if (MyLoadRecord != NULL)
            {
               for (int k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
               {
                    if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                     {
                        int ADay;
                        //Debugger(MyPEcoDynClass->GetJulianDay()); Debugger(MyPEcoDynClass->GetNumberOfDaysForLoads());
                        if (MyPEcoDynClass->GetJulianDay() > MyPEcoDynClass->GetNumberOfDaysForLoads())
                            ADay = GetNumberOfDaysForLoads();
                        else
                            ADay = MyPEcoDynClass->GetJulianDay();
                        //Debugger(k); Debugger(i); Debugger(j);
                        //Debugger(ADay); Debugger( MyLoadRecord[k].LoadFlow[ADay]);
                        DynamicHeight[index] = DynamicHeight[index] + ATimeStep / 2.0 *
                                                         MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                         (
                                                            //(BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                                            (ASpatialStep * ASpatialStep)
                                                         );
                    }
               }
            }

      	}
      }
	}
    GenericLoad = SaltLoad;
    AdvectDiffuse(Salt);
//    Salt = Generic;
//    EnableContinuity = false;
}


void TBiDimensionalIntertidal::AdvectDiffuse(double* Generic)
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
   		 MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion;
    int index;
    double* PastGeneric = new double[NumberOfBoxes];

   for (int i = 0; i < NumberOfBoxes; i++)
   	PastGeneric[i] = Generic[i];

   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        index = i * GridColumns + j;

      	if (
               //(BoxArray[index].Dummybool2) &&
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
            MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGeneric);

            MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGeneric);

            MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGeneric);

            MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGeneric);

            Generic[index ] = PastGeneric[index]  *
                    BoxDepth[index] /
                    (BoxDepth[index] + DynamicHeight[index] - PastHeight[index])
                    +
                    (
                     MyWestUFlow -
                     MyEastUFlow -
                     MyNorthVFlow +
                     MySouthVFlow
                    ) /
                    (2.0 * ASpatialStep) /
                     (BoxDepth[index] + DynamicHeight[index] - PastHeight[index])
                    +
                    (
                     MyWestDiffusion +
                     MyEastDiffusion +
                     MyNorthDiffusion +
                     MySouthDiffusion
                     ) /
                     (2.0 * ASpatialStep * ASpatialStep) /
                      (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]);
            if (MyLoadRecord != NULL)
            {
               for (int k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
               {
                    if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                     {
                        int ADay;
                        if (MyPEcoDynClass->GetJulianDay() > MyPEcoDynClass->GetNumberOfDaysForLoads())
                            ADay = GetNumberOfDaysForLoads();
                        else
                            ADay = MyPEcoDynClass->GetJulianDay();

                        Generic[index ] = Generic[index ] + ATimeStep * GenericLoad[index]* /* Loads[k] */
                                                         MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                         (
                                                            (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                                            (ASpatialStep * ASpatialStep)
                                                         );
                         //Loads[k] = 0.0;                                
                    }
               }
            }
      	 }
      }
   }
   delete [] PastGeneric;
}


void TBiDimensionalIntertidal::TideForcing()
{
#ifdef __BORLANDC__
	MessageBox(0,
					  "Hydrodynamic bidimensional object",
					  "Always override Tide Forcing",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::TideForcing - Always override Tide Forcing" << endl;
#endif  // __BORLANDC__


}


void TBiDimensionalIntertidal::RiverForcing()
{
#ifdef __BORLANDC__
	MessageBox(0,
					  "Hydrodynamic bidimensional object",
					  "Always override River Forcing",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::RiverForcing - Always override River Forcing" << endl;
#endif  // __BORLANDC__
}






void TBiDimensionalIntertidal::UpdateOfTidalBoundariesVelocities()
{

	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        int index = i * GridColumns + j;
   		if (
               (BoxArray[index].Dummybool2) &&
               ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) ||
               	(BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) ||
               	(BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||
               	(BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2)
               )
   			)
      	{

         	if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) && (i > 0) && (i < GridLines))  //Northern sea boundary
            {
            	VVelocity[(i + 1) * GridColumns + j] = (
            			                               		VVelocity[index] *
                                                      	(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[index]) / 2.0 -
                                                      	(DynamicHeight[index] - PastHeight[index]) *
                                                         ASpatialStep / (ATimeStep/ 2.0)
                                                   	) / BoxDepth[index];
            }
            else if ((BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) && (i < GridLines))  //Southern sea boundary
            {
               VVelocity[index] =       (
            			                               		VVelocity[(i + 1) * GridColumns + j] *
                                                      	(BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[index]) / 2.0 +
                                                      	(DynamicHeight[index] - PastHeight[index]) *
                                                         ASpatialStep / (ATimeStep/ 2.0)
                                                   	) / BoxDepth[index];
            }
            //Nesta versão das velocidades nas fronteiras já não há o problema da anterior, pois calculo as velocidades em t+1
            // e não centradas no tempo

            else if  ((BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) &&  (j > 0) && (j < GridColumns)) //Eastern sea boundary
            {

               UVelocity[i * (GridColumns + 1) + j + 1] =   (
            			                               				UVelocity[i * (GridColumns + 1) + j] *
                                                      			(BoxDepth[index - 1] + BoxDepth[index]) / 2.0 -
                                                      			(DynamicHeight[index] - PastHeight[index]) *
                                                         		ASpatialStep / (ATimeStep/ 2.0)
                                                   			) / BoxDepth[index];
            }
            else if  ((BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) && (j < GridColumns)) //Western sea boundary
            {
               UVelocity[i * (GridColumns + 1) + j] =   		(
            			                               				UVelocity[i * (GridColumns + 1) + j + 1] *
                                                      			(BoxDepth[index + 1] + BoxDepth[index]) / 2.0 +
                                                      			(DynamicHeight[index] - PastHeight[index]) *
                                                         		ASpatialStep / (ATimeStep/ 2.0)
                                                   			) / BoxDepth[index];

            }
         }
      }
   }
}






double TBiDimensionalIntertidal::UPStreamUVelocity(int i, int j) //Calcula o Ui,j-1/2           ok
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
       	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::UPStreamUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::UPStreamUVelocity - alert" << endl;
#endif  // __BORLANDC__
	else if (j > 0) //Implica que a caixa n?o constitu? limite da malha a oeste
   {
		if (
      		//(BoxDepth[index - 1] < CriticalDepthForLandBoundary) ||
            (! BoxArray[index - 1].Dummybool2) ||
         	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4)
         ) //Ligação com terra a oeste
   		//return UVelocityPast[i * (GridColumns + 1) + j] / 2.0;
         return 0.0;
   	else
   		return (
         			UVelocityPast[i * (GridColumns + 1) + j - 1] +
                  UVelocityPast[i * (GridColumns + 1) + j]
                ) / 2.0;
   }
   else if (j == 0)  //Implica que a caixa constituí limite da malha a oeste
   {
   	if (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4)
      	return 0.0;
      else
      	return UVelocityPast[i * (GridColumns + 1) + j];//No caso de a fronteira ser liquída devolver a velocidade da própria caixa
   }
   return 0.0;
}


double TBiDimensionalIntertidal::UPStreamVVelocity(int i, int j)  //Calcula o Vi-1/2,j         ok
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::UPStreamVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::UPStreamVVelocity - alert" << endl;
#endif  // __BORLANDC__
	else if (i > 0) //Implica que a caixa n?o constitu? limite da malha a sul
   {
		if(
      		//(BoxDepth[indexV] < CriticalDepthForLandBoundary) || //Fronteira com terra
            (! BoxArray[indexV].Dummybool2) ||   //Fronteira com terra
            (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4)
        )
        //return VVelocityPast[index] / 2.0;
        return 0.0;
   	else
        return (
                  VVelocityPast[indexV] +
         			VVelocityPast[index]
                ) / 2.0;
   }
   else if (i == 0)  //Implica que a caixa constituí limite da malha a sul
   {
   	if (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4)
        return 0.0;
      else
        return VVelocityPast[index];//No caso de a fronteira ser liquída devolver a velocidade da própria caixa
   }
   return 0.0;
}


double TBiDimensionalIntertidal::DOWNStreamUVelocity(int i, int j)  //Calcula o Ui,j+1/2          ok
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::DOWNStreamUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::DOWNStreamUVelocity - alert" << endl;
#endif  // __BORLANDC__
	else if (j < GridColumns - 1)
   {
      if(
      		//(BoxDepth[index + 1] < CriticalDepthForLandBoundary) || //Fronteira com terra
            (! BoxArray[index + 1].Dummybool2) ||
            (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 4)
        )
   		return UVelocityPast[i * (GridColumns + 1) + j] / 2.0;
   	else
   		return (
         			UVelocityPast[i * (GridColumns + 1) + j] +
                  UVelocityPast[i * (GridColumns + 1) + j + 1]
                ) / 2.0;
   }
   else if (j == GridColumns - 1)
   {
   	/*if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 4)
      	return UVelocityPast[i * (GridColumns + 1) + j];
      else */
			return (
         			UVelocityPast[i * (GridColumns + 1) + j] +
                  UVelocityPast[i * (GridColumns + 1) + j + 1]
                ) / 2.0;
   }
   return 0.0;
}


double TBiDimensionalIntertidal::DOWNStreamVVelocity(int i, int j)  //Calcula o Vi+1/2,j            ok
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::DownStreamVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::DownStreamVVelocity - alert" << endl;
#endif  // __BORLANDC__
	else
   if (i < GridLines - 1)
   {
		if (
      		 //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary) ||
            (! BoxArray[(i + 1) * GridColumns + j].Dummybool2) ||
            (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 4)
         )
         return VVelocityPast[index] / 2.0;
   	else
   		return (
                  VVelocityPast[index] +
         			VVelocityPast[(i + 1) * GridColumns + j]
                ) / 2.0;
   }
   if (i == GridLines - 1)
   {
   	/*if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 4)
      	return VVelocityPast[index];
      else*/
   		return (
      				VVelocityPast[index] +
               	VVelocityPast[(i + 1) * GridColumns + j]
             	 ) / 2.0;
   }
   return 0.0;
}


double TBiDimensionalIntertidal::UpperUVelocity(int i, int j)   //Calcula o Ui+1/2,j    ok
{
   if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::UpperUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::UpperUVelocity - alert" << endl;
#endif  // __BORLANDC__
	else
   {
		if (
      		(i == GridLines - 1) ||
            (
            	(j <= GridColumns) &&		//Modificado a 19/2/2004
            	(i < GridLines - 1) &&
               //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary)
               (! BoxArray[(i + 1) * GridColumns + j].Dummybool2) || (!BoxArray[(i + 1) * GridColumns + j - 1].Dummybool2)		// Modificado a 19/2/2004
            )              /*      ||
           (                                                                                                                    // Modificado a 19/2/2004
            	(j == GridColumns) &&
            	(i < GridLines - 1) &&
               //(BoxDepth[(i + 1) * GridColumns + j - 1] < CriticalDepthForLandBoundary)
               (! BoxArray[(i + 1) * GridColumns + j - 1].Dummybool2)
            )  */
         )
         return UVelocityPast[i * (GridColumns + 1) + j]; //Se a superfície lateral da parede for < 10% da superfície horizontal
                                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).
   	else
   		return (
                  UVelocityPast[i * (GridColumns + 1) + j] +
         			UVelocityPast[(i + 1) * (GridColumns + 1) + j]
                ) / 2.0;
   }
   return 0.0;
}



double TBiDimensionalIntertidal::LowerUVelocity(int i, int j)  //Calcula o Ui-1/2,j      ok
{
    int indexV = (i - 1) * GridColumns + j;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::LowerUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::LowerUVelocity - alert" << endl;
#endif  // __BORLANDC__
      Debugger(i); Debugger(j);
   }
	else
   {
		if (
      		(i == 0) ||
            (
            	(j <= GridColumns) &&                      //Modificado a 19/2/2004
            	(i > 0) &&
               //(BoxDepth[indexV] < CriticalDepthForLandBoundary) //Fronteira com terra
               ((! BoxArray[indexV].Dummybool2) ||  (! BoxArray[indexV - 1].Dummybool2))       //Modificado a 19/2/2004
            )       /* ||                                                                                                            //Modificado a 19/2/2004
            (
            	(j == GridColumns)&&
            	(i > 0) &&
               //(BoxDepth[indexV - 1] < CriticalDepthForLandBoundary) //Fronteira com terra
               (! BoxArray[indexV - 1].Dummybool2)
            )  */
         )

      		return UVelocityPast[i * (GridColumns + 1) + j];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                            //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                            //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                            //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                            //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   		else
        		return (
                  	UVelocityPast[(i - 1) * (GridColumns + 1) + j] +
         				UVelocityPast[i * (GridColumns + 1) + j]
                	  ) / 2.0;
   }
   return 0.0;
}


double TBiDimensionalIntertidal::LeftVVelocity(int i, int j)   //Calcula o Vi,j-1/2        ok
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i > GridLines) || (j < 0) || (j >= GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::LeftVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::LeftVVelocity - alert" << endl;
#endif  // __BORLANDC__
      //Debugger(i); Debugger(j);
   }
	else
   {
		if (
      		(j == 0) ||
            (
            	(i <= GridLines) &&              //Modificado a 19/2/2004
            	(j > 0) &&
               //(BoxDepth[index - 1] < CriticalDepthForLandBoundary)
               ((! BoxArray[index - 1].Dummybool2) ||  (! BoxArray[(i - 1) * GridColumns + j - 1].Dummybool2))    //Modificado a 19/2/2004
            )      /*  ||                                                                                                       //Modificado a 19/2/2004
            (
            	(i == GridLines) &&
            	(j > 0) &&
               //(BoxDepth[(i - 1) * GridColumns + j - 1] < CriticalDepthForLandBoundary)
               (! BoxArray[(i - 1) * GridColumns + j - 1].Dummybool2)
            )  */

         ) //Ligação com terra a oeste
   		return VVelocityPast[index];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   	else
      {
   		return (
         			VVelocityPast[index - 1] +
                  VVelocityPast[index]
                ) / 2.0;
      }
   }
   return 0.0;
}


double TBiDimensionalIntertidal::RightVVelocity(int i, int j) //Calcula o Vi,j+1/2          ok
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

	if ((i < 0) || (i > GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::RightVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::RightVVelocity - alert" << endl;
#endif  // __BORLANDC__
	else
   {
      if(
      		(j == GridColumns - 1) ||
            (
            	(i <= GridLines) &&           //Modificado a 19/2/2004
      	   	    (i < GridColumns - 1) &&
               //(BoxDepth[index + 1] < CriticalDepthForLandBoundary)
               ((! BoxArray[index + 1].Dummybool2) || (! BoxArray[indexV + 1].Dummybool2))   //Modificado a 19/2/2004
            )                 /*     ||                                                                                       //Modificado a 19/2/2004
            (
            	(i == GridLines) &&
      	   	    (i < GridColumns - 1) &&
               //(BoxDepth[indexV + 1] < CriticalDepthForLandBoundary)
               (! BoxArray[indexV + 1].Dummybool2)
            )    */
        )
   		return VVelocityPast[index] ;//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                     //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                     //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                     //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                     //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   	else
   		return (
         			VVelocityPast[index] +
                  VVelocityPast[index + 1]
                ) / 2.0;
   }
   return 0.0;
}




double TBiDimensionalIntertidal::LowerLeftCornerUVelocity(int i, int j)  //Ui-1/2,j-1/2   ok
{
	//Debugger((LowerUVelocity(i,j-1) + LowerUVelocity(i,j)) / 2);
	return (LowerUVelocity(i,j-1) + LowerUVelocity(i,j)) / 2.0;
}

double TBiDimensionalIntertidal::UpperLeftCornerUVelocity(int i, int j)  //Ui+1/2,j-1/2    ok
{
	//Debugger((UpperUVelocity(i,j-1) + UpperUVelocity(i,j)) / 2);
	return (UpperUVelocity(i,j-1) + UpperUVelocity(i,j)) / 2.0;
}

double TBiDimensionalIntertidal::LowerRightCornerUVelocity(int i, int j) //Ui-1/2,j+1/2   ok
{
	//Debugger((LowerUVelocity(i,j) + LowerUVelocity(i,j+1)) / 2);
	return (LowerUVelocity(i,j) + LowerUVelocity(i,j+1)) / 2.0;
}

double TBiDimensionalIntertidal::UpperRightCornerUVelocity(int i, int j) //Ui+1/2,j+1/2     ok
{
	//Debugger((UpperUVelocity(i,j) + UpperUVelocity(i,j+1)) / 2);
	return (UpperUVelocity(i,j) + UpperUVelocity(i,j+1)) / 2.0;
}

double TBiDimensionalIntertidal::LowerLeftCornerVVelocity(int i, int j) //Vi-1/2,j-1/2       ok
{
	//Debugger((LeftVVelocity(i-1,j) + LeftVVelocity(i,j)) / 2);
	return (LeftVVelocity(i-1,j) + LeftVVelocity(i,j)) / 2.0;
}

double TBiDimensionalIntertidal::UpperLeftCornerVVelocity(int i, int j) //Vi+1/2,j-1/2         ok
{
	//Debugger((LeftVVelocity(i+1,j) + LeftVVelocity(i,j)) / 2.0);
	return (LeftVVelocity(i+1,j) + LeftVVelocity(i,j)) / 2.0;
}

double TBiDimensionalIntertidal::LowerRightCornerVVelocity(int i, int j) //Vi-1/2,j+1/2       ok
{
	//Debugger((RightVVelocity(i-1,j) + RightVVelocity(i,j)) / 2.0);
	return (RightVVelocity(i-1,j) + RightVVelocity(i,j)) / 2.0;
}

double TBiDimensionalIntertidal::UpperRightCornerVVelocity(int i, int j) //Vi-1/2,j+1/2        ok
{
	//Debugger((RightVVelocity(i,j) + RightVVelocity(i+1,j)) / 2.0);
	return (RightVVelocity(i,j) + RightVVelocity(i+1,j)) / 2.0;
}


double TBiDimensionalIntertidal::UpperLeftCornerDepth(int i, int j)      //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
         	 (
            	(Line >= 0) && (Line < GridLines)
             )
             &&
             (
            	(Column >= 0) && (Column < GridColumns)
             )
             &&
             (
               (BoxDepth[Line * GridColumns + Column] >= CriticalDepthForLandBoundary)
               //(BoxArray[Line * GridColumns + Column].Dummybool2)
             )
            )
            {
            	SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TBiDimensionalIntertidal::UpperRightCornerDepth(int i, int j)        //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Line * GridColumns + Column] >= CriticalDepthForLandBoundary)
                  //(BoxArray[Line * GridColumns + Column].Dummybool2)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TBiDimensionalIntertidal::LowerLeftCornerDepth(int i, int j)          //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
   	for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Line * GridColumns + Column] >= CriticalDepthForLandBoundary)
                  //(BoxArray[Line * GridColumns + Column].Dummybool2)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TBiDimensionalIntertidal::LowerRightCornerDepth(int i, int j)     //ok
{
	double SumOfSurroundingDepths, NumberOfParcels;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Line * GridColumns + Column] >= CriticalDepthForLandBoundary)
                  //(BoxArray[Line * GridColumns + Column].Dummybool2)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Line * GridColumns + Column];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   {
   	//Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
   }
}






double TBiDimensionalIntertidal::VCoriolis(int i, int j)
{
  int index = i * GridColumns + j;
	double VVelocityComponent = 0.0, NumberOfParcels = 2.0;

   VVelocityComponent = VVelocityComponent + VVelocity[index] + VVelocity[(i + 1) * GridColumns + j ];
   if ((j > 0) && (BoxArray[index - 1].Dummybool2)) //(BoxDepth[index - 1] >= CriticalDepthForLandBoundary))
   {
   	VVelocityComponent = VVelocityComponent + VVelocity[index - 1];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocity[(i + 1) * GridColumns + j - 1 ];
   	NumberOfParcels++;
   }

	return ATimeStep * Coriolis ( MyLatitude ) * VVelocityComponent / (2.0 * NumberOfParcels);
}


double TBiDimensionalIntertidal::UCoriolis(int i, int j)
{
	double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocity[i * (GridColumns + 1) + j ] + UVelocity[i * (GridColumns + 1) + j + 1];
   if ((i > 0) && (BoxArray[(i - 1) * GridColumns + j].Dummybool2)) //(BoxDepth[(i - 1) * GridColumns + j] >= CriticalDepthForLandBoundary))
   {
   	UVelocityComponent = UVelocityComponent + UVelocity[(i - 1) * (GridColumns + 1) + j];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocity[(i - 1) * (GridColumns + 1) + j + 1 ];
   	NumberOfParcels++;
   }

	return ATimeStep * Coriolis ( MyLatitude ) * UVelocityComponent / (2.0 * NumberOfParcels);
}




double TBiDimensionalIntertidal::UVerticalDiffusion(int i, int j)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;
  int indexVN = (i + 1) * GridColumns + j;

	double NorthernFlux, SouthernFlux;

   //Southern flux
   if (
   		(i > 0) &&
         //(BoxDepth[indexV] > CriticalDepthForLandBoundary)  &&
         (BoxArray[indexV].Dummybool2) &&
      	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)  &&
         (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         //(BoxDepth[indexV - 1] > CriticalDepthForLandBoundary)               //Nova
         (BoxArray[indexV - 1].Dummybool2)
 		)
   {
      SouthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(LowerLeftCornerDepth(i,j), LowerUVelocity(i, j))*
                    (
                    	UVelocityPast[i * (GridColumns + 1) + j] -
               			UVelocityPast[(i - 1) * (GridColumns + 1) + j]
                    ) / ASpatialStep;
   }
   else
   	SouthernFlux = 0.0;

   //Northern flux
   if	 (
   	 	(i < GridLines -  1) &&
         //(BoxDepth[indexVN] > CriticalDepthForLandBoundary) &&
         (BoxArray[indexVN].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxArray[indexVN].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         //(BoxDepth[indexVN - 1] > CriticalDepthForLandBoundary)                    //Nova
         (BoxArray[indexVN - 1].Dummybool2)
       )

       NorthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(UpperLeftCornerDepth(i,j), UpperUVelocity(i, j))*
                    (
             			UVelocityPast[(i + 1) * (GridColumns + 1) + j] -
               			UVelocityPast[i * (GridColumns + 1) + j]
                    ) / ASpatialStep;
   else
   	NorthernFlux = 0.0;

   return ATimeStep / 2.0 * (NorthernFlux - SouthernFlux) /  ASpatialStep;

}

double TBiDimensionalIntertidal::UHorizontalDiffusion(int i, int j)
{
  int index = i * GridColumns + j;
	double WesternFlux, EasternFlux;

      //Western flux
      if (
      		(
      			(j == 1) && //(j > 0) &&
               (BoxArray[index - 1].Dummybool2) &&
            	(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4)
         	) ||
            (
            	(j >= 2) &&
               (BoxArray[index - 1].Dummybool2) &&
      			(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
               (BoxArray[index - 2].Dummybool2) &&
         		(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4)
            )
         )
      	WesternFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(BoxDepth[index - 1], UPStreamUVelocity(i, j))*
                       (
                       		UVelocityPast[i * (GridColumns + 1) + j] -
                           UVelocityPast[i * (GridColumns + 1) + j - 1]
                       ) / ASpatialStep;
      else
      	WesternFlux = 0.0;

      //EasternFlux

      if (
   			(
   				(j < GridColumns - 1) &&
         		//(BoxDepth[index + 1] > CriticalDepthForLandBoundary) &&
               (BoxArray[index + 1].Dummybool2) &&
      			(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4)
      		)  ||
         	(
         		(j == GridColumns - 1) &&
            	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4)
         	)
      	)
      	EasternFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(BoxDepth[index], DOWNStreamUVelocity(i, j))*
                       (
                      		UVelocityPast[i * (GridColumns + 1) + j + 1] -
                        	UVelocityPast[i * (GridColumns + 1) + j]
                      	) / ASpatialStep;
      else
      	EasternFlux = 0.0;

      return  ATimeStep / 2.0 * (EasternFlux - WesternFlux) /  ASpatialStep;
}


double TBiDimensionalIntertidal::VVerticalDiffusion(int i, int j)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

	double NorthernFlux, SouthernFlux;

   //Southern flux
   if (
   		(
   			(i == 1) &&    //(i > 0) &&
         	//(BoxDepth[indexV] > CriticalDepthForLandBoundary) &&
            (BoxArray[indexV].Dummybool2) &&
      		(BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)  //mudado index para indexV
 			)   ||
      	(
      		(i >= 2) &&
         	//(BoxDepth[indexV] > CriticalDepthForLandBoundary) &&
            (BoxArray[indexV].Dummybool2) &&
      		(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
         	//(BoxDepth[(i - 2) * GridColumns + j] > CriticalDepthForLandBoundary) &&
            (BoxArray[(i - 2) * GridColumns + j].Dummybool2)  &&
         	(BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)
      	)
   	)

      SouthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(BoxDepth[indexV], UPStreamVVelocity(i, j))*
                    (
             				VVelocityPast[index] -
               			VVelocityPast[indexV]
                    ) / ASpatialStep;
   else
   	SouthernFlux = 0.0;

   //Northern flux
   if (
   		(
   			(i < GridLines - 1) &&
         	//(BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary) &&
            (BoxArray[(i + 1) * GridColumns + j].Dummybool2) &&
      		(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4)
      	)  ||
         (
         	(i == GridLines - 1) &&
            (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4)
         )
      )
      NorthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(BoxDepth[index], DOWNStreamVVelocity(i, j))*
                    (
             				VVelocityPast[(i + 1) * GridColumns + j] -
               			VVelocityPast[index]
                    ) / ASpatialStep;
   else
   	NorthernFlux = 0.0;

   return ATimeStep / 2.0 * (NorthernFlux - SouthernFlux) /  ASpatialStep;
}


double TBiDimensionalIntertidal::VHorizontalDiffusion(int i, int j)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;
    double WesternFlux, EasternFlux;

      //Western flux
      if (
      		(j > 0) &&
      		//(BoxDepth[index - 1] >= CriticalDepthForLandBoundary) &&
            (BoxArray[index - 1].Dummybool2) &&
            (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
            (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
            //(BoxDepth[indexV - 1] >= CriticalDepthForLandBoundary)                        //Nova
            (BoxArray[indexV - 1].Dummybool2)
         )
      	WesternFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(LowerLeftCornerDepth(i,j), LeftVVelocity(i, j))*
                       (
                       		VVelocityPast[index] -
                           VVelocityPast[index - 1]
                       ) / ASpatialStep;
      else
      	WesternFlux = 0.0;

      //EasternFlux
      if (
      		(j < GridColumns - 1) &&
      		(
            	//(BoxDepth[index + 1] > CriticalDepthForLandBoundary) &&
               (BoxArray[index + 1].Dummybool2) &&
            	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4) &&
               (BoxArray[index + 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
               //(BoxDepth[indexV + 1] >= CriticalDepthForLandBoundary)                       //Nova
               (BoxArray[indexV + 1].Dummybool2)
             )
         )
         EasternFlux = HorizontalDiffCoef *
                        CorrectionOfHorizontalDiffCoef(LowerRightCornerDepth(i,j), RightVVelocity(i, j))*
                       (
                      		VVelocityPast[index + 1] -
                        	VVelocityPast[index]
                      	) / ASpatialStep;

      else
      	EasternFlux = 0.0;

      return  ATimeStep / 2.0 * (EasternFlux - WesternFlux) /  ASpatialStep;
}









double TBiDimensionalIntertidal::UVAdvectionOfUEquation(int i, int j)
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	    MessageBox(0,
					  "TBiDimensionalIntertidal::UVAdvectionOfUEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::VUAdvectionOfUEquation - alert" << endl;
#endif  // __BORLANDC__

    if ((j > 0) && ((BoxDepth[index - 1] <= MinDepthForAdvection) || (BoxDepth[index] <= MinDepthForAdvection)))
    return 0.0;

	double VelocityComponent;
   VelocityComponent = 0.0;
   double MyUpperLeftCornerVVelocity, MLowerLeftCornerVVelocity;
   //MyUpperLeftCornerVVelocity = (VVelocityPast[(i + 1) * GridColumns + j] + VVelocityPast[(i + 1) * GridColumns + j - 1]) / 2.0;
   //MLowerLeftCornerVVelocity = (VVelocityPast[i * GridColumns + j] + VVelocityPast[i * GridColumns + j - 1]) / 2.0;

   MyUpperLeftCornerVVelocity = (VVelocity[(i + 1) * GridColumns + j] + VVelocity[(i + 1) * GridColumns + j - 1]) / 2.0;
   MLowerLeftCornerVVelocity = (VVelocity[i * GridColumns + j] + VVelocity[i * GridColumns + j - 1]) / 2.0;


   //if (i < GridLines - 1)
   	VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j) *
      	                 (MyUpperLeftCornerVVelocity + fabs(MyUpperLeftCornerVVelocity)) *
         	              UVelocityPast[i * (GridColumns + 1) + j]);

   //if ((i == 8) && (j == 1) && (MyPIntegrate->GetCurrentTime() > 0.0)) Debugger(1000);
   //if ((i == 8) && (j == 1) && (MyPIntegrate->GetCurrentTime() > 0.0)) Debugger(VelocityComponent);

   if (i > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MLowerLeftCornerVVelocity + fabs(MLowerLeftCornerVVelocity)) *
                          UVelocityPast[(i - 1) * (GridColumns + 1) + j]);

   //if ((i == 8) && (j == 1) && (MyPIntegrate->GetCurrentTime() > 0.0)) Debugger(VelocityComponent);

   if (i < GridLines - 1)
   	VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j) *
                          (MyUpperLeftCornerVVelocity - fabs(MyUpperLeftCornerVVelocity)) *
                          UVelocityPast[(i + 1) * (GridColumns + 1) + j]);

   //if ((i == 8) && (j == 1) && (MyPIntegrate->GetCurrentTime() > 0.0)) Debugger(VelocityComponent);

   //if (i > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MLowerLeftCornerVVelocity - fabs(MLowerLeftCornerVVelocity)) *
                          UVelocityPast[i * (GridColumns + 1) + j]);

   //if ((i == 8) && (j == 1) && (MyPIntegrate->GetCurrentTime() > 0.0)) Debugger(VelocityComponent);
   /*if ((i == 8) && (j == 1) && (MyPIntegrate->GetCurrentTime() > 0.0))
   {
   	Debugger(LowerLeftCornerDepth(i,j));
      Debugger(MLowerLeftCornerVVelocity);
      Debugger(UVelocityPast[i * (GridColumns + 1) + j]);
   } */

   return ATimeStep / (2.0 * (BoxDepth[index] +  BoxDepth[index - 1])) *
          VelocityComponent / ASpatialStep;
}

double TBiDimensionalIntertidal::VUAdvectionOfExplicitUEquation(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	    MessageBox(0,
					  "TBiDimensionalIntertidal::VUAdvectionOfExplicitUEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::VUAdvectionOfExplicitUEquation - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent;
   VelocityComponent = AUpperLeftDepth * (AUpperLeftVVelocity + fabs(AUpperLeftVVelocity)) /
                       ASpatialStep * UVelocityPast[i * (GridColumns + 1) + j ];
   if (i > 0)
   	VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftVVelocity + fabs(ALowerLeftVVelocity)) /
                       		ASpatialStep * UVelocityPast[(i - 1) * (GridColumns + 1) + j ];
   if (i < GridLines - 1)
   	VelocityComponent = VelocityComponent + AUpperLeftDepth * (AUpperLeftVVelocity - fabs(AUpperLeftVVelocity)) /
                          ASpatialStep * UVelocityPast[(i + 1) * (GridColumns + 1) + j ];

   VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftVVelocity - fabs(ALowerLeftVVelocity)) /
                       ASpatialStep * UVelocityPast[i * (GridColumns + 1) + j ];
   return VelocityComponent;
}




double TBiDimensionalIntertidal::GravityOfRightSideOfUEquationOfMotion(int i, int j)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
       	MessageBox(0,
					  "TBiDimensionalIntertidal::GravityOfRightSideOfUEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::GravityOfRightSideOfUEquationOfMotion - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent1, VelocityComponent2, VelocityComponent3, VelocityComponent4,
          NumberOfparcels;

   VelocityComponent2 = 0.0;
   VelocityComponent3 = 0.0;
   VelocityComponent4 = 0.0;
   NumberOfparcels = 0.0;

   //VelocityComponent1
   VelocityComponent1 = - Gravity * PastHeight[index] *
                        ATimeStep / (2.0 * ASpatialStep);

   //VelocityComponent2
   VelocityComponent2 = VelocityComponent2 +
                        (-BoxDepth[index] * VVelocityPast[index]);


   NumberOfparcels++;
   VelocityComponent2 = VelocityComponent2 +
                        (BoxDepth[index] * VVelocityPast[(i + 1) * GridColumns + j]);


   NumberOfparcels++;
   if ((i < GridLines - 1) && (BoxArray[(i + 1) * GridColumns + j].Dummybool2)) //(BoxDepth[(i + 1) * GridColumns + j] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (BoxDepth[(i + 1) * GridColumns + j] * VVelocityPast[(i + 1) * GridColumns + j]);

      NumberOfparcels++;
   }
   if ((i > 0) && (BoxArray[indexV].Dummybool2)) //(BoxDepth[indexV]) >= CriticalDepthForLandBoundary)
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (-BoxDepth[indexV] * VVelocityPast[index]);

      NumberOfparcels++;
   }
   VelocityComponent2 =  Gravity * (ATimeStep * ATimeStep) * VelocityComponent2 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);

   //VelocityComponent3
   if ((j > 0) && (BoxArray[index - 1].Dummybool2)) //(BoxDepth[index - 1] >= CriticalDepthForLandBoundary))
   	VelocityComponent3 = Gravity * PastHeight[index - 1] *
                           ATimeStep / (2.0 * ASpatialStep);

   //VelocityComponent4
   NumberOfparcels = 0.0;
   if ((j > 0) && (BoxArray[index - 1].Dummybool2)) //(BoxDepth[index - 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (-BoxDepth[index - 1] * VVelocityPast[(i + 1) * GridColumns + j - 1]);


   	NumberOfparcels++;
      VelocityComponent4 = VelocityComponent4 +
      	                  (BoxDepth[index - 1] * VVelocityPast[index - 1]);


      NumberOfparcels++;
   }
   if ((i < GridLines - 1) && (j > 0) && (BoxArray[(i + 1) * GridColumns + j - 1].Dummybool2)) //(BoxDepth[(i + 1) * GridColumns + j - 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (-BoxDepth[(i + 1) * GridColumns + j - 1] * VVelocityPast[(i + 1) * GridColumns + j - 1]);


   	NumberOfparcels++;
   }
   if ((i > 0) && (j > 0) && (BoxArray[indexV - 1].Dummybool2)) //(BoxDepth[indexV - 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
                           (BoxDepth[indexV - 1] * VVelocityPast[index - 1]);

      NumberOfparcels++;
   }
   VelocityComponent4 = Gravity * (ATimeStep * ATimeStep) * VelocityComponent4 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);

   return VelocityComponent1 + VelocityComponent2 + VelocityComponent3 + VelocityComponent4;
}





double TBiDimensionalIntertidal::UVAdvectionOfVEquation(int i, int j)
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "TBiDimensionalIntertidal::UVAdvectionOfVEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::UVAdvectionOfVEquation - alert" << endl;
#endif  // __BORLANDC__

    if ((i > 0) && ((BoxDepth[(i - 1) * GridColumns + j] <= MinDepthForAdvection) || (BoxDepth[index] <= MinDepthForAdvection)))
    return 0.0;

	double VelocityComponent, MyLowerRightCornerUVelocity /*i-1/2, j+1/2 */, MyLowerLeftCornerUVelocity /*i-1/2, j-1/2 */;
   VelocityComponent = 0.0;

   //MyLowerRightCornerUVelocity = (UVelocityPast[i * (GridColumns + 1) + j + 1] + UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1]) / 2.0;
   //MyLowerLeftCornerUVelocity =  (UVelocityPast[i * (GridColumns + 1) + j] + UVelocityPast[(i - 1) * (GridColumns + 1) + j]) / 2.0;

   MyLowerRightCornerUVelocity = (UVelocity[i * (GridColumns + 1) + j + 1] + UVelocity[(i - 1) * (GridColumns + 1) + j + 1]) / 2.0;
   MyLowerLeftCornerUVelocity =  (UVelocity[i * (GridColumns + 1) + j] + UVelocity[(i - 1) * (GridColumns + 1) + j]) / 2.0;

   VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j) *
                       (MyLowerRightCornerUVelocity + fabs(MyLowerRightCornerUVelocity)) *
                       VVelocityPast[index]);

   if (j > 0)
   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MyLowerLeftCornerUVelocity + fabs(MyLowerLeftCornerUVelocity)) *
                          VVelocityPast[index - 1]);

   if (j < GridColumns - 1)
   	VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j) *
                       		(MyLowerRightCornerUVelocity - fabs(MyLowerRightCornerUVelocity)) *
                        	VVelocityPast[index + 1]);

   	VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j) *
                          (MyLowerLeftCornerUVelocity - fabs(MyLowerLeftCornerUVelocity)) *
                          VVelocityPast[index]);

   return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j])) *
          VelocityComponent / ASpatialStep;
}


double TBiDimensionalIntertidal::UVAdvectionOfExplicitVEquation(int i, int j)
{
  int index = i * GridColumns + j;

	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
       	MessageBox(0,
					  "TBiDimensionalIntertidal::UVAdvectionOfExplicitVEquation",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::UVAdvectionOfExplicitVEquation - alert" << endl;
#endif  // __BORLANDC__

	double VelocityComponent;

   VelocityComponent = ALowerRightDepth * (ALowerRightUVelocity + fabs(ALowerRightUVelocity)) /
                       ASpatialStep * VVelocityPast[index];
   if (j > 0)
   	VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftUVelocity + fabs(ALowerLeftUVelocity)) /
                       	  ASpatialStep * VVelocityPast[index - 1];

   if (j < GridColumns - 1)
      VelocityComponent = VelocityComponent + ALowerRightDepth * (ALowerRightUVelocity - fabs(ALowerRightUVelocity)) /
                          ASpatialStep * VVelocityPast[index + 1];

   VelocityComponent = VelocityComponent - ALowerLeftDepth * (ALowerLeftUVelocity - fabs(ALowerLeftUVelocity)) /
                        ASpatialStep * VVelocityPast[index];

   return VelocityComponent;
}


double TBiDimensionalIntertidal::GravityOfRightSideOfVEquationOfMotion(int i, int j)
{
	if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1))
#ifdef __BORLANDC__
       	MessageBox(0,  "TBiDimensionalIntertidal::GravityOfRightSideOfVEquationOfMotion",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalIntertidal::GravityOfRightSideOfVEquationOfMotion - alert" << endl;
#endif  // __BORLANDC__

    int index = i * GridColumns + j;
    int indexV = (i - 1) * GridColumns + j;
	double VelocityComponent1, VelocityComponent2, VelocityComponent3, VelocityComponent4,
          NumberOfparcels;

   VelocityComponent2 = 0.0;
   VelocityComponent3 = 0.0;
   VelocityComponent4 = 0.0;
   NumberOfparcels = 0.0;

   //VelocityComponent1
   VelocityComponent1 = - Gravity * PastHeight[index] *
                        ATimeStep / (2.0 * ASpatialStep);

   /*if (((i == 1) || (i == 33)) && (j == 25) && (MyPIntegrate->GetCurrentTime() > 0.010))
   {
   	Debugger(VelocityComponent1 * pow(10,15));
      Debugger(PastHeight[index] * pow(10,5));
   } */


   //VelocityComponent2
   VelocityComponent2 = VelocityComponent2 +
                        (-BoxDepth[index] * UVelocityPast[i * (GridColumns + 1) + j]);

   NumberOfparcels++;
   VelocityComponent2 = VelocityComponent2 +
                        (BoxDepth[index] * UVelocityPast[i * (GridColumns + 1) + j + 1]);


   NumberOfparcels++;
   if ((j < GridColumns - 1) && (BoxArray[index + 1].Dummybool2))//(BoxDepth[index + 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (BoxDepth[index + 1] * UVelocityPast[i * (GridColumns + 1) + j + 1]);
      NumberOfparcels++;
   }
   if ((j > 0) && (BoxArray[index - 1].Dummybool2))//(BoxDepth[index - 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent2 = VelocityComponent2 +
                           (-BoxDepth[index - 1] * UVelocityPast[i * (GridColumns + 1) + j]);
      NumberOfparcels++;
   }

   VelocityComponent2 = Gravity * (ATimeStep * ATimeStep) * VelocityComponent2 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);

   //VelocityComponent3
   if ((i > 0) && (BoxArray[indexV].Dummybool2))//(BoxDepth[indexV] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent3 = Gravity * PastHeight[indexV] *
                           ATimeStep / (2.0 * ASpatialStep);

   }

   //VelocityComponent4
   NumberOfparcels = 0.0;
   if ((i > 0) && (BoxArray[indexV].Dummybool2))//(BoxDepth[indexV] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (-BoxDepth[indexV] * UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1]);

   	NumberOfparcels++;
      VelocityComponent4 = VelocityComponent4 +
      	                  (BoxDepth[indexV] * UVelocityPast[(i - 1) * (GridColumns + 1) + j]);

      NumberOfparcels++;

   }
   if ((i > 0) && (j < GridColumns - 1) && (BoxArray[indexV + 1].Dummybool2))//(BoxDepth[indexV + 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
                           (-BoxDepth[indexV + 1] * UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1]);
      NumberOfparcels++;
   }
   if ((i > 0) && (j > 0) && (BoxArray[indexV - 1].Dummybool2))//(BoxDepth[indexV - 1] >= CriticalDepthForLandBoundary))
   {
   	VelocityComponent4 = VelocityComponent4 +
      	                  (BoxDepth[indexV - 1] * UVelocityPast[(i - 1) * (GridColumns + 1) + j]);
   	NumberOfparcels++;
   }

   VelocityComponent4 = Gravity * (ATimeStep * ATimeStep) * VelocityComponent4 /
                        (2.0 * ASpatialStep * ASpatialStep * NumberOfparcels);


   return VelocityComponent1 + VelocityComponent2 + VelocityComponent3 + VelocityComponent4;
}


double TBiDimensionalIntertidal::UForRugosity(int i, int j)
{
  int index = i * GridColumns + j;

	double VVelocityComponent = 0.0, NumberOfParcels = 2.0;

   VVelocityComponent = VVelocityComponent + VVelocity[index] + VVelocity[(i + 1) * GridColumns + j ];
   if ((j > 0) && (BoxArray[index - 1].Dummybool2))//(BoxDepth[index - 1] >= CriticalDepthForLandBoundary))
   {
   	VVelocityComponent = VVelocityComponent + VVelocity[index - 1];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocity[(i + 1) * GridColumns + j - 1 ];
   	NumberOfParcels++;
   }

	return sqrt(
   					sqr(UVelocityPast[i * (GridColumns + 1) + j ]) +
   		 			sqr(
                  		VVelocityComponent / NumberOfParcels
                  	)
          		);
}

double TBiDimensionalIntertidal::VForRugosity(int i, int j)
{
    int indexV = (i - 1) * GridColumns + j;
    int indexV_1 = (i - 1) * (GridColumns + 1) + j;
    int indexF = i * (GridColumns + 1) + j;

	double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocity[indexF] + UVelocity[indexF + 1];
   if ((i > 0) && (BoxArray[indexV].Dummybool2))//(BoxDepth[indexV] >= CriticalDepthForLandBoundary))
   {
   	UVelocityComponent = UVelocityComponent + UVelocity[indexV_1];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocity[indexV_1 + 1 ];
   	NumberOfParcels++;
   }

	return sqrt(
   					sqr(VVelocityPast[i * GridColumns + j]) +
   		 			sqr(
                  		UVelocityComponent / NumberOfParcels
                  	)
          		);
}


double TBiDimensionalIntertidal::aUCalculation (int i, int j)
{
  int index = i * GridColumns + j;
	// Cálculo do a
   if (
   		(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4) ||//Left neighbour cell is a western land boundary
         (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) ||    //Cell is a river boundary
         (
         	(j >= 2) &&
            (! BoxArray[index - 2].Dummybool2)
         ) ||
         (
         	(j >= 2) &&
            (
            	(
         			(BoxDepth[index - 1] <= CriticalDepthForLandBoundary) &&
            		(BoxDepth[index - 2] <= CriticalDepthForLandBoundary)
            	) ||
            	(
            		(BoxDepth[index - 1] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[index - 2] < DynamicHeight[index - 1])
         		) ||
            	(
            		(BoxDepth[index - 2] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[index - 1] < DynamicHeight[index - 2])
         		)
            )
         )
   	)
      	return (0.0);
      else
      if (
      		(BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) || //Left neighbour cell is a western ocean boundary
            (BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)    //Left neighbour cell is a western river boundary
         )
      {
      	return (-  UAdvectionOfUEquation(i,j,2) -
                    HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[index - 1], UPStreamUVelocity(i, j))
                    / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                    GravityOfUEquation(i,j,2));
      }
      else  //Left neighbour cell is a "normal" cell
      {
      	return (- UAdvectionOfUEquation(i,j,2) -
                   HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[index - 1], UPStreamUVelocity(i, j))
                   / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                   GravityOfUEquation(i,j,3));
      }
}

double TBiDimensionalIntertidal::bUCalculation(int i, int j)
{
  int index = i * GridColumns + j;

	// Cálculo do b
	if (
         (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) //||
      )
   	return (1.0);
   else
   	return 1 +
            UAdvectionOfUEquation(i,j,3) +
            DragOfUEquation(i,j)+
            HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index - 1] + BoxDepth[index]),
                                                                0.5*(UPStreamUVelocity(i, j) + DOWNStreamUVelocity(i,j)))
            / (ASpatialStep * ASpatialStep) * ATimeStep +
            GravityOfUEquation(i,j,4);
}

double TBiDimensionalIntertidal::cUCalculation(int i, int j)
{
  int index = i * GridColumns + j;

	//Cálculo do c - termo correspondente à velocidade na célula seguinte
	if (
         (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 4) ||
         (
         	(j < GridColumns - 1) &&
            //(BoxDepth[index + 1] < CriticalDepthForLandBoundary)
            (! BoxArray[index + 1].Dummybool2)
         ) ||
         (
         	(j < GridColumns - 1) &&
            (
            	(
         			(BoxDepth[index] <= CriticalDepthForLandBoundary) &&
            		(BoxDepth[index + 1] < CriticalDepthForLandBoundary)
            	) ||
            	(
         			(BoxDepth[index] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[index + 1] < DynamicHeight[index])
            	)  ||
            	(
         			(BoxDepth[index + 1] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[index] < DynamicHeight[index + 1])
            	)
            )
         )
   	)
   	return(0.0);
   else
   if (
   		(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) ||//Cell is a eastern ocean boundary
         (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)   //Cell is a eastern river boundary
   	)
      	return(UAdvectionOfUEquation(i,j,4) -
            HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[index], DOWNStreamUVelocity(i, j))
            / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
            GravityOfUEquation(i,j,5));
         else
         if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 0)    //"Normal" cell
         	return (UAdvectionOfUEquation(i,j,4) -
                    HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[index], DOWNStreamUVelocity(i, j))
                    / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                    GravityOfUEquation(i,j,6));
   	return(0.0);
}

double TBiDimensionalIntertidal::aVCalculation(int i, int j)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

	// Cálculo do a
   if (
   		(BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4) || //South neighbour cell is a southern land boundary
         (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||
         (
         	(i >= 2) &&
            //(BoxDepth[(i - 2) * GridColumns + j] < CriticalDepthForLandBoundary)
            (! BoxArray[(i - 2) * GridColumns + j].Dummybool2)
         )  ||
         (
         	(i >= 2) &&
            (
            	(
         			(BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
            		(BoxDepth[(i - 2) * GridColumns + j] <= CriticalDepthForLandBoundary)
            	) ||
            	(
         			(BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[(i - 2) * GridColumns + j] < DynamicHeight[indexV])
            	) ||
            	(
         			(BoxDepth[(i - 2) * GridColumns + j] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[indexV] < DynamicHeight[(i - 2) * GridColumns + j])
            	)
            )
         )
   	)
      	return (0.0);
      else
      if (
      		(BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) ||//South neighbour cell is a southern ocean boundary
            (BoxArray[indexV].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)   //South neighbour cell is a southern river boundary
         )
      {
      	return -VAdvectionOfVEquation(i,j,2) -
                HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[indexV], UPStreamVVelocity(i, j))
                / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                GravityOfVEquation(i,j,2);
      }
      else  //South neighbour cells are "normal" cells
         return -VAdvectionOfVEquation(i,j,2) -
                HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[indexV], UPStreamVVelocity(i, j))
                / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                GravityOfVEquation(i,j,3);

}


double TBiDimensionalIntertidal::bVCalculation (int i, int j)
{
  int index = i * GridColumns + j;

	// Cálculo do b
	if (
   		//(BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) || //South neighbour cell is a southern river boundary
         (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) /*||
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) *///Cell is a northern river boundary
      )
   	return (1.0);
   else
   	return 1 +
             VAdvectionOfVEquation(i,j,3) +
             DragOfVEquation(i,j)+
             HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[index]),
                                                                 0.5*(UPStreamVVelocity(i, j) + DOWNStreamVVelocity(i,j)))
             / (ASpatialStep * ASpatialStep) * ATimeStep +
             GravityOfVEquation(i,j,4);
}


double TBiDimensionalIntertidal::cVCalculation(int i, int j)
{
  int index = i * GridColumns + j;

	//Cálculo do c - termo correspondente à velocidade na célula seguinte
	if (
   		//(BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         //(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||    //Cell is connected to a northern river boundary
         (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 4) ||   //Cell is connected to a northern land boundary
      	(
      		(i < GridLines - 1) &&
         	//(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary)
            (! BoxArray[(i + 1) * GridColumns + j].Dummybool2)
      	)  ||
         (
         	(i < GridLines - 1) &&
            (
            	(
         			(BoxDepth[index] <= CriticalDepthForLandBoundary) &&
            		(BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepthForLandBoundary)
            	) ||
            	(
         			(BoxDepth[index] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[(i + 1) * GridColumns + j] < DynamicHeight[index])
            	) ||
            	(
         			(BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepthForLandBoundary) &&
            		(DynamicHeight[index] < DynamicHeight[(i + 1) * GridColumns + j])
            	)
            )
         )
      )
   {
   	return(0.0);
   }
   else
   if (
   		(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) ||//Cell is a northern ocean boundary
         (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)   //Cell is a northern river boundary

         /*(i == GridLines - 1) ||                                                       //Grid limit
			(
         	(i < GridLines - 1) &&
            (BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary) //Cell is connected to a northern land node
         )    */
   	)
      	return(VAdvectionOfVEquation(i,j,4) -
                HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[index], DOWNStreamVVelocity(i, j))
                / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                GravityOfVEquation(i,j,5));

   else
   if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 0)    //"Normal" cell
   		return(VAdvectionOfVEquation(i,j,4) -
                HorizontalDiffCoef * CorrectionOfHorizontalDiffCoef(BoxDepth[index], DOWNStreamVVelocity(i, j))
                / (ASpatialStep * ASpatialStep) * ATimeStep / 2.0 -
                GravityOfVEquation(i,j,6));
   	return(0.0);
}



double TBiDimensionalIntertidal::WesternComponentOfContinuity(int i, int j)
{
  int index = i * GridColumns + j;

	//Western flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((j == 0) ||
   		((j > 0) &&
         //(BoxDepth[index - 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index - 1].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
         	     UVelocity[i * (GridColumns + 1) + j]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
         	            (
            	         	BoxDepth[index] +
               	         BoxDepth[index - 1]
                  	   ) *
                     	UVelocity[i * (GridColumns + 1) + j]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((j == 0) ||
      	((j > 0) &&
         //(BoxDepth[index - 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index - 1].Dummybool2))
      	)
      {
      	return (ATimeStep * BoxDepth[index ] *
                  UVelocityPast[i * (GridColumns + 1) + j ]);
      }
      else
      {
      	return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index ] +
                        	     	 	BoxDepth[index - 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j ]);
      }
	}
   	return(0.0);
}

double TBiDimensionalIntertidal::EasternComponentOfContinuity(int i, int j)
{
  int index = i * GridColumns + j;

	//Eastern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((j == GridColumns - 1)  ||
   		((j < GridColumns - 1) &&
         //(BoxDepth[index + 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index + 1].Dummybool2))
   		)
   	{
   		return (ATimeStep * BoxDepth[index ] *
         	     UVelocity[i * (GridColumns + 1) + j + 1]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
         	           (
            	        		BoxDepth[index ] +
               	         BoxDepth[index + 1]
                  	  ) *
                    	UVelocity[i * (GridColumns + 1) + j + 1]);
   	}
    }
    else
    {
    	if ((j == GridColumns - 1)  ||
      	((j < GridColumns - 1) &&
         //(BoxDepth[index  + 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index + 1].Dummybool2))
         )
      {
      	return (ATimeStep * BoxDepth[index] *
                 UVelocityPast[i * (GridColumns + 1) + j + 1]);
      }
      else
      {
      	return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	      	BoxDepth[index + 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j + 1]);
      }
    }
}

double TBiDimensionalIntertidal::NorthernComponentOfContinuity(int i, int j)
{
  int index = i * GridColumns + j;

	//Northern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) &&
         //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary))
         (! BoxArray[(i + 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                     VVelocityPast[(i + 1) * GridColumns + j]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[(i + 1) * GridColumns + j]
                     ) *
                     VVelocityPast[(i + 1) * GridColumns + j]);
   	}
   }
   else
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) &&
         //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary))
         (! BoxArray[(i + 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                     VVelocity[(i + 1) * GridColumns + j]);
   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[(i + 1) * GridColumns + j]
                     ) *
                     VVelocity[(i + 1) * GridColumns + j]);
   	}

   }
}

double TBiDimensionalIntertidal::SouthernComponentOfContinuity(int i, int j)
{
  int index = i * GridColumns + j;

	//Southern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((i == 0) ||
      	((i > 0) &&
         //(BoxDepth[(i - 1) * GridColumns + j ] < CriticalDepthForLandBoundary))
         (! BoxArray[(i - 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index ] *
                     VVelocityPast[index ]);

   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[(i - 1) * GridColumns + j] +
                        BoxDepth[index ]
                     ) *
                     VVelocityPast[index]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((i == 0) ||
      	((i > 0) &&
         //(BoxDepth[(i - 1) * GridColumns + j ] < CriticalDepthForLandBoundary))
         (! BoxArray[(i - 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index ] *
                     VVelocity[index ]);

   	}
   	else
   	{
   		return (0.5 * ATimeStep *
                     (
                     	BoxDepth[(i - 1) * GridColumns + j] +
                        BoxDepth[index ]
                     ) *
                     VVelocity[index]);
   	}
   }
   	return(0.0);
}




double TBiDimensionalIntertidal::WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Western flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((j == 0) ||
   		((j > 0) &&
         //(BoxDepth[index - 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index - 1].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
         	     UVelocity[i * (GridColumns + 1) + j] *
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (UVelocity[i * (GridColumns + 1) + j] > 0.0)
   			return (0.5 * ATimeStep *
         	            (
            	         	BoxDepth[index] +
               	         BoxDepth[index - 1]
                  	   ) *
                     	UVelocity[i * (GridColumns + 1) + j] *
                        PastGeneric[index - 1]);
         else
           return (0.5 * ATimeStep *
         	            (
            	         	BoxDepth[index] +
               	         BoxDepth[index - 1]
                  	   ) *
                     	UVelocity[i * (GridColumns + 1) + j] *
                        PastGeneric[index]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((j == 0) ||
      	((j > 0) &&
         //(BoxDepth[index - 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index - 1].Dummybool2))
      	)
      {
      	return (ATimeStep * BoxDepth[index ] *
                  UVelocityPast[i * (GridColumns + 1) + j ] *
                 PastGeneric[index]);
      }
      else
      {
      	if (UVelocityPast[i * (GridColumns + 1) + j] > 0.0)
      		return (0.5 * ATimeStep *
                       (
                       		BoxDepth[index ] +
                           BoxDepth[index - 1]
                       ) *
                       UVelocityPast[i * (GridColumns + 1) + j ]*
                       PastGeneric[index - 1]);
         else
           return (0.5 * ATimeStep *
                       (
                       		BoxDepth[index ] +
                           BoxDepth[index - 1]
                       ) *
                       UVelocityPast[i * (GridColumns + 1) + j ]*
                       PastGeneric[index]);
      }
	}
   	return(0.0);
}

double TBiDimensionalIntertidal::EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Eastern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((j == GridColumns - 1)  ||
   		((j < GridColumns - 1) &&
         //(BoxDepth[index + 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index + 1].Dummybool2))
   		)
   	{
   		return (ATimeStep * BoxDepth[index ] *
         	     UVelocity[i * (GridColumns + 1) + j + 1] *
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (UVelocity[i * (GridColumns + 1) + j + 1] > 0.0)
   			return (0.5 * ATimeStep *
         	           (
            	        		BoxDepth[index ] +
               	         BoxDepth[index + 1]
                  	  ) *
                    	UVelocity[i * (GridColumns + 1) + j + 1]*
                 		PastGeneric[index]);
         else
         	return (0.5 * ATimeStep *
         	           (
            	        		BoxDepth[index ] +
               	         BoxDepth[index + 1]
                  	  ) *
                    	UVelocity[i * (GridColumns + 1) + j + 1]*
                 		PastGeneric[index + 1]);
   	}
   }
   else
   {
    	if ((j == GridColumns - 1)  ||
      	((j < GridColumns - 1) &&
         //(BoxDepth[index  + 1] < CriticalDepthForLandBoundary))
         (! BoxArray[index + 1].Dummybool2))
         )
      {
      	return (ATimeStep * BoxDepth[index] *
                 UVelocityPast[i * (GridColumns + 1) + j + 1]*
                 PastGeneric[index]);
      }
      else
      {
      	if (UVelocityPast[i * (GridColumns + 1) + j + 1] > 0.0)
      		return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	      	BoxDepth[index + 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j + 1]*
                 		            PastGeneric[index]);
         else
         	return (0.5 * ATimeStep *
                           	  (
                                		BoxDepth[index] +
                        	      	BoxDepth[index + 1]
                     	         ) *
                  	            UVelocityPast[i * (GridColumns + 1) + j + 1]*
                 		            PastGeneric[index + 1]);
      }
   }
}

double TBiDimensionalIntertidal::NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Northern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) &&
         //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary))
         (! BoxArray[(i + 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                 VVelocityPast[(i + 1) * GridColumns + j] *
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (VVelocityPast[(i + 1) * GridColumns + j] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[(i + 1) * GridColumns + j]
                     ) *
                     VVelocityPast[(i + 1) * GridColumns + j] *
                     PastGeneric[index]);
         else
         	return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[(i + 1) * GridColumns + j]
                     ) *
                     VVelocityPast[(i + 1) * GridColumns + j] *
                     PastGeneric[(i + 1) * GridColumns + j]);
   	}
   }
   else
   {
   	if ((i == GridLines - 1)  ||
   		((i < GridLines - 1) &&
         //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary))
         (! BoxArray[(i + 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index] *
                 VVelocity[(i + 1) * GridColumns + j]*
                 PastGeneric[index]);
   	}
   	else
   	{
      	if (VVelocity[(i + 1) * GridColumns + j] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[(i + 1) * GridColumns + j]
                     ) *
                     VVelocity[(i + 1) * GridColumns + j] *
                     PastGeneric[index]);
         else
            return (0.5 * ATimeStep *
                     (
                     	BoxDepth[index] +
                        BoxDepth[(i + 1) * GridColumns + j]
                     ) *
                     VVelocity[(i + 1) * GridColumns + j] *
                     PastGeneric[(i + 1) * GridColumns + j]);
   	}

   }
}

double TBiDimensionalIntertidal::SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Southern flow
   if (SemiTimeStep == 1) //First semi timestep
   {
		if ((i == 0) ||
      	((i > 0) &&
         //(BoxDepth[(i - 1) * GridColumns + j ] < CriticalDepthForLandBoundary))
         (! BoxArray[(i - 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index ] *
                 VVelocityPast[index ] *
                 PastGeneric[index ]);

   	}
   	else
   	{
      	if (VVelocityPast[index] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[(i - 1) * GridColumns + j] +
                        BoxDepth[index ]
                     ) *
                     VVelocityPast[index] *
                     PastGeneric[(i - 1) * GridColumns + j ]);
         else
         	return (0.5 * ATimeStep *
                     (
                     	BoxDepth[(i - 1) * GridColumns + j] +
                        BoxDepth[index ]
                     ) *
                     VVelocityPast[index] *
                     PastGeneric[index ]);
   	}
   }
   else
   if (SemiTimeStep == 2) //Second semi timestep
   {
   	if ((i == 0) ||
      	((i > 0) &&
         //(BoxDepth[(i - 1) * GridColumns + j ] < CriticalDepthForLandBoundary))
         (! BoxArray[(i - 1) * GridColumns + j].Dummybool2))
      	)
   	{
   		return (ATimeStep * BoxDepth[index ] *
                 VVelocity[index ] *
                 PastGeneric[index ]);

   	}
   	else
   	{
      	if (VVelocity[index ] > 0.0)
   			return (0.5 * ATimeStep *
                     (
                     	BoxDepth[(i - 1) * GridColumns + j] +
                        BoxDepth[index ]
                     ) *
                     VVelocity[index]*
                     PastGeneric[(i - 1) * GridColumns + j ]);
         else
         	return (0.5 * ATimeStep *
                     (
                     	BoxDepth[(i - 1) * GridColumns + j] +
                        BoxDepth[index ]
                     ) *
                     VVelocity[index]*
                     PastGeneric[index ]);
   	}
   }
   	return(0.0);
}


double TBiDimensionalIntertidal::SaltWesternHorizontalDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;
   if (
   		(j > 0) &&
         //(BoxDepth[index] >= CriticalDepthForLandBoundary)
         (BoxArray[index].Dummybool2)&&
         //(BoxDepth[index - 1] >= CriticalDepthForLandBoundary)
         (BoxArray[index - 1].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
   	)
   	return //-ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
             -0.5 * ATimeStep *
             (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index - 1]), UPStreamUVelocity(i, j)) *
      		(
                   BoxDepth[index] + BoxDepth[index - 1]
             	) *  PastGeneric[index ] -
             	//MyUpStreamPastConcentration -
                HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index - 1]), UPStreamUVelocity(i, j)) *
      		(
                   BoxDepth[index] + BoxDepth[index - 1]
                ) * PastGeneric[index - 1]
                //MyCurrentBoxPastConcentration
             );
   else
   	return 0.0;

}

double TBiDimensionalIntertidal::SaltEasternHorizontalDiffusion(int i, int j, double* PastGeneric)
{
	/*double MyDownStreamPastConcentration, MyCurrentBoxPastConcentration;
   MyDownStreamPastConcentration = ADownStreamPastConcentration;
   MyCurrentBoxPastConcentration = ACurrentBoxPastConcentration;*/

  int index = i * GridColumns + j;

	if (
   		(j < GridColumns - 1) &&
         //(BoxDepth[index] >= CriticalDepthForLandBoundary)
         (BoxArray[index].Dummybool2) &&
         //(BoxDepth[index + 1] >= CriticalDepthForLandBoundary)
         (BoxArray[index + 1].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1)
   	)
   {

   	return //ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
              0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index + 1]), DOWNStreamUVelocity(i, j)) *
      		 	(
             		BoxDepth[index + 1] +
               	BoxDepth[index]
             	) *
                PastGeneric[index + 1] -
                HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[index + 1]), DOWNStreamUVelocity(i, j)) *
      		 	(
             		BoxDepth[index + 1] +
               	BoxDepth[index]
             	) *
               PastGeneric[index ]
             );
   }
   else
   	return 0.0;
}

double TBiDimensionalIntertidal::SaltNorthernVerticalDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;
	if (
   		(i < GridLines - 1) &&
         //(BoxDepth[index] >= CriticalDepthForLandBoundary)
         (BoxArray[index].Dummybool2) &&
         //(BoxDepth[(i + 1) * GridColumns + j] >= CriticalDepthForLandBoundary)
         (BoxArray[(i + 1) * GridColumns + j].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1)
   	)


   	return //ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
              0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]), DOWNStreamVVelocity(i, j)) *
      		 	(
             		BoxDepth[(i + 1) * GridColumns + j] +
               	BoxDepth[index]
             	) *
               PastGeneric[(i + 1) * GridColumns + j] -
             	//MyDownStreamPastConcentration -
               HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j]), DOWNStreamVVelocity(i, j)) *
      		 	(
             		BoxDepth[(i + 1) * GridColumns + j] +
               	BoxDepth[index]
             	) *
               PastGeneric[index ]
               //MyCurrentBoxPastConcentration
             );

   else
   	return 0.0;

}

double TBiDimensionalIntertidal::SaltSouthernVerticalDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	if (
   		(i > 0) &&
         //(BoxDepth[index] >= CriticalDepthForLandBoundary)
         (BoxArray[index].Dummybool2) &&
         //(BoxDepth[(i - 1) * GridColumns + j] >= CriticalDepthForLandBoundary)
         (BoxArray[(i - 1) * GridColumns + j].Dummybool2) &&
         (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1)
   	)
   	return //-ATimeStep / (2.0 * ASpatialStep * ASpatialStep) *
             -0.5 * ATimeStep *
      		 (
             	HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]), UPStreamVVelocity(i, j)) *
      		 	(
             		BoxDepth[index] +
               	BoxDepth[(i - 1) * GridColumns + j]
             	) *
               PastGeneric[index] -
             	//MyDownStreamPastConcentration -
               HorizontalDiffCoef *
                CorrectionOfHorizontalDiffCoef(0.5*(BoxDepth[index] + BoxDepth[(i - 1) * GridColumns + j]), UPStreamVVelocity(i, j)) *
      		 	(
             		BoxDepth[index] +
               	BoxDepth[(i - 1) * GridColumns + j]
             	) *
               PastGeneric[(i - 1) * GridColumns + j]
               //MyCurrentBoxPastConcentration
             );
   else
   	return 0.0;
}


double TBiDimensionalIntertidal::CoefRugosidade(int ABoxNumber)
{
   int MyBoxNumber;
   double MyDepth;
   double const MinDepth = 0.4; //m  Valor mínimo de profundidade para cálculo do coeficiente de rugosidade segundo o Ramiro Neves  28/1/2004
   MyBoxNumber = ABoxNumber;
   if (BoxDepth[MyBoxNumber] < 0.5) MyDepth = 0.5;
   else MyDepth = BoxDepth[MyBoxNumber];
   return(Gravity * pow(CoefManning[MyBoxNumber],2.0)) / pow(MAX(MyDepth, MinDepth),0.333333);
   //return 0.004;
}

void TBiDimensionalIntertidal::Reinitialize()
{
	double MyWaterTemperature;
    double const MinCritDepth = 0.01;  //0.00001 m

    EnableReinitializeFromGo = false;   // AP, 2005.09.16 - moved to here
    EnableReinitializeFromIntegrate = false;

	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	for (int i = 0; i < GridLines; i++)
    {
   	for (int j = 0; j < GridColumns; j++)
      {
        int index = i * GridColumns + j;

      	atemporary[index] = 0.0; btemporary[index] = 0.0;
         ctemporary[index] = 0.0; rtemporary[index] = 0.0;
         //if (BoxArray[index].Dummybool2)
         //{

      		BoxDepth[index] = BoxDepth[index] +
                              (DynamicHeight[index] - PastHeight[index]);
           
         //}
         if ((BoxArray[index].Dummybool2)  && (BoxDepth[index] <= CriticalDepthForLandBoundary))
             BoxArray[index].Dummybool2 = false;

      }
   }
   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
         int index = i * GridColumns + j;

         if   (
                  (
                  (! BoxArray[index].Dummybool2) && (BoxDepth[index] >= MinCritDepth) &&  //Tive que mudar este valor de zero para 0.00001 pois havia casos
                  (                                                                       // em que a BoxDepth estava t?o pr?xima de zero que ocorria um floating point error
                  	(                                                                     //no AdvectDiffuse no c?lculo da concentra??o, quando divide pela soma
                     	(j < GridColumns - 1) &&                                          // da BoxDepth com a diferen?a entre a eleva??o presente e a eleva??o passada
                        //(DynamicHeight[index + 1] > BoxDepth[index]) &&
                        (DynamicHeight[index + 1] > DynamicHeight[index]) &&
                        (BoxDepth[index + 1] > CriticalDepthForLandBoundary)
                     ) ||
                     (
                     	(j > 0) &&
                        //(DynamicHeight[index - 1] > BoxDepth[index]) &&
                        (DynamicHeight[index - 1] > DynamicHeight[index]) &&
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
                     ) ||
                  	(
                  		(i < GridLines - 1)  &&
                        //(DynamicHeight[(i + 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i + 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)    ||
                  	(
                  		(i > 0) &&
                        //(DynamicHeight[(i - 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i - 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)
               	)
                )
                ||
                (! BoxArray[index].Dummybool2) && (BoxDepth[index] >= CriticalDepthForLandBoundary)
            	)
               BoxArray[index].Dummybool2 = true;

         if (
         		(!BoxArray[index].Dummybool2) ||
               (
               	(BoxArray[index].Dummybool2) &&
                  (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  (DynamicHeight[index] < PastHeight[index ])
               )
            )

         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            VVelocity[index] = 0.0;
            UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
            VVelocity[(i + 1) * GridColumns + j] = 0.0;
            //DynamicHeight[index] = BoxDepth[index ];
         }
      }
    }
   RiverForcing();


   for (int i = 0; i < NumberOfBoxes + MAX(GridLines,GridColumns); i++)
   {
      if (i < NumberOfBoxes + GridLines)
         UVelocityPast[i] = UVelocity[i];

      if (i < NumberOfBoxes + GridColumns)
         VVelocityPast[i] = VVelocity[i];

      if (i < NumberOfBoxes)
      {
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
        {
           MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

      	   WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
           WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
      }
   }
   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;

//   EnableReinitializeFromGo = false;
//   EnableReinitializeFromIntegrate = false;
}

/*
void TBiDimensionalIntertidal::Reinitialize()
{
	double MyWaterTemperature;
    double const MinCritDepth = 0.01;  //0.00001 m
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        int index = i * GridColumns + j;

      	atemporary[index] = 0.0; btemporary[index] = 0.0;
         ctemporary[index] = 0.0; rtemporary[index] = 0.0;
         if (BoxArray[index].Dummybool2)
         {
      		BoxDepth[index] = BoxDepth[index] +
                              (DynamicHeight[index] - PastHeight[index]);
         }
         if ((BoxArray[index].Dummybool2)  && (BoxDepth[index] <= CriticalDepthForLandBoundary))
             BoxArray[index].Dummybool2 = false;

      }
   }
   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
         int index = i * GridColumns + j;

         if   (
                  (! BoxArray[index].Dummybool2) && (BoxDepth[index] >= MinCritDepth) &&  //Tive que mudar este valor de zero para 0.00001 pois havia casos
                  (                                                                       // em que a BoxDepth estava t?o pr?xima de zero que ocorria um floating point error
                  	(                                                                     //no AdvectDiffuse no c?lculo da concentra??o, quando divide pela soma
                     	(j < GridColumns - 1) &&                                          // da BoxDepth com a diferen?a entre a eleva??o presente e a eleva??o passada
                        //(DynamicHeight[index + 1] > BoxDepth[index]) &&
                        (DynamicHeight[index + 1] > DynamicHeight[index]) &&
                        (BoxDepth[index + 1] > CriticalDepthForLandBoundary)
                     ) ||
                     (
                     	(j > 0) &&
                        //(DynamicHeight[index - 1] > BoxDepth[index]) &&
                        (DynamicHeight[index - 1] > DynamicHeight[index]) &&
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
                     ) ||
                  	(
                  		(i < GridLines - 1)  &&
                        //(DynamicHeight[(i + 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i + 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)    ||
                  	(
                  		(i > 0) &&
                        //(DynamicHeight[(i - 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i - 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)
               	)
            	)
               BoxArray[index].Dummybool2 = true;

         if (
         		(!BoxArray[index].Dummybool2) ||
               (
               	(BoxArray[index].Dummybool2) &&
                  (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  (DynamicHeight[index] < PastHeight[index ])
               )
            )

         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            VVelocity[index] = 0.0;
            UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
            VVelocity[(i + 1) * GridColumns + j] = 0.0;
            //DynamicHeight[index] = BoxDepth[index ];
         }
      }
    }
   RiverForcing();

   if (SemiTimeStep == 2)
   {
   	for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   	{
   		VVelocityPast[i] = VVelocity[i];
   	}

      for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   	{
   		UVelocityPast[i] = UVelocity[i];
   	}

   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
			{
				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

      		WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
      		WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   	}
   }
   else
   if (SemiTimeStep == 1)
   {
   	for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   	{
   		UVelocityPast[i] = UVelocity[i];
   	}

      for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   	{
   		VVelocityPast[i] = VVelocity[i];
   	}

   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
			{
				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

            WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
      		WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   	}

   }

   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;

}
*/

void TBiDimensionalIntertidal::DepthCorrection()
{
	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        int index = i * GridColumns + j;
       	if (
         		(BoxDepth[index ] <= CriticalDepthForLandBoundary) &&
               (DynamicHeight[index ] <  PastHeight[index ]) &&
               (BoxArray[index].Dummybool2)
            )
         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            VVelocity[index] = 0.0;
            UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
            VVelocity[(i + 1) * GridColumns + j] = 0.0;
            DynamicHeight[index] = BoxDepth[index ];
            BoxArray[index].Dummybool2 = false;
         }
      }
   }
   Go(); //Recursive call
}



double TBiDimensionalIntertidal::UAdvectionOfUEquation(int i, int j, int k)
{
  int index = i * GridColumns + j;
  int indexU = i * (GridColumns + 1) + j;

    if ((j > 0) && ((BoxDepth[index - 1] <= MinDepthForAdvection) || (BoxDepth[index] <= MinDepthForAdvection)))
	    return 0.0;

	if (k == 1)
		return ATimeStep /
          (
          	2.0 * (BoxDepth[index ] + BoxDepth[index - 1])
          ) *
          (
          	BoxDepth[index] *
            (ADOWNStreamU + fabs(ADOWNStreamU)) / ASpatialStep *
            UVelocityPast[indexU] -
            BoxDepth[index - 1] *
            (AUPStreamU + fabs(AUPStreamU)) / ASpatialStep *
            UVelocityPast[indexU - 1] +
            BoxDepth[index] *
            (ADOWNStreamU - fabs(ADOWNStreamU)) / ASpatialStep *
            UVelocityPast[indexU + 1] -
            BoxDepth[index - 1] *
            (AUPStreamU - fabs(AUPStreamU)) / ASpatialStep *
            UVelocityPast[indexU]
          );
    else if (k == 2)
    return 	ATimeStep / (2.0 * (BoxDepth[index ] + BoxDepth[index - 1])) *
            BoxDepth[index - 1] *
            (AUPStreamU + fabs(AUPStreamU)) / ASpatialStep;
    else if (k == 3)
    return  ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[index - 1])) *
            (
            	BoxDepth[index] *
               (ADOWNStreamU + fabs(ADOWNStreamU)) / ASpatialStep -
               BoxDepth[index - 1] *
               (AUPStreamU - fabs(AUPStreamU)) / ASpatialStep
            );
    else if (k == 4)
    return  ATimeStep /
            (2.0 * (BoxDepth[index] + BoxDepth[index - 1])) *
            BoxDepth[index] *
            (ADOWNStreamU - fabs(ADOWNStreamU)) /
            ASpatialStep;
    else 
#ifdef __BORLANDC__
       MessageBox(0, "Ilegal k argument in UAdvectionOfUEquation", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
      cerr << "TBiDimensionalIntertidal::UAdvectionOfUEquation - Illegal k argument" << endl;
#endif  // __BORLANDC__
   	return(0.0);
}

double TBiDimensionalIntertidal::GravityOfUEquation(int i, int j, int k)
{
  int index = i * GridColumns + j;

   if (k == 1)
	return ATimeStep /
          2.0 * Gravity *
          (PastHeight[index] - PastHeight[index - 1]) /
          ASpatialStep;
   else if (k == 2)
   return Gravity *
          (BoxDepth[index - 1]) *
          (ATimeStep * ATimeStep) /
          (4.0 * (ASpatialStep * ASpatialStep));
   else if (k == 3)
   return Gravity *
          (BoxDepth[index - 1] + BoxDepth[index - 2]) *
          (ATimeStep * ATimeStep) /
          (8.0 * (ASpatialStep * ASpatialStep));
   else if (k == 4)
   return Gravity *
          (BoxDepth[index] +BoxDepth[index - 1]) *
          (ATimeStep * ATimeStep) /
          (4.0 * (ASpatialStep * ASpatialStep));
   else if (k == 5)
   return Gravity *
         (BoxDepth[index]) *
         (ATimeStep * ATimeStep) /
         (4.0 * (ASpatialStep * ASpatialStep));
   else if (k == 6)
   return Gravity *
          (BoxDepth[index] + BoxDepth[index + 1]) *
          (ATimeStep * ATimeStep) /
          (8.0 * (ASpatialStep * ASpatialStep));
   else 
#ifdef __BORLANDC__
       MessageBox(0, "Ilegal k argument in GravityOfUEquation", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
      cerr << "TBiDimensionalIntertidal::GravityOfUEquation - Illegal k argument" << endl;
#endif  // __BORLANDC__
   	return(0.0);
}

double TBiDimensionalIntertidal::DragOfUEquation(int i, int j)
{
  int index = i * GridColumns + j;

	return ATimeStep /
          (2.0 * (BoxDepth[index] + BoxDepth[index - 1])) *
                       		(
                       				CoefRugosidade(index) +
                   	 				CoefRugosidade(index - 1)
                       		) *
                  	  		UForRugosity(i,j);
}


double TBiDimensionalIntertidal::VAdvectionOfVEquation(int i, int j, int k)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

    if ((i > 0) && ((BoxDepth[indexV] <= MinDepthForAdvection) || (BoxDepth[index] <= MinDepthForAdvection)))
    return 0.0;

	if (k == 1)
	return ATimeStep /
          (2.0 * (BoxDepth[index] + BoxDepth[indexV])) *
                     	(
                        	BoxDepth[index] *
                          	(ADOWNStreamV + fabs(ADOWNStreamV)) / ASpatialStep *
                          	VVelocityPast[index] -
                          	BoxDepth[indexV] *
                          	(AUPStreamV + fabs(AUPStreamV)) / ASpatialStep *
                          	VVelocityPast[indexV] +
                          	BoxDepth[index] *
                          	(ADOWNStreamV - fabs(ADOWNStreamV)) / ASpatialStep *
                          	VVelocityPast[(i + 1) * GridColumns + j ] -
                          	BoxDepth[indexV] *
                          	(AUPStreamV - fabs(AUPStreamV)) / ASpatialStep *
                          	VVelocityPast[index]
                     	);
   else if (k == 2)
   return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[indexV])) *
          BoxDepth[indexV] *
          (AUPStreamV + fabs(AUPStreamV)) / ASpatialStep;
   else if (k == 3)
   return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[indexV])) *
          (
             BoxDepth[index] *
             (ADOWNStreamV + fabs(ADOWNStreamV)) / ASpatialStep -
             BoxDepth[indexV] *
             (AUPStreamV - fabs(AUPStreamV)) / ASpatialStep
          );
   else if (k == 4)
   return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[indexV])) *
          BoxDepth[index] *
          (ADOWNStreamV - fabs(ADOWNStreamV)) / ASpatialStep;
   else 
#ifdef __BORLANDC__
      MessageBox(0, "Illegal k argument in VAdvectionOfVEquation", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
      cerr << "TBiDimensionalIntertidal::VAdvectionOfVEquation - Illegal k argument" << endl;
#endif  // __BORLANDC__
   return 0.0;
}

double TBiDimensionalIntertidal::GravityOfVEquation(int i, int j, int k)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

   if (k == 1)
	return ATimeStep /
          2.0 * Gravity * (PastHeight[index] -PastHeight[indexV]) /
          ASpatialStep;
   else if (k == 2)
   return Gravity * (BoxDepth[indexV]) *
          (ATimeStep * ATimeStep) /
          (4.0 * (ASpatialStep * ASpatialStep));
   else if (k == 3)
   return Gravity * (BoxDepth[indexV] + BoxDepth[(i - 2) * GridColumns + j]) *
          (ATimeStep * ATimeStep) /
          (8.0 * (ASpatialStep * ASpatialStep));
   else if (k == 4)
   return Gravity * (BoxDepth[index] + BoxDepth[indexV]) *
          (ATimeStep * ATimeStep) /
          (4.0 * (ASpatialStep * ASpatialStep));
   else if (k == 5)
   return Gravity * (BoxDepth[index]) *
          (ATimeStep * ATimeStep) /
          (4.0 * (ASpatialStep * ASpatialStep));
   else if (k == 6)
   return Gravity * (BoxDepth[index] + BoxDepth[(i + 1) * GridColumns + j ]) *
          (ATimeStep * ATimeStep) /
          (8.0 * (ASpatialStep * ASpatialStep));
   else 
#ifdef __BORLANDC__
      MessageBox(0, "Illegal k argument in GravityOfVEquation", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
      cerr << "TBiDimensionalIntertidal::GravityOfVEquation - Illegal k argument" << endl;
#endif  // __BORLANDC__
   return 0.0;
}

double TBiDimensionalIntertidal::DragOfVEquation(int i, int j)
{
  int index = i * GridColumns + j;
  int indexV = (i - 1) * GridColumns + j;

	return ATimeStep / (2.0 * (BoxDepth[index] + BoxDepth[indexV])) *
                     	(
                       			CoefRugosidade(index) +
                   	 			CoefRugosidade(indexV)
                     	) *
                  	  	VForRugosity(i,j);
}



// One dimensional hydrodynamic model object CPP code file
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

// One dimensional explicit object
// Public functions
//
// Constructors invoked outside EcoDyn shell...
//

TOneDimensionalDIExplicit::TOneDimensionalDIExplicit(char* className, float timeStep,
        char* morphologyFilename)
            :TOneDimensionalDITransport(className, timeStep, morphologyFilename)
{
}

TOneDimensionalDIExplicit::TOneDimensionalDIExplicit(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TOneDimensionalDITransport(className, timeStep, nLines, nColumns, aDepth,
                      aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
}
// ...

TOneDimensionalDIExplicit::TOneDimensionalDIExplicit(TEcoDynClass* PEcoDynClass, char* className)
                            :TOneDimensionalDITransport(PEcoDynClass, className)
{
}



void TOneDimensionalDIExplicit::Go()
{
	int i;

    Continuity();

    if (CalculateEvaporation)
        Evaporate();

    for (i = 0; i < NumberOfBoxes + 1; i++)
    {
   	    if (i == 0)
      	    BoxCrossSectionalArea[i] = BoxWidth[i] * BoxDepth[i];
        else if ((i > 0) && (i < NumberOfBoxes))
            BoxCrossSectionalArea[i] = BoxWidth[i] * (BoxDepth[i - 1] + BoxDepth[i]) / 2.0;
   	    else if (i == NumberOfBoxes)
   		    BoxCrossSectionalArea[i] = BoxDepth[i - 1] * BoxWidth[i];
    }

	EquationOfMotion();

    GenericRiver = RiverSalt;
    GenericLoad = SaltLoad;

    AdvectDiffuse(Salt);

//  Salt = Generic;

	for (int i = 0; i < NumberOfBoxes + 1; i++)
	{
   	    if (i < NumberOfBoxes)
        {
      	    PastHeight[i] = DynamicHeight[i];
            PastBoxDepth[i] = BoxDepth[i];
        }
		UVelocity[i] = FlowBetweenBoxes[i] / BoxCrossSectionalArea[i];
        PastBoxCrossSectionalArea[i] = BoxCrossSectionalArea[i];
        PastFlowBetweenBoxes[i] = FlowBetweenBoxes[i];
	}
}


void TOneDimensionalDIExplicit::EquationOfMotion()
{
	double MeanVelocityBeforeBox, MeanVelocityAfterBox;
    double /*MyCurrentTime,*/ DamDischarge;

	TEcoDynClass* MyFlowPointer = MyPEcoDynClass->GetFlowPointer();

	if (MyFlowPointer != NULL)
	{
		for (int i = 0; i < NumberOfBoxes + 1; i++)// Fronteira fluvial
		{

			if (i == 0)  //Fronteira fluvial
			{
				MyFlowPointer->Inquiry(GetEcoDynClassName(), RiverFlow,
												   i,
												   "Hourly river flow",
												   ObjectCode);
                FlowBetweenBoxes[i] = RiverFlow;
            }
            else if ((i > 0) && (i < NumberOfBoxes)) //Caixas intermedias
			{
                HydraulicRadius = BoxCrossSectionalArea[i] /
									(2.0 * BoxWidth[i] + 2.0 * DynamicHeight[i]);


                MeanWidthBeforeBox = (BoxWidth[i - 1] + BoxWidth[i]) / 2.0;
				MeanWidthAfterBox = (BoxWidth[i] + BoxWidth[i + 1]) / 2.0;

                MeanVelocityBeforeBox = (PastFlowBetweenBoxes[i - 1] / PastBoxCrossSectionalArea[i - 1] +
                                     PastFlowBetweenBoxes[i] / PastBoxCrossSectionalArea[i]) / 2.0;

				MeanVelocityAfterBox = (PastFlowBetweenBoxes[i] / PastBoxCrossSectionalArea[i] +
                                     PastFlowBetweenBoxes[i + 1] / PastBoxCrossSectionalArea[i + 1]) / 2.0;


				if (MeanVelocityAfterBox > 0.0)
					AlfaB = -1.0;
				else
					AlfaB = 1.0;

                if (MeanVelocityBeforeBox > 0.0)
					AlfaA = -1.0;
				else
					AlfaA = 1.0;

				FlowBetweenBoxes[i] = PastFlowBetweenBoxes[i] - ATimeStep / BoxLength[i - 1]*
											 (
												MeanVelocityAfterBox * ((1 - AlfaB)/2 * PastFlowBetweenBoxes[i] +
																				(1 + AlfaB)/2 * PastFlowBetweenBoxes[i + 1]
																			  ) -
												MeanVelocityBeforeBox * ((1 - AlfaA)/2 * PastFlowBetweenBoxes[i - 1] +
                                                            (1 + AlfaA)/2 * PastFlowBetweenBoxes[i]
																				)
											 ) - ATimeStep / BoxLength[i - 1] * Gravity * BoxCrossSectionalArea[i] *
											 (DynamicHeight[i] - DynamicHeight[i - 1]) -
                                  ATimeStep * BoxWidth[i] * CoefRugosidade(i) * fabs(UVelocity[i]) * UVelocity[i];
			}
			else if (i == NumberOfBoxes) //Flow between the last box and the river
			{
				MyFlowPointer->Inquiry(GetEcoDynClassName(), DamDischarge,
												   i,
												  "Hourly dam discharge",
												   ObjectCode);
                FlowBetweenBoxes[i] = DamDischarge;
                UVelocity[i] = FlowBetweenBoxes[i] / BoxCrossSectionalArea[i];
			}
		}
	}
}

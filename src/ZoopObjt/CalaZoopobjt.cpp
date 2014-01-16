// Zooplankton object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.03.11
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "zoopobjt.h"

// TCalaZooplankton Class

// Cala do Norte
//
// Constructors invoked outside EcoDyn shell...
//

TCalaZooplankton::TCalaZooplankton(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double riverZoo, double oceanZoo,
        double aZooBiomass[], double aZooLoad[],
        int nParams, double kGraze, double phytoLowerLimit, double rmax,
        double deathLoss, double docLoss, double metabolism, double excretionLoss,
        double maximumDeathLoss, double kZooStarvedByPhyto, double q10Coefficient,
        double q10WaterTempReference, double q10KPhytoMass, double q10PhytoLowerLimit,
        double q10Rmax, double phytoCoefficient, double detritalCoefficient,
        double bacterialCoefficient, double assimilationEfficiency,
        double ammoniaFractionOfExcretion, double detritalFractionOfMortality,
        double redfieldCFactor, double redfieldNFactor, double redfieldPFactor)
    :TZooplanktonBasic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, riverZoo, oceanZoo, aZooBiomass, aZooLoad,
            nParams, kGraze, phytoLowerLimit, rmax, deathLoss, docLoss,
            metabolism, excretionLoss, maximumDeathLoss, kZooStarvedByPhyto,
            q10Coefficient, q10WaterTempReference, q10KPhytoMass,
            q10PhytoLowerLimit, q10Rmax, phytoCoefficient, detritalCoefficient,
            bacterialCoefficient, assimilationEfficiency,
            ammoniaFractionOfExcretion, detritalFractionOfMortality,
            redfieldCFactor, redfieldNFactor, redfieldPFactor)

{
}

TCalaZooplankton::TCalaZooplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TZooplanktonBasic(className, timeStep, morphologyFilename, variablesFilename,
                parametersFilename)
{
}
// ...

TCalaZooplankton::TCalaZooplankton(TEcoDynClass* APEcoDynClass, char* className)
							  : TZooplanktonBasic(APEcoDynClass, className)
{
}

void TCalaZooplankton::Graze()      //NG 6/6/96
{
	double PhytoMass;
	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{

		if (MyPhytoPointer != NULL)
		{
			MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoMass,
												 i,
												 "Phytoplankton biomass",
												 ObjectCode);

		//	 Debugger(PhytoMass);

			// Phytoplankton returns its mass as ug chl a l-1
			// convert to mg C m-3
			PhytoMass = PhytoMass * ChlorophyllToCarbon;
			// only graze if there is more than the lower limit of C


			ZooGrossProduction[i] = 0;
			//NG : GrossProduction stays at 0 if zooplankton doesn't graze 4/6/96
			// in  mg C m-3


			if (PhytoMass > Q10PhytoLowerLimit)
			{
				 // Grazing influenced by temperature : Baretta p. 90, 286...
				// for parameters and 316 for expression Q10
				// ration in mg/mg/day}
				double WaterColumnTemperature;
				TEcoDynClass* MyWaterTemperaturePointer
											= MyPEcoDynClass->GetWaterTemperaturePointer();


				if (MyWaterTemperaturePointer != NULL)

				{
					MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterColumnTemperature,
																		i,
																		"Water temperature",
																		ObjectCode);

					double Q10Zoo = 0;

		  //	Q10Zoo = 2 * exp((WaterColumnTemperature-12)/10);

					Q10Zoo =   Q10Coefficient
								* exp((WaterColumnTemperature-Q10WaterTempReference)/10);



					Ration =		Q10Rmax
								 * Q10Zoo
								 * ( (PhytoMass- Q10PhytoLowerLimit)
									 / ( Q10KPhytoMass + PhytoMass - Q10PhytoLowerLimit));

			/*		Ration = 	Q10Rmax
								 * Q10Zoo
								  * (1-exp(KGraze *(Q10PhytoLowerLimit - PhytoMass)));
			*/

				// ZooProduction in mg/m3/day  Fresh Weight ?

					ZooProd = ZooBiomass[i] * Ration; // /assimilation

					ZooGrossProduction[i] = ZooProd;
					// NG 4/6/96

				// phyto removal in ug C m-3   // NO!!! It is being removed as mg C m-3 I think! JGF 96.08.02
				

					PhytoMass = ZooProd
								 //	* CUBIC
									/ (DryWeightToFreshWeight
									* CarbonToDryWeight);


		  //		Debugger(PhytoMass);

					if (ZooProd > 0)
					{
					// return negative value because this is subtracted
						MyPhytoPointer->Update(GetEcoDynClassName(), -PhytoMass,
														i,
														"Phytoplankton biomass",
														ObjectCode);
						ZooProduction[i] = ZooProd;
					}
				}
			}

		}
	}
}


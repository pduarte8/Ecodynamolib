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

// TCarlZooplankton Class

// Carlingford zooplankton
//
// Constructors invoked outside EcoDyn shell...
//

TCarlZooplankton::TCarlZooplankton(char* className, float timeStep,
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

TCarlZooplankton::TCarlZooplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TZooplanktonBasic(className, timeStep, morphologyFilename, variablesFilename,
                parametersFilename)
{
}
// ...

TCarlZooplankton::TCarlZooplankton(TEcoDynClass* APEcoDynClass, char* className)
							  : TZooplanktonBasic(APEcoDynClass, className)
{
}

void TCarlZooplankton::Graze()
{
	double PhytoMass,
			 RmaxExp;
	// const double KRmax = 0.01;
	const double KRmax = 10;
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
			RmaxExp = Rmax * exp(-KRmax * PhytoMass);
			// RmaxExp = Rmax;
			// Phytoplankton returns its mass as ug chl a l-1
			// convert to mg C m-3 (ug C l-1)
			PhytoMass = PhytoMass * ChlorophyllToCarbon;
			// only graze if there is more than the lower limit of C


			ZooGrossProduction[i] = 0;
			//NG : GrossProduction stays at 0 if zooplankton doesn't graze 4/6/96
			// in  mg C m-3


			if (PhytoMass > PhytoLowerLimit)
			{
				// Adams & Steele, 1966; Parsons et al., 1967; Nassogne, 1970 - Eq. 101 - Parsons PTH}
				// ration in mg/mg/day}
				Ration = RmaxExp * (1-exp(KGraze *(PhytoLowerLimit - PhytoMass)));

				// Debugger(Ration);



				// ZooProduction in mg/m3/day  Fresh Weight ?

				ZooProd = ZooBiomass[i] * Ration; // /assimilation

				ZooGrossProduction[i] = ZooProd;
				// NG 4/6/96

				// phyto removal in mg C m-3  // Corrected 96.06.18 JGF

				PhytoMass = ZooProd
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

void TCarlZooplankton::Die()
{
	double NutrientMass;

	double MaxMortality  = 0.80,
			 KZoodeathLOGISTIC = 2,
			 rZoodeathLOGISTIC = 0.1;

	double BaseMortalityRate = 0; // if zero, does not die if not producing

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();


	for (int i = 0; i < NumberOfBoxes; i++)
	{
//		if (ZooProduction[i] != 0)  // Changed by JGF/NG 96.06.04 Die all the time
		{
			// Logistic Mortality - see ZOOMORT.XLS
			// N=K/(1+exp(a-rt))
			DeathLoss = MaxMortality                   // Death mg FW m-3 d-1    96.06.16 JGF
							/ (1 + exp (KZoodeathLOGISTIC
											- rZoodeathLOGISTIC
											* ZooBiomass[i]
											/ DryWeightToFreshWeight));

			if (ZooProduction[i] > 0) // Dies if producing
				DeathLoss = ZooProduction[i]
								* DeathLoss;
			else                      // always dies anyway if BMR > 0
				DeathLoss = BaseMortalityRate
								* ZooBiomass[i];

			ZooFlux[i] = ZooFlux[i]
							- DeathLoss;

			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = DeathLoss
									* CUBIC
									/ (DryWeightToFreshWeight
										* CarbonToDryWeight
										* 45 * 14/7);
				// return positive value because this is added
				MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass,
													 i,
													 "Ammonia",
													 ObjectCode);
			}
		}
	}
}


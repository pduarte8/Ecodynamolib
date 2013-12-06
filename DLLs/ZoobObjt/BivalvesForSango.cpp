// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 00.10.09
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
#include "zoobobjt.h"

// TBivalvesForSango Class
//
// Constructors invoked outside EcoDyn shell...
//

TBivalvesForSango::TBivalvesForSango(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TSuspensionFeeders(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    ATimeStep = MyPEcoDynClass->GetTimeStep();
}

TBivalvesForSango::TBivalvesForSango(char* className, float timeStep, char* morphologyFilename)
        :TSuspensionFeeders(className, timeStep, morphologyFilename)
{
    ATimeStep = MyPEcoDynClass->GetTimeStep();
}
// ...


TBivalvesForSango::TBivalvesForSango(TEcoDynClass* APEcoDynClass, char* className)
								 : TSuspensionFeeders(APEcoDynClass, className)
{
    ATimeStep = MyPEcoDynClass->GetTimeStep();
}

TBivalvesForSango::~TBivalvesForSango()
{
}

void TBivalvesForSango::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{

}


void TBivalvesForSango::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												double ExtraValue,
												int AnObjectCode)
{
	double MyHarvestableBiomass = 0.0;
	int MyBoxNumber = BoxNumber;

	for (int k = 0; k < NumberOfClasses; k++)
	{
		if (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] >=
				 ExtraValue)
			MyHarvestableBiomass = MyHarvestableBiomass +
				ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber];
	}
	Value = MyHarvestableBiomass;
	// values in TFW(live weight) na area em que existe a esp?cie
    LogMessage("Inquiry", srcName, "Harvestable biomass", Value, MyBoxNumber);
}

void TBivalvesForSango::Update(char* srcName, double ZooDensityInput,  // ind / m2 / day
                               int BoxNumber,
                               int ClassNumber,
                               int AnObjectCode)
{ //This works for recruitment or harvest of individuals to/from any class
    LogMessage("Update", srcName, "Zoo Class Density Flux", ZooDensityInput, BoxNumber);

	for (int k = 0; k < NumberOfClasses; k++)
		for (int i = 0; i < NumberOfBoxes; i++)
			if ((i == BoxNumber) && (k == ClassNumber))
			{
				if (ZooDensityInput > 0.0)
				{
					ZooClassDensityFlux[k * NumberOfBoxes + i]
						= ZooClassDensityFlux[k * NumberOfBoxes + i] + ZooDensityInput;
					ZooClassBiomassFlux[k * NumberOfBoxes + i]
						= ZooClassBiomassFlux[k * NumberOfBoxes + i] + ZooDensityInput *
                    (ZooIndividualWeight[k * NumberOfBoxes + i] + ZooShellMass[k * NumberOfBoxes + i]);
				}
			}
}


void TBivalvesForSango::Update(char* srcName, double Value,
                               double ExtraValue,
                               int BoxNumber,
                               int AnObjectCode)
{
	//This works for harvest of individuals above a certain size
   int k;
	double  mass_flux, PreyWeight, MyHarvestableBiomass = 0.0;
	int MyBoxNumber = BoxNumber, MyHarvestableClass = 0;
    LogMessage("Update", srcName, "Zoo Class Density Flux", Value, MyBoxNumber);

	//All values debited as gDW com concha
	mass_flux = Value;
	PreyWeight = ExtraValue;
	//Values recalculated to the current units - gDW sem concha
	for (k = 0; k < NumberOfClasses; k++)
	{
		if (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] >=
				 PreyWeight)
		{
			if (MyHarvestableClass == 0)
				MyHarvestableClass = k;
			MyHarvestableBiomass = MyHarvestableBiomass +
						ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber];
		}
	}

	for (k = MyHarvestableClass; k < NumberOfClasses; k++)
	{
		if ((mass_flux != 0) &&
			 (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] > 0)
			 )
		{
			ZooClassDensityFlux[k * NumberOfBoxes + MyBoxNumber]
							= ZooClassDensityFlux[k * NumberOfBoxes + MyBoxNumber] +
						  (mass_flux /
						  ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber]) *
						  (ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] /
						  MyHarvestableBiomass);

			ZooClassBiomassFlux[k * NumberOfBoxes + MyBoxNumber]
							= ZooClassBiomassFlux[k * NumberOfBoxes + MyBoxNumber] +
						  mass_flux *
						  (ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] /
						  MyHarvestableBiomass);
		}
	}
}




void TBivalvesForSango::Integrate()
{
   //if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() > 3.86))  Debugger(1);

//	Generic = SoftTissueEnergeticTotal; GenericFlux = ZooIndividualFlux;
   Integration(SoftTissueEnergeticTotal, ZooIndividualFlux);
//   SoftTissueEnergeticTotal = Generic; ZooIndividualFlux = GenericFlux;

   //if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() > 3.86))  Debugger(2);

//   Generic = ShellEnergeticTotal; GenericFlux = ZooIndividualFluxForTheShell;
   Integration(ShellEnergeticTotal, ZooIndividualFluxForTheShell);
//   ShellEnergeticTotal = Generic; ZooIndividualFluxForTheShell = GenericFlux;

   //if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() > 3.86))  Debugger(3);

//   Generic = ZooClassDensity; GenericFlux = ZooClassDensityFlux;
   Integration(ZooClassDensity, ZooClassDensityFlux);
//   ZooClassDensity = Generic; ZooClassDensityFlux = GenericFlux;

   //if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() > 3.86))  Debugger(4);

   for (int i = 0; i < NumberOfBoxes; i++)
	{

   	int AClass = -1;
      do
      {
      	AClass++;
      } while  (
        		   	(
           		   	(ZooClassBiomass[AClass * NumberOfBoxes + i] <= 0.0) ||
                    	(ZooClassDensity[AClass * NumberOfBoxes + i] <= 0.0) ||
                    	(ZooIndividualWeight[AClass * NumberOfBoxes + i] <= 0.0)
        		   	) && (AClass < NumberOfClasses - 1)
      			);
      /*int MyClass;
      MyClass = AClass;


      if (
				 ( ZooClassBiomass[MyClass * NumberOfBoxes + i] > 0.0) &&
				 ( ZooClassDensity[MyClass * NumberOfBoxes + i] > 0.0) &&
				 ( ZooIndividualWeight[MyClass * NumberOfBoxes + i] > 0.0)
				)
      {
      	IRPHY[i] = IRPHY[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	IRDET[i] = IRDET[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	IRPIM[i] = IRPIM[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	IRTPM[i]  = IRTPM[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	FRTPM[i] = FRTPM[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	FRPIM[i] = FRPIM[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	RRTPM[i] = RRTPM[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	RRPIM[i] = RRPIM[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	NEA[i] = NEA[i] / AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + i],AllometricCoefficientForFeeding);

      	NEB[i] = NEA[i] - MaintenanceRespiration * DAYSTOHOURS * TemperatureEffectsOnMaintenanceHeatLosses(i) -
                        ActivityRespirationCoefficient * NEA[i];
      }
      else
      {
      	IRPHY[i] = 0.0; IRDET[i] = 0.0;IRPIM[i] = 0.0;IRTPM[i] = 0.0;
         FRTPM[i] = 0.0; FRPIM[i] = 0.0;
         RRTPM[i] = 0.0; RRPIM[i] = 0.0;
         NEA[i] = 0.0; NEB[i] = 0.0;
      }    */


      for (int k = 0; k < NumberOfClasses; k++)
		{
			if (
				 /*( ZooClassBiomass[k * NumberOfBoxes + i] > 0.0) &&*/
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
				)
			{

         	ZooIndividualWeight[k * NumberOfBoxes + i] = SoftTissueEnergeticTotal[k * NumberOfBoxes + i] /
                                                         MeatEnergeticContents;

            ZooShellMass[k * NumberOfBoxes + i] = ShellEnergeticTotal[k * NumberOfBoxes + i] /
                                                  ShellEnergeticContents;

            ShellLength[k * NumberOfBoxes + i] = ShellLenghtCalculation(ZooShellMass[k * NumberOfBoxes + i]);

            ZooClassBiomass[k * NumberOfBoxes + i] = ZooClassDensity[k * NumberOfBoxes + i] *
                                                     (ZooIndividualWeight[k * NumberOfBoxes + i]+ ZooShellMass[k * NumberOfBoxes + i]) ;


         }

      }
   }
   //if ((MyPEcoDynClass->GetJulianDay() == 1) && (MyPEcoDynClass->GetCurrentTime() > 3.86))  Debugger(5);
	//DemographicUpwind();
}


void TBivalvesForSango::Feed(int ABox, int AClass)
{

}


void TBivalvesForSango::Respire(int ABox, int AClass)
{
   int MyBox, MyClass;

   MyBox = ABox;
   MyClass = AClass;

	TotalMaintenanceRespiration = MaintenanceRespiration / HOURSTOSECONDS/** DAYSTOHOURS*/ * TemperatureEffectsOnMaintenanceHeatLosses(MyBox) *
                            AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox], AllometricCoefficientForRespiration);
   TotalRespiration = ActivityRespirationCoefficient * NEA[MyBox] + TotalMaintenanceRespiration;
   //Debugger(TotalRespiration * DAYSTOSECONDS);
   /*if ((MyBox == 232) && (MyPEcoDynClass->GetCurrentTime() > 9.0))
   {
   	Debugger(10);
   	Debugger(TotalMaintenanceRespiration);
      Debugger(TotalRespiration);
      Debugger(AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox], AllometricCoefficientForRespiration));
   } */

}


void TBivalvesForSango::Excrete(int ABox, int AClass)
{
	double TotalAmmoniaExcretionPerClassPerBox, MyDepth;
	int MyBox, MyClass;
   MyBox = ABox; MyClass = AClass;
   TEcoDynClass * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
              * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   TotalExcretoryLosses = (((TotalRespiration/14.06)/16.0)/OxygenToNitrogenRatio(MyBox,MyClass))*NITROGENATOMICWEIGHT*CUBIC; //ug NH4 / d / individual
   TotalAmmoniaExcretionPerClassPerBox = TotalExcretoryLosses *
                                         ZooClassDensity[MyClass * NumberOfBoxes + MyBox]; //ug NH4 / d / m2
   //Debugger(TotalExcretoryLosses / 0.02428 * HOURSTOSECONDS);
   //Debugger(OxygenToNitrogenRatio(MyBox,MyClass));
   if (MyTransportPointer != NULL)
   	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
                                      MyBox,
                                      "Box depth",
                                      ObjectCode);
   else
   	MyDepth = MyPEcoDynClass->GetBoxDepth(MyBox);

   if (MyNutrientPointer != NULL)
      MyNutrientPointer->Update(GetEcoDynClassName(), TotalAmmoniaExcretionPerClassPerBox / MyDepth / NITROGENATOMICWEIGHT,   //umol NH4 / d / m3
											MyBox,
											"Ammonia",
											ObjectCode);
}


double TBivalvesForSango::OxygenToNitrogenRatio(int ABoxNumber, int AClass)
{
	int MyBoxNumber, MyClass;
   double WtStandNEA;
   MyBoxNumber = ABoxNumber;
   MyClass = AClass;
 	/*if ((10.0 + 0.038873 * NEA[MyBoxNumber]) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.038873 * NEA[MyBoxNumber]) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.038873 * NEA[MyBoxNumber]); */
   WtStandNEA = NEA[MyBoxNumber] * DAYSTOSECONDS /
   				 AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber],AllometricCoefficientForFeeding);

   if ((10.0 + 0.0363636 * WtStandNEA) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.0363636 * WtStandNEA) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.0363636 * WtStandNEA);
   //Pode variar entre 10 e 50, na propor??o de WtStandNEA }
}

void TBivalvesForSango::Pseudofaeces(int ABox, int AClass)
{
	int MyBox, MyClass;
   MyBox = ABox;
   MyClass = AClass;
   OrganicPseudoFaeces[MyBox] = OrganicPseudoFaeces[MyBox] + (RRTPM[MyBox] - RRPIM[MyBox]) *
                                ZooClassDensity[MyClass * NumberOfBoxes + MyBox];
   InorganicPseudofaeces[MyBox] = InorganicPseudofaeces[MyBox] + RRPIM[MyBox] *
   										 ZooClassDensity[MyClass * NumberOfBoxes + MyBox];
}

void TBivalvesForSango::Assimilate(int ABox, int AClass)
{
	int MyBox/*, MyClass*/;
   MyBox = ABox;
   //MyClass = AClass;
    /*if (MyBox == 267)
    {
        Debugger(NEA[MyBox] * DAYSTOSECONDS);
        Debugger(TotalRespiration * DAYSTOSECONDS);
        Debugger(TotalExcretoryLosses * DAYSTOSECONDS);
        Debugger(AmmoniaToJoules);
    } */
	NEB[MyBox] = NEA[MyBox] - TotalRespiration - TotalExcretoryLosses * AmmoniaToJoules;

   /*if ((MyBox == 232) && (MyPEcoDynClass->GetCurrentTime() > 9.0))
   {
   	Debugger(100);
   	Debugger(NEA[MyBox]);
      Debugger(TotalRespiration);
      Debugger(TotalExcretoryLosses);
   } */
}


void TBivalvesForSango::Reproduce(int ABox, int AClass)
{

}


void TBivalvesForSango::ShellDeposition(int ABox, int AClass)
{
	int MyBox, MyClass;
   MyBox = ABox;
   MyClass = AClass;
	if (
   		(NEB[MyBox] > 0.0) &&
         (
         	SoftTissueEnergeticTotal[MyClass * NumberOfBoxes + MyBox] /
         	(
            	SoftTissueEnergeticTotal[MyClass * NumberOfBoxes + MyBox] +
             	ShellEnergeticTotal[MyClass * NumberOfBoxes + MyBox]
            ) >= SoftTissueEnergyToTotalEnergy
         )
      )
   	ZooIndividualFluxForTheShell[MyClass * NumberOfBoxes + MyBox] = ZooIndividualFluxForTheShell[MyClass * NumberOfBoxes + MyBox] +
      																					 ProportionOfEnergyAllocatedToShell *
                                                                      NEB[MyBox];
      else
      ZooIndividualFluxForTheShell[MyClass * NumberOfBoxes + MyBox] = 0.0;
}



void TBivalvesForSango::SoftTissueGrowth(int ABox, int AClass)
{
	int MyBox, MyClass;

   MyBox = ABox;
   MyClass = AClass;

   if (
   		(NEB[MyBox] > 0.0) &&
    		(
         	SoftTissueEnergeticTotal[MyClass * NumberOfBoxes + MyBox] /
         	(
            	SoftTissueEnergeticTotal[MyClass * NumberOfBoxes + MyBox] +
             	ShellEnergeticTotal[MyClass * NumberOfBoxes + MyBox]
            ) >= SoftTissueEnergyToTotalEnergy
         )
   	)
      ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] = ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] +
         	                                                  ProportionOfEnergyAllocatedToSoftTissue *
            	                                               NEB[MyBox];
   else
   	ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] = ZooIndividualFlux[MyClass * NumberOfBoxes + MyBox] +
                                                           NEB[MyBox];
}

void TBivalvesForSango::Die(int ABox, int AClass)
{
	int MyBox, MyClass;

   MyBox = ABox;
   MyClass = AClass;

   //if ((MyPEcoDynClass->GetJulianDay() == 13) && (MyPEcoDynClass->GetCurrentTime() > 10.228) && (MyBox == 195)) Debugger(112);

   Mortality[MyClass * NumberOfBoxes + MyBox] = ZooClassDensity[MyClass * NumberOfBoxes + MyBox] -
                                               ZooClassDensity[MyClass * NumberOfBoxes + MyBox] *
                                               exp(-IntrinsicRateOfMortality / DAYSTOHOURS / HOURSTOSECONDS * 1.0);
   if (Mortality[MyClass * NumberOfBoxes + MyBox] < 0.0)
            	Mortality[MyClass * NumberOfBoxes + MyBox] = 0.0;
   ZooClassDensityFlux[MyClass * NumberOfBoxes + MyBox]
					= ZooClassDensityFlux[MyClass * NumberOfBoxes + MyBox] - Mortality[MyClass * NumberOfBoxes + MyBox];

   //if ((MyPEcoDynClass->GetJulianDay() == 13) && (MyPEcoDynClass->GetCurrentTime() > 10.228) && (MyBox == 195)) Debugger(113);            
}


double TBivalvesForSango::TemperatureEffectsOnFeedingRate(int ABoxNumber)
{
    return 1.0;
}




double TBivalvesForSango::TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber)
{
    return 1.0;
}


double TBivalvesForSango::AllometricCorrection (double AWeight, double AnAllometricCoefficient)
{
	double MyWeight, MyAllometricCoefficient;
   MyWeight = AWeight;
   MyAllometricCoefficient = AnAllometricCoefficient;
	return pow(MyWeight/DryWeightOfAStandardAnimal, MyAllometricCoefficient);
}

double TBivalvesForSango::ShellLenghtCalculation(double AShellDryWeight)
{
    return 1.0;
}

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

// TVerticalModelZooplankton Class
//
// Constructors invoked outside EcoDyn shell...
//

TVerticalModelZooplankton::TVerticalModelZooplankton(char* className, float timeStep,
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
    BuildVerticalModelZoo();
}

TVerticalModelZooplankton::TVerticalModelZooplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TZooplanktonBasic(className, timeStep, morphologyFilename, variablesFilename,
                parametersFilename)
{
    BuildVerticalModelZoo();
}
// ...

TVerticalModelZooplankton::TVerticalModelZooplankton(TEcoDynClass* APEcoDynClass,
        char* className) : TZooplanktonBasic(APEcoDynClass, className)
{
    BuildVerticalModelZoo();
}

void TVerticalModelZooplankton::BuildVerticalModelZoo()
{
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		AverageGrazedNCRatio = new double[NumberOfBoxes];
    }
    for (int i = 0; i < NumberOfBoxes; i++)
   	    AverageGrazedNCRatio[i] = 0;
}


TVerticalModelZooplankton::~TVerticalModelZooplankton()
{
    freeMemory();
}

void TVerticalModelZooplankton::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
            delete [] AverageGrazedNCRatio;
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TVerticalModelZooplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TVerticalModelZooplankton::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Zooplankton biomass") == 0)
		Value = ZooBiomass[MyBoxNumber]/ DryWeightToFreshWeight / CarbonToDryWeight;
	else if (strcmp(MyParameter, "Zooplankton production") == 0)
		Value = ZooProduction[MyBoxNumber]/ DryWeightToFreshWeight / CarbonToDryWeight;
	else if (strcmp(MyParameter, "Zooplankton net production") == 0)
		Value = ZooNetProduction[MyBoxNumber]/ DryWeightToFreshWeight / CarbonToDryWeight;
	else if (strcmp(MyParameter, "Zooplankton gross production") == 0)
		Value = ZooGrossProduction[MyBoxNumber]/ DryWeightToFreshWeight / CarbonToDryWeight;
	else if (strcmp(MyParameter, "Zooplankton net/gross ratio") == 0)
	{
	    if ( ZooGrossProduction[MyBoxNumber] != 0 )
		    Value = (ZooNetProduction[MyBoxNumber]
					/ ZooGrossProduction[MyBoxNumber])*100;
		else
            Value = INFINITY;
	}
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 2",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TVerticalModelZooplankton::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
				}
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TVerticalModelZooplankton::SetVariableValue(char* srcName, double Value,
									int BoxNumber,
									char* VariableName)
{
  bool rc = true;

   // Zooplankton expects to receive in mgFW m-3 // JGF 97.08.11
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Zooplankton biomass") == 0)
        ZooBiomass[BoxNumber] = Value * DryWeightToFreshWeight * CarbonToDryWeight;
	else if (strcmp(VariableName, "Zooplankton production") == 0)
		ZooProduction[BoxNumber] = Value * DryWeightToFreshWeight * CarbonToDryWeight;
	else if (strcmp(VariableName, "Zooplankton net production") == 0)
		ZooNetProduction[BoxNumber] = Value * DryWeightToFreshWeight * CarbonToDryWeight;
	else if (strcmp(VariableName, "Zooplankton gross production") == 0)
        ZooGrossProduction[BoxNumber] = Value * DryWeightToFreshWeight * CarbonToDryWeight;
    else
        rc = false;

    return rc;
}


void TVerticalModelZooplankton::Go ()
{

	for (int i = 0; i < NumberOfBoxes; i++)
   	ZooProduction[i] = 0;

	Graze();
   Drool();
   Faeces();
	Respire();
	//Excrete();
	Die();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooFlux[i] = ZooFlux[i] + ZooProduction[i];
      //Debugger(i);
      //Debugger(ZooProduction[i]);
      //Debugger(ZooFlux[i]);
      //Debugger(ZooBiomass[i]);
		ZooNetProduction[i] = ZooProduction[i];
	}
}



void TVerticalModelZooplankton::Integrate()  //Zooplankton assumed to move in the vertical, independently of
                                             //vertical eddy diffusion
{
//	Generic = ZooBiomass; GenericFlux = ZooFlux;
	Integration(ZooBiomass, ZooFlux);
//	ZooBiomass = Generic; ZooFlux = GenericFlux;
}


void TVerticalModelZooplankton::Graze()      //Pedro Duarte 6/4/98 following Fasham et al. (1990) -
                                             //J. of Marine Research 48, 591-639
{
	double PhytoMass = 0, DetritalMass = 0, DetritalCarbonMass = 0,
          DetritalNitrogenMass = 0, BacterialMass = 0,
          PhytoPlanktonRation = 0, ChlorophyllCarbonRatio,
          DetritalRation = 0, BacterialRation = 0,
          Drooled = 0,
          //NitrogenDrooled = 0,
          //CarbonDrooled = 0,
          PhytoCMass = 0,PhytoNMass = 0,
          SPMCMass = 0, SPMNMass = 0,
          BacterialCMass = 0, BacterialNMass = 0,
          //AverageNCRatio,
          PhytoNCRatio = 0, DetritusNCRatio = 0, BacteriaNCRatio = 0;

   //TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
   //TEcoDynClass* MyBacteriaPointer = MyPEcoDynClass->GetBacteriaPointer();
   TEcoDynClass* MyManPointer = MyPEcoDynClass->GetManPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if ((MyPhytoPointer != NULL)||(MySPMPointer != NULL) || (MyManPointer != NULL))/* || (MyBacteriaPointer != NULL)*/
		{
      	Ration = 0;
      	if (MyPhytoPointer != NULL)
				MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoMass,
													 i,
													 "Phytoplankton C biomass",
													 ObjectCode);
         // Phytoplankton returns its mass as  mg C m-3


         if (MySPMPointer != NULL)
         {
         	MySPMPointer->Inquiry(GetEcoDynClassName(), DetritalMass,
            	                       i,
               	                    "Particulate organic carbon",
                  	                 ObjectCode);
         	// SPM returns its mass as mg C l-1
				// convert to mg C m-3
            DetritalMass = DetritalMass * CUBIC;
         }

         //if (MyBacteriaPointer != NULL)
         if (MyManPointer != NULL)
         	/*MyBacteriaPointer->Inquiry(GetEcoDynClassName(), BacterialMass,
													 i,
													 "Bacterial biomass",
													 ObjectCode);  */

            //Bacteria returns its mass as  mg C m-3
            MyManPointer->Inquiry(GetEcoDynClassName(), BacterialMass,
													 i,
													 "Bacterial biomass",
													 ObjectCode);

         if (MyPhytoPointer != NULL)
         	PhytoPlanktonRation = Rmax * PhytoCoefficient * PhytoMass * PhytoMass /
                                    (KGraze *
               	                   	(PhytoCoefficient * PhytoMass +
                  	                   DetritalCoefficient * DetritalMass +
                     	                BacterialCoefficient * BacterialMass
                        	             ) +
                           	          PhytoCoefficient * PhytoMass * PhytoMass +
                              	       DetritalCoefficient * DetritalMass * DetritalMass +
                                 	    BacterialCoefficient * BacterialMass * BacterialMass
                  	   	          );

         if (MySPMPointer != NULL)
            DetritalRation = Rmax * DetritalCoefficient * DetritalMass * DetritalMass /
            	                      (KGraze *
               	                   	(PhytoCoefficient * PhytoMass +
                  	                   DetritalCoefficient * DetritalMass +
                     	                BacterialCoefficient * BacterialMass
                        	             ) +
                           	          PhytoCoefficient * PhytoMass * PhytoMass +
                              	       DetritalCoefficient * DetritalMass * DetritalMass +
                                 	    BacterialCoefficient * BacterialMass * BacterialMass
                               	    );

         //if (MyBacteriaPointer != NULL)
         if (MyManPointer != NULL)
         	/*MyBacteriaPointer->Inquiry(GetEcoDynClassName(), BacterialMass,
													 i,
													 "Bacterial biomass",
													 ObjectCode);  */

            //Bacteria returns its mass as  mg C m-3

         	BacterialRation = Rmax * BacterialCoefficient * BacterialMass * BacterialMass /
            	                      (KGraze *
               	                   	(PhytoCoefficient * PhytoMass +
                  	                   DetritalCoefficient * DetritalMass +
                     	                BacterialCoefficient * BacterialMass
                        	             ) +
                           	          PhytoCoefficient * PhytoMass * PhytoMass +
                              	       DetritalCoefficient * DetritalMass * DetritalMass +
                                 	    BacterialCoefficient * BacterialMass * BacterialMass
                                	  );


			ZooGrossProduction[i] = 0;

         Ration = PhytoPlanktonRation + DetritalRation + BacterialRation;

         ZooProd = ZooBiomass[i] * Ration;

         ZooGrossProduction[i] = ZooProd;
         //Debugger(ZooGrossProduction[i]);

         if (ZooProd > 0)   //Removal of phytoplankton, particulate carbon and nitrogen (detritus) and bacteria
         {
          	// phyto removal in mg C m-3
          	PhytoMass = ZooBiomass[i] * PhytoPlanktonRation
            	         /DryWeightToFreshWeight/CarbonToDryWeight;

         	// detritus removal in mg C l-1
         	DetritalMass = ZooBiomass[i] * DetritalRation
            	            /DryWeightToFreshWeight/CarbonToDryWeight
               	         /CUBIC;

            // bacteria removal in mg C m-3
         	BacterialMass = ZooBiomass[i] * BacterialRation
            	             / DryWeightToFreshWeight/CarbonToDryWeight;

            // return negative value because this is subtracted

            if (MyPhytoPointer != NULL)
            	MyPhytoPointer->Update(GetEcoDynClassName(), -PhytoMass,
                                        i,
                                        "Phytoplankton biomass",
                                        ObjectCode);

            if (MySPMPointer != NULL)    //Remove particulate carbon and particulate nitrogen
            {
              	MySPMPointer->Update(GetEcoDynClassName(), -DetritalMass,
			 										i,
													"Particulate organic carbon",
		  											ObjectCode);

               MySPMPointer->Inquiry(GetEcoDynClassName(), DetritalCarbonMass,
            	                       i,
               	                    "Particulate organic carbon",
                  	                 ObjectCode);
               MySPMPointer->Inquiry(GetEcoDynClassName(), DetritalNitrogenMass,
            	                       i,
               	                    "Particulate organic nitrogen",
                  	                 ObjectCode);
               if (DetritalCarbonMass > 0)    //Remove particulate nitrogen in the proportion of removed particulate carbon
              		MySPMPointer->Update(GetEcoDynClassName(), -DetritalMass * DetritalNitrogenMass / DetritalCarbonMass,
													i,
	  												"Particulate organic nitrogen",
	 												ObjectCode);
            }
            //if (MyBacteriaPointer != NULL)
            if (MyManPointer != NULL)
            	/*MyBacteriaPointer->Update(GetEcoDynClassName(), -BacterialMass,
														  i,
														  "Bacterial biomass",
                        	                 ObjectCode); */
               MyManPointer->Update(GetEcoDynClassName(), -BacterialMass,
														  i,
														  "Bacterial biomass",
                        	                 ObjectCode);

            ZooProduction[i] = ZooProd;



            if ((MyPhytoPointer != NULL) || (MySPMPointer != NULL) || (MyManPointer != NULL))/*(MyBacteriaPointer != NULL)*/
            {
            	if (MyPhytoPointer != NULL)
               {
               	MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoCMass,
													 i,
													 "Phytoplankton C biomass",
													 ObjectCode);
                  MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoNMass,
													 i,
													 "Phytoplankton N biomass",
													 ObjectCode);
                  if (PhytoCMass > 0)
                     	PhytoNCRatio = PhytoNMass / PhytoCMass;
               }
               if (MySPMPointer != NULL)
               {
               	MySPMPointer->Inquiry(GetEcoDynClassName(), SPMCMass,
													 i,
													 "Particulate organic carbon",
													 ObjectCode);
                  MySPMPointer->Inquiry(GetEcoDynClassName(), SPMNMass,
													 i,
													 "Particulate organic nitrogen",
													 ObjectCode);
                  if (SPMCMass > 0)
                     	DetritusNCRatio = SPMNMass / SPMCMass;
               }
               //if (MyBacteriaPointer != NULL)
               if (MyManPointer != NULL)
               {
               	/*MyBacteriaPointer->Inquiry(GetEcoDynClassName(), BacterialCMass,
                                        i,
													 "Bacterial Cbiomass",
													 ObjectCode);
                  MyBacteriaPointer->Inquiry(GetEcoDynClassName(), BacterialNMass,
													 i,
													 "Bacterial N biomass",
													 ObjectCode);*/

                  MyManPointer->Inquiry(GetEcoDynClassName(), BacterialCMass,
                                        i,
													 "Bacterial carbon biomass",
													 ObjectCode);
                  MyManPointer->Inquiry(GetEcoDynClassName(), BacterialNMass,
													 i,
													 "Bacterial nitrogen biomass",
													 ObjectCode);

                  if (BacterialCMass > 0)
                     	BacteriaNCRatio = BacterialNMass / BacterialCMass;
               }

               AverageGrazedNCRatio[i] = (PhytoNCRatio  * PhytoPlanktonRation +
                  		DetritusNCRatio * DetritalRation * CUBIC /*+
                  		BacteriaNCRatio * BacterialRation */) /
                  		(PhytoPlanktonRation + DetritalRation * CUBIC /*+ BacterialRation*/);

            }
         }
		}
	}
}

void TVerticalModelZooplankton::Drool()
{
	double Drooled, CarbonDrooled, NitrogenDrooled;

   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
   	Drooled = ZooProduction[i]
								 * DOCLoss;
      ZooProduction[i] = ZooProduction[i]
									 - Drooled;
      //Debugger(ZooProduction[i]); Debugger(DOCLoss);

      if ((Drooled > 0) && (MyNutrientPointer != NULL))
      {
      	CarbonDrooled = Drooled /DryWeightToFreshWeight/CarbonToDryWeight
									* CUBIC
                           / 12 /*Carbon Atomic Weight*/;
         NitrogenDrooled = Drooled / DryWeightToFreshWeight / CarbonToDryWeight
               	            * CUBIC  *  AverageGrazedNCRatio[i]
									   /NITROGENATOMICWEIGHT;

         //Debugger(CarbonDrooled); Debugger(NitrogenDrooled); Debugger(AverageGrazedNCRatio[i]);
         
         MyNutrientPointer->Update(GetEcoDynClassName(), CarbonDrooled,
                                        	i,
                                        	"DOC",
                                        	ObjectCode);
         MyNutrientPointer->Update(GetEcoDynClassName(), NitrogenDrooled,
                                        	i,
                                        	"DON",
                                        	ObjectCode);
      }
   }
}



void TVerticalModelZooplankton::Faeces()
{
	double FaecesProduction;
	TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (ZooProduction[i] > 0)
		{
         FaecesProduction = ZooProduction[i] * (1.0 - AssimilationEfficiency);
         ZooProduction[i] = ZooProduction[i] - FaecesProduction;

      	if (MySPMPointer != NULL)
         {
         	MySPMPointer->Update(GetEcoDynClassName(), FaecesProduction / DryWeightToFreshWeight / CUBIC,
                                  i,
                                  "Suspended matter",
                                  ObjectCode);
         	MySPMPointer->Update(GetEcoDynClassName(), FaecesProduction / DryWeightToFreshWeight / CarbonToDryWeight
                                  / CUBIC,
                                  i,
                                  "Particulate organic carbon",
                                  ObjectCode);                      //return as mg C / l
            MySPMPointer->Update(GetEcoDynClassName(), FaecesProduction / DryWeightToFreshWeight/ CarbonToDryWeight
                                  / CUBIC *
                                  AverageGrazedNCRatio[i],
                                  i,
                                  "Particulate organic nitrogen",
                                  ObjectCode);                    //return as mg N / l

         }
      }
	}
}



void TVerticalModelZooplankton::Respire()
{
	double NutrientMass,
			 BiomassMetabolised, CarbonMetabolized;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (ZooProduction[i] != 0)
		{

         BiomassMetabolised = ZooProduction[i]
								      * Metabolism;
         Excrete(i);

			ZooProduction[i] = ZooProduction[i]
									 - BiomassMetabolised;

		}
	}
}

void TVerticalModelZooplankton::Excrete(int i)
{
	double NutrientMass,
			 Excretion;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

   Excretion = ZooProduction[i]
								  * ExcretionLoss;

//Aten??o!!! Neste objecto assume-se que as perdas por excre??o resultam da respira??o (metabolismo)
//pelo que aqui n?o se reduz a ZooProduction, dado que j? foi reduzida devido ? respira??o no m?todo
//"respire". Este m?todo "excrete" calcula assim o azoto excretado devido ao metabolismo, com base
//na raz?o de Redfield. Verificar sempre se a % de biomassa perdida pela respira??o ("Metabolism") ?
//igual ? % de azoto excretado ("ExcretionLoss").

			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = Excretion
									* CUBIC
									/ (DryWeightToFreshWeight *
                              CarbonToDryWeight
										* (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor));
				// return positive value because this is added
            MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass * AmmoniaFractionOfExcretion,
													i,
													"Ammonia",
													ObjectCode);
				MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass * (1 - AmmoniaFractionOfExcretion),
													i,
													"DON",
													ObjectCode);
			}

}


void TVerticalModelZooplankton::Die()
{
	double ZooplanktonDead;

	TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
   	ZooplanktonDead = ZooBiomass[i]
						 * DeathLoss;
	  	ZooFlux[i] = ZooFlux[i]
	 					- ZooplanktonDead;

      //Aten??o !!! Neste objecto assume-se que a remineraliza??o do zoopl?ncton passa primeiro
      //pela sua transforma??o em detritos, excepto a parte da mortalidade que ? de imediato convertida em am?nia.

      if (MySPMPointer != NULL)
      {
         	/*MySPMPointer->Update(GetEcoDynClassName(), ZooplanktonDead * DryWeightToFreshWeight / CUBIC,
                                  i,
                                  "Suspended matter",
                                  ObjectCode);  */
      	 MySPMPointer->Update(GetEcoDynClassName(), ZooplanktonDead /DryWeightToFreshWeight/CarbonToDryWeight / CUBIC *
          							  DetritalFractionOfMortality,
                                i,
                                "Particulate organic carbon",
                                ObjectCode);                      //return as mg C / l
          MySPMPointer->Update(GetEcoDynClassName(), ZooplanktonDead /DryWeightToFreshWeight/CarbonToDryWeight / CUBIC *
                                RedfieldNFactor / RedfieldCFactor *
                                DetritalFractionOfMortality,
                                i,
                                "Particulate organic nitrogen",
                                ObjectCode);                    //return as mg N / l
      }
      if (MyNutrientPointer != NULL)
      {
				// All values returned in umol m-3 N
				// return positive value because this is added
            MyNutrientPointer->Update(GetEcoDynClassName(), ZooplanktonDead * (1 - DetritalFractionOfMortality)
									            * CUBIC
									            / (DryWeightToFreshWeight *
                                       CarbonToDryWeight *
										         (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor)),
													i,
													"Ammonia",
													ObjectCode);
      }
	}
}

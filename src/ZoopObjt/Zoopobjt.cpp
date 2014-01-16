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

// TZooplankton Class
//
// Constructors invoked outside EcoDyn shell...
//

TZooplankton::TZooplankton(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildZooplankton(className);
}

TZooplankton::TZooplankton(char* className, float timeStep,
        char* morphologyFilename) :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology()) {
        BuildZooplankton(className);
    }
}
// ...


TZooplankton::TZooplankton(TEcoDynClass* APEcoDynClass, char* className)
									: TEcoDynClass()

{
	// Receive pointer to integrate
	MyPEcoDynClass = APEcoDynClass;
    BuildZooplankton(className);
}

void TZooplankton::BuildZooplankton(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    BoxArray = MyPEcoDynClass->BoxArray;
    TimeStep = MyPEcoDynClass->GetTimeStep();

	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		ZooBiomass         = new double[NumberOfBoxes];
		ZooLoad            = new double[NumberOfBoxes];
		ZooProduction      = new double[NumberOfBoxes];
		ZooFlux            = new double[NumberOfBoxes];
		ZooGrossProduction = new double[NumberOfBoxes];
		ZooNetProduction   = new double[NumberOfBoxes];
		ZooplanktonInterfaceExchange
			= new AdvectionDiffusionRecord[NumberOfBoxes*NumberOfBoxes];

		// Create generics

/*		Generic     = new double[NumberOfBoxes];
		GenericLoad = new double[NumberOfBoxes];
		GenericFlux = new double[NumberOfBoxes];*/
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Zooplankton object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TZooplankton::BuildZooplankton - Zooplankton object array not dimensioned" << endl;
#endif  // __BORLANDC__
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    ZooBiomass[i] = 0;
        ZooLoad[i] = 0;
        ZooFlux[i] = 0;
        ZooProduction[i] = 0;
        ZooNetProduction[i] = 0;
        ZooGrossProduction[i] = 0;
    }

	// rates and conversions

	DryWeightToFreshWeight = 5;           // Empirical 20%
	ChlorophyllToCarbon = 35;             // Ems-Dollard p.83
	CarbonToDryWeight = 3.333333;         // Empirical carbon is 1/3 DW
	ExcretionLoss = 0.10;                 // 3-10% of photosynthetic carbon fixed
	DOCLoss = 0.18;                       // Mean from Valiela p. 278 - data
													  // of Copping & Lorenzen 1980: 17-19% using labelling
	DeathLoss = 0.90;                     //  condition - 90% of zooplankton dies

	Rmax = 5;                             // 5mg FW/ mg FW/ day (500% body weight/day)
	KGraze = 0.01;                        // 0.0005 - see ZOORMAX.XLS
	Metabolism = 0.75;                    // 75% average from Parsons, Takahashi & Hargrave p. 138
	PhytoLowerLimit = 40;                 // 40-130 ug C l-1 for zoo: PTH p.133

	KZooStarvedByPhyto = 0;
	MaximumDeathLoss = 0;
	Q10Coefficient = 0;
	Q10WaterTempReference = 0;
	Q10KPhytoMass = 0;
	Q10PhytoLowerLimit = 0;
	Q10Rmax = 0;

	ObjectCode = ZOOPOBJECTCODE;
}

TZooplankton::~TZooplankton()
{
    freeMemory();
}

void TZooplankton::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] ZooBiomass;
            delete [] ZooLoad;
            delete [] ZooProduction;
            delete [] ZooFlux;
            delete [] ZooGrossProduction;
            delete [] ZooNetProduction;
            delete [] ZooplanktonInterfaceExchange;

    /*		delete [] Generic;
            delete [] GenericLoad;
            delete [] GenericFlux;*/
        }
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
				cerr << "TZooplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TZooplankton::Go ()
{
	// Active methods
 //	Graze();

	// NG 7/6/96  : before when the zoo doen't graze,
	// Zooproduction stayed at it last value, decreasing slowly
	// because of drool, excrete....
	for (int j = 0; j < NumberOfBoxes; j++)
	ZooProduction[j] = 0;


//	GrazeWithTemperature();   //NG 6/6/96
	Graze();
	Drool();
   Faeces();
	Respire();
	Excrete();
	Die();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooFlux[i] = ZooFlux[i] + ZooProduction[i];
 //		ZooNetProduction[i] = ZooProduction[i];        //NG 5/6/96
 // Supprime et mis ds Die : voir comment ameliorer ?a.

 // NG 16/6/96 : NP Zoopk remise ici et ne tient pas compte de la mortalit?,
 // comme il a ?t? fait pour la phytopk.

		ZooNetProduction[i] = ZooProduction[i];

	}
}

void TZooplankton::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Zooplankton biomass") == 0)
		Value = ZooBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Zooplankton production") == 0)
		Value = ZooProduction[MyBoxNumber];
	else if (strcmp(MyParameter, "Zooplankton net production") == 0)
		Value = ZooNetProduction[MyBoxNumber];
	else if (strcmp(MyParameter, "Zooplankton gross production") == 0)
        Value = ZooGrossProduction[MyBoxNumber];
	else if (strcmp(MyParameter, "Zooplankton net/gross ratio") == 0)
	{
	    if ( ZooGrossProduction[MyBoxNumber] != 0 )
			Value = (ZooNetProduction[MyBoxNumber]              //NG 5/6/96
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
                      "EcoDynamo Alert - Inquiry 3",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TZooplankton::Inquiry 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TZooplankton::Inquiry(char* srcName, double &Value,
										 int BoxX,
										 int BoxY,
										 char* ParameterName,
										 int AnObjectCode)
{
	// All values in mg FW s-1 - Convert to kg C timestep-1
	int MyBoxNumber = BoxX * NumberOfBoxes + BoxY;
	char MyParameter[65]; // 64 characters (max) for parameter name
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Zoop. advective flux matrix") == 0) // kg C for the integration interval - checked 96.06.24 JGF
	{
		Value = ZooplanktonInterfaceExchange[MyBoxNumber].AdvectiveFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
									  / DryWeightToFreshWeight
									  / CarbonToDryWeight
									  / (CUBIC * CUBIC);
    }
    else if (strcmp(MyParameter, "Zoop. dispersive flux matrix") == 0) // kg C for the integration interval - checked 96.06.24 JGF
    {
		Value = ZooplanktonInterfaceExchange[MyBoxNumber].DispersiveFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
                             / DryWeightToFreshWeight
									  / CarbonToDryWeight
									  / (CUBIC * CUBIC);
    }
    else if (strcmp(MyParameter, "Zoop. net flux matrix") == 0) // kg C for the integration interval - checked 96.06.24 JGF
    {
		Value = ZooplanktonInterfaceExchange[MyBoxNumber].InFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
                             / DryWeightToFreshWeight
									  / CarbonToDryWeight
									  / (CUBIC * CUBIC);
    }
    else
    {
#ifdef __BORLANDC__
    	char Caption[129];
	    strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Inquiry 4",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TZooplankton::Inquiry 4 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TZooplankton::Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode)
{
   // Zooplankton expects to receive in mgFW m-3 // JGF 97.08.11
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

	if (strcmp(MyParameter, "Zooplankton biomass") == 0)
		ZooFlux[MyBoxNumber] = ZooFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
	    char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TZooplankton::Update - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}

bool TZooplankton::SetVariableValue(char* srcName, double Value,
									int BoxNumber,
									char* VariableName)
{
  bool rc = true;

   // Zooplankton expects to receive in mgFW m-3 // JGF 97.08.11
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Zooplankton biomass") == 0)
		ZooBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zooplankton production") == 0)
		ZooProduction[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zooplankton net production") == 0)
		ZooNetProduction[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zooplankton gross production") == 0)
        ZooGrossProduction[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TZooplankton::Integrate()
{
	// Integrate zooplankton biomass within the box
  //	Debugger(ZooBiomass[0]);
  //	Debugger(ZooFlux[0]);
//	Generic = ZooBiomass; GenericFlux = ZooFlux;
	Integration(ZooBiomass, ZooFlux);
//	ZooBiomass = Generic; ZooFlux = GenericFlux;
  //	Debugger(ZooBiomass[0]);
	// Transport the zooplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
		// New calls for boundary exchange
		MyTransportPointer->Go(ZooBiomass, ZooLoad, RiverZoo, OceanZoo);

		//GenericInterfaceExchange = MyTransportPointer->Exchange;

		// Important note - The pointer variables must be passed like this (below) to a
		// pointer which does not point to the Exchange (in transport (or GIE - here) address
		// otherwise, the pointer starting address is retained, and as exchange changes, so
		// do the results in ZooplanktonInterface exchange - therefore, the real values must be
		// specifically converted and assigned one by one - wow 96.06.24 JGF

		/*for (int i = 0; i < NumberOfBoxes; i++)
			for (int j = 0; j < NumberOfBoxes; j++)
			{
				double Value1 = GenericInterfaceExchange[i * NumberOfBoxes + j].AdvectiveFlow;
				ZooplanktonInterfaceExchange[i * NumberOfBoxes + j].AdvectiveFlow = Value1;
				double Value2 = GenericInterfaceExchange[i * NumberOfBoxes + j].DispersiveFlow;
				ZooplanktonInterfaceExchange[i * NumberOfBoxes + j].DispersiveFlow = Value2;
				ZooplanktonInterfaceExchange[i * NumberOfBoxes + j].InFlow = Value1 + Value2; // net
			}   */
	}
}


void TZooplankton::Graze()      // Function no more used for the moment
										  // NG 6/6/96
{
	double PhytoMass, ChlorophyllCarbonRatio;
	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (MyPhytoPointer != NULL)
		{
			MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoMass,
												 i,
												 "Phytoplankton biomass",
												 ObjectCode);
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllCarbonRatio,
												 i,
												 "Chlorophyll : Carbon",
												 ObjectCode);
		//	 Debugger(PhytoMass);

			// Phytoplankton returns its mass as ug chl a l-1
			// convert to mg C m-3
         ChlorophyllToCarbon = 1 /  ChlorophyllCarbonRatio;

			PhytoMass = PhytoMass * ChlorophyllToCarbon;
			// only graze if there is more than the lower limit of C


			ZooGrossProduction[i] = 0;
			//NG : GrossProduction stays at 0 if zooplankton doesn't graze 4/6/96
			// in  mg C m-3


			if (PhytoMass > PhytoLowerLimit)
			{
				// Adams & Steele, 1966; Parsons et al., 1967; Nassogne, 1970 - Eq. 101 - Parsons PTH}
				// ration in mg/mg/day}
				Ration = Rmax * (1-exp(KGraze *(PhytoLowerLimit - PhytoMass)));

		//		Debugger(Ration);



				// ZooProduction in mg/m3/day  Fresh Weight ?

				ZooProd = ZooBiomass[i] * Ration; // /assimilation

				ZooGrossProduction[i] = ZooProd;
				// NG 4/6/96

				// phyto removal in ug C m-3

			 /*	PhytoMass = ZooProd
								* CUBIC
								/ (DryWeightToFreshWeight
									* CarbonToDryWeight);
			  */

			  // phyto removal in mg C m-3

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

void TZooplankton::Drool()
{
	double NutrientMass,
			 CarbonDrooled;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (ZooProduction[i] != 0)
		{
			CarbonDrooled = ZooProduction[i]
								 * DOCLoss;
			ZooProduction[i] = ZooProduction[i]
									 - CarbonDrooled;
			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonDrooled
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

// Sort out excrete and respire, because:
// zooplankton excretes due to metabolic activity, i.e. respiration, and
// therefore it seems one method is redundant

void TZooplankton::Excrete()
{
	double NutrientMass,
			 CarbonExcreted;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (ZooProduction[i] != 0)
		{
			CarbonExcreted = ZooProduction[i]
								  * ExcretionLoss;
			ZooProduction[i] = ZooProduction[i]
									 - CarbonExcreted;
			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonExcreted
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

void TZooplankton::Respire()
{
	double NutrientMass,
			 CarbonMetabolised;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (ZooProduction[i] != 0)
		{
			CarbonMetabolised = ZooProduction[i]
									  * Metabolism;
			ZooProduction[i] = ZooProduction[i]
									 - CarbonMetabolised;
			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonMetabolised
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

void TZooplankton::Faeces()
{
}

void TZooplankton::Die()
{
	double NutrientMass,
			 CarbonDead;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	double PhytoMass;

	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
//		if (ZooProduction[i] != 0)  // Changed by JGF/NG 96.06.04 Die all the time
		{
			if (MyPhytoPointer != NULL)
			{
				MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoMass,
													 i,
													 "Phytoplankton biomass",
													 ObjectCode);

				// Modified mortality function uses a first order decay rate
				// dMort/dPhyto = -K * Mort
				// Death loss is day-1
				// Maximum death loss is day-1
				// PhytoMass is in ug l-1 chla
				// KZooStarvedByPhyto is in 1 / (ug l-1) chla
				// NG/JGF 96.06.04
				double ZooBaseDeath = 0.05;

				DeathLoss = ZooBaseDeath
								+ MaximumDeathLoss
								* exp(-KZooStarvedByPhyto
										* PhytoMass);


			}
			else
				DeathLoss = 0.05; // Day-1

			/*	CarbonDead = ZooProduction[i]
								 * DeathLoss;
				ZooProduction[i] = ZooProduction[i]
									 - CarbonDead;
			*/
  //		Debugger(DeathLoss);                 //Changed by NG, June 96

			CarbonDead = ZooBiomass[i]
							 * DeathLoss;
			ZooFlux[i] = ZooFlux[i]
							- CarbonDead;


		//	ZooNetProduction[i] = ZooProduction[i]-CarbonDead;
		// NG 16/6/96 : supprim? pour ne pas tenir compte de Mortality et
		// remis ds Go().




  //		Debugger(CarbonDead);

			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonDead
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



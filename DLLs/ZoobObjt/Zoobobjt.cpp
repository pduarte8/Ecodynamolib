// Zoobenthos object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.06.13
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

// TZoobenthos Class
//
// Constructors invoked outside EcoDyn shell...
//

TZoobenthos::TZoobenthos(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildZoobenthos(className);
}

TZoobenthos::TZoobenthos(char* className, float timeStep, char* morphologyFilename)
        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology()) {
        BuildZoobenthos(className);
    }
}
// ...

TZoobenthos::TZoobenthos(TEcoDynClass* APEcoDynClass, char* className)
									: TEcoDynClass()
{
	// Receive pointer to integrate
	MyPEcoDynClass = APEcoDynClass;
    BuildZoobenthos(className);
}

void TZoobenthos::BuildZoobenthos(char* className)
{
    strcpy(EcoDynClassName, className);
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();
    bool BenthicRecord;
    BenthicRecord = MyPEcoDynClass->HasABenthicSpeciesRecord();
    if (!BenthicRecord)
    {
        if (MyPEcoDynClass->OpenBenthicSpecies(MyPEcoDynClass->GetBenthicSpeciesFile()))
            BenthicRecord = true;
    }
    if (BenthicRecord)
    {
        MyBenthicSpeciesRecord = MyPEcoDynClass->GetABenthicSpeciesRecord();
        NumberOfBoxesWithBenthicSpecies = MyPEcoDynClass->GetNumberOfCellsWithBenthicSpecies();
    }
    else
#ifdef __BORLANDC__
        MessageBox(0,"Benthic species record not initialized",
              "EcoDynamo alert",
              MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TZoobenthos::BuildZoobenthos - Benthic species record not initialized" << endl;
#endif  // __BORLANDC__
	// Box areas
	BoxArray = MyPEcoDynClass->BoxArray;
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		ZooBiomass         = new double[NumberOfBoxes];
		ZooDensity         = new double[NumberOfBoxes];
		ZooLoad            = new double[NumberOfBoxes];
		ZooProduction      = new double[NumberOfBoxes];
		ZooBiomassFlux     = new double[NumberOfBoxes];
		ZooDensityFlux     = new double[NumberOfBoxes];
		ZooGrossProduction = new double[NumberOfBoxes];
		ZooNetProduction   = new double[NumberOfBoxes];
		HabitatArea        = new double[NumberOfBoxes];
		POMa               = new double[NumberOfBoxes];
		ZooDINRelease      = new double[NumberOfBoxes];
		ZooPhytoUptake     = new double[NumberOfBoxes];
		ZooPOCUptake       = new double[NumberOfBoxes];

		// Zero all arrays
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			ZooBiomass[i] = 0;
			ZooDensity[i] = 0;
			ZooLoad[i] = 0;
			ZooBiomassFlux[i] = 0;
			ZooDensityFlux[i] = 0;
			ZooProduction[i] = 0;
			ZooNetProduction[i] = 0;
			ZooGrossProduction[i] = 0;
			HabitatArea[i] = 0;
			POMa[i] = 0;
			ZooDINRelease[i] = 0;
            ZooPhytoUptake[i] = 0;    // AP, 2006.07.20
            ZooPOCUptake[i] = 0;
		}
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Zoobenthos object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TZoobenthos::BuildZoobenthos - Zoobenthos object array not dimensioned" << endl;
#endif  // __BORLANDC__

	TimeStep = MyPEcoDynClass->GetTimeStep();
	SigmaNumber = 2.5;
	ObjectCode = ZOOBOBJECTCODE;
}

TZoobenthos::~TZoobenthos()
{
    freeMemory();
}

void TZoobenthos::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] ZooBiomass;
            delete [] ZooDensity;
            delete [] ZooLoad;
            delete [] ZooProduction;
            delete [] ZooBiomassFlux;
            delete [] ZooDensityFlux;
            delete [] ZooGrossProduction;
            delete [] ZooNetProduction;
            delete [] HabitatArea;
            delete [] POMa;
            delete [] ZooDINRelease;
            delete [] ZooPhytoUptake;
            delete [] ZooPOCUptake;
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
				cerr << "TZoobenthos::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TZoobenthos::Go ()
{
	// Active methods
   for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooBiomassFlux[i] = ZooBiomassFlux[i] + ZooProduction[i];
		ZooDINRelease[i] = 0;
		ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
	}
	Feed();
	Ingestion();
	Assimilate();
	Excrete();
	Respire();
	Reproduce();
	Die();
}

void TZoobenthos::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Zoobenthos biomass") == 0)
				Value = ZooBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Zoobenthos production") == 0)
				Value = ZooProduction[MyBoxNumber];
	else if (strcmp(MyParameter, "Zoobenthos net production") == 0)
				Value = ZooNetProduction[MyBoxNumber];
	else if (strcmp(MyParameter, "Zoobenthos gross production") == 0)
				Value = ZooGrossProduction[MyBoxNumber];
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
                      "EcoDynamo Alert - Inquiry 8",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TZoobenthos::Inquiry 8 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.05.29
bool TZoobenthos::SetVariableValue(char* srcName, double Value,
                                             int BoxNumber,
                                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Zoobenthos biomass") == 0)
        ZooBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zoobenthos production") == 0)
        ZooProduction[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zoobenthos net production") == 0)
        ZooNetProduction[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zoobenthos gross production") == 0)
        ZooGrossProduction[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

void TZoobenthos::Integrate()
{
	// Integrate zoobenthos biomass within the box
//	Generic = ZooBiomass; GenericFlux = ZooBiomassFlux;
	Integration(ZooBiomass, ZooBiomassFlux);
//	ZooBiomass = Generic; ZooBiomassFlux = GenericFlux;

	// Why does the zoobenthos get transported? Is it not benthos?

	// Transport the zoobenthos
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
		MyTransportPointer->Go(ZooBiomass, ZooLoad, RiverZoo, OceanZoo);

}

double TZoobenthos::InverseNormalSt(double ADensity)
{
	if ( ADensity < 0.398942 )
		return sqrt( -2 * log ( ADensity * sqrt( 2 * M_PI ) ) );
	else
		return 0;
}

void TZoobenthos::ClassTransitionNormal()
{
	int i,j,k, kk;
	double critical_size, critical_limit, distribution_limit,growth,
	integral0,integral1,integral2,MeanDensity,Mean1,Mean2,u,
	soma1, soma2, stdev;

//C?lculo das transfer?ncias entre classes para aferi??o dos fluxos de biomassa}
	for (i = 0; i < NumberOfBoxes; i++)
	{
		for (j = 0; j < NumberOfClasses * NumberOfClasses; j++)
		{
			transition_number[j]  = 0;
			transition_biomass[j] = 0;
			transition_weight[j]  = 0;
		}
		for (k = 0; k < NumberOfClasses; k++)
		{
			if ((ZooClassBiomass[k * NumberOfBoxes + i] > 0) && (ZooInitialWeight[k * NumberOfBoxes + i] > 0))
			{
				kk = k;
				integral0 = 0;
				growth = ZooIndividualWeight[k * NumberOfBoxes + i]-ZooInitialWeight[k * NumberOfBoxes + i];
//				Debugger(k);
//				Debugger(i);
//				Debugger(ZooIndividualWeight[k * NumberOfBoxes + i]);
//				Debugger(ZooInitialWeight[k * NumberOfBoxes + i]);
				if (growth > 0)
				{
					kk = k + 1;
					distribution_limit = SigmaNumber;
					critical_limit = ClassMaxLimits[k];
				}
				else
				if (growth < 0)
				{
					kk--;
					distribution_limit = -SigmaNumber;
					critical_limit = ClassMinLimits[k];
				}
				if ((kk != -1) && (kk < NumberOfClasses) && (growth != 0))
				{
					stdev = class_st_dev(ClassMinLimits[k],ClassMaxLimits[k],ZooInitialWeight[k * NumberOfBoxes + i]);
//					Debugger(k);
//					Debugger(i);
//					Debugger(ClassMinLimits[k]);
//					Debugger(ClassMaxLimits[k]);
//					Debugger(ZooInitialWeight[k * NumberOfBoxes + i]);
					if (stdev <= 0)
//					if (stdev == 0)
					stdev = 0.0000000001;
					critical_size = (critical_limit-growth-ZooInitialWeight[k * NumberOfBoxes + i])
										 /stdev;
//					Debugger(critical_limit);
//					Debugger(growth);
//					Debugger(ZooInitialWeight[k * NumberOfBoxes + i]);
//					Debugger(stdev);
//					Debugger(critical_size);
					if (critical_size > SigmaNumber)
						critical_size = SigmaNumber;
					if (critical_size < -SigmaNumber)
						critical_size = -SigmaNumber;

					/* Integra??o para o caso em que o critical_size e o distribution_limit se
					encontram em lados de sinal oposto da normal padr?o */
//			  		Debugger(critical_size);
					if (
						(critical_size < 0) &&
						(growth > 0) ||
						( critical_size > 0 ) &&
						( growth < 0 )
						)

					{
						integral1 = integral_normal(0, distribution_limit);
						Mean1 = integral1/(fabs(distribution_limit-0));
						integral2 = integral_normal(0, fabs(critical_size));
						Mean2 = integral2/(fabs(critical_size-0));
						integral0 = integral1 + integral2;
						MeanDensity = (Mean1*fabs(distribution_limit-0)+Mean2*fabs(critical_size-0))/
												(fabs(distribution_limit-0)+fabs(critical_size-0));
					}
					/* Integra??o para o caso em que o critical_size e o distribution_limit se
					encontram em lados do mesmo sinal da normal padr?o */
					if (
						 (critical_size < 0) &&
						(growth < 0) ||
						(critical_size > 0)
						&& (growth > 0)
						)
					{
						integral0 = integral_normal(critical_size, distribution_limit);
						if (integral0 == 0)
							MeanDensity = 0;
						else
							MeanDensity = integral0/(fabs(distribution_limit-critical_size));
					}
					ZooClassDensity[k * NumberOfBoxes + i]
							= ZooClassBiomass[k * NumberOfBoxes + i]
							/ ZooIndividualWeight[k * NumberOfBoxes + i];


//             	CONVENTION  k * NumberOfClasses + kk
					transition_number[k * NumberOfClasses + kk]
							= integral0
							*(ZooClassDensity[k * NumberOfBoxes + i]);

					if (transition_number[k * NumberOfClasses + kk] > ZooClassDensity[k * NumberOfBoxes + i])
						transition_number[k * NumberOfClasses + kk] = ZooClassDensity[k * NumberOfBoxes + i];


					if (transition_number[k * NumberOfClasses + kk] > 0)
					{
						//Algas que mudam de classe}
						u = InverseNormalSt(MeanDensity);
						if (growth < 0)
							u = -1.0 * u;
						transition_weight[k * NumberOfClasses + kk]
								= u
								* class_st_dev(ClassMinLimits[k],ClassMaxLimits[k],
												  ZooInitialWeight[k * NumberOfBoxes + i])
							  + ZooInitialWeight[k * NumberOfBoxes + i]
							  + growth;
						transition_biomass[k * NumberOfClasses + kk]
								= transition_weight[k * NumberOfClasses + kk]
								* transition_number[k * NumberOfClasses + kk];

						if (transition_biomass[k * NumberOfClasses + kk]
								 > ZooClassBiomass[k * NumberOfBoxes + i])
								transition_biomass[k * NumberOfClasses + kk]
								= ZooClassBiomass[k * NumberOfBoxes + i];
					}
				}
			}
		}
		for (k = 0; k < NumberOfClasses; k++)
		{
			soma1 = 0;
			soma2 = 0;
			for (kk = 0; kk < NumberOfClasses; kk++)
			{
				soma1 = soma1
						  + transition_biomass[kk * NumberOfClasses + k];
				soma2 = soma2
						  + transition_number[kk * NumberOfClasses + k];

				soma1 = soma1
						  - transition_biomass[k * NumberOfClasses + kk];
				soma2 = soma2
						  - transition_number[k * NumberOfClasses + kk];

			}
			ZooClassBiomass[k * NumberOfBoxes + i]
				= ZooClassBiomass[k * NumberOfBoxes + i]
				  + soma1;

			ZooClassDensity[k * NumberOfBoxes + i]
				= ZooClassDensity[k * NumberOfBoxes + i]
				  + soma2;
			if (
				 (ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 (ZooClassBiomass[k * NumberOfBoxes + i]!=0)
				)
				ZooIndividualWeight[k * NumberOfBoxes + i]
					= ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooClassDensity[k * NumberOfBoxes + i];

		}
	}
}

void TZoobenthos::DemographicModel()
{
	int i,k,FirstNonZeroClass;

	for (i = 0; i < NumberOfBoxes; i++)
	{
		FirstNonZeroClass = -1;
		for (k = 0; k < NumberOfClasses; k++)
		{
			//Debugger(GrowthRate[k * NumberOfBoxes + i]);
			if (ZooClassDensity[k * NumberOfBoxes + i] > 0.0)
			{
				if (FirstNonZeroClass == -1)
					FirstNonZeroClass = k;
			}
		}
		if (FirstNonZeroClass >= 0)
		{
			for (k = FirstNonZeroClass; k < NumberOfClasses; k++)
			{

				if (k == FirstNonZeroClass)
				{

					r[0] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												  GrowthRate[(k + 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k + 1) * NumberOfBoxes + i] /
												  (4.0 * ClassIncrement)
												 ) *
												 TimeStep;

					b[0] = (1.0 + TimeStep / 2.0
						 * Mortality[k * NumberOfBoxes + i]);

					c[0] = TimeStep /
						 (4.0 * ClassIncrement) *
						  GrowthRate[(k + 1) * NumberOfBoxes + i];
				}
				else
				if ((k > FirstNonZeroClass) && (k < NumberOfClasses - 1))
				{
					r[k - FirstNonZeroClass] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												 (GrowthRate[(k + 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k + 1) * NumberOfBoxes + i]-
												  GrowthRate[(k - 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k - 1) * NumberOfBoxes + i]) /
												  (4.0 * ClassIncrement)
												 ) *
												 TimeStep;

					a[k - FirstNonZeroClass] = - TimeStep /
						(4.0 *ClassIncrement ) *
						 GrowthRate[(k - 1) * NumberOfBoxes + i];

					b[k - FirstNonZeroClass] = (1.0 + TimeStep / 2.0
						 * Mortality[k * NumberOfBoxes + i]);

					c[k - FirstNonZeroClass] = TimeStep /
						 (4.0 *ClassIncrement) *
						  GrowthRate[(k + 1) * NumberOfBoxes + i];
				}
				else
				if (k == NumberOfClasses - 1)
				{
					r[k - FirstNonZeroClass] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												 (GrowthRate[(k - 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k - 1) * NumberOfBoxes + i]) /
												  (4.0 * ClassIncrement)
												 ) *
												 TimeStep;

					a[k - FirstNonZeroClass] = - TimeStep /
						(4.0 *ClassIncrement) *
						 GrowthRate[(k - 1) * NumberOfBoxes + i];

					b[k - FirstNonZeroClass] = (1.0 + TimeStep / 2.0
						 * Mortality[k * NumberOfBoxes + i]);

				}
			}
			IntegrationTridag(/*a, b, c, r,*/ NumberOfClasses - FirstNonZeroClass);
			//MessageBox(0,"Tridag okay","",MB_OK);
			for (k = FirstNonZeroClass; k < NumberOfClasses; k++)
			{
				//Debugger(k - FirstNonZeroClass);
				//Debugger(u[k - FirstNonZeroClass]);
				//Debugger(k * NumberOfBoxes + i);
				//Debugger(ZooClassDensity[k * NumberOfBoxes + i]);*/
				ZooClassDensity[k * NumberOfBoxes + i] = u[k - FirstNonZeroClass];
			}
		}
	}
}

void TZoobenthos::DemographicModelCentralDifferences()
{
//	Generic = ZooClassDensity;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		for (int k = 0; k < NumberOfClasses; k++)
		{
			if (k == 0)
			{
				if (ZooClassDensity[k * NumberOfBoxes + i] <= 0.0)
					ZooClassDensityFlux[k * NumberOfBoxes + i] = 0.0;
				else
				{
					ZooClassDensityFlux[k * NumberOfBoxes + i] =
						-(GrowthRate[k * NumberOfBoxes + i] *
						ZooClassDensity[k * NumberOfBoxes + i] +
						GrowthRate[(k + 1) * NumberOfBoxes + i] *
						ZooClassDensity[(k + 1) * NumberOfBoxes + i]) /
						(2 * ClassIncrement) -
						Mortality[k * NumberOfBoxes + i] *
						ZooClassDensity[k * NumberOfBoxes + i];
					if (fabs(ZooClassDensityFlux[k * NumberOfBoxes + i]) * TimeStep >
						 ZooClassDensity[k * NumberOfBoxes + i])
						 ZooClassDensityFlux[k * NumberOfBoxes + i] =
								- ZooClassDensity[k * NumberOfBoxes + i] /
								TimeStep;
				}
			}
			if (

					(k > 0) &&
					(k < NumberOfClasses - 1)
				)
			{
				if (ZooClassDensity[k * NumberOfBoxes + i] == 0.0)
				ZooClassDensityFlux[k * NumberOfBoxes + i] = (GrowthRate[(k - 1) * NumberOfBoxes + i] *
					ZooClassDensity[(k - 1) * NumberOfBoxes + i])/
					(2 * ClassIncrement) -
					Mortality[k * NumberOfBoxes + i] *
					ZooClassDensity[k * NumberOfBoxes + i];
				else
				ZooClassDensityFlux[k * NumberOfBoxes + i] = (GrowthRate[(k - 1) * NumberOfBoxes + i] *
					ZooClassDensity[(k - 1) * NumberOfBoxes + i]
					- GrowthRate[(k + 1) * NumberOfBoxes + i] *
					ZooClassDensity[(k + 1) * NumberOfBoxes + i]) /
					(2 * ClassIncrement) -
					Mortality[k * NumberOfBoxes + i] *
					ZooClassDensity[k * NumberOfBoxes + i];
				if (
					 (ZooClassDensityFlux[k * NumberOfBoxes + i] < 0.0) &&
					 (fabs(ZooClassDensityFlux[k * NumberOfBoxes + i]) * TimeStep >
						  ZooClassDensity[k * NumberOfBoxes + i]
					 )
					)
						ZooClassDensityFlux[k * NumberOfBoxes + i] =
							- ZooClassDensityFlux[(k - 1) * NumberOfBoxes + i] -
							Mortality[k * NumberOfBoxes + i] *
							ZooClassDensity[k * NumberOfBoxes + i] -
							ZooClassDensity[k * NumberOfBoxes + i] /
							TimeStep;
			}
			if (k == NumberOfClasses - 1)
			{
				if (ZooClassDensity[(k - 1) * NumberOfBoxes + i] <= 0.0)
					ZooClassDensityFlux[k * NumberOfBoxes + i] = 0.0;
				else
				{
					ZooClassDensityFlux[k * NumberOfBoxes + i] =
						(GrowthRate[k * NumberOfBoxes + i] *
						ZooClassDensity[k * NumberOfBoxes + i] +
						GrowthRate[(k - 1) * NumberOfBoxes + i] *
						ZooClassDensity[(k - 1) * NumberOfBoxes + i]) /
						(2 * ClassIncrement) -
						Mortality[k * NumberOfBoxes + i] *
						ZooClassDensity[k * NumberOfBoxes + i];
					 if (fabs(ZooClassDensityFlux[k * NumberOfBoxes + i]) * TimeStep >
						  ZooClassDensity[(k - 1)* NumberOfBoxes + i])
						  ZooClassDensityFlux[k * NumberOfBoxes + i] =
								ZooClassDensity[(k - 1) * NumberOfBoxes + i] /
								TimeStep -
								Mortality[(k - 1) * NumberOfBoxes + i] *
								ZooClassDensity[k * NumberOfBoxes + i];
				}
			}
		}

	}
	Integration(NumberOfClasses, ZooClassDensity, ZooClassDensityFlux);
//	ZooClassDensity = Generic; ZooClassDensityFlux = GenericFlux;
}


void TZoobenthos::DemographicUpwind()
//Para que este algoritmo funcione importa que num passo de c?lculo
//os individuos de uma classe possam transitar somente para uma das classes
//adjacentes.
{
	int i,k;
	for (i = 0; i < NumberOfBoxes; i++)
	{
		//Inicializa??o das transfer?ncias entre classes
		for (k = 0; k < NumberOfClasses * NumberOfClasses; k++)
			transition_number[k] = 0;

		for (k = 0; k < NumberOfClasses; k++)
		{
			if (
				 (ZooIndividualFlux[k * NumberOfBoxes + i] > 0.0) &&
				 (k < NumberOfClasses - 1)
				)
			{ //Situa??o "normal", em que a taxa de crescimento ? positiva
			  //A ?ltima classe n?o deve exibir crescimento positivo.
				transition_number[k * NumberOfClasses + (k + 1)] =
					ZooIndividualFlux[k * NumberOfBoxes + i] *
					ZooClassDensity[k * NumberOfBoxes + i] /
					ClassIncrement;//Ind?viduos perdidos pela classe k
										//para a classe k+1


			}
			if(
				(ZooIndividualFlux[k * NumberOfBoxes + i] < 0.0) &&
				(k > 0)
			  )
			{ //Situa??o "anormal", em que a taxa de crescimento ? negativa.
			  //A primeira classe n?o deve exibir crescimento positivo.


				transition_number[k * NumberOfClasses + (k - 1)] =
						-ZooIndividualFlux[k * NumberOfBoxes + i] *          // - because negative growth rate
						ZooClassDensity[k * NumberOfBoxes + i] /
						ClassIncrement;//Ind?viduos perdidos pela classe k
											//para a classe k-1
			}
		}
		for (k = 0; k < NumberOfClasses; k++)
		{
			for (int kk = 0; kk < NumberOfClasses; kk++)
			{

				ZooClassDensityFlux[k * NumberOfBoxes + i] =
					ZooClassDensityFlux[k * NumberOfBoxes + i] -
					transition_number[k * NumberOfClasses + kk] +
					transition_number[kk * NumberOfClasses + k];

				/*
				if (
					  (transition_number[k * NumberOfClasses + kk] != 0) &&
					  (transition_number[kk * NumberOfClasses + k] != 0)
					)
				{
					Debugger(transition_number[k * NumberOfClasses + kk]);
					Debugger(transition_number[kk * NumberOfClasses + k]);
				}
            */

				/*
				if (
					(ZooClassDensityFlux[k * NumberOfBoxes + i] < 0.0)&&
					(fabs(ZooClassDensityFlux[k * NumberOfBoxes + i]*TimeStep) >
					 ZooClassDensity[k * NumberOfBoxes + i])
					)
				{
					Debugger(k); Debugger(kk); Debugger(i);
					Debugger(ZooClassDensity[k * NumberOfBoxes + i]);
					Debugger(ZooClassDensityFlux[k * NumberOfBoxes + i]);
					Debugger(transition_number[k * NumberOfClasses + kk]);
					Debugger(transition_number[kk * NumberOfClasses + k]);
				}
				*/
			}
		}

		/*
		//Reinicializa??o das transfer?ncias entre classes
		for (k = 0; k < NumberOfClasses * NumberOfClasses; k++)
		{
				transition_number[k] = 0;
		}
		*/

	} //End of boxes

	//Integra??o dos fluxos de individuos
//	Generic = ZooClassDensity;
//	GenericFlux = ZooClassDensityFlux;
	Integration(NumberOfClasses, ZooClassDensity, ZooClassDensityFlux);
//	ZooClassDensity = Generic; ZooClassDensityFlux = GenericFlux;
	for (i = 0; i < NumberOfBoxes; i++)
	{
		for (int k = 0; k < NumberOfClasses; k++)
		{
			ZooClassBiomass[k * NumberOfBoxes + i] =
				ZooIndividualWeight[k * NumberOfBoxes + i] *
				ZooClassDensity[k * NumberOfBoxes + i];
			if (ZooClassBiomass[k * NumberOfBoxes + i] < 0.0)
				 ZooClassBiomass[k * NumberOfBoxes + i] = 0.0;
			if (ZooClassDensity[k * NumberOfBoxes + i] < 0.0)
				 ZooClassDensity[k * NumberOfBoxes + i] = 0.0;
		}
	}
}

void TZoobenthos::DemographicUpwindSemiImplicit()
{
	int i,k,FirstNonZeroClass;

	for (i = 0; i < NumberOfBoxes; i++)
	{
		FirstNonZeroClass = -1;
		for (k = 0; k < NumberOfClasses; k++)
		{
			if (ZooClassDensity[k * NumberOfBoxes + i] > 0.0)
			{
				if (FirstNonZeroClass == -1)
					FirstNonZeroClass = k;
			}
		}
		if (FirstNonZeroClass >= 0)
		{
			for (k = FirstNonZeroClass; k < NumberOfClasses; k++)
			{

				if (k == FirstNonZeroClass)
				{
					if (GrowthRate[k * NumberOfBoxes + i] >= 0.0)
					//Situa??o "normal" em que a taxa de crescimento ? >= 0
					{
						r[0] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												  - GrowthRate[k  * NumberOfBoxes + i] *
												  ZooClassDensity[k * NumberOfBoxes + i] /
												  (2.0 * ClassIncrement)
												 ) *
												 TimeStep;

						b[0] = (1.0 + TimeStep / 2.0
							* Mortality[k * NumberOfBoxes + i]);

						c[0] = TimeStep /
							 (2.0 * ClassIncrement) *
							GrowthRate[k * NumberOfBoxes + i];
					}
					if (GrowthRate[k * NumberOfBoxes + i] < 0.0)
					//Situa??o "anormal" em que a taxa de crescimento ? < 0
					{
						r[0] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												  (GrowthRate[(k + 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k + 1) * NumberOfBoxes + i]-
												  GrowthRate[k  * NumberOfBoxes + i] *
												  ZooClassDensity[k * NumberOfBoxes + i]) /
												  (2.0 * ClassIncrement)
												 ) *
												 TimeStep;
						a[0] = - TimeStep /
							(2.0 *ClassIncrement ) *
							GrowthRate[(k + 1) * NumberOfBoxes + i];

						b[0] = (1.0 + TimeStep / 2.0
							* Mortality[k * NumberOfBoxes + i]);
					}
				}
				else
				if ((k > FirstNonZeroClass) && (k < NumberOfClasses - 1))
				{
					if (GrowthRate[k * NumberOfBoxes + i] >= 0.0)
					{
						r[k - FirstNonZeroClass] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												 (GrowthRate[(k - 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k - 1) * NumberOfBoxes + i]-
												  GrowthRate[k * NumberOfBoxes + i] *
												  ZooClassDensity[k * NumberOfBoxes + i]) /
												  (2.0 * ClassIncrement)
												 ) *
												 TimeStep;

						a[k - FirstNonZeroClass] = - TimeStep /
							(2.0 *ClassIncrement ) *
							GrowthRate[(k - 1) * NumberOfBoxes + i];

						b[k - FirstNonZeroClass] = (1.0 + TimeStep / 2.0
							 * Mortality[k * NumberOfBoxes + i]);

						c[k - FirstNonZeroClass] = TimeStep /
							 (2.0 * ClassIncrement) *
							 GrowthRate[k * NumberOfBoxes + i];
					}
					if (GrowthRate[k * NumberOfBoxes + i] < 0.0)
					{
						r[k - FirstNonZeroClass] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												 (GrowthRate[(k + 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k + 1) * NumberOfBoxes + i]-
												  GrowthRate[k * NumberOfBoxes + i] *
												  ZooClassDensity[k * NumberOfBoxes + i]) /
												  (2.0 * ClassIncrement)
												 ) *
												 TimeStep;

						a[k - FirstNonZeroClass] = - TimeStep /
							(2.0 *ClassIncrement ) *
							GrowthRate[(k + 1) * NumberOfBoxes + i];

						b[k - FirstNonZeroClass] = (1.0 + TimeStep / 2.0
							 * Mortality[k * NumberOfBoxes + i]);

						c[k - FirstNonZeroClass] = TimeStep /
							 (2.0 * ClassIncrement) *
							 GrowthRate[k * NumberOfBoxes + i];

					}

				}
				else
				if (k == NumberOfClasses - 1)
				{
					if (GrowthRate[k * NumberOfBoxes + i] >= 0.0)
					{
						r[k - FirstNonZeroClass] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												 (GrowthRate[(k - 1) * NumberOfBoxes + i] *
												  ZooClassDensity[(k - 1) * NumberOfBoxes + i]-
												  GrowthRate[k * NumberOfBoxes + i] *
												  ZooClassDensity[k * NumberOfBoxes + i]) /
												  (2.0 * ClassIncrement)
												 ) *
												 TimeStep;

						a[k - FirstNonZeroClass] = - TimeStep /
							(2.0 *ClassIncrement) *
							GrowthRate[(k - 1) * NumberOfBoxes + i];

						b[k - FirstNonZeroClass] = (1.0 + TimeStep / 2.0
							* Mortality[k * NumberOfBoxes + i]);
					}
					if (GrowthRate[k * NumberOfBoxes + i] < 0.0)
					{
						r[k - FirstNonZeroClass] = ZooClassDensity[k * NumberOfBoxes + i] *
												(1.0 - (TimeStep / 2.0) * Mortality[k * NumberOfBoxes + i]) -
												(
												  - GrowthRate[k  * NumberOfBoxes + i] *
												  ZooClassDensity[k * NumberOfBoxes + i] /
												  (2.0 * ClassIncrement)
												 ) *
												 TimeStep;

						b[k - FirstNonZeroClass] = (1.0 + TimeStep / 2.0
							* Mortality[k * NumberOfBoxes + i]);

						c[k - FirstNonZeroClass] = TimeStep /
							 (2.0 * ClassIncrement) *
							GrowthRate[k * NumberOfBoxes + i];
					}
				}
			}
			IntegrationTridag(NumberOfClasses - FirstNonZeroClass);
			for (k = FirstNonZeroClass; k < NumberOfClasses; k++)
			{
				//Debugger(k - FirstNonZeroClass);
				//Debugger(u[k - FirstNonZeroClass]);
				//Debugger(k * NumberOfBoxes + i);
				//Debugger(ZooClassDensity[k * NumberOfBoxes + i]);*/
				ZooClassDensity[k * NumberOfBoxes + i] = u[k - FirstNonZeroClass];
				ZooClassBiomass[k * NumberOfBoxes + i] =
					ZooClassDensity[k * NumberOfBoxes + i] *
					ZooIndividualWeight[k * NumberOfBoxes + i];
			}
		}
	}

}

double TZoobenthos::integral_normal(double z1,
												double z2)
/*

Esta fun??o calcula a propor??o da distribui??o normal padr?o(m?dia=0, desvio padr?o=1
entre quaisquer dois valores z1 e z2. Estes valores devem ter o mesmo sinal. O c?lculo
efectua-se segundo a f?rmula de Hastings (Best approximation)(Abramowitz & Stegun, 1964).
Nos casos em que se pretenda utilizar a f?rmula para calcular o integral entre dois pontos
de sinais diferentes, tem que se calcular o integral ? esquerda e som?-lo ao integral ? direita

*/

{
	double
		p = 0.2316419,
		b1 = 0.319381530,
		b2 = -0.356563782,
		b3 = 1.781477937,
		b4 = -1.821255978,
		b5 = 1.330274429,
		t,integral1,integral2/*,
		tolerance = 0.00000000000000000001*/;


	if (z2 < 0)
		z2 = fabs(z2);
	t = 1/(1 + p * z2);
	integral1 = 1-(1/sqrt(2*M_PI))*
						exp(-z2*z2/2)*
						(b1*t+b2*t*t+b3*t*t*t+b4*t*t*t*t+b5*t*t*t*t*t);
	if (z1 < 0)
		z1 = fabs(z1);
	t = 1/(1 + p * z1);
	integral2 = 1-(1/sqrt(2*M_PI))*
						exp(-z1*z1/2)*
						(b1*t+b2*t*t+b3*t*t*t+b4*t*t*t*t+b5*t*t*t*t*t);


	return (integral1-integral2);
//	Debugger(integral1-integral2);
//	Debugger(fabs(integral1-integral2));
/*
	if (fabs(integral1-integral2) * -1.0 > tolerance)
		return (integral1-integral2);
	else
		return 0.0;
*/
}



double TZoobenthos::class_st_dev(double down_limit,
											double upper_limit,
											double average)
{
double minimum_sigma, sigma;

	sigma = (average - down_limit)/SigmaNumber;
	minimum_sigma = sigma;
	sigma = (upper_limit - average)/SigmaNumber;
	if (sigma < minimum_sigma)
		return sigma;
	else
		return minimum_sigma;
}



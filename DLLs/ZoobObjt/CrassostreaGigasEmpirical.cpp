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
#include "iodll.h"

// TZoobenthos Class


TCrassostreaGigasEmpirical::TCrassostreaGigasEmpirical(TEcoDynClass* APEcoDynClass,
        char* className) : TCrassostreaGigas(APEcoDynClass, className)
{
	POCtoPOM = 2.5;   //CH2O  mg/mgC
	SelectionEfficiency = new double[NumberOfBoxes * NumberOfClasses];
}

TCrassostreaGigasEmpirical::~TCrassostreaGigasEmpirical()
{
    freeMemory();
}

void TCrassostreaGigasEmpirical::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
            delete [] SelectionEfficiency;
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
				cerr << "TCrassostreaGigasEmpirical::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
		}

void TCrassostreaGigasEmpirical:: Go()
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooDINRelease[i] = 0;
      ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
	}
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if ((ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				 (ZooClassDensity[k * NumberOfBoxes + i] >0))
			{
				if (ZooIndividualWeight[k * NumberOfBoxes + i] == 0)
				ZooIndividualWeight[k * NumberOfBoxes + i]
					= ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooClassDensity[k * NumberOfBoxes + i];
				ZooInitialWeight[k * NumberOfBoxes + i]
					= ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooClassDensity[k * NumberOfBoxes + i];
				if (ZooInitialWeight[k * NumberOfBoxes + i] > ClassMaxLimits[k])
					ZooInitialWeight[k * NumberOfBoxes + i] = ClassMaxLimits[k];

				/*Os m?todos feed, pseudofaeces, ingestion, assimilation e respiration s?o utilizados
				ao n?vel do individuo, recorrendo a rela??es alom?tricas. O c?lculo do crescimento
				ponderal das ostras baseia-se no balan?o energ?tico apresentado em Raillard(1991)
				Tese de Doutoramento pp. 53-70}*/
				//C?lculo do fluxo l?quido ao n?vel individual e da popula??o, com base nos processos metab?licos}
				Feed();
				Pseudofaeces();
				Ingestion();
				//Ingest?o de seston pela popula??o da caixa}
				// seston_ingestion := seston_ingestion * (ZooClassBiomass^[i,k]/ZooIndividualWeight^[i,k])*HoursInDay;
				Assimilate();
				Respire();
				/*
				O m?todo deposition ? utilizado ao n?vel da popula??o. Calcula
				assim a deposi??o da popula??o de cada caixa. ? invocado a partir do m?todo
				assimilation. O m?todo excretion ? invocado a partir do mesmo m?todo e calcula
				a excre??o ao n?vel do individuo e da popula??o}
				*/
				Die();


				/*A f?rmula abaixo descrita calcula o fluxo liqu?do ao n?vel da popula??o de cada
				caixa*/

				ZooClassBiomassFlux[k * NumberOfBoxes + i]			//g dia-1 m-3
					= ZooClassBiomassFlux[k * NumberOfBoxes + i]
					  + (ZooIndividualFlux[k * NumberOfBoxes + i]
					  * DAYSTOHOURS)
					  * (ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooIndividualWeight[k * NumberOfBoxes + i]);

				/*if seston_ingestion > 0 then
				ecological_efficiency^[i,k] := (ZooClassBiomass_flux^[i,k]*time_conv/
				(seston_ingestion*org_seston_mass/total_seston_mass))*100;
				if org_seston_mass > 0 then
				food_chain_efficiency^[i,k] := (ZooClassBiomass_flux^[i,k]/total_seston_mass)*100* time_conv;*/
			}
		}
	}
}


void TCrassostreaGigasEmpirical::Feed()
{
double phyto_mass, phyto_biomass,
			 phyto_filtration,
			 nonliving_seston_filtration, delta,
			 ChlorophyllCarbonRatio, ChlorophyllToBiomass;

const double  aFR = 1.309;

	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
				 *	MySPMPointer = MyPEcoDynClass->GetSPMPointer();

	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{

			POC_mass = 0; seston_mass = 0;


			if (ZooClassBiomass[k * NumberOfBoxes + i] <= 0)
				SestonFiltration[k * NumberOfBoxes + i] = 0;
			else
			{
				if (MyPhytoPointer != NULL)
				{
					MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,
														 i,
														 "Phytoplankton biomass",
														 ObjectCode);
					MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllCarbonRatio,
														 i,
														 "Chlorophyll : Carbon",
														 ObjectCode);
					MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,
														 i,
														 "Chlorophyll to biomass",
														 ObjectCode);
					// Phytoplankton returns its mass as ug chl a l-1
					// convert to mg l-1 and mg C l-1, respectively
					phyto_biomass = phyto_mass * ChlorophyllToBiomass /
										 CUBIC; //mg/l
					phyto_mass = phyto_mass / (ChlorophyllCarbonRatio *
									 CUBIC); //mg C l-1
				}

				if (MySPMPointer != NULL)
				{
					MySPMPointer->Inquiry(GetEcoDynClassName(), seston_mass,
													  i,
													  "Suspended matter",
													  ObjectCode);
					MySPMPointer->Inquiry(GetEcoDynClassName(), POC_mass,
													  i,
													  "Particulate organic carbon",
													  ObjectCode);
				}
				org_seston_mass[k * NumberOfBoxes + i] = phyto_mass + POC_mass;
				total_seston_mass[k * NumberOfBoxes + i] = phyto_biomass + seston_mass;
				POMa[i]  = POC_mass
							 * POCtoPOM
							 + phyto_biomass;

			  //Chla a expressa em mgCm-3
			  //Biomassa da Crassostrea expressa em gDW/m3

				if (total_seston_mass[k * NumberOfBoxes + i] <= 0)
					SestonFiltration[k * NumberOfBoxes + i] = 0;
				else
				{

					//Rela??o alom?trica para c?lculo da taxa de filtra??o

					filtration_rate = pow( total_seston_mass[k * NumberOfBoxes + i], aFR);  // mg/h/g
														//* exp(Filtration_coef
														//*log(ZooIndividualWeight[k * NumberOfBoxes + i]));



					/*SestonFiltration /*g/h = filtration_rate *
												  (total_seston_mass)/cubic;*/
					if (MyPhytoPointer != NULL)
					{
						phyto_filtration        // mg C day-1 m-3
							= filtration_rate
							  / total_seston_mass[k * NumberOfBoxes + i]
							  * phyto_mass
							  * exp(Filtration_coef
							  * log(ZooIndividualWeight[k * NumberOfBoxes + i]))
							  //* phyto_mass
							  * DAYSTOHOURS
							  * (ZooClassBiomass[k * NumberOfBoxes + i]
								  /ZooIndividualWeight[k * NumberOfBoxes + i]);



						if (phyto_filtration * TimeStep > phyto_mass * CUBIC)
						{
#ifdef __BORLANDC__
							::MessageBox(0, "Attention!", "EcoDynamo alert from zoobenthos", MB_OK);
							Debugger(phyto_filtration);
							Debugger (filtration_rate);
#else  // __BORLANDC__
							cerr << "TCrassostreaGigasEmpirical::Feed - Attention! EcoDynamo alert from zoobenthos. phyto_filtration=" << phyto_filtration << ". filtration_rate=" << filtration_rate << endl;
#endif  // __BORLANDC__
							phyto_filtration = phyto_mass
													 *
													 CUBIC
													 / TimeStep;


						}

						MyPhytoPointer->Update(GetEcoDynClassName(), -phyto_filtration,    // mg day-1 m-3
														i,
														"Phytoplankton biomass",
														ObjectCode);
					}


					if (MySPMPointer != NULL)
					{
						nonliving_seston_filtration      // mg dia-1 m-3
							= filtration_rate
							  / total_seston_mass[k * NumberOfBoxes + i]
							  * seston_mass
							  * exp(Filtration_coef
							  * log(ZooIndividualWeight[k * NumberOfBoxes + i]))
							  * DAYSTOHOURS
							  * (ZooClassBiomass[k * NumberOfBoxes + i]
								  / ZooIndividualWeight[k * NumberOfBoxes + i]);

						if (nonliving_seston_filtration * TimeStep > seston_mass * CUBIC)
						{
#ifdef __BORLANDC__
							::MessageBox(0, "Attention! SPM", "EcoDynamo alert from zoobenthos", MB_OK);
							Debugger(nonliving_seston_filtration);
							Debugger (filtration_rate);
#else  // __BORLANDC__
							cerr << "TCrassostreaGigasEmpirical::Feed - Attention! EcoDynamo alert from zoobenthos. nonliving_seston_filtration=" << nonliving_seston_filtration << ". filtration_rate=" << filtration_rate << endl;
#endif  // __BORLANDC__
							nonliving_seston_filtration
								= seston_mass
								  / TimeStep;
						}

						MySPMPointer->Update(GetEcoDynClassName(), -nonliving_seston_filtration / CUBIC, // to return in mg l-1 PD & JGF 96.06.26
													 i,
													 "Suspended matter",
													 ObjectCode);
					}

					/*
					ZooIndividualFlux[k * NumberOfBoxes + i]            // mg/g/h
						= ZooIndividualFlux[k * NumberOfBoxes + i]
						  + filtration_rate;
					*/

					SestonFiltration[k * NumberOfBoxes + i] = filtration_rate;

				}
			}

			} // End of if

		}
	}
}

void TCrassostreaGigasEmpirical::Ingestion()
{
const double aIR = 14.375, bIR = 1.392,
				 aSE = 0.086, bSE = 0.571, cSE = 0.787,
				 aOAR = 1.139, bOAR = -0.125;

double IngestionRate, OrganicIngestionRate, OrganicAssimilationRate;

	for (int k = 0; k < NumberOfClasses; k++)
		for (int i = 0; i < NumberOfBoxes; i++)
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{
				IngestionRate
					= aIR
					  + bIR * SestonFiltration[k * NumberOfBoxes + i]
					  * POMa[i]
					  / total_seston_mass[k * NumberOfBoxes + i];
//				Debugger(IngestionRate);
				SelectionEfficiency[k * NumberOfBoxes + i]
					= aSE
					  * pow(SestonFiltration[k * NumberOfBoxes + i],bSE)
					  * pow(POMa[i] / total_seston_mass[k * NumberOfBoxes + i], cSE);
//				Debugger(SelectionEfficiency[k * NumberOfBoxes + i]);
				OrganicIngestionRate
					= SestonFiltration[k * NumberOfBoxes + i]
					  * POMa[i] / total_seston_mass[k * NumberOfBoxes + i]
					  * ((1 - SelectionEfficiency[k * NumberOfBoxes + i])
					  * IngestionRate
					  / SestonFiltration[k * NumberOfBoxes + i]
					  + SelectionEfficiency[k * NumberOfBoxes + i]);
/*
OrganicIngestionRate = SestonFiltration[k * NumberOfBoxes + i]
											  * POMa[i] / total_seston_mass[k * NumberOfBoxes + i]
											  * ((1 - SelectionEfficiency[k * NumberOfBoxes + i])
													  * ZooIndividualFlux[k * NumberOfBoxes + i]
													  / SestonFiltration[k * NumberOfBoxes + i]
													+ SelectionEfficiency[k * NumberOfBoxes + i]);
*/
//				Debugger(OrganicIngestionRate);
				OrganicAssimilationRate = OrganicIngestionRate
												  * (aOAR + bOAR
													  / OrganicIngestionRate
													  * IngestionRate);

//				Debugger(OrganicAssimilationRate);
				ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i]
																		 + OrganicAssimilationRate
																		 * pow(ZooIndividualWeight[k * NumberOfBoxes + i], Filtration_coef); //mg/h/ind

				ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i]
																		 * Oyster_g_cal
																		 * cal_joules; //J/h/ind
				ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i];

			}
}



void TCrassostreaGigasEmpirical::Pseudofaeces()
{
const double aPF = -14.377, bPF = 1.048, cPF = -1.392;

	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{

			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{


			pseudofaeces_production[k * NumberOfBoxes + i] = aPF
																			 + bPF * SestonFiltration[k * NumberOfBoxes + i]
																			 + cPF
																			 * SestonFiltration[k * NumberOfBoxes + i]
																			 * POMa[i]
																			 / total_seston_mass[k * NumberOfBoxes + i];
			pseudofaeces_production[k * NumberOfBoxes + i] = pseudofaeces_production[k * NumberOfBoxes + i];

			} // end of if

		}
	}
}

void TCrassostreaGigasEmpirical::Assimilate()
{
}

void TCrassostreaGigasEmpirical::Respire()
{
double  MyWaterTemperature;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

const double aRER = -0.432, bRER = 0.613, cRER = 1.042, dRER = 0.8,
				 MolarMass = 15.9994 * 2, // One O2 molecule weighs 15.9994 * 2 g
				 MolarVolume = 22.4, // litres
				 GasConversionFactor = MolarMass/MolarVolume; // 15.9994 * 2 g

	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{

				if (MyWaterTemperaturePointer != NULL)
					MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
																		i,
																		"Water temperature",
																		ObjectCode);
				else
				{
#ifdef __BORLANDC__
					MessageBox(0,
								  "Water temperature object is undefined",
								  "EcoDynamo alert",
								  MB_OK);
#else  // __BORLANDC__
					cerr << "TCrassostreaGigasEmpirical::Respire - water temperature object undefined." << endl;
#endif  // __BORLANDC__
					return;
				}
				standard_respiration = (aRER
											  + bRER * pow(cRER, MyWaterTemperature))
											  / GasConversionFactor;// mlO2/h/g DW

				standard_respiration = standard_respiration
											  * cal_joules
											  * Oxygen_ml_cal;      //J/h/g


				total_respiration = standard_respiration
										  * pow(ZooIndividualWeight[k * NumberOfBoxes + i], dRER); //J/h/ind
				/*

				O consumo de oxig?nio ? convertido em cal, de modo a poder calcular o consumo equivalente de
				biomassa. Na f?rmula abaixo descrita, o zoo-flux ? reconvertido a g para poder ser utilizado
				no m?todo "production" para actualiza??o da biomassa individual e populacional.

				*/
//				if (ZooIndividualFlux[k * NumberOfBoxes + i] < (total_respiration * meat_conversion))
//					::MessageBox(0, "Hello...","",MB_OK);
				ZooIndividualFlux[k * NumberOfBoxes + i]
					= ZooIndividualFlux[k * NumberOfBoxes + i]
					  - total_respiration;   //J/h/ind

				ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i]
																		 / cal_joules
																		 / Oyster_g_cal;
				ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i];

			} // of if
		}
	}
}


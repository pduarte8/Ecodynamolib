// Phytoplankton object CPP code file
// @ Pedro Duarte
// Rev. A 00.09.25
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>
	#include <cmath> 
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "phytobjt.h"
#include "iodll.h"

// TCrestumaLeverPhytoplankton Class
//
// Constructors invoked outside EcoDyn shell...
//

TCrestumaLeverPhytoplankton::TCrestumaLeverPhytoplankton(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
        double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
        double aChlCarbonRatio[], double aChlToBiomass[],
        double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
        double simulationInterval,
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration)
    :TPhytoplanktonBasic(className, timeStep, nLines, nColumns,
            aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, aChlToCarbon, riverPhyto, oceanPhyto, aPhytoBiomass,
            aPhytoLoad, aChlCarbonRatio, aChlToBiomass,
            nParams, pmax, iopt, slope, aEiler, bEiler, cEiler, kValue, ks,
            kAmmonia, kNitrate, pHi, maintenanceRespiration, respirationCoefficient,
            docLoss, docStressLoss, deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate, fallingSpeed,
            ratioLightDarkRespiration)
{
    PhytoplanktonInitialBiomass = phytoplanktonInitialBiomass * ChlToCarbon[0];
    PhytoplanktonFinalBiomass = phytoplanktonFinalBiomass * ChlToCarbon[0];
	SimulationInterval = simulationInterval;
}

TCrestumaLeverPhytoplankton::TCrestumaLeverPhytoplankton(char* className,
                float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
        :TPhytoplanktonBasic(className, timeStep, morphologyFilename,
                variablesFilename, parametersFilename)
{
    BuildCrestumaLeverPhyto();
}
// ...

TCrestumaLeverPhytoplankton::TCrestumaLeverPhytoplankton(TEcoDynClass* APEcoDynClass,
        char* className) : TPhytoplanktonBasic(APEcoDynClass, className)
{
    BuildCrestumaLeverPhyto();
}

void TCrestumaLeverPhytoplankton::BuildCrestumaLeverPhyto()
{
	PhytoplanktonInitialBiomass = -99.0;
    PhytoplanktonFinalBiomass = -99.0;

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        int X, Y;

        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            // NumberOfVariables and VariableNameArray already filled
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                if (strcmp(MyParameter, "Phytoplankton initial biomass") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PhytoplanktonInitialBiomass);
                    PhytoplanktonInitialBiomass = PhytoplanktonInitialBiomass * ChlToCarbon[0];
                }
                else if (strcmp(MyParameter, "Phytoplankton final biomass") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PhytoplanktonFinalBiomass);
                    PhytoplanktonFinalBiomass = PhytoplanktonFinalBiomass * ChlToCarbon[0];
                }
                else if (strcmp(MyParameter, "Simulation interval (hours)") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SimulationInterval);
                }
          }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - TCrestumaLeverPhytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverPhytoplankton::BuildCrestumaLeverPhyto - Variables: Undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

// AP, 2007.07.12
bool TCrestumaLeverPhytoplankton::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Phytoplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Phytoplankton");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
            PReadWrite->WriteSeparator();

        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Phytoplankton biomass") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverPhyto / ChlCarbonRatio[0], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanPhyto / ChlCarbonRatio[0], 8);
        }

        // variables from TCrestumaLeverPhytoplankton
        else if (strcmp(VariableNameArray[i], "Phytoplankton initial biomass") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(PhytoplanktonInitialBiomass / ChlToCarbon[0], 8);
        }
        else if (strcmp(VariableNameArray[i], "Phytoplankton final biomass") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(PhytoplanktonFinalBiomass / ChlToCarbon[0], 8);
        }
        else if (strcmp(VariableNameArray[i], "Simulation interval (hours)") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(SimulationInterval, 2);
        }

        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phytoplankton load values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Chlorophyll : Carbon values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Chlorophyll to Carbon values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Chlorophyll to biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PhytoLoad[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ChlCarbonRatio[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ChlToBiomass[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TCrestumaLeverPhytoplankton::Go ()
{
	double MyPOMFlux, MyBoxDepth, LimitingNutrient1, LimitingNutrient2;
	// Active methods
   JulianDay = MyPEcoDynClass->GetJulianDay();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   for (int i = 0; i < NumberOfBoxes; i++)
	{
   	if (MyTransportPointer != NULL)
		{
   		MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
      											  i,
            	                          "Box depth",
               	                       ObjectCode);
   	}
      else
      	MyBoxDepth = MyPEcoDynClass->GetBoxDepth(i);
      if (
      		(MyBoxDepth > 0.0)
         )
      {
      /*if ((MyPEcoDynClass->GetCurrentTime() > 4.49) && (i == 19))
      {
      	Debugger(i);
      	Debugger(1);
      } */
   		Production(i);
			Respiration(i);
			Exudation(i);
			Mortality(i);
      }
   }


	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
	}
   if ((PhytoplanktonInitialBiomass >= 0.0) && (PhytoplanktonFinalBiomass >= 0.0))
   {
      //Debugger(MyPEcoDynClass->GetTimeStep());
   	RiverPhyto = RiverPhyto +  (PhytoplanktonFinalBiomass - PhytoplanktonInitialBiomass) / SimulationInterval *
                   MyPEcoDynClass->GetTimeStep() * DAYSTOHOURS;

   }
}



void TCrestumaLeverPhytoplankton::Production(int ABoxNumber)
{
	double BoxDepth, LightAtTop, LightAtBottom;
   int MyBoxNumber;

   MyBoxNumber = ABoxNumber;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				 * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
             * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
      		if (MyLightPointer != NULL)
	 			{
					if (MyTransportPointer != NULL)
      				MyTransportPointer->Inquiry(GetEcoDynClassName(), BoxDepth,
            			                             MyBoxNumber,
               			                          "Box depth",
                  			                       ObjectCode);
         		else
         			BoxDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);

					if (MySPMPointer != NULL)
						MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
														  MyBoxNumber,
														  "Extinction coefficient Ktot",
														  ObjectCode);



            	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												       MyBoxNumber,
												       "PAR surface irradiance",
												       ObjectCode);
            	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												       BoxDepth,
												       MyBoxNumber,
												       "Sub-surface PAR irradiance",
												       ObjectCode);

		 			if ((BoxDepth > 0.0) && (LightAtTop > 0.0))
            	{
		 				Productivity = PhytoBiomass[MyBoxNumber]
										 * Pmax[MyBoxNumber]
										 * 2.718282
                     	       /(KValue * BoxDepth)
							 			* ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
								  		-exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
							  			)
							  			* DAYSTOHOURS;
                  //if ((MyPEcoDynClass->GetCurrentTime() > 4.49) && (MyBoxNumber == 19))
                  //Debugger(Productivity);
                  TemperatureLimitation(MyBoxNumber);
                  //if ((MyPEcoDynClass->GetCurrentTime() > 4.49) && (MyBoxNumber == 19))
                  //Debugger(Productivity);
		 				NutrientLimitation(MyBoxNumber);
                  //if ((MyPEcoDynClass->GetCurrentTime() > 4.49) && (MyBoxNumber == 19))
                  //Debugger(Productivity);
                  //Debugger (Productivity);
           		}
               else Productivity = 0.0;
               GPP[MyBoxNumber] = Productivity;
               PhytoProd[MyBoxNumber] = Productivity;
            }
         }
}



void TCrestumaLeverPhytoplankton::Respiration(int ABoxNumber)
{
	double Resp;
   int MyBoxNumber;

   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
				if (PhytoProd[MyBoxNumber] > 0.0)
      			Resp = PhytoProd[MyBoxNumber] * RespirationCoefficient;
      		else
      			Resp = MaintenanceRespiration[MyBoxNumber] * PhytoBiomass[MyBoxNumber];
      		PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - ( Resp );

            if (MyNutrientPointer != NULL)
            {
            	MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (RedfieldCFactor * NITROGENATOMICWEIGHT/RedfieldNFactor),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
               MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (RedfieldCFactor * PHOSPHORUSATOMICWEIGHT/RedfieldPFactor),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
            }
      		//OxygenNetProduction[MyBoxNumber] = OxygenNetProduction[MyBoxNumber] -
            //		                                     Resp / CarbonToOxygenResp;
         }
}

void TCrestumaLeverPhytoplankton::Exudation(int ABoxNumber)
{
   double  Exudate;
   int MyBoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   MyBoxNumber = ABoxNumber;

         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
				DocStressLoss = DocLoss;
				if (GPP[MyBoxNumber] > 0.0)
					Exudate = GPP[MyBoxNumber] * DocStressLoss;
				else Exudate = 0.0;
					PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - Exudate;
            if (MyNutrientPointer != NULL)
            {
            	MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (RedfieldCFactor * NITROGENATOMICWEIGHT/RedfieldNFactor),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
               MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (RedfieldCFactor * PHOSPHORUSATOMICWEIGHT/RedfieldPFactor),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
            }
			}
}


void TCrestumaLeverPhytoplankton::Mortality(int ABoxNumber)
{
	double Release = 0;
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	int MyBoxNumber;

   MyBoxNumber = ABoxNumber;

   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
		Release = PhytoBiomass[MyBoxNumber] * DeathLoss;

		PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - Release;

		PhytoMortality[MyBoxNumber] = Release; // flux per day for mass balance

		if (MyNutrientPointer != NULL)
      {
			MyNutrientPointer->Update(GetEcoDynClassName(), Release * CUBIC / (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);
         MyNutrientPointer->Update(GetEcoDynClassName(), Release * CUBIC /
                                   (RedfieldCFactor * PHOSPHORUSATOMICWEIGHT/RedfieldPFactor),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
      }
	}
}



void TCrestumaLeverPhytoplankton::NutrientLimitation(int ABoxNumber)
{
	double LimitingNutrient1, LimitingNutrient2, LimitingNutrient3, LimitingNutrient4, ATimeStep;
   int MyBoxNumber;
   char MyLimitingNutrient[65];

   MyBoxNumber = ABoxNumber;
   ATimeStep = MyPEcoDynClass->GetTimeStep();

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	if (MyNutrientPointer != NULL)
	{
		// the limitation is based on umol l-1 values passed from the
		// communicate method
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient1,
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);
		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient2,
												 MyBoxNumber,
												 "Nitrate",
												 ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient3,
												 MyBoxNumber,
												 "Nitrite",
												 ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient4,
												 MyBoxNumber,
												 "Phosphate",
												 ObjectCode);
      //It is assumed that if there is enough inorganic nitrogen to sustain a certain production level then
      //there is no limitation. Whenever nitrogen is not enough for the light and temperature limited productivity
      //then productivity is reduced until the minimal value allowed by available nitrogen (the sum of ammonia and nitrate.
      //This may consume virtually all available inorganic nitrogen in the water
      if (
      		((LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) == 0.0) ||
            (LimitingNutrient4 == 0.0)
          )
          Productivity = 0.0;
      else
      {
      	//Debugger(Productivity);
         //Debugger(MyBoxNumber);
      	if (
      			(LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) / LimitingNutrient4 >
            	(RedfieldNFactor * NITROGENATOMICWEIGHT) / (RedfieldPFactor * PHOSPHORUSATOMICWEIGHT)
         	)
      		strcpy(MyLimitingNutrient,"Phosphate");
      	else
      		strcpy(MyLimitingNutrient,"DIN");

      	if (strcmp(MyLimitingNutrient,"Phosphate") == 0)
      	{
         	if (
            		Productivity /
               	(RedfieldCFactor*PHOSPHORUSATOMICWEIGHT/RedfieldPFactor) * ATimeStep >        //productivity expressed in umol P / L / timestep
               	LimitingNutrient4
         		)
            		Productivity = LimitingNutrient4 *
            		(RedfieldCFactor*PHOSPHORUSATOMICWEIGHT/RedfieldPFactor) / ATimeStep;  //productivity expressed in is normal units - mg C / m3 / day
      	}
      	else
      	if (strcmp(MyLimitingNutrient,"DIN") == 0)
      	{
      		if (
         			Productivity /
               	(RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor) * ATimeStep >        //productivity expressed in umol N / L / timestep
               	(LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3)
         		)

         	   	Productivity = (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) *
                        	(RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor) / ATimeStep;  //productivity expressed in is normal units - mg C / m3 / day

      	}
      	if (Productivity < 0.0000000001) //This is to avoid extremely small values that may lead to errors
      		Productivity = 0.0;


      	if (Productivity > 0.0)
      	{
      		MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * CUBIC /
                                       (RedfieldCFactor*PHOSPHORUSATOMICWEIGHT/RedfieldPFactor),
                                        MyBoxNumber,
                                        "Phosphate",
                                        ObjectCode);

      		if ((LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) > 0.0000000001)
      		MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * LimitingNutrient1 / (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) * CUBIC /
         	   		                       (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
															MyBoxNumber,
															"Ammonia",
															ObjectCode);
      		MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * LimitingNutrient2 / (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) * CUBIC /
         	   		                       (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
															MyBoxNumber,
															"Nitrate",
															ObjectCode);
            MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * LimitingNutrient3 / (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) * CUBIC /
         	   		                       (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
															MyBoxNumber,
															"Nitrite",
															ObjectCode);
			}
      }
	}
}



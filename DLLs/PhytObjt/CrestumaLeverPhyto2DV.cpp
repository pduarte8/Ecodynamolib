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

#ifndef _PORT_FORTRAN_
	#include "iodll.h"
#endif
#include "ecodyn.rh"
#include "phytobjt.h"

// TCrestumaLeverPhytoplankton2DV Class
//
// Constructors invoked outside EcoDyn shell...
//

#ifndef _PORT_FORTRAN_
TCrestumaLeverPhytoplankton2DV::TCrestumaLeverPhytoplankton2DV(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
        double chlToBiomass, double aPhytoBiomass[], double riverPhyto,
        double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
        double simulationInterval,
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration)
    :TPhytoplankton(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nParams, pmax, iopt, slope, aEiler, bEiler, cEiler, kValue, ks,
            kAmmonia, kNitrate, pHi, maintenanceRespiration, respirationCoefficient,
            docLoss, docStressLoss, deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate, fallingSpeed,
            ratioLightDarkRespiration)
{
    PreBuildCrestumaLeverPhyto2DV();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ChlCarbonRatio[i] = chlCarbonRatio;
		ChlToCarbon[i] = chlToCarbon;
		ChlToBiomass[i] = chlToBiomass;
		PhytoBiomass[i] = aPhytoBiomass[i];
	}
    RiverPhyto = riverPhyto;
    PhytoplanktonInitialBiomass = phytoplanktonInitialBiomass;
    PhytoplanktonFinalBiomass = phytoplanktonFinalBiomass;
	SimulationInterval = simulationInterval;

    PosBuildCrestumaLeverPhyto2DV();
}

TCrestumaLeverPhytoplankton2DV::TCrestumaLeverPhytoplankton2DV(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TPhytoplankton(className, timeStep, morphologyFilename, parametersFilename)
{
    strcpy(VarsFileName, variablesFilename);
    BuildCrestumaLeverPhyto2DV();
}
// ...

TCrestumaLeverPhytoplankton2DV::TCrestumaLeverPhytoplankton2DV(TEcoDynClass* APEcoDynClass,
        char* className): TPhytoplankton(APEcoDynClass, className)
{
    BuildCrestumaLeverPhyto2DV();
}

void TCrestumaLeverPhytoplankton2DV::PreBuildCrestumaLeverPhyto2DV()
{
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
}

void TCrestumaLeverPhytoplankton2DV::BuildCrestumaLeverPhyto2DV()
{
    PreBuildCrestumaLeverPhyto2DV();
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Phytoplankton variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrestumaLeverPhytoplankton2DV::BuildCrestumaLeverPhyto2DV - Phytoplankton variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        int X, Y, XV, YV;
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            char MyVariable[65], RiverName[100];
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
            {
                double MyValue;
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
            
                if (strcmp(MyVariable, "Chlorophyll : Carbon") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 2, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ChlCarbonRatio[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "Chlorophyll to Carbon") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 2, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ChlToCarbon[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "Chlorophyll to biomass") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 2, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ChlToBiomass[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "Phytoplankton biomass") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, RiverPhyto);

                    if (!PReadWrite->FindString("Phytoplankton biomass values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y + 1;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[j]);
                    }
                }
                else if (strcmp(MyVariable, "Phytoplankton initial biomass") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, PhytoplanktonInitialBiomass);
                }
                else if (strcmp(MyVariable, "Phytoplankton final biomass") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, PhytoplanktonFinalBiomass);
                }
                else if (strcmp(MyVariable, "Simulation interval (hours)") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 2, SimulationInterval);
                }
            //More variables here
            }

            if (PReadWrite->FindString("RiverLoadLines", XV, YV) == 0)
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].LineCoordinate);
                  ALoadRiverRecord[j].LineCoordinate = GridLines - ALoadRiverRecord[j].LineCoordinate;
                  //Debugger(ALoadRiverRecord[j].LineCoordinate);
               }
            }
            if (PReadWrite->FindString("RiverLoadColumns", XV, YV) == 0)
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].ColumnCoordinate);
                  ALoadRiverRecord[j].ColumnCoordinate = ALoadRiverRecord[j].ColumnCoordinate - 1;
                  //Debugger(ALoadRiverRecord[j].ColumnCoordinate);
               }
            }
            if (PReadWrite->FindString("RiverName", XV, YV) == 0)
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  PReadWrite->ReadString(XV, YV + 1 + j, RiverName);
                  strcpy(ALoadRiverRecord[j].RiverName, RiverName);
               }
            }
            if (PReadWrite->FindString("PhytoPlanktonBiomassRiverLoad", XV, YV) == 0)
            {
               for (int j = 0; j < NumberOfRiverLoads; j++)
               {
                  for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                  {
                     int offset = j * NumberOfDaysForLoads;
                     PReadWrite->ReadNumber(XV, YV + 1 + offset + Loads, ALoadRiverRecord[j].PhytoplanktonBiomassLoad[Loads]); //River Loads should be in mg / L in fact they are concentrations
                  }
               }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Crestuma Lever 2DV Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverPhytoplankton2DV::BuildCrestumaLeverPhyto2DV - Variables: Undefined object - Crestuma Lever 2DV Phytoplankton." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    PosBuildCrestumaLeverPhyto2DV();
}

void TCrestumaLeverPhytoplankton2DV::PosBuildCrestumaLeverPhyto2DV()
{
    RiverPhyto = RiverPhyto * ChlToCarbon[0];
    PhytoplanktonInitialBiomass = PhytoplanktonInitialBiomass * ChlToCarbon[0];
    PhytoplanktonFinalBiomass = PhytoplanktonFinalBiomass * ChlToCarbon[0];
    for (int i = 0; i < NumberOfBoxes; i++)
   	    PhytoBiomass[i] = PhytoBiomass[i] * ChlToCarbon[i];

    PastUpperLayer = GridLines - 1;
    CurrentUpperLayer = GridLines - 1;
}


#else
TCrestumaLeverPhytoplankton2DV::TCrestumaLeverPhytoplankton2DV(char* className)
        :TPhytoplankton(className)
{
}

TCrestumaLeverPhytoplankton2DV::~TCrestumaLeverPhytoplankton2DV()
{
    try {    if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
    }
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
}
#endif  // _PORT_FORTRAN_



// AP, 2007.07.12
bool TCrestumaLeverPhytoplankton2DV::SaveVariables()
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
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' values
     *
     * 1st line
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Phytoplankton biomass") == 0)
            PReadWrite->WriteCell(RiverPhyto / ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton initial biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonInitialBiomass, 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton final biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonFinalBiomass, 8);
    }
    PReadWrite->WriteSeparator(true);
    /*
     * 2nd line
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Chlorophyll : Carbon") == 0)
            PReadWrite->WriteCell(ChlCarbonRatio[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to biomass") == 0)
            PReadWrite->WriteCell(ChlToBiomass[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Simulation interval (hours)") == 0)
            PReadWrite->WriteCell(SimulationInterval, 8);

    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save loads values
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("RiverLoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverName");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        PReadWrite->WriteCell(GridLines - ALoadRiverRecord[j].LineCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRiverRecord[j].ColumnCoordinate + 1, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRiverRecord[j].RiverName);
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("PhytoPlanktonBiomassRiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonBiomassLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TCrestumaLeverPhytoplankton2DV::Go ()
{
   double MyBoxDepth, ALayer, MyPreviousSurfVolume, MyCurrentSurfVolume;
	// Active methods
   JulianDay = MyPEcoDynClass->GetJulianDay();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyTransportPointer != NULL)
   {
   	MyTransportPointer->Inquiry(GetEcoDynClassName(), ALayer,
                                      0,
                                      "Upper layer",
                                      ObjectCode);

      CurrentUpperLayer = floor(ALayer);
      if (CurrentUpperLayer != PastUpperLayer)
      {
      	for (int j = 0; j < GridColumns; j++)
      	{
            if (CurrentUpperLayer < PastUpperLayer)
            {
      			MyTransportPointer->Inquiry(GetEcoDynClassName(), MyPreviousSurfVolume,
			  											     PastUpperLayer * GridColumns + j,
               		                          "Past box volume",
                  		                       ObjectCode);

            	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyCurrentSurfVolume,
			  											     CurrentUpperLayer * GridColumns + j,
               		                          "Past box volume",
                  		                       ObjectCode);
            	PhytoBiomass[CurrentUpperLayer * GridColumns + j] =
            		(
            			PhytoBiomass[PastUpperLayer * GridColumns + j] * MyPreviousSurfVolume +
               		PhytoBiomass[CurrentUpperLayer * GridColumns + j] * MyCurrentSurfVolume
            		) / (MyPreviousSurfVolume + MyCurrentSurfVolume);
            }
            else if (CurrentUpperLayer > PastUpperLayer)
            	PhytoBiomass[CurrentUpperLayer * GridColumns + j] = PhytoBiomass[PastUpperLayer * GridColumns + j];
      	}
      	PastUpperLayer = CurrentUpperLayer;
      }
   }

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





void TCrestumaLeverPhytoplankton2DV::Production(int ABoxNumber)
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



void TCrestumaLeverPhytoplankton2DV::Respiration(int ABoxNumber)
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

void TCrestumaLeverPhytoplankton2DV::Exudation(int ABoxNumber)
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


void TCrestumaLeverPhytoplankton2DV::Mortality(int ABoxNumber)
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



void TCrestumaLeverPhytoplankton2DV::NutrientLimitation(int ABoxNumber)
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

void TCrestumaLeverPhytoplankton2DV::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    /*if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1; */

    ADay = floor(MyPEcoDynClass->GetTime() + 1);

    for (int j = 0; j < NumberOfLoads; j++)
    {
        index = ALoadRecord[j].LineCoordinate * GridColumns
                + ALoadRecord[j].ColumnCoordinate;
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TCrestumaLeverPhytoplankton2DV::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TCrestumaLeverPhytoplankton2DV::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << ")\n\nPlease correct configuration files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        //if (BoxArray[index].Dummybool2)
        if (Depth > 0.0)
        {
            PhytoLoad[index] = ALoadRecord[j].PhytoplanktonCLoad[ADay-1];
            /*PhytoNLoad[index] = ALoadRecord[j].PhytoplanktonNLoad[ADay-1];
            PhytoPLoad[index] = ALoadRecord[j].PhytoplanktonPLoad[ADay-1];  */
        }
    }

    //Dado que se assume neste modelo que a concentra??o na fronteira montante ? constante em profundidade,
    //dependendo somente do dia do ano, n?o ? necess?rio mais do que um valor de cargas para actualizar o RiverPhyto
    int j;
    j = 0;
    do {
        index = ALoadRiverRecord[j].LineCoordinate * GridColumns
                + ALoadRecord[j].ColumnCoordinate;
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TCrestumaLeverPhytoplankton2DV::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TCrestumaLeverPhytoplankton2DV::Loads (Rivers) - Index " << index << " > Number of Boxes (" << NumberOfBoxes << ")\n\nPlease correct configuration files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        j++;
    }  while ((Depth < 0.0) || (j == NumberOfRiverLoads));
    RiverPhyto = ALoadRiverRecord[j - 1].PhytoplanktonBiomassLoad[ADay-1] * ChlToCarbon[j - 1];
    /*for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        index = ALoadRiverRecord[j].LineCoordinate * GridColumns
                + ALoadRecord[j].ColumnCoordinate;
        if (index >= NumberOfBoxes) {
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TCrestumaLeverPhytoplankton2DV::Loads()",
                      MB_OK | MB_ICONHAND);
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        if (Depth > 0.0)
        {
            RiverPhyto = ALoadRiverRecord[j].PhytoplanktonBiomassLoad[ADay-1] * ChlToCarbon[index];
        }
    }*/
}


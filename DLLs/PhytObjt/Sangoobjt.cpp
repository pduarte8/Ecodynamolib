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

// TSangoPhytoplankton Class
//
// Constructors invoked outside EcoDyn shell...
//

TSangoPhytoplankton::TSangoPhytoplankton(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
        double chlToBiomass, double aPhytoBiomass[], double aDailyBoundaryPhytoplankton[],
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
    PreBuildSangoPhytoplankton();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ChlCarbonRatio[i] = chlCarbonRatio;
		ChlToCarbon[i] = chlToCarbon;
		ChlToBiomass[i] = chlToBiomass;
		PhytoBiomass[i] = aPhytoBiomass[i] * ChlToCarbon[i];
	}
	for (int i = 0; i < 365; i++)
	{
		DailyBoundaryPhytoplankton[i] = aDailyBoundaryPhytoplankton[i];
    }
}

TSangoPhytoplankton::TSangoPhytoplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TPhytoplankton(className, timeStep, morphologyFilename, parametersFilename)
{
    strcpy(VarsFileName, variablesFilename);
    BuildSangoPhytoplankton();
}
// ...

TSangoPhytoplankton::TSangoPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
              : TPhytoplankton(APEcoDynClass, className)
{
    BuildSangoPhytoplankton();
}

void TSangoPhytoplankton::PreBuildSangoPhytoplankton()
{
	WesternBoundaryNumber = 4;
    SouthernBoundaryNumber = 3;
    EasternBoundaryNumber = 2;
    NorthernBoundaryNumber = 1;

    DailyBoundaryPhytoplankton  = new double[365];
    for (int i = 0; i < 365; i++)
        DailyBoundaryPhytoplankton[i] = 0;

    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();

    for (int i = 0; i < GridLines; i++)
   	    BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary = 2;

    ATimeStep = MyPEcoDynClass->GetTimeStep();
}

void TSangoPhytoplankton::BuildSangoPhytoplankton()
{
    PreBuildSangoPhytoplankton();

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Phytoplankton variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoPhytoplankton::BuildSangoPhytoplankton - Phytoplankton variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        int X, Y, XV, YV;
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            char MyVariable[65];
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
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ChlCarbonRatio[j] = MyValue;
                    }

                }
                else if (strcmp(MyVariable, "Chlorophyll to Carbon") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ChlToCarbon[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "Chlorophyll to biomass") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        ChlToBiomass[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "Phytoplankton biomass") == 0)
                {
                    if (!PReadWrite->FindString("Phytoplankton biomass values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[j]);
                        PhytoBiomass[j] = PhytoBiomass[j] * ChlToCarbon[j];
                    }
                }
                else if (strcmp(MyVariable, "Daily boundary phytoplankton") == 0)
                {
                    if (!PReadWrite->FindString("Daily boundary phytoplankton values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyBoundaryPhytoplankton[j]);
                        /// ?????????????
                        DailyBoundaryPhytoplankton[j] = DailyBoundaryPhytoplankton[j] * ChlToCarbon[0];
                    }
                }
                //More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables:Undefined object - SangoPhytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoPhytoplankton::BuildSangoPhytoplankton - Variables:Undefined object - SangoPhytoplankton" << endl;
#endif  // __BORLANDC__
       }
        CloseDataFile((void*)PReadWrite);
    }
}


TSangoPhytoplankton::~TSangoPhytoplankton()
{
    freeMemory();
}

void TSangoPhytoplankton::freeMemory()
{
    try {
        delete [] DailyBoundaryPhytoplankton;
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
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
				cerr << "TSangoPhytoplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TSangoPhytoplankton::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Phytoplankton biomass") == 0)
	    Value = PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton C biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton N biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber] * RedfieldNFactor / RedfieldCFactor;
	else if (strcmp(MyParameter, "Phytop. total GPP") == 0)
		Value = GPP[MyBoxNumber];
	else if (strcmp(MyParameter, "Phytop. total NPP") == 0)
		Value = NPP[MyBoxNumber];
	else if (strcmp(MyParameter, "Phytop. GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
			Value = GPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTODAYS;              // mg C/ mg C /h
    }
	else if (strcmp(MyParameter, "Phytop. NPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
		    Value = NPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTODAYS;
    }
	else if (strcmp(MyParameter, "Chlorophyll : Carbon") == 0)
		Value = ChlCarbonRatio[MyBoxNumber];
	else if (strcmp(MyParameter, "Chlorophyll to biomass") == 0)
        Value = ChlToBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Phyto NPP/GPP ratio") == 0)
	{
		if (GPP[MyBoxNumber]!= 0)                             //NG 5/6/96
		    Value = NPP[MyBoxNumber]/GPP[MyBoxNumber]*100;
		else
			Value = INFINITY;
	}
	else if (strcmp(MyParameter, "Phyto PMax*RTPH") == 0)
		Value = RTPH * Pmax[MyBoxNumber];
	else if (strcmp(MyParameter, "Phyto Update") == 0)
		Value = PhytoBalance[MyBoxNumber];
	else if (strcmp(MyParameter, "N Counter") == 0)
		Value = NCounter;
	else if (strcmp(MyParameter, "P Counter") == 0)
		Value = PCounter;
	else if (strcmp(MyParameter, "Phytoplankton mortality") == 0)
		Value = PhytoMortality[MyBoxNumber];
    else if (strcmp(MyParameter, "Chlorophyll to Carbon") == 0)
		Value = ChlToCarbon[MyBoxNumber];
    else if (strcmp(MyParameter, "Daily boundary phytoplankton") == 0)
		Value = DailyBoundaryPhytoplankton[MyPEcoDynClass->GetJulianDay() - 1];
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
                      "EcoDynamo Alert - Inquiry 4",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSangoPhytoplankton::Inquiry 4 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TSangoPhytoplankton::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Phytoplankton C biomass") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "Phytop. total GPP") == 0)
        GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "Phytop. total NPP") == 0)
        NPP[BoxNumber] = Value;
	else if (strcmp(VariableName, "Chlorophyll : Carbon") == 0)
		ChlCarbonRatio[BoxNumber] = Value;
	else if (strcmp(VariableName, "Chlorophyll to biomass") == 0)
        ChlToBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Phyto PMax*RTPH") == 0)
		Pmax[BoxNumber] = Value / RTPH;
	else if (strcmp(VariableName, "Phyto Update") == 0)
		PhytoBalance[BoxNumber] = Value;
	else if (strcmp(VariableName, "N Counter") == 0)
		NCounter = Value;
	else if (strcmp(VariableName, "P Counter") == 0)
		PCounter = Value;
	else if (strcmp(VariableName, "Phytoplankton mortality") == 0)
		PhytoMortality[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll to Carbon") == 0)
		ChlToCarbon[BoxNumber] = Value;
    else if (strcmp(VariableName, "Daily boundary phytoplankton") == 0)
    {
        for (int i = 0; i < 365; i++)
        {
            DailyBoundaryPhytoplankton[i] = Value;
        }
    }
    else
        rc = false;

    return rc;
}


// AP, 2007.07.12
bool TSangoPhytoplankton::SaveVariables()
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
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save daily boundary values
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Daily boundary phytoplankton values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(DailyBoundaryPhytoplankton[j] / ChlToCarbon[0], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TSangoPhytoplankton::Go ()
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
      		(MyBoxDepth > 0.0) &&
      	   (BoxArray[i].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
            (BoxArray[i].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
            (BoxArray[i].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            (BoxArray[i].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)  ||  //No northern ocean boundary
            (
         		(MyBoxDepth > 0.0) &&
            	(MyTransportPointer == NULL)
         	)
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

      TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
		if (MySPMPointer != NULL)
		{

      	MyPOMFlux = PhytoFlux[i] * ChlCarbonRatio[i] * // mg Chl / m3 / d
                     ChlToBiomass[i] / CUBIC /*/ DAYSTOHOURS / HOURSTOSECONDS*/; //convertion from mg Chl / m3 / d to mg DW / L / s

   		MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             i,
            	   	          "POM",
               	             ObjectCode);
         MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             i,
            	   	          "TPM",
               	             ObjectCode);

   	}

      /*TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
		if (MyNutrientPointer != NULL)
		{
      		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient1,
												 i,
												 "Ammonia",
												 ObjectCode);
				MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient2,
												 i,
												 "Nitrate",
												 ObjectCode);
				// return negative value because this is subtracted when productivity > 0
            //Ammonia and Nitrate are removed when NPP > 0 at proportionatelly to their reespective concentrations
            if (NPP[i] > 0.0)
            {
					MyNutrientPointer->Update(GetEcoDynClassName(), -NPP[i] * LimitingNutrient1 / (LimitingNutrient1 + LimitingNutrient2) * CUBIC /
            		                       (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
														i,
														"Ammonia",
														ObjectCode);
         		MyNutrientPointer->Update(GetEcoDynClassName(), -NPP[i] * LimitingNutrient2 / (LimitingNutrient1 + LimitingNutrient2) * CUBIC /
            		                       (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
														i,
														"Nitrate",
														ObjectCode);
            }
            else    //Only ammonia is added when NPP < 0 assuming that rejected nitrogen is in the form of ammonia
               MyNutrientPointer->Update(GetEcoDynClassName(), NPP[i] * CUBIC /
            		                       (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
														i,
														"Ammonia",
														ObjectCode);
      } */
	}
}

void TSangoPhytoplankton::Integrate()
{
	// Integrate phytoplankton biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;

   //PhytoProd = GenericFlux;
   for (int i = 0; i < NumberOfBoxes; i++)
   	if (PhytoBiomass[i] < 0.0) PhytoBiomass[i] = 0.0;
	// Transport the phytoplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
	  MyTransportPointer->Go(PhytoBiomass, PhytoBiomass, 0.0, 0.0);
      double ABoxDepth;
      JulianDay = MyPEcoDynClass->GetJulianDay();
   	//Southern boundary
   	int i,j;
   	for (j = 0; j < GridColumns; j++)
   	{
   		i = 0;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
  	 		if (
   				(ABoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
   			)
      	{
      		PhytoBiomass[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1];
      	}
		}
      //Northern boundary
   	for (j = 0; j < GridColumns; j++)
   	{
   		i = GridLines - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
      	if (
   				(ABoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
   			)
      	{
      		PhytoBiomass[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1];
      	}
   	}
      //Eastern boundary
   	for (i = 0; i < GridLines; i++)
   	{
   		j = GridColumns - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
      	if (
   				(ABoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
   			)
      	{
      		PhytoBiomass[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1];
      	}
   	}
	}
}


void TSangoPhytoplankton::Production(int ABoxNumber)
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
                     	           /( KValue * BoxDepth)
                                   * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                                   -exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                                   ) / HOURSTOSECONDS;    //mg C m-3 s-1
							  			//* DAYSTOHOURS;

                  TemperatureLimitation(MyBoxNumber);
                  NutrientLimitationByAmmoniaAndNitrate(MyBoxNumber);
                  //Debugger (Productivity);
           		}
               else Productivity = 0.0;
               GPP[MyBoxNumber] = Productivity;
               PhytoProd[MyBoxNumber] = Productivity;
            }
         }
}



void TSangoPhytoplankton::Respiration(int ABoxNumber)
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
      			Resp = MaintenanceRespiration[MyBoxNumber] / DAYSTOHOURS / HOURSTOSECONDS * PhytoBiomass[MyBoxNumber];
      		PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - ( Resp );

            if (MyNutrientPointer != NULL)
            	MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (RedfieldCFactor * NITROGENATOMICWEIGHT/RedfieldNFactor),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
      		//OxygenNetProduction[MyBoxNumber] = OxygenNetProduction[MyBoxNumber] -
            //		                                     Resp / CarbonToOxygenResp;
         }
}

void TSangoPhytoplankton::Exudation(int ABoxNumber)
{
   double  Exudate;
   int MyBoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   MyBoxNumber = ABoxNumber;

         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
				DocStressLoss = DocLoss;
				if (GPP[MyBoxNumber] > 0.0)
					Exudate = GPP[MyBoxNumber] * DocStressLoss /*/ DAYSTOHOURS / HOURSTOSECONDS*/;
				else Exudate = 0.0;
					PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - Exudate;
            if (MyNutrientPointer != NULL)
            	MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (RedfieldCFactor * NITROGENATOMICWEIGHT/RedfieldNFactor),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
			}
}


void TSangoPhytoplankton::Mortality(int ABoxNumber)
{
	double MortalityFlux, MyPOMFlux;
	//TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
	int MyBoxNumber;

   MyBoxNumber = ABoxNumber;

   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
		MortalityFlux = PhytoBiomass[MyBoxNumber] * DeathLoss;

		PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - MortalityFlux / DAYSTOHOURS / HOURSTOSECONDS;

		PhytoMortality[MyBoxNumber] = MortalityFlux; // flux per day for mass balance

		/*if (MyNutrientPointer != NULL)
			MyNutrientPointer->Update(GetEcoDynClassName(), MortalityFlux * CUBIC / (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);   */

		if (MySPMPointer != NULL)
		{

      	MyPOMFlux = MortalityFlux / DAYSTOHOURS / HOURSTOSECONDS * ChlCarbonRatio[MyBoxNumber] * // mg Chl / m3 / d
                     ChlToBiomass[MyBoxNumber] / CUBIC; //convertion from mg Chl / m3 / d to mg DW / L / d

   		MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             MyBoxNumber,
            	   	          "POM",
               	             ObjectCode);

         MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             MyBoxNumber,
            	   	          "TPM",
               	             ObjectCode);                      

   	}
	}
}

                                                 

void TSangoPhytoplankton::NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber)
{
	double LimitingNutrient1, LimitingNutrient2, LimitingNutrient3
           /*NitrateLimitation, AmmoniumLimitation*/;
   int MyBoxNumber;

   MyBoxNumber = ABoxNumber;

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
      //It is assumed that if there is enough inorganic nitrogen to sustain a certain production level then
      //there is no limitation. Whenever nitrogen is not enough for the light and temperature limited productivity
      //then productivity is reduced until the minimal value allowed by available nitrogen (the sum of ammonia and nitrate.
      //This may consume virtually all available inorganic nitrogen in the water
      if (
      		Productivity /
            (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor) * ATimeStep >        //productivity expressed in umol N / L / timestep
            (LimitingNutrient1 + LimitingNutrient2 +LimitingNutrient3)
         )
         Productivity = (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) *
                        (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor) / ATimeStep;  //productivity expressed in is normal units - mg C / m3 / day

      if (Productivity < 0.0000000001) //This is to avoid extremely small values that may lead to errors
      	Productivity = 0.0;

      if (
      		((LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) > 0.0000000001) &&
            (Productivity > 0.0)
          )
      {
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



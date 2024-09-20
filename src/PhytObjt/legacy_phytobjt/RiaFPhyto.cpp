// Phytoplankton object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.01.22
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

// TRiaFormosaPhytoplankton Classes
//
// Constructors invoked outside EcoDyn shell...
//

TRiaFormosaPhytoplankton::TRiaFormosaPhytoplankton(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
        double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
        double aChlCarbonRatio[], double aChlToBiomass[], double aOceanDailyChlorophyll[],
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration,
        double carbonToOxygenProd, double carbonToOxygenResp)
    :TPhytoplanktonBasic(className, timeStep, nLines, nColumns,
            aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, aChlToCarbon, riverPhyto, oceanPhyto,
            aPhytoBiomass, aPhytoLoad, aChlCarbonRatio, aChlToBiomass,
            nParams, pmax, iopt, slope, aEiler, bEiler, cEiler, kValue, ks,
            kAmmonia, kNitrate, pHi, maintenanceRespiration, respirationCoefficient,
            docLoss, docStressLoss, deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate, fallingSpeed,
            ratioLightDarkRespiration)
{
    PreBuildRiaFormosaPhyto();

	for (int i = 0; i < 365; i++)
	{
		OceanDailyChlorophyll[i] = aOceanDailyChlorophyll[i] * ChlToCarbon[0];
	}
    CarbonToOxygenProd = carbonToOxygenProd;
    CarbonToOxygenResp = carbonToOxygenResp;
}

TRiaFormosaPhytoplankton::TRiaFormosaPhytoplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TPhytoplanktonBasic(className, timeStep, morphologyFilename,
                variablesFilename, parametersFilename)
{
    BuildRiaFormosaPhyto();
}
// ...

TRiaFormosaPhytoplankton::TRiaFormosaPhytoplankton(TEcoDynClass* APEcoDynClass,
        char* className) : TPhytoplanktonBasic(APEcoDynClass, className)
{
    BuildRiaFormosaPhyto();
}

void TRiaFormosaPhytoplankton::PreBuildRiaFormosaPhyto()
{
	OxygenNetProduction  = new double[NumberOfBoxes];
    for (int i = 0; i< NumberOfBoxes; i++)
   	    OxygenNetProduction[i] = 0;
}

void TRiaFormosaPhytoplankton::BuildRiaFormosaPhyto()
{
    PreBuildRiaFormosaPhyto();

    int X,Y, XV, YV;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            char MyVariable[65];
            // NumberOfVariables and VariableNameArray already filled in TPhytoplanktonBasic
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                if (strcmp(MyVariable, "Phytoplankton biomass") == 0)
                {
                    // Read the daily chlorophyll oceanic values
                    if (!PReadWrite->FindString("Daily Phytoplankton biomass", XV, YV))
                    {
                        XV = X + 4;
                        YV = i - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, OceanDailyChlorophyll[j]);
                        OceanDailyChlorophyll[j] = OceanDailyChlorophyll[j] * ChlToCarbon[0];
                    }
                }
            }
       }
       else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - RiaFormosa Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaFormosaPhytoplankton::BuildRiaFormosaPhyto - Variables: Undefined object in Ria Formosa phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);

    }
    //Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            // NumberOfParameters already read in TPhytoplankton
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenProd);
                else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenResp);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object in Ria Formosa phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRiaFormosaPhytoplankton::BuildRiaFormosaPhyto - Parameters: Undefined object in Ria Formosa phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TRiaFormosaPhytoplankton::~TRiaFormosaPhytoplankton()
{
    freeMemory();
}

void TRiaFormosaPhytoplankton::freeMemory()
{
    try {
       delete[] OxygenNetProduction;
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
				cerr << "TRiaFormosaPhytoplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TRiaFormosaPhytoplankton::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TPhytoplankton
    if (strcmp(MyParameter, "Pmax") == 0)
    {
        value = Pmax[0];
    }
    else if (strcmp(MyParameter, "Iopt") == 0)
    {
        value = Iopt[0];
    }
    else if (strcmp(MyParameter, "Slope") == 0)
    {
        value = Slope[0];
    }
    else if (strcmp(MyParameter, "DefaultAEiler") == 0)
    {
        value = AEiler[0];
    }
    else if (strcmp(MyParameter, "BEiler") == 0)
    {
        value = BEiler[0];
    }
    else if (strcmp(MyParameter, "CEiler") == 0)
    {
        value = CEiler[0];
    }
    else if (strcmp(MyParameter, "KValue") == 0)
    {
        value = KValue;
    }
    else if (strcmp(MyParameter, "Ks") == 0)
    {
        value = Ks[0];
    }
    else if (strcmp(MyParameter, "KAmmonia") == 0)
    {
        value = KAmmonia[0];
    }
    else if (strcmp(MyParameter, "KNitrate") == 0)
    {
        value = KNitrate[0];
    }
    else if (strcmp(MyParameter, "phi") == 0)
    {
        value = phi;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration[0];
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        value = RespirationCoefficient;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon loss") == 0)
    {
        value = DocLoss;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon nutrient stress loss") == 0)
    {
        value = DocStressLoss;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
    {
        value = RedfieldCFactor;
    }
    else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
    {
        value = RedfieldNFactor;
    }
    else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
    {
        value = RedfieldPFactor;
    }
    else if (strcmp(MyParameter, "TemperatureAugmentationRate") == 0)
    {
        value = TemperatureAugmentationRate;
    }
    else if (strcmp(MyParameter, "Falling speed") == 0)
    {
        value = FallingSpeed;
    }
    else if (strcmp(MyParameter, "Light/Dark resp") == 0)
    {
        value = RatioLightDarkRespiration;
    }
    else if (strcmp(MyParameter, "NumberOfLoads") == 0)
    {
        value = NumberOfLoads;
    }
    else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
    {
        value = NumberOfDaysForLoads;
    }
    else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
    {
        value = NumberOfRiverLoads;
    }

    // parameters from TRiaFormosaPhytoplankton
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
        value = CarbonToOxygenProd;
    else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
        value = CarbonToOxygenResp;
    else
        value = 0.0;
    return value;
}

bool TRiaFormosaPhytoplankton::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TPhytoplankton
    if (strcmp(MyParameter, "Pmax") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Pmax[j] = value;
    }
    else if (strcmp(MyParameter, "Iopt") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Iopt[j] = value;
    }
    else if (strcmp(MyParameter, "Slope") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Slope[j] = value;
    }
    else if (strcmp(MyParameter, "DefaultAEiler") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            AEiler[j] = value;
    }
    else if (strcmp(MyParameter, "BEiler") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            BEiler[j] = value;
    }
    else if (strcmp(MyParameter, "CEiler") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            CEiler[j] = value;
    }
    else if (strcmp(MyParameter, "KValue") == 0)
    {
        KValue = value;
    }
    else if (strcmp(MyParameter, "Ks") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Ks[j] = value;
    }
    else if (strcmp(MyParameter, "KAmmonia") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            KAmmonia[j] = value;
    }
    else if (strcmp(MyParameter, "KNitrate") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            KNitrate[j] = value;
    }
    else if (strcmp(MyParameter, "phi") == 0)
    {
        phi = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            MaintenanceRespiration[j] = value;
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        RespirationCoefficient = value;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon loss") == 0)
    {
        DocLoss = value;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon nutrient stress loss") == 0)
    {
        DocStressLoss = value;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
    {
        RedfieldCFactor = value;
    }
    else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
    {
        RedfieldNFactor = value;
    }
    else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
    {
        RedfieldPFactor = value;
    }
    else if (strcmp(MyParameter, "TemperatureAugmentationRate") == 0)
    {
        TemperatureAugmentationRate = value;
    }
    else if (strcmp(MyParameter, "Falling speed") == 0)
    {
        FallingSpeed = value;
    }
    else if (strcmp(MyParameter, "Light/Dark resp") == 0)
    {
        RatioLightDarkRespiration = value;
    }
    else if (strcmp(MyParameter, "NumberOfLoads") == 0)
    {
        NumberOfLoads = value;
    }
    else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
    {
        NumberOfDaysForLoads = value;
    }
    else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
    {
        NumberOfRiverLoads = value;
    }

    // parameters from TRiaFormosaPhytoplankton
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
        CarbonToOxygenProd = value;
    else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
        CarbonToOxygenResp = value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.12
bool TRiaFormosaPhytoplankton::SaveVariables()
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

    // from TRiaFormosaPhytoplankton
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Daily Phytoplankton biomass");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(OceanDailyChlorophyll[j] / ChlToCarbon[0], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TRiaFormosaPhytoplankton::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Phytoplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Phytoplankton");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Pmax") == 0)
        {
            PReadWrite->WriteCell(Pmax[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Iopt") == 0)
        {
            PReadWrite->WriteCell(Iopt[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Slope") == 0)
        {
            PReadWrite->WriteCell(Slope[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "DefaultAEiler") == 0)
        {
            PReadWrite->WriteCell(AEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "BEiler") == 0)
        {
            PReadWrite->WriteCell(BEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "CEiler") == 0)
        {
            PReadWrite->WriteCell(CEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KValue") == 0)
        {
            PReadWrite->WriteCell(KValue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Ks") == 0)
        {
            PReadWrite->WriteCell(Ks[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KAmmonia") == 0)
        {
            PReadWrite->WriteCell(KAmmonia[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNitrate") == 0)
        {
            PReadWrite->WriteCell(KNitrate[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "phi") == 0)
        {
            PReadWrite->WriteCell(phi, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration Coefficient") == 0)
        {
            PReadWrite->WriteCell(RespirationCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Dissolved organic carbon loss") == 0)
        {
            PReadWrite->WriteCell(DocLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Dissolved organic carbon nutrient stress loss") == 0)
        {
            PReadWrite->WriteCell(DocStressLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RedfieldCFactor") == 0)
        {
            PReadWrite->WriteCell(RedfieldCFactor, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RedfieldNFactor") == 0)
        {
            PReadWrite->WriteCell(RedfieldNFactor, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RedfieldPFactor") == 0)
        {
            PReadWrite->WriteCell(RedfieldPFactor, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TemperatureAugmentationRate") == 0)
        {
            PReadWrite->WriteCell(TemperatureAugmentationRate, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Falling speed") == 0)
        {
            PReadWrite->WriteCell(FallingSpeed, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Light/Dark resp") == 0)
        {
            PReadWrite->WriteCell(RatioLightDarkRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfLoads, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfDaysForLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForLoads, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfRiverLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfRiverLoads, 0);
        }

        // parameters from TRiaFormosaPhytoplankton
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in photosynthesis") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenProd, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in respiration") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenResp, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TRiaFormosaPhytoplankton::Go()
{
	// Active methods
   //Debugger(CarbonToOxygenProd);
   //Debugger(CarbonToOxygenResp);
  	Production();
	Respiration();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		MyNutrientPointer->Update(GetEcoDynClassName(), OxygenNetProduction[i] / CUBIC, //conver ug to mg
													i,
													"Oxygen",
													ObjectCode);
      	OxygenNetProduction[i] = 0;
   	}
   }
	Exudation();
	Mortality();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
	}
}

void TRiaFormosaPhytoplankton::Integrate()
{
	// Integrate phytoplankton biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
	// Transport the phytoplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
		// New calls for boundary exchange
      JulianDay = MyPEcoDynClass->GetJulianDay();
      OceanPhyto = OceanDailyChlorophyll[JulianDay - 1];
		MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);
		/*GenericInterfaceExchange = MyTransportPointer->Exchange;

		// Important note - The pointer variables must be passed like this (below) to a
		// pointer which does not point to the Exchange (in transport (or GIE - here) address
		// otherwise, the pointer starting address is retained, and as exchange changes, so
		// do the results in DINInterface exchange - therefore, the real values must be
		// specifically converted and assigned one by one - wow 96.06.24 JGF

		for (int i = 0; i < NumberOfBoxes; i++)
			for (int j = 0; j < NumberOfBoxes; j++)
			{
				double Value1 = GenericInterfaceExchange[i * NumberOfBoxes + j].AdvectiveFlow;
				PhytoplanktonInterfaceExchange[i * NumberOfBoxes + j].AdvectiveFlow = Value1;
				double Value2 = GenericInterfaceExchange[i * NumberOfBoxes + j].DispersiveFlow;
				PhytoplanktonInterfaceExchange[i * NumberOfBoxes + j].DispersiveFlow = Value2;
				PhytoplanktonInterfaceExchange[i * NumberOfBoxes + j].InFlow = Value1 + Value2; // net
			} */
	}
}


void TRiaFormosaPhytoplankton::Production()
{
	double MyDepth, MyTidalHeight, LightAtTop, LightAtBottom, AverageLight;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				 * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
             * MyTidePointer = MyPEcoDynClass->GetTidePointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
      if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

      MyDepth = MyPEcoDynClass->GetBoxDepth(i);

      if (MyTidePointer != NULL)
      {
      	MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,i,
                                              "Tidal height",
                                              ObjectCode);
         MyDepth = MyDepth + MyTidalHeight / CUBIC;

      }

	   if (MyLightPointer != NULL)
	   {
		   MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
		   									  i,
			   								  "PAR surface irradiance",
				   							  ObjectCode);
         if (LightAtTop == 0)
				Productivity = 0.0;
         else
         {
		   	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
		   										  MyDepth,
			   									  i,
				   								  "Sub-surface PAR irradiance",
					   							  ObjectCode);

		   	Productivity = PhytoBiomass[i]
            	            * Pmax[i]
               	         * 2.718282
								   /( KValue * MyDepth)
								   * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[i] )
                        	-exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[i] )
								   )
								   * DAYSTOHOURS;
         }
         //NutrientLimitation(i);
         GPP[i] = Productivity;
         PhytoProd[i] = Productivity;
         OxygenNetProduction[i] = Productivity / CarbonToOxygenProd;
	   }
   }
}

void TRiaFormosaPhytoplankton::Respiration()
{
	double Resp;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (PhytoProd[i] > 0)
      	Resp = PhytoProd[i] * RespirationCoefficient;
      else
      	Resp = MaintenanceRespiration[i] * PhytoBiomass[i];
      PhytoProd[i] = PhytoProd[i] - ( Resp );
      OxygenNetProduction[i] = OxygenNetProduction[i] -
                               Resp / CarbonToOxygenResp;
	}
}


void TRiaFormosaPhytoplankton::Exudation()
{
   double  Exudate;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		DocStressLoss = DocLoss;
		if (GPP[i] > 0)
		Exudate = GPP[i] * DocStressLoss;
		else Exudate = 0;
		PhytoProd[i] = PhytoProd[i] - Exudate;
	}
}

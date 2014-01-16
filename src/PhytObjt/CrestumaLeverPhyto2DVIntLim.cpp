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

// TCrestumaLeverPhytoplankton2DVIntLim Class
//
// Constructors invoked outside EcoDyn shell...
//
#ifndef _PORT_FORTRAN_
TCrestumaLeverPhytoplankton2DVIntLim::TCrestumaLeverPhytoplankton2DVIntLim(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
        double chlToBiomass, double aPhytoBiomass[], double riverPhyto,
        double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
        double simulationInterval, double nCellQuota, double pCellQuota,
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration, double minNPRatio,
        double maxNPRatio, double pMaxUptake, double nMaxUptake, double kP,
        double kNO3, double kNH4, double minPCellQuota, double maxPCellQuota,
        double minNCellQuota, double maxNCellQuota, double kPInternal,
        double kNInternal, double settlingSpeed, double carbonToOxygenProd,
        double carbonToOxygenResp, double tminRespiration, double tminPhotosynthesis)
    :TCrestumaLeverPhytoplankton2DV(className, timeStep, nLines, nColumns, aDepth, aLength,
            aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, chlCarbonRatio, chlToCarbon,
            chlToBiomass, aPhytoBiomass, riverPhyto,
            phytoplanktonInitialBiomass, phytoplanktonFinalBiomass,
            simulationInterval,
            nParams, pmax, iopt, slope, aEiler, bEiler, cEiler, kValue, ks,
            kAmmonia, kNitrate, pHi, maintenanceRespiration, respirationCoefficient,
            docLoss, docStressLoss, deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate, fallingSpeed,
            ratioLightDarkRespiration)
{
    PreBuildCrestumaLeverPhyto2DVIntLim();

    SetCLPhytoParams(nCellQuota, pCellQuota, minNPRatio,
            maxNPRatio, pMaxUptake, nMaxUptake, kP,
            kNO3, kNH4, minPCellQuota, maxPCellQuota,
            minNCellQuota, maxNCellQuota, kPInternal,
            kNInternal, settlingSpeed, carbonToOxygenProd,
            carbonToOxygenResp, tminRespiration, tminPhotosynthesis);

    PosBuildCrestumaLeverPhyto2DVIntLim();
}

TCrestumaLeverPhytoplankton2DVIntLim::TCrestumaLeverPhytoplankton2DVIntLim(char* className,
    float timeStep, char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TCrestumaLeverPhytoplankton2DV(className, timeStep, morphologyFilename,
            variablesFilename, parametersFilename)
{
    BuildCrestumaLeverPhyto2DVIntLim();
}
// ...


TCrestumaLeverPhytoplankton2DVIntLim::TCrestumaLeverPhytoplankton2DVIntLim(
    TEcoDynClass* APEcoDynClass, char* className)
				 : TCrestumaLeverPhytoplankton2DV(APEcoDynClass, className)
{
    BuildCrestumaLeverPhyto2DVIntLim();
}

void TCrestumaLeverPhytoplankton2DVIntLim::BuildCrestumaLeverPhyto2DVIntLim()
{
    PreBuildCrestumaLeverPhyto2DVIntLim();
    NitrogenLimitation	= 1;
    PhosphorusLimitation = 1;

    int X, Y;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            // NumberOfParameters already read in TPhytoplankton
    //		ParametersNameArray = new VNA[NumberOfParameters];

            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "MinNPRatio") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinNPRatio);
                }
                else if (strcmp(MyParameter, "MaxNPRatio") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxNPRatio);
                }
                else if (strcmp(MyParameter, "PMaxUptake") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PMaxUptake);
                }
                else if (strcmp(MyParameter, "NMaxUptake") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NMaxUptake);
                }
                else if (strcmp(MyParameter, "KP") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KP);
                }
                else if (strcmp(MyParameter, "KNO3") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KNO3);
                }
                else if (strcmp(MyParameter, "KNH4") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KNH4);
                }
                else if (strcmp(MyParameter, "MinPCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinPCellQuota);
                }
                else if (strcmp(MyParameter, "MinNCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinNCellQuota);
                }
                else if (strcmp(MyParameter, "MaxPCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxPCellQuota);
                }
                else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxNCellQuota);
                }
                else if (strcmp(MyParameter, "KPInternal") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KPInternal);
                }
                else if (strcmp(MyParameter, "KNInternal") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KNInternal);
                }
                else if (strcmp(MyParameter, "SettlingSpeed") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SettlingSpeed);
                }
                else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenProd);
                }
                else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenResp);
                }
                else if (strcmp(MyParameter, "TminRespiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TminRespiration);
                }
                else if (strcmp(MyParameter, "TminPhotosynthesis") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TminPhotosynthesis);
                }
                else if (strcmp(MyParameter, "Nitrogen limitation") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NitrogenLimitation);
                }
                else if (strcmp(MyParameter, "Phosphorus limitation") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PhosphorusLimitation);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - CrestumaLever 2DV IntLim Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverPhytoplankton2DVIntLim::BuildCrestumaLeverPhyto2DVIntLim - Parameters: Undefined object - CrestumaLever 2DV IntLim Phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
                char MyVariable[65];
                // NumberOfVariables and VariableNameArray already filled in TCrestumaLeverPhytoplankton2DV
                for (int i = 0; i < NumberOfVariables; i++)
            {
                double MyValue;
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                if (strcmp(MyVariable, "NCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        NCellQuota[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "PCellQuota") == 0)
                {
                   PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PCellQuota[j] = MyValue;
                    }
                }
                // More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Crestuma Lever 2DV IntLim Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrestumaLeverPhytoplankton2DVIntLim::BuildCrestumaLeverPhyto2DVIntLim - Variables: Undefined object - CrestumaLever 2DV IntLim Phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    PosBuildCrestumaLeverPhyto2DVIntLim();
}

#else  // _PORT_FORTRAN_
TCrestumaLeverPhytoplankton2DVIntLim
    ::TCrestumaLeverPhytoplankton2DVIntLim(char* className)
        :TCrestumaLeverPhytoplankton2DV(className)
{
    PreBuildCrestumaLeverPhyto2DVIntLim();
    PosBuildCrestumaLeverPhyto2DVIntLim();
}
#endif  // _PORT_FORTRAN_


void TCrestumaLeverPhytoplankton2DVIntLim::PreBuildCrestumaLeverPhyto2DVIntLim()
{
    NCellQuota = new double[NumberOfBoxes];
    PCellQuota = new double[NumberOfBoxes];
    NPhyto = new double[NumberOfBoxes];
    PPhyto = new double[NumberOfBoxes];
    NCellFlux = new double[NumberOfBoxes];
    PCellFlux = new double[NumberOfBoxes];
    NUptake = new double[NumberOfBoxes];
    PUptake = new double[NumberOfBoxes];
    PhytoNLoad = new double[NumberOfBoxes];
    PhytoPLoad = new double[NumberOfBoxes];
    DailyMeanGPP = new double[NumberOfBoxes];
    ADay = new int[NumberOfBoxes];
    NumberOfParcels = new double[NumberOfBoxes];
    SettlingLoss = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    NCellQuota[i] = 0.0; PCellQuota[i] = 0.0;
        NPhyto[i] = 0.0; PPhyto[i] = 0.0;
        NCellFlux[i] = 0.0; PCellFlux[i] = 0.0;
        NUptake[i] = 0.0; PUptake[i] = 0.0;
        PhytoNLoad[i] = 0.0; PhytoPLoad[i] = 0.0;
        DailyMeanGPP[i] = 0.0;  ADay[i] = MyPEcoDynClass->GetJulianDay();
        NumberOfParcels[i] = 0.0; SettlingLoss[i] = 0.0;
    }
}

void TCrestumaLeverPhytoplankton2DVIntLim::SetCLPhytoParams(
        float nCellQuota, float pCellQuota, float minNPRatio,
        float maxNPRatio, float pMaxUptake, float nMaxUptake, float kP,
        float kNO3, float kNH4, float minPCellQuota, float maxPCellQuota,
        float minNCellQuota, float maxNCellQuota, float kPInternal,
        float kNInternal, float settlingSpeed, float carbonToOxygenProd,
        float carbonToOxygenResp, float tminRespiration, float tminPhotosynthesis)
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NCellQuota[i] = nCellQuota;
		PCellQuota[i] = pCellQuota;
	}
    MinNPRatio = minNPRatio;
    MaxNPRatio = maxNPRatio;
    PMaxUptake = pMaxUptake;
	NMaxUptake = nMaxUptake;
    KP = kP;
    KNO3 = kNO3;
    KNH4 = kNH4;
    MinPCellQuota = minPCellQuota;
    MaxPCellQuota = maxPCellQuota;
    MinNCellQuota = minNCellQuota;
    MaxNCellQuota = maxNCellQuota;
    KPInternal = kPInternal;
    KNInternal = kNInternal;
    SettlingSpeed = settlingSpeed;
    CarbonToOxygenProd = carbonToOxygenProd;
    CarbonToOxygenResp = carbonToOxygenResp;
    TminRespiration = tminRespiration;
    TminPhotosynthesis = tminPhotosynthesis;
}

void TCrestumaLeverPhytoplankton2DVIntLim::PosBuildCrestumaLeverPhyto2DVIntLim()
{
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    NPhyto[i] = PhytoBiomass[i] * NCellQuota[i];
        PPhyto[i] = PhytoBiomass[i] * PCellQuota[i];
        PhytoNLoad[i] = PhytoLoad[i] * RedfieldNFactor/ RedfieldCFactor;
        PhytoPLoad[i] = PhytoLoad[i] * RedfieldPFactor/ RedfieldCFactor;
    }
}


TCrestumaLeverPhytoplankton2DVIntLim::~TCrestumaLeverPhytoplankton2DVIntLim()
{
    freeMemory();
}

void TCrestumaLeverPhytoplankton2DVIntLim::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
       {
          delete [] NCellQuota;
          delete [] PCellQuota;
          delete [] NPhyto;
          delete [] PPhyto;
          delete [] NCellFlux;
          delete [] PCellFlux;
          delete [] PhytoNLoad;
          delete [] PhytoPLoad;
          delete [] NUptake;
          delete [] PUptake;
          delete [] DailyMeanGPP;
          delete [] NumberOfParcels;
          delete [] SettlingLoss;
          delete [] ADay;
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
				cerr << "TCrestumaLeverPhytoplankton2DVIntLim::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

bool TCrestumaLeverPhytoplankton2DVIntLim::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Phytoplankton biomass") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll to Carbon") == 0)
        ChlToCarbon[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll : Carbon") == 0)
        ChlCarbonRatio[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll to biomass") == 0)
        ChlToBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "Phytoplankton initial biomass") == 0)
        PhytoplanktonInitialBiomass = Value;
    else if (strcmp(VariableName, "Phytoplankton final biomass") == 0)
        PhytoplanktonFinalBiomass = Value;
    else if (strcmp(VariableName, "Simulation interval (hours)") == 0)
        SimulationInterval = Value;
    else if (strcmp(VariableName, "NCellQuota") == 0)
        NCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "PCellQuota") == 0)
        PCellQuota[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


double TCrestumaLeverPhytoplankton2DVIntLim::GetParameterValue(char* MyParameter)
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

    // parameters from TCrestumaLeverPhytoplankton2DVIntLim
    else if (strcmp(MyParameter, "MinNPRatio") == 0)
    {
        value = MinNPRatio;
    }
    else if (strcmp(MyParameter, "MaxNPRatio") == 0)
    {
        value = MaxNPRatio;
    }
    else if (strcmp(MyParameter, "PMaxUptake") == 0)
    {
        value = PMaxUptake;
    }
    else if (strcmp(MyParameter, "NMaxUptake") == 0)
    {
        value = NMaxUptake;
    }
    else if (strcmp(MyParameter, "KP") == 0)
    {
        value = KP;
    }
    else if (strcmp(MyParameter, "KNO3") == 0)
    {
        value = KNO3;
    }
    else if (strcmp(MyParameter, "KNH4") == 0)
    {
        value = KNH4;
    }
    else if (strcmp(MyParameter, "MinPCellQuota") == 0)
    {
        value = MinPCellQuota;
    }
    else if (strcmp(MyParameter, "MinNCellQuota") == 0)
    {
        value = MinNCellQuota;
    }
    else if (strcmp(MyParameter, "MaxPCellQuota") == 0)
    {
        value = MaxPCellQuota;
    }
    else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
    {
        value = MaxNCellQuota;
    }
    else if (strcmp(MyParameter, "KPInternal") == 0)
    {
        value = KPInternal;
    }
    else if (strcmp(MyParameter, "KNInternal") == 0)
    {
        value = KNInternal;
    }
    else if (strcmp(MyParameter, "SettlingSpeed") == 0)
    {
        value = SettlingSpeed;
    }
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
    {
        value = CarbonToOxygenProd;
    }
    else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
    {
        value = CarbonToOxygenResp;
    }
    else if (strcmp(MyParameter, "TminRespiration") == 0)
    {
        value = TminRespiration;
    }
    else if (strcmp(MyParameter, "TminPhotosynthesis") == 0)
    {
        value = TminPhotosynthesis;
    }
    else if (strcmp(MyParameter, "Nitrogen limitation") == 0)
    {
        value = NitrogenLimitation;
    }
    else if (strcmp(MyParameter, "Phosphorus limitation") == 0)
    {
        value = PhosphorusLimitation;
    }
    else
        value = 0.0;
    return value;
}

bool TCrestumaLeverPhytoplankton2DVIntLim::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TCrestumaLeverPhytoplankton2DVIntLim
    else if (strcmp(MyParameter, "MinNPRatio") == 0)
    {
        MinNPRatio = value;
    }
    else if (strcmp(MyParameter, "MaxNPRatio") == 0)
    {
        MaxNPRatio = value;
    }
    else if (strcmp(MyParameter, "PMaxUptake") == 0)
    {
        PMaxUptake = value;
    }
    else if (strcmp(MyParameter, "NMaxUptake") == 0)
    {
        NMaxUptake = value;
    }
    else if (strcmp(MyParameter, "KP") == 0)
    {
        KP = value;
    }
    else if (strcmp(MyParameter, "KNO3") == 0)
    {
        KNO3 = value;
    }
    else if (strcmp(MyParameter, "KNH4") == 0)
    {
        KNH4 = value;
    }
    else if (strcmp(MyParameter, "MinPCellQuota") == 0)
    {
        MinPCellQuota = value;
    }
    else if (strcmp(MyParameter, "MinNCellQuota") == 0)
    {
        MinNCellQuota = value;
    }
    else if (strcmp(MyParameter, "MaxPCellQuota") == 0)
    {
        MaxPCellQuota = value;
    }
    else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
    {
        MaxNCellQuota = value;
    }
    else if (strcmp(MyParameter, "KPInternal") == 0)
    {
        KPInternal = value;
    }
    else if (strcmp(MyParameter, "KNInternal") == 0)
    {
        KNInternal = value;
    }
    else if (strcmp(MyParameter, "SettlingSpeed") == 0)
    {
        SettlingSpeed = value;
    }
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
    {
        CarbonToOxygenProd = value;
    }
    else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
    {
        CarbonToOxygenResp = value;
    }
    else if (strcmp(MyParameter, "TminRespiration") == 0)
    {
        TminRespiration = value;
    }
    else if (strcmp(MyParameter, "TminPhotosynthesis") == 0)
    {
        TminPhotosynthesis = value;
    }
    else if (strcmp(MyParameter, "Nitrogen limitation") == 0)
    {
        NitrogenLimitation = value;
    }
    else if (strcmp(MyParameter, "Phosphorus limitation") == 0)
    {
        PhosphorusLimitation = value;
    }
    else
        rc = false;

    return rc;
}

void TCrestumaLeverPhytoplankton2DVIntLim::Inquiry(char* srcName, double &Value,
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
		Value = NPhyto[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton P biomass") == 0)
		Value = PPhyto[MyBoxNumber];
    else if (strcmp(MyParameter, "NCellQuota") == 0)
		Value = NCellQuota[MyBoxNumber];
    else if (strcmp(MyParameter, "PCellQuota") == 0)
		Value = PCellQuota[MyBoxNumber];
    else if (strcmp(MyParameter, "NUptake") == 0)
		Value = NUptake[MyBoxNumber];
    else if (strcmp(MyParameter, "PUptake") == 0)
		Value = PUptake[MyBoxNumber];
    else if (strcmp(MyParameter, "Chlorophyll to Carbon") == 0)
		Value = ChlToCarbon[MyBoxNumber];
	else if (strcmp(MyParameter, "Phytop. total GPP") == 0)
		Value = GPP[MyBoxNumber] * DAYSTOSECONDS;
	else if (strcmp(MyParameter, "Phytop. total NPP") == 0)
        Value = NPP[MyBoxNumber] * DAYSTOSECONDS;
	else if (strcmp(MyParameter, "Phytop. GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
			Value = GPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTOSECONDS;              // mg C/ mg C /h
    }
	else if (strcmp(MyParameter, "Phytop. NPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
		    Value = NPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTOSECONDS;
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
	else if (strcmp(MyParameter, "Phytoplankton mortality") == 0)
		Value = PhytoMortality[MyBoxNumber];
    else if (strcmp(MyParameter, "Chlorophyll to Carbon") == 0)
	    Value = ChlToCarbon[MyBoxNumber];
#ifndef _PORT_FORTRAN_
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
                      "EcoDynamo Alert - Inquiry 1",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
#endif  // _PORT_FORTRAN_
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.07.12
bool TCrestumaLeverPhytoplankton2DVIntLim::SaveParameters()
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
        // parameters from TPhytoplankton
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
        // parameters from TCrestumaLeverPhytoplankton2DVIntLim
        else if (strcmp(ParametersNameArray[i], "MinNPRatio") == 0)
        {
            PReadWrite->WriteCell(MinNPRatio, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxNPRatio") == 0)
        {
            PReadWrite->WriteCell(MaxNPRatio, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PMaxUptake") == 0)
        {
            PReadWrite->WriteCell(PMaxUptake, 8);
        }
        else if (strcmp(ParametersNameArray[i], "NMaxUptake") == 0)
        {
            PReadWrite->WriteCell(NMaxUptake, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KP") == 0)
        {
            PReadWrite->WriteCell(KP, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNO3") == 0)
        {
            PReadWrite->WriteCell(KNO3, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNH4") == 0)
        {
            PReadWrite->WriteCell(KNH4, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinPCellQuota") == 0)
        {
            PReadWrite->WriteCell(MinPCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinNCellQuota") == 0)
        {
            PReadWrite->WriteCell(MinNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxPCellQuota") == 0)
        {
            PReadWrite->WriteCell(MaxPCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxNCellQuota") == 0)
        {
            PReadWrite->WriteCell(MaxNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KPInternal") == 0)
        {
            PReadWrite->WriteCell(KPInternal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNInternal") == 0)
        {
            PReadWrite->WriteCell(KNInternal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "SettlingSpeed") == 0)
        {
            PReadWrite->WriteCell(SettlingSpeed, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in photosynthesis") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenProd, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in respiration") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenResp, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TminRespiration") == 0)
        {
            PReadWrite->WriteCell(TminRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TminPhotosynthesis") == 0)
        {
            PReadWrite->WriteCell(TminPhotosynthesis, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Nitrogen limitation") == 0)
        {
            PReadWrite->WriteCell(NitrogenLimitation, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Phosphorus limitation") == 0)
        {
            PReadWrite->WriteCell(PhosphorusLimitation, 6);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TCrestumaLeverPhytoplankton2DVIntLim::SaveVariables()
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
        // variables from TCrestumaLeverPhytoplankton2DV
        if (strcmp(VariableNameArray[i], "Phytoplankton biomass") == 0)
            PReadWrite->WriteCell(RiverPhyto / ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton initial biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonInitialBiomass, 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton final biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonFinalBiomass, 8);

        // variables from TCrestumaLeverPhytoplankton2DVIntLim
        else if (strcmp(VariableNameArray[i], "NCellQuota") == 0)
            PReadWrite->WriteCell(NCellQuota[0], 8);

        else if (strcmp(VariableNameArray[i], "PCellQuota") == 0)
            PReadWrite->WriteCell(PCellQuota[0], 8);
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

void TCrestumaLeverPhytoplankton2DVIntLim::Go()
{
	double MyBoxDepth, ALayer, MyPreviousSurfVolume, MyCurrentSurfVolume;
	// Active methods
   JulianDay = MyPEcoDynClass->GetJulianDay();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   //Debugger(1);
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
   int ABoxNumber;
   //Debugger(2);
   for (int i = 0; i <= CurrentUpperLayer; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {

      	ABoxNumber = i * GridColumns + j;
   		if (MyTransportPointer != NULL)
			{
   			MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
      												  ABoxNumber,
            		                          "Box depth",
               		                       ObjectCode);
   		}
      	else
      		MyBoxDepth = MyPEcoDynClass->GetBoxDepth(ABoxNumber);
      	if (
      			(MyBoxDepth > 0.0)
         	)
      	{
   			Production(i,j);
            Respiration(ABoxNumber);
            Exudation(ABoxNumber);
            if (NitrogenLimitation == 1)
             	NitrogenUptake(ABoxNumber);
            if (PhosphorusLimitation == 1)
             	PhosphorusUptake(ABoxNumber);
            Mortality(ABoxNumber);
            Settling(i,j);
         }
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
                   MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS/** DAYSTOHOURS*/;

   }
}


void TCrestumaLeverPhytoplankton2DVIntLim::Integrate()
{
	// Integrate phytoplankton biomass within the box
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

   for (int i = 0; i < NumberOfBoxes; i++)
   	GenericProduct1[i] = PhytoBiomass[i] * DailyMeanGPP[i];
   if (MyTransportPointer != NULL)
   	MyTransportPointer->Go(GenericProduct1, GenericProduct2, 0, 0);
///	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;

   for (int i = 0; i < NumberOfBoxes; i++)
   	if (PhytoBiomass[i] < 0.0) PhytoBiomass[i] = 0.0;

	if (MyTransportPointer != NULL)
		MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (PhytoBiomass[i] > 0)
   		DailyMeanGPP[i] = GenericProduct1[i] / PhytoBiomass[i];
      else
         DailyMeanGPP[i] = 0.0;
   }

//   Generic = NPhyto; GenericFlux = NCellFlux;
	Integration(NPhyto, NCellFlux);
//	NPhyto = Generic; NCellFlux = GenericFlux;
   if (MyTransportPointer != NULL)
		MyTransportPointer->Go(NPhyto, GenericProduct2,   //zeros
                             RiverPhyto * RedfieldNFactor/ RedfieldCFactor,
                             OceanPhyto* RedfieldNFactor/ RedfieldCFactor);

//   Generic = PPhyto; GenericFlux = PCellFlux;
	Integration(PPhyto, PCellFlux);
//	PPhyto = Generic; PCellFlux = GenericFlux;
   if (MyTransportPointer != NULL)
   {
		MyTransportPointer->Go(PPhyto, GenericProduct2, //zeros
                             RiverPhyto * RedfieldPFactor/ RedfieldCFactor,
                             OceanPhyto * RedfieldPFactor/ RedfieldCFactor);
   }




   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	NCellFlux[i] = 0.0; PCellFlux[i] = 0.0;
      if (PhytoBiomass[i] > 0.0)
      {
      	NCellQuota[i] = NPhyto[i] / PhytoBiomass[i];
      	PCellQuota[i] = PPhyto[i] / PhytoBiomass[i];
   	}
   }
   Loads();
}


void TCrestumaLeverPhytoplankton2DVIntLim::Production(int ALine, int AColumn)
{
	double BoxDepth, LightAtTop, LightAtBottom, WaterTemperature, ATop, ABottom;
   int MyBoxNumber;

   MyBoxNumber = ALine * GridColumns + AColumn;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				 * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
             * MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
             * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
      		if (MyLightPointer != NULL)
	 			{
               if (MyTransportPointer != NULL)
               {
               	MyTransportPointer->Inquiry(GetEcoDynClassName(), BoxDepth,
            			                             MyBoxNumber,
               			                          "Box depth",
                  			                       ObjectCode);


               	if (ALine < CurrentUpperLayer)
                  	MyTransportPointer->Inquiry(GetEcoDynClassName(), ATop,      // Depth at top surface
			  						 							     (ALine + 1) * GridColumns + AColumn,
			 											            "Total box depth",
														           ObjectCode);
                  if (ALine == CurrentUpperLayer)
                  	ABottom = BoxDepth;
                  else
               		MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,     // Depth at bottom surface
			  											        ALine * GridColumns + AColumn,
			 											        "Total box depth",
														        ObjectCode);
               }
               else
               {
                  BoxDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
                  ATop = 0.0; ABottom = BoxDepth;
               }

					if (MySPMPointer != NULL)
						MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
														  MyBoxNumber,
														  "Extinction coefficient Ktot",
														  ObjectCode);


               if (ALine == CurrentUpperLayer)
            		MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												       	MyBoxNumber,
												       	"PAR surface irradiance",
												       	ObjectCode);
               else
               	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												       ATop,
												       MyBoxNumber,
												       "Sub-surface PAR irradiance",
												       ObjectCode);
            	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												       ABottom,
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
							  			/ HOURSTOSECONDS;    //mg C m-3 s-1;
                  if (MyWaterTemperaturePointer != NULL)
                  {
                     MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
                     Productivity = Productivity * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminPhotosynthesis);
                  }

		 				NutrientLimitation(MyBoxNumber);
                  
                  if (MyPEcoDynClass->GetJulianDay() != ADay[MyBoxNumber])
                  {
                  	//if (MyBoxNumber == 2) Debugger(ADay[MyBoxNumber]);
                     //if (MyBoxNumber == 2)Debugger(MyPEcoDynClass->GetJulianDay());
                     DailyMeanGPP[MyBoxNumber] = 0.0;
                     NumberOfParcels[MyBoxNumber] = 0;
                     ADay[MyBoxNumber] = ADay[MyBoxNumber] + 1;

                  }
                  DailyMeanGPP[MyBoxNumber] =
                  	(
                     	DailyMeanGPP[MyBoxNumber] * NumberOfParcels[MyBoxNumber] +
                     	Productivity / CarbonToOxygenProd / (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * HOURSTOSECONDS
                     )  / (NumberOfParcels[MyBoxNumber] + 1);  //mg O2 / mg Chla / h
                  NumberOfParcels[MyBoxNumber] = NumberOfParcels[MyBoxNumber] + 1;
           		}
               else Productivity = 0.0;
               GPP[MyBoxNumber] = Productivity;
               PhytoProd[MyBoxNumber] = Productivity;
               /*if ((ALine == 0) && (AColumn == 7) && (GPP[MyBoxNumber] < 0.001))
               Debugger(MyPEcoDynClass->GetCurrentTime());  */
               /*if ((MyPEcoDynClass->GetCurrentTime() > 9.251) && (MyBoxNumber == 140))
               {
               	Debugger(GPP[MyBoxNumber]);

               } */
            }
         }
}

void TCrestumaLeverPhytoplankton2DVIntLim::NitrogenUptake(int ABoxNumber)
{
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   int i = ABoxNumber;
   double AmmoniaUpTake, NitrateAndNitriteUptake, NMaxUptakeOfNitrate;
	if (MyNutrientPointer != NULL)
	{
		// the limitation is based on umol l-1 values passed from the
		// communicate method
      double Ammonia, Nitrate, Nitrite;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Ammonia,
												 i,
												 "Ammonia",   //micro M
												 ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Nitrate,
												 i,
												 "Nitrate",
												 ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Nitrite,
												 i,
												 "Nitrite",
												 ObjectCode);
      //...

   	if (
            (NCellQuota[i] >= MaxNCellQuota) ||
            (NCellQuota[i] / PCellQuota[i] > MaxNPRatio)
         )
         NUptake[i] = 0.0;
      else
      {

      	 AmmoniaUpTake =  NMaxUptake *  Ammonia / (Ammonia + KNH4) * NPhyto[i];  //mg N m-3 h-1


         NUptake[i] = AmmoniaUpTake;  //mg N m-3 h-1
         NMaxUptakeOfNitrate = MAX(0.0,NMaxUptake - NMaxUptake * Ammonia / (Ammonia + KNH4));

         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -AmmoniaUpTake /HOURSTOSECONDS/** DAYSTOHOURS*/ * CUBIC /NITROGENATOMICWEIGHT,
															i,
															"Ammonia",
															ObjectCode);

         //...
         NitrateAndNitriteUptake = /*NMaxUptake*/NMaxUptakeOfNitrate * (Nitrate + Nitrite) / ((Nitrate + Nitrite) + KNO3) * NPhyto[i]; //mg N m-3 h-1


         NUptake[i] = NUptake[i] + NitrateAndNitriteUptake; //mg N m-3 d-1

         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrate / (Nitrate + Nitrite) / HOURSTOSECONDS/**
                                     DAYSTOHOURS */ * CUBIC /NITROGENATOMICWEIGHT,
															i,
															"Nitrate",
															ObjectCode);


         MyNutrientPointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrite / (Nitrate + Nitrite) / HOURSTOSECONDS/**
                                     DAYSTOHOURS*/ * CUBIC /NITROGENATOMICWEIGHT,
															i,
															"Nitrite",
															ObjectCode);
          //...
    	}
    }
    else
       NUptake[i] = 0.0;

    NCellFlux[i] = NCellFlux[i] + NUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/;   //mg N m-3 d-1

}




void TCrestumaLeverPhytoplankton2DVIntLim::PhosphorusUptake(int ABoxNumber)
{
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   int i = ABoxNumber;
	if (MyNutrientPointer != NULL)
	{
		// the limitation is based on umol l-1 values passed from the
		// communicate method
      double Phosphate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Phosphate,
												 i,
												 "Phosphate",
												 ObjectCode);
      //...

   	if (
            (PCellQuota[i] > MaxPCellQuota) ||
            (NCellQuota[i] / PCellQuota[i] <= MinNPRatio)
         )
         PUptake[i] = 0.0;
      else
      {
         PUptake[i] = PMaxUptake *  Phosphate / (Phosphate + KP) * PPhyto[i];
         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -PUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/ * CUBIC / PHOSPHORUSATOMICWEIGHT,
                                        i,
                                        "Phosphate",
                                        ObjectCode);
         //...                               
    	}
   }
   else
      PUptake[i] = 0.0;

   PCellFlux[i] = PCellFlux[i] + PUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/;  //mg P m-3 d-1
}

void TCrestumaLeverPhytoplankton2DVIntLim::NutrientLimitation(int ABoxNumber)
{
   //No caso de a classe ser invocada a partir do EcoDynamo...
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   //...
   if (MyNutrientPointer != NULL)
   {
	int i = ABoxNumber;  double NLimitation, PLimitation;

   if (NitrogenLimitation == 1) {
     if (PCellQuota[i] < MinPCellQuota)
          PLimitation = 0.0;
     else
          PLimitation = PCellQuota[i] / (PCellQuota[i] + KPInternal);
   }
   else
     PLimitation = 1.0;
   if (NitrogenLimitation == 1) {
     if (NCellQuota[i] < MinNCellQuota)
          NLimitation = 0.0;
     else
          NLimitation = NCellQuota[i] / (NCellQuota[i] + KNInternal);
   }
   else
      NLimitation = 1.0;

   if ((PLimitation == 0.0) || (NLimitation == 0.0))
   	Productivity = 0.0;
   else
   	Productivity = Productivity * MIN(PLimitation,NLimitation);
   }

}


void TCrestumaLeverPhytoplankton2DVIntLim::Respiration(int ABoxNumber)
{
	double Resp, WaterTemperature;
   int MyBoxNumber;
   double const OxygenMolecularWeight = 32.0;

   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
            Resp = MaintenanceRespiration[MyBoxNumber] / HOURSTOSECONDS/** DAYSTOHOURS*/;    //mmol O2 / mgChl / d


            if (MyWaterTemperaturePointer != NULL)
            	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
            else
               WaterTemperature = 0.0;
            
            if (PhytoProd[MyBoxNumber] <= 0.0)
            	Resp = Resp + RespirationCoefficient *
               	DailyMeanGPP[MyBoxNumber] / OxygenMolecularWeight
                  * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminRespiration) / HOURSTOSECONDS
                  /** DAYSTOHOURS*/;
            else
               Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient *
               	DailyMeanGPP[MyBoxNumber] / OxygenMolecularWeight
                  * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminRespiration) / HOURSTOSECONDS
                  /** DAYSTOHOURS*/;
            //Resp in //mmol O2 / mg Chl / d

            Resp = Resp * CarbonToOxygenProd * (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * OxygenMolecularWeight;
            //Resp in //mg C /m3 / d


            
      		PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - ( Resp );

            if (MyNutrientPointer != NULL)
            {
            	MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3

               NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Resp * NCellQuota[MyBoxNumber];

               MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3

               PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Resp * PCellQuota[MyBoxNumber];
            }
      		//OxygenNetProduction[MyBoxNumber] = OxygenNetProduction[MyBoxNumber] -
            //		                                     Resp / CarbonToOxygenResp;
         }
}
/*
void TCrestumaLeverPhytoplankton2DVIntLim::Respiration(int ABoxNumber)
{
	double Resp;
   int MyBoxNumber;

   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
         if (PhytoBiomass[MyBoxNumber] > 0.0)
   		{
            Resp = MaintenanceRespiration[MyBoxNumber] * PhytoBiomass[MyBoxNumber];

            if (PhytoProd[MyBoxNumber] <= 0.0)
            	Resp = Resp + RespirationCoefficient * PhytoBiomass[MyBoxNumber];
            else
               Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient * PhytoBiomass[MyBoxNumber];


      		PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - ( Resp );

            if (MyNutrientPointer != NULL)
            {
            	MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
               NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Resp * NCellQuota[MyBoxNumber];

               MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
               PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Resp * PCellQuota[MyBoxNumber];
            }
      		//OxygenNetProduction[MyBoxNumber] = OxygenNetProduction[MyBoxNumber] -
            //		                                     Resp / CarbonToOxygenResp;
         }
} */

void TCrestumaLeverPhytoplankton2DVIntLim::Exudation(int ABoxNumber)
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
      //No caso de a classe ser invocada a partir do EcoDynamo...
      if (MyNutrientPointer != NULL)
      {
          MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
          NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Exudate * NCellQuota[MyBoxNumber];

          MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
          PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Exudate * PCellQuota[MyBoxNumber];
      }
      //...
   }
}


void TCrestumaLeverPhytoplankton2DVIntLim::Mortality(int ABoxNumber)
{
	double Release = 0;
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	int MyBoxNumber;

   MyBoxNumber = ABoxNumber;

   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      Release = PhytoBiomass[MyBoxNumber] * DeathLoss / DAYSTOSECONDS;
      PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - Release;
      PhytoMortality[MyBoxNumber] = Release; // flux per second for mass balance
      //No caso de a classe ser invocada a partir do EcoDynamo...
      if (MyNutrientPointer != NULL)
      {
         MyNutrientPointer->Update(GetEcoDynClassName(), Release * CUBIC / /*(RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor)*/
                                    (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);
         NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Release * NCellQuota[MyBoxNumber];

         MyNutrientPointer->Update(GetEcoDynClassName(), Release * CUBIC /
                                   /*(RedfieldCFactor * PHOSPHORUSATOMICWEIGHT/RedfieldPFactor)*/
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
         PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Release * PCellQuota[MyBoxNumber];
      }
      //...
   }
}


double TCrestumaLeverPhytoplankton2DVIntLim::TemperatureArrheniusExponentialLimitation(double ATemperature, double AReferenceTemperature)
{
	return exp(TemperatureAugmentationRate * (ATemperature - AReferenceTemperature));
}


void TCrestumaLeverPhytoplankton2DVIntLim::Settling(int ALine, int AColumn)
{
	double MyDepth, MyUpperDepth, MyLowerDepth, NLoss, PLoss, MySettlingSpeed;
   int MyBoxNumber, MyUpperBoxNumber;
   MySettlingSpeed = SettlingSpeed / DAYSTOSECONDS; //m/s
   MyBoxNumber = ALine * GridColumns + AColumn;
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyTransportPointer != NULL)
   {
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
                                      MyBoxNumber,
                                      "Box depth",
                                      ObjectCode);
      if (ALine > 0)
      	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyLowerDepth,
                                         (ALine - 1) * GridColumns + AColumn,
                                         "Box depth",
                                         ObjectCode);
      else MyLowerDepth = 0.0;

      PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - MySettlingSpeed  * PhytoBiomass[MyBoxNumber] / MyDepth;

      NLoss = MySettlingSpeed * NPhyto[MyBoxNumber] / MyDepth;
      NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - NLoss;

      PLoss = MySettlingSpeed * PPhyto[MyBoxNumber] / MyDepth;
      PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - PLoss;

      if (
            (MyNutrientPointer != NULL) &&
            (
               (ALine == 0) || ((ALine > 0) && (MyLowerDepth == 0.0))
            )
         )
      {
			MyNutrientPointer->Update(GetEcoDynClassName(), NLoss * CUBIC / NITROGENATOMICWEIGHT,
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);

         MyNutrientPointer->Update(GetEcoDynClassName(), PLoss * CUBIC /PHOSPHORUSATOMICWEIGHT,
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
      }
      if (ALine < CurrentUpperLayer)
      {
      	MyUpperBoxNumber =  (ALine + 1) * GridColumns + AColumn;
      	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyUpperDepth,
                                         MyUpperBoxNumber,
                                         "Box depth",
                                         ObjectCode);
      	PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + MySettlingSpeed * PhytoBiomass[MyUpperBoxNumber] / MyUpperDepth;
        NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] + MySettlingSpeed * NPhyto[MyUpperBoxNumber] / MyUpperDepth;
        PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] + MySettlingSpeed * PPhyto[MyUpperBoxNumber] / MyUpperDepth;
      }
   }
}

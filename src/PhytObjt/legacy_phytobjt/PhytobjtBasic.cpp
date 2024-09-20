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
	
	using namespace std;
#endif  // __BORLANDC__
	#include "ecodyn.rh"
	#include "phytobjt.h"
	#include "iodll.h"

// TPhytoplanktonBasic Class
//
// Constructors invoked outside EcoDyn shell...
//

TPhytoplanktonBasic::TPhytoplanktonBasic(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
        double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
        double aChlCarbonRatio[], double aChlToBiomass[],
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
	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ChlToCarbon[i] = aChlToCarbon[i];
		PhytoBiomass[i] = aPhytoBiomass[i] * ChlToCarbon[i];
		PhytoLoad[i] = aPhytoLoad[i] * ChlToCarbon[i];
		ChlCarbonRatio[i] = aChlCarbonRatio[i];
		ChlToBiomass[i] = aChlToBiomass[i];
	}
    RiverPhyto = riverPhyto * ChlToCarbon[0];
    OceanPhyto = oceanPhyto * ChlToCarbon[0];
}

TPhytoplanktonBasic::TPhytoplanktonBasic(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TPhytoplankton(className, timeStep, morphologyFilename, parametersFilename)
{
    strcpy(VarsFileName, variablesFilename);
    BuildPhytoplanktonBasic();
}
// ...

TPhytoplanktonBasic::TPhytoplanktonBasic(TEcoDynClass* APEcoDynClass,
        char* className) : TPhytoplankton(APEcoDynClass, className)
{
    BuildPhytoplanktonBasic();
}

void TPhytoplanktonBasic::BuildPhytoplanktonBasic()
{
	int i;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Phytoplankton variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TPhytoplanktonBasic::BuildPhytoplanktonBasic - Phytoplankton variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        int X,Y, XV, YV, XL, YL;

        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {

            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            char MyVariable[65];
            for (i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);

                if (strcmp(MyVariable, "Chlorophyll to Carbon") == 0)
                {
                    if (PReadWrite->FindString("Chlorophyll to Carbon values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1+ j, ChlToCarbon[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, ChlToCarbon[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Phytoplankton biomass") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverPhyto = GenericRiver * ChlToCarbon[0];  // was [1] jgf 96.06.17
                    OceanPhyto = GenericOcean * ChlToCarbon[0];  //All values in ug l-1

                    if (PReadWrite->FindString("Phytoplankton biomass values", XV, YV))
                    {
                        if (!PReadWrite->FindString("Phytoplankton load values", XL, YL))
                        {
                            XL = XV + 1;
                            YL = YV;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[j]);
                            PhytoBiomass[j] = PhytoBiomass[j] * ChlToCarbon[j];
                            PReadWrite->ReadNumber(XL, YL + 1 + j, PhytoLoad[j]);
                            PhytoLoad[j] = PhytoLoad[j] * ChlToCarbon[j];
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, PhytoBiomass[j]);
                            PhytoBiomass[j] = PhytoBiomass[j] * ChlToCarbon[j];
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, PhytoLoad[j]);
                            PhytoLoad[j] = PhytoLoad[j] * ChlToCarbon[j];
                        }
                    }
                }
                else if (strcmp(MyVariable, "Chlorophyll : Carbon") == 0)
                {
                    if (PReadWrite->FindString("Chlorophyll : Carbon values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1+ j, ChlCarbonRatio[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, ChlCarbonRatio[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Chlorophyll to biomass") == 0)
                {
                    if (PReadWrite->FindString("Chlorophyll to biomass values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1+ j, ChlToBiomass[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, ChlToBiomass[j]);
                        }
                    }
                }
                // More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TPhytoplanktonBasic::BuildPhytoplanktonBasic - Undefined object - phytoplankton." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

// AP, 2007.07.12
bool TPhytoplanktonBasic::SaveVariables()
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

    CloseDataFile((void*)PReadWrite);
    return true;
}



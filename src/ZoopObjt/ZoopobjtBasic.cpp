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
#include "iodll.h"

// TZooplanktonBasic Class
//
// Constructors invoked outside EcoDyn shell...
//
#ifndef _PORT_FORTRAN_  //_PORT_FORTRAN_
TZooplanktonBasic::TZooplanktonBasic(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
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
    :TZooplankton(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildZooplanktonBasic();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooBiomass[i] = aZooBiomass[i];
		ZooLoad[i] = aZooLoad[i];
	}
    RiverZoo = riverZoo;
    OceanZoo = oceanZoo;

    NumberOfParameters = nParams;
    KGraze = kGraze;
    PhytoLowerLimit = phytoLowerLimit;
    Rmax =  rmax;
    DeathLoss = deathLoss;
    DOCLoss = docLoss;
    Metabolism = metabolism;
    ExcretionLoss = excretionLoss;
    MaximumDeathLoss = maximumDeathLoss;
    KZooStarvedByPhyto = kZooStarvedByPhyto;
    Q10Coefficient = q10Coefficient;
    Q10WaterTempReference = q10WaterTempReference;
    Q10KPhytoMass = q10KPhytoMass;
    Q10PhytoLowerLimit = q10PhytoLowerLimit;
    Q10Rmax = q10Rmax;
    PhytoCoefficient = phytoCoefficient;
    DetritalCoefficient = detritalCoefficient;
    BacterialCoefficient = bacterialCoefficient;
    AssimilationEfficiency = assimilationEfficiency;
    AmmoniaFractionOfExcretion = ammoniaFractionOfExcretion;
    DetritalFractionOfMortality = detritalFractionOfMortality;
    RedfieldCFactor = redfieldCFactor;
    RedfieldNFactor = redfieldNFactor;
    RedfieldPFactor = redfieldPFactor;
}

TZooplanktonBasic::TZooplanktonBasic(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TZooplankton(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildZooplanktonBasic();
}
// ...

TZooplanktonBasic::TZooplanktonBasic(TEcoDynClass* APEcoDynClass, char* className)
									: TZooplankton(APEcoDynClass, className)

{
    BuildZooplanktonBasic();
}

void TZooplanktonBasic::PreBuildZooplanktonBasic()
{
	// Initialise pointers
	VariableCount = 0;
	ParameterCount = 0;

	TimeStep = MyPEcoDynClass->GetTimeStep();
	// Zero all arrays
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooBiomass[i] = 0;
		ZooLoad[i] = 0;
		ZooFlux[i] = 0;
		ZooProduction[i] = 0;
		ZooNetProduction[i] = 0;
		ZooGrossProduction[i] = 0;
	}
}

void TZooplanktonBasic::BuildZooplanktonBasic()
{
    PreBuildZooplanktonBasic();

    int X,Y, XV, YV, XL, YL;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Zooplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Zooplankton variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TZooplanktonBasic::BuildZooplanktonBasic - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Zooplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);

            if (!PReadWrite->FindString("RiverZoo value", XV, YV))
            {
                // Read the end-member values
                PReadWrite->ReadNumber(X+3, Y, GenericRiver);
                PReadWrite->ReadNumber(X+4, Y, GenericOcean);

                RiverZoo = GenericRiver;
                OceanZoo = GenericOcean;  //All values in ug l-1

                for (int j = 0; j < NumberOfBoxes; j++)
                {
                    PReadWrite->ReadNumber(X+5+j, Y, ZooBiomass[j]);
                    PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, Y, ZooLoad[j]);
                }
            }
            else
            {
                PReadWrite->ReadNumber(XV+1, YV, RiverZoo);
                if (PReadWrite->FindString("OceanZoo value", XV, YV))
                    PReadWrite->ReadNumber(XV+1, YV, OceanZoo);

                if (PReadWrite->FindString("ZooBiomass values", XV, YV)
                        && PReadWrite->FindString("ZooLoad values", XL, YL))
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ZooBiomass[j]);
                        PReadWrite->ReadNumber(XL, YL + 1 + j, ZooLoad[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object", "EcoDynamo Zooplankton", MB_OK);
#else  // __BORLANDC__
						cerr << "TZooplanktonBasic::BuildZooplanktonBasic - Variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Zooplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Zooplankton parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TZooplanktonBasic::BuildZooplanktonBasic - Parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        double MySPMRessuspension = 0, MyValue = 0;
        if (PReadWrite->FindString("Zooplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "KGraze") == 0)
                {
                    double MyKGraze = 0;
                    PReadWrite->ReadNumber(X+3, i, MyKGraze);
                    KGraze   = MyKGraze;
                }
                else if (strcmp(MyParameter, "PhytoLowerLimit") == 0)
                {
                    double MyPhytoLowerLimit = 0;
                    PReadWrite->ReadNumber(X+3, i, MyPhytoLowerLimit);
                    PhytoLowerLimit   = MyPhytoLowerLimit;
                 //	Debugger(PhytoLowerLimit);
                }
                else if (strcmp(MyParameter, "Rmax") == 0)
                {
                    double MyRmax = 0;
                    PReadWrite->ReadNumber(X+3, i, MyRmax);
                    Rmax   = MyRmax;
                 //	Debugger(Rmax);
                }
                else if (strcmp(MyParameter, "DeathLoss") == 0)
                {
                    double MyDeathLoss = 0;
                    PReadWrite->ReadNumber(X+3, i, MyDeathLoss);
                    DeathLoss   = MyDeathLoss;
                 //	Debugger(DeathLoss);
                }
                else if (strcmp(MyParameter, "DOC Loss") == 0)
                {
                    double MyDOCLoss = 0;
                    PReadWrite->ReadNumber(X+3, i, MyDOCLoss);
                    DOCLoss = MyDOCLoss;
                   //Debugger(DOCLoss);
                }
                else if (strcmp(MyParameter, "Metabolism") == 0)
                {
                    double MyMetabolism = 0;
                    PReadWrite->ReadNumber(X+3, i, MyMetabolism);
                    Metabolism   = MyMetabolism;
    //				Debugger(Metabolism);
                }
                else if (strcmp(MyParameter, "Excretion loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    ExcretionLoss   = MyValue;
    //				Debugger(Metabolism);
                }
                else if (strcmp(MyParameter, "MaximumDeathLoss") == 0)
                {
                    double MyMaximumDeathLoss = 0;
                    PReadWrite->ReadNumber(X+3, i, MyMaximumDeathLoss);
                    MaximumDeathLoss = MyMaximumDeathLoss;
                }
                else if (strcmp(MyParameter, "KZooStarvedByPhyto") == 0)
                {
                    double MyKZooStarvedByPhyto = 0;
                    PReadWrite->ReadNumber(X+3, i, MyKZooStarvedByPhyto);
                    KZooStarvedByPhyto   = MyKZooStarvedByPhyto;
                }
                else if (strcmp(MyParameter, "Q10Coefficient") == 0)
                {
                    double MyQ10Coefficient = 0;
                    PReadWrite->ReadNumber(X+3, i, MyQ10Coefficient);
                    Q10Coefficient   = MyQ10Coefficient;
                }
                else if (strcmp(MyParameter, "Q10WaterTempReference") == 0)
                {
                    double MyQ10WaterTempReference = 0;
                    PReadWrite->ReadNumber(X+3, i, MyQ10WaterTempReference);
                    Q10WaterTempReference   = MyQ10WaterTempReference;
                }
                else if (strcmp(MyParameter, "Q10KPhytoMass") == 0)
                {
                    double MyQ10KPhytoMass = 0;
                    PReadWrite->ReadNumber(X+3, i, MyQ10KPhytoMass);
                    Q10KPhytoMass   = MyQ10KPhytoMass;
                }
                else if (strcmp(MyParameter, "Q10PhytoLowerLimit") == 0)
                {
                    double MyQ10PhytoLowerLimit = 0;
                    PReadWrite->ReadNumber(X+3, i, MyQ10PhytoLowerLimit);
                    Q10PhytoLowerLimit   = MyQ10PhytoLowerLimit;
                }
                else if (strcmp(MyParameter, "Q10Rmax") == 0)
                {
                    double MyQ10Rmax = 0;
                    PReadWrite->ReadNumber(X+3, i, MyQ10Rmax);
                    Q10Rmax   = MyQ10Rmax;
                }
                else if (strcmp(MyParameter, "Phyto coefficient for grazing") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    PhytoCoefficient  = MyValue;
                    //Debugger(PhytoCoefficient);
                }
                else if (strcmp(MyParameter, "Detrital coefficient for grazing") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    DetritalCoefficient = MyValue;
                }
                else if (strcmp(MyParameter, "Bacterial coefficient for grazing") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    BacterialCoefficient = MyValue;
                    //Debugger(BacterialCoefficient);
                }
                else if (strcmp(MyParameter, "Assimilation Efficiency") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    AssimilationEfficiency = MyValue;
                    //Debugger(AssimilationEfficiency);
                }
                else if (strcmp(MyParameter, "Ammonia fraction of excretion") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    AmmoniaFractionOfExcretion = MyValue;
                    //Debugger(AmmoniaFractionOfExcretion);
                }
                else if (strcmp(MyParameter, "Detrital fraction of mortality") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    DetritalFractionOfMortality = MyValue;
                    //Debugger(DetritalFractionOfMortality);
                }
                else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RedfieldCFactor = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RedfieldNFactor = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RedfieldPFactor= MyValue;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Zooplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TZooplanktonBasic::BuildZooplanktonBasic - Parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

#endif //_PORT_FORTRAN_
double TZooplanktonBasic::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "KGraze") == 0)
    {
        value = KGraze;
    }
    else if (strcmp(MyParameter, "PhytoLowerLimit") == 0)
    {
        value = PhytoLowerLimit;
    }
    else if (strcmp(MyParameter, "Rmax") == 0)
    {
        value = Rmax;
    }
    else if (strcmp(MyParameter, "DeathLoss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "DOC Loss") == 0)
    {
        value = DOCLoss;
    }
    else if (strcmp(MyParameter, "Metabolism") == 0)
    {
        value = Metabolism;
    }
    else if (strcmp(MyParameter, "Excretion loss") == 0)
    {
        value = ExcretionLoss;
    }
    else if (strcmp(MyParameter, "MaximumDeathLoss") == 0)
    {
        value = MaximumDeathLoss;
    }
    else if (strcmp(MyParameter, "KZooStarvedByPhyto") == 0)
    {
        value = KZooStarvedByPhyto;
    }
    else if (strcmp(MyParameter, "Q10Coefficient") == 0)
    {
        value = Q10Coefficient;
    }
    else if (strcmp(MyParameter, "Q10WaterTempReference") == 0)
    {
        value = Q10WaterTempReference;
    }
    else if (strcmp(MyParameter, "Q10KPhytoMass") == 0)
    {
        value = Q10KPhytoMass;
    }
    else if (strcmp(MyParameter, "Q10PhytoLowerLimit") == 0)
    {
        value = Q10PhytoLowerLimit;
    }
    else if (strcmp(MyParameter, "Q10Rmax") == 0)
    {
        value = Q10Rmax;
    }
    else if (strcmp(MyParameter, "Phyto coefficient for grazing") == 0)
    {
        value = PhytoCoefficient;
    }
    else if (strcmp(MyParameter, "Detrital coefficient for grazing") == 0)
    {
        value = DetritalCoefficient;
    }
    else if (strcmp(MyParameter, "Bacterial coefficient for grazing") == 0)
    {
        value = BacterialCoefficient;
    }
    else if (strcmp(MyParameter, "Assimilation Efficiency") == 0)
    {
        value = AssimilationEfficiency;
    }
    else if (strcmp(MyParameter, "Ammonia fraction of excretion") == 0)
    {
        value = AmmoniaFractionOfExcretion;
    }
    else if (strcmp(MyParameter, "Detrital fraction of mortality") == 0)
    {
        value = DetritalFractionOfMortality;
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
    else
        value = 0.0;
    return value;
}

bool TZooplanktonBasic::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "KGraze") == 0)
    {
        KGraze = value;
    }
    else if (strcmp(MyParameter, "PhytoLowerLimit") == 0)
    {
        PhytoLowerLimit = value;
    }
    else if (strcmp(MyParameter, "Rmax") == 0)
    {
        Rmax = value;
    }
    else if (strcmp(MyParameter, "DeathLoss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "DOC Loss") == 0)
    {
        DOCLoss = value;
    }
    else if (strcmp(MyParameter, "Metabolism") == 0)
    {
        Metabolism = value;
    }
    else if (strcmp(MyParameter, "Excretion loss") == 0)
    {
        ExcretionLoss = value;
    }
    else if (strcmp(MyParameter, "MaximumDeathLoss") == 0)
    {
        MaximumDeathLoss = value;
    }
    else if (strcmp(MyParameter, "KZooStarvedByPhyto") == 0)
    {
        KZooStarvedByPhyto = value;
    }
    else if (strcmp(MyParameter, "Q10Coefficient") == 0)
    {
        Q10Coefficient = value;
    }
    else if (strcmp(MyParameter, "Q10WaterTempReference") == 0)
    {
        Q10WaterTempReference = value;
    }
    else if (strcmp(MyParameter, "Q10KPhytoMass") == 0)
    {
        Q10KPhytoMass = value;
    }
    else if (strcmp(MyParameter, "Q10PhytoLowerLimit") == 0)
    {
        Q10PhytoLowerLimit = value;
    }
    else if (strcmp(MyParameter, "Q10Rmax") == 0)
    {
        Q10Rmax = value;
    }
    else if (strcmp(MyParameter, "Phyto coefficient for grazing") == 0)
    {
        PhytoCoefficient = value;
    }
    else if (strcmp(MyParameter, "Detrital coefficient for grazing") == 0)
    {
        DetritalCoefficient = value;
    }
    else if (strcmp(MyParameter, "Bacterial coefficient for grazing") == 0)
    {
        BacterialCoefficient = value;
    }
    else if (strcmp(MyParameter, "Assimilation Efficiency") == 0)
    {
        AssimilationEfficiency = value;
    }
    else if (strcmp(MyParameter, "Ammonia fraction of excretion") == 0)
    {
        AmmoniaFractionOfExcretion = value;
    }
    else if (strcmp(MyParameter, "Detrital fraction of mortality") == 0)
    {
        DetritalFractionOfMortality = value;
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
    else
        rc = false;

    return rc;
}

#ifndef _PORT_FORTRAN_  //_PORT_FORTRAN_
bool TZooplanktonBasic::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Zooplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Zooplankton");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save variables' values
     */
    PReadWrite->WriteCell("RiverZoo value");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(RiverZoo, 8);
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("OceanZoo value");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(OceanZoo, 8);
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("ZooBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZooLoad values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(ZooBiomass[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ZooLoad[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TZooplanktonBasic::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Zooplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Zooplankton");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();

    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Rmax") == 0)
            PReadWrite->WriteCell(Rmax, 6);

        else if (strcmp(ParametersNameArray[i], "KGraze") == 0)
            PReadWrite->WriteCell(KGraze, 6);

        else if (strcmp(ParametersNameArray[i], "DeathLoss") == 0)
            PReadWrite->WriteCell(DeathLoss, 6);

        else if (strcmp(ParametersNameArray[i], "Metabolism") == 0)
            PReadWrite->WriteCell(Metabolism, 6);

        else if (strcmp(ParametersNameArray[i], "PhytoLowerLimit") == 0)
            PReadWrite->WriteCell(PhytoLowerLimit, 6);

        else if (strcmp(ParametersNameArray[i], "RedfieldNFactor") == 0)
            PReadWrite->WriteCell(RedfieldNFactor, 6);

        else if (strcmp(ParametersNameArray[i], "RedfieldPFactor") == 0)
            PReadWrite->WriteCell(RedfieldPFactor, 6);

        else if (strcmp(ParametersNameArray[i], "DOC Loss") == 0)
            PReadWrite->WriteCell(DOCLoss, 6);

        else if (strcmp(ParametersNameArray[i], "Excretion loss") == 0)
            PReadWrite->WriteCell(ExcretionLoss, 6);

        else if (strcmp(ParametersNameArray[i], "KZooStarvedByPhyto") == 0)
            PReadWrite->WriteCell(KZooStarvedByPhyto, 6);

        else if (strcmp(ParametersNameArray[i], "MaximumDeathLoss") == 0)
            PReadWrite->WriteCell(MaximumDeathLoss, 6);

        else if (strcmp(ParametersNameArray[i], "Q10Coefficient") == 0)
            PReadWrite->WriteCell(Q10Coefficient, 6);

        else if (strcmp(ParametersNameArray[i], "Q10WaterTempReference") == 0)
            PReadWrite->WriteCell(Q10WaterTempReference, 6);

        else if (strcmp(ParametersNameArray[i], "Q10KPhytoMass") == 0)
            PReadWrite->WriteCell(Q10KPhytoMass, 6);

        else if (strcmp(ParametersNameArray[i], "Q10PhytoLowerLimit") == 0)
            PReadWrite->WriteCell(Q10PhytoLowerLimit, 6);

        else if (strcmp(ParametersNameArray[i], "Q10Rmax") == 0)
            PReadWrite->WriteCell(Q10Rmax, 6);

        else if (strcmp(ParametersNameArray[i], "Phyto coefficient for grazing") == 0)
            PReadWrite->WriteCell(PhytoCoefficient, 6);

        else if (strcmp(ParametersNameArray[i], "Detrital coefficient for grazing") == 0)
            PReadWrite->WriteCell(DetritalCoefficient, 6);

        else if (strcmp(ParametersNameArray[i], "Bacterial coefficient for grazing") == 0)
            PReadWrite->WriteCell(BacterialCoefficient, 6);

        else if (strcmp(ParametersNameArray[i], "Assimilation Efficiency") == 0)
            PReadWrite->WriteCell(AssimilationEfficiency, 6);

        else if (strcmp(ParametersNameArray[i], "Ammonia fraction of excretion") == 0)
            PReadWrite->WriteCell(AmmoniaFractionOfExcretion, 6);

        else if (strcmp(ParametersNameArray[i], "Detrital fraction of mortality") == 0)
            PReadWrite->WriteCell(DetritalFractionOfMortality, 6);

        else if (strcmp(ParametersNameArray[i], "RedfieldCFactor") == 0)
            PReadWrite->WriteCell(RedfieldCFactor, 6);

        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

#endif  //_PORT_FORTRAN_


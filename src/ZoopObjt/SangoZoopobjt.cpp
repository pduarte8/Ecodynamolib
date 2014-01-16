// Zooplankton object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 01.04.30
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

// TSangoZooplankton Class
//
// Constructors invoked outside EcoDyn shell...
//

TSangoZooplankton::TSangoZooplankton(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aZooBiomass[], double aBoundaryZooConcentration[],
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
    PreBuildSangoZooplankton();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
		ZooBiomass[i] = aZooBiomass[i];
    for (int i = 0; i < 365; i++)
        BoundaryZooConcentration[i] = aBoundaryZooConcentration[i];

    NumberOfParameters = nParams;
    KGraze = kGraze;
    PhytoLowerLimit = phytoLowerLimit;
    Rmax =  rmax / (DAYSTOHOURS * HOURSTOSECONDS);
    DeathLoss = deathLoss / (DAYSTOHOURS * HOURSTOSECONDS);
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

TSangoZooplankton::TSangoZooplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TZooplankton(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildSangoZooplankton();
}
// ...

TSangoZooplankton::TSangoZooplankton(TEcoDynClass* APEcoDynClass, char* className)
									: TZooplankton(APEcoDynClass, className)

{
    BuildSangoZooplankton();
}

void TSangoZooplankton::PreBuildSangoZooplankton()
{
	WesternBoundaryNumber = 4;
    SouthernBoundaryNumber = 3;
    EasternBoundaryNumber = 2;
    NorthernBoundaryNumber = 1;

    BoundaryZooConcentration = new double [365];
    for (int i = 0; i < 365; i++)
   	    BoundaryZooConcentration[i] = 0.0;
	VariableCount = 0;
	ParameterCount = 0;
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
}

void TSangoZooplankton::BuildSangoZooplankton()
{
    PreBuildSangoZooplankton();

    int X,Y, XV, YV;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Zooplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Zooplankton variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSangoZooplankton::BuildSangoZooplankton - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Zooplankton", X, Y))
        {
            char MyVariable[65];
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
                PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);

            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                if (strcmp(MyVariable, "Zooplankton biomass") == 0)
                {
                    double MyValue;
                    if (!PReadWrite->FindString("Zooplankton biomass values", XV,YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ZooBiomass[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary Zoo concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary Zoo concentration values", XV,YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryZooConcentration[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Sango Zooplankton", "EcoDynamo Zooplankton", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoZooplankton::BuildSangoZooplankton - variables: undefined object Sango Zooplankton" << endl;
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
		cerr << "TSangoZooplankton::BuildSangoZooplankton - parameters file missing." << endl;
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

                if (strcmp(MyParameter, "Rmax") == 0)
                {
                    double MyRmax = 0;
                    PReadWrite->ReadNumber(X+3, i, MyRmax);
                    Rmax   = MyRmax / (DAYSTOHOURS * HOURSTOSECONDS);
                }
                else if (strcmp(MyParameter, "KGraze") == 0)
                {
                    double MyKGraze = 0;
                    PReadWrite->ReadNumber(X+3, i, MyKGraze);
                    KGraze   = MyKGraze;
                    //Debugger(KGraze);
                }
                else if (strcmp(MyParameter, "DeathLoss") == 0)
                {
                    double MyDeathLoss = 0;
                    PReadWrite->ReadNumber(X+3, i, MyDeathLoss);
                    DeathLoss   = MyDeathLoss / (DAYSTOHOURS * HOURSTOSECONDS);
                    //Debugger(DeathLoss);
                }
                else if (strcmp(MyParameter, "Metabolism") == 0)
                {
                    double MyMetabolism = 0;
                    PReadWrite->ReadNumber(X+3, i, MyMetabolism);
                    Metabolism   = MyMetabolism;
                    //Debugger(Metabolism);
                }
                else
                if (strcmp(MyParameter, "PhytoLowerLimit") == 0)
                {
                    double MyPhytoLowerLimit = 0;
                    PReadWrite->ReadNumber(X+3, i, MyPhytoLowerLimit);
                    PhytoLowerLimit   = MyPhytoLowerLimit;
                    //Debugger(PhytoLowerLimit);
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
                else if (strcmp(MyParameter, "DOC Loss") == 0)
                {
                    double MyDOCLoss = 0;
                    PReadWrite->ReadNumber(X+3, i, MyDOCLoss);
                    DOCLoss = MyDOCLoss;
                   //Debugger(DOCLoss);
                }
                else if (strcmp(MyParameter, "Excretion loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    ExcretionLoss   = MyValue;
                    //Debugger(ExcretionLoss);
                }
                else if (strcmp(MyParameter, "KZooStarvedByPhyto") == 0)
                {
                    double MyKZooStarvedByPhyto = 0;
                    PReadWrite->ReadNumber(X+3, i, MyKZooStarvedByPhyto);
                    KZooStarvedByPhyto   = MyKZooStarvedByPhyto;
                }
                else if (strcmp(MyParameter, "MaximumDeathLoss") == 0)
                {
                    double MyMaximumDeathLoss = 0;
                    PReadWrite->ReadNumber(X+3, i, MyMaximumDeathLoss);
                    MaximumDeathLoss = MyMaximumDeathLoss;
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
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "parameters: Undefined object - Sango Zooplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoZooplankton::BuildSangoZooplankton - parameters: undefined object Sango Zooplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}


TSangoZooplankton::~TSangoZooplankton()
{
    freeMemory();
}

void TSangoZooplankton::freeMemory()
{
    try {
        delete [] BoundaryZooConcentration;
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }
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
				cerr << "TSangoZooplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TSangoZooplankton::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Rmax") == 0)
    {
        value = Rmax * (DAYSTOHOURS * HOURSTOSECONDS);
    }
    else if (strcmp(MyParameter, "KGraze") == 0)
    {
        value = KGraze;
    }
    else if (strcmp(MyParameter, "DeathLoss") == 0)
    {
        value = DeathLoss * (DAYSTOHOURS * HOURSTOSECONDS);
    }
    else if (strcmp(MyParameter, "Metabolism") == 0)
    {
        value = Metabolism;
    }
    else if (strcmp(MyParameter, "PhytoLowerLimit") == 0)
    {
        value = PhytoLowerLimit;
    }
    else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
    {
        value = RedfieldNFactor;
    }
    else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
    {
        value = RedfieldPFactor;
    }
    else if (strcmp(MyParameter, "DOC Loss") == 0)
    {
        value = DOCLoss;
    }
    else if (strcmp(MyParameter, "Excretion loss") == 0)
    {
        value = ExcretionLoss;
    }
    else if (strcmp(MyParameter, "KZooStarvedByPhyto") == 0)
    {
        value = KZooStarvedByPhyto;
    }
    else if (strcmp(MyParameter, "MaximumDeathLoss") == 0)
    {
        value = MaximumDeathLoss;
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
    else
        value = 0.0;
    return value;
}

bool TSangoZooplankton::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Rmax") == 0)
    {
        Rmax = value / (DAYSTOHOURS * HOURSTOSECONDS);
    }
    else if (strcmp(MyParameter, "KGraze") == 0)
    {
        KGraze = value;
    }
    else if (strcmp(MyParameter, "DeathLoss") == 0)
    {
        DeathLoss = value / (DAYSTOHOURS * HOURSTOSECONDS);
    }
    else if (strcmp(MyParameter, "Metabolism") == 0)
    {
        Metabolism = value;
    }
    else if (strcmp(MyParameter, "PhytoLowerLimit") == 0)
    {
        PhytoLowerLimit = value;
    }
    else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
    {
        RedfieldNFactor = value;
    }
    else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
    {
        RedfieldPFactor = value;
    }
    else if (strcmp(MyParameter, "DOC Loss") == 0)
    {
        DOCLoss = value;
    }
    else if (strcmp(MyParameter, "Excretion loss") == 0)
    {
        ExcretionLoss = value;
    }
    else if (strcmp(MyParameter, "KZooStarvedByPhyto") == 0)
    {
        KZooStarvedByPhyto = value;
    }
    else if (strcmp(MyParameter, "MaximumDeathLoss") == 0)
    {
        MaximumDeathLoss = value;
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
    else
        rc = false;

    return rc;
}


void TSangoZooplankton::Inquiry(char* srcName, double &Value,
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
		Value = ZooProduction[MyBoxNumber] * (DAYSTOHOURS * HOURSTOSECONDS);
    else if (strcmp(MyParameter, "Zooplankton net production") == 0)
		Value = ZooNetProduction[MyBoxNumber] * (DAYSTOHOURS * HOURSTOSECONDS);
	else if (strcmp(MyParameter, "Zooplankton gross production") == 0)
		Value = ZooGrossProduction[MyBoxNumber] * (DAYSTOHOURS * HOURSTOSECONDS);
	else if (strcmp(MyParameter, "Zooplankton net/gross ratio") == 0)
	{
	    if ( ZooGrossProduction[MyBoxNumber] != 0 )
		    Value = (  ZooNetProduction[MyBoxNumber]              //NG 5/6/96
								/ ZooGrossProduction[MyBoxNumber])*100;
        else
			Value = INFINITY;
    }
    else if (strcmp(MyParameter, "Boundary Zoo concentration") == 0)
		Value = BoundaryZooConcentration[MyPEcoDynClass->GetJulianDay() - 1];
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
						cerr <<"TSangoZooplankton::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TSangoZooplankton::SetVariableValue(char* srcName, double Value,
									int BoxNumber,
									char* VariableName)
{
  bool rc = true;

   // Zooplankton expects to receive in mgFW m-3 // JGF 97.08.11
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Zooplankton biomass") == 0)
		ZooBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Zooplankton production") == 0)
		ZooProduction[BoxNumber] = Value / (DAYSTOHOURS * HOURSTOSECONDS);
	else if (strcmp(VariableName, "Zooplankton net production") == 0)
		ZooNetProduction[BoxNumber] = Value / (DAYSTOHOURS * HOURSTOSECONDS);
	else if (strcmp(VariableName, "Zooplankton gross production") == 0)
        ZooGrossProduction[BoxNumber] = Value / (DAYSTOHOURS * HOURSTOSECONDS);
    else if (strcmp(VariableName, "Boundary Zoo concentration") == 0)
    {
        for (int i = 0; i < 365; i++)
            BoundaryZooConcentration[i] = Value;
    }
    else
        rc = false;

    return rc;
}

bool TSangoZooplankton::SaveVariables()
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

    /*
     * save variables' names
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
    PReadWrite->WriteCell("Boundary Zoo concentration values");
    PReadWrite->WriteSeparator(true);
    for (int i = 0; i < 365; i++)
    {
        PReadWrite->WriteCell(BoundaryZooConcentration[i], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("Zooplankton biomass values");
    PReadWrite->WriteSeparator(true);
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        PReadWrite->WriteCell(ZooBiomass[i], 6);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TSangoZooplankton::SaveParameters()
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
            PReadWrite->WriteCell(Rmax * (DAYSTOHOURS * HOURSTOSECONDS), 6);

        else if (strcmp(ParametersNameArray[i], "KGraze") == 0)
            PReadWrite->WriteCell(KGraze, 6);

        else if (strcmp(ParametersNameArray[i], "DeathLoss") == 0)
            PReadWrite->WriteCell(DeathLoss * (DAYSTOHOURS * HOURSTOSECONDS), 6);

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

void TSangoZooplankton::Integrate()
{
	double MyBoxDepth;

//	Generic = ZooBiomass; GenericFlux = ZooFlux;
	Integration(ZooBiomass, ZooFlux);
//	ZooBiomass = Generic; ZooFlux = GenericFlux;

	// Transport the zooplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
		// New calls for boundary exchange
		MyTransportPointer->Go(ZooBiomass, ZooBiomass, 0.0, 0.0);
      //Southern boundary
   	int i,j;
   	for (j = 0; j < GridColumns; j++)
   	{
   		i = 0;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
  	 		if (
   				(MyBoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
   			)
      	{
      		ZooBiomass[i * GridColumns + j] = BoundaryZooConcentration[JulianDay-1];
      	}
		}
      //Northern boundary
   	for (j = 0; j < GridColumns; j++)
   	{
   		i = GridLines - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
      	if (
   				(MyBoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
   			)
      	{
      		ZooBiomass[i * GridColumns + j] = BoundaryZooConcentration[JulianDay-1];
      	}
   	}
      //Eastern boundary
   	for (i = 0; i < GridLines; i++)
   	{
   		j = GridColumns - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
      	if (
   				(MyBoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
   			)
      	{
      		ZooBiomass[i * GridColumns + j] = BoundaryZooConcentration[JulianDay-1];
      	}
   	}
	}
}


void TSangoZooplankton::Go()
{
	double ABoxDepth;
	JulianDay = MyPEcoDynClass->GetJulianDay();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   for (int i = 0; i < NumberOfBoxes; i++)
	{
   	if (MyTransportPointer != NULL)
		{
   		MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
      											  i,
            	                          "Box depth",
               	                       ObjectCode);
   	}
      else
      	ABoxDepth = MyPEcoDynClass->GetBoxDepth(i);
      if (
      		(ABoxDepth > 0.0) &&
      	   (BoxArray[i].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
            (BoxArray[i].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
            (BoxArray[i].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            (BoxArray[i].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) ||    //No northern ocean boundary
            (
         		(ABoxDepth > 0.0) &&
            	(MyTransportPointer == NULL)
         	)
         )

      {
      	ZooProduction[i] = 0;
   		Graze(i);
        Drool(i);
   		Faeces(i);
        Respire(i);
        Excrete(i);   //O excretado ? uma frac??o do que fica depois da respira??o, deste modo, ? tamb?m uma frac??o constante da ingest?o
        Die(i);
      }
   }

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooFlux[i] = ZooFlux[i] + ZooProduction[i];
		ZooNetProduction[i] = ZooProduction[i];
	}
}








void TSangoZooplankton::Graze(int ABoxNumber)
{
	double PhytoMass, ChlorophyllCarbonRatio;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
	if (ZooBiomass[MyBoxNumber] > 0.0)
	{
		if (MyPhytoPointer != NULL)
		{
			MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoMass,
												 MyBoxNumber,
												 "Phytoplankton biomass",
												 ObjectCode);
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllCarbonRatio,
												 MyBoxNumber,
												 "Chlorophyll : Carbon",
												 ObjectCode);
		   //Debugger(PhytoMass)
			// Phytoplankton returns its mass as ug chl a l-1
			// convert to mg C m-3
         ChlorophyllToCarbon = 1 /  ChlorophyllCarbonRatio;

			PhytoMass = PhytoMass * ChlorophyllToCarbon;
			// only graze if there is more than the lower limit of C

			ZooGrossProduction[MyBoxNumber] = 0;
			//NG : GrossProduction stays at 0 if zooplankton doesn't graze 4/6/96
			// in  mg C m-3
			if (PhytoMass > PhytoLowerLimit)
			{
				// Adams & Steele, 1966; Parsons et al., 1967; Nassogne, 1970 - Eq. 101 - Parsons PTH}
				// ration in mg/mg/day}
				Ration = Rmax * (1.0 - exp(KGraze *(PhytoLowerLimit - PhytoMass)));

				//Debugger(Ration);
				// ZooProduction in mg/m3/day  Fresh Weight ?

				ZooProd = ZooBiomass[MyBoxNumber] * Ration; // /assimilation


				ZooGrossProduction[MyBoxNumber] = ZooProd;
			  // phyto removal in mg C m-3

					PhytoMass = ZooProd
									/ (DryWeightToFreshWeight
										* CarbonToDryWeight);
				if (ZooProd > 0.0)
				{
					// return negative value because this is subtracted
					MyPhytoPointer->Update(GetEcoDynClassName(), -PhytoMass,
													MyBoxNumber,
													"Phytoplankton biomass",
													ObjectCode);
					ZooProduction[MyBoxNumber] = ZooProd;
				}
			}
		}
	}
}

void TSangoZooplankton::Drool(int ABoxNumber)
{
	double NutrientMass, CarbonDrooled;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	if (ZooBiomass[MyBoxNumber] > 0.0)
	{
		if (ZooProduction[MyBoxNumber] != 0)
		{
			CarbonDrooled = ZooProduction[MyBoxNumber]
								 * DOCLoss;
			ZooProduction[MyBoxNumber] = ZooProduction[MyBoxNumber]
									 - CarbonDrooled;
			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonDrooled
									* CUBIC
									/ (DryWeightToFreshWeight
										* CarbonToDryWeight
										* RedfieldCFactor * NITROGENATOMICWEIGHT / RedfieldNFactor);
				// return positive value because this is added
				MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass,
													 MyBoxNumber,
													 "Ammonia",
													 ObjectCode);
			}
		}
	}
}

// Sort out excrete and respire, because:
// zooplankton excretes due to metabolic activity, i.e. respiration, and
// therefore it seems one method is redundant

void TSangoZooplankton::Excrete(int ABoxNumber)
{
	double NutrientMass, CarbonExcreted;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	if (ZooBiomass[MyBoxNumber] > 0.0)
	{
		if (ZooProduction[MyBoxNumber] != 0)
		{
			CarbonExcreted = ZooProduction[MyBoxNumber]
								  * ExcretionLoss;
			ZooProduction[MyBoxNumber] = ZooProduction[MyBoxNumber]
									 - CarbonExcreted;
			if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonExcreted
									* CUBIC
									/ (DryWeightToFreshWeight
										* CarbonToDryWeight
										* RedfieldCFactor * NITROGENATOMICWEIGHT /RedfieldNFactor);
				// return positive value because this is added
				MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass,
													MyBoxNumber,
													"Ammonia",
													ObjectCode);
			}
		}
	}
}


void TSangoZooplankton::Respire(int ABoxNumber)
{
	double NutrientMass, CarbonMetabolised;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;

	//TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	if (ZooBiomass[MyBoxNumber] > 0.0)
	{
		if (ZooProduction[MyBoxNumber] != 0)
		{
			CarbonMetabolised = ZooProduction[MyBoxNumber]
									  * Metabolism;
			ZooProduction[MyBoxNumber] = ZooProduction[MyBoxNumber]
									 - CarbonMetabolised;
			/*if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonMetabolised
									* CUBIC
									/ (DryWeightToFreshWeight
										* CarbonToDryWeight
										* RedfieldCFactor * NITROGENATOMICWEIGHT /RedfieldNFactor);
				// return positive value because this is added
				MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass,
													 MyBoxNumber,
													 "Ammonia",
													 ObjectCode);
			}  */
		}
	}
}


void TSangoZooplankton::Faeces(int ABoxNumber)
{
//Embora a produ??o de fezes n?o esteja a ser calculada, por falta de informa??o
//h? uma frac??o do alimento que ? "egested" atrav?s do "Drool", de modo, que posso
//assumir que a referida frac??o corresponde ao "Drool" e ?s fezes.
}

void TSangoZooplankton::Die(int ABoxNumber)
{
	double /*NutrientMass,*/ ZooDead, PhytoMass, MyPOMFlux;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;

	//TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();

	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();

	if (ZooBiomass[MyBoxNumber] > 0.0)
	{
//		if (ZooProduction[MyBoxNumber] != 0)  // Changed by JGF/NG 96.06.04 Die all the time
		{
			if (MyPhytoPointer != NULL)
			{
				MyPhytoPointer->Inquiry(GetEcoDynClassName(), PhytoMass,
													 MyBoxNumber,
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

				DeathLoss = DeathLoss
								+ MaximumDeathLoss
								* exp(-KZooStarvedByPhyto
										* PhytoMass);        // Day-1


			}

			ZooDead = ZooBiomass[MyBoxNumber]
							 * DeathLoss;
			ZooFlux[MyBoxNumber] = ZooFlux[MyBoxNumber]
							- ZooDead;


		//	ZooNetProduction[MyBoxNumber] = ZooProduction[MyBoxNumber]-CarbonDead;
		// NG 16/6/96 : supprim? pour ne pas tenir compte de Mortality et
		// remis ds Go().




  //		Debugger(CarbonDead);

			/*if (MyNutrientPointer != NULL)
			{
				// All values returned in umol m-3 N
				NutrientMass = CarbonDead
									* CUBIC
									/ (DryWeightToFreshWeight
										* CarbonToDryWeight
										* RedfieldCFactor * NITROGENATOMICWEIGHT /RedfieldNFactor);
				// return positive value because this is added
				MyNutrientPointer->Update(GetEcoDynClassName(), NutrientMass,
													 MyBoxNumber,
													 "Ammonia",
													 ObjectCode);
			} */

         if (MySPMPointer != NULL)
			{

      		MyPOMFlux = ZooDead / DryWeightToFreshWeight / CUBIC; //convertion from mg FW / m3 / d to mg DW / L / d

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
}

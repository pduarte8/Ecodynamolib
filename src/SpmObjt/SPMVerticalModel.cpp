// SPM object CPP code file
// @ J. Gomes Ferreira
// Rev. A 96.02.27
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
#include "spmobjt.h"
#include "iodll.h"

// TVerticalModelSPMOceanic Class
//
// Constructors invoked outside EcoDyn shell...
//

TVerticalModelSPMOceanic::TVerticalModelSPMOceanic(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
        double aSPMConcentration[], double aSPMLoad[],
        double riverPOC, double oceanPOC, double aPOCConcentration[], double aPOCLoad[],
        double riverPON, double oceanPON, double aPONConcentration[], double aPONLoad[],
        int nParams, double localLatitude, double localHeight, double localSalt,
        double localTemperature, double turbulenceFactor, double aSPMRessuspension[],
        double detritusBreakDown, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double pocFraction, double spmSedimentationRate)
    :TSPMOceanic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, riverSPM, oceanSPM, aSPMConcentration, aSPMLoad,
                nParams, localLatitude, localHeight, localSalt,
                localTemperature, turbulenceFactor, aSPMRessuspension)
{
    PreBuildVerticalModelSPM();
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        POCConcentration[i] = aPOCConcentration[i];
        POCLoad[i] = aPOCLoad[i];
        PONConcentration[i] = aPONConcentration[i];
        PONLoad[i] = aPONLoad[i];
        SPMSedimentationRate[i] = spmSedimentationRate;
    }
    RiverPOC = riverPOC;
    OceanPOC = oceanPOC;
    RiverPON = riverPON;
    OceanPON = oceanPON;
    DetritusBreakDown = detritusBreakDown;
    RedfieldCFactor = redfieldCFactor;
    RedfieldNFactor = redfieldNFactor;
    RedfieldPFactor = redfieldPFactor;
    POCFraction = pocFraction;
}

TVerticalModelSPMOceanic::TVerticalModelSPMOceanic(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
     :TSPMOceanic(className, timeStep, morphologyFilename, variablesFilename, parametersFilename)
{
    BuildVerticalModelSPM();
}
// ...

TVerticalModelSPMOceanic::TVerticalModelSPMOceanic(TEcoDynClass* APEcoDynClass,
        char* className) : TSPMOceanic(APEcoDynClass, className)
{
    BuildVerticalModelSPM();
}

void TVerticalModelSPMOceanic::PreBuildVerticalModelSPM()
{
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{
		POCConcentration = new double[NumberOfBoxes];
		POCLoad = new double[NumberOfBoxes];
		POCFlux = new double[NumberOfBoxes];
        PONConcentration = new double[NumberOfBoxes];
		PONLoad = new double[NumberOfBoxes];
		PONFlux = new double[NumberOfBoxes];
    }
    // Fluxes and conversions
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    POCFlux[i] = 0;
        PONFlux[i] = 0;
    }
}

void TVerticalModelSPMOceanic::BuildVerticalModelSPM()
{
    PreBuildVerticalModelSPM();

	int X,Y, XV, YV, XL, YL;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            // NumberOfVariables and VariableNameArray[] filled in SPMBasic
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                if (strcmp(MyVariable, "Particulate organic carbon") == 0)
                    {
                    // Read the end-member values

                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);

                    RiverPOC = GenericRiver;  // mg l-1
                    OceanPOC = GenericOcean;  // mg l-1

                    //Debugger(RiverPOC); Debugger(OceanPOC);
                    if (!PReadWrite->FindString("POC concentration values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, POCConcentration[j]);
                            PReadWrite->ReadNumber(X+NumberOfBoxes+5+j, i, POCLoad[j]);
                        }
                    }
                    else
                    {
                        if (!PReadWrite->FindString("POC load values", XL, YL))
                        {
                            XL = XV + 1;
                            YL = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, POCConcentration[j]);  // mg l-1
                            PReadWrite->ReadNumber(XL, YL + 1 + j, POCLoad[j]);  // mg l-1
                        }
                    }
                }
                else if (strcmp(MyVariable, "Particulate organic nitrogen") == 0)
                {

                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);

                    RiverPON = GenericRiver;  // mg l-1
                    OceanPON = GenericOcean;  // mg l-1

                    //Debugger(RiverPON); Debugger(OceanPON);
                    if (!PReadWrite->FindString("PON concentration values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, PONConcentration[j]);
                            PReadWrite->ReadNumber(X+NumberOfBoxes+5+j, i, PONLoad[j]);
                        }
                    }
                    else
                    {
                        if (!PReadWrite->FindString("PON load values", XL, YL))
                        {
                            XL = XV + 1;
                            YL = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, PONConcentration[j]);  // mg l-1
                            PReadWrite->ReadNumber(XL, YL + 1 + j, PONLoad[j]);  // mg l-1
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TVerticalModelSPMOceanic::BuildVerticalModelSPM - Variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
	if (PReadWrite != NULL)
	{
        double MyValue;

        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            // NumberOfParameters filled in SPMBasic
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Detritus breakdown") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    DetritusBreakDown = MyValue; //ugC /ug C/ h
                }
                else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    RedfieldCFactor = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    RedfieldNFactor = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    RedfieldPFactor = MyValue;
                }
                else if (strcmp(MyParameter, "POC fraction") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    POCFraction = MyValue;
                }
                else if (strcmp(MyParameter, "Falling speed") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        SPMSedimentationRate[j] = MyValue;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TVerticalModelSPMOceanic::BuildVerticalModelSPM - Parameters undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}


TVerticalModelSPMOceanic::~TVerticalModelSPMOceanic()
{
    freeMemory();
}

void TVerticalModelSPMOceanic::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] POCConcentration;
            delete [] POCLoad;
            delete [] POCFlux;
            delete [] PONConcentration;
            delete [] PONLoad;
            delete [] PONFlux;
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
				cerr << "TVerticalModelSPMOceanic::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TVerticalModelSPMOceanic::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TSPMBasic
    if (strcmp(MyParameter, "SPM Ressuspension") == 0)
        value = SPMRessuspension[0];
    else if (strcmp(MyParameter, "Latitude") == 0)
        value = LocalLatitude;
    else if (strcmp(MyParameter, "Height") == 0)
        value = LocalHeight;
    else if (strcmp(MyParameter, "Salinity") == 0)
        value = LocalSalt;
    else if (strcmp(MyParameter, "Temperature") == 0)
        value = LocalTemperature;
    else if (strcmp(MyParameter, "Turbulence") == 0)
        value = TurbulenceFactor;

    // parameters from TVerticalModelSPMOceanic
    else if (strcmp(MyParameter, "Detritus breakdown") == 0)
    {
        value = DetritusBreakDown;
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
    else if (strcmp(MyParameter, "POC fraction") == 0)
    {
        value = POCFraction;
    }
    else if (strcmp(MyParameter, "Falling speed") == 0)
    {
        value = SPMSedimentationRate[0];
    }
    else
        value = 0.0;
    return value;
}

bool TVerticalModelSPMOceanic::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TSPMBasic
    if (strcmp(MyParameter, "SPM Ressuspension") == 0) {
        for (j = 0; j < NumberOfBoxes; j++)
            SPMRessuspension[j] = value;
    }
    else if (strcmp(MyParameter, "Latitude") == 0)
        LocalLatitude = value;
    else if (strcmp(MyParameter, "Height") == 0)
        LocalHeight = value;
    else if (strcmp(MyParameter, "Salinity") == 0)
        LocalSalt = value;
    else if (strcmp(MyParameter, "Temperature") == 0)
        LocalTemperature = value;
    else if (strcmp(MyParameter, "Turbulence") == 0)
        TurbulenceFactor = value;

    // parameters from TVerticalModelSPMOceanic
    else if (strcmp(MyParameter, "Detritus breakdown") == 0)
    {
        DetritusBreakDown = value;
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
    else if (strcmp(MyParameter, "POC fraction") == 0)
    {
        POCFraction = value;
    }
    else if (strcmp(MyParameter, "Falling speed") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            SPMSedimentationRate[j] = value;
    }
    else
        rc = false;

    return rc;
}


void TVerticalModelSPMOceanic::Inquiry(char* srcName, double &Value,
							  int BoxNumber,
							  char* ParameterName,
							  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Particulate organic carbon") == 0)
		Value = POCConcentration[MyBoxNumber];
	else if (strcmp(MyParameter, "Extinction coefficient Ktot") == 0)
		Value = TotalExtinctionCoefficient[MyBoxNumber];
    else if (strcmp(MyParameter, "Extinction coefficient Kchl") == 0)
		Value = ChlorophyllExtinctionCoefficient[MyBoxNumber];
    else if (strcmp(MyParameter, "Particulate organic nitrogen") == 0)
		Value = PONConcentration[MyBoxNumber];
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
                      "EcoDynamo Alert - Inquiry 5",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TVerticalModelSPMOceanic::Inquiry 5 - " << MyParameter << "does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}



void TVerticalModelSPMOceanic::Update(char* srcName, double Value,
						 int BoxNumber,
						 char* ParameterName,
						 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

    if (strcmp(MyParameter, "Particulate organic carbon") == 0)
		POCFlux[MyBoxNumber] = POCFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Particulate organic nitrogen") == 0)
	    PONFlux[MyBoxNumber] = PONFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
	    char Caption[129];
	    strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 5",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TVerticalModelSPMOceanic::Update 5 - " << MyParameter << "does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}

bool TVerticalModelSPMOceanic::SetVariableValue(char* srcName, double Value,
						 int BoxNumber,
						 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Particulate organic carbon") == 0)
		POCConcentration[BoxNumber] = Value;
	else if (strcmp(VariableName, "Extinction coefficient Ktot") == 0)
		TotalExtinctionCoefficient[BoxNumber] = Value;
    else if (strcmp(VariableName, "Extinction coefficient Kchl") == 0)
		ChlorophyllExtinctionCoefficient[BoxNumber] = Value;
    else if (strcmp(VariableName, "Particulate organic nitrogen") == 0)
		PONConcentration[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


// AP, 2007.07.11
bool TVerticalModelSPMOceanic::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    bool spmValues = false, pocValues = false, ponValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Suspended matter") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverSPM, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanSPM, 8);
            spmValues = true;
        }
        else if (strcmp(VariableNameArray[i], "Particulate organic carbon") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverPOC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanPOC, 8);
            pocValues = true;
        }
        else if (strcmp(VariableNameArray[i], "Particulate organic nitrogen") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverPON, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanPON, 8);
            ponValues = true;
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save others variables' values
     */
    if (spmValues || pocValues || ponValues) {
        if (spmValues) {
            PReadWrite->WriteCell("SPM concentration values");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("SPM load values");
            PReadWrite->WriteSeparator();
        }
        if (pocValues) {
            PReadWrite->WriteCell("POC concentration values");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("POC load values");
            PReadWrite->WriteSeparator();
        }
        if (ponValues) {
            PReadWrite->WriteCell("PON concentration values");
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell("PON load values");
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            if (spmValues) {
                PReadWrite->WriteCell(SPMConcentration[j], 8);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(SPMLoad[j], 8);
                PReadWrite->WriteSeparator();
            }
            if (pocValues) {
                PReadWrite->WriteCell(POCConcentration[j], 8);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(POCLoad[j], 8);
                PReadWrite->WriteSeparator();
            }
            if (ponValues) {
                PReadWrite->WriteCell(PONConcentration[j], 8);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(PONLoad[j], 8);
                PReadWrite->WriteSeparator();
            }
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TVerticalModelSPMOceanic::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    bool spmValues = false;
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();

        // parameters from TSPMBasic
        if (strcmp(ParametersNameArray[i], "SPM Ressuspension") == 0)
        {
            PReadWrite->WriteCell(SPMRessuspension[0], 8);
            spmValues = true;
        }
        else if (strcmp(ParametersNameArray[i], "Latitude") == 0)
        {
            PReadWrite->WriteCell(LocalLatitude, 3);
        }
        else if (strcmp(ParametersNameArray[i], "Height") == 0)
        {
            PReadWrite->WriteCell(LocalHeight, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Salinity") == 0)
        {
            PReadWrite->WriteCell(LocalSalt, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Temperature") == 0)
        {
            PReadWrite->WriteCell(LocalTemperature, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Turbulence") == 0)
        {
            PReadWrite->WriteCell(TurbulenceFactor, 8);
        }

        // parameters from TVerticalModelSPMOceanic
        else if (strcmp(ParametersNameArray[i], "Detritus breakdown") == 0)
        {
            PReadWrite->WriteCell(DetritusBreakDown, 8);
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
        else if (strcmp(ParametersNameArray[i], "POC fraction") == 0)
        {
            PReadWrite->WriteCell(POCFraction, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Falling speed") == 0)
        {
            PReadWrite->WriteCell(SPMSedimentationRate[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save other parameters' initial values
     */
    if (spmValues) {
        PReadWrite->WriteCell("SPM Ressuspension values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            PReadWrite->WriteCell(SPMRessuspension[j], 8);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TVerticalModelSPMOceanic::Go()
{
	Mineralize();
	ExtinctionCoefficientKTot();
	ExtinctionCoefficientKChl();
}


void TVerticalModelSPMOceanic::Integrate()
{
//    Generic = POCConcentration; GenericFlux = POCFlux;
	Integration(POCConcentration, POCFlux);
//	POCConcentration = Generic; POCFlux = GenericFlux;
	// Transport the POC
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
   	MyTransportPointer->Go(POCConcentration, 0, 0, 0);

//   Generic = PONConcentration; GenericFlux = PONFlux;
	Integration(PONConcentration, PONFlux);
//	PONConcentration = Generic; PONFlux = GenericFlux;
	// Transport the PON
	MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
   	MyTransportPointer->Go(PONConcentration, 0, 0, 0);

}


void TVerticalModelSPMOceanic::Mineralize()
{
	double POCBreakDown, PONBreakDown;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
      POCBreakDown = DetritusBreakDown * POCConcentration[i];
      PONBreakDown = DetritusBreakDown * PONConcentration[i];

      POCFlux[i] = POCFlux[i] - POCBreakDown;
      PONFlux[i] = PONFlux[i] - PONBreakDown;
   	//SPMFlux[i] = SPMFlux[i] - (POCBreakDown + PONBreakDown);

      if (MyNutrientPointer != NULL)
		{
      	MyNutrientPointer->Update(GetEcoDynClassName(), POCBreakDown * CUBIC /
                                    12 /*Carbon atmoc weight*/,
												i,
												"DOC",
												ObjectCode);

			MyNutrientPointer->Update(GetEcoDynClassName(), PONBreakDown * CUBIC /
                                    NITROGENATOMICWEIGHT,
												i,
												"DON",
												ObjectCode);
      }

   }
}


void TVerticalModelSPMOceanic :: Falling()
{
	double BoxDepth;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
   	BoxDepth = MyPEcoDynClass->GetBoxDepth(i);

      POCFlux[i] = POCFlux[i] - SPMSedimentationRate[i] / BoxDepth * POCConcentration[i]; //Ao POCFlux de cada caixa ? subtra?da a frac??o que cai
      if (i > 0)
      	POCFlux[i] = POCFlux[i] + SPMSedimentationRate[i] / BoxDepth * POCConcentration[i - 1]; //Ao POCFlux de cada caixa ? somada a frac??o que cai da caixa superior

      PONFlux[i] = PONFlux[i] - SPMSedimentationRate[i] / BoxDepth * PONConcentration[i]; //Ao PONFlux de cada caixa ? subtra?da a frac??o que cai
      if (i > 0)
      	PONFlux[i] = PONFlux[i] + SPMSedimentationRate[i] / BoxDepth * PONConcentration[i - 1]; //Ao PONFlux de cada caixa ? somada a frac??o que cai da caixa superior

   }
}


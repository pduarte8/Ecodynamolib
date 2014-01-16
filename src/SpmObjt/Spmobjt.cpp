// SPM object CPP code file
// @ J. Gomes Ferreira
// Rev. A 96.02.27
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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

// TSPM Class
//
// Constructors invoked outside EcoDyn shell...
//

TSPM::TSPM(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildSPM(className);
}

TSPM::TSPM(char* className, float timeStep, char* morphologyFilename) :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology()) {
        BuildSPM(className);
    }
}
// ...

TSPM::TSPM(TEcoDynClass* APEcoDynClass, char* className) :TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcoDynClass;
    BuildSPM(className);
}

void TSPM::BuildSPM(char* className)
{
	int i,j;
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
    TimeStep = MyPEcoDynClass->GetTimeStep();

    AStartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    ATime = AStartTime;
    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
    /*AStartTime = MyPEcoDynClass->GetAStartTime();
    ATime = AStartTime;
    AFinishTime = MyPEcoDynClass->GetAFinishTime();*/
    //ADay = 1; 
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    BoxArray = MyPEcoDynClass->BoxArray;
	ObjectCode = SPMOBJECTCODE;

	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
		SPMConcentration = new double[NumberOfBoxes];
		SPMLoad = new double[NumberOfBoxes];
		SPMFlux = new double[NumberOfBoxes];
		SPMRessuspension = new double[NumberOfBoxes];
		TotalExtinctionCoefficient = new double[NumberOfBoxes];
		ChlorophyllExtinctionCoefficient = new double[NumberOfBoxes];
		SPMSedimentationRate = new double[NumberOfBoxes];
		/*SPMInterfaceExchange
			= new AdvectionDiffusionRecord[NumberOfBoxes*NumberOfBoxes];*/

		// Create generics

/*		Generic     = new double[NumberOfBoxes];
		GenericLoad = new double[NumberOfBoxes];
		GenericFlux = new double[NumberOfBoxes];*/
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "SPM object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TSPM::BuildSPM - SPM object array not dimensioned" << endl;
#endif  // __BORLANDC__
    for (i = 0; i < NumberOfBoxes; i++)
    {
   	    SPMConcentration[i] = 0.0;
        SPMLoad[i] = 0.0;
        SPMFlux[i] = 0.0;
        SPMRessuspension[i] = 0.0;
        TotalExtinctionCoefficient[i] = 0.0;
        ChlorophyllExtinctionCoefficient[i] = 0.0;
        SPMSedimentationRate[i] = 0.0;
    }

}

TSPM::~TSPM()
{
    freeMemory();
}

void TSPM::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] SPMConcentration;
            delete [] SPMLoad;
            delete [] SPMFlux;
            delete [] SPMRessuspension;
            delete [] SPMSedimentationRate;
            delete [] TotalExtinctionCoefficient;
            delete [] ChlorophyllExtinctionCoefficient;
            //delete [] SPMInterfaceExchange;
    /*		delete [] VariableNameArray;
            delete [] Generic;
            delete [] GenericLoad;
            delete [] GenericFlux;*/
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
				cerr << "TSPM::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TSPM::Go()
{
	// Active methods
	Flocculate();
	Deposit();
	Turbulence();
	Ressuspend();
	Dissolve();
	ExtinctionCoefficientKTot();
	ExtinctionCoefficientKChl();
}

void TSPM::Update(char* srcName, double Value,
						 int BoxNumber,
						 char* ParameterName,
						 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

    if (strcmp(MyParameter, "Suspended matter") == 0)
	    SPMFlux[MyBoxNumber] = SPMFlux[MyBoxNumber] + Value;
    /*else
    {
    	char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
    	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 4",
				  MB_OK | MB_ICONHAND);
    } */
}

bool TSPM::SetVariableValue(char* srcName, double Value,
						 int BoxNumber,
						 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Suspended matter") == 0)
		SPMConcentration[BoxNumber] = Value;
	else if (strcmp(VariableName, "Sedimentation rate") == 0)
		SPMSedimentationRate[BoxNumber] = Value;
    else if (strcmp(VariableName, "Particulate organic carbon") == 0)
		SPMConcentration[BoxNumber] = Value / POCFraction;
    else if (strcmp(VariableName, "Extinction coefficient Ktot") == 0)
		TotalExtinctionCoefficient[BoxNumber] = Value;
    else if (strcmp(VariableName, "Extinction coefficient Kchl") == 0)
	    ChlorophyllExtinctionCoefficient[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TSPM::Inquiry(char* srcName, double &Value,
							  int BoxNumber,
							  char* ParameterName,
							  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Suspended matter") == 0)
		Value = SPMConcentration[MyBoxNumber];
	else if (strcmp(MyParameter, "Sedimentation rate") == 0)
		Value = SPMSedimentationRate[MyBoxNumber];
    else if (strcmp(MyParameter, "Particulate organic carbon") == 0)
		Value = SPMConcentration[MyBoxNumber]
				  * POCFraction;
    else if (strcmp(MyParameter, "Extinction coefficient Ktot") == 0)
		Value = TotalExtinctionCoefficient[MyBoxNumber];
    else if (strcmp(MyParameter, "Extinction coefficient Kchl") == 0)
	    Value = ChlorophyllExtinctionCoefficient[MyBoxNumber];
    else
    {
       /*Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 4",
                      MB_OK | MB_ICONHAND);
        }      */
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TSPM::Inquiry(char* srcName, double &Value,
							  int BoxX,
							  int BoxY,
							  char* ParameterName,
							  int AnObjectCode)
{
/*	static char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxX * NumberOfBoxes + BoxY;
    strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "POC advective flux matrix") == 0) // kg per integration period - 96.06.24 jgf
    {
		Value = SPMInterfaceExchange[MyBoxNumber].AdvectiveFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
									  * POCFraction
									  / (CUBIC);
    }
	else if (strcmp(MyParameter, "POC dispersive flux matrix") == 0) // kg per integration period - 96.06.24 jgf
    {
		Value = SPMInterfaceExchange[MyBoxNumber].DispersiveFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
									  * POCFraction
									  / (CUBIC);
    }
	else if (strcmp(MyParameter, "POC net flux matrix") == 0) // kg per integration period - 96.06.24 jgf
    {
		Value = SPMInterfaceExchange[MyBoxNumber].InFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
									  * POCFraction
									  / (CUBIC);
    }
    else
    {
	    static char Caption[129];
	    strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Inquiry.",
				  MB_OK | MB_ICONHAND);
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);   */
}

void TSPM::Integrate()
{
//	Generic = SPMConcentration; GenericFlux = SPMFlux;
	Integration(SPMConcentration, SPMFlux);
//	SPMConcentration = Generic; SPMFlux = GenericFlux;

	// Transport the SPM
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
	// Old calls
		MyTransportPointer->Go(SPMConcentration, SPMLoad, RiverSPM, OceanSPM);

		// Important note - The pointer variables must be passed like this (below) to a
		// pointer which does not point to the Exchange (in transport (or GIE - here) address
		// otherwise, the pointer starting address is retained, and as exchange changes, so
		// do the results in DINInterface exchange - therefore, the real values must be
		// specifically converted and assigned one by one - wow 96.06.24 JGF

	 }
}

// Protected methods
void TSPM::Flocculate()
{
}
void TSPM::Deposit()
{


}

void TSPM::Turbulence()
{
}

void TSPM::Ressuspend()
{

	// Ressuspension is added to the box flux - it must be a positive term in order to
	// reduce deposition: This term is simply added, because the material which is
	// ressuspended is not calculated from SPM in the water column, but has to do with
	// the bottom shear stress, sediment grain size and compaction - JGF - 94.04.11}

	for (int i = 0; i < NumberOfBoxes; i++)
		SPMFlux[i] = SPMFlux[i]
						 + SPMRessuspension[i];
}

void TSPM::Dissolve()
{
}

void TSPM::Mineralize()
{
}

void TSPM::SettlingSpeed()
{

}

double TSPM::SPMFraction(int k,
								 double TotalSPM)
{
    return 1.0;
}

void TSPM::ExtinctionCoefficientKTot()
{

}


void TSPM::ExtinctionCoefficientKChl()
{

}







// Hydrodynamic object CPP code file
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "hydrobjt.h"
#include "globals.h"

// TTransport Class
//
// Constructors invoked outside EcoDyn shell...
//
TTransport::TTransport(char* className, float timeStep, char* morphologyFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology()) {
        BuildTransport(className);
    }
}

TTransport::TTransport(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildTransport(className);
}
// ...

TTransport::TTransport(TEcoDynClass* PEcoDynClass, char* className)
                        :TEcoDynClass()
{
    MyPEcoDynClass = PEcoDynClass;
    BuildTransport(className);
}

void TTransport::BuildTransport(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfBoxes = MyPEcoDynClass->GetGridSize();
    TimeStep = MyPEcoDynClass->GetTimeStep();
    ATimeStep = MyPEcoDynClass->GetTimeStep();

    AStartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    ATime = AStartTime;
    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
    //ADay = 1;
    /*AStartTime = MyPEcoDynClass->GetAStartTime();
    ATime = AStartTime;
    AFinishTime = MyPEcoDynClass->GetAFinishTime(); */

    if (NumberOfBoxes > 0)         // Initialise arrays for variable pairs - one
    {
      Salt = new double[NumberOfBoxes];
      SaltLoad = new double[NumberOfBoxes];
      MixedLayer = new double[NumberOfBoxes];

		// Create generics
//		GenericFlux = new double[NumberOfBoxes];
//		GenericLoad = new double[NumberOfBoxes];
//		Generic     = new double[NumberOfBoxes];
//		GenericExchange = new double[NumberOfBoxes];

		for (int i = 0; i < NumberOfBoxes; i++)
        {
			MixedLayer[i] = 10; // temporary
                        //RiverLoads[i] = 0.0;
        }
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Hydrodynamic array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TTransport::BuildTransport - hydrodynamic array not dimensioned" << endl;
#endif  // __BORLANDC__

	ATimeStep = MyPEcoDynClass->GetTimeStep() * DAYSTOSECONDS;
    BoxArray = MyPEcoDynClass->BoxArray;

	VariableCount = 0;
    ParameterCount = 0;
    NumberOfVariables = 0;
    NumberOfParameters = 0;
	ObjectCode = HYDROBJECTCODE;

    if (MyPEcoDynClass->HasALoadRecord())
    {
        MyLoadRecord = MyPEcoDynClass->GetALoadRecord();
    }
    else
    {
        MyLoadRecord = NULL;
    }

    if (MyPEcoDynClass->HasALossesRecord())
    {
        MyLossesRecord = MyPEcoDynClass->GetALossesRecord();
    }
    else
    {
        MyLossesRecord = NULL;
    }

    if (MyPEcoDynClass->HasARiverRecord())
    {
        MyRiverRecord = MyPEcoDynClass->GetARiverRecord();
        NumberOfRivers = MyPEcoDynClass->GetNumberOfRivers();
        //Debugger(NumberOfRivers);
    }
    else
    {
        MyRiverRecord = NULL;
        NumberOfRivers = 0;
    }

    if (MyPEcoDynClass->HasASeaBoundariesRecord())
    {
        MySeaBoundariesRecord = MyPEcoDynClass->GetASeaBoundariesRecord();
        NumberOfSeaBoundaries = MyPEcoDynClass->GetNumberOfSeaBoundaries();
        //Debugger(NumberOfSeaBoundaries);
    }
    else
    {
        MySeaBoundariesRecord = NULL;
        NumberOfSeaBoundaries = 0;
    }
}

TTransport::~TTransport()
{
    freeMemory();
}

void TTransport::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] Salt;
            delete [] SaltLoad;
    //		delete [] GenericFlux;
    //		delete [] Generic;
    //      delete [] GenericLoad;
    //      delete [] GenericExchange;
            delete [] MixedLayer;
        }
        /*if (MyPEcoDynClass->GetNumberOfLoads())
        {
            delete [] Loads;
            delete [] MyLoads;
        }
        if (MyPEcoDynClass->GetNumberOfLosses())
        {
            delete [] Losses;
            delete [] MyLosses;
        } */
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
        cerr << "TTransport::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TTransport::Inquiry(char* srcName, double &Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Salinity") == 0)
		Value = Salt[MyBoxNumber];
	else if (strcmp(MyParameter, "Mixed layer depth") == 0)
		Value = MixedLayer[MyBoxNumber];
	else if (strcmp(MyParameter, "Water density") == 0)
		Value = 1026.75; // seawater at 35psu and 10oC;
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
            cerr << "TTransport::Inquiry 5 -" << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.05.28
bool TTransport::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Salinity") == 0)
		Salt[BoxNumber] = Value;
	else if (strcmp(VariableName, "Mixed layer depth") == 0)
		MixedLayer[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TTransport::EnergyAcrossSurface(double* AGeneric,
												 int BoxNumber,
												 char* ParameterName)
{
	// Fill this appropriately
}

void TTransport::Go(double* AGeneric,
						  double* AGenericLoad,
						  double ARiver,
						  double AOcean)
{

}

void TTransport::Go(double* AGeneric,
						  double* AGenericLoad,
						  double* AGenericExchange,
						  double ARiver,
						  double AOcean)
{
/*	Generic = AGeneric;
	GenericLoad = AGenericLoad;
	GenericExchange = AGenericExchange;
	GenericRiver = ARiver;
	GenericOcean = AOcean;
	AdvectDiffuse();
	AGeneric = Generic;
	AGenericExchange = GenericExchange;  */
}


// Protected methods
void TTransport::AdvectDiffuse(double* Generic)
{
}

void TTransport::MixedLayerDepth()
{
	for (int i = 0; i < NumberOfBoxes; i++)
		MixedLayer[i] = 10;
}



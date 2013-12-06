// Flow object CPP code file
// @  J. Gomes Ferreira
// Rev. A 96.03.11
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "flowobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"


// TFlow Class
//
// Constructors invoked outside EcoDyn shell...
//
TFlow::TFlow(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow)
        :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    PreBuildFlow(className);
	// Read in the variables
    NumberOfVariables = nVars;

    // Create array for state variable names
	VariableNameArray = new VNA[NumberOfVariables];

	// read in the variable names
	for (int i = 0; i < NumberOfVariables; i++) {
		strcpy(VariableNameArray[i], aVarsNames[i]);
    }

	ModalFlow = modalFlow;
	FlowAmplitude = flowAmplitude;
	RandomFlow = randomFlow;
}

TFlow::TFlow(char* className, float timeStep, char* morphologyFilename, char* variablesFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    strcpy(VarsFileName, variablesFilename);
    if (OpenMorphology()) {
        BuildFlow(className);
    }
}
// ...

TFlow::TFlow(TEcoDynClass* PEcoDynClass, char* className)
				 : TEcoDynClass()
{
	// Receive pointer to integrate
	MyPEcoDynClass = PEcoDynClass;
    BuildFlow(className);
}

void TFlow::PreBuildFlow(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes

	VariableCount = 0;

	TimeStep = MyPEcoDynClass->GetTimeStep();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	TimeBefore = MyPEcoDynClass->GetTimeBefore();

	ObjectCode = FLOWOBJECTCODE;

	MainRiverFlow = 0;
	ModalFlow = 400;
	FlowAmplitude = 0;
	RandomFlow = 0;
}

void TFlow::BuildFlow(char* className)
{
    PreBuildFlow(className);

	// Get variables file name
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Flow");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Flow variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TFlow::BuildFlow - variables file missing." << endl;
#endif   // __BORLANDC__
	}
    else {
        // Read in the variables
        int X,Y;

        if (PReadWrite->FindString("Flow", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
            }
            PReadWrite->ReadNumber(X+3, Y, ModalFlow);
            PReadWrite->ReadNumber(X+4, Y, FlowAmplitude);
            PReadWrite->ReadNumber(X+5, Y, RandomFlow);
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TFlow::~TFlow()
{
    freeMemory();
}

void TFlow::freeMemory()
{
    try {
        if (NumberOfVariables > 0)
            delete [] VariableNameArray;
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
				cerr << "TFlow::freeMemory - " << exc.what() << endl;
		}
#endif   // __BORLANDC__
}

bool TFlow::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Flow");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Flow");
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
            PReadWrite->WriteCell(VariableNameArray[i]);
        }
        else
        {
            PReadWrite->WriteCell(VariableNameArray[i]);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ModalFlow, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(FlowAmplitude, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RandomFlow, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

void TFlow::Go()
{
	// Why bother with this?? JGF 96.02.09

	// Active methods
	MyTime = MyPEcoDynClass->GetTime();

	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
	CalculateFlow();
}

void TFlow::Inquiry(char* srcName, double &Value,
								int BoxNumber,
								char* ParameterName,
								int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Main flow") == 0)
		Value = MainRiverFlow;
    else
        Value = 0;

    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TFlow::CalculateFlow()
{
	randomize();
	MainRiverFlow = ModalFlow
						 + ( cos ( M_PI * ( JulianDay + 50 ) / 180)) // -50 by default
						 * FlowAmplitude
						 + random(RandomFlow);
}

// AP, 2007.05.28
bool TFlow::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Main flow") == 0)
    {
        MainRiverFlow = Value;
    }
    else
        rc = false;

    return rc;
}








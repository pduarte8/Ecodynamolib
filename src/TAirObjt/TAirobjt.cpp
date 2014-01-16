// Air temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.08

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "tairobjt.h"
#include "iodll.h"

// TAirTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TAirTemperature::TAirTemperature(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    PreBuildAirTemperature(className);

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);
}

TAirTemperature::TAirTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    strcpy(VarsFileName, variablesFilename);
    if (OpenMorphology()) {
        BuildAirTemperature(className);
    }
}
// ...

TAirTemperature::TAirTemperature(TEcoDynClass* APEcodynClass, char* className)
											: TEcoDynClass()
{
	// Receive pointer to integrate
	MyPEcoDynClass = APEcodynClass;
    BuildAirTemperature(className);
}

void TAirTemperature::PreBuildAirTemperature(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
	VariableCount = 0;

	TimeStep = MyPEcoDynClass->GetTimeStep();
    TimeBefore = MyPEcoDynClass->GetTimeBefore();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	ObjectCode = TAIROBJECTCODE;

	// Initialise air temperature
	MeanAirTemperature = 0;
}

void TAirTemperature::BuildAirTemperature(char* className)
{
    PreBuildAirTemperature(className);

	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("AirTemperature");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "AirTemperature variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TAirTemperature::BuildAirTemperature - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        int X,Y;

        if (PReadWrite->FindString("Air temperature", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TAirTemperature::~TAirTemperature()
{
    freeMemory();
}

void TAirTemperature::freeMemory()
{
    try {
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
				cerr << "TAirTemperature::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TAirTemperature::Go()
{
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
}

void TAirTemperature::Integrate()
{
	GetAirTemperature();
}

void TAirTemperature::Inquiry(char* srcName, double &Value,
											 int BoxNumber,
											 char* ParameterName,
											 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name

	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Air temperature") == 0)
        Value = MeanAirTemperature;
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
                      "EcoDynamo Alert - Inquiry",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TAirTemperature::Inquiry - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TAirTemperature::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Air temperature") == 0)
        MeanAirTemperature = Value;
    else
        rc = false;

    return rc;
}

bool TAirTemperature::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("AirTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Air temperature");
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
    CloseDataFile((void*)PReadWrite);

    return true;
}

// Protected methods

void TAirTemperature::GetAirTemperature()
{
	MeanAirTemperature = 13	* ( 1.1 - cos ( ( 2 * M_PI * ( MyTime - 380 ) ) / 365 ) );
}




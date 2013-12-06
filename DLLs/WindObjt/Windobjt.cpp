// Wind object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09
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
#include "windobjt.h"
#include "iodll.h"

// TWind Class
//
// Constructors invoked outside EcoDyn shell...
//

TWind::TWind(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    PreBuildWind(className);

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);
}

TWind::TWind(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    strcpy(VarsFileName, variablesFilename);
    if (OpenMorphology()) {
        BuildWind(className);
    }
}
// ...

TWind::TWind(TEcoDynClass* APEcodynClass, char* className)
            : TEcoDynClass()
{
	// Receive pointer to integrate
    MyPEcoDynClass = APEcodynClass;
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
    BuildWind(className);
}

void TWind::PreBuildWind(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    // Dummy angle of 90o
    Direction = M_PI/4;
    // Dummy speed of 6 ms-1
    Speed = 6;
    // Get array size
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    // Initialise pointers
    if (NumberOfBoxes > 0)                       // Initialise arrays for variables
    {
        AverageWindSpeed = new double[NumberOfBoxes];
        AverageWindSpeedX = new double[NumberOfBoxes];
        AverageWindSpeedY = new double[NumberOfBoxes];
        for (int i = 0; i < NumberOfBoxes; i++)
        {
            AverageWindSpeed[i] = Speed; // ms-1
            AverageWindSpeedX[i] = -Speed * sin(Direction); // ms-1
            AverageWindSpeedY[i] = -Speed * cos(Direction); // ms-1
        }
    }
    else
#ifdef __BORLANDC__
        MessageBox(0,
                    "Wind object array not dimensioned",
                    "EcoDynamo alert",
                    MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TWind::PreBuildWind - wind object array not dimensioned" << endl;
#endif  // __BORLANDC__
    VariableCount = 0;
    TimeStep = MyPEcoDynClass->GetTimeStep();
    TimeBefore = MyPEcoDynClass->GetTimeBefore();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    ObjectCode = WINDOBJECTCODE;
}

void TWind::BuildWind(char* className)
{
    PreBuildWind(className);

    // Read in the variables
    //	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Wind");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
       MessageBox(0, "Wind variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TWind::BuildWind - Variables file missing." << endl;
#endif  // __BORLANDC__
    }
    else {
        int X,Y;

        if (PReadWrite->FindString("Wind", X, Y))
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

TWind::~TWind()
{
	freeMemory();
}

void TWind::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] AverageWindSpeed;
            delete [] AverageWindSpeedX;
            delete [] AverageWindSpeedY;
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
				cerr << "TWind::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

bool TWind::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Wind");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Wind");
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

void TWind::Go()
{
    // Active methods
    MyTime = MyPEcoDynClass->GetTime();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    TimeBefore = MyTime;
}

void TWind::Integrate()
{
	CalculateWindSpeed();
}

void TWind::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Wind speed") == 0)
        Value = AverageWindSpeed[MyBoxNumber];
	else if (strcmp(MyParameter, "Wind speed (x component)") == 0)
		Value = AverageWindSpeedX[MyBoxNumber];
	else if (strcmp(MyParameter, "Wind speed (y component)") == 0)
		Value = AverageWindSpeedY[MyBoxNumber];
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
						cerr << "TWind::Inquiry - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TWind::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Wind speed") == 0)
        AverageWindSpeed[BoxNumber] = Value;
	else if (strcmp(VariableName, "Wind speed (x component)") == 0)
		AverageWindSpeedX[BoxNumber] = Value;
	else if (strcmp(VariableName, "Wind speed (y component)") == 0)
		AverageWindSpeedY[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

void TWind::CalculateWindSpeed()
{
	int FirstDay = 120,
		 LastDay = 260;
	if ((JulianDay > FirstDay) && (JulianDay < LastDay))
	{
		// Dummy angle of 90o
		Direction = M_PI/4;
		// Dummy speed of 1 ms-1
		Speed = 6;//6;
	}
	else
	{
		// Dummy angle of 90o
		Direction = M_PI/4;
		// Dummy speed of 6 ms-1
		Speed = 10;//10;
	}
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		AverageWindSpeed[i] = Speed; // ms-1
		AverageWindSpeedX[i] = -Speed * sin(Direction); // ms-1
		AverageWindSpeedY[i] = -Speed * cos(Direction); // ms-1
	}
}



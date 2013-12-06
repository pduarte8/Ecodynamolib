// Tide object CPP code file
// @ Pedro Duarte
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
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynClass.h"
#include "tideobjt.h"
#include "iodll.h"
#include "tide.h"

// TTide Class
//
// Constructors invoked outside EcoDyn shell...
//
TTide::TTide(char* className, int nLines, int nColumns,
             int nVars, VNA aVariablesNames[],
             int nParams, double startYear,
             double unit, double z0,
             double saAmplitude, double saPhase,
             double o1Amplitude, double o1Phase,
             double q1Amplitude, double q1Phase,
             double k1Amplitude, double k1Phase,
             double m2Amplitude, double m2Phase,
             double n2Amplitude, double n2Phase,
             double s2Amplitude, double s2Phase,
             double mN4Amplitude, double mN4Phase,
             double m4Amplitude, double m4Phase,
             double mS4Amplitude, double mS4Phase)
                        :TEcoDynClass()
{
    MyPEcoDynClass = (TEcoDynClass*)this;
#ifdef __BORLANDC__
    pMem = NULL;
#endif  // __BORLANDC__
    strcpy(EcoDynClassName, className);
    NumberOfLines = nLines;
    NumberOfColumns = nColumns;
    NumberOfBoxes = nLines * nColumns;

	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variables
	{
		BoxTidalHeight = new double[NumberOfBoxes];
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			BoxTidalHeight[i]= 0;
		}
	}
	ObjectCode = TIDEOBJECTCODE;

	// Read in the forcing functions
	NumberOfVariables = nVars;

    // Create array for state variable names
	VariableNameArray = new VNA[NumberOfVariables];

	// read in the variable names
	for (int i = 0; i < NumberOfVariables; i++)
    {
		strcpy(VariableNameArray[i], aVariablesNames[i]);
	}

	// Read in the parameters
	NumberOfParameters = nParams;

	// read in the parameter names
	StartYear = startYear;

	// Read in the tide parameters
	PTidalHeightPrediction = new TTidalHeightPrediction(unit, z0,
             saAmplitude, saPhase, o1Amplitude, o1Phase,
             q1Amplitude, q1Phase, k1Amplitude, k1Phase,
             m2Amplitude, m2Phase, n2Amplitude, n2Phase,
             s2Amplitude, s2Phase, mN4Amplitude, mN4Phase,
             m4Amplitude, m4Phase, mS4Amplitude, mS4Phase);
}

TTide::TTide(char* className, int nLines, int nColumns,
             char* variablesFilename, char* parametersFilename, char* tideFilename)
                        :TEcoDynClass()
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    strcpy(TideFileName, tideFilename);
    strcpy(EcoDynClassName, className);
#ifdef __BORLANDC__
    pMem = NULL;
#endif  // __BORLANDC__
    NumberOfLines = nLines;
    NumberOfColumns = nColumns;
    NumberOfBoxes = nLines * nColumns;
    BuildTide();
}
// ...

TTide::TTide(TEcoDynClass* PEcoDynClass, char* className) : TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = PEcoDynClass;
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
	// Get array size
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    HarmonicsTimeLag = 0.0;
    BuildTide();
}

void TTide::BuildTide()
{
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variables
	{
		BoxTidalHeight = new double[NumberOfBoxes];

		for (int i = 0; i < NumberOfBoxes; i++)
		{
			BoxTidalHeight[i]= 0;
		}
	}
	ObjectCode = TIDEOBJECTCODE;

    int X,Y;
	// Read in the forcing functions
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Tide");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Tide variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TTide::BuildTide - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Tide", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - Tide", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TTide::BuildTide - Variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Tide");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Tide parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TTide::BuildTide - Parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Tide", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];
            // read in the parameter names

            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Start Year") == 0)
                {
                    double MyVar = 0;
                    PReadWrite->ReadNumber(X+3, i, MyVar);
                    StartYear = MyVar;
                }
                else
                if (strcmp(MyParameter, "HarmonicsTimeLag") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, HarmonicsTimeLag);
                    //Debugger(HarmonicsTimeLag);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - Tide", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TTide::BuildTide - Parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the tide parameters
	PReadWrite = new TReadWrite(MyPEcoDynClass->GetTideFile());
	if (!PReadWrite->SetupFile(READFILE))
	{
		return;
	}
	PReadWrite->CloseFile();
	PTidalHeightPrediction = new TTidalHeightPrediction(MyPEcoDynClass->GetTideFile(),"Dublin");
}


TTide::~TTide()
{
	freeMemory();
}

void TTide::freeMemory()
{
    try {
        //delete PTidalHeightPrediction;
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }

        if (NumberOfBoxes > 0)
        {
            delete [] BoxTidalHeight;
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
				cerr << "TTide::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TTide::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Start Year") == 0)
    {
        value = StartYear;
    }
    else
        value = 0.0;
    return value;
}

bool TTide::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Start Year") == 0)
    {
        StartYear = value;
    }
    else
        rc = false;

    return rc;
}

void TTide::Inquiry(char* srcName, double &Value,
						int BoxNumber,
                        char* ParameterName,
                        int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Tidal height") == 0)
		Value = TidalHeight;
    else
        Value = 0;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TTide::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Tidal height") == 0)
        TidalHeight = Value;
    else
        rc = false;

    return rc;
}

void TTide::Inquiry(char* srcName, double &ATidalHeight, double AYear, double AMonth,
					 double ADay, double AnHour)
{
	double MyTidalHeight, MyYear, MyMonth, MyDay, MyHour;
        long MyTime;
	MyTidalHeight = ATidalHeight;
        MyYear = AYear;
        MyMonth = AMonth;
        MyDay = ADay;
        MyHour = AnHour;
        if (HarmonicsTimeLag != 0.0){
          MyTime = MyPEcoDynClass->GetTimeUTC() + long(HarmonicsTimeLag * HOURSTOSECONDS);
          MyYear = MyPEcoDynClass->GetAYear(MyTime);
          MyMonth = MyPEcoDynClass->GetAMonth(MyTime);
          MyDay = MyPEcoDynClass->GetADate(MyTime);
          //MyHour = GetACurrentTime(MyTime);
          MyHour = MyPEcoDynClass->GetAnHour(MyTime);
        }
	PTidalHeightPrediction->TidalHeightCalculation(MyTidalHeight,MyYear,MyMonth,MyDay,MyHour);
	ATidalHeight = MyTidalHeight;
        LogMessage("Inquiry", srcName, "Tidal height", ATidalHeight, 0);
}


void TTide::Go()
{
	double MyYear, MyMonth, MyDay, MyHour;
        long MyTime;
	// Active methods
        MyYear = MyPEcoDynClass->GetYear();
        MyMonth = MyPEcoDynClass->GetMonth();
        MyDay = MyPEcoDynClass->GetDate();
        MyHour = MyPEcoDynClass->GetCurrTime();
        //Debugger(MyYear); Debugger(MyMonth); Debugger(MyDay); Debugger(MyHour);
        //Debugger(HarmonicsTimeLag);
        if (HarmonicsTimeLag != 0.0){
          MyTime = MyPEcoDynClass->GetTimeUTC() + long(HarmonicsTimeLag * HOURSTOSECONDS);
          MyYear = MyPEcoDynClass->GetAYear(MyTime);
          MyMonth = MyPEcoDynClass->GetAMonth(MyTime);
          MyDay = MyPEcoDynClass->GetADate(MyTime);
          MyHour = MyPEcoDynClass->GetAnHour(MyTime);
          //Debugger(2222); Debugger(MyPEcoDynClass->GetTimeUTC());Debugger(MyTime);
          //Debugger(MyYear);Debugger(MyMonth);Debugger(MyDay);Debugger(MyHour); Debugger(MyPEcoDynClass->GetCurrTime());
        }
	PTidalHeightPrediction->TidalHeightCalculation(TidalHeight,MyYear,MyMonth,MyDay,MyHour);
}
//---------------------------------------------------------------------------

void TTide::SaveTide(void* fHandle)
{
    if (PTidalHeightPrediction != NULL)
        PTidalHeightPrediction->SaveHarmonics(fHandle);
    else if (PTidalHeightPrediction2 != NULL)
        PTidalHeightPrediction2->SaveHarmonics(fHandle);
}
//---------------------------------------------------------------------------

bool TTide::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Tide");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Tide");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * close file handler
     */
    CloseDataFile((void*)PReadWrite);

    return true;
}
//---------------------------------------------------------------------------

bool TTide::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Tide");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Tide");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();

    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i < (NumberOfParameters - 1))
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Start Year") == 0)
        {
           PReadWrite->WriteCell(StartYear, 0);
        }
        PReadWrite->WriteSeparator(true);
    }

    /*
     * close file handler
     */
    CloseDataFile((void*)PReadWrite);

    return true;
}
//---------------------------------------------------------------------------





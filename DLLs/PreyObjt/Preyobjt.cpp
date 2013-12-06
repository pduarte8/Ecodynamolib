/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
    #include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>

	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "preyobjt.h"
#include "iodll.h"

TPrey::TPrey(TEcoDynClass* APEcoDynClass, char* className): TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcoDynClass;
    BuildPrey(className);
}

void TPrey::BuildPrey(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    // Get array size
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    BoxArray = MyPEcoDynClass->BoxArray;
    TimeStep = MyPEcoDynClass->GetTimeStep();
    // Initialise pointers
    if (NumberOfBoxes > 0)
    {
      PreyAbundance  = new double[NumberOfBoxes];
      PreyFlux = new double[NumberOfBoxes];
    }
    else
#ifdef __BORLANDC__
      MessageBox(0,"Prey object array not dimensioned","EcoDynamo alert",
                       MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
      cerr << "TPrey::BuildPrey - prey object array not dimensioned" << endl;
#endif  // __BORLANDC__
    for (int i = 0; i < NumberOfBoxes; i++)
    {
       PreyAbundance[i] = 0.0;
       PreyFlux[i] = 0.0;
    }

    int X,Y;
    // Read in the variables
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Prey");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "Prey variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TPrey::BuildPrey - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Prey", X, Y))
        {
            char MyVariable[65];
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);

                if (strcmp(MyVariable, "Prey abundance") == 0)
                {
                   for (int j = 0; j < NumberOfBoxes; j++)
                   {
                      PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, PreyAbundance[j]);
                   }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "Variables: Undefined object - Prey", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
           cerr << "TPrey::BuildPrey - variables: undefined object Prey" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    // Read in the parameters
//    PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Prey");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "Prey parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TPrey::BuildPrey - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Prey", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // Create array for parameters names
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "IntrinsicRateOfIncrease") == 0)
                {
                   PReadWrite->ReadNumber(X+3, i, IntrinsicRateOfIncrease);
                   IntrinsicRateOfIncrease = IntrinsicRateOfIncrease / DAYSTOSECONDS;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "parameters: Undefined object - Prey", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
           cerr << "TPrey::BuildPrey - parameters: undefined object Prey" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TPrey::~TPrey()
{
    freeMemory();
}

void TPrey::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] PreyAbundance;
            delete [] PreyFlux;
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
        cerr << "TPrey::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TPrey::Go ()
{
   Grow();
}

void TPrey::Grow ()
{
   for (int i = 0; i < NumberOfBoxes; i++)
      PreyFlux[i] = PreyFlux[i] + IntrinsicRateOfIncrease * PreyAbundance[i];
}

void TPrey::Inquiry(char* srcName, double &Value, int BoxNumber,
        char* ParameterName, int AnObjectCode)
{
    if (strcmp(ParameterName, "Prey abundance") == 0)
        Value = PreyAbundance[BoxNumber];
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(ParameterName, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, ParameterName);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,Caption,"EcoDynamo Alert - Inquiry",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TPrey::Inquiry - " << ParameterName << " does not exist in "
                << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, ParameterName, Value, BoxNumber);
}



void TPrey::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    LogMessage("Update", srcName, ParameterName, Value, BoxNumber);
    if (strcmp(ParameterName, "Prey abundance") == 0)
       PreyFlux[BoxNumber] = PreyFlux[BoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
        char Caption[129];
    	strcpy(Caption, ParameterName);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
        MessageBox(0,Caption,"EcoDynamo Alert - Update",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TPrey::Update - " << ParameterName << " does not exist in "
            << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}


void TPrey::Integrate()
{
    // Integrate zooplankton biomass within the box
    // Generic = PreyAbundance; GenericFlux = PreyFlux;
    Integration(PreyAbundance, PreyFlux);
    // PreyAbundance = Generic; PreyFlux = GenericFlux;
}



double TPrey::GetParameterValue(char* MyParameter)
{
  double value = 0.0;

    if (strcmp(MyParameter, "IntrinsicRateOfIncrease") == 0)
    {
        value = IntrinsicRateOfIncrease * DAYSTOSECONDS;
    }
    return value;
}

bool TPrey::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "IntrinsicRateOfIncrease") == 0)
    {
        IntrinsicRateOfIncrease = value / DAYSTOSECONDS;
    }
    else
        rc = false;

    return rc;
}


bool TPrey::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);
    if (strcmp(VariableName, "Prey abundance") == 0)
       PreyAbundance[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

bool TPrey::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Prey");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Prey");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);

    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteSeparator();

        for (int i = 0; i < NumberOfVariables; i++)
        {
            PReadWrite->WriteSeparator();
            if (strcmp(VariableNameArray[i], "Prey abundance") == 0)
            {
                PReadWrite->WriteCell(PreyAbundance[j], 2);
            }
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TPrey::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Prey");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Prey");
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
        if (strcmp(ParametersNameArray[i], "IntrinsicRateOfIncrease") == 0)
        {
           PReadWrite->WriteCell(IntrinsicRateOfIncrease * DAYSTOSECONDS, 2);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}


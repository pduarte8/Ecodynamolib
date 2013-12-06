/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "predatorobjt.h"
#include "iodll.h"

TPredator::TPredator(TEcoDynClass* APEcoDynClass, char* className): TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcoDynClass;
    BuildPredator(className);
}

void TPredator::BuildPredator(char* className)
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
      PredatorAbundance  = new double[NumberOfBoxes];
      PredatorFlux = new double[NumberOfBoxes];
    }
    else
#ifdef __BORLANDC__
      MessageBox(0,"Predator object array not dimensioned","EcoDynamo alert",
                       MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TPredator::BuildPredator - Predator object array not dimensioned" << endl;;
#endif  // __BORLANDC__
    for (int i = 0; i < NumberOfBoxes; i++)
    {
       PredatorAbundance[i] = 0.0;
       PredatorFlux[i] = 0.0;
    }

    int X,Y;
    // Read in the variables
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Predator");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "Predator variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TPredator::BuildPredator - variables file missing" << endl;;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Predator", X, Y))
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

                if (strcmp(MyVariable, "Predator abundance") == 0)
                {
                   double MyValue;
                   for (int j = 0; j < NumberOfBoxes; j++)
                   {
                      PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, PredatorAbundance[j]);
                   }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "Variables: Undefined object - Predator", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TPredator::BuildPredator - variables: undefined object Predator" << endl;;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    // Read in the parameters
//    PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Predator");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "Predator parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TPredator::BuildPredator - parameters file missing" << endl;;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Predator", X, Y))
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

                if (strcmp(MyParameter, "FoodAbsorption") == 0)
                {
                   PReadWrite->ReadNumber(X+3, i, FoodAbsorption);
                }
                else if (strcmp(MyParameter, "FeedRate") == 0)
                {
                   PReadWrite->ReadNumber(X+3, i, FeedRate);
                   FeedRate = FeedRate / DAYSTOSECONDS;
                }
                else if (strcmp(MyParameter, "DeathRate") == 0)
                {
                   PReadWrite->ReadNumber(X+3, i, DeathRate);
                   DeathRate = DeathRate / DAYSTOSECONDS;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "parameters: Undefined object - Predator", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TPredator::BuildPredator - parameters: undefined object Predator" << endl;;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TPredator::~TPredator()
{
    freeMemory();
}

void TPredator::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] PredatorAbundance;
            delete [] PredatorFlux;
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
        cerr << "TPredator::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TPredator::Go ()
{
   Graze();
   Die();
}

void TPredator::Graze ()
{
   double MyPreyAbundance;
   TEcoDynClass* MyPreyPointer = MyPEcoDynClass->GetPreyPointer();
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      if (MyPreyPointer != NULL)
        MyPreyPointer->Inquiry(GetEcoDynClassName(), MyPreyAbundance,i,"Prey abundance",ObjectCode);
      else MyPreyAbundance = 0.0;
      PredatorFlux[i] = PredatorFlux[i] + FoodAbsorption * FeedRate * MyPreyAbundance * PredatorAbundance[i];
      if (MyPreyPointer != NULL)
        MyPreyPointer->Update(GetEcoDynClassName(), -FeedRate * MyPreyAbundance * PredatorAbundance[i],
													i,
													"Prey abundance",
													ObjectCode);
   }
}

void TPredator::Die ()
{
   for (int i = 0; i < NumberOfBoxes; i++)
      PredatorFlux[i] = PredatorFlux[i] - DeathRate * PredatorAbundance[i];
}

void TPredator::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    if (strcmp(ParameterName, "Predator abundance") == 0)
        Value = PredatorAbundance[BoxNumber];
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
        cerr << "TPredator::Inquiry - " << ParameterName << " does not exist in "
            << GetEcoDynClassName() << endl;;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, ParameterName, Value, BoxNumber);
}



void TPredator::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    LogMessage("Update", srcName, ParameterName, Value, BoxNumber);
    if (strcmp(ParameterName, "Predator abundance") == 0)
       PredatorFlux[BoxNumber] = PredatorFlux[BoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
        char Caption[129];
    	strcpy(Caption, ParameterName);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
        MessageBox(0,Caption,"EcoDynamo Alert - Update",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TPredator::Update - " << ParameterName << " does not exist in "
            << GetEcoDynClassName() << endl;;
#endif  // __BORLANDC__
    }
}


void TPredator::Integrate()
{
    // Integrate zooplankton biomass within the box
    // Generic = PredatorAbundance; GenericFlux = PredatorFlux;
    Integration(PredatorAbundance, PredatorFlux);
    // PredatorAbundance = Generic; PredatorFlux = GenericFlux;
}


double TPredator::GetParameterValue(char* MyParameter)
{
  double value = 0.0;

    if (strcmp(MyParameter, "FoodAbsorption") == 0)
    {
        value = FoodAbsorption;
    }
    else if (strcmp(MyParameter, "FeedRate") == 0)
    {
        value = FeedRate * DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "DeathRate") == 0)
    {
        value = DeathRate * DAYSTOSECONDS;
    }
    return value;
}

bool TPredator::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "FoodAbsorption") == 0)
    {
        FoodAbsorption = value;
    }
    else if (strcmp(MyParameter, "FeedRate") == 0)
    {
        FeedRate = value / DAYSTOSECONDS;
    }
    else if (strcmp(MyParameter, "DeathRate") == 0)
    {
        DeathRate = value / DAYSTOSECONDS;
    }
    else
        rc = false;

    return rc;
}


bool TPredator::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);
    if (strcmp(VariableName, "Predator abundance") == 0)
       PredatorAbundance[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

// Methods called from EcoDynamo when closing model:
bool TPredator::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Predator");
    if (PReadWrite == NULL)
    {
        return false;
    }
    /*
     * header file
     */
    PReadWrite->WriteCell("Predator");
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
            if (strcmp(VariableNameArray[i], "Predator abundance") == 0)
            {
                PReadWrite->WriteCell(PredatorAbundance[j], 2);
            }
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TPredator::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Predator");
    if (PReadWrite == NULL)
    {
        return false;
    }
    /*
     * header file
     */
    PReadWrite->WriteCell("Predator");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();
    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "FoodAbsorption") == 0)
        {
           PReadWrite->WriteCell(FoodAbsorption, 2);
        }
        else if (strcmp(ParametersNameArray[i], "FeedRate") == 0)
        {
           PReadWrite->WriteCell(FeedRate * DAYSTOSECONDS, 2);
        }
        else if (strcmp(ParametersNameArray[i], "DeathRate") == 0)
        {
           PReadWrite->WriteCell(DeathRate * DAYSTOSECONDS, 2);
        }
        /*
         * next line
         */
        PReadWrite->WriteSeparator(true);
        if (i < (NumberOfParameters - 1))
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}


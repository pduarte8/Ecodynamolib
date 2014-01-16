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

// TSPM Class

TSPMTimeSeries::TSPMTimeSeries(TEcoDynClass* APEcoDynClass, char* className)
			  : TSPM(APEcoDynClass, className)
{
	// Receive pointer to integrate
    NumberOfDaysForTimeSeries = 0.0;
    NumberOfHoursForTimeSeries = 0.0;
    NumberOfMomentsForTimeSeries = 0.0;
    AMoment = 0;
    BuildTSPM(className);
}

void TSPMTimeSeries::BuildTSPM(char* className)
{
    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
    NumberOfMomentsForTimeSeries = ceil((AFinishTime - AStartTime) / DAYSTOSECONDS) + 1;
    //Embora o n?mero de dias da s?rie seja aqui calculado, pode ser alterado se
    //for definido no ficheiro de par?metros, sendo lido nas linhas seguintes.
    //Inclusivamente, ? pss?vel exprimir os "momentos" em horas se assim forem
    //definidos no ficheiro de par?metros.
    int X,Y;
    TReadWrite*PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "SuspendedMatter parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSPMTimeSeries::BuildTSPM - SuspendedMatter parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        double MySPMRessuspension = 0;
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
                if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfDaysForTimeSeries);
                    NumberOfMomentsForTimeSeries = NumberOfDaysForTimeSeries;
                }
                else if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfHoursForTimeSeries);
                    NumberOfMomentsForTimeSeries = NumberOfHoursForTimeSeries;
                }
            }
            //Debugger(NumberOfMomentsForTimeSeries);
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - SPM", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSPMTimeSeries::BuildTSPM - Parameters: Undefined Object - SPM" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    //NumberOfDays = ceil((AFinishTime - AStartTime) / DAYSTOSECONDS) + 1;
    int XV, YV;
    ForcingTPM = new double[NumberOfMomentsForTimeSeries];
    ForcingPOM = new double[NumberOfMomentsForTimeSeries];
    ForcingDetritalC = new double[NumberOfMomentsForTimeSeries];
    ForcingDetritalN = new double[NumberOfMomentsForTimeSeries];
    ForcingDetritalP = new double[NumberOfMomentsForTimeSeries];
    Food = new double[NumberOfMomentsForTimeSeries];
    for (int i = 0; i < NumberOfMomentsForTimeSeries; i++)
    {
        ForcingTPM[i] = 0.0; ForcingPOM[i] = 0.0; ForcingDetritalC[i] = 0.0; ForcingDetritalN[i] = 0.0; ForcingDetritalP[i] = 0.0;
        Food[i] = 0.0;
    }

    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "SuspendedMatter variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSPMTimeSeries::BuildTSPM - SuspendedMatter variables file missing." << endl;
#endif  // __BORLANDC__
    }
    else {
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            // Read the water TSPM values
            char MyVariable[65];
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
                if (strcmp(MyVariable, "TPM") == 0)
                {
                    if (!PReadWrite->FindString("TPM values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ForcingTPM[j]);
                        //Debugger(ForcingTSPM[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "POM") == 0)
                {
                    if (!PReadWrite->FindString("POM values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ForcingPOM[j]);
                        //Debugger(ForcingTSPM[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "Food") == 0)
                {
                    if (!PReadWrite->FindString("Food values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Food[j]);
                        //Debugger(Food[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "DetritalC") == 0)
                {
                    if (!PReadWrite->FindString("DetritalC values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ForcingDetritalC[j]);
                        //Debugger(ForcingTSPM[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "DetritalN") == 0)
                {
                    if (!PReadWrite->FindString("DetritalN values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ForcingDetritalN[j]);
                        //Debugger(ForcingTSPM[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "DetritalP") == 0)
                {
                    if (!PReadWrite->FindString("DetritalP values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ForcingDetritalP[j]);
                        //Debugger(ForcingTSPM[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "Det energy contents") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, DetEnergeticContents);  //J/mg
                    //Debugger(DetEnergeticContents);
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TSPMTimeSeries::~TSPMTimeSeries()
{
    freeMemory();
}

void TSPMTimeSeries::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] ForcingTPM; delete [] ForcingPOM;
            delete [] ForcingDetritalC; delete [] ForcingDetritalN; delete [] ForcingDetritalP;
            delete [] Food;
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
				cerr << "TSPMTimeSeries::freeMemory- " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TSPMTimeSeries::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    if (AMoment > NumberOfMomentsForTimeSeries - 1) AMoment = NumberOfMomentsForTimeSeries - 1;
    if (strcmp(MyParameter, "TPM") == 0)
    {
        //Debugger(ADay-1);
        Value = ForcingTPM[AMoment];
    }
    else
    if (strcmp(MyParameter, "POM") == 0)
    {
        //Debugger(ADay-1);
        Value = ForcingPOM[AMoment];
    }
    else
    if (strcmp(MyParameter, "Food") == 0)
    {
        //Debugger(ADay-1);
        Value = Food[AMoment];
        //Debugger(Value);
    }
    else
    if (strcmp(MyParameter, "DetritalC") == 0)
    {
        //Debugger(ADay-1);
        Value = ForcingDetritalC[AMoment];
    }
    else
    if (strcmp(MyParameter, "DetritalN") == 0)
    {
        //Debugger(ADay-1);
        Value = ForcingDetritalN[AMoment];
    }
    else
    if (strcmp(MyParameter, "DetritalP") == 0)
    {
        //Debugger(ADay-1);
        Value = ForcingDetritalP[AMoment];
    }
    else
    if (strcmp(MyParameter, "Det energy contents") == 0)
    {
        Value = DetEnergeticContents;
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TSPMTimeSeries::SetVariableValue(char* srcName, double Value,
						 int BoxNumber,
						 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "TPM") == 0)
    {
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
            ForcingTPM[j] = Value;
    }
    else if (strcmp(VariableName, "POM") == 0)
    {
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
            ForcingPOM[j] = Value;
    }
    else if (strcmp(VariableName, "Food") == 0)
    {
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
            Food[j] = Value;
    }
    else if (strcmp(VariableName, "DetritalC") == 0)
    {
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
            ForcingDetritalC[j] = Value;
    }
    else if (strcmp(VariableName, "DetritalN") == 0)
    {
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
            ForcingDetritalN[j] = Value;
    }
    else if (strcmp(VariableName, "DetritalP") == 0)
    {
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
            ForcingDetritalP[j] = Value;
    }
    else
        rc = false;

    return rc;
}

void TSPMTimeSeries::Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode)
{
}

void TSPMTimeSeries::Go()
{

}

void TSPMTimeSeries::Integrate()
{
   int AHour;
   ATime = ATime + MyPEcoDynClass->GetTimeStep();
   ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS);
   //Debugger(ADay);
   if  (NumberOfDaysForTimeSeries >= 1)
   {
      AMoment = MIN(ADay, NumberOfMomentsForTimeSeries);
   }
   else
   if (NumberOfHoursForTimeSeries >= 1)
   {
      AHour = (int) (ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime()));
      AMoment = AHour;
      //Debugger(ATime); Debugger(ADay); Debugger(AHour);
   }
}

// AP, 2007.07.10
bool TSPMTimeSeries::SaveVariables()
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
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

	// save value of DetEnergyContents variable
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (strcmp(VariableNameArray[i], "Det energy contents") == 0)
        {
	        PReadWrite->WriteCell(DetEnergeticContents, 8);
			break;
		}
        PReadWrite->WriteSeparator();
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("TPM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Food values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalP values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
    {
        PReadWrite->WriteCell(ForcingTPM[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ForcingPOM[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Food[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ForcingDetritalC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ForcingDetritalN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ForcingDetritalP[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


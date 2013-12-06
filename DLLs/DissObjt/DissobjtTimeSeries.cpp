// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 01.04.27 14.07.2005
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"


TNutrientsForcing::TNutrientsForcing(TEcoDynClass* APEcoDynClass, char* className)
							  : TNutrients(APEcoDynClass, className)
{
        int i,j;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TNutrientsForcing::TNutrientsForcing - Nutrients variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        int X,Y,XV,YV;
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (i = 0; i < NumberOfVariables; i++)
                PReadWrite->ReadString(X+2+i, Y, VariableNameArray[i]);
            char MyVariable[65];
            for (i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2+i, Y, MyVariable);
                if (strcmp(MyVariable, "DIN") == 0)
                {
                    if (!PReadWrite->FindString("DailyDIN", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyDIN[j]);
                    }
                }
                else if (strcmp(MyVariable, "Phosphate") == 0)
                {
                    if (!PReadWrite->FindString("DailyPhosphate", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyPhosphate[j]);
                    }
                }
                else if (strcmp(MyVariable, "Oxygen") == 0)
                {
                    if (!PReadWrite->FindString("DailyOxygen", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyOxygen[j]);
                    }
                }
                else if (strcmp(MyVariable, "Nitrate") == 0)
                {
                    if (!PReadWrite->FindString("DailyNitrate", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyNitrate[j]);
                    }
                }
                else if (strcmp(MyVariable, "Ammonia") == 0)
                {
                    if (!PReadWrite->FindString("DailyAmmonia", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyAmmonia[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in nutrients", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TNutrientsForcing::TNutrientsForcing - Variables: undefined object in nutrients" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

void TNutrientsForcing::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "DIN") == 0)
        Value = DailyDIN[MyPEcoDynClass->GetJulianDay() - 1];
    else if (strcmp(MyParameter, "Phosphate") == 0)
        Value = DailyPhosphate[MyPEcoDynClass->GetJulianDay() - 1];
    else if (strcmp(MyParameter, "Oxygen") == 0)
        Value = DailyOxygen[MyPEcoDynClass->GetJulianDay() - 1];
    else if (strcmp(MyParameter, "Nitrate") == 0)
        Value = DailyNitrate[MyPEcoDynClass->GetJulianDay() - 1];
    else if (strcmp(MyParameter, "Ammonia") == 0)
        Value = DailyAmmonia[MyPEcoDynClass->GetJulianDay() - 1];
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
						cerr << "TNutrientsForcing::Inquiry 5 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TNutrientsForcing::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "DIN") == 0)
    {
        for (int j = 0; j < 365; j++)
            DailyDIN[j] = Value;
    }
    else if (strcmp(VariableName, "Phosphate") == 0)
    {
        for (int j = 0; j < 365; j++)
            DailyPhosphate[j] = Value;
    }
	else if (strcmp(VariableName, "Oxygen") == 0)
    {
        for (int j = 0; j < 365; j++)
            DailyOxygen[j] = Value;
    }
	else if (strcmp(VariableName, "Nitrate") == 0)
    {
        for (int j = 0; j < 365; j++)
            DailyNitrate[j] = Value;
    }
	else if (strcmp(VariableName, "Ammonia") == 0)
    {
        for (int j = 0; j < 365; j++)
            DailyAmmonia[j] = Value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TNutrientsForcing::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Nutrients");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Nutrients");
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
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */

    PReadWrite->WriteCell("DailyAmmonia");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DailyNitrate");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DailyPhosphate");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DailyOxygen");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DailyDIN");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(DailyAmmonia[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DailyNitrate[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DailyPhosphate[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DailyOxygen[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DailyDIN[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}



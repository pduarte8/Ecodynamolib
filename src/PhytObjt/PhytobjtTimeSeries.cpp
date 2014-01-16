// Phytoplankton object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.01.22
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>
	#include <cmath> 
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "phytobjt.h"
#include "iodll.h"

TPhytoplanktonTimeSeries::TPhytoplanktonTimeSeries(
        TEcoDynClass* APEcoDynClass, char* className)
			  : TPhytoplankton(APEcoDynClass, className)
{
	// Receive pointer to integrate
    BuildPhytoplankton(className);
}

void TPhytoplanktonTimeSeries::BuildPhytoplankton(char* className)
{
    AStartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    /*StartYear = MyPEcoDynClass->GetAYear(AStartTime);
    StartMonth = MyPEcoDynClass->GetAMonth(AStartTime);
    StartDate = MyPEcoDynClass->GetAMonth(AStartTime);
    StartJulianDay = dayOfYear(StartYear, StartMonth, StartDate);
    StartCurrentTime = MyPEcoDynClass->GetACurrentTime(AStartTime);*/
    ATime = AStartTime; 

    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
    ADay = 1;

    /*FinishYear = MyPEcoDynClass->GetAYear(AFinishTime);
    FinishMonth = MyPEcoDynClass->GetAMonth(AFinishTime);
    FinishDate = MyPEcoDynClass->GetAMonth(AFinishTime);
    FinishJulianDay = dayOfYear(FinishYear, FinishMonth, FinishDate);
    FinishCurrentTime = MyPEcoDynClass->GetACurrentTime(AFinishTime);  */

    //Debugger(AStartTime); Debugger(AFinishTime);
    //NumberOfDays = (int)((AFinishTime - AStartTime) / DAYSTOSECONDS) + 1;
    NumberOfDays = ceil((AFinishTime - AStartTime) / DAYSTOSECONDS) + 1;
    int X, Y, XV, YV;
    //Debugger(NumberOfDays);
    ForcingPhytoplankton = new double[NumberOfDays];
    ForcingChlToBiomass = new double[NumberOfDays];
    ForcingChlCarbonRatio = new double[NumberOfDays];
    PhytoEnergeticContents = 0.0;
    for (int i = 0; i < NumberOfDays; i++)
    {
        ForcingPhytoplankton[i] = 0.0;
        ForcingChlToBiomass[i] = 0.0;
        ForcingChlCarbonRatio[i] = 0.0;
    }
    double MyValue;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Phytoplankton variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TPhytoplanktonTimeSeries::BuildPhytoplankton - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // Read the water Phytoplankton values
            char MyVariable[65];
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);

                if (strcmp(MyVariable, "Phytoplankton biomass") == 0)
                {
                    if (!PReadWrite->FindString("Daily Phytoplankton biomass", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDays; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, ForcingPhytoplankton[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "Chlorophyll to biomass") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfDays; j++)
                    {
                        ForcingChlToBiomass[j] = MyValue;
                    }
                }
                else
                if (strcmp(MyVariable, "Chlorophyll : Carbon") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfDays; j++)
                    {
                        ForcingChlCarbonRatio[j] = MyValue;
                    }
                }
                else
                if (strcmp(MyVariable, "Phyto energetic contents") == 0)
                {
                  PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                  PhytoEnergeticContents = MyValue;       //J/mg
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TPhytoplanktonTimeSeries::~TPhytoplanktonTimeSeries()
{
    freeMemory();
}

void TPhytoplanktonTimeSeries::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] ForcingPhytoplankton; delete [] ForcingChlToBiomass; delete [] ForcingChlCarbonRatio;
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
				cerr << "TPhytoplanktonTimeSeries::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

bool TPhytoplanktonTimeSeries::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Phytoplankton biomass") == 0)
    {
        for (int j = 0; j < NumberOfDays; j++)
            ForcingPhytoplankton[j] = Value;
    }
    else if (strcmp(VariableName, "Chlorophyll : Carbon") == 0)
    {
        for (int j = 0; j < NumberOfDays; j++)
            ForcingChlCarbonRatio[j] = Value;
    }
    else if (strcmp(VariableName, "Chlorophyll to biomass") == 0)
    {
        for (int j = 0; j < NumberOfDays; j++)
            ForcingChlToBiomass[j] = Value;
    }
    else if (strcmp(VariableName, "Phyto energetic contents") == 0)
    {
     	PhytoEnergeticContents = Value;       //J/mg
    }
    else
        rc = false;

    return rc;
}


void TPhytoplanktonTimeSeries::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
	if (strcmp(MyParameter, "Phytoplankton biomass") == 0)
    {
        //Debugger(ADay-1);
        if (ADay > NumberOfDays - 1) ADay = NumberOfDays - 1;
        Value = ForcingPhytoplankton[ADay - 1];
    }
    else
    if (strcmp(MyParameter, "Chlorophyll to biomass") == 0)
    {
        //Debugger(ADay-1);
        if (ADay > NumberOfDays - 1) ADay = NumberOfDays - 1;
        Value = ForcingChlToBiomass[ADay - 1];
    }
    else
    if (strcmp(MyParameter, "Chlorophyll : Carbon") == 0)
    {
        if (ADay > NumberOfDays - 1) ADay = NumberOfDays - 1;
        Value = ForcingChlCarbonRatio[ADay - 1];
    }
    else
    if (strcmp(MyParameter, "Phyto energetic contents") == 0)
    {
        Value = PhytoEnergeticContents;
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TPhytoplanktonTimeSeries::Go()
{

}

void TPhytoplanktonTimeSeries::Integrate()
{
  ATime = ATime + MyPEcoDynClass->GetTimeStep();
  ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
}


// AP, 2007.07.12
bool TPhytoplanktonTimeSeries::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Phytoplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Phytoplankton");
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

    /*
     * save variables' values
     *
     * 1st line
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Chlorophyll : Carbon") == 0)
            PReadWrite->WriteCell(ForcingChlCarbonRatio[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to biomass") == 0)
            PReadWrite->WriteCell(ForcingChlToBiomass[0], 8);

        else if ((i == 0) && (strcmp(VariableNameArray[i], "Phyto energetic contents") == 0))
          	PReadWrite->WriteCell(PhytoEnergeticContents, 8);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values
     */
    PReadWrite->WriteCell("Daily Phytoplankton biomass");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfDays; j++)
    {
        PReadWrite->WriteCell(ForcingPhytoplankton[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


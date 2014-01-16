// Flow object CPP code file
// @  Pedro Duarte
// Rev. A 2001.01.23
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "flowobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"

// TOneDimensionalFlow Class
//
// Constructors invoked outside EcoDyn shell...
//
TCrestumaLeverFlow::TCrestumaLeverFlow(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow,
                   int numberOfDays, int firstDay,
                   double aDailyRiverFlow[], double aDailyDamDischarge[],
                   double aHourlyRiverFlow[], double aHourlyDamDischarge[])
        :TOneDimensionalFlow(className, timeStep, nLines, nColumns, aDepth,
                      aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                      nVars, aVarsNames, modalFlow, flowAmplitude, randomFlow)
{
    NumberOfDays = numberOfDays;
    FirstDay = firstDay;

		int daysHours = NumberOfDays * DAYSTOHOURS;
    DailyRiverFlow = new double[NumberOfDays];
    DailyDamDischarge = new double[NumberOfDays];
    HourlyRiverFlow = new double[daysHours];
    HourlyDamDischarge = new double[daysHours];

    for (int j = 0; j < NumberOfDays; j++)
    {
		DailyRiverFlow[j] = aDailyRiverFlow[j];
        DailyDamDischarge[j] = aDailyDamDischarge[j];
    }
    for (int j = 0; j < NumberOfDays * DAYSTOHOURS; j++)
    {
		HourlyRiverFlow[j] = aHourlyRiverFlow[j];
        HourlyDamDischarge[j] = aHourlyDamDischarge[j];
    }
}

TCrestumaLeverFlow::TCrestumaLeverFlow(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename)
        :TOneDimensionalFlow(className, timeStep, morphologyFilename, variablesFilename)
{
    BuildCrestumaLeverFlow();
}
// ...

TCrestumaLeverFlow::TCrestumaLeverFlow(TEcoDynClass* PEcoDynClass,
        char* className) : TOneDimensionalFlow(PEcoDynClass, className)
{
    BuildCrestumaLeverFlow();
}

void TCrestumaLeverFlow::BuildCrestumaLeverFlow()
{
	// Read in the variables

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Flow");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Flow variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrestumaLeverFlow::BuildCrestumaLeverFlow - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        int X,Y,XM,YM,XD,YD;
        if (PReadWrite->FindString("River Flow", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            VariableNameArray = new VNA[NumberOfVariables];
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X + 2, i, VariableNameArray[i-Y]);

            if (!PReadWrite->FindString("Main flow", XM, YM))
            {
                XM = X + 2;
                YM = Y;
            }
            PReadWrite->ReadNumber(XM+1, YM, NumberOfDays);
            if (!PReadWrite->FindString("FirstDay", XD, YD))
            {
                XD = X + 2;
                YD = Y + 1;
            }
            PReadWrite->ReadNumber(XD+1, YD, FirstDay);

						int daysHours = NumberOfDays * DAYSTOHOURS;
            DailyRiverFlow = new double[NumberOfDays];
            DailyDamDischarge = new double[NumberOfDays];
            HourlyRiverFlow = new double[daysHours];
            HourlyDamDischarge = new double[daysHours];

            if (!PReadWrite->FindString("Main flow values", XM, YM))
            {
                XM = X + 4;
                YM = Y - 1;
            }
            if (!PReadWrite->FindString("Dam discharge values", XD, YD))
            {
                XD = X + 5;
                YD = Y - 1;
            }
            for (int j = 0; j < NumberOfDays; j++)
            {
                PReadWrite->ReadNumber(XM, YM + 1 + j, DailyRiverFlow[j]);
                PReadWrite->ReadNumber(XD, YD + 1 + j, DailyDamDischarge[j]);
            }
            if (!PReadWrite->FindString("Hourly river flow values", XM, YM))
            {
                XM = X + 6;
                YM = Y - 1;
            }
            if (!PReadWrite->FindString("Hourly river flow values", XD, YD))
            {
                XD = X + 7;
                YD = Y - 1;
            }
            for (int j = 0; j < NumberOfDays * DAYSTOHOURS; j++)
            {
                PReadWrite->ReadNumber(XM, YM + 1 + j, HourlyRiverFlow[j]);
                PReadWrite->ReadNumber(XD, YD + 1 + j, HourlyDamDischarge[j]);
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
    HourlyMainRiverFlow = 0.0;
    HourlyDischarge = 0.0;
}

TCrestumaLeverFlow::~TCrestumaLeverFlow()
{
    freeMemory();
}

void TCrestumaLeverFlow::freeMemory()
{
    try {
        if (NumberOfDays > 0) {
            delete [] DailyRiverFlow;
            delete [] DailyDamDischarge;
            delete [] HourlyRiverFlow;
            delete [] HourlyDamDischarge;
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
				cerr << "TCrestumaLeverFlow::freeMemory - " << exc.what() << endl;
		}
#endif   // __BORLANDC__
}


bool TCrestumaLeverFlow::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Flow");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("River Flow");
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
        if (strcmp(VariableNameArray[i], "Main flow") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(NumberOfDays, 0);
        }
        else if (strcmp(VariableNameArray[i], "FirstDay") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(FirstDay, 0);
        }
        PReadWrite->WriteSeparator(true);
    }
    /*
     * save variables' values
     */
    if (NumberOfDays > 0)
    {
        PReadWrite->WriteCell("Main flow values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Dam discharge values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Hourly river flow values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Hourly dam discharge values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfDays * DAYSTOHOURS; j++)
        {
            if (j < NumberOfDays)
            {
                PReadWrite->WriteCell(DailyRiverFlow[j], 8);
                PReadWrite->WriteSeparator();
                PReadWrite->WriteCell(DailyDamDischarge[j], 8);
                PReadWrite->WriteSeparator();
            }
            else
            {
                PReadWrite->WriteSeparator();
                PReadWrite->WriteSeparator();
            }

            PReadWrite->WriteCell(HourlyRiverFlow[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(HourlyDamDischarge[j], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

void TCrestumaLeverFlow::Inquiry(char* srcName, double &Value,
                 int BoxNumber,
                 char* ParameterName,
                 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    for (int i=0; i < NumberOfVariables; i++)
    {
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{
            Value = MainRiverFlow;
            if (strcmp(MyParameter, "Dam discharge") == 0)
				Value = DamDischarge;
            else if (strcmp(MyParameter, "Hourly river flow") == 0)
				Value = HourlyMainRiverFlow;
            else if (strcmp(MyParameter, "Hourly dam discharge") == 0)
				Value = HourlyDischarge;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
			return;
		}
    }
	Value = 0;
}



void TCrestumaLeverFlow::CalculateFlow()
{
	int MyDay, MyHour;
    double MyTime;
    MyDay = MyPEcoDynClass->GetJulianDay() - 1;
    MyTime = MyPEcoDynClass->GetCurrTime();
    MyTime = (MyDay - FirstDay) * DAYSTOHOURS + MyTime;
    MyHour = floor(MyTime);
    if ((MyDay >= FirstDay) && (MyDay < FirstDay + NumberOfDays))
    {
		MainRiverFlow = DailyRiverFlow[MyDay - FirstDay];
        DamDischarge = DailyDamDischarge[MyDay - FirstDay];
        if (MyDay < FirstDay + NumberOfDays - 1)
        {
      	    HourlyMainRiverFlow = HourlyRiverFlow[MyHour] + (MyTime - MyHour) *
                              (HourlyRiverFlow[MyHour + 1] - HourlyRiverFlow[MyHour]) /
                              (1.0);
            HourlyDischarge = HourlyDamDischarge[MyHour] + (MyTime - MyHour) *
                          (HourlyDamDischarge[MyHour + 1] - HourlyDamDischarge[MyHour]) /
                          (1.0);
        }
        else
        {
            HourlyMainRiverFlow = HourlyRiverFlow[MyHour];
      	    HourlyDischarge = HourlyDamDischarge[MyHour];
        }
    }
    else
    {
   	    MainRiverFlow = 0.0;
        DamDischarge = 0.0;
        HourlyMainRiverFlow = 0.0;
        HourlyDischarge = 0.0;
    }
}

// AP, 2007.05.28
bool TCrestumaLeverFlow::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Dam discharge") == 0)
        DamDischarge = Value;
    else if (strcmp(VariableName, "Hourly river flow") == 0)
        HourlyMainRiverFlow = Value;
    else if (strcmp(VariableName, "Hourly dam discharge") == 0)
        HourlyDischarge = Value;
    else
        rc = false;

    return rc;
}



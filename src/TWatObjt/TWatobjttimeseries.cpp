// Water temperature object CPP code file
// @ Pedro Duarte
// Rev. A 30.01.2004
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <ctime>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"


TWaterTemperatureTimeSeries::TWaterTemperatureTimeSeries(
        TEcoDynClass* APEcoDynClass, char* className)
			  : TWaterTemperature(APEcoDynClass, className)
{
	// Receive pointer to integrate
    BuildWaterTemperature(className);
}

void TWaterTemperatureTimeSeries::BuildWaterTemperature(char* className)
{
    AStartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    StartYear = MyPEcoDynClass->GetAYear(AStartTime);
    StartMonth = MyPEcoDynClass->GetAMonth(AStartTime);
    StartDate = MyPEcoDynClass->GetAMonth(AStartTime);
    StartJulianDay = dayOfYear(StartYear, StartMonth, StartDate);
    StartCurrentTime = MyPEcoDynClass->GetACurrentTime(AStartTime);
    ATime = AStartTime;
    //Debugger(ATime);
    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
    ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
    //ADay = 1;
    /*FinishYear = MyPEcoDynClass->GetAYear(AFinishTime);
    FinishMonth = MyPEcoDynClass->GetAMonth(AFinishTime);
    FinishDate = MyPEcoDynClass->GetAMonth(AFinishTime);
    FinishJulianDay = dayOfYear(FinishYear, FinishMonth, FinishDate);
    FinishCurrentTime = MyPEcoDynClass->GetACurrentTime(AFinishTime);  */

    //Debugger(AStartTime); Debugger(AFinishTime);
    //NumberOfDays = (int)((AFinishTime - AStartTime) / DAYSTOSECONDS) + 1;
    NumberOfDays = ceil((AFinishTime - AStartTime) / DAYSTOSECONDS) + 1;
    int X, Y, XW,YW;
    
    ForcingTemperature = new double[NumberOfDays];
    for (int i = 0; i < NumberOfDays; i++)
        ForcingTemperature[i] = 0.0;

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("WaterTemperature");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "WaterTemperature variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TWaterTemperatureTimeSeries::BuildWaterTemperature - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Water temperature", X, Y))
        {
            /*
             * variables are read in TWaterTemperature in a different order
             *  ==> must be read again
             */
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
                PReadWrite->ReadString(X+2+i, Y, VariableNameArray[i]);

            // Read the water temperature values
            char MyVariable[65];
            
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                if (strcmp(MyVariable, "Water temperature") == 0)
                {
                    if (!PReadWrite->FindString("Water temperature values", XW, YW))
                    {
                        XW = X + 2 + i;
                        YW = Y;
                    }
                    for (int j = 0; j < NumberOfDays; j++)
                    {
                        PReadWrite->ReadNumber(XW, YW + 1 + j, ForcingTemperature[j]);
                        //Debugger(ForcingTemperature[j]);
                    }
                }
            }
            //Debugger(ForcingTemperature[NumberOfDays-1]);
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TWaterTemperatureTimeSeries::~TWaterTemperatureTimeSeries()
{
	freeMemory();
}

void TWaterTemperatureTimeSeries::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] ForcingTemperature;
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
				cerr << "TWaterTemperatureTimeSeries::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TWaterTemperatureTimeSeries::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Water temperature") == 0)
    {
        //Debugger(ADay-1);
        if (ADay > NumberOfDays - 1) ADay = NumberOfDays - 1;
        Value = ForcingTemperature[ADay - 1];
    }
    else
        Value = 0;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TWaterTemperatureTimeSeries::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Water temperature") == 0)
        for (int i = 0; i < NumberOfDays; i++)
            ForcingTemperature[i] = Value;
    else
        rc = false;

    return rc;
}

bool TWaterTemperatureTimeSeries::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("WaterTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Water temperature");
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
        if (strcmp(VariableNameArray[i], "Relative humidity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RelativeHumidity, 8);
        }
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Water temperature values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfDays; j++)
    {
        PReadWrite->WriteCell(ForcingTemperature[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);
    return true;
}

void TWaterTemperatureTimeSeries::Go()
{

}

void TWaterTemperatureTimeSeries::Integrate()
{
   ATime = ATime + MyPEcoDynClass->GetTimeStep();
   ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
}







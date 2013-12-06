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

// TAirTemperatureTimeSeries Class
//
// Constructors invoked outside EcoDyn shell...
//


TAirTemperatureTimeSeries::TAirTemperatureTimeSeries(TEcoDynClass* APEcodynClass, char* className)
         :TAirTemperature(APEcodynClass, className)
{
    MyPEcoDynClass = APEcodynClass;
    AirTemperatureInBox = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++){
       AirTemperatureInBox[i] = 0.0;
    }
    NumberOfSubDomains = 1;
    RecycleTimeSeries = 1;
    MyDay = 0;
    BuilAirTemperatureTimeSeries();
}

void TAirTemperatureTimeSeries::BuilAirTemperatureTimeSeries()
{
    int X,Y, XV, YV;
    NumberOfMomentsForAirTemperatureTimeSeries = 1;
    NumberOfDaysForAirTemperatureTimeSeries = 0;
    NumberOfHoursForAirTemperatureTimeSeries = 0;
    //  TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("AirTemperature");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
          MessageBox(0, "AirTemperature parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TAirTemperatureTimeSeries::BuildAirTemperatureTimeSeries - parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        if (PReadWrite->FindString("Air temperature", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
                if (strcmp(MyParameter, "NumberOfDaysForAirTemperatureTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfDaysForAirTemperatureTimeSeries);
                    NumberOfMomentsForAirTemperatureTimeSeries = NumberOfDaysForAirTemperatureTimeSeries;
                    //Debugger(NumberOfDaysForAirTemperatureTimeSeries);
                }
                else
                if (strcmp(MyParameter, "NumberOfHoursForAirTemperatureTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfHoursForAirTemperatureTimeSeries);
                    NumberOfMomentsForAirTemperatureTimeSeries = NumberOfHoursForAirTemperatureTimeSeries;
                    //Debugger(NumberOfHoursForAirTemperatureTimeSeries);
                }
                else
                if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfSubDomains); //Debugger(NumberOfSubDomains);
                }
                else
                if (strcmp(MyParameter, "RecycleTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RecycleTimeSeries); Debugger(RecycleTimeSeries);
                }
            }
            MyAirTemperatureSubDomains = new SubDomain[NumberOfSubDomains];
            for (int j = 0; j < NumberOfSubDomains; j++)
            {
                MyAirTemperatureSubDomains[j].FirstLine = 0;                                         //Southern limit
                MyAirTemperatureSubDomains[j].LastLine = MyPEcoDynClass->GetNumberOfLines() - 1;     //Northern limit
                MyAirTemperatureSubDomains[j].FirstColumn = 0;                                       //Western limit
                MyAirTemperatureSubDomains[j].LastColumn = MyPEcoDynClass->GetNumberOfColumns() - 1; //Eastern limit
            }
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
                if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                {
                    int AValue, index = 0;
                    for (int j = i; j < i + NumberOfSubDomains; j++)
                    {
                       //Debugger(j);
                       PReadWrite->ReadNumber(X+4, j, AValue);
                       MyAirTemperatureSubDomains[index].FirstLine = NumberOfLines - AValue; //Debugger(AValue);//Southern limit
                       PReadWrite->ReadNumber(X+5, j, AValue);
                       MyAirTemperatureSubDomains[index].LastLine = NumberOfLines - AValue; //Debugger(AValue); //Northern limit
                       PReadWrite->ReadNumber(X+6, j, AValue);
                       MyAirTemperatureSubDomains[index].FirstColumn = AValue - 1;          //Debugger(AValue); //Western limit
                       PReadWrite->ReadNumber(X+7, j, AValue);
                       MyAirTemperatureSubDomains[index].LastColumn = AValue - 1;           //Debugger(AValue); //Eastern limit
                       //Debugger(index);
                       //Debugger(MyAirTemperatureSubDomains[index].FirstLine); Debugger(MyAirTemperatureSubDomains[index].LastLine);
                       //Debugger(MyAirTemperatureSubDomains[index].FirstColumn); Debugger(MyAirTemperatureSubDomains[index].LastColumn);
                       index++;
                    }
                }
            }

        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object Air temperature Parameters ", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TAirTemperatureTimeSeries::BuildAirTemperatureTimeSeries - parameters undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    MyAirTemperatureInSubDomains = new double[NumberOfSubDomains*NumberOfMomentsForAirTemperatureTimeSeries];
    int index;
    for (int i = 0; i < NumberOfSubDomains; i++)
    {
       for (int j = 0; j < NumberOfMomentsForAirTemperatureTimeSeries; j++)
       {
          index = i * NumberOfMomentsForAirTemperatureTimeSeries + j;
          MyAirTemperatureInSubDomains[index] = 0.0;
       }
    }

    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("AirTemperature");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Air temperature", X, Y))
        {
           char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2, i, MyVariable);
                if (strcmp(MyVariable, "Air temperature") == 0)
                {
                    if (!PReadWrite->FindString("Air temperature time series values", XV, YV)) {
                        XV = X + 3;
                        YV = i - 1;
                    }
                    for (int i = 0; i < NumberOfSubDomains; i++)
                    {
                        for (int j = 0; j < NumberOfMomentsForAirTemperatureTimeSeries; j++)
                        {
                            index = i * NumberOfMomentsForAirTemperatureTimeSeries + j;
                            PReadWrite->ReadNumber(XV + i, YV + 1 + j, MyAirTemperatureInSubDomains[index]);
                            //if ((index == 0) || ((i == NumberOfSubDomains-1) && (j == NumberOfMomentsForAirTemperatureTimeSeries-1)))
                            //   Debugger(MyAirTemperatureInSubDomains[index]);
                        }
                    }
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
    GetAirTemperature();
}

TAirTemperatureTimeSeries::~TAirTemperatureTimeSeries()
{
	freeMemory();
}

void TAirTemperatureTimeSeries::freeMemory()
{
    try {
      delete [] MyAirTemperatureInSubDomains;
      delete [] AirTemperatureInBox;
      if (NumberOfParameters > 0) {
          delete [] ParametersNameArray;
          delete [] MyAirTemperatureSubDomains;
          NumberOfParameters = 0;
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
				cerr << "TAirTemperatureTimeSeries::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TAirTemperatureTimeSeries::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name

    int MyBoxNumber = BoxNumber, index;
    strcpy(MyParameter, ParameterName);
    index = MyPEcoDynClass->GetLineIndex3D(MyBoxNumber) * NumberOfColumns + GetColumnIndex3D(MyBoxNumber);
    if (strcmp(MyParameter, "Air temperature") == 0)
    Value = AirTemperatureInBox[index];
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
						cerr << "TAirTemperatureTimeSeries::inquiry - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

int TAirTemperatureTimeSeries::CalculateMoment()
{
    //Debugger(MyPEcoDynClass->GetTime());
    int ADay, AHour, AMoment;
    AMoment = 0.0;
    if  (NumberOfDaysForAirTemperatureTimeSeries >= 1)
    {
       ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForAirTemperatureTimeSeries - 1);
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForAirTemperatureTimeSeries >= 1)
    {
       ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForAirTemperatureTimeSeries - 1);
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "TAirTemperatureTimeSeries::GetAirTemperature", "Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries", MB_OK);
#else  // __BORLANDC__
				cerr << "TAirTemperatureTimeSeries::GetAirTemperature - Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries" << endl;
#endif  // __BORLANDC__
    return AMoment;
    /*if  (NumberOfDaysForAirTemperatureTimeSeries >= 1)
    {
       if ((RecycleTimeSeries == 0) && (MyDay > 0)){
          MyDay = MyDay + floor(MyPEcoDynClass->GetTime());
          ADay = MyDay;
       }
       else
       {
          ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForAirTemperatureTimeSeries - 1);
          MyDay++;
       }
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForAirTemperatureTimeSeries >= 1)
    {
       if ((RecycleTimeSeries == 0) && (MyDay > 0)){
          MyDay = MyDay + 1;
          ADay = MyDay;
       }
       else
       {
          ADay = MyPEcoDynClass->GetJulianDay() - 1;
          MyDay++;
       }
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForAirTemperatureTimeSeries - 1);
    }
    else
       MessageBox(0, "TAirTemperatureTimeSeries::GetAirTemperature", "Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries", MB_OK);  */
}

void TAirTemperatureTimeSeries::GetAirTemperature()
{
    int /*ADay, AHour, */AMoment, index1, index2, InitialLine, FinalLine, InitialColumn, FinalColumn;

    /*if  (NumberOfDaysForAirTemperatureTimeSeries >= 1)
    {
       if ((!RecycleTimeSeries) && (MyDay > 0)){
          MyDay = MyDay + 1;
          ADay = MyDay;
       }
       else
          ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForAirTemperatureTimeSeries - 1);
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForAirTemperatureTimeSeries >= 1)
    {
       if ((!RecycleTimeSeries) && (MyDay > 0)){
          MyDay = MyDay + 1;
          ADay = MyDay;
       }
       else
          ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForAirTemperatureTimeSeries - 1);
    }
    else
       MessageBox(0, "TAirTemperatureTimeSeries::GetAirTemperature", "Incorrect value in NumberOfDaysForTimeSeries or NumberOfHoursForTimeSeries", MB_OK);
    */
    AMoment = CalculateMoment();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    if (pSubDomain)
    {
       InitialLine = pSubDomain->FirstLine;
       FinalLine = pSubDomain->LastLine;
    }
    else
    {
       InitialLine = 0;
       FinalLine = NumberOfLines -1;
       InitialColumn = 0;
       FinalColumn = NumberOfColumns -1;
    }
    //for (int i = 0; i < NumberOfLines; i++)
    for (int i = InitialLine; i <= FinalLine; i++)
    {
       if (pSubDomain)
       {
          InitialColumn = pSubDomain->IColumn[i];
          FinalColumn = MIN(pSubDomain->FColumn[i],MyPEcoDynClass->GetNumberOfColumns()-1);
       }
       //for (int j = 0; j < NumberOfColumns; j++)
       for (int j = InitialColumn; j <= FinalColumn; j++)
       {
          index1 = i * NumberOfColumns + j;
          AirTemperatureInBox[index1] = MyAirTemperatureInSubDomains[AMoment]; //Por defeito assume que a temperatura na c?lula do modelo = temperatura do primeiro subdom?nio
          for (int k = 0; k < NumberOfSubDomains; k++)
          {
              index2 = k * NumberOfMomentsForAirTemperatureTimeSeries + AMoment;
              if (
                   (i >= MyAirTemperatureSubDomains[k].FirstLine) && (i <= MyAirTemperatureSubDomains[k].LastLine) &&
                   (j >= MyAirTemperatureSubDomains[k].FirstColumn) && (j <= MyAirTemperatureSubDomains[k].LastColumn)
                 )
              {
                 AirTemperatureInBox[index1] = MyAirTemperatureInSubDomains[index2];
              }
          }
       }
    }
}



// AP, 2008.01.16...
bool TAirTemperatureTimeSeries::SaveVariables()
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
    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Air temperature time series values");
    PReadWrite->WriteSeparator(true);
    int index;
    for (int j = 0; j < NumberOfMomentsForAirTemperatureTimeSeries; j++)
    {
        for (int k = 0; k < NumberOfSubDomains; k++)
        {
           index = k * NumberOfMomentsForAirTemperatureTimeSeries + j;
           PReadWrite->WriteCell(AirTemperatureInBox[index], 8);
           PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

double TAirTemperatureTimeSeries::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "NumberOfDaysForAirTemperatureTimeSeries") == 0)
    {
        value = NumberOfDaysForAirTemperatureTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForAirTemperatureTimeSeries") == 0)
    {
        value = NumberOfHoursForAirTemperatureTimeSeries;
    }
    else
        value = 0.0;
    return value;
}

bool TAirTemperatureTimeSeries::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "NumberOfDaysForAirTemperatureTimeSeries") == 0)
    {
        NumberOfDaysForAirTemperatureTimeSeries = value;
        NumberOfMomentsForAirTemperatureTimeSeries = NumberOfDaysForAirTemperatureTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForAirTemperatureTimeSeries") == 0)
    {
        NumberOfHoursForAirTemperatureTimeSeries = value;
        NumberOfMomentsForAirTemperatureTimeSeries = NumberOfHoursForAirTemperatureTimeSeries;
    }
    else
        rc = false;

    return rc;
}


bool TAirTemperatureTimeSeries::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("AirTemperature");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Air temperature");
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
        if (strcmp(ParametersNameArray[i], "NumberOfDaysForAirTemperatureTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForAirTemperatureTimeSeries, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfHoursForAirTemperatureTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfHoursForAirTemperatureTimeSeries, 0);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}


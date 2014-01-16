// Wind object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09

/* Este objecto l? s?ries temporais de valores di?rios ou hor?rios de velocidade (m/s) e direc??o do vento (radianos) a partir
de um ficheiro de texto, para usar como fun??o for?adora de outros objectos. O primeiro valor do ficheiro deve corresponder ao
primeiro dia ou ? primeira hora do ano simulado. O ?ltimo valor deve corresponder ao ?ltimo dia ou ? ?ltima hora do ano simulado.
Quando o tempo simulado excede o per?odo correspondente ? s?rie temporal, este objecto devolve o ?ltimo valor da s?rie (por exemplo:
quando a s?rie temporal compreende valores correspondentes a um per?odo inferior a um ano e o per?odo simulado for de um ano). Se o per?odo
simulado for de mais de um ano, o objecto devolve sempre os mesmos valores em dias/horas iguais de anos diferentes, ou seja,
"recicla" a s?rie temporal.*/

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

// TWindDailyTimeSeries Class
//
// Constructors invoked outside EcoDyn shell...
//


TWindTimeSeries::TWindTimeSeries(TEcoDynClass* APEcodynClass, char* className)
         :TWindDailyTimeSeries(APEcodynClass, className)
{
    MyPEcoDynClass = APEcodynClass;
    NumberOfSubDomains = 1;
    BuildWindTimeSeries();
}

void TWindTimeSeries::BuildWindTimeSeries()
{
    int X,Y, XV, YV;
    NumberOfMomentsForWindTimeSeries = 1;
    NumberOfDaysForWindTimeSeries = 0;
    NumberOfHoursForWindTimeSeries = 0;
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Wind");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
       MessageBox(0, "Wind parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TWindTimeSeries::BuildWindTimeSeries - parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        if (PReadWrite->FindString("Wind", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "NumberOfDaysForWindTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfDaysForWindTimeSeries);
                    NumberOfMomentsForWindTimeSeries = NumberOfDaysForWindTimeSeries;
                    //Debugger(NumberOfDaysForWindTimeSeries);
                }
                else
                if (strcmp(MyParameter, "NumberOfHoursForWindTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfHoursForWindTimeSeries);
                    NumberOfMomentsForWindTimeSeries = NumberOfHoursForWindTimeSeries;
                    //Debugger(NumberOfHoursForWindTimeSeries);
                }
                else
                if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfSubDomains); //Debugger(NumberOfSubDomains);
                }
            }
            MyWindSubDomains = new SubDomain[NumberOfSubDomains];
            for (int j = 0; j < NumberOfSubDomains; j++)
            {
                MyWindSubDomains[j].FirstLine = 0;                                         //Southern limit
                MyWindSubDomains[j].LastLine = MyPEcoDynClass->GetNumberOfLines() - 1;     //Northern limit
                MyWindSubDomains[j].FirstColumn = 0;                                       //Western limit
                MyWindSubDomains[j].LastColumn = MyPEcoDynClass->GetNumberOfColumns() - 1; //Eastern limit
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
                       MyWindSubDomains[index].FirstLine = NumberOfLines - AValue; //Debugger(AValue);//Southern limit
                       PReadWrite->ReadNumber(X+5, j, AValue);
                       MyWindSubDomains[index].LastLine = NumberOfLines - AValue; //Debugger(AValue); //Northern limit
                       PReadWrite->ReadNumber(X+6, j, AValue);
                       MyWindSubDomains[index].FirstColumn = AValue - 1;          //Debugger(AValue); //Western limit
                       PReadWrite->ReadNumber(X+7, j, AValue);
                       MyWindSubDomains[index].LastColumn = AValue - 1;           //Debugger(AValue); //Eastern limit
                       //Debugger(index);
                       //Debugger(MyWindSubDomains[index].FirstLine); Debugger(MyWindSubDomains[index].LastLine);
                       //Debugger(MyWindSubDomains[index].FirstColumn); Debugger(MyWindSubDomains[index].LastColumn);
                       index++;
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object Wind in Parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWindTimeSeries::BuildWindTimeSeries - Undefined object Wind in parameters file" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    WindSpeed = new double[NumberOfSubDomains*NumberOfMomentsForWindTimeSeries];
    WindSpeedX = new double[NumberOfSubDomains*NumberOfMomentsForWindTimeSeries];
    WindSpeedY = new double[NumberOfSubDomains*NumberOfMomentsForWindTimeSeries];
    WindDirection = new double[NumberOfSubDomains*NumberOfMomentsForWindTimeSeries];
    int index;
    for (int i = 0; i < NumberOfSubDomains; i++)
    {
       for (int j = 0; j < NumberOfMomentsForWindTimeSeries; j++)
       {
          index = i * NumberOfMomentsForWindTimeSeries + j;
          WindSpeed[index] = 0.0; WindSpeedX[index] = 0.0;
          WindSpeedY[index] = 0.0; WindDirection[index] = 0.0;
       }
    }
    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Wind");
    if (PReadWrite != NULL)
    {
      if (PReadWrite->FindString("Wind", X, Y))
      {
          char MyVariable[65];
          for (int i = Y; i < Y + NumberOfVariables; i++)
          {
              PReadWrite->ReadString(X + 2, i, MyVariable);
              if (strcmp(MyVariable, "Wind speed") == 0)
              {
                  if (!PReadWrite->FindString("Wind speed values", XV, YV)) {
                      XV = X + 3;
                      YV = i - 1;
                  }
                  for (int i = 0; i < NumberOfSubDomains; i++)
                  {
                      for (int j = 0; j < NumberOfMomentsForWindTimeSeries; j++)
                      {
                          index = i * NumberOfMomentsForWindTimeSeries + j;
                          PReadWrite->ReadNumber(XV + i, YV + 1 + j, WindSpeed[index]);
                          //if ((index == 0) || ((i == NumberOfSubDomains-1) && (j == NumberOfMomentsForWindTimeSeries-1)))
                          //   Debugger(WindSpeed[index]);
                      }
                  }
              }
              else
              if (strcmp(MyVariable, "Wind Direction") == 0)
              {
                  //Debugger(NumberOfSubDomains);
                  if (!PReadWrite->FindString("Wind Direction values", XV, YV)) {
                      XV = X + 3 + NumberOfSubDomains;
                      YV = i - 1;
                  }
                  for (int i = 0; i < NumberOfSubDomains; i++)
                  {
                      for (int j = 0; j < NumberOfMomentsForWindTimeSeries; j++)
                      {
                          index = i * NumberOfMomentsForWindTimeSeries + j;
                          PReadWrite->ReadNumber(XV + i, YV + 1 + j, WindDirection[index]);
                          //if ((index == 0) || ((i == NumberOfSubDomains-1) && (j == NumberOfMomentsForWindTimeSeries-1)))
                          //   Debugger(WindDirection[index]);
                      }
                  }
              }
          }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object Wind in variables file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TWindTimeSeries::BuildWindTimeSeries - Undefined object Wind in variables file" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    CalculateWindSpeed();
}

TWindTimeSeries::~TWindTimeSeries()
{
	freeMemory();
}

void TWindTimeSeries::freeMemory()
{
    try {
        delete [] WindSpeed;
        delete [] WindSpeedX;
        delete [] WindSpeedY;
        delete [] WindDirection;
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            delete [] MyWindSubDomains;
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
				cerr << "TWindTimeSeries::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

int TWindTimeSeries::CalculateMoment()
{
    int ADay, AHour, AMoment;
    AMoment = 0.0;
    if  (NumberOfDaysForWindTimeSeries >= 1)
    {
       ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForWindTimeSeries - 1);
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForWindTimeSeries >= 1)
    {
       ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForWindTimeSeries - 1);
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "TWindTimeSeries::CalculateWindSpeed", "Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForWindTimeSeries", MB_OK);
#else  // __BORLANDC__
			 cerr << "TWindTimeSeries::CalculateWindSpeed - Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForWindTimeSeries" << endl;
#endif  // __BORLANDC__
    return AMoment;
}


void TWindTimeSeries::CalculateWindSpeed()
{
    int index, AMoment, GridColumns, UpperLayer, index2, InitialLine, FinalLine, InitialColumn, FinalColumn;
    UpperLayer = MyPEcoDynClass->GetNumberOfLayers() - 1;
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
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    AMoment = CalculateMoment();
    //Debugger(NumberOfLayers);
    if ((UpperLayer > 0) && (MyTransportPointer != NULL))
    {
       double AValue;
       MyTransportPointer->Inquiry(GetEcoDynClassName(),AValue,0,"UpperLayer",ObjectCode);
       UpperLayer = ceil(AValue);
    }
    else
       UpperLayer = NumberOfLayers - 1;
    //Debugger(InitialLine); Debugger(FinalLine);
    //for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    for (int i = InitialLine; i <= FinalLine; i++)
    {
       //for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],MyPEcoDynClass->GetNumberOfColumns()-1); j++)
       if (pSubDomain)
       {
          InitialColumn = pSubDomain->IColumn[i];
          FinalColumn = MIN(pSubDomain->FColumn[i],MyPEcoDynClass->GetNumberOfColumns()-1);
       }
       for (int j = InitialColumn; j <= FinalColumn; j++)
       {
          index = Get3DIndex(i,j,UpperLayer);
          AverageWindSpeed[index] = WindSpeed[AMoment]; // ms-1
          AverageWindSpeedX[index] = -WindSpeed[AMoment] * sin(WindDirection[AMoment]); // ms-1
          AverageWindSpeedY[index] = -WindSpeed[AMoment] * cos(WindDirection[AMoment]); // ms-1
          //Por defeito assume que o vento na c?lula do modelo = vento do primeiro subdom?nio
          for (int k = 0; k < NumberOfSubDomains; k++)
          {
              index2 = k * NumberOfMomentsForWindTimeSeries + AMoment;
              if (
                   (i >= MyWindSubDomains[k].FirstLine) && (i <= MyWindSubDomains[k].LastLine) &&
                   (j >= MyWindSubDomains[k].FirstColumn) && (j <= MyWindSubDomains[k].LastColumn)
                 )
              {
                 AverageWindSpeed[index] = WindSpeed[index2]; // ms-1
                 AverageWindSpeedX[index] = -WindSpeed[index2] * sin(WindDirection[index2]); // ms-1
                 AverageWindSpeedY[index] = -WindSpeed[index2] * cos(WindDirection[index2]); // ms-1
              }
          }
       }
    }
}

bool TWindTimeSeries::SaveVariables()
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
    bool dailyValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Daily wind speed") == 0)
            dailyValues = true;
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save variables' initial values - TWindTimeSeries
     */
    if (NumberOfMomentsForWindTimeSeries > 0) {
	    PReadWrite->WriteCell("Wind speed values");
	    PReadWrite->WriteSeparator();
	    PReadWrite->WriteCell("Wind Direction values");
	    PReadWrite->WriteSeparator(true);

	    for (int j = 0; j < NumberOfMomentsForWindTimeSeries; j++)
	    {
	        PReadWrite->WriteCell(WindSpeed[j], 8);
	        PReadWrite->WriteSeparator();
	        PReadWrite->WriteCell(WindDirection[j], 8);
	        PReadWrite->WriteSeparator(true);
	    }
	    PReadWrite->WriteSeparator(true);
	}
    /*
     * save variables' values inherited from TWindDailyTimeSeries
     */
    if (dailyValues) {
        PReadWrite->WriteCell("Daily wind speed values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < 365; j++)
        {
            PReadWrite->WriteCell(DailyWindSpeed[j], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

double TWindTimeSeries::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "NumberOfDaysForWindTimeSeries") == 0)
    {
        value = NumberOfDaysForWindTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForWindTimeSeries") == 0)
    {
        value = NumberOfHoursForWindTimeSeries;
    }
    else
        value = 0.0;
    return value;
}

bool TWindTimeSeries::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "NumberOfDaysForWindTimeSeries") == 0)
    {
        NumberOfDaysForWindTimeSeries = value;
        NumberOfMomentsForWindTimeSeries = NumberOfDaysForWindTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForWindTimeSeries") == 0)
    {
        NumberOfHoursForWindTimeSeries = value;
        NumberOfMomentsForWindTimeSeries = NumberOfHoursForWindTimeSeries;
    }
    else
        rc = false;

    return rc;
}


bool TWindTimeSeries::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Wind");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Wind");
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
        if (strcmp(ParametersNameArray[i], "NumberOfDaysForWindTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForWindTimeSeries, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfHoursForWindTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfHoursForWindTimeSeries, 0);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}


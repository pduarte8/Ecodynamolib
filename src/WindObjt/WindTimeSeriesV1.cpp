// Wind object CPP code file
// @ Pedro Duarte
// 22/07/2010

/* Este objecto l? s?ries temporais de valores di?rios ou hor?rios de velocidade (m/s) e direc??o do vento (radianos) a partir
de um ficheiro de texto, para usar como fun??o for?adora de outros objectos. O primeiro valor do ficheiro deve corresponder ao
primeiro momento simulado. Assim, quando se modifica o per?odo simulado, a s?rie temporal tem de ser reorganizada.
Quando o tempo simulado excede o per?odo correspondente ? s?rie temporal, este objecto devolve o ?ltimo
valor da s?rie.*/

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
// Completar


TWindTimeSeriesV1::TWindTimeSeriesV1(TEcoDynClass* APEcodynClass, char* className)
         :TWindTimeSeries(APEcodynClass, className)
{
    BuildWindTimeSeriesV1();
}

void TWindTimeSeriesV1::BuildWindTimeSeriesV1()
{
    AStartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    AFinishTime = MyPEcoDynClass->getDefaultsRecord()->FinishTime;
    ATime = AStartTime;
    ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
    for (int i = 0; i < NumberOfBoxes; i++)
    {
      AverageWindSpeed[i] = WindSpeed[CalculateMoment()]; // ms-1
      AverageWindSpeedX[i] = -WindSpeed[CalculateMoment()] * sin(WindDirection[CalculateMoment()]); // ms-1
      AverageWindSpeedY[i] = -WindSpeed[CalculateMoment()] * cos(WindDirection[CalculateMoment()]); // ms-1
    }
}

TWindTimeSeriesV1::~TWindTimeSeriesV1()
{
	freeMemory();
}

void TWindTimeSeriesV1::freeMemory()
{
}

int TWindTimeSeriesV1::CalculateMoment()
{
    int ADay, AHour, AMoment;
    AMoment = 0.0;
    ATime = ATime + MyPEcoDynClass->GetTimeStep();
    ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS + 1);
    if  (NumberOfDaysForWindTimeSeries >= 1)
    {
       AMoment = MIN(ADay - 1, NumberOfDaysForWindTimeSeries - 1);
       //Debugger(AMoment);
    }
    else
    if (NumberOfHoursForWindTimeSeries >= 1)
    {
       AHour = (ADay - 1) * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForWindTimeSeries - 1);
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "TWindTimeSeriesV1::CalculateMoment", "Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForWindTimeSeries", MB_OK);
#else  // __BORLANDC__
			 cerr << "TWindTimeSeriesV1::CalculateMoment - Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForWindTimeSeries" << endl;
#endif  // __BORLANDC__
    return AMoment;
}




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
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "flowobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"

// TOneDimensionalFlow Class
//


TRiaFormosaFlow::TRiaFormosaFlow(TEcoDynClass* PEcoDynClass,
        char* className) : TOneDimensionalFlow(PEcoDynClass, className)
{
    BuildRiaFormosaFlow();
}

void TRiaFormosaFlow::BuildRiaFormosaFlow()
{
    if (MyPEcoDynClass->HasARiverRecord())
    {
        MyRiverRecord = MyPEcoDynClass->GetARiverRecord();
        NumberOfRivers = MyPEcoDynClass->GetNumberOfRivers();
        //Debugger(NumberOfRivers);
    }
    else
    {
        MyRiverRecord = NULL;
        NumberOfRivers = 0;
    }
}


void TRiaFormosaFlow::Inquiry(char* srcName, double &Value,
										 int BoxNumber, //int ABoundary,
										 char* ParameterName,
										 int AnObjectCode)
{
    int MyDay, ADay;
    MyDay = MyPEcoDynClass->GetJulianDay() - 1;
    ADay = MyDay;
    if (MyDay  > NumberOfDaysForFlowTimeSeries - 1)
    {
        ADay = NumberOfDaysForFlowTimeSeries - 1;
    }
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    int LineIndex = GetLineIndex(MyBoxNumber);
    int ColumnIndex= GetColumnIndex(MyBoxNumber);
    //Debugger(LineIndex); Debugger(ColumnIndex);
    strcpy(MyParameter, ParameterName);
    //Debugger(NumberOfVariables);
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (strcmp(MyParameter, VariableNameArray[i]) == 0)
        {
            if (strcmp(MyParameter, "Daily river flow") == 0)
            {
               //Debugger(NumberOfRivers);
               for (int River = 0; River < NumberOfRivers; River++)
               {
                  if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
                  {
                      ColumnIndex = MyBoxNumber / (MyPEcoDynClass->GetNumberOfColumns() + 1);
                  }
                  if (
                        (MyRiverRecord[River].LineCoordinate == LineIndex) &&
                        (MyRiverRecord[River].ColumnCoordinate == ColumnIndex) /*&&
                        (ABoundary == ARiverRecord[River].BoundaryNumber) */
                      )
                   {
                     Value = MyRiverRecord[River].RiverFlow[ADay];
                     //Debugger(Value);
                   }
               }
            }
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
            return;
        }
    }
}



void TRiaFormosaFlow::Go()
{

}


// AP, 2007.05.28
bool TRiaFormosaFlow::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
    bool rc = true;
    int LineIndex = GetLineIndex(BoxNumber);
    int ColumnIndex = GetColumnIndex(BoxNumber);

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Daily river flow") == 0)
    {
       for (int River = 0; River < NumberOfRivers; River++)
       {
          if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
          {
              ColumnIndex = BoxNumber / (MyPEcoDynClass->GetNumberOfColumns() + 1);
          }
          if (
                (MyRiverRecord[River].LineCoordinate == LineIndex) &&
                (MyRiverRecord[River].ColumnCoordinate == ColumnIndex) /*&&
                (ABoundary == ARiverRecord[River].BoundaryNumber) */
              )
           {
             for (int ADay = 0; ADay < NumberOfDaysForFlowTimeSeries; ADay++)
                 MyRiverRecord[River].RiverFlow[ADay] = Value;
             return true;
           }
       }
    }
    else
        rc = false;

    return rc;
}



// SPM object CPP code file
// @ Pedro Duarte
// REV A - 2005.03.06
/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "sedimentobjt.h"
#include "iodll.h"
#include "params.h"

// TSediments Class
//

TSedimentsTimeSeries::TSedimentsTimeSeries(TEcoDynClass* APEcoDynClass, char* className)
        : TSediments(APEcoDynClass, className)
{
/*
 * NumberOfVariables and variables names and values already read in "TSediments"
 *

 //        TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
    	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Sediments");
        if (PReadWrite == NULL)
        {
           return;
        }
        char MyVariable[65];
        int Index, X, Y;
        if (PReadWrite->FindString("Sediments", X, Y))
        {
           for (int i = 0; i < NumberOfVariables; i++)
           {
              //Debugger(i); Debugger(NumberOfBoxesWithSediments);
              PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
              if (strcmp(MyVariable, "NH4InPoreWater 1") == 0)
              {
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                   Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                   PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, NH4InPoreWater[Index]);
                 }
                 Debugger(NH4InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "O2InPoreWater 1") == 0)
              {
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, O2InPoreWater[Index]);
                 }
                 Debugger(Index);
                 Debugger(O2InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "NO3InPoreWater 1") == 0)
              {
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, NO3InPoreWater[Index]);
                 }
                 Debugger(Index);
                 Debugger(NO3InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "PO4InPoreWater 1") == 0)
              {
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1 + j, PO4InPoreWater[Index]);
                 }
                 Debugger(Index);
                 Debugger(PO4InPoreWater[Index]);
              }
	   }
        }
        else MessageBox(0, "Variables - Undefined object in Sango sediments", "EcoDynamo alert", MB_OK);
    CloseDataFile((void*)PReadWrite);
*/
}

void TSedimentsTimeSeries::Go()
{

}

void TSedimentsTimeSeries::Integrate()
{
}

void TSedimentsTimeSeries::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{

}

void TSedimentsTimeSeries::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName, int AnObjectCode)
{
    static char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);
    if (strcmp(MyParameter, "NH4InPoreWater 1") == 0)
       Value = NH4InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "O2InPoreWater 1") == 0)
       Value = O2InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "NO3InPoreWater 1") == 0)
       Value = NO3InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "PO4InPoreWater 1") == 0)
       Value = PO4InPoreWater[MyBoxNumber];
    else {
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
            strcat(Caption, " does not exist in this object");
            MessageBox(0,Caption,"EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSedimentsTimeSeries::Inquiry - " << MyParameter << " does not exits in this object" << endl;
#endif  // __BORLANDC__
				}
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TSedimentsTimeSeries::SetVariableValue(char* srcName, double Value,
            int BoxNumber,char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "NH4InPoreWater 1") == 0)
       NH4InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "O2InPoreWater 1") == 0)
       O2InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "NO3InPoreWater 1") == 0)
       NO3InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "PO4InPoreWater 1") == 0)
       PO4InPoreWater[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


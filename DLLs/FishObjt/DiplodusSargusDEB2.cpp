#include <math.h>
#include <vcl.h>
#include <stdlib.h>
#include "ecodyn.rh"
#include "fishobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"
#include <math.h>
#include <vcl.h>
#include <stdlib.h>


TDiplodusSargusDEB2::TDiplodusSargusDEB2(TEcoDynClass* APEcoDynClass, char* className)
							 :	TFish(APEcoDynClass, className)
{
   strcpy(MyFishName,"DiplodusSargus");
   BuildFish();
}


void TDiplodusSargusDEB2::ReadParameters()
{
    // Read in the parameters
    NumberOfParameterRecords = 0;
    //MyPReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("DiplodusSargus");
    MyPReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile(MyFishName);
    if (MyPReadWrite == NULL)
    {
            return;
    }
    double MyValue;
    int X, Y;
    if (MyPReadWrite->FindString("Fish", X, Y))
    {
        MyPReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
        MyParsRecord = new ParsRecord[NumberOfParameters - 1];
        // read in the parameter names
        char MyParameter[65];
        int index;
        index = NumberOfParameterRecords - 1;
        for (int i = Y; i < Y + NumberOfParameters; i++)
        {
            //Debugger(i);
            MyPReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "JXm") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"JXm");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                //Debugger(MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);

            }
            else if (strcmp(MyParameter, "Xk") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"Xk");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);

            }
            else if (strcmp(MyParameter, "PAm") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"PAm");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);

            }
            else if (strcmp(MyParameter, "AE") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"AE");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                AbsorptionEfficiency = true;
            }
            else if (strcmp(MyParameter, "Em") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Em");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "Eg") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Eg");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "Shape coefficient") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Shape coefficient");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "Shape coefficient 2") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Shape coefficient 2");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "K") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"K");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "PM") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"PM");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "Body density") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Body density");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "Dry wet ratio") == 0)
            {
                index++;     //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"Dry wet ratio");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "TA") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"TA");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "T1") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"T1");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
            }
            else if (strcmp(MyParameter, "EnergyPerReserveMass") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, EnergyPerReserveMass);
                //Debugger(EnergyPerReserveMass);
            }
            else if (strcmp(MyParameter, "FoodDecayRate") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, FoodDecayRate);
                //Debugger(FoodDecayRate);
            }
            else if (strcmp(MyParameter, "AdultLength") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, AdultLength);
                Debugger(AdultLength);
            }
            else if (strcmp(MyParameter, "NumberOfIndividuals") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, NumberOfIndividuals);
            }
            else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, IndividualBasedVariables);
            }
            //Debugger(index);
        }
        //NumberOfParameterRecords = index + 1;
        //Debugger(NumberOfParameterRecords);
    }
    else
       MessageBox(0, "Undefined parameters in Fish parameters file", "EcoDynamo alert", MB_OK);
}


double TDiplodusSargusDEB2::FishLength(int AFish)
{
   double MyShapeCoefficient;
   if (MyIndividualRecord[AFish].Age < AdultLength)
      Inquiry(GetEcoDynClassName(), MyShapeCoefficient, AFish, 0,"Shape coefficient",ObjectCode);
   else
      Inquiry(GetEcoDynClassName(), MyShapeCoefficient, AFish, 0,"Shape coefficient 2",ObjectCode);
   return pow(MyIndividualRecord[AFish].Volume,0.333333333)/MyShapeCoefficient;
}
 

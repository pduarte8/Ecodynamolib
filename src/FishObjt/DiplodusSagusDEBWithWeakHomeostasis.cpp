/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
    #include <stdlib.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
    #include <cstdlib>

	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "fishobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"


TDiplodusSargusDEBWithWeakHomeostasis::TDiplodusSargusDEBWithWeakHomeostasis(TEcoDynClass* APEcoDynClass, char* className)
							 :	TFishWithWeakHomeostasis(APEcoDynClass, className)
{
   strcpy(MyFishName,"DiplodusSargus");
   BuildFish();
}


void TDiplodusSargusDEBWithWeakHomeostasis::ReadParameters()
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
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */

            }
            else if (strcmp(MyParameter, "Xk") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"Xk");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */

            }
            else if (strcmp(MyParameter, "PAm") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"PAm");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */

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
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Eg") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Eg");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Shape coefficient") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Shape coefficient");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "K") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"K");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "PM") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"PM");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Body density") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Body density");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Dry wet ratio") == 0)
            {
                index++;     //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"Dry wet ratio");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                //Debugger(MyParsRecord[index].Value);
                /*Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
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
            else if (strcmp(MyParameter, "NumberOfIndividuals") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, NumberOfIndividuals);
            }
            else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, IndividualBasedVariables);
            }
            else if (strcmp(MyParameter, "FishCNratio") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, FishCNratio);
                //Debugger(FishCNratio);
            }
            else if (strcmp(MyParameter, "FishCPratio") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, FishCPratio);
                //Debugger(FishCPratio);
            }
            else if (strcmp(MyParameter, "PolysaccharideContent") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, PolysaccharideContent);
                //Debugger(PolysaccharideContent);
            }
            else if (strcmp(MyParameter, "LipidContent") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, LipidContent);
                //Debugger(LipidContent);
            }
            else if (strcmp(MyParameter, "ProteinContent") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, ProteinContent);
                //Debugger(ProteinContent);
            }
            else if (strcmp(MyParameter, "FoodEnergyContent") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, FoodEnergyContent);
                //Debugger(FoodEnergyContent);
            }
            //Debugger(index);
        }
        //NumberOfParameterRecords = index + 1;
        //Debugger(NumberOfParameterRecords);
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "Undefined parameters in Fish parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TDiplodusSargusDEBWithWeakHomeostasis::ReadParameters - Undefined parameters in Fish parameters file" << endl;
#endif  // __BORLANDC__
}

/*
void TDiplodusSargusDEBWithWeakHomeostasis::ReadVariables()
{
    // Read in the variables
    char MyVariable[65];
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile(MyFishName);
    if (PReadWrite == NULL)
       MessageBox(0, "Fish variables file missing.", MyFishName, MB_OK);
    else {
        double MyValue;
        int X, Y;
        if (PReadWrite->FindString("Fish", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // read in the variable names
            //Debugger(NumberOfVariables);
            VariableNameArray = new VNA[NumberOfVariables];
            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
            }
            int XV, YV;
            for (int i = 0; i < NumberOfVariables; i++)
            {
               //Debugger(i);
               PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
               if (strcmp(MyVariable, "Fish body volume") == 0)         //cm3 / individual
               {
                  if (PReadWrite->FindString("Fish body volume values", XV, YV))
                  {
                      PReadWrite->ReadNumber(XV, YV + 1, MyValue);
                      for (int n = 0; n < NumberOfIndividuals; n++)
                      {
                         MyIndividualRecord[n].Volume = MyValue;
                         MyIndividualRecord[n].Length = FishLength(n);
                      }
                  }
                  else // old file format
                  {
                      PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                      //Debugger(MyValue);
                      for (int n = 0; n < NumberOfIndividuals; n++)
                      {
                         MyIndividualRecord[n].Volume = MyValue;
                         MyIndividualRecord[n].Length = FishLength(n);
                      }
                  }
               }
               else if (strcmp(MyVariable, "Fish reserve energy") == 0)        //J cm-3
               {
                  if (PReadWrite->FindString("Fish reserve energy values", XV, YV))
                  {
                      PReadWrite->ReadNumber(XV, YV + 1, MyValue);
                      for (int n = 0; n < NumberOfIndividuals; n++)
                         MyIndividualRecord[n].Reserves = MyValue;
                  }
                  else // old file format
                  {
                      PReadWrite->ReadNumber( X + 2 + i, Y + 1, MyValue);
                      //Debugger(MyValue);
                      for (int n = 0; n < NumberOfIndividuals; n++)
                         MyIndividualRecord[n].Reserves = MyValue;
                  }
               }
            }
        }
        else MessageBox(0, "Variables: Undefined object in Fish", "EcoDynamo alert", MB_OK);
        CloseDataFile((void*)PReadWrite);
    } 
}
*/

/*void TDiplodusSargusDEBWithWeakHomeostasis::PreBuildFish()
{
   AWaterTemperature = 0.0;
   FishBiomass = new double[NumberOfBoxes];
   FishDensity = new double[NumberOfBoxes];
   FishBiomass[0] = 0.0;
   FishDensity[0] = (double)NumberOfIndividuals;
   MyIndividualRecord = new IndividualRecord[NumberOfIndividuals];
} */

/*
void TDiplodusSargusDEBWithWeakHomeostasis::PostBuildFish()
{
    randomize();
    for (int  i = 0; i < NumberOfIndividuals; i++)
    {
        MyIndividualRecord[i].AE = 0.0; MyIndividualRecord[i].PA = 0.0;
        MyIndividualRecord[i].Volume = 0.0; MyIndividualRecord[i].Weight = 0.0;
        MyIndividualRecord[i].Reserves = 0.0; MyIndividualRecord[i].Length = 0.0;
        MyIndividualRecord[i].Age = 0.0; MyIndividualRecord[i].Jx = 0.0;
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            MyIndividualRecord[i].IndividualParameters[j] = MyParsRecord[j];

            if (MyIndividualRecord[i].IndividualParameters[j].HigherValue -
                MyIndividualRecord[i].IndividualParameters[j].LowerValue > 0.0)
            {
                if (!MyIndividualRecord[i].IndividualParameters[j].RandomizeFlag)  // Sistematic increment of parameter value from its minimum till its maximum
                {
                    //Debugger(i); Debugger(j);
                    if (i == 0) //First individual gets the lower value
                    {
                        MyIndividualRecord[i].IndividualParameters[j].Value = MyIndividualRecord[i].IndividualParameters[j].LowerValue;
                        //Debugger(i);
                        //Debugger(MyIndividualRecord[i].IndividualParameters[j].Value);
                    }
                    else
                    {
                        MyIndividualRecord[i].IndividualParameters[j].Value =
                            (MyIndividualRecord[i].IndividualParameters[j].HigherValue - MyIndividualRecord[i].IndividualParameters[j].LowerValue) /
                            (NumberOfIndividuals - 1) + MyIndividualRecord[i - 1].IndividualParameters[j].Value;
                        //Debugger(i);
                        //Debugger(MyIndividualRecord[i].IndividualParameters[j].Value);
                    }
                }
                else //Random generation of parameter values between lower and higher limits
                {
                    double ARandomValue; double const MaximumRandomValue = 10000;
                    ARandomValue = random(MaximumRandomValue);
                    //Debugger(i); Debugger(j);
                    //ARandomValue = rand() % MaximumRandomValue;
                    MyIndividualRecord[i].IndividualParameters[j].Value =
                        MyIndividualRecord[i].IndividualParameters[j].LowerValue +
                        (MyIndividualRecord[i].IndividualParameters[j].HigherValue - MyIndividualRecord[i].IndividualParameters[j].LowerValue) *
                        ARandomValue / MaximumRandomValue;
                    //Debugger(MyIndividualRecord[i].IndividualParameters[j].Value);
                }
            }
        }
    }
}
 */

 

/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "zoobobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"


TMytilusGalloprovincialisIndividualDEB::TMytilusGalloprovincialisIndividualDEB(TEcoDynClass* APEcoDynClass, char* className)
							 :	TMytilusGalloprovincialisIndividual(APEcoDynClass, className)
{
   BuildMytilusGalloprovincialisIndividual();
}


void TMytilusGalloprovincialisIndividualDEB::BuildMytilusGalloprovincialisIndividual()
{
    if (NumberOfBoxes > 0)
    {
       ZooBodyVolume = new double[NumberOfBoxes * NumberOfClasses];
       ZooReserves = new double[NumberOfBoxes * NumberOfClasses];
       FoodEnergeticContents = new double[NumberOfBoxes];
       for (int i = 0; i < NumberOfBoxes * NumberOfClasses; i++)
       {
          ZooBodyVolume[i] = 0.0;
          ZooReserves[i] = 0.0;
          if (i < NumberOfBoxes) FoodEnergeticContents[i] = 0.0;
       }
    }
    TA = 0.0;
    T1 = 0.0;
    AWaterTemperature = 0.0;
    ReadParameters(); 
    ReadVariables();
    PostBuildMytilusGalloprovincialisIndividual();
}

void TMytilusGalloprovincialisIndividualDEB::ReadParameters()
{
    // Read in the parameters
    MyPReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Mytilus");
    if (MyPReadWrite == NULL)
    {
            return;
    }

    double MyValue;
    int X, Y;
    if (MyPReadWrite->FindString("Mytilus galloprovincialis", X, Y))
    {
        MyPReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
        //Debugger(NumberOfParameters);
        //MyParsRecord = new ParsRecord[NumberOfParameters-1];
        // read in the parameter names
        char MyParameter[65];
        int index;
        index = NumberOfParameterRecords - 1;
        //Debugger(index);
        for (int i = Y; i < Y + NumberOfParameters; i++)
        {
            //Debugger(i);
            MyPReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "PAm") == 0)
            {
                index++;
                //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"PAm");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */

            }
            else if (strcmp(MyParameter, "Em") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Em");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Eg") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Eg");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Shape coefficient") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Shape coefficient");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "K") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"K");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "PM") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"PM");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "Body density") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"Body density");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment); */
            }
            else if (strcmp(MyParameter, "Dry wet ratio") == 0)
            {
                index++;     //Debugger(index);
                strcpy(MyParsRecord[index].ParameterName,"Dry wet ratio");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                /*Debugger(MyParsRecord[index].Value);
                Debugger(MyParsRecord[index].HigherValue);
                Debugger(MyParsRecord[index].LowerValue);
                Debugger(MyParsRecord[index].Increment);*/
            }
            else if (strcmp(MyParameter, "TA") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"TA");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
            }
            else if (strcmp(MyParameter, "T1") == 0)
            {
                index++;
                strcpy(MyParsRecord[index].ParameterName,"T1");
                MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
            }
            //Debugger(index);
        }
        NumberOfParameterRecords = index + 1;
        //Debugger(NumberOfParameterRecords);
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "Undefined parameters in Mytilus parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TMytilusGalloprovincialisIndividualDEB::ReadParameters - Undefined parameters in Mytilus parameters file" << endl;
#endif  // __BORLANDC__
}

void TMytilusGalloprovincialisIndividualDEB::ReadVariables()
{
    // Read in the variables
    char MyVariable[65];
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Mytilus");
    if (PReadWrite == NULL)
#ifdef __BORLANDC__
       MessageBox(0, "MytilusGalloprovinvialisDEB variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TMytilusGalloprovincialisIndividualDEB::ReadVariables - variables file missing." << endl;
#endif  // __BORLANDC__
    else {
        double MyValue;
        int X, Y;
        if (PReadWrite->FindString("Mytilus galloprovincialis", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            //Debugger(NumberOfVariables);
            // Create array for state variable names
           // VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
            }
            int k, j, RecordNumber, Index, offset, XV, YV;
            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
               if (strcmp(MyVariable, "M. galloprovincialis body volume 1") == 0)         //g Shell dry weight / individual
               {
                  if (PReadWrite->FindString("M. galloprovincialis body volume 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                              Index = MytilusGalloprovincialisRecord[j].LineCoordinate * NumberOfColumns +
                                      MytilusGalloprovincialisRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooBodyVolume[offset + Index] = MyValue;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithMytilusGalloprovincialis; j < k * NumberOfBoxesWithMytilusGalloprovincialis + NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                            Index = MytilusGalloprovincialisRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    MytilusGalloprovincialisRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooBodyVolume[offset + Index] = MyValue;
                            RecordNumber++;
                          }
                      }
                  }
               }
               else if (strcmp(MyVariable, "M. galloprovincialis reserve energy 1") == 0)        //g Soft tissue dry weight / individual
               {
                  if (PReadWrite->FindString("M. galloprovincialis reserve energy 1 values", XV, YV))
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          for (j = 0; j < NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                              Index = MytilusGalloprovincialisRecord[j].LineCoordinate * NumberOfColumns +
                                      MytilusGalloprovincialisRecord[j].ColumnCoordinate;
                              PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                              ZooReserves[offset + Index] = MyValue;
                          }
                      }
                  }
                  else // old file format
                  {
                      for (k = 0; k < NumberOfClasses; k++)
                      {
                          offset = k * NumberOfBoxes;
                          RecordNumber = 0;
                          for (j = k * NumberOfBoxesWithMytilusGalloprovincialis; j < k * NumberOfBoxesWithMytilusGalloprovincialis + NumberOfBoxesWithMytilusGalloprovincialis; j++)
                          {
                            Index = MytilusGalloprovincialisRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                    MytilusGalloprovincialisRecord[RecordNumber].ColumnCoordinate;
                            PReadWrite->ReadNumber( X + 2 + i, Y + 1 + j, MyValue);
                            ZooReserves[offset + Index] = MyValue;
                            RecordNumber++;
                          }
                      }
                  }
               }
            }
        }
        else 
#ifdef __BORLANDC__
					MessageBox(0, "Variables: Undefined object in Mytilus", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TMytilusGalloprovincialisIndividualDEB::ReadVariables - Undefined object in Mytilus" << endl;
#endif  // __BORLANDC__

        CloseDataFile((void*)PReadWrite);
    }
}

void TMytilusGalloprovincialisIndividualDEB::PostBuildMytilusGalloprovincialisIndividual()
{
    int index;
    for (int k = 0; k < NumberOfBoxesWithMytilusGalloprovincialis; k++)
    {
      for (int  i = 0; i < NumberOfIndividuals; i++)
      {
          index = i * NumberOfBoxesWithMytilusGalloprovincialis + k;
          //Debugger(i);
          MyIndividualRecord[index].PA = 0.0;
          MyIndividualRecord[index].ZooIndividualVolume = ZooBodyVolume[k];
          MyIndividualRecord[index].ZooIndividualReserves = ZooReserves[k];

          for (int j = 0; j < NumberOfParameterRecords; j++)
          {
              MyIndividualRecord[index].IndividualParameters[j] = MyParsRecord[j];

              if (MyIndividualRecord[index].IndividualParameters[j].HigherValue -
                  MyIndividualRecord[index].IndividualParameters[j].LowerValue > 0.0)
              {
                  if (!MyIndividualRecord[index].IndividualParameters[j].RandomizeFlag)  // Sistematic increment of parameter value from its minimum till its maximum
                  {
                      //Debugger(index); Debugger(j);
                      if (index == 0) //First individual gets the lower value
                      {
                          MyIndividualRecord[index].IndividualParameters[j].Value = MyIndividualRecord[index].IndividualParameters[j].LowerValue;
                          //Debugger(index);
                          //Debugger(MyIndividualRecord[index].IndividualParameters[j].Value);
                      }
                      else
                      {
                          MyIndividualRecord[index].IndividualParameters[j].Value =
                              (MyIndividualRecord[index].IndividualParameters[j].HigherValue - MyIndividualRecord[index].IndividualParameters[j].LowerValue) /
                              (NumberOfIndividuals - 1) + MyIndividualRecord[index - 1].IndividualParameters[j].Value;
                          //Debugger(index);
                          //Debugger(MyIndividualRecord[index].IndividualParameters[j].Value);
                      }
                  }
                  else //Random generation of parameter values between lower and higher limits
                  {
                      double ARandomValue; double MaximumRandomValue = 10000;
                      ARandomValue = random(MaximumRandomValue);
                      //Debugger(index); Debugger(j);
                      //ARandomValue = rand() % MaximumRandomValue;
                      MyIndividualRecord[index].IndividualParameters[j].Value =
                          MyIndividualRecord[index].IndividualParameters[j].LowerValue +
                          (MyIndividualRecord[index].IndividualParameters[j].HigherValue - MyIndividualRecord[index].IndividualParameters[j].LowerValue) *
                          ARandomValue / MaximumRandomValue;
                      //Debugger(MyIndividualRecord[index].IndividualParameters[j].Value);
                  }
              }
          }
          MyIndividualRecord[index].ShellLength = ShellLength(i,k);
      }

    }

}

TMytilusGalloprovincialisIndividualDEB::~TMytilusGalloprovincialisIndividualDEB()
{
    freeMemory();
}

void TMytilusGalloprovincialisIndividualDEB::freeMemory()
{
    delete [] ZooBodyVolume;
    delete [] ZooReserves;
    delete [] FoodEnergeticContents;
    //delete [] VariableNameArray;
}



void TMytilusGalloprovincialisIndividualDEB::Inquiry(char* srcName, double &Value,int BivalveNumber, int BoxNumber,
					char* ParameterName,int AnObjectCode)
{
    int i,j, index;
    char MyParameter[65], // 64 characters (max) for parameter name
    Myjstring[5],MyName[65];
    int MyBivalveNumber = BivalveNumber;
    int MyBoxNumber = BoxNumber;
    index = MyBivalveNumber * NumberOfBoxesWithMytilusGalloprovincialis + MyBoxNumber;
    strcpy(MyParameter, ParameterName);
    //Debugger(NumberOfParameterRecords); Debugger(NumberOfVariables);

    //Procura da string entre as vari?veis

    for (i = 0; i < NumberOfVariables; i++)
    {
      //MessageBox(0,"Caption",VariableNameArray[i],MB_OK | MB_ICONHAND);
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis CR " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring );
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].ZooCR;
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis meat weight " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring );
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].ZooIndividualWeight;
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis shell mass " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring );
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].DSW;
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "Individual growth " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring);
                if (strcmp(MyParameter, MyName) == 0){
                        Value = MyIndividualRecord[index].ZooIndividualFlux; // Values in g day-1 ind-1
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis shell length " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring);
                if (strcmp(MyParameter, MyName) == 0) {
                        Value = MyIndividualRecord[index].ShellLength; // Values in mm
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis body volume " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring);
                if (strcmp(MyParameter, MyName) == 0) {
                        Value = MyIndividualRecord[index].ZooIndividualVolume; // Values in cm3
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        for (j = 0; j < NumberOfClasses; j++)
        {
                strcpy( MyName, "M. galloprovincialis reserve energy " );
                sprintf( Myjstring , "%i", j + 1 );
                strcat( MyName, Myjstring);
                if (strcmp(MyParameter, MyName) == 0) {
                        Value = MyIndividualRecord[index].ZooIndividualReserves; // Values in J
                        LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                        return;
                }
        }
        if (strcmp(MyParameter, "IRPHY") == 0)
        {
           Value = MyIndividualRecord[index].IRPHY;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);return;
        }
        else if (strcmp(MyParameter, "IRDET") == 0)
        {
           Value = MyIndividualRecord[index].IRDET;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "IRPIM") == 0)
        {
           Value = MyIndividualRecord[index].IRPIM;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "IRTPM") == 0)
        {
           Value = MyIndividualRecord[index].IRTPM;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "FRTPM") == 0)
        {
           Value = MyIndividualRecord[index].FRTPM;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "FRPIM") == 0)
        {
           Value = MyIndividualRecord[index].FRPIM;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "RRTPM") == 0)
        {
           Value = MyIndividualRecord[index].RRTPM;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "RRPIM") == 0)
        {
           Value = MyIndividualRecord[index].RRPIM;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OIR") == 0)
        {
           Value = MyIndividualRecord[index].OIR;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OCI") == 0)
        {
           Value = MyIndividualRecord[index].OCI;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OCPF") == 0)
        {
           Value = MyIndividualRecord[index].OCPF;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NAEIO") == 0)
        {
           Value = MyIndividualRecord[index].NAEIO;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NEA") == 0)
        {
           Value = MyIndividualRecord[index].NEA;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NEB") == 0)
        {
           Value = MyIndividualRecord[index].NEB;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "SomC") == 0)
        {
           Value = MyIndividualRecord[index].SomC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "SomN") == 0)
        {
           Value = MyIndividualRecord[index].SomN;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "StorC") == 0)
        {
           Value = MyIndividualRecord[index].StorC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "OSW") == 0)
        {
           Value = MyIndividualRecord[index].OSW;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "Gut passage time") == 0)
        {
           Value = MyIndividualRecord[index].GPT;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ARC") == 0)
        {
           Value = MyIndividualRecord[index].ARC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ARN") == 0)
        {
           Value = MyIndividualRecord[index].ARN;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "PA") == 0)
        {
           Value = MyIndividualRecord[index].PA;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NetProdC") == 0)
        {
           Value = MyIndividualRecord[index].NetProdC;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "NetProdN") == 0)
        {
           Value = MyIndividualRecord[index].NetProdN;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "DSW") == 0)
        {
           Value = MyIndividualRecord[index].DSW;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "ConditionIndex") == 0)
        {
           Value = MyIndividualRecord[index].ConditionIndex;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "SomFrac") == 0)
        {
           Value = MyIndividualRecord[index].SomFrac;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "StorFrac") == 0)
        {
           Value = MyIndividualRecord[index].StorFrac;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
        else if (strcmp(MyParameter, "Age") == 0)
        {
           Value = MyIndividualRecord[index].Age;
           LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;
        }
      }
    }  //i
    //Fim de procura da string entre as vari?veis
    //Debugger(1);
    //Procura da string entre os par?metros
    if (strcmp(MyParameter, "PAm") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "PAm") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Em") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Em") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Eg") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Eg") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Shape coefficient") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Shape coefficient") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "K") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "K") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "PM") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "PM") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Dry wet ratio") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Dry wet ratio") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Body density") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Body density") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Meat energetic contents") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "CDWFrac") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "CDWFrac") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "NDWFracInGametes") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "NDWFracInGametes") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "CDWFood") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "CDWFood") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "NDWFood") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "NDWFood") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Agc") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Agc") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Bgc") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Bgc") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "GPTMin") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "GPTMin") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "GPTMax") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "GPTMax") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "PHYORGDigestibility") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "PHYORGDigestibility") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return;}
        }
    }
    else if (strcmp(MyParameter, "DETORGDigestibility") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "DETORGDigestibility") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Q10Res") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Q10Res") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Reference temperature") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Reference temperature") == 0) {
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "ReproductionDate") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "ReproductionDate") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            //Debugger(j); Debugger(Value);
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "ReproductionExpenditure") == 0)
    {
       for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "ReproductionExpenditure") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "TA") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "TA") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "T1") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "T1") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    //Fim da procura da string entre os par?metros
#ifdef __BORLANDC__
    char Caption[129];
    strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
    MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TMytilusGalloprovincialisIndividualDEB::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
		Value = 0;
}


void TMytilusGalloprovincialisIndividualDEB::Inquiry(char* srcName, double &Value,
						int BoxNumber,char* ParameterName,int AnObjectCode)
{
    int i,j;
    char MyParameter[65], // 64 characters (max) for parameter name
    Myjstring[5],
    MyName[65];
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

    for (i = 0; i < NumberOfVariables; i++)
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
              for (j = 0; j < NumberOfClasses; j++)
              {
                      strcpy( MyName, "M. galloprovincialis density " );
                      sprintf( Myjstring , "%i", j + 1 );
                      strcat( MyName, Myjstring );
                      if (strcmp(MyParameter, MyName) == 0)
                              Value = ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
              }
              for (j = 0; j < NumberOfClasses; j++)
              {
                      strcpy( MyName, "M. galloprovincialis biomass " );
                      sprintf( Myjstring , "%i", j + 1 );
                      strcat( MyName, Myjstring );
                      if (strcmp(MyParameter, MyName) == 0)
                              Value = ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
              }
              for (j = 0; j < NumberOfClasses; j++)
              {
                      strcpy( MyName, "M. galloprovincialis body volume " );
                      sprintf( Myjstring , "%i", j + 1 );
                      strcat( MyName, Myjstring );
                      if (strcmp(MyParameter, MyName) == 0)
                              Value = ZooBodyVolume[NumberOfBoxes * j + MyBoxNumber];
              }
              for (j = 0; j < NumberOfClasses; j++)
              {
                      strcpy( MyName, "M. galloprovincialis reserve energy " );
                      sprintf( Myjstring , "%i", j + 1 );
                      strcat( MyName, Myjstring );
                      if (strcmp(MyParameter, MyName) == 0)
                              Value = ZooReserves[NumberOfBoxes * j + MyBoxNumber];
              }

             LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
             return;
    }
#ifdef __BORLANDC__
  char Caption[129];
  strcpy(Caption, MyParameter);
  strcat(Caption, " does not exist in ");
  strcat(Caption, GetEcoDynClassName());
  MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 2",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "TMytilusGalloprovincialisIndividualDEB::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
  Value = 0;
}

void TMytilusGalloprovincialisIndividualDEB::Go()
{
   JulianDay = MyPEcoDynClass->GetJulianDay();
   int index;    
   double MyDepth, MyChlorophyllCarbonRatio, MyCDWFrac, WSom, WStor, MyOSWtoDSW, MyBodyDensity, MyDryWetRatio, ShapeCoefficient, Eg;
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
                * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
   {
      for (int i = 0; i < NumberOfBoxes; i++)
      {
         MySPMPointer->Inquiry(GetEcoDynClassName(), TPM,i,"TPM",ObjectCode);
         MySPMPointer->Inquiry(GetEcoDynClassName(), POM,i,"POM",ObjectCode);
         //Debugger(TPM); Debugger(POM);
         PIM = TPM - POM;
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,i,"Chlorophyll to biomass", ObjectCode);
         MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,i,"Phytoplankton biomass",ObjectCode);
         if (MyWaterTemperaturePointer != NULL)
            MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature, i, "Water temperature",ObjectCode);
         PHYORG = phyto_mass / CUBIC * ChlorophyllToBiomass; //mg/l
         DETORG = POM - PHYORG; 
         for (int  bivalve = 0; bivalve < NumberOfIndividuals; bivalve++)
         {
           index = bivalve * NumberOfBoxesWithMytilusGalloprovincialis + i;
           UpdateIndividualParameters(index);
           Inquiry(GetEcoDynClassName(), Eg, index, i,"Eg",ObjectCode);
           ZooIndividualWeight[i] = MyIndividualRecord[index].ZooIndividualWeight;
           for (int k = 0; k < NumberOfClasses; k++)
           {
              /*Debugger(ZooClassBiomass[k * NumberOfBoxes + i]);
              Debugger(ZooClassDensity[k * NumberOfBoxes + i]);
              Debugger(ZooIndividualWeight[k * NumberOfBoxes + i]);  */
              if (
                      ( ZooClassBiomass[k * NumberOfBoxes + i] > 0.0) &&
                      ( ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
                      ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
                  )
              {
                 Feed(i, bivalve);
                 Absorption(i, bivalve);
                 Respire(i, bivalve);
                 Excrete(i, bivalve);
                 Assimilation(i, bivalve);
                 Allocation(i, bivalve);
                 Reproduce(i, bivalve);
                 Aging(i, bivalve);
                 Inquiry(GetEcoDynClassName(), MyBodyDensity, bivalve, i,"Body density",ObjectCode);
                 Inquiry(GetEcoDynClassName(), MyDryWetRatio, bivalve, i,"Dry wet ratio",ObjectCode);
                 /*MyIndividualRecord[index].ZooIndividualWeight = (
                                                                    MyIndividualRecord[index].ZooIndividualVolume * MyBodyDensity
                                                                 ) * MyDryWetRatio;  //g  */
                 MyIndividualRecord[index].ZooIndividualWeight = (
                                                                    MyIndividualRecord[index].ZooIndividualVolume  +
                                                                    MyIndividualRecord[index].ZooIndividualReserves / Eg
                                                                 ) * MyBodyDensity * MyDryWetRatio;
                 //Inquiry(GetEcoDynClassName(), ShapeCoefficient, bivalve, i,"Shape coefficient",ObjectCode);

                 MyIndividualRecord[index].ShellLength = MAX(MyIndividualRecord[index].ShellLength,ShellLength(bivalve, i));
                 //MAX(MyIndividualRecord[index].ShellLength,pow(MyIndividualRecord[index].ZooIndividualVolume,0.333333333) / ShapeCoefficient * 10.0);// mm
                 /*Debugger(MyIndividualRecord[index].ZooIndividualVolume);
                 Debugger(ShapeCoefficient);
                 Debugger(MyIndividualRecord[index].ShellLength); */
                 MyIndividualRecord[index].DSW = 0.00002 * pow(MyIndividualRecord[index].ShellLength, 3.053); //g

                 MyIndividualRecord[index].ConditionIndex = MyIndividualRecord[index].ZooIndividualWeight /
                                                            MyIndividualRecord[index].DSW;
              
                 ZooClassBiomass[NumberOfBoxes * k + i] = ZooClassBiomass[NumberOfBoxes * k + i] +
                                                          MyIndividualRecord[index].ZooIndividualWeight +
                                                          MyIndividualRecord[index].DSW;

              }
           }
         }
      }
   }
}

void TMytilusGalloprovincialisIndividualDEB::Feed(int ABox, int ABivalve)
{
   double const SpFoodMass = 1.0; //{ mg/mm3}
   double Q1, IRmax, Volume_6_cm_mussel, ShapeCoefficient;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   if (TPM > 0.0)
      Q1 = POM / TPM;
   else
      Q1 = 0.0;
   MyIndividualRecord[index].GPT = GutPassageTime(ABox, ABivalve);
   MyIndividualRecord[index].GV = GutVolume(ABox,ABivalve);
   IRmax = (MyIndividualRecord[index].GV * SpFoodMass)/MyIndividualRecord[index].GPT; //{ mg/h}
   Inquiry(GetEcoDynClassName(), ShapeCoefficient, ABivalve, ABox,"Shape coefficient",ObjectCode);
   Volume_6_cm_mussel = pow(ShapeCoefficient*6.0, 3.0);
   if (Option_Clearance_Rate == 1)
        ClearanceRate[ABox] = 3.9 / pow(Volume_6_cm_mussel,0.666666667) * pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667);// 1.253336 L/h/cm2 corresponds to 3.9 L/h/mussel of 6 cm
   else if (Option_Clearance_Rate == 2)
   {
      if (phyto_mass < 1.18)
      {

         ClearanceRate[ABox] = MAX(
                                     ( 7.51 * TPM - 3.01 * pow(TPM,2.0) - 6.86 * Q1 + 2.26) /
                                     pow(Volume_6_cm_mussel,0.666666667) * pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667),
                                     0.0
                                   );
      }
      else
      {
         ClearanceRate[ABox] = 3.9 / pow(Volume_6_cm_mussel,0.666666667) * pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667);// 1.253336 L/h/cm2 corresponds to 3.9 L/h/mussel
      }
   }
   else if (Option_Clearance_Rate == 3){
       ClearanceRate[ABox] = MIN(
                                    3.9 / pow(Volume_6_cm_mussel,0.666666667) * pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667),
                                    MyIndividualRecord[index].GV / ((0.6794 * log(TPM) + 0.8648) * MyIndividualRecord[ABivalve].GPT)
                                 );
   }
   else if (Option_Clearance_Rate == 4)
   {
      if (phyto_mass < 1.18)
      {

         ClearanceRate[ABox] = MAX(
                                     ( 7.51 * TPM - 3.01 * pow(TPM,2.0) - 6.86 * Q1 + 2.26) /
                                     pow(Volume_6_cm_mussel,0.666666667) * pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667),
                                     0.0
                                   );
      }
      else
      {
         ClearanceRate[ABox] = MAX(
                                     (5.13 * TPM - 2.04 * pow(TPM,2.0) + 0.76) / pow(Volume_6_cm_mussel,0.666666667) * pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667),
                                      0.0
                                   );// 1.253336 L/h/cm2 corresponds to 3.9 L/h/mussel
      }
   }

   Inquiry(GetEcoDynClassName(), TA, ABivalve, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, ABivalve, ABox,"T1",ObjectCode);
   ClearanceRate[ABox] = ClearanceRate[ABox] * TemperatureLimitation(TA,T1,AWaterTemperature);
   MyIndividualRecord[index].ZooCR = ClearanceRate[ABox]; //L/h/mussel 
   MyIndividualRecord[index].FRTPM = ClearanceRate[ABox] * TPM;
   MyIndividualRecord[index].IRTPM = MIN(MyIndividualRecord[index].FRTPM,IRmax);//{ mg/h}

   double MySpFoodMass = 0.6794 * log(TPM) + 0.8648;
   IRFlux[ABox] = MyIndividualRecord[index].IRTPM / SpFoodMass;
   Egestion[ABox] = MyIndividualRecord[index].GV / MyIndividualRecord[index].GPT;

   if  (Option_Ingestion_Rate == 1) {
      MyIndividualRecord[index].IRPHY = ClearanceRate[ABox] * PHYORG; //Debugger(MyIndividualRecord[index].IRPHY);
      MyIndividualRecord[index].IRDET = ClearanceRate[ABox] * DETORG; //Debugger(MyIndividualRecord[index].IRTPM);
      MyIndividualRecord[index].IRTPM = MyIndividualRecord[index].IRPHY + MyIndividualRecord[index].IRDET + ClearanceRate[ABox] * (TPM - PHYORG - DETORG);
   }
   else {
      if (TPM > 0.0)
      {
         MyIndividualRecord[index].IRPHY = PHYORG / TPM * MyIndividualRecord[index].IRTPM;
         MyIndividualRecord[index].IRDET = DETORG / TPM * MyIndividualRecord[index].IRTPM;

      }
      else
      {
         MyIndividualRecord[index].IRPHY = 0.0;
         MyIndividualRecord[index].IRDET = 0.0;
      }
   }
   MyIndividualRecord[index].OIR = (MyIndividualRecord[index].IRPHY + MyIndividualRecord[index].IRDET) / CUBIC * DAYSTOHOURS; //{ g /d }

}

void TMytilusGalloprovincialisIndividualDEB::Absorption(int ABox, int ABivalve)
{
   double Q1, CDWFood, NDWFood, PAm;
   double const TINNY = 0.000001;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   MySPMPointer->Inquiry(GetEcoDynClassName(), DETEnergeticContents,ABox,"Det energy contents",ObjectCode);
   MyPhytoPointer->Inquiry(GetEcoDynClassName(), PHYEnergeticContents,ABox,"Phyto energetic contents",ObjectCode);
   if (TPM > 0.0)
      Q1 = POM / TPM;
   else
      Q1 = 0.0;  
   Inquiry(GetEcoDynClassName(), CDWFood, ABivalve, ABox,"CDWFood",ObjectCode);
   Inquiry(GetEcoDynClassName(), NDWFood, ABivalve, ABox,"NDWFood",ObjectCode);
   Inquiry(GetEcoDynClassName(), PAm, ABivalve, ABox,"PAm",ObjectCode);
   if (Q1 > TINNY)
      MyIndividualRecord[index].AE = 0.95-0.18 / Q1; //{Equation derived by Uxio and Maria Jos?}
   else
      MyIndividualRecord[index].AE = 0.0;
   if (MyIndividualRecord[index].IRDET+MyIndividualRecord[index].IRPHY > TINNY)
      FoodEnergeticContents[ABox] = (DETEnergeticContents*MyIndividualRecord[index].IRDET+PHYEnergeticContents*MyIndividualRecord[index].IRPHY) /
                                 (MyIndividualRecord[index].IRDET+MyIndividualRecord[index].IRPHY)*CUBIC; //J/g}
   else
      FoodEnergeticContents[ABox] = 0.0;
  
   if (FoodEnergeticContents[ABox] > TINNY)
      MyIndividualRecord[index].PA = MIN(MyIndividualRecord[index].AE * MyIndividualRecord[index].OIR,
                                       PAm / FoodEnergeticContents[ABox]*pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667));
                                       //food absorption rate - g/d}
   else
      MyIndividualRecord[index].PA = 0.0;

   /*Debugger(DETEnergeticContents); Debugger(PHYEnergeticContents);
   Debugger(MyIndividualRecord[index].IRDET); Debugger(MyIndividualRecord[index].IRPHY);
   Debugger(MyIndividualRecord[index].AE * MyIndividualRecord[index].OIR);
   Debugger(PAm / FoodEnergeticContents[ABox]*pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667));*/

   MyIndividualRecord[index].PA = MAX(MyIndividualRecord[index].PA,0.0);
   MyIndividualRecord[index].ARC = MyIndividualRecord[index].PA * CDWFood;//{ carbon absorption rate - gC/d}
   MyIndividualRecord[index].ARN = MyIndividualRecord[index].PA * NDWFood;//{ ARN - Absorption rate of Nitrogen - gN/d}

   //Debugger(MyIndividualRecord[index].PA); Debugger(FoodEnergeticContents[ABox]);

   MyIndividualRecord[index].PA = MyIndividualRecord[index].PA * FoodEnergeticContents[ABox];   //Jd-1
}

void TMytilusGalloprovincialisIndividualDEB::Assimilation(int ABox, int ABivalve)
{
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   //Inquiry(GetEcoDynClassName(), AOption, ABivalve, ABox,"Option_Respiration",ObjectCode);
   if (Option_Respiration == 1)
     MyIndividualRecord[index].NetProdC = MyIndividualRecord[index].ARC - MyIndividualRecord[index].RespC; //{Respira??o calculada a partir da clorofila e do TPM}
   else
   {
     //Debugger(MyIndividualRecord[index].RespR);
     MyIndividualRecord[index].NetProdC = MyIndividualRecord[index].ARC - MyIndividualRecord[index].RespR;
   }
   MyIndividualRecord[index].NetProdN = MyIndividualRecord[index].ARN - MyIndividualRecord[index].ExcR; //{ Place right hand side of equation here... }
}


void TMytilusGalloprovincialisIndividualDEB::Allocation(int ABox, int ABivalve)
{
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;

   double PAm, K, Em, Eg, PM, MyVolume, MyReserveDensity, Pc;
   Inquiry(GetEcoDynClassName(), PAm, ABivalve, ABox,"PAm",ObjectCode);
   Inquiry(GetEcoDynClassName(), K, ABivalve, ABox,"K",ObjectCode);
   Inquiry(GetEcoDynClassName(), Em, ABivalve, ABox,"Em",ObjectCode);
   Inquiry(GetEcoDynClassName(), Eg, ABivalve, ABox,"Eg",ObjectCode);
   Inquiry(GetEcoDynClassName(), PM, ABivalve, ABox,"PM",ObjectCode);
   Inquiry(GetEcoDynClassName(), TA, ABivalve, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, ABivalve, ABox,"T1",ObjectCode);

   PM = PM * TemperatureLimitation(TA,T1,AWaterTemperature);

   MyVolume = MyIndividualRecord[index].ZooIndividualVolume;
   MyReserveDensity = MyIndividualRecord[index].ZooIndividualReserves / MyVolume;
   MyIndividualRecord[index].ZooIndividualVolume = MyIndividualRecord[index].ZooIndividualVolume +
                                     (
                                        K * PAm * MyIndividualRecord[index].ZooIndividualReserves / MyVolume *
                                        pow(MyIndividualRecord[index].ZooIndividualVolume,0.666666667)/
                                        (Em * (Eg + K * MyIndividualRecord[index].ZooIndividualReserves / MyVolume)) -
                                        MyIndividualRecord[index].RespR  //cm3 d-1
                                     ) * TimeStep / DAYSTOSECONDS;  //cm3 timestep-1

   /*MyIndividualRecord[index].ZooIndividualReserves = MyIndividualRecord[index].ZooIndividualReserves +
                                     (
                                         MyIndividualRecord[index].PA / MyVolume -
                                         PAm * pow(MyVolume,-0.333333333)*
                                         MyIndividualRecord[index].ZooIndividualReserves / Em   //cm3 d-1
                                     ) * TimeStep / DAYSTOSECONDS;//J cm-3 d-1 */

   Pc = (MyIndividualRecord[index].ZooIndividualReserves / MyVolume) / (K * (MyIndividualRecord[index].ZooIndividualReserves / MyVolume) + Eg)*
       ((PAm * Eg / Em) * pow(MyVolume,0.666666667) + PM * MyVolume) ;
   MyIndividualRecord[index].ZooIndividualReserves = MyIndividualRecord[index].ZooIndividualReserves +
                                     (MyIndividualRecord[index].PA - Pc) * TimeStep / DAYSTOSECONDS;//J d-1

}

void TMytilusGalloprovincialisIndividualDEB::Respire(int ABox, int ABivalve)
{
   int index;
   double PM, K, Eg;
   Inquiry(GetEcoDynClassName(), PM, ABivalve, ABox,"PM",ObjectCode);
   Inquiry(GetEcoDynClassName(), K, ABivalve, ABox,"K",ObjectCode);
   Inquiry(GetEcoDynClassName(), Eg, ABivalve, ABox,"Eg",ObjectCode);
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   MyIndividualRecord[index].RespR = PM * MyIndividualRecord[index].ZooIndividualVolume /
                                     (Eg + K * MyIndividualRecord[index].ZooIndividualReserves / MyIndividualRecord[index].ZooIndividualVolume);//cm3 d-1

   Inquiry(GetEcoDynClassName(), TA, ABivalve, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, ABivalve, ABox,"T1",ObjectCode);
   MyIndividualRecord[index].RespR = MyIndividualRecord[index].RespR * TemperatureLimitation(TA,T1,AWaterTemperature);

   /*Debugger(PM);
   Debugger(MyIndividualRecord[index].ZooIndividualVolume);
   Debugger(Eg);
   Debugger(K);
   Debugger(MyIndividualRecord[index].ZooIndividualReserves);
   Debugger(MyIndividualRecord[index].RespR);  */
}

void TMytilusGalloprovincialisIndividualDEB::Excrete(int ABox, int ABivalve)
{
  
}

void TMytilusGalloprovincialisIndividualDEB::Reproduce(int ABox, int ABivalve)
{
   int index;
   double MyReproductionDate;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   Inquiry(GetEcoDynClassName(), MyReproductionDate, ABivalve, ABox,"ReproductionDate",ObjectCode);
   if (MyPEcoDynClass->GetJulianDay() == MyReproductionDate)
   {
      double MyReproductionExpenditure;
      Inquiry(GetEcoDynClassName(), MyReproductionExpenditure, ABivalve, ABox,"ReproductionExpenditure",ObjectCode);
      MyIndividualRecord[index].ZooIndividualReserves = MyIndividualRecord[index].ZooIndividualReserves -
                                                              MyReproductionExpenditure * MyIndividualRecord[index].ZooIndividualReserves *
                                                              TimeStep / DAYSTOSECONDS;

   }
}


double TMytilusGalloprovincialisIndividualDEB::ShellLength(int ABivalve, int ABox)
{
   double ShapeCoefficient;
   int index;
   index = ABivalve * NumberOfBoxesWithMytilusGalloprovincialis + ABox;
   Inquiry(GetEcoDynClassName(), ShapeCoefficient, ABivalve, ABox,"Shape coefficient",ObjectCode);
   return pow(MyIndividualRecord[index].ZooIndividualVolume,0.333333333) / ShapeCoefficient * 10.0;// mm
}


double TMytilusGalloprovincialisIndividualDEB::TemperatureLimitation(double ATA, double AT1, double AWaterTemperature)
{
   double MyTA, MyT1,MyWaterTemperature;
   //Debugger(ATA); Debugger(AT1); Debugger(AWaterTemperature); 
   if ((AT1 > 0.0) && (AWaterTemperature > 0.0))
   {
      MyTA = ATA;
      MyT1 = AT1;
      MyWaterTemperature = AWaterTemperature + ABSOLUTEZERO;
      return exp(MyTA / MyT1 - MyTA / MyWaterTemperature);
   }
   else
      return 1.0;
}



 

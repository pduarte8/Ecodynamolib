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

TFish::TFish(TEcoDynClass* APEcoDynClass, char* className): TEcoDynClass()
{
    MyPEcoDynClass = APEcoDynClass;
    BuildFish(className);
}

void TFish::BuildFish(char* className)
{
    TINNY = 0.0000000001;
    BoxArray = MyPEcoDynClass->BoxArray;
    strcpy(EcoDynClassName, className);
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
    TimeStep = MyPEcoDynClass->GetTimeStep();
    if (NumberOfBoxes > 1)
#ifdef __BORLANDC__
        MessageBox(0, "TFishDEB1 constructor - model must be 0D", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TFish::BuildFish - model must be 0D" << endl;
#endif  // __BORLANDC__
    NumberOfIndividuals = 0;
    OxygenToJoules = 13.84; //J/mgO2
    FoodLoad = 0.0;
    FoodDecayRate = 0.0;
    F = 0.0;
    AbsorptionEfficiency = false;
    ObjectCode = FISHOBJECTCODE;
}

TFish::~TFish()
{
    freeMemory();
}

void TFish::freeMemory()
{
   /*if (NumberOfBoxes > 0)
   {
      delete [] FishBiomass;
      Debugger(1);
      delete [] FishDensity;
      Debugger(2);
      delete [] MyIndividualRecord;
      Debugger(3);
      delete [] MyParsRecord;
      Debugger(4);
   } */
   //Debugger(NumberOfParameters); Debugger(NumberOfIndividuals); Debugger(NumberOfBoxes);
   try {
        if (NumberOfParameters > 0)
            delete [] MyParsRecord;
        if (NumberOfIndividuals > 0)
            delete [] MyIndividualRecord;
        if (NumberOfBoxes > 0)
        {
           delete [] FishBiomass;
           delete [] FishDensity;
           delete [] TotalIngestion;
           delete [] TotalAbsorption;
           delete [] TotalRespiration;
        }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        Application->ShowException(&e);
    }
    catch (Exception &exception) {
        Application->ShowException(&exception);
    }
#else  // __BORLANDC__
    catch (exception &exc) {
        cerr << "TFish::freeMemory - " << exc.what() << endl;
    }
#endif  //  __BORLANDC__
}

double TFish::GetParameterValue(char* parmName)
{

}

bool TFish::SetParameterValue(char* parmName, double value)
{

}

void TFish::Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int ClassNumber,int AnObjectCode)
{

}

void TFish::Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int AnObjectCode)
{

}

bool TFish::SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName)
{
}

bool TFish::SaveVariables()
{
}

bool TFish::SaveParameters()
{
}


void TFish::BuildFish()
{
    ReadParameters();
    PreBuildFish();
    PostBuildFish();
    ReadVariables();
}

void TFish::ReadParameters()
{

}

bool TFish::ReadParsRecord(int X, int i, int index)
{
    //O record de parâmetros só será preenchido devidamente se para todos os parâmetros
    //existirem valores para os limites inferior e superior e para a flag que determina
    //se a geração de parâmetros é aleatória ou não (boolenana que pode assumir valor 0 ou 1
    //Se uma destas condições não se verificar, o modelo usará parâmetros iguais para todos os
    //indíviduos

    MyPReadWrite->ReadNumber(X+5, i, MyParsRecord[index].HigherValue);
    MyPReadWrite->ReadNumber(X+6, i, MyParsRecord[index].LowerValue);
    MyPReadWrite->ReadBool(X+7, i, MyParsRecord[index].RandomizeFlag);
    {
        NumberOfParameterRecords++;
        return true;
    }
    return false;
}


void TFish::ReadVariables()
{
    // Read in the variables
    char MyVariable[65];
    //TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Fish");
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile(MyFishName);
    if (PReadWrite == NULL)
#ifdef __BORLANDC__
       MessageBox(0, "Fish variables file missing.", MyFishName, MB_OK);
#else  // __BORLANDC__
            cerr << "TFish::ReadVariables - Fish variables file missing" << endl;
#endif  // __BORLANDC__
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
        else
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object in Fish", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TFish::ReadVariables - undefined object in Fish" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    } 
}

void TFish::PreBuildFish()
{
   AWaterTemperature = 0.0;
   FishBiomass = new double[NumberOfBoxes];
   FishDensity = new double[NumberOfBoxes];

   MyIndividualRecord = new IndividualRecord[NumberOfIndividuals];
   TotalIngestion = new double[NumberOfBoxes];
   TotalAbsorption = new double[NumberOfBoxes];
   TotalRespiration = new double[NumberOfBoxes];
   /*for (int  i = 0; i < NumberOfBoxes; i++)
   {
      FishBiomass[i] = 0.0;
      FishDensity[i] = (double)NumberOfIndividuals;
      TotalIngestion[i] = 0.0;
      TotalAbsorption[i] = 0.0;
      TotalRespiration[i] = 0.0;
   } */
    FishBiomass[0] = 0.0;
    FishDensity[0] = (double)NumberOfIndividuals;
    TotalIngestion[0] = 0.0;
    TotalAbsorption[0] = 0.0;
    TotalRespiration[0] = 0.0;
}

void TFish::PostBuildFish()
{
    randomize();
    for (int  i = 0; i < NumberOfIndividuals; i++)
    {
        MyIndividualRecord[i].AE = 0.0; MyIndividualRecord[i].PA = 0.0; MyIndividualRecord[i].Egestion = 0.0;
        MyIndividualRecord[i].Volume = 0.0; MyIndividualRecord[i].Weight = 0.0;
        MyIndividualRecord[i].Reserves = 0.0; MyIndividualRecord[i].Length = 0.0;
        MyIndividualRecord[i].Age = 0.0; MyIndividualRecord[i].Jx = 0.0; MyIndividualRecord[i].RespR = 0.0;
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

void TFish::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    int i,j;
    char MyParameter[65], // 64 characters (max) for parameter name
    Myjstring[5],
    MyName[65];
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

    for (i = 0; i < NumberOfVariables; i++)
    {
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
         if (strcmp(MyParameter, "Fish density") == 0)
                  Value = FishDensity[0];
         else if (strcmp(MyParameter, "Fish biomass") == 0)
                  Value = FishBiomass[0];
         else if (strcmp(MyParameter, "F") == 0)
                  Value = F;
         else if (strcmp(MyParameter, "TotalIngestion") == 0)
                  Value = TotalIngestion[0];
         else if (strcmp(MyParameter, "TotalAbsorption") == 0)
                  Value = TotalAbsorption[0];
         else if (strcmp(MyParameter, "TotalRespiration") == 0)
                  Value = TotalRespiration[0];
         LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
         return;
       }
    }
#ifdef __BORLANDC__
    char Caption[129];
    strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
    MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 2",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TFish::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    Value = 0;
}


void TFish::Inquiry(char* srcName, double &Value,int FishNumber, int BoxNumber,
					char* ParameterName,int AnObjectCode)
{
    int i,j, index;
    char MyParameter[65], // 64 characters (max) for parameter name
    Myjstring[5];
    int MyFishNumber = FishNumber;
    int MyBoxNumber = BoxNumber;
    index = MyFishNumber;
    strcpy(MyParameter, ParameterName);
    //Debugger(NumberOfParameterRecords); Debugger(NumberOfVariables);
    //Procura da string entre as variáveis

    for (i = 0; i < NumberOfVariables; i++)
    {
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
        if (strcmp(MyParameter, "Fish body volume") == 0){
                Value = MyIndividualRecord[index].Volume;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "Fish reserve energy") == 0){
                Value = MyIndividualRecord[index].Reserves;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "Fish length") == 0){
                Value = MyIndividualRecord[index].Length;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "Fish age") == 0){
                Value = MyIndividualRecord[index].Age;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "Fish wet weight") == 0){
                Value = MyIndividualRecord[index].Weight;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "Jx") == 0){
                Value = MyIndividualRecord[index].Jx;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "PA") == 0){
                Value = MyIndividualRecord[index].PA;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "Egestion") == 0){
                Value = MyIndividualRecord[index].Egestion;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if (strcmp(MyParameter, "RespR") == 0){
                Value = MyIndividualRecord[index].RespR;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
        }
        else if ((strcmp(MyParameter, "AE") == 0) && (!AbsorptionEfficiency)) {
                Value = MyIndividualRecord[index].AE;
                LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
                return;
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
      }
    }  //i
    //Fim de procura da string entre as variáveis
    //Debugger(1);
    //Procura da string entre os parâmetros
    if (strcmp(MyParameter, "JXm") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "JXm") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "Xk") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "Xk") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "AE") == 0)
    {
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            if (strcmp(MyIndividualRecord[index].IndividualParameters[j].ParameterName, "AE") == 0){
            Value = MyIndividualRecord[index].IndividualParameters[j].Value;
            LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber); return; }
        }
    }
    else if (strcmp(MyParameter, "PAm") == 0)
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
    //Fim da procura da string entre os parâmetros
    char Caption[129];
    strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
#ifdef __BORLANDC__    
    MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TFish::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
}

void TFish::Integrate()
{

}

void TFish::Reinitialize()
{

}

VNA* TFish::Inquiry(char* srcName,int &Individuals, int &DimVariables)
{
    VNA* MyVarNameArray;

    Individuals = NumberOfIndividuals;
    DimVariables = IndividualBasedVariables;
    MyVarNameArray = new VNA[IndividualBasedVariables];
    for (int i = 0; i < IndividualBasedVariables; i++)
    {
        strcpy(MyVarNameArray[i], VariableNameArray[i]);
    }
    LogMessage("Inquiry", srcName, MyVarNameArray[0], Individuals, DimVariables);
    return MyVarNameArray;
}

void TFish::Go()
{
   JulianDay = MyPEcoDynClass->GetJulianDay();
   AWaterTemperature = 20.0;
   double MyBodyDensity, MyDryWetRatio, Eg, PM;
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if ((MySPMPointer != NULL))
   {
      for (int i = 0; i < NumberOfBoxes; i++)
      {
         FishBiomass[i] = 0.0; TotalIngestion[i] = 0.0; TotalAbsorption[i] = 0.0; TotalRespiration[i] = 0.0;
      }
      for (int i = 0; i < NumberOfBoxes; i++)
      {
         MySPMPointer->Inquiry(GetEcoDynClassName(), FoodLoad,i,"Food",ObjectCode);

         //Debugger(FoodLoad);
         if (MyWaterTemperaturePointer != NULL)
            MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature, i, "Water temperature",ObjectCode);
         //Debugger(AWaterTemperature);
         //TotalFishMass = 0.0;
         for (int  fish = 0; fish < NumberOfIndividuals; fish++)
         {
            //UpdateIndividualParameters(index);
            //Debugger(FishDensity[i]);
            if (
                    /*( FishBiomass[i] > 0.0) &&*/ ( FishDensity[i] > 0.0)
                )
            {
               Inquiry(GetEcoDynClassName(), MyBodyDensity, fish, i,"Body density",ObjectCode);
               //FoodLoadPerFish = FoodLoad * CUBIC / (FishDensity[i] * MyIndividualRecord[fish].Volume * MyBodyDensity); //g of food per g of fish per day or mg of food per fish
               Feed(i, fish);
               Absorption(i, fish);
               Egestion(i, fish);
               Respire(i, fish);
               Excrete(i, fish);
               Assimilation(i, fish);
               Allocation(i, fish);
               Reproduce(i, fish);
               Aging(i, fish);
               Inquiry(GetEcoDynClassName(), MyDryWetRatio, fish, i,"Dry wet ratio",ObjectCode);
               Inquiry(GetEcoDynClassName(), Eg, fish, i,"Eg",ObjectCode);
               /*MyIndividualRecord[fish].Weight = (
                                                         MyIndividualRecord[fish].Volume * MyBodyDensity
                                                      ) * MyDryWetRatio;  //g  */
               MyIndividualRecord[fish].Weight = MyIndividualRecord[fish].Volume * MyBodyDensity + MyIndividualRecord[fish].Reserves / Eg * MyBodyDensity/*EnergyPerReserveMass*/;
               MyIndividualRecord[fish].Length = FishLength(fish);
               FishBiomass[i] = FishBiomass[i] + MyIndividualRecord[fish].Weight;
               TotalIngestion[i] = TotalIngestion[i] + MyIndividualRecord[fish].Jx;
               TotalAbsorption[i] = TotalAbsorption[i] + MyIndividualRecord[fish].PA;
               Inquiry(GetEcoDynClassName(), PM, fish, i,"PM",ObjectCode);   //J/cm3
               TotalRespiration[i] = TotalRespiration[i] + PM * MyIndividualRecord[fish].Volume / OxygenToJoules; //mg O2 d-1 por todos os peixes
            }
         }
      }
   }
}

void TFish::Feed(int ABox, int AFish)
{
   double Xk, JXm, TA, T1;
   Inquiry(GetEcoDynClassName(), Xk, AFish, ABox,"Xk",ObjectCode);
   Inquiry(GetEcoDynClassName(), JXm, AFish, ABox,"JXm",ObjectCode);
   Inquiry(GetEcoDynClassName(), TA, AFish, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, AFish, ABox,"T1",ObjectCode);

   if (FoodDecayRate <= 0.0)
      F = FoodLoad / (Xk + FoodLoad);
   else
      F = (log(FoodLoad + Xk) - log(FoodLoad * exp(-FoodDecayRate * TimeStep) + Xk)) /
          (FoodDecayRate * TimeStep);
   MyIndividualRecord[AFish].Jx = JXm * F * pow(MyIndividualRecord[AFish].Volume, 0.666666667) * TemperatureLimitation(TA, T1, AWaterTemperature); //J d-1
   //Debugger(F); Debugger(Xk); Debugger(FoodLoadPerFish);  Debugger(JXm); Debugger(MyIndividualRecord[AFish].Jx);
}

void TFish::Absorption(int ABox, int AFish)
{
   double JXm, PA, PAm;
   Inquiry(GetEcoDynClassName(), JXm, AFish, ABox,"JXm",ObjectCode);
   Inquiry(GetEcoDynClassName(), PAm, AFish, ABox,"PAm",ObjectCode);

   if (!AbsorptionEfficiency)
   {
     if (JXm > TINNY)
     {
        MyIndividualRecord[AFish].AE = PAm / JXm;
        AE = MyIndividualRecord[AFish].AE;
     }
     else
#ifdef __BORLANDC__
        MessageBox(0, "TFish::Absorption: JXm to small...", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TFish::Absorption - JXm to small..." << endl;
#endif  // __BORLANDC__
   }
   else
   {
      Inquiry(GetEcoDynClassName(), AE, AFish, ABox,"AE",ObjectCode);
      //Debugger(AE);
   }
   PA = MyIndividualRecord[AFish].Jx * AE; //J d-1
   MyIndividualRecord[AFish].PA = PA;
   //Debugger(PA);
}

void TFish::Assimilation(int ABox, int AFish)
{

}

void TFish::Allocation(int ABox, int AFish)
{
   int index;
   index = AFish;

   double PAm, K, Em, Eg, PM, TA, T1, MyVolume, MyReserveDensity, Pc;
   Inquiry(GetEcoDynClassName(), PAm, AFish, ABox,"PAm",ObjectCode);
   Inquiry(GetEcoDynClassName(), K, AFish, ABox,"K",ObjectCode);
   Inquiry(GetEcoDynClassName(), Em, AFish, ABox,"Em",ObjectCode);
   Inquiry(GetEcoDynClassName(), Eg, AFish, ABox,"Eg",ObjectCode);
   Inquiry(GetEcoDynClassName(), PM, AFish, ABox,"PM",ObjectCode);
   Inquiry(GetEcoDynClassName(), TA, AFish, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, AFish, ABox,"T1",ObjectCode);

   //PM = PM * TemperatureLimitation(TA,T1,AWaterTemperature);

   MyVolume = MyIndividualRecord[index].Volume;
   MyReserveDensity = MyIndividualRecord[index].Reserves / MyVolume;
   MyIndividualRecord[index].Volume = MyIndividualRecord[index].Volume +
                                     (
                                        K * PAm * MyIndividualRecord[index].Reserves / MyVolume *
                                        pow(MyIndividualRecord[index].Volume,0.666666667)/
                                        (Em * (Eg + K * MyIndividualRecord[index].Reserves / MyVolume)) *
                                        TemperatureLimitation(TA,T1,AWaterTemperature)-
                                        MyIndividualRecord[index].RespR  //cm3 d-1
                                     ) * TimeStep / DAYSTOSECONDS;  //cm3 TimeStep-1


   Pc = (MyIndividualRecord[index].Reserves / MyVolume) / (K * (MyIndividualRecord[index].Reserves / MyVolume) + Eg)*
       ((PAm * Eg / Em) * pow(MyVolume,0.666666667) + PM * MyVolume) * TemperatureLimitation(TA,T1,AWaterTemperature); //J d-1
   MyIndividualRecord[index].Reserves = MyIndividualRecord[index].Reserves +
                                     (MyIndividualRecord[index].PA - Pc) * TimeStep / DAYSTOSECONDS;//J

}

void TFish::Respire(int ABox, int AFish)
{
   int index;
   double PM, K, Eg, TA, T1, MyVolume;
   index = AFish;
   Inquiry(GetEcoDynClassName(), PM, AFish, ABox,"PM",ObjectCode);   //J/cm3
   Inquiry(GetEcoDynClassName(), K, AFish, ABox,"K",ObjectCode);
   Inquiry(GetEcoDynClassName(), Eg, AFish, ABox,"Eg",ObjectCode);
   MyVolume = MyIndividualRecord[index].Volume;
   MyIndividualRecord[index].RespR = PM * MyVolume /
                                     (Eg + K * MyIndividualRecord[index].Reserves / MyVolume);//cm3 d-1
   Inquiry(GetEcoDynClassName(), TA, AFish, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, AFish, ABox,"T1",ObjectCode);
   MyIndividualRecord[index].RespR = MyIndividualRecord[index].RespR * TemperatureLimitation(TA,T1,AWaterTemperature);
}

void TFish::Excrete(int ABox, int AFish)
{

}

void TFish::Reproduce(int ABox, int AFish)
{

}

double TFish::FishLength(int AFish)
{
   double MyShapeCoefficient;
   Inquiry(GetEcoDynClassName(), MyShapeCoefficient, AFish, 0,"Shape coefficient",ObjectCode);
   return pow(MyIndividualRecord[AFish].Volume,0.333333333)/MyShapeCoefficient;
}


double TFish::TemperatureLimitation(double ATA, double AT1, double AWaterTemperature)
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

void TFish::Aging(int ABox, int AFish)
{
   int index;
   MyIndividualRecord[AFish].Age = MyIndividualRecord[AFish].Age + TimeStep / DAYSTOSECONDS;
}
















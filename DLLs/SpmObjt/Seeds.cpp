/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <exception>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"
#include "iodll.h"


TSeeds::TSeeds(TEcoDynClass* APEcoDynClass, char* className)
            :TSPMBasic(APEcoDynClass, className)
{
    BoxArray = MyPEcoDynClass->BoxArray;
    NumberOfTypes = 0;
    TimeOfPositiveBuoyancy = 0.0;
    SettlingVelocity = 0.0;
    NumberOfBoxesWithBenthicSpecies = 0;
    NumberOfBoxesWithSeeds = 0;
    CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();
    BuildSeedsRecord();
    BuildSeeds();
}

void TSeeds::BuildSeedsRecord()
{
    bool BenthicRecord;
    BenthicRecord = MyPEcoDynClass->HasABenthicSpeciesRecord();
    if (!BenthicRecord)
    {
        if (MyPEcoDynClass->OpenBenthicSpecies(MyPEcoDynClass->GetBenthicSpeciesFile()))
        BenthicRecord = true;
    }
    if (BenthicRecord)
    {
        MyBenthicSpeciesRecord = MyPEcoDynClass->GetABenthicSpeciesRecord();
        NumberOfBoxesWithBenthicSpecies = MyPEcoDynClass->GetNumberOfCellsWithBenthicSpecies();
    }
    else
#ifdef __BORLANDC__
        MessageBox(0,"Benthic species record not initialized",
              "EcoDynamo alert in TSeeds::BuildSeeds",
              MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TSeeds::BuildSeedsRecord - Benthic species record not initialized" << endl;
#endif  // __BORLANDC__

    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
       if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Zostera noltii seeds") == 0)
       NumberOfBoxesWithSeeds++;
    }
    SeedsRecord = new BenthicSpeciesRecord[NumberOfBoxesWithSeeds];
    int AIndex = 0;
    //Debugger(NumberOfBoxesWithBenthicSpecies); Debugger(NumberOfBoxesWithSeeds);
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Zostera noltii seeds") == 0)
        {
            strcpy(SeedsRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
            SeedsRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
            SeedsRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
            //Debugger(SeedsRecord[AIndex].LineCoordinate); Debugger(SeedsRecord[AIndex].ColumnCoordinate);
            AIndex++;
        }
    }
}

void TSeeds::BuildSeeds()
{
    int X, Y, XV, YV, Index, TypesTimesBoxes;
    TypesTimesBoxes = 0;
    double MyValue;
    // Read in the parameters
    ParameterCount = 0;
    TypesPerBox = new int[NumberOfBoxes];

    for (int i = 0; i < NumberOfBoxes; i++)
       TypesPerBox[i] = 0;

    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
       MessageBox(0, "SuspendedMatter parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TSeeds::BuildSeeds - Parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        if (PReadWrite->FindString("Seeds", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "NumberOfTypes") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfTypes);
                    //Debugger(NumberOfTypes);
                }
                else if (strcmp(MyParameter, "TimeOfPositiveBuoyancy") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TimeOfPositiveBuoyancy);
                    //Debugger(TimeOfPositiveBuoyancy);
                }
                else if (strcmp(MyParameter, "SettlingVelocity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SettlingVelocity);
                    //Debugger(SettlingVelocity);
                }
                else if (strcmp(MyParameter, "TypesPerBox") == 0)
                {
                    for (int j = 0; j < NumberOfBoxesWithSeeds; j++)
                    {
                        Index = SeedsRecord[j].LineCoordinate * NumberOfColumns + SeedsRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(X+3, i + j, MyValue);
                        TypesPerBox[Index] = MyValue;
                        TypesTimesBoxes = TypesTimesBoxes + MyValue;
                        //Debugger(TypesTimesBoxes);
                    }
                }

            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Suspended matter/Seeds", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSeeds::BuildSeeds - Parameters: Undefined object - Suspended matter/Seeds" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    SeedType = new int[NumberOfTypes];
    CurrentPosition = new int[NumberOfBoxes * NumberOfTypes];
    SeedNumberPerTypeInWater = new double[NumberOfBoxes * NumberOfTypes];
    SeedNumberPerTypeSettled = new double[NumberOfBoxes * NumberOfTypes];
    SeedNumberPerBox = new double[NumberOfBoxes];
    SeedAge = new double[NumberOfBoxes * NumberOfTypes];
    GerminationStatus = new bool[NumberOfBoxes * NumberOfTypes];
    SeedInWaterFlux = new double[NumberOfBoxes * NumberOfTypes];
    SeedSettlingFlux = new double[NumberOfBoxes * NumberOfTypes];
    SeedLoad = new double[NumberOfBoxes];
    Seeds = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
       SeedNumberPerBox[i] = 0.0;
       SeedLoad[i] = 0.0;
       Seeds[i] = 0.0;
    }
    for (int i = 0; i < NumberOfTypes; i++)
       SeedType[i] = 0;
    for (int i = 0; i < NumberOfTypes; i++)
    {
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            CurrentPosition[i * NumberOfBoxes + j] = 0;
            SeedNumberPerTypeInWater[i * NumberOfBoxes + j] = 0.0;        //Number of seeds in the water columns per type per model box
            SeedNumberPerTypeSettled[i * NumberOfBoxes + j] = 0.0; //Number of seeds in the sediments per type per model box
            SeedAge[i * NumberOfBoxes + j] = 0.0;                  //Seed age per type per md
            GerminationStatus[i * NumberOfBoxes + j] = false;
            SeedInWaterFlux[i * NumberOfBoxes + j] = 0.0;
            SeedSettlingFlux[i * NumberOfBoxes + j] = 0.0;
        }
    }
    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "SuspendedMatter variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSeeds::BuildSeeds - Variables file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        if (PReadWrite->FindString("Seeds", X, Y))
        {
            char MyVariable[65];
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables * NumberOfTypes];
            //Debugger(NumberOfVariables * NumberOfTypes);
            // read in the variable names
            int k, j, InitialValue, FinalValue, CheckSum;
            CheckSum = 0;
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN + i, Y, MyVariable);
                //if (PReadWrite->FindString("SeedNumberPerTypeInWater", XV, YV))
                if (strcmp(MyVariable, "SeedNumberPerTypeInWater") == 0)
                {
                    InitialValue = 0;  FinalValue = 0;
                    for (j = 0; j < NumberOfBoxesWithSeeds; j++)
                    {
                        Index = SeedsRecord[j].LineCoordinate * NumberOfColumns + SeedsRecord[j].ColumnCoordinate;
                        FinalValue = FinalValue + TypesPerBox[Index];
                        for (k = InitialValue; k < FinalValue; k++)
                        {
                           //Debugger(k);
                           sprintf(MyVariable, "SeedNumberPerTypeInWater%i", k + 1);
                           strcpy(VariableNameArray[k + i * NumberOfTypes],MyVariable);
                           //MessageBox(0,VariableNameArray[k + i * NumberOfVariables] , "SeedNumberPerTypeInWater", MB_OK);
                           SeedType[k] = k;
                           PReadWrite->ReadNumber(X + VARIABLENAMECOLUMN + i, Y + 1 + k, MyValue); //Debugger(MyValue);
                           SeedNumberPerTypeInWater[k * NumberOfBoxes + Index] = MyValue;
                           SeedNumberPerBox[Index] = SeedNumberPerBox[Index] + SeedNumberPerTypeInWater[k * NumberOfBoxes + Index];
                           SeedNumberPerTypeInWater[k * NumberOfBoxes + Index] = SeedNumberPerTypeInWater[k * NumberOfBoxes + Index]
                                                                                 / MyPEcoDynClass->GetBoxVolume(Index); //Seeds in the water expressed as seeds/m3
                           /*if ((SeedNumberPerTypeInWater[k * NumberOfBoxes + Index]>0.0) && (Index == 28224))
                           {
                              Debugger(k);
                              Debugger(MyPEcoDynClass->GetBoxVolume(Index));
                              Debugger(SeedNumberPerTypeInWater[k * NumberOfBoxes + Index]);
                           } */
                           CurrentPosition[k * NumberOfBoxes + Index] = 1;
                           CheckSum++;
                        }
                        InitialValue = InitialValue + TypesPerBox[Index];
                    }
                    if (CheckSum != TypesTimesBoxes) {
#ifdef __BORLANDC__
                       MessageBox(0, "Error 1 in TSeeds::BuildSeeds - Check TypesTimesBoxes and SeedNumberPerTypeInWater data", "EcoDynamo alert", MB_OK);
                       Debugger(CheckSum); Debugger(TypesTimesBoxes);
#else  // __BORLANDC__
												cerr << "TSeeds::BuildSeeds - Error 1 in TSeeds::BuildSeeds  - Check TypesTimesBoxes (" << TypesTimesBoxes << ") and SeedNumberPerTypeInWater (" << CheckSum << ") data" << endl;
#endif  // __BORLANDC__
                    }
                    CheckSum = 0;
                }
                else
                //if (PReadWrite->FindString("SeedNumberPerTypeSettled", XV, YV))
                if (strcmp(MyVariable, "SeedNumberPerTypeSettled") == 0)
                {
                    InitialValue = 0;  FinalValue = 0;
                    for (j = 0; j < NumberOfBoxesWithSeeds; j++)
                    {
                        Index = SeedsRecord[j].LineCoordinate * NumberOfColumns + SeedsRecord[j].ColumnCoordinate;
                        FinalValue = FinalValue + TypesPerBox[Index];
                        for (k = InitialValue; k < FinalValue; k++)
                        {
                           //Debugger(k);
                           sprintf(MyVariable, "SeedNumberPerTypeSettled%i", k + 1);
                           strcpy(VariableNameArray[k + i * NumberOfTypes],MyVariable);
                           //MessageBox(0,VariableNameArray[k + i * NumberOfVariables] , "SeedNumberPerTypeSettled", MB_OK);
                           PReadWrite->ReadNumber(X + VARIABLENAMECOLUMN + i, Y + 1 + k, MyValue); //Debugger(MyValue);
                           SeedNumberPerTypeSettled[k * NumberOfBoxes + Index] = MyValue;
                           SeedNumberPerBox[Index] = SeedNumberPerBox[Index] + SeedNumberPerTypeSettled[k * NumberOfBoxes + Index];
                           SeedNumberPerTypeSettled[k * NumberOfBoxes + Index] = SeedNumberPerTypeSettled[k * NumberOfBoxes + Index]
                                                                                 / MyPEcoDynClass->GetBoxArea(Index); //Seeds in the sediments expressed as seeds/m2
                           CheckSum++;
                        }
                        InitialValue = InitialValue + TypesPerBox[Index];
                    }
#ifdef __BORLANDC__
                    if (CheckSum != TypesTimesBoxes)
                       MessageBox(0, "Error 2 in TSeeds::BuildSeeds  - Check TypesPerBox and SeedNumberPerTypeSettled data", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
												cerr << "TSeeds::BuildSeeds - Error 2 in TSeeds::BuildSeeds  - Check TypesPerBox and SeedNumberPerTypeSettled data" << endl;
#endif  // __BORLANDC__
                    CheckSum = 0;
                }
                else
                if (strcmp(MyVariable, "SeedAge") == 0)
                //if (PReadWrite->FindString("SeedAge", XV, YV))
                {
                    InitialValue = 0;  FinalValue = 0;
                    for (j = 0; j < NumberOfBoxesWithSeeds; j++)
                    {
                        Index = SeedsRecord[j].LineCoordinate * NumberOfColumns + SeedsRecord[j].ColumnCoordinate;
                        FinalValue = FinalValue + TypesPerBox[Index];
                        for (k = InitialValue; k < FinalValue; k++)
                        {
                           //Debugger(k);
                           sprintf(MyVariable, "SeedAge%i", k + 1);
                           strcpy(VariableNameArray[k + i * NumberOfTypes],MyVariable);
                           //MessageBox(0,VariableNameArray[k + i * NumberOfVariables] , "SeedAge", MB_OK);
                           PReadWrite->ReadNumber(X + VARIABLENAMECOLUMN + i, Y + 1 + k, MyValue); //Debugger(MyValue);
                           SeedAge[k * NumberOfBoxes + Index] = MyValue;
                           CheckSum++;
                        }
                        InitialValue = InitialValue + TypesPerBox[Index];
                    }
                    if (CheckSum != TypesTimesBoxes)
#ifdef __BORLANDC__
                       MessageBox(0, "Error 2 in TSeeds::BuildSeeds  - Check TypesPerBox and SeedAge data", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
												cerr << "TSeeds::BuildSeeds - Error 2 in TSeeds::BuildSeeds  - Check TypesPerBox and SeedAge data" << endl;
#endif  // __BORLANDC__
                    CheckSum = 0;
                }
                else
                if (strcmp(MyVariable, "GerminationStatus") == 0)
                //if (PReadWrite->FindString("GerminationStatus", XV, YV))
                {
                    InitialValue = 0;  FinalValue = 0;
                    for (j = 0; j < NumberOfBoxesWithSeeds; j++)
                    {
                        Index = SeedsRecord[j].LineCoordinate * NumberOfColumns + SeedsRecord[j].ColumnCoordinate;
                        FinalValue = FinalValue + TypesPerBox[Index];
                        for (k = InitialValue; k < FinalValue; k++)
                        {
                           //Debugger(k);
                           sprintf(MyVariable, "GerminationStatus%i", k + 1);
                           strcpy(VariableNameArray[k + i * NumberOfTypes],MyVariable);
                           //MessageBox(0,VariableNameArray[k + i * NumberOfVariables] , "GerminationStatus", MB_OK);
                           PReadWrite->ReadNumber(X + VARIABLENAMECOLUMN + i, Y + 1 + k, MyValue); //Debugger(MyValue);
                           GerminationStatus[k * NumberOfBoxes + Index] = false;
                           CheckSum++;
                        }
                        InitialValue = InitialValue + TypesPerBox[Index];
                    }
                    if (CheckSum != TypesTimesBoxes)
#ifdef __BORLANDC__
                       MessageBox(0, "Error 2 in TSeeds::BuildSeeds  - Check TypesPerBox and SeedAge data", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
												cerr << "TSeeds::BuildSeeds - Error 2 in TSeeds::BuildSeeds  - Check TypesPerBox and SeedAge data" << endl;
#endif  // __BORLANDC__

                    CheckSum = 0;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object in TSeeds", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSeeds::BuildSeeds - Variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
        NumberOfVariables = NumberOfVariables * NumberOfTypes;
        //Debugger(NumberOfVariables);
    }
}


TSeeds::~TSeeds()
{
    freeMemory();
}

void TSeeds::freeMemory()
{
    try {
        if (NumberOfTypes > 0)
            delete [] SeedType;
        if (NumberOfBoxes > 0)
        {
            delete [] SeedNumberPerBox;
            delete [] TypesPerBox;
            delete [] SeedLoad;
            delete [] Seeds;
        }
        if ((NumberOfTypes > 0) && (NumberOfBoxes > 0))
        {
            delete [] CurrentPosition;
            delete [] SeedNumberPerTypeInWater;
            delete [] SeedNumberPerTypeSettled;
            delete [] SeedAge;
            delete [] GerminationStatus;
            delete [] SeedInWaterFlux;
            delete [] SeedSettlingFlux;
        }
        if (NumberOfBoxesWithBenthicSpecies > 0)
           delete [] MyBenthicSpeciesRecord;
        if (NumberOfBoxesWithSeeds > 0)
            delete [] SeedsRecord;
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
				cerr << "TSeeds::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TSeeds::Go()
{
  int index; double ABoxDepth;
  TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
  SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
  JulianDay = MyPEcoDynClass->GetJulianDay();
  MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
  for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
  {
     index = pSubDomain->BoxNumber[k];
     if (MyTransportPointer != NULL)
     {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,index,"Box depth",ObjectCode);
     }
     else
        ABoxDepth = MyPEcoDynClass->GetBoxDepth(index);
     if (
            //(ABoxDepth > CriticalWaterDepth) &&
            (BoxArray[index].Dummybool1) &&
            (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) && //No western ocean boundary
            (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) && //No eastern ocean boundary
            (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&  //No southern ocean boundary
            (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2)    //No northern ocean boundary
        )
     {
       for (int Type = 0; Type < NumberOfTypes; Type++)
       {
          //Debugger(Type);
          if (SeedNumberPerTypeInWater[Type * NumberOfBoxes + index] > 0.0)
          {
             Deposit(Type * NumberOfBoxes + index, ABoxDepth);
             SeedAge[Type * NumberOfBoxes + index] = SeedAge[Type * NumberOfBoxes + index] + TimeStep / DAYSTOSECONDS; //Age in days
          }
       }
     }
  }
}

void TSeeds::Integrate()
{
    int index;

    Integration(NumberOfTypes, SeedNumberPerTypeInWater, SeedInWaterFlux);
    Integration(NumberOfTypes, SeedNumberPerTypeSettled, SeedSettlingFlux);
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    // Transport the seeds
    //Debugger(NumberOfTypes);Debugger(MyNumberOfBoxesInSubDomain); Debugger(NumberOfBoxes);
    for (int Type = 0; Type < NumberOfTypes; Type++)
    {
       for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
       {
          index = pSubDomain->BoxNumber[k];
          Seeds[index] = SeedNumberPerTypeInWater[Type * NumberOfBoxes + index];
       }
       if (MyTransportPointer != NULL)
          MyTransportPointer->Go(Seeds, SeedLoad,0.0, 0.0);
       for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
       {
          index = pSubDomain->BoxNumber[k];
          SeedNumberPerTypeInWater[Type * NumberOfBoxes + index] = MAX(0.0,Seeds[index]);
       }
    }
}

void TSeeds::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    int i,j;
    char MyParameter[65], // 64 characters (max) for parameter name
    MyName[65];
    strcpy(MyParameter, ParameterName);
    double ABoxDepth, AVolume, AnArea;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
       MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,BoxNumber,"Box depth",ObjectCode);
    }
    else
       ABoxDepth = MyPEcoDynClass->GetBoxDepth(BoxNumber);
    AnArea =  MyPEcoDynClass->GetBoxArea(BoxNumber);
    AVolume = AnArea * ABoxDepth;
    if (strstr(MyParameter, "SeedNumberPerTypeInWater") != NULL)
    {
        for (j = 0; j < NumberOfTypes; j++)
        {
            sprintf( MyName, "SeedNumberPerTypeInWater%i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                /*if ((BoxNumber == 28224) && (j == 4))
                {
                   Debugger(ABoxDepth);
                   Debugger(AVolume);
                   Debugger(SeedNumberPerTypeInWater[NumberOfBoxes * 4 + BoxNumber]);
                } */
                Value = SeedNumberPerTypeInWater[NumberOfBoxes * j + BoxNumber]
                          * AVolume;
                break;
            }
        }
    }
    else if (strstr(MyParameter, "SeedNumberPerTypeSettled" ) != NULL)
    {
        for (j = 0; j < NumberOfTypes; j++)
        {
            sprintf( MyName, "SeedNumberPerTypeSettled%i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = SeedNumberPerTypeSettled[NumberOfBoxes * j + BoxNumber]
                          * AnArea;
                break;
            }
        }
    }
}

void TSeeds::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{

}

void TSeeds::Deposit(int index, double Depth)
{
   double SedimentationRate, SeedFlux;
   if ((Depth > CriticalWaterDepth) && (SeedAge[index] > TimeOfPositiveBuoyancy))
   {
      SedimentationRate = SettlingVelocity / Depth;  //s-1
      SeedFlux = SeedNumberPerTypeInWater[index] * SedimentationRate; //Seeds / m3 /s
   }
   else
   if (Depth <= CriticalWaterDepth)
       SeedFlux = SeedNumberPerTypeInWater[index] / TimeStep; // To guarantte that all seeds deposit in integration - Seeds / m3 /s
   else
       SeedFlux = 0.0;
   SeedInWaterFlux[index] = SeedInWaterFlux[index] - SeedFlux;   // Seeds / m3 /s
   SeedSettlingFlux[index] = SeedSettlingFlux[index] + SeedFlux * Depth; //Seeds / m2 /s
}

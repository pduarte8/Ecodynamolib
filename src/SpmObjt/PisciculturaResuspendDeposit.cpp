// SPM object CPP code file
// @ Pedro Duarte
// Rev. A 13/12/2010
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "spmobjt.h"
#include "iodll.h"



TPisciculturaResuspendDeposit::TPisciculturaResuspendDeposit(TEcoDynClass* APEcoDynClass,
        char* className) : TRiaFormosaResuspendDeposit(APEcoDynClass, className)
{
    BuildPisciculturaResuspendDeposit();
}

void TPisciculturaResuspendDeposit::BuildPisciculturaResuspendDeposit()
{
   NumberOfDaysForFoodTimeSeries = 0;
   NumberOfHoursForFoodTimeSeries = 0;
   NumberOfMomentsForFoodLoads = 0;

   SinkingVelocityOfFoodPellets = 1.0; //m/d
   Food = new double[NumberOfBoxes];
   FoodC = new double[NumberOfBoxes];
   FoodN = new double[NumberOfBoxes];
   FoodP = new double[NumberOfBoxes];
   FoodFlux = new double[NumberOfBoxes];
   FoodCFlux = new double[NumberOfBoxes];
   FoodNFlux = new double[NumberOfBoxes];
   FoodPFlux = new double[NumberOfBoxes];
   GenericLoad = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      Food[i] = 0.0;
      FoodC[i] = 0.0;
      FoodN[i] = 0.0;
      FoodP[i] = 0.0;
      FoodFlux[i] = 0.0;
      FoodCFlux[i] = 0.0;
      FoodNFlux[i] = 0.0;
      FoodPFlux[i] = 0.0;
      GenericLoad[i] = 0.0;
   }
   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
   int X, Y;
   if (PReadWrite != NULL)
   {
      if (PReadWrite->FindString("Suspended matter", X, Y))
      {
         PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
         char MyParameter[65];
         for (int i = Y; i < Y + NumberOfParameters; i++)
         {
            PReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "NumberOfDaysForFoodTimeSeries") == 0)
            {
                PReadWrite->ReadNumber(X+3, i, NumberOfDaysForFoodTimeSeries);
                NumberOfMomentsForFoodLoads = NumberOfDaysForFoodTimeSeries;
                //Debugger(NumberOfDaysForFoodTimeSeries);
            }
            else if (strcmp(MyParameter, "NumberOfHoursForFoodTimeSeries") == 0)
            {
                PReadWrite->ReadNumber(X+3, i, NumberOfHoursForFoodTimeSeries);
                NumberOfMomentsForFoodLoads = NumberOfHoursForFoodTimeSeries;
                //Debugger(NumberOfHoursForFoodTimeSeries);
            }
            else if (strcmp(MyParameter, "FoodCContent") == 0)
            {
                PReadWrite->ReadNumber(X+3, i, FoodCContent);
                //Debugger(FoodCContent);
            }
            else if (strcmp(MyParameter, "FoodNContent") == 0)
            {
                PReadWrite->ReadNumber(X+3, i, FoodNContent);
                //Debugger(FoodNContent);
            }
            else if (strcmp(MyParameter, "FoodPContent") == 0)
            {
                PReadWrite->ReadNumber(X+3, i, FoodPContent);
                //Debugger(FoodPContent);
            }
            else if (strcmp(MyParameter, "SinkingVelocityOfFoodPellets") == 0)
            {
                PReadWrite->ReadNumber(X+3, i, SinkingVelocityOfFoodPellets);
                //Debugger(SinkingVelocityOfFoodPellets);
            }
         }
      }
      else
      {
#ifdef __BORLANDC__
         MessageBox(0, "Parameters - Undefined object in Ria Formosa Suspended matter",
         "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TPisciculturaResuspendDeposit::BuildPisciculturaResuspendDeposit - Parameters undefined object" << endl;
#endif  // __BORLANDC__
      }
      CloseDataFile((void*)PReadWrite);
   }
   if (NumberOfLoads > 0)
      AFarmLoadRecord = new FarmLoadRecord[NumberOfLoads];
   //Debugger(NumberOfLoads); Debugger(NumberOfMomentsForFoodLoads);
   for (int i = 0; i < NumberOfLoads; i++)
   {
      AFarmLoadRecord[i].FoodLoad = new double[NumberOfMomentsForFoodLoads];
   }
   for (int i = 0; i < NumberOfLoads; i++)
   {
      AFarmLoadRecord[i].LineCoordinate = 0;
      AFarmLoadRecord[i].ColumnCoordinate = 0;
      for (int j = 0; j < NumberOfMomentsForFoodLoads; j++)
      {
         AFarmLoadRecord[i].FoodLoad[j] = 0.0;
      }
   }
   char MyVariable[65], LoadName[100], RiverName[100];
   int XV, YV, XL, YL;
   PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
   if (PReadWrite != NULL)
   {
      if (PReadWrite->FindString("Suspended matter", X, Y))
      {
         PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
         //for (int i = 0; i < NumberOfVariables; i++)
         //{
            //PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
            if (PReadWrite->FindString("LoadLines", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, AFarmLoadRecord[j].LineCoordinate);
                  AFarmLoadRecord[j].LineCoordinate = GridLines - AFarmLoadRecord[j].LineCoordinate;
                  //Debugger(GridLines);
                  //Debugger(AFarmLoadRecord[j].LineCoordinate);
               }
            }
            if (PReadWrite->FindString("LoadColumns", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadNumber(X, Y + 1 + j, AFarmLoadRecord[j].ColumnCoordinate);
                  AFarmLoadRecord[j].ColumnCoordinate = AFarmLoadRecord[j].ColumnCoordinate - 1;
                  //Debugger(AFarmLoadRecord[j].ColumnCoordinate);
               }
            }
            if (PReadWrite->FindString("LoadName", X, Y))
            {
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  PReadWrite->ReadString(X, Y + 1 + j, LoadName);
                  strcpy(AFarmLoadRecord[j].LoadName, LoadName);
               }
            }
            if (PReadWrite->FindString("FoodLoad", X, Y))
            {
               //Debugger(NumberOfLoads); Debugger(NumberOfMomentsForFoodLoads);
               for (int j = 0; j < NumberOfLoads; j++)
               {
                  int offset = j * NumberOfMomentsForFoodLoads;
                  for (int Loads = 0; Loads < NumberOfMomentsForFoodLoads; Loads++)
                  {
                     PReadWrite->ReadNumber(X, Y + 1 + Loads + offset,AFarmLoadRecord[j].FoodLoad[Loads]); //Loads should be in mg / L
                     //if (Loads == NumberOfMomentsForFoodLoads - 1) Debugger(ALoadRecord[j].TPMLoad[Loads]);
                  }
               }
            }
         //}
      }
      else
      {
#ifdef __BORLANDC__
         MessageBox(0, "Undefined object - TPisciculturaResuspendDeposit", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TPisciculturaResuspendDeposit::BuildPisciculturaResuspendDeposit - Variables undefined object" << endl;
#endif  // __BORLANDC__
      }
      CloseDataFile((void*)PReadWrite);
   }
}

TPisciculturaResuspendDeposit::~TPisciculturaResuspendDeposit()
{
    freeMemory();
}

double TPisciculturaResuspendDeposit::GetParameterValue(char* parmName)
{
}

bool TPisciculturaResuspendDeposit::SetParameterValue(char* parmName, double value)
{
}

bool TPisciculturaResuspendDeposit::SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName)
{
}

bool TPisciculturaResuspendDeposit::SaveVariables()
{
}

bool TPisciculturaResuspendDeposit::SaveParameters()
{
}

void TPisciculturaResuspendDeposit::freeMemory()
{
    try {
        if (NumberOfBoxes > 0) {
            delete []Food;
            delete []FoodC;
            delete []FoodN;
            delete []FoodP;
            delete [] FoodFlux;
            delete [] FoodCFlux;
            delete [] FoodNFlux;
            delete [] FoodPFlux;
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
				cerr << "TPisciculturaResuspendDeposit::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
    if ((NumberOfLoads > 0) && (NumberOfMomentsForFoodLoads > 0)) {
      for (int i = 0; i < NumberOfLoads; i++)
      {
          delete [] AFarmLoadRecord[i].FoodLoad;
      }
      delete [] AFarmLoadRecord;
    }
}

void TPisciculturaResuspendDeposit::Update(char* srcName, double Value,
						 int BoxNumber,
						 char* ParameterName,
						 int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);
   for (int i=0; i < NumberOfVariables; i++)
      if (strcmp(MyParameter, VariableNameArray[i]) == 0)
      {
         if (strcmp(MyParameter, "TPM") == 0)
            SPMFlux[MyBoxNumber] = SPMFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "POM") == 0)
         {
            POMFlux[MyBoxNumber] = POMFlux[MyBoxNumber] + Value;
         }
         else if (strcmp(MyParameter, "DetritalC") == 0)
            DetritalCFlux[MyBoxNumber] = DetritalCFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "DetritalN") == 0)
            DetritalNFlux[MyBoxNumber] = DetritalNFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "DetritalP") == 0)
            DetritalPFlux[MyBoxNumber] = DetritalPFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveC") == 0)
            LiveC[MyBoxNumber] = LiveC[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveN") == 0)
            LiveN[MyBoxNumber] = LiveN[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "LiveP") == 0)
            LiveP[MyBoxNumber] = LiveP[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "Food") == 0)
            FoodFlux[MyBoxNumber] = FoodFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "FoodC") == 0)
            FoodCFlux[MyBoxNumber] = FoodCFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "FoodN") == 0)
            FoodNFlux[MyBoxNumber] = FoodNFlux[MyBoxNumber] + Value;
         else if (strcmp(MyParameter, "FoodP") == 0)
            FoodPFlux[MyBoxNumber] = FoodPFlux[MyBoxNumber] + Value;
         return;
      }

#ifdef __BORLANDC__
   char Caption[129];
   strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
   MessageBox(0,Caption,"EcoDynamo Alert - Update 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TPisciculturaResuspendDeposit::Update 1 - " << MyParameter <<" does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
}

void TPisciculturaResuspendDeposit::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   /*if ((MyPEcoDynClass->GetJulianDay() > 128) && (MyPEcoDynClass->GetJulianDay() < 130) && (MyPEcoDynClass->GetCurrTime()>11.0)) {
       MessageBox(0,"MyParameter",MyParameter,MB_OK | MB_ICONHAND);
       } */
   if (strcmp(MyParameter, "TPM") == 0)
      Value = SPMConcentration[MyBoxNumber];
   else if (strcmp(MyParameter, "POM") == 0)
      Value = POMConcentration[MyBoxNumber];
   else if (strcmp(MyParameter, "Extinction coefficient Ktot") == 0)
      Value = TotalExtinctionCoefficient[MyBoxNumber];
   else if (strcmp(MyParameter, "DetritalC") == 0)
      Value = DetritalC[MyBoxNumber];
   else if (strcmp(MyParameter, "DetritalN") == 0)
      Value = DetritalN[MyBoxNumber];
   else if (strcmp(MyParameter, "DetritalP") == 0)
      Value = DetritalP[MyBoxNumber];
   else if (strcmp(MyParameter, "LiveC") == 0)
      Value = LiveC[MyBoxNumber];
   else if (strcmp(MyParameter, "LiveN") == 0)
      Value = LiveN[MyBoxNumber];
   else if (strcmp(MyParameter, "LiveP") == 0)
      Value = LiveP[MyBoxNumber];
   else if (strcmp(MyParameter, "TPM_Mineralization_C") == 0)
      Value = Mineralization_C[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "TPM_Mineralization_N") == 0)
      Value = Mineralization_N[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "TPM_Mineralization_P") == 0)
      Value = Mineralization_P[MyBoxNumber]; //mg L-1 d-1
   else if (strcmp(MyParameter, "Dissolution_C") == 0)
      Value = Dissolution_C[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "Dissolution_N") == 0)
      Value = Dissolution_N[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "Dissolution_P") == 0)
      Value = Dissolution_P[MyBoxNumber];  //mg L-1 d-1
   else if (strcmp(MyParameter, "DepositionFlux") == 0)
      Value = DepositionFlux[MyBoxNumber];
   else if (strcmp(MyParameter, "ResuspensionFlux") == 0)
      Value = ResuspensionFlux[MyBoxNumber];
   else if (strcmp(MyParameter, "Boundary SPM concentration") == 0)
      Value = BoundarySPMConcentration[JulianDay - 1];
   else if (strcmp(MyParameter, "Boundary POM concentration") == 0)
      Value = BoundaryPOMConcentration[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryDetritalC") == 0)
      Value = BoundaryDetritalC[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryDetritalN") == 0)
      Value = BoundaryDetritalN[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryDetritalP") == 0)
      Value = BoundaryDetritalP[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryLiveC") == 0)
      Value = BoundaryLiveC[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryLiveN") == 0)
      Value = BoundaryLiveN[JulianDay - 1];
   else if (strcmp(MyParameter, "BoundaryLiveP") == 0)
      Value = BoundaryLiveP[JulianDay - 1];
   else if (strcmp(MyParameter, "Food") == 0)
      Value = Food[MyBoxNumber];
   else if (strcmp(MyParameter, "FoodC") == 0)
      Value = FoodC[MyBoxNumber];
   else if (strcmp(MyParameter, "FoodN") == 0)
      Value = FoodN[MyBoxNumber];
   else if (strcmp(MyParameter, "FoodP") == 0)
      Value = FoodP[MyBoxNumber];
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
            MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TPisciculturaResuspendDeposit::Inquiry 1 - " << MyParameter <<" does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);

}

void TPisciculturaResuspendDeposit::Integrate()
{
    double ABoxDepth; int index;

    Integration(SPMConcentration, SPMFlux,
                POMConcentration, POMFlux,
                DetritalC, DetritalCFlux,
                DetritalN, DetritalNFlux,
                DetritalP, DetritalPFlux);
    Integration(Food,FoodFlux);
    Integration(FoodC,FoodCFlux);
    Integration(FoodN,FoodNFlux);
    Integration(FoodP,FoodPFlux);
    // Transport the SPM

    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Go(SPMConcentration, TPMLoad,0.0, 0.0);
        MyTransportPointer->Go(POMConcentration, POMLoad,0.0, 0.0);
        MyTransportPointer->Go(DetritalC, DetritalCLoad,0.0, 0.0);
        /*if ((MyPEcoDynClass->GetJulianDay() > 128) && (MyPEcoDynClass->GetJulianDay() < 130) && (MyPEcoDynClass->GetCurrTime()>10.0)) {
          Debugger(33);
          Debugger(DetritalCLoad[0]);
          Debugger(DetritalC[0]);
        } */
        MyTransportPointer->Go(DetritalN, DetritalNLoad,0.0, 0.0);
        MyTransportPointer->Go(DetritalP, DetritalPLoad,0.0, 0.0);
        MyTransportPointer->Go(Food,  GenericLoad,0.0,0.0);  //GenericLoad is set to zero in BuildPisciculturaResuspendDeposit
        MyTransportPointer->Go(FoodC, GenericLoad,0.0,0.0);  //GenericLoad is set to zero in BuildPisciculturaResuspendDeposit
        MyTransportPointer->Go(FoodN, GenericLoad,0.0,0.0);  //GenericLoad is set to zero in BuildPisciculturaResuspendDeposit
        MyTransportPointer->Go(FoodP, GenericLoad,0.0,0.0);  //GenericLoad is set to zero in BuildPisciculturaResuspendDeposit

        JulianDay = MyPEcoDynClass->GetJulianDay();
   	//Southern boundary
   	int i,j;
        for (j = pSubDomain->FirstColumn;
            j <= pSubDomain->LastColumn; j++)
        {
           i = pSubDomain->ILine[j];
           if (i <= pSubDomain->FLine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
              if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    //(BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
                 )
      	      {
                 SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                 POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                 DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                 DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                 DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	      }
           }
        }
      //Northern boundary
   	for (j = pSubDomain->FirstColumn;
            j <= pSubDomain->LastColumn; j++)
        {
           i = pSubDomain->FLine[j] /*- 1*/;
           if (i >= pSubDomain->ILine[j])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
      	      if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    //(BoxArray[index].Dummybool2) &&
            	    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
                 )
      	      {
                 SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                 POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                 DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                 DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                 DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	      }
           }
   	}
      //Eastern boundary
   	for (i = pSubDomain->FirstLine;
            i <= pSubDomain->LastLine; i++)
        {
           j = pSubDomain->FColumn[i] /*- 1*/;
           if (j >= pSubDomain->IColumn[i])
           {
              index = i * GridColumns + j;
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
      	      if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    //(BoxArray[index].Dummybool2) &&
            	    (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
                 )
      	      {
                 SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                 POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                 DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                 DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                 DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	      }
           }
   	}
        //Western boundary
        for (i = pSubDomain->FirstLine;
            i <= pSubDomain->LastLine; i++)
   	    {
               j = pSubDomain->IColumn[i];
               if (j <= pSubDomain->FColumn[i])
               {
                  index = i * GridColumns + j;
                   MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
         	                             index,
            	                          "Box depth",
               	                       ObjectCode);
      	          if (
      			(ABoxDepth > CriticalWaterDepth) &&
                        //(BoxArray[index].Dummybool2) &&
            	        (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
      		     )
      	          {
      		     SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
      		     POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
         	     DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                     DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                     DetritalP[index] = BoundaryDetritalP[JulianDay-1];
      	          }
               }
   	    }
    }
    TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
    double PHYORG, MyPhytoBiomass, MyChlorophyllToBiomass;
    int ADay, AMoment, AHour;
    ADay = 0; AMoment = 0; AHour = 0;
    //ATime = ATime + MyPEcoDynClass->GetTimeStep();
    ADay = (int) ((ATime - AStartTime) / DAYSTOSECONDS);
    if  (NumberOfDaysForFoodTimeSeries >= 1)
    {
       //ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForFoodTimeSeries - 1);
       AMoment = ADay;
    }
    else
    if (NumberOfHoursForFoodTimeSeries >= 1)
    {
       //ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForFoodTimeSeries - 1);
    }
    //Debugger(MyNumberOfBoxesInSubDomain);
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
      index = pSubDomain->BoxNumber[k];
      //Debugger(index);
      //Debugger(AMoment);
      //Debugger(AFarmLoadRecord[index].FoodLoad[AMoment]);
      double FoodInput;
      FoodInput = AFarmLoadRecord[index].FoodLoad[AMoment] * MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS; //Food load in mg/L/h
      Food[index] = Food[index] + FoodInput; //Food load in mg/L/h
      FoodC[index] = FoodC[index] + FoodInput * FoodCContent;
      FoodN[index] = FoodN[index] + FoodInput * FoodNContent;
      FoodP[index] = FoodP[index] + FoodInput * FoodPContent;
      SPMConcentration[index] = SPMConcentration[index] + AFarmLoadRecord[index].FoodLoad[AMoment] * MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS; //Food load in mg/L/h
      POMConcentration[index] = POMConcentration[index] + AFarmLoadRecord[index].FoodLoad[AMoment] * MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS; //Food load in mg/L/h
      if (SPMConcentration[index] < 0.0) SPMConcentration[index] = 0.0;
      if (POMConcentration[index] < 0.0) POMConcentration[index] = 0.0;
      if (DetritalC[index] < 0.0) DetritalC[index] = 0.0;
      if (DetritalN[index] < 0.0) DetritalN[index] = 0.0;
      if (DetritalP[index] < 0.0) DetritalP[index] = 0.0;
      if (LiveC[index] < 0.0) LiveC[index] = 0.0;
      if (LiveN[index] < 0.0) LiveN[index] = 0.0;
      if (LiveP[index] < 0.0) LiveP[index] = 0.0;
      if (Food[index] < 0.0) Food[index] = 0.0;
      if (FoodC[index] < 0.0) FoodC[index] = 0.0;
      if (FoodN[index] < 0.0) FoodN[index] = 0.0;
      if (FoodP[index] < 0.0) FoodP[index] = 0.0;
      ExtinctionCoefficientKTot(index);
    }
    Loads();
}

void TPisciculturaResuspendDeposit::Deposit(int ABox)
{
   TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
   double TotalDeposition, OrganicDeposition, CDeposition, NDeposition, PDeposition,
          FoodDeposition, FoodCDeposition, FoodNDeposition, FoodPDeposition;
   double MyPhytoBiomass, MyChlorophyllToBiomass, PHYORG, MySinkingVelocity, MyPOMSinkingVelocity, MyFoodSinkingVelocity;
   int MyBox;
   MyBox = ABox;
   MySinkingVelocity = SinkingVelocity / DAYSTOSECONDS;                   //m/s
   MyPOMSinkingVelocity = POMSinkingVelocity / DAYSTOSECONDS;             //m/s
   MyFoodSinkingVelocity = SinkingVelocityOfFoodPellets / DAYSTOSECONDS;  //m/s
   TEcoDynClass *MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
   if (MyPhytoPointer != NULL)
   {
      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,
                                MyBox,
               	                "Phytoplankton biomass",
                                ObjectCode);

      MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllToBiomass,
            			             MyBox,
               	                "Chlorophyll to biomass",
                  	             ObjectCode);
      // Phytoplankton returns its mass as ug chl a l-1
      // convert to mg l-1 of organic dry matter
      PHYORG = MyPhytoBiomass / CUBIC * MyChlorophyllToBiomass; //mg/l
   }
   else
      PHYORG = 0.0;

   if (POMConcentration[MyBox] >= PHYORG)
   {
      FoodDeposition = MyFoodSinkingVelocity * Food[MyBox] / ABoxDepth;
      OrganicDeposition = MyPOMSinkingVelocity * (POMConcentration[MyBox] - PHYORG - Food[MyBox]) / ABoxDepth + FoodDeposition;

      FoodCDeposition = MyFoodSinkingVelocity * FoodC[MyBox] / ABoxDepth;
      CDeposition = MyPOMSinkingVelocity * DetritalC[MyBox] / ABoxDepth;
      //CDeposition = MyPOMSinkingVelocity * (DetritalC[MyBox] - FoodC[MyBox]) / ABoxDepth + FoodCDeposition;

      FoodNDeposition = MyFoodSinkingVelocity * FoodN[MyBox] / ABoxDepth;
      NDeposition = MyPOMSinkingVelocity * DetritalN[MyBox] / ABoxDepth;
      //NDeposition = MyPOMSinkingVelocity * (DetritalN[MyBox] - FoodN[MyBox]) / ABoxDepth + FoodNDeposition;

      FoodPDeposition = MyFoodSinkingVelocity * FoodP[MyBox] / ABoxDepth;
      PDeposition = MyPOMSinkingVelocity * DetritalP[MyBox] / ABoxDepth;
      //PDeposition = MyPOMSinkingVelocity * (DetritalP[MyBox] - FoodP[MyBox]) / ABoxDepth + FoodPDeposition;
   }
   else
   {
      OrganicDeposition = 0.0;
      CDeposition = 0.0;
      NDeposition = 0.0;
      PDeposition = 0.0;
      FoodDeposition = 0.0;
      FoodCDeposition = 0.0;
      FoodNDeposition = 0.0;
      FoodPDeposition = 0.0;
   }
   if (SPMConcentration[MyBox] > POMConcentration[MyBox])
      TotalDeposition = MySinkingVelocity * (SPMConcentration[MyBox] - PHYORG - POMConcentration[MyBox] - Food[MyBox])  / ABoxDepth +
                        OrganicDeposition;   //mg / L / s
   else
      TotalDeposition = 0.0;

   if (MySedimentPointer != NULL)
   {
      //double SedimentDepth;
      //MySedimentPointer->Inquiry(GetEcoDynClassName(), SedimentDepth,MyBox,"LayerDepth 1",ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), (CDeposition + FoodCDeposition) * CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "Corganic 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), (NDeposition + FoodNDeposition) * CUBIC * /*SedimentDepth*/ABoxDepth, MyBox,"Norganic 1",ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), (PDeposition + FoodPDeposition) * CUBIC * /*SedimentDepth*/ABoxDepth, MyBox, "Porganic 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), OrganicDeposition * CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "OrganicMass 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), (TotalDeposition - OrganicDeposition)* CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "InorganicMass 1", ObjectCode);
   }

   DepositionFlux[MyBox] = TotalDeposition * DAYSTOSECONDS; //mg TPM/L/d
   SPMFlux[MyBox] = SPMFlux[MyBox] - TotalDeposition;
   POMFlux[MyBox] = POMFlux[MyBox] - OrganicDeposition;

   DetritalCFlux[MyBox] = DetritalCFlux[MyBox] - CDeposition;
   DetritalNFlux[MyBox] = DetritalNFlux[MyBox] - NDeposition;
   DetritalPFlux[MyBox] = DetritalPFlux[MyBox] - PDeposition;

   FoodFlux[MyBox] = FoodFlux[MyBox] - FoodDeposition;
   FoodCFlux[MyBox] = FoodCFlux[MyBox] - FoodCDeposition;
   FoodNFlux[MyBox] = FoodNFlux[MyBox] - FoodNDeposition;
   FoodPFlux[MyBox] = FoodPFlux[MyBox] - FoodPDeposition;
}







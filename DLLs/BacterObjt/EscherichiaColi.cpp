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
#include "bacteriaobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"


TEscherichiaColi::TEscherichiaColi(TEcoDynClass* APEcoDynClass, char* className): TBacteria(APEcoDynClass, className)
{
   EschirichiaColiNumber = new double[NumberOfBoxes];
   K = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++) {
       EschirichiaColiNumber[i] = 0.0;
       K[i] = 0.0;
   }
   ReadParameters();
   ReadVariables();
}

TEscherichiaColi::~TEscherichiaColi()
{
    freeMemory();
}

void TEscherichiaColi::freeMemory()
{
    delete [] EschirichiaColiNumber;
    delete [] K;
    delete [] VariableNameArray;
    try {
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }
        if ((NumberOfLoads > 0) && (NumberOfDaysForLoads > 0)) {
            for (int i = 0; i < NumberOfLoads; i++)
            {
                delete [] ALoadRecord[i].BacteriaCLoad;
                delete [] ALoadRecord[i].BacteriaNLoad;
                delete [] ALoadRecord[i].BacteriaPLoad;
                delete [] ALoadRecord[i].BacteriaNumberLoad;
            }
            delete [] ALoadRecord;
        }
        if ((NumberOfRiverLoads > 0) && (NumberOfDaysForLoads > 0)) {
            for (int i = 0; i < NumberOfRiverLoads; i++)
            {
                delete [] ALoadRiverRecord[i].BacteriaCRiverLoad;
                delete [] ALoadRiverRecord[i].BacteriaNRiverLoad;
                delete [] ALoadRiverRecord[i].BacteriaPRiverLoad;
                delete [] ALoadRiverRecord[i].BacteriaBiomassLoad;
            }
            delete [] ALoadRiverRecord;
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
        cerr << "TEscherichiaColi::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TEscherichiaColi::ReadParameters()
{
    // Read in the parameters
    MyPReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("EscherichiaColi");
    if (MyPReadWrite == NULL)
    {
            return;
    }
    double MyValue;
    int X, Y;
    if (MyPReadWrite->FindString("Bacteria", X, Y))
    {
        MyPReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
        // read in the parameter names
        char MyParameter[65];
        for (int i = Y; i < Y + NumberOfParameters; i++)
        {
            //Debugger(i);
            MyPReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "K0") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, K0);
                //Debugger(K0);
            }
            else if (strcmp(MyParameter, "KT20") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, KT20);
                //Debugger(KT20);
            }
            else if (strcmp(MyParameter, "KS") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, KS);
                //Debugger(KS);
            }
            else if (strcmp(MyParameter, "KL") == 0)
            {
                MyPReadWrite->ReadNumber(X+3, i, KL);
                //Debugger(KL);
            }
            else if (strcmp(MyParameter, "NumberOfLoads") == 0)
            {
                MyPReadWrite->ReadNumber(X + 3, i, NumberOfLoads);
                //Debugger(NumberOfLoads);
            }
            else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
            {
                MyPReadWrite->ReadNumber(X + 3, i, NumberOfDaysForLoads);
                //Debugger(NumberOfDaysForLoads);
            }
            else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
            {
                MyPReadWrite->ReadNumber(X + 3, i, NumberOfRiverLoads);
                //Debugger(NumberOfRiverLoads);
            }
        }
    }
    else
#ifdef __BORLANDC__
       MessageBox(0, "Undefined parameters in EscherichiaColi parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TEscherichiaColi::ReadParameters - undefined object" << endl;
#endif  // __BORLANDC__

    if (NumberOfLoads > 0)
       ALoadRecord = new LoadRecord[NumberOfLoads];
    if (NumberOfRiverLoads > 0)
       ALoadRiverRecord = new LoadRiverRecord[NumberOfRiverLoads];
    for (int i = 0; i < NumberOfLoads; i++)
    {
        ALoadRecord[i].BacteriaCLoad = new double[NumberOfDaysForLoads];
        ALoadRecord[i].BacteriaNLoad = new double[NumberOfDaysForLoads];
        ALoadRecord[i].BacteriaPLoad = new double[NumberOfDaysForLoads];
        ALoadRecord[i].BacteriaPLoad = new double[NumberOfDaysForLoads];
        ALoadRecord[i].BacteriaNumberLoad = new double[NumberOfDaysForLoads];
    }
    for (int i = 0; i < NumberOfLoads; i++)
    {
        ALoadRecord[i].LineCoordinate = 0;
        ALoadRecord[i].ColumnCoordinate = 0;
        for (int j = 0; j < NumberOfDaysForLoads; j++)
        {
            ALoadRecord[i].BacteriaCLoad[j] = 0.0;
            ALoadRecord[i].BacteriaNLoad[j] = 0.0;
            ALoadRecord[i].BacteriaPLoad[j] = 0.0;
            ALoadRecord[i].BacteriaNumberLoad[j] = 0.0;
        }
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
        ALoadRiverRecord[i].BacteriaCRiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].BacteriaNRiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].BacteriaPRiverLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].BacteriaBiomassLoad = new double[NumberOfDaysForLoads];
        ALoadRiverRecord[i].BacteriaNumberRiverLoad = new double[NumberOfDaysForLoads];
    }
    for (int i = 0; i < NumberOfRiverLoads; i++)
    {
        ALoadRiverRecord[i].LineCoordinate = 0;
        ALoadRiverRecord[i].ColumnCoordinate = 0;
        for (int j = 0; j < NumberOfDaysForLoads; j++)
        {
            ALoadRiverRecord[i].BacteriaCRiverLoad[j] = 0.0;
            ALoadRiverRecord[i].BacteriaNRiverLoad[j] = 0.0;
            ALoadRiverRecord[i].BacteriaPRiverLoad[j] = 0.0;
            ALoadRiverRecord[i].BacteriaBiomassLoad[j] = 0.0;
            ALoadRiverRecord[i].BacteriaNumberRiverLoad[j] = 0.0;
        }
    }
}

void TEscherichiaColi::ReadVariables()
{
   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("EscherichiaColi");
   if (PReadWrite != NULL)
   {
       int X, Y, XV, YV;
       if (PReadWrite->FindString("Bacteria", X, Y))
       {
          //Debugger(X); Debugger(Y);
          char MyVariable[65], LoadName[100], RiverName[100];
          PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
          // Create array for state variable names
          // read in the variable names
          VariableNameArray = new VNA[NumberOfVariables];
          for (int i = 0; i < NumberOfVariables; i++)
          {
             PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
          }
          for (int i = 0; i < NumberOfVariables; i++)
          {
             double MyValue;
             PReadWrite->ReadString(X + 2 + i, Y, MyVariable);

             if (strcmp(MyVariable, "EscherichiaColi number") == 0)
             {
                if (!PReadWrite->FindString("EscherichiaColi number values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxes; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, EschirichiaColiNumber[j]);
                }
                //Debugger(EschirichiaColiNumber[NumberOfBoxes-1]);
             }
          }
          if (PReadWrite->FindString("Daily boundary EscherichiaColi numbers", XV, YV))
          {
            for (int j = 0; j < 365; j++)
            {
               PReadWrite->ReadNumber(XV, YV + 1 + j, DailyBoundaryBacteria[j]);
            }
            //Debugger(DailyBoundaryBacteria[364]);
          }
          if (PReadWrite->FindString("LoadLines", XV, YV))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRecord[j].LineCoordinate);
                ALoadRecord[j].LineCoordinate = NumberOfLines - ALoadRecord[j].LineCoordinate;
             }
          }
          if (PReadWrite->FindString("LoadColumns", XV, YV))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRecord[j].ColumnCoordinate);
                ALoadRecord[j].ColumnCoordinate = ALoadRecord[j].ColumnCoordinate - 1;
             }
          }
          if (PReadWrite->FindString("LoadLayers", XV, YV))
          {
             //Debugger(XV); Debugger(YV);
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRecord[j].LayerCoordinate);
                ALoadRecord[j].LayerCoordinate = NumberOfLayers - ALoadRecord[j].LayerCoordinate;
                //Debugger(ALoadRecord[j].LayerCoordinate);
             }
          }
          if (PReadWrite->FindString("LoadName", XV, YV))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadString(XV, YV + 1 + j, LoadName);
                strcpy(ALoadRecord[j].LoadName, LoadName);
             }
          }
          if (PReadWrite->FindString("BacteriaNumberLoad", XV, YV))
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRecord[j].BacteriaNumberLoad[Loads]); //Loads should be in bacteria / mL 
                }
             }
          }
          if (PReadWrite->FindString("RiverLoadLines", XV, YV))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].LineCoordinate);
                ALoadRiverRecord[j].LineCoordinate = NumberOfLines - ALoadRiverRecord[j].LineCoordinate;
                //Debugger(ALoadRiverRecord[j].LineCoordinate);
             }
          }
          if (PReadWrite->FindString("RiverLoadColumns", XV, YV))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].ColumnCoordinate);
                ALoadRiverRecord[j].ColumnCoordinate = ALoadRiverRecord[j].ColumnCoordinate - 1;
                //Debugger(ALoadRiverRecord[j].ColumnCoordinate);
             }
          }
          if (PReadWrite->FindString("RiverLoadLayers", XV, YV))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].LayerCoordinate);
                //Debugger(ALoadRiverRecord[j].LayerCoordinate);
                ALoadRiverRecord[j].LayerCoordinate = NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate;

             }
          }
          if (PReadWrite->FindString("RiverName", XV, YV))
          //if (PReadWrite->FindString("RiverName", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadString(XV, YV + 1 + j, RiverName);
                strcpy(ALoadRiverRecord[j].RiverName, RiverName);
             }
          }
          if (PReadWrite->FindString("BacteriaNumberRiverLoad", XV, YV))
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRiverRecord[j].BacteriaNumberRiverLoad[Loads]); //Loads should be in bacteria / mL
                }
             }
             //Debugger(NumberOfRiverLoads); Debugger(NumberOfDaysForLoads);
             //Debugger(ALoadRiverRecord[NumberOfRiverLoads-1].BacteriaNumberRiverLoad[NumberOfDaysForLoads-1]);
          }
       }
       else
#ifdef __BORLANDC__
          MessageBox(0, "Variables:Undefined object - TEscherichiaColi", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TEscherichiaColi::ReadVariables - undefined object" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
   }
}

void TEscherichiaColi::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName,int AnObjectCode)
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
         if (strcmp(MyParameter, "EscherichiaColi number") == 0)
                  Value = EschirichiaColiNumber[MyBoxNumber];
         if (strcmp(MyParameter, "K") == 0)
                  Value = K[MyBoxNumber] * HOURSTOSECONDS;   //return values in h-1
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
    cerr << "TEscherichiaColi::Inquiry 2 - " << MyParameter
        << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    Value = 0;
}

void TEscherichiaColi::Integrate()
{
   //Debugger(1);
   int index;
   double AValue, NPhytoLoad, PPhytoLoad, Depth;
   double const Tinny = 0.000001;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

   Generic = EschirichiaColiNumber; GenericFlux = BacteriaFlux;
   Integration3DSubDomain(UpperLayer);
   EschirichiaColiNumber = Generic; BacteriaFlux = GenericFlux;

   if (MyTransportPointer != NULL)
        MyTransportPointer->Go(EschirichiaColiNumber, BacteriaNumberLoad, 0.0, 0.0);
   for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
   {
      for (int j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
      {
          for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], NumberOfLines - 1); i++)
          {
             if (i <= pSubDomain->FLine[j])
             {
                index = Get3DIndex(i,j,LayerNr);
                if (MyTransportPointer != NULL)
                {
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                                       index,
                                                       "Box depth",
                                                       ObjectCode);
                }
                else Depth = MyPEcoDynClass->GetBoxDepth(index);

                EschirichiaColiNumber[index] = MAX(EschirichiaColiNumber[index],0.0);

                if (
                       (Depth > CriticalDepthForLandBoundary) &&
                       (BoxArray[index].Dummybool1) &&
                       (
                          (BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) || //No western ocean boundary
                          (BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) || //No eastern ocean boundary
                          (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||  //No southern ocean boundary
                          (BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2)    //No northern ocean boundary
                       )
                   )
                {
                   EschirichiaColiNumber[index] = DailyBoundaryBacteria[JulianDay-1];
                }
             }
          }
      }
   }
   Loads();
}


void TEscherichiaColi::Go()
{
    int k, index;
    double AValue, TotalLightAtTop, TotalLightAtBottom, PARLightAtTop, PARLightAtBottom, WaterTemperature, Salinity, KValue, Depth;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;

    TotalLightAtTop = 0.0; TotalLightAtBottom = 0.0; PARLightAtTop = 0.0; PARLightAtBottom = 0.0;
    WaterTemperature = 0.0; Salinity = 0.0; KValue = 1.0;

    TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
                * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer(),
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
        UpperLayer = ceil(AValue);
    }
    else
        UpperLayer = NumberOfLayers - 1;
    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
       //Debugger(i);
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],NumberOfColumns-1); j++)
       {
          //if (i == 0) Debugger(j);
          for (int LayerNr = UpperLayer; LayerNr >= 0; LayerNr--)
          {
             index = Get3DIndex(i,j,LayerNr);
             if (MyTransportPointer != NULL)
             {
                 MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,index,"Box depth",ObjectCode);
                 MyTransportPointer->Inquiry(GetEcoDynClassName(), Salinity,index,"Salinity",ObjectCode);
             }
             else
             {
                 Depth = MyPEcoDynClass->GetBoxDepth(index);
             }
             index = Get3DIndex(i,j,LayerNr);
             if (
                   (Depth > CriticalDepthForLandBoundary) &&
                   (BoxArray[index].Dummybool1) &&
                   (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) && //No western ocean boundary
                   (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) && //No eastern ocean boundary
                   (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&  //No southern ocean boundary
                   (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2)    //No northern ocean boundary
                )
             {
               if (MyLightPointer != NULL)
               {
                   if (MySPMPointer != NULL)
                      MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,index,"Extinction coefficient Ktot", ObjectCode);
                   if (LayerNr == UpperLayer)
                   {
                       int index1;
                       index1 = Get3DIndex(i,j,0);  //O liteobjt calcula a radia??o ? superf?cie para a camada 0
                       MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtTop, index1,"Total surface irradiance",ObjectCode);
                       MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtTop, index1,"PAR surface irradiance",ObjectCode);
                       MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtBottom,
                                                                   TotalLightAtTop,
                                                                   Depth,
                                                                   KValue,
                                                                   index,
                                                                   "Sub-surface irradiance",
                                                                   ObjectCode);
                       MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtBottom,
                                                                   TotalLightAtTop,
                                                                   Depth,
                                                                   KValue,
                                                                   index,
                                                                   "Sub-surface PAR irradiance",
                                                                   ObjectCode);
                   }
                   else
                   {
                       TotalLightAtTop = TotalLightAtBottom;
                       PARLightAtTop = PARLightAtBottom;
                       MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtBottom,
                                               TotalLightAtTop, Depth, KValue, index,
                                               "Sub-surface irradiance", ObjectCode);
                       MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtBottom,
                                               TotalLightAtTop, Depth, KValue, index,
                                               "Sub-surface PAR irradiance", ObjectCode);
                   }
               }
               if (MyWaterTemperaturePointer != NULL)
                  MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,index,
                                             "Water temperature",
                                             ObjectCode);
               Mortality(index,Depth, KValue, WaterTemperature,Salinity, PARLightAtTop, PARLightAtBottom);
             }
          } //layer
       }
    }
}

void TEscherichiaColi::Mortality(int ABoxNumber, double ABoxDepth, double AKValue, double AWaterTemperature, double ASalinity, double APARLightAtTop, double APARLightAtBottom)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   double MyBoxDepth, MyKValue, MyWaterTemperature, MySalinity, MyPARLightAtTop, MyPARLightAtBottom;
   MyBoxDepth = ABoxDepth; MyKValue = AKValue;
   MyWaterTemperature = AWaterTemperature; MySalinity = ASalinity;
   MyPARLightAtTop = APARLightAtTop; MyPARLightAtBottom = APARLightAtBottom;
   K[MyBoxNumber] = K0 * pow(KT20,MyWaterTemperature - 20.0) * pow(KS,MySalinity) + KL * (MyPARLightAtTop - MyPARLightAtBottom) / ( MyKValue * MyBoxDepth);
   K[MyBoxNumber] = K[MyBoxNumber] / HOURSTOMINUTES; //K comes in 1/minutes, so dividing by HOURSTOMINUTES = dividing by 60 resulting in K in 1/seconds
   BacteriaFlux[MyBoxNumber] = BacteriaFlux[MyBoxNumber] - K[MyBoxNumber] * EschirichiaColiNumber[MyBoxNumber];
   /*if (EschirichiaColiNumber[MyBoxNumber] > 0.0)
   {
      Debugger(K[MyBoxNumber]);
      Debugger(BacteriaFlux[MyBoxNumber]);
   } */
}

void TEscherichiaColi::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    for (int j = 0; j < NumberOfLoads; j++)
    {
        index = Get3DIndex(ALoadRecord[j].LineCoordinate, ALoadRecord[j].ColumnCoordinate, ALoadRecord[j].LayerCoordinate);
        //Debugger(index);
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TEscherichiaColi::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TEscherichiaColi::Loads - Index " << index
                << " > Number of Boxes (" << NumberOfBoxes << ")\n\n"
                << "Please correct Configuration Files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        //if (BoxArray[index].Dummybool2)
        if (Depth > 0.0)
        {
            BacteriaNumberLoad[index] = ALoadRecord[j].BacteriaNumberLoad[ADay-1];
        }
    }
    //Debugger(NumberOfRiverLoads);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        index = Get3DIndex(ALoadRiverRecord[j].LineCoordinate, ALoadRiverRecord[j].ColumnCoordinate, ALoadRiverRecord[j].LayerCoordinate);
        /*Debugger(index);
        Debugger(ALoadRiverRecord[j].LineCoordinate);
        Debugger(ALoadRiverRecord[j].ColumnCoordinate);
        Debugger(ALoadRiverRecord[j].LayerCoordinate); */

        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TEscherichiaColi::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TEscherichiaColi::Loads (Rivers) - Index " << index
                << " > Number of Boxes (" << NumberOfBoxes << ")\n\n"
                << "Please correct Configuration Files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);

        if (Depth > 0.0)
        {
            EschirichiaColiNumber[index] = ALoadRiverRecord[j].BacteriaNumberRiverLoad[ADay-1];
            //Debugger(EschirichiaColiNumber[index]);
        }
    }
}

// Phytoplankton object CPP code file
// @ Pedro Duarte
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>
	#include <cmath> 
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "phytobjt.h"
#include "iodll.h"


TPhytoplankton3D::TPhytoplankton3D(TEcoDynClass* APEcoDynClass, char* className)
              : TRiaF2DPhytoplankton(APEcoDynClass, className)
{
    //BuildRiaF2DPhytoplankton();
    BuildTPhytoplankton3D();
}



void TPhytoplankton3D::BuildTPhytoplankton3D()
{
   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
   if (PReadWrite != NULL)
   {
       int X, Y, XV, YV;
       if (PReadWrite->FindString("Phytoplankton", X, Y))
       {
          //Debugger(X); Debugger(Y);
          char MyVariable[65], LoadName[100], RiverName[100];
          PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
          // Create array for state variable names
          // read in the variable names
          //Debugger(NumberOfLayers);
          for (int i = 0; i < NumberOfVariables; i++)
          {
              double MyValue;
              PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
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
              if (PReadWrite->FindString("RiverLoadLayers", XV, YV))
              {
                 for (int j = 0; j < NumberOfRiverLoads; j++)
                 {
                    PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].LayerCoordinate);
                    //Debugger(ALoadRiverRecord[j].LayerCoordinate);
                    ALoadRiverRecord[j].LayerCoordinate = NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate;

                 }
              }
           }
       }
       else
#ifdef __BORLANDC__
          MessageBox(0, "Variables:Undefined object - TPhytoplankton3D", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TPhytoplankton3D::BuildTPhytoplankton3D - Variables:Undefined object - TPhytoplankton3D" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
   }
}


void TPhytoplankton3D::Go()
{
    int index; double AValue, Depth, MyPOMFlux;
    double TotalLightAtTop, TotalLightAtBottom, PARLightAtTop, PARLightAtBottom;

    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
                * MyLightPointer = MyPEcoDynClass->GetLightPointer();
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
        UpperLayer = ceil(AValue);
    }
    else
        UpperLayer = NumberOfLayers - 1; 
    TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
       {
          //if (i == 9) Debugger(j);
          TotalLightAtTop = 0.0; TotalLightAtBottom = 0.0;
          PARLightAtTop = 0.0; PARLightAtBottom = 0.0;
          for (int LayerNr = UpperLayer; LayerNr >= 0; LayerNr--)
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
             if (
                   (Depth > CriticalDepthForLandBoundary) &&
                   (BoxArray[index].Dummybool1) &&
                   (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) && //No western ocean boundary
                   (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) && //No eastern ocean boundary
                   (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&  //No southern ocean boundary
                   (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2)    //No northern ocean boundary
                )
             {
                if (
                      (NCellQuota[index] < PhytoBiomass[index]) &&
                      (PCellQuota[index] < PhytoBiomass[index]) &&
                      (PCellQuota[index] < NCellQuota[index]) &&
                      (PhytoBiomass[index] > 0.000001)
                   )
                 {
                      if (MyLightPointer != NULL)
                      {
                           if (MySPMPointer != NULL)
                               MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,index,"Extinction coefficient Ktot", ObjectCode);

                           if (LayerNr == UpperLayer)
                           {
                               //if ((i == 9) && (j == 91)) Debugger(11);
                               int index1;
                               index1 = Get3DIndex(i,j,0);  //O liteobjt calcula a radia??o ? superf?cie para a camada 0
                               MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtTop, index1,"Total surface irradiance",ObjectCode);
                               //if ((i == 9) && (j == 91)) Debugger(TotalLightAtTop);
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
                      Production(index, PARLightAtTop, PARLightAtBottom);
                      Respiration(index);
                      Exudation(index);
                      NitrogenUptake(index);
                      PhosphorusUptake(index);
                      Settling(index);
                }
                Mortality(index);
                NPP[index] = PhytoProd[index];
                PhytoFlux[index] = PhytoFlux[index] + PhytoProd[index];
                MyPOMFlux = PhytoProd[index] - PhytoMortality[index] - SettlingLoss[index];
                //Importa separar o PhytoFlux do MyPOMFlux, pois o primeiro pode ser afectado por trocas
                //com outras classes, enquanto que o segundo ? somente devido aos processos internos do fito.
                //Quando algum objecto remover/adicionar Phyto, deve tamb?m remover/adicionar POM no objecto de SPM.
                if (MySPMPointer != NULL)
                {
                   //MyPOMFlux = PhytoFlux[index] * ChlCarbonRatio[index] * // mg Chl / m3 / s
                   //            ChlToBiomass[index] / CUBIC; //convertion from mg Chl / m3 / d to mg DW / L / s
                   MyPOMFlux = MyPOMFlux * ChlCarbonRatio[index] * // mg Chl / m3 / s
                               ChlToBiomass[index] / CUBIC; //convertion from mg Chl / m3 / d to mg DW / L / s
                   MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / s
                                index,
                                    "POM",
                                        ObjectCode);
                   MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / s
                                 index,
                                    "TPM",
                                        ObjectCode);
                }
             }
          }
       }
    }
}

void TPhytoplankton3D::Integrate()
{
   //Debugger(1);
   int index;
   double AValue, NPhytoLoad, PPhytoLoad, Depth;
   double const Tinny = 0.000001;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
   {
      //Debugger(LayerNr);
      for (int j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
      {
          for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
          {
             if (i <= pSubDomain->FLine[j])
             {
                 index = Get3DIndex(i,j,LayerNr);
                 GenericProduct1[index] = PhytoBiomass[index] * DailyMeanGPP[index];
                 if (GenericProduct1[index] < 0.0) GenericProduct1[index] = 0.0;
             }
          }
      }
   }

   //Debugger(2);

   if (MyTransportPointer != NULL)
      MyTransportPointer->Go(GenericProduct1, GenericProduct2, 0.0, 0.0);

   //Debugger(PhytoFlux[Get3DIndex(87,60,0)]);

   Generic = PhytoBiomass; GenericFlux = PhytoFlux;
   Integration3DSubDomain(UpperLayer);
   PhytoBiomass = Generic; PhytoFlux = GenericFlux;

   Generic = NPhyto; GenericFlux = NCellFlux;
   Integration3DSubDomain(UpperLayer);
   NPhyto = Generic; NCellFlux = GenericFlux;

   Generic = PPhyto; GenericFlux = PCellFlux;
   Integration3DSubDomain(UpperLayer);
   PPhyto = Generic; PCellFlux = GenericFlux;

   //Debugger(PhytoBiomass[Get3DIndex(87,60,0)]);
   
   if (MyTransportPointer != NULL)
   {
        MyTransportPointer->Go(PhytoBiomass, PhytoLoad, 0.0, 0.0);
        MyTransportPointer->Go(NPhyto, PhytoNLoad, 0.0, 0.0);
        MyTransportPointer->Go(PPhyto, PhytoPLoad,0.0, 0.0);
        MyTransportPointer->Go(GenericProduct1, GenericProduct2, 0.0, 0.0);
   }
   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
   for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
   {
      for (int j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
      {
          for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
          {
             if (i <= pSubDomain->FLine[j])
             {
                index = Get3DIndex(i,j,LayerNr);
                if (PhytoBiomass[index] > Tinny)
                   DailyMeanGPP[index] = GenericProduct1[index] / PhytoBiomass[index];
                else
                   DailyMeanGPP[index] = 0.0;
                if (MyTransportPointer != NULL)
                {
                    MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                                       index,
                                                       "Box depth",
                                                       ObjectCode);
                }
                else Depth = MyPEcoDynClass->GetBoxDepth(index);
                /*if (Depth > CriticalDepthForLandBoundary) {
                if ( (PhytoBiomass[index] < 0.0) || (PhytoBiomass[index] > 10000)) {
                Debugger(i); Debugger(j); Debugger(LayerNr); Debugger(index);
                Debugger(PhytoBiomass[index]);} }  */
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
                   PhytoBiomass[index] = DailyBoundaryPhytoplankton[JulianDay-1];
                   NPhyto[index] = DailyBoundaryPhytoplankton[JulianDay-1] * NCellQuota[index];
                   PPhyto[index] = DailyBoundaryPhytoplankton[JulianDay-1] * PCellQuota[index];
                }
                if (PhytoBiomass[index] < 0.0) PhytoBiomass[index] = 0.0;
                if (NPhyto[index] < 0.0) NPhyto[index] = 0.0;
                if (PPhyto[index] < 0.0) PPhyto[index] = 0.0;

                if (PhytoBiomass[index] > Tinny)
                {
                  NCellQuota[index] = NPhyto[index] / PhytoBiomass[index];
                  PCellQuota[index] = PPhyto[index] / PhytoBiomass[index];
                }
                if (MySPMPointer != NULL)
                {
                   MySPMPointer->Update(GetEcoDynClassName(), PhytoBiomass[index] / CUBIC,//mg C / L
                                        index, "LiveC",ObjectCode);
                   MySPMPointer->Update(GetEcoDynClassName(), NPhyto[index] / CUBIC,     //mg N / L
                                        index, "LiveN",ObjectCode);
                   MySPMPointer->Update(GetEcoDynClassName(), PPhyto[index] / CUBIC,     //mg P / L
                                        index,"LiveP",ObjectCode);
                }
             }
          }
      }
   }
   Loads();
}

void TPhytoplankton3D::Production(int ABoxNumber, double PARLightAtTop, double PARLightAtBottom)
{
   double BoxDepth/*, TotalLightAtTop, TotalLightAtBottom, PARLightAtTop, PARLightAtBottom, TotalBoxDepth*/;
   int MyBoxNumber/*, MyLayer*/;
   MyBoxNumber = ABoxNumber;
   /*MyLayer = GetLayerIndex(MyBoxNumber);
   TotalLightAtTop = 0.0; TotalLightAtBottom = 0.0;
   PARLightAtTop = 0.0; PARLightAtBottom = 0.0;
   TotalBoxDepth = 0.0; */
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
     * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
     * MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
     * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      if (MyLightPointer != NULL)
      {
          /*for (int k = UpperLayer; k--; k <= MyLayer)
          {  */
             if (MyTransportPointer != NULL)
                MyTransportPointer->Inquiry(GetEcoDynClassName(), BoxDepth, MyBoxNumber,"Box depth",ObjectCode);
             else
                BoxDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
             //TotalBoxDepth = TotalBoxDepth + BoxDepth;

             if (MySPMPointer != NULL)
                 MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,MyBoxNumber,"Extinction coefficient Ktot", ObjectCode);
             /*
             if (k == UpperLayer)
             {
                 MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtTop, MyBoxNumber,"Total surface irradiance",ObjectCode);
                 MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtTop, MyBoxNumber,"PAR surface irradiance",ObjectCode);
                 //MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtBottom, BoxDepth, MyBoxNumber,"Sub-surface irradiance",ObjectCode);
                 //MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtBottom, BoxDepth, MyBoxNumber,"Sub-surface PAR irradiance",ObjectCode);
                 MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtBottom,
                                                             TotalLightAtTop,
                                                             ADepth,
                                                             MyExtinctionCoeff,
                                                             index,
                                                             "Sub-surface irradiance",
                                                             ObjectCode);
                 MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtBottom,
                                                             PARLightAtTop,
                                                             ADepth,
                                                             MyExtinctionCoeff,
                                                             index,
                                                             "Sub-surface PAR irradiance",
                                                             ObjectCode);

             }
             else
             {
                 TotalLightAtTop = TotalLightAtBottom;
                 PARLightAtTop = PARLightAtBottom;
                 MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtBottom,
                                         TotalLightAtTop, TotalBoxDepth, KValue, MyBoxNumber,
                                         "Sub-surface irradiance", ObjectCode);
                 MyLightPointer->Inquiry(GetEcoDynClassName(), PARLightAtBottom,
                                         TotalLightAtTop, TotalBoxDepth, KValue, MyBoxNumber,
                                         "Sub-surface PAR irradiance", ObjectCode);
             }

          } */
          Productivity = PhytoBiomass[MyBoxNumber]
                                   * Pmax[MyBoxNumber]
                                   * 2.718282
                     	           /( KValue * BoxDepth)
                                   * ( exp ( -PARLightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                                   -exp( -PARLightAtTop * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                                   ) / HOURSTOSECONDS;    //mg C m-3 s-1

          if (MyWaterTemperaturePointer != NULL)
          {
             MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                           MyBoxNumber,
                                           "Water temperature",
                                           ObjectCode);
             Productivity = Productivity * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminPhotosynthesis);
          }

          NutrientLimitation(MyBoxNumber);

          if (MyPEcoDynClass->GetJulianDay() != ADay[MyBoxNumber])
          {
             DailyMeanGPP[MyBoxNumber] = 0.0;
             NumberOfParcels[MyBoxNumber] = 0;
             ADay[MyBoxNumber] = ADay[MyBoxNumber] + 1;
          }
          DailyMeanGPP[MyBoxNumber] =
              (
                     DailyMeanGPP[MyBoxNumber] * NumberOfParcels[MyBoxNumber] +
                     Productivity / CarbonToOxygenProd / (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * HOURSTOSECONDS
              )  / (NumberOfParcels[MyBoxNumber] + 1);  //mg O2 / mg Chla / h
          NumberOfParcels[MyBoxNumber] = NumberOfParcels[MyBoxNumber] + 1;
      }
      else Productivity = 0.0;
      GPP[MyBoxNumber] = Productivity;
      PhytoProd[MyBoxNumber] = Productivity;
      OxygenNetProduction[MyBoxNumber] = Productivity / CarbonToOxygenProd;
   }
}

void TPhytoplankton3D::Settling(int MyBoxNumber)
{
   double MyDepth, CLoss, NLoss, PLoss, MySettlingSpeed, MyLowerDepth;
   int MyLayer, MyLine, MyColumn, index;

   MyLine = GetLineIndex3D(MyBoxNumber);
   MyColumn = GetColumnIndex3D(MyBoxNumber);
   MyLayer = GetLayerIndex(MyBoxNumber);
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
        MySettlingSpeed = SettlingSpeed / DAYSTOSECONDS; //m/s
        TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
        TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
        if (MyTransportPointer != NULL)
           MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,MyBoxNumber,"Box depth",ObjectCode);
        else
           MyDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
        CLoss = MySettlingSpeed * PhytoBiomass[MyBoxNumber] / MyDepth;     //mg CPhyto / m3 / s
        SettlingLoss[MyBoxNumber] = CLoss;
        PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - SettlingLoss[MyBoxNumber];
        NLoss = MySettlingSpeed * NPhyto[MyBoxNumber] / MyDepth;     //mg NPhyto / m3 / s
        NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - NLoss;
        PLoss = MySettlingSpeed * PPhyto[MyBoxNumber] / MyDepth;     //mg PPhyto / m3 / s
        PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - PLoss;
        index = Get3DIndex(MyLine,MyColumn,MyLayer - 1);

        MyLowerDepth = -1.0;
        if (MyTransportPointer != NULL)
           MyTransportPointer->Inquiry(GetEcoDynClassName(), MyLowerDepth,index,"Box depth",ObjectCode);
        else
           MyLowerDepth = MyPEcoDynClass->GetBoxDepth(index);

        if (
             (MySedimentPointer != NULL) &&
             (
               (MyLayer == 0) || ((MyLayer > 0 ) && (MyDepth < CriticalDepthForLandBoundary))
             )
           )
        {
            MySedimentPointer->Update(GetEcoDynClassName(), CLoss * MyDepth / ChlToCarbon[MyBoxNumber] * ChlToBiomass[MyBoxNumber]/*mg / m2 / s*/, MyBoxNumber, "OrganicMass 1", ObjectCode);
            MySedimentPointer->Update(GetEcoDynClassName(), CLoss * MyDepth/*mg / m2 / s*/, MyBoxNumber, "Corganic 1", ObjectCode);
            MySedimentPointer->Update(GetEcoDynClassName(), NLoss * MyDepth, MyBoxNumber,"Norganic 1",ObjectCode);
            MySedimentPointer->Update(GetEcoDynClassName(), PLoss * MyDepth, MyBoxNumber, "Porganic 1", ObjectCode);
        }
        else if ((MyLayer > 0) && (MyLowerDepth > CriticalDepthForLandBoundary))
        {
            PhytoFlux[index] = PhytoFlux[index] + SettlingLoss[MyBoxNumber] * MyDepth / MyLowerDepth;  //mg CPhyto / m3 / s
            NCellFlux[index] = NCellFlux[index] + NLoss * MyDepth / MyLowerDepth;                      //mg NPhyto / m3 / s
            PCellFlux[index] = PCellFlux[index] + PLoss * MyDepth / MyLowerDepth;                      //mg PPhyto / m3 / s
        }
   }
}



void TPhytoplankton3D::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    //Debugger(NumberOfLoads);
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
                      "EcoDynamo Alert - TPhytoplankton3D::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TPhytoplankton3D::Loads (Loads) Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files" << endl;
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
            PhytoLoad[index] = ALoadRecord[j].PhytoplanktonCLoad[ADay-1];
            PhytoNLoad[index] = ALoadRecord[j].PhytoplanktonNLoad[ADay-1];
            PhytoPLoad[index] = ALoadRecord[j].PhytoplanktonPLoad[ADay-1];
            //Debugger(PhytoLoad[index]);
        }
    }
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        index = Get3DIndex(ALoadRiverRecord[j].LineCoordinate, ALoadRiverRecord[j].ColumnCoordinate, ALoadRiverRecord[j].LayerCoordinate);
        /*Debugger(22);
        Debugger(ALoadRiverRecord[j].LineCoordinate);
        Debugger(ALoadRiverRecord[j].ColumnCoordinate);
        Debugger(ALoadRiverRecord[j].LayerCoordinate);
        Debugger(index);  */
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TPhytoplankton3D::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TPhytoplankton3D::Loads (RiverLoads) Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files" << endl;
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
            PhytoBiomass[index] = ALoadRiverRecord[j].PhytoplanktonCRiverLoad[ADay-1];
            NPhyto[index] = ALoadRiverRecord[j].PhytoplanktonNRiverLoad[ADay-1];
            PPhyto[index] = ALoadRiverRecord[j].PhytoplanktonPRiverLoad[ADay-1];
        }
    }
}

// AP, 2009.04.08
bool TPhytoplankton3D::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Phytoplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Phytoplankton");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' values
     *
     * 1st line
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        // variables from TCrestumaLeverPhytoplankton2DV
        if (strcmp(VariableNameArray[i], "Phytoplankton biomass") == 0)
            PReadWrite->WriteCell(RiverPhyto / ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton initial biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonInitialBiomass, 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton final biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonFinalBiomass, 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll : Carbon") == 0)
            PReadWrite->WriteCell(ChlCarbonRatio[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to biomass") == 0)
            PReadWrite->WriteCell(ChlToBiomass[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Simulation interval (hours)") == 0)
            PReadWrite->WriteCell(SimulationInterval, 8);

        // variables from TCrestumaLeverPhytoplankton2DVIntLim
        else if (strcmp(VariableNameArray[i], "NCellQuota") == 0)
            PReadWrite->WriteCell(NCellQuota[0], 8);

        else if (strcmp(VariableNameArray[i], "PCellQuota") == 0)
            PReadWrite->WriteCell(PCellQuota[0], 8);
    }
    PReadWrite->WriteSeparator(true);
    /*
     * 2nd line - repeat some values of variables
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Chlorophyll : Carbon") == 0)
            PReadWrite->WriteCell(ChlCarbonRatio[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to biomass") == 0)
            PReadWrite->WriteCell(ChlToBiomass[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Simulation interval (hours)") == 0)
            PReadWrite->WriteCell(SimulationInterval, 8);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values - from TCrestumaLeverPhytoplankton2DV
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save loads values
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("RiverLoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadLayers");    // from TPhytoplankton3D
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverName");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        PReadWrite->WriteCell(GridLines - ALoadRiverRecord[j].LineCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRiverRecord[j].ColumnCoordinate + 1, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRiverRecord[j].RiverName);
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("PhytoPlanktonBiomassRiverLoad");
    // from TRiaF2DPhytoplankton
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonCRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonNRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonPRiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonBiomassLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonCRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonNRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonPRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    /*
     * from TRiaF2DPhytoplankton
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("LoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadLayers");    // from TPhytoplankton3D
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadName");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        PReadWrite->WriteCell(GridLines - ALoadRecord[j].LineCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRecord[j].ColumnCoordinate + 1, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NumberOfLayers - ALoadRecord[j].LayerCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRecord[j].LoadName);
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("PhytoPlanktonCLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonNLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonPLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRecord[j].PhytoplanktonCLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].PhytoplanktonNLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].PhytoplanktonPLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    /*
     * daily boundaries
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Daily boundary phytoplankton");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(DailyBoundaryPhytoplankton[j] / ChlToCarbon[0], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

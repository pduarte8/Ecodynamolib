// SPM object CPP code file
// @ Pedro Duarte

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
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "spmobjt.h"
#include "iodll.h"

TSPM3D::TSPM3D(TEcoDynClass* APEcoDynClass,
        char* className) : TRiaFormosaResuspendDeposit(APEcoDynClass, className)
{
    BuildTSPM3D();
    JulianDay = MyPEcoDynClass->GetJulianDay();
}

void TSPM3D::BuildTSPM3D()
{
    char MyVariable[65], LoadName[100], RiverName[100];
    int X, Y, XV, YV, XL, YL;
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            if (PReadWrite->FindString("LoadLayers", XV, YV))
            {
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
                  ALoadRiverRecord[j].LayerCoordinate = NumberOfLayers - ALoadRiverRecord[j].LayerCoordinate;
                  //Debugger(ALoadRiverRecord[j].LayerCoordinate);
               }
            }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "Undefined object - Suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSPM3D::BuildTSPM3D - Undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

void TSPM3D::Go()
{
    int index;
    double AValue;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
    //Debugger(MyNumberOfBoxesInSubDomain);
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
        UpperLayer = ceil(AValue);
    }
    else UpperLayer = NumberOfLayers - 1;
    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
       {
          for (int LayerNr = UpperLayer; LayerNr >= 0; LayerNr--)
          {
             index = Get3DIndex(i,j,LayerNr);
             LiveC[index] = 0.0; LiveN[index] = 0.0; LiveP[index] = 0.0;
             if (MyTransportPointer != NULL)
             {
                MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,index,"Box depth",ObjectCode);
             }
             else
                ABoxDepth = MyPEcoDynClass->GetBoxDepth(index);
             if (
                    (ABoxDepth > CriticalWaterDepth) &&
                    (BoxArray[index].Dummybool1) &&
                    (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
                    (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) ||   //No northern ocean boundary
                    (
                       (ABoxDepth > 0.0) &&
                       (MyTransportPointer == NULL)
                    )
                )
             {
                Deposit(index);
                Ressuspend(index);
                CarbonMineralization(index);
                NitrogenMineralization(index);
                PhosphorusMineralization(index);
                DissolutionOfSuspendedOrganics(index);
                //ExtinctionCoefficientKTot(index);
             }
          }
       }
    }
}


void TSPM3D::Integrate()
{

    double MyBoxDepth; int index;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    
    Generic = SPMConcentration; GenericFlux = SPMFlux;
    Integration3DSubDomain(UpperLayer);
    SPMConcentration = Generic; SPMFlux = GenericFlux;

    Generic = POMConcentration; GenericFlux = POMFlux;
    Integration3DSubDomain(UpperLayer);
    POMConcentration = Generic; POMFlux = GenericFlux;

    Generic = DetritalC; GenericFlux = DetritalCFlux;
    Integration3DSubDomain(UpperLayer);
    DetritalC = Generic; DetritalCFlux = GenericFlux;

    Generic = DetritalN; GenericFlux = DetritalNFlux;
    Integration3DSubDomain(UpperLayer);
    DetritalN = Generic; DetritalNFlux = GenericFlux;

    Generic = DetritalP; GenericFlux = DetritalPFlux;
    Integration3DSubDomain(UpperLayer);
    DetritalP = Generic; DetritalPFlux = GenericFlux;

    // Transport the SPM


    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Go(SPMConcentration, TPMLoad,0.0, 0.0);
        MyTransportPointer->Go(POMConcentration, POMLoad, 0.0, 0.0);
        MyTransportPointer->Go(DetritalC, DetritalCLoad, 0.0, 0.0);
        MyTransportPointer->Go(DetritalN, DetritalNLoad, 0.0, 0.0);
        MyTransportPointer->Go(DetritalP, DetritalPLoad, 0.0, 0.0);
   	    for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
        {
          for (int j = pSubDomain->FirstColumn; j <= pSubDomain->LastColumn; j++)
          {
              for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
              {
                 if (i <= pSubDomain->FLine[j])
                 {
                     index = Get3DIndex(i,j,LayerNr);
                     if (i <= pSubDomain->FLine[j])
                     {
                        MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                        index,"Box depth",ObjectCode);
                        if (
                              (MyBoxDepth > 0.0) &&
                              (
                                  (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  || //Southern ocean boundary
                                  (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) ||//Northern ocean boundary
                                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) ||//Eastern ocean boundary
                                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Western ocean boundary
                              )
                            )
                        {
                           SPMConcentration[index] = BoundarySPMConcentration[JulianDay-1];
                           POMConcentration[index] = BoundaryPOMConcentration[JulianDay-1];
                           DetritalC[index] = BoundaryDetritalC[JulianDay-1];
                           DetritalN[index] = BoundaryDetritalN[JulianDay-1];
                           DetritalP[index] = BoundaryDetritalP[JulianDay-1];
                        }
                        if (SPMConcentration[index] < 0.0) SPMConcentration[index] = 0.0;
                        if (POMConcentration[index] < 0.0) POMConcentration[index] = 0.0;
                        if (DetritalC[index] < 0.0) DetritalC[index] = 0.0;
                        if (DetritalN[index] < 0.0) DetritalN[index] = 0.0;
                        if (DetritalP[index] < 0.0) DetritalP[index] = 0.0;
                        if (LiveC[index] < 0.0) LiveC[index] = 0.0;
                        if (LiveN[index] < 0.0) LiveN[index] = 0.0;
                        if (LiveP[index] < 0.0) LiveP[index] = 0.0;
                        ExtinctionCoefficientKTot(index);
                     }
                 }
              }
           }
        }
    }
    Loads();
}



void TSPM3D::Deposit(int ABox)
{
   TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   double TotalDeposition, OrganicDeposition, CDeposition, NDeposition, PDeposition;
   double MyPhytoBiomass, MyChlorophyllToBiomass, PHYORG, MySinkingVelocity, MyPOMSinkingVelocity, ABoxDepth, ALowerBoxDepth;
   int MyBox, MyLine, MyColumn, MyLayer,/*LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber,*/ index;
   MyBox = ABox;
   MyLine = GetLineIndex3D(MyBox);
   MyColumn = GetColumnIndex3D(MyBox);
   MyLayer = GetLayerIndex(MyBox);

   MySinkingVelocity = SinkingVelocity / DAYSTOSECONDS;
   MyPOMSinkingVelocity = POMSinkingVelocity / DAYSTOSECONDS;
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

   if (MyTransportPointer != NULL)
      MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,MyBox,"Box depth",ObjectCode);
   else
      ABoxDepth = MyPEcoDynClass->GetBoxDepth(MyBox);

   if (POMConcentration[MyBox] > PHYORG)
   {
      OrganicDeposition = MyPOMSinkingVelocity * (POMConcentration[MyBox] - PHYORG) / ABoxDepth;
      CDeposition = MyPOMSinkingVelocity * DetritalC[MyBox]/ ABoxDepth;
      NDeposition = MyPOMSinkingVelocity * DetritalN[MyBox]/ ABoxDepth;
      PDeposition = MyPOMSinkingVelocity * DetritalP[MyBox]/ ABoxDepth;
   }
   else
   {
      OrganicDeposition = 0.0;
      CDeposition = 0.0;
      NDeposition = 0.0;
      PDeposition = 0.0;
   }
   if (SPMConcentration[MyBox] > POMConcentration[MyBox])
      TotalDeposition = MySinkingVelocity * (SPMConcentration[MyBox] - PHYORG - POMConcentration[MyBox])  / ABoxDepth +
                        OrganicDeposition;   //mg / L / s
   else
      TotalDeposition = 0.0;

   index = Get3DIndex(MyLine,MyColumn,MyLayer - 1);


   ALowerBoxDepth = -1.0;
   if (MyLayer > 0) {
     if (MyTransportPointer != NULL)
        MyTransportPointer->Inquiry(GetEcoDynClassName(), ALowerBoxDepth,index,"Box depth",ObjectCode);
     else
        ALowerBoxDepth = MyPEcoDynClass->GetBoxDepth(index);
   }
   //if ((MyLine == 1) && (MyColumn > 188) && (MyLayer == 0)) Debugger(16);
   if ((MySedimentPointer != NULL) &&
       (
         (MyLayer == 0) || ((MyLayer > 0 ) && (ALowerBoxDepth < CriticalWaterDepth))
       )
      ) 
   {
      MySedimentPointer->Update(GetEcoDynClassName(), CDeposition * CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "Corganic 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), NDeposition * CUBIC * /*SedimentDepth*/ABoxDepth, MyBox,"Norganic 1",ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), PDeposition * CUBIC * /*SedimentDepth*/ABoxDepth, MyBox, "Porganic 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), OrganicDeposition * CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "OrganicMass 1", ObjectCode);
      MySedimentPointer->Update(GetEcoDynClassName(), (TotalDeposition - OrganicDeposition)* CUBIC * /*SedimentDepth*/ABoxDepth/*mg / m2 / s*/, MyBox, "InorganicMass 1", ObjectCode);
   }
   else if ((MyLayer > 0) && (ALowerBoxDepth > CriticalWaterDepth))
   {
      //if ((MyLine == 1) && (MyColumn > 188) && (MyLayer == 0)) Debugger(18);
      SPMFlux[index] = SPMFlux[index] + TotalDeposition * ABoxDepth / ALowerBoxDepth;
      POMFlux[index] = POMFlux[index] + OrganicDeposition * ABoxDepth / ALowerBoxDepth;
      DetritalCFlux[index] = DetritalCFlux[index] + CDeposition * ABoxDepth / ALowerBoxDepth;
      DetritalNFlux[index] = DetritalNFlux[index] + NDeposition * ABoxDepth / ALowerBoxDepth;
      DetritalPFlux[index] = DetritalPFlux[index] + PDeposition * ABoxDepth / ALowerBoxDepth;
      //if ((MyLine == 1) && (MyColumn > 188) && (MyLayer == 0)) Debugger(19);
   }

   DepositionFlux[MyBox] = TotalDeposition * DAYSTOSECONDS; //mg TPM/L/d
   SPMFlux[MyBox] = SPMFlux[MyBox] - TotalDeposition;
   POMFlux[MyBox] = POMFlux[MyBox] - OrganicDeposition;
   DetritalCFlux[MyBox] = DetritalCFlux[MyBox] - CDeposition;
   DetritalNFlux[MyBox] = DetritalNFlux[MyBox] - NDeposition;
   DetritalPFlux[MyBox] = DetritalPFlux[MyBox] - PDeposition;
}


void TSPM3D::Loads()
{
    int index, ADay/*, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber*/;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    for (int j = 0; j < NumberOfLoads; j++)
    {
        index = Get3DIndex(ALoadRecord[j].LineCoordinate, ALoadRecord[j].ColumnCoordinate, ALoadRecord[j].LayerCoordinate);
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TSPM3D::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSPM3D::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files" << endl;
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
        //if (Depth > CriticalWaterDepth)
        if (Depth > 0.0)
        {
            //Debugger(index); Debugger(ALoadRecord[j].LineCoordinate);
            //Debugger(ALoadRecord[j].ColumnCoordinate);Debugger(ALoadRecord[j].TPMLoad[ADay-1]);
            TPMLoad[index] = ALoadRecord[j].TPMLoad[ADay-1];
            POMLoad[index] = ALoadRecord[j].POMLoad[ADay-1];
            DetritalCLoad[index] = ALoadRecord[j].DetritalCLoad[ADay-1];
            DetritalNLoad[index] = ALoadRecord[j].DetritalNLoad[ADay-1];
            DetritalPLoad[index] = ALoadRecord[j].DetritalPLoad[ADay-1];
        }
    }
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        index = Get3DIndex(ALoadRiverRecord[j].LineCoordinate, ALoadRiverRecord[j].ColumnCoordinate, ALoadRiverRecord[j].LayerCoordinate);
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TSPM3D::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSPM3D::Loads (River) - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files" << endl;
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
        //if (Depth > CriticalWaterDepth)
        if (Depth > 0.0)
        {
            SPMConcentration[index] = ALoadRiverRecord[j].TPMRiverLoad[ADay-1];
            POMConcentration[index] = ALoadRiverRecord[j].POMRiverLoad[ADay-1];
            DetritalC[index] = ALoadRiverRecord[j].DetritalCRiverLoad[ADay-1];
            DetritalN[index] = ALoadRiverRecord[j].DetritalNRiverLoad[ADay-1];
            DetritalP[index] = ALoadRiverRecord[j].DetritalPRiverLoad[ADay-1];
        }
    }
}

// AP, 2009.04.08 - imported from TRiaFormosaResuspendDeposit
bool TSPM3D::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Extinction coefficient Ktot") == 0)
        {
            PReadWrite->WriteCell(TotalExtinctionCoefficient[0], 8);
            break;
        }
    }
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteSeparator(true);

    /*
     * save other variables' initial values
     */
    PReadWrite->WriteCell("Boundary SPM concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary POM concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryDetritalC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryDetritalN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryDetritalP values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryLiveC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryLiveN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("BoundaryLiveP values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(BoundarySPMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryPOMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDetritalC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDetritalN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryDetritalP[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryLiveC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryLiveN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryLiveP[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("TPM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POM values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalP values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LiveC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LiveN values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LiveP values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(SPMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(POMConcentration[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DetritalC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DetritalN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DetritalP[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(LiveC[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(LiveN[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(LiveP[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    // Loads
    PReadWrite->WriteCell("LoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadLayers");
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

    PReadWrite->WriteCell("TPMLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POMLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalCLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalNLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalPLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRecord[j].TPMLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].POMLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DetritalCLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DetritalNLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].DetritalPLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    // Rivers
    PReadWrite->WriteCell("RiverLoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadLayers");
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

    PReadWrite->WriteCell("TPMRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("POMRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalCRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalNRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DetritalPRiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForBoundaryTimeSeries; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].TPMRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].POMRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DetritalCRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DetritalNRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].DetritalPRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


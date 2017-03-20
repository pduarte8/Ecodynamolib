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

//This class implements the DEB approach based on the weak homeosthasis assumption
//For details about the above mentioned assumption see Kooijman, 2000. Dynamic Energy and Mass Budgets in Biological Systems, page 31
TFishWithWeakHomeostasis::TFishWithWeakHomeostasis(TEcoDynClass* APEcoDynClass, char* className)
							 :	TFish(APEcoDynClass, className)
{
   AverageRQ = 1.0;            //Default value
   PolysaccharidesRQ = 1.00;   //cf. - Kooijman, 2000. Dynamic Energy and Mass Budgets in Biological Systems, page 137
   LipidsRQ = 0.67;            //cf. - Kooijman, 2000. Dynamic Energy and Mass Budgets in Biological Systems, page 137
   ProteinsRQ = 0.84;          //cf. - Kooijman, 2000. Dynamic Energy and Mass Budgets in Biological Systems, page 137
   PolysaccharideContent = 0.015;   //proportion referred to food dry weight
   LipidContent = 0.135;            //proportion referred to food dry weight
   ProteinContent = 0.43;           //proportion referred to food dry weight
   FoodCContent = 0.413;            //proportion referred to food dry weight
   FoodNContent = 0.066;             //proportion referred to food dry weight
   FoodPContent = 0.012;            //proportion referred to food dry weight
   FoodCNratio =  FoodCContent / FoodNContent;
   FoodCPratio =  FoodCContent / FoodPContent;
   FishCNratio =  6.0;
   FishCPratio =  36.6;
   FoodEnergyContent = 23.8;//J/mg(DW)
}

void TFishWithWeakHomeostasis::Feed(int ABox, int AFish)
{
   double Xk, JXm, F, TA, T1;
   Inquiry(GetEcoDynClassName(), Xk, AFish, ABox,"Xk",ObjectCode);
   Inquiry(GetEcoDynClassName(), JXm, AFish, ABox,"JXm",ObjectCode);
   Inquiry(GetEcoDynClassName(), TA, AFish, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, AFish, ABox,"T1",ObjectCode);
   if  (Xk + FoodLoad > TINNY)
      F = FoodLoad / (Xk + FoodLoad);
   else
      F = 0.0;
   MyIndividualRecord[AFish].Jx = JXm * F * pow(MyIndividualRecord[AFish].Volume, 0.666666667) * TemperatureLimitation(TA, T1, AWaterTemperature); //J d-1 fish-1
   //Debugger(F); Debugger(Xk); Debugger(FoodLoadPerFish);  Debugger(JXm); Debugger(MyIndividualRecord[AFish].Jx);
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MySPMPointer != NULL)
   {
      double MyVolume, JxInMgPerL;
      if (MyTransportPointer != NULL)
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyVolume,ABox,"Box Volume",ObjectCode);  //m3
      else
         MyVolume = MyPEcoDynClass->GetBoxVolume(ABox); //m3
      JxInMgPerL = MyIndividualRecord[AFish].Jx / FoodEnergyContent / DAYSTOSECONDS / MyVolume / CUBIC; //mg s-1 L-1 fish-1
      MySPMPointer->Update(GetEcoDynClassName(),-JxInMgPerL, ABox,"Food",ObjectCode); //mg/L/s
      MySPMPointer->Update(GetEcoDynClassName(),-JxInMgPerL * FoodCContent, ABox,"FoodC",ObjectCode); //mg/L/s
      MySPMPointer->Update(GetEcoDynClassName(),-JxInMgPerL * FoodNContent, ABox,"FoodN",ObjectCode); //mg/L/s
      MySPMPointer->Update(GetEcoDynClassName(),-JxInMgPerL * FoodPContent, ABox,"FoodP",ObjectCode); //mg/L/s
      MySPMPointer->Update(GetEcoDynClassName(),-JxInMgPerL, ABox,"TPM",ObjectCode);   //mg/L/s
      MySPMPointer->Update(GetEcoDynClassName(),-JxInMgPerL, ABox,"POM",ObjectCode);   //mg/L/s
   }
}


void TFishWithWeakHomeostasis::Absorption(int ABox, int AFish)
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
        MessageBox(0, "TFishWithWeakHomeostasis::Absorption: JXm to small...", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TFishWithWeakHomeostasis::Absorption - JXm to small..." << endl;
#endif  // __BORLANDC__
   }
   else
   {
      Inquiry(GetEcoDynClassName(), AE, AFish, ABox,"AE",ObjectCode);
      //Debugger(AE);
   }
   PA = MyIndividualRecord[AFish].Jx * AE; //J d-1
   MyIndividualRecord[AFish].PA = PA;
}

void TFishWithWeakHomeostasis::Egestion(int ABox, int AFish)
{
   double Egestion, EgestionInMgPerL;
   Egestion = MyIndividualRecord[AFish].Jx - MyIndividualRecord[AFish].PA;
   MyIndividualRecord[AFish].Egestion = Egestion;
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MySPMPointer != NULL)
   {
      double MyVolume, EgestionInMgPerL;
      if (MyTransportPointer != NULL)
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyVolume,ABox,"Box Volume",ObjectCode);  //m3
      else
         MyVolume = MyPEcoDynClass->GetBoxVolume(ABox); //m3
      EgestionInMgPerL = Egestion / FoodEnergyContent / DAYSTOSECONDS / MyVolume / CUBIC;  //mg s-1 L-1 fish-1
      MySPMPointer->Update(GetEcoDynClassName(),EgestionInMgPerL * FoodCContent, ABox,"DetritalC",ObjectCode); //mg/L
      MySPMPointer->Update(GetEcoDynClassName(),EgestionInMgPerL * FoodNContent, ABox,"DetritalN",ObjectCode); //mg/L
      MySPMPointer->Update(GetEcoDynClassName(),EgestionInMgPerL * FoodPContent, ABox,"DetritalP",ObjectCode); //mg/L
      MySPMPointer->Update(GetEcoDynClassName(),EgestionInMgPerL, ABox,"TPM",ObjectCode);   //mg/L
      MySPMPointer->Update(GetEcoDynClassName(),EgestionInMgPerL, ABox,"POM",ObjectCode);   //mg/L
   }
}

void TFishWithWeakHomeostasis::Respire(int ABox, int AFish)
{
   int index;
   double PM, K, Eg, TA, T1, MyVolume, OxygenConsumption, Em;
   index = AFish;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
               * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
               * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   Inquiry(GetEcoDynClassName(), PM, AFish, ABox,"PM",ObjectCode);   //J/cm3/d
   Inquiry(GetEcoDynClassName(), K, AFish, ABox,"K",ObjectCode);
   Inquiry(GetEcoDynClassName(), Eg, AFish, ABox,"Eg",ObjectCode);
   Inquiry(GetEcoDynClassName(), Em, AFish, ABox,"Em",ObjectCode);
   MyVolume = MyIndividualRecord[index].Volume;
   MyIndividualRecord[index].RespR = PM * MyVolume /
                                     (Eg + K * MyIndividualRecord[index].Reserves / MyVolume);//cm3 d-1
   Inquiry(GetEcoDynClassName(), TA, AFish, ABox,"TA",ObjectCode);
   Inquiry(GetEcoDynClassName(), T1, AFish, ABox,"T1",ObjectCode);
   MyIndividualRecord[index].RespR = MyIndividualRecord[index].RespR * TemperatureLimitation(TA,T1,AWaterTemperature);
   //OxygenConsumption = PM * MyVolume / OxygenToJoules; //mg O2 d-1 por peixe
   OxygenConsumption = (MyIndividualRecord[index].Reserves / MyVolume) / (K * (MyIndividualRecord[index].Reserves / MyVolume) + Eg)*
       ((PAm * Eg / Em) * pow(MyVolume,0.666666667) + PM * MyVolume) * TemperatureLimitation(TA,T1,AWaterTemperature) / OxygenToJoules;
   if (MyNutrientPointer != NULL)
   {
      double MyVolume;
      if (MyTransportPointer != NULL)
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyVolume,ABox,"Box Volume",ObjectCode);  //m3
      else
         MyVolume = MyPEcoDynClass->GetBoxVolume(ABox); //m3
         
      MyNutrientPointer->Update(GetEcoDynClassName(),
                                -OxygenConsumption / DAYSTOSECONDS / MyVolume / CUBIC, //mg/L
                                ABox,"Oxygen",ObjectCode);
   }
   if (MySPMPointer != NULL)
   {
      MySPMPointer->Inquiry(GetEcoDynClassName(), FoodC, ABox,"FoodC",ObjectCode);
      MySPMPointer->Inquiry(GetEcoDynClassName(), FoodN, ABox,"FoodN",ObjectCode);
      MySPMPointer->Inquiry(GetEcoDynClassName(), FoodP, ABox,"FoodP",ObjectCode);
      if (FoodN > TINNY)
         FoodCNratio =  FoodC / FoodN;
      else
         FoodCNratio = 0.0;
      if (FoodP > TINNY)
         FoodCPratio =  FoodC / FoodP;
      else
         FoodCPratio = 0.0;
   }
   AverageRQ = (PolysaccharidesRQ * PolysaccharideContent + LipidsRQ * LipidContent + ProteinsRQ * ProteinContent) /
                  (PolysaccharideContent + LipidContent + ProteinContent);
   CarbonLostByResp = AverageRQ * CARBONATOMICWEIGHT / (OXYGENATOMICWEIGHT * 2.0) * OxygenConsumption; //mg C d-1 per fish in CO2 lost through respiration
}


void TFishWithWeakHomeostasis::Excrete(int ABox, int AFish)
{
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
               * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

   double MyVolume;
   if (MyTransportPointer != NULL)
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyVolume,ABox,"Box Volume",ObjectCode);  //m3
   else
      MyVolume = MyPEcoDynClass->GetBoxVolume(ABox); //m3
   if (FoodCNratio > TINNY)
      NitrogenLostByExcretion = CarbonLostByResp / FoodCNratio;  //mg N d-1 per fish in NH4+ lost through excretion
   else
      NitrogenLostByExcretion = 0.0;
   if (FoodCPratio > 0.0)
      PhosphorusLostByExcretion = CarbonLostByResp / FoodCPratio; //mg P d-1 per fish in phosphate lost through excretion
   else
      PhosphorusLostByExcretion = 0.0;
   if (MyNutrientPointer != NULL)
   {
      MyNutrientPointer->Update(GetEcoDynClassName(),
                                NitrogenLostByExcretion / NITROGENATOMICWEIGHT / DAYSTOSECONDS / MyVolume * CUBIC, //umol per m3
                                ABox,"Ammonia",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(),
                                PhosphorusLostByExcretion / PHOSPHORUSATOMICWEIGHT / DAYSTOSECONDS / MyVolume * CUBIC, //umol per m3
                                ABox,"Phosphate",ObjectCode);
   }
}

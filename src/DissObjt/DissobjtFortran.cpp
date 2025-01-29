#include "../include/ecodyn.rh"
#include "dissobjt.h"
#include "../BGCLibraries/LightLimitation.cpp"
#include "../BGCLibraries/TemperatureLimitation.cpp"
#include "../BGCLibraries/BGCCycles.cpp"
#include <math.h>
#include <stdio.h>

void dissobjt_new__(long* PNutrients, double* NitriR, double* kdenit, double* knitO2, double* kdenitO2,  double* kt, double *minRate,
                    double *ProportionOfNH4FromDenitrification,double *FractionMineralizedToAmmonia, double *ThresholdForLightInhib, double *HalfSatForLightInhib)
{
   TRiaF2DNutrients* ptr;
   ptr->SetNumberOfLines(1);
   ptr->SetNumberOfColumns(1);
   ptr->SetNumberOfLayers(1);
   ptr->SetNumberOfBoxes(1);
   ptr = TRiaF2DNutrients::getNuts();
   *PNutrients = (long)ptr;
   ptr->BuildRiaF2DNutrients();
   ptr->SetParameterValue("knit", *NitriR); //Nitrification rate in Fennel model is NitriR but in EcoDynamo the maximum nitrification is knit
   ptr->SetParameterValue("kdenit", *kdenit);
   ptr->SetParameterValue("knitO2", *knitO2);
   ptr->SetParameterValue("kdenitO2", *kdenitO2);
   ptr->SetParameterValue("kt", *kt);
   ptr->SetParameterValue("minRate", *minRate);
   ptr->SetParameterValue("ProportionOfNH4FromDenitrification", *ProportionOfNH4FromDenitrification);
   ptr->SetParameterValue("FractionMineralizedToAmmonia", *FractionMineralizedToAmmonia);
   ptr->SetParameterValue("ThresholdForLightInhib", *ThresholdForLightInhib);
   ptr->SetParameterValue("HalfSatForLightInhib", *HalfSatForLightInhib);
}

void dissobjt_go__(long* PNutrients, double* layerThickness, double* timeStep)
{
   TRiaF2DNutrients* ptr = (TRiaF2DNutrients*) *PNutrients;
   ptr->SetTimeStep(*timeStep);
   ptr->SetABoxDepth(*layerThickness);
   ptr->SetABoxNumber(0);
}

void dissobjt_nitrification__(long* PNutrients, double* lightAtTop, double* lightAtBottom, double* kValue,
                              double* layerThickness, double* waterTemperature,
                              double* Ammonia, double *Oxygen, double *NitrificationFlux, double *OxygenFlux)
{
   double MyLightAtTop, MyKValue, MyDepth, MyWaterTemperature, MyAmmonia, MyOxygen, AverLight;
   double MyTemperatureLimitation = 1.0, MyOxygenLimitation = 1.0, MyLightLimitation = 1.0;
   double OxygenNitrogenRatioInNitrification = 0.064; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (1995) - Ecolog. Modell 80: 131-147)
   double OxyF;
   TRiaF2DNutrients* ptr = (TRiaF2DNutrients*) *PNutrients;
   MyLightAtTop = MAX(0.0,*lightAtTop);
   MyKValue = std::max(0.0,*kValue);
   MyDepth = std::max(0.0,*layerThickness);
   MyWaterTemperature = *waterTemperature;
   MyAmmonia = std::max(0.0,*Ammonia);
   MyOxygen = std::max(0.0,*Oxygen) * (2.0 * OXYGENATOMICWEIGHT) / CUBIC; //Convert to mg O2 L-for compatibility with EcoDynamo
   AverLight = VertLightAvg(MyLightAtTop, MyKValue, MyDepth);
   MyLightLimitation = LightLimNitr(ptr->HalfSatForLightInhib, ptr->ThresholdForLightInhib, AverLight);
   MyTemperatureLimitation = TemperatureExponentialLimitation(MyWaterTemperature, ptr->Kt, 0.0);
   MyOxygenLimitation = MichaelisMentenLimitation(MyOxygen, ptr->knitO2);
   *NitrificationFlux = Nitrification(MyAmmonia, ptr->knit, MyTemperatureLimitation, MyOxygenLimitation, MyLightLimitation) /
                        DAYSTOSECONDS; //mmol N m-3 s-1
   OxyF = *NitrificationFlux * OxygenNitrogenRatioInNitrification; //mg O2 L-1
   *OxygenFlux = OxyF / (2.0 * OXYGENATOMICWEIGHT) * CUBIC; //mmol m-3 s-1
}

void dissobjt_denitrification__(long *PNutrients, double * waterTemperature,double *Nitrate, double *Oxygen, double *AmmoniaFlux, double *NitrateFlux)
{
   double MyWaterTemperature, MyNitrate, MyOxygen;
   double MyTemperatureLimitation = 1.0, MyOxygenLimitation = 1.0;
   TRiaF2DNutrients* ptr = (TRiaF2DNutrients*) *PNutrients;
   MyWaterTemperature = *waterTemperature;
   MyNitrate = std::max(0.0,*Nitrate);
   MyOxygen = std::max(0.0,*Oxygen) * (2.0 * OXYGENATOMICWEIGHT) / CUBIC; //Convert from mmol/m3 to mg O2 L-for compatibility with EcoDynamo
   MyTemperatureLimitation = TemperatureExponentialLimitation(MyWaterTemperature, ptr->Kt, 0.0);
   MyOxygenLimitation = MichaelisMentenLimitation(MyOxygen, ptr->kdenitO2);
   *NitrateFlux = DenitrificationToNH4(MyNitrate,ptr->kdenit,MyTemperatureLimitation,MyOxygenLimitation) / DAYSTOSECONDS;
   *AmmoniaFlux = *NitrateFlux * ptr->ProportionOfNH4FromDenitrification;   //mmol N m-3 s-1
}

void dissobjt_CarbonMineralization__(long *PNutrients, double * waterTemperature,
                                     double *OrganicCarbon, double *Oxygen, double *OrganicCarbonFlux, double *minRateC)
{
   double MyWaterTemperature, MyOrganicCarbon, MyOxygen, MyminRateC;
   double MyTemperatureLimitation = 1.0, MyOxygenLimitation = 1.0;
   TRiaF2DNutrients* ptr = (TRiaF2DNutrients*) *PNutrients;
   MyWaterTemperature = *waterTemperature;
   MyOrganicCarbon = std::max(0.0,*OrganicCarbon);
   MyOxygen = std::max(0.0,*Oxygen) * (2.0 * OXYGENATOMICWEIGHT) / CUBIC; //Convert to mg O2 L-for compatibility with EcoDynamo
   MyTemperatureLimitation = TemperatureExponentialLimitation(MyWaterTemperature, ptr->Kt, 0.0);
   MyOxygenLimitation = MichaelisMentenLimitation(MyOxygen, ptr->kminO2);
   MyminRateC = std::max(0.0, *minRateC);
   *OrganicCarbonFlux = Mineralization(MyminRateC,MyTemperatureLimitation, MyOxygenLimitation, MyOrganicCarbon) / DAYSTOSECONDS;
}

void dissobjt_NitrogenMineralization__(long *PNutrients, double * waterTemperature,
                                     double *OrganicNitrogen, double *Oxygen, double *OrganicNitrogenFlux, double *OxygenFlux, double *minRateN)
{
   double MyWaterTemperature, MyOrganicNitrogen, MyOxygen, MyminRateN;
   double OxygenNitrogenRatio = 0.212; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (2000) - Ecolog. Modell 127: 161-181)^M
   double MyTemperatureLimitation = 1.0, MyOxygenLimitation = 1.0;
   TRiaF2DNutrients* ptr = (TRiaF2DNutrients*) *PNutrients;
   MyWaterTemperature = *waterTemperature;
   MyOrganicNitrogen = std::max(0.0,*OrganicNitrogen);
   MyOxygen = std::max(0.0,*Oxygen) * (2.0 * OXYGENATOMICWEIGHT) / CUBIC; //Convert to mg O2 L-for compatibility with EcoDynamo
   MyTemperatureLimitation = TemperatureExponentialLimitation(MyWaterTemperature, ptr->Kt, 0.0);
   MyOxygenLimitation = MichaelisMentenLimitation(MyOxygen, ptr->kminO2);
   MyminRateN = std::max(0.0, *minRateN);
   *OrganicNitrogenFlux =  Mineralization(MyminRateN,MyTemperatureLimitation,MyOxygenLimitation, MyOrganicNitrogen) / DAYSTOSECONDS;
   *OxygenFlux = *OrganicNitrogenFlux * OxygenNitrogenRatio; //mg l-1 s-1^M
   *OxygenFlux = *OxygenFlux / (2.0 * OXYGENATOMICWEIGHT) * CUBIC; //OxygenFlux in mmol O2 m-3 s-1
}

void dissobjt_PhosphorusMineralization__(long *PNutrients, double * waterTemperature,
                                      double *OrganicPhosphorus, double *Oxygen, double *OrganicPhosphorusFlux, double *minRateP)
{
   double MyWaterTemperature, MyOrganicPhosphorus, MyOxygen, MyminRateP;
   double MyTemperatureLimitation = 1.0, MyOxygenLimitation = 1.0;
   TRiaF2DNutrients* ptr = (TRiaF2DNutrients*) *PNutrients;
   MyWaterTemperature = *waterTemperature;
   MyOrganicPhosphorus = std::max(0.0,*OrganicPhosphorus);
   MyOxygen = std::max(0.0,*Oxygen) * (2.0 * OXYGENATOMICWEIGHT) / CUBIC; //Convert to mg O2 L-for compatibility with EcoDynamo
   MyTemperatureLimitation = TemperatureExponentialLimitation(MyWaterTemperature, ptr->Kt, 0.0);
   MyOxygenLimitation = MichaelisMentenLimitation(MyOxygen, ptr->kminO2);
   MyminRateP = std::max(0.0, *minRateP);
   *OrganicPhosphorusFlux = Mineralization(MyminRateP,MyTemperatureLimitation,MyOxygenLimitation, MyOrganicPhosphorus) / DAYSTOSECONDS;   //Flux in mmol P m-3 s-1
}


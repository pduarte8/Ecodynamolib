#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
#include <array>
using namespace std;


//The functions below are described in:
//Serpa, D., Ferreira, P.P., Caetano, M., Cancela da Fonseca, L., Dinis, M.T., and P. Duarte (2012) Modelling of biogeochemical processes in fish earth ponds: Model development
//and calibration. Ecological Modelling 247: 286-301
//and references therein

double DenitrificationToNH4(double NO3, double Kdenit/*T^(-1)*/, double TemperatureLimitation, double OxygenLimitation)
{
   double MyNO3, MyKdenit, MyTempLim, MyOxyLim; 
   MyNO3 = std::max(0.0,NO3);
   MyKdenit = std::max(0.0,Kdenit);
   MyTempLim = std::max(0.0,TemperatureLimitation);
   MyOxyLim = std::max(0.0,OxygenLimitation);
   MyOxyLim = std::min(1.0,MyOxyLim);
   return MyKdenit * MyNO3 * MyTempLim * (1.0 - MyOxyLim); 
   //returns units of N-NO3/N-NH4 concentration decrease/increase in the water per unit of time
   //concentration and time units are defined by the user
}

double DenitrificationToN2(double DenitrificationToNH4, double alfa)
{
   double MyDenitToNH4, Myalfa;
   MyDenitToNH4 = std::max(0.0,DenitrificationToNH4);
   Myalfa = std::max(0.0,alfa);
   Myalfa = std::min(1.0,Myalfa);
   return MyDenitToNH4 * Myalfa; 
   //returns units of N-NH4/N2 concentration decrease/increase in the water per unit of time
   //concentration and time units are defined by the user
}

double Mineralization(double minR/*T^(-1)*/,double TemperatureLimitation, double OxygenLimitation, double Xorganic)
{
   double MyminR, MyTempLim, MyOxyLim, MyXorganic;
   MyTempLim = std::max(0.0,TemperatureLimitation);
   MyOxyLim = std::max(0.0,OxygenLimitation);
   MyOxyLim = std::min(1.0,MyOxyLim);
   MyminR = std::max(0.0,minR);
   MyXorganic = std::max(0.0,Xorganic);
   return MyminR * MyXorganic * MyTempLim * MyOxyLim; 
   //Xorganic stands for any organic substance being mineralized and it may be expressed in different currencies such as
   //carbon, nitrogen and phosphorus. 
   //returns units of Xorganic/X inorganic concentration decrease/increase per unit of time
   //if Xorganic is expressed in concentration per unit of mass of sediments (for example) so the results will also
   //be in similar units. Therefore, to convert it to the concentration of the inorganic substance released to
   //the water, the result must be multiplied by the SedimentWaterRatio in ML^(-3) 
   //concentration and time units are defined by the user
}

double Nitrification(double NH4, double Knit/*T^(-1)*/, double TemperatureLimitation, double OxygenLimitation)
{
   double MyNH4, MyKnit, MyTempLim, MyOxyLim;
   MyNH4 = std::max(0.0,NH4);
   MyKnit = std::max(0.0,Knit);
   MyTempLim = std::max(0.0,TemperatureLimitation);
   MyOxyLim = std::max(0.0,OxygenLimitation);
   MyOxyLim = std::min(1.0,MyOxyLim);
   return MyKnit * MyNH4 * MyTempLim * MyOxyLim; 
   //returns units of N-NH4/N-NO3 concentration decrease/increase in the water per unit of time
   //concentration and time units are defined by the user
}

double OrganicDissolution(double dissR,  double TemperatureLimitation, double Xorganic)
{
   double MydissR, MyTempLim, MyXorganic;
   MyTempLim = std::max(0.0,TemperatureLimitation);
   MydissR = std::max(0.0,dissR);
   MyXorganic = std::max(0.0,Xorganic);
   return MydissR * MyXorganic * MyTempLim;
   //Xorganic stands for any organic substance being dissolved and it may be expressed in different currencies such as
   //carbon, nitrogen and phosphorus.
   //returns units of Xorganic particulate/X inorganic dissolved concentration decrease/increase per unit of time
   //if Xorganic is expressed in concentration per unit of mass of sediments (for example) so the results will also
   //be in similar units. Therefore, to convert it to the concentration of the dissolved substance released to
   //the water, the result must be multiplied by the SedimentWaterRatio in ML^(-3)
   //concentration and time units are defined by the user

}

//This function calculates the flux of dissolved of P-PO4 adsorbed from the pore water to the sediments. It returns the amount of P lost by the water per unit of time
//This function may also be applied to adsorption of P-PO4 by suspended sediments, in which case PO4 and O2 concentrations are not those of the pore water but instead those of the water
//where the particles are suspended.
double PhosphorusAdsorption(double PO4InPoreWater,double Pads/*MM^(-1)*/,double Pmax, double OxygenInPoreWater, double OxygenThershold, double Ka1, double Ka2)
{
   double MyPmax, MyPO4, MyPads, TINNY = 0.0000000001, MyOxygen, MyOxyThr, MyKa1, MyKa2, Adsorption = 0.0;
   MyPmax = std::max(0.0,Pmax);
   MyPads = std::max(0.0,Pads);
   MyPads = std::min(Pads,Pmax); //Pads should not exceed Pmax. If it does should be limiteds for the calculations below and a warning should be issued
   MyPO4  = std::max(0.0,PO4InPoreWater);
   MyOxygen = std::max(0.0,OxygenInPoreWater);
   MyOxyThr = std::max(0.0,OxygenThershold); 
   MyKa1  = std::max(0.0,Ka1);
   MyKa2  = std::max(0.0,Ka2); 
   if (MyPmax > TINNY) 
   {
     if (MyOxygen > MyOxyThr) 
        Adsorption =  MyKa1 * (1.0 - MyPads / MyPmax) * MyPO4; //returns units of P-PO4 concentration decrease in the pore water per unit of time
     else                                                      //which may be converted to units of P-PO4 concentration by mass of sediments
        Adsorption =  MyKa2 * (1.0 - MyPads / MyPmax) * MyPO4; //by diving the result by the SedimentWaterRatio in ML^(-3) to compute the
                                                               // P-PO4 gains by the sediments
   }
   return Adsorption; 
}


//This function calculates the flux of desorbed P-PO4 from the sediments to the pore water. It returns the amount of P lost by the sediment per unit of time.
 double PhosphorusDesorption(double Pads/*MM^(-1)*/, double Kd, double Pmax)
{
   double MyPads, MyKd, MyPmax, Desorption = 0.0, TINNY = 0.0000000001;
   MyKd  = std::max(0.0,Kd);
   MyPmax = std::max(0.0,Pmax);
   MyPads = std::max(0.0,Pads);
   MyPads = std::min(Pads,Pmax); //Pads should not exceed Pmax. If it does should be limiteds for the calculations below and a warning should be issued
   if (MyPmax > TINNY)
      Desorption = MyKd * MyPads / MyPmax; //returns Desorption in P-PO4 concentration units per unit of sediment mass
                                           //which may be normalized to concentration per unit of water volume
                                           //multiplying the result of this function by the SedimentWaterRatio in ML^(-3) and
					   //alloing to compute the P-PO4 gains by the water 
   return Desorption;
}

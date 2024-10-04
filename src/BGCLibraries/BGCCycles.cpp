#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
#include <array>
using namespace std;


double DenitrificationToNH4(double NO3, double Kdenit/*T^(-1)*/, double TemperatureLimitation, double OxygenLimitation)
{
   double MyNO3, MyKdenit, MyTempLim, MyOxyLim; 
   MyNO3 = std::max(0.0,NO3);
   MyKdenit = std::max(0.0,Kdenit);
   MyTempLim = std::max(0.0,TemperatureLimitation);
   MyOxyLim = std::max(0.0,OxygenLimitation);
   MyOxyLim = std::min(1.0,MyOxyLim);
   return MyKdenit * MyNO3 * MyTempLim * (1.0 - MyOxyLim); //returns units of N-NH4 concentration in the water per unit of time
}

double DenitrificationToN2(double DenitrificationToNH4, double alfa)
{
   double MyDenitToNH4, Myalfa;
   MyDenitToNH4 = std::max(0.0,DenitrificationToNH4);
   Myalfa = std::max(0.0,alfa);
   Myalfa = std::min(1.0,Myalfa);
   return MyDenitToNH4 * Myalfa; //returns units of N-NH4 concentration in the water per unit of time
}

double Nitrification(double NH4, double Knit/*T^(-1)*/, double TemperatureLimitation, double OxygenLimitation)
{
   double MyNH4, MyKnit, MyTempLim, MyOxyLim;
   MyNH4 = std::max(0.0,NH4);
   MyKnit = std::max(0.0,Knit);
   MyTempLim = std::max(0.0,TemperatureLimitation);
   MyOxyLim = std::max(0.0,OxygenLimitation);
   MyOxyLim = std::min(1.0,MyOxyLim);
   return MyKnit * MyNH4 * MyTempLim * MyOxyLim; //returns units of N-NO3 concentration in the water per unit of time
}

//This function calculate the flux of dissolved of P-PO4 adsorbed from the pore water to the sediments. It returns the amount of P lost by the water per unit of time
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
        Adsorption =  MyKa1 * (1.0 - MyPads / MyPmax) * MyPO4; //returns units of P-PO4InPoreWater concentration in the pore water per unit of time
     else                                                      //which may be converted to units of P-PO4 concentration by mass of sediments
        Adsorption =  MyKa2 * (1.0 - MyPads / MyPmax) * MyPO4; //by diving the result by the SedimentWaterRatio in ML^(-3)
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
                                           //which may be normalized to concentration per unit of pore water volume
                                           //multiplying the result of this function by the SedimentWaterRatio in ML^(-3) 
   return Desorption;
}

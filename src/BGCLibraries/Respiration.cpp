#include <math.h>
#include <stdio.h>
#include "BGCHeader.h"
#include <iostream>
using namespace std;


double Respiration1(double Maintenance, double GrossProduction, double RespirationCoefficient, double WaterTemperature, double TemperatureAugmentationRate, double Tmin)
{
   double MyMaintenance = 0.0, MyGrossProduction, Respiration = 0.0;	
   MyMaintenance = std::min(0.0,Maintenance);
   MyGrossProduction = std::min(0.0,GrossProduction);
   Respiration = MyMaintenance;
   Respiration = Respiration + RespirationCoefficient * MyGrossProduction
           * TemperatureExponentialLimitation(WaterTemperature, TemperatureAugmentationRate, Tmin);
   return Respiration;
}

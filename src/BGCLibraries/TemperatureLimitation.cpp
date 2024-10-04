#include <math.h>
#include <stdio.h>
#include "BGCHeader.h"

double TemperatureExponentialLimitation(double Temperature, double TemperatureAugmentationRate, double Tmin)
{
   return exp(TemperatureAugmentationRate * (Temperature - Tmin));
}





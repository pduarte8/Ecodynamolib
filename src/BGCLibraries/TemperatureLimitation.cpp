#include <math.h>
#include <stdio.h>
#include "BGCHeader.h"

double TemperatureExponentialLimitation(double WaterTemperature, double TemperatureAugmentationRate, double Tmin)
{
   return exp(TemperatureAugmentationRate * (WaterTemperature - Tmin));
}


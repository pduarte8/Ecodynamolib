#ifndef BGCHEADER_H
#define BGCHEADER_H
#include <array>

using namespace std;


extern "C" {
   double DenitrificationToNH4(double NO3, double Kdenit, double TemperatureLimitation, double OxygenLimitation);
   double DenitrificationToN2(double DenitrificationToNH4, double alfa);
   double EilersAndPeeters1(double PARtop, double KValue, double Depth, double a, double b, double c, double Pmax);
   double EilersAndPeeters2(double PAR, double a, double b, double c, double Pmax);
   double EilersAndPeetersSlope(double C);
   double Hollings(double K, double Prey, int HollingsType);
   double InternalNutrientLimitation(double CellQuota, double MinCellQuota, double HalfSaturation);
   double Ivlev(double Lambda, double Prey);
   double LightLimNitr(double KI, double I0, double Light);
   double MichaelisMentenLimitation(double N, double KN);
   double Mineralization(double minR,double TemperatureLimitation, double OxygenLimitation, double Xorganic);
   double NitrateAndAmmoniumLimitation(double NH4, double KNH4, double NO3, double KNO3, double NO2);
   double Nitrification(double NH4, double Knit, double TemperatureLimitation, double OxygenLimitation);
   double OrganicDissolution(double dissR,  double TemperatureLimitation, double Xorganic);
   double PhosphorusAdsorption(double PO4InPoreWater,double Pads,double Pmax, double OxygenInPoreWater, double OxygenThershold, double Ka1, double Ka2);
   double PhosphorusDesorption(double Pads, double Kd, double Pmax);
   double Platt1(double PARtop, double KValue, double Depth, double Pmax, double beta, double slope, int EulerSteps);
   double Platt2(double PAR, double Pmax, double beta, double slope);
   double Respiration1(double Maintenance, double GrossProduction, double RespirationCoeff, double WaterTemperature, double TemperatureAugmentationRate, double Tmin); 
   double Steele1(double PARtop, double KValue, double Depth, double PARopt);
   double Steele2(double PAR, double PARopt);
   double SteeleSlope(double Pmax, double Iopt);
   double TemperatureExponentialLimitation(double WaterTemperature, double TemperatureAugmentationRate, double Tmin);
   double VertLightAvg(double LightAtTop, double KValue, double Depth);
}

#endif // YOUR_HEADER_H

#ifndef BGCHEADER_H
#define BGCHEADER_H

using namespace std;


extern "C" {
   double EilersAndPeeters1(double PARtop, double KValue, double Depth, double a, double b, double c);
   double EilersAndPeeters2(double PAR, double a, double b, double c);
   double Hollings(double K, double Prey, int HollingsType);
   double InternalNutrientLimitation(double CellQuota, double MinCellQuota, double HalfSaturation);
   double Ivlev(double Lambda, double Prey);
   double MichaelisMentenLimitation(double N, double KN);
   double NitrateAndAmmoniumLimitation(double NH4, double KNH4, double NO3, double KNO3, double NO2);
   double Platt1(double PARtop, double KValue, double Depth, double Pmax, double beta, double slope, int EulerSteps);
   double Platt2(double PAR, double Pmax, double beta, double slope);
   double Steele1(double PARtop, double KValue, double Depth, double PARopt);
   double Steele2(double PAR, double PARopt);
   double TemperatureExponentialLimitation(double WaterTemperature, double TemperatureAugmentationRate, double Tmin);
}

#endif // YOUR_HEADER_H

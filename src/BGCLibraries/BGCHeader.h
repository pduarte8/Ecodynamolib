

using namespace std;


extern "C" {
   double EilersAndPeeters(double PARtop, double KValue, double Depth, double a, double b, double c);
   double EilersAndPeetersX(double PAR, double a, double b, double c);
   double Hollings(double K, double Prey, int HollingsType);
   double InternalNutrientLimitation(double CellQuota, double MinCellQuota, double HalfSaturation);
   double MichaelisMentenLimitation(double N, double KN);
   double NitrateAndAmmoniumLimitation(double NH4, double KNH4, double NO3, double KNO3, double NO2);
   double Platt(double PARtop, double KValue, double Depth, double Pmax, double beta, double slope, int EulerSteps);
   double PlattX(double PAR, double Pmax, double beta, double slope);
   double Steele(double PARtop, double KValue, double Depth, double PARopt);
   double SteeleX(double PAR, double PARopt);
   double TemperatureExponentialLimitation(double WaterTemperature, double TemperatureAugmentationRate, double Tmin);
}

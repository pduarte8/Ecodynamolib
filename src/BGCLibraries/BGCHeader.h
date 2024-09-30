

using namespace std;


extern "C" {
   double EilersAndPeeters__(double* PARtop, double* KValue, double* Depth, double* a, double *b, double* c);
   double EilersAndPeetersX__(double* PAR, double* a, double *b, double* c);
   double Hollings__(double* K, double* Prey, int* HollingsType);
   double InternalNutrientLimitation__(double* CellQuota, double* MinCellQuota, double* HalfSaturation);
   double MichaelisMentenLimitation__(double* N, double* KN);
   double NitrateAndAmmoniumLimitation__(double* NH4, double* KNH4, double* NO3, double* KNO3, double* NO2);
   double Platt__(double* PARtop, double* KValue, double* Depth, double* Pmax, double* beta, double* slope, int* EulerSteps);
   double PlattX__(double* PAR, double* Pmax, double* beta, double* slope);
   double Steele__(double* PARtop, double* KValue, double* Depth, double* PARopt);
   double SteeleX__(double* PAR, double* PARopt);
   double TemperatureExponentialLimitation__(double* WaterTemperature, double* TemperatureAugmentationRate, double* Tmin);
}

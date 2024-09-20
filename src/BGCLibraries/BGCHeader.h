

using namespace std;


extern "C" {
   double EilersAndPeeters__(double* PARtop, double* PARbottom, double* KValue, double* Depth, double* a, double *b, double* c);
   double InternalNutrientLimitation__(double CellQuota[], double MinCellQuota[], double HalfSaturation[]);
   double MichaelisMentenLimitation__(double N, double KN);
   double NitrateAndAmmoniumLimitation__(double NH4, double KNH4, double NO3, double KNO3, double NO2);
   double Platt__(double* PARtop, double* KValue, double* Depth, double* Pmax, double* beta, double* slope, int* EulerSteps);
   double Steele__(double* PARtop, double* PARbottom, double* KValue, double* Depth, double* PARopt);
   double TemperatureExponentialLimitation__(double* WaterTemperature, double* TemperatureAugmentationRate, double* Tmin);
}

#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
//#include <cstdlib>
//#include <cstdio>
//#include <cmath>
//#include <exception>
using namespace std;

double InternalNutrientLimitation(double CellQuota, double MinCellQuota, double HalfSaturation)
{
   double LiebigLimitation = 1.0;
   double Limitation;
   Limitation = std::max((CellQuota - MinCellQuota)/ (CellQuota + HalfSaturation - MinCellQuota),0.0);
   LiebigLimitation = std::min(LiebigLimitation,Limitation);
   return LiebigLimitation;
}


double NitrateAndAmmoniumLimitation(double NH4, double KNH4, double NO3, double KNO3, double NO2) 
{ 
   double cff1, cff2, L_NH4, L_NO3;
   cff1= NH4 / KNH4;
   cff2= (NO3 + NO2) / KNO3;
   L_NH4=std::max(cff1/(1.0+cff1),0.0);
   L_NO3=std::max(cff2/(1.0+cff2),0.0);
   return std::min(L_NO3+L_NH4,1.0);
}

double MichaelisMentenLimitation(double N, double KN)
{
   return N/(N+KN);
}



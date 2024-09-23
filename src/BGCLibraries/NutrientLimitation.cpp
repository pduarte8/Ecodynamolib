#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
//#include <cstdlib>
//#include <cstdio>
//#include <cmath>
//#include <exception>
using namespace std;

double InternalNutrientLimitation__(double CellQuota[], double MinCellQuota[], double HalfSaturation[])
{
   int n;
   double LiebigLimitation = 1.0;
   n = sizeof(CellQuota);
   double Limitation[n];
   for (int i = 0; i < n; i++)
   {
     Limitation[i] = std::max((CellQuota[i] - MinCellQuota[i])/ (CellQuota[i] + HalfSaturation[i] - MinCellQuota[i]),0.0);
     LiebigLimitation = std::min(LiebigLimitation,Limitation[i]);
   }
   return std::min(LiebigLimitation,1.0);
}


double NitrateAndAmmoniumLimitation__(double NH4, double KNH4, double NO3, double KNO3, double NO2) 
{ 
   double cff1, cff2, L_NH4, L_NO3;
   cff1= NH4 / KNH4;
   cff2= (NO3 + NO2) / KNO3;
   L_NH4=std::max(cff1/(1.0+cff1),0.0);
   L_NO3=std::max(cff2/(1.0+cff2),0.0);
   return std::min(L_NO3+L_NH4,1.0);
}

double MichaelisMentenLimitation__(double N, double KN)
{
   return N/(N+KN);
}


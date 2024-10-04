#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
//#include <cstdlib>
//#include <cstdio>
//#include <cmath>
//#include <exception>
using namespace std;

//Based on: Jørgensen, S.E., Bendoricchio, G., 2001. Fundamentals of ecological modelling. Developments
//in Ecological Modelling 21. Elsevier, Amsterdam.
double InternalNutrientLimitation(double CellQuota, double MinCellQuota, double HalfSaturation)
{
   double LiebigLimitation = 1.0, MyCellQuota, MyMinCellQuota, MyHalfSaturation;
   double Limitation;
   MyCellQuota = std::max(0.0,CellQuota);
   MyMinCellQuota = std::max(0.0,MinCellQuota);
   MyHalfSaturation = std::max(0.0,HalfSaturation);
   Limitation = std::max((MyCellQuota - MyMinCellQuota)/ (MyCellQuota + MyHalfSaturation - MyMinCellQuota),0.0);
   LiebigLimitation = std::min(LiebigLimitation,Limitation);
   return LiebigLimitation;
}

//Based a simples hyperbolic function and assuming that nitrogen limitation is alleviated equaly by NO3+NO2
//and ammonium. The final result is limitied to a maximum of 1.0.
double NitrateAndAmmoniumLimitation(double NH4, double KNH4, double NO3, double KNO3, double NO2) 
{ 
   double cff1, cff2, L_NH4, L_NO3, MyNH4, MyKNH4, MyNO3, MyKNO3, MyNO2, TINNY = 0.0000000001;
   MyNH4 = std::max(0.0,NH4);
   MyKNH4 = std::max(0.0,KNH4);
   MyNO3 = std::max(0.0,NO3);
   MyKNO3 = std::max(0.0,KNO3);
   MyNO2 = std::max(0.0,NO2);
   if (MyKNH4 > TINNY)
   {	    
      cff1= MyNH4 / MyKNH4;
      L_NH4=std::max(cff1/(1.0+cff1),0.0);
   }   
   else
      L_NH4 = 1.0; 
   if (MyKNO3) 
   {	   
      cff2= (MyNO3 + MyNO2) / MyKNO3;
      L_NO3=std::max(cff2/(1.0+cff2),0.0);
   }
   else
      L_NO3 = 1.0;	   
   return std::min(L_NO3+L_NH4,1.0);
}

double MichaelisMentenLimitation(double N, double KN)
{
   double MyN, MyKN, TINNY = 0.0000000001;
   MyN = std::max(0.0,N);
   MyKN = std::max(0.0,KN);
   if (MyKN > TINNY)   
      return MyN / (MyN + MyKN);
   else
      return 1.0;
}



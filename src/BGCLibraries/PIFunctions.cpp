#include <math.h>
#include <stdio.h>
#include "BGCHeader.h" 
#include <iostream>

double Platt__(double* PARtop, double* KValue, double* Depth, double* Pmax, double* beta, double* slope, int* EulerSteps)
{
   double DeltaZ, Soma = 0.0, LightLimitation = 0.0, TINNY = 0.0000000001;
/*   cout << "PARtop= " << *PARtop << endl;
   cout << "KValue= " << *KValue << endl;
   cout << "Depth= "<<  *Depth << endl; 
   cout << "Pmax= " << *Pmax << endl;
   cout << "beta= " << *beta << endl;
   cout << "slope= " << *slope << endl;
   cout << "EulerSteps= " << *EulerSteps << endl;*/
   if ((*Depth > TINNY) && (*PARtop > TINNY) && (*EulerSteps >= 1.0))
   {
         DeltaZ = *Depth / *EulerSteps;
         for (int Step = 1; Step <= *EulerSteps; Step++)    //Euler integration as a function of depth
         {
            Soma = Soma + (1 - exp(-*slope * *PARtop / *Pmax)) * exp(-*beta * *PARtop/ *Pmax) * DeltaZ;
            *PARtop = *PARtop * exp(-*KValue * DeltaZ);
         }
         LightLimitation = Soma / *Depth;
  }
  return LightLimitation;
}
//******************************************************************************************************************************************************
////Steele P-I function
////Steele, J. H.: Environmental control of photosynthesis in he sea. Limnlogy andOceanography, 7, 137-150, 1962
////
//
double Steele__(double* PARtop, double* PARbottom, double* KValue, double* Depth, double* PARopt)
{
   double LightLimitation = 0.0, TINNY = 0.0000000001;
   if ((*Depth > TINNY) && (*PARtop > TINNY))
      LightLimitation = 2.718282 / (*KValue * *Depth) * (exp(-*PARbottom / *PARopt)-exp(-*PARtop / *PARopt));
   return LightLimitation;
}


//******************************************************************************************************************************************************
////Eilers and Peeters P-I function
////Eilers, P. H. C., Peeters, J. C. H.: A model for the relationship between light intensity and the rate of photosynthesis in phytoplankton,
////Ecological Modelling, 42, 199-215, 1988
////
double EilersAndPeeters__(double* PARtop, double* PARbottom, double* KValue, double* Depth, double* a, double* b, double* c)
{
   double D, B1, B2, Productivity = 0.0, TINNY = 0.0000000001;
   if ((*Depth > TINNY) && (*PARtop > TINNY))
   {
      if (*a != 0.0)
      {
         D = *b * *b - 4 * *a * *c;
         B1 = 2.0 * *a * *PARtop + *b;
         B2 = 2.0 * *a * *PARbottom + *b;
         if (D < 0.0)
            Productivity = 2.0 / (*KValue*sqrt(-D)) *(atan(B1/sqrt(-D))- atan(B2/sqrt(-D))) / *Depth;
         else if (D == 0.0)
            Productivity = 2.0 / *KValue * (1.0 / B2 - 1.0 / B1) / *Depth;
         else if (D > 0.0)
            Productivity = 1.0 / (*KValue*sqrt(D)) * log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
                           ((B1 + sqrt(D)) * (B2 - sqrt(D)))) / *Depth;
       }
       else
          Productivity = 1.0 / (*KValue * *b) * log (fabs(*b * *PARtop + *c)/
                         fabs(*b * *PARbottom + *c)) / *Depth;
   }
   return Productivity;
}
//******************************************************************************************************************************************************
//

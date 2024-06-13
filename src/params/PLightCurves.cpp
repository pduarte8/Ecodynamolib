#include <math.h>
#include <stdio.h>
#include "params.h"


// TPLightCurve's methods


// Constructor******************************************************************************************************************************************
TPLightCurves::TPLightCurves()
{
   TINNY = 0.0000000001;
   Steps = 30.0;
}
// Destructor*******************************************************************************************************************************************
TPLightCurves::~TPLightCurves()
{
}
//******************************************************************************************************************************************************
//The arguments to these functions should be in compatible light, time and length units. The return argument (LightLimitation) is productibity per unit 
//of mass per unit of time (1/t)
//
//Examples of units:
//
//Platt P-I function
//Platt, T., Gallegos, C. L., Harrison, W. G.: Photoinhibition of photosynthesis in natural assemblages of marine phytoplankton, 
//Journal of Marine Research, 38 (4), 687-701, https://elischolar.library.yale.edu/journal_of_marine_research/1525, 1980
//
double TPLightCurves::Platt(double PARtop, double KValue, double Depth, double Pmax, double beta, double slope)
{
   double DeltaZ, Soma, PAR;
   Productivity = 0.0;
   if ((Depth > TINNY) && (PARtop > TINNY) && (Steps >= 1.0))
   {  
      DeltaZ = Depth / Steps;
      Soma = 0.0;
      PAR = PARtop; 
      for (int Step = 1; Step <= Steps; Step++) //Euler integration as a function of depth
      {      
         Soma = Soma + (1 - exp(-slope * PAR / Pmax)) * exp(-beta * PAR / Pmax) * DeltaZ;
         PAR = PAR * exp(-KValue * DeltaZ);
      }
      Productivity  = Pmax * Soma / Depth; //Vertical averaging of light limitation
   }
   return Productivity;
}	
//******************************************************************************************************************************************************
//Steele P-I function
//Steele, J. H.: Environmental control of photosynthesis in he sea. Limnlogy andOceanography, 7, 137-150, 1962
//
double TPLightCurves::Steele(double PARtop, double PARbottom, double KValue, double Depth, double Pmax, double PARopt)
{	
   Productivity = 0.0;	
   if ((Depth > TINNY) && (PARtop > TINNY))
      Productivity = Pmax * 2.718282 / (KValue * Depth) * (exp(-PARbottom / PARopt)-exp(-PARtop / PARopt));    
   return Productivity;
}	
//******************************************************************************************************************************************************
//Eilers and Peeters P-I function
//Eilers, P. H. C., Peeters, J. C. H.: A model for the relationship between light intensity and the rate of photosynthesis in phytoplankton,
//Ecological Modelling, 42, 199-215, 1988
//
double TPLightCurves::EilersAndPeeters(double PARtop, double PARbottom, double KValue, double Depth, double a, double b, double c)
{
   double D, B1, B2;
   Productivity = 0.0;
   if ((Depth > TINNY) && (PARtop > TINNY))
   {
      if (a != 0.0)
      {
         D = b * b - 4 * a * c;
         B1 = 2.0 * a * PARtop + b;
         B2 = 2.0 * a * PARbottom + b;
         if (D < 0.0)
            Productivity = 2.0 / (KValue*sqrt(-D)) *(atan(B1/sqrt(-D))- atan(B2/sqrt(-D))) / Depth;
         else if (D == 0.0)
            Productivity = 2.0 / KValue * (1.0 / B2 - 1.0 / B1) / Depth;
         else if (D > 0.0)
            Productivity = 1.0 / (KValue*sqrt(D)) * log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
                           ((B1 + sqrt(D)) * (B2 - sqrt(D)))) / Depth;
       }
       else
          Productivity = 1.0 / (KValue * b) * log (fabs(b * PARtop + c)/
                         fabs(b * PARbottom + c)) / Depth;
   }
   return Productivity;
}	
//******************************************************************************************************************************************************

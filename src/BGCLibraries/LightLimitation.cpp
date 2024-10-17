#include <math.h>
#include <stdio.h>
#include "BGCHeader.h" 
#include <iostream>
using namespace std;


//Platt, T., Gallegos, C. L., and Harrison, W. G.: Photoinhibition of Photosynthesis in Natural Assemblages of Marine-Phytoplankton, J Mar Res, 38, 687-701, 1980.
//Platt1 returns vertically averaged light inhibition. This is achieved with a numerical integration of the Platt function, which is shown in Platt2, using the Euler method.
//Platt2, returns light inhibition at a specified PAR level.

double Platt1(double PARtop, double KValue, double Depth, double Pmax, double beta, double slope, int EulerSteps)
{
   double DeltaZ, Soma = 0.0, LightLimitation = 0.0, TINNY = 0.00000000001, PAR;
   double MyKValue, MyDepth, MyPmax, Mybeta, Myslope;
   int MySteps;
   //cout << "PARtop= " << PARtop << endl;
   PAR = std::max(0.0,PARtop);
   MyKValue = std::max(0.0,KValue);
   MyDepth = std::max(0.0,Depth);
   MyPmax = std::max(0.0,Pmax);
   Mybeta = std::max(0.0,beta);
   Myslope = std::max(0.0,slope);
   MySteps = std::max(1,EulerSteps);
   if ((MyDepth > TINNY) && (PAR > TINNY) && (MyPmax > TINNY))
   {
         DeltaZ = MyDepth / MySteps;
         for (int Step = 1; Step <= MySteps; Step++)    //Euler integration as a function of depth
         {
            Soma = Soma + (1 - exp(-Myslope * PAR / MyPmax)) * exp(-Mybeta * PAR/ MyPmax) * DeltaZ;
            PAR = PAR * exp(-MyKValue * DeltaZ);
         }
         LightLimitation = Soma / MyDepth;
  }
  return LightLimitation;
}

double Platt2(double PAR, double Pmax, double beta, double slope)
{
   double LightLimitation = 0.0, TINNY = 0.00000000001, MyPAR;
   double MyPmax, Mybeta, Myslope;
   MyPAR = std::max(0.0,PAR);
   MyPmax = std::max(0.0,Pmax);
   Mybeta = std::max(0.0,beta);
   Myslope = std::max(0.0,slope);
   if ((MyPAR > TINNY) && (MyPmax > TINNY)) 
      LightLimitation = (1.0-exp(-Myslope * MyPAR / MyPmax)) * exp(-Mybeta * MyPAR / MyPmax);
   return LightLimitation;
}

//******************************************************************************************************************************************************
//Steele P-I function
//Steele, J. H.: Environmental control of photosynthesis in the sea. Limnology and Oceanography, 7, 137-150, 1962
//Steele1 returns vertically averaged light inhibition. This is achieved with the analytical integration of the Steele function, which is shown in Steele2.
//Steele2 returns light inhibition at a specified PAR level.
//
//
double Steele1(double PARtop, double KValue, double Depth, double PARopt)
{
   double LightLimitation = 0.0, TINNY = 0.00000000001;
   double MyPARtop, MyPARbottom, MyKValue, MyDepth, MyPARopt;
   MyPARtop = std::max(0.0,PARtop);
   MyKValue = std::max(0.0,KValue);
   MyDepth = std::max(0.0,Depth);
   MyPARopt = std::max(0.0,PARopt);
   MyPARbottom = std::max(0.0, MyPARtop*exp(-MyKValue * MyDepth));
   if ((MyKValue * MyDepth > TINNY) && (MyPARtop > TINNY))
      LightLimitation = 2.718282 / (MyKValue * MyDepth) * (exp(-MyPARbottom / MyPARopt)-exp(-MyPARtop / MyPARopt));
   return LightLimitation;
}

double Steele2(double PAR, double PARopt)
{
   double LightLimitation = 0.0, TINNY = 0.00000000001, MyPAR, MyPARopt;
   MyPAR = std::max(0.0,PAR);
   MyPARopt = std::max(0.0,PARopt);
   if ((MyPAR > TINNY) && (MyPARopt > TINNY))
      LightLimitation = MyPAR / MyPARopt * exp(1.0 - MyPAR / MyPARopt);
   return LightLimitation;
}

double SteeleSlope(double Pmax, double Iopt)
{
   double Slope = 0.0,  TINNY = 0.00000000001, MyPmax, MyIopt;
   MyPmax = std::max(0.0,Pmax);
   MyIopt = std::max(0.0,Iopt);   
   if (MyIopt > TINNY) Slope= MyPmax * exp(1.0) / MyIopt;
   return Slope;
}
//Eilers and Peeters P-I function
//Eilers, P. H. C., Peeters, J. C. H.: A model for the relationship between light intensity and the rate of photosynthesis in phytoplankton,
//Ecological Modelling, 42, 199-215, 1988
//EilersAndPeeters1 returns vertically averaged light inhibition. This is achieved with the analytical integration of the EilersAndPeeters function, which is shown in EilersAndPeeters2.
//EilersAndPeeters2 returns light inhibition at a specified PAR level.
//
double EilersAndPeeters1(double PARtop, double KValue, double Depth, double a, double b, double c, double Pmax)
{
   double D, B1, B2, LightLimitation = 0.0, TINNY = 0.00000000001, P;
   double MyPARtop, MyKValue, MyDepth, MyPARbottom, Mya, Myc, MyPmax;
   //cout << "PARtop= " << PARtop << endl;
   MyPARtop = std::max(0.0,PARtop);
   MyKValue = std::max(0.0,KValue);
   MyDepth = std::max(0.0,Depth);
   Mya = std::max(0.0,a);
   Myc = std::max(0.0,c);
   MyPmax = std::max(0.0,Pmax);
   MyPARbottom = std::max(0.0, MyPARtop*exp(-MyKValue * MyDepth));
   if (MyPARtop - MyPARbottom > TINNY)
   {	   
      if ((MyDepth > TINNY) && (MyPARtop > TINNY) && (MyPmax > TINNY))
      {
         if (Mya != 0.0)
         {
            D = b * b - 4 * Mya * Myc;
            B1 = 2.0 * Mya * MyPARtop + b;
            B2 = 2.0 * Mya * MyPARbottom + b;
            if (D < 0.0)
               P = 2.0 / (MyKValue*sqrt(-D)) *(atan(B1/sqrt(-D))- atan(B2/sqrt(-D))) / MyDepth;
            else if (D == 0.0)
               P = 2.0 / MyKValue * (1.0 / B2 - 1.0 / B1) / MyDepth;
            else if (D > 0.0)
               P = 1.0 / (MyKValue*sqrt(D)) * log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
                           ((B1 + sqrt(D)) * (B2 - sqrt(D)))) / MyDepth;
         }
         else	 
            P = 1.0 / (MyKValue * b) * log (fabs(b * MyPARtop + Myc)/
                         fabs(b * MyPARbottom + Myc)) / MyDepth;
         LightLimitation = P/MyPmax;
      }
   }
   else{
      EilersAndPeeters2(MyPARtop, Mya, b, Myc, MyPmax);}//If top and bottom light do not differ signmificantly, use the standard function, without vertical averaging
   return LightLimitation;
}

double EilersAndPeeters2(double PAR, double a, double b, double c, double Pmax)
{
   double LightLimitation = 0.0, TINNY = 0.00000000001, MyPAR, x, P;
   double Mya, Myc, MyPmax;
   MyPAR = std::max(0.0,PAR);
   Mya = std::max(0.0,a);
   Myc = std::max(0.0,c);
   MyPmax = std::max(0.0,Pmax);
   x = Mya * MyPAR * MyPAR + b * MyPAR + Myc;
   if ((x > TINNY) && (MyPmax > TINNY))
   {	   
      P = MyPAR / x;
      LightLimitation = P / MyPmax;
   }   
   return LightLimitation; 
}

double EilersAndPeetersSlope(double C)
{
   double Slope = 0.0, TINNY = 0.00000000001, MyC;
   MyC = std::max(0.0,C);   
   if (MyC > TINNY) Slope = 1.0 / MyC;
   return Slope;
}


//From: Fennel et al. (2006) - Global Biogeochem. Cycles, 20,GB3007, doi:10.1029/2005GB002456
double LightLimNitr(double KI, double I0, double Light)
{
   double MyKI, MyI0, MyLight;
   MyLight = std::max(0.0,Light);
   MyKI = std::max(0.0,KI);//Light intensity at which the
                           //inhibition of nitrification is half-saturated
   MyI0 = std::max(0.0,I0);//W m-2 Light intensity threshold for
                           //light-inhibition of nitrification
   return (1.0 - std::max(0.0,(MyLight - MyI0) / (MyKI + MyLight - MyI0)));
}	

double VertLightAvg(double LightAtTop, double KValue, double Depth)
{
   double TINNY = 0.00000000001, MyLight, MyKValue, MyDepth;
   MyLight = std::max(0.0,LightAtTop);
   MyKValue = std::max(0.0,KValue);
   MyDepth = std::max(0.0,Depth);
   if (MyKValue * MyDepth > TINNY)
       return(MyLight * (1.0 - exp(-MyKValue * MyDepth)) / (MyKValue * MyDepth));
   else
       return MyLight;   
}	
//******************************************************************************************************************************************************
//

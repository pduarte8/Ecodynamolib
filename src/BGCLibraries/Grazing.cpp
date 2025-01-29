#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
using namespace std;

//Holling, C. S. 1959a. The components of predation as revealed by a study of small mammal predation
//of the European pine sawfly. Canadian Entomology 91:293–320
//Holling, C. S. 1959b. Some characteristics of simple types of predation and parasitism. Canadian Entomology 91:385–398.
//Holling, C. S. 1965. The functional response of predators to prey density and its role in mimicry and
//population regulation. Memoirs of the Entomological Society of Canada 45:5–60.
//
//See also:
//Denny, M., 2014. Buzz Holling and the Functional Response. Bulletin of the Ecological Society of America, 95(3)
//Rohr, T., Richardson, A. J., Lenton, A., and Shadwick, E.: Recommendations for the formulation of grazing in marine biogeochemical and ecosystem models, Prog Oceanogr, 208, Artn 102878
//10.1016/J.Pocean.2022.102878, 2022.
//
//
//This functions may be used for Hollings types 0, 1 and 2, depending on the values of its last argument.
//
double Hollings(double K, double Prey, int HollingsType)
{
   double GrazingLimitation = 0.0,  TINNY = 0.0000000001;
   double MyK, MyPrey, MyHollingsType;
   //cout << "K= " << K << endl;
   //cout << "Prey= " << Prey << endl;
   //cout << "HollingsType= " << HollingsType << endl;
   MyK = std::max(0.0,K);
   MyPrey = std::max(0.0,Prey);
   MyHollingsType = std::max(0,HollingsType);
   if (MyPrey > TINNY)
      GrazingLimitation = std::max(0.0,pow(MyPrey,MyHollingsType)/(pow(MyK,MyHollingsType)+pow(MyPrey,MyHollingsType)));
   return GrazingLimitation;
}


//Ivlev V. Experimental Ecology of the Feeding of Fishes
//Yale University Press, New Haven (1961)
//
//See also:
//Rohr, T., Richardson, A. J., Lenton, A., and Shadwick, E.: Recommendations for the formulation of grazing in marine biogeochemical and ecosystem models, Prog Oceanogr, 208, Artn 102878
//10.1016/J.Pocean.2022.102878, 2022.
//
double IvlevFunction(double Lambda, double Prey)
{
   double GrazingLimitation, MyLambda, MyPrey;
   MyLambda = std::max(0.0,Lambda);
   MyPrey = std::max(0.0,Prey);
   GrazingLimitation = 1.0 - exp(-MyLambda * MyPrey);
   return GrazingLimitation;
}

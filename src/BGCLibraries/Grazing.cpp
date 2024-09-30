#include <math.h>
#include <stdio.h>
#include <iostream>
#include "BGCHeader.h"
using namespace std;

double Hollings__(double* K, double* Prey, int* HollingsType)
{
   double Grazing = 0.0,  TINNY = 0.0000000001;
   double MyK, MyPrey, MyHollingsType;
   MyK = std::max(0.0,*K);
   MyPrey = std::max(0.0,*Prey);
   MyHollingsType = std::max(0,*HollingsType);
   if (MyPrey > TINNY)
      Grazing = std::max(0.0,pow(MyPrey,MyHollingsType)/(pow(MyK,MyHollingsType)+pow(MyPrey,MyHollingsType)));
   return Grazing;
}

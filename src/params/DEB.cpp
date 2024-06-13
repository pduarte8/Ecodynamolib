#include <math.h>
#include <stdio.h>
#include "params.h"


// TDEB's methods


// Constructor******************************************************************************************************************************************
TDEB::TDEB()
{
   TINNY = 0.0000000001;
}
// Destructor*******************************************************************************************************************************************
TDEB::~TDEB()
{
}
//******************************************************************************************************************************************************
double TDEB::Feed(double JXm, double F, double StructuralVolume)  //JX
{
   return JXm * F * pow(StructuralVolume, 0.666666667); //J d-1
}
//******************************************************************************************************************************************************
double TDEB::Absorption(double PAm, double F, double StructuralVolume)  //PA
{
   return PAm * F * pow(StructuralVolume, 0.666666667); //J d-1
}
//******************************************************************************************************************************************************
double TDEB::StructuralVolumeChange(double PAm, double K, double Em, double Eg, double PT, double Respiration, double Reserves, double StructuralVolume)
{
   double MyReserveDensity;
   double denominator;
   if (StructuralVolume > TINNY)
      MyReserveDensity = Reserves / StructuralVolume;
   else
      MyReserveDensity = 0.0;
   denominator = Eg + K * MyReserveDensity;
   if ((denominator > TINNY) && (Em > TINNY))
   {
      return   (
                  K * PAm * MyReserveDensity * pow(StructuralVolume,0.666666667)/(Em * denominator)-
                  Respiration -
                  PT * pow(StructuralVolume,0.666666667) / denominator
               ) ;  //cm3 day-1
   }
   else
   {
      MessageBox(0, "TDEB::StructuralVolumeChange", "Attempting to divide by zero", MB_OK);
      return 0.0;
   }
}
//******************************************************************************************************************************************************
double TDEB::GrowthAndMetabolicCosts(double PAm, double PM, double Em, double Eg, double PT, double Reserves, double StructuralVolume)
{
   double MyReserveDensity;
   double denominator;
   if (StructuralVolume > TINNY)
      MyReserveDensity = Reserves / StructuralVolume;
   else
      MyReserveDensity = 0.0;
   denominator = Eg + MyReserveDensity;
   if (denominator > TINNY)
   {
      return MyReserveDensity / denominator *
             (
               (PAm * Eg / Em) * pow(StructuralVolume,0.666666667) + PM * StructuralVolume + PT * pow(StructuralVolume,0.666666667)
             ); //J d-1
   }
   else
   {
      MessageBox(0, "TDEB::GrowthAndMetabolicCosts", "Attempting to divide by zero", MB_OK);
      return 0.0;
   }
}
//******************************************************************************************************************************************************
double TDEB::ReserveConsumption(double GrowthAndMetabolism, double MaturityCosts, double ReprodCosts) //Pc
{
   return GrowthAndMetabolism + ReprodCosts + MaturityCosts; //J d-1
}
//******************************************************************************************************************************************************
double TDEB::Respire(double PM, double K, double Eg, double Reserves, double StructuralVolume)
{
   double MyReserveDensity;
   double denominator;
   if (StructuralVolume > TINNY)
      MyReserveDensity = Reserves / StructuralVolume;
   else
      MyReserveDensity = 0.0;
   denominator = Eg + K * MyReserveDensity;
   if (denominator > TINNY)
   {
      return PM * StructuralVolume /denominator;//cm3 d-1
   }
   else
   {
      MessageBox(0, "TDEB::GrowthAndMetabolicCosts", "Attempting to divide by zero", MB_OK);
      return 0.0;
   }
}
//******************************************************************************************************************************************************
double TDEB::BodyLength(double StructuralVolume, double AShapeCoefficient)
{
   return pow(StructuralVolume,0.333333333)/AShapeCoefficient;
}
//******************************************************************************************************************************************************
double TDEB::TemperatureLimitation(double ATA, double AT1, double AWaterTemperature) //AWaterTemperature should be in Celsius
{
   double MyTA, MyT1,MyWaterTemperature;
   //Debugger(ATA); Debugger(AT1); Debugger(AWaterTemperature);
   if ((AT1 > TINNY) && (fabs(AWaterTemperature + ABSOLUTEZERO) > TINNY))
   {
      MyTA = ATA;
      MyT1 = AT1;
      MyWaterTemperature = AWaterTemperature + ABSOLUTEZERO;
      return exp(MyTA / MyT1 - MyTA / MyWaterTemperature);
   }
   else
      return 1.0;   //No temperature limitation
}
//******************************************************************************************************************************************************
double TDEB::Excrete()
{

}
//******************************************************************************************************************************************************
double TDEB::Reproduce()
{

}
//******************************************************************************************************************************************************
void TDEB::Debugger(double bug)
{
	char szBug[21];
	sprintf(szBug, "%5.10f", bug);
	MessageBox(0,(LPSTR(szBug)),"Debug", MB_OK);
}

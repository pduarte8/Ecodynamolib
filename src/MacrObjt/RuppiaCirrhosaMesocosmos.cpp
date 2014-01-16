// Macrophyte object CPP code file
// @ Pedro Duarte, Novembro de 1998

/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "macrobjt.h"
#include "iodll.h"

TRuppiaCirrhosaMesocosmos::TRuppiaCirrhosaMesocosmos(TEcoDynClass* APEcoDynClass,
        char* className) : TRuppiaCirrhosa(APEcoDynClass, className)
{
}

void TRuppiaCirrhosaMesocosmos::Go()
{
	for (int i = 0; i < NumberOfBoxes; i++)
   {
   	GPP[i] = 0;
      NPP[i] = 0;
      PhytoProd[i] = 0;
      GrossOxygenProductivity[i] = 0;
      NetOxygenProductivity[i] = 0;
      GrossOxygenProduction[i] = 0;
      NetOxygenProduction[i] = 0;
   }
   LagoonDepth = MyPEcoDynClass->GetBoxDepth(0);
   Year = MyPEcoDynClass->GetYear();
   JulianDay = MyPEcoDynClass->GetJulianDay();
   if ((JulianDay >= 146) || (Year > 1))
   {
		Production();
   	Respiration();
   	Mortality();
   	for (int i = 0; i < NumberOfBoxes; i++)
   		PhytoFlux[i] = PhytoProd[i];
   }
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyNutrientPointer != NULL)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		MyNutrientPointer->Update(GetEcoDynClassName(), NetOxygenProduction[i], // mg / d / l
                                    i,
                                    "Oxygen",
                                    ObjectCode);
   	}
   }
}


void TRuppiaCirrhosaMesocosmos::Production()
{
	double CanopyExtinctionCoef, CanopyHeight, CanopyTopLight, BottomLight;
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      if (MyLightPointer != NULL)
      {
   		IkStAndre = GetIkStAndre(IkTancada[JulianDay],LagoonDepth);
      	CanopyHeight = GetCanopyHeight(i);

   		MyLightPointer->Inquiry(GetEcoDynClassName(), CanopyTopLight,
                                     LagoonDepth - CanopyHeight,
											    i,
											    "Sub-surface PAR irradiance",
											    ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), BottomLight,
                                     LagoonDepth,
                                     i,
                                     "Sub-surface PAR irradiance",
											    ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), CanopyExtinctionCoef,
                                     i,
                                     "KValue",
                                     ObjectCode);                            
         if (CanopyTopLight > 0)
         {
	      	GrossOxygenProductivity[i] = (GetDailyPmax()/(CanopyExtinctionCoef * CanopyHeight))*
         						              (log(fabs(IkStAndre + CanopyTopLight * WATTSTOMICROEINSTEINS)/
                 	                        fabs(IkStAndre + BottomLight * WATTSTOMICROEINSTEINS)));//mgO2 g(DW)-1 dia-1

            GrossOxygenProduction[i] = GrossOxygenProductivity[i] *
                                       PhytoBiomass[i] /
                                       LagoonDepth /
                                       CUBIC; //mg O2 l-1 d-1
            NetOxygenProductivity[i] = GrossOxygenProductivity[i];
            NetOxygenProduction[i] = GrossOxygenProduction[i];
            PhytoProd[i] = GrossOxygenProductivity[i] *
                           OxygenToCarbon *
                           CarbonToDryWeight /
                           CUBIC *
                           PhytoBiomass[i]; //g(DW) m-2 d-1

            //Debugger(PhytoProd[i]);
         	GPP[i] = PhytoProd[i];
            NPP[i] = PhytoProd[i];
         }
		}
   }
}

void TRuppiaCirrhosaMesocosmos::Respiration()
{
	double Respiration;
   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	Respiration = MaintenanceRespiration[i] * DAYSTOHOURS; //mgO2 g(DW)-1 dia-1
   	NetOxygenProductivity[i] = NetOxygenProductivity[i] - Respiration;

   	NetOxygenProduction[i] = NetOxygenProduction[i] -
                               Respiration *
      	                      PhytoBiomass[i] /
         	                   LagoonDepth /
            	                CUBIC; //mg O2 l-1 d-1
   	PhytoProd[i] = PhytoProd[i] -
      	            Respiration *
         	         OxygenToCarbon *
            	      CarbonToDryWeight /
               	   CUBIC *
                  	PhytoBiomass[i]; //g(DW) m-2 d-1
   	NPP[i] = PhytoProd[i];
   }
}

double TRuppiaCirrhosaMesocosmos::GetDailyPmax()
{
  return	Pmax[0] * DAYSTOHOURS;
}


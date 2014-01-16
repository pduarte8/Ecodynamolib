// Phytoplankton object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.01.22
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>
	#include <cmath> 
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "phytobjt.h"


//Adaptative phytoplankton slope


TAdaptativeSlopePhytoplankton::TAdaptativeSlopePhytoplankton(
    TEcoDynClass* APEcoDynClass, char* className)
					 : TAdaptativePhytoplankton(APEcoDynClass, className)
{

}



TAdaptativeSlopePhytoplankton::~TAdaptativeSlopePhytoplankton()
{
    freeMemory();
}

void TAdaptativeSlopePhytoplankton::freeMemory()
{
}




void TAdaptativeSlopePhytoplankton :: Production()
{
  double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			D, B1, B2;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

  for (int i = 0; i < NumberOfBoxes; i++)
  {
	  AEiler[i] = DefaultAEiler[i];
	  if (AEiler[i] > 0.0)
		BEiler[i] = 1 / Pmax[i] -
						2 / (Slope[i] * Iopt[i]);
	  else
		BEiler[i] = 1 / Pmax[i];

	  BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
	  MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);

	  if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

	 if (MyLightPointer != NULL)
	 {

		 	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
											  i,
											  "PAR surface irradiance",
											  ObjectCode);

		 	BoxDepth = MyPEcoDynClass->GetBoxDepth(i);

		 	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
											  BoxDepth,
											  i,
											  "Sub-surface PAR irradiance",
											  ObjectCode);


		 LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
		 LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
		 if (LightAtTop == 0.0)
			Productivity = 0.0;
		 else
		 {
			if (AEiler[i] != 0.0)
			{
				D = BEiler[i] * BEiler[i] - 4 * AEiler[i] * CEiler[i];
				B1 = 2*AEiler[i] * LightAtTop + BEiler[i];
				B2 = 2*AEiler[i] * LightAtBottom + BEiler[i];
				if (D < 0.0)
				{
					Productivity = PhytoBiomass[i] *
													2.0 / (KValue*sqrt(-D)) *
													(atan(B1/sqrt(-D))- atan(B2/sqrt(-D)))
													/ (ABottom - ATop)
													* DAYSTOHOURS;
				}
				else if (D == 0.0)
				{
					Productivity = PhytoBiomass[i] *
													2.0 / KValue *
													(1.0 / B2 - 1.0 / B1)
													/ (ABottom - ATop)
													* DAYSTOHOURS;

				}
				else if (D > 0.0)
				{
					Productivity = PhytoBiomass[i] *
													1.0 / (KValue*sqrt(D)) *
													log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
														  ((B1 + sqrt(D)) * (B2 - sqrt(D))))
													/ (ABottom - ATop)
													* DAYSTOHOURS;
				}
			}
			if (AEiler[i] == 0)
				Productivity = PhytoBiomass[i] * 1.0 / (KValue * BEiler[i]) *
														log (fabs(BEiler[i] * LightAtTop + CEiler[i])/
														fabs(BEiler[i] * LightAtBottom + CEiler[i]))
														/ (ABottom - ATop)
														* DAYSTOHOURS;
		 }

       TemperatureLimitation(i);

		 NutrientLimitation(i);
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	  }
	}
	LightShadeAdaptation();
}


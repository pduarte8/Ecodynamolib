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

// ADAPTATIVE

TAdaptativePhytoplankton::TAdaptativePhytoplankton(TEcoDynClass* APEcoDynClass,
        char* className) : TEilerPhytoplankton(APEcoDynClass, className)
{

}

TAdaptativePhytoplankton::~TAdaptativePhytoplankton()
{
    freeMemory();
}

void TAdaptativePhytoplankton::freeMemory()
{
}



void TAdaptativePhytoplankton :: Production()
{
  //const double KValue = 0.25;
  double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			D, B1, B2;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

  for (int i = 0; i < NumberOfBoxes; i++)
  {
	  AEiler[i] = DefaultAEiler[i];
	  BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
	  MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);
	  //Debugger (ATop); Debugger (ABottom);


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

			if (AEiler[i] != 0.0)
			{
				Pmax[i] = 1 /
						(BEiler[i] + 2 * sqrt(AEiler[i] * CEiler[i]));
				Iopt[i] = sqrt(CEiler[i] / AEiler[i]);
			}
			else
				Pmax[i] = 1 /
						BEiler[i];
		 }

       TemperatureLimitation(i);

		 NutrientLimitation(i);
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	  }
	}
	LightShadeAdaptation();
}

void TAdaptativePhytoplankton :: LightShadeAdaptation()
{
	double MyNoonLight, MyAverageLight, ATop, ABottom, MyAverageDepth, Lambda;

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);
		MyAverageDepth = (ATop + ABottom) /
							  2;
		TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
		if (MyLightPointer != NULL)
		{
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyNoonLight,
											  i,
											  "Noon surface PAR",
											  ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyAverageLight,
											  i,
											  "Mean horizontal water PAR irradiance",
											  ObjectCode);

			MyNoonLight = MyNoonLight *
							  WATTSTOMICROEINSTEINS;
			MyAverageLight = MyAverageLight * WATTSTOMICROEINSTEINS;
			//Debugger (MyNoonLight);
			if (MyAverageLight / MyNoonLight < 0.01)
				OptimalChlCarbonRatio[i] = 0.04;
			else
				OptimalChlCarbonRatio[i] = 0.01 +
										 0.03 *
										 log (MyAverageLight / MyNoonLight) /
										 log (0.01);
			if (MyAverageLight > 0.0)
				Lambda = 0.2;//h-1
			else
				Lambda = 0.0;//h-1
			ChlCarbonRatio[i] = ChlCarbonRatio[i] +
								Lambda *
								(OptimalChlCarbonRatio[i] - ChlCarbonRatio[i]) *
								DAYSTOHOURS *
								TimeStep;
			ChlToCarbon[i] = 1 / ChlCarbonRatio[i];
			Slope[i] = Pmax[i] /
						(MyNoonLight * (0.25 - 5.0 * ChlCarbonRatio[i]));
			CEiler[i] = 1 /
						 Slope[i];
			if (DefaultAEiler[i] != 0)
				DefaultAEiler[i] = 1.0 /
						 (Slope[i] * sqr(CriticalLight[i]));

		}
	}
}


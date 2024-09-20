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



// EILER

TEilerPhytoplankton::TEilerPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
						 : TPhytoplanktonBasic(APEcoDynClass, className)
{
	for (int i = 0; i < NumberOfBoxes; i++)
  {
	  DefaultAEiler[i] = 0.004598;
	  AEiler[i] = DefaultAEiler[i];
	  BEiler[i] = 3.160603;
	  CEiler[i] = 735.7589;
  }
  for (int i = 0; i < NumberOfBoxes; i++)
   {
	  if (AEiler[i] != 0.0)
	  {
		Pmax[i] = 1 /
					(BEiler[i] + 2 * sqrt(AEiler[i] * CEiler[i]));
		Iopt[i] = sqrt(CEiler[i] / AEiler[i]);
	  }
	  else
	  {
		Pmax[i] = 1 /
					 BEiler[i]; //The limit of I/(bI+c) when I tends to infinite
		Iopt[i] = 400;
	  }
	  Slope[i] = 1 / CEiler[i]; // The limit of the model derivative with a equal
										 // to zero or different then zero, when I tends to zero
	  CriticalLight[i] = Iopt[i];
   }
}

TEilerPhytoplankton::~TEilerPhytoplankton()
{
    freeMemory();
}

void TEilerPhytoplankton::freeMemory()
{
    try {
        if (NumberOfVariables > 0)
        {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
			cerr << "TEilerPhytoplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TEilerPhytoplankton::Integrate()
{
	int i;
	// Integrate phytoplankton biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
	// Transport the phytoplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
//		MessageBox(0,"About to transport", "", MB_OK);
	{
		//Debugger (BEiler[1]);
		for (i = 0; i < NumberOfBoxes; i++)
		{
			GenericProduct1[i] = PhytoBiomass[i] * DefaultAEiler[i];
			GenericProduct2[i] = PhytoBiomass[i] * AEiler[i];
			GenericProduct3[i] = PhytoBiomass[i] * BEiler[i];
			GenericProduct4[i] = PhytoBiomass[i] * CEiler[i];
			GenericProduct5[i] = PhytoBiomass[i] * CriticalLightExposure[i];
			GenericProduct6[i] = PhytoBiomass[i] * OptimalChlCarbonRatio[i];
			GenericProduct7[i] = PhytoBiomass[i] * ChlCarbonRatio[i];
		}

		MyTransportPointer->Go(GenericProduct1, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct2, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct3, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct4, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct5, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct6, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct7, 0, 0, 0);
		MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);

		for (i = 0; i < NumberOfBoxes; i++)
		{
			if (PhytoBiomass[i] > 0)
			{
				DefaultAEiler[i] = GenericProduct1[i] / PhytoBiomass[i];
				AEiler[i] = GenericProduct2[i] / PhytoBiomass[i];
				BEiler[i] = GenericProduct3[i] / PhytoBiomass[i];
				CEiler[i] = GenericProduct4[i] / PhytoBiomass[i];
				CriticalLightExposure[i] = GenericProduct5[i] / PhytoBiomass[i];
				OptimalChlCarbonRatio[i] = GenericProduct6[i] / PhytoBiomass[i];
				ChlCarbonRatio[i] = GenericProduct7[i] / PhytoBiomass[i];

				//Debugger (JulianDay);


			}
		}
		//Debugger (BEiler[1]);
	}
}

void TEilerPhytoplankton :: Production()
{
	//const double KValue = 0.25;
	double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			 AverageLight, D, B1, B2;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
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
												 ATop,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												 ABottom,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);
			LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
			LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
			if (LightAtTop == 0)
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
}


// Water temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "EcoDynClass.h"

// TWaterTemperatureTwoDimensional Class
//
// Constructors invoked outside EcoDyn shell...
//

TWaterTemperatureTwoDimensional::TWaterTemperatureTwoDimensional(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity)
    :TWaterTemperature(className, timeStep, nLines, nColumns,
                aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
}

TWaterTemperatureTwoDimensional::TWaterTemperatureTwoDimensional(char* className,
        float timeStep, char* morphologyFilename, char* variablesFilename)
              :TWaterTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...


TWaterTemperatureTwoDimensional::TWaterTemperatureTwoDimensional(
        TEcoDynClass* APEcoDynClass, char* className)
			  : TWaterTemperature(APEcoDynClass, className)
{
}

void TWaterTemperatureTwoDimensional::Go()
{
	// Active methods
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	TimeBefore = MyTime;
	GetWaterTemperature();
}


void TWaterTemperatureTwoDimensional::Integrate()
{

	// Temporary
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
    {
		MyTransportPointer->Go(AverageWaterTemperature, AverageWaterTemperature, 0, 0);
    }
}

void TWaterTemperatureTwoDimensional::GetWaterTemperature()
{
	const double WaterSpecificHeat = 4100; // J/kg/k

	double MySalinity,
			 MySunIrradiance,
			 MyLightAtTop,
			 MyLightAtBottom,
			 ATop,
			 ABottom,
			 MyAtmosphericIR,
			 MySVPAtWaterTemperature,
			 MySVPAtAirTemperature,
			 MyAirTemperature,
			 MyTemperatureVariation,
			 MyWindSpeed,
          MyTidalHeight;

	TEcoDynClass* MyTAirTemperaturePointer = MyPEcoDynClass->GetAirTemperaturePointer();
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (MyTAirTemperaturePointer != NULL)
			MyTAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature,
															  i,
															  "Air temperature",
															  ObjectCode);
		else
		{
			return;
		}

		if (MyTransportPointer != NULL)
		{
			MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalinity,
											        i,
											        "Salinity",
											        ObjectCode);
			MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,
													  i,
													  "Box depth",
													  ObjectCode);
			MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,
													  i,
													  "Water density",
													  ObjectCode);
		}
		else
		{
			MySalinity = 37;      // local variable
			MixedLayerDepth = 15; // object variable
			MyWaterDensity = 1027.29; // seawater at 35psu and 10oC
            ABottom = MyPEcoDynClass->GetBoxDepth(i);
		}

		if (MyLightPointer != NULL)
		{
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
												 0,
												 i,
												 "Sub-surface irradiance",
												 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
												 ABottom,
												 i,
												 "Sub-surface irradiance",
												 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyAtmosphericIR,
												 i,
												 "Atmospheric IR",
												 ObjectCode);
		}
		else
		{
			return;
		}

		if (MyWindPointer != NULL)
			MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
												 i,
												 "Wind speed",
												 ObjectCode);
		else
		{
			return;
		}



        WaterIrradiance[i] = GetWaterSurfaceRadiation(AverageWaterTemperature[i]);

        MySVPAtWaterTemperature = GetSaturationVapourPressure(AverageWaterTemperature[i]);

        MySVPAtAirTemperature = GetSaturationVapourPressure(MyAirTemperature);

        LatentHeatFlux[i] = GetLatentWaterHeatFlux(MySVPAtWaterTemperature,
																 MySVPAtAirTemperature,
																 MyWindSpeed);

        SensibleHeatFlux[i] = GetSensibleWaterHeatFlux(AverageWaterTemperature[i],
																	  MyAirTemperature,
																	  MyWindSpeed);


        NetHeatFlux[i] = MyLightAtTop
							  - MyLightAtBottom
							  + MyAtmosphericIR
							  + WaterIrradiance[i]
							  + LatentHeatFlux[i]
							  + SensibleHeatFlux[i];

		MyTemperatureVariation = NetHeatFlux[i]
										 * TimeStep               // Is already in days
										 * DAYSTOSECONDS          // Convert it to seconds
										 / (MyWaterDensity
											 * WaterSpecificHeat
											 * ABottom);

		AverageWaterTemperature[i] = AverageWaterTemperature[i]
											  + MyTemperatureVariation;
	}
}



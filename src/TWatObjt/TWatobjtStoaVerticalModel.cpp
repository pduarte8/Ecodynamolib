// Water temperature object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.02.09
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "EcoDynClass.h"

// TStoaVerticalModelWaterTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TStoaVerticalModelWaterTemperature::TStoaVerticalModelWaterTemperature(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity)
    :TSto0ModelWaterTemperature(className, timeStep, nLines, nColumns,
                aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
}

TStoaVerticalModelWaterTemperature::TStoaVerticalModelWaterTemperature(char* className,
        float timeStep, char* morphologyFilename, char* variablesFilename)
              :TSto0ModelWaterTemperature(className, timeStep, morphologyFilename,
                    variablesFilename)
{
}
// ...


TStoaVerticalModelWaterTemperature::TStoaVerticalModelWaterTemperature(
        TEcoDynClass* APEcoDynClass, char* className)
               : TSto0ModelWaterTemperature(APEcoDynClass, className)
{
}

void TStoaVerticalModelWaterTemperature::GetWaterTemperature()
{
	const double WaterSpecificHeat = 4100 /* J/kg/k */,
                MolecularThermalDiffusivity = 0.00000015 /* m2/s*/;

	double MySalinity,
			 MySunIrradiance,
			 MyLightAtTop,
			 MyLightAtBottom,
			 MyAtmosphericIR,
			 MySVPAtWaterTemperature,
			 MySVPAtAirTemperature,
			 MyAirTemperature,
			 MyTemperatureVariation,
			 MyWindSpeed,
          MyLagoonDepth;

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
			MyTransportPointer->Inquiry(GetEcoDynClassName(), MixedLayerDepth,
													  i,
													  "Mixed layer depth",
													  ObjectCode);
			MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,
													  i,
													  "Water density",
													  ObjectCode);

		}
		else
		{
			MySalinity = 37;      // local variable
			MixedLayerDepth = 10; // object variable
			MyWaterDensity = 1026.75; // seawater at 35psu and 10oC

		}

		if (MyLightPointer != NULL)
		{
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyLagoonDepth,
												 i,
												 "Lagoon depth",
												 ObjectCode);
            if (i == 0)
                MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
												 i,
												 "Total surface irradiance",
												 ObjectCode);
             else

                MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
													 i - 1,
													 "Sub-surface irradiance",
													 ObjectCode);

			MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
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

		if (i == 0)
		{
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
            //Molecular diffusivity of heat with the lower box

            if (NumberOfBoxes > 1)
         	    NetHeatFlux[i] = NetHeatFlux[i] - WaterSpecificHeat *
                             MolecularThermalDiffusivity *
                             MyWaterDensity *
                             (AverageWaterTemperature[i] - AverageWaterTemperature[i + 1]) /
                             (MyLagoonDepth / NumberOfBoxes);
		}
		else
        {
			NetHeatFlux[i] = MyLightAtTop-
							  MyLightAtBottom;
            //Molecular diffusivity of heat with the upper box
            NetHeatFlux[i] = NetHeatFlux[i] - WaterSpecificHeat *
                          MolecularThermalDiffusivity *
                          MyWaterDensity *
                          (AverageWaterTemperature[i] - AverageWaterTemperature[i - 1]) /
                          (MyLagoonDepth / NumberOfBoxes);
            //Molecular diffusivity of heat with the lower box
            if (i < NumberOfBoxes - 1)
         	    NetHeatFlux[i] = NetHeatFlux[i] - WaterSpecificHeat *
                             MolecularThermalDiffusivity *
                             MyWaterDensity *
                             (AverageWaterTemperature[i] - AverageWaterTemperature[i - 1]) /
                             (MyLagoonDepth / NumberOfBoxes);

        }

		MyTemperatureVariation = NetHeatFlux[i]
										 * TimeStep               // Is already in days
										 * DAYSTOSECONDS          // Convert it to seconds
										 / (MyWaterDensity
											 * WaterSpecificHeat
											 * (MyLagoonDepth / NumberOfBoxes));

		AverageWaterTemperature[i] = AverageWaterTemperature[i]
											  + MyTemperatureVariation;
	}
}


void TStoaVerticalModelWaterTemperature::AdvectDiffuseWaterTemperature()
{
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
		MyTransportPointer->Go(AverageWaterTemperature, 0, 0, 0);
    }
}



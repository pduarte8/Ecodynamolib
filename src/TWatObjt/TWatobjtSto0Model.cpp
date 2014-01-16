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

// TSto0ModelWaterTemperature Class
//
// Constructors invoked outside EcoDyn shell...
//

TSto0ModelWaterTemperature::TSto0ModelWaterTemperature(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aAverageTemperature[],
        double genericRiver, double relativeHumidity)
    :TWaterTemperature(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
                aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, aAverageTemperature,
                genericRiver, relativeHumidity)
{
}

TSto0ModelWaterTemperature::TSto0ModelWaterTemperature(char* className,
        float timeStep, char* morphologyFilename, char* variablesFilename)
              :TWaterTemperature(className, timeStep, morphologyFilename, variablesFilename)
{
}
// ...


TSto0ModelWaterTemperature::TSto0ModelWaterTemperature(TEcoDynClass* APEcoDynClass,
        char* className)
               :TWaterTemperature(APEcoDynClass, className)
{
}



void TSto0ModelWaterTemperature::GetWaterTemperature()
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
    TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();

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
			MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterDensity,
											i,
											"Water density",
											ObjectCode);
		}
		else
		{
			MySalinity = 13.5;      // Salinidade m?dia em St. Andr?
			MyWaterDensity = 1008.65; // Calculado a partir da salinidade e tempeeratura m?dia em St. Andr?
		}

		if (MyLightPointer != NULL)
		{
            MyLightPointer->Inquiry(GetEcoDynClassName(), LagoonDepth,
												 i,
												 "Lagoon depth",
												 ObjectCode);

			MyLightPointer->Inquiry(GetEcoDynClassName(), MySunIrradiance,
												 i,
												 "Total surface irradiance",
												 ObjectCode);
            ATop = 0;
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
												 ATop,
												 i,
												 "Sub-surface irradiance",
												 ObjectCode);
            ABottom = LagoonDepth;
            if ((MyTidePointer != NULL) && (i == 0))
      	    {
      		    MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,
            	                        i,
               	                     "Tidal height",
                  	                  ObjectCode);
         	    ABottom = ABottom + MyTidalHeight / CUBIC; //Convert from mm to m
      	    }
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


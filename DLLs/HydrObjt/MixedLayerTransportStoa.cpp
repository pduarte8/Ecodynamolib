// Hydrodynamic object CPP code file
// @ J. Gomes Ferreira
// Rev. A 95.12.31
// Rev. B 97.11.28
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "globals.h"

// TMixedLayerTransportStoa Class
//
// Constructors invoked outside EcoDyn shell...
//

TMixedLayerTransportStoa::TMixedLayerTransportStoa(char* className,
        float timeStep, int nLines, int nColumns,
        double aDepth[], double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aSalt[], double aMixedLayerDepth[],
        double aWaterDensity[], double aUVelocity[], double aVVelocity[],
        double aRichardsonNumber[], double aExchangedFraction[],
        int nParams, double latitude, double coefManning)
    :TMixedLayerTransport(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth,
        aElevation, aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
        nVars, aVarsNames, aSalt, aMixedLayerDepth, aWaterDensity,
        aUVelocity, aVVelocity, aRichardsonNumber, aExchangedFraction,
        nParams, latitude, coefManning)
{
}

TMixedLayerTransportStoa::TMixedLayerTransportStoa(char* className,
        float timeStep, char* morphologyFilename,
        char* variablesFilename, char* parametersFilename)
			:TMixedLayerTransport(className, timeStep, morphologyFilename,
                            variablesFilename, parametersFilename)
{
}

TMixedLayerTransportStoa::TMixedLayerTransportStoa(TEcoDynClass* PEcoDynClass, char* className)
										 :TMixedLayerTransport(PEcoDynClass, className)
{
}


void TMixedLayerTransportStoa::DeterministicPhase()
{
	double MyWindSpeed,
			 MyWindSpeedX,
			 MyWindSpeedY,
			 MyWaterTemperature;
	double MyBoxDepth, MyLagoonDepth;

	// Surface velocity calculation
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
	if (MyWindPointer != NULL)
	{
		MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
											0,
											"Wind speed",
											ObjectCode);
		MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeedX,
											0,
											"Wind speed [x component]",
											ObjectCode);
		MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeedY,
											0,
											"Wind speed [y component]",
											ObjectCode);
	}
	else
	{
#ifdef __BORLANDC__
		MessageBox(0, "Wind object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TMixedLayerTransportStoa::DeterministicPhase - wind object is undefined" << endl;
#endif  // __BORLANDC__
		return;
	}
    if (MyLightPointer != NULL)
    {
   	    MyLightPointer->Inquiry(GetEcoDynClassName(), MyLatitude,
											 0,
											 "Latitude",
											 ObjectCode);

   	    MyLightPointer->Inquiry(GetEcoDynClassName(), MyLagoonDepth,
                                  0,
                                  "Lagoon depth",
                                  ObjectCode);
        MyBoxDepth = MyLagoonDepth / NumberOfBoxes;
	}
    else
    {
#ifdef __BORLANDC__
   	    MessageBox(0, "Light object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TMixedLayerTransportStoa::DeterministicPhase - light object is undefined" << endl;
#endif  // __BORLANDC__
		return;
    }
	UVelocity[0] = UVelocity[0]
						+ Coriolis ( MyLatitude )
						* VVelocity[0]
						* TimeStep                           // Timestep accounted for  in seconds
						+ WindStress ( MyWindSpeed, MyWindSpeedX )
						* TimeStep
						/ ( WaterDensity[0]
							* MyBoxDepth);

	VVelocity[0] = VVelocity[0]
						- Coriolis ( MyLatitude )
						* UVelocity[0]
						* TimeStep                           // Timestep accounted for in seconds
						+ WindStress ( MyWindSpeed, MyWindSpeedY )
						* TimeStep
						/ ( WaterDensity[0]
							 * MyBoxDepth);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (MyWaterTemperaturePointer != NULL)
		{
         MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												            i,
												            "Water temperature",
												            ObjectCode);
		}
		else
		{
#ifdef __BORLANDC__
			MessageBox(0,
						  "Water temperature object is undefined",
						  "EcoDynamo alert",
						  MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLayerTransportStoa::DeterministicPhase - water temperature object is undefined" << endl;
#endif  // __BORLANDC__
			return;
		}
        WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],
                                                 MyWaterTemperature);
	}
}


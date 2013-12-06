// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 00.1.15
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


//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensionalNormalTimeStep::TBiDimensionalNormalTimeStep(char* className,
        float timeStep, char* morphologyFilename)
                    :TBiDimensional(className, timeStep, morphologyFilename)
{
    BuildBiDimensionalNormalTimeStep();
}

TBiDimensionalNormalTimeStep::TBiDimensionalNormalTimeStep(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TBiDimensional(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    BuildBiDimensionalNormalTimeStep();
}
// ...

TBiDimensionalNormalTimeStep::TBiDimensionalNormalTimeStep(TEcoDynClass* PEcoDynClass,
        char* className) :TBiDimensional(PEcoDynClass, className)
{
    BuildBiDimensionalNormalTimeStep();
}

void TBiDimensionalNormalTimeStep::BuildBiDimensionalNormalTimeStep()
{
	SemiTimeStep = 1;
    ATimeStep = ATimeStep * 2.0;
}

TBiDimensionalNormalTimeStep::~TBiDimensionalNormalTimeStep()
{
}



void TBiDimensionalNormalTimeStep::Go()
{
    /*if (samples == 0)
        UpdateMeanSamples();  */
   	TideForcing();
    EquationOfMotion();
    UpdateOfTidalBoundariesVelocities();
    Generic = Salt;
    Continuity();
    Salt = Generic;
    UpdateMeanSamples();        // AP
    
}

void TBiDimensionalNormalTimeStep::Reinitialize()
{
	double MyWaterTemperature;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
      	atemporary[i * GridColumns + j] = 0.0; btemporary[i * GridColumns + j] = 0.0;
         ctemporary[i * GridColumns + j] = 0.0; rtemporary[i * GridColumns + j] = 0.0;
      	if (
         		(BoxDepth[i * GridColumns + j] > CriticalDepthForLandBoundary)
            )
         {
      		BoxDepth[i * GridColumns + j ] = BoxDepth[i * GridColumns + j ] +
                                          	(DynamicHeight[i * GridColumns + j ] - PastHeight[i * GridColumns + j ]);

         }
         //else
         if (BoxDepth[i * GridColumns + j ] <= CriticalDepthForLandBoundary)
         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            VVelocity[i * GridColumns + j] = 0.0;
            UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
            VVelocity[(i + 1) * GridColumns + j] = 0.0;
         }
      }
	}
   //TideForcing();
   RiverForcing();

   if (SemiTimeStep == 2)
   {
   	for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   	{
   		VVelocityPast[i] = VVelocity[i];
   	}

      for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   	{
   		UVelocityPast[i] = UVelocity[i];
   	}

   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
			{
				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

      		WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
      		WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   	}
   }
   else
   if (SemiTimeStep == 1)
   {
   	for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   	{
   		UVelocityPast[i] = UVelocity[i];
   	}

      for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   	{
   		VVelocityPast[i] = VVelocity[i];
   	}

   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
			{
				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

      		WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
      		WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   	}

   }

   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;

   //RiverForcing();
}




void TBiDimensionalNormalTimeStep::Continuity()
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow;

   //Debugger(SemiTimeStep);
   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
      	if (
         		(BoxDepth[i * GridColumns + j] > CriticalDepthForLandBoundary) &&
               (BoxArray[i * GridColumns + j].BoxToBoundary[1].TypeOfBoundary != 2) &&
               (BoxArray[i * GridColumns + j].BoxToBoundary[2].TypeOfBoundary != 2) &&
               (BoxArray[i * GridColumns + j].BoxToBoundary[3].TypeOfBoundary != 2) &&
               (BoxArray[i * GridColumns + j].BoxToBoundary[4].TypeOfBoundary != 2) &&
               (BoxArray[i * GridColumns + j].Dummybool1)
            )
         {
         	MyWestUFlow = WesternComponentOfContinuity(i,j);
            MyEastUFlow = EasternComponentOfContinuity(i,j);
            MyNorthVFlow = NorthernComponentOfContinuity(i,j);
            MySouthVFlow = SouthernComponentOfContinuity(i,j);



            DynamicHeight[i * GridColumns + j ] = (2.0 * ASpatialStep *
                                                   	PastHeight[i * GridColumns + j ] +
                                                   	MyWestUFlow -
                                                   	MyEastUFlow -
                                                   	MyNorthVFlow +
                                                   	MySouthVFlow
                                                   ) /
                                                   (2.0 * ASpatialStep);
      	}
      }
	}
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt);
}


void TBiDimensionalNormalTimeStep::SimetryCheck()
{
	for (int j = 0; j < GridColumns; j++)
   {
   	for (int i = 0; i <= floor(GridLines / 2); i++)
      {
         if (i < floor(GridLines / 2))
         {
      		if (BoxDepth[i * GridColumns + j] - BoxDepth[(GridLines - 1 - i) * GridColumns + j] != 0)
         	{
#ifdef __BORLANDC__
         		MessageBox(0,
					  "SimetryCheck - Profundidades assimétricas",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "TBiDimensionalNormalTimeStep::SimetryCheck - Profundidades assim?tricas" << endl;
#endif  // __BORLANDC__
            	Debugger(i); Debugger(j);
            	Debugger(SemiTimeStep);
            	//Debugger(MyPIntegrate->GetCurrentTime());
         	}
            if (UVelocity[i * (GridColumns + 1) + j ] - UVelocity[(GridLines - 1 - i) * (GridColumns + 1) + j ] != 0)
         	{
#ifdef __BORLANDC__
         		MessageBox(0,
					  "SimetryCheck - Velocidades U assimétricas",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "TBiDimensionalNormalTimeStep::SimetryCheck - Velocidades U assim?tricas" << endl;
#endif  // __BORLANDC__
            	Debugger(i); Debugger(j);
            	Debugger(SemiTimeStep);
            	//Debugger(MyPIntegrate->GetCurrentTime());
         	}

         }
         if (VVelocity[i * GridColumns + j] + VVelocity[(GridLines - i) * GridColumns + j] != 0)
         {
#ifdef __BORLANDC__
         	MessageBox(0,
					  "SimetryCheck - Velocidades V assimétricas",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TBiDimensionalNormalTimeStep::SimetryCheck - Velocidades V assim?tricas" << endl;
#endif  // __BORLANDC__
            Debugger(i); Debugger(j);
            Debugger(SemiTimeStep);
            //Debugger(MyPIntegrate->GetCurrentTime());
            Debugger(VVelocity[i * GridColumns + j] * 10000000000);
            Debugger(VVelocity[(GridLines - i) * GridColumns + j] * 10000000000);
            exit;
         }
      }
   }
}


#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>

	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"
#include "SymbiosesFramework.h"
#include "HydrodynamicModel.h"
#include "AtmosphericModel.h"

TTriDimensionalSymbioses::TTriDimensionalSymbioses(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensionalWithBodyDrag2(PEcoDynClass, className)
{
   SymbiosesFramework &symb = SymbiosesFramework::getInstance();
   ocean = symb.getHydrodynamicModel("ocean");
   atmo = symb.getAtmosphericModel("meteo");
   /*griddims = symb.getReferenceGridDimensions();
   gridlats = symb.getReferenceGridLatitudes();
   gridlons = symb.getReferenceGridLongitudes(); */
   //I have these in EcoDynamo configuration files: GridLines, GridLines, GridLayers, Lats and Longs and these are the ones I should use
   //for I may want to use a subdomain
}

TTriDimensionalSymbioses::~TTriDimensionalSymbioses()
{
}

void TTriDimensionalSymbioses::Go()   //This overwrites previous Go to prevent EcoDynamo from calulating hydrodynamics
{
   cout<<"Start Go"<<endl;
   ReadVariablesFromSymbioses();
   Continuity();
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt); //Salinity being transported - once we get this from SYMBIOSES this line should be removed
   cout<<"End Go"<<endl;
}

void TTriDimensionalSymbioses::ReadVariablesFromSymbioses()
{
    int index3D;
    float v[3], MyLat, MyLong, layers[GridLayers], d;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
        for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
            int ABoxNumber;
            MyLat = MyPEcoDynClass->GetBoxLatitude(Get2DIndex(i,j));
            MyLong = MyPEcoDynClass->GetBoxLongitude(Get2DIndex(i,j));
            ocean->getLayerDepths(MyLat, MyLong, layers);   //I am assuming that getLayerDepths returns the cumulative layer depth from the surface till the bottom
            for (int k = GridLayers - 1; k > 0; k--)  //From surface to bottom in EcoDynamo
            {
                index3D = Get3DIndex(i,j,k);          //In EcoDynamo the zero layer it is at the bottom wheras in SYMBIOSES it is at the surface.
                float d = layers[GridLayers - 1 - k]; //Therefore, here layer coordinates are "inverted" to properly map SYMBIOSES layers
                float e = layers[GridLayers - 1 - k + 1];
                /*if (k == GridLayers - 1)
                    BoxDepth[index3D] = layers[k];     //In spite of the name...BoxDepth correspons to layer thickness in EcoDynamo.
                //BoxDepth[index3D] = 2.0 * layers[k];   //In case layer depth is evaluated at the box "centroid"
                else
                    BoxDepth[index3D] = layers[k] - layers[k + 1];
                //BoxDepth[index3D] = 2.0 * (layers[k] - layers[k + 1] - 0.5 * BoxDepth[Get3DIndex(i,j,k + 1)]);  //In case layer depth is evaluated at the box "centroid"
                */
                //Salt[index3D] = ocean->getSalinity(lat, lon, d);  //Let's try to calculate salinity withint EcoDynamo for mass conservation testing
		//BoxDepth[index3D] = e - d;
                //ocean->getVelocity(MyLat, MyLong, d, v);
                ocean->getVelocity(MyLat, MyLong, BoxDepth[index3D],v);
                UVelocity[Get3DIndexForUVelocity(i,j,k)] = v[1];
                VVelocity[Get3DIndexForVVelocity(i,j,k)] = v[0];
                WVelocity[index3D] = v[2];
            }
        }
    }
}


void TTriDimensionalSymbioses::Continuity()
{
   double EasternDepth, WesternDepth, SouthernDepth, NorthernDepth, MeanBoxWidth,
          UComponent1, UComponent2, VComponent1, VComponent2,
          WesternFlux, EasternFlux, SouthernFlux, NorthernFlux,
          DischargeFlux, UptakeFlux;
   int index, indexUp;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   int layer = UpperLayer;
   //for (int i = 0; i < GridLines; i++)
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
   {
      //for (int j = 0; j < GridColumns; j++)
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i], GridColumns - 1); j++)
      {
         WesternFlux = 0.0; EasternFlux = 0.0; SouthernFlux = 0.0; NorthernFlux = 0.0;
         DischargeFlux = 0.0; UptakeFlux = 0.0;
                 if (!CheckForSeaBoundary(i,j,layer))
         {
            index = Get3DIndex(i,j,layer);

            if (BoxArray[index].Dummybool2)
            {
                indexUp = Get3DIndex(i,j,layer + 1);
                if (
                      (j == GridColumns - 1) ||
                      (
                        (j < GridColumns - 1) && (!BoxArray[Get3DIndex(i,j + 1,layer)].Dummybool2) || (CheckForSeaBoundary(i,j + 1,layer))
                      )
                    )
                {
                       EasternDepth = BoxDepth[index];
                }
                else
                {
                       EasternDepth = 0.5* (BoxDepth[index] + BoxDepth[Get3DIndex(i,j + 1,layer)]);
                }
                if (
                      (j == 0) ||
                      (j > 0) && (!BoxArray[Get3DIndex(i,j - 1,layer)].Dummybool2) ||  (CheckForSeaBoundary(i,j - 1,layer))
                   )
                {
                       WesternDepth = BoxDepth[index];
                }
                else
                {
                       WesternDepth = 0.5 *(BoxDepth[Get3DIndex(i,j - 1,layer)] + BoxDepth[index]);
                }
                if (
                      (i == GridLines - 1) ||
                      (
                        (i < GridLines - 1) && (!BoxArray[Get3DIndex(i + 1,j,layer)].Dummybool2) || (CheckForSeaBoundary(i + 1,j,layer))
                      )
                    )
                {
                       NorthernDepth = BoxDepth[index];
                }
                else
                {
                       NorthernDepth = 0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i + 1,j,layer)]);
                }
                if (
                      (i == 0) ||
                      (
                         (i > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer)].Dummybool2) ||  (CheckForSeaBoundary(i - 1,j,layer))
                      )
                   )
                {
                       SouthernDepth = BoxDepth[index];
                }
                else
                {
                       SouthernDepth = 0.5 * (BoxDepth[Get3DIndex(i - 1,j,layer)] + BoxDepth[index]);
                }
                if (SemiTimeStep == 1)
                {
                  UComponent1 = UVelocity[Get3DIndexForUVelocity(i,j,layer)];
                  UComponent2 = UVelocity[Get3DIndexForUVelocity(i,j + 1,layer)];
                  VComponent1 = VVelocityPast[Get3DIndexForVVelocity(i,j,layer)];
                  VComponent2 = VVelocityPast[Get3DIndexForVVelocity(i + 1,j,layer)];
                }
                else
                if (SemiTimeStep == 2)
                {
                  UComponent1 = UVelocityPast[Get3DIndexForUVelocity(i,j,layer)];
                  UComponent2 = UVelocityPast[Get3DIndexForUVelocity(i,j + 1,layer)];
                  VComponent1 = VVelocity[Get3DIndexForVVelocity(i,j,layer)];
                  VComponent2 = VVelocity[Get3DIndexForVVelocity(i + 1,j,layer)];
                }

                WesternFlux = WesternFlux + UComponent1 * WesternDepth * BoxWidth[index];
                EasternFlux = EasternFlux + UComponent2 * EasternDepth * BoxWidth[index];
                SouthernFlux = SouthernFlux + VComponent1 * SouthernDepth * BoxLength[index];
                NorthernFlux = NorthernFlux + VComponent2 * NorthernDepth * BoxLength[index];
                DischargeFlux = DischargeFlux + FlowDischarge[index];
                UptakeFlux = UptakeFlux + FlowUptake[index];

                if ((BoxArray[index].Dummybool2) && (BoxArray[index].Dummybool1))
                BoxVolume[index] = PastBoxVolume[index] +
                                   (
                                     (WesternFlux - EasternFlux) +
                                     (SouthernFlux - NorthernFlux) +
                                     (DischargeFlux - UptakeFlux)
                                   ) * ATimeStep;
                double DepthVariation;
                DepthVariation = (BoxVolume[index] - PastBoxVolume[index]) / (BoxWidth[index] * BoxLength[index]);
                DynamicHeight[index] = PastHeight[index] + DepthVariation;
                if (layer < UpperLayer)
                   WVelocity[indexUp] = WVelocity[index] -
                                     (UComponent2 * EasternDepth - UComponent1 * WesternDepth) / BoxLength[index] -
                                     (VComponent2 * NorthernDepth - VComponent1 * SouthernDepth) / BoxWidth[index] +
                                     (FlowDischarge[index] - FlowUptake[index]) / (BoxWidth[index] * BoxLength[index]);  
            }
         } //Sea boundary check
         /*index = Get3DIndex(i,j,UpperLayer);
         if (CheckForSeaBoundary(i,j,UpperLayer))
             BoxVolume[index] = ((DynamicHeight[index] - PastHeight[index]) + BoxDepth[index]) * BoxWidth[index] * BoxLength[index];
         else
         {

             if ((BoxArray[index].Dummybool2) && (BoxArray[index].Dummybool1))
                BoxVolume[index] = PastBoxVolume[index] +
                                   (
                                     (WesternFlux - EasternFlux) +
                                     (SouthernFlux - NorthernFlux) +
                                     (DischargeFlux - UptakeFlux)
                                   ) * ATimeStep;
             double DepthVariation;
             DepthVariation = (BoxVolume[index] - PastBoxVolume[index]) / (BoxWidth[index] * BoxLength[index]);
             DynamicHeight[index] = PastHeight[index] + DepthVariation;
        }  */
      }//j
   }   //i
}





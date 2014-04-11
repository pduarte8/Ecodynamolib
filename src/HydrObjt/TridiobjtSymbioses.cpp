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
   ReadVariablesFromSymbioses();
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt); //Salinity being transported - once we get this from SYMBIOSES this line should be removed
}

void TTriDimensionalSymbioses::ReadVariablesFromSymbioses()
{
    int index3D;
    float v[3], lat, lon, layers[GridLayers], d;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
            int ABoxNumber;
            lat = MyPEcoDynClass->GetBoxLatitude(ABoxNumber);
            lon = MyPEcoDynClass->GetBoxLongitude(ABoxNumber);
            ocean->getLayerDepths(lat, lon, layers);   //I am assuming that getLayerDepths returns the cumulative layer depth from the surface till the bottom
            for (int k = GridLayers - 1; k >= 0; k--)  //From surface to bottom in EcoDynamo
            {
                index3D = Get3DIndex(i,j,k);          //In EcoDynamo the zero layer it is at the bottom wheras in SYMBIOSES it is at the surface.
                float d = layers[GridLayers - 1 - k]; //Therefore, here layer coordinates are "inverted" to properly map SYMBIOSES layers
                if (k == GridLayers - 1)
                    BoxDepth[index3D] = layers[k];     //In spite of the name...BoxDepth correspons to layer thickness in EcoDynamo.
                //BoxDepth[index3D] = 2.0 * layers[k];   //In case layer depth is evaluated at the box "centroid"
                else
                    BoxDepth[index3D] = layers[k] - layers[k + 1];
                //BoxDepth[index3D] = 2.0 * (layers[k] - layers[k + 1] - 0.5 * BoxDepth[Get3DIndex(i,j,k + 1)]);  //In case layer depth is evaluated at the box "centroid"
                //Salt[index3D] = ocean->getSalinity(lat, lon, d);  //Let's try to calculate salinity withint EcoDynamo for mass conservation testing
                ocean->getVelocity(lat, lon, d, v);
                UVelocity[Get3DIndexForUVelocity(i,j,k)] = v[1];
                VVelocity[Get3DIndexForVVelocity(i,j,k)] = v[0];
                WVelocity[index3D] = v[2];
            }
        }
    }
}






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

TTriDimensionalSymbiosesV1::TTriDimensionalSymbiosesV1(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensionalSymbioses(PEcoDynClass, className)
{
    SymbiosesFramework &symb = SymbiosesFramework::getInstance();
    ocean = symb.getHydrodynamicModel("ocean");
    atmo = symb.getAtmosphericModel("meteo");
    /*griddims = symb.getReferenceGridDimensions();
    gridlats = symb.getReferenceGridLatitudes();
    gridlons = symb.getReferenceGridLongitudes(); */
    //I have these in EcoDynamo configuration files: GridLines, GridLines, GridLayers, Lats and Longs and these are the ones I should use
    //for I may want to use a subdomain
    Tilt = (double*) calloc(NumberOfBoxes, sizeof(double));
    cout<<NumberOfBoxes<<endl;
    int X,Y, XV, YV;
    double MyValue;
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            cout<<NumberOfParameters<<endl;
            // read in the parameter names
            char MyParameter[65];
            for (int l = Y; l < Y + NumberOfParameters; l++)
            {
                PReadWrite->ReadString(X+2, l, MyParameter);
                cout<<MyParameter<<endl;
                if (strcmp(MyParameter, "Tilt") == 0)
                {
                    cout<<"Tilt found"<<endl;
                    int index3D;
                    for (int i = 0; i < GridLines; i++)
                    {
                        for (int j = 0; j < GridColumns; j++)
                        {
                            index3D = Get3DIndex(i,j,0);
                            PReadWrite->ReadNumber(X + 3, l + index3D, MyValue);
                            for (int k = 0; k < GridLayers; k++)  //From surface to bottom in EcoDynamo
                            {
                               index3D = Get3DIndex(i,j,k);
                               Tilt[index3D] = MyValue * M_PI / 180.0;
                            }
                       }
                    }
                    cout<<index3D<<endl;
                }
            }
            cout<<Tilt[NumberOfBoxes-1]<<endl;
        }
        else
        {
            #ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object Transport", "EcoDynamo alert", MB_OK);
            #else  // __BORLANDC__
            cerr << "TTriDimensional::BuildTriDimensionalIntertidal - parameters: undefined object" << endl;
            #endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TTriDimensionalSymbiosesV1::~TTriDimensionalSymbiosesV1()
{
   freeMemory();
}

void TTriDimensionalSymbiosesV1::freeMemory()
{
   try
   {
      if (NumberOfBoxes > 0) {
          free(Tilt);
      }
      if (NumberOfVariables > 0) {
          delete [] VariableNameArray;
          NumberOfVariables = 0;
      }
   }
   #ifdef __BORLANDC__
      catch (EAccessViolation &e) {}
      catch (Exception &exception) {}
   #else  // __BORLANDC__
      catch (exception &exc) {
      cerr << "TIce::freeMemory - " << exc.what() << endl;
   }
   #endif  // __BORLANDC__
}

void TTriDimensionalSymbiosesV1::Go()   //This overwrites previous Go to prevent EcoDynamo from calculating hydrodynamics
{
   cout<<"Start Go"<<endl;
   ReadVariablesFromSymbioses();
   CorrectVelocities();
   Continuity();
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt); //Salinity being transported - once we get this from SYMBIOSES this line should be removed
   cout<<"End Go"<<endl;
}

void TTriDimensionalSymbiosesV1::ReadVariablesFromSymbioses()
{
    int index3D;
    float v[3], MyLat, MyLong, layers[GridLayers], d, MyDepth;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
            int ABoxNumber;
            MyLat = MyPEcoDynClass->GetBoxLatitude(Get2DIndex(i,j));
            MyLong = MyPEcoDynClass->GetBoxLongitude(Get2DIndex(i,j));
            ocean->getLayerDepths(MyLat, MyLong, layers);   //I am assuming that getLayerDepths returns the cumulative layer depth from the surface till the bottom
            MyDepth = 0.0;
            for (int k = GridLayers - 1; k >= 0; k--)  //From surface to bottom in EcoDynamo
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
                MyDepth = MyDepth + BoxDepth[index3D];
                ocean->getVelocity(MyLat, MyLong, MyDepth,v);
                UVelocity[Get3DIndexForUVelocity(i,j,k)] = v[1];
                VVelocity[Get3DIndexForVVelocity(i,j,k)] = v[0];
                WVelocity[index3D] = v[2];
            }
        }
    }
}

void TTriDimensionalSymbiosesV1::CorrectVelocities()
{
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   int index2D;
   double ResultantDirection, ResultantVelocity, TINNY;
   TINNY = 0.0000001;
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
   {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
            for (int k = GridLayers - 1; k >= 0; k--)  //From surface to bottom in EcoDynamo
            {
                index2D = Get2DIndex(i,j);          //In EcoDynamo the zero layer it is at the bottom wheras in SYMBIOSES it is at the surface.
                ResultantVelocity = sqrt(pow(UVelocity[Get3DIndexForUVelocity(i,j,k)],2.0) + pow(VVelocity[Get3DIndexForVVelocity(i,j,k)],2.0));

                if (ResultantVelocity <= TINNY)
                   ResultantDirection = 0.0;
                else
                {
                   if (UVelocity[Get3DIndexForUVelocity(i,j,k)] > 0.0)
                      ResultantDirection =  acos(VVelocity[Get3DIndexForVVelocity(i,j,k)] / ResultantVelocity);
                   else
                      ResultantDirection =  2.0 * M_PI - acos(VVelocity[Get3DIndexForVVelocity(i,j,k)] / ResultantVelocity);
                }
                UVelocity[Get3DIndexForUVelocity(i,j,k)] = UVelocity[Get3DIndexForUVelocity(i,j,k)] * sin(ResultantDirection + Tilt[index2D]);
                VVelocity[Get3DIndexForVVelocity(i,j,k)] = VVelocity[Get3DIndexForVVelocity(i,j,k)] * cos(ResultantDirection + Tilt[index2D]);
            }
        }
    }
}






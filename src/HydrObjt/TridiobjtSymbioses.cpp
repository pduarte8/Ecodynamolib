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
#define RAW_GRIDS
#include "HydrodynamicModel.h"
//#include "HydrodynamicGridModel.h"
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

TTriDimensionalSymbioses::~TTriDimensionalSymbioses()
{
   freeMemory();
}

void TTriDimensionalSymbioses::freeMemory()
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

void TTriDimensionalSymbioses::Go()   //This overwrites previous Go to prevent EcoDynamo from calulating hydrodynamics
{
   cout<<"Start Go"<<endl;
   ReadVariablesFromSymbioses();
   //CorrectVelocities();
   Continuity();
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt); //Salinity being transported - once we get this from SYMBIOSES this line should be removed
   cout<<"End Go"<<endl;
}

void TTriDimensionalSymbioses::ReadVariablesFromSymbioses()
{
    int index3D;
    float v[3], MyLat, MyLong, layers[GridLayers], MyDepth;
    float* MyUVelocity = new float[NumberOfBoxes + GridLines * GridLayers];
    float* MyVVelocity = new float[NumberOfBoxes + GridColumns * GridLayers];
    float* MyWVelocity = new float[NumberOfBoxes];
    float* MyElevation = new float[NumberOfBoxes];
    ocean->getUGrid(MyUVelocity);
    ocean->getVGrid(MyVVelocity);
    ocean->getWGrid(MyWVelocity);
    ocean->getElevatedDepthGrid(MyElevation);
    cout<<MyElevation[Get3DIndex(0,0,0)]<<endl;
    cout<<MyElevation[Get3DIndex(0,0,35)]<<endl;
    cout<<MyElevation[Get3DIndex(335,0,0)]<<endl;
    cout<<MyElevation[Get3DIndex(335,0,35)]<<endl;
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
                ocean->getUGridPoint(i,j,k);
                //MyDepth = MyDepth + BoxDepth[index3D];
                //ocean->getVelocity(MyLat, MyLong, MyDepth,v);
                UVelocity[Get3DIndexForUVelocity(i,j,k)] = MyUVelocity[Get3DIndexForUVelocity(i,j,k - GridLayers)];
                VVelocity[Get3DIndexForVVelocity(i,j,k)] = MyVVelocity[Get3DIndexForVVelocity(i,j,k - GridLayers)];
                WVelocity[index3D] = MyWVelocity[Get3DIndex(i,j,k - GridLayers)];
            }
        }
    }
    delete [] MyUVelocity; delete [] MyVVelocity; delete [] MyWVelocity; delete [] MyElevation;
}

void TTriDimensionalSymbioses::CorrectVelocities()
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
   for (int i = pSubDomain->FirstLine;i <= pSubDomain->LastLine; i++)
   {
      //for (int j = 0; j < GridColumns; j++)
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i], GridColumns - 1); j++)
      {
         WesternFlux = 0.0; EasternFlux = 0.0; SouthernFlux = 0.0; NorthernFlux = 0.0;
         DischargeFlux = 0.0; UptakeFlux = 0.0;
         for (int layer = 0; layer <= UpperLayer; layer++)
         {
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
                    /*if ((i == 47) && (j == 5))
                    {
                       cout<<UComponent1<<","<<UComponent2<<","<<VComponent1<<","<<VComponent2<<endl;
                       cout<<WesternDepth<<","<<EasternDepth<<","<<SouthernDepth<<","<<NorthernDepth<<endl;
                       cout<<BoxLength[index]<<","<<BoxWidth[index]<<endl;
                       int h;
                       cin >> h;
                    }   */
                    WesternFlux = WesternFlux + UComponent1 * WesternDepth * BoxWidth[index];
                    EasternFlux = EasternFlux + UComponent2 * EasternDepth * BoxWidth[index];
                    SouthernFlux = SouthernFlux + VComponent1 * SouthernDepth * BoxLength[index];
                    NorthernFlux = NorthernFlux + VComponent2 * NorthernDepth * BoxLength[index];
                    DischargeFlux = DischargeFlux + FlowDischarge[index];
                    UptakeFlux = UptakeFlux + FlowUptake[index];

                    /*if ((BoxArray[index].Dummybool2) && (BoxArray[index].Dummybool1))
                    BoxVolume[index] = PastBoxVolume[index] +
                                       (
                                         (WesternFlux - EasternFlux) +
                                         (SouthernFlux - NorthernFlux) +
                                         (DischargeFlux - UptakeFlux)
                                       ) * ATimeStep;
                    double DepthVariation;
                    DepthVariation = (BoxVolume[index] - PastBoxVolume[index]) / (BoxWidth[index] * BoxLength[index]);
                    DynamicHeight[index] = PastHeight[index] + DepthVariation;  */
                    if (layer < UpperLayer)
                       WVelocity[indexUp] = WVelocity[index] -
                                         (UComponent2 * EasternDepth - UComponent1 * WesternDepth) / BoxLength[index] -
                                         (VComponent2 * NorthernDepth - VComponent1 * SouthernDepth) / BoxWidth[index] +
                                         (FlowDischarge[index] - FlowUptake[index]) / (BoxWidth[index] * BoxLength[index]);
                }
             } //Sea boundary check
         }//Layer
         index = Get3DIndex(i,j,UpperLayer);
         if (!CheckForSeaBoundary(i,j,UpperLayer))
            /* BoxVolume[index] = ((DynamicHeight[index] - PastHeight[index]) + BoxDepth[index]) * BoxWidth[index] * BoxLength[index];
         else */
         {
             if ((BoxArray[index].Dummybool2) && (BoxArray[index].Dummybool1))
                BoxVolume[index] = PastBoxVolume[index] +
                                   (
                                     (WesternFlux - EasternFlux) +
                                     (SouthernFlux - NorthernFlux) +
                                     (DischargeFlux - UptakeFlux)
                                   ) * ATimeStep;
                /*if (BoxVolume[index] < 0.0)
                {
                   cout<<i<<endl;
                   cout<<j<<endl;
                   cout<<PastBoxVolume[index]<<endl;
                   cout<<WesternFlux<<endl;
                   cout<<EasternFlux<<endl;
                   cout<<SouthernFlux<<endl;
                   cout<<NorthernFlux<<endl;
                   cout<<DischargeFlux<<endl;
                   cout<<UptakeFlux<<endl;
                   cout<<ATimeStep<<endl;
                   int h;  
                   cin >> h; 
                }    */
                 /*BoxVolume[index] = PastBoxVolume[index] +
                                   (
                                     (WesternFlux - EasternFlux) +
                                     (SouthernFlux - NorthernFlux) +
                                     WVelocity[index] * BoxWidth[index] * BoxLength[index] + 
                                     (DischargeFlux - UptakeFlux)
                                   ) * ATimeStep;  */
             double DepthVariation;
             DepthVariation = (BoxVolume[index] - PastBoxVolume[index]) / (BoxWidth[index] * BoxLength[index]);
             DynamicHeight[index] = PastHeight[index] + DepthVariation;
        }
      }//j
   }   //i
}


void TTriDimensionalSymbioses::AdvectDiffuse(double* Generic)
{
   double EasternDepth, WesternDepth, SouthernDepth, NorthernDepth,
          UComponent1, UComponent2, VComponent1, VComponent2, WComponent1, WComponent2,
          WesternConcentration, EasternConcentration, SouthernConcentration, NorthernConcentration,
          ConcentrationBelow, ConcentrationAbove;
   int index, cont;

   double* PastGeneric = new double[NumberOfBoxes];
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int i = 0; i < NumberOfBoxes; i++)
   	  PastGeneric[i] = Generic[i];
   cont = 0;
   //for (int i = 0; i < GridLines; i++)
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         for (int layer = 0; layer <= UpperLayer; layer++)
         {
            index = Get3DIndex(i,j,layer);
            if (BoxArray[index].Dummybool2)
            {
                if ((!CheckForSeaBoundary(i,j,layer)) && (BoxArray[index].Dummybool1))
                {
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

                    WComponent1 = WVelocityPast[index];
                    if (layer < UpperLayer)
                        WComponent2 = WVelocityPast[Get3DIndex(i,j,layer + 1)];
                    else
                        WComponent2 = 0.0;
                    WesternConcentration = 0.0; EasternConcentration = 0.0; SouthernConcentration = 0.0; NorthernConcentration = 0.0;
                    ConcentrationBelow = 0.0; ConcentrationAbove = 0.0;


                    if (
                          (j == GridColumns - 1) ||
                          (
                            (j < GridColumns - 1) && (!BoxArray[Get3DIndex(i,j + 1,layer)].Dummybool2) || (CheckForSeaBoundary(i,j + 1,layer))
                          )
                        )
                    {
                        EasternDepth = BoxDepth[index];
                        if ((j < GridColumns - 1) && (CheckForSeaBoundary(i,j + 1,layer)))
                           EasternConcentration = PastGeneric[Get3DIndex(i,j + 1,layer)];
                        else
                           EasternConcentration = PastGeneric[index];
                    }
                    else
                    {
                        EasternDepth = 0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i,j + 1,layer)]);
                        if (UComponent2 > 0.0)
                           EasternConcentration = PastGeneric[index];
                        else
                           EasternConcentration = PastGeneric[Get3DIndex(i,j + 1,layer)];
                    }
                    if (
                          (j == 0) ||
                          (j > 0) && (!BoxArray[Get3DIndex(i,j - 1,layer)].Dummybool2) ||  (CheckForSeaBoundary(i,j - 1,layer))
                       )
                    {
                        WesternDepth = BoxDepth[index];
                        if ((j > 0) && (CheckForSeaBoundary(i,j - 1,layer)))
                           WesternConcentration = PastGeneric[Get3DIndex(i,j - 1,layer)];
                        else
                           WesternConcentration = PastGeneric[index];
                    }
                    else
                    {
                        WesternDepth = 0.5 *(BoxDepth[Get3DIndex(i,j - 1,layer)] + BoxDepth[index]);
                        if (UComponent1 > 0.0)
                           WesternConcentration = PastGeneric[Get3DIndex(i,j - 1,layer)];
                        else
                           WesternConcentration = PastGeneric[index];
                    }
                    if (
                          (i == GridLines - 1) ||
                          (
                            (i < GridLines - 1) && (!BoxArray[Get3DIndex(i + 1,j,layer)].Dummybool2) || (CheckForSeaBoundary(i + 1,j,layer))
                          )
                        )
                    {
                        NorthernDepth = BoxDepth[index];
                        if ((i < GridLines - 1) && (CheckForSeaBoundary(i + 1,j,layer)))
                           NorthernConcentration = PastGeneric[Get3DIndex(i + 1,j,layer)];
                        else
                           NorthernConcentration = PastGeneric[index];
                    }
                    else
                    {
                        NorthernDepth = 0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i + 1,j,layer)]);
                        if (VComponent2 > 0.0)
                           NorthernConcentration = PastGeneric[index];
                        else
                           NorthernConcentration = PastGeneric[Get3DIndex(i + 1,j,layer)];
                    }
                    if (
                          (i == 0) ||
                          (i > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer)].Dummybool2) ||  (CheckForSeaBoundary(i - 1,j,layer))
                       )
                    {
                        SouthernDepth = BoxDepth[index];
                        if ((i > 0) && (CheckForSeaBoundary(i - 1,j,layer)))
                           SouthernConcentration = PastGeneric[Get3DIndex(i - 1,j,layer)];
                        else
                           SouthernConcentration = PastGeneric[index];
                    }
                    else
                    {
                        SouthernDepth = 0.5 * (BoxDepth[Get3DIndex(i - 1,j,layer)] + BoxDepth[index]);
                        if (VComponent1 > 0.0)
                           SouthernConcentration = PastGeneric[Get3DIndex(i - 1,j,layer)];
                        else
                           SouthernConcentration = PastGeneric[index];
                    }
                    if ((layer > 0) && (BoxArray[Get3DIndex(i,j,layer - 1)].Dummybool2))
                    {
                       if (WComponent1 > 0.0)
                          ConcentrationBelow = PastGeneric[Get3DIndex(i,j,layer-1)];
                       else
                          ConcentrationBelow = PastGeneric[Get3DIndex(i,j,layer)];
                    }
                    if ((layer < UpperLayer) && (BoxArray[Get3DIndex(i,j,layer + 1)].Dummybool2))
                    {
                       if (WComponent2 > 0.0)
                          ConcentrationAbove = PastGeneric[Get3DIndex(i,j,layer)];
                       else
                          ConcentrationAbove = PastGeneric[Get3DIndex(i,j,layer + 1)];
                    }
                    double TINNY = 0.0000001;
                    r[cont] = PastGeneric[index] * BoxDepth[index] +
                             (
                               (UComponent1 * WesternDepth * WesternConcentration - UComponent2 * EasternDepth * EasternConcentration) / BoxLength[index] +
                               (VComponent1 * SouthernDepth * SouthernConcentration - VComponent2 * NorthernDepth * NorthernConcentration) / BoxWidth[index] +
                               (WComponent1 * ConcentrationBelow - WComponent2 * ConcentrationAbove) +
                               SaltWEHorizontalDiffusion(i,j,layer, PastGeneric) +
                               SaltNSHorizontalDiffusion(i,j,layer, PastGeneric)
                             ) * ATimeStep;
                    b[cont] = BoxVolume[index] / (BoxLength[index] * BoxWidth[index]);

                    if ((layer > 0) && (BoxArray[Get3DIndex(i,j,layer-1)].Dummybool2))
                       a[cont] = - 0.25 * (
                                            VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,layer)] + VerticalDiffCoefU[Get3DIndexForUVelocity(i,j+1,layer)]+
                                            VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,layer)] + VerticalDiffCoefV[Get3DIndexForVVelocity(i+1,j,layer)]
                                          ) / (0.5 * (BoxDepth[Get3DIndex(i,j,layer-1)] + BoxDepth[index])) * ATimeStep;
                    else
                       a[cont] = 0.0;
                    b[cont] = b[cont] - a[cont];

                    if ((layer < UpperLayer) && (BoxArray[Get3DIndex(i,j,layer+1)].Dummybool2))
                       c[cont] = - 0.25 * (
                                            VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,layer + 1)] + VerticalDiffCoefU[Get3DIndexForUVelocity(i,j+1,layer + 1)]+
                                            VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,layer + 1)] + VerticalDiffCoefV[Get3DIndexForVVelocity(i+1,j,layer + 1)]
                                          ) / (0.5 * (BoxDepth[Get3DIndex(i,j,layer+1)] + BoxDepth[index])) * ATimeStep;
                    else
                       c[cont] = 0.0;
                    b[cont] = b[cont] - c[cont];
                    /*if ((MyPEcoDynClass->GetCurrTime() >= 0.093) && (cont == 1)){
                      Debugger(103);  Debugger(i); Debugger(j); Debugger(layer);
                      Debugger(VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,layer + 1)]);
                      Debugger(VerticalDiffCoefU[Get3DIndexForUVelocity(i,j+1,layer + 1)]);
                      Debugger(VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,layer + 1)]);
                      Debugger(VerticalDiffCoefV[Get3DIndexForVVelocity(i+1,j,layer + 1)]);
                      Debugger(BoxDepth[index]);
                      Debugger(BoxDepth[Get3DIndex(i,j,layer+1)]);
                      Debugger(c[cont]);
                      Debugger(b[cont]);
                    } */
                    cont++;
                }
            }
         }  //layer
      }     //j
   }        //i
   IntegrationTridag(a, b, c, r, cont);
   cont = 0;
   //for (int i = 0; i < GridLines; i++)
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         for (int layer = 0; layer <= UpperLayer; layer++)
         {
            index = Get3DIndex(i,j,layer);
            if (BoxArray[index].Dummybool2)
            {
                if ((!CheckForSeaBoundary(i,j,layer)) && (BoxArray[index].Dummybool1))
                {
                   Generic[index] = u[cont];
                   a[cont] = 0.0; b[cont] = 0.0; c[cont] = 0.0; r[cont] = 0.0;
                   cont++;
                }
            }
         }
      }
   }
   if (MyLoadRecord != NULL)
   {
       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
       {
          int i = MyLoadRecord[k].LineCoordinate;
          int j = MyLoadRecord[k].ColumnCoordinate;
          int l = MyLoadRecord[k].LayerCoordinate;
          index = Get3DIndex(i,j,l);
          if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j)) && (BoxDepth[Get3DIndex(i,j,l)] > 0.0))
          {
             /*Debugger(MyPEcoDynClass->GetNumberOfLoads());
             Debugger(FlowDischarge[index]);
             Debugger(GenericLoad[index]);  */
             Generic[index] = Generic[index ] + ATimeStep * GenericLoad[index] *
                                                         FlowDischarge[index] /
                                                         (
                                                            (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                                            (BoxLength[index] * BoxWidth[index])
                                                         );
          }
       }
   }
   if (MyLossesRecord != NULL)
   {
       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLosses(); k++)
       {
          int i = MyLossesRecord[k].LineCoordinate;
          int j = MyLossesRecord[k].ColumnCoordinate;
          int l = MyLossesRecord[k].LayerCoordinate;
          index = Get3DIndex(i,j,l);
          if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j)) && (BoxDepth[Get3DIndex(i,j,l)] > 0.0))
          {
             /*Debugger(MyPEcoDynClass->GetNumberOfLoads());
             Debugger(FlowDischarge[index]);
             Debugger(GenericLoad[index]);  */
             Generic[index] = Generic[index ] - ATimeStep * PastGeneric[index] *
                                                         FlowUptake[index] /
                                                         (
                                                            (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                                            (BoxLength[index] * BoxWidth[index])
                                                         );
          }
       }
   }
   delete [] PastGeneric;
}





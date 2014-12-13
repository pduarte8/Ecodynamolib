#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>

	using namespace std;
#endif  // __BORLANDC__

#define RAW_GRIDS
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"
#include "SymbiosesFramework.h"
#include "HydrodynamicModel.h"
#include "AtmosphericModel.h"

TTriDimensionalSymbiosesV2::TTriDimensionalSymbiosesV2(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensionalSymbioses(PEcoDynClass, className)
{
    Step = 0;
}

TTriDimensionalSymbiosesV2::~TTriDimensionalSymbiosesV2()
{
   freeMemory();
}


void TTriDimensionalSymbiosesV2::Go()   //This overwrites previous Go to prevent EcoDynamo from calulating hydrodynamics
{
   cout<<"Start Go V2"<<endl;
   cout<<"Read variables"<<endl;
   ReadVariablesFromSymbioses();
   cout<<"Continuity"<<endl;
   if (Step > 0)
   {
      Continuity();
      GenericLoad = SaltLoad;
      AdvectDiffuse(Salt); //Salinity being transported - once we get this from SYMBIOSES this line should be removed
   }
   cout<<"End Go"<<endl;
   Step++;
}



void TTriDimensionalSymbiosesV2::Continuity()
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

                    UComponent1 = 0.5 * (UVelocity[Get3DIndexForUVelocity(i,j,layer)] + UVelocityPast[Get3DIndexForUVelocity(i,j,layer)]);
                    UComponent2 = 0.5 * (UVelocity[Get3DIndexForUVelocity(i,j + 1,layer)] + UVelocityPast[Get3DIndexForUVelocity(i,j + 1,layer)]);
                    VComponent1 = 0.5 * (VVelocityPast[Get3DIndexForVVelocity(i,j,layer)] + VVelocity[Get3DIndexForVVelocity(i,j,layer)]);
                    VComponent2 = 0.5 * (VVelocityPast[Get3DIndexForVVelocity(i + 1,j,layer)] + VVelocity[Get3DIndexForVVelocity(i + 1,j,layer)]);

                    WesternFlux = WesternFlux + UComponent1 * WesternDepth * BoxWidth[index];
                    EasternFlux = EasternFlux + UComponent2 * EasternDepth * BoxWidth[index];
                    SouthernFlux = SouthernFlux + VComponent1 * SouthernDepth * BoxLength[index];
                    NorthernFlux = NorthernFlux + VComponent2 * NorthernDepth * BoxLength[index];
                    DischargeFlux = DischargeFlux + FlowDischarge[index];
                    UptakeFlux = UptakeFlux + FlowUptake[index];

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
        }
      }//j
   }   //i
}


void TTriDimensionalSymbiosesV2::AdvectDiffuse(double* Generic)
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
                    UComponent1 = 0.5 * (UVelocity[Get3DIndexForUVelocity(i,j,layer)] + UVelocityPast[Get3DIndexForUVelocity(i,j,layer)]);
                    UComponent2 = 0.5 * (UVelocity[Get3DIndexForUVelocity(i,j + 1,layer)] + UVelocityPast[Get3DIndexForUVelocity(i,j + 1,layer)]);
                    VComponent1 = 0.5 * (VVelocityPast[Get3DIndexForVVelocity(i,j,layer)] + VVelocity[Get3DIndexForVVelocity(i,j,layer)]);
                    VComponent2 = 0.5 * (VVelocityPast[Get3DIndexForVVelocity(i + 1,j,layer)] + VVelocity[Get3DIndexForVVelocity(i + 1,j,layer)]);

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





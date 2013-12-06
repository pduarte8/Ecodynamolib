// Tridimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 10/07/2007
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
#include "iodll.h"
//#include "params.h"

TTriDimensionalWithBodyDrag2::TTriDimensionalWithBodyDrag2(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensionalWithBodyDrag(PEcoDynClass, className)

{
    FUPw = new double[NumberOfBoxes];
    FLWw = new double[NumberOfBoxes];
    TDVw =  new double[NumberOfBoxes];
    for (int i = 0; i++; i< NumberOfBoxes)
    {
        FUPw[i] = 0.0; FLWw[i] = 0.0; TDVw[i] = 0.0;
    }
}

TTriDimensionalWithBodyDrag2::~TTriDimensionalWithBodyDrag2()
{
   try {
       if (NumberOfBoxes > 0)
       {
         delete [] FUPw;
         delete [] FLWw;
         delete [] TDVw;
       }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
    catch (exception &exc) {
        cerr << "TTriDimensionalWithBodyDrag2::~TTriDimensionalWithBodyDrag2 - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensionalWithBodyDrag2::AdvectDiffuse(double* Generic)
{
   double EasternDepth, WesternDepth, SouthernDepth, NorthernDepth, LowerDepth, UpperDepth,
          UComponent1, UComponent2, VComponent1, VComponent2, WComponent1, WComponent2,
          WesternConcentration, EasternConcentration, SouthernConcentration, NorthernConcentration,
          ConcentrationBelow, ConcentrationAbove, MyR, MyFr, FLMMinusOneHalf, FLMPlusOneHalf, FLMPlusThreeHalves;
   int index, cont, S;
   double const MinDepth = 0.0000001;
   double* PastGeneric = new double[NumberOfBoxes];
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int i = 0; i < NumberOfBoxes; i++)
   	  PastGeneric[i] = Generic[i];

   cont = 0;
   if (RemovingLayer)
   {
      for (int i = pSubDomain->FirstLine;i <= pSubDomain->LastLine; i++)
      {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
           index = Get3DIndex(i,j,UpperLayer);
           if (PastBoxDepth[index] + BoxDepth[Get3DIndex(i,j,UpperLayer + 1)] > MinDepth)
              PastGeneric[index] = (PastGeneric[index] * PastBoxDepth[index] +
                                PastGeneric[Get3DIndex(i,j,UpperLayer + 1)] * BoxDepth[Get3DIndex(i,j,UpperLayer + 1)]) /
                                (PastBoxDepth[index] + BoxDepth[Get3DIndex(i,j,UpperLayer + 1)]);
           else
              PastGeneric[index] = 0.0;
           Generic[index] = PastGeneric[index];
           if (!CheckForSeaBoundary(i,j,UpperLayer + 1)){
              PastGeneric[Get3DIndex(i,j,UpperLayer + 1)] = 0.0;
              Generic[Get3DIndex(i,j,UpperLayer + 1)] = PastGeneric[Get3DIndex(i,j,UpperLayer + 1)];
           }
        }
      }
   }
   if (AddingLayer)
   {
      for (int i = pSubDomain->FirstLine;i <= pSubDomain->LastLine; i++)
      {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
           index = Get3DIndex(i,j,UpperLayer);
           PastGeneric[index] = PastGeneric[Get3DIndex(i,j,UpperLayer - 1)];
           Generic[index] = PastGeneric[index];
           if ((PastBoxDepth[Get3DIndex(i,j,UpperLayer - 1)] <= 0.0000000001) && (!CheckForSeaBoundary(i,j,UpperLayer - 1)))
           {
              PastGeneric[Get3DIndex(i,j,UpperLayer - 1)] = 0.0;
              Generic[Get3DIndex(i,j,UpperLayer - 1)] = PastGeneric[Get3DIndex(i,j,UpperLayer - 1)];
           }
        }
      }
   }
   //Upwind scheme only for horizontal transport components
   //Total Diminishing Variance (TDV) scheme only for vertical transport
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         //Upwind and Lax-Wendroff schemes only for vertical transport component
         for (int layer = 0; layer <= UpperLayer; layer++)
         {
            index = Get3DIndex(i,j,layer);
            FUPw[index] = 0.0; FLWw[index] = 0.0; TDVw[index] = 0.0;
            if (BoxArray[index].Dummybool2)
            {
                if (
                     (!CheckForSeaBoundary(i,j,layer)) && (BoxArray[index].Dummybool1) &&
                     (layer > 0) && (BoxArray[Get3DIndex(i,j,layer - 1)].Dummybool2)
                   )
                {
                   WComponent1 = WVelocityPast[index];
                   LowerDepth = 0.5 * (BoxDepth[Get3DIndex(i,j,layer - 1)] + BoxDepth[index]);
                   if (WComponent1 > 0.0)
                      ConcentrationBelow = PastGeneric[Get3DIndex(i,j,layer-1)];
                   else
                      ConcentrationBelow = PastGeneric[Get3DIndex(i,j,layer)];

                   FUPw[index] = WComponent1 * ConcentrationBelow;  //Upwind
                   FLWw[index] = WComponent1 / 2.0 *                //Lax-Wendroff
                                 (PastGeneric[Get3DIndex(i,j,layer-1)] + PastGeneric[Get3DIndex(i,j,layer)]) +
                                 pow(WComponent1,2.0) * ATimeStep / (2.0 * LowerDepth) *
                                 (PastGeneric[Get3DIndex(i,j,layer-1)] - PastGeneric[Get3DIndex(i,j,layer)]); 
                }
            }
         } 
         FLMMinusOneHalf = 0.0; FLMPlusOneHalf = 0.0; FLMPlusThreeHalves = 0.0;
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
                    WesternConcentration = 0.0; EasternConcentration = 0.0; SouthernConcentration = 0.0; NorthernConcentration = 0.0;
                    FLMMinusOneHalf = 0.0; FLMPlusOneHalf = 0.0; FLMPlusThreeHalves = 0.0;
                    if (
                          (j == GridColumns - 1) ||
                          (
                            (j < GridColumns - 1) && (!BoxArray[Get3DIndex(i,j + 1,layer)].Dummybool2)
                          )
                        )
                    {
                        EasternDepth = BoxDepth[index];
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
                          (j > 0) && (!BoxArray[Get3DIndex(i,j - 1,layer)].Dummybool2)
                       )
                    {
                        WesternDepth = BoxDepth[index];
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
                            (i < GridLines - 1) && (!BoxArray[Get3DIndex(i + 1,j,layer)].Dummybool2)
                          )
                        )
                    {
                        NorthernDepth = BoxDepth[index];
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
                          (i > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer)].Dummybool2)
                       )
                    {
                        SouthernDepth = BoxDepth[index];
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
                       FLMMinusOneHalf = FLWw[Get3DIndex(i,j,layer-1)] - FUPw[Get3DIndex(i,j,layer-1)];
                    }
                    FLMPlusOneHalf = FLWw[Get3DIndex(i,j,layer)] - FUPw[Get3DIndex(i,j,layer)];
                    if ((layer < UpperLayer) && (BoxArray[Get3DIndex(i,j,layer + 1)].Dummybool2))
                    {
                       FLMPlusThreeHalves = FLWw[Get3DIndex(i,j,layer + 1)] - FUPw[Get3DIndex(i,j,layer + 1)];
                    }
                    if (WVelocityPast[index] > 0) S = 1;
                    else S = -1;
                    MyR = ReturnR(S, FLMMinusOneHalf, FLMPlusOneHalf, FLMPlusThreeHalves);
                    TDVw[index] = FUPw[index] + Superbee(MyR) * FLMPlusOneHalf;
                    FLMMinusOneHalf = FLMPlusOneHalf;
                    FLMPlusOneHalf = FLMPlusThreeHalves;
                    if ((layer + 1 < UpperLayer) && (BoxArray[Get3DIndex(i,j,layer + 1)].Dummybool2))
                    {
                       FLMPlusThreeHalves = FLWw[Get3DIndex(i,j,layer + 2)] - FUPw[Get3DIndex(i,j,layer + 2)];
                       if (WVelocityPast[Get3DIndex(i,j,layer + 1)] > 0) S = 1;
                       else S = -1;
                       MyR = ReturnR(S, FLMMinusOneHalf, FLMPlusOneHalf, FLMPlusThreeHalves);
                    }
                    double UpperFlow;
                    if ((layer < UpperLayer) && (BoxArray[Get3DIndex(i,j,layer + 1)].Dummybool2))
                    {
                       TDVw[Get3DIndex(i,j,layer + 1)] = FUPw[Get3DIndex(i,j,layer + 1)]+ Superbee(MyR) * FLMPlusOneHalf;
                       UpperFlow = TDVw[Get3DIndex(i,j,layer + 1)];
                    }
                    else
                       UpperFlow = 0.0;  //Sem esta instru??o h? problemas porque o UpperFlow ? necess?rio na equa??o seguinte mesmo quando se atinge a camada superficial, fora das dimens?es do TVDw
                    double TINNY = 0.0000001;
                    r[cont] = PastGeneric[index] * BoxDepth[index] +
                             (
                               (UComponent1 * WesternDepth * WesternConcentration - UComponent2 * EasternDepth * EasternConcentration) / BoxLength[index] +
                               (VComponent1 * SouthernDepth * SouthernConcentration - VComponent2 * NorthernDepth * NorthernConcentration) / BoxWidth[index] +
                               (TDVw[index] - UpperFlow) +
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

                    /*if (((i == 4) || (i == 5)) && (j == 11) && (layer == 1))
                    {
                        Debugger(1);
                        Debugger(i);
                        Debugger(r[cont]*10000); Debugger(a[cont]*10000); Debugger(b[cont]*10000); Debugger(c[cont]*10000);
                    } */
                    cont++;
                }
            }
         }  //layer
      }     //j
   }        //i
   IntegrationTridag(a, b, c, r, cont);
   cont = 0;
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
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
          if (l > UpperLayer) l = UpperLayer;
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
          if (l > UpperLayer) l = UpperLayer;
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
//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensionalWithBodyDrag2::ReturnR(int s, double FLMMinusOneHalf, double FLMPlusOneHalf, double FLMPlusThreeHalves)
{
   double const TINNY = 0.0000001;
   if (FLMPlusOneHalf < TINNY) return 0.0;
   else if (s == 1) return FLMMinusOneHalf / FLMPlusOneHalf;
   else if (s == -1) return FLMPlusThreeHalves / FLMPlusOneHalf;
   else 
#ifdef __BORLANDC__
      MessageBox(0,"Error in TTriDimensionalWithBodyDrag2::ReturnR","EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
      cerr << "TTriDimensionalWithBodyDrag2::ReturnR - error" << endl;
#endif  // __BORLANDC__
}

double TTriDimensionalWithBodyDrag2::Superbee(double r)
{
   return MAX(0.0,MAX(MIN(2.0 * r, 1.0),MIN(r,2.0)));
}




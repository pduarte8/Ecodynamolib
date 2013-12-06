// Tridimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 30/07/2011
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



TTriDimensionalWithBodyDrag::TTriDimensionalWithBodyDrag(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensionalDynamicSurfaceLayer(PEcoDynClass, className)
{
   Cd = new double[NumberOfBoxes];
   BodyDiameter = new double[NumberOfBoxes];
   BodyHeight = new double[NumberOfBoxes];
   ASum3 = new double[GridColumns * GridLines]; ASum4 = new double[GridColumns * GridLines];
   for (int i = 0; i < GridColumns * GridLines; i++)
   {
       if (i < NumberOfBoxes)
       {
          Cd[i] = 0.0; BodyDiameter[i] = 0.0; BodyHeight[i] = 0.0;
       }
       ASum3[i] = 0.0; ASum4[i] = 0.0;
   }
   double MyValue;
   int X, Y;
   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
   if (PReadWrite != NULL)
   {
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                if (strcmp(MyParameter, "Cd") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(X + 3, i + j, MyValue);
                       Cd[j] = MyValue;
                    }
                    //Debugger(Cd[309]);
                }
                else if (strcmp(MyParameter, "BodyDiameter") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(X + 4, i + j, MyValue);
                       BodyDiameter[j] = MyValue;
                    }
                    //Debugger(BodyDiameter[309]);
                }
                else if (strcmp(MyParameter, "BodyHeight") == 0)
                {
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                       PReadWrite->ReadNumber(X + 5, i + j, MyValue);
                       BodyHeight[j] = MyValue;
                    }
                    //Debugger(BodyHeight[309]);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object Transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TTriDimensionalWithBodyDrag::TTriDimensionalWithBodyDrag - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    //End reading parameters file
}

TTriDimensionalWithBodyDrag::~TTriDimensionalWithBodyDrag()
{
    freeMemory();
    try {
       if (NumberOfBoxes > 0)
       {
         delete [] Cd;
         delete [] BodyDiameter;
         delete [] BodyHeight;
         delete [] ASum3;
         delete [] ASum4;
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
        cerr << "TTriDimensionalWithBodyDrag::~TTriDimensionalWithBodyDrag - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}


void TTriDimensionalWithBodyDrag::EquationOfMotionUImplicit()    //Implicit scheme
{
    int k, index, indexMenosUmJ, indexU;
    k = 0;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    
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
                 indexMenosUmJ = Get3DIndex(i,j - 1,layer);
                 indexU = Get3DIndexForUVelocity(i,j,layer);
                 //Calculating intermediate depths and velocities
                 AUPStreamU = UPStreamUVelocity(i,j,layer);
                 ADOWNStreamU = DOWNStreamUVelocity(i,j,layer);
                 if (
                       (j > 0) &&
                       (
                         (
                            (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                            (DynamicHeight[indexMenosUmJ] < DynamicHeight[index])
                          ) ||
                          (
                            (BoxDepth[indexMenosUmJ] <= CriticalDepthForLandBoundary) &&
                            (DynamicHeight[index] < DynamicHeight[indexMenosUmJ])
                          )
                       )
                    )
                      BoxArray[index].Dummybool3 = false;
                 else
                      BoxArray[index].Dummybool3 = true;
                 if (
                       (j > 0) &&
                       (
                          (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) //No western boundary
                          || (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                        )
                        && (BoxArray[index].Dummybool2) &&
                        (BoxArray[indexMenosUmJ].Dummybool2) &&
                        (
                            (BoxDepth[index] > CriticalDepthForLandBoundary) ||
                            (BoxDepth[indexMenosUmJ] > CriticalDepthForLandBoundary)
                        ) &&
                        (BoxArray[index].Dummybool3) &&
                        (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) && //No northern ocean boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                        (BoxArray[indexMenosUmJ].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2) && //No northern ocean boundary
                        (BoxArray[indexMenosUmJ].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) && //No southern ocean boundary
                        (BoxArray[index].Dummybool1) //No river output cell
                      )
                 {
                    //Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                    if (
                          (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                          || (BoxArray[indexMenosUmJ].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)
                       )
                        rtemporary[index] = UVelocityPast[indexU];
                    else
                    {
                        rtemporary[index] =
                           UVelocityPast[indexU] -
                           UConvectiveTerm(i,j,layer)-
                           WEBarotropicPressureTerm(i,j, j - 1, layer)-
                           WEBaroclinicPressureTerm(i,j, j - 1, layer)+
                           VCoriolis(i,j,layer)+
                           UWEHorizontalDiffusion(i,j,layer)+
                           UNSHorizontalDiffusion(i,j,layer);

                        if (layer == UpperLayer)
                        {
                             WindEffect = WindStress(index,"Wind speed (x component)") * ATimeStep /
                             (
                               0.5 * (WaterDensity[index] + WaterDensity[indexMenosUmJ]) *
                               0.5 * (BoxDepth[index] + BoxDepth[indexMenosUmJ])
                             );
                             rtemporary[index] = rtemporary[index] +  WindEffect;
                         }
                         /*if (((i == 4) || (i == 5)) && (j == 12) && (layer == 0))
                         {
                           Debugger(11);
                           //Debugger(rtemporary[index]);
                           //Debugger(UVelocityPast[indexU]);
                           //Debugger(UConvectiveTerm(i,j,layer));
                           //Debugger(WEBarotropicPressureTerm(i,j, j - 1, layer));
                           //Debugger(WEBaroclinicPressureTerm(i,j, j - 1, layer));
                           //Debugger(UWEHorizontalDiffusion(i,j,layer));
                           //Debugger(UNSHorizontalDiffusion(i,j,layer));
                           //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer)]);
                           Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+1)]);
                           //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+2)]);
                           //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+3)]);
                           //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+4)]);
                         } */
                    }
                    // Cálculo do a, b e c
                    if (
                          (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                          || (BoxArray[indexMenosUmJ].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)
                       )
                    {
                        atemporary[index] = 0.0;
                        btemporary[index] = 1.0;
                        ctemporary[index] = 0.0;
                    }
                    else
                    {
                        atemporary[index] = -UVerticalDiffusion(1, i, j, layer);//aUCalculation(i,j,layer);
                        btemporary[index] = 1.0 + UVerticalDiffusion(2, i, j, layer);//bUCalculation(i,j,layer);
                        if ((layer == 0) || ((layer > 0) && (!BoxArray[Get3DIndex(i,j,layer-1)].Dummybool2)) ||
                           ((layer > 0) && (!BoxArray[Get3DIndex(i,j - 1,layer-1)].Dummybool2)))
                        btemporary[index] = btemporary[index] + (CoefRugosidade(i,j,layer,1) + CoefRugosidade(i,j-1,layer,1)) * ATimeStep /
                                                                 (BoxDepth[Get3DIndex(i,j,layer)] + BoxDepth[Get3DIndex(i,j - 1,layer)]) *
                                                                 UForRugosity(i,j,layer);
                        btemporary[index] = btemporary[index] + (
                                                                  Cd[Get3DIndex(i,j,layer)] * BodyDiameter[Get3DIndex(i,j,layer)] * MIN(BodyHeight[Get3DIndex(i,j,layer)],BoxDepth[Get3DIndex(i,j,layer)])+
                                                                  Cd[Get3DIndex(i,j-1,layer)] * BodyDiameter[Get3DIndex(i,j-1,layer)] * MIN(BodyHeight[Get3DIndex(i,j-1,layer)],BoxDepth[Get3DIndex(i,j-1,layer)])
                                                                 ) * ATimeStep /
                                                                 (2.0*(BoxVolume[Get3DIndex(i,j,layer)] + BoxVolume[Get3DIndex(i,j-1,layer)])) *
                                                                 UForRugosity(i,j,layer);
                        ctemporary[index] = -UVerticalDiffusion(3, i, j, layer);//cUCalculation(i,j,layer);
                    }
                    a[k] = atemporary[index];
                    b[k] = btemporary[index];
                    c[k] = ctemporary[index];
                    r[k] = rtemporary[index];
                    k++;

                 }
                 else
                 {
                    atemporary[index] = 0.0;
                    btemporary[index] = 0.0;
                    ctemporary[index] = 0.0;
                    rtemporary[index] = 0.0;
                 }
              }
           } //layer
       } //j
    } //i
    IntegrationTridag(a, b, c, r, k);
    //Debugger(2);
    //mprovetridag(a, b, c, r, u, k);
    k = 0;
    //for (int layer = 0; layer <= UpperLayer; layer++)
    //{
      //for (int i = 0; i < GridLines; i++)
      for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
      {
         //for (int j = 0; j < GridColumns; j++)
         for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i], GridColumns-1); j++)
         {
          for (int layer = 0; layer <= UpperLayer; layer++)
          {
              index = Get3DIndex(i,j,layer);
              indexMenosUmJ = Get3DIndex(i,j - 1,layer);
              indexU = Get3DIndexForUVelocity(i,j,layer);
              if (
                     (j > 0) &&
                     (
                       (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                       (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                     )
                     &&
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[indexMenosUmJ].Dummybool2) &&
                    (
                      (BoxDepth[index] > CriticalDepthForLandBoundary) ||
                      (BoxDepth[indexMenosUmJ] > CriticalDepthForLandBoundary)
                      /*(BoxArray[index].Dummybool2) ||
                      (BoxArray[indexMenosUmJ].Dummybool2)*/
                    ) &&
                    (BoxArray[index].Dummybool3) &&
                    (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                    (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                    (BoxArray[indexMenosUmJ].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                    (BoxArray[indexMenosUmJ].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                    (BoxArray[index].Dummybool1) //No river output cell
                  )
              {
                 UVelocity[indexU] = u[k];

                 if (fabs(UVelocity[indexU]) < 0.00000000001)
                    UVelocity[indexU] = 0.0;
                 a[k] = 0.0;
                 b[k] = 0.0;
                 c[k] = 0.0;
                 r[k] = 0.0;
                 k++;
              }
          } //layer
       } //j
    } //i
    /*Debugger(1212);
    Debugger(UVelocity[Get3DIndexForUVelocity(4,12,0)]);
    Debugger(UVelocity[Get3DIndexForUVelocity(5,12,0)]);`*/
}

void TTriDimensionalWithBodyDrag::EquationOfMotionVImplicit()    //Implicit scheme
{
  int k, index, indexV;
  k = 0;
  SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
  //for (int j = 0; j < GridColumns; j++)
  for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
  {
     //for (int i = 0; i < GridLines; i++)
     for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j],GridLines-1); i++)
     {
       for (int layer = 0; layer <= UpperLayer; layer++)
       {
          index = Get3DIndex(i,j,layer);
          if (BoxArray[index].Dummybool2)
          {
             indexV = Get3DIndex(i - 1,j,layer);
             AUPStreamV = UPStreamVVelocity(i,j,layer);
             ADOWNStreamV = DOWNStreamVVelocity(i,j,layer);
             //vs  implicit
             if (
                   (i > 0) &&
                   (
                      (
                         (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                         //(!BoxArray[index].Dummybool2) &&
                         (DynamicHeight[indexV] < DynamicHeight[index])
                      ) ||
                      (
                         (BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
                         //(!BoxArray[indexV].Dummybool2) &&
                         (DynamicHeight[index] < DynamicHeight[indexV])
                      )
                   )
                )
                BoxArray[index].Dummybool3 = false;
              else
                BoxArray[index].Dummybool3 = true;
              if (
                   (i > 0) &&
                   (
                      (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                      (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                   ) &&
                   (BoxArray[index].Dummybool2) &&
                   (BoxArray[indexV].Dummybool2) &&
                   (
                      (BoxDepth[index] > CriticalDepthForLandBoundary) ||
                      (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                      /*(BoxArray[index].Dummybool2) ||
                      (BoxArray[indexV].Dummybool2) */
                   ) &&
                   (BoxArray[index].Dummybool3) &&
                   (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                   (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                   (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                   (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                   (BoxArray[index].Dummybool1)
                 )
              {
                 //Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                 if (
                      (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
                      (BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                    )
                    rtemporary[index] = VVelocityPast[Get3DIndexForVVelocity(i,j,layer)];
                 else
                 {
                    rtemporary[index] =
                           VVelocityPast[Get3DIndexForVVelocity(i,j,layer)] -
                           VConvectiveTerm(i,j,layer)-
                           NSBarotropicPressureTerm(i,i-1,j,layer)-
                           NSBaroclinicPressureTerm(i,i-1,j,layer)-
                           UCoriolis(i,j,layer)+
                           VWEHorizontalDiffusion(i,j,layer)+
                           VNSHorizontalDiffusion(i,j,layer);
                    if (layer == UpperLayer)
                    {
                         WindEffect = WindStress(index,"Wind speed (y component)") * ATimeStep /
                         (
                           0.5 * (WaterDensity[index] + WaterDensity[indexV]) *
                           0.5 * (BoxDepth[index] + BoxDepth[indexV])
                         );
                         r[k] = r[k] +  WindEffect;
                     }
                 }
                 // Cálculo do a, b e c
                 if (
                      (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
                      (BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                    )
                 {
                    atemporary[index] = 0.0;
                    btemporary[index] = 1.0;
                    ctemporary[index] = 0.0;
                 }
                 else
                 {
                    atemporary[index] = -VVerticalDiffusion(1, i, j, layer);//aVCalculation(i,j,layer);
                    btemporary[index] = 1.0 + VVerticalDiffusion(2, i, j, layer);//bVCalculation(i,j,layer);
                    if ((layer == 0) || ((layer > 0) && (!BoxArray[Get3DIndex(i,j,layer-1)].Dummybool2)) ||
                        ((layer > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer-1)].Dummybool2)))
                    btemporary[index] = btemporary[index] + (CoefRugosidade(i,j,layer,2) + CoefRugosidade(i-1,j,layer,2)) * ATimeStep /
                                                            (BoxDepth[Get3DIndex(i,j,layer)] + BoxDepth[Get3DIndex(i-1,j,layer)]) *
                                                            VForRugosity(i,j,layer);
                    btemporary[index] = btemporary[index] + (
                                                               Cd[Get3DIndex(i,j,layer)] * BodyDiameter[Get3DIndex(i,j,layer)] * MIN(BodyHeight[Get3DIndex(i,j,layer)],BoxDepth[Get3DIndex(i,j,layer)])+
                                                               Cd[Get3DIndex(i-1,j,layer)] * BodyDiameter[Get3DIndex(i-1,j,layer)] * MIN(BodyHeight[Get3DIndex(i-1,j,layer)],BoxDepth[Get3DIndex(i-1,j,layer)])
                                                             ) * ATimeStep /
                                                             (2.0*(BoxVolume[Get3DIndex(i,j,layer)] + BoxVolume[Get3DIndex(i-1,j,layer)])) *
                                                            VForRugosity(i,j,layer);
                    ctemporary[index] = -VVerticalDiffusion(3, i, j, layer);//cVCalculation(i,j,layer);
                 }
                 /*if ((i == 5) && (j == 1) && (layer == 3))
                 {
                   Debugger(121212);
                   Debugger(VVelocityPast[Get3DIndexForVVelocity(i,j,layer)]);
                   Debugger(rtemporary[index]);
                   Debugger(VConvectiveTerm(i,j,layer));
                   Debugger(NSBarotropicPressureTerm(i,i-1,j,layer));
                   Debugger(NSBaroclinicPressureTerm(i,i-1,j,layer));
                   Debugger(atemporary[index]);
                   Debugger(btemporary[index]);
                   Debugger(ctemporary[index]);
                 }  */
                 a[k] = atemporary[index];
                 b[k] = btemporary[index];
                 c[k] = ctemporary[index];
                 r[k] = rtemporary[index];
                 k++;
             }
             else
             {
                atemporary[index] = 0.0; btemporary[index] = 0.0;
                ctemporary[index] = 0.0; rtemporary[index] = 0.0;
             }
          }
       }  //layer
     } //i
  } //j
  IntegrationTridag(a, b, c, r, k);
  //mprovetridag(a, b, c, r, u, k);

  k = 0;
    //for (int j = 0; j < GridColumns; j++)
    for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
    {
      //for (int i = 0; i < GridLines; i++)
      for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j],GridLines - 1); i++)
      {
        for (int layer = 0; layer <= UpperLayer; layer++)
        {
        index = Get3DIndex(i,j,layer);
        indexV = Get3DIndex(i - 1,j,layer);
        if (
              (i > 0) &&
              (
                 (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                 (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
              ) &&
              (BoxArray[index].Dummybool2) &&
              (BoxArray[indexV].Dummybool2) &&
              (
                 (BoxDepth[index] > CriticalDepthForLandBoundary) ||
                 (BoxDepth[indexV] > CriticalDepthForLandBoundary)
              ) &&
                  (BoxArray[index].Dummybool3) &&
                  (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                  (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&&   //No eastern ocean boundary
                  (BoxArray[index].Dummybool1)
              )
        {
           VVelocity[Get3DIndexForVVelocity(i,j,layer)] = u[k];
           if (fabs(VVelocity[Get3DIndexForVVelocity(i,j,layer)]) < 0.00000000001)
           VVelocity[Get3DIndexForVVelocity(i,j,layer)] = 0.0;
           a[k] = 0.0;
           b[k] = 0.0;
           c[k] = 0.0;
           r[k] = 0.0;
           k++;
        }
      }
    }
  }
  //Debugger(111);
  //Debugger(VVelocity[Get3DIndexForVVelocity(5,11,0)]);
  //Debugger(VVelocity[Get3DIndexForVVelocity(7,13,0)]*1000000.0); */
}

void TTriDimensionalWithBodyDrag::EquationOfMotionUExplicit()    //Explicit scheme
{
  int index, indexMenosUmJ, indexU;
  SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
  //for (int i = 0; i < GridLines; i++)
  for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
  {
     //Debugger(i);
     for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
     {
        for (int layer = 0; layer <= UpperLayer; layer++)
        {
          index = Get3DIndex(i,j,layer);
          if (BoxArray[index].Dummybool2)
          {
            indexMenosUmJ = Get3DIndex(i,j - 1,layer);
            indexU = Get3DIndexForUVelocity(i,j,layer);
            AUPStreamU = UPStreamUVelocity(i,j, layer);
            ADOWNStreamU = DOWNStreamUVelocity(i,j, layer);
            if (
                  (j > 0) &&
                  (
                    (
                      (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                      (PastHeight[indexMenosUmJ] < PastHeight[index])
                     ) ||
                     (
                       (BoxDepth[indexMenosUmJ] <= CriticalDepthForLandBoundary) &&
                       (PastHeight[index] < PastHeight[indexMenosUmJ])
                     )
                   )
                )
              BoxArray[index].Dummybool3 = false;
            else
              BoxArray[index].Dummybool3 = true;
            if (
                  (j > 0) &&
                  (
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  )  &&
                  (BoxArray[index].Dummybool2) &&
                  (BoxArray[indexMenosUmJ].Dummybool2) &&
                 (
                     (BoxDepth[index] > CriticalDepthForLandBoundary) ||
                     (BoxDepth[indexMenosUmJ] > CriticalDepthForLandBoundary)
                 ) &&
                 (BoxArray[index].Dummybool3) &&
                 (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                 (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                 (BoxArray[indexMenosUmJ].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                 (BoxArray[indexMenosUmJ].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&   //No southern ocean boundary
                 (BoxArray[index].Dummybool1) //No river output cell
               )
            {
               /*if (
                    (!BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                    && (!BoxArray[indexMenosUmJ].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)
                   ) */
                if (
                    (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 1)
                    && (BoxArray[indexMenosUmJ].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 1)
                   )
               {
                 UVelocity[indexU] = UVelocityPast[indexU] -
                                     UConvectiveTerm(i,j,layer)-
                                     WEBarotropicPressureTerm(i,j, j - 1, layer)-
                                     WEBaroclinicPressureTerm(i,j, j - 1, layer)+
                                     VCoriolis(i,j,layer)+
                                     UWEHorizontalDiffusion(i,j,layer)+
                                     UNSHorizontalDiffusion(i,j,layer)+
                                     UVerticalDiffusionExplicit(i,j,layer)-
                                     BodyDragOfUEquation(i,j,layer) * UVelocityPast[indexU];
                 /*if (((i == 4) || (i == 5)) && (j == 12) && (layer == 0))
                 {
                   Debugger(22);
                   //Debugger(UVelocityPast[indexU]);
                   //Debugger(UConvectiveTerm(i,j,layer));
                   //Debugger(WEBarotropicPressureTerm(i,j, j - 1, layer));
                   //Debugger(WEBaroclinicPressureTerm(i,j, j - 1, layer));
                   //Debugger(WaterDensity[Get3DIndex(i,j,layer)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j,layer+1)]);
                   Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+1)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j,layer+2)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+2)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j,layer+3)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+3)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j,layer+4)]);
                   //Debugger(WaterDensity[Get3DIndex(i,j - 1,layer+4)]);
                   //Debugger(UVerticalDiffusionExplicit(i,j,layer));
                   //Debugger(BodyDragOfUEquation(i,j,layer));
                 } */

                 if (layer == UpperLayer)
                 {
                   WindEffect = WindStress(index,"Wind speed (x component)") * ATimeStep /
                               (
                                 0.5 * (WaterDensity[index] + WaterDensity[indexMenosUmJ]) *
                                 0.5 * (BoxDepth[index] + BoxDepth[indexMenosUmJ])
                               );
                   UVelocity[indexU] = UVelocity[indexU] +  WindEffect;
                 }
                 if ((layer == 0) || ((layer > 0) && (!BoxArray[Get3DIndex(i,j,layer-1)].Dummybool2)) ||
                    ((layer > 0) && (!BoxArray[Get3DIndex(i,j - 1,layer-1)].Dummybool2)))
                      UVelocity[indexU] = UVelocity[indexU] - DragOfUEquation(i, j, layer) * UVelocityPast[indexU];
               }
            }
          }
        } //layer
     } //j
  } //i
  /*Debugger(1111);
  Debugger(UVelocity[Get3DIndexForUVelocity(4,12,0)]);
  Debugger(UVelocity[Get3DIndexForUVelocity(5,12,0)]); */
}

void TTriDimensionalWithBodyDrag::EquationOfMotionVExplicit()    //Explicit scheme
{
  int index, indexU, indexV;
  SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
  //for (int i = 0; i < GridLines; i++)
  for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
  {
     //for (int j = 0; j < GridColumns; j++)
     for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
     {
        for (int layer = 0; layer <= UpperLayer; layer++)
        {
          /*index = i * GridColumns + j;
          indexV = (i - 1) * GridColumns + j;*/
          index = Get3DIndex(i,j,layer);
          if (BoxArray[index].Dummybool2)
          {

            indexV = Get3DIndex(i - 1,j,layer);
            AUPStreamV = UPStreamVVelocity(i,j,layer);
            ADOWNStreamV = DOWNStreamVVelocity(i,j,layer);


            if (
                 (i > 0) &&
                 (
                   (
                     (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                     //(!BoxArray[index].Dummybool2) &&
                     (PastHeight[indexV] < PastHeight[index])
                    ) ||
                    (
                      (BoxDepth[indexV] <= CriticalDepthForLandBoundary) &&
                      //(!BoxArray[indexV].Dummybool2) &&
                      (PastHeight[index] < PastHeight[indexV])
                    )
                 )
               )
                BoxArray[index].Dummybool3 = false;
            else
                BoxArray[index].Dummybool3 = true;
            if (
                 (i > 0) &&
                 (
                   (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                   (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                 ) &&
                 (BoxArray[index].Dummybool2) &&
                 (BoxArray[indexV].Dummybool2) &&
                 (
                   (BoxDepth[index] > CriticalDepthForLandBoundary) ||
                   (BoxDepth[indexV] > CriticalDepthForLandBoundary)
                   /*(BoxArray[index].Dummybool2) ||
                   (BoxArray[indexV].Dummybool2) */
                 )&&
                 (BoxArray[index].Dummybool3) &&
                 (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                 (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                 (BoxArray[indexV].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                 (BoxArray[indexV].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                 (BoxArray[index].Dummybool1) //No river output cell
               ) //De notar que este conjunto de condições lógicas não impede que seja calculada a velocidade
              //explícita para uma célula que seja fronteira fluvial. No entanto, no final do passo de cálculo é imposta
              //nessa célula a velocidade calculada a partir do caudal fluvial. Poderiam haver entretanto problemas no
              //cálculo da continuidade e do transporte, mas não se passa nada de errado porque os referidos cálculos se baseiam
              //nos termos explícitos passados (actualizados no final d cada passode cálculo. Os implicitos são os presentes, mas quanto a esses
              //não há problemas porque o a=0, o b=1 e o c=0 nas fornteiras fluviais pelo que o cálculo implicito da velocidade não
              //muda o seu valor nas fronteiras fluviais.
            {
                /*if (
                      (!BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) &&
                      (!BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                    ) */

                if (
                      (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 1) &&
                      (BoxArray[indexV].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 1)
                    )
                {
                    VVelocity[Get3DIndexForVVelocity(i,j,layer)] = VVelocityPast[Get3DIndexForVVelocity(i,j,layer)]-
                                       VConvectiveTerm(i,j,layer)-
                                       NSBarotropicPressureTerm(i, i - 1, j,layer)-
                                       NSBaroclinicPressureTerm(i, i - 1, j,layer)-
                                       UCoriolis(i,j,layer)+
                                       VWEHorizontalDiffusion(i,j,layer)+
                                       VNSHorizontalDiffusion(i,j,layer)+
                                       VVerticalDiffusionExplicit(i,j,layer)-
                                       BodyDragOfVEquation(i,j,layer) * VVelocityPast[Get3DIndexForVVelocity(i,j,layer)];


                   if (layer == UpperLayer)
                   {
                     WindEffect = WindStress(index,"Wind speed (y component)") * ATimeStep /
                                 (
                                   0.5 * (WaterDensity[index] + WaterDensity[indexV]) *
                                   0.5 * (BoxDepth[index] + BoxDepth[indexV])
                                 );
                     VVelocity[Get3DIndexForVVelocity(i,j,layer)] = VVelocity[Get3DIndexForVVelocity(i,j,layer)] +  WindEffect;
                   }
                   /*if ((i == 5) && (j == 1) && (layer == 3))
                   {
                     Debugger(222222);
                     Debugger(MyPEcoDynClass->GetCurrTime());
                     Debugger(VVelocityPast[Get3DIndexForVVelocity(i,j,layer)]);
                     Debugger(VConvectiveTerm(i,j,layer));
                     Debugger(NSBarotropicPressureTerm(i,i-1,j,layer));
                     Debugger(NSBaroclinicPressureTerm(i,i-1,j,layer));
                     Debugger(VVerticalDiffusionExplicit(i,j,layer));
                   } */
                   if ((layer == 0) || ((layer > 0) && (!BoxArray[Get3DIndex(i,j,layer-1)].Dummybool2)) ||
                        ((layer > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer-1)].Dummybool2)))
                        VVelocity[Get3DIndexForVVelocity(i,j,layer)] = VVelocity[Get3DIndexForVVelocity(i,j,layer)] - DragOfVEquation(i, j, layer) * VVelocityPast[Get3DIndexForVVelocity(i,j,layer)];

               }
            }
          }
        }
     }
  }
  //Debugger(333);
  //Debugger(VVelocity[Get3DIndexForVVelocity(5,11,0)]);
  //Debugger(VVelocity[Get3DIndexForVVelocity(7,11,0)]);
}

void TTriDimensionalWithBodyDrag::Continuity()
{
   int BottomBox, Cont;
   int jj, ii;
   PartialSumInitialization();
   double AProduct;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   Cont = 0;
   if (SemiTimeStep == 1)
   {
      for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
      {
         //jj = 0;
         jj = pSubDomain->IColumn[i];
         do
         {
            for (int j = jj; j < jj + 2; j++)
            {
                BottomBox = 0;
                for (int k = 0; k <= UpperLayer; k++)  //Cálculo de somas de produtos por colunas
                {
                   if (
                         (j < GridColumns) && (!BoxArray[Get3DIndex(i,j,k)].Dummybool2) && (k < UpperLayer)
                         ||((j == GridColumns) && (!BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2) && (k < UpperLayer))
                      )
                      BottomBox++;
                   else
                   {
                      if (
                            (j < GridColumns) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2)  ||
                            (j == GridColumns) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)
                         )
                      {
                          //Somatórios para cálculo dos termos implícitos na direcção U
                         //Este termo é zero no caso da célula montante não existir ou ser terra
                         //porque não há gradiente de pressão e este termo resulta do referido gradiente
                         if (j < GridColumns) //Estes termos são tantos quantos as GridColumns
                         {
                            if  ((j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2))/*&& (!CheckForSeaBoundary(i,j-1,k))*/
                            {
                                Soma1[Get2DIndex(i,j)] = Soma1[Get2DIndex(i,j)] + SumOfAjacentDepthsInU(i,j,j-1,k);
                            }
                            //Este termo é zero no caso da célula juzante não existir ou ser terra
                            //porque não há gradiente de pressão e este termo resulta do referido gradiente
                            if ((j < GridColumns - 1) && (BoxArray[Get3DIndex(i,j+1,k)].Dummybool2))/*&& (!CheckForSeaBoundary(i,j+1,k))*/
                            {
                                Soma2[Get2DIndex(i,j)] = Soma2[Get2DIndex(i,j)] + SumOfAjacentDepthsInU(i,j + 1,j,k);
                            }
                         }
                         //Somatórios para cálculo dos termos explícitos - Soma3: termo relativo a caudais nos limites montante e juzante
                         if (
                              ((j < GridColumns)&& (!BoxArray[Get3DIndex(i,j,k)].Dummybool1))
                              || ((j > 0) && (!BoxArray[Get3DIndex(i,j-1,k)].Dummybool1)) //River boundary
                            )   //Limite montante
                         {
                            //Debugger(1); Debugger(i); Debugger(j); Debugger(k);
                            Soma3[Get2DIndex(i,j, GridColumns+1)] = Soma3[Get2DIndex(i,j, GridColumns+1)] +  UBoundaryFlow(i,j,j,k);
                         }
                         else
                         {
                            if ((j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (j < GridColumns) && (BoxArray[Get3DIndex(i,j, k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 1))
                               Soma0[Get2DIndex(i,j, GridColumns+1)] = Soma0[Get2DIndex(i,j, GridColumns+1)] + UFlow(i,j,j-1,k);

                            UConvectiveTerm(i,j,k);

                            if ((j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (j < GridColumns))
                            {
                               Soma4[Get2DIndex(i,j, GridColumns+1)] = Soma4[Get2DIndex(i,j, GridColumns+1)] +
                                       (
                                          (UU2[Get3DIndexForUVelocity(i,j,k)] - UU1[Get3DIndexForUVelocity(i,j,k)] +
                                           UU4[Get3DIndexForUVelocity(i,j,k)] - UU3[Get3DIndexForUVelocity(i,j,k)]
                                           + UVU[Get3DIndexForUVelocity(i,j,k)]) /
                                          (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)])
                                          //+ UVU[Get3DIndexForUVelocity(i,j,k)]
                                       ) * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)])
                                       + (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)])
                                       * UW[Get3DIndexForUVelocity(i,j,k)] /
                                       (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);

                                //Esta multiplicação destina-se a anular a divisão pelo termo multiplicado
                                //efectuada no void ConvectiveTerm(i,j). Acontece que na equação dos Us
                                //a referida divisão é necessária, mas não na equação da continuidade.
                                //Deste modo, pode usar-se o mesmo void para resolver a equação do momento
                                // e a da continuidade tendo só o cuidado de efectuar esta correção

                               Soma5[Get2DIndex(i,j, GridColumns+1)] = Soma5[Get2DIndex(i,j, GridColumns+1)] +
                                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                                 WEBaroclinicPressureTerm(i,j,j-1,k) /
                                 (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);

                               Soma6[Get2DIndex(i,j, GridColumns+1)] = Soma6[Get2DIndex(i,j, GridColumns+1)] +
                                    (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                                    UWEHorizontalDiffusion(i,j,k) /
                                    (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);

                               Soma6[Get2DIndex(i,j, GridColumns+1)] = Soma6[Get2DIndex(i,j, GridColumns+1)] +
                                    (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                                    UNSHorizontalDiffusion(i,j,k) /
                                    (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);

                               Soma11[Get2DIndex(i,j)] = Soma11[Get2DIndex(i,j)] + (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                                                                                    VCoriolis(i,j,k) / (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);
                            }
                            if (                     //VERIFICAR!!!!
                                 (j > 0) && (k > 0) && (j < GridColumns) &&
                                 (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2)
                                  && (!BoxArray[Get3DIndex(i,j - 1,k - 1)].Dummybool2) &&
                                 (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2)
                                 )
                            {
                                 AProduct = DragOfUEquation(i,j,k) *
                                        (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]);
                                 Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)]
                                    + (
                                         AProduct * ATimeStep *
                                         UImplicitForDrag(i,j,k)
                                      )/ (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);
                                 ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfUEquation(i,j,k) + BodyDragOfUEquation(i,j,k));
                            }
                            if (                                      // //VERIFICAR!!!! Body drag contribution
                                 (j > 0) && (j < GridColumns) &&
                                 (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2)
                               )
                            {
                                 AProduct = BodyDragOfUEquation(i,j,k) *
                                        //(BoxVolume[Get3DIndex(i,j,k)] + BoxVolume[Get3DIndex(i,j-1,k)]) *
                                        (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]);
                                 Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)]
                                    + (
                                         AProduct * ATimeStep *
                                         UImplicitForDrag(i,j,k)
                                      )/ (2.0 * MeanColumnLength[Get2DIndex(i,j)]);
                                 ASum3[Get2DIndex(i,j)] = ASum3[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfUEquation(i,j,k) + BodyDragOfUEquation(i,j,k));
                            }
                            if (                 //VERIFICAR!!!!
                                 (j < GridColumns - 1) && (k > 0) && (BoxArray[Get3DIndex(i,j+1,k)].Dummybool2)
                                 && (!BoxArray[Get3DIndex(i,j+1,k - 1)].Dummybool2) &&
                                 (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2))
                             {
                                 AProduct = DragOfUEquation(i,j + 1,k) *
                                        (BoxDepth[Get3DIndex(i,j+1,k)] + BoxDepth[Get3DIndex(i,j,k)]);
                                 ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfUEquation(i,j+1,k) + BodyDragOfUEquation(i,j+1,k));

                             }
                             if (      //VERIFICAR!!!!
                                 (j < GridColumns - 1) && (BoxArray[Get3DIndex(i,j+1,k)].Dummybool2)
                                )
                             {
                                 AProduct = BodyDragOfUEquation(i,j + 1,k) *
                                        //(BoxVolume[Get3DIndex(i,j+1,k)] + BoxVolume[Get3DIndex(i,j,k)])*
                                        (BoxDepth[Get3DIndex(i,j+1,k)] + BoxDepth[Get3DIndex(i,j,k)]);
                                 ASum4[Get2DIndex(i,j)] = ASum4[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfUEquation(i,j+1,k) + BodyDragOfUEquation(i,j+1,k));
                             }
                         }
                         if ((j < GridColumns)&& (BoxArray[Get3DIndex(i,j,k)].Dummybool1) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
                         {
                            Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] + (FlowDischarge[Get3DIndex(i,j,k)] - FlowUptake[Get3DIndex(i,j,k)]);
                            Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] + VContributionToContinuity(i, j, k);
                         }
                      }
                   }
                }  //k
                if (    //VERIFICAR!!!
                     (j > 0) && (j < GridColumns)
                     && (BoxArray[Get3DIndex(i,j - 1,BottomBox)].Dummybool2)
                     && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool2)
                     && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool1)
                     && (BoxArray[Get3DIndex(i,j-1,BottomBox)].Dummybool1)
                  )
                {
                   AProduct = DragOfUEquation(i,j,BottomBox) *
                             (BoxDepth[Get3DIndex(i,j,BottomBox)] + BoxDepth[Get3DIndex(i,j-1,BottomBox)]);
                   Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)]
                                    + (
                                         AProduct * ATimeStep *
                                         UImplicitForDrag(i,j,BottomBox)
                                      ) /(/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);
                   ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                           (1.0 + DragOfUEquation(i,j,BottomBox) + BodyDragOfUEquation(i,j,BottomBox));

                }

                if (  //VERIFICAR!!!
                  (j < GridColumns - 1)
                  && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool2)
                  && (BoxArray[Get3DIndex(i,j+1,BottomBox)].Dummybool2)
                  && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool1)
                  && (BoxArray[Get3DIndex(i,j+1,BottomBox)].Dummybool1)
                )
                {
                  ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + DragOfUEquation(i,j+1,BottomBox) / ATimeStep *
                         (BoxDepth[Get3DIndex(i,j+1,BottomBox)] + BoxDepth[Get3DIndex(i,j,BottomBox)]) /
                         (1.0 + DragOfUEquation(i,j+1,BottomBox) + BodyDragOfUEquation(i,j+1,BottomBox));
                }


                // Somas para termos explícitos
                if (j < GridColumns)
                {
                   Soma0[Get2DIndex(i,j, GridColumns+1)] = Soma0[Get2DIndex(i,j, GridColumns+1)] * ATimeStep / (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]);
                   Soma3[Get2DIndex(i,j, GridColumns+1)] = Soma3[Get2DIndex(i,j, GridColumns+1)] * ATimeStep / (2.0 * MeanColumnLength[Get2DIndex(i,j)] /** MeanColumnWidth[Get2DIndex(i,j)]*/);
                   Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] * ATimeStep /(MeanColumnWidth[Get2DIndex(i,j)] * MeanColumnLength[Get2DIndex(i,j)]);
                   Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] * ATimeStep;
                   Soma11[Get2DIndex(i,j)] = Soma11[Get2DIndex(i,j)] * ATimeStep;
                }
                else
                   Soma3[Get2DIndex(i,j, GridColumns+1)] = Soma3[Get2DIndex(i,j, GridColumns+1)] * ATimeStep / (2.0 * MeanColumnLength[Get2DIndex(i,j-1)] /** MeanColumnWidth[Get2DIndex(i,j-1)]*/);

                Soma4[Get2DIndex(i,j, GridColumns+1)] = Soma4[Get2DIndex(i,j, GridColumns+1)] * ATimeStep;
                Soma5[Get2DIndex(i,j, GridColumns+1)] = Soma5[Get2DIndex(i,j, GridColumns+1)] * ATimeStep;
                Soma6[Get2DIndex(i,j, GridColumns+1)] = Soma6[Get2DIndex(i,j, GridColumns+1)] * ATimeStep;


                if ((j > 0) && (j < GridColumns) && (BoxArray[Get3DIndex(i,j,UpperLayer)].Dummybool2) && (BoxArray[Get3DIndex(i,j-1,UpperLayer)].Dummybool2)
                                 && (BoxArray[Get3DIndex(i,j,UpperLayer)].Dummybool1)
                                 && (BoxArray[Get3DIndex(i,j-1,UpperLayer)].Dummybool1))
                {
                   Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)] - WindStress(Get3DIndex(i,j,UpperLayer),"Wind speed (x component)") *
                                   pow(ATimeStep,2) /
                                   (
                                      0.5 * (WaterDensity[Get3DIndex(i,j,UpperLayer)] + WaterDensity[Get3DIndex(i,j-1,UpperLayer)]) *
                                      /*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)]
                                   );
                }
            } //j
            jj = jj + 2;
         }  while (jj <= MIN(pSubDomain->FColumn[i], GridColumns-1));//while (jj < GridColumns);
      } //i
      //for (int i = 0; i < GridLines; i++)
      for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
      {
         //for (int j = 0; j < GridColumns; j++)
         for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
         {
            if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j)/*(!CheckForRiverBoundariesInColumns(i,j))*/))
            {
                //Termos implícitos da equação da continuidade para cálculo da elevação
                if ((j > 0) && (!CheckForRivers(i,j-1)))
                {
                  atemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep * ATimeStep *
                      (-Soma1[Get2DIndex(i,j)] +  ATimeStep * ASum1[Get2DIndex(i,j)] + ATimeStep * ASum3[Get2DIndex(i,j)]) /
                      (2.0 * MeanColumnLength[Get2DIndex(i,j)] * 0.5 * (MeanColumnLength[Get2DIndex(i,j-1)] + MeanColumnLength[Get2DIndex(i,j)]));
                }
                else
                  atemporaryCont[Get2DIndex(i,j)] = 0.0;
                btemporaryCont[Get2DIndex(i,j)] = - atemporaryCont[Get2DIndex(i,j)];

                if ((j < GridColumns - 1) && (!CheckForRivers(i,j+1)))
                {
                  ctemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep * ATimeStep *
                      (-Soma2[Get2DIndex(i,j)] + ATimeStep * ASum2[Get2DIndex(i,j)] + ATimeStep * ASum4[Get2DIndex(i,j)]) /
                      (2.0 * MeanColumnLength[Get2DIndex(i,j)] * 0.5 * (MeanColumnLength[Get2DIndex(i,j)] + MeanColumnLength[Get2DIndex(i,j+1)]));
                }
                else
                  ctemporaryCont[Get2DIndex(i,j)] = 0.0;
                btemporaryCont[Get2DIndex(i,j)] = btemporaryCont[Get2DIndex(i,j)] - ctemporaryCont[Get2DIndex(i,j)];
                btemporaryCont[Get2DIndex(i,j)] = 1.0 + btemporaryCont[Get2DIndex(i,j)];
                //if ((ASum1 != 0.0) || (ASum2 != 0.0)) Debugger(j);
                //Termo explícito
                double LengthCorrection;
                if (j < GridColumns - 1)
                   LengthCorrection = MeanColumnLength[Get2DIndex(i,j)] / MeanColumnLength[Get2DIndex(i,j + 1)];
                else
                   LengthCorrection = 1.0;
                rtemporaryCont[Get2DIndex(i,j)] = (PastHeight[Get3DIndex(i,j,UpperLayer)] +
                                                   Soma0[Get2DIndex(i,j,GridColumns+1)] + Soma3[Get2DIndex(i,j, GridColumns+1)] - Soma4[Get2DIndex(i,j, GridColumns+1)] - Soma5[Get2DIndex(i,j, GridColumns+1)] +
                                                   Soma6[Get2DIndex(i,j, GridColumns+1)] - Soma8[Get2DIndex(i,j, GridColumns+1)] + Soma10[Get2DIndex(i,j)]+ Soma11[Get2DIndex(i,j)]) -
      	                                          (
                                                     Soma0[Get2DIndex(i,j+1,GridColumns+1)] + Soma3[Get2DIndex(i,j+1, GridColumns+1)] - Soma4[Get2DIndex(i,j+1, GridColumns+1)] -
                                                     Soma5[Get2DIndex(i,j+1, GridColumns+1)] + Soma6[Get2DIndex(i,j+1, GridColumns+1)]- Soma8[Get2DIndex(i,j+1, GridColumns+1)]
                                                  ) * LengthCorrection
                                                  + Soma9[Get2DIndex(i,j)];

                if (j < GridColumns - 1)
                   rtemporaryCont[Get2DIndex(i,j)] = rtemporaryCont[Get2DIndex(i,j)] - Soma11[Get2DIndex(i,j + 1)];
                if ((j > 0) && (CheckForSeaBoundariesInColumns(i,j - 1))){
                   rtemporaryCont[Get2DIndex(i,j)] = rtemporaryCont[Get2DIndex(i,j)]
                          -DynamicHeight[Get3DIndex(i,j - 1,UpperLayer)] *
                          atemporaryCont[Get2DIndex(i,j)];
                   atemporaryCont[Get2DIndex(i,j)] = 0.0;
                }
                if ((j < GridColumns - 1) && (CheckForSeaBoundariesInColumns(i,j + 1))){
                   rtemporaryCont[Get2DIndex(i,j)] = rtemporaryCont[Get2DIndex(i,j)]
                          -DynamicHeight[Get3DIndex(i,j + 1,UpperLayer)] *
                          ctemporaryCont[Get2DIndex(i,j)];
                   ctemporaryCont[Get2DIndex(i,j)] = 0.0;
                }
                u[Cont] = 0.0;
                //Indexação dos termos implícitos e do termo explícito
                a[Cont] = atemporaryCont[Get2DIndex(i,j)];
                b[Cont] = btemporaryCont[Get2DIndex(i,j)];
                c[Cont] = ctemporaryCont[Get2DIndex(i,j)];
                r[Cont] = rtemporaryCont[Get2DIndex(i,j)];

                Cont++;
            }
         }
      }
      IntegrationTridag(a, b, c, r, Cont);
      //mprovetridag(a, b, c, r, u, Cont);
      int kk = 0;
      for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
      {
          for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
          {
            if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j))){
               DynamicHeight[Get3DIndex(i,j, UpperLayer)] = u[kk];
               //if ((i == 97) && (j == 47)) Debugger(DynamicHeight[Get3DIndex(i,j, UpperLayer)]);
               a[kk] = 0.0;
               b[kk] = 0.0;
               c[kk] = 0.0;
               r[kk] = 0.0;
               kk++;
            }
          }
      }
   } //Semi time step 1

   if (SemiTimeStep == 2)
   {
      for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
      {
         ii = pSubDomain->ILine[j];
         do
         {
            for (int i = ii; i < ii + 2; i++)
            {
                BottomBox = 0;
                for (int k = 0; k <= UpperLayer; k++)  //Cálculo de somas de produtos por colunas
                {
                   if   (
                          (i < GridLines) && (!BoxArray[Get3DIndex(i,j,k)].Dummybool2)  && (k < UpperLayer) ||
                          ((i == GridLines) && (!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (k < UpperLayer))
                        )
                      BottomBox++;
                   else
                   {
                      if (
                            (i < GridLines) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2)  ||
                            (i == GridLines) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)
                         )
                      {
                              //Somatórios para cálculo dos termos implícitos na direcção V
                              //Este termo é zero no caso da célula montante não existir ou ser terra
                             //porque não há gradiente de pressão e este termo resulta do referido gradiente
                          if (i < GridLines)  //Estes termos são tantos quantos as GridLines
                          {
                             if  ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2))/*&& (!CheckForSeaBoundary(i-1,j,k))*/
                                Soma1[Get2DIndex(i,j)] = Soma1[Get2DIndex(i,j)] + SumOfAjacentDepthsInV(i,i-1,j,k);
                             //Este termo é zero no caso da célula juzante não existir ou ser terra
                             //porque não há gradiente de pressão e este termo resulta do referido gradiente
                             if ((i < GridLines - 1) && (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2))/*&& (!CheckForSeaBoundary(i+1,j,k))*/
                                Soma2[Get2DIndex(i,j)] = Soma2[Get2DIndex(i,j)] + SumOfAjacentDepthsInV(i+1,i,j,k);
                          }
                          //Somatórios para cálculo dos termos explícitos - Soma3: termo relativo a caudais nos limites montante e juzante
                          if (
                               ((i < GridLines) && (!BoxArray[Get3DIndex(i,j,k)].Dummybool1)) ||
                               ((i > 0) && (!BoxArray[Get3DIndex(i-1,j,k)].Dummybool1))//River boundary
                             )
                          {
                             Soma3[Get2DIndex(i,j, GridColumns+1)] = Soma3[Get2DIndex(i,j, GridColumns+1)] +  VBoundaryFlow(i,i,j,k);
                          }
                          else
                          {
                             if ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (i < GridLines)&&
                               (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 1))
                                Soma0[Get2DIndex(i,j,GridColumns+1)] = Soma0[Get2DIndex(i,j,GridColumns+1)] + VFlow(i,i-1,j,k);
                             VConvectiveTerm(i,j,k);
                             //if ((i == 107) && (j == 121) && (k == 4)) Debugger(2);
                             if ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (i < GridLines))
                             {
                                Soma4[Get2DIndex(i,j, GridColumns+1)] = Soma4[Get2DIndex(i,j, GridColumns+1)] +
                                   (
                                      (VV2[Get3DIndexForVVelocity(i,j,k)] - VV1[Get3DIndexForVVelocity(i,j,k)] +
                                       VV4[Get3DIndexForVVelocity(i,j,k)] - VV3[Get3DIndexForVVelocity(i,j,k)] +
                                       UVV[Get3DIndexForVVelocity(i,j,k)]) /
                                       (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)])
                                   )
                                   * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])
                                   + (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])
                                   * VW[Get3DIndexForVVelocity(i,j,k)] /
                                   (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);

                                   //Esta multiplicação destina-se a anular a divisão pelo termo multiplicado
                                   //efectuada no void ConvectiveTerm(i,j). Acontece que na equação dos Us
                                   //a referida divisão é necessária, mas não na equação da continuidade.
                                   //Deste modo, pode usar-se o mesmo void para resolver a equação do momento
                                   // e a da continuidade tendo só o cuidado de efectuar esta correção

                                //if (((i == 4) ||(i == 5)) && (j == 4) && (MyPEcoDynClass->GetCurrTime()> 0.0006))

                                /*if ((i == 5) && (j == 1))
                                {
                                  Debugger(k);
                                  Debugger(VV1[Get3DIndexForVVelocity(i,j,k)]); Debugger(VV2[Get3DIndexForVVelocity(i,j,k)]);
                                  Debugger(VV3[Get3DIndexForVVelocity(i,j,k)]); Debugger(VV4[Get3DIndexForVVelocity(i,j,k)]);
                                  Debugger(UVV[Get3DIndexForVVelocity(i,j,k)]);
                                } */

                                Soma5[Get2DIndex(i,j, GridColumns+1)] = Soma5[Get2DIndex(i,j, GridColumns+1)] +
                                (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                                NSBaroclinicPressureTerm(i,i-1,j,k) /
                                (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);
                                //if (((i == 4) || (i == 5) || (i == 6)) && (j == 11))
                                /*if (((i == 4) || (i == 5) || (i == 6)) && (j == 1) && (MyPEcoDynClass->GetCurrTime()>0.0008))
                                {
                                   Debugger(344);
                                   Debugger(i); Debugger(k);
                                   //Debugger(UpperLayer);
                                   Debugger(WaterDensity[Get3DIndex(i, j, k)]);
                                   Debugger(NSBaroclinicPressureTerm(i,i-1,j,k));
                                   Debugger(Soma5[Get2DIndex(i,j, GridColumns+1)]);
                                   Debugger(BoxDepth[Get3DIndex(i,j,k)]);
                                   Debugger(MeanColumnWidth[Get2DIndex(i,j)]);
                                } */
                                Soma6[Get2DIndex(i,j, GridColumns+1)] = Soma6[Get2DIndex(i,j, GridColumns+1)] +
                                    (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                                    VNSHorizontalDiffusion(i,j,k) /
                                    (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);

                                Soma6[Get2DIndex(i,j, GridColumns+1)] = Soma6[Get2DIndex(i,j, GridColumns+1)] +
                                    (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                                    VWEHorizontalDiffusion(i,j,k) /
                                    (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);

                                Soma11[Get2DIndex(i,j)] = Soma11[Get2DIndex(i,j)] - (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])
                                                                                    *UCoriolis(i,j,k) / (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);

                             }
                             if (   //VERIFICAR!!!
                                 (i > 0) && (k > 0) && (i < GridLines) &&
                                 (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)
                                 && (!BoxArray[Get3DIndex(i-1,j,k - 1)].Dummybool2) &&
                                 (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                                 (BoxLength[Get3DIndex(i,j,k)] > 0.0)
                                 )
                             {
                                 AProduct = DragOfVEquation(i,j,k) *
                                        (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]);
                                 Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)]
                                    + (
                                         AProduct * ATimeStep *
                                         VImplicitForDrag(i,j,k)
                                      )/ (2.0 * MeanColumnWidth[Get2DIndex(i,j)]);
                                 ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfVEquation(i,j,k) + BodyDragOfVEquation(i,j,k));
                             }
                             //Body drag contribution
                             if (       //VERIFICAR!!!
                                   (i > 0) && (i < GridLines) &&
                                   (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)
                                )
                             {
                                 AProduct = BodyDragOfVEquation(i,j,k) *
                                        //(BoxVolume[Get3DIndex(i,j,k)] + BoxVolume[Get3DIndex(i-1,j,k)]) *
                                        (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]);
                                 Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)]
                                    + (
                                         AProduct * ATimeStep *
                                         VImplicitForDrag(i,j,k)
                                      )/ (2.0 * MeanColumnWidth[Get2DIndex(i,j)]);
                                 ASum3[Get2DIndex(i,j)] = ASum3[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfVEquation(i,j,k) + BodyDragOfVEquation(i,j,k));
                             }
                             if (       //VERIFICAR!!!
                                 (i < GridLines - 1) && (k > 0) && (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2)
                                 && (!BoxArray[Get3DIndex(i+1,j,k - 1)].Dummybool2) &&
                                 (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2)
                                 )
                             {
                                 AProduct = DragOfVEquation(i + 1,j,k) *
                                        (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[Get3DIndex(i,j,k)]);
                                 ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + (AProduct / ATimeStep)
                                                         / (1.0 + DragOfVEquation(i + 1,j,k) + BodyDragOfVEquation(i + 1,j,k));

                             }
                             if (    //VERIFICAR!!!
                                 (i < GridLines - 1) && (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2)
                                 )
                             {
                                 AProduct = BodyDragOfVEquation(i + 1,j,k) *
                                        //(BoxVolume[Get3DIndex(i + 1,j,k)] + BoxVolume[Get3DIndex(i,j,k)]) *
                                        (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[Get3DIndex(i,j,k)]);
                                 ASum4[Get2DIndex(i,j)] = ASum4[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                                         (1.0 + DragOfVEquation(i + 1,j,k) + BodyDragOfVEquation(i + 1,j,k));

                             }
                         }
                         if ((i < GridLines)  && (BoxArray[Get3DIndex(i,j,k)].Dummybool1) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
                         {
                            Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] + (FlowDischarge[Get3DIndex(i,j,k)] - FlowUptake[Get3DIndex(i,j,k)]);
                            /*if (((i == 4) || (i == 5)) && (j == 11)) {
                               Debugger(i); Debugger(j); Debugger(k);
                               Debugger(UContributionToContinuity(i, j, k));
                            } */
                            Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] + UContributionToContinuity(i, j, k);
                         }
                      }
                   }
                }  //k

             if (
               (i > 0) && (i < GridLines)
               && (BoxArray[Get3DIndex(i-1,j,BottomBox)].Dummybool2)
               && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool2)
               && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool1)
               && (BoxArray[Get3DIndex(i-1,j,BottomBox)].Dummybool1)
             )
             {
                AProduct = DragOfVEquation(i,j,BottomBox) *
                          (BoxDepth[Get3DIndex(i,j,BottomBox)] + BoxDepth[Get3DIndex(i-1,j,BottomBox)]);
                Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)]
                              + (
                                   AProduct * ATimeStep *
                                   VImplicitForDrag(i,j,BottomBox)
                                )/ (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);
                ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) /
                                        (1.0 + DragOfVEquation(i,j,BottomBox)+ BodyDragOfVEquation(i,j,BottomBox));
             }

             if (
                  (i < GridLines - 1)&&
                  (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool2)
                  && (BoxArray[Get3DIndex(i+1,j,BottomBox)].Dummybool2)
                  && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool1)
                  && (BoxArray[Get3DIndex(i+1,j,BottomBox)].Dummybool1)
                )
             {
               ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + DragOfVEquation(i+1,j,BottomBox) / ATimeStep*
                       (BoxDepth[Get3DIndex(i+1,j,BottomBox)] + BoxDepth[Get3DIndex(i,j,BottomBox)]) /
                       (1.0 + DragOfVEquation(i+1,j,BottomBox) + BodyDragOfVEquation(i+1,j,BottomBox));
             }
             //Termos implícitos da equação da continuidade para cálculo da elevação
             // Somas para termos explícitos
             if (i < GridLines)
             {

               Soma0[Get2DIndex(i,j,GridColumns+1)] = Soma0[Get2DIndex(i,j,GridColumns+1)] * ATimeStep / (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);
               //Soma7[Get2DIndex(i,j, GridColumns+1)] = Soma0[Get2DIndex(i,j, GridColumns+1)];
               Soma3[Get2DIndex(i,j, GridColumns+1)] = Soma3[Get2DIndex(i,j, GridColumns+1)] * ATimeStep / (2.0 * MeanColumnWidth[Get2DIndex(i,j)]/**
                                MeanColumnLength[Get2DIndex(i,j)]*/);
               Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] * ATimeStep /(MeanColumnWidth[Get2DIndex(i,j)] *
                                MeanColumnLength[Get2DIndex(i,j)]);
               Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] * ATimeStep;
               Soma11[Get2DIndex(i,j)] = Soma11[Get2DIndex(i,j)] * ATimeStep;
             }
             else
               Soma3[Get2DIndex(i,j, GridColumns+1)] = Soma3[Get2DIndex(i,j, GridColumns+1)] * ATimeStep / (2.0 * MeanColumnWidth[Get2DIndex(i-1,j)] /**
                                MeanColumnLength[Get2DIndex(i-1,j)]*/);
             Soma4[Get2DIndex(i,j, GridColumns+1)] = Soma4[Get2DIndex(i,j, GridColumns+1)] * ATimeStep;
             Soma5[Get2DIndex(i,j, GridColumns+1)] = Soma5[Get2DIndex(i,j, GridColumns+1)] * ATimeStep /** ATimeStep * Gravity*/;
             Soma6[Get2DIndex(i,j, GridColumns+1)] = Soma6[Get2DIndex(i,j, GridColumns+1)] * ATimeStep;
             if ((i > 0) && (i < GridLines) && (BoxArray[Get3DIndex(i,j,UpperLayer)].Dummybool2) && (BoxArray[Get3DIndex(i-1,j,UpperLayer)].Dummybool2)
               && (BoxArray[Get3DIndex(i,j,UpperLayer)].Dummybool1)
               && (BoxArray[Get3DIndex(i-1,j,UpperLayer)].Dummybool1))
              {
                Soma8[Get2DIndex(i,j, GridColumns+1)] = Soma8[Get2DIndex(i,j, GridColumns+1)] - WindStress(Get3DIndex(i,j,UpperLayer),"Wind speed (y component)") *
                                   pow(ATimeStep,2) /
                                   (
                                    0.5 * (WaterDensity[Get3DIndex(i,j,UpperLayer)] + WaterDensity[Get3DIndex(i-1,j,UpperLayer)]) *
                                     /*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]
                                   );
              }
            } //i
            ii = ii + 2;
         }  while (ii <= MIN(pSubDomain->FLine[j],GridLines-1));//while (ii < GridLines);
      } //j
       //Termo explícito
       for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
       {
          for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
          {
             if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j)))
             {
                if ((i > 0) && (!CheckForRivers(i-1,j)))
                {
                     atemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep  * ATimeStep *
                            (-Soma1[Get2DIndex(i,j)] +  ATimeStep * ASum1[Get2DIndex(i,j)] + ATimeStep * ASum3[Get2DIndex(i,j)]) /
                            (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)] * 0.5 * (MeanColumnWidth[Get2DIndex(i-1,j)] + MeanColumnWidth[Get2DIndex(i,j)]));
                }
                else
                     atemporaryCont[Get2DIndex(i,j)] = 0.0;
                btemporaryCont[Get2DIndex(i,j)] = - atemporaryCont[Get2DIndex(i,j)];
                if ((i < GridLines - 1) && (!CheckForRivers(i+1,j)))
                {
                     ctemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep * ATimeStep *
                             (-Soma2[Get2DIndex(i,j)] + ATimeStep * ASum2[Get2DIndex(i,j)] +  ATimeStep * ASum4[Get2DIndex(i,j)]) /
                                      (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)] *
                                      0.5 * (MeanColumnWidth[Get2DIndex(i,j)] + MeanColumnWidth[Get2DIndex(i+1,j)]));
                }
                else
                     ctemporaryCont[Get2DIndex(i,j)] = 0.0;
                btemporaryCont[Get2DIndex(i,j)] = btemporaryCont[Get2DIndex(i,j)] - ctemporaryCont[Get2DIndex(i,j)];
                btemporaryCont[Get2DIndex(i,j)] = 1.0 + btemporaryCont[Get2DIndex(i,j)];
                double WidthCorrection;
                if (i < GridLines - 1)
                   WidthCorrection = MeanColumnWidth[Get2DIndex(i+1,j)] / MeanColumnWidth[Get2DIndex(i,j)];
                else
                   WidthCorrection = 1.0;
                rtemporaryCont[Get2DIndex(i,j)] = (PastHeight[Get3DIndex(i,j,UpperLayer)]
                        + Soma0[Get2DIndex(i,j,GridColumns+1)] + Soma3[Get2DIndex(i,j, GridColumns+1)] - Soma4[Get2DIndex(i,j, GridColumns+1)]
                        - Soma5[Get2DIndex(i,j, GridColumns+1)] + Soma6[Get2DIndex(i,j, GridColumns+1)]
                        - Soma8[Get2DIndex(i,j, GridColumns+1)] + Soma10[Get2DIndex(i,j)] + Soma11[Get2DIndex(i,j)]) -
                       (
                          Soma0[Get2DIndex(i+1,j,GridColumns+1)] + Soma3[Get2DIndex(i+1,j, GridColumns+1)] - Soma4[Get2DIndex(i+1,j, GridColumns+1)] -
                          Soma5[Get2DIndex(i+1,j, GridColumns+1)] + Soma6[Get2DIndex(i+1,j, GridColumns+1)] - Soma8[Get2DIndex(i+1,j, GridColumns+1)]
                       ) * WidthCorrection
                       + Soma9[Get2DIndex(i,j)];

                if (i < GridLines - 1)
                   rtemporaryCont[Get2DIndex(i,j)] = rtemporaryCont[Get2DIndex(i,j)] - Soma11[Get2DIndex(i + 1,j)];
                if ((i > 0) && (CheckForSeaBoundariesInColumns(i-1,j))){
                   rtemporaryCont[Get2DIndex(i,j)] = rtemporaryCont[Get2DIndex(i,j)]
                          -DynamicHeight[Get3DIndex(i-1,j,UpperLayer)] *
                          atemporaryCont[Get2DIndex(i,j)];
                   atemporaryCont[Get2DIndex(i,j)] = 0.0;
                }
                if ((i < GridLines - 1) && (CheckForSeaBoundariesInColumns(i+1,j))){
                   rtemporaryCont[Get2DIndex(i,j)] = rtemporaryCont[Get2DIndex(i,j)]
                          -DynamicHeight[Get3DIndex(i+1,j,UpperLayer)] *
                          ctemporaryCont[Get2DIndex(i,j)];
                   ctemporaryCont[Get2DIndex(i,j)] = 0.0;
                }
                u[Cont] = 0.0;
                //Indexação dos termos implícitos e dos termos explícitos
                a[Cont] = atemporaryCont[Get2DIndex(i,j)];
                b[Cont] = btemporaryCont[Get2DIndex(i,j)];
                c[Cont] = ctemporaryCont[Get2DIndex(i,j)];
                r[Cont] = rtemporaryCont[Get2DIndex(i,j)];
                //if (((i == 4) || (i == 5)) && (j == 12))
                /*if (((i == 4) || (i == 5) || (i == 6)) && (j == 11) && (MyPEcoDynClass->GetCurrTime()>0.0053))
                {
                  Debugger(1111);   //Debugger(MyPEcoDynClass->GetCurrTime());
                  Debugger(rtemporaryCont[Get2DIndex(i,j)]);
                  Debugger(atemporaryCont[Get2DIndex(i,j)]);
                  Debugger(btemporaryCont[Get2DIndex(i,j)]);
                  Debugger(ctemporaryCont[Get2DIndex(i,j)]);
                  Debugger(PastHeight[Get3DIndex(i,j,UpperLayer)]);
                  Debugger(Soma0[Get2DIndex(i,j,GridColumns+1)]);
                  Debugger(Soma3[Get2DIndex(i,j, GridColumns+1)]);
                  Debugger(Soma4[Get2DIndex(i,j, GridColumns+1)]);
                  Debugger(Soma5[Get2DIndex(i,j, GridColumns+1)]);
                  Debugger(Soma6[Get2DIndex(i,j, GridColumns+1)]);
                  Debugger(Soma8[Get2DIndex(i,j, GridColumns+1)]);
                  Debugger(Soma10[Get2DIndex(i,j)]);
                  Debugger(Soma11[Get2DIndex(i,j)]);
                  Debugger(Soma0[Get2DIndex(i+1,j,GridColumns+1)]* WidthCorrection);
                  Debugger(Soma3[Get2DIndex(i+1,j, GridColumns+1)]* WidthCorrection);
                  Debugger(Soma4[Get2DIndex(i+1,j, GridColumns+1)]* WidthCorrection);
                  Debugger(Soma5[Get2DIndex(i+1,j, GridColumns+1)]* WidthCorrection);
                  Debugger(Soma6[Get2DIndex(i+1,j, GridColumns+1)]* WidthCorrection);
                  Debugger(Soma8[Get2DIndex(i+1,j, GridColumns+1)]* WidthCorrection); 
                  //Debugger(Soma9[Get2DIndex(i+1,j)]);
                } */
                Cont++;
             }
          }
       }
       IntegrationTridag(a, b, c, r, Cont);
       //mprovetridag(a, b, c, r, u, Cont);
       int kk = 0;
       //for (int j = 0; j < GridColumns; j++)
       for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
       {
          //for (int i = 0; i < GridLines; i++)
          for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
          {

            if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j))){
               DynamicHeight[Get3DIndex(i,j, UpperLayer)] = u[kk];
               a[kk] = 0.0;
               b[kk] = 0.0;
               c[kk] = 0.0;
               r[kk] = 0.0;
               kk++;
            }
          }
       }
   }  //Semi time step 2
   if (MyLoadRecord != NULL)
   {
       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
       {
          int i = MyLoadRecord[k].LineCoordinate;
          int j = MyLoadRecord[k].ColumnCoordinate;
          int l = MyLoadRecord[k].LayerCoordinate;
          int index = Get3DIndex(i,j,l);
          DynamicHeight[Get3DIndex(i,j, UpperLayer)] = DynamicHeight[Get3DIndex(i,j, UpperLayer)] +
                                                       ATimeStep * FlowDischarge[index] /
                                                       (
                                                          (BoxLength[index] * BoxWidth[index])
                                                       );
       }
   }
   if (MyLossesRecord != NULL)
   {
       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLosses(); k++)
       {
          int i = MyLossesRecord[k].LineCoordinate;
          int j = MyLossesRecord[k].ColumnCoordinate;
          int l = MyLossesRecord[k].LayerCoordinate;
          int index = Get3DIndex(i,j,l);
          DynamicHeight[Get3DIndex(i,j, UpperLayer)] = DynamicHeight[Get3DIndex(i,j, UpperLayer)] -
                                                       ATimeStep * FlowUptake[index] /
                                                       (
                                                          (BoxLength[index] * BoxWidth[index])
                                                       );
       }
   }
}



double TTriDimensionalWithBodyDrag::BodyDragOfUEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2))
   {
      return ATimeStep /
          (BoxVolume[Get3DIndex(i,j,k)] + BoxVolume[Get3DIndex(i,j - 1,k)]) *
          (
             Cd[Get3DIndex(i,j,k)] * BodyDiameter[Get3DIndex(i,j,k)] * MIN(BodyHeight[Get3DIndex(i,j,k)],BoxDepth[Get3DIndex(i,j,k)])+
             Cd[Get3DIndex(i,j-1,k)] * BodyDiameter[Get3DIndex(i,j-1,k)] * MIN(BodyHeight[Get3DIndex(i,j-1,k)],BoxDepth[Get3DIndex(i,j-1,k)])
          ) *
          UForRugosity(i,j,k) / 2.0;
   }
   else
      return 0.0;
}


double TTriDimensionalWithBodyDrag::BodyDragOfVEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i > 0) && (BoxArray[Get3DIndex(i - 1,j ,k)].Dummybool2))
      return ATimeStep /
          (BoxVolume[Get3DIndex(i,j,k)] + BoxVolume[Get3DIndex(i-1,j,k)]) *
          (
            Cd[Get3DIndex(i,j,k)] * BodyDiameter[Get3DIndex(i,j,k)] * MIN(BodyHeight[Get3DIndex(i,j,k)],BoxDepth[Get3DIndex(i,j,k)])+
            Cd[Get3DIndex(i-1,j,k)] * BodyDiameter[Get3DIndex(i-1,j,k)] * MIN(BodyHeight[Get3DIndex(i-1,j,k)],BoxDepth[Get3DIndex(i-1,j,k)])
           ) * VForRugosity(i,j,k) / 2.0;
   else
      return 0.0;

}


double TTriDimensionalWithBodyDrag::UImplicitForDrag(int ALine, int AColumn, int ALayer)
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;   //Emendar a partir daqui
   double MyDeltaU, MyWindEffect;
   MyWindEffect = 0.0;
   if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2))
   {
      if (k < UpperLayer)
         MyDeltaU = UVelocityPast[Get3DIndexForUVelocity(i,j,k + 1)] - UVelocityPast[Get3DIndexForUVelocity(i,j,k)];
      else
      {
         MyDeltaU = - UVelocityPast[Get3DIndexForUVelocity(i,j,k)];
         {
           MyWindEffect = WindStress(Get3DIndex(i,j,k),"Wind speed (x component)") * ATimeStep /
                       (
                         0.5 * (WaterDensity[Get3DIndex(i,j,k)] + WaterDensity[Get3DIndex(i,j-1,k)]) *
                         0.5 * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)])
                       );
         }
      }
      return(
          	    UVelocityPast[Get3DIndexForUVelocity(i,j,k)]
                - UConvectiveTerm(i,j,k)
                - WEBaroclinicPressureTerm(i,j, j-1, k)
                //- WEBarotropicPressureTerm(i,j, j-1, k)
                + UWEHorizontalDiffusion(i,j,k)
                + UNSHorizontalDiffusion(i,j,k) 
                + UVerticalDiffusion(3,i,j, k) * MyDeltaU
                + MyWindEffect
                + VCoriolis(i,j,k)
            )  /
            (1.0 + DragOfUEquation(i,j,k) + BodyDragOfUEquation(i,j,k));
   }
   else
      return 0.0;
}


double TTriDimensionalWithBodyDrag::VImplicitForDrag(int ALine, int AColumn, int ALayer)
{
   int i,j,k,MyLayer;
   i = ALine; j = AColumn; k = ALayer;
   double MyDeltaV, MyWindEffect;
   MyWindEffect = 0.0;
   if ((i > 0) && (BoxArray[Get3DIndex(i - 1,j ,k)].Dummybool2))
   {
     if (k < UpperLayer)
        MyDeltaV = VVelocityPast[Get3DIndexForVVelocity(i,j,k + 1)] - VVelocityPast[Get3DIndexForVVelocity(i,j,k)];
     else
     {
        MyDeltaV = - VVelocityPast[Get3DIndexForVVelocity(i,j,k)];
        MyWindEffect = WindStress(Get3DIndex(i,j,k),"Wind speed (y component)") * ATimeStep /
                                 (
                                   0.5 * (WaterDensity[Get3DIndex(i,j,k)] + WaterDensity[Get3DIndex(i- 1,j ,k)]) *
                                   0.5 * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i- 1,j ,k)])
                                 );
     }
     return(
               VVelocityPast[Get3DIndexForVVelocity(i,j,k)]
               - VConvectiveTerm(i,j,k)
               - NSBaroclinicPressureTerm(i, i-1, j,k)
               //- NSBarotropicPressureTerm(i, i-1, j,k)
               + VWEHorizontalDiffusion(i,j,k)
               + VNSHorizontalDiffusion(i,j,k)
               + VVerticalDiffusion(3,i,j, k) * MyDeltaV
               + WindEffect
               - UCoriolis(i,j,k)
            )  /
            (1.0 + DragOfVEquation(i,j,k) + BodyDragOfVEquation(i,j,k));
   }
   else
   return 0.0;
}


void TTriDimensionalWithBodyDrag::PartialSumInitialization()
{
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int i = 0; i <= GridLines; i++)
   {
      for (int j = 0; j <= GridColumns; j++)
      //for (int j = pSubDomain->IColumn[i]; j <= pSubDomain->FColumn[i]; j++)
      {
         int Index, Index1;
         Index = Get2DIndex(i,j);
         Index1 = Get2DIndex(i,j, GridColumns+1);
   	     Soma0[Index1] = 0.0;
   	     Soma3[Index1] = 0.0; Soma4[Index1] = 0.0;
         Soma5[Index1] = 0.0; Soma6[Index1] = 0.0; Soma7[Index1] = 0.0; Soma8[Index1] = 0.0;
         if ((i < GridLines) && (j < GridColumns))
         {
      	   Soma1[Index] = 0.0; Soma2[Index] = 0.0; Soma9[Index] = 0.0; Soma10[Index] = 0.0; Soma11[Index] = 0.0;
           ASum1[Index] = 0.0; ASum2[Index] = 0.0;
           ASum3[Index] = 0.0; ASum4[Index] = 0.0;
         }
      }
   }
}



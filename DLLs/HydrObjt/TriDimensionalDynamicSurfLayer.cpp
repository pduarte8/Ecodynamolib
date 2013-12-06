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

TTriDimensionalDynamicSurfaceLayer::TTriDimensionalDynamicSurfaceLayer(TEcoDynClass* PEcoDynClass,
        char* className) : TTriDimensional(PEcoDynClass, className)

{
   RemovingLayer = false; AddingLayer = false;
}

TTriDimensionalDynamicSurfaceLayer::~TTriDimensionalDynamicSurfaceLayer()
{
    freeMemory();
}


void TTriDimensionalDynamicSurfaceLayer::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    int LineIndex = GetLineIndex(MyBoxNumber);
    int ColumnIndex= GetColumnIndex(MyBoxNumber);
    strcpy(MyParameter, ParameterName);
    if (strcmp(MyParameter, "U Velocity") == 0)
        Value = getUVelocity3D(MyBoxNumber);
    else if (strcmp(MyParameter, "V Velocity") == 0)
        Value = getVVelocity3D(MyBoxNumber);
    else if (strcmp(MyParameter, "W Velocity") == 0)
         Value = getWVelocity3D(MyBoxNumber); //m / s A velocidade W é devolvida com sinal menos para que o Noesys
    else if (strcmp(MyParameter, "Dynamic height") == 0)
         Value = DynamicHeight[MyBoxNumber];
    else if (strcmp(MyParameter, "Salinity") == 0)
         Value = Salt[MyBoxNumber];
    else if (strcmp(MyParameter, "Box depth") == 0)
         Value = BoxDepth[MyBoxNumber];
    else if (strcmp(MyParameter,"Water density") == 0)
       	Value = WaterDensity[MyBoxNumber];
    else if (strcmp(MyParameter,"Box Volume") == 0)
       	Value = BoxVolume[MyBoxNumber];
    else if (strcmp(MyParameter,"Drag coefficient") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
       	Value = CoefRugosidade(i, j, k,2);
    }
    else if (strcmp(MyParameter,"VerticalDiffCoefU") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
        if ((k > 0) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
       	   Value = VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)];
        else
           Value = 0.0;   
    }
    else if (strcmp(MyParameter,"VerticalDiffCoefV") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
        if ((k > 0) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
       	   Value = VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)];
        else
           Value = 0.0;
    }
    else if (strcmp(MyParameter,"LengthScaleU") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
        if (((k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] > 0.0)) && (BoxDepth[Get3DIndex(i,j,k)] > 0.0))
           Value = LengthScale(i, j, k, getUVelocity3D(MyBoxNumber), UVelocity[Get3DIndexForUVelocity(i,j,k-1)], 1);
        else Value = 0.0;
    }
    else if (strcmp(MyParameter,"LengthScaleV") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
        if (((k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] > 0.0)) && (BoxDepth[Get3DIndex(i,j,k)] > 0.0))
           Value = LengthScale(i, j, k, getVVelocity3D(MyBoxNumber), VVelocity[Get3DIndexForVVelocity(i,j,k-1)], 2);
        else Value = 0.0;
    }
    else if (strcmp(MyParameter,"RichardsonNumberU") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
        if (((k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] > 0.0)) && (BoxDepth[Get3DIndex(i,j,k)] > 0.0))
           Value = RichardsonNumber(i, j, k, getUVelocity3D(MyBoxNumber), UVelocity[Get3DIndexForUVelocity(i,j,k-1)], 1);
        else Value = 0.0;
    }
    else if (strcmp(MyParameter,"RichardsonNumberV") == 0)
    {
        int i = GetLineIndex3D(MyBoxNumber);
        int j = GetColumnIndex3D(MyBoxNumber);
        int k = GetLayerIndex(MyBoxNumber);
        if (((k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] > 0.0)) && (BoxDepth[Get3DIndex(i,j,k)] > 0.0))
           Value = RichardsonNumber(i, j, k, getVVelocity3D(MyBoxNumber), VVelocity[Get3DIndexForVVelocity(i,j,k-1)], 2);
        else Value = 0.0;   
    }
    else if (strcmp(MyParameter, "U Flow") == 0)
    {
        Value = getUFlow(MyBoxNumber);
    }
    else if (strcmp(MyParameter, "V Flow") == 0)
    {
        Value = getVFlow(MyBoxNumber);
    }
    else if (strcmp(MyParameter, "UpperLayer") == 0)
    {
        Value = UpperLayer;
    }
    else if (strcmp(MyParameter, "Mean U Velocity") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
           	Value = MeanUVelocity[MyBoxNumber] / samples;
    }
    else if (strcmp(MyParameter, "Mean V Velocity") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
		    Value = MeanVVelocity[MyBoxNumber] / samples;
    }
	else if (strcmp(MyParameter, "Mean U Flow") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
	    	Value = MeanUFlow[MyBoxNumber] / samples;
    }
    else if (strcmp(MyParameter, "Mean V Flow") == 0)
    {
        if (samples == 0)
            Value = 0;
        else
    		Value = MeanVFlow[MyBoxNumber] / samples;
    }
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,Caption,"EcoDyn Alert - Inquiry 6",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TtriDimensionalDynamicSurfaceLayer::Inquiry 6 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


double TTriDimensionalDynamicSurfaceLayer::DragOfUEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2))
   {
      return ATimeStep /
          (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j - 1,k)]) *
          (CoefRugosidade(i,j,k,1) + CoefRugosidade(i,j-1,k,1)) *
          UForRugosity(i,j,k);
   }
   else
      return 0.0;
}


double TTriDimensionalDynamicSurfaceLayer::CoefRugosidade(int ALine, int AColumn, int ALayer, int AComponent)  //Tese do Aires, pág. 137
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;
   double MyCoefficient;
   //Debugger(CoefManning[Get3DIndex(i,j,k)]);
   if (CoefManning[Get3DIndex(i,j,k)] > 0.0)
   {
      double MyHidraulicRadius;
      MyHidraulicRadius = HydraulicRadius(i,j,k, AComponent);
      //MyHidraulicRadius = 1.0;
      if (MyHidraulicRadius == 0.0)
         MyCoefficient = 0.0;
      else
         MyCoefficient = (Gravity * pow(CoefManning[Get3DIndex(i,j,k)],2.0)) / pow(MAX(MyHidraulicRadius, HMINC),0.333333);
   }
   else
   {        //Tese do Aires, pág. 137  */
     const double VonKarman = 0.4;
     double DistanceFromTheBottom, Z;
     DistanceFromTheBottom = Depth(i,j,0) - (Depth(i,j,k) - BoxDepth[Get3DIndex(i,j,k)] / 2.0);
      //Retiro metade da espessura da caixa à distância ao fundo do respectivo topo
     Z = MAX(DistanceFromTheBottom, HMINC);
     MyCoefficient = pow(VonKarman, 2.0) / pow((log(Z / Rugosity[Get3DIndex(i,j,k)])),2.0);
   }
   return MyCoefficient;
}


double TTriDimensionalDynamicSurfaceLayer::HydraulicRadius(int ALine, int AColumn, int ALayer, int AComponent)
{
   int i = ALine, j = AColumn, k = ALayer, index;
   double Sum = 0.0, MyHydraulicRadius = 0.0;
   index = Get3DIndex(i,j,k);
   
   if ((BoxArray[Get3DIndex(i,j,k)].Dummybool2) && (BoxArray[Get3DIndex(i,j,k)].Dummybool1))
   {
     if (AComponent == 1) //u component
     {
           if (
                (
                    (i == 0) && (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4)
                ) ||
                (
                    (i > 0 ) && ((!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool1))
                )
             )
              Sum = Sum + BoxDepth[index];

           if (
                (
                    (i == GridLines - 1) && (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 4)
                ) ||
                (
                    (i < GridLines-1) && ((!BoxArray[Get3DIndex(i+1,j,k)].Dummybool2) && (BoxArray[Get3DIndex(i+1,j,k)].Dummybool1))
                )
              )
              Sum = Sum + BoxDepth[index];
           if (
                 (
                   (k > 0) && (!BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                   (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool1)
                 ) ||
                 (k == 0)
              )
              Sum = Sum + BoxWidth[index];
              if (Sum > 0.000000000001)
                 MyHydraulicRadius = BoxDepth[index] * BoxWidth[Get3DIndex(i,j,k)] / Sum;
     }
     else //v component
     {
           if (
                (
                    (j == 0) && (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4)
                ) ||
                (
                    (j > 0) && ((!BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool1))
                )
              )
              Sum = Sum + BoxDepth[index];
           if (
                (
                    (j == GridColumns - 1) && (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 4)
                ) ||
                (
                    (j < GridColumns - 1) && (!(BoxArray[Get3DIndex(i,j + 1,k)].Dummybool2) && (BoxArray[Get3DIndex(i,j+1,k)].Dummybool1))
                )
              )
              Sum = Sum + BoxDepth[index];
           if (
                 (
                   (k > 0) && (!BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                   (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool1)
                 ) ||
                 (k == 0)
              )
              Sum = Sum + BoxLength[index];
              if (Sum > 0.000000000001)
                 MyHydraulicRadius = BoxDepth[index] * BoxLength[Get3DIndex(i,j,k)] / Sum;
     }
   }
   return MyHydraulicRadius;
}


double TTriDimensionalDynamicSurfaceLayer::UImplicitForDrag(int ALine, int AColumn, int ALayer)
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
            (
               1.0 + ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j - 1,k)])
               * (CoefRugosidade(i,j,k,1) + CoefRugosidade(i,j - 1,k,1)) * UForRugosity(i, j, k)
            );
   }
   else
      return 0.0;
}


double TTriDimensionalDynamicSurfaceLayer::DragOfVEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i > 0) && (BoxArray[Get3DIndex(i - 1,j ,k)].Dummybool2))
      return ATimeStep /
          (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                       		(
                                   CoefRugosidade(i,j,k,2) +
                                   CoefRugosidade(i-1,j,k,2)
                       		) * VForRugosity(i,j,k);
   else
      return 0.0;

}


double TTriDimensionalDynamicSurfaceLayer::VImplicitForDrag(int ALine, int AColumn, int ALayer)
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
               + MyWindEffect
               - UCoriolis(i,j,k)
            )  /
            (
               1.0 + ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i- 1,j ,k)])
               * (CoefRugosidade(i,j,k,2) + CoefRugosidade(i - 1,j,k,2)) * VForRugosity(i, j, k)
            );
   }
   else
   return 0.0;
}


double TTriDimensionalDynamicSurfaceLayer::bUCalculation(int ALine, int AColumn, int ALayer)
{
     int i,j,k;
    i = ALine; j = AColumn; k = ALayer;
    double MyValue;
    if (
          (k == 0) ||
          (
             //(k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] <= CriticalDepthForLandBoundary) /*Fundo=> Não há difusão, mas sim atrito*/
             (k > 0) && (!BoxArray[Get3DIndex(i,j,k-1)].Dummybool2) 
          ) ||
          (
             (k > 0) && (!BoxArray[Get3DIndex(i,j-1,k-1)].Dummybool2)
          )
      	)
    {
       //if ((k < UpperLayer) && (BoxDepth[Get3DIndex(i,j,k+1)] > CriticalDepthForLandBoundary))
       if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2))
      		MyValue = VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k+1)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j+1,k)] + BoxDepth[Get3DIndex(i,j,k)]),2.0);

       //else if ((k == UpperLayer) || ((k < UpperLayer) && (BoxDepth[Get3DIndex(i,j,k+1)] < CriticalDepthForLandBoundary)))
       else if ((k == UpperLayer) || ((k < UpperLayer) && (!BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)))
            MyValue = 0.0;
       return  (
                 	  1.0 + ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)])
                    * (CoefRugosidade(i,j,k,1) + CoefRugosidade(i,j - 1,k,1)) * fabs(UVelocityPast[Get3DIndexForUVelocity(i,j,k)])
                 ) + MyValue;

    }
    else
      {
       //if ((k == UpperLayer) || ((k < UpperLayer) && (BoxDepth[Get3DIndex(i,j,k+1)] < CriticalDepthForLandBoundary)))
       if ((k == UpperLayer) || ((k < UpperLayer) && (!BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)))
         	MyValue = VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0);
       else
   			MyValue = VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0)
                 /*pow(BoxLength[i * GridColumns + j - 1],2.0)*/ +
              	  VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k+1)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j,k+1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0)
                 /*pow(BoxLength[i * GridColumns + j],2.0)*/;
       return  1.0 + MyValue;
      }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensionalDynamicSurfaceLayer::bVCalculation(int ALine, int AColumn, int ALayer)
{
    int i,j,k;
    i = ALine; j = AColumn; k = ALayer;
    double MyValue;
    if (
         	(k == 0) ||
         	(
                    //(k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] <= CriticalDepthForLandBoundary) /*Fundo=> Não há difusão, mas sim atrito*/
                    (k > 0) && (!BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)
         	) ||
                (
                   (k > 0) && (!BoxArray[Get3DIndex(i-1,j,k-1)].Dummybool2)
                )
      	)
    {
       //if ((k < UpperLayer) && (BoxDepth[Get3DIndex(i,j,k+1)] > CriticalDepthForLandBoundary))
       if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2))
      		MyValue = VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k+1)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i+1,j,k)] + BoxDepth[Get3DIndex(i,j,k)]),2.0);

       //else if ((k == UpperLayer) || ((k < UpperLayer) && (BoxDepth[Get3DIndex(i,j,k+1)] < CriticalDepthForLandBoundary)))
       else if ((k == UpperLayer) || ((k < UpperLayer) && (!BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)))
            MyValue = 0.0;

       return  (
                 	  1.0 + ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])
                    * (CoefRugosidade(i,j,k,2) + CoefRugosidade(i-1,j,k,2)) * fabs(VVelocityPast[Get3DIndexForVVelocity(i,j,k)])
                 ) + MyValue;
    }
    else
      {
       //if ((k == UpperLayer) || ((k < UpperLayer) && (BoxDepth[Get3DIndex(i,j,k+1)] < CriticalDepthForLandBoundary)))
       if ((k == UpperLayer) || ((k < UpperLayer) && (!BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)))
         	MyValue = VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0);
       else
   			MyValue = VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0)
                 /*pow(BoxLength[i * GridColumns + j - 1],2.0)*/ +
              	  VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k+1)] * ATimeStep /
                 pow(0.5*(BoxDepth[Get3DIndex(i,j,k+1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0)
                 /*pow(BoxLength[i * GridColumns + j],2.0)*/;

       return  1.0 + MyValue;
      }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensionalDynamicSurfaceLayer::AdvectDiffuse(double* Generic)
{
   double EasternDepth, WesternDepth, SouthernDepth, NorthernDepth,
          UComponent1, UComponent2, VComponent1, VComponent2, WComponent1, WComponent2,
          WesternConcentration, EasternConcentration, SouthernConcentration, NorthernConcentration,
          ConcentrationBelow, ConcentrationAbove;
   int index, cont;
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
                    /*if ((AddingLayer) && (cont == 0))
                    {
                      Debugger(i); Debugger(j); Debugger(layer);
                      Debugger(BoxVolume[index]); 
                    } */
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
                    /*if (cont == 4)
                    {
                      Debugger(111111);
                      Debugger(i); Debugger(j); Debugger(layer);
                      Debugger(index);
                      Debugger(BoxDepth[index]);
                    }  */
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
void TTriDimensionalDynamicSurfaceLayer::Reinitialize()
{
    double const MinCritDepth = 0.01;  //0.00001 m
    double const TINNY = 0.00000000001;
    int index3D, index2D, index3DU;
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    double MyWaterTemperature, CumulativePressure, AddedPressure;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
    {
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
       {
         index2D = Get2DIndex(i,j);
         atemporaryCont[index2D] = 0.0; btemporaryCont[index2D] = 0.0;
         ctemporaryCont[index2D] = 0.0; rtemporaryCont[index2D] = 0.0;
         for (int k = 0; k <= UpperLayer; k++)
         {
            index3D = Get3DIndex(i,j,k);
      	    atemporary[index3D] = 0.0; btemporary[index3D] = 0.0;
            ctemporary[index3D] = 0.0; rtemporary[index3D] = 0.0;

            if ((k == UpperLayer) && (BoxArray[index3D].Dummybool2) && (BoxArray[index3D].Dummybool1))
            {
               BoxDepth[index3D] = BoxVolume[index3D] / (BoxWidth[index3D] * BoxLength[index3D]);
               PastBoxVolume[index3D] = BoxVolume[index3D];
               if (BoxDepth[index3D] <= CriticalDepthForLandBoundary) //&&(BoxArray[index3D].Dummybool2))
               {
                  if (
                       (k == 0) ||
                       ((k > 0) && (!BoxArray[Get3DIndex(i,j,UpperLayer-1)].Dummybool2))
                     )
                  BoxArray[index3D].Dummybool2 = false;
               }
            }
         }
      }
   }
   for (int i = pSubDomain->FirstLine;i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         for (int k = 0; k <= UpperLayer; k++)
         {
            index3D = Get3DIndex(i,j,k);
            if (
                  (k == UpperLayer)  &&
                  (!CheckForSeaBoundary(i, j, k))
               )
            {
                if   (
                        (! BoxArray[index3D].Dummybool2) && (BoxDepth[index3D] >= MinCritDepth) &&  //Tive que mudar este valor de zero para 0.00001 pois havia casos
                        (                                                                       // em que a BoxDepth estava tão próxima de zero que ocorria um floating point error
                             (                                                                     //no AdvectDiffuse no cálculo da concentração, quando divide pela soma
                               (j < GridColumns - 1) &&                                          // da BoxDepth com a diferença entre a elevação presente e a elevação passada
                               (DynamicHeight[Get3DIndex(i,j + 1,UpperLayer)] > DynamicHeight[index3D]) &&
                               (BoxDepth[Get3DIndex(i,j + 1,UpperLayer)] > CriticalDepthForLandBoundary)
                             ) ||
                             (
                               (j > 0) &&
                               (DynamicHeight[Get3DIndex(i,j - 1,UpperLayer)] > DynamicHeight[index3D]) &&
                               (BoxDepth[Get3DIndex(i,j - 1,UpperLayer)] > CriticalDepthForLandBoundary)
                             ) ||
                             (
                               (i < GridLines - 1)  &&
                               (DynamicHeight[Get3DIndex(i + 1,j,UpperLayer)] > DynamicHeight[index3D]) &&
                               (BoxDepth[Get3DIndex(i + 1,j,UpperLayer)] > CriticalDepthForLandBoundary)
                             ) ||
                             (
                               (i > 0) &&
                               (DynamicHeight[Get3DIndex(i - 1,j,UpperLayer)] > DynamicHeight[index3D]) &&
                               (BoxDepth[Get3DIndex(i - 1,j,UpperLayer)] > CriticalDepthForLandBoundary)
                             )
                        )
                        ||
                        (! BoxArray[index3D].Dummybool2) && (BoxDepth[index3D] >= CriticalDepthForLandBoundary)
            	     )
                     BoxArray[index3D].Dummybool2 = true;
            }
            if (
         		(!BoxArray[index3D].Dummybool2) ||
               (
               	  (BoxArray[index3D].Dummybool2) &&
                  (BoxDepth[index3D] <= CriticalDepthForLandBoundary) &&
                  (DynamicHeight[index3D] < PastHeight[index3D])
               )
            )

           {
              UVelocity[Get3DIndexForUVelocity(i,j,k)] = 0.0;
              VVelocity[Get3DIndexForVVelocity(i,j,k)] = 0.0;
              UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] = 0.0;
              VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] = 0.0;
              WVelocity[index3D] = 0.0;
           }
           if (fabs(UVelocity[Get3DIndexForUVelocity(i,j,k)]) < TINNY)
               UVelocity[Get3DIndexForUVelocity(i,j,k)] = 0.0;
           if (fabs(VVelocity[Get3DIndexForVVelocity(i,j,k)]) < TINNY)
               VVelocity[Get3DIndexForVVelocity(i,j,k)] = 0.0;
           if (fabs(WVelocity[index3D]) < TINNY)
               WVelocity[index3D] = 0.0;

         }
      }
   }
   CheckSurfaceLayer();
   //if (MyPEcoDynClass->GetCurrTime() > 7.00694)
   //Debugger(2);
   RiverForcing();
   //if (MyPEcoDynClass->GetCurrTime() > 7.00694)
   DischargeForcing();

   for (int i = 0; i <= GridLines; i++)
   {
      for (int j = 0; j <= GridColumns; j++)
      {
         CumulativePressure = 0.0;
         for (int k = UpperLayer; k >= 0; k--)
         {
            index3D = Get3DIndex(i,j,k);
            index3DU = Get3DIndexForUVelocity(i,j,k);
            if (i < GridLines){
               UVelocityPast[index3DU] = UVelocity[index3DU];
            }
            if (j < GridColumns) {
               VVelocityPast[Get3DIndexForVVelocity(i,j,k)] = VVelocity[Get3DIndexForVVelocity(i,j,k)];
            }
            if ((i < GridLines) && (j < GridColumns))
            {
              PastHeight[index3D] = DynamicHeight[index3D];
              WVelocityPast[index3D] = WVelocity[index3D];
              if (BoxArray[index3D].Dummybool2){
                  AddedPressure = Gravity * WaterDensity[index3D] * 0.5 * BoxDepth[index3D];
                  if (k == UpperLayer)
                     Pressure[index3D] = AddedPressure;
                  else if (k < UpperLayer)
                     Pressure[index3D] = CumulativePressure + AddedPressure;
                  CumulativePressure = CumulativePressure + 2.0 * AddedPressure;
                //if ((i == 0) && (j == 1) && (k == 4)) Debugger(3);
                if (MyWaterTemperaturePointer != NULL)
                {
                   MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,index3D,"Water temperature",ObjectCode);

                   //if ((i == 0) && (j == 1) && (k == 4)) Debugger(Salt[index3D]);
                   //if ((i == 0) && (j == 1) && (k == 4)) Debugger(Pressure[index3D]);
                   WaterDensity[index3D] = MyPEcoDynClass->GetDensity(MAX(0.0,Salt[index3D]),MAX(0.0,MyWaterTemperature),Pressure[index3D] * pow(10.0,-4) /*Converting pressure from Pascal to DeciBar*/);
                   /*if (((i == 4) || (i == 5)) && (j == 11) && (k == 1))
                   {
                       //Debugger(2);
                       Debugger(i);
                       //Debugger(MyWaterTemperature);
                       //Debugger(Salt[index3D]);
                       Debugger(WaterDensity[index3D]*10000);
                   }  */
                }
                else
                   //WaterDensity[index3D] = 1026.75; // seawater at 35psu and 10oC;
                   WaterDensity[index3D] = MyPEcoDynClass->GetDensity(Salt[index3D],15.0,Pressure[index3D] * pow(10.0,-4) /*Converting pressure from Pascal to DeciBar*/);
                //if ((i == 0) && (j == 1) && (k == 4)) Debugger(4);
              }
            }
         }
      }
   }
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
   {
      for (int j = MAX(pSubDomain->IColumn[i],0); j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         for (int k = 1; k <= UpperLayer; k++)
         {
          VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] = VerticalMomentumDiffusivity(i,j,k,UVelocity[Get3DIndexForUVelocity(i,j,k)],UVelocity[Get3DIndexForUVelocity(i,j,k - 1)],1);
          VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] = VerticalMomentumDiffusivity(i,j,k,VVelocity[Get3DIndexForVVelocity(i,j,k)],VVelocity[Get3DIndexForVVelocity(i,j,k - 1)],2);
         }
      }
   }
   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensionalDynamicSurfaceLayer::CheckSurfaceLayer()
{
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   int index3D;
   double Delta;
   Delta = 2.0 * CriticalDepthForLandBoundary;
   RemovingLayer = false; AddingLayer = false;
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         index3D = Get3DIndex(i,j,UpperLayer);
         if (
             (BoxDepth[index3D] > 0.0) &&
             (BoxDepth[index3D] < CriticalDepthForLandBoundary) && (UpperLayer > 0) &&
             (BoxDepth[Get3DIndex(i,j,UpperLayer-1)] > CriticalDepthForLandBoundary)
            )
         {
            RemovingLayer = true; break;
         }
         if ((UpperLayer < GridLayers - 1) && (BoxDepth[index3D] > 0.0) && (PastBoxDepth[index3D] > 0.0))
         {
           Delta = MIN(BoxDepth[index3D] - PastBoxDepth[index3D], Delta);
         }
      }
      if (RemovingLayer) break;
   }
   if (RemovingLayer) RemoveLayer();
   else
   if ((Delta > CriticalDepthForLandBoundary) && (UpperLayer < GridLayers - 1))
   {
     AddingLayer = true;
     //Debugger(Delta);
     AddLayer();
   }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensionalDynamicSurfaceLayer::RemoveLayer()
{
   //Debugger(1);
   if (UpperLayer > 0) {
     int index, indexMinusOne;
     double UpperDepth, LowerDepth, TotalDepth;
     SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
     for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine + 1; i++)
     {
       for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i] + 1,GridColumns); j++)
       {
           if (i < pSubDomain->LastLine + 1) {
             //U component
             if ((j > pSubDomain->IColumn[i]) && (j < pSubDomain->FColumn[i] + 1)) {
               UpperDepth = MAX(0,(0.5 * (BoxDepth[Get3DIndex(i,j-1,UpperLayer)] + BoxDepth[Get3DIndex(i,j,UpperLayer)])));
               LowerDepth = MAX(0,(0.5 * (BoxDepth[Get3DIndex(i,j-1,UpperLayer-1)] + BoxDepth[Get3DIndex(i,j,UpperLayer-1)])));
             }
             else if (j == pSubDomain->IColumn[i])
             {
                UpperDepth = BoxDepth[Get3DIndex(i,j,UpperLayer)];
                LowerDepth = BoxDepth[Get3DIndex(i,j,UpperLayer-1)];
             }
             else if (j == pSubDomain->FColumn[i] + 1)
             {
                UpperDepth = BoxDepth[Get3DIndex(i,j-1,UpperLayer)];
                LowerDepth = BoxDepth[Get3DIndex(i,j-1,UpperLayer-1)];
             }
             TotalDepth = UpperDepth + LowerDepth;
             if (TotalDepth > 0.0)
                UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer - 1)] = (UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer - 1)] * LowerDepth +
                                                                         UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer)] * UpperDepth) /TotalDepth;
             else
                UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer - 1)] = 0.0;
             UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer)] = 0.0;
           }
           if (j < pSubDomain->FColumn[i] + 1) {
           //V component
             if ((i > pSubDomain->FirstLine) && (i < pSubDomain->LastLine + 1)) {
               UpperDepth = MAX(0,(0.5 * (BoxDepth[Get3DIndex(i-1,j,UpperLayer)] + BoxDepth[Get3DIndex(i,j,UpperLayer)])));
               LowerDepth = MAX(0,(0.5 * (BoxDepth[Get3DIndex(i-1,j,UpperLayer-1)] + BoxDepth[Get3DIndex(i,j,UpperLayer-1)])));
             }
             else if (i == pSubDomain->FirstLine)
             {
               UpperDepth = BoxDepth[Get3DIndex(i,j,UpperLayer)];
               LowerDepth = BoxDepth[Get3DIndex(i,j,UpperLayer-1)];
             }
             else if (i == pSubDomain->LastLine + 1)
             {
               UpperDepth = BoxDepth[Get3DIndex(i-1,j,UpperLayer)];
               LowerDepth = BoxDepth[Get3DIndex(i-1,j,UpperLayer-1)];
             }
             TotalDepth = UpperDepth + LowerDepth;
             if (TotalDepth > 0.0)
                 VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer - 1)] = (VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer - 1)] * LowerDepth +
                                                                          VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer)] * UpperDepth) /TotalDepth;
             else
                 VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer - 1)] = 0.0;
             VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer)] = 0.0;
           }
           //W component
           if ((i < pSubDomain->LastLine + 1) && (j < pSubDomain->FColumn[i] + 1))
              WVelocity[Get3DIndex(i,j,UpperLayer)] = 0.0;
        }
     }
     for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
     {
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {   
           index = Get3DIndex(i,j,UpperLayer);
           if (BoxDepth[Get3DIndex(i,j,UpperLayer - 1)] > 0.0)
           {
             BoxDepth[Get3DIndex(i,j,UpperLayer - 1)] = BoxDepth[Get3DIndex(i,j,UpperLayer - 1)] + BoxDepth[index];
             BoxVolume[Get3DIndex(i,j,UpperLayer - 1)] = BoxVolume[Get3DIndex(i,j,UpperLayer - 1)] + BoxVolume[index];
           }
           else
           {
             BoxDepth[Get3DIndex(i,j,UpperLayer - 1)] = BoxDepth[index];
             BoxVolume[Get3DIndex(i,j,UpperLayer - 1)] = BoxVolume[index];
           }
           if (BoxDepth[Get3DIndex(i,j,UpperLayer - 1)] > CriticalDepthForLandBoundary) BoxArray[Get3DIndex(i,j,UpperLayer - 1)].Dummybool2 = true;
           PastBoxVolume[Get3DIndex(i,j,UpperLayer - 1)] = BoxVolume[Get3DIndex(i,j,UpperLayer - 1)];
           DynamicHeight[Get3DIndex(i,j,UpperLayer - 1)] = DynamicHeight[index];
           PastHeight[Get3DIndex(i,j,UpperLayer - 1)] = DynamicHeight[Get3DIndex(i,j,UpperLayer - 1)];
        }
     }
     UpperLayer = UpperLayer - 1;
     }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensionalDynamicSurfaceLayer::AddLayer()
{
   //Debugger(2);
   int index, indexAbove;
   double UpperDepth, LowerDepth, TotalDepth;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine + 1; i++)
   {
     for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i] + 1,GridColumns); j++)
     {
         if ((i < pSubDomain->LastLine + 1) && (j < pSubDomain->FColumn[i] + 1)) {
           index = Get3DIndex(i,j,UpperLayer);
           indexAbove = Get3DIndex(i,j,UpperLayer + 1);
           BoxDepth[indexAbove] = BoxDepth[index] - PastBoxDepth[index];
           BoxDepth[index] = PastBoxDepth[index];
           BoxVolume[indexAbove] = BoxLength[indexAbove] * BoxWidth[indexAbove] * BoxDepth[indexAbove];
           PastBoxVolume[indexAbove] = BoxVolume[indexAbove];
           BoxVolume[index] = BoxLength[index] * BoxWidth[index] * BoxDepth[index];
           PastBoxVolume[index] = BoxVolume[index];
           DynamicHeight[indexAbove] = DynamicHeight[index];
           PastHeight[indexAbove] = DynamicHeight[indexAbove];
           DynamicHeight[index] = 0.0;
           PastHeight[index] = 0.0;
           //WVelocity[indexAbove] = WVelocity[index];
           if (BoxDepth[indexAbove] > CriticalDepthForLandBoundary) BoxArray[indexAbove].Dummybool2 = true;
           if (BoxDepth[index] < CriticalDepthForLandBoundary) BoxArray[index].Dummybool2 = false;
         }
         if (i < pSubDomain->LastLine + 1)
            UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer + 1)] = UVelocity[Get3DIndexForUVelocity(i,j,UpperLayer)];
         if (j < pSubDomain->FColumn[i] + 1)
            VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer + 1)] = VVelocity[Get3DIndexForVVelocity(i,j,UpperLayer)];
     }
   }
   UpperLayer = UpperLayer + 1;
   //Debugger(UpperLayer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensionalDynamicSurfaceLayer::TideForcing()
{
   double MyCurrentTime;
   MyTime = MyPEcoDynClass->GetCurrentTime();
   SurfaceHeight = cos(2.0 * M_PI / 12 * MyTime) * 0.5 + 0.5;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

   double MyYear, MyMonth, MyDay;
   int i, nbox, Index3D;
   TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();

   if (MyTidePointer != NULL)
   {
      MyYear = MyPEcoDynClass->GetYear();
      MyMonth = MyPEcoDynClass->GetMonth();
      MyDay = MyPEcoDynClass->GetDate();
      MyTime = MyPEcoDynClass->GetCurrTime();

      MyTidePointer->Inquiry(GetEcoDynClassName(), SurfaceHeight,
              MyYear, MyMonth, MyDay, MyTime);
      SurfaceHeight = SurfaceHeight / CUBIC;  //convert from mm to m
      for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
      {
        for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
        {
            Index3D = Get3DIndex(i,j, UpperLayer);
            if ((BoxArray[Index3D].Dummybool2) ||
                ((!BoxArray[Index3D].Dummybool2) && (SurfaceHeight > PastHeight[Index3D]) && (BoxDepth[Index3D] > CriticalDepthForLandBoundary)))   //15/6/2009
            {
              if ((BoxArray[Index3D].BoxToBoundary[1].TypeOfBoundary == 2) ||
                  (BoxArray[Index3D].BoxToBoundary[2].TypeOfBoundary == 2) ||
                  (BoxArray[Index3D].BoxToBoundary[3].TypeOfBoundary == 2) ||
                  (BoxArray[Index3D].BoxToBoundary[4].TypeOfBoundary == 2))
              {
                  DynamicHeight[Index3D] = SurfaceHeight;
                  BoxArray[Index3D].Dummybool2 = true;
              }
            }
         }
      }
   }  
}

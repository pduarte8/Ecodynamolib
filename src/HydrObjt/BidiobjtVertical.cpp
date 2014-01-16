// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
    #include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"

TBiDimensionalVertical::TBiDimensionalVertical(TEcoDynClass* PEcoDynClass, char* className)
                            :TOneDimensionalDITransport(PEcoDynClass, className)
{
    if  (strcmp(className,"TBiDimensionalVertical") == 0)
    BuildBiDimensional();
}

void TBiDimensionalVertical::BuildBiDimensional()
{
	UpstreamBoundaryNumber = 1; DownstreamBoundaryNumber = 3;
    CriticalDepth = 0.03;
    CriticalWidth = 0.5;
    MaxVertDiffusivity = 0.01;
    TotalHours = 0.0;
   //Debugger(NumberOfBoxes);

   //Parameters file
   int X,Y, XV, YV;
   double MyValue;

   GridLines = MyPEcoDynClass->GetNumberOfLines();
   GridColumns = MyPEcoDynClass->GetNumberOfColumns();
   //Debugger(GridLines); Debugger(GridColumns);
   UpperLayer = GridLines - 1;
   //Debugger(UpperLayer);
   //Debugger(NumberOfBoxes);
   if (NumberOfBoxes > 0)
	{
      HorizontalDiffCoefU = new double[NumberOfBoxes + GridLines];
      VerticalDiffCoefU = new double[NumberOfBoxes + GridLines];
      VerticalMassAndHeatDiffCoef = new double[NumberOfBoxes + GridLines];
      Rugosity = new double[NumberOfBoxes];
      ConvectionValue = new double[NumberOfBoxes];
      DiffusionValue = new double[NumberOfBoxes];
      //BottomDragValue = new double[NumberOfBoxes];
      BarotropicPressureValue = new double[NumberOfBoxes];
      BaroclinicPressureValue = new double[NumberOfBoxes];
      MassDiffusion = new double[NumberOfBoxes];
      MassConvection = new double[NumberOfBoxes];
//      PastGeneric = new double[NumberOfBoxes];
      SurfaceBox = new int[GridColumns];
      SurfaceLayer = new int[GridColumns];
      DepthVariation = new double[GridColumns];
      for (int i = 0; i < NumberOfBoxes + GridLines; i++)
      {
      	if (i < GridColumns)
         {
         	SurfaceLayer[i] = UpperLayer;
            DepthVariation[i] = 0.0;
         }
         if (i < NumberOfBoxes)
         {
         	Rugosity[i] = 0.0;
         	BoxLength[i] = MyPEcoDynClass->GetBoxLength(i);
         	ConvectionValue[i] = 0.0;
         	DiffusionValue[i] = 0.0;
         	//BottomDragValue[i] = 0.0;
         	BarotropicPressureValue[i] = 0.0;
         	BaroclinicPressureValue[i] = 0.0;
         	MassDiffusion[i] = 0.0;
         	MassConvection[i] = 0.0;
//         	PastGeneric[i] = 0.0;
         }
         VerticalDiffCoefU[i] = 0.0;
         VerticalMassAndHeatDiffCoef[i] = 0.0;
         HorizontalDiffCoefU[i] = 0.0;
      }
	}

    // Read in the parameters
    ParameterCount = 0;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);
    //
    // List of parameters initialized in TOneDimensionalDITransport
    //
                if (strcmp(MyParameter, "Gravity") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, Gravity);
                    //Debugger(Gravity);
                }
                else
                if (strcmp(MyParameter, "Critical depth") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, CriticalDepth);
                    //Debugger(CriticalDepth);
                }
                else
                if (strcmp(MyParameter, "Rugosity") == 0)
                {
                    if (PReadWrite->FindString("Rugosity values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Rugosity[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        //Debugger(MyValue);
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            Rugosity[j] = MyValue;
                        }
                    }
                }
                else
                if (strcmp(MyParameter, "HMINC") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, HMINC);
                    //Debugger(HMINC);
                }
                else
                if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
                {
                    if (PReadWrite->FindString("Coeficiente de Manning values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, CoefManning[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 3, i + j, CoefManning[j]);
                            //Debugger(CoefManning[j]);
                        }
                    }
                }
                else
                if (strcmp(MyParameter, "Horizontal diffusion coefficient") == 0)
                {
                    if (PReadWrite->FindString("Horizontal diffusion coefficient values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, HorizontalDiffCoefU[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                        {
                            PReadWrite->ReadNumber(X + 4, i + j, HorizontalDiffCoefU[j]);
                            //if (j == NumberOfBoxes + GridLines - 1) Debugger(HorizontalDiffCoefU[j]);
                        }
                    }
                }
                else
                if (strcmp(MyParameter, "Wind stress coeficient") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, WindStressCoef);
                    //Debugger(WindStressCoef);
                }
                else
                if (strcmp(MyParameter, "kdiffusion") == 0)
                {
                   PReadWrite->ReadNumber(X + 3, i, kdiffusion);
                   //Debugger(kdiffusion);
                }
                else
                if (strcmp(MyParameter, "Critical width") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, CriticalWidth);
                    //Debugger(CriticalWidth);
                }
                else
                if (strcmp(MyParameter, "Maximum vertical momentum diffusivity") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, MaxVertDiffusivity);
                    //Debugger(CriticalWidth);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalVertical::TBiDimensionalVertical - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
       //End reading parameters file
        CloseDataFile((void*)PReadWrite);
    }
    
   if (NumberOfBoxes > 0)
	{
   	//HorizontalDiffCoefU = new double[NumberOfBoxes + GridLines];
      UVelocity = new double[NumberOfBoxes + GridLines];
      UVelocityPast = new double[NumberOfBoxes + GridLines];
      WVelocity = new double[NumberOfBoxes];
      WVelocityPast = new double[NumberOfBoxes];
      UFlowBetweenBoxes = new double[NumberOfBoxes + GridLines];
      WFlowBetweenBoxes = new double[NumberOfBoxes];
      a = new double[NumberOfBoxes + GridLines];
      b = new double[NumberOfBoxes + GridLines];
      c = new double[NumberOfBoxes + GridLines];
      r = new double[NumberOfBoxes + GridLines];
      atemporary = new double[NumberOfBoxes + GridLines];
      btemporary = new double[NumberOfBoxes + GridLines];
      ctemporary = new double[NumberOfBoxes + GridLines];
      rtemporary = new double[NumberOfBoxes + GridLines];
		gam = new double[NumberOfBoxes + GridLines];
		u = new double[NumberOfBoxes + GridLines];
      BoxWidth = new double[NumberOfBoxes + GridLines];
      BoxCrossSectionalArea = new double[NumberOfBoxes + GridLines];
      TotalDepth = new double[GridColumns];
      TotalDepthPast = new double[GridColumns];
      OldBoxDepth = new double[NumberOfBoxes];
      OldBoxWidth = new double[NumberOfBoxes + GridLines];
      UU1 = new double[NumberOfBoxes + GridLines];
      UU2 = new double[NumberOfBoxes + GridLines];
      UU3 = new double[NumberOfBoxes + GridLines];
      UW = new double[NumberOfBoxes + GridLines];
      Soma0 = new double[GridColumns + 1];
      Soma1 = new double[GridColumns]; Soma2 = new double[GridColumns];
      Soma3 = new double[GridColumns + 1]; Soma4 = new double[GridColumns + 1];
      Soma5 = new double[GridColumns + 1]; Soma6 = new double[GridColumns + 1];
      Soma7 = new double[GridColumns + 1]; Soma8 = new double[GridColumns + 1];
      Soma9 = new double[GridColumns];

      aColumns = new double[GridColumns]; bColumns = new double[GridColumns]; cColumns = new double[GridColumns];
      rColumns = new double[GridColumns];
      MeanColumnWidth = new double[GridColumns];
      MeanColumnLength = new double[GridColumns];
      /*CaudalPorColunaPassado = new double[GridColumns + 1];
      CaudalPorColunaActual = new double[GridColumns + 1];
      AlturaEstimada = new double[GridColumns]; */
   }


   for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   {
   	atemporary[i] = 0.0; btemporary[i] = 0.0; ctemporary[i] = 0.0;
      //HorizontalDiffCoefU[i] = 0.0;
   	if (i < NumberOfBoxes)
      {
   		BoxDepth[i] = MyPEcoDynClass->GetBoxDepth(i);
        OldBoxDepth[i] = BoxDepth[i];
      	WaterDensity[i] = 1000.0;
      	BoxArray[i].BoxToBoundary[1].TypeOfBoundary = 0;
      	BoxArray[i].BoxToBoundary[2].TypeOfBoundary = 0;
      	BoxArray[i].BoxToBoundary[3].TypeOfBoundary = 0;
      	BoxArray[i].BoxToBoundary[4].TypeOfBoundary = 0;
      	WVelocity[i] = 0;
      	WVelocityPast[i] = 0;
      	WFlowBetweenBoxes[i] = 0;

         if (i > NumberOfBoxes - GridColumns - 1)
         {
         	SurfaceBox[GridColumns - (NumberOfBoxes - i)] = i;
            //Debugger(GridColumns - (NumberOfBoxes - i));
            //Debugger(SurfaceBox[GridColumns - (NumberOfBoxes - i)]);
         }
      }
      if (i <= GridColumns)
      {
      	Soma0[i] = 0.0;
      	Soma3[i] = 0.0; Soma4[i] = 0.0; Soma5[i] = 0.0;
         Soma6[i] = 0.0; Soma7[i] = 0.0; Soma8[i] = 0.0;
      }
      if (i < GridColumns)
      {
      	Soma1[i] = 0.0; Soma2[i] = 0.0; Soma9[i] = 0.0;
      	TotalDepth[i] = 0.0;
         TotalDepthPast[i] = 0.0;
         aColumns[i] = 0.0;
         bColumns[i] = 0.0;
         cColumns[i] = 0.0;
         rColumns[i] = 0.0;
         MeanColumnWidth[i] = 0.0;
         MeanColumnLength[i] = 0.0;
         //AlturaEstimada[i] = 0.0;
      }
      UU1[i] = 0.0; UU2[i] = 0.0; UU3[i] = 0.0; UW[i] = 0.0;
   }


	//Variables file
	char MyVariable[65];
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Array for state variable names created in TOneDimensionalDITransport

            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y , MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
                if (strcmp(MyVariable, "U Velocity") == 0)
                {
                    if (!PReadWrite->FindString("U Velocity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, UVelocity[j]);
                        UVelocityPast[j] = UVelocity[j];
                        //if (j == NumberOfBoxes + GridLines - 1) Debugger(UVelocity[j]);
                    }
                }
                else if (strcmp(MyVariable, "W Velocity") == 0)
                {
                    if (!PReadWrite->FindString("W Velocity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, WVelocity[j]);
                        WVelocityPast[j] = WVelocity[j];
                        //if (j == NumberOfBoxes-1) Debugger(WVelocity[j]);
                    }
                }
                else if (strcmp(MyVariable, "Dynamic height") == 0)
                {
                    if (!PReadWrite->FindString("Dynamic height values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DynamicHeight[j]);
                        PastHeight[j] = DynamicHeight[j];
                   //if (j == NumberOfBoxes-1) Debugger(DynamicHeight[j]);
                    }
                }
                else if (strcmp(MyVariable, "U Flow") == 0)
                {
                    if (!PReadWrite->FindString("U Flow values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, UFlowBetweenBoxes[j]);
                        //if (j == NumberOfBoxes + GridLines - 1) Debugger(UFlowBetweenBoxes[j]);
                    }
                }
                else if (strcmp(MyVariable, "W Flow") == 0)
                {
                    if (!PReadWrite->FindString("W Flow values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, WFlowBetweenBoxes[j]);
                        //if (j == NumberOfBoxes-1) Debugger(WFlowBetweenBoxes[j]);
                    }
                }
                else if (strcmp(MyVariable, "Flow discharge") == 0)
                {
                    if (!PReadWrite->FindString("Flow discharge values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, FlowDischarge[j]);
                   //Debugger(FlowDischarge[j]);
                    }
                }
                else if (strcmp(MyVariable, "Flow uptake") == 0)
                {
                    if (!PReadWrite->FindString("Flow uptake values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, FlowUptake[j]);
                   //Debugger(FlowUptake[j]);
                    }
                }
                else if (strcmp(MyVariable, "Salinity") == 0)
                {
                    if (!PReadWrite->FindString("Salinity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    PReadWrite->ReadNumber(XV, YV + 1, RiverSalt);
                    //Debugger(RiverSalt);
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 2 + j, Salt[j]);
                        /*if (j == NumberOfBoxes-1) Debugger(Salt[j]); */
                    }
                }
                else if (strcmp(MyVariable, "Box Width") == 0)
                {
                    if (!PReadWrite->FindString("Box Width values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoxWidth[j]);
                        OldBoxWidth[j] = BoxWidth[j];
                        //if (j == NumberOfBoxes+ GridLines-1) Debugger(BoxWidth[j]);
                    }
                }
                else
                if (strcmp(MyVariable, "Water density") == 0)
                {
                    if (!PReadWrite->FindString("Water density values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, WaterDensity[j]);
                   //if (j == 1) Debugger(WaterDensity[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalVertical::TBiDimensionalVertical - variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }

   /*for (int j = 0; j < GridColumns; j++)
   {
      for (int i = 0; i < GridLines; i++)
      {
        if (BoxWidth[i * (GridColumns + 1) + j] <= CriticalWidth)
        BoxWidth[i *(GridColumns + 1) + j] = CriticalWidth;
        if ((BoxWidth[i * (GridColumns + 1) + j] <= 0.0) &&  (BoxWidth[i * (GridColumns + 1) + j + 1] <= 0.0))
        BoxDepth[i * GridColumns + j] = 0.0; OldBoxDepth[i * GridColumns + j] = 0.0;
      }
   } */
   for (int j = 0; j < GridColumns + 1; j++)
   {
      for (int i = 0; i < GridLines; i++)
      {
        if (BoxWidth[i * (GridColumns + 1) + j] <= CriticalWidth)
        BoxWidth[i *(GridColumns + 1) + j] = 0.0;
      }
   }

   for (int j = 0; j < GridColumns; j++)
   {
      for (int i = 0; i < GridLines; i++)
      {
        if ((BoxWidth[i * (GridColumns + 1) + j] <= 0.0) &&  (BoxWidth[i * (GridColumns + 1) + j + 1] <= 0.0))
        {
            BoxDepth[i * GridColumns + j] = 0.0; OldBoxDepth[i * GridColumns + j] = 0.0;
        }    
      }
   }
   for (int i = 0; i < GridLines; i++)
   {
      BoxArray[i * GridColumns].BoxToBoundary[UpstreamBoundaryNumber].TypeOfBoundary = 1;
      BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[DownstreamBoundaryNumber].TypeOfBoundary = 1;
   }
   double  DownStreamDepth, UpStreamDepth;

   for (int j = 0; j < GridColumns; j++)
   {
   	double SomaWidth = 0.0; double SomaLength = 0.0; int TrueGridLines = 0.0;

   	for (int i = 0; i < GridLines; i++)
      {
      	 if (BoxDepth[i * GridColumns + j] > CriticalDepth)
          {

   			TotalDepthPast[j] = TotalDepthPast[j] + BoxDepth[i * GridColumns + j];
            TrueGridLines++;
            //SomaWidth = SomaWidth + (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]) / 2.0 * BoxDepth[i * GridColumns + j];
            SomaWidth = SomaWidth + BoxWidth[i * (GridColumns + 1) + j] * BoxDepth[i * GridColumns + j];
            SomaLength = SomaLength + BoxLength[i * GridColumns + j];
            BoxVolume[i * GridColumns + j] = 0.5 *
                                             (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]) *
                                             BoxDepth[i * GridColumns + j] *
                                             BoxLength[i * GridColumns + j];

            //if (i == GridLines - 1) Debugger (BoxVolume[i * GridColumns + j]);

            PastBoxVolume[i * GridColumns + j] = BoxVolume[i * GridColumns + j];
      	}
      }
      TotalDepth[j] = TotalDepthPast[j];
      //MeanColumnWidth[j] = SomaWidth / TrueGridLines;
      MeanColumnWidth[j] = SomaWidth / TotalDepth[j];
      MeanColumnLength[j] = SomaLength / TrueGridLines;
   }
	//ATimeStep = MyPEcoDynClass->GetTimeStep() * DAYSTOSECONDS;
   //Debugger(ATimeStep);
}

TBiDimensionalVertical::~TBiDimensionalVertical()
{
    freeMemory();
}

void TBiDimensionalVertical::freeMemory()
{
/*
 *  ? PRECISO VERIFICAR BEM OS newS E OS deleteS DESTA CLASSE
 *
 */
    try {
        if (NumberOfBoxes > 0)
        {
          delete [] WVelocity;
          delete [] WVelocityPast;
          delete [] HorizontalDiffCoefU;
          delete [] VerticalDiffCoefU;
          delete [] VerticalMassAndHeatDiffCoef;
          delete [] Rugosity;
    //      delete [] PastGeneric;
          delete [] a;
          delete [] b;
          delete [] c;
          delete [] atemporary;
          delete [] btemporary;
          delete [] ctemporary;
          delete [] rtemporary;
          delete [] r;
          delete [] gam;
          delete [] u;
          delete [] BoxWidth;
          delete [] TotalDepth;
          delete [] TotalDepthPast;
          delete [] BoxCrossSectionalArea;
          delete [] MassDiffusion;
          delete [] MassConvection;
          delete [] ConvectionValue;
          delete [] DiffusionValue;
          //delete [] BottomDragValue;
          delete [] BarotropicPressureValue;
          delete [] BaroclinicPressureValue;
          delete [] UU1;
          delete [] UU2;
          delete [] UU3;
          delete [] UW;
          delete [] Soma0; delete [] Soma1; delete [] Soma2; delete [] Soma3; delete [] Soma4; delete [] Soma5; delete [] Soma6;
          delete [] Soma7; delete [] Soma8; delete [] Soma9;
          delete [] aColumns;
          delete [] bColumns;
          delete [] cColumns;
          delete [] rColumns;
          delete [] MeanColumnWidth;
          delete [] MeanColumnLength;
          delete [] SurfaceBox;
          delete [] SurfaceLayer;
          delete [] OldBoxDepth;
          delete [] OldBoxWidth;
          delete [] DepthVariation;
          /*delete [] CaudalPorColunaPassado;
          delete [] CaudalPorColunaActual;
          delete [] AlturaEstimada;*/
          //delete [] CellVolume;
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
        cerr << "TBiDimensionalVertical::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__

}


double TBiDimensionalVertical::GetParameterValue(char* MyParameter)
{
  double value;


    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        value = Gravity;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        value = CoefManning[0];
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        value = HorizontalDiffCoef;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        value = BoxWidth[0];
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        value = StartYear;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        value = kdiffusion;
    }

    // parameters from TBiDimensionalVertical
    else if (strcmp(MyParameter, "Critical depth") == 0)
    {
        value = CriticalDepth;
    }
    else if (strcmp(MyParameter, "Rugosity") == 0)
    {
        value = Rugosity[0];
    }
    else if (strcmp(MyParameter, "HMINC") == 0)
    {
        value = HMINC;
    }
    else if (strcmp(MyParameter, "Horizontal diffusion coefficient") == 0)
    {
        value = HorizontalDiffCoefU[0];
    }
    else if (strcmp(MyParameter, "Wind stress coeficient") == 0)
    {
        value = WindStressCoef;
    }
    else if (strcmp(MyParameter, "Critical width") == 0)
    {
        value = CriticalWidth;
    }
    else if (strcmp(MyParameter, "Maximum vertical momentum diffusivity") == 0)
    {
        value = MaxVertDiffusivity;
    }
    else
        value = 0.0;
    return value;
}

bool TBiDimensionalVertical::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        Gravity = value;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
        {
            CoefManning[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        HorizontalDiffCoef = value;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        for (j = 0; j < NumberOfBoxes + 1; j++)
        {
            BoxWidth[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        StartYear = value;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        kdiffusion = value;
    }

    // parameters from TBiDimensionalVertical
    else if (strcmp(MyParameter, "Critical depth") == 0)
    {
        CriticalDepth = value;
    }
    else if (strcmp(MyParameter, "Rugosity") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
        {
            Rugosity[j] = value;
        }
    }
    else if (strcmp(MyParameter, "HMINC") == 0)
    {
        HMINC = value;
    }
    else if (strcmp(MyParameter, "Horizontal diffusion coefficient") == 0)
    {
        for (j = 0; j < NumberOfBoxes + GridLines; j++)
        {
            HorizontalDiffCoefU[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Wind stress coeficient") == 0)
    {
        WindStressCoef = value;
    }
    else if (strcmp(MyParameter, "Critical width") == 0)
    {
        CriticalWidth = value;
    }
    else if (strcmp(MyParameter, "Maximum vertical momentum diffusivity") == 0)
    {
        MaxVertDiffusivity = value;
    }
    else
        rc = false;

    return rc;
}
// AP, 2007.07.09
bool TBiDimensionalVertical::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Transport");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Transport");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);

    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("U Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("U Flow values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Box Width values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Salinity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("W Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Dynamic height values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("W Flow values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow discharge values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow uptake values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
    {
        PReadWrite->WriteCell(UVelocity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(UFlowBetweenBoxes[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoxWidth[j], 8);
        if ( j > NumberOfBoxes)
        {
            PReadWrite->WriteSeparator(true);
            continue;
        }
        PReadWrite->WriteSeparator();
        if (j == 0)
            PReadWrite->WriteCell(RiverSalt, 8);
        else
            PReadWrite->WriteCell(Salt[j - 1], 8);
        if (j == NumberOfBoxes)
        {
            PReadWrite->WriteSeparator(true);
            continue;
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WVelocity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DynamicHeight[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WFlowBetweenBoxes[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowDischarge[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowUptake[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WaterDensity[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TBiDimensionalVertical::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Transport");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Transport");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();

    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(ParametersNameArray[i]);

        // TOneDimensionalDITransport
        if (strcmp(ParametersNameArray[i], "Gravity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Gravity, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Difusion coeficient") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(HorizontalDiffCoef, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Start Year") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(StartYear, 0);
        }
        else if (strcmp(ParametersNameArray[i], "kdiffusion") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(kdiffusion, 6);
        }

        // TBiDimensionalVertical
        else if (strcmp(ParametersNameArray[i], "Critical depth") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(CriticalDepth, 6);
        }
        else if (strcmp(ParametersNameArray[i], "HMINC") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(HMINC, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Wind stress coeficient") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(WindStressCoef, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Critical width") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(CriticalWidth, 6);
        }
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteCell("Horizontal diffusion coefficient values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Box Width values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Coeficiente de Manning values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Rugosity values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < (NumberOfBoxes + GridLines); j++)
    {
        PReadWrite->WriteCell(HorizontalDiffCoefU[j], 6);
        if (j > NumberOfBoxes)
        {
            PReadWrite->WriteSeparator(true);
            continue;
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoxWidth[j], 6);
        if (j == NumberOfBoxes)
        {
            PReadWrite->WriteSeparator(true);
            continue;
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(CoefManning[j], 6);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WaterDensity[j], 6);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Rugosity[j], 6);
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);
    return true;
}

void TBiDimensionalVertical::EquationOfMotion()
{
   double DamDischarge, WindEffect;

   for (int i = 0; i <= UpperLayer; i++)
   {
   	  for (int j = 0; j < GridColumns + 1; j++)
      {
         if (
         		(j == 0) && (BoxDepth[i * GridColumns + j] > CriticalDepth) && (BoxWidth[i * (GridColumns + 1) + j] > 0.0)||
               (
               	(j > 0) && (j <= GridColumns) /*&&
                  (BoxDepth[i * GridColumns + j - 1] > CriticalDepth)*/ &&
                  (BoxDepth[i * GridColumns + j] > CriticalDepth) && (BoxWidth[i * (GridColumns + 1) + j] > 0.0)
            	)
         	)
         {

            if ((j == 0)  || (j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth))
            {
            	atemporary[i * (GridColumns + 1) + j] = 0.0;
            	btemporary[i * (GridColumns + 1) + j] = 1.0;
            	ctemporary[i * (GridColumns + 1) + j] = 0.0;
                rtemporary[i * (GridColumns + 1) + j] = UVelocity[i * (GridColumns + 1) + j];
            }
            else
            if (j == GridColumns)
            {
               atemporary[i * (GridColumns + 1) + j] = 0.0;
               btemporary[i * (GridColumns + 1) + j] = 1.0;
               ctemporary[i * (GridColumns + 1) + j] = 0.0;
               rtemporary[i * (GridColumns + 1) + j] = UVelocity[i * (GridColumns + 1) + j];
            }
            else
            if (
            		(j > 0) && (j < GridColumns)
               )
            {
               atemporary[i * (GridColumns + 1) + j] = aUCalculation(i,j);
               btemporary[i * (GridColumns + 1) + j] = bUCalculation(i,j);
               ctemporary[i * (GridColumns + 1) + j] = cUCalculation(i,j);
               BaroclinicPressureValue[i * GridColumns + j] = BaroclinicPressureTerm(i,j);
               BarotropicPressureValue[i * GridColumns + j] = BarotropicPressureTerm(i,j);
               ConvectionValue[i * GridColumns + j] = ConvectiveTerm(i,j);
               DiffusionValue[i * GridColumns + j] = HorizontalDiffusion(i,j);

               if (
      					(i * GridColumns + j == SurfaceBox[j])
         			)
         			WindEffect = WindStress(i * GridColumns + j) * ATimeStep /
                               (
                               	0.5 * (WaterDensity[i * GridColumns + j] + WaterDensity[i * GridColumns + j - 1]) *
                                 0.5 * (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1])
                               );
               else WindEffect = 0.0;

               rtemporary[i * (GridColumns + 1) + j] = UVelocityPast[i * (GridColumns + 1) + j]
                                                       - ConvectionValue[i * GridColumns + j]
                 	                                   - BarotropicPressureValue[i * GridColumns + j]
                                                       - BaroclinicPressureValue[i * GridColumns + j]
                    	                               + DiffusionValue[i * GridColumns + j]
                                                       + WindEffect;
            }
         }
   	   }
	}
}

void TBiDimensionalVertical::Continuity()
{
   int BottomBox;
   //BottomBox = 0;
   for (int j = 0; j <= GridColumns; j++)
   {
   	Soma0[j] = 0.0;
   	Soma3[j] = 0.0; Soma4[j] = 0.0;
      Soma5[j] = 0.0; Soma6[j] = 0.0; Soma7[j] = 0.0; Soma8[j] = 0.0;
      if (j < GridColumns)  //Há GridColumns + 1 velocidades, mas somente GridColumns alturas
      {
      	Soma1[j] = 0.0; Soma2[j] = 0.0; Soma9[j] = 0.0;
      	aColumns[j] = 0.0; bColumns[j] = 0.0; cColumns[j] = 0.0; rColumns[j] = 0.0;
      }
   }


   for (int j = 0; j <= GridColumns; j++)
   {
      BottomBox = 0;
      for (int i = 0; i <= UpperLayer; i++)  //Cálculo de somas de produtos por colunas
      {
      	if (
         		(j < GridColumns) && (BoxDepth[i * GridColumns + j] <= CriticalDepth)  ||
               (j == GridColumns) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)
            )
         {
            BottomBox++;
         }
         else
         {
         if (
         		(j < GridColumns) && (BoxDepth[i * GridColumns + j] > CriticalDepth) && (BoxWidth[i * (GridColumns + 1) + j] > 0.0) ||
               (j == GridColumns) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (BoxWidth[i * (GridColumns + 1) + j] > 0.0)
            )
         {

         	 //Somatórios para cálculo dos termos implícitos

             //Este termo é zero no caso da célula montante não existir ou ser terra
             //porque não há gradiente de pressão e este termo resulta do referido gradiente
            if (j < GridColumns)  //Estes termos são tantos quantos as GridColumns
            {
         		if  ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth))
            		Soma1[j] = Soma1[j] + BoxWidth[i * (GridColumns + 1) + j] *
            			       (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]);

            //Este termo é zero no caso da célula juzante não existir ou ser terra
             //porque não há gradiente de pressão e este termo resulta do referido gradiente
            	if ((j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] > CriticalDepth))
            		Soma2[j] = Soma2[j] + BoxWidth[i * (GridColumns + 1) + j + 1] *
            			       (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j + 1]);
            }


            //Somatórios para cálculo dos termos explícitos
            if (
            		(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns)
               )
               Soma3[j] = 0.0;/*Soma3[j] + UVelocityPast[i * (GridColumns + 1) + j] * BoxWidth[i * (GridColumns + 1) + j] *
            		       (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]); */  //Limite montante ou fronteira sólida a montante
            else if (j < GridColumns)
            {
               Soma3[j] = Soma3[j] +
                         UVelocity[i * (GridColumns + 1) + j] * BoxWidth[i * (GridColumns + 1) + j] *
            		       (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j]) +
                          UVelocity[i * (GridColumns + 1) + j] * BoxWidth[i * (GridColumns + 1) + j] *
            		       (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j]);
             //Uso aqui 2X a velocidade actual no pressuposto de que a velocidade do afluente
             //permance constante durante o intervalo de tempo. Aplico o mesmo princípio ao
             //efluente nas linhas seguintes.
               //Debugger(Soma3[j]);
            }
            else if (j == GridColumns) //Limite juzante
            {
               Soma3[j] = Soma3[j] +
                          UVelocity[i * (GridColumns + 1) + j] * BoxWidth[i * (GridColumns + 1) + j] *
            		       (BoxDepth[i * GridColumns + j - 1] + BoxDepth[i * GridColumns + j - 1]) +
                          UVelocity[i * (GridColumns + 1) + j] * BoxWidth[i * (GridColumns + 1) + j] *
            		       (BoxDepth[i * GridColumns + j - 1] + BoxDepth[i * GridColumns + j - 1]);
               //Este produto pela soma
               //da profundidade da caixa consigo própria é para manter a coerência com a divisão por 4
               //efectuada mais abaixo
               //A Soma3 é um dos termos da equação do transporte quando se trata de uma célula normal
               //e é a velocidade centrada no tempo na fronteira montante quando se trate de uma célula com fronteira
               //sólida a montante
               //Debugger(Soma3[j]);
            }


            if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns))
            {
            	Soma0[j] = Soma0[j] + UVelocityPast[i * (GridColumns + 1) + j] *
                                                (
            	                                    BoxWidth[i * (GridColumns + 1) + j] *
                                                   (
               	                                    BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
                                                    )
                                                 );

            }
            else
            	Soma0[j] = 0.0;

            ConvectiveTerm(i,j);
            //Termo Uij-1

            if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns))
            	UU1[i * (GridColumns + 1) + j] = UU1[i * (GridColumns + 1) + j] *
                                                (
            	                                    BoxWidth[i * (GridColumns + 1) + j] *
                                                   (
               	                                    BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
                                                    )
                                                 );


            else
            	UU1[i * (GridColumns + 1) + j] = 0.0;
            //Termo Uij
            if (
   					(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns)
      			)
            		UU2[i * (GridColumns + 1) + j] = UU2[i * (GridColumns + 1) + j] *
                                                (
            	                                    BoxWidth[i * (GridColumns + 1) + j] *
                                                   (
               	                                    BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
                                                    )
                                                 );
            else
                 UU2[i * (GridColumns + 1) + j] = 0.0;

            //Termo Uij+1
            if (
   					(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns)
      			)
            		UU3[i * (GridColumns + 1) + j] = UU3[i * (GridColumns + 1) + j] *
                                                (
            	                                    BoxWidth[i * (GridColumns + 1) + j] *
                                                   (
               	                                    BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
                                                    )
                                                 );
            else
                 UU3[i * (GridColumns + 1) + j] = 0.0;
            //Esta multiplicação destina-se a anular a divisão pelo termo multiplicado
            //efectuada no void ConvectiveTerm(i,j). Acontece que na equação dos Us
            //a referida divisão é necessária, mas não na equação da continuidade.
            //Deste modo, pode usar-se o mesmo void para resolver a equação do momento
            // e a da continuidade tendo só o cuidado de efectuar esta correção

            if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns))
            {
             /* AP, 2005.05.19 ...
               double MeanWidth1, MeanWidth2;
               if (i < UpperLayer)
      				MeanWidth1 = (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j]) / 2.0; //i+1/2j
      			else
      				MeanWidth1 = BoxWidth[i * (GridColumns + 1) + j];

      			if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepth))
      				MeanWidth2 = (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j]) / 2.0; //i-1/2j
      			else
      				MeanWidth2 = BoxWidth[i * (GridColumns + 1) + j];
               */

               Soma4[j] = Soma4[j] + (UU2[i * (GridColumns + 1) + j] - UU1[i * (GridColumns + 1) + j] + UU3[i * (GridColumns + 1) + j]) /
                                   (4.0 * MeanColumnWidth[j] * MeanColumnLength[j] /** MeanColumnLength[j]*/)
                                   + BoxWidth[i * (GridColumns + 1) + j] * (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1])
                                  // /(MeanWidth1 + MeanWidth2)
                                   * UW[i * (GridColumns + 1) + j] /
                                   (4.0 * MeanColumnWidth[j] * MeanColumnLength[j] /** MeanColumnLength[j]*/);

            }
            else
               Soma4[j] = 0.0;


            if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns))
            {
             	Soma5[j] = Soma5[j] + BoxWidth[i * (GridColumns + 1) + j] *
                          (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]) *
                          BaroclinicPressureTerm(i,j) /
                          (4.0 * MeanColumnWidth[j] * MeanColumnLength[j]);

            }
            else
               Soma5[j] = 0.0;

            if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns))
            {
               Soma6[j] = Soma6[j] + BoxWidth[i * (GridColumns + 1) + j] *
                  	        (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]) *
                     	     HorizontalDiffusion(i,j) /
                             (4.0 * MeanColumnWidth[j] * MeanColumnLength[j]);
            }
            else
            {
               Soma6[j] = 0.0;
            }

            if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) && (j < GridColumns))
            {
            	Soma7[j] = Soma7[j] + UVelocityPast[i * (GridColumns + 1) + j] * BoxWidth[i * (GridColumns + 1) + j] *
                  	                 (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]) /
                     	              (4.0 * MeanColumnWidth[j] * MeanColumnLength[j]);
            }
            else
            	Soma7[j] = 0.0;


            if (j < GridColumns)
            Soma9[j] = Soma9[j] + (FlowDischarge[i * GridColumns + j] - FlowUptake[i * GridColumns + j]);
         }

         }
         //Debugger(i);
      }  //i
      //Termos implícitos da equação da continuidade para cálculo da elevação
      if (j < GridColumns)  //Estes termos são tantos quantas as GridColumns
      {
      	aColumns[j] = - Gravity * ATimeStep  * ATimeStep * Soma1[j] /
                      (4.0 * MeanColumnWidth[j] * MeanColumnLength[j] * MeanColumnLength[j]);

      	bColumns[j] = 1 + Gravity * ATimeStep * ATimeStep / MeanColumnLength[j] *
                    (
                    		Soma1[j] / (4.0 * MeanColumnWidth[j] * MeanColumnLength[j]) +
                        Soma2[j] / (4.0 * MeanColumnWidth[j] * MeanColumnLength[j])
                    );
      	if (j < GridColumns - 1)
      	cColumns[j] = - Gravity * ATimeStep * ATimeStep * Soma2[j] /
                      (4.0 * MeanColumnWidth[j] * MeanColumnLength[j] * MeanColumnLength[j]);
      }
      // Somas para termos explícitos
      if (j < GridColumns)
      {

      	Soma0[j] = Soma0[j] * ATimeStep / (4.0 * MeanColumnLength[j] * MeanColumnWidth[j]);
      	Soma3[j] = Soma3[j] * ATimeStep / (4.0 * MeanColumnLength[j] * MeanColumnWidth[j]);
        Soma9[j] = Soma9[j] * ATimeStep /(MeanColumnWidth[j] * MeanColumnLength[j]);
      }
      else
         Soma3[j] = Soma3[j] * ATimeStep / (4.0 * MeanColumnLength[j - 1] * MeanColumnWidth[j - 1]);

      Soma4[j] = Soma4[j] * ATimeStep;
      Soma5[j] = Soma5[j] * ATimeStep /** ATimeStep * Gravity*/;                   //20/7/2004
      Soma6[j] = Soma6[j] * ATimeStep;
      Soma7[j] = Soma7[j] * ATimeStep;
      //if ((j > 0) && (BoxDepth[j - 1] > CriticalDepth) && (j < GridColumns) && (BoxWidth[j] > 0.0))
      if (
           (j > 0)
           && (BoxDepth[BottomBox * GridColumns + j - 1] > CriticalDepth)          //20/7/2004
           && (j < GridColumns)
           && (BoxWidth[BottomBox * (GridColumns + 1) + j] > 0.0)
         )
      {

      	/*Soma8[j] = Drag(BottomBox,j) *
                        ATimeStep * BoxWidth[j] /
                        (4.0 * MeanColumnWidth[j] * MeanColumnLength[j]);*/
          Soma8[j] = Drag(BottomBox,j) * (BoxDepth[BottomBox * GridColumns + j] + BoxDepth[BottomBox * GridColumns + j - 1])*
                        ATimeStep * BoxWidth[BottomBox * (GridColumns + 1) + j] /
                        (4.0 * MeanColumnWidth[j] * MeanColumnLength[j]);

      }
      else
         Soma8[j] = 0.0;

      int Top, ALayer;
      if (j < GridColumns){
        Top = SurfaceBox[j];
        ALayer = (Top - j) / GridColumns;
      }

      if ((j > 0) && (BoxDepth[Top] > CriticalDepth) && (j < GridColumns))
      {

         Soma8[j] = Soma8[j] - WindStress(Top) * BoxWidth[ALayer * (GridColumns + 1) + j] *
                               (BoxDepth[ALayer * GridColumns + j] + BoxDepth[ALayer * GridColumns + j - 1]) *
                               pow(ATimeStep,2) /
                               (
                               	0.5 * (WaterDensity[ALayer * GridColumns + j] + WaterDensity[ALayer * GridColumns + j - 1]) *
                                 4.0 * MeanColumnWidth[j] * MeanColumnLength[j]
                               ) *
                               0.5 * (BoxDepth[ALayer * GridColumns + j] + BoxDepth[ALayer * GridColumns + j - 1]);

   	}
   }

   //Termo explícito
   for (int j = 0; j < GridColumns; j++)
   {

   	rColumns[j] = (PastHeight[UpperLayer * GridColumns + j] + Soma0[j] + Soma3[j] - Soma4[j] - Soma5[j] + Soma6[j] + Soma7[j] - Soma8[j]) -
      	           (Soma0[j + 1] + Soma3[j + 1] - Soma4[j + 1] - Soma5[j + 1] + Soma6[j + 1] + Soma7[j + 1] - Soma8[j + 1])
                    + Soma9[j];
      a[j] = aColumns[j];
      b[j] = bColumns[j];
      c[j] = cColumns[j];
      r[j] = rColumns[j];

      u[j] = 0.0;
   }
   //IntegrationTridag(GridColumns);
   IntegrationTridag(a, b, c, r, GridColumns);
   mprovetridag(a, b, c, r, u, GridColumns);
   for (int j = 0; j < GridColumns; j++)
   {
   	  DynamicHeight[UpperLayer * GridColumns + j] = u[j];
   }
   //AdvectDiffuse();

}

void TBiDimensionalVertical::Inquiry(char* srcName,double &Value,
                                         int BoxNumber,
                                         char* ParameterName,
                                         int AnObjectCode)
{

	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
    int LineNumber = GetLineIndex(MyBoxNumber);
    int ColumnNumber = GetColumnIndex(MyBoxNumber);
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "U Velocity") == 0)
    {
        Value = UVelocity[LineNumber * (GridColumns + 1) + ColumnNumber]; //m / s
    }
    else if (strcmp(MyParameter, "W Velocity") == 0)
        Value = -WVelocity[LineNumber * GridColumns + ColumnNumber]; //m / s A velocidade W é devolvida com sinal menos para que o Noesys
    else if (strcmp(MyParameter, "U Flow") == 0)
    {
         Value = UFlowBetweenBoxes[LineNumber * (GridColumns + 1) + ColumnNumber];
    }
    else if (strcmp(MyParameter, "W Flow") == 0)
    {
        Value = -WFlowBetweenBoxes[LineNumber * GridColumns + ColumnNumber];  // m3 / s
    }
    else if (strcmp(MyParameter, "Box volume") == 0)
    {
        Value = BoxVolume[LineNumber * GridColumns + ColumnNumber];  // m3
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        Value = BoxWidth[LineNumber * (GridColumns + 1) + ColumnNumber];  // m
    }
    else if (strcmp(MyParameter, "Past box volume") == 0)
    {
        Value = PastBoxVolume[LineNumber * GridColumns + ColumnNumber];  // m3
    }
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
    {
        if (
                (ColumnNumber < GridColumns) /*&& (LineNumber == 0)*/ &&
              (BoxDepth[LineNumber * GridColumns + ColumnNumber] > CriticalDepth)
           )
                Value = CoefRugosidade(LineNumber,ColumnNumber);
        else
            Value = 0.0;
    }
    else if (strcmp(MyParameter, "Horizontal diffusion coefficient") == 0)
    {
        Value = HorizontalDiffCoefU[LineNumber * (GridColumns + 1) + ColumnNumber];  // m2 s-1
    }
    else if  (strcmp(MyParameter,"Total box depth") == 0)
        Value = Depth(LineNumber,ColumnNumber) /*+ (DynamicHeight[MyBoxNumber] - PastHeight[MyBoxNumber])*/;
    else if  (strcmp(MyParameter,"Total depth") == 0)
        Value = TotalDepthPast[ColumnNumber] /*+ (DynamicHeight[MyBoxNumber] - PastHeight[MyBoxNumber])*/;
    else if (strcmp(MyParameter, "Dynamic height") == 0)
        Value = DynamicHeight[MyBoxNumber];
    else if (strcmp(MyParameter, "Salinity") == 0)
        Value = Salt[MyBoxNumber];
    else if (strcmp(MyParameter, "Box depth") == 0)
        Value = BoxDepth[MyBoxNumber] /*+ (DynamicHeight[MyBoxNumber] - PastHeight[MyBoxNumber])*/;
    else if (strcmp(MyParameter, "Vertical diffusivity") == 0)
        Value = VerticalDiffCoefU[LineNumber * (GridColumns + 1) + ColumnNumber];
    else if (strcmp(MyParameter,"Water density") == 0)
        Value = WaterDensity[MyBoxNumber];
    else if (strcmp(MyParameter,"Flow discharge") == 0)
        Value = FlowDischarge[MyBoxNumber];
    else if (strcmp(MyParameter,"Flow uptake") == 0)
        Value = FlowUptake[MyBoxNumber];
    else if (strcmp(MyParameter,"Columns") == 0)
        Value = float(ColumnNumber + 1.0); //A adição de 1 é para ter o número de colunas 1-based
    else if (strcmp(MyParameter,"Lines") == 0)
        Value = float(GridLines - LineNumber);//A subtracção entre () destina-se a inverter o referencial vertical
                                              //para compatibilidade com o Noesys
    else if (strcmp(MyParameter,"Barotropic pressure") == 0)
        Value = BarotropicPressureValue[MyBoxNumber];
    else if (strcmp(MyParameter,"Baroclinic pressure") == 0)
        Value = BaroclinicPressureValue[MyBoxNumber];
    else if (strcmp(MyParameter,"Convection of momentum") == 0)
        Value = ConvectionValue[MyBoxNumber];
    else if (strcmp(MyParameter,"Diffusion of momentum") == 0)
        Value = DiffusionValue[MyBoxNumber];
    else if (strcmp(MyParameter,"Bottom drag") == 0)
    {
        if (BoxWidth[LineNumber * (GridColumns + 1) + ColumnNumber] > 0.0)
            Value = Drag(LineNumber, ColumnNumber);
        else
            Value = -99;
    }
    else if (strcmp(MyParameter,"Diffusion") == 0)
        Value = MassDiffusion[MyBoxNumber];
    else if (strcmp(MyParameter,"Convection") == 0)
        Value = MassConvection[MyBoxNumber];
    else if (strcmp(MyParameter,"Upper layer") == 0)
        Value = UpperLayer;
    else if (strcmp(MyParameter,"LengthScale") == 0)
    {
        if ((BoxDepth[LineNumber * GridColumns + ColumnNumber] > CriticalDepth) && (LineNumber > 0))
            Value = LengthScale(LineNumber, ColumnNumber);
        else Value = -99;
    }
    else if (strcmp(MyParameter,"RichardsonNumber") == 0)
    {
        if ((BoxDepth[LineNumber * GridColumns + ColumnNumber] > CriticalDepth) && (LineNumber > 0))
            Value = RichardsonNumber(LineNumber, ColumnNumber);
        else Value = -99;
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
            MessageBox(0,
                          Caption,
                          "EcoDynamo Alert - Inquiry",
                          MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TBiDimensionalVertical::Inquiry - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, BoxNumber);
}


// AP, 2007.05.28
bool TBiDimensionalVertical::SetVariableValue(char* srcName,
                             double Value,
                             int BoxNumber,
                             char* VariableName)
{
    bool rc = true;
    int LineNumber = GetLineIndex(BoxNumber);
    int ColumnNumber = GetColumnIndex(BoxNumber);
    
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "U Velocity") == 0)
    {
        UVelocity[LineNumber * (GridColumns + 1) + ColumnNumber] = Value; //m / s
    }
    else if (strcmp(VariableName, "W Velocity") == 0)
    {
        WVelocity[LineNumber * GridColumns + ColumnNumber] = -Value; //m / s A velocidade W é devolvida com sinal menos para que o Noesys
    }
    else if (strcmp(VariableName, "U Flow") == 0)
    {
         UFlowBetweenBoxes[LineNumber * (GridColumns + 1) + ColumnNumber] = Value;
    }
    else if (strcmp(VariableName, "W Flow") == 0)
    {
        WFlowBetweenBoxes[LineNumber * GridColumns + ColumnNumber] = -Value;  // m3 / s
    }
    else if (strcmp(VariableName, "Box volume") == 0)
    {
        BoxVolume[LineNumber * GridColumns + ColumnNumber] = Value;  // m3
    }
    else if (strcmp(VariableName, "Box Width") == 0)
    {
        BoxWidth[LineNumber * (GridColumns + 1) + ColumnNumber] = Value;  // m
    }
    else if (strcmp(VariableName, "Past box volume") == 0)
    {
        PastBoxVolume[LineNumber * GridColumns + ColumnNumber] = Value;  // m3
    }
    else if (strcmp(VariableName, "Horizontal diffusion coefficient") == 0)
    {
        HorizontalDiffCoefU[LineNumber * (GridColumns + 1) + ColumnNumber] = Value;  // m2 s-1
    }
    else if  (strcmp(VariableName,"Total depth") == 0)
        TotalDepthPast[ColumnNumber] = Value;
    else if (strcmp(VariableName, "Dynamic height") == 0)
        DynamicHeight[BoxNumber] = Value;
    else if (strcmp(VariableName, "Salinity") == 0)
        Salt[BoxNumber] = Value;
    else if (strcmp(VariableName, "Box depth") == 0)
        BoxDepth[BoxNumber] = Value;
    else if (strcmp(VariableName, "Vertical diffusivity") == 0)
        VerticalDiffCoefU[LineNumber * (GridColumns + 1) + ColumnNumber] = Value;
    else if (strcmp(VariableName,"Water density") == 0)
        WaterDensity[BoxNumber] = Value;
    else if (strcmp(VariableName,"Flow discharge") == 0)
        FlowDischarge[BoxNumber] = Value;
    else if (strcmp(VariableName,"Flow uptake") == 0)
        FlowUptake[BoxNumber] = Value;
    else if (strcmp(VariableName,"Barotropic pressure") == 0)
        BarotropicPressureValue[BoxNumber] = Value;
    else if (strcmp(VariableName,"Baroclinic pressure") == 0)
        BaroclinicPressureValue[BoxNumber] = Value;
    else if (strcmp(VariableName,"Convection of momentum") == 0)
        ConvectionValue[BoxNumber] = Value;
    else if (strcmp(VariableName,"Diffusion of momentum") == 0)
        DiffusionValue[BoxNumber] = Value;
    else if (strcmp(VariableName,"Diffusion") == 0)
        MassDiffusion[BoxNumber] = Value;
    else if (strcmp(VariableName,"Convection") == 0)
        MassConvection[BoxNumber] = Value;
    else if (strcmp(VariableName,"Upper layer") == 0)
        UpperLayer = Value;
    else
        rc = false;

    return rc;
}

void TBiDimensionalVertical::Go()
{
   int k/*, TrueGridLines1, TrueGridLines2*/;
   double DamDischarge, RiverMeanWidth, DamMeanWidth, RiverDepth, DamDepth, RiverFlowTotal, DamDischargeTotal;
   //Debugger(MyPIntegrate->GetCurrentTime());
   TEcoDynClass* MyFlowPointer = MyPEcoDynClass->GetFlowPointer();
   RiverMeanWidth = 0.0; DamMeanWidth = 0.0; /*TrueGridLines1 = 0; TrueGridLines2 = 0; */

   RiverDepth = 0.0; DamDepth = 0.0;
   RiverFlowTotal = 0.0; DamDischargeTotal = 0.0;
   for (int i = 0; i < GridLines/*= UpperLayer*/; i++)
   {
   	if ((BoxDepth[i * GridColumns] > 0.0) && (BoxWidth[i * (GridColumns + 1)] > 0.0))
      {
      	RiverMeanWidth = RiverMeanWidth + BoxWidth[i * (GridColumns + 1)] * BoxDepth[i * GridColumns];
        RiverDepth = RiverDepth + BoxDepth[i * GridColumns];
         //TrueGridLines1++;
      }
      if ((BoxDepth[i * GridColumns + GridColumns - 1] > 0.0) && (BoxWidth[i * (GridColumns + 1) + GridColumns] > 0.0))
      {
         DamMeanWidth = DamMeanWidth + BoxWidth[i * (GridColumns + 1) + GridColumns] * BoxDepth[i * GridColumns + GridColumns - 1] ;
         DamDepth = DamDepth + BoxDepth[i * GridColumns + GridColumns - 1];
         //TrueGridLines2++;
      }
      RiverFlowTotal = RiverFlowTotal + UFlowBetweenBoxes[i * (GridColumns + 1)];
      DamDischargeTotal = DamDischargeTotal + UFlowBetweenBoxes[i * (GridColumns + 1) + GridColumns];
      //Debugger(i); Debugger(RiverFlow); Debugger(DamDischarge);
   }

   //RiverMeanWidth = RiverMeanWidth / TotalDepth[0]/*TrueGridLines1*/;
   //DamMeanWidth = DamMeanWidth / TotalDepth[GridColumns - 1]/*TrueGridLines2*/;
   RiverMeanWidth = RiverMeanWidth / RiverDepth;
   DamMeanWidth = DamMeanWidth / DamDepth;

   for (int i = 0; i < GridLines /*= UpperLayer*/; i++)
   {
      /*if (MyFlowPointer != NULL)
      {
              MyFlowPointer->Inquiry(GetEcoDynClassName(),RiverFlow,i * GridColumns,
                                                              "Hourly river flow",ObjectCode);
              MyFlowPointer->Inquiry(GetEcoDynClassName(),DamDischarge,
                                      i * GridColumns + GridColumns,
                                     "Hourly dam discharge",
                                     ObjectCode);
      }
      else
      {
              RiverFlow = 0.0;
              DamDischarge = 0.0;
      } */

      if ((BoxDepth[i * GridColumns] > 0.0) && (BoxWidth[i * (GridColumns + 1)] > 0.0))
      {

      	/*RiverFlow = RiverFlow *
      				BoxDepth[i * GridColumns] * BoxWidth[i * (GridColumns + 1)]
                   / (TotalDepthPast[0] * RiverMeanWidth); *///Flow evenly distributed through all layers
        RiverFlow = RiverFlowTotal *
      				BoxDepth[i * GridColumns] * BoxWidth[i * (GridColumns + 1)]
                   / (RiverDepth * RiverMeanWidth); //Flow evenly distributed through all layers

   		UVelocity[i * (GridColumns + 1)] = RiverFlow / (BoxWidth[i * (GridColumns + 1)] * BoxDepth[i * GridColumns]);
        UVelocityPast[i * (GridColumns + 1)] = UVelocity[i * (GridColumns + 1)];
        UFlowBetweenBoxes[i * (GridColumns + 1)] = RiverFlow;
      }
      if ((BoxDepth[i * GridColumns + GridColumns - 1] > 0.0) && (BoxWidth[i * (GridColumns + 1) + GridColumns] > 0.0))
      {
      	/*DamDischarge = DamDischarge *
      					BoxDepth[i * GridColumns + GridColumns - 1] * BoxWidth[i * (GridColumns + 1) + GridColumns]
      					/ (TotalDepthPast[GridColumns - 1] * DamMeanWidth); *///Flow evenly distributed through all layers
        DamDischarge = DamDischargeTotal *
      					BoxDepth[i * GridColumns + GridColumns - 1] * BoxWidth[i * (GridColumns + 1) + GridColumns]
      					/ (DamDepth * DamMeanWidth); //Flow evenly distributed through all layers

        //Debugger(DamDischarge);
      	UVelocity[i * (GridColumns + 1) + GridColumns] = DamDischarge / (BoxWidth[i * (GridColumns + 1) + GridColumns] * BoxDepth[i * GridColumns + GridColumns - 1]);
        UVelocityPast[i * (GridColumns + 1) + GridColumns] = UVelocity[i * (GridColumns + 1) + GridColumns];
        UFlowBetweenBoxes[i * (GridColumns + 1) + GridColumns] = DamDischarge;
      }

      //UVelocityPast[i * (GridColumns + 1) + GridColumns] = UVelocity[i * (GridColumns + 1) + GridColumns];
      //Debugger(UVelocity[i * (GridColumns + 1) + GridColumns]);
   }
   Continuity();
   EquationOfMotion();
   k = 0;
   for (int j = 0; j < GridColumns + 1; j++)
   {
      for (int i = 0; i <= UpperLayer; i++)
      {
   		if (
         		(j == 0) && (BoxDepth[i * GridColumns + j] > CriticalDepth) && (BoxWidth[i * (GridColumns + 1) + j] > 0.0)||
               (
               	(j > 0) && (j <= GridColumns) &&
                  (BoxDepth[i * GridColumns + j] > CriticalDepth) &&
                  (BoxWidth[i * (GridColumns + 1) + j] > 0.0)
            	)
         	)
         {


          a[k] = atemporary[i * (GridColumns + 1) + j];
          b[k] = btemporary[i * (GridColumns + 1) + j];
          c[k] = ctemporary[i * (GridColumns + 1) + j];
          r[k] = rtemporary[i * (GridColumns + 1) + j];
          k++;
        }
   	}
   }
   //IntegrationTridag(k);
   IntegrationTridag(a, b, c, r, k);
   mprovetridag(a, b, c, r, u, k);
   k = 0;
   for (int j = 0; j < GridColumns + 1; j++)
   {
   	for (int i = 0; i <= UpperLayer; i++)
      {
      	if (
         		(j == 0) && (BoxDepth[i * GridColumns + j] > CriticalDepth) && (BoxWidth[i * (GridColumns + 1) + j] > 0.0)||
               (
               	(j > 0) && (j < GridColumns)  &&
                  (BoxDepth[i * GridColumns + j] > CriticalDepth) &&
                  (BoxWidth[i * (GridColumns + 1) + j] > 0.0)
            	)
         	)
         {
         	UVelocity[i * (GridColumns + 1) + j] = u[k];
         	if (fabs(UVelocity[i * (GridColumns + 1) + j]) < 0.00000000001)
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
         	k++;
         }
      }
   }

   VerticalVelocity();
   SurfaceVolume();
   //Evaporate();
   //Debugger(6);
   //GenericRiver = 35.0;
   GenericRiver = RiverSalt;
//   Generic = Salt;
   GenericLoad = SaltLoad;
   AdvectDiffuse(Salt);
//   Salt = Generic;
}






void TBiDimensionalVertical::VerticalVelocity()
{
	double UpStreamDepth, DownStreamDepth, MeanBoxWidth, MeanBoxWidthBelow, MeanBoxWidthAbove;
   for (int j = 0; j < GridColumns; j++)
   {
   	for (int i = 0; i < UpperLayer; i++)
      {
      	if (
         		(BoxDepth[i * GridColumns + j] > CriticalDepth)
            )
         {
         	if (
            		(j == GridColumns - 1) ||
                  (
                  	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
                  )
               )
            	DownStreamDepth = BoxDepth[i * GridColumns + j];
            else
            	DownStreamDepth = BoxDepth[i * GridColumns + j + 1];
            if (
            		(j == 0) ||
                  (j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)
               )
            	UpStreamDepth = BoxDepth[i * GridColumns + j];
         	else
            	UpStreamDepth = BoxDepth[i * GridColumns + j - 1];

            MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
            if ((i == 0) ||((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)))
         		MeanBoxWidthBelow = MeanBoxWidth;
      		else
         		MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);

            if ((i == UpperLayer) || ((i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)))
            	MeanBoxWidthAbove = MeanBoxWidth;
            else
               MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);

            WVelocity[(i + 1) * GridColumns + j] =
                              (
                                 WVelocity[i * GridColumns + j] * 0.5 * (MeanBoxWidth + MeanBoxWidthBelow) -
               						(
                                 	(UVelocityPast[i * (GridColumns + 1) + j + 1] + UVelocity[i * (GridColumns + 1) + j + 1]) / 2.0 *
                                    (DownStreamDepth + BoxDepth[i * GridColumns + j]) * BoxWidth[i * (GridColumns + 1) + j + 1] / 2.0 -
                                    (UVelocityPast[i * (GridColumns + 1) + j] + UVelocity[i * (GridColumns + 1) + j]) * BoxWidth[i * (GridColumns + 1) + j] / 2.0 *
                                    (UpStreamDepth + BoxDepth[i * GridColumns + j]) / 2.0
                                 ) / BoxLength[i * GridColumns + j]
                              ) / (0.5 * (MeanBoxWidth + MeanBoxWidthAbove))
                              +  (FlowDischarge[i * GridColumns + j] - FlowUptake[i * GridColumns + j]) /
                                 (
                                 	BoxLength[i * GridColumns + j] *
                                    /*0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1])*/
                                    0.5 * (MeanBoxWidthAbove + MeanBoxWidth)
                                 );

      	}
      }
	}
}

void TBiDimensionalVertical::SurfaceVolume()
{
	double UpStreamDepth, DownStreamDepth, MeanBoxWidth, MeanBoxWidthBelow;
   int i;
   i = UpperLayer;
   for (int j = 0; j < GridColumns; j++)
   {
   	//Debugger(j);
         MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
      	if (
         		(BoxDepth[i * GridColumns + j] > CriticalDepth)
            )
         {
         	if (
            		(j == GridColumns - 1) ||
                  (
                  	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
                  )
               )
            	DownStreamDepth = BoxDepth[i * GridColumns + j];
            else
            	DownStreamDepth = BoxDepth[i * GridColumns + j + 1];
            if (
            		(j == 0) ||
                  (j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)
               )
            	UpStreamDepth = BoxDepth[i * GridColumns + j];
         	else
            	UpStreamDepth = BoxDepth[i * GridColumns + j - 1];

            if ((i == 0) ||((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)))
         		MeanBoxWidthBelow = MeanBoxWidth;
      		else
         		MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
            //Debugger(BoxVolume[i * GridColumns + j]);
            BoxVolume[i * GridColumns + j] = BoxVolume[i * GridColumns + j] -
                           (
               					(
                           		(UVelocityPast[i * (GridColumns + 1) + j + 1] + UVelocity[i * (GridColumns + 1) + j + 1]) / 2.0 *
                              	(DownStreamDepth + BoxDepth[i * GridColumns + j]) / 2.0 *
                                 BoxWidth[i * (GridColumns + 1) + j + 1] -
                              	(UVelocityPast[i * (GridColumns + 1) + j] + UVelocity[i * (GridColumns + 1) + j]) / 2.0 *
                              	(UpStreamDepth + BoxDepth[i * GridColumns + j]) / 2.0 *
                                 BoxWidth[i * (GridColumns + 1) + j]
                           	) -
                           	WVelocity[i * GridColumns + j] *
                           	BoxLength[i * GridColumns + j] *
                           	0.5 * (MeanBoxWidthBelow + MeanBoxWidth)
                           ) * ATimeStep +
                           (FlowDischarge[i * GridColumns + j] - FlowUptake[i * GridColumns + j]) * ATimeStep;   //O volume é recalculado a partir da divergência das velocidades
            //Debugger(BoxVolume[i * GridColumns + j]);
            PastBoxVolume[i * GridColumns + j] = BoxVolume[i * GridColumns + j];

            DynamicHeight[i * GridColumns + j] = BoxVolume[i * GridColumns + j] /
                                                 (
                                                 	BoxLength[i * GridColumns + j] *
                                                  	//0.5 * (MeanBoxWidthBelow + MeanBoxWidth)
                                                   0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1])
                                                 );  //A elevação é recalculada em função do volume
      	}
	}
}

void TBiDimensionalVertical::AdvectDiffuse(double* Generic)
{
	double  DownStreamDepth, UpStreamDepth, DepthAbove, DepthBelow,
           UpWindXConcentration, DownWindXConcentration, UpWindZConcentration,  DownWindZConcentration,
           MeanBoxWidth, MeanBoxWidthAbove;
   int k, kk;
   double *PastGeneric = new double[NumberOfBoxes];

   for (int i = 0; i < NumberOfBoxes; i++)
   	PastGeneric[i] = Generic[i];

   k = 0;
   for (int j = 0; j < GridColumns; j++)
   {
   	for (int i = 0; i <= UpperLayer; i++)
      {
      	//Debugger(i);
      	if (
         		(BoxDepth[i * GridColumns + j] > CriticalDepth)  //&& (BoxWidth[i * (GridColumns + 1) + j] > 0.0)
            )
         {
            MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
            if ((i == UpperLayer) || ((i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)))
            	MeanBoxWidthAbove = MeanBoxWidth;
            else
               MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);

            MassConvection[i * GridColumns + j] = ConvectiveTransport(i,j,PastGeneric);

            //Debugger(MassConvection[i * GridColumns + j]);
            atemporary[i * GridColumns + j] = aDiffusiveTransport(i,j);
            btemporary[i * GridColumns + j] = bDiffusiveTransport(i,j);
            ctemporary[i * GridColumns + j] = cDiffusiveTransport(i,j);


            rtemporary[i * GridColumns + j] = PastGeneric[i * GridColumns + j] * BoxDepth[i * GridColumns + j]+
                                              (
                                              	 MassConvection[i * GridColumns + j] +
                                              	 DiffusiveTransport(i,j,PastGeneric)
                                              ) /*+
                                              ATimeStep *
                        (
                           FlowDischarge[i * GridColumns + j] * GenericLoad[i * GridColumns + j] -
                           FlowUptake[i * GridColumns + j] * PastGeneric[i * GridColumns + j]
                        ) /
                        (
                           BoxLength[i * GridColumns + j] *
                           0.5 *  (MeanBoxWidthAbove + MeanBoxWidth)
                        )*/;

            /*if (i * GridColumns + j == 16)
            {
            	Debugger(MassConvection[i * GridColumns + j]);
               Debugger(DiffusiveTransport(i,j));

            } */
            a[k] = atemporary[i * GridColumns + j];
            b[k] = btemporary[i * GridColumns + j];
            c[k] = ctemporary[i * GridColumns + j];
            r[k] = rtemporary[i * GridColumns + j];
            k++;

         }
      }
	}
   //IntegrationTridag(k);
   IntegrationTridag(a, b, c, r, k);
   mprovetridag(a, b, c, r, u, k);
   k = 0;
   for (int j = 0; j < GridColumns; j++)
   {
   	//Debugger(j);
   	for (int i = 0; i <= UpperLayer; i++)
      {
      	if (
         		(BoxDepth[i * GridColumns + j] > CriticalDepth) //&& (BoxWidth[i * (GridColumns + 1) + j] > 0.0)
            )
         {
         	Generic[i * GridColumns + j] = u[k];

            /*Generic[i * GridColumns + j] = Generic[i * GridColumns + j] + ATimeStep *
                        (
                           FlowDischarge[i * GridColumns + j] * GenericLoad[i * GridColumns + j] -
                           FlowUptake[i * GridColumns + j] * PastGeneric[i * GridColumns + j]
                        ) /
                        (
                        	BoxVolume[i * GridColumns + j] -
                           (DynamicHeight[i * GridColumns + j] - PastHeight[i * GridColumns + j]) *
                           BoxLength[i * GridColumns + j] *
                           0.5 *  (MeanBoxWidthAbove + MeanBoxWidth)
                        ); */

            if (MyLoadRecord != NULL)
            {
                    //Debugger(1); Debugger(MyPEcoDynClass->GetNumberOfLoads());
                    int ADay;
                    for (kk = 0; kk < MyPEcoDynClass->GetNumberOfLoads(); kk++)
                    {
                            if (
                                    (i == MyLoadRecord[kk].LineCoordinate) &&
                                    (j == MyLoadRecord[kk].ColumnCoordinate)
                            )
                            {
                                    ADay = MIN(floor(MyPEcoDynClass->GetTime()) + 1,
                                    MyPEcoDynClass->GetNumberOfDaysForLoads());
                                    //Debugger(ADay);
                                    //Debugger(i); Debugger(j);
                                    //Debugger(kk); Debugger(MyLoadRecord[kk].LoadFlow[ADay - 1]);
                                    //Debugger(GenericLoad[i * GridColumns + j]);

                                    Generic[i * GridColumns + j] = Generic[i * GridColumns + j] +
                                      ATimeStep * GenericLoad[i * GridColumns + j] * MyLoadRecord[kk].LoadFlow[ADay - 1] /
                                      (
                                          BoxVolume[i * GridColumns + j] -
                                         (DynamicHeight[i * GridColumns + j] - PastHeight[i * GridColumns + j]) *
                                         BoxLength[i * GridColumns + j] *
                                         0.5 *  (MeanBoxWidthAbove + MeanBoxWidth)
                                      );
                                    break;
                            }
                    }
           }
           if (MyLossesRecord != NULL)
            {
                    //Debugger(200); Debugger(MyPEcoDynClass->GetNumberOfLosses());
                    int ADay;
                    for (kk = 0; kk < MyPEcoDynClass->GetNumberOfLosses(); kk++)
                    {
                            if (
                                    (i == MyLossesRecord[kk].LineCoordinate) &&
                                    (j == MyLossesRecord[kk].ColumnCoordinate)
                            )
                            {
                                    ADay = MIN(floor(MyPEcoDynClass->GetTime()) + 1,
                                    MyPEcoDynClass->GetNumberOfDaysForLosses());
                                    //Debugger(ADay);
                                    //Debugger(i); Debugger(j);
                                    //Debugger(kk); Debugger(MyLossesRecord[kk].LossFlow[ADay - 1]);

                                    Generic[i * GridColumns + j] = Generic[i * GridColumns + j] -
                                      ATimeStep * PastGeneric[i * GridColumns + j] * MyLossesRecord[kk].LossFlow[ADay - 1] /
                                      (
                                          BoxVolume[i * GridColumns + j] -
                                         (DynamicHeight[i * GridColumns + j] - PastHeight[i * GridColumns + j]) *
                                         BoxLength[i * GridColumns + j] *
                                         0.5 *  (MeanBoxWidthAbove + MeanBoxWidth)
                                      );

                                    break;
                            }
                    }
           }
            k++;
         }
      }
   }
   delete [] PastGeneric;
}


double TBiDimensionalVertical::UPStreamUVelocity(int i, int j) //Calcula o Ui,j-1/2           ok
{
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns))
    {
#ifdef __BORLANDC__
   	    MessageBox(0,
					  "Erro in TBiDimensionalVertical::UPStreamUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TBiDimensionalVertical::UPStreamUVelocity - erro" << endl;
#endif  // __BORLANDC__
        return 0.0;
    }
	else if (j > 0) //Implica que a caixa não constituí limite da malha a oeste
   {
		if (
      		(BoxDepth[i * GridColumns + j - 1] <= CriticalDepth) ||
         	(BoxArray[i * GridColumns + j].BoxToBoundary[UpstreamBoundaryNumber].TypeOfBoundary == 4)
         ) //Ligação com terra a montante
   		return UVelocityPast[i * (GridColumns + 1) + j] / 2.0;
         //return 0.0;
   	else
   		return (
         			UVelocityPast[i * (GridColumns + 1) + j - 1] +
                  UVelocityPast[i * (GridColumns + 1) + j]
                ) / 2.0;
   }
   else /*if (j == 0)*/  //Implica que a caixa constitu? limite da malha a montante
   {
   	if (BoxArray[i * GridColumns + j].BoxToBoundary[UpstreamBoundaryNumber].TypeOfBoundary == 4)
      	return 0.0;
      else
      	return UVelocityPast[i * (GridColumns + 1) + j];//No caso de a fronteira ser liquída devolver a velocidade da própria caixa
   }
}


double TBiDimensionalVertical::DOWNStreamUVelocity(int i, int j)  //Calcula o Ui,j+1/2          ok
{
	if ((i < 0) || (i >= GridLines) || (j < 0) || (j >= GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::DOWNStreamUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
     cerr << "TBiDimensionalVertical::DOWNStreamUVelocity - erro" << endl;
#endif  // __BORLANDC__
      Debugger(i); Debugger(j);
        return 0.0;
    }
	else if (j < GridColumns)
   {
      if(
      		//(BoxDepth[i * GridColumns + j + 1] <= CriticalDepth) || //Fronteira com terra
            (BoxArray[i * GridColumns + j ].BoxToBoundary[DownstreamBoundaryNumber].TypeOfBoundary == 4)
        )
   		return UVelocityPast[i * (GridColumns + 1) + j] / 2.0;
   	else
   		return (
         			UVelocityPast[i * (GridColumns + 1) + j] +
                  UVelocityPast[i * (GridColumns + 1) + j + 1]
                ) / 2.0;
   }
   else /*if (j == GridColumns)*/ //(j == GridColumns - 1)
   {
   	if (BoxArray[i * GridColumns + j - 1].BoxToBoundary[DownstreamBoundaryNumber].TypeOfBoundary == 4)
      	return 0.0;
      else
			return UVelocityPast[i * (GridColumns + 1) + j + 1];
   }
}






double TBiDimensionalVertical::aUCalculation (int i, int j)
{
	// Cálculo do a
   return - Diffusion(1,i,j);
}


double TBiDimensionalVertical::bUCalculation(int i, int j)
{

   if (
         (i == 0) ||
         (
            (i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth) /*Fundo=> Não há difusão, mas sim atrito*/
         )
      )
         return  (
                 	  1.0 + ATimeStep / (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1])
                    * (CoefRugosidade(i,j) + CoefRugosidade(i,j - 1)) * fabs(UVelocityPast[i * (GridColumns + 1) + j])
                 ) + Diffusion(2,i,j);
   else
   		return  1.0 + Diffusion(2,i,j);
}




double TBiDimensionalVertical::cUCalculation(int i, int j)
{
	//Cálculo do c - termo correspondente à velocidade na célula seguinte
   return - Diffusion(3,i,j);
}

double TBiDimensionalVertical::BarotropicPressureTerm(int Line, int Column)
{
	//return 0.0;
	int i, j;
   double DeltaX;
   i = Line; j = Column;
   DeltaX = (BoxLength[i * GridColumns + j - 1] + BoxLength[i * GridColumns + j]) / 2.0;
         return Gravity *
                (DynamicHeight[UpperLayer * GridColumns + j] - DynamicHeight[UpperLayer * GridColumns + j - 1]) *
                ATimeStep / DeltaX;
}



double TBiDimensionalVertical::BaroclinicPressureTerm(int Line, int Column)
{
   //return 0.0;
  	int i, j;
   double PressureGradienteForce;
   i = Line; j = Column;
   PressureGradienteForce = 0.0;
   for (int k = UpperLayer; k >= Line; k--)
   {
      if (k > Line)
   		PressureGradienteForce = PressureGradienteForce + Gravity * ATimeStep *
         	                      (WaterDensity[k * GridColumns + j] - WaterDensity[k * GridColumns + j - 1])*
            	                   0.5 * (BoxDepth[k * GridColumns + j] + BoxDepth[k * GridColumns + j - 1]) /
               	                (0.5 * (BoxLength[k * GridColumns + j] + BoxLength[k * GridColumns + j - 1]));
      else if (k == Line)
         PressureGradienteForce = PressureGradienteForce + Gravity * ATimeStep *
         	                      (WaterDensity[k * GridColumns + j] - WaterDensity[k * GridColumns + j - 1])*
            	                   0.5 * 0.5 * (BoxDepth[k * GridColumns + j] + BoxDepth[k * GridColumns + j - 1]) /
               	                (0.5 * (BoxLength[k * GridColumns + j] + BoxLength[k * GridColumns + j - 1]));
                                  //Os dois 0.5 consecutivos destinam-se a só contabilizar metade da profundidade
                                  //da última camada para a qual se calcula a força do gradiente de pressão
                                  //pois assim reportam-se à profundidade que vai desde a camada superior
                                  //até meio da camada inferior. A lógica é o gradiente de pressão ser calculado
                                  //ao nível da profundidade média da camada e não junto ao fundo ou ao topo
                                  // da mesma
   }
   return PressureGradienteForce /
   		(
         	0.5 * (
                     WaterDensity[i * GridColumns + j] +
                     WaterDensity[i * GridColumns + j - 1]
                   )
         );
}

double TBiDimensionalVertical::ConvectiveTerm(int Line, int Column)
{
	int i, j;
   i = Line; j = Column;
   //Termo Uij-1
   if (
   		(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) &&
         (j < GridColumns)
      )
   {
   	UU1[i * (GridColumns + 1) + j] = ATimeStep * UVelocityPast[i * (GridColumns + 1) + j - 1] *
            (0.5 * (BoxWidth[i * (GridColumns + 1) + j - 1] + BoxWidth[i * (GridColumns + 1) + j])) *
         	BoxDepth[i * GridColumns + j - 1] *
            (UPStreamUVelocity(i,j) + fabs(UPStreamUVelocity(i,j))) /
            (
            	BoxWidth[i * (GridColumns + 1) + j] *
               (
               	BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
               ) *
         		(0.5 * (BoxLength[i * GridColumns + j - 1] + BoxLength[i * GridColumns + j]))
            );

   }
   else
      UU1[i * (GridColumns + 1) + j] = 0.0;


   //Termo Uij
   if (
   		(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) &&
         (j < GridColumns)
      )
   {
      UU2[i * (GridColumns + 1) + j] = ATimeStep * UVelocityPast[i * (GridColumns + 1) + j] *
         	(
            	(0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1])) *
            	BoxDepth[i * GridColumns + j] *
            	(DOWNStreamUVelocity(i,j) + fabs(DOWNStreamUVelocity(i,j)))  -
               (0.5 * (BoxWidth[i * (GridColumns + 1) + j - 1] + BoxWidth[i * (GridColumns + 1) + j])) *
            	BoxDepth[i * GridColumns + j - 1] *
            	(UPStreamUVelocity(i,j) - fabs(UPStreamUVelocity(i,j)))
         	)  /
            (
            	BoxWidth[i * (GridColumns + 1) + j] *
               (
               	BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
               )  *
         		(0.5 * (BoxLength[i * GridColumns + j - 1] + BoxLength[i * GridColumns + j]))
            );
   }
   else
      UU2[i * (GridColumns + 1) + j] = 0.0;

 
   //Termo Uij+1
	if (
   		(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) &&
         (j < GridColumns)
      )
   {
   	UU3[i * (GridColumns + 1) + j] = ATimeStep * UVelocityPast[i * (GridColumns + 1) + j + 1] *
            (0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1])) *
            BoxDepth[i * GridColumns + j] *
            (DOWNStreamUVelocity(i,j) - fabs(DOWNStreamUVelocity(i,j)))/
   			(
            	BoxWidth[i * (GridColumns + 1) + j] *
               (
               	BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1]
               ) *
         		(0.5 * (BoxLength[i * GridColumns + j - 1] + BoxLength[i * GridColumns + j]))
            );
   
   }
   else
      UU3[i * (GridColumns + 1) + j] = 0.0;


   //Termo UW
   if (
   		(j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth) &&
         (j < GridColumns)
      )
   {
      double MeanWidth1, MeanWidth2, MeanW1, MeanW2, UUpstream, UDownstream;
      if (i < UpperLayer)
      {
      	MeanWidth1 = (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j]) / 2.0; //i+1/2j
         MeanW1 = (WVelocityPast[(i + 1) * GridColumns + j - 1] + WVelocityPast[(i + 1) * GridColumns + j]) / 2.0; //i+1j-1/2
         //UDownstream = UVelocityPast[(i + 1) * (GridColumns + 1) + j];
      }
      else
      {
      	MeanWidth1 = BoxWidth[i * (GridColumns + 1) + j];
         MeanW1 = 0.0;
         //UDownstream = 0.0;
      }

      if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepth))
      {
      	MeanWidth2 = (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j]) / 2.0; //i-1/2j
         MeanW2 = (WVelocityPast[i * GridColumns + j - 1] + WVelocityPast[i * GridColumns + j]) / 2.0; //ij-1/2
         UUpstream = UVelocityPast[(i - 1) * (GridColumns + 1) + j];
      }
      else
      {
      	MeanWidth2 = BoxWidth[i * (GridColumns + 1) + j];
         MeanW2 = (WVelocityPast[i * GridColumns + j - 1] + WVelocityPast[i * GridColumns + j]) / 2.0; //ij-1/2
         UUpstream = 0.0;
      }

      if ((i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepth))
      	UDownstream = UVelocityPast[(i + 1) * (GridColumns + 1) + j];
      else
      	UDownstream = 0.0;
      if (MeanWidth1 + MeanWidth2 <= 0.0) UW[i * (GridColumns + 1) + j] = 0.0;
      else
      UW[i * (GridColumns + 1) + j] = ATimeStep / (MeanWidth1 + MeanWidth2) *
             	(
             	 	MeanWidth1 * (MeanW1 + fabs(MeanW1)) *
                	UVelocityPast[i * (GridColumns + 1) + j] -
                	MeanWidth2 * (MeanW2 + fabs(MeanW2)) *
                  UUpstream +
                	MeanWidth1 * (MeanW1 - fabs(MeanW1)) *
                	UDownstream -
                	MeanWidth2 * (MeanW2 - fabs(MeanW2)) *
                	UVelocityPast[i * (GridColumns + 1) + j]
             	) / (0.5 * (BoxDepth[i * GridColumns + j - 1] + BoxDepth[i * GridColumns + j]));
   }
   else
   	UW[i * (GridColumns + 1) + j] = 0.0;

   return (UU2[i * (GridColumns + 1) + j] - UU1[i * (GridColumns + 1) + j] + UU3[i * (GridColumns + 1) + j]) + UW[i * (GridColumns + 1) + j];
}


double TBiDimensionalVertical::Diffusion(int TermOrder, int Line, int Column)
{
	int i, j;
   i = Line; j = Column;
   double DragEffect;

   if (TermOrder == 1)  //a
   {
   	if (
         	(i == 0) ||
         	(
         		(i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth) /*Fundo=> Não há difusão, mas sim atrito*/
         	)
      	)
   		return 0.0;
   	else
      	return VerticalDiffCoefU[i * (GridColumns + 1) + j] * ATimeStep /
                pow(0.5*(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]),2.0)
             	 /*pow(BoxLength[i * GridColumns + j],2.0)*/;
   }

   else if (TermOrder == 2)  //b
   {
      if (
         	(i == 0) ||
         	(
         		(i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth) /*Fundo=> Não há difusão, mas sim atrito*/
         	)
      	)
      {
      	if ((i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepth))
      		return  VerticalDiffCoefU[(i + 1) * (GridColumns + 1) + j] * ATimeStep /
                 pow(0.5*(BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]),2.0);

      	else if ((i == UpperLayer) || ((i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] < CriticalDepth)))
            return  0.0;
      }
      else
      {
      	if ((i == UpperLayer) || ((i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] < CriticalDepth)))
         	return  VerticalDiffCoefU[i * (GridColumns + 1) + j] * ATimeStep /
                 pow(0.5*(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]),2.0);
         else
   			return  VerticalDiffCoefU[i * (GridColumns + 1) + j] * ATimeStep /
                 pow(0.5*(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]),2.0)
                 /*pow(BoxLength[i * GridColumns + j - 1],2.0)*/ +
              	  VerticalDiffCoefU[(i + 1) * (GridColumns + 1) + j] * ATimeStep /
                 pow(0.5*(BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]),2.0)
                 /*pow(BoxLength[i * GridColumns + j],2.0)*/;
      }
   }

   else if (TermOrder == 3)  //c
   {
   	if (i >= UpperLayer)
    {
      	return 0.0;
    }
    else
   		return VerticalDiffCoefU[(i + 1) * (GridColumns + 1) + j] * ATimeStep /
                pow(0.5*(BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]),2.0)
             	 /*pow(BoxLength[i * GridColumns + j],2.0)*/;
   }

   return 0.0;

   /*else if (TermOrder == 4)  //r
   {
   	double DragEffect, TotalDiffusion, UpStreamDiffusion, DownStreamDiffusion;
      DragEffect = 0.0;
      TotalDiffusion = 0.0;

   	if ((j == 0) || (j == GridColumns))
      	return 0.0;
      else
      if (
            (i == 0) ||
            (
            	(i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)
            )
         )
         DragEffect = Drag(i,j);

      //Acrescentar efeito do vento

   	if (
      		(j >= GridColumns - 1) ||
            (
            	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
         	)
         )
      	DownStreamDiffusion = 0.0;
      else
      {
         double ADepth, AVelocity;
         ADepth = 0.5 * (TotalDepth[j] + TotalDepth[j + 1]);
         AVelocity = 0.5 * (UVelocityPast[i * (GridColumns + 1) + j] + UVelocityPast[i * (GridColumns + 1) + j + 1]);
         DownStreamDiffusion = HorizontalDiffCoefU[i * (GridColumns + 1) + j ]
                               * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity)
                               * ATimeStep /
                               pow(BoxLength[i * GridColumns + j],2.0) *
                               (
                                  UVelocityPast[i * (GridColumns + 1) + j + 1] -
         		                   UVelocityPast[i * (GridColumns + 1) + j]
                                );
       }
       if (
             (j == 0) ||
             (
             	(j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)
             )
          )
          UpStreamDiffusion = 0.0;
       else
       {
       	//HorizontalDiffCoefU[i * (GridColumns + 1) + j] = HorizontalDiffusionCoefficient(i,j);
          double ADepth, AVelocity;
          ADepth = 0.5 * (TotalDepth[j - 1] + TotalDepth[j]);
          AVelocity = 0.5 * (UVelocityPast[i * (GridColumns + 1) + j - 1] + UVelocityPast[i * (GridColumns + 1) + j]);
          UpStreamDiffusion = HorizontalDiffCoefU[i * (GridColumns + 1) + j]
                              * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity)
                              * ATimeStep /
                              pow(BoxLength[i * GridColumns + j - 1],2.0) *
                              (
                                 UVelocityPast[i * (GridColumns + 1) + j] -
         		                  UVelocityPast[i * (GridColumns + 1) + j - 1]
                               );
       }
       return -DragEffect + DownStreamDiffusion - UpStreamDiffusion;
   }                                                                         */
}


double TBiDimensionalVertical::HorizontalDiffusion(int Line, int Column)
{
	int i, j;
   double TotalDiffusion, UpStreamDiffusion, DownStreamDiffusion;
   i = Line; j = Column;

   if (
   		(j == GridColumns) ||  /*(j == GridColumns - 1) ||*/
         (
         	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
         )
   	)
      DownStreamDiffusion = 0.0;
   else
   {
   	//HorizontalDiffCoefU[i * (GridColumns + 1) + j + 1] = HorizontalDiffusionCoefficient(i,j + 1);
    double ADepth, AVelocity;
    if (j < GridColumns - 1)
        ADepth = 0.5 * (TotalDepth[j] + TotalDepth[j + 1]);
    else
        ADepth = TotalDepth[j];
    AVelocity = 0.5 * (UVelocityPast[i * (GridColumns + 1) + j] + UVelocityPast[i * (GridColumns + 1) + j + 1]);

   	DownStreamDiffusion = HorizontalDiffCoefU[i * (GridColumns + 1) + j/* + 1*/]
                               * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity)
                               * ATimeStep /
                               pow(BoxLength[i * GridColumns + j],2.0) *
                               (
                                  UVelocityPast[i * (GridColumns + 1) + j + 1] -
         		                   UVelocityPast[i * (GridColumns + 1) + j]
                                );
   }
   if (
   		(j == 0) ||
   		(
   			(j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)
   		)
   	)
      UpStreamDiffusion = 0.0;
   else
   {
   	//HorizontalDiffCoefU[i * (GridColumns + 1) + j] = HorizontalDiffusionCoefficient(i,j);
    double ADepth, AVelocity;
    ADepth = 0.5 * (TotalDepth[j - 1] + TotalDepth[j]);
    AVelocity = 0.5 * (UVelocityPast[i * (GridColumns + 1) + j - 1] + UVelocityPast[i * (GridColumns + 1) + j]);

   	UpStreamDiffusion = HorizontalDiffCoefU[i * (GridColumns + 1) + j]
                              * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity)
                              * ATimeStep /
                              pow(BoxLength[i * GridColumns + j - 1],2.0) *
                              (
                                 UVelocityPast[i * (GridColumns + 1) + j] -
         		                  UVelocityPast[i * (GridColumns + 1) + j - 1]
                               );
   }
   return DownStreamDiffusion - UpStreamDiffusion;
}



double TBiDimensionalVertical::WindStress(int ABox)
{
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
   double MyWindSpeed;
   if (MyWindPointer != NULL)
   {
   	MyWindPointer->Inquiry(GetEcoDynClassName(),MyWindSpeed,
											ABox,
                                 "Wind speed",
                                 ObjectCode);

      return WindStressCoef * pow(MyWindSpeed,2.0);
   }
   else
   	return 0.0;
}

double TBiDimensionalVertical::CoefRugosidade(int i, int j)
{
   const double VonKarman = 0.4;
                //Rugosity = 0.0025, //m
                //HMINC = 0.5; //m
   double DistanceFromTheBottom, Z, /*UpStreamCoef, DownSteamCoef,*/ MyCoefficient;
   DistanceFromTheBottom = Depth(0,j) - (Depth(i,j) - BoxDepth[i * GridColumns + j] / 2.0);
    //Retiro metade da espessura da caixa à distância ao fundo do respectivo topo
   /*if ((i == 0) && (j == 1))
   {
   	Debugger(Depth(0,j));
      Debugger(Depth(i,j));
   	Debugger(DistanceFromTheBottom);

   } */

   Z = MAX(DistanceFromTheBottom, HMINC);
   //DownSteamCoef = pow(VonKarman, 2.0) / pow((log(Z / Rugosity[i * GridColumns + j])),2.0);
   /*if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth))
   {
   	DistanceFromTheBottom = Depth(0,j - 1) - (Depth(i,j - 1) - BoxDepth[i * GridColumns + j - 1] / 2.0);
   	//Retiro metade da espessura da caixa à distância ao fundo do respectivo topo
   	Z = MAX(DistanceFromTheBottom, HMINC);
   	UpStreamCoef = pow(VonKarman, 2.0) / pow((log(Z / Rugosity[i * GridColumns + j - 1])),2.0);
   }
   if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth))
		return (UpStreamCoef + DownSteamCoef)/2.0;
   else
      return DownSteamCoef; */
   MyCoefficient = pow(VonKarman, 2.0) / pow((log(Z / Rugosity[i * GridColumns + j])),2.0);
   return MyCoefficient;
}

double TBiDimensionalVertical::HorizontalDiffusionCoefficient(int i, int j)
{
	return kdiffusion * BoxDepth[i * GridColumns + j] * sqrt(CoefRugosidade(i,j)) * fabs(UVelocity[i * GridColumns + j]);
}


double TBiDimensionalVertical::VerticalMomentumDiffusivity(int i, int j)
{
	//fide Aires Santos (pHD Thesis, p. 22 - 26)

   if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j >= GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TBiDimensional::VerticalMomentumDiffusivity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
     cerr << "TBiDimensionalVertical::VerticalMomentumDiffusivity - erro" << endl;
#endif  // __BORLANDC__
        return 0.0;
    }
   else
   if ((BoxDepth[i * GridColumns + j] <= CriticalDepth) || (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth))
   	return 0.0;
   else
   {
      return MIN(fabs(pow(LengthScale(i,j),2.0) *
            (UVelocityPast[i * (GridColumns + 1) + j] - UVelocityPast[(i - 1) * (GridColumns + 1) + j]) /
          	(0.5 * (BoxDepth[i * GridColumns + j] + BoxDepth[(i - 1) * GridColumns + j]))),MaxVertDiffusivity);

	}
}

double TBiDimensionalVertical::VerticalMassAndHeatDiffusivity(int i, int j)
{
	double Beta, gama1, gama2;
   //Formulation by Lendertse and Liu (1978) fide Aires Santos (pHD Thesis, p. 26)
   gama1 = 1.4; gama2 = 1.2;
   /*if ((j == 1) && (i == 1))
   {
   	Debugger(1000);
      Debugger(WaterDensity[i * GridColumns + j]);Debugger(Salt[i * GridColumns + j]);
      Debugger(WaterDensity[(i - 1) * GridColumns + j]);Debugger(Salt[(i - 1) * GridColumns + j]);
   	Debugger(RichardsonNumber(i,j));
      Debugger(VerticalDiffCoefU[i * (GridColumns + 1) + j]);
   } */
   Beta = gama1 * exp(-gama2 * RichardsonNumber(i,j));
   return VerticalDiffCoefU[i * (GridColumns + 1) + j] * Beta;

   //Formulation by Munk and Anderson (1948) fide Aires Santos (pHD Thesis, p. 26)
   //1 <= gama <= 2
   /*gama = 1.5;
   return gama * sqrt(1.0 + 10.0 * RichardsonNumber(i,j)) / pow((1.0 + 3.33 * RichardsonNumber(i,j)),3/2); */
}


/*double TBiDimensionalVertical::LengthScale(int i, int j)
{
	double delta,MyTotalDepth, DistanceFromTheBottom;
   const double VonKarman = 0.4, alfa = 0.8;
	TEcoDynClass* MyWindPointer = MyPIntegrate->GetWindPointer();
   if (MyWindPointer != NULL)
   	delta = 0.5;
   else
   	delta = 1.0;
   MyTotalDepth = Depth(0,j);
   DistanceFromTheBottom = MyTotalDepth - Depth(i,j);
   //Formulation by Nihoul (1982) fide Aires Santos (pHD Thesis, p. 23)
	return MyTotalDepth * VonKarman * DistanceFromTheBottom / MyTotalDepth *
          (1.0 - delta * DistanceFromTheBottom / MyTotalDepth) *
          exp(-alfa * RichardsonNumber(i,j));
}    */

double TBiDimensionalVertical::LengthScale(int i, int j)
{
	double delta,MyTotalDepth1, MyTotalDepth2, DistanceFromTheBottom1, DistanceFromTheBottom2, MyAverageDepth, MyAverageDistance;
   const double VonKarman = 0.4, alfa = 0.8;
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
   if (MyWindPointer != NULL)
   	delta = 0.5;
   else
   	delta = 1.0;
   if (j > 0)
   {
   	  MyTotalDepth1 = Depth(0,j - 1);
      DistanceFromTheBottom1 = MIN(Depth(i,j - 1), MyTotalDepth1 - Depth(i,j - 1));
   }
   else
   {
    	MyTotalDepth1 = 0.0;
        DistanceFromTheBottom1 = 0.0;
   }
   MyTotalDepth2 = Depth(0,j);
   DistanceFromTheBottom2 = MIN(Depth(i,j), MyTotalDepth2 - Depth(i,j));

   MyAverageDepth = 0.5 * (MyTotalDepth1 + MyTotalDepth2);
   MyAverageDistance = 0.5 * (DistanceFromTheBottom1 + DistanceFromTheBottom2);
   //Formulation by Nihoul (1982) fide Aires Santos (pHD Thesis, p. 23)

	return MyAverageDepth * VonKarman * MyAverageDistance / MyAverageDepth *
          (1.0 - delta * MyAverageDistance / MyAverageDepth) *
          exp(-alfa * RichardsonNumber(i,j));
}

double TBiDimensionalVertical::RichardsonNumber(int i, int j)
{
   double const Rs = 0.25;                     // Número de Richardson abaixo do qual o
   double AValue1, AValue2, AVerageValue;      // "shear stress" é suficiente para provocar
                                               // trocas verticais
   if   (
           ((UVelocity[i * (GridColumns + 1) + j] - UVelocity[(i - 1) * (GridColumns + 1) + j])>= 0.0000001) ||
           ((UVelocity[i * (GridColumns + 1) + j] - UVelocity[(i - 1) * (GridColumns + 1) + j])<= -0.0000001)
        )

   {
	  	AValue1 = (
     	      		(
      	         	//Gravity * (WaterDensity[i * GridColumns + j] - WaterDensity[(i - 1) * GridColumns + j]) /
                     Gravity * (WaterDensity[(i - 1) * GridColumns + j] - WaterDensity[i * GridColumns + j]) /
         	         (0.5 * (WaterDensity[i * GridColumns + j] + WaterDensity[(i - 1) * GridColumns + j]))
            	   )   /
               	(
               		( pow( (UVelocity[i * (GridColumns + 1) + j] - UVelocity[(i - 1) * (GridColumns + 1) + j]), 2.0)) /
                     (0.5 * (BoxDepth[i * GridColumns + j] + BoxDepth[(i - 1) * GridColumns + j]))
               	)
   				);

      if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > 0.0) && (BoxDepth[(i - 1) * GridColumns + j - 1] > 0.0))
      {
      	AValue2 = (
     	      		(
      	         	//Gravity * (WaterDensity[i * GridColumns + j - 1] - WaterDensity[(i - 1) * GridColumns + j - 1]) /
                     Gravity * (WaterDensity[(i - 1) * GridColumns + j - 1] - WaterDensity[i * GridColumns + j - 1]) /
         	         (0.5 * (WaterDensity[i * GridColumns + j - 1] + WaterDensity[(i - 1) * GridColumns + j - 1]))
            	   )   /
               	(
               		( pow( (UVelocity[i * (GridColumns + 1) + j] - UVelocity[(i - 1) * (GridColumns + 1) + j]), 2.0)) /
                     (0.5 * (BoxDepth[i * GridColumns + j - 1] + BoxDepth[(i - 1) * GridColumns + j - 1]))
               	)
   				);

         return /*MIN(*/0.5 * (MAX(0.0,AValue1) + MAX(0.0,AValue2))/*, Rs)*/;
       }
       else
         return /*MIN(*/MAX(0.0,AValue1)/*, Rs)*/;
          //AValue2 = 0.0;
           // AVerageValue = 0.5 * (MAX(0,AValue1) + MAX(0,AValue2));

       //if (AVerageValue > 0.0) return AVerageValue;
       //else return 0.0;
   }
   else
   	return Rs;
}


double TBiDimensionalVertical::Depth(int i, int j)
{
   double Total;
   int Line, k;
   Total = 0.0; Line = i;
   for (int k = UpperLayer; k >= Line; k--)
   {
   	if (BoxDepth[k * GridColumns + j] > 0.0)
   		Total = Total + BoxDepth[k * GridColumns + j];
      /*else
         MessageBox(0, "Intermediate solid layer - TBiDimensionalVertical::Depth", "EcoDynamo alert", MB_OK); */
   }
   return Total;
}


double TBiDimensionalVertical::ConvectiveTransport(int i, int j, double* PastGeneric)
{
	double  DownStreamDepth, UpStreamDepth,
           UpWindXConcentration1, DownWindXConcentration1, UpWindZConcentration1,  DownWindZConcentration1,
           UpWindXConcentration2, DownWindXConcentration2, UpWindZConcentration2,  DownWindZConcentration2,
           MeanBoxWidth, MeanBoxWidthBelow, MeanBoxWidthAbove;
	if (
   		(BoxDepth[i * GridColumns + j] > CriticalDepth)
   	)
   {
      MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
		if (
      		(j == GridColumns - 1) ||
            (
            	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
            )
      	)
         DownStreamDepth = BoxDepth[i * GridColumns + j];
      else
      	DownStreamDepth = BoxDepth[i * GridColumns + j + 1];


      if (
      		(j == 0) ||
            (j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)
      	)
         UpStreamDepth = BoxDepth[i * GridColumns + j];
      else
      	UpStreamDepth = BoxDepth[i * GridColumns + j - 1];


      if (
      		(i == UpperLayer) ||
            (
            	(i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)
            )
      	)
      {
         MeanBoxWidthAbove = MeanBoxWidth;
      }
      else
      {
         MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);
      }

      if (
      		(i == 0) ||
            (i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)
      	)
      {
         MeanBoxWidthBelow = MeanBoxWidth;
      }
      else
      {
         MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
      }

      if (UVelocityPast[i * (GridColumns + 1) + j] > 0.0)
      {
       	if (j == 0)
         	UpWindXConcentration1 = GenericRiver;
         else if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth))
            UpWindXConcentration1 = 0.0;
         else
      		UpWindXConcentration1 = PastGeneric[i * GridColumns + j - 1];
      }
      else
      	UpWindXConcentration1 = PastGeneric[i * GridColumns + j];

      if (UVelocityPast[i * (GridColumns + 1) + j + 1] < 0.0)
      {
      	if (
         		(j == GridColumns - 1) ||
               (
               	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
               )
            )
         	DownWindXConcentration1 = 0.0;
         else
      		DownWindXConcentration1 = PastGeneric[i * GridColumns + j + 1];
      }
      else
      	DownWindXConcentration1 = PastGeneric[i * GridColumns + j];

      if (WVelocityPast[i * GridColumns + j] > 0.0)
      {
      	if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepth))
      		UpWindZConcentration1 = PastGeneric[(i - 1) * GridColumns + j];
      	else
            UpWindZConcentration1 = 0.0;
      }
      else
      	UpWindZConcentration1 = PastGeneric[i * GridColumns + j];


      if ((i < UpperLayer) && (WVelocityPast[(i + 1) * GridColumns + j] < 0.0))
      {
      	if(BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)
         	DownWindZConcentration1 = 0.0;
         else
      		DownWindZConcentration1 = PastGeneric[(i + 1) * GridColumns + j];
      }
      else
      	DownWindZConcentration1 = PastGeneric[i * GridColumns + j];

      if (UVelocity[i * (GridColumns + 1) + j] > 0.0)
      {
       	if (j == 0)
         	UpWindXConcentration2 = GenericRiver;
         else if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth))
            UpWindXConcentration2 = 0.0;
         else
      		UpWindXConcentration2 = PastGeneric[i * GridColumns + j - 1];
      }
      else
      	UpWindXConcentration2 = PastGeneric[i * GridColumns + j];

      if (UVelocity[i * (GridColumns + 1) + j + 1] < 0.0)
      {
      	if (
         		(j == GridColumns - 1) ||
               (
               	(j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)
               )
            )
         	DownWindXConcentration2 = 0.0;
         else
      		DownWindXConcentration2 = PastGeneric[i * GridColumns + j + 1];
      }
      else
      	DownWindXConcentration2 = PastGeneric[i * GridColumns + j];

      if (WVelocity[i * GridColumns + j] > 0.0)
      {
      	if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepth))
      		UpWindZConcentration2 = PastGeneric[(i - 1) * GridColumns + j];
      	else
            UpWindZConcentration2 = 0.0;
      }
      else
      	UpWindZConcentration2 = PastGeneric[i * GridColumns + j];


      if ((i < UpperLayer) && (WVelocity[(i + 1) * GridColumns + j] < 0.0))
      {
      	if(BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)
         	DownWindZConcentration2 = 0.0;
         else
      		DownWindZConcentration2 = PastGeneric[(i + 1) * GridColumns + j];
      }
      else
      	DownWindZConcentration2 = PastGeneric[i * GridColumns + j];


      if (i < UpperLayer)
      {
        /*if (i * GridColumns + j == 16)
        {
        		Debugger(2000);
        		Debugger(WVelocity[(i + 1) * GridColumns + j] * (MeanBoxWidthAbove + MeanBoxWidth) * BoxLength[i * GridColumns + j]);

        }  */
         
      	return (
             		(
                  	(UpStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocity[i * (GridColumns + 1) + j] * UpWindXConcentration2 +
                  	(UpStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocityPast[i * (GridColumns + 1) + j] * UpWindXConcentration1
                  )  * ATimeStep * BoxWidth[i * (GridColumns + 1) + j] -
                  (
               		(DownStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocity[i * (GridColumns + 1) + j + 1] * DownWindXConcentration2 +
                  	(DownStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocityPast[i * (GridColumns + 1) + j + 1] * DownWindXConcentration1
                  )  * ATimeStep * BoxWidth[i * (GridColumns + 1) + j + 1]
                ) /
                (
                   4.0 * BoxLength[i * GridColumns + j] * MeanBoxWidth
                ) +
                (
                	(
                     (MeanBoxWidthBelow + MeanBoxWidth)* WVelocity[i * GridColumns + j] * UpWindZConcentration2
                     //+(MeanBoxWidthBelow + MeanBoxWidth)* WVelocityPast[i * GridColumns + j] * UpWindZConcentration1
                  )  * ATimeStep -
                  (
                     (MeanBoxWidthAbove + MeanBoxWidth)* WVelocity[(i + 1) * GridColumns + j] * DownWindZConcentration2
                     //+(MeanBoxWidthAbove + MeanBoxWidth)* WVelocityPast[(i + 1) * GridColumns + j] * DownWindZConcentration1
                  )  * ATimeStep
                ) / (2.0 * MeanBoxWidth);

       }
       else
       {

       	return (
             		(
                  	(UpStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocity[i * (GridColumns + 1) + j] * UpWindXConcentration2 +
                  	(UpStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocityPast[i * (GridColumns + 1) + j]* UpWindXConcentration1
                  ) * ATimeStep * BoxWidth[i * (GridColumns + 1) + j] -
                  (
               		(DownStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocity[i * (GridColumns + 1) + j + 1] * DownWindXConcentration2 +
                  	(DownStreamDepth + BoxDepth[i * GridColumns + j]) *
               		UVelocityPast[i * (GridColumns + 1) + j + 1] * DownWindXConcentration1
                  )  * ATimeStep * BoxWidth[i * (GridColumns + 1) + j + 1]
                ) /
                (
                   4.0 * BoxLength[i * GridColumns + j] * MeanBoxWidth
                ) +
             	 (
                	(
                     (MeanBoxWidthBelow + MeanBoxWidth)* WVelocity[i * GridColumns + j] * UpWindZConcentration2
                     //+ (MeanBoxWidthBelow + MeanBoxWidth)* WVelocityPast[i * GridColumns + j] * UpWindZConcentration1
                  )  * ATimeStep
                ) / (2.0 * MeanBoxWidth);

       }
	}
    else
    {
        return 0.0;
    }

}

double TBiDimensionalVertical::DiffusiveTransport(int i, int j, double *PastGeneric)
{

	double  LeftFlux, RightFlux, ADepth, AVelocity, LeftDiffusivity, RightDiffusivity;
  /*
    if (j < GridColumns - 1)
        ADepth = 0.5 * (TotalDepth[j] + TotalDepth[j + 1]);
    else
        ADepth = TotalDepth[j];
    AVelocity = 0.5 * (UVelocityPast[i * (GridColumns + 1) + j] + UVelocityPast[i * (GridColumns + 1) + j + 1]);

   	LeftDiffusion = HorizontalDiffCoefU[i * (GridColumns + 1) + j]
                               * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity)  */



      if ((j == 0) || ((j > 0) && (BoxDepth[i * GridColumns + j - 1] <= CriticalDepth)))
      	LeftFlux = 0.0;
      else
      {
        ADepth = 0.5 * (TotalDepth[j - 1] + TotalDepth[j]);
        AVelocity = UVelocityPast[i * (GridColumns + 1) + j];
        LeftDiffusivity = HorizontalDiffCoefU[i * (GridColumns + 1) + j]
                               * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity);
      	LeftFlux = /*HorizontalDiffCoefU[i * (GridColumns + 1) + j]*/ LeftDiffusivity *
                    (PastGeneric[i * GridColumns + j] - PastGeneric[i * GridColumns + j - 1]) *
                    (BoxDepth[i * GridColumns + j - 1] + BoxDepth[i * GridColumns + j]) *
                    BoxWidth[i * (GridColumns + 1) + j] * ATimeStep /
                    (pow(0.5 * (BoxLength[i * GridColumns + j - 1] + BoxLength[i * GridColumns + j]),2));

      }
      if ((j == GridColumns - 1) || ((j < GridColumns - 1) && (BoxDepth[i * GridColumns + j + 1] <= CriticalDepth)))
      	RightFlux = 0.0;
      else
      {
        ADepth = 0.5 * (TotalDepth[j] + TotalDepth[j + 1]);
        AVelocity = UVelocityPast[i * (GridColumns + 1) + j + 1];
        RightDiffusivity = HorizontalDiffCoefU[i * (GridColumns + 1) + j + 1] * CorrectionOfHorizontalDiffCoef(ADepth, AVelocity);
      	RightFlux = /*HorizontalDiffCoefU[i * (GridColumns + 1) + j + 1]*/ RightDiffusivity *
                     (PastGeneric[i * GridColumns + j + 1] - PastGeneric[i * GridColumns + j]) *
            	     	(BoxDepth[i * GridColumns + j + 1] + BoxDepth[i * GridColumns + j]) *
                     BoxWidth[i * (GridColumns + 1) + j + 1] * ATimeStep /
                    (pow(0.5 * (BoxLength[i * GridColumns + j + 1] + BoxLength[i * GridColumns + j]),2));

      }

      return (
      			(RightFlux - LeftFlux)
             ) / (2.0 * 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]));
}


double TBiDimensionalVertical::aDiffusiveTransport (int i, int j)
{
	double MeanBoxWidth, MeanBoxWidthBelow, VerticalDistance;

   MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);

	if ((i == 0) || (i > 0) && (BoxDepth[(i - 1) * GridColumns + j] < 0.0))
   	return (0.0);
   else
   {
      MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
      VerticalDistance = (BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
   	return(-VerticalMassAndHeatDiffusivity(i,j) *
         	(MeanBoxWidthBelow + MeanBoxWidth) * ATimeStep /
         	(2.0 * VerticalDistance * MeanBoxWidth));
   }
}

double TBiDimensionalVertical::bDiffusiveTransport (int i, int j)
{
	double MeanBoxWidth, MeanBoxWidthBelow, MeanBoxWidthAbove, VerticalDistanceBelow, VerticalDistanceAbove, Parcel1, Parcel2;
   MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
   if (
   		(i == 0) ||
         (i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)
   	)
   {
      MeanBoxWidthBelow = MeanBoxWidth;
      VerticalDistanceBelow = BoxDepth[i * GridColumns + j] / 2.0;
      Parcel1 = 0.0;
   }
   else
   {
      MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
      VerticalDistanceBelow = (BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
      Parcel1 = VerticalMassAndHeatDiffusivity(i,j) * (MeanBoxWidthBelow + MeanBoxWidth);
   }
   if (
   		(i == UpperLayer) ||
         (
         	(i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)
         )
   	)
   {
      MeanBoxWidthAbove = MeanBoxWidth;
      VerticalDistanceAbove = BoxDepth[i * GridColumns + j] / 2.0;
      Parcel2 = 0.0;
   }
   else
   {
      MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);
      VerticalDistanceAbove = (BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
      Parcel2 = VerticalMassAndHeatDiffusivity(i + 1,j) * (MeanBoxWidthAbove + MeanBoxWidth);
   }

	return BoxDepth[i * GridColumns + j] + DynamicHeight[i * GridColumns + j] - PastHeight[i * GridColumns + j] +
          (Parcel1 / (2.0 * VerticalDistanceBelow * MeanBoxWidth) + Parcel2 / (2.0 * VerticalDistanceAbove * MeanBoxWidth)) * ATimeStep;
}

double TBiDimensionalVertical::cDiffusiveTransport (int i, int j)
{
	double MeanBoxWidth, MeanBoxWidthAbove, VerticalDistance;
   MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);


	if ((i == UpperLayer) || (i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] < 0.0))
   	return(0.0);
   else
   {
   	MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);
      VerticalDistance = (BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
   	return (-VerticalMassAndHeatDiffusivity(i + 1,j) *
             (MeanBoxWidthAbove + MeanBoxWidth) * ATimeStep /
             (2.0 * VerticalDistance * MeanBoxWidth));
   }
}


double TBiDimensionalVertical::aTransport (int i, int j)
{
	double MeanBoxWidth, MeanBoxWidthBelow, VerticalDistance;

   MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);

	if ((i == 0) || (i > 0) && (BoxDepth[(i - 1) * GridColumns + j] < CriticalDepth))
   	return (0.0);
   else
   {
      MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
      VerticalDistance = (BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
      if (WVelocity[i * GridColumns + j] > 0.0)
   	return -VerticalMassAndHeatDiffusivity(i,j) *
         	(MeanBoxWidthBelow + MeanBoxWidth) * ATimeStep /
         	(2.0 * VerticalDistance * MeanBoxWidth) -
            (MeanBoxWidthBelow + MeanBoxWidth)* WVelocity[i * GridColumns + j]
            * ATimeStep / (2.0 * MeanBoxWidth);
      else
      return -VerticalMassAndHeatDiffusivity(i,j) *
         	(MeanBoxWidthBelow + MeanBoxWidth) * ATimeStep /
         	(2.0 * VerticalDistance * MeanBoxWidth);
   }
}

double TBiDimensionalVertical::bTransport (int i, int j)
{
	double MeanBoxWidth, MeanBoxWidthBelow, MeanBoxWidthAbove, VerticalDistanceBelow, VerticalDistanceAbove, Parcel1, Parcel2,
   Parcel3, Parcel4;
   MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
   if (
   		(i == 0) ||
         (i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)
   	)
   {
      MeanBoxWidthBelow = MeanBoxWidth;
      VerticalDistanceBelow = BoxDepth[i * GridColumns + j] / 2.0;
      Parcel1 = 0.0;
   }
   else
   {
      MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
      VerticalDistanceBelow = (BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
      Parcel1 = VerticalMassAndHeatDiffusivity(i,j) * (MeanBoxWidthBelow + MeanBoxWidth);
   }
   if (
   		(i == UpperLayer) ||
         (
         	(i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] <= CriticalDepth)
         )
   	)
   {
      MeanBoxWidthAbove = MeanBoxWidth;
      VerticalDistanceAbove = BoxDepth[i * GridColumns + j] / 2.0;
      Parcel2 = 0.0;
   }
   else
   {
      MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);
      VerticalDistanceAbove = (BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
      Parcel2 = VerticalMassAndHeatDiffusivity(i + 1,j) * (MeanBoxWidthAbove + MeanBoxWidth);
   }

   if (WVelocity[i * GridColumns + j] < 0.0)
   	Parcel3 = (MeanBoxWidthBelow + MeanBoxWidth)* WVelocity[i * GridColumns + j]
                * ATimeStep / (2.0 * MeanBoxWidth);
   else
      Parcel3 = 0.0;

   if (WVelocity[(i + 1) * GridColumns + j] > 0.0)
   	Parcel4 = (MeanBoxWidthAbove + MeanBoxWidth)* WVelocity[(i + 1) * GridColumns + j]
                * ATimeStep / (2.0 * MeanBoxWidth);
   else
      Parcel4 = 0.0;

	return BoxDepth[i * GridColumns + j] + DynamicHeight[i * GridColumns + j] - PastHeight[i * GridColumns + j] +
          (Parcel1 / (2.0 * VerticalDistanceBelow * MeanBoxWidth) + Parcel2 / (2.0 * VerticalDistanceAbove * MeanBoxWidth)) * ATimeStep -
          Parcel3 + Parcel4;
}

double TBiDimensionalVertical::cTransport (int i, int j)
{
	double MeanBoxWidth, MeanBoxWidthAbove, VerticalDistance;
   MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);


	if ((i == UpperLayer) || (i < UpperLayer) && (BoxDepth[(i + 1) * GridColumns + j] < 0.0))
   	return(0.0);
   else
   {
   	MeanBoxWidthAbove = 0.5 * (BoxWidth[(i + 1) * (GridColumns + 1) + j] + BoxWidth[(i + 1) * (GridColumns + 1) + j + 1]);
      VerticalDistance = (BoxDepth[(i + 1) * GridColumns + j] + BoxDepth[i * GridColumns + j]) / 2.0;
      
      if (WVelocity[(i + 1) * GridColumns + j] < 0.0)
   	return -VerticalMassAndHeatDiffusivity(i + 1,j) *
             (MeanBoxWidthAbove + MeanBoxWidth) * ATimeStep /
             (2.0 * VerticalDistance * MeanBoxWidth) +
             (MeanBoxWidthAbove + MeanBoxWidth)* WVelocity[(i + 1) * GridColumns + j]
             * ATimeStep / (2.0 * MeanBoxWidth);
      else
      return -VerticalMassAndHeatDiffusivity(i + 1,j) *
             (MeanBoxWidthAbove + MeanBoxWidth) * ATimeStep /
             (2.0 * VerticalDistance * MeanBoxWidth);
   }
}

void TBiDimensionalVertical::Reinitialize()
{
	double MyWaterTemperature, MeanBoxWidth, MeanBoxWidthBelow;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
	for (int i = 0; i <= UpperLayer; i++)
    {
   	  for (int j = 0; j < GridColumns + 1; j++)
      {
         if (j < GridColumns)
           GenericLoad[i * GridColumns + j] = 0.0;

      	atemporary[i * GridColumns + j] = 0.0; btemporary[i * GridColumns + j] = 0.0;
         ctemporary[i * GridColumns + j] = 0.0; rtemporary[i * GridColumns + j] = 0.0;

      	if (
         		(j < GridColumns) && (BoxDepth[i * GridColumns + j] > CriticalDepth)
            )
         {
      		if ((i == UpperLayer) && (j < GridColumns))
            {
               DepthVariation[j] =  (DynamicHeight[i * GridColumns + j ] - PastHeight[i * GridColumns + j ]);
         		BoxDepth[i * GridColumns + j ] = BoxDepth[i * GridColumns + j ] +
      		                              	DepthVariation[j];
               PastHeight[i * GridColumns + j ] = DynamicHeight[i * GridColumns + j ];          //***
            }


            if (MyWaterTemperaturePointer != NULL)
   			{
      			double MyWaterTemperature;
   				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(),MyWaterTemperature,
  											       i * GridColumns + j,
            	                                   "Water temperature",
               	                                   ObjectCode);
      			WaterDensity[i * GridColumns + j] = MyPEcoDynClass->GetDensity(Salt[i * GridColumns + j],MyWaterTemperature);

   			}
      		else
         		WaterDensity[i * GridColumns + j] = 1000.0;
         }
         if (BoxWidth[i * (GridColumns + 1) + j] <= 0.0)
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
         if ((j < GridColumns) && (BoxDepth[i * GridColumns + j ] <= CriticalDepth))
         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            WVelocity[i * GridColumns + j] = 0.0;
            //UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
         }
         UVelocityPast[i * (GridColumns + 1) + j ] = UVelocity[i * (GridColumns + 1) + j];

         if (j < GridColumns)
         	WVelocityPast[i * GridColumns + j ] = WVelocity[i * GridColumns + j];

         if (j == 0)
      		BoxCrossSectionalArea[i * (GridColumns + 1) + j] = BoxWidth[i * (GridColumns + 1) + j] *
            	                                                  BoxDepth[i * GridColumns + j];
      	else if ((j > 0) && (j < GridColumns))
         	BoxCrossSectionalArea[i * (GridColumns + 1) + j] = BoxWidth[i * (GridColumns + 1) + j] *
            	                                                  (
               	                                                  BoxDepth[i * GridColumns + j - 1] +
                  	                                               BoxDepth[i * GridColumns + j]
                     	                                         ) / 2.0;
   		else if (j == GridColumns)
   			BoxCrossSectionalArea[i * (GridColumns + 1) + j] = BoxDepth[i * GridColumns + j - 1] *
            	                                                BoxWidth[i * (GridColumns + 1) + j];


         if ((j > 0) && (j < GridColumns))
         	UFlowBetweenBoxes[i * (GridColumns + 1) + j] = UVelocity[i * (GridColumns + 1) + j] *
                                                           BoxCrossSectionalArea[i * (GridColumns + 1) + j];

         MeanBoxWidth = 0.5 * (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]);
         if ((i == 0) ||((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] <= CriticalDepth)))
         		MeanBoxWidthBelow = MeanBoxWidth;
      		else
         		MeanBoxWidthBelow = 0.5 * (BoxWidth[(i - 1) * (GridColumns + 1) + j] + BoxWidth[(i - 1) * (GridColumns + 1) + j + 1]);
         if (j < GridColumns)
         	WFlowBetweenBoxes[i * GridColumns + j] = WVelocity[i * GridColumns + j] *
                                                     BoxLength[i * GridColumns + j] *
                                                     0.5 * (MeanBoxWidthBelow + MeanBoxWidth);
      } //j
	}  //i



   //Debugger(UpperLayer); Debugger(SurfaceLayer[1]);
   int ALayer, OldLowerLayer; double Delta; bool flooding;
   OldLowerLayer = UpperLayer;
   flooding = false;
   for (int j = 0; j < GridColumns; j++)   //Procura da coluna cuja camada superficial se encontra mais baixa
   {
   	  ALayer = SurfaceLayer[j];
      /*while (BoxDepth[ALayer * GridColumns + j] <= CriticalDepth) ALayer--;
      UpperLayer = MIN(UpperLayer, ALayer);*/
      if (BoxDepth[ALayer * GridColumns + j] <= CriticalDepth)
      {
        //Debugger(100); Debugger(MyPEcoDynClass->GetCurrTime());
        //Debugger(UpperLayer); Debugger(ALayer);
        ALayer--;
        UpperLayer = MIN(UpperLayer, ALayer);
        //Debugger(UpperLayer);
      }
      if ((UpperLayer < 0) ||((UpperLayer == 0) && (BoxDepth[j] <= CriticalDepth)))
#ifdef __BORLANDC__
      	MessageBox(0, "No water in column", "EcoDynamo alert - TBiDimensionalVertical::Reinitialize", MB_OK);
#else  // __BORLANDC__
         cerr << "TBiDimensionalVertical::Reinitialize - No water in column" << endl;
#endif  // __BORLANDC__
      if (fabsl(SurfaceLayer[j] - UpperLayer) > 1)
#ifdef __BORLANDC__
      	MessageBox(0, "Loss of more than one layer", "EcoDynamo alert - TBiDimensionalVertical::Reinitialize", MB_OK);
#else  // __BORLANDC__
         cerr << "TBiDimensionalVertical::Reinitialize - loss of more than one layer" << endl;
#endif  // __BORLANDC__
   }  //j
   if ((UpperLayer < GridLines - 1) && (UpperLayer == OldLowerLayer))
   {
        //Debugger(500); Debugger(MyPEcoDynClass->GetCurrTime()); Debugger(UpperLayer); Debugger(OldLowerLayer);
   	    double Depth0;
   	    Delta = 2.0 * CriticalDepth;
   	    for (int j = 0; j < GridColumns; j++)
        {
            Delta = MIN(Delta,BoxDepth[UpperLayer * GridColumns + j] - OldBoxDepth[UpperLayer * GridColumns + j]);
        }
        if (Delta > CriticalDepth) flooding = true;
        /*else
        {        //Situação em que a "UpperLayer" está situada numa linha abaixo da superf. de referência e em que é necessário continuar a ponderar
               //as larguras
      	    for (int j = 0; j < GridColumns; j++)
      	    {
                if (BoxDepth[UpperLayer * GridColumns + j] - OldBoxDepth[UpperLayer * GridColumns + j] > 0.001)
                {
      			    Depth0 = BoxDepth[UpperLayer * GridColumns + j] - OldBoxDepth[UpperLayer * GridColumns + j];
      			    if (j == 0)
            		BoxWidth[UpperLayer * (GridColumns + 1) + j] =
         			(
               		BoxWidth[UpperLayer * (GridColumns + 1) + j] * OldBoxDepth[UpperLayer * GridColumns + j] +
               		BoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j] * Depth0
            		) / (OldBoxDepth[UpperLayer * GridColumns + j] + Depth0);

                }
                else
            	    BoxWidth[UpperLayer * (GridColumns + 1) + j] = OldBoxWidth[UpperLayer * (GridColumns + 1) + j];
                    BoxWidth[UpperLayer * (GridColumns + 1) + j + 1] = CalculateWidth(UpperLayer,j);
      	    }
        }*/

   } //if

   //Debugger(UpperLayer);

   //if (UpperLayer < GridLines - 1) Debugger(12);

   for (int j = 0; j < GridColumns; j++)
   {
      double Depth1, Depth2;
   	  if (UpperLayer < SurfaceLayer[j])  //Houve uma diminuição do número de camadas com água, relativamente à iteracção anterior
      {
         Depth1 = BoxDepth[UpperLayer * GridColumns + j];
         Depth2 = BoxDepth[SurfaceLayer[j] * GridColumns + j];
         BoxDepth[UpperLayer * GridColumns + j] = Depth1 + Depth2;  //Profundidade da "nova" camada resultante da união de duas
         DynamicHeight[UpperLayer * GridColumns + j] = BoxDepth[UpperLayer * GridColumns + j];
         PastHeight[UpperLayer * GridColumns + j] = BoxDepth[UpperLayer * GridColumns + j];     //***

         Salt[UpperLayer * GridColumns + j] =
            (
               Salt[UpperLayer * GridColumns  + j] * BoxVolume[UpperLayer * GridColumns + j] +
               Salt[SurfaceLayer[j] * GridColumns + j] * BoxVolume[SurfaceLayer[j] * GridColumns + j]
            ) /  (BoxVolume[UpperLayer * GridColumns + j] + BoxVolume[SurfaceLayer[j] * GridColumns + j]);
         WaterDensity[UpperLayer * GridColumns + j] =
            (
               WaterDensity[UpperLayer * GridColumns + j] * BoxVolume[UpperLayer * GridColumns + j] +
               WaterDensity[SurfaceLayer[j] * GridColumns + j] * BoxVolume[SurfaceLayer[j] * GridColumns + j]
            ) /  (BoxVolume[UpperLayer * GridColumns + j] + BoxVolume[SurfaceLayer[j] * GridColumns + j]);


         BoxVolume[UpperLayer * GridColumns + j] = BoxVolume[UpperLayer * GridColumns + j] +
                                                   BoxVolume[SurfaceLayer[j] * GridColumns + j];


         if (j == 0)
            BoxWidth[UpperLayer * (GridColumns + 1) + j] =
         	(
               BoxWidth[UpperLayer * (GridColumns + 1) + j] * Depth1 +
               BoxWidth[SurfaceLayer[j] * (GridColumns + 1) + j] * Depth2
            ) / (Depth1 + Depth2);


         BoxWidth[UpperLayer * (GridColumns + 1) + j + 1] = CalculateWidth(UpperLayer,j);
         BoxVolume[SurfaceLayer[j] * GridColumns + j] = 0.0;
         BoxDepth[SurfaceLayer[j] * GridColumns + j] = 0.0;  //Anulação da profundidade da camada que ficou sem água
         DynamicHeight[SurfaceLayer[j] * GridColumns + j] = 0.0;
         PastHeight[SurfaceLayer[j] * GridColumns + j] = 0.0;    //***


         if (j > 0)
         {
         	UVelocity[UpperLayer * (GridColumns + 1) + j] =
            (
               UVelocity[UpperLayer * (GridColumns + 1) + j] * Depth1 +
               UVelocity[SurfaceLayer[j] * (GridColumns + 1) + j] * Depth2
            ) / (Depth1 + Depth2);        //Calculo da velocidade no lado esquerdo da nova
                                          //camada, ponderada pelas profundidades das 2 camadas originais
            UVelocityPast[UpperLayer * (GridColumns + 1) + j] = UVelocity[UpperLayer * (GridColumns + 1) + j];
         	UVelocity[SurfaceLayer[j] * (GridColumns + 1) + j] = 0.0;  //Anulação da velocidade da camada que ficou sem água
            UVelocityPast[SurfaceLayer[j] * (GridColumns + 1) + j] = UVelocity[SurfaceLayer[j] * (GridColumns + 1) + j];

            UFlowBetweenBoxes[UpperLayer * (GridColumns + 1) + j] = UVelocity[UpperLayer * (GridColumns + 1) + j] *
                                                                BoxWidth[UpperLayer * (GridColumns + 1) + j] *
                                                                BoxDepth[UpperLayer * GridColumns + j];
                                         //Calculo do caudal no lado esquerdo da nova camada
         	UFlowBetweenBoxes[SurfaceLayer[j] * (GridColumns + 1) + j] = 0.0; //Anulação do caudal da camada que ficou sem água
         }
         WVelocity[SurfaceLayer[j] * GridColumns + j] = 0.0;
         WVelocityPast[SurfaceLayer[j] * GridColumns + j ] = 0.0;
         WFlowBetweenBoxes[SurfaceLayer[j] * GridColumns + j] = 0.0;
      } //if
      else     //Não houve diminuição do número de camadas, relativamente à iteracção anterior
      if (
      		flooding
            /*(UpperLayer < GridLines - 1) &&
            (BoxDepth[UpperLayer * GridColumns + j] - OldBoxDepth[UpperLayer * GridColumns + j] > CriticalDepth)*/
         )   //Se já tiver havido diminuição do número de camadas e se a agora camada superficial tiver água que
             // justifique, então acrescente-se uma camada
      {

         Depth1 = BoxDepth[UpperLayer * GridColumns + j];       //***

         /*if (j == 1)
         {
           Debugger(5000); Debugger(MyPEcoDynClass->GetCurrTime());
           Debugger(BoxDepth[UpperLayer * GridColumns + j]);
           Debugger(OldBoxDepth[UpperLayer * GridColumns + j]);

         } */
         BoxDepth[(UpperLayer + 1) * GridColumns + j] = BoxDepth[UpperLayer * GridColumns + j] -
                                                        OldBoxDepth[UpperLayer * GridColumns + j];
         BoxVolume[(UpperLayer + 1) * GridColumns + j] = BoxDepth[(UpperLayer + 1) * GridColumns + j] *
                                                         BoxLength[(UpperLayer + 1) * GridColumns + j] *
                                                         0.5 *
                                                         (
                                                            BoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j] +
                                                            BoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j + 1]
                                                         );
         BoxDepth[UpperLayer * GridColumns + j] = OldBoxDepth[UpperLayer * GridColumns + j];    //***
         DynamicHeight[(UpperLayer + 1) * GridColumns + j] = BoxDepth[(UpperLayer + 1) * GridColumns + j];
         PastHeight[(UpperLayer + 1) * GridColumns + j] = DynamicHeight[(UpperLayer + 1) * GridColumns + j];   // ***
         BoxVolume[UpperLayer * GridColumns + j] = BoxVolume[UpperLayer * GridColumns + j] -
                                                   BoxVolume[(UpperLayer + 1) * GridColumns + j];

         if (j == 0)
         	BoxWidth[UpperLayer * (GridColumns + 1) + j] = OldBoxWidth[UpperLayer * (GridColumns + 1) + j];
         BoxWidth[UpperLayer * (GridColumns + 1) + j + 1] = CalculateWidth(UpperLayer,j);
         if (j == 0)
         	BoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j] = OldBoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j];
         BoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j + 1] = CalculateWidth((UpperLayer + 1),j);

         //PastHeight[(UpperLayer + 1) * GridColumns + j] = DynamicHeight[(UpperLayer + 1) * GridColumns + j];   // ****
         DynamicHeight[UpperLayer * GridColumns + j] = 0.0;
         PastHeight[UpperLayer * GridColumns + j] = 0.0;         //***
      	//BoxDepth[UpperLayer * GridColumns + j] = OldBoxDepth[UpperLayer * GridColumns + j];    ***
         Salt[(UpperLayer + 1) * GridColumns + j] = Salt[UpperLayer * GridColumns + j];
         WaterDensity[(UpperLayer + 1) * GridColumns + j] = WaterDensity[UpperLayer * GridColumns + j];
         if (j > 0)
         {
         	UVelocity[(UpperLayer + 1) * (GridColumns + 1) + j] = UVelocity[UpperLayer * (GridColumns + 1) + j];
            UVelocityPast[(UpperLayer + 1) * (GridColumns + 1) + j] = UVelocity[(UpperLayer + 1) * (GridColumns + 1) + j];
            UFlowBetweenBoxes[UpperLayer * (GridColumns + 1) + j] = UVelocity[UpperLayer * (GridColumns + 1) + j] *
                                                                BoxWidth[UpperLayer * (GridColumns + 1) + j] *
                                                                /*BoxDepth[UpperLayer * GridColumns + j]*/Depth1;
                                         //Calculo do caudal no lado esquerdo da nova camada

         	UFlowBetweenBoxes[(UpperLayer + 1) * (GridColumns + 1) + j] =
               UVelocity[(UpperLayer + 1) * (GridColumns + 1) + j] *
               BoxWidth[(UpperLayer + 1) * (GridColumns + 1) + j] *
               BoxDepth[(UpperLayer + 1) * GridColumns + j];
         }  //if

         SurfaceLayer[j]++;

      } //if
   } //j
   if (flooding)
   {
      //Debugger(1000); Debugger(MyPEcoDynClass->GetCurrTime());
      //Debugger(UpperLayer);
      UpperLayer = SurfaceLayer[0];
   }
   for (int j = 0; j < GridColumns; j++)
   {
      if (!flooding)
   		SurfaceLayer[j] = UpperLayer;
   	  TotalDepth[j] = 0.0;
   }


   //Debugger(UpperLayer);

   for (int j = 0; j < GridColumns; j++)
   {
   	double SomaWidth = 0.0; double SomaLength = 0.0; int TrueGridLines = 0.0;
      //PastHeight[UpperLayer * GridColumns + j] = DynamicHeight[UpperLayer * GridColumns + j];  //***
   	for (int i = 0; i <= UpperLayer; i++)
      {
      	if (BoxDepth[i * GridColumns + j] > CriticalDepth)
         {
   			TotalDepth[j] = TotalDepth[j] + BoxDepth[i * GridColumns + j];
            TrueGridLines++;
            //SomaWidth = SomaWidth + (BoxWidth[i * (GridColumns + 1) + j] + BoxWidth[i * (GridColumns + 1) + j + 1]) / 2.0 * BoxDepth[i * GridColumns + j];
            SomaWidth = SomaWidth + BoxWidth[i * (GridColumns + 1) + j] * BoxDepth[i * GridColumns + j];
            SomaLength = SomaLength + BoxLength[i * GridColumns + j];
         }
         if ((i > 0) && (j < GridColumns))
         {
         	VerticalDiffCoefU[i * (GridColumns + 1) + j] =  VerticalMomentumDiffusivity(i,j);
         }
      }
      TotalDepthPast[j] = TotalDepth[j];
      //MeanColumnWidth[j] = SomaWidth / TrueGridLines;
      MeanColumnWidth[j] = SomaWidth / TotalDepth[j];
      MeanColumnLength[j] = SomaLength / TrueGridLines;
   }
   RiverForcing();
}

double TBiDimensionalVertical::CalculateWidth(int i, int j)
{
	if (BoxDepth[i * GridColumns + j] > 0)
   	return 2.0 * BoxVolume[i * GridColumns + j] /
                (
         			BoxLength[i * GridColumns + j] *
            		(BoxDepth[i * GridColumns + j] - DepthVariation[j])
         		  ) - BoxWidth[i * (GridColumns + 1) + j];
   else
      return BoxWidth[i * (GridColumns + 1) + j + 1];
}








double TBiDimensionalVertical::Drag(int i, int j)
{
   //return 0.0;
	if ((j > 0) && (BoxDepth[i * GridColumns + j - 1] > CriticalDepth))
   {

   return ATimeStep / (/*0.5 * */(BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1])) *
          (CoefRugosidade(i,j) + CoefRugosidade(i,j - 1))*
          fabs(UVelocityPast[i * (GridColumns + 1) + j]) *
          (
          	 UVelocityPast[i * (GridColumns + 1) + j]
             - ConvectiveTerm(i,j)
             - BaroclinicPressureTerm(i,j)
             - BarotropicPressureTerm(i,j)
             + HorizontalDiffusion(i,j)
             + Diffusion(3,i,j)
             * (UVelocityPast[(i + 1) * (GridColumns + 1) + j] - UVelocityPast[i * (GridColumns + 1) + j])
          )  /
          (
             1.0 + ATimeStep / (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j - 1])
             * (CoefRugosidade(i,j) + CoefRugosidade(i,j - 1)) * fabs(UVelocityPast[i * (GridColumns + 1) + j])
          );
   }
   else
   return ATimeStep / (BoxDepth[i * GridColumns + j]) *
          CoefRugosidade(i,j) *
          fabs(UVelocityPast[i * (GridColumns + 1) + j]) * /*UVelocityPast[i * (GridColumns + 1) + j]* */
          (
          	 UVelocityPast[i * (GridColumns + 1) + j]
             - ConvectiveTerm(i,j)
             - BaroclinicPressureTerm(i,j)
             - BarotropicPressureTerm(i,j)
             + HorizontalDiffusion(i,j)
             + Diffusion(3,i,j)
             * (UVelocityPast[(i + 1) * (GridColumns + 1) + j] - UVelocityPast[i * (GridColumns + 1) + j])
          )  /
          (
             1.0 + ATimeStep / (BoxDepth[i * GridColumns + j] + BoxDepth[i * GridColumns + j])
             * (CoefRugosidade(i,j) + CoefRugosidade(i,j)) * fabs(UVelocityPast[i * (GridColumns + 1) + j])
          );
}


double TBiDimensionalVertical::CorrectionOfHorizontalDiffCoef(double ADepth, double AVelocityComponent)
{
    double const ReferenceSpeed = 1.0; //1.0 ms-1
    double const ReferenceDepth = 10.0; //m
    double CorrectionFactor;
    CorrectionFactor =  pow(AVelocityComponent/ReferenceSpeed,2) * ADepth / ReferenceDepth;
    //CorrectionFactor = 0.0;
    return MAX(1.0, CorrectionFactor);
}


void TBiDimensionalVertical::RiverForcing()
{
   if (MyRiverRecord != NULL)
   {
      int MyHour, LineIndex, ColumnIndex, LineNumber, ColumnNumber;
      double HourlyRiverFlow;
      NumberOfHoursForFlowTimeSeries = MyPEcoDynClass->GetNumberOfHoursForFlowTimeSeries();

      if (TotalHours >= NumberOfHoursForFlowTimeSeries)
         TotalHours = 0.0;
      MyHour = floor(TotalHours);
      //Debugger(NumberOfHoursForFlowTimeSeries); Debugger(TotalHours);
      for (int River = 0; River < NumberOfRivers; River++)
      {
        LineIndex = MyRiverRecord[River].LineCoordinate;
        ColumnIndex = MyRiverRecord[River].ColumnCoordinate;
        HourlyRiverFlow = MyRiverRecord[River].RiverFlow[MyHour] + (TotalHours - MyHour) *
                                (MyRiverRecord[River].RiverFlow[MyHour + 1] - MyRiverRecord[River].RiverFlow[MyHour]) /
                                (1.0);

        if ((MyRiverRecord[River].BoundaryNumber == 2) || (MyRiverRecord[River].BoundaryNumber == 4))
        {
            if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns))
            {

               UFlowBetweenBoxes[LineIndex * (GridColumns + 1) + ColumnIndex + 1] = HourlyRiverFlow;  //Western river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex == GridColumns))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the east located at eastern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "TBiDimensionalVertical::RiverForcing - River flow to the East located at eastern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex >= 0))
            {
               UFlowBetweenBoxes[LineIndex * (GridColumns + 1) + ColumnIndex] = HourlyRiverFlow;  //Eastern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex < 0))
#ifdef __BORLANDC__
               MessageBox(0,"Hydrodynamic bidimensional object - RiverForcing",
                            "River flow to the West located at western grid limit",
                            MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                cerr << "TBiDimensionalVertical::RiverForcing - River flow to the West located at western grid limit" << endl;
#endif  // __BORLANDC__
        }
      }
   }
   TotalHours = TotalHours + MyPEcoDynClass->GetTimeStep() / HOURSTOSECONDS;
}



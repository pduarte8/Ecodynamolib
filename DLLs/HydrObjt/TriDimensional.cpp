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

TTriDimensional::TTriDimensional(TEcoDynClass* PEcoDynClass,
        char* className) : TBiDimensionalIntertidal(PEcoDynClass, className)

{
    BuildTriDimensionalIntertidal();
}

void TTriDimensional::Debugger(double bug)
{
	char szBug[64];
	sprintf(szBug, "%10.12f", bug);
	//MessageBox(0,(LPSTR(szBug)),"Debug", MB_OK);
#ifdef __BORLANDC__
        MessageBox(0,szBug,"Debug", MB_OK);
#else  // __BORLANDC__
        cerr << "Debug " << szBug << endl;
#endif  // __BORLANDC__
}

void TTriDimensional::BuildTriDimensionalIntertidal()
{
    /*
     * Temporariamente activo até especificação do multithread
     * para o TriDimensional
     */
    //EnableMultiThread = false;
    /* ... */

    /*long double x1 = 1765.000000000000004, x2 = 1763.999999995550524;
    long double xxx = x1 + x2;
    Debugger(x1);
    Debugger(x2);
    Debugger(xxx);  */
    int index, index1;

    ATimeStep = MyPEcoDynClass->GetTimeStep();
    HMINC = 0.5; //m - Default value
    WindStressCoef = 0.002; //kg/m3
    MaxVertDiffusivity = 0.01;
    delta = 0.5;
    //MinDepthForAdvection = 0.1; //m seguindo a tese do Ramiro 28/1/2004
    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
    GridLayers = MyPEcoDynClass->GetNumberOfLayers();
    UpperLayer = GridLayers - 1;
    if (NumberOfBoxes > 0)
    {
      SurfaceBox = new int[GridColumns * GridLines];
      SurfaceLayer = new int[GridColumns * GridLines];
      DepthVariation = new double[GridColumns * GridLines];
      Soma0 = new double[(GridColumns + 1) * (GridLines + 1)];
      Soma1 = new double[GridColumns * GridLines]; Soma2 = new double[GridColumns * GridLines];
      Soma3 = new double[(GridColumns + 1) * (GridLines + 1)]; Soma4 = new double[(GridColumns + 1) * (GridLines + 1)];
      Soma5 = new double[(GridColumns + 1) * (GridLines + 1)]; Soma6 = new double[(GridColumns + 1) * (GridLines + 1)];
      Soma7 = new double[(GridColumns + 1) * (GridLines + 1)]; Soma8 = new double[(GridColumns + 1) * (GridLines + 1)];
      Soma9 = new double[GridColumns * GridLines]; Soma10 = new double[GridColumns * GridLines]; Soma11 = new double[GridColumns * GridLines];
      ASum1 = new double[GridColumns * GridLines]; ASum2 = new double[GridColumns * GridLines];

      UU1 = new double[NumberOfBoxes + GridLines * GridLayers];
      UU2 = new double[NumberOfBoxes + GridLines * GridLayers];
      UU3 = new double[NumberOfBoxes + GridLines * GridLayers];
      UU4 = new double[NumberOfBoxes + GridLines * GridLayers];
      UVU = new double[NumberOfBoxes + GridLines * GridLayers];
      UW = new double[NumberOfBoxes + GridLines * GridLayers];

      VV1 = new double[NumberOfBoxes + GridColumns * GridLayers];
      VV2 = new double[NumberOfBoxes + GridColumns * GridLayers];
      VV3 = new double[NumberOfBoxes + GridColumns * GridLayers];
      VV4 = new double[NumberOfBoxes + GridColumns * GridLayers];
      UVV = new double[NumberOfBoxes + GridColumns * GridLayers];
      VW = new double[NumberOfBoxes + GridColumns * GridLayers];

      MeanColumnWidth = new double[GridColumns * GridLines];
      MeanColumnLength = new double[GridColumns * GridLines];
      atemporaryCont = new double[GridColumns * GridLines];
      btemporaryCont = new double[GridColumns * GridLines];
      ctemporaryCont = new double[GridColumns * GridLines];
      rtemporaryCont = new double[GridColumns * GridLines];

      WVelocity = new double[NumberOfBoxes];
      WVelocityPast = new double[NumberOfBoxes];
      UVelocity = new double[NumberOfBoxes + GridLines * GridLayers];
      VVelocity = new double[NumberOfBoxes + GridColumns * GridLayers];
      UVelocityPast = new double[NumberOfBoxes + GridLines * GridLayers];
      VVelocityPast = new double[NumberOfBoxes + GridColumns * GridLayers];
      UFlowBetweenBoxes = new double[NumberOfBoxes + GridLines * GridLayers];
      VFlowBetweenBoxes = new double[NumberOfBoxes + GridColumns * GridLayers];
      VerticalDiffCoefU = new double[NumberOfBoxes + GridLines * GridLayers];
      VerticalDiffCoefV = new double[NumberOfBoxes + GridColumns * GridLayers];
      Rugosity = new double[NumberOfBoxes];
      BarotropicPressureValue = new double[NumberOfBoxes];
      BaroclinicPressureValue = new double[NumberOfBoxes];
      ConvectionValue = new double[NumberOfBoxes];
      DiffusionValue = new double[NumberOfBoxes];
      Pressure = new double[NumberOfBoxes];

      for (int i = 0; i <= GridLines; i++)
      {
        for (int j = 0; j <= GridColumns; j++)
        {
           index = Get2DIndex(i,j);
           index1 = Get2DIndex(i,j, GridColumns + 1);
           Soma0[index1] = 0.0; Soma3[index1] = 0.0; Soma4[index1] = 0.0; Soma5[index1] = 0.0;
           Soma6[index1] = 0.0; Soma7[index1] = 0.0; Soma8[index1] = 0.0;
           if (index < GridColumns * GridLines)
           {
             atemporaryCont[index] = 0.0;
             btemporaryCont[index] = 0.0;
             ctemporaryCont[index] = 0.0;
             rtemporaryCont[index] = 0.0;
           }
           if ((i < GridLines) && (j < GridColumns))
           {
              SurfaceBox[index] = Get3DIndex(i,j,UpperLayer);
              SurfaceLayer[index] = UpperLayer;
              DepthVariation[index] = 0.0;
              Soma1[index] = 0.0; Soma2[index] = 0.0; Soma9[index] = 0.0; Soma10[index] = 0.0; Soma11[index] = 0.0;
              MeanColumnWidth[index] = MyPEcoDynClass->GetChannelWidth(index);
              MeanColumnLength[index] = MyPEcoDynClass->GetBoxLength(index);
              ASum1[index] = 0.0; ASum2[index] = 0.0;
           }
           for (int k = 0; k < GridLayers; k++)
           {
              if (j < GridColumns)
              {
                 VVelocity[Get3DIndexForVVelocity(i,j,k)] = 0.0; VVelocityPast[Get3DIndexForVVelocity(i,j,k)] = 0.0;
                 VFlowBetweenBoxes[Get3DIndexForVVelocity(i,j,k)] = 0.0;
                 VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] = 0.0;
                 VV1[Get3DIndexForVVelocity(i,j,k)] = 0.0; VV2[Get3DIndexForVVelocity(i,j,k)] = 0.0;
                 VV3[Get3DIndexForVVelocity(i,j,k)] = 0.0; VV4[Get3DIndexForVVelocity(i,j,k)] = 0.0;
                 UVV[Get3DIndexForVVelocity(i,j,k)] = 0.0; VW[Get3DIndexForVVelocity(i,j,k)] = 0.0;
              }
              if (i < GridLines)
              {
                 UVelocity[Get3DIndexForUVelocity(i,j,k)] = 0.0; UVelocityPast[Get3DIndexForUVelocity(i,j,k)] = 0.0;
                 UFlowBetweenBoxes[Get3DIndexForUVelocity(i,j,k)] = 0.0;
                 VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] = 0.0;
                 UU1[Get3DIndexForUVelocity(i,j,k)] = 0.0; UU2[Get3DIndexForUVelocity(i,j,k)] = 0.0;
                 UU3[Get3DIndexForUVelocity(i,j,k)] = 0.0; UU4[Get3DIndexForUVelocity(i,j,k)] = 0.0;
                 UVU[Get3DIndexForUVelocity(i,j,k)] = 0.0; UW[Get3DIndexForUVelocity(i,j,k)] = 0.0;
              }
              if ((i < GridLines) && (j < GridColumns))
              {
                 WVelocity[Get3DIndex(i,j,k)] = 0.0; WVelocityPast[Get3DIndex(i,j,k)] = 0.0;
                 Rugosity[Get3DIndex(i,j,k)] = 0.0;
                 BarotropicPressureValue[Get3DIndex(i,j,k)] = 0.0;
                 BaroclinicPressureValue[Get3DIndex(i,j,k)] = 0.0;
                 ConvectionValue[Get3DIndex(i,j,k)] = 0.0;
                 DiffusionValue[Get3DIndex(i,j,k)] = 0.0;
                 BoxWidth[Get3DIndex(i,j,k)] = MyPEcoDynClass->GetChannelWidth(Get3DIndex(i,j,k));
                 BoxVolume[Get3DIndex(i,j,k)] = MyPEcoDynClass->GetBoxVolume(Get3DIndex(i,j,k));
                 PastBoxVolume[Get3DIndex(i,j,k)] = BoxVolume[Get3DIndex(i,j,k)];
                 Pressure[Get3DIndex(i,j,k)] = 0.0;
              }
           }
        }
      }
    }
    int X,Y, XV, YV;
    double MyValue;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
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
                    /*if (strcmp(MyParameter, "Gravity") == 0)
                    {
                            PReadWrite->ReadNumber(X+3, i, Gravity);
                    }
                    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        //Debugger(MyValue);
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                                CoefManning[j] = MyValue;
                        }
                    }
                    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, HorizontalDiffCoef);
                    }
                    else*/
                    if (strcmp(MyParameter, "Latitude") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyLatitude);
                    }
                    else if (strcmp(MyParameter, "Critical depth for land boundary") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, CriticalDepthForLandBoundary);
                    }
                    else if (strcmp(MyParameter, "Rugosity") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        //Debugger(MyValue);
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                           Rugosity[j] = MyValue;
                        }
                    }
                    else if (strcmp(MyParameter, "MaxVertDiffusivity") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        //Debugger(MyValue);
                        MaxVertDiffusivity = MyValue;
                    }
                    else if (strcmp(MyParameter, "WindStressCoef") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        //Debugger(MyValue);
                        WindStressCoef = MyValue;
                    }
                    else if (strcmp(MyParameter, "delta") == 0)
                    {
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        //Debugger(delta);
                        delta = MyValue;
                    }
                }
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

        //End reading parameters file
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
            // but not filled because the direction is wrong
                for (int i = 0; i < NumberOfVariables; i++)
                {
                    PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                    strcpy(VariableNameArray[i], MyVariable);
                
                    if (strcmp(MyVariable, "U Velocity") == 0)
                    {
                        if (!PReadWrite->FindString("U Velocity values", XV, YV))
                        {
                            XV = X + 2 + i;
                            YV = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes + GridLines * GridLayers; j++)
                        {
                                PReadWrite->ReadNumber(XV , YV + 1 + j, UVelocity[j]);
                                UVelocityPast[j] = UVelocity[j];
                        }
                    }
                    else if (strcmp(MyVariable, "V Velocity") == 0)
                    {
                        if (!PReadWrite->FindString("V Velocity values", XV, YV))
                        {
                            XV = X + 2 + i;
                            YV = Y;
                        }
                            for (int j = 0; j < NumberOfBoxes + GridColumns * GridLayers; j++)
                            {
                                    PReadWrite->ReadNumber(XV, YV + 1 + j, VVelocity[j]);
                                    VVelocityPast[j] = VVelocity[j];
                            }
                    }
                    else if (strcmp(MyVariable, "Box depth values") == 0)
                    {
                        if (!PReadWrite->FindString("Box depth values", XV, YV))
                        {
                            XV = X + 2 + i;
                            YV = Y;
                        }
                            for (int j = 0; j < NumberOfBoxes; j++)
                            {
                                    PReadWrite->ReadNumber(XV, YV + 1 + j, BoxDepth[j]);
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
                        }
                    }
                    else if (strcmp(MyVariable, "U Flow") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                        for (int j = 0; j < NumberOfBoxes + GridLines * GridLayers; j++)
                        {
                           UFlowBetweenBoxes[j] = MyValue;
                        }
                    }
                    else if (strcmp(MyVariable, "V Flow") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                        for (int j = 0; j < NumberOfBoxes + GridColumns * GridLayers; j++)
                        {
                           VFlowBetweenBoxes[j] = MyValue;
                        }
                    }
                    else if (strcmp(MyVariable, "Salinity") == 0)
                    {
                        if (!PReadWrite->FindString("Salinity values", XV, YV))
                        {
                            XV = X + 2 + i;
                            YV = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                           PReadWrite->ReadNumber(XV, YV + 1 + j, MyValue);
                           Salt[j] = MyValue;
                        }
                    }
                    else if (strcmp(MyVariable, "Water density") == 0)
                    {
                        if (!PReadWrite->FindString("Water density values", XV, YV))
                        {
                            XV = X + 2 + i;
                            YV = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                                PReadWrite->ReadNumber(XV, YV + 1 + j, WaterDensity[j]);
                        }
                       //Debugger(WaterDensity[NumberOfBoxes-1]);
                    }
                    else if (strcmp(MyVariable, "Salt load") == 0)
                    {
                        if (!PReadWrite->FindString("Salt load values", XV, YV))
                        {
                            XV = X + 2 + i;
                            YV = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, SaltLoad[j]);
                            //if (SaltLoad[j] > 0) Debugger(SaltLoad[j]);
                        }
                    }
                }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object Transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TTriDimensional::BuildTriDimensionalIntertidal - variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }

	for (int i = 0; i < NumberOfBoxes; i++)
    {
        if (BoxDepth[i] > CriticalDepthForLandBoundary)
            BoxArray[i].Dummybool2 = true;
        else
            BoxArray[i].Dummybool2 = false;
    }
    MinDepthForAdvection = MAX(0.1,CriticalDepthForLandBoundary); //m seguindo a tese do Ramiro 28/1/2004
    SemiTimeStep = 1;
}

TTriDimensional::~TTriDimensional()
{
    freeMemory();
}

void TTriDimensional::freeMemory()
{
    try {
       if (NumberOfBoxes > 0)
       {
         delete [] SurfaceBox;
         delete [] SurfaceLayer;
         delete [] DepthVariation;
         delete [] Soma0;
         delete [] Soma1;
         delete [] Soma2;
         delete [] Soma3;
         delete [] Soma4;
         delete [] Soma5;
         delete [] Soma6;
         delete [] Soma7;
         delete [] Soma8;
         delete [] Soma9;
         delete [] Soma10;
	     delete [] Soma11;
         delete [] UU1;
         delete [] UU2;
         delete [] UU3;
         delete [] UU4;
         delete [] UW;
         delete [] UVU;
         delete [] VV1;
         delete [] VV2;
         delete [] VV3;
         delete [] VV4;
         delete [] VW;
         delete [] UVV;
         delete [] MeanColumnLength;
         delete [] MeanColumnWidth;
         //delete [] MyLoadRecord;
         delete [] WVelocity;
         delete [] WVelocityPast;
         delete [] VerticalDiffCoefU;
         delete [] VerticalDiffCoefV;
         delete [] Rugosity;
         delete [] BarotropicPressureValue;
         delete [] BaroclinicPressureValue;
         delete [] ConvectionValue;
         delete [] DiffusionValue;
         delete [] atemporaryCont;
         delete [] btemporaryCont;
         delete [] ctemporaryCont;
         delete [] rtemporaryCont;
	     delete [] ASum1;
	     delete [] ASum2;
	     delete [] Pressure;
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
        cerr << "TTriDimensional::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}


double TTriDimensional::GetParameterValue(char* MyParameter)
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

    // parameters from TTriDimensional
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        value = MyLatitude;
    }
    else if (strcmp(MyParameter, "Critical depth for land boundary") == 0)
    {
        value = CriticalDepthForLandBoundary;
    }
    else if (strcmp(MyParameter, "Rugosity") == 0)
    {
        value = Rugosity[0];
    }
    else if (strcmp(MyParameter, "MaxVertDiffusivity") == 0)
    {
        value = MaxVertDiffusivity;
    }
    else if (strcmp(MyParameter, "WindStressCoef") == 0)
    {
        value = WindStressCoef;
    }
    else if (strcmp(MyParameter, "delta") == 0)
    {
        value = delta;
    }
    else
        value = 0.0;
    return value;
}

bool TTriDimensional::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TTriDimensional
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        MyLatitude = value;
    }
    else if (strcmp(MyParameter, "Critical depth for land boundary") == 0)
    {
        CriticalDepthForLandBoundary = value;
    }
    else if (strcmp(MyParameter, "Rugosity") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
        {
            Rugosity[j] = value;
        }
    }
    else if (strcmp(MyParameter, "MaxVertDiffusivity") == 0)
    {
        MaxVertDiffusivity = value;
    }
    else if (strcmp(MyParameter, "WindStressCoef") == 0)
    {
        WindStressCoef = value;
    }
    else if (strcmp(MyParameter, "delta") == 0)
    {
        delta = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.09
bool TTriDimensional::SaveVariables()
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
     * save variables' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "U Flow") == 0)
            PReadWrite->WriteCell(UFlowBetweenBoxes[0], 6);

        else if (strcmp(VariableNameArray[i], "V Flow") == 0)
            PReadWrite->WriteCell(VFlowBetweenBoxes[0], 6);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("U Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("V Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("W Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Salinity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Box depth");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Dynamic height values");
    PReadWrite->WriteSeparator(true);

    int m = MAX(GridLines * GridLayers, GridColumns * GridLayers);
    for (int j = 0; j < (NumberOfBoxes + m); j++)
    {
        if (j < NumberOfBoxes) {
            PReadWrite->WriteCell(UVelocity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(VVelocity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(WVelocity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Salt[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(WaterDensity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoxDepth[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(DynamicHeight[j], 8);
        }
        else {
            if (GridLines > GridColumns) {
                if (j >= (NumberOfBoxes + GridColumns * GridLayers)) {
                    PReadWrite->WriteCell(UVelocity[j], 8);
                }
                else {
                    PReadWrite->WriteCell(UVelocity[j], 8);
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(VVelocity[j], 8);
                }
            }
            else {
                if (j >= (NumberOfBoxes + GridLines * GridLayers)) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(VVelocity[j], 8);
                }
                else {
                    PReadWrite->WriteCell(UVelocity[j], 8);
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(VVelocity[j], 8);
                }
            }
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TTriDimensional::SaveParameters()
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
        else if (strcmp(ParametersNameArray[i], "Latitude") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(MyLatitude, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Critical depth for land boundary") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(CriticalDepthForLandBoundary, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Rugosity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Rugosity[0], 6);
        }
        else if (strcmp(ParametersNameArray[i], "MaxVertDiffusivity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(MaxVertDiffusivity, 6);
        }
        else if (strcmp(ParametersNameArray[i], "WindStressCoef") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(WindStressCoef, 6);
        }
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteCell("Coeficiente de Manning values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(CoefManning[j], 6);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TTriDimensional::Go()
{
    //if (samples == 0) UpdateMeanSamples();
    //Esquema de Leendertsee (ver pág. 124 da tese do Flávio)
    //Debugger(SemiTimeStep);
    if (SemiTimeStep == 1)
    {
        EquationOfMotionVExplicit();
        TideForcing();
        Continuity();
        EquationOfMotionUImplicit();
    }
    else if (SemiTimeStep == 2)
    {
        EquationOfMotionUExplicit();
        TideForcing();
        Continuity();
        EquationOfMotionVImplicit();  
    }
    VerticalVelocity();
    GenericLoad = SaltLoad;
    AdvectDiffuse(Salt);
    UpdateOfTidalBoundariesVelocities();
    UpdateMeanSamples();
}

void TTriDimensional::UpdateMeanSamples()
{
    samples++;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        MeanUVelocity[i] += getUVelocity3D(i);
        MeanVVelocity[i] += getVVelocity3D(i);
        MeanUFlow[i] += getUFlow(i);
        MeanVFlow[i] += getVFlow(i);
    }
}

void TTriDimensional::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
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
       	Value = CoefRugosidade(i, j, k);
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
            MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 6",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TTriDimensional::Inquiry 6 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.05.28
bool TTriDimensional::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Dynamic height") == 0)
        DynamicHeight[BoxNumber] = Value;
    else if (strcmp(VariableName, "Salinity") == 0)
        Salt[BoxNumber] = Value;
    else if (strcmp(VariableName, "Box depth") == 0)
    {
        BoxDepth[BoxNumber] = Value;
        BoxVolume[BoxNumber] = BoxDepth[BoxNumber] * (BoxWidth[BoxNumber] * BoxLength[BoxNumber]);
    }
    else if (strcmp(VariableName,"Water density") == 0)
       	WaterDensity[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow discharge") == 0)
		FlowDischarge[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow uptake") == 0)
		FlowUptake[BoxNumber] = Value;
    else if (strcmp(VariableName, "UpperLayer") == 0)
        UpperLayer = Value;
    else
        rc = false;

    return rc;
}


double TTriDimensional::getUVelocity3D(int ABoxNumber)
{
    int i = GetLineIndex3D(ABoxNumber);
    int j = GetColumnIndex3D(ABoxNumber);
    int k = GetLayerIndex(ABoxNumber);
    return UVelocity[Get3DIndexForUVelocity(i,j,k)]; //m / s
}

double TTriDimensional::getVVelocity3D(int ABoxNumber)
{
    int i = GetLineIndex3D(ABoxNumber);
    int j = GetColumnIndex3D(ABoxNumber);
    int k = GetLayerIndex(ABoxNumber);
    return -VVelocity[Get3DIndexForVVelocity(i,j,k)]; //m / s A velocidade v é devolvida com sinal menos para que o Noesys
                                          // a entenda como de baixo para cima
}

double TTriDimensional::getWVelocity3D(int ABoxNumber)
{
    int i = GetLineIndex3D(ABoxNumber);
    int j = GetColumnIndex3D(ABoxNumber);
    int k = GetLayerIndex(ABoxNumber);
    return -WVelocity[Get3DIndex(i,j,k)]; //m / s A velocidade v é devolvida com sinal menos para que o Noesys
                                          // a entenda como de baixo para cima
}

double TTriDimensional::getUFlow(int ABoxNumber)
{
    double Value, AVelocity;
    int i = GetLineIndex3D(ABoxNumber);
    int j = GetColumnIndex3D(ABoxNumber);
    int k = GetLayerIndex(ABoxNumber);
    if (SemiTimeStep == 1) AVelocity = UVelocity[Get3DIndexForUVelocity(i,j,k)];
    else if (SemiTimeStep == 2) AVelocity = UVelocityPast[Get3DIndexForUVelocity(i,j,k)];

    if ((j == 0) ||
        ((j > 0) && (BoxDepth[Get3DIndex(i,j-1,k)] <= 0.0))
       )
        Value = AVelocity * BoxDepth[Get3DIndex(i,j,k)] * BoxWidth[Get3DIndex(i,j,k)];
    else
    {
        Value = AVelocity *
                  (0.5 * (BoxDepth[Get3DIndex(i,j-1,k)] + BoxDepth[Get3DIndex(i,j,k)])) *
                  BoxWidth[Get3DIndex(i,j,k)];
    }
    return Value;
}

double TTriDimensional::getVFlow(int ABoxNumber)
{
    double Value, AVelocity;
    int i = GetLineIndex3D(ABoxNumber);
    int j = GetColumnIndex3D(ABoxNumber);
    int k = GetLayerIndex(ABoxNumber);
    if (SemiTimeStep == 1) AVelocity = VVelocityPast[Get3DIndexForVVelocity(i,j,k)];
    else if (SemiTimeStep == 2) AVelocity = VVelocity[Get3DIndexForVVelocity(i,j,k)];

    if ((i == 0) ||
        ((i > 0) && (BoxDepth[Get3DIndex(i-1,j,k)] <= 0.0))
       )
        Value = -AVelocity * BoxDepth[Get3DIndex(i,j,k)] * BoxLength[Get3DIndex(i,j,k)];
    else
    {
        Value = -AVelocity *
                  (0.5 * (BoxDepth[Get3DIndex(i-1,j,k)]+ BoxDepth[Get3DIndex(i,j,k)]))*
                  BoxLength[Get3DIndex(i,j,k)];
    }
    return Value;
}

void TTriDimensional::Reinitialize()
{
    double const MinCritDepth = 0.01;  //0.00001 m
    double const TINNY = 0.00000000001;
    int index3D, index2D, index3DU;
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    double MyWaterTemperature, CumulativePressure, AddedPressure;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
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
            }
            if ((BoxArray[index3D].Dummybool2)  && (BoxDepth[index3D] <= CriticalDepthForLandBoundary))
               BoxArray[index3D].Dummybool2 = false;
         }
      }
   }
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         for (int k = 0; k <= UpperLayer; k++)
         {
            index3D = Get3DIndex(i,j,k);
            if (k == UpperLayer)
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
   RiverForcing();
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

                if (MyWaterTemperaturePointer != NULL)
                {
                   MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,index3D,"Water temperature",ObjectCode);
                   WaterDensity[index3D] = MyPEcoDynClass->GetDensity(Salt[index3D],MyWaterTemperature,Pressure[index3D] * pow(10.0,-4) /*Converting pressure from Pascal to DeciBar*/);
                }
                else
                   WaterDensity[index3D] = MyPEcoDynClass->GetDensity(Salt[index3D],15.0,Pressure[index3D] * pow(10.0,-4) /*Converting pressure from Pascal to DeciBar*/);
              }
            }
         }
      }
   }
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
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



void TTriDimensional::Continuity()
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
                            if (
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
                                 ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) / (1.0 + DragOfUEquation(i,j,k));
                            }
                            if (
                                 (j < GridColumns - 1) && (k > 0) && (BoxArray[Get3DIndex(i,j+1,k)].Dummybool2)
                                 && (!BoxArray[Get3DIndex(i,j+1,k - 1)].Dummybool2) &&
                                 (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) /*&&
                                 (i < GridLines) */
                                 )
                             {
                                 AProduct = DragOfUEquation(i,j + 1,k) *
                                        (BoxDepth[Get3DIndex(i,j+1,k)] + BoxDepth[Get3DIndex(i,j,k)]);
                                 ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + (AProduct / ATimeStep) / (1.0 + DragOfUEquation(i,j+1,k));

                             }
                            /*if (j < GridColumns)
                            {
                               Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] + (FlowDischarge[Get3DIndex(i,j,k)] - FlowUptake[Get3DIndex(i,j,k)]);
                               Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] + VContributionToContinuity(i, j, k);
                            }*/
                         }
                         if ((j < GridColumns)&& (BoxArray[Get3DIndex(i,j,k)].Dummybool1) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
                         {
                            Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] + (FlowDischarge[Get3DIndex(i,j,k)] - FlowUptake[Get3DIndex(i,j,k)]);
                            Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] + VContributionToContinuity(i, j, k);
                         }
                      }
                   }
                }  //k
                if (
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
                   ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) / (1.0 + DragOfUEquation(i,j,BottomBox));

                }

                if (
                  (j < GridColumns - 1)
                  && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool2)
                  && (BoxArray[Get3DIndex(i,j+1,BottomBox)].Dummybool2)
                  && (BoxArray[Get3DIndex(i,j,BottomBox)].Dummybool1)
                  && (BoxArray[Get3DIndex(i,j+1,BottomBox)].Dummybool1)
                )
                {
                  ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + DragOfUEquation(i,j+1,BottomBox) / ATimeStep *
                         (BoxDepth[Get3DIndex(i,j+1,BottomBox)] + BoxDepth[Get3DIndex(i,j,BottomBox)]) /
                         (1.0 + DragOfUEquation(i,j+1,BottomBox));
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
                  atemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep * ATimeStep * (-Soma1[Get2DIndex(i,j)] +  ATimeStep * ASum1[Get2DIndex(i,j)]) /
                      (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)] * 0.5 * (MeanColumnLength[Get2DIndex(i,j-1)] + MeanColumnLength[Get2DIndex(i,j)]));
                }
                else
                  atemporaryCont[Get2DIndex(i,j)] = 0.0;
                btemporaryCont[Get2DIndex(i,j)] = - atemporaryCont[Get2DIndex(i,j)];

                if ((j < GridColumns - 1) && (!CheckForRivers(i,j+1)))
                {
                  ctemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep * ATimeStep * (-Soma2[Get2DIndex(i,j)] + ATimeStep * ASum2[Get2DIndex(i,j)]) /
                      (/*4.0*/2.0 * MeanColumnLength[Get2DIndex(i,j)] * 0.5 * (MeanColumnLength[Get2DIndex(i,j)] + MeanColumnLength[Get2DIndex(i,j+1)]));
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
                                Soma5[Get2DIndex(i,j, GridColumns+1)] = Soma5[Get2DIndex(i,j, GridColumns+1)] +
                                (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                                NSBaroclinicPressureTerm(i,i-1,j,k) /
                                (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);

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
                             if (
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
                                      )/ (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)]);
                                 ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) / (1.0 + DragOfVEquation(i,j,k));
                             }
                             if (
                                 (i < GridLines - 1) && (k > 0) && (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2)
                                 && (!BoxArray[Get3DIndex(i+1,j,k - 1)].Dummybool2) &&
                                 (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) /*&&
                                 (i < GridLines)   */
                                 )
                             {
                                 AProduct = DragOfVEquation(i + 1,j,k) *
                                        (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[Get3DIndex(i,j,k)]);
                                 ASum2[Get2DIndex(i,j)] = ASum2[Get2DIndex(i,j)] + (AProduct / ATimeStep) / (1.0 + DragOfVEquation(i + 1,j,k));

                             }
                             /*if (i < GridLines)
                             {
                                Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] + (FlowDischarge[Get3DIndex(i,j,k)] - FlowUptake[Get3DIndex(i,j,k)]);
                                Soma10[Get2DIndex(i,j)] = Soma10[Get2DIndex(i,j)] + UContributionToContinuity(i, j, k);
                             } */
                         }
                         if ((i < GridLines)  && (BoxArray[Get3DIndex(i,j,k)].Dummybool1) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
                         {
                            Soma9[Get2DIndex(i,j)] = Soma9[Get2DIndex(i,j)] + (FlowDischarge[Get3DIndex(i,j,k)] - FlowUptake[Get3DIndex(i,j,k)]);
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
                ASum1[Get2DIndex(i,j)] = ASum1[Get2DIndex(i,j)] + (AProduct / ATimeStep) / (1.0 + DragOfVEquation(i,j,BottomBox));
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
                       (1.0 + DragOfVEquation(i+1,j,BottomBox));
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
                     atemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep  * ATimeStep * (-Soma1[Get2DIndex(i,j)] +  ATimeStep * ASum1[Get2DIndex(i,j)]) /
                            (/*4.0*/2.0 * MeanColumnWidth[Get2DIndex(i,j)] * 0.5 * (MeanColumnWidth[Get2DIndex(i-1,j)] + MeanColumnWidth[Get2DIndex(i,j)]));
                }
                else
                     atemporaryCont[Get2DIndex(i,j)] = 0.0;
                btemporaryCont[Get2DIndex(i,j)] = - atemporaryCont[Get2DIndex(i,j)];
                if ((i < GridLines - 1) && (!CheckForRivers(i+1,j)))
                {
                     ctemporaryCont[Get2DIndex(i,j)] = Gravity * ATimeStep * ATimeStep * (-Soma2[Get2DIndex(i,j)] + ATimeStep * ASum2[Get2DIndex(i,j)]) /
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



double TTriDimensional::VContributionToContinuity(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   double SouthernComponent, NorthernComponent;
   SouthernComponent = 0.0; NorthernComponent = 0.0;
   //SouthernComponent
   if ((i > 0) && (i < GridLines) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2))
       SouthernComponent = (VVelocityPast[Get3DIndexForVVelocity(i,j,k)] *
                           (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) /
                           (2.0 * MeanColumnWidth[Get2DIndex(i,j)]));
   else
   if ((i == 0) || ((i > 0) && (i < GridLines) && (!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)))
       SouthernComponent = VVelocityPast[Get3DIndexForVVelocity(i,j,k)] *
                           BoxDepth[Get3DIndex(i,j,k)] /
                           MeanColumnWidth[Get2DIndex(i,j)];
   //NorthernComponent
   if ((i < GridLines - 1) && (i >= 0) && (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2))
       NorthernComponent = (VVelocityPast[Get3DIndexForVVelocity(i + 1,j,k)] *
                           (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i + 1,j,k)]) /
                           (2.0 * MeanColumnWidth[Get2DIndex(i,j)]));
   else
   if ((i == GridLines - 1) || ((i >= 0) && (i < GridLines) && (!BoxArray[Get3DIndex(i + 1,j,k)].Dummybool2)))
       NorthernComponent = VVelocityPast[Get3DIndexForVVelocity(i + 1,j,k)] *
                           BoxDepth[Get3DIndex(i,j,k)]/
                           MeanColumnWidth[Get2DIndex(i,j)];

   return SouthernComponent - NorthernComponent;
}

double TTriDimensional::UContributionToContinuity(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   double WesternComponent, EasternComponent;
   WesternComponent = 0.0; EasternComponent = 0.0;
   //WesternComponent
   if ((j > 0) && (j < GridColumns) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2))
        WesternComponent = (UVelocityPast[Get3DIndexForUVelocity(i,j,k)] *
                           (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j - 1,k)]) /
                           (2.0 * MeanColumnLength[Get2DIndex(i,j)]));
   else
   if ((j == 0) || ((j > 0) && (j < GridColumns) && (!BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)))
        WesternComponent = UVelocityPast[Get3DIndexForUVelocity(i,j,k)] *
                           BoxDepth[Get3DIndex(i,j,k)]/
                           MeanColumnLength[Get2DIndex(i,j)];

   //EasternComponent
   if ((j >= 0) && (j < GridColumns - 1) &&  (BoxArray[Get3DIndex(i,j + 1,k)].Dummybool2))
        EasternComponent = (UVelocityPast[Get3DIndexForUVelocity(i,j + 1,k)] *
                           (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j + 1,k)]) /
                           (2.0 * MeanColumnLength[Get2DIndex(i,j)]));
   else
   if ((j == GridColumns - 1) || ((j >= 0) && (j < GridColumns) && (!BoxArray[Get3DIndex(i,j + 1,k)].Dummybool2)))
        EasternComponent = UVelocityPast[Get3DIndexForUVelocity(i,j + 1,k)] *
                           BoxDepth[Get3DIndex(i,j,k)] /
                           MeanColumnLength[Get2DIndex(i,j)];
   /*if (((i == 4) || (i == 5)) && (j == 11)) {
     Debugger(i); Debugger(j); Debugger(k);
     Debugger(WesternComponent - EasternComponent);
   }*/
   return WesternComponent - EasternComponent;
}

int TTriDimensional::Get2DIndex(int i,int j)
{
    return i * GridColumns + j;
}

int TTriDimensional::Get2DIndex(int i,int j, int Columns)
{
    return i * Columns + j;
}

int TTriDimensional::Get3DIndex(int i,int j,int k)
{
    return (i * GridColumns + j) + k * GridLines * GridColumns;
}

int TTriDimensional::Get3DIndexForUVelocity(int i,int j, int k)
{
    return (i * (GridColumns + 1) + j) + k * GridLines * (GridColumns + 1);
}

int TTriDimensional::Get3DIndexForVVelocity(int i,int j, int k)
{
    return (i * GridColumns + j) + k * (GridLines + 1)* GridColumns;
}

void TTriDimensional::PartialSumInitialization()
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
      	   //aColumns[Index] = 0.0; bColumns[Index] = 0.0; cColumns[Index] = 0.0; rColumns[Index] = 0.0;
           //aLines[Index] = 0.0; bLines[Index] = 0.0; cLines[Index] = 0.0; rLines[Index] = 0.0;
         }
      }
   }
}

double TTriDimensional::SumOfAjacentDepthsInU(int ALine,int AColumn,int ASecondColumn,int ALayer)
{
    return (BoxDepth[Get3DIndex(ALine,AColumn,ALayer)] +
          BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)]);
}

double TTriDimensional::SumOfAjacentDepthsInV(int ALine,int ASecondLine,int AColumn,int ALayer)
{
    return (BoxDepth[Get3DIndex(ALine,AColumn,ALayer)] +
          BoxDepth[Get3DIndex(ASecondLine,AColumn,ALayer)]);
}


double TTriDimensional::UBoundaryFlow(int ALine,int AColumn,int ASecondColumn,int ALayer)
{
   //Uso aqui 2X a velocidade actual no pressuposto de que a velocidade do afluente
   //permance constante durante o intervalo de tempo.
   //Este produto pela soma
   //da profundidade da caixa consigo própria é para manter a coerência com a divisão por 4
   //efectuada na equação da continuidade
   //A Soma3 é um dos termos da equação da continuidade quando se trata de uma célula normal
   //e é a velocidade centrada no tempo na fronteira montante quando se trate de uma célula com fronteira
   //sólida a montante
   return UVelocity[Get3DIndexForUVelocity(ALine,AColumn,ALayer)] * /*BoxWidth[Get3DIndex(ALine,AColumn,ALayer)] *     */
            		       (BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)] /*+ BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)]*/) +
                          UVelocity[Get3DIndexForUVelocity(ALine,AColumn,ALayer)] * /*BoxWidth[Get3DIndex(ALine,AColumn,ALayer)] *   */
            		       (BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)] /*+ BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)]*/);

}

double TTriDimensional::UFlow(int ALine,int AColumn,int ASecondColumn,int ALayer)
{
    return UVelocityPast[Get3DIndexForUVelocity(ALine,AColumn,ALayer)] *
                                                (
            	                                    //BoxWidth[Get3DIndex(ALine,AColumn,ALayer)] *
                                                   (
               	                                    BoxDepth[Get3DIndex(ALine,AColumn,ALayer)] + BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)]
                                                    )
                                                 );
}

double TTriDimensional::VBoundaryFlow(int ALine, int ASecondLine, int AColumn,int ALayer)
{
   //Uso aqui 2X a velocidade actual no pressuposto de que a velocidade do afluente
   //permance constante durante o intervalo de tempo. Aplico o mesmo princípio ao
   //efluente nas linhas seguintes.
   //Este produto pela soma
   //da profundidade da caixa consigo própria é para manter a coerência com a divisão por 4
   //efectuada na equação da continuidade
   //A Soma3 é um dos termos da equação da continuidade quando se trata de uma célula normal
   //e é a velocidade centrada no tempo na fronteira montante quando se trate de uma célula com fronteira
   //sólida a montante
   return VVelocity[Get3DIndexForVVelocity(ALine,AColumn,ALayer)] * /*BoxWidth[Get3DIndex(ALine,AColumn,ALayer)] *     */
            		       (BoxDepth[Get3DIndex(ASecondLine,AColumn,ALayer)] /*+ BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)]*/) +
                          VVelocity[Get3DIndexForVVelocity(ALine,AColumn,ALayer)] * /*BoxWidth[Get3DIndex(ALine,AColumn,ALayer)] *   */
            		       (BoxDepth[Get3DIndex(ASecondLine,AColumn,ALayer)] /*+ BoxDepth[Get3DIndex(ALine,ASecondColumn,ALayer)]*/);

}

double TTriDimensional::VFlow(int ALine, int ASecondLine, int AColumn,int ALayer)
{
    return VVelocityPast[Get3DIndexForVVelocity(ALine,AColumn,ALayer)] *
                                                (
            	                                    //BoxWidth[Get3DIndex(ALine,AColumn,ALayer)] *
                                                   (
               	                                    BoxDepth[Get3DIndex(ALine,AColumn,ALayer)] + BoxDepth[Get3DIndex(ASecondLine,AColumn,ALayer)]
                                                    )
                                                 );
}

double TTriDimensional::WEBaroclinicPressureTerm(int ALine, int AColumn, int ASecondColumn, int ALayer)
{
   double PressureGradienteForce = 0.0;
   int Index1, Index2;
   for (int k = UpperLayer; k >= ALayer; k--)
   {
      Index1 = Get3DIndex(ALine, AColumn, k);
      Index2 = Get3DIndex(ALine, ASecondColumn, k);
      if (k > ALayer)
        PressureGradienteForce = PressureGradienteForce + Gravity * ATimeStep *
         	                      (WaterDensity[Index1] - WaterDensity[Index2])*
            	                   0.5 * (BoxDepth[Index1] + BoxDepth[Index2]) /
               	                (0.5 * (BoxLength[Index1] + BoxLength[Index2]));

      else if (k == ALayer)
         PressureGradienteForce = PressureGradienteForce + Gravity * ATimeStep *
         	                      (WaterDensity[Index1] - WaterDensity[Index2])*
            	                   0.5 * 0.5 * (BoxDepth[Index1] + BoxDepth[Index2]) /
               	                (0.5 * (BoxLength[Index1] + BoxLength[Index2]));
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
                     WaterDensity[Index1] + WaterDensity[Index2]
                   )
         );
}

double TTriDimensional::NSBaroclinicPressureTerm(int ALine, int ASecondLine, int AColumn, int ALayer)
{
   double PressureGradienteForce = 0.0;
   int Index1, Index2;
   for (int k = UpperLayer; k >= ALayer; k--)
   {
      Index1 = Get3DIndex(ALine, AColumn, k);
      Index2 = Get3DIndex(ASecondLine, AColumn, k);
      if (k > ALayer)
      {
        PressureGradienteForce = PressureGradienteForce + Gravity * ATimeStep *
         	                      (WaterDensity[Index1] - WaterDensity[Index2])*
            	                   0.5 * (BoxDepth[Index1] + BoxDepth[Index2]) /
               	                (0.5 * (BoxWidth[Index1] + BoxWidth[Index2]));
         /*if (((ALine == 4) || (ALine == 5)) && (AColumn == 11))
         {
            Debugger(1111);
            Debugger(WaterDensity[Index1]); Debugger(WaterDensity[Index2]);
         } */
      }
      else if (k == ALayer)
         PressureGradienteForce = PressureGradienteForce + Gravity * ATimeStep *
         	                      (WaterDensity[Index1] - WaterDensity[Index2])*
            	                   0.5 * 0.5 * (BoxDepth[Index1] + BoxDepth[Index2]) /
               	                (0.5 * (BoxWidth[Index1] + BoxWidth[Index2]));
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
                     WaterDensity[Index1] + WaterDensity[Index2]
                   )
         );
}

double TTriDimensional::WEBarotropicPressureTerm(int ALine, int AColumn, int ASecondColumn, int ALayer)
{
   int Index1, Index2;
   double DeltaX;
   Index1 = Get3DIndex(ALine, AColumn, ALayer);
   Index2 = Get3DIndex(ALine, ASecondColumn, ALayer);
   DeltaX = (BoxLength[Index2] + BoxLength[Index1]) / 2.0;
 
   return (Gravity *
          (DynamicHeight[Get3DIndex(ALine, AColumn, UpperLayer)] - DynamicHeight[Get3DIndex(ALine, ASecondColumn, UpperLayer)])*
          ATimeStep / DeltaX);
          
}

double TTriDimensional::NSBarotropicPressureTerm(int ALine, int ASecondLine, int AColumn, int ALayer)
{
   int Index1, Index2;
   double DeltaY;
   Index1 = Get3DIndex(ALine, AColumn, ALayer);
   Index2 = Get3DIndex(ASecondLine, AColumn, ALayer);
   DeltaY = (BoxWidth[Index2] + BoxWidth[Index1]) / 2.0;
   /*if ((ALine == 5) && (AColumn == 11) && (ALayer == 0))
   {
       Debugger(DeltaY);
       Debugger (DynamicHeight[Get3DIndex(ALine, AColumn, UpperLayer)]*100000);
       Debugger (DynamicHeight[Get3DIndex(ASecondLine, AColumn, UpperLayer)]*100000);
   } */
   return Gravity *
          (DynamicHeight[Get3DIndex(ALine, AColumn, UpperLayer)] - DynamicHeight[Get3DIndex(ASecondLine, AColumn, UpperLayer)])*
          ATimeStep / DeltaY;
}


double TTriDimensional::UConvectiveTerm(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   double UU = 0.0, UVU = 0.0, UW = 0.0;
   //Termo UU
   UU = UAdvectionOfUEquation(i,j,k);
   //Termo UV
   UVU = UVAdvectionOfUEquation(i,j,k);
   //Termo UW
   UW = UWAdvectionOfUEquation(i,j,k);
   /*if (((i == 4) || (i == 5)) && (j == 12) && (k == 0))
   {
      Debugger(i);
      Debugger(UU); Debugger(UVU); Debugger(UW);
      //Debugger(UU + UVU + UW);
   } */
   return UU + UVU + UW; //ms-1
}

double TTriDimensional::UAdvectionOfUEquation(int ALine, int AColumn, int ALayer)
{
    int i,j,k;
    i = ALine; j = AColumn; k = ALayer;
    if (((j > 0) && (BoxDepth[Get3DIndex(i,j-1,k)] <= MinDepthForAdvection)) || ((j < GridColumns) && (BoxDepth[Get3DIndex(i,j,k)] <= MinDepthForAdvection)))
	    return 0.0;
    else
    {
      if (
           (j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (j < GridColumns)
         )
      {
         UU1[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j-1,k)] *   //Termo Uij-1
              BoxDepth[Get3DIndex(i,j-1,k)] *
              (UPStreamUVelocity(i,j,k) + fabs(UPStreamUVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                 (0.5 * (BoxLength[Get3DIndex(i,j-1,k)] + BoxLength[Get3DIndex(i,j,k)]))
              );

         UU2[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j,k)] *     //Termo Uij
              BoxDepth[Get3DIndex(i,j,k)] *
              (DOWNStreamUVelocity(i,j,k) + fabs(DOWNStreamUVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)])  *
                 (0.5 * (BoxLength[Get3DIndex(i,j-1,k)] + BoxLength[Get3DIndex(i,j,k)]))
              );

         UU3[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j,k)] *    //Termo Uij
              BoxDepth[Get3DIndex(i,j-1,k)] *
              (UPStreamUVelocity(i,j,k) - fabs(UPStreamUVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)])  *
                 (0.5 * (BoxLength[Get3DIndex(i,j-1,k)] + BoxLength[Get3DIndex(i,j,k)]))
              );
      }
      else
      if (
           (j == 0) || (!BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (j < GridColumns)
         )
      {
         UU1[Get3DIndexForUVelocity(i,j,k)] = 0.0;
         UU2[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j,k)] *     //Termo Uij
              BoxDepth[Get3DIndex(i,j,k)] *
              (DOWNStreamUVelocity(i,j,k) + fabs(DOWNStreamUVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j,k)])  *
                 BoxLength[Get3DIndex(i,j,k)]
              );
         UU3[Get3DIndexForUVelocity(i,j,k)] = 0.0;
      }
      else
      if (
           (j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (j == GridColumns)
         )
      {
         UU1[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j-1,k)] *   //Termo Uij-1
              BoxDepth[Get3DIndex(i,j-1,k)] *
              (UPStreamUVelocity(i,j,k) + fabs(UPStreamUVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j-1,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                 BoxLength[Get3DIndex(i,j-1,k)]
              );

         UU2[Get3DIndexForUVelocity(i,j,k)] = 0.0;

         UU3[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j,k)] *    //Termo Uij
              BoxDepth[Get3DIndex(i,j-1,k)] *
              (UPStreamUVelocity(i,j,k) - fabs(UPStreamUVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j-1,k)] + BoxDepth[Get3DIndex(i,j-1,k)])  *
                 BoxLength[Get3DIndex(i,j-1,k)]
              );
      }
      if  (j < GridColumns)
      {
         if (j > 0)
            UU4[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j+1,k)] *  //Termo Uij+1
                 BoxDepth[Get3DIndex(i,j,k)] *
                 (DOWNStreamUVelocity(i,j,k) - fabs(DOWNStreamUVelocity(i,j,k)))/
                 (
                    (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]) *
                    (0.5 * (BoxLength[Get3DIndex(i,j-1,k)] + BoxLength[Get3DIndex(i,j,k)]))
                 );
         else
            UU4[Get3DIndexForUVelocity(i,j,k)] = ATimeStep * UVelocityPast[Get3DIndexForUVelocity(i,j+1,k)] *  //Termo Uij+1
                 BoxDepth[Get3DIndex(i,j,k)] *
                 (DOWNStreamUVelocity(i,j,k) - fabs(DOWNStreamUVelocity(i,j,k)))/
                 (
                    (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j,k)]) *
                    BoxLength[Get3DIndex(i,j,k)]
                 );

     }
     else
        UU4[Get3DIndexForUVelocity(i,j,k)] = 0.0;
      
     return (UU2[Get3DIndexForUVelocity(i,j,k)] - UU1[Get3DIndexForUVelocity(i,j,k)] +
             UU4[Get3DIndexForUVelocity(i,j,k)] - UU3[Get3DIndexForUVelocity(i,j,k)]);
   }
}



double TTriDimensional::UVAdvectionOfUEquation(int ALine, int AColumn, int ALayer)
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;
    if (
          ((j > 0) && (BoxDepth[Get3DIndex(i,j-1,k)] <= MinDepthForAdvection))
          || ((j < GridColumns) && (BoxDepth[Get3DIndex(i,j,k)] <= MinDepthForAdvection))
          || (j == 0) || (j > GridColumns - 1)
       )
    return 0.0;
    else
    {
      double VelocityComponent;
      VelocityComponent = 0.0;
      double MyUpperLeftCornerVVelocity, MyLowerLeftCornerVVelocity;
      MyUpperLeftCornerVVelocity = (VVelocityPast[Get3DIndexForVVelocity(i+1,j,k)] + VVelocityPast[Get3DIndexForVVelocity(i+1,j-1,k)]) / 2.0;
      MyLowerLeftCornerVVelocity = (VVelocityPast[Get3DIndexForVVelocity(i,j,k)] + VVelocityPast[Get3DIndexForVVelocity(i,j-1,k)]) / 2.0;
      VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j,k) *
                          (MyUpperLeftCornerVVelocity + fabs(MyUpperLeftCornerVVelocity)) *
                          UVelocityPast[Get3DIndexForUVelocity(i,j,k)]);
      if (i > 0)
         VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j,k) *
                            (MyLowerLeftCornerVVelocity + fabs(MyLowerLeftCornerVVelocity)) *
                            UVelocityPast[Get3DIndexForUVelocity(i-1,j,k)]);
      if (i < GridLines - 1)
         VelocityComponent = VelocityComponent + (UpperLeftCornerDepth(i,j,k) *
                            (MyUpperLeftCornerVVelocity - fabs(MyUpperLeftCornerVVelocity)) *
                            UVelocityPast[Get3DIndexForUVelocity(i+1,j,k)]);
      VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j,k) *
                            (MyLowerLeftCornerVVelocity - fabs(MyLowerLeftCornerVVelocity)) *
                            UVelocityPast[Get3DIndexForUVelocity(i,j,k)]);
      double MeanWidth;
      /*if (i > 0)
         MeanWidth = 0.5 * (BoxWidth[Get3DIndex(i,j,k)] +  BoxWidth[Get3DIndex(i - 1,j,k)]);
      else */
         MeanWidth = BoxWidth[Get3DIndex(i,j,k)];
      UVU[Get3DIndexForUVelocity(i,j,k)] = ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] +  BoxDepth[Get3DIndex(i,j-1,k)]) *
            VelocityComponent / MeanWidth;

      return (UVU[Get3DIndexForUVelocity(i,j,k)]);
    }
}

double TTriDimensional::UWAdvectionOfUEquation(int ALine, int AColumn, int ALayer)
{
    int i,j,k;
    i = ALine; j = AColumn; k = ALayer;
    if (
         (j > 0) && (BoxDepth[Get3DIndex(i,j-1,k)] > MinDepthForAdvection) &&
         (j < GridColumns) && (BoxDepth[Get3DIndex(i,j,k)] > MinDepthForAdvection)
          && (k > 0)
      )
   {
      double MeanW1, MeanW2, UUpstream, UDownstream, Parcels1;
      MeanW1 = 0; MeanW2 = 0.0; Parcels1 = 0.0;

      if (k < UpperLayer)
      {
         if (BoxArray[Get3DIndex(i,j-1,k+1)].Dummybool2){
            MeanW1 = MeanW1 + WVelocityPast[Get3DIndex(i,j-1,k+1)]; //i j+1/2 k+1
            Parcels1++;
         }
         if (BoxArray[Get3DIndex(i,j-1,k+1)].Dummybool2){
            MeanW1 = MeanW1 + WVelocityPast[Get3DIndex(i,j,k+1)];
            Parcels1++;
         }
         if (Parcels1 > 0.0)
            MeanW1 = MeanW1 / Parcels1;
      }
      //if ((BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
      //A verificação acima é redundante, pois no início foi verificada a condição da produnfidade minima
      //para cálculo da advecção que é maior do que a profuncidade crítica para considerar que uma célula tem água
      MeanW2 = (WVelocityPast[Get3DIndex(i,j-1,k)] + WVelocityPast[Get3DIndex(i,j,k)]) / 2.0; //i j-1/2 k

      if (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)
         UUpstream = UVelocityPast[Get3DIndexForUVelocity(i,j,k-1)];
      else
         UUpstream = 0.0;

      if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2))
      	 UDownstream = UVelocityPast[Get3DIndexForUVelocity(i,j,k+1)];
      else
      	 UDownstream = 0.0;

      UW[Get3DIndexForUVelocity(i,j,k)] = ATimeStep *
              (
                (MeanW1 + fabs(MeanW1)) *
                UVelocityPast[Get3DIndexForUVelocity(i,j,k)] -
                (MeanW2 + fabs(MeanW2)) *
                UUpstream +
                (MeanW1 - fabs(MeanW1)) *
                UDownstream -
                (MeanW2 - fabs(MeanW2)) *
                UVelocityPast[Get3DIndexForUVelocity(i,j,k)]
              ) / (2.0 * (BoxDepth[Get3DIndex(i,j-1,k)] + BoxDepth[Get3DIndex(i,j,k)]));
   }
   else
   	  UW[Get3DIndexForUVelocity(i,j,k)] = 0.0;
      
   return (UW[Get3DIndexForUVelocity(i,j,k)]);
}

double TTriDimensional::VConvectiveTerm(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   double VV = 0.0, UVV = 0.0, VW = 0.0;
   //Termo VV
   VV = VAdvectionOfVEquation(i,j,k);
   //Termo UV
   UVV = UVAdvectionOfVEquation(i,j,k);
   //Termo VW
   VW = VWAdvectionOfVEquation(i,j,k);

   return VV + UVV + VW;  //ms-1
}

double TTriDimensional::VAdvectionOfVEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   if (((i > 0) && (BoxDepth[Get3DIndex(i-1,j,k)] <= MinDepthForAdvection)) || ((i < GridLines) && (BoxDepth[Get3DIndex(i,j,k)] <= MinDepthForAdvection)))
	    return 0.0;
   else
   {
     if (
           (i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (i < GridLines)
        )
     {
          VV1[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i-1,j,k)] *           //Termo Vi-1j
              BoxDepth[Get3DIndex(i-1,j,k)] *
              (UPStreamVVelocity(i,j,k) + fabs(UPStreamVVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                 (0.5 * (BoxWidth[Get3DIndex(i-1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]))
              );

          VV2[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i,j,k)] *            //Termo Vij
              BoxDepth[Get3DIndex(i,j,k)] *
              (DOWNStreamVVelocity(i,j,k) + fabs(DOWNStreamVVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])  *
                 (0.5 * (BoxWidth[Get3DIndex(i-1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]))
              );

          VV3[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i,j,k)] *            //Termo Vij
              BoxDepth[Get3DIndex(i-1,j,k)] *
              (UPStreamVVelocity(i,j,k) - fabs(UPStreamVVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])  *
                 (0.5 * (BoxWidth[Get3DIndex(i-1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]))
              );

      }
      else
      if (
           (i == 0) || (!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (i < GridLines)
        )
      {
          VV1[Get3DIndexForVVelocity(i,j,k)] = 0.0;   //Termo Vi-1j
          VV2[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i,j,k)] *            //Termo Vij
              BoxDepth[Get3DIndex(i,j,k)] *
              (DOWNStreamVVelocity(i,j,k) + fabs(DOWNStreamVVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j,k)])  *
                 BoxWidth[Get3DIndex(i,j,k)]
              );
          VV3[Get3DIndexForVVelocity(i,j,k)] = 0.0;  //Termo Vij

     }
     else
     if (
           (i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (i == GridLines)
        )
     {
          /*if ((j == 3) && (i == GridLines) && (k == 5)) Debugger(111);
          if ((j == 3) && (i == GridLines) && (k == 5)) {
             Debugger(VVelocityPast[Get3DIndexForVVelocity(i-1,j,k)]);
             Debugger(BoxDepth[Get3DIndex(i-1,j,k)]);
             Debugger(BoxWidth[Get3DIndex(i-1,j,k)]);
             Debugger(UPStreamVVelocity(i,j,k));
          } */
          VV1[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i-1,j,k)] *           //Termo Vi-1j
              BoxDepth[Get3DIndex(i-1,j,k)] *
              (UPStreamVVelocity(i,j,k) + fabs(UPStreamVVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i-1,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                 BoxWidth[Get3DIndex(i-1,j,k)]
              );
          VV2[Get3DIndexForVVelocity(i,j,k)] = 0.0;
          //if ((j == 3) && (i == GridLines) && (k == 5)) Debugger(222);
          VV3[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i,j,k)] *            //Termo Vij
              BoxDepth[Get3DIndex(i-1,j,k)] *
              (UPStreamVVelocity(i,j,k) - fabs(UPStreamVVelocity(i,j,k))) /
              (
                 (BoxDepth[Get3DIndex(i-1,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)])  *
                 BoxWidth[Get3DIndex(i-1,j,k)]
              );
          //if ((j == 3) && (i == GridLines) && (k == 5)) Debugger(333);    
     }
     if (i < GridLines)
     {
         if (i > 0)
            VV4[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i+1,j,k)] *         //Termo Vi+1j
              BoxDepth[Get3DIndex(i,j,k)] *
              (DOWNStreamVVelocity(i,j,k) - fabs(DOWNStreamVVelocity(i,j,k)))/
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                 (0.5 * (BoxWidth[Get3DIndex(i-1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]))
              );
         else
            VV4[Get3DIndexForVVelocity(i,j,k)] = ATimeStep * VVelocityPast[Get3DIndexForVVelocity(i+1,j,k)] *         //Termo Vi+1j
              BoxDepth[Get3DIndex(i,j,k)] *
              (DOWNStreamVVelocity(i,j,k) - fabs(DOWNStreamVVelocity(i,j,k)))/
              (
                 (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j,k)]) *
                 BoxWidth[Get3DIndex(i,j,k)]
              );
     }
     else
        VV4[Get3DIndexForVVelocity(i,j,k)] = 0.0;  //Termo Vi+1j

     return (VV2[Get3DIndexForVVelocity(i,j,k)] - VV1[Get3DIndexForVVelocity(i,j,k)] +
             VV4[Get3DIndexForVVelocity(i,j,k)] - VV3[Get3DIndexForVVelocity(i,j,k)]);
   }
}


double TTriDimensional::UVAdvectionOfVEquation(int ALine, int AColumn, int ALayer)
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;
    if (
          ((i > 0) && (BoxDepth[Get3DIndex(i-1,j,k)] <= MinDepthForAdvection))
          || ((i < GridLines) && (BoxDepth[Get3DIndex(i,j,k)] <= MinDepthForAdvection))
          || (i == 0) || (i > GridLines - 1)
       )
    return 0.0;
    else
    {
      double VelocityComponent, MyLowerRightCornerUVelocity, MyLowerLeftCornerUVelocity;
      VelocityComponent = 0.0;


      MyLowerRightCornerUVelocity = (UVelocityPast[Get3DIndexForUVelocity(i,j+1,k)] + UVelocityPast[Get3DIndexForUVelocity(i-1,j+1,k)]) / 2.0;
      MyLowerLeftCornerUVelocity =  (UVelocityPast[Get3DIndexForUVelocity(i,j,k)] + UVelocityPast[Get3DIndexForUVelocity(i-1,j,k)]) / 2.0;

      VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j,k) *
                         (MyLowerRightCornerUVelocity + fabs(MyLowerRightCornerUVelocity)) *
                         VVelocityPast[Get3DIndexForVVelocity(i,j,k)]);

      if (j > 0)
          VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j,k) *
                            (MyLowerLeftCornerUVelocity + fabs(MyLowerLeftCornerUVelocity)) *
                            VVelocityPast[Get3DIndexForVVelocity(i,j-1,k)]);

      if (j < GridColumns - 1)
          VelocityComponent = VelocityComponent + (LowerRightCornerDepth(i,j,k) *
                                  (MyLowerRightCornerUVelocity - fabs(MyLowerRightCornerUVelocity)) *
                                  VVelocityPast[Get3DIndexForVVelocity(i,j+1,k)]);

      VelocityComponent = VelocityComponent - (LowerLeftCornerDepth(i,j,k) *
                            (MyLowerLeftCornerUVelocity - fabs(MyLowerLeftCornerUVelocity)) *
                            VVelocityPast[Get3DIndexForVVelocity(i,j,k)]);

      double MeanLength;
      /*if (j > 0)
        MeanLength = 0.5 * (BoxLength[Get3DIndex(i,j,k)] + BoxLength[Get3DIndex(i,j - 1,k)]);
      else*/
        MeanLength = BoxLength[Get3DIndex(i,j,k)];

      UVV[Get3DIndexForVVelocity(i,j,k)] =  ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
            VelocityComponent / MeanLength;
      return UVV[Get3DIndexForVVelocity(i,j,k)];
   }
}



double TTriDimensional::VWAdvectionOfVEquation(int ALine, int AColumn, int ALayer)
{
  int i, j, k;
  i = ALine; j = AColumn; k = ALayer;
  if (
       (i > 0) && (BoxDepth[Get3DIndex(i-1,j,k)] > MinDepthForAdvection) &&
       (i < GridLines) && (BoxDepth[Get3DIndex(i,j,k)] > MinDepthForAdvection) &&
       (k > 0)
     )
  {
      double MeanW1, MeanW2, VUpstream, VDownstream, Parcels1;
      MeanW1 = 0.0; Parcels1 = 0.0; MeanW2 = 0.0;

      if (k < UpperLayer)
      {
         if (BoxArray[Get3DIndex(i-1,j,k+1)].Dummybool2){
            MeanW1 = MeanW1 + WVelocityPast[Get3DIndex(i-1,j,k+1)];
            Parcels1++;
         }
         if (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2){
            MeanW1 = MeanW1 + WVelocityPast[Get3DIndex(i,j,k+1)];
            Parcels1++;
         }
         if (Parcels1 > 0.0)
            MeanW1 = MeanW1 / Parcels1;
      }

      //if ((BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) && (BoxArray[Get3DIndex(i,j,k)].Dummybool2))
      //A verificação acima é redundante, pois no início foi verificada a condição da produnfidade minima
      //para cálculo da advecção que é maior do que a profuncidade crítica para considerar que uma célula tem água
      MeanW2 = (WVelocityPast[Get3DIndex(i-1,j,k)] + WVelocityPast[Get3DIndex(i,j,k)]) / 2.0; //i-1/2j,k
      
      if (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)
         VUpstream = VVelocityPast[Get3DIndexForVVelocity(i,j,k-1)];
      else
         VUpstream = 0.0;

      if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2))
      	VDownstream = VVelocityPast[Get3DIndexForVVelocity(i,j,k+1)];
      else
      	VDownstream = 0.0;

      VW[Get3DIndexForVVelocity(i,j,k)] = ATimeStep *
             	(
                  (MeanW1 + fabs(MeanW1)) *
                  VVelocityPast[Get3DIndexForVVelocity(i,j,k)] -
                  (MeanW2 + fabs(MeanW2)) *
                  VUpstream +
                  (MeanW1 - fabs(MeanW1)) *
                  VDownstream -
                  (MeanW2 - fabs(MeanW2)) *
                  VVelocityPast[Get3DIndexForVVelocity(i,j,k)]
             	) / (2.0 * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i - 1,j,k)]));
  }
  else VW[Get3DIndexForVVelocity(i,j,k)] = 0.0;

  return VW[Get3DIndexForVVelocity(i,j,k)];
}

double TTriDimensional::UPStreamUVelocity(int ALine, int AColumn, int ALayer) //Calcula o Ui,j-1/2, k
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;
	if ((i < 0) || (i >/*=*/ GridLines) || (j < 0) || (j > GridColumns) || (k < 0) || (k > UpperLayer))
    {
#ifdef __BORLANDC__
   	    MessageBox(0,"Erro in TTriDimensional::UPStreamUVelocity","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TTriDimensional::UPStreamUVelocity - erro" << endl;
#endif  // __BORLANDC__
        //Debugger(i); Debugger(j); Debugger(k);
        //Debugger(GridLines); Debugger(GridColumns); Debugger(UpperLayer);
    }
    else
    {
        if (
            (
              //(j > 0) && ((!BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2) || (BoxArray[Get3DIndex(i,j-1,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2))
              (j > 0) && (!BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)
            ) ||
            (
               (j < GridColumns) &&
               (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 4) || (j == 0)
            )
           ) //Ligação com terra a montante
               return UVelocityPast[Get3DIndexForUVelocity(i,j,k)];
        else
               return (UVelocityPast[Get3DIndexForUVelocity(i,j-1,k)] + UVelocityPast[Get3DIndexForUVelocity(i,j,k)]) / 2.0;
    }
}


double TTriDimensional::DOWNStreamUVelocity(int ALine, int AColumn, int ALayer)  //Calcula o Ui,j+1/2, k
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;
	if ((i < 0) || (i >/*=*/ GridLines) || (j < 0) || (j > GridColumns) || (k > UpperLayer))
    {
#ifdef __BORLANDC__
   	   MessageBox(0,"Erro in TTriDimensional::DOWNStreamUVelocity","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
       cerr << "TTriDimensional::DOWNStreamUVelocity - erro" << endl;
#endif  // __BORLANDC__
      //Debugger(i); Debugger(j);
    }
    else
    {
       //if ((j == GridColumns) || ((j < GridColumns) && (BoxArray[Get3DIndex(i,j + 1,k)].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2)))
       if (j == GridColumns)
   		  return UVelocityPast[Get3DIndexForUVelocity(i,j,k)];
   	   else
   		  return (
         			UVelocityPast[Get3DIndexForUVelocity(i,j,k)] +
                    UVelocityPast[Get3DIndexForUVelocity(i,j+1,k)]
                 ) / 2.0;
    }
}


double TTriDimensional::UPStreamVVelocity(int ALine, int AColumn, int ALayer)  //Calcula o Vi-1/2,j,k         ok
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i < 0) || (i >/*=*/ GridLines) || (j < 0) || (j >/*=*/ GridColumns) || (k > UpperLayer))
   {
#ifdef __BORLANDC__
      MessageBox(0,"Error in TTriDimensional::UPStreamVVelocity","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
      cerr << "TTriDimensional::UPStreamVvelocity - erro" << endl;
#endif  // __BORLANDC__
      Debugger(GridLines); Debugger(GridColumns); Debugger(UpperLayer);
      Debugger(i); Debugger(j); Debugger(k);
   }
   else
   {
      if(
          (
            (i > 0) && //Implica que a caixa não constituí limite da malha a sul
            (!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)
            //((!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) || (BoxArray[Get3DIndex(i-1,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2))//Fronteira com terra
          ) ||
          (
            (i < GridLines) &&
            (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 4) || (i == 0)
          )
        )
           return VVelocityPast[Get3DIndexForVVelocity(i,j,k)];
      else
           return (
                     VVelocityPast[Get3DIndexForVVelocity(i-1,j,k)] +
                     VVelocityPast[Get3DIndexForVVelocity(i,j,k)]
                   ) / 2.0;
   }
}


double TTriDimensional::DOWNStreamVVelocity(int ALine, int AColumn, int ALayer)  //Calcula o Vi+1/2,j            ok
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i < 0) || (i >/*=*/ GridLines) || (j < 0) || (j >/*=*/ GridColumns) || (k > UpperLayer))
#ifdef __BORLANDC__
   	   MessageBox(0,"Erro in TTriDimensional::DownStreamVVelocity","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
       cerr << "TTriDimensional::DOWNStreamVVelocity - erro" << endl;
#endif  // __BORLANDC__
   else
   {
       if (i == GridLines )
          return VVelocityPast[Get3DIndexForVVelocity(i,j,k)];
       else
          return (
                      VVelocityPast[Get3DIndexForVVelocity(i,j,k)] +
                      VVelocityPast[Get3DIndexForVVelocity(i+1,j,k)]
                    ) / 2.0;
   }
}


double TTriDimensional::UpperLeftCornerDepth(int ALine, int AColumn, int ALayer)      //ok
{
   double SumOfSurroundingDepths, NumberOfParcels;
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
         	 (
            	(Line >= 0) && (Line < GridLines)
             )
             &&
             (
            	(Column >= 0) && (Column < GridColumns)
             )
             &&
             (
               (BoxDepth[Get3DIndex(Line,Column,k)] >= CriticalDepthForLandBoundary)
               //(BoxArray[Line * GridColumns + Column].Dummybool2)
             )
            )
            {
            	SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Get3DIndex(Line,Column,k)];
               NumberOfParcels++;
            }
      }
   }
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TTriDimensional::UpperRightCornerDepth(int ALine, int AColumn, int ALayer)        //ok
{
   double SumOfSurroundingDepths, NumberOfParcels;
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i; Line < i + 2; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Get3DIndex(Line,Column,k)] >= CriticalDepthForLandBoundary)
                  //(BoxArray[Line * GridColumns + Column].Dummybool2)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Get3DIndex(Line,Column,k)];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   //Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
}


double TTriDimensional::LowerLeftCornerDepth(int ALine, int AColumn, int ALayer)          //ok
{
   double SumOfSurroundingDepths, NumberOfParcels;
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
      for (int Column = j - 1; Column < j + 1; Column++)
      {
      	if (
             (
                (Line >= 0) && (Line < GridLines)
             )&&
             (
              (Column >= 0) && (Column < GridColumns)
             ) &&
             (
                (BoxDepth[Get3DIndex(Line,Column,k)] >= CriticalDepthForLandBoundary)
             )
           )
          {
             SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Get3DIndex(Line,Column,k)];
             NumberOfParcels++;
          }
      }
   }
   if (NumberOfParcels == 0.0)
      return 0.0;
   else
      return SumOfSurroundingDepths / NumberOfParcels;
}


double TTriDimensional::LowerRightCornerDepth(int ALine, int AColumn, int ALayer)     //ok
{
   double SumOfSurroundingDepths, NumberOfParcels;
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   SumOfSurroundingDepths = 0;
   NumberOfParcels = 0;

   for (int Line = i - 1; Line < i + 1; Line++)
   {
   	for (int Column = j; Column < j + 2; Column++)
      {
      	if (
         		(
         			(Line >= 0) && (Line < GridLines)
               )
               	&&
               (
               	(Column >= 0) && (Column < GridColumns)
               )
               	&&
               (
                  (BoxDepth[Get3DIndex(Line,Column,k)] >= CriticalDepthForLandBoundary)
                  //(BoxArray[Line * GridColumns + Column].Dummybool2)
               )
            )
            {
          		SumOfSurroundingDepths = SumOfSurroundingDepths + BoxDepth[Get3DIndex(Line,Column,k)];
               NumberOfParcels++;
            }
      }
   }
   if (NumberOfParcels == 0.0)
   	return 0.0;
   else
   {
   	//Debugger(SumOfSurroundingDepths / NumberOfParcels);
   	return SumOfSurroundingDepths / NumberOfParcels;
   }
}

double TTriDimensional::UWEHorizontalDiffusion(int ALine, int AColumn, int ALayer)
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;
    double WesternFlux, EasternFlux, MyLength;
    double const TINNY = 0.000000001;
    WesternFlux = 0.0; EasternFlux = 0.0; MyLength =0.0;
    //Western flux
    if  (
            (j > 0) &&
            (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2)
         )
         WesternFlux = HorizontalDiffCoef *
                       CorrectionOfHorizontalDiffCoef(BoxDepth[Get3DIndex(i,j-1,k)], UPStreamUVelocity(i,j,k))*
                       (
                           UVelocityPast[Get3DIndexForUVelocity(i,j,k)] -
                           UVelocityPast[Get3DIndexForUVelocity(i,j-1,k)]
                       ) / (BoxLength[Get3DIndex(i,j-1,k)]);
    else
      	WesternFlux = 0.0;

    //EasternFlux
    if  (j < GridColumns)
      	EasternFlux = HorizontalDiffCoef *
                      CorrectionOfHorizontalDiffCoef(BoxDepth[Get3DIndex(i,j,k)], DOWNStreamUVelocity(i,j,k))*
                     (
                          UVelocityPast[Get3DIndexForUVelocity(i,j + 1,k)] -
                          UVelocityPast[Get3DIndexForUVelocity(i,j,k)]
                      ) / (BoxLength[Get3DIndex(i,j,k)]);
    else
      	EasternFlux = 0.0;

    if ((j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2))
       MyLength = (0.5 * (BoxLength[Get3DIndex(i,j,k)] + BoxLength[Get3DIndex(i,j-1,k)]));
    else
       MyLength = BoxLength[Get3DIndex(i,j,k)];
    if (MyLength > fabs(TINNY))
       return  ATimeStep * (EasternFlux - WesternFlux) / MyLength;
    else return 0.0;
}

double TTriDimensional::UNSHorizontalDiffusion(int ALine, int AColumn, int ALayer)
{
  int i, j, k;
  i = ALine; j = AColumn; k = ALayer;
  double SouthernFlux, MeanWidth1, NorthernFlux, MeanWidth2;
  SouthernFlux = 0.0; NorthernFlux = 0.0;
   //Southern flux
   if (
         (i > 0) && (j > 0) &&
         (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) &&
      	 (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4)  &&
         (BoxArray[Get3DIndex(i-1,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxArray[Get3DIndex(i-1,j-1,k)].Dummybool2)
      )
   {
       MeanWidth1 = 0.5 * (BoxWidth[Get3DIndex(i-1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]);
       if (MeanWidth1 <= 0.0) 
#ifdef __BORLANDC__
            MessageBox(0, "TTriDimensional::UNSHorizontalDiffusion - zero MeanWidth1", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TTriDimensional::UNSHorizontalDiffusion - zero MeanWidth1" << endl;
#endif  // __BORLANDC__
       SouthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(LowerLeftCornerDepth(i,j,k), LowerUVelocity(i,j,k))*
                    (
                    	UVelocityPast[Get3DIndexForUVelocity(i,j,k)] -
                        UVelocityPast[Get3DIndexForUVelocity(i-1,j,k)]
                    ) / MeanWidth1;
   }
   else
   	SouthernFlux = 0.0;

   //Northern flux
   if (
         (i < GridLines -  1) &&  (j > 0) &&
         (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2) &&
         (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxArray[Get3DIndex(i+1,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
         (BoxArray[Get3DIndex(i+1,j-1,k)].Dummybool2)
       )
   {
       MeanWidth2 = 0.5 * (BoxWidth[Get3DIndex(i+1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]);
       if (MeanWidth2 <= 0.0) 
#ifdef __BORLANDC__
            MessageBox(0, "TTriDimensional::UNSHorizontalDiffusion - zero MeanWidth2", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TTriDimensional::UNSHorizontalDiffusion - zero MeanWidth2" << endl;
#endif  // __BORLANDC__
       NorthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(UpperLeftCornerDepth(i,j,k), UpperUVelocity(i,j,k))*
                    (
                        UVelocityPast[Get3DIndexForUVelocity(i+1,j,k)] -
                        UVelocityPast[Get3DIndexForUVelocity(i,j,k)]
                    ) / MeanWidth2;
   }
   else
   	NorthernFlux = 0.0;

   return ATimeStep * (NorthernFlux - SouthernFlux) /  BoxWidth[Get3DIndex(i,j,k)];
}

double TTriDimensional::UVerticalDiffusion(int TermOrder, int Line, int Column, int ALayer)
{
   int i, j, k;
   i = Line; j = Column; k = ALayer;
   double DepthForUpperDiffusion, DepthForLowerDiffusion, MyVerticalDiffusion, Parcels1, Parcels2;
   DepthForLowerDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels1 = 1.0;
   DepthForUpperDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels2 = 1.0;
   MyVerticalDiffusion = 0.0;

   if ((TermOrder == 1) || (TermOrder == 2)) //a or b
   {
      if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)) {
        DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j-1,k)]; Parcels1++;
      }
      if ((k > 0) && (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)){
        DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j,k-1)]; Parcels1++;
      }
      if ((j > 0) && (k > 0) && (BoxArray[Get3DIndex(i,j-1,k-1)].Dummybool2)){
        DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j-1,k-1)]; Parcels1++;
      }
      DepthForLowerDiffusion = DepthForLowerDiffusion / Parcels1;   

      if (
            (k > 0) &&
            (
                (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2) &&
                (
                   (j > 0) && (BoxArray[Get3DIndex(i,j - 1,k - 1)].Dummybool2)
                )
            )
         )
         MyVerticalDiffusion = MyVerticalDiffusion + VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] * ATimeStep /
                                                     pow(DepthForLowerDiffusion, 2.0);
   }
   if ((TermOrder == 3) || (TermOrder == 2)) //c or b
   {
      if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)) {
        DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j-1,k)]; Parcels2++;
      }
      if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)){
        DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j,k+1)]; Parcels2++;
      }
      if ((j > 0) && (k < UpperLayer) && (BoxArray[Get3DIndex(i,j-1,k+1)].Dummybool2)){
        DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j-1,k+1)]; Parcels2++;
      }
      DepthForUpperDiffusion = DepthForUpperDiffusion / Parcels2; 

      if (
            (k < UpperLayer) &&
            (
               (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2) &&
               (
                   (j > 0) && (BoxArray[Get3DIndex(i,j - 1,k + 1)].Dummybool2)
               )
            )
         )
         MyVerticalDiffusion = MyVerticalDiffusion + VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k+1)] * ATimeStep /
                                                     pow(DepthForUpperDiffusion, 2.0);
   }
   return MyVerticalDiffusion;
}

double TTriDimensional::UVerticalDiffusionExplicit(int Line, int Column, int ALayer)
{
    //if ((Line == 0) && (Column == 1) && (ALayer == 4)) Debugger(1111);
    int i, j, k;
    i = Line; j = Column; k = ALayer;
    double DepthForUpperDiffusion, DepthForLowerDiffusion, LowerDiffusion, UpperDiffusion, Parcels1, Parcels2;
    DepthForLowerDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels1 = 1.0;
    DepthForUpperDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels2 = 1.0;
    LowerDiffusion = 0.0; UpperDiffusion = 0.0;

    if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)) {
      DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j-1,k)]; Parcels1++;
    }
    if ((k > 0) && (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)){
      DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j,k-1)]; Parcels1++;
    }
    if ((j > 0) && (k > 0) && (BoxArray[Get3DIndex(i,j-1,k-1)].Dummybool2)){
      DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j-1,k-1)]; Parcels1++;
    }
    DepthForLowerDiffusion = DepthForLowerDiffusion / Parcels1;

    if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2)) {
      DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j-1,k)]; Parcels2++;
    }
    if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)){
      DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j,k+1)]; Parcels2++;
    }
    if ((j > 0) && (k < UpperLayer) && (BoxArray[Get3DIndex(i,j-1,k+1)].Dummybool2)){
      DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j-1,k+1)]; Parcels2++;
    }
    DepthForUpperDiffusion = DepthForUpperDiffusion / Parcels2;    

    if (
            (k > 0) &&
            (
                (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2) &&
                (
                   (j > 0) && (BoxArray[Get3DIndex(i,j - 1,k - 1)].Dummybool2)
                )
            )
         )
         LowerDiffusion = VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] * ATimeStep *
                          (UVelocityPast[Get3DIndexForUVelocity(i,j,k)] - UVelocityPast[Get3DIndexForUVelocity(i,j,k - 1)]) /
                           pow(DepthForLowerDiffusion, 2.0);
    if (
            (k < UpperLayer) &&
            (
               (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2) &&
               (
                   (j > 0) && (BoxArray[Get3DIndex(i,j - 1,k + 1)].Dummybool2)
               )
            )
         )
         UpperDiffusion = VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k+1)] * ATimeStep *
                         (UVelocityPast[Get3DIndexForUVelocity(i,j,k + 1)] - UVelocityPast[Get3DIndexForUVelocity(i,j,k)]) /
                          pow(DepthForUpperDiffusion, 2.0);
    //if ((Line == 0) && (Column == 1) && (ALayer == 4)) Debugger(2222);
    return UpperDiffusion - LowerDiffusion;
}

double TTriDimensional::VVerticalDiffusion(int TermOrder, int Line, int Column, int ALayer)
{
   int i, j, k;
   i = Line; j = Column; k = ALayer;
   double DepthForUpperDiffusion, DepthForLowerDiffusion, MyVerticalDiffusion, Parcels1, Parcels2;
   DepthForLowerDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels1 = 1.0;
   DepthForUpperDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels2 = 1.0;
   MyVerticalDiffusion = 0.0;

   if ((TermOrder == 1) || (TermOrder == 2)) //a or b
   {
      if ((i > 0) && (BoxArray[Get3DIndex(i - 1,j,k)].Dummybool2)) {
        DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i - 1,j,k)]; Parcels1++;
      }
      if ((k > 0) && (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)){
        DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j,k-1)]; Parcels1++;
      }
      if ((i > 0) && (k > 0) && (BoxArray[Get3DIndex(i-1,j,k-1)].Dummybool2)){
        DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i-1,j,k-1)]; Parcels1++;
      }
      DepthForLowerDiffusion = DepthForLowerDiffusion / Parcels1;

      if (
            (k > 0) &&
            (
                (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2) &&
                (
                   (i > 0) && (BoxArray[Get3DIndex(i - 1,j,k - 1)].Dummybool2)
                )
            )
         )
         MyVerticalDiffusion = MyVerticalDiffusion + VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] * ATimeStep /
                                                     pow(DepthForLowerDiffusion, 2.0);
   }
   if ((TermOrder == 3) || (TermOrder == 2)) //c or b
   {
      if ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)) {
        DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i-1,j,k)]; Parcels2++;
      }
      if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)){
        DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j,k+1)]; Parcels2++;
      }
      if ((i > 0) && (k < UpperLayer) && (BoxArray[Get3DIndex(i-1,j,k+1)].Dummybool2)){
        DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i-1,j,k+1)]; Parcels2++;
      }
      DepthForUpperDiffusion = DepthForUpperDiffusion / Parcels2;

      if (
            (k < UpperLayer) &&
            (
               (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2) &&
               (
                   (i > 0) && (BoxArray[Get3DIndex(i-1,j,k + 1)].Dummybool2)
               )
            )
         )
         MyVerticalDiffusion = MyVerticalDiffusion + VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k+1)] * ATimeStep /
                                                     pow(DepthForUpperDiffusion, 2.0);
   }
   return MyVerticalDiffusion;
}


double TTriDimensional::VVerticalDiffusionExplicit(int Line, int Column, int ALayer)
{
    int i, j, k;
    i = Line; j = Column; k = ALayer;
    double DepthForUpperDiffusion, DepthForLowerDiffusion, LowerDiffusion, UpperDiffusion, Parcels1, Parcels2;
    DepthForLowerDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels1 = 1.0;
    DepthForUpperDiffusion = BoxDepth[Get3DIndex(i,j,k)]; Parcels2 = 1.0;
    LowerDiffusion = 0.0; UpperDiffusion = 0.0;

    if ((i > 0) && (BoxArray[Get3DIndex(i - 1,j,k)].Dummybool2)) {
      DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i - 1,j,k)]; Parcels1++;
    }
    if ((k > 0) && (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)){
      DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i,j,k-1)]; Parcels1++;
    }
    if ((i > 0) && (k > 0) && (BoxArray[Get3DIndex(i-1,j,k-1)].Dummybool2)){
      DepthForLowerDiffusion = DepthForLowerDiffusion + BoxDepth[Get3DIndex(i-1,j,k-1)]; Parcels1++;
    }
    DepthForLowerDiffusion = DepthForLowerDiffusion / Parcels1;

    if ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)) {
      DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i-1,j,k)]; Parcels2++;
    }
    if ((k < UpperLayer) && (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2)){
      DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i,j,k+1)]; Parcels2++;
    }
    if ((i > 0) && (k < UpperLayer) && (BoxArray[Get3DIndex(i-1,j,k+1)].Dummybool2)){
      DepthForUpperDiffusion = DepthForUpperDiffusion + BoxDepth[Get3DIndex(i-1,j,k+1)]; Parcels2++;
    }
    DepthForUpperDiffusion = DepthForUpperDiffusion / Parcels2; 

    if (
            (k > 0) &&
            (
                (BoxArray[Get3DIndex(i,j,k-1)].Dummybool2) &&
                (
                   (i > 0) && (BoxArray[Get3DIndex(i - 1,j,k - 1)].Dummybool2)
                )
            )
         )
         LowerDiffusion = VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] * ATimeStep *
                          (VVelocityPast[Get3DIndexForVVelocity(i,j,k)] - VVelocityPast[Get3DIndexForVVelocity(i,j,k - 1)]) /
                          pow(DepthForLowerDiffusion, 2.0);
    if (
            (k < UpperLayer) &&
            (
               (BoxArray[Get3DIndex(i,j,k+1)].Dummybool2) &&
               (
                   (i > 0) && (BoxArray[Get3DIndex(i-1,j,k + 1)].Dummybool2)
               )
            )
         )
         UpperDiffusion = VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k+1)] * ATimeStep *
                         (VVelocityPast[Get3DIndexForVVelocity(i,j,k + 1)] - VVelocityPast[Get3DIndexForVVelocity(i,j,k)]) /
                         pow(DepthForUpperDiffusion, 2.0);

  return UpperDiffusion - LowerDiffusion;
}


double TTriDimensional::DragOfUEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   
   if ((j > 0) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2))
   {
      return ATimeStep /
          (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j - 1,k)]) *
          (CoefRugosidade(i,j,k) + CoefRugosidade(i,j-1,k)) *
          UForRugosity(i,j,k);
   }
   else
      return 0.0;
}


double TTriDimensional::UForRugosity(int ALine, int AColumn, int ALayer)
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;
   double VVelocityComponent = 0.0, NumberOfParcels = 2.0;
   VVelocityComponent = VVelocityComponent + VVelocity[Get3DIndexForVVelocity(i,j,k)] + VVelocity[Get3DIndexForVVelocity(i+1,j,k) ];
   if ((j > 0) && (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2))
   {
          VVelocityComponent = VVelocityComponent + VVelocity[Get3DIndexForVVelocity(i,j-1,k)];
          NumberOfParcels++;
          VVelocityComponent = VVelocityComponent + VVelocity[Get3DIndexForVVelocity(i + 1,j-1,k)];
          NumberOfParcels++;
   }
   return sqrt(
                sqr(UVelocityPast[Get3DIndexForUVelocity(i,j,k)]) +
                sqr(VVelocityComponent / NumberOfParcels)
              );
}

double TTriDimensional::VForRugosity(int ALine, int AColumn, int ALayer)
{
  int i,j,k;
  i = ALine; j = AColumn; k = ALayer;
  double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocity[Get3DIndexForUVelocity(i,j,k)] + UVelocity[Get3DIndexForUVelocity(i,j+1,k) ];
   if ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2))
   {
   	  UVelocityComponent = UVelocityComponent + UVelocity[Get3DIndexForUVelocity(i-1,j,k)];
   	  NumberOfParcels++;
   	  UVelocityComponent = UVelocityComponent + UVelocity[Get3DIndexForUVelocity(i-1,j+1,k)];
   	  NumberOfParcels++;
   }
    return sqrt(
                  sqr(VVelocityPast[Get3DIndexForVVelocity(i,j,k)]) +
                  sqr(UVelocityComponent / NumberOfParcels)
                );
}

double TTriDimensional::CoefRugosidade(int ALine, int AColumn, int ALayer)  //Tese do Aires, pág. 137
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;
   const double VonKarman = 0.4;
                //Rugosity = 0.0025, //m
                //HMINC = 0.5; //m
   double DistanceFromTheBottom, Z, MyCoefficient;
   DistanceFromTheBottom = Depth(i,j,0) - (Depth(i,j,k) - BoxDepth[Get3DIndex(i,j,k)] / 2.0);

    //Retiro metade da espessura da caixa à distância ao fundo do respectivo topo
   Z = MAX(DistanceFromTheBottom, HMINC);
   MyCoefficient = pow(VonKarman, 2.0) / pow((log(Z / Rugosity[Get3DIndex(i,j,k)])),2.0);
   return MyCoefficient;
}

double TTriDimensional::Depth(int ALine, int AColumn, int ALayer)
{
   int i,j,k,MyLayer;
   i = ALine; j = AColumn;
   double Total;
   Total = 0.0; MyLayer = ALayer;
   for (int k = UpperLayer; k >= MyLayer; k--)
   {
   	if (BoxDepth[Get3DIndex(i,j,k)] > 0.0)
   		Total = Total + BoxDepth[Get3DIndex(i,j,k)];
   }
   return Total;
}

double TTriDimensional::UImplicitForDrag(int ALine, int AColumn, int ALayer)
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
               * (CoefRugosidade(i,j,k) + CoefRugosidade(i,j - 1,k)) * UForRugosity(i, j, k)
            );
   }
   else
      return 0.0;
}


double TTriDimensional::VWEHorizontalDiffusion(int ALine, int AColumn, int ALayer)
{
    int i, j, k;  double MeanLength1, MeanLength2, WesternFlux, EasternFlux;
    i = ALine; j = AColumn; k = ALayer;
    WesternFlux = 0.0; EasternFlux = 0.0;
    //Western flux
    if (
          (j > 0) && (i > 0) &&
          (BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) &&
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 4) &&
          (BoxArray[Get3DIndex(i,j-1,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
          (BoxArray[Get3DIndex(i-1,j-1,k)].Dummybool2)
       )
    {
       MeanLength1 = 0.5 * (BoxLength[Get3DIndex(i,j,k)] + BoxLength[Get3DIndex(i,j-1,k)]);
       if (MeanLength1 <= 0.0) 
#ifdef __BORLANDC__
            MessageBox(0, "TTriDimensional::VWEHorizontalDiffusion - zero MeanLength1", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
           cerr << "TTriDimensional::VWEHorizontalDiffusion - zero MeanLength1" << endl;
#endif  // __BORLANDC__
       WesternFlux = HorizontalDiffCoef *
                  CorrectionOfHorizontalDiffCoef(LowerLeftCornerDepth(i,j,k), LeftVVelocity(i,j,k))*
                     (
                         VVelocityPast[Get3DIndexForVVelocity(i,j,k)] -
                         VVelocityPast[Get3DIndexForVVelocity(i,j-1,k)]
                     ) / MeanLength1;
    }
    else WesternFlux = 0.0;

    //EasternFlux
    if (
          (j < GridColumns - 1) && (i > 0) &&
          (BoxArray[Get3DIndex(i,j+1,k)].Dummybool2) &&
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 4) &&
          (BoxArray[Get3DIndex(i,j+1,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 4) &&
          (BoxArray[Get3DIndex(i-1,j+1,k)].Dummybool2)
       )
    {
       MeanLength2 = 0.5 * (BoxLength[Get3DIndex(i,j,k)] + BoxLength[Get3DIndex(i,j+1,k)]);
       if (MeanLength2 <= 0.0) 
#ifdef __BORLANDC__
            MessageBox(0, "TTriDimensional::VWEHorizontalDiffusion - zero MeanLength2", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
           cerr << "TTriDimensional::VWEHorizontalDiffusion - zero MeanLength2" << endl;
#endif  // __BORLANDC__
       EasternFlux = HorizontalDiffCoef *
                   CorrectionOfHorizontalDiffCoef(LowerRightCornerDepth(i,j,k), RightVVelocity(i,j,k))*
                     (
                         VVelocityPast[Get3DIndexForVVelocity(i,j + 1,k)] -
                         VVelocityPast[Get3DIndexForVVelocity(i,j,k)]
                     ) / MeanLength2;
    }
    else EasternFlux = 0.0;

    return  ATimeStep * (EasternFlux - WesternFlux) /  BoxLength[Get3DIndex(i,j,k)];
}



double TTriDimensional::VNSHorizontalDiffusion(int ALine, int AColumn, int ALayer)
{
  int i, j, k;
  i = ALine; j = AColumn; k = ALayer;
  double NorthernFlux, SouthernFlux, MyWidth;
  double const TINNY = 0.000000001;
  MyWidth = 0.0;
  //Southern flux
  if (
        (i > 0) &&
        (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2)
      )
      SouthernFlux = HorizontalDiffCoef *
                    CorrectionOfHorizontalDiffCoef(BoxDepth[Get3DIndex(i-1,j,k)], UPStreamVVelocity(i,j,k))*
                    (
                        VVelocityPast[Get3DIndexForVVelocity(i,j,k)] - VVelocityPast[Get3DIndexForVVelocity(i-1,j,k)]
                    ) / BoxWidth[Get3DIndex(i-1,j,k)];
   else SouthernFlux = 0.0;
   //Northern flux
   if (i < GridLines)
      	NorthernFlux = HorizontalDiffCoef *
                       CorrectionOfHorizontalDiffCoef(BoxDepth[Get3DIndex(i,j,k)], DOWNStreamVVelocity(i,j,k))*
                       (
                      		VVelocityPast[Get3DIndexForVVelocity(i + 1,j,k)] -
                        	VVelocityPast[Get3DIndexForVVelocity(i,j,k)]
                      	) / BoxWidth[Get3DIndex(i,j,k)];
    else NorthernFlux = 0.0;
    if ((i > 0) && (BoxArray[Get3DIndex(i-1,j,k)].Dummybool2))
       MyWidth = 0.5 * (BoxWidth[Get3DIndex(i-1,j,k)] + BoxWidth[Get3DIndex(i,j,k)]);
    else
       MyWidth = BoxWidth[Get3DIndex(i,j,k)];
    if (MyWidth > fabs(TINNY))
      return  ATimeStep * (NorthernFlux - SouthernFlux) / MyWidth;
    else
      return 0.0;
}

double TTriDimensional::DragOfVEquation(int ALine, int AColumn, int ALayer)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i > 0) && (BoxArray[Get3DIndex(i - 1,j ,k)].Dummybool2))
      return ATimeStep /
          (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]) *
                       		(
                                   CoefRugosidade(i,j,k) +
                                   CoefRugosidade(i-1,j,k)
                       		) * VForRugosity(i,j,k);
   else
      return 0.0;

}


double TTriDimensional::VImplicitForDrag(int ALine, int AColumn, int ALayer)
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
            (
               1.0 + ATimeStep / (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i- 1,j ,k)])
               * (CoefRugosidade(i,j,k) + CoefRugosidade(i - 1,j,k)) * VForRugosity(i, j, k)
            );
   }
   else
   return 0.0;
}


void TTriDimensional::EquationOfMotionUImplicit()    //Implicit scheme
{
    int k, index, indexMenosUmJ, indexU;
    k = 0;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    
    //for (int i = 0; i < GridLines; i++)
    for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
    {
        //for (int j = 0; j < GridColumns; j++)
        for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
        {
           /*if ((i == 176) && (j == 317)) {
             Debugger(pSubDomain->IColumn[i]); Debugger(pSubDomain->FColumn[i]);
           }*/
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
                        btemporary[index] = btemporary[index] + (CoefRugosidade(i,j,layer) + CoefRugosidade(i,j-1,layer)) * ATimeStep /
                                                                 (BoxDepth[Get3DIndex(i,j,layer)] + BoxDepth[Get3DIndex(i,j - 1,layer)]) *
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
}

void TTriDimensional::EquationOfMotionVImplicit()    //Implicit scheme
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
                    btemporary[index] = btemporary[index] + (CoefRugosidade(i,j,layer) + CoefRugosidade(i-1,j,layer)) * ATimeStep /
                                                            (BoxDepth[Get3DIndex(i,j,layer)] + BoxDepth[Get3DIndex(i-1,j,layer)]) *
                                                            VForRugosity(i,j,layer);

                    ctemporary[index] = -VVerticalDiffusion(3, i, j, layer);//cVCalculation(i,j,layer);
                 }

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
}

void TTriDimensional::EquationOfMotionUExplicit()    //Explicit scheme
{
  int index, indexMenosUmJ, indexU;
  SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
  //for (int i = 0; i < GridLines; i++)
  for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
  {
     for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
     {
        for (int layer = 0; layer <= UpperLayer; layer++)
        {
          //if ((i == 0) && (j == 1)) Debugger(layer);
          //if ((i == 0) && (j == 1)) Debugger(13);
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
                 /*if ((i == 0) && (j == 1)) Debugger(16);
                 if ((i == 0) && (j == 1))
                 {
                   Debugger(UVelocityPast[indexU]);
                   Debugger(UConvectiveTerm(i,j,layer));
                   Debugger(WEBarotropicPressureTerm(i,j, j - 1, layer));
                   Debugger(WEBaroclinicPressureTerm(i,j, j - 1, layer));
                   Debugger(VCoriolis(i,j,layer));
                   Debugger(UWEHorizontalDiffusion(i,j,layer));
                   Debugger(1212);
                   Debugger(UNSHorizontalDiffusion(i,j,layer));
                   Debugger(1213); Debugger(layer);
                   Debugger(UVerticalDiffusionExplicit(i,j,layer));
                 }  */
                 UVelocity[indexU] = UVelocityPast[indexU] -
                                     UConvectiveTerm(i,j,layer)-
                                     WEBarotropicPressureTerm(i,j, j - 1, layer)-
                                     WEBaroclinicPressureTerm(i,j, j - 1, layer)+
                                     VCoriolis(i,j,layer)+
                                     UWEHorizontalDiffusion(i,j,layer)+
                                     UNSHorizontalDiffusion(i,j,layer)+
                                     UVerticalDiffusionExplicit(i,j,layer);
                 //if ((i == 0) && (j == 1)) Debugger(17);
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
}

void TTriDimensional::EquationOfMotionVExplicit()    //Explicit scheme
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
                                       VVerticalDiffusionExplicit(i,j,layer);


                   if (layer == UpperLayer)
                   {
                     WindEffect = WindStress(index,"Wind speed (y component)") * ATimeStep /
                                 (
                                   0.5 * (WaterDensity[index] + WaterDensity[indexV]) *
                                   0.5 * (BoxDepth[index] + BoxDepth[indexV])
                                 );
                     VVelocity[Get3DIndexForVVelocity(i,j,layer)] = VVelocity[Get3DIndexForVVelocity(i,j,layer)] +  WindEffect;
                   }

                   if ((layer == 0) || ((layer > 0) && (!BoxArray[Get3DIndex(i,j,layer-1)].Dummybool2)) ||
                        ((layer > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer-1)].Dummybool2)))
                        VVelocity[Get3DIndexForVVelocity(i,j,layer)] = VVelocity[Get3DIndexForVVelocity(i,j,layer)] - DragOfVEquation(i, j, layer) * VVelocityPast[Get3DIndexForVVelocity(i,j,layer)];

               }
            }
          }
        }
     }
  }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------

void TTriDimensional::VerticalVelocity()
{
   double EasternDepth, WesternDepth, SouthernDepth, NorthernDepth, MeanBoxWidth,
          UComponent1, UComponent2, VComponent1, VComponent2,
          WesternFlux, EasternFlux, SouthernFlux, NorthernFlux,
          DischargeFlux, UptakeFlux;
   int index, indexUp;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   //for (int i = 0; i < GridLines; i++)
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
   {
      //for (int j = 0; j < GridColumns; j++)
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i], GridColumns - 1); j++)
      {
         WesternFlux = 0.0; EasternFlux = 0.0; SouthernFlux = 0.0; NorthernFlux = 0.0;
         DischargeFlux = 0.0; UptakeFlux = 0.0;
         for (int layer = 0; layer <= UpperLayer; layer++)
         {
            if (!CheckForSeaBoundary(i,j,layer)) {
            index = Get3DIndex(i,j,layer);

            if (BoxArray[index].Dummybool2)
            {
                indexUp = Get3DIndex(i,j,layer + 1);
                if (
                      (j == GridColumns - 1) ||
                      (
                        (j < GridColumns - 1) && (!BoxArray[Get3DIndex(i,j + 1,layer)].Dummybool2)
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
                      (j > 0) && (!BoxArray[Get3DIndex(i,j - 1,layer)].Dummybool2)
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
                        (i < GridLines - 1) && (!BoxArray[Get3DIndex(i + 1,j,layer)].Dummybool2)
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
                      (i > 0) && (!BoxArray[Get3DIndex(i - 1,j,layer)].Dummybool2)
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

                if (layer < UpperLayer)
                   WVelocity[indexUp] = WVelocity[index] -
                                     (UComponent2 * EasternDepth - UComponent1 * WesternDepth) / BoxLength[index] -
                                     (VComponent2 * NorthernDepth - VComponent1 * SouthernDepth) / BoxWidth[index] +
                                     (FlowDischarge[index] - FlowUptake[index]) / (BoxWidth[index] * BoxLength[index]);
            } //Sea boundary check
            }
         }  //layer
         index = Get3DIndex(i,j,UpperLayer);
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
        }
      }//j
   }   //i
}

void TTriDimensional::DischargeForcing()
{
   if (MyLoadRecord != NULL)
   {
       int i, j, l;
       int ADay, AHour, AMoment;
       NumberOfDaysForLoads = MyPEcoDynClass->GetNumberOfDaysForLoads();
       NumberOfHoursForLoads = MyPEcoDynClass->GetNumberOfHoursForLoads();
       if  (NumberOfDaysForLoads >= 1)
       {
           ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForLoads - 1);
           AMoment = ADay;
       }
       else
       if (NumberOfHoursForLoads >= 1)
       {
           ADay = MyPEcoDynClass->GetJulianDay() - 1;
           AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
           AMoment = MIN(AHour, NumberOfHoursForLoads - 1);
       }
       else
#ifdef __BORLANDC__
           MessageBox(0, "TTridimensional::Continuity", "Incorrect value in NumberOfDaysForLoads or NumberOfHoursForLoads", MB_OK);
#else  // __BORLANDC__
           cerr << "TTriDimensional::Continuity - Incorrect value in NumberOfDaysForLoads or NumberOfHoursForLoads" << endl;
#endif  // __BORLANDC__

       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
       {
          i = MyLoadRecord[k].LineCoordinate;
          j = MyLoadRecord[k].ColumnCoordinate;
          l = MyLoadRecord[k].LayerCoordinate;
          if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j)) && (BoxDepth[Get3DIndex(i,j,l)] > 0.0))
          {
            FlowDischarge[Get3DIndex(i,j,l)] = MyLoadRecord[k].LoadFlow[AMoment];  //m3/s
          }
       }
   }
}

void TTriDimensional::UptakeForcing()
{
   if (MyLossesRecord != NULL)
   {
       int i, j, l;
       int ADay, AHour, AMoment;
       NumberOfDaysForLosses = MyPEcoDynClass->GetNumberOfDaysForLosses();
       NumberOfHoursForLosses = MyPEcoDynClass->GetNumberOfHoursForLosses();
       if  (NumberOfDaysForLosses >= 1)
       {
           ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForLosses - 1);
           AMoment = ADay;
       }
       else
       if (NumberOfHoursForLosses >= 1)
       {
           ADay = MyPEcoDynClass->GetJulianDay() - 1;
           AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
           AMoment = MIN(AHour, NumberOfHoursForLosses - 1);
       }
       else
#ifdef __BORLANDC__
           MessageBox(0, "TTridimensional::Continuity", "Incorrect value in NumberOfDaysForLosses or NumberOfHoursForLosses", MB_OK);
#else  // __BORLANDC__
           cerr << "TTriDimensional::Continuity - Incorrect value in NumberOfDaysForLosses or NumberOfHoursForLosses" << endl;
#endif  // __BORLANDC__

       for (int k = 0; k < MyPEcoDynClass->GetNumberOfLosses(); k++)
       {
          i = MyLossesRecord[k].LineCoordinate;
          j = MyLossesRecord[k].ColumnCoordinate;
          l = MyLossesRecord[k].LayerCoordinate;
          if ((!CheckForSeaBoundariesInColumns(i,j)) && (!CheckForRivers(i,j)) && (BoxDepth[Get3DIndex(i,j,l)] > 0.0))
          {
            FlowUptake[Get3DIndex(i,j,l)] = MyLossesRecord[k].LossFlow[AMoment];  //m3/s
          }
       }
   }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensional::AdvectDiffuse(double* Generic)
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
//-------------------------------------------------------------------------------------------------------------------------------------------------
/*void TTriDimensional::AdvectDiffuse(double* Generic)
{
   double EasternDepth, WesternDepth, SouthernDepth, NorthernDepth,
          UComponent1, UComponent2, VComponent1, VComponent2, WComponent1, WComponent2,
          WesternConcentration, EasternConcentration, SouthernConcentration, NorthernConcentration,
          ConcentrationBelow, ConcentrationAbove;
   int index;

   double* PastGeneric = new double[NumberOfBoxes];
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int i = 0; i < NumberOfBoxes; i++)
   	  PastGeneric[i] = Generic[i];

   for (int i = 0; i < GridLines; i++)
   {
      //for (int j = 0; j < GridColumns; j++)
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
                    if (BoxVolume[index] > TINNY)
                       Generic[index] = PastGeneric[index] * PastBoxVolume[index] / BoxVolume[index] +
                                         (
                                           (UComponent1 * WesternDepth * WesternConcentration - UComponent2 * EasternDepth * EasternConcentration) * BoxWidth[index] +
                                           (VComponent1 * SouthernDepth * SouthernConcentration - VComponent2 * NorthernDepth * NorthernConcentration) * BoxLength[index] +
                                           (
                                               WComponent1 * BoxLength[index]* BoxWidth[index] * ConcentrationBelow -
                                               WComponent2 * BoxLength[index]* BoxWidth[index] * ConcentrationAbove
                                           )
                                           //+(FlowDischarge[index] - FlowUptake[index])
                                         ) / BoxVolume[index] * ATimeStep;
                    else
                       Generic[index] = PastGeneric[index];
                }
            }
         }  //k
      }     //j
   }        //i
   delete [] PastGeneric;
} */
//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::SaltWEHorizontalDiffusion(int i, int j, int k, double *PastGeneric)
{
   double const TINNY = 0.000000001;
   int index = Get3DIndex(i,j,k);
   double MyLength, WesternFlux, EasternFlux;
   if (
        (j > 0) &&
        (BoxArray[index].Dummybool2)&&
        (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2) &&
        (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
       )
   {
      MyLength = (0.5 * (BoxLength[Get3DIndex(i,j,k)] + BoxLength[Get3DIndex(i,j-1,k)]));
      if (MyLength > TINNY)
         WesternFlux = HorizontalDiffCoef *
              CorrectionOfHorizontalDiffCoef(0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i,j - 1,k)]), UPStreamUVelocity(i,j,k)) *
              (PastGeneric[index] - PastGeneric[Get3DIndex(i,j - 1,k)]) *
              0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i,j - 1,k)]) / MyLength;
   }
   else
      WesternFlux = 0.0;

   if (
        (j < GridColumns - 1) &&
        (BoxArray[index].Dummybool2)&&
        (BoxArray[Get3DIndex(i,j + 1,k)].Dummybool2) &&
        (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1)
       )
   {
      MyLength = (0.5 * (BoxLength[Get3DIndex(i,j,k)] + BoxLength[Get3DIndex(i,j+1,k)]));
      if (MyLength > TINNY)
         EasternFlux = HorizontalDiffCoef *
              CorrectionOfHorizontalDiffCoef(0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i,j + 1,k)]), DOWNStreamUVelocity(i,j,k)) *
              (PastGeneric[Get3DIndex(i,j + 1,k)] - PastGeneric[index]) *
              0.5 * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / MyLength;
   }
   else
      EasternFlux = 0.0;
   return (EasternFlux - WesternFlux) / BoxLength[index];
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::SaltNSHorizontalDiffusion(int i, int j, int k, double *PastGeneric)
{
   double const TINNY = 0.000000001;
   int index = Get3DIndex(i,j,k);
   double MyWidth, NorthernFlux, SouthernFlux;
   if (
        (i > 0) &&
        (BoxArray[index].Dummybool2)&&
        (BoxArray[Get3DIndex(i - 1,j,k)].Dummybool2) &&
        (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1)
       )
   {
      MyWidth = (0.5 * (BoxWidth[Get3DIndex(i,j,k)] + BoxWidth[Get3DIndex(i-1,j,k)]));
      if (MyWidth > TINNY)
         SouthernFlux = HorizontalDiffCoef *
              CorrectionOfHorizontalDiffCoef(0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i-1,j,k)]), UPStreamVVelocity(i,j,k)) *
              (PastGeneric[index] - PastGeneric[Get3DIndex(i-1,j,k)]) *
              0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i-1,j,k)]) / MyWidth;
   }
   else
      SouthernFlux = 0.0;

   if (
        (i < GridLines - 1) &&
        (BoxArray[index].Dummybool2)&&
        (BoxArray[Get3DIndex(i+1,j,k)].Dummybool2) &&
        (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1)
       )
   {
      MyWidth = (0.5 * (BoxWidth[Get3DIndex(i,j,k)] + BoxWidth[Get3DIndex(i+1,j,k)]));
      if (MyWidth > TINNY)
         NorthernFlux = HorizontalDiffCoef *
              CorrectionOfHorizontalDiffCoef(0.5 * (BoxDepth[index] + BoxDepth[Get3DIndex(i+1,j,k)]), DOWNStreamVVelocity(i,j,k)) *
              (PastGeneric[Get3DIndex(i+1,j,k)] - PastGeneric[index]) *
              0.5 * (BoxDepth[Get3DIndex(i+1,j,k)] + BoxDepth[index]) / MyWidth;
   }
   else
      NorthernFlux = 0.0;
   return (NorthernFlux - SouthernFlux) / BoxWidth[index];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::aUCalculation(int ALine, int AColumn, int ALayer)
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;
   if (
        (k == 0) ||
        (
          //(k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] <= CriticalDepthForLandBoundary) /*Fundo=> Não há difusão, mas sim atrito*/
          (k > 0) && (!BoxArray[Get3DIndex(i,j,k-1)].Dummybool2) 
        )
      )
      return 0.0;
   else
      return -VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k)] * ATimeStep /
             pow(0.5*(BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::bUCalculation(int ALine, int AColumn, int ALayer)
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
                    * (CoefRugosidade(i,j,k) + CoefRugosidade(i,j - 1,k)) * fabs(UVelocityPast[Get3DIndexForUVelocity(i,j,k)])
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
double TTriDimensional::cUCalculation(int ALine, int AColumn, int ALayer)
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;

   if (k >= UpperLayer)
   {
      return 0.0;
   }
   else
      return -VerticalDiffCoefU[Get3DIndexForUVelocity(i,j,k+1)] * ATimeStep /
             pow(0.5*(BoxDepth[Get3DIndex(i,j,k+1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::aVCalculation(int ALine, int AColumn, int ALayer)
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;
   if (
         	(k == 0) ||
         	(
         		//(k > 0) && (BoxDepth[Get3DIndex(i,j,k-1)] <= CriticalDepthForLandBoundary) /*Fundo=> Não há difusão, mas sim atrito*/
                        (k > 0) && (!BoxArray[Get3DIndex(i,j,k-1)].Dummybool2)
         	)
      )
   		return 0.0;
   else
      	return -VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k)] * ATimeStep /
                pow(0.5*(BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0)
             	 /*pow(BoxLength[i * GridColumns + j],2.0)*/;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::bVCalculation(int ALine, int AColumn, int ALayer)
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
                    * (CoefRugosidade(i,j,k) + CoefRugosidade(i-1,j,k)) * fabs(VVelocityPast[Get3DIndexForVVelocity(i,j,k)])
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
double TTriDimensional::cVCalculation(int ALine, int AColumn, int ALayer)
{
   int i,j,k;
   i = ALine; j = AColumn; k = ALayer;
   if (k >= UpperLayer)
   {
      	return 0.0;
   }
   else
   		return -VerticalDiffCoefV[Get3DIndexForVVelocity(i,j,k+1)] * ATimeStep /
                pow(0.5*(BoxDepth[Get3DIndex(i,j,k+1)] + BoxDepth[Get3DIndex(i,j,k)]),2.0)
             	 /*pow(BoxLength[i * GridColumns + j],2.0)*/;

}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::WindStress(int ABox, char* ParameterName)
{
   TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
   if (MyWindPointer != NULL)
   {
      double MyWindSpeed, ResultantSpeed;
      char MyParameter[65]; // 64 characters (max) for parameter name
      strcpy(MyParameter, ParameterName);
      if (strcmp(MyParameter, MyParameter) == 0)
      {
         MyWindPointer->Inquiry(GetEcoDynClassName(),MyWindSpeed,ABox,MyParameter,ObjectCode);
         MyWindPointer->Inquiry(GetEcoDynClassName(),ResultantSpeed,ABox,"Wind speed",ObjectCode);
      }
      //return WindStressCoef * pow(MyWindSpeed,2.0);
      //return WindStressCoef * MyWindSpeed * fabs(MyWindSpeed);
      return WindStressCoef * MyWindSpeed * fabs(ResultantSpeed);
   }
   else
      return 0.0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::UpperUVelocity(int ALine, int AColumn, int ALayer)   //Calcula o Ui+1/2,j    ok
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns) || (k < 0) || (k > GridLayers))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TTriDimensional::UpperUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TTriDimensional::UpperUVelocity - erro" << endl;
#endif  // __BORLANDC__
	else
   {
		if (
      		(i == GridLines - 1) ||
            (
            	(j <= GridColumns) &&		//Modificado a 19/2/2004
            	(i < GridLines - 1) &&
                (j > 0) &&
               //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary)
               (! BoxArray[Get3DIndex(i + 1,j,k)].Dummybool2) || (!BoxArray[Get3DIndex(i + 1,j - 1,k)].Dummybool2)		// Modificado a 19/2/2004
            )              /*      ||
           (                                                                                                                    // Modificado a 19/2/2004
            	(j == GridColumns) &&
            	(i < GridLines - 1) &&
               //(BoxDepth[(i + 1) * GridColumns + j - 1] < CriticalDepthForLandBoundary)
               (! BoxArray[(i + 1) * GridColumns + j - 1].Dummybool2)
            )  */
         )
         return UVelocityPast[Get3DIndexForUVelocity(i,j,k)]; //Se a superfície lateral da parede for < 10% da superfície horizontal
                                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).
   	else
   		return (
                  UVelocityPast[Get3DIndexForUVelocity(i,j,k)] +
         			UVelocityPast[Get3DIndexForUVelocity(i+1,j,k)]
                ) / 2.0;
   }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::LowerUVelocity(int ALine, int AColumn, int ALayer)  //Calcula o Ui-1/2,j      ok
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns) || (k < 0) || (k > GridLayers))
   {
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TTriDimensional::LowerUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TTriDimensional::LowerUVelocity - erro" << endl;
#endif  // __BORLANDC__
      //Debugger(i); Debugger(j);
   }
	else
   {
		if (
      		(i == 0) ||
            (
            	(j <= GridColumns) &&                      //Modificado a 19/2/2004
            	(i > 0) &&
                (j > 0) &&
               //(BoxDepth[indexV] < CriticalDepthForLandBoundary) //Fronteira com terra
               ((! BoxArray[Get3DIndex(i - 1,j,k)].Dummybool2) ||  (! BoxArray[Get3DIndex(i - 1,j - 1,k)].Dummybool2))       //Modificado a 19/2/2004
            )       /* ||                                                                                                            //Modificado a 19/2/2004
            (
            	(j == GridColumns)&&
            	(i > 0) &&
               //(BoxDepth[indexV - 1] < CriticalDepthForLandBoundary) //Fronteira com terra
               (! BoxArray[indexV - 1].Dummybool2)
            )  */
         )

      		return UVelocityPast[Get3DIndexForUVelocity(i,j,k)];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                            //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                            //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                            //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                            //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   		else
        		return (
                  	UVelocityPast[Get3DIndexForUVelocity(i-1,j,k)] +
         				UVelocityPast[Get3DIndexForUVelocity(i,j,k)]
                	  ) / 2.0;
   }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
double TTriDimensional::UpperVVelocity (int ALine, int AColumn, int ALayer)
{
 /*       int i, j, k;
   i = ALine; j = AColumn; k = ALayer;

   if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns) || (k < 0) || (k > GridLayers))
   	MessageBox(0,
					  "Erro in TBiDimensionalIntertidal::UpperVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
	else
   {
		if (
      		(j == GridColumns - 1) ||
            (
            	(i <= GridLines) &&		//Modificado a 19/2/2004
            	(j < GridColumns - 1) &&
                (i > 0) &&
               //(BoxDepth[(i + 1) * GridColumns + j] < CriticalDepthForLandBoundary)
               (! BoxArray[Get3DIndex(i,j + 1,k)].Dummybool2) || (!BoxArray[Get3DIndex(i - 1,j + 1 ,k)].Dummybool2)
            )
         )
         return VVelocityPast[Get3DIndexForVVelocity(i,j,k)]; //Se a superfície lateral da parede for < 10% da superfície horizontal
                                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).
   	else
   		return (
                  VVelocityPast[Get3DIndexForVVelocity(i,j,k)] +
         			VVelocityPast[Get3DIndexForVVelocity(i,j + 1,k)]
                ) / 2.0;
   }*/
}
double TTriDimensional::LowerVVelocity (int ALine, int AColumn, int ALayer)
{
     /*   int i, j, k;
    i = ALine; j = AColumn; k = ALayer;

	if ((i < 0) || (i >= GridLines) || (j < 0) || (j > GridColumns) || (k < 0) || (k > GridLayers))
   {
   	MessageBox(0,
					  "Erro in TTriDimensional::LowerUVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
      Debugger(i); Debugger(j);
   }
	else
   {
		if (
      		(j == 0) ||
            (
            	(i <= GridLines) &&                      //Modificado a 19/2/2004
            	(j > 0) &&
                (i > 0) &&
               //(BoxDepth[indexV] < CriticalDepthForLandBoundary) //Fronteira com terra
               ((! BoxArray[Get3DIndex(i,j,k)].Dummybool2) ||  (! BoxArray[Get3DIndex(i-1,j,k)].Dummybool2))       //Modificado a 19/2/2004
            )
         )

      		return VVelocityPast[Get3DIndexForVVelocity(i,j,k)];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                            //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                            //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                            //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                            //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   		else
        		return (
                  	VVelocityPast[Get3DIndexForVVelocity(i,j - 1,k)] +
         				VVelocityPast[Get3DIndexForVVelocity(i,j,k)]
                	  ) / 2.0;
   }   */
}
double TTriDimensional::LeftVVelocity(int ALine, int AColumn, int ALayer)   //Calcula o Vi,j-1/2        ok
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   int index = Get3DIndex(i,j,k);
   if ((i < 0) || (i > GridLines) || (j < 0) || (j >= GridColumns))
   {
#ifdef __BORLANDC__
   	MessageBox(0,"Erro in TTriDimensional::LeftVVelocity","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TTriDimensional::LeftVVelocity - erro" << endl;
#endif  // __BORLANDC__
      //Debugger(i); Debugger(j);
   }
   else
   {
      if (
            (j == 0) ||
            (
            	(i <= GridLines) &&
            	(j > 0) &&
                ((! BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2) ||  (! BoxArray[Get3DIndex(i - 1,j - 1,k)].Dummybool2))    //Modificado a 19/2/2004
            )
         ) //Ligação com terra a oeste
         return VVelocityPast[Get3DIndexForVVelocity(i,j,k)];//Se a superfície lateral da parede for < 10% da superfície horizontal
                                    //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                    //pode ser desprezado (Tese do Ramiro, pág. 144).
                                    //Se a superfície lateral for muito grande, então deve utilizar-se a
                                    //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).
      else
         return (VVelocityPast[Get3DIndexForVVelocity(i,j - 1,k)] + VVelocityPast[Get3DIndexForVVelocity(i,j,k)]) / 2.0;
   }
   return 0.0;
}


double TTriDimensional::RightVVelocity(int ALine, int AColumn, int ALayer) //Calcula o Vi,j+1/2          ok
{
  int i, j, k;
  i = ALine; j = AColumn; k = ALayer;
  int index = Get3DIndex(i,j,k);
  int indexV = Get3DIndex(i - 1,j,k);

	if ((i < 0) || (i > GridLines) || (j < 0) || (j >= GridColumns))
#ifdef __BORLANDC__
   	MessageBox(0,
					  "Erro in TTriDimensional::RightVVelocity",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TTriDimensional::RightVvelocity - erro" << endl;
#endif  // __BORLANDC__
	else
   {
      if(
            (j == GridColumns - 1) ||
            (
            (i <= GridLines) &&           //Modificado a 19/2/2004
            (i < GridColumns - 1) &&
            ((! BoxArray[Get3DIndex(i ,j + 1,k)].Dummybool2) || (! BoxArray[Get3DIndex(i - 1,j + 1,k)].Dummybool2))   //Modificado a 19/2/2004
             )
         )
            return VVelocityPast[Get3DIndexForVVelocity(i,j,k)] ;//Se a superfície lateral da parede for < 10% da superfície horizontal
                                                 //da célula o fluxo difusivo é comparável à tensão de corte junto ao fundo e
                                                 //pode ser desprezado (Tese do Ramiro, pág. 144).
                                                 //Se a superfície lateral for muito grande, então deve utilizar-se a
                                                 //condição de "non-glissment" ou "no slipping" (Tese do Ramiro, pág. 144).

   	else
            return (VVelocityPast[Get3DIndexForVVelocity(i,j,k)] + VVelocityPast[Get3DIndexForVVelocity(i,j + 1,k)]) / 2.0;
   }
   return 0.0;
}

double TTriDimensional::VerticalMomentumDiffusivity(int i, int j, int k, double AVelocity, double ALowerVelocity, int VelocityComponent)
{
	//fide Aires Santos (pHD Thesis, p. 22 - 26)
   double DeltaZ, Parcels, MyDiffusivity;
   const double TINNY = 0.0000000001;
   DeltaZ = 0.0; Parcels = 0.0;
   if ((i < 0) || (i > GridLines - 1) || (j < 0) || (j > GridColumns - 1) || (k < 0) || (k > GridLayers))
   {
#ifdef __BORLANDC__
   	MessageBox(0,"Erro in TTriDimensional::VerticalMomentumDiffusivity","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
    cerr << "TTriDimensional::VerticalMomentumDiffusivity - erro" << endl;
#endif  // __BORLANDC__
    //Debugger(i); Debugger(j); Debugger(k);
   }
   if ((VelocityComponent == 1) && ((j == 0) || ((j > 0) && (!BoxArray[Get3DIndex(i,j-1,k)].Dummybool2))))
      return 0.0;
   if ((VelocityComponent == 2) && ((i == 0) || ((i > 0) && (!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2))))
      return 0.0;
   if ((!BoxArray[Get3DIndex(i,j,k)].Dummybool2) || (!BoxArray[Get3DIndex(i,j,k-1)].Dummybool2))
   	  return 0.0;
   else {
      DeltaZ = DeltaZ + BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j,k - 1)];
      Parcels = Parcels + 2.0;
   }
   if ((VelocityComponent == 1) && (j > 0) && ((!BoxArray[Get3DIndex(i,j-1,k)].Dummybool2) || (!BoxArray[Get3DIndex(i,j-1,k-1)].Dummybool2)))
      return 0.0;
   else if (VelocityComponent == 1){
      DeltaZ = DeltaZ + BoxDepth[Get3DIndex(i,j-1,k)] + BoxDepth[Get3DIndex(i,j-1,k - 1)];
      Parcels = Parcels + 2.0;
   }
   if ((VelocityComponent == 2) && (i > 0) && ((!BoxArray[Get3DIndex(i-1,j,k)].Dummybool2) || (!BoxArray[Get3DIndex(i-1,j,k-1)].Dummybool2)))
      return 0.0;
   else if (VelocityComponent == 2){
      DeltaZ = DeltaZ + BoxDepth[Get3DIndex(i-1,j,k)] + BoxDepth[Get3DIndex(i-1,j,k - 1)];
      Parcels = Parcels + 2.0;
   }
   MyDiffusivity = MIN(fabs(pow(LengthScale(i,j,k,AVelocity,ALowerVelocity,VelocityComponent),2.0) *
            (AVelocity - ALowerVelocity) /
          	(DeltaZ / Parcels)),MaxVertDiffusivity);
   if (MyDiffusivity < TINNY)
     return 0.0;
   else
     return MyDiffusivity;
}



double TTriDimensional::LengthScale(int ALine, int AColumn, int ALayer, double AVelocity, double ALowerVelocity, int VelocityComponent)
{
    int i, j, k;
    i = ALine; j = AColumn; k = ALayer;
    double /*delta,*/MyTotalDepth1, MyTotalDepth2, DistanceFromTheBottom1, DistanceFromTheBottom2, MyAverageDepth, MyAverageDistance, MyLengthScale;
    const double VonKarman = 0.4, alfa = 0.8, TINNY = 0.0000000001;
	TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
    /*if (MyWindPointer != NULL)
   	   delta = 0.5;
    else
   	  delta = 1.0;*/
    MyTotalDepth2 = Depth(i,j,0);
    DistanceFromTheBottom2 = MIN(Depth(i,j,k), MyTotalDepth2 - Depth(i,j,k));
    if (VelocityComponent == 1)
    {
        if (j > 0)
        {
            MyTotalDepth1 = Depth(i,j - 1,0);
            DistanceFromTheBottom1 = MIN(Depth(i,j - 1,k), MyTotalDepth1 - Depth(i,j - 1,k));
        }
        else
        {
            MyTotalDepth1 = MyTotalDepth2;
            DistanceFromTheBottom1 = DistanceFromTheBottom2;
        }
    }
    else if (VelocityComponent == 2)
    {
         if (i > 0)
        {
            MyTotalDepth1 = Depth(i-1,j,0);
            DistanceFromTheBottom1 = MIN(Depth(i-1,j,k), MyTotalDepth1 - Depth(i-1,j,k));
        }
        else
        {
            MyTotalDepth1 = MyTotalDepth2;
            DistanceFromTheBottom1 = DistanceFromTheBottom2;
        }
    }

    MyAverageDepth = 0.5 * (MyTotalDepth1 + MyTotalDepth2);
    MyAverageDistance = 0.5 * (DistanceFromTheBottom1 + DistanceFromTheBottom2);

    //Formulation by Nihoul (1982) fide Aires Santos (pHD Thesis, p. 23)
    MyLengthScale = MyAverageDepth * VonKarman * MyAverageDistance / MyAverageDepth *
          (1.0 - delta * MyAverageDistance / MyAverageDepth) *
           exp(-alfa * RichardsonNumber(i,j,k,AVelocity,ALowerVelocity, VelocityComponent));
    if (MyLengthScale < TINNY)
      return 0.0;
    else
      return MyLengthScale;
}



double TTriDimensional::RichardsonNumber(int ALine, int AColumn, int ALayer, double AVelocity, double ALowerVelocity, int VelocityComponent)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   double const TINNY = 0.000000001;
   double const Rs = 0.25;  // Número de Richardson abaixo do qual o shear stress" é suficiente para provocar trocas verticais
   double AValue1, AValue2, Ri;
   Ri = Rs;
   AValue1 = 0.0; AValue2 = 0.0;
   if   (fabs(AVelocity - ALowerVelocity)>= 0.00001)
   {
	  	AValue1 = (
     	      		(
                     -Gravity * (WaterDensity[Get3DIndex(i,j,k)] - WaterDensity[Get3DIndex(i,j,k-1)]) /
         	         (0.5 * (WaterDensity[Get3DIndex(i,j,k-1)] + WaterDensity[Get3DIndex(i,j,k)]))
            	    ) /
                    (
                        ( pow((AVelocity - ALowerVelocity), 2.0)) /
                        (0.5 * (BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j,k)]))
                    )
   				  );
        if (VelocityComponent == 1) {
            if ((j > 0) && (BoxDepth[Get3DIndex(i,j-1,k-1)] > 0.0) && (BoxDepth[Get3DIndex(i,j-1,k)] > 0.0))
            {
              AValue2 = (
                          (
                           -Gravity * (WaterDensity[Get3DIndex(i,j-1,k)] - WaterDensity[Get3DIndex(i,j-1,k-1)]) /
                           (0.5 * (WaterDensity[Get3DIndex(i,j-1,k-1)] + WaterDensity[Get3DIndex(i,j-1,k)]))
                          ) /
                          (
                              ( pow((AVelocity - ALowerVelocity), 2.0)) /
                              (0.5 * (BoxDepth[Get3DIndex(i,j-1,k-1)] + BoxDepth[Get3DIndex(i,j-1,k)]))
                          )
                        );
                //return 0.5 * (MAX(0.0,AValue1) + MAX(0.0,AValue2));
                Ri = 0.5 * (AValue1 + AValue2);
             }
             else
                //return MAX(0.0,AValue1);
                Ri = AValue1;
        }
        if (VelocityComponent == 2) {
            if ((i > 0) && (BoxDepth[Get3DIndex(i-1,j,k-1)] > 0.0) && (BoxDepth[Get3DIndex(i-1,j,k)] > 0.0))
            {
              AValue2 = (
                          (
                           -Gravity * (WaterDensity[Get3DIndex(i-1,j,k)] - WaterDensity[Get3DIndex(i-1,j,k-1)]) /
                           (0.5 * (WaterDensity[Get3DIndex(i-1,j,k-1)] + WaterDensity[Get3DIndex(i-1,j,k)]))
                           ) /
                          (
                               ( pow( (AVelocity - ALowerVelocity), 2.0)) /
                               (0.5 * (BoxDepth[Get3DIndex(i-1,j,k-1)] + BoxDepth[Get3DIndex(i-1,j,k)]))
                          )
                        );
               // return 0.5 * (MAX(0.0,AValue1) + MAX(0.0,AValue2));
               Ri = 0.5 * (AValue1 + AValue2);
            }
            else
               //return MAX(0.0,AValue1);
               Ri = AValue1;
        }
   }
   //Ri = MIN(Ri,Rs);
   if (Ri < TINNY) Ri = 0.0;
   return Ri;
}

/*double TTriDimensional::RichardsonNumber(int ALine, int AColumn, int ALayer, double AVelocity, double ALowerVelocity, int VelocityComponent)
{
   int i, j, k;
   i = ALine; j = AColumn; k = ALayer;
   double const TINNY = 0.000000001;
   double const Rs = 0.25;  // Número de Richardson abaixo do qual o shear stress" é suficiente para provocar trocas verticais
   double Ri, MeanUpperDensity, MeanLowerDensity, MeanUpperDepth, MeanLowerDepth;
   Ri = Rs;
   if   (fabs(AVelocity - ALowerVelocity)>= 0.0000001)
   {
        if (VelocityComponent == 1) {
           MeanUpperDensity = 0.5 * (WaterDensity[Get3DIndex(i,j,k)] + WaterDensity[Get3DIndex(i,j-1,k)]);
           MeanUpperDepth = 0.5 * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i,j-1,k)]);
           MeanLowerDensity = 0.5 * (WaterDensity[Get3DIndex(i,j,k-1)] + WaterDensity[Get3DIndex(i,j-1,k-1)]);
           MeanLowerDepth = 0.5 * (BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i,j-1,k-1)]);
        }
        if (VelocityComponent == 2) {
           MeanUpperDensity = 0.5*(WaterDensity[Get3DIndex(i,j,k)] + WaterDensity[Get3DIndex(i-1,j,k)]);
           MeanUpperDepth = 0.5 * (BoxDepth[Get3DIndex(i,j,k)] + BoxDepth[Get3DIndex(i-1,j,k)]);
           MeanLowerDensity = 0.5 * (WaterDensity[Get3DIndex(i,j,k-1)] + WaterDensity[Get3DIndex(i-1,j,k-1)]);
           MeanLowerDepth = 0.5 * (BoxDepth[Get3DIndex(i,j,k-1)] + BoxDepth[Get3DIndex(i-1,j,k-1)]);
        }
        Ri = (
                (
                 Gravity * (MeanUpperDensity - MeanLowerDensity) /
                 (0.5 * (MeanUpperDensity + MeanLowerDensity))
                ) /
                (
                    ( pow((AVelocity - ALowerVelocity), 2.0)) /
                    (0.5 * (MeanUpperDepth + MeanLowerDepth))
                )
             );
   }
   Ri = fabs(Ri);
   if (Ri < TINNY) Ri = 0.0;
   return Ri;
}*/

void TTriDimensional::TideForcing()
{
   double MyCurrentTime;
   MyTime = MyPEcoDynClass->GetCurrentTime();
   SurfaceHeight = cos(2.0 * M_PI / 12 * MyTime) * 0.5 + 0.5;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   /*for (int i = 0; i < GridLines; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
             if ((BoxArray[Get3DIndex(i,j,UpperLayer)].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2)|| //northern ocean boundary
                 (BoxArray[Get3DIndex(i,j,UpperLayer)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) ||  //southern ocean boundary
                 (BoxArray[Get3DIndex(i,j,UpperLayer)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2)|| //western ocean boundary
                 (BoxArray[Get3DIndex(i,j,UpperLayer)].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2))  //eastern ocean boundary
   	     DynamicHeight[Get3DIndex(i,j,UpperLayer)] = SurfaceHeight;
      }
   } */
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
      //for (int j = 0; j < GridColumns; j++)
      for (int j = pSubDomain->FirstColumn;
         j <= pSubDomain->LastColumn; j++)
      {
        for (int i = pSubDomain->ILine[j]; i <= MIN(pSubDomain->FLine[j], GridLines - 1); i++)
        {
            Index3D = Get3DIndex(i,j, UpperLayer);
            if ((BoxArray[Index3D].BoxToBoundary[1].TypeOfBoundary == 2) ||
                (BoxArray[Index3D].BoxToBoundary[2].TypeOfBoundary == 2) ||
                (BoxArray[Index3D].BoxToBoundary[3].TypeOfBoundary == 2) ||
                (BoxArray[Index3D].BoxToBoundary[4].TypeOfBoundary == 2))
            {
                DynamicHeight[Index3D] = SurfaceHeight;
            }
         }
      }
   }
}


double TTriDimensional::VCoriolis(int i, int j, int k)
{
   int index = Get3DIndex(i,j,k);
   double VVelocityComponent = 0.0, NumberOfParcels = 2.0;
   double ALatitude;
   if ((i < GridLines) && (j < GridColumns))
      VVelocityComponent = VVelocityComponent + VVelocity[Get3DIndexForVVelocity(i,j,k)] + VVelocity[Get3DIndexForVVelocity(i + 1,j,k)];
   if ((j > 0) && (i < GridLines) && (BoxArray[Get3DIndex(i,j - 1,k)].Dummybool2))
   {
   	VVelocityComponent = VVelocityComponent + VVelocity[Get3DIndexForVVelocity(i,j - 1,k)];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocity[Get3DIndexForVVelocity(i + 1,j - 1,k) ];
   	NumberOfParcels++;
   }
   if (MyPEcoDynClass->GetLatitudeBoolean())
       ALatitude = MyPEcoDynClass->GetBoxLatitude(Get3DIndex(i,j,k));
   else
       ALatitude = MyLatitude;
   return ATimeStep * Coriolis ( ALatitude ) * VVelocityComponent / NumberOfParcels;
}


double TTriDimensional::UCoriolis(int i, int j, int k)
{
   double UVelocityComponent = 0.0, NumberOfParcels = 2.0;
   double ALatitude;
   if ((i < GridLines) && (j < GridColumns))
      UVelocityComponent = UVelocityComponent + UVelocity[Get3DIndexForUVelocity(i,j,k)] + UVelocity[Get3DIndexForUVelocity(i,j + 1,k)];
   if ((i > 0) && (j < GridColumns) && (BoxArray[Get3DIndex(i - 1,j,k)].Dummybool2))
   {
   	UVelocityComponent = UVelocityComponent + UVelocity[Get3DIndexForUVelocity(i - 1,j,k)];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocity[Get3DIndexForUVelocity(i - 1,j + 1,k) ];
   	NumberOfParcels++;
   }
   if (MyPEcoDynClass->GetLatitudeBoolean())
       ALatitude = MyPEcoDynClass->GetBoxLatitude(Get3DIndex(i,j,k));
   else
       ALatitude = MyLatitude;
   return ATimeStep * Coriolis ( ALatitude ) * UVelocityComponent / NumberOfParcels;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensional::UpdateOfTidalBoundariesVelocities()
{
   if (MyPEcoDynClass->HasASeaBoundariesRecord()) TidalHeightAndVelocityBoundaries();
   else TidalHeightBoundaries();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensional::TidalHeightBoundaries()
{
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   //for (int i = 0; i < GridLines; i++)
   for (int i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
   {
      //for (int j = 0; j < GridColumns; j++)
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],GridColumns-1); j++)
      {
         for (int k = 0; k <= UpperLayer; k++)
         {
            int index = Get3DIndex(i,j,k);
            int indexU = Get3DIndexForUVelocity(i,j,k);
            if (
                 (BoxArray[index].Dummybool2) &&
                 (
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) ||
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) ||
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2)
                 )
               )
            {
                if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) && (i > 0) && (i < GridLines))  //Northern sea boundary
                {
                    if (k < UpperLayer) //Bottom layer and intermediate layers
                       VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] = (
                                                            (WVelocity[index] - WVelocity[Get3DIndex(i,j,k + 1)]) *
                                                            BoxWidth[index] +
                                                            VVelocity[Get3DIndexForVVelocity(i,j,k)] * (BoxDepth[Get3DIndex(i - 1,j,k)] + BoxDepth[index]) / 2.0
                                                          ) / BoxDepth[index];
                    else
                       VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] = (
                                                            WVelocity[index] * BoxWidth[index] +
                                                            VVelocity[Get3DIndexForVVelocity(i,j,k)] * (BoxDepth[Get3DIndex(i - 1,j,k)] + BoxDepth[index]) / 2.0 -
                                                            (DynamicHeight[index] - PastHeight[index]) * BoxWidth[index] / ATimeStep
                                                          ) / BoxDepth[index];
                }
                else if ((BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) && (i < GridLines))  //Southern sea boundary
                {
                    if (k < UpperLayer) //Bottom layer and intermediate layers
                       VVelocity[Get3DIndexForVVelocity(i,j,k)] = (
                                            (WVelocity[Get3DIndex(i,j,k + 1)] - WVelocity[index]) *
                                            BoxWidth[index] +
                                            VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[index]) / 2.0
                                          ) / BoxDepth[index];
                    else
                       VVelocity[Get3DIndexForVVelocity(i,j,k)] = (
                                            - WVelocity[index] * BoxWidth[index] +
                                            VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[index]) / 2.0 +
                                            (DynamicHeight[index] - PastHeight[index]) * BoxWidth[Get3DIndex(i,j,k)] / ATimeStep
                                          ) / BoxDepth[index];
                }
                else if  ((BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) &&  (j > 0) && (j < GridColumns)) //Eastern sea boundary
                {
                    if (k < UpperLayer) //Bottom layer and intermediate layers
                       UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] = (
                                                                         (WVelocity[index] - WVelocity[Get3DIndex(i,j,k + 1)]) *
                                                                         BoxLength[index] +
                                                                         UVelocity[indexU] * (BoxDepth[Get3DIndex(i,j - 1,k)] + BoxDepth[index]) / 2.0
                                                                       ) / BoxDepth[index];
                    else
                       UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] =  (
                                                                         WVelocity[index] * BoxLength[index] +
                                                                         UVelocity[indexU] * (BoxDepth[Get3DIndex(i,j - 1,k)] + BoxDepth[index]) / 2.0 -
                                                                         (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                                                                       ) / BoxDepth[index];
                }
                else if  ((BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) && (j < GridColumns)) //Western sea boundary
                {
                   if (k < UpperLayer) //Bottom layer and intermediate layers
                       UVelocity[indexU] = (
                                             (WVelocity[Get3DIndex(i,j,k + 1)] - WVelocity[index]) *
                                             BoxLength[index] +
                                             UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / 2.0
                                           ) / BoxDepth[index];
                   else
                       UVelocity[indexU] = (
                                             - WVelocity[index] * BoxLength[index] +
                                             UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / 2.0 +
                                             (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                                           ) / BoxDepth[index];

                }
            }
         }
      }
   }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensional::TidalHeightAndVelocityBoundaries()
{
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   NumberOfDaysForSeaBoundaries = MyPEcoDynClass->GetNumberOfDaysForSeaBoundariesTimeSeries();
   NumberOfHoursForSeaBoundaries = MyPEcoDynClass->GetNumberOfHoursForSeaBoundariesTimeSeries();
   double FlowGradient, TimeSpan1, TimeSpan2;
   int ADay, AHour, AMoment, ANextMoment;
   int i, j, k,index;
   if  (NumberOfDaysForSeaBoundaries >= 1)
   {
       ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForSeaBoundaries - 1);
       AMoment = ADay;
       ANextMoment = MIN(MyPEcoDynClass->GetJulianDay(), NumberOfDaysForSeaBoundaries - 1);
       TimeSpan1 = DAYSTOSECONDS;
       TimeSpan2 = 0.0;
   }
   else
   if (NumberOfHoursForSeaBoundaries >= 1)
   {
       ADay = MyPEcoDynClass->GetJulianDay() - 1;
       AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
       AMoment = MIN(AHour, NumberOfHoursForSeaBoundaries - 1);
       ANextMoment = MIN(AHour + 1, NumberOfHoursForSeaBoundaries - 1);
       TimeSpan1 = HOURSTOSECONDS;
       TimeSpan2 = MyPEcoDynClass->GetCurrTime() * HOURSTOSECONDS - MAX(0.0, MyPEcoDynClass->GetCurrTime() * HOURSTOSECONDS - ATimeStep);
   }
   else
#ifdef __BORLANDC__
       MessageBox(0, "TTriDimensional::TidalHeightAndVelocityBoundaries", "Incorrect value in NumberOfDaysForSeaBoundaries or NumberOfHoursForSeaBoundaries", MB_OK);
#else  // __BORLANDC__
       cerr << "TTriDimensional::TidalHeightAndVelocityBoundaries - Incorrect value in NumberOfDaysForSeaBoundaries or NumberOfHoursForSeaBoundaries" << endl;
#endif  // __BORLANDC__
   //Debugger(NumberOfSeaBoundaries);
   for (int Boundary = 0; Boundary < NumberOfSeaBoundaries; Boundary++)
   {
      i = MySeaBoundariesRecord[Boundary].LineCoordinate;
      j = MySeaBoundariesRecord[Boundary].ColumnCoordinate;
      k = MySeaBoundariesRecord[Boundary].LayerCoordinate;
      /*for (int k = UpperLayer; k >= 0; k--)
      { */
        int index = Get3DIndex(i,j,k);
        int indexU = Get3DIndexForUVelocity(i,j,k);
        if ((BoxArray[index].Dummybool2) && (BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) && (i > 0) && (i < GridLines))  //Northern sea boundary
        {
            VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] = MySeaBoundariesRecord[Boundary].Velocity[AMoment];
            if (
                  (k > 0) && (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                  (VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] != 0.0)
                )
            {
              if (k == UpperLayer)
                 WVelocity[index] =
                      (
                         VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * BoxDepth[index] -
                         VVelocity[Get3DIndexForVVelocity(i,j,k)] * (BoxDepth[Get3DIndex(i - 1,j,k)] + BoxDepth[index]) / 2.0
                         + (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                      ) / BoxLength[index];
              else
                 WVelocity[index] =
                      (
                         VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * BoxDepth[index] -
                         VVelocity[Get3DIndexForVVelocity(i,j,k)] * (BoxDepth[Get3DIndex(i - 1,j,k)] + BoxDepth[index]) / 2.0
                         + WVelocity[Get3DIndex(i,j,k + 1)] * BoxLength[index]
                      ) / BoxLength[index];
            }
            else
            {
               if (k == UpperLayer)
                 VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] =
                        (
                          WVelocity[index] * BoxLength[index] +
                          VVelocity[Get3DIndexForVVelocity(i,j,k)] * (BoxDepth[Get3DIndex(i - 1,j,k)] + BoxDepth[index]) / 2.0 -
                          (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                        ) / BoxDepth[index];
               else
                 VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] =
                       (
                          (WVelocity[index] - WVelocity[Get3DIndex(i,j,k + 1)]) *
                          BoxLength[index] +
                          VVelocity[Get3DIndexForVVelocity(i,j,k)] * (BoxDepth[Get3DIndex(i - 1,j,k)] + BoxDepth[index]) / 2.0
                        ) / BoxDepth[index];

            }
        }
        else if ((BoxArray[index].Dummybool2) && (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) && (i < GridLines))  //Southern sea boundary
        {
            VVelocity[Get3DIndexForVVelocity(i,j,k)] = MySeaBoundariesRecord[Boundary].Velocity[AMoment];
            /*if (((k == 3) || (k == UpperLayer)) && (i == 0) && (j == 3))
            {
              Debugger(k);
              Debugger(Boundary);
              Debugger(VVelocity[Get3DIndexForVVelocity(i,j,k)]);
            } */
            if (
                  (k > 0) && (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                  (VVelocity[Get3DIndexForVVelocity(i,j,k)] != 0.0)
                )
            {
               if (k == UpperLayer)
                 WVelocity[index] =
                      (
                         VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[index]) / 2.0 -
                         VVelocity[Get3DIndexForVVelocity(i,j,k)] * BoxDepth[index]
                         + (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                      ) / BoxLength[index];

               else
                 WVelocity[index] =
                      (
                         VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[index]) / 2.0 -
                         VVelocity[Get3DIndexForVVelocity(i,j,k)] * BoxDepth[index]
                         + WVelocity[Get3DIndex(i,j,k + 1)] * BoxLength[index]
                      ) / BoxLength[index];
            }
            else
            {
               if (k == UpperLayer)
                  VVelocity[Get3DIndexForVVelocity(i,j,k)] = (
                                    - WVelocity[index] * BoxLength[index] +
                                    VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[index]) / 2.0 +
                                    (DynamicHeight[index] - PastHeight[index]) * BoxLength[Get3DIndex(i,j,k)] / ATimeStep
                                  ) / BoxDepth[index];


               else
                  VVelocity[Get3DIndexForVVelocity(i,j,k)] = (
                                    (WVelocity[Get3DIndex(i,j,k + 1)] - WVelocity[index]) *
                                    BoxLength[index] +
                                    VVelocity[Get3DIndexForVVelocity(i + 1,j,k)] * (BoxDepth[Get3DIndex(i + 1,j,k)] + BoxDepth[index]) / 2.0
                                  ) / BoxDepth[index];

            }
        }
        else if  ((BoxArray[index].Dummybool2) &&(BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) &&  (j > 0) && (j < GridColumns)) //Eastern sea boundary
        {
            UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] = MySeaBoundariesRecord[Boundary].Velocity[AMoment];
            //Debugger(UVelocity[Get3DIndexForUVelocity(i,j + 1,k)]);
            if (
                  (k > 0) && (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                  (UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] != 0.0)
                )
            {
              if (k == UpperLayer)
                 WVelocity[index] =
                       (
                         UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * BoxDepth[index]-
                         UVelocity[indexU] * (BoxDepth[Get3DIndex(i,j - 1,k)] + BoxDepth[index]) / 2.0
                         +(DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                       ) / BoxLength[index];
              else
                 WVelocity[index] =
                       (
                         UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * BoxDepth[index]-
                         UVelocity[indexU] * (BoxDepth[Get3DIndex(i,j - 1,k)] + BoxDepth[index]) / 2.0
                         + WVelocity[Get3DIndex(i,j,k + 1)] * BoxLength[index]
                       ) / BoxLength[index];
            }
            else
            {
               if (k == UpperLayer)
                 UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] =  (
                                                                   WVelocity[index] * BoxLength[index] +
                                                                   UVelocity[indexU] * (BoxDepth[Get3DIndex(i,j - 1,k)] + BoxDepth[index]) / 2.0 -
                                                                   (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                                                                 ) / BoxDepth[index];
              else
                 UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] = (
                                                                   (WVelocity[index] - WVelocity[Get3DIndex(i,j,k + 1)]) *
                                                                   BoxLength[index] +
                                                                   UVelocity[indexU] * (BoxDepth[Get3DIndex(i,j - 1,k)] + BoxDepth[index]) / 2.0
                                                                 ) / BoxDepth[index];
            }

        }
        else if  ((BoxArray[index].Dummybool2) &&(BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) && (j < GridColumns)) //Western sea boundary
        {
           UVelocity[Get3DIndexForUVelocity(i,j,k)] = MySeaBoundariesRecord[Boundary].Velocity[AMoment];
           //Debugger(AMoment);
           //Debugger(UVelocity[Get3DIndexForUVelocity(i,j,k)]);
            if (
                  (k > 0) && (BoxArray[Get3DIndex(i,j,k - 1)].Dummybool2) &&
                  (UVelocity[Get3DIndexForUVelocity(i,j,k)] != 0.0)
                )
           {
              if (k == UpperLayer)
                 WVelocity[index] =
                       (
                         UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / 2.0-
                         UVelocity[indexU] * BoxDepth[index]
                         +(DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                       ) / BoxLength[index];
              else
                 WVelocity[index] =
                       (
                         UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / 2.0-
                         UVelocity[indexU] * BoxDepth[index]
                         + WVelocity[Get3DIndex(i,j,k + 1)] * BoxLength[index]
                       ) / BoxLength[index];
           }
           else
           {
               if (k == UpperLayer) //Bottom layer and intermediate layers
                   UVelocity[indexU] = (
                                         - WVelocity[index] * BoxLength[index] +
                                         UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / 2.0 +
                                         (DynamicHeight[index] - PastHeight[index]) * BoxLength[index] / ATimeStep
                                       ) / BoxDepth[index];

               else
                   UVelocity[indexU] = (
                                         (WVelocity[Get3DIndex(i,j,k + 1)] - WVelocity[index]) *
                                         BoxLength[index] +
                                         UVelocity[Get3DIndexForUVelocity(i,j + 1,k)] * (BoxDepth[Get3DIndex(i,j + 1,k)] + BoxDepth[index]) / 2.0
                                       ) / BoxDepth[index];

           }
        }
      //} //k
   } //Boundary
}
//----------------------------------------------------------------------------------------------------------------------------------------------------
bool TTriDimensional::CheckForSeaBoundary(int ALine, int AColumn, int ALayer)
{
  int i, j, k;
  i = ALine; j = AColumn; k = ALayer;
  if
    (
      (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) ||
      (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) ||
      (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) ||
      (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2)
    )
      return true;
  else
      return false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
bool TTriDimensional::CheckForSeaBoundariesInColumns(int ALine, int AColumn)
{
   int i, j;
   bool SeaBoundary = false;
   i = ALine; j = AColumn;
   for (int k = 0; k <= UpperLayer; k++)
   {
      if
        (
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) ||
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) ||
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) ||
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2)
        )
        {
           SeaBoundary = true;
           break;
        }
   }
      return SeaBoundary;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
bool TTriDimensional::CheckForRiverBoundariesInColumns(int ALine, int AColumn)
{
   int i, j;
   bool RiverBoundary = false;
   i = ALine; j = AColumn;
   for (int k = 0; k <= UpperLayer; k++)
   {
      if
        (
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1) ||
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1) ||
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
          (BoxArray[Get3DIndex(i,j,k)].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
        )
        {
           RiverBoundary = true;
           break;
        }
   }
      return RiverBoundary;
}

bool TTriDimensional::CheckForRivers(int ALine, int AColumn)
{
   for (int River = 0; River < NumberOfRivers; River++)
   {
      for (int ALayer = 0; ALayer <= UpperLayer; ALayer++) {
        if (
             (MyRiverRecord[River].LineCoordinate == ALine) &&
             (MyRiverRecord[River].ColumnCoordinate == AColumn) &&
             (MyRiverRecord[River].LayerCoordinate == ALayer)
           )
        return true;
        break;
      }
   }
   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
void TTriDimensional::RiverForcing()
{
   int NumberOfHoursForFlowTimeSeries;
   NumberOfDaysForFlowTimeSeries = MyPEcoDynClass->GetNumberOfDaysForFlowTimeSeries();
   NumberOfHoursForFlowTimeSeries = MyPEcoDynClass->GetNumberOfHoursForFlowTimeSeries();
   if ((MyRiverRecord != NULL) && ((NumberOfDaysForFlowTimeSeries > 0) || (NumberOfHoursForFlowTimeSeries) > 0))
   {
      double TotalDepth, FlowGradient, TimeSpan1, TimeSpan2;
      int ADay, AHour, AMoment, ANextMoment;
      int LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
      if  (NumberOfDaysForFlowTimeSeries >= 1)
      {
         ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForFlowTimeSeries - 1);
         AMoment = ADay;
         ANextMoment = MIN(MyPEcoDynClass->GetJulianDay(), NumberOfDaysForFlowTimeSeries - 1);
         TimeSpan1 = DAYSTOSECONDS;
         TimeSpan2 = 0.0;
      }
      else
      if (NumberOfHoursForFlowTimeSeries >= 1)
      {
         ADay = MyPEcoDynClass->GetJulianDay() - 1;
         AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
         AMoment = MIN(AHour, NumberOfHoursForFlowTimeSeries - 1);
         ANextMoment = MIN(AHour + 1, NumberOfHoursForFlowTimeSeries - 1);
         TimeSpan1 = HOURSTOSECONDS;
         TimeSpan2 = MyPEcoDynClass->GetCurrTime() * HOURSTOSECONDS - MAX(0.0, MyPEcoDynClass->GetCurrTime() * HOURSTOSECONDS - ATimeStep) ;
      }
      else
#ifdef __BORLANDC__
         MessageBox(0, "TTriDimensional::RiverForcing", "Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForFlowTimeSeries", MB_OK);
#else  // __BORLANDC__
         cerr << "TTriDimensional::RiverForcing - incorrect value in NumberOfDaysForFlowTimeSeries or NumberofHoursForFlowTimeSeries" << endl;
#endif  // __BORLANDC__
      for (int River = 0; River < NumberOfRivers; River++)
      {
        LineIndex = MyRiverRecord[River].LineCoordinate;
        ColumnIndex = MyRiverRecord[River].ColumnCoordinate;
        //Debugger(LineIndex); Debugger(ColumnIndex);
        TotalDepth = 0.0;
        /*for (int layer = 0; layer < UpperLayer; layer++)
        {
           if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
              BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex + 1,ColumnIndex,layer)];
           if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
              BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex - 1,ColumnIndex,layer)];
           if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns - 1))
              BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex + 1,layer)];
           if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
              BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex - 1,layer)];
           if (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] > 0.0)
              TotalDepth = TotalDepth + BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)];
        } */
        /*if (TimeSpan1 > 0.0)
            FlowGradient = (MyRiverRecord[River].RiverFlow[ANextMoment] - MyRiverRecord[River].RiverFlow[AMoment])*TimeSpan2/TimeSpan1;
        else  */
            FlowGradient = 0.0;

        if (MyRiverRecord[River].HasLayers)
        {
           int LayerIndex;
           LayerIndex = MIN(MyRiverRecord[River].LayerCoordinate, UpperLayer);
           if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
          {
              if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
              {
                 BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = BoxDepth[Get3DIndex(LineIndex + 1,ColumnIndex,LayerIndex)];
                 DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = DynamicHeight[Get3DIndex(LineIndex + 1,ColumnIndex,LayerIndex)];
                 if (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] > CriticalDepthForLandBoundary)
                    VVelocity[Get3DIndexForVVelocity(LineIndex + 1,ColumnIndex,LayerIndex)] =
                                          (MyRiverRecord[River].RiverFlow[AMoment] + FlowGradient) /
                                          (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] * BoxLength[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)]);
                 else
                    VVelocity[Get3DIndexForVVelocity(LineIndex + 1,ColumnIndex,LayerIndex)] = 0.0;
                 //Northern river boundary
              }
              else
              if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex == GridLines - 1))
#ifdef __BORLANDC__
                 MessageBox(0,
                        "Hydrodynamic tridimensional object - RiverForcing",
                        "River flow to the North located at northern grid limit",
                        MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                 cerr << "TTriDimensional::RiverForcing - River flow to the North located at northern grid limit" << endl;
#endif  // __BORLANDC__

              if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
              {
                 BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = BoxDepth[Get3DIndex(LineIndex - 1,ColumnIndex,LayerIndex)];
                 DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = DynamicHeight[Get3DIndex(LineIndex - 1,ColumnIndex,LayerIndex)];
                 if (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] > CriticalDepthForLandBoundary)
                    VVelocity[Get3DIndexForVVelocity(LineIndex,ColumnIndex,LayerIndex)] =
                                          (MyRiverRecord[River].RiverFlow[AMoment]  + FlowGradient)/
                                          (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] * BoxLength[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)]);
                 else
                    VVelocity[Get3DIndexForVVelocity(LineIndex,ColumnIndex,LayerIndex)] = 0.0;
                //Southern river boundary
              }
              else
              if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex == 0))
#ifdef __BORLANDC__
                 MessageBox(0,
                        "Hydrodynamic tridimensional object - RiverForcing",
                        "River flow to the South located at southern grid limit",
                        MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                 cerr << "TTriDimensional::RiverForcing - River flow to the South located at southern grid limit" << endl;
#endif  // __BORLANDC__
          }
          else
          if ((MyRiverRecord[River].BoundaryNumber == 2) || (MyRiverRecord[River].BoundaryNumber == 4))
          {
              if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns - 1))
              {
                 BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex + 1,LayerIndex)];
                 DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = DynamicHeight[Get3DIndex(LineIndex,ColumnIndex + 1,LayerIndex)];
                 if (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] > CriticalDepthForLandBoundary)
                    UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex + 1,LayerIndex)] =
                                         (MyRiverRecord[River].RiverFlow[AMoment]  + FlowGradient) /
                                         (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] * BoxWidth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)]);
                 else
                    UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex + 1,LayerIndex)] = 0.0;
                 //Western river boundary
              }
              else
              if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex == GridColumns - 1))
#ifdef __BORLANDC__
                 MessageBox(0,
                        "Hydrodynamic tridimensional object - RiverForcing",
                        "River flow to the east located at eastern grid limit",
                        MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                 cerr << "TTriDimensional::RiverForcing - River flow to the East located at eastern grid limit" << endl;
#endif  // __BORLANDC__

              if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
              {
                 BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex - 1,LayerIndex)];
                 DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] = DynamicHeight[Get3DIndex(LineIndex,ColumnIndex - 1,LayerIndex)];
                 if (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] > CriticalDepthForLandBoundary)
                   UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex,LayerIndex)] =
                                          (MyRiverRecord[River].RiverFlow[AMoment]  +  FlowGradient) /
                                          (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)] * BoxWidth[Get3DIndex(LineIndex,ColumnIndex,LayerIndex)]);
                 else
                   UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex,LayerIndex)] = 0.0;
                //Eastern river boundary
              }
              else
              if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex == 0))
#ifdef __BORLANDC__
                 MessageBox(0,
                        "Hydrodynamic tridimensional object - RiverForcing",
                        "River flow to the West located at western grid limit",
                        MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                 cerr << "TTriDimensional::RiverForcing - River flow to the West located at western grid limit" << endl;
#endif  // __BORLANDC__
          }
        }
        else
        {
          for (int layer = 0; layer <= UpperLayer; layer++)
          {
            if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
            {
                if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
                {
                   BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex + 1,ColumnIndex,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex + 1,ColumnIndex,layer)];
                }
                if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
                {
                   BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex - 1,ColumnIndex,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex - 1,ColumnIndex,layer)];
                }
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 2) || (MyRiverRecord[River].BoundaryNumber == 4))
            {
                if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns - 1))
                {
                   BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex + 1,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex,ColumnIndex + 1,layer)];
                }
                if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
                {
                   BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex - 1,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex,ColumnIndex - 1,layer)];
                }
            }
          }

          for (int layer = 0; layer <= UpperLayer; layer++)
          {
            if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
            {
                if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
                {
                   /*BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex + 1,ColumnIndex,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex + 1,ColumnIndex,layer)];  */
                   if ((Depth(LineIndex,ColumnIndex,0) > CriticalDepthForLandBoundary)  &&
                       (BoxDepth[Get3DIndex(LineIndex ,ColumnIndex,layer)] > CriticalDepthForLandBoundary))
                      VVelocity[Get3DIndexForVVelocity(LineIndex + 1,ColumnIndex,layer)] =
                                            (MyRiverRecord[River].RiverFlow[AMoment] + FlowGradient) /
                                            (Depth(LineIndex,ColumnIndex,0) * BoxLength[Get3DIndex(LineIndex,ColumnIndex,layer)]);  //Northern river boundary
                   else
                      VVelocity[Get3DIndexForVVelocity(LineIndex + 1,ColumnIndex,layer)] = 0.0;
                }
                else
                if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex == GridLines - 1))
#ifdef __BORLANDC__
                   MessageBox(0,
                          "Hydrodynamic tridimensional object - RiverForcing",
                          "River flow to the North located at northern grid limit",
                          MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                    cerr << "TTriDimensional::RiverForcing - River flow to the North located at northern grid limit" << endl;
#endif  // __BORLANDC__

                if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
                {
                   /*BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex - 1,ColumnIndex,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex - 1,ColumnIndex,layer)]; */
                   if ((Depth(LineIndex,ColumnIndex,0) > CriticalDepthForLandBoundary) &&
                       (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] > CriticalDepthForLandBoundary))
                      VVelocity[Get3DIndexForVVelocity(LineIndex,ColumnIndex,layer)] =
                                            (MyRiverRecord[River].RiverFlow[AMoment]  + FlowGradient)/
                                            (Depth(LineIndex,ColumnIndex,0) * BoxLength[Get3DIndex(LineIndex,ColumnIndex,layer)]);  //Southern river boundary
                   else
                      VVelocity[Get3DIndexForVVelocity(LineIndex,ColumnIndex,layer)] = 0.0;
                }
                else
                if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex == 0))
#ifdef __BORLANDC__
                   MessageBox(0,
                          "Hydrodynamic tridimensional object - RiverForcing",
                          "River flow to the South located at southern grid limit",
                          MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                    cerr << "TTriDimensional::RiverForcing - River flow to the South located at southern grid limit" << endl;
#endif  // __BORLANDC__
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 2) || (MyRiverRecord[River].BoundaryNumber == 4))
            {
                if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns - 1))
                {
                   /*BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex + 1,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex,ColumnIndex + 1,layer)]; */
                   if ((Depth(LineIndex,ColumnIndex,0) > CriticalDepthForLandBoundary) &&
                       (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] > CriticalDepthForLandBoundary))
                      UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex + 1,layer)] =
                                           (MyRiverRecord[River].RiverFlow[AMoment]  + FlowGradient) /
                                           (Depth(LineIndex,ColumnIndex,0) * BoxWidth[Get3DIndex(LineIndex,ColumnIndex,layer)]);  //Western river boundary
                   else
                      UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex + 1,layer)] = 0.0;
                }
                else
                if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex == GridColumns - 1))
#ifdef __BORLANDC__
                   MessageBox(0,
                          "Hydrodynamic tridimensional object - RiverForcing",
                          "River flow to the east located at eastern grid limit",
                          MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                    cerr << "TTriDimensional::RiverForcing - River flow to the East located at eastern grid limit" << endl;
#endif  // __BORLANDC__

                if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
                {
                   /*BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] = BoxDepth[Get3DIndex(LineIndex,ColumnIndex - 1,layer)];
                   DynamicHeight[Get3DIndex(LineIndex,ColumnIndex,layer)] = DynamicHeight[Get3DIndex(LineIndex,ColumnIndex - 1,layer)];*/
                   if ((Depth(LineIndex,ColumnIndex,0) > CriticalDepthForLandBoundary) &&
                       (BoxDepth[Get3DIndex(LineIndex,ColumnIndex,layer)] > CriticalDepthForLandBoundary))
                      UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex,layer)] =
                                            (MyRiverRecord[River].RiverFlow[AMoment]  +  FlowGradient) /
                                            (Depth(LineIndex,ColumnIndex,0) * BoxWidth[Get3DIndex(LineIndex,ColumnIndex,layer)]);  //Eastern river boundary
                   else
                      UVelocity[Get3DIndexForUVelocity(LineIndex,ColumnIndex,layer)] = 0.0;
                }
                else
                if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex == 0))
#ifdef __BORLANDC__
                   MessageBox(0,
                          "Hydrodynamic tridimensional object - RiverForcing",
                          "River flow to the West located at western grid limit",
                          MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
                    cerr << "TTriDimensional::RiverForcing - River flow to the West located at western grid limit" << endl;
#endif  // __BORLANDC__
            }
          }
        }
      }
   }
}

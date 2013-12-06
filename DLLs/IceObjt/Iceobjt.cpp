// Sea Ice temperature object CPP code file
// @ Pedro Duarte

// Rev. 05.07.2013
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "iceobjt.h"
#include "iodll.h"

// TIce Class
//
// Constructors invoked outside EcoDyn shell...
//

TIce::TIce(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    PreBuildIce(className);
    NumberOfVariables = nVars;
    VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);
}

TIce::TIce(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    strcpy(VarsFileName, variablesFilename);
    if (OpenMorphology()) {
        BuildIce(className);
    }
}
// ...

TIce::TIce(TEcoDynClass* APEcodynClass, char* className): TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcodynClass;
    bool AIceRecord;
    KforMeltingMYIce = 0.794;
    KforMeltingFYIce = 0.93;
    KforPondedMYIce = 0.645;
    I0forMeltingMYIce = 0.93;
    I0forMeltingFYIce = 0.97;
    I0forPondedMYIce = 0.99;
    /*K_Ice = 3.015126667;
    I0_Ice = 0.603733333;
    Albedo = 0.4564;*/
    KforVisibleLightInIce = 0.789666667;
    KforNearInfraRedLightInIce = 4.626666667;
    I0forVisibleLightInIce = 0.963333333;
    I0forNearInfraRedLightInIce = 0.343333333;
    AlbedoForVisibleLightInIce = 0.582666667;
    AlbedoForNearInfraredLightInIce = 0.365;
    FreezingPointOfSeawater = -1.86; //Arrigo et al. (1993) - Journal of Geophysical Research 98: 6929-6946
    fs = 0.5; //Arrigo et al. (1993) - Journal of Geophysical Research 98: 6929-6946 (Fb, page 6932)
    KforVisibleLightInSnow = 0.0;
    AlbedoForVisibleLightInSnow = 0.0;
    DiffusionCoeff = 0.00001; //m/s2 //Jin et al. (2006) Annals of Glaciology 44
    Iteration = 0;
    UpperLayer = 0;
    SeaBoxDepth = 0.0;
    AIceRecord = MyPEcoDynClass->HasAIceRecord();
    if (!AIceRecord)
    {
       if (MyPEcoDynClass->OpenIce(MyPEcoDynClass->GetIceFile()))
       AIceRecord = true;
       //Debugger(1);
    }
    if (AIceRecord)
    {
       MyIceRecord = MyPEcoDynClass->GetAIceRecord();
       NumberOfBoxesWithIce = MyPEcoDynClass->GetNumberOfCellsWithIce();
       NumberOfCongelationLayers = MyIceRecord->CongelationIceLayers;
       NumberOfSkeletalLayers = MyIceRecord->SkeletalIceLayers;
       NumberOfPlateletLayers = MyIceRecord->PlateletIceLayers;
       NumberOfIceLayers = NumberOfCongelationLayers + NumberOfSkeletalLayers;
       //Debugger(2);
    }
    else
    {
#ifdef __BORLANDC__
        MessageBox(0, "Ice record missing in TIce.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TIce::TIce - Ice record missing in TIce" << endl;
#endif  // __BORLANDC__
    }
    PreBuildIce(className);
    BuildIce(className);
}

void TIce::PreBuildIce(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    // Get array size
    //NumberOfBoxesWithIce = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    //NumberOfBoxesWithIce = NumberOfLines * NumberOfColumns * NumberOfIceLayers;
    VariableCount = 0;
    TimeStep = MyPEcoDynClass->GetTimeStep();
    TimeBefore = MyPEcoDynClass->GetTimeBefore();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    ObjectCode = ICEOBJECTCODE;
    // Initialise air temperature
    SIarea = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    SIheff = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    SIhsnow = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    PastSIheff = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDensity = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkSalinity = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkTemperature = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineVolume = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineFractionalVolume = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkNO3 = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkNO2 = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkNH4 = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkPhosphate = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkSilicate = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkOxygen = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDOC = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDON = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDOP = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NO3Diffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NO2Diffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NH4Diffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    PhosphateDiffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    SilicateDiffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    OxygenDiffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DOCDiffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DONDiffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DOPDiffusion = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NO3BrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NH4BrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NO2BrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    PhosphateBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    SilicateBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    OxygenBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DOCBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DONBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DOPBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NO3BulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NO2BulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    NH4BulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    PhosphateBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    SilicateBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    OxygenBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DOCBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DONBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DOPBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    IceGrowthRate = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    SkeletalLayerDepth = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    LayerDepth = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineDetritalIceC = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineDetritalIceN = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineDetritalIceP = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineDetritalIceSi = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDetritalIceC = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDetritalIceN = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDetritalIceP = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkDetritalIceSi = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DetritalIceCBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); DetritalIceNBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DetritalIcePBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); DetritalIceSiBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DetritalIceCBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); DetritalIceNBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    DetritalIcePBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); DetritalIceSiBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    TPMIceBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); POMIceBulkFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    TPMIceBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); POMIceBrineFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineTPM = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrinePOM = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BulkTPM = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BulkPOM = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineDensity = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrineSalinity = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineTemperature = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); 
    BrineNO3 = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrineNO2 = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrineNH4 = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrinePhosphate = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrineSilicate = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineOxygen = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrineDOC = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    BrineDON = (double*) calloc(NumberOfBoxesWithIce, sizeof(double)); BrineDOP = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
    for (int i = 0; i < NumberOfBoxesWithIce; i++)
       BrineFractionalVolume[i] = fs; //Attention: This is good only for one layer models where only the skeletal layer is simulated
}

void TIce::BuildIce(char* className)
{
    int X,Y;
    // Read in the variables
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Ice");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "Ice variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TIce::BuildIce - Ice variables file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        if (PReadWrite->FindString("Sea ice", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
                PReadWrite->ReadString(X+2+i, Y, VariableNameArray[i-Y]);
            //MessageBox(0, VariableNameArray[1], "EcoDynamo alert", MB_OK);
            char MyVariable[65];
            for (int i = 0; i < NumberOfVariables; i++)
            {
               PReadWrite->ReadString(X+2+i, Y, MyVariable);
               if (strcmp(MyVariable, "BulkDensityIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkDensity[j]);
                       //Debugger(BulkDensity[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkSalinityIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkSalinity[j]);
                       //Debugger(BulkSalinity[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkNO3Ice") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkNO3[j]);
                       //Debugger(BulkNO3[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkNO2Ice") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkNO2[j]);
                       //Debugger(BulkNO2[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkNH4Ice") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkNH4[j]);
                       //Debugger(BulkNH4[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkPhosphateIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkPhosphate[j]);
                       //Debugger(BulkPhosphate[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkSilicateIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkSilicate[j]);
                       //Debugger(BulkSilicate[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkDOCIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkDOC[j]);
                       //Debugger(BulkDOC[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkDONIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkDON[j]);
                       //Debugger(BulkDON[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkDOPIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkDOP[j]);
                       //Debugger(BulkDOP[j]);
                    }
               }
               if (strcmp(MyVariable, "BulkOxygenIce") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BulkOxygen[j]);
                       //Debugger(BulkOxygen[j]);
                    }
               }
               if (strcmp(MyVariable, "BrineDetritalIceC") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BrineDetritalIceC[j]);
                       //Debugger(BrineDetritalIceC[j]);
                    }
               }
               if (strcmp(MyVariable, "BrineDetritalIceN") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BrineDetritalIceN[j]);
                       //Debugger(BrineDetritalIceN[j]);
                    }
               }
               if (strcmp(MyVariable, "BrineDetritalIceP") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BrineDetritalIceP[j]);
                       //Debugger(BrineDetritalIceP[j]);
                    }
               }
               if (strcmp(MyVariable, "BrineDetritalIceSi") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, BrineDetritalIceSi[j]);
                       //Debugger(BrineDetritalIceSi[j]);
                    }
               }
               if (strcmp(MyVariable, "LayerThickness") == 0)
               {
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                    {
                       PReadWrite->ReadNumber(X+2+i, Y+1+j, LayerThickness[j]);
                       //Debugger(LayerThickness[j]);
                    }
               }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
    PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Ice");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
            MessageBox(0, "Ice parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TIce::BuildIce - Ice parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        if (PReadWrite->FindString("Sea ice", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
                if (strcmp(MyParameter, "KforMeltingMYIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KforMeltingMYIce);
                    //Debugger(KforMeltingMYIce);
                }
                if (strcmp(MyParameter, "KforMeltingFYIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KforMeltingFYIce);
                    //Debugger(KforMeltingFYIce);
                }
                if (strcmp(MyParameter, "KforPondedMYIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KforPondedMYIce);
                    //Debugger(KforPondedMYIce);
                }
                if (strcmp(MyParameter, "I0forMeltingMYIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, I0forMeltingMYIce);
                    //Debugger(I0forMeltingMYIce);
                }
                if (strcmp(MyParameter, "I0forMeltingFYIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, I0forMeltingFYIce);
                    //Debugger(I0forMeltingFYIce);
                }
                if (strcmp(MyParameter, "I0forPondedMYIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, I0forPondedMYIce);
                    //Debugger(I0forPondedMYIce);
                }
                /*if (strcmp(MyParameter, "K_Ice") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, K_Ice);
                    //Debugger(K_Ice);
                }
                if (strcmp(MyParameter, "I0_Ice") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, I0_Ice);
                    //Debugger(I0_Ice);
                }
                if (strcmp(MyParameter, "Albedo") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Albedo);
                    //Debugger(Albedo);
                } */
                if (strcmp(MyParameter, "KforVisibleLightInIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KforVisibleLightInIce);
                    //Debugger(KforVisibleLightInIce);
                }
                if (strcmp(MyParameter, "KforNearInfraRedLightInIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KforNearInfraRedLightInIce);
                    //Debugger(KforNearInfraRedLightInIce);
                }
                if (strcmp(MyParameter, "I0forVisibleLightInIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, I0forVisibleLightInIce);
                    //Debugger(I0forVisibleLightInIce);
                }
                if (strcmp(MyParameter, "I0forNearInfraRedLightInIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, I0forNearInfraRedLightInIce);
                    //Debugger(I0forNearInfraRedLightInIce);
                }
                if (strcmp(MyParameter, "AlbedoForVisibleLightInIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AlbedoForVisibleLightInIce);
                    //Debugger(AlbedoForVisibleLightInIce);
                }
                if (strcmp(MyParameter, "AlbedoForNearInfraredLightInIce") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AlbedoForNearInfraredLightInIce);
                    //Debugger(AlbedoForNearInfraredLightInIce);
                }
                if (strcmp(MyParameter, "KforVisibleLightInSnow") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KforVisibleLightInSnow);
                    ///Debugger(KforVisibleLightInSnow);
                }
                if (strcmp(MyParameter, "AlbedoForVisibleLightInSnow") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AlbedoForVisibleLightInSnow);
                    //Debugger(AlbedoForVisibleLightInSnow);
                }
                if (strcmp(MyParameter, "SkeletalLayerDepth") == 0)
                {
                    double AValue;
                    PReadWrite->ReadNumber(X+3, i, AValue);
                    for (int j = 0; j < NumberOfBoxesWithIce; j++)
                       SkeletalLayerDepth[j] = AValue;
                    //Debugger(AValue);
                }
                if (strcmp(MyParameter, "DiffusionCoeff") == 0)
                {
                    PReadWrite->ReadNumber(X+4, i, DiffusionCoeff);
                    //Debugger(DiffusionCoeff);
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
    //MessageBox(0, VariableNameArray[1], "EcoDynamo alert", MB_OK);
}

TIce::~TIce()
{
    freeMemory();
}

void TIce::freeMemory()
{
   try
   {
      if (NumberOfBoxesWithIce > 0) {
          free(SIarea);
          free(SIheff);
          free(SIhsnow);
          free(PastSIheff);
          free(BulkDensity);
          free(BulkSalinity);
          free(BulkTemperature);
          free(BrineVolume);
          free(BrineFractionalVolume);
          free(BulkNO3);
          free(BulkNO2);
          free(BulkNH4);
          free(BulkPhosphate);
          free(BulkSilicate);
          free(BulkOxygen);
          free(BulkDOC);
          free(BulkDON);
          free(BulkDOP);
          free(NO3Diffusion);
          free(NO2Diffusion);
          free(NH4Diffusion);
          free(PhosphateDiffusion);
          free(SilicateDiffusion);
          free(OxygenDiffusion);
          free(DOCDiffusion);
          free(DONDiffusion);
          free(DOPDiffusion);
          free(NO3BrineFlux);
          free(NO2BrineFlux);
          free(NH4BrineFlux);
          free(PhosphateBrineFlux);
          free(SilicateBrineFlux);
          free(OxygenBrineFlux);
          free(DOCBrineFlux);
          free(DONBrineFlux);
          free(DOPBrineFlux);
          free(BrineFlux);
          free(NO3BulkFlux);
          free(NO2BulkFlux);
          free(NH4BulkFlux);
          free(PhosphateBulkFlux);
          free(SilicateBulkFlux);
          free(OxygenBulkFlux);
          free(DOCBulkFlux);
          free(DONBulkFlux);
          free(DOPBulkFlux);
          free(IceGrowthRate);
          free(LayerDepth);

          free(BrineDetritalIceC);
          free(BrineDetritalIceN);
          free(BrineDetritalIceP);
          free(BrineDetritalIceSi);

          free(BulkDetritalIceC);
          free(BulkDetritalIceN);
          free(BulkDetritalIceP);
          free(BulkDetritalIceSi);

          free(DetritalIceCBrineFlux);
          free(DetritalIceNBrineFlux);
          free(DetritalIcePBrineFlux);
          free(DetritalIceSiBrineFlux);
          free(DetritalIceCBulkFlux);
          free(DetritalIceNBulkFlux);
          free(DetritalIcePBulkFlux);
          free(DetritalIceSiBulkFlux);
          free(DetritalIceCBrineFlux);
          free(DetritalIceNBrineFlux);
          free(DetritalIcePBrineFlux);
          free(DetritalIceSiBrineFlux);
          free(DetritalIceCBulkFlux);
          free(DetritalIceNBulkFlux);
          free(DetritalIcePBulkFlux);
          free(DetritalIceSiBulkFlux);
          free(BrineDensity);
          free(BrineSalinity);
          free(BrineTemperature);
          free(BrineNO3);
          free(BrineNO2);
          free(BrineNH4);
          free(BrinePhosphate);
          free(BrineSilicate);
          free(BrineOxygen);
          free(BrineDOC);
          free(BrineDON);
          free(BrineDOP);
          free(SkeletalLayerDepth);
          free(TPMIceBulkFlux);
          free(POMIceBulkFlux);
          free(TPMIceBrineFlux);
          free(POMIceBrineFlux);
          free(BrineTPM);
          free(BrinePOM);
          free(BulkTPM);
          free(BulkPOM);
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

VNA* TIce::InquiryIce(char* srcName,int &IceLayers, int &IceVariables)
{
    VNA* MyVarNameArray;
    IceLayers = NumberOfIceLayers;
    IceVariables = NumberOfVariables;
    MyVarNameArray = new VNA[IceVariables];
    for (int i = 0; i < IceVariables; i++)
    {
        strcpy(MyVarNameArray[i], VariableNameArray[i]);
    }
    LogMessage("Inquiry", srcName, MyVarNameArray[0], IceLayers, IceVariables);
    return MyVarNameArray;
}

void TIce::Go()
{
   // Active methods

}

double TIce::DepthOfSurfaceLayer(int ALine, int AColumn)
{
   int MyLine, MyColumn;
   double MyDepth, AValue;
   MyLine = ALine; MyColumn = AColumn;
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyTransportPointer != NULL)
   {
      /*if (NumberOfIceLayers > 0)
      { */
         MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
         UpperLayer = ceil(AValue);
      /*}
      else
      {
         UpperLayer = 0;
      } */
      Index = Get3DIndex(MyLine,MyColumn,UpperLayer);
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth, Index,"Box depth",ObjectCode);
   }
   else
   {
      Index = Get3DIndex(MyLine,MyColumn,MyPEcoDynClass->GetNumberOfLayers()-1);
      MyDepth = MyPEcoDynClass->GetBoxDepth(Index);
   }
   SeaBoxDepth = MyDepth;
   return SeaBoxDepth;
}

void TIce::Integrate()
{
   for (int i = 0; i < NumberOfBoxesWithIce; i++)
   {
      NO3BulkFlux[i] = NO3BrineFlux[i] * fs;
      NO2BulkFlux[i] = NO2BrineFlux[i] * fs;
      NH4BulkFlux[i] = NH4BrineFlux[i] * fs;
      PhosphateBulkFlux[i] = PhosphateBrineFlux[i] * fs;
      SilicateBulkFlux[i] = SilicateBrineFlux[i] * fs;
      OxygenBulkFlux[i] = OxygenBrineFlux[i] * fs;
      DOCBulkFlux[i] = DOCBrineFlux[i] * fs;
      DONBulkFlux[i] = DONBrineFlux[i] * fs;
      DOPBulkFlux[i] = DOPBrineFlux[i] * fs;
      DetritalIceCBulkFlux[i] = DetritalIceCBrineFlux[i] * fs;
      DetritalIceNBulkFlux[i] = DetritalIceNBrineFlux[i] * fs;
      DetritalIcePBulkFlux[i] = DetritalIcePBrineFlux[i] * fs;
      DetritalIceSiBulkFlux[i] = DetritalIceSiBrineFlux[i] * fs;
   }

   Integration(BulkNO3, NO3BulkFlux, BulkNH4, NH4BulkFlux, BulkPhosphate, PhosphateBulkFlux, BulkSilicate , SilicateBulkFlux, BulkOxygen, OxygenBulkFlux);
   Integration(BulkNO2,NO2BulkFlux);
   Integration(BulkDOC,DOCBulkFlux);
   Integration(BulkDON,DONBulkFlux);
   Integration(BulkDOP,DOPBulkFlux);

   Integration(BulkDetritalIceC,DetritalIceCBulkFlux);
   Integration(BulkDetritalIceN,DetritalIceNBulkFlux);
   Integration(BulkDetritalIceP,DetritalIcePBulkFlux);
   Integration(BulkDetritalIceSi,DetritalIceSiBulkFlux);


   Integration(BrineNO3, NO3BrineFlux, BrineNH4, NH4BrineFlux, BrinePhosphate, PhosphateBrineFlux, BrineSilicate , SilicateBrineFlux, BrineOxygen, OxygenBrineFlux);
   Integration(BrineNO2,NO2BrineFlux);
   Integration(BrineDOC,DOCBrineFlux);
   Integration(BrineDON,DONBrineFlux);
   Integration(BrineDOP,DOPBrineFlux);

   Integration(BrineDetritalIceC,DetritalIceCBrineFlux);
   Integration(BrineDetritalIceN,DetritalIceNBrineFlux);
   Integration(BrineDetritalIceP,DetritalIcePBrineFlux);
   Integration(BrineDetritalIceSi,DetritalIceSiBrineFlux);

   Integration(BulkTPM,TPMIceBulkFlux);
   Integration(BulkPOM,POMIceBulkFlux);
   Integration(BrineTPM,TPMIceBrineFlux);
   Integration(BrinePOM,POMIceBrineFlux);

   for (int i = 0; i < NumberOfBoxesWithIce; i++)
   {
        if (BulkNO3[i] < 0.0) BulkNO3[i] = 0.0;
        if (BulkNO2[i] < 0.0) BulkNO2[i] = 0.0;
        if (BulkNH4[i] < 0.0) BulkNH4[i] = 0.0;
        if (BulkPhosphate[i] < 0.0) BulkPhosphate[i] = 0.0;
        if (BulkSilicate[i] < 0.0) BulkSilicate[i] = 0.0;
        if (BulkOxygen[i] < 0.0) BulkOxygen[i] = 0.0;
        if (BulkDOC[i] < 0.0) BulkDOC[i] = 0.0;
        if (BulkDON[i] < 0.0) BulkDON[i] = 0.0;
        if (BulkDOP[i] < 0.0) BulkDOP[i] = 0.0;

        if (BulkDetritalIceC[i] < 0.0) BulkDetritalIceC[i] = 0.0;
        if (BulkDetritalIceN[i] < 0.0) BulkDetritalIceN[i] = 0.0;
        if (BulkDetritalIceP[i] < 0.0) BulkDetritalIceP[i] = 0.0;
        if (BulkDetritalIceSi[i] < 0.0) BulkDetritalIceSi[i] = 0.0;

        if (BrineNO3[i] < 0.0) BrineNO3[i] = 0.0;
        if (BrineNO2[i] < 0.0) BrineNO2[i] = 0.0;
        if (BrineNH4[i] < 0.0) BrineNH4[i] = 0.0;
        if (BrinePhosphate[i] < 0.0) BrinePhosphate[i] = 0.0;
        if (BrineSilicate[i] < 0.0) BrineSilicate[i] = 0.0;
        if (BrineOxygen[i] < 0.0) BrineOxygen[i] = 0.0;
        if (BrineDOC[i] < 0.0) BrineDOC[i] = 0.0;
        if (BrineDON[i] < 0.0) BrineDON[i] = 0.0;
        if (BrineDOP[i] < 0.0) BrineDOP[i] = 0.0;

        if (BrineDetritalIceC[i] < 0.0) BrineDetritalIceC[i] = 0.0;
        if (BrineDetritalIceN[i] < 0.0) BrineDetritalIceN[i] = 0.0;
        if (BrineDetritalIceP[i] < 0.0) BrineDetritalIceP[i] = 0.0;
        if (BrineDetritalIceSi[i] < 0.0) BrineDetritalIceSi[i] = 0.0;

        if (BulkTPM[i] < 0.0) BulkTPM[i] = 0.0;
        if (BulkPOM[i] < 0.0) BulkPOM[i] = 0.0;
        if (BrineTPM[i] < 0.0) BrineTPM[i] = 0.0;
        if (BrinePOM[i] < 0.0) BrinePOM[i] = 0.0;
   }
}

void TIce::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber, index;
   strcpy(MyParameter, ParameterName);
   if (strcmp(MyParameter, "SIarea") == 0)
        Value = SIarea[BoxNumber];
   else if (strcmp(MyParameter, "SIheff") == 0)
        Value = SIheff[BoxNumber];
   else if (strcmp(MyParameter, "SIhsnow") == 0)
        Value = SIhsnow[BoxNumber];
   else if (strcmp(MyParameter, "Brine flux") == 0)
        Value = BrineFlux[BoxNumber] * DAYSTOSECONDS;  // m/d
   else if (strcmp(MyParameter, "KforMeltingMYIce") == 0)
        Value = KforMeltingMYIce;
   else if (strcmp(MyParameter, "KforMeltingFYIce") == 0)
        Value = KforMeltingFYIce;
   else if (strcmp(MyParameter, "KforPondedMYIce") == 0)
        Value = KforPondedMYIce;
   else if (strcmp(MyParameter, "I0forMeltingMYIce") == 0)
        Value = I0forMeltingMYIce;
   else if (strcmp(MyParameter, "I0forMeltingFYIce") == 0)
        Value = I0forMeltingFYIce;
   else if (strcmp(MyParameter, "I0forPondedMYIce") == 0)
        Value = I0forPondedMYIce;
   else if (strcmp(MyParameter, "KforVisibleLightInIce") == 0)
        Value = KforVisibleLightInIce;
   else if (strcmp(MyParameter, "KforNearInfraRedLightInIce") == 0)
        Value = KforNearInfraRedLightInIce;
   else if (strcmp(MyParameter, "I0forVisibleLightInIce") == 0)
        Value = I0forVisibleLightInIce;
   else if (strcmp(MyParameter, "I0forNearInfraRedLightInIce") == 0)
        Value = I0forNearInfraRedLightInIce;
   else if (strcmp(MyParameter, "AlbedoForVisibleLightInIce") == 0)
        Value = AlbedoForVisibleLightInIce;
   else if (strcmp(MyParameter, "AlbedoForNearInfraredLightInIce") == 0)
        Value = AlbedoForNearInfraredLightInIce;
   else if (strcmp(MyParameter, "KforVisibleLightInSnow") == 0)
        Value = KforVisibleLightInSnow;
   else if (strcmp(MyParameter, "AlbedoForVisibleLightInSnow") == 0)
        Value = AlbedoForVisibleLightInSnow;
   else if (strcmp(MyParameter, "IceGrowthRate") == 0)
        Value = IceGrowthRate[BoxNumber]; //return value in m/s
   else if (strcmp(MyParameter, "BulkDensityIce") == 0)
        Value = BulkDensity[BoxNumber];
   else if (strcmp(MyParameter, "BulkSalinityIce") == 0)
        Value = BulkSalinity[BoxNumber];
   else if (strcmp(MyParameter, "BulkTemperatureIce") == 0)
        Value = BulkTemperature[BoxNumber];
   else if (strcmp(MyParameter, "BulkNO3Ice") == 0)
        Value = BulkNO3[BoxNumber];
   else if (strcmp(MyParameter, "BulkNO2Ice") == 0)
        Value = BulkNO2[BoxNumber];
   else if (strcmp(MyParameter, "BulkNH4Ice") == 0)
        Value = BulkNH4[BoxNumber];
   else if (strcmp(MyParameter, "BulkPhosphateIce") == 0)
        Value = BulkPhosphate[BoxNumber];
   else if (strcmp(MyParameter, "BulkSilicateIce") == 0)
        Value = BulkSilicate[BoxNumber];
   else if (strcmp(MyParameter, "BulkOxygenIce") == 0)
        Value = BulkOxygen[BoxNumber];
   else if (strcmp(MyParameter, "BulkDOCIce") == 0)
        Value = BulkDOC[BoxNumber];
   else if (strcmp(MyParameter, "BulkDONIce") == 0)
        Value = BulkDON[BoxNumber];
   else if (strcmp(MyParameter, "BulkDOPIce") == 0)
        Value = BulkDOP[BoxNumber];
   else if (strcmp(MyParameter, "NO3DiffusionIce") == 0)
        Value = NO3Diffusion[BoxNumber];
   else if (strcmp(MyParameter, "NO2DiffusionIce") == 0)
        Value = NO2Diffusion[BoxNumber];
   else if (strcmp(MyParameter, "NH4DiffusionIce") == 0)
        Value = NH4Diffusion[BoxNumber];
   else if (strcmp(MyParameter, "PhosphateDiffusionIce") == 0)
        Value = PhosphateDiffusion[BoxNumber];
   else if (strcmp(MyParameter, "SilicateDiffusionIce") == 0)
        Value = SilicateDiffusion[BoxNumber];
   else if (strcmp(MyParameter, "OxygenDiffusionIce") == 0)
        Value = OxygenDiffusion[BoxNumber];
   else if (strcmp(MyParameter, "DOCDiffusionIce") == 0)
        Value = DOCDiffusion[BoxNumber];
   else if (strcmp(MyParameter, "DONDiffusionIce") == 0)
        Value = DONDiffusion[BoxNumber];
   else if (strcmp(MyParameter, "DOPDiffusionIce") == 0)
        Value = DOPDiffusion[BoxNumber];
   else if (strcmp(MyParameter, "NO3BrineFluxIce") == 0)
        Value = NO3BrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "NO2BrineFluxIce") == 0)
        Value = NO2BrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "NH4BrineFluxIce") == 0)
        Value = NH4BrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "PhosphateBrineFluxIce") == 0)
        Value = PhosphateBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "SilicateBrineFluxIce") == 0)
        Value = SilicateBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "OxygenBrineFluxIce") == 0)
        Value = OxygenBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DOCBrineFluxIce") == 0)
        Value = DOCBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DONBrineFluxIce") == 0)
        Value = DONBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DOPBrineFluxIce") == 0)
        Value = DOPBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "NO3FluxIce") == 0)
        Value = NO3BulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "NO2FluxIce") == 0)
        Value = NO2BulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "NH4FluxIce") == 0)
        Value = NH4BulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "PhosphateFluxIce") == 0)
        Value = PhosphateBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "SilicateFluxIce") == 0)
        Value = SilicateBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "OxygenFluxIce") == 0)
        Value = OxygenBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "DOCFluxIce") == 0)
        Value = DOCBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "DONFluxIce") == 0)
        Value = DONBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "DOPFluxIce") == 0)
        Value = DOPBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "BrineDetritalIceC") == 0)
        Value = BrineDetritalIceC[BoxNumber];
   else if (strcmp(MyParameter, "BrineDetritalIceN") == 0)
        Value = BrineDetritalIceC[BoxNumber];
   else if (strcmp(MyParameter, "BrineDetritalIceP") == 0)
        Value = BrineDetritalIceP[BoxNumber];
   else if (strcmp(MyParameter, "BrineDetritalIceSi") == 0)
        Value = BrineDetritalIceSi[BoxNumber];
   else if (strcmp(MyParameter, "BulkDetritalIceC") == 0)
        Value = BulkDetritalIceC[BoxNumber];
   else if (strcmp(MyParameter, "BulkDetritalIceN") == 0)
        Value = BulkDetritalIceC[BoxNumber];
   else if (strcmp(MyParameter, "BulkDetritalIceP") == 0)
        Value = BulkDetritalIceP[BoxNumber];
   else if (strcmp(MyParameter, "BulkDetritalIceSi") == 0)
        Value = BulkDetritalIceSi[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIceCBrineFlux") == 0)
        Value = DetritalIceCBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIceNBrineFlux") == 0)
        Value = DetritalIceNBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIcePBrineFlux") == 0)
        Value = DetritalIcePBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIceSiBrineFlux") == 0)
        Value = DetritalIceSiBrineFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIceCBulkFlux") == 0)
        Value = DetritalIceCBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIceNBulkFlux") == 0)
        Value = DetritalIceNBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIcePBulkFlux") == 0)
        Value = DetritalIcePBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "DetritalIceSiBulkFlux") == 0)
        Value = DetritalIceSiBulkFlux[BoxNumber];
   else if (strcmp(MyParameter, "BrineDensityIce") == 0)
        Value = BrineDensity[BoxNumber];
   else if (strcmp(MyParameter, "BrineSalinityIce") == 0)
        Value = BrineSalinity[BoxNumber];
   else if (strcmp(MyParameter, "BrineTemperatureIce") == 0)
        Value = BrineTemperature[BoxNumber];
   else if (strcmp(MyParameter, "BrineVolume") == 0)
        Value = BrineVolume[BoxNumber];
   else if (strcmp(MyParameter, "BrineFractionalVolume") == 0)
        Value = BrineFractionalVolume[BoxNumber];
   else if (strcmp(MyParameter, "BrineNO3Ice") == 0)
        Value = BrineNO3[BoxNumber];
   else if (strcmp(MyParameter, "BrineNO2Ice") == 0)
        Value = BrineNO2[BoxNumber];
   else if (strcmp(MyParameter, "BrineNH4Ice") == 0)
        Value = BrineNH4[BoxNumber];
   else if (strcmp(MyParameter, "BrinePhosphateIce") == 0)
        Value = BrinePhosphate[BoxNumber];
   else if (strcmp(MyParameter, "BrineSilicateIce") == 0)
        Value = BrineSilicate[BoxNumber];
   else if (strcmp(MyParameter, "BrineOxygenIce") == 0)
        Value = BrineOxygen[BoxNumber];
   else if (strcmp(MyParameter, "BrineDOCIce") == 0)
        Value = BrineDOC[BoxNumber];
   else if (strcmp(MyParameter, "BrineDONIce") == 0)
        Value = BrineDON[BoxNumber];
   else if (strcmp(MyParameter, "BrineDOPIce") == 0)
        Value = BrineDOP[BoxNumber];
   else if (strcmp(MyParameter, "SkeletalLayerDepth") == 0)
        Value = SkeletalLayerDepth[BoxNumber];

   else if (strcmp(MyParameter, "BulkTPM") == 0)
        Value = BulkTPM[BoxNumber];
   else if (strcmp(MyParameter, "BulkPOM") == 0)
        Value = BulkPOM[BoxNumber];
   else if (strcmp(MyParameter, "BrineTPM") == 0)
        Value = BrineTPM[BoxNumber];
   else if (strcmp(MyParameter, "BrinePOM") == 0)
        Value = BrineTPM[BoxNumber];
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
          MessageBox(0,Caption,"EcoDynamo Alert - Inquiry",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
					cerr << "TIce::Inquiry - " << MyParameter << " does not exits in" << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
      }
   }
   LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


void TIce::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);
   if (strcmp(MyParameter, "BulkNO3Ice") == 0)
        Value = NO3BulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkNO2Ice") == 0)
        Value = NO2BulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkNH4Ice") == 0)
        Value = NH4BulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkPhosphateIce") == 0)
        Value = PhosphateBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkSilicateIce") == 0)
        Value = SilicateBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkOxygenIce") == 0)
        Value = OxygenBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkDOCIce") == 0)
        Value = DOCBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkDONIce") == 0)
        Value = DONBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkDOPIce") == 0)
        Value = DOPBulkFlux[BoxNumber] + Value;

   else if (strcmp(MyParameter, "BrineNO3Ice") == 0)
        Value = NO3BrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineNO2Ice") == 0)
        Value = NO2BrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineNH4Ice") == 0)
        Value = NH4BrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrinePhosphateIce") == 0)
        Value = PhosphateBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineSilicateIce") == 0)
        Value = SilicateBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineOxygenIce") == 0)
        Value = OxygenBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDOCIce") == 0)
        Value = DOCBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDONIce") == 0)
        Value = DONBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDOPIce") == 0)
        Value = DOPBrineFlux[BoxNumber] + Value;

   else if (strcmp(MyParameter, "BrineNO3Ice") == 0)
        Value = NO3BrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineNO2Ice") == 0)
        Value = NO2BrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineNH4Ice") == 0)
        Value = NH4BrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrinePhosphateIce") == 0)
        Value = PhosphateBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineSilicateIce") == 0)
        Value = SilicateBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineOxygenIce") == 0)
        Value = OxygenBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDOCIce") == 0)
        Value = DOCBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDONIce") == 0)
        Value = DONBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDOPIce") == 0)
        Value = DOPBrineFlux[BoxNumber] + Value;

   else if (strcmp(MyParameter, "BrineDetritalIceC") == 0)
        Value = DetritalIceCBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDetritalIceN") == 0)
        Value = DetritalIceNBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDetritalIceP") == 0)
        Value = DetritalIcePBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineDetritalIceSi") == 0)
        Value = DetritalIceSiBrineFlux[BoxNumber] + Value;

   else if (strcmp(MyParameter, "BulkDetritalIceC") == 0)
        Value = DetritalIceCBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkDetritalIceN") == 0)
        Value = DetritalIceNBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkDetritalIceP") == 0)
        Value = DetritalIcePBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkDetritalIceSi") == 0)
        Value = DetritalIceSiBulkFlux[BoxNumber] + Value;

   else if (strcmp(MyParameter, "BulkTPM") == 0)
        Value = TPMIceBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BulkPOM") == 0)
        Value = POMIceBulkFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrineTPM") == 0)
        Value = TPMIceBrineFlux[BoxNumber] + Value;
   else if (strcmp(MyParameter, "BrinePOM") == 0)
        Value = POMIceBrineFlux[BoxNumber] + Value;
   else
   {
#ifdef __BORLANDC__
    	static char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in this TIce");
    	MessageBox(0,Caption,"EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TIce::Update - " << MyParameter << " does not exist in this TIce" << endl;
#endif  // __BORLANDC__
   }
}

bool TIce::SetVariableValue(char* srcName, double Value,int BoxNumber,char* VariableName)
{
     bool rc = true;
     LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);
     if (strcmp(VariableName, "SIarea") == 0)
          SIarea[BoxNumber] = Value;
     else if (strcmp(VariableName, "SIheff") == 0)
          SIheff[BoxNumber] = Value;
     else if (strcmp(VariableName, "SIhsnow") == 0)
          SIhsnow[BoxNumber] = Value;
     else if (strcmp(VariableName, "IceGrowthRate") == 0)
          IceGrowthRate[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkDensityIce") == 0)
          BulkDensity[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkSalinityIce") == 0)
          BulkSalinity[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkTemperatureIce") == 0)
          BulkTemperature[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkNO3Ice") == 0)
          BulkNO3[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkNO2Ice") == 0)
          BulkNO2[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkNH4Ice") == 0)
          BulkNH4[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkPhosphateIce") == 0)
          BulkPhosphate[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkSilicateIce") == 0)
          BulkSilicate[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkOxygenIce") == 0)
          BulkOxygen[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkDOCIce") == 0)
          BulkDOC[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkDONIce") == 0)
          BulkDON[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkDOPIce") == 0)
          BulkDOP[BoxNumber] = Value;
     else if (strcmp(VariableName, "NO3DiffusionIce") == 0)
          NO3Diffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "NO2DiffusionIce") == 0)
          NO2Diffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "NH4DiffusionIce") == 0)
          NH4Diffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "PhosphateDiffusionIce") == 0)
          PhosphateDiffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "SilicateDiffusionIce") == 0)
          SilicateDiffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "OxygenDiffusionIce") == 0)
          OxygenDiffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "DOCDiffusionIce") == 0)
          DOCDiffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "DONDiffusionIce") == 0)
          DONDiffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "DOPDiffusionIce") == 0)
          DOPDiffusion[BoxNumber] = Value;
     else if (strcmp(VariableName, "NO3BrineFluxIce") == 0)
          NO3BrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "NO2BrineFluxIce") == 0)
          NO2BrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "NH4BrineFluxIce") == 0)
          NH4BrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "PhosphateBrineFluxIce") == 0)
          PhosphateBrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "SilicateBrineFluxIce") == 0)
          SilicateBrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "OxygenBrineFluxIce") == 0)
          OxygenBrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "DOCBrineFluxIce") == 0)
          DOCBrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "DONBrineFluxIce") == 0)
          DONBrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "DOPBrineFluxIce") == 0)
          DOPBrineFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "NO3FluxIce") == 0)
          NO3BulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "NO2FluxIce") == 0)
          NO2BulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "NH4FluxIce") == 0)
          NH4BulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "PhosphateFluxIce") == 0)
          PhosphateBulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "SilicateFluxIce") == 0)
          SilicateBulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "OxygenFluxIce") == 0)
          OxygenBulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "DOCFluxIce") == 0)
          DOCBulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "DONFluxIce") == 0)
          DONBulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "DOPFluxIce") == 0)
          DOPBulkFlux[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDensityIce") == 0)
          BrineDensity[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineSalinityIce") == 0)
          BrineSalinity[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineTemperatureIce") == 0)
          BrineTemperature[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDetritalIceC") == 0)
          BrineDetritalIceC[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDetritalIceN") == 0)
          BrineDetritalIceC[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDetritalIceP") == 0)
          BrineDetritalIceP[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDetritalIceSi") == 0)
          BrineDetritalIceSi[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDensityIce") == 0)
          BulkDensity[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkSalinityIce") == 0)
          BulkSalinity[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkTemperatureIce") == 0)
          BulkTemperature[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineVolume") == 0)
          BrineVolume[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineNO3Ice") == 0)
          BrineNO3[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineNO2Ice") == 0)
          BrineNO2[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineNH4Ice") == 0)
          BrineNH4[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrinePhosphateIce") == 0)
          BrinePhosphate[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineSilicateIce") == 0)
          BrineSilicate[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineOxygenIce") == 0)
          BrineOxygen[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDOCIce") == 0)
          BrineDOC[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDONIce") == 0)
          BrineDON[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineDOPIce") == 0)
          BrineDOP[BoxNumber] = Value;
     else if (strcmp(VariableName, "SkeletalLayerDepth") == 0)
          SkeletalLayerDepth[BoxNumber] = Value;

     else if (strcmp(VariableName, "BulkTPM") == 0)
          BulkTPM[BoxNumber] = Value;
     else if (strcmp(VariableName, "BulkPOM") == 0)
          BulkPOM[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineTPM") == 0)
          BrineTPM[BoxNumber] = Value;
     else if (strcmp(VariableName, "BrineTPM") == 0)
          BrineTPM[BoxNumber] = Value;

     else
        rc = false;
     return rc;
}

bool TIce::SaveVariables()
{
    TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Ice");
    if (PReadWrite == NULL) return false;
    //header file
    PReadWrite->WriteCell("Sea ice");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();
    //save variables' names
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

// Protected methods

void TIce::GetIce()
{
}

double TIce::IceGrowth(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   return (SIheff[MyBoxNumber] - PastSIheff[MyBoxNumber]) / MyPEcoDynClass->GetTimeStep(); //m/s
}


void TIce::SkeletalDiffusion(int ABoxNumber, int Index, double ASeaBoxDepth)
{
   int MyBoxNumber, AValue, UpperLayer;
   double TINNY, AFactor;
   TINNY = 0.0000000001;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

   if ((MyNutrientPointer != NULL) && (SkeletalLayerDepth[MyBoxNumber] > TINNY))
   {
      double AmmoniaInWater, NitrateInWater, NitriteInWater, PhosphateInWater, SilicaInWater, OxygenInWater, DOCInWater, DONInWater, DOPInWater;
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), AmmoniaInWater,Index,"Ammonia",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), NitrateInWater,Index,"Nitrate",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), NitriteInWater,Index,"Nitrite",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), PhosphateInWater,Index,"Phosphate",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), SilicaInWater,Index,"Silica",ObjectCode);

      MyNutrientPointer->Inquiry(GetEcoDynClassName(), OxygenInWater,Index,"Oxygen",ObjectCode);

      MyNutrientPointer->Inquiry(GetEcoDynClassName(), DOCInWater,Index,"DOC",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), DONInWater,Index,"DON",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), DOPInWater,Index,"DOP",ObjectCode);
      //Dimensions: m2/s * microM / m2 = microM/s
      //Note - This multiplication by fs is to compute diffusion in a porous environment where the area available for diffusive exchanges
      //is reduced according to the fraction of channels
      AFactor = DiffusionCoeff * fs / pow(SkeletalLayerDepth[MyBoxNumber], 2.0);

      NH4Diffusion[MyBoxNumber] = AFactor * (AmmoniaInWater - BrineNH4[MyBoxNumber]);
      NO3Diffusion[MyBoxNumber] = AFactor * (NitrateInWater - BrineNO3[MyBoxNumber]);
      NO2Diffusion[MyBoxNumber] = AFactor * (NitriteInWater - BrineNO2[MyBoxNumber]) ;
      PhosphateDiffusion[MyBoxNumber] = AFactor * (PhosphateInWater - BrinePhosphate[MyBoxNumber]);
      SilicateDiffusion[MyBoxNumber] =  AFactor * (SilicaInWater - BrineSilicate[MyBoxNumber]);

      OxygenDiffusion[MyBoxNumber] = AFactor * (OxygenInWater - BrineOxygen[MyBoxNumber]);

      DOCDiffusion[MyBoxNumber] = AFactor * (DOCInWater - BrineDOC[MyBoxNumber]);
      DONDiffusion[MyBoxNumber] = AFactor * (DONInWater - BrineDON[MyBoxNumber]);
      DOPDiffusion[MyBoxNumber] = AFactor * (DOPInWater - BrineDOP[MyBoxNumber]);

      //Brine properties
      NH4BrineFlux[MyBoxNumber] = NH4BrineFlux[MyBoxNumber] + NH4Diffusion[MyBoxNumber];
      NO3BrineFlux[MyBoxNumber] =  NO3BrineFlux[MyBoxNumber] + NO3Diffusion[MyBoxNumber];
      NO2BrineFlux[MyBoxNumber] =  NO2BrineFlux[MyBoxNumber] + NO2Diffusion[MyBoxNumber];
      PhosphateBrineFlux[MyBoxNumber] = PhosphateBrineFlux[MyBoxNumber] + PhosphateDiffusion[MyBoxNumber];
      SilicateBrineFlux[MyBoxNumber] = SilicateBrineFlux[MyBoxNumber] + SilicateDiffusion[MyBoxNumber];

      OxygenBrineFlux[MyBoxNumber] = OxygenBrineFlux[MyBoxNumber] - OxygenDiffusion[MyBoxNumber];

      DOCBrineFlux[MyBoxNumber] = DOCBrineFlux[MyBoxNumber] + DOCDiffusion[MyBoxNumber];
      DONBrineFlux[MyBoxNumber] = DONBrineFlux[MyBoxNumber] + DONDiffusion[MyBoxNumber];
      DOPBrineFlux[MyBoxNumber] = DOPBrineFlux[MyBoxNumber] + DOPDiffusion[MyBoxNumber];

      //Bulk properties
      //Note - This multiplication by fs is to correct the effect of the flux in a larger volume, ie., the volume of ice + brine channel volume
      /*NH4BulkFlux[MyBoxNumber] = NH4BulkFlux[MyBoxNumber] + NH4Diffusion[MyBoxNumber] * fs;
      NO3BulkFlux[MyBoxNumber] = NO3BulkFlux[MyBoxNumber] + NO3Diffusion[MyBoxNumber] * fs;
      NO2BulkFlux[MyBoxNumber] = NO2BulkFlux[MyBoxNumber] + NO2Diffusion[MyBoxNumber] * fs;
      PhosphateBulkFlux[MyBoxNumber] = PhosphateBulkFlux[MyBoxNumber] + PhosphateDiffusion[MyBoxNumber] * fs;
      SilicateBulkFlux[MyBoxNumber] = SilicateBulkFlux[MyBoxNumber] + SilicateDiffusion[MyBoxNumber] * fs;

      OxygenBulkFlux[MyBoxNumber] = OxygenBulkFlux[MyBoxNumber] + OxygenDiffusion[MyBoxNumber] * fs;

      DOCBulkFlux[MyBoxNumber] = DOCBulkFlux[MyBoxNumber] + DOCDiffusion[MyBoxNumber] * fs;
      DONBulkFlux[MyBoxNumber] = DONBulkFlux[MyBoxNumber] + DONDiffusion[MyBoxNumber] * fs;
      DOPBulkFlux[MyBoxNumber] = DOPBulkFlux[MyBoxNumber] + DOPDiffusion[MyBoxNumber] * fs;
       */

      MyNutrientPointer->Update(GetEcoDynClassName(), -NH4Diffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Ammonia",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -NO3Diffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Nitrate",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -NO2Diffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Nitrite",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -PhosphateDiffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Phosphate",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -SilicateDiffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Silica",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -OxygenDiffusion[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Oxygen",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -DOCDiffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DOC",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -DONDiffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DON",ObjectCode);
      MyNutrientPointer->Update(GetEcoDynClassName(), -DOPDiffusion[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DOP",ObjectCode);

   }
}

void TIce::SkeletalBrineAndCongelationFluxes(int ABoxNumber, int Index, double ASeaBoxDepth) //This box number must correspond to cells of the first ice layer so, layer index must be zero
{
   int MyBoxNumber;
   double MyWaterTemperature, VolumeAreaCorrection, TINNY, AFactor1, AFactor2;
   TINNY = 0.000000001;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer(),
               * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
               * MySPMPointer = MyPEcoDynClass->GetSPMPointer();

   if (IceGrowthRate[MyBoxNumber] > 0.0)
   {
       //According with equation 5 of Arrigo et al (1993) - Journal of Geophysical Research 98: 6929-6946
       BrineFlux[MyBoxNumber] = MAX(0.0,(9.667e-9 + 4.49e-6 * IceGrowthRate[MyBoxNumber] * 100.0 -1.39e-7 * pow(IceGrowthRate[MyBoxNumber] * 100.0,2.0)) / 100.0);  //m/s
       //According with equation 6 of Arrigo et al (1993) - Journal of Geophysical Research 98: 6929-6946
       //fs - fraction of the skeletal layer which is open to convection
       //I am also assuming that the vertical exchange distance equals the depth of the skeletal layer
       AFactor1 = fs * BrineFlux[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / pow(SkeletalLayerDepth[MyBoxNumber],2.0);
       AFactor2 = IceGrowthRate[MyBoxNumber] * TimeStep / (SkeletalLayerDepth[MyBoxNumber] + IceGrowthRate[MyBoxNumber] * TimeStep);
       //Note - AFactor2 corresponds to the dillution of the ice properties when it grows from freezing sea water

       if (MyNutrientPointer != NULL)
       {

           double AmmoniaInWater, NitrateInWater, NitriteInWater, PhosphateInWater, SilicaInWater, OxygenInWater, DOCInWater, DONInWater, DOPInWater,
                  AmmoniaBrineFlux, NitrateBrineFlux, NitriteBrineFlux, PhosphorusBrineFlux, SilicaBrineFlux, OxyBrineFlux, DOCarbonBrineFlux, DONitrogenBrineFlux, DOPhosphorusBrineFlux,
                  AmmoniaCongelationFlux, NitrateCongelationFlux, NitriteCongelationFlux, PhosphateCongelationFlux,
                  SilicaCongelationFlux, OxygenCongelationFlux, DOCCongelationFlux, DONCongelationFlux, DOPCongelationFlux;
           /*****************************************************************************************************************************************/
           //Inputs from the nutrient pointer
           /*****************************************************************************************************************************************/
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), AmmoniaInWater,Index,"Ammonia",ObjectCode);
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), NitrateInWater,Index,"Nitrate",ObjectCode);
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), NitriteInWater,Index,"Nitrite",ObjectCode);
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), PhosphateInWater,Index,"Phosphate",ObjectCode);
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), SilicaInWater,Index,"Silica",ObjectCode);

           MyNutrientPointer->Inquiry(GetEcoDynClassName(), OxygenInWater,Index,"Oxygen",ObjectCode);

           MyNutrientPointer->Inquiry(GetEcoDynClassName(), DOCInWater,Index,"DOC",ObjectCode);
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), DONInWater,Index,"DON",ObjectCode);
           MyNutrientPointer->Inquiry(GetEcoDynClassName(), DOPInWater,Index,"DOP",ObjectCode);
           /*****************************************************************************************************************************************/
           //Calculation of brine fluxes of dissolved substances and storage of obtained values in local variables
           /*****************************************************************************************************************************************/
           AmmoniaBrineFlux = AFactor1 * (AmmoniaInWater - BrineNH4[MyBoxNumber]);
           NitrateBrineFlux = AFactor1 * (NitrateInWater - BrineNO3[MyBoxNumber]);  //micromol/m3/s
           NitriteBrineFlux = AFactor1 * (NitriteInWater - BrineNO2[MyBoxNumber]);
           PhosphorusBrineFlux = AFactor1 * (PhosphateInWater - BrinePhosphate[MyBoxNumber]);
           SilicaBrineFlux = AFactor1 * (SilicaInWater - BrineSilicate[MyBoxNumber]);

           OxyBrineFlux = AFactor1 * (OxygenInWater - BrineOxygen[MyBoxNumber]);

           DOCarbonBrineFlux = AFactor1 * (DOCInWater - BrineDOC[MyBoxNumber]);
           DONitrogenBrineFlux = AFactor1 * (DONInWater - BrineDON[MyBoxNumber]);
           DOPhosphorusBrineFlux = AFactor1 * (DOPInWater - BrineDOP[MyBoxNumber]);
           /*****************************************************************************************************************************************/
           //Calculation of congelation fluxes and storage of obtained values in local variables
           /*****************************************************************************************************************************************/
           AmmoniaCongelationFlux = AFactor2 * AmmoniaInWater / TimeStep;
           NitrateCongelationFlux = AFactor2 * NitrateInWater / TimeStep;
           NitriteCongelationFlux = AFactor2 * NitriteInWater / TimeStep;
           PhosphateCongelationFlux  = AFactor2 * PhosphateInWater / TimeStep;
           SilicaCongelationFlux = AFactor2 * SilicaInWater / TimeStep;
           OxygenCongelationFlux = AFactor2 * OxygenInWater / TimeStep;
           DOCCongelationFlux = AFactor2 * DOCInWater / TimeStep;
           DONCongelationFlux = AFactor2 * DONInWater / TimeStep;
           DOPCongelationFlux = AFactor2 * DOPInWater / TimeStep;
           /*****************************************************************************************************************************************/
           //Update of brine fluxes according to fluxes calculated above. Note that the division by fs of the congelation fluxes is to correct for the relative volume of brine channels
           /*****************************************************************************************************************************************/
           NH4BrineFlux[MyBoxNumber] =  NH4BrineFlux[MyBoxNumber] + AmmoniaBrineFlux + AmmoniaCongelationFlux / fs;
           NO3BrineFlux[MyBoxNumber] = NO3BrineFlux[MyBoxNumber] + NitrateBrineFlux + NitrateCongelationFlux / fs;
           NO2BrineFlux[MyBoxNumber] = NO2BrineFlux[MyBoxNumber] + NitriteBrineFlux + NitriteCongelationFlux / fs;
           PhosphateBrineFlux[MyBoxNumber] = PhosphateBrineFlux[MyBoxNumber] + PhosphorusBrineFlux + PhosphateCongelationFlux / fs;
           SilicateBrineFlux[MyBoxNumber] = SilicateBrineFlux[MyBoxNumber] + SilicaBrineFlux + SilicaCongelationFlux / fs;

           OxygenBrineFlux[MyBoxNumber] = OxygenBrineFlux[MyBoxNumber] + OxyBrineFlux + OxygenCongelationFlux / fs;

           DOCBrineFlux[MyBoxNumber] = DOCBrineFlux[MyBoxNumber] + DOCarbonBrineFlux + DOCCongelationFlux / fs;
           DONBrineFlux[MyBoxNumber] = DONBrineFlux[MyBoxNumber] + DONitrogenBrineFlux + DONCongelationFlux / fs;
           DOPBrineFlux[MyBoxNumber] = DOPBrineFlux[MyBoxNumber] + DOPhosphorusBrineFlux + DOPCongelationFlux / fs;
           /*****************************************************************************************************************************************/
           //Return of brine+congelation fluxes to the nutrient pointer for update of values of corresponding variables in the water
           /*****************************************************************************************************************************************/
           MyNutrientPointer->Update(GetEcoDynClassName(), -(AmmoniaBrineFlux + AmmoniaCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Ammonia",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(NitrateBrineFlux + NitrateCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Nitrate",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(NitriteBrineFlux + NitriteCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Nitrite",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(PhosphorusBrineFlux + PhosphateCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Phosphate",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(SilicaBrineFlux + SilicaCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Silica",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(OxyBrineFlux + OxygenCongelationFlux)* SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Oxygen",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(DOCarbonBrineFlux + DOCCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DOC",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(DONitrogenBrineFlux + DONCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DON",ObjectCode);
           MyNutrientPointer->Update(GetEcoDynClassName(), -(DOPhosphorusBrineFlux + DOPCongelationFlux) * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DOP",ObjectCode);
       }
       if (MySPMPointer != NULL)
       {
           double DetritalCInWater, DetritalNInWater, DetritalPInWater, TPMInWater, POMInWater,
                  CBrineFlux, NBrineFlux, PBrineFlux, TPMBrineFlux, POMBrineFlux,
                  CCongelationFlux, NCongelationFlux, PCongelationFlux, TPMCongelationFlux, POMCongelationFlux;
           /*****************************************************************************************************************************************/
           //Inputs from the suspended matter pointer
           /*****************************************************************************************************************************************/
           MySPMPointer->Inquiry(GetEcoDynClassName(), DetritalCInWater,Index,"DetritalC",ObjectCode);
           MySPMPointer->Inquiry(GetEcoDynClassName(), DetritalNInWater,Index,"DetritalN",ObjectCode);
           MySPMPointer->Inquiry(GetEcoDynClassName(), DetritalPInWater,Index,"DetritalP",ObjectCode);
           MySPMPointer->Inquiry(GetEcoDynClassName(), TPMInWater,Index,"TPMInWater",ObjectCode);
           MySPMPointer->Inquiry(GetEcoDynClassName(), POMInWater,Index,"POMInWater",ObjectCode);
           /*****************************************************************************************************************************************/
           //Calculation of brine fluxes of dissolved substances and storage of obtained values in local variables
           /*****************************************************************************************************************************************/
           CBrineFlux = AFactor1 * (DetritalCInWater - BrineDetritalIceC[MyBoxNumber]);
           NBrineFlux = AFactor1 * (DetritalNInWater - BrineDetritalIceN[MyBoxNumber]);
           PBrineFlux = AFactor1 * (DetritalPInWater - BrineDetritalIceP[MyBoxNumber]);
           TPMBrineFlux = AFactor1 * (TPMInWater - BrineTPM[MyBoxNumber]);
           POMBrineFlux = AFactor1 * (POMInWater - BrinePOM[MyBoxNumber]);
           /*****************************************************************************************************************************************/
           //Calculation of congelation fluxes and storage of obtained values in local variables
           /*****************************************************************************************************************************************/
           CCongelationFlux = AFactor2 * DetritalCInWater / TimeStep;
           NCongelationFlux = AFactor2 * DetritalNInWater / TimeStep;
           PCongelationFlux = AFactor2 * DetritalPInWater / TimeStep;
           TPMCongelationFlux =  AFactor2 * TPMInWater / TimeStep;
           POMCongelationFlux = AFactor2 * POMInWater / TimeStep;
           /*****************************************************************************************************************************************/
           //Update of brine fluxes according to fluxes calculated above. Note that the division by fs of the congelation fluxes is to correct for the relative volume of brine channels
           /*****************************************************************************************************************************************/
           DetritalIceCBrineFlux[MyBoxNumber] = DetritalIceCBrineFlux[MyBoxNumber] + CBrineFlux + CCongelationFlux / fs;
           DetritalIceNBrineFlux[MyBoxNumber] = DetritalIceNBrineFlux[MyBoxNumber] + NBrineFlux + NCongelationFlux / fs;
           DetritalIcePBrineFlux[MyBoxNumber] = DetritalIcePBrineFlux[MyBoxNumber] + PBrineFlux + PCongelationFlux / fs;
           TPMIceBrineFlux[MyBoxNumber] = TPMIceBrineFlux[MyBoxNumber] + TPMBrineFlux + TPMCongelationFlux / fs;
           POMIceBrineFlux[MyBoxNumber] = POMIceBrineFlux[MyBoxNumber] + POMBrineFlux + POMCongelationFlux / fs;
          /*****************************************************************************************************************************************/
           //Return of brine+congelation fluxes to the suspended matter pointer for update of values of corresponding variables in the water
           /*****************************************************************************************************************************************/
           MySPMPointer->Update(GetEcoDynClassName(), -(CBrineFlux + CCongelationFlux) * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DetritalC",ObjectCode);
           MySPMPointer->Update(GetEcoDynClassName(), -(NBrineFlux + NCongelationFlux) * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DetritalN",ObjectCode);
           MySPMPointer->Update(GetEcoDynClassName(), -(PBrineFlux + PCongelationFlux) * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DetritalP",ObjectCode);
           MySPMPointer->Update(GetEcoDynClassName(), -(TPMBrineFlux + TPMCongelationFlux) * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"TPM",ObjectCode);
           MySPMPointer->Update(GetEcoDynClassName(), -(POMBrineFlux + POMCongelationFlux) * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"POM",ObjectCode);
       }
   } //end
}


void TIce::SkeletalMeltFluxes(int ABoxNumber, int Index, double ASeaBoxDepth)
{
   int MyBoxNumber;
   double MyWaterTemperature, VolumeAreaCorrection, TINNY, AValue, AFactor;
   TINNY = 0.000000001;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer(),
               * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
               * MySPMPointer = MyPEcoDynClass->GetSPMPointer();

   if ((IceGrowthRate[MyBoxNumber] < 0.0) && (MyWaterTemperaturePointer != NULL))
   {
     MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,Index,"Water temperature",ObjectCode);
     if ((IceGrowthRate[MyBoxNumber] < 0.0) && (MyWaterTemperature > FreezingPointOfSeawater))
     {
         if ((SkeletalLayerDepth[MyBoxNumber] > TINNY) && (SeaBoxDepth > TINNY))
            AFactor = IceGrowthRate[MyBoxNumber] / SkeletalLayerDepth[MyBoxNumber];
         else
            AFactor = 0.0;
         if ((MyNutrientPointer != NULL) && (ASeaBoxDepth > TINNY))
         {
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkNH4[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Ammonia",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkNO3[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Nitrate",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkNO2[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Nitrite",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkPhosphate[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Phosphate",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkSilicate[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Silica",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkOxygen[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"Oxygen",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkDOC[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DOC",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkDON[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DON",ObjectCode);
             MyNutrientPointer->Update(GetEcoDynClassName(), AFactor * BulkDOP[MyBoxNumber] * CUBIC * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DOP",ObjectCode);
         }
         if ((MySPMPointer != NULL) && (ASeaBoxDepth > TINNY))
         {
             MySPMPointer->Update(GetEcoDynClassName(), AFactor * BulkDetritalIceC[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DetritalC",ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), AFactor * BulkDetritalIceN[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DetritalN",ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), AFactor * BulkDetritalIceP[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"DetritalP",ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), AFactor * BulkTPM[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"TPM",ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), AFactor * BulkPOM[MyBoxNumber] * SkeletalLayerDepth[MyBoxNumber] / ASeaBoxDepth, Index,"POM",ObjectCode);
         }
     }
   }
}



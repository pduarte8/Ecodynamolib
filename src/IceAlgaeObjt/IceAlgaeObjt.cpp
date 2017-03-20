// Ice Algae object CPP code file
// @ Pedro Duarte
// Rev. A 03.06.2013
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "IceAlgaeObjt.h"
#include "params.h"
#include "EcoDynClass.h"
#include "iodll.h"

// Constructors invoked outside EcoDyn shell...

TIceAlgae::TIceAlgae(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration)
    :TProdutor(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    InitializeVariables(className);
    ReadVariablesAndParameters(className);
}

TIceAlgae::TIceAlgae(char* className, float timeStep, char* morphologyFilename,
                char* parametersFilename):TProdutor(className, timeStep, morphologyFilename)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology()) {
        InitializeVariables(className);
        ReadVariablesAndParameters(className);
    }
}
// ...

TIceAlgae::TIceAlgae(TEcoDynClass* APEcoDynClass, char* className): TProdutor(APEcoDynClass, className)
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcoDynClass;
    bool AIceRecord;
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
        cerr << "TIceAlgae::TIceAlgae - Ice record missing in TIce." << endl;
#endif  // __BORLANDC__
    }
    InitializeVariables(className);
    ReadVariablesAndParameters(className);
}

void TIceAlgae::InitializeVariables(char* className)
{
    ObjectCode = ICEALGAEOBJECTCODE;
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
    // Get array size
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    TimeStep = MyPEcoDynClass->GetTimeStep();
    // Initialise pointers
    if (NumberOfBoxesWithIce > 0)                       // Initialise arrays for variable pairs - one
    {
        IceAlgaeChl = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeC = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeN = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeP = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeSi = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        NCellQuota = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        PCellQuota = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        SiCellQuota = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeChlFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeCFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeNFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaePFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeSiFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        KForIceAlgae = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeLayerThickness = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        OxygenNetProduction = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeChlInBox = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeCInBox = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeNInBox = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaePInBox = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        IceAlgaeSiInBox = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        DailyMeanGPP = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        NUptake = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        PUptake = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        SiUptake = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        NCellFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        PCellFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        SiCellFlux = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        NLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        PLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        SiLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        NPSLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        LightLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        TemperatureLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        SaltLimitation = (double*) calloc(NumberOfBoxesWithIce, sizeof(double));
        /*for (int i = 0; i < NumberOfBoxesWithIce; i++)
        {
      	    IceAlgaeChl[i] = 0.0;
            IceAlgaeC[i] = 0.0;
            IceAlgaeN[i] = 0.0;
            IceAlgaeP[i] = 0.0;
            IceAlgaeSi[i] = 0.0;
            NCellQuota[i] = 0.0;
            PCellQuota[i] = 0.0;
            SiCellQuota[i] = 0.0;
            IceAlgaeChlFlux[i] = 0.0;
            IceAlgaeCFlux[i] = 0.0;
            IceAlgaeNFlux[i] = 0.0;
            IceAlgaePFlux[i] = 0.0;
            IceAlgaeSiFlux[i] = 0.0;
            KForIceAlgae[i] = 0.0;
            IceAlgaeLayerThickness[i] = 0.0;
            OxygenNetProduction[i] = 0.0;
            IceAlgaeChlInBox[i] = 0.0;
            IceAlgaeCInBox[i] = 0.0;
            IceAlgaeNInBox[i] = 0.0;
            IceAlgaePInBox[i] = 0.0;
            IceAlgaeSiInBox[i] = 0.0;
            DailyMeanGPP[i] = 0.0;
            ChlToCarbon[i] = 50.0;
            NUptake[i] = 0.0;
            PUptake[i] = 0.0;
            SiUptake[i] = 0.0;
            NCellFlux[i] = 0.0;
            PCellFlux[i] = 0.0;
            SiCellFlux[i] = 0.0;
            NLimitation[i] = 0.0;
            PLimitation[i] = 0.0;
            SiLimitation[i] = 0.0;
            NPSLimitation[i] = 0.0;
            LightLimitation[i] = 0.0;
            TemperatureLimitation[i] = 0.0;
            SaltLimitation[i] = 0.0;
        } */
        MaxCCellQuota = 0.0;
        MaxNCellQuota = 0.0;
        MaxPCellQuota = 0.0;
        MaxSiCellQuota = 0.0;
        MinCCellQuota = 0.0;
        MinNCellQuota = 0.0;
        MinPCellQuota = 0.0;
        MinSiCellQuota = 0.0;
        KNInternal = 0.028;  //[mg N mg C-1] Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KPInternal = 0.04;   //[mg P mg C-1] Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KSiInternal = 0.0;
        MaxNPRatio = 291.0;
        MinNPRatio = 4.0;
        MinNSiRatio = 0.06;
        CarbonToOxygenProd = 2.666667;
        CarbonToOxygenResp = 0.0;
        LightAtTop = 0.0;
        LightAtBottom = 0.0;
        WaterTemperature = 0.0;
        TminPhotosynthesis = 0.0;
        TINNY = 0.0000000001;
        IntegrationSteps = 10.0;
        NumberOfParcels = 0.0;
        CarbonToOxygenProd = 0.375; //Assuming a PQ = 1
        RatioLightDarkRespiration = 2.0;
        TminRespiration = 0.0;
        TminPhotosynthesis = 0.0;
        TminMortality = 0.0;
        TempCoefRespiration = 0.0663;      //Jin et al (2006) - Annals of Glaciology 44 2006
        TempCoefPhotosynthesis = 0.0663;   //Jin et al (2006) - Annals of Glaciology 44 2006
        TempCoefMortality = 0.03;          //Jin et al (2006) - Annals of Glaciology 44 2006
        MortalityAtZeroDegres = 0.000923; //Jin et al (2006) - Annals of Glaciology 44 2006
        KNH4 = 2.94;                      //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KNO3 = 30.0;                      //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KP = 2.0;                         //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KSi = 30.0;        //Check????????????????
        NMaxUptake = 1.08;                //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        PMaxUptake = 1.08;                //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        SiMaxUptake = 1.08;               //Assumed the same as N and PMaxUptakes
        MaxNPRatio = 291.0;               //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        MinNPRatio = 4.0;                 //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        MinNSiRatio = 0.06; //Check?????????????
        KNO3External = 1.0;               //[micromol N l-1] Jin et al (2006) - Annals of Glaciology 44 2006
        KNH4External = 1.0;               //[micromol N l-1] Jin et al (2006) - Annals of Glaciology 44 2006
        KPExternal = 0.1;                 //[micromol P l-1] Check!
        KSiExternal = 4;                  //[micromol Si l-1]  Jin et al (2006) - Annals of Glaciology 44 2006
        Phi = 1.46;                       //[micromol Si-1 l]  Jin et al (2006) - Annals of Glaciology 44 2006
        DocStressLoss = 0.0;
        FreezingPointOfSeawater = -1.86;
        Seeding = 50.0; //mg C / m2  Value assumed as a minimum to restart ice algal growth
        strcpy(PIfunction, "Platt");
        strcpy(LimitationType, "External");
        ADay = MyPEcoDynClass->GetJulianDay();
        NutLimitation = true; SalLimitation = true; TempLimitation = true;

    }
    else
#ifdef __BORLANDC__
				MessageBox(0,"Ice algae object array not dimensioned","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TIceAlgae::InitializeVariables - Ice algae object array not dimensioned" << endl;
#endif  // __BORLANDC__

}

void TIceAlgae::ReadVariablesAndParameters(char* className)
{
   char MyVariable[65];
   double Value;
   //Read Variables file
   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("IceAlgae");
   if (PReadWrite == NULL) 
#ifdef __BORLANDC__
			MessageBox(0, "TIceAlgae variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TIceAlgae::ReadVariablesAndParameters - Variables file missing." << endl;
#endif  // __BORLANDC__
   if (PReadWrite != NULL)
   {
       int X, Y, XV, YV;
       if (PReadWrite->FindString("Ice algae", X, Y))
       {
          //Debugger(X); Debugger(Y);
          PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
          VariableNameArray = new VNA[NumberOfVariables];
          // Create array for state variable names
          // read in the variable names
          for (int i = 0; i < NumberOfVariables; i++)
          {
             PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
             //MessageBox(0, VariableNameArray[i], "EcoDynamo alert", MB_OK);
          }
          for (int i = 0; i < NumberOfVariables; i++)
          {
             if (strcmp(VariableNameArray[i], "Ice algae chl") == 0)
             {
                if (!PReadWrite->FindString("Ice algae chl values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, IceAlgaeChl[j]);
                }
             }
             if (strcmp(VariableNameArray[i], "Ice algae C") == 0)
             {
                if (!PReadWrite->FindString("Ice algae C values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, IceAlgaeC[j]);
                }
             }
             if (strcmp(VariableNameArray[i], "Ice algae N") == 0)
             {
                if (!PReadWrite->FindString("Ice algae N values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, IceAlgaeN[j]);
                }
             }
             if (strcmp(VariableNameArray[i], "Ice algae P") == 0)
             {
                if (!PReadWrite->FindString("Ice algae P values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, IceAlgaeP[j]);
                }
             }
             if (strcmp(VariableNameArray[i], "Ice algae Si") == 0)
             {
                if (!PReadWrite->FindString("Ice algae Si values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, IceAlgaeSi[j]);
                }
             }
             if (strcmp(VariableNameArray[i], "ChlToCarbon") == 0)
             {
                if (!PReadWrite->FindString("ChlToCarbon values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, ChlToCarbon[j]);
                }
             }
             if (strcmp(VariableNameArray[i], "IceAlgaeLayerThickness") == 0)
             {
                if (!PReadWrite->FindString("IceAlgaeLayerThickness values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, IceAlgaeLayerThickness[j]);
                }
                //Debugger(IceAlgaeLayerThickness[NumberOfBoxesWithIce-1]);
             }
          }
          CloseDataFile((void*)PReadWrite);
       }
       else
#ifdef __BORLANDC__
          MessageBox(0, "Variables:Undefined object - TIceAlgae", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TIceAlgae::ReadVariablesAndParameters - variables: undefined object TIceAlgae" << endl;
#endif  // __BORLANDC__
   }
   for (int j = 0; j < NumberOfBoxesWithIce; j++)
      ChlExtinctionCoefficient(j);
   PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("IceAlgae");
   if (PReadWrite == NULL) 
#ifdef __BORLANDC__
			MessageBox(0, "TIceAlgae parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TIceAlgae::ReadVariablesAndParameters - Parameters file missing." << endl;
#endif  // __BORLANDC__

   if (PReadWrite != NULL)
   {
       int X, Y;
       if (PReadWrite->FindString("Ice algae", X, Y))
       {
          //Debugger(X); Debugger(Y);

          PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
          // Create array for state variable names
          // read in the variable names
          for (int i = 0; i < NumberOfParameters; i++)
          {
             double MyValue;
             PReadWrite->ReadString(X + i + 2, Y, MyVariable);
             if (strcmp(MyVariable, "P-I function") == 0)
             {
                PReadWrite->ReadString(X + i + 2, Y + 1, PIfunction);
             }
             if (strcmp(MyVariable, "Nutrient limitation") == 0)
             {
                PReadWrite->ReadString(X + i + 2, Y + 1, LimitationType);
             }
             if (strcmp(MyVariable, "Pmax") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1 + j, Pmax[j]);
                }
                //Debugger(Pmax[NumberOfBoxesWithIce-1]);
             }
             if (strcmp(MyVariable, "Slope") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1 + j, Slope[j]);
                }
                //Debugger(Slope[NumberOfBoxesWithIce-1]);
             }
             if (strcmp(MyVariable, "beta") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce; j++)
                {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1 + j, beta[j]);
                }
                //Debugger(beta[NumberOfBoxesWithIce-1]);
             }
             if (strcmp(MyVariable, "CarbonToOxygenProd") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, CarbonToOxygenProd); //Debugger(CarbonToOxygenProd);
             }
             if (strcmp(MyVariable, "RespirationCoefficient") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, RespirationCoefficient); //Debugger(RespirationCoefficient);
             }
             if (strcmp(MyVariable, "R0") == 0)
             {
                   double AValue;
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, AValue); //Debugger(AValue);
                   for (int i = 0; i < NumberOfBoxesWithIce; i++)
                      MaintenanceRespiration[i] = AValue;
             }
             if (strcmp(MyVariable, "RatioLightDarkRespiration") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, RatioLightDarkRespiration); //Debugger(RatioLightDarkRespiration);
             }
             if (strcmp(MyVariable, "TminRespiration") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, TminRespiration); //Debugger(TminRespiration);
             }
             if (strcmp(MyVariable, "TminPhotosynthesis") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, TminPhotosynthesis); //Debugger(TminPhotosynthesis);
             }
             if (strcmp(MyVariable, "TminMortality") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, TminMortality); //Debugger(TminMortality);
             }
             if (strcmp(MyVariable, "TempCoefRespiration") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, TempCoefRespiration); //Debugger(TempCoefRespiration);
             }
             if (strcmp(MyVariable, "TempCoefPhotosynthesis") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, TempCoefPhotosynthesis); //Debugger(TempCoefPhotosynthesis);
             }
             if (strcmp(MyVariable, "TempCoefMortality") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, TempCoefMortality); //Debugger(TempCoefMortality);
             }
             if (strcmp(MyVariable, "MortalityAtZeroDegres") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MortalityAtZeroDegres); //Debugger(MortalityAtZeroDegres);
             }
             if (strcmp(MyVariable, "MaxNPRatio") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MaxNPRatio); //Debugger(MaxNPRatio);
             }
             if (strcmp(MyVariable, "MinNPRatio") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MinNPRatio); //Debugger(MinNPRatio);
             }
             if (strcmp(MyVariable, "MinNSiRatio") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MinNSiRatio); //Debugger(MinNSiRatio);
             }
             if (strcmp(MyVariable, "KNH4") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KNH4); //Debugger(KNH4);
             }
             if (strcmp(MyVariable, "KNO3") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KNO3); //Debugger(KNO3);
             }
             if (strcmp(MyVariable, "KP") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KP); //Debugger(KP);
             }
             if (strcmp(MyVariable, "KSi") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KSi); //Debugger(KSi);
             }
             if (strcmp(MyVariable, "NMaxUptake") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, NMaxUptake); //Debugger(NMaxUptake);
             }
             if (strcmp(MyVariable, "PMaxUptake") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, PMaxUptake); //Debugger(PMaxUptake);
             }
             if (strcmp(MyVariable, "SiMaxUptake") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, SiMaxUptake); //Debugger(SiMaxUptake);
             }
             if (strcmp(MyVariable, "KNInternal") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KNInternal); //Debugger(KNInternal);
             }
             if (strcmp(MyVariable, "KPInternal") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KPInternal); //Debugger(KPInternal);
             }
             if (strcmp(MyVariable, "KSiInternal") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KSiInternal); //Debugger(KSiInternal);
             }
             if (strcmp(MyVariable, "KNO3External") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KNO3External); //Debugger(KNO3External);
             }
             if (strcmp(MyVariable, "KNH4External") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KNH4External); //Debugger(KNH4External);
             }
             if (strcmp(MyVariable, "KPExternal") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KPExternal); //Debugger(KPExternal);
             }
             if (strcmp(MyVariable, "KSiExternal") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, KSiExternal); //Debugger(KSiExternal);
             }
             if (strcmp(MyVariable, "Phi") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, Phi); //Debugger(Phi);
             }
             if (strcmp(MyVariable, "DocStressLoss") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, DocStressLoss); //Debugger(DocStressLoss);
             }
             if (strcmp(MyVariable, "FreezingPointOfSeawater") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, FreezingPointOfSeawater); //Debugger(FreezingPointOfSeawater);
             }
             if (strcmp(MyVariable, "Seeding") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, Seeding); //Debugger(Seeding);
             }
             if (strcmp(MyVariable, "IntegrationSteps") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, IntegrationSteps); //Debugger(IntegrationSteps);
             }
             if (strcmp(MyVariable, "NutLimitation") == 0)
             {
                PReadWrite->ReadNumber(X + i + 2, Y + 1, Value);
                if (Value > 0) NutLimitation = true;
             }
             if (strcmp(MyVariable, "SalLimitation") == 0)
             {
                PReadWrite->ReadNumber(X + i + 2, Y + 1, Value);
                if (Value > 0) SalLimitation = true;
             }
             if (strcmp(MyVariable, "TempLimitation") == 0)
             {
                PReadWrite->ReadNumber(X + i + 2, Y + 1, Value);
                if (Value > 0) TempLimitation = true;
             }
          }
          CloseDataFile((void*)PReadWrite);
       }
       else
#ifdef __BORLANDC__
          MessageBox(0, "Parameters: Undefined object - TIceAlgae", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
	  cerr << "TIceAlgae::ReadVariablesAndParameters - Parameters: undefined object TIceAlgae" << endl;
#endif  // __BORLANDC__
   }
}

TIceAlgae::~TIceAlgae()
{
    freeMemory();
}

void TIceAlgae::freeMemory()
{
    try {
        if (NumberOfBoxesWithIce > 0)
        {
            free(IceAlgaeChl);
            free(IceAlgaeC);
            free(IceAlgaeN);
            free(IceAlgaeP);
            free(IceAlgaeSi);
            free(NCellQuota);
            free(PCellQuota);
            free(SiCellQuota);
            free(IceAlgaeChlFlux);
            free(IceAlgaeCFlux);
            free(IceAlgaeNFlux);
            free(IceAlgaePFlux);
            free(IceAlgaeSiFlux);
            free(KForIceAlgae);
            free(IceAlgaeLayerThickness);
            free(OxygenNetProduction);
            free(VariableNameArray);
            free(IceAlgaeChlInBox);
            free(IceAlgaeCInBox);
            free(IceAlgaeNInBox);
            free(IceAlgaePInBox);
            free(IceAlgaeSiInBox);
            free(DailyMeanGPP);
            free(NUptake);
            free(PUptake);
            free(SiUptake);
            free(NCellFlux);
            free(PCellFlux);
            free(SiCellFlux);
            free(NLimitation);
            free(PLimitation);
            free(SiLimitation);
            free(NPSLimitation);
            free(LightLimitation);
            free(TemperatureLimitation);
            free(SaltLimitation);
        }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
    }
    catch (Exception &exception) {
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TIceAlgae::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

VNA* TIceAlgae::InquiryIce(char* srcName,int &IceLayers, int &IceVariables)
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


void TIceAlgae::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   if (strcmp(MyParameter, "Ice algae chl") == 0)
		Value = IceAlgaeChl[MyBoxNumber];    //mg / m2
   else if (strcmp(MyParameter, "Ice algae C") == 0)
		Value = IceAlgaeC[MyBoxNumber];      //mg / m2
   else if (strcmp(MyParameter, "Ice algae N") == 0)
		Value = IceAlgaeN[MyBoxNumber];      //mg / m2
   else if (strcmp(MyParameter, "Ice algae P") == 0)
		Value = IceAlgaeP[MyBoxNumber];      //mg / m2
   else if (strcmp(MyParameter, "Ice algae Si") == 0)
		Value = IceAlgaeSi[MyBoxNumber];      //mg / m2
   else if (strcmp(MyParameter, "Ice algae chl in box") == 0)
		Value = IceAlgaeChlInBox[MyBoxNumber];   //mg / m2
   else if (strcmp(MyParameter, "Ice algae C in box") == 0)
		Value = IceAlgaeCInBox[MyBoxNumber];      //mg / m2
   else if (strcmp(MyParameter, "Ice algae N in box") == 0)
		Value = IceAlgaeNInBox[MyBoxNumber];      //mg / m2
   else if (strcmp(MyParameter, "Ice algae P in box") == 0)
		Value = IceAlgaePInBox[MyBoxNumber];     //mg / m2
   else if (strcmp(MyParameter, "Ice algae Si in box") == 0)
		Value = IceAlgaeSiInBox[MyBoxNumber];    //mg / m2
   else if (strcmp(MyParameter, "NCellQuota") == 0)
		Value = NCellQuota[MyBoxNumber];
   else if (strcmp(MyParameter, "PCellQuota") == 0)
		Value = PCellQuota[MyBoxNumber];
   else if (strcmp(MyParameter, "SiCellQuota") == 0)
		Value = SiCellQuota[MyBoxNumber];
   else if (strcmp(MyParameter, "IceAlgaeNPP") == 0)
              Value = NPP[MyBoxNumber] * HOURSTOSECONDS;
   else if (strcmp(MyParameter, "IceAlgaeGPP") == 0)
              Value = GPP[MyBoxNumber] * HOURSTOSECONDS;
   else if (strcmp(MyParameter, "Pmax") == 0)
      Value = Pmax[MyBoxNumber];                //mg C / mg Chl / h
   else if (strcmp(MyParameter, "beta") == 0)
      Value = beta[MyBoxNumber];                //mg C mg Ch1-1 h -1 (microE m -2 s-l) -1
   else if (strcmp(MyParameter, "Slope") == 0)
      Value = Slope[MyBoxNumber];               //mg C mg Ch1-1 h -1 (microE m -2 s-l) -1
   else if (strcmp(MyParameter, "KForIceAlgae") == 0)
      Value = KForIceAlgae[MyBoxNumber];                  //m-1
   else if (strcmp(MyParameter, "IceAlgaeLayerThickness") == 0)
      Value = IceAlgaeLayerThickness[MyBoxNumber];       //m
   else if (strcmp(MyParameter, "DailyMeanGPP") == 0)
      Value = DailyMeanGPP[MyBoxNumber] * HOURSTOSECONDS; //mg C / mg Chl / h
   else if (strcmp(MyParameter, "ChlToCarbon") == 0)
      Value = ChlToCarbon[MyBoxNumber];
   else if (strcmp(MyParameter, "NLimitation") == 0)
      Value = NLimitation[MyBoxNumber];
   else if (strcmp(MyParameter, "PLimitation") == 0)
      Value = PLimitation[MyBoxNumber];
   else if (strcmp(MyParameter, "SiLimitation") == 0)
      Value = SiLimitation[MyBoxNumber];
   else if (strcmp(MyParameter, "NPSLimitation") == 0)
      Value = NPSLimitation[MyBoxNumber];
   else if (strcmp(MyParameter, "LightLimitation") == 0)
      Value = LightLimitation[MyBoxNumber];
   else if (strcmp(MyParameter, "TemperatureLimitation") == 0)
      Value = TemperatureLimitation[MyBoxNumber];
   else if (strcmp(MyParameter, "SaltLimitation") == 0)
      Value = SaltLimitation[MyBoxNumber];
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
                    "EcoDynamo Alert - Inquiry 1",
                    MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TIceAlgae::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
      }
   }
   LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TIceAlgae::SetVariableValue(char* srcName, double Value,int BoxNumber,char* VariableName)
{
    bool rc = true;
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);
    if (strcmp(VariableName, "Ice algae chl") == 0)
    IceAlgaeChl[BoxNumber] = Value;
    else if (strcmp(VariableName, "Ice algae C") == 0)
    IceAlgaeC[BoxNumber] = Value;
    else if (strcmp(VariableName, "Ice algae N") == 0)
    IceAlgaeN[BoxNumber] = Value;
    else if (strcmp(VariableName, "Ice algae P") == 0)
    IceAlgaeP[BoxNumber] = Value;
    else if (strcmp(VariableName, "Ice algae Si") == 0)
    IceAlgaeSi[BoxNumber] = Value;
    else if (strcmp(VariableName, "NCellQuota") == 0)
    NCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "PCellQuota") == 0)
    PCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "SiCellQuota") == 0)
    SiCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "IceAlgaeNPP") == 0)
    NPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "IceAlgaeGPP") == 0)
    GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "Pmax") == 0)
    Pmax[BoxNumber] = Value;
    else if (strcmp(VariableName, "beta") == 0)
    beta[BoxNumber] = Value;
    else if (strcmp(VariableName, "Slope") == 0)
    Slope[BoxNumber] = Value;
    else if (strcmp(VariableName, "KForIceAlgae") == 0)
    KForIceAlgae[BoxNumber] = Value;
    else if (strcmp(VariableName, "IceAlgaeLayerThickness") == 0)
    IceAlgaeLayerThickness[BoxNumber] = Value;
    else if (strcmp(VariableName, "DailyMeanGPP") == 0)
    DailyMeanGPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "ChlToCarbon") == 0)
    ChlToCarbon[BoxNumber] = Value;
    else if (strcmp(VariableName, "NLimitation") == 0)
    NLimitation[BoxNumber] = Value;
    else if (strcmp(VariableName, "PLimitation") == 0)
    PLimitation[BoxNumber] = Value;
    else if (strcmp(VariableName, "SiLimitation") == 0)
    SiLimitation[BoxNumber] = Value;
    else if (strcmp(VariableName, "NPSLimitation") == 0)
    NPSLimitation[BoxNumber] = Value;
    else if (strcmp(VariableName, "LightLimitation") == 0)
    LightLimitation[BoxNumber] = Value;
    else if (strcmp(VariableName, "TemperatureLimitation") == 0)
    TemperatureLimitation[BoxNumber] = Value;
    else rc = false;
    return rc;
}

void TIceAlgae::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);
    if (strcmp(MyParameter, "Ice algae chl") == 0)
        IceAlgaeChlFlux[MyBoxNumber] = IceAlgaeChlFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Ice algae C") == 0)
		IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Ice algae N") == 0)
		IceAlgaeNFlux[MyBoxNumber] = IceAlgaeNFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Ice algae P") == 0)
		IceAlgaePFlux[MyBoxNumber] = IceAlgaePFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Ice algae Si") == 0)
		IceAlgaeSiFlux[MyBoxNumber] = IceAlgaeSiFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
        char Caption[129];
        strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
        MessageBox(0,Caption,"EcoDynamo Alert - Update 1",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TIceAlgae::Update - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}

void TIceAlgae::Go()
{
   double MyIceDepth;
   TEcoDynClass * MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if (MySeaIcePointer != NULL)
   {
       if (MyPEcoDynClass->GetJulianDay() != ADay)
       {
          NumberOfParcels = 0.0;
          ADay = ADay + 1;
       }
       for (int i = 0; i < NumberOfBoxesWithIce; i++)
       {
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceDepth, i,"SIheff", ObjectCode);
          MySeaIcePointer->Inquiry(GetEcoDynClassName(), BrineFractionalVolume, i,"BrineFractionalVolume", ObjectCode);
          //Debugger(BrineFractionalVolume);
          if (
               (IceAlgaeC[i] > TINNY) && (IceAlgaeLayerThickness[i] > TINNY) &&
               (MyPEcoDynClass->GetBoxDepth(i) > TINNY) && (MyIceDepth > IceAlgaeLayerThickness[i])
             )
          {
             if (NumberOfParcels == 0.0) DailyMeanGPP[i] = 0.0;
             ChlExtinctionCoefficient(i);
             Production(i);
             Respiration(i);
             Exudation(i);
             NitrogenUptake(i);
             PhosphorusUptake(i);
             SilicaUptake(i);
             Mortality(i); 
             IceSeaTransfer(i);
             IceAlgaeChlFlux[i] = IceAlgaeCFlux[i] / ChlToCarbon[i];  //mg Chl m-2 s-1
          }
       }
   }
   else
#ifdef __BORLANDC__
      MessageBox(0, "SeaIceObject not available - TIceAlgae::Go", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TIceAlgae::Go - SeaIceObject not available" << endl;
#endif  // __BORLANDC__
   NumberOfParcels = NumberOfParcels + 1.0;
}

void TIceAlgae::Integrate()
{
    Integration(IceAlgaeChl, IceAlgaeChlFlux, IceAlgaeC, IceAlgaeCFlux, IceAlgaeN, IceAlgaeNFlux, IceAlgaeP, IceAlgaePFlux, IceAlgaeSi, IceAlgaeSiFlux);
    TEcoDynClass*  MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    for (int i = 0; i < NumberOfBoxesWithIce; i++)
    {
        if (IceAlgaeChl[i] <= Seeding / ChlToCarbon[i]) IceAlgaeChl[i] = Seeding / ChlToCarbon[i];
        if (IceAlgaeC[i] <= Seeding) IceAlgaeC[i] = Seeding;
        if (IceAlgaeN[i] <= Seeding * NCellQuota[i]) IceAlgaeN[i] = Seeding * NCellQuota[i];
        if (IceAlgaeP[i] <= Seeding * PCellQuota[i]) IceAlgaeP[i] = Seeding * PCellQuota[i];
        if (IceAlgaeSi[i] <= Seeding * SiCellQuota[i]) IceAlgaeSi[i] = Seeding * SiCellQuota[i];
        if (MySeaIcePointer != NULL)
        {
           double MyIceRelativeArea, MyArea;
           MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceRelativeArea, i,"SIarea", ObjectCode);
           MyArea = MyPEcoDynClass->GetChannelWidth(i) * MyPEcoDynClass->GetBoxLength(i);
           if (MyIceRelativeArea <= TINNY)
           {
              IceAlgaeChl[i] = 0.0;
              IceAlgaeC[i] = 0.0;
              IceAlgaeN[i] = 0.0;
              IceAlgaeP[i] = 0.0;
              IceAlgaeSi[i] = 0.0;
           }
           else
           {
              IceAlgaeChlInBox[i] = IceAlgaeChl[i] / MyArea;
              IceAlgaeCInBox[i] = IceAlgaeC[i] / MyArea;
              IceAlgaeNInBox[i] = IceAlgaeN[i] / MyArea;
              IceAlgaePInBox[i] = IceAlgaeP[i] / MyArea;
              IceAlgaeSiInBox[i] = IceAlgaeSi[i] / MyArea;
           }
        }
        if (IceAlgaeC[i] > TINNY)
        {
           NCellQuota[i] = IceAlgaeN[i] / IceAlgaeC[i];
           PCellQuota[i] = IceAlgaeP[i] / IceAlgaeC[i];
           SiCellQuota[i] = IceAlgaeSi[i] / IceAlgaeC[i];
        }
        else
        {
           NCellQuota[i] = 0.0;
           PCellQuota[i] = 0.0;
           SiCellQuota[i] = 0.0;
        }
    }
}

void TIceAlgae::Production(int ABoxNumber)
{
    //double MyKforVisibleLightInICE;
    int MyBoxNumber;
    MyBoxNumber = ABoxNumber;
    TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
    * MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer(),
    * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

    if (MyLightPointer != NULL)
    {

       MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,MyBoxNumber,"Ice visible light above chl layer",ObjectCode);
       MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,MyBoxNumber,"Ice visible light at bottom",ObjectCode);
       LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
       LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
       if (LightAtTop > 0.0)
       {
          /*if (KForIceAlgae[MyBoxNumber] < TINNY)
          {
             MessageBox(0,"KForIceAlgae to low in TIceAlgae::Production","EcoDynamo alert",MB_OK | MB_ICONHAND);
             Debugger(MyBoxNumber);
          }  */
          if (strcmp(PIfunction, "Platt") == 0)
          {
             GPP[MyBoxNumber] = PlattPIFunction(MyBoxNumber);
          }
          else
          if (strcmp(PIfunction, "Steele") == 0)
          {
             GPP[MyBoxNumber] = SteelePIFunction(MyBoxNumber);
          }
          if ((MyWaterTemperaturePointer != NULL) && (TempLimitation))
          {
             MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,MyBoxNumber,"Water temperature",ObjectCode);
             TemperatureLimitation[MyBoxNumber] = TemperatureArrheniusExponentialLimitation(TempCoefPhotosynthesis, WaterTemperature, TminPhotosynthesis);
             Productivity = Productivity *  TemperatureLimitation[MyBoxNumber];
          }
          if (NutLimitation) NutrientLimitation(MyBoxNumber);
          if (SalLimitation) SalinityLimitation(MyBoxNumber);
       }
       else GPP[MyBoxNumber] = 0.0;
    }
    else
    {
       GPP[MyBoxNumber] = 0.0;
       //MessageBox(0,"Light object not available in TIceAlgae::Production","EcoDynamo alert",MB_OK | MB_ICONHAND);
    }
    OxygenNetProduction[MyBoxNumber] = GPP[MyBoxNumber] / CarbonToOxygenProd; //mg O2 /mgChl /s
    IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] + GPP[MyBoxNumber] * IceAlgaeChl[MyBoxNumber];  //mg C m-2 s-1
    DailyMeanGPP[MyBoxNumber] = (
                                   DailyMeanGPP[MyBoxNumber] * NumberOfParcels + GPP[MyBoxNumber]
                                )  / (NumberOfParcels + 1);  //mg C / mg Chla / s
}

void TIceAlgae::ChlExtinctionCoefficient(int ABoxNumber)
{
   int MyBoxNumber;
   double AnArgument;
   MyBoxNumber = ABoxNumber;
   if (IceAlgaeLayerThickness[MyBoxNumber] < TINNY)
      KForIceAlgae[MyBoxNumber] = 0.0;
   else
   {
      AnArgument = 1.102-0.196*log(IceAlgaeChl[MyBoxNumber]);
      if (AnArgument <= 0.0) AnArgument = TINNY; //This is logarithms of a quantity <= 0
      KForIceAlgae[MyBoxNumber] = MAX(0.0,-log(AnArgument) / IceAlgaeLayerThickness[MyBoxNumber]);
   }
}

double TIceAlgae::PlattPIFunction(int ABoxNumber)
{
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   double DeltaZ, Soma, MyLightAtTop;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   MyLightAtTop = LightAtTop;
   DeltaZ = IceAlgaeLayerThickness[MyBoxNumber] / IntegrationSteps;
   Soma = 0.0;
   for (int Step = 1; Step <= IntegrationSteps; Step++)    //Eiler integration as a function of depth
   {
      Soma = Soma + /*Pmax[MyBoxNumber] * */(1 - exp(-Slope[MyBoxNumber] * MyLightAtTop / Pmax[MyBoxNumber])) * exp(-beta[MyBoxNumber] * MyLightAtTop / Pmax[MyBoxNumber]) * DeltaZ;
      MyLightPointer->Inquiry(GetEcoDynClassName(),MyLightAtTop,MyLightAtTop,DeltaZ,KForIceAlgae[MyBoxNumber],MyBoxNumber,"Sub-surface irradiance",ObjectCode);
   }
   LightLimitation[MyBoxNumber] = Soma / IceAlgaeLayerThickness[MyBoxNumber];
   //return (Soma / IceAlgaeLayerThickness[MyBoxNumber]) / HOURSTOSECONDS; //mg C mg Chl s-1
   return Pmax[MyBoxNumber] * LightLimitation[MyBoxNumber] / HOURSTOSECONDS; //mg C mg Chl s-1
}

double TIceAlgae::SteelePIFunction(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   LightLimitation[MyBoxNumber] =  2.718282 / ( KForIceAlgae[MyBoxNumber] * IceAlgaeLayerThickness[MyBoxNumber])
                                     * (
                                         exp( -LightAtBottom / Iopt[MyBoxNumber] )
                                         -exp( -LightAtTop / Iopt[MyBoxNumber])
                                       );
   return Pmax[MyBoxNumber] * LightLimitation[MyBoxNumber] / HOURSTOSECONDS;  //mg C mg Chl s-1
}

void TIceAlgae::Respiration(int ABoxNumber)
{
    double Resp, WaterTemperature, AverageOxygenProduction;
    int MyBoxNumber;
    double const OxygenMolecularWeight = 32.0;
    MyBoxNumber = ABoxNumber;
    TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

    AverageOxygenProduction = DailyMeanGPP[MyBoxNumber] / CarbonToOxygenProd / OxygenMolecularWeight;  //mmol O2 / mg Chla / s
    Resp = MaintenanceRespiration[MyBoxNumber] / HOURSTOSECONDS;    //mmol O2 / mgChl / s
    if (MyWaterTemperaturePointer != NULL)
       MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,MyBoxNumber,"Water temperature",ObjectCode);
    else
       WaterTemperature = 0.0;
    if (GPP[MyBoxNumber] <= 0.0)
       Resp = Resp + RespirationCoefficient * AverageOxygenProduction
                     * TemperatureArrheniusExponentialLimitation(TempCoefRespiration,WaterTemperature, TminRespiration) / HOURSTOSECONDS;
    else
       Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient * AverageOxygenProduction
                     * TemperatureArrheniusExponentialLimitation(TempCoefRespiration, WaterTemperature, TminRespiration) / HOURSTOSECONDS;
          //Resp in //mmol O2 / mg Chl / s
    //Debugger(TemperatureArrheniusExponentialLimitation(TempCoefRespiration, WaterTemperature, TminRespiration));
    //Debugger(RespirationCoefficient);
    OxygenNetProduction[MyBoxNumber] = OxygenNetProduction[MyBoxNumber] - Resp * OXYGENATOMICWEIGHT;
    double RespInCarbonUnits;
    RespInCarbonUnits = Resp * OxygenMolecularWeight * CarbonToOxygenProd;
    NPP[MyBoxNumber] = GPP[MyBoxNumber] - RespInCarbonUnits;
    Resp = RespInCarbonUnits * IceAlgaeChl[MyBoxNumber];
          //Resp in //mg C /m2 / s
    //Debugger(Resp);

    IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] - Resp;


    if (MySeaIcePointer != NULL)
    {
       MySeaIcePointer->Update(GetEcoDynClassName(), OxygenNetProduction[MyBoxNumber] / IceAlgaeLayerThickness[MyBoxNumber] / CUBIC / BrineFractionalVolume, MyBoxNumber,"BrineOxygenIce", ObjectCode);//Return as mg O2 / L / s
    }
}

void TIceAlgae::Exudation(int ABoxNumber)
{
    double  Exudate, ArealDOCLoss;
    int MyBoxNumber;
    TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    MyBoxNumber = ABoxNumber;
    if (GPP[MyBoxNumber] > 0.0)
       Exudate = GPP[MyBoxNumber] * DocStressLoss;
    else Exudate = 0.0;
    ArealDOCLoss = Exudate * IceAlgaeChl[MyBoxNumber];   //mg C m-2 s-1;
    IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] - ArealDOCLoss;  //mg C m-2 s-1;
    if (MySeaIcePointer != NULL)
       MySeaIcePointer->Update(GetEcoDynClassName(), ArealDOCLoss / IceAlgaeLayerThickness[MyBoxNumber] / CARBONATOMICWEIGHT / BrineFractionalVolume, MyBoxNumber,"BrineDOCIce", ObjectCode);//Return as micromol C / L / s
}

void TIceAlgae::NutrientLimitation(int ABoxNumber)
{
    int i;
    //double NLimitation, PLimitation, SiLimitation;
    i = ABoxNumber;
    TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    if ((MySeaIcePointer != NULL))
    {
      if (strcmp(LimitationType, "Internal") == 0)
      {
          int i = ABoxNumber;
          if (PCellQuota[i] < MinPCellQuota)
             PLimitation[i] = 0.0;
          else
             PLimitation[i] = PCellQuota[i] / (PCellQuota[i] + KPInternal);
          if (NCellQuota[i] < MinNCellQuota)
             NLimitation[i] = 0.0;
          else
             NLimitation[i] = NCellQuota[i] / (NCellQuota[i] + KNInternal);
          if ((PLimitation[i] == 0.0) || (NLimitation[i] == 0.0))
             GPP[i] = 0.0;
          else
             GPP[i] = GPP[i] * MIN(PLimitation[i],NLimitation[i]);
      }
      else
      if (strcmp(LimitationType, "External") == 0)
      {
         {
             double Ammonia, Nitrate, Nitrite, Phosphate, Silicate;
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Ammonia,i,"BrineNH4Ice",   /*micro M*/ ObjectCode);
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrate,i,"BrineNO3Ice",ObjectCode);
             //MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrite,i,"Nitrite",ObjectCode);
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Phosphate,i,"BrinePhosphateIce",ObjectCode);
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Silicate,i,"BrineSilicateIce",ObjectCode);
             NLimitation[i] = Nitrate / (Nitrate + KNO3External) * exp(-Phi * Ammonia) + Ammonia / (Ammonia + KNH4External);
             PLimitation[i] = Phosphate / (Phosphate + KPExternal);
             SiLimitation[i] = Silicate / (Silicate + KSiExternal);
             NPSLimitation[i] = MIN(MIN(NLimitation[i], PLimitation[i]),SiLimitation[i]);
             GPP[i] = GPP[i] * NPSLimitation[i];
         }
      }
   }
}

void TIceAlgae::NitrogenUptake(int ABoxNumber)
{
   int i = ABoxNumber;
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((NCellQuota[i] > TINNY) && (PCellQuota[i] > TINNY) && (MySeaIcePointer != NULL))
   {
      double AmmoniaUpTake, NitrateAndNitriteUptake, NMaxUptakeOfNitrate;
      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Ammonia, Nitrate, Nitrite, AmmoniumUptakeLimitation;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Ammonia,i,"BrineNH4Ice",   /*micro M*/ ObjectCode);
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrate,i,"BrineNO3Ice",ObjectCode);
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrite,i,"BrineNO2Ice",ObjectCode);
      //...
      if (
            (NCellQuota[i] >= MaxNCellQuota) || (NCellQuota[i] / PCellQuota[i] > MaxNPRatio)
         )
         NUptake[i] = 0.0;
      else
      {
         if (Ammonia > TINNY)
         {
                AmmoniumUptakeLimitation = Ammonia / (Ammonia + KNH4) * (1.0 - NCellQuota[i] / MaxNCellQuota); //dimensionless
      	        AmmoniaUpTake =  NMaxUptake *  AmmoniumUptakeLimitation * IceAlgaeN[i];  //mg N m-2 d-1
         }
         else
                AmmoniaUpTake = 0.0;
         NUptake[i] = AmmoniaUpTake;  //mg N m-2 d-1
         NMaxUptakeOfNitrate = MAX(0.0, NMaxUptake - NMaxUptake * AmmoniumUptakeLimitation); //mg N m-2 d-1
         //No caso de a classe ser invocada a partir do EcoDynamo...
         MySeaIcePointer->Update(GetEcoDynClassName(), -AmmoniaUpTake / NITROGENATOMICWEIGHT / IceAlgaeLayerThickness[i] / DAYSTOSECONDS / BrineFractionalVolume/*microM/L/s*/,i,"BrineNH4Ice",ObjectCode);
         //...
         if (Nitrate + Nitrite > TINNY)
                NitrateAndNitriteUptake = /*NMaxUptake*/NMaxUptakeOfNitrate * (Nitrate + Nitrite) / ((Nitrate + Nitrite) + KNO3) *
                                  (1.0 - NCellQuota[i] / MaxNCellQuota) * IceAlgaeN[i]; //mg N m-2 d-1
         else
                NitrateAndNitriteUptake = 0.0;
         NUptake[i] = NUptake[i] + NitrateAndNitriteUptake;

         if ((NitrateAndNitriteUptake > 0.0) && (IceAlgaeLayerThickness[i] > TINNY) && (BrineFractionalVolume > TINNY))
         {
            MySeaIcePointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrate / (Nitrate + Nitrite) / NITROGENATOMICWEIGHT /
                                    IceAlgaeLayerThickness[i]/ DAYSTOSECONDS / BrineFractionalVolume/*microM/L/s*/,i,"BrineNO3Ice",ObjectCode);
            MySeaIcePointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrite / (Nitrate + Nitrite) / NITROGENATOMICWEIGHT /
                                    IceAlgaeLayerThickness[i]/ DAYSTOSECONDS / BrineFractionalVolume/*microM/L/s*/,i,"BrineNO2Ice",ObjectCode);
         }
          //...
      }
   }
   else
      NUptake[i] = 0.0;
   NCellFlux[i] = NCellFlux[i] + NUptake[i] / DAYSTOSECONDS;   //mg N m-2 s-1

}

void TIceAlgae::PhosphorusUptake(int ABoxNumber)
{
   int i = ABoxNumber;
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((NCellQuota[i] > TINNY) && (PCellQuota[i] > TINNY) && (MySeaIcePointer != NULL))
   {

      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Phosphate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Phosphate,i,"BrinePhosphateIce",ObjectCode);
      //...
      if (
            (PCellQuota[i] > MaxPCellQuota) ||
            (NCellQuota[i] / PCellQuota[i] <= MinNPRatio)
         )
         PUptake[i] = 0.0;
      else
      {
         if (Phosphate > TINNY)
                PUptake[i] = PMaxUptake *  Phosphate / (Phosphate + KP) * (1.0 - PCellQuota[i] / MaxPCellQuota) * IceAlgaeP[i];
         else
                PUptake[i] = 0.0;
         //No caso de a classe ser invocada a partir do EcoDynamo...
         if ((IceAlgaeLayerThickness[i] > TINNY) && (BrineFractionalVolume > TINNY))
            MySeaIcePointer->Update(GetEcoDynClassName(), -PUptake[i] / DAYSTOSECONDS / PHOSPHORUSATOMICWEIGHT / IceAlgaeLayerThickness[i] / BrineFractionalVolume,i,"BrinePhosphateIce",ObjectCode);
         //...
      }
   }
   else
      PUptake[i] = 0.0;
   PCellFlux[i] = PCellFlux[i] + PUptake[i] / DAYSTOSECONDS;  //mg P m-2 s-1
}

void TIceAlgae::SilicaUptake(int ABoxNumber)
{
   int i = ABoxNumber;
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((NCellQuota[i] > TINNY) && (PCellQuota[i] > TINNY) && (MySeaIcePointer != NULL))
   {

      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Silicate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Silicate,i,"BrineSilicateIce",ObjectCode);
      //...
      if (
            (SiCellQuota[i] > MaxPCellQuota) ||
            (NCellQuota[i] / SiCellQuota[i] <= MinNSiRatio)
         )
         SiUptake[i] = 0.0;
      else
      {
         if (Silicate > TINNY)
                SiUptake[i] = SiMaxUptake *  Silicate / (Silicate + KSi) * (1.0 - SiCellQuota[i] / MaxSiCellQuota) * IceAlgaeSi[i];
         else
                SiUptake[i] = 0.0;
         //No caso de a classe ser invocada a partir do EcoDynamo...
         if ((IceAlgaeLayerThickness[i] > TINNY) && (BrineFractionalVolume > TINNY))
            MySeaIcePointer->Update(GetEcoDynClassName(), -SiUptake[i] / DAYSTOSECONDS / SILICAATOMICWEIGHT / IceAlgaeLayerThickness[i] / BrineFractionalVolume, i,"BrineSilicateIce",ObjectCode);
         //...
      }
   }
   else
      SiUptake[i] = 0.0;
   SiCellFlux[i] = SiCellFlux[i] + SiUptake[i] / DAYSTOSECONDS;  //mg P m-2 s-1
}

void TIceAlgae::SalinityLimitation(int ABoxNumber)   //According with equation 36 of Arrigo et al (1993) - Journal of Geophysical Research 98: 6929-6946
{
   int MyBoxNumber = ABoxNumber;
   double MySalinity;
   double const alfa1 = 0.011, alfa2 = 0.03012, alfa3 = 0.0010342,
                alfa4 = -4.6033e-5, alfa5 = 4.926e-7, alfa6 = -1.659e-9;
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if (MySeaIcePointer != NULL)
   {
      MySeaIcePointer->Inquiry(GetEcoDynClassName(),MySalinity,MyBoxNumber,"BrineSalinityIce",ObjectCode);
      SaltLimitation[MyBoxNumber] = alfa1 + alfa2 * MySalinity + alfa3 * pow(MySalinity, 2.0) + alfa4 * pow(MySalinity, 3.0) +
                                    alfa5 * pow(MySalinity, 4.0) + alfa6 * pow(MySalinity, 5.0);
      SaltLimitation[MyBoxNumber] = MAX(SaltLimitation[MyBoxNumber],0.0);  //To prevent negative values
      SaltLimitation[MyBoxNumber] = MIN(SaltLimitation[MyBoxNumber],1.0);  //To prevent values slightly above 1

   }
   else
      SaltLimitation[MyBoxNumber] = 1.0;
   GPP[MyBoxNumber] = GPP[MyBoxNumber] * SaltLimitation[MyBoxNumber];
}

void TIceAlgae::IceSeaTransfer(int ABoxNumber)
{
   int MyBoxNumber = ABoxNumber, Index, UpperLayer, MyLine, MyColumn;
   double MyIceGrowthRate, MyBrineFlux, MyTemperature, MyDepth, AValue;
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer(),
               * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
               * MyPhytoplanktonPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
               * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer(),
               * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   MyColumn = GetColumnIndex(MyBoxNumber);
   MyLine = GetLineIndex(MyBoxNumber);
   if (MyTransportPointer != NULL)
   {
      MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, 0,"UpperLayer",ObjectCode);
      UpperLayer = ceil(AValue);
      Index = Get3DIndex(MyLine,MyColumn,UpperLayer);
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth, Index,"Box depth",ObjectCode);
   }
   else
   {
      Index = Get3DIndex(MyLine,MyColumn,MyPEcoDynClass->GetNumberOfLayers()-1);
      MyDepth = MyPEcoDynClass->GetBoxDepth(Index);
   }
   //Debugger(MyDepth);
   if ((NCellQuota[MyBoxNumber] > TINNY) && (PCellQuota[MyBoxNumber] > TINNY) && (MySeaIcePointer != NULL) && (MyWaterTemperaturePointer != NULL))
   {
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceGrowthRate,MyBoxNumber,"IceGrowthRate",ObjectCode);
      MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyTemperature,MyBoxNumber,"Water temperature",ObjectCode);
      if ((MyIceGrowthRate < 0.0) && (MyTemperature > FreezingPointOfSeawater))
      {
         MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyBrineFlux,MyBoxNumber,"Brine flux",ObjectCode);
         if (IceAlgaeLayerThickness[MyBoxNumber] > TINNY)
         {
            IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] + MyIceGrowthRate / IceAlgaeLayerThickness[MyBoxNumber] * IceAlgaeC[MyBoxNumber];
            IceAlgaeNFlux[MyBoxNumber] = IceAlgaeNFlux[MyBoxNumber] + MyIceGrowthRate /IceAlgaeLayerThickness[MyBoxNumber] * IceAlgaeN[MyBoxNumber];
            IceAlgaePFlux[MyBoxNumber] = IceAlgaePFlux[MyBoxNumber] + MyIceGrowthRate / IceAlgaeLayerThickness[MyBoxNumber] * IceAlgaeP[MyBoxNumber];
            IceAlgaeSiFlux[MyBoxNumber] = IceAlgaeSiFlux[MyBoxNumber] + MyIceGrowthRate / IceAlgaeLayerThickness[MyBoxNumber] * IceAlgaeSi[MyBoxNumber];
         }
         if ((MySPMPointer != NULL) && (MyDepth > TINNY))
         {
            MySPMPointer->Update(GetEcoDynClassName(), IceAlgaeCFlux[MyBoxNumber] / MyDepth / CUBIC/*mg L-1 s-1*/, MyBoxNumber,"LiveC",ObjectCode);
            MySPMPointer->Update(GetEcoDynClassName(), IceAlgaeNFlux[MyBoxNumber] / MyDepth / CUBIC/*mg L-1 s-1*/, MyBoxNumber,"LiveN",ObjectCode);
            MySPMPointer->Update(GetEcoDynClassName(), IceAlgaePFlux[MyBoxNumber] / MyDepth / CUBIC/*mg L-1 s-1*/, MyBoxNumber,"LiveP",ObjectCode);
            MySPMPointer->Update(GetEcoDynClassName(), IceAlgaeSiFlux[MyBoxNumber] / MyDepth / CUBIC/*mg L-1 s-1*/, MyBoxNumber,"LiveSi",ObjectCode);
         }
         if ((MyPhytoplanktonPointer != NULL) && (MyDepth > TINNY))
         {
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), IceAlgaeCFlux[MyBoxNumber] / MyDepth/*mg m-3 s-1*/, MyBoxNumber,"Phytoplankton biomass",ObjectCode);
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), IceAlgaeNFlux[MyBoxNumber] / MyDepth/*mg m-3 s-1*/, MyBoxNumber,"Phytoplankton N biomass",ObjectCode);
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), IceAlgaePFlux[MyBoxNumber] / MyDepth/*mg m-3 s-1*/, MyBoxNumber,"Phytoplankton P biomass",ObjectCode);
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), IceAlgaeSiFlux[MyBoxNumber] / MyDepth/*mg m-3 s-1*/, MyBoxNumber,"Phytoplankton Si biomass",ObjectCode);
         }
      }
   }
}

double TIceAlgae::TemperatureArrheniusExponentialLimitation(double TemperatureAugmentationRate, double ATemperature, double AReferenceTemperature)
{
    //Default values according to Paul Tett (see reference below)
   //TemperatureAugmentationRate = 0.069ºC-1, AReferenceTemperature = 20 ºC
   //Default values according to Jin et al (see reference below)
   //TemperatureAugmentationRate = 0.0633ºC-1 for photosynthesis and respiration and 0.03 for motality, AReferenceTemperature = 0 ºC
   return exp(TemperatureAugmentationRate * (ATemperature - AReferenceTemperature));
   //Tett, P., 2000. Parameterising a microplankton model.
   //Department of Biological Sciences, Napier University,
   // Edinburgh EH10 5DT.
   //Jin, M., et al., 2006. Controls of the landfast ice–ocean ecosystem offshore
   //Barrow, Alaska. Annals of Glaciology 44: 63-72.
   //Jin, M., et al., 2006. A coupled ice-ocean ecosystem model for I-D and 3-D applications
   //in the Bering and Chukchi Seas. Chinese Journal of Polar Science, Vol. 19, No.2, 218 - 229.
}

bool TIceAlgae::SaveVariables()
{
    TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("IceAlgae");
    if (PReadWrite == NULL) return false;
    //header file
    PReadWrite->WriteCell("Ice algae");
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

void TIceAlgae::Mortality(int ABoxNumber)
{
   double Mortality;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   Mortality = MortalityAtZeroDegres * TemperatureArrheniusExponentialLimitation(TempCoefMortality,WaterTemperature, TminMortality) / HOURSTOSECONDS;
   IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] - Mortality * IceAlgaeC[MyBoxNumber];
   IceAlgaeNFlux[MyBoxNumber] = IceAlgaeNFlux[MyBoxNumber] - Mortality * IceAlgaeN[MyBoxNumber];
   IceAlgaePFlux[MyBoxNumber] = IceAlgaePFlux[MyBoxNumber] - Mortality * IceAlgaeP[MyBoxNumber];
   IceAlgaeSiFlux[MyBoxNumber] = IceAlgaeSiFlux[MyBoxNumber] - Mortality * IceAlgaeSi[MyBoxNumber];
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((MySeaIcePointer != NULL) && (IceAlgaeLayerThickness[MyBoxNumber] > TINNY) && (BrineFractionalVolume > TINNY))
   {
      MySeaIcePointer->Update(GetEcoDynClassName(), IceAlgaeCFlux[MyBoxNumber] / IceAlgaeLayerThickness[MyBoxNumber] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, MyBoxNumber,"BrineDetritalIceC",ObjectCode);
      MySeaIcePointer->Update(GetEcoDynClassName(), IceAlgaeNFlux[MyBoxNumber] / IceAlgaeLayerThickness[MyBoxNumber] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, MyBoxNumber,"BrineDetritalIceN",ObjectCode);
      MySeaIcePointer->Update(GetEcoDynClassName(), IceAlgaePFlux[MyBoxNumber] / IceAlgaeLayerThickness[MyBoxNumber] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, MyBoxNumber,"BrineDetritalIceP",ObjectCode);
      MySeaIcePointer->Update(GetEcoDynClassName(), IceAlgaeSiFlux[MyBoxNumber] / IceAlgaeLayerThickness[MyBoxNumber] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, MyBoxNumber,"BrineDetritalIceSi",ObjectCode);
   }
}




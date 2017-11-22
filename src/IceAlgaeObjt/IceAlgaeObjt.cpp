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
//#include "params.h"
#include "EcoDynClass.h"
#ifndef _PORT_FORTRAN_
	#include "iodll.h"
#endif


#ifdef _PORT_FORTRAN_
/*
 * singleton provider - TPhytoplanktonGeneric class method
 */

void icealgae_test__(long* PIceAlgae)
{

}

TIceAlgae* TIceAlgae::getIceAlgae()
{
   TIceAlgae* PIceAlgae = new TIceAlgae("TIceAlgae");
   return PIceAlgae;
}


TIceAlgae::TIceAlgae(char* className)
        :TProdutor(className)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
}

void icealgae_new__(long* PIceAlgae, int* piCurveOption, char* limitationType, 
                    double* pmax, double* slope, double* beta,double* tminPhotosynthesis,double* tempCoefPhotosynthesis, double* carbonToOxygenProd,
                    double* respirationCoefficient,double* maintenanceRespiration, double* ratioLightDarkRespiration,double* tminRespiration, double* tempCoefRespiration,
                    double* tminMortality, double* tempCoefMortality,double* mortalityAtZeroDegres, 
                    double* maxNPRatio, double* minNPRatio,double* minNSiRatio,double* kNH4, double* kNO3,double* kP,double* kSi, 
                    double* nMaxUptake, double* pMaxUptake, double* siMaxUptake,double* kNInternal,double* kPInternal,double* kSiInternal,  
                    double* kNO3External,double* kNH4External,double* kPExternal, double* kSiExternal, double* phi,
                    double* docStressLoss,
                    double* seeding, double* integrationSteps,double* nutLimitation,double*salLimitation,double*tempLimitation,
                    double* maxNCellQuota,double* maxPCellQuota,double* maxSiCellQuota,
                    double* minNCellQuota,double* minPCellQuota,double* minSiCellQuota,
                    double* lightThreshold, double* winterResp)
{
        TIceAlgae* ptr;
        //***Grid dimensions set to "ones" because this is for usage of EcoDynamo functions from ROMS (or other software) and the functions remain agnostic about grid details***/
        ptr = TIceAlgae::getIceAlgae();
        *PIceAlgae = (long)ptr;
        ptr->SetNumberOfLines(1);
        ptr->SetNumberOfColumns(1);
        ptr->SetNumberOfLayers(1);
        ptr->SetNumberOfBoxes(1); 
        ptr->SetNumberOfBoxesWithIce(1);
        ptr->SetNumberOfIceClasses(1);
        /*********************************************Variable declaration and zero initialization and parameter declaration and initialization*********************************/
        ptr->InitializeVariables("TIceAlgae");
        /****************************************************************Parameters values imported from Fortran program used to redefine intial parameter value****************/
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*pmax,0,"Pmax");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*slope,0,"Slope");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*beta,0,"beta");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tminPhotosynthesis,0,"TminPhotosynthesis");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tempCoefPhotosynthesis,0,"TempCoefPhotosynthesis");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*carbonToOxygenProd,0,"CarbonToOxygenProd");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*respirationCoefficient,0,"RespirationCoefficient");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*maintenanceRespiration,0,"R0");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*ratioLightDarkRespiration,0,"RatioLightDarkRespiratio");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tminRespiration,0,"TminRespiration");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tempCoefRespiration,0,"TempCoefRespiration");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tminMortality,0,"TminMortality");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tempCoefMortality,0,"TempCoefMortality"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*mortalityAtZeroDegres,0,"MortalityAtZeroDegres"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*maxNPRatio,0,"MaxNPRatio"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*minNPRatio,0,"MinNPRatio"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*minNSiRatio,0,"MinNSiRatio"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kNH4,0,"KNH4"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kNH4,0,"KNO3"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kP,0,"KP"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kSi,0,"KSi"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*nMaxUptake,0,"NMaxUptake"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*pMaxUptake,0,"PMaxUptake"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*siMaxUptake,0,"SiMaxUptake"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kNInternal,0,"KNInternal"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kPInternal,0,"KPInternal");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kSiInternal,0,"KSiInternal");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kNO3External,0,"KNO3External");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kPExternal,0,"KPExternal");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kSiExternal,0,"KSiExternal");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*phi,0,"Phi"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*docStressLoss,0,"DocStressLoss");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*seeding,0,"Seeding");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*integrationSteps,0,"IntegrationSteps"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*nutLimitation,0,"NutLimitation"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*salLimitation,0,"SalLimitation"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*tempLimitation,0,"TempLimitation"); 
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*maxNCellQuota,0,"MaxNCellQuota");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*maxPCellQuota,0,"MaxPCellQuota");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*maxSiCellQuota,0,"MaxSiCellQuota");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*minNCellQuota,0,"MinNCellQuota");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*minPCellQuota,0,"MinPCellQuota");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*minSiCellQuota,0,"MinSiCellQuota");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*lightThreshold,0,"LightThreshold");
        ptr->SetVariableValue(ptr->GetEcoDynClassName(),*winterResp,0,"WinterResp");
}
//-----------------------------------------------
void icealgae_go__(long* PiceAlgae, double* layerThickness, double* timeStep)
{
   TIceAlgae* ptr = (TIceAlgae*) *PiceAlgae;
   /********Time step is reseted every "go". This is not necessary for CICE but it may become necessary for other variable time step application******************/
   ptr->SetTimeStep(*timeStep);
   /*********************SetABoxDepth resets layer thickness. This is mostly necessary for surface layers*8******************************************************/
   ptr->SetABoxDepth(*layerThickness);
   /********************************************Resetes box number to zero....just in case....*******************************************************************/
   ptr->SetABoxNumber(0);
   //cout<< "Pmax= "<< ptr->Pmax[0] << endl;
}
//-----------------------------------------------
void icealgae_production__(long* PIceAlgae, double* lightAtTop, double* lightAtBottom, double* kValue,double* iceTemperature,
                               int* piCurveOption, double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* biomass, 
                               double *TIC, double *Slope, double* Chl2Carbon, double *OxygenProduction)
{
   double Productivity, MyBiomass, MyNPhyto, MyPPhyto, MyNCellQuota, MyPCellQuota, MyChl2Carbon, FromChl2Carbon, CarbonOxygenRatio;
   double Tmin, TCoef;
   TIceAlgae* ptr = (TIceAlgae*) *PIceAlgae;
   /**********************PAR light intensity reseting at the the topo and the bottom of a model cell**************************************************************/
   ptr->SetLightAtTop(*lightAtTop);
   ptr->SetLightAtBottom(*lightAtBottom);
   /********************************************Reseting of the light extinction coefficient for proper primary production vertical integration********************/
   ptr->SetVariableValue(ptr->GetEcoDynClassName(),*kValue,0,"KForIceAlgae");
   /***********Reseting of the slope of the P-I curve, just in case this is to be recalculated as a function of chlorophyll:carbon or something else...************/
   ptr->SetVariableValue(ptr->GetEcoDynClassName(),*Slope,0,"Slope");
   /********************************************Reseting of water temperature to account for corresponding limitation of photosynthesis****************************/
   ptr->SetIceTemperature(*iceTemperature);
   /********************************************Conversions from mmol/m3 (ROMS units) to mg / m3 (EcoDynamo units)*************************************************/                            
   MyBiomass = *biomass * CARBONATOMICWEIGHT; 
   MyNPhyto =  *nPhyto * NITROGENATOMICWEIGHT;
   MyPPhyto = *pPhyto * PHOSPHORUSATOMICWEIGHT;
   /*********************************************Reseting of the chlorophyll / carbon ratio************************************************************************/
   MyChl2Carbon = *Chl2Carbon;
   FromChl2Carbon = 1.0 / MyChl2Carbon; 
   /*******************************************Calculation of cell quotas for usage in the calculation of internal nutrient limitation*****************************/ 
   if (MyBiomass > ptr->aMin)
   {
       MyNCellQuota = MyNPhyto / MyBiomass;
       MyPCellQuota = MyPPhyto / MyBiomass; 
   }
   else
   {
       MyNCellQuota = 0.0;
       MyPCellQuota = 0.0;
   }
   /******************************************Reseting of variable values in the phytoplankton pointer in the units of EcoDynamo************************************/
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Ice algae C");
   ptr->SetVariableValue("Fortran", FromChl2Carbon,0,"ChlToCarbon");
   ptr->SetVariableValue("Fortran", MyNCellQuota,0,"NCellQuota");
   ptr->SetVariableValue("Fortran", MyPCellQuota,0,"PCellQuota");
   ptr->SetVariableValue("Fortran", MyNPhyto,0,"Ice algae N");
   ptr->SetVariableValue("Fortran", MyPPhyto,0,"Ice algae P");
   /*****************************************Selection of a P-I function depending on the option "piCurveOption" in the arguments above*****************************/
   switch (*piCurveOption)
   {
      case 1: /*PLATT*/ // add a list item
         ptr->Productivity = ptr->PlattPIFunction(0);
         break;
      case 2: /*STEELE*/ // add a list item
         ptr->Productivity = ptr->SteelePIFunction(0);
         break;
   }
   /***************************************Light limited productivity calculated in mg C / m3**********************************************************************/
   Productivity = ptr->GetParameterValue("Productivity"); 
   /***************************************Calculation of temperature limitation***********************************************************************************/
   Tmin = ptr->GetParameterValue("TminPhotosynthesis");
   TCoef= ptr->GetParameterValue("TempCoefPhotosynthesis");
   Productivity = ptr->GetParameterValue("Productivity") * ptr->TemperatureArrheniusExponentialLimitation(TCoef, *iceTemperature, Tmin);
   ptr->SetParameterValue("Productivity", Productivity);
   //cout<< "Productivity temp limited = "<< Productivity << endl;
   /***************************************Calculation of nutrient limitation**************************************************************************************/
   ptr->NutrientLimitation(0,0,0,0);
   //Productivity = ptr->GetParameterValue("Productivity");
   //cout<< "Productivity nut limited = "<< Productivity << endl;
   /***************************************Productivity recalculated in mmol C/m3/s*******************************************************************************/
   *GrossProduction = ptr->GetParameterValue("Productivity") / CARBONATOMICWEIGHT; //Return value in mmolC/m3/s for compatibility with ROMS
   *Slope = ptr->GetParameterValue("Slope"); //Return value in [s-1/(micro mol photons m-2 s-1)]
   /**************************************This was to calculate average daily production which is not being calculated now****************************************/
    ptr->SetJulianDay(*julianDay);
   //ptr->DailyAverageProduction();
   /*************************************Productivity expressed in oxygen units to feedback oxygen biogeochemical cycling****************************************/ 
   CarbonOxygenRatio = ptr->GetParameterValue("CarbonToOxygenProd");
   *OxygenProduction = ptr->GetParameterValue("Productivity") / CarbonOxygenRatio / (2.0 * OXYGENATOMICWEIGHT); //OxygenProduction in mmol O2 m-3 s-1 
}
//----------------------------------------------- 
#else
// Constructors invoked outside EcoDyn shell...

/*TIceAlgae::TIceAlgae(char* className,
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
}*/
// ...

TIceAlgae::TIceAlgae(TEcoDynClass* APEcoDynClass, char* className): TProdutor(APEcoDynClass, className)
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcoDynClass;
    ObjectCode = ICEALGAEOBJECTCODE;
    NumberOfIceClasses = MyPEcoDynClass->GetNumberOfIceClasses();
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
       NumberOfHorizontalCells = MyPEcoDynClass->GetNumberOfCellsWithIce();
       NumberOfCongelationLayers = MyIceRecord->CongelationIceLayers;
       NumberOfSkeletalLayers = MyIceRecord->SkeletalIceLayers;
       NumberOfPlateletLayers = MyIceRecord->PlateletIceLayers;
       NumberOfIceLayers = NumberOfCongelationLayers + NumberOfSkeletalLayers;
       //Debugger(2);
    }
    else
    {
#ifdef __BORLANDC__
        MessageBox(0, "Ice record missing in TIceAlgae.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TIceAlgae::TIceAlgae - Ice record missing in TIceAlgae." << endl;
#endif  // __BORLANDC__
    }
    NumberOfBoxesWithIce = MyPEcoDynClass->GetIceGridSize();
    InitializeVariables(className);
    ReadVariablesAndParameters(className);
}

#endif  //_PORT_FORTRAN_

void TIceAlgae::InitializeVariables(char* className)
{
    strcpy(EcoDynClassName, className);
#ifndef _PORT_FORTRAN_
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    TimeStep = MyPEcoDynClass->GetTimeStep();

    // Initialise pointers
    if (NumberOfBoxesWithIce > 0)                       // Initialise arrays for variable pairs - one
    {
#endif //_PORT_FORTRAN_
        IceAlgaeChl = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeC = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeN = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeP = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeSi = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        NCellQuota = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        PCellQuota = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        SiCellQuota = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeChlFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeCFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeNFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaePFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeSiFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        KForIceAlgae = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeLayerThickness = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        OxygenNetProduction = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeChlInBox = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeCInBox = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeNInBox = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaePInBox = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        IceAlgaeSiInBox = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        DailyMeanGPP = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        NUptake = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        PUptake = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        SiUptake = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        NCellFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        PCellFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        SiCellFlux = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        NLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        PLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        SiLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        NPSLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        LightLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        TemperatureLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        SaltLimitation = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        ChlToCarbonIceAlgae = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        RespirationRate = (double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        MortalityAtZeroDegres =(double*) calloc(NumberOfBoxesWithIce*NumberOfIceClasses, sizeof(double));
        SnowMeltingCLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        SnowMeltingNLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        SnowMeltingPLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        SnowMeltingSiLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        IceMeltingCLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        IceMeltingNLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        IceMeltingPLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        IceMeltingSiLosseRate =(double*) calloc(NumberOfLines * NumberOfColumns *NumberOfIceClasses, sizeof(double));
        GPP = new double [NumberOfBoxesWithIce*NumberOfIceClasses];
        NPP = new double [NumberOfBoxesWithIce*NumberOfIceClasses];

        Pmax = new double [NumberOfBoxesWithIce*NumberOfIceClasses];
        Slope = new double [NumberOfBoxesWithIce*NumberOfIceClasses];
        beta = new double [NumberOfBoxesWithIce*NumberOfIceClasses];
        Iopt = new double [NumberOfBoxesWithIce*NumberOfIceClasses];
        MaintenanceRespiration = new double [NumberOfBoxesWithIce*NumberOfIceClasses];

        for (int i = 0; i < NumberOfBoxesWithIce*NumberOfIceClasses; i++)
        {
           GPP[i] = 0.0; NPP[i] = 0.0;
           Pmax[i] = 0.0; Slope[i] = 0.0; beta[i] = 0.0; Iopt[i] = 0.0;
           MaintenanceRespiration[i] = 0.0;
        }
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
        NRedfieldQuota = 0.176, PRedfieldQuota = 0.024; SiRedfieldQuota = 0.176;
        CarbonToOxygenResp = 0.0;
        LightAtTop = 0.0;
        LightAtBottom = 0.0;
        WaterTemperature = 0.0;
        IceTemperature = 0.0;
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
        //MortalityAtZeroDegres = 0.000923; //Jin et al (2006) - Annals of Glaciology 44 2006
        KNH4 = 2.94;                      //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KNO3 = 30.0;                      //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KP = 2.0;                         //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        KSi = 30.0;        //Check????????????????
        NMaxUptake = 1.08;                //Duarte et al. (2007) - Transitional Waters Monographs 1: 13-51
        NutrientReleaseRatio = 1.0;       //d-1
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
        IceMeltAmplificationFactor = 1.0;  
        Seeding = 50.0; //mg C / m2  Value assumed as a minimum to restart ice algal growth
        strcpy(PIfunction, "Platt");
        strcpy(LimitationType, "External");
        ADay = MyPEcoDynClass->GetJulianDay();
        NutLimitation = true; SalLimitation = true; TempLimitation = true; TransportInIce = true; ArealBiomassCorrection = false;
        UpperLayer = 0;
        LowerLayerForChlExchange = 0;
        LightThreshold = 0.0;
        WinterResp = 0.0;
        aMin = 0.00000000001;
#ifndef _PORT_FORTRAN_
    }
    else
#endif //_PORT_FORTRAN_
#ifdef __BORLANDC__
				MessageBox(0,"Ice algae object array not dimensioned","EcoDynamo alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TIceAlgae::InitializeVariables - Ice algae object array not dimensioned" << endl;
#endif  // __BORLANDC__

}
#ifndef _PORT_FORTRAN_
void TIceAlgae::ReadVariablesAndParameters(char* className)
{
  
   char MyVariable[65], MyName[65];
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
          if (NumberOfIceClasses > 1)
          {
             //Debugger(NumberOfVariables); Debugger(NumberOfIceClasses);
             BuildClassVariableNames(NumberOfVariables,NumberOfIceClasses);
          }
          for (int i = 0; i < NumberOfVariables; i++)
          {
             //MessageBox(0, VariableNameArray[i], "EcoDynamo alert", MB_OK);
             PReadWrite->ReadString(X+2+i, Y, MyVariable);
             if (strcmp(MyVariable, "Ice algae chl") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, IceAlgaeChl[j]);
                   //if (j == NumberOfBoxesWithIce * NumberOfIceClasses-1) Debugger(IceAlgaeChl[j]);
                }
             }
             if (strcmp(MyVariable, "Ice algae C") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, IceAlgaeC[j]);
                }
             }
             if (strcmp(MyVariable, "Ice algae N") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, IceAlgaeN[j]);
                }
             }
             if (strcmp(MyVariable, "Ice algae P") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, IceAlgaeP[j]);
                }
             }
             if (strcmp(MyVariable, "Ice algae Si") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, IceAlgaeSi[j]);
                }
             }
             if (strcmp(MyVariable, "ChlToCarbon") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, ChlToCarbonIceAlgae[j]);
                   //Debugger(ChlToCarbonIceAlgae[j]);
                }
             }
             if (strcmp(MyVariable, "IceAlgaeLayerThickness") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X+2+i, Y+1+j, IceAlgaeLayerThickness[j]);
                }
                //Debugger(IceAlgaeLayerThickness[NumberOfBoxesWithIce-1]);
             }
          }
          CloseDataFile((void*)PReadWrite);
          NumberOfVariables =  NumberOfVariables * NumberOfIceClasses;
       }
       else
#ifdef __BORLANDC__
          MessageBox(0, "Variables:Undefined object - TIceAlgae", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TIceAlgae::ReadVariablesAndParameters - variables: undefined object TIceAlgae" << endl;
#endif  // __BORLANDC__
   }
   /*for (int j = 0; j < NumberOfBoxesWithIce; j++)
      ChlExtinctionCoefficient(j);*/
   PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("IceAlgae");
   if (PReadWrite == NULL) 
#ifdef __BORLANDC__
			MessageBox(0, "TIceAlgae parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TIceAlgae::ReadVariablesAndParameters - Parameters file missing." << endl;
#endif  // __BORLANDC__
#ifdef __BORLANDC__
   if (PReadWrite == NULL) MessageBox(0, "TIceAlgae parameters file missing.", "EcoDynamo alert", MB_OK);
#endif  // __BORLANDC__
   if (PReadWrite != NULL)
   {
       int X, Y; double AValue;
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
             if (strcmp(MyVariable, "TransportInIce") == 0)
             {
                PReadWrite->ReadNumber(X + i + 2, Y + 1, AValue);
                if (AValue == 0) TransportInIce = false;
                else TransportInIce = true;
             }
             if (strcmp(MyVariable, "Pmax") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1 + j, Pmax[j]);
                }
                //Debugger(Pmax[NumberOfBoxesWithIce-1]);
             }
             if (strcmp(MyVariable, "Slope") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1 + j, Slope[j]);
                }
                //Debugger(Slope[NumberOfBoxesWithIce-1]);
             }
             if (strcmp(MyVariable, "beta") == 0)
             {
                for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
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
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, AValue); //Debugger(AValue);
                   for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                      MaintenanceRespiration[j] = AValue;
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
                   //PReadWrite->ReadNumber(X + i + 2, Y + 1, MortalityAtZeroDegres); //Debugger(MortalityAtZeroDegres);

                   for (int j = 0; j < NumberOfBoxesWithIce * NumberOfIceClasses; j++)
                   {
                      PReadWrite->ReadNumber(X + i + 2, Y + 1 + j, AValue); //Debugger(AValue);
                      MortalityAtZeroDegres[j] = AValue;  //Debugger(MortalityAtZeroDegres[j]);
                   }
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
             if (strcmp(MyVariable, "MaxCCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MaxCCellQuota); //Debugger(MaxCCellQuota);
             }
             if (strcmp(MyVariable, "MaxNCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MaxNCellQuota); //Debugger(MaxNCellQuota);
             }
             if (strcmp(MyVariable, "MaxPCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MaxPCellQuota); //Debugger(MaxPCellQuota);
             }
             if (strcmp(MyVariable, "MaxSiCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MaxSiCellQuota); //Debugger(MaxSiCellQuota);
             }
             if (strcmp(MyVariable, "MinCCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MinCCellQuota); //Debugger(MinCCellQuota);
             }
             if (strcmp(MyVariable, "MinNCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MinNCellQuota); //Debugger(MinNCellQuota);
             }
             if (strcmp(MyVariable, "MinPCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MinPCellQuota); //Debugger(MinPCellQuota);
             }
             if (strcmp(MyVariable, "MinSiCellQuota") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, MinSiCellQuota); //Debugger(MinSiCellQuota);
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
             if (strcmp(MyVariable, "DocLoss") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, DocLoss); //Debugger(DocStressLoss);
             }            
             if (strcmp(MyVariable, "DocStressLoss") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, DocStressLoss); //Debugger(DocStressLoss);
             }
             if (strcmp(MyVariable, "NutrientReleaseRatio") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, NutrientReleaseRatio); //Debugger(NutrientReleaseRatio);
             }
             if (strcmp(MyVariable, "FreezingPointOfSeawater") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, FreezingPointOfSeawater); //Debugger(FreezingPointOfSeawater);
             }
             if (strcmp(MyVariable, "IceMeltAmplificationFactor") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, IceMeltAmplificationFactor); //Debugger(IceMeltAmplificationFactor);
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
                else NutLimitation = false; 
             }
             if (strcmp(MyVariable, "SalLimitation") == 0)
             {
                PReadWrite->ReadNumber(X + i + 2, Y + 1, Value);
                if (Value > 0) SalLimitation = true;
                else SalLimitation = false; 
             }
             if (strcmp(MyVariable, "TempLimitation") == 0)
             {
                PReadWrite->ReadNumber(X + i + 2, Y + 1, Value);
                if (Value > 0) TempLimitation = true;
                else TempLimitation = false;
             }
             if (strcmp(MyVariable, "LowerLayerForChlExchange") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, LowerLayerForChlExchange); //Debugger(LowerLayerForChlExchange);
             }
             if (strcmp(MyVariable, "LightThreshold") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, LightThreshold); //Debugger(LightThreshold);
             }
             if (strcmp(MyVariable, "WinterResp") == 0)
             {
                   PReadWrite->ReadNumber(X + i + 2, Y + 1, WinterResp); //Debugger(WinterResp);
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
#endif //_PORT_FORTRAN_

TIceAlgae::~TIceAlgae()
{
    freeMemory();
    if (NumberOfVariables > 0) delete VariableNameArray;
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
            free(ChlToCarbonIceAlgae);
            free(RespirationRate);
            free(MortalityAtZeroDegres);
            free(SnowMeltingCLosseRate);
            free(SnowMeltingNLosseRate);
            free(SnowMeltingPLosseRate);
            free(SnowMeltingSiLosseRate);
            free(IceMeltingCLosseRate);
            free(IceMeltingNLosseRate);
            free(IceMeltingPLosseRate);
            free(IceMeltingSiLosseRate);
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
#ifdef __BORLANDC__
    IceLayers = NumberOfIceLayers;
#else
    IceLayers = 0;
#endif
    IceVariables = NumberOfVariables;
    MyVarNameArray = new VNA[IceVariables];
    for (int i = 0; i < IceVariables; i++)
    {
        strcpy(MyVarNameArray[i], VariableNameArray[i]);
    }
#ifndef _PORT_FORTRAN_
    LogMessage("Inquiry", srcName, MyVarNameArray[0], IceLayers, IceVariables);
#endif
    return MyVarNameArray;
}


void TIceAlgae::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
   char MyParameter[65], MyName[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   if (NumberOfIceClasses == 1)
   {
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
              Value = NPP[MyBoxNumber] * HOURSTOSECONDS;  //mg C / mg Chl / h
   else if (strcmp(MyParameter, "IceAlgaeGPP") == 0)
              Value = GPP[MyBoxNumber] * HOURSTOSECONDS;  //mg C / mg Chl / h
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
      Value = ChlToCarbonIceAlgae[MyBoxNumber];
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
   else if (strcmp(MyParameter, "NUptake ice") == 0)
      Value = NUptake[MyBoxNumber];
   else if (strcmp(MyParameter, "PUptake ice") == 0)
      Value = PUptake[MyBoxNumber];
   else if (strcmp(MyParameter, "RespirationRate") == 0)
      Value = RespirationRate[MyBoxNumber] * HOURSTOSECONDS; //mmol O2 / mg Chl / h
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
   }
   else
   {
   if (strstr(MyParameter, "Ice algae chl") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae chl %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeChl[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Ice algae C") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae C %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeC[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Ice algae N") != NULL)
   {		
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae N %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeN[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Ice algae P") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae P %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeP[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Ice algae Si") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae Si %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeSi[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Box ice algae chl") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Box ice algae chl %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeChlInBox[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Box ice algae C") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Box ice algae C %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeCInBox[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Box ice algae N") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Box ice algae N %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeNInBox[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Box ice algae P") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Box ice algae P %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaePInBox[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Box ice algae Si") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Box ice algae Si %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeSiInBox[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg / m2
               break;
            }
         }
   }
   else if (strstr(MyParameter, "NCellQuota") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NCellQuota %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = NCellQuota[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "PCellQuota") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "PCellQuota %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = PCellQuota[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "SiCellQuota") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "SiCellQuota %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = SiCellQuota[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "IceAlgaeNPP") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "IceAlgaeNPP %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = NPP[NumberOfBoxesWithIce * Class + MyBoxNumber] * HOURSTOSECONDS;  //mg C / mg Chl / h
               break;
            }
         }
   }
   else if (strstr(MyParameter, "IceAlgaeGPP") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "IceAlgaeGPP %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = GPP[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg C / mg Chl / h
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Pmax") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Pmax %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = Pmax[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg C / mg Chl / h
               break;
            }
         }
   }
   else if (strstr(MyParameter, "beta") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "beta %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = beta[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg C mg Ch1-1 h -1 (microE m -2 s-l) -1
               break;
            }
         }
   }
   else if (strstr(MyParameter, "Slope") != NULL)
   {   
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Slope %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = Slope[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mg C mg Ch1-1 h -1 (microE m -2 s-l) -1
               break;
            }
         }
   }
   else if (strstr(MyParameter, "KForIceAlgae") != NULL)
   {   
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "KForIceAlgae %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = KForIceAlgae[NumberOfBoxesWithIce * Class + MyBoxNumber];   //m-1
               break;
            }
         }
   }
   else if (strstr(MyParameter, "IceAlgaeLayerThickness") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "IceAlgaeLayerThickness %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = IceAlgaeLayerThickness[NumberOfBoxesWithIce * Class + MyBoxNumber];   //m
               break;
            }
         }
   }
   else if (strstr(MyParameter, "DailyMeanGPP") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "DailyMeanGPP %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = DailyMeanGPP[NumberOfBoxesWithIce * Class + MyBoxNumber];   //mg C / mg Chl / h
               break;
            }
         }
   }
   else if (strstr(MyParameter, "ChlToCarbon") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "ChlToCarbon %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = ChlToCarbonIceAlgae[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "NLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = NLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "PLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "PLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = PLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "SiLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "SiLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = SiLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "NPSLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NPSLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = NPSLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "LightLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "LightLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = LightLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "TemperatureLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "TemperatureLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = TemperatureLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "SaltLimitation") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "SaltLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = SaltLimitation[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "NUptake ice") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NUptake ice %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = NUptake[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "PUptake ice") != NULL)
   {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "PUptake ice %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = PUptake[NumberOfBoxesWithIce * Class + MyBoxNumber];
               break;
            }
         }
   }
   else if (strstr(MyParameter, "RespirationRate") != NULL)
   {    
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "RespirationRate %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Value = RespirationRate[NumberOfBoxesWithIce * Class + MyBoxNumber];  //mmol O2 / mg Chl / h
               break;
            }
         }
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
#ifdef __BORLAND
      if (j == NumberOfVariables)
      {
          char Caption[129];
          strcpy(Caption, MyParameter);
          strcat(Caption, " does not exist in ");
          strcat(Caption, GetEcoDynClassName());
          MessageBox(0,
                    Caption,
                    "EcoDynamo Alert - Inquiry 1",
                    MB_OK | MB_ICONHAND);
      }
#endif
   }
   }
#ifndef _PORT_FORTRAN_
   LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
#endif
}

bool TIceAlgae::SetVariableValue(char* srcName, double Value,int BoxNumber,char* VariableName)
{
    char MyParameter[65], MyName[65]; // 64 characters (max) for parameter name
    strcpy(MyParameter, VariableName);
    bool rc = true;
#ifndef _PORT_FORTRAN_
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);
#endif
    if (strstr(MyParameter, "Ice algae chl") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeChl[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae chl %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeChl[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "Ice algae C") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeC[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae C %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeC[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "Ice algae N") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeN[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae N %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeN[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "Ice algae P") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeP[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae P %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeP[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "Ice algae Si") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeSi[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae Si %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeSi[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "NCellQuota") != NULL)
    {
      if (NumberOfIceClasses == 1)
         NCellQuota[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NCellQuota %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               NCellQuota[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "PCellQuota") != NULL)
    {
      if (NumberOfIceClasses == 1)
         PCellQuota[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "PCellQuota %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               PCellQuota[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "SiCellQuota") != NULL)
    {
      if (NumberOfIceClasses == 1)
         SiCellQuota[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "SiCellQuota %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               SiCellQuota[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "IceAlgaeNPP") != NULL)
    {
      if (NumberOfIceClasses == 1)
         NPP[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "IceAlgaeNPP %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               NPP[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "IceAlgaeGPP") != NULL)
    {
      if (NumberOfIceClasses == 1)
         GPP[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "IceAlgaeGPP %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               GPP[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "Pmax") != NULL)
    {
      if (NumberOfIceClasses == 1)
         Pmax[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Pmax %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Pmax[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "beta") != NULL)
    {
      if (NumberOfIceClasses == 1)
         beta[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "beta %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               beta[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "Slope") != NULL)
    {
      if (NumberOfIceClasses == 1)
         Slope[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Slope %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               Slope[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "R0") != NULL)
    {
      if (NumberOfIceClasses == 1)
         Slope[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "R0 %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               MaintenanceRespiration[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "MortalityAtZeroDegres") != NULL)
    {
      if (NumberOfIceClasses == 1)
         Slope[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "MortalityAtZeroDegres %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               MortalityAtZeroDegres[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }

    if (strstr(MyParameter, "CarbonToOxygenProd") != NULL)
       CarbonToOxygenProd = Value;
    if (strstr(MyParameter, "RespirationCoefficient") != NULL)
       RespirationCoefficient = Value;
    if (strstr(MyParameter, "RatioLightDarkRespiration") != NULL)
       RatioLightDarkRespiration = Value;
    if (strstr(MyParameter, "TminPhotosynthesis") != NULL)
       TminPhotosynthesis = Value;
    if (strstr(MyParameter, "TempCoefPhotosynthesis") != NULL)
       TempCoefPhotosynthesis = Value;
    if (strstr(MyParameter, "RatioLightDarkRespiration") != NULL)
       RatioLightDarkRespiration = Value;
    if (strstr(MyParameter, "TminRespiration") != NULL)
       TminRespiration = Value;
    if (strstr(MyParameter, "TempCoefRespiration") != NULL)
       TempCoefRespiration = Value;
    if (strstr(MyParameter, "MaxNPRatio") != NULL)
       MaxNPRatio = Value;
    if (strstr(MyParameter, "MinNPRatio") != NULL)
       MinNPRatio = Value;
    if (strstr(MyParameter, "MinNSiRatio") != NULL)
       MinNSiRatio = Value;
    if (strstr(MyParameter, "KNH4") != NULL)
       KNH4 = Value; 
    if (strstr(MyParameter, "KNO3") != NULL)
       KNO3 = Value; 
    if (strstr(MyParameter, "KP") != NULL)
       KP = Value; 
    if (strstr(MyParameter, "KSi") != NULL)
       KSi = Value; 
    if (strstr(MyParameter, "NMaxUptake") != NULL)
       NMaxUptake = Value;
    if (strstr(MyParameter, "PMaxUptake") != NULL)
       PMaxUptake = Value;
    if (strstr(MyParameter, "SiMaxUptake") != NULL)
       SiMaxUptake = Value;
    if (strstr(MyParameter, "KNInternal") != NULL)
       KNInternal = Value;
    if (strstr(MyParameter, "KPInternal") != NULL)
       KPInternal = Value;
    if (strstr(MyParameter, "KSiInternal") != NULL)
       KSiInternal = Value;
    if (strstr(MyParameter, "KNO3External") != NULL)
       KNO3External = Value;
    if (strstr(MyParameter, "KPExternal") != NULL)
       KPExternal = Value;
    if (strstr(MyParameter, "KSiExternal") != NULL)
       KSiExternal = Value;
    if (strstr(MyParameter, "Phi") != NULL)
       Phi = Value; 
    if (strstr(MyParameter, "DocStressLoss") != NULL)
       DocStressLoss = Value;  
    if (strstr(MyParameter, "Seeding") != NULL)
       Seeding = Value; 
    if (strstr(MyParameter, "IntegrationSteps") != NULL)
       IntegrationSteps = Value; 
    if (strstr(MyParameter, "NutLimitation") != NULL)
       NutLimitation = Value; 
    if (strstr(MyParameter, "SalLimitation") != NULL)
       SalLimitation = Value; 
    if (strstr(MyParameter, "TempLimitation") != NULL)
       TempLimitation = Value;  
    if (strstr(MyParameter, "MaxNCellQuota") != NULL)
       MaxNCellQuota = Value;  
    if (strstr(MyParameter, "MaxPCellQuota") != NULL)
       MaxPCellQuota = Value; 
    if (strstr(MyParameter, "MaxSiCellQuota") != NULL)
       MaxSiCellQuota = Value;  
    if (strstr(MyParameter, "LightThreshold") != NULL)
       LightThreshold = Value;   
    if (strstr(MyParameter, "WinterResp") != NULL)
       WinterResp = Value;   

    if (strstr(MyParameter, "KForIceAlgae") != NULL)
    {
      if (NumberOfIceClasses == 1)
         KForIceAlgae[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "KForIceAlgae %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               KForIceAlgae[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "IceAlgaeLayerThickness") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeLayerThickness[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "IceAlgaeLayerThickness %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeLayerThickness[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "DailyMeanGPP") != NULL)
    {
      if (NumberOfIceClasses == 1)
         DailyMeanGPP[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "DailyMeanGPP %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               DailyMeanGPP[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "ChlToCarbon") != NULL)
    {
      if (NumberOfIceClasses == 1)
         ChlToCarbonIceAlgae[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "ChlToCarbon %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               ChlToCarbonIceAlgae[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "NLimitation") != NULL)
    {
      if (NumberOfIceClasses == 1)
         NLimitation[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               NLimitation[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "PLimitation") != NULL)
    {
      if (NumberOfIceClasses == 1)
         PLimitation[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "PLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               PLimitation[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "SiLimitation") != NULL)
    {
      if (NumberOfIceClasses == 1)
         SiLimitation[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "SiLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               SiLimitation[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "NPSLimitation") != NULL)
    {
      if (NumberOfIceClasses == 1)
         NPSLimitation[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "NPSLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               NPSLimitation[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "LightLimitation") != NULL)
    {
      if (NumberOfIceClasses == 1)
         LightLimitation[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "LightLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               LightLimitation[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    if (strstr(MyParameter, "TemperatureLimitation") != NULL)
    {
      if (NumberOfIceClasses == 1)
         TemperatureLimitation[BoxNumber] = Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "TemperatureLimitation %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               TemperatureLimitation[NumberOfBoxesWithIce * Class + BoxNumber] = Value;
               break;
            }
         }
      }
    }
    else rc = false;
    return rc;
}

void TIceAlgae::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65], MyName[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);
#ifndef _PORT_FORTRAN_
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);
#endif
    if (strstr(MyParameter, "Ice algae chl") != NULL)
    {
      if (NumberOfIceClasses == 1)
         IceAlgaeChlFlux[MyBoxNumber] = IceAlgaeChlFlux[MyBoxNumber] + Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae chl %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeChlFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] = IceAlgaeChlFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] + Value;
               break;
            }
         }
      }
    }
    else if (strstr(MyParameter, "Ice algae C") != NULL)
    {
      if (NumberOfIceClasses == 1)
            IceAlgaeCFlux[MyBoxNumber] = IceAlgaeCFlux[MyBoxNumber] + Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae C %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeCFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] = IceAlgaeCFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] + Value;
               break;
            }
         }
      }
    }
    else if (strstr(MyParameter, "Ice algae N") != NULL)
    {
      if (NumberOfIceClasses == 1)
            IceAlgaeNFlux[MyBoxNumber] = IceAlgaeNFlux[MyBoxNumber] + Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae N %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeNFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] = IceAlgaeNFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] + Value;
               break;
            }
         }
      }
    }
    else if (strstr(MyParameter, "Ice algae P") != NULL)
    {
      if (NumberOfIceClasses == 1)
            IceAlgaePFlux[MyBoxNumber] = IceAlgaePFlux[MyBoxNumber] + Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae P %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaePFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] = IceAlgaePFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] + Value;
               break;
            }
         }
      }
    }
    else if (strstr(MyParameter, "Ice algae Si") != NULL)
    {
      if (NumberOfIceClasses == 1)
            IceAlgaeSiFlux[MyBoxNumber] = IceAlgaeSiFlux[MyBoxNumber] + Value;
      else
      {
         for (int Class = 0; Class < NumberOfIceClasses; Class++)
         {
            sprintf(MyName, "Ice algae Si %i", Class + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
               IceAlgaeSiFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] = IceAlgaeSiFlux[NumberOfBoxesWithIce * Class + MyBoxNumber] + Value;
               break;
            }
         }
      }
    }
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



void TIceAlgae::Integrate()
{
#ifndef _PORT_FORTRAN_
    Integration(IceAlgaeChl, IceAlgaeChlFlux, IceAlgaeC, IceAlgaeCFlux, IceAlgaeN, IceAlgaeNFlux, IceAlgaeP, IceAlgaePFlux, IceAlgaeSi, IceAlgaeSiFlux, NumberOfBoxesWithIce*NumberOfIceClasses);
    TEcoDynClass*  MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    int index, BottomIndex, Index3D, Index2D; double IceGrowth, LayerThickness;
    double MyMeltingSnowFlux,SnowMeltingRate, MyMeltingSwitch, MyIceGrowth, IceMeltingRate, MyIceRelativeArea;
    char MyName[65];
    //if ((MyPEcoDynClass->GetJulianDay() > 91) && (MyPEcoDynClass->GetCurrTime()>4.13)) Debugger(11);

    if (MySeaIcePointer != NULL)
    {
       for (int Class = 0; Class < NumberOfIceClasses; Class++)
       {
       for (int i = 0; i < NumberOfLines; i++)
       {
           for (int j = 0; j < NumberOfColumns; j++)
           {
              for (int k = 0; k < NumberOfIceLayers; k++)   //Conversion from M L-2 to M L-3
              {
                 if (k == 0)
                 {
                    //Debugger(IceAlgaeC[Get3DIndex(i,j,k)]);
                    Index3D = GetIceIndex(i,j,0, Class);
                    Index2D = Get3DIndex(i,j,0) + Class * NumberOfLines * NumberOfColumns;
                    if (NumberOfIceClasses > 1) sprintf(MyName, "MeltingSwitch %i", Class + 1);
                    else strcpy(MyName, "MeltingSwitch");
                    MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyMeltingSwitch,GetIceIndex(i,j,k,0),MyName,ObjectCode);
                    if ((MyMeltingSwitch == 1.0) && (IceAlgaeLayerThickness[Index3D] > TINNY))
                    {
                       if (NumberOfIceClasses > 1)sprintf(MyName, "IceGrowthRate %i", Class + 1);
                       else strcpy(MyName, "IceGrowthRate");
                       MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceGrowth,GetIceIndex(i,j,k,0),MyName,ObjectCode);
                       //Debugger(MyIceGrowth);
                       //This fluxes are used in the next Go to update water column variables from ice losses
                       IceMeltingRate = MyIceGrowth / IceAlgaeLayerThickness[Index3D] * IceMeltAmplificationFactor;  //s-1
                       //The multiplication by minus in the four following lines is to nulify the minus sign of Ice growth during melting
                       //and to ensure that these fluxes are positive in the Update of Phytoplankton and Suspended matter
                       IceMeltingCLosseRate[Index2D] = -IceMeltingRate * IceAlgaeC[Index2D]; //mg C / m2 / s-1
                       IceMeltingNLosseRate[Index2D] = -IceMeltingRate * IceAlgaeN[Index2D]; //mg C / m2 / s-1
                       IceMeltingPLosseRate[Index2D] = -IceMeltingRate * IceAlgaeP[Index2D]; //mg C / m2 / s-1
                       IceMeltingSiLosseRate[Index2D] = -IceMeltingRate * IceAlgaeSi[Index2D]; //mg C / m2 / s-1
                    }
                    else
                    {
                       IceMeltingCLosseRate[Index2D] = 0.0; IceMeltingNLosseRate[Index2D] = 0.0;
                       IceMeltingPLosseRate[Index2D] = 0.0; IceMeltingSiLosseRate[Index2D] = 0.0;
                    }
                    if (NumberOfIceClasses > 1)sprintf(MyName, "MeltedSnowFlux %i", Class + 1);
                    else strcpy(MyName, "MeltedSnowFlux");
                    MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyMeltingSnowFlux,GetIceIndex(i,j,k,0),MyName,ObjectCode); //Debugger(MyMeltingSnowFlux);
                    if (NumberOfIceClasses > 1)sprintf(MyName, "SIarea %i", Class + 1);
                    else strcpy(MyName, "SIarea");
                    MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceRelativeArea, GetIceIndex(i,j,0,0),MyName, ObjectCode);
                    if ((MyMeltingSnowFlux > 0.0) && (IceAlgaeLayerThickness[Index3D] > TINNY) && (MyIceRelativeArea > TINNY))
                    {
                       //This fluxes are used in the next Go to update water column variables from ice losses
                       /*if (NumberOfIceClasses > 1)sprintf(MyName, "SIarea %i", Class + 1);
                       else strcpy(MyName, "SIarea");
                       MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceRelativeArea, GetIceIndex(i,j,0,Class),MyName, ObjectCode);  */
                       //SnowMeltingRate = MyMeltingSnowFlux / MyPEcoDynClass->GetBoxArea(Get3DIndex(i,j,0)) / IceAlgaeLayerThickness[Index3D]; //s-1
                       SnowMeltingRate = MyMeltingSnowFlux / (MyPEcoDynClass->GetBoxArea(Get3DIndex(i,j,0)) * MyIceRelativeArea) / IceAlgaeLayerThickness[Index3D]; //s-1
                       SnowMeltingCLosseRate[Index2D] = SnowMeltingRate * IceAlgaeC[Index2D]; //mg C / m2 / s-1
                       SnowMeltingNLosseRate[Index2D] = SnowMeltingRate * IceAlgaeN[Index2D]; //mg C / m2 / s-1
                       SnowMeltingPLosseRate[Index2D] = SnowMeltingRate * IceAlgaeP[Index2D]; //mg C / m2 / s-1
                       SnowMeltingSiLosseRate[Index2D] = SnowMeltingRate * IceAlgaeSi[Index2D]; //mg C / m2 / s-1
                    }
                    else
                    {
                       SnowMeltingCLosseRate[Index2D] = 0.0; SnowMeltingNLosseRate[Index2D] = 0.0;
                       SnowMeltingPLosseRate[Index2D] = 0.0; SnowMeltingSiLosseRate[Index2D] = 0.0;
                    }

                 }
                 Index3D = GetIceIndex(i, j, k, Class);
                 if (k > 0) //Bottom layer thickness is constant
                 {
                    if (NumberOfIceClasses > 1)sprintf(MyName, "Layer thickness %i", Class + 1);
                    else strcpy(MyName, "Layer thickness");
                    MySeaIcePointer->Inquiry(GetEcoDynClassName(), IceAlgaeLayerThickness[Index3D], GetIceIndex(i,j,k,0),MyName, ObjectCode);
                 }
                 /*if ((i == 15) && (j == 0))
                 {
                      Debugger(k); Debugger(Class);
                      Debugger(Index3D);
                      Debugger(IceAlgaeLayerThickness[Index3D]);
                      Debugger(IceAlgaeChl[Index3D]);
                 } */
                 if (IceAlgaeLayerThickness[Index3D] > TINNY)    //Conversion from M L-2 to M L-3
                 {
                    IceAlgaeChl[Index3D] = IceAlgaeChl[Index3D] / IceAlgaeLayerThickness[Index3D];    //mg / m3
                    IceAlgaeC[Index3D] = IceAlgaeC[Index3D] / IceAlgaeLayerThickness[Index3D];
                    IceAlgaeN[Index3D] = IceAlgaeN[Index3D] / IceAlgaeLayerThickness[Index3D];
                    IceAlgaeP[Index3D] = IceAlgaeP[Index3D] / IceAlgaeLayerThickness[Index3D];
                    IceAlgaeSi[Index3D] = IceAlgaeSi[Index3D] / IceAlgaeLayerThickness[Index3D];
                 }
                 //else MessageBox(0, "Division by zero in TIceAlgae::Integrate.", "EcoDynamo alert", MB_OK);
                 else
                 {
                    //MessageBox(0, "Division by zero in TIceAlgae::Integrate.", "EcoDynamo alert", MB_OK);
                    IceAlgaeChl[Index3D] = 0.0;    //mg / m3
                    IceAlgaeC[Index3D] = 0.0;
                    IceAlgaeN[Index3D] = 0.0;
                    IceAlgaeP[Index3D] = 0.0;
                    IceAlgaeSi[Index3D] = 0.0;
                 }
              }
              if (TransportInIce)
              {
                  //MySeaIcePointer->UpdateLayerProperties(i,j,LowerLayerForChlExchange,IceAlgaeChl);
                  //MySeaIcePointer->UpdateLayerProperties(i,j,LowerLayerForChlExchange,IceAlgaeC);
                  //MySeaIcePointer->UpdateLayerProperties(i,j,LowerLayerForChlExchange,IceAlgaeN);
                  //MySeaIcePointer->UpdateLayerProperties(i,j,LowerLayerForChlExchange,IceAlgaeP);
                  //MySeaIcePointer->UpdateLayerProperties(i,j,LowerLayerForChlExchange,IceAlgaeSi);

                  //Debugger(IceAlgaeC[Get3DIndex(i,j,0)]);

                  for (int Class = 0; Class < NumberOfIceClasses; Class++)
                  {
                     MySeaIcePointer->UpdateLayerPropertiesForPhytoplankton(i,j,Class,IceAlgaeChl);
                     MySeaIcePointer->UpdateLayerPropertiesForPhytoplankton(i,j,Class,IceAlgaeC);
                     //if (MyPEcoDynClass->GetJulianDay() == 173) Debugger(173);
                     MySeaIcePointer->UpdateLayerPropertiesForPhytoplankton(i,j,Class,IceAlgaeN);
                     MySeaIcePointer->UpdateLayerPropertiesForPhytoplankton(i,j,Class,IceAlgaeP);
                     MySeaIcePointer->UpdateLayerPropertiesForPhytoplankton(i,j,Class,IceAlgaeSi);
                  }

                  //Debugger(IceAlgaeC[Get3DIndex(i,j,0)]);
              }
              //Here the above losses are integrated in the ice algal variables but they are integrated in the water column variables of suspended matter
              //and phytoplankton only during the next Go, when SeaIceTransfer is called. THis integration is done using the SnowMeltingLosses calculated above.
             //Debugger(IceAlgaeChl[0]);

              MySeaIcePointer->SnowMeltTransport(i,j,IceAlgaeChl,0.0);
              MySeaIcePointer->SnowMeltTransport(i,j,IceAlgaeC,0.0);
              MySeaIcePointer->SnowMeltTransport(i,j,IceAlgaeN,0.0);
              MySeaIcePointer->SnowMeltTransport(i,j,IceAlgaeP,0.0);
              MySeaIcePointer->SnowMeltTransport(i,j,IceAlgaeSi,0.0);

              //Debugger(IceAlgaeC[Get3DIndex(i,j,0)]);

              for (int k = 0; k < NumberOfIceLayers; k++)  //Conversion from M L-3 to M L-2
              {
                 Index3D = GetIceIndex(i,j,k,Class);
                 IceAlgaeChl[Index3D] = IceAlgaeChl[Index3D] * IceAlgaeLayerThickness[Index3D];
                 IceAlgaeC[Index3D] = IceAlgaeC[Index3D] * IceAlgaeLayerThickness[Index3D];
                 IceAlgaeN[Index3D] = IceAlgaeN[Index3D] * IceAlgaeLayerThickness[Index3D];
                 IceAlgaeP[Index3D] = IceAlgaeP[Index3D] * IceAlgaeLayerThickness[Index3D];
                 IceAlgaeSi[Index3D] = IceAlgaeSi[Index3D] * IceAlgaeLayerThickness[Index3D];
              }

              //Debugger(IceAlgaeC[Get3DIndex(i,j,0)]);
           }
       }
       }
    }
    //if ((MyPEcoDynClass->GetJulianDay() > 91) && (MyPEcoDynClass->GetCurrTime()>4.13)) Debugger(12);
    if (MySeaIcePointer != NULL)
    {
      double TotalC, MyIceGrowthRate, LayerSeeding;

      for (int Class = 0; Class < NumberOfIceClasses; Class++)
      {
      //if ((MyPEcoDynClass->GetJulianDay() > 91) && (MyPEcoDynClass->GetCurrTime()>4.13)) Debugger(Class);
      for (int i = 0; i < NumberOfLines; i++)
      {
         //if ((MyPEcoDynClass->GetJulianDay() > 91) && (MyPEcoDynClass->GetCurrTime()>4.13)) Debugger(i);
         for (int j = 0; j < NumberOfColumns; j++)
         {
            int Count,Layer;
            Count = 0;
            //try
            //{
            Index2D = GetIceIndex(i,j,0,Class);
            if (NumberOfIceClasses > 1) sprintf(MyName, "IceGrowthRate %i", Class + 1);
            else strcpy(MyName, "IceGrowthRate");
            MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceGrowthRate,GetIceIndex(i,j,0,0),MyName,ObjectCode);
            TotalC = 0.0;
            for (int k = 0; k < NumberOfIceLayers; k++)
               TotalC = TotalC + IceAlgaeC[GetIceIndex(i,j,k,Class)];
            Count = 1;
            for (int k = 0; k < NumberOfIceLayers; k++)
            {
                Layer = k;
                Index3D = GetIceIndex(i,j,k,Class);

                if (MySeaIcePointer != NULL)
                {
                   if (k > 0)  //Bottom layer thickness is constant
                   {
                      if (NumberOfIceClasses > 1)sprintf(MyName, "Layer thickness %i", Class + 1);
                      else strcpy(MyName, "Layer thickness");
                      MySeaIcePointer->Inquiry(GetEcoDynClassName(), IceAlgaeLayerThickness[Index3D], GetIceIndex(i,j,k,0),MyName, ObjectCode);
                   }
                   if (IceAlgaeLayerThickness[GetIceIndex(i,j,0,Class)] > TINNY)
                      LayerSeeding = Seeding / IceAlgaeLayerThickness[GetIceIndex(i,j,0,Class)] * IceAlgaeLayerThickness[Index3D];  //Seeding per unit of volume is the same for skeletal and congelation layers
                   else
                      LayerSeeding = 0.0;
                   Count = 2;
                   if ((TotalC <= LayerSeeding) && (k == 0) && (MyIceGrowthRate > 0.0))
                   {
                       if (TotalC > 0.0) LayerSeeding =  LayerSeeding - TotalC;

                       if (ChlToCarbonIceAlgae[Index3D] > TINNY) IceAlgaeChl[Index3D] = MAX(IceAlgaeChl[Index3D],0.0) + LayerSeeding / ChlToCarbonIceAlgae[Index3D];  //Seeding only the bottom layer
                       IceAlgaeC[Index3D] = MAX(IceAlgaeC[Index3D],0.0) + LayerSeeding;

                       if (NCellQuota[Index3D] > TINNY)
                          IceAlgaeN[Index3D] = IceAlgaeC[Index3D] * NCellQuota[Index3D];
                       else
                          IceAlgaeN[Index3D] = IceAlgaeC[Index3D] * NRedfieldQuota;
                       if (PCellQuota[Index3D] > TINNY)
                          IceAlgaeP[Index3D] = IceAlgaeC[Index3D] * PCellQuota[Index3D];
                       else
                          IceAlgaeP[Index3D] = IceAlgaeC[Index3D] * PRedfieldQuota;
                       if (SiCellQuota[Index3D] > TINNY)
                          IceAlgaeSi[Index3D] = IceAlgaeC[Index3D] * SiCellQuota[Index3D];
                       else
                          IceAlgaeSi[Index3D] = IceAlgaeC[Index3D] * SiRedfieldQuota;
                   }
                   Count = 3;

                   if (NumberOfIceClasses > 1)sprintf(MyName, "SIarea %i", Class + 1);
                   else strcpy(MyName, "SIarea");
                   MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceRelativeArea, GetIceIndex(i,j,0,0),MyName, ObjectCode);
                   Count = 4;
                   //MyArea = MyPEcoDynClass->GetChannelWidth(GetBoxIndex(j,i)) * MyPEcoDynClass->GetBoxLength(GetBoxIndex(j,i));
                   if ((MyIceRelativeArea <= TINNY) || (IceAlgaeC[Index3D] < 0.0))
                   {
                      IceAlgaeChl[Index3D] = 0.0;
                      IceAlgaeC[Index3D] = 0.0;
                      IceAlgaeN[Index3D] = 0.0;
                      IceAlgaeP[Index3D] = 0.0;
                      IceAlgaeSi[Index3D] = 0.0;
                      IceAlgaeChlInBox[Index3D] = 0.0;
                      IceAlgaeCInBox[Index3D] = 0.0;
                      IceAlgaeNInBox[Index3D] = 0.0;
                      IceAlgaePInBox[Index3D] = 0.0;
                      IceAlgaeSiInBox[Index3D] = 0.0;
                   }
                   else
                   {
                      Count = 5;
                      IceAlgaeChlInBox[Index3D] = IceAlgaeChl[Index3D] * MyIceRelativeArea;
                      IceAlgaeCInBox[Index3D] = IceAlgaeC[Index3D] * MyIceRelativeArea;
                      IceAlgaeNInBox[Index3D] = IceAlgaeN[Index3D] * MyIceRelativeArea;
                      IceAlgaePInBox[Index3D] = IceAlgaeP[Index3D] * MyIceRelativeArea;
                      IceAlgaeSiInBox[Index3D] = IceAlgaeSi[Index3D] * MyIceRelativeArea;
                   }
                }
                Count = 6;
                if (IceAlgaeC[Index3D] > TINNY)
                {
                   NCellQuota[Index3D] = IceAlgaeN[Index3D] / IceAlgaeC[Index3D];
                   PCellQuota[Index3D] = IceAlgaeP[Index3D] / IceAlgaeC[Index3D];
                   SiCellQuota[Index3D] = IceAlgaeSi[Index3D] / IceAlgaeC[Index3D];
                }
                else
                {
                   NCellQuota[Index3D] = 0.0;
                   PCellQuota[Index3D] = 0.0;
                   SiCellQuota[Index3D] = 0.0;
                }
            }
            //}
            /*catch (EAccessViolation &e)
            {
               char MyText1[10],MyText2[10],MyText3[10],MyText4[10], MyText5[10],MyWholeText[50];
               sprintf(MyText1," Class %i",Class);
               sprintf(MyText2," Line %i",i);
               sprintf(MyText3," Column %i",j);
               sprintf(MyText4," Layer %i",Layer);
               sprintf(MyText5," Count %i",Count);
               strcpy(MyWholeText,MyText1);
               strcat(MyWholeText,MyText2);
               strcat(MyWholeText,MyText3);
               strcat(MyWholeText,MyText4);
               strcat(MyWholeText,MyText5);
               ShowMessage(e.Message + MyWholeText);
            }
            catch (Exception &exception) {
               char MyText1[10],MyText2[10],MyText3[10],MyText4[10], MyText5[10],MyWholeText[50];
               sprintf(MyText1," Class %i",Class);
               sprintf(MyText2," Line %i",i);
               sprintf(MyText3," Column %i",j);
               sprintf(MyText4," Layer %i",Layer);
               sprintf(MyText5," Count %i",Count);
               strcpy(MyWholeText,MyText1);
               strcat(MyWholeText,MyText2);
               strcat(MyWholeText,MyText3);
               strcat(MyWholeText,MyText4);
               strcat(MyWholeText,MyText5);
               ShowMessage(AnsiString(exception.ClassName())+ " "+exception.Message);
               ShowMessage(MyWholeText);
               Debugger(IceAlgaeChl[Index3D]); Debugger(Index3D); Debugger(GetIceIndex(i,j,0,Class));
               Debugger(MyIceRelativeArea);
               Debugger(IceAlgaeCInBox[Index3D]); Debugger(IceAlgaeNInBox[Index3D]); Debugger(IceAlgaePInBox[Index3D]);
            }
            catch (...) {
               char MyText1[10],MyText2[10],MyText3[10],MyText4[10], MyText5[10],MyWholeText[50];
               sprintf(MyText1," Class %i",Class);
               sprintf(MyText2," Line %i",i);
               sprintf(MyText3," Column %i",j);
               sprintf(MyText4," Layer %i",Layer);
               sprintf(MyText5," Count %i",Count);
               strcpy(MyWholeText,MyText1);
               strcat(MyWholeText,MyText2);
               strcat(MyWholeText,MyText3);
               strcat(MyWholeText,MyText4);
               strcat(MyWholeText,MyText5);
               ShowMessage(MyWholeText);
            } */
         }
      }
      }
   }
#endif //_PORT_FORTRAN_
}


/*
//Integrate to use with simulations with fixed biomass
void TIceAlgae::Integrate()
{

    int Index3D, Index2D;

    for (int i = 0; i < NumberOfLines; i++)
    {
       for (int j = 0; j < NumberOfColumns; j++)
       {
          for (int k = 0; k < NumberOfIceLayers; k++)
          {
              Index3D = Get3DIndex(i,j,k);
              if (IceAlgaeC[Index3D] > TINNY)
              {
                 NCellQuota[Index3D] = IceAlgaeN[Index3D] / IceAlgaeC[Index3D];
                 PCellQuota[Index3D] = IceAlgaeP[Index3D] / IceAlgaeC[Index3D];
                 SiCellQuota[Index3D] = IceAlgaeSi[Index3D] / IceAlgaeC[Index3D];
              }
              else
              {
                 NCellQuota[Index3D] = 0.0;
                 PCellQuota[Index3D] = 0.0;
                 SiCellQuota[Index3D] = 0.0;
              }
          }
       }
    }
}  */


void TIceAlgae::Go()
{
  #ifdef __BORLAND
   double MyIceDepth, AValue, AverageLight;
   AverageLight = 0.0;
   int Index3D;
   char MyName[65];
   TEcoDynClass * MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   TEcoDynClass* MyIceLightPointer = MyPEcoDynClass->GetIceLightPointer();


   if (MySeaIcePointer != NULL)
   {
       //This is for GPP daily averaging**********************************************************************************
       if (MyPEcoDynClass->GetJulianDay() != ADay)
       {
          NumberOfParcels = 0.0;
          ADay = ADay + 1;
       }
       //*****************************************************************************************************************
       for (int Class = 0; Class < NumberOfIceClasses; Class++)
       {
       for (int i = 0; i < NumberOfLines; i++)
       {
          for (int j = 0; j < NumberOfColumns; j++)
          {
             //if ((Class == 1) && (i == 0) && (j >= 13)) Debugger(1);
             IceSeaTransfer(i,j,Class);
             //if ((Class == 1) && (i == 0) && (j >= 13)) Debugger(j);
             if (MyTransportPointer != NULL)
             {
                MyTransportPointer->Inquiry(GetEcoDynClassName(), AValue, Get3DIndex(i,j,0),"UpperLayer",ObjectCode);
                UpperLayer = ceil(AValue);
             }
             else
                UpperLayer = MyPEcoDynClass->GetNumberOfLayers()- 1;

             for (int k = 0; k < NumberOfIceLayers; k++)
             {
                //if ((Class == 1) && (i == 0) && (j >= 13)) Debugger(k);
                Index3D = GetIceIndex(i,j,k,Class);
                if (NumberOfIceClasses > 1) sprintf(MyName, "Layer depth %i", Class + 1);
                else strcpy(MyName, "Layer depth");
                MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceDepth, GetIceIndex(i,j,k,0),MyName, ObjectCode);
                if (NumberOfIceClasses > 1) sprintf(MyName, "BrineFractionalVolume %i", Class + 1);
                else strcpy(MyName, "BrineFractionalVolume");
                MySeaIcePointer->Inquiry(GetEcoDynClassName(), BrineFractionalVolume, GetIceIndex(i,j,k,0),MyName, ObjectCode);
                if (NumberOfIceLayers > 1)
                {
                   if (NumberOfIceClasses > 1) sprintf(MyName, "Layer thickness %i", Class + 1);
                   else strcpy(MyName, "Layer thickness");
                   MySeaIcePointer->Inquiry(GetEcoDynClassName(), IceAlgaeLayerThickness[Index3D], GetIceIndex(i,j,k,0),MyName, ObjectCode);
                }
                if (NumberOfIceClasses > 1) sprintf(MyName, "BrineTemperatureIce %i", Class + 1);
                else strcpy(MyName, "BrineTemperatureIce");
                MySeaIcePointer->Inquiry(GetEcoDynClassName(), IceTemperature,GetIceIndex(i,j,k,0),MyName,ObjectCode);
                if (NumberOfIceClasses > 1) sprintf(MyName, "Ice visible average light above chl layer %i", Class + 1);
                else strcpy(MyName, "Ice visible average light above chl layer");
                MyIceLightPointer->Inquiry(GetEcoDynClassName(), AverageLight,GetIceIndex(i,j,k,0),MyName,ObjectCode);
                /*if ((i == 20) && (j == 2) && (Class == 0))
                     {
                       Debugger(222);
                       Debugger(IceAlgaeLayerThickness[Index3D]); Debugger(BrineFractionalVolume);
                       Debugger(AverageLight);
                     } */
                if (AverageLight * WATTSTOMICROEINSTEINS >= LightThreshold)
                {
                  if ((IceAlgaeC[Index3D] > TINNY) && (IceAlgaeLayerThickness[Index3D] > TINNY) && (BrineFractionalVolume > TINNY))
                  {
                     if (NumberOfParcels == 0.0) DailyMeanGPP[Index3D] = 0.0;
                     ChlExtinctionCoefficient(Index3D);
                     Production(i,j,k,Class); //if ((i == 0) && (j == 0) && (k == 19)) {Debugger(115); Debugger(Index3D); }

                     Respiration(i,j,k,Class); //if ((i == 0) && (j == 0) && (k == 19)) Debugger(116);

                     Exudation(i,j,k,Class); //if ((i == 0) && (j == 0) && (k == 19)) Debugger(117);

                     NitrogenUptake(i,j,k,Class); //if ((i == 0) && (j == 0) && (k == 19)) Debugger(118);
                     PhosphorusUptake(i,j,k,Class);  //if ((i == 0) && (j == 0) && (k == 19)) Debugger(119);
                     SilicaUptake(i,j,k,Class); //if ((i == 0) && (j == 0) && (k == 19)) Debugger(120);
                     Mortality(i,j,k,Class); //if ((i == 0) && (j == 0) && (k == 19)) Debugger(121);
                  }
                  else
                  {
                     GPP[Index3D] = 0.0;
                     NPP[Index3D] = 0.0;
                     DailyMeanGPP[Index3D] = 0.0;
                     OxygenNetProduction[Index3D] = 0.0;
                     NUptake[Index3D] = 0.0; PUptake[Index3D] = 0.0; SiUptake[Index3D] = 0.0;
                     NPSLimitation[Index3D] = 0.0; SaltLimitation[Index3D] = 0.0;
                     TemperatureLimitation[Index3D] = 0.0;LightLimitation[Index3D] = 0.0;
                  }
                }
                else if ((IceAlgaeC[Index3D] > TINNY) && (IceAlgaeLayerThickness[Index3D] > TINNY) && (BrineFractionalVolume > TINNY))
                {
                   WinterLosses(i,j,k,Class);
                   GPP[Index3D] = 0.0;
                   NPSLimitation[Index3D] = 0.0; SaltLimitation[Index3D] = 0.0;
                   TemperatureLimitation[Index3D] = 0.0; LightLimitation[Index3D] = 0.0;
                }
                if (ChlToCarbonIceAlgae[Index3D] > TINNY)
                    IceAlgaeChlFlux[Index3D] = IceAlgaeCFlux[Index3D] / ChlToCarbonIceAlgae[Index3D];  //mg Chl m-2 s-1
                else
                    IceAlgaeChlFlux[Index3D] = 0.0;
                
             }
          }
       }
       }
   }
   else
   MessageBox(0, "SeaIceObject not available - TIceAlgae::Go", "EcoDynamo alert", MB_OK);
#endif //__BORLAND
}



void TIceAlgae::Production(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
    int index1, index2;
    char MyName[65];
#ifndef _PORT_FORTRAN_
    TEcoDynClass* MyIceLightPointer = MyPEcoDynClass->GetIceLightPointer();
    index1 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
    index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);

    if (MyIceLightPointer != NULL)
    {
       if (NumberOfIceClasses > 1) sprintf(MyName, "Ice visible light above chl layer %i", ClassNumber + 1);
       else strcpy(MyName, "Ice visible light above chl layer");
       MyIceLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,index2,MyName,ObjectCode);
       if (NumberOfIceClasses > 1) sprintf(MyName, "Ice visible light at bottom %i", ClassNumber + 1);
       else strcpy(MyName, "Ice visible light at bottom");
       MyIceLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,index2,MyName,ObjectCode);
#endif
       LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
       LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
       if (LightAtTop > TINNY)
       {
          if (strcmp(PIfunction, "Platt") == 0)
             GPP[index1] = PlattPIFunction(index1);
          else
          if (strcmp(PIfunction, "Steele") == 0)
             GPP[index1] = SteelePIFunction(index1);
          TemperatureLimitation[index1] = TemperatureArrheniusExponentialLimitation(TempCoefPhotosynthesis, IceTemperature, TminPhotosynthesis);
          GPP[index1] = GPP[index1] * TemperatureLimitation[index1];
          if (NutLimitation) NutrientLimitation(LineNumber, ColumnNumber, LayerNumber, ClassNumber);
          if (SalLimitation) SalinityLimitation(LineNumber, ColumnNumber, LayerNumber, ClassNumber);
       }
       else
       {
          GPP[index1] = 0.0;
          LightLimitation[index1] = 0.0;
       } 
#ifndef _PORT_FORTRAN_  
    }
    else
       GPP[index1] = 0.0;
#endif
    OxygenNetProduction[index1] = GPP[index1] / CarbonToOxygenProd; //mg O2 /mgChl /s
    IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] + GPP[index1] * IceAlgaeChl[index1];  //mg C m-2 s-1
    DailyMeanGPP[index1] = (DailyMeanGPP[index1] * NumberOfParcels + GPP[index1])  / (NumberOfParcels + 1);  //mg C / mg Chla / s
}

void TIceAlgae::ChlExtinctionCoefficient(int ABoxNumber)
{
   int MyBoxNumber;
   double Transmittance;
   MyBoxNumber = ABoxNumber;
   /*if (IceAlgaeLayerThickness[MyBoxNumber] < TINNY)
      KForIceAlgae[MyBoxNumber] = 0.0;
   else
   {  */
      if (IceAlgaeChl[MyBoxNumber] <= TINNY) KForIceAlgae[MyBoxNumber] = 0.0;
      else
      {
         Transmittance = MIN(1.0, 1.102-0.196*log(IceAlgaeChl[MyBoxNumber])); //The formula for tranmittance was taken from Cota et al. (1991) - Journal of Marine Systems 2: 257-277.
                                                                              //However, it is necessary to guarantte that the result is <= 1, which may not be the case if Chl is to low. This is why I use the MIN function
         KForIceAlgae[MyBoxNumber] = MAX(0.0,(1.0 - Transmittance) / IceAlgaeLayerThickness[MyBoxNumber]);
      }
   //}
}

double TIceAlgae::PlattPIFunction(int ABoxNumber)
{
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   double DeltaZ, Soma, MyLightAtTop, MyK;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   MyLightAtTop = LightAtTop;
   DeltaZ = IceAlgaeLayerThickness[MyBoxNumber] / IntegrationSteps;
   Soma = 0.0;
   MyK = KForIceAlgae[MyBoxNumber];
   //if (IceAlgaeLayerThickness[MyBoxNumber] > TINNY)
   //{
     for (int Step = 1; Step <= IntegrationSteps; Step++)    //Eiler integration as a function of depth
     {
        Soma = Soma + (1 - exp(-Slope[MyBoxNumber] * MyLightAtTop / Pmax[MyBoxNumber])) * exp(-beta[MyBoxNumber] * MyLightAtTop / Pmax[MyBoxNumber]) * DeltaZ;

        MyLightPointer->Inquiry(GetEcoDynClassName(),MyLightAtTop,MyLightAtTop,DeltaZ,MyK,0,"Sub-surface irradiance",ObjectCode);
     }
     LightLimitation[MyBoxNumber] = Soma / IceAlgaeLayerThickness[MyBoxNumber];
   //}
   //else
   //  LightLimitation[MyBoxNumber] = 0.0;
   return Pmax[MyBoxNumber] * LightLimitation[MyBoxNumber] / HOURSTOSECONDS; //mg C mg Chl s-1
}

double TIceAlgae::SteelePIFunction(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   if /*(*/(Iopt[MyBoxNumber] > TINNY) //&& (IceAlgaeLayerThickness[MyBoxNumber] > TINNY))
      LightLimitation[MyBoxNumber] =  2.718282 / ( KForIceAlgae[MyBoxNumber] * IceAlgaeLayerThickness[MyBoxNumber])
                                     * (
                                         exp( -LightAtBottom / Iopt[MyBoxNumber] )
                                         -exp( -LightAtTop / Iopt[MyBoxNumber])
                                       );
   else
      LightLimitation[MyBoxNumber] = 0.0;
   return Pmax[MyBoxNumber] * LightLimitation[MyBoxNumber] / HOURSTOSECONDS;  //mg C mg Chl s-1
}


void TIceAlgae::Respiration(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
    double Resp, AverageOxygenProduction;
    double const OxygenMolecularWeight = 32.0;
    int index1, index2;
    char MyName[65];
    TEcoDynClass * MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    index1 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
    index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
    AverageOxygenProduction = /*DailyMeanGPP[Index3D]*/GPP[index1] / CarbonToOxygenProd / (2.0 * OXYGENATOMICWEIGHT);  //mmol O2 / mg Chla / s
    Resp = MaintenanceRespiration[index1] / HOURSTOSECONDS * TemperatureArrheniusExponentialLimitation(TempCoefRespiration,IceTemperature, TminRespiration);    //mmol O2 / mgChl / s
    if (GPP[index1] <= 0.0)
       Resp = Resp + RespirationCoefficient * AverageOxygenProduction
                     * TemperatureArrheniusExponentialLimitation(TempCoefRespiration,IceTemperature, TminRespiration) / HOURSTOSECONDS;
    else
       Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient * AverageOxygenProduction
                     * TemperatureArrheniusExponentialLimitation(TempCoefRespiration, IceTemperature, TminRespiration) / HOURSTOSECONDS;
    RespirationRate[index1] = Resp;//Resp in //mmol O2 / mg Chl / s
    OxygenNetProduction[index1] = OxygenNetProduction[index1] - Resp * 2.0 * OXYGENATOMICWEIGHT;  //mg O2 /mg Chl /s
    double RespInCarbonUnits;
    RespInCarbonUnits = Resp * 2.0 * OXYGENATOMICWEIGHT * CarbonToOxygenProd;   //mg C /mg Chl / s
    NPP[index1] = GPP[index1] - RespInCarbonUnits; //Debugger(NPP[Index3D]);
    /*if ((LineNumber == 9) && (ColumnNumber == 2) && (LayerNumber == 4) && (ClassNumber == 0))
    {
      Debugger(10101); Debugger(AverageOxygenProduction); Debugger(Resp); Debugger(GPP[index1]); Debugger(NPP[index1]);
      Debugger(MaintenanceRespiration[index1]); Debugger(TemperatureArrheniusExponentialLimitation(TempCoefRespiration,IceTemperature, TminRespiration));
      Debugger(IceTemperature);
    } */
    Resp = RespInCarbonUnits * IceAlgaeChl[index1]; //Resp in //mg C /m2 / s
    if ((NPP[index1] < 0.0) && ((NCellQuota[index1] > MaxNCellQuota) || (PCellQuota[index1] > MaxPCellQuota)))
    {
       IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] - Resp;
       IceAlgaeNFlux[index1] = IceAlgaeNFlux[index1] - Resp * NCellQuota[index1]; //mg N / m2 / s
       IceAlgaePFlux[index1] = IceAlgaePFlux[index1] - Resp * PCellQuota[index1]; //mg P / m2 / s
       if (MySeaIcePointer != NULL)
       {
          if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNH4Ice %i", ClassNumber + 1);
          else strcpy(MyName, "BrineNH4Ice");
          MySeaIcePointer->Update(GetEcoDynClassName(), Resp * NCellQuota[index1] / NITROGENATOMICWEIGHT / IceAlgaeLayerThickness[index1] / BrineFractionalVolume,index2,MyName,ObjectCode); //microM/L/s
          if (NumberOfIceClasses > 1) sprintf(MyName, "BrinePhosphateIce %i", ClassNumber + 1);
          else strcpy(MyName, "BrinePhosphateIce");
          MySeaIcePointer->Update(GetEcoDynClassName(), Resp * PCellQuota[index1] / PHOSPHORUSATOMICWEIGHT / IceAlgaeLayerThickness[index1] / BrineFractionalVolume,index2,MyName,ObjectCode); //microM/L/s
       }
    }
    else
       IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] - Resp;
    if (MySeaIcePointer != NULL)
    {
          if (NumberOfIceClasses > 1) sprintf(MyName, "BrineOxygenIce %i", ClassNumber + 1);
          else strcpy(MyName, "BrineOxygenIce");
          MySeaIcePointer->Update(GetEcoDynClassName(), OxygenNetProduction[index1] * IceAlgaeChl[index1] / IceAlgaeLayerThickness[index1] / CUBIC / BrineFractionalVolume, index2,MyName, ObjectCode);//Return as mg O2 / L / s
    }
}

void TIceAlgae::Exudation(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
    double  Exudate, ArealDOCLoss, AmmoniaRelease, PhosphorusRelease, SilicaRelease;
    int index1, index2;
    char MyName[65];
    index1 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
    index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
    AmmoniaRelease = 0.0; PhosphorusRelease = 0.0;
    TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    if ((GPP[index1] > 0.0) && (NutLimitation) && ((NCellQuota[index1] < NRedfieldQuota) || (PCellQuota[index1] < PRedfieldQuota)))
       Exudate = GPP[index1] * DocStressLoss; //mg C mg Chl-1 s-1
    else
       Exudate = 0.0;
    ArealDOCLoss = Exudate * IceAlgaeChl[index1];   //mg C m-2 s-1;
    IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] - ArealDOCLoss;  //mg C m-2 s-1;

    //When NPP is less than zero it is more likely that N/C and P/C ratios may exceed the maximum possible ratios. Therefore, in the following lines ammonia and phosphorus releases
    //are calculated as a function of the difference between N and P cell quotas and maximum allowed quotas.
   /* if ((NPP[MyBoxNumber] < 0.0) && (NCellQuota[MyBoxNumber] > MaxNCellQuota))
    {
       AmmoniaRelease =  NutrientReleaseRatio *  (NCellQuota[MyBoxNumber] - MaxNCellQuota) * IceAlgaeN[MyBoxNumber];  //mg N m-2 d-1
       IceAlgaeNFlux[MyBoxNumber] = IceAlgaeNFlux[MyBoxNumber] - AmmoniaRelease / DAYSTOSECONDS;   //mg N m-2 s-1
    }
    if ((NPP[MyBoxNumber] < 0.0) && (PCellQuota[MyBoxNumber] > MaxPCellQuota))
    {
       PhosphorusRelease =  NutrientReleaseRatio *  (PCellQuota[MyBoxNumber] - MaxPCellQuota) * IceAlgaeP[MyBoxNumber];  //mg P m-2 d-1
       IceAlgaePFlux[MyBoxNumber] = IceAlgaePFlux[MyBoxNumber] - PhosphorusRelease / DAYSTOSECONDS;   //mg P m-2 s-1
    }
    if ((NPP[MyBoxNumber] < 0.0) && (SiCellQuota[MyBoxNumber] > MaxSiCellQuota))
    {
       SilicaRelease =  NutrientReleaseRatio *  (SiCellQuota[MyBoxNumber] - MaxSiCellQuota) * IceAlgaeSi[MyBoxNumber];  //mg P m-2 d-1
       IceAlgaeSiFlux[MyBoxNumber] = IceAlgaeSiFlux[MyBoxNumber] - SilicaRelease / DAYSTOSECONDS;   //mg P m-2 s-1
    }
    if (MySeaIcePointer != NULL)
    {
        MySeaIcePointer->Update(GetEcoDynClassName(), ArealDOCLoss / IceAlgaeLayerThickness[MyBoxNumber] / CARBONATOMICWEIGHT / BrineFractionalVolume, MyBoxNumber,"BrineDOCIce", ObjectCode);//Return as micromol C / L / s
        MySeaIcePointer->Update(GetEcoDynClassName(), AmmoniaRelease / NITROGENATOMICWEIGHT / IceAlgaeLayerThickness[MyBoxNumber] / DAYSTOSECONDS / BrineFractionalVolume,MyBoxNumber,"BrineNH4Ice",ObjectCode);   //microM/L/s
        MySeaIcePointer->Update(GetEcoDynClassName(), PhosphorusRelease / PHOSPHORUSATOMICWEIGHT / IceAlgaeLayerThickness[MyBoxNumber] / DAYSTOSECONDS / BrineFractionalVolume,MyBoxNumber,"BrinePhosphateIce",ObjectCode); //microM/L/s
        MySeaIcePointer->Update(GetEcoDynClassName(), SilicaRelease / SILICAATOMICWEIGHT / IceAlgaeLayerThickness[MyBoxNumber] / DAYSTOSECONDS / BrineFractionalVolume,MyBoxNumber,"BrineSilicateIce",ObjectCode); //microM/L/s
    }  */
}

void TIceAlgae::NutrientLimitation(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
    int i, index2;
    char MyName[65];
    i = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
    index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
    TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
    if ((MySeaIcePointer != NULL))
    {
      if (strcmp(LimitationType, "Internal") == 0)
      {
          if (PCellQuota[i] <= MinPCellQuota)
             PLimitation[i] = 0.0;
          else
             PLimitation[i] = PCellQuota[i] / (PCellQuota[i] + KPInternal);
          if (NCellQuota[i] <= MinNCellQuota)
             NLimitation[i] = 0.0;
          else
             NLimitation[i] = NCellQuota[i] / (NCellQuota[i] + KNInternal);
          if (KSiInternal == 0.0)
             SiLimitation[i] = 1.0;
          else
          {
            if (SiCellQuota[i] <= MinSiCellQuota)
               SiLimitation[i] = 0.0;
            else
               SiLimitation[i] = SiCellQuota[i] / (SiCellQuota[i] + KSiInternal);
          }
      }
      else
      if (strcmp(LimitationType, "External") == 0)
      {
         {
             double Ammonia, Nitrate, Nitrite, Phosphate, Silicate;
             if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNH4Ice %i", ClassNumber + 1);
             else strcpy(MyName, "BrineNH4Ice");
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Ammonia,index2,MyName,   /*micro M*/ ObjectCode);
             if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNO3Ice %i", ClassNumber + 1);
             else strcpy(MyName, "BrineNO3Ice");
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrate,index2,MyName,ObjectCode);
             //MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrite,i,"Nitrite",ObjectCode);
             if (NumberOfIceClasses > 1) sprintf(MyName, "BrinePhosphateIce %i", ClassNumber + 1);
             else strcpy(MyName, "BrinePhosphateIce");
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Phosphate,index2,MyName,ObjectCode);
             if (NumberOfIceClasses > 1) sprintf(MyName, "BrineSilicateIce %i", ClassNumber + 1);
             else strcpy(MyName, "BrineSilicateIce");
             MySeaIcePointer->Inquiry(GetEcoDynClassName(), Silicate,index2,MyName,ObjectCode);
             NLimitation[i] = Nitrate / (Nitrate + KNO3External) * exp(-Phi * Ammonia) + Ammonia / (Ammonia + KNH4External);
             PLimitation[i] = Phosphate / (Phosphate + KPExternal);
             if (KSiExternal == 0.0)
                SiLimitation[i] = 1.0;
             else
                SiLimitation[i] = Silicate / (Silicate + KSiExternal);
         }
      }
      NPSLimitation[i] = MIN(MIN(NLimitation[i], PLimitation[i]),SiLimitation[i]);
      GPP[i] = GPP[i] * NPSLimitation[i];
   }
}

void TIceAlgae::NitrogenUptake(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
   int i, index2;
   char MyName[65];
   i = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
   index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((NCellQuota[i] > TINNY) && (PCellQuota[i] > TINNY) && (MySeaIcePointer != NULL))
   {
      double AmmoniaUpTake, NitrateAndNitriteUptake, NMaxUptakeOfNitrate;
      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Ammonia, Nitrate, Nitrite, AmmoniumUptakeLimitation;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNH4Ice %i", ClassNumber + 1);
      else strcpy(MyName, "BrineNH4Ice");
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Ammonia,index2,MyName,   /*micro M*/ ObjectCode);
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNO3Ice %i", ClassNumber + 1);
      else strcpy(MyName, "BrineNO3Ice");
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrate,index2,MyName,ObjectCode);
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNO2Ice %i", ClassNumber + 1);
      else strcpy(MyName, "BrineNO2Ice");
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Nitrite,index2,MyName,ObjectCode);
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
         if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNH4Ice %i", ClassNumber + 1);
         else strcpy(MyName, "BrineNH4Ice");
         MySeaIcePointer->Update(GetEcoDynClassName(), -AmmoniaUpTake / NITROGENATOMICWEIGHT / IceAlgaeLayerThickness[i] / DAYSTOSECONDS / BrineFractionalVolume/*microM/L/s*/,index2,MyName,ObjectCode);
         //...
         if (Nitrate + Nitrite > TINNY)
                NitrateAndNitriteUptake = /*NMaxUptake*/NMaxUptakeOfNitrate * (Nitrate + Nitrite) / ((Nitrate + Nitrite) + KNO3) *
                                  (1.0 - NCellQuota[i] / MaxNCellQuota) * IceAlgaeN[i]; //mg N m-2 d-1
         else
                NitrateAndNitriteUptake = 0.0;
         NUptake[i] = NUptake[i] + NitrateAndNitriteUptake;

         if ((NitrateAndNitriteUptake > TINNY) && (IceAlgaeLayerThickness[i] > TINNY) && (BrineFractionalVolume > TINNY))
         {
            //Debugger(BrineFractionalVolume);

            //Debugger(-NitrateAndNitriteUptake * Nitrate / (Nitrate + Nitrite) / NITROGENATOMICWEIGHT /IceAlgaeLayerThickness[i]/ DAYSTOSECONDS / BrineFractionalVolume);

            if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNO3Ice %i", ClassNumber + 1);
            else strcpy(MyName, "BrineNO3Ice");
            MySeaIcePointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrate / (Nitrate + Nitrite) / NITROGENATOMICWEIGHT /
                                    IceAlgaeLayerThickness[i]/ DAYSTOSECONDS / BrineFractionalVolume/*microM/L/s*/,index2,MyName,ObjectCode);

            if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNO2Ice %i", ClassNumber + 1);
            else strcpy(MyName, "BrineNO2Ice");
            MySeaIcePointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrite / (Nitrate + Nitrite) / NITROGENATOMICWEIGHT /
                                    IceAlgaeLayerThickness[i]/ DAYSTOSECONDS / BrineFractionalVolume/*microM/L/s*/,index2,MyName,ObjectCode);
         }
          //...
      }
   }
   else
      NUptake[i] = 0.0;
   //NCellFlux[i] = NCellFlux[i] + NUptake[i] / DAYSTOSECONDS;   //mg N m-2 s-1
   IceAlgaeNFlux[i] = IceAlgaeNFlux[i] + NUptake[i] / DAYSTOSECONDS;   //mg N m-2 s-1

}

void TIceAlgae::PhosphorusUptake(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
   int i, index2;
   char MyName[65];
   i = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
   index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((NCellQuota[i] > TINNY) && (PCellQuota[i] > TINNY) && (MySeaIcePointer != NULL))
   {

      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Phosphate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrinePhosphateIce %i", ClassNumber + 1);
      else strcpy(MyName, "BrinePhosphateIce");
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Phosphate,index2,MyName,ObjectCode);
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
         //Debugger(11);Debugger(PUptake[i]);
         if ((IceAlgaeLayerThickness[i] > TINNY) && (BrineFractionalVolume > TINNY))
         {
            MySeaIcePointer->Update(GetEcoDynClassName(), -PUptake[i] / DAYSTOSECONDS / PHOSPHORUSATOMICWEIGHT / IceAlgaeLayerThickness[i] / BrineFractionalVolume,index2,MyName,ObjectCode);
         }
         //...
      }
   }
   else
      PUptake[i] = 0.0;
   //PCellFlux[i] = PCellFlux[i] + PUptake[i] / DAYSTOSECONDS;  //mg P m-2 s-1
   IceAlgaePFlux[i] = IceAlgaePFlux[i] + PUptake[i] / DAYSTOSECONDS;   //mg P m-2 s-1
}

void TIceAlgae::SilicaUptake(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
   int i, index2;
   char MyName[65];
   i = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
   index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((NCellQuota[i] > TINNY) && (PCellQuota[i] > TINNY) && (MySeaIcePointer != NULL))
   {

      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Silicate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineSilicateIce %i", ClassNumber + 1);
      else strcpy(MyName, "BrineSilicateIce");
      MySeaIcePointer->Inquiry(GetEcoDynClassName(), Silicate,index2,MyName,ObjectCode);
      //...
      if (
            (SiCellQuota[i] > MaxSiCellQuota) ||
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
            MySeaIcePointer->Update(GetEcoDynClassName(), -SiUptake[i] / DAYSTOSECONDS / SILICAATOMICWEIGHT / IceAlgaeLayerThickness[i] / BrineFractionalVolume, index2,MyName,ObjectCode);
         //...
      }
   }
   else
      SiUptake[i] = 0.0;
   //SiCellFlux[i] = SiCellFlux[i] + SiUptake[i] / DAYSTOSECONDS;  //mg Si m-2 s-1
   IceAlgaeSiFlux[i] = IceAlgaeSiFlux[i] + SiUptake[i] / DAYSTOSECONDS;   //mg Si m-2 s-1
}

void TIceAlgae::SalinityLimitation(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)   //According with equation 36 of Arrigo et al (1993) - Journal of Geophysical Research 98: 6929-6946
{
   int index1, index2;
   char MyName[65];
   index1 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
   index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
   double MySalinity;
   double const alfa1 = 0.011, alfa2 = 0.03012, alfa3 = 0.0010342,
                alfa4 = -4.6033e-5, alfa5 = 4.926e-7, alfa6 = -1.659e-9;
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if (MySeaIcePointer != NULL)
   {
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineSalinityIce %i", ClassNumber + 1);
      else strcpy(MyName, "BrineSalinityIce");
      MySeaIcePointer->Inquiry(GetEcoDynClassName(),MySalinity,index2,MyName,ObjectCode);
      SaltLimitation[index1] = alfa1 + alfa2 * MySalinity + alfa3 * pow(MySalinity, 2.0) + alfa4 * pow(MySalinity, 3.0) +
                                    alfa5 * pow(MySalinity, 4.0) + alfa6 * pow(MySalinity, 5.0);
      SaltLimitation[index1] = MAX(SaltLimitation[index1],0.0);  //To prevent negative values
      SaltLimitation[index1] = MIN(SaltLimitation[index1],1.0);  //To prevent values slightly above 1

   }
   else
      SaltLimitation[index1] = 1.0;
   GPP[index1] = GPP[index1] * SaltLimitation[index1];
}

void TIceAlgae::IceSeaTransfer(int LineNumber, int ColumnNumber, int ClassNumber)
{
   int MyBoxNumber, Index, UpperLayer, MyLine, MyColumn, MyClass,Index2D;
   double MyDepth, AValue, MyMeltingSwitch, MyMeltingSnowFlux, MyIceRelativeArea;
   char MyName[65];
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer(),
               * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
               * MyPhytoplanktonPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
               * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   MyColumn = ColumnNumber;
   MyLine = LineNumber;
   MyClass = ClassNumber;
   MyMeltingSwitch = 0.0;
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
   Index2D = Get3DIndex(MyLine,MyColumn,0) + MyClass * NumberOfLines * NumberOfColumns;
   if (NumberOfIceClasses > 1)sprintf(MyName, "SIarea %i", MyClass + 1);
   else strcpy(MyName, "SIarea");
   MySeaIcePointer->Inquiry(GetEcoDynClassName(), MyIceRelativeArea, GetIceIndex(MyLine,MyColumn,0,MyClass),MyName, ObjectCode);
   if ((MySPMPointer != NULL) && (MyDepth > TINNY))
   {

            MySPMPointer->Update(GetEcoDynClassName(), (IceMeltingCLosseRate[Index2D] + SnowMeltingCLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth / CUBIC/*mg L-1 s-1*/, Index,"LiveC",ObjectCode);
            MySPMPointer->Update(GetEcoDynClassName(), (IceMeltingNLosseRate[Index2D] + SnowMeltingNLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth / CUBIC/*mg L-1 s-1*/, Index,"LiveN",ObjectCode);
            MySPMPointer->Update(GetEcoDynClassName(), (IceMeltingPLosseRate[Index2D] + SnowMeltingPLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth / CUBIC/*mg L-1 s-1*/, Index,"LiveP",ObjectCode);
            MySPMPointer->Update(GetEcoDynClassName(), (IceMeltingSiLosseRate[Index2D] + SnowMeltingSiLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth / CUBIC/*mg L-1 s-1*/, Index,"LiveSi",ObjectCode);
            //Snow and ice melting losses losses are integrated in ice algal variables in the integrate of the previous time step, when SnowMeltTransport is called.
            //Therefore, here they will add to phytoplankton variables in the water
            //but they will not add to ice algal losses accounted for in the previous time step.
            //The reason for doing this update of phytoplankton gains in "this" time step it is because these gains are known only at the "integrate" time of
            //the previous time step.
   }
   if ((MyPhytoplanktonPointer != NULL) && (MyDepth > TINNY))
   {
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), (IceMeltingCLosseRate[Index2D] + SnowMeltingCLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth/*mg m-3 s-1*/, Index,"Phytoplankton biomass",ObjectCode);
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), (IceMeltingNLosseRate[Index2D] + SnowMeltingNLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth/*mg m-3 s-1*/, Index,"Phytoplankton N biomass",ObjectCode);
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), (IceMeltingPLosseRate[Index2D] + SnowMeltingPLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth/*mg m-3 s-1*/, Index,"Phytoplankton P biomass",ObjectCode);
            MyPhytoplanktonPointer->Update(GetEcoDynClassName(), (IceMeltingSiLosseRate[Index2D] + SnowMeltingSiLosseRate[Index2D]) * MyIceRelativeArea/ MyDepth/*mg m-3 s-1*/, Index,"Phytoplankton Si biomass",ObjectCode);
            //Snow and ice melting losses losses are integrated in ice algal variables in the integrate of the previous time step, when SnowMeltTransport is called.
            //Therefore, here they will add to phytoplankton variables in the water
            //but they will not add to ice algal losses accounted for in the previous time step.
            //The reason for doing this update of phytoplankton gains in "this" time step it is because these gains are known only at the "integrate" time of
            //the previous time step.
   }
      //}
   //}
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
#ifndef _PORT_FORTRAN_
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
#endif //_PORT_FORTRAN_

void TIceAlgae::Mortality(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
   double Mortality;
   int index1, index2;
   char MyName[65];
   index1 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
   index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
   Mortality = MortalityAtZeroDegres[index1] * TemperatureArrheniusExponentialLimitation(TempCoefMortality,IceTemperature, TminMortality) / HOURSTOSECONDS;
   IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] - Mortality * IceAlgaeC[index1];
   IceAlgaeNFlux[index1] = IceAlgaeNFlux[index1] - Mortality * IceAlgaeN[index1];
   IceAlgaePFlux[index1] = IceAlgaePFlux[index1] - Mortality * IceAlgaeP[index1];
   IceAlgaeSiFlux[index1] = IceAlgaeSiFlux[index1] - Mortality * IceAlgaeSi[index1];
   TEcoDynClass* MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();
   if ((MySeaIcePointer != NULL) && (IceAlgaeLayerThickness[index1] > TINNY) && (BrineFractionalVolume > TINNY))
   {
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineDetritalIceC %i", ClassNumber + 1);
      else strcpy(MyName, "BrineDetritalIceC");
      MySeaIcePointer->Update(GetEcoDynClassName(), Mortality * IceAlgaeC[index1] / IceAlgaeLayerThickness[index1] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, index2,MyName,ObjectCode);
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineDetritalIceN %i", ClassNumber + 1);
      else strcpy(MyName, "BrineDetritalIceN");
      MySeaIcePointer->Update(GetEcoDynClassName(), Mortality * IceAlgaeN[index1] / IceAlgaeLayerThickness[index1] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, index2,MyName,ObjectCode);
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineDetritalIceP %i", ClassNumber + 1);
      else strcpy(MyName, "BrineDetritalIceP");
      MySeaIcePointer->Update(GetEcoDynClassName(), Mortality * IceAlgaeP[index1] / IceAlgaeLayerThickness[index1] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, index2,MyName,ObjectCode);
      if (NumberOfIceClasses > 1) sprintf(MyName, "BrineDetritalIceSi %i", ClassNumber + 1);
      else strcpy(MyName, "BrineDetritalIceSi");
      MySeaIcePointer->Update(GetEcoDynClassName(), Mortality * IceAlgaeSi[index1] / IceAlgaeLayerThickness[index1] / CUBIC / BrineFractionalVolume/*mg L-1 s-1*/, index2,MyName,ObjectCode);
   }
}


void TIceAlgae::WinterLosses(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber)
{
    double Resp, AverageOxygenProduction;
    int index1, index2;
    char MyName[65];
    index1 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,ClassNumber);
    index2 = GetIceIndex(LineNumber,ColumnNumber,LayerNumber,0);
    TEcoDynClass * MySeaIcePointer = MyPEcoDynClass->GetSeaIcePointer();

    if (IceAlgaeChl[index1] > TINNY)
    {
       Resp = WinterResp / DAYSTOSECONDS * IceAlgaeC[index1];    //mg C / m2 / s
       OxygenNetProduction[index1] = - Resp / CarbonToOxygenProd / IceAlgaeChl[index1];   //mg O2/ mg Chl / s
       RespirationRate[index1] = Resp / CarbonToOxygenProd / (2.0 * OXYGENATOMICWEIGHT) / IceAlgaeChl[index1];  //mmol O2 / mg Chla / s
       NPP[index1] = - Resp / IceAlgaeChl[index1]; //mg C / mg Chl / s
    }
    else
    {
       Resp = 0.0;
       OxygenNetProduction[index1] = 0.0;
       RespirationRate[index1] = 0.0;
       NPP[index1] = 0.0;
    }

    if ((NPP[index1] < 0.0) && ((NCellQuota[index1] > MaxNCellQuota) || (PCellQuota[index1] > MaxPCellQuota)))
    {
       IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] - Resp;
       IceAlgaeNFlux[index1] = IceAlgaeNFlux[index1] - Resp * NCellQuota[index1]; //mg N / m2 / s
       IceAlgaePFlux[index1] = IceAlgaePFlux[index1] - Resp * PCellQuota[index1]; //mg P / m2 / s
       if (MySeaIcePointer != NULL)
       {
          if (NumberOfIceClasses > 1) sprintf(MyName, "BrineNH4Ice %i", ClassNumber + 1);
          else strcpy(MyName, "BrineNH4Ice");
          MySeaIcePointer->Update(GetEcoDynClassName(), Resp * NCellQuota[index1] / NITROGENATOMICWEIGHT / IceAlgaeLayerThickness[index1] / BrineFractionalVolume,index2,MyName,ObjectCode); //microM/L/s
          if (NumberOfIceClasses > 1) sprintf(MyName, "BrinePhosphateIce %i", ClassNumber + 1);
          else strcpy(MyName, "BrinePhosphateIce");
          MySeaIcePointer->Update(GetEcoDynClassName(), Resp * PCellQuota[index1] / PHOSPHORUSATOMICWEIGHT / IceAlgaeLayerThickness[index1] / BrineFractionalVolume,index2,MyName,ObjectCode); //microM/L/s
       }
    }
    else
       IceAlgaeCFlux[index1] = IceAlgaeCFlux[index1] - Resp;
    if (MySeaIcePointer != NULL)  {
       if (NumberOfIceClasses > 1) sprintf(MyName, "BrineOxygenIce %i", ClassNumber + 1);
       else strcpy(MyName, "BrineOxygenIce");
       MySeaIcePointer->Update(GetEcoDynClassName(), OxygenNetProduction[index1] * IceAlgaeChl[index1] / IceAlgaeLayerThickness[index1] / CUBIC / BrineFractionalVolume, index2,MyName, ObjectCode);//Return as mg O2 / L / s
   }
}


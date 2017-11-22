// Ice algae object header file
// REV A - 04.07.2013

/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef ICEALGAEOBJT_H
#define ICEALGAEOBJT_H

#include "EcoDynClass.h"
#include "prodobjt.h"

// DLL header file
#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TIceAlgae : public TProdutor
{
   public :
#ifndef _PORT_FORTRAN_     
        // constructors invoked outside EcoDyn
        /*TIceAlgae(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration);
        TIceAlgae(char* className, float timeStep, char* morphologyFilename,char* parametersFilename);*/
        //Constructors invoked internally
        TIceAlgae(TEcoDynClass* APEcoDynClass, char* className);
#else
        static TIceAlgae* getIceAlgae();
        TIceAlgae(char* className);
     
        void SetLightAtTop(double lightAtTop){LightAtTop=lightAtTop;}
        void SetLightAtBottom(double lightAtBottom){LightAtBottom=lightAtBottom;}
        void SetWaterTemperature(double waterTemperature){WaterTemperature=waterTemperature;}
        void SetIceTemperature(double iceTemperature){IceTemperature=iceTemperature;}
        void SetTimeStep(double timeStep) {TimeStep = timeStep;} 
        void SetABoxDepth(double layerThickness) {LayerThickness = layerThickness;}    
        void SetABoxNumber (int aBoxNumber) {ABoxNumber = aBoxNumber;}         
        void SetNumberOfLines(int numberOfLines) {NumberOfLines = numberOfLines;}
        void SetNumberOfColumns(int numberOfColumns) {NumberOfColumns = numberOfColumns;}
        void SetNumberOfLayers(int numberOfLayers) {NumberOfLayers = numberOfLayers;}
        void SetNumberOfBoxes(int numberOfBoxes) {NumberOfBoxes = numberOfBoxes;}
        void SetNumberOfBoxesWithIce(int numberOfBoxesWithIce) {NumberOfBoxesWithIce = numberOfBoxesWithIce;}  
        void SetNumberOfIceClasses(int numberOfIceClasses) {NumberOfIceClasses = numberOfIceClasses;}
#endif  //_PORT_FORTRAN_
        ~TIceAlgae();
        virtual void freeMemory();
        virtual void Go();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        VNA* InquiryIce(char* srcName,int &IceLayers, int &IceVariables);
        virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Integrate();
        virtual bool SetVariableValue(char* srcName, double Value,int BoxNumber,char* VariableName);
        void InitializeVariables(char* classeName);
        double aMin;
#ifndef _PORT_FORTRAN_    
        void ReadVariablesAndParameters(char* classeName);
        bool SaveVariables();
#endif  //_PORT_FORTRAN_
        virtual void Production(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void Respiration(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void Exudation(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void NutrientLimitation(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void NitrogenUptake(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void PhosphorusUptake(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void SilicaUptake(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual void Mortality(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumberr);
        virtual void IceSeaTransfer(int LineNumber, int ColumnNumber, int ClassNumber);
        virtual void ChlExtinctionCoefficient(int ABoxNumber);
        virtual void SalinityLimitation(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        virtual double TemperatureArrheniusExponentialLimitation(double TemperatureAugmentationRate, double ATemperature, double AReferenceTemperature);
        virtual void WinterLosses(int LineNumber, int ColumnNumber, int LayerNumber, int ClassNumber);
        
        int MyNumberOfBoxesInSubDomain, ADay, ABoxNumber;
        double *IceAlgaeChl, *IceAlgaeC, *IceAlgaeN, *IceAlgaeP, *IceAlgaeSi,
               *NCellQuota, *PCellQuota, *SiCellQuota,
               *IceAlgaeChlFlux, *IceAlgaeCFlux, *IceAlgaeNFlux, *IceAlgaePFlux, *IceAlgaeSiFlux,
               *KForIceAlgae, *IceAlgaeLayerThickness, *OxygenNetProduction,
               *IceAlgaeChlInBox, *IceAlgaeCInBox, *IceAlgaeNInBox, *IceAlgaePInBox, *IceAlgaeSiInBox, *DailyMeanGPP,
               *NUptake, *PUptake, *SiUptake, *NCellFlux, *PCellFlux, *SiCellFlux,
               *NLimitation, *PLimitation, *SiLimitation, *NPSLimitation,*LightLimitation, *TemperatureLimitation, *SaltLimitation, *RespirationRate,
               *MortalityAtZeroDegres, *SnowMeltingCLosseRate, *SnowMeltingNLosseRate, *SnowMeltingPLosseRate, *SnowMeltingSiLosseRate,
               *IceMeltingCLosseRate, *IceMeltingNLosseRate, *IceMeltingPLosseRate, *IceMeltingSiLosseRate, *ChlToCarbonIceAlgae;
        double MaxCCellQuota, MaxNCellQuota, MaxPCellQuota, MaxSiCellQuota,
               MinCCellQuota, MinNCellQuota, MinPCellQuota, MinSiCellQuota, MaxNPRatio, MinNPRatio, MinNSiRatio,KNH4, KNO3, KP, KSi, NMaxUptake, PMaxUptake, SiMaxUptake,
               KNInternal, KPInternal, KSiInternal, KNO3External, KNH4External,	KPExternal, KSiExternal, Phi,
               CarbonToOxygenProd, CarbonToOxygenResp, WaterTemperature, IceTemperature,
               TINNY, LightAtTop, LightAtBottom, IntegrationSteps, NumberOfParcels,
               TminRespiration, TminPhotosynthesis, TminMortality, TempCoefRespiration, TempCoefPhotosynthesis,	TempCoefMortality,RatioLightDarkRespiration,
               /*MortalityAtZeroDegres, */DocStressLoss, FreezingPointOfSeawater, Seeding, BrineFractionalVolume, NutrientReleaseRatio, NRedfieldQuota, PRedfieldQuota, 
               SiRedfieldQuota,IceMeltAmplificationFactor,
               LightThreshold, WinterResp, LayerThickness;
        char PIfunction[65], LimitationType[65];
        double PlattPIFunction(int ABoxNumber);
        double SteelePIFunction(int ABoxNumber);
        bool Steele, Platt, External, Internal, NutLimitation, SalLimitation, TempLimitation, TransportInIce, ArealBiomassCorrection;
        int NumberOfBoxesWithIce, NumberOfCongelationLayers, NumberOfSkeletalLayers, NumberOfPlateletLayers, NumberOfHorizontalCells, UpperLayer, LowerLayerForChlExchange, NumberOfIceClasses;
        IceRecord *MyIceRecord;
};

#ifndef _PORT_FORTRAN_

static TEcoDynClass *PIceAlgae;

#else  //_PORT_FORTRAN_
extern "C" {
   void icealgae_test__(long* PIceAlgae);
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
                       double* lightThreshold, double* winterResp);

    //ChlExtinctionCoefficient(Index3D);
    //Production(i,j,k,Class); 
    //Respiration(i,j,k,Class);
    //Exudation(i,j,k,Class); 
    //NitrogenUptake(i,j,k,Class); 
    //PhosphorusUptake(i,j,k,Class); 
    //SilicaUptake(i,j,k,Class); 
    //Mortality(i,j,k,Class); 
    void icealgae_go__(long* PIceAlgae, double* layerThickness, double* timeStep);

    void icealgae_production__(long* PIceAlgae, double* lightAtTop, double* lightAtBottom, double* kValue,double* iceTemperature,
                               int* piCurveOption, double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* biomass, 
                               double *TIC, double *Slope, double* Chl2Carbon, double *OxygenProduction);

   /* void icealgae_respiration__(long* PIceAlgae, double* waterTemperature, double* cffCRespiration, double *GrossProduction, double* biomass, double *Oxygen, double* Chl2Carbon, double *OxygenConsumption);

    void icealgae_exudation__(long* PIceAlgae, double* cffCExudation, double *GrossProduction, double* biomass, double *NCellQuota, double *PCellQuota);

    void icealgae_nitrogen_uptake__(long* PIceAlgae, double* Ammonia, double* Nitrate, double* Nitrite,double* cffNH4, double *cffNO3NO2, double* nPhyto, double* biomass);  

    void icealgae_phosphorus_uptake__(long* PIceAlgae, double* Phosphate,double* cffPO4, double* pPhyto, double* biomass); 

    void icealgae_silica_uptake__(long* PIceAlgae, double* Phosphate,double* cffPO4, double* pPhyto, double* biomass);  

    void icealgae_mortality__(long* PIceAlgae, double* nCellQuota, double* pCellQuota,
                               double* waterTemperature, double* biomass, double* timeStep, double* cff); */
    
}
#endif  //_PORT_FORTRAN_

#endif  // ICEALGAEOBJT_H



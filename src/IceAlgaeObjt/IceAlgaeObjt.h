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
        // constructors invoked outside EcoDyn
        TIceAlgae(char* className,
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
        TIceAlgae(char* className, float timeStep, char* morphologyFilename,char* parametersFilename);
        //Constructors invoked internally
        TIceAlgae(TEcoDynClass* APEcoDynClass, char* className);
        ~TIceAlgae();
        virtual void freeMemory();
        virtual void Go();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        VNA* InquiryIce(char* srcName,int &IceLayers, int &IceVariables);
        virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Integrate();
        virtual bool SetVariableValue(char* srcName, double Value,int BoxNumber,char* VariableName);
   protected :
        void InitializeVariables(char* classeName);
        void ReadVariablesAndParameters(char* classeName);
        virtual void Production(int ABoxNumber);
        virtual void Respiration(int ABoxNumber);
        virtual void Exudation(int ABoxNumber);
        virtual void NutrientLimitation(int ABoxNumber);
        virtual void NitrogenUptake(int ABoxNumber);
        virtual void PhosphorusUptake(int ABoxNumber);
        virtual void SilicaUptake(int ABoxNumber);
        virtual void Mortality(int ABoxNumber);
        virtual void IceSeaTransfer(int ABoxNumber);
        virtual void ChlExtinctionCoefficient(int ABoxNumber);
        virtual void SalinityLimitation(int ABoxNumber);
        virtual double TemperatureArrheniusExponentialLimitation(double TemperatureAugmentationRate, double ATemperature, double AReferenceTemperature);
        bool SaveVariables();
        int MyNumberOfBoxesInSubDomain, ADay;
        double *IceAlgaeChl, *IceAlgaeC, *IceAlgaeN, *IceAlgaeP, *IceAlgaeSi,
               *NCellQuota, *PCellQuota, *SiCellQuota,
               *IceAlgaeChlFlux, *IceAlgaeCFlux, *IceAlgaeNFlux, *IceAlgaePFlux, *IceAlgaeSiFlux,
               *KForIceAlgae, *IceAlgaeLayerThickness, *OxygenNetProduction,
               *IceAlgaeChlInBox, *IceAlgaeCInBox, *IceAlgaeNInBox, *IceAlgaePInBox, *IceAlgaeSiInBox, *DailyMeanGPP,
               *NUptake, *PUptake, *SiUptake, *NCellFlux, *PCellFlux, *SiCellFlux,
               *NLimitation, *PLimitation, *SiLimitation, *NPSLimitation,*LightLimitation, *TemperatureLimitation, *SaltLimitation;
        double MaxCCellQuota, MaxNCellQuota, MaxPCellQuota, MaxSiCellQuota,
               MinCCellQuota, MinNCellQuota, MinPCellQuota, MinSiCellQuota, MaxNPRatio, MinNPRatio, MinNSiRatio,KNH4, KNO3, KP, KSi, NMaxUptake, PMaxUptake, SiMaxUptake,
               KNInternal, KPInternal, KSiInternal, KNO3External, KNH4External,	KPExternal, KSiExternal, Phi,
               CarbonToOxygenProd, CarbonToOxygenResp, WaterTemperature,
               TINNY, LightAtTop, LightAtBottom, IntegrationSteps, NumberOfParcels,
               TminRespiration, TminPhotosynthesis, TminMortality, TempCoefRespiration, TempCoefPhotosynthesis,	TempCoefMortality,RatioLightDarkRespiration,
               MortalityAtZeroDegres, DocStressLoss, FreezingPointOfSeawater, Seeding, BrineFractionalVolume;
        char PIfunction[65], LimitationType[65];
        double PlattPIFunction(int ABoxNumber);
        double SteelePIFunction(int ABoxNumber);
        bool Steele, Platt, External, Internal, NutLimitation, SalLimitation, TempLimitation;
        int NumberOfBoxesWithIce, NumberOfIceLayers, NumberOfCongelationLayers, NumberOfSkeletalLayers, NumberOfPlateletLayers;
        IceRecord *MyIceRecord;
};


static TEcoDynClass *PIceAlgae;

#endif  // ICEALGAEOBJT_H



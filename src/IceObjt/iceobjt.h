// Ice object header file
// @ Pedro Duarte

/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef ICEOBJT_H
#define ICEOBJT_H

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TIce : public TEcoDynClass
{
    public :
      // constructors invoked outside EcoDyn
      TIce(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
          double aLength[], double aWidth[], double aElevation[], int aType[],
          int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
          int nVars, VNA aVarsNames[]);
      TIce(char* className, float timeStep, char* morphologyFilename,char* variablesFilename);
      TIce(TEcoDynClass* APEcodynClass, char* className);
      ~TIce();
      virtual void freeMemory();
      virtual void Go();
      virtual void Integrate();
      virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber, char* ParameterName,int AnObjectCode);
      VNA* InquiryIce(char* srcName,int &IceLayers, int &IceVariables);
      virtual bool SetVariableValue(char* srcName,double Value,int BoxNumber, char* VariableName);
      virtual bool SaveVariables();
    protected :
      virtual double IceGrowth(int ABoxNumber);
      virtual void SkeletalDiffusion(int ABoxNumber, int AIndex, double ASeaBoxDepth);
      virtual void SkeletalBrineAndCongelationFluxes(int ABoxNumber, int AIndex, double ASeaBoxDepth);
      virtual void SkeletalMeltFluxes(int ABoxNumber, int AIndex, double ASeaBoxDepth);
      double DepthOfSurfaceLayer(int ALine, int AColumn);
      virtual void GetIce();
      void PreBuildIce(char* className);
      void BuildIce(char* className);
      double *SIarea/*Fractional sea ice cover (m/m)*/,*SIheff/*Sea ice thickness (m)*/,*SIhsnow/*Sea ice snow thickness (m)*/, *PastSIheff;
      //double K_Ice, I0_Ice, Albedo,
      double *BulkDensity, *BulkSalinity, *BulkTemperature,
             *BulkNO3, *BulkNO2, *BulkNH4, *BulkPhosphate, *BulkSilicate , *BulkOxygen,     //microM, except oxygen that is in mg/L
             *BulkDOC, *BulkDON, *BulkDOP,
             *BrineDensity, *BrineSalinity, *BrineTemperature, *BrineVolume, *BrineFractionalVolume,
             *BrineNO3, *BrineNO2, *BrineNH4, *BrinePhosphate, *BrineSilicate , *BrineOxygen,     //microM, except oxygen that is in mg/L
             *BrineDOC, *BrineDON, *BrineDOP,
             *NO3Diffusion, *NO2Diffusion,*NH4Diffusion, *PhosphateDiffusion, *SilicateDiffusion, *OxygenDiffusion,
             *DOCDiffusion,*DONDiffusion,*DOPDiffusion,
             *NO3BrineFlux, *NO2BrineFlux,*NH4BrineFlux, *PhosphateBrineFlux, *SilicateBrineFlux,
             *OxygenBrineFlux, *DOCBrineFlux, *DONBrineFlux,*DOPBrineFlux,
             *NO3BulkFlux, *NO2BulkFlux, *NH4BulkFlux, *PhosphateBulkFlux, *SilicateBulkFlux, *OxygenBulkFlux,
             *DOCBulkFlux,*DONBulkFlux,*DOPBulkFlux, //Is there some dissolved organic silica?
             *BrineFlux,
             *DetritalIceCBrineFlux, *DetritalIceNBrineFlux, *DetritalIcePBrineFlux, *DetritalIceSiBrineFlux,
             *DetritalIceCBulkFlux, *DetritalIceNBulkFlux, *DetritalIcePBulkFlux, *DetritalIceSiBulkFlux,
             *TPMIceBulkFlux, *POMIceBulkFlux, *TPMIceBrineFlux, *POMIceBrineFlux,
             *BrineDetritalIceC, *BrineDetritalIceN, *BrineDetritalIceP, *BrineDetritalIceSi, *BrineTPM, *BrinePOM,
             *BulkDetritalIceC, *BulkDetritalIceN, *BulkDetritalIceP, *BulkDetritalIceSi, *BulkTPM, *BulkPOM,
             *LayerThickness, *IceGrowthRate, *SkeletalLayerDepth,
             *LayerDepth;

      double KforMeltingMYIce,KforMeltingFYIce,KforPondedMYIce,I0forMeltingMYIce,I0forMeltingFYIce,I0forPondedMYIce,
             KforVisibleLightInIce, KforNearInfraRedLightInIce, I0forVisibleLightInIce, I0forNearInfraRedLightInIce, AlbedoForVisibleLightInIce, AlbedoForNearInfraredLightInIce,
             KforVisibleLightInSnow, AlbedoForVisibleLightInSnow, DiffusionCoeff, FreezingPointOfSeawater, fs, SeaBoxDepth;
      int Iteration, UpperLayer, Index, NumberOfBoxesWithIce, NumberOfIceLayers, NumberOfCongelationLayers, NumberOfSkeletalLayers, NumberOfPlateletLayers;
      IceRecord *MyIceRecord;
};

class _export TIceTimeSeries : public TIce
{
    public:
        TIceTimeSeries(TEcoDynClass* PEcoDynClass, char* className);
       	~TIceTimeSeries();
        virtual void ResetPointers();
        virtual void freeMemory();
        virtual void ResetValues();
        bool ReadTimeSeriesValues(int &fileIndex);
        virtual void Go();
        virtual void ReadCurrentValues();
        virtual void ReinitializeValues();
        virtual void Reinitialize();
    protected:
        virtual double IceGrowth(int ABoxNumber);
        int MeanValuesSamplesNumber,indexFile, MyNumberOfBoxes, steps, AStep;
        double *mSIarea, *mSIheff, *mSIhsnow;
        int *MyBoxNumber;
        long *timeSeries;
        long StartTime, meanTimeDiff, myTimeSeriesStep, AMoment;
        VNA* TSVariableNameArray;
        bool firstRead, recursiveCall, FoundFirstStep;
};


class _export TIceTimeSeries3D : public TIceTimeSeries
{
    public:
        TIceTimeSeries3D(TEcoDynClass* PEcoDynClass, char* className);
       	~TIceTimeSeries3D();
        virtual void freeMemory();
        virtual void Go();
    protected:
        virtual void PreBuildIce(char* className);
        virtual void TemperatureProfile(int ABoxNumber);
        virtual void SalinityProfile(int ABoxNumber);
        virtual double FYIceSalinityProfile(int ABoxNumber);
        virtual double MYIceSalinityProfile(int ABoxNumber);
};


static TEcoDynClass *PSeaIce;

#endif //ICEOBJT_H



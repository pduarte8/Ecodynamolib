// Phytoplankton object header file
// REV A - 95.09.25

/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef PHYTOBJT_H
#define PHYTOBJT_H

#include "prodobjt.h"

// DLL header file

#ifndef __BORLANDC__
using namespace std;
#define _export 
#endif  // __BORLANDC__

class _export TPhytoplankton : public TProdutor
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TPhytoplankton(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nParams, double pmax, double iopt, double slope, double beta, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration);
        TPhytoplankton(char* className, float timeStep, char* morphologyFilename,
            char* parametersFilename);

		TPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
#else  // _PORT_FORTRAN_
        TPhytoplankton(char* className);
#endif  // _PORT_FORTRAN_
		~TPhytoplankton();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
	virtual void Go ();
	virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
	virtual void Inquiry(char* srcName, double &Value,int BoxX,int BoxY,char* ParameterName,int AnObjectCode);
	virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
		virtual void Integrate();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

        virtual void SetPhytoParams(double pmax, double iopt,
        double slope, double beta, double aEiler, double bEiler, double cEiler,
       	double kValue, double ks, double kAmmonia,
       	double kNitrate, double pHi, double maintenanceRespiration,
       	double respirationCoefficient, double docLoss, double docStressLoss,
       	double deathLoss, double redfieldCFactor, double redfieldNFactor,
       	double redfieldPFactor, double temperatureAugmentationRate,
       	double fallingSpeed, double ratioLightDarkRespiration);
           
        // AP, 2007.07.11
#ifndef _PORT_FORTRAN_
        virtual bool SaveParameters();
#endif
	protected :
	  long NCounter, PCounter;
        public :
          void PreBuildPhytoplankton();
        protected : 
#ifndef _PORT_FORTRAN_
        void BuildPhytoplankton();
#endif  // _PORT_FORTRAN_
        virtual void Production();
	virtual void ProductionWithTemperature();
	virtual void NutrientLimitation(int ABoxNumber);
        virtual void NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber);
        virtual void TemperatureLimitation(int ABoxNumber);
	virtual void Respiration();
	virtual void Exudation();
	virtual void Senescence(int ABoxNumber);
	virtual void Mortality();
        virtual void Loads();

     typedef struct PhytoplanktonLoads
     {
        char LoadName[210];
        int LineCoordinate, ColumnCoordinate, LayerCoordinate;
        double *PhytoplanktonCLoad;
        double *PhytoplanktonNLoad;
        double *PhytoplanktonPLoad;
     }  LoadRecord;
     LoadRecord* ALoadRecord;
     typedef struct RiverLoads
     {
        char RiverName[210];
        int LineCoordinate, ColumnCoordinate, LayerCoordinate;
        double *PhytoplanktonCRiverLoad;
        double *PhytoplanktonNRiverLoad;
        double *PhytoplanktonPRiverLoad;
        double *PhytoplanktonBiomassLoad;
     }  LoadRiverRecord;
     LoadRiverRecord* ALoadRiverRecord;
     double FallingSpeed, RatioLightDarkRespiration, CriticalDepthForLandBoundary;
     int NumberOfLoads, NumberOfRiverLoads, NumberOfDaysForLoads;
};


class _export TCrestumaLeverPhytoplankton2DV : public TPhytoplankton     // Pedro Duarte 4/2003
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TCrestumaLeverPhytoplankton2DV(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
            double chlToBiomass, double aPhytoBiomass[], double riverPhyto,
            double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
            double simulationInterval,
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration);
        TCrestumaLeverPhytoplankton2DV(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TCrestumaLeverPhytoplankton2DV(TEcoDynClass* APEcoDynClass, char* className);
#else  //_PORT_FORTRAN_
        TCrestumaLeverPhytoplankton2DV(char* className);
#endif
#ifndef _PORT_FORTRAN_
        ~TCrestumaLeverPhytoplankton2DV();
#endif  //_PORT_FORTRAN_
         virtual void Go();
        // AP, 2007.07.11
#ifndef _PORT_FORTRAN_
        virtual bool SaveVariables();
#endif

   protected :
#ifndef _PORT_FORTRAN_
        void PreBuildCrestumaLeverPhyto2DV();
        void BuildCrestumaLeverPhyto2DV();
        void PosBuildCrestumaLeverPhyto2DV();
#endif  //_PORT_FORTRAN_
	  virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void Exudation(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      virtual void NutrientLimitation(int ABoxNumber);
      virtual void Loads();
	int PastUpperLayer, CurrentUpperLayer, GridColumns, GridLines;
   double PhytoplanktonInitialBiomass, PhytoplanktonFinalBiomass, SimulationInterval;
};

class _export TCrestumaLeverPhytoplankton2DVIntLim : public TCrestumaLeverPhytoplankton2DV     // Pedro Duarte 5/2003
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TCrestumaLeverPhytoplankton2DVIntLim(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
            double chlToBiomass, double aPhytoBiomass[], double riverPhyto,
            double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
            double simulationInterval, double nCellQuota, double pCellQuota,
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration, double minNPRatio,
            double maxNPRatio, double pMaxUptake, double nMaxUptake, double kP,
            double kNO3, double kNH4, double minPCellQuota, double maxPCellQuota,
            double minNCellQuota, double maxNCellQuota, double kPInternal,
            double kNInternal, double settlingSpeed, double carbonToOxygenProd,
            double carbonToOxygenResp, double tminRespiration, double tminPhotosynthesis);
        TCrestumaLeverPhytoplankton2DVIntLim(char* className, float timeStep,
            char* morphologyFilename, char* variablesFilename, char* parametersFilename);

		TCrestumaLeverPhytoplankton2DVIntLim(TEcoDynClass* APEcoDynClass,
                char* className);
#else  //_PORT_FORTRAN_
        TCrestumaLeverPhytoplankton2DVIntLim(char* className);
#endif  //_PORT_FORTRAN_

       ~TCrestumaLeverPhytoplankton2DVIntLim();
       virtual void freeMemory();
       virtual double GetParameterValue(char* parmName);
       virtual bool SetParameterValue(char* parmName, double value);
       virtual int GetIntParameterValue(char* parmName);
       virtual bool SetIntParameterValue(char* parmName, int value);
       virtual void Go();
       virtual void Integrate();
       virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
       void SetCLPhytoParams(float nCellQuota, float pCellQuota, float minNPRatio,
       float maxNPRatio, float pMaxUptake, float nMaxUptake, float kP,
       float kNO3, float kNH4, float minPCellQuota, float maxPCellQuota,
       float minNCellQuota, float maxNCellQuota, float kPInternal,
       float kNInternal, float settlingSpeed, float carbonToOxygenProd,
       float carbonToOxygenResp, float tminRespiration, float tminPhotosynthesis);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.11
#ifndef _PORT_FORTRAN_ 
        virtual bool SaveParameters();
        virtual bool SaveVariables();
#endif

   protected :
#ifndef _PORT_FORTRAN_
        void BuildCrestumaLeverPhyto2DVIntLim();
#endif  //_PORT_FORTRAN_
   public :
        void PreBuildCrestumaLeverPhyto2DVIntLim();
   protected : 
        void PosBuildCrestumaLeverPhyto2DVIntLim();

   	virtual void Production(int ALine, int AColumn);
        virtual void Respiration(int ABoxNumber);
   public : 
        virtual void Exudation(int ABoxNumber);
        virtual void Mortality(int ABoxNumber);
#ifndef _PORT_FORTRAN_
        virtual void NitrogenUptake(int ABoxNumber);
        virtual void PhosphorusUptake(int ABoxNumber);
        virtual void SilicaUptake(int ABoxNumber); 
#else
        virtual void NitrogenUptake(int ABoxNumber, double Ammonia, double Nitrate, double Nitrite);
        virtual void PhosphorusUptake(int ABoxNumber, double Phosphate);
        virtual void SilicaUptake(int ABoxNumber, double Silica); 
#endif
        virtual void NutrientLimitation(int ABoxNumber);
        virtual double TemperatureArrheniusExponentialLimitation(double ATemperature, double AReferenceTemperature);
        virtual void Settling(int ALine, int AColumn);
        double *NCellQuota, *PCellQuota, *NUptake, *PUptake,
             *PhytoNLoad, *PhytoPLoad, *NPhyto, *PPhyto,
             *NCellFlux, *PCellFlux, *DailyMeanGPP, *NumberOfParcels, *SettlingLoss,
             *SiCellQuota, *SiUptake, *PhytoSiLoad, *SiPhyto, *SiCellFlux, *NLimFactor,*PLimFactor,*SiLimFactor;
       int *ADay;
       double MinNPRatio,MaxNPRatio, MinNSiRatio, MaxNSiRatio, PMaxUptake,NMaxUptake, SiMaxUptake,KP, KNO3,
             KNH4, KSi, MaxPCellQuota, MaxNCellQuota, MaxSiCellQuota, MinPCellQuota, MinNCellQuota, MinSiCellQuota,
             KPInternal, KNInternal, KSiInternal,SettlingSpeed, CarbonToOxygenProd, CarbonToOxygenResp,
             TminPhotosynthesis, TminRespiration,AmmoniaUpTake, NitrateAndNitriteUptake, ExudatedFlux;
       int NitrogenLimitation, PhosphorusLimitation, SilicaLimitation, PIFunction, NutrientLimitationType, Line, Column, Layer;      
};



#define STEELE 'S'
#define MICHAELIS_MENTEN 'M'
#define EILER 'E'

class _export TPhytoplanktonGeneric : public TCrestumaLeverPhytoplankton2DVIntLim
{
	public :
#ifndef _PORT_FORTRAN_
        TPhytoplanktonGeneric(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
            double chlToBiomass, double aPhytoBiomass[], double riverPhyto,
            double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
            double simulationInterval, double nCellQuota, double pCellQuota,
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration, double minNPRatio,
            double maxNPRatio, double pMaxUptake, double nMaxUptake, double kP,
            double kNO3, double kNH4, double minPCellQuota, double maxPCellQuota,
            double minNCellQuota, double maxNCellQuota, double kPInternal,
            double kNInternal, double settlingSpeed, double carbonToOxygenProd,
            double carbonToOxygenResp, double tminRespiration, double tminPhotosynthesis);
        TPhytoplanktonGeneric(char* className, float timeStep,
            char* morphologyFilename, char* variablesFilename, char* parametersFilename);

		TPhytoplanktonGeneric(TEcoDynClass* APEcoDynClass,
                char* className);
#else  //_PORT_FORTRAN_
        static TPhytoplanktonGeneric* getPhyto();
        static TPhytoplanktonGeneric* getPhyto(TPhytoplanktonGeneric* pphytoplankton);
        TPhytoplanktonGeneric(char* className);
        void PreBuildPhytoplanktonGeneric(char* className);
        void SetABoxDepth(double layerThickness) {BoxDepth = layerThickness;}    
        void SetLightAtTop(double lightAtTop){LightAtTop=lightAtTop;}
        void SetLightAtBottom(double lightAtBottom){LightAtBottom=lightAtBottom;}
        void SetWaterTemperature(double waterTemperature){WaterTemperature=waterTemperature;}
        void SetTimeStep(double timeStep) {TimeStep = timeStep;} 
        void SetABoxNumber (int aBoxNumber) {ABoxNumber = aBoxNumber;}         
        void SetNumberOfLines(int numberOfLines) {NumberOfLines = numberOfLines;}
        void SetNumberOfColumns(int numberOfColumns) {NumberOfColumns = numberOfColumns;}
        void SetNumberOfLayers(int numberOfLayers) {NumberOfLayers = numberOfLayers;}
        void SetNumberOfBoxes(int numberOfBoxes) {NumberOfBoxes = numberOfBoxes;}
        void SetTmin(double tmin) {Tmin = tmin;} 
        double GetKValue() {return KValue;}
        double GetDocLoss() {return DocLoss;}          
        double GetBiomass(int box) {return PhytoBiomass[box];}
        char debug;
#endif  //_PORT_FORTRAN_
       ~TPhytoplanktonGeneric();
       virtual void Go();

    protected :
        virtual void GetLightIntensity();
        virtual void Production(int ABoxNumber);
    public :
        virtual void Respiration(int ABoxNumber);
        double SteeleProduction();
        double Michaelis_MentenProduction();
        double EilerProduction();
        double PlattProduction();
	double TPhytoplanktonGeneric::Platt1();
        double JassyAndPlattProduction();
        double SteeleSlope();
        double EilersAndPeetersSlope();
        double MichaelisMentenSlope();
        virtual void Settling(int ABoxNumber);
        virtual double TemperatureArrheniusExponentialLimitation(int ABoxNumber);
        void DailyAverageProduction();

        char EquationName;
        double BoxDepth, MyUpperDepth, LightAtTop, LightAtBottom,
                WaterTemperature, Tmin, MyDay, RhoChl;
        int ABoxNumber, AUpperBoxNumber;
        bool SurfaceCell, BottomBox;
        double aMin;
/*#ifdef _PORT_FORTRAN_
    private:
        static TPhytoplanktonGeneric *PPhytoplankton;
#endif  //_PORT_FORTRAN_*/
};

#ifndef _PORT_FORTRAN_
static TEcoDynClass *PPhytoplankton;
#else  //_PORT_FORTRAN_

/* Functions that can be called from Fortran */
extern "C" {
    void phyto_getvalues__(float* phytobiomass);
	void phyto_getbiomass__(float* phytobiomass);
/*
 * This constructor calls Go() and Integrate() after initialisation
 */
    void phytoplankton__(long* PPhytoplankton,int* box, float* depth, float* biomass,
                        float* lightAtTop, float* lightAtBottom,
                        float* waterTemperature, float* timeStep);

    void phyto_setparams__(long* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope, double* beta, double* aEiler, double* bEiler, double* cEiler, double* kValue,
                            double* Ks, double* phi, double* maintenanceRespiration, double* respirationCoefficient,double* docLoss, double* docStressLoss,
                            double* deathLoss, double* redfieldCFactor, double* redfieldNFactor,double* redfieldPFactor, double* temperatureAugmentationRate,
                            double* ratioLightDarkRespiration, double* minNPRatio,
                            double* maxNPRatio, double* pMaxUptake, double* nMaxUptake, double* kP,double* kNO3, double* kNH4, double* minPCellQuota, double* maxPCellQuota,
                            double* minNCellQuota, double* maxNCellQuota, double* kPInternal,double* kNInternal, double* settlingSpeed, double* carbonToOxygenProd,
                            double* carbonToOxygenResp, double* tminRespiration,double* tminPhotosynthesis, double* layerThickness, double* lightAtTop, double* lightAtBottom,
                            double* waterTemperature, int* piCurveOption);
	
    void phyto_setdebug__(char* Debug);
    void phyto_setdepth__(int* box, float* depth);
    void phyto_setkvalue__(float* kvalue);
    void phyto_setbiomass__(int* box, float* biomass);

    void phytoplankton_new__(long* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope,double* beta, double* aEiler, double* bEiler, double* cEiler, 
                            double* maintenanceRespiration, double* respirationCoefficient,double* docStressLoss,
                            double* deathLoss, double* redfieldCFactor, double* redfieldNFactor,double* redfieldPFactor, double* temperatureAugmentationRate,
                            double* ratioLightDarkRespiration, double* minNPRatio,double* maxNPRatio, double* pMaxUptake, double* nMaxUptake, double* kP,double* kNO3, 
                            double* kNH4, double* minPCellQuota, double* maxPCellQuota,double* minNCellQuota, double* maxNCellQuota, double* kPInternal,double* kNInternal, 
                            double* settlingSpeed, double* carbonToOxygenProd,double* carbonToOxygenResp, double* tminRespiration,double* tminPhotosynthesis,
                            int* nitrogenLimitation, int* phosphorusLimitation, int* silicaLimitation, double* maxSiCellQuota, double* minSiCellQuota, 
                            double* minNSiRatio, double* siMaxUptake,
                            double* kSi, double* kSiInternal, double* redfieldSi, int* pifunction, int* nutrientLimitationType);

    void phytoplankton_go__(long* PPhytoplankton, double* layerThickness, double* timeStep);

    void phytoplankton_production__(long* PPhytoplankton, double* lightAtTop, double* lightAtBottom, double* kValue,double* waterTemperature,
                                    double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* SiPhyto, double* biomass, double *Slope, double* Chl2Carbon, double *OxygenProduction, int *line, int *column, int *layer, double* layerThickness);

    void phytoplankton_respiration__(long* PPhytoplankton, double* waterTemperature, double* cffCRespiration, double *GrossProduction, double* biomass, double *Oxygen, double* Chl2Carbon, double *OxygenConsumption);

    void phytoplankton_exudation__(long* PPhytoplankton, double* cffCExudation, double *GrossProduction, double* biomass, double *NCellQuota, double *PCellQuota);

    void phytoplankton_nitrogen_uptake__(long* PPhytoplankton, double* Ammonia, double* Nitrate, double* Nitrite,double* cffNH4, double *cffNO3NO2, double* nPhyto, double* pPhyto, double* biomass);  

    void phytoplankton_phosphorus_uptake__(long* PPhytoplankton, double* Phosphate,double* cffPO4, double* nPhyto, double* pPhyto, double* biomass); 
 
    void phytoplankton_silica_uptake__(long* PPhytoplankton, double* Silicate,double* cffSiOH4, double* nPhyto, double *siPhyto, double* biomass);
  
    void phytoplankton_external_nut_limitation__(long* PPhytoplankton, double* Ammonia, double* Nitrate,double* Nitrite, double* Phosphate, double* Silicate, double* Limitation);
    void phytoplankton_rhochl_(long* PPhytoplankton, double* light, double* kValue, double* GrossProduction, double* slope, double* rhochl);  

    void phytoplankton_mortality__(long* PPhytoplankton, double* nCellQuota, double* pCellQuota,
                               double* waterTemperature, double* biomass, double* timeStep, double* cff); 

    void phytoplankton_integrate__(long* PPhytoplankton,double* nCellQuota, double* pCellQuota,double* biomass); 
   
/*
 * calls Go() and Integrate()
 */
	void phyto_go__();
}
#endif  //_PORT_FORTRAN_

#endif  //PHYTOBJT_H




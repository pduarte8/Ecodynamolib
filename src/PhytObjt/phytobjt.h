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
            int nParams, double pmax, double iopt, double slope, double aEiler,
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
        	double slope, double aEiler, double bEiler, double cEiler,
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

#ifndef _PORT_FORTRAN_

class _export TPhytoplanktonBasic : public TPhytoplankton
{
	public :
        // constructors invoked outside EcoDyn
        TPhytoplanktonBasic(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
            double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
            double aChlCarbonRatio[], double aChlToBiomass[],
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration);
        TPhytoplanktonBasic(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TPhytoplanktonBasic(TEcoDynClass* APEcoDynClass, char* className);
        // AP, 2007.07.11
        virtual bool SaveVariables();

    protected:
        void BuildPhytoplanktonBasic();
};

class _export TTejoPhytoplankton : public TPhytoplanktonBasic
{
	public :
        // constructors invoked outside EcoDyn
        TTejoPhytoplankton(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
            double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
            double aChlCarbonRatio[], double aChlToBiomass[],
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration,
            double Q10PH, double rTMPH);
        TTejoPhytoplankton(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TTejoPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
      void BuildTejoPhyto();
	  virtual void Senescence(int ABoxNumber);
};

class _export TTejoNPSiPhytoplankton : public TTejoPhytoplankton
{
	public :
		TTejoNPSiPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
		double KsDIN,KsPO4;
		virtual void NutrientLimitation(int ABoxNumber);
};

class _export TEilerPhytoplankton : public TPhytoplanktonBasic     // Pedro Duarte 4/96
{
	public :
		TEilerPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TEilerPhytoplankton();
        virtual void freeMemory();
		virtual void Integrate();
	protected :
		virtual void Production();
};


class _export TDynamicPhytoplankton : public TEilerPhytoplankton   // Pedro Duarte 4/96
{
	public :
		TDynamicPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TDynamicPhytoplankton();
        virtual void freeMemory();
       virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
		virtual void Production();
};

class _export TAdaptativePhytoplankton : public TEilerPhytoplankton  // Pedro Duarte 4/96
{
	public :
		TAdaptativePhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TAdaptativePhytoplankton();
        virtual void freeMemory();
		//virtual void go();
	protected :
		virtual void LightShadeAdaptation();
		virtual void Production();
};

class _export TDynamicAdaptativePhytoplankton : public TAdaptativePhytoplankton  // Pedro Duarte 4/96
{
	public :
		TDynamicAdaptativePhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TDynamicAdaptativePhytoplankton();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		//virtual void go();
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
		virtual void Production();
};

class _export TAdaptativeSlopePhytoplankton : public TAdaptativePhytoplankton   // Pedro Duarte 4/96
{
	public :
		TAdaptativeSlopePhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TAdaptativeSlopePhytoplankton();
        virtual void freeMemory();
	protected :
		virtual void Production();
};


class _export TCalaPhytoplankton : public  TTejoPhytoplankton
{
	public :
		TCalaPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
		double
			ThresholdNH4;

		virtual void NutrientLimitation(int ABoxNumber);
};

class _export TEilerVerticalModelPhytoplankton : public TPhytoplanktonBasic   // Pedro Duarte 3/98
{
	public :
		TEilerVerticalModelPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TEilerVerticalModelPhytoplankton();
      virtual void Go();
		virtual void Integrate();
	protected :
		virtual void Production();
      virtual void Exudation();
      virtual void Falling();
      virtual void Mortality();
};

class _export TPhytoplanktonBiomassConstant : public TEilerVerticalModelPhytoplankton   // Pedro Duarte 7/99
{
	public :
		TPhytoplanktonBiomassConstant(TEcoDynClass* APEcoDynClass, char* className);
		virtual void Integrate();
	protected :
      virtual void Falling();
      virtual void Mortality();
};

class _export TDynamicVerticalModelPhytoplankton : public TEilerVerticalModelPhytoplankton  // Pedro Duarte 3/98
{
	public :
		TDynamicVerticalModelPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TDynamicVerticalModelPhytoplankton();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
		virtual void Production();
};

class _export TAdaptativeVerticalModelPhytoplankton : public TEilerVerticalModelPhytoplankton   // Pedro Duarte 3/98
{
	public :
		TAdaptativeVerticalModelPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TAdaptativeVerticalModelPhytoplankton();
		//virtual void go();
	protected :
		virtual void LightShadeAdaptation();
		virtual void Production();
};

class _export TDynamicAdaptativeVerticalModelPhytoplankton : public TAdaptativeVerticalModelPhytoplankton  // Pedro Duarte 3/98
{
	public :
		TDynamicAdaptativeVerticalModelPhytoplankton(TEcoDynClass* APEcoDynClass,
            char* className);
		~TDynamicAdaptativeVerticalModelPhytoplankton();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		//virtual void go();
        // AP, 2007.07.11
        virtual bool SaveParameters();

	protected :
		virtual void Production();
};

class _export TAdaptativeSlopeVerticalModelPhytoplankton : public TAdaptativeVerticalModelPhytoplankton  // Pedro Duarte 3/98
{
	public :
		TAdaptativeSlopeVerticalModelPhytoplankton(TEcoDynClass* APEcoDynClass,
                char* className);
		~TAdaptativeSlopeVerticalModelPhytoplankton();
	protected :
		virtual void Production();
};


class _export TRiaFormosaPhytoplankton : public TPhytoplanktonBasic
{
	public :
        // constructors invoked outside EcoDyn
        TRiaFormosaPhytoplankton(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
            double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
            double aChlCarbonRatio[], double aChlToBiomass[], double aOceanDailyChlorophyll[],
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration,
            double carbonToOxygenProd, double carbonToOxygenResp);
        TRiaFormosaPhytoplankton(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TRiaFormosaPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
      ~TRiaFormosaPhytoplankton();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Integrate();
        // AP, 2007.07.11
        virtual bool SaveParameters();
        virtual bool SaveVariables();

	protected :
        void PreBuildRiaFormosaPhyto();
        void BuildRiaFormosaPhyto();
	   virtual void Production();
      virtual void Respiration();
      virtual void Exudation();
      double CarbonToOxygenProd, CarbonToOxygenResp;
      double OceanDailyChlorophyll[365];
      double *OxygenNetProduction;
};


class _export TSto0Phytoplankton : public TPhytoplanktonBasic
{
	public :
		TSto0Phytoplankton(TEcoDynClass* APEcoDynClass, char* className);
      ~TSto0Phytoplankton();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Integrate();
        // AP, 2007.07.11
        virtual bool SaveParameters();
        virtual bool SaveVariables();
	protected :
	   virtual void Production();
      virtual void Respiration();
      virtual void Exudation();
      virtual void CalculatePmax();
      virtual void CalculateImax();
      double CarbonToOxygenProd, CarbonToOxygenResp, LagoonDepth;
      double *OxygenNetProduction, *DailyAverageBiomass;
};


class _export TSangoPhytoplankton : public TPhytoplankton
{
	public :
        // constructors invoked outside EcoDyn
        TSangoPhytoplankton(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
            double chlToBiomass, double aPhytoBiomass[], double aDailyBoundaryPhytoplankton[],
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration);
        TSangoPhytoplankton(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TSangoPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
      ~TSangoPhytoplankton();
      virtual void freeMemory();
      virtual void Go();
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Integrate();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.11
        virtual bool SaveVariables();

   protected :
      void PreBuildSangoPhytoplankton();
      void BuildSangoPhytoplankton();
      virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
	  virtual void Exudation(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      virtual void NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber);
   double *DailyBoundaryPhytoplankton;
   double ATimeStep;
   int GridColumns, GridLines,
   	 WesternBoundaryNumber, SouthernBoundaryNumber,
       EasternBoundaryNumber, NorthernBoundaryNumber;
};

class _export TSangoPhytoplanktonIntLim : public TSangoPhytoplankton     // Pedro Duarte
{
   public:
      TSangoPhytoplanktonIntLim(TEcoDynClass* APEcoDynClass, char* className);
      ~TSangoPhytoplanktonIntLim();
      virtual void freeMemory();
      virtual void Go();
      virtual void Integrate();
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.11
        virtual bool SaveParameters();
        virtual bool SaveVariables();
   protected:
      void PreBuildSangoPhytoplanktonIntLim();
      void PosBuildSangoPhytoplanktonIntLim();
      virtual void NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber);
      virtual void NitrogenUptake(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      double NMaxUptake, KNO3, KNH4, MinNCellQuota, MaxNCellQuota, KNInternal;
      double *NCellQuota, *NUptake,*NPhyto,*NCellFlux;
};


class _export TCrestumaLeverPhytoplankton : public TPhytoplanktonBasic     // Pedro Duarte
{
	public :
        // constructors invoked outside EcoDyn
        TCrestumaLeverPhytoplankton(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
            double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
            double aChlCarbonRatio[], double aChlToBiomass[],
            double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
            double simulationInterval,
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration);
        TCrestumaLeverPhytoplankton(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TCrestumaLeverPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
		virtual void Go();
        // AP, 2007.07.11
        virtual bool SaveVariables();
	protected :
        void BuildCrestumaLeverPhyto();
		virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void Exudation(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      virtual void NutrientLimitation(int ABoxNumber);

   double PhytoplanktonInitialBiomass, PhytoplanktonFinalBiomass, SimulationInterval;
};

#endif

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
#else
        virtual void NitrogenUptake(int ABoxNumber, double Ammonia, double Nitrate, double Nitrite);
        virtual void PhosphorusUptake(int ABoxNumber, double Phosphate);
#endif
        virtual void NutrientLimitation(int ABoxNumber);
        virtual double TemperatureArrheniusExponentialLimitation(double ATemperature, double AReferenceTemperature);
        virtual void Settling(int ALine, int AColumn);
        double *NCellQuota, *PCellQuota, *NUptake, *PUptake,
             *PhytoNLoad, *PhytoPLoad, *NPhyto, *PPhyto,
             *NCellFlux, *PCellFlux, *DailyMeanGPP, *NumberOfParcels, *SettlingLoss;
       int *ADay;
       double MinNPRatio,MaxNPRatio, PMaxUptake,NMaxUptake,KP, KNO3,
             KNH4, MaxPCellQuota, MaxNCellQuota, MinPCellQuota, MinNCellQuota,
             KPInternal, KNInternal, SettlingSpeed, CarbonToOxygenProd, CarbonToOxygenResp,
             TminPhotosynthesis, TminRespiration,AmmoniaUpTake, NitrateAndNitriteUptake, ExudatedFlux;
       int NitrogenLimitation, PhosphorusLimitation;      
};

#ifndef _PORT_FORTRAN_
class _export TPhytoplanktonTimeSeries : public TPhytoplankton
{
	public :
  		TPhytoplanktonTimeSeries(TEcoDynClass* APEcoDynClass, char* className);
		~TPhytoplanktonTimeSeries();
        virtual void freeMemory();
		virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.11
        virtual bool SaveVariables();
	protected :
        void BuildPhytoplankton(char* className);
        double *ForcingPhytoplankton, *ForcingChlToBiomass, *ForcingChlCarbonRatio;
        double PhytoEnergeticContents;
        int  NumberOfDays, StartYear, StartMonth, StartDate, StartJulianDay, StartCurrentTime,
             FinishYear, FinishMonth, FinishDate, FinishJulianDay, FinishCurrentTime;
};


class _export TRiaF2DPhytoplankton : public TCrestumaLeverPhytoplankton2DVIntLim
{
   public :
      TRiaF2DPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
      ~TRiaF2DPhytoplankton();
      virtual void freeMemory();
      virtual void Go();
      virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Integrate();
        // AP, 2007.07.11
        virtual bool SaveVariables();
   protected :
      void BuildRiaF2DPhytoplankton();
      virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void Exudation(int ABoxNumber);
      virtual void NutrientLimitation(int ABoxNumber);
      virtual void NitrogenUptake(int ABoxNumber);
      virtual void PhosphorusUptake(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      virtual void Settling(int ABoxNumber);
      virtual void Loads();
      virtual double TemperatureArrheniusExponentialLimitation(double ATemperature, double AReferenceTemperature);
   int MyNumberOfBoxesInSubDomain;
   double *DailyBoundaryPhytoplankton, *OxygenNetProduction,
          *PhytoNLoad, *PhytoPLoad, *PhytoPlanktonCRiverLoad, *PhytoPlanktonNRiverLoad, *PhytoPlanktonPRiverLoad;
   /*double *NCellQuota, *PCellQuota, *NUptake, *PUptake,
          *PhytoNLoad, *PhytoPLoad, *NPhyto, *PPhyto,
          *NCellFlux, *PCellFlux, *DailyMeanGPP, *NumberOfParcels;
   double MinNPRatio,MaxNPRatio, PMaxUptake,NMaxUptake,KP, KNO3,
          KNH4, MaxPCellQuota, MaxNCellQuota, MinPCellQuota, MinNCellQuota,
          KPInternal, KNInternal, SettlingSpeed, CarbonToOxygenProd, CarbonToOxygenResp,
          TminPhotosynthesis, TminRespiration;*/
};


class _export TCarlPhytoplankton : public TRiaF2DPhytoplankton
{
	public :
        // constructors invoked outside EcoDyn
        /*TCarlPhytoplankton(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
            double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
            double aChlCarbonRatio[], double aChlToBiomass[],
            int nParams, double pmax, double iopt, double slope, double aEiler,
            double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
            double kNitrate, double pHi, double maintenanceRespiration,
            double respirationCoefficient, double docLoss, double docStressLoss,
            double deathLoss, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double temperatureAugmentationRate,
            double fallingSpeed, double ratioLightDarkRespiration,
            double Q10PH, double rTMPH);
        TCarlPhytoplankton(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);  */

            TCarlPhytoplankton(TEcoDynClass* APEcoDynClass, char* className);
            virtual void Integrate();
            virtual void Go ();
	protected :
        void BuildCarlPhyto();
        virtual void Production();
        virtual void NutrientLimitation(int ABoxNumber);
	virtual void Respiration();
	virtual void Exudation();
	virtual void Mortality();
        virtual void LightShadeAdaptation();
};

class _export TPhytoplankton3D : public TRiaF2DPhytoplankton
{
	public :
        TPhytoplankton3D(TEcoDynClass* APEcoDynClass, char* className);
        virtual void BuildTPhytoplankton3D();
        virtual void Integrate();
        virtual void Go ();
        // AP, 2009.04.08
        virtual bool SaveVariables();
	protected :
        virtual void Production(int ABoxNumber, double PARLightAtTop, double PARLightAtBottom);
        virtual void Settling(int ABoxNumber);
        virtual void Loads();
    int UpperLayer;
};
#endif

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

    void phyto_setparams__(long* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope, double* aEiler, double* bEiler, double* cEiler, double* kValue,
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

    void phytoplankton_new__(long* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope, double* aEiler, double* bEiler, double* cEiler, 
                            double* maintenanceRespiration, double* respirationCoefficient,double* docStressLoss,
                            double* deathLoss, double* redfieldCFactor, double* redfieldNFactor,double* redfieldPFactor, double* temperatureAugmentationRate,
                            double* ratioLightDarkRespiration, double* minNPRatio,double* maxNPRatio, double* pMaxUptake, double* nMaxUptake, double* kP,double* kNO3, 
                            double* kNH4, double* minPCellQuota, double* maxPCellQuota,double* minNCellQuota, double* maxNCellQuota, double* kPInternal,double* kNInternal, 
                            double* settlingSpeed, double* carbonToOxygenProd,double* carbonToOxygenResp, double* tminRespiration,double* tminPhotosynthesis, 
                            int* nitrogenLimitation, int* phosphorusLimitation);

    void phytoplankton_go__(long* PPhytoplankton, double* layerThickness, double* timeStep);

    void phytoplankton_production__(long* PPhytoplankton, double* lightAtTop, double* lightAtBottom, double* kValue,double* waterTemperature,
                                    int* piCurveOption, double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* biomass, double *TIC, double *Slope, double* Chl2Carbon, double *OxygenProduction);

    void phytoplankton_respiration__(long* PPhytoplankton, double* waterTemperature, double* cffCRespiration, double *GrossProduction, double* biomass, double *Oxygen, double* Chl2Carbon, double *OxygenConsumption);

    void phytoplankton_exudation__(long* PPhytoplankton, double* cffCExudation, double *GrossProduction, double* biomass, double *NCellQuota, double *PCellQuota);

    void phytoplankton_nitrogen_uptake__(long* PPhytoplankton, double* Ammonia, double* Nitrate, double* Nitrite,double* cffNH4, double *cffNO3NO2, double* nPhyto, double* biomass);  

    void phytoplankton_phosphorus_uptake__(long* PPhytoplankton, double* Phosphate,double* cffPO4, double* pPhyto, double* biomass); 

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




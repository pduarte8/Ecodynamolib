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

#endif

#define STEELE 'S'
#define MICHAELIS_MENTEN 'M'
#define EILER 'E'


#endif  //PHYTOBJT_H




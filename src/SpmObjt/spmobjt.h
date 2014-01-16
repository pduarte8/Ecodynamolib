// SPM object header file
// REV A - 96.02.27

#ifndef SPMOBJT_H
#define SPMOBJT_H
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifndef __BORLANDC__
	#include <iostream>
	
	using namespace std;
	#define _export 
	
#endif  // __BORLANDC__

#include "EcoDynClass.h"

// DLL header file

class _export TSPM : public TEcoDynClass
{
    public :
      // constructors invoked outside EcoDyn
      TSPM(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
          double aLength[], double aWidth[], double aElevation[], int aType[],
          int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
      TSPM(char* className, float timeStep, char* morphologyFilename);
      TSPM(TEcoDynClass* APEcoDynClass, char* className);
      ~TSPM();
      virtual void freeMemory();
      virtual void Go();
      virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,int BoxX,int BoxY,char* ParameterName,int AnObjectCode);
      virtual void Integrate();
      // AP, 2006.05.26
      virtual bool SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName);
    protected :
      // Grain size record definition
      typedef struct ParticleSpecs
      {
         double SettlingVelocity,       // m day-1
                Density,                // kg m-3
                ESD,                    // mm
                phi,                    // phi = log2 particle size in mm
                PercentOrganic;         // %
      } ParticleRecord;

      ParticleRecord* ParticleData;
      double POCFraction,TurbulenceFactor;
      int GrainClasses; // Number of relevant classes for a
                        // particular ecosystem
      // Stokes equation
      double LocalGravity,LocalLatitude,LocalDensity,LocalViscosity,LocalHeight,LocalSalt,LocalTemperature,
             RedfieldCFactor,RedfieldNFactor,RedfieldPFactor;
      int    NumberOfDaysForTimeSeries,NumberOfHoursForTimeSeries, NumberOfMomentsForTimeSeries;
      double *SPMConcentration,*SPMLoad,*SPMFlux,*SPMRessuspension,*SPMSedimentationRate,
             *TotalExtinctionCoefficient,*ChlorophyllExtinctionCoefficient;
      double RiverSPM,OceanSPM;
      AdvectionDiffusionRecord *SPMInterfaceExchange;
      void BuildSPM(char* className);
      virtual void Flocculate();
      virtual void Deposit();
      virtual void Turbulence();
      virtual void Ressuspend();
      virtual void Dissolve();
      virtual void Mineralize();
      virtual void SettlingSpeed();
      virtual double SPMFraction(int k,double TotalSPM);
      virtual void ExtinctionCoefficientKTot();
      virtual void ExtinctionCoefficientKChl();
};

class _export TSPMBasic : public TSPM
{
    public :
      // constructors invoked outside EcoDyn
      TSPMBasic(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
          double aLength[], double aWidth[], double aElevation[], int aType[],
          int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
          int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
          double aSPMConcentration[], double aSPMLoad[],
          int nParams, double localLatitude, double localHeight, double localSalt,
          double localTemperature, double turbulenceFactor, double aSPMRessuspension[]);
      TSPMBasic(char* className, float timeStep, char* morphologyFilename,
              char* variablesFilename, char* parametersFilename);

      TSPMBasic(TEcoDynClass* APEcoDynClass, char* className);
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      // AP, 2007.07.10
      virtual bool SaveVariables();
      virtual bool SaveParameters();

    protected :
      void PreBuildSPMBasic();
      void BuildSPMBasic();
      void PosBuildSPMBasic();
      virtual void Deposit();
      virtual void Turbulence();
      virtual void SettlingSpeed();
      virtual double SPMFraction(int k, double TotalSPM);
      virtual void ExtinctionCoefficientKTot();
      virtual void ExtinctionCoefficientKChl();
};

class _export TSeeds : public TSPMBasic
{
   public :
      TSeeds(TEcoDynClass* APEcoDynClass, char* className);
      ~TSeeds();
      virtual void BuildSeeds();
      virtual void freeMemory();
      virtual void Go();
      virtual void Integrate();
      virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
   protected:
      virtual void BuildSeedsRecord();
      BenthicSpeciesRecord *ZosteraNoltiiSeedsRecord;
      int NumberOfTypes, NumberOfBoxesWithBenthicSpecies, NumberOfBoxesWithSeeds, MyNumberOfBoxesInSubDomain;
      int *SeedType, *TypesPerBox, *CurrentPosition;
      double *SeedNumberPerTypeInWater, *SeedNumberPerTypeSettled, * SeedAge, *SeedNumberPerBox,
             *SeedInWaterFlux, *SeedSettlingFlux, *SeedLoad, *Seeds;
      bool *GerminationStatus;
      double SettlingVelocity/*m s-1*/, TimeOfPositiveBuoyancy /*days*/, CriticalWaterDepth;
      BenthicSpeciesRecord *MyBenthicSpeciesRecord;
      BenthicSpeciesRecord *SeedsRecord;
      virtual void Deposit(int index, double Depth);
};

class _export TSPMOceanic : public TSPMBasic
{
    public :
        // constructors invoked outside EcoDyn
        TSPMOceanic(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
            double aSPMConcentration[], double aSPMLoad[],
            int nParams, double localLatitude, double localHeight, double localSalt,
            double localTemperature, double turbulenceFactor, double aSPMRessuspension[]);
        TSPMOceanic(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);
        TSPMOceanic(TEcoDynClass* APEcoDynClass, char* className);
        ~TSPMOceanic();
        virtual void freeMemory();
    protected:
       virtual void ExtinctionCoefficientKTot(){};
       virtual void ExtinctionCoefficientKChl();
};

class _export TCarlingfordSPM : public TSPMBasic
{
	public :
        // constructors invoked outside EcoDyn
        TCarlingfordSPM(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
            double aSPMConcentration[], double aSPMLoad[],
            int nParams, double localLatitude, double localHeight, double localSalt,
            double localTemperature, double turbulenceFactor, double aSPMRessuspension[]);
        TCarlingfordSPM(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TCarlingfordSPM(TEcoDynClass* APEcoDynClass, char* className);
		virtual void Go();
	protected:
		virtual void ExtinctionCoefficientKTot();
};


class _export TVerticalModelSPMOceanic : public TSPMOceanic     //Pedro Duarte 6/4/98
{
	public :
        // constructors invoked outside EcoDyn
        TVerticalModelSPMOceanic(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
            double aSPMConcentration[], double aSPMLoad[],
            double riverPOC, double oceanPOC, double aPOCConcentration[], double aPOCLoad[],
            double riverPON, double oceanPON, double aPONConcentration[], double aPONLoad[],
            int nParams, double localLatitude, double localHeight, double localSalt,
            double localTemperature, double turbulenceFactor, double aSPMRessuspension[],
            double detritusBreakDown, double redfieldCFactor, double redfieldNFactor,
            double redfieldPFactor, double pocFraction, double spmSedimentationRate);
        TVerticalModelSPMOceanic(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TVerticalModelSPMOceanic(TEcoDynClass* APEcoDynClass, char* className);
      ~TVerticalModelSPMOceanic();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Integrate();
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
      virtual void Go();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.10
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected :
        void PreBuildVerticalModelSPM();
        void BuildVerticalModelSPM();
   	    virtual void Mineralize();
        virtual void Falling();

   double DetritusBreakDown,
          RiverPOC,
          OceanPOC,
          RiverPON,
          OceanPON;

   double *POCConcentration,*POCLoad,*POCFlux,*PONConcentration,*PONLoad,*PONFlux;
};



class _export TSPMRiaFormosa : public TSPMBasic
{
	public :
        // constructors invoked outside EcoDyn
        TSPMRiaFormosa(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
            double aSPMConcentration[], double aSPMLoad[], double aDailySPMConcentration[],
            int nParams, double localLatitude, double localHeight, double localSalt,
            double localTemperature, double turbulenceFactor, double aSPMRessuspension[],
            double pocFraction);
        TSPMRiaFormosa(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TSPMRiaFormosa(TEcoDynClass* APEcoDynClass, char* className);
		~TSPMRiaFormosa();
      virtual void Go();
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
      virtual void Integrate();
	protected:
        void PreBuildSPMRiaFormosa();
        void BuildSPMRiaFormosa();
        virtual void ExtinctionCoefficientKTot();
      double *DailySPMConcentration;
};

class _export TSPMRiaFormosaRessuspendDeposit : public TSPMBasic
{
	public :
        // constructors invoked outside EcoDyn
        TSPMRiaFormosaRessuspendDeposit(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
            double aSPMConcentration[], double aSPMLoad[],
            int nParams, double localLatitude, double localHeight, double localSalt,
            double localTemperature, double turbulenceFactor, double aSPMRessuspension[]);
        TSPMRiaFormosaRessuspendDeposit(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TSPMRiaFormosaRessuspendDeposit(TEcoDynClass* APEcoDynClass, char* className);
      virtual void Go();
      virtual void Integrate();
	protected:
   	virtual void Deposit();
	 	virtual void Ressuspend();
		virtual void ExtinctionCoefficientKTot();
   double DepositedFlux;
};

class _export TSangoResuspendDeposit : public TSPM
{
	public :
        // constructors invoked outside EcoDyn
        TSangoResuspendDeposit(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double totalExtinctionCoefficient,
            double aSPMConcentration[], double aPOMConcentration[],
            double aBoundarySPMConcentration[], double aBoundaryPOMConcentration[],
            int nParams, double karmanConstant, double dragCoefficient,
            double criticalVelocity, double sinkingVelocity, double nContents,
            double detritusBreakdown, double bottomSedimentOrganicContents,
            double aErosionRate[], double aRoughness[]);
        TSangoResuspendDeposit(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TSangoResuspendDeposit(TEcoDynClass* APEcoDynClass, char* className);
      ~TSangoResuspendDeposit();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Integrate();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.10
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected:
        void PreBuildSangoResuspendDeposit();
        void BuildSangoResuspendDeposit();
    	virtual void Deposit(int ABox);
	 	virtual void Ressuspend(int ABox);
		virtual void ExtinctionCoefficientKTot(int ABox);
      virtual void Mineralize(int ABox);
      //virtual void POMProduction(int ABox);
      virtual double ShearStress(int ABox);
   double DepositedFlux, CriticalVelocity, SinkingVelocity, KarmanConstant, DragCoefficient, ABoxDepth, NContents, DetritusBreakDown;
   double *POMConcentration, *POMFlux, *BoundarySPMConcentration, *BoundaryPOMConcentration, *BottomSedimentOrganicContents,
          *Roughness, *ErosionRate;
   int GridColumns, GridLines,
       WesternBoundaryNumber, SouthernBoundaryNumber,
       EasternBoundaryNumber, NorthernBoundaryNumber;
};

class _export TSPMCrestuma : public TSPMOceanic
{
	public :
        // constructors invoked outside EcoDyn
        TSPMCrestuma(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
            double aSPMConcentration[], double aSPMLoad[], double totalExtinctionCoefficient,
            int nParams, double localLatitude, double localHeight, double localSalt,
            double localTemperature, double turbulenceFactor, double aSPMRessuspension[]);
        TSPMCrestuma(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TSPMCrestuma(TEcoDynClass* APEcoDynClass, char* className);
		~TSPMCrestuma();
      virtual void Go();
      virtual void Integrate();
        // AP, 2007.07.10
        virtual bool SaveVariables();

	protected:
        void BuildSPMCrestuma();
		virtual void ExtinctionCoefficientKTot();

   double KValue;
};


class _export TSPMCrestumaVariableKValue : public TSPMCrestuma
{
	public :
		TSPMCrestumaVariableKValue(TEcoDynClass* APEcoDynClass, char* className);
        ~TSPMCrestumaVariableKValue();
        virtual void freeMemory();
      virtual void Go();
        // AP, 2007.07.10
        virtual bool SaveVariables();

    protected:
        void BuildSPMCrestumaVariableKValue();
    double *DailyKValue;
};

class _export TSPMTimeSeries : public TSPM
{
	public :
  		TSPMTimeSeries(TEcoDynClass* APEcoDynClass, char* className);
		~TSPMTimeSeries();
        virtual void freeMemory();
		virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.10
        virtual bool SaveVariables();
        
	protected :
        void BuildTSPM(char* className);
        double *ForcingTPM, *ForcingPOM, *ForcingDetritalC, *ForcingDetritalN, *ForcingDetritalP, *Food;
        double DetEnergeticContents;
        int AMoment;
};

class _export TRiaFormosaResuspendDeposit : public TSangoResuspendDeposit
{
	public :
        // constructors invoked outside EcoDyn
        /*TRiaFormosaResuspendDeposit(char* className, float timeStep,int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double totalExtinctionCoefficient,
            double aSPMConcentration[], double aPOMConcentration[],
            double aBoundarySPMConcentration[], double aBoundaryPOMConcentration[],
            int nParams, double karmanConstant, double dragCoefficient,
            double criticalVelocity, double sinkingVelocity, double nContents,
            double detritusBreakdown, double bottomSedimentOrganicContents,
            double aErosionRate[], double aRoughness[]);
        TRiaFormosaResuspendDeposit(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename); */

        TRiaFormosaResuspendDeposit(TEcoDynClass* APEcoDynClass, char* className);
        ~TRiaFormosaResuspendDeposit();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        virtual void Go();
        virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Integrate();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.11
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected:
        void PreBuildRiaFormosaResuspendDeposit();
        void BuildRiaFormosaResuspendDeposit();
    	virtual void Deposit(int ABox);
        virtual void Ressuspend(int ABox);
        virtual void ExtinctionCoefficientKTot(int ABox);
        void NitrogenMineralization(int ABoxNumber);
        void PhosphorusMineralization(int ABoxNumber);
        void CarbonMineralization(int ABoxNumber);
        void DissolutionOfSuspendedOrganics(int ABoxNumber);
        void Loads();
        double OxygenLimitation(int ABoxNumber, double K);
        double TemperatureLimitation(int ABoxNumber); 
        double *DetritalC, *DetritalN, *DetritalP,
               *LiveC, *LiveN, *LiveP,
               *DetritalCFlux, *DetritalNFlux, *DetritalPFlux,
               *LiveCFlux, *LiveNFlux, *LivePFlux,
               *BoundaryDetritalC, *BoundaryDetritalN, *BoundaryDetritalP,
               *BoundaryLiveC, *BoundaryLiveN, *BoundaryLiveP,
               *Mineralization_C, *Mineralization_N, *Mineralization_P, *DepositionFlux, *ResuspensionFlux,
               *Dissolution_C, *Dissolution_N, *Dissolution_P;
        double *TPMLoad, *POMLoad, *DetritalCLoad, *DetritalNLoad, *DetritalPLoad,
               *TPMRiverLoad, *POMRiverLoad, *DetritalCRiverLoad, *DetritalNRiverLoad, *DetritalPRiverLoad;
        double CriticalWaterDepth, KminO2, minNs, minPs, minCs, Kt, POMSinkingVelocity;
        double CarbonToOrganics, OrganicFractionThatDissolves, FractionMineralizedToAmmonia;
        typedef struct TPMLoads
        {
            char LoadName[210];
            int LineCoordinate, ColumnCoordinate, LayerCoordinate;
            double *TPMLoad, *POMLoad, *DetritalCLoad, *DetritalNLoad, *DetritalPLoad;
        }  LoadRecord;
        typedef struct RiverLoads
        {
            char RiverName[210];
            int LineCoordinate, ColumnCoordinate, LayerCoordinate;
            double *TPMRiverLoad, *POMRiverLoad, *DetritalCRiverLoad, *DetritalNRiverLoad, *DetritalPRiverLoad;
        }  LoadRiverRecord;
        LoadRiverRecord* ALoadRiverRecord;
        LoadRecord* ALoadRecord;
        int NumberOfLoads, NumberOfRiverLoads, NumberOfDaysForLoads, NumberOfDaysForBoundaryTimeSeries,
            MyNumberOfBoxesInSubDomain, NumberOfMomentsForLoads;
};

class _export TPisciculturaResuspendDeposit : public TRiaFormosaResuspendDeposit
{
	public :
          TPisciculturaResuspendDeposit(TEcoDynClass* APEcoDynClass, char* className);
          ~TPisciculturaResuspendDeposit();
          virtual void freeMemory();
          virtual double GetParameterValue(char* parmName);
          virtual bool SetParameterValue(char* parmName, double value);
          virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
          virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
          virtual void Integrate();
          virtual bool SetVariableValue(char* srcName,
                              double Value,
                               int BoxNumber,
                               char* VariableName);
          virtual bool SaveVariables();
          virtual bool SaveParameters();

	protected:
          void BuildPisciculturaResuspendDeposit();
          virtual void Deposit(int ABox);
          double *Food, *FoodC, *FoodN, *FoodP, *FoodFlux, *FoodCFlux, *FoodNFlux, *FoodPFlux;
          double FoodCContent, FoodNContent, FoodPContent, SinkingVelocityOfFoodPellets;
          int NumberOfDaysForFoodTimeSeries, NumberOfHoursForFoodTimeSeries, NumberOfMomentsForFoodLoads;
          typedef struct TPMFarmLoads
          {
            char LoadName[210];
            int LineCoordinate, ColumnCoordinate, LayerCoordinate;
            double *FoodLoad;
          } FarmLoadRecord;
          FarmLoadRecord* AFarmLoadRecord;
};


class _export TSPM3D : public TRiaFormosaResuspendDeposit
{
     public :
        TSPM3D(TEcoDynClass* APEcoDynClass, char* className);
        virtual void Go();
        virtual void Integrate();
        // AP, 2009.04.08
        virtual bool SaveVariables();
     protected:
        void BuildTSPM3D();
    	virtual void Deposit(int ABox);
        void Loads();
        int UpperLayer;    
};





static TEcoDynClass *PSuspendedMatter;

#endif //SPMOBJT.H



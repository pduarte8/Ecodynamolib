// Macrophyte object header file
//Pedro Duarte

#ifndef MACROBJT_H
#define MACROBJT_H


#include "prodobjt.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TMacrophytes : public TProdutor
{
	public :
        // constructors invoked outside EcoDyn
        TMacrophytes(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TMacrophytes(char* className, float timeStep, char* morphologyFilename);

		TMacrophytes(TEcoDynClass* APEcoDynClass, char* className);
		~TMacrophytes();
        virtual void freeMemory();
		virtual void Go ();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
		virtual void Integrate();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
	protected :
        void BuildMacrophytes();
	  virtual void Production();
	  virtual void Respiration();
	  virtual void Mortality();
          virtual double TemperatureLimitation(double AWaterTemperature, double AReferenceTemperature, double ATemperatureCoefficient);
        double ATimeStep, CriticalWaterDepth, AReferenceTemperature, ATemperatureCoefficient, OxygenDemand, KT, beta, AWaterTemperature;
        int NumberOfBoxesWithBenthicSpecies, NumberOfColumns;
        BenthicSpeciesRecord *MyBenthicSpeciesRecord;
};

class _export TSaltMarshForcing : public TMacrophytes
{
	public :
           TSaltMarshForcing(TEcoDynClass* APEcoDynClass, char* className);
           ~TSaltMarshForcing();
           virtual void freeMemory();
          virtual double GetParameterValue(char* parmName);
          virtual bool SetParameterValue(char* parmName, double value);
           virtual void Go();
           virtual void Integrate();
        // AP, 2007.07.13
        virtual bool SaveParameters();

        protected:
           int NumberOfInterfaceBoxes;
           BenthicSpeciesRecord *SaltMarshInterfaceBoxesRecord;
           double *OrganicRelease, *NitrateConsumption, *AmmoniumRelease;
};

class _export TRuppiaCirrhosa : public TMacrophytes
{
	public :
		TRuppiaCirrhosa(TEcoDynClass* APEcoDynClass, char* className);
      ~TRuppiaCirrhosa();
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
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

	protected :
     double *PhytoDeadBiomass,
            *IkTancada,
            *CanopyHeight,
            *KSelfShading,
            *ConversionToKUnits,
            *KLength,
            *GrossOxygenProductivity,
            *NetOxygenProductivity,
            *GrossOxygenProduction,
            *NetOxygenProduction;
     double IkFraction, IkStAndre, KCanopy, OxygenToCarbon, CarbonToDryWeight, LagoonDepth;
     double UpperBoxForOxygenDistribution;
	  virtual void Production();
	  virtual void Respiration();
	  virtual void Mortality();
     virtual double GetIkStAndre (double IkTancada,
                          double LagoonDepth);
     virtual double GetCanopyExtinctionCoefficient(int ABoxNumber);
     virtual double GetCanopyHeight(int ABoxNumber);
     virtual double GetDailyPmax();
};


class _export TRuppiaCirrhosaMesocosmos : public TRuppiaCirrhosa
{
	public :
		TRuppiaCirrhosaMesocosmos(TEcoDynClass* APEcoDynClass, char* className);
      virtual void Go();
   protected :
   	virtual void Production();
      virtual void Respiration();
      virtual double GetDailyPmax();
};

class _export TEnteromorphaSp : public TRuppiaCirrhosa
{
   public :
      TEnteromorphaSp(TEcoDynClass* APEcoDynClass, char* className);
      ~TEnteromorphaSp();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName, int AnObjectCode);
      virtual void Go();
      virtual void Integrate();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
      virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void NitrogenUptake(int ABoxNumber);
      virtual void PhosphorusUptake(int ABoxNumber);
      double  NutrientLimitation(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      int NumberOfBoxesWithEnteromorphaSp;
      BenthicSpeciesRecord *EnteromorphaSpRecord;
      double *CCellQuota, *NCellQuota, *PCellQuota, *NUptake, *PUptake,
          *CPhyto, *NPhyto, *PPhyto,
          *NCellFlux, *PCellFlux;
      double MaxPCellQuota, MaxNCellQuota, MinPCellQuota, MinNCellQuota,
             OxygenToCarbonProd, OxygenToCarbonResp, VNmax, VPmax, KN, KP, MyBoxDepth;
};

class _export TUlvaSp : public TRuppiaCirrhosa
{
   public :
      TUlvaSp(TEcoDynClass* APEcoDynClass, char* className);
      ~TUlvaSp();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName, int AnObjectCode);
      virtual void Go();
      virtual void Integrate();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
      virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void NitrogenUptake(int ABoxNumber);
      virtual void PhosphorusUptake(int ABoxNumber);
      double  NutrientLimitation(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      int NumberOfBoxesWithUlvaSp;
      BenthicSpeciesRecord *UlvaSpRecord;
      double *CCellQuota, *NCellQuota, *PCellQuota, *NUptake, *PUptake,
          *CPhyto, *NPhyto, *PPhyto,
          *NCellFlux, *PCellFlux;
      double MaxPCellQuota, MaxNCellQuota, MinPCellQuota, MinNCellQuota,
             OxygenToCarbonProd, OxygenToCarbonResp, VNmax, VPmax, KN, KP, MyBoxDepth;

};

class _export TZosteraNoltii : public TUlvaSp  //According to Plus et al (2003). Ecol. Modell. 161: 213-238
{
   public :
      TZosteraNoltii(TEcoDynClass* APEcoDynClass, char* className);
      ~TZosteraNoltii();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName, int AnObjectCode);
      virtual void Go();
      virtual void Integrate();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
      virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void NitrogenUptake(int ABoxNumber);
      virtual void PhosphorusUptake(int ABoxNumber);
      virtual void NitrogenAndPhosphorusTransfer(int ABoxNumber);
      virtual void NitrogenReclamation(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      virtual void Growth(int ABoxNumber);
      virtual void RecruitmentFunction(int ABoxNumber);
      double TemperatureLimitation(double AWaterTemperature, double AYIntercept, double ASlope);
      double  NutrientLimitation(double ASaturationLevel,double ACoefficient);
      double CalculateLeafAreaIndex(int ABoxNumber);
      double CalculateIK(int ADay);
      double SaturationLevelOfLeafNQuota(int ABoxNumber);
      double SaturationLevelOfRootNQuota(int ABoxNumber);
      double TemperatureLimitationOfRecruitment(int ABoxNumber);
      double SelfShadingLimitation(int ABoxNumber);
      double BelowgroundLimitation(int ABoxNumber);
      int NumberOfBoxesWithZosteraNoltii;
      BenthicSpeciesRecord *ZosteraNoltiiRecord;
      double TetaPmax, Pmax0, KLAI, K3, Ikmax, Ikmin, TetaLR, LR0, TetaRR, RR0, ER20,
             PQ, IK, E1, K, E2, MinNCellQuotaRoot, MaxNCellQuotaRoot, Sigma1, VNmaxRoot, KNroot, Sigma2, Tau,
             Srec, RECmax, LMR20, TetaLM, LMRv, K4, RMR20, RECruitmax, TetaRec, SB0, Krec1, Krec2, BottomLight, CarbonToDryWeight;
      double *PhytoRootBiomass, *CCellQuotaRoot, *NCellQuotaRoot, *PCellQuotaRoot, *NUptakeRoot, *PUptakeRoot,
             *CPhytoRoot, *NPhytoRoot, *PPhytoRoot,
             *PhytoFluxRoot, *NCellFluxRoot, *PCellFluxRoot, *LAI, *LeafResp, *RootResp,
             *RootNSaturation, *LeafNSaturation, *LeafNReclamation, *RootNReclamation,
             *TotalGrowth, *RootGrowth, *LeafGrowth, *Recruitment, *ShootDensity, *ShootDensityFlux,
             *TPMProduction, *POMProduction;
};

class _export TZosteraMarina : public TMacrophytes
{
	public :
		TZosteraMarina(TEcoDynClass* APEcoDynClass, char* className);
      ~TZosteraMarina();
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
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
   	virtual void Production(int ABoxNumber);
      virtual void Respiration(int ABoxNumber);
      virtual void Mortality(int ABoxNumber);
      double MyDepth, CarbonToOxygenProd;
      double *OxygenNetProduction;
      BenthicSpeciesRecord *ZosteraMarinaRecord;
      int NumberOfBoxesWithZosteraMarina;
};


class _export TGelidiumSesquipedale : public TMacrophytes
{
	public :
		TGelidiumSesquipedale(TEcoDynClass* APEcoDynClass, char* className);
      ~TGelidiumSesquipedale();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
      virtual void Update(char* srcName, double Value,
                           double HarvestableSize,
									int BoxNumber,
									int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,
                               int BoxNumber,
                               char* ParameterName,
                               int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,
                               int BoxNumber,
                               double HarvestSize,
                               int AnObjectCode);
      virtual void Integrate();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
   	virtual void Production();
      virtual void Respiration();
      virtual void Exudation();
      virtual void FrondBreakage();
      virtual void Mortality();
      virtual void DemographicUpwind();
      double WeightVsLenght(double Size);
      double LengthVsWeight(double Weight);
      double ArrheniusTemperatureLimitation(int ABoxNumber, double AWaterTemperature);
      double StochasticFrondBreakage(double size);
      double SizeOfBrokenPortion (double size);

      int NumberOfClasses;
      double Gelidium_min_prof, Gelidium_max_prof,
             ClassIncrement, CarbonContent, ExudationRate,
             TotalBiomass,BiomassCarryingCapacity;
      double /**Generic,
      		 *GenericFlux,*/
             *PhytoClassBiomass,
             *PhytoClassDensity,
             //*PhytoInitialSize,
             *PhytoIndividualSize,
             *PhytoIndividualWeight,
             *PhytoClassBiomassFlux,
             *PhytoClassDensityFlux,
             *PhytoIndividualFlux,
             //*org_seston_mass,
             //*total_seston_mass,
			    *GrowthRate,
			    //*Mortality,
             *TransitionNumber,
			    *TransitionBiomass,
			    *TransitionWeight,
             *ParticulateMass,
             *ParticulateFlux,
             *DissolvedMass,
             *DissolvedFlux,
             *RespiratedFlux,
             *DTemperature,
             *ETemperature,
             *AssimptoticBiomass;
};


class _export TLaminariaJaponica : public TMacrophytes
{
	public :
		TLaminariaJaponica(TEcoDynClass* APEcoDynClass, char* className);
      ~TLaminariaJaponica();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
      virtual void Update(char* srcName, double Value,
                           double HarvestableSize,
									int BoxNumber,
									int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,
                               int BoxNumber,
                               char* ParameterName,
                               int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,
                               int BoxNumber,
                               double HarvestSize,
                               int AnObjectCode);
      virtual void Integrate();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
   	virtual void Production();
      virtual void Respiration();
      virtual void Exudation();
      virtual void FrondBreakage();
      virtual void Mortality();
      double TemperatureLimitation(int ABoxNumber);
      double NutrientLimitation(int ABoxNumber);
      double WeightVsLenght(double Size);
      double LengthVsWeight(double Weight);
      double ArrheniusTemperatureLimitation(int ABoxNumber, double AWaterTemperature);
      double StochasticFrondBreakage(double size);
      double SizeOfBrokenPortion (double size);
      double GetCanopyExtinctionCoefficient(int ABoxNumber);
      double GetCanopyHeight(int ABoxNumber);
      double CarbonContent, ExudationRate,
             TotalBiomass,BiomassCarryingCapacity, OxygenToCarbon, CarbonToDryWeight;

      double *Ik, *CultureBottom, *CultureTop,
      		 *GrossOxygenProductivity,
             *NetOxygenProductivity,
             *GrossOxygenProduction,
             *NetOxygenProduction;

      int GridColumns, GridLines;
      int NumberOfBoxesWithLaminariaJaponica;
      BenthicSpeciesRecord *LaminariaJaponicaRecord;
};

class _export TLaminariaJaponicaExponentialGrowth : public TMacrophytes
{
	public :
        // constructors invoked outside EcoDyn
        TLaminariaJaponicaExponentialGrowth(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aPhytoBiomass[], double aPhytoIndividualWeight[],
            int nParams, double intrinsicRateOfIncrease, double startDay,
            double harvestDay, double carbonContents, double nitrogenContents,
            double betat, double ts, double te, double kgrowth);
        TLaminariaJaponicaExponentialGrowth(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TLaminariaJaponicaExponentialGrowth(TEcoDynClass* APEcoDynClass, char* className);
      ~TLaminariaJaponicaExponentialGrowth();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();

      virtual void Inquiry(char* srcName, double &Value,
                               int BoxNumber,
                               char* ParameterName,
                               int AnObjectCode);

     virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);

      virtual void Integrate();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();
        virtual bool SaveVariables();

   protected :
      void PreBuildLJEGrowth();
      void BuildLJEGrowth();
   	  virtual void Growth(int ABoxNumber);
      virtual void NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber);
      double TemperatureLimitation(int ABoxNumber);
      int GridColumns, GridLines;
      double IntrinsicRateOfIncrease, StartDay, HarvestDay,
             CarbonContents, NitrogenContents,
             Betat, Ts, Te, ATimeStep, Kgrowth;
      double *PhytoIndividualWeight, *PhytoIndividualFlux;
      int NumberOfBoxesWithLaminariaJaponica;
      BenthicSpeciesRecord *LaminariaJaponicaRecord;
};


static TEcoDynClass *PMacrophytes;


#endif //MACROBJT_H

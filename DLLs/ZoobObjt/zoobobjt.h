// Zoobenthos object header file
// REV A - 96.06.14

#ifndef ZOOBOBJT_H
#define ZOOBOBJT_H

/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifndef __BORLANDC__
	#include <iostream>
	
	using namespace std;
	#define _export 
	
#endif  // __BORLANDC__

#include "EcoDynClass.h"
#include "iodll.h"
#include "Queue.h"

// DLL header file

class _export TZoobenthos : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
        TZoobenthos(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TZoobenthos(char* className, float timeStep, char* morphologyFilename);

		TZoobenthos(TEcoDynClass* APEcoDynClass, char* className);
		~TZoobenthos();
        virtual void freeMemory();
		virtual void Go ();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Integrate();
		virtual double integral_normal(double z1,
												 double z2);
		virtual double class_st_dev(double down_limit,
											 double upper_limit,
											 double average);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

	protected :
        void BuildZoobenthos(char* className);
		double
			*ZooBiomass,
			*ZooLoad,
			*ZooProduction,
			*ZooBiomassFlux,            // Flux derivative
			*ZooIndividualFlux,
			*ZooGrossProduction,
			*ZooNetProduction,
			*ZooDensity,
			*ZooDensityFlux,
			*ZooClassBiomass,
			*ZooClassDensity,
			*ZooIndividualWeight,
			*ZooInitialWeight,
			*ZooClassBiomassFlux,
			*ZooClassDensityFlux,
			*ClassMaxLimits,
			*ClassMinLimits,
			*transition_number,
			*transition_biomass,
			*transition_weight,
			*HabitatArea,
			*assimilation_efficiency,
			*POMa,
			*SelectionEfficiency,
			*GrowthRate,
			*ClearanceRate,
			*Mortality,
			*ZooDINRelease,
			*ZooPhytoUptake,
			*ZooPOCUptake;


		int NumberOfClasses;

		double RiverZoo,
				 OceanZoo;

		// Conversion factors
		double DryWeightToFreshWeight,
				 ChlorophyllToCarbon,
				 CarbonToDryWeight,
				 ExcretionLoss,
				 DOCLoss,
				 DeathLoss,
				 TotalFWToMeatDW,/*conversão para transformar pesos totais de animais com
									  concha em pesos secos das partes moles -
									  deve ser inicializada em cada descendente*/
				 gDW_Calg,/*conversão de peso seco em energia-
							deve ser inicializada em cada descendente*/
				 gcarbon_joules,//conversão de gramas de carbono joules}
				 faeces_nitrogen,
				 Carbon_g_cal,
				 Oxygen_ml_cal,
				 dw_to_fw,
				 chl_carbon,
				 carbon_dw,
				 nitrogen_dw,
				 cal_joules;

		double ZooProd,
				 Ration,
				 Rmax,
				 KGraze,
				 Metabolism,
				 PhytoLowerLimit,
				 total_excretion, gamete_emission,
				 deposit_elimination,
				 faeces_production,
				 ammonia_excretion,
				 natural_mortality,
				 organic_seston_correction,
				 meat_conversion,
				 total_respiration,
				 standard_respiration,
				 standard_respiration0,
				 assimilation_slope,
				 respiration_slope,
				 SigmaNumber,
				 ClassIncrement;
             
      double ATimeStep, CriticalWaterDepth;
      int NumberOfBoxesWithBenthicSpecies;
      BenthicSpeciesRecord *MyBenthicSpeciesRecord;
		virtual void Feed(){};
		virtual void Ingestion(){};
		virtual void Assimilate(){};
		virtual void Excrete(){};
		virtual void Respire(){};
		virtual void Reproduce(){};
		virtual void Die(){};
		virtual void ClassTransitionNormal();
      virtual void DemographicModel();
		virtual void DemographicModelCentralDifferences();
		virtual void DemographicUpwind();
		virtual void DemographicUpwindSemiImplicit();
		virtual double InverseNormalSt(double);
};

class _export TSuspensionFeeders : public TZoobenthos
{
	public :
        // constructors invoked outside EcoDyn
        TSuspensionFeeders(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TSuspensionFeeders(char* className, float timeStep, char* morphologyFilename);

		TSuspensionFeeders(TEcoDynClass* APEcoDynClass, char* className);
        ~TSuspensionFeeders();

	protected :
		double
			*SestonFiltration,
			*seston_ingestion,
			*pseudofaeces_production,
			*org_seston_mass,
			*total_seston_mass;

		double pseudo_faeces_rate,
				 pseudo_faeces_maxo,
				 pseudo_faeces_maxi,
				 max_filtration_rate,
				 Standard_filtration,
				 saturation_level,
				 filtration_rate,
				 Filtration_coef,
				 seston_mass,//Organic and inorganic except phytoplankton},
				 POC_mass;
};


class _export TCrassostreaGigas : public TSuspensionFeeders
{
	public :
		TCrassostreaGigas(TEcoDynClass* APEcoDynClass, char* className);
		~TCrassostreaGigas();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		virtual void Go ();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 double ExtraValue,
										 int AnObjectCode);
		virtual void Update(char* srcName, double Value,
									double ExtraValue,
									int BoxNumber,
									int ClassNumber,
									int AnObjectCode);
		virtual void Update(char* srcName, double Value,
									double ExtraValue,
									int BoxNumber,
									int AnObjectCode);

		virtual void Integrate();
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
		double shell_deposition,
				 assimilation_efficiency0,

						//Parameters
				 xkf, /*Coeficiente da fórmula de cálculo da taxa máxima
						 de filtração - Raillard, 1991*/

				 c1o, /*Coeficientes da fórmula de cálculo das pseudofezes
							- Raillard, 1991*/
				 c1i,
				 xkpo,
				 xkpi,
				 c2o,
				 c2i,
				 xkp2o,
				 xkp2i,

				 Oyster_g_cal;//Conversion factor

		virtual void Feed();
		virtual void Ingestion();
		virtual void Pseudofaeces();
		virtual void Assimilate();
		virtual void Excrete();
		virtual void Respire();
		virtual void Reproduce();
		virtual void ShellDeposition();
		virtual void OrganicDeposition();
		virtual void Die();
};

class _export TCrassostreaGigasEmpirical : public TCrassostreaGigas
{
	public :
		TCrassostreaGigasEmpirical(TEcoDynClass* APEcoDynClass, char* className);
		~TCrassostreaGigasEmpirical();
        virtual void freeMemory();
		virtual void Go ();
	protected :

		virtual void Feed();
		virtual void Ingestion();
		virtual void Pseudofaeces();
		virtual void Assimilate();
		virtual void Respire();

		double POCtoPOM;

};


class _export TCrassostreaGigas1 : public TCrassostreaGigas
{
	public :
		TCrassostreaGigas1(TEcoDynClass* APEcoDynClass, char* className);
		~TCrassostreaGigas1();
		virtual void Integrate();
};



class _export TCrassostreaGigas2 : public TCrassostreaGigas
{
	public :
		TCrassostreaGigas2(TEcoDynClass* APEcoDynClass, char* className);
		~TCrassostreaGigas2();
		virtual void Go ();
		virtual void Integrate();
};

class _export TCrassostreaGigas3 : public TCrassostreaGigas2
{
	public :
		TCrassostreaGigas3(TEcoDynClass* APEcoDynClass, char* className);
		~TCrassostreaGigas3();
        virtual void freeMemory();
		virtual void Integrate();
		virtual void Die();
};

class _export TCrassostreaGigas4 : public TCrassostreaGigas2
{
	public :
		TCrassostreaGigas4(TEcoDynClass* APEcoDynClass, char* className);
		~TCrassostreaGigas4();
		virtual void Pseudofaeces();
};

class _export TRuditapesDecussatus : public TSuspensionFeeders
{
	public :
		TRuditapesDecussatus(TEcoDynClass* APEcoDynClass, char* className);
		~TRuditapesDecussatus();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Integrate();
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,double ExtraValue,int AnObjectCode);
      virtual void Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int ClassNumber,int AnObjectCode);
      virtual void Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int AnObjectCode);
        // AP, 2006.06.09
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected :
      virtual void Feed();
      virtual void Pseudofaeces();
      virtual void Ingestion();
      virtual void Assimilate();
      virtual void Excrete();
      virtual void Respire();
      virtual void Reproduce();
      virtual void ShellDeposition();
      virtual void OrganicDeposition();
      virtual void Die();
      double TemperatureLimitation(int ABoxNumber);
      double OxygenLimitation(int ABoxNumber);
      double ClearanceRateOfAStandardAnimal(int ABoxNumber);
      double RespirationOfAStandardAnimal(int ABoxNumber);
      double POCtoSESTON, DryWeightOfAStandardAnimal,
             ThresholdConsumptionForPseudofaeces, xkp,
             RespirationCoefficient, MeatEnergeticContents,
             StandardRespiration, StandarRespirationUnderHypoxia,IntrinsicRateOfMortality,
             AssimilationParameter, AssimilationMaximum, OrganicContents, MinimumAssimilationEfficiency;
      BenthicSpeciesRecord *RuditapesDecussatusRecord;
      int NumberOfBoxesWithRDecussatus;
};


class _export TRDecussatusFoodLimited : public TRuditapesDecussatus
{
	public :
		TRDecussatusFoodLimited(TEcoDynClass* APEcoDynClass, char* className);
      ~TRDecussatusFoodLimited();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.13
        virtual bool SaveParameters();

   protected :
   	virtual void Feed();
      //virtual void Die();
      double *BoxClearanceRate,
             *BoxClearanceTime;
      double CultivatedArea;       
};



class _export TRDecussatusIndivGrowth : public TRuditapesDecussatus
{
	public :
		TRDecussatusIndivGrowth(TEcoDynClass* APEcoDynClass, char* className);
      virtual void Go();
      virtual void Integrate();
};

class _export TRuditapesDecussatusPhyto : public TRuditapesDecussatus
{
	public :
		TRuditapesDecussatusPhyto(TEcoDynClass* APEcoDynClass, char* className);
      virtual void Feed();
};

class _export TRDecussatusIndivGrowthPhyto : public TRuditapesDecussatusPhyto
{
	public :
		TRDecussatusIndivGrowthPhyto(TEcoDynClass* APEcoDynClass, char* className);
      virtual void Go();
      virtual void Integrate();
};

class _export TRDecussatusIndivGrowthPhytoAndTPM : public TRDecussatusIndivGrowthPhyto
{
        public :
		TRDecussatusIndivGrowthPhytoAndTPM(TEcoDynClass* APEcoDynClass, char* className);
                virtual void Go();
        protected :
                virtual void Feed(int ABoxNumber);
                virtual void Pseudofaeces(int ABoxNumber);
                virtual void Ingestion(int ABoxNumber);
                virtual void Assimilate(int ABoxNumber);
                virtual void Excrete(int ABoxNumber, int AClassNumber, double ARespiration);
                virtual void Respire(int ABoxNumber);
                virtual void Reproduce(int ABoxNumber);
                virtual void ShellDeposition(int ABoxNumber);
                virtual void OrganicDeposition(int ABoxNumber);
                virtual void Die(int ABoxNumber);
                double SestonEnergeticContent;
                double DetritusEnergeticContents; //J g-1
                double PhytoEnergeticContents; //J g-1
         double TINNY;       
};


class _export TBivalvesForSango : public TSuspensionFeeders
{
	public :
        // constructors invoked outside EcoDyn
        TBivalvesForSango(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TBivalvesForSango(char* className, float timeStep, char* morphologyFilename);

   	    TBivalvesForSango(TEcoDynClass* APEcoDynClass, char* className);
      ~TBivalvesForSango();
      virtual void Integrate();
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 double ExtraValue,
										 int AnObjectCode);
      virtual void Update(char* srcName, double ZooDensityInput,  // ind / m2 / day
									int BoxNumber,
                           int ClassNumber,
                           int AnObjectCode);
		virtual void Update(char* srcName, double Value,
									double ExtraValue,
									int BoxNumber,
									int AnObjectCode);
   protected :
   	virtual void Feed(int ABox, int AClass);
		virtual void Pseudofaeces(int ABox, int AClass);
		virtual void Assimilate(int ABox, int AClass);
		virtual void Excrete(int ABox, int AClass);
		virtual void Respire(int ABox, int AClass);
		virtual void Reproduce(int ABox, int AClass);
		virtual void ShellDeposition(int ABox, int AClass);
		virtual void SoftTissueGrowth(int ABox, int AClass);
		virtual void Die(int ABox, int AClass);
      virtual double TemperatureEffectsOnFeedingRate(int ABoxNumber);
      virtual double TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber);
      virtual double AllometricCorrection (double AWeight, double AnAllometricCoefficient);
      virtual double OxygenToNitrogenRatio(int ABoxNumber, int AClass);
      virtual double ShellLenghtCalculation(double AShellDryWeight);
      double *ZooShellMass,
             *FRTPM, *FRPHY,*FRPIM,*FRPOM,*FRDET,
             *RRTPM,*RRPHY,*RRDET,
             *IRPHY,*IRTPM,*IRDET,*IRPIM, *RRPIM,
             *OIR,*OCI, *OCPF,
             *NAEIO, *NEA, *NEB,
             *TOTALFRTPM, *TOTALFRPOM, *TOTALFRPHY,
             *OrganicPseudoFaeces, *InorganicPseudofaeces,
             *SoftTissueEnergeticTotal, *ShellEnergeticTotal,
             *ZooIndividualFluxForTheShell, *ShellLength;
      double nonliving_seston_filtration, phyto_mass, ChlorophyllToBiomass,
             DETORG, PHYORG, TPM, POM, PIM, OCS, PHYCONTENTSPOM;
      double MeatEnergeticContents, ShellEnergeticContents,
             AllometricCoefficientForFeeding, AllometricCoefficientForRespiration,
             DryWeightOfAStandardAnimal, MaintenanceRespiration, TotalMaintenanceRespiration,
             ActivityRespirationCoefficient, TotalRespiration, IntrinsicRateOfMortality,
             ProportionOfEnergyAllocatedToShell, ProportionOfEnergyAllocatedToSoftTissue,
             TotalExcretoryLosses, AmmoniaToJoules, SoftTissueEnergyToTotalEnergy;

      int Lines, Columns;

};


class _export TChlamysFarreri : public TBivalvesForSango
{
	public :
        // constructors invoked outside EcoDyn
        TChlamysFarreri(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aZooShellMass[],
            double aZooIndividualWeight[], double aZooClassDensity[],
            int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
            double allometricCoefficientForRespiration, double meatEnergeticContents,
            double shellEnergeticContents, double intrinsicRateOfMortality,
            double summerIntrinsicRateOfMortality, double maintenanceRespiration,
            double activityRespirationCoefficient, double proportionOfEnergyAllocatedToShell,
            double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
            double softTissueEnergyToTotalEnergy, double numberOfClasses,
            double totalFWToMeatDW);
        TChlamysFarreri(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TChlamysFarreri(TEcoDynClass* APEcoDynClass, char* className);
      ~TChlamysFarreri();
      virtual void freeMemory();
      virtual void Update(char* srcName, double Value,
									int BoxNumber,
                           char* ParameterName,
                           int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Go();
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
      // AP, 2007.06.15
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.13
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected :
        void PreBuildChlamysFarreri();
        void BuildChlamysFarreri();
        void PosBuildChlamysFarreri();
        void ReadParameters();
        void ReadVariables();
        void BuildArrays();
   	    virtual void Feed(int ABox, int AClass);
		virtual void Reproduce(int ABox, int AClass);
      virtual double TemperatureEffectsOnFeedingRate(int ABoxNumber);
      virtual double TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber);
      virtual double OxygenToNitrogenRatio(int ABoxNumber, int AClass);
      virtual double ShellLenghtCalculation(double AShellDryWeight);
      virtual void Die(int ABox, int AClass);
   double SummerIntrinsicRateOfMortality;
   int NumberOfBoxesWithChlamysFarreri;
   BenthicSpeciesRecord *ChlamysFarreriRecord;
};


class _export TChlamysFarreriIndivGrowth : public TChlamysFarreri
{
	public :
		TChlamysFarreriIndivGrowth(TEcoDynClass* APEcoDynClass, char* className);
      virtual void Go();
      virtual void Integrate();
};

class _export TChlamysFarreriV8 : public TChlamysFarreri
{
	public :
        // constructors invoked outside EcoDyn
        TChlamysFarreriV8(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aZooShellMass[],
            double aZooIndividualWeight[], double aZooClassDensity[],
            int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
            double allometricCoefficientForRespiration, double meatEnergeticContents,
            double shellEnergeticContents, double intrinsicRateOfMortality,
            double summerIntrinsicRateOfMortality, double maintenanceRespiration,
            double activityRespirationCoefficient, double proportionOfEnergyAllocatedToShell,
            double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
            double softTissueEnergyToTotalEnergy, double numberOfClasses,
            double totalFWToMeatDW);
        TChlamysFarreriV8(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TChlamysFarreriV8(TEcoDynClass* APEcoDynClass, char* className);
      ~TChlamysFarreriV8();
      virtual void freeMemory();
      virtual void Go();
      virtual void Reinitialize();  //AP, 2006.06.30
   protected:
      void BuildChlamysFarreriV8();
      virtual void Feed(int ABox, int AClass);
      bool boxWithScallops(int boxNumber);
};


class _export  TCrassostreaGigas5 : public TBivalvesForSango
{
	public :
		 TCrassostreaGigas5(TEcoDynClass* APEcoDynClass, char* className);
       ~TCrassostreaGigas5();
       virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected:
       virtual void Go();
       virtual void Update(char* srcName, double Value,
									int BoxNumber,
                           char* ParameterName,
                           int AnObjectCode);
       virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
   protected :
   	virtual void Feed(int ABox, int AClass);
		virtual void Reproduce(int ABox, int AClass);
      virtual double TemperatureEffectsOnFeedingRate(int ABoxNumber);
      virtual double TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber);
      virtual double OxygenToNitrogenRatio(int ABoxNumber);
      virtual double ShellLenghtCalculation(double AShellDryWeight);

   double *NOSE, *IRORG;
};


class _export  TCrassostreaGigas6 : public TBivalvesForSango
{
	public :
        // constructors invoked outside EcoDyn
        TCrassostreaGigas6(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aZooShellMass[],
            double aZooIndividualWeight[], double aZooClassDensity[],
            int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
            double allometricCoefficientForRespiration, double meatEnergeticContents,
            double shellEnergeticContents, double intrinsicRateOfMortality,
            double respirationSlope, double respirationAtZero,
            double proportionOfEnergyAllocatedToShell,
            double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
            double numberOfClasses, double totalFWToMeatDW,
            double standardClearanceRate, double saturationTPM, double c1o,
            double c1m, double c2, double pFmaxo, double pFmaxm, double Xkpo,
            double Xkpm, double Xkp2o, double Xkp2m, double oxygenToJoules,
            double softTissueEnergyToTotalEnergy);
        TCrassostreaGigas6(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		 TCrassostreaGigas6(TEcoDynClass* APEcoDynClass, char* className);
       ~TCrassostreaGigas6();
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
       virtual void Reinitialize();  //AP, 2006.07.27
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected :
        void PreBuildCrassostreaGigas6();
        void BuildCrassostreaGigas6();
        void PosBuildCrassostreaGigas6();
        void ReadParameters();
        void ReadVariables();
        void BuildArrays();
        bool boxWithOysters(int boxNumber);     // AP, 2006.07.27

   	virtual void Feed(int ABox, int AClass);
      virtual void Respire(int ABox, int AClass);
		virtual void Reproduce(int ABox, int AClass);
      virtual double TemperatureEffectsOnFeedingRate(int ABoxNumber);
      virtual double TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber);
      virtual double OxygenToNitrogenRatio(int ABoxNumber, int AClass);
      virtual double ShellLenghtCalculation(double AShellDryWeight);

   double *NOSE, *IRORG;
   double RespirationSlope, RespirationAtZero, StandardClearanceRate, SaturationTPM,
          C1o, C1m, C2, PFmaxo, PFmaxm, xkpo, xkpm, xkp2o, xkp2m, OxygenToJoules;
   int NumberOfBoxesWithCrassostreaGigas;
   BenthicSpeciesRecord *CrassostreaGigasRecord;
};

class _export  TCrassostreaGigas7 : public TCrassostreaGigas6
{
	public :
        // constructors invoked outside EcoDyn
        TCrassostreaGigas7(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aZooShellMass[],
            double aZooIndividualWeight[], double aZooClassDensity[],
            int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
            double allometricCoefficientForRespiration, double meatEnergeticContents,
            double shellEnergeticContents, double intrinsicRateOfMortality,
            double respirationSlope, double respirationAtZero,
            double proportionOfEnergyAllocatedToShell,
            double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
            double numberOfClasses, double totalFWToMeatDW,
            double standardClearanceRate, double saturationTPM, double c1o,
            double c1m, double c2, double pFmaxo, double pFmaxm, double Xkpo,
            double Xkpm, double Xkp2o, double Xkp2m, double oxygenToJoules,
            double softTissueEnergyToTotalEnergy, double Fileseuil1,
            double pFmaxCHL, double XkpCHL);
        TCrassostreaGigas7(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		 TCrassostreaGigas7(TEcoDynClass* APEcoDynClass, char* className);
       ~TCrassostreaGigas7();
       virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);

        // AP, 2007.07.13
        virtual bool SaveParameters();

   protected :
      void BuildCrassostreaGigas7();
   	  virtual void Feed(int ABox, int AClass);
      virtual void Respire(int ABox, int AClass);
      virtual double TPMEffectsOnFeedingRate(double ATPM);
      virtual double TemperatureEffectsOnFeedingRate(int ABoxNumber);
      virtual double Retention(double ATPM);
      virtual double TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber);
      virtual double OxygenToNitrogenRatio(int ABoxNumber, int AClass);

   double filseuil1, PFmaxCHL, xkpCHL;
};



class _export TChlamysFarreriIndividual : public TChlamysFarreriV8
{
	public :
        // constructors invoked outside EcoDyn still missing
      TChlamysFarreriIndividual(TEcoDynClass* APEcoDynClass, char* className);
      ~TChlamysFarreriIndividual();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Integrate();

      virtual void Inquiry(char* srcName, double &Value,
							int BivalveNumber, int BoxNumber,
                            char* ParameterName,
                            int AnObjectCode);
      VNA* Inquiry(char* srcName,int &Individuals, int &DimVariables);
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.13
        virtual bool SaveParameters();

   protected:
      void BuildChlamysFarreriIndividual();
      void PreBuildChlamysFarreriIndividual();
      void PostBuildChlamysFarreriIndividual();
      void ReadParameters();
      bool ReadParsRecord(int X, int i, int index);
      void UpdateIndividualVariables(int ABivalve, int ABox);
      void UpdateIndividualParameters(int ABivalve);
      virtual void Feed(int ABox, int AClass);
      typedef struct  Individual{
        double IRPHY, IRDET, IRPIM,IRTPM, FRTPM,
               FRPIM, RRTPM, RRPIM,OIR, OCI, OCPF,
               NAEIO, NEA, NEB, IR,
               ZooIndividualWeight, ZooShellMass, ShellLength,
               ZooIndividualFlux, ZooIndividualFluxForTheShell, ZooIndividualVolume, ZooIndividualReserves,
               SoftTissueEnergeticTotal, ShellEnergeticTotal, ZooCR,
               ClearanceRate, SomC, SomN, StorC, OSW, OSWFrac, GPT, GV, AE, ARC, ARN,
               NetProdC, NetProdN, RespC, RespR, ExcR, Age, DSW, ConditionIndex,SomFrac,StorFrac, PA;
        ParsRecord IndividualParameters[100];
      } IndividualRecord;
      ParsRecord *MyParsRecord;
      TReadWrite *MyPReadWrite;
      Individual *MyIndividualRecord;
      int NumberOfIndividuals;
      int NumberOfParameterRecords, IndividualBasedVariables;

};

class _export TMytilusGalloprovincialisIndividual : public TChlamysFarreriIndividual
{
	public :
        // constructors invoked outside EcoDyn still missing
      TMytilusGalloprovincialisIndividual(TEcoDynClass* APEcoDynClass, char* className);
      ~TMytilusGalloprovincialisIndividual();
      virtual void freeMemory();

      //virtual double GetParameterValue(char* parmName);
      //virtual bool SetParameterValue(char* parmName, double value);
      virtual void Go();
      virtual void Integrate();

      virtual void Inquiry(char* srcName, double &Value,
							int BivalveNumber, int BoxNumber,
                            char* ParameterName,
                            int AnObjectCode);
      //VNA* Inquiry(char* srcName,int &Individuals, int &DimVariables);
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      /*virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);*/
      //virtual bool SaveParameters();

   protected:
      void BuildMytilusGalloprovincialisIndividual();
      void PreBuildMytilusGalloprovincialisIndividual();
      void PostBuildMytilusGalloprovincialisIndividual();
      void ReadParameters();
      void ReadVariables();
      //void UpdateIndividualVariables(int ABivalve, int ABox);
      //void UpdateIndividualParameters(int ABivalve);
      virtual void Feed(int ABox, int ABivalve);
      virtual void Absorption(int ABox, int ABivalve);
      virtual void Assimilation(int ABox, int ABivalve);
      virtual void Allocation(int ABox, int ABivalve);
      virtual void Respire(int ABox, int ABivalve);
      virtual void Excrete(int ABox, int ABivalve);
      virtual void Reproduce(int ABox, int ABivalve);
      virtual void Aging(int ABox, int ABivalve);
      double GutPassageTime(int ABox, int ABivalve);
      double GutVolume(int ABox, int ABivalve);
      double AllometricCorrection(double AMeasure, double AStandardMeasure, double AnAllometricCoefficient);
      virtual double ShellLength(double AShellWeight);
      double *IRFlux, *Egestion;
      int Option_Ingestion_Rate, Option_Clearance_Rate, Option_Respiration;
      double SomC, SomN, StorC, OSW, Age, ReferenceTemperature, AsymptoticLength;
      int NumberOfBoxesWithMytilusGalloprovincialis;
      BenthicSpeciesRecord *MytilusGalloprovincialisRecord, index;
};


class _export TMytilusGalloprovincialisIndividualDEB : public TMytilusGalloprovincialisIndividual
{
	public :
        // constructors invoked outside EcoDyn still missing
      TMytilusGalloprovincialisIndividualDEB(TEcoDynClass* APEcoDynClass, char* className);
      ~TMytilusGalloprovincialisIndividualDEB();
      virtual void freeMemory();
      virtual void Go();

      virtual void Inquiry(char* srcName, double &Value, int BivalveNumber, int BoxNumber,char* ParameterName,int AnObjectCode);
      virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);

   protected:
      void BuildMytilusGalloprovincialisIndividual();
      void PreBuildMytilusGalloprovincialisIndividual();
      void PostBuildMytilusGalloprovincialisIndividual();
      void ReadParameters();
      void ReadVariables();
      virtual void Feed(int ABox, int ABivalve);
      virtual void Absorption(int ABox, int ABivalve);
      virtual void Assimilation(int ABox, int ABivalve);
      virtual void Allocation(int ABox, int ABivalve);
      virtual void Respire(int ABox, int ABivalve);
      virtual void Excrete(int ABox, int ABivalve);
      virtual void Reproduce(int ABox, int ABivalve);
      virtual double ShellLength(int ABivalve, int ABox);
      virtual double TemperatureLimitation(double ATA, double AT1, double AWaterTemperature);
      double *ZooBodyVolume, *ZooReserves, *FoodEnergeticContents;
      double DETEnergeticContents/*J mg-1*/,PHYEnergeticContents/*J mg-1*/, TA/*ºK*/, T1/*ºK*/, AWaterTemperature/*ºC*/;
};

static TEcoDynClass *PZoobenthos;
static TEcoDynClass *PZoobenthos2;


#endif //ZOOBOBJT_H

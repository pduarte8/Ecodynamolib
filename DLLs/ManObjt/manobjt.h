// Man object header file
// REV A - 96.06.25
/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef MANOBJT_H
#define MANOBJT_H

#include "EcoDynClass.h"
#include "Queue.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TMan : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
        TMan(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TMan(char* className, float timeStep, char* morphologyFilename);

		TMan(TEcoDynClass* APEcoDynClass, char* className);
	protected :
        void BuildMan(char* className);

        int NumberOfBoxesWithBenthicSpecies;
        BenthicSpeciesRecord* MyBenthicSpeciesRecord;
};

class _export TIrishMan : public TMan
{
	public :
        // constructors invoked outside EcoDyn
        TIrishMan(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aSeedInput[], double aSeedWeight[],
            double aCrop[], double aZoobLowerLimit[], double aZoobUpperLimit[],
            double salePricePerkgCrassostrea, double aPricePerkgCrassostreaHarvest[],
            double pricePerkgCrassostreaSeed,
            int nParams, double carbonToDryWeight, double harvestableWeight,
            int firstSeedingDay, int lastSeedingDay,
            int firstHarvestDay, int lastHarvestDay);
        TIrishMan(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TIrishMan(TEcoDynClass* APEcoDynClass, char* className);
		~TIrishMan();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		virtual void Go();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Integrate();
		/*
		virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);
		*/
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.16
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
        void PreBuildIrishMan();
        void BuildIrishMan();
		double
			*HarvestBiomass,
			*HarvestFlux,
			*SeedInput,
			*SeedWeight,
			*Crop,
			*ZoobLowerLimit,
			*ZoobUpperLimit,
			*HarvestEffort,
			*HarvestEfficiency,
			*PricePerkgCrassostreaHarvest,
			*HarvestableBiomass;

		double PricePerkgCrassostreaSeed, // gastos referentes a sementeira de cada kg
				 SalePricePerkgCrassostrea,
				 DryToFreshWeight,
				 ChlorophyllToCarbon,
				 CarbonToDryWeight,
				 HarvestableWeight;
		int FirstSeedingDay,
			 LastSeedingDay,
			 FirstHarvestDay,
			 LastHarvestDay;

		 char PreySpecies[30];

		virtual void ZooHarvest();
		virtual void Seed();
};

/*
class _export TBacteria : public TMan
{
	public :
		TBacteria(TEcoDynClass* APEcoDynClass, char* className);
		~TBacteria();
		virtual void Go();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Integrate();

		virtual void Update(char* srcName, double Value,
									int BoxNumber,
									char* ParameterName,
									int AnObjectCode);

	protected :
		double
			*BacterialCarbonBiomass,
         *BacterialGrossProduction,
         *BacterialNetProduction,
         *BacteriaFlux;


		double
             ExcretionRate,
             NitrogenHalfSaturation,
             AmmoniunDONUptakeRatio,
             AmmoniumUptake,
             DONUptake,
             DOCUptake,
             BacteriaMaximumGrowthRate,
             GrossGrowthEfficiencyForNitrogen,
             GrossGrowthEfficiencyForCarbon,
             OceanBacteria,
             RedfieldCFactor,
				 RedfieldNFactor;

		virtual void Uptake();
		virtual void Excrete();
};  */

class _export TEspichelMan : public TIrishMan
{
	public :
		TEspichelMan(TEcoDynClass* APEcoDynClass, char* className);
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		virtual void Go();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.16
        virtual bool SaveParameters();

	protected :
		virtual void MacrophyteHarvest();

   double MinimalHarvestSize;
   // priceKg_Gelidium, priceKg_Gelidium_harvest : real;
};


class _export TSangoMan : public TMan
{
	public :
        // constructors invoked outside EcoDyn
        TSangoMan(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[],
            int nParams, double kelpFirstSeedingDay, double kelpLastSeedingDay,
            double kelpFirstHarvestDay, double kelpLastHarvestDay,
            double seedWeightOfKelp, double biomassInputOfKelp,
            int aCultivationAreaOfKelp[],
            double oysterFirstSeedingDay, double oysterLastSeedingDay,
            double oysterFirstHarvestDayFirstPeriod, double oysterLastHarvestDayFirstPeriod,
            double oysterFirstHarvestDaySecondPeriod, double oysterLastHarvestDaySecondPeriod,
            double shellSeedWeightOfOyster, double meatSeedWeightOfOyster,
            double seedInputOfOyster, double harvestableSizeOfOyster,
            int aCultivationAreaOfOyster[],
            double scallopFirstSeedingDay, double scallopLastSeedingDay,
            double scallopFirstHarvestDayFirstPeriod, double scallopLastHarvestDayFirstPeriod,
            double scallopFirstHarvestDaySecondPeriod, double scallopLastHarvestDaySecondPeriod,
            double shellSeedWeightOfScallop, double meatSeedWeightOfScallop,
            double seedInputOfScallop, double harvestableSizeOfScallop,
            int aCultivationAreaOfScallop[]);
        TSangoMan(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TSangoMan(TEcoDynClass* APEcoDynClass, char* className);
      ~TSangoMan();
      virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Integrate();
		virtual void Go();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.16
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
        void PreBuildSangoMan();
        void BuildSangoMan();
   	    virtual void Seed();
		virtual void KelpSeed();
      virtual void OysterSeed();
      virtual void ScallopSeed();
      virtual void KelpHarvest();
      virtual void OysterHarvest();
      virtual void ScallopHarvest();
      void InitKelpCultivationArea();
      void InitOysterCultivationArea();
      void InitScallopCultivationArea();

      double *HarvestBiomassOfKelp, *HarvestFluxOfKelp,
             *BiomassInputOfKelp, *SeedWeightOfKelp,
		       *HarvestableBiomassOfKelp, *HarvestBiomassOfOyster,
		       *HarvestFluxOfOyster, *SeedInputOfOyster,
             *ShellSeedWeightOfOyster, *MeatSeedWeightOfOyster,
             *HarvestableBiomassOfOyster, *HarvestableSizeOfOyster,
             *HarvestBiomassOfScallop, *HarvestFluxOfScallop,
             *SeedInputOfScallop, *ShellSeedWeightOfScallop, *MeatSeedWeightOfScallop,
		       *HarvestableBiomassOfScallop, *HarvestableSizeOfScallop;

      int *CultivationAreaOfKelp, *CultivationAreaOfScallop, *CultivationAreaOfOyster;

      double KelpFirstSeedingDay, KelpLastSeedingDay,  //Kelp parameters
             KelpFirstHarvestDay, KelpLastHarvestDay;

      double ScallopFirstSeedingDay, ScallopLastSeedingDay,  //Scallop parameters
             ScallopFirstHarvestDayFirstPeriod, ScallopLastHarvestDayFirstPeriod,
             ScallopFirstHarvestDaySecondPeriod, ScallopLastHarvestDaySecondPeriod,
             ScallopCommercialSize;

      double OysterFirstSeedingDay, OysterLastSeedingDay,  //Oyster parameters
             OysterFirstHarvestDayFirstPeriod, OysterLastHarvestDayFirstPeriod,
             OysterFirstHarvestDaySecondPeriod, OysterLastHarvestDaySecondPeriod,
             OysterCommercialSize;


};

class _export TFisherMan : public TMan
{
	public :
        // constructors invoked outside EcoDyn
		TFisherMan(TEcoDynClass* APEcoDynClass, char* className);
      ~TFisherMan();
      virtual void freeMemory();
      virtual void Go();
      // AP, 2005.11.11
      virtual void procMsgECDP(EcoDynProtocol* ecdp,
            MESSAGE_ECDP* msg, RegionList* pRL, bool notRunning);

      virtual Queue* getSeedingQueue();
        // AP, 2011.02.05
        virtual bool SaveVariables();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);

	protected :
//      int NumberOfBoxesWithBenthicSpecies;
//      BenthicSpeciesRecord *MyBenthicSpeciesRecord;

      // AP, 2005.11.15...
      Queue* seedQueue;
      Queue* inspectQueue;
      Queue* harvestQueue;

      // AP, 2006.06.30
      Queue* seedingQueue;

      void BuildFisher();

      // AP, 2011.02.01
      SEED_DB* findSeedDB(int firstDay, int lastDay);
      HARVEST_DB* findHarvestDB(int firstDay, int lastDay);

      // FisherMan_Kelp.cpp
      virtual void KelpHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion);

      // FisherMan_Oyster.cpp
      virtual void OysterHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion);

      // FisherMan_Scallop.cpp
      virtual void ScallopHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion);

      // FisherMan_Mussel.cpp
      virtual void MusselHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion);

      // FisherMan_Clam.cpp
      virtual void ClamHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion);

      // FisherMan_Inspect.cpp
      virtual void BivalvesInspect(INSPECT_DB* pDB);
};

static TEcoDynClass *PMan;
static TEcoDynClass *PFisherMan;    // AP, 20110204

#endif  //MANOBJT_H


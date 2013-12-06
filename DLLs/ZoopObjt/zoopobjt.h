// Zooplankton object header file
// REV A - 96.03.11
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef ZOOPOBJT_H
#define ZOOPOBJT_H

#include "EcoDynClass.h"


// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TZooplankton : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
        TZooplankton(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TZooplankton(char* className, float timeStep, char* morphologyFilename);

		TZooplankton(TEcoDynClass* APEcoDynClass, char* className);
		~TZooplankton();
        virtual void freeMemory();
		virtual void Go ();
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxX,
										 int BoxY,
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
        void BuildZooplankton(char* className);
		double
						*ZooBiomass,
						*ZooLoad,
						*ZooProduction,
						*ZooFlux,
						*ZooGrossProduction,
						*ZooNetProduction;

		AdvectionDiffusionRecord *ZooplanktonInterfaceExchange;


		double RiverZoo,
				 OceanZoo;

		// Conversion factors
		double DryWeightToFreshWeight,
				 ChlorophyllToCarbon,
				 CarbonToDryWeight,
				 ExcretionLoss,
				 DOCLoss,
				 DeathLoss,
             RedfieldCFactor,
             RedfieldNFactor,
             RedfieldPFactor;

		double ZooProd,
				 Ration,
				 Rmax,
				 KGraze,
				 Metabolism,
				 PhytoLowerLimit,
				 KZooStarvedByPhyto,
				 MaximumDeathLoss,
				 Q10Coefficient,
				 Q10WaterTempReference,
				 Q10KPhytoMass,
				 Q10PhytoLowerLimit,
				 Q10Rmax,
             AssimilationEfficiency,
             AmmoniaFractionOfExcretion,
             DetritalFractionOfMortality;

       double
      	PhytoCoefficient,
         DetritalCoefficient,
         BacterialCoefficient;

	  virtual void Graze();
 //	  virtual void GrazeWithTemperature();
	  virtual void Drool();
     virtual void Faeces();
	  virtual void Excrete();
	  virtual void Respire();
	  virtual void Die();
};

class _export TZooplanktonBasic : public TZooplankton
{
	public :
        // constructors invoked outside EcoDyn
        TZooplanktonBasic(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverZoo, double oceanZoo,
            double aZooBiomass[], double aZooLoad[],
            int nParams, double kGraze, double phytoLowerLimit, double rmax,
            double deathLoss, double docLoss, double metabolism, double excretionLoss,
            double maximumDeathLoss, double kZooStarvedByPhyto, double q10Coefficient,
            double q10WaterTempReference, double q10KPhytoMass, double q10PhytoLowerLimit,
            double q10Rmax, double phytoCoefficient, double detritalCoefficient,
            double bacterialCoefficient, double assimilationEfficiency,
            double ammoniaFractionOfExcretion, double detritalFractionOfMortality,
            double redfieldCFactor, double redfieldNFactor, double redfieldPFactor);
        TZooplanktonBasic(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TZooplanktonBasic(TEcoDynClass* APEcoDynClass, char* className);
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.06
        virtual bool SaveVariables();
        virtual bool SaveParameters();

    protected:
        void PreBuildZooplanktonBasic();
        void BuildZooplanktonBasic();
};

class _export TCarlZooplankton : public TZooplanktonBasic
{
	public :
        // constructors invoked outside EcoDyn
        TCarlZooplankton(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverZoo, double oceanZoo,
            double aZooBiomass[], double aZooLoad[],
            int nParams, double kGraze, double phytoLowerLimit, double rmax,
            double deathLoss, double docLoss, double metabolism, double excretionLoss,
            double maximumDeathLoss, double kZooStarvedByPhyto, double q10Coefficient,
            double q10WaterTempReference, double q10KPhytoMass, double q10PhytoLowerLimit,
            double q10Rmax, double phytoCoefficient, double detritalCoefficient,
            double bacterialCoefficient, double assimilationEfficiency,
            double ammoniaFractionOfExcretion, double detritalFractionOfMortality,
            double redfieldCFactor, double redfieldNFactor, double redfieldPFactor);
        TCarlZooplankton(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TCarlZooplankton(TEcoDynClass* APEcoDynClass, char* className);
	protected :
		virtual void Graze();
		virtual void Die();
};

class _export TCalaZooplankton : public TZooplanktonBasic
{
	public :
        // constructors invoked outside EcoDyn
        TCalaZooplankton(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverZoo, double oceanZoo,
            double aZooBiomass[], double aZooLoad[],
            int nParams, double kGraze, double phytoLowerLimit, double rmax,
            double deathLoss, double docLoss, double metabolism, double excretionLoss,
            double maximumDeathLoss, double kZooStarvedByPhyto, double q10Coefficient,
            double q10WaterTempReference, double q10KPhytoMass, double q10PhytoLowerLimit,
            double q10Rmax, double phytoCoefficient, double detritalCoefficient,
            double bacterialCoefficient, double assimilationEfficiency,
            double ammoniaFractionOfExcretion, double detritalFractionOfMortality,
            double redfieldCFactor, double redfieldNFactor, double redfieldPFactor);
        TCalaZooplankton(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TCalaZooplankton(TEcoDynClass* APEcoDynClass, char* className);
	protected :
		virtual void Graze();
};


class _export TVerticalModelZooplankton : public TZooplanktonBasic   //Pedro Duarte 6/4/98
{
	public :
        // constructors invoked outside EcoDyn
        TVerticalModelZooplankton(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double riverZoo, double oceanZoo,
            double aZooBiomass[], double aZooLoad[],
            int nParams, double kGraze, double phytoLowerLimit, double rmax,
            double deathLoss, double docLoss, double metabolism, double excretionLoss,
            double maximumDeathLoss, double kZooStarvedByPhyto, double q10Coefficient,
            double q10WaterTempReference, double q10KPhytoMass, double q10PhytoLowerLimit,
            double q10Rmax, double phytoCoefficient, double detritalCoefficient,
            double bacterialCoefficient, double assimilationEfficiency,
            double ammoniaFractionOfExcretion, double detritalFractionOfMortality,
            double redfieldCFactor, double redfieldNFactor, double redfieldPFactor);
        TVerticalModelZooplankton(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TVerticalModelZooplankton(TEcoDynClass* APEcoDynClass, char* className);
      ~TVerticalModelZooplankton();
      virtual void freeMemory();
      virtual void Go();
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Integrate();
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
	protected :
        void BuildVerticalModelZoo();
		virtual void Graze();
      virtual void Drool();
      virtual void Faeces();
      virtual void Respire();
      //virtual void Excrete();
      virtual void Excrete(int i);
      virtual void Die();
      /*double
      	PhytoCoefficient,
         DetritalCoefficient,
         BacterialCoefficient; */
		double
      	*AverageGrazedNCRatio;

};


class _export TSangoZooplankton : public TZooplankton
{
	public :
        // constructors invoked outside EcoDyn
        TSangoZooplankton(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aZooBiomass[], double aBoundaryZooConcentration[],
            int nParams, double kGraze, double phytoLowerLimit, double rmax,
            double deathLoss, double docLoss, double metabolism, double excretionLoss,
            double maximumDeathLoss, double kZooStarvedByPhyto, double q10Coefficient,
            double q10WaterTempReference, double q10KPhytoMass, double q10PhytoLowerLimit,
            double q10Rmax, double phytoCoefficient, double detritalCoefficient,
            double bacterialCoefficient, double assimilationEfficiency,
            double ammoniaFractionOfExcretion, double detritalFractionOfMortality,
            double redfieldCFactor, double redfieldNFactor, double redfieldPFactor);
        TSangoZooplankton(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TSangoZooplankton(TEcoDynClass* APEcoDynClass, char* className);
      ~TSangoZooplankton();
      virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Go();
		virtual void Integrate();
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.06
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected :
        void PreBuildSangoZooplankton();
        void BuildSangoZooplankton();
   	virtual void Graze(int ABoxNumber);
	   virtual void Drool(int ABoxNumber);
      virtual void Faeces(int ABoxNumber);
	   virtual void Excrete(int ABoxNumber);
	   virtual void Respire(int ABoxNumber);
	   virtual void Die(int ABoxNumber);
      double *BoundaryZooConcentration;
      double RedfieldCFactor, RedfieldNFactor;
      int GridLines, GridColumns,
          WesternBoundaryNumber, SouthernBoundaryNumber,
          EasternBoundaryNumber, NorthernBoundaryNumber;

};


static TEcoDynClass *PZooplankton;

#endif //ZOOPOBJT_H



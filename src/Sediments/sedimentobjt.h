// Sediment object header file
// REV A - 2005.03.06

#ifndef SEDIMENTOBJ_H
#define SEDIMENTOBJ_H

/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TSediments : public TEcoDynClass
{
	public :
		TSediments(TEcoDynClass* APEcoDynClass, char* className);
		~TSediments();
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
        // AP, 2007.07.06
        virtual bool SaveVariables();
        virtual bool SaveParameters();
        
	protected :
           //void BuildSediments(char* className);
           void ReadParameters(char* className);
           void InitializeVariables();
           void ReadVariables(char* className);
	   virtual void BenthicNitrogenMineralization(int ABoxNumber, int ASedimentRecordNumber);
	   virtual void BenthicPhosphorusMineralization(int ABoxNumber, int ASedimentRecordNumber);
           virtual void BenthicCarbonMineralization(int ABoxNumber, int ASedimentRecordNumber);
	   virtual void AmmoniaSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
           virtual void NitrateSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
           virtual void PhosphorusSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
           virtual void OxygenSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
	   virtual void PhosphorusAdsorption(int ABoxNumberint, int ASedimentRecordNumber);
	   virtual void PhosphorusDesorption(int ABoxNumber, int ASedimentRecordNumber);
           virtual void Nitrification(int ABoxNumber, int ASedimentRecordNumber);
           virtual void DeNitrification(int ABoxNumber, int ASedimentRecordNumber);
           double OxygenLimitation(int ABoxNumber, double K);
           double TemperatureLimitation(int ABoxNumber);
           //Variables
           double *Norganic, *NH4InPoreWater, *NO3InPoreWater, *Porganic, *PO4InPoreWater, *O2InPoreWater, *Padsorbed,
                  *NorganicFlux, *NH4InPoreWaterFlux, *PorganicFlux, *PO4InPoreWaterFlux, *O2InPoreWaterFlux, *NO3InPoreWaterFlux,
                  *Mineralization_N, *Mineralization_P, *Adsorption_P, *Desorption_P, *Diffusion_NH4, *Diffusion_NO3, *Diffusion_P, *Diffusion_O2, *SorptionFlux,
                  *NitrificationFlux, *DeNitrificationFlux, *NorganicInOut, *PorganicInOut, *Corganic, *Mineralization_C, *CorganicFlux, *CorganicInOut,
                  *OrganicMass, *InorganicMass, *OrganicMassFlux, *InorganicMassFlux,*InorganicInOut, *OrganicInOut, *ReferenceHeight, *LayerDepth, *SedimentMass, *SedimentDensity,
                  *CarbonToOrganics;
           //Parameters
           double Kt, KminO2, DiffusionCoefficientForN, Ka1, Ka2, Pmax, O2thrs, DiffusionCoefficientForP, Kd, minNs,
                  FractionMineralizedToAmmonia, minPs, FractionMineralizedToPhosphorus,
                  CriticalWaterDepth, DiffusionLayer, DiffusionCoefficientForO2, knit, kdenit, knitO2, kdenitO2, minCs,
                  //CarbonToOrganics,
                  DiffusionCoefficientForNInSediments, DiffusionCoefficientForPInSediments, DiffusionCoefficientForO2InSediments,
                  ReferenceDepth,MinTopDepth, MaxTopDepth, ProportionOfNH4FromDenitrification;
           double MineralDensity, OrganicMatterDensity;
           int NumberOfBoxesWithSediments, NumberOfSedimentLayers;
           SedimentsRecord *MySedimentsRecord;
};

class _export TSedimentsPlus : public TSediments
{
//Esta classe difere da ascendente nos seguintes aspectos:
//(i) os coeficientes de difus?o em vez de serem gerais para o azoto e o f?sforo s?o discriminados
//pelas v?rias formas de azoto e de f?sforo;
//(ii) contempla as formas org?nicas dissolvidas de azoto e de f?sforo;
//(iii) inclui os processos de dissolu??o das formas particuladas org?nicas;
//(iv) inclui a dissolu??o da mat?ria org?nica particulada
//(v)  icluir  dissolu??o da mat?ria org?nica particulada e a sua inclus?o nas "pools" de mat?ria org?nica dissolvida 
      public :
		   TSedimentsPlus(TEcoDynClass* APEcoDynClass, char* className);
           ~TSedimentsPlus();
           virtual void freeMemory();
           virtual double GetParameterValue(char* parmName);
           virtual bool SetParameterValue(char* parmName, double value);
           virtual void Go();
           virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
           virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
           virtual void Integrate();
           virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
           virtual bool SaveVariables();
           virtual bool SaveParameters();
      protected :
           void ReadParameters(char* className);
           void InitializeVariables();
           void ReadVariables(char* className);
           virtual void BenthicDissolvedCarbonMineralization(int ABoxNumber, int ASedimentRecordNumber);
           virtual void BenthicDissolvedNitrogenMineralization(int ABoxNumber, int ASedimentRecordNumber);
	       virtual void BenthicDissolvedPhosphorusMineralization(int ABoxNumber, int ASedimentRecordNumber);
           virtual void CorganicDissolvedSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
           virtual void NorganicDissolvedSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
           virtual void PorganicDissolvedSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer);
           virtual void DissolutionOfSedimentOrganics(int ABoxNumber, int ASedimentRecordNumber);
           //Variables
           double *CorganicDissolved,*NorganicDissolved, *PorganicDissolved,
                  *CorganicDissolvedFlux,*NorganicDissolvedFlux, *PorganicDissolvedFlux,
                  *Mineralization_CorganicDissolved,*Mineralization_NorganicDissolved, *Mineralization_PorganicDissolved,
                  *Diffusion_CorganicDissolved,*Diffusion_NorganicDissolved, *Diffusion_PorganicDissolved,
                  *Dissolution_CSediments, *Dissolution_NSediments, *Dissolution_PSediments;
           //Parameters
           double DiffusionCoefficientForNO3, DiffusionCoefficientForNH4,
                  DiffCoeffForCorganicDissolved,
                  DiffCoeffForNorganicDissolved,
                  DiffCoeffForPorganicDissolved,
                  DiffCoeffForNO3InSediments, DiffCoeffForNH4InSediments,
                  DiffCoeffForCorganicDissolvedInSediments,
                  DiffCoeffForNorganicDissolvedInSediments,
                  DiffCoeffForPorganicDissolvedInSediments,
                  OrganicFractionThatDissolves,
                  minCd, minNd, minPd, Ktd, KminO2d;
};

class _export TSedimentsTimeSeries : public TSediments
{
      public :
		TSedimentsTimeSeries(TEcoDynClass* APEcoDynClass, char* className);
                virtual void Go();
                virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
                virtual void Integrate();
                virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
};

static TEcoDynClass *PSediments;

#endif  // SEDIMENTOBJ_H



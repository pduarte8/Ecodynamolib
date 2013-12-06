/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */


#ifndef FISHOBJT_H
#define FISHOBJT_H

#include "EcoDynClass.h"
#include "iodll.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TFish : public TEcoDynClass
{
     public :
        TFish(TEcoDynClass* APEcoDynClass, char* className);
        ~TFish();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        virtual void Go();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Inquiry(char* srcName, double &Value, int FishNumber, int BoxNumber,char* ParameterName,int AnObjectCode);
        VNA* Inquiry(char* srcName,int &Individuals, int &DimVariables);
        virtual void Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int ClassNumber,int AnObjectCode);
        virtual void Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int AnObjectCode);
        virtual void Integrate();
        virtual void Reinitialize();
        virtual bool SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName);
        virtual bool SaveVariables();
        virtual bool SaveParameters();

     protected :
        virtual void BuildFish(char* className);
        virtual void BuildFish();
        virtual void PreBuildFish();
        virtual void PostBuildFish();
        virtual void ReadParameters();
        bool ReadParsRecord(int X, int i, int index);
        virtual void ReadVariables();
        virtual void Feed(int ABox, int AFish);
        virtual void Absorption(int ABox, int AFish);
        virtual void Egestion(int ABox, int AFish){};
        virtual void Assimilation(int ABox, int AFish);
        virtual void Allocation(int ABox, int AFish);
        virtual void Respire(int ABox, int AFish);
        virtual void Excrete(int ABox, int AFish);
        virtual void Reproduce(int ABox, int AFish);
        virtual void Aging(int ABox, int AFish);
        virtual double FishLength(int AFish);
        virtual double TemperatureLimitation(double ATA, double AT1, double AWaterTemperature);
        typedef struct  Individual{
           double Volume, Weight, Length, Reserves, Age, Jx, ARC, ARN, AE,
                  IndividualFlux, NetProdC, NetProdN, RespC, RespR, ExcR, SomFrac,StorFrac, PA, Egestion;
           ParsRecord IndividualParameters[100];
        } IndividualRecord;
        ParsRecord *MyParsRecord;
        TReadWrite *MyPReadWrite;
        Individual *MyIndividualRecord;
        int NumberOfIndividuals, NumberOfParameterRecords, IndividualBasedVariables;
        double AWaterTemperature, FoodLoad, FoodLoadPerFish,
               F, Xk, AE, Jx, PAm;
        double *FishDensity, *FishBiomass, *TotalIngestion, *TotalAbsorption, *TotalRespiration;
        char MyFishName[100];
        double OxygenToJoules, TINNY, FoodDecayRate, EnergyPerReserveMass;
        bool AbsorptionEfficiency;
};

class _export TDiplodusSargusDEB : public TFish
{
	public :
           // constructors invoked outside EcoDyn still missing
           TDiplodusSargusDEB(TEcoDynClass* APEcoDynClass, char* className);
           //~TDiplodusSargusDEB(){};
        protected :
           //void BuildFish();
           //void PreBuildFish();
           //void PostBuildFish();
           void ReadParameters();
           //void ReadVariables();
};


class _export TSparusAurataDEB : public TFish
{
	public :
           // constructors invoked outside EcoDyn still missing
           TSparusAurataDEB(TEcoDynClass* APEcoDynClass, char* className);
           //~TSparusAurataDEB(){};
        protected :
           //void BuildFish();
           //void PreBuildFish();
           //void PostBuildFish();
           void ReadParameters();
           //void ReadVariables();
};

class _export TFishWithWeakHomeostasis : public TFish
{
	public :
           // constructors invoked outside EcoDyn still missing
           TFishWithWeakHomeostasis(TEcoDynClass* APEcoDynClass, char* className);
           //~TFishWithWeakHomeostasis(){};
        protected :
           virtual void Feed(int ABox, int AFish);
           virtual void Absorption(int ABox, int AFish);
           virtual void Egestion(int ABox, int AFish);
           virtual void Respire(int ABox, int AFish);
           virtual void Excrete(int ABox, int AFish);
           double AverageRQ, PolysaccharidesRQ, LipidsRQ, ProteinsRQ, FishCNratio, FishCPratio,
                  FoodCNratio, FoodCPratio, FoodCContent, FoodNContent, FoodPContent,
                  PolysaccharideContent, LipidContent, ProteinContent, FoodC, FoodN, FoodP,
                  CarbonLostByResp, NitrogenLostByExcretion, PhosphorusLostByExcretion, FoodEnergyContent;
};

class _export TDiplodusSargusDEBWithWeakHomeostasis : public TFishWithWeakHomeostasis
{
	public :
           // constructors invoked outside EcoDyn still missing
           TDiplodusSargusDEBWithWeakHomeostasis(TEcoDynClass* APEcoDynClass, char* className);
           //~TDiplodusSargusDEBWithWeakHomeostasis(){};
        protected:
           //void BuildFish();
           //void PreBuildFish();
           //void PostBuildFish();
           void ReadParameters();
           //void ReadVariables();
};

class _export TSparusAurataDEBWithWeakHomeostasis : public TFishWithWeakHomeostasis
{
	public :
           // constructors invoked outside EcoDyn still missing
           TSparusAurataDEBWithWeakHomeostasis(TEcoDynClass* APEcoDynClass, char* className);
           //~TSparusAurataDEBWithWeakHomeostasis(){};
        protected:
           //void BuildFish();
           //void PreBuildFish();
           //void PostBuildFish();
           void ReadParameters();
           //void ReadVariables();
};



static TEcoDynClass *PFish;

#endif  // FISHOBJT_H


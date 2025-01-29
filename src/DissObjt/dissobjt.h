// Dissolved object header file
// REV A - 96.01.22

#ifndef DISSOBJT_H
#define DISSOBJT_H
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#include "EcoDynClass.h"

// DLL header file
/*
extern "C"
{
*/
//#ifndef __BORLANDC__
//#define _export 
//#endif  // __BORLANDC__
#ifndef __BORLANDC__
using namespace std;
#define _export 
#endif  // __BORLANDC__

class _export TNutrients : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TNutrients(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        TNutrients(char* className, float timeStep, char* morphologyFilename);

	TNutrients(TEcoDynClass* APEcoDynClass, char* className);
#else
        TNutrients::TNutrients(char* className);
#endif
	~TNutrients();
        virtual void freeMemory();
	virtual void Go();
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
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

	//protected :
		double
				  *Ammonia,           // Nitrogen species
                  *Nitrite,
				  *Nitrate,
                  *NH4Load,
                  *NO2Load,
                  *NO3Load,
                  *DON,               //Pedro Duarte  20/4/98
                  *DONLoad,           //Pedro Duarte  20/4/98
                  *DOP,
                  *DOPLoad,
                  *Phosphate,         // Phosphorus species
                  *PO4Load,
                  *Silica,            // Silica species
                  *SiO2Load,
                  *DOC,               //Carbon species - Pedro Duarte  20/4/98
                  *DOCLoad,
                  *O2Load,
                  *NH4Flux,           // Fluxes - derivatives
                  *NO2Flux,
                  *NO3Flux,
                  *DONFlux,           //Pedro Duarte  20/4/98
                  *DOCFlux,           //Pedro Duarte  20/4/98
                  *DOPFlux,
                  *PO4Flux,
                  *SiO2Flux,
                  *IMNH4, *IFNH4,     // Iterim mass and flux
                  *IMNO2, *IFNO2,
                  *IMNO3, *IFNO3,
                  *IMDON, *IFDON,     //Pedro Duarte  20/4/98
                  *IMDOC, *IFDOC,     //Pedro Duarte  20/4/98
                  *IMPO4, *IFPO4,
                  *IMSi, *IFSi,
                  *NH4RiverLoad,
                  *NO2RiverLoad,
                  *NO3RiverLoad,
                  *DONRiverLoad,           //Pedro Duarte  20/4/98
                  *DOPRiverLoad,
                  *PO4RiverLoad,
                  *SiO2RiverLoad,
                  *DOCRiverLoad,
                  *O2RiverLoad;

		double RiverAmmonia,
				 OceanAmmonia,
				 RiverNitrite,
				 OceanNitrite,
				 RiverNitrate,
				 OceanNitrate,
                 RiverDON,        //Pedro Duarte  20/4/98
                 RiverDOP,
                 OceanDON,        //Pedro Duarte  20/4/98
                 OceanDOP,
                 RiverDOC,        //Pedro Duarte  20/4/98
                 OceanDOC,        //Pedro Duarte  20/4/98
				 RiverPhosphate,
				 OceanPhosphate,
				 RiverSilica,
				 OceanSilica;

		// Proportion of DIN in each form of dissolved nitrogen
		double NH4_TO_DIN,
				 NO2_TO_DIN,
				 NO3_TO_DIN;

		// Redfield ratios
		double Atomic_NP_ratio, Mass_NP_ratio;

		AdvectionDiffusionRecord *DINInterfaceExchange;

        void BuildNutrients(char* className);
		virtual void RedfieldRatio();
		virtual void DINProportion(int ABoxNumber);
		virtual void Nitrification();
        typedef struct ChemicalLoads
        {
            char LoadName[210];
            int LineCoordinate, ColumnCoordinate, LayerCoordinate;
            double *NH4Load, *NO3Load, *NO2Load, *PO4Load, *SiO2Load, *O2Load, *DOCLoad, *DONLoad, *DOPLoad;
        }  LoadRecord;
        LoadRecord* ALoadRecord;
        typedef struct RiverLoads
        {
            char RiverName[210];
            int LineCoordinate, ColumnCoordinate, LayerCoordinate;
            double *NH4RiverLoad, *NO3RiverLoad, *NO2RiverLoad, *PO4RiverLoad, *SiO2RiverLoad, *O2RiverLoad, *DOCRiverLoad, *DONRiverLoad, *DOPRiverLoad;
        }  LoadRiverRecord;
        LoadRiverRecord* ALoadRiverRecord;
       int NumberOfLoads, NumberOfRiverLoads, NumberOfDaysForRiverLoads, NumberOfDaysForLoads, NumberOfDaysForBoundaryTimeSeries, MyNumberOfBoxesInSubDomain;

};

class _export TSangoNutrients: public  TNutrients
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TSangoNutrients(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[],
            double aAmmonia[], double aNH4Load[], double aNitrite[], double aNO2Load[],
            double aNitrate[], double aNO3Load[], double aBoundaryNH4Concentration[],
            double aBoundaryNO3Concentration[], double aBoundaryNO2Concentration[]);
        TSangoNutrients(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename);

	TSangoNutrients(TEcoDynClass* APEcoDynClass, char* className);
#else
        TSangoNutrients(char* className);
#endif //_PORT_FORTRAN_
        ~TSangoNutrients();
        virtual void freeMemory();
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
		virtual void Integrate();
        virtual void Go();
        // AP, 2007.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.10
        virtual bool SaveVariables();

   protected:
        void PreBuildSangoNutrients();
        void BuildSangoNutrients();
        void PosBuildSangoNutrients();
   	    int GridColumns, GridLines,
            WesternBoundaryNumber, SouthernBoundaryNumber,
            EasternBoundaryNumber, NorthernBoundaryNumber;
        double *BoundaryNH4Concentration, *BoundaryNO3Concentration, *BoundaryNO2Concentration;
};


class _export TRiaF2DNutrients: public  TSangoNutrients
{
	public :
#ifndef _PORT_FORTRAN_
	TRiaF2DNutrients(TEcoDynClass* APEcoDynClass, char* className);
#else
        static TRiaF2DNutrients* getNuts();
        TRiaF2DNutrients(char* className);
        void SetNumberOfLines(int numberOfLines) {NumberOfLines = numberOfLines;}
        void SetNumberOfColumns(int numberOfColumns) {NumberOfColumns = numberOfColumns;}
        void SetNumberOfLayers(int numberOfLayers) {NumberOfLayers = numberOfLayers;}
        void SetNumberOfBoxes(int numberOfBoxes) {NumberOfBoxes = numberOfBoxes;}
        void SetABoxDepth(double layerThickness) {BoxDepth = layerThickness;}   
        void SetABoxNumber (int aBoxNumber) {ABoxNumber = aBoxNumber;}  
        void SetWaterTemperature(double waterTemperature){WaterTemperature=waterTemperature;}
#endif
        ~TRiaF2DNutrients();
        virtual void freeMemory();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName, int AnObjectCode);
        // AP, 2006.05.26
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
		virtual void Integrate();
        virtual void Go();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        // AP, 2007.07.10
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   //protected:
        void BuildRiaF2DNutrients();
        //void PosBuildRiaF2DNutrients();
        void Loads();
        /*typedef struct ChemicalLoads
        {
            char LoadName[210];
            int LineCoordinate, ColumnCoordinate;
            double *NH4Load, *NO3Load, *NO2Load, *PO4Load, *SiO2Load, *DOCLoad;
        }  LoadRecord;
        LoadRecord* ACLoadRecord;
        typedef struct RiverLoads
        {
            char RiverName[210];
            int LineCoordinate, ColumnCoordinate;
            double *NH4RiverLoad, *NO3RiverLoad, *NO2RiverLoad, *PO4RiverLoad, *SiO2RiverLoad, *DOCRiverLoad, *O2RiverLoad;
        }  LoadRiverRecord;
        LoadRiverRecord* ALoadRiverRecord;*/

        virtual void Nitrification(int ABoxNumber);
        virtual void DeNitrification(int ABoxNumber);
        double OxygenLimitation(int ABoxNumber, double K);
        double TemperatureLimitation(int ABoxNumber);
        double RaerationCoefficient(double AWindSpeed, double ADepth);
        virtual void Raeration(int ABoxNumber, double ADepth);
        virtual void NitrogenMineralization(int ABoxNumber);
        virtual void PhosphorusMineralization(int ABoxNumber);
        virtual void CarbonMineralization(int ABoxNumber);
        double LightInhibitionOfNitrification(int ABoxNumber);

   double CriticalDepthForLandBoundary, knit, kdenit, knitO2, kdenitO2, Kt, ProportionOfNH4FromDenitrification, ARaerationCoefficient,
          minRate, kminO2, FractionMineralizedToAmmonia, BoxDepth, WaterTemperature, ThresholdForLightInhib, HalfSatForLightInhib,
          lightAtTop, lightAtBottom, kValue;
   double *Oxygen, *OxygenFlux, *OxygenLoad, *OxygenSaturation, *TemporaryOxygenSaturation, *BoundaryPhosphateConcentration, *BoundaryOxygenConcentration,
          *BoundaryDOCConcentration, *BoundaryDONConcentration, *BoundaryDOPConcentration,
          *WaterNitrificationFlux, *WaterDeNitrificationFlux, *RaerationFlux, *RaerationCoef, *TemporaryRaerationCoef,
          *WaterNitrogenMineralizationFlux, *WaterPhosphorusMineralizationFlux, *WaterCarbonMineralizationFlux, *DIC, *CO2, *Alkalinity;
   int ABoxNumber;
/*#ifdef _PORT_FORTRAN_
       private:
          static TRiaF2DNutrients *PNutrients;
#endif  //_PORT_FORTRAN_*/
};

#ifndef _PORT_FORTRAN_^M
static TEcoDynClass *PNutrients;^M
#else  //_PORT_FORTRAN_

/* Functions that can be called from Fortran */
extern "C" {
              void dissobjt_new__(long *PNutrients, double *NitriR, double *kdenit, double *knitO2, double *kdenitO2,  double *kt, double *minRate,double *ProportionOfNH4FromDenitrification, double *FractionMineralizedToAmmonia, double *ThresholdForLightInhib, double *HalfSatForLightInhib);

              void dissobjt_go__(long *PNutrients, double *layerThickness, double *timeStep);

              void dissobjt_nitrification__(long *PNutrients, double *lightAtTop, double *lightAtBottom, double *kValue, double *layerThickness, double * waterTemperature,
                                            double *Ammonia, double *Oxygen, double *NitrificationFlux, double *OxygenFlux);
              void dissobjt_denitrification__(long *PNutrients, double * waterTemperature,
                                            double *Nitrate, double *Oxygen, double *AmmoniaFlux, double *NitrateFlux);

              void dissobjt_CarbonMineralization__(long *PNutrients, double * waterTemperature,
                                                    double *OrganicCarbon, double *Oxygen, double *OrganicCarbonFlux, double *minRateC);
              void dissobjt_NitrogenMineralization__(long *PNutrients, double * waterTemperature,
                                                    double *OrganicNitrogen, double *Oxygen, double *OrganicNitrogenFlux, double *OxygenFlux,  double *minRateN);
              void dissobjt_PhosphorusMineralization__(long *PNutrients, double * waterTemperature,
                                                    double *OrganicPhosphorus, double *Oxygen, double *OrganicPhosphorusFlux, double *minRateP);       

} // of extern C

#endif  //_PORT_FORTRAN_
#endif  // DISSOBJT_H



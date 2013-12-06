// Bacteria object header file
/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef BACTERIAOBJT_H
#define BACTERIAOBJT_H

#include "EcoDynClass.h"
#include "iodll.h"

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TBacteria : public TEcoDynClass
{
	public :
          TBacteria(TEcoDynClass* APEcoDynClass, char* className);
          ~TBacteria();
          virtual void freeMemory();
          virtual double GetParameterValue(char* parmName);
          virtual bool SetParameterValue(char* parmName, double value);
          virtual void Go();
          virtual void Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName,int AnObjectCode);
          virtual void Inquiry(char* srcName, double &Value,int BoxX,int BoxY,char* ParameterName,int AnObjectCode);
          virtual void Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode);
          virtual void Integrate();
          virtual void Reinitialize();
          virtual bool SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName);
          virtual bool SaveParameters();

	protected :
	  long NCounter, PCounter;
          void PreBuildBacteria();
          void BuildBacteria();
          virtual void Production(int ABoxNumber);
          virtual void ProductionWithTemperature(int ABoxNumber);
          virtual void NutrientLimitation(int ABoxNumber);
          virtual void NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber);
          virtual void TemperatureLimitation(int ABoxNumber);
          virtual void Respiration(int ABoxNumber);
          virtual void Exudation(int ABoxNumber);
          virtual void Senescence(int ABoxNumber);
          virtual void Mortality(int ABoxNumber);
          virtual void Loads();
          typedef struct BacteriaLoads
          {
             char LoadName[210];
             int LineCoordinate, ColumnCoordinate, LayerCoordinate;
             double *BacteriaNumberLoad;
             double *BacteriaCLoad;
             double *BacteriaNLoad;
             double *BacteriaPLoad;
          }  LoadRecord;
          LoadRecord* ALoadRecord;
          typedef struct RiverLoads
          {
             char RiverName[210];
             int LineCoordinate, ColumnCoordinate, LayerCoordinate;
             double *BacteriaNumberRiverLoad;
             double *BacteriaCRiverLoad;
             double *BacteriaNRiverLoad;
             double *BacteriaPRiverLoad;
             double *BacteriaBiomassLoad;
          }  LoadRiverRecord;
          LoadRiverRecord* ALoadRiverRecord;
          TReadWrite *MyPReadWrite;
          double *BacteriaFlux, *BacteriaNumberLoad;
          double DailyBoundaryBacteria[365];
          double FallingSpeed, RatioLightDarkRespiration, CriticalDepthForLandBoundary;
          int NumberOfLoads, NumberOfRiverLoads, NumberOfDaysForLoads, GridLines, GridColumns, GridLayers, UpperLayer, MyNumberOfBoxesInSubDomain;
};


class _export TEscherichiaColi : public TBacteria
{
	public :
           TEscherichiaColi(TEcoDynClass* APEcoDynClass, char* className);
           ~TEscherichiaColi();
           virtual void freeMemory();
           virtual void Go();
           virtual void Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName,int AnObjectCode);
           virtual void Integrate();
        protected :
           void ReadParameters();
           void ReadVariables();
           virtual void Mortality(int ABoxNumber, double ABoxDepth, double AKValue, double AWaterTemperature, double ASalinity, double APARLightAtTop, double APARLightAtBottom);
           virtual void Loads();
           double *EschirichiaColiNumber, *K;
           double K0, KT20, KS, KL; //Bacterial die-off coefficients according to Canteras et al. (1995). Wat. Sci. Tech. Vol. 32, No. 2: 37-44
};


static TEcoDynClass *PBacteria;

#endif

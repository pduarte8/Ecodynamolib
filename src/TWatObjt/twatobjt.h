// Water temperature object header file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// REV A - 96.02.09
/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef TWATOBJT_H
#define TWATOBJT_H

#ifndef __BORLANDC__
	#include <iostream>
	
	using namespace std;
	#define _export 
	
#endif  // __BORLANDC__

#include "EcoDynClass.h"

// DLL header file

class _export TWaterTemperature : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TWaterTemperature(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity);
        TWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
#else  // _PORT_FORTRAN_
        static TWaterTemperature* getWaterTemperature();
        TWaterTemperature();

        void SetAirTemperature(double value) {airT = value;}
        void SetSalinity(double value) {sal = value;}
        void SetWaterDensity(double value) {waterD = value;}
        void SetTidalHeight(double value) {tidalH = value;}
        void SetTotalIR(double value) {totalI = value;}
        void SetSubIR(double value) {subI = value;}
        void SetAtmosphericIR(double value) {atIR = value;}
        void SetWindSpeed(double value) {windS = value;}
        void SetAverageWaterTemperature(int box, double value) {AverageWaterTemperature[box] = value;}
        void SetRiverTemperature(double value) {GenericRiver = value;}
        void SetRelativeHumidity(double value) {RelativeHumidity = value;}
        void SetTimeStep(double value) {TimeStep = value;}

        char debug;
#endif  // _PORT_FORTRAN_
		~TWaterTemperature();
        virtual void freeMemory();
		virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName);
        // AP, 2007.07.04
        virtual bool SaveVariables();
        // AP, 2008.01.16
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        virtual bool SaveParameters();
        virtual void Reinitialize();

	protected :
          double *AverageWaterTemperature,*WaterIrradiance,*LatentHeatFlux,*SensibleHeatFlux,*NetHeatFlux;
          double MyWaterDensity,MixedLayerDepth,
          RelativeHumidity, CriticalDepthForLandBoundary;
          virtual void GetWaterTemperature();
          virtual double GetWaterSurfaceRadiation(double AWaterTemperature);
          virtual double GetLatentWaterHeatFlux(double Aesw,double Aesa,double AWindSpeed);
          virtual double GetSaturationVapourPressure(double ATemperature);
          virtual double GetSensibleWaterHeatFlux(double AWaterTemperature,double AnAirTemperature,double AWindSpeed);
          void PreBuildWaterTemperature(char* className);
          void BuildWaterTemperature(char* className);
          SubDomain *RelativeHumiditySubDomains;
          int MyNumberOfBoxesInSubDomain, NumberOfSubDomains;
          typedef struct TemperatureLoads
          {
              char LoadName[210];
              int LineCoordinate, ColumnCoordinate, LayerCoordinate;
              double *TemperatureLoad;
          }  LoadRecord;
          typedef struct RiverLoads
          {
              char RiverName[210];
              int LineCoordinate, ColumnCoordinate, LayerCoordinate;
              double *TemperatureRiverLoad;
          }  LoadRiverRecord;
          LoadRiverRecord* ALoadRiverRecord;
          LoadRecord* ALoadRecord;
          int NumberOfLoads, NumberOfRiverLoads, NumberOfDaysForLoads, NumberOfDaysForRiverLoads, NumberOfDaysForBoundaryTimeSeries;
          int NumberOfMomentsForTimeSeries, NumberOfDaysForTimeSeries, NumberOfHoursForTimeSeries;
          double *WaterTemperatureLoad, *RelativeHumidityTimeSeries;
          double LatentFluxReductionFactor;
#ifdef _PORT_FORTRAN_
    private:
        static TWaterTemperature *PWaterTemperature;
        double airT, sal, waterD, tidalH, totalI, subI, atIR, windS;
#endif  // _PORT_FORTRAN_
};


#ifndef _PORT_FORTRAN_

class _export TCalaWaterTemperature : public TWaterTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TCalaWaterTemperature(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aAverageTemperature[],
            double genericRiver, double relativeHumidity,
            int nParams, double modalWaterTemperature, double waterTempAmplitude,
            double waterTempPic, double waterTempPhase, double randomWaterTemp);
        TCalaWaterTemperature(char* className, float timeStep, char* morphologyFilename,
            char* variablesFilename, char* parametersFilename);

		TCalaWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
		~TCalaWaterTemperature();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
		virtual void Go();
        // AP, 2007.07.05
        virtual bool SaveParameters();

	protected :
        void BuildCalaWaterTemperature();
       	virtual void GetWaterTemperature();

		double 	ModalWaterTemperature,     // NG 5/6/96
					WaterTempAmplitude,
					WaterTempPic,
					WaterTempPhase,
					RandomWaterTemp;

};

class _export TCarlWaterTemperature : public TWaterTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TCarlWaterTemperature(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity);
        TCarlWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TCarlWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
	protected :
		virtual void GetWaterTemperature();
};


class _export TVerticalModelWaterTemperature : public TWaterTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TVerticalModelWaterTemperature(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity);
        TVerticalModelWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TVerticalModelWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
        virtual void AdvectDiffuseWaterTemperature();
	protected :
		virtual void GetWaterTemperature();
};



class _export TRiaFormosaWaterTemperature : public TWaterTemperature
{
	public:
        // constructors invoked outside EcoDyn
        TRiaFormosaWaterTemperature(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity, double aDailyOceanTemperature[]);
        TRiaFormosaWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

   	    TRiaFormosaWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
        ~TRiaFormosaWaterTemperature();
        virtual void freeMemory();
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
        // AP, 2007.07.05
        virtual bool SaveVariables();

    protected:
        void BuildRiaFormosaWaterTemperature();
   	    double *DailyOceanTemperature;
};



//Zero dimensional water temperature model for St Andr? with tide independent variable depth
class _export TSto0ModelWaterTemperature : public TWaterTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TSto0ModelWaterTemperature(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity);
        TSto0ModelWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TSto0ModelWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
	protected :
		virtual void GetWaterTemperature();
   double LagoonDepth;
};


//Vertically resolved water temperature model for St Andr? with tide independent variable depth
class _export TStoaVerticalModelWaterTemperature : public TSto0ModelWaterTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TStoaVerticalModelWaterTemperature(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity);
        TStoaVerticalModelWaterTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TStoaVerticalModelWaterTemperature(TEcoDynClass* APEcoDynClass, char* className);
        virtual void AdvectDiffuseWaterTemperature();
	protected :
		virtual void GetWaterTemperature();
};


class _export TEspichelWaterTemperature : public TWaterTemperature
{
	public :
		TEspichelWaterTemperature(TEcoDynClass* APEcoDynClass);
      virtual void Go();
	protected :
		virtual void GetWaterTemperature();
   	double DailyWaterTemperature[365];
};


class _export TWaterTemperatureTwoDimensional : public TWaterTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TWaterTemperatureTwoDimensional(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity);
        TWaterTemperatureTwoDimensional(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TWaterTemperatureTwoDimensional(TEcoDynClass* APEcoDynClass, char* className);
        virtual void Go();
        virtual void Integrate();
	protected :
		virtual void GetWaterTemperature();
};

class _export TWaterTemperatureTwoDimensionalForSango : public TWaterTemperatureTwoDimensional
{

	public :
        // constructors invoked outside EcoDyn
        TWaterTemperatureTwoDimensionalForSango(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity, double aDailyOceanTemperature[]);
        TWaterTemperatureTwoDimensionalForSango(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TWaterTemperatureTwoDimensionalForSango(TEcoDynClass* APEcoDynClass,
                char* className);
        ~TWaterTemperatureTwoDimensionalForSango();
        virtual void freeMemory();
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
        // AP, 2007.07.04
        virtual bool SaveVariables();

    protected :
		virtual void GetWaterTemperature();
        void PreBuild2DimSango();
        void Build2DimSango();

        int GridColumns, GridLines;
        int WesternBoundaryNumber, SouthernBoundaryNumber,
            EasternBoundaryNumber, NorthernBoundaryNumber;
        double *DailyOceanTemperature;
};


class _export TWaterTemperatureCrestumaLever : public TWaterTemperature
{

	public :
        // constructors invoked outside EcoDyn
        TWaterTemperatureCrestumaLever(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity,
                double aTemperatureOutflow[], int boxWithThermicEffluent);
        TWaterTemperatureCrestumaLever(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TWaterTemperatureCrestumaLever(TEcoDynClass* APEcoDynClass, char* className);
        ~TWaterTemperatureCrestumaLever();
        virtual void freeMemory();
        virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        double *TemperatureOutflow;
        int BoxWithThermicEffluent;
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.05
        virtual bool SaveVariables();

    protected:
        void BuildCrestumaWaterTemperature();
};


class _export TWaterTemperatureCrestumaTwoDimensional : public TWaterTemperatureTwoDimensional
{

	public :
        // constructors invoked outside EcoDyn
        TWaterTemperatureCrestumaTwoDimensional(char* className, float timeStep,
                int nLines, int nColumns, double aDepth[],
                double aLength[], double aWidth[], double aElevation[], int aType[],
                int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                int nVars, VNA aVarsNames[], double aAverageTemperature[],
                double genericRiver, double relativeHumidity,
        double aTemperatureOutflow[], int boxWithThermicEffluent);
        TWaterTemperatureCrestumaTwoDimensional(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TWaterTemperatureCrestumaTwoDimensional(TEcoDynClass* APEcoDynClass,
                char* className);
        ~TWaterTemperatureCrestumaTwoDimensional();
        virtual void freeMemory();
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
        // AP, 2007.07.05
        virtual bool SaveVariables();

   protected :
		virtual void GetWaterTemperature();
        void PreBuild2DimCrestuma();
        void Build2DimCrestuma();

        int GridColumns, GridLines, BoxWithThermicEffluent, UpperLayer;
        double *TemperatureOutflow, *DailyOceanTemperature;
};

class _export TWaterTemperatureTimeSeries : public TWaterTemperature
{
	public :
  		TWaterTemperatureTimeSeries(TEcoDynClass* APEcoDynClass, char* className);
		~TWaterTemperatureTimeSeries();
        virtual void freeMemory();
		virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.05
        virtual bool SaveVariables();

	protected :
        void BuildWaterTemperature(char* className);
        double *ForcingTemperature;
        int  NumberOfDays, StartYear, StartMonth, StartDate, StartJulianDay, StartCurrentTime,
             FinishYear, FinishMonth, FinishDate, FinishJulianDay, FinishCurrentTime;
};

class _export TWaterTemperatureTimeSeries2 : public TWaterTemperature
{
	public:
        TWaterTemperatureTimeSeries2(TEcoDynClass* PEcoDynClass, char* className);
       	~TWaterTemperatureTimeSeries2();
        virtual void ResetPointers();
        virtual void freeMemory();
        virtual void ResetValues();
        bool ReadTimeSeriesValues(int &fileIndex);
        virtual void Go();
        virtual void ReadCurrentValues();
        virtual void ReinitializeValues();
        virtual void Reinitialize();
    protected:
        int MeanValuesSamplesNumber,indexFile, MyNumberOfBoxes, steps, AStep;
        double *mWaterTemperature;
        int *MyBoxNumber;
        long *timeSeries;
        long StartTime, meanTimeDiff, myTimeSeriesStep, AMoment;
        VNA* TSVariableNameArray;
        bool firstRead, recursiveCall, FoundFirstStep;
};

class _export TWaterTemperatureTwoDimensionalForRiaFormosa : public TWaterTemperatureTwoDimensionalForSango
{

	public :
		TWaterTemperatureTwoDimensionalForRiaFormosa(TEcoDynClass* APEcoDynClass,
                char* className);

        virtual void Go();
        virtual void Integrate();
        ~TWaterTemperatureTwoDimensionalForRiaFormosa();
        virtual void freeMemory();
        // AP, 2007.07.05
        virtual bool SaveVariables();

    protected :
        virtual void GetWaterTemperature();
        virtual void Loads();
        bool LoadRecordFilled, LoadRiverRecordFilled;
};

class _export TWaterTemperatureThreeDimensional : public TWaterTemperatureTwoDimensionalForRiaFormosa
{

	public :
		TWaterTemperatureThreeDimensional(TEcoDynClass* APEcoDynClass,
                char* className);
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        // AP, 2009.04.08
        virtual bool SaveVariables();
        virtual void Reinitialize();
    protected :
        virtual void GetWaterTemperature();
        virtual void GetRelativeHumidity(int i, int j);
        virtual void Loads();
};

#endif  // _PORT_FORTRAN_



#ifndef _PORT_FORTRAN_
static TEcoDynClass *PWaterTemperature;
#else

/* Functions that can be called from Fortran */
extern "C" {

    void watertemp_getvalues__(float* watertemperature, float* waterirradiance,
                float* netheatflux, float* latentheatflux,
                float* sensibleheatflux, float* relativehumidity);
	void watertemp_getwatertemperature__(float* watertemperature);
	void watertemp_getwaterirradiance__(float* waterirradiance);
	void watertemp_getnetheatflux__(float* netheatflux);
	void watertemp_getlatentheatflux__(float* latentheatflux);
	void watertemp_getsensibleheatflux__(float* sensibleheatflux);
	void watertemp_getrelativehumidity__(float* relativehumidity);
/*
 * This constructor calls Go() after initialisation
 */
    void watertemp_(int* box, float* depth,
                float* currenttime, float* julianday, float* airtemperature,
                float* salinity, float* waterdensity,
                float* totalir, float* subir,
                float* atmosphericir, float* speed, float* avtemperature,
                float* rivertemperature, float* humidity, float* timeStep);
    void watertemp_setdebug__(char* Debug);
	void watertemp_setdepth__(int* box, float* depth);
	void watertemp_setjulianday__(float* julianday);
	void watertemp_setcurrenttime__(float* currenttime);
	void watertemp_setairtemperature__(float* airtemperature);
	void watertemp_setsalinity__(float* salinity);
	void watertemp_setwaterdensity__(float* waterdensity);
	void watertemp_settidalheight__(float* height);
	void watertemp_settotalsurfaceirradiance__(float* totalir);
	void watertemp_setsubsurfaceirradiance__(float* subir);
	void watertemp_setatmosphericir__(float* atmosphericir);
	void watertemp_setwindspeed__(float* speed);
	void watertemp_setaveragewatertemperature__(int* box, float* avtemperature);
	void watertemp_setrivertemperature__(float* rivertemperature);
	void watertemp_setrelativehumidity__(float* humidity);

	void watertemp_go__();
}
#endif  // _PORT_FORTRAN_

#endif  // TWATOBJT_H



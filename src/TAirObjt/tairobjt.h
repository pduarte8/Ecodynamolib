// Air temperature object header file
// @ Pedro Duarte

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef TAIROBJT_H
#define TAIROBJT_H

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TAirTemperature : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
        TAirTemperature(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[]);
        TAirTemperature(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename);

		TAirTemperature(TEcoDynClass* APEcodynClass, char* className);
		~TAirTemperature();
        virtual void freeMemory();
		virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber, char* ParameterName,int AnObjectCode);
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.04
        virtual bool SaveVariables();

	protected :
		double MeanAirTemperature;

		virtual void GetAirTemperature();
        void PreBuildAirTemperature(char* className);
        void BuildAirTemperature(char* className);
};

class _export TMiraAirTemperature : public TAirTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TMiraAirTemperature(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[]);
        TMiraAirTemperature(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename);

		TMiraAirTemperature(TEcoDynClass* APEcodynClass, char* className);

	protected :
		virtual void GetAirTemperature();
};


class _export TAdriaticAirTemperature : public TAirTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TAdriaticAirTemperature(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[]);
        TAdriaticAirTemperature(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename);

		TAdriaticAirTemperature(TEcoDynClass* APEcodynClass, char* className);

	protected :
		virtual void GetAirTemperature();
};


class _export TRiaFormosaAirTemperature : public TAirTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TRiaFormosaAirTemperature(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aDailyAirTemperature[]);
        TRiaFormosaAirTemperature(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename);

        TRiaFormosaAirTemperature(TEcoDynClass* APEcodynClass, char* className);
        // AP, 2007.07.04
        virtual bool SaveVariables();

	protected :
        double DailyAirTemperature[365];
		virtual void GetAirTemperature();
        void BuildRiaFormosaAirTemperature();
};

class _export TAirTemperatureTimeSeries : public TAirTemperature
{
	public:
   	    TAirTemperatureTimeSeries(TEcoDynClass* APEcodynClass, char* className);
       	~TAirTemperatureTimeSeries();
        virtual void freeMemory();
        // AP, 2008.01.16
        virtual bool SaveVariables();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber, char* ParameterName,int AnObjectCode);
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        virtual bool SaveParameters();
    protected:
        int NumberOfMomentsForAirTemperatureTimeSeries, NumberOfDaysForAirTemperatureTimeSeries, NumberOfHoursForAirTemperatureTimeSeries,
            NumberOfSubDomains;
        SubDomain *MyAirTemperatureSubDomains;
        double *MyAirTemperatureInSubDomains,*AirTemperatureInBox;
        int MyDay,RecycleTimeSeries;
        void BuilAirTemperatureTimeSeries();
        int CalculateMoment();
        void GetAirTemperature();
};

class _export TAirTemperatureTimeSeries2 : public TAirTemperature
{
	public:
        TAirTemperatureTimeSeries2(TEcoDynClass* PEcoDynClass, char* className);
       	~TAirTemperatureTimeSeries2();
        virtual void ResetPointers();
        virtual void freeMemory();
        virtual void ResetValues();
        bool ReadTimeSeriesValues(int fileIndex);
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Go();
        virtual void ReadCurrentValues();
        virtual void ReinitializeValues();
        virtual void Reinitialize();
    protected:
        int MeanValuesSamplesNumber,indexFile, MyNumberOfBoxes, steps, AStep;
        double *mAirTemperature;
        int *MyBoxNumber;
        long *timeSeries;
        long StartTime, meanTimeDiff, myTimeSeriesStep, AMoment;
        VNA* TSVariableNameArray;
        bool firstRead, recursiveCall, FoundFirstStep;
        double *AirTemperatureInBox;
        int NumberOf2DBoxes;
};
//Este objecto é praticamente igual do
//TMiraAirTemperature, excepto a ordenada na origem
//da fórmula de cálculo da temp (de 10 passou a 12)

class _export TAtlanticoAirTemperature : public TAirTemperature
{
	public :
        // constructors invoked outside EcoDyn
        TAtlanticoAirTemperature(char* className, float timeStep,
            int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[]);
        TAtlanticoAirTemperature(char* className, float timeStep,
                char* morphologyFilename, char* variablesFilename);

		TAtlanticoAirTemperature(TEcoDynClass* APEcodynClass, char* className);

	protected :
		virtual void GetAirTemperature();
};


static TEcoDynClass *PAirTemperature;

#endif //TAIROBJT_H



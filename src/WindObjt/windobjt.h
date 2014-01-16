// Wind object header file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// REV A - 96.02.09
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef WINDOBJT_H
#define WINDOBJT_H

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TWind : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
        TWind(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[]);
        TWind(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename);

		TWind(TEcoDynClass* APEcodynClass, char* className);
		~TWind();
        virtual void freeMemory();
		virtual void Go();
        virtual void Integrate();
		virtual void CalculateWindSpeed();
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
		double Speed,
			   Direction;

		double *AverageWindSpeed,
			   *AverageWindSpeedX,
			   *AverageWindSpeedY;
        void PreBuildWind(char* className);
        void BuildWind(char* className);
};


class _export TWindDailyTimeSeries : public TWind
{
	public:
        // constructors invoked outside EcoDyn
        TWindDailyTimeSeries(char* className,
            float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aDailyWindSpeed[]);
        TWindDailyTimeSeries(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename);

   	    TWindDailyTimeSeries(TEcoDynClass* APEcodynClass, char* className);
       	~TWindDailyTimeSeries();
        virtual void freeMemory();
   	    virtual void CalculateWindSpeed();
        // AP, 2007.07.05
        virtual bool SaveVariables();

    protected:
   	    double *DailyWindSpeed;
        void BuildDailyWind();
};


class _export TWindTimeSeries : public TWindDailyTimeSeries
{
	public:
   	    TWindTimeSeries(TEcoDynClass* APEcodynClass, char* className);
       	~TWindTimeSeries();
        virtual void freeMemory();
   	    virtual void CalculateWindSpeed();
        // AP, 2008.01.16
        virtual bool SaveVariables();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        virtual bool SaveParameters();
        virtual int CalculateMoment();
    protected:
        int NumberOfMomentsForWindTimeSeries, NumberOfDaysForWindTimeSeries, NumberOfHoursForWindTimeSeries,
            NumberOfSubDomains;
        SubDomain *MyWindSubDomains;
        double *WindSpeed, *WindSpeedX, *WindSpeedY, *WindDirection, *MyWindSpeedInSubDomains, *MyWindDirectionInSubDomains;
        void BuildWindTimeSeries();
};

class _export TWindTimeSeriesV1 : public TWindTimeSeries
{
     public:
        TWindTimeSeriesV1(TEcoDynClass* APEcodynClass, char* className);
       	~TWindTimeSeriesV1();
        virtual void freeMemory();
        virtual int CalculateMoment();
    protected:
        void BuildWindTimeSeriesV1();
};

static TEcoDynClass *PWind;

#endif  //WINDOBJT_H



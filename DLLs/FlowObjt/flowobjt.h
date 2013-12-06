// Flow object header file
// @ J. Gomes Ferreira
// REV A - 96.03.11
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef FLOWOBJT_H
#define FLOWOBJT_H

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TFlow : public TEcoDynClass
{
	public :
        // constructors used outside EcoDyn
        TFlow(char* className, float timeStep, char* morphologyFilename, char* variablesFilename);
        TFlow(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow);

		TFlow(TEcoDynClass* PEcoDynClass, char* className);
		~TFlow();
        virtual void freeMemory();
		virtual void Go();

		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.06
        virtual bool SaveVariables();

	protected :
		double MainRiverFlow,
				 ModalFlow,
				 FlowAmplitude,
				 RandomFlow;

		virtual void CalculateFlow();
        void PreBuildFlow(char* className);
        void BuildFlow(char* className);
};

class _export TCarlFlow : public TFlow
{
	public :
        // constructors used outside EcoDyn
        TCarlFlow(char* className, float timeStep, char* morphologyFilename, char* variablesFilename);
        TCarlFlow(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow);

		TCarlFlow(TEcoDynClass* PEcoDynClass, char* className);
   protected :
		virtual void CalculateFlow();
};



class _export TOneDimensionalFlow : public TFlow
{
	public :
        // constructors used outside EcoDyn
        TOneDimensionalFlow(char* className, float timeStep, char* morphologyFilename, char* variablesFilename);
        TOneDimensionalFlow(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow);

		TOneDimensionalFlow(TEcoDynClass* PEcoDynClass, char* className);
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);

	protected :

	//Parameters
		virtual void CalculateFlow();
};

class _export TCrestumaLeverFlow : public TOneDimensionalFlow
{
	public :
        // constructors used outside EcoDyn
        TCrestumaLeverFlow(char* className, float timeStep, char* morphologyFilename, char* variablesFilename);
        TCrestumaLeverFlow(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double modalFlow,
                   double flowAmplitude, double randomFlow,
                   int numberOfDays, int firstDay,
                   double aDailyRiverFlow[], double aDailyDamDischarge[],
                   double aHourlyRiverFlow[], double aHourlyDamDischarge[]);
		TCrestumaLeverFlow(TEcoDynClass* PEcoDynClass, char* className);
        ~TCrestumaLeverFlow();
        virtual void freeMemory();
      virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.06
        virtual bool SaveVariables();

	protected :
		virtual void CalculateFlow();
        void BuildCrestumaLeverFlow();
        double *DailyRiverFlow, *DailyDamDischarge,
      		 *HourlyRiverFlow, *HourlyDamDischarge;
        double DamDischarge, HourlyMainRiverFlow, HourlyDischarge;
        int NumberOfDays, FirstDay;
};

/*class _export TRiaFormosaFlow : public TOneDimensionalFlow
{
	public :
        // constructors used outside EcoDyn
		TRiaFormosaFlow(TEcoDynClass* PEcoDynClass, char* className);
        ~TRiaFormosaFlow();
        void BuildingRiverLineCoordinateMatrix(int Number, int ABoundary, int AnInteger, int index);
        void BuildingRiverColumnCoordinateMatrix(int Number, int ABoundary, int AnInteger, int index);
        virtual void Inquiry(char* srcName, double &Value1, double &Value2, double &Value3, double &Value4,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
	protected :
		virtual void CalculateFlow();
        void BuildRiaFormosaFlow();
        int NumberOfRivers, NumberOfDays, FirstDay, GridLines, GridColumns;
        typedef struct Rivers
        {
            char RiverName[21];
            int NumberOfLineCoordinates, NumberOfColumnCoordinates;
            int *RiverLineCoordinates, *RiverColumnCoordinates;
            double *RiverFlow;
        }  RiverRecord;
        RiverRecord* ARiverRecord;
};     */

class _export TRiaFormosaFlow : public TOneDimensionalFlow
{
	public :
        // constructors used outside EcoDyn
		TRiaFormosaFlow(TEcoDynClass* PEcoDynClass, char* className);
        virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber, //int ABoundary,
										 char* ParameterName,
										 int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
	protected :
        void BuildRiaFormosaFlow();
		virtual void Go();
    int ADay, NumberOfRivers;
    RiversRecord *MyRiverRecord;
};

static TEcoDynClass *PFlow;


#endif //FLOWOBJT_H



// Tide object header file
// @ Pedro Duarte
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef TIDEOBJT_H
#define TIDEOBJT_H

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TTide : public TEcoDynClass
{
	public :
        // Constructors invoked outside EcoDyn environment...
        TTide(char* className, int nLines, int nColumns,
              char* variablesFilename, char* parametersFilename, char* tideFilename);
        TTide(char* className, int nLines, int nColumns,
             int nVars, VNA aVariablesNames[],
             int nParams, double startYear,
             double unit, double z0,
             double saAmplitude, double saPhase,
             double o1Amplitude, double o1Phase,
             double q1Amplitude, double q1Phase,
             double k1Amplitude, double k1Phase,
             double m2Amplitude, double m2Phase,
             double n2Amplitude, double n2Phase,
             double s2Amplitude, double s2Phase,
             double mN4Amplitude, double mN4Phase,
             double m4Amplitude, double m4Phase,
             double mS4Amplitude, double mS4Phase);

		TTide(TEcoDynClass* PEcoDynClass, char* className);
		~TTide();
        virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		virtual void Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode);
      virtual void Inquiry(char* srcName, double &ATidalHeight, double AYear, double AMonth,
										 double ADay, double AnHour);
      virtual void Go();
        // AP, 2007.05.29
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.02
        virtual void SaveTide(void* fHandle);
        virtual bool SaveVariables();
        virtual bool SaveParameters();

   protected :
        void BuildTide();
		double *BoxTidalHeight;
		char TideFileName[129];
		double TidalHeight, HarmonicsTimeLag;
		int StartYear;
};


class _export TDoubleTide : public TTide
{
	public :
		TDoubleTide(TEcoDynClass* PEcoDynClass, char* className);
		~TDoubleTide();
      virtual void Inquiry(char* srcName, double &ATidalHeight, double &OtherTidalHeight,
                               double AYear, double AMonth,
                               double ADay, double AnHour);
   protected :
		double OtherTidalHeight;
};


class _export TTideWithWanContinuousHarmonics : public TTide
{
	public :
        // Constructors invoked outside EcoDyn environment...
        TTideWithWanContinuousHarmonics(char* className, int nLines, int nColumns,
              char* variablesFilename, char* parametersFilename, char* tideFilename);
        TTideWithWanContinuousHarmonics(char* className, int nLines, int nColumns,
             int nVars, VNA aVariablesNames[],
             int nParams, double startYear,
             double unit, double z0,
             double saAmplitude, double saPhase,
             double o1Amplitude, double o1Phase,
             double q1Amplitude, double q1Phase,
             double k1Amplitude, double k1Phase,
             double m2Amplitude, double m2Phase,
             double n2Amplitude, double n2Phase,
             double s2Amplitude, double s2Phase,
             double mN4Amplitude, double mN4Phase,
             double m4Amplitude, double m4Phase,
             double mS4Amplitude, double mS4Phase);

		TTideWithWanContinuousHarmonics(TEcoDynClass* PEcoDynClass, char* className);
		~TTideWithWanContinuousHarmonics();
        virtual void freeMemory();
      virtual void Inquiry(char* srcName, double &ATidalHeight, double AYear, double AMonth,
										 double ADay, double AnHour, int AGridLine);
};


static TEcoDynClass *PTide;

#endif //TIDEOBJT_H

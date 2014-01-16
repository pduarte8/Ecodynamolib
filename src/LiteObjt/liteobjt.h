/*
  Name: Liteobjt.h
  Light object header file
  Copyright: P.Duarte
  Translated by J. Gomes Ferreira
  Rev. A 96.02.04
  Description: Light object header file
  Date: 18-11-04 (Antonio Pereira) - Code translated to be called by Fortran programs
*/
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef LITEOBJT_H
#define LITEOBJT_H

#include "EcoDynClass.h"

// DLL header file

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TLight : public TEcoDynClass
{
	public :
        // constructors invoked outside EcoDyn
#ifndef _PORT_FORTRAN_
        TLight(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
            int nVars, VNA aVarsNames[], double aLatitude[],
            int nParams, double modalCloud, double cloudAmplitude, double cloudPeak,
            double cloudPhase, double randomCloud, double cloudCover, double kValue);
        TLight(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename);

		TLight(TEcoDynClass* APEcoDynClass, char* className);
#else  // _PORT_FORTRAN_
/* to remove the next line in the final version (_new) renamed as original*/
        static TLight* getLight();

        static TLight* getLight(TLight* plight);
        TLight();
        void SetLatitude(int box, double latitude) {Latitude[box] = latitude;}
        void SetKValue(double kvalue) {KValue = kvalue;}
        void SetCloudCover(double cloudcover) {CloudCover = cloudcover;}
        void SetAirTemperature(double airtemperature) {AirTemperature = airtemperature;}

        char debug;
#endif  // _PORT_FORTRAN_
		~TLight();
        virtual void freeMemory();
        virtual double GetParameterValue(char* parmName);
        virtual bool SetParameterValue(char* parmName, double value);
        virtual void Go();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Inquiry(char* srcName, double &Value,double Value1,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Inquiry(char* srcName, double &Value,double ALight,double ADepth,double AExtinctionCoefficient,int BoxNumber,char* ParameterName,int AnObjectCode);

        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.05
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
		double  *AverageHorizontalLight,
                *AverageVerticalLight,
                *EuphoticZone,
                *SubSurfaceLight,
                *TotalSurfaceLight,
                *ParSurfaceLight,
                *AnyPar,
                *HoursOfSun,
                *PhoticDepth,
                *AtmosphericIR,
                *ExtinctionCoefficient,
                *Sunrise,
                *Sunset,
                *Latitude,
                *TotalLight,
                *DailyAverageLight,
                *TotalSurfaceLightTimeSeries;

        double   MeanWaterColumnLight,
                 LightToPAR,
                 IncidentLight,
                 SolarConstant,
                 Sigma,
                 KValue,
                 BoxDepth,
                 TidalHeight,
                 SeaAlbedo,
                 CloudCover,
                 ModalCloud,   //Nathalie Addition
                 CloudAmplitude,
                 CloudPeak,
                 CloudPhase,
                 RandomCloud,
                 CloudSpecificTime,
                 DoubleRandomCloud,
                 AirTemperature;

                 SubDomain *MyLightSubDomains;

        int NumberOfMomentsForTimeSeries, NumberOfDaysForTimeSeries, NumberOfHoursForTimeSeries, NumberOfSubDomains;

        void PreBuildLight(char* className);
        void BuildLight(char* className);
        virtual void GetLightAtSurface();
        virtual void GetLightAtSurface(double CurrentHour);
        //virtual void GetMeanWaterColumnRadiation(int ABoxNumber);
        virtual void GetHorizontalMeanWaterColumnRadiation();
        virtual void GetVerticalMeanWaterColumnRadiation();
        virtual void GetDaylightHours();
        virtual void GetPhoticDepth();
        //virtual void GetAverageLight();
        virtual void GetLightAtAnyDepth(int ABoxNumber,double ADepth);
        virtual void GetMeanSurfaceRadiation(double InitialTime,double FinalTime,int BoxNumber);
        virtual void GetAtmosphericInfraRed();
        virtual double GetAtmosphericInfraRed(int ABoxNumber);
        virtual double GetDeclination(int ADay);
        virtual double GetSunSetRiseHourAngle(double ALatitude,double ADeclinationAngle);
        virtual double GetHourAngle(double ATrueSolarTime);
        virtual double GetRadiusVector(int ADay);
        virtual double GetSolarAltitude(double ALatitude,double ADeclinationAngle,double AHourAngle);
        virtual double GetRadiationAtTopOfAtmosphere(double ASolarAltitude,double ARadiusVector);
        virtual double GetAtmosphericTransmission(double ASolarAltitude);
        virtual double GetRadiationAtSeaLevel( double ARadiationAtTopOfAtmosphere,double AnAtmosphericTransmission);
        virtual void GetCloudCover(); //NG 96.06.17
        virtual void GetLightAtAnyDepth(int ABoxNumber,double ALight,double ADepth,double AExtinctionCoefficient);


/* to remove the next lines in the final version (_new) renamed as original*/
#ifdef _PORT_FORTRAN_
    private:
        static TLight *PLight;
#endif  // _PORT_FORTRAN_
/* till here */
};

class _export TLightWithIce : public TLight
{
   public :
        TLightWithIce(TEcoDynClass* APEcoDynClass, char* className);
        ~TLightWithIce();
        virtual void freeMemory();
        virtual void Integrate();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
   protected :
        double *IceVisibleLightAtBottom, *IceVisibleLightAboveChlLayer, *IceNearInfraRedLightAtBottom, *IceNearInfraRedLightAboveChlLayer;
        double MyIceDepth, MySnowDepth, MyAlbedo, MyK_Ice, MyI0_Ice, MyIceAlgaeK, MyIceAlgaeLayerThickness,
               MyKforVisibleLightInIce, MyKforNearInfraRedLightInIce,
               MyI0forVisibleLightInIce, MyI0forNearInfraRedLightInIce,
               MyAlbedoForVisibleLightInIce, MyAlbedoForNearInfraredLightInIce,
               MyKforVisibleLightInSnow, MyAlbedoForVisibleLightInSnow;
        virtual void GetIceBottomLight();
        double TINNY;
};


#ifndef _PORT_FORTRAN_
static TEcoDynClass *PLight;
#else  // _PORT_FORTRAN_

/* Functions that can be called from Fortran */
extern "C" {

/* to remove the next lines in the final version (_new) renamed as original*/
    void light_getvalues__(float* totallight, float* parlight,
                float* hoursofsun, float* horizontallight, float* parhorizontallight,
                float* noonpar, float* photicdepth, float* subsurfacelight,
                float* parsubsurfacelight);
	void light_gettotalsurfaceirradiance__(float* totallight);
	void light_getparsurfaceirradiance__(float* parlight);
	void light_getdaylighthours__(float* hoursofsun);
	void light_getdepthintegratedirradiance__(float* horizontallight);
	void light_getdepthintegratedparirradiance__(float* parhorizontallight);
	void light_getmeanverticalwaterirradiance__(float* verticallight);
	void light_getmeanverticalparwaterirradiance__(float* parverticallight);
	void light_getnoonsurfacepar__(float* noonpar);
	void light_getphoticdepth__(float* photicdepth);
	void light_getsubsurfaceirradiance__(float* subsurfacelight);
	void light_getsubsurfaceirradiancepar__(float* parsubsurfacelight);
	void light_getatmosphericir__(float* atmosphericir);
/*
 * This constructor calls Go() after initialisation
 */
    void light1_(int* PLight, int* nc, int* nr, int* nz, float* pcb);
    void light_(int* box, float* latitude,
                float* currenttime, float* julianday, float* kvalue,
                float* depth, float* cloudcover, float* airTemperature);
    void light_setdebug__(char* Debug);
	void light_setlatitude__(int* box, float* latitude);
	void light_setdepth__(int* box, float* depth);
	void light_setjulianday__(float* julianday);
	void light_setcurrenttime__(float* currenttime);
	void light_setkvalue__(float* kvalue);
	void light_setcloudcover__(float* cloudcover);

	void light_go__();

/* till here -remove (_new) */


/*
 * The following are the real functions to include
 */
    void light_new__(int* plight, int* nc, int* nr, int* nz,
                float* latitude, float* kvalue, float* depth,
                float* sigma, float* cloudcover, float* airtemperat);
    void light_new_go__(int* plight, float* curtime, float* julianday);
    void light_new_getvalues__(int* plight, int* ic, int* ir, int* iz,
                float* totallight, float* parlight,
                float* parhorizontallight, float* hoursofsun,
                float* horizontallight, float* noonpar,
                float* photicdepth, float* subsurfacelight,
                float* parsubsurfacelight, float* atmosphericir);
}
#endif  // _PORT_FORTRAN_

#endif //LITEOBJT_H



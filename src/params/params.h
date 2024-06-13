#ifndef _PARAMS_H_
#define _PARAMS_H_

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

 #ifdef __BORLANDC__
	#include <windows.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <ctime>
	
	#ifndef BOOL
	#define BOOL bool
	#endif
	#ifndef TRUE
	#define TRUE true
	#endif
	#ifndef FALSE
	#define FALSE false
	#endif
	#define _export 
	
#endif  // __BORLANDC__
#include "params.rh"

// Params.h
// copyright (c) J. Gomes Ferreira, 1997
// REV 1 : 97.09.26

// Header file

class _export TPhysics
{
    public :
        TPhysics();
        ~TPhysics();
        double ParticleDensity, WaterSalinity, LocalGravity;
        double density(double salinity, double temperature);
        double viscosity(double salinity, double temperature);
        double fallvelocity(double grainsize, double temperature);
        double salt (double conductivity, double temperature);
     double gravity(double DegreesMinutes, double heightAboveSeaLevel);
};

static TPhysics* PPhysics;

class _export TChemistry
{
    public :
        TChemistry();
        ~TChemistry();

        virtual double OxygenSaturationConstants (double salinity,
                                                                double temperature,
                                                                double gaspercent,
                                                                BOOL ValueInMililiters = TRUE);
        virtual double NitrogenSaturationConstants (double salinity,
                                                                  double temperature,
                                                                  double gaspercent,
                                                                  BOOL ValueInMililiters = TRUE);
        virtual double ArgonSaturationConstants (double salinity,
                                                              double temperature,
                                                              double gaspercent,
                                                              BOOL ValueInMililiters = TRUE);
        virtual double NeonSaturationConstants (double salinity,
                                                             double temperature,
                                                             double gaspercent,
                                                             BOOL ValueInMililiters = TRUE);
        virtual double HeliumSaturationConstants (double salinity,
                                                                double temperature,
                                                                double gaspercent,
                                                                BOOL ValueInMililiters = TRUE);
    private :
        double Gas_A1, Gas_A2, Gas_A3, Gas_A4, Gas_B1, Gas_B2, Gas_B3;

        double GasConversionFactor;
        double VolumeCompensation;
        double ZeroCelsiusInKelvin;
        double MolarVolume;
        double MolarMass;

        virtual double gas (double salinity, double temperature);

};

static TChemistry* PChemistry;

class _export TAstronomy
{
    public :
        TAstronomy();
        ~TAstronomy();
        BOOL LoadDateStruct (int AYear, int AMonth, int ADay );
        BOOL IsDateValid( int AYear, int AMonth, int ADay ); // overloaded for external calls
        BOOL IsYearLeap( int AYear );
        int DayOfTheWeek( int AYear, int AMonth, int ADay, char* AWeekDay );
     int DaysOfTheMonth( int AYear,int AMonth);
        int DayOfTheYear( int AYear, int AMonth, int ADay );
        long DateToDayNumber( int AYear, int AMonth, int ADay );
        BOOL DayNumberToDate( long ADayNumber, int &AYear, int &AMonth, int &ADay );
        long DateDiff ( int AStartYear,
                             int AStartMonth,
                             int AStartDay,
                             int AnEndYear,
                             int AnEndMonth,
                             int AnEndDay );
        // moon stuff
        void GetMoonPhase(double AJulianDay, int APhase, int &AYear, int &AMonth, int &ADay);
        void GetMoonPhase(int TheYear, int TheMonth, int TheDay, int APhase,
                                int &AYear, int &AMonth, int &ADay);
        void GetNextMoonPhase(int TheYear, int TheMonth, int TheDay, int APhase,
                                     int &AYear, int &AMonth, int &ADay);
        int GetNextMoonPhase(int TheYear, int TheMonth, int TheDay, int APhaseNumber,
                                    int &APhase, int &AHour, int &AMinute, int &ASecond);
        int GetLunation(int AYear,int AMonth,int ADay);
     int GetLunation();

    private :
        // new moon stuff
        double EightPhases[8], EightDates[8][2]/*[2]*/, MyDay, LunationD;
     int Lunation;
        void CalculateMoonPhase();
        double MoonPhaseByLunation(int lun, int phi);
        double MoonPhase(double k, int phi);
        time_t JDtoDate(double jd, struct tm *event_date);
        // end of nms
        int DaysInMonth[13], DaysInMonthLeap[13];
        typedef char DN[20];
        DN DayName[7];
        typedef char MN[20];
        MN MonthName[13];
        struct tm Date;
        BOOL IsDateValid();
        long DateToDayNumber();

};

static TAstronomy* PAstronomy;

class _export TDEB
{
    public :
        TDEB();
        ~TDEB();
        double Feed(double JXm, double F, double StructuralVolume);
        double Absorption(double PAm, double F, double StructuralVolume);
        double StructuralVolumeChange(double PAm, double K, double Em, double Eg, double PT, double Respiration, double Reserves, double StructuralVolume);
        double GrowthAndMetabolicCosts(double PAm, double PM, double Em, double Eg, double PT, double Reserves, double StructuralVolume);
        double ReserveConsumption(double GrowthAndMetabolism, double MaturityCosts, double ReprodCosts);
        double Respire(double PM, double K, double Eg, double Reserves, double StructuralVolume);
        double BodyLength(double StructuralVolume, double AShapeCoefficient);
        double TemperatureLimitation(double ATA, double AT1, double AWaterTemperature);
        double Excrete();
        double Reproduce();
        void Debugger(double i);
        double TINNY;
    private :

};

class _export TPLightCurves
{
    public :
	TPLightCurves();
        ~TPLightCurves();
        double Platt(double PARtop, double KValue, double Depth, double Pmax, double beta, double slope);
        double Steele(double PARtop, double PARbottom, double KValue, double Depth, double Pmax, double PARopt);
        double EilersAndPeeters(double PARtop, double PARbottom, double KValue, double Depth, double a, double b, double c);
        double TINNY, Productivity;
        double Steps; 	
};

class _export TNutrientLimitation
{
    public :
        TNutrientLimitation();
        ~TNutrientLimitation();
	double ExternalLimitation();
	double InternalLimitation();
	double RedfieldMolarRatio[3];
        double TINNY;
};

class _export TNutrientUptake
{
    public :
        TNutrientUptake();
        ~TNutrientUptake();
        double NitrateUptake();
	double AmmoniaUptake();
	double PhosphateUptake();
	double SilicaUptake();
	double TINNY;

};



static TDEB* PDEB;
static TPLightCurves* PPLightCurves;
static TNutrientLimitation* PNutrientLimitation;
static TNutrientUptake* PNutrientUptake;

#endif //_PARAMS_H_



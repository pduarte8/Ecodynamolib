// Params.cpp
// copyright (c) J. Gomes Ferreira, 1997
// REV 1 : 97.09.26


/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

//General includes
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstdlib>
	#include <cstring>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "params.h"

// TPhysics?s methods

// Constructor

TPhysics::TPhysics()
{
	// Fall velocity initialisation
	ParticleDensity = 2600;
	WaterSalinity = 0; // Seawater is default
	LocalGravity = 9.78031; // m s-2
}

TPhysics::~TPhysics()
{
}

//Riley & Skirrow - Page 391
double TPhysics::density(double salinity, double temperature)   // Call Salinity in o/oo
                                                                // and Temperature in oC
{
	double At,Bt,Et, SigmaT, Sigma0, Cl;
	//Use either Cox or Knudsen
	if (salinity > 10)

		// Cox et al., 1970 - First coefficient is ten(-2) not ten(2) as Riley
		//						    indicates in the listed equation
		SigmaT = 8.009691 * pow(10,-2)
					+ 5.88194 * pow(10,-2) * temperature
					+ 0.7901864 * salinity
					- 8.114654 * pow(10,-3) * pow(temperature,2)
					- 3.253104 * pow(10,-3) * salinity * temperature
					+ 1.31708 * pow(10,-4) * pow(salinity,2)
					+ 4.76004 * pow(10,-5) * temperature * pow(temperature,2)
					+ 3.892875 * pow(10,-5) * salinity * pow(temperature,2)
					+ 2.879715 * pow(10,-6) * pow(salinity,2) * temperature
					- 6.118315 * pow(10,-8) * salinity * pow(salinity,2);

	else
	{
		// Knudsen - 1901
		Cl = salinity/1.80655;

		Sigma0 = -6.9 * pow(10,-2)
					+ 1.4708 * Cl
					- 1.570 * pow(10,-3) * pow(Cl,2)
					+ 3.98 * pow(10,-5) * Cl;

		At =   4.7867 * pow(10,-3) * temperature
			  - 9.8185 * pow(10,-5) * pow(temperature,2)
			  + 1.0843 * pow(10,-6) * temperature * pow(temperature,2);

		Bt =   1.803 * pow(10,-5) * temperature
			  - 8.146 * pow(10,-7) * pow(temperature,2)
			  + 1.667 * pow(10,-8) * temperature * pow(temperature,2);

		Et = -(pow((temperature-3.98),2) * (temperature +283)
			  /(503.57 * (temperature + 67.26)));

		SigmaT =   (Sigma0 + 0.1324)
					* (1 - At + Bt * (Sigma0 - 0.1324))
					+ Et;
	}

	return SigmaT + 1000; // kg m-3
}

//Riley & Skirrow - Page 576 - in centipoise
double TPhysics::viscosity(double salinity, double temperature)   // Call Salinity in o/oo
																  // and Temperature in oC
{
	const double A5 = 0.000366;
	const double A25 = 0.001403;
	const double B5 = 0.002756;
	const double B25 = 0.003416;

	double viscosity_pureH20, A_t, B_t, A_slope, B_slope,
			 A_intercept, B_intercept, Cl_vol;

	// Calculate viscosity of pure water for specified temperature
	viscosity_pureH20 = 1.0020 * pow( 10, (1.1709 * (20 - temperature)
										   - 0.001827 * pow( (temperature - 20),2 ))
							               / (temperature + 89.93)); // in the pascal code (and EcoWin) its 89.92 - wrong)

	// Calculate volume chlorinity = chlorinity * density
	Cl_vol = (salinity/1.80655) * density(salinity,temperature)/1000;

	// Linear interpolation/extrapolation to find A_t and B_t
	A_slope = (A25-A5)/20;
	B_slope = (B25-B5)/20;
	A_intercept = A5 - A_slope * 5;
	B_intercept = B5 - B_slope * 5;
	A_t = A_slope * temperature + A_intercept;
	B_t = B_slope * temperature + B_intercept;

	// Calculate viscosity
	return viscosity_pureH20
			 * (1+ A_t * sqrt(Cl_vol) + B_t * Cl_vol);
}

double TPhysics::fallvelocity(double grainsize, double temperature) // Call grain size in mm
																	// and temperature in oC
{
	const int two = 2;
	const int ninths = 9;

	// Stokes equation - results in cm s-1

	return (
			  two
			  * LocalGravity
			  * (ParticleDensity - density(WaterSalinity, temperature))
			  * pow(pow(10,grainsize * -log10(2)) * 0.001/2,2)
			  / (ninths * viscosity(WaterSalinity, temperature) / 1000) ); // m s-1
}

//Riley & Skirrow - Page 391
double TPhysics::salt(double conductivity, double temperature)  //call conductivity in mmhos cm-1
																// temperature in oC
{
	const double seawater_conductivity = 42.896; // 35 o/oo, 15oC
	double R_t, R15, SaltValue;
	// Formula below is for conductivity ratio, but is passed specific:
	// conductivity by the call. Cox et al., 1967, UNESCO, 1968

	// calculate R15
	R_t = conductivity/seawater_conductivity;
	R15 = R_t + R_t * (R_t-1) * (temperature - 15)
			* (96.7 - 72 * R_t + 37.3 * pow(R_t,2)
				- (0.63 + 0.21 * pow(R_t,2)) * (temperature - 15))
			* pow(10,-5);

	SaltValue = - 0.08996
					+ 28.2970 * R15
					+ 12.80832 * pow(R15,2)
					- 10.67869 * pow(R15,3)
					+ 5.98624 * pow(R15,4)
					- 1.32311 * pow(R15,5);
	if (SaltValue < 0) return 0; else return SaltValue;
}

// R.M. Tennent - Science data book - Page 80
double TPhysics::gravity(double DegreesMinutes, double heightAboveSeaLevel)   // Call DEGMIN e.g.38º44' (3844)
																										// and height above sea level in m
{
//	const double polar_latitude = 9000;
	const double polar_latitude = 90;

	// gravity = 9.80665;                                   // "Default" gravity
	if (DegreesMinutes < 0)
        DegreesMinutes = -DegreesMinutes;              // for negative latitudes
	if (heightAboveSeaLevel < 0)
        heightAboveSeaLevel = 0;   // normalize at sea-level
	if (DegreesMinutes <= polar_latitude)
	{
/*
		latitude = floor(DegreesMinutes/100);              // Latitude in degrees
		latitude = latitude                                // and minutes as decimal
					  + ((DegreesMinutes/100)-latitude)*100/60;
*/
		double latitude = DegreesMinutes;
		return  9.80616 - 0.025928 * cos(2 * latitude * M_PI/180) // m s-2
				  + 0.000069 * pow(cos (2 * latitude * M_PI/180),2)
				  - 0.000003 * heightAboveSeaLevel;
	}
    return 9.80616;
}

// Tchemistry´s methods

// Constructor

TChemistry::TChemistry()
{
	MolarVolume = 22.4; // liters at STP
	ZeroCelsiusInKelvin = 273.15;
	VolumeCompensation = MolarVolume             // liters for 1 mole at
								/ZeroCelsiusInKelvin;   // STP/0 degrees celsius
}

TChemistry::~TChemistry()
{
}

double TChemistry::gas(double salinity, double temperature)
{
	const double kelvin = 273.15;
	// 120 - VERY excessive limit for boiling water
	if (temperature < 120) temperature = temperature + kelvin; // Sent in Celsius
	return exp (Gas_A1
					+ Gas_A2 * (100/temperature)
					+ Gas_A3 * log(temperature/100)
					+ Gas_A4 * (temperature/100)
					+ salinity * (Gas_B1
									+ Gas_B2 * (temperature/100)
									+ Gas_B3 * pow(temperature/100,2)));
}

double TChemistry::OxygenSaturationConstants (double salinity,
															 double temperature,
															 double gaspercent,
															 BOOL ValueInMililiters)
{
	// From Kester, p. 506, Riley and Skirrow,
	// Marine chemistry - values in ml l-1 (bracketed values)

	Gas_A1 = -173.4292;
	Gas_A2 = 249.6339;
	Gas_A3 = 143.3483;
	Gas_A4 = -21.8492;
	Gas_B1 = -0.033096;
	Gas_B2 = 0.014259;
	Gas_B3 = -0.0017000;

	MolarMass = 15.9994 * 2; // One O2 molecule weighs 15.9994 * 2 g

	if (ValueInMililiters)
		GasConversionFactor = 1;
	else
		GasConversionFactor = MolarMass
									 /((temperature+ZeroCelsiusInKelvin)*VolumeCompensation);

	if (gaspercent != -1) return gaspercent * 100
										  / ( gas(salinity, temperature) * GasConversionFactor);
	else return gas(salinity, temperature)
					* GasConversionFactor;
}

double TChemistry::NitrogenSaturationConstants (double salinity,
																double temperature,
																double gaspercent,
																BOOL ValueInMililiters)
{
	// From Kester, p. 506, Riley and Skirrow,
	// Marine chemistry - values in ml l-1 (bracketed values)

	Gas_A1 = -172.4965;
	Gas_A2 = 248.4262;
	Gas_A3 = 143.0738;
	Gas_A4 = -21.7120;
	Gas_B1 = -0.049781;
	Gas_B2 = 0.025018;
	Gas_B3 = -0.0034861;

	MolarMass = 14.0067 * 2; // One N2 molecule weighs 14.0067 * 2 g

	if (ValueInMililiters)
		GasConversionFactor = 1;
	else
		GasConversionFactor = MolarMass
									 /((temperature+ZeroCelsiusInKelvin)*VolumeCompensation);

   if (gaspercent != -1) return gaspercent * 100
										  / ( gas(salinity, temperature) * GasConversionFactor);
	else return gas(salinity, temperature)
					* GasConversionFactor;
}

double TChemistry::ArgonSaturationConstants (double salinity,
															double temperature,
															double gaspercent,
															BOOL ValueInMililiters)
{
	// From Kester, p. 506, Riley and Skirrow,
	// Marine chemistry - values in ml l-1 (bracketed values)

	Gas_A1 = -173.5146;
	Gas_A2 = 245.4510;
	Gas_A3 = 141.8222;
	Gas_A4 = -21.8020;
	Gas_B1 = -0.034474;
	Gas_B2 = 0.014934;
	Gas_B3 = 0.0017729;

	MolarMass = 39.948; // One Ar molecule weighs 39.948 g

	if (ValueInMililiters)
		GasConversionFactor = 1;
	else
		GasConversionFactor = MolarMass
									 /((temperature+ZeroCelsiusInKelvin)*VolumeCompensation);

   if (gaspercent != -1) return gaspercent * 100
										  / ( gas(salinity, temperature) * GasConversionFactor);
	else return gas(salinity, temperature)
					* GasConversionFactor;
}

double TChemistry::NeonSaturationConstants (double salinity,
														  double temperature,
														  double gaspercent,
														  BOOL ValueInMililiters)
{
	// From Kester, p. 506, Riley and Skirrow,
	// Marine chemistry - values in ml l-1 (bracketed values)

	Gas_A1 = -160.2630;
	Gas_A2 = 211.0969;
	Gas_A3 = 132.1657;
	Gas_A4 = -21.3165;
	Gas_B1 = -0.122883;
	Gas_B2 = 0.077055;
	Gas_B3 = -0.0125568;

	MolarMass = 20.179; // One Ne molecule weighs 20.179 g

	if (ValueInMililiters)
		GasConversionFactor = 1;
	else
		GasConversionFactor = MolarMass
									 /((temperature+ZeroCelsiusInKelvin)*VolumeCompensation);

	if (gaspercent != -1) return gaspercent * 100
										  / ( gas(salinity, temperature) * GasConversionFactor);
	else return gas(salinity, temperature)
					* GasConversionFactor;
}

double TChemistry::HeliumSaturationConstants (double salinity,
															 double temperature,
															 double gaspercent,
															 BOOL ValueInMililiters)
{
	Gas_A1 = -152.9405;
	Gas_A2 = 196.8840;
	Gas_A3 = 126.8015;
	Gas_A4 = -20.6767;
	Gas_B1 = -0.040543;
	Gas_B2 = 0.021315;
	Gas_B3 = 0.0030732;

	MolarMass = 4.00260; // One He molecule weighs 4.00260 g

	if (ValueInMililiters)
		GasConversionFactor = 1;
	else
		GasConversionFactor = MolarMass
									 /((temperature+ZeroCelsiusInKelvin)*VolumeCompensation);

	if (gaspercent != -1) return gaspercent * 100
										  / ( gas(salinity, temperature) * GasConversionFactor);
	else return gas(salinity, temperature)
					* GasConversionFactor;

}

// TAstronomy´s methods - built 97.10.27 (start and finish) JGF

// Constructor

TAstronomy::TAstronomy()
{
	// Days in month
	DaysInMonth[0]  = 0;
	DaysInMonth[1]  = 31;  DaysInMonth[2]  = 28;  DaysInMonth[3]  = 31;
	DaysInMonth[4]  = 30;  DaysInMonth[5]  = 31;  DaysInMonth[6]  = 30;
	DaysInMonth[7]  = 31;  DaysInMonth[8]  = 31;  DaysInMonth[9]  = 30;
	DaysInMonth[10] = 31;  DaysInMonth[11] = 30;  DaysInMonth[12] = 31;

	// Days in month leap
	for ( int i = 0; i < 13; i++)
    	DaysInMonthLeap[i] = DaysInMonth[i];
	DaysInMonthLeap[2] = 29;

	// Initialise weekdays
	strcpy(DayName[0],"Sunday"); strcpy(DayName[1],"Monday"); strcpy(DayName[2],"Tuesday");
	strcpy(DayName[3],"Wednesday"); strcpy(DayName[4],"Thursday"); strcpy(DayName[5],"Friday");
	strcpy(DayName[6],"Saturday");
	// Initialise months
	strcpy(MonthName[0],"");
	strcpy(MonthName[1],"January"); strcpy(MonthName[2],"February"); strcpy(MonthName[3],"March");
	strcpy(MonthName[4],"April"); strcpy(MonthName[5],"May"); strcpy(MonthName[6],"June");
	strcpy(MonthName[7],"July"); strcpy(MonthName[8],"August"); strcpy(MonthName[9],"September");
	strcpy(MonthName[10],"October"); strcpy(MonthName[11],"November"); strcpy(MonthName[12],"December");
}

TAstronomy::~TAstronomy()
{
}

// date functions from Binstock & REx, Practical algorithms for programmers, p.367
// longer ones by ftp from ftp.awl.com

BOOL TAstronomy::LoadDateStruct (int AYear, int AMonth, int ADay )
{
	Date.tm_year = AYear;
	Date.tm_mon = AMonth -1; // for compatibility with ANSI C
	Date.tm_mday = ADay;
	if (!IsDateValid())
		return FALSE;
	return TRUE;
}

BOOL TAstronomy::IsDateValid()
{
	if ( Date.tm_mon < 0 || Date.tm_mon > 11 ) // ANSI zero-based months
		return FALSE;
	if ( Date.tm_year < 1 )
		return FALSE;
	if ( !IsYearLeap ( Date.tm_year )) {
		if ( Date.tm_mday < 1 || Date.tm_mday > DaysInMonth[ Date.tm_mon + 1])
			return FALSE;
    }
	else {
		if ( Date.tm_mday < 1 || Date.tm_mday > DaysInMonthLeap[ Date.tm_mon + 1])
			return FALSE;
    }
    return TRUE;
}

BOOL TAstronomy::IsDateValid( int AYear, int AMonth, int ADay )
{
	TAstronomy::LoadDateStruct ( AYear, AMonth, ADay );
	if (TAstronomy::IsDateValid())
		return TRUE;
	else
		return FALSE;
}

BOOL TAstronomy::IsYearLeap( int AYear )
{
	int MyYear = AYear;
	if ( MyYear % 4 != 0 ) // divisible by 4? No, not leap
		return FALSE;

	if ( MyYear < 1582 ) // All years divisible by 4 prior to 1582 were leap
		return TRUE;

	if ( MyYear % 100 != 0 ) // divisible by 4 but not by 100? Leap year
		return TRUE;

	if ( MyYear % 400 != 0 ) // divisible by 100 but not by 400? Not leap year
		return FALSE;
	else
		return TRUE;
}

int TAstronomy::DayOfTheWeek( int AYear, int AMonth, int ADay, char* AWeekDay )
{
	TAstronomy::LoadDateStruct ( AYear, AMonth, ADay );
	if (!TAstronomy::IsDateValid()) // -1 if invalid date
		return -1;

	long MyDayNumber = TAstronomy::DateToDayNumber();

	if ( MyDayNumber < 0 )
		return -1;

	int MyWeekDay = ( ( int) (( MyDayNumber % 7 ) + 5 ) % 7 );
	strcpy(AWeekDay, DayName[MyWeekDay]);
	return MyWeekDay;
}

int TAstronomy::DaysOfTheMonth( int AYear, int AMonth)   //Acrescentada pelo Pedro a 3/2/98
{
	if (IsYearLeap(AYear))
   	    return DaysInMonthLeap[AMonth];
    else
		return DaysInMonth[AMonth];
}

int TAstronomy::DayOfTheYear( int AYear, int AMonth, int ADay )
{
	TAstronomy::LoadDateStruct ( AYear, AMonth, ADay );
	if (!TAstronomy::IsDateValid()) // -1 if invalid date
		return -1;

	long MyDateDiff = DateToDayNumber () - DateToDayNumber ( AYear, 1, 1 );

	return ((int) ( MyDateDiff + 1 ));

}

long TAstronomy::DateDiff ( int AStartYear,
									 int AStartMonth,
									 int AStartDay,
									 int AnEndYear,
									 int AnEndMonth,
									 int AnEndDay )
{
	TAstronomy::LoadDateStruct ( AnEndYear, AnEndMonth, AnEndDay );
	if (!TAstronomy::IsDateValid()) // -1 if invalid date
		return -1;

	return DateToDayNumber () - DateToDayNumber ( AStartYear, AStartMonth, AStartDay );
}

long TAstronomy::DateToDayNumber( int AYear, int AMonth, int ADay )
{
	TAstronomy::LoadDateStruct ( AYear, AMonth, ADay );
	if (!TAstronomy::IsDateValid()) // -1 if invalid date
		return -1;
	return TAstronomy::DateToDayNumber();
}

long TAstronomy::DateToDayNumber()
{
	long DayNumber;
	int  mm, yy;

	yy = Date.tm_year;
	mm = Date.tm_mon + 1;

	// get all the regular days in preceding years

	DayNumber =  ( yy - 1 ) * 365L;

	// now get the leap years

	DayNumber += ( yy - 1 ) / 4L;

	// now back out the century years that are not leap:
	// this would be all century years that are
	// not evenly divisible by 400: 1700, 1800, 1900, 2100...

	DayNumber -= ( yy - 1 ) / 100L;
	DayNumber += ( yy - 1 ) / 400L;

	// before 1582 all century years were leap, so adjust for
	// this. If year is > 1582, then just add 12 days for years
	// 100, 200, 300, 500, 600, 700, 900, 1000, 1100, 1300, 1400
	// and 1500. Otherwise, calculate it.

	if ( yy - 1 > 1582L )
		DayNumber += 12L;
	else
	{
		DayNumber += ( yy - 1 ) / 100L;
		DayNumber -= ( yy - 1 ) / 400L;
	}

	// now, add the days elapsed in the year so far */

	if ( !IsYearLeap ( Date.tm_year ) )
		while ( --mm )
			DayNumber += DaysInMonth[ mm ];
	else
		while ( --mm )
			DayNumber += DaysInMonthLeap[ mm ];

	// add days in current month for the year being evaluated */

	DayNumber += Date.tm_mday;

	// now adjust for the 10 days cut out of the calendar when
	// the change was made to the Gregorian calendar. This change
	// reflects the jump from October 4 to October 15, 1582, a
	// deletion of 10 days.

	if ( DayNumber > 577737L )
		DayNumber -= 10L;

	return ( DayNumber );
}

BOOL TAstronomy::DayNumberToDate( long ADayNumber, int &AYear, int &AMonth, int &ADay )
{
	long DaysLeft;

	if ( ADayNumber > 577737L )
		ADayNumber += 10L;

	AYear = (int) ( ADayNumber / 365 );
	DaysLeft = ADayNumber % 365L;

	// prior to 1700, all years evenly divisible by 4 are leap
	// Can't understand this, thought it was 1582 Europe or 1752 U.S. p. 391 B&R

	if ( AYear < 1700 )
		DaysLeft -= ( AYear / 4 );
	else
	{
		DaysLeft -= ( AYear / 4 );    // deduct leap years
		DaysLeft += ( AYear / 100 );  // add in century years
		DaysLeft -= ( AYear / 400 );  // deduct years / 400
		DaysLeft -= 12;            	// deduct century years that
												//	were leap before 1700
	}

	// make sure DaysLeft is > 0

	while ( DaysLeft <= 0 )
	{
		if ( IsYearLeap ( AYear ) )
		{
			AYear -= 1;
			DaysLeft += 366;
		}
		else
		{
			AYear -= 1;
			DaysLeft += 365;
		}
	}

	// AYear holds the number of elapsed years.
	// So, add 1 for the current year.

	AYear += 1;

	// now deduct the days in each month, starting
	// from January to find month and day of month.
	// adjust for leap year, of course.

	ADay = (int) DaysLeft;
	AMonth = 0;

	if ( IsYearLeap ( AYear ) )
		for ( int i = 1; i <= 12; i++ )
		{
			AMonth = i;
			if ( ADay <= DaysInMonthLeap[i] )
				break;
			else
				ADay -= DaysInMonthLeap[i];
		}
	else
	for ( int i = 1; i <= 12; i++ )
	{
		AMonth = i;
		if ( ADay <= DaysInMonth[i] )
			break;
		else
			ADay -= DaysInMonth[i];
	}

	if ( !IsDateValid (  AYear, AMonth, ADay ))
	{
		AYear = -1; AMonth = -1; ADay = -1;
		return FALSE;
	}
	else
		return TRUE;
}

// Moon phases

double TAstronomy::MoonPhaseByLunation(int lun, int phi)
{
	double k;
	k = lun + phi / 4.0;
	return MoonPhase(k, phi);
}

double TAstronomy::MoonPhase(double k, int phi)
{
	int i;                       // iterator to be named later.  Every program needs an i
	double T;                    // time parameter, Julian Centuries since J2000
	double JDE;                  // Julian Ephemeris Day of phase event
	double E;                    // Eccentricity anomaly
	double M;                    // Sun's mean anomaly
	double M1;                   // Moon's mean anomaly
	double F;                    // Moon's argument of latitude
	double O;                    // Moon's longitude of ascenfing node
	double A[15];                // planetary arguments
	double W;                    // added correction for quarter phases

	T = k / 1236.85;             // (47.3)

	// this is the first approximation.  all else is for style points!
	JDE = 2451550.09765 + (29.530588853 * k)    // (47.1)
			+ T * T * (0.0001337 + T * (-0.000000150 + 0.00000000073 * T));

	// these are correction parameters used below
	E = 1.0                                     // (45.6)
		 + T * (-0.002516 + -0.0000074 * T);
	M = 2.5534 + 29.10535669 * k                // (47.4)
		 + T * T * (-0.0000218 + -0.00000011 * T);
	M1 = 201.5643 + 385.81693528 * k            // (47.5)
		  + T * T * (0.0107438 + T * (0.00001239 + -0.000000058 * T));
	F = 160.7108 + 390.67050274 * k             // (47.6)
		 + T * T * (-0.0016341 * T * (-0.00000227 + 0.000000011 * T));
	O = 124.7746 - 1.56375580 * k               // (47.7)
		 + T * T * (0.0020691 + 0.00000215 * T);

	// planetary arguments
	A[0]  = 0; // unused!
	A[1]  = 299.77 +  0.107408 * k - 0.009173 * T * T;
	A[2]  = 251.88 +  0.016321 * k;
	A[3]  = 251.83 + 26.651886 * k;
	A[4]  = 349.42 + 36.412478 * k;
	A[5]  =  84.66 + 18.206239 * k;
	A[6]  = 141.74 + 53.303771 * k;
	A[7]  = 207.14 +  2.453732 * k;
	A[8]  = 154.84 +  7.306860 * k;
	A[9]  =  34.52 + 27.261239 * k;
	A[10] = 207.19 +  0.121824 * k;
	A[11] = 291.34 +  1.844379 * k;
	A[12] = 161.72 + 24.198154 * k;
	A[13] = 239.56 + 25.513099 * k;
	A[14] = 331.55 +  3.592518 * k;

	// all of the above must be made into radians!!! except for E...

	M = torad(M);
	M1 = torad(M1);
	F = torad(F);
	O = torad(O);

	// all those planetary arguments, too!
	for (i=1; i<=14; i++)
		A[i] = torad(A[i]);

	// ok, we have all the parameters, let's apply them to the JDE.
	// (remember the JDE?  this is a program about the JDE...)

	switch(phi)
	{
		// a special case for each different phase.  NOTE!, I'm not treating these in a 0123 order!

		case NEWMOON: // New Moon
			JDE = JDE
					- 0.40720         * sin (M1)
					+ 0.17241 * E     * sin (M)
					+ 0.01608         * sin (2.0 * M1)
					+ 0.01039         * sin (2.0 * F)
					+ 0.00739 * E     * sin (M1 - M)
					- 0.00514 * E     * sin (M1 + M)
					+ 0.00208 * E * E * sin (2.0 * M)
					- 0.00111         * sin (M1 - 2.0 * F)
					- 0.00057         * sin (M1 + 2.0 * F)
					+ 0.00056 * E     * sin (2.0 * M1 + M)
					- 0.00042         * sin (3.0 * M1)
					+ 0.00042 * E     * sin (M + 2.0 * F)
					+ 0.00038 * E     * sin (M - 2.0 * F)
					- 0.00024 * E     * sin (2.0 * M1 - M)
					- 0.00017         * sin (O)
					- 0.00007         * sin (M1 + 2.0 * M)
					+ 0.00004         * sin (2.0 * M1 - 2.0 * F)
					+ 0.00004         * sin (3.0 * M)
					+ 0.00003         * sin (M1 + M - 2.0 * F)
					+ 0.00003         * sin (2.0 * M1 + 2.0 * F)
					- 0.00003         * sin (M1 + M + 2.0 * F)
					+ 0.00003         * sin (M1 - M + 2.0 * F)
					- 0.00002         * sin (M1 - M - 2.0 * F)
					- 0.00002         * sin (3.0 * M1 + M)
					+ 0.00002         * sin (4.0 * M1);

			break;

		case FULLMOON: // Full Moon
			JDE = JDE
					- 0.40614         * sin (M1)
					+ 0.17302 * E     * sin (M)
					+ 0.01614         * sin (2.0 * M1)
					+ 0.01043         * sin (2.0 * F)
					+ 0.00734 * E     * sin (M1 - M)
					- 0.00515 * E     * sin (M1 + M)
					+ 0.00209 * E * E * sin (2.0 * M)
					- 0.00111         * sin (M1 - 2.0 * F)
					- 0.00057         * sin (M1 + 2.0 * F)
					+ 0.00056 * E     * sin (2.0 * M1 + M)
					- 0.00042         * sin (3.0 * M1)
					+ 0.00042 * E     * sin (M + 2.0 * F)
					+ 0.00038 * E     * sin (M - 2.0 * F)
					- 0.00024 * E     * sin (2.0 * M1 - M)
					- 0.00017         * sin (O)
					- 0.00007         * sin (M1 + 2.0 * M)
					+ 0.00004         * sin (2.0 * M1 - 2.0 * F)
					+ 0.00004         * sin (3.0 * M)
					+ 0.00003         * sin (M1 + M - 2.0 * F)
					+ 0.00003         * sin (2.0 * M1 + 2.0 * F)
					- 0.00003         * sin (M1 + M + 2.0 * F)
					+ 0.00003         * sin (M1 - M + 2.0 * F)
					- 0.00002         * sin (M1 - M - 2.0 * F)
					- 0.00002         * sin (3.0 * M1 + M)
					+ 0.00002         * sin (4.0 * M1);

			break;

		case CRESCENTMOON: // First Quarter
		case GIBBOUSMOON: // Last Quarter
			JDE = JDE
					- 0.62801         * sin (M1)
					+ 0.17172 * E     * sin (M)
					- 0.01183 * E     * sin (M1 + M)
					+ 0.00862         * sin (2.0 * M1)
					+ 0.00804         * sin (2.0 * F)
					+ 0.00454 * E     * sin (M1 - M)
					+ 0.00204 * E * E * sin (2.0 * M)
					- 0.00180         * sin (M1 - 2.0 * F)
					- 0.00070         * sin (M1 + 2.0 * F)
					- 0.00040         * sin (3.0 * M1)
					- 0.00034 * E     * sin (2.0 * M1 - M)
					+ 0.00032 * E     * sin (M + 2.0 * F)
					+ 0.00032 * E     * sin (M - 2.0 * F)
					- 0.00028 * E * E * sin (M1 + 2.0 * M)
					+ 0.00027 * E     * sin (2.0 * M1 + M)
					- 0.00017         * sin (O)
					- 0.00005         * sin (M1 - M - 2.0 * F)
					+ 0.00004         * sin (2.0 * M1 + 2.0 * F)
					- 0.00004         * sin (M1 + M + 2.0 * F)
					+ 0.00004         * sin (M1 - 2.0 * M)
					+ 0.00003         * sin (M1 + M - 2.0 * F)
					+ 0.00003         * sin (3.0 * M)
					+ 0.00002         * sin (2.0 * M1 - 2.0 * F)
					+ 0.00002         * sin (M1 - M + 2.0 * F)
					- 0.00002         * sin (3.0 * M1 + M);

			W = 0.00306
				 - 0.00038 * E * cos(M)
				 + 0.00026 * cos(M1)
				 - 0.00002 * cos(M1 - M)
				 + 0.00002 * cos(M1 + M)
				 + 0.00002 * cos(2.0 * F);
			if (phi == 3)
				W = -W;
			JDE += W;

			break;

		default: // oops!
#ifdef __BORLANDC__
			::MessageBox(0, "The Moon has exploded!", " Params DLL", MB_OK);
#else  // __BORLANDC__
			cerr << "Params DLL: The Moon has exploded!" << endl;
#endif  // __BORLANDC__
			// exit(1);
			break; // unexecuted code
	}
	// now there are some final correction to everything
	JDE = JDE
			+ 0.000325 * sin(A[1])
			+ 0.000165 * sin(A[2])
			+ 0.000164 * sin(A[3])
			+ 0.000126 * sin(A[4])
			+ 0.000110 * sin(A[5])
			+ 0.000062 * sin(A[6])
			+ 0.000060 * sin(A[7])
			+ 0.000056 * sin(A[8])
			+ 0.000047 * sin(A[9])
			+ 0.000042 * sin(A[10])
			+ 0.000040 * sin(A[11])
			+ 0.000037 * sin(A[12])
			+ 0.000035 * sin(A[13])
			+ 0.000023 * sin(A[14]);

	return JDE;
}

time_t TAstronomy::JDtoDate(double jd, struct tm *event_date)
/* convert a Julian Date to a date-time group */
{
  long a, a1, z, b, c, d, e;
  double f, day;

  jd += 0.5;
  z = (long) jd;
  f = jd - z;

  if (z < 2299161)
  {
	 a = z;
  }
  else
  {
	 a1 = (long) ((z - 1867216.25) / 36524.25);
	 a = z + 1 + a1 - (long)(a1 / 4);
  }

  b = a + 1524;
  c = (long)((b - 122.1) / 365.25);
  d = (long)(365.25 * c);
  e = (long)((b - d)/30.6001);

  day = b - d - (long)(30.6001 * e) + f;

  if (e < 14)
  {
	 event_date->tm_mon = (e - 1) - 1;
  }
  else
  {
	 event_date->tm_mon = (e - 13) - 1;
  }

  if (event_date->tm_mon > (2 - 1))
  {
	 event_date->tm_year = c - 4716 - 1900;
  }
  else
  {
	 event_date->tm_year = c - 4715 - 1900;
  }
  event_date->tm_mday = (int)day;
  day -= event_date->tm_mday;
  day *= 24;
  event_date->tm_hour = (int)day;
  day -= event_date->tm_hour;
  day *= 60;
  event_date->tm_min = (int)day;
  day -= event_date->tm_min;
  day *= 60;
  event_date->tm_sec = (int)day;

  event_date->tm_isdst = 0;

  return mktime(event_date);
}

void TAstronomy::GetMoonPhase(double AJulianDay,
										int APhase,
										int &AYear,
										int &AMonth,
										int &ADay)
{
	MyDay = AJulianDay;
	CalculateMoonPhase();
	//	struct tm event_date[4]; // storage for the final result
	tm EventDate; // storage for the final result
	JDtoDate(EightPhases[ APhase ], &EventDate);
	AYear = EventDate.tm_year + BASEYEAR;
	AMonth = EventDate.tm_mon + 1; // because tm_mon is zero based
	ADay = EventDate.tm_mday;
}

void TAstronomy::GetMoonPhase(int TheYear,
										int TheMonth,
										int TheDay,
										int APhase,
										int &AYear,
										int &AMonth,
										int &ADay)
{
	MyDay = DateToDayNumber(TheYear, TheMonth, TheDay);
	GetLunation();
	CalculateMoonPhase();
	tm EventDate; // storage for the final result
	JDtoDate(EightPhases[ APhase ], &EventDate);
	AYear = EventDate.tm_year + BASEYEAR;
	AMonth = EventDate.tm_mon + 1; // because tm_mon is zero based
	ADay = EventDate.tm_mday;
}

void TAstronomy::GetNextMoonPhase(int TheYear,
											 int TheMonth,
											 int TheDay,
											 int APhase,
											 int &AYear,
											 int &AMonth,
											 int &ADay)
{
	MyDay = DateToDayNumber(TheYear, TheMonth, TheDay);
	GetLunation();
	CalculateMoonPhase();
	tm EventDate; // storage for the final result
	JDtoDate(EightPhases[ APhase ], &EventDate);
	double TideDay = DateToDayNumber(EventDate.tm_year + BASEYEAR, EventDate.tm_mon + 1, EventDate.tm_mday);

//	static char szBug[20];
//	sprintf(szBug, "%20f", TideDay);
//	MessageBox(0,(LPSTR(szBug)),"TideDay", MB_OK);
//	sprintf(szBug, "%20f", TideDay-MyDay);
//	MessageBox(0,(LPSTR(szBug)),"TideDay diff", MB_OK);

// trick to give us the next phase event AFTER specified date
	if (
		 ( TideDay - MyDay < 0 ) && // before specified date
		 ( APhase < 4 )
		)
		JDtoDate(EightPhases[ APhase + 4 ], &EventDate);

	AYear = EventDate.tm_year + BASEYEAR;
	AMonth = EventDate.tm_mon + 1; // because tm_mon is zero based
	ADay = EventDate.tm_mday;
}

int TAstronomy::GetNextMoonPhase(int TheYear,
											int TheMonth,
											int TheDay,
											int APhaseNumber,
											int &APhase,
											int &AHour,
											int &AMinute,
											int &ASecond)
{
	MyDay = DateToDayNumber(TheYear, TheMonth, TheDay);
	GetLunation();
	CalculateMoonPhase();

	char szBug[20];

/*
	EightDates[0] = 5;
	EightDates[1] = 1;
	EightDates[2] = 8;
	EightDates[3] = 3;
	EightDates[4] = 7;
	EightDates[5] = 4;
	EightDates[6] = 2;
	EightDates[7] = 6;
*/

	// bubble sort by date
	for (int j = 0; j < 8; j++)
	{
		EightDates[j][0] = EightPhases[j];
		EightDates[j][1] = j;
	}

/*
	for (j = 0; j < 8; j++)
	{
		sprintf(szBug, "%20f", EightPhases[j]);
		MessageBox(0,(LPSTR(szBug)),"phases", MB_OK);
	}
*/


	if (APhaseNumber == -1)
	{
		for (int j = 0; j < 8; j++)
		{
#ifdef __BORLANDC__
			sprintf(szBug, "%20f", EightDates[j][0]);
			MessageBox(0,(LPSTR(szBug)),"dates", MB_OK);
			sprintf(szBug, "%20f", EightDates[j][1]);
			MessageBox(0,(LPSTR(szBug)),"dates", MB_OK);
#else  // __BORLANDC__
			cerr << "dates " << EightDates[j][0] << " " << EightDates[j][1] << endl;
#endif  // __BORLANDC__
		}
	}

	int limit;

	int N = 8, temp;
	double dtemp;
	// Make steadily shorter passes ...
	for ( limit = N - 1; limit > 0; limit-- )
	{
		int swapped;

		// On each pass, sweep largest element to end of array
		swapped = 0;
		for (int j = 0; j < limit; j++ )
		{
//			if ( Compare ( EightDates[j], EightDates[j+1] ) > 0 )
			if ( EightDates[j][0] > EightDates[j+1][0] )
			{
				dtemp = EightDates[j][0];
				temp = EightDates[j][1];
				EightDates[j][0] = EightDates[j+1][0];
				EightDates[j][1] = EightDates[j+1][1];
				EightDates[j+1][0] = dtemp;
				EightDates[j+1][1] = temp;
				swapped = 1;
			}
		}

		if ( !swapped )
			break; // if no swaps, we have finished
	 }


	if (APhaseNumber == -1)
	{
		// static char szBug[20];

		for (int j = 0; j < 8; j++)
		{
#ifdef __BORLANDC__
			sprintf(szBug, "%20f", EightDates[j][0]);
			MessageBox(0,(LPSTR(szBug)),"Sort by date", MB_OK);
			sprintf(szBug, "%20f", EightDates[j][1]);
			MessageBox(0,(LPSTR(szBug)),"Sort by date", MB_OK);
#else  // __BORLANDC__
			cerr << "Sort by date " << EightDates[j][0] << " " << EightDates[j][1] << endl;
#endif  // __BORLANDC__
		}
	}

	tm EventDate; // storage for the final result

	int i = 0;
	for (int j=0; j<8; j++)
	{
		JDtoDate(EightDates[j][0], &EventDate);
		if ( EventDate.tm_mon + 1 == TheMonth )
		{
			i++; // found a phase in the required month
			if ( i == APhaseNumber )
			{
				APhase = EightDates[j][1];
				AHour = EventDate.tm_hour;
				AMinute = EventDate.tm_min;
				ASecond = EventDate.tm_sec;
				return EventDate.tm_mday;
			}
		}
	}
	// the function should only get here on a bad call
	APhase = -1;
	AHour = -1;
	AMinute = -1;
	ASecond = -1;
	return -1;
}

void TAstronomy::CalculateMoonPhase()
{
	int MyLunation = Lunation;
//	static char szBug[20];
//	sprintf(szBug, "%20i", MyLunation);
//	MessageBox(0,(LPSTR(szBug)),"Looney", MB_OK);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
			EightPhases[i*4+j] = MoonPhaseByLunation(MyLunation - LUNATION_OFFSET, j);
		MyLunation++;
	}


}

int TAstronomy::GetLunation(int AYear,
									 int AMonth,
									 int ADay)
{
	MyDay = DateToDayNumber(AYear, AMonth, ADay);
	return GetLunation();
}

int TAstronomy::GetLunation()
{
	Lunation = floor(MyDay / synmonth + firstlunation); // Osborne synodic month = 29.530588853 slightly different
	tm EventDate; // storage for the final result
	JDtoDate(MoonPhaseByLunation(Lunation - LUNATION_OFFSET, NEWMOON), &EventDate);
	double TempDay = DateToDayNumber(EventDate.tm_year + BASEYEAR, EventDate.tm_mon + 1, EventDate.tm_mday);
	int synodicmonth = synmonth;
	long MyLongDay = MyDay-TempDay;
	Lunation = Lunation + MyLongDay / synodicmonth;
//	static char szBug[20];
//	sprintf(szBug, "%20i", Lunation);
//	MessageBox(0,(LPSTR(szBug)),"Lunation from dll", MB_OK);
	return Lunation;
}

// Old stuff

/*

// Moon phases - Routines from phoon

// TruePhase - given a K value used to determine the mean phase of the
//             new moon, and a phase selector (0.0, 0.25, 0.5, 0.75),
//             obtain the true, corrected phase time

double TAstronomy::TruePhase(double k, double phase)
{
	double t, t2, t3, pt, m, mprime, f;
	int apcor = 0;

	k += phase;		   // add phase to new moon time
	t = k / 1236.85;	   // time in Julian centuries from 1900 January 0.5
	t2 = t * t;		   // square for frequent use
	t3 = t2 * t;	      // cube for frequent use

	pt = 2415020.75933	              // mean time of phase
		  + synmonth * k
		  + 0.0001178 * t2
		  - 0.000000155 * t3
		  + 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

	m = 359.2242                      // Sun's mean anomaly
		 + 29.10535608 * k
		 - 0.0000333 * t2
		 - 0.00000347 * t3;

	mprime = 306.0253                 // Moon's mean anomaly
				+ 385.81691806 * k
				+ 0.0107306 * t2
				+ 0.00001236 * t3;

	f = 21.2964                       // Moon's argument of latitude
		 + 390.67050646 * k
		 - 0.0016528 * t2
		 - 0.00000239 * t3;

	if (
		 ( phase < 0.01 ) ||
		 ( abs(phase - 0.5) < 0.01 )
		)
	{
		// Corrections for New and Full Moon

		pt += (0.1734 - 0.000393 * t) * dsin(m)
				+ 0.0021 * dsin(2 * m)
				- 0.4068 * dsin(mprime)
				+ 0.0161 * dsin(2 * mprime)
				- 0.0004 * dsin(3 * mprime)
				+ 0.0104 * dsin(2 * f)
				- 0.0051 * dsin(m + mprime)
				- 0.0074 * dsin(m - mprime)
				+ 0.0004 * dsin(2 * f + m)
				- 0.0004 * dsin(2 * f - m)
				- 0.0006 * dsin(2 * f + mprime)
				+ 0.0010 * dsin(2 * f - mprime)
				+ 0.0005 * dsin(m + 2 * mprime);

		apcor = 1;
	}
	else
	if (
		  ( abs( phase - 0.25 ) < 0.01 ||
		  ( abs( phase - 0.75 ) < 0.01 ))
		)
	{
		pt += (0.1721 - 0.0004 * t) * dsin(m)
				+ 0.0021 * dsin(2 * m)
				- 0.6280 * dsin(mprime)
				+ 0.0089 * dsin(2 * mprime)
				- 0.0004 * dsin(3 * mprime)
				+ 0.0079 * dsin(2 * f)
				- 0.0119 * dsin(m + mprime)
				- 0.0047 * dsin(m - mprime)
				+ 0.0003 * dsin(2 * f + m)
				- 0.0004 * dsin(2 * f - m)
				- 0.0006 * dsin(2 * f + mprime)
				+ 0.0021 * dsin(2 * f - mprime)
				+ 0.0003 * dsin(m + 2 * mprime)
				+ 0.0004 * dsin(m - 2 * mprime)
				- 0.0003 * dsin(2 * m + mprime);

		if (phase < 0.5) // First quarter correction
			pt += 0.0028 - 0.0004 * dcos(m) + 0.0003 * dcos(mprime);
		else // Last quarter correction
			pt += -0.0028 + 0.0004 * dcos(m) - 0.0003 * dcos(mprime);
		apcor = 1;
	}

	if (!apcor)
	{
		::MessageBox(0, "TruePhase() called with invalid phase selector", "Params DLL", MB_OK);
		return -1;
		// abort();
	}
	return pt;
}

void TAstronomy::JYear(double td, int* yy, int* mm, int* dd)
{
	double j, d, y, m;

	td += 0.5;		   // astronomical to civil
	j = floor(td);
	j = j - 1721119.0;
	y = floor(((4 * j) - 1) / 146097.0);
	j = (j * 4.0) - (1.0 + (146097.0 * y));
	d = floor(j / 4.0);
	j = floor(((4.0 * d) + 3.0) / 1461.0);
	d = ((4.0 * d) + 3.0) - (1461.0 * j);
	d = floor((d + 4.0) / 4.0);
	m = floor(((5.0 * d) - 3) / 153.0);
	d = (5.0 * d) - (3.0 + (153.0 * m));
	d = floor((d + 5.0) / 5.0);
	y = (100.0 * y) + j;
	if (m < 10.0)
		m = m + 3;
	else
	{
		m = m - 9;
		y = y + 1;
	}
	*yy = y;
	*mm = m;
	*dd = d;
}

// MeanPhase - calculates mean phase of the Moon for a given base date and desired phase:
//	0.0   New Moon
// 0.25  First quarter
// 0.5   Full moon
// 0.75  Last quarter
// Beware!!!  This routine returns meaningless results for any other phase arguments.  Don't attempt to generalise
// it without understanding that the motion of the moon is far more complicated that this calculation reveals.

double TAstronomy::MeanPhase(double sdate, double phase, double* usek)
{
	int yy, mm, dd;
	double k, t, t2, t3, nt1;
	long ldate = sdate;
	// DayNumberToDate(ldate, yy, mm, dd);
	JYear(sdate, &yy, &mm, &dd); // call julian year

	k = (yy + ((mm - 1) * (1.0 / 12.0)) - 1900) * 12.3685;

	// Time in Julian centuries from 1900 January 0.5.
	t = (sdate - 2415020.0) / 36525;
	t2 = t * t;			// square for frequent use
	t3 = t2 * t;		// cube for frequent use

	*usek = k = floor(k) + phase;
	nt1 = 2415020.75933 + synmonth * k
			+ 0.0001178 * t2
			- 0.000000155 * t3
			+ 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

	return nt1;
}

// MeanPhase - calculates mean phase of the Moon for a given base date and desired phase - Overloaded by JGF on 97.12.08
//	0.0   New Moon
// 0.25  First quarter
// 0.5   Full moon
// 0.75  Last quarter
// Beware!!!  This routine returns meaningless results for any other phase arguments.  Don't attempt to generalise
// it without understanding that the motion of the moon is far more complicated that this calculation reveals.

double TAstronomy::MeanPhase(double MyDayNumber, int AYear, int AMonth, double phase, double* usek)
{
	const double offset = 1721426.0; // to match the jdate function used by phoon to "my" other astronomical functions
	double k, t, t2, t3, nt1;

	k = (AYear + ((AMonth - 1) * (1.0 / 12.0)) - 1900) * 12.3685;

	// Time in Julian centuries from 1900 January 0.5.
	t = ((MyDayNumber + offset) - 2415020.0) / 36525;
//

	t = (MyDayNumber - 2415020.0) / 36525;
	t2 = pow(t,2);			// square
	t3 = pow(t,3);		   // cube

	*usek = k = floor(k) + phase;
	nt1 = 2415020.75933 + synmonth * k
			+ 0.0001178 * t2
			- 0.000000155 * t3
			+ 0.00033 * dsin(166.56 + 132.87 * t - 0.009173 * t2);

	return nt1;
}

// PhaseHunt5 - find time of phases of the moon which surround the current date.  Five phases are found,
// starting and ending with the new moons which bound the current lunation

void TAstronomy::PhaseHunt5(double AJulianDay)
{
	const double offset = 1721426.0; // to match the jdate function used by phoon to "my" other astronomical functions
	static char szBug[20];
	int yy, mm, dd;
	double adate, k1, k2, nt1, nt2;
//	AJulianDay = AJulianDay + offset; // to match the jdate function used by phoon to "my" other astronomical functions
	adate = AJulianDay - 45;
	long ldate = adate;
	DayNumberToDate(ldate, yy, mm, dd);
	sprintf(szBug, "%20i", yy);
	MessageBox(0,(LPSTR(szBug)),"year", MB_OK);
	sprintf(szBug, "%20i", mm);
	MessageBox(0,(LPSTR(szBug)),"month", MB_OK);
	sprintf(szBug, "%20i", dd);
	MessageBox(0,(LPSTR(szBug)),"day", MB_OK);
	nt1 = MeanPhase(ldate, yy, mm, 0.0, &k1);

   adate = adate + offset;

	sprintf(szBug, "%20f", adate);
	MessageBox(0,(LPSTR(szBug)),"adate", MB_OK);
	sprintf(szBug, "%20f", nt1);
	MessageBox(0,(LPSTR(szBug)),"nt1", MB_OK);



//	int i = 1;
	for (;;)
	{
//		i++;
//		if ( i == 100)
//			break;
		adate += synmonth;
		nt2 = MeanPhase(ldate, yy, mm, 0.0, &k2);

//		sprintf(szBug, "%20f", nt2);
//		MessageBox(0,(LPSTR(szBug)),"nt2", MB_OK);

		if (nt1 <= adate && nt2 > adate)
			break;
		nt1 = nt2;
		k1 = k2;
	}
	FivePhases[0] = TruePhase(k1, 0.0) - offset;   // New moon
	FivePhases[1] = TruePhase(k1, 0.25) - offset;  // First quarter
	FivePhases[2] = TruePhase(k1, 0.5) - offset;   // Full moon
	FivePhases[3] = TruePhase(k1, 0.75) - offset;  // Last quarter
	FivePhases[4] = TruePhase(k2, 0.0) - offset;   // Next new moon
}

// PhaseHunt2 - find time of phases of the moon which surround the current date. Two phases are found.

void TAstronomy::PhaseHunt2(double sdate)
{
	double adate, k1, k2, nt1, nt2;
	adate = sdate - 45;
	nt1 = MeanPhase(adate, 0.0, &k1);
	for (;;)
	{
		adate += synmonth;
		nt2 = MeanPhase(adate, 0.0, &k2);
		if (nt1 <= sdate && nt2 > sdate)
			break;
		nt1 = nt2;
		k1 = k2;
	}
	TwoPhases[0] = TruePhase(k1, 0.0);
	Which[0] = 0.0;
	TwoPhases[1] = TruePhase(k1, 0.25);
	Which[1] = 0.25;
	if ( TwoPhases[1] <= sdate )
	{
		TwoPhases[0] = TwoPhases[1];
		Which[0] = Which[1];
		TwoPhases[1] = TruePhase(k1, 0.5);
		Which[1] = 0.5;
		if ( TwoPhases[1] <= sdate )
		{
			TwoPhases[0] = TwoPhases[1];
			Which[0] = Which[1];
			TwoPhases[1] = TruePhase(k1, 0.75);
			Which[1] = 0.75;
			if ( TwoPhases[1] <= sdate )
			{
				TwoPhases[0] = TwoPhases[1];
				Which[0] = Which[1];
				TwoPhases[1] = TruePhase(k2, 0.0);
				Which[1] = 0.0;
			}
		}
	}
}

// Kepler - solve the equation of Kepler
double TAstronomy::Kepler( double m,  double ecc )
{
	double e, delta;

	e = m = torad(m);
	do
	{
		delta = e - ecc * sin(e) - m;
		e -= delta / (1 - ecc * cos(e));
	}
	while (abs(delta) > EPSILON);
	return e;
}

// Calculate phase of moon as a fraction.
// @param pdate time for which the phase is requested, as from jtime()
// @param pphaseR Ref for illuminated fraction of Moon's disk
// @param mageR Ref for age of moon in days
// @param distR Ref for distance in km from center of Earth
// @param angdiaR Ref for angular diameter in degrees as seen from Earth
// @param sudistR Ref for distance in km to Sun
// @param suangdiaR Ref for Sun's angular diameter
// @return terminator phase angle as a fraction of a full circle (i.e., 0 to 1)

double TAstronomy::Phase( double pdate, double pphaseR, double mageR, double distR,
								  double angdiaR, double sudistR, double suangdiaR )
{
	double Day, N, M, Ec, Lambdasun, ml, MM, Ev, Ae, A3, MmP,
			 mEc, A4, lP, V, lPP,
			 MoonAge, MoonPhase,
			 MoonDist, MoonDFrac, MoonAng,
			 F, SunDist, SunAng;

	// Calculation of the Sun's position

	Day = pdate - epoch;                    // date within epoch
	N = fixangle((360 / 365.2422) * Day);   // mean anomaly of the Sun
	M = fixangle(N + elonge - elongp);      // convert from perigee co-ordinates to epoch 1980.0
	Ec = Kepler(M, eccent);                 // solve equation of Kepler
	Ec = sqrt((1 + eccent) / (1 - eccent)) * tan(Ec / 2);
	Ec = 2 * todeg(atan(Ec));               // true anomaly
	Lambdasun = fixangle(Ec + elongp);      // Sun's geocentric ecliptic longitude

	// Orbital distance factor
	F = ((1 + eccent * cos(torad(Ec))) / (1 - eccent * eccent));
	SunDist = sunsmax / F;                  // distance to Sun in km
	SunAng = F * sunangsiz;                 // Sun's angular size in degrees

	// Calculation of the Moon's position

	// Moon's mean longitude
	ml = fixangle(13.1763966 * Day + mmlong);

	// Moon's mean anomaly
	MM = fixangle(ml - 0.1114041 * Day - mmlongp);

	// Evection
	Ev = 1.2739 * sin(torad(2 * (ml - Lambdasun) - MM));

	// Annual equation
	Ae = 0.1858 * sin(torad(M));

	// Correction term
	A3 = 0.37 * sin(torad(M));

	// Corrected anomaly
	MmP = MM + Ev - Ae - A3;

	// Correction for the equation of the centre
	mEc = 6.2886 * sin(torad(MmP));

	// Another correction term
	A4 = 0.214 * sin(torad(2 * MmP));

	// Corrected longitude.
	lP = ml + Ev + mEc - Ae + A4;

	// Variation
	V = 0.6583 * sin(torad(2 * (lP - Lambdasun)));

	// True longitude
	lPP = lP + V;

	// Calculation of the phase of the Moon

	// Age of the Moon in degrees
	MoonAge = lPP - Lambdasun;

	// Phase of the Moon.
	MoonPhase = (1 - cos(torad(MoonAge))) / 2;

	// Calculate distance of moon from the centre of the Earth

	MoonDist = (msmax * (1 - mecc * mecc)) /
				  (1 + mecc * cos(torad(MmP + mEc)));

	// Calculate Moon's angular diameter

	MoonDFrac = MoonDist / msmax;
	MoonAng = mangsiz / MoonDFrac;

	pphaseR = MoonPhase;
	mageR = synmonth * (fixangle(MoonAge) / 360.0);
	distR = MoonDist;
	angdiaR = MoonAng;
	sudistR = SunDist;
	suangdiaR = SunAng;
	return torad(fixangle(MoonAge));
}

void TAstronomy::GetMoonPhase(double AJulianDay,
										int APhase,
										int &AYear,
										int &AMonth,
										int &ADay)
{
	MyDay = AJulianDay;
	PhaseHunt5(MyDay);
	long longdate = FivePhases[ APhase ];
//   static char szBug[20];
//	sprintf(szBug, "%20i", longdate);
//	MessageBox(0,(LPSTR(szBug)),"longdate", MB_OK);
	DayNumberToDate( longdate, AYear, AMonth, ADay );
//	sprintf(szBug, "%20i", ADay);
//	MessageBox(0,(LPSTR(szBug)),"Day", MB_OK);
//	sprintf(szBug, "%20i", AMonth);
//	MessageBox(0,(LPSTR(szBug)),"Month", MB_OK);
//	sprintf(szBug, "%20i", AYear);
//	MessageBox(0,(LPSTR(szBug)),"Year", MB_OK);
}

*/

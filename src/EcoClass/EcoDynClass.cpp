/**
  Name: EcoDynClass.cpp
  Copyright: P.Duarte
  Author: P.Duarte & A.Pereira
  Description: Base class for all objects
  Date: 14-09-2004 16:43 - Code translated to be called by Fortran programs
	
	Date: 02-10-2013, AP - inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifdef __BORLANDC__
	#include <math.h>
	#include <time.h>

	#ifdef _PORT_FORTRAN_
		#include <strings.h>
	#else
		#include "iodll.h"
	#endif  //_PORT_FORTRAN_

#else		// __BORLANDC__
	#include <cmath>
	#include <ctime>
	#include <cstring>
	#include <exception>
	
	using namespace std;
	
	#include "iodll.h"
#endif		// __BORLANDC__
#include "EcoDynClass.h"

//
// Null constructor (invoked inside/outside EcoDyn shell...)
//
TEcoDynClass::TEcoDynClass()
{
    clearVariables();

#ifdef _PORT_FORTRAN_
    buildCalendar(5.0);

    NumberOfLines = 1;
    NumberOfColumns = 1;
    NumberOfBoxes = 1 * 1;

    BoxArray = new TBoxes::BoxRecord[1];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        BoxArray[i].Dummybool2 = true;
        BoxArray[i].Depth = 2.397723;
        BoxArray[i].Length = 100;
        BoxArray[i].Width = 100;
        BoxArray[i].Elevation = 1.7072;
        BoxArray[i].XPosition = 0.0;
        BoxArray[i].YPosition = 0.0;
        BoxArray[i].Type = 1;
        BoxArray[i].BoxToBoundary[1].TypeOfBoundary = 0;
        BoxArray[i].BoxToBoundary[2].TypeOfBoundary = 0;
        BoxArray[i].BoxToBoundary[3].TypeOfBoundary = 2;
        BoxArray[i].BoxToBoundary[4].TypeOfBoundary = 0;
    }
    BoxArrayAllocated = true;
#endif		// _PORT_FORTRAN_
}

#ifndef _PORT_FORTRAN_
//
// Constructors invoked outside EcoDyn shell...
//
TEcoDynClass::TEcoDynClass(float timeStep)
{
    clearVariables();
    buildCalendar(timeStep);
}

TEcoDynClass::TEcoDynClass(float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
{
    clearVariables();
    buildCalendar(timeStep);

    NumberOfLines = nLines;
    NumberOfColumns = nColumns;
    NumberOfBoxes = nLines * nColumns;

    BoxArray = new TBoxes::BoxRecord[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        BoxArray[i].Depth = aDepth[i];
        BoxArray[i].Length = aLength[i];
        BoxArray[i].Width = aWidth[i];
        BoxArray[i].Elevation = aElevation[i];
        BoxArray[i].Type = aType[i];
        BoxArray[i].BoxToBoundary[1].TypeOfBoundary = aNBoundary[i];
        BoxArray[i].BoxToBoundary[2].TypeOfBoundary = aEBoundary[i];
        BoxArray[i].BoxToBoundary[3].TypeOfBoundary = aSBoundary[i];
        BoxArray[i].BoxToBoundary[4].TypeOfBoundary = aWBoundary[i];
    }
    BoxArrayAllocated = true;
}

//
// Constructors invoked inside EcoDyn shell...
//

TEcoDynClass::TEcoDynClass(float timeStep,
                           char* APathName, char *AEcosystemName)
{
  char prefixName[256];

    clearVariables();
    buildCalendar(timeStep);

    strcpy(MyPathName, APathName);

    // build prefixName for mandatoty files
    strcpy(prefixName, APathName);
    strcat(prefixName, PATH_SEPARATOR);
    strcat(prefixName, AEcosystemName);

	// register the file name for file containing morphology
    strcpy(MorphologyFileName, prefixName);
    strcat(MorphologyFileName, GLB_MORPHOLOGY);

	// register the file name for file containing the names of available classes
    strcpy(ClassesFileName, prefixName);
    strcat(ClassesFileName, GLB_CLASSES);

	// register the file name for file containing tide harmonics
    strcpy(TideFileName, prefixName);
    strcat(TideFileName, GLB_TIDE);

    // register the file name for file containing information on location, name and flows for the loads
    strcpy(LoadsFileName, prefixName);
    strcat(LoadsFileName, GLB_LOADS);

    // register the file name for file containing information on location, name and flows for the losses
    strcpy(LossesFileName, prefixName);
    strcat(LossesFileName, GLB_LOSSES);

    // register the file name for file containing information on location, name and flows for the rivers
    strcpy(RiversFileName, prefixName);
    strcat(RiversFileName, GLB_RIVERS);

    // register the file name for file containing information on location and velocities across sea boundaries
    strcpy(SeaBoundariesFileName, prefixName);
    strcat(SeaBoundariesFileName, GLB_SEABOUNDARIES);

    // register the file name for file containing information on location, name and physical characteristics for the sediments
    strcpy(SedimentsFileName, prefixName);
    strcat(SedimentsFileName, GLB_SEDIMENTS);

    // register the file name for file containing information on location and name for the benthic species
    strcpy(BenthicSpeciesFileName, prefixName);
    strcat(BenthicSpeciesFileName, GLB_BENTHICSPECIES);

    // register the file name for file containing information on location, name and physical characteristics for the ice
    strcpy(IceFileName, prefixName);
    strcat(IceFileName, GLB_ICE);
}
#endif // !(_PORT_FORTRAN_)

//This destructor must be virtual for descendants' destructors to be called,if initialised as a base class
TEcoDynClass::~TEcoDynClass()
{
    try {
        if (MyVarRecSize > 0)
            delete [] MyVarRec;
        if (MyDisplayBoxSize > 0)
            delete [] MyDisplayBoxRec;
        if (BoxArrayAllocated)
            delete [] BoxArray;
        if ((NumberOfLoads > 0)
                && ((NumberOfDaysForLoads > 0) || (NumberOfHoursForLoads > 0))) {
            if (LoadsExist) {
                for (int i = 0; i < NumberOfLoads; i++)
                    delete [] ALoadRecord[i].LoadFlow;
            }
            delete [] ALoadRecord;
        }
        if ((NumberOfLosses > 0)
                && ((NumberOfDaysForLosses > 0) || (NumberOfHoursForLosses > 0))){
            if (LossesExist) {
                for (int i = 0; i < NumberOfLosses; i++)
                    delete [] ALossesRecord[i].LossFlow;
            }
            delete [] ALossesRecord;
        }
        if ((NumberOfRivers > 0)
                && ((NumberOfDaysForFlowTimeSeries > 0) || (NumberOfHoursForFlowTimeSeries > 0))) {
            if (RiversExist) {
                for (int i = 0; i < NumberOfRivers; i++)
                    delete [] ARiverRecord[i].RiverFlow;
            }
            delete [] ARiverRecord;
        }
        if ((NumberOfSeaBoundaries > 0)
                && ((NumberOfDaysForSeaBoundaries > 0) || (NumberOfHoursForSeaBoundaries > 0))) {
            if (SeaBoundariesExist) {
                for (int i = 0; i < NumberOfSeaBoundaries; i++)
                    delete [] ASeaBoundariesRecord[i].Velocity;
            }
            delete [] ASeaBoundariesRecord;
        }
        if (NumberOfCellsWithSediments > 0) {
            delete [] ASedimentsRecord;
        }
        if (BenthicSpeciesRecordAllocated) {
            delete [] ABenthicSpeciesRecord;
        }
        if (NumberOfCellsWithIce > 0) {
            delete [] AIceRecord;
         }   
#ifdef __BORLANDC__
        delete pMemCriticalSection;
        delete pMemDebugCriticalSection;
#endif // __BORLANDC__
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else		// __BORLANDC__
		catch (exception &exc) {
				cerr << "TEcoDynClass::~TEcoDynClass " << exc.what() << endl;
		}
#endif		// __BORLANDC__
}

void TEcoDynClass::clearVariables()
{
    NumberOfBoxes = 0;
    NumberOfLines = 0;
    NumberOfColumns = 0;
    NumberOfLayers = 0;

    ALoadRecord = NULL;
    LoadsExist = false;
    NumberOfLoads = NumberOfDaysForLoads = NumberOfHoursForLoads =0;

    ALossesRecord = NULL;
    LossesExist = false;
    NumberOfLosses = NumberOfDaysForLosses = NumberOfHoursForLosses = 0;

    ARiverRecord = NULL;
    RiversExist = false;
    NumberOfRivers = 0;

    ASeaBoundariesRecord = NULL;
    SeaBoundariesExist = false;
    NumberOfSeaBoundaries = 0;

    ASedimentsRecord = NULL;
    SedimentsExist = false;
    NumberOfCellsWithSediments = 0;

    ABenthicSpeciesRecord = NULL;
    BenthicSpeciesExist = false;
    NumberOfCellsWithBenthicSpecies = 0;
    BenthicSpeciesRecordAllocated = false;

    AIceRecord = NULL;
    IceExist = false;
    NumberOfCellsWithIce = 0;

    BoxArray = NULL;
    BoxArrayAllocated = false;

#ifdef __BORLANDC__
    pMem = NULL;
    pMemDebug = NULL;
    pMemCriticalSection = new TCriticalSection();
    pMemDebugCriticalSection = new TCriticalSection();
#else  // __BORLANDC__
	logEnabled = false;
	debugEnabled = false;
#endif // __BORLANDC__

    pRecord = NULL;

    MyPathName[0] = '\0';

    MyPEcoDynClass = this;

    Generic = NULL;
    GenericLoad = NULL;
    GenericFlux = NULL;
    GenericExchange = NULL;
    /*Loads = NULL;
    Losses = NULL; */

    GenericRiver = GenericOcean = 0.0;

    NumberOfVariables = NumberOfParameters = 0;
    ObjectCode = 0;
    VariableNameArray = ParametersNameArray = NULL;

    VariableCount = ParameterCount = 0;
    BoundaryNumberArray = NULL;
    LoadConcentrationArray = NULL;
    UnitNameArray = NULL;

    TimeStep = MyRunTime = MyTime = TimeUTC = 0;
    Year = Month = JulianDay = Date = 0;

    CurrentTime = TimeBefore = 0;

    a = b = c = r = u = gam = NULL;

    MorphologyFileName[0] = '\0';
    VarsFileName[0] = '\0';
    ParsFileName[0] = '\0';
    TideFileName[0] = '\0';
    ClassesFileName[0] = '\0';
    EcoDynClassName[0] = '\0';
    LoadsFileName[0] = '\0';
    LossesFileName[0] = '\0';
    RiversFileName[0] = '\0';
    SeaBoundariesFileName[0] = '\0';
    SedimentsFileName[0]= '\0';
    BenthicSpeciesFileName[0]= '\0';
    IceFileName[0]= '\0';
    MyVarRecSize = 0;
    MyVarRec = NULL;

    MyDisplayBoxSize = 0;
    MyDisplayBoxRec = NULL;

    subDomain = NULL;

    ActiveObjects = ActiveStateVariables = 0;

    NumberOfSelectedVariables = 0;

    StartSaveDay = EndSaveDay = 0;

    OutputFrequency = 0;

    MyBoxTop = MyBoxBottom = 0;

    MyTransportPointer = NULL;
    MyNutrientPointer = NULL;
    MySPMPointer = NULL;
    MyPhytoplanktonPointer = NULL;
    MyZooplanktonPointer = NULL;
    MySaltMarshPointer = NULL;
    MyZoobenthosPointer = NULL;
    MyZoobenthosPointer2 = NULL;
    MyNektonPointer = NULL;
    MyManPointer = NULL;
    MyFisherPointer = NULL;
    MyBacteriaPointer = NULL;
    MyMacrophytePointer = NULL;
    MyMicrophytobenthosPointer = NULL;
    MyLightPointer = NULL;
    MyAirTemperaturePointer = NULL;
    MyWaterTemperaturePointer = NULL;
    MyWindPointer = NULL;
    MyFlowPointer = NULL;
    MyTidePointer = NULL;
    MySedimentPointer = NULL;
    MyPreyPointer = NULL;
    MyPredatorPointer = NULL;
    MyFishPointer = NULL;

    // Following lines included to support multi-threaded version
    EnableContinuity = false;
    EnableReinitializeFromGo = false;
    EnableReinitializeFromIntegrate = false;
    EnableMultiThread = false;
    IsMultiThreadedVersion = false;
}

void TEcoDynClass::buildCalendar(float timeStep)
{
    TimeStep = timeStep;

	// For calendar functions
	DaysInMonth[0] = 31;
	DaysInMonth[1] = 28;
	DaysInMonth[2] = 31;
	DaysInMonth[3] = 30;
	DaysInMonth[4] = 31;
	DaysInMonth[5] = 30;
	DaysInMonth[6] = 31;
	DaysInMonth[7] = 31;
	DaysInMonth[8] = 30;
	DaysInMonth[9] = 31;
	DaysInMonth[10] = 30;
	DaysInMonth[11] = 31;

	Year = 1;
	Month = 1;
	Date = 1;
	JulianDay = 1;//181
	CurrentTime = 0; //0
	TimeBefore = -1;
    MyTime = 0;
    AStartTime = 0; AFinishTime = 0; ATime = 0; ADay = 0;
    // Start time calendar is adjusted before "GO" with "SetTime"
    OutputFrequency = ALWAYSOUTPUT;
}

double TEcoDynClass::ten(double power)  // returns power of ten passed to it by call
{
	const float log_e = 0.434294;
	return exp(power/log_e);
}


double TEcoDynClass::exponent(double mantissa, double power)  //returns power of
            												//mantissa passed to it
															// by call

{
	return exp(power * log(mantissa));
}

double TEcoDynClass::GetDensity(double ASalinity,
										double ATemperature)
{
	double At,Bt,Et, SigmaT, Sigma0, Cl,
			 MySalinity = ASalinity,
			 MyTemperature = ATemperature;

	//Use either Cox or Knudsen}
	if (MySalinity > 10)

		// Cox et al., 1970 - First coefficient is ten(-2) not ten(2) as Riley
		//						    indicates in the listed equation
		SigmaT = 8.009691 * ten(-2)
					+ 5.88194 * ten(-2) * MyTemperature
					+ 0.7901864 * MySalinity
					- 8.114654 * ten(-3) * sqr(MyTemperature)
					- 3.253104 * ten(-3) * MySalinity * MyTemperature
					+ 1.31708 * ten(-4) * sqr(MySalinity)
					+ 4.76004 * ten(-5) * MyTemperature * sqr(MyTemperature)
					+ 3.892875 * ten(-5) * MySalinity * sqr(MyTemperature)
					+ 2.879715 * ten(-6) * sqr(MySalinity) * MyTemperature
					- 6.118315 * ten(-8) * MySalinity * sqr(MySalinity);

	else
	{
		// Knudsen - 1901
		Cl = MySalinity/1.80655;

		Sigma0 = -6.9 * ten(-2)
					+ 1.4708 * Cl
					- 1.570 * ten(-3) * sqr(Cl)
					+ 3.98 * ten(-5) * Cl;

		At =   4.7867 * ten(-3) * MyTemperature
			  - 9.8185 * ten(-5) * sqr(MyTemperature)
			  + 1.0843 * ten(-6) * MyTemperature * sqr(MyTemperature);

		Bt =   1.803 * ten(-5) * MyTemperature
			  - 8.146 * ten(-7) * sqr(MyTemperature)
			  + 1.667 * ten(-8) * MyTemperature * sqr(MyTemperature);

		Et = -(sqr(MyTemperature-3.98) * (MyTemperature +283)
			  /(503.57 * (MyTemperature + 67.26)));

		SigmaT =   (Sigma0 + 0.1324)
					* (1 - At + Bt * (Sigma0 - 0.1324))
					+ Et;
	}

	return SigmaT + 1000; // kg m-3
}


//International Equation of State of Sea Water, 1980 (e.g. Pond & Pickard, 1983. Introductory Dynamical Oceanogrpahy. Appendix 3
//Code obtained from http://www.listserv.uga.edu/cgi-bin/wa?A2=ind0307&L=spssx-l&P=9414
double TEcoDynClass::GetDensity(double salinity /*psu*/,double temperature, double pressure/*decibars*/)
{
	double   s, t, p, density, A, B, C, D, d0, E, F, G, H, I, J, M, N, s1p5, pb, K;

    s = salinity;
    t = temperature;
    p = pressure;

    A = 999.842594 + t * (  6.793952e-02 + t * ( -9.095290e-03 + t *(1.001685e-04 + t * ( -1.120083e-06 + t * 6.536332e-09 ))));
    B = 0.824493 + t * ( -4.0899e-03 + t * (7.6438e-05+ t * ( -8.2467e-07 + t * 5.3875e-09 )));
    C = -5.72466e-03 + t * ( 1.0227e-04 - t * 1.6546e-06 );
    D = 4.8314e-04;
    d0 = A + s * (  B + C * sqrt(s) + D * s );
    if (pressure > 0.0) {
      E = 19652.21 + 148.4206 * t - 2.327105 * pow(t,2) + 1.360477e-2 * pow(t,3) - 5.155288e-5 * pow(t,4);
      F = 54.6746 - 0.603459 * t + 1.09987e-2 * pow(t,2) - 6.1670e-5 * pow(t,3);
      G = 7.944e-2 + 1.6483e-2 * t - 5.3009e-4 * pow(t,2);
      H = 3.239908 + 1.43713e-3 * t + 1.16092e-4 * pow(t,2) - 5.77905e-7 * pow(t,3);
      I = 2.2838e-3 - 1.0981e-5 * t - 1.6078e-6 * pow(t,2);
      J = 1.91075e-4;
      M = 8.50935e-5 - 6.12293e-6 * t + 5.2787e-8 * pow(t,2);
      N = -9.9348e-7 + 2.0816e-8 * t + 9.1697e-10 * pow(t,2);

      s1p5 = s * sqrt(s);

      pb = p/10.0;

      K = (E + F*s + G*s1p5) + (H + I*s + J*s1p5) * pb + (M + N*s) * pow(pb,2);
      density = d0 / (1 - pb/K);
    }
    else density = d0;
    return density;
}

//Riley & Skirrow - Page 576 - in centipoise
double TEcoDynClass::GetViscosity(double salinity, double temperature) // Call Salinity in o/oo
																							// and Temperature in oC
{
	const double A5 = 0.000366;
	const double A25 = 0.001403;
	const double B5 = 0.002756;
	const double B25 = 0.003416;

	double viscosity_pureH20, A_t, B_t, A_slope, B_slope,
			 A_intercept, B_intercept, Cl_vol;

	// Calculate viscosity of pure water for specified temperature
	viscosity_pureH20 = 1.0020 * ten((1.1709 * (20 - temperature)
							  - 0.001827 * sqr(temperature - 20))
							  / (temperature + 89.93)); // in the pascal code (and EcoWin) its 89.92 - wrong)

	// Calculate volume chlorinity = chlorinity * density
	Cl_vol = (salinity/1.80655) * GetDensity(salinity,temperature)/1000;

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

// R.M. Tennent - Science data book - Page 80
double TEcoDynClass::GetGravity(double DegreesMinutes, double heightAboveSeaLevel)   // Call DEGMIN e.g.38º44' (3844)
																											  // and height above sea level in m
{
	const double polar_latitude = 9000;
	double latitude;
	// gravity = 9.80665;                                   // "Default" gravity
	if (DegreesMinutes < 0) DegreesMinutes
									= -DegreesMinutes;              // for negative latitudes
	if (heightAboveSeaLevel < 0) heightAboveSeaLevel = 0;   // normalize at sea-level
	if (DegreesMinutes <= polar_latitude)
	{
		latitude = floor(DegreesMinutes/100);              // Latitude in degrees - OK (different from Oceanus)
		latitude = latitude                                // and minutes as decimal - 96.06.24 JGF
					  + ((DegreesMinutes/100)-latitude)*100/60;
		return 9.80616 - 0.025928 * cos(2 * latitude * M_PI/180)  // m s-2
				 + 0.000069 * sqr(cos (2 * latitude * M_PI/180))
				 - 0.000003 * heightAboveSeaLevel;
	}
    else
        return 9.80616;
}

void TEcoDynClass::CalculateNumberOfObjects()
{
	ActiveObjects = 0;
	ActiveStateVariables = 0;
	if (GetTransportPointer() != NULL)
	{
		ActiveStateVariables = ActiveStateVariables
									  + GetTransportPointer()->NumberOfVariables;
		ActiveObjects++;
	}
	if (GetNutrientPointer() != NULL)
	{
		ActiveObjects++;
		ActiveStateVariables = ActiveStateVariables
									  + GetNutrientPointer()->NumberOfVariables;
	}
	if (GetSPMPointer() != NULL)
	{
   	ActiveStateVariables = ActiveStateVariables
									  + GetSPMPointer()->NumberOfVariables;
		ActiveObjects++;
	}
	if (GetPhytoplanktonPointer() != NULL)
	{
		ActiveStateVariables = ActiveStateVariables
									  + GetPhytoplanktonPointer()->NumberOfVariables;
		ActiveObjects++;
	}
	if (GetZooplanktonPointer() != NULL)
	{
		ActiveStateVariables = ActiveStateVariables
									  + GetZooplanktonPointer()->NumberOfVariables;
		ActiveObjects++;
	}
	if (GetZoobenthosPointer() != NULL)
	{
   	ActiveStateVariables = ActiveStateVariables
									  + GetZoobenthosPointer()->NumberOfVariables;
		ActiveObjects++;
	}
    if (GetNektonPointer() != NULL)
    {
   	    ActiveStateVariables = ActiveStateVariables
      							  + GetNektonPointer()->NumberOfVariables;
        ActiveObjects++;
    }
	if (GetManPointer() != NULL)
	{
   	    ActiveStateVariables = ActiveStateVariables
									  + GetManPointer()->NumberOfVariables;
		ActiveObjects++;
	}
	if (GetMacrophytePointer() != NULL)
	{
   	    ActiveStateVariables = ActiveStateVariables
									  + GetMacrophytePointer()->NumberOfVariables;
		ActiveObjects++;
	}
    if (GetSaltMarshPointer() != NULL)
	{
   	    ActiveStateVariables = ActiveStateVariables
									  + GetSaltMarshPointer()->NumberOfVariables;
		ActiveObjects++;
	}
    if (GetMicrophytobenthosPointer() != NULL)
	{
   	    ActiveStateVariables = ActiveStateVariables
									  + GetMicrophytobenthosPointer()->NumberOfVariables;
		ActiveObjects++;
	}
    if (GetTidePointer() != NULL)
	{
   	    ActiveStateVariables = ActiveStateVariables
									  + GetTidePointer()->NumberOfVariables;
		ActiveObjects++;
	}
    if (GetSedimentPointer() != NULL)
	{
   	    ActiveStateVariables = ActiveStateVariables
									  + GetSedimentPointer()->NumberOfVariables;
		ActiveObjects++;
	}
}

//New - Pedro Duarte 9/97



// This routine is temporary - it only works for a 1D system with vertical box
// structure, because the algorithm considers that the first box is the surface
// and that subsequent boxes are sequentially deeper - 96.02.14

void  TEcoDynClass::GetBoxTopAndBottom(int ABoxNumber,
												 double &ATop,
												 double &ABottom)
{
	int MyBoxNumber = ABoxNumber;
	if (MyBoxNumber > NumberOfBoxes)
	{
#ifdef __BORLANDC__
        if ( Application->MessageBox("Invalid box number","EcoDynamo Message", MB_OKCANCEL) != IDOK)
            throw;
#else		// __BORLANDC__
				cerr << "TEcoDynClass::GetBoxTopAndBottom - Invalid box number: " << ABoxNumber << endl;
#endif // __BORLANDC__
		ATop = 0;
		ABottom = 0;
		return;
	}
	float MyTop = 0;
	float MyBottom = GetBoxDepth(MyBoxNumber);

	for (int i = 0; i < MyBoxNumber; i++)
		MyTop = MyTop + GetBoxDepth(i);

	MyBottom = MyBottom + MyTop;

	ATop = MyTop;
	ABottom = MyBottom;
}

// AP, 2005.07.29
void TEcoDynClass::SetStartTime(long Atime)
{
    startTimeUTC = Atime;
    AStartTime = getDefaultsRecord()->StartTime;
    ATime = AStartTime;
    //Debugger(ATime);
    AFinishTime = getDefaultsRecord()->FinishTime;
}

long TEcoDynClass::GetStartTime()
{
    return startTimeUTC;
}

void TEcoDynClass::SetTime(long ATime)
{
  struct tm *startDT;

    if (ATime > 0) {
    	TimeUTC = ATime;
        startDT = gmtime(&ATime);
        Year = startDT->tm_year + 1900;
        Month = startDT->tm_mon + 1;
        Date = startDT->tm_mday;
        CurrentTime = (((startDT->tm_hour * 60) + (startDT->tm_min)) * 60 + (startDT->tm_sec))
                       / HOURSTOSECONDS;
    	JulianDay = dayOfYear(Year, Month, Date);
    }
}
   
int TEcoDynClass::GetAYear(long ATime)
{
  struct tm *startDT;

    if (ATime > 0) {
        startDT = gmtime(&ATime);
        return (startDT->tm_year + 1900);
    }
    else
    return 0;
}

int TEcoDynClass::GetAMonth(long ATime)
{
  struct tm *startDT;

    if (ATime > 0) {
        startDT = gmtime(&ATime);
        return (startDT->tm_mon + 1);
    }
    else
    return 0;
}

int TEcoDynClass::GetADate(long ATime)
{
  struct tm *startDT;

    if (ATime > 0) {
        startDT = gmtime(&ATime);
        return (startDT->tm_mday);
    }
    else
    return 0;
}

int TEcoDynClass::GetAJulianDay(long ATime) // AP, 2005.11.11
{
  struct tm *startDT;

    if (ATime > 0) {
        startDT = gmtime(&ATime);
    	return dayOfYear(startDT->tm_year + 1900,
                         startDT->tm_mon + 1,
                         startDT->tm_mday);
    }
    else
        return 0;
}

float TEcoDynClass::GetAnHour(long ATime)
{
   struct tm *startDT;
   if (ATime > 0) {
      startDT = gmtime(&ATime);
      return((1.0*((startDT->tm_hour * 60) + (startDT->tm_min)) * 60 + (startDT->tm_sec))
                     / HOURSTOSECONDS);
   }
}

long TEcoDynClass::GetACurrentTime(long ATime)
{
  struct tm *startDT;

    if (ATime > 0) {
        startDT = gmtime(&ATime);
        return ((((startDT->tm_hour * 60) + (startDT->tm_min)) * 60 + (startDT->tm_sec))
                       / HOURSTOSECONDS);
    }
    else
    return 0;
}

void TEcoDynClass::SetMyTime(long ATime)
{
    MyTime = MyTime + TimeStep / DAYSTOSECONDS;
}

int TEcoDynClass::GetDaysToMonth(int year, int month)
{
  int days = 0;
  int leapYear = ((year % 4 == 0) ? 1 : 0);

    for (int i = 0; i < month - 1; i++)
        days += DaysInMonth[i];
    if (month > 2)
        days += leapYear;
    return days;
}

int TEcoDynClass::dayOfYear(int year, int month, int day)
{
    return (GetDaysToMonth(year, month) + day);
}

/*
bool TEcoDynClass::TurnOn(char* &AName, int &AnObjectCode)
{
	AnObjectCode = ObjectCode;
	if (VariableCount < NumberOfVariables)
		strcpy(AName, VariableNameArray[VariableCount]);
	else
		strcpy(AName, "Error");
	VariableCount++;
	if (VariableCount == NumberOfVariables)
	{
		VariableCount = 0;
		return true;
	}
	return false;
}
*/

void TEcoDynClass::SetStateVariableArray(VariableRecord* AVarRec, int AVarRecSize)
{
	int i;
	MyVarRecSize = 0;

	for (i = 0; i < AVarRecSize; i++)
		if (AVarRec[i].IsSelected)
			MyVarRecSize++;

	if (MyVarRecSize == 0)
	{
		return;
	}

	MyVarRec = new VariableRecord[MyVarRecSize];
	int Index = 0;
	for (i = 0; i < AVarRecSize; i++)
		if (AVarRec[i].IsSelected)
		{
			strcpy(MyVarRec[Index].VariableName, AVarRec[i].VariableName);
			MyVarRec[Index].ObjectCode = AVarRec[i].ObjectCode;
			MyVarRec[Index].Low = AVarRec[i].Low;
			MyVarRec[Index].High = AVarRec[i].High;
			MyVarRec[Index].ObjectPointer = AVarRec[i].ObjectPointer;
			MyVarRec[Index].IsStateVariable = AVarRec[i].IsStateVariable;
			MyVarRec[Index].IsSelected = AVarRec[i].IsSelected;
			Index++;
		}
}


void TEcoDynClass::SetDisplayBoxArray(DisplayBoxRecord* ABoxRec, int ABoxRecSize)
{
	int i;

	MyDisplayBoxSize = 0;

	for (i = 0; i < ABoxRecSize; i++)
		if (ABoxRec[i].IsSelected)
			MyDisplayBoxSize++;

	if (MyDisplayBoxSize == 0)
	{
		return;
	}

	MyDisplayBoxRec = new DisplayBoxRecord[MyDisplayBoxSize];
	int Index = 0;
	for (i = 0; i < ABoxRecSize; i++)
		if (ABoxRec[i].IsSelected)
		{
			strcpy(MyDisplayBoxRec[Index].BoxName, ABoxRec[i].BoxName);
			MyDisplayBoxRec[Index].BoxNumber = ABoxRec[i].BoxNumber;
			MyDisplayBoxRec[Index].IsSelected = ABoxRec[i].IsSelected;
			Index++;
		}
}

/* AP, 2006.01.19...

void TEcoDynClass::Integration()
{
	int i;
	// Update the array
	for (i = 0; i < NumberOfBoxes; i++)
	{
		Generic[i] = Generic[i] + GenericFlux[i] * TimeStep;
    	// Zero the fluxes
		GenericFlux[i] = 0;
	}
}
*/


void TEcoDynClass::Integration()
{
	int i, box;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

	// Update the arrays
	for (i = 0; i < pSubDomain->NumberOfBoxes; i++)
	{
        box = pSubDomain->BoxNumber[i];
		Generic[box] = Generic[box] + GenericFlux[box] * TimeStep;
    	        // Zero the fluxes
		GenericFlux[box] = 0;
	}
}

void TEcoDynClass::Integration3DSubDomain(int UpperLayer)
{
   int index;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int i = pSubDomain->FirstLine; i <= pSubDomain->LastLine; i++)
   {
      for (int j = pSubDomain->IColumn[i]; j <= MIN(pSubDomain->FColumn[i],NumberOfColumns-1); j++)
      {
         for (int LayerNr = 0; LayerNr <= UpperLayer; LayerNr++)
         {
             index = Get3DIndex(i,j,LayerNr);
             Generic[index ] = Generic[index ] + GenericFlux[index ] * TimeStep;
             // Zero the fluxes
		     GenericFlux[index] = 0;
         }
      }
   }
}

void TEcoDynClass::Integration(double *Generic, double *GenericFlux)
{
	int i, box;
    float timeStep = MyPEcoDynClass->GetTimeStep();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
	// Update the arrays
	for (i = 0; i < pSubDomain->NumberOfBoxes; i++)
	{
        box = pSubDomain->BoxNumber[i];
		Generic[box] += (GenericFlux[box] * timeStep);
    	// Zero the fluxes
		GenericFlux[box] = 0;
	}
}


void TEcoDynClass::Integration(double *A, double *AFlux,
                                   double *B, double *BFlux,
                                   double *C, double *CFlux,
                                   double *D, double *DFlux,
                                   double *E, double *EFlux)
{
	int i, box;
    float timeStep = MyPEcoDynClass->GetTimeStep();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
	// Update the arrays
	for (i = 0; i < pSubDomain->NumberOfBoxes; i++)
	{
        box = pSubDomain->BoxNumber[i];
		A[box] += (AFlux[box] * timeStep);
		B[box] += (BFlux[box] * timeStep);
		C[box] += (CFlux[box] * timeStep);
		D[box] += (DFlux[box] * timeStep);
		E[box] += (EFlux[box] * timeStep);
    	// Zero the fluxes
		AFlux[box] = 0;
		BFlux[box] = 0;
		CFlux[box] = 0;
		DFlux[box] = 0;
		EFlux[box] = 0;
	}
}

void TEcoDynClass::Integration(int NumberOfClasses)
{
	int k, index;
	// Update the array
	for (k = 0; k < NumberOfClasses; k++)
		for (int i = 0; i < NumberOfBoxes; i++)
	    {
	    	index = k * NumberOfBoxes + i;
			Generic[index] = Generic[index]
											+ GenericFlux[index]
											* TimeStep;
        	// Zero the fluxes
			GenericFlux[index] = 0;
	    }
}

void TEcoDynClass::Integration(int NumberOfClasses, double *Generic, double *GenericFlux)
{
    int k, index;
    float timeStep = MyPEcoDynClass->GetTimeStep();
    // Update the array
    for (k = 0; k < NumberOfClasses; k++)
        for (int i = 0; i < NumberOfBoxes; i++)
        {
            index = k * NumberOfBoxes + i;
                    Generic[index] = Generic[index] + GenericFlux[index]* timeStep;
            // Zero the fluxes
            GenericFlux[index] = 0;
        }
}

void TEcoDynClass::Integration(int NumberOfLayers, int ANumberOfBoxes, SedimentsRecord *ASedimentsRecord)
{
	int i, j, Index;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

        for (int m = 0; m < ANumberOfBoxes; m++)
        {
                i = ASedimentsRecord[m].LineCoordinate;
                j = ASedimentsRecord[m].ColumnCoordinate;
                if (
                        (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j] /*- 1*/) &&
                        (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i] /*- 1*/)
                   )
                {
                        for (int Layer = 0; Layer < NumberOfLayers; Layer++)
                        {
                                Index = Get3DIndex(i, j, Layer);
		                Generic[Index] = Generic[Index] + GenericFlux[Index] * TimeStep;
                                GenericFlux[Index] = 0; // Zero the fluxes
                        }
                }
        }
}

void TEcoDynClass::Integration(double AStep)
{
	int i;
	float MyStep = AStep;
	for (i = 0; i < NumberOfBoxes; i++)
	{
		Generic[i] = Generic[i]	+ GenericFlux[i] * MyStep;
    	// Zero the fluxes
		GenericFlux[i] = 0;
	}
}

void TEcoDynClass::IntegrationTridag (int n)
// Press et al., 1992. Numerical recipes in C (pages 50 - 51)
{
	int j;
	double bet;

	if (b[0] == 0.0) {
#ifdef __BORLANDC__
		MessageBox(0, "Error 1 in tridag (Press et al., NRC p. 51)",
					  "EcoDynamo alert",
					  MB_OK);
#else		// __BORLANDC__
				cerr << "TEcoDynClass::IntegrationTridag - Error 1 in tridag (Press et al., NRC p. 51)" << endl;
#endif // __BORLANDC__
        return;
    }
    	bet = b[0];
	u[0] = r[0] / bet;
	for (j = 1; j <= (n - 1); j++)      // Decomposition and forward substitution.
	{
		gam[j] = c[j - 1] / bet;
		bet = b[j] - a[j] * gam[j];

		if (bet == 0.0)
        {
#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
      	    Debugger(j); Debugger(a[j]);Debugger(b[j]); Debugger(gam[j]);Debugger(c[j - 1]);

            if ( Application->MessageBox("Error 2 in tridag (Press et al., NRC p. 51)","EcoDynamo Message", MB_OKCANCEL) != IDOK)
                throw;
#else		// __BORLANDC__
				cerr << "TEcoDynClass::IntegrationTridag - Error 2 in tridag (Press et al., NRC p. 51)" << endl;
#endif // __BORLANDC__
#endif 	// !(_PORT_FORTRAN_)
            return;
        }
		u[j] = (r[j] - a[j] * u[j-1]) / bet;
	}
	for (j = (n - 2); j >= 0; j--)
	{
		u[j] = u[j] - (gam[j + 1] * u[j + 1]);     // Backsubstitution.
	}
}
void TEcoDynClass::IntegrationTridag (double  a[], double  b[], double  c[],
										double  r[], int n)
//#endif
// Press et al., 1992. Numerical recipes in C (pages 50 - 51)
{
	int j;
	double bet;
	if (b[0] == 0.0) {
#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
		MessageBox(0, "Error 1 in tridag (Press et al., NRC p. 51)",
					  "EcoDynamo alert",
					  MB_OK);
#else		// __BORLANDC__
				cerr << "TEcoDynClass::IntegrationTridag(2) - Error 1 in tridag (Press et al., NRC p. 51)" << endl;
#endif // __BORLANDC__
#endif 	// !(_PORT_FORTRAN_)
        return;
    }
   	bet = b[0];
	u[0] = r[0] / bet;
	for (j = 1; j <= (n - 1); j++)      // Decomposition and forward substitution.
	{
		gam[j] = c[j - 1] / bet;
		bet = b[j] - a[j] * gam[j];
		if (bet == 0.0)
        {
#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
      	    Debugger(j); Debugger(a[j]);Debugger(b[j]); Debugger(gam[j]);Debugger(c[j - 1]);
            
		    if ( Application->MessageBox("Error 2 in tridag (Press et al., NRC p. 51)","EcoDynamo Message", MB_OKCANCEL) != IDOK)
                throw;
#else		// __BORLANDC__
				cerr << "TEcoDynClass::IntegrationTridag(2) - Error 2 in tridag (Press et al., NRC p. 51)" << endl;
#endif // __BORLANDC__
#endif 	// !(_PORT_FORTRAN_)
            return;
        }
		u[j] = (r[j] - a[j] * u[j-1]) / bet;
	}
	for (j = (n - 2); j >= 0; j--)
	{
		u[j] = u[j] - (gam[j + 1] * u[j + 1]);     // Backsubstitution.
	}
}

void TEcoDynClass::mprovetridag(double a[], double b[], double c[], double r[], double u[], int n)
{
   double sdp;
   double *Residual, *Oldu;
   Residual = (double *)calloc(n, sizeof(double));
   Oldu = (double *)calloc(n, sizeof(double));
   for (int i = 0; i < n; i++)
   {
   	  Oldu[i] = u[i];
      sdp = -r[i];
      if (i == 0)
      	 sdp += b[i] * u[i] + c[i] * u[i + 1];
      else
      if ((i > 0) && (i < n - 1))
         sdp += a[i] * u[i - 1] + b[i] * u[i] + c[i] * u[i + 1];
      else
      if (i == n - 1)
         sdp += a[i] * u[i - 1] + b[i] * u[i];
      Residual[i] = sdp;
   }
   IntegrationTridag(a, b, c, Residual, n);
   for (int i = 0; i < n; i++)
   {
   	  Oldu[i] = Oldu[i] - u[i];
      u[i] = Oldu[i];
   }
   free(Residual);
   free(Oldu);
}

void TEcoDynClass::IntegrationBandec(double **a, unsigned long n, int m1, int m2, double **a1, unsigned long indx[], float *d)
{
}

/* This is a C version of the codes given on the class
homepage.  They have been tested using a 7x7 and a 5x5 matrice.
There are a few minor changes.  The matrix A is now a vector A of
size N^2.  The variable N does not refer to the number of grid points
along the x or y axis, it instead refers to the total number of grid
points in the problem.  So if you split your 2-D area into a 11x11
grid, N would equal 121.

Courtesy: Travis Grove
*/
void TEcoDynClass::Ludcmp(int N, double A[], int Indx[])
{
	int i, j, k, D, Imax;
	double Tiny, Dum, sum, AAmax;
	double *VV;

	VV = (double *)calloc(N, sizeof(double));
	Tiny=1E-20;

	D=1;


	for (i=0; i<N; i++)
	{
		AAmax=0;
       	for(j=0; j<N; j++)
   	    {
   		    if(fabs(A[i*N+j])>AAmax)
      	    {
   			    AAmax=fabs(A[i*N+j]);
      	    }
   	    }
   	    if(AAmax==0)
   	    {
#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
   		    MessageBox(0,
					  "Error in Ludcmp - Singular matrix (Press et al., NRC p. 46)",
					  "EcoDynamo alert",
					  MB_OK);
#else // __BORLANDC__
					cerr << "Error in TEcoDynClass::Ludcmp - Singular matrix (Press et al., NRC p. 46)" << endl;
#endif  // __BORLANDC__
#endif // !(_PORT_FORTRAN_)
            return;
       	}
   	    VV[i]=(double) 1/AAmax;
	}

	for(j=0; j<N; j++)
	{
       	if(j>0)
        {
      	    for(i=0; i<j; i++)
            {
         	    sum=A[i*N+j];
                if(i>0)
                {
            	    for(k=0; k<i; k++)
                    {
               	        sum=sum-A[i*N+k]*A[k*N+j];
                    }
                    A[i*N+j]=sum;
                }
            }
        }
        AAmax=0;
        for(i=j; i<N; i++)
        {
      	    sum=A[i*N+j];
            if(j>0)
            {
         	    for(k=0; k<j; k++)
                {
            	    sum=sum-A[i*N+k]*A[k*N+j];
                }
                A[i*N+j]=sum;
            }
            Dum=VV[i]*fabs(sum);
            if(Dum>=AAmax)
            {
         	    Imax=i;
                AAmax=Dum;
            }
        }
        if(j!=Imax)
        {
      	    for(k=0; k<N; k++)
            {
         	    Dum=A[Imax*N+k];
                A[Imax*N+k]=A[j*N+k];
                A[j*N+k]=Dum;
            }
            D=-D;
            VV[Imax]=VV[j];
        }
        Indx[j]=Imax;
        if(j!=N-1)
        {
      	    if(A[j*N+j]==0)
            {
         	    A[j*N+j]=Tiny;
            }
            Dum=(double)1/A[j*N+j];
            for(i=j+1; i<N; i++)
            {
         	    A[i*N+j]=A[i*N+j]*Dum;
            }
        }
	} //j

	if(A[N*N-1]==0)
	{
   	    A[N*N-1]=Tiny;
	}
	free(VV);

	return;
}

/* This is a C version of the codes given on the class
homepage.  They have been tested using a 7x7 and a 5x5 matrice.
There are a few minor changes.  The matrix A is now a vector A of
size N^2.  The variable N does not refer to the number of grid points
along the x or y axis, it instead refers to the total number of grid
points in the problem.  So if you split your 2-D area into a 11x11
grid, N would equal 121.

Courtesy: Travis Grove
*/
void TEcoDynClass::Lubksb(int N, double A[], int Indx[], double B[])
{
    int ii,i,j,ll;
	double sum;


	ii=-1;

	for(i=0; i<N; i++)
	{
   	    ll=Indx[i];
        sum=B[ll];
        B[ll]=B[i];
        if(ii!=-1)
        {
      	    for(j=ii; j<i; j++)
            {
         	    sum=sum-A[i*N+j]*B[j];
            }
        }
        else if(sum!=0)
        {
     	    ii=i;
        }
        B[i]=sum;
	}

	for(i=N-1; i>=0; i--)
	{
		sum=B[i];
        if(i<N-1)
        {
      	    for(j=i+1; j<N; j++)
            {
         	    sum=sum-A[i*N+j]*B[j];
            }
        }
        B[i]=sum/A[i*N+i];
	}
    return;
}


void TEcoDynClass::Calendar()
{
    if (MyTime > 0)
    {
        MyTime = MyTime + TimeStep / DAYSTOSECONDS;
    }
    else
    {
        MyTime = 1;
    }
	CurrentTime = CurrentTime + TimeStep / HOURSTOSECONDS;
	if (floor(CurrentTime) >= 24)
	{
		CurrentTime = CurrentTime-24;
		JulianDay++;
		Date++;
	}
	if (Date > DaysInMonth[Month-1])
	{
		if (Month == 12) {
            Year++;
			Month = 1;
            Date = 1;
            JulianDay = 1;
        }
		else if ((Month == 2) && (Date == 29)) {
            if ((Year % 4) != 0) {  // not leap year
    			Month++;
                Date = 1;
            }
        }
        else {
			Month++;
    		Date = 1;
        }
	}
}

char* TEcoDynClass::GetAvailableVariables(int i)
{
    if (VariableNameArray == NULL)
        return "";
    return VariableNameArray[i];
}

char* TEcoDynClass::GetAvailableParameters(int i)
{
    if (ParametersNameArray == NULL)
        return "";
        
    return ParametersNameArray[i];
}

#ifndef _PORT_FORTRAN_
bool TEcoDynClass::OpenMorphology()
{
  //cout << "Open morphology 1" << endl;
   return OpenMorphology(GetMorphologyFile());
   // cout << "Open morphology 10" << endl;
}

bool TEcoDynClass::OpenMorphology(char* FileName)
{
    char MorphologyFileName[129];
    int X,Y;
    int XCol, XLin, XDep, XLen, XWid, XEle, XTyp, 
		XNbo, XEbo, XSbo, XWbo, XLay, XLatitude, XLongitude;
    strcpy(MorphologyFileName, FileName);
    LatitudeBoolean = true; LongitudeBoolean = true;
    
    TReadWrite* PReadWrite = new TReadWrite(MorphologyFileName);
    
    if (!PReadWrite->SetupFile(READFILE))
    {
        PReadWrite->CloseFile();
        return false;
    }
    
    // AP, 030723..
    NumberOfLines = NumberOfColumns = NumberOfLayers = NumberOfBoxes = 1;
    if (PReadWrite->FindString("NumberOfLines", X, Y))
        PReadWrite->ReadNumber(X + 1, Y, NumberOfLines);
   
    if (PReadWrite->FindString("NumberOfColumns", X, Y))
        PReadWrite->ReadNumber(X + 1, Y, NumberOfColumns);
  
    if (PReadWrite->FindString("NumberOfLayers", X, Y))
    {
        PReadWrite->ReadNumber(X + 1, Y, NumberOfLayers);
    }
     
    /* AP, 070614...
    PReadWrite->FindString("NumberOfBoxes", X, Y);
    PReadWrite->ReadNumber(X + 1, Y, NumberOfBoxes);
    */
    
    NumberOfBoxes = NumberOfLines * NumberOfColumns * NumberOfLayers;
   
    // ... AP, 070614
   
    BoxArray = new TBoxes::BoxRecord[NumberOfBoxes];
    BoxArrayAllocated = true;
    
    // AP, 050309
    if (PReadWrite->FindString("Critical depth for land boundary", X, Y))
        PReadWrite->ReadNumber(X + 1, Y, ECDCriticalDepthForLandBoundary);
    
    // AP, 070511
    if (PReadWrite->FindString("ModelType", X, Y))
        PReadWrite->ReadString(X + 1, Y, modelType);
   
    if (!PReadWrite->FindString("Latitude", XLatitude, Y)) {
        //PReadWrite->CloseFile();
        LatitudeBoolean = false;
    }
    
    if (!PReadWrite->FindString("Longitude", XLongitude, Y)) {
        //PReadWrite->CloseFile();
        LongitudeBoolean = false;
    }
    
    // Verify if all mandatory columns exist...
    //
    if (!PReadWrite->FindString("Columns", XCol, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
   
    if (!PReadWrite->FindString("Lines", XLin, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
   
    if (NumberOfLayers > 1)
    {
        if (!PReadWrite->FindString("Layers", XLay, Y)) {
            PReadWrite->CloseFile();
            return false;
        }
    }
   
    if (!PReadWrite->FindString("BoxDepth", XDep, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("BoxLength", XLen, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("BoxWidth", XWid, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("BoxElevation", XEle, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("BoxType", XTyp, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("Nboundary", XNbo, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("Eboundary", XEbo, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("Sboundary", XSbo, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("Wboundary", XWbo, Y)) {
        PReadWrite->CloseFile();
        return false;
    }


    Y++;
    // cout << "Open morphology 6" << endl;
    // cout << NumberOfBoxes << endl;
    int line, column, index, layer;
    NumberOfSeaBoundaries = 0;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        if (i >= 916023) cout << i << endl;
        PReadWrite->ReadNumber(XCol, Y + i, column);
        if (i >= 916023) cout << XCol << endl;
        if (i >= 916023) cout << Y << endl;
        if (i >= 916023) cout << column << endl;
        if (i >= 916023) cout << XLin << endl;
        if (i >= 916023) cout << "Fim" << endl;
        PReadWrite->ReadNumber(XLin, Y + i, line);
        
        if (i >= 916023) cout << "Fimline2" << endl;

        if (NumberOfLayers == 1)
            index = GetBoxNumber(column, line);
        else {
            PReadWrite->ReadNumber(XLay, Y + i, layer);
            //if (i >= 916023) cout << layer << endl;
            index = GetBoxNumber(column, line, layer/*NumberOfLayers-layer+1*/);
            
        }
        //if (i >= 916023) cout << index << endl;
        PReadWrite->ReadNumber(XDep, Y + i, BoxArray[index].Depth);//cout << XDep, Y + 1,BoxArray[index].Depth << endl;
        PReadWrite->ReadNumber(XLen, Y + i, BoxArray[index].Length);
        PReadWrite->ReadNumber(XWid, Y + i, BoxArray[index].Width);
        PReadWrite->ReadNumber(XEle, Y + i, BoxArray[index].Elevation);
        PReadWrite->ReadNumber(XTyp, Y + i, BoxArray[index].Type);
        PReadWrite->ReadNumber(XNbo, Y + i, BoxArray[index].BoxToBoundary[1].TypeOfBoundary);
        PReadWrite->ReadNumber(XEbo, Y + i, BoxArray[index].BoxToBoundary[2].TypeOfBoundary);
        PReadWrite->ReadNumber(XSbo, Y + i, BoxArray[index].BoxToBoundary[3].TypeOfBoundary);
        PReadWrite->ReadNumber(XWbo, Y + i, BoxArray[index].BoxToBoundary[4].TypeOfBoundary);

        if (LatitudeBoolean)
			PReadWrite->ReadNumber(XLatitude, Y + i, BoxArray[index].YPosition);
        if (LongitudeBoolean)
			PReadWrite->ReadNumber(XLongitude, Y + i, BoxArray[index].XPosition);

        BoxArray[index].Dummybool1 = true;
        /*if (i >= 916023)
	{
	   cout << BoxArray[index].Depth << endl;
           cout << BoxArray[index].Length << endl;
           cout << BoxArray[index].Width << endl;
           cout << BoxArray[index].Elevation  << endl;
           cout << BoxArray[index].Type << endl;
           cout << BoxArray[index].BoxToBoundary[1].TypeOfBoundary << endl;
           cout << BoxArray[index].BoxToBoundary[2].TypeOfBoundary << endl;
           cout << BoxArray[index].BoxToBoundary[3].TypeOfBoundary << endl;
           cout << BoxArray[index].BoxToBoundary[4].TypeOfBoundary << endl;
           cout << BoxArray[index].YPosition << endl;
           cout << BoxArray[index].XPosition << endl;
	   }*/
        if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) ||
            (BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) ||
            (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||
            (BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2))
             NumberOfSeaBoundaries++;
    }
    PReadWrite->CloseFile();
    // cout << "Open morphology 7" << endl;
    if (NumberOfLayers == 1){
      for (int i = 0; i < NumberOfLines; i++)
      {
         for (int j = 0; j < NumberOfColumns; j++)
         {
            index = i * NumberOfColumns + j;
            if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 1) && (i < NumberOfLines - 1))
               BoxArray[(i + 1) * NumberOfColumns + j].Dummybool1 = false;                              //River to the North
            if ((BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 1) && (j < NumberOfColumns - 1))
               BoxArray[index + 1].Dummybool1 = false;                                //River to the East
            if ((BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 1) && (i > 0))
               BoxArray[(i - 1) * NumberOfColumns + j].Dummybool1 = false;                              //River to the South
            if ((BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 1) && (j > 0))
               BoxArray[index - 1].Dummybool1 = false;                                //River to the West
            if (BoxArray[index].Depth < 0.0)
               BoxArray[index].Dummybool1 = false; //Nesta linha "isolam-se" as células com profundidades negativas
                   //pois quando o Dummybool1 é "false" as células são tratadas como rios. O ideal seria usar o DummyBool2, tal como em baixo
                   //para modelos 3D. No entanto, não faz diferença, pois nos modelos 2D, as células com o DummyBool "false" são totalmente
                   //ignoradas nos cálculos
         }
      }
    }
    else
    {
      for (int i = 0; i < NumberOfLines; i++)
      {
         for (int j = 0; j < NumberOfColumns; j++)
         {
            for (int k = 0; k < NumberOfLayers; k++)
            {
                index = Get3DIndex(i,j,k);
                if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 1) && (i < NumberOfLines - 1))
                   BoxArray[Get3DIndex(i+1,j,k)].Dummybool1 = false;                              //River to the North
                if ((BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 1) && (j < NumberOfColumns - 1))
                   BoxArray[Get3DIndex(i,j+1,k)].Dummybool1 = false;                                //River to the East
                if ((BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 1) && (i > 0))
                   BoxArray[Get3DIndex(i-1,j,k)].Dummybool1 = false;                              //River to the South
                if ((BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 1) && (j > 0))
                   BoxArray[Get3DIndex(i,j-1,k)].Dummybool1 = false;                                //River to the West
                if (BoxArray[index].Depth < 0.0)
                   BoxArray[index].Dummybool2 = false;  //Nesta linha "isolam-se" as células com profundidades negativas
                   //pois quando o Dummybool2 é "false" as células são tratadas como terra.
            }
         }
      }
    }
    return true;
}

bool TEcoDynClass::OpenLoads()
{
    return OpenLoads(GetLoadsFile());
}

bool TEcoDynClass::OpenLosses()
{
    return OpenLosses(GetLossesFile());
}

bool TEcoDynClass::OpenRivers()
{
    return OpenRivers(GetRiversFile());
}

bool TEcoDynClass::OpenSeaBoundaries()
{
    return OpenSeaBoundaries(GetSeaBoundariesFile());
}

bool TEcoDynClass::OpenSediments()
{
    return OpenSediments(GetSedimentsFile());
}

bool TEcoDynClass::OpenBenthicSpecies()
{
    return OpenBenthicSpecies(GetBenthicSpeciesFile());
}

bool TEcoDynClass::OpenIce()
{
    return OpenIce(GetIceFile());
}

bool TEcoDynClass::OpenLoads(char* FileName)
{
    char LoadsFileName[129], flowStr[16];
    int X, Y, XLL, YLL, XLC, YLC, XLN, YLN, XF, YF, XLY, YLY;
    int NumberOfMomentsForLoads;
    bool Days, Hours;
    Days = false; Hours = false;
    NumberOfLoads = 0;
    NumberOfDaysForLoads = 0; NumberOfHoursForLoads = 0; NumberOfMomentsForLoads = 0;
    strcpy(LoadsFileName, FileName);

    TReadWrite* PReadWrite = new TReadWrite(LoadsFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        PReadWrite->CloseFile();
        return false;
    }
    char LoadName[100];


    if (PReadWrite->FindString("NumberOfLoads", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, NumberOfLoads);
    else {
        PReadWrite->CloseFile();
        return false;
    }

    if (PReadWrite->FindString("NumberOfDaysForLoads", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfDaysForLoads);
        Days = true;
        NumberOfMomentsForLoads = NumberOfDaysForLoads;
    }
    else
    if (PReadWrite->FindString("NumberOfHoursForLoads", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfHoursForLoads);
        Hours = true;
        NumberOfMomentsForLoads = NumberOfHoursForLoads;
    }
    else
    {
        PReadWrite->CloseFile();
        return false;
    }

    if (NumberOfLoads > 0)
    {
        ALoadRecord = new LoadsRecord[NumberOfLoads];


/*+++ AP, 050105*/
        if (!PReadWrite->FindString("LoadLines", XLL, YLL))
        {
            PReadWrite->CloseFile();
            return false;
        }

        if (!PReadWrite->FindString("LoadColumns", XLC, YLC))
        {
            PReadWrite->CloseFile();
            return false;
        }

        if (!PReadWrite->FindString("LoadName", XLN, YLN))
        {
            PReadWrite->CloseFile();
            return false;
        }
        PReadWrite->FindString("LoadLayers", XLY, YLY);
        // if not found fills XLY and YLY with -1

        for (int j = 0; j < NumberOfLoads; j++)
        {
            ALoadRecord[j].LoadFlow = new double[NumberOfMomentsForLoads];

            PReadWrite->ReadNumber(XLL, YLL + 1 + j, ALoadRecord[j].LineCoordinate);
            ALoadRecord[j].LineCoordinate = NumberOfLines - ALoadRecord[j].LineCoordinate;
            PReadWrite->ReadNumber(XLC, YLC + 1 + j, ALoadRecord[j].ColumnCoordinate);
            ALoadRecord[j].ColumnCoordinate = ALoadRecord[j].ColumnCoordinate - 1;
            if (XLY != -1)
            {
                PReadWrite->ReadNumber(XLY, YLY + 1 + j, ALoadRecord[j].LayerCoordinate);
                ALoadRecord[j].LayerCoordinate = NumberOfLayers - ALoadRecord[j].LayerCoordinate;
            }
            PReadWrite->ReadString(XLN, YLN + 1 + j, LoadName);
            strcpy(ALoadRecord[j].LoadName, LoadName);

            sprintf(flowStr, "Flow%-d", j + 1);
            PReadWrite->FindString(flowStr, XF, YF);
            // if not found fills XF and YF with -1

            for (int Loads = 0; Loads < NumberOfMomentsForLoads; Loads++)
            {
                ALoadRecord[j].LoadFlow[Loads] = 0.0;
                if (XF > -1)
                    PReadWrite->ReadNumber(XF, YF + 1 + Loads, ALoadRecord[j].LoadFlow[Loads]);
            }

        }
        LoadsExist = true;
/*--- AP, 050105*/
    }
    PReadWrite->CloseFile();
    return true;
}

bool TEcoDynClass::OpenLosses(char* FileName)
{
    char LossesFileName[129], dischStr[16];
    int X, Y, XLL, YLL, XLC, YLC, XLN, YLN, XF, YF, XLY, YLY;
    int NumberOfMomentsForLosses;
    bool Days, Hours;
    Days = false; Hours = false;
    NumberOfLosses = 0;
    NumberOfDaysForLosses = 0; NumberOfHoursForLosses = 0; NumberOfMomentsForLosses = 0;
    strcpy(LossesFileName, FileName);

    TReadWrite* PReadWrite = new TReadWrite(LossesFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        PReadWrite->CloseFile();
        return false;
    }
    char LossName[100];


    if (PReadWrite->FindString("NumberOfLosses", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, NumberOfLosses);
    else {
         PReadWrite->CloseFile();
         return false;
    }
    if (PReadWrite->FindString("NumberOfDaysForLosses", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfDaysForLosses);
        Days = true;
        NumberOfMomentsForLosses = NumberOfDaysForLosses;
        //Debugger(NumberOfDaysForLosses);
    }
    else
    if (PReadWrite->FindString("NumberOfHoursForLosses", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfHoursForLosses);
        Hours = true;
        NumberOfMomentsForLosses = NumberOfHoursForLosses;
    }
    else
    {
       PReadWrite->CloseFile();
       return false;
    }
    if (NumberOfLosses > 0)
    {
        ALossesRecord = new LossesRecord[NumberOfLosses];

        if (!PReadWrite->FindString("LossesLines", XLL, YLL))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("LossesColumns", XLC, YLC))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("LossesName", XLN, YLN))
        {
            PReadWrite->CloseFile();
            return false;
        }
        PReadWrite->FindString("LossesLayers", XLY, YLY);
        // if not found fills XLY and YLY with -1

        for (int j = 0; j < NumberOfLosses; j++)
        {
            ALossesRecord[j].LossFlow = new double[NumberOfMomentsForLosses];

            PReadWrite->ReadNumber(XLL, YLL + 1 + j, ALossesRecord[j].LineCoordinate);
            ALossesRecord[j].LineCoordinate = NumberOfLines - ALossesRecord[j].LineCoordinate;
            PReadWrite->ReadNumber(XLC, YLC + 1 + j, ALossesRecord[j].ColumnCoordinate);
            ALossesRecord[j].ColumnCoordinate = ALossesRecord[j].ColumnCoordinate - 1;
            if (XLY != -1)
            {
                PReadWrite->ReadNumber(XLY, YLY + 1 + j, ALossesRecord[j].LayerCoordinate);
                ALossesRecord[j].LayerCoordinate = NumberOfLayers - ALossesRecord[j].LayerCoordinate;
            }
            PReadWrite->ReadString(XLN, YLN + 1 + j, LossName);
            strcpy(ALossesRecord[j].LossesName, LossName);

            sprintf(dischStr, "Disch%-d", j + 1);
            PReadWrite->FindString(dischStr, XF, YF);
            for (int Losses = 0; Losses < NumberOfMomentsForLosses; Losses++)
            {
               ALossesRecord[j].LossFlow[Losses] = 0.0;
               if (XF > -1)
                   PReadWrite->ReadNumber(XF, YF + 1 + Losses, ALossesRecord[j].LossFlow[Losses]);
            }
        }
        LossesExist = true;
    }
    PReadWrite->CloseFile();
    return true;
}

bool TEcoDynClass::OpenRivers(char* FileName)
{
    char RiversFileName[129], flowStr[16];
    int X, Y, XFL, YFL, XFC, YFC, XB, YB, XRN, YRN, XF, YF, XLY, YLY;
    int NumberOfMomentsForFlowTimeSeries;
    bool Days, Hours, Layers;
    Days = false; Hours = false; //Layers = false;
    NumberOfRivers = 0;
    NumberOfDaysForFlowTimeSeries = 0; NumberOfHoursForFlowTimeSeries = 0; NumberOfMomentsForFlowTimeSeries = 0;

    strcpy(RiversFileName, FileName);

    TReadWrite* PReadWrite = new TReadWrite(RiversFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        return false;
    }
    char RiverName[100];


    if (PReadWrite->FindString("NumberOfRivers", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, NumberOfRivers);

    if (PReadWrite->FindString("NumberOfDaysForFlowTimeSeries", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfDaysForFlowTimeSeries);
        Days = true;
        NumberOfMomentsForFlowTimeSeries = NumberOfDaysForFlowTimeSeries;
    }
    else
    if (PReadWrite->FindString("NumberOfHoursForFlowTimeSeries", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfHoursForFlowTimeSeries);
        Hours = true;
        NumberOfMomentsForFlowTimeSeries = NumberOfHoursForFlowTimeSeries;
        //Debugger(NumberOfHoursForFlowTimeSeries);
    }
    if (NumberOfRivers > 0)
    {
        ARiverRecord = new RiversRecord[NumberOfRivers];

        for (int j = 0; j < NumberOfRivers; j++)
            ARiverRecord[j].RiverFlow = new double[NumberOfMomentsForFlowTimeSeries];

        for (int i = 0; i < NumberOfRivers; i++)
        {
            ARiverRecord[i].LineCoordinate = 0; ARiverRecord[i].ColumnCoordinate = 0;
            ARiverRecord[i].LayerCoordinate = 0; ARiverRecord[i].HasLayers = false;
        }
        if (!PReadWrite->FindString("InputFlowLines", XFL, YFL))
            return false;

        if (!PReadWrite->FindString("InputFlowColumns", XFC, YFC))
            return false;
        if (!PReadWrite->FindString("Boundary", XB, YB))
            return false;

        if (!PReadWrite->FindString("RiverName", XRN, YRN))
            return false;

        PReadWrite->FindString("InputFlowLayers", XLY, YLY);
        // if not found fills XLY and YLY with -1

        for (int j = 0; j < NumberOfRivers; j++)
        {
            PReadWrite->ReadNumber(XFL, YFL + 1 + j, ARiverRecord[j].LineCoordinate);
            ARiverRecord[j].LineCoordinate = NumberOfLines - ARiverRecord[j].LineCoordinate;
            PReadWrite->ReadNumber(XFC, YFC + 1 + j, ARiverRecord[j].ColumnCoordinate);
            ARiverRecord[j].ColumnCoordinate = ARiverRecord[j].ColumnCoordinate - 1;
            if (XLY != -1)
            {
                ARiverRecord[j].HasLayers = true;
                PReadWrite->ReadNumber(XLY, YLY + 1 + j, ARiverRecord[j].LayerCoordinate);
                ARiverRecord[j].LayerCoordinate = NumberOfLayers - ARiverRecord[j].LayerCoordinate;
            }
            PReadWrite->ReadNumber(XB, YB + 1 + j, ARiverRecord[j].BoundaryNumber);
            PReadWrite->ReadString(XRN, YRN + 1 + j, RiverName);
            strcpy(ARiverRecord[j].RiverName, RiverName);

            sprintf(flowStr, "Flow%-d", j + 1);
            PReadWrite->FindString(flowStr, XF, YF);
            // if not found fills XF and YF with -1

            for (int flows = 0; flows < NumberOfMomentsForFlowTimeSeries; flows++)
            {
                ARiverRecord[j].RiverFlow[flows] = 0.0;
                if (XF > -1)
                    PReadWrite->ReadNumber(XF, YF + 1 + flows,
                            ARiverRecord[j].RiverFlow[flows]);
            }
        }
        RiversExist = true;
    }
    PReadWrite->CloseFile();
    return true;
}

bool TEcoDynClass::OpenSeaBoundaries(char* FileName)
{
    char /*SeaBoundariesFileName[129],*/ flowStr[16];
    int X, Y, XFL, YFL, XFC, YFC, XFLayer, YFLayer, XB, YB, XF, YF;
    int NumberOfMomentsForSeaBoundaries, index;
    bool Days, Hours, Layers;
    Days = false; Hours = false; //Layers = false;
    NumberOfDaysForSeaBoundaries = 0; NumberOfHoursForSeaBoundaries = 0; NumberOfMomentsForSeaBoundaries = 0;

    strcpy(SeaBoundariesFileName, FileName);

    TReadWrite* PReadWrite = new TReadWrite(SeaBoundariesFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        return false;
    }

    int MySeaBundaries;
    if (PReadWrite->FindString("NumberOfSeaBoundaries", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, MySeaBundaries);

    if (NumberOfSeaBoundaries != MySeaBundaries)
#ifdef __BORLANDC__
       MessageBox(0, "Error in OpenSeaBoundaries  - Morphology file sea boundaries not compatible with sea boundaries number",
					  "EcoDynamo alert",
					  MB_OK);
#else  // __BORLANDC__
        cerr << "TEcoDynClass::OpenSeaBoundaries - - Morphology file sea boundaries not compatible with sea boundaries number" << endl;
#endif  // __BORLANDC__
    if (PReadWrite->FindString("NumberOfDaysForSeaBoundaries", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfDaysForSeaBoundaries);
        Days = true;
        NumberOfMomentsForSeaBoundaries = NumberOfDaysForSeaBoundaries;
    }
    else
    if (PReadWrite->FindString("NumberOfHoursForSeaBoundaries", X, Y))
    {
        PReadWrite->ReadNumber(X, Y + 1, NumberOfHoursForSeaBoundaries);
        Hours = true;
        NumberOfMomentsForSeaBoundaries = NumberOfHoursForSeaBoundaries;
    }
    if (NumberOfSeaBoundaries > 0)
    {
        ASeaBoundariesRecord = new SeaBoundariesRecord[NumberOfSeaBoundaries];

        for (int j = 0; j < NumberOfSeaBoundaries; j++)
            ASeaBoundariesRecord[j].Velocity = new double[NumberOfMomentsForSeaBoundaries];

        for (int i = 0; i < NumberOfSeaBoundaries; i++)
        {
            ASeaBoundariesRecord[i].HasLayers = false;
            ASeaBoundariesRecord[i].LineCoordinate = 0;
            ASeaBoundariesRecord[i].ColumnCoordinate = 0;
            ASeaBoundariesRecord[i].LayerCoordinate = 0;
        }
        if (!PReadWrite->FindString("InputFlowLines", XFL, YFL))
            return false;

        if (!PReadWrite->FindString("InputFlowColumns", XFC, YFC))
            return false;

//        if (!PReadWrite->FindString("InputFlowLayers", XFLayer, YFLayer))
//            return false;
        PReadWrite->FindString("InputFlowLayers", XFLayer, YFLayer);
        // if not found fills XFLayer and YFLayer with -1

        if (!PReadWrite->FindString("Boundary", XB, YB))
            return false;

        for (int j = 0; j < NumberOfSeaBoundaries; j++)
        {
            int column, line, layer;
            PReadWrite->ReadNumber(XFL, YFL + 1 + j, line);
            ASeaBoundariesRecord[j].LineCoordinate = NumberOfLines - line;
            PReadWrite->ReadNumber(XFC, YFC + 1 + j, column);
            ASeaBoundariesRecord[j].ColumnCoordinate = column - 1;
            if (XFLayer != -1)
            {
                ASeaBoundariesRecord[j].HasLayers = true;
                PReadWrite->ReadNumber(XFLayer, YFLayer + 1 + j, layer);
                ASeaBoundariesRecord[j].LayerCoordinate = NumberOfLayers - layer;
            }
            PReadWrite->ReadNumber(XB, YB + 1 + j, ASeaBoundariesRecord[j].BoundaryNumber);
            if (NumberOfLayers == 1)
               index = ASeaBoundariesRecord[j].LineCoordinate * NumberOfColumns + ASeaBoundariesRecord[j].ColumnCoordinate;
            else
               index = Get3DIndex(ASeaBoundariesRecord[j].LineCoordinate,ASeaBoundariesRecord[j].ColumnCoordinate, ASeaBoundariesRecord[j].LayerCoordinate);

            // Force box boundary as sea boundary
            BoxArray[index].BoxToBoundary[ASeaBoundariesRecord[j].BoundaryNumber].TypeOfBoundary = 2;

            sprintf(flowStr, "Velocity%-d", j + 1);
            PReadWrite->FindString(flowStr, XF, YF);
            // if not found fills XF and YF with -1

            for (int flows = 0; flows < NumberOfMomentsForSeaBoundaries; flows++)
            {
                ASeaBoundariesRecord[j].Velocity[flows] = 0.0;
                if (XF > -1)
                    PReadWrite->ReadNumber(XF, YF + 1 + flows,
                            ASeaBoundariesRecord[j].Velocity[flows]);
            }
        }
        SeaBoundariesExist = true;
    }
    PReadWrite->CloseFile();
    return true;
}

bool TEcoDynClass::OpenSediments(char* FileName)
{
    char SedimentsFileName[129];
    int X,Y;
    int XName, XLine, XColumn, XDensity, XPorosity, XLHeight, XSWRatio;

    NumberOfCellsWithSediments = 0;
    strcpy(SedimentsFileName, FileName);

    TReadWrite* PReadWrite = new TReadWrite(SedimentsFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        PReadWrite->CloseFile();
        return false;
    }
    char SedimentName[200];

    if (PReadWrite->FindString("NumberOfCellsWithSediments", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, NumberOfCellsWithSediments);
    if (NumberOfCellsWithSediments > 0)
    {
        ASedimentsRecord = new SedimentsRecord[NumberOfCellsWithSediments];

        if (!PReadWrite->FindString("SedimentName", XName, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("SedimentLines", XLine, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("SedimentColumns", XColumn, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("Density", XDensity, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("Porosity", XPorosity, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("LayerHeight", XLHeight, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("SedimentWaterRatio", XSWRatio, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }

        for (int j = 0; j < NumberOfCellsWithSediments; j++)
        {
            PReadWrite->ReadString(XName, Y + 1 + j, SedimentName);
            strcpy(ASedimentsRecord[j].SedimentName, SedimentName);

            PReadWrite->ReadNumber(XLine, Y + 1 + j, ASedimentsRecord[j].LineCoordinate);
            ASedimentsRecord[j].LineCoordinate = NumberOfLines-ASedimentsRecord[j].LineCoordinate;

            PReadWrite->ReadNumber(XColumn, Y + 1 + j, ASedimentsRecord[j].ColumnCoordinate);
            ASedimentsRecord[j].ColumnCoordinate = ASedimentsRecord[j].ColumnCoordinate - 1;

            PReadWrite->ReadNumber(XDensity, Y + 1 + j, ASedimentsRecord[j].Density);
            PReadWrite->ReadNumber(XPorosity, Y + 1 + j, ASedimentsRecord[j].Porosity);
            PReadWrite->ReadNumber(XLHeight, Y + 1 + j, ASedimentsRecord[j].LayerHeight);
            PReadWrite->ReadNumber(XSWRatio, Y + 1 + j, ASedimentsRecord[j].SedimentWaterRatio);
        }
        SedimentsExist = true;
    }
    PReadWrite->CloseFile();
    return true;
}

bool TEcoDynClass::OpenBenthicSpecies(char* FileName)
{
    char BenthicSpeciesFileName[129];
    int X,Y;
    int XName, XLine, XColumn, XDensity, XPorosity, XLHeight, XSWRatio;

    NumberOfCellsWithBenthicSpecies = 0;
    strcpy(BenthicSpeciesFileName, FileName);

    TReadWrite* PReadWrite = new TReadWrite(BenthicSpeciesFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        PReadWrite->CloseFile();
        return false;
    }
    char BenthicSpeciesName[200];

    if (PReadWrite->FindString("NumberOfCellsWithBenthicSpecies", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, NumberOfCellsWithBenthicSpecies);
    if (NumberOfCellsWithBenthicSpecies > 0)
    {
        ABenthicSpeciesRecord = new BenthicSpeciesRecord[NumberOfCellsWithBenthicSpecies];
        BenthicSpeciesRecordAllocated = true;

        if (!PReadWrite->FindString("SpeciesName", XName, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("LineCoordinate", XLine, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("ColumnCoordinate", XColumn, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        for (int j = 0; j < NumberOfCellsWithBenthicSpecies; j++)
        {
            PReadWrite->ReadString(XName, Y + 1 + j, BenthicSpeciesName);
            strcpy(ABenthicSpeciesRecord[j].SpeciesName, BenthicSpeciesName);

            PReadWrite->ReadNumber(XLine, Y + 1 + j, ABenthicSpeciesRecord[j].LineCoordinate);
            ABenthicSpeciesRecord[j].LineCoordinate = NumberOfLines-ABenthicSpeciesRecord[j].LineCoordinate;

            PReadWrite->ReadNumber(XColumn, Y + 1 + j, ABenthicSpeciesRecord[j].ColumnCoordinate);
            ABenthicSpeciesRecord[j].ColumnCoordinate = ABenthicSpeciesRecord[j].ColumnCoordinate - 1;
        }
        BenthicSpeciesExist = true;
    }
    PReadWrite->CloseFile();
    return true;
}

bool TEcoDynClass::OpenIce(char* FileName)
{
    char IceFileName[129];
    int X,Y;
    int XName, XLine, XColumn, Xthikness, Ythikness, XCongelationLayers, XSkeletalLayers, XPlateletIceLayers;
    XName = 0; XLine = 0; XColumn = 0; Xthikness = 0; Ythikness = 0; XCongelationLayers = 0; XSkeletalLayers = 0; XPlateletIceLayers = 0;
    NumberOfCellsWithIce = 0;  IceGrid = 0;
    strcpy(IceFileName, FileName);
    TReadWrite* PReadWrite = new TReadWrite(IceFileName);
    if (!PReadWrite->SetupFile(READFILE, true, false))
    {
        PReadWrite->CloseFile();
        return false;
    }
    char IceName[200];	

    if (PReadWrite->FindString("NumberOfCellsWithIce", X, Y))
        PReadWrite->ReadNumber(X, Y + 1, NumberOfCellsWithIce);
    if (NumberOfCellsWithIce > 0)
    {
        AIceRecord = new IceRecord[NumberOfCellsWithIce];
        if (!PReadWrite->FindString("CriticalIceThickness", Xthikness, Ythikness))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("IceType", XName, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("IceLines", XLine, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("IceColumns", XColumn, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("CongelationLayers", XCongelationLayers, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("SkeletalLayers", XSkeletalLayers, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }
        if (!PReadWrite->FindString("PlateletLayers", XPlateletIceLayers, Y))
        {
            PReadWrite->CloseFile();
            return false;
        }

        for (int j = 0; j < NumberOfCellsWithIce; j++)
        {
            PReadWrite->ReadString(XName, Y + 1 + j, IceName);
            strcpy(AIceRecord[j].IceName, IceName);

            PReadWrite->ReadNumber(XLine, Y + 1 + j, AIceRecord[j].LineCoordinate);
            AIceRecord[j].LineCoordinate = NumberOfLines-AIceRecord[j].LineCoordinate;

            PReadWrite->ReadNumber(XColumn, Y + 1 + j, AIceRecord[j].ColumnCoordinate);
            AIceRecord[j].ColumnCoordinate = AIceRecord[j].ColumnCoordinate - 1;

            PReadWrite->ReadNumber(XCongelationLayers, Y + 1, AIceRecord[j].CongelationIceLayers); //All records forced to have the same number of layers
            PReadWrite->ReadNumber(XSkeletalLayers, Y + 1, AIceRecord[j].SkeletalIceLayers);
            PReadWrite->ReadNumber(XPlateletIceLayers, Y + 1, AIceRecord[j].PlateletIceLayers);

            PReadWrite->ReadNumber(Xthikness, Ythikness + 1, AIceRecord[j].CriticalIceThickness);
        }
        IceGrid = NumberOfCellsWithIce * (AIceRecord[0].CongelationIceLayers + AIceRecord[0].SkeletalIceLayers);
        IceExist = true;
    }
    PReadWrite->CloseFile();
    return true;
}

#endif // !(_PORT_FORTRAN_)
//---------------------------------------------------------------------------
void TEcoDynClass::SetBoxDepth(int ABoxNumber, double depth)
{
	this->BoxArray[ABoxNumber].Depth = depth;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetBoxDepth(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].Depth;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetBoxType(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].Type;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetNorthernBoundary(int ABoxNumber)
{
    return this->BoxArray[ABoxNumber].BoxToBoundary[1].TypeOfBoundary;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetEasternBoundary(int ABoxNumber)
{
    return this->BoxArray[ABoxNumber].BoxToBoundary[2].TypeOfBoundary;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetSouthernBoundary(int ABoxNumber)
{
    return this->BoxArray[ABoxNumber].BoxToBoundary[3].TypeOfBoundary;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetWesternBoundary(int ABoxNumber)
{
    return this->BoxArray[ABoxNumber].BoxToBoundary[4].TypeOfBoundary;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetBoxLength(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].Length;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetChannelWidth(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].Width;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetBoxElevation(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].Elevation;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetBoxVolume(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].Depth
            * this->BoxArray[ABoxNumber].Width
            * this->BoxArray[ABoxNumber].Length;
}
//Pedro Duarte 9/5/2013
//---------------------------------------------------------------------------
double TEcoDynClass::GetBoxLatitude(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].YPosition;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetBoxLongitude(int ABoxNumber)
{
	return this->BoxArray[ABoxNumber].XPosition;
}
//---------------------------------------------------------------------------
//Pedro Duarte 9/5/2013
LoadsRecord* TEcoDynClass::GetALoadRecord()
{
	return ALoadRecord;
}
//---------------------------------------------------------------------------
LossesRecord* TEcoDynClass::GetALossesRecord()
{
	return ALossesRecord;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasALoadRecord()
{
    return LoadsExist;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasALossesRecord()
{
    return LossesExist;
}
//------------------------------------------------------------------------
int TEcoDynClass::GetLoadLines(int ALoad)
{
    return ALoadRecord[ALoad].LineCoordinate;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetLoadColumns(int ALoad)
{
    return ALoadRecord[ALoad].ColumnCoordinate;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetLoadFlows(int ALoad, int ATime)
{
    return ALoadRecord[ALoad].LoadFlow[ATime];
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetLossesLines(int ALoss)
{
    return ALossesRecord[ALoss].LineCoordinate;
}
//---------------------------------------------------------------------------
int TEcoDynClass::GetLossesColumns(int ALoss)
{
    return ALossesRecord[ALoss].ColumnCoordinate;
}
//---------------------------------------------------------------------------
double TEcoDynClass::GetLossesFlows(int ALoss, int ATime)
{
    return ALossesRecord[ALoss].LossFlow[ATime];
}
//---------------------------------------------------------------------------
RiversRecord* TEcoDynClass::GetARiverRecord()
{
	return ARiverRecord;
}
//---------------------------------------------------------------------------
SeaBoundariesRecord* TEcoDynClass::GetASeaBoundariesRecord()
{
	return ASeaBoundariesRecord;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasARiverRecord()
{
    return RiversExist;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasASeaBoundariesRecord()
{
    return SeaBoundariesExist;
}
//---------------------------------------------------------------------------
SedimentsRecord* TEcoDynClass::GetASedimentsRecord()
{
	return ASedimentsRecord;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasASedimentsRecord()
{
    return SedimentsExist;
}
//---------------------------------------------------------------------------
BenthicSpeciesRecord* TEcoDynClass::GetABenthicSpeciesRecord()
{
	return ABenthicSpeciesRecord;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasABenthicSpeciesRecord()
{
    return BenthicSpeciesExist;
}
//---------------------------------------------------------------------------
IceRecord* TEcoDynClass::GetAIceRecord()
{
	return AIceRecord;
}
//---------------------------------------------------------------------------
bool TEcoDynClass::HasAIceRecord()
{
    return IceExist;
}
//---------------------------------------------------------------------------
void TEcoDynClass::LogMessage(char* type, char* srcName, char* ParameterName,
        double Value, int BoxNumber)
{
#ifdef __BORLANDC__
  char text[512];

    if ((pMem == NULL) || (srcName == NULL))
        return;

    pMemCriticalSection->Acquire();
    sprintf(text, "%s\t%s\t%s\t%s\t%f\t%d\n",
            srcName, type, GetEcoDynClassName(),
            ParameterName, Value, BoxNumber);
    pMem->Write(text, strlen(text));
    pMemCriticalSection->Release();

#else  // __BORLANDC__
	if (logEnabled && srcName != NULL)
		cout << "LogMessage: " << srcName << "; " << type << "; " 
				<< GetEcoDynClassName() << "; " << ParameterName << "; " 
				<< Value << "; " << BoxNumber << endl;
	
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------
void TEcoDynClass::DebugMessage(char* text)   // AP, 2005.07.13
{
#ifdef __BORLANDC__
  char debugText[512];

    if (pMemDebug == NULL)
        return;

    pMemDebugCriticalSection->Acquire();
    sprintf(debugText, "%s\n", text);
    pMemDebug->Write(debugText, strlen(debugText));
    pMemDebugCriticalSection->Release();

#else  // __BORLANDC__
	if (debugEnabled)
		cout << "DebugMessage: " << text << endl;
	
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------
void TEcoDynClass::SetTimeStep(float tStep)
{
    TimeStep = tStep;
    if(MyTransportPointer != NULL) MyTransportPointer->TimeStep = tStep;
    if(MyNutrientPointer != NULL) MyNutrientPointer->TimeStep = tStep;
    if(MySPMPointer != NULL) MySPMPointer->TimeStep = tStep;
    if(MyPhytoplanktonPointer != NULL) MyPhytoplanktonPointer->TimeStep = tStep;
    if(MyZooplanktonPointer != NULL) MyZooplanktonPointer->TimeStep = tStep;
    if(MySaltMarshPointer != NULL) MySaltMarshPointer->TimeStep = tStep;
    if(MyZoobenthosPointer != NULL) MyZoobenthosPointer->TimeStep = tStep;
    if(MyZoobenthosPointer2 != NULL) MyZoobenthosPointer2->TimeStep = tStep;
    if(MyNektonPointer != NULL) MyNektonPointer->TimeStep = tStep;
    if(MyManPointer != NULL) MyManPointer->TimeStep = tStep;
    if(MyFisherPointer != NULL) MyFisherPointer->TimeStep = tStep;
    if(MyFishPointer != NULL) MyFishPointer->TimeStep = tStep;
        //TEcoDynClass *MyXenobioticPointer;
    if(MyBacteriaPointer != NULL) MyBacteriaPointer->TimeStep = tStep;
    if(MyMacrophytePointer != NULL) MyMacrophytePointer->TimeStep = tStep;
    if(MyMicrophytobenthosPointer != NULL) MyMicrophytobenthosPointer->TimeStep = tStep;
        // Forcing function objects
    if(MyLightPointer != NULL) MyLightPointer->TimeStep = tStep;
    if(MyAirTemperaturePointer != NULL) MyAirTemperaturePointer->TimeStep = tStep;
    if(MyWaterTemperaturePointer != NULL) MyWaterTemperaturePointer->TimeStep = tStep;
    if(MyWindPointer != NULL) MyWindPointer->TimeStep = tStep;
    if(MyFlowPointer != NULL) MyFlowPointer->TimeStep = tStep;
    if(MyTidePointer != NULL) MyTidePointer->TimeStep = tStep;
    if(MySedimentPointer != NULL) MySedimentPointer->TimeStep = tStep;
    if(MyPreyPointer != NULL) MyPreyPointer->TimeStep = tStep;
    if(MyPredatorPointer != NULL) MyPredatorPointer->TimeStep = tStep;
}

#ifndef _PORT_FORTRAN_
//---------------------------------------------------------------------------
// TEcoDynClass debuggers

void TEcoDynClass::Debugger(int bug)
{
#ifdef __BORLANDC__
	char szBug[21];
	sprintf(szBug, "%20i", bug);
	MessageBox(0,(LPSTR(szBug)),"Debug", MB_OK);
#else //__BORLANDC__
	cerr << "Debug " << bug << endl;
#endif 	// __BORLANDC__
}

void TEcoDynClass::Debugger(long bug)
{
#ifdef __BORLANDC__
	char szBug[21];
	sprintf(szBug, "%20i", bug);
	MessageBox(0,(LPSTR(szBug)),"Debug", MB_OK);
#else //__BORLANDC__
	cerr << "Debug " << bug << endl;
#endif 	// __BORLANDC__
}

void TEcoDynClass::Debugger(float bug)
{
#ifdef __BORLANDC__
	char szBug[21];
	sprintf(szBug, "%20f", bug);
	MessageBox(0,(LPSTR(szBug)),"Debug", MB_OK);
#else //__BORLANDC__
	cerr << "Debug " << bug << endl;
#endif 	// __BORLANDC__
}

void TEcoDynClass::Debugger(double bug)
{
#ifdef __BORLANDC__
	char szBug[21];
	sprintf(szBug, "%5.10f", bug);
	MessageBox(0,(LPSTR(szBug)),"Debug", MB_OK);
#else //__BORLANDC__
	cerr << "Debug " << bug << endl;
#endif 	// __BORLANDC__
}
#endif  // !(_PORT_FORTRAN_)



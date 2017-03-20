/*
  Name: Liteobjt.cpp
  Light object CPP code file
  Copyright: P.Duarte
  Translated by J. Gomes Ferreira
  Rev. A 96.02.04
  Description: Light object CPP code file
  Date: 14-09-04 (Antonio Pereira) - Code translated to be called by Fortran programs
*/
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "liteobjt.h"
#include "iodll.h"


// TLight Class
//
#ifdef _PORT_FORTRAN_
//-----------------------------------------------
// ...Methods invoked from Fortran...
//
//        only one box
//
//-----------------------------------------------

void light_getvalues__(float* totallight, float* parlight,
            float* hoursofsun, float* horizontallight, float* parhorizontallight,
            float* noonpar, float* photicdepth, float* subsurfacelight,
            float* parsubsurfacelight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   ptr = TLight::getLight();

   strcpy(MyParameter, "Total surface irradiance");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *totallight = Value;

   strcpy(MyParameter, "PAR surface irradiance");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *parlight = Value;

   strcpy(MyParameter, "Daylight hours");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *hoursofsun = Value;

   strcpy(MyParameter, "Mean horizontal water irradiance");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *horizontallight = Value;

   strcpy(MyParameter, "Mean horizontal water PAR irradiance");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *parhorizontallight = Value;

   strcpy(MyParameter, "Noon surface PAR");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *noonpar = Value;

   strcpy(MyParameter, "Photic depth");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *photicdepth = Value;

   strcpy(MyParameter, "Sub-surface irradiance");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, ptr->GetBoxDepth(0), 0, MyParameter, 0);
   *subsurfacelight = Value;

   strcpy(MyParameter, "Sub-surface PAR irradiance");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, ptr->GetBoxDepth(0), 0, MyParameter, 0);
   *parsubsurfacelight = Value;
}

void light_gettotalsurfaceirradiance__(float* totallight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Total surface irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *totallight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Total surface irradiance (W/m2): " << *totallight << endl;

}

void light_getparsurfaceirradiance__(float* parlight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "PAR surface irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *parlight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: PAR surface irradiance (W/m2): " << *parlight << endl;

}

void light_getdaylighthours__(float* hoursofsun)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Daylight hours");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *hoursofsun = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Daylight hours (h): " << *hoursofsun << endl;

}

void light_getdepthintegratedirradiance__(float* horizontallight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Mean horizontal water irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *horizontallight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Depth integrated irradiance (W/m2): " << *horizontallight << endl;

}

void light_getdepthintegratedparirradiance__(float* parhorizontallight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Mean horizontal water PAR irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *parhorizontallight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Depth integrated PAR irradiance (W/m2): " << *parhorizontallight << endl;

}

void light_getmeanverticalwaterirradiance__(float* verticallight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Mean vertical water irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *verticallight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Mean vertical water irradiance (W/m2): " << *verticallight << endl;

}

void light_getmeanverticalparwaterirradiance__(float* parverticallight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Mean vertical water PAR irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *parverticallight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Mean vertical water PAR irradiance (W/m2): " << *parverticallight << endl;

}

void light_getnoonsurfacepar__(float* noonpar)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Noon surface PAR");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *noonpar = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Noon surface PAR (W/m2): " << *noonpar << endl;

}

void light_getphoticdepth__(float* photicdepth)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Photic depth");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *photicdepth = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Photic depth (m): " << *photicdepth << endl;

}

void light_getsubsurfaceirradiance__(float* subsurfacelight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Sub-surface irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, ptr->GetBoxDepth(0), 0, MyParameter, 0);
   *subsurfacelight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Sub-surface irradiance (W/m2): " << *subsurfacelight << endl;

}

void light_getsubsurfaceirradiancepar__(float* parsubsurfacelight)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Sub-surface PAR irradiance");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, ptr->GetBoxDepth(0), 0, MyParameter, 0);
   *parsubsurfacelight = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Sub-surface PAR irradiance (W/m2): " << *parsubsurfacelight << endl;

}

void light_getatmosphericir__(float* atmosphericir)
{
   TLight* ptr;
   double Value;
   char MyParameter[65];
   strcpy(MyParameter, "Atmospheric IR");

   ptr = TLight::getLight();
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *atmosphericir = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Atmospheric IR (W/m2): " << *atmosphericir << endl;

}


void light_setdebug__(char* Debug)
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->debug = *Debug;

   if ((ptr->debug =='Y') || (ptr->debug == 'y')) {
      cout << endl << "  TLight address: " << ptr << endl;
      cout << "LO: Debug: " << *Debug << endl;
   }
}

void light_setlatitude__(int* box, float* latitude)
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->SetLatitude(*box, *latitude);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Latitude: " << *latitude << endl;

}

void light_setdepth__(int* box, float* depth)
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->SetBoxDepth(*box, *depth);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Depth: " << *depth << endl;

}

void light_setjulianday__(float* julianday)
{
   TLight* ptr;
   int jd = *julianday;
   ptr = TLight::getLight();
   ptr->SetJulianDay(jd);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Julian day: " << jd << endl;

}

void light_setcurrenttime__(float* currenttime)
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->SetCurrentTime(*currenttime);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Current time: " << *currenttime << endl;

}

void light_setkvalue__(float* kvalue)
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->SetKValue(*kvalue);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: kValue: " << *kvalue << endl;

}

void light_setcloudcover__(float* cloudcover)
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->SetCloudCover(*cloudcover);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "LO: Cloud Cover: " << *cloudcover << endl;

}

void light_go__()
{
   TLight* ptr;
   ptr = TLight::getLight();
   ptr->Go();
}

//-----------------------------------------------
// ...Constructors adapted to Fortran...
//
//        only one box
//
//-----------------------------------------------

TLight* TLight::PLight = 0;

/* just for fun */
void light1_(int* PLight, int* nc, int* nr, int* nz, float* pcb)
{
int i, j, k;
int z, c, r;
float* pf;

    TLight* ptr;// = (TLight*) *PLight;
//    if (ptr == 0) {
        ptr = TLight::getLight((TLight*) *PLight);
        *PLight = (int)ptr;
//    }

    cout << "LO: PLight= " << ptr << endl;

    c = *nc;
    r = *nr;
    z = *nz;

    cout << "LO: NC= " << c << endl;
    cout << "LO: NR= " << r << endl;
    cout << "LO: NZ= " << z << endl;

    pf = pcb;
    for (i = 1; i <= c; i++) {
      for (j = 1; j <= r; j++) {
        for (k = 1; k <= z; k++) {
          cout<<" PCB["<<k<<","<<j<<","<<i<<"]= "<<
            *(pf + (k-1) + z*(j-1) + (z*r)*(i-1)) << endl;
        }
      }
    }
    cout << "------------" << endl;
    for (i = 1; i <= c*r*z; i++) {
      cout<<" PCB["<<i<<"]= "<< *(pf + (i-1)) << endl;
    }
    cout << "+++++++++++" << endl;
}


/*
 * This constructor calls Go() after initialisation
 */
void light_(int* box, float* latitude,
            float* currenttime, float* julianday, float* kvalue,
            float* depth, float* cloudcover, float* airTemperature)
{
   TLight* ptr;
   int jd = *julianday;

   ptr = TLight::getLight();
   ptr->SetLatitude(*box, *latitude);
   ptr->SetCurrentTime(*currenttime);
   ptr->SetJulianDay(jd);
   ptr->SetKValue(*kvalue);
   ptr->SetBoxDepth(*box, *depth);
   ptr->SetCloudCover(*cloudcover);
   ptr->SetAirTemperature(*airTemperature);
   ptr->Go();
}

/*
 * singleton provider - TLight class method
 */
TLight* TLight::getLight(TLight* plight)
{
   PLight = plight;
   if (plight == 0) {
      PLight = new TLight();
      PLight->debug = 'N';
   }
   return PLight;
}

TLight* TLight::getLight()
{
   if (PLight == 0) {
      PLight = new TLight();
      PLight->debug = 'N';
   }
   return PLight;
}


TLight::TLight():TEcoDynClass()
{
   MyPEcoDynClass = (TEcoDynClass*)this;
   PreBuildLight("TLight");

   NumberOfVariables = 15;
   VariableNameArray = new VNA[NumberOfVariables];

   strcpy(VariableNameArray[0], "Total surface irradiance");
   strcpy(VariableNameArray[1], "PAR surface irradiance");
   strcpy(VariableNameArray[2], "Daylight hours");
   strcpy(VariableNameArray[3], "Mean horizontal water irradiance");
   strcpy(VariableNameArray[4], "Mean vertical water irradiance");
   strcpy(VariableNameArray[5], "Mean horizontal water PAR irradiance");
   strcpy(VariableNameArray[6], "Mean vertical water PAR irradiance");
   strcpy(VariableNameArray[7], "Noon surface PAR");
   strcpy(VariableNameArray[8], "Photic depth");
   strcpy(VariableNameArray[9], "Sub-surface irradiance");
   strcpy(VariableNameArray[10], "Sub-surface PAR irradiance");
   strcpy(VariableNameArray[11], "Atmospheric IR");
   strcpy(VariableNameArray[12], "Latitude");
   strcpy(VariableNameArray[13], "Julian day");
   strcpy(VariableNameArray[14], "Current time");

   for (int i = 0; i < NumberOfBoxes; i++)
	Latitude[i] = 37;

   NumberOfParameters = 2;
   ModalCloud = 0.54;
   CloudAmplitude = 0;
   CloudPeak = 0;
   CloudPhase = 0;
   RandomCloud = 0;
}

void TLight::PreBuildLight(char* className)
{
   strcpy(EcoDynClassName, className);
   NumberOfLines = 1;
   NumberOfColumns = 1;
   NumberOfBoxes = 1; // Size of array - i.e. number of model boxes
   // Initialise constants

   LightToPAR = 0.42;
   IncidentLight = 0.85;
   SolarConstant = 1367; // W/m2
   Sigma = 0.0000000567; // 5.67e-8; // W/m2/k4 Stefan-Boltzman constant

   TidalHeight = 1.00; // Generic Tidal Height = 1.00m
   CloudCover = 0.54;  // 0.54 Average cloud cover - see Fenómenos de transferência
   SeaAlbedo = 0.06;   // Transmissivity of clear sky - see Portela & Neves (1994)
					  // - Advances in water resources technology and management
   KValue = 0.5;

   // Initialise pointers
   if (NumberOfBoxes > 0)                       // Initialise arrays for variables
   {
	AverageHorizontalLight= new double[NumberOfBoxes];
	AverageVerticalLight  = new double[NumberOfBoxes];
	EuphoticZone   	    = new double[NumberOfBoxes];
	SubSurfaceLight       = new double[NumberOfBoxes];
	TotalSurfaceLight     = new double[NumberOfBoxes];
	ParSurfaceLight       = new double[NumberOfBoxes];
	AnyPar                = new double[NumberOfBoxes];
	HoursOfSun   	    = new double[NumberOfBoxes];
	PhoticDepth   	    = new double[NumberOfBoxes];
	AtmosphericIR         = new double[NumberOfBoxes];
	ExtinctionCoefficient = new double[NumberOfBoxes];
	Sunrise   		    = new double[NumberOfBoxes];
	Sunset       	    = new double[NumberOfBoxes];
	Latitude   		    = new double[NumberOfBoxes];
      TotalLight            = new double[NumberOfBoxes];
      DailyAverageLight     = new double[NumberOfBoxes];
	// Fluxes and conversions
	for (int i = 0; i < NumberOfBoxes; i++)
	{
           AverageHorizontalLight[i]= 0.0;
	   AverageVerticalLight[i]  = 0.0;
	   EuphoticZone[i]          = 0.0;
	   SubSurfaceLight[i]       = 0.0;
	   TotalSurfaceLight[i]     = 0.0;
	   ParSurfaceLight[i]       = 0.0;
	   AnyPar[i]                = 0.0;
	   HoursOfSun[i]            = 0.0;
	   PhoticDepth[i]           = 0.0;
	   AtmosphericIR[i]         = 0.0;
	   ExtinctionCoefficient[i] = 0.0;
	   Sunrise[i]               = 0.0;
	   Sunset[i]                = 0.0;
	   Latitude[i]              = 0.0;
           TotalLight[i]            = 0.0;
           DailyAverageLight[i]     = 0.0;
	}
   }
   VariableCount = 0;
   ParameterCount = 0;
   TimeStep = 5;		// 5 seg.
   JulianDay = 266;	// 22.Setembro
   CurrentTime = 12;  	// 12:00
   TimeBefore = -1;
   MyTime = 0.5;
   ObjectCode = LITEOBJECTCODE;

   CloudSpecificTime = 0.0;   //NG : for random cloud parameter;

   DoubleRandomCloud = rand()*RandomCloud/100.00;

   int NumberOfSubDomains = 1;

 
   subDomain.NumberOfBoxes = 1; 
   subDomain.FirstLine = 0;     //Southern limit
   subDomain.LastLine =  0;     //Northern limit
   subDomain.FirstColumn = 0;   //Western limit
   subDomain.LastColumn = 0;    //Eastern limit
   subDomain.AnIndex = new int[NumberOfBoxes]; 
   subDomain.ILine = new int[NumberOfColumns];
   subDomain.FLine = new int[NumberOfColumns]; 
   subDomain.IColumn = new int[NumberOfLines];
   subDomain.FColumn = new int[NumberOfLines];
   subDomain.BoxNumber = new int[NumberOfBoxes]; 
   for (int i = 0; i < NumberOfColumns; i++)
   { 
      subDomain.ILine[i] = 0;
      subDomain.FLine[i] = 0;
   }
   for (int i = 0; i < NumberOfLines; i++)
   { 
      subDomain.IColumn[i] = 0;
      subDomain.FColumn[i] = 0;
   }   for (int i = 0; i < NumberOfBoxes; i++)
   { 
      subDomain.AnIndex[i] = i;
      subDomain.BoxNumber[i] = i;
   }
   SetSubDomain(&subDomain);
   NumberOfMomentsForTimeSeries = 0;
}

void light_new__(long* PLight)
{
  //MyPEcoDynClass = (TEcoDynClass*)this;
   TLight* ptr;
   ptr = TLight::getLight();
   *PLight = (long)ptr;
   ptr->PreBuildLight("TLight"); 
}

void light_new_go__(long* plight, double* curtime, double* julianday, double* latitude, double* cloudcover, double* seaalbedo, double* light)
{
    int numberOfMomentsForTimeSeries = 0;
    /*cout << "Starting light_new_go__ ";
    cout<<"plight = "<<*plight;
    cout<<"curtime = "<<*curtime;
    cout<<"julianday = "<<*julianday;
    cout<<"latitude = "<<*latitude;
    cout<<"cloudcover = "<<*cloudcover;
    cout<<"seaalbedo = "<<*seaalbedo<<endl;*/
    TLight* ptr = (TLight*) *plight;
    //cout << " ptr initialized"<< endl;
    char* classname;
    char MyParameter[65];
    double Value;
    classname = ptr->GetEcoDynClassName();
    strcpy(MyParameter,"Total surface irradiance");
    ptr->SetCurrentTime(*curtime);
    ptr->SetJulianDay(*julianday);
    ptr->SetLatitude(0,*latitude);
    ptr->SetCloudCover(*cloudcover);
    ptr->SetSeaAlbedo(*seaalbedo); 
    ptr->SetNumberOfMomentsForTimeSeries(numberOfMomentsForTimeSeries);
#ifdef _PORT_FORTRAN_   
    ptr->GetLightAtSurface(ptr);
#endif
    ptr->Inquiry(classname, Value,0,MyParameter,0);
    //cout << "Light was calculated"<< endl;
    *light = Value;
}

//-----------------------------------------------

#else

// Constructors invoked outside EcoDyn shell...
//

TLight::TLight(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aLatitude[],
        int nParams, double modalCloud, double cloudAmplitude, double cloudPeak,
        double cloudPhase, double randomCloud, double cloudCover, double kValue)
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    PreBuildLight(className);

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);
	for (int i = 0; i < NumberOfBoxes; i++)
		Latitude[i] = aLatitude[i];

	NumberOfParameters = nParams;
	ModalCloud = modalCloud;
    CloudAmplitude = cloudAmplitude;
	CloudPeak = cloudPeak;
	CloudPhase = cloudPhase;
    RandomCloud = randomCloud;
    CloudCover = cloudCover;
    KValue = kValue;
}

TLight::TLight(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
                        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    if (OpenMorphology()) {
        BuildLight(className);
    }
}
// ...

TLight::TLight(TEcoDynClass* APEcodynClass, char* className)
					: TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = APEcodynClass;
    NumberOfSubDomains = 1;
    BuildLight(className);
}

void TLight::PreBuildLight(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
	// Initialise constants

	LightToPAR = 0.42;
	IncidentLight = 0.85;
	SolarConstant = 1367; // W/m2
	Sigma = 0.0000000567; // 5.67e-8; // W/m2/k4 Stefan-Boltzman constant

	TidalHeight = 1.00; // Generic Tidal Height = 1.00m
	CloudCover = 0.54;  // 0.54 Average cloud cover - see Fenómenos de transferência
	SeaAlbedo = 0.06;   // Transmissivity of clear sky - see Portela & Neves (1994)
					  // - Advances in water resources technology and management
	KValue = 0.1; // 2

	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variables
	{
		AverageHorizontalLight= new double[NumberOfBoxes];
		AverageVerticalLight  = new double[NumberOfBoxes];
		EuphoticZone = new double[NumberOfBoxes];
		SubSurfaceLight = new double[NumberOfBoxes];
		TotalSurfaceLight = new double[NumberOfBoxes];
		ParSurfaceLight = new double[NumberOfBoxes];
		AnyPar = new double[NumberOfBoxes];
		HoursOfSun = new double[NumberOfBoxes];
		PhoticDepth = new double[NumberOfBoxes];
		AtmosphericIR = new double[NumberOfBoxes];
		ExtinctionCoefficient = new double[NumberOfBoxes];
		Sunrise = new double[NumberOfBoxes];
		Sunset = new double[NumberOfBoxes];
		Latitude = new double[NumberOfBoxes];
                TotalLight = new double[NumberOfBoxes];
                DailyAverageLight = new double[NumberOfBoxes];
		// Fluxes and conversions
		for (int i = 0; i < NumberOfBoxes; i++)
		{
                    AverageHorizontalLight[i]= 0.0;
                    AverageVerticalLight[i]  = 0.0;
                    EuphoticZone[i]          = 0.0;
                    SubSurfaceLight[i]       = 0.0;
                    TotalSurfaceLight[i]     = 0.0;
                    ParSurfaceLight[i]       = 0.0;
                    AnyPar[i]                = 0.0;
                    HoursOfSun[i]            = 0.0;
                    PhoticDepth[i]           = 0.0;
                    AtmosphericIR[i]         = 0.0;
                    ExtinctionCoefficient[i] = 0.0;
                    Sunrise[i]               = 0.0;
                    Sunset[i]                = 0.0;
                    if (MyPEcoDynClass->GetLatitudeBoolean())
                       Latitude[i] = MyPEcoDynClass->GetBoxLatitude(i);
                    else
                       Latitude[i] = 0.0;
                    TotalLight[i]            = 0.0;
                    DailyAverageLight[i]     = 0.0;
		}
	}
	VariableCount = 0;
	ParameterCount = 0;
	TimeStep = MyPEcoDynClass->GetTimeStep();
	JulianDay = MyPEcoDynClass->GetJulianDay();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
	TimeBefore = MyPEcoDynClass->GetTimeBefore();
	ObjectCode = LITEOBJECTCODE;

	CloudSpecificTime = 0.0;   //NG : for random cloud parameter;

	int IntRandomCloud = random(RandomCloud);
	DoubleRandomCloud = IntRandomCloud/100.00;

    NumberOfMomentsForTimeSeries = 1;
    NumberOfDaysForTimeSeries = 0;
    NumberOfHoursForTimeSeries = 0;
}

void TLight::BuildLight(char* className)
{
	int X,Y, XV, YV;
    char MyParameter[65];

    RandomCloud = 0.0;
    PreBuildLight(className);

	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Light");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Light parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TLight::BuildLight parameters file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Light", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];
            // read in the parameter names

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Modal Cloud") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ModalCloud);
                }
                else if (strcmp(MyParameter, "Cloud Amplitude") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CloudAmplitude);
                }
                else if (strcmp(MyParameter, "Cloud Peak") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CloudPeak);
                }
                else if (strcmp(MyParameter, "Cloud Phase") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CloudPhase);
                }
                else if (strcmp(MyParameter, "Random Cloud") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RandomCloud);
                }
                else if (strcmp(MyParameter, "Cloud cover") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CloudCover);
                }
                else if (strcmp(MyParameter, "KValue") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KValue);
                }
                else if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfDaysForTimeSeries);
                    NumberOfMomentsForTimeSeries = NumberOfDaysForTimeSeries;
                    //Debugger(NumberOfDaysForTimeSeries);
                }
                else if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfHoursForTimeSeries);
                    NumberOfMomentsForTimeSeries = NumberOfHoursForTimeSeries;
                    //Debugger(NumberOfHoursForTimeSeries);
                }
                else
                if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NumberOfSubDomains); //Debugger(NumberOfSubDomains);
                }
                MyLightSubDomains = new SubDomain[NumberOfSubDomains];
                for (int j = 0; j < NumberOfSubDomains; j++)
                {
                    MyLightSubDomains[j].FirstLine = 0;                                         //Southern limit
                    MyLightSubDomains[j].LastLine = MyPEcoDynClass->GetNumberOfLines() - 1;     //Northern limit
                    MyLightSubDomains[j].FirstColumn = 0;                                       //Western limit
                    MyLightSubDomains[j].LastColumn = MyPEcoDynClass->GetNumberOfColumns() - 1; //Eastern limit
                }
                for (int i = Y; i < Y + NumberOfParameters; i++)
                {
                    PReadWrite->ReadString(X+2, i, MyParameter);
                    strcpy(ParametersNameArray[i-Y], MyParameter);
                    if (strcmp(MyParameter, "NumberOfSubDomains") == 0)
                    {
                        int AValue, index = 0;
                        for (int j = i; j < i + NumberOfSubDomains; j++)
                        {
                           //Debugger(j);
                           PReadWrite->ReadNumber(X+4, j, AValue);
                           MyLightSubDomains[index].FirstLine = NumberOfLines - AValue; //Debugger(AValue);//Southern limit
                           PReadWrite->ReadNumber(X+5, j, AValue);
                           MyLightSubDomains[index].LastLine = NumberOfLines - AValue; //Debugger(AValue); //Northern limit
                           PReadWrite->ReadNumber(X+6, j, AValue);
                           MyLightSubDomains[index].FirstColumn = AValue - 1;          //Debugger(AValue); //Western limit
                           PReadWrite->ReadNumber(X+7, j, AValue);
                           MyLightSubDomains[index].LastColumn = AValue - 1;           //Debugger(AValue); //Eastern limit
                           //Debugger(index);
                           //Debugger(MyAirTemperatureSubDomains[index].FirstLine); Debugger(MyAirTemperatureSubDomains[index].LastLine);
                           //Debugger(MyAirTemperatureSubDomains[index].FirstColumn); Debugger(MyAirTemperatureSubDomains[index].LastColumn);
                           index++;
                        }
                    }
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }

    TotalSurfaceLightTimeSeries = new double[NumberOfSubDomains*NumberOfMomentsForTimeSeries];
    int index;
    for (int i = 0; i < NumberOfSubDomains; i++)
    {
       for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
       {
          index = i * NumberOfMomentsForTimeSeries + j;
          TotalSurfaceLightTimeSeries[index] = 0.0;
       }
    }
	// Read in the variables
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Light");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Light variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TLight::BuildLight variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Light", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++) {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(VariableNameArray[i-Y], MyParameter);
                //MessageBox(0, MyParameter, "EcoDynamo alert", MB_OK);
                if (strcmp(MyParameter, "Latitude") == 0)
                {
                    double MyLatitude;
                    PReadWrite->ReadNumber(X+3, i, MyLatitude);
                    for (int i = 0; i < NumberOfBoxes; i++)
                    {
                        Latitude[i] = MyLatitude;
                    }
                }
                else if (strcmp(MyParameter, "TotalSurfaceLightTimeSeries") == 0)
                {
                    if (!PReadWrite->FindString("TotalSurfaceLightTimeSeries values", XV, YV)) {
                        XV = X + 3;
                        YV = i - 1;
                    }
                    //Debugger(XV); Debugger(YV); Debugger(NumberOfSubDomains); Debugger(NumberOfMomentsForTimeSeries);
                    for (int i = 0; i < NumberOfSubDomains; i++)
                    {
                        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
                        {
                            index = i * NumberOfMomentsForTimeSeries + j;
                            PReadWrite->ReadNumber(XV + i, YV + 1 + j, TotalSurfaceLightTimeSeries[index]);
                        }
                    }
                    //Debugger(TotalSurfaceLightTimeSeries[index]);
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}
#endif  // _PORT_FORTRAN_

TLight::~TLight()
{
    freeMemory();
}

void TLight::freeMemory()
{
    try {

        if (NumberOfBoxes > 0)
        {
            delete [] AverageHorizontalLight;
            delete [] AverageVerticalLight;
            delete [] EuphoticZone;
            delete [] SubSurfaceLight;
            delete [] TotalSurfaceLight;
            delete [] ParSurfaceLight;
            delete [] AnyPar;
            delete [] HoursOfSun;
            delete [] PhoticDepth;
            delete [] AtmosphericIR;
            delete [] ExtinctionCoefficient;
            delete [] Sunrise;
            delete [] Sunset;
            delete [] Latitude;
            delete [] TotalLight;
            delete [] DailyAverageLight;
            delete [] TotalSurfaceLightTimeSeries;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            delete [] MyLightSubDomains;
            NumberOfParameters = 0;
        }
#ifdef _PORT_FORTRAN_
        PLight = 0;
#endif  // _PORT_FORTRAN_
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TLight::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TLight::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Modal Cloud") == 0)
    {
        value = ModalCloud;
    }
    else if (strcmp(MyParameter, "Cloud Amplitude") == 0)
    {
        value = CloudAmplitude;
    }
    else if (strcmp(MyParameter, "Cloud Peak") == 0)
    {
        value = CloudPeak;
    }
    else if (strcmp(MyParameter, "Cloud Phase") == 0)
    {
        value = CloudPhase;
    }
    else if (strcmp(MyParameter, "Random Cloud") == 0)
    {
        value = RandomCloud;
    }
    else if (strcmp(MyParameter, "Cloud cover") == 0)
    {
        value = CloudCover;
    }
    else if (strcmp(MyParameter, "KValue") == 0)
    {
        value = KValue;
    }
    else if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
    {
        value = NumberOfDaysForTimeSeries;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
    {
        value = NumberOfHoursForTimeSeries;
    }
    else
        value = 0.0;
    return value;
}

bool TLight::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Modal Cloud") == 0)
    {
        ModalCloud = value;
    }
    else if (strcmp(MyParameter, "Cloud Amplitude") == 0)
    {
        CloudAmplitude = value;
    }
    else if (strcmp(MyParameter, "Cloud Peak") == 0)
    {
        CloudPeak = value;
    }
    else if (strcmp(MyParameter, "Cloud Phase") == 0)
    {
        CloudPhase = value;
    }
    else if (strcmp(MyParameter, "Random Cloud") == 0)
    {
        RandomCloud = value;
    }
    else if (strcmp(MyParameter, "Cloud cover") == 0)
    {
        CloudCover = value;
    }
    else if (strcmp(MyParameter, "KValue") == 0)
    {
        KValue = value;
    }
    else if (strcmp(MyParameter, "NumberOfDaysForTimeSeries") == 0)
    {
        NumberOfDaysForTimeSeries = value;
        NumberOfMomentsForTimeSeries = value;
    }
    else if (strcmp(MyParameter, "NumberOfHoursForTimeSeries") == 0)
    {
        NumberOfHoursForTimeSeries = value;
        NumberOfMomentsForTimeSeries = value;
    }
    else
        rc = false;

    return rc;
}

bool TLight::SaveVariables()
{
    TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Light");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Light");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Latitude") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Latitude[0], 2);
        }
        PReadWrite->WriteSeparator(true);
    }

    if (NumberOfHoursForTimeSeries > 0 || NumberOfDaysForTimeSeries > 0) {
        PReadWrite->WriteSeparator(true);
        PReadWrite->WriteCell("TotalSurfaceLightTimeSeries values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfMomentsForTimeSeries; j++)
        {
            PReadWrite->WriteCell(TotalSurfaceLightTimeSeries[j], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    CloseDataFile((void*)PReadWrite);

    return true;
}

bool TLight::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Light");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Light");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();

    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Modal Cloud") == 0)
        {
            PReadWrite->WriteCell(ModalCloud, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Cloud Amplitude") == 0)
        {
            PReadWrite->WriteCell(CloudAmplitude, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Cloud Peak") == 0)
        {
            PReadWrite->WriteCell(CloudPeak, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Cloud Phase") == 0)
        {
            PReadWrite->WriteCell(CloudPhase, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Random Cloud") == 0)
        {
            PReadWrite->WriteCell(RandomCloud, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Cloud cover") == 0)
        {
            PReadWrite->WriteCell(CloudCover, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KValue") == 0)
        {
            PReadWrite->WriteCell(KValue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfDaysForTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForTimeSeries, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfHoursForTimeSeries") == 0)
        {
            PReadWrite->WriteCell(NumberOfHoursForTimeSeries, 0);
        }
        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

void TLight::Go()
{
   MyTime = MyPEcoDynClass->GetTime();
   CurrentTime = MyPEcoDynClass->GetCurrTime();
   JulianDay = MyPEcoDynClass->GetJulianDay();
   TimeBefore = MyTime;
}

void TLight::Integrate()
{
    // Active methods
    GetDaylightHours();
    GetLightAtSurface (12.0);
#ifndef _PORT_FORTRAN_
    GetLightAtSurface();
#endif
    GetPhoticDepth();
    GetHorizontalMeanWaterColumnRadiation();
    //GetAtmosphericInfraRed();
}


void TLight::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Total surface irradiance") == 0)
		Value = TotalSurfaceLight[MyBoxNumber];
        else if (strcmp(MyParameter, "TotalSurfaceLightTimeSeries") == 0)
		Value = TotalSurfaceLight[MyBoxNumber];
	else if (strcmp(MyParameter, "PAR surface irradiance") == 0)
		Value = ParSurfaceLight[MyBoxNumber];
	else if (strcmp(MyParameter, "Daylight hours") == 0)
		Value = HoursOfSun[MyBoxNumber];
	else if (strcmp(MyParameter, "Mean horizontal water irradiance") == 0)
		Value = AverageHorizontalLight[MyBoxNumber];
	else if (strcmp(MyParameter, "Mean vertical water irradiance") == 0)
		Value = AverageVerticalLight[MyBoxNumber];
	else if (strcmp(MyParameter, "Mean horizontal water PAR irradiance") == 0)
		Value = AverageHorizontalLight[MyBoxNumber] * LightToPAR;
	else if (strcmp(MyParameter, "Mean vertical water PAR irradiance") == 0)
		Value = AverageVerticalLight[MyBoxNumber] * LightToPAR;
	else if (strcmp(MyParameter, "Noon surface PAR") == 0)
		Value = AnyPar[MyBoxNumber] * LightToPAR;
	else if (strcmp(MyParameter, "Photic depth") == 0)
		Value = PhoticDepth[MyBoxNumber];
	else if (strcmp(MyParameter, "Sub-surface irradiance") == 0)
		Value = SubSurfaceLight[MyBoxNumber];
	else if (strcmp(MyParameter, "Sub-surface PAR irradiance") == 0)
		Value = SubSurfaceLight[MyBoxNumber] * LightToPAR;
	else if (strcmp(MyParameter, "Atmospheric IR") == 0)
		//Value = AtmosphericIR[MyBoxNumber];
		Value = GetAtmosphericInfraRed(MyBoxNumber);
	else if (strcmp(MyParameter, "Latitude") == 0)
		Value = Latitude[MyBoxNumber];
	// temporary
	else if (strcmp(MyParameter, "Julian day") == 0)
		Value = MyPEcoDynClass->GetJulianDay();
	else if (strcmp(MyParameter, "Current time") == 0)
		Value = MyPEcoDynClass->GetCurrTime();
    else if (strcmp(MyParameter, "KValue") == 0)
		Value = KValue;
    else if (strcmp(MyParameter, "Daily average light") == 0)
   	    Value = DailyAverageLight[MyBoxNumber];

#ifndef _PORT_FORTRAN_
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 1", MB_OK);
#else  // __BORLANDC__
						cerr << "TLight::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
#endif  // _PORT_FORTRAN_
}

bool TLight::SetVariableValue(char* srcName, double Value,
            int BoxNumber,char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Total surface irradiance") == 0)
		TotalSurfaceLight[BoxNumber] = Value;
	else if (strcmp(VariableName, "PAR surface irradiance") == 0)
		ParSurfaceLight[BoxNumber] = Value;
	else if (strcmp(VariableName, "Daylight hours") == 0)
		HoursOfSun[BoxNumber] = Value;
	else if (strcmp(VariableName, "Mean horizontal water irradiance") == 0)
		AverageHorizontalLight[BoxNumber] = Value;
	else if (strcmp(VariableName, "Mean vertical water irradiance") == 0)
		AverageVerticalLight[BoxNumber] = Value;
	else if (strcmp(VariableName, "Mean horizontal water PAR irradiance") == 0)
		AverageHorizontalLight[BoxNumber] = Value / LightToPAR;
	else if (strcmp(VariableName, "Mean vertical water PAR irradiance") == 0)
		AverageVerticalLight[BoxNumber] = Value / LightToPAR;
	else if (strcmp(VariableName, "Noon surface PAR") == 0)
		AnyPar[BoxNumber] = Value / LightToPAR;
	else if (strcmp(VariableName, "Photic depth") == 0)
		PhoticDepth[BoxNumber] = Value;
	else if (strcmp(VariableName, "Sub-surface irradiance") == 0)
		SubSurfaceLight[BoxNumber] = Value;
	else if (strcmp(VariableName, "Sub-surface PAR irradiance") == 0)
		SubSurfaceLight[BoxNumber] = Value / LightToPAR;
    else if (strcmp(VariableName, "KValue") == 0)
		KValue = Value;
    else if (strcmp(VariableName, "Daily average light") == 0)
   	    DailyAverageLight[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TLight::Inquiry(char* srcName, double &Value,
								 double Value1,
								 int BoxNumber,
								 char* ParameterName,
								 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Sub-surface irradiance") == 0)
	{
		GetLightAtAnyDepth(BoxNumber, Value1);
		Value = SubSurfaceLight[MyBoxNumber];
	}
	else if (strcmp(MyParameter, "Sub-surface PAR irradiance") == 0)
	{
		GetLightAtAnyDepth(BoxNumber, Value1);
		Value = SubSurfaceLight[MyBoxNumber] * LightToPAR;
	}
    else if (strcmp(MyParameter, "Total surface irradiance") == 0)
	{
#ifndef _PORT_FORTRAN_
		GetLightAtSurface();
#endif
		Value = TotalSurfaceLight[MyBoxNumber];
	}

#ifndef _PORT_FORTRAN_
	else {
#ifdef __BORLANDC__
		MessageBox(0, "Danger - Use only for light at specific depth",
					   "EcoDynamo Alert - Light object",
					   MB_OK | MB_ICONHAND);
    	char Caption[129];
	    strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
	    MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Inquiry 2",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TLight::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << "\nDanger - Use only for light at specific depth" << endl;
#endif  // __BORLANDC__
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
#endif  // _PORT_FORTRAN_
}



void TLight::Inquiry(char* srcName, double &Value,
                         double ALight,
						 double ADepth,
                         double AExtinctionCoefficient,
                         int BoxNumber,
                         char* ParameterName,
                         int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Sub-surface PAR irradiance") == 0)
	{
		GetLightAtAnyDepth(MyBoxNumber, ALight, ADepth, AExtinctionCoefficient);
		Value = SubSurfaceLight[MyBoxNumber] * LightToPAR;
	}
    else if (strcmp(MyParameter, "Sub-surface irradiance") == 0)
	{
		GetLightAtAnyDepth(MyBoxNumber, ALight, ADepth, AExtinctionCoefficient);
                Value = SubSurfaceLight[MyBoxNumber];
	}

#ifndef _PORT_FORTRAN_
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
#endif  // _PORT_FORTRAN_
}


// Protected methods
#ifndef _PORT_FORTRAN_
void TLight::GetLightAtSurface()
#else
void TLight::GetLightAtSurface(TLight* ptr)
#endif
{
double HourAngle,
       SolarAltitude,
       AtmosphericTransmission,
       RadiusVector,
       DeclinationAngle,
       RadiationAtTop;
    int nbox, i;
    //cout<<"GetLightAtSurface - initialzing pSubDomain"<<endl;
#ifndef _PORT_FORTRAN_
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
#else
    SubDomain *pSubDomain = ptr->GetSubDomain();
#endif
    //cout<<"GetLightAtSurface - pSubDomain initialized"<<endl; 
    HourAngle = GetHourAngle(CurrentTime);
    //cout<<" HourAngle = "<< HourAngle<<endl;
    DeclinationAngle = GetDeclination(JulianDay);
    //cout<<" DeclinationAngle = "<< DeclinationAngle<<endl;
    RadiusVector = GetRadiusVector(JulianDay);
    /*cout<<" RadiusVector = "<< RadiusVector<<endl;
    cout<<" NumberOfMomentsForTimeSeries = "<< NumberOfMomentsForTimeSeries<<endl;
    cout<<" pSubDomain->NumberOfBoxes = "<< pSubDomain->NumberOfBoxes<<endl;
    cout<<" ptr->NumberOfBoxes = "<< ptr->NumberOfBoxes<<endl;*/
    if (NumberOfMomentsForTimeSeries <= 1) {
      for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
      {
          i = pSubDomain->BoxNumber[nbox];
          /*cout<<" nbox = "<< nbox<<endl; 
          cout<<" i = "<< i<<endl; 
          cout<<" NumberOfBoxes = "<< NumberOfBoxes<<endl;*/
          SolarAltitude = GetSolarAltitude(Latitude[i],
                                               DeclinationAngle,
                                               HourAngle);
          //cout<<" SolarAltitude = "<< SolarAltitude<<endl;  
          AtmosphericTransmission = GetAtmosphericTransmission(SolarAltitude);
          //cout<<" AtmosphericTransmission = "<< AtmosphericTransmission<<endl;  
          RadiationAtTop = GetRadiationAtTopOfAtmosphere(SolarAltitude,
                                                             RadiusVector); // W/m2
          //cout<<" RadiationAtTop = "<< RadiationAtTop<<endl;  
          TotalSurfaceLight[i] = GetRadiationAtSeaLevel(RadiationAtTop,
                                                            AtmosphericTransmission);
          //cout<<" TotalSurfaceLight = "<< TotalSurfaceLight[i]<<endl; 
          ParSurfaceLight[i] = TotalSurfaceLight[i] * LightToPAR; // W/m2
          //cout<<" ParSurfaceLight = "<< ParSurfaceLight[i]<<endl; 
             
      }
    }
    else
    {
       int ADay, AMoment, AHour, index1, index2, InitialLine, FinalLine, InitialColumn, FinalColumn;
       if  (NumberOfDaysForTimeSeries >= 1)
       {
          ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,NumberOfDaysForTimeSeries - 1);
          AMoment = ADay;
       }
       else
       if (NumberOfHoursForTimeSeries >= 1)
       {
          ADay = MyPEcoDynClass->GetJulianDay() - 1;
          AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
          AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
       }
       if (pSubDomain)
       {
           InitialLine = pSubDomain->FirstLine;
           FinalLine = pSubDomain->LastLine;
       }
       else
       {
           InitialLine = 0;
           FinalLine = NumberOfLines -1;
           InitialColumn = 0;
           FinalColumn = NumberOfColumns -1;
       }
       for (int i = InitialLine; i <= FinalLine; i++)
       {
          if (pSubDomain)
          {
              InitialColumn = pSubDomain->IColumn[i];
              FinalColumn = MIN(pSubDomain->FColumn[i],MyPEcoDynClass->GetNumberOfColumns()-1);
          }
          for (int j = InitialColumn; j <= FinalColumn; j++)
          {
            index1 = i * NumberOfColumns + j;
            TotalSurfaceLight[index1] = TotalSurfaceLightTimeSeries[AMoment];
            for (int k = 0; k < NumberOfSubDomains; k++)
            {
              index2 = k * NumberOfMomentsForTimeSeries + AMoment;
              if (
                   (i >= MyLightSubDomains[k].FirstLine) && (i <= MyLightSubDomains[k].LastLine) &&
                   (j >= MyLightSubDomains[k].FirstColumn) && (j <= MyLightSubDomains[k].LastColumn)
                 )
              {
                 TotalSurfaceLight[index1] = TotalSurfaceLightTimeSeries[index2]; // W/m2
              }
            }
            ParSurfaceLight[index1] = TotalSurfaceLight[index1] * LightToPAR; // W/m2
          }
       }
    }
}

void TLight::GetLightAtSurface(double CurrentHour)
{
double HourAngle,
       SolarAltitude,
       AtmosphericTransmission,
       RadiusVector,
       DeclinationAngle,
       RadiationAtTop;
int nbox, i;
SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    HourAngle = GetHourAngle(CurrentHour);
    DeclinationAngle = GetDeclination(JulianDay);
    RadiusVector = GetRadiusVector(JulianDay);
    if (NumberOfMomentsForTimeSeries <= 1) {
      for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
      {
          i = pSubDomain->BoxNumber[nbox];
          /*if (
               (CurrentHour < Sunrise[i]) ||
               (CurrentHour > Sunset[i])
              )
              AnyPar[i] = 0.0;
          else
          {   */
              SolarAltitude = GetSolarAltitude(Latitude[i],
                                               DeclinationAngle,
                                               HourAngle);
              AtmosphericTransmission = GetAtmosphericTransmission(SolarAltitude);
              RadiationAtTop = GetRadiationAtTopOfAtmosphere(SolarAltitude,
                                                             RadiusVector); // W/m2
              AnyPar[i] = GetRadiationAtSeaLevel(RadiationAtTop,
                                                 AtmosphericTransmission); // W/m2
         // }
      }
    }
    else
    {
       int ADay, AMoment, AHour, index, index2, i, j;
       if (NumberOfHoursForTimeSeries >= 1)
       {
          ADay = MyPEcoDynClass->GetJulianDay() - 1;
          AHour = ADay * DAYSTOHOURS + CurrentHour;
          AMoment = MIN(AHour, NumberOfHoursForTimeSeries - 1);
          for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
          {
              index = pSubDomain->BoxNumber[nbox];
              TotalSurfaceLight[index] = TotalSurfaceLightTimeSeries[AMoment];
              for (int k = 0; k < NumberOfSubDomains; k++)
              {
                index2 = k * NumberOfMomentsForTimeSeries + AMoment;
                i = MyPEcoDynClass->GetLineIndex3D(nbox);
                j = MyPEcoDynClass->GetColumnIndex3D(nbox);
                if (
                     (i >= MyLightSubDomains[k].FirstLine) && (i <= MyLightSubDomains[k].LastLine) &&
                     (j >= MyLightSubDomains[k].FirstColumn) && (j <= MyLightSubDomains[k].LastColumn)
                   )
                {
                   AnyPar[index] = TotalSurfaceLightTimeSeries[index2]; // W/m2
                }
              }
          }
       }
       else  
#ifdef __BORLANDC__
					MessageBox(0,"Light::GetLightAtSurface","Noon Par not available in time series", MB_OK);
#else  // __BORLANDC__
					cerr << "TLight::etLightAtSurface - Noon Par not available in time series" << endl;
#endif  // __BORLANDC__
   }
}


void TLight::GetDaylightHours()
{
	double DeclinationAngle,
			 SunSetRiseHourAngle;
    int nbox, i;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

	DeclinationAngle = GetDeclination(JulianDay);

	for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
	{
                i = pSubDomain->BoxNumber[nbox];
		SunSetRiseHourAngle = GetSunSetRiseHourAngle(Latitude[i],DeclinationAngle);
		HoursOfSun[i] = (SunSetRiseHourAngle*180/M_PI/15)*2; // Brock, 1981 Ecol. Modelling 14: 1-19
		Sunrise[i] = 12 - 0.5 * HoursOfSun[i]; // Brock, 1981 Ecol. Modelling 14: 1-19
		Sunset[i]  = 12 + 0.5 * HoursOfSun[i]; // Brock, 1981 Ecol. Modelling 14: 1-19
	}
}

void TLight::GetPhoticDepth()
{
	double ICompensationDepth = 0.01; // Convention 1% of surface radiation
	double TotalDepth;
	TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
    int nbox, i;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

	for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
	{
                //Debugger(nbox);
                i = pSubDomain->BoxNumber[nbox];
		if (MySPMPointer != NULL)
		{
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);
		}
		BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
		TotalDepth = BoxDepth /*+ TidalHeight*/;
		// Compensation depth
                if (KValue > 0)
		   PhoticDepth[i] = log(ICompensationDepth)
							  /-KValue;
                else
                   PhoticDepth[i] = 0.0;
		// surface light intensity
		if (PhoticDepth[i] >= TotalDepth)
			PhoticDepth[i] = TotalDepth;
		EuphoticZone[i] = PhoticDepth[i];
	}

}

void TLight::GetHorizontalMeanWaterColumnRadiation()
{
	double BoxDepth;
	TEcoDynClass* MySPMPointer  = MyPEcoDynClass->GetSPMPointer();
        int nbox, i;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

	for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
	{
                i = pSubDomain->BoxNumber[nbox];
		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);
		BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
                if ((BoxDepth > 0.0) && (KValue > 0.0))
			AverageHorizontalLight[i] = (TotalSurfaceLight[i]/BoxDepth)
									  * (1.0 - exp(-KValue * BoxDepth))
									  / KValue;
	}
}



void TLight::GetVerticalMeanWaterColumnRadiation()
{
	double ATop, ABottom, LightAtTop, LightAtBottom, MyOpticalPath;
	TEcoDynClass* MySPMPointer  = MyPEcoDynClass->GetSPMPointer();
    int nbox, i;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

	for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
	{
        i = pSubDomain->BoxNumber[nbox];

		MyPEcoDynClass->GetBoxTopAndBottom(i,
												ATop,
												ABottom);

      MyOpticalPath = ABottom - ATop;

      if (MySPMPointer != NULL)
				MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
												  i,
												  "Extinction coefficient Ktot",
												  ObjectCode);

      if (i == 0)
      {
         Inquiry(GetEcoDynClassName(), LightAtTop,
		  				   i,
		 				   "Total surface irradiance",
						   ObjectCode);
      }
      else
      {
			Inquiry(GetEcoDynClassName(), LightAtTop,
							i - 1,
							"Sub-surface irradiance",
							ObjectCode);
      }
		// Mean light - Pedro's equation
		AverageVerticalLight[i] = (LightAtTop/MyOpticalPath)
										* (exp(-KValue * MyOpticalPath)
										- exp(-KValue * 0.0))
										/ (-KValue);
      TotalLight[i] = AverageVerticalLight[i] + TotalLight[i];
	}
}




void TLight::GetLightAtAnyDepth(int ABoxNumber,double ADepth)
{
    int MyBoxNumber = ABoxNumber;
    double MyDepth = ADepth;
    TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
    if (MySPMPointer != NULL)
    {
            MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
                                  MyBoxNumber,
                                  "Extinction coefficient Ktot",
                                  ObjectCode);
    }
    if (KValue < 40)
    {
            SubSurfaceLight[MyBoxNumber] = TotalSurfaceLight[MyBoxNumber]* exp(-KValue* MyDepth);
    }
    else SubSurfaceLight[MyBoxNumber] = 0.0;
}





void TLight::GetMeanSurfaceRadiation(double InitialTime,
												 double FinalTime,
												 int BoxNumber)
{
}

void TLight::GetAtmosphericInfraRed()
{
	const double SeaAlbedoForAtmosphericRadiation = 0.03;
	TEcoDynClass *MyTAirPointer = MyPEcoDynClass->GetAirTemperaturePointer();
        int nbox, i;
        SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
	for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
	{
            i = pSubDomain->BoxNumber[nbox];
            // Retrieve air temperature from air temperature object
            if (MyTAirPointer != NULL)
            {
                    MyTAirPointer->Inquiry(GetEcoDynClassName(), AirTemperature, i,"Air temperature", ObjectCode);
            }
            else
                AirTemperature = 0;
            AtmosphericIR[i] = 0.00000937 // 0.937e-5
                                     *Sigma
                                     *exp ( 6.0 * log (ABSOLUTEZERO + AirTemperature ) )
                                     * ( 1.0 + 0.17 * sqr (CloudCover))
                                     * ( 1.0 - SeaAlbedoForAtmosphericRadiation);
	}
}

double TLight::GetAtmosphericInfraRed(int ABoxNumber)
{
	const double SeaAlbedoForAtmosphericRadiation = 0.03;
	//double AirTemperature;
        int i;
        i = ABoxNumber;
	TEcoDynClass *MyTAirPointer = MyPEcoDynClass->GetAirTemperaturePointer();
        // Retrieve air temperature from air temperature object
        if (MyTAirPointer != NULL)
        {
           MyTAirPointer->Inquiry(GetEcoDynClassName(), AirTemperature, i,"Air temperature", ObjectCode);
        }
        else
           AirTemperature = 0;
        AtmosphericIR[i] = 0.00000937 // 0.937e-5
                                 *Sigma
                                 *exp ( 6.0 * log (ABSOLUTEZERO + AirTemperature ) )
                                 * ( 1.0 + 0.17 * sqr (CloudCover))
                                 * ( 1.0 - SeaAlbedoForAtmosphericRadiation);
        return(AtmosphericIR[i]);
}

double TLight::GetDeclination(int ADay) // Brock, 1981 Ecol. Model. 14: 1-19
{
	return 23.45 * sin( 360.0 * (284.0 + ADay) / 365.25 * M_PI / 180.0); // Degrees
}

double TLight::GetSunSetRiseHourAngle(double ALatitude,  // Brock, 1981 Ecol. Model. 14: 1-19
									  double ADeclinationAngle)
{
        double MySen;
        MySen = (-sin(ALatitude * M_PI / 180)/ cos(ALatitude * M_PI / 180))* sin(ADeclinationAngle * M_PI / 180) / cos(ADeclinationAngle * M_PI / 180);
        if (MySen < -1.0)
           return M_PI;
        else
        if (MySen > 1.0)
           return 0.0;
        else
           return M_PI/2 - asin(MySen); // Radians

#ifndef _PORT_FORTRAN_
    //MessageBox(0, "You should be seeing polar bears by now!", "Light Object", MB_OK);
#endif  // _PORT_FORTRAN_
    //return 0.0;
}

double TLight::GetHourAngle(double ATrueSolarTime)  // Brock, 1981 Ecol. Model. 14: 1-19
{
	return (ATrueSolarTime - 12)
			 * 15
			 * M_PI/180; // Radians
}

double TLight::GetRadiusVector(int ADay)
{
	return 1 / sqrt( 1 + (0.033 * cos( 360.0 * ADay / 365.25 * M_PI / 180 ) ) );
}

double TLight::GetSolarAltitude(double ALatitude,double ADeclinationAngle,double AHourAngle)
{

   return asin ( sin (ALatitude * M_PI / 180.0 ) * sin ( ADeclinationAngle * M_PI / 180.0)
			  + cos ( ALatitude * M_PI / 180.0 )
			  * cos ( ADeclinationAngle * M_PI / 180.0 ) *cos(AHourAngle ) ); // Radians
}

double TLight::GetRadiationAtTopOfAtmosphere(double ASolarAltitude,double ARadiusVector)
{
	if (
		 (ASolarAltitude >= 0.0) &&
		 (ASolarAltitude <= M_PI / 2.0)
		)
		return SolarConstant
				 * sin(ASolarAltitude)
				 / sqr(ARadiusVector);
	else
		return 0.0;

}

// Calcula a transmissão atmosférica somente quando a altitude do sol é
// positiva, ou seja, entre o nascer e o pôr do Sol, caso contrário
// devolve um zero

double TLight::GetAtmosphericTransmission(double ASolarAltitude)
{
	const double ClearSkyTransmission = 0.74; // Transmissivity of clear sky
	const double WaterOzoneAbsorption = 0.09; // & Water ozone absorption
															// See Portela & Neves (1994)
															// Advances in water resources
															//	technology and management
	double RelativeAirMass, DirectBeamFraction,DiffuseBeamFraction,
	MySolarAltitude = ASolarAltitude;

	if (
		 ( MySolarAltitude >= 0.0 ) &&
		 ( MySolarAltitude <= M_PI / 2.0 )
		)
		{

			RelativeAirMass = 1 / sin( MySolarAltitude );
			if ( RelativeAirMass * log ( ClearSkyTransmission ) < -20.0 )  // Para evitar exponenciais
				DirectBeamFraction = 0.0;	       									 // demasiado pequenos
			else
				DirectBeamFraction = exp(RelativeAirMass * log(ClearSkyTransmission));

			DiffuseBeamFraction = (1.0-WaterOzoneAbsorption-DirectBeamFraction)/2.0;
			return DirectBeamFraction + DiffuseBeamFraction;
		}
	else
		return 0.0;
}

double TLight::GetRadiationAtSeaLevel( double ARadiationAtTopOfAtmosphere,double AnAtmosphericTransmission)
{
	return ARadiationAtTopOfAtmosphere
			 * AnAtmosphericTransmission
			 * ( 1.0 - 0.65 * sqr(CloudCover))
			 * ( 1.0 - SeaAlbedo);
}

void TLight::GetCloudCover()
{
	// NG 17/6/96
	// Random Cloud in % because random need an integer and RandomCloud is between 0 and 1.
	// The following procedure change the RandomCloud every 6 hours if the
	//Time step is 1 hour, but it changes it every day idf time step is 1 day.....


	double MyTime =  JulianDay + CurrentTime * HOURSTODAYS;
	double CheckTime =  MyTime - CloudSpecificTime;
	double SixHour = 0.25;


	if ((CheckTime > SixHour) || (CheckTime == SixHour))
	{
#ifndef _PORT_FORTRAN_
		int IntRandomCloud = random(RandomCloud);
#else  // _PORT_FORTRAN_
		double IntRandomCloud = rand()*RandomCloud;
#endif  // _PORT_FORTRAN_
		DoubleRandomCloud = IntRandomCloud/100.00;
		CloudSpecificTime = MyTime;
	}

	if  (JulianDay == 365)
		CloudSpecificTime = 0.0;

	CloudCover = ModalCloud
					 + ( cos ( M_PI * ( JulianDay + CloudPeak ) / CloudPhase)) // -50 by default
					 * CloudAmplitude
					 + DoubleRandomCloud;

	if (CloudCover > 1)
	{
		CloudCover = 1;

#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
		MessageBox(0,
					  "Cloud Cover > 100 % - You are on Mars",
					  "EcoDynamo Light Alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TLight::GetRadiationAtSeaLevel - Cloud Cover > 100 % - You are on Mars" << endl;
#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
	}
	else if  (CloudCover < 0.0)
	{
		CloudCover = 0.0;

#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
		MessageBox(0,
					  "Cloud Cover < 0 % - You are in space",
					  "EcoDynamo Light Alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TLight::GetRadiationAtSeaLevel - Cloud Cover < 0 % - You are in space" << endl;
#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
	}

}


void TLight::GetLightAtAnyDepth(int ABoxNumber,
                                double ALight,
                                double ADepth,
                                double AExtinctionCoefficient)
{
	int MyBoxNumber;
	double MyLight, MyDepth, MyExtinctionCoefficient;
    MyLight = ALight;
    MyDepth = ADepth;
    MyBoxNumber = ABoxNumber;
    MyExtinctionCoefficient = AExtinctionCoefficient;
    if (MyExtinctionCoefficient < 40.0)
	SubSurfaceLight[MyBoxNumber] = MyLight * exp(-MyExtinctionCoefficient * MyDepth);
    else SubSurfaceLight[MyBoxNumber] = 0.0;

}




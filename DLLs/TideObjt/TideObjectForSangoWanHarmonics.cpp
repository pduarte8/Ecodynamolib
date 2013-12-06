// Tide object CPP code file
// @ Pedro Duarte

/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "tideobjt.h"
#include "iodll.h"
#include "tide.h"
#include "EcoDynClass.h"

// TTideWithWanContinuousHarmonics Class
//
// Constructors invoked outside EcoDyn shell...
//
TTideWithWanContinuousHarmonics::TTideWithWanContinuousHarmonics(
            char* className, int nLines, int nColumns,
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
             double mS4Amplitude, double mS4Phase)
        : TTide(className, nLines, nColumns, nVars, aVariablesNames, nParams, startYear,
                unit, z0, saAmplitude, saPhase, o1Amplitude, o1Phase,
                q1Amplitude, q1Phase, k1Amplitude, k1Phase,
                m2Amplitude, m2Phase, n2Amplitude, n2Phase,
                s2Amplitude, s2Phase, mN4Amplitude, mN4Phase,
                m4Amplitude, m4Phase, mS4Amplitude, mS4Phase)
{
    PTidalHeightPrediction2 = new TTidalHeightPrediction(unit, z0,
             saAmplitude, saPhase, o1Amplitude, o1Phase,
             q1Amplitude, q1Phase, k1Amplitude, k1Phase,
             m2Amplitude, m2Phase, n2Amplitude, n2Phase,
             s2Amplitude, s2Phase, mN4Amplitude, mN4Phase,
             m4Amplitude, m4Phase, mS4Amplitude, mS4Phase);
}

TTideWithWanContinuousHarmonics::TTideWithWanContinuousHarmonics(char* className,
             int nLines, int nColumns,
             char* variablesFilename, char* parametersFilename, char* tideFilename)
        : TTide(className, nLines, nColumns, variablesFilename, parametersFilename, tideFilename)
{
	PTidalHeightPrediction2 = new TTidalHeightPrediction(tideFilename,"Dublin");
}
//...

TTideWithWanContinuousHarmonics::TTideWithWanContinuousHarmonics(
    TEcoDynClass* PEcoDynClass, char* className)
        : TTide(PEcoDynClass, className)
{
	PTidalHeightPrediction2 = new TTidalHeightPrediction(PEcoDynClass->GetTideFile(),"Dublin");
}

TTideWithWanContinuousHarmonics::~TTideWithWanContinuousHarmonics()
{
	freeMemory();
}

void TTideWithWanContinuousHarmonics::freeMemory()
{
	//delete PTidalHeightPrediction2;
}


void TTideWithWanContinuousHarmonics::Inquiry(char* srcName,
                        double &ATidalHeight, double AYear, double AMonth,
						double ADay, double AnHour, int AGridLine)
{
	double MyYear, MyMonth, MyDay, MyHour, MyDistance, MyM2Amplitude, MyM2Phase, MyBoxWidth;
    int  MySouthernLine;
	char MyParameter[65];

	MyYear = AYear;
	MyMonth = AMonth;
	MyDay = ADay;
	MyHour = AnHour;


    MyBoxWidth = MyPEcoDynClass->GetBoxLength(0);
    MySouthernLine = 6;
    MyDistance = (AGridLine - MySouthernLine) * MyBoxWidth + MyBoxWidth / 2.0;

    MyM2Amplitude = 651.7857 - 0.0042 * MyDistance;
    MyM2Phase = 30.17857 - 0.00042 * MyDistance;

	PTidalHeightPrediction2->TidalHeightCalculation(ATidalHeight, MyYear, MyMonth,
												    MyDay, MyHour,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, false,
                                                   MyM2Amplitude, MyM2Phase, true,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, true,
                                                   0.0, 0.0, true);
    LogMessage("Inquiry", srcName, "Tidal height", ATidalHeight, AGridLine);
}



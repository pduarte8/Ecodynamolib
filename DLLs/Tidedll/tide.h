#ifndef TIDE_H
#define TIDE_H
//copyright Pedro Duarte 1997
// modified (slightly) JGF 97.10.09
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export TTidalHeightPrediction
{

    #define Millimeters 1                     // 1 mm
    #define Centimeters 0.1                   // 1 mm in cm
    #define Decimeters  0.01                  // 1 mm in dm
    #define Meters      0.001                 // 1 mm in m
    #define Inches      0.03937007874016      // 1 mm in inches (1in = 2.54cm)
    #define Feet        0.003280839895013     // 1 mm in feet (1 ft = 12 inches)
    #define Yards       0.001093613298338     // 1 mm in yards (1 yard = 3 feet)

    #define NoPort      "NOSUCHPORT"
    #define NW          ""
    #define UW          "Unknown"
    #define LW          "Low water"
    #define HW          "High water"
    #define EW          "Ebb tide"
    #define FW          "Flood tide"

    #define NoTide      -1.0

    public:
        TTidalHeightPrediction(char* AFileName,
                                      char* APortName = "NOSUCHPORT",
                                      double AUnit = Millimeters);
        TTidalHeightPrediction(double unit, double z0,
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
        virtual ~TTidalHeightPrediction();
        void TidalHeightCalculation(double &TidalHeight, double AYear, double AMonth,
                                             double ADay, double AHour);

     void TidalHeightCalculation(double &TidalHeight, double AYear, double AMonth,
                double ADay, double AHour,
                double ASaAmplitude, double ASaPhase, bool SaTrueOrFalse,
                double AO1Amplitude, double AO1Phase, bool O1TrueOrFalse,
                double AQ1Amplitude, double AQ1Phase, bool Q1TrueOrFalse,
                double AK1Amplitude, double AK1Phase, bool K1TrueOrFalse,
                double AM2Amplitude, double AM2Phase, bool M2TrueOrFalse,
                double AN2Amplitude, double AN2Phase, bool N2TrueOrFalse,
                double AS2Amplitude, double AS2Phase, bool S2TrueOrFalse,
                double AMN4Amplitude, double AMN4Phase, bool MN4TrueOrFalse,
                double AM4Amplitude, double AM4Phase, bool M4TrueOrFalse,
                double AMS4Amplitude, double AMS4Phase, bool MS4TrueOrFalse);
        double TidalTime(double AYear, double AMonth,
                double ADay, double AInitialHour, double AFinalHour,
                double AStep);
     double TidalTime(double AYear, double AMonth,
                              double ADay, double AInitialHour, double AFinalHour,
                              double AStep, char*);
        double TidalTime(double AYear, double AMonth,
                              double ADay, double AInitialHour,
                              double AStep, char*);
        double TidalTime(double AYear, double AMonth,
                              double ADay, double ATideNumber,
                              char*);
     double TidalTime(double AYear, double AMonth,
                                     double ADay,
                                     double AAproximateTideTime);
     double RightAngle(double ACosenus, double ASinus);
     void TimeCorrection (double &AYear, double &AMonth, double &ADay, double &AHour);
     double DeltaPHI(double APHI24, double APHI0);
        bool FindPeriod(double AYear, double AMonth, int ANumberOfDays);
        char MyLastTideSituation[20], MyPresentTideSituation[20];
        double CurrentTidalTime;
     void TidePeriodicity(int &jota, int &MaximumNumberOfTides, double AYear, double AMonth, double ADay);
     // AP, 2007.07.02
     void SaveHarmonics(void* fHandle);

    protected :

        double Sa[2],
                 K1[2],
                 O1[2],
                 Q1[2],
                 P1[2],
                 o1nodalcorrection[2],
                 k1nodalcorrection[2],
                 M2[2],
                 N2[2],
                 S2[2],
                 TwoN2[2],
                 miu2[2],
                 niu2[2],
                 L2[2],
                 K2[2],
                 T2[2],
                 m2nodalcorrection[2],
                 k2nodalcorrection[2],
                 MN4[2],
                 M4[2],
                 MS4[2],
                 V[21],
                 X[2],
                 Y[2],
                 R[2],
                 jota[21],
                 n1[21],
                 n2[21],
                 n3[21],
                 n4[21],
                 n5[21],
                 n6[21],
                 TidalHeight, Z0,
                 s, h, p, Nlinha, pp1,

            R1t, cosphi1, sinphi1,  //As vari?veis seguintes eram priovadas
            R2t, cosphi2, sinphi2,  //do void TidalHeightCalculation. Foram tornadas
            R3t, cosphi3, sinphi3,  //gen?ricas de modo a poderem ser acedidas
            R4t, cosphi4, sinphi4;  //por outro void para c?lculo das horas das PM e BM  - P. Duarte 27/1/98

          char AFileName[129],
                 MyFileName[129],
                 MyPortName[129],
                 MyPathName[129];

          virtual void Initialization();
       virtual bool ReadHarmonics();

          // Debuggers
          void Debugger(int i);    //overloaded debugger
          //void Debugger(UINT i);   //overloaded debugger
          void Debugger(long i);   //overloaded debugger
          void Debugger(float i);  //overloaded debugger
          void Debugger(double i);  //overloaded debugger

    private:
        int NumberOfParameters;
        double TheUnit;
        double JulianDaysFromTime0(double AYear, double AMonth,
                                            double ADay, double AHour);
        void CalculateAstronomicParameters (double AT);
        double AstronomicArgument(int j, double AHour);

        // new JGF - 97.11.02
        char MyTideSituation[80], TheTideSituation[20];
        char* PMT;
     double TheInterval;

        double CalculationStep, InitialTime, InitialHeight,
                 InitialYear, InitialMonth, InitialDay, InitialHour,
                 CurrentHeight, Tolerance;

        void TideTime(int ASlope);

};

static TTidalHeightPrediction *PTidalHeightPrediction;
static TTidalHeightPrediction *PTidalHeightPrediction2;

#endif //TIDE_H

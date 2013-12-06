// copyright Pedro Duarte, 1997
// Ref? - Table des marees des grands ports du Monde. Service Hydrographique
// et Oceanographic de la Marine - Paris.

// This DLL needs the harmonic constants (a file name must be given),
// year, month, date and hour to return the tidal height in mm, cm, dm, m, inches, feet or yards

// REV A 97.10.10
// Additions/changes JGF updated 97.11.01
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__

#include "iodll.h"
#include "tide.h"
#include "params.h"


// implementation of TTidalHeightPrediction

TTidalHeightPrediction::TTidalHeightPrediction(double unit, double z0,
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
{
	PAstronomy = new(TAstronomy);
	TheUnit = unit;
    Z0 = z0;
	Sa[0] = saAmplitude;
	Sa[1] = saPhase;
	O1[0] = o1Amplitude;
	O1[1] = o1Phase;
	Q1[0] = q1Amplitude;
	Q1[1] = q1Phase;
	K1[0] = k1Amplitude;
	K1[1] = k1Phase;
	M2[0] = m2Amplitude;
	M2[1] = m2Phase;
	N2[0] = n2Amplitude;
	N2[1] = n2Phase;
	S2[0] = s2Amplitude;
	S2[1] = s2Phase;
	MN4[0] = mN4Amplitude;
	MN4[1] = mN4Phase;
	M4[0] = m4Amplitude;
	M4[1] = m4Phase;
	MS4[0] = mS4Amplitude;
	MS4[1] = mS4Phase;
	Initialization();
}

TTidalHeightPrediction::TTidalHeightPrediction(char* AFileName,
                                               char* APortName,
											   double AUnit)
{
	TheUnit = AUnit;
	strcpy(MyFileName, AFileName);
	strcpy(MyPortName, APortName);
	PAstronomy = new(TAstronomy);

	if ( ReadHarmonics() )
		Initialization();
	else
#ifdef __BORLANDC__
		MessageBox(0, "Could not read file", "Tide DLL error", MB_OK);
#else   // __BORLANDC__
		cerr << "TTidalHeightPrediction::TTidalHeightPrediction - Could not read file" << endl;
#endif  // __BORLANDC__
}

bool TTidalHeightPrediction::ReadHarmonics()
{
	TReadWrite* PReadWrite = new TReadWrite(MyFileName);

	if (!PReadWrite->SetupFile(READFILE))
	{
		return false;
	}
	int X, Y, XH, YH, XA, XP;
	double MyValue = 0.0;

	if (PReadWrite->FindString("Z0", X, Y))
	{
		PReadWrite->ReadNumber(X + 1, Y, MyValue);
		Z0 = MyValue;
        //Debugger(Z0);
        PReadWrite->FindString("Amplitude in mm", XA, Y);
        PReadWrite->FindString("Phase in degrees", XP, Y);
        PReadWrite->FindString("Harmonic constants", XH, YH);
		PReadWrite->ReadNumber(XH + 1 , YH, NumberOfParameters);
        //Debugger(NumberOfParameters);
		char MyParameter[65];
		for (int i = YH; i < YH + NumberOfParameters; i++)
		{
			PReadWrite->ReadString(XH + 2, i, MyParameter);
			if (strcmp(MyParameter, "Sa") == 0)
			{
                PReadWrite->ReadNumber( XA, i, Sa[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, Sa[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber( X + 2 + j, i, MyValue );
					Sa[j] = MyValue;
                    //Debugger(Sa[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "Q1") == 0)
			{
				PReadWrite->ReadNumber( XA, i, Q1[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, Q1[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					Q1[j] = MyValue;
                    //Debugger(Q1[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "O1") == 0)
			{
				PReadWrite->ReadNumber( XA, i, O1[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, O1[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					O1[j] = MyValue;
                    //Debugger(O1[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "K1") == 0)
			{
				PReadWrite->ReadNumber( XA, i, K1[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, K1[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					K1[j] = MyValue;
                    //Debugger(K1[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "N2") == 0)
			{
				PReadWrite->ReadNumber( XA, i, N2[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, N2[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					N2[j] = MyValue;
                    //Debugger(N2[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "M2") == 0)
			{
				PReadWrite->ReadNumber( XA, i, M2[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, M2[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					M2[j] = MyValue;
                    //Debugger(M2[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "S2") == 0)
			{
				PReadWrite->ReadNumber( XA, i, S2[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, S2[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					S2[j] = MyValue;
                    //Debugger(S2[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "MN4") == 0)
			{
				PReadWrite->ReadNumber( XA, i, MN4[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, MN4[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					MN4[j] = MyValue;
                    //Debugger(MN4[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "M4") == 0)
			{
				PReadWrite->ReadNumber( XA, i, M4[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, M4[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					M4[j] = MyValue;
                    //Debugger(M4[j]);
				}*/
			}
			else if  (strcmp(MyParameter, "MS4") == 0)
			{
				PReadWrite->ReadNumber( XA, i, MS4[0]); // amplitude in mm
                PReadWrite->ReadNumber( XP, i, MS4[1]); // phase in degrees

				/*for (int j = 0; j < 2; j++)
				{
					PReadWrite->ReadNumber(X + 2 + j, i, MyValue);
					MS4[j] = MyValue;
                    //Debugger(MS4[j]);
				}*/
			}
		}
	}
	else
	{
#ifdef __BORLANDC__
		MessageBox(0, "Non-existent port in file", "Tide DLL alert", MB_OK);
#else   // __BORLANDC__
		cerr << "TTidalHeightPrediction::ReadHarmonics - Non-existent port in file" << endl;
#endif  // __BORLANDC__
    	PReadWrite->CloseFile();
		return FALSE;
	}
	PReadWrite->CloseFile();
	return TRUE;
}
//---------------------------------------------------------------------------

void TTidalHeightPrediction::SaveHarmonics(void* fHandle)
{
    TReadWrite* PReadWrite = (TReadWrite*)fHandle;

    PReadWrite->WriteCell(MyPortName);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Mean level");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Z0");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(Z0);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("mm");
    PReadWrite->WriteSeparator(true);

    // parameters header line
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Amplitude in mm");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phase in degrees");
    PReadWrite->WriteSeparator(true);

    // line 1
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Harmonic constants");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Sa");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(Sa[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(Sa[1]);
    PReadWrite->WriteSeparator(true);

    // line 2
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Q1");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(Q1[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(Q1[1]);
    PReadWrite->WriteSeparator(true);

    // line 3
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("O1");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(O1[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(O1[1]);
    PReadWrite->WriteSeparator(true);

    // line 4
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("K1");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(K1[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(K1[1]);
    PReadWrite->WriteSeparator(true);

    // line 5
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("N2");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(N2[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(N2[1]);
    PReadWrite->WriteSeparator(true);

    // line 6
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("M2");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(M2[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(M2[1]);
    PReadWrite->WriteSeparator(true);

    // line 7
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("S2");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(S2[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(S2[1]);
    PReadWrite->WriteSeparator(true);

    // line 8
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("MN4");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(MN4[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(MN4[1]);
    PReadWrite->WriteSeparator(true);

    // line 9
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("M4");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(M4[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(M4[1]);
    PReadWrite->WriteSeparator(true);

    // line 10
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("MS4");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(MS4[0]);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(MS4[1]);
    PReadWrite->WriteSeparator(true);
}
//---------------------------------------------------------------------------

void TTidalHeightPrediction::Initialization()
{
	int i;
	// jota
	for (i = 0; i < 21; i++)
	{
		if ( i == 0)
			jota[i] = 0;
		else
		if ( ( i > 0) && ( i < 7) )
			jota[i] = 1;
		else
		if ( ( i >= 7) && ( i < 18) )
			jota[i] = 2;
		else
		if ( i >= 18)
			jota[i] = 4;
	}

	// n1
	n1[0] =  n1[1] = 0; n1[2] = -2; n1[3] = -3; n1[4] =  0; n1[5] = -2;
	n1[6] =  0; n1[7] = -2; n1[8] = -3; n1[9] =  0; n1[10]= n1[11]= -4;
	n1[12]= -3; n1[13]= -1; n1[14]=  n1[15]= 0; n1[16]= -2; n1[17]=  0;
	n1[18]= -5; n1[19]= -4; n1[20]= -2;

	// n2
	n2[0] = n2[1] = n2[2] = n2[3] = 1; n2[4] = -1; n2[5] = n2[6] = 1;
	n2[7] = n2[8] = 2; n2[9] = 0; n2[10]= 2; n2[11]= n2[12]= 4;
	n2[13]= n2[14]= 2; n2[15]= -1; n2[16]= n2[17]= 2; n2[18]= n2[19]= 4;
	n2[20]= 2;

	// n3
	n3[0] = n3[1] = n3[2] = 0; n3[3] = 1; n3[4] = n3[5] = n3[6] = n3[7] = 0;
	n3[8] = 1; n3[9] = 0; n3[10]= 2; n3[11]= 0; n3[12]= n3[13]= -1;
	n3[14]= n3[15]= n3[16]= n3[17]= 0; n3[18]=  1; n3[19]= n3[20]= 0;

	// n4
	n4[0] = n4[1] = n4[2] = n4[3] = n4[4] = 0; n4[5] = -1; n4[6] =  1;
	n4[7] = n4[8] = n4[9] = n4[10]= n4[11]= n4[12]= n4[13]= n4[14]= n4[15]= 0;
	n4[16]= -1; n4[17]=  1; n4[18]= n4[19]= n4[20]= 0;

	// n5
	for (i = 0; i < 21; i++)
		n5[i] = 0;

	// n6
	n6[0] =  0; n6[1] =  1; n6[2] = -1; n6[3] = -1; n6[4] =  1; n6[5] = -1; n6[6] =  1;
	for (i = 7; i < 21; i++)
		n6[i] = 0;

	P1[0] = -1.0 / 3.0 * K1[0]; P1[1] = K1[1];
	o1nodalcorrection[0] = 1.0 / 5.3 * O1[0]; o1nodalcorrection[1] = O1[1];
	k1nodalcorrection[0] = 1.0 / 7.4 * K1[0]; k1nodalcorrection[1] = K1[1];
	TwoN2[0] = 1.0 / 7.6 * N2[0]; TwoN2[1] = N2[1];
	miu2[0] = 1.0 / 6.3 * N2[0]; miu2[1] = N2[1];
	niu2[0] = 1.0 / 5.3 * N2[0]; niu2[1] = N2[1];
	L2[0] = -1.0 / 35.0 * M2[0]; L2[1] = M2[1];
	K2[0] = 1.0 / 3.7 * S2[0]; K2[1] = S2[1];
	T2[0] = 1.0 / 17.0 * S2[0]; T2[1] = S2[1] - 283.0;
	m2nodalcorrection[0] = -1.0 / 27.0 * M2[0]; m2nodalcorrection[1] = M2[1];
	k2nodalcorrection[0]	= 1.0 / 12.0 * S2[0];	k2nodalcorrection[1] = S2[1];
}

TTidalHeightPrediction::~TTidalHeightPrediction()
{
    delete PAstronomy;
}

void TTidalHeightPrediction::TidalHeightCalculation(double &TidalHeight,
																	double AYear, double AMonth,
																	double ADay, double AHour)
{

	double T, MyYear, MyMonth, MyDay, MyHour,
			 X1t, Y1t, //R1t, cosphi1, sinphi1,     Vou tornar estas vari?veis
			 X2t, Y2t, //R2t, cosphi2, sinphi2,     globais de modo a poder ter
			 X3t, Y3t, //R3t, cosphi3, sinphi3,     acesso aos seus valores
			 X4t, Y4t; //R4t, cosphi4, sinphi4;     no c?lculo das horas das PM e BM  P. Duarte 24/1/98

	MyYear = AYear; MyMonth = AMonth; MyDay = ADay; MyHour = AHour;


	T = JulianDaysFromTime0(MyYear, MyMonth, MyDay, MyHour);

	CalculateAstronomicParameters(T);
	for (int j = 0; j < 21; j++)
	{
		V[j] = AstronomicArgument(j, MyHour);
	}

	X1t = Sa[0] * cos((V[0] - Sa[1]) * M_PI / 180);
	Y1t = Sa[0] * sin((V[0] - Sa[1]) * M_PI / 180);
	R1t = sqrt(X1t * X1t + Y1t * Y1t);
	if (R1t != 0.0)
	{
		cosphi1 = X1t / R1t;
		sinphi1 = Y1t / R1t;
	}
	else
	{
		cosphi1 = 0.0;
		sinphi1 = 0.0;
	}

	X2t = K1[0] * cos((V[1] - K1[1]) * M_PI / 180) +
			O1[0] * cos((V[2] - O1[1]) * M_PI / 180) +
			Q1[0] * cos((V[3] - Q1[1]) * M_PI / 180) +
			P1[0] * cos((V[4] - P1[1]) * M_PI / 180) +
			o1nodalcorrection[0] * cos((V[5] - o1nodalcorrection[1]) * M_PI / 180) +
			k1nodalcorrection[0] * cos((V[6] - k1nodalcorrection[1]) * M_PI / 180);
	Y2t = K1[0] * sin((V[1] - K1[1]) * M_PI / 180) +
			O1[0] * sin((V[2] - O1[1]) * M_PI / 180) +
			Q1[0] * sin((V[3] - Q1[1]) * M_PI / 180) +
			P1[0] * sin((V[4] - P1[1]) * M_PI / 180) +
			o1nodalcorrection[0] * sin((V[5] - o1nodalcorrection[1]) * M_PI / 180) +
			k1nodalcorrection[0] * sin((V[6] - k1nodalcorrection[1]) * M_PI / 180);
	R2t = sqrt(X2t * X2t + Y2t * Y2t);
        if (R2t != 0.0)
        {
	   cosphi2 = X2t / R2t;
	   sinphi2 = Y2t / R2t;
        }
        else
        {
           cosphi2 = 0.0;
	   sinphi2 = 0.0;
        }
	X3t = M2[0] * cos((V[7] - M2[1]) * M_PI / 180) +
		N2[0] * cos((V[8] - N2[1]) * M_PI / 180) +
		S2[0] * cos((V[9] - S2[1]) * M_PI / 180) +
		TwoN2[0] * cos((V[10] - TwoN2[1]) * M_PI / 180) +
		miu2[0] * cos((V[11] - miu2[1]) * M_PI / 180) +
		niu2[0] * cos((V[12] - niu2[1]) * M_PI / 180)+
		L2[0] * cos((V[13] - L2[1]) * M_PI / 180) +
		K2[0] * cos((V[14] - K2[1]) * M_PI / 180) +
		T2[0] * cos((V[15] - T2[1]) * M_PI / 180) +
		m2nodalcorrection[0] * cos((V[16] - m2nodalcorrection[1]) * M_PI / 180) +
		k2nodalcorrection[0] * cos((V[17] - k2nodalcorrection[1]) * M_PI / 180);

	Y3t = M2[0] * sin((V[7] - M2[1]) * M_PI / 180) +
		N2[0] * sin((V[8] - N2[1]) * M_PI / 180) +
		S2[0] * sin((V[9] - S2[1]) * M_PI / 180) +
		TwoN2[0] * sin((V[10] - TwoN2[1]) * M_PI / 180) +
		miu2[0] * sin((V[11] - miu2[1]) * M_PI / 180) +
		niu2[0] * sin((V[12] - niu2[1]) * M_PI / 180)+
		L2[0] * sin((V[13] - L2[1]) * M_PI / 180) +
		K2[0] * sin((V[14] - K2[1]) * M_PI / 180) +
		T2[0] * sin((V[15] - T2[1]) * M_PI / 180) +
		m2nodalcorrection[0] * sin((V[16] - m2nodalcorrection[1]) * M_PI / 180) +
		k2nodalcorrection[0] * sin((V[17] - k2nodalcorrection[1]) * M_PI / 180);

	R3t = sqrt(X3t * X3t + Y3t * Y3t);
        if (R3t != 0.0)
        {
	   cosphi3 = X3t / R3t;
	   sinphi3 = Y3t / R3t;
        }
        else
        {
           cosphi3 = 0.0;
           sinphi3 = 0.0;
        }
	X4t = MN4[0] * cos((V[18] - MN4[1]) * M_PI / 180) +
		M4[0] * cos((V[19] - M4[1]) * M_PI / 180) +
		MS4[0] * cos((V[20] - MS4[1]) * M_PI / 180);
	Y4t = MN4[0] * sin((V[18] - MN4[1]) * M_PI / 180) +
		M4[0] * sin((V[19] - M4[1]) * M_PI / 180) +
		MS4[0] * sin((V[20] - MS4[1]) * M_PI / 180);
	R4t = sqrt(X4t * X4t + Y4t * Y4t);
        if (R4t != 0.0)
        {
                cosphi4 = X4t / R4t;
                sinphi4 = Y4t / R4t;
        }
        else
        {
                cosphi4 = 0.0;
                sinphi4 = 0.0;
        }

	TidalHeight = Z0 + R1t * cosphi1 + R2t * cosphi2 +
							 R3t * cosphi3 + R4t * cosphi4;

	TidalHeight = TidalHeight * TheUnit; // mm or whatever other unit is asked for - 97.10.15 JGF
}



double TTidalHeightPrediction::TidalTime(double AYear, double AMonth,
													  double ADay, double ATideNumber,
													  char* AMT)   //Vers?o baseada no livro do SHOM - P. Duarte 27/1/98
{

	double MyTidalHeight, DeltaJPHI, DeltaT,
          PHI24, PHI0, TimeOfFirstTide, MyTideTime,MyFinalTideTime,ATideTime,
          AInitialTime, AFinalTime, AStep, MyTideNumber, ATrackTideNumber = 0.0;
   const double MyFinalHour = 24.00;
	int jota, n, MaximumNumberOfTides;

   if ( (ATideNumber > 0) && ( ATideNumber < 5 ) ) // rough check on call
		MyTideNumber = ATideNumber;
	else
	{
#ifdef __BORLANDC__
		::MessageBox( 0, "Invalid tide number, using first tide", "Tide DLL error", MB_OK);
#else   // __BORLANDC__
			cerr << "TTidalHeightPrediction::TidalTime - Invalid tide number, using first tide" << endl;
#endif  // __BORLANDC__
      Debugger(ATideNumber);
		MyTideNumber = 1;
	}
	PMT = AMT;
	strcpy( MyTideSituation, AMT );

	//Verifica??o da periodicidade da mar? para determinar o valor de jota

   TidePeriodicity(jota, MaximumNumberOfTides, AYear, AMonth, ADay);

   //C?lculo dos PHIs e do DeltaJ

   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 24.0);
   if (jota == 1)
   	PHI24 = RightAngle(cosphi2, sinphi2);
   else if (jota == 2)
   	PHI24 = RightAngle(cosphi3, sinphi3);

   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 0.0);
   if (jota == 1)
   	PHI0 = RightAngle(cosphi2, sinphi2);
   else if (jota == 2)
   	PHI0 = RightAngle(cosphi3, sinphi3);

   DeltaJPHI = DeltaPHI(PHI24, PHI0);

   //Determina??o do tipo e da hora aproximada da 1? mar?

   if ((PHI0 > -180) && (PHI0 < 0))  //A 1? mar? do dia ? uma Preia-mar
   {
   	TimeOfFirstTide = -24.0 * PHI0 / (360 * jota + DeltaJPHI);
      strcpy(TheTideSituation, HW); // high water
   }
   else if ((PHI0 > 0) && (PHI0 < 180)) //A 1? mar? do dia ? uma Baixa-mar
   {
   	TimeOfFirstTide = 24.0 * (180 - PHI0) / (360 * jota - DeltaJPHI);
      strcpy(TheTideSituation, LW); // low water
   }

   do
   {
      if (ATrackTideNumber == MyTideNumber)
      {
      	if (ATrackTideNumber == 1)
         	MyTideTime = TimeOfFirstTide;
         else
         	MyTideTime = TimeOfFirstTide + (ATrackTideNumber - 1.0) *
                         180.0 / (360.0 * jota + DeltaJPHI) * 24.0;
         if (MyTideTime <= MyFinalHour)
         {
      		strcpy( MyTideSituation, TheTideSituation);
         	strcpy(PMT, MyTideSituation);

            //C?lculo da hora exacta da mar?

            MyFinalTideTime = TidalTime(AYear, AMonth, ADay,MyTideTime);
            if (MyFinalTideTime < 0.0)
            	MyFinalTideTime = MyTideTime;
            return MyFinalTideTime;
         }
      }
      ATrackTideNumber++;
      if (ATrackTideNumber > 1)
      {
      	if (strcmp( TheTideSituation, HW ) == 0)
      		strcpy(TheTideSituation, LW);
      	else if ((strcmp( TheTideSituation, LW ) == 0))
      		strcpy(TheTideSituation, HW);
      }
   }
   while  (MyTideTime < MyFinalHour);
   // if no such tide
	strcpy(MyTideSituation, NW);
	strcpy(PMT, MyTideSituation);
	return NoTide;
}


void TTidalHeightPrediction::TidalHeightCalculation(double &TidalHeight, double AYear, double AMonth,
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
                                                    double AMS4Amplitude, double AMS4Phase, bool MS4TrueOrFalse)

{

   if (SaTrueOrFalse)
   {
   	Sa[0] = ASaAmplitude; Sa[1] = ASaPhase;
   }

   if (O1TrueOrFalse)
   {
   	O1[0] = AO1Amplitude; O1[1] = AO1Phase;
   }

   if (Q1TrueOrFalse)
   {
   	Q1[0] = AQ1Amplitude; Q1[1] = AQ1Phase;
   }

   if (K1TrueOrFalse)
   {
   	K1[0] = AK1Amplitude; K1[1] = AK1Phase;
   }

   if (M2TrueOrFalse)
   {
   	M2[0] = AM2Amplitude; M2[1] = AM2Phase;
   }

   if (N2TrueOrFalse)
   {
   	N2[0] = AN2Amplitude; N2[1] = AN2Phase;
   }

   if (S2TrueOrFalse)
   {
   	S2[0] = AS2Amplitude; S2[1] = AS2Phase;
   }

   if (MN4TrueOrFalse)
   {
   	MN4[0] = AMN4Amplitude; MN4[1] = AMN4Phase;
   }

   if (M4TrueOrFalse)
   {
   	M4[0] = AM4Amplitude; M4[1] = AM4Phase;
   }

   if (MS4TrueOrFalse)
   {
   	MS4[0] = AMS4Amplitude; MS4[1] = AMS4Phase;
   }

   double T, MyYear, MyMonth, MyDay, MyHour,
			 X1t, Y1t, //R1t, cosphi1, sinphi1,     Vou tornar estas vari?veis
			 X2t, Y2t, //R2t, cosphi2, sinphi2,     globais de modo a poder ter
			 X3t, Y3t, //R3t, cosphi3, sinphi3,     acesso aos seus valores
			 X4t, Y4t; //R4t, cosphi4, sinphi4;     no c?lculo das horas das PM e BM  P. Duarte 24/1/98

	MyYear = AYear; MyMonth = AMonth; MyDay = ADay; MyHour = AHour;


	T = JulianDaysFromTime0(MyYear, MyMonth, MyDay, MyHour);

	CalculateAstronomicParameters(T);
	for (int j = 0; j < 21; j++)
	{
		V[j] = AstronomicArgument(j, MyHour);
	}

	X1t = Sa[0] * cos((V[0] - Sa[1]) * M_PI / 180);
	Y1t = Sa[0] * sin((V[0] - Sa[1]) * M_PI / 180);
	R1t = sqrt(X1t * X1t + Y1t * Y1t);
	if (R1t != 0.0)
	{
		cosphi1 = X1t / R1t;
		sinphi1 = Y1t / R1t;
	}
	else
	{
		cosphi1 = 0.0;
		sinphi1 = 0.0;
	}

	X2t = K1[0] * cos((V[1] - K1[1]) * M_PI / 180) +
			O1[0] * cos((V[2] - O1[1]) * M_PI / 180) +
			Q1[0] * cos((V[3] - Q1[1]) * M_PI / 180) +
			P1[0] * cos((V[4] - P1[1]) * M_PI / 180) +
			o1nodalcorrection[0] * cos((V[5] - o1nodalcorrection[1]) * M_PI / 180) +
			k1nodalcorrection[0] * cos((V[6] - k1nodalcorrection[1]) * M_PI / 180);
	Y2t = K1[0] * sin((V[1] - K1[1]) * M_PI / 180) +
			O1[0] * sin((V[2] - O1[1]) * M_PI / 180) +
			Q1[0] * sin((V[3] - Q1[1]) * M_PI / 180) +
			P1[0] * sin((V[4] - P1[1]) * M_PI / 180) +
			o1nodalcorrection[0] * sin((V[5] - o1nodalcorrection[1]) * M_PI / 180) +
			k1nodalcorrection[0] * sin((V[6] - k1nodalcorrection[1]) * M_PI / 180);
	R2t = sqrt(X2t * X2t + Y2t * Y2t);
	cosphi2 = X2t / R2t;
	sinphi2 = Y2t / R2t;

	X3t = M2[0] * cos((V[7] - M2[1]) * M_PI / 180) +
		N2[0] * cos((V[8] - N2[1]) * M_PI / 180) +
		S2[0] * cos((V[9] - S2[1]) * M_PI / 180) +
		TwoN2[0] * cos((V[10] - TwoN2[1]) * M_PI / 180) +
		miu2[0] * cos((V[11] - miu2[1]) * M_PI / 180) +
		niu2[0] * cos((V[12] - niu2[1]) * M_PI / 180)+
		L2[0] * cos((V[13] - L2[1]) * M_PI / 180) +
		K2[0] * cos((V[14] - K2[1]) * M_PI / 180) +
		T2[0] * cos((V[15] - T2[1]) * M_PI / 180) +
		m2nodalcorrection[0] * cos((V[16] - m2nodalcorrection[1]) * M_PI / 180) +
		k2nodalcorrection[0] * cos((V[17] - k2nodalcorrection[1]) * M_PI / 180);
	Y3t = M2[0] * sin((V[7] - M2[1]) * M_PI / 180) +
		N2[0] * sin((V[8] - N2[1]) * M_PI / 180) +
		S2[0] * sin((V[9] - S2[1]) * M_PI / 180) +
		TwoN2[0] * sin((V[10] - TwoN2[1]) * M_PI / 180) +
		miu2[0] * sin((V[11] - miu2[1]) * M_PI / 180) +
		niu2[0] * sin((V[12] - niu2[1]) * M_PI / 180)+
		L2[0] * sin((V[13] - L2[1]) * M_PI / 180) +
		K2[0] * sin((V[14] - K2[1]) * M_PI / 180) +
		T2[0] * sin((V[15] - T2[1]) * M_PI / 180) +
		m2nodalcorrection[0] * sin((V[16] - m2nodalcorrection[1]) * M_PI / 180) +
		k2nodalcorrection[0] * sin((V[17] - k2nodalcorrection[1]) * M_PI / 180);
	R3t = sqrt(X3t * X3t + Y3t * Y3t);
	cosphi3 = X3t / R3t;
	sinphi3 = Y3t / R3t;

	X4t = MN4[0] * cos((V[18] - MN4[1]) * M_PI / 180) +
		M4[0] * cos((V[19] - M4[1]) * M_PI / 180) +
		MS4[0] * cos((V[20] - MS4[1]) * M_PI / 180);
	Y4t = MN4[0] * sin((V[18] - MN4[1]) * M_PI / 180) +
		M4[0] * sin((V[19] - M4[1]) * M_PI / 180) +
		MS4[0] * sin((V[20] - MS4[1]) * M_PI / 180);
	R4t = sqrt(X4t * X4t + Y4t * Y4t);
   if (R4t != 0.0)
	{
		cosphi4 = X4t / R4t;
		sinphi4 = Y4t / R4t;
	}
   else
	{
		cosphi4 = 0.0;
		sinphi4 = 0.0;
   }

	TidalHeight = Z0 + R1t * cosphi1 + R2t * cosphi2 +
							 R3t * cosphi3 + R4t * cosphi4;

	TidalHeight = TidalHeight * TheUnit; // mm or whatever other unit is asked for - 97.10.15 JGF
}

double TTidalHeightPrediction::TidalTime(double AYear, double AMonth,
                                         double ADay,
                                         double AAproximateTideTime)
{
	double MyTidalHeight, DeltaSigmaT, DeltaSigmaTPlusONE, ACorrectedTideTime,
          PHI1, PHI2, PHI3, PHI4, PHI24, PHI0, DeltaJPHI1, DeltaJPHI2, DeltaJPHI3, DeltaJPHI4,
          ControlSum;
   int jota, count;



   ACorrectedTideTime = AAproximateTideTime;

   jota = 0;
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 24.0);
   PHI24 = RightAngle(cosphi1, sinphi1);
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 0.0);
   PHI0 = RightAngle(cosphi1, sinphi1);
   DeltaJPHI1 = DeltaPHI(PHI24, PHI0);
   PHI1 = PHI0 + AAproximateTideTime / 24 * (360.0 * jota + DeltaJPHI1);

   jota = 1;
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 24.0);
   PHI24 = RightAngle(cosphi2, sinphi2);
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 0.0);
   PHI0 = RightAngle(cosphi2, sinphi2);
   DeltaJPHI2 = DeltaPHI(PHI24, PHI0);
   PHI2 = PHI0 + AAproximateTideTime / 24 * (360.0 * jota + DeltaJPHI2);

   jota = 2;
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 24.0);
   PHI24 = RightAngle(cosphi3, sinphi3);
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 0.0);
   PHI0 = RightAngle(cosphi3, sinphi3);
   DeltaJPHI3 = DeltaPHI(PHI24, PHI0);
   PHI3 = PHI0 + AAproximateTideTime / 24 * (360.0 * jota + DeltaJPHI3);

   jota = 4;
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 24.0);
   PHI24 = RightAngle(cosphi4, sinphi4);
   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 0.0);
   PHI0 = RightAngle(cosphi4, sinphi4);
   DeltaJPHI4 = DeltaPHI(PHI24, PHI0);
   PHI4 = PHI0 + AAproximateTideTime / 24 * (360.0 * jota + DeltaJPHI4);

   TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, AAproximateTideTime);
   count = 0; ControlSum = 0.0;
   do
   {
   	count ++;

      if (count > 1)
      	DeltaSigmaT = DeltaSigmaTPlusONE;
      else
      	DeltaSigmaT = 0.0;

      DeltaSigmaTPlusONE = - (R2t * sin(PHI2 * M_PI / 180) + 2 * R3t * sin(PHI3 * M_PI / 180) +
                              4 * R4t * sin (PHI4 * M_PI / 180)) /
                             (2 * M_PI * (R2t * cos(PHI2 * M_PI / 180) +
                             4 * R3t * cos(PHI3 * M_PI / 180) + 16 * R4t * cos (PHI4 * M_PI / 180)))
                             * 24.0;
      ControlSum = ControlSum + DeltaSigmaTPlusONE;
      if (ControlSum > 3.0)
      {
      	ACorrectedTideTime = AAproximateTideTime;
         return ACorrectedTideTime;
      }
      ACorrectedTideTime = ACorrectedTideTime + DeltaSigmaTPlusONE;
      PHI2 = PHI2 + 15 * DeltaSigmaTPlusONE;
      PHI3 = PHI3 + 15 * 2 * DeltaSigmaTPlusONE;
      PHI4 = PHI4 + 15 * 4 * DeltaSigmaTPlusONE;
   }
   while ((fabs(DeltaSigmaTPlusONE - DeltaSigmaT) > 0.033)||(count < 2));

   return ACorrectedTideTime;
}

double TTidalHeightPrediction::DeltaPHI(double APHI24, double APHI0)
{
	double PHI24, PHI0, Delta;
   PHI24 = APHI24; PHI0 = APHI0;

	if ((PHI24 - PHI0 - 360 > -180)&& (PHI24 - PHI0 - 360 < 180))
   	Delta = PHI24 - PHI0 - 360;
   else if ((PHI24 - PHI0 > -180)&& (PHI24 - PHI0 < 180))
   	Delta = PHI24 - PHI0;
   else
   	Delta = PHI24 - PHI0 + 360;
   return Delta;
}

double TTidalHeightPrediction::RightAngle(double ACosenus, double ASinus)
{
	double MyACOS, MyASIN;
	MyACOS = acos(ACosenus)* 180/ M_PI;
	MyASIN = asin(ASinus)* 180/ M_PI;

   if ((ACosenus >= 0.0) && (ASinus >= 0.0)) //?ngulo do 1? quadrante (0? a 90?)
   {
   	if ((MyACOS >= 0.0) && (MyACOS <= 90))
      	return(MyACOS);
      else if ((MyASIN >= 0.0) && (MyASIN <= 90))
      	return(MyASIN);
      else return(-MyACOS);
   }

   else if ((ACosenus < 0.0) && (ASinus >= 0.0)) //?ngulo do 2? quadrante (90? a 180?)
   {
   	if ((MyACOS > 90) && (MyACOS <= 180))
      	return(MyACOS);
      else if ((MyASIN > 90) && (MyASIN <= 180))
      	return(MyASIN);
      else return(-MyACOS);
   }

   else if ((ACosenus < 0.0) && (ASinus < 0.0)) //?ngulo do 3? quadrante (-90? a -180?)
   {
   	if ((MyACOS < -90) && (MyACOS >= -180))
      	return(MyACOS);
      else if ((MyASIN < -90) && (MyASIN >= -180))
      	return(MyASIN);
      else return(-MyACOS);
   }

//  teste redundante....
//
//   else if ((ACosenus >= 0.0) && (ASinus < 0.0)) //?ngulo do 4? quadrante (0? a -90?)
//   {
   	if ((MyACOS <= 0.0) && (MyACOS >= -90))
      	return(MyACOS);
      else if ((MyASIN < -90) && (MyASIN >= -180))
      	return(MyASIN);
      else return(-MyACOS);
//   }
}

void TTidalHeightPrediction::TidePeriodicity(int &jota, int &MaximumNumberOfTides,
                                             double AYear, double AMonth, double ADay)
{
	double MyTidalHeight;
	TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, 12.0);
   MaximumNumberOfTides = 0;
	if(R3t > 0.5 * R2t)       //A mar? ? semi-diurna
   {
   	jota = 2;
      MaximumNumberOfTides = 4;
   }
   else if(R3t < 0.25 * R2t) //A mar? ? diurna
   {
   	jota = 1;
      MaximumNumberOfTides = 2;
   }
   else if((R3t > 0.25 * R2t)&&(R3t < 0.5 * R2t))//Situa??o de aus?ncia de uma PM ou BM bem definida -(trata-se como semi-diurna)
   {
   	jota = 1;
      MaximumNumberOfTides = 4; //Verificar  !!!
   }
}



double TTidalHeightPrediction::JulianDaysFromTime0(double AYear, double AMonth,
																  double ADay, double AHour)
{
	return floor(30.6001 * (1 + AMonth + 12 * floor(1 / (AMonth + 1) + 0.7))) +
			 floor(365.25 * (AYear - floor(1 / (AMonth + 1) + 0.7))) +
			 ADay + AHour / 24 - 723258;
}


void TTidalHeightPrediction::CalculateAstronomicParameters(double AT)
{
	s = 78.16 + 13.17639673 * AT;
	h = 279.82 + 0.98564734 * AT;
	p = 349.50 + 0.11140408 * AT;
	Nlinha = 208.10 + 0.05295392 * AT;
	pp1 = 282.6 + 0.000047069 * AT;
}


double TTidalHeightPrediction::AstronomicArgument(int j, double AHour)
{
	double MyHour;
	MyHour = AHour;
	return 15.0 * jota[j] * MyHour + n1[j] * s + n2[j] * h + n3[j] * p +
			 n4[j] * Nlinha + n5[j] * pp1 + n6[j] * 90.0;
}

void TTidalHeightPrediction::TimeCorrection (double &AYear, double &AMonth, double &ADay, double &AHour)
{
	int IYear, IMonth, IDay;
	IYear = int(AYear); IMonth = int(AMonth);
	if (AHour < 0.0)
   {
   	AHour = 24.0 + AHour;
      ADay = ADay - 1;
      if (ADay <= 0)
      {
      	AMonth = AMonth - 1;
         if (AMonth < 1.0)
         {
         	AMonth = 12.0;
            AYear = AYear - 1;
         }
         IDay = PAstronomy->DaysOfTheMonth(IYear,IMonth);
         ADay = IDay;
      }
   }
   if (AHour > 24.0)
   {
   	AHour = AHour - 24.0;
      ADay = ADay + 1;
      if (ADay > PAstronomy->DaysOfTheMonth(IYear,IMonth))
      {
      	ADay = 1.0;
         AMonth = AMonth + 1;
         if (AMonth > 12.0)
         {
         	AMonth = 1.0;
            AYear = AYear + 1.0;
         }
      }
   }
}




// Debuggers

void TTidalHeightPrediction::Debugger(int bug)
{
#ifdef __BORLANDC__
	static char szBugD[20];
	sprintf(szBugD, "%20i", bug);
	MessageBox(0,(LPSTR(szBugD)),"Debug", MB_OK);
#else   // __BORLANDC__
	cerr << "TTidalHeightPrediction::Debugger - " << bug << endl;
#endif  // __BORLANDC__
}

void TTidalHeightPrediction::Debugger(long bug)
{
#ifdef __BORLANDC__
	static char szBugD[20];
	sprintf(szBugD, "%20i", bug);
	MessageBox(0,(LPSTR(szBugD)),"Debug", MB_OK);
#else   // __BORLANDC__
	cerr << "TTidalHeightPrediction::Debugger - " << bug << endl;
#endif  // __BORLANDC__
}

void TTidalHeightPrediction::Debugger(float bug)
{
#ifdef __BORLANDC__
	static char szBugD[20];
	sprintf(szBugD, "%20f", bug);
	MessageBox(0,(LPSTR(szBugD)),"Debug", MB_OK);
#else   // __BORLANDC__
	cerr << "TTidalHeightPrediction::Debugger - " << bug << endl;
#endif  // __BORLANDC__
}

void TTidalHeightPrediction::Debugger(double bug)
{
#ifdef __BORLANDC__
	static char szBugD[20];
	sprintf(szBugD, "%20f", bug);
	MessageBox(0,(LPSTR(szBugD)),"Debug", MB_OK);
#else   // __BORLANDC__
	cerr << "TTidalHeightPrediction::Debugger - " << bug << endl;
#endif  // __BORLANDC__
}



//-------------------Tudo o que est? para baixo ? para apagar--------


//Pedro stuff

double TTidalHeightPrediction::TidalTime(double AYear, double AMonth,
													  double ADay, double AInitialHour, double AFinalHour,
													  double AStep)
{
	double MyTidalHeight, MyTidalTime, MyYear, MyMonth, MyDay, MyInitialHour, AHour,
			 MyFinalHour,MyHour, MyStepReduction, MyStep, MyLastTidalHeight,
			 MyPresentTidalHeight;


	MyYear = AYear; MyMonth = AMonth; MyDay = ADay;
	MyInitialHour = AInitialHour;   //Time in hours
	MyFinalHour = AFinalHour;       //Time in hours

   //Checking calculation step

   MyStep = AStep;

   if (MyStep <= 0.0)
#ifdef __BORLANDC__
   	MessageBox(0, "Negative or zero calculation step", "Tide DLL error", MB_OK);
#else  // __BORLANDC__
		cerr << "TTidalHeightPrediction::TidalTime - Negative or zero calculation step" << endl;
#endif  // __BORLANDC__
	MyStepReduction = 0.1/*0.25*/; //This value may be changed

	strcpy(MyLastTideSituation,"Unknown");
	strcpy(MyPresentTideSituation,"Unknown");

   TimeCorrection(AYear, AMonth, ADay, MyInitialHour);

	TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, MyInitialHour);
   /*if (MyDay == 3)
   {
   	Debugger(AYear);Debugger(AMonth);Debugger(ADay);
   	Debugger(MyInitialHour);
      Debugger(MyTidalHeight);
   } */

	MyLastTidalHeight = MyTidalHeight;

	MyTidalTime = - 1.0;

	for (MyHour = MyInitialHour + MyStep; MyHour < MyFinalHour + MyStep;
		  MyHour = MyHour + MyStep)
	{

		//Debugger(MyHour);
      //Time checking
      AYear = MyYear; AMonth = MyMonth; ADay = MyDay; AHour = MyHour;
      TimeCorrection(AYear, AMonth, ADay, AHour);


		TidalHeightCalculation (MyTidalHeight, AYear, AMonth, ADay, AHour);

		MyPresentTidalHeight = MyTidalHeight;

      /*if (MyDay == 3)
      {
      	//Debugger(AYear);
   		//Debugger(AMonth);
   		//Debugger(ADay);
   		//Debugger(AHour);
         Debugger(MyInitialHour);
         Debugger(MyFinalHour);
         Debugger(MyHour);
         Debugger(MyStep);
         Debugger(MyLastTidalHeight);
			Debugger(MyPresentTidalHeight);
      } */


		if (MyLastTidalHeight > MyPresentTidalHeight)
			strcpy(MyPresentTideSituation,"Descending");
		else if (MyLastTidalHeight < MyPresentTidalHeight)
			strcpy(MyPresentTideSituation,"Rising");
		if (MyHour > MyInitialHour + MyStep)
		{
			//Debugger(MyHour);


			if ((strcmp(MyPresentTideSituation, MyLastTideSituation) != 0) && (ADay == MyDay))

			{
				//Debugger(MyHour);

				MyTidalTime = AHour - MyStep;
            if (MyTidalTime < 0.0)
            {
            	AYear = MyYear; AMonth = MyMonth; ADay = MyDay; AHour = MyHour;
               TimeCorrection(AYear, AMonth, ADay, MyTidalTime);
            }
            //Debugger(MyTidalTime);
				MyInitialHour = MyTidalTime - MyStep;
				MyFinalHour = MyTidalTime + MyStep;
				MyStep = MyStep * MyStepReduction;

				if ((MyStep < 0.01) && (ADay == MyDay))
				{
				 //Debugger(MyTidalTime);
					CurrentTidalTime = MyTidalTime;
					break;
				 //	return MyTidalTime;
				}

				else
				MyTidalTime = TidalTime(AYear, AMonth,
								  ADay, MyInitialHour, MyFinalHour, MyStep);
				//Recursive function calling
				//Each time the function is called MyInitialHour and MyFinalHour are closer and
				//MyStep is shorter.
			}

		}
		MyLastTidalHeight = MyPresentTidalHeight;
		strcpy(MyLastTideSituation,MyPresentTideSituation);
	}
	//return MyTidalTime;
	//}
	return MyTidalTime;
}

// Jo?o stuff

double TTidalHeightPrediction::TidalTime(double AYear, double AMonth,
													  double ADay, double AInitialHour, double AFinalHour,
													  double AStep, char* AMT)
{
	PMT = AMT;
	strcpy( MyTideSituation, AMT );
	double MyTidalHeight, MyTidalTime, MyYear, MyMonth, MyDay, MyInitialHour,
			 MyFinalHour,MyHour, MyStepReduction, MyStep, MyLastTidalHeight,
			 MyPresentTidalHeight;
	MyYear = AYear;
	MyMonth = AMonth;
	MyDay = ADay;
	MyInitialHour = AInitialHour;   //Time in hours
	MyFinalHour = AFinalHour;       //Time in hours
	if (MyFinalHour - MyInitialHour < AStep)
		MyStep = MyFinalHour - MyInitialHour;
	else
		MyStep = AStep;
	MyStepReduction = 0.25; //This value may be changed
	strcpy(MyLastTideSituation, UW); // Unknown
	strcpy(MyPresentTideSituation, UW); // Unknown
	strcpy( MyTideSituation, MyPresentTideSituation);
	strcpy(PMT, MyTideSituation);
	TidalHeightCalculation (MyTidalHeight, MyYear, MyMonth, MyDay, MyInitialHour);
	MyLastTidalHeight = MyTidalHeight;
	MyTidalTime = NoTide;
	for (MyHour = MyInitialHour + MyStep; MyHour < MyFinalHour + MyStep;
		  MyHour = MyHour + MyStep)
	{
		TidalHeightCalculation (MyTidalHeight, MyYear, MyMonth, MyDay, MyHour);
		MyPresentTidalHeight = MyTidalHeight;

      // this was the other way around, bugged JGF 97.11.03
		if (MyLastTidalHeight > MyPresentTidalHeight)
			strcpy( MyPresentTideSituation, FW );
		else if (MyLastTidalHeight < MyPresentTidalHeight)
			strcpy( MyPresentTideSituation, EW );


		if (MyHour > MyInitialHour + MyStep)
		{
			// The line below seems a very inefficient algorithm, comparing strings - JGF 97.11.02
			if (strcmp(MyPresentTideSituation, MyLastTideSituation) != 0)
			{

				MyTidalTime = MyHour - MyStep;

				MyInitialHour = MyTidalTime - MyStep;
				MyFinalHour = MyTidalTime + MyStep;
				MyStep = MyStep * MyStepReduction;

				if (MyStep < 0.01)
				{
					CurrentTidalTime = MyTidalTime;
					break;
				}

				else //Recursive function call - Each time the function is called MyInitialHour and MyFinalHour are closer and MyStep is shorter.
					MyTidalTime = TidalTime(MyYear, MyMonth,                   // Don't forget to call with AMT - JGF 97.11.01
													MyDay, MyInitialHour, MyFinalHour,
													MyStep, AMT);

			}

		}
		MyLastTidalHeight = MyPresentTidalHeight;
		strcpy(MyLastTideSituation,MyPresentTideSituation);
	}

	if (strcmp( MyPresentTideSituation, LW ) == 0) // we were ebbing
	{
		strcpy( MyTideSituation, MyPresentTideSituation);
		strcpy(PMT, MyTideSituation);
	}
	else
	if (strcmp( MyPresentTideSituation, HW ) == 0) // we were ebbing
	{
		strcpy( MyTideSituation, MyPresentTideSituation);
		strcpy(PMT, MyTideSituation);
	}
	else
	if (strcmp( MyPresentTideSituation, FW ) == 0) // we were flooding
	{
		strcpy(MyPresentTideSituation, HW);
		strcpy( MyTideSituation, MyPresentTideSituation);
		strcpy(PMT, MyTideSituation);
	}
	else
	if (strcmp( MyPresentTideSituation, EW ) == 0) // we were ebbing
	{
		strcpy(MyPresentTideSituation, LW );
		strcpy( MyTideSituation, MyPresentTideSituation);
		strcpy(PMT, MyTideSituation);
	}

	if ( MyTidalTime == NoTide )
	{
		strcpy(MyPresentTideSituation, NW );
		strcpy( MyTideSituation, MyPresentTideSituation);
		strcpy(PMT, MyTideSituation);
	}

	return MyTidalTime;
}

// JGF stuff - 97.11.03

void TTidalHeightPrediction::TideTime(int ASlope)
{
	double TempHeight, Delta = 0.001;
	TidalHeightCalculation (CurrentHeight,
									InitialYear,
									InitialMonth,
									InitialDay,
									InitialHour );

	if ( InitialDay == 1 )
	{
		// Debugger ( ASlope );
		// Debugger (  InitialHeight );
		// Debugger (  CurrentHeight );
		// Debugger (  InitialHour );
	}

	if ( ASlope == 1 ) // flooding
	{
		if  ( CurrentHeight >= InitialHeight )
		{
			TidalHeightCalculation (TempHeight,
											InitialYear,
											InitialMonth,
											InitialDay,
											InitialHour + Delta);
			if ( TempHeight > CurrentHeight ) // still flooding
				InitialHeight = CurrentHeight;
			else                              // Past rise
				if ( CalculationStep >= Tolerance )
				{
					InitialHour = InitialHour - CalculationStep;
					CalculationStep = CalculationStep / 2;
				}
		}
		else // past rise
			if ( CalculationStep >= Tolerance )
			{
				InitialHour = InitialHour - CalculationStep;
				CalculationStep = CalculationStep / 2;
			}
	}
	else
	if (ASlope == -1) // ebbing
	{
		if  ( CurrentHeight <= InitialHeight )
		{
			TidalHeightCalculation (TempHeight,
											InitialYear,
											InitialMonth,
											InitialDay,
											InitialHour + Delta);
			if ( TempHeight < CurrentHeight ) // still ebbing
				InitialHeight = CurrentHeight;
			else                              // Past fall
				if ( CalculationStep >= Tolerance )
				{
					InitialHour = InitialHour - CalculationStep;
					CalculationStep = CalculationStep / 2;
				}
		}
		else // past fall
			if ( CalculationStep >= Tolerance )
			{
				InitialHour = InitialHour - CalculationStep;
				CalculationStep = CalculationStep / 2;
			}
	}
	InitialHour = InitialHour + CalculationStep;

	if (InitialHour < 0 )
		InitialHour == 0;
	else if ( InitialHour > 24.00 ) // end of day
	{
		InitialHour = NoTide;
		CalculationStep = Tolerance - Delta; // force break
	}
	if ( CalculationStep >= Tolerance )
		TideTime(ASlope);
}


// overloaded, JGF routine 97.11.02

double TTidalHeightPrediction::TidalTime(double AYear, double AMonth,
													  double ADay, double AInitialHour,
													  double AStep, char* AMT)
{
	const double MyFinalHour = 24.00, Delta = 0.05, LittleBit = 0.05;
	const int NumberOfIterations = 1000;
	PMT = AMT;
	strcpy( MyTideSituation, AMT );

	InitialYear = AYear; InitialMonth = AMonth; InitialDay = ADay; InitialHour = AInitialHour;
	if ( (MyFinalHour-InitialHour) < AStep )
		CalculationStep = (MyFinalHour-InitialHour);
	else
		CalculationStep = AStep;

	TidalHeightCalculation (InitialHeight, InitialYear, InitialMonth, InitialDay, InitialHour );
	// test to see flood or ebb
	int MySlope;
	double PreviousHeight, NextHeight;
	if ( InitialHour - LittleBit >= 0 )
		TidalHeightCalculation (PreviousHeight, InitialYear, InitialMonth, InitialDay, InitialHour - LittleBit );
	else
	{
		if (InitialDay > 1)
			TidalHeightCalculation (PreviousHeight, InitialYear, InitialMonth, InitialDay-1, MyFinalHour - LittleBit );
		else // First of the month
		{
			if (InitialMonth > 1)
			{
				// first find number of days in previous month
				TidalHeightCalculation (PreviousHeight, InitialYear, InitialMonth-1, InitialDay-1, MyFinalHour - LittleBit );
			}
			else // January
			{
				// test previous year
			}
		}
	}
	TidalHeightCalculation (NextHeight, InitialYear, InitialMonth, InitialDay, InitialHour + LittleBit );

	// what about beginning of the day being the asymptote? e.g. HW at 00.00 h???
	if ( NextHeight > InitialHeight)
	{
		MySlope = 1; // flooding
		strcpy(TheTideSituation, HW); // High water
	}
	else
	if ( NextHeight < InitialHeight)
	{
		MySlope = -1; // ebbing
		strcpy(TheTideSituation, LW); // Low water
	}
	if ( NextHeight == InitialHeight) // we are on the flat (can this ever occur with a time difference???)
	{
//		MySlope = 0; // flat
		TidalHeightCalculation (NextHeight, InitialYear, InitialMonth, InitialDay, InitialHour + Delta );
		if ( NextHeight < InitialHeight)
			strcpy(TheTideSituation, HW); // Low water
		else
			strcpy(TheTideSituation, LW); // Low water
		return InitialHour;
	}

	strcpy( MyTideSituation, TheTideSituation);
	strcpy(PMT, MyTideSituation);


	if (
		 (InitialHour > 0 /* Delta*/ ) &&
		 (InitialHour < 24 - Delta )
		)
		InitialHour = InitialHour + Delta;


	CurrentHeight = InitialHeight;
	Tolerance = CalculationStep/NumberOfIterations;

	if (InitialDay == 5)
	{
//		MessageBox(0,MyTideSituation,"",MB_OK);
//		Debugger(InitialDay);
//		Debugger(InitialHour);
	}

	TideTime(MySlope);

	if (InitialDay == 5)
	{
		// MessageBox(0,MyTideSituation,"",MB_OK);
		// Debugger(InitialDay);
		// Debugger(InitialHour);
	}


	return InitialHour;
}


bool TTidalHeightPrediction::FindPeriod(double AYear,
													 double AMonth,
													 int ANumberOfDays)
{
	const double FifteenMinutes = 0.25;
	double j, MaxHour = 24, PrevHeight, ThisHeight, NextHeight,
			 ThisTideTime, MyTideInterval;
	TheInterval = MaxHour; // approximate minimum interval

	for (int i = 1; i <= ANumberOfDays; i++)
	{
		j = 0; ThisTideTime = NoTide;
		do
		{
			TidalHeightCalculation (PrevHeight, AYear, AMonth, i, j );
			TidalHeightCalculation (ThisHeight, AYear, AMonth, i, j + FifteenMinutes );
			TidalHeightCalculation (NextHeight, AYear, AMonth, i, j + 2*FifteenMinutes );
			if (
				 ( (PrevHeight < ThisHeight) && (NextHeight < ThisHeight) ) || // gone thru HW
				 ( (PrevHeight > ThisHeight) && (NextHeight > ThisHeight) )    // gone thru LW
				)
			{
				if (ThisTideTime == NoTide)            // First tide of the day
					ThisTideTime = j + FifteenMinutes;
				else
				{
					MyTideInterval = j + FifteenMinutes - ThisTideTime; // subsequent tides
					if ( MyTideInterval < TheInterval )
						TheInterval = MyTideInterval;
					ThisTideTime = j + FifteenMinutes;
				}

			}
			j = j + FifteenMinutes;
		}
		while ( j + 2*FifteenMinutes < MaxHour);
	}
	TheInterval = TheInterval * 0.9; // 90% of maximum interval - conservative
	// Debugger( TheInterval );
	return TRUE;
}

/*
double TTidalHeightPrediction::TidalTime(double AYear, double AMonth,
													  double ADay, int ATideNumber,
													  char* AMT)
{
	const double MyFinalHour = 24.00, TwoMinutes = 0.05, OneMinute = 0.025;
	// TwoMinutes = 0.2, OneMinute = 0.1;
	double j, PrevHeight, ThisHeight, NextHeight, MyStep, MyTideTime;
	int MyTideNumber, TrackTideNumber = 0;
	if ( (ATideNumber > 0) && ( ATideNumber * TheInterval < 24 ) ) // rough check on call
		MyTideNumber = ATideNumber;
	else
	{
		::MessageBox( 0, "Invalid tide number, using first tide", "Tide DLL error", MB_OK);
		MyTideNumber = 1;
	}
	PMT = AMT;
	strcpy( MyTideSituation, AMT );

	InitialYear = AYear; InitialMonth = AMonth; InitialDay = ADay; InitialHour = 0;

	j = 0; MyStep = TwoMinutes; MyTideTime = NoTide;
	do
	{
   	// Debugger(j);
		TidalHeightCalculation (PrevHeight, AYear, AMonth, ADay, j );
		MyStep = TwoMinutes;
		TidalHeightCalculation (ThisHeight, AYear, AMonth, ADay, j + TwoMinutes);
		TidalHeightCalculation (NextHeight, AYear, AMonth, ADay, j + 2*TwoMinutes);
		if (
			 ( (PrevHeight < ThisHeight) && (NextHeight < ThisHeight) ) || // gone thru HW
			 ( (PrevHeight > ThisHeight) && (NextHeight > ThisHeight) )    // gone thru LW
			)
		{
			// found a tide
			if ( (PrevHeight < ThisHeight) && (NextHeight < ThisHeight) ) // HW
			{
				TidalHeightCalculation (PrevHeight, AYear, AMonth, ADay, j + OneMinute);
				TidalHeightCalculation (NextHeight, AYear, AMonth, ADay, j + OneMinute + TwoMinutes);
				if (PrevHeight > ThisHeight)
					MyTideTime = j + OneMinute;
				else
				if (NextHeight > ThisHeight)
					MyTideTime = j + OneMinute + TwoMinutes;
				else
					MyTideTime = j + TwoMinutes;
				strcpy(TheTideSituation, HW); // high water
			}
			else                                                          // LW
			{
				TidalHeightCalculation (PrevHeight, AYear, AMonth, ADay, j + OneMinute);
				TidalHeightCalculation (NextHeight, AYear, AMonth, ADay, j + OneMinute + TwoMinutes);
				if (PrevHeight < ThisHeight)
					MyTideTime = j + OneMinute;
				else
				if (NextHeight < ThisHeight)
					MyTideTime = j + OneMinute + TwoMinutes;
				else
					MyTideTime = j + TwoMinutes;
				strcpy(TheTideSituation, LW); // low water
			}

			TrackTideNumber++;
			if (TrackTideNumber == MyTideNumber)
			{
				strcpy( MyTideSituation, TheTideSituation);
				strcpy(PMT, MyTideSituation);
				return MyTideTime;
			}
         MyStep = TheInterval;
		}
		j = j + MyStep;
	}
	while ( j + 2 * TwoMinutes < MyFinalHour);
	// if no such tide
	strcpy( MyTideSituation, NW);
	strcpy(PMT, MyTideSituation);
	return NoTide;
}
*/

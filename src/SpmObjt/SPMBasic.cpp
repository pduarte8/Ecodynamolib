// SPM object CPP code file
// @ J. Gomes Ferreira
// Rev. A 96.02.27

/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"
#include "iodll.h"

// TSPMBasic Class
//
// Constructors invoked outside EcoDyn shell...
//

TSPMBasic::TSPMBasic(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
        double aSPMConcentration[], double aSPMLoad[],
        int nParams, double localLatitude, double localHeight, double localSalt,
        double localTemperature, double turbulenceFactor, double aSPMRessuspension[])
    :TSPM(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildSPMBasic();

	NumberOfParameters = nParams;
	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
            SPMConcentration[j] = aSPMConcentration[j];
            SPMLoad[j] = aSPMLoad[j];
            SPMRessuspension[j] = aSPMRessuspension[j];
    }
    RiverSPM = riverSPM;
    OceanSPM = oceanSPM;
    LocalLatitude = localLatitude;
    LocalHeight = localHeight;
    LocalSalt = localSalt;
    LocalTemperature = localTemperature;
    TurbulenceFactor = turbulenceFactor;

    PosBuildSPMBasic();
}

TSPMBasic::TSPMBasic(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
                        :TSPM(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildSPMBasic();
}
// ...

TSPMBasic::TSPMBasic(TEcoDynClass* APEcoDynClass, char* className)
            :TSPM(APEcoDynClass, className)
{
    // Receive pointer to integrate
    BuildSPMBasic();
}

void TSPMBasic::PreBuildSPMBasic()
{
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes

	VariableCount = 0;
	TimeStep = MyPEcoDynClass->GetTimeStep();

	// Fluxes and conversions
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		SPMFlux[i] = 0;
		SPMRessuspension[i] = 0;
		SPMSedimentationRate[i] = 0;
		TotalExtinctionCoefficient[i] = 0.25;       // Cannot be zero, FP divide by zero in light
		ChlorophyllExtinctionCoefficient[i] = 0.25; // Cannot be zero, FP divide by zero in light
	}

//	GrainClasses = 7;  // 5 classes + marine snow + floatables
	GrainClasses = 6;  // 5 classes + marine snow
	POCFraction = 0.043; // Calibrated 0.043 - 94.03.24 - was 0.2 before
	ParticleData = new ParticleRecord[GrainClasses];

	// All this is Tagus specific (until ENDTAGUS) - Put in Tagus object 96.02.29

	LocalLatitude = 3900;   // Latitude in degrees and minutes
	LocalHeight = 0;        // Sea level
	LocalSalt = 15;         // psu
	LocalTemperature = 15;  // oC
	TurbulenceFactor = 0.8; // 80% turbulence - or: The SPM can only sediment during 20% of
									// tidal cycle

	// New SPM ressuspension stuff - JGF 96.05.03

	ParameterCount = 0;
}

void TSPMBasic::BuildSPMBasic()
{
    PreBuildSPMBasic();

    int X,Y, XV, YV, XL, YL;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "SuspendedMatter variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSPMBasic::BuildSPMBasic - SuspendedMatter variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        char MyVariable[65];
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);
                if (strcmp(MyVariable, "Suspended matter") == 0)
                {
                    //Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, RiverSPM);  // mg l-1
                    PReadWrite->ReadNumber(X+4, i, OceanSPM);  // mg l-1
                    if (!PReadWrite->FindString("SPM concentration values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, SPMConcentration[j]);  // mg l-1
                            PReadWrite->ReadNumber(X+NumberOfBoxes+5+j, i, SPMLoad[j]);  // mg l-1
                        }
                    }
                    else
                    {
                        if (!PReadWrite->FindString("SPM load values", XL, YL))
                        {
                            XL = XV + 1;
                            YL = Y;
                        }
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, SPMConcentration[j]);  // mg l-1
                            PReadWrite->ReadNumber(XL, YL + 1 + j, SPMLoad[j]);  // mg l-1
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - SPM", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSPMBasic::BuildSPMBasic - Variables: Undefined object - SPM" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("SuspendedMatter");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "SuspendedMatter parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSPMBasic::BuildSPMBasic - SuspendedMatter parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MySPMRessuspension = 0;
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "SPM Ressuspension") == 0)
                {
                    if (!PReadWrite->FindString("SPM Ressuspension values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+3+j, i, SPMRessuspension[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, SPMRessuspension[j]);
                        }
                    }
                }
                else if (strcmp(MyParameter, "Latitude") == 0)
                    PReadWrite->ReadNumber(X+3, i, LocalLatitude);
                else if (strcmp(MyParameter, "Height") == 0)
                    PReadWrite->ReadNumber(X+3, i, LocalHeight);
                else if (strcmp(MyParameter, "Salinity") == 0)
                    PReadWrite->ReadNumber(X+3, i, LocalSalt);
                else if (strcmp(MyParameter, "Temperature") == 0)
                    PReadWrite->ReadNumber(X+3, i, LocalTemperature);
                else if (strcmp(MyParameter, "Turbulence") == 0)
                    PReadWrite->ReadNumber(X+3, i, TurbulenceFactor);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - SPM", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSPMBasic::BuildSPMBasic - Parameters: Undefined object - SPM" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    PosBuildSPMBasic();
}

void TSPMBasic::PosBuildSPMBasic()
{
	// Initialize physical variables for Stokes Equation
	LocalGravity = MyPEcoDynClass->GetGravity(LocalLatitude,LocalHeight);
	LocalDensity = MyPEcoDynClass->GetDensity(LocalSalt,LocalTemperature);
	LocalViscosity = MyPEcoDynClass->GetViscosity(LocalSalt,LocalTemperature);

	// Initialize array for particle characteristics
	for (int i = 0; i < GrainClasses; i++)
	{
		ParticleData[i].SettlingVelocity = 0;

		if (i == 0) //clay
		{
			ParticleData[i].Density         = 2600;  // Particle density
			ParticleData[i].ESD             = 0.003; // Equivalent spherical diameter
			ParticleData[i].phi             = 8;     // Phi-Wentworth class
			ParticleData[i].PercentOrganic  = 0;     // mean % organic matter
		}
		else if (i == 1) // Silt
		{
			ParticleData[i].Density         = 2600;  // Particle density
			ParticleData[i].ESD             = 0.040; // Equivalent spherical diameter
			ParticleData[i].phi             = 6;     // Phi-Wentworth class
			ParticleData[i].PercentOrganic = 0;     // mean % organic matter
		}
		else if (i == 2) // Fine sand
		{
			ParticleData[i].Density         = 2600;  // Particle density
			ParticleData[i].ESD             = 0.200; // Equivalent spherical diameter
			ParticleData[i].phi             = 2.5;   // Phi-Wentworth class
			ParticleData[i].PercentOrganic = 0;     // mean % organic matter
		}
		else if (i == 3) // Coarse sand
		{
			ParticleData[i].Density         = 2600;  // Particle density
			ParticleData[i].ESD             = 0.75;  // Equivalent spherical diameter
			ParticleData[i].phi             = 0.5;   // Phi-Wentworth class
			ParticleData[i].PercentOrganic = 0;     // mean % organic matter
		}
		else if (i == 4) // Gravel
		{
			ParticleData[i].Density         = 2600;  // Particle density
			ParticleData[i].ESD             = 3;     // Equivalent spherical diameter
			ParticleData[i].phi             = -1.5;  // Phi-Wentworth class
			ParticleData[i].PercentOrganic = 0;     // mean % organic matter
		}
		else if (i == 5) // Marine snow
		{
			ParticleData[i].Density         = 2600;  // Particle density
			ParticleData[i].ESD             = 30;    // Equivalent spherical diameter
			ParticleData[i].phi             = -4.0;  // Phi-Wentworth class
			ParticleData[i].PercentOrganic = 0;     // mean % organic matter
		}

	}

	SettlingSpeed();
}


double TSPMBasic::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "SPM Ressuspension") == 0)
        value = SPMRessuspension[0];
    else if (strcmp(MyParameter, "Latitude") == 0)
        value = LocalLatitude;
    else if (strcmp(MyParameter, "Height") == 0)
        value = LocalHeight;
    else if (strcmp(MyParameter, "Salinity") == 0)
        value = LocalSalt;
    else if (strcmp(MyParameter, "Temperature") == 0)
        value = LocalTemperature;
    else if (strcmp(MyParameter, "Turbulence") == 0)
        value = TurbulenceFactor;
    else
        value = 0.0;
    return value;
}

bool TSPMBasic::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "SPM Ressuspension") == 0) {
        for (j = 0; j < NumberOfBoxes; j++)
            SPMRessuspension[j] = value;
    }
    else if (strcmp(MyParameter, "Latitude") == 0)
        LocalLatitude = value;
    else if (strcmp(MyParameter, "Height") == 0)
        LocalHeight = value;
    else if (strcmp(MyParameter, "Salinity") == 0)
        LocalSalt = value;
    else if (strcmp(MyParameter, "Temperature") == 0)
        LocalTemperature = value;
    else if (strcmp(MyParameter, "Turbulence") == 0)
        TurbulenceFactor = value;
    else
        rc = false;

    return rc;
}


// AP, 2007.07.11
bool TSPMBasic::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    bool spmValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Suspended matter") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverSPM, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanSPM, 8);
            spmValues = true;
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save others variables' values
     */
    if (spmValues) {
        PReadWrite->WriteCell("SPM concentration values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("SPM load values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            PReadWrite->WriteCell(SPMConcentration[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(SPMLoad[j], 8);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TSPMBasic::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    bool spmValues = false;
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "SPM Ressuspension") == 0)
        {
            PReadWrite->WriteCell(SPMRessuspension[0], 8);
            spmValues = true;
        }
        else if (strcmp(ParametersNameArray[i], "Latitude") == 0)
        {
            PReadWrite->WriteCell(LocalLatitude, 3);
        }
        else if (strcmp(ParametersNameArray[i], "Height") == 0)
        {
            PReadWrite->WriteCell(LocalHeight, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Salinity") == 0)
        {
            PReadWrite->WriteCell(LocalSalt, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Temperature") == 0)
        {
            PReadWrite->WriteCell(LocalTemperature, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Turbulence") == 0)
        {
            PReadWrite->WriteCell(TurbulenceFactor, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save other parameters' initial values
     */
    if (spmValues) {
        PReadWrite->WriteCell("SPM Ressuspension values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            PReadWrite->WriteCell(SPMRessuspension[j], 8);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TSPMBasic::Deposit()
{
    double SedimentationRate, MySalt;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), MySalt,i,"Salinity",ObjectCode);
            LocalViscosity = GetViscosity(MySalt, LocalTemperature);
            SettlingSpeed();
        }
        SPMSedimentationRate[i] = 0; // initialise this before k loop
        for (int k = 0; k < GrainClasses; k++)
        {
            if (
                     (ParticleData[k].SettlingVelocity
                     / MyPEcoDynClass->GetBoxDepth(i) >= 1)
                    )
                    SedimentationRate = 1;
            else
                    SedimentationRate = ParticleData[k].SettlingVelocity
                                                                      / MyPEcoDynClass->GetBoxDepth(i);
            if (SPMConcentration[i] > 0)
            {
                    SPMFlux[i] = SPMFlux[i] - SPMFraction ( k , SPMConcentration[i] )* SedimentationRate;
                    SPMSedimentationRate[i] = SPMSedimentationRate[i] // mg l-1 day-1
                                              + SPMFraction ( k , SPMConcentration[i] ) * SedimentationRate;
            }
        }
    }
}

void TSPMBasic::Turbulence()
{
	// SPM flux is negative after deposition - if we subtract a fraction which
	// does not settle due to turbulent mixing, we increase the concentration
	// of suspended solids in the water column - JGF 94.04.11
	// Note: the second term on the RHS is negative, because spm_flux^[i,j] is
	// always negative after depositing, since it is initialized as zero}
	for (int i = 0; i < NumberOfBoxes; i++)
		if (SPMConcentration[i] > 0)
			SPMFlux[i] = SPMFlux[i] - (SPMFlux[i] * TurbulenceFactor);
}



void TSPMBasic::SettlingSpeed()
{
    const double two = 2,
    ninths = 9;
    // Stokes equation - results in m day-1
    for (int i = 0; i < GrainClasses; i++)
            ParticleData[i].SettlingVelocity  = two * LocalGravity * DAYSTOSECONDS * (ParticleData[i].Density- LocalDensity)
                                                * sqr(ParticleData[i].ESD * 0.001/2.0)/ (ninths * (LocalViscosity / 1000.0));
}

double TSPMBasic::SPMFraction(int k,double TotalSPM)
{

	// Data from Stumm and others p. 655 - empirical coefficient Alpha from
	// JGF 93.10.27, for diameter in mm and SPM in mg l-1 - the proportion
	// of spm in each fraction is calculated by mass - see STUMMVOL.XLS
	// This already includes fractioning due to flocculation

	const double AlphaFraction = 2.0133335,
        BetaFraction = -4;

	double UnitVolume,UnitMass;

	UnitVolume = exponent( (ParticleData[k].ESD * 0.001/2), 3)*4.0 * M_PI/3;

	UnitMass = UnitVolume * ParticleData[k].Density * 1000.0;

	return UnitMass * AlphaFraction * TotalSPM * exponent( ParticleData[k].ESD,BetaFraction );
}

void TSPMBasic::ExtinctionCoefficientKTot()
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		//From JEEP92 paper - Equation corrected in 94.04.11 JGF}
		TotalExtinctionCoefficient[i] = (1.7/exp(2.034))* exp(0.734 * log(SPMConcentration[i]));
		// Alternative expression - From BarcaWin - correlation of k and SPM for SPM < 80 mg l-1}
		//  TotalExtinctionCoefficient[i] = 0.055721 * SPMConcentration[i] + 0.913152;
	}
}


void TSPMBasic::ExtinctionCoefficientKChl()
{
    double MyChl;
    // if ugChla_per_liter < 50 then ??
    TEcoDynClass* MyPhytoplanktonPointer = MyPEcoDynClass->GetPhytoplanktonPointer();

    for (int i = 0; i < NumberOfBoxes; i++)
    {
            if (MyPhytoplanktonPointer != NULL)
                    MyPhytoplanktonPointer->Inquiry(GetEcoDynClassName(), MyChl,i,"Phytoplankton biomass",ObjectCode);
            else return;

            if (MyChl <= 0)
            {
#ifdef __BORLANDC__
                    MessageBox(0, "Chlorophyll a is zero or negative", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
										cerr << "TSPMBasic::ExtinctionCoefficientKChl - Chlorophyll a is zero or negative" << endl;
#endif  // __BORLANDC__
                    return;
            }
            ChlorophyllExtinctionCoefficient[i] = 0.04        // f?rmula tirada de Parsons et al.,
                                                                                                              + 0.0088    // Biological Oceanographic Processes
                                                                                                              * MyChl
                                                                                                              + 0.054
                                                                                                              * exp (2.0/3.0 * log(MyChl));

    }
    // else ChlorophyllExtinctionCoefficient[i] := exp(0.035*ugChla_per_liter);

}


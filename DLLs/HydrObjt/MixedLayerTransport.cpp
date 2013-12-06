// Hydrodynamic object CPP code file
// @ J. Gomes Ferreira
// Rev. A 95.12.31
// Rev. B 97.11.28
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"
#include "globals.h"

// TMixedLayerTransport Class

// Mixed layer transport class - 96.02.14

// Cálculo do transporte vertica através da camada de mistura
// forçado pelo vento e pelas trocas de energia através da superfície.
// Este algoritmo encontra-se descrito em Janowitz & Kamykowski (1991),
// Journal of Plankton Research, 13: 983-1002

// Public functions

TMixedLayerTransport::TMixedLayerTransport(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA pVarsNames[], double aSalt[], double aMixedLayerDepth[],
        double aWaterDensity[], double aUVelocity[], double aVVelocity[],
        double aRichardsonNumber[], double aExchangedFraction[],
        int nParams, double latitude, double coefManning)
    :TTransport(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildMixedLayerTransport();

	// Read in the parameters
    NumberOfParameters = nParams;
    MyLatitude = latitude;
    CoefManning = coefManning;

	// Read in the variables
    NumberOfVariables = nVars;

    // Create array for state variable names
	VariableNameArray = new VNA[NumberOfVariables];

	// read in the variable names
	for (int i = 0; i < NumberOfVariables; i++) {
		strcpy(VariableNameArray[i], pVarsNames[i]);
    }

	// Read in the variables values
	for (int j = 0; j < NumberOfBoxes; j++)
	{
        Salt[j] = aSalt[j];
        MixedLayer[j] = aMixedLayerDepth[j];
		WaterDensity[j] = aWaterDensity[j];
        UVelocity[j] = aUVelocity[j];
        VVelocity[j] = aVVelocity[j];
        RichardsonNumber[j] = aRichardsonNumber[j];
        ExchangedFraction[j] = aExchangedFraction[j];
    }
}

TMixedLayerTransport::TMixedLayerTransport(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
  					 :TTransport(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildMixedLayerTransport();
}

TMixedLayerTransport::TMixedLayerTransport(TEcoDynClass* PEcoDynClass, char* className)
         :TTransport(PEcoDynClass, className)
{
    BuildMixedLayerTransport();
}

void TMixedLayerTransport::PreBuildMixedLayerTransport()
{
	int i;
	MixedLayerDepth = 0.0;
    CoefManning = 0.0;
    gravity = 9.80665;

	ConvectiveBox = 0;
	Convection = false;
	ParameterCount = 0;

	if (NumberOfBoxes > 0)
	{
		UVelocity = new double[NumberOfBoxes];
		VVelocity = new double[NumberOfBoxes];
		WaterDensity = new double[NumberOfBoxes];
		RichardsonNumber = new double[NumberOfBoxes];
		ExchangedFraction = new double[NumberOfBoxes];
		for (i = 0; i < NumberOfBoxes; i++)
		{
			UVelocity[i] = 0;
			VVelocity[i] = 0;
			WaterDensity[i] = 0;
			RichardsonNumber[i] = 0;
			ExchangedFraction[i] = 0;
		}
	}
	else {
#ifdef __BORLANDC__
		MessageBox(0,
					  "Hydrodynamic array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TMixedLayerTransport::PreBuildMixedLayerTransport - hydrodynamic array not dimensioned" << endl;
#endif  // __BORLANDC__
        return;
    }
}

void TMixedLayerTransport::BuildMixedLayerTransport()
{
    PreBuildMixedLayerTransport();

    int X,Y, XV, YV;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Transport variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
       cerr << "TMixedLayerTransport::BuildMixedLayerTransport - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);

            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);

                if (strcmp(MyVariable, "Salinity") == 0)
                {
                    if (PReadWrite->FindString("Salinity values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Salt[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, Salt[j]);
                    }
                }
                else if (strcmp(MyVariable, "Mixed layer depth") == 0)
                {
                    if (PReadWrite->FindString("Mixed layer depth values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, MixedLayer[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, MixedLayer[j]);
                    }
                }
                else if (strcmp(MyVariable, "Water density") == 0)
                {
                    if (PReadWrite->FindString("Water density values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, WaterDensity[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, WaterDensity[j]);
                    }
                }
                else if (strcmp(MyVariable, "U Velocity") == 0)
                {
                    if (PReadWrite->FindString("U Velocity values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, UVelocity[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, UVelocity[j]);
                    }
                }
                else if (strcmp(MyVariable, "V Velocity") == 0)
                {
                    if (PReadWrite->FindString("V Velocity values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, VVelocity[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, VVelocity[j]);
                    }
                }
                else if (strcmp(MyVariable, "Richardson number") == 0)
                {
                    if (PReadWrite->FindString("Richardson number values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, RichardsonNumber[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, RichardsonNumber[j]);
                    }
                }
                else if (strcmp(MyVariable, "Exchanged fraction") == 0)
                {
                    if (PReadWrite->FindString("Exchanged fraction values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, ExchangedFraction[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(X+5+j, i, ExchangedFraction[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Mixed layer transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLayerTransport::BuildMixedLayerTransport - variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Transport parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TMixedLayerTransport::BuildMixedLayerTransport - parameters file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Latitude") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyLatitude);
                }
                else if (strcmp(MyParameter, "Manning coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CoefManning);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Mixed layer transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLayerTransport::BuildMixedLayerTransport - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TMixedLayerTransport::~TMixedLayerTransport()
{
    freeMemory();
}

void TMixedLayerTransport::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] UVelocity;
            delete [] VVelocity;
            delete [] WaterDensity;
            delete [] RichardsonNumber;
            delete [] ExchangedFraction;
        }
        if (NumberOfVariables > 0)
            delete [] VariableNameArray;
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
        cerr << "TMixedLayerTransport::freeMemory() - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

double TMixedLayerTransport::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Latitude") == 0)
    {
        value = MyLatitude;
    }
    else if (strcmp(MyParameter, "Manning coefficient") == 0)
    {
        value = CoefManning;
    }
    else
        value = 0.0;
    return value;
}

bool TMixedLayerTransport::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Latitude") == 0)
    {
        MyLatitude = value;
    }
    else if (strcmp(MyParameter, "Manning coefficient") == 0)
    {
        CoefManning = value;
    }
    else
        rc = false;

    return rc;
}

void TMixedLayerTransport::Inquiry(char* srcName, double &Value,
													int BoxNumber,
													char* ParameterName,
													int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Salinity") == 0)
		Value = Salt[MyBoxNumber];
	else if (strcmp(MyParameter, "Mixed layer depth") == 0)
		Value = MixedLayerDepth/*[MyBoxNumber]*/;
	else if (strcmp(MyParameter, "Water density") == 0)
		Value = WaterDensity[MyBoxNumber];
	else if (strcmp(MyParameter, "U Velocity") == 0)
		Value = UVelocity[MyBoxNumber];
	else if (strcmp(MyParameter, "V Velocity") == 0)
		Value = VVelocity[MyBoxNumber];
	else if (strcmp(MyParameter, "Richardson number") == 0)
		Value = RichardsonNumber[MyBoxNumber];
	else if (strcmp(MyParameter, "Exchanged fraction") == 0)
		Value = ExchangedFraction[MyBoxNumber];
	else if (strcmp(MyParameter, "Sigma") == 0)
		Value = WaterDensity[MyBoxNumber]-CUBIC;
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
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 4",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TMixedLayerTransport::Inquiry 4 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.05.28
bool TMixedLayerTransport::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Salinity") == 0)
		Salt[BoxNumber] = Value;
	else if (strcmp(VariableName, "Mixed layer depth") == 0)
		MixedLayerDepth = Value;
	else if (strcmp(VariableName, "Water density") == 0)
		WaterDensity[BoxNumber] = Value;
	else if (strcmp(VariableName, "U Velocity") == 0)
		UVelocity[BoxNumber] = Value;
	else if (strcmp(VariableName, "V Velocity") == 0)
		VVelocity[BoxNumber] = Value;
	else if (strcmp(VariableName, "Richardson number") == 0)
		RichardsonNumber[BoxNumber] = Value;
	else if (strcmp(VariableName, "Exchanged fraction") == 0)
		ExchangedFraction[BoxNumber = Value];
	else if (strcmp(VariableName, "Sigma") == 0)
		WaterDensity[BoxNumber] = Value + CUBIC;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.09
bool TMixedLayerTransport::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Transport");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Transport");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    PReadWrite->WriteSeparator();

    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteCell(VariableNameArray[i]);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Salinity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Mixed layer depth values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("U Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("V Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Richardson number values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Exchanged fraction values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Salt[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(MixedLayer[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WaterDensity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(UVelocity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VVelocity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(RichardsonNumber[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ExchangedFraction[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TMixedLayerTransport::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Transport");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Transport");
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

        if (strcmp(ParametersNameArray[i], "Latitude") == 0)
            PReadWrite->WriteCell(MyLatitude, 6);

        else if (strcmp(ParametersNameArray[i], "Manning coefficient") == 0)
            PReadWrite->WriteCell(CoefManning, 6);

        PReadWrite->WriteSeparator(true);
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}

void TMixedLayerTransport::Go()
{
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
	JulianDay = MyPEcoDynClass->GetJulianDay();
   if (JulianDay == 365)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
		{
      	UVelocity[i] = 0;
         VVelocity[i] = 0;
      }
   }
	Convection = false;
	MixedLayerDepth = 0;
	DeterministicPhase();
	ConvectiveMixing();
	DynamicMixing();

   Generic = Salt;
	GenericLoad = SaltLoad;
	GenericRiver = RiverSalt;
	GenericOcean = OceanSalt;
	AdvectDiffuse(Salt);
//	Salt = Generic;
   if (MyWaterTemperaturePointer != NULL)
   {
   	MyWaterTemperaturePointer->AdvectDiffuseWaterTemperature();
	}
}

// Protected member functions

void TMixedLayerTransport::DeterministicPhase()
{
	const double CoefficientOfThermalExpansion = 0.00012; // 1.2e-4
																			// ?C-1,Janowitz & Kamykowski, 1991
	const double WaterSpecificHeat = 4100; // J/kg/k

	double MyWindSpeed,
			 MyWindSpeedX,
			 MyWindSpeedY,
			 MyNetHeating,
			 MySurfaceHeatLoss,
			 MyLightAtTop,
			 MyLightAtBottom;

	// Surface velocity calculation
    TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	if (MyWindPointer != NULL)
	{
		MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
											0,
											"Wind speed",
											ObjectCode);
		MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeedX,
											0,
											"Wind speed (x component)",
											ObjectCode);
		MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeedY,
											0,
											"Wind speed (y component)",
											ObjectCode);
	}
	else
	{
#ifdef __BORLANDC__
		MessageBox(0, "Wind object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TMixedLayerTransport::DeterministicPhase - Wind object is undefined" << endl;
#endif  // __BORLANDC__
		return;
	}

	// Temporary because Latitude is being read as zero
	//MyLatitude = 44.78333;


	UVelocity[0] = UVelocity[0]
						+ Coriolis ( MyLatitude )
						* VVelocity[0]
						* TimeStep                           // Timestep accounted for  in seconds
						+ WindStress ( MyWindSpeed, MyWindSpeedX )
						* TimeStep
						/ ( WaterDensity[0]
							* MyPEcoDynClass->GetBoxDepth(0) );

	VVelocity[0] = VVelocity[0]
						- Coriolis ( MyLatitude )
						* UVelocity[0]
						* TimeStep                           // Timestep accounted for in seconds
						+ WindStress ( MyWindSpeed, MyWindSpeedY )
						* TimeStep
						/ ( WaterDensity[0]
							 * MyPEcoDynClass->GetBoxDepth(0) );

   VVelocity[NumberOfBoxes - 1] = VVelocity[NumberOfBoxes - 1]
						- CoefRugosidade(NumberOfBoxes - 1)
						* VVelocity[NumberOfBoxes - 1];

	// Density calculation

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
		if (MyLightPointer != NULL)
		{
      	if (i == 0)
         	MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
											       i,
                                        "Total surface irradiance",
											       ObjectCode);

        else
		   	MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtTop,
											       i - 1,
                                        "Sub-surface irradiance",
											       ObjectCode);

        MyLightPointer->Inquiry(GetEcoDynClassName(), MyLightAtBottom,
												 i,
												 "Sub-surface irradiance",
												 ObjectCode);
		}
		MyNetHeating = MyLightAtTop - MyLightAtBottom;
		if (MyWaterTemperaturePointer != NULL)
    		MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MySurfaceHeatLoss,
															0,
															"Net heat flux",
															ObjectCode);
		else
		{
#ifdef __BORLANDC__
			MessageBox(0,
						  "Water temperature object is undefined",
						  "EcoDynamo alert",
						  MB_OK);
#else  // __BORLANDC__
            cerr << "TMixedLayerTransport::DeterministicPhase - Water temperature object is undefined" << endl;
#endif  // __BORLANDC__
			return;
		}

		if (i == 0) // A densidade pode ser aferida também em função da salinidade
			WaterDensity[i] = WaterDensity[i]
									- CoefficientOfThermalExpansion
									* MySurfaceHeatLoss
									* TimeStep                              // Timestep accounted for in seconds
									/( WaterSpecificHeat
										* MyPEcoDynClass->GetBoxDepth(i));
		else
			WaterDensity[i] = WaterDensity[i]
									- CoefficientOfThermalExpansion
									* MyNetHeating
									* TimeStep                              // Timestep accounted for in seconds
									/( WaterSpecificHeat
										* MyPEcoDynClass->GetBoxDepth(i));

	}
}

void TMixedLayerTransport::ConvectiveMixing()
{
	double CumulativeDensity,
			 CumulativeUVelocity,
			 CumulativeVVelocity;
	ConvectiveBox = NumberOfBoxes;

	int i = 0;

	do
	{
		if  ( WaterDensity[i] > WaterDensity[i+1] ) // Situação de instabilidade
		{
			MixedLayerDepth = 0;
			CumulativeDensity = 0;
			CumulativeUVelocity = 0;
			CumulativeVVelocity = 0;
			ConvectiveBox = i+1;
			Convection = true;
			for (int j = 0; j <= ConvectiveBox; j++)
			{
				CumulativeDensity = CumulativeDensity
										  + WaterDensity[j];
				CumulativeUVelocity = CumulativeUVelocity
											 + UVelocity[j];
				CumulativeVVelocity = CumulativeVVelocity
											 + VVelocity[j];
			}
			for (int j = 0; j <= ConvectiveBox; j++)
			{
				WaterDensity[j] = CumulativeDensity
										/ (ConvectiveBox+1); // +1 because array is zero based
				UVelocity[j] = CumulativeUVelocity
									/ (ConvectiveBox+1);   // +1 because array is zero based
				VVelocity[j] = CumulativeVVelocity
									/ (ConvectiveBox+1);   // +1 because array is zero based
				MixedLayerDepth = MixedLayerDepth + MyPEcoDynClass->GetBoxDepth(j);
			}
		}
		i++;
	}
	while ( i < (NumberOfBoxes - 1));
}


void TMixedLayerTransport::DynamicMixing()
{
	const double  // ms-2 - Aceleração da gravidade
					 Rs = 0.25;         // Número de Richardson abaixo do qual o
											  // "shear stress" é suficiente para provocar
											  // trocas verticais
	double MyTotalDepth, MyMixedLayerDepth;


	MyMixedLayerDepth = 0.0;

	for (int i = 0; i < NumberOfBoxes - 1; i++) //  to n_boxes-1 i.e. <= NumberOfBoxes-2
	{
		if (i == 0)
			MyTotalDepth = MyPEcoDynClass->GetBoxDepth(i);
		MyTotalDepth = MyTotalDepth + MyPEcoDynClass->GetBoxDepth(i + 1);

		if (
			  sqr( UVelocity[i+1] - UVelocity[i] )
			  + sqr( VVelocity[i+1] - VVelocity[i] )
			  == 0
			)
			RichardsonNumber[i] = Rs;
		else
		{
			RichardsonNumber[i] = ( WaterDensity[i+1] - WaterDensity[i] )
										 * gravity
										 * MyPEcoDynClass->GetBoxDepth(i)
										 / ( WaterDensity[i] *
											 ( sqr ( UVelocity[i+1] - UVelocity[i] )
											  + sqr ( VVelocity[i+1] - VVelocity[i] ))
											) ;
		}

		if (RichardsonNumber[i] > 0.25)
			RichardsonNumber[i] = Rs;

		if (RichardsonNumber[i] < 0.25)
			MyMixedLayerDepth = MyTotalDepth;

	  ExchangedFraction[i] = 0.5
									 * ( 1 - RichardsonNumber[i] / Rs);

	  WaterDensity[i] = WaterDensity[i]
							  + ExchangedFraction[i]
							  *( WaterDensity[i+1] - WaterDensity[i] );

	  WaterDensity[i+1] = WaterDensity[i+1]
								 - ExchangedFraction[i]
								 * ( WaterDensity[i+1] - WaterDensity[i] );

	  UVelocity[i] = UVelocity[i]
						  + ExchangedFraction[i]
						  * ( UVelocity[i+1] - UVelocity[i] );

	  UVelocity[i+1] = UVelocity[i+1]
							 - ExchangedFraction[i]
							 * ( UVelocity[i+1] - UVelocity[i] );

	  VVelocity[i] = VVelocity[i]
						  + ExchangedFraction[i]
						  * ( VVelocity[i+1] - VVelocity[i] );

	  VVelocity[i+1] = VVelocity[i+1]
							 - ExchangedFraction[i]
							 * ( VVelocity[i+1] - VVelocity[i] );

	}
	if (MyMixedLayerDepth > MixedLayerDepth)
		MixedLayerDepth = MyMixedLayerDepth;
	if (MixedLayerDepth < MyPEcoDynClass->GetBoxDepth(0))
		MixedLayerDepth = MyPEcoDynClass->GetBoxDepth(0);
}

void TMixedLayerTransport::AdvectDiffuse(double* Generic)
{
	double CumulativeObjValue,
			 MyGeneric;

	for (int i = 0; i <= ConvectiveBox; i++)
		GenericExchange[i] = 0;

	if (Convection)
	{
		CumulativeObjValue = 0;

		for (int i = 0; i <= ConvectiveBox; i++)
		{
			CumulativeObjValue = CumulativeObjValue
										+ Generic[i];
			GenericExchange[i] = Generic[i];           // save initial concentrations
		}

		for (int i = 0; i <= ConvectiveBox; i++)
		{
			Generic[i] = CumulativeObjValue
							 / (ConvectiveBox+1);
			GenericExchange[i] = Generic[i]            // positive influx
										- GenericExchange[i]; // new conc. - old conc.
		}

	}



	// Acima - Transporte convectivo de qualquer variável, através da homogeneização da sua concentração
	// em todas as caixas em que houve convecção. Da forma como o programa está escrito este
	// fenómeno só pode ocorrer entre a superfície e outra qualquer profundidade. Poderá vir
	// a interessar que possa também ser simulado a profundidades intermédias, embora as
	// situações de instabilidade geralmente ocorram a partir da superfície.

	for (int i = 0; i < NumberOfBoxes-1; i++)
	{
		MyGeneric = Generic[i]
						+ ExchangedFraction[i]
						* ( Generic[i+1] - Generic[i] );

		Generic[i+1] = Generic[i+1]
							- ExchangedFraction[i]
							* ( Generic[i+1] - Generic[i] );

		GenericExchange[i] = GenericExchange[i]
									+ ExchangedFraction[i]
									* ( Generic[i+1] - Generic[i] );

		GenericExchange[i+1] = GenericExchange[i+1]
							- ExchangedFraction[i]
							* ( Generic[i+1] - Generic[i] );

		Generic[i] = MyGeneric;
	}
}

double TMixedLayerTransport::Coriolis(float ALatitude)
{
	const double EarthAngularSpeed = 0.0000727;  //7.27e-5; // s-1, ou seja, 2Pi em 24 horas}
	return 2
			 * EarthAngularSpeed
			 * sin (M_PI * ALatitude / 180);
}

double TMixedLayerTransport::WindStress(//double &AStress, // what does the stress do here??
													 double AWindSpeed,
													 double AWindComponent)
{
	const double AirDensity = 1.255, // Kg/m3
					 CoefficientOfMomentumTransfer = 0.0017; // 1.7e-3  Janowitz & Kamykowski, 1991
	return AirDensity
			 * CoefficientOfMomentumTransfer
			 * AWindSpeed
			 * AWindComponent;
}


double TMixedLayerTransport::CoefRugosidade(int ABoxNumber)
{
    double MyDepth = MyPEcoDynClass->GetBoxDepth(0);
    if (MyDepth < 0.5)
        MyDepth = 0.5;
    return(gravity * pow(CoefManning,2.0)) / pow(MyDepth,0.333333);
}


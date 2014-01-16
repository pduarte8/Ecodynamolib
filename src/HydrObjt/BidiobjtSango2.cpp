// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 95.4.23
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

//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensionalSango2::TBiDimensionalSango2(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aUFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary, int northernTidalReference,
                   int southernTidalReference, double phaseLag)
        :TBiDimensionalNormalTimeStep(className, timeStep, nLines, nColumns,
                      aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
	ParameterCount = 0;

	// Read in the parameters
    NumberOfParameters = nParams;
    MyLatitude = latitude;
    Gravity = gravity;
    HorizontalDiffCoef = horizontalDiffCoef;
    StartYear = startYear;
    CriticalDepthForLandBoundary = criticalDepthForLandBoundary;
    NorthernTidalReference = northernTidalReference;
    SouthernTidalReference = southernTidalReference;
    PhaseLag = phaseLag;
	for (int j = 0; j < NumberOfBoxes; j++)
	{
		CoefManning[j] = aCoefManning[j];
    }
    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();

    delete[] UVelocity;
    if (NumberOfBoxes > 0)
	{
        Salt = new double[NumberOfBoxes];
		UVelocity = new double[NumberOfBoxes + GridLines];
        VVelocity = new double[NumberOfBoxes + GridColumns];
        UVelocityPast = new double[NumberOfBoxes + GridLines];
        VVelocityPast = new double[NumberOfBoxes + GridColumns];
		UFlowBetweenBoxes = new double[NumberOfBoxes + GridLines];
        VFlowBetweenBoxes = new double[NumberOfBoxes + GridColumns];
    }

    for (int i = 0; i < NumberOfBoxes + GridLines; i++)
    {
   	    UVelocity[i] = 0;
        UVelocityPast[i] = 0;
        UFlowBetweenBoxes[i] = 0;
    }

    for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
    {
   	    VVelocity[i] = 0;
        VVelocityPast[i] = 0;
        VFlowBetweenBoxes[i] = 0;
    }

	// Read in the variables
    NumberOfVariables = nVars;

    // Create array for state variable names
	VariableNameArray = new VNA[NumberOfVariables];

	// read in the variable names
	for (int i = 0; i < NumberOfVariables; i++) {
		strcpy(VariableNameArray[i], aVarsNames[i]);
    }

	// Read in the variables values
	for (int j = 0; j < NumberOfBoxes; j++)
	{
        Salt[j] = aSalt[j];
        DynamicHeight[j] = aDynamicHeight[j];
		WaterDensity[j] = aWaterDensity[j];
    }
	for (int j = 0; j < NumberOfBoxes + GridLines; j++)
	{
        UVelocity[j] = aUVelocity[j];
        UVelocityPast[j] = UVelocity[j];
	}
	for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
	{
        VVelocity[j] = aVVelocity[j];
        VVelocityPast[j] = VVelocity[j];
    }
	UFlowBetweenBoxes[0] = aUFlow[0];
	VFlowBetweenBoxes[0] = aVFlow[0];
}

TBiDimensionalSango2::TBiDimensionalSango2(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
		 :TBiDimensionalNormalTimeStep(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildBiDimensionalSango2();
}

TBiDimensionalSango2::TBiDimensionalSango2(TEcoDynClass* PEcoDynClass, char* className)
                        :TBiDimensionalNormalTimeStep(PEcoDynClass, className)

{
    BuildBiDimensionalSango2();
}

void TBiDimensionalSango2::BuildBiDimensionalSango2()
{
    //Parameters file
    double MyValue;
    int X,Y, XV, YV;

    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();
	ParameterCount = 0;

	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
    //
    // List of parameters initialized in TOneDimensionalDITransport
    //
                if (strcmp(MyParameter, "Gravity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Gravity);
                }
                else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
                {
                    if (!PReadWrite->FindString("Coeficiente de Manning values", XV, YV))
                    {
                        XV = X + 3;
                        YV = i - 1;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, CoefManning[j]);
                    }
                }
                else if (strcmp(MyParameter, "Difusion coeficient") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, HorizontalDiffCoef);
                }
                else
                if (strcmp(MyParameter, "Latitude") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, MyLatitude);
                }
    /*            else if (strcmp(MyParameter, "Critical depth for land boundary") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, CriticalDepthForLandBoundary);
                } */
                else if (strcmp(MyParameter, "Start Year") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, StartYear);
                }
                else if (strcmp(MyParameter, "Northern tidal reference") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, NorthernTidalReference);
                    if ((NorthernTidalReference > NumberOfBoxes - 1) || (NorthernTidalReference < 0))
#ifdef __BORLANDC__
                        MessageBox(0, "Northern Tidal Reference out of range", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
                        cerr << "TBiDimensionalSango2::BuildBiDimensionalSango2 - Northern Tidal Reference out of range" << endl;
#endif  // __BORLANDC__
                }
                else if (strcmp(MyParameter, "Southern tidal reference") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, SouthernTidalReference);
                    if ((SouthernTidalReference > NumberOfBoxes - 1) || (SouthernTidalReference < 0))
#ifdef __BORLANDC__
                        MessageBox(0, "Southern Tidal Reference out of range", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
                        cerr << "TBiDimensionalSango2::BuildBiDimensionalSango2 - Southern Tidal Reference out of range" << endl;
#endif  // __BORLANDC__
                }
                else if (strcmp(MyParameter, "Tidal Phase Lag") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, PhaseLag);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - BiDimensionalSango2", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalSango2::BuildBiDimensionalSango2 - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
       //End reading parameters file
        CloseDataFile((void*)PReadWrite);
    }
    
    delete[] UVelocity;
    if (NumberOfBoxes > 0)
	{
		UVelocity = new double[NumberOfBoxes + GridLines];
        VVelocity = new double[NumberOfBoxes + GridColumns];
        UVelocityPast = new double[NumberOfBoxes + GridLines];
        VVelocityPast = new double[NumberOfBoxes + GridColumns];
		UFlowBetweenBoxes = new double[NumberOfBoxes + GridLines];
        VFlowBetweenBoxes = new double[NumberOfBoxes + GridColumns];
    }

    for (int i = 0; i < NumberOfBoxes + GridLines; i++)
    {
   	    UVelocity[i] = 0;
        UVelocityPast[i] = 0;
        UFlowBetweenBoxes[i] = 0;
    }

    for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
    {
   	    VVelocity[i] = 0;
        VVelocityPast[i] = 0;
        VFlowBetweenBoxes[i] = 0;
    }

	//Variables file
	char MyVariable[65];

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
	if (PReadWrite != NULL)
	{

        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);

            // Array for state variable names created in TOneDimensionalDITransport
            // but not filled because the direction is wrong
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);
                if (strcmp(MyVariable, "U Velocity") == 0)
                {
                    if (!PReadWrite->FindString("U Velocity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        PReadWrite->ReadNumber(XV , YV + 1 + j, UVelocity[j]);
                        UVelocityPast[j] = UVelocity[j];
                    }
                }
             if (strcmp(MyVariable, "V Velocity") == 0)
                {
                    if (!PReadWrite->FindString("V Velocity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, VVelocity[j]);
                        VVelocityPast[j] = VVelocity[j];
                    }
                }
                else if (strcmp(MyVariable, "Dynamic height") == 0)
                {
                    if (!PReadWrite->FindString("Dynamic height values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DynamicHeight[j]);
                        PastHeight[j] = DynamicHeight[j];
                    }
                }
                else if (strcmp(MyVariable, "U Flow") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes + GridLines; j++)
                    {
                        UFlowBetweenBoxes[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "V Flow") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
                    {
                        VFlowBetweenBoxes[j] = MyValue;
                    }
                }
                else if (strcmp(MyVariable, "Salinity") == 0)
                {
                    if (!PReadWrite->FindString("Salinity values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Salt[j]);
                    }
                }
                else if (strcmp(MyVariable, "Box depth") == 0)
                {
                    if (!PReadWrite->FindString("Box depth values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoxDepth[j]);
                    }
                }
                else if (strcmp(MyVariable, "Water density") == 0)
                {
                    if (!PReadWrite->FindString("Water density values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, WaterDensity[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - BiDimensionalSango2", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalSango2::BuildBiDimensionalSango2 - variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }

    for (int i = 0; i < GridLines; i++)
    {
   	    BoxArray[i * GridColumns].BoxToBoundary[4].TypeOfBoundary = 4;
        BoxArray[(i + 1) * GridColumns - 1].BoxToBoundary[2].TypeOfBoundary = 4;
    }
   	for (int j = 0; j < GridColumns; j++)
    {
   	    BoxArray[j].BoxToBoundary[3].TypeOfBoundary = 4;
        BoxArray[(GridLines - 1) * GridColumns + j].BoxToBoundary[1].TypeOfBoundary = 4;
    }

    for (int i = 0; i < GridLines; i++)
        BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary = 2;
}

TBiDimensionalSango2::~TBiDimensionalSango2()
{
    freeMemory();
}

void TBiDimensionalSango2::freeMemory()
{
    try {
        delete[] UVelocity; delete[] UVelocityPast; delete[] UFlowBetweenBoxes;
        delete[] VVelocity; delete[] VVelocityPast; delete[] VFlowBetweenBoxes;
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
        cerr << "TBiDimensionalSango2::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__

}


double TBiDimensionalSango2::GetParameterValue(char* MyParameter)
{
  double value;


    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        value = Gravity;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        value = CoefManning[0];
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        value = HorizontalDiffCoef;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        value = BoxWidth[0];
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        value = StartYear;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        value = kdiffusion;
    }

    // parameters from TBiDimensionalSango2
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        value = MyLatitude;
    }
    else if (strcmp(MyParameter, "Northern tidal reference") == 0)
    {
        value = NorthernTidalReference;
    }
    else if (strcmp(MyParameter, "Southern tidal reference") == 0)
    {
        value = SouthernTidalReference;
    }
    else if (strcmp(MyParameter, "Tidal Phase Lag") == 0)
    {
        value = PhaseLag;
    }
    else
        value = 0.0;
    return value;
}

bool TBiDimensionalSango2::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;


    // parameters from TOneDimensionalDITransport
    if (strcmp(MyParameter, "Gravity") == 0)
    {
        Gravity = value;
    }
    else if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
        {
            CoefManning[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Difusion coeficient") == 0)
    {
        HorizontalDiffCoef = value;
    }
    else if (strcmp(MyParameter, "Box Width") == 0)
    {
        for (j = 0; j < NumberOfBoxes + 1; j++)
        {
            BoxWidth[j] = value;
        }
    }
    else if (strcmp(MyParameter, "Start Year") == 0)
    {
        StartYear = value;
    }
    else if (strcmp(MyParameter, "kdiffusion") == 0)
    {
        kdiffusion = value;
    }

    // parameters from TBiDimensionalSango2
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        MyLatitude = value;
    }
    else if (strcmp(MyParameter, "Northern tidal reference") == 0)
    {
        NorthernTidalReference = value;
    }
    else if (strcmp(MyParameter, "Southern tidal reference") == 0)
    {
        SouthernTidalReference = value;
    }
    else if (strcmp(MyParameter, "Tidal Phase Lag") == 0)
    {
        PhaseLag = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.09
bool TBiDimensionalSango2::SaveVariables()
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
    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);
    /*
     * save variables' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "U Flow") == 0)
            PReadWrite->WriteCell(UFlowBetweenBoxes[0], 6);

        else if (strcmp(VariableNameArray[i], "V Flow") == 0)
            PReadWrite->WriteCell(VFlowBetweenBoxes[0], 6);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("U Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("V Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Salinity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Dynamic height values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Box depth values");
    PReadWrite->WriteSeparator(true);

    int m = MAX(GridLines, GridColumns);
    for (int j = 0; j < (NumberOfBoxes + m); j++)
    {
        if (j < NumberOfBoxes) {
            PReadWrite->WriteCell(UVelocity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(VVelocity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Salt[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(WaterDensity[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(DynamicHeight[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(BoxDepth[j], 8);
        }
        else {
            if (GridLines > GridColumns) {
                if (j >= (NumberOfBoxes + GridColumns)) {
                    PReadWrite->WriteCell(UVelocity[j], 8);
                }
                else {
                    PReadWrite->WriteCell(UVelocity[j], 8);
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(VVelocity[j], 8);
                }
            }
            else {
                if (j >= (NumberOfBoxes + GridLines)) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(VVelocity[j], 8);
                }
                else {
                    PReadWrite->WriteCell(UVelocity[j], 8);
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(VVelocity[j], 8);
                }
            }
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TBiDimensionalSango2::SaveParameters()
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

        if (strcmp(ParametersNameArray[i], "Gravity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Gravity, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Difusion coeficient") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(HorizontalDiffCoef, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Start Year") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(StartYear, 0);
        }
        else if (strcmp(ParametersNameArray[i], "kdiffusion") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(kdiffusion, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Latitude") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(MyLatitude, 6);
        }
        else if (strcmp(ParametersNameArray[i], "Northern tidal reference") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(NorthernTidalReference, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Southern tidal reference") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(SouthernTidalReference, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Tidal Phase Lag") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(PhaseLag, 0);
        }
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteCell("Coeficiente de Manning values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Box Width values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(CoefManning[j], 6);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WaterDensity[j], 6);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoxWidth[j], 6);
        PReadWrite->WriteSeparator(true);
    }
    /*
     * one more value for "BoxWidth"
     */
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(BoxWidth[NumberOfBoxes], 6);
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

//Harmonic forced tide with a time lag. It was designed for a semitimestep driven by the shell not by hydrobjt

void TBiDimensionalSango2::TideForcing()
{
	int MyYear, MyMonth, MyDay;
	double MySecondSurfaceHeight;
	TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();
    if (MyTidePointer != NULL)
    {
        MyYear = MyPEcoDynClass->GetYear()/*- 1 + StartYear*/;
        MyMonth = MyPEcoDynClass->GetMonth();
        MyDay = MyPEcoDynClass->GetDate();
        MyTime = MyPEcoDynClass->GetCurrTime();
        MyTidePointer->Inquiry(GetEcoDynClassName(), SurfaceHeight, MyYear, MyMonth, MyDay, MyTime);
        SurfaceHeight = SurfaceHeight / CUBIC;  //convert from mm to
        DynamicHeight[NorthernTidalReference] = SurfaceHeight;
        MyTime = MyTime + PhaseLag;
        if (floor(MyTime) >= 24)
        {
      	    MyTime = MyTime - 24.0;
            MyDay++;
        }
        if (MyDay > DaysInMonth[MyMonth - 1])
        {
      	    if (MyMonth == 12)
            {
         	    MyMonth = 1;
                MyYear++;
            }
            else
                MyMonth++;
            MyDay = 1;
        }
        MyTidePointer->Inquiry(GetEcoDynClassName(), MySecondSurfaceHeight, MyYear, MyMonth, MyDay, MyTime);
        MySecondSurfaceHeight = MySecondSurfaceHeight / CUBIC;  //convert from mm to m
        DynamicHeight[SouthernTidalReference] = MySecondSurfaceHeight;

   	    for (int i = 0; i < GridLines; i++)
   	    {
   		    if ((BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[1].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[3].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[4].TypeOfBoundary == 2))
            {
                if ((i * GridColumns + GridColumns - 1 != SouthernTidalReference)
               		&& (i * GridColumns + GridColumns - 1 != NorthernTidalReference))
                {
               	    if (i * GridColumns + GridColumns - 1 == GridColumns - 1)
               		    DynamicHeight[i * GridColumns + GridColumns - 1] = DynamicHeight[SouthernTidalReference] -
                                                                        (DynamicHeight[NorthernTidalReference] - DynamicHeight[SouthernTidalReference]) /
                     	                                                ((NorthernTidalReference + 1) / GridColumns - (SouthernTidalReference + 1) / GridColumns) *
                                                                        ((SouthernTidalReference + 1) / GridColumns - 1);
                    else
                  	    DynamicHeight[i * GridColumns + GridColumns - 1] = DynamicHeight[GridColumns - 1] +
                                                                        (DynamicHeight[NorthernTidalReference] - DynamicHeight[SouthernTidalReference]) /
                     	                                                ((NorthernTidalReference + 1) / GridColumns - (SouthernTidalReference + 1) / GridColumns) * i;
                }
            }
        }
    }
}



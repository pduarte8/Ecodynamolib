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
	#include <exception>
    
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"
#include "globals.h"


//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensionalSango1::TBiDimensionalSango1(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aUFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary, int northernTidalReference,
                   int southernTidalReference)
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
	for (int j = 0; j < NumberOfBoxes; j++)
	{
		CoefManning[j] = aCoefManning[j];
    }
    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();

    if (NumberOfBoxes > 0)
	{
//        Salt = new double[NumberOfBoxes];
        if (UVelocity != NULL)
            delete [] UVelocity;
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

TBiDimensionalSango1::TBiDimensionalSango1(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
			 :TBiDimensionalNormalTimeStep(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildBiDimensionalSango1();
}

TBiDimensionalSango1::TBiDimensionalSango1(TEcoDynClass* PEcoDynClass,
        char* className) :TBiDimensionalNormalTimeStep(PEcoDynClass, className)

{
    BuildBiDimensionalSango1();
}

void TBiDimensionalSango1::BuildBiDimensionalSango1()
{
   double MyValue;
   int X,Y, XV, YV;

    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
	ParameterCount = 0;
    CriticalDepthForLandBoundary = MyPEcoDynClass->GetCriticalDepth();

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);

            // read in the parameter names
            char MyParameter[65];
    //		ParametersNameArray = new VNA[NumberOfParameters];

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);
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
                }*/
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
                        cerr << "TBiDimensionalRiaFormosa::BuildBiDimensionalSango1 - alert: Northern Tidal Reference out of range" << endl;
#endif  // __BORLANDC__
                }
                else if (strcmp(MyParameter, "Southern tidal reference") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, SouthernTidalReference);
                    if ((SouthernTidalReference > NumberOfBoxes - 1) || (SouthernTidalReference < 0))
#ifdef __BORLANDC__
                       MessageBox(0, "Southern Tidal Reference out of range", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
                        cerr << "TBiDimensionalRiaFormosa::BuildBiDimensionalSango1 - alert: Southern Tidal Reference out of range" << endl;
#endif  // __BORLANDC__
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - BiDimensionalSango1", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalRiaFormosa::BuildBiDimensionalSango1 - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
       //End reading parameters file
        CloseDataFile((void*)PReadWrite);
    }
    
    delete[] UVelocity;

    if (NumberOfBoxes > 0)
	{
//        Salt = new double[NumberOfBoxes];
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
                    UFlowBetweenBoxes[0] = MyValue;
                }
                else if (strcmp(MyVariable, "V Flow") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                    VFlowBetweenBoxes[0] = MyValue;
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
                else
                if (strcmp(MyVariable, "Water density") == 0)
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
            MessageBox(0, "Variables: Undefined object - BiDimensionalSango1", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalRiaFormosa::BuildBiDimensionalSango1 - variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }
}



TBiDimensionalSango1::~TBiDimensionalSango1()
{
    freeMemory();
}

void TBiDimensionalSango1::freeMemory()
{
    try {
    //	delete[] UVelocity;   // made in TOneDimensionalDITransport
        delete[] VVelocity;
        delete[] UVelocityPast;
        delete[] VVelocityPast;
        delete[] UFlowBetweenBoxes;
        delete[] VFlowBetweenBoxes;
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
        cerr << "TBiDimensionalSango1::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

double TBiDimensionalSango1::GetParameterValue(char* MyParameter)
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

    // parameters from TBiDimensionalSango1
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
    else
        value = 0.0;
    return value;
}

bool TBiDimensionalSango1::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TBiDimensionalSango1
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
    else
        rc = false;

    return rc;
}

// AP, 2007.07.09
bool TBiDimensionalSango1::SaveVariables()
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


bool TBiDimensionalSango1::SaveParameters()
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


//Forced with harmonics in two points, interpolations between them and extrapolations above and below as in the
//previous void, except that this one was designed for a semitimestep driven by the shell not by hydrobjt

void TBiDimensionalSango1::Go()
{
   int k;

    TideForcing();
    EquationOfMotion();

    if (SemiTimeStep == 1)
    {
   	    k = 0;
       	for (int i = 0; i < GridLines; i++)
   	    {
   		    for (int j = 0; j < GridColumns; j++)
          	{
                int index = i * GridColumns + j;
   		    	if ((j > 0) &&
            		(
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                      )  &&
            		(BoxDepth[index] > CriticalDepthForLandBoundary) &&
            		(BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
            		(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&  //No southern ocean boundary
            	    )
       			{
       				a[k] = atemporary[index];
          			b[k] = btemporary[index];
      	    		c[k] = ctemporary[index];
      		    	r[k] = rtemporary[index];

            	    k++;
          		}
	    	}
       	}

       	IntegrationTridag(k);

       	k = 0;
   	    for (int i = 0; i < GridLines; i++)
       	{
   	    	for (int j = 0; j < GridColumns; j++)
      	    {
                int index = i * GridColumns + j;
       			if ((j > 0) &&
            		(
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                      )  &&
            		(BoxDepth[index] > CriticalDepthForLandBoundary) &&
            		(BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
                   	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                   	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
               	    (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
            		(BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&  //No southern ocean boundary
              		)
   	    		{
             		UVelocity[i * (GridColumns + 1) + j] = u[k];
                	if (fabs(UVelocity[i * (GridColumns + 1) + j]) < 0.00000000001)
                	    UVelocity[i * (GridColumns + 1) + j] = 0.0;
                	k++;
      		    }
            }
   	    }
    }
    else if (SemiTimeStep == 2)
    {
        k = 0;
        for (int j = 0; j < GridColumns; j++)
   	    {
      	    for (int i = 0; i < GridLines; i++)
          	{
                int index = i * GridColumns + j;
                int iPrevLine = (i - 1) * GridColumns + j;

   		    	if ((i > 0) &&
                      (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                      ) &&
                      (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                      (BoxDepth[iPrevLine] > CriticalDepthForLandBoundary) &&
                      (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                      (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)&& //No eastern ocean boundary
                      (BoxArray[iPrevLine].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                      (BoxArray[iPrevLine].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
          		    )
   		    	{
   			    	a[k] = atemporary[index];
      			    b[k] = btemporary[index];
          			c[k] = ctemporary[index];
          			r[k] = rtemporary[index];
                	k++;
          		}
	    	}
       	}

   	    IntegrationTridag(k);

        k = 0;
        for (int j = 0; j < GridColumns; j++)
       	{
          	for (int i = 0; i < GridLines; i++)
      	    {
                int index = i * GridColumns + j;
                int iPrevLine = (i - 1) * GridColumns + j;

       			if (
      				(i > 0) &&
                      (
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                     (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                      ) &&
                      (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                      (BoxDepth[iPrevLine] > CriticalDepthForLandBoundary) &&
                      (BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                      (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                      (BoxArray[iPrevLine].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                      (BoxArray[iPrevLine].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2)//&&   //No eastern ocean boundary
              		)
   		    	{
             		VVelocity[index] = u[k];

                    if (fabs(VVelocity[index]) < 0.00000000001)
                        VVelocity[index] = 0.0;
                	k++;
          		}
            }
       	}
    }

   	UpdateOfTidalBoundariesVelocities();
    Generic = Salt;
    Continuity();
    Salt = Generic;
    UpdateMeanSamples();
    
}

void TBiDimensionalSango1::EquationOfMotion()    //Implicit scheme
{


   for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
         int index = i * GridColumns + j;
         int iNextLine = (i + 1) * GridColumns + j;
         int iPrevLine = (i - 1) * GridColumns + j;

         if (BoxDepth[index] > CriticalDepthForLandBoundary)
         {
         	//Calculating intermediate depths and velocities

         	AUPStreamU = UPStreamUVelocity(i,j);
            //Debugger(AUPStreamU);
            ADOWNStreamU = DOWNStreamUVelocity(i,j);
            //Debugger(ADOWNStreamU);
            AUPStreamV = UPStreamVVelocity(i,j);
            //Debugger(AUPStreamV);
            ADOWNStreamV = DOWNStreamVVelocity(i,j);
            //Debugger(ADOWNStreamV);
            AUpperLeftDepth = UpperLeftCornerDepth(i,j);
            ALowerLeftDepth = LowerLeftCornerDepth(i,j);
            AUpperRightDepth = UpperRightCornerDepth(i,j);
            ALowerRightDepth = LowerRightCornerDepth(i,j);
            //


            if (SemiTimeStep == 1)
            {
               if ((j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                    	 	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&  //No southern ocean boundary
                     //(BoxArray[index].Dummybool1)
                  )
               {
               	//Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                  if ((BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1) ||
                      (BoxArray[index - 1].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 1)
                     )
                  	rtemporary[index] = UVelocityPast[i * (GridColumns + 1) + j ];
                  else
               		rtemporary[index] =
                     	UVelocityPast[i * (GridColumns + 1) + j] +
                        VCoriolis(i,j) +
                        UVerticalDiffusion(i,j) -
                        UVAdvectionOfUEquation(i,j)+
                   		GravityOfRightSideOfUEquationOfMotion(i,j);


                  // Cálculo do a, b e c


               	atemporary[index] = aUCalculation(i,j);
                  btemporary[index] = bUCalculation(i,j);
                  ctemporary[index] = cUCalculation(i,j);


                  if (BoxArray[index - 1].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 2) //Left neighbour cell is a western ocean boundary
                  {
                  	btemporary[index] = btemporary[index] + atemporary[index] *
                                        (BoxDepth[index - 1] +
                                         BoxDepth[index]
                                        ) / (2.0 * BoxDepth[index - 1]);


                     double Parcel1;

                     Parcel1 = 2.0 * ASpatialStep * (DynamicHeight[index - 1]
                               - PastHeight[index - 1]) /
                              (ATimeStep * BoxDepth[index - 1]);

                     if ((i < GridLines - 1) &&
                           (BoxDepth[iNextLine - 1] > 0.0)
                        )
                     	Parcel1 = Parcel1 + (BoxDepth[index - 1] + BoxDepth[iNextLine - 1]) *
                     							VVelocityPast[iNextLine - 1] /
                           	            (2.0 * BoxDepth[index - 1]);
                     else
                     	Parcel1 = Parcel1 + (BoxDepth[index - 1] + BoxDepth[index - 1]) *
                     							VVelocityPast[iNextLine - 1] /
                           	            (2.0 * BoxDepth[index - 1]);
                     if (
                           (i > 0) &&
                           (BoxDepth[iPrevLine - 1] > 0.0)
                        )

                        Parcel1 = Parcel1 - (BoxDepth[index - 1] + BoxDepth[iPrevLine - 1]) *
                                            VVelocityPast[index - 1] /
                                            (2.0 * BoxDepth[index - 1]);
                     else
                        Parcel1 = Parcel1 -  (BoxDepth[index - 1] + BoxDepth[index - 1]) *
                                            VVelocityPast[index - 1] /
                                            (2.0 * BoxDepth[index - 1]);

                     rtemporary[index] = rtemporary[index] - atemporary[index] * Parcel1;



                     atemporary[index] = 0.0;
                  }
                  else
                  if (BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Cell is a eastern ocean boundary
                  {

                  	btemporary[index] = btemporary[index] + ctemporary[index] *
                                        (BoxDepth[index] + BoxDepth[index - 1]) /
                                        (2.0 * BoxDepth[index]);


                     double Parcel2;

                     Parcel2 = - 2.0 * ASpatialStep * (DynamicHeight[index]  - PastHeight[index]) /
                              (ATimeStep * BoxDepth[index]);
                     if (
                        	(i < GridLines - 1) &&
                           (BoxDepth[iNextLine] > 0.0)
                        )
                        Parcel2 = Parcel2 - (BoxDepth[index] + BoxDepth[iNextLine]) *
                                             VVelocityPast[iNextLine] /
                                             (2.0 * BoxDepth[index]);
                     else
                        Parcel2 = Parcel2 - (BoxDepth[index] + BoxDepth[index]) *
                                             VVelocityPast[iNextLine] /
                                             (2.0 * BoxDepth[index]);
                     if (
                           (i > 0) &&
                           (BoxDepth[iPrevLine] > 0.0)
                        )

                        Parcel2 = Parcel2 + (BoxDepth[index] + BoxDepth[iPrevLine]) *
                        							VVelocityPast[index] /
                                             (2.0 * BoxDepth[index]);
                     else
                     	Parcel2 = Parcel2 + (BoxDepth[index] + BoxDepth[index]) *
                        							VVelocityPast[index] /
                                             (2.0 * BoxDepth[index]);


                     rtemporary[index] = rtemporary[index] - ctemporary[index] * Parcel2;


                     ctemporary[index] = 0.0;

                  }
               }
               else
               {
               	atemporary[index] = 0.0; btemporary[index] = 0.0;
                  ctemporary[index] = 0.0; rtemporary[index] = 0.0;
               }



               if (
               		(i > 0) &&
                     (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                     ) &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[iPrevLine] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
                     (BoxArray[iPrevLine].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[iPrevLine].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
                     //(BoxArray[index].Dummybool1)

                     //De notar que este conjunto de condições lógicas não impede que seja calculada a velocidade
                     //explícita para uma célula que seja fronteira fluvial. No entanto, no final do passo de cálculo é imposta
                     //nessa célula a velocidade calculada a partir do caudal fluvial. Poderiam haver entretanto problemas no
                     //cálculo da continuidade e do transporte, mas não se passa nada de errado porque os referidos cálculos se baseiam
                     //nos termos explícitos passados (actualizados no final d cada passode cálculo. Os implicitos são os presentes, mas quanto a esses
                     //não há problemas porque o a=0, o b=1 e o c=0 nas fornteiras fluviais pelo que o cálculo implicito da velocidade não
                     //muda o seu valor nas fronteiras fluviais.
                   )
               {

               	//vs  explicit
                  VVelocity[index] =
              			  	VVelocityPast[index] -
                     	ATimeStep / (2.0 * (BoxDepth[index] +
                     	BoxDepth[iPrevLine])) *
                     	(
                        	BoxDepth[index] *
                          	(ADOWNStreamV + fabs(ADOWNStreamV)) / ASpatialStep *
                          	VVelocityPast[index] -
                          	BoxDepth[iPrevLine] *
                          	(AUPStreamV + fabs(AUPStreamV)) / ASpatialStep *
                          	VVelocityPast[iPrevLine] +
                          	BoxDepth[index] *
                          	(ADOWNStreamV - fabs(ADOWNStreamV)) / ASpatialStep *
                          	VVelocityPast[iNextLine] -
                          	BoxDepth[iPrevLine] *
                          	(AUPStreamV - fabs(AUPStreamV)) / ASpatialStep *
                          	VVelocityPast[index]
                     	) -
                     	UVAdvectionOfVEquation(i,j) -
                       	ATimeStep / 2.0 * Gravity * (PastHeight[index] -
                       	PastHeight[iPrevLine]) / ASpatialStep -
                       	ATimeStep / (2.0 * (BoxDepth[index] +
                        	                   BoxDepth[iPrevLine])) *
                     	(
                       			CoefRugosidade(index) +
                   	 			CoefRugosidade(iPrevLine)
                     	) *
                  	  	VForRugosity(i,j) *
                       	VVelocityPast[index] -
                        UCoriolis(i,j)
                       	+
                        VVerticalDiffusion(i,j) + VHorizontalDiffusion(i,j);



               }
            }
            //Second semi-timestep

            else if (SemiTimeStep == 2)
         	{
               //vs  implicit
               if (
                     (i > 0) &&
                     (
                     	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 0) ||//No southern boundary
                        (BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1)
                     ) &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[iPrevLine] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                  	(BoxArray[index].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) &&  //No eastern ocean boundary
                     (BoxArray[iPrevLine].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2)&& //No western ocean boundary
                     (BoxArray[iPrevLine].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) //&&  //No eastern ocean boundary
                     //(BoxArray[index].Dummybool1)
                  )
               {
               	//Cálculo do r - termo correspondente à velocidade na célula corrente no semi-passo anterior
                 if (
                 			(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 1) ||
                        (BoxArray[iPrevLine].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 1)
                     )
                  rtemporary[index] = VVelocityPast[index];
                 else
                 	rtemporary[index] =
                     	VVelocityPast[index] -
                        UCoriolis(i,j) +
                        VHorizontalDiffusion(i,j) -
                        UVAdvectionOfVEquation(i,j) +
                   		GravityOfRightSideOfVEquationOfMotion(i,j);


                 // Cálculo do a, b e c
                 atemporary[index] = aVCalculation(i,j);
                 btemporary[index] = bVCalculation(i,j);
                 ctemporary[index] = cVCalculation(i,j);



                 if (BoxArray[(i - 1) * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2) //South neighbour cell is a southern ocean boundary
                 {
                  	btemporary[index] = btemporary[index] + atemporary[index] *
                                        (BoxDepth[iPrevLine] +
                                         BoxDepth[index]
                                        ) / (2.0 * BoxDepth[iPrevLine]);

                     //DynamicHeight[iPrevLine] = SurfaceHeight;
                     //BoxDepth[iPrevLine] = BoxDepth[iPrevLine] +
                     //                     (DynamicHeight[i*revLine] - PastHeight[iPrevLine]);

                     double Parcel3;

                     Parcel3 = 2.0 * ASpatialStep * (DynamicHeight[iPrevLine]
                               - PastHeight[iPrevLine]) /
                               (ATimeStep * BoxDepth[iPrevLine]);

                     if (
                     		(j > 0) &&
                           (BoxDepth[iPrevLine - 1] > 0.0)
                        )
                        Parcel3 = Parcel3 - (BoxDepth[iPrevLine] + BoxDepth[iPrevLine - 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[iPrevLine]);
                     else
                     	Parcel3 = Parcel3 - (BoxDepth[iPrevLine] + BoxDepth[iPrevLine]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[iPrevLine]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[iPrevLine + 1] > 0.0)
                        )
                        Parcel3 = Parcel3 + (BoxDepth[iPrevLine] + BoxDepth[iPrevLine + 1]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[iPrevLine]);
                     else
                     	Parcel3 = Parcel3 + (BoxDepth[iPrevLine] + BoxDepth[iPrevLine]) *
                                             UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[iPrevLine]);

                     rtemporary[index] = rtemporary[index] - atemporary[index] *  Parcel3;


                     atemporary[index] = 0.0;
                  }
                  else
                  if (BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Cell is a northern ocean boundary
                  {
                  	btemporary[index] = btemporary[index] + ctemporary[index] *
                                        (BoxDepth[index] + BoxDepth[iPrevLine]) /
                                        (2.0 * BoxDepth[index]);

                     //DynamicHeight[index] = SurfaceHeight;
                     //BoxDepth[index] = BoxDepth[index] +
                     //                  (DynamicHeight[index] - PastHeight[index]);
                     double Parcel4;

                     Parcel4 = - 2.0 * ASpatialStep * (DynamicHeight[index]  - PastHeight[index]) /
                              (ATimeStep * BoxDepth[index]);


                     if (
                     		(j > 0) &&
                           (BoxDepth[index - 1] > 0.0)
                        )
                        Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index - 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     else
                     	Parcel4 = Parcel4 + (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j] /
                                             (2.0 * BoxDepth[index]);
                     if (
                     		(j < GridColumns - 1) &&
                           (BoxDepth[index + 1] > 0.0)
                        )
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index + 1]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);
                     else
                        Parcel4 = Parcel4 - (BoxDepth[index] + BoxDepth[index]) *
                                             UVelocityPast[i * (GridColumns + 1) + j + 1] /
                                             (2.0 * BoxDepth[index]);

                     rtemporary[index] = rtemporary[index] - ctemporary[index] * Parcel4;



                     ctemporary[index] = 0.0;
                  }
               }
               else
               {
               	atemporary[index] = 0.0; btemporary[index] = 0.0;
                  ctemporary[index] = 0.0; rtemporary[index] = 0.0;
               }

               //us explicit

               if (
               		(j > 0) &&
            			(
                  		(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 0) ||//No western boundary
                     	(BoxArray[index].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary == 1)
                  	)  &&
                     (BoxDepth[index] > CriticalDepthForLandBoundary) &&
                     (BoxDepth[index - 1] > CriticalDepthForLandBoundary) &&
                  	(BoxArray[index].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                  	(BoxArray[index].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)&& //No northern ocean boundary
                     (BoxArray[index - 1].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) //&&   //No southern ocean boundary
                     //(BoxArray[index].Dummybool1)
               	)
               {
                   	  	UVelocity[i * (GridColumns + 1) + j ] =
                       		UVelocityPast[i * (GridColumns + 1) + j ] -
                       		ATimeStep / (2.0 * (BoxDepth[index] +
                        		  BoxDepth[index - 1])
                       		) *
                       		(
                       		  	BoxDepth[index] *
                          		(ADOWNStreamU + fabs(ADOWNStreamU)) / ASpatialStep *
                          		UVelocityPast[i * (GridColumns + 1) + j ] -
                          		BoxDepth[index - 1] *
                          		(AUPStreamU + fabs(AUPStreamU)) / ASpatialStep *
                          		UVelocityPast[i * (GridColumns + 1) + j - 1] +
                          		BoxDepth[index] *
                          		(ADOWNStreamU - fabs(ADOWNStreamU)) / ASpatialStep *
                          		UVelocityPast[i * (GridColumns + 1) + j + 1] -
                          		BoxDepth[index - 1] *
                          		(AUPStreamU - fabs(AUPStreamU)) / ASpatialStep *
                          		UVelocityPast[i * (GridColumns + 1) + j ]
                           ) -
                           UVAdvectionOfUEquation(i,j) -
                       		ATimeStep / 2.0 * Gravity * (PastHeight[index] -
                       		PastHeight[index - 1]) / ASpatialStep -
                       		ATimeStep / (2.0 * (BoxDepth[index] +
                        		                   BoxDepth[index - 1])) *
                       		(
                       				CoefRugosidade(index) +
                   	 				CoefRugosidade(index - 1)
                       		) *
                  	  		UForRugosity(i,j) *
                       		UVelocityPast[i * (GridColumns + 1) + j ] +
                           VCoriolis(i,j)
                       	 	+
                           UHorizontalDiffusion(i,j) + UVerticalDiffusion(i,j);




               }
           	}
         }
      } //j
	} //i
}




double TBiDimensionalSango1::VCoriolis(int i, int j)
{
	double VVelocityComponent = 0.0, NumberOfParcels = 2.0;
    int index = i * GridColumns + j;
    int iNextLine = (i + 1) * GridColumns + j;

   VVelocityComponent = VVelocityComponent + VVelocityPast[index] + VVelocityPast[iNextLine];
   if ((j > 0) && (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
   {
   	VVelocityComponent = VVelocityComponent + VVelocityPast[index - 1];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocityPast[iNextLine - 1 ];
   	NumberOfParcels++;
   }

	return ATimeStep * Coriolis ( MyLatitude ) * VVelocityComponent / (2.0 * NumberOfParcels);
}


double TBiDimensionalSango1::UCoriolis(int i, int j)
{
	double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocityPast[i * (GridColumns + 1) + j ] + UVelocityPast[i * (GridColumns + 1) + j + 1];
   if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary))
   {
   	UVelocityComponent = UVelocityComponent + UVelocityPast[(i - 1) * (GridColumns + 1) + j];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1 ];
   	NumberOfParcels++;
   }

	return ATimeStep * Coriolis ( MyLatitude ) * UVelocityComponent / (2.0 * NumberOfParcels);
}

double TBiDimensionalSango1::UForRugosity(int i, int j)
{
	double VVelocityComponent = 0.0, NumberOfParcels = 2.0;
    int index = i * GridColumns + j;
    int iNextLine = (i + 1) * GridColumns + j;

   VVelocityComponent = VVelocityComponent + VVelocityPast[index ] + VVelocityPast[iNextLine];
   if ((j > 0) && (BoxDepth[index - 1] > CriticalDepthForLandBoundary))
   {
   	VVelocityComponent = VVelocityComponent + VVelocityPast[index - 1];
   	NumberOfParcels++;
   	VVelocityComponent = VVelocityComponent + VVelocityPast[iNextLine - 1 ];
   	NumberOfParcels++;
   }
      
	return sqrt(
   					sqr(UVelocityPast[i * (GridColumns + 1) + j ]) +
   		 			sqr(
                  		VVelocityComponent / NumberOfParcels
                  	)
          		);
}

double TBiDimensionalSango1::VForRugosity(int i, int j)
{
	double UVelocityComponent = 0.0, NumberOfParcels = 2.0;

   UVelocityComponent = UVelocityComponent + UVelocityPast[i * (GridColumns + 1) + j ] + UVelocityPast[i * (GridColumns + 1) + j + 1];
   if ((i > 0) && (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary))
   {
   	UVelocityComponent = UVelocityComponent + UVelocityPast[(i - 1) * (GridColumns + 1) + j];
   	NumberOfParcels++;
   	UVelocityComponent = UVelocityComponent + UVelocityPast[(i - 1) * (GridColumns + 1) + j + 1 ];
   	NumberOfParcels++;
   }
      
	return sqrt(
   					sqr(VVelocityPast[i * GridColumns + j ]) +
   		 			sqr(
                  		UVelocityComponent / NumberOfParcels
                  	)
          		);
}

void TBiDimensionalSango1::TideForcing()
{
	double MyYear, MyMonth, MyDay, MySecondSurfaceHeight;
	TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();
   if (MyTidePointer != NULL)
   {
      MyYear = MyPEcoDynClass->GetYear()/*- 1 + StartYear*/;
      MyMonth = MyPEcoDynClass->GetMonth();
      MyDay = MyPEcoDynClass->GetDate();
      //Debugger(MyYear); Debugger(MyMonth); Debugger(MyDay); Debugger(MyPIntegrate->GetCurrentTime());

   	for (int i = 0; i < GridLines; i++)
   	{
   		if (
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[1].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[2].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[3].TypeOfBoundary == 2) ||
         		(BoxArray[i * GridColumns + GridColumns - 1].BoxToBoundary[4].TypeOfBoundary == 2) &&
               (BoxDepth[i * GridColumns + GridColumns - 1] > CriticalDepthForLandBoundary)
   			)
         {
         	//Debugger(i);
					MyTime = MyPEcoDynClass->GetCurrTime();
					MyTidePointer->Inquiry(GetEcoDynClassName(), SurfaceHeight, MySecondSurfaceHeight, MyYear, MyMonth, MyDay, MyTime);
               SurfaceHeight = SurfaceHeight / CUBIC;  //convert from mm to
					DynamicHeight[NorthernTidalReference] = SurfaceHeight;
               MySecondSurfaceHeight = MySecondSurfaceHeight / CUBIC;  //convert from mm to m
					DynamicHeight[SouthernTidalReference] = MySecondSurfaceHeight;
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
               //Debugger(MyTime);
               //Debugger(DynamicHeight[i]);

         }
      }
      for (int j = 0; j < GridColumns - 1; j++)
      {
      	if (
         		(BoxArray[j].BoxToBoundary[1].TypeOfBoundary == 2) ||
         		(BoxArray[j].BoxToBoundary[2].TypeOfBoundary == 2) ||
         		(BoxArray[j].BoxToBoundary[3].TypeOfBoundary == 2) ||
         		(BoxArray[j].BoxToBoundary[4].TypeOfBoundary == 2) &&
               (BoxDepth[j] > CriticalDepthForLandBoundary)
   			)
         {
         	DynamicHeight[j] = DynamicHeight[GridColumns -1];
            DynamicHeight[(GridLines - 1) * GridColumns + j] =
            			DynamicHeight[(GridLines - 1) * GridColumns + GridColumns - 1];
         }

      }
   }
}

void TBiDimensionalSango1::Reinitialize()
{
	double MyWaterTemperature;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        int index = i * GridColumns + j;

      	atemporary[index] = 0.0; btemporary[index] = 0.0;
         ctemporary[index] = 0.0; rtemporary[index] = 0.0; 
      	if (
         		(BoxDepth[index] > CriticalDepthForLandBoundary)
            )
         {
      		BoxDepth[index ] = BoxDepth[index ] +
                                          	(DynamicHeight[index ] - PastHeight[index]);

         }
         else           //Este else foi retirado de todas as outras versões do bidimensional pois faz com que no caso da BoxDepth calculada
                        //imediatamente acima seja menor que a CriticalDepthForLandBoundary não se executem de imediato as instruções que anulam as velocidades
                        //mas tão somente no passo de cálculo seguinte. Este problema não tem consequências no modelo de Sango
         if (BoxDepth[index] <= CriticalDepthForLandBoundary)
         {
         	UVelocity[i * (GridColumns + 1) + j] = 0.0;
            VVelocity[index] = 0.0;
            UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
            VVelocity[(i + 1) * GridColumns + j] = 0.0;
         }
      }
	}
   //TideForcing();
   RiverForcing();

   if (SemiTimeStep == 2)
   {
   	for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   	{
   		VVelocityPast[i] = VVelocity[i];
   	}

      for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   	{
   		UVelocityPast[i] = UVelocity[i];
   	}

   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
			{
				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

      		WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
      		WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   	}
   }
   else
   if (SemiTimeStep == 1)
   {
   	for (int i = 0; i < NumberOfBoxes + GridLines; i++)
   	{
   		UVelocityPast[i] = UVelocity[i];
   	}

      for (int i = 0; i < NumberOfBoxes + GridColumns; i++)
   	{
   		VVelocityPast[i] = VVelocity[i];
   	}

   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		PastHeight[i] = DynamicHeight[i];
      	if (MyWaterTemperaturePointer != NULL)
			{
				MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
												               i,
												               "Water temperature",
												               ObjectCode);

      		WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
      	}
      	else
      		WaterDensity[i] = 1026.75; // seawater at 35psu and 10oC;
   	}

   }

   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;

   //RiverForcing();
}


void TBiDimensionalSango1::RiverForcing()
{
}

void TBiDimensionalSango1::UpdateOfTidalBoundariesVelocities()
{

	for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)
      {
        int index = i * GridColumns + j;
        int iNextLine = (i + 1) * GridColumns + j;
   		if (
         		(BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) ||
               (BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) ||
               (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||
               (BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2)
   			)
      	{

         	if ((BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2) && (i > 0) && (i < GridLines))  //Northern sea boundary
            	VVelocity[iNextLine] = (
            			                               		(VVelocity[index] + VVelocityPast[index]) / 2.0 *
                                                      	(BoxDepth[(i - 1) * GridColumns + j] + BoxDepth[index]) / 2.0 -
                                                      	(DynamicHeight[index] - PastHeight[index]) *
                                                         ASpatialStep / (ATimeStep/ 2.0)
                                                   	) / BoxDepth[index];

            else if ((BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) && (i < GridLines))  //Southern sea boundary
               VVelocity[index] =       (
            			                               		(VVelocity[iNextLine] + VVelocityPast[iNextLine]) / 2.0 *
                                                      	(BoxDepth[iNextLine] + BoxDepth[index]) / 2.0 +
                                                      	(DynamicHeight[index] - PastHeight[index]) *
                                                         ASpatialStep / (ATimeStep/ 2.0)
                                                   	) / BoxDepth[index];



            //Estes termos de velocidade estão centrados no tempo em vez de reportados a t, pois baseiam-se nos valores a de velocidade a N, S, W, ou E (conforme as fronteiras)
            //calculados em t e em t+1. No entanto, isto não faz mal, pois equivale a assumir que nas fonteiras marinhas a velocidade se mantém constante
            // ao longo de cada passo de cálculo, mudando instantaneamente quando o passo de cálculo muda.Ou seja, assume-se que a velocidade em t+1 é igual
            //à velocidade centrada no tempo (velocidade média do intervalo de tempo)
            else if  ((BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) &&  (j > 0) && (j < GridColumns)) //Eastern sea boundary
               UVelocity[i * (GridColumns + 1) + j + 1] =   (
            			                               				(UVelocity[i * (GridColumns + 1) + j] + UVelocityPast[i * (GridColumns + 1) + j]) / 2.0 *
                                                      			(BoxDepth[index - 1] + BoxDepth[index]) / 2.0 -
                                                      			(DynamicHeight[index] - PastHeight[index]) *
                                                         		ASpatialStep / (ATimeStep/ 2.0)
                                                   			) / BoxDepth[index];

            else if  ((BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) && (j < GridColumns)) //Western sea boundary
               UVelocity[i * (GridColumns + 1) + j] =   		(
            			                               				(UVelocity[i * (GridColumns + 1) + j + 1] + UVelocityPast[i * (GridColumns + 1) + j + 1]) / 2.0 *
                                                      			(BoxDepth[index + 1] + BoxDepth[index]) / 2.0 +
                                                      			(DynamicHeight[index] - PastHeight[index]) *
                                                         		ASpatialStep / (ATimeStep/ 2.0)
                                                   			) / BoxDepth[index];


         }
      }
   }
}



// Bidimensional hydrodynamic model object CPP code file
// 	@ Pedro Duarte
// Rev. A 16/7/2002
/**
 * NPI work
 * 	14 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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

//
// Constructors invoked outside EcoDyn shell...
//
TBiDimensionalDouro::TBiDimensionalDouro(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aUFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary)
        :TBiDimensionalIntertidal(className, timeStep, nLines, nColumns,
                      aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
	NumberOfParameters = nParams;
    MyLatitude = latitude;
    Gravity = gravity;
    HorizontalDiffCoef = horizontalDiffCoef;
    StartYear = startYear;
    CriticalDepthForLandBoundary = criticalDepthForLandBoundary;
	for (int j = 0; j < NumberOfBoxes; j++)
	{
		CoefManning[j] = aCoefManning[j];
    }
/*    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();*/

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
    	UFlowBetweenBoxes[j] = aUFlow[j];
	}
	for (int j = 0; j < NumberOfBoxes + GridColumns; j++)
	{
        VVelocity[j] = aVVelocity[j];
        VVelocityPast[j] = VVelocity[j];
    	VFlowBetweenBoxes[j] = aVFlow[j];
    }

    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);

	for (int i = 0; i < NumberOfBoxes; i++)
    {
        if (BoxDepth[i] > CriticalDepthForLandBoundary)
            BoxArray[i].Dummybool2 = true;
        else
            BoxArray[i].Dummybool2 = false;
    }
}

TBiDimensionalDouro::TBiDimensionalDouro(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
		 :TBiDimensionalIntertidal(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildBiDimensionalDouro();
}
// ...

TBiDimensionalDouro::TBiDimensionalDouro(TEcoDynClass* PEcoDynClass,
        char* className) : TBiDimensionalIntertidal(PEcoDynClass, className)

{
    BuildBiDimensionalDouro();
}

TBiDimensionalDouro::~TBiDimensionalDouro()
{
    freeMemory();
}

void TBiDimensionalDouro::freeMemory()
{
    try {
       if (NumberOfBoxes > 0)
        {
    //		delete [] UVelocity;    // made in TOneDimensionalDITransport
            delete [] VVelocity;
            delete [] UVelocityPast;
            delete [] VVelocityPast;
            delete [] UFlowBetweenBoxes;
            delete [] VFlowBetweenBoxes;
        }
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
        cerr << "TBiDimensionalDouro::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

void TBiDimensionalDouro::BuildBiDimensionalDouro()
{
    //Parameters file
    double MyValue;
    int X,Y, XV, YV;
	ParameterCount = 0;

	// Read in the parameters
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
                else if (strcmp(MyParameter, "Latitude") == 0)
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
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object Transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalDouro::BuildBiDimensionalDouro - Parameters: undefined object Transport" << endl;
#endif  // __BORLANDC__
        }
       //End reading parameters file
        CloseDataFile((void*)PReadWrite);
    }
    
    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();


    if (NumberOfBoxes > 0)
	{
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
                else if (strcmp(MyVariable, "V Velocity") == 0)
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
                        PReadWrite->ReadNumber(XV, YV + 1 + j, MyValue);
                        Salt[j] = MyValue;
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
                else if (strcmp(MyVariable, "Salt load") == 0)
                {
                    if (!PReadWrite->FindString("Salt load values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, SaltLoad[j]);
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object Transport", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TBiDimensionalDouro::BuildBiDimensionalDouro - Variables: undefined object Transport" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }

	for (int i = 0; i < NumberOfBoxes; i++)
    {
        if (BoxDepth[i] > CriticalDepthForLandBoundary)
            BoxArray[i].Dummybool2 = true;
        else
            BoxArray[i].Dummybool2 = false;
    }
}



double TBiDimensionalDouro::GetParameterValue(char* MyParameter)
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

    // parameters from TBiDimensionalDouro
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        value = MyLatitude;
    }
    else
        value = 0.0;
    return value;
}

bool TBiDimensionalDouro::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TBiDimensionalDouro
    else if (strcmp(MyParameter, "Latitude") == 0)
    {
        MyLatitude = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.09
bool TBiDimensionalDouro::SaveVariables()
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


bool TBiDimensionalDouro::SaveParameters()
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



//Huniform harmonic forced tide

void TBiDimensionalDouro::TideForcing()
{
	double MyYear, MyMonth, MyDay;
    int i, nbox;
	TEcoDynClass* MyTidePointer = MyPEcoDynClass->GetTidePointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    if (MyTidePointer != NULL)
    {
        MyYear = MyPEcoDynClass->GetYear()/*- 1 + StartYear*/;
        MyMonth = MyPEcoDynClass->GetMonth();
        MyDay = MyPEcoDynClass->GetDate();
        MyTime = MyPEcoDynClass->GetCurrTime();

        MyTidePointer->Inquiry(GetEcoDynClassName(), SurfaceHeight,
                MyYear, MyMonth, MyDay, MyTime);
        SurfaceHeight = SurfaceHeight / CUBIC;  //convert from mm to m

   	    for (nbox = 0; nbox < pSubDomain->NumberOfBoxes; nbox++)
   	    {
            i = pSubDomain->BoxNumber[nbox];
   		    if ((BoxArray[i].BoxToBoundary[1].TypeOfBoundary == 2) ||
         		(BoxArray[i].BoxToBoundary[2].TypeOfBoundary == 2) ||
         		(BoxArray[i].BoxToBoundary[3].TypeOfBoundary == 2) ||
         		(BoxArray[i].BoxToBoundary[4].TypeOfBoundary == 2))
            {
                DynamicHeight[i] = SurfaceHeight;
            }
        }
    }
}


void TBiDimensionalDouro::RiverForcing()
{
   int NumberOfHoursForFlowTimeSeries;
   NumberOfDaysForFlowTimeSeries = MyPEcoDynClass->GetNumberOfDaysForFlowTimeSeries();
   NumberOfHoursForFlowTimeSeries = MyPEcoDynClass->GetNumberOfHoursForFlowTimeSeries();
   if ((MyRiverRecord != NULL) && ((NumberOfDaysForFlowTimeSeries > 0) || (NumberOfHoursForFlowTimeSeries) > 0) )
   {
      int ADay, AHour, AMoment;
      int LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
      if  (NumberOfDaysForFlowTimeSeries >= 1)
      {
         ADay = MIN(MyPEcoDynClass->GetJulianDay() - 1,
                 NumberOfDaysForFlowTimeSeries - 1);
         AMoment = ADay;
      }
      else
      if (NumberOfHoursForFlowTimeSeries >= 1)
      {
         ADay = MyPEcoDynClass->GetJulianDay() - 1;
         AHour = ADay * DAYSTOHOURS + floor(MyPEcoDynClass->GetCurrTime());
         AMoment = MIN(AHour, NumberOfHoursForFlowTimeSeries - 1);
      }
      else
#ifdef __BORLANDC__
         MessageBox(0, "TBiDimensionalDouro::RiverForcing", "Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForFlowTimeSeries", MB_OK);
#else  // __BORLANDC__
         cerr << "TBiDimensionalDouro::RiverForcing - Incorrect value in NumberOfDaysForFlowTimeSeries or NumberOfHoursForFlowTimeSeries" << endl;
#endif  // __BORLANDC__
      //Debugger(NumberOfRivers);
      //Debugger(NumberOfDaysForFlowTimeSeries);
      for (int River = 0; River < NumberOfRivers; River++)
      {
        
        LineIndex = MyRiverRecord[River].LineCoordinate;
        ColumnIndex = MyRiverRecord[River].ColumnCoordinate;
        //if (MyPEcoDynClass->GetCurrTime() > 0.7) {Debugger(LineIndex); Debugger(ColumnIndex); Debugger(River);}
        //Debugger(ADay); Debugger(LineIndex); Debugger(ColumnIndex);
        if ((MyRiverRecord[River].BoundaryNumber == 1) || (MyRiverRecord[River].BoundaryNumber == 3))
        {
            if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex < GridLines - 1))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[(LineIndex + 1) * GridColumns + ColumnIndex];
               //if (BoxArray[(LineIndex + 1) * GridColumns + ColumnIndex].Dummybool2)
                  VVelocity[(LineIndex + 1) * GridColumns + ColumnIndex] = MyRiverRecord[River].RiverFlow[AMoment] /
                                                               (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Northern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 1) && (LineIndex == GridLines - 1))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the North located at northern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
             cerr << "TBiDimensionalDouro::RiverForcing - "
                << "River flow to the North located at northern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex > 0))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[(LineIndex - 1) * GridColumns + ColumnIndex];
               //if (BoxArray[(LineIndex - 1) * GridColumns + ColumnIndex].Dummybool2)
                  VVelocity[LineIndex * GridColumns + ColumnIndex] = MyRiverRecord[River].RiverFlow[AMoment] /
                                                               (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Southern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 3) && (LineIndex == 0))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the South located at southern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
             cerr << "TBiDimensionalDouro::RiverForcing - "
                << "River flow to the South located at southern grid limit" << endl;
#endif  // __BORLANDC__
            //Debugger(LineIndex); Debugger(ColumnIndex);
            //Debugger(MyRiverRecord[River].RiverFlow[AMoment]);

        }
        else
        if ((MyRiverRecord[River].BoundaryNumber == 2) || (MyRiverRecord[River].BoundaryNumber == 4))
        {
            if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex < GridColumns - 1))
            {

               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[LineIndex * GridColumns + ColumnIndex + 1];
               //if (BoxArray[LineIndex * GridColumns + ColumnIndex + 1].Dummybool2)
                  UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex + 1] = MyRiverRecord[River].RiverFlow[AMoment] /
                                                                     (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Western river boundary

            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 2) && (ColumnIndex == GridColumns - 1))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the east located at eastern grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
             cerr << "TBiDimensionalDouro::RiverForcing - "
                << "River flow to the East located at eastern grid limit" << endl;
#endif  // __BORLANDC__

            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex > 0))
            {
               BoxDepth[LineIndex * GridColumns + ColumnIndex] = BoxDepth[LineIndex * GridColumns + ColumnIndex - 1];
               //if (BoxArray[LineIndex * GridColumns + ColumnIndex - 1].Dummybool2)
                  UVelocity[LineIndex * (GridColumns + 1) + ColumnIndex] = MyRiverRecord[River].RiverFlow[AMoment] /
                                                                     (BoxDepth[LineIndex * GridColumns + ColumnIndex] * ASpatialStep);  //Eastern river boundary
            }
            else
            if ((MyRiverRecord[River].BoundaryNumber == 4) && (ColumnIndex == 0))
#ifdef __BORLANDC__
               MessageBox(0,
					  "Hydrodynamic bidimensional object - RiverForcing",
					  "River flow to the West located at western grid limit",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
             cerr << "TBiDimensionalDouro::RiverForcing - "
                << "River flow to the West located at western grid limit" << endl;
#endif  // __BORLANDC__


        }
      }
   }
}



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
#include "globals.h"
#include "iodll.h"

// One dimensional density independent class
// Public functions
//
// Constructors invoked outside EcoDyn shell...
//
TOneDimensionalDITransport::TOneDimensionalDITransport(char* className, float timeStep,
        char* morphologyFilename):TTransport(className, timeStep, morphologyFilename)

{
    BuildOneDimensionalDITransport();
}


TOneDimensionalDITransport::TOneDimensionalDITransport(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TTransport(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    BuildOneDimensionalDITransport();
}
// ...

TOneDimensionalDITransport::TOneDimensionalDITransport(TEcoDynClass* PEcoDynClass,
        char* className) :TTransport(PEcoDynClass, className)

{
    BuildOneDimensionalDITransport();
}

void TOneDimensionalDITransport::BuildOneDimensionalDITransport()
{
    ATimeStep = MyPEcoDynClass->GetTimeStep();
	if (NumberOfBoxes > 0)
	{
		DynamicHeight = new double[NumberOfBoxes];
		PastHeight = new double[NumberOfBoxes];
		BoxDepth = new double[NumberOfBoxes];
        PastBoxDepth = new double[NumberOfBoxes];
		BoxLength = new double[NumberOfBoxes];
		BoxVolume = new double[NumberOfBoxes];
        PastBoxVolume = new double[NumberOfBoxes];
		WaterDensity = new double[NumberOfBoxes];
		CoefManning = new double[NumberOfBoxes];
//        PastGeneric = new double[NumberOfBoxes];
        FlowDischarge = new double[NumberOfBoxes];
        FlowUptake = new double[NumberOfBoxes];
        EvaporativeFlux = new double[NumberOfBoxes];

		UVelocity = new double[NumberOfBoxes + 1];
        PastUVelocity = new double[NumberOfBoxes + 1];
		FlowBetweenBoxes = new double[NumberOfBoxes + 1];
        PastFlowBetweenBoxes = new double[NumberOfBoxes + 1];
		BoxWidth = new double[NumberOfBoxes + 1];
		BoxCrossSectionalArea = new double[NumberOfBoxes + 1];
        PastBoxCrossSectionalArea = new double[NumberOfBoxes + 1];
		a = new double[NumberOfBoxes + 1];
		b = new double[NumberOfBoxes + 1];
		c = new double[NumberOfBoxes + 1];
		r = new double[NumberOfBoxes + 1];
		gam = new double[NumberOfBoxes + 1];
		u = new double[NumberOfBoxes + 1];
	}
	else {
#ifdef __BORLANDC__
		MessageBox(0,
					  "Hydrodynamic array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
        cerr << "TOneDimensionalDITransport::BuildOneDimensionalDITransport - Hydrodynamic array not dimensioned" << endl;
#endif  // __BORLANDC__
        return;
    }

    for (int i = 0; i < NumberOfBoxes + 1; i++)
    {
   	    UVelocity[i] = 0.0;
        PastUVelocity[i] = 0.0;
		FlowBetweenBoxes[i] = 0.0;
        PastFlowBetweenBoxes[i] = 0.0;
        BoxWidth[i] = 0.0;
        BoxCrossSectionalArea[i] = 0.0;
        PastBoxCrossSectionalArea[i] = 0.0;
        a[i] = 0.0;
        b[i] = 0.0;
        c[i] = 0.0;
        r[i] = 0.0;
        gam[i] = 0.0;
        u[i] = 0.0;
        if (i < NumberOfBoxes)
        {
            //GenericLoad[i] = 0.0;
            SaltLoad[i] = 0.0;
			DynamicHeight[i] = 0.0;
			PastHeight[i] = 0.0;
			BoxDepth[i] = MyPEcoDynClass->GetBoxDepth(i);
            PastBoxDepth[i] = 0.0;
			BoxLength[i] = 0.0;
			BoxVolume[i] = 0.0;
            PastBoxVolume[i] = 0.0;
			WaterDensity[i] = 0.0;
			CoefManning[i] = 0.0;
//            PastGeneric[i] = 0.0;
            FlowDischarge[i] = 0.0;
            FlowUptake[i] = 0.0;
            EvaporativeFlux[i] = 0.0;
        }
    }

    //Variables file

    int X,Y, XV, YV;
	char MyVariable[65];
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Transport");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Transport variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TOneDimensionalDITransport::BuildOneDimensionalDITransport - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Transport", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);
                if (strcmp(MyVariable, "U Velocity") == 0)
                {
                    if (PReadWrite->FindString("U Velocity values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes + 1; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, UVelocity[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes + 1; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, UVelocity[j]);
                            PastUVelocity[j] = UVelocity[j];
                            //Debugger(UVelocity[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Dynamic height") == 0)
                {
                    if (PReadWrite->FindString("Dynamic height values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, DynamicHeight[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, DynamicHeight[j]);
                            //Debugger(DynamicHeight[j]);
                            PastHeight[j] = DynamicHeight[j];
                        }
                    }
                }
                else if (strcmp(MyVariable, "Flow") == 0)
                {
                    if (PReadWrite->FindString("Flow values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes + 1; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, FlowBetweenBoxes[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes + 1; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, FlowBetweenBoxes[j]);
                            //Debugger(FlowBetweenBoxes[j]);
                            PastFlowBetweenBoxes[j] = FlowBetweenBoxes[j];
                        }
                    }
                }
             else if (strcmp(MyVariable, "Flow discharge") == 0)
                {
                    if (PReadWrite->FindString("Flow discharge values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, FlowDischarge[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, FlowDischarge[j]);
                       //Debugger(FlowDischarge[j]);
                        }
                    }
                }
             else if (strcmp(MyVariable, "Flow uptake") == 0)
                {
                    if (PReadWrite->FindString("Flow uptake values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, FlowUptake[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, FlowUptake[j]);
                       //Debugger(FlowUptake[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Salinity") == 0)
                {
                PReadWrite->ReadNumber(X+3, i, RiverSalt);
                //Debugger(RiverSalt);
                    if (PReadWrite->FindString("Salinity values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Salt[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, Salt[j]);
                        }
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
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, WaterDensity[j]);
                            //Debugger(WaterDensity[j]);
                        }
                    }
                }
             else if (strcmp(MyVariable, "Evaporative flux") == 0)
                {
                CalculateEvaporation = true;
                    if (PReadWrite->FindString("Evaporative flux values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, EvaporativeFlux[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 5 + j, i, EvaporativeFlux[j]);
                            //Debugger(EvaporativeFlux[j]);
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TOneDimensionalDITransport::BuildOneDimensionalDITransport - variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        //End reading variables file
        CloseDataFile((void*)PReadWrite);
    }

    Gravity = 9.80665; //Calcular a partir da latitude
	//Parameters file

    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
	ParameterCount = 0;
	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Transport");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Transport parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TOneDimensionalDITransport::BuildOneDimensionalDITransport - parameters file missing" << endl;
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

                if (strcmp(MyParameter, "Gravity") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Gravity);
                    //Debugger(Gravity);
                }
                else
                if (strcmp(MyParameter, "Coeficiente de Manning") == 0)
                {
                    if (PReadWrite->FindString("Coeficiente de Manning values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, CoefManning[j]);
                    }
                    else {
                        double MyValue;
                        PReadWrite->ReadNumber(X + 3, i, MyValue);
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            //PReadWrite->ReadNumber(X + 3 + j, i, CoefManning[j]);
                            CoefManning[j] = MyValue;
                            //Debugger(CoefManning[j]);
                        }
                    }
                }
                else
                if (strcmp(MyParameter, "Water density") == 0)
                {
                    if (PReadWrite->FindString("Water density values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, WaterDensity[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + 3 + j, i, WaterDensity[j]);
                            //Debugger(WaterDensity[j]);
                        }
                    }
                }
                else
                if (strcmp(MyParameter, "Difusion coeficient") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, HorizontalDiffCoef);
                    //Debugger(HorizontalDiffCoef);
                }
                else
                if (strcmp(MyParameter, "Box Width") == 0)
                {
                    if (PReadWrite->FindString("Box Width values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes + 1; j++)
                            PReadWrite->ReadNumber(XV, YV + 1 + j, BoxWidth[j]);
                    }
                    else {
                        for (int j = 0; j < NumberOfBoxes + 1; j++)
                        {
                            PReadWrite->ReadNumber(X + 3 + j, i, BoxWidth[j]);
                            //Debugger(BoxWidth[j]);
                        }
                    }
                }
                else
                if (strcmp(MyParameter, "Start Year") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, StartYear);
                    //Debugger(StartYear);
                }
                else
                if (strcmp(MyParameter, "kdiffusion") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, kdiffusion);
                    //Debugger(kdiffusion);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TOneDimensionalDITransport::BuildOneDimensionalDITransport - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	//End reading parameters file
    UpperLayer = GridLines - 1;

	for (int i = 0; i < NumberOfBoxes + 1; i++)
	{
   	    if (i < NumberOfBoxes)
            {
            
			//BoxDepth[i] = MyPEcoDynClass->GetBoxDepth(i) + DynamicHeight[i];
            PastBoxDepth[i] = BoxDepth[i];
			BoxLength[i] = MyPEcoDynClass->GetBoxLength(i);
			BoxVolume[i] = MyPEcoDynClass->GetBoxVolume(i);
            PastBoxVolume[i] = BoxVolume[i];

            if (i == 0)
				BoxCrossSectionalArea[i] = BoxWidth[i] * BoxDepth[i];
            else if ((i > 0) && (i < NumberOfBoxes))
         	    BoxCrossSectionalArea[i] = BoxWidth[i] * (BoxDepth[i - 1] + BoxDepth[i]) / 2.0;
   		    else if (i == NumberOfBoxes)
   			    BoxCrossSectionalArea[i] = BoxDepth[i - 1] * BoxWidth[i];

            PastBoxCrossSectionalArea[i] = BoxCrossSectionalArea[i];
        }
        else
        {
      	    BoxWidth[i] = BoxWidth[i - 1];
            BoxCrossSectionalArea[i] = BoxWidth[i - 1] * BoxDepth[i - 1];
            PastBoxCrossSectionalArea[i] = BoxCrossSectionalArea[i];
        }
	}
}

TOneDimensionalDITransport::~TOneDimensionalDITransport()
{
    freeMemory();
}

void TOneDimensionalDITransport::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] DynamicHeight;
            delete [] PastHeight;
            delete [] BoxDepth;
            delete [] PastBoxDepth;
            delete [] BoxLength;
            delete [] BoxVolume;
            delete [] PastBoxVolume;
            delete [] WaterDensity;
            delete [] CoefManning;
            delete [] FlowDischarge;
            delete [] FlowUptake;
            delete [] EvaporativeFlux;
            delete [] UVelocity;
            delete [] PastUVelocity;
            delete [] FlowBetweenBoxes;
            delete [] PastFlowBetweenBoxes;
            delete [] BoxWidth;
            delete [] BoxCrossSectionalArea;
            delete [] PastBoxCrossSectionalArea;
            delete [] a;
            delete [] b;
            delete [] c;
            delete [] r;
            delete [] gam;
            delete [] u;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
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
        cerr << "TOneDimensionalDITransport::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

double TOneDimensionalDITransport::GetParameterValue(char* MyParameter)
{
  double value;

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
    else
        value = 0.0;
    return value;
}

bool TOneDimensionalDITransport::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

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
    else
        rc = false;

    return rc;
}

// AP, 2007.07.09
bool TOneDimensionalDITransport::SaveVariables()
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
        if (i > 0)
        {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Salinity") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverSalt, 6);
        }
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Salinity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Water density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("U Velocity values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Dynamic height values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow discharge values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Flow uptake values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Evaporative flux values");
    PReadWrite->WriteSeparator(true);

    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Salt[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(WaterDensity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(UVelocity[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DynamicHeight[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowBetweenBoxes[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowDischarge[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(FlowUptake[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(EvaporativeFlux[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    // one more value for UVelocity and FlowBetweenBoxes
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(UVelocity[NumberOfBoxes], 8);
    PReadWrite->WriteSeparator();
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(FlowBetweenBoxes[NumberOfBoxes], 8);
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TOneDimensionalDITransport::SaveParameters()
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

void TOneDimensionalDITransport::Go()
{
    int i;
	for (int i = 0; i < NumberOfBoxes + 1; i++)
	{
   	    if (i < NumberOfBoxes)
        {
      	    PastHeight[i] = DynamicHeight[i];
            PastBoxDepth[i] = BoxDepth[i];
        }
		UVelocity[i] = FlowBetweenBoxes[i] / BoxCrossSectionalArea[i];
        PastBoxCrossSectionalArea[i] = BoxCrossSectionalArea[i];
        PastFlowBetweenBoxes[i] = FlowBetweenBoxes[i];
	}

    EquationOfMotion();

    IntegrationTridag(NumberOfBoxes + 1);

    for (int i = 0; i < NumberOfBoxes + 1; i++)
   	    FlowBetweenBoxes[i] = u[i];

    Continuity();

    if (CalculateEvaporation)
        Evaporate();

    for (i = 0; i < NumberOfBoxes + 1; i++)
    {
   	    if (i == 0)
      	    BoxCrossSectionalArea[i] = BoxWidth[i] * BoxDepth[i];
        else if ((i > 0) && (i < NumberOfBoxes))
            BoxCrossSectionalArea[i] = BoxWidth[i] * (BoxDepth[i - 1] + BoxDepth[i]) / 2.0;
   	    else if (i == NumberOfBoxes)
   		    BoxCrossSectionalArea[i] = BoxDepth[i - 1] * BoxWidth[i];
    }

    GenericRiver = RiverSalt;
    GenericLoad = SaltLoad;

    AdvectDiffuse(Salt);

//    Salt = Generic;
}

void TOneDimensionalDITransport::Go(double* AGeneric,
			double* ASecondGeneric,
                        double ARiver,
			double AOcean)

{
    Generic = AGeneric;
    GenericRiver = ARiver; 
    GenericOcean = AOcean;
    GenericLoad = ASecondGeneric;
    AdvectDiffuse(AGeneric);
}

void TOneDimensionalDITransport::Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "U Velocity") == 0)
		Value = UVelocity[MyBoxNumber]; //m / s
	else if (strcmp(MyParameter, "Flow") == 0)
		Value = FlowBetweenBoxes[MyBoxNumber];  // m3 / s
	else if (strcmp(MyParameter, "Dynamic height") == 0)
		Value = DynamicHeight[MyBoxNumber];
	else if (strcmp(MyParameter, "Box cross sectional area") == 0)
		Value = BoxCrossSectionalArea[MyBoxNumber];
	else if (strcmp(MyParameter, "Salinity") == 0)
		Value = Salt[MyBoxNumber];
    else if (strcmp(MyParameter, "Water density") == 0)
		Value = WaterDensity[MyBoxNumber];
    else if (strcmp(MyParameter, "Box depth") == 0)
		Value = BoxDepth[MyBoxNumber];
    else if (strcmp(MyParameter, "Flow discharge") == 0)
		Value = FlowDischarge[MyBoxNumber];
    else if (strcmp(MyParameter, "Flow uptake") == 0)
		Value = FlowUptake[MyBoxNumber];
    else if (strcmp(MyParameter, "Evaporative flux") == 0)
		Value = EvaporativeFlux[MyBoxNumber];
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
		Value = CoefRugosidade(MyBoxNumber);
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
                      "EcoDynamo Alert - Inquiry 7",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TOneDimensionalDITransport::Inquiry 7 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.05.28
bool TOneDimensionalDITransport::SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Dynamic height") == 0)
		DynamicHeight[BoxNumber] = Value;
	else if (strcmp(VariableName, "Box cross sectional area") == 0)
		BoxCrossSectionalArea[BoxNumber] = Value;
	else if (strcmp(VariableName, "Salinity") == 0)
		Salt[BoxNumber] = Value;
    else if (strcmp(VariableName, "Water density") == 0)
		WaterDensity[BoxNumber] = Value;
    else if (strcmp(VariableName, "Box depth") == 0)
		BoxDepth[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow discharge") == 0)
		FlowDischarge[BoxNumber] = Value;
    else if (strcmp(VariableName, "Flow uptake") == 0)
		FlowUptake[BoxNumber] = Value;

    else
        rc = false;

    return rc;
}


void TOneDimensionalDITransport::EquationOfMotion()    //Implicit scheme
{
	double MeanVelocityBeforeBox, MeanVelocityAfterBox;
    double DamDischarge;

	TEcoDynClass * MyFlowPointer = MyPEcoDynClass->GetFlowPointer();

	if (MyFlowPointer != NULL)
	{
		for (int i = 0; i < NumberOfBoxes + 1; i++) // Fronteira fluvial
		{
			if (i == 0)  //Fronteira fluvial
			{
				MyFlowPointer->Inquiry(GetEcoDynClassName(), RiverFlow,
													i,
													"Hourly river flow",
													ObjectCode);

                a[i] = 0.0;
                b[i] = 1.0;
                c[i] = 0.0;
                r[i] = RiverFlow;
			}
			else if ((i > 0) && (i < NumberOfBoxes)) //Caixas intermedias
			{
				MeanWidthBeforeBox = (BoxWidth[i - 1] + BoxWidth[i]) / 2.0;
				MeanWidthAfterBox = (BoxWidth[i] + BoxWidth[i + 1]) / 2.0;
                MeanVelocityBeforeBox = (PastFlowBetweenBoxes[i - 1] / PastBoxCrossSectionalArea[i - 1] +
                                     PastFlowBetweenBoxes[i] / PastBoxCrossSectionalArea[i]) / 2.0;

				MeanVelocityAfterBox = (PastFlowBetweenBoxes[i] / PastBoxCrossSectionalArea[i] +
                                     PastFlowBetweenBoxes[i + 1] / PastBoxCrossSectionalArea[i + 1]) / 2.0;

				if (MeanVelocityAfterBox > 0.0)
					AlfaB = -1.0;
				else
					AlfaB = 1.0;

                if (MeanVelocityBeforeBox > 0.0)
					AlfaA = -1.0;
				else
					AlfaA = 1.0;


				a[i] = - Gravity * BoxCrossSectionalArea[i] *
						(
                  	(ATimeStep * ATimeStep) /
							(BoxLength[i] * BoxLength[i])
                  ) *
                  1.0 / MeanWidthBeforeBox -
                  (
                  	(ATimeStep) / (BoxLength[i])
                  ) * MeanVelocityBeforeBox *
                  (1 - AlfaA) / 2 -
                  ATimeStep * HorizontalDiffCoef / (BoxLength[i] * BoxLength[i]);


				b[i] = 1.0 + ATimeStep / BoxLength[i] * MeanVelocityAfterBox * (1 - AlfaB) / 2 -
						 ATimeStep / BoxLength[i] * MeanVelocityBeforeBox * (1 + AlfaA) / 2 +
						 Gravity * BoxCrossSectionalArea[i] *
                   (
                   	(ATimeStep * ATimeStep) /
							(BoxLength[i] * BoxLength[i])
                   ) * 1.0 / MeanWidthAfterBox +
						Gravity * BoxCrossSectionalArea[i] *
                  (
                  	(ATimeStep * ATimeStep) /
							(BoxLength[i] * BoxLength[i])
                  ) * 1.0 / MeanWidthBeforeBox +
                  2.0 * ATimeStep * HorizontalDiffCoef / (BoxLength[i] * BoxLength[i]) +
                  ATimeStep * BoxWidth[i] * CoefRugosidade(i) * fabs(FlowBetweenBoxes[i]) /  (BoxCrossSectionalArea[i] * BoxCrossSectionalArea[i]);



				c[i] = ATimeStep / BoxLength[i] * MeanVelocityAfterBox * (1 + AlfaB) / 2 -
						 Gravity * BoxCrossSectionalArea[i] *
                   (
                   	(ATimeStep * ATimeStep) /
							(BoxLength[i] * BoxLength[i])
                   ) * 1.0 / MeanWidthAfterBox -
                   ATimeStep * HorizontalDiffCoef / (BoxLength[i] * BoxLength[i]);

				r[i] = FlowBetweenBoxes[i] - Gravity * BoxCrossSectionalArea[i] *
						 ATimeStep / BoxLength[i] * DynamicHeight[i] +
						 ATimeStep / BoxLength[i] * Gravity * BoxCrossSectionalArea[i] *
						 DynamicHeight[i - 1]
                   +
                   (
                   	(ATimeStep * ATimeStep) /
                     (BoxLength[i] * BoxLength[i]) *
                     Gravity * BoxCrossSectionalArea[i] *
                     (
                     	(1.0 / MeanWidthAfterBox * (- FlowDischarge[i] + FlowUptake[i])) -
                        (1.0 / MeanWidthBeforeBox * ( - FlowDischarge[i - 1] + FlowUptake[i - 1]))
                     )
                   );

			}
			else if (i == NumberOfBoxes)//Fronteira marinha
			{
         	    MyFlowPointer->Inquiry(GetEcoDynClassName(), DamDischarge,
												   i,
												  "Hourly dam discharge",
												   ObjectCode);
                a[i] = 0.0;
                b[i] = 1.0;
                c[i] = 0.0;
                r[i] = DamDischarge;
			}
		}
	}
}


void TOneDimensionalDITransport::Continuity()
{
    for (int i = 0; i < NumberOfBoxes; i++)
	{
   	    MeanWidthBeforeBox = (BoxWidth[i + 1] + BoxWidth[i]) / 2.0;
		if (i == 0)
			DynamicHeight[i] = DynamicHeight[i] - 1 / MeanWidthBeforeBox *
								    ATimeStep / BoxLength[i] *
								    (FlowBetweenBoxes[i + 1] - RiverFlow - FlowDischarge[i] + FlowUptake[i]);
		else if (i > 0)
		{
			DynamicHeight[i] = DynamicHeight[i] - 1 / MeanWidthBeforeBox *
									 ATimeStep / BoxLength[i] *
									 (FlowBetweenBoxes[i + 1] - FlowBetweenBoxes[i] - FlowDischarge[i] + FlowUptake[i]);
		}
        BoxDepth[i] = BoxDepth[i] + (DynamicHeight[i] - PastHeight[i]);
    }
}

void TOneDimensionalDITransport::AdvectDiffuse(double* Generic)
{
	double /*MeanConcentrationBeforeBox,*/ MeanConcentrationAfterBox,
          MeanCrossSectionalAreaBeforeBox, PastMeanCrossSectionalAreaBeforeBox, DamDischarge;
    double* PastGeneric = new double[NumberOfBoxes];

    TEcoDynClass* MyFlowPointer = MyPEcoDynClass->GetFlowPointer();


    for (int i = 0; i < NumberOfBoxes; i++)
   	    PastGeneric[i] = Generic[i];


    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    if (i == 0)  //Fronteira fluvial
        {
      	    if (MyFlowPointer != NULL)
            {
        		MyFlowPointer->Inquiry(GetEcoDynClassName(), RiverFlow,
		  											i,
		 											"Hourly river flow",
													ObjectCode);
        		FlowBetweenBoxes[i] = RiverFlow;
            }

//            MeanConcentrationAfterBox = (PastGeneric[i] + PastGeneric[i + 1]) / 2.0;
            PastMeanCrossSectionalAreaBeforeBox = (PastBoxCrossSectionalArea[i] + PastBoxCrossSectionalArea[i + 1]) / 2.0;
            MeanCrossSectionalAreaBeforeBox = (BoxCrossSectionalArea[i] + BoxCrossSectionalArea[i + 1]) / 2.0;

            if (FlowBetweenBoxes[i + 1] < 0.0)
         	    AlfaB = -1.0;
            else
         	    AlfaB = 1.0;

            Generic[i] = (PastGeneric[i] * PastMeanCrossSectionalAreaBeforeBox -
           	      			ATimeStep / BoxLength[i] *
                    			(
                           	(
                              	(1 - AlfaB)/2 * PastGeneric[i + 1] * FlowBetweenBoxes[i + 1] +
                              	(1 + AlfaB)/2 * PastGeneric[i] * FlowBetweenBoxes[i + 1]
                              ) -
                       			GenericRiver * RiverFlow
                         	) +
                    		 	ATimeStep / (BoxLength[i] * BoxLength[i]) *
                         	(
                         		PastBoxCrossSectionalArea[i + 1] *
                           	(
                           		HorizontalDiffCoef * (PastGeneric[i + 1] - PastGeneric[i])
                           	)
                         	)
                       	) / MeanCrossSectionalAreaBeforeBox +
                        ATimeStep * (FlowDischarge[i] * GenericLoad[i] - FlowUptake[i] * PastGeneric[i]) /
                        (PastMeanCrossSectionalAreaBeforeBox * BoxLength[i]);


        }
        else if ((i > 0) && (i < NumberOfBoxes - 1))
        {
//            MeanConcentrationAfterBox = (PastGeneric[i] + PastGeneric[i + 1]) / 2.0;
            MeanCrossSectionalAreaBeforeBox = (BoxCrossSectionalArea[i] + BoxCrossSectionalArea[i + 1]) / 2.0;
            PastMeanCrossSectionalAreaBeforeBox = (PastBoxCrossSectionalArea[i] + PastBoxCrossSectionalArea[i + 1]) / 2.0;

            if (FlowBetweenBoxes[i + 1] < 0.0)
         	    AlfaB = -1.0;
            else
         	    AlfaB = 1.0;

            if (FlowBetweenBoxes[i] < 0.0)
         	    AlfaA = -1.0;
            else
         	    AlfaA = 1.0;

            Generic[i] = (PastGeneric[i] * PastMeanCrossSectionalAreaBeforeBox -
            	      		ATimeStep / BoxLength[i] *
                           (
                     			(
                              	(1 - AlfaB)/2 * PastGeneric[i + 1] * FlowBetweenBoxes[i + 1] +
                              	(1 + AlfaB)/2 * PastGeneric[i] * FlowBetweenBoxes[i + 1]
                           	) -
                           	(
                              	(1 - AlfaA)/2 * PastGeneric[i] * FlowBetweenBoxes[i] +
                              	(1 + AlfaA)/2 * PastGeneric[i - 1] * FlowBetweenBoxes[i]
                           	)
                           )  +
                     		ATimeStep / (BoxLength[i] * BoxLength[i]) *
                     		(
                           	PastBoxCrossSectionalArea[i + 1] *
                        		(
                        			HorizontalDiffCoef * (PastGeneric[i + 1] - PastGeneric[i])
                        		)  -
                     			PastBoxCrossSectionalArea[i] *
                        		(
                        			HorizontalDiffCoef * (PastGeneric[i] - PastGeneric[i - 1])
                        		)
                           )
                         ) / MeanCrossSectionalAreaBeforeBox +
                        ATimeStep * (FlowDischarge[i] * GenericLoad[i] - FlowUptake[i] * PastGeneric[i]) /
                        (PastMeanCrossSectionalAreaBeforeBox * BoxLength[i]);

        }
        else if (i == NumberOfBoxes - 1)
        {
      	    if (MyFlowPointer != NULL)
            {
        		MyFlowPointer->Inquiry(GetEcoDynClassName(), DamDischarge,
		  											i,
		 											"Hourly dam discharge",
													ObjectCode);
            }

            MeanConcentrationAfterBox = PastGeneric[i];                                         //
            MeanCrossSectionalAreaBeforeBox = (BoxCrossSectionalArea[i] + BoxCrossSectionalArea[i + 1]) / 2.0;
            PastMeanCrossSectionalAreaBeforeBox = (PastBoxCrossSectionalArea[i] + PastBoxCrossSectionalArea[i + 1]) / 2.0;

            if (FlowBetweenBoxes[i] < 0.0)
         	    AlfaA = -1.0;
            else
         	    AlfaA = 1.0;

            Generic[i] =(PastGeneric[i] * PastMeanCrossSectionalAreaBeforeBox -
            	      		ATimeStep / BoxLength[i] *
                     		(
                     			MeanConcentrationAfterBox * DamDischarge -
                        		(
                              	(1 - AlfaA)/2 * PastGeneric[i] * FlowBetweenBoxes[i] +
                              	(1 + AlfaA)/2 * PastGeneric[i - 1] * FlowBetweenBoxes[i]
                           	)
                    			 ) -
                     		ATimeStep / (BoxLength[i] * BoxLength[i]) *
                     		(
                     			PastBoxCrossSectionalArea[i] *
                        		(
                        			HorizontalDiffCoef * (PastGeneric[i] - PastGeneric[i - 1])
                        		)
                           )
                         ) / MeanCrossSectionalAreaBeforeBox +
                        ATimeStep * (FlowDischarge[i] * GenericLoad[i] - FlowUptake[i] * PastGeneric[i]) /
                        (PastMeanCrossSectionalAreaBeforeBox * BoxLength[i]);
        }
    }
    delete [] PastGeneric;
}



void TOneDimensionalDITransport::Evaporate()
{
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    TEcoDynClass* MyAirTemperaturePointer = MyPEcoDynClass->GetAirTemperaturePointer();
    if ((MyWaterTemperaturePointer != NULL) &&
         (MyAirTemperaturePointer != NULL))
    {
       	double MyWaterTemperature, MyAirTemperature, MySVPAtWaterTemperature,
             MySVPAtAirTemperature, MyRelativeHumidity,
             MyWaterVapourMassTransferCoefficient, MyEvaporativeFlux;
        double GasConstant = 0.00008206; //m3 atm / (mol ºk)
        double WaterMolecularWeight = 18.0; //g/mol
        double PascalToAtm = 101300; //Pascal/Atm Ratio Pascal/Atm

   	    for (int i = 0; i < NumberOfBoxes; i++)
   	    {
   		    MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
         	                                    i,
            	                                 "Water temperature",
               	                              ObjectCode);

            MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyRelativeHumidity,
         	                                    i,
            	                                 "Relative humidity",
               	                              ObjectCode);
            MySVPAtWaterTemperature = GetSaturationVapourPressure(MyWaterTemperature);

            MyAirTemperaturePointer->Inquiry(GetEcoDynClassName(), MyAirTemperature,
         	                                  i,
                                              "Air temperature",
                                              ObjectCode);
            MySVPAtAirTemperature = GetSaturationVapourPressure(MyAirTemperature);

            MyWaterVapourMassTransferCoefficient = WaterVapourMassTransferCoefficient(i);

            MySVPAtWaterTemperature = MySVPAtWaterTemperature / PascalToAtm;
            MySVPAtAirTemperature = MySVPAtAirTemperature / PascalToAtm;

            if (MySVPAtWaterTemperature - MySVPAtAirTemperature * MyRelativeHumidity > 0.0)
         	    MyEvaporativeFlux = MyWaterVapourMassTransferCoefficient /
            	                 (GasConstant * (ABSOLUTEZERO + MyWaterTemperature)) *
               	              (MySVPAtWaterTemperature - MySVPAtAirTemperature * MyRelativeHumidity)*
                  	           WaterMolecularWeight;  //g/m2/s
            else
                MyEvaporativeFlux = 0.0;

            WaterDensity[i] = MyPEcoDynClass->GetDensity(Salt[i],MyWaterTemperature);
            MyEvaporativeFlux = MyEvaporativeFlux / CUBIC / WaterDensity[i] * ATimeStep; //m/s

            DynamicHeight[i] = DynamicHeight[i] - MyEvaporativeFlux;
            BoxDepth[i] = BoxDepth[i] - MyEvaporativeFlux;

            EvaporativeFlux[i] = MyEvaporativeFlux * BoxLength[i] * BoxWidth[i] /*/ ATimeStep*/; //m3/s
	    }
    }
}


double TOneDimensionalDITransport::GetSaturationVapourPressure(double ATemperature)
{
	double MyTemperature = ATemperature;
	return 612.0
			 *exp ( 17.5 * MyTemperature / ( 241 + MyTemperature ) );
}


double TOneDimensionalDITransport::WaterVapourMassTransferCoefficient(int ABoxNumber)
{
	int MyBoxNumber;
    double MyWindSpeed;

    MyBoxNumber = ABoxNumber;
    TEcoDynClass* MyWindPointer = MyPEcoDynClass->GetWindPointer();
    if (MyWindPointer != NULL)
    {
   	    MyWindPointer->Inquiry(GetEcoDynClassName(), MyWindSpeed,
                                 MyBoxNumber,
                                 "Wind speed",
                                 ObjectCode);

        return (0.1857 + 11.36 * MyWindSpeed) / 3600.0; //m/s    Manual do EXAMS
    }
    else
   	    return 0.1857 / 3600.0; //m/s
}



double TOneDimensionalDITransport::CoefRugosidade(int ABoxNumber)
{
    int MyBoxNumber;
    double HydraulicRadius, AverageDepth;

    MyBoxNumber = ABoxNumber;
    if ((ABoxNumber > 0) && (ABoxNumber < NumberOfBoxes - 1))
   	    AverageDepth = (BoxDepth[MyBoxNumber - 1] + BoxDepth[MyBoxNumber]) / 2.0;
    else
        AverageDepth = BoxDepth[MyBoxNumber];

    HydraulicRadius = BoxCrossSectionalArea[MyBoxNumber] /
									(BoxWidth[MyBoxNumber] + 2.0 * AverageDepth);
    if (HydraulicRadius < 0.5)
        HydraulicRadius = 0.5;

    return(Gravity * pow(CoefManning[ABoxNumber],2.0)) / pow(HydraulicRadius,0.333333);
}



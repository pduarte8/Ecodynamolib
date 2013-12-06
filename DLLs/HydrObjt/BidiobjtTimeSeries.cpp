// Bidimensional hydrodynamic model object CPP code file
// 	@ Antonio Pereira
// Rev. A 2004.03.03
/**
 * NPI work
 * 	15 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
    #include <sstream>
    	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "hydrobjt.h"
#include "iodll.h"

//
// Constructors invoked outside EcoDyn shell...
//
/*TBiDimensionalTimeSeries::TBiDimensionalTimeSeries(char* className,
        float timeStep, char* morphologyFilename)
            :TBiDimensionalRiaFormosa(className, timeStep, morphologyFilename)
{
	ReadTimeSeriesVariables();
	ReadTimeSeriesValues();
}


TBiDimensionalTimeSeries::TBiDimensionalTimeSeries(char* className, float timeStep,
                   int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
        :TBiDimensionalRiaFormosa(className, timeStep, nLines, nColumns,
                      aDepth, aLength, aWidth, aElevation,
                      aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
	ReadTimeSeriesVariables();
	ReadTimeSeriesValues();
}  */
// ...

TBiDimensionalTimeSeries::TBiDimensionalTimeSeries(TEcoDynClass* PEcoDynClass,
        		char* className, bool readValues, bool cyclic,
                int nrOfPeriods, SimulationPeriod* sPeriods)
        :TBiDimensionalRiaFormosa(PEcoDynClass, className)
{
    MyNumberOfBoxes = 0;
//    MyBoxNumber = new int[NumberOfBoxes];
    MeanValuesSamplesNumber = 0;
	ReadTimeSeriesVariables();
    cyclicRun = cyclic && (nrOfPeriods > 0);

    memcpy(periods, sPeriods, 12 * sizeof(SimulationPeriod));   // AP, 2005.11.30
    numberOfPeriods = nrOfPeriods;
    periodNumber = 0;
    periodCycle = 0;

    firstRead = true;
    if (readValues) {
        MyBoxNumber = new int[NumberOfBoxes];
        if (cyclicRun) {
            LoadPeriodInMemory(0);
        }
        else {
            firstIndexFileCycle = 0;
            while (!ReadTimeSeriesValues(firstIndexFileCycle)) {
                firstIndexFileCycle++;
            }
            firstMeanTime = timeSeries[0];
        }

        InitialBoxDepth = new double[NumberOfBoxes];
        InitialBoxElevation = new double[NumberOfBoxes];

        for (int i = 0; i < NumberOfBoxes; i++)
        {
            InitialBoxDepth[i] = BoxDepth[i];
            InitialBoxElevation[i] = DynamicHeight[i];
        }
    }

/*    InitialBoxDepth = new double[NumberOfBoxes];
    InitialBoxElevation = new double[NumberOfBoxes];

    for (int i = 0; i < NumberOfBoxes; i++)
    {
        InitialBoxDepth[i] = BoxDepth[i];
        InitialBoxElevation[i] = DynamicHeight[i];
    }*/
}

TBiDimensionalTimeSeries::~TBiDimensionalTimeSeries()
{
    freeMemory();
}

void TBiDimensionalTimeSeries::freeMemory()
{
    try {
        if (!firstRead)
        {
            if (NumberOfBoxes > 0) {
                delete [] MyBoxNumber;
                delete [] InitialBoxDepth;
                delete [] InitialBoxElevation;
                if (MeanValuesSamplesNumber > 0) {
                    delete [] mUVelocity;
                    delete [] mVVelocity;
                    delete [] mUFlow;
                    delete [] mVFlow;
                    delete [] timeSeries;
                }
            }
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
        cerr << "TBiDimensionalTimeSeries::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

// called directly from 'RunThread' ...
void TBiDimensionalTimeSeries::ResetValues()
{
    startTimeIndexFile = -1;
    startTime = MyPEcoDynClass->GetStartTime();
    ReadVelocitiesAndFlows();
}


void TBiDimensionalTimeSeries::ReadTimeSeriesVariables()
{
	int X, Y;

    GridLines = GetNumberOfLines();
    GridColumns = GetNumberOfColumns();
    if (NumberOfBoxes > 0)
	{
        memset(UVelocity, 0, sizeof(UVelocity));
        memset(VVelocity, 0, sizeof(VVelocity));
        memset(UVelocityPast, 0, sizeof(UVelocityPast));
        memset(VVelocityPast, 0, sizeof(VVelocityPast));
        memset(UFlowBetweenBoxes, 0, sizeof(UFlowBetweenBoxes));
        memset(VFlowBetweenBoxes, 0, sizeof(VFlowBetweenBoxes));
    }

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("TransportTimeSeries");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "TransportTimeSeries variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TBiDimensionalTimeSeries::ReadTimeSeriesVariables - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("TransportTimeSeries", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfTSVariables);

            // Create array for state variable names
            TSVariableNameArray = new VNA[NumberOfTSVariables];
            // read in the variable names
            for (int i = 0; i < NumberOfTSVariables; i++) {
                PReadWrite->ReadString(X + 2 + i, Y, TSVariableNameArray[i]);
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

// AP, 2007.07.09
bool TBiDimensionalTimeSeries::SaveVariables()
{
    /*
     * save variables for TBiDimensionalTimeSeries class
     */
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("TransportTimeSeries");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("TransportTimeSeries");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfTSVariables, 0);
    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfTSVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(TSVariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);

    /*
     * save variables for parent classes
     */
    TBiDimensionalDouro::SaveVariables();

    return true;
}

bool TBiDimensionalTimeSeries::ReadTimeSeriesValues(int fileIndex)
{
    int X,Y;
    int XTime, XStep, XBox, XUVel, XVVel, XUFlo, XVFlo;
    double Value;
    char text[512];

#ifdef __BORLANDC__
     AnsiString TimeSeriesFileName = MyPEcoDynClass->GetPathName()
        + AnsiString(PATH_SEPARATOR) + GLB_TIMESERIES + AnsiString(fileIndex) + GLB_XLSP;
#else  // __BORLANDC__
     stringstream TimeSeries;
     TimeSeries << MyPEcoDynClass->GetPathName() << PATH_SEPARATOR 
			<< GLB_TIMESERIES << fileIndex << GLB_XLSP;
	 string TimeSeriesFileName = TimeSeries.str();
#endif  // __BORLANDC__

	// Debug time series values file name...
    sprintf(text, "[TBiDimensionalTimeSeries] Opening file %s\n",
            TimeSeriesFileName.c_str());
    DebugMessage(text);

	// Read in the time series values...
    sprintf(text, TimeSeriesFileName.c_str());
    TReadWrite* PReadWrite = new TReadWrite(text);
	if (!PReadWrite->SetupFile(READFILE))
	{
		return false;
	}

    indexFile = fileIndex;

	int nSizeArray = PReadWrite->GetNumberOfRows() - 2; // Last line only have EOL
                                                   // and first is the header line

    // Verify if all mandatory columns exist...
    //
    if (!PReadWrite->FindString("Time(seconds)", XTime, Y))
        return false;
    if (!PReadWrite->FindString("TimeStep", XStep, Y))
        return false;
    if (!PReadWrite->FindString("BoxNumber", XBox, Y))
        return false;
    if (!PReadWrite->FindString("Mean U Velocity", XUVel, Y))
        XUVel = -1;
    if (!PReadWrite->FindString("Mean V Velocity", XVVel, Y))
        XVVel = -1;
    if (!PReadWrite->FindString("Mean U Flow", XUFlo, Y))
        XUFlo = -1;
    if (!PReadWrite->FindString("Mean V Flow", XVFlo, Y))
        XVFlo = -1;

    Y++;        // line above header - tipically the 2nd

    // verify how many samples the file has
    //
    int firstStep, lastStep;

    PReadWrite->ReadNumber(XStep, Y, firstStep);
    PReadWrite->ReadNumber(XStep, Y + nSizeArray - 1, lastStep);

    MeanValuesSamplesNumber = lastStep - firstStep + 1;

    // if it isn't the first read delete allocated memory
    //
    if (!firstRead)
    {
        delete [] mUVelocity;
        delete [] mVVelocity;
        delete [] mUFlow;
        delete [] mVFlow;
        delete [] timeSeries;
    }

    // allocate and clear memory for all samples
    //
    mUVelocity = new double[NumberOfBoxes * MeanValuesSamplesNumber];
    memset(mUVelocity, 0, sizeof(mUVelocity));
    mVVelocity = new double[NumberOfBoxes * MeanValuesSamplesNumber];
    memset(mVVelocity, 0, sizeof(mVVelocity));
    mUFlow = new double[NumberOfBoxes * MeanValuesSamplesNumber];
    memset(mUFlow, 0, sizeof(mUFlow));
    mVFlow = new double[NumberOfBoxes * MeanValuesSamplesNumber];
    memset(mVFlow, 0, sizeof(mVFlow));
    timeSeries = new long[MeanValuesSamplesNumber];
    memset(timeSeries, 0, sizeof(timeSeries));

    int index, stepNumber, nBox, loopNumber;
    index = -1;
    for (int i = 0; i < nSizeArray; i++)
    {
        PReadWrite->ReadNumber(XStep, Y + i, stepNumber);
//        loopNumber = (stepNumber - 1) % MeanValuesSamplesNumber;
        loopNumber = stepNumber - firstStep;
        PReadWrite->ReadNumber(XTime, Y + i, timeSeries[loopNumber]);
        PReadWrite->ReadNumber(XBox, Y + i, nBox);
        if ((loopNumber == 0) && firstRead) //(indexFile == 0))
        {
           MyNumberOfBoxes++;
           //index = nBox; 3/12
           //MyBoxNumber[i] = index; 3/12
           MyBoxNumber[i] = nBox;  //3/12
           //index = i;
        }
        //else    //3/12
           //index = nBox + loopNumber * MyNumberOfBoxes;  3/12
        //index = nBox + loopNumber * NumberOfBoxes;   Esta linha => que quando o primeiro nBox > 0, os ?ndices para loopNumber > 0 ficam errados
        //Debugger(index);
        index++;  //3/12
        if (XUVel != -1)
            PReadWrite->ReadNumber(XUVel, Y + i, mUVelocity[index]);
        if (XUFlo != -1)
            PReadWrite->ReadNumber(XUFlo, Y + i, mUFlow[index]);
        if (XVVel != -1) {
            PReadWrite->ReadNumber(XVVel, Y + i, Value);
            mVVelocity[index] = -Value;
        }
        if (XVFlo != -1) {
            PReadWrite->ReadNumber(XVFlo, Y + i, Value);
            mVFlow[index] = -Value;
        }
        //Debugger(mUFlow[index]);
    }
    PReadWrite->CloseFile();
    //...end reading time series values
    firstRead = false;

    steps = 0;
    for (int i = 0; i < MeanValuesSamplesNumber; i++, steps++) {
        if (timeSeries[i] == 0)
            break;
    }

    meanTimeDiff = timeSeries[1] - timeSeries[0];

    ASpatialStep = MyPEcoDynClass->GetBoxLength(1);
    DefaultTimeStep = MyPEcoDynClass->GetTimeStep();
    return true;

}

// AP, 2005.11.30
// Get index of the period that includes "simulationTime"
//  Returns -1 if simulationTime doesn't belong to any period
//
int TBiDimensionalTimeSeries::GetPeriodIndex(long simulationTime)
{
  int i;

    for (i = 0; i < numberOfPeriods; i++) {
        if ((simulationTime >= periods[i].start) && (simulationTime < periods[i].finish)) {
            return i;
        }
    }
    return -1;
}

// AP, 2005.12.06
// Load values related with "period" in memory.
//  Indicates in "indexFile" the first file available for that period.
//
void TBiDimensionalTimeSeries::LoadPeriodInMemory(int period)
{
    char text[512];

    periodNumber = period;
    periodCycle = periods[periodNumber].cycle;
    switch (periods[periodNumber].unit) {
        case 6:
            periodCycle = periodCycle * 365;
        case 5:
            periodCycle = periodCycle * 30;
        case 4:
            periodCycle = periodCycle * 24;
        case 3:
            periodCycle = periodCycle * 60;
        case 2:
            periodCycle = periodCycle * 60;
        default:
            break;
    }
    indexFile = periods[periodNumber].fileIndex;
    while (!ReadTimeSeriesValues(indexFile)) {
        indexFile++;
    }
    startTimeIndexFile = -1;
    firstMeanTime = timeSeries[0];
    cycle = 0;

    // Debug time series value...
    sprintf(text, "[TBiDimensionalTimeSeries] LoadPeriodInMemory - Period %d, IndexFile %d\n",
            periodNumber, indexFile);
    DebugMessage(text);
}

// AP, 2005.07.15
// Enable cyclic run
//
long TBiDimensionalTimeSeries::GetCyclicTime(long simulationTime)
{
    long cyclicTime;
    int t;
    char text[512];
    int nPeriod;

    if (!cyclicRun)
        return simulationTime;

    // cyclicRun
	// Debug time series value...
    sprintf(text, "[TBiDimensionalTimeSeries] GetCyclicTime - simulationTime %d, firstMeanTime %d\n",
            simulationTime, firstMeanTime);
    DebugMessage(text);

    nPeriod = GetPeriodIndex(simulationTime);
    if (nPeriod != periodNumber)
        LoadPeriodInMemory(nPeriod);

    if (startTimeIndexFile == -1) { // find file for Start Time
        while (timeSeries[steps - 1] < periods[nPeriod].start) {
            if (!ReadTimeSeriesValues(indexFile + 1))
                break;
        }
        startTimeIndexFile = indexFile;

        for (t = 0; t < steps; t++) {  // find registry index for first mean time
            if (timeSeries[t] >= periods[nPeriod].start)
                break;
        }
        firstMeanTime = timeSeries[t == 0 ? t : t - 1];

        // Debug time series value...
        sprintf(text, "[TBiDimensionalTimeSeries] GetCyclicTime - startTime %d, firstMeanTime %d, startTimeIndexFile %d\n",
                periods[nPeriod].start, firstMeanTime, startTimeIndexFile);
        DebugMessage(text);
    }

    t = (simulationTime - periods[nPeriod].start) / periodCycle;
    if (t > cycle) {
        cycle = t;
        ReadTimeSeriesValues(startTimeIndexFile);
        CorrectElevation();
    }

    cyclicTime = simulationTime -(periodCycle * t);

	// Debug time series value...
    sprintf(text, "[TBiDimensionalTimeSeries] GetCyclicTime - cyclicTime %d, t %d, cycle %d\n",
            cyclicTime, t, cycle);
    DebugMessage(text);

    return cyclicTime;
}

int TBiDimensionalTimeSeries::GetStep(long simulationTime, int stepI, int stepF)
{
    long serialTime = GetCyclicTime(simulationTime);

	if (serialTime <= timeSeries[stepI])
    	return stepI;
    else if (serialTime >= timeSeries[stepF])
    {
        if (serialTime < (timeSeries[stepF] + meanTimeDiff))
            return stepF;
        else if (ReadTimeSeriesValues(indexFile + 1))
            return GetStep(serialTime, 0, steps - 1);
        else
    	    return stepF;
    }

    if ((stepF - stepI) < 2)
       return stepI;

    int i = (stepF + stepI) / 2;
    if (serialTime <= timeSeries[i])
    	return GetStep(serialTime, stepI, i);
    else
    	return GetStep(serialTime, i, stepF);
}

void TBiDimensionalTimeSeries::Go()
{
    TideForcing();
    myTimeSeriesStep = GetStep(MyPEcoDynClass->GetTimeUTC(), 0, steps - 1);

    // AP, 2005.08.30
    DoContinuity();
}

void TBiDimensionalTimeSeries::TimeStepCheck()
{
    //Debugger(ATimeStep);
    double MinTimeStep, CourantCondition;
    double CourantRestriction = 0.1;
    MinTimeStep = DefaultTimeStep;
    for (int i = 0; i < GridLines; i++)
    {
        for (int j = 0; j < GridColumns; j++)
        {
            int index1 = i * GridColumns + j;
            int index2 = i * (GridColumns + 1) + j;
            int index3 = (i + 1) * GridColumns + j;
            int index4 = i * (GridColumns + 1) + j + 1;

            CourantCondition = UFlowBetweenBoxes[index2] * MinTimeStep / (BoxDepth[index1] * ASpatialStep * ASpatialStep);
            if (CourantCondition > CourantRestriction)
               MinTimeStep = MIN(MinTimeStep,CourantRestriction * (BoxDepth[index1] * ASpatialStep * ASpatialStep) / UFlowBetweenBoxes[index2]);

            CourantCondition = UFlowBetweenBoxes[index4] * MinTimeStep / (BoxDepth[index1] * ASpatialStep * ASpatialStep);
            if (CourantCondition > CourantRestriction)
               MinTimeStep = MIN(MinTimeStep,CourantRestriction * (BoxDepth[index1] * ASpatialStep * ASpatialStep) / UFlowBetweenBoxes[index4]);

            CourantCondition = VFlowBetweenBoxes[index1] * MinTimeStep / (BoxDepth[index1] * ASpatialStep * ASpatialStep);
            if (CourantCondition > CourantRestriction)
               MinTimeStep = MIN(MinTimeStep,CourantRestriction * (BoxDepth[index1] * ASpatialStep * ASpatialStep) / VFlowBetweenBoxes[index1]);

            CourantCondition = VFlowBetweenBoxes[index3] * MinTimeStep / (BoxDepth[index1] * ASpatialStep * ASpatialStep);
            if (CourantCondition > CourantRestriction)
               MinTimeStep = MIN(MinTimeStep,CourantRestriction * (BoxDepth[index1] * ASpatialStep * ASpatialStep) / VFlowBetweenBoxes[index3]);
        }
    }
    MyPEcoDynClass->SetTimeStep(MinTimeStep);
    //MyPEcoDynClass->SetTimeStep(1.1);
    ATimeStep = 2.0 * MyPEcoDynClass->GetTimeStep();
    //Debugger(ATimeStep);
}

void TBiDimensionalTimeSeries::Inquiry(char* srcName, double &Value,
										   int BoxNumber,
										   char* ParameterName,
										   int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
	int myBoxNumber = BoxNumber;
    int LineIndex = GetLineIndex(BoxNumber);
    int ColumnIndex= GetColumnIndex(BoxNumber);
    int indexU = LineIndex * (GridColumns + 1) + ColumnIndex;

	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Box depth") == 0)
		Value = BoxDepth[myBoxNumber];
	else if (strcmp(MyParameter, "Salinity") == 0)
		Value = Salt[myBoxNumber];
    else if (strcmp(MyParameter,"Water density") == 0)
       	Value = WaterDensity[myBoxNumber];
	else if (strcmp(MyParameter, "Dynamic height") == 0)
		Value = DynamicHeight[myBoxNumber];
	else if (strcmp(MyParameter, "U Velocity") == 0)
        //Value = UVelocity[myBoxNumber];
        Value = UVelocity[indexU];
    else if (strcmp(MyParameter, "V Velocity") == 0)
        Value = -VVelocity[myBoxNumber];
        //Value = getVVelocity(myBoxNumber);

	else if (strcmp(MyParameter, "U Flow") == 0)
        //Value = mUFlow[index];
        //Value = getUFlow(myBoxNumber);
        Value = UFlowBetweenBoxes[indexU];
    else if (strcmp(MyParameter, "V Flow") == 0)
        //Value = -mVFlow[index];
        Value = -VFlowBetweenBoxes[myBoxNumber];
    else if (strcmp(MyParameter, "Drag coefficient") == 0)
	    Value = CoefRugosidade(myBoxNumber);
//	    Value = mDragCoefficient[index];

    else if (strcmp(MyParameter,"Columns") == 0)
       	Value = float(ColumnIndex + 1.0); //A adi??o de 1 ? para ter o n?mero de colunas 1-based
    else if (strcmp(MyParameter,"Lines") == 0)
       	Value = float(GridLines - LineIndex);//A subtrac??o entre () destina-se a inverter o referencial vertical
                                              //para compatibilidade com o Noesys
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfTSVariables; j++)
        {
            if (strcmp(MyParameter, TSVariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfTSVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDyn Alert - Inquiry 2",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TBiDimensionalTimeSeries::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
	}
    LogMessage("Inquiry", srcName, MyParameter, Value, BoxNumber);
}


// AP, 2007.05.28
bool TBiDimensionalTimeSeries::SetVariableValue(char* srcName,
                             double Value,
                             int BoxNumber,
                             char* VariableName)
{
    bool rc = true;
    int LineIndex = GetLineIndex(BoxNumber);
    int ColumnIndex= GetColumnIndex(BoxNumber);
    int indexU = LineIndex * (GridColumns + 1) + ColumnIndex;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Box depth") == 0)
		BoxDepth[BoxNumber] = Value;
	else if (strcmp(VariableName, "Salinity") == 0)
		Salt[BoxNumber] = Value;
    else if (strcmp(VariableName,"Water density") == 0)
       	WaterDensity[BoxNumber] = Value;
	else if (strcmp(VariableName, "Dynamic height") == 0)
		DynamicHeight[BoxNumber] = Value;
	else if (strcmp(VariableName, "U Velocity") == 0)
        UVelocity[indexU] = Value;
    else if (strcmp(VariableName, "V Velocity") == 0)
        VVelocity[BoxNumber] = -Value;
	else if (strcmp(VariableName, "U Flow") == 0)
        UFlowBetweenBoxes[indexU] = Value;
    else if (strcmp(VariableName, "V Flow") == 0)
        VFlowBetweenBoxes[BoxNumber] = -Value;
    else
        rc = false;

    return rc;
}

void TBiDimensionalTimeSeries::ReadVelocitiesAndFlows()
{
    int MyLineNumber, MyColumnNumber;
    myTimeSeriesStep = GetStep(MyPEcoDynClass->GetTimeUTC(), 0, steps - 1);
    for (int i = 0; i < MyNumberOfBoxes; i++)
    {
        int index = i + myTimeSeriesStep * MyNumberOfBoxes;
        MyLineNumber = MyPEcoDynClass->GetLineIndex(MyBoxNumber[i]);
        MyColumnNumber = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[i]);
        UVelocity[MyLineNumber * (GridColumns + 1) + MyColumnNumber] = mUVelocity[index];
        VVelocity[MyLineNumber * GridColumns + MyColumnNumber] = mVVelocity[index];
        UFlowBetweenBoxes[MyLineNumber * (GridColumns + 1) + MyColumnNumber] = mUFlow[index];
        VFlowBetweenBoxes[MyLineNumber * GridColumns + MyColumnNumber] = mVFlow[index];
    }
}

void TBiDimensionalTimeSeries::RiverForcing()
{

}


void TBiDimensionalTimeSeries::Continuity()
{
    double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    EnableContinuity = false;       // AP, 2005.09.16 - moved to here

//    Generic = Salt;     // AP, 2005.08.30

    for (m = 0; m < MyNumberOfBoxes; m++)
    {
        i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
        j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
        if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
            )
        {
            index = i * GridColumns + j;
      	    if (
                //(BoxArray[index].Dummybool2) &&
                (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                (BoxArray[index].Dummybool1) /*&&
                (BoxDepth[index] > 0.0) */
                )
            {
                MyWestUFlow = WesternComponentOfContinuity(i,j);
                MyEastUFlow = EasternComponentOfContinuity(i,j);
                MyNorthVFlow = NorthernComponentOfContinuity(i,j);
                MySouthVFlow = SouthernComponentOfContinuity(i,j);

                if (CheckTemporalResolutionForAdvection(index, BoxDepth[index], MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow))
                //if (
                //     (CheckTemporalResolutionForAdvection(index, BoxDepth[index], MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow))/* ||
                     //(BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 1) ||
                     //(BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 1) ||
                     //(BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 1) ||
                     //(BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 1)
                //   )
                {
                        DynamicHeight[index] = PastHeight[index] +
                                   (
                                        MyWestUFlow -
                                        MyEastUFlow -
                                        MyNorthVFlow +
                                        MySouthVFlow
                                    ) / (ASpatialStep * ASpatialStep);
                        if (MyLoadRecord != NULL)
                        {
                                for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                                {
                                        if (
                                                (i == MyLoadRecord[k].LineCoordinate) &&
                                                (j == MyLoadRecord[k].ColumnCoordinate)
                                           )
                                        {
                                                ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                                MyPEcoDynClass->GetNumberOfDaysForLoads());
                                                DynamicHeight[index] = DynamicHeight[index] + ATimeStep / 2.0 *
                                                         MyLoadRecord[k].LoadFlow[ADay - 1] /
                                                         (
                                                            (ASpatialStep * ASpatialStep)
                                                         );
                                                break;
                                        }
                                }
                        }
                }
                else
                {
                   BoxArray[index].Dummybool2 = false;



                } 

            }
      	}
    }
    AdvectDiffuse(Salt, SaltLoad);

//    Salt = Generic;     // AP, 2005.08.30
//    EnableContinuity = false;
}


bool TBiDimensionalTimeSeries::CheckTemporalResolutionForAdvection(int ABoxNumber, double ABoxDepth,
                                                               double MyWestUFlow, double MyEastUFlow,
                                                               double MyNorthVFlow, double MySouthVFlow)
{
/*        double Input, Output, MyBoxVolume;
        Input = 0; Output = 0;
        if (MyWestUFlow >= 0.0)
                Input = Input + MyWestUFlow;
        else
                Output = Output + abs(MyWestUFlow);
        if (MySouthVFlow >= 0.0)
                Input = Input + MySouthVFlow;
        else
                Output = Output + abs(MySouthVFlow);
        if (MyEastUFlow < 0.0)
                Input = Input + abs(MyEastUFlow);
        else
                Output = Output + MyEastUFlow;
        if (MyNorthVFlow < 0.0)
                Input = Input + abs(MyNorthVFlow);
        else
                Output = Output + MyNorthVFlow;
        MyBoxVolume = ABoxDepth * ASpatialStep * ASpatialStep;

        //if ((Input * ATimeStep / 2.0 >= 1.0 * MyBoxVolume) || (Output * ATimeStep / 2.0 >= 1.0 * MyBoxVolume))
        if ((Output - Input) * ATimeStep / 2.0 >= 0.25 * MyBoxVolume)
                return false;
        else*/
                return true;

}

void TBiDimensionalTimeSeries::AdvectDiffuse(double* Generic, double* GenericLoad)
{
	double /*MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,*/MyWestUDischarge, MyEastUDischarge, MyNorthVDischarge, MySouthVDischarge,
   		 MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double* PastGeneric = new double[NumberOfBoxes];

   for (i = 0; i < NumberOfBoxes; i++)
   	PastGeneric[i] = Generic[i];

   /*for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)  */
   /*for (int i = MyPEcoDynClass->GetSubDomain()->FirstLine; i <= MyPEcoDynClass->GetSubDomain()->LastLine; i++)
   {
      for (int j = MyPEcoDynClass->GetSubDomain()->IColumn[i]; j <= MyPEcoDynClass->GetSubDomain()->FColumn[i]; j++)
      {
         if ((i > MyPEcoDynClass->GetSubDomain()->ILine[j]) && (i < MyPEcoDynClass->GetSubDomain()->FLine[j]))
         { */
         for (m = 0; m < MyNumberOfBoxes; m++)
         {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
                )
            {
            index = i * GridColumns + j;

            if (
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1)
                )
            {
         	    //Advective fluxes

                /*MyWestUFlow= UFlowBetweenBoxes[i * (GridColumns + 1) + j];

                MyEastUFlow = UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1];

                MyNorthVFlow = VFlowBetweenBoxes[(i + 1) * GridColumns + j]);

                MySouthVFlow = VFlowBetweenBoxes[i * GridColumns + j];  */
                /*if (BoxArray[index].Dummybool2)
                {   */
                   MyWestUDischarge = WesternComponentOfAdvectionDiffusion(i,j,PastGeneric);
                   MyEastUDischarge = EasternComponentOfAdvectionDiffusion(i,j,PastGeneric);
                   MyNorthVDischarge = NorthernComponentOfAdvectionDiffusion(i,j,PastGeneric);
                   MySouthVDischarge = SouthernComponentOfAdvectionDiffusion(i,j,PastGeneric);
                /*}
                else
                {
                   MyWestUDischarge = 0.0;
                   MyEastUDischarge = 0.0;
                   MyNorthVDischarge = 0.0;
                   MySouthVDischarge = 0.0;
                }  */
                //Diffusive fluxes
                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGeneric);

                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGeneric);

                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGeneric);

                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGeneric);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGeneric[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                /*if ((MyPEcoDynClass->GetCurrTime() > 7.166) && (index == 14547))
                {
                   Debugger(PastGeneric[index]); Debugger(PastGeneric[i * GridColumns + j - 1]); Debugger(PastGeneric[(i+1) * GridColumns + j]);
                   Debugger(BoxDepth[index]);
                   Debugger(PastHeight[index]);
                   Debugger(DynamicHeight[index]);
                   Debugger(MyWestUDischarge); Debugger(MyEastUDischarge); Debugger(MyNorthVDischarge); Debugger(MySouthVDischarge);
                   Debugger(MyWestDiffusion); Debugger(MyEastDiffusion); Debugger(MyNorthDiffusion); Debugger(MySouthDiffusion);
                   Debugger(ResultantDiffusion);
                } */
                Generic[index ] = PastGeneric[index]  *
                    BoxDepth[index] /
                    (BoxDepth[index] + DynamicHeight[index] - PastHeight[index])
                    +
                    (
                        MyWestUDischarge -
                        MyEastUDischarge -
                        MyNorthVDischarge +
                        MySouthVDischarge
                    ) /
                    (ASpatialStep * ASpatialStep *
                     (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]))
                    +
                    /*(
                        MyWestDiffusion +
                        MyEastDiffusion +
                        MyNorthDiffusion +
                        MySouthDiffusion
                     )*/ResultantDiffusion /
                     (2.0 * ASpatialStep * ASpatialStep *
                     (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]));

                /*if ((MyPEcoDynClass->GetCurrTime() >= 7.166) && (index == 14547))
                {
                   Debugger(MyWestUFlow); Debugger(MyEastUFlow); Debugger(MyNorthVFlow); Debugger(MySouthVFlow);
                   Debugger(MyWestDiffusion); Debugger(MyEastDiffusion); Debugger(MyNorthDiffusion); Debugger(MySouthDiffusion);
                   Debugger(BoxDepth[index]); Debugger(PastHeight[index]); Debugger(DynamicHeight[index]);
                   Debugger(PastGeneric[index]); Debugger(Generic[index]);
                }*/



                /*if ((Generic[index] < 0.0) && (Generic[index] > -0.000001))
                    Generic[index] = 0.0;
                if (Generic[index] < -0.0000000001)
                {
                    Debugger(i); Debugger(j); Debugger(index);
                    Debugger(PastGeneric[index]); Debugger(Generic[index]);
                    Debugger(PastGeneric[i * GridColumns + j - 1]); Debugger(PastGeneric[i * GridColumns + j + 1]);
                    Debugger(PastGeneric[(i - 1) * GridColumns + j]); Debugger(PastGeneric[(i + 1) * GridColumns + j]);
                    Debugger(MyWestUFlow); Debugger(MyEastUFlow); Debugger(MyNorthVFlow); Debugger(MySouthVFlow);
                    Debugger(MyWestDiffusion); Debugger(MyEastDiffusion); Debugger(MyNorthDiffusion); Debugger(MySouthDiffusion);
                    Debugger(DynamicHeight[index]); Debugger(PastHeight[index]);
                    Debugger(ASpatialStep);
                    Debugger(BoxDepth[index]);
                    Debugger(BoxDepth[i * GridColumns + j - 1]); Debugger(BoxDepth[i * GridColumns + j + 1]);
                    Debugger(BoxDepth[(i + 1) * GridColumns + j]); Debugger(BoxDepth[(i - 1) * GridColumns + j]);
                }   */
                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            Generic[index ] = Generic[index ] + ATimeStep / 2.0 * GenericLoad[index] *
                                              MyLoadRecord[k].LoadFlow[ADay - 1] /
                                              (
                                               (BoxDepth[index] + DynamicHeight[index] - PastHeight[index]) *
                                               (ASpatialStep * ASpatialStep)
                                              );
                            break;
                        }
                    }
                }
      	    }
            }
         }
//      }  //j
//   }  //i
    delete [] PastGeneric;
}


bool TBiDimensionalTimeSeries::CheckTemporalResolutionForMassDiffusion(int ABoxNumber, double ABoxDepth,
                                                                       double MyWestDiffusion, double MyEastDiffusion,
                                                                       double MyNorthDiffusion, double MySouthDiffusion,double AGeneric)
{
        double Input, Output, MyBoxVolume;
        Input = 0; Output = 0;
        if (MyWestDiffusion >= 0.0)
                Input = Input + MyWestDiffusion;
        else
                Output = Output + fabs(MyWestDiffusion);
        if (MySouthDiffusion >= 0.0)
                Input = Input + MySouthDiffusion;
        else
                Output = Output + fabs(MySouthDiffusion);
        if (MyEastDiffusion >= 0.0)
                Input = Input + MyEastDiffusion;
        else
                Output = Output + fabs(MyEastDiffusion);
        if (MyNorthDiffusion >= 0.0)
                Input = Input + MyNorthDiffusion;
        else
                Output = Output + fabs(MyNorthDiffusion);
        MyBoxVolume = ABoxDepth * ASpatialStep * ASpatialStep;

        //if ((Input * ATimeStep / 2.0 >= 1.0 * MyBoxVolume) || (Output * ATimeStep / 2.0 >= 1.0 * MyBoxVolume))
        if (fabs(Output - Input) * ATimeStep / 2.0 >= 0.1 * MyBoxVolume * AGeneric)
                return false;
        else
                return true;

}

double TBiDimensionalTimeSeries::WesternComponentOfContinuity(int i, int j)
{
  int index = i * (GridColumns + 1) + j;
  //Western flow
  //if ((j == 0) || ((j > 0) && (! BoxArray[index - 1].Dummybool2)))
    return (ATimeStep / 2.0 * UFlowBetweenBoxes[index]);
  //else return 0.0;
}

double TBiDimensionalTimeSeries::EasternComponentOfContinuity(int i, int j)
{
  int index = i * (GridColumns + 1) + j + 1;
  //Eastern flow
  //if ((j == GridColumns - 1)  || ((j < GridColumns - 1) && (! BoxArray[index + 1].Dummybool2)))
     return (ATimeStep / 2.0 * UFlowBetweenBoxes[index]);
  //else return 0.0;
}

double TBiDimensionalTimeSeries::NorthernComponentOfContinuity(int i, int j)
{
  int index = (i + 1) * GridColumns + j;
  //Northern flow
  //if ((i == GridLines - 1)  || ((i < GridLines - 1) && (! BoxArray[(i + 1) * GridColumns + j].Dummybool2)))
     return (ATimeStep / 2.0 * VFlowBetweenBoxes[index]);
  //else return 0.0;
}

double TBiDimensionalTimeSeries::SouthernComponentOfContinuity(int i, int j)
{
  int index = i * GridColumns + j;
  //Southern flow
  //if ((i == 0) ||((i > 0) && (! BoxArray[(i - 1) * GridColumns + j].Dummybool2)))
     return (ATimeStep / 2.0 * VFlowBetweenBoxes[index]);
  //else return 0.0;
}



double TBiDimensionalTimeSeries::WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Western flow
    if /*(*/(j == 0) /*|| ((j > 0) && (!BoxArray[index - 1].Dummybool2)))*/
   	{
   		return (
                  ATimeStep / 2.0 *
         	      UFlowBetweenBoxes[i * (GridColumns + 1) + j] *
                  PastGeneric[index]
                );
   	}
   	else
   	{
      	if (UFlowBetweenBoxes[i * (GridColumns + 1) + j] > 0.0)
        {
            //if (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
            //if (BoxArray[index - 1].Dummybool2) // Pedro - 4 de Junho de 2005
   			    return  (
                       ATimeStep / 2.0 *
         	           UFlowBetweenBoxes[i * (GridColumns + 1) + j] *
                       PastGeneric[index - 1]
                    );
             //else return 0.0;
         }
         else
           return (
                     ATimeStep / 2.0 *
         	         UFlowBetweenBoxes[i * (GridColumns + 1) + j] *
                     PastGeneric[index]
                   );
   	}
}

double TBiDimensionalTimeSeries::EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Eastern flow
   	if /*(*/(j == GridColumns - 1)/* ||
   		((j < GridColumns - 1) &&
         (! BoxArray[index + 1].Dummybool2))
   		)  */
   	{
   		return (ATimeStep / 2.0 *
         	    UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] *
                PastGeneric[index]);
   	}
   	else
   	{
      	if (UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] > 0.0)
   			return (ATimeStep / 2.0 *
         	        UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] *
                     PastGeneric[index]);
        else
        //if  (BoxDepth[index + 1] > CriticalDepthForLandBoundary)
        //if (BoxArray[index + 1].Dummybool2) // Pedro - 4 de Junho de 2005
         	return (ATimeStep / 2.0 *
         	        UFlowBetweenBoxes[i * (GridColumns + 1) + j + 1] *
                     PastGeneric[index + 1]);
        //else return 0.0;
   	}
}

double TBiDimensionalTimeSeries::NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Northern flow
   	if /*(*/(i == GridLines - 1) /* ||
   		((i < GridLines - 1) &&
         (! BoxArray[(i + 1) * GridColumns + j].Dummybool2))
      	)  */
   	{
   		return (ATimeStep / 2.0 *
                VFlowBetweenBoxes[(i + 1) * GridColumns + j] *
                PastGeneric[index]);
   	}
   	else
   	{
      	if (VFlowBetweenBoxes[(i + 1) * GridColumns + j] > 0.0)
            return (ATimeStep / 2.0 *
                    VFlowBetweenBoxes[(i + 1) * GridColumns + j] *
                    PastGeneric[index]);
         else
         //if (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary)
         //if (BoxArray[(i + 1) * GridColumns + j].Dummybool2) // Pedro - 4 de Junho de 2005
         	return (ATimeStep / 2.0 *
                    VFlowBetweenBoxes[(i + 1) * GridColumns + j] *
                    PastGeneric[(i + 1) * GridColumns + j]);
         //else return 0.0;
   	}
}

double TBiDimensionalTimeSeries::SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric)
{
  int index = i * GridColumns + j;

	//Southern flow
    if /*(*/(i == 0) /*||
      	((i > 0) &&
         //(BoxDepth[(i - 1) * GridColumns + j ] < CriticalDepthForLandBoundary))
         (! BoxArray[(i - 1) * GridColumns + j].Dummybool2))
      	)              */
   	{
   		return (ATimeStep / 2.0 *
                 VFlowBetweenBoxes[index ] *
                 PastGeneric[index ]);

   	}
   	else
   	{
      	if (VFlowBetweenBoxes[index] > 0.0)
        {
            //if (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary)
            //if (BoxArray[(i - 1) * GridColumns + j].Dummybool2) // Pedro - 4 de Junho de 2005
   			    return (ATimeStep / 2.0 *
                    VFlowBetweenBoxes[index ] *
                    PastGeneric[(i - 1) * GridColumns + j]);
            //else return 0.0;
        }
        else
         	return (ATimeStep / 2.0 *
                    VFlowBetweenBoxes[index ] *
                    PastGeneric[i * GridColumns + j]);
   	}
}

void TBiDimensionalTimeSeries::Reinitialize()
{
    double MyWaterTemperature;
    double const MinCritDepth = 0.01;  //0.00001 m
    int i, j, index;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    EnableReinitializeFromGo = false;   // AP, 2005.09.16 - moved to here
    EnableReinitializeFromIntegrate = false;

    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

    for (i = pSubDomain->FirstLine;
         i <= pSubDomain->LastLine; i++)
    {
       for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i]; j++)
       {
          index = i * GridColumns + j;
      	  atemporary[index] = 0.0; btemporary[index] = 0.0;
          ctemporary[index] = 0.0; rtemporary[index] = 0.0;
          //if (BoxArray[index].Dummybool2)
          //{
             BoxDepth[index] = BoxDepth[index] +
                              (DynamicHeight[index] - PastHeight[index]);
             if (BoxDepth[index] <= CriticalDepthForLandBoundary)
             {
                 BoxArray[index].Dummybool2 = false;
             }
          //}
      }
   }
   for (i = pSubDomain->FirstLine;
        i <= pSubDomain->LastLine + 1; i++)
   {
      for (j = pSubDomain->IColumn[i];
         j <= pSubDomain->FColumn[i] + 1; j++)
      {
         index = i * GridColumns + j;
         if (index < NumberOfBoxes)
         {
             PastHeight[index] = DynamicHeight[index];
      	     if (MyWaterTemperaturePointer != NULL)
             {
                MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature, index,"Water temperature",ObjectCode);
      	        WaterDensity[index] = MyPEcoDynClass->GetDensity(Salt[index],MyWaterTemperature);
      	     }
      	     else
             WaterDensity[index] = 1026.75; // seawater at 35psu and 10oC;

             if   (
                  (! BoxArray[index].Dummybool2) && (BoxDepth[index] >= MinCritDepth) &&  //Tive que mudar este valor de zero para 0.00001 pois havia casos
                  (                                                                       // em que a BoxDepth estava tão próxima de zero que ocorria um floating point error
                  	(                                                                     //no AdvectDiffuse no cálculo da concentração, quando divide pela soma
                     	(j < GridColumns - 1) &&                                          // da BoxDepth com a diferença entre a elevação presente e a elevação passada
                        //(DynamicHeight[index + 1] > BoxDepth[index]) &&
                        (DynamicHeight[index + 1] > DynamicHeight[index]) &&
                        (BoxDepth[index + 1] > CriticalDepthForLandBoundary)
                     ) ||
                     (
                     	(j > 0) &&
                        //(DynamicHeight[index - 1] > BoxDepth[index]) &&
                        (DynamicHeight[index - 1] > DynamicHeight[index]) &&
                        (BoxDepth[index - 1] > CriticalDepthForLandBoundary)
                     ) ||
                  	(
                  		(i < GridLines - 1)  &&
                        //(DynamicHeight[(i + 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i + 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i + 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)    ||
                  	(
                  		(i > 0) &&
                        //(DynamicHeight[(i - 1) * GridColumns + j] > BoxDepth[index]) &&
                        (DynamicHeight[(i - 1) * GridColumns + j] > DynamicHeight[index]) &&
                        (BoxDepth[(i - 1) * GridColumns + j] > CriticalDepthForLandBoundary)
                  	)
               	   )
            	)
               BoxArray[index].Dummybool2 = true;

             if (
               (!BoxArray[index].Dummybool2) ||
               (
                  (BoxArray[index].Dummybool2) &&
                  (BoxDepth[index] <= CriticalDepthForLandBoundary) &&
                  (DynamicHeight[index] < PastHeight[index ])
               )
             )
             {
                UVelocity[i * (GridColumns + 1) + j] = 0.0;
                VVelocity[index] = 0.0;
                UVelocity[i * (GridColumns + 1) + j + 1] = 0.0;
                VVelocity[(i + 1) * GridColumns + j] = 0.0;
                //DynamicHeight[index] = BoxDepth[index ];
             }
          }
          if (index < NumberOfBoxes + GridLines)
             UVelocityPast[i * (GridColumns + 1) + j] = UVelocity[i * (GridColumns + 1) + j];

          if (index < NumberOfBoxes + GridColumns)
             VVelocityPast[index] = VVelocity[index];
       }
    }
   RiverForcing();

   if (SemiTimeStep == 1) SemiTimeStep = 2;
   else if (SemiTimeStep == 2) SemiTimeStep = 1;

   ReadVelocitiesAndFlows();
}


// AP, 2006.01.17
void TBiDimensionalTimeSeries::Go(double* AGeneric,double* ASecondGeneric,
                                  double ARiver, double AOcean)

{
	Generic = AGeneric;
	AdvectDiffuse(AGeneric, ASecondGeneric);
}

// AP, 2005.05.17
void TBiDimensionalTimeSeries::Go(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad,
                        double ARiver, double AOcean)
{
	AdvectDiffuse(A, ALoad, B, BLoad, C, CLoad, D, DLoad, E, ELoad);
};
// Pedro Duarte, 2005.07.07
void TBiDimensionalTimeSeries::Go(double *A, double *ALoad, double *ARiverLoad,
                                  double *B, double *BLoad, double *BRiverLoad,
                                  double *C, double *CLoad, double *CRiverLoad,
                                  double *D, double *DLoad, double *DRiverLoad,
                                  double *E, double *ELoad, double *ERiverLoad,
                                  double ARiver, double AOcean)
{
	AdvectDiffuse(A, ALoad, ARiverLoad, B, BLoad, BRiverLoad, C, CLoad, CRiverLoad, D, DLoad, DRiverLoad, E, ELoad, ERiverLoad);
};
// Pedro Duarte, 2005.06.20
void TBiDimensionalTimeSeries::Go(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad,
                        double *F, double *FLoad,
                        double *G, double *GLoad,
                        double *H, double *HLoad,
                        double ARiver, double AOcean)
{
	AdvectDiffuse(A, ALoad, B, BLoad, C, CLoad, D, DLoad, E, ELoad, F, FLoad, G, GLoad, H, HLoad);
};


void TBiDimensionalTimeSeries::AdvectDiffuse(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad)
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
        MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    double H1, Q1;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    double *PastGenericA, *PastGenericB, *PastGenericC, *PastGenericD, *PastGenericE;

    PastGenericA = new double[NumberOfBoxes];
    PastGenericB = new double[NumberOfBoxes];
    PastGenericC = new double[NumberOfBoxes];
    PastGenericD = new double[NumberOfBoxes];
    PastGenericE = new double[NumberOfBoxes];

    for (i = 0; i < NumberOfBoxes; i++)
    {
        PastGenericA[i] = A[i];
        PastGenericB[i] = B[i];
        PastGenericC[i] = C[i];
        PastGenericD[i] = D[i];
        PastGenericE[i] = E[i];
    }

   /*for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)  */
   /*for (int i = MyPEcoDynClass->GetSubDomain()->FirstLine; i <= MyPEcoDynClass->GetSubDomain()->LastLine; i++)
   {
      for (int j = MyPEcoDynClass->GetSubDomain()->IColumn[i]; j <= MyPEcoDynClass->GetSubDomain()->FColumn[i]; j++)
      {
         if ((i > MyPEcoDynClass->GetSubDomain()->ILine[j]) && (i < MyPEcoDynClass->GetSubDomain()->FLine[j]))
         { */
         for (m = 0; m < MyNumberOfBoxes; m++)
         {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
                )
            {
            index = i * GridColumns + j;

            if (
        			(BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1)
                )
            {

                // AP, 2005.05.19 ...
                H1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
                Q1 = ASpatialStep * ASpatialStep * H1;

         	    //Advective fluxes and Diffusive fluxes

                // AP, 2005.05.19 ...
         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericA);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericA);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericA);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericA);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericA[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;


                A[index] = PastGenericA[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);
                if ((A[index] < 0.0) && (A[index] > -0.000001))
                    A[index] = 0.0;

         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericB);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericB);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericB);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericB);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericB[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                B[index] = PastGenericB[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((B[index] < 0.0) && (B[index] > -0.000001))
                    B[index] = 0.0;

         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericC);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericC);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericC);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericC);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericC[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                C[index] = PastGenericC[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((C[index] < 0.0) && (C[index] > -0.000001))
                    C[index] = 0.0;

         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericD);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericD);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericD);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericD);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericD[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                D[index] = PastGenericD[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((D[index] < 0.0) && (D[index] > -0.000001))
                    D[index] = 0.0;

         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericE);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericE);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericE);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericE);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericE[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                E[index] = PastGenericE[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((E[index] < 0.0) && (E[index] > -0.000001))
                    E[index] = 0.0;

                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            // AP, 2005.05.19 ...
                            A[index] = A[index] + ATimeStep / 2.0 * ALoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            B[index] = B[index] + ATimeStep / 2.0 * BLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            C[index] = C[index] + ATimeStep / 2.0 * CLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            D[index] = D[index] + ATimeStep / 2.0 * DLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            E[index] = E[index] + ATimeStep / 2.0 * ELoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            break;
                        }
                    }
                }
      	    }
            }
         }
//      }  //j
//   }  //i
    delete [] PastGenericA;
    delete [] PastGenericB;
    delete [] PastGenericC;
    delete [] PastGenericD;
    delete [] PastGenericE;
}

void TBiDimensionalTimeSeries::AdvectDiffuse(double *A, double *ALoad, double *ARiverLoad,
                        double *B, double *BLoad, double *BRiverLoad,
                        double *C, double *CLoad, double *CRiverLoad,
                        double *D, double *DLoad, double *DRiverLoad,
                        double *E, double *ELoad, double *ERiverLoad)
{
	double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
        MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    double H1, Q1;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double *PastGenericA, *PastGenericB, *PastGenericC, *PastGenericD, *PastGenericE;

    PastGenericA = new double[NumberOfBoxes];
    PastGenericB = new double[NumberOfBoxes];
    PastGenericC = new double[NumberOfBoxes];
    PastGenericD = new double[NumberOfBoxes];
    PastGenericE = new double[NumberOfBoxes];

    for (i = 0; i < NumberOfBoxes; i++)
    {
        PastGenericA[i] = A[i];
        PastGenericB[i] = B[i];
        PastGenericC[i] = C[i];
        PastGenericD[i] = D[i];
        PastGenericE[i] = E[i];
    }

   /*for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)  */
   /*for (int i = MyPEcoDynClass->GetSubDomain()->FirstLine; i <= MyPEcoDynClass->GetSubDomain()->LastLine; i++)
   {
      for (int j = MyPEcoDynClass->GetSubDomain()->IColumn[i]; j <= MyPEcoDynClass->GetSubDomain()->FColumn[i]; j++)
      {
         if ((i > MyPEcoDynClass->GetSubDomain()->ILine[j]) && (i < MyPEcoDynClass->GetSubDomain()->FLine[j]))
         { */
         for (m = 0; m < MyNumberOfBoxes; m++)
         {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
                )
            {
            index = i * GridColumns + j;

            if (
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1)
                )
            {

                // AP, 2005.05.19 ...
                H1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
                Q1 = ASpatialStep * ASpatialStep * H1;

         	    //Advective fluxes and Diffusive fluxes

                // AP, 2005.05.19 ...
//                PastGeneric = PastGenericA;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericA);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericA);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericA);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericA);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericA[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                A[index] = PastGenericA[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((A[index] < 0.0) && (A[index] > -0.000001))
                    A[index] = 0.0;

//                PastGeneric = PastGenericB;
         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericB);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericB);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericB);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericB);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericB[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                B[index] = PastGenericB[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((B[index] < 0.0) && (B[index] > -0.000001))
                    B[index] = 0.0;

//                PastGeneric = PastGenericC;
         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericC);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericC);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericC);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericC);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericC[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                C[index] = PastGenericC[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((C[index] < 0.0) && (C[index] > -0.000001))
                    C[index] = 0.0;

//                PastGeneric = PastGenericD;
         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericD);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericD);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericD);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericD);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericD[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                D[index] = PastGenericD[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((D[index] < 0.0) && (D[index] > -0.000001))
                    D[index] = 0.0;

//                PastGeneric = PastGenericE;
         	    MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericE);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericE);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericE);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericE);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericE[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                E[index] = PastGenericE[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((E[index] < 0.0) && (E[index] > -0.000001))
                    E[index] = 0.0;


                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            // AP, 2005.05.19 ...
                            A[index] = A[index] + ATimeStep / 2.0 * ALoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            B[index] = B[index] + ATimeStep / 2.0 * BLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            C[index] = C[index] + ATimeStep / 2.0 * CLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            D[index] = D[index] + ATimeStep / 2.0 * DLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            E[index] = E[index] + ATimeStep / 2.0 * ELoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;

                            break;
                        }
                    }
                }
                if (MyRiverRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfRivers(); k++)
                    {
                        if (
                          (i == MyRiverRecord[k].LineCoordinate) &&
                          (j == MyRiverRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            // AP, 2005.05.19 ...
                            A[index] = A[index] + ATimeStep / 2.0 * ARiverLoad[index] *
                                        MyRiverRecord[k].RiverFlow[ADay - 1] / Q1;
                            B[index] = B[index] + ATimeStep / 2.0 * BRiverLoad[index] *
                                        MyRiverRecord[k].RiverFlow[ADay - 1] / Q1;
                            C[index] = C[index] + ATimeStep / 2.0 * CRiverLoad[index] *
                                        MyRiverRecord[k].RiverFlow[ADay - 1] / Q1;
                            D[index] = D[index] + ATimeStep / 2.0 * DRiverLoad[index] *
                                        MyRiverRecord[k].RiverFlow[ADay - 1] / Q1;
                            E[index] = E[index] + ATimeStep / 2.0 * ERiverLoad[index] *
                                        MyRiverRecord[k].RiverFlow[ADay - 1] / Q1;

                            break;
                        }
                    }
                }
      	    }
            }
         }
//      }  //j
//   }  //i
    delete [] PastGenericA;
    delete [] PastGenericB;
    delete [] PastGenericC;
    delete [] PastGenericD;
    delete [] PastGenericE;
}

void TBiDimensionalTimeSeries::AdvectDiffuse(double *A, double *ALoad,
                                             double *B, double *BLoad,
                                             double *C, double *CLoad,
                                             double *D, double *DLoad,
                                             double *E, double *ELoad,
                                             double *F, double *FLoad,
                                             double *G, double *GLoad,
                                             double *H, double *HLoad)
{
    double MyWestUFlow, MyEastUFlow, MyNorthVFlow, MySouthVFlow,
    MyWestDiffusion, MyEastDiffusion, MyNorthDiffusion, MySouthDiffusion, ResultantDiffusion;
    double H1, Q1;
    int i,j,index, k, ADay, m;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    double *PastGenericA, *PastGenericB, *PastGenericC, *PastGenericD,
           *PastGenericE, *PastGenericF, *PastGenericG, *PastGenericH;

    PastGenericA = new double[NumberOfBoxes];
    PastGenericB = new double[NumberOfBoxes];
    PastGenericC = new double[NumberOfBoxes];
    PastGenericD = new double[NumberOfBoxes];
    PastGenericE = new double[NumberOfBoxes];
    PastGenericF = new double[NumberOfBoxes];
    PastGenericG = new double[NumberOfBoxes];
    PastGenericH = new double[NumberOfBoxes];

    for (i = 0; i < NumberOfBoxes; i++)
    {
        PastGenericA[i] = A[i];
        PastGenericB[i] = B[i];
        PastGenericC[i] = C[i];
        PastGenericD[i] = D[i];
        PastGenericE[i] = E[i];
        PastGenericF[i] = F[i];
        PastGenericG[i] = G[i];
        PastGenericH[i] = H[i];
    }

   /*for (int i = 0; i < GridLines; i++)
   {
   	for (int j = 0; j < GridColumns; j++)  */
   /*for (int i = MyPEcoDynClass->GetSubDomain()->FirstLine; i <= MyPEcoDynClass->GetSubDomain()->LastLine; i++)
   {
      for (int j = MyPEcoDynClass->GetSubDomain()->IColumn[i]; j <= MyPEcoDynClass->GetSubDomain()->FColumn[i]; j++)
      {
         if ((i > MyPEcoDynClass->GetSubDomain()->ILine[j]) && (i < MyPEcoDynClass->GetSubDomain()->FLine[j]))
         { */
         for (m = 0; m < MyNumberOfBoxes; m++)
         {
            i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
            j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
            if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j] /*- 1*/)
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i] /*- 1*/)
                )
            {
            index = i * GridColumns + j;

            if (
                    (BoxArray[index].Dummybool2) &&
                    (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&
                    (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) &&
                    (BoxArray[index].Dummybool1)
                )
            {

                // AP, 2005.05.19 ...
                H1 = BoxDepth[index] + DynamicHeight[index] - PastHeight[index];
                Q1 = ASpatialStep * ASpatialStep * H1;

         	    //Advective fluxes and Diffusive fluxes

                // AP, 2005.05.19 ...
//                PastGeneric = PastGenericA;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericA);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericA);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericA);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericA);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericA);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericA);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericA[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                A[index] = PastGenericA[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((A[index] < 0.0) && (A[index] > -0.000001))
                    A[index] = 0.0;

//                PastGeneric = PastGenericB;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericB);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericB);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericB);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericB);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericB);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericB);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericB[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                B[index] = PastGenericB[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((B[index] < 0.0) && (B[index] > -0.000001))
                    B[index] = 0.0;

//                PastGeneric = PastGenericC;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericC);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericC);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericC);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericC);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericC);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericC);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericC[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                C[index] = PastGenericC[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((C[index] < 0.0) && (C[index] > -0.000001))
                    C[index] = 0.0;

//                PastGeneric = PastGenericD;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericD);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericD);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericD);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericD);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericD);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericD);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericD[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                D[index] = PastGenericD[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((D[index] < 0.0) && (D[index] > -0.000001))
                    D[index] = 0.0;

//                PastGeneric = PastGenericE;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericE);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericE);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericE);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericE);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericE);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericE);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericE[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                E[index] = PastGenericE[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((E[index] < 0.0) && (E[index] > -0.000001))
                    E[index] = 0.0;

//                PastGeneric = PastGenericF;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericF);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericF);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericF);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericF);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericF);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericF);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericF);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericF);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericF[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                F[index] = PastGenericF[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((F[index] < 0.0) && (F[index] > -0.000001))
                    F[index] = 0.0;

//                PastGeneric = PastGenericG;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericG);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericG);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericG);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericG);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericG);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericG);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericG);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericG);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericG[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                G[index] = PastGenericG[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion) */
                               ResultantDiffusion / (2.0 * Q1);

                if ((G[index] < 0.0) && (G[index] > -0.000001))
                    G[index] = 0.0;

//                PastGeneric = PastGenericH;
                MyWestUFlow = WesternComponentOfAdvectionDiffusion(i,j,PastGenericH);
                MyEastUFlow = EasternComponentOfAdvectionDiffusion(i,j,PastGenericH);
                MyNorthVFlow = NorthernComponentOfAdvectionDiffusion(i,j,PastGenericH);
                MySouthVFlow = SouthernComponentOfAdvectionDiffusion(i,j,PastGenericH);

                MyWestDiffusion = SaltWesternHorizontalDiffusion(i,j,PastGenericH);
                MyEastDiffusion = SaltEasternHorizontalDiffusion(i,j,PastGenericH);
                MyNorthDiffusion = SaltNorthernVerticalDiffusion(i,j,PastGenericH);
                MySouthDiffusion = SaltSouthernVerticalDiffusion(i,j,PastGenericH);

                if (CheckTemporalResolutionForMassDiffusion(index, BoxDepth[index], MyWestDiffusion, MyEastDiffusion,
                                                            MyNorthDiffusion, MySouthDiffusion, PastGenericH[index]))
                        ResultantDiffusion = MyWestDiffusion + MyEastDiffusion +
                                             MyNorthDiffusion + MySouthDiffusion;
                else
                        ResultantDiffusion = 0.0;

                H[index] = PastGenericH[index] * BoxDepth[index] / H1
                            + (MyWestUFlow - MyEastUFlow -
                               MyNorthVFlow + MySouthVFlow) / Q1
                            + /*(MyWestDiffusion + MyEastDiffusion +
                               MyNorthDiffusion + MySouthDiffusion)*/
                               ResultantDiffusion / (2.0 * Q1);

                if ((H[index] < 0.0) && (H[index] > -0.000001))
                    H[index] = 0.0;

                if (MyLoadRecord != NULL)
                {
                    for (k = 0; k < MyPEcoDynClass->GetNumberOfLoads(); k++)
                    {
                        if (
                          (i == MyLoadRecord[k].LineCoordinate) &&
                          (j == MyLoadRecord[k].ColumnCoordinate)
                        )
                        {
                            ADay = MIN(MyPEcoDynClass->GetJulianDay(),
                                       MyPEcoDynClass->GetNumberOfDaysForLoads());

                            // AP, 2005.05.19 ...
                            A[index] = A[index] + ATimeStep / 2.0 * ALoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            B[index] = B[index] + ATimeStep / 2.0 * BLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            C[index] = C[index] + ATimeStep / 2.0 * CLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            D[index] = D[index] + ATimeStep / 2.0 * DLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            E[index] = E[index] + ATimeStep / 2.0 * ELoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            F[index] = F[index] + ATimeStep / 2.0 * FLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            G[index] = G[index] + ATimeStep / 2.0 * GLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            H[index] = H[index] + ATimeStep / 2.0 * HLoad[index] *
                                        MyLoadRecord[k].LoadFlow[ADay - 1] / Q1;
                            break;
                        }
                    }
                }
      	    }
            }
         }
//      }  //j
//   }  //i
    delete [] PastGenericA;
    delete [] PastGenericB;
    delete [] PastGenericC;
    delete [] PastGenericD;
    delete [] PastGenericE;
    delete [] PastGenericF;
    delete [] PastGenericG;
    delete [] PastGenericH;
}


void TBiDimensionalTimeSeries::CorrectElevation()
{
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    int m, i, j, index;
    double NewHeight;
    for (m = 0; m < MyNumberOfBoxes; m++)
    {
        i = MyPEcoDynClass->GetLineIndex(MyBoxNumber[m]);
        j = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[m]);
        if (
                  (i >= pSubDomain->ILine[j])
                  && (i <= pSubDomain->FLine[j])
                  && (j >= pSubDomain->IColumn[i])
                  && (j <= pSubDomain->FColumn[i])
            )
        {
           index = i * GridColumns + j;
           NewHeight = InitialBoxDepth[index] - (BoxDepth[index] - DynamicHeight[index]);
           BoxDepth[index] = BoxDepth[index] - DynamicHeight[index] + NewHeight;
           DynamicHeight[index] = NewHeight;
           PastHeight[index] = DynamicHeight[index];
        }
    }
}


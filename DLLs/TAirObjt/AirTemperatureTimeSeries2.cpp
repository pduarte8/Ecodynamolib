/**
 * NPI work
 * 	26 November 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <cstring>
    #include <sstream>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "tairobjt.h"
#include "iodll.h"

// TAirTemperatureTimeSeries Class
//
// Constructors invoked outside EcoDyn shell...
//
TAirTemperatureTimeSeries2::TAirTemperatureTimeSeries2(TEcoDynClass* PEcoDynClass, char* className)
                              :TAirTemperature(PEcoDynClass, className)
{
    int Lines, Columns;
    NumberOfBoxes = MyPEcoDynClass->GetGridSize();
    Lines = MyPEcoDynClass->GetNumberOfLines();
    Columns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOf2DBoxes = Lines * Columns;
    MyBoxNumber = new int[NumberOf2DBoxes];
    AirTemperatureInBox = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
       if (i < NumberOf2DBoxes) MyBoxNumber[i] = i;
       AirTemperatureInBox[i] = 0.0;
    }
    StartTime = MyPEcoDynClass->getDefaultsRecord()->StartTime;
    AMoment = StartTime;
    indexFile = -1;
    firstRead = true;
    recursiveCall = false;
    FoundFirstStep = false;
    AStep = 0;
    MeanValuesSamplesNumber = 1;
    ReadCurrentValues();
}


TAirTemperatureTimeSeries2::~TAirTemperatureTimeSeries2()
{
   freeMemory();
   ResetPointers();
}

void TAirTemperatureTimeSeries2::ResetPointers()
{
   delete [] mAirTemperature;
}

void TAirTemperatureTimeSeries2::freeMemory()
{
   delete [] AirTemperatureInBox;
}

// called directly from 'RunThread' ...
void TAirTemperatureTimeSeries2::ResetValues()
{

}

bool TAirTemperatureTimeSeries2::ReadTimeSeriesValues(int fileIndex)
{
     int X,Y;
     int XTime, XStep, XBox, Xtemperature;
     char text[512];

#ifdef __BORLANDC__
     AnsiString TimeSeriesFileName = MyPEcoDynClass->GetPathName()
        + AnsiString(PATH_SEPARATOR) + AT_TIMESERIES + AnsiString(fileIndex) + GLB_XLSP;
#else  // __BORLANDC__
     stringstream TimeSeries;
     TimeSeries << MyPEcoDynClass->GetPathName() << PATH_SEPARATOR 
			<< AT_TIMESERIES << fileIndex << GLB_XLSP;
	 string TimeSeriesFileName = TimeSeries.str();
#endif  // __BORLANDC__

     //Debugger(fileIndex);
     //Debug time series values file name...
     sprintf(text, "[TAirTemperatureTimeSeries2] Opening file %s\n",TimeSeriesFileName.c_str());
     DebugMessage(text);

     // Read in the time series values...
     sprintf(text, TimeSeriesFileName.c_str());
     TReadWrite* PReadWrite = new TReadWrite(text);
     if (!PReadWrite->SetupFile(READFILE))
     {
        return false;
     }
     indexFile = fileIndex;
     int nSizeArray = PReadWrite->GetNumberOfRows() - 2; // Last line only have EOL and first is the header line
     // Verify if all mandatory columns exist...
     
     if (!PReadWrite->FindString("Time", XTime, Y))
        return false;
     if (!PReadWrite->FindString("Step", XStep, Y))
        return false; 
     if (!PReadWrite->FindString("BoxNumber", XBox, Y))
        return false;
     if (!PReadWrite->FindString("AirTemperature", Xtemperature, Y))
        Xtemperature = -1;

     Y++;        // line above header - tipically the 2nd
     // verify how many samples the file has
     //
     int firstStep, lastStep;
     PReadWrite->ReadNumber(XStep, Y, firstStep);
     PReadWrite->ReadNumber(XStep, Y + nSizeArray - 1, lastStep);
     MeanValuesSamplesNumber = lastStep - firstStep + 1;
     // if it isn't the first read delete allocated memory
     if ((!firstRead) || (recursiveCall)) ResetPointers();
     // allocate and clear memory for all samples

     mAirTemperature = new double[NumberOf2DBoxes * MeanValuesSamplesNumber];
     memset(mAirTemperature, 0, sizeof(mAirTemperature));
     timeSeries = new long[MeanValuesSamplesNumber];
     memset(timeSeries, 0, sizeof(timeSeries));
     int index, stepNumber, nBox, loopNumber;
     index = -1;
     for (int i = 0; i < nSizeArray; i++)
     {
        PReadWrite->ReadNumber(XStep, Y + i, stepNumber);
        loopNumber = stepNumber - firstStep;
        PReadWrite->ReadNumber(XTime, Y + i, timeSeries[loopNumber]);
        if ((firstRead) && (loopNumber == 0))
        {
            if (timeSeries[loopNumber] > AMoment)
            {
#ifdef __BORLANDC__
               MessageBox(0, "TAirTemperatureTimeSeries2::ReadTimeSeriesValues", "Model run starts before time series beginning", MB_OK);
               // This means that model initial simulation time is before the fist moment of the forcing time series.
               // Therefore, either an earlier time series should be used or the initial simulation time should be shifted forward.
#else  // __BORLANDC__
               cerr << "TAirTemperatureTimeSeries2::ReadTimeSeriesValues " 
					<< "- Model run starts before time series beginning" << endl;
#endif  // __BORLANDC_
               break;
            }
            else
            {
               PReadWrite->ReadNumber(XBox, Y + i, nBox);
               MyNumberOfBoxes++;
               MyBoxNumber[i] = nBox;
            }
        }
        if (!FoundFirstStep)
        {
            if (timeSeries[loopNumber] == AMoment) {
               AStep = loopNumber;
               FoundFirstStep = true;
            }
            if (timeSeries[loopNumber] > AMoment) {
               AStep = loopNumber - 1;
               FoundFirstStep = true;
            }
        }
        index++;
        if (Xtemperature != -1) PReadWrite->ReadNumber(Xtemperature, Y + i, mAirTemperature[index]);
     }
     PReadWrite->CloseFile();
     //...end reading time series values

     meanTimeDiff = timeSeries[1] - timeSeries[0];
     if (meanTimeDiff < MyPEcoDynClass->GetTimeStep())
     {
        Debugger(meanTimeDiff); Debugger(timeSeries[0]); Debugger(timeSeries[1]); Debugger(MyPEcoDynClass->GetTimeStep()); 
#ifdef __BORLANDC__
        MessageBox(0, "TAirTemperatureTimeSeries2::ReadTimeSeriesValues", "Model step > than ice forcing time step", MB_OK);
#else  // __BORLANDC__
		   cerr << "TAirTemperatureTimeSeries2::ReadTimeSeriesValues " 
				<< "- Model step > ice forcing time step" << endl;
#endif  // __BORLANDC_
     }
     if (firstRead)
     {
       if (timeSeries[loopNumber] < StartTime)
       {
         recursiveCall = true;
         MyNumberOfBoxes = 0;
         fileIndex++;
         //Debugger(timeSeries[loopNumber]); Debugger(fileIndex);
         ReadTimeSeriesValues(fileIndex);   //Recursive call during the first reading to find the starting time series file index
       }
       else
		 recursiveCall = false;
     }
     //Debugger(fileIndex);
     return true;
}

void TAirTemperatureTimeSeries2::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name

    int MyBoxNumber = BoxNumber, index;
    strcpy(MyParameter, ParameterName);
    index = MyPEcoDynClass->GetLineIndex3D(MyBoxNumber) * NumberOfColumns + GetColumnIndex3D(MyBoxNumber);
    if (strcmp(MyParameter, "Air temperature") == 0)
    Value = AirTemperatureInBox[index];
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
                      "EcoDynamo Alert - Inquiry",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		   cerr << "TAirTemperatureTimeSeries2::Inquiry - " << MyParameter
				<< " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC_
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TAirTemperatureTimeSeries2::Go()
{
   AMoment = MyPEcoDynClass->GetTimeUTC();
   //ReadCurrentValues();
}

void TAirTemperatureTimeSeries2::ReadCurrentValues()
{
    if (firstRead)
    {
       myTimeSeriesStep = StartTime;
       ReadTimeSeriesValues(indexFile + 1);
       ReinitializeValues();
       firstRead = false;
    }
    else
       myTimeSeriesStep = AMoment;

    if ((!firstRead) && (myTimeSeriesStep >= timeSeries[MeanValuesSamplesNumber-1] + meanTimeDiff))
    {
       AStep = 0;
       ReadTimeSeriesValues(indexFile + 1);
       ReinitializeValues();
    }
    else
    if ((!firstRead) && (myTimeSeriesStep >= timeSeries[AStep] + meanTimeDiff))
    {
       AStep = AStep + 1;
       ReinitializeValues();
    }
}

void TAirTemperatureTimeSeries2::ReinitializeValues()
{
   int index, MyLineNumber, MyColumnNumber, MyLayerNumber, NumberOfLayers;
   NumberOfLayers = MyPEcoDynClass->GetNumberOfLayers();
   for (int i = 0; i < NumberOf2DBoxes; i++)
   {
      index = i + AStep * NumberOf2DBoxes;
      MyLineNumber = MyPEcoDynClass->GetLineIndex3D(MyBoxNumber[i]);
      MyColumnNumber = MyPEcoDynClass->GetColumnIndex3D(MyBoxNumber[i]);
      for (int j = 0; j < NumberOfLayers; j++)
      {
      //MyLayerNumber = MyPEcoDynClass->GetLayerIndex(MyBoxNumber[i]);
         AirTemperatureInBox[Get3DIndex(MyLineNumber, MyColumnNumber, j)] = mAirTemperature[index];
      }
   }
}


void TAirTemperatureTimeSeries2::Reinitialize()
{
   ReadCurrentValues();
}
 

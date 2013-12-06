// Water temperature object CPP code file
// @ Pedro Duarte
// Rev. A 30.01.2004
/**
 * NPI work
 * 	10 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <ctime>
	#include <sstream>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "twatobjt.h"
#include "iodll.h"


TWaterTemperatureTimeSeries2::TWaterTemperatureTimeSeries2(TEcoDynClass* PEcoDynClass, char* className)
                              :TWaterTemperature(PEcoDynClass, className)
{
    MyNumberOfBoxes = 0;
    MyBoxNumber = new int[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
       MyBoxNumber[i] = i;
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


TWaterTemperatureTimeSeries2::~TWaterTemperatureTimeSeries2()
{
   freeMemory();
   ResetPointers();
}

void TWaterTemperatureTimeSeries2::ResetPointers()
{
   free(mWaterTemperature);
   free(timeSeries);
}

void TWaterTemperatureTimeSeries2::freeMemory()
{
}

// called directly from 'RunThread' ...
void TWaterTemperatureTimeSeries2::ResetValues()
{

}

bool TWaterTemperatureTimeSeries2::ReadTimeSeriesValues(int &fileIndex)
{
     int X,Y;
     int XTime, XStep, XBox, Xtemperature;
     char text[512];

#ifdef __BORLANDC__
     AnsiString TimeSeriesFileName = MyPEcoDynClass->GetPathName()
        + AnsiString(PATH_SEPARATOR) + WT_TIMESERIES + AnsiString(fileIndex) + GLB_XLSP;
#else  // __BORLANDC__
     stringstream TimeSeries;
     TimeSeries << MyPEcoDynClass->GetPathName() << PATH_SEPARATOR 
			<< WT_TIMESERIES << fileIndex << GLB_XLSP;
	 string TimeSeriesFileName = TimeSeries.str();
#endif  // __BORLANDC__

     //Debugger(fileIndex);
     //Debug time series values file name...
     sprintf(text, "[TWaterTemperatureTimeSeries2] Opening file %s\n",TimeSeriesFileName.c_str());
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
     if (!PReadWrite->FindString("SST", Xtemperature, Y))
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
     mWaterTemperature = (double*) calloc(NumberOfBoxes * MeanValuesSamplesNumber, sizeof(double));
     timeSeries = (long*) calloc(MeanValuesSamplesNumber, sizeof(long));

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
               MessageBox(0, "TWaterTemperatureTimeSeries2::ReadTimeSeriesValues", "Model run starts before time series beginning", MB_OK);
#else  // __BORLANDC__
				cerr << "TWaterTemperatureTimeSeries2::ReadTimeSeriesValues - Model run starts before time series beginning" << endl;
#endif  // __BORLANDC__
               // This means that model initial simulation time is before the fist moment of the forcing time series.
               // Therefore, either an earlier time series should be used or the initial simulation time should be shifted forward.
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
        if (Xtemperature != -1) PReadWrite->ReadNumber(Xtemperature, Y + i, mWaterTemperature[index]);
     }
     PReadWrite->CloseFile();
     //...end reading time series values

     meanTimeDiff = timeSeries[1] - timeSeries[0];
     if (meanTimeDiff < MyPEcoDynClass->GetTimeStep())
#ifdef __BORLANDC__
        MessageBox(0, "TWaterTemperatureTimeSeries2::ReadTimeSeriesValues", "Model step > than ice forcing time step", MB_OK);
#else  // __BORLANDC__
		cerr << "TWaterTemperatureTimeSeries2::ReadTimeSeriesValues - Model step > than ice forcing time step" << endl;
#endif  // __BORLANDC__
			
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

void TWaterTemperatureTimeSeries2::Go()
{
   AMoment = MyPEcoDynClass->GetTimeUTC();
   //ReadCurrentValues();
}

void TWaterTemperatureTimeSeries2::ReadCurrentValues()
{
    if (firstRead)
    {
       myTimeSeriesStep = StartTime;
			 indexFile++;
       ReadTimeSeriesValues(indexFile);
       ReinitializeValues();
       firstRead = false;
    }
    else
       myTimeSeriesStep = AMoment;

    if ((!firstRead) && (myTimeSeriesStep >= timeSeries[MeanValuesSamplesNumber-1] + meanTimeDiff))
    {
       AStep = 0;
			 indexFile++;
       ReadTimeSeriesValues(indexFile);
       ReinitializeValues();
    }
    else
    if ((!firstRead) && (myTimeSeriesStep >= timeSeries[AStep] + meanTimeDiff))
    {
       AStep = AStep + 1;
       ReinitializeValues();
    }
}

void TWaterTemperatureTimeSeries2::ReinitializeValues()
{
   int index, MyLineNumber, MyColumnNumber, MyLayerNumber;
   
   for (int i = 0; i < MyNumberOfBoxes; i++)
   {
      index = i + AStep * MyNumberOfBoxes;
      MyLineNumber = MyPEcoDynClass->GetLineIndex3D(MyBoxNumber[i]);
      MyColumnNumber = MyPEcoDynClass->GetColumnIndex3D(MyBoxNumber[i]);
      MyLayerNumber = MyPEcoDynClass->GetLayerIndex(MyBoxNumber[i]);
      AverageWaterTemperature[Get3DIndex(MyLineNumber, MyColumnNumber, MyLayerNumber)] = mWaterTemperature[index];
   }
}


void TWaterTemperatureTimeSeries2::Reinitialize()
{
   ReadCurrentValues();
}

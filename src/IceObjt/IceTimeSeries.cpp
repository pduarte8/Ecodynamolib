/**
 * NPI work
 * 	09 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <sstream>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "iceobjt.h"
#include "iodll.h"

// TIceTimeSeries Class
//


TIceTimeSeries::TIceTimeSeries(TEcoDynClass* PEcoDynClass, char* className)
                              :TIce(PEcoDynClass, className)
{
    MyNumberOfBoxes = 0;
    MyBoxNumber = new int[NumberOfBoxesWithIce];
    for (int i = 0; i < NumberOfBoxesWithIce; i++)
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


TIceTimeSeries::~TIceTimeSeries()
{
   freeMemory();
   ResetPointers();
}

void TIceTimeSeries::ResetPointers()
{
   delete [] mSIarea;
   delete [] mSIheff;
   delete [] mSIhsnow;
   delete [] timeSeries;
}

void TIceTimeSeries::freeMemory()
{
    if (NumberOfBoxesWithIce > 0) delete [] MyBoxNumber;
    try {
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
			cerr << "TIceTimeSeries::freeMemory - " << exc.what() << endl;
	}
#endif  // __BORLANDC__
}

// called directly from 'RunThread' ...
void TIceTimeSeries::ResetValues()
{

}

bool TIceTimeSeries::ReadTimeSeriesValues(int &fileIndex)
{
     int X,Y;
     int XTime, XStep, XBox, XIarea, XIheff, XIsnow;
     char text[512];

#ifdef __BORLANDC__
     AnsiString TimeSeriesFileName = MyPEcoDynClass->GetPathName()
        + AnsiString(PATH_SEPARATOR) + SI_TIMESERIES + AnsiString(fileIndex) + GLB_XLSP;
#else  // __BORLANDC__
     stringstream TimeSeries;
		 TimeSeries << MyPEcoDynClass->GetPathName() << PATH_SEPARATOR 
				<< SI_TIMESERIES << fileIndex << GLB_XLSP;
	 string TimeSeriesFileName = TimeSeries.str();
#endif  // __BORLANDC__

     //Debugger(fileIndex);
     //Debug time series values file name...
     sprintf(text, "[TIceTimeSeries] Opening file %s\n",TimeSeriesFileName.c_str());
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
     if (!PReadWrite->FindString("SIarea", XIarea, Y))
        XIarea = -1;
     if (!PReadWrite->FindString("SIheff", XIheff, Y))
        XIheff = -1;
     if (!PReadWrite->FindString("SIhsnow", XIsnow, Y))
        XIsnow = -1;
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

     mSIarea = new double[NumberOfBoxesWithIce * MeanValuesSamplesNumber];
     memset(mSIarea, 0, sizeof(mSIarea));
     mSIheff = new double[NumberOfBoxesWithIce * MeanValuesSamplesNumber];
     memset(mSIheff, 0, sizeof(mSIheff));
     mSIhsnow = new double[NumberOfBoxesWithIce * MeanValuesSamplesNumber];
     memset(mSIhsnow, 0, sizeof(mSIhsnow));
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
               MessageBox(0, "TIceTimeSeries::ReadTimeSeriesValues", "Model run starts before time series beginning", MB_OK);
#else  // __BORLANDC__
				cerr << "TIceTimeSeries::ReadTimeSeriesValues - Model run starts before time series beginning" << endl;
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
        if (XIarea != -1) PReadWrite->ReadNumber(XIarea, Y + i, mSIarea[index]);
        if (XIheff != -1) PReadWrite->ReadNumber(XIheff, Y + i, mSIheff[index]);
        if (XIsnow != -1) PReadWrite->ReadNumber(XIsnow, Y + i, mSIhsnow[index]);
     }
     PReadWrite->CloseFile();
     //...end reading time series values

     meanTimeDiff = timeSeries[1] - timeSeries[0];
     if (meanTimeDiff < MyPEcoDynClass->GetTimeStep())
#ifdef __BORLANDC__
        MessageBox(0, "TIceTimeSeries::ReadTimeSeriesValues", "Model step > than ice forcing time step", MB_OK);
#else  // __BORLANDC__
		cerr << "TIceTimeSeries::ReadTimeSeriesValues - Model step > than ice forcing time step" << endl;
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

void TIceTimeSeries::Go()
{
   int ABoxNumber;
   AMoment = MyPEcoDynClass->GetTimeUTC();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   for (int i = 0; i < NumberOfLines; i++)
   {
      for (int j = 0; j < NumberOfColumns; j++)
      {
         ABoxNumber = GetBoxIndex(j,i);
         SeaBoxDepth = DepthOfSurfaceLayer(i,j);
         SkeletalBrineAndCongelationFluxes(ABoxNumber, Index, SeaBoxDepth);
         SkeletalMeltFluxes(ABoxNumber, Index, SeaBoxDepth);
         SkeletalDiffusion(ABoxNumber, Index, SeaBoxDepth);
      }
   }
   Iteration++;
}

void TIceTimeSeries::ReadCurrentValues()
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

void TIceTimeSeries::ReinitializeValues()
{
   int index, MyLineNumber, MyColumnNumber;
   for (int i = 0; i < MyNumberOfBoxes; i++)
   {
      index = i + AStep * MyNumberOfBoxes;
      MyLineNumber = MyPEcoDynClass->GetLineIndex(MyBoxNumber[i]);
      MyColumnNumber = MyPEcoDynClass->GetColumnIndex(MyBoxNumber[i]);
      SIarea[MyLineNumber * NumberOfColumns + MyColumnNumber] = mSIarea[index];
      SIheff[MyLineNumber * NumberOfColumns + MyColumnNumber] = mSIheff[index];
      SIhsnow[MyLineNumber * NumberOfColumns + MyColumnNumber] = mSIhsnow[index];
   }
}


void TIceTimeSeries::Reinitialize()
{
   for (int i = 0; i < NumberOfBoxesWithIce; i++)
   {
      if (Iteration > 0)
      {
         if (SIheff[i] != PastSIheff[i])  IceGrowthRate[i] = IceGrowth(i);
      }
      PastSIheff[i] = SIheff[i];
   }
   ReadCurrentValues();
}


double TIceTimeSeries::IceGrowth(int ABoxNumber)
{
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   return (SIheff[MyBoxNumber] - PastSIheff[MyBoxNumber]) / meanTimeDiff; //m/s
}












/*
 * UpdateOutput.cpp
 * 
 * - implement functions updating output files
 * 
 *   Public:
 *     void UpdateOutput(double, double)
 *     void UpdateFile(double, double)
 *     void UpdateIndividualsFile(double, double)
 *     void UpdateTimeSeriesFile(double, double)
 *     void UpdateHDFFile(double, double)
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#include "outputResults.h"

#ifdef __BORLANDC__
    #include "MLHDF.h"
#endif  // __BORLANDC__

//---------------------------------------------------------------------------
//
// Method called inside RunThread context.
//
void OutputResults::UpdateOutput(double utcTime, double ATime)
{

    if (outputToFile()) {
		MyNumberOfRecordedTimeSteps++;
        if (hdfFile)
#ifdef __BORLANDC__
            MLHDF_UpdateHDFFile();
#else  // __BORLANDC__
            UpdateHDFFile();
#endif  // __BORLANDC__
        else
            UpdateFile(utcTime, ATime);
    }
    clearMeanSamples();
}
//---------------------------------------------------------------------------
//
// Method called inside RunThread context (from UpdateWindow).
//
void OutputResults::UpdateFile(double utcTime, double ATime)
{
    if (myRegister->MeanValuesSamplesNumber > 0) {
        UpdateTimeSeriesFile(utcTime, ATime);
        return;
    }

	TReadWrite* MyPReadWrite = new TReadWrite(MyFile);
	if (!MyPReadWrite->SetupFile(WRITEAPPENDFILE)) {
#ifdef __BORLANDC__
		MessageBox(0, "Output file error.\nPlease reinitialize model again",
			   MyFile, MB_OK);
#else  // __BORLANDC__
        cerr << "Output file error (" << MyFile 
				<< ").\nPlease reinitialise model again" << endl;
#endif  // __BORLANDC__
        return;
    }

    int n = myRegister->MyPEcoDynClass->GetGridSize();
    if (pBoxFileQueue->size() > 0)
        n = pBoxFileQueue->size();

    for (int j = 0; j < n; j++)
    {
       int boxNumber = j;
       if (pBoxFileQueue->size() > 0) {
           RegBox* pBox = (RegBox*)pBoxFileQueue->RetrieveObject(j);
           boxNumber = pBox->number;
       }

       MyPReadWrite->WriteCell(utcTime,0);
       MyPReadWrite->WriteSeparator();
       MyPReadWrite->WriteCell(ATime,6);
       MyPReadWrite->WriteSeparator();
       MyPReadWrite->WriteCell(MyNumberOfRecordedTimeSteps);
       MyPReadWrite->WriteSeparator();
       if (myRegister->MyPEcoDynClass->GetNumberOfLayers() > 1)
           MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetColumnNumber3D(boxNumber));
       else
           MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetColumnNumber(boxNumber));
       MyPReadWrite->WriteSeparator();
       if (myRegister->MyPEcoDynClass->GetNumberOfLayers() > 1)
           MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetLineNumber3D(boxNumber));
       else
           MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetLineNumber(boxNumber));
       MyPReadWrite->WriteSeparator();
       MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetLayerNumber(boxNumber));
       MyPReadWrite->WriteSeparator();
       MyPReadWrite->WriteCell(boxNumber);

       for (int v = 0; v < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; v++)
	   {
           bool found = false;
	       char* variableName = myRegister->defaultsRecord->SelectedVariablesFileOutput[v];
           if (!IsVariableAvailable(variableName))
              continue;

           for (int i = 0; (i < myRegister->defaultsRecord->nrClassesSelected) && !found; i++)
           {
              if (!IsVariableAvailable(&myRegister->varsRecord[i], variableName))
                continue;

              for (int k = 0; k < myRegister->varsRecord[i].nrVariablesSelectedFileOutput; k++)
              {
                 if (strcmp(variableName,myRegister->varsRecord[i].SelectedVariablesFileOutput[k]) == 0) {
                     myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
									myRegister->varsRecord[i].Value,
                                    boxNumber,
									variableName,
									SAVEDISKCODE);
                     MyPReadWrite->WriteSeparator();
                     MyPReadWrite->WriteCell(myRegister->varsRecord[i].Value,8);
                     found = true;
                     break;
                }
              }
           }
       }
       MyPReadWrite->WriteSeparator(true);
	}
    MyPReadWrite->CloseFile();

    UpdateIndividualsFile(utcTime, ATime);
    UpdateTimeSeriesFile(utcTime, ATime);
    UpdateIceFile(utcTime, ATime);
}
//---------------------------------------------------------------------------
//
// Only one class update individuals...
//
void OutputResults::UpdateIndividualsFile(double utcTime, double ATime)
{
    if (!individualsFile)
        return;

    TReadWrite* MyPReadWrite = new TReadWrite(MyFileIndividuals);
	if (!MyPReadWrite->SetupFile(WRITEAPPENDFILE)) {
#ifdef __BORLANDC__
		MessageBox(0, "Output Individuals file error.\nPlease reinitialize model again",
			   MyFileIndividuals, MB_OK);
#else  // __BORLANDC__
        cerr << "Output Individuals file error (" << MyFileIndividuals
				<< ").\nPlease reinitialise model again" << endl;
#endif  // __BORLANDC__
        return;
    }

    int individuals;
    int n = myRegister->MyPEcoDynClass->GetGridSize();
    if (pBoxFileQueue->size() > 0)
        n = pBoxFileQueue->size();
    for (int j = 0; j < n; j++)
    {
       int boxNumber = j;
       if (pBoxFileQueue->size() > 0) {
           RegBox* pBox = (RegBox*)pBoxFileQueue->RetrieveObject(j);
           boxNumber = pBox->number;
       }
       for (int c = 0; c < myRegister->defaultsRecord->nrClassesSelected; c++)
       {
           individuals = myRegister->varsRecord[c].nrIndividuals;
           if (individuals != 0)
               break;
       }
       for (int k = 0; k < individuals; k++)
       {
           MyPReadWrite->WriteCell(ATime,6);
           MyPReadWrite->WriteSeparator();
           MyPReadWrite->WriteCell(MyNumberOfRecordedTimeSteps);
           MyPReadWrite->WriteSeparator();
           MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetColumnNumber(boxNumber));
           MyPReadWrite->WriteSeparator();
           MyPReadWrite->WriteCell(myRegister->MyPEcoDynClass->GetLineNumber(boxNumber));
           MyPReadWrite->WriteSeparator();
           MyPReadWrite->WriteCell(boxNumber);
           MyPReadWrite->WriteSeparator();
           MyPReadWrite->WriteCell(k + 1);      // Individual number

           for (int v = 0; v < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; v++)
	       {
             bool found = false;
	         char* variableName = myRegister->defaultsRecord->SelectedVariablesFileOutput[v];
             for (int i = 0; (i < myRegister->defaultsRecord->nrClassesSelected) && !found; i++)
             {
                if (!IsIndividualVariable(&myRegister->varsRecord[i], variableName))
                   continue;

                myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
								myRegister->varsRecord[i].Value,
                                k,
                                boxNumber,
      							variableName,
								SAVEDISKCODE);
                MyPReadWrite->WriteSeparator();
                MyPReadWrite->WriteCell(myRegister->varsRecord[i].Value,8);
                found = true;
                break;
             }
           }
           MyPReadWrite->WriteSeparator(true);
       }
	}
    MyPReadWrite->CloseFile();
}
//---------------------------------------------------------------------------
//
// Only one class update time series...
//
void OutputResults::UpdateTimeSeriesFile(double utcTime, double ATime)
{
 // AP, 2005.07.21...
 char timeMsg[32];
 struct tm *timeDT;
 long myTime = utcTime;
 // ...AP, 2005.07.21

    if (!timeSeriesFile || (timeSeriesVariables == 0))
        return;

    if ((MyNumberOfRecordedTimeSteps % myRegister->MeanValuesSamplesNumber) == 1)
        SetupHeaderTimeSeriesFile((int)(MyNumberOfRecordedTimeSteps / myRegister->MeanValuesSamplesNumber));

    TReadWrite* MyPReadWrite = new TReadWrite(MyFileTimeSeries);
	if (!MyPReadWrite->SetupFile(WRITEAPPENDFILE)) {
#ifdef __BORLANDC__
		MessageBox(0, "Output time series file error.\nPlease reinitialize model again",
			   MyFileTimeSeries, MB_OK);
#else  // __BORLANDC__
        cerr << "Output time series file error (" << MyFileTimeSeries
				<< ").\nPlease reinitialise model again" << endl;
#endif  // __BORLANDC__
        return;
    }

    int timeseries;
    int n = myRegister->MyPEcoDynClass->GetGridSize();
    if (pBoxFileQueue->size() > 0)
        n = pBoxFileQueue->size();

    // AP, 2005.07.21...
    timeDT = gmtime(&myTime);
    strftime(timeMsg, 22, DMY_HMS_FORMAT, timeDT);
    // ...AP, 2005.07.21

    for (int j = 0; j < n; j++)
    {
       	int boxNumber = j;
       	if (pBoxFileQueue->size() > 0) {
           	RegBox* pBox = (RegBox*)pBoxFileQueue->RetrieveObject(j);
           	boxNumber = pBox->number;
       	}
       	for (int c = 0; c < myRegister->defaultsRecord->nrClassesSelected; c++)
       	{
           	timeseries = myRegister->varsRecord[c].nTimeSeries;
           	if (timeseries != 0)
               	break;
       	}
        // AP, 2005.07.21...
       	MyPReadWrite->WriteCell(timeMsg);
       	MyPReadWrite->WriteSeparator();
        // ...AP, 2005.07.21

       	MyPReadWrite->WriteCell(utcTime);
       	MyPReadWrite->WriteSeparator();
       	MyPReadWrite->WriteCell(MyNumberOfRecordedTimeSteps);
       	MyPReadWrite->WriteSeparator();
       	MyPReadWrite->WriteCell(boxNumber);

       	for (int v = 0; v < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; v++)
       	{
       		bool found = false;
	        char* variableName = myRegister->defaultsRecord->SelectedVariablesFileOutput[v];
            for (int i = 0; (i < myRegister->defaultsRecord->nrClassesSelected) && !found; i++)
            {
            	if (!IsTimeSeriesVariable(&myRegister->varsRecord[i], variableName))
                	continue;

                myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
									myRegister->varsRecord[i].Value,
                                    boxNumber,
      								variableName,
									SAVEDISKCODE);
                MyPReadWrite->WriteSeparator();
                MyPReadWrite->WriteCell(myRegister->varsRecord[i].Value,8);
                found = true;
                break;
            }
       }
       MyPReadWrite->WriteSeparator(true);
	}
    MyPReadWrite->CloseFile();
}
//---------------------------------------------------------------------------

/*
 * SetupOutput.cpp
 * 
 * - implement functions setting up output files
 * 
 *   Public:
 *     void SetupOutput()
 *     void SetupOutputFile()
 *     void SetupIndividualsFile()
 *     void SetupTimeSeriesVariables()
 *     void SetupHeaderTimeSeriesFile()
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#include "outputResults.h"

//---------------------------------------------------------------------------

void OutputResults::SetupOutput()
{
    clearMeanSamples();
	
    if (outputToFile()) {
		BuildIndividualsRecord();
		BuildTimeSeriesRecord();
		BuildIceRecord();

		string UCFileExtension = GetFileExtension(myRegister->outputFileName);
		int StringComparison = strcmp(UCFileExtension.c_str(), ".HDF");
		hdfFile = (StringComparison == 0);
        if (hdfFile)
            SetupHDFOutput();
        else
            SetupOutputFile();
    }

    MyNumberOfRecordedTimeSteps = 0;
}
//---------------------------------------------------------------------------

void OutputResults::SetupOutputFile()
{
	// pBoxFileQueue empty indicates all boxes
	//
	FillBoxes(pBoxFileQueue, myRegister->regsBoxFileName);
		
    if (myRegister->MeanValuesSamplesNumber > 0) {
        SetupTimeSeriesVariables();
        return;
    }

	TReadWrite* MyPReadWrite = new TReadWrite(MyFile);
	if (!MyPReadWrite->SetupFile(WRITENEWFILE, false)) {
#ifdef __BORLANDC__
		MessageBox(0, "Output file error.\nPlease reinitialise model again",
                   MyFile, MB_OK);
#else  // __BORLANDC__
        cerr << "Output file error: "<< MyFile 
				<< ".\nPlease select output file again" << endl;
#endif  // __BORLANDC__
        return;
    }

    MyPReadWrite->WriteCell("Time(UTC)");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("Time(hours)");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("TimeStep");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("GridColumn");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("GridLine");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("GridLayer");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("BoxNumber");

    for (int i = 0; i < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; i++)
    {
        char* varName = myRegister->defaultsRecord->SelectedVariablesFileOutput[i];
        if (IsVariableAvailable(varName)
        	&& !IsIndividualVariable(varName)
            && !IsTimeSeriesVariable(varName)
            && !IsIceVariable(varName))
        {
            MyPReadWrite->WriteSeparator();
            MyPReadWrite->WriteCell(varName);
        }
    }
    MyPReadWrite->WriteSeparator(true);
    MyPReadWrite->CloseFile();

    int kk;
    for (int k = 0; k < myRegister->defaultsRecord->nrClassesSelected; k++)
    {
        kk = 0;
        for (int j = 0; j < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; j++)
        {
            char* varName = myRegister->defaultsRecord->SelectedVariablesFileOutput[j];
            if (IsVariableAvailable(&myRegister->varsRecord[k], varName)
                && !IsIndividualVariable(&myRegister->varsRecord[k], varName)
                && !IsTimeSeriesVariable(&myRegister->varsRecord[k], varName)
                && !IsIceVariable(&myRegister->varsRecord[k], varName))
            {
                strcpy(myRegister->varsRecord[k].SelectedVariablesFileOutput[kk], varName);
                    kk++;
            }
        }
        myRegister->varsRecord[k].nrVariablesSelectedFileOutput = kk;
    }

    SetupIndividualsFile();
    SetupTimeSeriesVariables();
    SetupIceFile();
}
//---------------------------------------------------------------------------

void OutputResults::SetupIndividualsFile()
{
    // builds header file if individuals exist
    if (individualsFile) {
    	TReadWrite* MyPReadWrite = new TReadWrite(MyFileIndividuals);
	    if (!MyPReadWrite->SetupFile(WRITENEWFILE, false)) {
 #ifdef __BORLANDC__
			MessageBox(0, "Output Individuals file error.\nPlease reinitialise model again",
                   MyFileIndividuals, MB_OK);
#else  // __BORLANDC__
			cerr << "Output Individuals file error: "<< MyFileIndividuals 
				<< ".\nPlease select output file again" << endl;
#endif  // __BORLANDC__
            return;
        }

	    MyPReadWrite->WriteCell("Time(hours)");
	    MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("TimeStep");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("GridColumn");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("GridLine");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("BoxNumber");
        MyPReadWrite->WriteSeparator();
        MyPReadWrite->WriteCell("Individual");

        for (int i = 0; i < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; i++)
        {
            if (IsIndividualVariable(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]))
            {
                MyPReadWrite->WriteSeparator();
                MyPReadWrite->WriteCell(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]);
            }
        }
        MyPReadWrite->WriteSeparator(true);
        MyPReadWrite->CloseFile();
    }
}
//---------------------------------------------------------------------------

void OutputResults::SetupTimeSeriesVariables()
{
    // builds header file if time series variables exist
    if (timeSeriesFile) {
        timeSeriesVariables = 0;
        for (int i = 0; i < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; i++)
        {
            if (IsTimeSeriesVariable(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]))
            {
				timeSeriesVariables++;
            }
        }
    }
}
//---------------------------------------------------------------------------

void OutputResults::SetupHeaderTimeSeriesFile(int fileIndex)
{
    char index[8];
    char* last = strrchr(MyFile, OSCH_SEPARATOR);
    int posI = last - MyFile;

    memset(MyFileTimeSeries, 0, sizeof(MyFileTimeSeries));

    strncpy(MyFileTimeSeries, MyFile, posI + 1);
    strcat(MyFileTimeSeries, GLB_TIMESERIES);
    sprintf(index, "%d", fileIndex);
    strcat(MyFileTimeSeries, index);
    strcat(MyFileTimeSeries, GLB_XLSP);

    TReadWrite* MyPReadWrite = new TReadWrite(MyFileTimeSeries);
    if (!MyPReadWrite->SetupFile(WRITENEWFILE, false)) {
#ifdef __BORLANDC__
		MessageBox(0, "Output times series file error.\nPlease verify if directory exists",
			   MyFileTimeSeries, MB_OK);
#else  // __BORLANDC__
        cerr << "Output time series file error: "<< MyFileTimeSeries 
				<< ".\nPlease verify if directory exists" << endl;
#endif  // __BORLANDC__
        return;
    }

    MyPReadWrite->WriteCell("Date_Time");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("Time(seconds)");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("TimeStep");
    MyPReadWrite->WriteSeparator();
    MyPReadWrite->WriteCell("BoxNumber");

    for (int i = 0; i < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; i++)
    {
        if (IsTimeSeriesVariable(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]))
        {
            MyPReadWrite->WriteSeparator();
            MyPReadWrite->WriteCell(myRegister->defaultsRecord->SelectedVariablesFileOutput[i]);
        }
    }
    MyPReadWrite->WriteSeparator(true);
    MyPReadWrite->CloseFile();
}
//---------------------------------------------------------------------------

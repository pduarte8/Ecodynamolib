/*
 * Win_MLHDF.cpp
 *
 * - implement functions to interface with Windows MLHDF class
 *
 *   USED ONLY IN WINDOWS SYSTEMS
 *
 *   Public:
 *     void MLHDF_SetupHDFSeriesFile(int fileIndex)
 * 	   void MLHDF_UpdateHDFFile()
 *
 * Copyright 2013 - A.Pereira
 */

#ifdef __BORLANDC__

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#include "outputResults.h"
#include "MLHDF.h"

void OutputResults::MLHDF_SetupHDFSeriesFile(int fileIndex)
{
    int lines = myRegister->MyPEcoDynClass->GetNumberOfLines();
    int columns = myRegister->MyPEcoDynClass->GetNumberOfColumns();
    int layers = myRegister->MyPEcoDynClass->GetNumberOfLayers();

	int posI;
    char index[8];
    char* last = strrchr(MyFile, DOT); // last delimiter '.'
    if (last != NULL)
		posI = last - MyFile;
	else
		posI = strlen(MyFile);

    memset(MyFileHDFSeries, 0, sizeof(MyFileHDFSeries));

    strncpy(MyFileHDFSeries, MyFile, posI);
    sprintf(index, "_%d", fileIndex);
    strcat(MyFileHDFSeries, index);
    strcat(MyFileHDFSeries, GLB_HDFP);

    if(MyMLHDF != NULL)
        MyMLHDF->cleanMLHDF();

    MyMLHDF = new MLHDF(8019);     //8019 port to HDFWriter
//
//  ARRANCA MATLAB E CONSTRÓI FICHEIROS HDF... PEDRO, 3/5/2004
//  AP, 2005.11.22
//
    if (layers == 1)
    {
        if (!MyMLHDF->setupHDFFile(MyFileHDFSeries,
                    myRegister->HDFSamplesNumber,
                    lines,
                    columns,
                    myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
                    myRegister->hdfOutputWater_record.SelectedVariablesFileOutput))
        {
            MyMLHDF = NULL;
            //return;
        }
    }
    else
    {
        if (!MyMLHDF->setupHDFFile(MyFileHDFSeries,
                    myRegister->HDFSamplesNumber,
                    lines,
                    columns,
                    layers,
                    myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
                    myRegister->hdfOutputWater_record.SelectedVariablesFileOutput))
        {
            MyMLHDF = NULL;
            //return;
        }
    }
    //Debugger((int)ep);
    if (iceFile)
    {
        //MessageBox(0, "Started setup of file ith water variables","Control message", MB_OK);
        last = strrchr(MyFileIce, DOT);
        if (last != NULL)
			posI = last - MyFileIce;
		else
			posI = strlen(MyFileIce);

       memset(MyFileIceHDFSeries, 0, sizeof(MyFileIceHDFSeries));
       strncpy(MyFileIceHDFSeries, MyFileIce, posI);
       sprintf(index, "_%d", fileIndex);
       strcat(MyFileIceHDFSeries, index);
       strcat(MyFileIceHDFSeries, GLB_HDFP);

       if(MyMLHDFIce != NULL)
           MyMLHDFIce->cleanMLHDF();

       MyMLHDFIce = new MLHDF(8019);     //8019 port to HDFWriter
   //
   //  ARRANCA MATLAB E CONSTRÓI FICHEIROS HDF... PEDRO, 3/5/2004
   //  AP, 2005.11.22
   //
       if (IceLayers == 1)
       {
          if (!MyMLHDFIce->setupHDFFile(MyFileIceHDFSeries,
                        myRegister->HDFSamplesNumber,
                        lines,
                        columns,
                        myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
                        myRegister->hdfOutputIce_record.SelectedVariablesFileOutput))
          {
              MyMLHDFIce = NULL;
              return;
          }
       }
       else
       {
          if (!MyMLHDFIce->setupHDFFile(MyFileIceHDFSeries,
                        myRegister->HDFSamplesNumber,
                        lines,
                        columns,
                        IceLayers,
                        myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
                        myRegister->hdfOutputIce_record.SelectedVariablesFileOutput))
          {
              MyMLHDFIce = NULL;
              return;
          }
       }
       //MessageBox(0, "Finished setup of file ith water variables","Control message", MB_OK);
    }
}
//---------------------------------------------------------------------------

void OutputResults::MLHDF_UpdateHDFFile()
{
    int lines = myRegister->MyPEcoDynClass->GetNumberOfLines();
    int columns = myRegister->MyPEcoDynClass->GetNumberOfColumns();
    int layers = myRegister->MyPEcoDynClass->GetNumberOfLayers();
    int sample, nDim;

    if (myRegister->HDFSamplesNumber > 0) {
        sample = MyNumberOfRecordedTimeSteps % myRegister->HDFSamplesNumber;

        if (sample == 1) {
            MLHDF_SetupHDFSeriesFile((int)(MyNumberOfRecordedTimeSteps
                        / myRegister->HDFSamplesNumber));
        }
        else if (sample == 0) {
            sample = myRegister->HDFSamplesNumber;
        }
        if (MyMLHDF == NULL)
            return;
    }
    else {
        sample = MyNumberOfRecordedTimeSteps;

        if (MyMLHDF == NULL) {
            strcpy(MyFileHDFSeries, MyFile);
            MyMLHDF = new MLHDF(8019);            //8019 port to HDFWriter
			nDim = (myRegister->defaultsRecord->OutputFinishTime
					- myRegister->defaultsRecord->OutputStartTime)
					/ myRegister->defaultsRecord->OutputFrequency;
			if ((nDim * myRegister->defaultsRecord->OutputFrequency) <=
					(myRegister->defaultsRecord->OutputFinishTime
					 - myRegister->defaultsRecord->OutputStartTime))
				nDim++;
    //
    //  ARRANCA MATLAB E CONSTRÓI FICHEIROS HDF... PEDRO, 3/5/2004
    //
            if (layers == 1)
            {
                if (!MyMLHDF->setupHDFFile(MyFileHDFSeries,
                            nDim,
                            lines,
                            columns,
                            myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
                            myRegister->hdfOutputWater_record.SelectedVariablesFileOutput))
                {
                    MyMLHDF = NULL;
                    return;
                }
            }
            else
            {
                if (!MyMLHDF->setupHDFFile(MyFileHDFSeries,
                            nDim,
                            lines,
                            columns,
                            layers,
                            myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
                            myRegister->hdfOutputWater_record.SelectedVariablesFileOutput))
                {
                    MyMLHDF = NULL;
                    return;
                }
            }
            if (iceFile)
            {
               if (MyMLHDFIce == NULL){
                   MyMLHDFIce = new MLHDF(8019);            //8019 port to HDFWriter
                   strcpy(MyFileIceHDFSeries, MyFileIce);
                   if (IceLayers == 1)
                    {
                        if (!MyMLHDFIce->setupHDFFile(MyFileIceHDFSeries,
                                    nDim,
                                    lines,
                                    columns,
                                    myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
                                    myRegister->hdfOutputIce_record.SelectedVariablesFileOutput))
                        {
                            MyMLHDFIce = NULL;
                            return;
                        }
                    }
                    else
                    {
                        if (!MyMLHDFIce->setupHDFFile(MyFileIceHDFSeries,
                                    nDim,
                                    lines,
                                    columns,
                                    IceLayers,
                                    myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
                                    myRegister->hdfOutputIce_record.SelectedVariablesFileOutput))
                        {
                            MyMLHDFIce = NULL;
                            return;
                        }
                    }
                }
            }
            //Debugger((int)ep);
        }
    }

    int boxNumber;
    int nBoxes = myRegister->MyPEcoDynClass->GetGridSize();
    double *DataSeries = new double[nBoxes];

    for (int v = 0; v < myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput; v++)
    {
        bool found = false;
	    char* variableName = myRegister->hdfOutputWater_record.SelectedVariablesFileOutput[v];
        if (!IsVariableAvailable(variableName))
            continue;
	    for (int i = 0; (i < myRegister->defaultsRecord->nrClassesSelected) && !found; i++)
	    {
		    if (!IsVariableAvailable(&myRegister->varsRecord[i], variableName)
					|| IsIceVariable(&myRegister->varsRecord[i], variableName))
			    continue;
		    for (int k = 0; k < myRegister->varsRecord[i].nrVariablesSelectedFileOutput; k++)
		    {
			    if (strcmp(variableName,myRegister->varsRecord[i].SelectedVariablesFileOutput[k]) == 0)
			    {
					found = true;
				    for (int j = 0; j < nBoxes; j++)
				    {
					    boxNumber = j;
                        myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
                                        myRegister->varsRecord[i].Value,
                                        boxNumber,
                                        variableName,
                                        SAVEDISKCODE);
                         DataSeries[boxNumber] = myRegister->varsRecord[i].Value;
                     }
                     if (layers == 1)
                        MyMLHDF->UpdateDataSeries(MyFileHDFSeries, sample, nBoxes,
                                    lines, columns, v,
                                    variableName, DataSeries);
                     else
                        MyMLHDF->UpdateDataSeries(MyFileHDFSeries, sample, nBoxes,
                                    lines, columns, layers, v,
                                    variableName, DataSeries);
                     break;
                }
            }
        }
    }
    //MessageBox(0, "Beginning", "Check message", MB_OK);

    memset(DataSeries, nBoxes, sizeof(double));	// clear values

	for (int v = 0; v < myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput; v++)
	{
		bool found = false;
		char* variableName = myRegister->hdfOutputIce_record.SelectedVariablesFileOutput[v];
		if (!IsVariableAvailable(variableName))
			continue;
		for (int i = 0; (i < myRegister->defaultsRecord->nrClassesSelected) && !found; i++)
		{
			if (!IsVariableAvailable(&myRegister->varsRecord[i], variableName)
				|| !IsIceVariable(&myRegister->varsRecord[i], variableName))
			continue;
			for (int k = 0; k < myRegister->varsRecord[i].nrVariablesSelectedFileOutput; k++)
			{
				if (strcmp(variableName,myRegister->varsRecord[i].SelectedVariablesFileOutput[k]) == 0)
				{
					found = true;
					for (int j = 0; j < nBoxes; j++)
					{
						boxNumber = j;
						myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
										  myRegister->varsRecord[i].Value,
										  boxNumber,
										  variableName,
										  SAVEDISKCODE);
						DataSeries[boxNumber] =  myRegister->varsRecord[i].Value;
					}
                    if (IceLayers == 1)
                        MyMLHDFIce->UpdateDataSeries(MyFileIceHDFSeries, sample, nBoxes,
                                    lines, columns, v,
                                    variableName, DataSeries);
                     else
                        MyMLHDFIce->UpdateDataSeries(MyFileIceHDFSeries, sample, nBoxes,
                                    lines, columns, IceLayers, v,
                                    variableName, DataSeries);
                     break;
                }
            }
        }
    }
    delete [] DataSeries;
}
//---------------------------------------------------------------------------

#endif  // __BORLANDC__

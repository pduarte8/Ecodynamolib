/*
 * HDFOutput.cpp
 * 
 * - implement interface functions to output and save results
 *     in files with HDF format
 * 
 *   Public:
 *     void SetupHDFOutput()
 * 	   void SetupHDFSeriesFile(int fileIndex)
 *     void UpdateHDFFile()
 *     void flipLinesAndLayers(...)    // flip lines & layers in DataSeries
 *
 *     void SetupHDFFile(...) 		// 2D and 3D versions
 * 	   void UpdateHDFDataSeries(...)  	// 2D and 3D versions
 *
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <cstring>

#ifndef __BORLANDC__
    #include "hdf5.h"
#endif  // __BORLANDC__

using namespace std;

#include "outputResults.h"

//---------------------------------------------------------------------------
void OutputResults::SetupHDFOutput()
{
    int kk, kkk1, kkk2;
    kkk1 = 0; kkk2 = 0;
    for (int k = 0; k < myRegister->defaultsRecord->nrClassesSelected; k++)
    {
        kk = 0;
        for (int j = 0; j < myRegister->defaultsRecord->nrVariablesSelectedFileOutput; j++)
        {
            char* varName = myRegister->defaultsRecord->SelectedVariablesFileOutput[j];
            if (IsVariableAvailable(&myRegister->varsRecord[k], varName)
                && !IsIndividualVariable(&myRegister->varsRecord[k], varName)
                && !IsIceVariable(&myRegister->varsRecord[k], varName))
            {
                strcpy(myRegister->varsRecord[k].SelectedVariablesFileOutput[kk], varName);
                strcpy(myRegister->hdfOutputWater_record.SelectedVariablesFileOutput[kkk1], varName);
                kk++;
                kkk1++;
            }
            else if (IsVariableAvailable(&myRegister->varsRecord[k], varName)
                  && IsIceVariable(&myRegister->varsRecord[k], varName))
            {
                strcpy(myRegister->varsRecord[k].SelectedVariablesFileOutput[kk], varName);
                strcpy(myRegister->hdfOutputIce_record.SelectedVariablesFileOutput[kkk2], varName);
                kk++;
                kkk2++;
            }
        }
        myRegister->varsRecord[k].nrVariablesSelectedFileOutput = kk;
    }
    myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput = kkk1;
    myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput = kkk2;
}
//---------------------------------------------------------------------------

#ifndef __BORLANDC__

void OutputResults::SetupHDFSeriesFile(int fileIndex)
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

//
//  BUILD FILES HDF... PEDRO, 3/5/2004
//  AP, 2005.11.22
//
    if (layers == 1)
    {
        SetupHDFFile(MyFileHDFSeries,
				   myRegister->HDFSamplesNumber,
				   lines,
				   columns,
				   myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
				   myRegister->hdfOutputWater_record.SelectedVariablesFileOutput);
    }
    else
    {
        SetupHDFFile(MyFileHDFSeries,
				   myRegister->HDFSamplesNumber,
				   lines,
				   columns,
				   layers,
				   myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
				   myRegister->hdfOutputWater_record.SelectedVariablesFileOutput);
    }
    if (iceFile)
    {
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

   //
   //  BUILD FILES HDF... PEDRO, 3/5/2004
   //  AP, 2005.11.22
   //
       if (IceLayers == 1)
       {
          SetupHDFFile(MyFileIceHDFSeries,
					 myRegister->HDFSamplesNumber,
					 lines,
					 columns,
					 myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
					 myRegister->hdfOutputIce_record.SelectedVariablesFileOutput);
       }
       else
       {
          SetupHDFFile(MyFileIceHDFSeries,
					 myRegister->HDFSamplesNumber,
					 lines,
					 columns,
					 IceLayers,
					 myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
					 myRegister->hdfOutputIce_record.SelectedVariablesFileOutput);
       }
    }
}
//---------------------------------------------------------------------------

void OutputResults::UpdateHDFFile()
{
    int lines = myRegister->MyPEcoDynClass->GetNumberOfLines();
    int columns = myRegister->MyPEcoDynClass->GetNumberOfColumns();
    int layers = myRegister->MyPEcoDynClass->GetNumberOfLayers();
    int sample, nDim;

    if (myRegister->HDFSamplesNumber > 0) {
        sample = MyNumberOfRecordedTimeSteps % myRegister->HDFSamplesNumber;

        if (sample == 1) {
            SetupHDFSeriesFile((int)(MyNumberOfRecordedTimeSteps 
					/ myRegister->HDFSamplesNumber));
        }
        else if (sample == 0) {
            sample = myRegister->HDFSamplesNumber;
        }
    }
    else {
        sample = MyNumberOfRecordedTimeSteps;

		if (MyNumberOfRecordedTimeSteps == 1) {	// first registry
//
//  BUILD FILES HDF... PEDRO, 3/5/2004
//
			strcpy(MyFileHDFSeries, MyFile);
			nDim = (myRegister->defaultsRecord->OutputFinishTime
					- myRegister->defaultsRecord->OutputStartTime)
					/ myRegister->defaultsRecord->OutputFrequency;
			if ((nDim * myRegister->defaultsRecord->OutputFrequency) <=
					(myRegister->defaultsRecord->OutputFinishTime
					 - myRegister->defaultsRecord->OutputStartTime))
				nDim++;

			if (layers == 1)
			{
				SetupHDFFile(MyFileHDFSeries,
						   nDim,
						   lines,
						   columns,
						   myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
						   myRegister->hdfOutputWater_record.SelectedVariablesFileOutput);
			}
			else
			{
				SetupHDFFile(MyFileHDFSeries,
						   nDim,
						   lines,
						   columns,
						   layers,
						   myRegister->hdfOutputWater_record.nrVariablesSelectedFileOutput,
						   myRegister->hdfOutputWater_record.SelectedVariablesFileOutput);
			}
			if (iceFile)
			{
			   strcpy(MyFileIceHDFSeries, MyFileIce);
				if (IceLayers == 1)
				{
					SetupHDFFile(MyFileIceHDFSeries,
							 nDim,
							 lines,
							 columns,
							 myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
							 myRegister->hdfOutputIce_record.SelectedVariablesFileOutput);
				}
				else
				{
					SetupHDFFile(MyFileIceHDFSeries,
							 nDim,
							 lines,
							 columns,
							 IceLayers,
							 myRegister->hdfOutputIce_record.nrVariablesSelectedFileOutput,
							 myRegister->hdfOutputIce_record.SelectedVariablesFileOutput);
				}
			}
		}
    }

    int nBoxes = myRegister->MyPEcoDynClass->GetGridSize();
    int boxNumber;
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
					flipLinesAndLayers(DataSeries, lines, columns, layers);
					if (layers == 1)
						UpdateHDFDataSeries(MyFileHDFSeries, sample, nBoxes,
								lines, columns, v, 
								variableName, DataSeries);
					else
						UpdateHDFDataSeries(MyFileHDFSeries, sample, nBoxes, 
								lines, columns, layers, v, 
								variableName, DataSeries);

					break;
				}
			}
		}
	}
    delete [] DataSeries;

    int nBoxesIce = myRegister->MyPEcoDynClass->GetIceGridSize(); //Debugger(m); Debugger(IceLayers);
    double *IceDataSeries = new double[nBoxesIce];
	
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
					for (int j = 0; j < nBoxesIce; j++)
					{
						boxNumber = j;
						myRegister->varsRecord[i].ObjectPointer->Inquiry((char*)NULL,
										  myRegister->varsRecord[i].Value,
										  boxNumber,
										  variableName,
										  SAVEDISKCODE);
						IceDataSeries[boxNumber] =  myRegister->varsRecord[i].Value;
					}
					flipLinesAndLayers(IceDataSeries, lines, columns, IceLayers);
					if (IceLayers == 1)
						UpdateHDFDataSeries(MyFileIceHDFSeries, sample, nBoxesIce, 
								lines, columns, v,
								variableName, IceDataSeries);
					else
						UpdateHDFDataSeries(MyFileIceHDFSeries, sample, nBoxesIce, 
								lines, columns, IceLayers, v,
								variableName, IceDataSeries);
					break;
				}
			}
		}
	}	
    delete [] IceDataSeries;
}
//---------------------------------------------------------------------------

//
// flip lines within DataSeries matrix
//
void OutputResults::flipLinesAndLayers(double *DataSeries, 
		int nLines, int nColumns, int nLayers)
{
	int x, y, z, boxBase, boxFliped, xy;
	double temp;
	
	// Flip lines
	for (z = 0; z < nLayers; z++) 
		for (y = 0; y < nLines/2; y++)
			for (x = 0; x < nColumns; x++)
	{
		boxBase = nLines*nColumns*z + nColumns*y + x;
		boxFliped = nLines*nColumns*z + nColumns*(nLines-y) - (nColumns-x);
		temp = DataSeries[boxBase];
		DataSeries[boxBase] = DataSeries[boxFliped];
		DataSeries[boxFliped] = temp;
	}
	// Flip layers
	for (z = 0; z < nLayers/2; z++) 
		for (xy = 0; xy < nLines*nColumns; xy++)
	{
		boxBase = nLines*nColumns*z + xy;
		boxFliped = nLines*nColumns*(nLayers-z-1) + xy;
		temp = DataSeries[boxBase];
		DataSeries[boxBase] = DataSeries[boxFliped];
		DataSeries[boxFliped] = temp;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//   			2D systems
//---------------------------------------------------------------------------

bool OutputResults::SetupHDFFile(char* AFileName, int nDim, 
		int nLines, int nColumns,
		int NumberOfVariables, char names[][256]) 
{
	hid_t file_id, dset_id, dspace_id;
	hsize_t dims[3];
	herr_t status;
	
	// 1. create HDF file
	file_id = H5Fcreate(AFileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	
	// 2. create data space
	dims[0] = nDim;
	dims[1] = nLines;
	dims[2] = nColumns;
	dspace_id = H5Screate_simple(3, dims, NULL);

	// 3. create one dataset for each variable; then close it
    for (int i = 0; i < NumberOfVariables; i++)
    {
        dset_id = H5Dcreate(file_id, names[i], H5T_IEEE_F64LE, 
				dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = H5Dclose(dset_id);
	}
	
	// 4. close data space
	status = H5Sclose(dspace_id);
	
	// 5. close file
	status = H5Fclose(file_id);
    return true;
}
//---------------------------------------------------------------------------

void OutputResults::UpdateHDFDataSeries(char* AFileName, int Step,
		int n, int nLines, int nColumns, int DataID, 
		char* variableName, double* DataSet)
{
    hid_t file_id, dset_id, memspace, space;
    herr_t status;
    hsize_t offset[3]={Step-1, 0, 0};
    hsize_t count[3]={1, nLines, nColumns};
    hsize_t slabsize[3]={1, nLines, nColumns};
    
    // 1. open HDF file
    file_id = H5Fopen(AFileName, H5F_ACC_RDWR, H5P_DEFAULT);
    
    // 2. open 'variableName' dataset
    dset_id = H5Dopen(file_id, variableName, H5P_DEFAULT);
    
    // 3. open memory space
    memspace = H5Screate_simple (3, slabsize, NULL);
    
    // 4. get data space of dataset
	space = H5Dget_space (dset_id);

    // 5. select hyperslab - hyper rectangle
    status = H5Sselect_hyperslab (space, H5S_SELECT_SET, offset, NULL,
			count, NULL);

	// 6. write data
    status = H5Dwrite (dset_id, H5T_NATIVE_DOUBLE, memspace, space, 
					  H5P_DEFAULT, DataSet);

	// 7. close open resources
    status=H5Sclose(space);
    status=H5Sclose(memspace);
    status=H5Dclose(dset_id);
    status=H5Fclose(file_id);
}

//---------------------------------------------------------------------------
//   			3D systems
//---------------------------------------------------------------------------

bool OutputResults::SetupHDFFile(char* AFileName, int nDim, 
		int nLines, int nColumns, int nLayers,
		int NumberOfVariables, char names[][256])
{
	hid_t file_id, dset_id, dspace_id;
	hsize_t dims[4];
	herr_t status;
	
	// 1. create HDF file
	file_id = H5Fcreate(AFileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	
	// 2. create data space
	dims[0] = nDim;
	dims[1] = nLayers;
	dims[2] = nLines;
	dims[3] = nColumns;
	dspace_id = H5Screate_simple(4, dims, NULL);

	// 3. create one dataset for each variable; then close it
    for (int i = 0; i < NumberOfVariables; i++)
    {
        dset_id = H5Dcreate(file_id, names[i], H5T_IEEE_F64LE, 
				dspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		status = H5Dclose(dset_id);
	}

	// 4. close data space
	status = H5Sclose(dspace_id);
	
	// 5. close file
	status = H5Fclose(file_id);
    return true;
}
//---------------------------------------------------------------------------

void OutputResults::UpdateHDFDataSeries(char* AFileName,int Step, 
		int n, int nLines, int nColumns, int nLayers, int DataID, 
		char* variableName, double* DataSet)
{
    hid_t file_id, dset_id, memspace, space;
    herr_t status;
    hsize_t offset[4]={Step-1, 0, 0, 0};
    hsize_t count[4]={1, nLayers, nLines, nColumns};
    hsize_t slabsize[4]={1, nLayers, nLines, nColumns};
    
    // 1. open HDF file
    file_id = H5Fopen(AFileName, H5F_ACC_RDWR, H5P_DEFAULT);
    
    // 2. open 'variableName' dataset
    dset_id = H5Dopen(file_id, variableName, H5P_DEFAULT);
    
    // 3. open memory space
    memspace = H5Screate_simple (4, slabsize, NULL);
    
    // 4. get data space of dataset
	space = H5Dget_space (dset_id);

    // 5. select hyperslab - hyper rectangle
    status = H5Sselect_hyperslab (space, H5S_SELECT_SET, offset, NULL,
			count, NULL);

	// 6. write data
    status = H5Dwrite (dset_id, H5T_NATIVE_DOUBLE, memspace, space, 
					  H5P_DEFAULT, DataSet);

	// 7. close open resources
    status=H5Sclose(space);
    status=H5Sclose(memspace);
    status=H5Dclose(dset_id);
    status=H5Fclose(file_id);
}
//---------------------------------------------------------------------------
#endif  // __BORLANDC__



/*
 * OutputResults.cpp
 * 
 * - implement functions related OutputResults class
 * 
 *   Public:
 *     void FinalizeOutput()
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#include "outputResults.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
/**
 * Convenience functions to UPPER and lower strings
 */
void stringToUpper(string &s)
{
   for(unsigned int l = 0; l < s.length(); l++)
  {
    s[l] = toupper(s[l]);
  }
}
		//----------------------------//

void stringToLower(string &s)
{
   for(unsigned int l = 0; l < s.length(); l++)
  {
    s[l] = tolower(s[l]);
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

OutputResults::OutputResults(OUTPUT_REGISTER* pRegister) {
	myRegister = pRegister;
	
    individualsFile = false;
    timeSeriesFile = false;
    timeSeriesVariables = 0;
    
    pBoxFileQueue = new Queue();
    pLogStepQueue = new Queue();
    
	sprintf(MyFile, "%s%s%s", myRegister->defaultsRecord->PathName, 
			PATH_SEPARATOR, myRegister->outputFileName);

    BuildIndividualsFilename(MyFile, MyFileIndividuals);
    BuildIceFilename(MyFile, MyFileIce);
}
//---------------------------------------------------------------------------

OutputResults::~OutputResults() {
	delete pBoxFileQueue;
	delete pLogStepQueue;
}

//---------------------------------------------------------------------------

bool OutputResults::outputToFile()
{
    if(myRegister->MeanValuesSamplesNumber > 0)
		return true;
		
    return (strlen(myRegister->outputFileName) != 0
        && myRegister->defaultsRecord->nrVariablesSelectedFileOutput > 0);
}
//---------------------------------------------------------------------------
//
// Verify if 'varName' variable is available
//
bool OutputResults::IsVariableAvailable(char* varName)
{
    for (int i = 0; i < myRegister->defaultsRecord->nrClassesSelected; i++)
    {
        if (IsVariableAvailable(&myRegister->varsRecord[i], varName))
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//
// Verify if 'varName' variable is available on 'pVars' structure
//
bool OutputResults::IsVariableAvailable(TEcoDynClass::VarsRecord* pVars, char* varName)
{
    for (int i = 0; i < pVars->ObjectPointer->GetNumberOfVariablesInObject(); i++)
    {
        if (strcmp(pVars->AvailableVariables[i], varName) == 0)
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------

void OutputResults::clearMeanSamples()
{
    if ((myRegister->defaultsRecord->nrVariablesSelectedFileOutput +
		 myRegister->defaultsRecord->nrVariablesSelectedGraphOutput +
		 myRegister->defaultsRecord->nrVariablesSelectedTableOutput) > 0)
    {
        for (int i = 0; i < myRegister->defaultsRecord->nrClassesSelected; i++) {
            if (myRegister->varsRecord[i].ObjectPointer != NULL)
                myRegister->varsRecord[i].ObjectPointer->ClearMeanSamples();
        }
    }
}
//---------------------------------------------------------------------------
//
// Returns file extension (with dot) in upper case
//
string OutputResults::GetFileExtension(string AFileName)  
{
    unsigned index = AFileName.find_last_of(".");
    if (index == string::npos)		// no extension defined
		return " "; 		// send 'space' extension
    string fileExtension = AFileName.substr(index); //extension with dot
    stringToUpper(fileExtension);
    return fileExtension;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// Individuals File functions 
//
void OutputResults::BuildIndividualsRecord() {
    // verify if individuals exists
	int n=0;
    for (int k = 0; k < myRegister->defaultsRecord->nrClassesSelected; k++)
    {
        myRegister->varsRecord[k].pVarNameArray =
			myRegister->varsRecord[k].ObjectPointer->Inquiry("Main",
							myRegister->varsRecord[k].nrIndividuals,
							myRegister->varsRecord[k].dimVariables);
        n += myRegister->varsRecord[k].dimVariables;
    }
    individualsFile = (n > 0);
}
//---------------------------------------------------------------------------

void OutputResults::BuildIndividualsFilename(char* name, char* individName)
{
    char* lastDot = strrchr(name, DOT); // last delimiter '.'
    if (lastDot == NULL) {
		strcpy(individName, name);
		strcat(individName, GLB_INDIVIDUALS);
	}
	else {
		int pos = lastDot - name;
		strncpy(individName, name, pos);
		strcat(individName, GLB_INDIVIDUALS);
		strcat(individName, &name[pos]);
	}
}
//---------------------------------------------------------------------------
//
// Verify if 'pName' variable is an individual variable
//
bool OutputResults::IsIndividualVariable(char* pName)
{
    for (int i = 0; i < myRegister->defaultsRecord->nrClassesSelected; i++)
    {
        if (IsIndividualVariable(&myRegister->varsRecord[i], pName))
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//
// Verify if 'pName' variable is an individual variable of 'pVars' structure
//
bool OutputResults::IsIndividualVariable(TEcoDynClass::VarsRecord* pVars, char* pName)
{
    for (int k = 0; k < pVars->dimVariables; k++)
    {
        if (strcmp(pVars->pVarNameArray[k], pName) == 0)
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// TimeSeries File functions 
//
void OutputResults::BuildTimeSeriesRecord() {
    // verify if time series variables exists
	int n=0;
    for (int k = 0; k < myRegister->defaultsRecord->nrClassesSelected; k++)
    {
        myRegister->varsRecord[k].pTimeSeriesArray =
			myRegister->varsRecord[k].ObjectPointer->Inquiry("Main",
								myRegister->varsRecord[k].nTimeSeries);
        n += myRegister->varsRecord[k].nTimeSeries;
    }
    timeSeriesFile = (n > 0);
}
//---------------------------------------------------------------------------
//
// Verify if 'pName' variable is a time series variable
//
bool OutputResults::IsTimeSeriesVariable(char* pName)
{
    for (int i = 0; i < myRegister->defaultsRecord->nrClassesSelected; i++)
    {
        if (IsTimeSeriesVariable(&myRegister->varsRecord[i], pName))
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//
// Verify if 'pName' variable is a time series variable of 'pVars' structure
//
bool OutputResults::IsTimeSeriesVariable(TEcoDynClass::VarsRecord* pVars, char* pName)
{
    for (int k = 0; k < pVars->nTimeSeries; k++)
    {
        if (strcmp(pVars->pTimeSeriesArray[k], pName) == 0)
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// Ice File functions
//
void OutputResults::BuildIceRecord() {
    // verify if ice exists
    int n=0; 
    IceLayers = 0;
    for (int k = 0; k < myRegister->defaultsRecord->nrClassesSelected; k++)
    {
          myRegister->varsRecord[k].pIceVarNameArray =
                myRegister->varsRecord[k].ObjectPointer->InquiryIce("Main",
							myRegister->varsRecord[k].nrIceLayers,
                            myRegister->varsRecord[k].nrIceVariables);
        IceLayers = max(IceLayers,myRegister->varsRecord[k].nrIceLayers);
        n += myRegister->varsRecord[k].nrIceVariables;
    }
    iceFile = (n > 0);
}
//---------------------------------------------------------------------------

void OutputResults::BuildIceFilename(char* name, char* iceName)
{
    char* lastDot = strrchr(name, DOT); // last delimiter '.'
    if (lastDot == NULL) {
		strcpy(iceName, name);
		strcat(iceName, GLB_ICE1);
	}
	else {
		int pos = lastDot - name;
		strncpy(iceName, name, pos);
		strcat(iceName, GLB_ICE1);
		strcat(iceName, &name[pos]);
	}
}
//---------------------------------------------------------------------------
//
// Verify if 'pName' variable is an ice variable
//
bool OutputResults::IsIceVariable(char* pName)
{
    for (int i = 0; i < myRegister->defaultsRecord->nrClassesSelected; i++)
    {
        if (IsIceVariable(&myRegister->varsRecord[i], pName))
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//
// Verify if 'pName' variable is an ice variable of 'pVars' structure
//
bool OutputResults::IsIceVariable(TEcoDynClass::VarsRecord* pVars, char* pName)
{
    for (int k = 0; k < pVars->nrIceVariables; k++)
    {
        if (strcmp(pVars->pIceVarNameArray[k], pName) == 0)
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//
// Fill pBoxFileQueue with points to register
//
void OutputResults::FillBoxes(Queue* pQueue, char* filename)
{
	PointBox* outBoxes;
	int nBoxes, nC, nL;

    if (readPointsFile(filename, nBoxes, outBoxes)) {
		
		nC = myRegister->MyPEcoDynClass->GetNumberOfColumns();
		nL = myRegister->MyPEcoDynClass->GetNumberOfLines();

		// fill pBoxFileQueue with points
        for (int i = 0; i < nBoxes; i++)
        {
            if ((outBoxes[i].Line < 1) || (outBoxes[i].Line > nL))
                continue;
            if ((outBoxes[i].Column < 1) || (outBoxes[i].Column > nC))
                continue;
            
            int boxNumber = (nL - outBoxes[i].Line) * nC + outBoxes[i].Column - 1;
            RegBox* pBox = new RegBox(boxNumber, outBoxes[i].Line, outBoxes[i].Column);
            if (!pQueue->InsertObject((TObject*)pBox))
                continue;
         }
         delete [] outBoxes;	// array can be discarded
	}
}
//---------------------------------------------------------------------------

bool OutputResults::readPointsFile(char* fileName, int& size, 
		PointBox* &filePoints)
{
    int n;
    int Y, XCol, XLin;
	char fullFileName[512];
	
	sprintf(fullFileName, "%s%s%s", myRegister->defaultsRecord->PathName, 
		PATH_SEPARATOR, fileName);
		
    size = 0;
    TReadWrite* PReadWrite = new TReadWrite(fullFileName);
	if (!PReadWrite->SetupFile(READFILE))
	{
		return false;
	}

    // Verify if all mandatory columns exist...
    //
    if (!PReadWrite->FindString("COLUMN_X_", XCol, Y)) {
        PReadWrite->CloseFile();
        return false;
    }
    if (!PReadWrite->FindString("LINE_Y_", XLin, Y)) {
        PReadWrite->CloseFile();
        return false;
    }

    Y++;

	int nSizeArray = PReadWrite->GetNumberOfRows() - 1; // remove header line
    filePoints = new PointBox[nSizeArray];
    size = nSizeArray;

    for (int i = 0; i < nSizeArray; i++)
    {
        PReadWrite->ReadNumber(XCol, Y + i, filePoints[i].Column, -1);
        PReadWrite->ReadNumber(XLin, Y + i, filePoints[i].Line, -1);
    }
    PReadWrite->CloseFile();
    //...end reading boxes values
    return true;
}
//---------------------------------------------------------------------------

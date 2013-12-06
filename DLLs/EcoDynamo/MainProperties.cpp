/*
 * MainProperties.cpp 
 * 
 * - implement EcoDynamo functions related with Properties:
 * 
 *   Public:
 *     bool readParameters(char* fileName)
 *     bool saveParameters(char* fileName)
 * 
 *   Private:
 *     bool readParameters(Properties *pProps, DefaultsRecord *pDefaults, 
 *         char* path, char* fileName)
 *     bool saveParameters(Properties *pProps, DefaultsRecord *pDefaults, 
 *         char* path, char* fileName)
 * 
 *   Local:
 *     long convertTimeToLong(char* timeStr)
 *     void convertLongToTime(long lTime, char* timeStr)
 * 
 * Copyright 2013 - A.Pereira
 * 
 *  2013.10.23 - simulation and output times saved in 'DD-MM-AAAA hh:mm' format
 */


#include <iostream>
#include <fstream>

using namespace std;

#include "EcoDynamo.h"
#include "modelState.h"

//---------------------------------------------------------------------------
//  Time convenience functions - AP, 2013.10.23
//---------------------------------------------------------------------------
#define DT_PROPERTIES "%02d-%02d-%04d %02d:%02d"
/*
 * support time saved in two formats: UTC or date-time
 * maintaining compatibility with old versions of "EcoDynamo.properties"
 */

bool oldTime = false;

long convertTimeToLong(char* timeStr)
{
  struct tm timeDT;

    memset( &timeDT, 0, sizeof(timeDT) );
    if (strlen(timeStr) == 0)
        return 0;

	if (strchr(timeStr, ':') == NULL) {
		// time was saved in UTC format
        oldTime = true;
		return atol(timeStr);
	}
	
	// time was saved in DateTime format
    sscanf(timeStr, DT_PROPERTIES,
           &timeDT.tm_mday, &timeDT.tm_mon, &timeDT.tm_year,
           &timeDT.tm_hour, &timeDT.tm_min);
    timeDT.tm_year -= 1900;
    timeDT.tm_mon--;
    timeDT.tm_hour;
    timeDT.tm_sec = 1;    // this is done to avoid error on 01.01.1970 00:00:00
    timeDT.tm_isdst = 0;

    return (mktime(&timeDT) - 1);
}
//---------------------------------------------
/*
 * time is saved in original format
 */
void convertLongToTime(long lTime, char* timeStr)
{
  struct tm *timeDT;

   if (oldTime)
        sprintf(timeStr, "%ld", lTime);
    else {
        timeDT = gmtime(&lTime);
        sprintf(timeStr, DT_PROPERTIES,
                timeDT->tm_mday, timeDT->tm_mon+1, timeDT->tm_year+1900,
                timeDT->tm_hour, timeDT->tm_min);
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

bool EcoDynamo::readParameters(char* fileName) {
	if (fileName == NULL)
		return readParameters(pProperties, &defaultsRecord, 
			defaultModelPath, "EcoDynamo.properties");
	return readParameters(pProperties, &defaultsRecord, 
		defaultModelPath, fileName);
}
//---------------------------------------------------------------------------

bool EcoDynamo::saveParameters(char* fileName) {
	if (fileName == NULL)
		return saveParameters(pProperties, &defaultsRecord, 
			defaultModelPath, "EcoDynamo.properties");
    return saveParameters(pProperties, &defaultsRecord, 
		defaultModelPath, fileName);
}
//---------------------------------------------------------------------------

bool EcoDynamo::readParameters(Properties *pProps, DefaultsRecord *pDefaults, 
		char* path, char* fileName)
{    
  char ParValue[128];
  char ParName[128];
  int i;

    if (!pProps->readProperties(fileName, path))
        return false;

    strcpy(pDefaults->PathName, path);
    if (pProps->getProperty("PrefixName", ParValue) != -1)
        strcpy(pDefaults->PrefixName, ParValue);
    if (pProps->getProperty("Type", ParValue) != -1)
        strcpy(pDefaults->EcosystemType, ParValue);

    if (pProps->getProperty("NrClassesSelected", ParValue) != -1) {
        pDefaults->nrClassesSelected = atoi(ParValue);
        for (i = 0; i < pDefaults->nrClassesSelected; i++) {
            sprintf(ParName, "Selected_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                strcpy(pDefaults->SelectedClasses[i], ParValue);
        }
    }
    if (pProps->getProperty("NrClassesAvailable", ParValue) != -1) {
        pDefaults->nrClassesAvailable = atoi(ParValue);
        for (i = 0; i < pDefaults->nrClassesAvailable; i++) {
            sprintf(ParName, "Available_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                strcpy(pDefaults->AvailableClasses[i], ParValue);
        }
    }

    if (pProps->getProperty("NrVariablesAvailable", ParValue) != -1) {
        pDefaults->nrVariablesAvailable = atoi(ParValue);
        for (i = 0; i < pDefaults->nrVariablesAvailable; i++) {
            sprintf(ParName, "AvailableVars_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                strcpy(pDefaults->AvailableVariables[i], ParValue);
        }
     }
    if (pProps->getProperty("NrVariablesSelectedFileOutput", ParValue) != -1) {
        pDefaults->nrVariablesSelectedFileOutput = atoi(ParValue);
        for (i = 0; i < pDefaults->nrVariablesSelectedFileOutput; i++) {
            sprintf(ParName, "SelectedVarsFileOutput_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                strcpy(pDefaults->SelectedVariablesFileOutput[i], ParValue);
        }
    }
    if (pProps->getProperty("NrVariablesSelectedGraphOutput", ParValue) != -1) {
        pDefaults->nrVariablesSelectedGraphOutput = atoi(ParValue);
        for (i = 0; i < pDefaults->nrVariablesSelectedGraphOutput; i++) {
            sprintf(ParName, "SelectedVarsGraphOutput_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                strcpy(pDefaults->SelectedVariablesGraphOutput[i], ParValue);
        }
    }

    if (pProps->getProperty("NrVariablesSelectedTableOutput", ParValue) != -1) {
        pDefaults->nrVariablesSelectedTableOutput = atoi(ParValue);
        for (i = 0; i < pDefaults->nrVariablesSelectedTableOutput; i++) {
            sprintf(ParName, "SelectedVarsTableOutput_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                strcpy(pDefaults->SelectedVariablesTableOutput[i], ParValue);
        }
    }

    if (pProps->getProperty("StartTime", ParValue) != -1)
        pDefaults->StartTime = convertTimeToLong(ParValue);
    if (pProps->getProperty("FinishTime", ParValue) != -1)
        pDefaults->FinishTime = convertTimeToLong(ParValue);
    if (pProps->getProperty("TimeStep", ParValue) != -1)
        pDefaults->TimeStep = atof(ParValue);
    if (pProps->getProperty("TimeUnit", ParValue) != -1)
        strcpy(pDefaults->TimeUnit, ParValue);
    if (pProps->getProperty("RunMode", ParValue) != -1)
        strcpy(pDefaults->RunMode, ParValue);
    if (pProps->getProperty("Integration", ParValue) != -1)
        strcpy(pDefaults->Integration, ParValue);
    if (pProps->getProperty("OutputStartTime", ParValue) != -1)
        pDefaults->OutputStartTime = convertTimeToLong(ParValue);
    if (pProps->getProperty("OutputFinishTime", ParValue) != -1)
        pDefaults->OutputFinishTime = convertTimeToLong(ParValue);
    if (pProps->getProperty("OutputFrequency", ParValue) != -1)
        pDefaults-> OutputFrequency = atof(ParValue);
    if (pProps->getProperty("OutputTimeUnit", ParValue) != -1)
        strcpy(pDefaults->OutputTimeUnit, ParValue);
    if (pProps->getProperty("SimulationRunType", ParValue) != -1)
        strcpy(pDefaults->SimulationRunType, ParValue);
    if (pProps->getProperty("NumberOfPeriods", ParValue) != -1) {
        pDefaults->NumberOfPeriods = atoi(ParValue);
        for (i = 0; i < pDefaults->NumberOfPeriods; i++) {
            sprintf(ParName, "StartPeriod_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                pDefaults->period[i].start = atol(ParValue);
            sprintf(ParName, "FinishPeriod_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                pDefaults->period[i].finish = atol(ParValue);
            sprintf(ParName, "CyclePeriod_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                pDefaults->period[i].cycle = atol(ParValue);
            sprintf(ParName, "UnitCycle_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                pDefaults->period[i].unit = atoi(ParValue);
            sprintf(ParName, "FirstFileIndex_%d", i);
            if (pProps->getProperty(ParName, ParValue) != -1)
                pDefaults->period[i].fileIndex = atoi(ParValue);
        }
    }
    
    strcpy(pDefaults->EcosystemName, pDefaults->PrefixName);
	if (strlen(defaultModelName) != 0)
		strcpy(pDefaults->EcosystemName, defaultModelName);
		
    return true;
}
//---------------------------------------------------------------------------

bool EcoDynamo::saveParameters(Properties *pProps, DefaultsRecord *pDefaults,
        char* path, char* fileName)
{
  char ParName[128];
  char ParValue[256];
  int i;

    if (strlen(pDefaults->PathName) <= 0)
        return false;

    pProps->clear();
    pProps->putProperty("PrefixName", pDefaults->PrefixName);
    pProps->putProperty("Type", pDefaults->EcosystemType);
    pProps->putProperty("PathName", pDefaults->PathName);
    sprintf(ParValue, "%d", pDefaults->nrClassesAvailable);
    pProps->putProperty("NrClassesAvailable", ParValue);
    for (i = 0; i < pDefaults->nrClassesAvailable; i++) {
        sprintf(ParName, "Available_%d", i);
        pProps->putProperty(ParName,
                pDefaults->AvailableClasses[i]);
    }
    sprintf(ParValue, "%d", pDefaults->nrClassesSelected);
    pProps->putProperty("NrClassesSelected", ParValue);
    for (i = 0; i < pDefaults->nrClassesSelected; i++) {
        sprintf(ParName, "Selected_%d", i);
        pProps->putProperty(ParName,
                pDefaults->SelectedClasses[i]);
    }

    sprintf(ParValue, "%d", pDefaults->nrVariablesAvailable);
    pProps->putProperty("NrVariablesAvailable", ParValue);
    for (i = 0; i < pDefaults->nrVariablesAvailable; i++) {
        sprintf(ParName, "AvailableVars_%d", i);
        pProps->putProperty(ParName,
                pDefaults->AvailableVariables[i]);
    }

    sprintf(ParValue, "%d", pDefaults->nrVariablesSelectedFileOutput);
    pProps->putProperty("NrVariablesSelectedFileOutput", ParValue);
    for (i = 0; i < pDefaults->nrVariablesSelectedFileOutput; i++) {
        sprintf(ParName, "SelectedVarsFileOutput_%d", i);
        pProps->putProperty(ParName,
                pDefaults->SelectedVariablesFileOutput[i]);
    }

    sprintf(ParValue, "%d", pDefaults->nrVariablesSelectedGraphOutput);
    pProps->putProperty("NrVariablesSelectedGraphOutput", ParValue);
    for (i = 0; i < pDefaults->nrVariablesSelectedGraphOutput; i++) {
        sprintf(ParName, "SelectedVarsGraphOutput_%d", i);
        pProps->putProperty(ParName,
                pDefaults->SelectedVariablesGraphOutput[i]);
    }

    sprintf(ParValue, "%d", pDefaults->nrVariablesSelectedTableOutput);
    pProps->putProperty("NrVariablesSelectedTableOutput", ParValue);
    for (i = 0; i < pDefaults->nrVariablesSelectedTableOutput; i++) {
        sprintf(ParName, "SelectedVarsTableOutput_%d", i);
        pProps->putProperty(ParName,
                pDefaults->SelectedVariablesTableOutput[i]);
    }

    long ltime = pDefaults->StartTime;
    if ((path != NULL) && (getModelState() >= STM_INIT))
        ltime = modelTime;
    convertLongToTime(ltime, ParValue);
    pProps->putProperty("StartTime", ParValue);

    convertLongToTime(pDefaults->FinishTime, ParValue);
    pProps->putProperty("FinishTime", ParValue);
    sprintf(ParValue, "%f", pDefaults->TimeStep);
    pProps->putProperty("TimeStep", ParValue);
    pProps->putProperty("TimeUnit", pDefaults->TimeUnit);
    pProps->putProperty("RunMode", pDefaults->RunMode);
    pProps->putProperty("Integration", pDefaults->Integration);
    convertLongToTime(pDefaults->OutputStartTime, ParValue);
    pProps->putProperty("OutputStartTime", ParValue);
    convertLongToTime(pDefaults->OutputFinishTime, ParValue);
    pProps->putProperty("OutputFinishTime", ParValue);
    sprintf(ParValue, "%f", pDefaults->OutputFrequency);
    pProps->putProperty("OutputFrequency", ParValue);
    pProps->putProperty("OutputTimeUnit", pDefaults->OutputTimeUnit);
    pProps->putProperty("SimulationRunType", pDefaults->SimulationRunType);
    sprintf(ParValue, "%d", pDefaults->NumberOfPeriods);
    pProps->putProperty("NumberOfPeriods", ParValue);
    for (i = 0; i < pDefaults->NumberOfPeriods; i++) {
        sprintf(ParName, "StartPeriod_%d", i);
        sprintf(ParValue, "%ld", pDefaults->period[i].start);
        pProps->putProperty(ParName, ParValue);
        sprintf(ParName, "FinishPeriod_%d", i);
        sprintf(ParValue, "%ld", pDefaults->period[i].finish);
        pProps->putProperty(ParName, ParValue);
        sprintf(ParName, "CyclePeriod_%d", i);
        sprintf(ParValue, "%ld", pDefaults->period[i].cycle);
        pProps->putProperty(ParName, ParValue);
        sprintf(ParName, "UnitCycle_%d", i);
        sprintf(ParValue, "%d", pDefaults->period[i].unit);
        pProps->putProperty(ParName, ParValue);
        sprintf(ParName, "FirstFileIndex_%d", i);
        sprintf(ParValue, "%d", pDefaults->period[i].fileIndex);
        pProps->putProperty(ParName, ParValue);
    }

    if (path == NULL || strlen(path) == 0)
        pProps->saveProperties(fileName, pDefaults->PathName);
    else
        pProps->saveProperties(fileName, path);
    return true;
}
//---------------------------------------------------------------------------

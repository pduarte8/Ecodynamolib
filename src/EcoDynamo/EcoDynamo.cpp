/**
 * EcoDynamo.cpp
 * 
 * - implement EcoDynamo functions:
 * 
 *   Public:
 *     	EcoDynamo(string pathName)
 *     	~EcoDynamo()
 * 		void setModelName(string modelName)
 * 		void setOutputFileName(string fileName)
 * 		void setAgentName(string agentName)
 * 		void setPortNumber(string portNumber)
 *     	int getModelState()
 * 	   	void runModel()
 * 
 *   Private:
 *     freeMemoryEDC(Queue* pQueue)
 *     deleteQueue(Queue* &pQueue)
 * 	   initResources()
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "EcoDynamo.h"
#include "modelState.h"


//---------------------------------------------------------------------------
//
// EcoDynamo constructor
//
EcoDynamo::EcoDynamo(char* pathName) 
{
	if (strlen(pathName) > 0)
        strcpy(defaultModelPath, pathName);
    else 
        defaultModelPath[0] = '\0';

	initResources();
}
//---------------------------------------------------------------------------

EcoDynamo::~EcoDynamo() {
    if (pProperties != NULL)
		delete pProperties;
	if (pEDCQueue != NULL)
		delete pEDCQueue;
}
//---------------------------------------------------------------------------

void EcoDynamo::initResources() {
    defaultModelName[0] = '\0';
    domainFileName[0] = '\0';
    portNumber = 45000;
    logThreadsEnabled = false;
    debugMessagesEnabled = false;

    pEDCQueue = new Queue();
    memset(&defaultsRecord, 0, sizeof(DefaultsRecord));
    MyPEcoDynClass = NULL; 	/* no initial TEcoDynClass */
    varsRecord = NULL;  	/* initially without vars */
    
    nrOfSteps = step = 0;
    modelState = STM_IDLE;

    outRegister.outputFileName[0] = '\0';
    outRegister.logFileName[0] = '\0';
    outRegister.regsBoxFileName[0] = '\0';
    outRegister.MeanValuesSamplesNumber = 0;
	outRegister.HDFSamplesNumber = 0;
	memset(&outRegister.hdfOutputWater_record, 0, sizeof(HDFOutput_record));
	memset(&outRegister.hdfOutputIce_record, 0, sizeof(HDFOutput_record));
	outRegister.defaultsRecord = &defaultsRecord;

    pProperties = new Properties();
    //
    // read configuration from file 
    //   - it can be overwritten by command line options
    //
    if (!pProperties->readProperties("configuration.cfg", defaultModelPath))
        return;

	char value[128];

    if (pProperties->getProperty("name", value) != -1)
        setModelName(value);
    if (pProperties->getProperty("outfile", value) != -1)
		setOutputFileName(value);
    if (pProperties->getProperty("agent", value) != -1)
		setAgentName(value);
    if (pProperties->getProperty("port", value) != -1)
		setPortNumber(atoi(value));
    if (pProperties->getProperty("logthreads", value) != -1)
		setLogThreads(true);
    if (pProperties->getProperty("debug", value) != -1)
		setDebugMessages(true);
    if (pProperties->getProperty("logsteps", value) != -1)
		setLogFile(value);
    if (pProperties->getProperty("subdomain", value) != -1)
		setDomainFile(value);
    if (pProperties->getProperty("outboxes", value) != -1)
		setRegsBoxFileName(value);
    if (pProperties->getProperty("meanvalues", value) != -1)
		setRegisterMeanValues(atoi(value));
    if (pProperties->getProperty("hdfsamples", value) != -1)
		setHDFSamplesValue(atoi(value));
}
//---------------------------------------------------------------------------

int EcoDynamo::getModelState() {
    return modelState;
}
//---------------------------------------------------------------------------

void EcoDynamo::runModel() {
	startRunning(false, 0, false);  // no step mode, 0 steps, local command
}
//---------------------------------------------------------------------------

void EcoDynamo::setModelName(char* modelName) {
	if (strlen(modelName) > 0)
        strcpy(defaultModelName, modelName);
}
//---------------------------------------------------------------------------

void EcoDynamo::setOutputFileName(char* fileName) {
	if (strlen(fileName) > 0)
        strcpy(outRegister.outputFileName, fileName);
}
//---------------------------------------------------------------------------

void EcoDynamo::setLogFile(char* fileName) {
	if (strlen(fileName) > 0)
        strcpy(outRegister.logFileName, fileName);
}
//---------------------------------------------------------------------------

void EcoDynamo::setDomainFile(char* fileName) {
	if (strlen(fileName) > 0)
        strcpy(domainFileName, fileName);
}
//---------------------------------------------------------------------------

void EcoDynamo::setAgentName(char* agentName) {
	if (strlen(agentName) > 0)
        strcpy(this->agentName, agentName);
}
//---------------------------------------------------------------------------

void EcoDynamo::setPortNumber(int portNumber) {
	if (portNumber > 1024 && portNumber < 64*1024)
        this->portNumber = portNumber;
}
//---------------------------------------------------------------------------

void EcoDynamo::setLogThreads(bool state) {
	logThreadsEnabled = state;
}
//---------------------------------------------------------------------------

void EcoDynamo::setDebugMessages(bool state) {
	debugMessagesEnabled = state;
}
//---------------------------------------------------------------------------

void EcoDynamo::setRegsBoxFileName(char* fileName) {
	if (strlen(fileName) > 0)
        strcpy(outRegister.regsBoxFileName, fileName);
}
//---------------------------------------------------------------------------

void EcoDynamo::setRegisterMeanValues(int value) {
	if (value >= 0)
		outRegister.MeanValuesSamplesNumber = value;
}
//---------------------------------------------------------------------------

void EcoDynamo::setHDFSamplesValue(int value) {
	if (value >= 0)
		outRegister.HDFSamplesNumber = value;
}
//---------------------------------------------------------------------------
/*
 * Free resources related with TEcoDynClass classes
 *
 * 2005.07.11 (AP) - first release
 */
//---------------------------------------------------------------------------
void EcoDynamo::freeMemoryEDC(Queue* pQueue)
{
  int i;
  TEcoDynClass* pEDC;

    while (pQueue->size() > 0) {
        pEDC = (TEcoDynClass*)pQueue->RemoveObject(0);
        try {
            delete pEDC;     // AP, 2006.07.20
        }
        catch (exception &exc)
        {
            cerr << "EcoDynamo::freeMemoryEDC -(" << pEDC->GetEcoDynClassName()
					<< ") - " << exc.what() << endl;
        }
    }
}
//---------------------------------------------------------------------------
/*
 * delete queue allocated resources
 *
 * 2007.05.11 (AP)
 */
//---------------------------------------------------------------------------
void EcoDynamo::deleteQueue(Queue* &pQueue)
{
    try {
        delete pQueue;
    }
    catch (exception &exc)
    {
        cerr << "EcoDynamo::deleteQueue - " << exc.what() << endl;
    }
}
//---------------------------------------------------------------------------

void EcoDynamo::printDefaults()
{    
  DefaultsRecord* pDefaults = &defaultsRecord;
  int i;
  
    cout << "\n\t Default Values\n " << endl;
    cout << "\tEcosystemName: " << pDefaults->EcosystemName << endl;
    cout << "\tPathName: " << pDefaults->PathName << endl;
    cout << "\tPrefixName: " << pDefaults->PrefixName << endl;
    cout << "\tEcosystemType: " << pDefaults->EcosystemType << endl;

    cout << "\tNrClassesSelected: " << pDefaults->nrClassesSelected << endl;
	for (i = 0; i < pDefaults->nrClassesSelected; i++) {
		cout << "\tSelected_" << i << ": " << pDefaults->SelectedClasses[i] << endl;
	}
    cout << "\tNrClassesAvailable: " << pDefaults->nrClassesAvailable << endl;
	for (i = 0; i < pDefaults->nrClassesAvailable; i++) {
		cout << "\tAvailable_" << i << ": " << pDefaults->AvailableClasses[i] << endl;
	}

    cout << "\tNrVariablesAvailable: " << pDefaults->nrVariablesAvailable << endl;
	for (i = 0; i < pDefaults->nrVariablesAvailable; i++) {
		cout << "\tAvailableVars_" << i << ": " << pDefaults->AvailableVariables[i] << endl;
	}
    cout << "\tNrVariablesSelectedFileOutput: " << pDefaults->nrVariablesSelectedFileOutput << endl;
	for (i = 0; i < pDefaults->nrVariablesSelectedFileOutput; i++) {
		cout << "\tSelectedVarsFileOutput_" << i << ": " << pDefaults->SelectedVariablesFileOutput[i] << endl;
	}
    cout << "\tNrVariablesSelectedGraphOutput: " << pDefaults->nrVariablesSelectedGraphOutput << endl;
	for (i = 0; i < pDefaults->nrVariablesSelectedGraphOutput; i++) {
		cout << "\tSelectedVarsGraphOutput_" << i << ": " << pDefaults->SelectedVariablesGraphOutput[i] << endl;
	}

    cout << "\tNrVariablesSelectedTableOutput: " << pDefaults->nrVariablesSelectedTableOutput << endl;
	for (i = 0; i < pDefaults->nrVariablesSelectedTableOutput; i++) {
		cout << "\tSelectedVarsTableOutput_" << i<< ": " << pDefaults->SelectedVariablesTableOutput[i] << endl;
	}

    cout << "\tStartTime: " << pDefaults->StartTime << endl;
    cout << "\tFinishTime: " << pDefaults->FinishTime << endl;
    cout << "\tTimeStep: " << pDefaults->TimeStep << endl;
    cout << "\tTimeUnit: " << pDefaults->TimeUnit << endl;
    cout << "\tRunMode: " << pDefaults->RunMode << endl;
    cout << "\tIntegration: " << pDefaults->Integration << endl;
    cout << "\tOutputStartTime: " << pDefaults->OutputStartTime << endl;
    cout << "\tOutputFinishTime: " << pDefaults->OutputFinishTime << endl;
    cout << "\tOutputFrequency: " << pDefaults-> OutputFrequency << endl;
    cout << "\tOutputTimeUnit: "<< pDefaults->OutputTimeUnit << endl;
    cout << "\tSimulationRunType: " << pDefaults->SimulationRunType << endl;
    cout << "\tNumberOfPeriods: " << pDefaults->NumberOfPeriods << endl;
	for (i = 0; i < pDefaults->NumberOfPeriods; i++) {
		cout << "\tStartPeriod_" << i << ": " << pDefaults->period[i].start << endl;
		cout << "\tFinishPeriod_" << i << ": " << pDefaults->period[i].finish << endl;
		cout << "\tCyclePeriod_" << i << ": " << pDefaults->period[i].cycle << endl;
		cout << "\tUnitCycle_" << i << ": " << pDefaults->period[i].unit << endl;
		cout << "\tFirstFileIndex_" << i << ": " << pDefaults->period[i].fileIndex << endl;
	}
}
//---------------------------------------------------------------------------

void EcoDynamo::Debugger(int debug) {
	if (debugMessages())
		cerr << "Debugger: " << debug << endl;
}
//---------------------------------------------------------------------------

void EcoDynamo::Debugger(unsigned debug) {
	if (debugMessages())
		cerr << "Debugger: " << debug << endl;
}
//---------------------------------------------------------------------------

void EcoDynamo::Debugger(long debug) {
	if (debugMessages())
		cerr << "Debugger: " << debug << endl;
}
//---------------------------------------------------------------------------

void EcoDynamo::Debugger(float debug) {
	if (debugMessages())
		cerr << "Debugger: " << debug << endl;
}
//---------------------------------------------------------------------------

void EcoDynamo::Debugger(char* debug) {
	if (debugMessages())
		cerr << "Debugger: " << debug << endl;
}
//---------------------------------------------------------------------------

void EcoDynamo::Debugger(string debug) {
	if (debugMessages())
		cerr << "Debugger: " << debug << endl;
}
//---------------------------------------------------------------------------

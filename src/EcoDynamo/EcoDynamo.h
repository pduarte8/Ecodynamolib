/***
 *  header file for EcoDynamo class
 */

#ifndef _ECODYNAMO_H_
#define _ECODYNAMO_H_

#include <string>
using namespace std;

#ifdef __BORLANDC__

	#include <syncobjs.hpp>
	#include "RunThread.h"
#else  // __BORLANDC__

	#include <pthread.h>
#endif  // __BORLANDC__

#include "globals.h"
#include "EcoDynClass.h"
#include "Properties.h"
#include "Queue.h"
#include "iodll.h"
#include "Log.h"
#include "Parser.h"
#include "outputResults.h"

typedef struct {
	bool stepMode;
	int nSteps;
	bool agentCmd;
	bool logThreads;
} RUN_STRUCT;

/**
 * base class for EcoDynamo project
 */
class EcoDynamo {
public:
    //   === EcoDynamo.cpp ===
	/**
	 * EcoDynamo default constructor
	 */
	EcoDynamo();
	/**
	 * EcoDynamo constructor
	 */
	EcoDynamo(char* pathName);
	/**
	 * EcoDynamo destructor
	 */
	~EcoDynamo();
    /**
     * Run model - start simulation
     */
    void runModel();

	/**
	 * Set configuration parameters
	 */
	void setModelName(char* modelName);
	void setOutputFileName(char* fileName);
	void setLogFile(char* fileName);
	void setDomainFile(char* fileName);
	void setAgentName(char* agentName);
	void setPortNumber(int portNumber);
	void setLogThreads(bool state);
	void setDebugMessages(bool state);
	void setRegsBoxFileName(char* fileName);
	void setRegisterMeanValues(int value);
	void setHDFSamplesValue(int value);

	/**
	 * Test function
	 */
	void printDefaults();

	/**
     * Get model state
     * @return model state in simulation
     */
    int getModelState();

	void Debugger(int debug);
	void Debugger(unsigned debug);
	void Debugger(long debug);
	void Debugger(float debug);
	void Debugger(char* debug);
	void Debugger(string debug);

    //   === MainProperties.cpp ===
	/**
     * Read properties of the model
     * @return true if properties read; false if an error occurs
     */
    bool readParameters(char* fileName = NULL);
	/**
     * Save properties of the model
     * @return true if properties saved; false if an error occurs
     */
    bool saveParameters(char* fileName = NULL);

    //   === MainRun.cpp ===
	/**
     * Change model state
     * @arg newState new state of simulation
     * @arg rTime running time of simulation
     */
    void changeModelState(int newState, double rTime);

//==========================================
//
//  interface with Symbioses
//
//==========================================
	TEcoDynClass *init();
	void next();
	void finalize();
//==========================================

private:
    //   === EcoDynamo.cpp ===
    void initResources();
	void freeMemoryEDC(Queue* pQueue);
	void deleteQueue(Queue* &pQueue);

    //   === MainProperties.cpp ===
	/**
     * Read parameters of the model from "EcoDynamo.properties" file through Properties object
     * @arg pProps address of Properties object
     * @arg pDefaults address of structure to save model parameters
     * @arg path model directory
     * @arg fileName name of the file with properties
     * @return true if properties read; false if an error occurs
     */
    bool readParameters(Properties *pProps, DefaultsRecord *pDefaults, char* path, char* fileName);
    /**
     * Save parameters of the model in "EcoDynamo.properties" file through Properties object
     * @arg pProps address of Properties object
     * @arg pDefaults address of structure with model parameters
     * @arg path model directory
     * @arg fileName name of the file to save properties
     * @return true if properties saved; false if an error occurs
     */
    bool saveParameters(Properties *pProps, DefaultsRecord *pDefaults, char* path, char* fileName);

    //   === MainClasses.cpp ===
    /**
     * Initialize classes to run the model
     */
	void getVariables(Queue* pEDCQueue, char* className, TEcoDynClass::VarsRecord* pVars);

    //   === MainRun.cpp ===
	void startRunning(bool stepMode, int nSteps, bool agentCmd);
	void initializeClasses(bool agentCmd);
	void addAvailableVariables(TEcoDynClass* pEDC, TEcoDynClass::VarsRecord* pVars);
    void threadRunning(bool stepMode, int nSteps);
    bool logThreads();
	bool debugMessages();
	void fillSubDomain();
	bool readPointsFile(char* fileName, int& size, PointBox* &filePoints);

/** ====================================
 *                fields...
 *  ====================================
 */
public:
#ifdef __BORLANDC__

	RunThread runThread;
#else  // BORLANDC__

	pthread_t runThreadX;
#endif  // __BORLANDC__

	RUN_STRUCT runStruct;

    DefaultsRecord defaultsRecord;
    TEcoDynClass* MyPEcoDynClass;
    Queue *pEDCQueue;
	OutputResults* pResults;

private:
	OUTPUT_REGISTER outRegister;	// values are initialized here
    char defaultModelPath[256];    	// value passed by arg (-path)
    char defaultModelName[64];    	// value passed by arg (-name)
	char domainFileName[64];		// value passed by arg (-subdomain)
	char agentName[64];				// value passed by arg (-agent)
	int portNumber;					// value passed by arg (-port)
	bool logThreadsEnabled;			// value passed by arg (-logthreads)
	bool debugMessagesEnabled;		// value passed by arg (-debug)

    int nrOfSteps;
    int step;
    int modelState;      // values defined in "modelState.h"
    long modelTime;     // AP, 2005.08.02

    Properties* pProperties;

    SubDomain subDomain;
    TEcoDynClass::VarsRecord* varsRecord;

    //   === MainRun.cpp ===
    time_t startRunningTime;
    time_t startRunningTimeStat;
    time_t elapsedTime;
    time_t lastElapsedTime;
    time_t estimatedTime;
    double simulationTime;
    double lastSimulationTime;
    int lastStep;

    //   === EcoDynamoSymbioses.cpp
    double MyTime;
    double TimeSpent;
	bool firstRegister;

};


#endif  // _ECODYNAMO_H_

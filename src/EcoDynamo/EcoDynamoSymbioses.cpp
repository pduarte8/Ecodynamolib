/*
 * EcoDynamoSymbioses.cpp
 *
 * - implement EcoDynamo functions that interface with Symbioses:
 *
 *   Public:
 *     	EcoDynamo()
 * 		void init()
 * 		void next()
 * 		void finalize()
 *
 * Copyright 2014 - A.Pereira / P.Duarte, Norsk Polarinstitutt
 */


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "EcoDynamo.h"

//===========================================================================
//  interface with Symbioses
//---------------------------------------------------------------------------

//
// EcoDynamo default constructor
//
EcoDynamo::EcoDynamo()
{
	strcpy(defaultModelPath, "Model");
	initResources();
}
//---------------------------------------------------------------------------

TEcoDynClass *EcoDynamo::init()
{
  TEcoDynClass* pEDC;
  int i;

	if (!readParameters()) {
        cerr << "\n\t[2] Error reading model properties." << endl;
        cerr << "\t     Please correct EcoDynamo.properties file in " << defaultModelPath
                << " directory \n\n " << endl;
	}

    //~ cout << "EcoDynamo::init 1 - define OutputResult/initialize model" << endl;
    pResults = new OutputResults(&outRegister);
    initializeClasses(false);

    //~ cout << "EcoDynamo::init 2 - fill subdomain" << endl;
    fillSubDomain();

    for (i = 0; i < pEDCQueue->size(); i++) {
        pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(i);
        pEDC->enableDebug(debugMessages());
    }

    //~ cout << "EcoDynamo::init 4 - init output system" << endl;
    //
    // Init OUTPUT FILE
    //
    pResults->SetupOutput();
    MyPEcoDynClass->SetTime(defaultsRecord.StartTime);
    MyPEcoDynClass->SetStartTime(defaultsRecord.StartTime);

	runStruct.stepMode = false;
	runStruct.nSteps = 0;
	runStruct.agentCmd = false;
	runStruct.logThreads = logThreads();

	// initialization imported from RunThread
    for (int i = 0; i < pEDCQueue->size(); i++) {
        pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(i);
        pEDC->ResetValues();
    }
	MyTime = defaultsRecord.StartTime;
	TimeSpent = 0.0;
	firstRegister = true;

	cout << "EcoDynamo::init done" << endl;
    return pEDC;

}
//---------------------------------------------------------------------------

void EcoDynamo::next()
{
  //---- Place execute code here ----
  TEcoDynClass* pEDC = NULL;

    if ((MyTime >= defaultsRecord.OutputStartTime)
            && (MyTime <= defaultsRecord.OutputFinishTime)) {

        if (firstRegister) {        // force first output register
            firstRegister = false;
        }
        pResults->UpdateOutput(MyTime, MyPEcoDynClass->GetCurrTime());
        TimeSpent = 0.0;
    }

	for (int i = 0; i < pEDCQueue->size(); i++) {
		pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(i);
		pEDC->Go();
	}

	// "Integrate" - invoked after solved all sources and sinks
	for (int i = 0; i < pEDCQueue->size(); i++) {
		pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(i);
		pEDC->Integrate();
	}

	// "Reinitialize"
	for (int i = 0; i < pEDCQueue->size(); i++) {
		pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(i);
		pEDC->Reinitialize();
	}

	// get next step time
	double stepTime = MyPEcoDynClass->GetTimeStep();

	MyPEcoDynClass->SetTime(MyTime + stepTime);
	MyPEcoDynClass->SetMyTime(MyTime + stepTime);

	// adjust time for the next iteration - AP, 2005.07.21 moved to here
	MyTime += stepTime;

	if ((MyTime >= defaultsRecord.OutputStartTime)
			&& (MyTime <= defaultsRecord.OutputFinishTime)) {

		if (firstRegister) {        // force first output register
			firstRegister = false;
			TimeSpent = defaultsRecord.OutputFrequency;
		}
		TimeSpent += stepTime;

		if (TimeSpent >= defaultsRecord.OutputFrequency)
		{
			pResults->UpdateOutput(MyTime, MyPEcoDynClass->GetCurrTime());
			TimeSpent = 0.0;
		}
	}
}
//---------------------------------------------------------------------------

void EcoDynamo::finalize()
{
}
//---------------------------------------------------------------------------
//  interface with Symbioses
//===========================================================================

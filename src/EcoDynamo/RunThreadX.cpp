//---------------------------------------------------------------------------
#include <iostream>
#include <pthread.h>
#include <ctime>

#include "RunThreadX.h"

//---------------------------------------------------------------------------
//
//     THIS FILE IS USED ONLY IN LINUX
//
//---------------------------------------------------------------------------

/**
 * diff time in nanoseconds - downloaded from Internet (http://www.guyrutenberg.com/)
 */
timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
//---------------------------------------------------------------------------

void *execute(void *arg)
{
    //---- Place thread code here ----
  EcoDynamo* eco = (EcoDynamo*)arg;
  
  TEcoDynClass* pEDC = NULL;
  double TimeSpent = 0.0;
  bool firstRegister = true;
  bool runRegister = true;
  char status[128];
  double stepTime;
  timespec t, start, end;
  int threadState;
  int stepNumber;
  int countCycles;
  
  //try {
	cout << "RunThreadX::execute" << endl;

    double MyTime = eco->defaultsRecord.StartTime;

    for (int i = 0; i < eco->pEDCQueue->size(); i++) {
        pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);
        pEDC->ResetValues();
    }

    /*
     * AP, 2008.02.06 (UFP) - output initial data?
     */
    if ((MyTime >= eco->defaultsRecord.OutputStartTime)
            && (MyTime <= eco->defaultsRecord.OutputFinishTime)) {

        if (firstRegister) {        // force first output register
            firstRegister = false;
        }
        eco->pResults->UpdateOutput(MyTime, eco->MyPEcoDynClass->GetCurrTime());
        TimeSpent = 0.0;
    }
    //eco->updateRemote(MyTime, eco->MyPEcoDynClass->GetCurrTime());
    /* ... */

	countCycles = 0;
    while (MyTime < eco->defaultsRecord.FinishTime)
    {
        eco->changeModelState(STM_RUN, MyTime);
        //if (runRegister)
        //{
            //runRegister = false;
            //eco->sendRunningMessage();
        //}

		// some feedback to terminal 
		if ((++countCycles % 10)== 0)
		{
			if ((countCycles % 100)== 0)
				cout << "_";
			cout << "-";
			cout.flush();
		}
        // "Go"
        if (!eco->runStruct.logThreads) {
            for (int i = 0; i < eco->pEDCQueue->size(); i++) {
                pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);
                pEDC->Go();
            }

            // "Integrate"
            // O integrate só deve ser invocado depois de resolvidas todas as fontes e sumidouros de cada classe
            for (int i = 0; i < eco->pEDCQueue->size(); i++) {
                pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);
                pEDC->Integrate();
            }

            // "Reinitialize"
            for (int i = 0; i < eco->pEDCQueue->size(); i++) {
                pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);
                pEDC->Reinitialize();
            }
        }
        else {      // AP, 20050510
            clock_gettime(CLOCK_REALTIME, &t);
            sprintf(status, " GO STEP - time is: %ld:%02ld:%02ld.%09ld",
               (t.tv_sec%86400)/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
            cout << status << endl;
                
            for (int i = 0; i < eco->pEDCQueue->size(); i++) {
                pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);

				cout << "Go " << pEDC->GetEcoDynClassName() << endl;
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
                pEDC->Go();
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
                t = diff(start, end);
                sprintf(status, "\tGo %s - time spent: %ld:%02ld:%02ld.%09ld",
                        pEDC->GetEcoDynClassName(),
                        t.tv_sec/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
                cout << status << endl;
            }

            clock_gettime(CLOCK_REALTIME, &t);
            sprintf(status, " INTEGRATE STEP - time is: %ld:%02ld:%02ld.%09ld",
               (t.tv_sec%86400)/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
            cout << status << endl;

            // "Integrate"
            // O integrate só deve ser invocado depois de resolvidas todas as fontes e sumidouros de cada classe
            for (int i = 0; i < eco->pEDCQueue->size(); i++) {
                pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);

				cout << "Integrate " << pEDC->GetEcoDynClassName() << endl;
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
                pEDC->Integrate();
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
                t = diff(start, end);
                sprintf(status, "\tIntegrate %s - time spent: %ld:%02ld:%02ld.%09ld",
                        pEDC->GetEcoDynClassName(),
                        t.tv_sec/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
                cout << status << endl;
            }

            clock_gettime(CLOCK_REALTIME, &t);
            sprintf(status, " REINITIALIZE STEP - time is: %ld:%02ld:%02ld.%09ld",
               (t.tv_sec%86400)/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
            cout << status << endl;

            // "Reinitialize"
            for (int i = 0; i < eco->pEDCQueue->size(); i++) {
                pEDC =(TEcoDynClass*)eco->pEDCQueue->RetrieveObject(i);

				cout << "Reinitialize " << pEDC->GetEcoDynClassName() << endl;
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
                pEDC->Reinitialize();
                clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
                t = diff(start, end);
                sprintf(status, "\tReinitialize %s - time spent: %ld:%02ld:%02ld.%09ld",
                        pEDC->GetEcoDynClassName(),
                        t.tv_sec/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
                cout << status << endl;
            }
            clock_gettime(CLOCK_REALTIME, &t);
            sprintf(status, " END LOOP - time is: %2ld:%02ld:%02ld.%09ld",
               (t.tv_sec%86400)/3600, (t.tv_sec%3600)/60, t.tv_sec%60, t.tv_nsec);
            cout << status << endl;
        }

        // get next step time
        stepTime = eco->MyPEcoDynClass->GetTimeStep();

		eco->MyPEcoDynClass->SetTime(MyTime + stepTime);
		eco->MyPEcoDynClass->SetMyTime(MyTime + stepTime);

        // adjust time for the next iteration - AP, 2005.07.21 moved to here
        MyTime += stepTime;

        if ((MyTime >= eco->defaultsRecord.OutputStartTime)
                && (MyTime <= eco->defaultsRecord.OutputFinishTime)) {

            if (firstRegister) {        // force first output register
                firstRegister = false;
                TimeSpent = eco->defaultsRecord.OutputFrequency;
            }
            TimeSpent += stepTime;

            if (TimeSpent >= eco->defaultsRecord.OutputFrequency)
            {
                eco->pResults->UpdateOutput(MyTime, eco->MyPEcoDynClass->GetCurrTime());
                TimeSpent = 0.0;
            }
        }
        //eco->updateRemote(MyTime, eco->MyPEcoDynClass->GetCurrTime());
        //eco->logMessages();

        //eco->getModelState();
        threadState = eco->getModelState();

        if (MyTime > eco->defaultsRecord.FinishTime)
            break;
        else if (threadState == STM_STOP) {
            eco->changeModelState(STM_STOP, MyTime);
            //eco->sendStoppedMessage();
            break;
        }
        else if (threadState == STM_PAUSE) {
            eco->changeModelState(STM_PAUSE, MyTime);
            //eco->sendPausedMessage();
            runRegister = true;

            //Suspend();
            stepNumber = 0;
            threadState = eco->getModelState();

            if (threadState == STM_STOP) {
                //eco->sendStoppedMessage();
                break;
            }
        }
        else if (eco->runStruct.stepMode) {
            stepNumber++;
            if (stepNumber == eco->runStruct.nSteps) {
                eco->changeModelState(STM_PAUSE, MyTime);
                //eco->sendEndStepMessage();
                runRegister = true;

                //Suspend();
                stepNumber = 0;
                threadState = eco->getModelState();

                if (threadState == STM_STOP) {
                    //eco->sendStoppedMessage();
                    break;
                }
            }
        }
    }
     
    if (threadState == STM_RUN) {
        eco->changeModelState(STM_DONE, MyTime);
        //eco->sendEndSimulationMessage();
		cout << "\nRunThreadX::done" << endl;
    }
}
//---------------------------------------------------------------------------

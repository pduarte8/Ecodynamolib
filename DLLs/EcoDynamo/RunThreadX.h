//---------------------------------------------------------------------------
#ifndef RunThreadXH
#define RunThreadXH
//---------------------------------------------------------------------------
#include <pthread.h>
#include "Queue.h"
#include "EcoDynClass.h"
#include "EcoDynamo.h"
#include "modelState.h"

//---------------------------------------------------------------------------
//
//     THIS FILE IS USED ONLY IN LINUX
//
//---------------------------------------------------------------------------

void *execute(void * arg);	// entry routine for RunThreadX::myThread

/* ======================= not yet used - AP 2013/11  ================
class RunThreadX
{
public:
    EcoDynamo* pMain;
    DefaultsRecord* pRecord;
    Queue* pQueue;
    int threadState;
    double runTime;
    char text[128];
    Queue* pThreads;
    pthread_t myThread;

    void DoRefreshState(void);
    void DoGetModelState(void);
    void addDebug(char* status);
    void buildThreads(void);
    void waitForThreads(int level);
    void kickThreads(void);
    void destroyThreads(void);

    void refreshState(int state, double rTime=0);
    void getModelState();
    
    int* evCount;
    bool stepMode;
    int nSteps;
    int stepNumber;
    RunThreadX(EcoDynamo* mForm, DefaultsRecord* wRecord,
            Queue* pEDCQueue, bool bStep, int steps);
    ~RunThreadX();
    void finalize();
    void resume();
};
//---------------------------------------------------------------------------
class EDCThread
{
private:
    EcoDynamo* pMain;
    RunThreadX* pRun;
    TEcoDynClass* pClass;
    int index;
    char text[128];
    
protected:
    void execute(void);
    void wait(int value);
    void signal(int value);
    void waitForEnableReinitialize(void);
    void addDebug(char* status);
	void cycleMultiThread(void);
	void cycleSingleThread(void);
	
public:
    EDCThread(EcoDynamo* mForm, RunThreadX* pThread,
            TEcoDynClass* pEDC, int ndx);
    ~EDCThread();
};
//---------------------------------------------------------------------------
class ContinuityThread
{
private:
    EcoDynamo* pMain;
    RunThreadX* pRun;
    EDCThread* pEDCThread;
    TEcoDynClass* pClass;
    int index;
    char text[128];
    
protected:
    void execute(void);
    void wait(int value);
    void waitForContinuity(void);
    void waitForEnableReinitialize(void);
    
public:
    ContinuityThread(EcoDynamo* mForm, RunThreadX* pThread,
            EDCThread* pEDCThr, TEcoDynClass* pEDC, int ndx);
    ~ContinuityThread();
};
//---------------------------------------------------------------------------
* 
*/
#endif // RunThreadXH

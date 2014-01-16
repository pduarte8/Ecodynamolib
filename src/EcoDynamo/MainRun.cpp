/**
 * MainRun.cpp
 * 
 * - implement EcoDynamo functions:
 * 
 *   Private:
 *     void startRunning(bool stepMode, int nSteps, bool agentCmd)
 * 
 * Copyright 2013 - A.Pereira
 */


#include <iostream>
#include <fstream>

using namespace std;

#include "EcoDynamo.h"
#include "modelState.h"
#include "RunThreadX.h"


//---------------------------------------------------------------------------

void EcoDynamo::startRunning(bool stepMode, int nSteps, bool agentCmd) {
  TEcoDynClass* pEDC;
  int i;

    cout << "EcoDynamo::startRunning 1 - define OutputResult/initialize model" << endl;
    pResults = new OutputResults(&outRegister);
    initializeClasses(agentCmd);

    cout << "EcoDynamo::startRunning 2 - fill subdomain" << endl;
    fillSubDomain();
	
    cout << "EcoDynamo::startRunning 3 - change state to STM_RUN" << endl;
    //
    // the following is done to avoid asynchronism between RunThreads
    //
    changeModelState(STM_RUN, defaultsRecord.StartTime);

    for (i = 0; i < pEDCQueue->size(); i++) {
        pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(i);
        pEDC->enableDebug(debugMessages());
        //
        // verify LOG in step 1
        //if (stepInLog(1)) {
            //pEDC->enableLog(true);
        //}
    }

    cout << "EcoDynamo::startRunning 4 - init output system" << endl;
    //
    // Init OUTPUT FILE
    //
    pResults->SetupOutput();
    MyPEcoDynClass->SetTime(defaultsRecord.StartTime);
    MyPEcoDynClass->SetStartTime(defaultsRecord.StartTime);

    //
    // clear remote registers
    //
/*    for (i = 1; i < MAX_AGENTS; i++) {
        pAgent = ecdp->getAgentTableAtPosition(i);
        if (pAgent == NULL)           // no more agents
            break;
        remoteRegisterTime[pAgent->id].MyNumberOfRemoteRegister = 0;
        remoteRegisterTime[pAgent->id].regTime =
                remoteRegisterTime[pAgent->id].startTime;
    }*/

    cout << "EcoDynamo::startRunning 5 - starting RunThreadX" << endl;
    try {
        // 2008.08.28, AP - save last configuration code
/*        if (makeBackup && (agentCmd || !stepMode)) {
            SaveLastConfiguration();
        }*/
        
        runStruct.stepMode = stepMode;
        runStruct.nSteps = nSteps;
        runStruct.agentCmd = agentCmd;
        runStruct.logThreads = logThreads();
        
		int rcode = pthread_create(&runThreadX, NULL, &execute, (void*)this);
        //pRun = new RunThreadX(this, &defaultsRecord, pEDCQueue, stepMode, nSteps);
        cout << "EcoDynamo::startRunning5a - Thread 'execute' created" << endl;
        
    }
    catch (exception &exc) {
        cerr << "EcoDynamo::startRunning - " << exc.what() << endl;
    }
 
	cout << "EcoDynamo::startRunning 6 - waiting for RunThreadX finish" << endl;
	pthread_join(runThreadX, NULL);
	cout << "EcoDynamo::startRunning 6a - exit" << endl;
 
}
//---------------------------------------------------------------------------

void EcoDynamo::initializeClasses(bool agentCmd)
{
	cout << "EcoDynamo::initializeClasses - model [" << defaultsRecord.EcosystemName 
			<< "] " << strlen(defaultsRecord.EcosystemName) << endl;
	
    if (strlen(defaultsRecord.EcosystemName) > 0) {
        cout << "EcoDynamo::initializeClasses - Initialising VarsRecord" << endl;
        /*
         * try to allocate memory
         */
        int retry = 10;
        while (retry) {
            try {
                varsRecord = new TEcoDynClass::VarsRecord[defaultsRecord.nrClassesSelected];
                retry = 0;
            }
            catch (exception &exc) {
                cerr << "EcoDynamo::initializeClasses - " << exc.what() << endl;
                retry--;
            }
        }

        /*
         * model read from scratch (database files)
         */
		cout << "EcoDynamo::initializeClasses - Creating master EcoDynClass" << endl;
		MyPEcoDynClass = new TEcoDynClass(defaultsRecord.TimeStep,
										  defaultsRecord.PathName,
										  defaultsRecord.PrefixName);
		MyPEcoDynClass->setDefaultsRecord(&defaultsRecord);
		MyPEcoDynClass->SetTime(defaultsRecord.StartTime);

		cout << "EcoDynamo::initializeClasses - Initialising morphology" << endl;
		if (MyPEcoDynClass->OpenMorphology()) {

			cout << "EcoDynamo::initializeClasses - Initialising loads" << endl;
			if (!MyPEcoDynClass->OpenLoads()) {
				if (!agentCmd)
					cerr << "EcoDynamo::initializeClasses - Loads file not found." << endl;
			}

			cout << "EcoDynamo::initializeClasses - Initialising losses" << endl;
			if (!MyPEcoDynClass->OpenLosses()) {
				if (!agentCmd)
					cerr << "EcoDynamo::initializeClasses - Losses file not found." << endl;
			}

			cout << "EcoDynamo::initializeClasses - Initialising rivers" << endl;
			if (!MyPEcoDynClass->OpenRivers()) {
				if (!agentCmd)
					cerr << "EcoDynamo::initializeClasses - Rivers file not found." << endl;
			}

			cout << "EcoDynamo::initializeClasses - Initialising sea boundaries" << endl;
			if (!MyPEcoDynClass->OpenSeaBoundaries()) {
				if (!agentCmd)
					cerr << "EcoDynamo::initializeClasses - Sea boundaries file not found." << endl;
			}

			cout << "EcoDynamo::initializeClasses - Initialising sediments" << endl;
			if (!MyPEcoDynClass->OpenSediments()) {
				if (!agentCmd)
					cerr << "EcoDynamo::initializeClasses - Sediments file not found." << endl;
			}
			
			cout << "EcoDynamo::initializeClasses - Initialising ice" << endl;
			if (!MyPEcoDynClass->OpenIce()) {
				if (!agentCmd)
					cerr << "EcoDynamo::initializeClasses - Ice file not found." << endl;
			}

            defaultsRecord.nrVariablesAvailable = 0;
            for (int i = 0; i < defaultsRecord.nrClassesSelected; i++) {
                cout << "EcoDynamo::initializeClasses - Initialising " 
						<< defaultsRecord.SelectedClasses[i] << endl;
                getVariables(pEDCQueue, defaultsRecord.SelectedClasses[i],
                        &varsRecord[i]);
            }
            changeModelState(STM_INIT, defaultsRecord.StartTime);
            cout << "Model initialised - CriticalDepthForLandBoundary " 
            		<< MyPEcoDynClass->GetCriticalDepth() << endl;

            MyPEcoDynClass->setEDCQueue(pEDCQueue);
            outRegister.MyPEcoDynClass = MyPEcoDynClass;
            outRegister.varsRecord = varsRecord;
        }
		else {
			cerr << "\n\n   EcoDynamo::initializeClasses - Morphology file not found." 
					<< endl << "    Please correct and initialise again\n\n" << endl;
			exit(-1);
		}
    }
}
//---------------------------------------------------------------------------

void EcoDynamo::addAvailableVariables(TEcoDynClass* pEDC,
            TEcoDynClass::VarsRecord* pVars)
{
int nVars = pEDC->GetNumberOfVariablesInObject();
int baseIndex = defaultsRecord.nrVariablesAvailable;

	cout << "EcoDynamo::addAvailableVariables - " << pEDC->GetEcoDynClassName() 
			<< " ::nVars=" << nVars << endl;
	
    strcpy(pVars->ClassName, pEDC->GetEcoDynClassName());
    pVars->ObjectPointer = pEDC;
    pVars->nrVariablesAvailable = nVars;
    for (int j = 0; j < nVars; j++) {
        strcpy(defaultsRecord.AvailableVariables[j + baseIndex],
               pEDC->GetAvailableVariables(j));
        strcpy(pVars->AvailableVariables[j],
               defaultsRecord.AvailableVariables[j + baseIndex]);
    }
    defaultsRecord.nrVariablesAvailable += nVars;
}
//---------------------------------------------------------------------------
//
// Method called also inside RunThreadX context.
//
void EcoDynamo::changeModelState(int newState, double rTime)
{
 int oldState = modelState;
 char timeMsg[32];
 struct tm *timeDT;
 char runMsg[256]={0};

    if (modelState != newState) {
        modelState = newState;
        switch (newState) {
            case STM_IDLE:
                strcpy(runMsg, " Idle");
                break;

            case STM_INIT:
                strcpy(runMsg, " Initialised");
                modelTime = defaultsRecord.StartTime;
                break;

            case STM_RUN:
                if (oldState == STM_PAUSE) {
                    startRunningTimeStat = time(NULL) - elapsedTime;  // pause preserve elapsed time

                    if (step < nrOfSteps) {
                        float percentage;

                        percentage = (float)(rTime - defaultsRecord.StartTime)
                            / (defaultsRecord.FinishTime - defaultsRecord.StartTime);
                        sprintf(runMsg, "Percentage completed %.2f (estimated)",
								percentage * 100);

                        ++step;
                    }
                }
                else {
                    startRunningTime = time(NULL);
                    startRunningTimeStat = startRunningTime;
                    elapsedTime = 0;
                    estimatedTime = 0;
                    nrOfSteps = (defaultsRecord.FinishTime - defaultsRecord.StartTime)
                                / defaultsRecord.TimeStep;
                    if ((nrOfSteps * defaultsRecord.TimeStep) <
                            (defaultsRecord.FinishTime - defaultsRecord.StartTime))
                        nrOfSteps++;
                    step = 0;
                    sprintf(runMsg, " Total of %d steps", nrOfSteps);
                }

                modelTime = rTime;
                timeDT = gmtime(&modelTime);
                strftime(timeMsg, 22, DMY_HMS_FORMAT, timeDT);
                sprintf(&runMsg[strlen(runMsg)], "Running - step %d/%d (%s)",
						step, nrOfSteps, timeMsg);
                break;

            case STM_PAUSE:
                modelTime = rTime;
                timeDT = gmtime(&modelTime);
                strftime(timeMsg, 22, DMY_HMS_FORMAT, timeDT);
                sprintf(runMsg, "Paused - step %d/%d (%s)",
						step, nrOfSteps, timeMsg);
                break;

            case STM_STOP:
                modelTime = rTime;
                timeDT = gmtime(&modelTime);
                strftime(timeMsg, 22, DMY_HMS_FORMAT, timeDT);
                sprintf(runMsg, "Stopped - step %d/%d (%s)",
						step, nrOfSteps, timeMsg);

                if (oldState == STM_PAUSE)
                    threadRunning(runStruct.stepMode, runStruct.nSteps);
                lastElapsedTime = 0;
                elapsedTime = 0;
                if (oldState != STM_STOP)
                    pResults->FinalizeOutput();
                break;

            case STM_DONE:
                modelTime = rTime;
                strcpy(runMsg, "Run Complete");
                lastElapsedTime = 0;
                elapsedTime = 0;
                pResults->FinalizeOutput();
                break;

            default: 
				break;
        }
    }
	else if (oldState == STM_RUN) {

		if (step < nrOfSteps) {
			float percentage;

			++step;
			modelTime = rTime;
            timeDT = gmtime(&modelTime);
			strftime(timeMsg, 22, DMY_HMS_FORMAT, timeDT);
			percentage = (float)(rTime - defaultsRecord.StartTime)
				/ (defaultsRecord.FinishTime - defaultsRecord.StartTime);
			sprintf(runMsg, "Running - step %d/%d (%s)\nPercentage completed %.2f (estimated)",
						step, nrOfSteps, timeMsg, percentage * 100);
		}
	}
	if (logThreads())
		cout << runMsg << endl;
}
//---------------------------------------------------------------------------

void EcoDynamo::threadRunning(bool stepMode, int nSteps)
{
    try {
        runStruct.stepMode = stepMode;
        runStruct.nSteps = nSteps;
        //pRun->resume();
    }
    catch (exception &exc) {
        cerr << "EcoDynamo::threadRunning - " << exc.what() << endl;;
    }
}
//---------------------------------------------------------------------------

bool EcoDynamo::logThreads()
{
    return logThreadsEnabled;
}
//---------------------------------------------------------------------------

bool EcoDynamo::debugMessages()
{
    return debugMessagesEnabled;
}
//---------------------------------------------------------------------------

void EcoDynamo::fillSubDomain()
{
  int i, nL, nC, nBoxes;
  int firstBoxInLine, boxNumber, lineIndex, columnIndex;
  PointBox* filePoints;

	// build new SubDomain
	nC = MyPEcoDynClass->GetNumberOfColumns();
	nL = MyPEcoDynClass->GetNumberOfLines();
	nBoxes = nC * nL;

	subDomain.ILine = new int[nC];
	subDomain.FLine = new int[nC];
	subDomain.IColumn = new int[nL];
	subDomain.FColumn = new int[nL];

	if (strlen(domainFileName) > 0
			&& readPointsFile(domainFileName, nBoxes, filePoints)) {
		// sub domain defined in file
        subDomain.FirstLine = nL;
        subDomain.LastLine = -1;
        subDomain.FirstColumn = nC;
        subDomain.LastColumn = -1;
        for (i = 0; i < nC; i++)
        {
            subDomain.ILine[i] = nL;
            subDomain.FLine[i] = -1;
        }
        for (i = 0; i < nL; i++)
        {
            subDomain.IColumn[i] = nC;
            subDomain.FColumn[i] = -1;
        }

        subDomain.BoxNumber = new int[nBoxes];
        subDomain.NumberOfBoxes = nBoxes;
        for (i = 0; i < nBoxes; i++)
        {
            firstBoxInLine = (nL - filePoints[i].Line) * nC;
            if ((firstBoxInLine < 0) || (firstBoxInLine >= nC*nL))
                continue;
            if ((filePoints[i].Column < 1) || (filePoints[i].Column > nC))
                continue;
            
            boxNumber = firstBoxInLine + filePoints[i].Column - 1;
            lineIndex = MyPEcoDynClass->GetLineIndex(boxNumber);
            columnIndex = MyPEcoDynClass->GetColumnIndex(boxNumber);

            // first line in sub domain
            if (lineIndex < subDomain.FirstLine)
                subDomain.FirstLine = lineIndex;
            // last line in sub domain
            if (lineIndex > subDomain.LastLine)
                subDomain.LastLine = lineIndex;
            // first column in sub domain
            if (columnIndex < subDomain.FirstColumn)
                subDomain.FirstColumn = columnIndex;
            // last column in sub domain
            if (columnIndex > subDomain.LastColumn)
                subDomain.LastColumn = columnIndex;

            // first line in 'columnIndex'
            if (lineIndex < subDomain.ILine[columnIndex])
                subDomain.ILine[columnIndex] = lineIndex;
            // last line in 'columnIndex'
            if (lineIndex > subDomain.FLine[columnIndex])
                subDomain.FLine[columnIndex] = lineIndex;
            // first column in 'lineIndex'
            if (columnIndex < subDomain.IColumn[lineIndex])
                subDomain.IColumn[lineIndex] = columnIndex;
            // last column in 'lineIndex'
            if (columnIndex > subDomain.FColumn[lineIndex])
                subDomain.FColumn[lineIndex] = columnIndex;

            subDomain.BoxNumber[i] = boxNumber;
        }
        delete [] filePoints;	// PointBox array can be discarded
	}
	else {
		// sub domain overlaps domain
		
		subDomain.FirstLine = 0;
		subDomain.LastLine = nL - 1;
		subDomain.FirstColumn = 0;
		subDomain.LastColumn = nC - 1;
		for (i = 0; i < nC; i++)
		{
			subDomain.ILine[i] = 0;
			subDomain.FLine[i] = nL - 1;
		}
		for (i = 0; i < nL; i++)
		{
			subDomain.IColumn[i] = 0;
			subDomain.FColumn[i] = nC - 1;
		}

		subDomain.BoxNumber = new int[nBoxes];
		subDomain.NumberOfBoxes = nBoxes;
		for (i = 0; i < nBoxes; i++)
		{
			subDomain.BoxNumber[i] = i;
		}
	}
    MyPEcoDynClass->SetSubDomain(&subDomain);
}
//---------------------------------------------------------------------------

bool EcoDynamo::readPointsFile(char* fileName, int& size, 
		PointBox* &filePoints)
{
    int n;
    int Y, XCol, XLin;
	char fullFileName[512];
	
	sprintf(fullFileName, "%s%s%s", defaultModelPath, 
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

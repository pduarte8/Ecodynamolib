// Man object CPP code file
// @ Antonio Pereira
// Rev. A 2005.11.18
/**
 * NPI work
 * 	20 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
    #include <vcl.h>
    #include <stdio.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>

	using namespace std;
#endif  // __BORLANDC__


#include "ecodyn.rh"
#include "manobjt.h"
#include "iodll.h"

// Farmer man class - PUBLIC methods
//

TFisherMan::TFisherMan(TEcoDynClass* APEcoDynClass, char* className)
							: TMan(APEcoDynClass, className)
{
    BuildFisher();
}

void TFisherMan::BuildFisher()
{
    seedQueue = new Queue();
    inspectQueue = new Queue();
    harvestQueue = new Queue();
    seedingQueue = new Queue();

    int X,Y;
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Fisher");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Fisher variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TFisherMan::BuildFisher - Fisher variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Fisher", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Fisher Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TFisherMan::BuildFisher - Variables: undefined object Fisher Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

// AP, 2011.02.05
bool TFisherMan::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Fisher");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Fisher");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TFisherMan::Inquiry(char* srcName, double &Value,
                            int BoxNumber,
                            char* ParameterName,
                            int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    Value = 1;
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);}


TFisherMan::~TFisherMan()
{
    freeMemory();
}

void TFisherMan::freeMemory()
{
    try {
        for (int i = 0; i < seedQueue->size(); i++) {
            BUF* pBuf = seedQueue->getElementAt(i);
            SEED_DB* pSeed = (SEED_DB*)pBuf->pData;
            delete pSeed->seedRegions;
        }
        for (int i = 0; i < seedingQueue->size(); i++) {
            BUF* pBuf = seedingQueue->getElementAt(i);
            SEED_DB* pSeed = (SEED_DB*)pBuf->pData;
            delete pSeed->seedRegions;
        }
        for (int i = 0; i < harvestQueue->size(); i++) {
            BUF* pBuf = harvestQueue->getElementAt(i);
            HARVEST_DB* pHarvest = (HARVEST_DB*)pBuf->pData;
            delete pHarvest->harvestRegions;
        }
        delete seedQueue;
        delete inspectQueue;
        delete harvestQueue;
        delete seedingQueue;
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
    catch (exception &exc) {
        cerr << "TFisherMan::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

// AP, 2011.02.01
/**
 * find SEED_DB element in seedQueue with same firstDay and lastDay
 */
SEED_DB* TFisherMan::findSeedDB(int firstDay, int lastDay) {

    if (seedQueue->size() > 0) {
        for (int i = seedQueue->size(); i  > 0; i--) {
            BUF* pBuf = seedQueue->getElementAt(i - 1);
            SEED_DB* pSeed = (SEED_DB*)pBuf->pData;
            if ((pSeed->firstDay == firstDay) && (pSeed->lastDay == lastDay))
                return pSeed;
        }
    }
    return NULL;
}

/**
 * find HARVEST_DB element in harvestQueue with same firstDay and lastDay
 */
HARVEST_DB* TFisherMan::findHarvestDB(int firstDay, int lastDay) {

    if (harvestQueue->size() > 0) {
        for (int i = harvestQueue->size(); i  > 0; i--) {
            BUF* pBuf = harvestQueue->getElementAt(i - 1);
            HARVEST_DB* pHarvest = (HARVEST_DB*)pBuf->pData;
            if ((pHarvest->firstDay == firstDay) && (pHarvest->lastDay == lastDay))
                return pHarvest;
        }
    }
    return NULL;
}


// AP, 2005.11.11
void TFisherMan::procMsgECDP(EcoDynProtocol* ecdp, MESSAGE_ECDP* msg,
        RegionList* pRL, bool notRunning)
{
  int actionResult;
  MSG_CONTENT* msgContent;
  Region* pRegion;
  BUF* pBuf;
  ECOTIME* ecotime;
  long myTime;
  int myDay;

    msgContent = &(msg->msgContent);
    switch (msgContent->type) {

      case SEED_ACTION_CODE:
      {
        SEED_ACTION* seed;
        SEED_DB* pDBSeed;
        SEED_REGION* pSeedRegion;
        int lastDay;

        pRegion = pRL->getRegion(msgContent->content.action.seed.regName);
        if (pRegion == NULL) {
            actionResult = RESULT_FAILED;
        }
        else {
            seed = &(msgContent->content.action.seed);
            ecotime = &(seed->time);
            if (ecotime->type == TM_TYPE_NOW)
                myDay = JulianDay;
            else {
                myTime = ecotime->time;
                myDay = MyPEcoDynClass->GetAJulianDay(myTime);     // day of the year
            }
            lastDay = (myDay + 30) % 365;       // 30 days to seed all bivalves
            if (seed->bivalve.type == BV_TYPE_KELP)
                lastDay = (pDBSeed->lastDay + 30) % 365;         // more 30 days to seed kelps

            if ((seed->bivalve.type != BV_TYPE_SCALLOP)
                && (seed->bivalve.type != BV_TYPE_KELP)
                && (seed->bivalve.type != BV_TYPE_OYSTER)
                && (seed->bivalve.type != BV_TYPE_CLAM)) {
                actionResult = RESULT_FAILED;
            }
            else {
                pDBSeed = findSeedDB(myDay, lastDay);
                if (pDBSeed == NULL) {
                    if ((pBuf = Queue::allocBuf(sizeof(SEED_DB))) != NULL) {
                        pDBSeed = (SEED_DB*)pBuf->pData;
                        pDBSeed->firstDay = myDay;
                        pDBSeed->lastDay = lastDay;
                        pDBSeed->regionList = pRL;       // save RegionList pointer
                        pDBSeed->seeding = false;
                        pDBSeed->error = false;
                        pDBSeed->seedRegions = new Queue();
                        seedQueue->insertElement(pBuf);
                    }
                }
                if ((pDBSeed != NULL) && (pBuf = Queue::allocBuf(sizeof(SEED_REGION))) != NULL) {
                    pSeedRegion = (SEED_REGION*)pBuf->pData;
                    strcpy(pSeedRegion->regName, seed->regName);
                    pSeedRegion->bivalve_s.type = seed->bivalve.type;
                    pSeedRegion->bivalve_s.weight1 = seed->bivalve.weight1;
                    pSeedRegion->bivalve_s.weight2 = seed->bivalve.weight2;
                    pSeedRegion->density = seed->density;
                    pDBSeed->seedRegions->insertElement(pBuf);
                    actionResult = RESULT_OK;
                }
                else {
                    actionResult = RESULT_FAILED;
                }
            }
        }
        ecdp->buildSeedResult(&msg->messageId, actionResult);
        break;
      }

      case INSPECT_ACTION_CODE:
      {
        INSPECT_ACTION* inspect;
        INSPECT_DB* pDBInspect;
        Queue* pBivalveQueue;
        BUF* pBuf;
        BIVALVE* pBivalve;

        pRegion = pRL->getRegion(msgContent->content.action.inspect.regName);
        if (pRegion == NULL) {
            pBivalveQueue = new Queue();
            if ((pBuf = Queue::allocBuf(sizeof(BIVALVE))) == NULL) {
                break;
            }
            pBivalve = (BIVALVE*)(pBuf->pData);
            pBivalve->type = BV_TYPE_CLAM;
            pBivalve->length = 0.0;
            pBivalveQueue->insertElement(pBuf);
            ecdp->buildInspectResult(&msg->messageId, pBivalveQueue);
            delete pBivalveQueue;
        }
        else {
            inspect = &(msgContent->content.action.inspect);
            ecotime = &(inspect->time);
            if (ecotime->type == TM_TYPE_NOW)
                myDay = JulianDay;
            else {
                myTime = ecotime->time;
                myDay = MyPEcoDynClass->GetAJulianDay(myTime);
            }
            if ((pBuf = Queue::allocBuf(sizeof(INSPECT_DB))) == NULL) {
                break;
            }
            pDBInspect = (INSPECT_DB*)pBuf->pData;
            strcpy(pDBInspect->regName, inspect->regName);
            pDBInspect->day = myDay;
            pDBInspect->regionList = pRL;       // save RegionList pointer
            pDBInspect->ecdp = ecdp;       // save EcoDynProtocol pointer
            pDBInspect->msg_id.idMsg = msg->messageId.idMsg;
            strcpy(pDBInspect->msg_id.sender, msg->messageId.sender);
            strcpy(pDBInspect->msg_id.receiver, msg->messageId.receiver);
            inspectQueue->insertElement(pBuf);
        }
        break;
      }

      case HARVEST_ACTION_CODE:
      {
        HARVEST_ACTION* harvest;
        HARVEST_DB* pDBHarvest;
        HARVEST_REGION* pHarvestRegion;
        int lastDay;

        pRegion = pRL->getRegion(msgContent->content.action.harvest.regName);
        if (pRegion == NULL) {
            actionResult = RESULT_FAILED;
        }
        else {
            harvest = &(msgContent->content.action.harvest);
            ecotime = &(harvest->time);
            if (ecotime->type == TM_TYPE_NOW)
                myDay = JulianDay;
            else {
                myTime = ecotime->time;
                myDay = MyPEcoDynClass->GetAJulianDay(myTime);
            }
            lastDay = (myDay + 30) % 365;    // 30 days to harvest all bivalves

            if (
                (harvest->bivalve.type != BV_TYPE_SCALLOP)
                && (harvest->bivalve.type != BV_TYPE_KELP)
                && (harvest->bivalve.type != BV_TYPE_OYSTER)
                && (harvest->bivalve.type != BV_TYPE_CLAM)) {
                actionResult = RESULT_FAILED;
            }
            else {
                pDBHarvest = findHarvestDB(myDay, lastDay);
                if (pDBHarvest == NULL) {
                    if ((pBuf = Queue::allocBuf(sizeof(HARVEST_DB))) != NULL) {
                        pDBHarvest = (HARVEST_DB*)pBuf->pData;
                        pDBHarvest->firstDay = myDay;
                        pDBHarvest->lastDay = lastDay;
                        pDBHarvest->regionList = pRL;       // save RegionList pointer
                        pDBHarvest->ecdp = ecdp;       // save EcoDynProtocol pointer
                        pDBHarvest->harvesting = false;
                        pDBHarvest->harvestRegions = new Queue();
                        harvestQueue->insertElement(pBuf);
                    }
                }

                if ((pDBHarvest != NULL) && (pBuf = Queue::allocBuf(sizeof(HARVEST_REGION))) != NULL) {
                    pHarvestRegion = (HARVEST_REGION*)pBuf->pData;
                    strcpy(pHarvestRegion->regName, harvest->regName);
                    pHarvestRegion->bivalve.type = harvest->bivalve.type;
                    pHarvestRegion->bivalve.length = harvest->bivalve.length;
                    pHarvestRegion->msg_id.idMsg = msg->messageId.idMsg;
                    pHarvestRegion->weightHarvested = 0;
                    strcpy(pHarvestRegion->msg_id.sender, msg->messageId.sender);
                    strcpy(pHarvestRegion->msg_id.receiver, msg->messageId.receiver);
                    pDBHarvest->harvestRegions->insertElement(pBuf);
                    actionResult = RESULT_OK;
                }
                else {
                    actionResult = RESULT_FAILED;
                }
            }
        }
        if (actionResult == RESULT_FAILED)
            ecdp->buildHarvestResult(&msg->messageId, RESULT_FAILED, 0.0);
        break;
      }

      default:
        break;
    }

    /*
     * INSPECT is done even if model is not running
     *
     * ??? can we do the same for INSPECT and HARVEST ???
     *
     *        LET'S TRY ...
     */
    if (notRunning) {
        Go();
    }
}

Queue* TFisherMan::getSeedingQueue()
{
    return seedingQueue;
}

void TFisherMan::Go()
{
  int i;
  BUF* pBuf;
  SEED_DB* pSeed;
  HARVEST_DB* pHarvest;
  INSPECT_DB* pInspect;

    JulianDay = MyPEcoDynClass->GetJulianDay();

    /*
     * clear ancient seeding messages already processed
     */
    if (seedingQueue->size() > 0) {
        for (i = seedingQueue->size(); i  > 0; i--) {
            pBuf = seedingQueue->getElementAt(i - 1);
            pSeed = (SEED_DB*)pBuf->pData;
            if (pSeed->seeding) {
                pBuf = seedingQueue->removeElementAt(i-1);
                seedQueue->insertElement(pBuf); // reinsert element in seedQueue to do periodic seed - next year
                // Queue::freeBuf(pBuf);
            }
        }
    }

   /*
    * implement EcoDynamo Protocol messages
    */
    if (seedQueue->size() > 0) {
        for (i = seedQueue->size(); i  > 0; i--) {
            pBuf = seedQueue->getElementAt(i - 1);
            pSeed = (SEED_DB*)pBuf->pData;
            if (pSeed->firstDay <= pSeed->lastDay) {
                if ((JulianDay >= pSeed->firstDay) && (JulianDay <= pSeed->lastDay)) {
                    if (!pSeed->seeding) {
                        pBuf = seedQueue->removeElementAt(i-1);
                        seedingQueue->insertElement(pBuf);
                    }
                }
                else {
                    pSeed->seeding = false;
                }
            }
            else if ((JulianDay >= pSeed->firstDay) || (JulianDay <= pSeed->lastDay)) {
                if (!pSeed->seeding) {
                    pBuf = seedQueue->removeElementAt(i-1);
                    seedingQueue->insertElement(pBuf);
                }
            }
            else {
                pSeed->seeding = false;
            }
        }
    }

    if (harvestQueue->size() > 0) {
        for (i = harvestQueue->size(); i > 0 ; i--) {
            pBuf = harvestQueue->getElementAt(i - 1);
            pHarvest = (HARVEST_DB*)pBuf->pData;
            if (pHarvest->firstDay == 0) {
                // harvest immediate
                // send EcoDynamo Protocol message to Farmer Agent
                for (int k = pHarvest->harvestRegions->size(); k > 0; k--) {
                    BUF* pAux = pHarvest->harvestRegions->removeElementAt(k-1);
                    HARVEST_REGION* pHarvestRegion = (HARVEST_REGION*)pAux->pData;
                    pHarvest->ecdp->buildHarvestResult(&pHarvestRegion->msg_id, RESULT_OK,
                            pHarvestRegion->weightHarvested);
                    Queue::freeBuf(pAux);
                }

                pBuf = harvestQueue->removeElementAt(i - 1);
                Queue::freeBuf(pBuf);
            }
            else if (pHarvest->firstDay <= pHarvest->lastDay) {
                if ((JulianDay >= pHarvest->firstDay) && (JulianDay <= pHarvest->lastDay)) {
                    pHarvest->harvesting = true;
                    for (int k = pHarvest->harvestRegions->size(); k > 0; k--) {
                        BUF* pAux = pHarvest->harvestRegions->getElementAt(k-1);
                        HARVEST_REGION* pHarvestRegion = (HARVEST_REGION*)pAux->pData;
                        if (pHarvestRegion->bivalve.type == BV_TYPE_SCALLOP) {
                            ScallopHarvest(pHarvest, pHarvestRegion);
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_KELP) {
                            KelpHarvest(pHarvest, pHarvestRegion);
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_OYSTER) {
                            OysterHarvest(pHarvest, pHarvestRegion);
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_MUSSEL) {
                            MusselHarvest(pHarvest, pHarvestRegion);     // never called yet
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_CLAM) {
                            ClamHarvest(pHarvest, pHarvestRegion);
                        }
                    }
                }
                else if (pHarvest->harvesting) {
                    // send EcoDynamo Protocol message to Farmer Agent
                    for (int k = pHarvest->harvestRegions->size(); k > 0; k--) {
                        BUF* pAux = pHarvest->harvestRegions->getElementAt(k - 1);
                        HARVEST_REGION* pHarvestRegion = (HARVEST_REGION*)pAux->pData;
                        pHarvest->ecdp->buildHarvestResult(&pHarvestRegion->msg_id, RESULT_OK,
                                pHarvestRegion->weightHarvested);
                    }
                    pHarvest->harvesting = false;
                }
            }
            else {
                if ((JulianDay >= pHarvest->firstDay) || (JulianDay <= pHarvest->lastDay)) {
                    pHarvest->harvesting = true;
                    for (int k = pHarvest->harvestRegions->size(); k > 0; k--) {
                        BUF* pAux = pHarvest->harvestRegions->getElementAt(k-1);
                        HARVEST_REGION* pHarvestRegion = (HARVEST_REGION*)pAux->pData;
                        if (pHarvestRegion->bivalve.type == BV_TYPE_SCALLOP) {
                            ScallopHarvest(pHarvest, pHarvestRegion);
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_KELP) {
                            KelpHarvest(pHarvest, pHarvestRegion);
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_OYSTER) {
                            OysterHarvest(pHarvest, pHarvestRegion);
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_MUSSEL) {
                            MusselHarvest(pHarvest, pHarvestRegion);     // never called yet
                        }
                        else if (pHarvestRegion->bivalve.type == BV_TYPE_CLAM) {
                            ClamHarvest(pHarvest, pHarvestRegion);
                        }
                    }
                }
                else if (pHarvest->harvesting) {
                    // send EcoDynamo Protocol message to Farmer Agent
                    for (int k = pHarvest->harvestRegions->size(); k > 0; k--) {
                        BUF* pAux = pHarvest->harvestRegions->getElementAt(k - 1);
                        HARVEST_REGION* pHarvestRegion = (HARVEST_REGION*)pAux->pData;
                        pHarvest->ecdp->buildHarvestResult(&pHarvestRegion->msg_id, RESULT_OK,
                                pHarvestRegion->weightHarvested);
                    }
                    pHarvest->harvesting = false;
                }
            }
        }
    }

    if (inspectQueue->size() > 0) {
        for (i = inspectQueue->size(); i > 0; i--) {
            pBuf = inspectQueue->getElementAt(i - 1);
            pInspect = (INSPECT_DB*)pBuf->pData;
            if (JulianDay >= pInspect->day) {
                BivalvesInspect(pInspect);
                pBuf = inspectQueue->removeElementAt(i - 1);
                Queue::freeBuf(pBuf);
            }
        }
    }
}



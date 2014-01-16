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


// Farmer man class - INSPECT private method
// AP, 2005.11.18...

void TFisherMan::BivalvesInspect(INSPECT_DB* pDB)
{
  Queue* pBoxes;
  int size, i, n, nBiv1, nBiv2;
  double length1, ShellLength1, length2, ShellLength2, ExtraValue;
  BUF* pBuf;
  REGION_BOX* pBox;
  Queue* pQueue;
  BIVALVE* pBivalve;
  int type;
  char variableName[64];
  int type2;
  char variableName2[64];

  TEcoDynClass* pEDC;
  TEcoDynClass* MyZoobenthosPointer;
  Queue* pEDCQueue;
  int qSize, qIndex;

    pQueue = new Queue();
    pEDCQueue = MyPEcoDynClass->getEDCQueue();
    qSize = pEDCQueue->size();
    for (qIndex = 0; qIndex < qSize; qIndex++) {

        pEDC =(TEcoDynClass*)pEDCQueue->RetrieveObject(qIndex);
        if (pEDC->GetObjectCode() != ZOOBOBJECTCODE)
            continue;

        MyZoobenthosPointer = pEDC;
        if (strstr(MyZoobenthosPointer->GetEcoDynClassName(), "ChlamysFarreri"))
        {
            strcpy(variableName, "C. farreri shell length 1");
            type = BV_TYPE_SCALLOP;
        }
        else if (strstr(MyZoobenthosPointer->GetEcoDynClassName(), "CrassostreaGigas"))
        {
            strcpy(variableName, "C. gigas shell length 1");
            type = BV_TYPE_OYSTER;
        }
/*        else if (strstr(MyZoobenthosPointer->GetEcoDynClassName(), "LaminariaJaponica"))
        {
            strcpy(variableName, "L. japonica individual weight");
            type = BV_TYPE_KELP;
        } */
        else if (strstr(MyZoobenthosPointer->GetEcoDynClassName(), "Decussatus")) {
    //        strcpy(variableName, "R. decussatus shell length 1");
            strcpy(variableName, "R. decussatus biomass 1");      // to change
            type = BV_TYPE_CLAM;
        }
        else
            continue;

        length1 = 0.0;
        nBiv1 = 0;

        pBoxes = pDB->regionList->getRegion(pDB->regName)->getRegionArea();
        size = (pBoxes == NULL) ? NumberOfBoxes : pBoxes->size();
        n = 0;

        while (n < size) {
            if (pBoxes == NULL) {
                i = n;
            }
            else {
                pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                i = GetBoxNumber(pBox->column, pBox->line);
            }

            if (type == BV_TYPE_CLAM)       // special Inquiry (Harvestable Biomass)
                MyZoobenthosPointer->Inquiry(GetEcoDynClassName(),
                                             ShellLength1,
                                             i,
                                             0.0,
                                             ObjectCode);
            else
                MyZoobenthosPointer->Inquiry(GetEcoDynClassName(),
                                             ShellLength1,
                                             i,
                                             variableName,
                                             ObjectCode);
            length1 += ShellLength1;
            nBiv1++;

            n++;
        }

        if (nBiv1 > 0) {
            ShellLength1 = length1 / nBiv1;
            if ((pBuf = Queue::allocBuf(sizeof(BIVALVE))) != NULL) {
                pBivalve = (BIVALVE*)pBuf->pData;
                pBivalve->type = type;
                pBivalve->length = ShellLength1;
                pQueue->insertElement(pBuf);
            }
        }
    }

    if (pQueue->size() > 0) {
        pDB->ecdp->buildInspectResult(&pDB->msg_id, pQueue);
    }
    pQueue->clear();
}


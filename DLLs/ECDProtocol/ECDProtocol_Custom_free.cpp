//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_free.cpp
 *
 *  PRIVATE method invoked by 'freeMessageRx' and 'freeMessageTx' (ECDProtocol.cpp)
 *
 *  Every time a new message is defined some code must be included HERE:
 *      PRIVATE void freeMsgECDP(MESSAGE_ECDP* msgECDP)
 *
 *  and in:
 *      PRIVATE void formatMessage(MESSAGE_ECDP* msgECDP, char* message)
 *                                                 ::ECDProtocol_Custom_fmt.cpp
 *      PUBLIC void saveMessageRx(char* message)   ::ECDProtocol_Custom_get.cpp
 *
 *  ECOLANG - EcoDynamo Protocol v1.0 - 2007.01.30 (AP)
 *    included messages: delete and unselect_variables.
 */
//---------------------------------------------------------------------------

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifdef __BORLANDC__
	#include <vcl.h>
	#pragma hdrstop

	#include <stdio.h>
	#include <string.h>
#else  // __BORLANDC__
	#include <cstdio>
	#include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//---------------------------------------------------------------------------
/*
 *  NAME freeMsgECDP
 *
 *  This method frees instances created by received messages:
 *   1. verify message type
 *   2. delete instances allocated (tipically QUEUEs)
 */
 
void EcoDynProtocol::freeMsgECDP(MESSAGE_ECDP* msgECDP)
{
    switch (msgECDP->msgContent.type) {
      case INSPECT_RESULT_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.inspectResult.pQueue);
        //msgECDP->msgContent.content.perception.inspectResult.pQueue->clear();
      }
      break;

      case DEFINE_CODE:
      {
        if (msgECDP->msgContent.content.definition.region.type == RG_TYPE_REGION) {
            SUB_REGION reg = msgECDP->msgContent.content.definition.region.region.reg;
            deleteQueue(reg.pRegionArea);
            //reg.pRegionArea->clear();
        }
        else {      // RG_TYPE_REG_NAME
            deleteQueue(msgECDP->msgContent.content.definition.region.region.pRegNames);
            //msgECDP->msgContent.content.definition.region.region.pRegNames->clear();
        }
      }
      break;

      case REGION_RESULT_CODE:
      {
        REGION_RESULT regionResult =
                msgECDP->msgContent.content.perception.regionResult;
        if (regionResult.definition.region.type == RG_TYPE_REGION) {
            SUB_REGION reg = regionResult.definition.region.region.reg;
            deleteQueue(reg.pRegionArea);
            //reg.pRegionArea->clear();
        }
        else {      // RG_TYPE_REG_NAME
            deleteQueue(regionResult.definition.region.region.pRegNames);
            //regionResult.definition.region.region.pRegNames->clear();
        }
      }
      break;

      case DELETE_ACTION_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.action.specs.pNames);
        //msgECDP->msgContent.content.action.specs.pNames->clear();
      }
      break;

      case SELECT_CLASSES_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.action.specs.pNames);
        //msgECDP->msgContent.content.action.specs.pNames->clear();
      }
      break;

      case GET_VARIABLE_VALUE_CODE:
      {
        BUF* pBuf;
        int index;
        VAR_VALUE_ACTION* pValue =
                &msgECDP->msgContent.content.action.varValue;
        if (pValue->boxes.type == BX_TYPE_CELLS)
            deleteQueue(pValue->boxes.domain.pCells);
            //pValue->boxes.domain.pCells->clear();
        else if (pValue->boxes.domain.subDomain.type == DOM_TYPE_REG)
            deleteQueue(pValue->boxes.domain.subDomain.pRegNames);
            //pValue->boxes.domain.subDomain.pRegNames->clear();
      }
      break;

      case SET_VARIABLE_VALUE_CODE:
      {
        BUF* pBuf;
        int index;
        VAR_VALUE* pValue;
        Queue* pVars = msgECDP->msgContent.content.action.setValue.pValues;
        index = 0;
        while (index < pVars->size()) {
            pBuf = pVars->getElementAt(index);
            pValue = (VAR_VALUE*)pBuf->pData;
            if (pValue->boxes.type == BX_TYPE_CELLS)
                deleteQueue(pValue->boxes.domain.pCells);
                //pValue->boxes.domain.pCells->clear();
            else if (pValue->boxes.domain.subDomain.type == DOM_TYPE_REG)
                deleteQueue(pValue->boxes.domain.subDomain.pRegNames);
                //pValue->boxes.domain.subDomain.pRegNames->clear();
            index++;
        }
        deleteQueue(pVars);
        //pVars->clear();
      }
      break;

      case SET_PARAMETERS_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.action.setValue.pValues);
        //msgECDP->msgContent.content.action.setValue.pValues->clear();
      }
      break;

      case SUBDOMAIN_CODE:
      {
        if (msgECDP->msgContent.content.action.subDomain.type == DOM_TYPE_REG)
            deleteQueue(msgECDP->msgContent.content.action.subDomain.pRegNames);
            //msgECDP->msgContent.content.action.subDomain.pRegNames->clear();
      }
      break;

      case CLASSES_AVAILABLE_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.varClassResult.pQueue);
        //msgECDP->msgContent.content.perception.varClassResult.pQueue->clear();
      }
      break;

      case CLASSES_SELECTED_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.varClassResult.pQueue);
        //msgECDP->msgContent.content.perception.varClassResult.pQueue->clear();
      }
      break;

      case VARIABLES_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.varClassResult.pQueue);
        //msgECDP->msgContent.content.perception.varClassResult.pQueue->clear();
      }
      break;

      case PARAMETERS_VALUES_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.parmsResult.pQueue);
        //msgECDP->msgContent.content.perception.parmsResult.pQueue->clear();
      }
      break;

      case SELECT_VARIABLES_CODE:
      {
        SEL_VAR_ACTION* pAction;

        pAction = &msgECDP->msgContent.content.action.selVarAction;
        deleteQueue(pAction->pVarNames);
        //pAction->pVarNames->clear();
        if (pAction->boxes.type == BX_TYPE_CELLS)
            deleteQueue(pAction->boxes.domain.pCells);
            //pAction->boxes.domain.pCells->clear();
        else if (pAction->boxes.domain.subDomain.type == DOM_TYPE_REG)
            deleteQueue(pAction->boxes.domain.subDomain.pRegNames);
            //pAction->boxes.domain.subDomain.pRegNames->clear();
      }
      break;

      case LOG_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.action.logAction.pSteps);
        //msgECDP->msgContent.content.action.logAction.pSteps->clear();
      }
      break;

      case VARIABLES_AVAILABLE_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.varClassResult.pQueue);
        //msgECDP->msgContent.content.perception.varClassResult.pQueue->clear();
      }
      break;

      case UNSELECT_VARIABLES_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.action.unSelVarAction.pVarNames);
        //msgECDP->msgContent.content.action.unSelVarAction.pVarNames->clear();
      }
      break;

      case REGISTER_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.registerMsg.pQueue);
        //msgECDP->msgContent.content.perception.registerMsg.pQueue->clear();
      }
      break;

      case LOGGER_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.loggerMsg.pQueue);
        //msgECDP->msgContent.content.perception.loggerMsg.pQueue->clear();
      }
      break;

      case VARIABLE_VALUE_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.varValueResult.pQueue);
        //msgECDP->msgContent.content.perception.varValueResult.pQueue->clear();
      }
      break;

      case MODEL_MORPHOLOGY_RESULT_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.morphologyResult.pQueue);
        //msgECDP->msgContent.content.perception.morphologyResult.pQueue->clear();
      }
      break;

      case REGION_NAMES_RESULT_CODE:
      {
        deleteQueue(msgECDP->msgContent.content.perception.regionNamesResult.pQueue);
        //msgECDP->msgContent.content.perception.regionNamesResult.pQueue->clear();
      }
      break;

      case MODEL_SPECIES_RESULT_CODE:
      {
        BUF* pBuf;
        int index;
        SPECIES_VALUE* pValue;
        Queue* pQueue = msgECDP->msgContent.content.perception.benthicSpeciesResult.pQueue;
        index = 0;
        while (index < pQueue->size()) {
            pBuf = pQueue->getElementAt(index);
            pValue = (SPECIES_VALUE*)pBuf->pData;
            if (pValue->boxes.type == BX_TYPE_CELLS)
                deleteQueue(pValue->boxes.domain.pCells);
                //pValue->boxes.domain.pCells->clear();
            else if (pValue->boxes.domain.subDomain.type == DOM_TYPE_REG)
                deleteQueue(pValue->boxes.domain.subDomain.pRegNames);
                //pValue->boxes.domain.subDomain.pRegNames->clear();
            index++;
        }
        deleteQueue(pQueue);
        //pVars->clear();
      }
      break;

    /* AP - ADD CODE HERE...
     */

      default:
      break;
    }
}
//---------------------------------------------------------------------------


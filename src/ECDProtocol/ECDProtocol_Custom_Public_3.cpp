//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_Public_3.cpp
 *
 *   PUBLIC methods invoked by Agents
 *
 *   BULK 3:
 *
 *      void buildGetAvailableClassesAction(MSG_ID* msg);
 *      void buildGetSelectedClassesAction(MSG_ID* msg);
 *      void buildSelectClassesAction(MSG_ID* msg, Queue* pClassNames);
 *      void buildGetVariablesAction(MSG_ID* msg, char* className);
 *      void buildGetVariableValueAction(MSG_ID* msg, char* className,
 *              char* varName, BOXES* pBoxes);
 *      void buildSetVariableValueAction(MSG_ID* msg, char* className, Queue* pVars);
 *      void buildGetParametersAction(MSG_ID* msg, char* className);
 *      void buildSetParametersAction(MSG_ID* msgchar* className, Queue* pParms);
 *      void buildGetTimeSpecAction(MSG_ID* msg);
 *      void buildSetTimeSpecAction(MSG_ID* msg, int step, int startT, int finishT);
 *      void buildSubDomainAction(MSG_ID* msg, R_DOMAIN* pDomain);
 *      void buildClassesAvailableResult(MSG_ID* msg, Queue* pClassNames);
 *      void buildClassesSelectedResult(MSG_ID* msg, Queue* pClassNames);
 *      void buildVariablesResult(MSG_ID* msg, Queue* pVars);
 *      void buildVariableValueResult(MSG_ID* msg, Queue* pValues);
 *      void buildVariableSetResult(MSG_ID* msg, int result);
 *      void buildParametersValuesResult(MSG_ID* msg, Queue* pParms);
 *      void buildParametersSetResult(MSG_ID* msg, int result);
 *      void buildTimeSpecResult(MSG_ID* msg, int step, long startT, long finishT);
 *      void buildSubDomainResult(MSG_ID* msg, int result);
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
    #include <cstring>
	#include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//---------------------------------------------------------------------------
/*
 *  NAME buildGetAvailableClassesAction
 *
 *  Build Get Available Classes action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetAvailableClassesAction(char* destination)
{
    return buildSimpleAction(destination, GET_AVAILABLE_CLASSES_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetSelectedClassesAction
 *
 *  Build Get Selected Classes action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetSelectedClassesAction(char* destination)
{
    return buildSimpleAction(destination, GET_SELECTED_CLASSES_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSelectClassesAction
 *
 *  Build "SPECS_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSelectClassesAction(char* destination, Queue* pClassNames)
{
    SPECS_ACTION* pSpecs;
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SELECT_CLASSES_CODE;
    pSpecs = &(msgECDP_action->msgContent.content.action.specs);
    pSpecs->pNames = duplicateElementQueue(pClassNames, ECDP_STRING);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetVariablesAction
 *
 *  Build "CLASS_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetVariablesAction(char* destination, char* className)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = GET_VARIABLES_CODE;
    strcpy(msgECDP_action->msgContent.content.action.clAction.name, className);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetVariableValueAction
 *
 *  Build "VAR_VALUE_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

int EcoDynProtocol::buildGetVariableValueAction(char* destination, char* className,
            char* varName, BOXES* pBoxes)
{
    char varNameProto[ECDP_STRING];
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = GET_VARIABLE_VALUE_CODE;
    strcpy(msgECDP_action->msgContent.content.action.varValue.className, className);
    strcpy(msgECDP_action->msgContent.content.action.varValue.varName, varName);

    BOXES* pBoxesNew = &msgECDP_action->msgContent.content.action.varValue.boxes;

    pBoxesNew->type = pBoxes->type;
    if (pBoxesNew->type == BX_TYPE_CELLS) {
        pBoxesNew->domain.pCells =
                duplicateElementQueue(pBoxes->domain.pCells, sizeof(int));
    }
    else {
        pBoxesNew->domain.subDomain.type = pBoxes->domain.subDomain.type;
        if (pBoxesNew->domain.subDomain.type == DOM_TYPE_REG) {
            pBoxesNew->domain.subDomain.pRegNames =
                duplicateElementQueue(pBoxes->domain.subDomain.pRegNames, ECDP_STRING);
        }
        else
            pBoxesNew->domain.subDomain.pRegNames = NULL;
    }
    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSetVariableValueAction
 *
 *  Build "SET_VALUE_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSetVariableValueAction(char* destination, char* className,
        Queue* pVars)
{
    Queue* pNew;
    BUF* pBufNew;
    VAR_VALUE* pValueNew;
    BOXES* pBoxesNew;

    BUF* pBufVar;
    VAR_VALUE* pValue;
    BOXES* pBoxes;
    int index;

    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SET_VARIABLE_VALUE_CODE;
    strcpy(msgECDP_action->msgContent.content.action.setValue.className, className);

    pNew = new Queue();
    index = 0;
    while (index < pVars->size()) {
        pBufVar = pVars->getElementAt(index);
        pValue = (VAR_VALUE*)pBufVar->pData;
        pBoxes = &pValue->boxes;

        if ((pBufNew = Queue::allocBuf(sizeof(VAR_VALUE))) == NULL) {
            break;
        }
        pValueNew = (VAR_VALUE*)pBufNew->pData;
        strcpy(pValueNew->name, pValue->name);
        pValueNew->value = pValue->value;
        pBoxesNew = &pValueNew->boxes;

        pBoxesNew->type = pBoxes->type;
        if (pBoxesNew->type == BX_TYPE_CELLS) {
            pBoxesNew->domain.pCells =
                    duplicateElementQueue(pBoxes->domain.pCells, sizeof(int));
        }
        else {
            pBoxesNew->domain.subDomain.type = pBoxes->domain.subDomain.type;
            if (pBoxesNew->domain.subDomain.type == DOM_TYPE_REG) {
                pBoxesNew->domain.subDomain.pRegNames =
                    duplicateElementQueue(pBoxes->domain.subDomain.pRegNames, ECDP_STRING);
            }
            else
                pBoxesNew->domain.subDomain.pRegNames = NULL;
        }
        pNew->insertElement(pBufNew);
        index++;
    }
    msgECDP_action->msgContent.content.action.setValue.pValues = pNew;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetParametersAction
 *
 *  Build "CLASS_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetParametersAction(char* destination, char* className)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = GET_PARAMETERS_CODE;
    strcpy(msgECDP_action->msgContent.content.action.clAction.name, className);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSetParametersAction
 *
 *  Build "SET_VALUE_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSetParametersAction(char* destination, char* className,
        Queue* pParms)
{
    Queue* pNew;
    BUF* pBufNew;
    PARM_VALUE* pValueNew;

    BUF* pBufVar;
    PARM_VALUE* pValue;
    int index;

    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SET_PARAMETERS_CODE;
    strcpy(msgECDP_action->msgContent.content.action.setValue.className, className);

    pNew = new Queue();
    index = 0;
    while (index < pParms->size()) {
        pBufVar = pParms->getElementAt(index);
        pValue = (PARM_VALUE*)pBufVar->pData;

        if ((pBufNew = Queue::allocBuf(sizeof(PARM_VALUE))) == NULL) {
            break;
        }
        pValueNew = (PARM_VALUE*)pBufNew->pData;
        strcpy(pValueNew->name, pValue->name);
        pValueNew->value = pValue->value;
        pNew->insertElement(pBufNew);
        index++;
    }
    msgECDP_action->msgContent.content.action.setValue.pValues = pNew;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetTimeSpecAction
 *
 *  Build Get Time Spec action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetTimeSpecAction(char* destination)
{
    return buildSimpleAction(destination, GET_TIME_SPEC_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSetTimeSpecAction
 *
 *  Build "TIME_SPEC_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSetTimeSpecAction(char* destination, int step, int startT,
        int finishT)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SET_TIME_SPEC_CODE;
    msgECDP_action->msgContent.content.action.timeSpec.step = step;
    msgECDP_action->msgContent.content.action.timeSpec.startTime = startT;
    msgECDP_action->msgContent.content.action.timeSpec.finishTime = finishT;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSubDomainAction
 *
 *  Build "SUBDOMAIN_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSubDomainAction(char* destination, R_DOMAIN* pDomain)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SUBDOMAIN_CODE;
    msgECDP_action->msgContent.content.action.subDomain.type = pDomain->type;
    if (pDomain->type == DOM_TYPE_REG) {
        msgECDP_action->msgContent.content.action.subDomain.pRegNames =
            duplicateElementQueue(pDomain->pRegNames, ECDP_STRING);
    }
    else
        pDomain->pRegNames = NULL;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildClassesAvailableResult
 *
 *  Build "VARCLASS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildClassesAvailableResult(MSG_ID* msg, Queue* pClassNames)
{
    return buildVarClassResult(msg, pClassNames, CLASSES_AVAILABLE_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildClassesSelectedResult
 *
 *  Build "VARCLASS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildClassesSelectedResult(MSG_ID* msg, Queue* pClassNames)
{
    return buildVarClassResult(msg, pClassNames, CLASSES_SELECTED_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildVariablesResult
 *
 *  Build "VARCLASS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildVariablesResult(MSG_ID* msg, Queue* pVarNames)
{
    return buildVarClassResult(msg, pVarNames, VARIABLES_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildVariableValueResult
 *
 *  Build "VAR_VALUE_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildVariableValueResult(MSG_ID* msg, Queue* pValues)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = VARIABLE_VALUE_CODE;
    msgECDP_answer->msgContent.content.perception.varValueResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.varValueResult.pQueue =
            duplicateElementQueue(pValues, sizeof(BOX_VALUE));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildVariableSetResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildVariableSetResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, VARIABLE_SET_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildParametersValuesResult
 *
 *  Build "PARMS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildParametersValuesResult(MSG_ID* msg, Queue* pParms)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = PARAMETERS_VALUES_CODE;
    msgECDP_answer->msgContent.content.perception.parmsResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.parmsResult.pQueue =
            duplicateElementQueue(pParms, sizeof(PARM_VALUE));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildParametersSetResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildParametersSetResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, PARAMETERS_SET_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildTimeSpecResult
 *
 *  Build "TIME_SPEC_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildTimeSpecResult(MSG_ID* msg, int step, long startT,
        long finishT)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = TIME_SPEC_CODE;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.step = step;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.startTime = startT;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.finishTime = finishT;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSubDomainResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSubDomainResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, SUBDOMAIN_RESULT_CODE, result);
}
//---------------------------------------------------------------------------


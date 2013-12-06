//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_Public_5.cpp
 *
 *   PUBLIC methods invoked by Agents
 *
 *   BULK 5:
 *
 *      int buildConnectAction(char* destination, char* hostname, char* hostaddr, int portServer);
 *      int buildDisconnectAction(char* destination);
 *      int buildAcceptResult(MSG_ID* msg, int result);
 *
 *   == added in 207.05.03 by AP
 *      int buildGetRegionNamesAction(char* destination);
 *      int buildGetRegionAction(char* destination, char* regName);
 *      int buildModelDimensionsAction(char* destination);
 *      int buildModelMorphologyAction(char* destination);
 *      int buildModelSpeciesAction(char* destination);
 *      int buildRegionNamesResult(MSG_ID* msg, Queue* pRegNames);
 *      int buildRegionResult(MSG_ID* msg, char* regName, REGION* pRegion);
 *      int buildDimensionsResult(MSG_ID* msg, int lines, int columns,
 *              int layers, int modelType);
 *      int buildMorphologyResult(MSG_ID* msg, Queue* pValues);
 *      int buildBenthicSpeciesResult(MSG_ID* msg, Queue* pSpecies);
 *      int buildMorphologyResult(MSG_ID* msg, Queue* pValues);
 *      int buildBenthicSpeciesResult(MSG_ID* msg, Queue* pSpecies);
 *
 *   == added in 2007.05.15 by AP
 *      int buildMorphologyEndResult(MSG_ID* msg);
 *      int buildBenthicSpeciesEndResult(MSG_ID* msg);
 *
 *   == added in 2008.05.21 by AP
 *      int buildAgentsAction(char* destination);
 *      int buildKnownAgentsResult(MSG_ID* msg, Queue* pKnownAgents);
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
 *  NAME buildConnectAction
 *
 *  Build "CONNECT_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildConnectAction(char* destination, char* hostname,
        char* hostaddr, int portServer)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = CONNECT_CODE;
    strcpy(msgECDP_action->msgContent.content.action.connectAction.hostname, hostname);
    strcpy(msgECDP_action->msgContent.content.action.connectAction.hostaddr, hostaddr);
    msgECDP_action->msgContent.content.action.connectAction.portServer = portServer;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildDisconnectAction
 *
 *  Build Disconnect action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildDisconnectAction(char* destination)
{
    return buildSimpleAction(destination, DISCONNECT_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildAcceptResult
 *
 *  Build "ACCEPT_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildAcceptResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, ACCEPT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetRegionNamesAction
 *
 *  Build Get Region Names action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetRegionNamesAction(char* destination)
{
    return buildSimpleAction(destination, REGION_NAMES_ACTION_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetRegionAction
 *
 *  Build "REGION_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetRegionAction(char* destination, char* regName)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = GET_REGION_ACTION_CODE;
    strcpy(msgECDP_action->msgContent.content.action.regionAction.name, regName);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildModelDimensionsAction
 *
 *  Build Model Dimensions action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildModelDimensionsAction(char* destination)
{
    return buildSimpleAction(destination, MODEL_DIMENSIONS_ACTION_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildModelMorphologyAction
 *
 *  Build Model Morphology action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildModelMorphologyAction(char* destination)
{
    return buildSimpleAction(destination, MODEL_MORPHOLOGY_ACTION_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildModelSpeciesAction
 *
 *  Build Model Species action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildModelSpeciesAction(char* destination)
{
    return buildSimpleAction(destination, MODEL_SPECIES_ACTION_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildRegionNamesResult
 *
 *  Build "REGION_NAMES_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildRegionNamesResult(MSG_ID* msg, Queue* pRegNames)
{
    return buildVarClassResult(msg, pRegNames, REGION_NAMES_RESULT_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildRegionResult
 *
 *  Build "REGION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildRegionResult(MSG_ID* msg, char* regName, REGION* pRegion)
{
    REGION* pMsgRegion;

    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = REGION_RESULT_CODE;
    msgECDP_answer->msgContent.content.perception.regionResult.id = msg->idMsg;
    strcpy(msgECDP_answer->msgContent.content.perception.regionResult.definition.regName, regName);
    pMsgRegion = &(msgECDP_answer->msgContent.content.perception.regionResult.definition.region);
    pMsgRegion->type = pRegion->type;
    if (pRegion->type == RG_TYPE_REGION) {
        pMsgRegion->region.reg.regionType.type = pRegion->region.reg.regionType.type;
        if (pMsgRegion->region.reg.regionType.type == RT_TYPE_WATER) {
            pMsgRegion->region.reg.regionType.waterRegion.type =
                    pRegion->region.reg.regionType.waterRegion.type;
            pMsgRegion->region.reg.regionType.waterRegion.quality =
                    pRegion->region.reg.regionType.waterRegion.quality;
            pMsgRegion->region.reg.regionType.waterRegion.sedimentCaract.type =
                    pRegion->region.reg.regionType.waterRegion.sedimentCaract.type;
            pMsgRegion->region.reg.regionType.waterRegion.sedimentCaract.quality =
                    pRegion->region.reg.regionType.waterRegion.sedimentCaract.quality;
        }
        pMsgRegion->region.reg.pRegionArea =
                duplicateElementQueue(pRegion->region.reg.pRegionArea, sizeof(SIMPLE_REGION));
    }
    else {
        pMsgRegion->region.pRegNames =
                duplicateElementQueue(pRegion->region.pRegNames, ECDP_STRING);
    }

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildDimensionsResult
 *
 *  Build "DIMENSIONS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildDimensionsResult(MSG_ID* msg, int lines, int columns,
            int layers, char* modelType)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = MODEL_DIMENSIONS_RESULT_CODE;
    msgECDP_answer->msgContent.content.perception.dimensionsResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.dimensionsResult.lines = lines;
    msgECDP_answer->msgContent.content.perception.dimensionsResult.columns = columns;
    msgECDP_answer->msgContent.content.perception.dimensionsResult.layers = layers;
    strcpy(msgECDP_answer->msgContent.content.perception.dimensionsResult.modelType,
            modelType);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildMorphologyResult
 *
 *  Build "MORPHOLOGY_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildMorphologyResult(MSG_ID* msg, Queue* pValues)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = MODEL_MORPHOLOGY_RESULT_CODE;
    msgECDP_answer->msgContent.content.perception.morphologyResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.morphologyResult.pQueue =
            duplicateElementQueue(pValues, sizeof(BOX_VALUE));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildBenthicSpeciesResult
 *
 *  Build "BENTHIC_SPECIES_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildBenthicSpeciesResult(MSG_ID* msg, Queue* pSpecies)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = MODEL_SPECIES_RESULT_CODE;
    msgECDP_answer->msgContent.content.perception.benthicSpeciesResult.id = msg->idMsg;

    Queue* pNew;
    BUF* pBufNew;
    SPECIES_VALUE* pValueNew;
    BOXES* pBoxesNew;

    BUF* pBufVar;
    SPECIES_VALUE* pValue;
    BOXES* pBoxes;
    int index;

    pNew = new Queue();
    index = 0;
    while (index < pSpecies->size()) {
        pBufVar = pSpecies->getElementAt(index);
        pValue = (SPECIES_VALUE*)pBufVar->pData;
        pBoxes = &pValue->boxes;

        if ((pBufNew = Queue::allocBuf(sizeof(VAR_VALUE))) == NULL) {
            break;
        }
        pValueNew = (SPECIES_VALUE*)pBufNew->pData;
        strcpy(pValueNew->name, pValue->name);
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
    msgECDP_answer->msgContent.content.perception.benthicSpeciesResult.pQueue = pNew;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildMorphologyEndResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildMorphologyEndResult(MSG_ID* msg)
{
    return buildActionResult(msg, MODEL_MORPHOLOGY_END_CODE, RESULT_OK);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildBenthicSpeciesEndResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildBenthicSpeciesEndResult(MSG_ID* msg)
{
    return buildActionResult(msg, MODEL_SPECIES_END_CODE, RESULT_OK);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildAgentsAction
 *
 *  Build Agents action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildAgentsAction(char* destination)
{
    return buildSimpleAction(destination, AGENTS_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildKnownAgentsResult
 *
 *  Build "KNOWN_AGENTS_CODE" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildKnownAgentsResult(MSG_ID* msg, Queue* pKnownAgents)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = KNOWN_AGENTS_CODE;
    msgECDP_answer->msgContent.content.perception.agentsResult.id = msg->idMsg;

    Queue* pNew;
    BUF* pBufNew;
    AGENT_DATA* pValueNew;

    BUF* pBufAg;
    AGENT_DATA* pValue;
    int index;

    pNew = new Queue();
    index = 0;
    while (index < pKnownAgents->size()) {
        pBufAg = pKnownAgents->getElementAt(index);
        pValue = (AGENT_DATA*)pBufAg->pData;

        if ((pBufNew = Queue::allocBuf(sizeof(AGENT_DATA))) == NULL) {
            break;
        }
        pValueNew = (AGENT_DATA*)pBufNew->pData;
        strcpy(pValueNew->agentName, pValue->agentName);
        strcpy(pValueNew->hostName, pValue->hostName);
        strcpy(pValueNew->hostAddr, pValue->hostAddr);
        pValueNew->serverPort = pValue->serverPort;
        pValueNew->connected = pValue->connected;

        pNew->insertElement(pBufNew);
        index++;
    }
    msgECDP_answer->msgContent.content.perception.agentsResult.pQueue = pNew;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------


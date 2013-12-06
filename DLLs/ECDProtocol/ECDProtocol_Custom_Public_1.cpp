//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_Public_1.cpp
 *
 *   PUBLIC methods invoked by Agents
 *
 *   BULK 1:
 *
 *      void buildDefinitionAction(MSG_ID* msg, char* regName, REGION* pRegion);
 *      void buildSeedAction(MSG_ID* msg, char* regionName,
 *              ECOTIME* pTime, BIVALVE_S* pBivalve_s, double weight);
 *      void buildInspectAction(MSG_ID* msg, char* regionName, ECOTIME* pTime);
 *      void buildHarvestAction(MSG_ID* msg, char* regionName,
 *              ECOTIME* pTime, BIVALVE* pBivalve);
 *      void buildSeedResult(MSG_ID* msg, int result);
 *      void buildInspectResult(MSG_ID* msg, Queue* pBivalveQueue);
 *      void buildHarvestResult(MSG_ID* msg, int result, double weight);
 *
 *   == added in 30/01/07 by AP
 *      void buildDeleteAction(MSG_ID* msg, Queue* pRegNames);
 *      void buildDefineResult(MSG_ID* msg, int result);
 *      void buildDeleteResult(MSG_ID* msg, int result);
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
 *  NAME buildDefinitionAction
 *
 *  Build "DEFINITION_MSG" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildDefinitionAction(char* destination, char* regName,
        REGION* pRegion)
{
    REGION* pMsgRegion;

    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = DEFINE_CODE;
    strcpy(msgECDP_action->msgContent.content.definition.regName, regName);
    pMsgRegion = &(msgECDP_action->msgContent.content.definition.region);
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
 *  NAME buildSeedAction
 *
 *  Build "SEED_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSeedAction(char* destination, char* regionName,
        ECOTIME* pTime, BIVALVE_S* pBivalve_s, double density)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SEED_ACTION_CODE;
    strcpy(msgECDP_action->msgContent.content.action.seed.regName, regionName);
    msgECDP_action->msgContent.content.action.seed.time.type = pTime->type;
    msgECDP_action->msgContent.content.action.seed.time.time = pTime->time;
    msgECDP_action->msgContent.content.action.seed.bivalve.type = pBivalve_s->type;
    msgECDP_action->msgContent.content.action.seed.bivalve.weight1 =
            pBivalve_s->weight1;
    msgECDP_action->msgContent.content.action.seed.bivalve.weight2 =
            pBivalve_s->weight2;
    msgECDP_action->msgContent.content.action.seed.density = density;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildInspectAction
 *
 *  Build "INSPECT_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildInspectAction(char* destination, char* regionName,
        ECOTIME* pTime)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = INSPECT_ACTION_CODE;
    strcpy(msgECDP_action->msgContent.content.action.inspect.regName, regionName);
    msgECDP_action->msgContent.content.action.inspect.time.type = pTime->type;
    msgECDP_action->msgContent.content.action.inspect.time.time = pTime->time;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildHarvestAction
 *
 *  Build "HARVEST_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildHarvestAction(char* destination, char* regionName,
        ECOTIME* pTime, BIVALVE* pBivalve)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = HARVEST_ACTION_CODE;
    strcpy(msgECDP_action->msgContent.content.action.harvest.regName, regionName);
    msgECDP_action->msgContent.content.action.harvest.time.type = pTime->type;
    msgECDP_action->msgContent.content.action.harvest.time.time = pTime->time;
    msgECDP_action->msgContent.content.action.harvest.bivalve.type = pBivalve->type;
    msgECDP_action->msgContent.content.action.harvest.bivalve.length =
            pBivalve->length;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSeedResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSeedResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, SEED_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildInspectResult
 *
 *  Bu "INSPECT_RESULT" in a MESSAGE_ECDP strucildture.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildInspectResult(MSG_ID* msg, Queue* pBivalveQueue)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = INSPECT_RESULT_CODE;
    msgECDP_answer->msgContent.content.perception.inspectResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.inspectResult.pQueue =
            duplicateElementQueue(pBivalveQueue, sizeof(BIVALVE));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildHarvestResult
 *
 *  Build "HARVEST_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildHarvestResult(MSG_ID* msg, int result,
        double weight)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = HARVEST_RESULT_CODE;
    msgECDP_answer->msgContent.content.perception.harvestResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.harvestResult.actionResult = result;
    msgECDP_answer->msgContent.content.perception.harvestResult.weight = weight;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildDeleteAction
 *
 *  Build "SPECS_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildDeleteAction(char* destination, Queue* pRegNames)
{
    SPECS_ACTION* pSpecs;
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = DELETE_ACTION_CODE;
    pSpecs = &(msgECDP_action->msgContent.content.action.specs);
    pSpecs->pNames = duplicateElementQueue(pRegNames, ECDP_STRING);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildDefineResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildDefineResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, DEFINE_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildDeleteResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildDeleteResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, DELETE_RESULT_CODE, result);
}
//---------------------------------------------------------------------------


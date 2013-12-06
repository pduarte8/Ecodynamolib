//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_Public_4.cpp
 *
 *   PUBLIC methods invoked by Agents
 *
 *   BULK 4:
 *
 *      void buildOutputFileAction(char* destination, char* filename);
 *      void buildGetAvailableVariablesAction(char* destination);
 *      void buildSelectVariablesAction(char* destination, int type,
 *              Queue* pVarNames, BOXES* pBoxes);
 *      void buildLogAction(char* destination, LOGTYPE logType, Queue* pSteps);
 *      void buildGetOutputTimeAction(char* destination);
 *      void buildSetOutputTimeAction(char* destination, int step, int startT,
 *              int finishT);
 *      void buildTraceAction(char* destination);
 *      void buildOutputFileResult(MSG_ID* msg, int result);
 *      void buildVariablesAvailableResult(MSG_ID* msg, Queue* pVarNames);
 *      void buildSelectVariablesResult(MSG_ID* msg, int result);
 *      void buildLogResult(MSG_ID* msg, int result);
 *      void buildOutputTimeResult(MSG_ID* msg, int step, long startT, long finishT);
 *      void buildTraceResult(MSG_ID* msg, int traceStatus);
 *      void buildRegisterMsg(char* destination, int regIndex, int regTime,
 *              char* varName, Queue* pValues);
 *      void buildLoggerMsg(char* destination, int step, , Queue* pLogs);
 *      void buildEndSimulation(char* destination);
 *      void buildEndStep(char* destination);
 *
 *   == added in 30/01/07 by AP
 *      void buildUnSelectVariablesAction(char* destination, int type,
 *              Queue* pVarNames);
 *      void buildUnSelectVariablesResult(MSG_ID* msg, int result);
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
 *  NAME buildOutputFileAction
 *
 *  Build "OUTFILE_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildOutputFileAction(char* destination, char* filename)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = OUTPUT_FILE_CODE;
    strcpy(msgECDP_action->msgContent.content.action.outFile.name, filename);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetAvailableVariablesAction
 *
 *  Build Get Available Variables action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetAvailableVariablesAction(char* destination)
{
    return buildSimpleAction(destination, GET_AVAILABLE_VARIABLES_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSelectVariablesAction
 *
 *  Build "SEL_VAR_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSelectVariablesAction(char* destination, int type,
        Queue* pVarNames, BOXES* pBoxes)
{
    SEL_VAR_ACTION* pAction;
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SELECT_VARIABLES_CODE;
    pAction = &(msgECDP_action->msgContent.content.action.selVarAction);
    pAction->type = type;
    pAction->pVarNames = duplicateElementQueue(pVarNames, ECDP_STRING);

    pAction->boxes.type = pBoxes->type;
    if (pBoxes->type == BX_TYPE_CELLS) {
        pAction->boxes.domain.pCells =
                duplicateElementQueue(pBoxes->domain.pCells, sizeof(int));
    }
    else {
        pAction->boxes.domain.subDomain.type = pBoxes->domain.subDomain.type;
        if (pBoxes->domain.subDomain.type == DOM_TYPE_REG) {
            pAction->boxes.domain.subDomain.pRegNames =
                duplicateElementQueue(pBoxes->domain.subDomain.pRegNames, ECDP_STRING);
        }
        else
            pAction->boxes.domain.subDomain.pRegNames = NULL;
    }
    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildLogAction
 *
 *  Build "LOG_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildLogAction(char* destination, LOGTYPE logType, Queue* pSteps)
{
    LOG_ACTION* pAction;
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = LOG_CODE;
    pAction = &(msgECDP_action->msgContent.content.action.logAction);
    pAction->type = logType;
    pAction->pSteps = duplicateElementQueue(pSteps, sizeof(int));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildGetOutputTimeAction
 *
 *  Build Get Output Time action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildGetOutputTimeAction(char* destination)
{
    return buildSimpleAction(destination, GET_OUTPUT_TIME_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSetOutputTimeAction
 *
 *  Build "TIME_SPEC_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSetOutputTimeAction(char* destination, int step,
        long startT, long finishT)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = SET_OUTPUT_TIME_CODE;
    msgECDP_action->msgContent.content.action.timeSpec.step = step;
    msgECDP_action->msgContent.content.action.timeSpec.startTime = startT;
    msgECDP_action->msgContent.content.action.timeSpec.finishTime = finishT;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildTraceAction
 *
 *  Build Trace action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildTraceAction(char* destination)
{
    return buildSimpleAction(destination, TRACE_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildOutputFileResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildOutputFileResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, OUTPUT_FILE_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildVariablesAvailableResult
 *
 *  Build "VARCLASS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildVariablesAvailableResult(MSG_ID* msg, Queue* pVarNames)
{
    return buildVarClassResult(msg, pVarNames, VARIABLES_AVAILABLE_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSelectVariablesResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSelectVariablesResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, SELECT_VARIABLES_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildLogResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildLogResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, LOG_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildOutputTimeResult
 *
 *  Build "TIME_SPEC_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildOutputTimeResult(MSG_ID* msg, int step, long startT,
        long finishT)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = OUTPUT_TIME_CODE;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.step = step;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.startTime = startT;
    msgECDP_answer->msgContent.content.perception.timeSpecResult.finishTime = finishT;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildTraceResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildTraceResult(MSG_ID* msg, int traceStatus)
{
    return buildActionResult(msg, TRACE_RESULT_CODE, traceStatus);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildRegisterMsg
 *
 *  Build "REGISTER_MSG" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 *
 * AP, 2007.03.12 - message modified.
 */

int EcoDynProtocol::buildRegisterMsg(char* destination, int regIndex,
            int regTime, char* varName, Queue* pValues)
{
    REGISTER_MSG* pReg;
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_answer->messageId.receiver, destination);
    msgECDP_answer->msgContent.type = REGISTER_CODE;
    pReg = &(msgECDP_answer->msgContent.content.perception.registerMsg);
    pReg->regIndex = regIndex;
    pReg->regTime = regTime;
    strcpy(pReg->varName, varName);
    pReg->pQueue = duplicateElementQueue(pValues, sizeof(BOX_VALUE));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildRegisterMsg
 *
 *  Build "LOGGER_MSG" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildLoggerMsg(char* destination, int step, Queue* pLogs)
{
    LOGGER_MSG* pLogger;
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_answer->messageId.receiver, destination);
    msgECDP_answer->msgContent.type = LOGGER_CODE;
    pLogger = &(msgECDP_answer->msgContent.content.perception.loggerMsg);
    pLogger->step = step;
    pLogger->pQueue = duplicateElementQueue(pLogs, sizeof(LOG_MSG));

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildEndSimulation
 *
 *  Build End Simulation perception.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildEndSimulation(char* destination)
{
    return buildSimpleAction(destination, END_SIMULATION_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildEndStep
 *
 *  Build Trace action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildEndStep(char* destination)
{
    return buildSimpleAction(destination, END_STEP_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildUnSelectVariablesAction
 *
 *  Build "UNSEL_VAR_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildUnSelectVariablesAction(char* destination, int type,
        Queue* pVarNames)
{
    UNSEL_VAR_ACTION* pAction;
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = UNSELECT_VARIABLES_CODE;
    pAction = &(msgECDP_action->msgContent.content.action.unSelVarAction);
    pAction->type = type;
    pAction->pVarNames = duplicateElementQueue(pVarNames, ECDP_STRING);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildUnSelectVariablesResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildUnSelectVariablesResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, UNSELECT_VARIABLES_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildRunningMsg
 *
 *  Build Running perception.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildRunningMsg(char* destination)
{
    return buildSimpleAction(destination, RUNNING_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildStoppedMsg
 *
 *  Build Stopped perception.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildStoppedMsg(char* destination)
{
    return buildSimpleAction(destination, STOPPED_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildPausedMsg
 *
 *  Build Paused perception.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildPausedMsg(char* destination)
{
    return buildSimpleAction(destination, PAUSED_CODE);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_Public_2.cpp
 *
 *   PUBLIC methods invoked by Agents
 *
 *   BULK 2:
 *
 *      void buildOpenModelAction(MSG_ID* msg, char* modelName);
 *      void buildCloseModelAction(MSG_ID* msg);
 *      void buildModelNameAction(MSG_ID* msg);
 *      void buildInitialiseAction(MSG_ID* msg);
 *      void buildRunAction(MSG_ID* msg);
 *      void buildStopAction(MSG_ID* msg);
 *      void buildStepAction(MSG_ID* msg, int nSteps);
 *      void buildPauseAction(MSG_ID* msg);
 *      void buildOpenResult(MSG_ID* msg, int result);
 *      void buildCloseResult(MSG_ID* msg, int result);
 *      void buildModelNameResult(MSG_ID* msg, char* modelName);
 *      void buildExecResult(MSG_ID* msg, int result);
 *
 *   added in 2007.07.03 by AP:
 *      void buildSaveConfigurationAction(char* destination);
 *      void buildSaveResult(MSG_ID* msg, int result);
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
 *  NAME buildOpenModelAction
 *
 *  Build "MODEL_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildOpenModelAction(char* destination, char* modelName)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = OPEN_MODEL_CODE;
    strcpy(msgECDP_action->msgContent.content.action.model.name, modelName);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildCloseModelAction
 *
 *  Build Close model action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildCloseModelAction(char* destination)
{
    return buildSimpleAction(destination, CLOSE_MODEL_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildModelNameAction
 *
 *  Build Model name action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildModelNameAction(char* destination)
{
    return buildSimpleAction(destination, MODEL_NAME_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildInitialiseAction
 *
 *  Build Initialise action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildInitialiseAction(char* destination)
{
    return buildSimpleAction(destination, INITIALISE_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildRunAction
 *
 *  Build Run action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildRunAction(char* destination)
{
    return buildSimpleAction(destination, RUN_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildStopAction
 *
 *  Build Stop action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildStopAction(char* destination)
{
    return buildSimpleAction(destination, STOP_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildStepAction
 *
 *  Build "STEP_ACTION" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildStepAction(char* destination, int nSteps)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = STEP_CODE;
    msgECDP_action->msgContent.content.action.steps.nSteps = nSteps;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildPauseAction
 *
 *  Build Pause action.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildPauseAction(char* destination)
{
    return buildSimpleAction(destination, PAUSE_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildOpenResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildOpenResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, OPEN_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildCloseResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildCloseResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, CLOSE_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildModelNameResult
 *
 *  Bu "MODEL_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildModelNameResult(MSG_ID* msg, char* modelName)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = MODEL_CODE;
    msgECDP_answer->msgContent.content.perception.modelResult.id = msg->idMsg;
    strcpy(msgECDP_answer->msgContent.content.perception.modelResult.modelName,
            modelName);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildExecResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildExecResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, EXEC_RESULT_CODE, result);
}
//---------------------------------------------------------------------------
// AP, 2007.07.03
/*
 *  NAME buildSaveConfigurationAction
 *
 *  Insert SAVE_CONFIGURATION_CODE in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSaveConfigurationAction(char* destination)
{
    return buildSimpleAction(destination, SAVE_CONFIGURATION_CODE);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSaveResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSaveResult(MSG_ID* msg, int result)
{
    return buildActionResult(msg, SAVE_RESULT_CODE, result);
}
//---------------------------------------------------------------------------


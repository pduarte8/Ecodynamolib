//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_fmt_2.cpp
 *
 *   PRIVATE methods kicked by 'formatMessage' (ECDProtocol_Custom_fmt.cpp)
 *
 *   BULK 2:      started in 2005.05.05, AP
 *
 *     void fmtOpenModelAction(MODEL_ACTION* modelAction, char* contentStr)
 *     void fmtCloseModelAction(MSG_CONTENT* msgContent, char* contentStr)
 *     void fmtModelNameAction(MSG_CONTENT* msgContent, char* contentStr)
 *     void fmtInitialiseAction(MSG_CONTENT* msgContent, char* contentStr)
 *     void fmtRunAction(MSG_CONTENT* msgContent, char* contentStr)
 *     void fmtStopAction(MSG_CONTENT* msgContent, char* contentStr)
 *     void fmtStepAction(STEP_ACTION* stepAction, char* contentStr)
 *     void fmtPauseAction(MSG_CONTENT* msgContent, char* contentStr)
 *     void fmtOpenResult(ACTION_RESULT* openResult, char* contentStr)
 *     void fmtCloseResult(ACTION_RESULT* closeResult, char* contentStr)
 *     void fmtModelResult(MODEL_RESULT* modelResult, char* contentStr)
 *     void fmtExecResult(ACTION_RESULT* execResult, char* contentStr)
 *
 * added in 2007.07.03 by AP - ECOLANG v1.3b
 *     void fmtSaveConfigurationAction(SAVE_CONFIGURATION* saveAction, char* contentStr);
 *     void fmtSaveResult(ACTION_RESULT* saveResult, char* contentStr);
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
 * NAME fmtOpenModelAction
 *
 * fill message with MODEL_ACTION structure data
 */

void EcoDynProtocol::fmtOpenModelAction(MODEL_ACTION* modelAction, char* contentStr)
{
    sprintf(contentStr, "open_model %s", modelAction->name);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtCloseModelAction
 *
 * fill message with "close_model" text
 */

void EcoDynProtocol::fmtCloseModelAction(MSG_CONTENT* msgContent, char* contentStr)
{
    strcpy(contentStr, "close_model");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtModelNameAction
 *
 * fill message with "model_name" text
 */

void EcoDynProtocol::fmtModelNameAction(MSG_CONTENT* msgContent, char* contentStr)
{
    strcpy(contentStr, "model_name");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtInitialiseAction
 *
 * fill message with "initialise" text
 */

void EcoDynProtocol::fmtInitialiseAction(MSG_CONTENT* msgContent, char* contentStr)
{
    strcpy(contentStr, "initialise");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtRunAction
 *
 * fill message with "run" text
 */

void EcoDynProtocol::fmtRunAction(MSG_CONTENT* msgContent, char* contentStr)
{
    strcpy(contentStr, "run");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtStopAction
 *
 * fill message with "stop" text
 */

void EcoDynProtocol::fmtStopAction(MSG_CONTENT* msgContent, char* contentStr)
{
    strcpy(contentStr, "stop");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtStepAction
 *
 * fill message with STEP_ACTION structure data
 */

void EcoDynProtocol::fmtStepAction(STEP_ACTION* stepAction, char* contentStr)
{
    sprintf(contentStr, "step %d", stepAction->nSteps);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtPauseAction
 *
 * fill message with "pause" text
 */

void EcoDynProtocol::fmtPauseAction(MSG_CONTENT* msgContent, char* contentStr)
{
    strcpy(contentStr, "pause");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtOpenResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtOpenResult(ACTION_RESULT* openResult, char* contentStr)
{
    fmtActionResult(openResult, "open_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtCloseResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtCloseResult(ACTION_RESULT* closeResult, char* contentStr)
{
    fmtActionResult(closeResult, "close_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtModelResult
 *
 * fill message with MODEL_RESULT structure data
 */

void EcoDynProtocol::fmtModelResult(MODEL_RESULT* modelResult, char* contentStr)
{
    sprintf(contentStr, "model(%i %s)", modelResult->id, modelResult->modelName);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtExecResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtExecResult(ACTION_RESULT* execResult, char* contentStr)
{
    fmtActionResult(execResult, "exec_result", contentStr);
}
//---------------------------------------------------------------------------
// AP, 2007.07.03
/*
 * NAME fmtSaveConfigurationAction
 *
 * fill message with "save_configuration" text
 */

void EcoDynProtocol::fmtSaveConfigurationAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "save_configuration");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSaveResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtSaveResult(ACTION_RESULT* saveResult, char* contentStr)
{
    fmtActionResult(saveResult, "save_result", contentStr);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_get_2.cpp
 *
 *   PRIVATE methods kicked by 'saveMessageRx' (ECDProtocol_Custom_get.cpp)
 *
 *   BULK 2:      started in 2005.05.05, AP
 *
 *     void getOpenModelActionData
 *     void getCloseModelActionData
 *     void getModelNameActionData
 *     void getInitialiseActionData
 *     void getRunActionData
 *     void getStopActionData
 *     void getStepActionData
 *     void getPauseActionData
 *     void getOpenModelResultData
 *     void getCloseModelResultData
 *     void getModelNameResultData
 *     void getExecResultData
 *
 *  added in 2007.07.03 by AP - ECOLANG v1.3b
 *     void getSaveConfigurationActionData
 *     void getSaveResultData
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
 * NAME getOpenModelActionData
 *
 * fill MODEL_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with model information (modelName)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getOpenModelActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = OPEN_MODEL_CODE;
    strcpy(msgContent->content.action.model.name, tokens[index]);
}
//---------------------------------------------------------------------------
/*
 * NAME getCloseModelActionData
 *
 * fill MSG_CONTENT structure with CLOSE_MODEL_CODE in "type"
 */

void EcoDynProtocol::getCloseModelActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = CLOSE_MODEL_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getModelNameActionData
 *
 * fill MSG_CONTENT structure with MODEL_NAME_CODE in "type"
 */

void EcoDynProtocol::getModelNameActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = MODEL_NAME_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getInitialiseActionData
 *
 * fill MSG_CONTENT structure with INITIALISE_CODE in "type"
 */

void EcoDynProtocol::getInitialiseActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = INITIALISE_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getRunActionData
 *
 * fill MSG_CONTENT structure with RUN_CODE in "type"
 */

void EcoDynProtocol::getRunActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = RUN_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getStopActionData
 *
 * fill MSG_CONTENT structure with STOP_CODE in "type"
 */

void EcoDynProtocol::getStopActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = STOP_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getStepActionData
 *
 * 'n' - number of total tokens
 * 'index' - index of first token with number of steps information (nSteps)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getStepActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = STEP_CODE;
    msgContent->content.action.steps.nSteps = atoi(tokens[index]);
}
//---------------------------------------------------------------------------
/*
 * NAME getPauseActionData
 *
 * fill MSG_CONTENT structure with PAUSE_CODE in "type"
 */

void EcoDynProtocol::getPauseActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = PAUSE_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getOpenModelResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with open model action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with OPEN_RESULT_CODE code.
 */

void EcoDynProtocol::getOpenModelResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(OPEN_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getCloseModelResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with close model action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with CLOSE_RESULT_CODE code.
 */

void EcoDynProtocol::getCloseModelResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(CLOSE_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getModelNameResultData
 *
 * fill MODEL_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with model name action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getModelNameResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = MODEL_CODE;
    msgContent->content.perception.modelResult.id = atoi(tokens[index]);
    strcpy(msgContent->content.perception.modelResult.modelName, tokens[index + 1]);
}
//---------------------------------------------------------------------------
/*
 * NAME getExecResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with execution action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with EXEC_RESULT_CODE code.
 */

void EcoDynProtocol::getExecResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(EXEC_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
// AP, 2007.07.03
/*
 * NAME getSaveConfigurationActionData
 *
 * fill MSG_CONTENT structure with SAVE_CONFIGURATION_CODE in "type"
 */

void EcoDynProtocol::getSaveConfigurationActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = SAVE_CONFIGURATION_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getSaveResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with execution action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with SAVE_RESULT_CODE code.
 */

void EcoDynProtocol::getSaveResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(SAVE_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_fmt_4.cpp
 *
 *   PRIVATE methods kicked by 'formatMessage' (ECDProtocol_Custom_fmt.cpp)
 *
 *   BULK 4:      started in 2005.06.27, AP
 *
 *     void fmtOutputFileAction(OUTFILE_ACTION* outfileAction, char* contentStr);
 *     void fmtGetAvailableVariablesAction(MSG_CONTENT* msgContent,  char* contentStr);
 *     void fmtSelectVariablesAction(SEL_VAR_ACTION* selVarAction,  char* contentStr);
 *     void fmtLogAction(LOG_ACTION* logAction,  char* contentStr);
 *     void fmtGetOutputTimeAction(MSG_CONTENT* msgContent,  char* contentStr);
 *     void fmtSetOutputTimeAction(TIME_SPEC_ACTION* outputTimeAction,  char* contentStr);
 *     void fmtTraceAction(MSG_CONTENT* msgContent,  char* contentStr);
 *     void fmtOutputFileResult(ACTION_RESULT* regResult,  char* contentStr);
 *     void fmtVariablesAvailableResult(VARCLASS_RESULT* varsResult, char* contentStr);
 *     void fmtSelectVariablesResult(ACTION_RESULT* regResult,  char* contentStr);
 *     void fmtLogResult(ACTION_RESULT* regResult,  char* contentStr);
 *     void fmtOutputTimeResult(TIME_SPEC_RESULT* timeSpecResult,  char* contentStr);
 *     void fmtTraceResult(ACTION_RESULT* traceResult,  char* contentStr);
 *     void fmtRegisterMsg(REGISTER_MSG* registerMsg,  char* contentStr);
 *     void fmtLoggerMsg(LOGGER_MSG* loggerMsg,  char* contentStr);
 *     void fmtEndSimulationMsg(MSG_CONTENT* msgContent,  char* contentStr);
 *     void fmtEndStepMsg(MSG_CONTENT* msgContent,  char* contentStr);
 *
 *     == added in 2007.01.30 by AP
 *     void fmtUnSelectVariablesAction(UNSEL_VAR_ACTION* unSelVarAction,  char* contentStr);
 *     void fmtUnSelectVariablesResult(ACTION_RESULT* regResult,  char* contentStr);
 *
 *     == added in 2007.02.27 by AP
 *     void fmtRunningMsg(MSG_CONTENT* msgContent,  char* contentStr);
 *     void fmtStoppedMsg(MSG_CONTENT* msgContent,  char* contentStr);
 *     void fmtPausedMsg(MSG_CONTENT* msgContent,  char* contentStr);
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
 * NAME fmtOutputFileAction
 *
 * fill message with output filename
 */

void EcoDynProtocol::fmtOutputFileAction(OUTFILE_ACTION* outfileAction,
        char* contentStr)
{
    sprintf(contentStr, "output_file %s", outfileAction->name);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetAvailableVariablesAction
 *
 * fill message with "get_available_variables" structure data
 */

void EcoDynProtocol::fmtGetAvailableVariablesAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "get_available_variables");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSelectVariablesAction
 *
 * fill message with SEL_VAR_ACTION structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtSelectVariablesAction(SEL_VAR_ACTION* selVarAction,
        char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBufVar;
    BOXES* pBoxes;
    char* pVarName;
    int i;
    char type[16];

    if (selVarAction->type == SV_TYPE_GRAPH)
        strcpy(type, "graph");
    else if (selVarAction->type == SV_TYPE_TABLE)
        strcpy(type, "table");
    else if (selVarAction->type == SV_TYPE_REMOTE)
        strcpy(type, "remote");
    else
        strcpy(type, "file");
    sprintf(contentStr, "select_variables %s (", type);
    for (i = 0; i < selVarAction->pVarNames->size(); i++) {
        pBufVar = selVarAction->pVarNames->getElementAt(i);
        pVarName = (char*)pBufVar->pData;
        replaceParenthesesByBrackets(varName, pVarName);
        appendEnclosedName(contentStr, varName);
    }
    strcat(contentStr, ") (");
    pBoxes = &selVarAction->boxes;
    if (pBoxes->type == BX_TYPE_SUBDOMAIN) {
        appendSubDomain(contentStr, &(pBoxes->domain.subDomain));
    }
    else {
        appendCells(contentStr, pBoxes->domain.pCells);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtLogAction
 *
 * fill message with LOG_ACTION structure data
 */

void EcoDynProtocol::fmtLogAction(LOG_ACTION* logAction,  char* contentStr)
{
    BUF* pBufStep;
    int* pInt;
    int i;
    char type[16];

    if (logAction->type == XML)
        strcpy(type, "xml");
    else if (logAction->type == XLS)
        strcpy(type, "xls");
    else if (logAction->type == TXT)
        strcpy(type, "txt");
    else
        strcpy(type, "remote");
    sprintf(contentStr, "log %s (", type);
    appendCells(contentStr, logAction->pSteps);
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetOutputTimeAction
 *
 * fill message with "get_output_time" structure data
 */

void EcoDynProtocol::fmtGetOutputTimeAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "get_output_time");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSetOutputTimeAction
 *
 * fill message with TIME_SPEC_ACTION structure data
 */

void EcoDynProtocol::fmtSetOutputTimeAction(TIME_SPEC_ACTION* outputTimeAction,
        char* contentStr)
{
    sprintf(contentStr, "set_output_time %i %i %i",
            outputTimeAction->step, outputTimeAction->startTime,
            outputTimeAction->finishTime);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtTraceAction
 *
 * fill message with "trace" structure data
 */

void EcoDynProtocol::fmtTraceAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "trace");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtOutputFileResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtOutputFileResult(ACTION_RESULT* regResult,
        char* contentStr)
{
    fmtActionResult(regResult, "output_file_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtVariablesAvailableResult
 *
 * fill message with VARCLASS_RESULT structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtVariablesAvailableResult(VARCLASS_RESULT* varsResult,
        char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBufName;
    char* pName;
    int i;

    sprintf(contentStr, "variables_available(%i ", varsResult->id);
    for (i = 0; i < varsResult->pQueue->size(); i++) {
        pBufName = varsResult->pQueue->getElementAt(i);
        pName = (char*)pBufName->pData;
        replaceParenthesesByBrackets(varName, pName);
        appendEnclosedName(contentStr, varName);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSelectVariablesResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtSelectVariablesResult(ACTION_RESULT* regResult,
        char* contentStr)
{
    fmtActionResult(regResult, "select_variables_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtLogResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtLogResult(ACTION_RESULT* regResult,
        char* contentStr)
{
    fmtActionResult(regResult, "log_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtOutputTimeResult
 *
 * fill message with TIME_SPEC_RESULT structure data
 */

void EcoDynProtocol::fmtOutputTimeResult(TIME_SPEC_RESULT* timeSpecResult,
        char* contentStr)
{
    sprintf(contentStr, "output_time(%i %i %ld %ld)",
            timeSpecResult->id, timeSpecResult->step,
            timeSpecResult->startTime, timeSpecResult->finishTime);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtTraceResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtTraceResult(ACTION_RESULT* traceResult,
        char* contentStr)
{
    sprintf(contentStr, "trace_result(%i ", traceResult->id);
    if (traceResult->actionResult == RESULT_OK)
        strcat (contentStr, "on)");
    else
        strcat (contentStr, "off)");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtRegisterMsg
 *
 * fill message with REGISTER_MSG structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 *  AP, 2007.03.12 - message modified
 */

void EcoDynProtocol::fmtRegisterMsg(REGISTER_MSG* registerMsg, char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBuf;
    BOX_VALUE* pBoxValue;

    sprintf(contentStr, "register(%i %i ",
            registerMsg->regIndex, registerMsg->regTime);
    replaceParenthesesByBrackets(varName, registerMsg->varName);
    appendEnclosedName(contentStr, varName);

    for (int i = 0; i < registerMsg->pQueue->size(); i++) {
        pBuf = registerMsg->pQueue->getElementAt(i);
        pBoxValue = (BOX_VALUE*)pBuf->pData;
        sprintf(&contentStr[strlen(contentStr)], " (%i %f)",
                pBoxValue->cell, pBoxValue->value);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtLoggerMsg
 *
 * fill message with LOGGER_MSG structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 *  AP, 2007.03.15 - message modified
 */

void EcoDynProtocol::fmtLoggerMsg(LOGGER_MSG* loggerMsg, char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBuf;
    LOG_MSG* pLogMsg;

    sprintf(contentStr, "logger(%i ", loggerMsg->step);

    for (int i = 0; i < loggerMsg->pQueue->size(); i++) {
        pBuf = loggerMsg->pQueue->getElementAt(i);
        pLogMsg = (LOG_MSG*)pBuf->pData;
        sprintf(&contentStr[strlen(contentStr)], " ((%s) %s (%s) ",
                pLogMsg->className, pLogMsg->funcType, pLogMsg->dataClass);
        replaceParenthesesByBrackets(varName, pLogMsg->varName);
        appendEnclosedName(contentStr, varName);
        sprintf(&contentStr[strlen(contentStr)], " %i %f) ",
                pLogMsg->cell, pLogMsg->value);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEndSimulationMsg
 *
 * fill message with "end_simulation" structure data
 */

void EcoDynProtocol::fmtEndSimulationMsg(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "end_simulation");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEndStepMsg
 *
 * fill message with "end_step" structure data
 */

void EcoDynProtocol::fmtEndStepMsg(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "end_step");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtUnSelectVariablesAction
 *
 * fill message with UNSEL_VAR_ACTION structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtUnSelectVariablesAction(UNSEL_VAR_ACTION* unSelVarAction,
        char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBufVar;
    char* pVarName;
    int i;
    char type[16];

    if (unSelVarAction->type == SV_TYPE_GRAPH)
        strcpy(type, "graph");
    else if (unSelVarAction->type == SV_TYPE_TABLE)
        strcpy(type, "table");
    else if (unSelVarAction->type == SV_TYPE_REMOTE)
        strcpy(type, "remote");
    else
        strcpy(type, "file");
    sprintf(contentStr, "unselect_variables %s ", type);
    for (i = 0; i < unSelVarAction->pVarNames->size(); i++) {
        pBufVar = unSelVarAction->pVarNames->getElementAt(i);
        pVarName = (char*)pBufVar->pData;
        replaceParenthesesByBrackets(varName, pVarName);
        appendEnclosedName(contentStr, varName);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtUnSelectVariablesResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtUnSelectVariablesResult(ACTION_RESULT* regResult,
        char* contentStr)
{
    fmtActionResult(regResult, "unselect_variables_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtRunningMsg
 *
 * fill message with "running" structure data
 */

void EcoDynProtocol::fmtRunningMsg(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "running");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtStoppedMsg
 *
 * fill message with "stopped" structure data
 */

void EcoDynProtocol::fmtStoppedMsg(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "stopped");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtPausedMsg
 *
 * fill message with "paused" structure data
 */

void EcoDynProtocol::fmtPausedMsg(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "paused");
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_get_4.cpp
 *
 *   PRIVATE methods kicked by 'saveMessageRx' (ECDProtocol_Custom_get.cpp)
 *
 *   BULK 4:      started in 2005.06.27, AP
 *
 *     void getOutputFileActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getGetAvailableVariablesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getSelectVariablesActionData(char* type, TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getLogActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getGetOutputTimeActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getSetOutputTimeActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getTraceActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getOutputFileResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getVariablesAvailableResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getSelectVariablesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getLogResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getOutputTimeResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getTraceResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getRegisterMsgData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getLoggerMsgData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getEndSimulationData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getEndStepData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *
 *     == added in 2007.01.30 by AP
 *     void getUnSelectVariablesActionData(char* type, TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getUnSelectVariablesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *
 *     == added in 2007.02.27 by AP
 *     void getRunningData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getStoppedData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *     void getPausedData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
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
 * NAME getOutputFileActionData
 *
 * fill OUTFILE_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getOutputFileActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = OUTPUT_FILE_CODE;
    myParser->trim(msgContent->content.action.outFile.name, tokensT[index]);
}
//---------------------------------------------------------------------------
/*
 * NAME getGetAvailableVariablesActionData
 *
 * fill MSG_CONTENT structure with GET_AVAILABLE_VARIABLES_CODE in "type"
 */

void EcoDynProtocol::getGetAvailableVariablesActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = GET_AVAILABLE_VARIABLES_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getSelectVariablesActionData
 *
 * fill SEL_VAR_ACTION structure with information given in 'tokensT'
 * 'type' - output mode
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with variable name information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 */

void EcoDynProtocol::getSelectVariablesActionData(char* type, TOKEN* tokensT,
        int nT, int index, MSG_CONTENT* msgContent)
{
    char text[ECDP_STRING];
    SEL_VAR_ACTION* svAction;
    int ni;
    BUF* pBufName;
    BUF* pBufCell;
    BOXES* pBoxes;
    R_DOMAIN* pDomain;
    int* pInt;

    msgContent->type = SELECT_VARIABLES_CODE;
    svAction = &msgContent->content.action.selVarAction;

    if (strcmp(type, "graph") == 0)
        svAction->type = SV_TYPE_GRAPH;
    else if (strcmp(type, "table") == 0)
        svAction->type = SV_TYPE_TABLE;
    else if (strcmp(type, "remote") == 0)
        svAction->type = SV_TYPE_REMOTE;
    else
        svAction->type = SV_TYPE_FILE;

    svAction->pVarNames = new Queue();
    ni = getNextNotEmptyToken(tokensT, nT, index);

    // read variable names
    while ((strstr(tokensT[ni], "subdomain") == NULL) &&
            ((tokensT[ni][0] < '0') || (tokensT[ni][0] > '9'))) {
        myParser->trim(text, tokensT[ni]);
        if (strlen(text) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            replaceBracketsByParentheses((char*)pBufName->pData, text);
            svAction->pVarNames->insertElement(pBufName);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }

    pBoxes = &svAction->boxes;
    if (strstr(tokensT[ni], "subdomain") != NULL) {
        pBoxes->type = BX_TYPE_SUBDOMAIN;
        pDomain = &pBoxes->domain.subDomain;
        if (strstr(tokensT[ni], "all") != NULL) {
            pDomain->type = DOM_TYPE_ALL;
            pDomain->pRegNames = NULL;
        }
        else {
            pDomain->type = DOM_TYPE_REG;
            pDomain->pRegNames = new Queue();

            int n = myParser->getNrOfTokens(tokensT[ni + 1], " ");
            TOKEN* tokens = new TOKEN[n];
            myParser->parseTokens(tokensT[ni + 1], " ", tokens);

            int i = 0;
            while (i < n) {
                myParser->trim(text, tokens[i]);
                if (strlen(text) > 0) {
                    if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                        break;
                    }
                    strcpy((char*)pBufName->pData, text);
                    pDomain->pRegNames->insertElement(pBufName);
                }
                i++;
            }
            delete [] tokens;
        }
    }
    else {
        pBoxes->type = BX_TYPE_CELLS;
        pBoxes->domain.pCells = new Queue();

        int n = myParser->getNrOfTokens(tokensT[ni], " ");
        TOKEN* tokens = new TOKEN[n];
        myParser->parseTokens(tokensT[ni], " ", tokens);

        int i = 0;
        while (i < n) {
            myParser->trim(text, tokens[i]);
            if (strlen(text) > 0) {
                if ((pBufCell = Queue::allocBuf(sizeof(int))) == NULL) {
                    break;
                }
                pInt = (int*)pBufCell->pData;
                *pInt = atoi(text);
                pBoxes->domain.pCells->insertElement(pBufCell);
            }
            i++;
        }
        delete [] tokens;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getLogActionData
 *
 * fill LOG_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with subdomain information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getLogActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    LOG_ACTION* logAction;
    int i;
    BUF* pBufInt;
    int* pInt;

    msgContent->type = LOG_CODE;
    logAction = &msgContent->content.action.logAction;
    if (strcmp(tokens[index], "xml") == 0)
        logAction->type = XML;
    else if (strcmp(tokens[index], "xls") == 0)
        logAction->type = XLS;
    else if (strcmp(tokens[index], "txt") == 0)
        logAction->type = TXT;
    else
        logAction->type = REMOTE;

    logAction->pSteps = new Queue();
    i = index + 1;
    while (i < n) {
        if (strlen(tokens[i]) > 0) {
            if ((pBufInt = Queue::allocBuf(sizeof(int))) == NULL) {
                break;
            }
            pInt = (int*)pBufInt->pData;
            *pInt = atoi(tokens[i]);
            logAction->pSteps->insertElement(pBufInt);
        }
        i++;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getGetOutputTimeActionData
 *
 * fill MSG_CONTENT structure with GET_OUTPUT_TIME_CODE in "type"
 */

void EcoDynProtocol::getGetOutputTimeActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = GET_OUTPUT_TIME_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getSetOutputTimeActionData
 *
 * fill TIME_SPEC_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with step information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getSetOutputTimeActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = SET_OUTPUT_TIME_CODE;
    msgContent->content.action.timeSpec.step = atoi(tokens[index]);
    msgContent->content.action.timeSpec.startTime = atoi(tokens[index + 1]);
    msgContent->content.action.timeSpec.finishTime = atoi(tokens[index + 2]);
}
//---------------------------------------------------------------------------
/*
 * NAME getTraceActionData
 *
 * fill MSG_CONTENT structure with TRACE_CODE in "type"
 */

void EcoDynProtocol::getTraceActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = TRACE_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getOutputFileResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with OUTPUT_FILE_RESULT_CODE code.
 */

void EcoDynProtocol::getOutputFileResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(OUTPUT_FILE_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getVariablesAvailableResultData
 *
 * fill VARCLASS_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with action ID (id)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 */

void EcoDynProtocol::getVariablesAvailableResultData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    char varName[ECDP_STRING];
    int ni;
    BUF* pBufName;
    Queue* pQueue;

    msgContent->type = VARIABLES_AVAILABLE_CODE;
    msgContent->content.perception.varClassResult.id = atoi(tokensT[index]);
    msgContent->content.perception.varClassResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.varClassResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        myParser->trim(varName, tokensT[ni]);
        if (strlen(varName) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            replaceBracketsByParentheses((char*)pBufName->pData, varName);
            pQueue->insertElement(pBufName);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getSelectVariablesResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with SELECT_VARIABLES_RESULT_CODE code.
 */

void EcoDynProtocol::getSelectVariablesResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(SELECT_VARIABLES_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getLogResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with LOG_RESULT_CODE code.
 */

void EcoDynProtocol::getLogResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(LOG_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getOutputTimeResultData
 *
 * fill TIME_SPEC_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getOutputTimeResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = OUTPUT_TIME_CODE;
    msgContent->content.perception.timeSpecResult.id = atoi(tokens[index]);
    msgContent->content.perception.timeSpecResult.step = atoi(tokens[index + 1]);
    msgContent->content.perception.timeSpecResult.startTime = atol(tokens[index + 2]);
    msgContent->content.perception.timeSpecResult.finishTime = atol(tokens[index + 3]);
}
//---------------------------------------------------------------------------
/*
 * NAME getTraceResultData
 *
 * fill VAR_VALUE_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getTraceResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    int result;

    msgContent->type = TRACE_RESULT_CODE;
    msgContent->content.perception.result.id = atoi(tokens[index]);
    if (strcmp(tokens[index + 1], "on") == 0)
        result = RESULT_OK;
    else
        result = RESULT_FAILED;
    msgContent->content.perception.result.actionResult = result;
}
//---------------------------------------------------------------------------
/*
 * NAME getRegisterMsgData
 *
 * fill REGISTER_MSG structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with REG_INDEX
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 *  AP, 2007.03.12 - message modified
 */

void EcoDynProtocol::getRegisterMsgData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    char varName[ECDP_STRING];
    REGISTER_MSG* pReg;
    int n, ni;
    TOKEN* tokens;
    BUF* pBuf;
    BOX_VALUE* pBoxValue;

    msgContent->type = REGISTER_CODE;
    pReg = &msgContent->content.perception.registerMsg;

    n = myParser->getNrOfTokens(tokensT[index], " ");
    tokens = new TOKEN[n];
    myParser->parseTokens(tokensT[index], " ", tokens);
    pReg->regIndex = atoi(tokens[0]);
    pReg->regTime = atoi(tokens[1]);
    delete [] tokens;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    myParser->trim(varName, tokensT[ni]);
    replaceBracketsByParentheses(pReg->varName, varName);

    /* every element has 2 tokens */
    pReg->pQueue = new Queue();
    tokens = new TOKEN[2];

    ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    while (ni < nT)
    {
        if ((pBuf = Queue::allocBuf(sizeof(BOX_VALUE))) == NULL) {
            break;
        }
// ** n = 2 **     n = myParser->getNrOfTokens(tokensT[ni], " ");
        myParser->parseTokens(tokensT[ni], " ", tokens);

        pBoxValue = (BOX_VALUE*)pBuf->pData;
        pBoxValue->cell = atoi(tokens[0]);
        pBoxValue->value = atof(tokens[1]);
        pReg->pQueue->insertElement(pBuf);
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
    delete [] tokens;
}
//---------------------------------------------------------------------------
/*
 * NAME getLoggerMsgData
 *
 * fill LOGGER_MSG structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with STEP
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 *  AP, 2007.03.15 - message modified
 */

void EcoDynProtocol::getLoggerMsgData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    char varName[ECDP_STRING];
    LOGGER_MSG* pLog;
    int n, ni;
    TOKEN* tokens;
    BUF* pBuf;
    LOG_MSG* pLogMsg;

    msgContent->type = LOGGER_CODE;
    pLog = &msgContent->content.perception.loggerMsg;
    pLog->step = atoi(tokensT[index]);

    /* every element has 2 tokens */
    pLog->pQueue = new Queue();
    tokens = new TOKEN[2];

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT)
    {
        if ((pBuf = Queue::allocBuf(sizeof(LOG_MSG))) == NULL) {
            break;
        }
        pLogMsg = (LOG_MSG*)pBuf->pData;
        myParser->trim(pLogMsg->className, tokensT[ni]);

        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
        myParser->trim(pLogMsg->funcType, tokensT[ni]);

        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
        myParser->trim(pLogMsg->dataClass, tokensT[ni]);

        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
        myParser->trim(varName, tokensT[ni]);
        replaceBracketsByParentheses(pLogMsg->varName, varName);

        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
        myParser->parseTokens(tokensT[ni], " ", tokens);

        pLogMsg->cell = atoi(tokens[0]);
        pLogMsg->value = atof(tokens[1]);
        pLog->pQueue->insertElement(pBuf);
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
    delete [] tokens;
}
//---------------------------------------------------------------------------
/*
 * NAME getEndSimulationData
 *
 * fill MSG_CONTENT structure with END_SIMULATION_CODE in "type"
 */

void EcoDynProtocol::getEndSimulationData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = END_SIMULATION_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getEndStepData
 *
 * fill MSG_CONTENT structure with END_STEP_CODE in "type"
 */

void EcoDynProtocol::getEndStepData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = END_STEP_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getUnSelectVariablesActionData
 *
 * fill UNSEL_VAR_ACTION structure with information given in 'tokensT'
 * 'type' - output mode
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with variable name information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 */

void EcoDynProtocol::getUnSelectVariablesActionData(char* type, TOKEN* tokensT,
        int nT, int index, MSG_CONTENT* msgContent)
{
    char varName[ECDP_STRING];
    UNSEL_VAR_ACTION* svAction;
    int ni;
    BUF* pBufName;

    msgContent->type = UNSELECT_VARIABLES_CODE;
    svAction = &msgContent->content.action.unSelVarAction;

    if (strcmp(type, "graph") == 0)
        svAction->type = SV_TYPE_GRAPH;
    else if (strcmp(type, "table") == 0)
        svAction->type = SV_TYPE_TABLE;
    else if (strcmp(type, "remote") == 0)
        svAction->type = SV_TYPE_REMOTE;
    else
        svAction->type = SV_TYPE_FILE;

    svAction->pVarNames = new Queue();
    ni = getNextNotEmptyToken(tokensT, nT, index);

    // read variable names
    while (ni < nT) {
        myParser->trim(varName, tokensT[ni]);
        if (strlen(varName) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            replaceBracketsByParentheses((char*)pBufName->pData, varName);
            svAction->pVarNames->insertElement(pBufName);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getUnSelectVariablesResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with UNSELECT_VARIABLES_RESULT_CODE code.
 */

void EcoDynProtocol::getUnSelectVariablesResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(UNSELECT_VARIABLES_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getRunningData
 *
 * fill MSG_CONTENT structure with RUNNING_CODE in "type"
 */

void EcoDynProtocol::getRunningData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = RUNNING_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getStoppedData
 *
 * fill MSG_CONTENT structure with STOPPED_CODE in "type"
 */

void EcoDynProtocol::getStoppedData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = STOPPED_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getPausedData
 *
 * fill MSG_CONTENT structure with PAUSED_CODE in "type"
 */

void EcoDynProtocol::getPausedData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = PAUSED_CODE;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_get_3.cpp
 *
 *   PRIVATE methods kicked by 'saveMessageRx' (ECDProtocol_Custom_get.cpp)
 *
 *   BULK 2:      started in 2005.06.20, AP
 *
 *     void getGetAvailableClassesActionData
 *     void getGetSelectedClassesActionData
 *     void getSelectClassesActionData
 *     void getGetVariablesActionData
 *     void getGetVariableValueActionData
 *     void getSetVariableValueActionData
 *     void getGetParametersActionData
 *     void getSetParametersActionData
 *     void getGetTimeSpecActionData
 *     void getSetTimeSpecActionData
 *     void getSubDomainActionData
 *     void getClassesAvailableResultData
 *     void getClassesSelectedResultData
 *     void getVariablesResultData
 *     void getVariableValueResultData
 *     void getVariableSetResultData
 *     void getParametersValuesResultData
 *     void getParametersSetResultData
 *     void getTimeSpecResultData
 *     void getSubDomainResultData
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
 * NAME getGetAvailableClassesActionData
 *
 * fill MSG_CONTENT structure with GET_AVAILABLE_CLASSES_CODE in "type"
 */

void EcoDynProtocol::getGetAvailableClassesActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = GET_AVAILABLE_CLASSES_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getGetSelectedClassesActionData
 *
 * fill MSG_CONTENT structure with GET_SELECTED_CLASSES_CODE in "type"
 */

void EcoDynProtocol::getGetSelectedClassesActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = GET_SELECTED_CLASSES_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getSelectClassesActionData
 *
 * fill SPECS_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with 1st class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getSelectClassesActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBuf;
    char text[ECDP_STRING];
    Queue* pQueue;

    msgContent->type = SELECT_CLASSES_CODE;
    msgContent->content.action.specs.pNames = new Queue();
    pQueue = msgContent->content.action.specs.pNames;
    ni = getNextNotEmptyToken(tokensT, nT, index);
    while (ni < nT) {
        myParser->trim(text, tokensT[ni]);
        if (strlen(text) > 0) {
            if ((pBuf = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            strcpy((char*)pBuf->pData, text);
            pQueue->insertElement(pBuf);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni+1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getGetVariablesActionData
 *
 * fill CLASS_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getGetVariablesActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = GET_VARIABLES_CODE;
    myParser->trim(msgContent->content.action.clAction.name, tokensT[index]);
}
//---------------------------------------------------------------------------
/*
 * NAME getGetVariableValueActionData
 *
 * fill VAR_VALUE_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 */

void EcoDynProtocol::getGetVariableValueActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    char text[ECDP_STRING];
    int ni;

    msgContent->type = GET_VARIABLE_VALUE_CODE;
    myParser->trim(msgContent->content.action.varValue.className, tokensT[index]);
    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    myParser->trim(text, tokensT[ni]);
    replaceBracketsByParentheses(msgContent->content.action.varValue.varName, text);

    BOXES* pBoxes;
    R_DOMAIN* pDomain;
    int* pInt;

    ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    pBoxes = &msgContent->content.action.varValue.boxes;
    if (strstr(tokensT[ni], "subdomain") != NULL) {
        pBoxes->type = BX_TYPE_SUBDOMAIN;
        pDomain = &pBoxes->domain.subDomain;

        if (strstr(tokensT[ni], "all") != NULL) {
            pDomain->type = DOM_TYPE_ALL;
            pDomain->pRegNames = NULL;
        }
        else {
            BUF* pBufName;

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
        BUF* pBufCell;

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
 * NAME getSetVariableValueActionData
 *
 * fill SET_VALUE_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 */

void EcoDynProtocol::getSetVariableValueActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    char text[ECDP_STRING];
    int ni;
    BUF* pBufVar;
    BUF* pBufCell;
    BUF* pBufName;
    VAR_VALUE* pVarValue;
    BOXES* pBoxes;
    R_DOMAIN* pDomain;
    int* pInt;
    Queue* pQueue;

    msgContent->type = SET_VARIABLE_VALUE_CODE;
    myParser->trim(msgContent->content.action.setValue.className, tokensT[index]);
    msgContent->content.action.setValue.pValues = new Queue();
    pQueue = msgContent->content.action.setValue.pValues;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        if ((pBufVar = Queue::allocBuf(sizeof(VAR_VALUE))) == NULL) {
            break;
        }
        pVarValue = (VAR_VALUE*)pBufVar->pData;
        myParser->trim(text, tokensT[ni]);
        replaceBracketsByParentheses(pVarValue->name, text);
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);

        pBoxes = &pVarValue->boxes;
        if (strstr(tokensT[ni], "subdomain") != NULL) {
            pBoxes->type = BX_TYPE_SUBDOMAIN;
            pDomain = &pBoxes->domain.subDomain;

            if (strstr(tokensT[ni], "all") != NULL) {
                pDomain->type = DOM_TYPE_ALL;
                pDomain->pRegNames = NULL;
                pVarValue->value = atof(tokensT[ni + 1]);
                ni = getNextNotEmptyToken(tokensT, nT, ni + 2);
            }
            else {
                pDomain->type = DOM_TYPE_REG;
                pVarValue->value = atof(tokensT[ni + 2]);
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
                ni = getNextNotEmptyToken(tokensT, nT, ni + 3);
            }
        }
        else {
            pBoxes->type = BX_TYPE_CELLS;
            pVarValue->value = atof(tokensT[ni + 1]);
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
            ni = getNextNotEmptyToken(tokensT, nT, ni + 2);
        }
        pQueue->insertElement(pBufVar);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getGetParametersActionData
 *
 * fill CLASS_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getGetParametersActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = GET_PARAMETERS_CODE;
    myParser->trim(msgContent->content.action.clAction.name, tokensT[index]);
}
//---------------------------------------------------------------------------
/*
 * NAME getSetParametersActionData
 *
 * fill SET_VALUE_ACTION structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with class name information (className)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'parmName'
 */

void EcoDynProtocol::getSetParametersActionData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    char parmName[ECDP_STRING];
    int ni;
    BUF* pBufVar;
    PARM_VALUE* pParmValue;
    Queue* pQueue;

    msgContent->type = SET_PARAMETERS_CODE;
    myParser->trim(msgContent->content.action.setValue.className, tokensT[index]);
    msgContent->content.action.setValue.pValues = new Queue();
    pQueue = msgContent->content.action.setValue.pValues;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        myParser->trim(parmName, tokensT[ni]);
        if (strlen(parmName) > 0) {
            if ((pBufVar = Queue::allocBuf(sizeof(VAR_VALUE))) == NULL) {
                break;
            }
            pParmValue = (PARM_VALUE*)pBufVar->pData;
            replaceBracketsByParentheses(pParmValue->name, parmName);
            pParmValue->value = atof(tokensT[ni + 1]);
            pQueue->insertElement(pBufVar);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 2);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getGetTimeSpecActionData
 *
 * fill MSG_CONTENT structure with GET_TIME_SPEC_CODE in "type"
 */

void EcoDynProtocol::getGetTimeSpecActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = GET_TIME_SPEC_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getSetTimeSpecActionData
 *
 * fill TIME_SPEC_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with step information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getSetTimeSpecActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = SET_TIME_SPEC_CODE;
    msgContent->content.action.timeSpec.step = atoi(tokens[index]);
    msgContent->content.action.timeSpec.startTime = atoi(tokens[index + 1]);
    msgContent->content.action.timeSpec.finishTime = atoi(tokens[index + 2]);
}
//---------------------------------------------------------------------------
/*
 * NAME getSubDomainActionData
 *
 * fill SUBDOMAIN_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with subdomain information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getSubDomainActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    int i;
    BUF* pBufName;
    char* pName;
    Queue *pQueue;

    msgContent->type = SUBDOMAIN_CODE;
    if (strcmp(tokens[index], "all ") == 0) {
        msgContent->content.action.subDomain.type = DOM_TYPE_ALL;
        msgContent->content.action.subDomain.pRegNames = NULL;
    }
    else {
        msgContent->content.action.subDomain.type = DOM_TYPE_REG;
        msgContent->content.action.subDomain.pRegNames = new Queue();
        pQueue = msgContent->content.action.subDomain.pRegNames;

        i = index;
        while (i < n) {
            if (strlen(tokens[i]) > 0) {
                if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                    break;
                }
                pName = (char*)pBufName->pData;
                strcpy(pName, tokens[i]);
                pQueue->insertElement(pBufName);
            }
            i++;
        }
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getClassesAvailableResultData
 *
 * fill VARCLASS_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with action ID (id)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getClassesAvailableResultData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBufName;
    char text[ECDP_STRING];
    Queue* pQueue;

    msgContent->type = CLASSES_AVAILABLE_CODE;
    msgContent->content.perception.varClassResult.id = atoi(tokensT[index]);
    msgContent->content.perception.varClassResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.varClassResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        myParser->trim(text, tokensT[ni]);
        if (strlen(text) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            strcpy((char*)pBufName->pData, text);
            pQueue->insertElement(pBufName);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getClassesSelectedResultData
 *
 * fill VARCLASS_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with action ID (id)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getClassesSelectedResultData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBufName;
    char text[ECDP_STRING];
    Queue* pQueue;

    msgContent->type = CLASSES_SELECTED_CODE;
    msgContent->content.perception.varClassResult.id = atoi(tokensT[index]);
    msgContent->content.perception.varClassResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.varClassResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        myParser->trim(text, tokensT[ni]);
        if (strlen(text) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            strcpy((char*)pBufName->pData, text);
            pQueue->insertElement(pBufName);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getVariablesResultData
 *
 * fill VARCLASS_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with action ID (id)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'varName'
 */

void EcoDynProtocol::getVariablesResultData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBufName;
    char text[ECDP_STRING];
    Queue* pQueue;

    msgContent->type = VARIABLES_CODE;
    msgContent->content.perception.varClassResult.id = atoi(tokensT[index]);
    msgContent->content.perception.varClassResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.varClassResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        myParser->trim(text, tokensT[ni]);
        if (strlen(text) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            replaceBracketsByParentheses((char*)pBufName->pData, text);
            pQueue->insertElement(pBufName);
        }
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getVariableValueResultData
 *
 * fill VAR_VALUE_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getVariableValueResultData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    int ni;
    Queue* pQueue;

    msgContent->type = VARIABLE_VALUE_CODE;
    msgContent->content.perception.varValueResult.id = atoi(tokens[index]);
    msgContent->content.perception.varValueResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.varValueResult.pQueue;

    buildBoxValues(tokens, n, index + 1, pQueue);
}
//---------------------------------------------------------------------------
/*
 * NAME getVariableSetResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with VARIABLE_SET_RESULT_CODE code.
 */

void EcoDynProtocol::getVariableSetResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(VARIABLE_SET_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getParametersValuesResultData
 *
 * fill PARMS_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  AP, 2007.02.07 - replaced brackets by parentheses in 'parmName'
 */

void EcoDynProtocol::getParametersValuesResultData(TOKEN* tokensT, int nT,
        int index, MSG_CONTENT* msgContent)
{
    char parmName[ECDP_STRING];
    int ni;
    BUF* pBufVar;
    PARM_VALUE* pParmValue;
    Queue* pQueue;

    msgContent->type = PARAMETERS_VALUES_CODE;
    msgContent->content.perception.parmsResult.id = atoi(tokensT[index]);
    msgContent->content.perception.parmsResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.parmsResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        myParser->trim(parmName, tokensT[ni]);
        if (strlen(parmName) > 0) {
            if ((pBufVar = Queue::allocBuf(sizeof(PARM_VALUE))) == NULL) {
                break;
            }
            pParmValue = (PARM_VALUE*)pBufVar->pData;
            replaceBracketsByParentheses(pParmValue->name, parmName);
            pParmValue->value = atof(tokensT[ni + 1]);
            pQueue->insertElement(pBufVar);
            ni = getNextNotEmptyToken(tokensT, nT, ni + 2);
        }
        else
            ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getParametersSetResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with PARAMETERS_SET_RESULT_CODE code.
 */

void EcoDynProtocol::getParametersSetResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(PARAMETERS_SET_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getTimeSpecResultData
 *
 * fill TIME_SPEC_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getTimeSpecResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = TIME_SPEC_CODE;
    msgContent->content.perception.timeSpecResult.id = atoi(tokens[index]);
    msgContent->content.perception.timeSpecResult.step = atoi(tokens[index + 1]);
    msgContent->content.perception.timeSpecResult.startTime = atol(tokens[index + 2]);
    msgContent->content.perception.timeSpecResult.finishTime = atol(tokens[index + 3]);
}
//---------------------------------------------------------------------------
/*
 * NAME getSubDomainResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with SUBDOMAIN_RESULT_CODE code.
 */

void EcoDynProtocol::getSubDomainResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(SUBDOMAIN_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------


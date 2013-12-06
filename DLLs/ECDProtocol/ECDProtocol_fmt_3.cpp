//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_fmt_3.cpp
 *
 *   PRIVATE methods kicked by 'formatMessage' (ECDProtocol_Custom_fmt.cpp)
 *
 *   BULK 3:      started in 2005.06.25, AP
 *
 *     void fmtGetAvailableClassesAction
 *     void fmtGetSelectedClassesAction
 *     void fmtSelectClassesAction
 *     void fmtGetVariablesAction
 *     void fmtGetVariableValueAction
 *     void fmtSetVariableValueAction
 *     void fmtGetParametersAction
 *     void fmtSetParametersAction
 *     void fmtGetTimeSpecAction
 *     void fmtSetTimeSpecAction
 *     void fmtSubDomainAction
 *     void fmtClassesAvailableResult
 *     void fmtClassesSelectedResult
 *     void fmtVariablesResult
 *     void fmtVariableValueResult
 *     void fmtVariableSetResult
 *     void fmtParametersValuesResult
 *     void fmtParametersSetResult
 *     void fmtTimeSpecResult
 *     void fmtSubDomainResult
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
 * NAME fmtGetAvailableClassesAction
 *
 * fill message with "get_available_classes" structure data
 */

void EcoDynProtocol::fmtGetAvailableClassesAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "get_available_classes");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetSelectedClassesAction
 *
 * fill message with "get_selected_classes" text
 */

void EcoDynProtocol::fmtGetSelectedClassesAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "get_selected_classes");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSelectClassesAction
 *
 * fill message with SPECS_ACTION structure data
 */

void EcoDynProtocol::fmtSelectClassesAction(SPECS_ACTION* specsAction,
        char* contentStr)
{
    BUF* pBufName;
    char* pName;
    int i;

    strcpy(contentStr, "select_classes ");
    for (i = 0; i < specsAction->pNames->size(); i++) {
        pBufName = specsAction->pNames->getElementAt(i);
        pName = (char*)pBufName->pData;
        appendEnclosedName(contentStr, pName);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetVariablesAction
 *
 * fill message with CLASS_ACTION structure data
 */

void EcoDynProtocol::fmtGetVariablesAction(CLASS_ACTION* clAction,
        char* contentStr)
{
    sprintf(contentStr, "get_variables (%s)", clAction->name);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetVariableValueAction
 *
 * fill message with VAR_VALUE_ACTION structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtGetVariableValueAction(VAR_VALUE_ACTION* varValueAction,
        char* contentStr)
{
    char varName[ECDP_STRING];

    replaceParenthesesByBrackets(varName, varValueAction->varName);
    sprintf(contentStr, "get_variable_value (%s) (%s) ",
            varValueAction->className, varName);

    strcat(contentStr, "(");
    BOXES* pBoxes = &varValueAction->boxes;
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
 * NAME fmtSetVariableValueAction
 *
 * fill message with SET_VALUE_ACTION structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtSetVariableValueAction(SET_VALUE_ACTION* setValueAction,
        char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBufVar;
    VAR_VALUE* pVar;
    BOXES* pBoxes;
    int i;

    sprintf(contentStr, "set_variable_value (%s)", setValueAction->className);
    for (i = 0; i < setValueAction->pValues->size(); i++) {
        pBufVar = setValueAction->pValues->getElementAt(i);
        pVar = (VAR_VALUE*)pBufVar->pData;
        strcat(contentStr, "(");
        replaceParenthesesByBrackets(varName, pVar->name);
        appendEnclosedName(contentStr, varName);
        strcat(contentStr, "(");
        pBoxes = &pVar->boxes;
        if (pBoxes->type == BX_TYPE_SUBDOMAIN) {
            appendSubDomain(contentStr, &(pBoxes->domain.subDomain));
        }
        else {
            appendCells(contentStr, pBoxes->domain.pCells);
        }
        sprintf(&contentStr[strlen(contentStr)], ") %f)", pVar->value);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetParametersAction
 *
 * fill message with CLASS_ACTION structure data
 */

void EcoDynProtocol::fmtGetParametersAction(CLASS_ACTION* clAction,
        char* contentStr)
{
    sprintf(contentStr, "get_parameters (%s)", clAction->name);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSetParametersAction
 *
 * fill message with SET_VALUE_ACTION structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'parmName'
 */

void EcoDynProtocol::fmtSetParametersAction(SET_VALUE_ACTION* setValueAction,
        char* contentStr)
{
    char parmName[ECDP_STRING];
    BUF* pBufParm;
    PARM_VALUE* pParm;
    int i;

    sprintf(contentStr, "set_parameters (%s)", setValueAction->className);
    for (i = 0; i < setValueAction->pValues->size(); i++) {
        pBufParm = setValueAction->pValues->getElementAt(i);
        pParm = (PARM_VALUE*)pBufParm->pData;
        strcat(contentStr, "(");
        replaceParenthesesByBrackets(parmName, pParm->name);
        appendEnclosedName(contentStr, parmName);
        sprintf(&contentStr[strlen(contentStr)], " %f)", pParm->value);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetTimeSpecAction
 *
 * fill message with "get_time_spec" structure data
 */

void EcoDynProtocol::fmtGetTimeSpecAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "get_time_spec");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSetTimeSpecAction
 *
 * fill message with TIME_SPEC_ACTION structure data
 */

void EcoDynProtocol::fmtSetTimeSpecAction(TIME_SPEC_ACTION* timeSpecAction,
        char* contentStr)
{
    sprintf(contentStr, "set_time_spec %i %i %i",
            timeSpecAction->step, timeSpecAction->startTime,
            timeSpecAction->finishTime);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSubDomainAction
 *
 * fill message with SUBDOMAIN_ACTION structure data
 */

void EcoDynProtocol::fmtSubDomainAction(SUBDOMAIN_ACTION* subDomainAction,
        char* contentStr)
{
    contentStr[0] = '\0';
    appendSubDomain(contentStr, subDomainAction);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtClassesAvailableResult
 *
 * fill message with VARCLASS_RESULT structure data
 */

void EcoDynProtocol::fmtClassesAvailableResult(VARCLASS_RESULT* clsResult,
        char* contentStr)
{
    BUF* pBufName;
    char* pName;
    int i;

    sprintf(contentStr, "classes_available(%i ", clsResult->id);
    for (i = 0; i < clsResult->pQueue->size(); i++) {
        pBufName = clsResult->pQueue->getElementAt(i);
        pName = (char*)pBufName->pData;
        appendEnclosedName(contentStr, pName);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtClassesSelectedResult
 *
 * fill message with VARCLASS_RESULT structure data
 */

void EcoDynProtocol::fmtClassesSelectedResult(VARCLASS_RESULT* clsResult,
        char* contentStr)
{
    BUF* pBufName;
    char* pName;
    int i;

    sprintf(contentStr, "classes_selected(%i ", clsResult->id);
    for (i = 0; i < clsResult->pQueue->size(); i++) {
        pBufName = clsResult->pQueue->getElementAt(i);
        pName = (char*)pBufName->pData;
        appendEnclosedName(contentStr, pName);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtVariablesResult
 *
 * fill message with VARCLASS_RESULT structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtVariablesResult(VARCLASS_RESULT* varsResult,
        char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBufName;
    char* pName;
    int i;

    sprintf(contentStr, "variables(%i ", varsResult->id);
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
 * NAME fmtVariableValueResult
 *
 * fill message with VAR_VALUE_RESULT structure data
 */

void EcoDynProtocol::fmtVariableValueResult(VAR_VALUE_RESULT* varValueResult,
        char* contentStr)
{
  BUF* pBuf;
  BOX_VALUE* pBoxValue;

    sprintf(contentStr, "variable_value(%i", varValueResult->id);
    for (int i = 0; i < varValueResult->pQueue->size(); i++) {
        pBuf = varValueResult->pQueue->getElementAt(i);
        pBoxValue = (BOX_VALUE*)pBuf->pData;
        sprintf(&contentStr[strlen(contentStr)], " (%i %f)",
                pBoxValue->cell, pBoxValue->value);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtVariableSetResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtVariableSetResult(ACTION_RESULT* specsResult,
        char* contentStr)
{
    fmtActionResult(specsResult, "variables_set_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtParametersValuesResult
 *
 * fill message with PARMS_RESULT structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'parmName'
 */

void EcoDynProtocol::fmtParametersValuesResult(PARMS_RESULT* parmsResult,
        char* contentStr)
{
    char parmName[ECDP_STRING];
    BUF* pBufParm;
    PARM_VALUE* pParm;
    int i;

    sprintf(contentStr, "parameters_values (%i ", parmsResult->id);
    for (i = 0; i < parmsResult->pQueue->size(); i++) {
        pBufParm = parmsResult->pQueue->getElementAt(i);
        pParm = (PARM_VALUE*)pBufParm->pData;
        strcat(contentStr, "(");
        replaceParenthesesByBrackets(parmName, pParm->name);
        appendEnclosedName(contentStr, parmName);
        sprintf(&contentStr[strlen(contentStr)], " %f)", pParm->value);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtParametersSetResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtParametersSetResult(ACTION_RESULT* specsResult,
        char* contentStr)
{
    fmtActionResult(specsResult, "parameters_set_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtTimeSpecResult
 *
 * fill message with TIME_SPEC_RESULT structure data
 */

void EcoDynProtocol::fmtTimeSpecResult(TIME_SPEC_RESULT* timeSpecResult,
        char* contentStr)
{
    sprintf(contentStr, "time_spec(%i %i %ld %ld)",
            timeSpecResult->id, timeSpecResult->step,
            timeSpecResult->startTime, timeSpecResult->finishTime);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSubDomainResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtSubDomainResult(ACTION_RESULT* specsResult,
        char* contentStr)
{
    fmtActionResult(specsResult, "subdomain_result", contentStr);
}
//---------------------------------------------------------------------------


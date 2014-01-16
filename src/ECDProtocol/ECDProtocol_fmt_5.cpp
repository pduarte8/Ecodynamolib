//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_fmt_5.cpp
 *
 *   PRIVATE methods kicked by 'formatMessage' (ECDProtocol_Custom_fmt.cpp)
 *
 *   BULK 5:      started in 2006.05.16, AP
 *
 *     void fmtConnectAction
 *     void fmtDisconnectAction
 *     void fmtAcceptResult
 *
 *     == added in 2007.05.07 by AP
 *     void fmtGetRegionNamesAction
 *     void fmtGetRegionAction
 *     void fmtModelDimensionsAction
 *     void fmtModelMorphologyAction
 *     void fmtModelSpeciesAction
 *     void fmtRegionNamesResult
 *     void fmtRegionResult
 *     void fmtDimensionsResult
 *     void fmtMorphologyResult
 *     void fmtBenthicSpeciesResult
 *
 *     == added in 2007.05.07 by AP
 *     void fmtMorphologyEndResult
 *     void fmtBenthicSpeciesEndResult
 *
 *     == added in 2008.05.21 by AP
 *     void fmtAgentsAction
 *     void fmtKnownAgentsResult
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
 * NAME fmtConnectAction
 *
 * fill message with connect information
 */

void EcoDynProtocol::fmtConnectAction(CONNECT_ACTION* connectAction,
        char* contentStr)
{
    sprintf(contentStr, "connect %s %s %i", connectAction->hostname,
            connectAction->hostaddr, connectAction->portServer);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtDisconnectAction
 *
 * fill message with "disconnect" data
 */

void EcoDynProtocol::fmtDisconnectAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "disconnect");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtAcceptResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtAcceptResult(ACTION_RESULT* acceptResult,
        char* contentStr)
{
    fmtActionResult(acceptResult, "accept", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetRegionNamesAction
 *
 * fill message with "get_region_names" data
 */

void EcoDynProtocol::fmtGetRegionNamesAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "get_region_names");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtGetRegionAction
 *
 * fill message with "get_region_names" data
 * AP, 2012.02.28 - Bug fixed in sprintf
 */

void EcoDynProtocol::fmtGetRegionAction(REGION_ACTION* regAction,
        char* contentStr)
{
    sprintf(contentStr, "get_region %s", regAction->name);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtModelDimensionsAction
 *
 * fill message with "model_dimensions" data
 */

void EcoDynProtocol::fmtModelDimensionsAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "model_dimensions");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtModelMorphologyAction
 *
 * fill message with "model_morphology" data
 */

void EcoDynProtocol::fmtModelMorphologyAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "model_morphology");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtModelSpeciesAction
 *
 * fill message with "model_species" data
 */

void EcoDynProtocol::fmtModelSpeciesAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "model_species");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtRegionNamesResult
 *
 * fill message with REGION_NAMES_RESULT structure data
 * AP, 2012.02.28 - Bug fixed: separate regionNames by space.
 */

void EcoDynProtocol::fmtRegionNamesResult(REGION_NAMES_RESULT* regNamesResult,
        char* contentStr)
{
    BUF* pBufName;
    char* pName;
    int i, nR;

    sprintf(contentStr, "region_names (%i ", regNamesResult->id);
    nR = regNamesResult->pQueue->size();
    for (i = 0; i < nR; i++) {
        pBufName = regNamesResult->pQueue->getElementAt(i);
        strcat(contentStr, (char*)pBufName->pData);
        if (i < (nR - 1))
            strcat(contentStr, " ");
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtRegionResult
 *
 * fill message with REGION_RESULT structure data
 */

void EcoDynProtocol::fmtRegionResult(REGION_RESULT* regionResult,
        char* contentStr)
{
    sprintf(contentStr, "region (%d %s ", regionResult->id,
            regionResult->definition.regName);
    fmtRegion(&regionResult->definition.region, &contentStr[strlen(contentStr)]);
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtDimensionsResult
 *
 * fill message with DIMENSIONS_RESULT structure data
 */

void EcoDynProtocol::fmtDimensionsResult(DIMENSIONS_RESULT* dimensionsResult,
        char* contentStr)
{
    sprintf(contentStr, "dimensions (%d %d %d %d %s)", dimensionsResult->id,
            dimensionsResult->lines, dimensionsResult->columns,
            dimensionsResult->layers, dimensionsResult->modelType);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtMorphologyResult
 *
 * fill message with MORPHOLOGY_RESULT structure data
 */

void EcoDynProtocol::fmtMorphologyResult(MORPHOLOGY_RESULT* morphologyResult,
        char* contentStr)
{
  BUF* pBuf;
  BOX_VALUE* pBoxValue;

    sprintf(contentStr, "morphology (%i", morphologyResult->id);
    for (int i = 0; i < morphologyResult->pQueue->size(); i++) {
        pBuf = morphologyResult->pQueue->getElementAt(i);
        pBoxValue = (BOX_VALUE*)pBuf->pData;
        sprintf(&contentStr[strlen(contentStr)], " (%i %f)",
                pBoxValue->cell, pBoxValue->value);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtBenthicSpeciesResult
 *
 * fill message with BENTHIC_SPECIES_RESULT structure data
 *
 *  AP, 2007.02.07 - replaced parentheses by brackets in 'varName'
 */

void EcoDynProtocol::fmtBenthicSpeciesResult(BENTHIC_SPECIES_RESULT* benthicSpeciesResult,
        char* contentStr)
{
    char varName[ECDP_STRING];
    BUF* pBufSp;
    SPECIES_VALUE* pSpecies;
    BOXES* pBoxes;
    int i;

    sprintf(contentStr, "benthic_species (%i", benthicSpeciesResult->id);
    for (i = 0; i < benthicSpeciesResult->pQueue->size(); i++) {
        pBufSp = benthicSpeciesResult->pQueue->getElementAt(i);
        pSpecies = (SPECIES_VALUE*)pBufSp->pData;
        strcat(contentStr, " (");
        appendEnclosedName(contentStr, pSpecies->name);
        strcat(contentStr, " (");
        pBoxes = &pSpecies->boxes;
        if (pBoxes->type == BX_TYPE_SUBDOMAIN) {
            appendSubDomain(contentStr, &(pBoxes->domain.subDomain));
        }
        else {
            appendCells(contentStr, pBoxes->domain.pCells);
        }
        strcat(contentStr, "))");
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtMorphologyEndResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtMorphologyEndResult(ACTION_RESULT* endResult, char* contentStr)
{
    fmtActionResult(endResult, "morphology_end", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtBenthicSpeciesEndResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtBenthicSpeciesEndResult(ACTION_RESULT* endResult, char* contentStr)
{
    fmtActionResult(endResult, "benthic_species_end", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtAgentsAction
 *
 * fill message with "agents" data
 */

void EcoDynProtocol::fmtAgentsAction(MSG_CONTENT* msgContent,
        char* contentStr)
{
    strcpy(contentStr, "agents");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtKnownAgentsResult
 *
 * fill message with AGENTS_RESULT structure data
 */

void EcoDynProtocol::fmtKnownAgentsResult(AGENTS_RESULT* agentsResult,
        char* contentStr)
{
    BUF* pBufAg;
    AGENT_DATA* pAgent;
    int i;
    char agentDesc[ECDP_STRING];

    sprintf(contentStr, "known_agents (%i", agentsResult->id);
    for (i = 0; i < agentsResult->pQueue->size(); i++) {
        pBufAg = agentsResult->pQueue->getElementAt(i);
        pAgent = (AGENT_DATA*)pBufAg->pData;
        sprintf(agentDesc, " (%s %s %s %i ", pAgent->agentName,
                pAgent->hostName, pAgent->hostAddr, pAgent->serverPort);
        strcat(agentDesc, (pAgent->connected == CONNECTED ? "connected)" : "disconnected)"));
        strcat(contentStr, agentDesc);
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------


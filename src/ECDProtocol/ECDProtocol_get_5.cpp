//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_get_5.cpp
 *
 *   PRIVATE methods kicked by 'saveMessageRx' (ECDProtocol_Custom_get.cpp)
 *
 *   BULK 5:      started in 2006.05.16, AP
 *
 *     void getConnectActionData
 *     void getDisconnectActionData
 *     void getAcceptResultData
 *
 *     == added in 2007.05.07 by AP
 *     void getGetRegionNamesActionData
 *     void getGetRegionActionData
 *     void getModelDimensionsActionData
 *     void getModelMorphologyActionData
 *     void getModelSpeciesActionData
 *     void getRegionNamesResultData
 *     void getRegionResultData
 *     void getDimensionsResultData
 *     void getMorphologyResultData
 *     void getBenthicSpeciesResultData
 *
 *     == added in 2007.05.07 by AP
 *     void getMorphologyEndResultData
 *     void getBenthicSpeciesEndResultData
 *
 *     == added in 2008.05.21 by AP
 *     void getAgentsActionData
 *     void getKnownAgentsResultData
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
#else   // __BORLANDC__
	#include <cstdio>
    #include <cstring>
	#include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//---------------------------------------------------------------------------
/*
 * NAME getConnectActionData
 *
 * fill CONNECT_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with hostname information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getConnectActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    CONNECT_ACTION* connectAction;
    int i;

    msgContent->type = CONNECT_CODE;
    connectAction = &msgContent->content.action.connectAction;
    strcpy(connectAction->hostname, tokens[index]);
    strcpy(connectAction->hostaddr, tokens[index + 1]);
    connectAction->portServer = atoi(tokens[index + 2]);
}
//---------------------------------------------------------------------------
/*
 * NAME getDisconnectActionData
 *
 * fill MSG_CONTENT structure with DISCONNECT_CODE in "type"
 */

void EcoDynProtocol::getDisconnectActionData(TOKEN* tokens, int n,
        int index, MSG_CONTENT* msgContent)
{
    msgContent->type = DISCONNECT_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getAcceptResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with ACCEPT_CODE code.
 */

void EcoDynProtocol::getAcceptResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(ACCEPT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getGetRegionNamesActionData
 *
 * fill MSG_CONTENT structure with REGION_NAMES_ACTION_CODE in "type"
 */

void EcoDynProtocol::getGetRegionNamesActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = REGION_NAMES_ACTION_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getGetRegionActionData
 *
 * fill REGION_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of token with region name information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getGetRegionActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = GET_REGION_ACTION_CODE;
    strcpy(msgContent->content.action.regionAction.name, tokens[index]);
}
//---------------------------------------------------------------------------
/*
 * NAME getModelDimensionsActionData
 *
 * fill MSG_CONTENT structure with MODEL_DIMENSIONS_ACTION_CODE in "type"
 */

void EcoDynProtocol::getModelDimensionsActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = MODEL_DIMENSIONS_ACTION_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getModelMorphologyActionData
 *
 * fill MSG_CONTENT structure with MODEL_MORPHOLOGY_ACTION_CODE in "type"
 */

void EcoDynProtocol::getModelMorphologyActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = MODEL_MORPHOLOGY_ACTION_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getModelSpeciesActionData
 *
 * fill MSG_CONTENT structure with MODEL_SPECIES_ACTION_CODE in "type"
 */

void EcoDynProtocol::getModelSpeciesActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = MODEL_SPECIES_ACTION_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getRegionNamesResultData
 *
 * fill REGION_NAMES_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with message_id information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getRegionNamesResultData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBufName;
    char regName[ECDP_STRING];
    Queue* pQueue;

    msgContent->type = REGION_NAMES_RESULT_CODE;
    msgContent->content.perception.regionNamesResult.id = atoi(tokens[index]);
    msgContent->content.perception.regionNamesResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.regionNamesResult.pQueue;

    ni = index + 1;
    while (ni < n)
    {
        myParser->trim(regName, tokens[ni]);
        if (strlen(regName) > 0) {
            if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            strcpy((char*)pBufName->pData, regName);
            pQueue->insertElement(pBufName);
        }
        ni++;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getRegionResultData
 *
 * fill REGION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with message_id information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getRegionResultData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
  REGION_RESULT *pRegResult;

    msgContent->type = REGION_RESULT_CODE;
    pRegResult = &msgContent->content.perception.regionResult;
    pRegResult->id = atoi(tokens[index]);
    strcpy(pRegResult->definition.regName, tokens[index+1]);
    getRegion(tokens, n, index + 2, &(pRegResult->definition.region));
}
//---------------------------------------------------------------------------
/*
 * NAME getDimensionsResultData
 *
 * fill DIMENSIONS_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with message_id information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getDimensionsResultData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
  DIMENSIONS_RESULT *pDimResult;

    msgContent->type = MODEL_DIMENSIONS_RESULT_CODE;
    pDimResult = &msgContent->content.perception.dimensionsResult;
    pDimResult->id = atoi(tokens[index]);
    pDimResult->lines = atoi(tokens[index+1]);
    pDimResult->columns = atoi(tokens[index+2]);
    pDimResult->layers = atoi(tokens[index+3]);
    strcpy(pDimResult->modelType, tokens[index+4]);
}
//---------------------------------------------------------------------------
/*
 * NAME getMorphologyResultData
 *
 * fill MORPHOLOGY_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with message_id information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getMorphologyResultData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    Queue* pQueue;
    
    msgContent->type = MODEL_MORPHOLOGY_RESULT_CODE;
    msgContent->content.perception.morphologyResult.id = atoi(tokens[index]);
    msgContent->content.perception.morphologyResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.morphologyResult.pQueue;

    buildBoxValues(tokens, n, index + 1, pQueue);
}
//---------------------------------------------------------------------------
/*
 * NAME getBenthicSpeciesResultData
 *
 * fill BENTHIC_SPECIES_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with message_id information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getBenthicSpeciesResultData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBufSp;
    BUF* pBufName;
    BUF* pBufCell;
    SPECIES_VALUE* pSpecies;
    BOXES* pBoxes;
    R_DOMAIN* pDomain;
    int* pInt;
    Queue* pQueue;
    char text[ECDP_STRING];

    msgContent->type = MODEL_SPECIES_RESULT_CODE;
    msgContent->content.perception.benthicSpeciesResult.id = atoi(tokensT[index]);
    msgContent->content.perception.benthicSpeciesResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.benthicSpeciesResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        if ((pBufSp = Queue::allocBuf(sizeof(SPECIES_VALUE))) == NULL) {
            break;
        }
        pSpecies = (SPECIES_VALUE*)pBufSp->pData;
        myParser->trim(pSpecies->name, tokensT[ni]);
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);

        pBoxes = &pSpecies->boxes;
        if (strstr(tokensT[ni], "subdomain") != NULL) {
            pBoxes->type = BX_TYPE_SUBDOMAIN;
            pDomain = &pBoxes->domain.subDomain;

            if (strstr(tokensT[ni], "all") != NULL) {
                pDomain->type = DOM_TYPE_ALL;
                pDomain->pRegNames = NULL;
                ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
            }
            else {
                pDomain->type = DOM_TYPE_REG;
                pDomain->pRegNames = new Queue();

                ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
                int n = myParser->getNrOfTokens(tokensT[ni], " ");
                TOKEN* tokens = new TOKEN[n];
                myParser->parseTokens(tokensT[ni], " ", tokens);

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
                ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
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
            ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
        }
        pQueue->insertElement(pBufSp);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getMorphologyEndResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with open model action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with MODEL_MORPHOLOGY_END_CODE code.
 */

void EcoDynProtocol::getMorphologyEndResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(MODEL_MORPHOLOGY_END_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getBenthicSpeciesEndResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with open model action ID
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 *
 *  call function 'getActionResultData' with MODEL_SPECIES_END_CODE code.
 */

void EcoDynProtocol::getBenthicSpeciesEndResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(MODEL_SPECIES_END_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getAgentsActionData
 *
 * fill MSG_CONTENT structure with AGENTS_CODE in "type"
 */

void EcoDynProtocol::getAgentsActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = AGENTS_CODE;
}
//---------------------------------------------------------------------------
/*
 * NAME getKnownAgentsResultData
 *
 * fill AGENTS_RESULT structure with information given in 'tokensT'
 * 'nT' - number of total tokens enclosed by parentesis
 * 'index' - index of first token with message_id information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getKnownAgentsResultData(TOKEN* tokensT, int nT, int index,
        MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBuf;
    AGENT_DATA* pAgentD;
    int* pInt;
    Queue* pQueue;
    char text[ECDP_STRING];

    msgContent->type = KNOWN_AGENTS_CODE;
    msgContent->content.perception.agentsResult.id = atoi(tokensT[index]);
    msgContent->content.perception.agentsResult.pQueue = new Queue();
    pQueue = msgContent->content.perception.agentsResult.pQueue;

    ni = getNextNotEmptyToken(tokensT, nT, index + 1);
    while (ni < nT) {
        if ((pBuf = Queue::allocBuf(sizeof(AGENT_DATA))) == NULL) {
            break;
        }
        pAgentD = (AGENT_DATA*)pBuf->pData;

        int n = myParser->getNrOfTokens(tokensT[ni], " ");
        TOKEN* tokens = new TOKEN[n];
        myParser->parseTokens(tokensT[ni], " ", tokens);

        strcpy(pAgentD->agentName, tokens[0]);
        strcpy(pAgentD->hostName, tokens[1]);
        strcpy(pAgentD->hostAddr, tokens[2]);
        pAgentD->serverPort = atoi(tokens[3]);
        if (strcmp(tokens[4], "connected") == 0)
            pAgentD->connected = CONNECTED;
        else
            pAgentD->connected = DISCONNECTED;
        pQueue->insertElement(pBuf);

        delete [] tokens;
        ni = getNextNotEmptyToken(tokensT, nT, ni + 1);
    }
}
//---------------------------------------------------------------------------


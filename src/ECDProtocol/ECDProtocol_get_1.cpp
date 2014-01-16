//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_get_1.cpp
 *
 *   PRIVATE methods kicked by 'saveMessageRx' (ECDProtocol_Custom_get.cpp)
 *
 *   BULK 1:
 *
 *    void getDefinitionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getSeedActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getInspectActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getHarvestActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getActionResultData(int code, TOKEN* tokens, int index, MSG_CONTENT* msgContent);
 *    void getSeedResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getInspectResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getHarvestResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getBivalves(TOKEN* tokens, int n, int index, Queue* pBivalveQueue);
 *    void getBivalve(TOKEN* tokens, int n, int index, BIVALVE* pBivalve);
 *    void getBivalve_S(TOKEN* tokens, int n, int index, BIVALVE_S* pBivalve_s);
 *    void getRegion(TOKEN* tokens, int n, int index, REGION* pRegion);
 *    void getSubRegion(TOKEN* tokens, int n, int index, SUB_REGION* pSubRegion);
 *    void getRegionNames(TOKEN* tokens, int n, int index, Queue* pRegNames);
 *    void getRegionArea(TOKEN* tokens, int n, int index, Queue* pRegionArea);
 *    void getEcoPoint(TOKEN* tokens, int n, int index, ECOPOINT* pPoint);
 *    void getEcoRect(TOKEN* tokens, int n, int index, ECORECT* pRect);
 *    void getEcoSquare(TOKEN* tokens, int n, int index, ECOSQUARE* psquare);
 *    void getEcoCircle(TOKEN* tokens, int n, int index, ECOCIRCLE* pCircle);
 *    void getEcoArc(TOKEN* tokens, int n, int index, ECOARC* pArc);
 *    void getEcoTime(TOKEN* tokens, int n, int index, ECOTIME* pTime);
 *
 *    == added in 2007.01.30 by AP
 *    void getDeleteActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getDefineResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
 *    void getDeleteResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
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
	#include <iostream>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//---------------------------------------------------------------------------
/*
 * NAME getDefinitionData
 *
 * fill DEFINITION_MSG structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with definition information (regName)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getDefinitionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = DEFINE_CODE;
    strcpy(msgContent->content.definition.regName, tokens[index]);
    getRegion(tokens, n, index + 1, &(msgContent->content.definition.region));
}
//---------------------------------------------------------------------------
/*
 * NAME getRegion
 *
 * fill REGION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with REGION information
 * 'pRegion' - pointer to REGION structure to fill
 */

void EcoDynProtocol::getRegion(TOKEN* tokens, int n, int index,
        REGION* pRegion)
{
    if (stricmp(tokens[index], "land") == 0
        || stricmp(tokens[index], "subtidal") == 0
        || stricmp(tokens[index], "intertidal") == 0) {
        pRegion->type = RG_TYPE_REGION;
        getSubRegion(tokens, n, index, &(pRegion->region.reg));
    }
    else {
        pRegion->type = RG_TYPE_REG_NAME;
        pRegion->region.pRegNames = new Queue();
        getRegionNames(tokens, n, index, pRegion->region.pRegNames);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getRegionNames
 *
 * fill Region_Names Queue with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with region name
 * 'pRegNames' - Queue entry for Regions Names
 */

void EcoDynProtocol::getRegionNames(TOKEN* tokens, int n, int index,
        Queue* pRegNames)
{
    BUF* pBuf;
    char* pStr;

    for (int i = index; i < n; i++) {
        if (strlen(tokens[index]) > 0) {
            if ((pBuf = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            pStr = (char*)pBuf->pData;
            strcpy(pStr, tokens[i]);
            pRegNames->insertElement(pBuf);
        }
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getSubRegion
 *
 * fill SUB_REGION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with SUB_REGION information
 * 'pSubRegion' - pointer to SUB_REGION structure to fill
 */

void EcoDynProtocol::getSubRegion(TOKEN* tokens, int n, int index,
        SUB_REGION* pSubRegion)
{
    int indexR;

    if (stricmp(tokens[index], "land") == 0) {
        pSubRegion->regionType.type = RT_TYPE_LAND;
        indexR = index + 1;
    }
    else {  // water
        pSubRegion->regionType.type = RT_TYPE_WATER;
        if (stricmp(tokens[index], "subtidal") == 0)
            pSubRegion->regionType.waterRegion.type = WR_TYPE_SUBTIDAL;
        else // intertidal
            pSubRegion->regionType.waterRegion.type = WR_TYPE_INTERTIDAL;

        if (stricmp(tokens[index + 1], "excellent") == 0)
            pSubRegion->regionType.waterRegion.quality = QUALITY_EXCELLENT;
        else if (stricmp(tokens[index + 1], "good") == 0)
            pSubRegion->regionType.waterRegion.quality = QUALITY_GOOD;
        else // poor
            pSubRegion->regionType.waterRegion.quality = QUALITY_POOR;

        if (stricmp(tokens[index + 2], "sandy") == 0)
            pSubRegion->regionType.waterRegion.sedimentCaract.type = SC_TYPE_SANDY;
        else if (stricmp(tokens[index + 2], "sand_muddy") == 0)
            pSubRegion->regionType.waterRegion.sedimentCaract.type = SC_TYPE_SAND_MUDDY;
        else // muddy
            pSubRegion->regionType.waterRegion.sedimentCaract.type = SC_TYPE_MUDDY;

        if (stricmp(tokens[index + 3], "excellent") == 0)
            pSubRegion->regionType.waterRegion.sedimentCaract.quality = QUALITY_EXCELLENT;
        else if (stricmp(tokens[index + 3], "good") == 0)
            pSubRegion->regionType.waterRegion.sedimentCaract.quality = QUALITY_GOOD;
        else // poor
            pSubRegion->regionType.waterRegion.sedimentCaract.quality = QUALITY_POOR;

        indexR = index + 4;
    }
    pSubRegion->pRegionArea = new Queue();
    getRegionArea(tokens, n, indexR, pSubRegion->pRegionArea);
}
//---------------------------------------------------------------------------
/*
 * NAME getRegionArea
 *
 * fill Region_Area Queue with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with geometry information (SIMPLE_REGION)
 * 'pRegionArea' - Queue entry for Simple Regions areas
 */

void EcoDynProtocol::getRegionArea(TOKEN* tokens, int n, int index,
        Queue* pRegionArea)
{
    int indexLoop=index;
    BUF* pBuf;
    SIMPLE_REGION* pRegion;

    while (indexLoop < (n - 1)) {
        if (strlen(tokens[indexLoop]) == 0)
            break;

        if ((pBuf = Queue::allocBuf(sizeof(SIMPLE_REGION))) == NULL) {
            break;
        }
        pRegion = (SIMPLE_REGION*)pBuf->pData;

        if (stricmp(tokens[indexLoop], "point") == 0) {
            pRegion->type = SR_TYPE_POINT;
            getEcoPoint(tokens, n, indexLoop + 1, &pRegion->poligon.point);
            indexLoop += 3;     // number of tokens to define one POINT
        }
        else if (stricmp(tokens[indexLoop], "rect") == 0) {
            pRegion->type = SR_TYPE_RECT;
            getEcoRect(tokens, n, indexLoop + 1, &pRegion->poligon.rect);
            indexLoop += 7;     // number of tokens to define one RECT
        }
        else if (stricmp(tokens[indexLoop], "square") == 0) {
            pRegion->type = SR_TYPE_SQUARE;
            getEcoSquare(tokens, n, indexLoop + 1, &pRegion->poligon.square);
            indexLoop += 13;     // number of tokens to define one SQUARE
        }
        else if (stricmp(tokens[indexLoop], "circle") == 0) {
            pRegion->type = SR_TYPE_CIRCLE;
            getEcoCircle(tokens, n, indexLoop + 1, &pRegion->poligon.circle);
            indexLoop += 5;     // number of tokens to define one CIRCLE
        }
        else if (stricmp(tokens[indexLoop], "arc") == 0) {
            pRegion->type = SR_TYPE_ARC;
            getEcoArc(tokens, n, indexLoop + 1, &pRegion->poligon.arc);
            indexLoop += 8;     // number of tokens to define one ARC
        }
        pRegionArea->insertElement(pBuf);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getSeedActionData
 *
 * fill SEED_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with seed action information (REGION)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getSeedActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    msgContent->type = SEED_ACTION_CODE;
    strcpy(msgContent->content.action.seed.regName, tokens[index]);
    getEcoTime(tokens, n, index + 1, &(msgContent->content.action.seed.time));
    getBivalve_S(tokens, n, index + 2, &(msgContent->content.action.seed.bivalve));
    msgContent->content.action.seed.density = atof(tokens[index + 6]);
}
//---------------------------------------------------------------------------
/*
 * NAME getInspectActionData
 *
 * fill INSPECT_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with inspect action information (REGION)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getInspectActionData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = INSPECT_ACTION_CODE;
    strcpy(msgContent->content.action.inspect.regName, tokens[index]);
    getEcoTime(tokens, n, index + 1, &(msgContent->content.action.inspect.time));
}
//---------------------------------------------------------------------------
/*
 * NAME getHarvestActionData
 *
 * fill HARVEST_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with harvest action information (REGION)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getHarvestActionData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = HARVEST_ACTION_CODE;
    strcpy(msgContent->content.action.harvest.regName, tokens[index]);
    getEcoTime(tokens, n, index + 1, &(msgContent->content.action.harvest.time));
    getBivalve(tokens, n, index + 2, &(msgContent->content.action.harvest.bivalve));
}
//---------------------------------------------------------------------------
/*
 * NAME getActionResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'code' - perception code
 * 'index' - index of the token with perception information (ID)
 * 'index + 1' - index of the token with perception result (ok, failed)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getActionResultData(int code, TOKEN* tokens, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = code;
    msgContent->content.perception.result.id = atoi(tokens[index]);
    if (stricmp(tokens[index + 1], "ok") == 0)
        msgContent->content.perception.result.actionResult = RESULT_OK;
    else
        msgContent->content.perception.result.actionResult = RESULT_FAILED;
}
//---------------------------------------------------------------------------
/*
 * NAME getSeedResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with seed perception information (ID)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getSeedResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(SEED_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getInspectResultData
 *
 * fill INSPECT_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with inspect perception information (ID)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getInspectResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = INSPECT_RESULT_CODE;
    msgContent->content.perception.inspectResult.id = atoi(tokens[index]);
    msgContent->content.perception.inspectResult.pQueue = new Queue();
    getBivalves(tokens, n, index + 1,
            msgContent->content.perception.inspectResult.pQueue);
}
//---------------------------------------------------------------------------
/*
 * NAME getHarvestResultData
 *
 * fill HARVEST_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with harvest perception information (ID)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getHarvestResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    msgContent->type = HARVEST_RESULT_CODE;
    msgContent->content.perception.harvestResult.id = atoi(tokens[index]);
    if (stricmp(tokens[index + 1], "ok") == 0) {
        msgContent->content.perception.harvestResult.actionResult = RESULT_OK;
        msgContent->content.perception.harvestResult.weight = atof(tokens[index + 2]);
    }
    else
        msgContent->content.perception.harvestResult.actionResult = RESULT_FAILED;
}
//---------------------------------------------------------------------------
/*
 * NAME getBivalves
 *
 * fill BIVALVE structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with bivalve information (BTYPE)
 * 'pBivalveQueue' - Queue entry for Bivalve areas
 */

void EcoDynProtocol::getBivalves(TOKEN* tokens, int n, int index,
        Queue* pBivalveQueue)
{
    int indexLoop=index;
    BUF* pBuf;
    BIVALVE* pBivalve;

    while (indexLoop < (n - 1)) {
        if (strlen(tokens[indexLoop]) > 0) {
            if ((pBuf = Queue::allocBuf(sizeof(BIVALVE))) == NULL) {
                break;
            }
            pBivalve = (BIVALVE*)pBuf->pData;
            getBivalve(tokens, n, indexLoop, pBivalve);
            pBivalveQueue->insertElement(pBuf);
        }
        indexLoop += 3;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getBivalve
 *
 * fill BIVALVE structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with bivalve information (BTYPE)
 * 'pBivalve' - pointer to BIVALVE structure to fill
 */

void EcoDynProtocol::getBivalve(TOKEN* tokens, int n, int index,
        BIVALVE* pBivalve)
{
    if (stricmp(tokens[index], "oyster") == 0)
        pBivalve->type = BV_TYPE_OYSTER;
    else if (stricmp(tokens[index], "kelp") == 0)
        pBivalve->type = BV_TYPE_KELP;
    else if (stricmp(tokens[index], "mussel") == 0)
        pBivalve->type = BV_TYPE_MUSSEL;
    else if (stricmp(tokens[index], "clam") == 0)
        pBivalve->type = BV_TYPE_CLAM;
    else // scallop
        pBivalve->type = BV_TYPE_SCALLOP;

//    getBCaract(tokens, n, index + 1, &pBivalve->bCaract);
    // tokens[index + 1] = "length"
    pBivalve->length = atof(tokens[index + 2]);
}
//---------------------------------------------------------------------------
/*
 * NAME getBivalve_S
 *
 * fill BIVALVE_S structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with bivalve information (BTYPE)
 * 'pBivalve_s' - pointer to BIVALVE_S structure to fill
 */

void EcoDynProtocol::getBivalve_S(TOKEN* tokens, int n, int index,
        BIVALVE_S* pBivalve_s)
{
    if (stricmp(tokens[index], "oyster") == 0)
        pBivalve_s->type = BV_TYPE_OYSTER;
    else if (stricmp(tokens[index], "kelp") == 0)
        pBivalve_s->type = BV_TYPE_KELP;
    else if (stricmp(tokens[index], "mussel") == 0)
        pBivalve_s->type = BV_TYPE_MUSSEL;
    else if (stricmp(tokens[index], "clam") == 0)
        pBivalve_s->type = BV_TYPE_CLAM;
    else // scallop
        pBivalve_s->type = BV_TYPE_SCALLOP;

    pBivalve_s->weight1 = atof(tokens[index + 1]);
    pBivalve_s->weight2 = atof(tokens[index + 2]);
}
//---------------------------------------------------------------------------
/*
 * NAME getEcoArc
 *
 * fill ECOARC structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with center information
 * 'pArc' - pointer to ECOARC structure to fill
 */

void EcoDynProtocol::getEcoArc(TOKEN* tokens, int n, int index,
        ECOARC* pArc)
{
    if (stricmp(tokens[index], "point") == 0) {
        getEcoPoint(tokens, n, index + 1, &pArc->center);
        pArc->p1 = atof(tokens[index + 3]);
        pArc->p2 = atof(tokens[index + 4]);
        pArc->p3 = atof(tokens[index + 5]);
        pArc->p4 = atof(tokens[index + 6]);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getEcoCircle
 *
 * fill ECOCIRCLE structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with center information
 * 'pCircle' - pointer to ECOCIRCLE structure to fill
 */

void EcoDynProtocol::getEcoCircle(TOKEN* tokens, int n, int index,
        ECOCIRCLE* pCircle)
{
    if (stricmp(tokens[index], "point") == 0) {
        getEcoPoint(tokens, n, index + 1, &pCircle->center);
        pCircle->radius = atof(tokens[index + 3]);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getEcoSquare
 *
 * fill ECOSQUARE structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with p1 information
 * 'pSquare' - pointer to ECOSQUARE structure to fill
 */

void EcoDynProtocol::getEcoSquare(TOKEN* tokens, int n, int index,
        ECOSQUARE* pSquare)
{
    if (stricmp(tokens[index], "point") == 0) {
        getEcoPoint(tokens, n, index + 1, &pSquare->p1);
        if (stricmp(tokens[index + 3], "point") == 0) {
            getEcoPoint(tokens, n, index + 4, &pSquare->p2);
            if (stricmp(tokens[index + 6], "point") == 0) {
                getEcoPoint(tokens, n, index + 7, &pSquare->p3);
                if (stricmp(tokens[index + 9], "point") == 0)
                    getEcoPoint(tokens, n, index + 10, &pSquare->p4);
            }
        }
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getEcoRect
 *
 * fill ECORECT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with p1 information (ECOPOINT)
 * 'pRect' - pointer to ECORECT structure to fill
 */

void EcoDynProtocol::getEcoRect(TOKEN* tokens, int n, int index,
        ECORECT* pRect)
{
    if (stricmp(tokens[index], "point") == 0) {
        getEcoPoint(tokens, n, index + 1, &pRect->p1);
        if (stricmp(tokens[index + 3], "point") == 0)
            getEcoPoint(tokens, n, index + 4, &pRect->p2);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getEcoPoint
 *
 * fill ECOPOINT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with p1 information
 * 'pPoint' - pointer to ECOPOINT structure to fill
 */

void EcoDynProtocol::getEcoPoint(TOKEN* tokens, int n, int index,
        ECOPOINT* pPoint)
{
    pPoint->x = atoi(tokens[index]);
    pPoint->y = atoi(tokens[index + 1]);
}
//---------------------------------------------------------------------------
/*
 * NAME getEcoTime
 *
 * fill ECOTIME structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with type information
 * 'pTime' - pointer to ECOTIME structure to fill
 */
 
void EcoDynProtocol::getEcoTime(TOKEN* tokens, int n, int index,
        ECOTIME* pTime)
{
    if (stricmp(tokens[index], "now") == 0)
        pTime->type = TM_TYPE_NOW;
    else {
        try {
            pTime->type = TM_TYPE_ECOTIME;
            pTime->time = atoi(tokens[index]);
        }
#ifdef __BORLANDC__
        catch (EConvertError &exception) {    // devia retornar FAILED
            Application->ShowException(&exception);
            pTime->type = TM_TYPE_NOW;
        }
#else  // __BORLANDC__
        catch (exception &e) {    // devia retornar FAILED
            cerr << "EcoDynProtocol::getEcoTime " << e.what() << endl;
            pTime->type = TM_TYPE_NOW;
        }
#endif  // __BORLANDC__
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getDeleteActionData
 *
 * fill SPECS_ACTION structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with type information
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getDeleteActionData(TOKEN* tokens, int n, int index,
        MSG_CONTENT* msgContent)
{
    int ni;
    BUF* pBuf;
    char* pName;
    Queue* pQueue;

    msgContent->type = DELETE_ACTION_CODE;
    msgContent->content.action.specs.pNames = new Queue();
    pQueue = msgContent->content.action.specs.pNames;
    ni = index;
    while (ni < n) {
        if (strlen(tokens[ni]) > 0) {
            if ((pBuf = Queue::allocBuf(ECDP_STRING)) == NULL) {
                break;
            }
            pName = (char*)pBuf->pData;
            strcpy(pName, tokens[ni]);
            pQueue->insertElement(pBuf);
        }
        ni++;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME getDefineResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with seed perception information (ID)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getDefineResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(DEFINE_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------
/*
 * NAME getDeleteResultData
 *
 * fill ACTION_RESULT structure with information given in 'tokens'
 * 'n' - number of total tokens
 * 'index' - index of first token with seed perception information (ID)
 * 'msgContent' - pointer to MSG_CONTENT structure to fill
 */

void EcoDynProtocol::getDeleteResultData(TOKEN* tokens, int n, int index,
    MSG_CONTENT* msgContent)
{
    getActionResultData(DELETE_RESULT_CODE, tokens, index, msgContent);
}
//---------------------------------------------------------------------------


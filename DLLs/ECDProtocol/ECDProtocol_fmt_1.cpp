//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_fmt_1.cpp
 *
 *   PRIVATE methods kicked by 'formatMessage' (ECDProtocol_Custom_fmt.cpp)
 *
 *   BULK 1:
 *
 *     void fmtDefinitionMsg(DEFINITION_MSG* definition, char* contentStr)
 *     void fmtSeedAction(SEED_ACTION* seed, char* contentStr)
 *     void fmtInspectAction(INSPECT_ACTION* inspect, char* contentStr)
 *     void fmtHarvestAction(HARVEST_ACTION* harvest, char* contentStr)
 *     void fmtActionResult(ACTION_RESULT* aResult, const char* resultText,
 *                          char* contentStr)
 *     void fmtSeedResult(ACTION_RESULT* seedResult, char* contentStr)
 *     void fmtInspectResult(INSPECT_RESULT* inspectResult, char* contentStr)
 *     void fmtHarvestResult(HARVEST_RESULT* harvestResult, char* contentStr)
 *     void fmtBivalve(BIVALVE* bivalve, char* contentStr)
 *     void fmtBivalve_S(BIVALVE_S* bivalve_s, char* contentStr)
 *     void fmtRegion(REGION* region, char* contentStr)
 *     void fmtSubRegion(SUB_REGION* subRegion, char* contentStr)
 *     void fmtWaterRegion(WATER_REGION* waterRegion, char* contentStr)
 *     void fmtSedimentCaract(SEDIMENT_CARACT* sCaract, char* contentStr)
 *     void fmtSimpleRegion(SIMPLE_REGION* simpleRegion, char* contentStr)
 *     void fmtEcoArc(ECOARC* arc, char* contentStr)
 *     void fmtEcoCircle(ECOCIRCLE* circle, char* contentStr)
 *     void fmtEcoSquare(ECOSQUARE* square, char* contentStr)
 *     void fmtEcoRect(ECORECT* rect, char* contentStr)
 *     void fmtEcoPoint(ECOPOINT* point, char* contentStr)
 *     void fmtEcoTime(ECOTIME* ecotime, char* contentStr)
 *
 *     == added in 2007.01.30 by AP
 *     void fmtDeleteAction(SPECS_ACTION* deleteAction, char* contentStr);
 *     void fmtDefineResult(ACTION_RESULT* defineResult, char* contentStr);
  *    void fmtDeleteResult(ACTION_RESULT* deleteResult, char* contentStr);
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
 * NAME fmtDefinitionMsg
 *
 * fill message with DEFINITION_MSG structure data
 */

void EcoDynProtocol::fmtDefinitionMsg(DEFINITION_MSG* definition,
        char* contentStr)
{
    sprintf(contentStr, "define(%s ", definition->regName);
    fmtRegion(&definition->region, &contentStr[strlen(contentStr)]);
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSeedAction
 *
 * fill message with SEED_ACTION structure data
 */

void EcoDynProtocol::fmtSeedAction(SEED_ACTION* seed, char* contentStr)
{
  char weight[32];

    sprintf(contentStr, "seed(%s ", seed->regName);
    fmtEcoTime(&seed->time, &contentStr[strlen(contentStr)]);
    strcat(contentStr, " ");
    fmtBivalve_S(&seed->bivalve, &contentStr[strlen(contentStr)]);
    sprintf(weight, " (density %f))", seed->density);
    strcat(contentStr, weight);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtInspectAction
 *
 * fill message with INSPECT_ACTION structure data
 */

void EcoDynProtocol::fmtInspectAction(INSPECT_ACTION* inspect, char* contentStr)
{
    sprintf(contentStr, "inspect(%s ", inspect->regName);
    fmtEcoTime(&inspect->time, &contentStr[strlen(contentStr)]);
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtHarvestAction
 *
 * fill message with HARVEST_ACTION structure data
 */

void EcoDynProtocol::fmtHarvestAction(HARVEST_ACTION* harvest, char* contentStr)
{
    sprintf(contentStr, "harvest(%s ", harvest->regName);
    fmtEcoTime(&harvest->time, &contentStr[strlen(contentStr)]);
    strcat(contentStr, " ");
    fmtBivalve(&harvest->bivalve, &contentStr[strlen(contentStr)]);
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtActionResult
 *
 * fill general result message
 */

void EcoDynProtocol::fmtActionResult(ACTION_RESULT* aResult, 
				const char* resultText, char* contentStr)
{
  char result[8];

    if (aResult->actionResult == RESULT_OK)
        strcpy(result, "ok");
    else
        strcpy(result, "failed");
    sprintf(contentStr, "%s(%i %s)", resultText, aResult->id, result);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSeedResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtSeedResult(ACTION_RESULT* seedResult, char* contentStr)
{
    fmtActionResult(seedResult, "seed_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtInspectResult
 *
 * fill message with INSPECT_RESULT structure data
 */

void EcoDynProtocol::fmtInspectResult(INSPECT_RESULT* inspectResult,
        char* contentStr)
{
  BIVALVE* pBivalve;
  char inspectStr[64];

    sprintf(contentStr, "inspect_result(%i", inspectResult->id);
    Queue* pQueue = inspectResult->pQueue;
    BUF* pBuf = (pQueue != NULL ? pQueue->getFirst(): NULL);
    while (pBuf != NULL) {
        pBivalve = (BIVALVE*)pBuf->pData;
        fmtBivalve(pBivalve, inspectStr);
        sprintf(&contentStr[strlen(contentStr)], " %s", inspectStr);
        pBuf = pBuf->pNext;
    }
    strcat(contentStr, ")");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtHarvestResult
 *
 * fill message with HARVEST_RESULT structure data
 */

void EcoDynProtocol::fmtHarvestResult(HARVEST_RESULT* harvestResult,
        char* contentStr)
{
  char result[8];

    if (harvestResult->actionResult == RESULT_OK)
        strcpy(result, "ok");
    else
        strcpy(result, "failed");
    sprintf(contentStr, "harvest_result(%i %s %f)",
            harvestResult->id, result, harvestResult->weight);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtBivalve
 *
 * fill message with BIVALVE structure data
 */

void EcoDynProtocol::fmtBivalve(BIVALVE* bivalve, char* contentStr)
{
    switch (bivalve->type) {
      case BV_TYPE_CLAM:
        strcpy(contentStr, "clam ");
        break;
      case BV_TYPE_MUSSEL:
        strcpy(contentStr, "mussel ");
        break;
      case BV_TYPE_OYSTER:
        strcpy(contentStr, "oyster ");
        break;
      case BV_TYPE_KELP:
        strcpy(contentStr, "kelp ");
        break;
      case BV_TYPE_SCALLOP:
      default:
        strcpy(contentStr, "scallop ");
        break;
    }
//    fmtBCaract(&bivalve->bCaract, &contentStr[strlen(contentStr)]);
    sprintf(&contentStr[strlen(contentStr)], "(length %f)", bivalve->length);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtBivalve_S
 *
 * fill message with BIVALVE_S structure data
 */

void EcoDynProtocol::fmtBivalve_S(BIVALVE_S* bivalve_s, char* contentStr)
{
    switch (bivalve_s->type) {
      case BV_TYPE_CLAM:
        strcpy(contentStr, "clam ");
        break;
      case BV_TYPE_MUSSEL:
        strcpy(contentStr, "mussel ");
        break;
      case BV_TYPE_OYSTER:
        strcpy(contentStr, "oyster ");
        break;
      case BV_TYPE_KELP:
        strcpy(contentStr, "kelp ");
        break;
      case BV_TYPE_SCALLOP:
      default:
        strcpy(contentStr, "scallop ");
        break;
    }
    sprintf(&contentStr[strlen(contentStr)], "(%f %f)",
            bivalve_s->weight1, bivalve_s->weight2);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtRegion
 *
 * fill message with REGION structure data
 */

void EcoDynProtocol::fmtRegion(REGION* region, char* contentStr)
{
//    char* pStr;

    if (region->type == RG_TYPE_REGION) {
        fmtSubRegion(&region->region.reg, contentStr);
        return;
    }
    else {   // RG_TYPE_REG_NAME
        Queue* pQueue = region->region.pRegNames;
        BUF* pBuf = (pQueue != NULL ? pQueue->getFirst(): NULL);
        contentStr[0] = '\0';
        while (pBuf != NULL) {
//            pStr = (char*)pBuf->pData;
            sprintf(&contentStr[strlen(contentStr)], "%s ", (char*)pBuf->pData);
            pBuf = pBuf->pNext;
        }
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSubRegion
 *
 * fill message with SUB_REGION structure data
 */

void EcoDynProtocol::fmtSubRegion(SUB_REGION* subRegion, char* contentStr)
{
  SIMPLE_REGION* pSimpleRegion;
  REGION_TYPE rType = subRegion->regionType;

    if (rType.type == RT_TYPE_WATER)
        fmtWaterRegion(&rType.waterRegion, contentStr);
    else    // RT_TYPE_LAND)
        strcpy(contentStr,  "land");
    strcat(contentStr, " ");

    Queue* pQueue = subRegion->pRegionArea;
    BUF* pBuf = (pQueue != NULL ? pQueue->getFirst(): NULL);
    while (pBuf != NULL) {
        pSimpleRegion = (SIMPLE_REGION*)pBuf->pData;
        fmtSimpleRegion(pSimpleRegion, &contentStr[strlen(contentStr)]);
        pBuf = pBuf->pNext;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtWaterRegion
 *
 * fill message with WATER_REGION structure data
 */

void EcoDynProtocol::fmtWaterRegion(WATER_REGION* waterRegion, char* contentStr)
{
  char quality[16], type[16];

    if (waterRegion->type == WR_TYPE_SUBTIDAL)
        strcpy(type, "subtidal");
    else
        strcpy(type, "intertidal");

    switch (waterRegion->quality) {
      case QUALITY_EXCELLENT:
        strcpy(quality, "excellent");
        break;
      case QUALITY_GOOD:
        strcpy(quality, "good");
        break;
      case QUALITY_POOR:
      default:
        strcpy(quality, "poor");
        break;
    }
    sprintf(contentStr, "%s %s ", type, quality);
    fmtSedimentCaract(&waterRegion->sedimentCaract,
            &contentStr[strlen(contentStr)]);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSedimentCaract
 *
 * fill message with SEDIMENT_CARACT structure data
 */

void EcoDynProtocol::fmtSedimentCaract(SEDIMENT_CARACT* sCaract, char* contentStr)
{
  char type[16], quality[16];

    switch (sCaract->type) {
      case SC_TYPE_SANDY:
        strcpy(type, "sandy");
        break;
      case SC_TYPE_SAND_MUDDY:
        strcpy(type, "sand_muddy");
        break;
      case SC_TYPE_MUDDY:
      default:
        strcpy(type, "muddy");
        break;
    }
    switch (sCaract->quality) {
      case QUALITY_EXCELLENT:
        strcpy(quality, "excellent");
        break;
      case QUALITY_GOOD:
        strcpy(quality, "good");
        break;
      case QUALITY_POOR:
      default:
        strcpy(quality, "poor");
        break;
    }
    sprintf(contentStr, "(%s %s)", type, quality);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtSimpleRegion
 *
 * fill message with SIMPLE_REGION structure data
 */

void EcoDynProtocol::fmtSimpleRegion(SIMPLE_REGION* simpleRegion, char* contentStr)
{
    switch (simpleRegion->type) {
      case SR_TYPE_RECT:
        fmtEcoRect(&simpleRegion->poligon.rect, contentStr);
        break;
      case SR_TYPE_SQUARE:
        fmtEcoSquare(&simpleRegion->poligon.square, contentStr);
        break;
      case SR_TYPE_CIRCLE:
        fmtEcoCircle(&simpleRegion->poligon.circle, contentStr);
        break;
      case SR_TYPE_ARC:
        fmtEcoArc(&simpleRegion->poligon.arc, contentStr);
        break;
      case SR_TYPE_POINT:
      default:
        fmtEcoPoint(&simpleRegion->poligon.point, contentStr);
        break;
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEcoArc
 *
 * fill message with ECOARC structure data
 */

void EcoDynProtocol::fmtEcoArc(ECOARC* arc, char* contentStr)
{
  char center[32];

    fmtEcoPoint(&arc->center, center);
    sprintf(contentStr,  "(arc %s %f %f %f %f)", center,
            arc->p1, arc->p2, arc->p3, arc->p4);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEcoCircle
 *
 * fill message with ECOCIRCLE structure data
 */

void EcoDynProtocol::fmtEcoCircle(ECOCIRCLE* circle, char* contentStr)
{
  char center[32];

    fmtEcoPoint(&circle->center, center);
    sprintf(contentStr, "(circle %s %f)", center, circle->radius);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEcoSquare
 *
 * fill message with ECOSQUARE structure data
 */

void EcoDynProtocol::fmtEcoSquare(ECOSQUARE* square, char* contentStr)
{
  char p1[32], p2[32], p3[32], p4[32];

    fmtEcoPoint(&square->p1, p1);
    fmtEcoPoint(&square->p2, p2);
    fmtEcoPoint(&square->p3, p3);
    fmtEcoPoint(&square->p4, p4);
    sprintf(contentStr, "(square %s%s%s%s)", p1, p2, p3, p4);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEcoRect
 *
 * fill message with ECORECT structure data
 */

void EcoDynProtocol::fmtEcoRect(ECORECT* rect, char* contentStr)
{
  char p1[32], p2[32];

    fmtEcoPoint(&rect->p1, p1);
    fmtEcoPoint(&rect->p2, p2);
    sprintf(contentStr, "(rect %s%s)", p1, p2);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEcoPoint
 *
 * fill message with ECOPOINT structure data
 */

void EcoDynProtocol::fmtEcoPoint(ECOPOINT* point, char* contentStr)
{
    sprintf(contentStr, "(point %d %d)", point->x, point->y);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtEcoTime
 *
 * fill message with ECOTIME structure data
 */
 
void EcoDynProtocol::fmtEcoTime(ECOTIME* ecotime, char* contentStr)
{
    if (ecotime->type == TM_TYPE_ECOTIME)
        sprintf(contentStr, "%li", ecotime->time);
    else    // TM_TYPE_NOW
        strcpy(contentStr, "now");
}
//---------------------------------------------------------------------------
/*
 * NAME fmtDeleteAction
 *
 * fill message with SPECS_ACTION structure data
 */

void EcoDynProtocol::fmtDeleteAction(SPECS_ACTION* deleteAction,
        char* contentStr)
{
    BUF* pBufName;
    char* pName;
    int i;

    strcpy(contentStr, "delete");
    for (i = 0; i < deleteAction->pNames->size(); i++) {
        pBufName = deleteAction->pNames->getElementAt(i);
        pName = (char*)pBufName->pData;
        sprintf(&contentStr[strlen(contentStr)], " %s", pName);
    }
}
//---------------------------------------------------------------------------
/*
 * NAME fmtDefineResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtDefineResult(ACTION_RESULT* defineResult, char* contentStr)
{
    fmtActionResult(defineResult, "define_result", contentStr);
}
//---------------------------------------------------------------------------
/*
 * NAME fmtDeleteResult
 *
 * fill message with ACTION_RESULT structure data
 */

void EcoDynProtocol::fmtDeleteResult(ACTION_RESULT* deleteResult, char* contentStr)
{
    fmtActionResult(deleteResult, "delete_result", contentStr);
}
//---------------------------------------------------------------------------


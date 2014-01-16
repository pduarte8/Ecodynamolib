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
	#include <cstdlib>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "Region.h"

//---------------------------------------------------------------------------

//   Region

//---------------------------------------------------------------------------
#pragma package(smart_init)
/*
 * default constructor
 */
Region::Region() {
    pArea = new Queue();
    name[0] = '\0';
}
//---------------------------------------------------------------------------
/*
 * 2004-06-02, AP
 *
 * At the moment only POINT and RECT regions are processed...
 */
Region::Region(TObject* pList, char* regName, REGION* pRegion)
{
  BUF* pBuf;
  REGION_BOX* rb;

    strcpy(name, regName);
    pArea = new Queue();
    if (pRegion->type == RG_TYPE_REGION) {  // defined one poligon
        SUB_REGION subRegion = pRegion->region.reg;
        REGION_TYPE* pType = &subRegion.regionType;
        type.type = pType->type;
        type.waterRegion.type = pType->waterRegion.type;
        type.waterRegion.quality = pType->waterRegion.quality;
        type.waterRegion.sedimentCaract.type =
                pType->waterRegion.sedimentCaract.type;
        type.waterRegion.sedimentCaract.quality =
                pType->waterRegion.sedimentCaract.quality;
        for (int i = 0; i < subRegion.pRegionArea->size(); i++) {
            SIMPLE_REGION* pSimple =
                    (SIMPLE_REGION*)subRegion.pRegionArea->getElementAt(i)->pData;
            if (pSimple->type == SR_TYPE_POINT) {
                if ((pBuf = Queue::allocBuf(sizeof(REGION_BOX))) == NULL) {
                    break;
                }
                rb = (REGION_BOX*)pBuf->pData;
                rb->line = pSimple->poligon.point.y;
                rb->column = pSimple->poligon.point.x;
                pArea->insertElement(pBuf);
            }
            else if (pSimple->type == SR_TYPE_RECT) {
                int xm = MIN(pSimple->poligon.rect.p1.x,pSimple->poligon.rect.p2.x);
                int xM = MAX(pSimple->poligon.rect.p1.x,pSimple->poligon.rect.p2.x);
                int ym = MIN(pSimple->poligon.rect.p1.y,pSimple->poligon.rect.p2.y);
                int yM = MAX(pSimple->poligon.rect.p1.y,pSimple->poligon.rect.p2.y);
//                for (int y = ym; y <= yM; y++) {
                for (int y = yM; y >= ym; y--) {    // increasing boxes number
                    for (int x = xm; x <= xM; x++) {
                        if ((pBuf = Queue::allocBuf(sizeof(REGION_BOX))) == NULL) {
                            break;
                        }
                        rb = (REGION_BOX*)pBuf->pData;
                        rb->line = y;
                        rb->column = x;
                        pArea->insertElement(pBuf);
                    }
                }
            }
            else {
                // other types (SQUARE, CIRCLE, ARC) not processed yet
            }
        }
    }
    else {  // RG_TYPE_REG_NAME: region defined as a union of other defined regions
        for (int i = 0; i < pRegion->region.pRegNames->size(); i++) {
            char* pChar = (char*)pRegion->region.pRegNames->getElementAt(i)->pData;
            Region* pReg = ((RegionList*)pList)->getRegion(pChar);
            if (pReg != NULL) {
                REGION_TYPE* pType = pReg->getRegionType();
                type.type = pType->type;
                type.waterRegion.type = pType->waterRegion.type;
                type.waterRegion.quality = pType->waterRegion.quality;
                type.waterRegion.sedimentCaract.type =
                        pType->waterRegion.sedimentCaract.type;
                type.waterRegion.sedimentCaract.quality =
                        pType->waterRegion.sedimentCaract.quality;
                Queue* pQueue = pReg->getRegionArea();
                for (int i = 0; i < pQueue->size(); i++) {
                    REGION_BOX* pRB = (RegionBox*)pQueue->getElementAt(i)->pData;
                    if ((pBuf = Queue::allocBuf(sizeof(REGION_BOX))) == NULL) {
                        break;
                    }
                    rb = (REGION_BOX*)pBuf->pData;
                    rb->line = pRB->line;
                    rb->column = pRB->column;
                    pArea->insertElement(pBuf);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
Region::~Region()
{
    if (pArea != NULL) {
//        pArea->clear();
        delete pArea;
    }
}
//---------------------------------------------------------------------------
Queue* Region::getRegionArea()
{
    return pArea;
}
//---------------------------------------------------------------------------
char* Region::getRegionName()
{
    return name;
}
//---------------------------------------------------------------------------
REGION_TYPE* Region::getRegionType()
{
    return &type;
}
//---------------------------------------------------------------------------

//   RegionList

//---------------------------------------------------------------------------
/*
 * default constructor
 */
RegionList::RegionList() {
    pRegions = new Queue();
}
//---------------------------------------------------------------------------
RegionList::~RegionList()
{
    if (pRegions != NULL) {
        //
        // regions must be deleted by alloc processes
        //
/*        for (int i = pRegions->size() - 1; i >=0 ; i--) {
            Region* pR = (Region*)pRegions->RemoveObject(i);
            delete pR;
        }*/
//        pRegions->clear();
        delete pRegions;
    }
}
//---------------------------------------------------------------------------
/*
 * return "name" Region from the list or NULL if it doesn't exist
 */
Region* RegionList::getRegion(char* name)
{
    for (int i = 0; i < pRegions->size(); i++) {
        Region* pRegion = (Region*)pRegions->RetrieveObject(i);
        if (strcmp(pRegion->getRegionName(), name) == 0) {
            return pRegion;
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
/*
 * return Region index "index" in the list or NULL if "index > size()"
 */
Region* RegionList::getRegion(int index)
{
    return (Region*)pRegions->RetrieveObject(index);
}
//---------------------------------------------------------------------------
bool RegionList::addRegion(Region* pRegion)
{
    return pRegions->InsertObject((TObject*)pRegion);
}
//---------------------------------------------------------------------------
Region* RegionList::removeRegion(char* name)
{
    for (int i = 0; i < pRegions->size(); i++) {
        Region* pRegion = (Region*)pRegions->RetrieveObject(i);
        if (strcmp(pRegion->getRegionName(), name) == 0) {
            return (Region*)pRegions->RemoveObject(i);
        }
    }
    return NULL;
}
//---------------------------------------------------------------------------
Region* RegionList::removeRegion(int index)
{
    return (Region*)pRegions->RemoveObject(index);
}
//---------------------------------------------------------------------------
bool RegionList::isEmpty()
{
    return (pRegions->size() == 0 ? true : false);
}
//---------------------------------------------------------------------------
int RegionList::size()
{
    return pRegions->size();
}
//---------------------------------------------------------------------------


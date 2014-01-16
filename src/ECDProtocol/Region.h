//---------------------------------------------------------------------------
#ifndef _REGION_H_
#define _REGION_H_

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#include "ECDPMessages.h"

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

//---------------------------------------------------------------------------
/*
 * This file defines the RegionBox, RegionList and Region classes:
 *
 * RegionBox saves each box definition (box number, line and column)
 *
 * Region saves the description of each region has it is defined in
 *   the EcoDynamo Protocol
 *
 * RegionList is the list of defined regions.
 * Each object must have each own RegionList class.
 */
//---------------------------------------------------------------------------


// structure with the box definition
//
typedef struct RegionBox {
    int line;
    int column;
} REGION_BOX;

// class that saves one defined region area
//
class _export Region
{
  public:		// User declarations
    Region();
    Region(TObject* pList, char* regName, REGION* pRegion);
    ~Region();

    Queue* getRegionArea();
    char* getRegionName();
    REGION_TYPE* getRegionType();

  private:
    char name[64];
    REGION_TYPE type;
    Queue* pArea;         // REGION_BOX elements
};

// class that saves a list with all defined regions
//
class _export RegionList
{
  public:		// User declarations
    RegionList();
    ~RegionList();

    Region* getRegion(char* name);
    Region* getRegion(int index);
    bool addRegion(Region* pRegion);
    Region* removeRegion(char* name);
    Region* removeRegion(int index);
    bool isEmpty();
    int size();

  private:
    Queue* pRegions;        // Region elements
};

#endif //_REGION_H_

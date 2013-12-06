// Man object CPP code file
// @ Antonio Pereira
// Rev. A 2005.11.18
/**
 * NPI work
 * 	20 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
    #include <vcl.h>
    #include <stdio.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>

	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "manobjt.h"
#include "iodll.h"

// Farmer man class - Clams private methods HARVEST
// AP, 2005.11.18...

/*
 * CLAMS methods
 */

void TFisherMan::ClamHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion)
{
  // AP, 2006.02.14
  Queue* pBoxes;
  int size, i, n;
  BUF* pBuf;
  REGION_BOX* pBox;

   double ExistingBiomass, BiomassOutput;
   TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();
   TEcoDynClass* MyZoobenthosPointer2 = MyPEcoDynClass->GetZoobenthosPointer2();

   TEcoDynClass* ZoobenthosPointer = NULL;

    if ((MyZoobenthosPointer != NULL) &&
           (strstr(MyZoobenthosPointer->GetEcoDynClassName(), "Decussatus")))
    {
        ZoobenthosPointer = MyZoobenthosPointer;
    }

    if ((MyZoobenthosPointer2 != NULL) &&
            (strstr(MyZoobenthosPointer2->GetEcoDynClassName(), "Decussatus")))
    {
        ZoobenthosPointer = MyZoobenthosPointer2;
    }

    if (ZoobenthosPointer == NULL)        // force null answer
    {
        pHarvestRegion->weightHarvested = 0.0;
        return;
    }

    pBoxes = pDB->regionList->getRegion(pHarvestRegion->regName)->getRegionArea();
    size = (pBoxes == NULL) ? NumberOfBoxes : pBoxes->size();
    n = 0;
    while (n < size) {
        if (pBoxes != NULL) {
            pBuf = pBoxes->getElementAt(n);
            pBox = (REGION_BOX*)pBuf->pData;
            i = GetBoxNumber(pBox->column, pBox->line);
        }
        else {
            i = n;
        }

        ZoobenthosPointer->Inquiry(GetEcoDynClassName(), ExistingBiomass,
                                   i,
                                   "R. decussatus biomass 1",
                                   ObjectCode);

        // collect 80% of existing clams only once - collect all, AP 2011.02.07
        BiomassOutput = ExistingBiomass; // * 0.80;

        // 20% of existing clams remain in site
        ZoobenthosPointer->SetVariableValue(GetEcoDynClassName(),
                                        ExistingBiomass - BiomassOutput,
                                        i,
                                        "R. decussatus biomass 1");

        pHarvestRegion->weightHarvested += (BiomassOutput * MyPEcoDynClass->GetTimeStep());  // include Integration;
        n++;
    }
}


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

// Fisher man class - Oyster private method HARVEST
// AP, 2005.11.18...

/*
 * OYSTER methods
 */

void TFisherMan::OysterHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion)
{
  // AP, 2006.02.14
  Queue* pBoxes;
  int size, i, n;
  REGION_BOX* pBox;

   double ExistingDensity, ShellLength, BiomassOutput;
   double OysterShellWeight, OysterMeatWeight, NumberOfHarvestedOysters;

   TEcoDynClass* MyZoobenthosPointer = MyPEcoDynClass->GetZoobenthosPointer();
   TEcoDynClass* MyZoobenthosPointer2 = MyPEcoDynClass->GetZoobenthosPointer2();

   TEcoDynClass* ZoobenthosPointer = NULL;

    if ((MyZoobenthosPointer != NULL) &&
           (strstr(MyZoobenthosPointer->GetEcoDynClassName(), "CrassostreaGigas")))
    {
        ZoobenthosPointer = MyZoobenthosPointer;
    }
    else if ((MyZoobenthosPointer2 != NULL) &&
            (strstr(MyZoobenthosPointer2->GetEcoDynClassName(), "CrassostreaGigas")))
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
            pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
            i = GetBoxNumber(pBox->column, pBox->line);
        }
        else {
            i = n;
        }

        ZoobenthosPointer->Inquiry(GetEcoDynClassName(), ExistingDensity,
                                      i,
                                      "C. gigas density 1",
                                      ObjectCode);

        ZoobenthosPointer->Inquiry(GetEcoDynClassName(), ShellLength,
                                      i,
                                      "C. gigas shell length 1",
                                      ObjectCode);
        if ((ExistingDensity > 0.000001) && (ShellLength >= pHarvestRegion->bivalve.length))
        {
            ZoobenthosPointer->Inquiry(GetEcoDynClassName(), OysterShellWeight,    //g DW
                                           i,
                                            "C. gigas shell mass 1",
                                             ObjectCode);
            ZoobenthosPointer->Inquiry(GetEcoDynClassName(), OysterMeatWeight,    //g DW
                                           i,
                                            "C. gigas meat weight 1",
                                             ObjectCode);

            NumberOfHarvestedOysters = ExistingDensity / MyPEcoDynClass->GetTimeStep(); // AP, 2011.01.31

            ZoobenthosPointer->Update(GetEcoDynClassName(), -NumberOfHarvestedOysters,    //ind. / m2 / timestep  As ostras são todas apanhadas de uma só vez
                                       i,
                                        "C. gigas density 1",
                                         ObjectCode);

           BiomassOutput = NumberOfHarvestedOysters * (OysterShellWeight + OysterMeatWeight);
           BiomassOutput = BiomassOutput //* MyPEcoDynClass->GetTimeStep()  //Esta multiplicação pelo "time step" está errada, pois "cria" ostras - Pedro 7/06/2010
                 * MyPEcoDynClass->GetTimeStep()  // Multiplicação reposta para simular "Integration" (TSangoMan) - AP 2011.02.07
                 * pow(MyPEcoDynClass->GetBoxLength(i),2)/ CUBIC / CUBIC;
           pHarvestRegion->weightHarvested += BiomassOutput;       // AP, 2005.11.18
        }
        n++;
    }
}


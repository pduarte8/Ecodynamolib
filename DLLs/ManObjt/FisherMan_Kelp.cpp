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

// Fisher man class - Kelp private methods HARVEST
// AP, 2005.11.18...

/*
 * KELP methods
 */

void TFisherMan::KelpHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion)
{
    // yet fake
    // to simulate immediate harvesting...
    pHarvestRegion->weightHarvested = 0.0;
}
/*
 * ........? preciso pensar......
 *
 *...
  // AP, 2005.11.18
  Queue* pBoxes;
  int size, i, n;
  BUF* pBuf;
  REGION_BOX* pBox;

	const int LastDayOfTheYear = 365;
	double ExistingBiomass, ExistingDensity, CurrentKelpWeight,
          DaylyBiomassInput, SeedInputNumber, MyFirstSeedingDay, MyLastSeedingDay, NumberOfSeedDays;
	TEcoDynClass* MyMacrophytePointer = MyPEcoDynClass->GetMacrophytePointer();

   if (MyMacrophytePointer != NULL)
   {
      // AP, 2005.11.18...
      if (pDB == NULL) {
          MyFirstSeedingDay = KelpFirstSeedingDay;
          MyLastSeedingDay = KelpLastSeedingDay;
          pBoxes = NULL;
      }
      else {
          MyFirstSeedingDay = pDB->firstDay;
          MyLastSeedingDay = pDB->lastDay;
          pBoxes = pDB->regionList->getRegion(pDB->regName)->getRegionArea();
      }
      if (MyFirstSeedingDay > MyLastSeedingDay)
         MyLastSeedingDay += LastDayOfTheYear;

      NumberOfSeedDays = MyLastSeedingDay - (MyFirstSeedingDay - 1); //Both first and last days included in seeding period

      size = (pBoxes == NULL) ? NumberOfBoxes : pBoxes->size();
      n = 0;
      while (n < size) {
        if (pBoxes != NULL) {
            pBuf = pBoxes->getElementAt(n);
            pBox = (REGION_BOX*)pBuf->pData;
            i = GetBoxNumber(pBox->column, pBox->line);
            CultivationAreaOfKelp[i] = 1;
        }
        else {
            i = n;
        }
        // ...AP, 2005.11.18

      	if (CultivationAreaOfKelp[i] == 1)
        {
            MyMacrophytePointer->Inquiry(GetEcoDynClassName(), ExistingBiomass,    //g DW / m2
                                             i,
                                             "L. japonica biomass density",
                                             ObjectCode);
            MyMacrophytePointer->Inquiry(GetEcoDynClassName(), ExistingDensity,    //indivuals / m2
                                             i,
                                             "L. japonica density",
                                             ObjectCode);
            MyMacrophytePointer->Inquiry(GetEcoDynClassName(), CurrentKelpWeight,    //g DW / individual
                                             i,
                                             "L. japonica individual weight",
                                             ObjectCode);

            DaylyBiomassInput = BiomassInputOfKelp[i] / NumberOfSeedDays;     //g DW / day / m2
            SeedInputNumber = DaylyBiomassInput / SeedWeightOfKelp[i] / DAYSTOSECONDS;        //individuals / s / m2

            MyMacrophytePointer->Update(GetEcoDynClassName(), DaylyBiomassInput / DAYSTOSECONDS,    //g DW / m2 / s
                                         i,
                                         "L. japonica biomass input/output",
                                         ObjectCode);


            CurrentKelpWeight = (
                                   CurrentKelpWeight * ExistingDensity +
                                   SeedWeightOfKelp[i] * SeedInputNumber * MyPEcoDynClass->GetTimeStep()
                                 )  /
                                 (ExistingDensity + SeedInputNumber * MyPEcoDynClass->GetTimeStep());
                                 // g / individual, weighted by the existing and new individuals, that enter at each time step

            MyMacrophytePointer->Update(GetEcoDynClassName(), CurrentKelpWeight,    //g DW / invididual
                                         i,
                                         "L. japonica individual weight",
                                         ObjectCode);

         }
         n++;           // AP, 2005.11.18
      }
   }
   */



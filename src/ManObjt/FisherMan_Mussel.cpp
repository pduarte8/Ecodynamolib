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

// Farmer man class - Mussel private methods HARVEST
// AP, 2005.11.18...

/*
 * Mussel methods
 */

void TFisherMan::MusselHarvest(HARVEST_DB* pDB, HARVEST_REGION* pHarvestRegion)
{
    // yet fake
    // to simulate immediate harvesting...
    pHarvestRegion->weightHarvested = 0.0;
}



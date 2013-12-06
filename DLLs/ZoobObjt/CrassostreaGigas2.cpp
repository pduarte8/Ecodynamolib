// Zoobenthos object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.06.13
/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "zoobobjt.h"
#include "iodll.h"

// TZoobenthos Class

TCrassostreaGigas2::TCrassostreaGigas2(TEcoDynClass* APEcoDynClass, char* className)
							 :	 TCrassostreaGigas(APEcoDynClass, className)
{
	ClassIncrement = ClassMaxLimits[0] - ClassMinLimits[0];
}

TCrassostreaGigas2::~TCrassostreaGigas2()
{
}

void TCrassostreaGigas2:: Go()
{
	int i,k;
	for (i = 0; i < NumberOfBoxes; i++)
	{
		ZooDINRelease[i] = 0;
      ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
	}
	JulianDay = MyPEcoDynClass->GetJulianDay();
	//C?lculo do fluxo l?quido ao n?vel individual e da popula??o, com base nos processos metab?licos}
	Feed();
	Pseudofaeces();
	//MessageBox(0, "M?todo pseudofaeces OKAY", "EcoDynamo alert", MB_OK);
	Ingestion();
	//MessageBox(0, "M?todo ingestion OKAY", "EcoDynamo alert", MB_OK);
	Excrete();
	//MessageBox(0, "M?todo excrete OKAY", "EcoDynamo alert", MB_OK);
	Assimilate();
	//MessageBox(0, "M?todo assimilate OKAY", "EcoDynamo alert", MB_OK);
	Respire();
	//MessageBox(0, "M?todo respire OKAY", "EcoDynamo alert", MB_OK);
	Die();
	//MessageBox(0, "M?todo die OKAY", "EcoDynamo alert", MB_OK);
  /* JulianDay = MyPEcoDynClass->GetJulianDay();
             if(JulianDay > 120)
             Debugger(ZooIndividualFlux[0] *
					DAYSTOHOURS);  */
	for (i = 0; i < NumberOfBoxes; i++)
	{
		for (k = 0; k < NumberOfClasses; k++)
		{
			if ((ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
				 (ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
				)
				/*GrowthRate[k * NumberOfBoxes + i] =
					ZooIndividualFlux[k * NumberOfBoxes + i] *
					DAYSTOHOURS; *///g day-1    //Pedro 30/3/99

            ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] *
                                                       DAYSTOHOURS; //g day-1
            GrowthRate[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i];
	  /*		if (ZooIndividualFlux[k * NumberOfBoxes + i] < 0.0)
			{
				MessageBox(0, "C. gigas - Negative growth rate ", "EcoDynamo alert", MB_OK);
				Debugger(k);
				Debugger(i);
				Debugger(k * NumberOfBoxes + i);
				Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
				Debugger(ZooClassBiomass[k * NumberOfBoxes + i]);
				Debugger(ZooClassDensity[k * NumberOfBoxes + i]);
				Debugger(ZooIndividualWeight[k * NumberOfBoxes + i]);
				Debugger(SestonFiltration[k * NumberOfBoxes + i]);
				Debugger(pseudofaeces_production[k * NumberOfBoxes + i]);
				Debugger(seston_ingestion[k * NumberOfBoxes + i]);
				Debugger(total_respiration);
				Debugger(total_excretion);
			}*/
		}
	}
}


void TCrassostreaGigas2:: Integrate()
{
	DemographicUpwind();
	//MessageBox(0, "M?todo integrate okay", "EcoDynamo alert", MB_OK);
}


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
TCrassostreaGigas4::TCrassostreaGigas4(TEcoDynClass* APEcoDynClass, char* className)
							 :	 TCrassostreaGigas2(APEcoDynClass, className)
{
	ClassIncrement = ClassMaxLimits[0] - ClassMinLimits[0];
}

TCrassostreaGigas4::~TCrassostreaGigas4()
{
}

void TCrassostreaGigas4::Pseudofaeces()   //C?lculo das pseudofezes conforme descrito por Raillard (1991) Pedro 22/1/99
                                          //Este c?lculo n?o foi implementado nos trabalhos feitos sobre Carlingford
                                          //por n?o haver dados de POM. Neste caso assume-se que o POM ? 2X o POC.

{

	double delta1, delta2,
          OrganicConsumption, InorganicConsumption,
          OrganicPseudoFaecesRate, InorganicPseudoFaecesRate,
          OrganicEgestion, InorganicEgestion,
          OrganicIngestion, InorganicIngestion;
   double const CarbonToOrganic = 2;

	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{

			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{
         	OrganicConsumption = ZooIndividualFlux[k * NumberOfBoxes + i] *
            	                  ( org_seston_mass[k * NumberOfBoxes + i] * CarbonToOrganic/
               	                 total_seston_mass[k * NumberOfBoxes + i]);
         	InorganicConsumption = ZooIndividualFlux[k * NumberOfBoxes + i] *
            	                    (1 - (org_seston_mass[k * NumberOfBoxes + i] * CarbonToOrganic)/
               	                  total_seston_mass[k * NumberOfBoxes + i]);

            
         	//Organic pseudofaeces
				if (c1o /
					CUBIC -                // em g h-1 tal como o consumo
					OrganicConsumption/
            	ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
			   	delta1 = c1o /
						      CUBIC -         // em g h-1 tal como o consumo
					   	   OrganicConsumption /
					      	ZooIndividualWeight[k * NumberOfBoxes + i];
				else
					delta1 = 0;
				if (c2o /
					CUBIC -                // em g h-1 tal como o consumo
					OrganicConsumption/
			   	ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
			   	delta2 = c2o /
						      CUBIC -         // em g h-1 tal como o consumo
					   	   OrganicConsumption /
                        ZooIndividualWeight[k * NumberOfBoxes + i];
				else
					delta2 = 0;
				pseudo_faeces_rate = pseudo_faeces_maxo *
									(1-exp(xkpo*delta1)) +
									(1-pseudo_faeces_maxo) *
									(1-exp(xkp2o*delta2));
         	OrganicPseudoFaecesRate = pseudo_faeces_rate;

         	//Inorganic pseudofaeces

	  			if (c1i /
					CUBIC -                    // em g h-1 tal como o consumo
					InorganicConsumption/
					ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
					delta1 = c1i /
							CUBIC -           // em g h-1 tal como o consumo
							InorganicConsumption /
							ZooIndividualWeight[k * NumberOfBoxes + i];
				else
					delta1 = 0;
				if (c2i /
					CUBIC -                   // em g h-1 tal como o consumo
					InorganicConsumption/
					ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
					delta2 = c2i /
							  CUBIC -         // em g h-1 tal como o consumo
							  InorganicConsumption /
						 	  ZooIndividualWeight[k * NumberOfBoxes + i];
				else
					delta2 = 0;
				pseudo_faeces_rate = pseudo_faeces_rate +
										pseudo_faeces_maxi *
										(1-exp(xkpi*delta1))+
										(1-pseudo_faeces_maxi)*
										(1-exp(xkp2i*delta2));
         	InorganicPseudoFaecesRate = pseudo_faeces_rate;
            /*if ((JulianDay > 200) && (k = 10))
            {
            	Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
               Debugger(OrganicConsumption);
               Debugger(InorganicConsumption);
            }*/
            OrganicEgestion = OrganicPseudoFaecesRate * OrganicConsumption;
            InorganicEgestion = InorganicPseudoFaecesRate * InorganicConsumption;
         	OrganicIngestion = OrganicConsumption - OrganicPseudoFaecesRate * OrganicConsumption;
         	InorganicIngestion = InorganicConsumption - InorganicPseudoFaecesRate * InorganicConsumption;
            total_seston_mass[k * NumberOfBoxes + i] = OrganicIngestion + InorganicIngestion;
         	org_seston_mass[k * NumberOfBoxes + i] = OrganicIngestion / CarbonToOrganic;
            pseudofaeces_production[k * NumberOfBoxes + i] = OrganicEgestion + InorganicEgestion;
            /*if ((JulianDay > 200) && (k = 10))
            {
            	Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
               Debugger(OrganicConsumption);
               Debugger(InorganicConsumption);
               Debugger(OrganicEgestion);
               Debugger(InorganicEgestion);
               Debugger(OrganicIngestion);
               Debugger(InorganicIngestion);
            } */
			} // end of if
		}
	}
}

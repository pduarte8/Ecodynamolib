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

TCrassostreaGigas3::TCrassostreaGigas3(TEcoDynClass* APEcoDynClass, char* className)
							 :	 TCrassostreaGigas2(APEcoDynClass, className)
{
	a = new double[NumberOfClasses];
	b = new double[NumberOfClasses];
	c = new double[NumberOfClasses];
	r = new double[NumberOfClasses];
	u = new double[NumberOfClasses];
	gam = new double[NumberOfClasses];

	for (int i = 0; i < NumberOfClasses; i++)
	{
		a[i] = 0.0;
		b[i] = 0.0;
		c[i] = 0.0;
		r[i] = 0.0;
		u[i] = 0.0;
		gam[i] = 0.0;
	}
	ClassIncrement = ClassMaxLimits[0] - ClassMinLimits[0];
}

TCrassostreaGigas3::~TCrassostreaGigas3()
{
    freeMemory();
}

void TCrassostreaGigas3::freeMemory()
{
    try {
        if (NumberOfClasses > 0) {
            delete [] a;
            delete [] b;
            delete [] c;
            delete [] r;
            delete [] u;
            delete [] gam;
        }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TCrassostreaGigas3::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TCrassostreaGigas3::Integrate()
{
	DemographicUpwindSemiImplicit();
}

void TCrassostreaGigas3::Die()
{
	double MeanTFWDW = 0.03;
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0.0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0.0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0.0)
				)
			{
				//if (k == 0)
				if (ZooIndividualWeight[k * NumberOfBoxes + i] < 5.0 *
					 MeanTFWDW)
				natural_mortality = 0.042 *
										  ZooClassDensity[k * NumberOfBoxes + i];
				//if (k > 0)
				else
				{
					if ((JulianDay > 179) && (JulianDay < 271))
						natural_mortality = 0.0025
												  / 30
												  * ZooClassDensity[k * NumberOfBoxes + i];
					else // Relat?rio dos irlandeses
						natural_mortality = 0.01
												  / 30
												  * ZooClassDensity[k * NumberOfBoxes + i];
				}
				Mortality[k * NumberOfBoxes + i] = natural_mortality /
					  ZooClassDensity[k * NumberOfBoxes + i];
			}
		}
	}
}


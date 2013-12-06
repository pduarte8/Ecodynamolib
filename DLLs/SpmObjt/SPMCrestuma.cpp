// SPM object CPP code file
// @ J. Gomes Ferreira
// Rev. A 96.02.27

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
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"
#include "iodll.h"

// TSPMCrestuma Class
//
// Constructors invoked outside EcoDyn shell...
//

TSPMCrestuma::TSPMCrestuma(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double riverSPM, double oceanSPM,
        double aSPMConcentration[], double aSPMLoad[], double totalExtinctionCoefficient,
        int nParams, double localLatitude, double localHeight, double localSalt,
        double localTemperature, double turbulenceFactor, double aSPMRessuspension[])
    :TSPMOceanic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, riverSPM, oceanSPM, aSPMConcentration, aSPMLoad,
                nParams, localLatitude, localHeight, localSalt,
                localTemperature, turbulenceFactor, aSPMRessuspension)
{
    for (int i = 0; i < NumberOfBoxes; i++)
        TotalExtinctionCoefficient[i] = totalExtinctionCoefficient;
}

TSPMCrestuma::TSPMCrestuma(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename, char* parametersFilename)
     :TSPMOceanic(className, timeStep, morphologyFilename, variablesFilename, parametersFilename)
{
    BuildSPMCrestuma();
}
// ...

TSPMCrestuma::TSPMCrestuma(TEcoDynClass* APEcoDynClass, char* className)
								 : TSPMOceanic(APEcoDynClass, className)
{
    BuildSPMCrestuma();
}

void TSPMCrestuma::BuildSPMCrestuma()
{
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
	if (PReadWrite != NULL)
	{
        int X,Y;
        char MyVariable[65];
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            // NumberOfVariables and VariableNameArray[] filled in SPMBasic
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                if (strcmp(MyVariable, "Extinction coefficient Ktot") == 0)
                {
                    int j;
                    for (j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(X+3, i + j, KValue);
                        TotalExtinctionCoefficient[j] = KValue;
                        //if (j == 0) Debugger(TotalExtinctionCoefficient[j]);
                    }
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TSPMCrestuma::~TSPMCrestuma()
{

}

// AP, 2007.07.11
bool TSPMCrestuma::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("SuspendedMatter");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Suspended matter");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    bool spmValues = false;
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Suspended matter") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverSPM, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanSPM, 8);
            spmValues = true;
        }
        else if (strcmp(VariableNameArray[i], "Extinction coefficient Ktot") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(KValue, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save others variables' values
     */
    if (spmValues) {
        PReadWrite->WriteCell("SPM concentration values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("SPM load values");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < NumberOfBoxes; j++)
        {
            PReadWrite->WriteCell(SPMConcentration[j], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(SPMLoad[j], 8);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TSPMCrestuma::Go()
{
	// Active methods
   //Debugger(1);
   //ExtinctionCoefficientKChl();
   //Debugger(2);
   ExtinctionCoefficientKTot();
   //Debugger(3);
}

void TSPMCrestuma::Integrate()
{

}

void TSPMCrestuma::ExtinctionCoefficientKTot()
{
    	/*double MyChl;
   for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (ChlorophyllExtinctionCoefficient[i] > KValue)
			TotalExtinctionCoefficient[i] = KValue + TotalExtinctionCoefficient[i];
      else
         TotalExtinctionCoefficient[i] = KValue;
	} */

}

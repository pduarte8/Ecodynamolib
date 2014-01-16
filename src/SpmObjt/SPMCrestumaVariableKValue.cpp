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
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "spmobjt.h"
#include "iodll.h"



TSPMCrestumaVariableKValue::TSPMCrestumaVariableKValue(TEcoDynClass* APEcoDynClass, char* className)
								 : TSPMCrestuma(APEcoDynClass, className)
{
    BuildSPMCrestumaVariableKValue();
}

void TSPMCrestumaVariableKValue::BuildSPMCrestumaVariableKValue()
{
    DailyKValue = new double[365];
    for (int i = 0; i < 365; i++)
       DailyKValue[i] = 0.0;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("SuspendedMatter");
	if (PReadWrite != NULL)
	{
        int X,Y, XV, YV;
        char MyVariable[65];
        if (PReadWrite->FindString("Suspended matter", X, Y))
        {
            // NumberOfVariables and VariableNameArray[] filled in SPMBasic
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, MyVariable);
                if (strcmp(MyVariable, "Extinction coefficient Ktot") == 0)
                {
                    if (!PReadWrite->FindString("Daily Extinction coefficient Ktot", XV, YV))
                    {
                        XV = X + 3;
                        YV = i - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, DailyKValue[j]);
                        //if (j == 364) Debugger(DailyKValue[j]);
                    }
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TSPMCrestumaVariableKValue::~TSPMCrestumaVariableKValue()
{
    freeMemory();
}

void TSPMCrestumaVariableKValue::freeMemory()
{
    try {
       delete [] DailyKValue;
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
			cerr << "TSPMCrestumaVariableKValue::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

// AP, 2007.07.11
bool TSPMCrestumaVariableKValue::SaveVariables()
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
    bool spmValues = false, ktotValues = false;
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
            PReadWrite->WriteCell(DailyKValue[0], 8);
            ktotValues = true;
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save others variables' values
     */
    if (ktotValues) {
        PReadWrite->WriteCell("Daily Extinction coefficient Ktot");
        PReadWrite->WriteSeparator(true);
        for (int j = 0; j < 365; j++)
        {
            PReadWrite->WriteCell(DailyKValue[j], 8);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }

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

void TSPMCrestumaVariableKValue::Go()
{
   int MyDay;
   MyDay = MIN(364,MyPEcoDynClass->GetJulianDay()-1);
   KValue = DailyKValue[MyDay];
   ExtinctionCoefficientKChl();
   ExtinctionCoefficientKTot();
}





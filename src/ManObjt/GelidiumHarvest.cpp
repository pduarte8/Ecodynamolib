// Man object CPP code file
// @ Pedro Duarte
// Rev. A 99.10.26
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
#include "manobjt.h"
#include "iodll.h"
#include "ecodyn.rh"

TEspichelMan::TEspichelMan(TEcoDynClass* APEcoDynClass, char* className)
							: TIrishMan(APEcoDynClass, className)
{
    int X,Y;

//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)OpenVariablesFile("Man");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Man variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
        cerr << "TEspichelMan::TEspichelMan - Man variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Man", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (int i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TEspichelMan::TEspichelMan - Variables: undefined object Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	ParameterCount = 0;
	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Man");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Man", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Minimal harvest size") == 0)
                    PReadWrite->ReadNumber(X+3, i, MinimalHarvestSize);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Man", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
            cerr << "TEspichelMan::TEspichelMan - Parameters: undefined object Man" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }


   // Fluxes and conversions
   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	HarvestBiomass[i] = 0.0;
      HarvestFlux[i] = 0.0;
      HarvestableBiomass[i] = 0.0;
   }
}

double TEspichelMan::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TIrishMan
    if (strcmp(MyParameter, "Carbon to dry weight") == 0)
        value = CarbonToDryWeight;
    else if (strcmp(MyParameter, "Harvestable weight") == 0)
        value = HarvestableWeight;
    else if (strcmp(MyParameter, "First seeding day") == 0)
        value = FirstSeedingDay;
    else if (strcmp(MyParameter, "Last seeding day") == 0)
        value = LastSeedingDay;
    else if (strcmp(MyParameter, "First harvest day") == 0)
        value = FirstHarvestDay;
    else if (strcmp(MyParameter, "Last harvest day") == 0)
        value = LastHarvestDay;

    // parameters from TEspichelMan
    else if (strcmp(MyParameter, "Minimal harvest size") == 0)
        value = MinimalHarvestSize;
    else
        value = 0.0;
    return value;
}

bool TEspichelMan::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    // parameters from TIrishMan
    if (strcmp(MyParameter, "Carbon to dry weight") == 0)
        CarbonToDryWeight = value;
    else if (strcmp(MyParameter, "Harvestable weight") == 0)
        HarvestableWeight = value;
    else if (strcmp(MyParameter, "First seeding day") == 0)
        FirstSeedingDay = value;
    else if (strcmp(MyParameter, "Last seeding day") == 0)
        LastSeedingDay = value;
    else if (strcmp(MyParameter, "First harvest day") == 0)
        FirstHarvestDay = value;
    else if (strcmp(MyParameter, "Last harvest day") == 0)
        LastHarvestDay = value;

    // parameters from TEspichelMan
    else if (strcmp(MyParameter, "Minimal harvest size") == 0)
        MinimalHarvestSize = value;
    else
        rc = false;

    return rc;
}

void TEspichelMan::Go()
{
	JulianDay = MyPEcoDynClass->GetJulianDay();
   if (
   		(JulianDay >= FirstHarvestDay) &&
      	(JulianDay <= LastHarvestDay)
      )
   {
   	MacrophyteHarvest();
   }
}

void TEspichelMan::Inquiry(char* srcName, double &Value,
										         int BoxNumber,
										         char* ParameterName,
										         int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter,"Harvested biomass")== 0)
		Value = HarvestBiomass[MyBoxNumber];
    else
    {
        Value = 0;
        int j;
        for (j = 0; j < NumberOfVariables; j++)
        {
            if (strcmp(MyParameter, VariableNameArray[j]) == 0)
                break;
        }
        if (j == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, MyParameter);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 1",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
            cerr << "TEspichelMan::Inquiry 1 - " << MyParameter
                << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TEspichelMan::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Harvested biomass") == 0)
		HarvestBiomass[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.16
bool TEspichelMan::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Man");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Man");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();

        // parameters from TIrishMan
        if (strcmp(ParametersNameArray[i], "Carbon to dry weight") == 0)
        {
            PReadWrite->WriteCell(CarbonToDryWeight, 9);
        }
        else if (strcmp(ParametersNameArray[i], "Harvestable weight") == 0)
        {
            PReadWrite->WriteCell(HarvestableWeight, 9);
        }
        else if (strcmp(ParametersNameArray[i], "First seeding day") == 0)
        {
            PReadWrite->WriteCell(FirstSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Last seeding day") == 0)
        {
            PReadWrite->WriteCell(LastSeedingDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "First harvest day") == 0)
        {
            PReadWrite->WriteCell(FirstHarvestDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Last harvest day") == 0)
        {
            PReadWrite->WriteCell(LastHarvestDay, 0);
        }

        // parameters from TEspichelMan
        else if (strcmp(ParametersNameArray[i], "Minimal harvest day") == 0)
        {
            PReadWrite->WriteCell(MinimalHarvestSize, 0);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TEspichelMan::MacrophyteHarvest()
{
	TEcoDynClass* MyMacrophytePointer = MyPEcoDynClass->GetMacrophytePointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if ( MyMacrophytePointer != NULL )
		{
			MyMacrophytePointer->Inquiry(GetEcoDynClassName(), HarvestableBiomass[i],
														i,
														MinimalHarvestSize,
														ObjectCode);
         //Debugger(HarvestableBiomass[i]);
			MyMacrophytePointer->Update(GetEcoDynClassName(), -HarvestableBiomass[i],
                                      MinimalHarvestSize,
                                      i,
                                      ObjectCode);
			HarvestFlux[i] = HarvestableBiomass[i];
		}
	}
}



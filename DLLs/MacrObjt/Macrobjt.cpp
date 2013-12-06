// Macrophyte object CPP code file
// @ Pedro Duarte, Novembro de 1998

/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "macrobjt.h"

// TMacrophytes Class
//
// Constructors invoked outside EcoDyn shell...
//

TMacrophytes::TMacrophytes(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TProdutor(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    BuildMacrophytes();
}

TMacrophytes::TMacrophytes(char* className, float timeStep, char* morphologyFilename)
    :TProdutor(className, timeStep, morphologyFilename)
{
    BuildMacrophytes();
}
// ...

TMacrophytes::TMacrophytes(TEcoDynClass* APEcoDynClass, char* className)
										 : TProdutor(APEcoDynClass, className)

{
    BuildMacrophytes();
}

void TMacrophytes::BuildMacrophytes()
{
  bool BenthicRecord;

	ObjectCode = MACROBJECTCODE;
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();
    AReferenceTemperature = 0.0; ATemperatureCoefficient = 0.0; OxygenDemand = 0.0;
    BenthicRecord = MyPEcoDynClass->HasABenthicSpeciesRecord();
    if (!BenthicRecord)
    {
        if (MyPEcoDynClass->OpenBenthicSpecies(MyPEcoDynClass->GetBenthicSpeciesFile()))
            BenthicRecord = true;
    }
    if (BenthicRecord)
    {
        MyBenthicSpeciesRecord = MyPEcoDynClass->GetABenthicSpeciesRecord();
        NumberOfBoxesWithBenthicSpecies = MyPEcoDynClass->GetNumberOfCellsWithBenthicSpecies();
    }
    else
#ifdef __BORLANDC__
        MessageBox(0,"Benthic species record not initialized",
              "EcoDynamo alert",
              MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TMacrophytes::BuildMacrophytes - Benthic species record not initialized" << endl;
#endif  // __BORLANDC__
}


TMacrophytes::~TMacrophytes()
{
    freeMemory();
}

void TMacrophytes::freeMemory()
{
}


void TMacrophytes::Go()
{
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
    JulianDay = MyPEcoDynClass->GetJulianDay();
	Production();
    Respiration();
    Mortality();
}


void TMacrophytes::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Macrophyte biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "Macrophyte dead biomass") == 0)
        Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "Macrophyte total GPP") == 0)
		Value = GPP[MyBoxNumber];
    else if (strcmp(MyParameter, "Macrophyte total NPP") == 0)
		Value = NPP[MyBoxNumber];
    else if (strcmp(MyParameter, "Macrophyte GPP per unit mass") == 0)
		Value = GPP[MyBoxNumber]
				  / PhytoBiomass[MyBoxNumber]
				  * HOURSTODAYS;
    else if (strcmp(MyParameter, "Macrophyte NPP per unit mass") == 0)
        Value = NPP[MyBoxNumber]
				  / PhytoBiomass[MyBoxNumber]
				  * HOURSTODAYS;
    else if (strcmp(MyParameter, "Macrophyte mortality") == 0)
		Value = PhytoMortality[MyBoxNumber];
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
                      "EcoDynamo Alert - Inquiry 5",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TMacrophytes::Inquiry 5 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}



void TMacrophytes::Update(char* srcName, double Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

	if (strcmp(MyParameter, "Macrophyte biomass") == 0)
		PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
	    char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
    	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 2",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TMacrophytes::Update 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}


bool TMacrophytes::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Macrophyte biomass") == 0)
		PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "Macrophyte total GPP") == 0)
		GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "Macrophyte total NPP") == 0)
		NPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "Macrophyte mortality") == 0)
		PhytoMortality[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

void TMacrophytes::Integrate()
{
	// Integrate macrophyte biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
}

void TMacrophytes::Production()
{
#ifdef __BORLANDC__
	MessageBox(0,
					  "Always override TMacrophytes::Production()",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "Always override TMacrophytes::Production()" << endl;
#endif  // __BORLANDC__
}

void TMacrophytes::Respiration()
{
#ifdef __BORLANDC__
	MessageBox(0,
					  "Always override TMacrophytes::Respiration()",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "Always override TMacrophytes::Respiration()" << endl;
#endif  // __BORLANDC__
}

void TMacrophytes::Mortality()
{
#ifdef __BORLANDC__
	MessageBox(0,
					  "Always override TMacrophytes::Mortality()",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "Always override TMacrophytes::Mortality()" << endl;
#endif  // __BORLANDC__
}


double TMacrophytes::TemperatureLimitation(double AWaterTemperature, double AReferenceTemperature, double ATemperatureCoefficient)
{
        return 1.0 / (1.0 + exp(-ATemperatureCoefficient * (AWaterTemperature - AReferenceTemperature)));
}

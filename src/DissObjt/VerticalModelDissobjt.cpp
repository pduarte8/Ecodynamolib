// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 01.04.27
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"

// TVerticalModelNutrients Class
//
// Constructors invoked outside EcoDyn shell...
//

TVerticalModelNutrients::TVerticalModelNutrients(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[],
        double riverAmmonia, double oceanAmmonia, double aAmmonia[], double aNH4Load[],
        double riverNitrite, double oceanNitrite, double aNitrite[], double aNO2Load[],
        double riverNitrate, double oceanNitrate, double aNitrate[], double aNO3Load[],
        double riverPhosphate, double oceanPhosphate, double aPhosphate[], double aPO4Load[],
        double riverSilica, double oceanSilica, double aSilica[], double aSiO2Load[],
        double riverDON, double oceanDON, double aDON[], double aDONLoad[],
        double riverDOP, double oceanDOP, double aDOP[], double aDOPLoad[],
        double riverDOC, double oceanDOC, double aDOC[], double aDOCLoad[],
        int nParams, double bottomDiffusionCoeff)
    :TNutrientsBasic(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
                nVars, aVarsNames, riverAmmonia, oceanAmmonia, aAmmonia, aNH4Load,
                riverNitrite, oceanNitrite, aNitrite, aNO2Load,
                riverNitrate, oceanNitrate, aNitrate, aNO3Load,
                riverPhosphate, oceanPhosphate, aPhosphate, aPO4Load,
                riverSilica, oceanSilica, aSilica, aSiO2Load,
                riverDON, oceanDON, aDON, aDONLoad,
                riverDOP, oceanDOP, aDOP, aDOPLoad,
                riverDOC, oceanDOC, aDOC, aDOCLoad)
{
	ParameterCount = 0;
    NumberOfParameters = nParams;
    BottomDiffusionCoeff = bottomDiffusionCoeff;
}

TVerticalModelNutrients::TVerticalModelNutrients(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
            :TNutrientsBasic(className, timeStep, morphologyFilename, variablesFilename)
{
    strcpy(ParsFileName, parametersFilename);
    BuildVerticalModelNutrients();
}
// ...

TVerticalModelNutrients::TVerticalModelNutrients(TEcoDynClass* APEcoDynClass,
        char* className) : TNutrientsBasic (APEcoDynClass, className)
{
    BuildVerticalModelNutrients();
}

void TVerticalModelNutrients::BuildVerticalModelNutrients()
{
	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TVerticalModelNutrients::BuildVerticalModelNutrients - Nutrients parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue;
        int X, Y;
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // read in the parameter names
            char MyParameter[65];
            ParametersNameArray = new VNA[NumberOfParameters];

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Bottom diffusion coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    BottomDiffusionCoeff = MyValue;
                }
            }
        }
        CloseDataFile((void*)PReadWrite);
    }
}

double TVerticalModelNutrients::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Bottom diffusion coefficient") == 0)
        value = BottomDiffusionCoeff;
    else
        value = 0.0;
    return value;
}

bool TVerticalModelNutrients::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Bottom diffusion coefficient") == 0)
        BottomDiffusionCoeff = value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TVerticalModelNutrients::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Nutrients");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Nutrients");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);
    /*
     * save parameters' names and values
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        if (i > 0)
            PReadWrite->WriteSeparator();

        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Bottom diffusion coefficient") == 0)
        {
            PReadWrite->WriteCell(BottomDiffusionCoeff, 9);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TVerticalModelNutrients::Go()
{
	BottomDiffusion();
    Nitrification();
    Ammonification();
}


void TVerticalModelNutrients::Integrate()
{
/*
	// Integrate ammonia within the box
	Generic = Ammonia; GenericFlux = NH4Flux;
	Integration();
	Ammonia = Generic; NH4Flux = GenericFlux;

   	// Integrate nitrite within the box
	Generic = Nitrite; GenericFlux = NO2Flux;
	Integration();
	Nitrite = Generic; NO2Flux = GenericFlux;

	// Integrate nitrate within the box
	Generic = Nitrate; GenericFlux = NO3Flux;
	Integration();
	Nitrate = Generic; NO3Flux = GenericFlux;

    // Integrate DON within the box
    Generic = DON; GenericFlux = DONFlux;
	Integration();
	DON = Generic; DONFlux = GenericFlux;

	// Integrate phosphate within the box
	Generic = Phosphate; GenericFlux = PO4Flux;
	Integration();
	Phosphate = Generic; PO4Flux = GenericFlux;

	// Integrate silica within the box
	Generic = Silica; GenericFlux = SiO2Flux;
	Integration();
	Silica = Generic; SiO2Flux = GenericFlux;

    // Integrate DOC within the box
    Generic = DOC; GenericFlux = DOCFlux;
	Integration();
	DOC = Generic; DOCFlux = GenericFlux;
*/
    Integration(Ammonia, NH4Flux,
                Nitrite, NO2Flux,
                Nitrate, NO3Flux,
                Phosphate, PO4Flux,
                Silica, SiO2Flux);
    Integration(DON, DONFlux);
    Integration(DOC, DOCFlux);

    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
    {
        MyTransportPointer->Go(Ammonia, 0, 0, 0);
        MyTransportPointer->Go(Nitrite, 0, 0, 0);
        MyTransportPointer->Go(Nitrate, 0, 0, 0);
        MyTransportPointer->Go(DON, 0, 0, 0);
        MyTransportPointer->Go(Phosphate, 0, 0, 0);
        MyTransportPointer->Go(Silica, 0, 0, 0);
        MyTransportPointer->Go(DOC, 0, 0, 0);
    }
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (Ammonia[i]   < 0.0)   Ammonia[i]   = 0.0;
		if (Nitrite[i]   < 0.0)   Nitrite[i]   = 0.0;
		if (Nitrate[i]   < 0.0)   Nitrate[i]   = 0.0;
        if (DON[i]       < 0.0)   DON[i]       = 0.0;
		if (Phosphate[i] < 0.0)   Phosphate[i] = 0.0;
		if (Silica[i]    < 0.0)   Silica[i]    = 0.0;
        if (DOC[i]       < 0.0)   DOC[i]       = 0.0;
	}
}


void TVerticalModelNutrients::Mixing()
{
	int MyBoxNumber = 1;
    double MyMixedLayerDepth, MyAverageAmmonia, MyAverageNitrate, MyTop, MyBottom;

	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
   	    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyMixedLayerDepth,MyBoxNumber,
                                      "Mixed layer depth",
                                      ObjectCode);

        MyAverageAmmonia = 0.0;
        MyAverageNitrate = 0.0;

        int i = 0;
        do
		{
      	    MyPEcoDynClass->GetBoxTopAndBottom(i,MyTop,MyBottom);
      	    MyAverageAmmonia = MyAverageAmmonia + Ammonia[i];
            MyAverageNitrate = MyAverageNitrate + Nitrate[i];
            i++;
        }
        while (MyBottom < MyMixedLayerDepth);
        MyAverageAmmonia = MyAverageAmmonia / i;
        MyAverageNitrate = MyAverageNitrate / i;
    }
}


void TVerticalModelNutrients::BottomDiffusion()
{
	NO3Flux[NumberOfBoxes - 1] = NO3Flux[NumberOfBoxes - 1] + BottomDiffusionCoeff *
               	                 (OceanNitrate - Nitrate[NumberOfBoxes - 1]);
}

void TVerticalModelNutrients::Nitrification()
{
}

void TVerticalModelNutrients::Ammonification()
{
}


// Man object CPP code file
// @ Pedro Duarte
// Translated by J. Gomes Ferreira
// Rev. A 96.06.25
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
#include "ecodyn.rh"

// TMan Class
//
// Constructors invoked outside EcoDyn shell...
//

TMan::TMan(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    BuildMan(className);
}

TMan::TMan(char* className, float timeStep, char* morphologyFilename)
        :TEcoDynClass(timeStep)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    strcpy(MorphologyFileName, morphologyFilename);
    if (OpenMorphology())
        BuildMan(className);
}
// ...

TMan::TMan(TEcoDynClass* APEcoDynClass, char* className) : TEcoDynClass()
{
    MyPEcoDynClass = APEcoDynClass;
    BuildMan(className);
}

void TMan::BuildMan(char* className)
{
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
	BoxArray = MyPEcoDynClass->BoxArray;
	TimeStep = MyPEcoDynClass->GetTimeStep();

	VariableCount = 0;
	ParameterCount = 0;

	ObjectCode = MANOBJECTCODE;

    // AP, 2007.07.16
    bool BenthicRecord = MyPEcoDynClass->HasABenthicSpeciesRecord();
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
    NumberOfVariables = 0;
}

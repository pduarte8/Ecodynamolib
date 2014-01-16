// Zoobenthos object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.06.13
#include "ecodyn.rh"
#include "zoobobjt.h"

// TZoobenthos Class
//
// Constructors invoked outside EcoDyn shell...
//

TSuspensionFeeders::TSuspensionFeeders(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
    :TZoobenthos(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
}

TSuspensionFeeders::TSuspensionFeeders(char* className, float timeStep, char* morphologyFilename)
        :TZoobenthos(className, timeStep, morphologyFilename)
{
}
// ...

TSuspensionFeeders::TSuspensionFeeders(TEcoDynClass* APEcoDynClass, char* className)
										 :TZoobenthos(APEcoDynClass, className)
{
   NumberOfClasses = 1;
}

TSuspensionFeeders::~TSuspensionFeeders()
{
}


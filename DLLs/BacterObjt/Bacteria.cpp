/**
 * NPI work
 * 	18 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
    #include <stdlib.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
    #include <cstdlib>

	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "bacteriaobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"


TBacteria::TBacteria(TEcoDynClass* APEcoDynClass, char* className): TEcoDynClass()
{
    MyPEcoDynClass = APEcoDynClass;
    strcpy(EcoDynClassName, className);
    NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
    TimeStep = MyPEcoDynClass->GetTimeStep();
    CriticalDepthForLandBoundary = MyPEcoDynClass->GetCriticalDepth();
    NumberOfLoads = 0; NumberOfRiverLoads = 0; NumberOfDaysForLoads = 0;
    BacteriaFlux = new double[NumberOfBoxes];
    BacteriaNumberLoad = new double[NumberOfBoxes];
    BoxArray = MyPEcoDynClass->BoxArray;
    for (int i = 0; i < NumberOfBoxes; i++)
    {
       BacteriaFlux[i] = 0.0; BacteriaNumberLoad[i] = 0.0;
       if (i < 365)
          DailyBoundaryBacteria[i] = 0.0;
    }
}

TBacteria::~TBacteria()
{
    freeMemory();
}

void TBacteria::freeMemory()
{
   delete [] BacteriaFlux;
   delete [] BacteriaNumberLoad;
}

double TBacteria::GetParameterValue(char* parmName)
{

}

bool TBacteria::SetParameterValue(char* parmName, double value)
{
}

void TBacteria::Go()
{
}

void TBacteria::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName,int AnObjectCode)
{
}

void TBacteria::Inquiry(char* srcName, double &Value,int BoxX,int BoxY,char* ParameterName,int AnObjectCode)
{
}

void TBacteria::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
}

void TBacteria::Integrate()
{
   Loads();
}

void TBacteria::Reinitialize()
{
}

bool TBacteria::SetVariableValue(char* srcName,double Value,int BoxNumber,char* VariableName)
{
}

bool TBacteria::SaveParameters()
{
}


void TBacteria::Production(int ABoxNumber)
{
}

void TBacteria::ProductionWithTemperature(int ABoxNumber)
{
}

void TBacteria::NutrientLimitation(int ABoxNumber)
{
}

void TBacteria::NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber)
{
}

void TBacteria::TemperatureLimitation(int ABoxNumber)
{
}

void TBacteria::Respiration(int ABoxNumber)
{
}

void TBacteria::Exudation(int ABoxNumber)
{
}

void TBacteria::Senescence(int ABoxNumber)
{
}

void TBacteria::Mortality(int ABoxNumber)
{
}

void TBacteria::Loads()
{
}

	// Macrophyte object CPP code file
// @ Pedro Duarte, Outubro de 1999

/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <vcl.h>
	#include <math.h>
	#include <stdlib.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "macrobjt.h"
#include "iodll.h"


// TLaminariaJaponicaExponentialGrowth Class
//
// Constructors invoked outside EcoDyn shell...
//

TLaminariaJaponicaExponentialGrowth::TLaminariaJaponicaExponentialGrowth(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aPhytoBiomass[], double aPhytoIndividualWeight[],
        int nParams, double intrinsicRateOfIncrease, double startDay,
        double harvestDay, double carbonContents, double nitrogenContents,
        double betat, double ts, double te, double kgrowth)
    :TMacrophytes(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildLJEGrowth();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		PhytoBiomass[i] = aPhytoBiomass[i];
		PhytoIndividualWeight[i] = aPhytoIndividualWeight[i];
	}

    NumberOfParameters = nParams;
    IntrinsicRateOfIncrease = intrinsicRateOfIncrease;
    StartDay = startDay;
    HarvestDay = harvestDay;
    CarbonContents = carbonContents;
    NitrogenContents = nitrogenContents;
    Betat = betat;
    Ts = ts;
    Te = te;
    Kgrowth = kgrowth;
}

TLaminariaJaponicaExponentialGrowth::TLaminariaJaponicaExponentialGrowth(char* className,
        float timeStep, char* morphologyFilename, char* variablesFilename, char* parametersFilename)
    :TMacrophytes(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    strcpy(ParsFileName, parametersFilename);
    BuildLJEGrowth();
}
// ...

TLaminariaJaponicaExponentialGrowth::TLaminariaJaponicaExponentialGrowth(
    TEcoDynClass* APEcoDynClass, char* className)
							 : TMacrophytes(APEcoDynClass, className)

{
    NumberOfBoxesWithLaminariaJaponica = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
      if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Laminaria japonica") == 0)
              NumberOfBoxesWithLaminariaJaponica++;
    LaminariaJaponicaRecord = new BenthicSpeciesRecord[NumberOfBoxesWithLaminariaJaponica];
    int AIndex = 0;
    for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
    {
      if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Laminaria japonica") == 0)
      {
        strcpy(LaminariaJaponicaRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
        LaminariaJaponicaRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
        LaminariaJaponicaRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
        AIndex++;
      }
    }
    BuildLJEGrowth();
}

void TLaminariaJaponicaExponentialGrowth::PreBuildLJEGrowth()
{
    ATimeStep = MyPEcoDynClass->GetTimeStep();
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();
	ParameterCount = 0;
	VariableCount = 0;

    PhytoIndividualWeight = new double[NumberOfBoxes];
    PhytoIndividualFlux = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    PhytoIndividualWeight[i] = 0.0;
        PhytoIndividualFlux[i] = 0.0;
    }
}

void TLaminariaJaponicaExponentialGrowth::BuildLJEGrowth()
{
    PreBuildLJEGrowth();

    int X, Y;
    // Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("LaminariaJaponica");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "LaminariaJaponica parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TLaminariaJaponicaExponentialGrowth::BuildJLEGrowth - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        double MyValue;
        if (PReadWrite->FindString("Laminaria japonica", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];

            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Intrinsic rate of increase") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, IntrinsicRateOfIncrease);
                }
                else if (strcmp(MyParameter, "Start day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, StartDay);
                }
                else if (strcmp(MyParameter, "Harvest day") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, HarvestDay);
                }
                else if (strcmp(MyParameter, "Carbon contents") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CarbonContents);
                }
                else if (strcmp(MyParameter, "Nitrogen contents") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, NitrogenContents);
                }
                else if (strcmp(MyParameter, "Betat") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Betat);
                }
                else if (strcmp(MyParameter, "Optimal temperature") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Ts);
                }
                else if (strcmp(MyParameter, "Lethal temperature") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Te);
                }
                else if (strcmp(MyParameter, "Growth assimptote") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Kgrowth);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - L. japonica", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TLaminariaJaponicaExponentialGrowth::BuildLJEGrowth - Parameters: undefined object L. japonica" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    // Read in the variables
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("LaminariaJaponica");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "LaminariaJaponica variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TLaminariaJaponicaExponentialGrowth::BuildJLEGrowth - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        int Index, XV, YV;
        if (PReadWrite->FindString("Laminaria japonica", X, Y))
        {
            char MyVariable[65];
            double Value;

            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            for (int i = 0; i < NumberOfVariables; i++)
            {
                 PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
                 strcpy(VariableNameArray[i], MyVariable);

                 if (strcmp(MyVariable, "L. japonica biomass density") == 0)
                 {
                    if (!PReadWrite->FindString("L. japonica biomass density values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithLaminariaJaponica; j++)
                    {
                        Index = LaminariaJaponicaRecord[j].LineCoordinate * NumberOfColumns +
                                LaminariaJaponicaRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[Index]);
                    }
                 }
                 else if (strcmp(MyVariable, "L. japonica individual weight") == 0)
                 {
                    if (!PReadWrite->FindString("L. japonica individual weight values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithLaminariaJaponica; j++)
                    {
                        Index = LaminariaJaponicaRecord[j].LineCoordinate * NumberOfColumns +
                                LaminariaJaponicaRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoIndividualWeight[Index]);
                    }
                 }
             }
        }
        else
#ifdef __BORLANDC__
					MessageBox(0, "Variables: Undefined object - L. japonica", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TLaminariaJaponicaExponentialGrowth::BuildLJEGrowth - Variables: undefined object L. japonica" << endl;
#endif  // __BORLANDC__

        CloseDataFile((void*)PReadWrite);
    }
}

TLaminariaJaponicaExponentialGrowth::~TLaminariaJaponicaExponentialGrowth()
{
    freeMemory();
}

void TLaminariaJaponicaExponentialGrowth::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
        {
            delete [] PhytoIndividualWeight;
            delete [] PhytoIndividualFlux;
        }
        if (NumberOfBoxesWithLaminariaJaponica > 0) {
            delete [] LaminariaJaponicaRecord;
        }

        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
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
				cerr << "TLaminariaJaponicaExponentialGrowth::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TLaminariaJaponicaExponentialGrowth::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Intrinsic rate of increase") == 0)
    {
        value = IntrinsicRateOfIncrease;
    }
    else if (strcmp(MyParameter, "Start day") == 0)
    {
        value = StartDay;
    }
    else if (strcmp(MyParameter, "Harvest day") == 0)
    {
        value = HarvestDay;
    }
    else if (strcmp(MyParameter, "Carbon contents") == 0)
    {
        value = CarbonContents;
    }
    else if (strcmp(MyParameter, "Nitrogen contents") == 0)
    {
        value = NitrogenContents;
    }
    else if (strcmp(MyParameter, "Betat") == 0)
    {
        value = Betat;
    }
    else if (strcmp(MyParameter, "Optimal temperature") == 0)
    {
        value = Ts;
    }
    else if (strcmp(MyParameter, "Lethal temperature") == 0)
    {
        value = Te;
    }
    else if (strcmp(MyParameter, "Growth assimptote") == 0)
    {
        value = Kgrowth;
    }
    else
        value = 0.0;
    return value;
}

bool TLaminariaJaponicaExponentialGrowth::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Intrinsic rate of increase") == 0)
    {
        IntrinsicRateOfIncrease = value;
    }
    else if (strcmp(MyParameter, "Start day") == 0)
    {
        StartDay = value;
    }
    else if (strcmp(MyParameter, "Harvest day") == 0)
    {
        HarvestDay = value;
    }
    else if (strcmp(MyParameter, "Carbon contents") == 0)
    {
        CarbonContents = value;
    }
    else if (strcmp(MyParameter, "Nitrogen contents") == 0)
    {
        NitrogenContents = value;
    }
    else if (strcmp(MyParameter, "Betat") == 0)
    {
        Betat = value;
    }
    else if (strcmp(MyParameter, "Optimal temperature") == 0)
    {
        Ts = value;
    }
    else if (strcmp(MyParameter, "Lethal temperature") == 0)
    {
        Te = value;
    }
    else if (strcmp(MyParameter, "Growth assimptote") == 0)
    {
        Kgrowth = value;
    }
    else
        rc = false;

    return rc;
}


void TLaminariaJaponicaExponentialGrowth::Update(char* srcName, double Value,
									                       int BoxNumber,
									                       char* ParameterName,
									                       int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

	for (int i=0; i < NumberOfVariables; i++)
    {
   	if (strcmp(MyParameter, "L. japonica biomass input/output") == 0)
      {
   		PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Value;
      }
      else
      if (strcmp(MyParameter, "L. japonica individual weight") == 0)
      {
   		PhytoIndividualWeight[MyBoxNumber] = Value;
      }
      return;
   }

#ifdef __BORLANDC__
	char Caption[129];
	strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
   //Debugger(AnObjectCode);
	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 1",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "TLaminariaJaponicaExponentialGrowth::Update 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
}


bool TLaminariaJaponicaExponentialGrowth::SetVariableValue(char* srcName,
                                                           double Value,
									                       int BoxNumber,
									                       char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "L. japonica biomass density") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "L. japonica individual weight") == 0)
        PhytoIndividualWeight[BoxNumber] = Value;
    else if (strcmp(VariableName, "L. japonica NPP") == 0)
      	NPP[BoxNumber] = Value / DAYSTOHOURS / HOURSTOSECONDS;
    else
        rc = false;

    return rc;
}

void TLaminariaJaponicaExponentialGrowth::Inquiry(char* srcName, double &Value,
                                     int BoxNumber,
                                     char* ParameterName,
                                     int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);

   	if (strcmp(MyParameter, "L. japonica biomass density") == 0)
      	Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "L. japonica individual weight") == 0)
      	Value = PhytoIndividualWeight[MyBoxNumber];
    else if (strcmp(MyParameter, "L. japonica density") == 0)
    {
      	if ((PhytoBiomass[MyBoxNumber] > 0.0) && (PhytoIndividualWeight[MyBoxNumber] > 0.0))
      		Value = PhytoBiomass[MyBoxNumber] / PhytoIndividualWeight[MyBoxNumber];
        else
         	Value = 0.0;
    }
    else if (strcmp(MyParameter, "L. japonica NPP") == 0)
      	Value = NPP[MyBoxNumber] * DAYSTOHOURS * HOURSTOSECONDS; //g DW m-2 day-1
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
                   "EcoDynamo Alert - Inquiry 4",
                   MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
					cerr << "TLaminariaJaponicaExponentialGrowth::Inquiry 4 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


// AP, 2007.07.13
bool TLaminariaJaponicaExponentialGrowth::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("LaminariaJaponica");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Laminaria japonica");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * laminaria japonica density initial values
     */
    PReadWrite->WriteCell("L. japonica biomass density values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("L. japonica individual weight values");
    PReadWrite->WriteSeparator(true);
    int Index;
    for (int j = 0; j < NumberOfBoxesWithLaminariaJaponica; j++)
    {
        Index = LaminariaJaponicaRecord[j].LineCoordinate * NumberOfColumns +
               LaminariaJaponicaRecord[j].ColumnCoordinate;
        PReadWrite->WriteCell(PhytoBiomass[Index], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PhytoIndividualWeight[Index], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TLaminariaJaponicaExponentialGrowth::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("LaminariaJaponica");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Laminaria japonica");
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

        if (strcmp(ParametersNameArray[i], "Intrinsic rate of increase") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfIncrease, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Start day") == 0)
        {
            PReadWrite->WriteCell(StartDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Harvest day") == 0)
        {
            PReadWrite->WriteCell(HarvestDay, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon contents") == 0)
        {
            PReadWrite->WriteCell(CarbonContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Nitrogen contents") == 0)
        {
            PReadWrite->WriteCell(NitrogenContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Betat") == 0)
        {
            PReadWrite->WriteCell(Betat, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Optimal temperature") == 0)
        {
            PReadWrite->WriteCell(Ts, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Lethal temperature") == 0)
        {
            PReadWrite->WriteCell(Te, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Growth assimptote") == 0)
        {
            PReadWrite->WriteCell(Kgrowth, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TLaminariaJaponicaExponentialGrowth::Go()
{
   double MyYear; int i,j, index;
   JulianDay = MyPEcoDynClass->GetJulianDay();
   MyYear = MyPEcoDynClass->GetYear();
   double MyBoxDepth;
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   for (int m = 0; m < NumberOfBoxesWithLaminariaJaponica; m++)
   {
     i = LaminariaJaponicaRecord[m].LineCoordinate;
     j = LaminariaJaponicaRecord[m].ColumnCoordinate;
     if (
            (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
            (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
        )
     {
        index = i * NumberOfColumns + j;
        if (MyTransportPointer != NULL)
          MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,index,"Box depth",ObjectCode);
        else
          MyBoxDepth = MyPEcoDynClass->GetBoxDepth(index);
        if ((PhytoBiomass[index] > 0.0) && (MyBoxDepth > 0.0))
        {
          //Debugger(index);
           Growth(index);
           PhytoIndividualFlux[index] = NPP[index] / PhytoBiomass[index] * PhytoIndividualWeight[index]; //g DW / individual / day
        }
     }
   }
}

void TLaminariaJaponicaExponentialGrowth::Integrate()
{

	// Integrate biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;

//  Generic = PhytoIndividualWeight; GenericFlux = PhytoIndividualFlux;
	Integration(PhytoIndividualWeight, PhytoIndividualFlux);
//	PhytoIndividualWeight = Generic; PhytoIndividualFlux = GenericFlux;
}


void TLaminariaJaponicaExponentialGrowth::Growth(int ABoxNumber)
{
	int MyBox;
   MyBox = ABoxNumber;
   if(PhytoIndividualWeight[MyBox] < Kgrowth)
   {
      //if (MyBox == 305) Debugger(1000);
      
   	NPP[MyBox] = IntrinsicRateOfIncrease *         //day-1
      	          TemperatureLimitation(MyBox);

   	NPP[MyBox] = NPP[MyBox] * PhytoBiomass[MyBox] / DAYSTOHOURS / HOURSTOSECONDS; //g DW m-2 s-1

   	NutrientLimitationByAmmoniaAndNitrate(MyBox);

   	PhytoFlux[MyBox] = PhytoFlux[MyBox] + NPP[MyBox];  //g DW m-2 s-1
   }
   else
   {
    	NPP[MyBox] = 0.0;
      //PhytoFlux[MyBox] = 0.0;
   }
}



double TLaminariaJaponicaExponentialGrowth::TemperatureLimitation(int ABoxNumber)
{

	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature, Xt;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      //Debugger(Te); Debugger(Ts);
      if (WaterTemperature >= 25.0)
      	return 0.0;
      else
      {
      	if (Ts - Te != 0.0)
      		Xt = (WaterTemperature - Te) / (Ts - Te);
      	else
      	{
#ifdef __BORLANDC__
         	MessageBox(0,"Ts = Te",
            	        "EcoDynamo Alert in TLaminariaJaponicaExponentialGrowth::TemperatureLimitation ",
               	     MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
					cerr << "TLaminariaJaponicaExponentialGrowth::TemperatureLimitation - Ts = Te" << endl;
#endif  // __BORLANDC__
      	}
      	if (Xt * Xt + 2.0 * Betat * Xt + 1.0 != 0.0)
      		return 2.0 * (1.0 + Betat) * Xt / (Xt * Xt + 2.0 * Betat * Xt + 1.0);
      	else
      	{
#ifdef __BORLANDC__
      		MessageBox(0,"Impossible to evaluate function",
            	        "EcoDynamo Alert in TLaminariaJaponicaExponentialGrowth::TemperatureLimitation ",
               	     MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
					cerr << "TLaminariaJaponicaExponentialGrowth::TemperatureLimitation - Impossible to evaluate function" << endl;
#endif  // __BORLANDC__

      	}
      }
   }
   else return 1.0;
}


void TLaminariaJaponicaExponentialGrowth::NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber)
{
	double LimitingNutrient1, LimitingNutrient2, LimitingNutrient3
   /*,NitrateLimitation, AmmoniumLimitation*/, MyBoxDepth;
   int MyBoxNumber;

   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	if (MyNutrientPointer != NULL)
	{
		// the limitation is based on umol l-1 values passed from the
		// communicate method
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient1,
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);
		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient2,
												 MyBoxNumber,
												 "Nitrate",
												 ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient3,
												 MyBoxNumber,
												 "Nitrite",
												 ObjectCode);

      //It is assumed that if there is enough inorganic nitrogen to sustain a certain production level then
      //there is no limitation. Whenever nitrogen is not enough for the light and temperature limited productivity
      //then productivity is reduced until the minimal value allowed by available nitrogen (the sum of ammonia and nitrate.
      //This may consume virtually all available inorganic nitrogen in the water

      if (MyTransportPointer != NULL)
      {
      	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
                                         MyBoxNumber,
                                         "Box depth",
                                         ObjectCode);
      }
      else
      	MyBoxDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);

      if (
      		NPP[MyBoxNumber] / MyBoxDepth * NitrogenContents * CUBIC /         // mg N / m3
            NITROGENATOMICWEIGHT * ATimeStep >              //productivity expressed in umol N / L / timestep
            (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3)
         )
         NPP[MyBoxNumber] = (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) *  MyBoxDepth *
                                   NITROGENATOMICWEIGHT / CUBIC / NitrogenContents / ATimeStep;  //productivity expressed in is normal units - g DW / m2 / s

      if (NPP[MyBoxNumber] < 0.0000000001) //This is to avoid extremely small values that may lead to errors
      	NPP[MyBoxNumber] = 0.0;

      if (
      		((LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) > 0.0000000001) &&
            (NPP[MyBoxNumber] > 0.0)
          )
      {
      	MyNutrientPointer->Update(GetEcoDynClassName(), -NPP[MyBoxNumber] * LimitingNutrient1 /
                                     (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) /
                                     MyBoxDepth * NitrogenContents * CUBIC / NITROGENATOMICWEIGHT * CUBIC,                    //umol / m3
                                     MyBoxNumber,
                                     "Ammonia",
                                     ObjectCode);
      	MyNutrientPointer->Update(GetEcoDynClassName(), -NPP[MyBoxNumber] * LimitingNutrient2 /
                                     (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) /
                                     MyBoxDepth * NitrogenContents * CUBIC / NITROGENATOMICWEIGHT * CUBIC,                    //umol / m3
                                     MyBoxNumber,
                                     "Nitrate",
                                     ObjectCode);
         MyNutrientPointer->Update(GetEcoDynClassName(), -NPP[MyBoxNumber] * LimitingNutrient3 /
                                    (LimitingNutrient1 + LimitingNutrient2 + LimitingNutrient3) /
                                     MyBoxDepth * NitrogenContents * CUBIC / NITROGENATOMICWEIGHT * CUBIC,                    //umol / m3
                                     MyBoxNumber,
                                     "Nitrite",
                                     ObjectCode);
		}
	}
}



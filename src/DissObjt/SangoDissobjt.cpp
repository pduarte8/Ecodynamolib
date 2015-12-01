// Dissolved object CPP code file
// @ Pedro Duarte
// Rev. A 08.09.2004
/**
 * NPI work
 * 	04 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "dissobjt.h"
#include "iodll.h"
#include "params.h"
#include "EcoDynClass.h"

// TSangoNutrients Class
//
// Constructors invoked outside EcoDyn shell...
//
#ifndef _PORT_FORTRAN_
TSangoNutrients::TSangoNutrients(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[],
        double aAmmonia[], double aNH4Load[], double aNitrite[], double aNO2Load[],
        double aNitrate[], double aNO3Load[], double aBoundaryNH4Concentration[],
        double aBoundaryNO3Concentration[], double aBoundaryNO2Concentration[])
    :TNutrients(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildSangoNutrients();

	NumberOfVariables = nVars;
	VariableNameArray = new VNA[NumberOfVariables];
    for (int i = 0; i < NumberOfVariables; i++)
        strcpy(VariableNameArray[i], aVarsNames[i]);

	for (int j = 0; j < NumberOfBoxes; j++)
	{
		Ammonia[j] = aAmmonia[j];
		NH4Load[j] = aNH4Load[j];
		Nitrite[j] = aNitrite[j];
		NO2Load[j] = aNO2Load[j];
		Nitrate[j] = aNitrate[j];
		NO3Load[j] = aNO3Load[j];
    }
    for (int j = 0; j < 365; j++)
    {
		BoundaryNH4Concentration[j] = aBoundaryNH4Concentration[j];
		BoundaryNO3Concentration[j] = aBoundaryNO3Concentration[j];
		BoundaryNO2Concentration[j] = aBoundaryNO2Concentration[j];
	}
    PosBuildSangoNutrients();
}

TSangoNutrients::TSangoNutrients(char* className, float timeStep,
        char* morphologyFilename, char* variablesFilename)
                    :TNutrients(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    BuildSangoNutrients();
}
// ...

TSangoNutrients::TSangoNutrients(TEcoDynClass* APEcoDynClass, char* className)
							  : TNutrients(APEcoDynClass, className)
{
    // Receive pointer to integrate
    BuildSangoNutrients();
}
#endif

void TSangoNutrients::PreBuildSangoNutrients()
{
// already initialized in TNutrients
//    NumberOfBoxes = MyPEcoDynClass->GetGridSize();
//	TimeStep = MyPEcoDynClass->GetTimeStep();
//	ObjectCode = NUTROBJECTCODE;
    //Debugger(NumberOfDaysForBoundaryTimeSeries);
    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Nutrients");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
        MessageBox(0, "Nutrients parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TSangoNutrients::PreBuildSangoNutrients - Nutrients parameters file missing." << endl;
#endif  // __BORLANDC__
    }
    else
    {
        int X,Y;
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            char MyParameter[65];

            ParametersNameArray = new VNA[NumberOfParameters];

            for (int i = 0; i < NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2+i, Y, MyParameter);
                strcpy(ParametersNameArray[i], MyParameter);

                if (strcmp(MyParameter, "NumberOfDaysForBoundaryTimeSeries") == 0)
                {
                    PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfDaysForBoundaryTimeSeries);
                }
            }
         }
    }
    //Debugger(NumberOfDaysForBoundaryTimeSeries);
    BoxArray = MyPEcoDynClass->BoxArray;
    WesternBoundaryNumber = 4;
    SouthernBoundaryNumber = 3;
    EasternBoundaryNumber = 2;
    NorthernBoundaryNumber = 1;
    BoundaryNH4Concentration = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryNO3Concentration = new double[NumberOfDaysForBoundaryTimeSeries];
    BoundaryNO2Concentration = new double[NumberOfDaysForBoundaryTimeSeries];
    for (int i = 0; i < NumberOfDaysForBoundaryTimeSeries; i++)
    {
        BoundaryNH4Concentration[i] = 0.0;
        BoundaryNO3Concentration[i] = 0.0;
        BoundaryNO2Concentration[i] = 0.0;
    }
    ParameterCount = 0;
    GridLines = MyPEcoDynClass->GetNumberOfLines();
    GridColumns = MyPEcoDynClass->GetNumberOfColumns();

	VariableCount = 0;
}

void TSangoNutrients::BuildSangoNutrients()
{
    PreBuildSangoNutrients();
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSangoNutrients::BuildSangoNutrients - Nutrients variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        char MyVariable[65];
        int X, Y, XV, YV, XVL;

        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
                strcpy(MyVariable, VariableNameArray[i]);
                if (strcmp(MyVariable, "Ammonia") == 0)
                {
                    if (!PReadWrite->FindString("Ammonia values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                        XVL = XV + 6;
                    }
                    else
                        XVL = XV + 1;

                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Ammonia[j]);
                        PReadWrite->ReadNumber(XVL, YV + 1 + j, NH4Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Nitrate") == 0)
                {
                    if (!PReadWrite->FindString("Nitrate values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                        XVL = XV + 6;
                    }
                    else
                        XVL = XV + 1;

                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrate[j]);
                        PReadWrite->ReadNumber(XVL, YV + 1 + j, NO3Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Nitrite") == 0)
                {
                    if (!PReadWrite->FindString("Nitrite values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                        XVL = XV + 6;
                    }
                    else
                        XVL = XV + 1;

                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrite[j]);
                        PReadWrite->ReadNumber(XVL, YV + 1 + j, NO2Load[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary NH4 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NH4 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNH4Concentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary NO3 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NO3 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNO3Concentration[j]);
                    }
                }
                else if (strcmp(MyVariable, "Boundary NO2 concentration") == 0)
                {
                    if (!PReadWrite->FindString("Boundary NO2 concentration values", XV, YV))
                    {
                        XV = X + 2 + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, BoundaryNO2Concentration[j]);
                    }
                }
            }
        }
        else
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in Sango nutrients",
                    "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoNutrients::BuildSangoNutrients - Variables: undefined object in Sango nutrients" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }
    PosBuildSangoNutrients();
}

void TSangoNutrients::PosBuildSangoNutrients()
{
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        Ammonia[i]   = Ammonia[i] * CUBIC; // umol l-1 * CUBIC = umol m-3
        Nitrite[i]   = Nitrite[i] * CUBIC;
        Nitrate[i]   = Nitrate[i] * CUBIC;
        NH4Load[i]   = NH4Load[i] * CUBIC;
        NO3Load[i]   = NO3Load[i] * CUBIC;
        NO2Load[i]   = NO2Load[i] * CUBIC;
    }
    for (int i = 0; i < NumberOfDaysForBoundaryTimeSeries; i++)
    {
   	    BoundaryNH4Concentration[i] = BoundaryNH4Concentration[i] * CUBIC;
        BoundaryNO3Concentration[i] = BoundaryNO3Concentration[i] * CUBIC;
        BoundaryNO2Concentration[i] = BoundaryNO2Concentration[i] * CUBIC;
    }
}

TSangoNutrients::~TSangoNutrients()
{
    freeMemory();
}

void TSangoNutrients::freeMemory()
{
    try {
       delete [] BoundaryNH4Concentration;
       delete [] BoundaryNO3Concentration;
       delete [] BoundaryNO2Concentration;

       if (NumberOfVariables > 0)
          delete [] VariableNameArray;
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
				cerr << "TSangoNutrients::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

// AP, 2007.07.10
bool TSangoNutrients::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Nutrients");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Nutrients");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */
    PReadWrite->WriteCell("Boundary NH4 concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary NO3 concentration values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Boundary NO2 concentration values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
    {
        PReadWrite->WriteCell(BoundaryNH4Concentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryNO3Concentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(BoundaryNO2Concentration[j] / CUBIC, 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("Ammonia values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NH4Load values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3Load values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrite values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2Load values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Ammonia[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NH4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO3Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrite[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TSangoNutrients::Inquiry(char* srcName, double &Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Ammonia") == 0)
		Value = Ammonia[MyBoxNumber] / CUBIC;
	else if (strcmp(MyParameter, "Nitrite") == 0)
		Value = Nitrite[MyBoxNumber] / CUBIC;
	else if (strcmp(MyParameter, "Nitrate") == 0)
		Value = Nitrate[MyBoxNumber] / CUBIC;
	else if (strcmp(MyParameter, "DIN") == 0)
		Value = ( Ammonia[MyBoxNumber]
	    			 + Nitrite[MyBoxNumber]
		    		 + Nitrate[MyBoxNumber]
    			  ) / CUBIC;
    else if (strcmp(MyParameter, "Boundary NH4 concentration") == 0)
		Value = BoundaryNH4Concentration[JulianDay - 1] / CUBIC;
	else if (strcmp(MyParameter, "Boundary NO3 concentration") == 0)
		Value = BoundaryNO3Concentration[JulianDay - 1] / CUBIC;
	else if (strcmp(MyParameter, "Boundary NO2 concentration") == 0)
		Value = BoundaryNO2Concentration[JulianDay - 1] / CUBIC;
    else {
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
                      "EcoDynamo Alert - Inquiry 8",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSangoNutrients::Inquiry 8 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


void TSangoNutrients::Integrate()
{
	int i,j;
    double MyBoxDepth;
	// EcoDynClass ammonia within the box
//	Generic = Ammonia; GenericFlux = NH4Flux;
	Integration(Ammonia, NH4Flux);
//	Ammonia = Generic; NH4Flux = GenericFlux;

	// EcoDynClass nitrate within the box
//	Generic = Nitrate; GenericFlux = NO3Flux;
	Integration(Nitrate, NO3Flux);
//	Nitrate = Generic; NO3Flux = GenericFlux;

    // EcoDynClass nitrite within the box
//	Generic = Nitrite; GenericFlux = NO2Flux;
	Integration(Nitrite, NO2Flux);
//	Nitrite = Generic; NO2Flux = GenericFlux;

	// Transport the nutrients
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
		MyTransportPointer->Go(Ammonia, Ammonia, 0.0, 0.0);

		MyTransportPointer->Go(Nitrate, Nitrate, 0.0, 0.0);

        MyTransportPointer->Go(Nitrite, Nitrite, 0.0, 0.0);
		//Southern boundary
   	    for (j = 0; j < GridColumns; j++)
   	    {
   		    i = 0;
      	    MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
         	                             i * GridColumns + j,
            	                          "Box depth",
               	                       ObjectCode);
      	    if (
      			(MyBoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
      		)
      	    {
         	    Ammonia[i * GridColumns + j] = BoundaryNH4Concentration[JulianDay-1];
      		    Nitrate[i * GridColumns + j] = BoundaryNO3Concentration[JulianDay-1];
         	    Nitrite[i * GridColumns + j] = BoundaryNO2Concentration[JulianDay-1];
      	    }
   	    }

   	    //Northern boundary
   	    for (j = 0; j < GridColumns; j++)
   	    {
   		    i = GridLines - 1;

            MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
         	                             i * GridColumns + j,
            	                          "Box depth",
               	                       ObjectCode);

      	    if (
      			(MyBoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
      		)
      	    {
      		    Ammonia[i * GridColumns + j] = BoundaryNH4Concentration[JulianDay-1];
      		    Nitrate[i * GridColumns + j] = BoundaryNO3Concentration[JulianDay-1];
         	    Nitrite[i * GridColumns + j] = BoundaryNO2Concentration[JulianDay-1];
      	    }
   	    }

   	    //Eastern boundary
   	    for (i = 0; i < GridLines; i++)
   	    {
   		    j = GridColumns - 1;
            MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,
         	                             i * GridColumns + j,
            	                          "Box depth",
               	                       ObjectCode);
      	    if (
      			(MyBoxDepth > 0.0) &&
            	(BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
      		)
      	    {
      		    Ammonia[i * GridColumns + j] = BoundaryNH4Concentration[JulianDay-1];
      		    Nitrate[i * GridColumns + j] = BoundaryNO3Concentration[JulianDay-1];
         	    Nitrite[i * GridColumns + j] = BoundaryNO2Concentration[JulianDay-1];
      	    }
   	    }
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (Ammonia[i]   < 0.0)   Ammonia[i] = 0.0;
			if (Nitrite[i]   < 0.0)   Nitrite[i] = 0.0;
			if (Nitrate[i]   < 0.0)   Nitrate[i] = 0.0;
		}
	}
}

void TSangoNutrients::Go()
{
	JulianDay = MyPEcoDynClass->GetJulianDay();
}


bool TSangoNutrients::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Ammonia") == 0)
    {
        Ammonia[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Nitrate") == 0)
    {
        Nitrate[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Nitrite") == 0)
    {
        Nitrite[BoxNumber] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary NH4 concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryNH4Concentration[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary NO3 concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryNO3Concentration[j] = Value * CUBIC;
    }
	else if (strcmp(VariableName, "Boundary NO2 concentration") == 0)
    {
        for (int j = 0; j < NumberOfDaysForBoundaryTimeSeries; j++)
            BoundaryNO2Concentration[j] = Value * CUBIC;
    }
    else
        rc = false;

    return rc;
}


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

// TNutrientsBasic Class
//
// Constructors invoked outside EcoDyn shell...
//

TNutrientsBasic::TNutrientsBasic(char* className,
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
        double riverDOC, double oceanDOC, double aDOC[], double aDOCLoad[])
    :TNutrients(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
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
		Phosphate[j] = aPhosphate[j];
		PO4Load[j] = aPO4Load[j];
		Silica[j] = aSilica[j];
		SiO2Load[j] = aSiO2Load[j];
		DON[j] = aDON[j];
		DONLoad[j] = aDONLoad[j];
		DOC[j] = aDOC[j];
		DOCLoad[j] = aDOCLoad[j];
	}
	RiverAmmonia = riverAmmonia * CUBIC;  // CUBIC factor in river and ocean
	OceanAmmonia = oceanAmmonia * CUBIC;  // in pascal were in integrate
	RiverNitrite = riverNitrite * CUBIC;
	OceanNitrite = oceanNitrite * CUBIC;
	RiverNitrate = riverNitrate * CUBIC;
	OceanNitrate = oceanNitrate * CUBIC;
	RiverPhosphate = riverPhosphate * CUBIC;
	OceanPhosphate = oceanPhosphate * CUBIC;
	RiverSilica = riverSilica * CUBIC;
	OceanSilica = oceanSilica * CUBIC;
	RiverDON = riverDON * CUBIC;
	OceanDON = oceanDON * CUBIC;
	RiverDOC = riverDOC * CUBIC;
	OceanDOC = oceanDOC * CUBIC;

    PosBuildNutrientsBasic();
}

TNutrientsBasic::TNutrientsBasic(char* className, float timeStep, char* morphologyFilename,
                char* variablesFilename)
                        :TNutrients(className, timeStep, morphologyFilename)
{
    strcpy(VarsFileName, variablesFilename);
    BuildNutrientsBasic();
}
// ...


TNutrientsBasic::TNutrientsBasic(TEcoDynClass* APEcoDynClass, char* className)
							  : TNutrients(APEcoDynClass, className)
{
    BuildNutrientsBasic();
}

void TNutrientsBasic::BuildNutrientsBasic()
{
	int i,j;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Nutrients");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Nutrients variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TNutrientsBasic::BuildNutrientsBasic - Nutrients variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        int X,Y, XV, YV, XL, YL;
        if (PReadWrite->FindString("Nutrients", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names

            char MyVariable[65];
            for (i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);
                strcpy(MyVariable, VariableNameArray[i-Y]);
                if (strcmp(MyVariable, "Ammonia") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverAmmonia = GenericRiver * CUBIC;  // CUBIC factor in river and ocean
                    OceanAmmonia = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("Ammonia values", XV, YV))
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Ammonia[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, NH4Load[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("NH4 loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Ammonia[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, NH4Load[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Nitrite") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverNitrite = GenericRiver * CUBIC;  // CUBIC factor in River and ocean
                    OceanNitrite = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("Nitrite values", XV, YV))
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Nitrite[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, NO2Load[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("NO2 loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrite[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, NO2Load[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Nitrate") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverNitrate = GenericRiver * CUBIC;  // CUBIC factor in River and ocean
                    OceanNitrate = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("Nitrate values", XV, YV))
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Nitrate[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, NO3Load[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("NO3 loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Nitrate[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, NO3Load[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Phosphate") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverPhosphate = GenericRiver * CUBIC;  // CUBIC factor in River and ocean
                    OceanPhosphate = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("Phosphate values", XV, YV))
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Phosphate[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, PO4Load[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("PO4 loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Phosphate[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, PO4Load[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "Silica") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverSilica = GenericRiver * CUBIC;  // CUBIC factor in River and ocean
                    OceanSilica = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("Silica values", XV, YV))
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, Silica[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, SiO2Load[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("SiO2 loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, Silica[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, SiO2Load[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "DON") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverDON = GenericRiver * CUBIC;  // CUBIC factor in River and ocean
                    OceanDON = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("DON values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, DON[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, DONLoad[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("DON loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, DON[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, DONLoad[j]);
                        }
                    }
                }
                else if (strcmp(MyVariable, "DOC") == 0)
                {
                    // Read the end-member values
                    PReadWrite->ReadNumber(X+3, i, GenericRiver);
                    PReadWrite->ReadNumber(X+4, i, GenericOcean);
                    RiverDOC = GenericRiver * CUBIC;  // CUBIC factor in River and ocean
                    OceanDOC = GenericOcean * CUBIC;  // in pascal were in integrate
                    if (!PReadWrite->FindString("DOC values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+5+j, i, DOC[j]);
                            PReadWrite->ReadNumber(X + 5 + NumberOfBoxes + j, i, DOCLoad[j]);
                        }
                    }
                    else
                    {
                        PReadWrite->FindString("DOC loads", XL, YL);
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, DOC[j]);
                            if (XL > -1)
                                PReadWrite->ReadNumber(XL, YL + 1 + j, DOCLoad[j]);
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables - Undefined object in nutrients", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TNutrientsBasic::BuildNutrientsBasic - Variables: undefined object in nutrients" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    PosBuildNutrientsBasic();
}

void TNutrientsBasic::PosBuildNutrientsBasic()
{
	// Fluxes and conversions
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        Ammonia[i]   = Ammonia[i] * CUBIC; // umol l-1 * CUBIC = umol m-3
        Nitrite[i]   = Nitrite[i] * CUBIC;
        Nitrate[i]   = Nitrate[i] * CUBIC;
        Phosphate[i] = Phosphate[i] * CUBIC;
        Silica[i]    = Silica[i] * CUBIC;
        DON[i]       = DON[i] * CUBIC;
        DOC[i]       = DOC[i] * CUBIC;

        NH4Load[i]   = NH4Load[i] * CUBIC;  // Load in umol m-3: checked on 96.03.31
        NO2Load[i]   = NO2Load[i] * CUBIC;
        NO3Load[i]   = NO3Load[i] * CUBIC;
        PO4Load[i]   = PO4Load[i] * CUBIC;
        SiO2Load[i]  = SiO2Load[i] * CUBIC;
        DONLoad[i]   = DONLoad[i] * CUBIC;
        DOCLoad[i]   = DOCLoad[i] * CUBIC;

        NH4Flux[i]   = 0;
        NO2Flux[i]   = 0;
        NO3Flux[i]   = 0;
        PO4Flux[i]   = 0;
        SiO2Flux[i]  = 0;
        DONFlux[i]   = 0;
        DOCFlux[i]   = 0;
    }
	BoxArray = MyPEcoDynClass->BoxArray;
	VariableCount = 0;
}



void TNutrientsBasic::Go()
{
	// Active methods
	Nitrification();
}

void TNutrientsBasic::Inquiry(char* srcName, double &Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Ammonia") == 0)
		Value = Ammonia[MyBoxNumber] /CUBIC;
	else if (strcmp(MyParameter, "Nitrite") == 0)
		Value = Nitrite[MyBoxNumber] /CUBIC;
	else if (strcmp(MyParameter, "Nitrate") == 0)
		Value = Nitrate[MyBoxNumber] /CUBIC;
	else if (strcmp(MyParameter, "Phosphate") == 0)
		Value = Phosphate[MyBoxNumber] /CUBIC;
	else if (strcmp(MyParameter, "Silica") == 0)
		Value = Silica[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "DIN") == 0)
		Value = ( Ammonia[MyBoxNumber]
					 + Nitrite[MyBoxNumber]
					 + Nitrate[MyBoxNumber]
				  ) / CUBIC;
	else if (strcmp(MyParameter, "Net DIN Flux") == 0)
		Value = 0;
	else if (strcmp(MyParameter, "Redfield ratio (N/P)") == 0)
		Value = ( Ammonia[MyBoxNumber]
					 + Nitrite[MyBoxNumber]
					 + Nitrate[MyBoxNumber]
				  ) / Phosphate[MyBoxNumber];

    else if (strcmp(MyParameter, "DON") == 0)
		Value = DON[MyBoxNumber] /CUBIC;
    else if (strcmp(MyParameter, "DOC") == 0)
		Value = DOC[MyBoxNumber] /CUBIC;
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
                      "EcoDynamo Alert - Inquiry 3",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TNutrientsBasic::Inquiry 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TNutrientsBasic::Inquiry(char* srcName, double &Value,
									  int BoxX,
									  int BoxY,
									  char* ParameterName,
									  int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxX * NumberOfBoxes + BoxY;
	strcpy(MyParameter, ParameterName);

	// All exchanges in umol m-3, converted to kg mass flux
	if (strcmp(MyParameter, "DIN advective flux matrix") == 0)
	{
		Value = DINInterfaceExchange[MyBoxNumber].AdvectiveFlow // in kg
									  * TimeStep
									  * DAYSTOSECONDS
									  * NITROGENATOMICWEIGHT
									  / (CUBIC * CUBIC * CUBIC);
	}
	else if (strcmp(MyParameter, "DIN dispersive flux matrix") == 0)
	{
		Value = DINInterfaceExchange[MyBoxNumber].DispersiveFlow // in kg
									  * TimeStep
									  * DAYSTOSECONDS
									  * NITROGENATOMICWEIGHT
									  / (CUBIC * CUBIC * CUBIC);
	}
    else if (strcmp(MyParameter, "DIN net flux matrix") == 0)
    {
		Value = DINInterfaceExchange[MyBoxNumber].InFlow // made into net flow in kg
									  * TimeStep
									  * DAYSTOSECONDS
									  * NITROGENATOMICWEIGHT
									  / (CUBIC * CUBIC * CUBIC);
	}
    else {
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
			cerr << "TNutrientsBasic::Inquiry 4 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        Value = 0;
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


void TNutrientsBasic::Update(char* srcName, double Value,
								 int BoxNumber,
								 char* ParameterName,
								 int AnObjectCode)
{

	// Receives value as umol per m3

	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

	if (strcmp(MyParameter, "Ammonia") == 0)
		NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] + Value;
	else if (strcmp(MyParameter, "Nitrite") == 0)
		NO2Flux[MyBoxNumber] = NO2Flux[MyBoxNumber] + Value;
	else if (strcmp(MyParameter, "Nitrate") == 0)
		NO3Flux[MyBoxNumber] = NO3Flux[MyBoxNumber] + Value;
	else if (strcmp(MyParameter, "Phosphate") == 0)
		PO4Flux[MyBoxNumber] = PO4Flux[MyBoxNumber] + Value;
	else if (strcmp(MyParameter, "Silica") == 0)
		SiO2Flux[MyBoxNumber] = SiO2Flux[MyBoxNumber] + Value;
	else if (strcmp(MyParameter, "DIN") == 0)
	{
		DINProportion(MyBoxNumber);
		NH4Flux[MyBoxNumber] = NH4Flux[MyBoxNumber] + Value * NH4_TO_DIN;
		NO2Flux[MyBoxNumber] = NO2Flux[MyBoxNumber] + Value * NO2_TO_DIN;
		NO3Flux[MyBoxNumber] = NO3Flux[MyBoxNumber] + Value * NO3_TO_DIN;
	}
    else if (strcmp(MyParameter, "DON") == 0)
		DONFlux[MyBoxNumber] = DONFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "DOC") == 0)
		DOCFlux[MyBoxNumber] = DOCFlux[MyBoxNumber] + Value;
    else {
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
			cerr << "TNutrientsBasic::Update 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
}


bool TNutrientsBasic::SetVariableValue(char* srcName, double Value,
								 int BoxNumber,
								 char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Ammonia") == 0)
		Ammonia[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Nitrite") == 0)
		Nitrite[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Nitrate") == 0)
		Nitrate[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Phosphate") == 0)
		Phosphate[BoxNumber] = Value * CUBIC;
	else if (strcmp(VariableName, "Silica") == 0)
		Silica[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DON") == 0)
		DON[BoxNumber] = Value * CUBIC;
    else if (strcmp(VariableName, "DOC") == 0)
		DOC[BoxNumber] = Value * CUBIC;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.10
bool TNutrientsBasic::SaveVariables()
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
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0) {
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Ammonia") == 0)
        {
            PReadWrite->WriteCell(RiverAmmonia / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanAmmonia / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Nitrite") == 0)
        {
            PReadWrite->WriteCell(RiverNitrite / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanNitrite / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Nitrate") == 0)
        {
            PReadWrite->WriteCell(RiverNitrate / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanNitrate / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Phosphate") == 0)
        {
            PReadWrite->WriteCell(RiverPhosphate / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanPhosphate / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "Silica") == 0)
        {
            PReadWrite->WriteCell(RiverSilica / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanSilica / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "DON") == 0)
        {
            PReadWrite->WriteCell(RiverDON / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanDON / CUBIC, 8);
        }
        else if (strcmp(VariableNameArray[i], "DOC") == 0)
        {
            PReadWrite->WriteCell(RiverDOC / CUBIC, 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanDOC / CUBIC, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' initial values
     */

    PReadWrite->WriteCell("Ammonia values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NH4 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrite values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO2 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Nitrate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("NO3 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phosphate values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PO4 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Silica values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("SiO2 loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DON values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DON loads");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOC values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("DOC loads");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(Ammonia[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NH4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrite[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Nitrate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NO3Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Phosphate[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PO4Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(Silica[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(SiO2Load[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DON[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DONLoad[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DOC[j] / CUBIC, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(DOCLoad[j] / CUBIC, 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TNutrientsBasic::Integrate()
{
   int i,j;
	// Integrate ammonia within the box
/*
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

	// Integrate phosphate within the box
	Generic = Phosphate; GenericFlux = PO4Flux;
	Integration();
	Phosphate = Generic; PO4Flux = GenericFlux;

	// Integrate silica within the box
	Generic = Silica; GenericFlux = SiO2Flux;
	Integration();
	Silica = Generic; SiO2Flux = GenericFlux;
*/
    Integration(Ammonia, NH4Flux,
                Nitrite, NO2Flux,
                Nitrate, NO3Flux,
                Phosphate, PO4Flux,
                Silica, SiO2Flux);

    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    if (NO3Load[i] < 0.0) NO3Load[i] = Nitrate[i];
        if (NO2Load[i] < 0.0) NO2Load[i] = Nitrite[i];
        if (NH4Load[i] < 0.0) NH4Load[i] = Ammonia[i];
        if (PO4Load[i] < 0.0) PO4Load[i] = Phosphate[i];
        if (SiO2Load[i] < 0.0) SiO2Load[i] = Silica[i];
    }
	// Transport the nutrients

	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{
		// New calls for boundary exchange

		MyTransportPointer->Go(Ammonia, NH4Load, RiverAmmonia, OceanAmmonia);

		MyTransportPointer->Go(Nitrite, NO2Load, RiverNitrite, OceanNitrite);

		MyTransportPointer->Go(Nitrate, NO3Load, RiverNitrate, OceanNitrate);

		MyTransportPointer->Go(Phosphate, PO4Load, RiverPhosphate, OceanPhosphate);
		MyTransportPointer->Go(Silica, SiO2Load, RiverSilica, OceanSilica);
	}

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (Ammonia[i]   < 0.0)   Ammonia[i] = 0.0;
		if (Nitrite[i]   < 0.0)   Nitrite[i] = 0.0;
		if (Nitrate[i]   < 0.0)   Nitrate[i] = 0.0;
		if (Phosphate[i] < 0.0) Phosphate[i] = 0.0;
		if (Silica[i]    < 0.0)    Silica[i] = 0.0;
	}
}

// Protected methods
void TNutrientsBasic::RedfieldRatio()
{
}

void TNutrientsBasic::DINProportion(int ABoxNumber)
{
	double TotalDIN;
	TotalDIN = ( Ammonia[ABoxNumber]
					 + Nitrite[ABoxNumber]
					 + Nitrate[ABoxNumber] );

	if (TotalDIN > 0)
	{
		NH4_TO_DIN = Ammonia[ABoxNumber] / TotalDIN;
		NO2_TO_DIN = Nitrite[ABoxNumber] / TotalDIN;
		NO3_TO_DIN = Nitrate[ABoxNumber] / TotalDIN;
	}
	else
	{
		NH4_TO_DIN = 0;
		NO2_TO_DIN = 0;
		NO3_TO_DIN = 0;
	}
}

void TNutrientsBasic::Nitrification()
{
	// Fixed terms for nitrification
	const double NH4_TO_NO2 = 0.1;
	const double NO2_TO_NO3 = 0.1;

	double TempNH4, TempNO2, TempNO3;

	// Is this finally correct? I think so - 97.07.23 - JGF
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		TempNH4 = Ammonia[i] * -NH4_TO_NO2;
		TempNO2 = Ammonia[i] *  NH4_TO_NO2 + Nitrite[i] *  -NO2_TO_NO3;
		TempNO3 = Nitrite[i] *  NO2_TO_NO3;

      //if (i == 1) Debugger(TempNO2);

		NH4Flux[i] = NH4Flux[i]+ TempNH4;
		NO2Flux[i] = NO2Flux[i]+ TempNO2;
		NO3Flux[i] = NO3Flux[i]+ TempNO3;
	}
}





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
#include "iodll.h"

TZosteraMarina::TZosteraMarina(TEcoDynClass* APEcoDynClass, char* className)
							 : TMacrophytes(APEcoDynClass, className)
{
   NumberOfBoxesWithZosteraMarina = 0;
   for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
   {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Zostera marina") == 0)
                        NumberOfBoxesWithZosteraMarina++;

   }
   ZosteraMarinaRecord = new BenthicSpeciesRecord[NumberOfBoxesWithZosteraMarina];
   int AIndex = 0;
   for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
   {
        if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Zostera marina") == 0)
        {
                strcpy(ZosteraMarinaRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
                ZosteraMarinaRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
                ZosteraMarinaRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
                AIndex++;
        }
   }
   OxygenNetProduction = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++)
   	OxygenNetProduction[i] = 0;

	int X, Y, Index, XV, YV;

	VariableCount = 0;
	// Read in the variables
//   	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("ZosteraMarina");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "ZosteraMarina variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TZosteraMarina::TZosteraMarina - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Zostera marina", X, Y))
	    {
   	        PReadWrite->ReadNumber(X + NUMBEROFVARSCOLUMN, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // Create array for state variable units
            //UnitNameArray = new UNA[NumberOfVariables];
            // read in the variable names
            char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);
            
                if (strcmp(MyVariable, "Z. marina biomass") == 0)
                {
                    if (PReadWrite->FindString("Z. marina biomass values", XV, YV))
                    {
                        for (int j = 0; j < /*NumberOfBoxes*/NumberOfBoxesWithZosteraMarina; j++)
                        {
                            Index = ZosteraMarinaRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraMarinaRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[Index]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < /*NumberOfBoxes*/NumberOfBoxesWithZosteraMarina; j++)
                        {
                            Index = ZosteraMarinaRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraMarinaRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(X + FIRSTBOXDATACOLUMN + j, i, PhytoBiomass[Index]);
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - macrophytes", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TZosteraMarina::TZosteraMarina - Variables undefined object." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	//Read in the parameters
	ParameterCount = 0;
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("ZosteraMarina");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "ZosteraMarina parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TZosteraMarina::TZosteraMarina - Parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        double MyValue;
        if (PReadWrite->FindString("Zostera marina", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Pmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Pmax[j] = MyValue; //mg / h / g
                }
                else
                if (strcmp(MyParameter, "Maintenance respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        MaintenanceRespiration[j] = MyValue; //mg / h / g
                }
                else
                if (strcmp(MyParameter, "Death loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, DeathLoss);
                                    //Debugger(DeathLoss);
                }
                else
                if (strcmp(MyParameter, "I05") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Ks[j] = MyValue;
                }
                else
                if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenProd);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
							cerr << "TZosteraMarina::TZosteraMarina - Parameters undefined object." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TZosteraMarina::~TZosteraMarina()
{
    freeMemory();
}

void TZosteraMarina::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
            delete [] OxygenNetProduction;
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
				cerr << "TZosteraMarina::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TZosteraMarina::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Pmax") == 0)
    {
        value = Pmax[0];
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration[0];
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "I05") == 0)
    {
        value = Ks[0];
    }
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
    {
        value = CarbonToOxygenProd;
    }
    else
        value = 0.0;
    return value;
}

bool TZosteraMarina::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "Pmax") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Pmax[j] = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            MaintenanceRespiration[j] = value;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "I05") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Ks[j] = value;
    }
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
    {
        CarbonToOxygenProd = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TZosteraMarina::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("ZosteraMarina");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Zostera marina");
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
     * zostera marina biomass initial values
     */
    PReadWrite->WriteCell("Z. marina biomass values");
    PReadWrite->WriteSeparator(true);

    int Index;
    for (int j = 0; j < NumberOfBoxesWithZosteraMarina; j++)
    {
        Index = ZosteraMarinaRecord[j].LineCoordinate * NumberOfColumns +
                ZosteraMarinaRecord[j].ColumnCoordinate;
        PReadWrite->WriteCell(PhytoBiomass[Index], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TZosteraMarina::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("ZosteraMarina");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Zostera marina");
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

        if (strcmp(ParametersNameArray[i], "Pmax") == 0)
        {
            PReadWrite->WriteCell(Pmax[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "I05") == 0)
        {
            PReadWrite->WriteCell(Ks[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in photosynthesis") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenProd, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TZosteraMarina::Go()
{
        int index, i, j;
	MyTime = MyPEcoDynClass->GetTime();
	CurrentTime = MyPEcoDynClass->GetCurrTime();
        TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
        for (int m = 0; m < NumberOfBoxesWithZosteraMarina; m++)
        {
                //Debugger(m);
                i = ZosteraMarinaRecord[m].LineCoordinate;
                j = ZosteraMarinaRecord[m].ColumnCoordinate;
                //Debugger(i); Debugger(j);
                if (
                        (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                        (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
                )
                {
                        index = i * NumberOfColumns + j;
	                Production(index);
                        Respiration(index);
	                if (MyNutrientPointer != NULL)
                        {
                                MyNutrientPointer->Update(GetEcoDynClassName(), OxygenNetProduction[index] /
         									CUBIC, //converter from mg / s / m3 to mg / s / l
                                                                                index,
                                                                                "Oxygen",
                                                                                ObjectCode);
      	                        OxygenNetProduction[index] = 0;
   	                }
                }
	        Mortality(index);
                NPP[index] = PhytoProd[index];
		PhytoFlux[index] = PhytoFlux[index] + PhytoProd[index];
        }
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
	}
}


void TZosteraMarina::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber, JulianDay;
	strcpy(MyParameter, ParameterName);

   	if (strcmp(MyParameter, "Z. marina biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "Z. marina dead biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Z. marina total GPP") == 0)
		Value = GPP[MyBoxNumber];
	else if (strcmp(MyParameter, "Z. marina total NPP") == 0)
		Value = NPP[MyBoxNumber];
	else if (strcmp(MyParameter, "Z. marina GPP per unit mass") == 0)
		Value = GPP[MyBoxNumber]
				  / PhytoBiomass[MyBoxNumber]
				  * HOURSTODAYS;
	else if (strcmp(MyParameter, "Z. marina NPP per unit mass") == 0)
		Value = NPP[MyBoxNumber]
				  / PhytoBiomass[MyBoxNumber]
				  * HOURSTODAYS;
	else if (strcmp(MyParameter, "Z. marina mortality") == 0)
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
	        MessageBox(0,Caption,"EcoDynamo Alert - Inquiry 8",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
							cerr << "TZosteraMarina::Inquiry 8 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TZosteraMarina::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
        char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
        LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

	if (strcmp(MyParameter, "Z. marina biomass") == 0)
		PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Value;
        else
        {
#ifdef __BORLANDC__
    	        char Caption[129];
    	        strcpy(Caption, MyParameter);
                strcat(Caption, " does not exist in ");
                strcat(Caption, GetEcoDynClassName());
    	        MessageBox(0,Caption,"EcoDynamo Alert - Update 3",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
							cerr << "TZosteraMarina::Update 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
}


bool TZosteraMarina::SetVariableValue(char* srcName, double Value,
            int BoxNumber,char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Z. marina biomass") == 0)
		PhytoBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Z. marina total GPP") == 0)
		GPP[BoxNumber] = Value;
	else if (strcmp(VariableName, "Z. marina total NPP") == 0)
		NPP[BoxNumber] = Value;
	else if (strcmp(VariableName, "Z. marina mortality") == 0)
        PhytoMortality[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TZosteraMarina::Integrate()
{
}

void TZosteraMarina::Production(int ABoxNumber)
{
	double MyDepth, LightAtTop, LightAtBottom, MyWaterTemperature;
        int i = ABoxNumber;
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
                * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
                * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

	//for (int i = 0; i < NumberOfBoxes; i++)
	//{


	        if (MyLightPointer != NULL)
	        {
                        if (MyTransportPointer != NULL)
      	                        MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,i,
                                                "Box depth",
                                                ObjectCode);
                        else
                                MyDepth = MyPEcoDynClass->GetBoxDepth(i);

		        MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
                                                i,
                                                "PAR surface irradiance",
                                                ObjectCode);
                        if (LightAtTop == 0)
                                Productivity = 0.0;
                        else
                        {
         	                if (MySPMPointer != NULL)
				        MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
							i,
                                                        "Extinction coefficient Ktot",
                                                        ObjectCode);
		   	        MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
                                                MyDepth,
                                                i,
                                                "Sub-surface PAR irradiance",
                                                ObjectCode);
                                if (MyWaterTemperaturePointer != NULL)
                                {
            	                        MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
                                                                        i,
                                                                        "Water temperature",
                                                                        ObjectCode);
                                        if (MyWaterTemperature <= 22)
               	                                Pmax[i] = 12 + 0.21*(MyWaterTemperature-22);
                                        else
               	                                Pmax[i] = 12 - 0.24*(MyWaterTemperature-22);
                                        if (Pmax[i] < 0)
               	                                Pmax[i] = 0;
                                        //Debugger(Pmax[i]);
                                }
                                Productivity = PhytoBiomass[i]* Pmax[i]* ( LightAtBottom * WATTSTOMICROEINSTEINS)
                        	                /( LightAtBottom * WATTSTOMICROEINSTEINS + Ks[i] )
								   * DAYSTOHOURS / DAYSTOSECONDS;
                        }
                        //Debugger(GPP[i]);
                        GPP[i] = Productivity /* CarbonToOxygenProd*/;
                        PhytoProd[i] = Productivity /* CarbonToOxygenProd*/;
                        OxygenNetProduction[i] = Productivity /* CarbonToOxygenProd *// MyDepth;//mg O2 / s / m3
	        }
        //}
}


void TZosteraMarina::Respiration(int ABoxNumber)
{
        int i = ABoxNumber;
	double Resp, MyTidalHeight;
        TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	//for (int i = 0; i < NumberOfBoxes; i++)
	//{
   	        if (MyTransportPointer != NULL)
                        MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,i,
                                                "Box depth",
                                                ObjectCode);
                else
                        MyDepth = MyPEcoDynClass->GetBoxDepth(i);

                Resp = MaintenanceRespiration[i] * PhytoBiomass[i] * DAYSTOHOURS / DAYSTOSECONDS;
                PhytoProd[i] = PhytoProd[i] - ( Resp );
                OxygenNetProduction[i] = OxygenNetProduction[i] -
                               Resp / MyDepth;  //mg O2 / s / m3
	//}
}

void TZosteraMarina::Mortality(int ABoxNumber)
{
}

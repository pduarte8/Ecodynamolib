// Macrophyte object CPP code file
// @ Mestrado da Dalila Serpa, traduzido em Julho de 2005 por Pedro

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

TUlvaSp::TUlvaSp(TEcoDynClass* APEcoDynClass, char* className): TRuppiaCirrhosa(APEcoDynClass, className)
{
        NumberOfBoxesWithUlvaSp = 0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
            if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Ulva sp") == 0)
                NumberOfBoxesWithUlvaSp++;
        }
        UlvaSpRecord = new BenthicSpeciesRecord[NumberOfBoxesWithUlvaSp];
        int AIndex = 0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
            if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Ulva sp") == 0)
            {
                strcpy(UlvaSpRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
                UlvaSpRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
                UlvaSpRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
                AIndex++;
            }
        }
        CCellQuota = new double[NumberOfBoxes]; NCellQuota = new double[NumberOfBoxes];
        PCellQuota = new double[NumberOfBoxes];
        NUptake = new double[NumberOfBoxes]; PUptake = new double[NumberOfBoxes];
        CPhyto = new double[NumberOfBoxes]; NPhyto = new double[NumberOfBoxes];
        PPhyto = new double[NumberOfBoxes];
        NCellFlux = new double[NumberOfBoxes]; PCellFlux = new double[NumberOfBoxes];
        MaxPCellQuota = 0.0; MaxNCellQuota = 0.0; MinPCellQuota = 0.0; MinNCellQuota = 0.0;
        OxygenToCarbonProd = 1.2; OxygenToCarbonResp = 1.0;
        VNmax = 0.0; VPmax = 0.0; KN = 0.0; KP = 0.0;
        for (int i = 0; i < NumberOfBoxes; i++)
        {
           CCellQuota[i] = 0.0; NCellQuota[i] = 0.0; PCellQuota[i] = 0.0;
           NUptake[i] = 0.0; PUptake[i] = 0.0;
           CPhyto[i] = 0.0; NPhyto[i] = 0.0; PPhyto[i] = 0.0;
           NCellFlux[i] = 0.0; PCellFlux[i] = 0.0;
        }

    int X,Y, XV, YV;
    VariableCount = 0;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("UlvaSp");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "UlvaSp variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TUlvaSp::TUlvaSp - Variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("UlvaSp", X, Y))
        {
            PReadWrite->ReadNumber(X + NUMBEROFVARSCOLUMN, Y, NumberOfVariables);

            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // Create array for state variable units
            //UnitNameArray = new UNA[NumberOfVariables];
            // read in the variable names
            // for (int i = Y; i < Y + NumberOfVariables; i++)
    /*        for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN + i, Y, VariableNameArray[i]);
                //PReadWrite->ReadString(X + UNITSCOLUMN + i, Y, UnitNameArray[i]);
            }*/
            char MyVariable[65];
            int Index;
            //for (int i = Y; i < Y + NumberOfVariables; i++)
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);

                if (strcmp(MyVariable, "UlvaSpBiomass") == 0)
                {
                    if (!PReadWrite->FindString("UlvaSpBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithUlvaSp; j++)
                    {
                        Index = UlvaSpRecord[j].LineCoordinate * NumberOfColumns +
                                UlvaSpRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(XV, j + YV + 1, PhytoBiomass[Index]);
                        //if (j == NumberOfBoxesWithUlvaSp - 1) Debugger(PhytoBiomass[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "UlvaSpCBiomass") == 0)
                {
                    if (!PReadWrite->FindString("UlvaSpCBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithUlvaSp; j++)
                    {
                        Index = UlvaSpRecord[j].LineCoordinate * NumberOfColumns +
                                UlvaSpRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(XV, j + YV + 1, CPhyto[Index]);
                        //if (j == NumberOfBoxesWithUlvaSp - 1) Debugger(CPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "UlvaSpNBiomass") == 0)
                {
                    if (!PReadWrite->FindString("UlvaSpNBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithUlvaSp; j++)
                    {
                        Index = UlvaSpRecord[j].LineCoordinate * NumberOfColumns +
                                UlvaSpRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(XV, j + YV + 1, NPhyto[Index]);
                        //if (j == NumberOfBoxesWithUlvaSp - 1) Debugger(NPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "UlvaSpPBiomass") == 0)
                {
                    if (!PReadWrite->FindString("UlvaSpPBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithUlvaSp; j++)
                    {
                        Index = UlvaSpRecord[j].LineCoordinate * NumberOfColumns +
                                UlvaSpRecord[j].ColumnCoordinate;
                        PReadWrite->ReadNumber(XV, j + YV + 1, PPhyto[Index]);
                        //if (j == NumberOfBoxesWithUlvaSp - 1) Debugger(PPhyto[Index]);
                    }
                }
            //More variables here
            }
            for (int j = 0; j < NumberOfBoxesWithUlvaSp; j++)
            {
               Index = UlvaSpRecord[j].LineCoordinate * NumberOfColumns +
                       UlvaSpRecord[j].ColumnCoordinate;
               if (PhytoBiomass[Index] != 0)
               {
                   CCellQuota[Index] = CPhyto[Index] / PhytoBiomass[Index];   //g / g
                   NCellQuota[Index] = NPhyto[Index] / PhytoBiomass[Index];
                   PCellQuota[Index] = PPhyto[Index] / PhytoBiomass[Index];
               }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object variables - UlvaSp", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TUlvaSp::TUlvaSp - variables: undefined object UlvaSp" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	ParameterCount = 0;
	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("UlvaSp");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "UlvaSp parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TUlvaSp::TUlvaSp - Parameters file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        double MyValue;

        if (PReadWrite->FindString("UlvaSp", X, Y))
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
                        Pmax[j] = MyValue;
                }
                else
                if (strcmp(MyParameter, "Iopt") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Iopt[j] = MyValue;
                }
                else
                if (strcmp(MyParameter, "Respiration Coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                                    //Debugger(MyValue);
                                    RespirationCoefficient = MyValue;
                }
                else
                if (strcmp(MyParameter, "Death loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                                    //Debugger(MyValue);
                                    DeathLoss = MyValue;
                }
                else
                if (strcmp(MyParameter, "Oxygen to Carbon in photosynthesis") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OxygenToCarbonProd);
                        //Debugger(OxygenToCarbonProd);
                }
                else
                if (strcmp(MyParameter, "Oxygen to Carbon in respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, OxygenToCarbonResp);
                        //Debugger(OxygenToCarbonResp);
                }
                else
                if (strcmp(MyParameter, "Carbon to dry weight") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, CarbonToDryWeight);
                        //Debugger(CarbonToDryWeight);
                }
                else
                if (strcmp(MyParameter, "MaxNCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxNCellQuota);
                        //Debugger(MaxNCellQuota);
                }
                else
                if (strcmp(MyParameter, "MinNCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinNCellQuota);
                        //Debugger(MinNCellQuota);
                }
                else
                if (strcmp(MyParameter, "MaxPCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxPCellQuota);
                        //Debugger(MaxPCellQuota);
                }
                else
                if (strcmp(MyParameter, "MinPCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinPCellQuota);
                        //Debugger(MinPCellQuota);
                }
                else
                if (strcmp(MyParameter, "VNmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, VNmax);
                        //Debugger(VNmax);
                }
                else
                if (strcmp(MyParameter, "VPmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, VPmax);
                        //Debugger(VPmax);
                }
                else
                if (strcmp(MyParameter, "KN") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KN);
                        //Debugger(KN);
                }
                else
                if (strcmp(MyParameter, "KP") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KP);
                        //Debugger(KP);
                }
                else
                if (strcmp(MyParameter, "AReferenceTemperature") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, AReferenceTemperature);
                        //Debugger(AReferenceTemperature);
                }
                else
                if (strcmp(MyParameter, "ATemperatureCoefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ATemperatureCoefficient);
                        //Debugger(ATemperatureCoefficient);
                }
                else
                if (strcmp(MyParameter, "KT") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KT);
                        //Debugger(KT);
                }
                else
                if (strcmp(MyParameter, "beta") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, beta);
                        //Debugger(beta);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object parameters - UlvaSp", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TUlvaSp::TUlvaSp - parameters: undefined object UlvaSp" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TUlvaSp::~TUlvaSp()
{
    freeMemory();
}

void TUlvaSp::freeMemory()
{
    try {
       if (NumberOfBoxes > 0)
       {
          delete [] CCellQuota;
          delete [] NCellQuota;
          delete [] PCellQuota;
          delete [] NUptake;
          delete [] PUptake;
          delete [] CPhyto;
          delete [] NPhyto;
          delete [] PPhyto;
          delete [] NCellFlux;
          delete [] PCellFlux;
       }
       if (NumberOfBoxesWithUlvaSp > 0)
          delete [] UlvaSpRecord;
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
				cerr << "TUlvaSp::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TUlvaSp::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Pmax") == 0)
    {
        value = Pmax[0];
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        value = RespirationCoefficient;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "Carbon to dry weight") == 0)
    {
        value = CarbonToDryWeight;
    }
    else if (strcmp(MyParameter, "Iopt") == 0)
    {
        value = Iopt[0];
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon in photosynthesis") == 0)
    {
        value = OxygenToCarbonProd;
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon in respiration") == 0)
    {
        value = OxygenToCarbonResp;
    }
    else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
    {
        value = MaxNCellQuota;
    }
    else if (strcmp(MyParameter, "MinNCellQuota") == 0)
    {
        value = MinNCellQuota;
    }
    else if (strcmp(MyParameter, "MaxPCellQuota") == 0)
    {
        value = MaxPCellQuota;
    }
    else if (strcmp(MyParameter, "MinPCellQuota") == 0)
    {
        value = MinPCellQuota;
    }
    else if (strcmp(MyParameter, "VNmax") == 0)
    {
        value = VNmax;
    }
    else if (strcmp(MyParameter, "VPmax") == 0)
    {
        value = VPmax;
    }
    else if (strcmp(MyParameter, "KN") == 0)
    {
        value = KN;
    }
    else if (strcmp(MyParameter, "KP") == 0)
    {
        value = KP;
    }
    else if (strcmp(MyParameter, "AReferenceTemperature") == 0)
    {
        value = AReferenceTemperature;
    }
    else if (strcmp(MyParameter, "ATemperatureCoefficient") == 0)
    {
        value = ATemperatureCoefficient;
    }
    else if (strcmp(MyParameter, "KT") == 0)
    {
        value = KT;
    }
    else if (strcmp(MyParameter, "beta") == 0)
    {
        value = beta;
    }
    else
        value = 0.0;
    return value;
}

bool TUlvaSp::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "Pmax") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Pmax[j] = value;
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        RespirationCoefficient = value;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "Carbon to dry weight") == 0)
    {
        CarbonToDryWeight = value;
    }
    else if (strcmp(MyParameter, "Iopt") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Iopt[j] = value;
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon in photosynthesis") == 0)
    {
        OxygenToCarbonProd = value;
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon in respiration") == 0)
    {
        OxygenToCarbonResp = value;
    }
    else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
    {
        MaxNCellQuota = value;
    }
    else if (strcmp(MyParameter, "MinNCellQuota") == 0)
    {
        MinNCellQuota = value;
    }
    else if (strcmp(MyParameter, "MaxPCellQuota") == 0)
    {
        MaxPCellQuota = value;
    }
    else if (strcmp(MyParameter, "MinPCellQuota") == 0)
    {
        MinPCellQuota = value;
    }
    else if (strcmp(MyParameter, "VNmax") == 0)
    {
        VNmax = value;
    }
    else if (strcmp(MyParameter, "VPmax") == 0)
    {
        VPmax = value;
    }
    else if (strcmp(MyParameter, "KN") == 0)
    {
        KN = value;
    }
    else if (strcmp(MyParameter, "KP") == 0)
    {
        KP = value;
    }
    else if (strcmp(MyParameter, "AReferenceTemperature") == 0)
    {
        AReferenceTemperature = value;
    }
    else if (strcmp(MyParameter, "ATemperatureCoefficient") == 0)
    {
        ATemperatureCoefficient = value;
    }
    else if (strcmp(MyParameter, "KT") == 0)
    {
        KT = value;
    }
    else if (strcmp(MyParameter, "beta") == 0)
    {
        beta = value;
    }
    else
        rc = false;

    return rc;
}

void TUlvaSp::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName, int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber, JulianDay;
	strcpy(MyParameter, ParameterName);

   	if (strcmp(MyParameter, "UlvaSpBiomass") == 0)
    {
        //Debugger(PhytoBiomass[MyBoxNumber]);
        Value = PhytoBiomass[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "UlvaSpCBiomass") == 0)
        Value = CPhyto[MyBoxNumber];
    else if (strcmp(MyParameter, "UlvaSpNBiomass") == 0)
        Value = NPhyto[MyBoxNumber];
    else if (strcmp(MyParameter, "UlvaSpPBiomass") == 0)
        Value = PPhyto[MyBoxNumber];
    else if (strcmp(MyParameter, "UlvaSp GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
            Value = GPP[MyBoxNumber]/* * CUBIC
                / PhytoBiomass[MyBoxNumber]*/ * HOURSTOSECONDS;  //mg C / g /h
    }
	else if (strcmp(MyParameter, "UlvaSp NPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
            Value = NPP[MyBoxNumber]/* * CUBIC
            / PhytoBiomass[MyBoxNumber]*/ * HOURSTOSECONDS;  //mg C / g /h
    }
    else if (strcmp(MyParameter, "UlvaSpNCellQuota") == 0)
        Value = NCellQuota[MyBoxNumber];
    else if (strcmp(MyParameter, "UlvaSpPCellQuota") == 0)
        Value = PCellQuota[MyBoxNumber];
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
                  "EcoDynamo Alert - Inquiry 7",
                  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TUlvaSp::Inquiry 7 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TUlvaSp::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									     char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

   	if (strcmp(VariableName, "UlvaSpBiomass") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "UlvaSpCBiomass") == 0)
        CPhyto[BoxNumber] = Value;
    else if (strcmp(VariableName, "UlvaSpNBiomass") == 0)
        NPhyto[BoxNumber] = Value;
    else if (strcmp(VariableName, "UlvaSpPBiomass") == 0)
        PPhyto[BoxNumber] = Value;
    else if (strcmp(VariableName, "UlvaSp GPP per unit mass") == 0)
        GPP[BoxNumber] = Value / HOURSTOSECONDS;
	else if (strcmp(VariableName, "UlvaSp NPP per unit mass") == 0)
        NPP[BoxNumber] = Value / HOURSTOSECONDS;
    else if (strcmp(VariableName, "UlvaSpNCellQuota") == 0)
        NCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "UlvaSpPCellQuota") == 0)
        PCellQuota[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TUlvaSp::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("UlvaSp");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("UlvaSp");
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
     * UlvaSp variables initial values
     */
    PReadWrite->WriteCell("UlvaSpBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("UlvaSpCBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("UlvaSpNBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("UlvaSpPBiomass values");
    PReadWrite->WriteSeparator(true);
    int Index;
    for (int j = 0; j < NumberOfBoxesWithUlvaSp; j++)
    {
        Index = UlvaSpRecord[j].LineCoordinate * NumberOfColumns +
                UlvaSpRecord[j].ColumnCoordinate;
        PReadWrite->WriteCell(PhytoBiomass[Index], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(CPhyto[Index], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NPhyto[Index], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PPhyto[Index], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TUlvaSp::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("UlvaSp");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("UlvaSp");
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
        else if (strcmp(ParametersNameArray[i], "Respiration Coefficient") == 0)
        {
            PReadWrite->WriteCell(RespirationCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to dry weight") == 0)
        {
            PReadWrite->WriteCell(CarbonToDryWeight, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Iopt") == 0)
        {
            PReadWrite->WriteCell(Iopt[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen to Carbon in photosynthesis") == 0)
        {
            PReadWrite->WriteCell(OxygenToCarbonProd, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen to Carbon in respiration") == 0)
        {
            PReadWrite->WriteCell(OxygenToCarbonResp, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxNCellQuota") == 0)
        {
            PReadWrite->WriteCell(MaxNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinNCellQuota") == 0)
        {
            PReadWrite->WriteCell(MinNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxPCellQuota") == 0)
        {
            PReadWrite->WriteCell(MaxPCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinPCellQuota") == 0)
        {
            PReadWrite->WriteCell(MinPCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "VNmax") == 0)
        {
            PReadWrite->WriteCell(VNmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "VPmax") == 0)
        {
            PReadWrite->WriteCell(VPmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KN") == 0)
        {
            PReadWrite->WriteCell(KN, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KP") == 0)
        {
            PReadWrite->WriteCell(KP, 8);
        }
        else if (strcmp(ParametersNameArray[i], "AReferenceTemperature") == 0)
        {
            PReadWrite->WriteCell(AReferenceTemperature, 8);
        }
        else if (strcmp(ParametersNameArray[i], "ATemperatureCoefficient") == 0)
        {
            PReadWrite->WriteCell(ATemperatureCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KT") == 0)
        {
            PReadWrite->WriteCell(KT, 8);
        }
        else if (strcmp(ParametersNameArray[i], "beta") == 0)
        {
            PReadWrite->WriteCell(beta, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TUlvaSp::Go()
{
        double MyJulianDay; int i,j, index;
	MyJulianDay = MyPEcoDynClass->GetJulianDay();
        TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
        //Debugger(NumberOfBoxesWithUlvaSp);
        //if ((MyJulianDay == 36) && (MyPEcoDynClass->GetCurrTime() > 5.25)) Debugger(1);
        for (int m = 0; m < NumberOfBoxesWithUlvaSp; m++)
        {
                i = UlvaSpRecord[m].LineCoordinate;
                j = UlvaSpRecord[m].ColumnCoordinate;
                if (
                        (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                        (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
                )
                {
                        index = i * NumberOfColumns + j;
                        if (MyTransportPointer != NULL)
                                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth, index,"Box depth",ObjectCode);
                        else
                                MyBoxDepth = MyPEcoDynClass->GetBoxDepth(index);
                        GPP[index] = 0;
                        NPP[index] = 0;
                        GrossOxygenProduction[index] = 0;
                        NetOxygenProduction[index] = 0;

                        //if (BoxArray[index].Dummybool2)
                        if ((MyBoxDepth > CriticalWaterDepth)  && (PhytoBiomass[index] > 0.0))
                        {
                                //Debugger(PhytoBiomass[index]);
	                        Production(index);
                                Respiration(index);
	                        Mortality(index);
                                NitrogenUptake(index);
                                PhosphorusUptake(index);
                        }
                }
        }
        //if ((MyJulianDay == 36) && (MyPEcoDynClass->GetCurrTime() > 5.25)) Debugger(2);
}

void TUlvaSp::Integrate()
{
    int i, j, index;
    double const TINNY = 0.000001;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
	/*Generic = CPhyto; GenericFlux = PhytoFlux;
	Integration();
	CPhyto = Generic; PhytoFlux = GenericFlux;
        //Debugger(CPhyto[0]);
        Generic = NPhyto; GenericFlux = NCellFlux;
	Integration();
	NPhyto = Generic; NCellFlux = GenericFlux;

        Generic = PPhyto; GenericFlux = PCellFlux;
	Integration();
	PPhyto = Generic; PCellFlux = GenericFlux;*/
        for (int m = 0; m < NumberOfBoxesWithUlvaSp; m++)
        {
                i = UlvaSpRecord[m].LineCoordinate;
                j = UlvaSpRecord[m].ColumnCoordinate;
                if (
                        (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                        (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
                )
                {
                    index = i * NumberOfColumns + j;
                    CPhyto[index] = CPhyto[index] + PhytoFlux[index] * MyPEcoDynClass->GetTimeStep();
                    PhytoFlux[index] = 0.0;
                    NPhyto[index] = NPhyto[index] + NCellFlux[index] * MyPEcoDynClass->GetTimeStep();
                    NCellFlux[index] = 0.0;
                    PPhyto[index] = PPhyto[index] + PCellFlux[index] * MyPEcoDynClass->GetTimeStep();
                    PCellFlux[index] = 0.0;
                    if (CPhyto[index] > TINNY)
                    {
                       PhytoBiomass[index] = CPhyto[index] / CCellQuota[index];
                       NCellQuota[index] = NPhyto[index] / PhytoBiomass[index];
                       PCellQuota[index] = PPhyto[index] / PhytoBiomass[index];
                    }
                    else
                    {
                       PhytoBiomass[index] = 0.0;
                       NCellQuota[index] = 0.0;
                       PCellQuota[index] = 0.0;
                       //CPhyto[index] = 0.0;
                    }
                }
         }
         //if ((MyPEcoDynClass->GetJulianDay() == 36) && (MyPEcoDynClass->GetCurrTime() > 5.25)) Debugger(5);
}

void TUlvaSp::Production(int ABoxNumber)
{
   double LightAtBottom, TotalLightAtSurface;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
        * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
        * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      if (MyLightPointer != NULL)
      {

         if (MySPMPointer != NULL)
            MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,MyBoxNumber,"Extinction coefficient Ktot", ObjectCode);
         else
            KValue = 1.0;

         MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtSurface,MyBoxNumber,"Total surface irradiance",ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom, TotalLightAtSurface, MyBoxDepth, KValue, MyBoxNumber,
                                 "Sub-surface PAR irradiance",ObjectCode);

         //MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom, MyBoxDepth, MyBoxNumber,"Sub-surface PAR irradiance",ObjectCode);
         if ((MyBoxDepth > 0.0) && (LightAtBottom > 0.0))
         {
            Productivity = Pmax[MyBoxNumber] * (LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber]) *
                           exp(1.0 - LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber]) *
                           NutrientLimitation(MyBoxNumber) / HOURSTOSECONDS;//mg C / g (dw) / s
            if (MyWaterTemperaturePointer != NULL)
            {
               MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature,MyBoxNumber,"Water temperature", ObjectCode);
               Productivity = Productivity * TemperatureLimitation(AWaterTemperature,
                                                                   AReferenceTemperature,
                                                                   ATemperatureCoefficient);
            }
         }
         else Productivity = 0.0;
         GPP[MyBoxNumber] = Productivity;
         NPP[MyBoxNumber] = GPP[MyBoxNumber];
         Productivity = Productivity / CUBIC * PhytoBiomass[MyBoxNumber];  //g C / m2 / s
         PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Productivity;
         if (GPP[MyBoxNumber]< 0.0)
         {
           Debugger(Productivity);Debugger(PhytoBiomass[MyBoxNumber]); Debugger(CCellQuota[MyBoxNumber]); Debugger(NutrientLimitation(MyBoxNumber));
         }
         //NPP[MyBoxNumber] = GPP[MyBoxNumber];   //g C / m2 / s
         GrossOxygenProduction[MyBoxNumber] = Productivity / OxygenToCarbonProd; //g O2 / m2 / s
         NetOxygenProduction[MyBoxNumber] = GrossOxygenProduction[MyBoxNumber]; //g O2 / m2 / s
      }
   }
}

void TUlvaSp::Respiration(int ABoxNumber)
{
   double Resp;

   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
               * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   Resp = RespirationCoefficient;
   if (MyWaterTemperaturePointer != NULL)
   {
        MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature,ABoxNumber,"Water temperature", ObjectCode);
        Resp = Resp * TemperatureLimitation(AWaterTemperature,
                                            AReferenceTemperature,
                                            ATemperatureCoefficient);
   }
   NPP[ABoxNumber] = NPP[ABoxNumber] - Resp / HOURSTOSECONDS;
   Resp = Resp /*mg C / g DW / h*/ / CUBIC * PhytoBiomass[ABoxNumber] / HOURSTOSECONDS;  //g C / m2 / s
   PhytoFlux[ABoxNumber] = PhytoFlux[ABoxNumber] - Resp;
   NetOxygenProduction[ABoxNumber] = NetOxygenProduction[ABoxNumber] - Resp / OxygenToCarbonResp; //g O2 / m2 /s
   if (MyNutrientPointer != NULL)
   {
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  NetOxygenProduction[ABoxNumber] / MyBoxDepth, ABoxNumber,"Oxygen",   //Return as mg O2 / L / s
                                  ObjectCode);
        OxygenDemand = MIN(NetOxygenProduction[ABoxNumber] / MyBoxDepth * MyPEcoDynClass->GetTimeStep(),0.0); //mg O2 / L
   }
}

void TUlvaSp::NitrogenUptake(int ABoxNumber)
{
        double MyDIN, MyNUptake;
        TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
        if (MyNutrientPointer != NULL)
        {
                MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyDIN,ABoxNumber,"DIN",ObjectCode);
                MyDIN = MyDIN * NITROGENATOMICWEIGHT / CUBIC; //Conversion from micro mol to mg
                MyNUptake = VNmax * MyDIN / (MyDIN + KN) *
                        MAX((MaxNCellQuota - NCellQuota[ABoxNumber]) / (MaxNCellQuota - MinNCellQuota),0.0) / HOURSTOSECONDS;  //mg N / g DW / s
                MyNUptake = MyNUptake / CUBIC * PhytoBiomass[ABoxNumber] * NCellQuota[ABoxNumber]; //g N / m2
                NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] +  MyNUptake;
                MyNutrientPointer->Update(GetEcoDynClassName(),
                                  -MyNUptake / NITROGENATOMICWEIGHT / MyBoxDepth * CUBIC * CUBIC, ABoxNumber,"DIN",   //Return as micromol / m3 / s
                                  ObjectCode);
        }
}

void TUlvaSp::PhosphorusUptake(int ABoxNumber)
{
        double MyPhosphate, MyPUptake;
        TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
        if (MyNutrientPointer != NULL)
        {
                MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyPhosphate,ABoxNumber,"Phosphate",ObjectCode);
                MyPhosphate = MyPhosphate * PHOSPHORUSATOMICWEIGHT / CUBIC;  //Conversion from micro mol to mg
                MyPUptake = VPmax * MyPhosphate / (MyPhosphate + KN) *
                        MAX((MaxPCellQuota - PCellQuota[ABoxNumber]) / (MaxPCellQuota - MinPCellQuota),0.0) / HOURSTOSECONDS;  //mg P / g DW / s
                MyPUptake = MyPUptake / CUBIC * PhytoBiomass[ABoxNumber] * PCellQuota[ABoxNumber]; //g P / m2
                PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] +  MyPUptake;
                MyNutrientPointer->Update(GetEcoDynClassName(),
                                  -MyPUptake / PHOSPHORUSATOMICWEIGHT / MyBoxDepth * CUBIC * CUBIC, ABoxNumber,"Phosphate",   //Return as micromol / m3 / s
                                  ObjectCode);
        }
}

double TUlvaSp::NutrientLimitation(int ABoxNumber)
{
   double ALimitation;
   ALimitation =  MIN(
                       (NCellQuota[ABoxNumber]-MinNCellQuota)/(MaxNCellQuota-MinNCellQuota),
                       (PCellQuota[ABoxNumber]-MinPCellQuota)/(MaxPCellQuota-MinPCellQuota)
                     );

   if (ALimitation < 0.0)
      return 0.0;
   else
   if (ALimitation > 1.0)
      return 1.0;
   else
      return ALimitation;
}

void TUlvaSp::Mortality(int ABoxNumber)
{
        double CarbonLoss, NitrogenLoss, PhosphorusLoss, TotalDeathLoss, MyOxygen;
        TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer(),
                    * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
        if (PhytoBiomass[ABoxNumber] > 0.00000000001)
           TotalDeathLoss = DeathLoss * pow(PhytoBiomass[ABoxNumber], beta) / HOURSTOSECONDS;
        else
        {
           //Debugger(PhytoBiomass[ABoxNumber]);
           //Debugger(ABoxNumber);
           TotalDeathLoss = 0.0;
        }
        if (MyNutrientPointer != NULL)
        {
           MyNutrientPointer->Inquiry(GetEcoDynClassName(),MyOxygen,ABoxNumber,"Oxygen",ObjectCode);
           if (OxygenDemand < 0.0)
           {
              OxygenDemand = fabs(OxygenDemand);
              if (OxygenDemand > 0.0000001)
              TotalDeathLoss =  TotalDeathLoss + KT * MAX(OxygenDemand - MyOxygen, 0.0) / OxygenDemand * PhytoBiomass[ABoxNumber];
           }
        }
        CarbonLoss = TotalDeathLoss *  CCellQuota[ABoxNumber] / HOURSTOSECONDS; //g C / m2 / s
        PhytoFlux[ABoxNumber] = PhytoFlux[ABoxNumber] - CarbonLoss;
        //Debugger(DeathLoss); Debugger(CarbonLoss); Debugger(PhytoFlux[ABoxNumber]);
        NitrogenLoss = TotalDeathLoss *  NCellQuota[ABoxNumber] / HOURSTOSECONDS;  //g N / m2 / s
        NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] - NitrogenLoss;
        PhosphorusLoss = TotalDeathLoss *  PCellQuota[ABoxNumber] / HOURSTOSECONDS; //g P / m2 / s
        PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] - PhosphorusLoss;
        if (MySedimentPointer != NULL)
        {
                MySedimentPointer->Update(GetEcoDynClassName(), CarbonLoss * CUBIC /*mg / m2 / s*/, ABoxNumber, "Corganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), NitrogenLoss * CUBIC, ABoxNumber,"Norganic 1",ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), PhosphorusLoss * CUBIC, ABoxNumber, "Porganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), CarbonLoss * CUBIC  * PhytoBiomass[ABoxNumber] / CPhyto[ABoxNumber]/*mg DW/ m2 / s*/, ABoxNumber, "OrganicMass 1", ObjectCode);
        }
}
  

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

TRuppiaCirrhosa::TRuppiaCirrhosa(TEcoDynClass* APEcoDynClass, char* className)
								 : TMacrophytes(APEcoDynClass, className)
{
    bool BenthicRecord;
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
						cerr << "TRuppiaCirrhosa::TRuppiaCirrhosa - Benthic species record not initialized." << endl;
#endif  // __BORLANDC__

	int i;
	// Initialise pointers
	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
	{                                            // for boxes and one for loads into boxes
      PhytoDeadBiomass  = new double[NumberOfBoxes];
      IkTancada = new double[365];
      CanopyHeight = new double[365];
      KSelfShading = new double [NumberOfBoxes];
      ConversionToKUnits = new double[NumberOfBoxes];
      KLength = new double[NumberOfBoxes];
      GrossOxygenProductivity = new double[NumberOfBoxes];
      NetOxygenProductivity = new double[NumberOfBoxes];
      GrossOxygenProduction = new double[NumberOfBoxes];
      NetOxygenProduction = new double[NumberOfBoxes];
	}
	else
#ifdef __BORLANDC__
		MessageBox(0,
					  "Macrophyte object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TRuppiaCirrhosa::TRuppiaCirrhosa - Macrophyte object array not dimensioned." << endl;
#endif  // __BORLANDC__

   //Default values, just in case...
	DeathLoss = 0.05;
	RespirationCoefficient = 0.3;
	KValue = 2;//0.1;
   KCanopy = 0;
   IkFraction = 0;
   IkStAndre = 0;

   for (i = 0; i < NumberOfBoxes; i++)
   {
   	PhytoDeadBiomass[i] = 0.0;
      PhytoProd[i] = 0;
      Pmax[i] = 0.2; //ugC /ug C/ h
      MaintenanceRespiration[i] = 0.1 * Pmax[i];
      KSelfShading[i] = 0.0;
      ConversionToKUnits[i] = 0.0;
      KLength[i] = 0.0;
      GrossOxygenProductivity[i] = 0.0;
      NetOxygenProductivity[i] = 0.0;
      GrossOxygenProduction[i] = 0.0;
      NetOxygenProduction[i] = 0.0;
   }
   for (i = 0; i < 365; i++)
   {
   	IkTancada[i] = 0;
      CanopyHeight[i] = 0;
   }
   TimeStep = MyPEcoDynClass->GetTimeStep();

    int X,Y, XV, YV;
	VariableCount = 0;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("RuppiaCirrhosa");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "RuppiaCirrhosa variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TRuppiaCirrhosa::TRuppiaCirrhosa - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Ruppia cirrhosa", X, Y))
        {
        PReadWrite->ReadNumber(X + NUMBEROFVARSCOLUMN, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
          // Create array for state variable units
            //UnitNameArray = new UNA[NumberOfVariables];
            // read in the variable names
            char MyVariable[65];
            for (i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);
            
                if (strcmp(MyVariable, "R. cirrhosa biomass") == 0)
                {
                    if (!PReadWrite->FindString("R. cirrhosa biomass values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X + FIRSTBOXDATACOLUMN + j, i, PhytoBiomass[j]);
                        }
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[j]);
                        }
                    }
                }
                else
                if (strcmp(MyVariable, "R. cirrhosa Canopy height") == 0)
                {
                    if (!PReadWrite->FindString("R. cirrhosa Canopy height values", XV, YV))
                    {
                        XV = X + FIRSTBOXDATACOLUMN;
                        YV = Y - 1;
                    }
                    for (int j = 0; j < 365; j++)
                    {
                        PReadWrite->ReadNumber(XV, YV + 1 + j, CanopyHeight[j]);
                    }
                }
            //More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - R. cirrhosa", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRuppiaCirrhosa::TRuppiaCirrhosa - Variables: undefined object - R. cirrhosa" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

   //Inicialização de KSelfShading e de ConversionToKUnits. Estas duas variáveis são utilizadas
   //no cálculo do coeficiente de extinção da luz dentro do povoamento de Ruppia (ver Tese do Gonçalo)
   for (int j = 0; j < NumberOfBoxes; j++)
   {
   	//KSelfShading[j] = (2041.2*PhytoBiomass[j])/(360-(6.67*PhytoBiomass[j]));
      KSelfShading[j] = (2041.2*23)/(360-(6.67*23));
      ConversionToKUnits[j] = 10/(360/(KSelfShading[j]+360));
      //KLength[j] = (1130.4*PhytoBiomass[j])/(240-(5.71*PhytoBiomass[j]));
      KLength[j] = (1130.4*23)/(240-(5.71*23));
   }

	ParameterCount = 0;
	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("RuppiaCirrhosa");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "RuppiaCirrhosa parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TRuppiaCirrhosa::TRuppiaCirrhosa - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

       double MyValue;

        if (PReadWrite->FindString("Ruppia cirrhosa", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (i = Y; i < Y + NumberOfParameters; i++)
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
                if (strcmp(MyParameter, "Respiration Coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        RespirationCoefficient = MyValue; // Correct to make this an array for consistency with MaintenanceRespiration - jgf/pd 96.09.24
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
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        DeathLoss = MyValue;
                }
             else
             if (strcmp(MyParameter, "IkFraction") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                IkFraction = MyValue;
                }
             else
             if (strcmp(MyParameter, "IkTancada") == 0)
                {
                for (int j = 0; j < 365; j++)
                {
                    PReadWrite->ReadNumber(X+3, i+j, MyValue);
                   //Debugger(MyValue);
                    IkTancada[j] = MyValue;
                }
                }
             else
             if (strcmp(MyParameter, "Oxygen to Carbon in photosynthesis") == 0)
             {
                PReadWrite->ReadNumber(X+3, i, OxygenToCarbon);
                //Debugger(OxygenToCarbon);
             }
             else
             if (strcmp(MyParameter, "Carbon to dry weight") == 0)
             {
                PReadWrite->ReadNumber(X+3, i, CarbonToDryWeight);
                //Debugger(CarbonToDryWeight);
             }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - R. cirrhosa", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRuppiaCirrhosa::TRuppiaCirrhosa - Parameters: undefined object - R. cirrhosa" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}


TRuppiaCirrhosa::~TRuppiaCirrhosa()
{
    freeMemory();
}

void TRuppiaCirrhosa::freeMemory()
{
    try {
        if (NumberOfVariables > 0)
        {
          delete [] VariableNameArray;
          //delete [] UnitNameArray;
          NumberOfVariables = 0;
        }
        if (NumberOfParameters > 0) {
          delete [] ParametersNameArray;
          NumberOfParameters = 0;
        }

        if (NumberOfBoxes > 0)
       {
          delete [] PhytoDeadBiomass;
          delete [] IkTancada;
          delete [] CanopyHeight;
          delete [] KSelfShading;
          delete [] ConversionToKUnits;
          delete [] KLength;
          delete [] GrossOxygenProductivity;
          delete [] NetOxygenProductivity;
          delete [] GrossOxygenProduction;
          delete [] NetOxygenProduction;
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
				cerr << "TRuppiaCirrhosa::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TRuppiaCirrhosa::GetParameterValue(char* MyParameter)
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
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration[0];
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "IkFraction") == 0)
    {
        value = IkFraction;
    }
    else if (strcmp(MyParameter, "IkTancada") == 0)
    {
        value = IkTancada[0];
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon in photosynthesis") == 0)
    {
        value = OxygenToCarbon;
    }
    else if (strcmp(MyParameter, "Carbon to dry weight") == 0)
    {
        value = CarbonToDryWeight;
    }
    else
        value = 0.0;
    return value;
}

bool TRuppiaCirrhosa::SetParameterValue(char* MyParameter, double value)
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
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            MaintenanceRespiration[j] = value;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "IkFraction") == 0)
    {
        IkFraction = value;
    }
    else if (strcmp(MyParameter, "IkTancada") == 0)
    {
        for (j = 0; j < 365; j++)
            IkTancada[j] = value;
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon in photosynthesis") == 0)
    {
        OxygenToCarbon = value;
    }
    else if (strcmp(MyParameter, "Carbon to dry weight") == 0)
    {
        CarbonToDryWeight = value;
    }
    else
        rc = false;

    return rc;
}

void TRuppiaCirrhosa::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber, JulianDay;
	strcpy(MyParameter, ParameterName);

   	if (strcmp(MyParameter, "R. cirrhosa biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa dead biomass") == 0)
		Value = PhytoMortality[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa total GPP") == 0)
		Value = GPP[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa total NPP") == 0)
		Value = NPP[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
			Value = GPP[MyBoxNumber]
			    	  / PhytoBiomass[MyBoxNumber];
    }
	else if (strcmp(MyParameter, "R. cirrhosa NPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
			Value = NPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "R. cirrhosa net biomass production") == 0)
		Value = PhytoProd[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa gross oxygen productivity") == 0)
		Value = GrossOxygenProductivity[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa net oxygen productivity") == 0)
		Value = NetOxygenProductivity[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa gross oxygen production") == 0)
		Value = GrossOxygenProduction[MyBoxNumber];
    else if (strcmp(MyParameter, "R. cirrhosa net oxygen production") == 0)
		Value = NetOxygenProduction[MyBoxNumber];
	else if (strcmp(MyParameter, "R. cirrhosa mortality") == 0)
		Value = PhytoMortality[MyBoxNumber];
	else if (strcmp(MyParameter, "R. cirrhosa Canopy height") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
            Value = GetCanopyHeight(NumberOfBoxes - 1);
    }
    else if (strcmp(MyParameter, "Ruppia cirrhosa Canopy extinction coef") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
        {
         	KCanopy = GetCanopyExtinctionCoefficient(MyBoxNumber);
         	Value = KCanopy;
        }
    }
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
                      "EcoDynamo Alert - Inquiry 6",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRuppiaCirrhosa::Inquiry 6 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TRuppiaCirrhosa::Update(char* srcName, double Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
{
   char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

	for (int i=0; i < NumberOfVariables; i++)
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{
			if (strcmp(MyParameter, "R. cirrhosa biomass") == 0)
				PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Value;
			return;
		}

#ifdef __BORLANDC__
	char Caption[129];
	strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 3",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TRuppiaCirrhosa::Update 3 - " << MyParameter << " does not exist n " <<GetEcoDynClassName() << endl;
#endif  // __BORLANDC__

}

bool TRuppiaCirrhosa::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "R. cirrhosa biomass") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa Canopy height") == 0)
    {
        for (int j = 0; j < 365; j++)
        {
            CanopyHeight[j] = Value;
        }
    }
    else if (strcmp(VariableName, "R. cirrhosa dead biomass") == 0)
		PhytoMortality[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa total GPP") == 0)
		GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa total NPP") == 0)
		NPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa net biomass production") == 0)
		PhytoProd[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa gross oxygen productivity") == 0)
		GrossOxygenProductivity[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa net oxygen productivity") == 0)
		NetOxygenProductivity[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa gross oxygen production") == 0)
		GrossOxygenProduction[BoxNumber] = Value;
    else if (strcmp(VariableName, "R. cirrhosa net oxygen production") == 0)
		NetOxygenProduction[BoxNumber] = Value;
	else if (strcmp(VariableName, "R. cirrhosa mortality") == 0)
		PhytoMortality[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TRuppiaCirrhosa::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("RuppiaCirrhosa");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Ruppia cirrhosa");
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
     * ruppia cirrhosa biomass initial values
     */
    PReadWrite->WriteCell("R. cirrhosa biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j], 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * ruppia cirrhosa canopy height daily values
     */
    PReadWrite->WriteCell("R. cirrhosa Canopy height values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(CanopyHeight[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TRuppiaCirrhosa::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("RuppiaCirrhosa");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Ruppia cirrhosa");
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
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "IkFraction") == 0)
        {
            PReadWrite->WriteCell(IkFraction, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen to Carbon in photosynthesis") == 0)
        {
            PReadWrite->WriteCell(OxygenToCarbon, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to dry weight") == 0)
        {
            PReadWrite->WriteCell(CarbonToDryWeight, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * IkTancada daily values
     */
    PReadWrite->WriteCell("IkTancada values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(IkTancada[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TRuppiaCirrhosa::Go()    //Atenção!!! Este void só funciona para modelos verticais
{
	double CanopyHeight, BoxHeight;//, UpperBoxForOxygenDistribution = 0;
   UpperBoxForOxygenDistribution = 0;
	for (int i = 0; i < NumberOfBoxes; i++)
   {
   	GPP[i] = 0;
      NPP[i] = 0;
      PhytoProd[i] = 0;
      GrossOxygenProductivity[i] = 0;
      NetOxygenProductivity[i] = 0;
      GrossOxygenProduction[i] = 0;
      NetOxygenProduction[i] = 0;
   }
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   if (MyLightPointer != NULL)
   	MyLightPointer->Inquiry(GetEcoDynClassName(), LagoonDepth,
                                  0,
                                  "Lagoon depth",
                                  ObjectCode);
   else
   {
#ifdef __BORLANDC__
   	MessageBox(0, "Light object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TRuppiaCirrhosa::Go - Light object is undefined." << endl;
#endif  // __BORLANDC__
      return;
   }
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyNutrientPointer != NULL)
   {
   	if (PhytoBiomass[NumberOfBoxes - 1] > 0)
   	{
   		CanopyHeight = GetCanopyHeight(NumberOfBoxes - 1);
         //CanopyHeight = LagoonDepth;
      	if(CanopyHeight > LagoonDepth)
      		CanopyHeight = LagoonDepth;

         BoxHeight = LagoonDepth / NumberOfBoxes;
         if (CanopyHeight == LagoonDepth)
         	UpperBoxForOxygenDistribution = 0;
         else
         	UpperBoxForOxygenDistribution = NumberOfBoxes - ceil(CanopyHeight / BoxHeight);
         /*Debugger(BoxHeight);
         Debugger(CanopyHeight);
         Debugger(UpperBoxForOxygenDistribution);
         Debugger(NumberOfBoxes - UpperBoxForOxygenDistribution); */
   	}
   }
   Year = MyPEcoDynClass->GetYear();
   JulianDay = MyPEcoDynClass->GetJulianDay();
   if ((JulianDay >= 146) || (Year > 1))
   {
      //MessageBox(0, "Production", "EcoDynamo alert", MB_OK);
		Production();
   	Respiration();
   	Mortality();
   	for (int i = 0; i < NumberOfBoxes; i++)
   		PhytoFlux[i] = PhytoProd[i];
   }

   //TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyNutrientPointer != NULL)
   {
   	/*if (PhytoBiomass[NumberOfBoxes - 1] > 0)
   	{
   		CanopyHeight = GetCanopyHeight(NumberOfBoxes - 1);
      	if(CanopyHeight > LagoonDepth)
      		CanopyHeight = LagoonDepth;
         BoxHeight = LagoonDepth / NumberOfBoxes;
         UpperBoxForOxygenDistribution = NumberOfBoxes - ceil(CanopyHeight / BoxHeight);
   	} */
   	for (int i = UpperBoxForOxygenDistribution; i < NumberOfBoxes; i++)
   	{
      	//Debugger(i); Debugger(NetOxygenProduction[i]);
   		MyNutrientPointer->Update(GetEcoDynClassName(), NetOxygenProduction[NumberOfBoxes - 1]
                                    /*/(NumberOfBoxes - UpperBoxForOxygenDistribution)*/, // mg / d / l
                                    i,
                                    "Oxygen",
                                    ObjectCode);
   	}
   }
}

void TRuppiaCirrhosa::Integrate()
{
	// Integrate biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
}


void TRuppiaCirrhosa::Production()
{
	double CanopyExtinctionCoef, WaterExtinctionCoef, CanopyHeight, CanopyTopLight, LightAtTop, BottomLight;
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (PhytoBiomass[i] > 0)
      {
      	if (MyLightPointer != NULL)
      	{
   			IkStAndre = GetIkStAndre(IkTancada[JulianDay],LagoonDepth);
      		CanopyHeight = GetCanopyHeight(i);
            //CanopyHeight = LagoonDepth;
      		CanopyExtinctionCoef = GetCanopyExtinctionCoefficient(i);

         	//Debugger(LagoonDepth);Debugger(IkStAndre);Debugger(CanopyExtinctionCoef);
         	//Debugger(GetDailyPmax());Debugger(CanopyHeight);

   			/*MyLightPointer->Inquiry(GetEcoDynClassName(), CanopyTopLight,
            	                         LagoonDepth - CanopyHeight,
												    i,
												    "Sub-surface PAR irradiance",
												    ObjectCode);*/
            MyLightPointer->Inquiry(GetEcoDynClassName(), WaterExtinctionCoef,
												    i,
												    "KValue",
												    ObjectCode);
            MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
											       i,
											       "PAR surface irradiance",
											       ObjectCode);
            CanopyTopLight = LightAtTop * exp(- WaterExtinctionCoef * (LagoonDepth - CanopyHeight));

         	BottomLight = CanopyTopLight * exp(- CanopyExtinctionCoef * CanopyHeight);
         	if (CanopyTopLight > 0)
         	{
	      		GrossOxygenProductivity[i] = (GetDailyPmax()/(CanopyExtinctionCoef * CanopyHeight))*
         			  				              (log(fabs(IkStAndre + CanopyTopLight * WATTSTOMICROEINSTEINS)/
                 	  	                      fabs(IkStAndre + BottomLight * WATTSTOMICROEINSTEINS)));//mgO2 g(DW)-1 dia-1

            	GrossOxygenProduction[i] = GrossOxygenProductivity[i] *
               	                        PhytoBiomass[i] /
                                          (
                  	                     	(NumberOfBoxes -
                                              UpperBoxForOxygenDistribution) *
                                          	(LagoonDepth / NumberOfBoxes)
                                           )  /
                     	                  CUBIC; //mg O2 l-1 d-1
            	NetOxygenProductivity[i] = GrossOxygenProductivity[i];
            	NetOxygenProduction[i] = GrossOxygenProduction[i];
            	PhytoProd[i] = GrossOxygenProductivity[i] *
               	            OxygenToCarbon *
                  	         CarbonToDryWeight /
                     	      CUBIC *
                        	   PhytoBiomass[i]; //g(DW) m-2 d-1

            	//Debugger(PhytoProd[i]);
         		GPP[i] = PhytoProd[i];
            	NPP[i] = PhytoProd[i];
         	}
         }
		}
   }
}

void TRuppiaCirrhosa::Respiration()
{
	double Respiration;

	if (JulianDay < 90)
   	Respiration = 0.63;
   else
   if ((JulianDay >= 90) && (JulianDay < 137))
      Respiration = (0.08872 * JulianDay - 7.3355);
   else
   if ((JulianDay >= 137) && (JulianDay < 193))
      Respiration = (-0.0630 * JulianDay + 13.44);
   else
   if ((JulianDay >= 193) && (JulianDay < 296))
   	Respiration = (-0.00689 * JulianDay + 2.60);
   else
   if ((JulianDay >= 296) && (JulianDay < 359))
      Respiration = (0.00111 * JulianDay + 0.23);
   else
   	Respiration = 0.63;
   Respiration = Respiration * DAYSTOHOURS; //mgO2 g(DW)-1 dia-1
   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (PhytoBiomass[i] > 0)
      {
   		NetOxygenProductivity[i] = NetOxygenProductivity[i] - Respiration;

   		NetOxygenProduction[i] = NetOxygenProduction[i] -
         	                      Respiration *
      	   	                   PhytoBiomass[i] /
         	   	                (
                                  	(NumberOfBoxes -
                                     UpperBoxForOxygenDistribution) *
                                  	(LagoonDepth / NumberOfBoxes)
                                   ) /
            	   	             CUBIC; //mg O2 l-1 d-1
   		PhytoProd[i] = PhytoProd[i] -
      		            Respiration *
         		         OxygenToCarbon *
            		      CarbonToDryWeight /
               		   CUBIC *
                  		PhytoBiomass[i]; //g(DW) m-2 d-1
   		NPP[i] = PhytoProd[i];
      }
   }
}

void TRuppiaCirrhosa::Mortality()
{
	for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (PhytoBiomass[i] > 0)
      {
   		PhytoMortality[i] = DeathLoss * PhytoBiomass[i];
   		PhytoProd[i] = PhytoProd[i] - PhytoMortality[i];
		}
   }
}

double TRuppiaCirrhosa::GetIkStAndre (double IkTancada,
                                      double LagoonDepth)
{
	return IkTancada / LagoonDepth * IkFraction;
}


double TRuppiaCirrhosa::GetCanopyExtinctionCoefficient(int ABoxNumber)
{
	return PhytoBiomass[ABoxNumber] /
          (KSelfShading[ABoxNumber] + PhytoBiomass[ABoxNumber])
          * ConversionToKUnits[ABoxNumber];
}

double TRuppiaCirrhosa::GetCanopyHeight (int ABoxNumber)
{
  	if (PhytoBiomass[ABoxNumber] < 240)
   {
   	//Debugger(ABoxNumber);
   	//Debugger(PhytoBiomass[ABoxNumber]);
      //Debugger(KLength[ABoxNumber]);
   	return PhytoBiomass[ABoxNumber] /(KLength[ABoxNumber]+PhytoBiomass[ABoxNumber])*
         	 0.4/(240/(KLength[ABoxNumber] + 240));
   }
   else
   	return 0.4;
}

double TRuppiaCirrhosa::GetDailyPmax()
{
	double MyJulianDay;
   MyJulianDay = MyPEcoDynClass->GetJulianDay();
	if (MyJulianDay < 90)
   	return 5.50 * DAYSTOHOURS;
   else
   if ((MyJulianDay >= 90) && (MyJulianDay < 137))
   	return 5.50 * DAYSTOHOURS;
   else
   if ((MyJulianDay >= 137) && (MyJulianDay < 193))
   	return (-0.21 * MyJulianDay + 49.78) * DAYSTOHOURS;
   else
   if ((MyJulianDay >= 193) && (MyJulianDay < 296))
   	return (-0.066 * MyJulianDay + 21.94) * DAYSTOHOURS;
   else
   if ((MyJulianDay >= 296) && (MyJulianDay < 359))
   	return (0.050 * MyJulianDay - 12.45) * DAYSTOHOURS;
   else
   	return 5.50 * DAYSTOHOURS;

}


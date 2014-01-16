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



TLaminariaJaponica::TLaminariaJaponica(TEcoDynClass* APEcoDynClass, char* className)
								 : TMacrophytes(APEcoDynClass, className)

{
	Ik = new double[NumberOfBoxes];
   CultureTop = new double[NumberOfBoxes];
   CultureBottom = new double[NumberOfBoxes];
   GrossOxygenProductivity = new double[NumberOfBoxes];
   NetOxygenProductivity = new double[NumberOfBoxes];
   GrossOxygenProduction = new double[NumberOfBoxes];
   NetOxygenProduction = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	Ik[i] = 0.0;
      CultureTop[i] = 0.0;
      CultureBottom[i] = 0.0;
      GrossOxygenProductivity[i] = 0.0;
      NetOxygenProductivity[i] = 0.0;
      GrossOxygenProduction[i] = 0.0;
      NetOxygenProduction[i] = 0.0;
   }

   int X, Y;
   // Read in the parameters
   ParameterCount = 0;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("LaminariaJaponica");
	if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "LaminariaJaponica parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TLaminariaJaponica::TLaminariaJaponica - LaminariaJaponica parameters file missing." << endl;
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

             /*if (strcmp(MyParameter, "Columns") == 0)
             {
                PReadWrite->ReadNumber(X+3, i, GridColumns);
                //Debugger(GridColumns);
             }
             else if  (strcmp(MyParameter, "Lines") == 0)
             {
                PReadWrite->ReadNumber(X+3, i, GridLines);
                //Debugger(GridLines);
             }
             else*/
             if (strcmp(MyParameter, "Pmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Pmax[j] = MyValue;
                }
             else
             if (strcmp(MyParameter, "Ik") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                for (int j = 0; j < NumberOfBoxes; j++)
                        Ik[j] = MyValue;
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
             if (strcmp(MyParameter, "Oxygen to Carbon") == 0)
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
                else
                if (strcmp(MyParameter, "Death loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, DeathLoss);
                //Debugger(DeathLoss);
                }
             else
             if (strcmp(MyParameter, "Depth of culture bottom") == 0)
             {
                PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                for (int j = 0; j < NumberOfBoxes; j++)
                        CultureBottom[j] = MyValue; //m
             }
             else
             if (strcmp(MyParameter, "Depth of culture top") == 0)
             {
                PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                for (int j = 0; j < NumberOfBoxes; j++)
                        CultureTop[j] = MyValue; //m
             }
             else
                if (strcmp(MyParameter, "Temperature augmentation rate for photosynthesis") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TemperatureAugmentationRate);
                //Debugger(TemperatureAugmentationRate);
                }
             else
             if (strcmp(MyParameter, "KAmmonia") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                for (int j = 0; j < NumberOfBoxes; j++)
                    KAmmonia[j] = MyValue;
                }
             else
             if (strcmp(MyParameter, "KNitrate") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                for (int j = 0; j < NumberOfBoxes; j++)
                    KNitrate[j] = MyValue;
                }
             else
                if (strcmp(MyParameter, "Respiration Coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RespirationCoefficient);
                //Debugger(RespirationCoefficient);
                }
            }
        }
        else
#ifdef __BORLANDC__
					MessageBox(0, "Undefined object - L. japonica", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TLaminariaJaponica::TLaminariaJaponica - Undefined parameters object - L. japonica" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }

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
    int Index;

   // Read in the variables
	VariableCount = 0;
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("LaminariaJaponica");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "LaminariaJaponica variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TLaminariaJaponica::TLaminariaJaponica - LaminariaJaponica variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Laminaria japonica", X, Y))
        {
        char MyVariable[65];
        PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
          //Debugger(NumberOfVariables);
          //Debugger(X); Debugger(Y);
            // read in the variable names
            int XV, YV;
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
                //More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - L. japonica", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TLaminariaJaponica::TLaminariaJaponica - Undefined object - L. japonica" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TLaminariaJaponica::~TLaminariaJaponica()
{
    freeMemory();
}

void TLaminariaJaponica::freeMemory()
{
    try {
        if (NumberOfBoxes > 0)
       {
          delete [] Ik;
          delete [] CultureTop;
          delete [] CultureBottom;
          delete [] GrossOxygenProductivity;
          delete [] NetOxygenProductivity;
          delete [] GrossOxygenProduction;
          delete [] NetOxygenProduction;
        }
        if (NumberOfBoxesWithLaminariaJaponica > 0)
          delete [] LaminariaJaponicaRecord;
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
				cerr << "TLaminariaJaponica::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TLaminariaJaponica::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Pmax") == 0)
    {
        value = Pmax[0];
    }
    else if (strcmp(MyParameter, "Ik") == 0)
    {
        value = Ik[0];
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration[0];
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon") == 0)
    {
        value = OxygenToCarbon;
    }
    else if (strcmp(MyParameter, "Carbon to dry weight") == 0)
    {
        value = CarbonToDryWeight;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "Depth of culture bottom") == 0)
    {
        value = CultureBottom[0];
    }
    else if (strcmp(MyParameter, "Depth of culture top") == 0)
    {
        value = CultureTop[0];
    }
    else if (strcmp(MyParameter, "Temperature augmentation rate for photosynthesis") == 0)
    {
        value = TemperatureAugmentationRate;
    }
    else if (strcmp(MyParameter, "KAmmonia") == 0)
    {
        value = KAmmonia[0];
    }
    else if (strcmp(MyParameter, "KNitrate") == 0)
    {
        value = KNitrate[0];
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        value = RespirationCoefficient;
    }
    else
        value = 0.0;
    return value;
}

bool TLaminariaJaponica::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "Pmax") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Pmax[j] = value;
    }
    else if (strcmp(MyParameter, "Ik") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Ik[j] = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            MaintenanceRespiration[j] = value;
    }
    else if (strcmp(MyParameter, "Oxygen to Carbon") == 0)
    {
        OxygenToCarbon = value;
    }
    else if (strcmp(MyParameter, "Carbon to dry weight") == 0)
    {
        CarbonToDryWeight = value;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "Depth of culture bottom") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            CultureBottom[j] = value;
    }
    else if (strcmp(MyParameter, "Depth of culture top") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            CultureTop[j] = value;
    }
    else if (strcmp(MyParameter, "Temperature augmentation rate for photosynthesis") == 0)
    {
        TemperatureAugmentationRate = value;
    }
    else if (strcmp(MyParameter, "KAmmonia") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            KAmmonia[j] = value;
    }
    else if (strcmp(MyParameter, "KNitrate") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            KNitrate[j] = value;
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        RespirationCoefficient = value;
    }
    else
        rc = false;

    return rc;
}

void TLaminariaJaponica::Update(char* srcName, double Value,
									      int BoxNumber,
									     char* ParameterName,
									     int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber, JulianDay;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

   	if (strcmp(MyParameter, "L. japonica biomass density") == 0)
	    PhytoBiomass[MyBoxNumber] = Value;
/*      else
			return;
   }

	static char Caption[129];
	strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
   //Debugger(AnObjectCode);
	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update",
				  MB_OK | MB_ICONHAND);

	Value = 0;*/
}

void TLaminariaJaponica::Update(char* srcName, double Value,
                                 double HarvestableSize,
											int BoxNumber,
											int AnObjectCode)
{
    LogMessage("Update", srcName, "Harvestable Size", Value, BoxNumber);
}

bool TLaminariaJaponica::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									     char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

   	if (strcmp(VariableName, "L. japonica biomass density") == 0)
	    PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "L. japonica total GPP") == 0)
      	GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "L. japonica total NPP") == 0)
     	NPP[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

void TLaminariaJaponica::Inquiry(char* srcName, double &Value,
                                     int BoxNumber,
                                     char* ParameterName,
                                     int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
   int MyBoxNumber = BoxNumber;
   strcpy(MyParameter, ParameterName);
   for (int i=0; i < NumberOfVariables; i++)
   if (strcmp(MyParameter, VariableNameArray[i]) == 0)
   {
   	if (strcmp(MyParameter, "L. japonica biomass density") == 0)
      	Value = PhytoBiomass[MyBoxNumber];
      else if (strcmp(MyParameter, "L. japonica total GPP") == 0)
      	Value = GPP[MyBoxNumber];
      else if (strcmp(MyParameter, "L. japonica total NPP") == 0)
      	Value = NPP[MyBoxNumber];
      else if (strcmp(MyParameter, "L. japonica GPP per unit mass") == 0)
      {
      	if (PhytoBiomass[MyBoxNumber] > 0.0)
      		Value = GPP[MyBoxNumber] /PhytoBiomass[MyBoxNumber] * HOURSTODAYS;
         else Value = 0.0;
      }
      else if (strcmp(MyParameter, "L. japonica NPP per unit mass") == 0)
      {
      	if (PhytoBiomass[MyBoxNumber] > 0.0)
      		Value = NPP[MyBoxNumber]/ PhytoBiomass[MyBoxNumber] * HOURSTODAYS;
         else Value = 0.0;
      }
      LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
      return;
   }
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
		cerr << "TLaminariaJaponica::Inquiry 3 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
   Value = 0.0;
}


void TLaminariaJaponica::Inquiry(char* srcName, double &Value,
                                     int BoxNumber,
                                     double HarvestSize,
                                     int AnObjectCode)
{
    Value = 0.0;
    LogMessage("Inquiry", srcName, "Harvestable Biomass", Value, BoxNumber);
}

// AP, 2007.07.13
bool TLaminariaJaponica::SaveVariables()
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
    PReadWrite->WriteSeparator(true);
    int Index;
    for (int j = 0; j < NumberOfBoxesWithLaminariaJaponica; j++)
    {
        Index = LaminariaJaponicaRecord[j].LineCoordinate * NumberOfColumns +
               LaminariaJaponicaRecord[j].ColumnCoordinate;
        PReadWrite->WriteCell(PhytoBiomass[Index], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TLaminariaJaponica::SaveParameters()
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

        if (strcmp(ParametersNameArray[i], "Pmax") == 0)
        {
            PReadWrite->WriteCell(Pmax[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Ik") == 0)
        {
            PReadWrite->WriteCell(Ik[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen to Carbon") == 0)
        {
            PReadWrite->WriteCell(OxygenToCarbon, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to dry weight") == 0)
        {
            PReadWrite->WriteCell(CarbonToDryWeight, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Depth of culture bottom") == 0)
        {
            PReadWrite->WriteCell(CultureBottom[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Depth of culture top") == 0)
        {
            PReadWrite->WriteCell(CultureTop[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Temperature augmentation rate for photosynthesis") == 0)
        {
            PReadWrite->WriteCell(TemperatureAugmentationRate, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KAmmonia") == 0)
        {
            PReadWrite->WriteCell(KAmmonia[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNitrate") == 0)
        {
            PReadWrite->WriteCell(KNitrate[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration Coefficient") == 0)
        {
            PReadWrite->WriteCell(RespirationCoefficient, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TLaminariaJaponica::Go()
{
	//JulianDay = MyPEcoDynClass->GetJulianDay();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
   	GPP[i] = 0.0;
      NPP[i] = 0.0;
	}
	Production();
   Respiration();
   //Exudation();
   //Mortality();
   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	GPP[i] = GPP[i] * OxygenToCarbon * CarbonToDryWeight / CUBIC; //g DW  day-1
   	PhytoFlux[i] = PhytoFlux[i] * OxygenToCarbon * CarbonToDryWeight / CUBIC;   //g DW day-1
      NPP[i] = PhytoFlux[i]; //g DW day-1
	}
}

void TLaminariaJaponica::Integrate()
{
	// Integrate biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
}

void TLaminariaJaponica::Production()
{
	double /*CanopyExtinctionCoef,*/ LightAtTop, LightAtBottom, ATop, ABottom;
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   //TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (PhytoBiomass[i] > 0)
      {
      	if (MyLightPointer != NULL)
      	{
      		/*CultureTop[i] = CultureBottom[i] - GetCanopyHeight(i);
            if (CultureTop[i] < 0.0)
            	CultureTop[i] = 0.0; */
      		//CanopyExtinctionCoef = GetCanopyExtinctionCoefficient(i);
			   ATop = CultureTop[i];
      		ABottom = CultureBottom[i];

            MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
                                     	 ATop,
                                     	 i,
                                        "Sub-surface PAR irradiance",
                                        ObjectCode);

				MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
                                        ABottom,
												    i,
												    "Sub-surface PAR irradiance",
												    ObjectCode);

				LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
				LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;

         	if (LightAtTop == 0)
         		Productivity = 0.0;
         	else
         	{
            	Productivity = (
               						Pmax[i] * (LightAtTop * exp(LightAtTop / Ik[i]) + Ik[i]) * exp(-LightAtTop / Ik[i]) -
                                 Pmax[i] * (LightAtBottom * exp(LightAtBottom / Ik[i]) + Ik[i]) * exp(-LightAtBottom / Ik[i])
                              )
                              / (ABottom - ATop)
                              * DAYSTOHOURS;   //mgO2 g(DW)-1 dia-1

               Productivity = Productivity * TemperatureLimitation(i) * NutrientLimitation(i);

               //Productivity is calculated as the integral of Pmax*(1-exp(-I/Ik) with respect to light. This was
               //the equation used by Jean-Marc Deslous Paoli with experimental data on L. japonica from Sango Bay
         	}
         }
         GPP[i] = Productivity * PhytoBiomass[i]; //mgO2 dia-1
         PhytoFlux[i] = PhytoFlux[i] + GPP[i]; //mgO2 dia-1
		}
   }
}


void TLaminariaJaponica::Respiration()
{
	double Respiration, WaterTemperature;;
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	if (PhytoBiomass[i] > 0)
      {
      	if (MyWaterTemperaturePointer != NULL)
   		{
   			MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
            		                                 i,
                  		                           "Water temperature",
                        		                     ObjectCode);
            if (WaterTemperature <= 10.0)
            	Respiration = (RespirationCoefficient * 10.0 - 1.36) * DAYSTOHOURS; //Default respiration at 10?, as measured by Jean Marc
                                                                                   //Deslous-Paoli in Sango during the INCO project mgO2 g(DW)-1 dia-1;
            else
      			Respiration = (RespirationCoefficient * WaterTemperature - 1.36) * DAYSTOHOURS; //mgO2 g(DW)-1 dia-1
   		}
   		else
         	Respiration = (RespirationCoefficient * 10.0 - 1.36) * DAYSTOHOURS; //Default respiration at 10?, as measured by Jean Marc
                                                                                //Deslous-Paoli in Sango during the INCO project mgO2 g(DW)-1 dia-1;

         if (Respiration < 0.0) Respiration = 0.0;
   		/*NetOxygenProductivity[i] = NetOxygenProductivity[i] - Respiration;

   		NetOxygenProduction[i] = NetOxygenProduction[i] -
         	                      Respiration *
      	   	                   PhytoBiomass[i] /
         	   	                (
                                  	(NumberOfBoxes -
                                     UpperBoxForOxygenDistribution) *
                                  	(LagoonDepth / NumberOfBoxes)
                                   ) /
            	   	             CUBIC; //mg O2 l-1 d-1  */
   		PhytoFlux[i] = PhytoFlux[i] - Respiration * PhytoBiomass[i]; //mgO2 dia-1
      }
   }
}

void TLaminariaJaponica::Exudation()
{

}

void TLaminariaJaponica::FrondBreakage()
{

}

void TLaminariaJaponica::Mortality()
{

}


double TLaminariaJaponica::TemperatureLimitation(int ABoxNumber)
{
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      if (WaterTemperature <= 10.0)
      	return 1.0;
      else
      	return (exp(TemperatureAugmentationRate * WaterTemperature));
   }
   else return 1.0;
}

double TLaminariaJaponica::NutrientLimitation(int ABoxNumber)
{
	return 1.0;
}

double TLaminariaJaponica::WeightVsLenght(double Size)
{
	return 1.0;
}

double TLaminariaJaponica::LengthVsWeight(double Weight)
{
	return 1.0;
}
double TLaminariaJaponica::ArrheniusTemperatureLimitation(int ABoxNumber, double AWaterTemperature)
{
	return 1.0;
}

double TLaminariaJaponica::StochasticFrondBreakage(double size)
{
	return 1.0;
}

double TLaminariaJaponica::SizeOfBrokenPortion (double size)
{
	return 1.0;
}

double TLaminariaJaponica::GetCanopyExtinctionCoefficient(int ABoxNumber)
{
//This function should return light extinction coefficient within the kelp canopy
	return 1.0;
}

double TLaminariaJaponica::GetCanopyHeight(int ABoxNumber)
{
//This function should return kelp canopy height
	return 1.0;
}




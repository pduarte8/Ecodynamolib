// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 01.11.09
/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
	#include <stdlib.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	#include <cstdlib>
	#include <ctime>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "zoobobjt.h"
#include "iodll.h"
#include "EcoDynClass.h"



// TChlamysFarreri Class for individual based modelling
//



TChlamysFarreriIndividual::TChlamysFarreriIndividual(TEcoDynClass* APEcoDynClass, char* className)
							 :	TChlamysFarreriV8(APEcoDynClass, className)
{
    BuildChlamysFarreriIndividual();
}


void TChlamysFarreriIndividual::BuildChlamysFarreriIndividual()
{
    PreBuildChlamysFarreriIndividual();
    ReadParameters();
    PostBuildChlamysFarreriIndividual();
}

void TChlamysFarreriIndividual::PreBuildChlamysFarreriIndividual()
{
   //MyParsRecord = new ParsRecord[NumberOfParameters];
   NumberOfIndividuals = (int)ZooClassDensity[0];
   MyIndividualRecord = new IndividualRecord[NumberOfIndividuals];
   //Este modelo ser? em princ?pio 0D
   //por isso posso preencher o individual record s? para uma "caixa"
}

void TChlamysFarreriIndividual::ReadParameters()
{
    // Read in the parameters
    NumberOfParameterRecords = 0;
//	MyPReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	MyPReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("ChlamysFarreri");
	if (MyPReadWrite != NULL)
	{
        double MyValue;
        int X, Y;

        if (MyPReadWrite->FindString("Chlamys farreri", X, Y))
        {
            MyPReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            MyParsRecord = new ParsRecord[NumberOfParameters - 1];
            //  parameters' names read in "Chlamys farreri.cpp"
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            int index;
            index = -1;
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                MyPReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Allometric coefficient for feeding");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    //Debugger(index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */

                }
                else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Allometric coefficient for respiration");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment);  */
                }
                else if (strcmp(MyParameter, "Meat energetic contents") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Meat energetic contents");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "Shell energetic contents") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Shell energetic contents");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Intrinsic rate of mortality");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment);*/
                }
                else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Summer Intrinsic rate of mortality");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "Maintenance respiration") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Maintenance respiration");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment);  */
                }
                else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Activity respiration coefficient");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment);*/
                }
                else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Proportion of energy allocated to shell");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Proportion of energy allocated to soft tissue");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"umol NH4 to Joules");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"Energetic ratio between soft tissue and soft plus shell tissue");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment);*/
                }
                else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
                {
                    index++;
                    strcpy(MyParsRecord[index].ParameterName,"TotalFW to meatDW");
                    MyPReadWrite->ReadNumber(X+3, i, MyParsRecord[index].Value);
                    MyParsRecord[index].VariableValue = ReadParsRecord(X, i, index);
                    /*Debugger(MyParsRecord[index].Value);
                    Debugger(MyParsRecord[index].HigherValue);
                    Debugger(MyParsRecord[index].LowerValue);
                    Debugger(MyParsRecord[index].Increment); */
                }
                else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
                {
                    MyPReadWrite->ReadNumber(X+3, i, IndividualBasedVariables);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in C. farreri parameters file", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TChlamysFarreriIndividual::ReadParameters - Undefined parameters in C. farreri parameters file" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)MyPReadWrite);
    }
}



void TChlamysFarreriIndividual::PostBuildChlamysFarreriIndividual()
{
    randomize();
    for (int  i = 0; i < NumberOfIndividuals; i++)
    {
        //Debugger(i);
        MyIndividualRecord[i].IRPHY = 0.0; MyIndividualRecord[i].IRDET = 0.0;
        MyIndividualRecord[i].IRPIM = 0.0; MyIndividualRecord[i].IRTPM = 0.0;
        MyIndividualRecord[i].FRTPM = 0.0; MyIndividualRecord[i].FRPIM = 0.0;
        MyIndividualRecord[i].RRTPM = 0.0; MyIndividualRecord[i].RRPIM = 0.0;
        MyIndividualRecord[i].OIR = 0.0; MyIndividualRecord[i].OCI = 0.0;
        MyIndividualRecord[i].OCPF = 0.0; MyIndividualRecord[i].NAEIO = 0.0;
        MyIndividualRecord[i].NEA = 0.0; MyIndividualRecord[i].NEB = 0.0;
        MyIndividualRecord[i].ZooIndividualWeight = ZooIndividualWeight[0];
        MyIndividualRecord[i].ShellLength = ShellLength[0];
        MyIndividualRecord[i].ZooShellMass = ZooShellMass[0];
        MyIndividualRecord[i].ZooIndividualFlux = ZooIndividualFlux[0];
        MyIndividualRecord[i].ZooIndividualFluxForTheShell = ZooIndividualFluxForTheShell[0];
        MyIndividualRecord[i].SoftTissueEnergeticTotal = SoftTissueEnergeticTotal[0];
        MyIndividualRecord[i].ShellEnergeticTotal = ShellEnergeticTotal[0];
        MyIndividualRecord[i].ZooCR = 0.0;
        for (int j = 0; j < NumberOfParameterRecords; j++)
        {
            MyIndividualRecord[i].IndividualParameters[j] = MyParsRecord[j];

            if (MyIndividualRecord[i].IndividualParameters[j].HigherValue -
                MyIndividualRecord[i].IndividualParameters[j].LowerValue > 0.0)
            {
                if (!MyIndividualRecord[i].IndividualParameters[j].RandomizeFlag)  // Sistematic increment of parameter value from its minimum till its maximum
                {
                    //Debugger(i); Debugger(j);
                    if (i == 0) //First individual gets the lower value
                    {
                        MyIndividualRecord[i].IndividualParameters[j].Value = MyIndividualRecord[i].IndividualParameters[j].LowerValue;
                        //Debugger(i);
                        //Debugger(MyIndividualRecord[i].IndividualParameters[j].Value);
                    }
                    else
                    {
                        MyIndividualRecord[i].IndividualParameters[j].Value =
                            (MyIndividualRecord[i].IndividualParameters[j].HigherValue - MyIndividualRecord[i].IndividualParameters[j].LowerValue) /
                            (NumberOfIndividuals - 1) + MyIndividualRecord[i - 1].IndividualParameters[j].Value;
                        //Debugger(i);
                        //Debugger(MyIndividualRecord[i].IndividualParameters[j].Value);
                    }
                }
                else //Random generation of parameter values between lower and higher limits
                {
                    double ARandomValue; double MaximumRandomValue = 10000;
                    ARandomValue = random(MaximumRandomValue);
                    //Debugger(i); Debugger(j);
                    //ARandomValue = rand() % MaximumRandomValue;
                    MyIndividualRecord[i].IndividualParameters[j].Value =
                        MyIndividualRecord[i].IndividualParameters[j].LowerValue +
                        (MyIndividualRecord[i].IndividualParameters[j].HigherValue - MyIndividualRecord[i].IndividualParameters[j].LowerValue) *
                        ARandomValue / MaximumRandomValue;
                    //Debugger(MyIndividualRecord[i].IndividualParameters[j].Value);
                }
            }
        }
    }
}

TChlamysFarreriIndividual::~TChlamysFarreriIndividual()
{
    freeMemory();
}

void TChlamysFarreriIndividual::freeMemory()
{
    try {
        if (NumberOfParameters > 0)
            delete [] MyParsRecord;
        if (NumberOfIndividuals > 0)
            delete [] MyIndividualRecord;
        //delete [] VariableNameArray;
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        Application->ShowException(&e);
    }
    catch (Exception &exception) {
        Application->ShowException(&exception);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TChlamysFarreriIndividual::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TChlamysFarreriIndividual::GetParameterValue(char* MyParameter)
{
  double value = 0.0;
  int index;

    // parameters from TChlamysFarreri
    if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        value = DryWeightOfAStandardAnimal;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        value = AllometricCoefficientForFeeding;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        value = AllometricCoefficientForRespiration;
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        value = MeatEnergeticContents;
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        value = ShellEnergeticContents;
    }
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
    {
        value = IntrinsicRateOfMortality;
    }
    else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
    {
        value = SummerIntrinsicRateOfMortality;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration;
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        value = ActivityRespirationCoefficient;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        value = ProportionOfEnergyAllocatedToShell;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        value = ProportionOfEnergyAllocatedToSoftTissue;
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        value = AmmoniaToJoules;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        value = SoftTissueEnergyToTotalEnergy;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        value = NumberOfClasses;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        value = TotalFWToMeatDW;
    }

    // parameters from TChlamysFarreriIndividual
    else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
    {
        value = IndividualBasedVariables;
    }
    else
    {
        for (index = 0; index < NumberOfParameters; index++)
        {
            if (strcmp(MyParameter, MyParsRecord[index].ParameterName) == 0) {
                value = MyParsRecord[index].Value;
                break;
            }
        }
    }
    return value;
}

bool TChlamysFarreriIndividual::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int index;

    // parameters from TChlamysFarreri
    if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
    {
        DryWeightOfAStandardAnimal = value;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
    {
        AllometricCoefficientForFeeding = value;
    }
    else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
    {
        AllometricCoefficientForRespiration = value;
    }
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
    {
        MeatEnergeticContents = value;
    }
    else if (strcmp(MyParameter, "Shell energetic contents") == 0)
    {
        ShellEnergeticContents = value;
    }
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
    {
        IntrinsicRateOfMortality = value;
    }
    else if (strcmp(MyParameter, "Summer Intrinsic rate of mortality") == 0)
    {
        SummerIntrinsicRateOfMortality = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        MaintenanceRespiration = value;
    }
    else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
    {
        ActivityRespirationCoefficient = value;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
    {
        ProportionOfEnergyAllocatedToShell = value;
    }
    else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
    {
        ProportionOfEnergyAllocatedToSoftTissue = value;
    }
    else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
    {
        AmmoniaToJoules = value;
    }
    else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
    {
        SoftTissueEnergyToTotalEnergy = value;
    }
    else if (strcmp(MyParameter, "Number of classes") == 0)
    {
        NumberOfClasses = value;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        TotalFWToMeatDW = value;
    }

    // parameters from TChlamysFarreriIndividual
    else if (strcmp(MyParameter, "IndividualBasedVariables") == 0)
    {
        IndividualBasedVariables = value;
    }
    else
    {
        for (index = 0; index < NumberOfParameters; index++)
        {
            if (strcmp(MyParameter, MyParsRecord[index].ParameterName) == 0) {
                MyParsRecord[index].Value = value;
                break;
            }
        }
        if (index == NumberOfParameters)
            rc = false;
    }

    return rc;
}

// AP, 2007.07.16
bool TChlamysFarreriIndividual::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("ChlamysFarreri");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Chlamys farreri");
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

        // parameters from TChlamysFarreri and TChlamysFarreriIndividual
        if (strcmp(ParametersNameArray[i], "Number of classes") == 0)
        {
            PReadWrite->WriteCell(NumberOfClasses, 0);
        }
        else if (strcmp(ParametersNameArray[i], "Dry weight of a standard animal") == 0)
        {
            PReadWrite->WriteCell(DryWeightOfAStandardAnimal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for feeding") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForFeeding, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for respiration") == 0)
        {
            PReadWrite->WriteCell(AllometricCoefficientForRespiration, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
            PReadWrite->WriteCell(MeatEnergeticContents, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Shell energetic contents") == 0)
        {
            PReadWrite->WriteCell(ShellEnergeticContents, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Summer Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(SummerIntrinsicRateOfMortality, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Activity respiration coefficient") == 0)
        {
            PReadWrite->WriteCell(ActivityRespirationCoefficient, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to shell") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToShell, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to soft tissue") == 0)
        {
            PReadWrite->WriteCell(ProportionOfEnergyAllocatedToSoftTissue, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "umol NH4 to Joules") == 0)
        {
            PReadWrite->WriteCell(AmmoniaToJoules, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            PReadWrite->WriteCell(SoftTissueEnergyToTotalEnergy, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
            for (int index = 0; index < NumberOfParameters; index++)
            {
                if (strcmp(ParametersNameArray[i], MyParsRecord[index].ParameterName) == 0) {
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].Value, 8); // default value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].HigherValue, 8);  // higher value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].LowerValue, 8);  // lower value
                    PReadWrite->WriteSeparator();
                    PReadWrite->WriteCell(MyParsRecord[index].RandomizeFlag ? 1 : 0, 0);  // randomize flag
                    break;
                }
            }
        }
        else if (strcmp(ParametersNameArray[i], "IndividualBasedVariables") == 0)
        {
            PReadWrite->WriteCell(IndividualBasedVariables, 0);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


bool TChlamysFarreriIndividual::ReadParsRecord(int X, int i, int index)
{
/*    if (    //O record de par?metros s? ser? preenchido devidamente se para todos os par?metros
            //existirem valores para os limites inferior e superior e para a flag que determina
            //se a gera??o de par?metros ? aleat?ria ou n?o (boolenana que pode assumir valor 0 ou 1
            //Se uma destas condi??es n?o se verificar, o modelo usar? par?metros iguais para todos os
            //ind?viduos
            MyPReadWrite->ReadNumber(X+5, i, MyParsRecord[index].HigherValue)
            && MyPReadWrite->ReadNumber(X+6, i, MyParsRecord[index].LowerValue)
            && MyPReadWrite->ReadBool(X+7, i, MyParsRecord[index].RandomizeFlag)
        )*/
            MyPReadWrite->ReadNumber(X+5, i, MyParsRecord[index].HigherValue);
            MyPReadWrite->ReadNumber(X+6, i, MyParsRecord[index].LowerValue);
            MyPReadWrite->ReadBool(X+7, i, MyParsRecord[index].RandomizeFlag);
    {
        NumberOfParameterRecords++;
        /*if (!MyParsRecord[index].RandomizeFlag)
        {
            return MyPReadWrite->ReadNumber(X+8, i, MyParsRecord[index].Increment);
        } */
        return true;
    }
    return false;
}


void TChlamysFarreriIndividual::Go()
{
//    double result;
    //result = MyIndividualRecord[0].SoftTissueEnergeticTotal;
///	double MyYear;
	JulianDay = MyPEcoDynClass->GetJulianDay();
//    MyYear = MyPEcoDynClass->GetYear();

	int contador = 0;
	double MyDepth, MyChlorophyllCarbonRatio;
    TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
   			    * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer()/*,
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer()*/;
    if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
    {
        for (int  bivalve = 0; bivalve < NumberOfIndividuals; bivalve++)
        {
            UpdateIndividualParameters(bivalve);
	        for (int i = 0; i < NumberOfBoxes; i++)
	        {
		        ZooDINRelease[i] = 0;
		        ZooPhytoUptake[i] = 0;
		        ZooPOCUptake[i] = 0;
                OrganicPseudoFaeces[i] = 0.0;
   	            InorganicPseudofaeces[i] = 0.0;
                ZooIndividualWeight[i] = MyIndividualRecord[bivalve].ZooIndividualWeight;
                SoftTissueEnergeticTotal[i] = MyIndividualRecord[bivalve].SoftTissueEnergeticTotal;
	        }
   	        for (int i = 0; i < NumberOfBoxes; i++)
            {
      	        /*if (MyTransportPointer != NULL)
         	        MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
			  											  i,
			 											  "Box depth",
														  ObjectCode);
                else
         	        MyDepth = MyPEcoDynClass->GetBoxDepth(i); *

                if (MyDepth > 0.0)
                {       */
                contador++;
   			    seston_mass = 0.0; phyto_mass = 0.0;
      		    PHYORG = 0.0; DETORG = 0.0;
      		    TPM = 0.0; POM = 0.0; PIM = 0.0;
      		    PHYCONTENTSPOM = 0.0;
      		    TOTALFRTPM[i] = 0.0; TOTALFRPOM[i] = 0.0; TOTALFRPHY[i] = 0.0;
                IRPHY[i] = 0.0; IRDET[i] = 0.0; IRTPM[i] = 0.0; IRPIM[i] = 0.0;
                FRTPM[i] = 0.0; FRPIM[i] = 0.0;
                RRTPM[i] = 0.0; RRPIM[i] = 0.0;
                OIR[i] = 0.0;  OCI[i] = 0.0; OCPF[i] = 0.0;
                NAEIO[i] = 0.0;
         	    NEA[i] = 0.0; NEB[i] = 0.0;

         	    MySPMPointer->Inquiry(GetEcoDynClassName(), TPM,
            	                  i,"TPM",ObjectCode);
         	    MySPMPointer->Inquiry(GetEcoDynClassName(), POM,
            		              i,"POM",ObjectCode);
         	    PIM = TPM - POM;
      		    MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,
                                    i,"Phytoplankton biomass",
                  		            ObjectCode);
         	    MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,
            		                i,"Chlorophyll to biomass", ObjectCode);
                MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllCarbonRatio,
									i, "Chlorophyll : Carbon", ObjectCode);
         	    // Phytoplankton returns its mass as ug chl a l-1
         	    // convert to mg l-1 of organic dry matter
         	    PHYORG = phyto_mass / CUBIC * ChlorophyllToBiomass; //mg/l
      		    DETORG = POM - PHYORG;
      		    if (TPM > 0.0000000000000001)
      			    OCS = POM / TPM;
      		    else
      			    OCS = 0.0;
      		    if (POM > 0.0000000000000001)
      			    PHYCONTENTSPOM = PHYORG / POM;
                if (PHYCONTENTSPOM > 1.0)
                    PHYCONTENTSPOM = 1.0;
      		    for (int k = 0; k < NumberOfClasses; k++)
                {
                    if (
                        ( ZooClassBiomass[k * NumberOfBoxes + i] > 0.0) &&
                        ( ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
                        ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
                    )
                    {
                        Feed(i, k);
                        //TOTALFRTPM[i] = TOTALFRTPM[i] + (FRTPM[i] - FRPHY[i]) * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth / CUBIC;
                        //TOTALFRPOM[i] = TOTALFRPOM[i] + (FRPOM[i] - FRPHY[i]) * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth / CUBIC;
                        //Subtra? o FRPHY no c?lculo do TOTALFRTPM e TOTALFRPOM porque o objecto phytoplankton de Sango invoca o Update do objecto de
                        //Mat?ria em suspens?o, enviando-lhe o seu fluxo final. Neste fluxo est? integrada a perda do fito por consumo pelos bivalves
                        // que ? o TOTALFRPHY. Deste modo, a subtrac??o acima referida evita que a massa de fito consumida pelos bivalves, seja
                        //subttra?da 2 vezes ao TPM e ao POM

                        /*TOTALFRPHY[i] = TOTALFRPHY[i] + FRPHY[i]    //mg Org / ind / s
               	   	                     / ChlorophyllToBiomass   //mg Chl / indiv./ s
                                             / MyChlorophyllCarbonRatio       //mg C / indiv./ s
                                             * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth; //mg C / m3 / s */
                        Respire(i, k);
                        Excrete(i, k);
                        Pseudofaeces(i, k);
                        Assimilate(i, k);
                        Reproduce(i, k);
                        ShellDeposition(i, k);
                        SoftTissueGrowth(i, k);
                        //Die(i, k);
                        //}
                    }
                }
                /*if (MySPMPointer != NULL)
                {
      		        MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRTPM[i],
            	                   i,
               	                "TPM",
                  	             ObjectCode);

         	        MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRPOM[i],
            	                   i,
               	                "POM",
                  	             ObjectCode);
                }
                if (MyPhytoPointer != NULL)
                {
      		        MyPhytoPointer->Update(GetEcoDynClassName(), -TOTALFRPHY[i], //ug Chla /l
            	                      i,
               	                   "Phytoplankton biomass",
                  	                ObjectCode);
                } */

                UpdateIndividualVariables(bivalve, i);
                ZooIndividualFlux[i] = 0.0; ZooIndividualFluxForTheShell[i] = 0.0;

            }
        }//bivalve
    }
    else
#ifdef __BORLANDC__
        MessageBox(0, "Bivalves can not run without phytoplankton and suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
				cerr << "TChlamysFarreriIndividual::Go - Bivalves can not run without phytoplankton and suspended matter" << endl;
#endif  // __BORLANDC__
}



void TChlamysFarreriIndividual::UpdateIndividualVariables(int ABivalve, int ABox)
{
    MyIndividualRecord[ABivalve].IRPHY = IRPHY[ABox];
    MyIndividualRecord[ABivalve].IRDET = IRDET[ABox]; MyIndividualRecord[ABivalve].IRPIM = IRPIM[ABox];
    MyIndividualRecord[ABivalve].IRTPM = IRTPM[ABox]; MyIndividualRecord[ABivalve].FRTPM = FRTPM[ABox];
    MyIndividualRecord[ABivalve].FRPIM = FRPIM[ABox];
    MyIndividualRecord[ABivalve].RRTPM = RRTPM[ABox]; MyIndividualRecord[ABivalve].RRPIM = RRPIM[ABox];
    MyIndividualRecord[ABivalve].OIR = OIR[ABox]; MyIndividualRecord[ABivalve].OCI = OCI[ABox];
    MyIndividualRecord[ABivalve].OCPF = OCPF[ABox]; MyIndividualRecord[ABivalve].NAEIO = NAEIO[ABox];
    MyIndividualRecord[ABivalve].NEA = NEA[ABox]; MyIndividualRecord[ABivalve].NEB = NEB[ABox];
    MyIndividualRecord[ABivalve].ZooIndividualFlux = ZooIndividualFlux[ABox];
    MyIndividualRecord[ABivalve].ZooIndividualFluxForTheShell = ZooIndividualFluxForTheShell[ABox];
    MyIndividualRecord[ABivalve].ZooCR = ClearanceRate[ABox];


    /*MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal = MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal +
                                                            MyIndividualRecord[ABivalve].ZooIndividualFlux * ATimeStep;
    MyIndividualRecord[ABivalve].ShellEnergeticTotal = MyIndividualRecord[ABivalve].ShellEnergeticTotal +
                                                       MyIndividualRecord[ABivalve].ZooIndividualFluxForTheShell * ATimeStep;
    MyIndividualRecord[ABivalve].ZooIndividualWeight = MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal /
                                                        MeatEnergeticContents;
    MyIndividualRecord[ABivalve].ZooShellMass = MyIndividualRecord[ABivalve].ShellEnergeticTotal /
                                                ShellEnergeticContents;
    MyIndividualRecord[ABivalve].ShellLength = ShellLenghtCalculation(MyIndividualRecord[ABivalve].ZooShellMass);  */

    /*
    double result;
    try {
            if (
                (MyIndividualRecord[ABivalve].ZooIndividualFlux  < 0.0000000001) ||
                (MyIndividualRecord[ABivalve].ZooIndividualFlux  > 10000000000)
               )
            result = 0.0;
            else
            result = MyIndividualRecord[ABivalve].ZooIndividualFlux * ATimeStep;
         //result = ATemporaryFlux[ABivalve] * ATimeStep;
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(0);
        Debugger(ABivalve);
        Debugger(MyIndividualRecord[ABivalve].ZooIndividualFlux);
        Debugger(ATimeStep);
        Debugger(MyIndividualRecord[ABivalve].ZooIndividualFlux * ATimeStep);
        Debugger(result);
    }
    try {
      MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal = MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal +
                        result;

    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(1);
        Debugger(ABivalve);
        Debugger(MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal);
        Debugger(MyIndividualRecord[ABivalve].ZooIndividualFlux);
        Debugger(MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal +
                 MyIndividualRecord[ABivalve].ZooIndividualFlux * ATimeStep);
    }
    try {
      MyIndividualRecord[ABivalve].ShellEnergeticTotal = MyIndividualRecord[ABivalve].ShellEnergeticTotal +
                                                        MyIndividualRecord[ABivalve].ZooIndividualFluxForTheShell * ATimeStep;
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(2);
    }
    try {
      MyIndividualRecord[ABivalve].ZooIndividualWeight = MyIndividualRecord[ABivalve].SoftTissueEnergeticTotal /
                                                        MeatEnergeticContents;
      //Debugger(MyIndividualRecord[ABivalve].ZooIndividualWeight);
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(3);
    }
    try {
      MyIndividualRecord[ABivalve].ZooShellMass = MyIndividualRecord[ABivalve].ShellEnergeticTotal / ShellEnergeticContents;
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(4);
    }
    try {
      MyIndividualRecord[ABivalve].ShellLength = ShellLenghtCalculation(MyIndividualRecord[ABivalve].ZooShellMass);
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(5);
    } */
}

void TChlamysFarreriIndividual::UpdateIndividualParameters(int ABivalve)
{
    for (int j = 0; j < NumberOfParameterRecords; j++)
    {

        if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Allometric coefficient for feeding") == 0)
        {
            AllometricCoefficientForFeeding = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Allometric coefficient for respiration") == 0)
        {
            AllometricCoefficientForRespiration = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Meat energetic contents") == 0)
        {
            MeatEnergeticContents = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Shell energetic contents") == 0)
        {
            ShellEnergeticContents = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Intrinsic rate of mortality") == 0)
        {
            IntrinsicRateOfMortality = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Summer Intrinsic rate of mortality") == 0)
        {
            SummerIntrinsicRateOfMortality = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Maintenance respiration") == 0)
        {
            MaintenanceRespiration = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Activity respiration coefficient") == 0)
        {
            ActivityRespirationCoefficient = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Proportion of energy allocated to shell") == 0)
        {
            ProportionOfEnergyAllocatedToShell = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Proportion of energy allocated to soft tissue") == 0)
        {
            ProportionOfEnergyAllocatedToSoftTissue = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "umol NH4 to Joules") == 0)
        {
            AmmoniaToJoules = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
            SoftTissueEnergyToTotalEnergy = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
        else if (strcmp(MyIndividualRecord[ABivalve].IndividualParameters[j].ParameterName, "TotalFW to meatDW") == 0)
        {
            TotalFWToMeatDW = MyIndividualRecord[ABivalve].IndividualParameters[j].Value;
        }
    }
}


void TChlamysFarreriIndividual::Integrate()
{
   double Biomass;
   Biomass = 0;
   for (int  bivalve = 0; bivalve < NumberOfIndividuals; bivalve++)
   {
    try {
      MyIndividualRecord[bivalve].SoftTissueEnergeticTotal = MyIndividualRecord[bivalve].SoftTissueEnergeticTotal +
                        MyIndividualRecord[bivalve].ZooIndividualFlux * ATimeStep;;

    }
#ifdef __BORLANDC__
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(1);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TChlamysFarreriIndividual::Integrate - 1 - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
		
    try {
      MyIndividualRecord[bivalve].ShellEnergeticTotal = MyIndividualRecord[bivalve].ShellEnergeticTotal +
                                                        MyIndividualRecord[bivalve].ZooIndividualFluxForTheShell * ATimeStep;
    }
#ifdef __BORLANDC__
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(2);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TChlamysFarreriIndividual::Integrate - 2 - " << exc.what() << endl;
		}
#endif  // __BORLANDC__

    try {
      MyIndividualRecord[bivalve].ZooIndividualWeight = MyIndividualRecord[bivalve].SoftTissueEnergeticTotal /
                                                        MeatEnergeticContents;
    }
#ifdef __BORLANDC__
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(3);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TChlamysFarreriIndividual::Integrate - 3 - " << exc.what() << endl;
		}
#endif  // __BORLANDC__

    try {
      MyIndividualRecord[bivalve].ZooShellMass = MyIndividualRecord[bivalve].ShellEnergeticTotal / ShellEnergeticContents;
    }
#ifdef __BORLANDC__
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(4);
		}
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TChlamysFarreriIndividual::Integrate - 4 - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
    Biomass = Biomass + MyIndividualRecord[bivalve].ZooIndividualWeight + MyIndividualRecord[bivalve].ZooShellMass;

    try {
      MyIndividualRecord[bivalve].ShellLength = ShellLenghtCalculation(MyIndividualRecord[bivalve].ZooShellMass);
    }
#ifdef __BORLANDC__
		catch (Exception &exception)
    {
        Application->ShowException(&exception);
        Debugger(5);
    }
#else  // __BORLANDC__
		catch (exception &exc) {
				cerr << "TChlamysFarreriIndividual::Integrate - 5 - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
	
   }
   ZooClassBiomass[0] = Biomass;

}


void TChlamysFarreriIndividual::Inquiry(char* srcName, double &Value,
												int BivalveNumber, int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	//Debugger(ClearanceRate[20]);
	int i,j;
	char MyParameter[65], // 64 characters (max) for parameter name
					Myjstring[5],
					MyName[65];
	int MyBivalveNumber = BivalveNumber;
    int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	for (i = 0; i < NumberOfVariables; i++)
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{
			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "C. farreri CR " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = MyIndividualRecord[MyBivalveNumber].ZooCR;
			}
			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "C. farreri meat weight " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = MyIndividualRecord[MyBivalveNumber].ZooIndividualWeight;
			}
            for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "C. farreri shell mass " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = MyIndividualRecord[MyBivalveNumber].ZooShellMass;
			}
			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "Individual growth " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring);
				if (strcmp(MyParameter, MyName) == 0)
					Value = MyIndividualRecord[MyBivalveNumber].ZooIndividualFlux; // Values in g day-1 ind-1
			}
            for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "C. farreri shell length " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring);
				if (strcmp(MyParameter, MyName) == 0)
					Value = MyIndividualRecord[MyBivalveNumber].ShellLength; // Values in cm
			}
            if (strcmp(MyParameter, "IRPHY") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].IRPHY * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "IRDET") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].IRDET * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "IRPIM") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].IRPIM * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "IRTPM") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].IRTPM * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "FRTPM") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].FRTPM * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "FRPIM") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].FRPIM * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "RRTPM") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].RRTPM * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "RRPIM") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].RRPIM * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "OIR") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].OIR * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "OCI") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].OCI * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "OCPF") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].OCPF;
            }
            else if (strcmp(MyParameter, "NAEIO") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].NAEIO;
            }
            else if (strcmp(MyParameter, "NEA") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].NEA * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "NEB") == 0)
            {
				Value = MyIndividualRecord[MyBivalveNumber].NEB * DAYSTOSECONDS;
            }
            else if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Allometric coefficient for feeding") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Allometric coefficient for respiration") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Meat energetic contents") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Meat energetic contents") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Shell energetic contents") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Shell energetic contents") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Maintenance respiration") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Maintenance respiration") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Activity respiration coefficient") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Proportion of energy allocated to shell") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Proportion of energy allocated to soft tissue") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "umol NH4 to Joules") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
            }
            else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
            {
                for (int j = 0; j < NumberOfParameterRecords; j++)
                {
                    if (strcmp(MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].ParameterName, "TotalFW to meatDW") == 0)
                    Value = MyIndividualRecord[MyBivalveNumber].IndividualParameters[j].Value;
                }
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
				  "EcoDynamo Alert - Inquiry 1",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TChlamysFarreriIndividual::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
}


VNA* TChlamysFarreriIndividual::Inquiry(char* srcName,int &Individuals, int &DimVariables)
{
    VNA* MyVarNameArray;

    Individuals = NumberOfIndividuals;
    DimVariables = IndividualBasedVariables;
    MyVarNameArray = new VNA[IndividualBasedVariables];
    for (int i = 0; i < IndividualBasedVariables; i++)
    {
        strcpy(MyVarNameArray[i], VariableNameArray[i]);
    }
    LogMessage("Inquiry", srcName, MyVarNameArray[0], Individuals, DimVariables);
    return MyVarNameArray;
}



void TChlamysFarreriIndividual::Inquiry(char* srcName, double &Value,
										 int BoxNumber,
										 char* ParameterName,
										 int AnObjectCode)
{
    int i,j;
	char MyParameter[65], // 64 characters (max) for parameter name
					Myjstring[5],
					MyName[65];
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	for (i = 0; i < NumberOfVariables; i++)
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{
			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "C. farreri density " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
			}

			for (j = 0; j < NumberOfClasses; j++)
			{
				strcpy( MyName, "C. farreri biomass " );
				sprintf( Myjstring , "%i", j + 1 );
				strcat( MyName, Myjstring );
				if (strcmp(MyParameter, MyName) == 0)
					Value = ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
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
				  "EcoDynamo Alert - Inquiry 2",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
		cerr << "TChlamysFarreriIndividual::Inquiry 2 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__

		Value = 0;
}

bool TChlamysFarreriIndividual::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
    bool rc = true;
	char MyName[65]; // 64 characters (max) for parameter name

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strstr(VariableName, "C. farreri density ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. farreri density %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooClassDensity[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. farreri biomass ") != NULL)
    {
        for (int j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. farreri biomass %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else
        rc = false;

    return rc;
}

void TChlamysFarreriIndividual::Feed(int ABox, int AClass)
{
	int MyBox, MyClass;
   double PHYCNFPOM, RRFRPHYORG;

   MyBox = ABox;
   MyClass = AClass;

   //Detritus filtration, rejection and ingestion

   if(DETORG > 0.0001)
   {
   	FRDET[MyBox] = (0.542 + 0.586 * DETORG)* TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding)
               / HOURSTOSECONDS; //mg org / s  / ind
      /*if (FRDET[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > DETORG)
      FRDET[MyBox]= DETORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;  */
      if (FRDET[MyBox] < 0.0) FRDET[MyBox] = 0.0;
   }
   else
      FRDET[MyBox] = 0.0;


   if (FRDET[MyBox] * HOURSTOSECONDS > 0.0194)
      RRDET[MyBox] = (-0.00674 + 0.348 * FRDET[MyBox] * DAYSTOSECONDS) / DAYSTOSECONDS;
   else RRDET[MyBox] = 0.0;
   IRDET[MyBox] = FRDET[MyBox] - RRDET[MyBox];

   //Phytoplankton filtration

   if((POM + 3.0 > exp(-1)) && (PHYORG > 0.0001))
   {
   	FRPHY[MyBox] = (exp(7.5856 + (18.11*(log(log(POM + 3.0))))+(-10.5*(log(POM + 3.0)))+(0.6919*PHYORG))*
               TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding))
               / HOURSTOSECONDS; //mg phy-org / s / ind
      /*if (FRPHY[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > PHYORG)
      FRPHY[MyBox]= PHYORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;*/
      if (FRPHY[MyBox] < 0.0) FRPHY[MyBox] = 0.0;
   }
   else
      FRPHY[MyBox] = 0.0;


   // POM filtration
   FRPOM[MyBox] = FRDET[MyBox] + FRPHY[MyBox];

   //Phytoplankton rejection and ingestion
   PHYCNFPOM = FRPHY[MyBox] / FRPOM[MyBox];

   RRFRPHYORG = 1.0 - 0.895 * PHYCNFPOM;
   if (RRFRPHYORG < 0.0) RRFRPHYORG = 0.0;

   RRPHY[MyBox] = FRPHY[MyBox] * RRFRPHYORG;

   IRPHY[MyBox] = FRPHY[MyBox] - RRPHY[MyBox];

   //PIM filtration, rejection and ingestion

   if (PIM > 0.0001)
   {
   	FRPIM[MyBox] = (19.06 * (1.0 - exp(-0.110 * (PIM - 1.87))) *
      	            TemperatureEffectsOnFeedingRate(MyBox) *
         	         AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding))
                     / HOURSTOSECONDS;   //mg / s / ind
      /*if (FRPIM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > PIM)
      FRPIM[MyBox]= PIM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep; */
      if (FRPIM[MyBox] < 0.0) FRPIM[MyBox] = 0.0;
   }
   else
   	FRPIM[MyBox] = 0.0;


   RRPIM[MyBox] = (-0.841 + 0.936 * FRPIM[MyBox] * DAYSTOSECONDS) / DAYSTOSECONDS; //mg / s
   if (RRPIM[MyBox] < 0.0)
   RRPIM[MyBox] = 0.0;

   IRPIM[MyBox] = FRPIM[MyBox] - RRPIM[MyBox];

   //TPM filtration, rejection and ingestion

   FRTPM[MyBox] = FRDET[MyBox] + FRPHY[MyBox] + FRPIM[MyBox];
   RRTPM[MyBox] = RRDET[MyBox] + RRPHY[MyBox] + RRPIM[MyBox];
   IRTPM[MyBox] = IRDET[MyBox] + IRPHY[MyBox] + IRPIM[MyBox];

   if (TPM > 0.0000000000000001)
   	ClearanceRate[MyClass * NumberOfBoxes + MyBox] = FRTPM[MyBox] / TPM * DAYSTOSECONDS; //L / d / ind
   else
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;

   OIR[MyBox] = IRPHY[MyBox] + IRDET[MyBox]; //mg / d

   if (IRTPM[MyBox] > 0.0000000000000001)
      OCI[MyBox] =  (IRDET[MyBox] + IRPHY[MyBox]) / IRTPM[MyBox]; //fraction
   else
      OCI[MyBox] = 0.0;

   if (OCI[MyBox] > 0.0000000000000001)
      OCPF[MyBox] = OCS * OCS / OCI[MyBox];  //fraction
   else
      OCPF[MyBox] = 0.0;
   if (OCPF[MyBox] > 1.0)
   	OCPF[MyBox] = 1.0;

   if (OCI[MyBox] > 0.0000000000000001)
   	NAEIO[MyBox] = 1.12 - 0.129 * 1.0 / OCI[MyBox]; //fraction
   else
      NAEIO[MyBox] = 0.0;
   if (NAEIO[MyBox] < 0.0)
   NAEIO[MyBox] = 0.0;

   NEA[MyBox] = ((23.5 * IRPHY[MyBox]) +(6.1 * IRDET[MyBox])) * NAEIO[MyBox]; //J /d / ind
}


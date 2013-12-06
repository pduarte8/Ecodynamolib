// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 01.08.05
/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#include <math.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "zoobobjt.h"
#include "iodll.h"


TCrassostreaGigas5::TCrassostreaGigas5(TEcoDynClass* APEcoDynClass, char* className)
							 :	TBivalvesForSango(APEcoDynClass, className)
{
	DryWeightOfAStandardAnimal = 1.0;
   ProportionOfEnergyAllocatedToShell = 0.28;
   ProportionOfEnergyAllocatedToSoftTissue = 0.72;


	ParameterCount = 0;



   double MyValue;
   int X, Y;
   // Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("CrassostreaGigas");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "CrassostreaGigas parameters file missing.", "EcoDynamo alert", MB_OK);
#else // __BORLANDC__
		cerr << "TCrassostreaGigas5::TCrassostreaGigas5 - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
    /*
             if (strcmp(MyParameter, "Columns") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, Columns);
                //Debugger(Columns);
             }
             else
             if (strcmp(MyParameter, "Lines") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, Lines);
                //Debugger(Lines);
             }
             else*/
             if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, DryWeightOfAStandardAnimal);
                //Debugger(DryWeightOfAStandardAnimal);
             }
             else
             if (strcmp(MyParameter, "Allometric coefficient for feeding") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, AllometricCoefficientForFeeding);
                //Debugger(AllometricCoefficientForFeeding);
             }
             else
             if (strcmp(MyParameter, "Allometric coefficient for respiration") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, AllometricCoefficientForRespiration);
                //Debugger(AllometricCoefficientForRespiration);
             }
             else
             if (strcmp(MyParameter, "Meat energetic contents") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, MeatEnergeticContents);
                //Debugger(MeatEnergeticContents);
             }
             else
             if (strcmp(MyParameter, "Shell energetic contents") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, ShellEnergeticContents);
                //Debugger(ShellEnergeticContents);
             }
             else
             if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, IntrinsicRateOfMortality);
                //Debugger(IntrinsicRateOfMortality);
             }
             else
             if (strcmp(MyParameter, "Maintenance respiration") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, MaintenanceRespiration);
                //Debugger(MaintenanceRespiration);
             }
             else
             if (strcmp(MyParameter, "Activity respiration coefficient") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, ActivityRespirationCoefficient	);
                //Debugger(ActivityRespirationCoefficient);
             }
             else
             if (strcmp(MyParameter, "Proportion of energy allocated to shell") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, ProportionOfEnergyAllocatedToShell);
                //Debugger(ProportionOfEnergyAllocatedToShell);
             }
             else
             if (strcmp(MyParameter, "Proportion of energy allocated to soft tissue") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, ProportionOfEnergyAllocatedToSoftTissue);
                //Debugger(ProportionOfEnergyAllocatedToSoftTissue);
             }
             else
             if (strcmp(MyParameter, "umol NH4 to Joules") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, AmmoniaToJoules);
                //Debugger(AmmoniaToJoules);
             }
             else
                if (strcmp(MyParameter, "Number of classes") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, NumberOfClasses);
                //Debugger(NumberOfClasses);
             }
             else
             if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, TotalFWToMeatDW);
                //Debugger(TotalFWToMeatDW);
             }
             else
             if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
             {
                    PReadWrite->ReadNumber(X+3, i, SoftTissueEnergyToTotalEnergy);
                //Debugger(SoftTissueEnergyToTotalEnergy);
             }
            }
        }
       else
       {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in C. gigas parameters file", "EcoDynamo alert", MB_OK);
#else // __BORLANDC__
						cerr << "TCrassostreaGigas5::TCrassostreaGigas5 - Undefined parameters in C. gigas parameters file" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }


/*   Generic                 = new double[NumberOfBoxes * NumberOfClasses];
   GenericFlux             = new double[NumberOfBoxes * NumberOfClasses];*/
	ZooClassBiomass         = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensity         = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualWeight     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassBiomassFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensityFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualFlux       = new double[NumberOfBoxes * NumberOfClasses];
   ClassMaxLimits          = new double[NumberOfBoxes * NumberOfClasses];
   ClassMinLimits          = new double[NumberOfBoxes * NumberOfClasses];
	Mortality               = new double[NumberOfBoxes * NumberOfClasses];
   ZooShellMass            = new double[NumberOfBoxes * NumberOfClasses];
   ClearanceRate           = new double[NumberOfBoxes * NumberOfClasses];

   IRPHY                   = new double[NumberOfBoxes];
   IRDET                   = new double[NumberOfBoxes];
   FRTPM                   = new double[NumberOfBoxes];
   IRTPM                   = new double[NumberOfBoxes];
   RRTPM                   = new double[NumberOfBoxes];
   FRPIM                   = new double[NumberOfBoxes];
   IRPIM                   = new double[NumberOfBoxes];
   RRPIM                   = new double[NumberOfBoxes];
   OIR                     = new double[NumberOfBoxes];
   OCI                     = new double[NumberOfBoxes];
   OCPF                    = new double[NumberOfBoxes];
   NAEIO                   = new double[NumberOfBoxes];
   NEA                     = new double[NumberOfBoxes];
   NEB                     = new double[NumberOfBoxes];
   TOTALFRTPM              = new double[NumberOfBoxes];
   TOTALFRPOM              = new double[NumberOfBoxes];
   TOTALFRPHY              = new double[NumberOfBoxes];
   NOSE                    = new double[NumberOfBoxes];
   IRORG                   = new double[NumberOfBoxes];
   FRPHY                   = new double[NumberOfBoxes];
   FRPOM                   = new double[NumberOfBoxes];

   SoftTissueEnergeticTotal = new double[NumberOfBoxes * NumberOfClasses];
   ShellEnergeticTotal = new double[NumberOfBoxes * NumberOfClasses];
   ZooIndividualFluxForTheShell = new double[NumberOfBoxes * NumberOfClasses];
   OrganicPseudoFaeces     = new double[NumberOfBoxes];
   InorganicPseudofaeces   = new double[NumberOfBoxes];
   ShellLength = new double[NumberOfBoxes * NumberOfClasses];

   for (int m = 0; m < NumberOfClasses; m++)
   {
		for (int n = 0; n < NumberOfBoxes; n++)
		{
/*			Generic[m * NumberOfBoxes + n]                 = 0;
         GenericFlux[m * NumberOfBoxes + n]             = 0;*/
			ZooClassBiomass[m * NumberOfBoxes + n] 	     = 0;
			ZooClassDensity[m * NumberOfBoxes + n]         = 0;
			ZooIndividualWeight[m * NumberOfBoxes + n]     = 0;
			ZooClassBiomassFlux[m * NumberOfBoxes + n]     = 0;
			ZooClassDensityFlux[m * NumberOfBoxes + n]     = 0;
			ZooIndividualFlux[m * NumberOfBoxes + n]       = 0;
			Mortality[m * NumberOfBoxes + n] 	     		  = 0;
         ZooShellMass[m * NumberOfBoxes + n]            = 0;
         SoftTissueEnergeticTotal[m * NumberOfBoxes + n] = 0;
         ShellEnergeticTotal[m * NumberOfBoxes + n] = 0;
         ZooIndividualFluxForTheShell[m * NumberOfBoxes + n] = 0;
         ShellLength[m * NumberOfBoxes + n] = 0;
         ClearanceRate[m * NumberOfBoxes + n] = 0;
		}
   }


   for (int m = 0; m < NumberOfBoxes; m++)
   {
   	OrganicPseudoFaeces[m] = 0.0;
   	InorganicPseudofaeces[m] = 0.0;
      IRPHY[m]= 0;
      IRDET[m]= 0;
      FRTPM[m]= 0;
      IRTPM[m]= 0;
      RRTPM[m] = 0.0;
      FRPIM[m] = 0.0;
      IRPIM[m] = 0.0;
      RRPIM[m] = 0.0;
      OIR[m] = 0.0;
      OCI[m] = 0.0;
      OCPF[m] = 0.0;
      NAEIO[m] = 0.0;
      NEA[m] = 0.0;
      NEB[m] = 0.0;
      TOTALFRTPM[m] = 0.0;
      TOTALFRPOM[m] = 0.0;
      TOTALFRPHY[m] = 0.0;
      NOSE[m] = 0.0;
      IRORG[m] = 0.0;
      FRPHY[m] = 0.0;
      FRPOM[m] = 0.0;
   }
	// Read in the variables
	VariableCount = 0;
    char MyVariable[65];
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("CrassostreaGigas");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "CrassostreaGigas variables file missing.", "EcoDynamo alert", MB_OK);
#else // __BORLANDC__
		cerr << "TCrassostreaGigas5::TCrassostreaGigas5 - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        int XV, YV, offset;
        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
          PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];

            // read in the variable names
            int k, j;
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + 2 + i, Y, VariableNameArray[i]);
                strcpy(MyVariable, VariableNameArray[i]);

            /*if (strcmp(MyVariable, "C. gigas biomass 1") == 0)           //g soft + shell tissue DW / m2
            {
                //Debugger(i); Debugger(Y);
                    for (k = 0; k < NumberOfClasses; k++)
                    {
                        for (j = 0; j < NumberOfBoxes; j++)
                        {
                      PReadWrite->ReadNumber( X + 2 + i + k, Y + 1 + j, MyValue );
                            ZooClassBiomass[k * NumberOfBoxes + j] = MyValue;

                      //if (j == NumberOfBoxes - 1)
                      //Debugger(ZooClassBiomass[k * NumberOfBoxes + j]);
                        }
                    }
              }
             else */
            if (strcmp(MyVariable, "C. gigas shell mass 1") == 0)         //g Shell dry weight / individual
            {
                  if (!PReadWrite->FindString("C. gigas shell mass 1 values", XV, YV))
                  {
                      XV = X + 2 + i;
                      YV = Y;
                  }
                  for (k = 0; k < NumberOfClasses; k++)
                  {
                      offset = k * NumberOfBoxes;
                      for (j = 0; j < NumberOfBoxes; j++)
                      {
                          PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue);
                          ZooShellMass[offset + j] = MyValue;
                          ShellEnergeticTotal[offset + j] = MyValue * ShellEnergeticContents;
                      }
                  }
               }
           else
            if (strcmp(MyVariable, "C. gigas meat weight 1") == 0)        //g Soft tissue dry weight / individual
            {
                  if (!PReadWrite->FindString("C. gigas meat weight 1 values", XV, YV))
                  {
                      XV = X + 2 + i;
                      YV = Y;
                  }
                  for (k = 0; k < NumberOfClasses; k++)
                  {
                      offset = k * NumberOfBoxes;
                    for (j = 0; j < NumberOfBoxes; j++)
                    {
                      PReadWrite->ReadNumber( XV + k, YV + 1 + j, MyValue );
                      ZooIndividualWeight[offset + j] = MyValue;
                      SoftTissueEnergeticTotal[offset + j] = MyValue * MeatEnergeticContents;
                      //if (j == NumberOfBoxes - 1)
                      //Debugger(ZooIndividualWeight[k * NumberOfBoxes + j]);

                    }
                  }
               }
             else
             if (strcmp(MyVariable, "C. gigas density 1") == 0)           //g soft + shell tissue DW / m2
            {
                  if (!PReadWrite->FindString("C. gigas density 1 values", XV, YV))
                  {
                      XV = X + 2 + i;
                      YV = Y;
                  }
                  for (k = 0; k < NumberOfClasses; k++)
                  {
                    offset = k * NumberOfBoxes;
                    for (j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber( XV + k, YV + 1 + j, MyValue );
                        ZooClassDensity[offset + j] = MyValue;
                    }
                  }
              }
            }
        }
       else
       {
#ifdef __BORLANDC__
        MessageBox(0, "Undefined object in C. gigas for Sango", "EcoDynamo alert", MB_OK);
#else // __BORLANDC__
						cerr << "TCrassostreaGigas5::TCrassostreaGigas5 - Undefined object in C. gigas for Sango" << endl;
#endif  // __BORLANDC__
       }
        CloseDataFile((void*)PReadWrite);
    }

   for (int k = 0; k < NumberOfClasses; k++)
   {
   	for (int j = 0; j < NumberOfBoxes; j++)
      {
      	if (ZooIndividualWeight[k * NumberOfBoxes + j] != 0.0)
      		ZooClassBiomass[k * NumberOfBoxes + j] = ZooClassDensity[k * NumberOfBoxes + j] *
                                                     (ZooIndividualWeight[k * NumberOfBoxes + j] + ZooShellMass[k * NumberOfBoxes + j]);
         else
            ZooClassBiomass[k * NumberOfBoxes + j] = 0.0;
      }
   }
   if (NumberOfClasses > 1)
   	ClassIncrement = ClassMinLimits[1] - ClassMinLimits[0];

}


TCrassostreaGigas5::~TCrassostreaGigas5()
{
    freeMemory();
}

void TCrassostreaGigas5::freeMemory()
{
    try {
    /*	delete [] Generic;
       delete [] GenericFlux;*/
        delete [] ZooClassBiomass;
        delete [] ZooClassDensity;
    //	delete [] ZooInitialWeight;
        delete [] ZooIndividualWeight;
        delete [] ZooClassBiomassFlux;
        delete [] ZooClassDensityFlux;
        delete [] ZooIndividualFlux;
        delete [] ClassMaxLimits;
        delete [] ClassMinLimits;
       delete [] Mortality;
       delete [] ZooShellMass;
       delete [] IRPHY;
       delete [] IRDET;
       delete [] FRTPM;
       delete [] IRTPM;
       delete [] RRTPM;
       delete [] FRPIM;
       delete [] IRPIM;
       delete [] RRPIM;
       delete [] OIR;
       delete [] OCI;
       delete [] OCPF;
       delete [] NAEIO;
       delete [] NEA;
       delete [] NEB;
       delete [] TOTALFRTPM;
       delete [] TOTALFRPOM;
       delete [] TOTALFRPHY;
       delete [] SoftTissueEnergeticTotal;
       delete [] ShellEnergeticTotal;
       delete [] OrganicPseudoFaeces;
       delete [] InorganicPseudofaeces;
       delete [] ZooIndividualFluxForTheShell;
       delete [] NOSE;
       delete [] IRORG;
       delete [] FRPHY;
       delete [] ShellLength;
       delete [] ClearanceRate;
       if (NumberOfVariables > 0) {
           delete [] VariableNameArray;
           NumberOfVariables = 0;
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
				cerr << "TCrassotreaGigas5::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TCrassostreaGigas5::GetParameterValue(char* MyParameter)
{
  double value;

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
     else if (strcmp(MyParameter, "Number of classes") == 0)
     {
        value = NumberOfClasses;
     }
     else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
     {
        value = TotalFWToMeatDW;
     }
     else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
     {
        value = SoftTissueEnergyToTotalEnergy;
     }
    else
        value = 0.0;
    return value;
}

bool TCrassostreaGigas5::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

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
     else if (strcmp(MyParameter, "Number of classes") == 0)
     {
        NumberOfClasses = value;
     }
     else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
     {
        TotalFWToMeatDW = value;
     }
     else if (strcmp(MyParameter, "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
     {
        SoftTissueEnergyToTotalEnergy = value;
     }
    else
        rc = false;

    return rc;
}


void TCrassostreaGigas5::Update(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName,
									      int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int j, MyBoxNumber = BoxNumber;
    LogMessage("Update", srcName, VariableName, Value, MyBoxNumber);

    if (strstr(VariableName, "C. gigas shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas shell mass %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooShellMass[NumberOfBoxes * j + MyBoxNumber] = Value;
                ShellEnergeticTotal[NumberOfBoxes * j + MyBoxNumber] =
                        Value * ShellEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas meat weight %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + MyBoxNumber] = Value;
                SoftTissueEnergeticTotal[NumberOfBoxes * j + MyBoxNumber] =
                        Value * MeatEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyParameter, "C. gigas density %i", j + 1 );
            if (strcmp(MyParameter, VariableName) == 0) {
                ZooClassDensityFlux[NumberOfBoxes * j + MyBoxNumber] += Value;
                break;
            }
        }
    }
    else
    {
#ifdef __BORLANDC__
        char Caption[129];
        strcpy(Caption, VariableName);
        strcat(Caption, " does not exist in ");
        strcat(Caption, GetEcoDynClassName());
       //Debugger(AnObjectCode);
        MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Update 3",
                      MB_OK | MB_ICONHAND);
#else // __BORLANDC__
						cerr << "TCrassostreaGigas5::Update 3 - " << VariableName << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__

        Value = 0;
    }
}

bool TCrassostreaGigas5::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
    bool rc = true;
	char MyName[65]; // 64 characters (max) for parameter name
	int j;
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

     if (strstr(VariableName, "C. gigas density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas density %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ZooClassDensity[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas meat weight %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + BoxNumber] = Value;
                SoftTissueEnergeticTotal[NumberOfBoxes * j + BoxNumber] =
                        Value * MeatEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas biomass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas biomass %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas shell mass %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ZooShellMass[NumberOfBoxes * j + BoxNumber] = Value;
                ShellEnergeticTotal[NumberOfBoxes * j + BoxNumber] =
                        Value * ShellEnergeticContents;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Individual growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Individual growth %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ZooIndividualFlux[NumberOfBoxes * j + BoxNumber] = Value; // Values in g day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "Scope for growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Scope for growth %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                GrowthRate[NumberOfBoxes * j + BoxNumber] = Value; // Values in j day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas CR ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas CR %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ClearanceRate[NumberOfBoxes * j + BoxNumber] = Value; // Values in litres ind-1 d-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas shell length ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas shell length %i", j + 1 );
            if (strcmp(MyName, VariableName) == 0) {
                ShellLength[NumberOfBoxes * j + BoxNumber] = Value; // Values in cm
                break;
            }
        }
    }
    else if (strcmp(VariableName, "Zoobenthos DIN Release") == 0)
        ZooDINRelease[BoxNumber] = Value;

    else if (strcmp(VariableName, "Zoobenthos Phyto Uptake") == 0)
        ZooPhytoUptake[BoxNumber] = Value;

    else if (strcmp(VariableName, "Zoobenthos POC Uptake") == 0)
        ZooPOCUptake[BoxNumber] = Value;

    else if (strcmp(VariableName, "IRPHY") == 0)
    {
        IRPHY[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "IRDET") == 0)
    {
        IRDET[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "IRPIM") == 0)
    {
        IRPIM[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "IRTPM") == 0)
    {
        IRTPM[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "FRTPM") == 0)
    {
        FRTPM[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "FRPIM") == 0)
    {
        FRPIM[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "RRTPM") == 0)
    {
        RRTPM[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "RRPIM") == 0)
    {
        RRPIM[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "OIR") == 0)
    {
        OIR[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "OCI") == 0)
    {
        OCI[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "OCPF") == 0)
    {
        OCPF[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NAEIO") == 0)
    {
        NAEIO[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NEA") == 0)
    {
        NEA[BoxNumber] = Value;
    }
    else if (strcmp(VariableName, "NEB") == 0)
    {
        NEB[BoxNumber] = Value;
    }
    else
        rc = false;

    return rc;
}

void TCrassostreaGigas5::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	int i,j;
	char MyParameter[65], // 64 characters (max) for parameter name
					MyName[65];
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strstr(MyParameter, "C. gigas density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas density %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas meat weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas meat weight %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooIndividualWeight[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas biomass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas biomass %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas shell mass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas shell mass %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooShellMass[NumberOfBoxes * j + MyBoxNumber];
                break;
            }
        }
    }
    else if (strstr(MyParameter, "Individual growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Individual growth %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooIndividualFlux[NumberOfBoxes * j + MyBoxNumber]; // Values in g day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(MyParameter, "Scope for growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Scope for growth %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = GrowthRate[NumberOfBoxes * j + MyBoxNumber]; // Values in j day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas CR ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas CR %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ClearanceRate[NumberOfBoxes * j + MyBoxNumber]; // Values in litres ind-1 d-1
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas shell length ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas shell length %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ShellLength[NumberOfBoxes * j + MyBoxNumber]; // Values in cm
                break;
            }
        }
    }
    else if (strcmp(MyParameter, "Zoobenthos DIN Release") == 0)
        Value = ZooDINRelease[MyBoxNumber];

    else if (strcmp(MyParameter, "Zoobenthos Phyto Uptake") == 0)
        Value = ZooPhytoUptake[MyBoxNumber];

    else if (strcmp(MyParameter, "Zoobenthos POC Uptake") == 0)
        Value = ZooPOCUptake[MyBoxNumber];

    else if (strcmp(MyParameter, "Total biomass") == 0)
    {
        double TotalBiomass;
        TotalBiomass = 0.0;
        for (j = 0; j < NumberOfClasses; j++)
            {
                TotalBiomass = TotalBiomass + ZooClassBiomass[NumberOfBoxes * j + MyBoxNumber];
        }
        Value = TotalBiomass;
    }
    else if (strcmp(MyParameter, "Total density") == 0)
    {
        double TotalDensity;
        TotalDensity = 0.0;
        for (j = 0; j < NumberOfClasses; j++)
            {

                TotalDensity = TotalDensity + ZooClassDensity[NumberOfBoxes * j + MyBoxNumber];
        }
        Value = TotalDensity;
    }
    else if (strcmp(MyParameter, "IRPHY") == 0)
    {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = IRPHY[MyBoxNumber];
        //else Value = 0.0;
    }
    else if (strcmp(MyParameter, "IRDET") == 0)
    {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = IRDET[MyBoxNumber];
        //else Value = 0.0;
    }
    else if (strcmp(MyParameter, "IRPIM") == 0)
    {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = IRPIM[MyBoxNumber];
        //else Value = 0.0;
    }
    else if (strcmp(MyParameter, "IRTPM") == 0)
    {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = IRTPM[MyBoxNumber];
        //else Value = 0.0;
    }
    else if (strcmp(MyParameter, "FRTPM") == 0)
    {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = FRTPM[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "FRPIM") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = FRPIM[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "RRTPM") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = RRTPM[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "RRPIM") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = RRPIM[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "OIR") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = OIR[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "OCI") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = OCI[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "OCPF") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = OCPF[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "NAEIO") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = NAEIO[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "NEA") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = NEA[MyBoxNumber];
        //else Value = 0.0;
     }
     else if (strcmp(MyParameter, "NEB") == 0)
     {
        //if (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBoxNumber] > 0.0)
            Value = NEB[MyBoxNumber];
        //else Value = 0.0;
     }
     else
     {
        Value = 0;
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
                      "EcoDynamo Alert - Inquiry 5",
                      MB_OK | MB_ICONHAND);
#else // __BORLANDC__
						cerr << "TCrassostreaGigas5::Inquiry 5 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


// AP, 2007.07.16
bool TCrassostreaGigas5::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("CrassostreaGigas");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Crassostrea gigas");
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
    PReadWrite->WriteSeparator(true);

    /*
     * Crassostrea gigas initial values
     */
    char classText[16];
    PReadWrite->WriteCell("C. gigas density 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas shell mass 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("C. gigas meat weight 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator(true);

    int offset;
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        // density by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooClassDensity[offset + j], 8);
            PReadWrite->WriteSeparator();
        }

        // shell mass by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooShellMass[offset + j], 8);
            PReadWrite->WriteSeparator();
        }

        // meat weight by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooIndividualWeight[offset + j], 8);
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TCrassostreaGigas5::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("CrassostreaGigas");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Crassostrea gigas");
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
        }
        else if (strcmp(ParametersNameArray[i], "Allometric coefficient for respiration") == 0)
        {
			PReadWrite->WriteCell(AllometricCoefficientForRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
			PReadWrite->WriteCell(MeatEnergeticContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Shell energetic contents") == 0)
        {
			PReadWrite->WriteCell(ShellEnergeticContents, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
			PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
			PReadWrite->WriteCell(MaintenanceRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Activity respiration coefficient") == 0)
        {
			PReadWrite->WriteCell(ActivityRespirationCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to shell") == 0)
        {
			PReadWrite->WriteCell(ProportionOfEnergyAllocatedToShell, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Proportion of energy allocated to soft tissue") == 0)
        {
			PReadWrite->WriteCell(ProportionOfEnergyAllocatedToSoftTissue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "umol NH4 to Joules") == 0)
        {
			PReadWrite->WriteCell(AmmoniaToJoules, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
			PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Energetic ratio between soft tissue and soft plus shell tissue") == 0)
        {
			PReadWrite->WriteCell(SoftTissueEnergyToTotalEnergy, 8);
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TCrassostreaGigas5::Go()
{
//	double MyYear;
	JulianDay = MyPEcoDynClass->GetJulianDay();
//   MyYear = MyPEcoDynClass->GetYear();

	int contador = 0;
	double MyDepth, MyChlorophyllCarbonRatio;

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooDINRelease[i] = 0;
		ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
      OrganicPseudoFaeces[i] = 0.0;
   	InorganicPseudofaeces[i] = 0.0;
	}

	TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
   			  * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
              * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

   if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
   {
   	int i;
   	for (i = 0; i < NumberOfBoxes; i++)
      {
      	if (MyTransportPointer != NULL)
         	MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
			  											  i,
			 											  "Box depth",
														  ObjectCode);
         else
         	MyDepth = MyPEcoDynClass->GetBoxDepth(i);


         if (MyDepth > 0.0)
         {
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
            	                       i,
 												  "TPM",
                  	                 ObjectCode);
         	MySPMPointer->Inquiry(GetEcoDynClassName(), POM,
            		                    i,
               		                 "POM",
                  		              ObjectCode);

         	PIM = TPM - POM;

      		MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,
            		                      i,
               		                   "Phytoplankton biomass",
                  		                ObjectCode);

         	MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,
            		                      i,
               		                   "Chlorophyll to biomass",
                  		                ObjectCode);

            MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllCarbonRatio,
												    i,
												    "Chlorophyll : Carbon",
												    ObjectCode);

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
            /*if (PHYCONTENTSPOM > 1.0)
            {
            	MessageBox(0,"TBivalvesForSango::Go, PHYCONTENTSPOM > 1 ","EcoDynamo Alert",MB_OK | MB_ICONHAND);
               Debugger(i);
            }
            else
         		 PHYCONTENTSPOM = 0.0;  */

      		for (int k = 0; k < NumberOfClasses; k++)
				{
					if (
				 			( ZooClassBiomass[k * NumberOfBoxes + i] > 0.0) &&
				 			( ZooClassDensity[k * NumberOfBoxes + i] > 0.0) &&
				 			( ZooIndividualWeight[k * NumberOfBoxes + i] > 0.0)
						)
					{

            		Feed(i, k);


            		TOTALFRTPM[i] = TOTALFRTPM[i] + (FRTPM[i] - FRPHY[i]) * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth / CUBIC;
            		TOTALFRPOM[i] = TOTALFRPOM[i] + (FRPOM[i] - FRPHY[i]) * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth / CUBIC;

                  //Subtra? o FRPHY no c?lculo do TOTALFRTPM e TOTALFRPOM porque o objecto phytoplankton de Sango invoca o Update do objecto de
                  //Mat?ria em suspens?o, enviando-lhe o seu fluxo final. Neste fluxo est? integrada a perda do fito por consumo pelos bivalves
                  // que ? o TOTALFRPHY. Deste modo, a subtrac??o acima referida evita que a massa de fito consumida pelos bivalves, seja
                  //subttra?da 2 vezes ao TPM e ao POM


            		TOTALFRPHY[i] = TOTALFRPHY[i] + FRPHY[i]    //mg Org / ind / day
               	   	                     / ChlorophyllToBiomass   //mg Chl / indiv./ day
                                             / MyChlorophyllCarbonRatio       //mg C / indiv./ day
                                             * ZooClassDensity[k * NumberOfBoxes + i] / MyDepth; //mg C / m3 / day

                  //if(i == 207) MessageBox(0, "Go","Respire",MB_OK | MB_ICONHAND);

            		Respire(i, k);

                  //if(i == 207) MessageBox(0, "Go","Excrete",MB_OK | MB_ICONHAND);

                  Excrete(i, k);

                  //if(i == 207) MessageBox(0, "Go","Pseudofaeces",MB_OK | MB_ICONHAND);

            		Pseudofaeces(i, k);

                  //if(i == 207) MessageBox(0, "Go","Assimilate",MB_OK | MB_ICONHAND);

            		Assimilate(i, k);

                  //if(i == 207) MessageBox(0, "Go","Reproduce",MB_OK | MB_ICONHAND);

            		Reproduce(i, k);

                  //if(i == 207) MessageBox(0, "Go","Shell deposition",MB_OK | MB_ICONHAND);

            		ShellDeposition(i, k);

                  //if(i == 207) MessageBox(0, "Go","SoftTissueGrowth",MB_OK | MB_ICONHAND);

            		SoftTissueGrowth(i, k);

                  //if(i == 207) MessageBox(0, "Go","Die",MB_OK | MB_ICONHAND);

            		Die(i, k);

					}
            }
			}

      	if (MySPMPointer != NULL)
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
      	}
		}
   }
   else
#ifdef __BORLANDC__
   	MessageBox(0, "Bivalves can not run without phytoplankton and suspended matter", "EcoDynamo alert", MB_OK);
#else // __BORLANDC__
		 cerr << "TCrassostreaGigas5::Go - Bivalves can not run without phytoplankton and suspended matter" << endl;
#endif  // __BORLANDC__
}


void TCrassostreaGigas5::Feed(int ABox, int AClass)
{
	int MyBox, MyClass;

   MyBox = ABox;
   MyClass = AClass;



   //if (TPM > 0.0)
   if ((TPM > 0.001) && (POM > 0.0001) && (PHYORG > 0.0001))
   {

   	FRTPM[MyBox] = (0.64*pow(TPM,1.22))*TemperatureEffectsOnFeedingRate(MyBox)*
   		AllometricCorrection (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding) *
   		DAYSTOHOURS; //Filtration rate -  mg / ind / d
      if (FRTPM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > TPM)
      FRTPM[MyBox]= TPM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;

      IRTPM[MyBox] = (1.07*pow(TPM,0.69))*TemperatureEffectsOnFeedingRate(MyBox)*
   		AllometricCorrection (ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding) *
   		DAYSTOHOURS; //Ingestion rate; mg / d

      if (FRTPM[MyBox] < IRTPM[MyBox])
      	FRTPM[MyBox] = IRTPM[MyBox];
         
      if (TPM > 0.0000000000000001)
   		ClearanceRate[MyClass * NumberOfBoxes + MyBox] = FRTPM[MyBox] / TPM; //L / d / ind
   	else
      	ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;

   	FRPIM[MyBox] = FRTPM[MyBox] * PIM / TPM;
      if (FRPIM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > PIM)
      FRPIM[MyBox]= PIM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;

   	FRPHY[MyBox] = FRTPM[MyBox] * PHYORG / TPM;
      if (FRPHY[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > PHYORG)
      FRPHY[MyBox]= PHYORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;
      
      FRPOM[MyBox] = FRTPM[MyBox] - FRPHY[MyBox];
      if (FRPOM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] * ATimeStep > POM)
      FRPOM[MyBox]= POM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / ATimeStep;
   }
   else
   {
   	FRTPM[MyBox] = 0.0;
      IRTPM[MyBox] = 0.0;
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;
      FRPIM[MyBox] = 0.0;
      FRPHY[MyBox] = 0.0;
      FRPOM[MyBox] = 0.0;
   }


   if ((FRTPM[MyBox] > 0.0) && (OCS > 0.0))
   	NOSE[MyBox] = -0.04+(0.032*(1.0 / OCS))+(0.007 * FRTPM[MyBox] / DAYSTOHOURS)-
                	(0.00078 * FRTPM[MyBox] / DAYSTOHOURS*(1.0 / OCS)); //Net organic selection efficiency; fraction
   else
   	NOSE[MyBox] = 0.0;


   RRTPM[MyBox] = FRTPM[MyBox] - IRTPM[MyBox];

   IRORG[MyBox] = (FRTPM[MyBox] * OCS)-((1.0 - NOSE[MyBox])* OCS * RRTPM[MyBox]); //Ingestion of organic matter; mg/d


   if (IRTPM[MyBox] > 0.0)
   	OCI[MyBox] = IRORG[MyBox] / IRTPM[MyBox]; //Organic content of ingested matter; fraction
   else
      OCI[MyBox] = 0.0;

   if (OCI[MyBox] > 0.0)
   	NAEIO[MyBox] = 0.74 - (0.084 * (1.0 / OCI[MyBox])); //Net absorption efficiency from ingested organics; fraction
   else
      NAEIO[MyBox] = 0.0;


   if (PHYORG + DETORG > 0.0)
   {
   	IRDET[MyBox] = FRTPM[MyBox] * NOSE[MyBox] * DETORG /(PHYORG + DETORG);
   	IRPHY[MyBox] = FRTPM[MyBox] * NOSE[MyBox] * PHYORG/(DETORG + PHYORG);
   }
   else
   {
      IRDET[MyBox] = 0.0;
      IRPHY[MyBox] = 0.0;
   }
   IRPIM[MyBox] = IRTPM[MyBox]-(IRDET[MyBox] + IRPHY[MyBox]);
   if (FRPIM[MyBox] < IRPIM[MyBox])
   	FRPIM[MyBox] = IRPIM[MyBox];
   RRPIM[MyBox] = FRPIM[MyBox] - IRPIM[MyBox];


   if (POM > 0.0)
   	NEA[MyBox] = IRORG[MyBox] * (6.1 + (17.4 * (PHYORG / POM))) * NAEIO[MyBox]; //Net energy absorption rate; J/d
   else
      NEA[MyBox] = 0.0;
}


void TCrassostreaGigas5::Reproduce(int ABox, int AClass)
{

}


double TCrassostreaGigas5::TemperatureEffectsOnFeedingRate(int ABoxNumber)
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
      return (4.825-(0.013*pow((WaterTemperature - 18.954),2)))/(4.825-(0.013*pow((19.8-18.954),2)));
      //Equation is from Serges paper, with feeding measurements made at 19.8 oC as in my MEPS paper
   }
   else
    return 1.0;
}


double TCrassostreaGigas5::TemperatureEffectsOnMaintenanceHeatLosses(int ABoxNumber)
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
		return exp(0.067 * WaterTemperature)/exp(0.067*15.0); //new parameter derived from Serges paper to give Q10 of 1.96
   }
   else
   	return 1.0;
}

double TCrassostreaGigas5::OxygenToNitrogenRatio(int ABoxNumber)
{
	int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
 	if ((10.0 + 0.05 * NEA[MyBoxNumber]) > 50.0)
   	return 50.0;
   else
	if ((10.0 + 0.05 * NEA[MyBoxNumber]) < 10.0)
   	return 10.0;
   else
 		return (10.0 + 0.05 * NEA[MyBoxNumber]);
   //Pode variar entre 10 e 800, na propor??o de NEA }
}

double TCrassostreaGigas5::ShellLenghtCalculation(double AShellDryWeight)
{
	double MyShellDryWeight;
   MyShellDryWeight = AShellDryWeight;
   return 28.21* pow(MyShellDryWeight,0.347) / 10; //This is the natural log of shell dry weight, with length in cm
}




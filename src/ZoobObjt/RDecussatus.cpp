// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 99.03.27
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
#include "iodll.h"
#include "ecodyn.rh"
#include "zoobobjt.h"

TRuditapesDecussatus::TRuditapesDecussatus(TEcoDynClass* APEcoDynClass, char* className)
							 :	TSuspensionFeeders(APEcoDynClass, className)
{
        /*NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns(); */
        //CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();
        /*bool BenthicRecord;
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
                MessageBox(0,"Benthic species record not initialized",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND); */
        NumberOfBoxesWithRDecussatus = 0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
                if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Ruditapes decussatus") == 0)
                        NumberOfBoxesWithRDecussatus++;

        }

//        Debugger(NumberOfBoxesWithBenthicSpecies);
//        Debugger(NumberOfBoxesWithRDecussatus);

        RuditapesDecussatusRecord = new BenthicSpeciesRecord[NumberOfBoxesWithRDecussatus];
        int AIndex = 0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
                if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Ruditapes decussatus") == 0)
                {
                        strcpy(RuditapesDecussatusRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
                        RuditapesDecussatusRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
                        RuditapesDecussatusRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
                        AIndex++;
                }
        }

        ParameterCount = 0;
        double MyValue;
        int X, Y, Index, XV, YV;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("RuditapesDecussatus");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "RuditapesDecussatus parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TRuditapesDecussatus::TRuditapesDecussatus - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("Ruditapes decussatus", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Number of classes") == 0)
                    PReadWrite->ReadNumber(X+3, i, NumberOfClasses);
                else
                if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
                    PReadWrite->ReadNumber(X+3, i, TotalFWToMeatDW);
                else if (strcmp(MyParameter, "CriticalWaterDepth") == 0)
                    PReadWrite->ReadNumber(X+3, i, CriticalWaterDepth);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in R. decussatus", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRuditapesDecussatus::TRuditapesDecussatus - undefined parameters in R. decussatus" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

/*        Generic                 = new double[NumberOfBoxes * NumberOfClasses];
        GenericFlux             = new double[NumberOfBoxes * NumberOfClasses];*/
	ZooClassBiomass         = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensity         = new double[NumberOfBoxes * NumberOfClasses];
	ZooInitialWeight        = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualWeight     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassBiomassFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensityFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualFlux       = new double[NumberOfBoxes * NumberOfClasses];
        ClassMaxLimits          = new double[NumberOfBoxes * NumberOfClasses];
        ClassMinLimits          = new double[NumberOfBoxes * NumberOfClasses];
	SestonFiltration        = new double[NumberOfBoxes * NumberOfClasses];
	pseudofaeces_production = new double[NumberOfBoxes * NumberOfClasses];
	assimilation_efficiency = new double[NumberOfBoxes * NumberOfClasses];
	transition_number       = new double[NumberOfClasses * NumberOfClasses];
	transition_biomass      = new double[NumberOfClasses * NumberOfClasses];
	transition_weight       = new double[NumberOfClasses * NumberOfClasses];
	GrowthRate              = new double[NumberOfBoxes * NumberOfClasses];
	ClearanceRate           = new double[NumberOfBoxes * NumberOfClasses];
	Mortality               = new double[NumberOfBoxes * NumberOfClasses];
        for (int m = 0; m < NumberOfClasses; m++)
                for (int n = 0; n < NumberOfBoxes; n++)
		{
/*			Generic[m * NumberOfBoxes + n]                 = 0;
                        GenericFlux[m * NumberOfBoxes + n]             = 0;*/
			ZooClassBiomass[m * NumberOfBoxes + n] 	       = 0;
			ZooClassDensity[m * NumberOfBoxes + n]         = 0;
			ZooInitialWeight[m * NumberOfBoxes + n]        = 0;
			ZooIndividualWeight[m * NumberOfBoxes + n]     = 0;
			ZooClassBiomassFlux[m * NumberOfBoxes + n]     = 0;
			ZooClassDensityFlux[m * NumberOfBoxes + n]     = 0;
			ZooIndividualFlux[m * NumberOfBoxes + n]       = 0;
			SestonFiltration[m * NumberOfBoxes + n]        = 0;
			pseudofaeces_production[m * NumberOfBoxes + n] = 0;
			assimilation_efficiency[m * NumberOfBoxes + n] = 0;
			GrowthRate[m * NumberOfBoxes + n] 	       = 0;
			ClearanceRate[m * NumberOfBoxes + n] 	       = 0;
			Mortality[m * NumberOfBoxes + n] 	       = 0;
		}
        for (int m = 0; m < NumberOfClasses; m++)
        {
   	        for (int n = 0; n < NumberOfClasses; n++)
                {
      	                transition_number[m * NumberOfClasses + n]     = 0;
			transition_biomass[m * NumberOfClasses + n]    = 0;
			transition_weight[m * NumberOfClasses + n]     = 0;
                }
        }
	// Read in the variables
	VariableCount = 0;
        char MyVariable[65];
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("RuditapesDecussatus");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "RuditapesDecussatus variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TRuditapesDecussatus::TRuditapesDecussatus - variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Ruditapes decussatus", X, Y))
        {
            PReadWrite->ReadNumber(X + NUMBEROFVARSCOLUMN, Y, NumberOfVariables);
            VariableNameArray = new VNA[NumberOfVariables];
     //		UnitNameArray = new UNA[NumberOfVariables];
    /*		for (int i = Y; i < Y + NumberOfVariables; i++)
                    {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN, i, VariableNameArray[i-Y]);
                            //PReadWrite->ReadString(X + UNITSCOLUMN, i, UnitNameArray[i-Y]);
                            //MessageBox(0, VariableNameArray[i-Y], "EcoDynamo alert", MB_OK);
                            //MessageBox(0, UnitNameArray[i-Y], "EcoDynamo alert", MB_OK);
                    }*/
            int k, j, offset;
            //Debugger(Y); Debugger(NumberOfBoxesWithRDecussatus);
            int RecordNumber;
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN, i, MyVariable);
                strcpy(VariableNameArray[i-Y], MyVariable);

                if (strcmp(MyVariable, "R. decussatus biomass 1") == 0)
                {
                    if (PReadWrite->FindString("R. decussatus biomass 1 values", XV, YV))
                    {
                        for (k = 0; k < NumberOfClasses; k++)
                        {
                            offset = k * NumberOfBoxes;
                            for (int j = 0; j < NumberOfBoxesWithRDecussatus; j++)
                            {
                                Index = RuditapesDecussatusRecord[j].LineCoordinate * NumberOfColumns +
                                        RuditapesDecussatusRecord[j].ColumnCoordinate;
                                PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue );
                                ZooClassBiomass[offset + Index] = MyValue * TotalFWToMeatDW;
                            }
                        }
                    }
                    else     // old file format
                    {
                        for (k = 0; k < NumberOfClasses; k++)
                        {
                            RecordNumber = 0;
                            for (j = i + k * NumberOfBoxesWithRDecussatus; j < i + k * NumberOfBoxesWithRDecussatus + NumberOfBoxesWithRDecussatus/*NumberOfBoxesWithBenthicSpecies*/; j++)
                            {
                                Index = /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                        /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[RecordNumber].ColumnCoordinate;
                                PReadWrite->ReadNumber( X + 5, j + k, MyValue );
                                                        //Debugger(22); Debugger(X); Debugger(X + 5); Debugger(j + k);
                                ZooClassBiomass[k * NumberOfBoxes + Index] = MyValue * TotalFWToMeatDW;
                                                        //Debugger(ZooClassBiomass[k * NumberOfBoxes + Index]);
                                RecordNumber++;
                            }
                        }
                    }
                }
                else if (strcmp(MyVariable, "R. decussatus individual weight 1") == 0)
                {
                    if (PReadWrite->FindString("R. decussatus individual weight 1 values", XV, YV))
                    {
                        for (k = 0; k < NumberOfClasses; k++)
                        {
                            offset = k * NumberOfBoxes;
                            for (int j = 0; j < NumberOfBoxesWithRDecussatus; j++)
                            {
                                Index = RuditapesDecussatusRecord[j].LineCoordinate * NumberOfColumns +
                                        RuditapesDecussatusRecord[j].ColumnCoordinate;
                                PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue );
                                ZooIndividualWeight[offset + Index] = MyValue;
                                ZooInitialWeight[offset + Index] = MyValue;
                            }
                        }
                    }
                    else     // old file format
                    {
                        for (k = 0; k < NumberOfClasses; k++)
                        {
                            RecordNumber = 0;
                            for (j = i + k * NumberOfBoxesWithRDecussatus; j < i + k * NumberOfBoxesWithRDecussatus + NumberOfBoxesWithRDecussatus/*NumberOfBoxesWithBenthicSpecies*/; j++)
                            {
                                Index = /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[RecordNumber].LineCoordinate * NumberOfColumns +
                                        /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[RecordNumber].ColumnCoordinate;
                                PReadWrite->ReadNumber( X + 4, j + k, MyValue );
                                                        //Debugger(11); Debugger(X); Debugger(Y); Debugger(X + 4); Debugger(j + k);
                                ZooIndividualWeight[k * NumberOfBoxes + Index] = MyValue;
                                ZooInitialWeight[k * NumberOfBoxes + Index] = MyValue;
                                                        //Debugger(ZooIndividualWeight[k * NumberOfBoxes + Index]);
                                RecordNumber++;
                            }
                        }
                    }
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object in R. decussatus", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRuditapesDecussatus::TRuditapesDecussatus - undefined object in R. decussatus" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    for (int k = 0; k < NumberOfClasses; k++)
    {
        int offset = k * NumberOfBoxes;
        for (int j = 0; j < NumberOfBoxesWithRDecussatus/*NumberOfBoxesWithBenthicSpecies*/; j++)
        {
            Index = /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[j].LineCoordinate * NumberOfColumns +
                    /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[j].ColumnCoordinate;
            ZooClassDensity[offset + Index] = ZooClassBiomass[offset + Index] /
                                      ZooIndividualWeight[offset + Index];
        }
    }
	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("RuditapesDecussatus");
	if (PReadWrite != NULL)
	{
        // Just read number of classes
        if (PReadWrite->FindString("Ruditapes decussatus", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
                    PReadWrite->ReadNumber(X+3, i, DryWeightOfAStandardAnimal);
                else if (strcmp(MyParameter, "Filtration coefficient") == 0)
                    PReadWrite->ReadNumber(X + 3, i, Filtration_coef);
                else if (strcmp(MyParameter, "Pseudofaeces maximal production rate") == 0)
                    PReadWrite->ReadNumber(X + 3, i, pseudo_faeces_maxo);
                else if (strcmp(MyParameter, "Pseudofaces expoent") == 0)
                    PReadWrite->ReadNumber(X + 3, i, xkp);
                else if (strcmp(MyParameter, "Pseudofaeces threshold") == 0)
                    PReadWrite->ReadNumber(X + 3, i, ThresholdConsumptionForPseudofaeces);
                else if (strcmp(MyParameter, "Respiration coefficient") == 0)
                    PReadWrite->ReadNumber(X + 3, i, RespirationCoefficient);
                else if (strcmp(MyParameter, "Meat energetic contents") == 0)
                    PReadWrite->ReadNumber(X + 3, i, MeatEnergeticContents);
                else if (strcmp(MyParameter, "Standard respiration") == 0)
                    PReadWrite->ReadNumber(X + 3, i, StandardRespiration);
                else if (strcmp(MyParameter, "Standard respiration under hypoxia") == 0)
                    PReadWrite->ReadNumber(X + 3, i, StandarRespirationUnderHypoxia);
                else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
                    PReadWrite->ReadNumber(X + 3, i, IntrinsicRateOfMortality);
                else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, MyValue);
                    for (int k = 0; k < NumberOfClasses; k++)
                        for (int j = 0; j < NumberOfBoxes; j++)
                            assimilation_efficiency[k * NumberOfBoxes + j] = MyValue;
                    MinimumAssimilationEfficiency = MyValue;
                }
                else if (strcmp(MyParameter, "Lower  limit class 1") == 0)
                {
                    for (int k = 0; k < NumberOfClasses; k++)
                        PReadWrite->ReadNumber(X + 3 + k, i, ClassMinLimits[k]);
                }
                else if (strcmp(MyParameter, "Upper  limit class 1") == 0)
                {
                    for (int k = 0; k < NumberOfClasses; k++)
                        PReadWrite->ReadNumber(X + 3 + k, i, ClassMaxLimits[k]);
                }
                else if (strcmp(MyParameter, "R. decussatus mortality 1") == 0)
                {
                    for (int k = 0; k < NumberOfClasses; k++)
                        PReadWrite->ReadNumber(X + 3 + k, i, Mortality[k]);
                }
                else if (strcmp(MyParameter, "Assimilation parameter") == 0)
                    PReadWrite->ReadNumber(X + 3, i, AssimilationParameter);
                else if (strcmp(MyParameter, "Assimilation maximum") == 0)
                    PReadWrite->ReadNumber(X + 3, i, AssimilationMaximum);
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in R. decussatus", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRuditapesDecussatus::TRuditapesDecussatus - undefined parameters in R. decussatus" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

    if (NumberOfClasses > 1)
        ClassIncrement = ClassMinLimits[1] - ClassMinLimits[0];
}

TRuditapesDecussatus::~TRuditapesDecussatus()
{
    freeMemory();
}

void TRuditapesDecussatus::freeMemory()
{
    try {
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }
        if (NumberOfVariables > 0) {
            delete [] VariableNameArray;
            NumberOfVariables = 0;
        }
        if (NumberOfBoxesWithRDecussatus > 0)
            delete [] RuditapesDecussatusRecord;

        if (NumberOfBoxes > 0)
        {
        /*	delete [] Generic;
            delete [] GenericFlux;*/
            delete [] ZooClassBiomass;
            delete [] ZooClassDensity;
            delete [] ZooInitialWeight;
            delete [] ZooIndividualWeight;
            delete [] ZooClassBiomassFlux;
            delete [] ZooClassDensityFlux;
            delete [] ZooIndividualFlux;
            delete [] ClassMaxLimits;
            delete [] ClassMinLimits;
            delete [] SestonFiltration;
            delete [] pseudofaeces_production;
            delete [] assimilation_efficiency;
            delete [] transition_number;
            delete [] transition_biomass;
            delete [] transition_weight;
            delete [] GrowthRate;
            delete [] ClearanceRate;
            delete [] Mortality;
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
				cerr << "TRuditapesDecussatus::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TRuditapesDecussatus::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Number of classes") == 0)
        value = NumberOfClasses;
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
        value = TotalFWToMeatDW;
    else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
        value = DryWeightOfAStandardAnimal;
    else if (strcmp(MyParameter, "Filtration coefficient") == 0)
        value = Filtration_coef;
    else if (strcmp(MyParameter, "Pseudofaeces maximal production rate") == 0)
        value = pseudo_faeces_maxo;
    else if (strcmp(MyParameter, "Pseudofaces expoent") == 0)
        value = xkp;
    else if (strcmp(MyParameter, "Pseudofaeces threshold") == 0)
        value = ThresholdConsumptionForPseudofaeces;
    else if (strcmp(MyParameter, "Respiration coefficient") == 0)
        value = RespirationCoefficient;
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
        value = MeatEnergeticContents;
    else if (strcmp(MyParameter, "Standard respiration") == 0)
        value = StandardRespiration;
    else if (strcmp(MyParameter, "Standard respiration under hypoxia") == 0)
        value = StandarRespirationUnderHypoxia;
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
        value = IntrinsicRateOfMortality;
    else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
        value = assimilation_efficiency[0];
    else if (strcmp(MyParameter, "Lower  limit class 1") == 0)
        value = ClassMinLimits[0];
    else if (strcmp(MyParameter, "Upper  limit class 1") == 0)
        value = ClassMaxLimits[0];
    else if (strcmp(MyParameter, "R. decussatus mortality 1") == 0)
        value = Mortality[0];
    else if (strcmp(MyParameter, "Assimilation parameter") == 0)
        value = AssimilationParameter;
    else if (strcmp(MyParameter, "Assimilation maximum") == 0)
        value = AssimilationMaximum;
    else
        value = 0.0;
    return value;
}

bool TRuditapesDecussatus::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j, k;

    if (strcmp(MyParameter, "Number of classes") == 0)
        NumberOfClasses = value;
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
        TotalFWToMeatDW = value;
    else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
        DryWeightOfAStandardAnimal = value;
    else if (strcmp(MyParameter, "Filtration coefficient") == 0)
        Filtration_coef = value;
    else if (strcmp(MyParameter, "Pseudofaeces maximal production rate") == 0)
        pseudo_faeces_maxo = value;
    else if (strcmp(MyParameter, "Pseudofaces expoent") == 0)
        xkp = value;
    else if (strcmp(MyParameter, "Pseudofaeces threshold") == 0)
        ThresholdConsumptionForPseudofaeces = value;
    else if (strcmp(MyParameter, "Respiration coefficient") == 0)
        RespirationCoefficient = value;
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
        MeatEnergeticContents = value;
    else if (strcmp(MyParameter, "Standard respiration") == 0)
        StandardRespiration = value;
    else if (strcmp(MyParameter, "Standard respiration under hypoxia") == 0)
        StandarRespirationUnderHypoxia = value;
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
        IntrinsicRateOfMortality = value;
    else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            for (int j = 0; j < NumberOfBoxes; j++)
                assimilation_efficiency[k * NumberOfBoxes + j] = value;
    }
    else if (strcmp(MyParameter, "Lower  limit class 1") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            ClassMinLimits[k] = value;
    }
    else if (strcmp(MyParameter, "Upper  limit class 1") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            ClassMaxLimits[k] = value;
    }
    else if (strcmp(MyParameter, "R. decussatus mortality 1") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            Mortality[k] = value;
    }
    else if (strcmp(MyParameter, "Assimilation parameter") == 0)
        AssimilationParameter = value;
    else if (strcmp(MyParameter, "Assimilation maximum") == 0)
        AssimilationMaximum = value;
    else
        rc = false;

    return rc;
}


void TRuditapesDecussatus::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName,int AnObjectCode)
{
	int i,j;
	char MyName[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;

    if (strstr(ParameterName, "R. decussatus density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "R. decussatus density %i", j + 1 );
            if (strcmp(ParameterName, MyName) == 0) {
                Value = ZooClassDensity[NumberOfBoxes * j + BoxNumber];
                break;
            }
        }
    }
    else if (strstr(ParameterName, "R. decussatus individual weight " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "R. decussatus individual weight %i", j + 1 );
            if (strcmp(ParameterName, MyName) == 0) {
                Value = ZooIndividualWeight[NumberOfBoxes * j + BoxNumber];
                break;
            }
        }
    }
    else if (strstr(ParameterName, "R. decussatus biomass " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "R. decussatus biomass %i", j + 1 );
            if (strcmp(ParameterName, MyName) == 0) {
                Value = ZooClassBiomass[NumberOfBoxes * j + BoxNumber] / TotalFWToMeatDW;
                break;
            }
        }
    }
    else if (strstr(ParameterName, "Individual growth " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Individual growth %i", j + 1 );
            if (strcmp(ParameterName, MyName) == 0) {
                Value = ZooIndividualFlux[NumberOfBoxes * j + BoxNumber] * DAYSTOSECONDS; // Values in g day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(ParameterName, "Scope for growth " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Scope for growth %i", j + 1 );
            if (strcmp(ParameterName, MyName) == 0) {
                Value = GrowthRate[NumberOfBoxes * j + BoxNumber] * DAYSTOSECONDS; // Values in j day-1 ind-1
                break;
            }
        }
    }
    else if (strstr(ParameterName, "Clearance rate " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Clearance rate %i", j + 1 );
            if (strcmp(ParameterName, MyName) == 0) {
                Value = ClearanceRate[NumberOfBoxes * j + BoxNumber] * DAYSTOHOURS; // Values in litres d-1
                break;
            }
        }
    }
    if (strcmp(ParameterName, "Zoobenthos DIN Release") == 0)
        Value = ZooDINRelease[BoxNumber];

    else if (strcmp(ParameterName, "Zoobenthos Phyto Uptake") == 0)
        Value = ZooPhytoUptake[BoxNumber];

    else if (strcmp(ParameterName, "Zoobenthos POC Uptake") == 0)
        Value = ZooPOCUptake[BoxNumber];   //mgC s-1 L-1
    else if (strcmp(ParameterName, "Total biomass") == 0)
    {
        double TotalBiomass;
        TotalBiomass = 0.0;
        for (j = 0; j < NumberOfClasses; j++)
        {
            TotalBiomass = TotalBiomass + ZooClassBiomass[NumberOfBoxes * j + BoxNumber] /
                                 TotalFWToMeatDW;
        }
        Value = TotalBiomass;
    }
    else if (strcmp(ParameterName, "Total density") == 0)
    {
        double TotalDensity;
        TotalDensity = 0.0;
        for (j = 0; j < NumberOfClasses; j++)
        {
            TotalDensity = TotalDensity + ZooClassDensity[NumberOfBoxes * j + BoxNumber];
        }
        Value = TotalDensity;
    }
    else
    {
        for (i = 0; i < NumberOfVariables; i++)
            if (strcmp(ParameterName, VariableNameArray[i]) == 0)
                break;;

        if (i == NumberOfVariables)
        {
#ifdef __BORLANDC__
            char Caption[129];
            strcpy(Caption, ParameterName);
            strcat(Caption, " does not exist in ");
            strcat(Caption, GetEcoDynClassName());
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - Inquiry 7",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRuditapesDecussatus::Inquiry 7 - " << ParameterName << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
		Value = 0;
    }
    LogMessage("Inquiry", srcName, ParameterName, Value, MyBoxNumber);
}


void TRuditapesDecussatus::Inquiry(char* srcName, double &Value,int BoxNumber,double ExtraValue,int AnObjectCode)
{
	double MyHarvestableBiomass = 0.0;
	int MyBoxNumber = BoxNumber;
	for (int k = 0; k < NumberOfClasses; k++)
	{
		if (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] >=
				 ExtraValue * TotalFWToMeatDW)
			MyHarvestableBiomass = MyHarvestableBiomass +
				ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber];
	}
	Value = MyHarvestableBiomass
			  * BoxArray[BoxNumber].Volume
			  / (CUBIC * CUBIC * TotalFWToMeatDW);
	// values in TFW(live weight) na area em que existe a esp?cie
        LogMessage("Inquiry", srcName, "Harvestable Biomass", Value, MyBoxNumber);
}

void TRuditapesDecussatus::Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int ClassNumber,int AnObjectCode)
{       //This works for recruitment or harvest of individuals to/from any class
        double  mass_flux, individual_weight;
        //All values debited as gFW com concha
        LogMessage("Update", srcName, "Zoo Class Density Flux", Value, BoxNumber);
        mass_flux = Value * TotalFWToMeatDW;
        individual_weight = ExtraValue * TotalFWToMeatDW;
        //Values recalculated to the current units - gDW sem concha
        if ((mass_flux != 0) && (individual_weight > 0))
	{
	        ZooClassDensityFlux[ClassNumber * NumberOfBoxes + BoxNumber]
                        = ZooClassDensityFlux[ClassNumber * NumberOfBoxes + BoxNumber] + mass_flux/
						  individual_weight;
		ZooClassBiomassFlux[ClassNumber * NumberOfBoxes + BoxNumber]
			= ZooClassBiomassFlux[ClassNumber * NumberOfBoxes + BoxNumber] + mass_flux;
	}
}


void TRuditapesDecussatus::Update(char* srcName, double Value,double ExtraValue,int BoxNumber,int AnObjectCode)
{
        //This works for harvest of individuals above a certain size
        int k;
        double  mass_flux, PreyWeight, MyHarvestableBiomass = 0.0;
        int MyBoxNumber = BoxNumber, MyHarvestableClass = 0;
        //All values debited as gFW com concha
        LogMessage("Update", srcName, "Zoo Class Density Flux", Value, BoxNumber);
	mass_flux = Value * TotalFWToMeatDW;
	PreyWeight = ExtraValue * TotalFWToMeatDW;
	//Values recalculated to the current units - gDW sem concha
	for (k = 0; k < NumberOfClasses; k++)
	{
		if (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] >=
				 PreyWeight)
		{
			if (MyHarvestableClass == 0)
				MyHarvestableClass = k;
			MyHarvestableBiomass = MyHarvestableBiomass +
						ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber];
		}
	}
	for (k = MyHarvestableClass; k < NumberOfClasses; k++)
	{
		if ((mass_flux != 0) &&
			 (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] > 0)
			 )
		{
			ZooClassDensityFlux[k * NumberOfBoxes + MyBoxNumber]
							= ZooClassDensityFlux[k * NumberOfBoxes + MyBoxNumber] +
						  (mass_flux /
						  ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber]) *
						  (ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] /
						  MyHarvestableBiomass);
			ZooClassBiomassFlux[k * NumberOfBoxes + MyBoxNumber]
							= ZooClassBiomassFlux[k * NumberOfBoxes + MyBoxNumber] +
						  mass_flux *
						  (ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] /
						  MyHarvestableBiomass);
		}
	}
}

// AP, 2006.06.09
bool TRuditapesDecussatus::SetVariableValue(char* srcName, double Value,
                                             int BoxNumber,
                                             char* VariableName)
{
    bool rc = true;
    int j;
	char MyName[65]; // 64 characters (max) for parameter name
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strstr(VariableName, "R. decussatus density ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf(MyName, "R. decussatus density %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooClassDensity[NumberOfBoxes * j + BoxNumber] = Value;
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] =
                    Value * ZooIndividualWeight[NumberOfBoxes * j + BoxNumber];
                break;
            }
        }
    }
    else if (strstr(VariableName, "R. decussatus individual weight ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf(MyName, "R. decussatus individual weight %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + BoxNumber] = Value;
                break;
            }
        }
    }
    else if (strstr(VariableName, "R. decussatus biomass ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf(MyName, "R. decussatus biomass %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] = Value * TotalFWToMeatDW;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Individual growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf(MyName, "Individual growth %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooIndividualFlux[NumberOfBoxes * j + BoxNumber] = Value / DAYSTOSECONDS;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Scope for growth ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf(MyName, "Scope for growth %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                GrowthRate[NumberOfBoxes * j + BoxNumber] = Value / DAYSTOSECONDS;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Clearance rate ") != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf(MyName, "Clearance rate %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ClearanceRate[NumberOfBoxes * j + BoxNumber] = Value / DAYSTOHOURS;
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
    else
        rc = false;

    return rc;
}

// AP, 2007.07.16
bool TRuditapesDecussatus::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("RuditapesDecussatus");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Ruditapes decussatus");
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
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * Crassostrea gigas initial values
     */
    char classText[16];
    PReadWrite->WriteCell("R. decussatus biomass 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("R. decussatus individual weight 1 values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator(true);

    int offset, Index;
    for (int j = 0; j < NumberOfBoxesWithRDecussatus; j++)
    {
        Index = RuditapesDecussatusRecord[j].LineCoordinate * NumberOfColumns +
                RuditapesDecussatusRecord[j].ColumnCoordinate;

        // biomass by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooClassBiomass[offset + Index] / TotalFWToMeatDW, 8);
            PReadWrite->WriteSeparator();
        }

        // individual weight by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooIndividualWeight[offset + Index], 8);
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TRuditapesDecussatus::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("RuditapesDecussatus");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Ruditapes decussatus");
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
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Dry weight of a standard animal") == 0)
        {
            PReadWrite->WriteCell(DryWeightOfAStandardAnimal, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Filtration coefficient") == 0)
        {
            PReadWrite->WriteCell(Filtration_coef, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Pseudofaeces maximal production rate") == 0)
        {
            PReadWrite->WriteCell(pseudo_faeces_maxo, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Pseudofaces expoent") == 0)
        {
            PReadWrite->WriteCell(xkp, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Pseudofaeces threshold") == 0)
        {
            PReadWrite->WriteCell(ThresholdConsumptionForPseudofaeces, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Respiration coefficient") == 0)
        {
            PReadWrite->WriteCell(RespirationCoefficient, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
            PReadWrite->WriteCell(MeatEnergeticContents, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Standard respiration") == 0)
        {
            PReadWrite->WriteCell(StandardRespiration, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Standard respiration under hypoxia") == 0)
        {
            PReadWrite->WriteCell(StandarRespirationUnderHypoxia, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Assimilation efficiency") == 0)
        {
            PReadWrite->WriteCell(MinimumAssimilationEfficiency, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Lower  limit class 1") == 0)
        {
            for (int k = 0; k < NumberOfClasses; k++)
            {
                PReadWrite->WriteCell(ClassMinLimits[k], 8);
                PReadWrite->WriteSeparator();
            }
        }
		else if (strcmp(ParametersNameArray[i], "Upper  limit class 1") == 0)
        {
            for (int k = 0; k < NumberOfClasses; k++)
            {
                PReadWrite->WriteCell(ClassMaxLimits[k], 8);
                PReadWrite->WriteSeparator();
            }
        }
        else if (strcmp(ParametersNameArray[i], "R. decussatus mortality 1") == 0)
        {
            for (int k = 0; k < NumberOfClasses; k++)
            {
                PReadWrite->WriteCell(Mortality[k], 8);
                PReadWrite->WriteSeparator();
            }
        }
        else if (strcmp(ParametersNameArray[i], "Assimilation parameter") == 0)
        {
            PReadWrite->WriteCell(AssimilationParameter, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Assimilation maximum") == 0)
        {
            PReadWrite->WriteCell(AssimilationMaximum, 8);
        }

        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TRuditapesDecussatus::Go()
{
//	double MyJulianDay;
//	MyJulianDay = MyPEcoDynClass->GetJulianDay();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooDINRelease[i] = 0;
		ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
	}
	//C?lculo do fluxo l?quido ao n?vel individual e da popula??o, com base nos processos metab?licos}
	Feed();
    Pseudofaeces();
	Ingestion();
	Excrete();
	Assimilate();
	Respire();
    Reproduce();
	Die();
}


void TRuditapesDecussatus::Integrate()
{
	DemographicUpwind();
}



void TRuditapesDecussatus::Feed()
{
	//double MyHour;
	double nonliving_seston_filtration, MyDepth, MyTidalHeight;
	TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
              * MyTidePointer = MyPEcoDynClass->GetTidePointer();

	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0)
				)
			{
                if (MySPMPointer != NULL)
                    {
			   	        POC_mass = 0; seston_mass = 0;
            	                        max_filtration_rate = ClearanceRateOfAStandardAnimal(i)/(pow(DryWeightOfAStandardAnimal, Filtration_coef));
            	                        ClearanceRate[k * NumberOfBoxes + i] = max_filtration_rate *
                                                      exp(Filtration_coef *
                                                      log(ZooIndividualWeight[k * NumberOfBoxes + i])); // l h-1 ind-1
                                        MySPMPointer->Inquiry(GetEcoDynClassName(), seston_mass,i,"Suspended matter",ObjectCode);
                                        MySPMPointer->Inquiry(GetEcoDynClassName(), POC_mass,i,"Particulate organic carbon",ObjectCode);
                                        POCtoSESTON = POC_mass / seston_mass;
                                        MyDepth = MyPEcoDynClass->GetBoxDepth(i);
                                        if (MyTidePointer != NULL)
                                        {
               	                                MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,i,"Tidal height",ObjectCode);
                                                MyDepth = MyDepth + MyTidalHeight / CUBIC;
                                        }
                                        nonliving_seston_filtration = ClearanceRate[k * NumberOfBoxes + i]   //l h-1 ind-1
							                        * seston_mass     //mg l-1
							                        * DAYSTOHOURS     //24
							                        * (ZooClassBiomass[k * NumberOfBoxes + i] //g (meat DW) m-2
								                / ZooIndividualWeight[k * NumberOfBoxes + i]) //g (meat DW) ind-1
                                                                                / MyDepth  //m
                                                                                /DAYSTOSECONDS;
                                                                               // mg s-1 m-3
                                        /*if (nonliving_seston_filtration / CUBIC * TimeStep > seston_mass)
                                        {
               	                                nonliving_seston_filtration = seston_mass* CUBIC / TimeStep; // mg dia-1 m-3
                                        } */
                                        nonliving_seston_filtration = nonliving_seston_filtration * MyDepth / CUBIC;  //g s-1 m-2
                                        //Debugger(nonliving_seston_filtration);
                                        ZooIndividualFlux[k * NumberOfBoxes + i] = nonliving_seston_filtration
						                                        * (ZooIndividualWeight[k * NumberOfBoxes + i]
							                                / ZooClassBiomass[k * NumberOfBoxes + i]);  // g s-1 ind-1
                                        //Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
                                        MySPMPointer->Update(GetEcoDynClassName(), -nonliving_seston_filtration / MyDepth,i,"Suspended matter",ObjectCode);
                                        ZooPOCUptake[i] = ZooPOCUptake[i]+ nonliving_seston_filtration / MyDepth * POCtoSESTON; //mg C s-1 m-3
                                        SestonFiltration[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] * DAYSTOSECONDS;
                                }
			}
		}
	}
}




void TRuditapesDecussatus::Pseudofaeces()
{
	double delta;
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0)
                           )
			{
                                delta = ThresholdConsumptionForPseudofaeces /ZooIndividualWeight[k * NumberOfBoxes + i] -
                                        SestonFiltration[k * NumberOfBoxes + i];
                                if (delta > 0)
            	                        delta = 0;
				pseudo_faeces_rate = pseudo_faeces_maxo * (1 - exp(xkp * delta));
                                pseudofaeces_production[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] * pseudo_faeces_rate;//   g s-1 ind-1
                                //Debugger(pseudo_faeces_rate); Debugger(pseudofaeces_production[k * NumberOfBoxes + i]);
			}
		}
        }
}


void TRuditapesDecussatus::Ingestion()
{
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{
				ZooIndividualFlux[k * NumberOfBoxes + i]
					= ZooIndividualFlux[k * NumberOfBoxes + i] -  pseudofaeces_production[k * NumberOfBoxes + i]; //g s-1
			}
		}
	}
}


void TRuditapesDecussatus::Assimilate()
{
	double const SestonEnergeticContent = 23000; //J g-1 - Valor utilizado pela Paula Sobral para culturas de algas
        double const ProportionOfTimeInEmersion = 0.0;
        for (int k = 0; k < NumberOfClasses; k++)
        {
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
                           )
			{
         	                ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] *
                                                       POCtoSESTON *
                                                       assimilation_efficiency[k * NumberOfBoxes + i] *
                                                       (1 - ProportionOfTimeInEmersion); //g s-1 ind-1

                                GrowthRate[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] *
                                                    SestonEnergeticContent;// j s-1 ind-1*/
                                //Debugger(GrowthRate[k * NumberOfBoxes + i]);
                                //Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
			}
		}
	}
}



void TRuditapesDecussatus::Excrete()
{
//        double NH4OxygenRatio = 0.21; //mol N-NH4 / mol O2 Valore m?dio estimado a partir da Fig. 4.1 (p?g. 66) da tese da Paula Sobral
        
}


void TRuditapesDecussatus::Respire()
{
	double Respiration, MyDepth, MyTidalHeight;
        TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
                    * MyTidePointer = MyPEcoDynClass->GetTidePointer();
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{
                                Respiration = RespirationOfAStandardAnimal(i) * pow(ZooIndividualWeight[k * NumberOfBoxes + i] /
                                DryWeightOfAStandardAnimal, RespirationCoefficient) / DAYSTOSECONDS;//j s-1 ind-1
                                GrowthRate[k * NumberOfBoxes + i] = GrowthRate[k * NumberOfBoxes + i] - Respiration;// j s-1 */
                                ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] -
                                                                           Respiration / MeatEnergeticContents;	//g s-1 ind-1
                                //Debugger(GrowthRate[k * NumberOfBoxes + i]);
				if (MyNutrientPointer != NULL)
                                {
                                        MyDepth = MyPEcoDynClass->GetBoxDepth(i);
      			                if (MyTidePointer != NULL)
      			                {
      				                MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,i,"Tidal height",ObjectCode);
         			                MyDepth = MyDepth + MyTidalHeight / CUBIC;
      			                }
            	                        MyNutrientPointer->Update(GetEcoDynClassName(),
                                                -Respiration / MeatEnergeticContents * ZooClassDensity[k * NumberOfBoxes + i] /MyDepth, // mg / l
														i,
			  											"Oxygen",
			 											ObjectCode);
                                }
			}
		}
	}
}

void TRuditapesDecussatus::Reproduce()
{
	JulianDay = MyPEcoDynClass->GetJulianDay();
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
				)
			{
         	                if ((JulianDay > 181) && (JulianDay < 304))
            	                        ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] -
                                                          0.009 * ZooIndividualWeight[k * NumberOfBoxes + i] / DAYSTOSECONDS;//g s-1 ind-1

			}
		}
	}
}

void TRuditapesDecussatus::ShellDeposition()
{

}

void TRuditapesDecussatus::OrganicDeposition()
{

}

void TRuditapesDecussatus::Die()
{
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0)
                           )
			{
                                Mortality[k * NumberOfBoxes + i] = ZooClassDensity[k * NumberOfBoxes + i] -
                                               ZooClassDensity[k * NumberOfBoxes + i] *
                                               exp(-IntrinsicRateOfMortality * 1);
                                if (Mortality[k * NumberOfBoxes + i] < 0.0) Mortality[k * NumberOfBoxes + i] = 0.0;
				ZooClassDensityFlux[k * NumberOfBoxes + i] = ZooClassDensityFlux[k * NumberOfBoxes + i] - Mortality[k * NumberOfBoxes + i] / DAYSTOSECONDS; /**
                                ZooClassDensity[k * NumberOfBoxes + i]*/;
			}
		}
	}
}


double TRuditapesDecussatus::TemperatureLimitation(int ABoxNumber)
{
	double MyWaterTemperature;
        int MyBoxNumber;
        MyBoxNumber = ABoxNumber;
	TEcoDynClass * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
	if (MyWaterTemperaturePointer != NULL)
        {
   	        MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
                if (MyWaterTemperature <= 20.0)
      	                return 1.0 + 0.045 * (MyWaterTemperature - 20.0);
                else
      	                return 1.0 - 0.040 * (MyWaterTemperature - 20.0);
        }
        else
   	        return 1.0;
}

double TRuditapesDecussatus::OxygenLimitation(int ABoxNumber)

{
	double MyOxygenSaturation, MyOxygen, MyPercentSaturation;
        int MyBoxNumber;
        MyBoxNumber = ABoxNumber;
	TEcoDynClass * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
        {
   	        MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyOxygenSaturation,
                                     MyBoxNumber,
                                     "Oxygen saturation",
                                     ObjectCode);
                MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyOxygen,
                                     MyBoxNumber,
                                     "Oxygen",
                                     ObjectCode);
                //Debugger(MyOxygenSaturation);
                if (MyOxygenSaturation > 0)
                   MyPercentSaturation = MyOxygen / MyOxygenSaturation * 100;
                else
                   MyPercentSaturation = 100.0;
                if (MyPercentSaturation < 0.0)
#ifdef __BORLANDC__
      	                MessageBox(0, "Negative oxygen in TRuditapesDecussatus::OxygenLimitation", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
												cerr << "TRuditapesDecussatus::OxygenLimitation - Negative oxygen " << endl;
#endif  // __BORLANDC__
                if (MyPercentSaturation > 28)
      	                return 1.0;
                else
                {
      	                if (1.0 + 0.06 * (MyPercentSaturation - 28.0) > 0.0)
      		                return 1.0 + 0.06 * (MyPercentSaturation - 28.0);
                        else
         	                return 0.0;
                }
   }
   else
   	return 1.0;
}


double TRuditapesDecussatus::ClearanceRateOfAStandardAnimal(int ABoxNumber)
{
	int MyBoxNumber;
        MyBoxNumber = ABoxNumber;
        TEcoDynClass*	MySPMPointer = MyPEcoDynClass->GetSPMPointer();
        if (MySPMPointer != NULL)
        {
   	        MySPMPointer->Inquiry(GetEcoDynClassName(), seston_mass,
                                        MyBoxNumber,
                                        "TPM",
                                        ObjectCode);
                return (-0.003 * seston_mass + 1.426) * OxygenLimitation(MyBoxNumber) * TemperatureLimitation(MyBoxNumber);
	}
        else
#ifdef __BORLANDC__
   	        MessageBox(0, "SPM object unreachable for R. decussatus", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRuditapesDecussatus::ClearanceRateOfAStandardAnimal - SPM object unreachable for R. decussatus" << endl;
#endif  // __BORLANDC__

        return 1.0;  // is this the default value??????? AP, 2006.07.27
}


double TRuditapesDecussatus::RespirationOfAStandardAnimal(int ABoxNumber)
{
	double MyOxygenSaturation, MyOxygen, MyPercentSaturation;
        int MyBoxNumber;
        MyBoxNumber = ABoxNumber;
	TEcoDynClass * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
        {
   	        MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyOxygenSaturation,
                                             MyBoxNumber,
                                             "Oxygen saturation",
                                             ObjectCode);
                MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyOxygen,
                                        MyBoxNumber,
                                        "Oxygen",
                                        ObjectCode);
                MyPercentSaturation = MyOxygen / MyOxygenSaturation * 100;
                if (MyPercentSaturation >= 33)
      	                return StandardRespiration * DAYSTOHOURS;   //{6 J h-1 ind-1 Resultados das experi?ncias da Manuela Falc?o}
                else                         //{3.1 J h-1 ind-1  Tese da Paula Sobral, cap?tulo 3, a respira??o diminuia sob % de satura??o baixas }
      	                return StandarRespirationUnderHypoxia * DAYSTOHOURS;
        }
        else
   	        return StandardRespiration * DAYSTOHOURS;
}


TRDecussatusFoodLimited::TRDecussatusFoodLimited(TEcoDynClass* APEcoDynClass, char* className)
							 :	TRuditapesDecussatus(APEcoDynClass, className)
{
	BoxClearanceRate = new double[NumberOfBoxes];
	BoxClearanceTime = new double[NumberOfBoxes];
        for (int i = 0; i < NumberOfBoxes; i++)
        {
                BoxClearanceRate[i] = 0.0;
   	        BoxClearanceTime[i] = 0.0;
	}
        int X, Y;
        // Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("RuditapesDecussatus");
	if (PReadWrite != NULL)
	{
        // Just read number of classes
        if (PReadWrite->FindString("Ruditapes decussatus", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Cultivated area") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, CultivatedArea);
                        //Debugger(CultivatedArea);
                }
            }
        }
            else
            {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined parameters in R. decussatus food limited", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TRDecussatusFoodLimited::TRDecussatusFoodLimited - Undefined parameters in R. decussatus food limited" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TRDecussatusFoodLimited::~TRDecussatusFoodLimited()
{
    freeMemory();
}

void TRDecussatusFoodLimited::freeMemory()
{
    try {
        delete [] BoxClearanceRate;
        delete [] BoxClearanceTime;
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
				cerr << "TRDecussatusFoodLimited::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TRDecussatusFoodLimited::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TRuditapesDecussatus
    if (strcmp(MyParameter, "Number of classes") == 0)
        value = NumberOfClasses;
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
        value = TotalFWToMeatDW;
    else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
        value = DryWeightOfAStandardAnimal;
    else if (strcmp(MyParameter, "Filtration coefficient") == 0)
        value = Filtration_coef;
    else if (strcmp(MyParameter, "Pseudofaeces maximal production rate") == 0)
        value = pseudo_faeces_maxo;
    else if (strcmp(MyParameter, "Pseudofaces expoent") == 0)
        value = xkp;
    else if (strcmp(MyParameter, "Pseudofaeces threshold") == 0)
        value = ThresholdConsumptionForPseudofaeces;
    else if (strcmp(MyParameter, "Respiration coefficient") == 0)
        value = RespirationCoefficient;
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
        value = MeatEnergeticContents;
    else if (strcmp(MyParameter, "Standard respiration") == 0)
        value = StandardRespiration;
    else if (strcmp(MyParameter, "Standard respiration under hypoxia") == 0)
        value = StandarRespirationUnderHypoxia;
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
        value = IntrinsicRateOfMortality;
    else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
        value = assimilation_efficiency[0];
    else if (strcmp(MyParameter, "Lower  limit class 1") == 0)
        value = ClassMinLimits[0];
    else if (strcmp(MyParameter, "Upper  limit class 1") == 0)
        value = ClassMaxLimits[0];
    else if (strcmp(MyParameter, "R. decussatus mortality 1") == 0)
        value = Mortality[0];

    // parameters from TRDecussatusFoodLimited
    else if (strcmp(MyParameter, "Cultivated area") == 0)
        value = CultivatedArea;
    else
        value = 0.0;
    return value;
}

bool TRDecussatusFoodLimited::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j, k;

    // parameters from TRuditapesDecussatus
    if (strcmp(MyParameter, "Number of classes") == 0)
        NumberOfClasses = value;
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
        TotalFWToMeatDW = value;
    else if (strcmp(MyParameter, "Dry weight of a standard animal") == 0)
        DryWeightOfAStandardAnimal = value;
    else if (strcmp(MyParameter, "Filtration coefficient") == 0)
        Filtration_coef = value;
    else if (strcmp(MyParameter, "Pseudofaeces maximal production rate") == 0)
        pseudo_faeces_maxo = value;
    else if (strcmp(MyParameter, "Pseudofaces expoent") == 0)
        xkp = value;
    else if (strcmp(MyParameter, "Pseudofaeces threshold") == 0)
        ThresholdConsumptionForPseudofaeces = value;
    else if (strcmp(MyParameter, "Respiration coefficient") == 0)
        RespirationCoefficient = value;
    else if (strcmp(MyParameter, "Meat energetic contents") == 0)
        MeatEnergeticContents = value;
    else if (strcmp(MyParameter, "Standard respiration") == 0)
        StandardRespiration = value;
    else if (strcmp(MyParameter, "Standard respiration under hypoxia") == 0)
        StandarRespirationUnderHypoxia = value;
    else if (strcmp(MyParameter, "Intrinsic rate of mortality") == 0)
        IntrinsicRateOfMortality = value;
    else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            for (int j = 0; j < NumberOfBoxes; j++)
                assimilation_efficiency[k * NumberOfBoxes + j] = value;
    }
    else if (strcmp(MyParameter, "Lower  limit class 1") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            ClassMinLimits[k] = value;
    }
    else if (strcmp(MyParameter, "Upper  limit class 1") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            ClassMaxLimits[k] = value;
    }
    else if (strcmp(MyParameter, "R. decussatus mortality 1") == 0)
    {
        for (int k = 0; k < NumberOfClasses; k++)
            Mortality[k] = value;
    }

    // parameters from TRDecussatusFoodLimited
    else if (strcmp(MyParameter, "Cultivated area") == 0)
        CultivatedArea = value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.16
bool TRDecussatusFoodLimited::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("RuditapesDecussatus");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Ruditapes decussatus");
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

        // parameters from TRuditapesDecussatus
        if (strcmp(ParametersNameArray[i], "Number of classes") == 0)
        {
            PReadWrite->WriteCell(NumberOfClasses, 0);
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Dry weight of a standard animal") == 0)
        {
            PReadWrite->WriteCell(DryWeightOfAStandardAnimal, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Filtration coefficient") == 0)
        {
            PReadWrite->WriteCell(Filtration_coef, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Pseudofaeces maximal production rate") == 0)
        {
            PReadWrite->WriteCell(pseudo_faeces_maxo, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Pseudofaces expoent") == 0)
        {
            PReadWrite->WriteCell(xkp, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Pseudofaeces threshold") == 0)
        {
            PReadWrite->WriteCell(ThresholdConsumptionForPseudofaeces, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Respiration coefficient") == 0)
        {
            PReadWrite->WriteCell(RespirationCoefficient, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Meat energetic contents") == 0)
        {
            PReadWrite->WriteCell(MeatEnergeticContents, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Standard respiration") == 0)
        {
            PReadWrite->WriteCell(StandardRespiration, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Standard respiration under hypoxia") == 0)
        {
            PReadWrite->WriteCell(StandarRespirationUnderHypoxia, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Intrinsic rate of mortality") == 0)
        {
            PReadWrite->WriteCell(IntrinsicRateOfMortality, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Assimilation efficiency") == 0)
        {
            PReadWrite->WriteCell(MinimumAssimilationEfficiency, 8);
        }
		else if (strcmp(ParametersNameArray[i], "Lower  limit class 1") == 0)
        {
            for (int k = 0; k < NumberOfClasses; k++)
            {
                PReadWrite->WriteCell(ClassMinLimits[k], 8);
                PReadWrite->WriteSeparator();
            }
        }
		else if (strcmp(ParametersNameArray[i], "Upper  limit class 1") == 0)
        {
            for (int k = 0; k < NumberOfClasses; k++)
            {
                PReadWrite->WriteCell(ClassMaxLimits[k], 8);
                PReadWrite->WriteSeparator();
            }
        }
        else if (strcmp(ParametersNameArray[i], "R. decussatus mortality 1") == 0)
        {
            for (int k = 0; k < NumberOfClasses; k++)
            {
                PReadWrite->WriteCell(Mortality[k], 8);
                PReadWrite->WriteSeparator();
            }
        }
        else if (strcmp(ParametersNameArray[i], "Assimilation parameter") == 0)
        {
            PReadWrite->WriteCell(AssimilationParameter, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Assimilation maximum") == 0)
        {
            PReadWrite->WriteCell(AssimilationMaximum, 8);
        }

        // parameters from TRDecussatusFoodLimited
        else if (strcmp(ParametersNameArray[i], "Cultivated area") == 0)
        {
            PReadWrite->WriteCell(CultivatedArea, 8);
        }

        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TRDecussatusFoodLimited::Feed()
{
	//double MyHour;
	double nonliving_seston_filtration, MyDepth, MyTidalHeight, MyBoxVolume, MyWaterResidenceTime;
	TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
              * MyTidePointer = MyPEcoDynClass->GetTidePointer(),
              * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
        //C?lculo do tempo de filtra??o
        for (int i = 0; i < NumberOfBoxes; i++)
        {
   	        BoxClearanceRate[i] = 0.0;
   	        for (int k = 0; k < NumberOfClasses; k++)
		{
      	                if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0)
                           )
			{
         	                if (MySPMPointer != NULL)
                                {
            	                        max_filtration_rate = ClearanceRateOfAStandardAnimal(i)/
               	                                (pow(DryWeightOfAStandardAnimal, Filtration_coef));
            	                        ClearanceRate[k * NumberOfBoxes + i] = max_filtration_rate *
                                                      exp(Filtration_coef *
                                                      log(ZooIndividualWeight[k * NumberOfBoxes + i])); // l h-1 ind-1
                                        MyDepth = MyPEcoDynClass->GetBoxDepth(i);
                                        if (MyTidePointer != NULL)
                                        {
               	                                MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight, i,"Tidal height",ObjectCode);
                                                MyDepth = MyDepth + MyTidalHeight / CUBIC;
                                        }
                                        MyBoxVolume = MyDepth * MyPEcoDynClass->GetBoxLength(i) * MyPEcoDynClass->GetChannelWidth(i);
                                        BoxClearanceRate[i] = BoxClearanceRate[i] + ClearanceRate[k * NumberOfBoxes + i] /
                                                           CUBIC *
                                                           ZooClassDensity[k * NumberOfBoxes + i] *
                                                           MyPEcoDynClass->GetBoxLength(i) * MyPEcoDynClass->GetChannelWidth(i) *
                                                           DAYSTOHOURS / DAYSTOSECONDS; //m3 s-1
                                }
                        }
                }
                BoxClearanceTime[i] = MyBoxVolume / BoxClearanceRate[i];
        }
        for (int k = 0; k < NumberOfClasses; k++)
	{
   	        for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0)
				)
			{
         	                if (MySPMPointer != NULL)
                                {
			   	        POC_mass = 0; seston_mass = 0;
                                        MySPMPointer->Inquiry(GetEcoDynClassName(), seston_mass,i,"Suspended matter",ObjectCode);
					MySPMPointer->Inquiry(GetEcoDynClassName(), POC_mass,i,"Particulate organic carbon",ObjectCode);
                                        POCtoSESTON = POC_mass / seston_mass;
                                        MyDepth = MyPEcoDynClass->GetBoxDepth(i);
                                        if (MyTidePointer != NULL)
                                        {
               	                                MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,i,"Tidal height",ObjectCode);
                                                MyDepth = MyDepth + MyTidalHeight / CUBIC;
                                        }
                                        nonliving_seston_filtration = ClearanceRate[k * NumberOfBoxes + i]   //l h-1 ind-1
							                        * seston_mass     //mg l-1
							                        * DAYSTOHOURS     //24
							                        * (ZooClassBiomass[k * NumberOfBoxes + i] //g (meat DW) m-2
                                                                                / ZooIndividualWeight[k * NumberOfBoxes + i]) //g (meat DW) ind-1
                                                                                / MyDepth   //m
                                                                                /DAYSTOSECONDS;
                                                                                // mg s-1 m-3
                                        //Determina??o do tempo de renova??o da ?gua
                                        if (MyTransportPointer != NULL)
                                        {
                                                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyWaterResidenceTime,i,"Water Residence Time",ObjectCode);
                                                if (MyWaterResidenceTime == 0.0)
                                                {
#ifdef __BORLANDC__
                  	                                MessageBox(0, "Zero water residence time in TRDecussatusFoodLimited::Feed", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
																										cerr << "Zero water residence time in TRDecussatusFoodLimited::Feed" << endl;
#endif  // __BORLANDC__
      					                        return;
                                                }
                                        }
   				        else
   				        {
#ifdef __BORLANDC__
   					        MessageBox(0, "Undetermined water redicence time in TRDecussatusFoodLimited::Feed", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
										cerr << "Undetermined water redicence time in TRDecussatusFoodLimited::Feed" << endl;
#endif  // __BORLANDC__

      				                return;
   				        }
                                        if (BoxClearanceTime[i] < MyWaterResidenceTime)
               	                                nonliving_seston_filtration = nonliving_seston_filtration * BoxClearanceTime[i] / MyWaterResidenceTime; //Food limitation
                                        /*if (nonliving_seston_filtration / CUBIC * TimeStep > seston_mass)
                                        {
               	                                nonliving_seston_filtration = seston_mass * CUBIC / TimeStep; // mg dia-1 m-3
                                        } */
                                        nonliving_seston_filtration = nonliving_seston_filtration * MyDepth / CUBIC;  //g s-1 m-2
                                        ZooIndividualFlux[k * NumberOfBoxes + i] = nonliving_seston_filtration
						                                        * (ZooIndividualWeight[k * NumberOfBoxes + i]
                                                                                        / ZooClassBiomass[k * NumberOfBoxes + i]);  // g s-1 ind-1
                                        MySPMPointer->Update(GetEcoDynClassName(), -nonliving_seston_filtration / MyDepth,i,"Suspended matter",ObjectCode);
					ZooPOCUptake[i] = ZooPOCUptake[i]+ nonliving_seston_filtration / MyDepth * POCtoSESTON; //mg C s-1 m-3
					SestonFiltration[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] * DAYSTOSECONDS;
				}
			}
		}
	}
}



TRDecussatusIndivGrowth::TRDecussatusIndivGrowth(TEcoDynClass* APEcoDynClass, char* className)
						 :	TRuditapesDecussatus(APEcoDynClass, className)
{
}

void TRDecussatusIndivGrowth::Go()
{
	double MyJulianDay;
	MyJulianDay = MyPEcoDynClass->GetJulianDay();
	if (MyJulianDay > 31)
        {
	        for (int i = 0; i < NumberOfBoxes; i++)
	        {
		        ZooDINRelease[i] = 0;
		        ZooPhytoUptake[i] = 0;
		        ZooPOCUptake[i] = 0;
	        }
	        Feed();
                Pseudofaeces();
	        Ingestion();
	        Excrete();
	        Assimilate();
	        Respire();
                Reproduce();
	        for (int k = 0; k < NumberOfClasses; k++)
	        {
		        for (int i = 0; i < NumberOfBoxes; i++)
		        {
			        if ((ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				        (ZooClassDensity[k * NumberOfBoxes + i] > 0))
			        {
				        if (ZooIndividualWeight[k * NumberOfBoxes + i] == 0)
				                ZooIndividualWeight[k * NumberOfBoxes + i] = ZooClassBiomass[k * NumberOfBoxes + i]
					                / ZooClassDensity[k * NumberOfBoxes + i];
				        /*A f?rmula abaixo descrita calcula o fluxo liqu?do ao n?vel da popula??o de cada
				        caixa*/
				        ZooClassBiomassFlux[k * NumberOfBoxes + i]			//g dia-1 m-3
					        = ZooClassBiomassFlux[k * NumberOfBoxes + i]
					        + ZooIndividualFlux[k * NumberOfBoxes + i]
					        * (ZooClassBiomass[k * NumberOfBoxes + i]
					        / ZooIndividualWeight[k * NumberOfBoxes + i]);
			        }
		        }
	        }
        }
}

void TRDecussatusIndivGrowth::Integrate()
{
//	Generic = ZooClassBiomass; GenericFlux = ZooClassBiomassFlux;
	Integration(NumberOfClasses, ZooClassBiomass, ZooClassBiomassFlux);
//	ZooClassBiomass = Generic; ZooClassBiomassFlux = GenericFlux;

//	Generic = ZooClassDensity; GenericFlux = ZooClassDensityFlux;
	Integration(NumberOfClasses, ZooClassDensity, ZooClassDensityFlux);
//	ZooClassDensity = Generic; ZooClassDensityFlux = GenericFlux;

	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] > 0 ) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0 )
				)
			{
				ZooIndividualWeight[k * NumberOfBoxes + i]
					= ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooClassDensity[k * NumberOfBoxes + i];
				if (
					 (ZooIndividualWeight[k * NumberOfBoxes + i]
					  >= ClassMaxLimits[NumberOfClasses - 1]) &&
					 (k == (NumberOfClasses - 1))
					)
				{
					ZooClassBiomass[k * NumberOfBoxes + i]
						= ZooClassBiomass[k * NumberOfBoxes + i]
						  - ( ZooIndividualWeight[k * NumberOfBoxes + i]
								- ClassMaxLimits[NumberOfClasses-1])
						  * ZooClassDensity[k * NumberOfBoxes + i];

					ZooIndividualWeight[k * NumberOfBoxes + i]
						= ClassMaxLimits[NumberOfClasses-1];

				}
			}
		}
	}
}


TRuditapesDecussatusPhyto::TRuditapesDecussatusPhyto(TEcoDynClass* APEcoDynClass,
        char* className) : TRuditapesDecussatus(APEcoDynClass, className)
{
}



void TRuditapesDecussatusPhyto::Feed()
{
	double nonliving_seston_filtration, MyDepth, MyTidalHeight,
                MyCarbonPhytoMass, MyPhytoBiomass, MyChlToBiomass, MyPhytoFiltration;
	TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                     * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
                     * MyTidePointer = MyPEcoDynClass->GetTidePointer();
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] > 0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] > 0)
				)
			{
         	                if (MySPMPointer != NULL)
                                {
			   	        POC_mass = 0; seston_mass = 0;
                                        //Debugger(ClearanceRateOfAStandardAnimal(i));
                                        max_filtration_rate = ClearanceRateOfAStandardAnimal(i)/(pow(DryWeightOfAStandardAnimal, Filtration_coef));
            	                        ClearanceRate[k * NumberOfBoxes + i] = max_filtration_rate *
                                                      exp(Filtration_coef *
                                                      log(ZooIndividualWeight[k * NumberOfBoxes + i])); // l h-1 ind-1
                                        //Debugger(ClearanceRate[k * NumberOfBoxes + i]);
                                        MySPMPointer->Inquiry(GetEcoDynClassName(), seston_mass,i,"Suspended matter",ObjectCode);
					MySPMPointer->Inquiry(GetEcoDynClassName(), POC_mass,i,"Particulate organic carbon",ObjectCode);
                                        if (MyPhytoPointer != NULL)
                                        {
               	                                MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyCarbonPhytoMass, //mg m-3
                                                                        i,
                                                                        "Phytoplankton C biomass",
                                                                        ObjectCode);
                                                POCtoSESTON = MyCarbonPhytoMass /
                                                                CUBIC /             //Phytoplankton object returns this as ug, so convert to mg l-1
                                                                seston_mass;
                                        }
                                        else
               	                                POCtoSESTON = POC_mass / seston_mass;
                                        MyDepth = MyPEcoDynClass->GetBoxDepth(i);
                                        if (MyTidePointer != NULL)
                                        {
               	                                MyTidePointer->Inquiry(GetEcoDynClassName(), MyTidalHeight,
                                                                        i,
                                                                        "Tidal height",
                                                                        ObjectCode);
                                                MyDepth = MyDepth + MyTidalHeight / CUBIC;
                                        }
                                        nonliving_seston_filtration = ClearanceRate[k * NumberOfBoxes + i]   //l h-1 ind-1
							                        * seston_mass     //mg l-1
							                        * DAYSTOHOURS     //24
							                        * (ZooClassBiomass[k * NumberOfBoxes + i] //g (meat DW) m-2
                                                                                / ZooIndividualWeight[k * NumberOfBoxes + i]) //g (meat DW) ind-1
                                                                                / MyDepth //m
                                                                                /DAYSTOSECONDS;// mg s-1 m-3
                                        /*if (nonliving_seston_filtration / CUBIC * TimeStep > seston_mass)
               	                                nonliving_seston_filtration = seston_mass
                                                * CUBIC / TimeStep; // mg dia-1 m-3
                                        nonliving_seston_filtration = nonliving_seston_filtration * MyDepth / CUBIC;*/  //g dia-1 m-2
                                        //Debugger(nonliving_seston_filtration);
                                        ZooIndividualFlux[k * NumberOfBoxes + i] = nonliving_seston_filtration
						                                        * (ZooIndividualWeight[k * NumberOfBoxes + i]
                                                                                        / ZooClassBiomass[k * NumberOfBoxes + i]);  // g dia-1 ind-1
                                        //Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
                                        /*MySPMPointer->Update(GetEcoDynClassName(), -nonliving_seston_filtration / CUBIC,
                                        i,
                                        "Suspended matter",
                                        ObjectCode);*/
                                        ZooPOCUptake[i] = ZooPOCUptake[i]+ nonliving_seston_filtration / MyDepth * POCtoSESTON; //mg C s-1 m-3
					SestonFiltration[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] * DAYSTOSECONDS;
                                        if (MyPhytoPointer != NULL)
					{
               	                                MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyPhytoBiomass,i,"Phytoplankton biomass",ObjectCode);
                                                MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlToBiomass,i,"Chlorophyll to biomass",ObjectCode);
                                                MyPhytoBiomass = MyPhytoBiomass * MyChlToBiomass; //mg m-3
                                                MyPhytoFiltration = ClearanceRate[k * NumberOfBoxes + i] *     //l h-1 ind-1
                                                                        (ZooClassBiomass[k * NumberOfBoxes + i] /
                                                                        ZooIndividualWeight[k * NumberOfBoxes + i])/ //l h-1 m-2
                                                                        MyDepth /                                    //l h-1 m-3
                                                                        CUBIC *                                      //h-1
                                                                        MyCarbonPhytoMass *                          //mg m-3 h-1
                                                                        DAYSTOHOURS / DAYSTOSECONDS;                 //mg m-3 s-1

                                                if (MyPhytoFiltration * TimeStep > MyPhytoBiomass)
               		                                MyPhytoFiltration = MyPhytoBiomass  / TimeStep;
						MyPhytoPointer->Update(GetEcoDynClassName(), -MyPhytoFiltration,    // mgC s-1 m-3
														i,
														"Phytoplankton biomass",
														ObjectCode);
                                        }
				}
			}
		}
	}
}


TRDecussatusIndivGrowthPhyto::TRDecussatusIndivGrowthPhyto(TEcoDynClass* APEcoDynClass,
        char* className) : TRuditapesDecussatusPhyto(APEcoDynClass, className)
{
}


void TRDecussatusIndivGrowthPhyto::Go()
{
	double MyJulianDay;
	MyJulianDay = MyPEcoDynClass->GetJulianDay();
	if (MyJulianDay > 31)
        {
	        for (int i = 0; i < NumberOfBoxes; i++)
	        {
		        ZooDINRelease[i] = 0;
		        ZooPhytoUptake[i] = 0;
		        ZooPOCUptake[i] = 0;
	        }
	        Feed();
                Pseudofaeces();
	        Ingestion();
	        Excrete();
	        Assimilate();
	        Respire();
                Reproduce();
	        for (int k = 0; k < NumberOfClasses; k++)
	        {
		        for (int i = 0; i < NumberOfBoxes; i++)
		        {
			        if ((ZooClassBiomass[k * NumberOfBoxes + i] > 0) &&
				        (ZooClassDensity[k * NumberOfBoxes + i] > 0))
			        {
				        if (ZooIndividualWeight[k * NumberOfBoxes + i] == 0)
				        ZooIndividualWeight[k * NumberOfBoxes + i]
					        = ZooClassBiomass[k * NumberOfBoxes + i]
					        / ZooClassDensity[k * NumberOfBoxes + i];
				        /*A f?rmula abaixo descrita calcula o fluxo liqu?do ao n?vel da popula??o de cada
				        caixa*/
				        ZooClassBiomassFlux[k * NumberOfBoxes + i]			//g dia-1 m-3
					        = ZooClassBiomassFlux[k * NumberOfBoxes + i]
					        + ZooIndividualFlux[k * NumberOfBoxes + i]
					        * (ZooClassBiomass[k * NumberOfBoxes + i]
					        / ZooIndividualWeight[k * NumberOfBoxes + i]);
			        }
		        }
	        }
        }
}


void TRDecussatusIndivGrowthPhyto::Integrate()
{
    int i, j, index;
    double const TINNY = 0.000001;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
	for (int k = 0; k < NumberOfClasses; k++)
	{
                for (int m = 0; m < NumberOfBoxesWithRDecussatus/*NumberOfBoxesWithBenthicSpecies*/; m++)
                {
                      //Debugger(m);
                      i = /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[m].LineCoordinate;
                      j = /*MyBenthicSpeciesRecord*/RuditapesDecussatusRecord[m].ColumnCoordinate;
                      //Debugger(i); Debugger(j);
                      if (
                              (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                              (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
                         )
                      {
                          index = i * NumberOfColumns + j;
                          //ZooIndividualFlux[k * NumberOfBoxes + index] = 0.0;
                          //GrowthRate[k * NumberOfBoxes + index] = 0.0;
                          if (
                                  ( ZooClassBiomass[k * NumberOfBoxes + index] > TINNY ) &&
                                  ( ZooClassDensity[k * NumberOfBoxes + index] > TINNY )
                             )
                          {
                                  ZooIndividualWeight[k * NumberOfBoxes + index]
                                  = ZooClassBiomass[k * NumberOfBoxes + index]
                                    / ZooClassDensity[k * NumberOfBoxes + index];
                                  if (
                                          (ZooIndividualWeight[k * NumberOfBoxes + index]
                                           >= ClassMaxLimits[NumberOfClasses - 1]) &&
                                          (k == (NumberOfClasses - 1))
                                     )
                                  {
                                          Debugger(222);
                                          Debugger(ZooIndividualWeight[k * NumberOfBoxes + index]);
                                          Debugger(ClassMaxLimits[NumberOfClasses - 1]);
                                          ZooClassBiomass[k * NumberOfBoxes + index]
                                                  = ZooClassBiomass[k * NumberOfBoxes + i]
                                                  - ( ZooIndividualWeight[k * NumberOfBoxes + index]
                                                  - ClassMaxLimits[NumberOfClasses-1])
                                                  * ZooClassDensity[k * NumberOfBoxes + index];
                                          ZooIndividualWeight[k * NumberOfBoxes + index]
                                                  = ClassMaxLimits[NumberOfClasses-1];

                                  }
                          }
                      }
		}
	}
        //Debugger(111); Debugger(ZooClassBiomass[0]); Debugger(ZooClassBiomassFlux[0]);
//	Generic = ZooClassBiomass; GenericFlux = ZooClassBiomassFlux;
	Integration(NumberOfClasses, ZooClassBiomass, ZooClassBiomassFlux);
//	ZooClassBiomass = Generic; ZooClassBiomassFlux = GenericFlux;
        //Debugger(ZooClassBiomass[0]);
//	Generic = ZooClassDensity; GenericFlux = ZooClassDensityFlux;
	Integration(NumberOfClasses, ZooClassDensity, ZooClassDensityFlux);
//	ZooClassDensity = Generic; ZooClassDensityFlux = GenericFlux;
}


TRDecussatusIndivGrowthPhytoAndTPM::TRDecussatusIndivGrowthPhytoAndTPM(TEcoDynClass* APEcoDynClass,
        char* className) : TRDecussatusIndivGrowthPhyto(APEcoDynClass, className)
{
   DetritusEnergeticContents = 23000; //6100 J g-1
   PhytoEnergeticContents = 23000; //23500 J g-1
   SestonEnergeticContent = DetritusEnergeticContents;
   TINNY = 0.000001;
}

void TRDecussatusIndivGrowthPhytoAndTPM::Go()
{
    char text[64];
    double /*MyJulianDay,*/ MyDepth;
    int i,j, index;
//    MyJulianDay = MyPEcoDynClass->GetJulianDay();
    TEcoDynClass * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    DebugMessage("  Go RDecussatusIndivGrowthPhytoAndTPM");
    for (int m = 0; m < NumberOfBoxesWithRDecussatus/*NumberOfBoxesWithBenthicSpecies*/; m++)
    {
        i = RuditapesDecussatusRecord[m].LineCoordinate;
        j = RuditapesDecussatusRecord[m].ColumnCoordinate;
        if (
                (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
        )
        {
            index = i * NumberOfColumns + j;

            for (int k = 0; k < NumberOfClasses; k++)
            {
               ZooIndividualFlux[k * NumberOfBoxes + index] = 0.0;
               GrowthRate[k * NumberOfBoxes + index] = 0.0;
            }

            if (MyTransportPointer != NULL)
               MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth, index,"Box depth",ObjectCode);
            else MyDepth = MyPEcoDynClass->GetBoxDepth(index);

            if (MyDepth > CriticalWaterDepth)
            {
                ZooDINRelease[index] = 0;
                ZooPhytoUptake[index] = 0;
                ZooPOCUptake[index] = 0;
                Feed(index);
                Pseudofaeces(index);
                Ingestion(index);
                Assimilate(index);
                Respire(index);
                Reproduce(index);
                Die(index);
                for (int k = 0; k < NumberOfClasses; k++)
                {
                    if ((ZooClassBiomass[k * NumberOfBoxes + index] > 0) &&
                    (ZooClassDensity[k * NumberOfBoxes + index] > 0))
                    {
                        GrowthRate[k * NumberOfBoxes + index] = ZooIndividualFlux[k * NumberOfBoxes + index];
                        ZooIndividualFlux[k * NumberOfBoxes + index] = ZooIndividualFlux[k * NumberOfBoxes + index]
                                                                       / MeatEnergeticContents;
                        if (ZooIndividualWeight[k * NumberOfBoxes + index] == 0)
                        ZooIndividualWeight[k * NumberOfBoxes + index]
                                = ZooClassBiomass[k * NumberOfBoxes + index]
                                / ZooClassDensity[k * NumberOfBoxes + index];
                        /*A f?rmula abaixo descrita calcula o fluxo liqu?do ao n?vel da popula??o de cada
                        caixa*/
                        ZooClassBiomassFlux[k * NumberOfBoxes + index]			//g dia-1 m-3
                                = ZooClassBiomassFlux[k * NumberOfBoxes + index]
                                + ZooIndividualFlux[k * NumberOfBoxes + index]
                                * (ZooClassBiomass[k * NumberOfBoxes + index]
                                / ZooIndividualWeight[k * NumberOfBoxes + index]);
                    }
                }
            }
        }
    }
}

void TRDecussatusIndivGrowthPhytoAndTPM::Feed(int ABoxNumber)
{

	double total_seston_filtration, MyDepth,
               MyPhytoFiltration, POM, DetritalC, DetritalN, DetritalP, PhytoC, PhytoN, PhytoP;
        int MyBoxNumber;
        TEcoDynClass *MySPMPointer,
                     *MyPhytoPointer,
                     *MyTransportPointer;
        char EDCname[129];
        MyBoxNumber = ABoxNumber;
	MySPMPointer = MyPEcoDynClass->GetSPMPointer();
        MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer();
        MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
        strcpy(EDCname, GetEcoDynClassName());

	for (int k = 0; k < NumberOfClasses; k++)
	{
                if (
                         ( ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] > TINNY) &&
                         ( ZooClassDensity[k * NumberOfBoxes + MyBoxNumber] > TINNY) &&
                         ( ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] > TINNY)
                        )
                {
                        if (MySPMPointer != NULL)
                        {
                                POC_mass = 0; seston_mass = 0;
                                max_filtration_rate = ClearanceRateOfAStandardAnimal(MyBoxNumber)/(pow(DryWeightOfAStandardAnimal, Filtration_coef));
                                if (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber] < TINNY)
                                    ClearanceRate[k * NumberOfBoxes + MyBoxNumber] = 0.0;
                                else
                                {
                                    ClearanceRate[k * NumberOfBoxes + MyBoxNumber] = max_filtration_rate *
                                                exp(Filtration_coef *
                                                log(ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber])); // l h-1 ind-1
                                    MySPMPointer->Inquiry(EDCname, seston_mass,MyBoxNumber,"TPM",ObjectCode);
                                    MySPMPointer->Inquiry(EDCname, POM,MyBoxNumber, "POM",ObjectCode);
                                    if (seston_mass > TINNY)
                                       OrganicContents = POM / seston_mass;
                                    else OrganicContents = 0.0;
                                    MySPMPointer->Inquiry(EDCname, DetritalC, MyBoxNumber, "DetritalC", ObjectCode);
                                    POC_mass = DetritalC;
                                    MySPMPointer->Inquiry(EDCname, DetritalN, MyBoxNumber, "DetritalN", ObjectCode);
                                    MySPMPointer->Inquiry(EDCname, DetritalP, MyBoxNumber, "DetritalP", ObjectCode);
                                    SestonEnergeticContent = DetritusEnergeticContents;
                                    if (MyPhytoPointer != NULL)
                                    {
                                            MyPhytoPointer->Inquiry(EDCname, PhytoC, //mg m-3
                                                                    MyBoxNumber,
                                                                    "Phytoplankton C biomass",
                                                                    ObjectCode);
                                            POC_mass = DetritalC + PhytoC / CUBIC;
                                            if (POC_mass > TINNY)
                                                SestonEnergeticContent = (
                                                                           DetritusEnergeticContents * DetritalC +
                                                                           PhytoEnergeticContents * PhytoC / CUBIC
                                                                        ) / POC_mass;
                                            else
                                                SestonEnergeticContent = 0.0;

                                    }
                                    if (seston_mass > TINNY)
                                       POCtoSESTON = POC_mass / seston_mass;
                                    else
                                       POCtoSESTON = 0.0;

                                    if (MyTransportPointer != NULL)
                                            MyTransportPointer->Inquiry(EDCname, MyDepth,MyBoxNumber,"Box depth",ObjectCode);
                                    else MyDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
                                    if (seston_mass > TINNY)
                                       total_seston_filtration = ClearanceRate[k * NumberOfBoxes + MyBoxNumber]   //l h-1 ind-1
                                                                            * seston_mass     //mg l-1
                                                                            * DAYSTOHOURS     //24
                                                                            * (ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] //g (meat DW) m-2
                                                                            / ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber]) //g (meat DW) ind-1
                                                                            /*/ MyDepth *///m
                                                                            /DAYSTOSECONDS
                                                                            /CUBIC;// g s-1 m-2
                                    else total_seston_filtration = 0.0;
                                    ZooIndividualFlux[k * NumberOfBoxes + MyBoxNumber] = total_seston_filtration
                                                                                    * (ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber]
                                                                                    / ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber]);  // g s-1 ind-1

                                    ZooPOCUptake[MyBoxNumber] = ZooPOCUptake[MyBoxNumber]+ total_seston_filtration / MyDepth * POCtoSESTON * DAYSTOSECONDS; //g C d-1 m-3  ou mg C d-1 L-1
                                    SestonFiltration[k * NumberOfBoxes + MyBoxNumber] = ZooIndividualFlux[k * NumberOfBoxes + MyBoxNumber] * DAYSTOSECONDS;
                                    if (MyPhytoPointer != NULL)
                                    {
                                            if (PhytoC > 0.000001)
                                            {
                                                    MyPhytoPointer->Inquiry(EDCname, PhytoN,MyBoxNumber, "Phytoplankton N biomass", ObjectCode);
                                                    MyPhytoPointer->Inquiry(EDCname, PhytoP,MyBoxNumber, "Phytoplankton P biomass", ObjectCode);
                                                    MyPhytoFiltration = ClearanceRate[k * NumberOfBoxes + MyBoxNumber] *     //l h-1 ind-1
                                                                    (ZooClassBiomass[k * NumberOfBoxes + MyBoxNumber] /
                                                                    ZooIndividualWeight[k * NumberOfBoxes + MyBoxNumber])/ //l h-1 m-2
                                                                    MyDepth /                                    //l h-1 m-3
                                                                    CUBIC *                                      //h-1
                                                                    PhytoC *                          //mg m-3 h-1
                                                                    DAYSTOHOURS / DAYSTOSECONDS;                 //mg m-3 s-1

                                                    MyPhytoPointer->Update(EDCname, -MyPhytoFiltration,    // mgC s-1 m-3
                                                                                                            MyBoxNumber,
                                                                                                            "Phytoplankton biomass",
                                                                                                            ObjectCode);
                                                    MyPhytoPointer->Update(EDCname, -MyPhytoFiltration * PhytoN / PhytoC,    // mgC s-1 m-3
                                                                                                            MyBoxNumber,
                                                                                                            "Phytoplankton N biomass",
                                                                                                            ObjectCode);
                                                    MyPhytoPointer->Update(EDCname, -MyPhytoFiltration * PhytoP / PhytoC,    // mgC s-1 m-3
                                                                                                            MyBoxNumber,
                                                                                                            "Phytoplankton P biomass",
                                                                                                            ObjectCode);
                                            }
                                            else MyPhytoFiltration = 0.0;
                                    }
                                    if (total_seston_filtration > 0.0)
                                    {
                                        MySPMPointer->Update(EDCname, -total_seston_filtration / MyDepth,    // mg s-1 L-1 ou g s-1 m-3
                                                                                                                MyBoxNumber,
                                                                                                                "TPM",
                                                                                                                ObjectCode);
                                        MySPMPointer->Update(EDCname, -total_seston_filtration / MyDepth * POM / seston_mass,    // mgC s-1 L-1
                                                                                                                MyBoxNumber,
                                                                                                                "POM",
                                                                                                                ObjectCode);
                                        MySPMPointer->Update(EDCname, -total_seston_filtration / MyDepth * DetritalC / seston_mass,    // mgC s-1 L-1
                                                                                                                MyBoxNumber,
                                                                                                                "DetritalC",
                                                                                                                ObjectCode);
                                        MySPMPointer->Update(EDCname, -total_seston_filtration / MyDepth * DetritalN / seston_mass,    // mgC s-1 L-1
                                                                                                                MyBoxNumber,
                                                                                                                "DetritalN",
                                                                                                                ObjectCode);
                                        MySPMPointer->Update(EDCname, -total_seston_filtration / MyDepth * DetritalP / seston_mass,    // mgC s-1 L-1
                                                                                                                MyBoxNumber,
                                                                                                                "DetritalP",
                                                                                                                ObjectCode);
                                    }
                                }
                        }
                }
	}
}

void TRDecussatusIndivGrowthPhytoAndTPM::Pseudofaeces(int ABoxNumber)
{
    double delta;
    int i;
    i = ABoxNumber;
    for (int k = 0; k < NumberOfClasses; k++)
    {
        if (
                 (ZooClassBiomass[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooClassDensity[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooIndividualWeight[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooIndividualFlux[k * NumberOfBoxes + i] > 0.0)
           )
        {
            delta = ThresholdConsumptionForPseudofaeces /ZooIndividualWeight[k * NumberOfBoxes + i] -
                    SestonFiltration[k * NumberOfBoxes + i];
            if (delta > 0) delta = 0;
            pseudo_faeces_rate = pseudo_faeces_maxo * (1 - exp(xkp * delta));
            pseudofaeces_production[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] * pseudo_faeces_rate;//   g s-1 ind-1
        }
    }
}


void TRDecussatusIndivGrowthPhytoAndTPM::Ingestion(int ABoxNumber)
{
    //Debugger(11);
    int i;
    i = ABoxNumber;
    for (int k = 0; k < NumberOfClasses; k++)
    {
        if (
                 (ZooClassBiomass[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooClassDensity[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooIndividualWeight[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooIndividualFlux[k * NumberOfBoxes + i] > 0.0)
                )
        {
                ZooIndividualFlux[k * NumberOfBoxes + i]
                        = ZooIndividualFlux[k * NumberOfBoxes + i] -  pseudofaeces_production[k * NumberOfBoxes + i]; //g s-1
                //Debugger(ZooIndividualFlux[k * NumberOfBoxes + i]);
        }
    }
}


void TRDecussatusIndivGrowthPhytoAndTPM::Assimilate(int ABoxNumber)
{
    double const ProportionOfTimeInEmersion = 0.0;
    int i;
    i = ABoxNumber;
    for (int k = 0; k < NumberOfClasses; k++)
    {
        if (
                 (ZooClassBiomass[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooClassDensity[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooIndividualWeight[k * NumberOfBoxes + i] > TINNY) &&
                 (ZooIndividualFlux[k * NumberOfBoxes + i] > 0.0)
           )
        {
            if (OrganicContents > TINNY)
               assimilation_efficiency[k * NumberOfBoxes + i] = MAX(MinimumAssimilationEfficiency,AssimilationMaximum - AssimilationParameter / OrganicContents);
            else
               assimilation_efficiency[k * NumberOfBoxes + i] = 0.0;
            ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] *
                                   POCtoSESTON *
                                   assimilation_efficiency[k * NumberOfBoxes + i] *
                                   (1 - ProportionOfTimeInEmersion) * //g s-1 ind-1
                                    SestonEnergeticContent;// j s-1 ind-1

            /*GrowthRate[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] *
                                SestonEnergeticContent;*/// j s-1 ind-1*/
        }
    }
}


void TRDecussatusIndivGrowthPhytoAndTPM::Respire(int ABoxNumber)
{
    double Respiration, MyDepth, MyTidalHeight;
    double const AssimptoticWeight = 1.0; //g/ind
    double const JoulesToOxgen = 0.0000702; //gO2/Joule - considerando que 32 mg O2 - 456 J - tese da Paula Sobral p?g. 44)
                                            // => 0.032g/456Joules = 0.0000702 gO2/Joule
    TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    int i;
    i = ABoxNumber;
    for (int k = 0; k < NumberOfClasses; k++)
    {
        if (
                 ( ZooClassBiomass[k * NumberOfBoxes + i] > TINNY) &&
                 ( ZooClassDensity[k * NumberOfBoxes + i] > TINNY) &&
                 ( ZooIndividualWeight[k * NumberOfBoxes + i] > TINNY)
                )
        {
            if (ZooIndividualWeight[k * NumberOfBoxes + i] < AssimptoticWeight)
               Respiration = RespirationOfAStandardAnimal(i) * pow(ZooIndividualWeight[k * NumberOfBoxes + i] /
                DryWeightOfAStandardAnimal, RespirationCoefficient) / DAYSTOSECONDS;//j s-1 ind-1
            //else Respiration = GrowthRate[k * NumberOfBoxes + i];
            else Respiration = ZooIndividualFlux[k * NumberOfBoxes + i];


            //GrowthRate[k * NumberOfBoxes + i] = GrowthRate[k * NumberOfBoxes + i] - Respiration;// j s-1 ind-1*/
            /*ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] -
                                                       Respiration / MeatEnergeticContents; */	//g s-1 ind-1
            ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] - Respiration; // j s-1 ind-1*/
            if (MyTransportPointer != NULL)
               MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,i,"Box depth",ObjectCode);
            else MyDepth = MyPEcoDynClass->GetBoxDepth(i);
            //Respiration = Respiration / MeatEnergeticContents * ZooClassDensity[k * NumberOfBoxes + i] /MyDepth;
            Respiration = Respiration * JoulesToOxgen * ZooClassDensity[k * NumberOfBoxes + i] /MyDepth; //gO2/m3
            if (MyNutrientPointer != NULL)
              MyNutrientPointer->Update(GetEcoDynClassName(),-Respiration/*mg / l*/,i,"Oxygen",ObjectCode);
            Excrete(i, k, Respiration);
        }
    }
}

void TRDecussatusIndivGrowthPhytoAndTPM::Excrete(int ABoxNumber, int AClassNumber, double ARespirationRate)
{
   int i = ABoxNumber; int k = AClassNumber;
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   double NH4OxygenRatio = 0.21; //mol N-NH4 / mol O2 Valor m?dio estimado a partir da Fig. 4.1 (p?g. 66) da tese da Paula Sobral
   double NH4ToEnergy = 0.349; //J/microM N-NH4
   double ExcretionRate, EnergyLossThroughExcretion, MyDepth;

   if (MyTransportPointer != NULL)
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,i,"Box depth",ObjectCode);
   else MyDepth = MyPEcoDynClass->GetBoxDepth(i);

   ExcretionRate = ARespirationRate / OXYGENATOMICWEIGHT * NH4OxygenRatio; //mmol N-NH4 / L /s
   ZooDINRelease[ABoxNumber] = ExcretionRate * CUBIC; //micromol N-NH4 / L / s
   EnergyLossThroughExcretion = ZooDINRelease[ABoxNumber] * NH4ToEnergy * CUBIC /*J m-3 s-1*/
                               / (ZooClassDensity[k * NumberOfBoxes + i] / MyDepth); //J/ ind / s-1
   //GrowthRate[k * NumberOfBoxes + i] = GrowthRate[k * NumberOfBoxes + i] - EnergyLossThroughExcretion;// j s-1 ind-1
   /*ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] -
                                              EnergyLossThroughExcretion / MeatEnergeticContents; */	//g s-1 ind-1
   ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] - EnergyLossThroughExcretion;// j s-1 ind-1
   if (MyNutrientPointer != NULL)
   {
        MyNutrientPointer->Update(GetEcoDynClassName(), ZooDINRelease[ABoxNumber] * CUBIC/*micromol / m3*/, i,"Ammonia",ObjectCode);
   }
}

void TRDecussatusIndivGrowthPhytoAndTPM::Reproduce(int ABoxNumber)
{
    JulianDay = MyPEcoDynClass->GetJulianDay();
    double Reproduction;
    int i;
    i = ABoxNumber;
    for (int k = 0; k < NumberOfClasses; k++)
    {
        if (
             ( ZooClassBiomass[k * NumberOfBoxes + i] > TINNY) &&
             ( ZooClassDensity[k * NumberOfBoxes + i] > TINNY) &&
             ( ZooIndividualWeight[k * NumberOfBoxes + i] > TINNY)
            )
        {
          if ((JulianDay > 181) && (JulianDay < 304))
          {
             Reproduction = 0.009 * ZooIndividualWeight[k * NumberOfBoxes + i] / DAYSTOSECONDS;
             /*ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] -
                                    Reproduction;//g s-1 ind-1
             GrowthRate[k * NumberOfBoxes + i] = GrowthRate[k * NumberOfBoxes + i] - Reproduction * MeatEnergeticContents;*/ // j s-1 ind-1
             ZooIndividualFlux[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] - Reproduction * MeatEnergeticContents;
          }
        }
    }
}

void TRDecussatusIndivGrowthPhytoAndTPM::ShellDeposition(int ABoxNumber)
{

}

void TRDecussatusIndivGrowthPhytoAndTPM::OrganicDeposition(int ABoxNumber)
{

}

void TRDecussatusIndivGrowthPhytoAndTPM::Die(int ABoxNumber)
{
    int i;
    i = ABoxNumber;
    for (int k = 0; k < NumberOfClasses; k++)
    {
        if (
                 ( ZooClassBiomass[k * NumberOfBoxes + i] > TINNY) &&
                 ( ZooClassDensity[k * NumberOfBoxes + i] > TINNY) &&
                 ( ZooIndividualWeight[k * NumberOfBoxes + i] > TINNY)
           )
        {
            Mortality[k * NumberOfBoxes + i] = ZooClassDensity[k * NumberOfBoxes + i] -
                           ZooClassDensity[k * NumberOfBoxes + i] *
                           exp(-IntrinsicRateOfMortality * 1.0);
            if (Mortality[k * NumberOfBoxes + i] < 0.0) Mortality[k * NumberOfBoxes + i] = 0.0;
            ZooClassDensityFlux[k * NumberOfBoxes + i] = ZooClassDensityFlux[k * NumberOfBoxes + i] - Mortality[k * NumberOfBoxes + i] / DAYSTOSECONDS;
            ZooClassBiomassFlux[k * NumberOfBoxes + i] = ZooClassBiomassFlux[k * NumberOfBoxes + i] - Mortality[k * NumberOfBoxes + i] / DAYSTOSECONDS *
                                                         ZooIndividualWeight[k * NumberOfBoxes + i];
        }
    }
}



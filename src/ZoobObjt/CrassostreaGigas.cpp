// Zoobenthos object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.06.13
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

// TZoobenthos Class

TCrassostreaGigas::TCrassostreaGigas(TEcoDynClass* APEcoDynClass, char* className)
						 :	 TSuspensionFeeders(APEcoDynClass, className)
{

	int i, j, k, m, n, X, Y, MyNumberOfClasses, Class = 0/*, K = 0*/;
    int XV, YV;
    double MyValue = 0;
	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("CrassostreaGigas");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "CrassostreaGigas parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas::TCrassostreaGigas - parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        // Just read number of classes
        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Number of classes") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyNumberOfClasses);
                    NumberOfClasses = MyNumberOfClasses;
                }
                else
                if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TotalFWToMeatDW);
                }
            }
        }
        ClassMaxLimits  = new double[NumberOfClasses];
        ClassMinLimits  = new double[NumberOfClasses];

        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            // read in the parameter names

            bool classValuesFound = false;
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                if (strcmp(MyParameter, "Meat conversion") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        meat_conversion = MyValue;
                }
                /* ... previously read...
                else
                if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        TotalFWToMeatDW = MyValue;
                }*/
                else
                if (strcmp(MyParameter, "Nitrogen to DW") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        nitrogen_dw = MyValue;
                }
                else
                if (strcmp(MyParameter, "Standard filtration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        Standard_filtration = MyValue;
                }
                else
                if (strcmp(MyParameter, "xkf") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        xkf = MyValue;
                }
                else
                if (strcmp(MyParameter, "Saturation filtration level") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        saturation_level = MyValue;
                }
                else
                if (strcmp(MyParameter, "Filtration coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        Filtration_coef = MyValue;
                }
                else
                if (strcmp(MyParameter, "Assimilation slope") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        assimilation_slope = MyValue;
                }
                else
                if (strcmp(MyParameter, "Assimilation efficiency") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        assimilation_efficiency0 = MyValue;
                }
                else
                if (strcmp(MyParameter, "Respiration slope") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        respiration_slope = MyValue;
                }
                else
                if (strcmp(MyParameter, "Standard respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                         standard_respiration0 = MyValue;
                }
                else
                if (strcmp(MyParameter, "Pseudofaeces maxo") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        pseudo_faeces_maxo = MyValue;
                }
                else
                if (strcmp(MyParameter, "Pseudofaeces maxi") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        pseudo_faeces_maxi = MyValue;
                }
                else
                if (strcmp(MyParameter, "c1o") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        c1o = MyValue;
                }
                else
                if (strcmp(MyParameter, "c1i") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        c1i = MyValue;
                }
                else
                if (strcmp(MyParameter, "xkpo") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        xkpo = MyValue;
                }
                else
                if (strcmp(MyParameter, "xkpi") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        xkpi = MyValue;
                }
                else
                if (strcmp(MyParameter, "c2o") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        c2o = MyValue;
                }
                else
                if (strcmp(MyParameter, "c2i") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        c2i = MyValue;
                }
                else
                if (strcmp(MyParameter, "xkp2o") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        xkp2o = MyValue;
                }
                else
                if (strcmp(MyParameter, "xkp2i") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        xkp2i = MyValue;
                }
                else
                if (strcmp(MyParameter, "CalToJoules") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        cal_joules = MyValue;
                }
                else
                if (strcmp(MyParameter, "Oyster_g_cal") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        Oyster_g_cal = MyValue;
                }
                else
                if (strcmp(MyParameter, "Carbon_g_cal") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        Carbon_g_cal = MyValue;
                }
                else
                if (strcmp(MyParameter, "Oxygen_ml_cal") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        Oxygen_ml_cal = MyValue;
                }
                else
                if (strcmp(MyParameter, "Faeces to nitrogen") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //for (int j = 0; j < NumberOfBoxes; j++)
                        faeces_nitrogen = MyValue;
                }
                else  // this solution is a little bit spaghetti (AP, 2007.07.13)
                if (strcmp(MyParameter, "Licensed area") == 0)
                {
                    if (PReadWrite->FindString("Licensed area values", XV, YV))
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(XV, YV + 1 + j, HabitatArea[j]);
                        }
                        if (PReadWrite->FindString("ClassMinLimits values", XV, YV))
                            classValuesFound = true;
                    }
                    else
                    {
                        for (int j = 0; j < NumberOfBoxes; j++)
                        {
                            PReadWrite->ReadNumber(X+3+j, i, MyValue);
                            HabitatArea[j] = MyValue;
                        }
                    }
                }
                else
                if (!classValuesFound &&
                    (
                     (i > Y + NumberOfParameters - NumberOfClasses - 1) &&
                     (i < Y + NumberOfParameters)
                    ))
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    ClassMinLimits[Class] = MyValue * TotalFWToMeatDW;
                    Class++;
                };

            }
            if (classValuesFound)  // spaghetti continues (AP, 2007.07.13)
            {
                for (int i = 0; i < NumberOfClasses; i++)
                {
                    PReadWrite->ReadNumber(XV, YV + 1 + i, MyValue);
                    ClassMinLimits[Class] = MyValue * TotalFWToMeatDW;
                }
            }
            for (i = 0; i < NumberOfClasses; i++)
                if (i < NumberOfClasses - 1)
                    ClassMaxLimits[i] = ClassMinLimits[i + 1];
                else
                    ClassMaxLimits[i] = 1000;
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "C. gigas Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrassostreaGigas::TCrassostreaGigas - parameters undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }


	// Create generics

/*	Generic                 = new double[NumberOfBoxes * NumberOfClasses];
	GenericLoad             = new double[NumberOfBoxes * NumberOfClasses];
	GenericFlux             = new double[NumberOfBoxes * NumberOfClasses];*/
	ZooClassBiomass         = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensity         = new double[NumberOfBoxes * NumberOfClasses];
	ZooInitialWeight        = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualWeight     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassBiomassFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooClassDensityFlux     = new double[NumberOfBoxes * NumberOfClasses];
	ZooIndividualFlux       = new double[NumberOfBoxes * NumberOfClasses];
	SestonFiltration        = new double[NumberOfBoxes * NumberOfClasses];
	pseudofaeces_production = new double[NumberOfBoxes * NumberOfClasses];
	seston_ingestion        = new double[NumberOfBoxes * NumberOfClasses];
	org_seston_mass         = new double[NumberOfBoxes * NumberOfClasses];
	total_seston_mass       = new double[NumberOfBoxes * NumberOfClasses];
	assimilation_efficiency = new double[NumberOfBoxes * NumberOfClasses];
	transition_number       = new double[NumberOfClasses * NumberOfClasses];
	transition_biomass      = new double[NumberOfClasses * NumberOfClasses];
	transition_weight       = new double[NumberOfClasses * NumberOfClasses];
	GrowthRate              = new double[NumberOfBoxes * NumberOfClasses];
	ClearanceRate           = new double[NumberOfBoxes * NumberOfClasses];
	Mortality               = new double[NumberOfBoxes * NumberOfClasses];

	for (m = 0; m < NumberOfClasses; m++)
		for (n = 0; n < NumberOfBoxes; n++)
		{
/*			Generic[m * NumberOfBoxes + n]                 = 0;
			GenericLoad[m * NumberOfBoxes + n]             = 0;
			GenericFlux[m * NumberOfBoxes + n]             = 0;*/
			ZooClassBiomass[m * NumberOfBoxes + n] 	     = 0;
			ZooClassDensity[m * NumberOfBoxes + n]         = 0;
			ZooInitialWeight[m * NumberOfBoxes + n]        = 0;
			ZooIndividualWeight[m * NumberOfBoxes + n]     = 0;
			ZooClassBiomassFlux[m * NumberOfBoxes + n]     = 0;
			ZooClassDensityFlux[m * NumberOfBoxes + n]     = 0;
			ZooIndividualFlux[m * NumberOfBoxes + n]       = 0;
			SestonFiltration[m * NumberOfBoxes + n]        = 0;
			pseudofaeces_production[m * NumberOfBoxes + n] = 0;
			seston_ingestion[m * NumberOfBoxes + n]        = 0;
			org_seston_mass[m * NumberOfBoxes + n]         = 0;
			total_seston_mass[m * NumberOfBoxes + n]       = 0;
			assimilation_efficiency[m * NumberOfBoxes + n] = 0;
			GrowthRate[m * NumberOfBoxes + n] 	           = 0;
			ClearanceRate[m * NumberOfBoxes + n] 	        = 0;
			Mortality[m * NumberOfBoxes + n] 	     		  = 0;
		}

	for (m = 0; m < NumberOfClasses; m++)
		for (n = 0; n < NumberOfClasses; n++)
		{
			transition_number[m * NumberOfClasses + n]  = 0;
			transition_biomass[m * NumberOfClasses + n] = 0;
			transition_weight[m * NumberOfClasses + n]  = 0;
		}
	// Read in the variables
	VariableCount = 0;

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("CrassostreaGigas");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "CrassostreaGigas variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TCrassostreaGigas::TCrassostreaGigas - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {

        if (PReadWrite->FindString("Crassostrea gigas", X, Y))
        {

            PReadWrite->ReadNumber( X+1, Y, NumberOfVariables);
            // Create VNA array for variables
            VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            for (i = Y; i < Y + NumberOfVariables; i++)
                PReadWrite->ReadString( X+2, i, VariableNameArray[i-Y]);

            int offset;
            if (PReadWrite->FindString("ZooClassDensity values", XV,YV))
            {
                for (k = 0; k < NumberOfClasses; k++)
                {
                    offset = k * NumberOfBoxes;
                    for (j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue );
                        ZooClassDensity[offset + j] = MyValue;
                        ZooClassDensity[offset + j] = ZooClassDensity[offset + j]
                                                             * HabitatArea[j]/
                                                             BoxArray[j].Volume;
                    }
                }
                PReadWrite->FindString("ZooIndividualWeight values", XV,YV);
                for (k = 0; k < NumberOfClasses; k++)
                {
                    offset = k * NumberOfBoxes;
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue );
                        ZooIndividualWeight[offset + j] = MyValue * TotalFWToMeatDW;
                        ZooInitialWeight[offset + j] = MyValue * TotalFWToMeatDW;
                        ZooClassBiomass[offset + j] = ZooClassDensity[offset + j] *
                                                             ZooIndividualWeight[offset + j];
                    }
                }
                PReadWrite->FindString("ZooIndividualFlux values", XV,YV);
                for (k = 0; k < NumberOfClasses; k++)
                {
                    offset = k * NumberOfBoxes;
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber(XV + k, YV + 1 + j, MyValue );
                        ZooIndividualFlux[offset + j] = MyValue *
                            TotalFWToMeatDW *
                            TimeStep;
                    }
                }
            }
            else {  // old file format
                i = Y;
                for (k = 0; k < NumberOfClasses; k++)
                {
                    offset = k * NumberOfBoxes;
                    for (j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber( X + 5 + j, i, MyValue );
                        ZooClassDensity[offset + j] = MyValue;
                        ZooClassDensity[offset + j] = ZooClassDensity[offset + j]
                                                             * HabitatArea[j]/
                                                             BoxArray[j].Volume;
                    }
                    i++;
                }
                for (k = 0; k < NumberOfClasses; k++)
                {
                    offset = k * NumberOfBoxes;
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber( X + 5 + j, i, MyValue );
                        ZooIndividualWeight[offset + j] = MyValue * TotalFWToMeatDW;
                        ZooInitialWeight[offset + j] = MyValue * TotalFWToMeatDW;
                        ZooClassBiomass[offset + j] = ZooClassDensity[offset + j] *
                                                             ZooIndividualWeight[offset + j];
                    }
                    i++;
                }
                for (k = 0; k < NumberOfClasses; k++)
                {
                    offset = k * NumberOfBoxes;
                    for (int j = 0; j < NumberOfBoxes; j++)
                    {
                        PReadWrite->ReadNumber( X + 5 + j, i, MyValue );
                        ZooIndividualFlux[offset + j] = MyValue *
                            TotalFWToMeatDW *
                            TimeStep;
                    }
                    i++;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TCrassostreaGigas::TCrassostreaGigas - variables undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TCrassostreaGigas::~TCrassostreaGigas()
{
    freeMemory();
}

void TCrassostreaGigas::freeMemory()
{
    try {
    //	delete [] Generic;
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
        delete [] seston_ingestion;
        delete [] org_seston_mass;
        delete [] total_seston_mass;
        delete [] assimilation_efficiency;
        delete [] transition_number;
        delete [] transition_biomass;
        delete [] transition_weight;
        delete [] GrowthRate;
       delete [] ClearanceRate;
       delete [] Mortality;
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
				cerr << "TCrassostreaGigas::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TCrassostreaGigas::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Number of classes") == 0)
    {
        value = NumberOfClasses;
    }
    else if (strcmp(MyParameter, "Meat conversion") == 0)
    {
        value = meat_conversion;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        value = TotalFWToMeatDW;
    }
    else if (strcmp(MyParameter, "Nitrogen to DW") == 0)
    {
        value = nitrogen_dw;
    }
    else if (strcmp(MyParameter, "Standard filtration") == 0)
    {
        value = Standard_filtration;
    }
    else if (strcmp(MyParameter, "xkf") == 0)
    {
        value = xkf;
    }
    else if (strcmp(MyParameter, "Saturation filtration level") == 0)
    {
        value = saturation_level;
    }
    else if (strcmp(MyParameter, "Filtration coefficient") == 0)
    {
        value = Filtration_coef;
    }
    else if (strcmp(MyParameter, "Assimilation slope") == 0)
    {
        value = assimilation_slope;
    }
    else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
    {
        value = assimilation_efficiency0;
    }
    else if (strcmp(MyParameter, "Respiration slope") == 0)
    {
        value = respiration_slope;
    }
    else if (strcmp(MyParameter, "Standard respiration") == 0)
    {
        value = standard_respiration0;
    }
    else if (strcmp(MyParameter, "Pseudofaeces maxo") == 0)
    {
        value = pseudo_faeces_maxo;
    }
    else if (strcmp(MyParameter, "Pseudofaeces maxi") == 0)
    {
        value = pseudo_faeces_maxi;
    }
    else if (strcmp(MyParameter, "c1o") == 0)
    {
        value = c1o;
    }
    else if (strcmp(MyParameter, "c1i") == 0)
    {
        value = c1i;
    }
    else if (strcmp(MyParameter, "xkpo") == 0)
    {
        value = xkpo;
    }
    else if (strcmp(MyParameter, "xkpi") == 0)
    {
        value = xkpi;
    }
    else if (strcmp(MyParameter, "c2o") == 0)
    {
        value = c2o;
    }
    else if (strcmp(MyParameter, "c2i") == 0)
    {
        value = c2i;
    }
    else if (strcmp(MyParameter, "xkp2o") == 0)
    {
        value = xkp2o;
    }
    else if (strcmp(MyParameter, "xkp2i") == 0)
    {
        value = xkp2i;
    }
    else if (strcmp(MyParameter, "CalToJoules") == 0)
    {
        value = cal_joules;
    }
    else if (strcmp(MyParameter, "Oyster_g_cal") == 0)
    {
        value = Oyster_g_cal;
    }
    else if (strcmp(MyParameter, "Carbon_g_cal") == 0)
    {
        value = Carbon_g_cal;
    }
    else if (strcmp(MyParameter, "Oxygen_ml_cal") == 0)
    {
        value = Oxygen_ml_cal;
    }
    else if (strcmp(MyParameter, "Faeces to nitrogen") == 0)
    {
        value = faeces_nitrogen;
    }
    else if (strcmp(MyParameter, "Licensed area") == 0)
    {
        value = HabitatArea[0];
    }
    else
        value = 0.0;
    return value;
}

bool TCrassostreaGigas::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "Number of classes") == 0)
    {
        NumberOfClasses = value;
    }
    else if (strcmp(MyParameter, "Meat conversion") == 0)
    {
        meat_conversion = value;
    }
    else if (strcmp(MyParameter, "TotalFW to meatDW") == 0)
    {
        TotalFWToMeatDW = value;
    }
    else if (strcmp(MyParameter, "Nitrogen to DW") == 0)
    {
        nitrogen_dw = value;
    }
    else if (strcmp(MyParameter, "Standard filtration") == 0)
    {
        Standard_filtration = value;
    }
    else if (strcmp(MyParameter, "xkf") == 0)
    {
        xkf = value;
    }
    else if (strcmp(MyParameter, "Saturation filtration level") == 0)
    {
        saturation_level = value;
    }
    else if (strcmp(MyParameter, "Filtration coefficient") == 0)
    {
        Filtration_coef = value;
    }
    else if (strcmp(MyParameter, "Assimilation slope") == 0)
    {
        assimilation_slope = value;
    }
    else if (strcmp(MyParameter, "Assimilation efficiency") == 0)
    {
        assimilation_efficiency0 = value;
    }
    else if (strcmp(MyParameter, "Respiration slope") == 0)
    {
        respiration_slope = value;
    }
    else if (strcmp(MyParameter, "Standard respiration") == 0)
    {
        standard_respiration0 = value;
    }
    else if (strcmp(MyParameter, "Pseudofaeces maxo") == 0)
    {
        pseudo_faeces_maxo = value;
    }
    else if (strcmp(MyParameter, "Pseudofaeces maxi") == 0)
    {
        pseudo_faeces_maxi = value;
    }
    else if (strcmp(MyParameter, "c1o") == 0)
    {
        c1o = value;
    }
    else if (strcmp(MyParameter, "c1i") == 0)
    {
        c1i = value;
    }
    else if (strcmp(MyParameter, "xkpo") == 0)
    {
        xkpo = value;
    }
    else if (strcmp(MyParameter, "xkpi") == 0)
    {
        xkpi = value;
    }
    else if (strcmp(MyParameter, "c2o") == 0)
    {
        c2o = value;
    }
    else if (strcmp(MyParameter, "c2i") == 0)
    {
        c2i = value;
    }
    else if (strcmp(MyParameter, "xkp2o") == 0)
    {
        xkp2o = value;
    }
    else if (strcmp(MyParameter, "xkp2i") == 0)
    {
        xkp2i = value;
    }
    else if (strcmp(MyParameter, "CalToJoules") == 0)
    {
        cal_joules = value;
    }
    else if (strcmp(MyParameter, "Oyster_g_cal") == 0)
    {
        Oyster_g_cal = value;
    }
    else if (strcmp(MyParameter, "Carbon_g_cal") == 0)
    {
        Carbon_g_cal = value;
    }
    else if (strcmp(MyParameter, "Oxygen_ml_cal") == 0)
    {
        Oxygen_ml_cal = value;
    }
    else if (strcmp(MyParameter, "Faeces to nitrogen") == 0)
    {
        faeces_nitrogen = value;
    }
    else if (strcmp(MyParameter, "Licensed area") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            HabitatArea[j] = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TCrassostreaGigas::SaveVariables()
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
    PReadWrite->WriteCell("ZooClassDensity values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZooIndividualWeight values");
    for (int k = 1; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteSeparator();
        sprintf(classText, "Class %-d", k + 1);
        PReadWrite->WriteCell(classText);
    }
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZooIndividualFlux values");
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
            PReadWrite->WriteCell(ZooClassDensity[offset + j]
                    * BoxArray[j].Volume / HabitatArea[j], 8);
            PReadWrite->WriteSeparator();
        }

        // individual weight by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooIndividualWeight[offset + j] / TotalFWToMeatDW, 8);
            PReadWrite->WriteSeparator();
        }

        // individual fluxes by class
        for (int k = 0; k < NumberOfClasses; k++)
        {
            offset = k * NumberOfBoxes;
            PReadWrite->WriteCell(ZooIndividualFlux[offset + j]
                    / (TotalFWToMeatDW * TimeStep), 8);
            PReadWrite->WriteSeparator();
        }
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TCrassostreaGigas::SaveParameters()
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
        else if (strcmp(ParametersNameArray[i], "Meat conversion") == 0)
        {
            PReadWrite->WriteCell(meat_conversion, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TotalFW to meatDW") == 0)
        {
            PReadWrite->WriteCell(TotalFWToMeatDW, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Nitrogen to DW") == 0)
        {
            PReadWrite->WriteCell(nitrogen_dw, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Standard filtration") == 0)
        {
            PReadWrite->WriteCell(Standard_filtration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkf") == 0)
        {
            PReadWrite->WriteCell(xkf, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Saturation filtration level") == 0)
        {
            PReadWrite->WriteCell(saturation_level, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Filtration coefficient") == 0)
        {
            PReadWrite->WriteCell(Filtration_coef, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Assimilation slope") == 0)
        {
            PReadWrite->WriteCell(assimilation_slope, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Assimilation efficiency") == 0)
        {
            PReadWrite->WriteCell(assimilation_efficiency0, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration slope") == 0)
        {
            PReadWrite->WriteCell(respiration_slope, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Standard respiration") == 0)
        {
            PReadWrite->WriteCell(standard_respiration0, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Pseudofaeces maxo") == 0)
        {
            PReadWrite->WriteCell(pseudo_faeces_maxo, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Pseudofaeces maxi") == 0)
        {
            PReadWrite->WriteCell(pseudo_faeces_maxi, 8);
        }
        else if (strcmp(ParametersNameArray[i], "c1o") == 0)
        {
            PReadWrite->WriteCell(c1o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "c1i") == 0)
        {
            PReadWrite->WriteCell(c1i, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpo") == 0)
        {
            PReadWrite->WriteCell(xkpo, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkpi") == 0)
        {
            PReadWrite->WriteCell(xkpi, 8);
        }
        else if (strcmp(ParametersNameArray[i], "c2o") == 0)
        {
            PReadWrite->WriteCell(c2o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "c2i") == 0)
        {
            PReadWrite->WriteCell(c2i, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkp2o") == 0)
        {
            PReadWrite->WriteCell(xkp2o, 8);
        }
        else if (strcmp(ParametersNameArray[i], "xkp2i") == 0)
        {
            PReadWrite->WriteCell(xkp2i, 8);
        }
        else if (strcmp(ParametersNameArray[i], "CalToJoules") == 0)
        {
            PReadWrite->WriteCell(cal_joules, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oyster_g_cal") == 0)
        {
            PReadWrite->WriteCell(Oyster_g_cal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon_g_cal") == 0)
        {
            PReadWrite->WriteCell(Carbon_g_cal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Oxygen_ml_cal") == 0)
        {
            PReadWrite->WriteCell(Oxygen_ml_cal, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Faeces to nitrogen") == 0)
        {
            PReadWrite->WriteCell(faeces_nitrogen, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Licensed area") == 0)
        {
            PReadWrite->WriteCell(HabitatArea[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("ClassMinLimits values");
    PReadWrite->WriteSeparator(true);
    for (int k = 0; k < NumberOfClasses; k++)
    {
        PReadWrite->WriteCell(ClassMinLimits[k] / TotalFWToMeatDW, 8);
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    PReadWrite->WriteCell("Licensed area values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(HabitatArea[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TCrassostreaGigas:: Go()
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		ZooDINRelease[i] = 0;
		ZooPhytoUptake[i] = 0;
		ZooPOCUptake[i] = 0;
	}

	//C?lculo do fluxo l?quido ao n?vel individual e da popula??o, com base nos processos metab?licos}
	Feed();

	// MessageBox(0, "M?todo feed OKAY", "EcoDynamo alert", MB_OK);
	Pseudofaeces();

	// MessageBox(0, "M?todo pseudofaeces OKAY", "EcoDynamo alert", MB_OK);
	Ingestion();

	// MessageBox(0, "M?todo ingestion OKAY", "EcoDynamo alert", MB_OK);
	Excrete();

	// MessageBox(0, "M?todo excrete OKAY", "EcoDynamo alert", MB_OK);
	//Ingest?o de seston pela popula??o da caixa}
	// seston_ingestion := seston_ingestion * (ZooClassBiomass^[i,k]/ZooIndividualWeight^[i,k])*HoursInDay;
	Assimilate();

	// MessageBox(0, "M?todo assimilate OKAY", "EcoDynamo alert", MB_OK);
	Respire();

	Die();

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
				ZooInitialWeight[k * NumberOfBoxes + i]
					= ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooClassDensity[k * NumberOfBoxes + i];
				if (ZooInitialWeight[k * NumberOfBoxes + i] > ClassMaxLimits[k])
					ZooInitialWeight[k * NumberOfBoxes + i] = ClassMaxLimits[k];


				/*A f?rmula abaixo descrita calcula o fluxo liqu?do ao n?vel da popula??o de cada
				caixa*/
				ZooClassBiomassFlux[k * NumberOfBoxes + i]			//g dia-1 m-3
					= ZooClassBiomassFlux[k * NumberOfBoxes + i]
					  + (ZooIndividualFlux[k * NumberOfBoxes + i]
					  * DAYSTOHOURS)
					  * (ZooClassBiomass[k * NumberOfBoxes + i]
					  / ZooIndividualWeight[k * NumberOfBoxes + i]);
				/*if seston_ingestion > 0 then
				ecological_efficiency^[i,k] := (ZooClassBiomass_flux^[i,k]*time_conv/
				(seston_ingestion*org_seston_mass/total_seston_mass))*100;
				if org_seston_mass > 0 then
				food_chain_efficiency^[i,k] := (ZooClassBiomass_flux^[i,k]/total_seston_mass)*100* time_conv;*/
			}
		}
	}

/*	//C?lculo do fluxo l?quido ao n?vel individual e da popula??o, com base nos processos metab?licos}
	Feed();

	// MessageBox(0, "M?todo feed OKAY", "EcoDynamo alert", MB_OK);
	Pseudofaeces();

	// MessageBox(0, "M?todo pseudofaeces OKAY", "EcoDynamo alert", MB_OK);
	Ingestion();

	// MessageBox(0, "M?todo ingestion OKAY", "EcoDynamo alert", MB_OK);
	Excrete();

	// MessageBox(0, "M?todo excrete OKAY", "EcoDynamo alert", MB_OK);
	//Ingest?o de seston pela popula??o da caixa}
	// seston_ingestion := seston_ingestion * (ZooClassBiomass^[i,k]/ZooIndividualWeight^[i,k])*HoursInDay;
	Assimilate();

	// MessageBox(0, "M?todo assimilate OKAY", "EcoDynamo alert", MB_OK);
	Respire();
	// MessageBox(0, "M?todo respire OKAY", "EcoDynamo alert", MB_OK);
	//O m?todo deposition ? utilizado ao n?vel da popula??o. Calcula
	//assim a deposi??o da popula??o de cada caixa. ? invocado a partir do m?todo
	//assimilation. O m?todo excretion ? invocado a partir do mesmo m?todo e calcula
	//a excre??o ao n?vel do individuo e da popula??o}
	Die();  */

}

void TCrassostreaGigas::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												char* ParameterName,
												int AnObjectCode)
{
	int i,j;
	char MyParameter[65], // 64 characters (max) for parameter name
					MyName[65];
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

    if (strstr(MyParameter, "C. gigas density " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas density %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooClassDensity[NumberOfBoxes * j + BoxNumber]
                          * BoxArray[BoxNumber].Volume;
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas individual weight " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas individual weight %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooIndividualWeight[NumberOfBoxes * j + BoxNumber]
                          /TotalFWToMeatDW;
                break;
            }
        }
    }
    else if (strstr(MyParameter, "C. gigas biomass " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas biomass %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooClassBiomass[NumberOfBoxes * j + BoxNumber]
                          * BoxArray[BoxNumber].Volume
                          / (CUBIC * CUBIC * TotalFWToMeatDW); // values in TFW(live weight) na area em que existe a esp?cie;
                break;
            }
        }
    }
    else if (strstr(MyParameter, "Individual growth " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Individual growth %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ZooIndividualFlux[NumberOfBoxes * j + BoxNumber]; // Values in day-1
                break;
            }
        }
    }
    else if (strstr(MyParameter, "Clearance rate " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Clearance rate %i", j + 1 );
            if (strcmp(MyParameter, MyName) == 0) {
                Value = ClearanceRate[NumberOfBoxes * j + BoxNumber]; // Values in litres h-1
                break;
            }
        }
    }
    else if (strcmp(MyParameter, "Zoobenthos DIN Release") == 0)
        Value = ZooDINRelease[BoxNumber];

    else if (strcmp(MyParameter, "Zoobenthos Phyto Uptake") == 0)
        Value = ZooPhytoUptake[BoxNumber];

    else if (strcmp(MyParameter, "Zoobenthos POC Uptake") == 0)
        Value = ZooPOCUptake[BoxNumber];

    else {
        for (i = 0; i < NumberOfVariables; i++)
            if (strcmp(MyParameter, VariableNameArray[i]) == 0)
                break;

        if (i == NumberOfVariables)
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
						cerr << "TCrassostreaGigas::Inquiry 6 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
		Value = 0;
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}


bool TCrassostreaGigas::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									      char* VariableName)
{
    bool rc = true;
	char MyName[65]; // 64 characters (max) for parameter name
	int j;
    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strstr(VariableName, "C. gigas density " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas density %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooClassDensity[NumberOfBoxes * j + BoxNumber] = Value
                           / BoxArray[BoxNumber].Volume;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas individual weight " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas individual weight %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooIndividualWeight[NumberOfBoxes * j + BoxNumber] = Value
                          * TotalFWToMeatDW;
                break;
            }
        }
    }
    else if (strstr(VariableName, "C. gigas biomass " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "C. gigas biomass %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooClassBiomass[NumberOfBoxes * j + BoxNumber] = Value
                          / BoxArray[BoxNumber].Volume
                          * (CUBIC * CUBIC * TotalFWToMeatDW); // values in TFW(live weight) na area em que existe a esp?cie;
                break;
            }
        }
    }
    else if (strstr(VariableName, "Individual growth " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Individual growth %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ZooIndividualFlux[NumberOfBoxes * j + BoxNumber] = Value; // Values in day-1
                break;
            }
        }
    }
    else if (strstr(VariableName, "Clearance rate " ) != NULL)
    {
        for (j = 0; j < NumberOfClasses; j++)
        {
            sprintf( MyName, "Clearance rate %i", j + 1 );
            if (strcmp(VariableName, MyName) == 0) {
                ClearanceRate[NumberOfBoxes * j + BoxNumber] = Value; // Values in litres h-1
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

void TCrassostreaGigas::Inquiry(char* srcName, double &Value,
												int BoxNumber,
												double ExtraValue,
												int AnObjectCode)
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


void TCrassostreaGigas::Update(char* srcName, double Value,
											double ExtraValue,
											int BoxNumber,
											int ClassNumber,
											int AnObjectCode)
{ //This works for recruitment or harvest of individuals to/from any class
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


void TCrassostreaGigas::Update(char* srcName, double Value,
											double ExtraValue,
											int BoxNumber,
											int AnObjectCode)
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


void TCrassostreaGigas:: Integrate()
{
//	Generic = ZooClassBiomass; GenericFlux = ZooClassBiomassFlux;
	Integration(NumberOfClasses, ZooClassBiomass, ZooClassBiomassFlux);
//	ZooClassBiomass = Generic; ZooClassBiomassFlux = GenericFlux;

//	Generic = ZooClassDensity; GenericFlux = ZooClassDensityFlux;
	Integration(NumberOfClasses, ZooClassDensity, ZooClassDensityFlux);
//	ZooClassDensity = Generic; ZooClassDensityFlux = GenericFlux;

	// testar com numeros diferentes de caixas e classes para ver
	// se esta OK - 96.06.26 JGF/PD

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

				//Debugger(i);
				//Debugger(k);
				//Debugger(ZooIndividualWeight[i * NumberOfBoxes + k]);
				//Debugger(ZooClassBiomass[i * NumberOfBoxes + k]);
				//Debugger(ZooClassDensity[i * NumberOfBoxes + k]);

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
			/*else
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] <= 0 ) ||
				 ( ZooClassDensity[k * NumberOfBoxes + ik] <= 0 )
				)
			{
				Debugger(3000);
				Debugger(ZooClassBiomass[k * NumberOfBoxes + i]);
				Debugger(ZooClassDensity[k * NumberOfBoxes + ik]);
				ZooIndividualWeight[k * NumberOfBoxes + i] = 0;
				ZooInitialWeight[k * NumberOfBoxes + i] = 0;
			}
			Debugger(ZooInitialWeight[k * NumberOfBoxes + i]);*/
		}
	}
	//ClassTransitionNormal();
	//class_transition;
}

void TCrassostreaGigas::Feed()
{
//var  phyto_name, spm_name  : shortchartype;
	double phyto_mass, phyto_biomass,
			 phyto_filtration,
			 nonliving_seston_filtration, delta,
			 ChlorophyllCarbonRatio, ChlorophyllToBiomass, POCtoSESTON;

	TEcoDynClass* MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
				 *	MySPMPointer = MyPEcoDynClass->GetSPMPointer();

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
//				 Debugger(ZooClassBiomass[k * NumberOfBoxes + i]);
//				 Debugger(ZooClassDensity[k * NumberOfBoxes + i]);
//				 Debugger(ZooIndividualWeight[k * NumberOfBoxes + i]);
			POC_mass = 0; seston_mass = 0;


			if (ZooClassBiomass[k * NumberOfBoxes + i] <= 0)
				SestonFiltration[k * NumberOfBoxes + i] = 0;
			else
			{
				if (MyPhytoPointer != NULL)
				{
					MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,
														 i,
														 "Phytoplankton biomass",
														 ObjectCode);
					MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllCarbonRatio,
														 i,
														 "Chlorophyll : Carbon",
														 ObjectCode);
					MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,
														 i,
														 "Chlorophyll to biomass",
														 ObjectCode);
					// Phytoplankton returns its mass as ug chl a l-1
					// convert to mg l-1 and mg C l-1, respectively
					phyto_biomass = phyto_mass * ChlorophyllToBiomass /
										 CUBIC; //mg/l
					phyto_mass = phyto_mass / (ChlorophyllCarbonRatio *
									 CUBIC); //mg C l-1
				}

				if (MySPMPointer != NULL)
				{
					MySPMPointer->Inquiry(GetEcoDynClassName(), seston_mass,
													  i,
													  "Suspended matter",
													  ObjectCode);
					MySPMPointer->Inquiry(GetEcoDynClassName(), POC_mass,
													  i,
													  "Particulate organic carbon",
													  ObjectCode);
				}
				POCtoSESTON = POC_mass / seston_mass;
				org_seston_mass[k * NumberOfBoxes + i] = phyto_mass + POC_mass;
				total_seston_mass[k * NumberOfBoxes + i] = phyto_biomass + seston_mass;

//				Debugger(10);

				/*if org_seston_mass < 0 then debugger ('org_seston_mass',org_seston_mass);
			  {Chla a expressa em mgCm-3 }
			  {Biomassa da Crassostrea expressa em gDW/m3 }
				if (total_seston_mass <= 0)then debugger ('total_seston',total_seston_mass);*/
				if (total_seston_mass[k * NumberOfBoxes + i] <= 0)
					SestonFiltration[k * NumberOfBoxes + i] = 0;
				else
				{
					delta = saturation_level -
							total_seston_mass[k * NumberOfBoxes + i];
					if (delta > 0)
						delta = 0;
					max_filtration_rate = Standard_filtration *
												exp(xkf*delta);
					//Rela??o alom?trica para c?lculo da taxa de filtra??o
					filtration_rate/*l/h/ind*/ = max_filtration_rate *
													 exp(Filtration_coef *
													 log(ZooIndividualWeight[k * NumberOfBoxes + i]));

					ClearanceRate[k * NumberOfBoxes + i] = filtration_rate;

					if (MyPhytoPointer != NULL)
					{
						phyto_filtration        // mgC day-1 m-3
							= filtration_rate
							  * phyto_mass       // mgC/l
							  * DAYSTOHOURS
							  * (ZooClassBiomass[k * NumberOfBoxes + i]
								  /ZooIndividualWeight[k * NumberOfBoxes + i]);



						if (phyto_filtration * TimeStep > phyto_mass * CUBIC)
							phyto_filtration = phyto_mass
													 *
													 CUBIC
													 / TimeStep;

						MyPhytoPointer->Update(GetEcoDynClassName(), -phyto_filtration,    // mgC day-1 m-3
														i,
														"Phytoplankton biomass",
														ObjectCode);
					}
					ZooPhytoUptake[i] = ZooPhytoUptake[i] + phyto_filtration; //mgC day-1 m-3
					ZooIndividualFlux[k * NumberOfBoxes + i]     // g h-1
						= phyto_filtration
						  * ChlorophyllCarbonRatio
						  * ChlorophyllToBiomass
						  / CUBIC
						  / DAYSTOHOURS
						  * ( ZooIndividualWeight[k * NumberOfBoxes + i]
								/ ZooClassBiomass[k * NumberOfBoxes + i]);



					if (MySPMPointer != NULL)
					{
						nonliving_seston_filtration      // mg dia-1 m-3
							= filtration_rate
							  * seston_mass
							  * DAYSTOHOURS
							  * (ZooClassBiomass[k * NumberOfBoxes + i]
								  / ZooIndividualWeight[k * NumberOfBoxes + i]);

						if (nonliving_seston_filtration * TimeStep > seston_mass)
							nonliving_seston_filtration
								= seston_mass
								  / TimeStep;

						MySPMPointer->Update(GetEcoDynClassName(), -nonliving_seston_filtration / CUBIC, // to return in mg l-1 PD & JGF 96.06.26
													 i,
													 "Suspended matter",
													 ObjectCode);
					}

					ZooPOCUptake[i] = ZooPOCUptake[i]                   //mgC day-1 m-3
											+ nonliving_seston_filtration
											* POCtoSESTON;

					ZooIndividualFlux[k * NumberOfBoxes + i]            // g h-1 ind-1
						= ZooIndividualFlux[k * NumberOfBoxes + i]
						  + nonliving_seston_filtration
						  / ( CUBIC * DAYSTOHOURS )
						  * (ZooIndividualWeight[k * NumberOfBoxes + i]
							  / ZooClassBiomass[k * NumberOfBoxes + i]);

					SestonFiltration[k * NumberOfBoxes + i]
						= ZooIndividualFlux[k * NumberOfBoxes + i];
				}
			}

			} // End of if

		}
	}
}



void TCrassostreaGigas::Pseudofaeces() //C?lculo das pseudofezes adaptado do m?todo descrito em Raillard(1991)
                                       //Assumiu-se que a taxa de produ??o de pseudofezes org?nicas e
                                       //minerais ? a mesma. 
{

	double delta1, delta2;

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

			if (c1o /
			CUBIC -                // em g h-1 tal como o consumo
			SestonFiltration[k * NumberOfBoxes + i] /
			ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
			delta1 = c1o /
					 CUBIC -         // em g h-1 tal como o consumo
					 SestonFiltration[k * NumberOfBoxes + i] /
					 ZooIndividualWeight[k * NumberOfBoxes + i];
			else
				delta1 = 0;

			if (c2o /
			CUBIC -                // em g h-1 tal como o consumo
			SestonFiltration[k * NumberOfBoxes + i] /
			ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
			delta2 = c2o /
					 CUBIC -         // em g h-1 tal como o consumo
					 SestonFiltration[k * NumberOfBoxes + i] /
					 ZooIndividualWeight[k * NumberOfBoxes + i];
			else
				delta2 = 0;

			pseudo_faeces_rate = pseudo_faeces_maxo *
								(1-exp(xkpo*delta1)) +
								(1-pseudo_faeces_maxo) *
								(1-exp(xkp2o*delta2));

	  /*		if (c1i /
			CUBIC -                    // em g h-1 tal como o consumo
			SestonFiltration[k * NumberOfBoxes + i] /
			ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
				delta1 = c1i /
						CUBIC -           // em g h-1 tal como o consumo
						SestonFiltration[k * NumberOfBoxes + i] /
						ZooIndividualWeight[k * NumberOfBoxes + i];
			else
				delta1 = 0;

			if (c2i /
			CUBIC -                   // em g h-1 tal como o consumo
			SestonFiltration[k * NumberOfBoxes + i] /
			ZooIndividualWeight[k * NumberOfBoxes + i] < 0)
				delta2 = c2i /
						  CUBIC -         // em g h-1 tal como o consumo
						  SestonFiltration[k * NumberOfBoxes + i] /
						  ZooIndividualWeight[k * NumberOfBoxes + i];
			else
				delta2 = 0;
			pseudo_faeces_rate = pseudo_faeces_rate +
									pseudo_faeces_maxi *
									(1-exp(xkpi*delta1))+
									(1-pseudo_faeces_maxi)*
									(1-exp(xkp2i*delta2));  */
			pseudofaeces_production[k * NumberOfBoxes + i] = ZooIndividualFlux[k * NumberOfBoxes + i] *
											  pseudo_faeces_rate;

			} // end of if

		}
	}
}


void TCrassostreaGigas::Ingestion()
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
				seston_ingestion[k * NumberOfBoxes + i]
					= ZooIndividualFlux[k * NumberOfBoxes + i]
					  -  pseudofaeces_production[k * NumberOfBoxes + i];
				ZooIndividualFlux[k * NumberOfBoxes + i]
					= seston_ingestion[k * NumberOfBoxes + i];
			}
		}
	}
}

void TCrassostreaGigas::Assimilate()
{
	double MyWaterTemperature;
	int MyBoxNumber/*, Class = -1*/;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

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

			if (MyWaterTemperaturePointer != NULL)
			MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
															i,
															"Water temperature",
															ObjectCode);
			else
			{
#ifdef __BORLANDC__
				MessageBox(0,
						"Water temperature object is undefined",
						"EcoDynamo alert",
						MB_OK);
#else  // __BORLANDC__
				cerr << "TCrassostreaGigas::Assimilate - Water temperature object undefined" << endl;
#endif  // __BORLANDC__
			return;
			}
			assimilation_efficiency[k * NumberOfBoxes + i] = assimilation_slope *
											  MyWaterTemperature +
											  assimilation_efficiency0;

			ZooIndividualFlux[k * NumberOfBoxes + i]
				= ZooIndividualFlux[k * NumberOfBoxes + i]
				* Carbon_g_cal
				* (org_seston_mass[k * NumberOfBoxes + i]
				/ total_seston_mass[k * NumberOfBoxes + i])
				* assimilation_efficiency[k * NumberOfBoxes + i]
				* meat_conversion;

			} // of if
		}
	}
	/*

	Para calcular a assimila??o ? necess?rio utilizar somente a frac??o org?nica ingerida, da? a multiplica??o
	na f?rmula abaixo descrita do zoo_biomass_flux pela raz?o entre a frac??o org?nica e a frac??o total.
	A convers?o para energia faz-se de modo a poder exprimir a entrada de M.O. em calorias e compatibilizar
	os c?lculos com a f?rmula do balan?o energ?tico descrita em Raillard(1991)
	//deposition(i,k);
	//{excretion(i,k);
	zoo_individual_flux^[i,k] := zoo_individual_flux^[i,k]* Carbon_g_cal *
	(Org_seston_mass[k * NumberOfBoxes + i]/Total_seston_mass[k * NumberOfBoxes + i])*
										  assimilation_efficiency *
										  meat_conversion;

	*/
	//excretion(i,k);
}

void TCrassostreaGigas::Excrete()
{
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

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

				// Perdas por excre??o ao n?vel do individuo}
				ZooIndividualFlux[k * NumberOfBoxes + i]         // Bernard (1974)
					= ZooIndividualFlux[k * NumberOfBoxes + i]
					  - seston_ingestion[k * NumberOfBoxes + i]
					  * 0.004;

				// Excre??o ao n?vel da popula??o
				total_excretion = seston_ingestion[k * NumberOfBoxes + i]       // Bernard (1974)
										* 0.004
										* (ZooClassBiomass[k * NumberOfBoxes + i]
										/ ZooIndividualWeight[k * NumberOfBoxes + i]);
				//Os valores de am?nia s?o devolvidos em umol m-3 d-1}
				if (MyNutrientPointer != NULL)
				{
					//return positive value because this is added}
					//ammonia_excretion = ammonia_excretion + deposit_elimination * faeces_nitrogen;}
					ammonia_excretion = total_excretion /
											nitrogen_dw /
											NITROGENATOMICWEIGHT *
											CUBIC*
											CUBIC*
											DAYSTOHOURS;
//					Debugger(ammonia_excretion);
					ZooDINRelease[i] = ZooDINRelease[i] +  ammonia_excretion;
//					Debugger(ZooDINRelease[i]);

					MyNutrientPointer->Update(GetEcoDynClassName(), ammonia_excretion,
												i,
												"Ammonia",
												ObjectCode);
				}

			} // of if
		}
	}
}


void TCrassostreaGigas::Respire()
{
	double  MyWaterTemperature;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();

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

			if (MyWaterTemperaturePointer != NULL)
			MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
																i,
																"Water temperature",
																ObjectCode);
			else
			{
#ifdef __BORLANDC__
				MessageBox(0,
						"Water temperature object is undefined",
						"EcoDynamo alert",
						MB_OK);
#else  // __BORLANDC__
				cerr << "TCrassostreaGigas::Respire - Water temperature object undefined" << endl;
#endif  // __BORLANDC__
				return;
			}
			standard_respiration = (respiration_slope *
											MyWaterTemperature +
											standard_respiration0);//mlO2/h/g DW
			// Debugger(ZooIndividualWeight[k * NumberOfBoxes + i]);
			total_respiration = standard_respiration *
									  exp( 0.7 * log(ZooIndividualWeight[k * NumberOfBoxes + i]))
									  * Oxygen_ml_cal;

	/*

	O consumo de oxig?nio ? convertido em cal, de modo a poder calcular o consumo equivalente de
	biomassa. Na f?rmula abaixo descrita, o zoo-flux ? reconvertido a g para poder ser utilizado
	no m?todo "production" para actualiza??o da biomassa individual e populacional.

	*/
//			if (ZooIndividualFlux[k * NumberOfBoxes + i] < (total_respiration * meat_conversion))
//				::MessageBox(0, "Hello...","",MB_OK);
			ZooIndividualFlux[k * NumberOfBoxes + i]
				= (ZooIndividualFlux[k * NumberOfBoxes + i]
				- total_respiration
				* meat_conversion)
				/ Oyster_g_cal;

			} // of if
		}
	}
}

void TCrassostreaGigas::Reproduce()
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
				gamete_emission = seston_ingestion[k * NumberOfBoxes + i] *
														0.18; // Bernard (1974)
				ZooClassBiomassFlux[k * NumberOfBoxes + i]
					= ZooClassBiomassFlux[k * NumberOfBoxes + i]
					  - gamete_emission;
			}

		}
	}
}


void TCrassostreaGigas:: OrganicDeposition()
{
	faeces_production = 0;
	deposit_elimination = 0;

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
				// Acumula??o de dep?sitos pela popula??o da caixa
				faeces_production = faeces_production +
										 (1-assimilation_efficiency[k * NumberOfBoxes + i]) *
										  ZooIndividualFlux[k * NumberOfBoxes + i];
				deposit_elimination = deposit_elimination +
											 (pseudofaeces_production[k * NumberOfBoxes + i] + faeces_production) *
											 (ZooClassBiomass[k * NumberOfBoxes + i]/
											  ZooIndividualWeight[k * NumberOfBoxes + i])
											  *DAYSTOHOURS;
			}
		}
	}
}


void TCrassostreaGigas:: ShellDeposition()
{
  /*	shell_deposition := seston_ingestion * 0.0045; {Bernard, 1974}
	ZooClassbiomass_flux^[i,k] := zoo_biomass_flux^[i,k] - shell_deposition; */
}


void TCrassostreaGigas::Die()
{
	double MeanTFWDW = 0.03;
	for (int k = 0; k < NumberOfClasses; k++)
	{
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (
				 ( ZooClassBiomass[k * NumberOfBoxes + i] != 0.0) &&
				 ( ZooClassDensity[k * NumberOfBoxes + i] != 0.0) &&
				 ( ZooIndividualWeight[k * NumberOfBoxes + i] != 0.0)
				)
			{
				//if (k == 0)
				if (ZooIndividualWeight[k * NumberOfBoxes + i] < 5.0 *
					 MeanTFWDW)
				natural_mortality = 0.042 *
										  ZooClassDensity[k * NumberOfBoxes + i];
				//if (k > 0)
				else
				{
					if ((JulianDay > 179) && (JulianDay < 271))
						natural_mortality = 0.0025
												  / 30
												  * ZooClassDensity[k * NumberOfBoxes + i];
					else // Relat?rio dos irlandeses
						natural_mortality = 0.01
												  / 30
												  * ZooClassDensity[k * NumberOfBoxes + i];
				}
				ZooClassDensityFlux[k * NumberOfBoxes + i] =
					  ZooClassDensityFlux[k * NumberOfBoxes + i]
					  - natural_mortality;
				Mortality[k * NumberOfBoxes + i] = natural_mortality /
					  ZooClassDensity[k * NumberOfBoxes + i];
				ZooClassBiomassFlux[k * NumberOfBoxes + i] 	=
					  ZooClassBiomassFlux[k * NumberOfBoxes + i] -
					  natural_mortality *
					  ZooIndividualWeight[k * NumberOfBoxes + i];
			}
		}
	}
}


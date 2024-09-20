// Phytoplankton object CPP code file
// @ J. Gomes Ferreira  & Pedro Duarte
// Rev. A 96.01.22
/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <math.h>
	#include <stdio.h>
	#include <vcl.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <cstdio>
	#include <cmath> 
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "phytobjt.h"
#include "iodll.h"

// Tejo phytoplankton with N, P, Si limitation

TTejoNPSiPhytoplankton::TTejoNPSiPhytoplankton(TEcoDynClass* APEcoDynClass,
        char* className) : TTejoPhytoplankton(APEcoDynClass, className)
{

	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        int X,Y;
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //		ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "KsDIN") == 0)
                {
                    double MyKsDIN = 0;
                    PReadWrite->ReadNumber(X+3, i, MyKsDIN);
                        KsDIN = MyKsDIN; //umol l-1
                }
                else
                if (strcmp(MyParameter, "KsPO4") == 0)
                {
                    double MyKsPO4 = 0;
                    PReadWrite->ReadNumber(X+3, i, MyKsPO4);
                        KsPO4 = MyKsPO4; //umol l-1
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - phytoplankton", "EcoDynamo alert", MB_OK);
#else  //__BORLANDC__
						cerr << "TTejoNPSiPhytoplankton::TTejoNPSiPhytoplankton - Undefined object - phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

double TTejoNPSiPhytoplankton::GetParameterValue(char* MyParameter)
{
  double value;

    // parameters from TPhytoplankton
    if (strcmp(MyParameter, "Pmax") == 0)
    {
        value = Pmax[0];
    }
    else if (strcmp(MyParameter, "Iopt") == 0)
    {
        value = Iopt[0];
    }
    else if (strcmp(MyParameter, "Slope") == 0)
    {
        value = Slope[0];
    }
    else if (strcmp(MyParameter, "DefaultAEiler") == 0)
    {
        value = AEiler[0];
    }
    else if (strcmp(MyParameter, "BEiler") == 0)
    {
        value = BEiler[0];
    }
    else if (strcmp(MyParameter, "CEiler") == 0)
    {
        value = CEiler[0];
    }
    else if (strcmp(MyParameter, "KValue") == 0)
    {
        value = KValue;
    }
    else if (strcmp(MyParameter, "Ks") == 0)
    {
        value = Ks[0];
    }
    else if (strcmp(MyParameter, "KAmmonia") == 0)
    {
        value = KAmmonia[0];
    }
    else if (strcmp(MyParameter, "KNitrate") == 0)
    {
        value = KNitrate[0];
    }
    else if (strcmp(MyParameter, "phi") == 0)
    {
        value = phi;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        value = MaintenanceRespiration[0];
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        value = RespirationCoefficient;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon loss") == 0)
    {
        value = DocLoss;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon nutrient stress loss") == 0)
    {
        value = DocStressLoss;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        value = DeathLoss;
    }
    else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
    {
        value = RedfieldCFactor;
    }
    else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
    {
        value = RedfieldNFactor;
    }
    else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
    {
        value = RedfieldPFactor;
    }
    else if (strcmp(MyParameter, "TemperatureAugmentationRate") == 0)
    {
        value = TemperatureAugmentationRate;
    }
    else if (strcmp(MyParameter, "Falling speed") == 0)
    {
        value = FallingSpeed;
    }
    else if (strcmp(MyParameter, "Light/Dark resp") == 0)
    {
        value = RatioLightDarkRespiration;
    }
    else if (strcmp(MyParameter, "NumberOfLoads") == 0)
    {
        value = NumberOfLoads;
    }
    else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
    {
        value = NumberOfDaysForLoads;
    }
    else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
    {
        value = NumberOfRiverLoads;
    }

    // parameters from TTejoPhytoplankton
    else if (strcmp(MyParameter, "q10PH") == 0)
    {
        value = q10PH;
    }
    else if (strcmp(MyParameter, "RTMPH") == 0)
    {
        value = RTMPH;
    }

    // parameters from TTejoNPSiPhytoplankton
    else if (strcmp(MyParameter, "KsDIN") == 0)
    {
        value = KsDIN;
    }
    else if (strcmp(MyParameter, "KsPO4") == 0)
    {
        value = KsPO4;
    }
    else
        value = 0.0;
    return value;
}

bool TTejoNPSiPhytoplankton::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    // parameters from TPhytoplankton
    if (strcmp(MyParameter, "Pmax") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Pmax[j] = value;
    }
    else if (strcmp(MyParameter, "Iopt") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Iopt[j] = value;
    }
    else if (strcmp(MyParameter, "Slope") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Slope[j] = value;
    }
    else if (strcmp(MyParameter, "DefaultAEiler") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            AEiler[j] = value;
    }
    else if (strcmp(MyParameter, "BEiler") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            BEiler[j] = value;
    }
    else if (strcmp(MyParameter, "CEiler") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            CEiler[j] = value;
    }
    else if (strcmp(MyParameter, "KValue") == 0)
    {
        KValue = value;
    }
    else if (strcmp(MyParameter, "Ks") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            Ks[j] = value;
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
    else if (strcmp(MyParameter, "phi") == 0)
    {
        phi = value;
    }
    else if (strcmp(MyParameter, "Maintenance respiration") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            MaintenanceRespiration[j] = value;
    }
    else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
    {
        RespirationCoefficient = value;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon loss") == 0)
    {
        DocLoss = value;
    }
    else if (strcmp(MyParameter, "Dissolved organic carbon nutrient stress loss") == 0)
    {
        DocStressLoss = value;
    }
    else if (strcmp(MyParameter, "Death loss") == 0)
    {
        DeathLoss = value;
    }
    else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
    {
        RedfieldCFactor = value;
    }
    else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
    {
        RedfieldNFactor = value;
    }
    else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
    {
        RedfieldPFactor = value;
    }
    else if (strcmp(MyParameter, "TemperatureAugmentationRate") == 0)
    {
        TemperatureAugmentationRate = value;
    }
    else if (strcmp(MyParameter, "Falling speed") == 0)
    {
        FallingSpeed = value;
    }
    else if (strcmp(MyParameter, "Light/Dark resp") == 0)
    {
        RatioLightDarkRespiration = value;
    }
    else if (strcmp(MyParameter, "NumberOfLoads") == 0)
    {
        NumberOfLoads = value;
    }
    else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
    {
        NumberOfDaysForLoads = value;
    }
    else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
    {
        NumberOfRiverLoads = value;
    }

    // parameters from TTejoPhytoplankton
    else if (strcmp(MyParameter, "q10PH") == 0)
    {
        q10PH = value;
    }
    else if (strcmp(MyParameter, "RTMPH") == 0)
    {
        RTMPH = value;
    }

    // parameters from TTejoNPSiPhytoplankton
    else if (strcmp(MyParameter, "KsDIN") == 0)
    {
        KsDIN = value;
    }
    else if (strcmp(MyParameter, "KsPO4") == 0)
    {
        KsPO4 = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.12
bool TTejoNPSiPhytoplankton::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Phytoplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Phytoplankton");
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
        else if (strcmp(ParametersNameArray[i], "Iopt") == 0)
        {
            PReadWrite->WriteCell(Iopt[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Slope") == 0)
        {
            PReadWrite->WriteCell(Slope[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "DefaultAEiler") == 0)
        {
            PReadWrite->WriteCell(AEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "BEiler") == 0)
        {
            PReadWrite->WriteCell(BEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "CEiler") == 0)
        {
            PReadWrite->WriteCell(CEiler[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KValue") == 0)
        {
            PReadWrite->WriteCell(KValue, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Ks") == 0)
        {
            PReadWrite->WriteCell(Ks[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KAmmonia") == 0)
        {
            PReadWrite->WriteCell(KAmmonia[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNitrate") == 0)
        {
            PReadWrite->WriteCell(KNitrate[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "phi") == 0)
        {
            PReadWrite->WriteCell(phi, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Maintenance respiration") == 0)
        {
            PReadWrite->WriteCell(MaintenanceRespiration[0], 8);
        }
        else if (strcmp(ParametersNameArray[i], "Respiration Coefficient") == 0)
        {
            PReadWrite->WriteCell(RespirationCoefficient, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Dissolved organic carbon loss") == 0)
        {
            PReadWrite->WriteCell(DocLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Dissolved organic carbon nutrient stress loss") == 0)
        {
            PReadWrite->WriteCell(DocStressLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Death loss") == 0)
        {
            PReadWrite->WriteCell(DeathLoss, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RedfieldCFactor") == 0)
        {
            PReadWrite->WriteCell(RedfieldCFactor, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RedfieldNFactor") == 0)
        {
            PReadWrite->WriteCell(RedfieldNFactor, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RedfieldPFactor") == 0)
        {
            PReadWrite->WriteCell(RedfieldPFactor, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TemperatureAugmentationRate") == 0)
        {
            PReadWrite->WriteCell(TemperatureAugmentationRate, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Falling speed") == 0)
        {
            PReadWrite->WriteCell(FallingSpeed, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Light/Dark resp") == 0)
        {
            PReadWrite->WriteCell(RatioLightDarkRespiration, 8);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfLoads, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfDaysForLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfDaysForLoads, 0);
        }
        else if (strcmp(ParametersNameArray[i], "NumberOfRiverLoads") == 0)
        {
            PReadWrite->WriteCell(NumberOfRiverLoads, 0);
        }

        // parameters from TTejoPhytoplankton
        else if (strcmp(ParametersNameArray[i], "q10PH") == 0)
        {
            PReadWrite->WriteCell(q10PH, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RTMPH") == 0)
        {
            PReadWrite->WriteCell(RTMPH, 8);
        }

        // parameters from TTejoNPSiPhytoplankton
        else if (strcmp(ParametersNameArray[i], "KsDIN") == 0)
        {
            PReadWrite->WriteCell(KsDIN, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KsPO4") == 0)
        {
            PReadWrite->WriteCell(KsPO4, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TTejoNPSiPhytoplankton::NutrientLimitation(int ABoxNumber)
{

	// Nutrient limitation component - Developed (finally) in November 1996,
	// for N, P and Si limitation
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
	{
		// DIN and PO4 are used, and
		// the limitation is based on umol l-1 values passed from the
		// communicate method

		double MyDIN, MyPO4;

		MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyDIN,
												 ABoxNumber,
												 "DIN",
												 ObjectCode);

		MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyPO4,
												 ABoxNumber,
												 "Phosphate",
												 ObjectCode);

		if ( MyDIN/MyPO4 > RedfieldNFactorInAtoms ) // P limited
		{
			LimitingNutrient = MyPO4;
//			::MessageBox(0,"Phosphate limited","",MB_OK);
			if (LimitingNutrient == 0)
			{
				Productivity = 0;
				return;
			}
			Productivity = Productivity               // Michaelis-Menten formulation
							* LimitingNutrient
							/ (KsPO4 + LimitingNutrient);
			PCounter++;
		}
		else                                   // N limited
		{
			LimitingNutrient = MyDIN;
			if (LimitingNutrient == 0)
			{
				Productivity = 0;
				return;
			}
			Productivity = Productivity               // Michaelis-Menten formulation
							* LimitingNutrient
							/ (KsDIN + LimitingNutrient);
			NCounter++;
		}

		// REDFIELD RATIO TO CALCULATE NUTRIENTS REMOVED - BALANCE
		// production is in mgC

		// If production is already negative then do nothing - send balance

		if (Productivity > 0)
		{
			// return negative value because this is subtracted
			MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * CUBIC / ( RedfieldCFactor
																					* NITROGENATOMICWEIGHT
																					/ RedfieldNFactor ),
												 ABoxNumber,
												 "DIN",
												 ObjectCode);
			// return negative value because this is subtracted
			MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * CUBIC / ( RedfieldCFactor
																					* PHOSPHORUSATOMICWEIGHT
																					/ RedfieldPFactor ),
												 ABoxNumber,
												 "Phosphate",
												 ObjectCode);
		}
	}
}


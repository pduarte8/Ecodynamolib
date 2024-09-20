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
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "ecodyn.rh"
#include "phytobjt.h"
#include "iodll.h"

// Cala do Norte

TCalaPhytoplankton::TCalaPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
					: TTejoPhytoplankton(APEcoDynClass, className)
{
	// Get parameter file name

	VariableCount = 0;

	ThresholdNH4 = 0;	// Concentration seuil : en dessous le phyto
							// utilise  DIN comme nutrient plutot que NH4
							// en umol l-1
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        int X,Y, XV, YV;
        double kNH4;

        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            if (!PReadWrite->FindString("Threshold NH4", XV, YV))
            {
                PReadWrite->ReadNumber(X+3, Y,ThresholdNH4 );
                PReadWrite->ReadNumber(X+3, Y+1, kNH4 );
            }
            else
            {
                PReadWrite->ReadNumber(XV + 1, YV, ThresholdNH4 );
                if (!PReadWrite->FindString("kNH4", X, Y))
                {
                    X = XV;
                    Y = YV + 1;
                }
                PReadWrite->ReadNumber(X + 1, Y, kNH4 );
            }
            for (int j = 0; j < NumberOfBoxes; j++)
                Ks[j] = kNH4;
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object : Cala Phyto (Parameters)",
                                "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Undefined object : Cala Phyto (Parameters)" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }


}


// AP, 2007.07.12
bool TCalaPhytoplankton::SaveParameters()
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

        // parameters from TCalaPhytoplankton
        else if (strcmp(ParametersNameArray[i], "Threshold NH4") == 0)
        {
            PReadWrite->WriteCell(ThresholdNH4, 8);
        }
        else if (strcmp(ParametersNameArray[i], "kNH4") == 0)
        {
            PReadWrite->WriteCell(Ks[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TCalaPhytoplankton::NutrientLimitation(int ABoxNumber)
{
	bool UsingNH4 = true;

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	if (MyNutrientPointer != NULL)
	{
		// Nutrients are passed in umol l-1

		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient,
												 ABoxNumber,
												 "Ammonia",
												 ObjectCode);


		if (LimitingNutrient < ThresholdNH4)
		{
			MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient,
													 ABoxNumber,
													 "DIN",
													 ObjectCode);

			UsingNH4 = false;
		}


		if (LimitingNutrient == 0)
		{
			Productivity = 0;
			return;
		}

		Productivity = Productivity           // Michaelis-Menten formulation
							* LimitingNutrient
							/ (Ks[ABoxNumber] + LimitingNutrient);



		if (Productivity > 0)
		{
			// return negative value because this is subtracted
			if (UsingNH4)
			MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * CUBIC / (45*14/RedfieldNFactor),
												ABoxNumber,
												"Ammonia",
												ObjectCode);
			else
			MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * CUBIC / (45*14/RedfieldNFactor),
												ABoxNumber,
												"DIN",
												ObjectCode);

		}
	}
}

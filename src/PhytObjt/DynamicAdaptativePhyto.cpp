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

// DYNAMIC ADAPTATIVE

TDynamicAdaptativePhytoplankton::TDynamicAdaptativePhytoplankton(
    TEcoDynClass* APEcoDynClass, char* className)
			  : TAdaptativePhytoplankton(APEcoDynClass, className)
{

	ParameterCount = 0;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
       double MyValue;

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

                if (strcmp(MyParameter, "InhibitionTime") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        InhibitionTime[j] = MyValue;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TDynamicAdaptativePhytoplankton::TDynamicAdaptativePhytoplankton - Undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}



TDynamicAdaptativePhytoplankton::~TDynamicAdaptativePhytoplankton()
{
    freeMemory();
}

void TDynamicAdaptativePhytoplankton::freeMemory()
{
}


double TDynamicAdaptativePhytoplankton::GetParameterValue(char* MyParameter)
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

    // parameters from TDynamicAdaptativePhytoplankton
    else if (strcmp(MyParameter, "InhibitionTime") == 0)
    {
        value = InhibitionTime[0];
    }
    else
        value = 0.0;
    return value;
}

bool TDynamicAdaptativePhytoplankton::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TDynamicAdaptativePhytoplankton
    else if (strcmp(MyParameter, "InhibitionTime") == 0)
    {
        for (j = 0; j < NumberOfBoxes; j++)
            InhibitionTime[j] = value;
    }
    else
        rc = false;

    return rc;
}

// AP, 2007.07.12
bool TDynamicAdaptativePhytoplankton::SaveParameters()
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

        // parameters from TDynamicAdaptativePhytoplankton
        else if (strcmp(ParametersNameArray[i], "InhibitionTime") == 0)
        {
            PReadWrite->WriteCell(InhibitionTime[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TDynamicAdaptativePhytoplankton :: Production()
{
  //const double KValue = 0.25;
  double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			AverageLight, D, B1, B2, JulianDay;

  JulianDay = MyPEcoDynClass->GetJulianDay();
	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();
  for (int i = 0; i < NumberOfBoxes; i++)
  {
	  BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
	  MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);

		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

	 if (MyLightPointer != NULL)
	 {

		 	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
											  i,
											  "PAR surface irradiance",
											  ObjectCode);

		 	BoxDepth = MyPEcoDynClass->GetBoxDepth(i);

		 	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
											  BoxDepth,
											  i,
											  "Sub-surface PAR irradiance",
											  ObjectCode);

		 LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
		 LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
		 if (LightAtTop == 0)
			Productivity = 0.0;
		 else
		 {
			if (AEiler[i] < 0.000001) //Importante!!! - Sem esta ressalva acontecia
			AEiler[i] = 0;            //nalguns casos que sendo o A muito pequeno
											  //vinha o D infinito dando a produtividade
											  //valores disparatados.

			if (AEiler[i] != 0.0)
			{
				D = BEiler[i] * BEiler[i] - 4 * AEiler[i] * CEiler[i];
				B1 = 2*AEiler[i] * LightAtTop + BEiler[i];
				B2 = 2*AEiler[i] * LightAtBottom + BEiler[i];

				if (D < 0.0)
				{
					Productivity = PhytoBiomass[i] *
													2.0 / (KValue*sqrt(-D)) *
													(atan(B1/sqrt(-D))- atan(B2/sqrt(-D)))
													/ (ABottom - ATop)
													* DAYSTOHOURS;


				}
				if (D == 0.0)
				{
				Productivity = PhytoBiomass[i] *
													2.0 / KValue *
													(1.0 / B2 - 1.0 / B1)
													/ (ABottom - ATop)
													* DAYSTOHOURS;
				}
				if (D > 0.0)
				{
					Productivity = PhytoBiomass[i] *
													1.0 / (KValue*sqrt(D)) *
													log (((B1 - sqrt(D))*(B2 + sqrt(D))) /
														  ((B1 + sqrt(D)) * (B2 - sqrt(D))))
													/ (ABottom - ATop)
													* DAYSTOHOURS;
				}
			}
			if (AEiler[i] == 0)
			{
				Productivity = PhytoBiomass[i] * 1.0 / (KValue * BEiler[i]) *
														log (fabs(BEiler[i] * LightAtTop + CEiler[i])/
														fabs(BEiler[i] * LightAtBottom + CEiler[i]))
														/ (ABottom - ATop)
														* DAYSTOHOURS;
			}
		 }
		 //Critical Light Exposure
		 MyLightPointer->Inquiry(GetEcoDynClassName(), AverageLight,
											  i,
											  "Mean horizontal water PAR irradiance",
											  ObjectCode);
		 if (AverageLight * WATTSTOMICROEINSTEINS > CriticalLight[i])
			CriticalLightExposure[i] = CriticalLightExposure[i]+
											  TimeStep*DAYSTOHOURS;
		 else
			CriticalLightExposure[i] = CriticalLightExposure[i]-
											  TimeStep*DAYSTOHOURS;
		 if (CriticalLightExposure[i] < 0.0)
			CriticalLightExposure[i] = 0.0;
		 /*if (CriticalLightExposure[i] != 0)
			Debugger (CriticalLightExposure[i]);*/
		 /*Critical light exposure is calculated in hours. It increases when
		 the algae are exposed to a light intensity above the optimal level
		 and it decreases in the same way after the algae are exposed to a
		 sub-optimal light level. This is to ensure that the recovery from
		 photoinhibition takes the same time as its development*/

		 AEiler[i] = DefaultAEiler[i] *
						 (1-exp(-CriticalLightExposure[i] /
						 InhibitionTime[i]));

		 if (AEiler[i] != 0.0)
		 {
			Pmax[i] = 1 /
						(BEiler[i] + 2 * sqrt(AEiler[i] * CEiler[i]));
			Iopt[i] = sqrt(CEiler[i] / AEiler[i]);
		 }
		 else
			Pmax[i] = 1 /
						BEiler[i];
		 //Slope[i] = 1 / CEiler[i];

       TemperatureLimitation(i);

		 NutrientLimitation(i);
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	 }
  }
  LightShadeAdaptation();
}


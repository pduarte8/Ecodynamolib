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

// TTejoPhytoplankton Class
//
// Constructors invoked outside EcoDyn shell...
//

TTejoPhytoplankton::TTejoPhytoplankton(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aChlToCarbon[], double riverPhyto,
        double oceanPhyto, double aPhytoBiomass[], double aPhytoLoad[],
        double aChlCarbonRatio[], double aChlToBiomass[],
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration,
        double Q10PH, double rTMPH)
    :TPhytoplanktonBasic(className, timeStep, nLines, nColumns,
            aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, aChlToCarbon, riverPhyto,
            oceanPhyto, aPhytoBiomass, aPhytoLoad, aChlCarbonRatio, aChlToBiomass,
            nParams, pmax, iopt, slope, aEiler, bEiler, cEiler, kValue, ks,
            kAmmonia, kNitrate, pHi, maintenanceRespiration, respirationCoefficient,
            docLoss, docStressLoss, deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate, fallingSpeed,
            ratioLightDarkRespiration)
{
	KValue = 2;
    q10PH = Q10PH;
    RTMPH = rTMPH;
}

TTejoPhytoplankton::TTejoPhytoplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TPhytoplanktonBasic(className, timeStep, morphologyFilename,
                variablesFilename, parametersFilename)
{
    BuildTejoPhyto();
}
// ...


TTejoPhytoplankton::TTejoPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
							: TPhytoplanktonBasic(APEcoDynClass, className)
{
    BuildTejoPhyto();
}

void TTejoPhytoplankton::BuildTejoPhyto()
{
	KValue = 2;
	// Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        int X,Y;
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            // NumberOfParameters already filled in TPhytoplankton
    //		ParametersNameArray = new VNA[NumberOfParameters];

            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "q10PH") == 0)
                {
                    double Myq10PH = 0;
                    PReadWrite->ReadNumber(X+3, i, Myq10PH);
                    q10PH = Myq10PH;
                }
                else
                if (strcmp(MyParameter, "RTMPH") == 0)
                {
                    double MyRTMPH = 0;
                    PReadWrite->ReadNumber(X+3, i, MyRTMPH);
                    RTMPH = MyRTMPH;
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object - Tejo phytoplankton", "EcoDynamo alert", MB_OK);
#else  //__BORLANDC__
						cerr << "TTejoPhytoplankton::BuildTejoPhyto - Parameters: Undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

double TTejoPhytoplankton::GetParameterValue(char* MyParameter)
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
    else
        value = 0.0;
    return value;
}

bool TTejoPhytoplankton::SetParameterValue(char* MyParameter, double value)
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
    else
        rc = false;

    return rc;
}

// AP, 2007.07.12
bool TTejoPhytoplankton::SaveParameters()
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
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TTejoPhytoplankton::Senescence(int ABoxNumber)
{
	const double DOCMaximumStressLoss = 0.9, // 90% exudate in worst case
					 KDOC = 0.03;                // Gives 0.15 at about 60 umol l-1 DIN

	// Senescence basically works by imposing a stress DOC loss -
	// In this object it is directly linked to nutrient limitation

	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
	{
		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient,
												 ABoxNumber,
												 "DIN",
												 ObjectCode);

		DocStressLoss = DOCMaximumStressLoss;
		// At the moment only DIN is used, and
		// the limitation is based on umol l-1 values passed from the
		// respond method

		DocStressLoss = DocStressLoss
							 * exp(-KDOC * LimitingNutrient);
	}
}


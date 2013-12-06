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

// TPhytoplankton Classes


TEilerVerticalModelPhytoplankton::TEilerVerticalModelPhytoplankton(
    TEcoDynClass* APEcoDynClass, char* className)
				 : TPhytoplanktonBasic(APEcoDynClass, className)
{
	// Receive pointer to integrate

   
	for (int i = 0; i < NumberOfBoxes; i++)
   {
	  DefaultAEiler[i] = 0.004598;
	  AEiler[i] = DefaultAEiler[i];
	  BEiler[i] = 3.160603;
	  CEiler[i] = 735.7589;
   }


   for (int i = 0; i < NumberOfBoxes; i++)
   {
   	AEiler[i] = 1 / (Slope[i] * Iopt[i] * Iopt[i]);
      BEiler[i] = 1 / Pmax[i] - 2 / (Slope[i] * Iopt[i]);
      CEiler[i] = 1 / Slope[i];
   }
}

TEilerVerticalModelPhytoplankton::~TEilerVerticalModelPhytoplankton()
{
    try {
        if (NumberOfVariables > 0)
        {
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
				cerr  << "TEilerVerticalModelPhytoplankton::~TEilerVerticalModelPhytoplankton - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

void TEilerVerticalModelPhytoplankton::Go ()
{
	// Active methods
	Production();
	Exudation();
   Respiration();
	Mortality();  
   Falling();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
	}   
}



void TEilerVerticalModelPhytoplankton::Integrate()
{
	// Integrate phytoplankton biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
	// Transport the phytoplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
//		MessageBox(0,"About to transport", "", MB_OK);
	{
		//Debugger (BEiler[1]);
		for (int i = 0; i < NumberOfBoxes; i++)
		{
			GenericProduct1[i] = PhytoBiomass[i] * DefaultAEiler[i];
			GenericProduct2[i] = PhytoBiomass[i] * AEiler[i];
			GenericProduct3[i] = PhytoBiomass[i] * BEiler[i];
			GenericProduct4[i] = PhytoBiomass[i] * CEiler[i];
			GenericProduct5[i] = PhytoBiomass[i] * CriticalLightExposure[i];
			GenericProduct6[i] = PhytoBiomass[i] * OptimalChlCarbonRatio[i];
			GenericProduct7[i] = PhytoBiomass[i] * ChlCarbonRatio[i];
		}

		MyTransportPointer->Go(GenericProduct1, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct2, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct3, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct4, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct5, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct6, 0, 0, 0);
		MyTransportPointer->Go(GenericProduct7, 0, 0, 0);
		MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);

		for (int i = 0; i < NumberOfBoxes; i++)
		{
			if (PhytoBiomass[i] > 0)
			{
				DefaultAEiler[i] = GenericProduct1[i] / PhytoBiomass[i];
				AEiler[i] = GenericProduct2[i] / PhytoBiomass[i];
				BEiler[i] = GenericProduct3[i] / PhytoBiomass[i];
				CEiler[i] = GenericProduct4[i] / PhytoBiomass[i];
				CriticalLightExposure[i] = GenericProduct5[i] / PhytoBiomass[i];
				OptimalChlCarbonRatio[i] = GenericProduct6[i] / PhytoBiomass[i];
				ChlCarbonRatio[i] = GenericProduct7[i] / PhytoBiomass[i];

				//Debugger (JulianDay);


			}
		}
		//Debugger (BEiler[1]);
	}
}

void TEilerVerticalModelPhytoplankton :: Production()
{
	//const double KValue = 0.25;
	double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			 AverageLight, D, B1, B2;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
		MyPEcoDynClass->GetBoxTopAndBottom(i,
													ATop,
													ABottom);
		//Debugger (ATop); Debugger (ABottom);



		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);
		if (MyLightPointer != NULL)
		{
         if (i == 0)
         	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												 	i,
												 	"PAR surface irradiance",
												 	ObjectCode);
         else
				MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
													 i - 1,
													 "Sub-surface PAR irradiance",
													 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);
			/*if (i == 46)
         {
          	Debugger(1000);
            Debugger(LightAtTop);
            Debugger(LightAtBottom);

         } */
			LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
			LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
			if (LightAtTop == 0)
				Productivity = 0.0;
			else
			{

				if (AEiler[i] != 0.0)
				{

					D = BEiler[i] * BEiler[i] - 4 * AEiler[i] * CEiler[i];
					B1 = 2*AEiler[i] * LightAtTop + BEiler[i];
					B2 = 2*AEiler[i] * LightAtBottom + BEiler[i];
               /*if (i == 46)
            	{
            		Debugger(LightAtTop);
               	Debugger(LightAtBottom);
               	Debugger(D);
                  Debugger(B1);
                  Debugger(B2);
                  Debugger(PhytoBiomass[i]);
               } */
					if (D < 0.0)
					{
						Productivity = PhytoBiomass[i] *
											2.0 / (KValue*sqrt(-D)) *
											(atan(B1/sqrt(-D))- atan(B2/sqrt(-D)))
											/ (ABottom - ATop)
											* DAYSTOHOURS;
					}
					else if (D == 0.0)
					{
						Productivity = PhytoBiomass[i] *
											2.0 / KValue *
											(1.0 / B2 - 1.0 / B1)
											/ (ABottom - ATop)
											* DAYSTOHOURS;

					}
					else if (D > 0.0)
					{
						Productivity = PhytoBiomass[i] *
											1.0 / (KValue*sqrt(D)) *
											log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
											((B1 + sqrt(D)) * (B2 - sqrt(D))))
											/ (ABottom - ATop)
											* DAYSTOHOURS;

					}
				}
				if (AEiler[i] == 0)
					Productivity = PhytoBiomass[i] * 1.0 / (KValue * BEiler[i]) *
										log (fabs(BEiler[i] * LightAtTop + CEiler[i])/
										fabs(BEiler[i] * LightAtBottom + CEiler[i]))
										/ (ABottom - ATop)
										* DAYSTOHOURS;
			}
         TemperatureLimitation(i);
			NutrientLimitationByAmmoniaAndNitrate(i);
			GPP[i] = Productivity;
			PhytoProd[i] = Productivity;
		}
	}
}


void TEilerVerticalModelPhytoplankton::Exudation() //Copiei o m?todo exudation do CarlPhytoplankton porque
{                                                  //o do TPhytoplankton baseia-se erradamente na produ??o liquida
                                                   //Este m?todo tem a particularidade de transferir
                                                   //o azoto exudado para o DON
	double Exudate;

   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		DocStressLoss = DocLoss;
		if (GPP[i] > 0)
		Exudate = GPP[i] * DocStressLoss;
		else Exudate = 0;
		PhytoProd[i] = PhytoProd[i] - Exudate;

      if (MyNutrientPointer != NULL)
      {
         MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC / 12 /*Carbon Atomic Weight*/,
												i,
												"DOC",
												ObjectCode);      //Return as umol C / m3

			MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (RedfieldCFactor * NITROGENATOMICWEIGHT/RedfieldNFactor),
												i,
												"DON",
												ObjectCode);      //Return as umol N / m3
      }

	}
}

void TEilerVerticalModelPhytoplankton :: Falling()
{
	double BoxDepth;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
   	BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
      PhytoFlux[i] = PhytoFlux[i] - FallingSpeed / BoxDepth * PhytoBiomass[i]; //Ao PhytoFlux de cada caixa ? subtra?da a frac??o que cai
      if (i > 0) 
      	PhytoFlux[i] = PhytoFlux[i] + FallingSpeed / BoxDepth * PhytoBiomass[i - 1]; //Ao PhytoFlux de cada caixa ? somada a frac??o que cai da caixa superior

   }
}

void TEilerVerticalModelPhytoplankton::Mortality()
{
	double BiomassLost = 0;

	TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		BiomassLost = PhytoBiomass[i] * DeathLoss;

		PhytoFlux[i] = PhytoFlux[i] - BiomassLost;

		PhytoMortality[i] = BiomassLost; // flux per day for mass balance

		if (MySPMPointer != NULL)
      {
			/*MySPMPointer->Update(GetEcoDynClassName(), BiomassLost / ChlToCarbon[i] * ChlToBiomass[i] / CUBIC,
                                i,
                                "Suspended matter",
                                ObjectCode);   */
          MySPMPointer->Update(GetEcoDynClassName(), BiomassLost / CUBIC,
                                i,
                                "Particulate organic carbon",
                                ObjectCode);                      //return as mg C / l
          MySPMPointer->Update(GetEcoDynClassName(), BiomassLost / CUBIC *
                                 RedfieldNFactor / RedfieldCFactor,
                                i,
                                "Particulate organic nitrogen",
                                ObjectCode);                    //return as mg N / l
      }
	}
}
// DYNAMIC

TDynamicVerticalModelPhytoplankton::TDynamicVerticalModelPhytoplankton(
        TEcoDynClass* APEcoDynClass, char* className)
				 : TEilerVerticalModelPhytoplankton(APEcoDynClass, className)
{
	// Receive pointer to integrate

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
                //Debugger(MyValue);
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
            cerr << "TDynamicVerticalModelPhytoplankton::TDynamicVerticalModelPhytoplankton - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TDynamicVerticalModelPhytoplankton::~TDynamicVerticalModelPhytoplankton()
{
    try {
        if (NumberOfVariables > 0)
        {
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
				cerr  << "TDynamicVerticalModelPhytoplankton::~TDynamicVerticalModelPhytoplankton - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TDynamicVerticalModelPhytoplankton::GetParameterValue(char* MyParameter)
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

    // parameters from TDynamicVerticalModelPhytoplankton
    else if (strcmp(MyParameter, "InhibitionTime") == 0)
    {
        value = InhibitionTime[0];
    }
    else
        value = 0.0;
    return value;
}

bool TDynamicVerticalModelPhytoplankton::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TDynamicVerticalModelPhytoplankton
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
bool TDynamicVerticalModelPhytoplankton::SaveParameters()
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

        // parameters from TDynamicVerticalModelPhytoplankton
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



void TDynamicVerticalModelPhytoplankton :: Production()
{
	//const double KValue = 0.25;
	double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			 AverageLight, D, B1, B2;


	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

  for (int i = 0; i < NumberOfBoxes; i++)
  {
	  //Debugger (i);
	  BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
	  MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);
	  //Debugger (ATop); Debugger (ABottom);

		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

	 if (MyLightPointer != NULL)
	 {

		 if (i == 0)
         	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												 	i,
												 	"PAR surface irradiance",
												 	ObjectCode);
         else
				MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
													 i - 1,
													 "Sub-surface PAR irradiance",
													 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);

		 LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
		 LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
		 if (LightAtTop == 0.0)
			Productivity = 0.0;
		 else
		 {
			if (AEiler[i] < 0.000001)  //Importante!!! - Sem esta ressalva acontecia
			AEiler[i] = 0;          //nalguns casos que sendo o A muito pequeno
			if (AEiler[i] != 0.0)      //vinha o D infinito dando a produtividade
			{                          //valores disparatados.
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
		 MyLightPointer->Inquiry(GetEcoDynClassName(), AverageLight,
											  i,
											  "Mean vertical water PAR irradiance",
											  ObjectCode);
		 //Critical Light Exposure
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
		 Slope[i] = 1 / CEiler[i];

       TemperatureLimitation(i);
		 NutrientLimitation(i);
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
		 JulianDay = MyPEcoDynClass->GetJulianDay();
		 CurrentTime = MyPEcoDynClass->GetCurrTime();
	 }
  }
}

// ADAPTATIVE

TAdaptativeVerticalModelPhytoplankton::TAdaptativeVerticalModelPhytoplankton(
        TEcoDynClass* APEcoDynClass, char* className)
				: TEilerVerticalModelPhytoplankton(APEcoDynClass, className)
{

}

TAdaptativeVerticalModelPhytoplankton::~TAdaptativeVerticalModelPhytoplankton()
{
    try {
        if (NumberOfVariables > 0)
        {
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
				cerr  << "TAdaptativeVerticalModelPhytoplankton::~TAdaptativeVerticalModelPhytoplankton - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}



void TAdaptativeVerticalModelPhytoplankton :: Production()
{
  //const double KValue = 0.25;
  double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			D, B1, B2;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

  for (int i = 0; i < NumberOfBoxes; i++)
  {
	  AEiler[i] = DefaultAEiler[i];
	  BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
	  MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);
	  //Debugger (ATop); Debugger (ABottom);


	  if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

	 if (MyLightPointer != NULL)
	 {

		 if (i == 0)
         	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												 	i,
												 	"PAR surface irradiance",
												 	ObjectCode);
         else
				MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
													 i - 1,
													 "Sub-surface PAR irradiance",
													 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);


		 LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
		 LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
		 if (LightAtTop == 0.0)
			Productivity = 0.0;
		 else
		 {
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
				else if (D == 0.0)
				{
					Productivity = PhytoBiomass[i] *
													2.0 / KValue *
													(1.0 / B2 - 1.0 / B1)
													/ (ABottom - ATop)
													* DAYSTOHOURS;

				}
				else if (D > 0.0)
				{
					Productivity = PhytoBiomass[i] *
													1.0 / (KValue*sqrt(D)) *
													log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
														  ((B1 + sqrt(D)) * (B2 - sqrt(D))))
													/ (ABottom - ATop)
													* DAYSTOHOURS;
				}
			}
			if (AEiler[i] == 0)
				Productivity = PhytoBiomass[i] * 1.0 / (KValue * BEiler[i]) *
														log (fabs(BEiler[i] * LightAtTop + CEiler[i])/
														fabs(BEiler[i] * LightAtBottom + CEiler[i]))
														/ (ABottom - ATop)
														* DAYSTOHOURS;

			if (AEiler[i] != 0.0)
			{
				Pmax[i] = 1 /
						(BEiler[i] + 2 * sqrt(AEiler[i] * CEiler[i]));
				Iopt[i] = sqrt(CEiler[i] / AEiler[i]);
			}
			else
				Pmax[i] = 1 /
						BEiler[i];
		 }

       TemperatureLimitation(i);
		 NutrientLimitation(i);
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	  }
	}
	LightShadeAdaptation();
}

void TAdaptativeVerticalModelPhytoplankton :: LightShadeAdaptation()
{
	double MyNoonLight, MyAverageLight, ATop, ABottom, MyAverageDepth, Lambda;

	for (int i = 0; i < NumberOfBoxes; i++)
	{
		MyPEcoDynClass->GetBoxTopAndBottom(i,
												  ATop,
												  ABottom);
		MyAverageDepth = (ATop + ABottom) /
							  2;
		TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
		if (MyLightPointer != NULL)
		{
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyNoonLight,
											  i,
											  "Noon surface PAR",
											  ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), MyAverageLight,
											  i,
											  "Mean vertical water PAR irradiance",
											  ObjectCode);

			MyNoonLight = MyNoonLight *
							  WATTSTOMICROEINSTEINS;
			MyAverageLight = MyAverageLight * WATTSTOMICROEINSTEINS;
			//Debugger (MyNoonLight);
			if (MyAverageLight / MyNoonLight < 0.01)
				OptimalChlCarbonRatio[i] = 0.04;
			else
				OptimalChlCarbonRatio[i] = 0.01 +
										 0.03 *
										 log (MyAverageLight / MyNoonLight) /
										 log (0.01);
			if (MyAverageLight > 0.0)
				Lambda = 0.2;//h-1
			else
				Lambda = 0.0;//h-1
			ChlCarbonRatio[i] = ChlCarbonRatio[i] +
								Lambda *
								(OptimalChlCarbonRatio[i] - ChlCarbonRatio[i]) *
								DAYSTOHOURS *
								TimeStep;
			ChlToCarbon[i] = 1 / ChlCarbonRatio[i];
			Slope[i] = Pmax[i] /
						(MyNoonLight * (0.25 - 5.0 * ChlCarbonRatio[i]));
			CEiler[i] = 1 /
						 Slope[i];
			if (DefaultAEiler[i] != 0)
				DefaultAEiler[i] = 1.0 /
						 (Slope[i] * sqr(CriticalLight[i]));

		}
	}
}

// DYNAMIC ADAPTATIVE

TDynamicAdaptativeVerticalModelPhytoplankton::TDynamicAdaptativeVerticalModelPhytoplankton(
        TEcoDynClass* APEcoDynClass, char* className)
		  : TAdaptativeVerticalModelPhytoplankton(APEcoDynClass, className)
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
            cerr << "TDynamicAdaptativeVerticalModelPhytoplankton::TDynamicAdaptativeVerticalModelPhytoplankton - parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}



TDynamicAdaptativeVerticalModelPhytoplankton::~TDynamicAdaptativeVerticalModelPhytoplankton()
{
    try {
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
				cerr  << "TDynamicAdaptativeVerticalModelPhytoplankton::~TDynamicAdaptativeVerticalModelPhytoplankton - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TDynamicAdaptativeVerticalModelPhytoplankton::GetParameterValue(char* MyParameter)
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

    // parameters from TDynamicAdaptativeVerticalModelPhytoplankton
    else if (strcmp(MyParameter, "InhibitionTime") == 0)
    {
        value = InhibitionTime[0];
    }
    else
        value = 0.0;
    return value;
}

bool TDynamicAdaptativeVerticalModelPhytoplankton::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TDynamicAdaptativeVerticalModelPhytoplankton
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
bool TDynamicAdaptativeVerticalModelPhytoplankton::SaveParameters()
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

        // parameters from TDynamicAdaptativeVerticalModelPhytoplankton
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



void TDynamicAdaptativeVerticalModelPhytoplankton :: Production()
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

		 if (i == 0)
         	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												 	i,
												 	"PAR surface irradiance",
												 	ObjectCode);
         else
				MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
													 i - 1,
													 "Sub-surface PAR irradiance",
													 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
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
											  "Mean vertical water PAR irradiance",
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




//Adaptative phytoplankton slope


TAdaptativeSlopeVerticalModelPhytoplankton::TAdaptativeSlopeVerticalModelPhytoplankton(
        TEcoDynClass* APEcoDynClass, char* className)
		 : TAdaptativeVerticalModelPhytoplankton(APEcoDynClass, className)
{

}



TAdaptativeSlopeVerticalModelPhytoplankton::~TAdaptativeSlopeVerticalModelPhytoplankton()
{
    try {
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
				cerr  << "TAdaptativeSlopeVerticalModelPhytoplankton::~TAdaptativeSlopeVerticalModelPhytoplankton - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}  




void TAdaptativeSlopeVerticalModelPhytoplankton :: Production()
{
  double BoxDepth, ATop, ABottom, LightAtTop, LightAtBottom,
			D, B1, B2;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				  *MySPMPointer = MyPEcoDynClass->GetSPMPointer();

  for (int i = 0; i < NumberOfBoxes; i++)
  {
	  AEiler[i] = DefaultAEiler[i];
	  if (AEiler[i] > 0.0)
		BEiler[i] = 1 / Pmax[i] -
						2 / (Slope[i] * Iopt[i]);
	  else
		BEiler[i] = 1 / Pmax[i];

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

		 if (i == 0)
         	MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												 	i,
												 	"PAR surface irradiance",
												 	ObjectCode);
         else
				MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
													 i - 1,
													 "Sub-surface PAR irradiance",
													 ObjectCode);
			MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												 i,
												 "Sub-surface PAR irradiance",
												 ObjectCode);


		 LightAtTop = LightAtTop * WATTSTOMICROEINSTEINS;
		 LightAtBottom = LightAtBottom * WATTSTOMICROEINSTEINS;
		 if (LightAtTop == 0.0)
			Productivity = 0.0;
		 else
		 {
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
				else if (D == 0.0)
				{
					Productivity = PhytoBiomass[i] *
													2.0 / KValue *
													(1.0 / B2 - 1.0 / B1)
													/ (ABottom - ATop)
													* DAYSTOHOURS;

				}
				else if (D > 0.0)
				{
					Productivity = PhytoBiomass[i] *
													1.0 / (KValue*sqrt(D)) *
													log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
														  ((B1 + sqrt(D)) * (B2 - sqrt(D))))
													/ (ABottom - ATop)
													* DAYSTOHOURS;
				}
			}
			if (AEiler[i] == 0)
				Productivity = PhytoBiomass[i] * 1.0 / (KValue * BEiler[i]) *
														log (fabs(BEiler[i] * LightAtTop + CEiler[i])/
														fabs(BEiler[i] * LightAtBottom + CEiler[i]))
														/ (ABottom - ATop)
														* DAYSTOHOURS;
		 }

       TemperatureLimitation(i);
       
		 NutrientLimitation(i);
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	  }
	}
	LightShadeAdaptation();
}



TPhytoplanktonBiomassConstant::TPhytoplanktonBiomassConstant(
    TEcoDynClass* APEcoDynClass, char* className)
	 : TEilerVerticalModelPhytoplankton(APEcoDynClass, className)
{
}

void TPhytoplanktonBiomassConstant::Integrate()
{
}

void TPhytoplanktonBiomassConstant::Falling()
{
}

void TPhytoplanktonBiomassConstant::Mortality()
{
}




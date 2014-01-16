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


TSto0Phytoplankton::TSto0Phytoplankton(TEcoDynClass* APEcoDynClass, char* className)
                     : TPhytoplanktonBasic(APEcoDynClass, className)
{
	OxygenNetProduction  = new double[NumberOfBoxes];
   DailyAverageBiomass = new double[365];
   for (int i = 0; i< NumberOfBoxes; i++)
   	OxygenNetProduction[i] = 0;
   for (int i = 0; i < 365; i++)
      DailyAverageBiomass[i] = 0;


    int X, Y, XV, YV;
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
    //		ParametersNameArray = new VNA[NumberOfParameters];

            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
    //            strcpy(ParametersNameArray[i-Y], MyParameter);

             if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenProd);
                else
                if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
                    PReadWrite->ReadNumber(X+3, i, CarbonToOxygenResp);
             else
             if (strcmp(MyParameter, "Imax") == 0)
             {
                for (int j = 0; j < NumberOfBoxes; j++)
                {
                        PReadWrite->ReadNumber(X+3, i, Imax[j]);
                    //Debugger(Imax[j]);
                }
            }
          }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object - Sto0Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSto0Phytoplankton::TSto0Phytoplankton - Parameters - Undefined object Phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
	if (PReadWrite != NULL)
	{
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
            // Create array for state variable names
            //VariableNameArray = new VNA[NumberOfVariables];
            // read in the variable names
            //for (int i = Y; i < Y + NumberOfVariables; i++)
            //	PReadWrite->ReadString(X+2, i, VariableNameArray[i-Y]);

          char MyVariable[65];
            for (int i = Y; i < Y + NumberOfVariables; i++)
            {
            PReadWrite->ReadString(X + 2, i, MyVariable);
            if (strcmp(MyVariable, "Daily average biomass") == 0)
            {
                if (!PReadWrite->FindString("Daily average biomass values", XV, YV))
                {
                    XV = X + 3;
                    YV = i - 1;
                }
                for (int j = 0; j < 365; j++)
                {
                    PReadWrite->ReadNumber(XV, YV+1+j, DailyAverageBiomass[j]);
                   //Debugger(DailyAverageBiomass[j]);
                }
            }
          }
        }
        else
        {
#ifdef __BORLANDC__
					MessageBox(0, "Undefined object - Sto0Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TSto0Phytoplankton::TSto0Phytoplankton - Variables - Undefined object Phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
}

TSto0Phytoplankton::~TSto0Phytoplankton()
{
    freeMemory();
}

void TSto0Phytoplankton::freeMemory()
{
    try {
       delete [] OxygenNetProduction;
    //   delete [] VariableNameArray;
       delete [] DailyAverageBiomass;
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
				cerr << "TSto0Phytoplankton::freeMemory - " << exc.what() << endl;
    }
#endif  // __BORLANDC__
}

double TSto0Phytoplankton::GetParameterValue(char* MyParameter)
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

    // parameters from TSto0Phytoplankton
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
        value = CarbonToOxygenProd;
    else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
        value = CarbonToOxygenResp;
    else if (strcmp(MyParameter, "Imax") == 0)
        value = Imax[0];
    else
        value = 0.0;
    return value;
}

bool TSto0Phytoplankton::SetParameterValue(char* MyParameter, double value)
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

    // parameters from TSto0Phytoplankton
    else if (strcmp(MyParameter, "Carbon to Oxygen in photosynthesis") == 0)
        CarbonToOxygenProd = value;
    else if (strcmp(MyParameter, "Carbon to Oxygen in respiration") == 0)
        CarbonToOxygenResp = value;
    else if (strcmp(MyParameter, "Imax") == 0) {
        for (j = 0; j < NumberOfBoxes; j++)
            Imax[j] = value;
    }
    else
        rc = false;

    return rc;
}


// AP, 2007.07.12
bool TSto0Phytoplankton::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Phytoplankton");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Phytoplankton");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);
    /*
     * save variables' names and values
     */
    for (int i = 0; i < NumberOfVariables; i++)
    {
        if (i > 0)
            PReadWrite->WriteSeparator();

        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
        if (strcmp(VariableNameArray[i], "Phytoplankton biomass") == 0)
        {
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(RiverPhyto / ChlCarbonRatio[0], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(OceanPhyto / ChlCarbonRatio[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Phytoplankton load values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Chlorophyll : Carbon values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Chlorophyll to Carbon values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("Chlorophyll to biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PhytoLoad[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ChlCarbonRatio[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ChlToBiomass[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    // from TSto0Phytoplankton
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Daily average biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(DailyAverageBiomass[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TSto0Phytoplankton::SaveParameters()
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

        // parameters from TSto0Phytoplankton
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in photosynthesis") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenProd, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Carbon to Oxygen in respiration") == 0)
        {
            PReadWrite->WriteCell(CarbonToOxygenResp, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Imax") == 0)
        {
            PReadWrite->WriteCell(Imax[0], 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TSto0Phytoplankton::Go()
{
	// Active methods
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
   if (MyLightPointer != NULL)
   	MyLightPointer->Inquiry(GetEcoDynClassName(), LagoonDepth,
                                  0,
                                  "Lagoon depth",
                                  ObjectCode);
   else
   {
#ifdef __BORLANDC__
   	MessageBox(0, "Light object is undefined", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSto0Phytoplankton::Go - Light object is undefined" << endl;
#endif  // __BORLANDC__
      return;
   }
   for (int i = 0; i < NumberOfBoxes; i++)
   	PhytoBiomass[i] = DailyAverageBiomass[MyPEcoDynClass->GetJulianDay() - 1] *
                        ChlToCarbon[i];
  	Production();
	Respiration();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
   	{
   		MyNutrientPointer->Update(GetEcoDynClassName(), OxygenNetProduction[i] / CUBIC, //convert ug to mg
                                    i,
                                    "Oxygen",
                                    ObjectCode);
      	OxygenNetProduction[i] = 0;
   	}
   }
	Exudation();
	//Mortality();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
	}
}

void TSto0Phytoplankton::Integrate()
{
}


void TSto0Phytoplankton::Production()
{
double MyDepth, MyTidalHeight, LightAtTop, LightAtBottom, LightAtCanopyTop, LightAtCanopyBottom,
       KCanopy = 0, CanopyHeight = 0,
       ProductivityAboveTheCanopy, ProductivityBelowTheCanopy;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
             * MyMacrophytePointer = MyPEcoDynClass->GetMacrophytePointer();

	for (int i = 0; i < NumberOfBoxes; i++)
	{
      MyDepth = LagoonDepth;
	   if (MyLightPointer != NULL)
	   {

		   MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
                                     i,
                                     "PAR surface irradiance",
                                     ObjectCode);
         if (LightAtTop == 0)
				Productivity = 0.0;
         else
         {
         	MyLightPointer->Inquiry(GetEcoDynClassName(), KValue,
											       i,
											       "KValue",
											       ObjectCode);
            if (MyMacrophytePointer != NULL)
            {
            	MyMacrophytePointer->Inquiry(GetEcoDynClassName(), KCanopy,
											               i,
											               "Ruppia cirrhosa Canopy extinction coef",
											               ObjectCode);
               MyMacrophytePointer->Inquiry(GetEcoDynClassName(), CanopyHeight,
											               i,
											               "R. cirrhosa Canopy height",
											               ObjectCode);
               if (CanopyHeight > MyDepth)
               	CanopyHeight = MyDepth;
            }
            LightAtCanopyTop = LightAtTop * exp(-KValue * (MyDepth - CanopyHeight));
            CalculatePmax();
            CalculateImax();
            ProductivityAboveTheCanopy = PhytoBiomass[i]*
            	                          Pmax[i] /
                                         KValue *
                                         log(fabs(Imax[i] + LightAtTop * WATTSTOMICROEINSTEINS) /
                                             fabs(Imax[i] + LightAtCanopyTop * WATTSTOMICROEINSTEINS)
                                             ) *
                                         DAYSTOHOURS /
                                         (MyDepth - CanopyHeight);
            Productivity = ProductivityAboveTheCanopy;

            if (MyMacrophytePointer != NULL)
            {
               LightAtCanopyBottom = LightAtCanopyTop * exp(-KCanopy * CanopyHeight);
            	ProductivityBelowTheCanopy = PhytoBiomass[i]*
            	                             Pmax[i] /
                                            KCanopy *
                                            log(fabs(Imax[i] + LightAtCanopyTop * WATTSTOMICROEINSTEINS) /
                                                fabs(Imax[i] + LightAtCanopyBottom * WATTSTOMICROEINSTEINS)
                                               ) *
                                            DAYSTOHOURS /
                                            CanopyHeight;

               Productivity = (ProductivityAboveTheCanopy * (MyDepth - CanopyHeight) +
                               ProductivityBelowTheCanopy * CanopyHeight) /
                               MyDepth;
            }
         }
         //NutrientLimitation(i);
         GPP[i] = Productivity;
         PhytoProd[i] = Productivity;
         OxygenNetProduction[i] = Productivity / CarbonToOxygenProd;
	   }
   }
}

void TSto0Phytoplankton::Respiration()
{
	double Resp;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
      Resp = RespirationCoefficient *
             Pmax[i] *
             PhytoBiomass[i] *
             DAYSTOHOURS;
      PhytoProd[i] = PhytoProd[i] - ( Resp );
      OxygenNetProduction[i] = OxygenNetProduction[i] -
                               Resp /
                               CarbonToOxygenResp;
	}
}


void TSto0Phytoplankton::Exudation()
{
   double  Exudate;
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		DocStressLoss = DocLoss;
		if (GPP[i] > 0)
		Exudate = GPP[i] * DocStressLoss;
		else Exudate = 0;
		PhytoProd[i] = PhytoProd[i] - Exudate;
	}
}

void TSto0Phytoplankton::CalculatePmax()
{
	double MyWaterTemperature;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
      {
      	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
                                                i,
                                                "Water temperature",
                                                ObjectCode);
         Pmax[i] = (0.832 * MyWaterTemperature - 3.208) / ChlToCarbon[i]; //Regress?o da Mena
      }
   }
}


void TSto0Phytoplankton::CalculateImax()
{
	double MyWaterTemperature;
	TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	for (int i = 0; i < NumberOfBoxes; i++)
      {
      	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), MyWaterTemperature,
                                                i,
                                                "Water temperature",
                                                ObjectCode);
         Imax[i] = 15.47 * MyWaterTemperature - 100.707; //Regress?o da Mena
      }
   }
}

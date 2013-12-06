// Phytoplankton object CPP code file
// @ Pedro Duarte
// Rev. A 00.09.25
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

// TSangoPhytoplankton Class

TSangoPhytoplanktonIntLim::TSangoPhytoplanktonIntLim(TEcoDynClass* APEcoDynClass, char* className)
              : TSangoPhytoplankton(APEcoDynClass, className)
{
    PreBuildSangoPhytoplanktonIntLim();

    int X, Y;
//    TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
          // NumberOfParameters already read in TPhytoplankton
          char MyParameter[65];
          for (int i = Y; i < Y + NumberOfParameters; i++)
          {
            PReadWrite->ReadString(X+2, i, MyParameter);
            if (strcmp(MyParameter, "NMaxUptake") == 0)
            {
               PReadWrite->ReadNumber(X+3, i, NMaxUptake);
            }
            else if (strcmp(MyParameter, "KNO3") == 0)
            {
               PReadWrite->ReadNumber(X+3, i, KNO3);
            }
            else if (strcmp(MyParameter, "KNH4") == 0)
            {
               PReadWrite->ReadNumber(X+3, i, KNH4);
            }
            else if (strcmp(MyParameter, "MinNCellQuota") == 0)
            {
               PReadWrite->ReadNumber(X+3, i, MinNCellQuota);
            }
            else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
            {
               PReadWrite->ReadNumber(X+3, i, MaxNCellQuota);
            }
            else if (strcmp(MyParameter, "KNInternal") == 0)
            {
               PReadWrite->ReadNumber(X+3, i, KNInternal);
            }
          }
        }
        else
        {
#ifdef __BORLANDC__
           MessageBox(0, "Parameters: Undefined object - Sango IntLim Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoPhytoplanktonIntLim::TSangoPhytoplanktonIntLim - Parameters: Undefined object - Sango IntLim Phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

//    PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
    if (PReadWrite != NULL)
    {
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            char MyVariable[65];
            // NumberOfVariables and VariableNameArray already filled in TSangoPhytoplankton
            for (int i = 0; i < NumberOfVariables; i++)
            {
               double MyValue;
               PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
               if (strcmp(MyVariable, "NCellQuota") == 0)
               {
                  PReadWrite->ReadNumber(X + 2 + i, Y + 1, MyValue);
                  //Debugger(MyValue);
                  for (int j = 0; j < NumberOfBoxes; j++)
                  {
                    NCellQuota[j] = MyValue;
                  }
                }
                // More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Variables: Undefined object - Sango IntLim Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSangoPhytoplanktonIntLim::TSangoPhytoplanktonIntLim - Variables: Undefined object - Sango IntLim Phytoplankton" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }
    PosBuildSangoPhytoplanktonIntLim();
}


void TSangoPhytoplanktonIntLim::PreBuildSangoPhytoplanktonIntLim()
{
    NCellQuota = new double[NumberOfBoxes];
    NPhyto = new double[NumberOfBoxes];
    NCellFlux = new double[NumberOfBoxes];
    NUptake = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        NCellQuota[i] = 0.0;
        NPhyto[i] = 0.0;
        NCellFlux[i] = 0.0;
        NUptake[i] = 0.0;
    }
}


void TSangoPhytoplanktonIntLim::PosBuildSangoPhytoplanktonIntLim()
{
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        NPhyto[i] = PhytoBiomass[i] * NCellQuota[i];
    }
}


TSangoPhytoplanktonIntLim::~TSangoPhytoplanktonIntLim()
{
    freeMemory();
}

void TSangoPhytoplanktonIntLim::freeMemory()
{
    try {
       if (NumberOfBoxes > 0)
       {
          delete [] NCellQuota;
          delete [] NPhyto;
          delete [] NCellFlux;
          delete [] NUptake;
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
				cerr << "TSangoPhytoplanktonIntLim::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__

}

bool TSangoPhytoplanktonIntLim::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
 bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Phytoplankton C biomass") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "Phytoplankton N biomass") == 0)
        NPhyto[BoxNumber] = Value;
    else if (strcmp(VariableName, "NCellQuota") == 0)
        NCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "NUptake") == 0)
        NUptake[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll to Carbon") == 0)
        ChlToCarbon[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll : Carbon") == 0)
        ChlCarbonRatio[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll to biomass") == 0)
        ChlToBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Phytop. total GPP") == 0)
		GPP[BoxNumber] = Value / DAYSTOSECONDS;
	else if (strcmp(VariableName, "Phytop. total NPP") == 0)
        NPP[BoxNumber] = Value / DAYSTOSECONDS;
	else if (strcmp(VariableName, "Phytoplankton mortality") == 0)
		PhytoMortality[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}


void TSangoPhytoplanktonIntLim::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Phytoplankton biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton C biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton N biomass") == 0)
		Value = NPhyto[MyBoxNumber];
    /*else if (strcmp(MyParameter, "Phytoplankton P biomass") == 0)
		Value = PPhyto[MyBoxNumber]; */
    else if (strcmp(MyParameter, "NCellQuota") == 0)
		Value = NCellQuota[MyBoxNumber];
    /*else if (strcmp(MyParameter, "PCellQuota") == 0)
		Value = PCellQuota[MyBoxNumber];*/
    else if (strcmp(MyParameter, "NUptake") == 0)
		Value = NUptake[MyBoxNumber];
    /*else if (strcmp(MyParameter, "PUptake") == 0)
		Value = PUptake[MyBoxNumber];*/
    else if (strcmp(MyParameter, "Chlorophyll to Carbon") == 0)
		Value = ChlToCarbon[MyBoxNumber];
	else if (strcmp(MyParameter, "Phytop. total GPP") == 0)
		Value = GPP[MyBoxNumber] * DAYSTOSECONDS;
	else if (strcmp(MyParameter, "Phytop. total NPP") == 0)
        Value = NPP[MyBoxNumber] * DAYSTOSECONDS;
	else if (strcmp(MyParameter, "Phytop. GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
			Value = GPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTOSECONDS;              // mg C/ mg C /h
    }
	else if (strcmp(MyParameter, "Phytop. NPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
		    Value = NPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTOSECONDS;
    }
	else if (strcmp(MyParameter, "Chlorophyll : Carbon") == 0)
		Value = ChlCarbonRatio[MyBoxNumber];
	else if (strcmp(MyParameter, "Chlorophyll to biomass") == 0)
	   	Value = ChlToBiomass[MyBoxNumber];
	else if (strcmp(MyParameter, "Phyto NPP/GPP ratio") == 0)
	{
	    if (GPP[MyBoxNumber]!= 0)                             //NG 5/6/96
			Value = NPP[MyBoxNumber]/GPP[MyBoxNumber]*100;
		else
		    Value = INFINITY;
    }
	else if (strcmp(MyParameter, "Phytoplankton mortality") == 0)
		Value = PhytoMortality[MyBoxNumber];
#ifndef _PORT_FORTRAN_
    else
    {
        Value = 0;
        int j;
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
                      "EcoDynamo Alert - Inquiry 1",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSangoPhytoplanktonIntLim::Inquiry 1 - " << MyParameter << " does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
#endif  // _PORT_FORTRAN_
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

// AP, 2007.07.12
bool TSangoPhytoplanktonIntLim::SaveVariables()
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
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(VariableNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save variables' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(VariableNameArray[i], "Chlorophyll : Carbon") == 0)
            PReadWrite->WriteCell(ChlCarbonRatio[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to biomass") == 0)
            PReadWrite->WriteCell(ChlToBiomass[0], 8);

        // variables from TSangoPhytoplanktonIntLim
        else if (strcmp(VariableNameArray[i], "NCellQuota") == 0)
            PReadWrite->WriteCell(NCellQuota[0], 8);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save daily boundary values
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Daily boundary phytoplankton values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(DailyBoundaryPhytoplankton[j] / ChlToCarbon[0], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TSangoPhytoplanktonIntLim::SaveParameters()
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

        // parameters from TPhytoplankton
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

        // parameters from TSangoPhytoplanktonIntLim
        else if (strcmp(ParametersNameArray[i], "NMaxUptake") == 0)
        {
           PReadWrite->WriteCell(NMaxUptake, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNO3") == 0)
        {
           PReadWrite->WriteCell(KNO3, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNH4") == 0)
        {
           PReadWrite->WriteCell(KNH4, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinNCellQuota") == 0)
        {
           PReadWrite->WriteCell(MinNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxNCellQuota") == 0)
        {
           PReadWrite->WriteCell(MaxNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNInternal") == 0)
        {
           PReadWrite->WriteCell(KNInternal, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TSangoPhytoplanktonIntLim::Go ()
{
   double MyPOMFlux, MyBoxDepth, LimitingNutrient1, LimitingNutrient2;
   // Active methods
   JulianDay = MyPEcoDynClass->GetJulianDay();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      if (MyTransportPointer != NULL)
      {
         MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth,i,"Box depth",ObjectCode);
      }
      else
      	 MyBoxDepth = MyPEcoDynClass->GetBoxDepth(i);
      if (
           (MyBoxDepth > 0.0) &&
      	   (BoxArray[i].BoxToBoundary[WesternBoundaryNumber].TypeOfBoundary != 2) && //No western ocean boundary
           (BoxArray[i].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary != 2) && //No eastern ocean boundary
           (BoxArray[i].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary != 2) &&  //No southern ocean boundary
           (BoxArray[i].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary != 2)  ||  //No northern ocean boundary
           (
              (MyBoxDepth > 0.0) &&(MyTransportPointer == NULL)
           )
         )
      {
         Production(i);
         Respiration(i);
         Exudation(i);
         Mortality(i);
         NitrogenUptake(i);
      }
   }
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      NPP[i] = PhytoProd[i];
      PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
      TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
      if (MySPMPointer != NULL)
      {
      	 MyPOMFlux = PhytoFlux[i] * ChlCarbonRatio[i] * // mg Chl / m3 / d
                     ChlToBiomass[i] / CUBIC /*/ DAYSTOHOURS / HOURSTOSECONDS*/; //convertion from mg Chl / m3 / d to mg DW / L / s
         MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             i, "POM",ObjectCode);
         MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             i, "TPM",ObjectCode);
      }
   }
}


void TSangoPhytoplanktonIntLim::Integrate()
{
    double const Tinny = 0.000001;
    // Integrate phytoplankton biomass within the box
    // Generic = PhytoBiomass; GenericFlux = PhytoFlux;
    Integration(PhytoBiomass, PhytoFlux);
    // PhytoBiomass = Generic; PhytoFlux = GenericFlux;

    // Generic = NPhyto; GenericFlux = NCellFlux;
    Integration(NPhyto, NCellFlux);
    // NPhyto = Generic; NCellFlux = GenericFlux;


    // Transport the phytoplankton
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
      MyTransportPointer->Go(PhytoBiomass, PhytoBiomass, 0.0, 0.0);
      MyTransportPointer->Go(NPhyto, NPhyto, 0.0, 0.0);
      double ABoxDepth;
      JulianDay = MyPEcoDynClass->GetJulianDay();
      //Southern boundary
      int i,j;
      for (j = 0; j < GridColumns; j++)
      {
         i = 0;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                         i * GridColumns + j,
                                         "Box depth",
                                         ObjectCode);
         if (
               (ABoxDepth > 0.0) &&
               (BoxArray[i * GridColumns + j].BoxToBoundary[SouthernBoundaryNumber].TypeOfBoundary == 2)  //Southern ocean boundary
            )
      	{
           PhytoBiomass[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1];
           NPhyto[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1] * NCellQuota[i * GridColumns + j];
      	}
      }
      //Northern boundary
      for (j = 0; j < GridColumns; j++)
      {
         i = GridLines - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                       i * GridColumns + j,
                                       "Box depth",
                                       ObjectCode);
         if (
              (ABoxDepth > 0.0) &&
              (BoxArray[i * GridColumns + j].BoxToBoundary[NorthernBoundaryNumber].TypeOfBoundary == 2) //Northern ocean boundary
            )
         {
              PhytoBiomass[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1];
              NPhyto[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1] * NCellQuota[i * GridColumns + j];
          }
      }
      //Eastern boundary
      for (i = 0; i < GridLines; i++)
      {
         j = GridColumns - 1;
         MyTransportPointer->Inquiry(GetEcoDynClassName(), ABoxDepth,
                                       i * GridColumns + j,
                                       "Box depth",
                                       ObjectCode);
         if (
              (ABoxDepth > 0.0) &&
              (BoxArray[i * GridColumns + j].BoxToBoundary[EasternBoundaryNumber].TypeOfBoundary == 2) //Eastern ocean boundary
            )
         {
              PhytoBiomass[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1];
              NPhyto[i * GridColumns + j] = DailyBoundaryPhytoplankton[JulianDay-1] * NCellQuota[i * GridColumns + j];
         }
      }
   }
   for (int i = 0; i < NumberOfBoxes; i++) {
   	if (PhytoBiomass[i] < 0.0) PhytoBiomass[i] = 0.0;
        if (NPhyto[i] < 0.0) NPhyto[i] = 0.0;
        if (PhytoBiomass[i] > Tinny)
      	   NCellQuota[i] = NPhyto[i] / PhytoBiomass[i];
   }
}

void TSangoPhytoplanktonIntLim::NitrogenUptake(int ABoxNumber)
{
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   int i = ABoxNumber;
   double AmmoniaUpTake, NitrateAndNitriteUptake, NMaxUptakeOfNitrate;
   if (MyNutrientPointer != NULL)
   {
		// the limitation is based on umol l-1 values passed from the
		// communicate method
      double Ammonia, Nitrate, Nitrite;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Ammonia,i,"Ammonia",   //micro M
                                 ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Nitrate,i,"Nitrate",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Nitrite,i,"Nitrite",ObjectCode);
      //...
      if (
          (NCellQuota[i] >= MaxNCellQuota) /*||
          (NCellQuota[i] / PCellQuota[i] > MaxNPRatio)*/
       )
       NUptake[i] = 0.0;
      else
      {
      	 AmmoniaUpTake =  NMaxUptake *  Ammonia / (Ammonia + KNH4) * NPhyto[i];  //mg N m-3 h-1
         NUptake[i] = AmmoniaUpTake;  //mg N m-3 h-1
         NMaxUptakeOfNitrate = MAX(0.0,NMaxUptake - NMaxUptake * Ammonia / (Ammonia + KNH4));
         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -AmmoniaUpTake /HOURSTOSECONDS/** DAYSTOHOURS*/ * CUBIC /NITROGENATOMICWEIGHT,
															i,
															"Ammonia",
															ObjectCode);

         //...
         NitrateAndNitriteUptake = /*NMaxUptake*/NMaxUptakeOfNitrate * (Nitrate + Nitrite) / ((Nitrate + Nitrite) + KNO3) * NPhyto[i]; //mg N m-3 h-1
         NUptake[i] = NUptake[i] + NitrateAndNitriteUptake; //mg N m-3 d-1
         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrate / (Nitrate + Nitrite) / HOURSTOSECONDS/**
                                     DAYSTOHOURS */ * CUBIC /NITROGENATOMICWEIGHT,i,"Nitrate",ObjectCode);
         MyNutrientPointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrite / (Nitrate + Nitrite) / HOURSTOSECONDS/**
                                     DAYSTOHOURS*/ * CUBIC /NITROGENATOMICWEIGHT,i,"Nitrite",ObjectCode);
          //...
    	}
   }
   else
       NUptake[i] = 0.0;
   NCellFlux[i] = NCellFlux[i] + NUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/;   //mg N m-3 d-1
}



void TSangoPhytoplanktonIntLim::NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber)
{
   //No caso de a classe ser invocada a partir do EcoDynamo...
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   //...
   if (MyNutrientPointer != NULL)
   {

     int i = ABoxNumber;  double NLimitation, PLimitation;
     if (NCellQuota[i] < MinNCellQuota)
          NLimitation = 0.0;
     else
          NLimitation = NCellQuota[i] / (NCellQuota[i] + KNInternal);
     if (NLimitation == 0.0)
          Productivity = 0.0;
     else
          Productivity = Productivity * NLimitation;
   }
}


void TSangoPhytoplanktonIntLim::Mortality(int ABoxNumber)
{
   double MortalityFlux, MyPOMFlux, NLoss;
	//TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
	int MyBoxNumber;

   MyBoxNumber = ABoxNumber;

   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      MortalityFlux = PhytoBiomass[MyBoxNumber] * DeathLoss;
      PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - MortalityFlux / DAYSTOHOURS / HOURSTOSECONDS;
      NLoss = MortalityFlux / DAYSTOHOURS / HOURSTOSECONDS * NCellQuota[MyBoxNumber];     //mg N Phyto / m3 / s
      NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - NLoss;
      PhytoMortality[MyBoxNumber] = MortalityFlux; // flux per day for mass balance

      if (MySPMPointer != NULL)
      {

      	 MyPOMFlux = MortalityFlux / DAYSTOHOURS / HOURSTOSECONDS * ChlCarbonRatio[MyBoxNumber] * // mg Chl / m3 / d
                     ChlToBiomass[MyBoxNumber] / CUBIC; //convertion from mg Chl / m3 / d to mg DW / L / d

   		MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             MyBoxNumber,
            	   	          "POM",
               	             ObjectCode);

         MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / d
         	   	             MyBoxNumber,
            	   	          "TPM",
               	             ObjectCode);                      

      }
   }
}







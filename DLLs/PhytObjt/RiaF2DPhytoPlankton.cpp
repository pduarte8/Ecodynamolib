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
	#include <stdlib.h>
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
//
// Constructors invoked outside EcoDyn shell...

TRiaF2DPhytoplankton::TRiaF2DPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
              : TCrestumaLeverPhytoplankton2DVIntLim(APEcoDynClass, className)
{
    BuildRiaF2DPhytoplankton();
}



void TRiaF2DPhytoplankton::BuildRiaF2DPhytoplankton()
{
   CriticalDepthForLandBoundary = MyPEcoDynClass->GetCriticalDepth(); // AP, 050309
   DailyBoundaryPhytoplankton = new double[365];
   for (int i = 0; i < 365; i++)
      DailyBoundaryPhytoplankton[i] = 0;
   OxygenNetProduction = new double[NumberOfBoxes];
   PhytoNLoad = new double[NumberOfBoxes];
   PhytoPLoad= new double[NumberOfBoxes];
   PhytoPlanktonCRiverLoad = new double[NumberOfBoxes];
   PhytoPlanktonNRiverLoad = new double[NumberOfBoxes];
   PhytoPlanktonPRiverLoad = new double[NumberOfBoxes];
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      OxygenNetProduction[i] = 0.0;
      PhytoNLoad[i] = 0.0;
      PhytoPLoad[i] = 0.0;
      PhytoPlanktonCRiverLoad[i] = 0.0;
      PhytoPlanktonNRiverLoad[i] = 0.0;
      PhytoPlanktonPRiverLoad[i] = 0.0;
   }
//   TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Phytoplankton");
   if (PReadWrite != NULL)
   {
       int X, Y, XV, YV;
       if (PReadWrite->FindString("Phytoplankton", X, Y))
       {
          //Debugger(X); Debugger(Y);
          char MyVariable[65], LoadName[100], RiverName[100];
          PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
          // Create array for state variable names
          // read in the variable names
          for (int i = 0; i < NumberOfVariables; i++)
          {
             double MyValue;
             PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
             //strcpy(VariableNameArray[i], MyVariable);

             if (strcmp(MyVariable, "Phytoplankton biomass") == 0)
             {
                if (!PReadWrite->FindString("Phytoplankton biomass values", XV, YV))
                {
                    XV = X + 2 + i;
                    YV = Y;
                }
                for (int j = 0; j < NumberOfBoxes; j++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + j, PhytoBiomass[j]);
                   PhytoBiomass[j] = PhytoBiomass[j] * ChlToCarbon[j];
                   NPhyto[j] = PhytoBiomass[j] * NCellQuota[j];
                   PPhyto[j] = PhytoBiomass[j] * PCellQuota[j];
                }
             }
          }
          if (PReadWrite->FindString("Daily boundary phytoplankton", XV, YV))
          //if (PReadWrite->FindString("Daily boundary phytoplankton", XV, YV) == 0)
          {
            for (int j = 0; j < 365; j++)
            {
               PReadWrite->ReadNumber(XV, YV + 1 + j, DailyBoundaryPhytoplankton[j]);
               /// ?????????????
               DailyBoundaryPhytoplankton[j] = DailyBoundaryPhytoplankton[j] * ChlToCarbon[0];
               //if (j == 364) Debugger(DailyBoundaryPhytoplankton[j]);
            }
          }
          if (PReadWrite->FindString("LoadLines", XV, YV))
          //if (PReadWrite->FindString("LoadLines", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRecord[j].LineCoordinate);
                ALoadRecord[j].LineCoordinate = GridLines - ALoadRecord[j].LineCoordinate;
             }
          }
          if (PReadWrite->FindString("LoadColumns", XV, YV))
          //if (PReadWrite->FindString("LoadColumns", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRecord[j].ColumnCoordinate);
                ALoadRecord[j].ColumnCoordinate = ALoadRecord[j].ColumnCoordinate - 1;
             }
          }
          if (PReadWrite->FindString("LoadName", XV, YV))
          //if (PReadWrite->FindString("LoadName", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                PReadWrite->ReadString(XV, YV + 1 + j, LoadName);
                strcpy(ALoadRecord[j].LoadName, LoadName);
             }
          }
          if (PReadWrite->FindString("PhytoPlanktonCLoad", XV, YV))
          //if (PReadWrite->FindString("PhytoPlanktonCLoad", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRecord[j].PhytoplanktonCLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
          if (PReadWrite->FindString("PhytoPlanktonNLoad", XV, YV))
          //if (PReadWrite->FindString("PhytoPlanktonNLoad", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRecord[j].PhytoplanktonNLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
          if (PReadWrite->FindString("PhytoPlanktonPLoad", XV, YV))
          //if (PReadWrite->FindString("PhytoPlanktonPLoad", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRecord[j].PhytoplanktonPLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
          if (PReadWrite->FindString("RiverLoadLines", XV, YV))
          //if (PReadWrite->FindString("RiverLoadLines", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].LineCoordinate);
                ALoadRiverRecord[j].LineCoordinate = GridLines - ALoadRiverRecord[j].LineCoordinate;
                //Debugger(ALoadRiverRecord[j].LineCoordinate);
             }
          }
          if (PReadWrite->FindString("RiverLoadColumns", XV, YV))
          //if (PReadWrite->FindString("RiverLoadColumns", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadNumber(XV, YV + 1 + j, ALoadRiverRecord[j].ColumnCoordinate);
                ALoadRiverRecord[j].ColumnCoordinate = ALoadRiverRecord[j].ColumnCoordinate - 1;
                //Debugger(ALoadRiverRecord[j].ColumnCoordinate);
             }
          }
          if (PReadWrite->FindString("RiverName", XV, YV))
          //if (PReadWrite->FindString("RiverName", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                PReadWrite->ReadString(XV, YV + 1 + j, RiverName);
                strcpy(ALoadRiverRecord[j].RiverName, RiverName);
             }
          }
          if (PReadWrite->FindString("PhytoPlanktonCRiverLoad", XV, YV))
          //if (PReadWrite->FindString("PhytoPlanktonCRiverLoad", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRiverRecord[j].PhytoplanktonCRiverLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
          if (PReadWrite->FindString("PhytoPlanktonNRiverLoad", XV, YV))
          //if (PReadWrite->FindString("PhytoPlanktonNRiverLoad", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRiverRecord[j].PhytoplanktonNRiverLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
          if (PReadWrite->FindString("PhytoPlanktonPRiverLoad", XV, YV))
          //if (PReadWrite->FindString("PhytoPlanktonPRiverLoad", XV, YV) == 0)
          {
             for (int j = 0; j < NumberOfRiverLoads; j++)
             {
                int offset = j * NumberOfDaysForLoads;
                for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
                {
                   PReadWrite->ReadNumber(XV, YV + 1 + Loads + offset,
                        ALoadRiverRecord[j].PhytoplanktonPRiverLoad[Loads]); //Loads should be in mg / L / s
                }
             }
          }
       }
       else
#ifdef __BORLANDC__
          MessageBox(0, "Variables:Undefined object - RiaF2DPhytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TRiaF2DPhytoplankton::BuildRiaF2DPhytoplankton - Variables: Undefined object" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
   }
}

TRiaF2DPhytoplankton::~TRiaF2DPhytoplankton()
{
    freeMemory();
}

void TRiaF2DPhytoplankton::freeMemory()
{
    try {
       if (NumberOfBoxes > 0)
       {
          delete [] OxygenNetProduction;
          delete [] PhytoNLoad;
          delete [] PhytoPLoad;
          delete [] PhytoPlanktonCRiverLoad;
          delete [] PhytoPlanktonNRiverLoad;
          delete [] PhytoPlanktonPRiverLoad;
       }
       delete [] DailyBoundaryPhytoplankton;
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
				cerr << "TRiaF2DPhytoplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


void TRiaF2DPhytoplankton::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{

	// From Zooplankton this comes back as mg C m-3 day-1
	// In the pascal code it says it is ug C m-3 day-1 but I think it is wrong there - JGF 96.03.11

	// The old pascal code had phyoplankton in ug m-3, which is why the zooplankton (see above)
	// sent it back like that. Now the phyto stays as mg m-3, so the zoo MUST return the values
	// without multiplying by cubic NG/JGF 96.06.17
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);

//	for (int i=0; i < NumberOfVariables; i++) {
        if (strcmp(MyParameter, "Phytoplankton biomass") == 0) {
            PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Value;
            return;
        }
        else if (strcmp(MyParameter, "Phytoplankton N biomass") == 0) {
            NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] + Value;
            return;
        }
        else if (strcmp(MyParameter, "Phytoplankton P biomass") == 0) {
            PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] + Value;
            return;
        }
//    }
#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
	char Caption[129];
	strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());

	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 2",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "TRiaF2DPhytoplankton::Update 2 - " << MyParameter << " does not exist" << endl;
#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
}

// AP, 2007.07.12
bool TRiaF2DPhytoplankton::SaveVariables()
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
     *
     * 1st line
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfVariables; i++)
    {
        PReadWrite->WriteSeparator();
        // variables from TCrestumaLeverPhytoplankton2DV
        if (strcmp(VariableNameArray[i], "Phytoplankton biomass") == 0)
            PReadWrite->WriteCell(RiverPhyto / ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton initial biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonInitialBiomass, 8);

        else if (strcmp(VariableNameArray[i], "Phytoplankton final biomass") == 0)
            PReadWrite->WriteCell(PhytoplanktonFinalBiomass, 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll : Carbon") == 0)
            PReadWrite->WriteCell(ChlCarbonRatio[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to biomass") == 0)
            PReadWrite->WriteCell(ChlToBiomass[0], 8);

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Simulation interval (hours)") == 0)
            PReadWrite->WriteCell(SimulationInterval, 8);

        // variables from TCrestumaLeverPhytoplankton2DVIntLim
        else if (strcmp(VariableNameArray[i], "NCellQuota") == 0)
            PReadWrite->WriteCell(NCellQuota[0], 8);

        else if (strcmp(VariableNameArray[i], "PCellQuota") == 0)
            PReadWrite->WriteCell(PCellQuota[0], 8);
    }
    PReadWrite->WriteSeparator(true);
    /*
     * 2nd line - repeat some values of variables
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

        else if (strcmp(VariableNameArray[i], "Chlorophyll to Carbon") == 0)
            PReadWrite->WriteCell(ChlToCarbon[0], 8);

        else if (strcmp(VariableNameArray[i], "Simulation interval (hours)") == 0)
            PReadWrite->WriteCell(SimulationInterval, 8);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * other variables' values - from TCrestumaLeverPhytoplankton2DV
     */
    PReadWrite->WriteCell("Phytoplankton biomass values");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfBoxes; j++)
    {
        PReadWrite->WriteCell(PhytoBiomass[j] / ChlToCarbon[j], 8);
        PReadWrite->WriteSeparator(true);
    }

    /*
     * save loads values
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("RiverLoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverLoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("RiverName");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        PReadWrite->WriteCell(GridLines - ALoadRiverRecord[j].LineCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRiverRecord[j].ColumnCoordinate + 1, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRiverRecord[j].RiverName);
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("PhytoPlanktonBiomassRiverLoad");
    // from TRiaF2DPhytoplankton
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonCRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonNRiverLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonPRiverLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonBiomassLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonCRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonNRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRiverRecord[j].PhytoplanktonPRiverLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    /*
     * from TRiaF2DPhytoplankton
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("LoadLines");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadColumns");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("LoadName");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        PReadWrite->WriteCell(GridLines - ALoadRecord[j].LineCoordinate, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRecord[j].ColumnCoordinate + 1, 0);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ALoadRecord[j].LoadName);
        PReadWrite->WriteSeparator(true);
    }

    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("PhytoPlanktonCLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonNLoad");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("PhytoPlanktonPLoad");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < NumberOfLoads; j++)
    {
        for (int Loads = 0; Loads < NumberOfDaysForLoads; Loads++)
        {
            PReadWrite->WriteCell(ALoadRecord[j].PhytoplanktonCLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].PhytoplanktonNLoad[Loads], 8);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(ALoadRecord[j].PhytoplanktonPLoad[Loads], 8);
            PReadWrite->WriteSeparator(true);
        }
    }

    /*
     * daily boundaries
     */
    PReadWrite->WriteSeparator(true);
    PReadWrite->WriteCell("Daily boundary phytoplankton");
    PReadWrite->WriteSeparator(true);
    for (int j = 0; j < 365; j++)
    {
        PReadWrite->WriteCell(DailyBoundaryPhytoplankton[j] / ChlToCarbon[0], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}


void TRiaF2DPhytoplankton::Go()
{
    int k, index; double Depth, MyPOMFlux;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

    JulianDay = MyPEcoDynClass->GetJulianDay();
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
    //Loads();
    for (k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {

       index = pSubDomain->BoxNumber[k];
       //if (k == 102) Debugger(index);
       if (MyTransportPointer != NULL)
       {
          MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                             index,
                                             "Box depth",
                                             ObjectCode);
       }
       else Depth = MyPEcoDynClass->GetBoxDepth(index);
       if (
             (Depth > CriticalDepthForLandBoundary) &&
             (BoxArray[index].Dummybool1) &&
             (BoxArray[index].BoxToBoundary[4].TypeOfBoundary != 2) && //No western ocean boundary
             (BoxArray[index].BoxToBoundary[2].TypeOfBoundary != 2) && //No eastern ocean boundary
             (BoxArray[index].BoxToBoundary[3].TypeOfBoundary != 2) &&  //No southern ocean boundary
             (BoxArray[index].BoxToBoundary[1].TypeOfBoundary != 2)    //No northern ocean boundary
          )
       {
          if (
                (NCellQuota[index] < PhytoBiomass[index]) &&
                (PCellQuota[index] < PhytoBiomass[index]) &&
                (PCellQuota[index] < NCellQuota[index]) &&
                (PhytoBiomass[index] > 0.000001)
             )
           {
                Production(index);
                Respiration(index);
                Exudation(index);
                NitrogenUptake(index);
                PhosphorusUptake(index);
                Settling(index);
          }
          Mortality(index);
          NPP[index] = PhytoProd[index];
          PhytoFlux[index] = PhytoFlux[index] + PhytoProd[index];
          MyPOMFlux = PhytoProd[index] - PhytoMortality[index] - SettlingLoss[index];
          //Importa separar o PhytoFlux do MyPOMFlux, pois o primeiro pode ser afectado por trocas
          //com outras classes, enquanto que o segundo ? somente devido aos processos internos do fito.
          //Quando algum objecto remover/adicionar Phyto, deve tamb?m remover/adicionar POM no objecto de SPM.
          if (MySPMPointer != NULL)
          {
      	     //MyPOMFlux = PhytoFlux[index] * ChlCarbonRatio[index] * // mg Chl / m3 / s
             //            ChlToBiomass[index] / CUBIC; //convertion from mg Chl / m3 / d to mg DW / L / s
             MyPOMFlux = MyPOMFlux * ChlCarbonRatio[index] * // mg Chl / m3 / s
                         ChlToBiomass[index] / CUBIC; //convertion from mg Chl / m3 / d to mg DW / L / s
             MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / s
         	   	          index,
            	   	          "POM",
               	                  ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), MyPOMFlux,     //mg DW / L / s
         	   	           index,
            	   	          "TPM",
               	                  ObjectCode);
             /*MySPMPointer->Update(GetEcoDynClassName(), PhytoFlux[index] / CUBIC,     //mg C / L / s
         	   	           index,
            	   	          "LiveC",
               	                  ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), PhytoFlux[index] / CUBIC * NCellQuota[index],     //mg N / L / s
         	   	           index,
            	   	          "LiveN",
               	                  ObjectCode);
             MySPMPointer->Update(GetEcoDynClassName(), PhytoFlux[index] / CUBIC * NCellQuota[index],     //mg P / L / s
         	   	           index,
            	   	          "LiveP",
               	                  ObjectCode); */
          }
       }
       /*else
       if (Depth > CriticalDepthForLandBoundary)
       {
         PhytoBiomass[index] = DailyBoundaryPhytoplankton[JulianDay-1];
         NPhyto[index] = DailyBoundaryPhytoplankton[JulianDay-1] * NCellQuota[index];
         PPhyto[index] = DailyBoundaryPhytoplankton[JulianDay-1] * PCellQuota[index];
       } */
    }

}

void TRiaF2DPhytoplankton::Integrate()
{
   int k, index;
   double NPhytoLoad, PPhytoLoad, Depth;
   double const Tinny = 0.000001;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   //MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
   for (k = 0; k < MyNumberOfBoxesInSubDomain; k++)
   {
      index = pSubDomain->BoxNumber[k];
      GenericProduct1[index] = PhytoBiomass[index] * DailyMeanGPP[index];
      if (GenericProduct1[index] < 0.0) GenericProduct1[index] = 0.0;
   }
   if (MyTransportPointer != NULL)
      MyTransportPointer->Go(GenericProduct1, GenericProduct2, 0.0, 0.0);

//   Generic = PhytoBiomass; GenericFlux = PhytoFlux;
   Integration(PhytoBiomass, PhytoFlux);
//   PhytoBiomass = Generic; PhytoFlux = GenericFlux;

//   Generic = NPhyto; GenericFlux = NCellFlux;
   Integration(NPhyto, NCellFlux);
//   NPhyto = Generic; NCellFlux = GenericFlux;

//   Generic = PPhyto; GenericFlux = PCellFlux;
   Integration(PPhyto, PCellFlux);
//   PPhyto = Generic; PCellFlux = GenericFlux;

   /*for (k = 0; k < MyNumberOfBoxesInSubDomain; k++)
   {
      index = pSubDomain->BoxNumber[k];
      if (PhytoBiomass[index] < 0.0) PhytoBiomass[index] = 0.0;
      if (NPhyto[index] < 0.0) NPhyto[index] = 0.0;
      if (PPhyto[index] < 0.0) PPhyto[index] = 0.0;
   } */
   /*if (MyTransportPointer != NULL)
      MyTransportPointer->Go(PhytoBiomass, PhytoLoad, 0.0, 0.0); */

   if (MyTransportPointer != NULL)
        MyTransportPointer->Go(PhytoBiomass, PhytoLoad,
                             NPhyto, PhytoNLoad,
                             PPhyto, PhytoPLoad,
                             GenericProduct2, GenericProduct2,
                             GenericProduct2, GenericProduct2, 
                             0.0, 0.0);

   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
   for (k = 0; k < MyNumberOfBoxesInSubDomain; k++)
   {
      index = pSubDomain->BoxNumber[k];
      if (PhytoBiomass[index] > Tinny)
         DailyMeanGPP[index] = GenericProduct1[index] / PhytoBiomass[index];
      else
         DailyMeanGPP[index] = 0.0;
      if (MyTransportPointer != NULL)
      {
          MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                             index,
                                             "Box depth",
                                             ObjectCode);
      }
      else Depth = MyPEcoDynClass->GetBoxDepth(index);
      if (
             (Depth > CriticalDepthForLandBoundary) &&
             (BoxArray[index].Dummybool1) &&
             (
                (BoxArray[index].BoxToBoundary[4].TypeOfBoundary == 2) || //No western ocean boundary
                (BoxArray[index].BoxToBoundary[2].TypeOfBoundary == 2) || //No eastern ocean boundary
                (BoxArray[index].BoxToBoundary[3].TypeOfBoundary == 2) ||  //No southern ocean boundary
                (BoxArray[index].BoxToBoundary[1].TypeOfBoundary == 2)    //No northern ocean boundary
             )
         )
      {
         PhytoBiomass[index] = DailyBoundaryPhytoplankton[JulianDay-1];
         NPhyto[index] = DailyBoundaryPhytoplankton[JulianDay-1] * NCellQuota[index];
         PPhyto[index] = DailyBoundaryPhytoplankton[JulianDay-1] * PCellQuota[index];
      }
      if (PhytoBiomass[index] < 0.0) PhytoBiomass[index] = 0.0;
      if (NPhyto[index] < 0.0) NPhyto[index] = 0.0;
      if (PPhyto[index] < 0.0) PPhyto[index] = 0.0;

      if (PhytoBiomass[index] > Tinny)
      {
      	NCellQuota[index] = NPhyto[index] / PhytoBiomass[index];
      	PCellQuota[index] = PPhyto[index] / PhytoBiomass[index];
      }
      if (MySPMPointer != NULL)
      {
         MySPMPointer->Update(GetEcoDynClassName(), PhytoBiomass[index] / CUBIC,     //mg C / L
         	   	           index,
            	   	          "LiveC",
               	                   ObjectCode);
         MySPMPointer->Update(GetEcoDynClassName(), NPhyto[index] / CUBIC,     //mg N / L
         	   	           index,
            	   	          "LiveN",
               	                  ObjectCode);
         MySPMPointer->Update(GetEcoDynClassName(), PPhyto[index] / CUBIC,     //mg P / L
         	   	           index,
            	   	          "LiveP",
               	                  ObjectCode);
      }
   }




   /*for (k = 0; k < MyNumberOfBoxesInSubDomain; k++)
   {
      index = pSubDomain->BoxNumber[k];
      //NCellFlux[index] = 0.0; PCellFlux[index] = 0.0;
      if (PhytoBiomass[index] > 0.0)
      {
        //Debugger(NPhyto[index]); Debugger(PhytoBiomass[index]);
        //Debugger(PPhyto[index]);
      	NCellQuota[index] = NPhyto[index] / PhytoBiomass[index];
      	PCellQuota[index] = PPhyto[index] / PhytoBiomass[index];
      }
      if (MySPMPointer != NULL)
      {
         MySPMPointer->Update(GetEcoDynClassName(), PhytoBiomass[index] / CUBIC,     //mg C / L
         	   	           index,
            	   	          "LiveC",
               	                   ObjectCode);
         MySPMPointer->Update(GetEcoDynClassName(), NPhyto[index] / CUBIC,     //mg N / L
         	   	           index,
            	   	          "LiveN",
               	                  ObjectCode);
         MySPMPointer->Update(GetEcoDynClassName(), PPhyto[index] / CUBIC,     //mg P / L
         	   	           index,
            	   	          "LiveP",
               	                  ObjectCode);
      }
   } */
   Loads();
}

void TRiaF2DPhytoplankton::Production(int ABoxNumber)
{
   double BoxDepth, TotalLightAtTop, LightAtTop, LightAtBottom;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
   * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
   * MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
   * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      if (MyLightPointer != NULL)
      {
         if (MyTransportPointer != NULL)
            MyTransportPointer->Inquiry(GetEcoDynClassName(), BoxDepth, MyBoxNumber,"Box depth",ObjectCode);
         else
            BoxDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
         if (MySPMPointer != NULL)
            MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,MyBoxNumber,"Extinction coefficient Ktot", ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtTop,MyBoxNumber,"Total surface irradiance",ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,MyBoxNumber,"PAR surface irradiance",ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom, TotalLightAtTop, BoxDepth, KValue, MyBoxNumber,
                                 "Sub-surface irradiance",ObjectCode);
         //MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,BoxDepth,MyBoxNumber,"Sub-surface PAR irradiance",ObjectCode);
         //if (MyBoxNumber == 10918) Debugger(KValue);
         if ((BoxDepth > 0.0) && (LightAtTop > 0.0))
         {
            if (KValue < 0.00001)
            {
               Debugger(KValue);
               Debugger(MyBoxNumber);
            }
            Productivity = PhytoBiomass[MyBoxNumber]
                                   * Pmax[MyBoxNumber]
                                   * 2.718282
                     	           /( KValue * BoxDepth)
                                   * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                                   -exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                                   ) / HOURSTOSECONDS;    //mg C m-3 s-1
							  			//* DAYSTOHOURS;

            /*if ((MyPEcoDynClass->GetJulianDay() == 31) && (MyPEcoDynClass->GetCurrTime() > 14.21083) && (MyBoxNumber == 12687))
            {
               Debugger(101);
               Debugger(Productivity);
            } */

            if (MyWaterTemperaturePointer != NULL)
            {
               MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
               Productivity = Productivity * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminPhotosynthesis);
            }

            /*if ((MyPEcoDynClass->GetJulianDay() == 31) && (MyPEcoDynClass->GetCurrTime() > 14.21083) && (MyBoxNumber == 12687))
            {
               Debugger(102);
               Debugger(Productivity);
            } */

            NutrientLimitation(MyBoxNumber);

            /*if ((MyPEcoDynClass->GetJulianDay() == 31) && (MyPEcoDynClass->GetCurrTime() > 14.21083) && (MyBoxNumber == 12687))
            {
               Debugger(103);
               Debugger(Productivity);
            } */

            if (MyPEcoDynClass->GetJulianDay() != ADay[MyBoxNumber])
            {
               DailyMeanGPP[MyBoxNumber] = 0.0;
               NumberOfParcels[MyBoxNumber] = 0;
               ADay[MyBoxNumber] = ADay[MyBoxNumber] + 1;
            }
               DailyMeanGPP[MyBoxNumber] =
                  	(
                     	   DailyMeanGPP[MyBoxNumber] * NumberOfParcels[MyBoxNumber] +
                     	   Productivity / CarbonToOxygenProd / (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * HOURSTOSECONDS
                        )  / (NumberOfParcels[MyBoxNumber] + 1);  //mg O2 / mg Chla / h
               NumberOfParcels[MyBoxNumber] = NumberOfParcels[MyBoxNumber] + 1;

         }
         else Productivity = 0.0;
         GPP[MyBoxNumber] = Productivity;
         PhytoProd[MyBoxNumber] = Productivity;
         OxygenNetProduction[MyBoxNumber] = Productivity / CarbonToOxygenProd;
         /*if ((MyPEcoDynClass->GetJulianDay() == 31) && (MyPEcoDynClass->GetCurrTime() > 14.21083)&& (MyBoxNumber == 12687))
         {
             Debugger(104);
             Debugger(DailyMeanGPP[12687]);

         } */
      }
   }
}


void TRiaF2DPhytoplankton::Respiration(int ABoxNumber)
{
   double Resp, WaterTemperature;
   int MyBoxNumber;
   double const OxygenMolecularWeight = 32.0;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      Resp = MaintenanceRespiration[MyBoxNumber] / HOURSTOSECONDS/** DAYSTOHOURS*/;    //mmol O2 / mgChl / s
      if (MyWaterTemperaturePointer != NULL)
         MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,MyBoxNumber,"Water temperature",ObjectCode);
      else
         WaterTemperature = 0.0;
      if (PhytoProd[MyBoxNumber] <= 0.0)
         Resp = Resp + RespirationCoefficient * DailyMeanGPP[MyBoxNumber] / OxygenMolecularWeight
                       * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminRespiration) / HOURSTOSECONDS;
      else
         Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient *
               	       DailyMeanGPP[MyBoxNumber] / OxygenMolecularWeight
                       * TemperatureArrheniusExponentialLimitation(WaterTemperature, TminRespiration) / HOURSTOSECONDS;
            //Resp in //mmol O2 / mg Chl / s
      Resp = Resp * CarbonToOxygenProd * (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * OxygenMolecularWeight;
            //Resp in //mg C /m3 / s
      PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - ( Resp );
      OxygenNetProduction[MyBoxNumber] = OxygenNetProduction[MyBoxNumber] - Resp / CarbonToOxygenResp;

      if (MyNutrientPointer != NULL)
      {
         /*MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /(NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Ammonia",
												ObjectCode);      //Return as umol N / m3
         NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Resp * NCellQuota[MyBoxNumber];
         MyNutrientPointer->Update(GetEcoDynClassName(),
                                   Resp * CUBIC /(PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),
				   MyBoxNumber,"Phosphate", ObjectCode);     //Return as umol P / m3
         PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Resp * PCellQuota[MyBoxNumber];   */
         MyNutrientPointer->Update(GetEcoDynClassName(), OxygenNetProduction[MyBoxNumber] / CUBIC,MyBoxNumber,"Oxygen", ObjectCode);//Return as mg O2 / L / s
      }
   }
}


void TRiaF2DPhytoplankton::Exudation(int ABoxNumber)
{
   double  Exudate;
   int MyBoxNumber;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   MyBoxNumber = ABoxNumber;
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      DocStressLoss = DocLoss;
      if (GPP[MyBoxNumber] > 0.0)
         Exudate = GPP[MyBoxNumber] * DocStressLoss;
      else Exudate = 0.0;
         PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - Exudate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      /*if (MyNutrientPointer != NULL)
      {
          MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),MyBoxNumber,"Ammonia",ObjectCode);//Return as umol N / m3
          NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Exudate * NCellQuota[MyBoxNumber];

          MyNutrientPointer->Update(GetEcoDynClassName(), Exudate * CUBIC /
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),MyBoxNumber,"Phosphate",ObjectCode);//Return as umol P / m3
          PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Exudate * PCellQuota[MyBoxNumber];
      } */
      //...
   }
}


void TRiaF2DPhytoplankton::NutrientLimitation(int ABoxNumber)
{
   //No caso de a classe ser invocada a partir do EcoDynamo...
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   //...
   if (MyNutrientPointer != NULL)
   {
	int i = ABoxNumber;  double NLimitation, PLimitation;
        if (PCellQuota[i] < MinPCellQuota)
           PLimitation = 0.0;
        else
           PLimitation = PCellQuota[i] / (PCellQuota[i] + KPInternal);
        if (NCellQuota[i] < MinNCellQuota)
           NLimitation = 0.0;
        else
           NLimitation = NCellQuota[i] / (NCellQuota[i] + KNInternal);
        if ((PLimitation == 0.0) || (NLimitation == 0.0))
           Productivity = 0.0;
        else
           Productivity = Productivity * MIN(PLimitation,NLimitation);
   }
}

void TRiaF2DPhytoplankton::NitrogenUptake(int ABoxNumber)
{
   int i = ABoxNumber;
   if (/*(PhytoBiomass[ABoxNumber] > 0.0)*/(NCellQuota[i] > 0.0000000001) && (PCellQuota[i] > 0.0000000001))
   {
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();

   double AmmoniaUpTake, NitrateAndNitriteUptake, NMaxUptakeOfNitrate;
   if (MyNutrientPointer != NULL)
   {
      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Ammonia, Nitrate, Nitrite, AmmoniumUptakeLimitation;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Ammonia,i,"Ammonia",   /*micro M*/ ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Nitrate,i,"Nitrate",ObjectCode);
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Nitrite,i,"Nitrite",ObjectCode);
      //...
      /*if ((JulianDay == 3) && (MyPEcoDynClass->GetCurrentTime() >= 7.339) && (i == 28221))
      {
         Debugger(PhytoBiomass[i]);Debugger(NCellQuota[i]);
         Debugger(PCellQuota[i]); Debugger(Ammonia); Debugger(Nitrate); Debugger(Nitrite); Debugger(KNH4); Debugger(MaxNCellQuota);
      } */
      if (
            (NCellQuota[i] >= MaxNCellQuota) ||
            (NCellQuota[i] / PCellQuota[i] > MaxNPRatio)
         )
         NUptake[i] = 0.0;
      else
      {
         if (Ammonia > 0.0000000001)
         {
                AmmoniumUptakeLimitation = Ammonia / (Ammonia + KNH4) * (1.0 - NCellQuota[i] / MaxNCellQuota); //dimensionless
      	        AmmoniaUpTake =  NMaxUptake *  AmmoniumUptakeLimitation * NPhyto[i];  //mg N m-3 h-1
         }
         else
                AmmoniaUpTake = 0.0;
         NUptake[i] = AmmoniaUpTake;  //mg N m-3 h-1
         NMaxUptakeOfNitrate = MAX(0.0, NMaxUptake - NMaxUptake * AmmoniumUptakeLimitation);
         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -AmmoniaUpTake /HOURSTOSECONDS/** DAYSTOHOURS*/ * CUBIC /NITROGENATOMICWEIGHT,
															i,
															"Ammonia",
															ObjectCode);
         //...
         if (Nitrate + Nitrite > 0.0000000001)
                NitrateAndNitriteUptake = /*NMaxUptake*/NMaxUptakeOfNitrate * (Nitrate + Nitrite) / ((Nitrate + Nitrite) + KNO3) *
                                  (1.0 - NCellQuota[i] / MaxNCellQuota) * NPhyto[i]; //mg N m-3 h-1
         else
                NitrateAndNitriteUptake = 0.0;
         NUptake[i] = NUptake[i] + NitrateAndNitriteUptake; //mg N m-3 d-1
         //No caso de a classe ser invocada a partir do EcoDynamo...
         if (NitrateAndNitriteUptake > 0.0)
         {
                MyNutrientPointer->Update(GetEcoDynClassName(), -NitrateAndNitriteUptake * Nitrate / (Nitrate + Nitrite) / HOURSTOSECONDS*
                                   CUBIC /NITROGENATOMICWEIGHT,i,"Nitrate",ObjectCode);
                MyNutrientPointer->Update(GetEcoDynClassName(),
                                   -NitrateAndNitriteUptake * Nitrite / (Nitrate + Nitrite) / HOURSTOSECONDS *
                                   CUBIC /NITROGENATOMICWEIGHT,i,"Nitrite",ObjectCode);
         }
          //...
    	}
    }
    else
       NUptake[i] = 0.0;
    NCellFlux[i] = NCellFlux[i] + NUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/;   //mg N m-3 d-1
    }
}




void TRiaF2DPhytoplankton::PhosphorusUptake(int ABoxNumber)
{
   int i = ABoxNumber;
   if (/*(PhytoBiomass[ABoxNumber] > 0.0)*/(NCellQuota[i] > 0.0000000001) && (PCellQuota[i] > 0.0000000001))
   {
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyNutrientPointer != NULL)
   {
      // the limitation is based on umol l-1 values passed from the
      // communicate method
      double Phosphate;
      //No caso de a classe ser invocada a partir do EcoDynamo...
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), Phosphate,i,"Phosphate",ObjectCode);
      //...
      if (
            (PCellQuota[i] > MaxPCellQuota) ||
            (NCellQuota[i] / PCellQuota[i] <= MinNPRatio)
         )
         PUptake[i] = 0.0;
      else
      {
         if (Phosphate > 0.0000000001)
                PUptake[i] = PMaxUptake *  Phosphate / (Phosphate + KP) * (1.0 - PCellQuota[i] / MaxPCellQuota) * PPhyto[i];
         else
                PUptake[i] = 0.0;
         //No caso de a classe ser invocada a partir do EcoDynamo...
         MyNutrientPointer->Update(GetEcoDynClassName(), -PUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/ * CUBIC / PHOSPHORUSATOMICWEIGHT,
                                        i,
                                        "Phosphate",
                                        ObjectCode);
         //...
      }
   }
   else
      PUptake[i] = 0.0;
   PCellFlux[i] = PCellFlux[i] + PUptake[i] / HOURSTOSECONDS/** DAYSTOHOURS*/;  //mg P m-3 s-1
   }
}

    /*
void TRiaF2DPhytoplankton::Mortality(int ABoxNumber)
{
   double Release = 0;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;

   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      Release = PhytoBiomass[MyBoxNumber] * DeathLoss / DAYSTOSECONDS;
      PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - Release;
      PhytoMortality[MyBoxNumber] = Release; // flux per second for mass balance
      //No caso de a classe ser invocada a partir do EcoDynamo...
      if (MyNutrientPointer != NULL)
      {
         MyNutrientPointer->Update(GetEcoDynClassName(), Release * CUBIC /
                                    (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);
         NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Release * NCellQuota[MyBoxNumber];

         MyNutrientPointer->Update(GetEcoDynClassName(), Release * CUBIC /
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
         PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Release * PCellQuota[MyBoxNumber];
      }
      //...
   }
}  */


void TRiaF2DPhytoplankton::Mortality(int ABoxNumber)
{
   double CLoss, NLoss, PLoss, TotalLoss;
   TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;

   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      if (
            (NCellQuota[MyBoxNumber] >= PhytoBiomass[MyBoxNumber]) ||
            (PCellQuota[MyBoxNumber] >= PhytoBiomass[MyBoxNumber]) ||
            (PCellQuota[MyBoxNumber] >= PhytoBiomass[MyBoxNumber])
         )
         PhytoMortality[MyBoxNumber] = PhytoBiomass[MyBoxNumber] / MyPEcoDynClass->GetTimeStep();
      else
         PhytoMortality[MyBoxNumber] = PhytoBiomass[MyBoxNumber] * DeathLoss / DAYSTOSECONDS;

      PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - PhytoMortality[MyBoxNumber];// flux per second for mass balance
      TotalLoss = PhytoMortality[MyBoxNumber] * ChlCarbonRatio[MyBoxNumber] * ChlToBiomass[MyBoxNumber];
      CLoss = PhytoMortality[MyBoxNumber];     //mg C Phyto / m3 / s
      NLoss = PhytoMortality[MyBoxNumber] * NCellQuota[MyBoxNumber];     //mg N Phyto / m3 / s
      NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - NLoss;
      PLoss = PhytoMortality[MyBoxNumber] * NCellQuota[MyBoxNumber];     //mg P Phyto / m3 / s
      PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - PLoss;

      if (MySPMPointer != NULL)
      {
         MySPMPointer->Update(GetEcoDynClassName(), TotalLoss / CUBIC, MyBoxNumber,"POM",ObjectCode);   //mg DW / L / s
         MySPMPointer->Update(GetEcoDynClassName(), TotalLoss / CUBIC ,MyBoxNumber,"TPM",ObjectCode);   //mg DW / L / s
         MySPMPointer->Update(GetEcoDynClassName(), CLoss /CUBIC,
                              MyBoxNumber,"DetritalC",ObjectCode);       //mg C Phyto / L / s
         MySPMPointer->Update(GetEcoDynClassName(), NLoss /CUBIC,
                              MyBoxNumber,"DetritalN",ObjectCode);        //mg N Phyto / L / s
         MySPMPointer->Update(GetEcoDynClassName(), PLoss / CUBIC,
                              MyBoxNumber,"DetritalP",ObjectCode);         //mg N Phyto / L / s
      }
   }
}


void TRiaF2DPhytoplankton::Settling(int MyBoxNumber)
{
   double MyDepth, CLoss, NLoss, PLoss, MySettlingSpeed;
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
        MySettlingSpeed = SettlingSpeed / DAYSTOSECONDS; //m/s
        TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
        TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
        if (MyTransportPointer != NULL)
        {
                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
                                              MyBoxNumber,
                                              "Box depth",
                                              ObjectCode);
        }
        else
                MyDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
        SettlingLoss[MyBoxNumber] = MySettlingSpeed  * PhytoBiomass[MyBoxNumber] / MyDepth;
        PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - SettlingLoss[MyBoxNumber];
        CLoss = MySettlingSpeed * PhytoBiomass[MyBoxNumber] / MyDepth;     //mg CPhyto / m3 / s
        NLoss = MySettlingSpeed * NPhyto[MyBoxNumber] / MyDepth;     //mg NPhyto / m3 / s
        NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - NLoss;
        PLoss = MySettlingSpeed * PPhyto[MyBoxNumber] / MyDepth;     //mg PPhyto / m3 / s
        PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - PLoss;

        if (MySedimentPointer != NULL)
        {
                // SedimentDepth;
                //MySedimentPointer->Inquiry(GetEcoDynClassName(), SedimentDepth,MyBoxNumber,"LayerDepth 1",ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), CLoss * /*SedimentDepth*/MyDepth / ChlToCarbon[MyBoxNumber] * ChlToBiomass[MyBoxNumber]/*mg / m2 / s*/, MyBoxNumber, "OrganicMass 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), CLoss * /*SedimentDepth*/MyDepth/*mg / m2 / s*/, MyBoxNumber, "Corganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), NLoss * /*SedimentDepth*/MyDepth/** CUBIC / NITROGENATOMICWEIGHT*/, MyBoxNumber,"Norganic 1",ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), PLoss * /*SedimentDepth*/MyDepth/** CUBIC /PHOSPHORUSATOMICWEIGHT*/, MyBoxNumber, "Porganic 1", ObjectCode);
        }
   }
}

double TRiaF2DPhytoplankton::TemperatureArrheniusExponentialLimitation(double ATemperature, double AReferenceTemperature)
{
   //Default values according to Paul Tett (see reference below)
   //TemperatureAugmentationRate = 0.069?C-1, AReferenceTemperature = 20 ?C
   return exp(TemperatureAugmentationRate * (ATemperature - AReferenceTemperature));  //see Tett, P., 2000. Parameterising a microplankton model.
                                                                                      //     Department of Biological Sciences, Napier University,
                                                                                      //     Edinburgh EH10 5DT.
}


void TRiaF2DPhytoplankton::Loads()
{
    int index, ADay, LineIndex, ColumnIndex, LineNumber, ColumnNumber, BoxNumber;
    double Depth;
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();

    if (JulianDay < NumberOfDaysForLoads)
        ADay  = JulianDay;
    else
        ADay = 1;
    for (int j = 0; j < NumberOfLoads; j++)
    {
        index = ALoadRecord[j].LineCoordinate * GridColumns
                + ALoadRecord[j].ColumnCoordinate;
        /*LineNumber = ALoadRecord[j].LineCoordinate;
        ColumnNumber = ALoadRecord[j].ColumnCoordinate;
        BoxNumber = GetBoxNumber(ColumnNumber,LineNumber);
        LineIndex = GetLineIndex(BoxNumber);
        ColumnIndex = GetColumnIndex(BoxNumber);
        index = LineIndex * GridColumns + ColumnIndex; */
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TRiaF2DPhytoplankton::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaF2DPhytoplankton::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        //if (BoxArray[index].Dummybool2)
        if (Depth > 0.0)
        {
            PhytoLoad[index] = ALoadRecord[j].PhytoplanktonCLoad[ADay-1];
            PhytoNLoad[index] = ALoadRecord[j].PhytoplanktonNLoad[ADay-1];
            PhytoPLoad[index] = ALoadRecord[j].PhytoplanktonPLoad[ADay-1];
        }
    }
    for (int j = 0; j < NumberOfRiverLoads; j++)
    {
        index = ALoadRecord[j].LineCoordinate * GridColumns
                + ALoadRecord[j].ColumnCoordinate;
        /*LineNumber = ALoadRiverRecord[j].LineCoordinate;
        ColumnNumber = ALoadRiverRecord[j].ColumnCoordinate;
        BoxNumber = GetBoxNumber(ColumnNumber,LineNumber);
        LineIndex = GetLineIndex(BoxNumber);
        ColumnIndex = GetColumnIndex(BoxNumber);
        index = LineIndex * GridColumns + ColumnIndex;*/
        if (index >= NumberOfBoxes) {
#ifdef __BORLANDC__
            char Caption[129];
            sprintf(Caption, "Index %d > Number of Boxes (%d)\n\n",
                    index, NumberOfBoxes);
            strcat(Caption, "Please correct Configuration Files");
            MessageBox(0,
                      Caption,
                      "EcoDynamo Alert - TRiaF2DPhytoplankton::Loads()",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TRiaF2DPhytoplankton::Loads - Index " << index << " > Number of Boxes (" << NumberOfBoxes << "). Please correct Configuration Files" << endl;
#endif  // __BORLANDC__
            return;
        }
        if (MyTransportPointer != NULL)
        {
            MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                         index,
                                         "Box depth",
                                         ObjectCode);
        }
        else Depth = MyPEcoDynClass->GetBoxDepth(index);
        //if (Depth > CriticalDepthForLandBoundary)
        if (Depth > 0.0)
        {
            /*PhytoPlanktonCRiverLoad[index] = ALoadRiverRecord[j].PhytoplanktonCRiverLoad[ADay-1];
            PhytoPlanktonNRiverLoad[index] = ALoadRiverRecord[j].PhytoplanktonNRiverLoad[ADay-1];
            PhytoPlanktonPRiverLoad[index] = ALoadRiverRecord[j].PhytoplanktonPRiverLoad[ADay-1];  */
            PhytoBiomass[index] = ALoadRiverRecord[j].PhytoplanktonCRiverLoad[ADay-1];
            NPhyto[index] = ALoadRiverRecord[j].PhytoplanktonNRiverLoad[ADay-1];
            PPhyto[index] = ALoadRiverRecord[j].PhytoplanktonPRiverLoad[ADay-1];
        }
    }
}


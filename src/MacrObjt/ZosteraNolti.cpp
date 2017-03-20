// Zostera noltii object CPP code file
//According to Plus et al (2003). Ecol. Modell. 161: 213-238

/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <vcl.h>
	#include <math.h>
	#include <stdlib.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cmath>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__

#include "ecodyn.rh"
#include "macrobjt.h"
#include "iodll.h"

TZosteraNoltii::TZosteraNoltii(TEcoDynClass* APEcoDynClass, char* className): TUlvaSp(APEcoDynClass, className)
{
        NumberOfBoxesWithZosteraNoltii = 0;
        CarbonToDryWeight = 2.0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
                if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Zostera noltii") == 0)
                        NumberOfBoxesWithZosteraNoltii++;

        }
        ZosteraNoltiiRecord = new BenthicSpeciesRecord[NumberOfBoxesWithZosteraNoltii];
        int AIndex = 0;
        for (int i = 0; i < NumberOfBoxesWithBenthicSpecies; i++)
        {
                if (strcmp(MyBenthicSpeciesRecord[i].SpeciesName, "Zostera noltii") == 0)
                {
                        strcpy(ZosteraNoltiiRecord[AIndex].SpeciesName, MyBenthicSpeciesRecord[i].SpeciesName);
                        ZosteraNoltiiRecord[AIndex].LineCoordinate = MyBenthicSpeciesRecord[i].LineCoordinate;
                        ZosteraNoltiiRecord[AIndex].ColumnCoordinate = MyBenthicSpeciesRecord[i].ColumnCoordinate;
                        AIndex++;
                }
        }
        PhytoRootBiomass = new double[NumberOfBoxes]; PhytoFluxRoot = new double[NumberOfBoxes];
        CCellQuotaRoot = new double[NumberOfBoxes]; NCellQuotaRoot = new double[NumberOfBoxes];
        PCellQuotaRoot = new double[NumberOfBoxes];
        NUptakeRoot = new double[NumberOfBoxes]; PUptakeRoot = new double[NumberOfBoxes];
        CPhytoRoot = new double[NumberOfBoxes]; NPhytoRoot = new double[NumberOfBoxes];
        PPhytoRoot = new double[NumberOfBoxes];
        NCellFluxRoot = new double[NumberOfBoxes]; PCellFluxRoot = new double[NumberOfBoxes]; LAI = new double[NumberOfBoxes];
        LeafResp = new double[NumberOfBoxes]; RootResp = new double[NumberOfBoxes];
        LeafNSaturation = new double[NumberOfBoxes]; RootNSaturation = new double[NumberOfBoxes];
        LeafNReclamation = new double[NumberOfBoxes]; RootNReclamation = new double[NumberOfBoxes];
        TotalGrowth = new double[NumberOfBoxes]; RootGrowth = new double[NumberOfBoxes]; LeafGrowth = new double[NumberOfBoxes];
        Recruitment = new double[NumberOfBoxes]; ShootDensity = new double[NumberOfBoxes]; ShootDensityFlux = new double[NumberOfBoxes];
        TPMProduction = new double[NumberOfBoxes]; POMProduction = new double[NumberOfBoxes];
        /*MaxPCellQuota = 0.0; MaxNCellQuota = 0.0; MinPCellQuota = 0.0; MinNCellQuota = 0.0;
        OxygenToCarbonProd = 1.2; OxygenToCarbonResp = 1.0;
        VNmax = 0.0; VPmax = 0.0; KN = 0.0; KP = 0.0;  */
        for (int i = 0; i < NumberOfBoxes; i++)
        {
           PhytoBiomass[i] = 0.0;
           PhytoRootBiomass[i] = 0.0; PhytoFluxRoot[i] = 0.0;
           CCellQuotaRoot[i] = 0.0; NCellQuotaRoot[i] = 0.0; PCellQuotaRoot[i] = 0.0;
           NUptakeRoot[i] = 0.0; PUptakeRoot[i] = 0.0;
           CPhytoRoot[i] = 0.0; NPhytoRoot[i] = 0.0; PPhytoRoot[i] = 0.0;
           NCellFluxRoot[i] = 0.0; PCellFluxRoot[i] = 0.0; LAI[i] = 0.0;
           LeafResp[i] = 0.0; RootResp[i] = 0.0;
           LeafNSaturation[i] = 0.0; RootNSaturation[i] = 0.0;
           LeafNReclamation[i] = 0.0; RootNReclamation[i] = 0.0;
           TotalGrowth[i] = 0.0; RootGrowth[i] = 0.0; LeafGrowth[i] = 0.0;
           Recruitment[i] = 0.0; ShootDensity[i] = 0.0; ShootDensityFlux[i] = 0.0;
           TPMProduction[i] = 0.0; POMProduction[i] = 0.0;
        }

    int X,Y, XV, YV;
    VariableCount = 0;
	// Read in the variables
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("ZosteraNoltii");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "ZosteraNoltii variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TZosteraNoltii::TZosteraNoltii - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        if (PReadWrite->FindString("ZosteraNoltii", X, Y))
        {
            PReadWrite->ReadNumber(X + NUMBEROFVARSCOLUMN, Y, NumberOfVariables);

            // Create array for state variable names
            VariableNameArray = new VNA[NumberOfVariables];
            // Create array for state variable units
            //UnitNameArray = new UNA[NumberOfVariables];
            // read in the variable names
            // for (int i = Y; i < Y + NumberOfVariables; i++)
    /*        for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN + i, Y, VariableNameArray[i]);
                //PReadWrite->ReadString(X + UNITSCOLUMN + i, Y, UnitNameArray[i]);
            }*/
            char MyVariable[65];
            int Index;
            //for (int i = Y; i < Y + NumberOfVariables; i++)
            for (int i = 0; i < NumberOfVariables; i++)
            {
                PReadWrite->ReadString(X + VARIABLENAMECOLUMN + i, Y, MyVariable);
                strcpy(VariableNameArray[i], MyVariable);

                if (strcmp(MyVariable, "ZosteraNoltiiAboveGroundBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiAboveGroundBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, PhytoBiomass[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(PhytoBiomass[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiAboveGroundCBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiAboveGroundCBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, CPhyto[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(CPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiAboveGroundNBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiAboveGroundNBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, NPhyto[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(NPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiAboveGroundPBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiAboveGroundPBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, PPhyto[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(PPhyto[Index]);
                    }
                }
                else if (strcmp(MyVariable, "ZosteraNoltiiBelowGroundBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiBelowGroundBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, PhytoRootBiomass[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(PhytoBiomass[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiBelowGroundCBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiBelowGroundCBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, CPhytoRoot[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(CPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiBelowGroundNBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiBelowGroundNBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, NPhytoRoot[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(NPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiBelowGroundPBiomass") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiBelowGroundPBiomass values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, PPhytoRoot[Index]);
                            //if (j == NumberOfBoxesWithZosteraNoltii - 1) Debugger(PPhyto[Index]);
                    }
                }
                else
                if (strcmp(MyVariable, "ZosteraNoltiiShootDensity") == 0)
                {
                    if (!PReadWrite->FindString("ZosteraNoltiiShootDensity values", XV, YV))
                    {
                        XV = X + VARIABLENAMECOLUMN + i;
                        YV = Y;
                    }
                    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
                    {
                            Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                                    ZosteraNoltiiRecord[j].ColumnCoordinate;
                            PReadWrite->ReadNumber(XV, j + YV + 1, ShootDensity[Index]);
                            /*if (j == NumberOfBoxesWithZosteraNoltii - 1)
                            {
                               Debugger(NumberOfBoxesWithZosteraNoltii); Debugger(Index);
                               Debugger(ShootDensity[Index]);
                            } */
                    }
                }
            //More variables here
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object variables - ZosteraNoltii", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TZosteraNoltii::TZosteraNoltii - Variables: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

	ParameterCount = 0;
	// Read in the parameters
//	PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("ZosteraNoltii");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "ZosteraNoltii parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TZosteraNoltii::TZosteraNoltii - Parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        double MyValue;

        if (PReadWrite->FindString("ZosteraNoltii", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);
    //
    // Some parameters initialized in TUlvaSp
    //
                if (strcmp(MyParameter, "TetaPmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TetaPmax);
                                    //Debugger(TetaPmax);
                }
                else
                if (strcmp(MyParameter, "Pmax0") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Pmax0);
                                    //Debugger(Pmax0);
                }
                else
                if (strcmp(MyParameter, "KLAI") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KLAI);
                                    //Debugger(KLAI);
                }
                            else
                if (strcmp(MyParameter, "K3") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, K3);
                                    //Debugger(K3);
                }
                else
                if (strcmp(MyParameter, "Ikmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Ikmax);
                        //Debugger(Ikmax);
                }
                else
                if (strcmp(MyParameter, "Ikmin") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Ikmin);
                        //Debugger(Ikmin);
                }
                else
                if (strcmp(MyParameter, "TetaLR") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TetaLR);
                        //Debugger(TetaLR);
                }
                else
                if (strcmp(MyParameter, "LR0") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, LR0);
                        //Debugger(LR0);
                }
                else
                if (strcmp(MyParameter, "TetaRR") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TetaRR);
                        //Debugger(TetaRR);
                }
                else
                if (strcmp(MyParameter, "RR0") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RR0);
                        //Debugger(RR0);
                }
                else
                if (strcmp(MyParameter, "ER20") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, ER20);
                        //Debugger(ER20);
                }
                else
                if (strcmp(MyParameter, "PQ") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, PQ);
                        //Debugger(PQ);
                }
                else
                if (strcmp(MyParameter, "E1") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, E1);
                        //Debugger(E1);
                }
                else
                if (strcmp(MyParameter, "MinNCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinNCellQuota );
                        //Debugger(MinNCellQuota);
                }
                else
                if (strcmp(MyParameter, "MaxNCellQuota") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxNCellQuota);
                        //Debugger(MaxNCellQuota);
                }
                else
                if (strcmp(MyParameter, "K") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, K);
                        //Debugger(K);
                }
                else
                if (strcmp(MyParameter, "E2") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, E2);
                        //Debugger(E2);
                }
                else
                if (strcmp(MyParameter, "MinNCellQuotaRoot") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MinNCellQuotaRoot);
                        //Debugger(MinNCellQuotaRoot);
                }
                else
                if (strcmp(MyParameter, "MaxNCellQuotaRoot") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MaxNCellQuotaRoot);
                        //Debugger(MaxNCellQuotaRoot);
                }
                else
                if (strcmp(MyParameter, "VNmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, VNmax);
                        //Debugger(VNmax);
                }
                else
                if (strcmp(MyParameter, "KN") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KN);
                        //Debugger(KN);
                }
                else
                if (strcmp(MyParameter, "Sigma1") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Sigma1);
                        //Debugger(Sigma1);
                }
                else
                if (strcmp(MyParameter, "VNmaxRoot") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, VNmaxRoot);
                        //Debugger(VNmaxRoot);
                }
                else
                if (strcmp(MyParameter, "KNroot") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, KNroot);
                        //Debugger(KNroot);
                }
                else
                if (strcmp(MyParameter, "Sigma2") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Sigma2);
                        //Debugger(Sigma2);
                }
                else
                if (strcmp(MyParameter, "Tau") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Tau);
                        //Debugger(Tau);
                }
                else
                if (strcmp(MyParameter, "Srec") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Srec);
                        //Debugger(Srec);
                }
                else
                if (strcmp(MyParameter, "RECmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RECmax);
                        //Debugger(RECmax);
                }
                else
                if (strcmp(MyParameter, "LMR20") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, LMR20);
                        //Debugger(LMR20);
                }
                else
                if (strcmp(MyParameter, "TetaLM") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TetaLM);
                        //Debugger(TetaLM);
                }
                else
                if (strcmp(MyParameter, "LMRv") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, LMRv);
                        //Debugger(LMRv);
                }
                else
                if (strcmp(MyParameter, "K4") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, K4);
                        //Debugger(K4);
                }
                else
                if (strcmp(MyParameter, "RMR20") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RMR20);
                        //Debugger(RMR20);
                }
                else
                if (strcmp(MyParameter, "RECruitmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, RECruitmax);
                        //Debugger(RECruitmax);
                }
                else
                if (strcmp(MyParameter, "TetaRec") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, TetaRec);
                        //Debugger(TetaRec);
                }
                else
                if (strcmp(MyParameter, "SB0") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SB0);
                        //Debugger(SB0);
                }
                else
                if (strcmp(MyParameter, "Krec1") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Krec1);
                        //Debugger(Krec1);
                }
                else
                if (strcmp(MyParameter, "Krec2") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, Krec2);
                        //Debugger(Krec2);
                }
                else
                if (strcmp(MyParameter, "SB0") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, SB0);
                        //Debugger(Krec2);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Undefined object parameters - ZosteraNoltii", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TZosteraNoltii::TZosteraNoltii - Parameters: undefined object" << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

        int Index;
        for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
        {
                   Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                           ZosteraNoltiiRecord[j].ColumnCoordinate;

                   if (PhytoBiomass[Index] != 0)
                   {
                       CCellQuota[Index] = CPhyto[Index] / PhytoBiomass[Index];   //g / g
                       NCellQuota[Index] = NPhyto[Index] / PhytoBiomass[Index];
                       PCellQuota[Index] = PPhyto[Index] / PhytoBiomass[Index];
                   }
                   if (PhytoRootBiomass[Index] != 0)
                   {
                       CCellQuotaRoot[Index] = CPhytoRoot[Index] / PhytoRootBiomass[Index];   //g / g
                       NCellQuotaRoot[Index] = NPhytoRoot[Index] / PhytoRootBiomass[Index];
                       PCellQuotaRoot[Index] = PPhytoRoot[Index] / PhytoRootBiomass[Index];
                   }
                   CPhyto[Index] = CPhyto[Index] * CUBIC / CARBONATOMICWEIGHT;    //conversão de g / m2 para mmol / m2 para compatibilidade com o paper do Martin
                   NPhyto[Index] = NPhyto[Index] * CUBIC / NITROGENATOMICWEIGHT;  //idem
                   PPhyto[Index] = PPhyto[Index] * CUBIC / PHOSPHORUSATOMICWEIGHT; //idem
                   CPhytoRoot[Index] = CPhytoRoot[Index] * CUBIC / CARBONATOMICWEIGHT; //idem
                   NPhytoRoot[Index] = NPhytoRoot[Index] * CUBIC / NITROGENATOMICWEIGHT; //idem
                   PPhytoRoot[Index] = PPhytoRoot[Index] * CUBIC / PHOSPHORUSATOMICWEIGHT; //idem

                   LAI[Index] = CalculateLeafAreaIndex(Index);

        }
}

TZosteraNoltii::~TZosteraNoltii()
{
        freeMemory();
}

void TZosteraNoltii::freeMemory()
{
    try {
       if (NumberOfBoxes > 0)
       {
          delete [] PhytoRootBiomass;
          delete [] PhytoFluxRoot;
          delete [] CCellQuotaRoot;
          delete [] NCellQuotaRoot;
          delete [] PCellQuotaRoot;
          delete [] NUptakeRoot;
          delete [] PUptakeRoot;
          delete [] CPhytoRoot;
          delete [] NPhytoRoot;
          delete [] PPhytoRoot;
          delete [] NCellFluxRoot;
          delete [] PCellFluxRoot;
          delete [] LeafResp;
          delete [] RootResp;
          delete [] LeafNSaturation;
          delete [] RootNSaturation;
          delete [] LeafNReclamation;
          delete [] RootNReclamation;
          delete [] TotalGrowth;
          delete [] RootGrowth;
          delete [] LeafGrowth;
          delete [] Recruitment;
          delete [] ShootDensity;
          delete [] ShootDensityFlux;
          delete [] TPMProduction;
          delete [] POMProduction;
       }
       if (NumberOfBoxesWithZosteraNoltii > 0)
          delete [] ZosteraNoltiiRecord;
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
				cerr << "TZosteraNoltii::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TZosteraNoltii::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "TetaPmax") == 0)
    {
        value = TetaPmax;
    }
    else if (strcmp(MyParameter, "Pmax0") == 0)
    {
        value = Pmax0;
    }
    else if (strcmp(MyParameter, "KLAI") == 0)
    {
        value = KLAI;
    }
    else if (strcmp(MyParameter, "K3") == 0)
    {
        value = K3;
    }
    else if (strcmp(MyParameter, "Ikmax") == 0)
    {
        value = Ikmax;
    }
    else if (strcmp(MyParameter, "Ikmin") == 0)
    {
        value = Ikmin;
    }
    else if (strcmp(MyParameter, "TetaLR") == 0)
    {
        value = TetaLR;
    }
    else if (strcmp(MyParameter, "LR0") == 0)
    {
        value = LR0;
    }
    else if (strcmp(MyParameter, "TetaRR") == 0)
    {
        value = TetaRR;
    }
    else if (strcmp(MyParameter, "RR0") == 0)
    {
        value = RR0;
    }
    else if (strcmp(MyParameter, "ER20") == 0)
    {
        value = ER20;
    }
    else if (strcmp(MyParameter, "PQ") == 0)
    {
        value = PQ;
    }
    else if (strcmp(MyParameter, "E1") == 0)
    {
        value = E1;
    }
    else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
    {
        value = MaxNCellQuota;
    }
    else if (strcmp(MyParameter, "MinNCellQuota") == 0)
    {
        value = MinNCellQuota;
    }
    else if (strcmp(MyParameter, "K") == 0)
    {
        value = K;
    }
    else if (strcmp(MyParameter, "E2") == 0)
    {
        value = E2;
    }
    else if (strcmp(MyParameter, "MinNCellQuotaRoot") == 0)
    {
        value = MinNCellQuotaRoot;
    }
    else if (strcmp(MyParameter, "MaxNCellQuotaRoot") == 0)
    {
        value = MaxNCellQuotaRoot;
    }
    else if (strcmp(MyParameter, "VNmax") == 0)
    {
        value = VNmax;
    }
    else if (strcmp(MyParameter, "KN") == 0)
    {
        value = KN;
    }
    else if (strcmp(MyParameter, "Sigma1") == 0)
    {
        value = Sigma1;
    }
    else if (strcmp(MyParameter, "VNmaxRoot") == 0)
    {
        value = VNmaxRoot;
    }
    else if (strcmp(MyParameter, "KNroot") == 0)
    {
        value = KNroot;
    }
    else if (strcmp(MyParameter, "Sigma2") == 0)
    {
        value = Sigma2;
    }
    else if (strcmp(MyParameter, "Tau") == 0)
    {
        value = Tau;
    }
    else if (strcmp(MyParameter, "Srec") == 0)
    {
        value = Srec;
    }
    else if (strcmp(MyParameter, "RECmax") == 0)
    {
        value = RECmax;
    }
    else if (strcmp(MyParameter, "LMR20") == 0)
    {
        value = LMR20;
    }
    else if (strcmp(MyParameter, "TetaLM") == 0)
    {
        value = TetaLM;
    }
    else if (strcmp(MyParameter, "LMRv") == 0)
    {
        value = LMRv;
    }
    else if (strcmp(MyParameter, "K4") == 0)
    {
        value = K4;
    }
    else if (strcmp(MyParameter, "RMR20") == 0)
    {
        value = RMR20;
    }
    else if (strcmp(MyParameter, "RECruitmax") == 0)
    {
        value = RECruitmax;
    }
    else if (strcmp(MyParameter, "TetaRec") == 0)
    {
        value = TetaRec;
    }
    else if (strcmp(MyParameter, "SB0") == 0)
    {
        value = SB0;
    }
    else if (strcmp(MyParameter, "Krec1") == 0)
    {
        value = Krec1;
    }
    else if (strcmp(MyParameter, "Krec2") == 0)
    {
        value = Krec2;
    }
    else if (strcmp(MyParameter, "SB0") == 0)
    {
        value = SB0;
    }
    else
        value = 0.0;
    return value;
}

bool TZosteraNoltii::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

    if (strcmp(MyParameter, "TetaPmax") == 0)
    {
        TetaPmax = value;
    }
    else if (strcmp(MyParameter, "Pmax0") == 0)
    {
        Pmax0 = value;
    }
    else if (strcmp(MyParameter, "KLAI") == 0)
    {
        KLAI = value;
    }
    else if (strcmp(MyParameter, "K3") == 0)
    {
        K3 = value;
    }
    else if (strcmp(MyParameter, "Ikmax") == 0)
    {
        Ikmax = value;
    }
    else if (strcmp(MyParameter, "Ikmin") == 0)
    {
        Ikmin = value;
    }
    else if (strcmp(MyParameter, "TetaLR") == 0)
    {
        TetaLR = value;
    }
    else if (strcmp(MyParameter, "LR0") == 0)
    {
        LR0 = value;
    }
    else if (strcmp(MyParameter, "TetaRR") == 0)
    {
        TetaRR = value;
    }
    else if (strcmp(MyParameter, "RR0") == 0)
    {
        RR0 = value;
    }
    else if (strcmp(MyParameter, "ER20") == 0)
    {
        ER20 = value;
    }
    else if (strcmp(MyParameter, "PQ") == 0)
    {
        PQ = value;
    }
    else if (strcmp(MyParameter, "E1") == 0)
    {
        E1 = value;
    }
    else if (strcmp(MyParameter, "MaxNCellQuota") == 0)
    {
        MaxNCellQuota = value;
    }
    else if (strcmp(MyParameter, "MinNCellQuota") == 0)
    {
        MinNCellQuota = value;
    }
    else if (strcmp(MyParameter, "K") == 0)
    {
        K = value;
    }
    else if (strcmp(MyParameter, "E2") == 0)
    {
        E2 = value;
    }
    else if (strcmp(MyParameter, "MinNCellQuotaRoot") == 0)
    {
        MinNCellQuotaRoot = value;
    }
    else if (strcmp(MyParameter, "MaxNCellQuotaRoot") == 0)
    {
        MaxNCellQuotaRoot = value;
    }
    else if (strcmp(MyParameter, "VNmax") == 0)
    {
        VNmax = value;
    }
    else if (strcmp(MyParameter, "KN") == 0)
    {
        KN = value;
    }
    else if (strcmp(MyParameter, "Sigma1") == 0)
    {
        Sigma1 = value;
    }
    else if (strcmp(MyParameter, "VNmaxRoot") == 0)
    {
        VNmaxRoot = value;
    }
    else if (strcmp(MyParameter, "KNroot") == 0)
    {
        KNroot = value;
    }
    else if (strcmp(MyParameter, "Sigma2") == 0)
    {
        Sigma2 = value;
    }
    else if (strcmp(MyParameter, "Tau") == 0)
    {
        Tau = value;
    }
    else if (strcmp(MyParameter, "Srec") == 0)
    {
        Srec = value;
    }
    else if (strcmp(MyParameter, "RECmax") == 0)
    {
        RECmax = value;
    }
    else if (strcmp(MyParameter, "LMR20") == 0)
    {
        LMR20 = value;
    }
    else if (strcmp(MyParameter, "TetaLM") == 0)
    {
        TetaLM = value;
    }
    else if (strcmp(MyParameter, "LMRv") == 0)
    {
        LMRv = value;
    }
    else if (strcmp(MyParameter, "K4") == 0)
    {
        K4 = value;
    }
    else if (strcmp(MyParameter, "RMR20") == 0)
    {
        RMR20 = value;
    }
    else if (strcmp(MyParameter, "RECruitmax") == 0)
    {
        RECruitmax = value;
    }
    else if (strcmp(MyParameter, "TetaRec") == 0)
    {
        TetaRec = value;
    }
    else if (strcmp(MyParameter, "SB0") == 0)
    {
        SB0 = value;
    }
    else if (strcmp(MyParameter, "Krec1") == 0)
    {
        Krec1 = value;
    }
    else if (strcmp(MyParameter, "Krec2") == 0)
    {
        Krec2 = value;
    }
    else if (strcmp(MyParameter, "SB0") == 0)
    {
        SB0 = value;
    }
    else
        rc = false;

    return rc;
}

void TZosteraNoltii::Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName, int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber, JulianDay;
	strcpy(MyParameter, ParameterName);

   	if (strcmp(MyParameter, "ZosteraNoltiiAboveGroundBiomass") == 0) //g DW / m2
    {
            //Debugger(PhytoBiomass[MyBoxNumber]);
        Value = PhytoBiomass[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "ZosteraNoltiiAboveGroundCBiomass") == 0)
        Value = CPhyto[MyBoxNumber] / CUBIC * CARBONATOMICWEIGHT;       //g C / m2
    else if (strcmp(MyParameter, "ZosteraNoltiiAboveGroundNBiomass") == 0)
        Value = NPhyto[MyBoxNumber] / CUBIC * NITROGENATOMICWEIGHT;     //g N / m2
    else if (strcmp(MyParameter, "ZosteraNoltiiAboveGroundPBiomass") == 0)
        Value = PPhyto[MyBoxNumber] / CUBIC * PHOSPHORUSATOMICWEIGHT;   //g P / m2
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowGroundBiomass") == 0)
    {
            //Debugger(PhytoBiomass[MyBoxNumber]);
        Value = PhytoRootBiomass[MyBoxNumber];                              //g DW / m2
    }
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowGroundCBiomass") == 0)
        Value = CPhytoRoot[MyBoxNumber] / CUBIC * CARBONATOMICWEIGHT;   //g C / m2
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowGroundNBiomass") == 0)
        Value = NPhytoRoot[MyBoxNumber] / CUBIC * NITROGENATOMICWEIGHT; //g N / m2
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowGroundPBiomass") == 0)
        Value = PPhytoRoot[MyBoxNumber] / CUBIC * PHOSPHORUSATOMICWEIGHT; //g P / m2
    else if (strcmp(MyParameter, "ZosteraNoltiiLeafAreaIndex") == 0)
        Value = LAI[MyBoxNumber];   //m2/m2
    else if (strcmp(MyParameter, "ZosteraNoltiiIK") == 0)
        Value = IK; //W/m2
    else if (strcmp(MyParameter, "ZosteraNoltiiLeafRespiration") == 0)
    {
        if (CPhyto[MyBoxNumber] > 0.0)
            Value = LeafResp[MyBoxNumber] * DAYSTOSECONDS / CPhyto[MyBoxNumber];        //g O2 d-1 mmolC-1
        else
            Value = 0.0;
    }
    else if (strcmp(MyParameter, "ZosteraNoltiiRootRespiration") == 0)
    {
        if (CPhyto[MyBoxNumber] > 0.0)
            Value = RootResp[MyBoxNumber] * DAYSTOSECONDS / CPhytoRoot[MyBoxNumber];        //g O2 d-1 mmolC-1
        else
            Value = 0.0;
    }
    else if (strcmp(MyParameter, "ZosteraNoltiiTotalGrowth") == 0)
        Value = TotalGrowth[MyBoxNumber] / CUBIC * CARBONATOMICWEIGHT * DAYSTOSECONDS;   //g C / m2 /d
    else if (strcmp(MyParameter, "ZosteraNoltiiRootGrowth") == 0)
        Value = RootGrowth[MyBoxNumber] / CUBIC * CARBONATOMICWEIGHT * DAYSTOSECONDS;   //g C / m2 /d
    else if (strcmp(MyParameter, "ZosteraNoltiiLeafGrowth") == 0)
        Value = LeafGrowth[MyBoxNumber] / CUBIC * CARBONATOMICWEIGHT * DAYSTOSECONDS;   //g C / m2  /d
    else if (strcmp(MyParameter, "ZosteraNoltiiRecruitment") == 0)      //1/d
        Value = Recruitment[MyBoxNumber] * DAYSTOSECONDS;

    else if (strcmp(MyParameter, "ZosteraNoltii GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] == 0)
            Value = 0;
        else
            Value = GPP[MyBoxNumber] * DAYSTOSECONDS;  //g O2 mmolC-1 d-1
    }
    else if (strcmp(MyParameter, "ZosteraNoltiiNCellQuota") == 0)
        Value = NCellQuota[MyBoxNumber];
    else if (strcmp(MyParameter, "ZosteraNoltiiPCellQuota") == 0)
        Value = PCellQuota[MyBoxNumber];
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowGroundNCellQuota") == 0)
        Value = NCellQuotaRoot[MyBoxNumber];
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowGroundPCellQuota") == 0)
        Value = PCellQuotaRoot[MyBoxNumber];
    else if (strcmp(MyParameter, "ZosteraNoltiiShootDensity") == 0)
        Value = ShootDensity[MyBoxNumber];
    else if (strcmp(MyParameter, "ZosteraNoltiiTemperatureLimitationOfRecruitment") == 0)
        Value = TemperatureLimitationOfRecruitment(MyBoxNumber);
    else if (strcmp(MyParameter, "ZosteraNoltiiSelfShadingLimitation") == 0)
        Value = SelfShadingLimitation(MyBoxNumber);
    else if (strcmp(MyParameter, "ZosteraNoltiiBelowgroundLimitation") == 0)
        Value = BelowgroundLimitation(MyBoxNumber);
    else if (strcmp(MyParameter, "ZosteraNoltiiTPMProduction") == 0)
        Value = TPMProduction[MyBoxNumber];
    else if (strcmp(MyParameter, "ZosteraNoltiiPOMProduction") == 0)
        Value = POMProduction[MyBoxNumber];

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
            strcat(MyParameter, " does not exist in the macrophyte object");
            MessageBox(0,
                  Caption,
                  "EcoDynamo Alert",
                  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TZosteraNoltii::Inquiry - " << MyParameter << " does not exist in macrophyte object" << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

bool TZosteraNoltii::SetVariableValue(char* srcName, double Value,
									      int BoxNumber,
									     char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

   	if (strcmp(VariableName, "ZosteraNoltiiAboveGroundBiomass") == 0)
        PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiAboveGroundCBiomass") == 0)
        CPhyto[BoxNumber] = Value * CUBIC / CARBONATOMICWEIGHT;
    else if (strcmp(VariableName, "ZosteraNoltiiAboveGroundNBiomass") == 0)
        NPhyto[BoxNumber] = Value * CUBIC / NITROGENATOMICWEIGHT;
    else if (strcmp(VariableName, "ZosteraNoltiiAboveGroundPBiomass") == 0)
        PPhyto[BoxNumber] = Value * CUBIC / PHOSPHORUSATOMICWEIGHT;
    else if (strcmp(VariableName, "ZosteraNoltiiBelowGroundBiomass") == 0)
        PhytoRootBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiBelowGroundCBiomass") == 0)
        CPhytoRoot[BoxNumber] = Value * CUBIC / CARBONATOMICWEIGHT;
    else if (strcmp(VariableName, "ZosteraNoltiiBelowGroundNBiomass") == 0)
        NPhytoRoot[BoxNumber] = Value * CUBIC / NITROGENATOMICWEIGHT;
    else if (strcmp(VariableName, "ZosteraNoltiiBelowGroundPBiomass") == 0)
        PPhytoRoot[BoxNumber] = Value * CUBIC / PHOSPHORUSATOMICWEIGHT;
    else if (strcmp(VariableName, "ZosteraNoltiiLeafAreaIndex") == 0)
        LAI[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiIK") == 0)
        IK = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiTotalGrowth") == 0)
        TotalGrowth[BoxNumber] = Value * CUBIC / CARBONATOMICWEIGHT / DAYSTOSECONDS;
    else if (strcmp(VariableName, "ZosteraNoltiiRootGrowth") == 0)
        RootGrowth[BoxNumber] = Value * CUBIC / CARBONATOMICWEIGHT / DAYSTOSECONDS;
    else if (strcmp(VariableName, "ZosteraNoltiiLeafGrowth") == 0)
        LeafGrowth[BoxNumber] = Value * CUBIC / CARBONATOMICWEIGHT / DAYSTOSECONDS;
    else if (strcmp(VariableName, "ZosteraNoltiiRecruitment") == 0)
        Recruitment[BoxNumber] = Value / DAYSTOSECONDS;
    else if (strcmp(VariableName, "ZosteraNoltii GPP per unit mass") == 0)
        GPP[BoxNumber] = Value / DAYSTOSECONDS;  //g O2 mmolC-1 d-1
    else if (strcmp(VariableName, "ZosteraNoltiiNCellQuota") == 0)
        NCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiPCellQuota") == 0)
        PCellQuota[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiBelowGroundNCellQuota") == 0)
        NCellQuotaRoot[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiBelowGroundPCellQuota") == 0)
        PCellQuotaRoot[BoxNumber];
    else if (strcmp(VariableName, "ZosteraNoltiiShootDensity") == 0)
        ShootDensity[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiTPMProduction") == 0)
        TPMProduction[BoxNumber] = Value;
    else if (strcmp(VariableName, "ZosteraNoltiiPOMProduction") == 0)
        POMProduction[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

// AP, 2007.07.13
bool TZosteraNoltii::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("ZosteraNoltii");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("ZosteraNoltii");
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
     * Zostera noltii variables initial values
     */
    PReadWrite->WriteCell("ZosteraNoltiiAboveGroundBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiAboveGroundCBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiAboveGroundNBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiAboveGroundPBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiBelowGroundBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiBelowGroundCBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiBelowGroundNBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiBelowGroundPBiomass values");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell("ZosteraNoltiiShootDensity values");
    PReadWrite->WriteSeparator(true);
    int Index;
    for (int j = 0; j < NumberOfBoxesWithZosteraNoltii; j++)
    {
        Index = ZosteraNoltiiRecord[j].LineCoordinate * NumberOfColumns +
                ZosteraNoltiiRecord[j].ColumnCoordinate;
        PReadWrite->WriteCell(PhytoBiomass[Index], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(CPhyto[Index] / CUBIC * CARBONATOMICWEIGHT, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NPhyto[Index] / CUBIC * NITROGENATOMICWEIGHT, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PPhyto[Index] / CUBIC * PHOSPHORUSATOMICWEIGHT, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PhytoRootBiomass[Index], 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(CPhytoRoot[Index] / CUBIC * CARBONATOMICWEIGHT, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(NPhytoRoot[Index] / CUBIC * NITROGENATOMICWEIGHT, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(PPhytoRoot[Index] / CUBIC * PHOSPHORUSATOMICWEIGHT, 8);
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ShootDensity[Index], 8);
        PReadWrite->WriteSeparator(true);
    }

    CloseDataFile((void*)PReadWrite);
    return true;
}

bool TZosteraNoltii::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("ZosteraNoltii");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("ZosteraNoltii");
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

        if (strcmp(ParametersNameArray[i], "TetaPmax") == 0)
        {
            PReadWrite->WriteCell(TetaPmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Pmax0") == 0)
        {
            PReadWrite->WriteCell(Pmax0, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KLAI") == 0)
        {
            PReadWrite->WriteCell(KLAI, 8);
        }
        else if (strcmp(ParametersNameArray[i], "K3") == 0)
        {
            PReadWrite->WriteCell(K3, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Ikmax") == 0)
        {
            PReadWrite->WriteCell(Ikmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Ikmin") == 0)
        {
            PReadWrite->WriteCell(Ikmin, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TetaLR") == 0)
        {
            PReadWrite->WriteCell(TetaLR, 8);
        }
        else if (strcmp(ParametersNameArray[i], "LR0") == 0)
        {
            PReadWrite->WriteCell(LR0, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TetaRR") == 0)
        {
            PReadWrite->WriteCell(TetaRR, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RR0") == 0)
        {
            PReadWrite->WriteCell(RR0, 8);
        }
        else if (strcmp(ParametersNameArray[i], "ER20") == 0)
        {
            PReadWrite->WriteCell(ER20, 8);
        }
        else if (strcmp(ParametersNameArray[i], "PQ") == 0)
        {
            PReadWrite->WriteCell(PQ, 8);
        }
        else if (strcmp(ParametersNameArray[i], "E1") == 0)
        {
            PReadWrite->WriteCell(E1, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxNCellQuota") == 0)
        {
            PReadWrite->WriteCell(MaxNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinNCellQuota") == 0)
        {
            PReadWrite->WriteCell(MinNCellQuota, 8);
        }
        else if (strcmp(ParametersNameArray[i], "K") == 0)
        {
            PReadWrite->WriteCell(K, 8);
        }
        else if (strcmp(ParametersNameArray[i], "E2") == 0)
        {
            PReadWrite->WriteCell(E2, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MinNCellQuotaRoot") == 0)
        {
            PReadWrite->WriteCell(MinNCellQuotaRoot, 8);
        }
        else if (strcmp(ParametersNameArray[i], "MaxNCellQuotaRoot") == 0)
        {
            PReadWrite->WriteCell(MaxNCellQuotaRoot, 8);
        }
        else if (strcmp(ParametersNameArray[i], "VNmax") == 0)
        {
            PReadWrite->WriteCell(VNmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KN") == 0)
        {
            PReadWrite->WriteCell(KN, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Sigma1") == 0)
        {
            PReadWrite->WriteCell(Sigma1, 8);
        }
        else if (strcmp(ParametersNameArray[i], "VNmaxRoot") == 0)
        {
            PReadWrite->WriteCell(VNmaxRoot, 8);
        }
        else if (strcmp(ParametersNameArray[i], "KNroot") == 0)
        {
            PReadWrite->WriteCell(KNroot, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Sigma2") == 0)
        {
            PReadWrite->WriteCell(Sigma2, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Tau") == 0)
        {
            PReadWrite->WriteCell(Tau, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Srec") == 0)
        {
            PReadWrite->WriteCell(Srec, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RECmax") == 0)
        {
            PReadWrite->WriteCell(RECmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "LMR20") == 0)
        {
            PReadWrite->WriteCell(LMR20, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TetaLM") == 0)
        {
            PReadWrite->WriteCell(TetaLM, 8);
        }
        else if (strcmp(ParametersNameArray[i], "LMRv") == 0)
        {
            PReadWrite->WriteCell(LMRv, 8);
        }
        else if (strcmp(ParametersNameArray[i], "K4") == 0)
        {
            PReadWrite->WriteCell(K4, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RMR20") == 0)
        {
            PReadWrite->WriteCell(RMR20, 8);
        }
        else if (strcmp(ParametersNameArray[i], "RECruitmax") == 0)
        {
            PReadWrite->WriteCell(RECruitmax, 8);
        }
        else if (strcmp(ParametersNameArray[i], "TetaRec") == 0)
        {
            PReadWrite->WriteCell(TetaRec, 8);
        }
        else if (strcmp(ParametersNameArray[i], "SB0") == 0)
        {
            PReadWrite->WriteCell(SB0, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Krec1") == 0)
        {
            PReadWrite->WriteCell(Krec1, 8);
        }
        else if (strcmp(ParametersNameArray[i], "Krec2") == 0)
        {
            PReadWrite->WriteCell(Krec2, 8);
        }
        else if (strcmp(ParametersNameArray[i], "SB0") == 0)
        {
            PReadWrite->WriteCell(SB0, 8);
        }
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}

void TZosteraNoltii::Go()
{
        double MyJulianDay; int i,j, index;
        double TINNY = 0.000001;
	MyJulianDay = MyPEcoDynClass->GetJulianDay();
        TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
        SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
        //Debugger(NumberOfBoxesWithZosteraNoltii);
        IK = CalculateIK(MyJulianDay);
        for (int m = 0; m < NumberOfBoxesWithZosteraNoltii; m++)
        {
                i = ZosteraNoltiiRecord[m].LineCoordinate;
                j = ZosteraNoltiiRecord[m].ColumnCoordinate;
                if (
                        (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                        (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
                )
                {
                        index = i * NumberOfColumns + j;
                        if (MyTransportPointer != NULL)
                                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyBoxDepth, index,"Box depth",ObjectCode);
                        else
                                MyBoxDepth = MyPEcoDynClass->GetBoxDepth(index);
                        GPP[index] = 0.0;
                        NPP[index] = 0.0;
                        GrossOxygenProduction[index] = 0.0;
                        NetOxygenProduction[index] = 0.0;
                        TPMProduction[index] = 0.0;
                        POMProduction[index] = 0.0;
                        //if (BoxArray[index].Dummybool2)
                        if (
                              (MyBoxDepth > CriticalWaterDepth) &&
                              (
                                 (PhytoBiomass[index] > TINNY) && (PhytoRootBiomass[index] > TINNY) &&
                                 (CPhyto[index] > TINNY) && (CPhytoRoot[index] > TINNY) &&
                                 (NPhyto[index] > TINNY) && (NPhytoRoot[index] > TINNY) &&
                                 (PPhyto[index] > TINNY) && (PPhytoRoot[index] > TINNY) 
                              )
                            )
                        {
                                //Debugger(MyBoxDepth);
                                //Debugger(PhytoBiomass[index]);
	                        Production(index);
                                Respiration(index);
                                NitrogenReclamation(index);
	                        Mortality(index);
                                NitrogenUptake(index);
                                PhosphorusUptake(index);
                                NitrogenAndPhosphorusTransfer(index);
                                Growth(index);
                        }
                }
        }
}

void TZosteraNoltii::Integrate()
{
        int i, j, index;
        double TINNY = 0.000001;
        SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
        float myTimeStep = MyPEcoDynClass->GetTimeStep();
	// Integrate biomass within the box
        //Debugger(PhytoFlux[0]);

        /*Generic = CPhyto; GenericFlux = PhytoFlux;
        Integration();
	CPhyto = Generic; PhytoFlux = GenericFlux;

        Generic = NPhyto; GenericFlux = NCellFlux;
	Integration();
	NPhyto = Generic; NCellFlux = GenericFlux;

        Generic = PPhyto; GenericFlux = PCellFlux;
	Integration();
	PPhyto = Generic; PCellFlux = GenericFlux;

        Generic = CPhytoRoot; GenericFlux = PhytoFluxRoot;
        Integration();
	CPhytoRoot = Generic; PhytoFluxRoot = GenericFlux;

        Generic = NPhytoRoot; GenericFlux = NCellFluxRoot;
	Integration();
	NPhytoRoot = Generic; NCellFluxRoot = GenericFlux;

        Generic = PPhytoRoot; GenericFlux = PCellFluxRoot;
	Integration();
	PPhytoRoot = Generic; PCellFluxRoot = GenericFlux;

        Generic = ShootDensity; GenericFlux = ShootDensityFlux;
	Integration();
	ShootDensity = Generic; ShootDensityFlux = GenericFlux; */

        for (int m = 0; m < NumberOfBoxesWithZosteraNoltii; m++)
        {
            i = ZosteraNoltiiRecord[m].LineCoordinate;
            j = ZosteraNoltiiRecord[m].ColumnCoordinate;
            if (
                    (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                    (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
            )
            {
                index = i * NumberOfColumns + j;
                CPhyto[index] = CPhyto[index] + PhytoFlux[index] * myTimeStep;
                PhytoFlux[index] = 0.0;
                if (CPhyto[index] < 0.0) CPhyto[index] = 0.0;
                NPhyto[index] = NPhyto[index] + NCellFlux[index] * myTimeStep;
                NCellFlux[index] = 0.0;
                if (NPhyto[index] < 0.0) NPhyto[index] = 0.0;
                PPhyto[index] = PPhyto[index] + PCellFlux[index] * myTimeStep;
                PCellFlux[index] = 0.0;
                if (PPhyto[index] < 0.0) PPhyto[index] = 0.0;
                CPhytoRoot[index] = CPhytoRoot[index] + PhytoFluxRoot[index] * myTimeStep;
                PhytoFluxRoot[index] = 0.0;
                if (CPhytoRoot[index] < 0.0) CPhytoRoot[index] = 0.0;
                NPhytoRoot[index] = NPhytoRoot[index] + NCellFluxRoot[index] * myTimeStep;
                NCellFluxRoot[index] = 0.0;
                if (NPhytoRoot[index] < 0.0) NPhytoRoot[index] = 0.0;
                PPhytoRoot[index] = PPhytoRoot[index] + PCellFluxRoot[index] * myTimeStep;
                PCellFluxRoot[index] = 0.0;
                if (PPhytoRoot[index] < 0.0) PPhytoRoot[index] = 0.0;
                ShootDensity[index] = ShootDensity[index] + ShootDensityFlux[index] * myTimeStep;
                ShootDensityFlux[index] = 0.0;
                if (ShootDensity[index] < 0.0) ShootDensity     [index] = 0.0;

                /*CPhyto[index] = CPhyto[index] / CUBIC * CARBONATOMICWEIGHT;    //g DW / m2
                NPhyto[index] = NPhyto[index] / CUBIC * NITROGENATOMICWEIGHT;  //N / m2
                PPhyto[index] = PPhyto[index] / CUBIC * PHOSPHORUSATOMICWEIGHT;//P / m2 */
                //if ((PhytoBiomass[index] > TINNY) && (CCellQuota[index] > TINNY))
                if (CCellQuota[index] > TINNY)
                {
                    PhytoBiomass[index] = CPhyto[index] / CUBIC * CARBONATOMICWEIGHT / CCellQuota[index]; //g DW / m2
                    if (PhytoBiomass[index] > TINNY)
                    {
                        NCellQuota[index] = NPhyto[index] / CUBIC * NITROGENATOMICWEIGHT / PhytoBiomass[index];
                        PCellQuota[index] = PPhyto[index] / CUBIC * PHOSPHORUSATOMICWEIGHT/ PhytoBiomass[index];
                    }
                    else
                    {
                        PhytoBiomass[index] = 0.0; //g DW / m2
                        NCellQuota[index] = 0.0;
                        PCellQuota[index] = 0.0;
                        CPhyto[index] = 0.0; NPhyto[index] = 0.0; PPhyto[index] = 0.0;
                    }
                }
                else
                {
                    PhytoBiomass[index] = 0.0; //g DW / m2
                    NCellQuota[index] = 0.0;
                    PCellQuota[index] = 0.0;
                    CPhyto[index] = 0.0; NPhyto[index] = 0.0; PPhyto[index] = 0.0;
                }
                /*CPhytoRoot[index] = CPhytoRoot[index] / CUBIC * CARBONATOMICWEIGHT;    //g DW / m2
                NPhytoRoot[index] = NPhytoRoot[index] / CUBIC * NITROGENATOMICWEIGHT;  //N / m2
                PPhytoRoot[index] = PPhytoRoot[index] / CUBIC * PHOSPHORUSATOMICWEIGHT;//P / m2   */
                if (CCellQuotaRoot[index] > TINNY)
                {
                    PhytoRootBiomass[index] = CPhytoRoot[index] / CUBIC * CARBONATOMICWEIGHT / CCellQuotaRoot[index];  //g DW / m2
                    if (PhytoRootBiomass[index] > TINNY)
                    {
                        NCellQuotaRoot[index] = NPhytoRoot[index] / CUBIC * NITROGENATOMICWEIGHT / PhytoRootBiomass[index];
                        PCellQuotaRoot[index] = PPhytoRoot[index] / CUBIC * PHOSPHORUSATOMICWEIGHT / PhytoRootBiomass[index];
                    }
                    else
                    {
                        PhytoRootBiomass[index] = 0.0;  //g DW / m2
                        NCellQuotaRoot[index] = 0.0;
                        PCellQuotaRoot[index] = 0.0;
                        CPhytoRoot[index] = 0.0; NPhytoRoot[index] = 0.0; PPhytoRoot[index] = 0.0;
                    }
                }
                else
                {
                    PhytoRootBiomass[index] = 0.0;  //g DW / m2
                    NCellQuotaRoot[index] = 0.0;
                    PCellQuotaRoot[index] = 0.0;
                    CPhytoRoot[index] = 0.0; NPhytoRoot[index] = 0.0; PPhytoRoot[index] = 0.0;
                }
            }
         }
}
/*
void TZosteraNoltii::Production(int ABoxNumber)
{
   double ALight, CanopyTopLight, MyDepth, Step;
   double CanopyHeight = 0.3;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
        * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
        * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      if (MyLightPointer != NULL)
      {
         //Debugger(MyPEcoDynClass->GetCurrentTime());
         if (MySPMPointer != NULL)
            MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,MyBoxNumber,"Extinction coefficient Ktot", ObjectCode);
         MyDepth = MIN(0,MyBoxDepth - CanopyHeight);
         MyLightPointer->Inquiry(GetEcoDynClassName(), CanopyTopLight, MyDepth, MyBoxNumber,"Sub-surface PAR irradiance",ObjectCode);

         if ((MyBoxDepth > 0.0) && (CanopyTopLight > 0.0))
         {
            LAI[MyBoxNumber] = CalculateLeafAreaIndex(MyBoxNumber);
            Step = LAI[MyBoxNumber] / 10.0;
            if (MyWaterTemperaturePointer != NULL)
            {
               MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature,MyBoxNumber,"Water temperature", ObjectCode);
               Pmax[MyBoxNumber] = MAX(0, TemperatureLimitation(AWaterTemperature,Pmax0,TetaPmax)); //g O2 mmolC-1 d-1
               //Debugger(111);Debugger(AWaterTemperature); Debugger(Pmax0); Debugger(TetaPmax);
            }
            else
                Pmax[MyBoxNumber] = Pmax0;
            Productivity = 0.0;
            ALight =  CanopyTopLight;
            //Debugger(222); Debugger(Pmax[MyBoxNumber]);
            for (double z = 0; z <= LAI[MyBoxNumber]; z = z + Step)
            {
               ALight = CanopyTopLight * exp(-K3 * z); //Usa a luz em W/m2
               if (IK > 0.0)
                  Productivity = Productivity + Pmax[MyBoxNumber] * tanh(ALight / IK) * Step; //  g O2 mmolC-1 d-1
               else
                  MessageBox(0,"IK <= 0 in TZosteraNoltii::Production","EcoDynamo Alert",MB_OK | MB_ICONHAND);
            }
            BottomLight = ALight;
            //Debugger(333); Debugger(Productivity);
            Productivity = Productivity / DAYSTOSECONDS;   //g O2 mmolC-1 s-1
         }
         else Productivity = 0.0;
         //Debugger(444); Debugger(NutrientLimitation(SaturationLevelOfLeafNQuota(MyBoxNumber), E1));
         GPP[MyBoxNumber] = Productivity * NutrientLimitation(SaturationLevelOfLeafNQuota(MyBoxNumber), E1);
         //Debugger(555);Debugger(GPP[MyBoxNumber]);
         NPP[MyBoxNumber] = GPP[MyBoxNumber];
         //PhytoFlux[MyBoxNumber] = NPP[MyBoxNumber] * CPhyto[MyBoxNumber];   //g O2 s-1 m-2;
      }
      else BottomLight = 0.0;
      RecruitmentFunction(MyBoxNumber);
   }
} */

void TZosteraNoltii::Production(int ABoxNumber)
{
   double ALight, CanopyTopLight, MyDepth, Step, TotalLightAtSurface, LightAtSurface;
   double CanopyHeight = 0.3;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
        * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
        * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if ((PhytoBiomass[MyBoxNumber] > 0.0000000001) && (PhytoRootBiomass[MyBoxNumber] > 0.0000000001) && (ShootDensity[MyBoxNumber] > 0.0000000001))
   {
      if (MyLightPointer != NULL)
      {
         if (MySPMPointer != NULL)
            MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,MyBoxNumber,"Extinction coefficient Ktot", ObjectCode);
         else
            KValue = 1.0;
         MyDepth = MAX(0,MyBoxDepth - CanopyHeight);
         //MyLightPointer->Inquiry(GetEcoDynClassName(), CanopyTopLight, MyDepth, MyBoxNumber,"Sub-surface PAR irradiance",ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), TotalLightAtSurface,MyBoxNumber,"Total surface irradiance",ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtSurface,MyBoxNumber,"PAR surface irradiance",ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), CanopyTopLight, TotalLightAtSurface, MyDepth, KValue, MyBoxNumber,
                                 "Sub-surface PAR irradiance",ObjectCode);
         //Debugger(CanopyTopLight);
         if ((MyBoxDepth > 0.0) && (CanopyTopLight > 0.0))
         {
            LAI[MyBoxNumber] = CalculateLeafAreaIndex(MyBoxNumber);
            Step = LAI[MyBoxNumber] / 10.0;
            //Pmax[MyBoxNumber] = Pmax0;
            if (MyWaterTemperaturePointer != NULL)
            {
               MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature,MyBoxNumber,"Water temperature", ObjectCode);
               Pmax[MyBoxNumber] = MAX(0, TemperatureLimitation(AWaterTemperature,Pmax0,TetaPmax)); //g O2 mmolC-1 d-1
            }
            else
                Pmax[MyBoxNumber] = Pmax0;
            Productivity = 0.0;
            ALight =  CanopyTopLight;
            for (double z = 0; z <= LAI[MyBoxNumber]; z = z + Step)
            {
               if ((PhytoBiomass[MyBoxNumber] > 200.0) && (z > 0))
                 ALight = 0.0;
               else
                   ALight = CanopyTopLight * exp(-K3 * z); //Usa a luz em W/m2
               if (IK > 0.0)
                  Productivity = Productivity + Pmax[MyBoxNumber] * tanh(ALight / IK/*Ikmax*/) * Step; //  g O2 mmolC-1 d-1
               else
#ifdef __BORLANDC__
                  MessageBox(0,"IK <= 0 in TZosteraNoltii::Production","EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
									cerr << "IK <= 0 in TZosteraNoltii::Production" << endl;
#endif  // __BORLANDC__
            }
            BottomLight = ALight;
            Productivity = Productivity / DAYSTOSECONDS;   //g O2 mmolC-1 s-1
         }
         else Productivity = 0.0;
         GPP[MyBoxNumber] = Productivity * NutrientLimitation(SaturationLevelOfLeafNQuota(MyBoxNumber), E1);
         NPP[MyBoxNumber] = GPP[MyBoxNumber];
      }
      else BottomLight = 0.0;
      RecruitmentFunction(MyBoxNumber);
   }
}

void TZosteraNoltii::Respiration(int ABoxNumber)
{
   double Resp;
   int MyBoxNumber;
   MyBoxNumber = ABoxNumber;
   if ((PhytoBiomass[MyBoxNumber] > 0.0000000001) && (PhytoRootBiomass[MyBoxNumber] > 0.0000000001) && (ShootDensity[MyBoxNumber] > 0.0000000001))
   {
       TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
       TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
       TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
       if (MyWaterTemperaturePointer != NULL)
       {
            MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature,MyBoxNumber,"Water temperature", ObjectCode);
            LeafResp[MyBoxNumber] = MAX(0, TemperatureLimitation(AWaterTemperature,LR0,TetaLR)); //g O2 mmolC-1 d-1
            RootResp[MyBoxNumber] = MAX(0, TemperatureLimitation(AWaterTemperature,RR0,TetaRR)); //g O2 mmolC-1 d-1
       }
       else
       {
            LeafResp[MyBoxNumber] = LR0;
            RootResp[MyBoxNumber] = RR0;
       }
       LeafResp[MyBoxNumber] = LeafResp[MyBoxNumber] / DAYSTOSECONDS;
       RootResp[MyBoxNumber] = RootResp[MyBoxNumber] / DAYSTOSECONDS;
       LeafResp[MyBoxNumber] = LeafResp[MyBoxNumber] * CPhyto[MyBoxNumber];   //g O2 s-1 m-2
       RootResp[MyBoxNumber] = RootResp[MyBoxNumber] * CPhytoRoot[MyBoxNumber]; //g O2 s-1 m-2
       if (MyNutrientPointer != NULL)
       {
            MyNutrientPointer->Update(GetEcoDynClassName(),
                                      (GPP[ABoxNumber] * CPhyto[ABoxNumber] - LeafResp[ABoxNumber]) / MyBoxDepth, ABoxNumber,"Oxygen",   //Return as mg O2 / L / s
                                      ObjectCode);
       }
       if (MySedimentPointer != NULL)
       {
            MySedimentPointer->Update(GetEcoDynClassName(),
                                      -RootResp[ABoxNumber] / MyBoxDepth, ABoxNumber,"O2InPoreWater 1",   //Return as mg O2 / L / s
                                      ObjectCode);
       }

       Resp = LeafResp[MyBoxNumber] + RootResp[MyBoxNumber]; //g O2 s-1 m-2
       /*if (NPP[ABoxNumber] < 0.0)
       {
            PhytoFlux[ABoxNumber] = (NPP[ABoxNumber] - LeafResp[MyBoxNumber]) / (PQ  * 2.0 * OXYGENATOMICWEIGHT) * CUBIC; //mmol C s-1 m-2
            PhytoFluxRoot[ABoxNumber] = - RootResp[MyBoxNumber] / (PQ  * 2.0 * OXYGENATOMICWEIGHT) * CUBIC; //mmol C s-1 m-2
       } */
       NPP[ABoxNumber] = NPP[ABoxNumber] * CPhyto[MyBoxNumber]; //g O2 m-2 s-1
       if (NPP[ABoxNumber] - Resp < 0.0)
       {
            PhytoFlux[ABoxNumber] = (NPP[ABoxNumber] - LeafResp[MyBoxNumber]) / (PQ  * 2.0 * OXYGENATOMICWEIGHT) * CUBIC; //mmol C s-1 m-2
            PhytoFluxRoot[ABoxNumber] = - RootResp[MyBoxNumber] / (PQ  * 2.0 * OXYGENATOMICWEIGHT) * CUBIC; //mmol C s-1 m-2
       }
       else
            NPP[ABoxNumber] = (NPP[ABoxNumber] - Resp) / (PQ  * 2.0 * OXYGENATOMICWEIGHT) * CUBIC; //mmol C s-1 m-2
   }
}

void TZosteraNoltii::NitrogenUptake(int ABoxNumber)
{
        double MyAmmonia, MyNitrate, AmmoniaUptake, NitrateUpTake;
        TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
        TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
        if ((MyNutrientPointer != NULL) && (PhytoBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
        {
                MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyAmmonia, ABoxNumber,"Ammonia",ObjectCode);
                MyNutrientPointer->Inquiry(GetEcoDynClassName(), MyNitrate, ABoxNumber,"Nitrate",ObjectCode);
                LeafNSaturation[ABoxNumber] = SaturationLevelOfLeafNQuota(ABoxNumber);
                if (1.0 - LeafNSaturation[ABoxNumber] <= 0.00000001)
                {
                    AmmoniaUptake = 0.0;
                    NitrateUpTake = 0.0;
                }
                else
                {
                    AmmoniaUptake = VNmax * MyAmmonia / (MyAmmonia + KN) * pow((1.0 - LeafNSaturation[ABoxNumber]),Sigma1) / DAYSTOSECONDS;
                    NitrateUpTake = VNmax * MyNitrate / (MyNitrate + KN) * pow((1.0 - LeafNSaturation[ABoxNumber]),Sigma1) / DAYSTOSECONDS;
                }
                //mmol N / mmolC / s
                AmmoniaUptake = AmmoniaUptake * CPhyto[ABoxNumber]; //mmol N / m2 / s
                NitrateUpTake = NitrateUpTake * CPhyto[ABoxNumber];
                NUptake[ABoxNumber] = AmmoniaUptake + NitrateUpTake;  //mmol N / m2 / s
                //NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] + NUptake[ABoxNumber];
                NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] + NUptake[ABoxNumber]; //mmol N / m2 / s

                MyNutrientPointer->Update(GetEcoDynClassName(),
                                  -AmmoniaUptake / MyBoxDepth * CUBIC, ABoxNumber,"Ammonia",   //Return as micromol / m3 / s
                                  ObjectCode);
                MyNutrientPointer->Update(GetEcoDynClassName(),
                                  -NitrateUpTake / MyBoxDepth * CUBIC, ABoxNumber,"Nitrate",   //Return as micromol / m3 / s
                                  ObjectCode);
        }
        if ((MySedimentPointer != NULL) && (PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
        {
               MySedimentPointer->Inquiry(GetEcoDynClassName(), MyAmmonia, ABoxNumber,"NH4InPoreWater 1",ObjectCode);
               RootNSaturation[ABoxNumber] = SaturationLevelOfRootNQuota(ABoxNumber);
               if (1.0 - RootNSaturation[ABoxNumber] <= 0.00000001)
                   AmmoniaUptake = 0.0;
               else
                   AmmoniaUptake = VNmaxRoot * MyAmmonia / (MyAmmonia + KNroot) * pow((1.0 - RootNSaturation[ABoxNumber]),Sigma2) / DAYSTOSECONDS;
               AmmoniaUptake = AmmoniaUptake * CPhytoRoot[ABoxNumber]; //mmol N / m2 / s
               NUptakeRoot[ABoxNumber] = AmmoniaUptake;   //mmol N / m2 / s
               //NCellFluxRoot[ABoxNumber] = NCellFluxRoot[ABoxNumber] + AmmoniaUptake;
               NCellFluxRoot[ABoxNumber] = NCellFluxRoot[ABoxNumber] + NUptakeRoot[ABoxNumber]; //mmol N / m2 / s

               MySedimentPointer->Update(GetEcoDynClassName(),
                                  -AmmoniaUptake / MyBoxDepth, ABoxNumber,"NH4InPoreWater 1",   //Return as micromol / L / s
                                  ObjectCode);
        }
}

void TZosteraNoltii::PhosphorusUptake(int ABoxNumber)
{
        double MyPhosphate, MyPUptake;
        TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
        TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
        if ((MyNutrientPointer != NULL) && (PhytoBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
        {
                //PUptake[ABoxNumber] = NUptake[ABoxNumber] * (PCellQuota[ABoxNumber] / PHOSPHORUSATOMICWEIGHT)/ (NCellQuota[ABoxNumber] / NITROGENATOMICWEIGHT);
                PUptake[ABoxNumber] = NUptake[ABoxNumber] * PPhyto[ABoxNumber] / NPhyto[ABoxNumber];
                //PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] + PUptake[ABoxNumber];   //mmol P / mmolC / s
                PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] + PUptake[ABoxNumber];  //mmol P / m2 / s
                MyPUptake = PUptake[ABoxNumber];  //mmol P / m2 / s
                MyNutrientPointer->Update(GetEcoDynClassName(),
                                  -MyPUptake / MyBoxDepth * CUBIC, ABoxNumber,"Phosphate",   //Return as micromol / m3 / s
                                  ObjectCode);
        }
        if ((MySedimentPointer != NULL) && (PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
        {
                //PUptakeRoot[ABoxNumber] = NUptakeRoot[ABoxNumber] * (PCellQuotaRoot[ABoxNumber] / PHOSPHORUSATOMICWEIGHT) / (NCellQuotaRoot[ABoxNumber] / NITROGENATOMICWEIGHT);
                PUptakeRoot[ABoxNumber] = NUptakeRoot[ABoxNumber] * PPhytoRoot[ABoxNumber] / NPhytoRoot[ABoxNumber];
                //PCellFluxRoot[ABoxNumber] = PCellFluxRoot[ABoxNumber] + PUptakeRoot[ABoxNumber];  //mmol P / mmolC / s
                PCellFluxRoot[ABoxNumber] = PCellFluxRoot[ABoxNumber] + PUptakeRoot[ABoxNumber]; //mmol P / m2 / s
                MyPUptake = PUptakeRoot[ABoxNumber];  //mmol P / m2 / s
                MySedimentPointer->Update(GetEcoDynClassName(),
                                  -MyPUptake / MyBoxDepth, ABoxNumber,"PO4InPoreWater 1",   //Return as micromol / L / s
                                  ObjectCode);
        }
}

void TZosteraNoltii::NitrogenAndPhosphorusTransfer(int ABoxNumber)
{
   if ((PhytoBiomass[ABoxNumber] > 0.0000000001) && (PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
   {
   double delta, LeafTransfer, RootTransfer;
   delta = RootNSaturation[ABoxNumber] - LeafNSaturation[ABoxNumber];
   if ((delta > 0.0) && (LeafNSaturation[ABoxNumber] < 0.75))
   {
      LeafTransfer = 0.0;
      RootTransfer = Tau * delta / DAYSTOSECONDS * CPhytoRoot[ABoxNumber];   //mmolN / m2 / s
      NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] + RootTransfer;
      PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] + RootTransfer * PPhytoRoot[ABoxNumber] / NPhytoRoot[ABoxNumber];
      NCellFluxRoot[ABoxNumber] = NCellFluxRoot[ABoxNumber] - RootTransfer;
      PCellFluxRoot[ABoxNumber] = PCellFluxRoot[ABoxNumber] - RootTransfer * PPhytoRoot[ABoxNumber] / NPhytoRoot[ABoxNumber];
   }
   else
   if ((delta < 0.0) && (RootNSaturation[ABoxNumber] < 0.75))
   {
      LeafTransfer = Tau * fabs(delta) / DAYSTOSECONDS * CPhyto[ABoxNumber];
      RootTransfer = 0.0;
      NCellFluxRoot[ABoxNumber] = NCellFluxRoot[ABoxNumber] + LeafTransfer;
      PCellFluxRoot[ABoxNumber] = PCellFluxRoot[ABoxNumber] + LeafTransfer * PPhyto[ABoxNumber] / NPhyto[ABoxNumber];
      NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] - LeafTransfer;
      PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] - LeafTransfer * PPhyto[ABoxNumber] / NPhyto[ABoxNumber];
   }
   else
   {
      LeafTransfer = 0.0;
      RootTransfer = 0.0;
   }
   }
}


void TZosteraNoltii::NitrogenReclamation(int ABoxNumber)
{
    if ((PhytoBiomass[ABoxNumber] > 0.0000000001) && (PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
    {
        if (LeafNSaturation[ABoxNumber] > Srec)
           LeafNReclamation[ABoxNumber] = 0.0;
        else
        if (LeafNSaturation[ABoxNumber] < 0.00000001)
           LeafNReclamation[ABoxNumber] = RECmax;
        else
           LeafNReclamation[ABoxNumber] = (1.0 - pow(LeafNSaturation[ABoxNumber] / Srec, 2.0)) * RECmax;

        if (RootNSaturation[ABoxNumber] > Srec)
           RootNReclamation[ABoxNumber] = 0.0;
        else
        if (RootNSaturation[ABoxNumber] < 0.00000001)
           RootNReclamation[ABoxNumber] = RECmax;
        else
           RootNReclamation[ABoxNumber] = (1.0 - pow(RootNSaturation[ABoxNumber] / Srec, 2.0)) * RECmax;
    }
    else
    {
       LeafNReclamation[ABoxNumber] = 0.0;
       RootNReclamation[ABoxNumber] = 0.0;
    }
}

double TZosteraNoltii::NutrientLimitation(double ASaturationLevel,double ACoefficient)
{
   if (ASaturationLevel <= 0.00000001)
      return 0.0;
   else
      return pow(ASaturationLevel, ACoefficient);
}

void TZosteraNoltii::Mortality(int ABoxNumber)
{
        double LeafBiomassLoss, RootBiomassLoss, CarbonLoss, NitrogenLoss, PhosphorusLoss, LeafDeathLoss, RootDeathLoss, AWaterTemperature, AUVelocity, AVVelocity, F5;
        double const AReferenceTemperature = 20.0; //ºC
        if ((PhytoBiomass[ABoxNumber] > 0.0000000001) && (PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
        {
        TEcoDynClass* MySedimentPointer = MyPEcoDynClass->GetSedimentPointer(),
                    * MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer(),
                    * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer(),
                    * MyTransportPointer = MyPEcoDynClass->GetTransportPointer(),
                    * MySPMPointer = MyPEcoDynClass->GetSPMPointer();
        if (MyWaterTemperaturePointer != NULL)
        {
           MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature, ABoxNumber,"Water temperature", ObjectCode);
           if (TetaLM > 0.0)
              F5 = pow(TetaLM, AWaterTemperature - AReferenceTemperature);
           else F5 = 0.0;
        }
        else
           F5 = 1.0;
        LeafDeathLoss = LMR20 * F5;    // 1 / day

        if (MyTransportPointer != NULL)
        {
           MyTransportPointer->Inquiry(GetEcoDynClassName(), AUVelocity, ABoxNumber,"U Velocity", ObjectCode);
           MyTransportPointer->Inquiry(GetEcoDynClassName(), AVVelocity, ABoxNumber,"V Velocity", ObjectCode);
           if ((AUVelocity <= 0.00000001) && (AVVelocity > 0.00000001))
              LeafDeathLoss = LeafDeathLoss + LMRv * AVVelocity;
           else
           if ((AUVelocity > 0.00000001) && (AVVelocity <= 0.00000001))
              LeafDeathLoss = LeafDeathLoss + LMRv * AUVelocity /** exp(-K4 * MyBoxDepth)*/;  //1 / day
           else
              LeafDeathLoss = LeafDeathLoss + LMRv * sqrt(pow(AUVelocity, 2.0)+ pow(AVVelocity,2.0)) /** exp(-K4 * MyBoxDepth)*/;  //1 / day
        }
        ShootDensityFlux[ABoxNumber] = ShootDensityFlux[ABoxNumber] - LeafDeathLoss / DAYSTOSECONDS * ShootDensity[ABoxNumber]; //1/s/m2
        CarbonLoss = LeafDeathLoss / DAYSTOSECONDS * CPhyto[ABoxNumber]; //mmol C / m2 / s
        PhytoFlux[ABoxNumber] = PhytoFlux[ABoxNumber] - CarbonLoss; //mmol C / m2 / s
        CarbonLoss = CarbonLoss * CARBONATOMICWEIGHT; //mg C / m2 / s
        LeafBiomassLoss = CarbonLoss / CCellQuota[ABoxNumber]; //mg DW / m2 / s
        NitrogenLoss = /*CarbonLoss*/LeafBiomassLoss *  NCellQuota[ABoxNumber] * (1.0 - LeafNReclamation[ABoxNumber]);  //mg N / m2 / s
        NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] - NitrogenLoss / NITROGENATOMICWEIGHT;   //mmol N / m2 / s
        //PhosphorusLoss = NitrogenLoss * PCellQuota[ABoxNumber] / NCellQuota[ABoxNumber];
        PhosphorusLoss = NitrogenLoss * (PPhyto[ABoxNumber] * PHOSPHORUSATOMICWEIGHT) / (NPhyto[ABoxNumber] * NITROGENATOMICWEIGHT);
        PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] - PhosphorusLoss / PHOSPHORUSATOMICWEIGHT;  //mmol P / m2 / s
        double TPMRelease;
        TPMRelease = LeafBiomassLoss / MyBoxDepth / CUBIC;
        TPMProduction[ABoxNumber] = TPMProduction[ABoxNumber] + TPMRelease;
        POMProduction[ABoxNumber] = POMProduction[ABoxNumber] + TPMRelease;
        /*if (MySPMPointer != NULL)
        {
                MySPMPointer->Update(GetEcoDynClassName(), TPMRelease, ABoxNumber,"POM",ObjectCode);   //mg DW / L / s
                MySPMPointer->Update(GetEcoDynClassName(), TPMRelease, ABoxNumber,"TPM",ObjectCode);   //mg DW / L / s
                MySPMPointer->Update(GetEcoDynClassName(), CarbonLoss / MyBoxDepth /CUBIC,ABoxNumber,"DetritalC",ObjectCode);    //mg C / L / s
                MySPMPointer->Update(GetEcoDynClassName(), NitrogenLoss / MyBoxDepth / CUBIC,ABoxNumber,"DetritalN",ObjectCode); //mg N / L / s
                MySPMPointer->Update(GetEcoDynClassName(), PhosphorusLoss / MyBoxDepth / CUBIC,ABoxNumber,"DetritalP",ObjectCode);//mg N / L / s
        } */
        if (MySedimentPointer != NULL)
        {
                MySedimentPointer->Update(GetEcoDynClassName(), CarbonLoss /*mg / m2 / s*/, ABoxNumber, "Corganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), NitrogenLoss, ABoxNumber,"Norganic 1",ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), PhosphorusLoss, ABoxNumber, "Porganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), LeafBiomassLoss, ABoxNumber, "OrganicMass 1", ObjectCode);
        }

        RootDeathLoss = RMR20 * F5;
        CarbonLoss = RootDeathLoss / DAYSTOSECONDS * CPhytoRoot[ABoxNumber]; //mmol C / m2 / s
        PhytoFluxRoot[ABoxNumber] = PhytoFluxRoot[ABoxNumber] - CarbonLoss;  //mmol C / m2 / s
        CarbonLoss = CarbonLoss * CARBONATOMICWEIGHT;  //mg C / m2 / s
        RootBiomassLoss = CarbonLoss / CCellQuotaRoot[ABoxNumber];  //mg DW / m2 / s

        NitrogenLoss = /*CarbonLoss*/RootBiomassLoss *  NCellQuotaRoot[ABoxNumber] * (1.0 - RootNReclamation[ABoxNumber]);  //mg N / m2 / s
        NCellFluxRoot[ABoxNumber] = NCellFluxRoot[ABoxNumber] - NitrogenLoss / NITROGENATOMICWEIGHT;   //mmol N / m2 / s
        //PhosphorusLoss = NitrogenLoss * PCellQuotaRoot[ABoxNumber] / NCellQuotaRoot[ABoxNumber];
        PhosphorusLoss = NitrogenLoss * (PPhytoRoot[ABoxNumber] * PHOSPHORUSATOMICWEIGHT) / (NPhytoRoot[ABoxNumber] * NITROGENATOMICWEIGHT);
        PCellFluxRoot[ABoxNumber] = PCellFluxRoot[ABoxNumber] - PhosphorusLoss / PHOSPHORUSATOMICWEIGHT;  //mmol P / m2 / s
        if (MySedimentPointer != NULL)
        {
                MySedimentPointer->Update(GetEcoDynClassName(), CarbonLoss  /*mg / m2 / s*/, ABoxNumber, "Corganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), NitrogenLoss, ABoxNumber,"Norganic 1",ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), PhosphorusLoss, ABoxNumber, "Porganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), RootBiomassLoss, ABoxNumber, "OrganicMass 1", ObjectCode);
        }
        }
}



void  TZosteraNoltii::Growth(int ABoxNumber)
{
   double const AssimptoticIndividualBiomass = 0.015; //g
   double const MaxRootLeaveBiomassRatio = 0.65;
   if ((PhytoBiomass[ABoxNumber] > 0.0000000001) && (PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
   {
   if ((NPP[ABoxNumber] > 0.0) && (PhytoBiomass[ABoxNumber] / ShootDensity[ABoxNumber] <= AssimptoticIndividualBiomass))
   {
        TotalGrowth[ABoxNumber] = NPP[ABoxNumber];   //mmol C / m2 / s
        if (PhytoRootBiomass[ABoxNumber] / PhytoBiomass[ABoxNumber] <= MaxRootLeaveBiomassRatio)
                RootGrowth[ABoxNumber] = TotalGrowth[ABoxNumber] * NutrientLimitation(SaturationLevelOfRootNQuota(ABoxNumber), E2) * K;
        else
                RootGrowth[ABoxNumber] = 0.0;
        LeafGrowth[ABoxNumber] = TotalGrowth[ABoxNumber] - RootGrowth[ABoxNumber];
        PhytoFlux[ABoxNumber] = PhytoFlux[ABoxNumber] + LeafGrowth[ABoxNumber];
        PhytoFluxRoot[ABoxNumber] = PhytoFluxRoot[ABoxNumber] + RootGrowth[ABoxNumber];
   }
   else
   {
        TotalGrowth[ABoxNumber] = 0.0;   //mmol C / m2 / s
        RootGrowth[ABoxNumber] = 0.0;
        LeafGrowth[ABoxNumber] = 0.0;
        if (NPP[ABoxNumber] > 0.0)
        {
           TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
           * MySedimentPointer = MyPEcoDynClass->GetSedimentPointer();
           double CarbonLoss, LeafBiomassLoss, NitrogenLoss, PhosphorusLoss;
           PhytoFlux[ABoxNumber] = PhytoFlux[ABoxNumber] - NPP[ABoxNumber];
           CarbonLoss = NPP[ABoxNumber] * CARBONATOMICWEIGHT; //mg C / m2 / s
           LeafBiomassLoss = CarbonLoss / CCellQuota[ABoxNumber]; //mg DW / m2 / s
           NitrogenLoss = LeafBiomassLoss *  NCellQuota[ABoxNumber] * (1.0 - LeafNReclamation[ABoxNumber]);  //mg N / m2 / s
           NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] - NitrogenLoss / NITROGENATOMICWEIGHT;   //mmol N / m2 / s
           PhosphorusLoss = NitrogenLoss * (PPhyto[ABoxNumber] * PHOSPHORUSATOMICWEIGHT) / (NPhyto[ABoxNumber] * NITROGENATOMICWEIGHT); //mg P / m2 / s
           PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] - PhosphorusLoss / PHOSPHORUSATOMICWEIGHT;  //mmol P / m2 / s
           double TPMRelease;
           TPMRelease = LeafBiomassLoss / MyBoxDepth / CUBIC;
           TPMProduction[ABoxNumber] = TPMProduction[ABoxNumber] + TPMRelease;
           POMProduction[ABoxNumber] = POMProduction[ABoxNumber] + TPMRelease;

           /*if (MySPMPointer != NULL)
           {
                MySPMPointer->Update(GetEcoDynClassName(), TPMRelease, ABoxNumber,"POM",ObjectCode);   //mg DW / L / s
                MySPMPointer->Update(GetEcoDynClassName(), TPMRelease, ABoxNumber,"TPM",ObjectCode);   //mg DW / L / s
                MySPMPointer->Update(GetEcoDynClassName(), CarbonLoss / MyBoxDepth /CUBIC,ABoxNumber,"DetritalC",ObjectCode);    //mg C / L / s
                MySPMPointer->Update(GetEcoDynClassName(), NitrogenLoss / MyBoxDepth / CUBIC,ABoxNumber,"DetritalN",ObjectCode); //mg N / L / s
                MySPMPointer->Update(GetEcoDynClassName(), PhosphorusLoss / MyBoxDepth / CUBIC,ABoxNumber,"DetritalP",ObjectCode);//mg N / L / s
           } */

           if (MySedimentPointer != NULL)
           {
                MySedimentPointer->Update(GetEcoDynClassName(), CarbonLoss, ABoxNumber, "Corganic 1", ObjectCode); //mg / m2 / s
                MySedimentPointer->Update(GetEcoDynClassName(), NitrogenLoss, ABoxNumber,"Norganic 1",ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), PhosphorusLoss, ABoxNumber, "Porganic 1", ObjectCode);
                MySedimentPointer->Update(GetEcoDynClassName(), LeafBiomassLoss, ABoxNumber, "OrganicMass 1", ObjectCode);
           }  
        }
   }
   }
}

double TZosteraNoltii::TemperatureLimitation(double AWaterTemperature, double AYIntercept, double ASlope)
{
        return (ASlope * AWaterTemperature + AYIntercept);
}

double TZosteraNoltii::CalculateLeafAreaIndex(int ABoxNumber)
{
        return KLAI * CPhyto[ABoxNumber];
}

double TZosteraNoltii::CalculateIK(int ADay)
{
   return 0.5 * (Ikmax + Ikmin) + 0.5 * (Ikmax - Ikmin) * cos(2.0 * M_PI  * 1.0 / 365.0 * (ADay - 110));
}

double TZosteraNoltii::SaturationLevelOfLeafNQuota(int ABoxNumber)
{
        double X;
        if (CPhyto[ABoxNumber] > 0.0000000001) //Para evitar divisões por números muito pequenos
                X = MIN(MaxNCellQuota, NPhyto[ABoxNumber] / CPhyto[ABoxNumber]);
        else
                X = MaxNCellQuota;
        if (MaxNCellQuota - MinNCellQuota > 0.0)
           return MAX(0,(X - MinNCellQuota) / (MaxNCellQuota - MinNCellQuota));
        else
#ifdef __BORLANDC__
           MessageBox(0, "Denominator <= 0 in TZosteraNoltii::SaturationLevelOfLeafNQuota", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Denominator <= 0 in TZosteraNoltii::SaturationLevelOfLeafNQuota" << endl;
#endif  // __BORLANDC__
}

double TZosteraNoltii::SaturationLevelOfRootNQuota(int ABoxNumber)
{
        double X;
        if (CPhytoRoot[ABoxNumber] > 0.0000000001) //Para evitar divisões por números muito pequenos
                X = MIN(MaxNCellQuotaRoot, NPhytoRoot[ABoxNumber] / CPhytoRoot[ABoxNumber]);
        else
                X = MaxNCellQuotaRoot;
        if (MaxNCellQuotaRoot - MinNCellQuotaRoot > 0.0)
           return MAX(0,(X - MinNCellQuotaRoot) / (MaxNCellQuotaRoot - MinNCellQuotaRoot));
        else
#ifdef __BORLANDC__
           MessageBox(0, "Denominator <= 0 in TZosteraNoltii::SaturationLevelOfRootNQuota", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Denominator <= 0 in TZosteraNoltii::SaturationLevelOfRootNQuota" << endl;
#endif  // __BORLANDC__
}

void TZosteraNoltii::RecruitmentFunction(int ABoxNumber)
{
   double X,Y, Z;
   if ((PhytoRootBiomass[ABoxNumber] > 0.0000000001) && (ShootDensity[ABoxNumber] > 0.0000000001))
   {
   Recruitment[ABoxNumber] = TemperatureLimitationOfRecruitment(ABoxNumber) *
                             SelfShadingLimitation(ABoxNumber) * BelowgroundLimitation(ABoxNumber) / DAYSTOSECONDS;  //1/s
   X = Recruitment[ABoxNumber] * ShootDensity[ABoxNumber];
   ShootDensityFlux[ABoxNumber] = ShootDensityFlux[ABoxNumber] + X; //1/s/m2
   Y = X * SB0;
   Z = X * Y;
   PhytoFlux[ABoxNumber] = PhytoFlux[ABoxNumber] + Z; //mmolC m-2 s-1
   NCellFlux[ABoxNumber] = NCellFlux[ABoxNumber] + Z * NPhytoRoot[ABoxNumber] / CPhytoRoot[ABoxNumber]; //mmolN m-2 s-1
   PCellFlux[ABoxNumber] = PCellFlux[ABoxNumber] + Z * PPhytoRoot[ABoxNumber] / CPhytoRoot[ABoxNumber]; //mmolP m-2 s-1
   PhytoFluxRoot[ABoxNumber] = PhytoFluxRoot[ABoxNumber] - Z; //mmolC m-2 s-1
   NCellFluxRoot[ABoxNumber] = NCellFluxRoot[ABoxNumber] - Z * NPhytoRoot[ABoxNumber] / CPhytoRoot[ABoxNumber]; //mmolN m-2 s-1
   PCellFluxRoot[ABoxNumber] = PCellFluxRoot[ABoxNumber] - Z * PPhytoRoot[ABoxNumber] / CPhytoRoot[ABoxNumber]; //mmolP m-2 s-1
   }
}

double TZosteraNoltii::TemperatureLimitationOfRecruitment(int ABoxNumber)
{
   double AWaterTemperature;
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
        MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), AWaterTemperature, ABoxNumber,"Water temperature", ObjectCode);
        if (AWaterTemperature < 5.0) return 0.0;
        else
        if ((AWaterTemperature >= 5.0) && (AWaterTemperature <= 22.0)) return TetaRec;
        else
        if (AWaterTemperature > 22.0) return 2.8;
   }
   else
        return 1.0;
}

double TZosteraNoltii::SelfShadingLimitation(int ABoxNumber)
{
   if (BottomLight < 10.0) return 0.0;
   else
   return (BottomLight - 10.0) / ((BottomLight - 10.0) + Krec1);
}

double TZosteraNoltii::BelowgroundLimitation(int ABoxNumber)
{
   double const MinRootLeaveBiomassRatio = 0.3;
   if (PhytoBiomass[ABoxNumber] > 0.00001)
   {
        if (PhytoRootBiomass[ABoxNumber] / PhytoBiomass[ABoxNumber] >= MinRootLeaveBiomassRatio)
           return CPhytoRoot[ABoxNumber] / (CPhytoRoot[ABoxNumber] + Krec2);
        else
           return 0.0;
   }
   else
      return CPhytoRoot[ABoxNumber] / (CPhytoRoot[ABoxNumber] + Krec2);
}



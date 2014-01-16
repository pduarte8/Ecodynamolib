// SPM object CPP code file
// @ Pedro Duarte 2010.07.01
/**
 * NPI work
 * 	08 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "sedimentobjt.h"
#include "iodll.h"
#include "params.h"



// TSediments Class
//

TSedimentsPlus::TSedimentsPlus(TEcoDynClass* APEcoDynClass, char* className)
        : TSediments(APEcoDynClass, className)
{
    ReadParameters(className);
    int NumberOfSedimentCells;
    NumberOfSedimentCells = NumberOfBoxes * NumberOfSedimentLayers;
    Dissolution_CSediments = new double[NumberOfSedimentCells];
    Dissolution_NSediments = new double[NumberOfSedimentCells];
    Dissolution_PSediments = new double[NumberOfSedimentCells];
    for (int i = 0; i < NumberOfSedimentCells; i++)
    {
      Dissolution_CSediments[i] = 0.0;
      Dissolution_NSediments[i] = 0.0;
      Dissolution_PSediments[i] = 0.0;
    }
}

TSedimentsPlus::~TSedimentsPlus()
{
    freeMemory();
}

void TSedimentsPlus::freeMemory()
{
    try {
       /*if (NumberOfParameters > 0) {
          delete [] ParametersNameArray;
          NumberOfParameters = 0;
       } */
       if ((NumberOfBoxes > 0) && (NumberOfBoxesWithSediments > 0))
       {
          //delete [] Corganic;
          delete [] CorganicDissolved;
          delete [] NorganicDissolved;
          delete [] PorganicDissolved;
          delete [] CorganicDissolvedFlux;
          delete [] NorganicDissolvedFlux;
          delete [] PorganicDissolvedFlux;
          delete [] Mineralization_CorganicDissolved;
          delete [] Mineralization_NorganicDissolved;
          delete [] Mineralization_PorganicDissolved;
          delete [] Diffusion_CorganicDissolved;
          delete [] Diffusion_NorganicDissolved;
          delete [] Diffusion_PorganicDissolved;
          delete [] Dissolution_CSediments;
          delete [] Dissolution_NSediments;
          delete [] Dissolution_PSediments;
       }
       /*if (NumberOfVariables > 0) {
          delete [] VariableNameArray;
          NumberOfVariables = 0;
       } */
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
				cerr << "TSedimentsPlus::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TSedimentsPlus::GetParameterValue(char* MyParameter)
{
  double value;

    if (strcmp(MyParameter, "Kt") == 0)
    {
        value = Kt;
    }
    else if (strcmp(MyParameter, "KminO2") == 0)
    {
        value = KminO2;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForN") == 0)
    {
        value = DiffusionCoefficientForN;
    }
    else if (strcmp(MyParameter, "Ka1") == 0)
    {
        value = Ka1;
    }
    else if (strcmp(MyParameter, "Ka2") == 0)
    {
        value = Ka2;
    }
    else if (strcmp(MyParameter, "Pmax") == 0)
    {
        value = Pmax;
    }
    else if (strcmp(MyParameter, "O2thrs") == 0)
    {
        value = O2thrs;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForP") == 0)
    {
        value = DiffusionCoefficientForP;
    }
    else if (strcmp(MyParameter, "Kd") == 0)
    {
        value = Kd;
    }
    else if (strcmp(MyParameter, "minNs") == 0)
    {
        value = minNs;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
    {
        value = FractionMineralizedToAmmonia;
    }
    else if (strcmp(MyParameter, "minPs") == 0)
    {
        value = minPs;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToPhosphorus") == 0)
    {
        value = FractionMineralizedToPhosphorus;
    }
    else if (strcmp(MyParameter, "DiffusionLayer") == 0)
    {
        value = DiffusionLayer;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForO2") == 0)
    {
        value = DiffusionCoefficientForO2;
    }
    else if (strcmp(MyParameter, "knit") == 0)
    {
        value = knit;
    }
    else if (strcmp(MyParameter, "kdenit") == 0)
    {
        value = kdenit;
    }
    else if (strcmp(MyParameter, "knitO2") == 0)
    {
        value = knitO2;
    }
    else if (strcmp(MyParameter, "kdenitO2") == 0)
    {
        value = kdenitO2;
    }
    else if (strcmp(MyParameter, "minCs") == 0)
    {
        value = minCs;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForNInSediments") == 0)
    {
        value =  DiffusionCoefficientForNInSediments;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForPInSediments") == 0)
    {
        value = DiffusionCoefficientForPInSediments;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForO2InSediments") == 0)
    {
        value = DiffusionCoefficientForO2InSediments;
    }
    else if (strcmp(MyParameter, "NumberOfSedimentLayers") == 0)
    {
        value = NumberOfSedimentLayers;
    }
    else if (strcmp(MyParameter, "MinTopDepth") == 0)
    {
        value = MinTopDepth;
    }
    else if (strcmp(MyParameter, "MaxTopDepth") == 0)
    {
        value = MaxTopDepth;
    }
    else if (strcmp(MyParameter, "ReferenceDepth") == 0)
    {
        value = ReferenceDepth;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForNO3") == 0)
    {
        value = DiffusionCoefficientForNO3;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForNH4") == 0)
    {
        value = DiffusionCoefficientForNH4;
    }
    else if (strcmp(MyParameter, "DiffCoefficientForCorganicDissolved") == 0)
    {
        value = DiffCoeffForCorganicDissolved;
    }
    else if (strcmp(MyParameter, "DiffCoefficientForNorganicDissolved") == 0)
    {
        value = DiffCoeffForNorganicDissolved;
    }
    else if (strcmp(MyParameter, "DiffCoefficientForPorganicDissolved") == 0)
    {
        value = DiffCoeffForPorganicDissolved;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNO3InSediments") == 0)
    {
        value = DiffCoeffForNO3InSediments;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNH4InSediments") == 0)
    {
        value = DiffCoeffForNH4InSediments;
    }
    else if (strcmp(MyParameter, "DiffCoeffForCOrganicDissolvedInSediments") == 0)
    {
        value = DiffCoeffForCorganicDissolvedInSediments;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNOrganicDissolvedInSediments") == 0)
    {
        value = DiffCoeffForNorganicDissolvedInSediments;
    }
    else if (strcmp(MyParameter, "DiffCoeffForPOrganicDissolvedInSediments") == 0)
    {
        value = DiffCoeffForPorganicDissolvedInSediments;
    }
    else if (strcmp(MyParameter, "OrganicFractionThatDissolves") == 0)
    {
        value = OrganicFractionThatDissolves;
    }
    else if (strcmp(MyParameter, "minCd") == 0)
    {
        value = minCd;
    }
    else if (strcmp(MyParameter, "minNd") == 0)
    {
        value = minNd;
    }
    else if (strcmp(MyParameter, "minPd") == 0)
    {
        value = minPd;
    }
    else if (strcmp(MyParameter, "Ktd") == 0)
    {
        value = Ktd;
    }
    else if (strcmp(MyParameter, "KminO2d") == 0)
    {
        value = KminO2d;
    }
    else
        value = 0.0;
    return value;
}

bool TSedimentsPlus::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;

    if (strcmp(MyParameter, "Kt") == 0)
    {
        Kt = value;
    }
    else if (strcmp(MyParameter, "KminO2") == 0)
    {
        KminO2 = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForN") == 0)
    {
        DiffusionCoefficientForN = value;
    }
    else if (strcmp(MyParameter, "Ka1") == 0)
    {
        Ka1 = value;
    }
    else if (strcmp(MyParameter, "Ka2") == 0)
    {
        Ka2 = value;
    }
    else if (strcmp(MyParameter, "Pmax") == 0)
    {
        Pmax = value;
    }
    else if (strcmp(MyParameter, "O2thrs") == 0)
    {
        O2thrs = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForP") == 0)
    {
        DiffusionCoefficientForP = value;
    }
    else if (strcmp(MyParameter, "Kd") == 0)
    {
        Kd = value;
    }
    else if (strcmp(MyParameter, "minNs") == 0)
    {
        minNs = value;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
    {
        FractionMineralizedToAmmonia = value;
    }
    else if (strcmp(MyParameter, "minPs") == 0)
    {
        minPs = value;
    }
    else if (strcmp(MyParameter, "FractionMineralizedToPhosphorus") == 0)
    {
        FractionMineralizedToPhosphorus = value;
    }
    else if (strcmp(MyParameter, "DiffusionLayer") == 0)
    {
        DiffusionLayer = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForO2") == 0)
    {
        DiffusionCoefficientForO2 = value;
    }
    else if (strcmp(MyParameter, "knit") == 0)
    {
        knit = value;
    }
    else if (strcmp(MyParameter, "kdenit") == 0)
    {
        kdenit = value;
    }
    else if (strcmp(MyParameter, "knitO2") == 0)
    {
        knitO2 = value;
    }
    else if (strcmp(MyParameter, "kdenitO2") == 0)
    {
        kdenitO2 = value;
    }
    else if (strcmp(MyParameter, "minCs") == 0)
    {
        minCs = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForNInSediments") == 0)
    {
        DiffusionCoefficientForNInSediments = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForPInSediments") == 0)
    {
        DiffusionCoefficientForPInSediments = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForO2InSediments") == 0)
    {
        DiffusionCoefficientForO2InSediments = value;
    }
    else if (strcmp(MyParameter, "NumberOfSedimentLayers") == 0)
    {
        NumberOfSedimentLayers = value;
    }
    else if (strcmp(MyParameter, "MinTopDepth") == 0)
    {
        MinTopDepth = value;
    }
    else if (strcmp(MyParameter, "MaxTopDepth") == 0)
    {
        MaxTopDepth = value;
    }
    else if (strcmp(MyParameter, "ReferenceDepth") == 0)
    {
        ReferenceDepth = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForNO3") == 0)
    {
        DiffusionCoefficientForNO3 = value;
    }
    else if (strcmp(MyParameter, "DiffusionCoefficientForNH4") == 0)
    {
        DiffusionCoefficientForNH4 = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForCorganicDissolved") == 0)
    {
        DiffCoeffForCorganicDissolved = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNorganicDissolved") == 0)
    {
        DiffCoeffForNorganicDissolved = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForPorganicDissolved") == 0)
    {
        DiffCoeffForPorganicDissolved = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNO3InSediments") == 0)
    {
        DiffCoeffForNO3InSediments = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNH4InSediments") == 0)
    {
        DiffCoeffForNH4InSediments = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForCOrganicDissolvedInSediments") == 0)
    {
        DiffCoeffForCorganicDissolvedInSediments = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForNOrganicDissolvedInSediments") == 0)
    {
        DiffCoeffForNorganicDissolvedInSediments = value;
    }
    else if (strcmp(MyParameter, "DiffCoeffForPOrganicDissolvedInSediments") == 0)
    {
        DiffCoeffForPorganicDissolvedInSediments = value;
    }
    else if (strcmp(MyParameter, "OrganicFractionThatDissolves") == 0)
    {
        OrganicFractionThatDissolves = value;
    }
    else
        rc = false;

    return rc;
}

void TSedimentsPlus::Go()
{
   int index, ASedimentBoxNumber, i, j, m, Layer;
   char function[128];
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

 try {
   for (m = 0; m < NumberOfBoxesWithSediments; m++)
   {
      i = MySedimentsRecord[m].LineCoordinate;
      j = MySedimentsRecord[m].ColumnCoordinate;

      if (
            (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j] /*- 1*/) &&
            (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i] /*- 1*/)
          )
      {
         for (Layer = 0; Layer < NumberOfSedimentLayers; Layer++)
         {
            index = i * NumberOfColumns + j;
            ASedimentBoxNumber = Get3DIndex(i, j, Layer);
            if (LayerDepth[ASedimentBoxNumber] >= 0.9 * MinTopDepth)
            {
                strcpy(function, "BenthicNitrogenMineralization");
                BenthicNitrogenMineralization(ASedimentBoxNumber,m);
                strcpy(function, "BenthicPhosphorusMineralization");
                BenthicPhosphorusMineralization(ASedimentBoxNumber,m);
                strcpy(function, "BenthicCarbonMineralization");
                BenthicCarbonMineralization(ASedimentBoxNumber,m);
                strcpy(function, "AmmoniaSedimentWaterDiffusion");
                DiffusionCoefficientForN = DiffusionCoefficientForNH4;
                DiffusionCoefficientForNInSediments = DiffCoeffForNH4InSediments;
                AmmoniaSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "NitrateSedimentWaterDiffusion");
                DiffusionCoefficientForN = DiffusionCoefficientForNO3;
                DiffusionCoefficientForNInSediments = DiffCoeffForNO3InSediments;
                NitrateSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "Nitrification");
                Nitrification(ASedimentBoxNumber, m);
                strcpy(function, "DeNitrification");
                DeNitrification(ASedimentBoxNumber, m);
                strcpy(function, "PhosphorusSedimentWaterDiffusion");
                PhosphorusSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "OxygenSedimentWaterDiffusion");
                OxygenSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "PhosphorusAdsorption");
                PhosphorusAdsorption(ASedimentBoxNumber,m);
                strcpy(function, "PhosphorusDesorption");
                PhosphorusDesorption(ASedimentBoxNumber,m);
                strcpy(function, "BenthicDissolvedCarbonMineralization");
                BenthicDissolvedCarbonMineralization(ASedimentBoxNumber,m);
                strcpy(function, "BenthicDissolvedNitrogenMineralization");
                BenthicDissolvedNitrogenMineralization(ASedimentBoxNumber,m);
                strcpy(function, "BenthicDissolvedPhosphorusMineralization");
                BenthicDissolvedPhosphorusMineralization(ASedimentBoxNumber,m);
                strcpy(function, "CorganicDissolvedSedimentWaterDiffusion");
                CorganicDissolvedSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "NorganicDissolvedSedimentWaterDiffusion");
                NorganicDissolvedSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "PorganicDissolvedSedimentWaterDiffusion");
                PorganicDissolvedSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "DissolutionOfSedimentOrganics");
                DissolutionOfSedimentOrganics(ASedimentBoxNumber,m);
            }
         }
      }
   }
   //Debugger(2);
  }
#ifdef __BORLANDC__
  catch (EAccessViolation &e)
  {
    ShowMessage(e.Message+"("+function+", ASedimentBoxNumber: "+ASedimentBoxNumber
            +", box: "+m+", layer: "+Layer+") Go 1");
  }
  catch (Exception &exception) {
    ShowMessage(AnsiString(exception.ClassName())+ " "+exception.Message+"("
            +function+", ASedimentBoxNumber: "+ASedimentBoxNumber
            +", box: "+m+", layer: "+Layer+") Go 2");
  }
#else  // __BORLANDC__
  catch (exception &exc) {
    cerr << exc.what() << " (" << function<< ", ASedimentBoxNumber: " << ASedimentBoxNumber << ", box: " << m << ", layer: " << Layer << ") Go 2" << endl;
  }
#endif  // __BORLANDC__
}

void TSedimentsPlus::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    strcpy(MyParameter, ParameterName);
    LogMessage("Update", srcName, MyParameter, Value, MyBoxNumber);
    if (strcmp(MyParameter, "Norganic 1") == 0)
	    //NorganicFlux[MyBoxNumber] = NorganicFlux[MyBoxNumber] + Value / MySedimentsRecord[BoxNumber].SedimentWaterRatio;
            //"value" in mg N/m3/s = microg N/L/s, SedimentWaterRatio in g/L => result in micro microg/g/s
            NorganicInOut[MyBoxNumber] = NorganicInOut[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "NH4InPoreWater 1") == 0)
	    NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] + Value ;
    else if (strcmp(MyParameter, "Porganic 1") == 0)
	    //PorganicFlux[MyBoxNumber] = PorganicFlux[MyBoxNumber] + Value / MySedimentsRecord[BoxNumber].SedimentWaterRatio;
            PorganicInOut[MyBoxNumber] = PorganicInOut[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "PO4InPoreWater 1") == 0)
	    PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Padsorbed 1") == 0)
	    SorptionFlux[MyBoxNumber] = SorptionFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "O2InPoreWater 1") == 0)
	    O2InPoreWaterFlux[MyBoxNumber] = O2InPoreWaterFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "NO3InPoreWater 1") == 0)
	    NO3InPoreWaterFlux[MyBoxNumber] = NO3InPoreWaterFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "Corganic 1") == 0)
	    CorganicInOut[MyBoxNumber] = CorganicInOut[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "OrganicMass 1") == 0)
	    OrganicInOut[MyBoxNumber] = OrganicInOut[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "InorganicMass 1") == 0)
	    InorganicInOut[MyBoxNumber] = InorganicInOut[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "CorganicDissolved 1") == 0)
	    CorganicDissolvedFlux[MyBoxNumber] = CorganicDissolvedFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "NorganicDissolved 1") == 0)
	    NorganicDissolvedFlux[MyBoxNumber] = NorganicDissolvedFlux[MyBoxNumber] + Value;
    else if (strcmp(MyParameter, "PorganicDissolved 1") == 0)
	    PorganicDissolvedFlux[MyBoxNumber] = PorganicDissolvedFlux[MyBoxNumber] + Value;
    else
    {
#ifdef __BORLANDC__
    	static char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in this object 1");
    	MessageBox(0,Caption,"EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TSedimentsPlus::Update - " << MyParameter << " does not exist in this object" << endl;
#endif  // __BORLANDC__
    }
}

void TSedimentsPlus::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName, int AnObjectCode)
{
    char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber;
    int MyColumn = GetColumnIndex(MyBoxNumber);
    int MyLine = GetLineIndex(MyBoxNumber);
    strcpy(MyParameter, ParameterName);

    if (strcmp(MyParameter, "Corganic 1") == 0)
       Value = Corganic[MyBoxNumber];
    else if (strcmp(MyParameter, "Norganic 1") == 0)
       Value = Norganic[MyBoxNumber];
    else if (strcmp(MyParameter, "NH4InPoreWater 1") == 0)
       Value = NH4InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "Porganic 1") == 0)
       Value = Porganic[MyBoxNumber];
    else if (strcmp(MyParameter, "PO4InPoreWater 1") == 0)
       Value = PO4InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "Padsorbed 1") == 0)
       Value = Padsorbed[MyBoxNumber];
    else if (strcmp(MyParameter, "O2InPoreWater 1") == 0)
       Value = O2InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "Mineralization_C 1") == 0)
       Value = Mineralization_C[MyBoxNumber];
    else if (strcmp(MyParameter, "Mineralization_N 1") == 0)
       Value = Mineralization_N[MyBoxNumber];
    else if (strcmp(MyParameter, "Mineralization_P 1") == 0)
       Value = Mineralization_P[MyBoxNumber];
    else if (strcmp(MyParameter, "Mineralization_CorganicDissolved 1") == 0)
       Value = Mineralization_CorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Mineralization_NorganicDissolved 1") == 0)
       Value = Mineralization_NorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Mineralization_PorganicDissolved 1") == 0)
       Value = Mineralization_PorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_NH4 1") == 0)
       Value = Diffusion_NH4[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_NO3 1") == 0)
       Value = Diffusion_NO3[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_P 1") == 0)
       Value = Diffusion_P[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_CorganicDissolved 1") == 0)
       Value = Diffusion_CorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_NorganicDissolved 1") == 0)
       Value = Diffusion_NorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_PorganicDissolved 1") == 0)
       Value = Diffusion_PorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_O2 1") == 0)
       Value = Diffusion_O2[MyBoxNumber];
    else if (strcmp(MyParameter, "Adsorption_P 1") == 0)
       Value = Adsorption_P[MyBoxNumber];
    else if (strcmp(MyParameter, "Desorption_P 1") == 0)
       Value = Desorption_P[MyBoxNumber];
    else if (strcmp(MyParameter, "NO3InPoreWater 1") == 0)
       Value = NO3InPoreWater[MyBoxNumber];
    else if (strcmp(MyParameter, "Nitrification 1") == 0)
       Value = NitrificationFlux[MyBoxNumber];
    else if (strcmp(MyParameter, "DeNitrification 1") == 0)
      Value = DeNitrificationFlux[MyBoxNumber];
    else if (strcmp(MyParameter, "OrganicMass 1") == 0)
      Value = OrganicMass[MyBoxNumber];
    else if (strcmp(MyParameter, "InorganicMass 1") == 0)
      Value = InorganicMass[MyBoxNumber];
    else if (strcmp(MyParameter, "ReferenceHeight") == 0)
      Value = ReferenceHeight[MyBoxNumber];
    else if (strcmp(MyParameter, "LayerDepth 1") == 0)
      Value = LayerDepth[MyBoxNumber];
    else if (strcmp(MyParameter, "OrganicMass 1") == 0)
      Value = OrganicMass[MyBoxNumber];
    else if (strcmp(MyParameter, "CorganicDissolved 1") == 0)
      Value = CorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "NorganicDissolved 1") == 0)
      Value = NorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "PorganicDissolved 1") == 0)
      Value = PorganicDissolved[MyBoxNumber];
    else if (strcmp(MyParameter, "Dissolution_CSediments 1") == 0)
      Value = Dissolution_CSediments[MyBoxNumber];
    else if (strcmp(MyParameter, "Dissolution_NSediments 1") == 0)
      Value = Dissolution_NSediments[MyBoxNumber];
    else if (strcmp(MyParameter, "Dissolution_PSediments 1") == 0)
      Value = Dissolution_PSediments[MyBoxNumber];

    else if (strcmp(MyParameter, "Corganic 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Corganic[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Norganic 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Norganic[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "NH4InPoreWater 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = NH4InPoreWater[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Porganic 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Porganic[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "PO4InPoreWater 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = PO4InPoreWater[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Padsorbed 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Padsorbed[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "O2InPoreWater 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = O2InPoreWater[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Mineralization_C 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Mineralization_C[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Mineralization_N 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Mineralization_N[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Mineralization_P 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Mineralization_P[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Mineralization_CorganicDissolved 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Mineralization_CorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Mineralization_NorganicDissolved 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Mineralization_NorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Mineralization_PorganicDissolved 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Mineralization_PorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_NH4 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_NH4[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_NO3 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_NO3[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_P 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_P[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_CorganicDissolved 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_CorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_NorganicDissolved 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_NorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_PorganicDissolved 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_PorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Diffusion_O2 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Diffusion_O2[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Adsorption_P 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Adsorption_P[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Desorption_P 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = Desorption_P[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "NO3InPoreWater 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = NO3InPoreWater[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Nitrification 2") == 0)
    {
       MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
       Value = NitrificationFlux[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "DeNitrification 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = DeNitrificationFlux[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "OrganicMass 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = OrganicMass[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "InorganicMass 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = InorganicMass[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "LayerDepth 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = LayerDepth[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "CorganicDissolved 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = CorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "NorganicDissolved 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = NorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "PorganicDissolved 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = PorganicDissolved[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Dissolution_CSediments 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = Dissolution_CSediments[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Dissolution_NSediments 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = Dissolution_NSediments[MyBoxNumber];
    }
    else if (strcmp(MyParameter, "Dissolution_PSediments 2") == 0)
    {
      MyBoxNumber = Get3DIndex(MyLine, MyColumn, 1);
      Value = Dissolution_PSediments[MyBoxNumber];
    }
    else {
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
            strcat(Caption, " does not exist in this object");
            MessageBox(0,Caption,"EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TSedimentsPlus::Inquiry - " << MyParameter << " does not exist in this object" << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TSedimentsPlus::Integrate()
{
   double MyBoxDepth, Delta;
   int m, i,j, index, index2D, index0,index1,Layer;
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   double const TINNY = 0.00001;
   // Norganic  [micro g / g]
   try {
   for (m = 0; m < NumberOfBoxesWithSediments; m++)
   {
      //Debugger(m);
      i = MySedimentsRecord[m].LineCoordinate;
      j = MySedimentsRecord[m].ColumnCoordinate;
      //Debugger(i); Debugger(j);
      if (
            (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j] /*- 1*/) &&
            (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i] /*- 1*/)
          )
      {
         for (Layer = 0; Layer < NumberOfSedimentLayers; Layer++)
         {
            index = Get3DIndex(i, j, Layer);

            OrganicMassFlux[index] = OrganicMassFlux[index] / (CUBIC * CUBIC) * SedimentMass[index]; // g/m2/s
            OrganicMassFlux[index] = OrganicMassFlux[index] + OrganicInOut[index]/*mg/m2/s*/ / CUBIC; //g /m2 / s
            OrganicInOut[index] = 0.0;

            InorganicMassFlux[index] = InorganicMassFlux[index] + InorganicInOut[index]/*mg/m2/s*/ / CUBIC; //g /m2 / s
            InorganicInOut[index] = 0.0;

            if (Layer == 0)
            {
                //Debugger(index); Debugger(SedimentDensity[index]); Debugger(MySedimentsRecord[m].Porosity);
                if ((SedimentDensity[index] * CUBIC * CUBIC * (1 - MySedimentsRecord[m].Porosity)) == 0)
#ifdef __BORLANDC__
                     MessageBox(0, "Error 0 in TSediments::Integrate()", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
											cerr << "Error 0 in TSediments::Integrate()" << endl;
#endif  // __BORLANDC__

                ReferenceHeight[i * NumberOfColumns + j] = ReferenceHeight[i * NumberOfColumns + j] +
                        (InorganicMassFlux[index] + OrganicMassFlux[index]) /
                        (SedimentDensity[index] * CUBIC * CUBIC * (1 - MySedimentsRecord[m].Porosity))
                        * MyPEcoDynClass->GetTimeStep();
            }
         }

      }
   }
   }
#ifdef __BORLANDC__
   catch (EAccessViolation &e)
   {
      ShowMessage(e.Message+"(box: "+m+",layer: "+Layer+",index: "+index+") Integrate 1");
   }
   catch (Exception &exception) {
      ShowMessage(AnsiString(exception.ClassName())+ " "+exception.Message+"(box: "+m+",layer: "+Layer+",index: "+index+") Integrate 2");
   }
#else  // __BORLANDC__
   catch (exception &exc) {
      cerr << exc.what() << " (box: " << m << ",layer: " << Layer << ",index: "  << index << ") Integrate 1/2" << endl;
   }
#endif  // __BORLANDC__
   try {
       Generic = NH4InPoreWater; GenericFlux = NH4InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       NH4InPoreWater = Generic; NH4InPoreWaterFlux = GenericFlux;
       // PO4InPoreWater
       Generic = PO4InPoreWater; GenericFlux = PO4InPoreWaterFlux;
       //Debugger(10); Debugger(Generic[0]); Debugger(GenericFlux[0]);
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       PO4InPoreWater = Generic; PO4InPoreWaterFlux = GenericFlux;


       //Debugger(11); Debugger(Generic[0]);
       // O2InPoreWater
       Generic = O2InPoreWater; GenericFlux = O2InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       O2InPoreWater = Generic; O2InPoreWaterFlux = GenericFlux;
       // NO3InPoreWater
       Generic = NO3InPoreWater; GenericFlux = NO3InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       NO3InPoreWater = Generic; NO3InPoreWaterFlux = GenericFlux;

       Generic = OrganicMass; GenericFlux = OrganicMassFlux; //Debugger(OrganicMassFlux[0]); Debugger(OrganicMass[0]);
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       OrganicMass = Generic; OrganicMassFlux = GenericFlux;

       Generic = InorganicMass; GenericFlux = InorganicMassFlux; //Debugger(InorganicMassFlux[0]); Debugger(InorganicMass[0]);
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       InorganicMass = Generic; InorganicMassFlux = GenericFlux;

       Generic = CorganicDissolved; GenericFlux = CorganicDissolvedFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       CorganicDissolved = Generic; CorganicDissolvedFlux = GenericFlux;

       Generic = NorganicDissolved; GenericFlux = NorganicDissolvedFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       NorganicDissolved = Generic; NorganicDissolvedFlux = GenericFlux;

       Generic = PorganicDissolved; GenericFlux = PorganicDissolvedFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       PorganicDissolved = Generic; PorganicDissolvedFlux = GenericFlux;
   }
#ifdef __BORLANDC__
   catch (EAccessViolation &e)
   {
      ShowMessage(e.Message+"(Integration) Integrate 5");
   }
   catch (Exception &exception) {
      ShowMessage(AnsiString(exception.ClassName())+ " "+exception.Message+"(Integration) Integrate 6");
   }
#else  // __BORLANDC__
   catch (exception &exc) {
      cerr << exc.what() << " (Integration) Integrate 5/6" << endl;
   }
#endif  // __BORLANDC__
   double PastSedimentMass;
   for (m = 0; m < NumberOfBoxesWithSediments; m++)
   {
      //Debugger(m);
      i = MySedimentsRecord[m].LineCoordinate;
      j = MySedimentsRecord[m].ColumnCoordinate;
      //Debugger(i); Debugger(j);
      if (
            (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
            (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
          )
      {
         for (Layer = 0; Layer < NumberOfSedimentLayers; Layer++)
         {
            index = Get3DIndex(i, j, Layer);
            PastSedimentMass = SedimentMass[index];
            //Debugger(index); Debugger(SedimentMass[index]); Debugger(InorganicMass[index]); Debugger(OrganicMass[index]);
            SedimentMass[index] = InorganicMass[index] + OrganicMass[index];
            if (SedimentMass[index] > TINNY)
            {
                Corganic[index] = Corganic[index]/* * PastSedimentMass / SedimentMass[index]*/ +
                                  (CorganicFlux[index] + CorganicInOut[index] * CUBIC / SedimentMass[index]) * TimeStep; //micro g / g / timestep
                CorganicFlux[index] = 0.0;
                CorganicInOut[index] = 0.0;

                Norganic[index] = Norganic[index]/* * PastSedimentMass / SedimentMass[index]*/ +
                                  (NorganicFlux[index] + NorganicInOut[index] * CUBIC / SedimentMass[index]) * TimeStep; //micro g / g / timestep
                NorganicFlux[index] = 0.0;
                NorganicInOut[index] = 0.0;

                Porganic[index] = Porganic[index]/* * PastSedimentMass / SedimentMass[index]*/ +
                                  (PorganicFlux[index] + PorganicInOut[index] * CUBIC / SedimentMass[index]) * TimeStep; //micro g / g / timestep
                PorganicFlux[index] = 0.0;
                PorganicInOut[index] = 0.0;

                Padsorbed[index] = Padsorbed[index] * PastSedimentMass / SedimentMass[index] + SorptionFlux[index] * TimeStep; //micro g / g / timestep
                SorptionFlux[index] = 0.0;
            }
            else
            {
                CorganicFlux[index] = 0.0;
                CorganicInOut[index] = 0.0;
                NorganicFlux[index] = 0.0;
                NorganicInOut[index] = 0.0;
                PorganicFlux[index] = 0.0;
                PorganicInOut[index] = 0.0;
                SorptionFlux[index] = 0.0;
            }
         }
      }
   }


   try {
   for (m = 0; m < NumberOfBoxesWithSediments; m++)
   {
      //Debugger(m);
      i = MySedimentsRecord[m].LineCoordinate;
      j = MySedimentsRecord[m].ColumnCoordinate;
      if (
            (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j] /*- 1*/) &&
            (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i] /*- 1*/)
          )
      {

         index2D = i * NumberOfColumns + j;

         for (Layer = 0; Layer < NumberOfSedimentLayers; Layer++)
         {
                index = Get3DIndex(i, j, Layer);

                //Corganic[index] = OrganicMass[index] / (CarbonToOrganics * SedimentMass[index] / (CUBIC * CUBIC));

                if (SedimentMass[index] > TINNY){
                        SedimentDensity[index] = (MySedimentsRecord[m].Density * InorganicMass[index] + OrganicMatterDensity * OrganicMass[index]) /
                                                   SedimentMass[index]; 
                }
                if (Corganic[index] < 0.0) Corganic[index] = 0.0;
                if (Norganic[index] < 0.0) Norganic[index] = 0.0;
                if (Porganic[index] < 0.0) Porganic[index] = 0.0;
                if (NH4InPoreWater[index] < 0.0) NH4InPoreWater[index] = 0.0;
                if (PO4InPoreWater[index] < 0.0) PO4InPoreWater[index] = 0.0;
                if (Padsorbed[index] < 0.0) Padsorbed[index] = 0.0;
                if (O2InPoreWater[index] < 0.0) O2InPoreWater[index] = 0.0;
                if (NO3InPoreWater[index] < 0.0) NO3InPoreWater[index] = 0.0;
                if (OrganicMass[index] < 0.0) OrganicMass[index] = 0.0;
                if (InorganicMass[index] < 0.0) InorganicMass[index] = 0.0;
                if (CorganicDissolved[index] < 0.0) CorganicDissolved[index] = 0.0;
                if (NorganicDissolved[index] < 0.0) NorganicDissolved[index] = 0.0;
                if (PorganicDissolved[index] < 0.0) PorganicDissolved[index] = 0.0;
                if (Layer == 0)
                {
                    if (SedimentMass[index] > 0.1)  {
                     LayerDepth[index] =  SedimentMass[index] /
                                          (
                                             SedimentDensity[index] * CUBIC *  CUBIC  *   //density converted to g/m3
                                            (1 - MySedimentsRecord[m].Porosity)
                                          );
                    }
                    else
                     LayerDepth[index] = 0.0;
                }
                if ((Corganic[index]  > TINNY) && (SedimentMass[index] > TINNY))
                   CarbonToOrganics[index] = OrganicMass[index] / (Corganic[index] / (CUBIC * CUBIC) * SedimentMass[index]);
                else CarbonToOrganics[index] = 0.0;
         }
         if ((NumberOfSedimentLayers > 1) && (LayerDepth[Get3DIndex(i, j, 0)] < MinTopDepth))
         {
            index0 = Get3DIndex(i, j, 0);
            index1 = Get3DIndex(i, j, 1);
            Delta = (ReferenceDepth - LayerDepth[index0]);
            if (Delta < LayerDepth[index1])
            {
            if (LayerDepth[index0]+ Delta <= 0.0) 
#ifdef __BORLANDC__
							MessageBox(0, "Error 1 in TSediments::Integrate()", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
							cerr << "Error 1 in TSediments::Integrate()" << endl;
#endif  // __BORLANDC__
            Corganic[index0] = (Corganic[index0] * LayerDepth[index0] + Corganic[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            Norganic[index0] = (Norganic[index0] * LayerDepth[index0] + Norganic[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            Porganic[index0] = (Porganic[index0] * LayerDepth[index0] + Porganic[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            NH4InPoreWater[index0] = (NH4InPoreWater[index0] * LayerDepth[index0] + NH4InPoreWater[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            PO4InPoreWater[index0] = (PO4InPoreWater[index0] * LayerDepth[index0] + PO4InPoreWater[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            Padsorbed[index0] = (Padsorbed[index0] * LayerDepth[index0] + Padsorbed[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            O2InPoreWater[index0] = (O2InPoreWater[index0] * LayerDepth[index0] + O2InPoreWater[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            NO3InPoreWater[index0] = (NO3InPoreWater[index0] * LayerDepth[index0] + NO3InPoreWater[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            CorganicDissolved[index0] = (CorganicDissolved[index0] * LayerDepth[index0] + CorganicDissolved[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            NorganicDissolved[index0] = (NorganicDissolved[index0] * LayerDepth[index0] + NorganicDissolved[index1] * Delta) /
                       (LayerDepth[index0] + Delta);
            PorganicDissolved[index0] = (PorganicDissolved[index0] * LayerDepth[index0] + PorganicDissolved[index1] * Delta) /
                       (LayerDepth[index0] + Delta);

            SedimentMass[index0] = SedimentMass[index0] + SedimentMass[index1] * Delta / LayerDepth[index1];
            InorganicMass[index0] = InorganicMass[index0] + InorganicMass[index1] * Delta / LayerDepth[index1];
            //Debugger(111); Debugger(InorganicMass[index0]);
            //OrganicMass[index0] = Corganic[index0] * CarbonToOrganics * /*InorganicMass*/SedimentMass[index0] / (CUBIC * CUBIC); // g/m2
            OrganicMass[index0] = MAX(0.0,SedimentMass[index0] - InorganicMass[index0]);
            if ((Corganic[index0]  > TINNY) && (SedimentMass[index0] > TINNY))
                CarbonToOrganics[index0] = OrganicMass[index0] / (Corganic[index0] / (CUBIC * CUBIC) * SedimentMass[index0]);
            else CarbonToOrganics[index0] = 0.0;

            LayerDepth[index1] = LayerDepth[index1] - Delta;
            if (LayerDepth[index1] <= 0.0) 
#ifdef __BORLANDC__
							MessageBox(0, "Error 2 in TSediments::Integrate()", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
							cerr << "Error 2 in TSediments::Integrate()" << endl;
#endif  // __BORLANDC__
            LayerDepth[index0] = ReferenceDepth;

            SedimentMass[index1] = SedimentMass[index1] - SedimentMass[index1] * Delta / LayerDepth[index1];
            InorganicMass[index1] = InorganicMass[index1] - InorganicMass[index1] * Delta / LayerDepth[index1];
            //OrganicMass[index1] = Corganic[index1] * CarbonToOrganics * /*InorganicMass*/SedimentMass[index1] / (CUBIC * CUBIC); // g/m2
            OrganicMass[index1] = MAX(0.0,SedimentMass[index1] - InorganicMass[index1]);
            if ((Corganic[index1]  > TINNY) && (SedimentMass[index1] > TINNY))
                CarbonToOrganics[index1] = OrganicMass[index1] / (Corganic[index1] / (CUBIC * CUBIC) * SedimentMass[index1]);
            else CarbonToOrganics[index1] = 0.0;

            }
         }
         if ((NumberOfSedimentLayers > 1) && (LayerDepth[Get3DIndex(i, j, 0)] > MaxTopDepth))
         {

            index0 = Get3DIndex(i, j, 0);
            index1 = Get3DIndex(i, j, 1);
            Delta = (LayerDepth[index0] - ReferenceDepth);
            if (LayerDepth[index1]+ Delta <= 0.0) 
#ifdef __BORLANDC__
							MessageBox(0, "Error 3 in TSediments::Integrate()", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
							cerr << "Error 3 in TSediments::Integrate()" << endl;
#endif  // __BORLANDC__
            Corganic[index1] = (Corganic[index0] * Delta + Corganic[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            Norganic[index1] = (Norganic[index0] * Delta + Norganic[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            Porganic[index1] = (Porganic[index0] * Delta + Porganic[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            NH4InPoreWater[index1] = (NH4InPoreWater[index0] * Delta + NH4InPoreWater[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            PO4InPoreWater[index1] = (PO4InPoreWater[index0] * Delta + PO4InPoreWater[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            Padsorbed[index1] = (Padsorbed[index0] * Delta + Padsorbed[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            O2InPoreWater[index1] = (O2InPoreWater[index0] * Delta + O2InPoreWater[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            NO3InPoreWater[index1] = (NO3InPoreWater[index0] * Delta + NO3InPoreWater[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            CorganicDissolved[index1] = (CorganicDissolved[index0] * Delta + CorganicDissolved[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            NorganicDissolved[index1] = (NorganicDissolved[index0] * Delta + NorganicDissolved[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);
            PorganicDissolved[index1] = (PorganicDissolved[index0] * Delta + PorganicDissolved[index1] * LayerDepth[index1]) /
                               (Delta + LayerDepth[index1]);

            SedimentMass[index1] = SedimentMass[index1] + SedimentMass[index0] * Delta / LayerDepth[index0];
            InorganicMass[index1] = InorganicMass[index1] + InorganicMass[index0] * Delta / LayerDepth[index0];
            //OrganicMass[index1] = Corganic[index1] * CarbonToOrganics * /*InorganicMass*/SedimentMass[index1] / (CUBIC * CUBIC);
            OrganicMass[index1] = MAX(0.0,SedimentMass[index1] - InorganicMass[index1]);
            if ((Corganic[index1]  > TINNY) && (SedimentMass[index1] > TINNY))
               CarbonToOrganics[index1] = OrganicMass[index1] / (Corganic[index1] / (CUBIC * CUBIC) * SedimentMass[index1]);
            else CarbonToOrganics[index1] = 0.0;

            LayerDepth[index1] = LayerDepth[index1] + Delta;
            if (LayerDepth[index1] <= 0.0) 
#ifdef __BORLANDC__
							MessageBox(0, "Error 4 in TSediments::Integrate()", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
							cerr << "Error 4 in TSediments::Integrate()" << endl;
#endif  // __BORLANDC__
            LayerDepth[index0] = ReferenceDepth;

            SedimentMass[index0] = SedimentMass[index0] - SedimentMass[index0] * Delta / LayerDepth[index0];
            InorganicMass[index0] = InorganicMass[index0] - InorganicMass[index0] * Delta / LayerDepth[index0];
            //Debugger(222); Debugger(InorganicMass[index0]);
            //OrganicMass[index0] = Corganic[index0] * CarbonToOrganics * /*InorganicMass*/SedimentMass[index0] / (CUBIC * CUBIC);
            OrganicMass[index0] = MAX(0.0,SedimentMass[index0] - InorganicMass[index0]);
            if ((Corganic[index0]  > TINNY) && (SedimentMass[index0] > TINNY))
               CarbonToOrganics[index0] = OrganicMass[index0] / (Corganic[index0] / (CUBIC * CUBIC) * SedimentMass[index0]);
            else CarbonToOrganics[index0] = 0.0;
         }
      }
   }
   }
#ifdef __BORLANDC__
   catch (EAccessViolation &e)
   {
      ShowMessage(e.Message+"(box: "+m+",layer: "+Layer+",i: "+i+",j: "+j+") Integrate 3");
   }
   catch (Exception &exception) {
      ShowMessage(AnsiString(exception.ClassName())+ " "+exception.Message+"(box: "+m+",layer: "+Layer+",i: "+i+",j: "+j+") Integrate 4");
   }
#else  // __BORLANDC__
   catch (exception &exc) {
      cerr << exc.what() << " (box: " << m << ",layer: " << Layer << ",i: " << i << ",j: " << j << ") Integrate 3/4" << endl;
   }
#endif  // __BORLANDC__
}

bool TSedimentsPlus::SetVariableValue(char* srcName, double Value,
            int BoxNumber,char* VariableName)
{
    bool rc = true;
    int MyColumn = GetColumnIndex(BoxNumber);
    int MyLine = GetLineIndex(BoxNumber);

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

    if (strcmp(VariableName, "Corganic 1") == 0)
       Corganic[BoxNumber] = Value;
    else if (strcmp(VariableName, "Norganic 1") == 0)
       Norganic[BoxNumber] = Value;
    else if (strcmp(VariableName, "NH4InPoreWater 1") == 0)
       NH4InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "Porganic 1") == 0)
       Porganic[BoxNumber] = Value;
    else if (strcmp(VariableName, "PO4InPoreWater 1") == 0)
       PO4InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "Padsorbed 1") == 0)
       Padsorbed[BoxNumber] = Value;
    else if (strcmp(VariableName, "O2InPoreWater 1") == 0)
       O2InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "Mineralization_C 1") == 0)
       Mineralization_C[BoxNumber] = Value;
    else if (strcmp(VariableName, "Mineralization_N 1") == 0)
       Mineralization_N[BoxNumber] = Value;
    else if (strcmp(VariableName, "Mineralization_P 1") == 0)
       Mineralization_P[BoxNumber] = Value;
    else if (strcmp(VariableName, "Mineralization_CorganicDissolved 1") == 0)
       Mineralization_CorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Mineralization_NorganicDissolved 1") == 0)
       Mineralization_NorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Mineralization_PorganicDissolved 1") == 0)
       Mineralization_PorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_NH4 1") == 0)
       Diffusion_NH4[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_NO3 1") == 0)
       Diffusion_NO3[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_P 1") == 0)
       Diffusion_P[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_CorganicDissolved 1") == 0)
       Diffusion_CorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_NorganicDissolved 1") == 0)
       Diffusion_NorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_PorganicDissolved 1") == 0)
       Diffusion_PorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_O2 1") == 0)
       Diffusion_O2[BoxNumber] = Value;
    else if (strcmp(VariableName, "Adsorption_P 1") == 0)
       Adsorption_P[BoxNumber] = Value;
    else if (strcmp(VariableName, "Desorption_P 1") == 0)
       Desorption_P[BoxNumber] = Value;
    else if (strcmp(VariableName, "NO3InPoreWater 1") == 0)
       NO3InPoreWater[BoxNumber] = Value;
    else if (strcmp(VariableName, "Nitrification 1") == 0)
       NitrificationFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "DeNitrification 1") == 0)
       DeNitrificationFlux[BoxNumber] = Value;
    else if (strcmp(VariableName, "OrganicMass 1") == 0)
       OrganicMass[BoxNumber] = Value;
    else if (strcmp(VariableName, "InorganicMass 1") == 0)
       InorganicMass[BoxNumber] = Value;
    else if (strcmp(VariableName, "ReferenceHeight") == 0)
       ReferenceHeight[BoxNumber] = Value;
    else if (strcmp(VariableName, "LayerDepth 1") == 0)
       LayerDepth[BoxNumber] = Value;
    else if (strcmp(VariableName, "CorganicDissolved 1") == 0)
       CorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "NorganicDissolved 1") == 0)
       NorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "PorganicDissolved 1") == 0)
       PorganicDissolved[BoxNumber] = Value;
    else if (strcmp(VariableName, "Corganic 2") == 0)
    {
       Corganic[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Norganic 2") == 0)
    {
       Norganic[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "NH4InPoreWater 2") == 0)
    {
       NH4InPoreWater[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Porganic 2") == 0)
    {
       Porganic[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "PO4InPoreWater 2") == 0)
    {
       PO4InPoreWater[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Padsorbed 2") == 0)
    {
       Padsorbed[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "O2InPoreWater 2") == 0)
    {
       O2InPoreWater[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Mineralization_C 2") == 0)
    {
       Mineralization_C[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Mineralization_N 2") == 0)
    {
       Mineralization_N[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Mineralization_P 2") == 0)
    {
       Mineralization_P[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Mineralization_CorganicDissolved 2") == 0)
    {
       Mineralization_CorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Mineralization_NorganicDissolved 2") == 0)
    {
       Mineralization_NorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Mineralization_PorganicDissolved 2") == 0)
    {
       Mineralization_PorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_NH4 2") == 0)
    {
       Diffusion_NH4[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_NO3 2") == 0)
    {
       Diffusion_NO3[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_P 2") == 0)
    {
       Diffusion_P[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_CorganicDissolved 2") == 0)
    {
       Diffusion_CorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_NorganicDissolved 2") == 0)
    {
       Diffusion_NorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_PorganicDissolved 2") == 0)
    {
       Diffusion_PorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Diffusion_O2 2") == 0)
    {
       Diffusion_O2[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Adsorption_P 2") == 0)
    {
       Adsorption_P[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Desorption_P 2") == 0)
    {
       Desorption_P[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "NO3InPoreWater 2") == 0)
    {
       NO3InPoreWater[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "Nitrification 2") == 0)
    {
       NitrificationFlux[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "DeNitrification 2") == 0)
    {
       DeNitrificationFlux[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "OrganicMass 2") == 0)
    {
       OrganicMass[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "InorganicMass 2") == 0)
    {
       InorganicMass[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "LayerDepth 2") == 0)
    {
       LayerDepth[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "CorganicDissolved 2") == 0)
    {
       CorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "NorganicDissolved 2") == 0)
    {
       NorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else if (strcmp(VariableName, "PorganicDissolved 2") == 0)
    {
       PorganicDissolved[Get3DIndex(MyLine, MyColumn, 1)] = Value;
    }
    else
        rc = false;

    return rc;
}

bool TSedimentsPlus::SaveVariables()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveVariablesFile("Sediments");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Sediments");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfVariables, 0);

    /*
     * save variables' names
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
    if ((NumberOfSedimentLayers > 0) && (NumberOfBoxesWithSediments > 0))
    {
        PReadWrite->WriteCell("LayerDepth 1 values");
        PReadWrite->WriteSeparator(true);
        for (int i = 0; i < NumberOfSedimentLayers; i++)
        {
            PReadWrite->WriteCell(
                    LayerDepth[Get3DIndex(MySedimentsRecord[0].LineCoordinate,
                            MySedimentsRecord[0].ColumnCoordinate, i)], 6);
            PReadWrite->WriteSeparator(true);
        }
        PReadWrite->WriteSeparator(true);
    }
    if (NumberOfBoxesWithSediments > 0)
    {
        int Index;

        PReadWrite->WriteCell("Norganic 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("NH4InPoreWater 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Porganic 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("PO4InPoreWater 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("O2InPoreWater 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Padsorbed 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("NO3InPoreWater 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("Corganic 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("CorganicDissolved 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("NorganicDissolved 1 values");
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell("PorganicDissolved 1 values");
        PReadWrite->WriteSeparator(true);
        for (int i = 0; i < NumberOfBoxesWithSediments; i++)
        {
            Index = MySedimentsRecord[i].LineCoordinate * NumberOfColumns
                    + MySedimentsRecord[i].ColumnCoordinate;
            PReadWrite->WriteCell(Norganic[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(NH4InPoreWater[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Porganic[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(PO4InPoreWater[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(O2InPoreWater[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Padsorbed[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(NO3InPoreWater[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(Corganic[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(CorganicDissolved[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(NorganicDissolved[Index], 6);
            PReadWrite->WriteSeparator();
            PReadWrite->WriteCell(PorganicDissolved[Index], 6);
            PReadWrite->WriteSeparator(true);
        }
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}
bool TSedimentsPlus::SaveParameters()
{
	TReadWrite* PReadWrite = (TReadWrite*)SaveParametersFile("Sediments");
    if (PReadWrite == NULL)
    {
        return false;
    }

    /*
     * header file
     */
    PReadWrite->WriteCell("Sediments");
    PReadWrite->WriteSeparator();
    PReadWrite->WriteCell(NumberOfParameters, 0);

    /*
     * save parameters' names
     */
    for (int i = 0; i < NumberOfParameters; i++)
    {
        PReadWrite->WriteSeparator();
        PReadWrite->WriteCell(ParametersNameArray[i]);
    }
    PReadWrite->WriteSeparator(true);

    /*
     * save parameters' values
     */
    PReadWrite->WriteSeparator();
    for (int i = 0; i < NumberOfParameters; i++)
    {
        PReadWrite->WriteSeparator();
        if (strcmp(ParametersNameArray[i], "Kt") == 0)
            PReadWrite->WriteCell(Kt, 6);
        else if (strcmp(ParametersNameArray[i], "KminO2") == 0)
            PReadWrite->WriteCell(KminO2, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForN") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForN, 6);
        else if (strcmp(ParametersNameArray[i], "Ka1") == 0)
            PReadWrite->WriteCell(Ka1, 6);
        else if (strcmp(ParametersNameArray[i], "Ka2") == 0)
            PReadWrite->WriteCell(Ka2, 6);
        else if (strcmp(ParametersNameArray[i], "Pmax") == 0)
            PReadWrite->WriteCell(Pmax, 6);
        else if (strcmp(ParametersNameArray[i], "O2thrs") == 0)
            PReadWrite->WriteCell(O2thrs, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForP") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForP, 6);
        else if (strcmp(ParametersNameArray[i], "Kd") == 0)
            PReadWrite->WriteCell(Kd, 6);
        else if (strcmp(ParametersNameArray[i], "minNs") == 0)
            PReadWrite->WriteCell(minNs, 6);
        else if (strcmp(ParametersNameArray[i], "FractionMineralizedToAmmonia") == 0)
            PReadWrite->WriteCell(FractionMineralizedToAmmonia, 6);
        else if (strcmp(ParametersNameArray[i], "minPs") == 0)
            PReadWrite->WriteCell(minPs, 6);
        else if (strcmp(ParametersNameArray[i], "FractionMineralizedToPhosphorus") == 0)
            PReadWrite->WriteCell(FractionMineralizedToPhosphorus, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionLayer") == 0)
            PReadWrite->WriteCell(DiffusionLayer, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForO2") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForO2, 6);
        else if (strcmp(ParametersNameArray[i], "knit") == 0)
            PReadWrite->WriteCell(knit, 6);
        else if (strcmp(ParametersNameArray[i], "kdenit") == 0)
            PReadWrite->WriteCell(kdenit, 6);
        else if (strcmp(ParametersNameArray[i], "knitO2") == 0)
            PReadWrite->WriteCell(knitO2, 6);
        else if (strcmp(ParametersNameArray[i], "kdenitO2") == 0)
            PReadWrite->WriteCell(kdenitO2, 6);
        else if (strcmp(ParametersNameArray[i], "minCs") == 0)
            PReadWrite->WriteCell(minCs, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForNInSediments") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForNInSediments, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForPInSediments") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForPInSediments, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForO2InSediments") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForO2InSediments, 6);
        else if (strcmp(ParametersNameArray[i], "NumberOfSedimentLayers") == 0)
            PReadWrite->WriteCell(NumberOfSedimentLayers, 6);
        else if (strcmp(ParametersNameArray[i], "MinTopDepth") == 0)
            PReadWrite->WriteCell(MinTopDepth, 6);
        else if (strcmp(ParametersNameArray[i], "MaxTopDepth") == 0)
            PReadWrite->WriteCell(MaxTopDepth, 6);
        else if (strcmp(ParametersNameArray[i], "ReferenceDepth") == 0)
            PReadWrite->WriteCell(ReferenceDepth, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForNO3") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForNO3, 6);
        else if (strcmp(ParametersNameArray[i], "DiffusionCoefficientForNH4") == 0)
            PReadWrite->WriteCell(DiffusionCoefficientForNH4, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForCorganicDissolved") == 0)
            PReadWrite->WriteCell(DiffCoeffForCorganicDissolved, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForNorganicDissolved") == 0)
            PReadWrite->WriteCell(DiffCoeffForNorganicDissolved, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForPorganicDissolved") == 0)
            PReadWrite->WriteCell(DiffCoeffForPorganicDissolved, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForNO3InSediments") == 0)
            PReadWrite->WriteCell(DiffCoeffForNO3InSediments, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForNH4InSediments") == 0)
            PReadWrite->WriteCell(DiffCoeffForNH4InSediments, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForCOrganicDissolvedInSediments") == 0)
            PReadWrite->WriteCell(DiffCoeffForCorganicDissolvedInSediments, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForNOrganicDissolvedInSediments") == 0)
            PReadWrite->WriteCell(DiffCoeffForNorganicDissolvedInSediments, 6);
        else if (strcmp(ParametersNameArray[i], "DiffCoeffForPOrganicDissolvedInSediments") == 0)
            PReadWrite->WriteCell(DiffCoeffForPorganicDissolvedInSediments, 6);
        else if (strcmp(ParametersNameArray[i], "OrganicFractionThatDissolves") == 0)
            PReadWrite->WriteCell(OrganicFractionThatDissolves, 6);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);

    return true;
}

void TSedimentsPlus::ReadParameters(char* className)
{
    int i,X,Y;
    bool SedimentRecord;
    DiffusionCoefficientForNO3 = 0.0; DiffusionCoefficientForNH4 = 0.0;
    DiffCoeffForCorganicDissolved = 0.0;
    DiffCoeffForNorganicDissolved = 0.0;
    DiffCoeffForPorganicDissolved = 0.0;
    DiffCoeffForNO3InSediments = 0.0; DiffCoeffForNH4InSediments = 0.0;
    DiffCoeffForCorganicDissolvedInSediments = 0.0;
    DiffCoeffForNorganicDissolvedInSediments = 0.0;
    DiffCoeffForPorganicDissolvedInSediments = 0.0;
    OrganicFractionThatDissolves = 0.0;
    strcpy(EcoDynClassName, className);
    SedimentRecord = MyPEcoDynClass->HasASedimentsRecord();
    if (!SedimentRecord)
    {
       if (MyPEcoDynClass->OpenSediments(MyPEcoDynClass->GetSedimentsFile()))
       SedimentRecord = true;
    }
    if (SedimentRecord)
    {
         MySedimentsRecord = MyPEcoDynClass->GetASedimentsRecord();
         NumberOfBoxesWithSediments = MyPEcoDynClass->GetNumberOfCellsWithSediments();
    	 TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Sediments");
         if (PReadWrite == NULL)
         {
#ifdef __BORLANDC__
            MessageBox(0, "Sediments parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSedimentsPlus::ReadParameters - parameters file missing." << endl;
#endif  // __BORLANDC__
        }
        else {
             if (PReadWrite->FindString("Sediments", X, Y))
             {
                PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
                ParametersNameArray = new VNA[NumberOfParameters];

                char MyParameter[65];
                for (int i = 0; i < NumberOfParameters; i++)
                {
                    PReadWrite->ReadString(X+2+i, Y, MyParameter);
                    strcpy(ParametersNameArray[i], MyParameter);
                    if (strcmp(MyParameter, "DiffusionCoefficientForNO3") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForNO3);
                        //Debugger(DiffusionCoefficientForNO3);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForNH4") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForNH4);
                        //Debugger(DiffusionCoefficientForNH4);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForCorganicDissolved") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForCorganicDissolved);
                        //Debugger(DiffCoeffForCorganicDissolved);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForNorganicDissolved") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForNorganicDissolved);
                        //Debugger(DiffCoeffForNorganicDissolved);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForPorganicDissolved") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForPorganicDissolved);
                        //Debugger(DiffCoeffForPorganicDissolved);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForNO3InSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForNO3InSediments);
                        //Debugger(DiffCoeffForNO3InSediments);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForNH4InSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForNH4InSediments);
                        //Debugger(DiffCoeffForNH4InSediments);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForCOrganicDissolvedInSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForCorganicDissolvedInSediments);
                        //Debugger(DiffCoeffForCorganicDissolvedInSediments);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForNOrganicDissolvedInSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForNorganicDissolvedInSediments);
                        //Debugger(DiffCoeffForNorganicDissolvedInSediments);
                    }
                    else if (strcmp(MyParameter, "DiffCoeffForPorganicDissolvedInSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffCoeffForPorganicDissolvedInSediments);
                        //Debugger(DiffCoeffForPorganicDissolvedInSediments);
                    }
                    else if (strcmp(MyParameter, "OrganicFractionThatDissolves") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, OrganicFractionThatDissolves);
                        //Debugger(OrganicFractionThatDissolves);
                    }
                    else if (strcmp(MyParameter, "minCd") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, minCd);
                        //Debugger(minCd);
                    }
                    else if (strcmp(MyParameter, "minNd") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, minNd);
                        //Debugger(minNd);
                    }
                    else if (strcmp(MyParameter, "minPd") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, minPd);
                        //Debugger(minPd);
                    }
                    else if (strcmp(MyParameter, "Ktd") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, Ktd);
                        //Debugger(Ktd);
                    }
                    else if (strcmp(MyParameter, "KminO2d") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, KminO2d);
                        //Debugger(KminO2d);
                    }
                }
            }
            else
            {
#ifdef __BORLANDC__
                    MessageBox(0, "Parameters - Undefined object in Ria Formosa Sediments", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
										cerr << "TSedimentsPlus::ReadParameters - Undefined object in Ria Formosa sediments" << endl;
#endif  // __BORLANDC__
            }
            CloseDataFile((void*)PReadWrite);
        }
        InitializeVariables();
        ReadVariables(className);
    }
    else
    {
        MySedimentsRecord = NULL;
        NumberOfBoxesWithSediments = 0;
    }
}


void TSedimentsPlus::InitializeVariables()
{
    int NumberOfSedimentCells;
    if (NumberOfBoxes > 0)
    {
       NumberOfSedimentCells = NumberOfBoxes * NumberOfSedimentLayers;
       CorganicDissolved = new double[NumberOfSedimentCells];
       NorganicDissolved = new double[NumberOfSedimentCells];
       PorganicDissolved = new double[NumberOfSedimentCells];
       CorganicDissolvedFlux = new double[NumberOfSedimentCells];
       NorganicDissolvedFlux = new double[NumberOfSedimentCells];
       PorganicDissolvedFlux = new double[NumberOfSedimentCells];
       Mineralization_CorganicDissolved = new double[NumberOfSedimentCells];
       Mineralization_NorganicDissolved = new double[NumberOfSedimentCells];
       Mineralization_PorganicDissolved = new double[NumberOfSedimentCells];
       Diffusion_CorganicDissolved = new double[NumberOfSedimentCells];
       Diffusion_NorganicDissolved = new double[NumberOfSedimentCells];
       Diffusion_PorganicDissolved = new double[NumberOfSedimentCells];
	}
	else
#ifdef __BORLANDC__
      MessageBox(0,
                "Sediments object array not dimensioned",
                "EcoDynamo alert",
                MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TSedimentsPlus::InitializeVariables - sediments object array not dimensioned" << endl;
#endif  // __BORLANDC__
    for (int i = 0; i < NumberOfSedimentCells; i++)
    {
        CorganicDissolved[i] = 0.0;
        NorganicDissolved[i] = 0.0;
        PorganicDissolved[i] = 0.0;
        CorganicDissolvedFlux[i] = 0.0;
        NorganicDissolvedFlux[i] = 0.0;
        PorganicDissolvedFlux[i] = 0.0;
        Mineralization_CorganicDissolved[i] = 0.0;
        Mineralization_NorganicDissolved[i] = 0.0;
        Mineralization_PorganicDissolved[i] = 0.0;
        Diffusion_CorganicDissolved[i] = 0.0;
        Diffusion_NorganicDissolved[i] = 0.0;
        Diffusion_PorganicDissolved[i] = 0.0;
    }

}

void TSedimentsPlus::ReadVariables(char* className)
{
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Sediments");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "Sediments variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSedimentsPlus::ReadVariables - variables file missing" << endl;
#endif  // __BORLANDC__
	}
    else {
        char MyVariable[65];
        int X, Y, Index, XV, YV;
        if (PReadWrite->FindString("Sediments", X, Y))
        {
           PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
           // Create array for state variable names
           VariableNameArray = new VNA[NumberOfVariables];
           // read in the variable names
           for (int i = 0; i < NumberOfVariables; i++)
           {
              PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
              strcpy(VariableNameArray[i], MyVariable);

              if (strcmp(MyVariable, "CorganicDissolved 1") == 0)
              {
                 if (!PReadWrite->FindString("CorganicDissolved 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    //Debugger(j);
                    //Debugger(MySedimentsRecord[j].LineCoordinate); Debugger(NumberOfColumns); Debugger(MySedimentsRecord[j].ColumnCoordinate);
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, CorganicDissolved[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       CorganicDissolved[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          CorganicDissolved[Index];
                 }
                 //Debugger(Index);
                 //Debugger(NorganicDissolved[Index]);
              }
              if (strcmp(MyVariable, "NorganicDissolved 1") == 0)
              {
                 if (!PReadWrite->FindString("NorganicDissolved 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    //Debugger(j);
                    //Debugger(MySedimentsRecord[j].LineCoordinate); Debugger(NumberOfColumns); Debugger(MySedimentsRecord[j].ColumnCoordinate);
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, NorganicDissolved[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       NorganicDissolved[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          NorganicDissolved[Index];
                 }
                 //Debugger(Index);
                 //Debugger(NorganicDissolved[Index]);
              }
              else if (strcmp(MyVariable, "PorganicDissolved 1") == 0)
              {
                 if (!PReadWrite->FindString("PorganicDissolved 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                   Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                   PReadWrite->ReadNumber(XV, YV + 1 + j, PorganicDissolved[Index]);
                   for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       PorganicDissolved[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                       PorganicDissolved[Index];
                 }
              }

           }
        }
        else 
#ifdef __BORLANDC__
					MessageBox(0, "Variables - Undefined object in sediments", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TSedimentsPlus::ReadVariables - Undefined object in sediments" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }
}

void TSedimentsPlus::BenthicDissolvedCarbonMineralization(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber, WaterBoxNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   double MyMineralization;
   Mineralization_CorganicDissolved[MyBoxNumber] = minCd * CorganicDissolved[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber)* OxygenLimitation(MyBoxNumber, KminO2d);  //micro mol L-1 d-1
   CorganicDissolvedFlux[MyBoxNumber] = CorganicDissolvedFlux[MyBoxNumber] - Mineralization_CorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;                          //micro mol L-1 s-1
   MyMineralization = Mineralization_CorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;  //micro mol N-organic L-1 s-1
}

void TSedimentsPlus::BenthicDissolvedNitrogenMineralization(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber, WaterBoxNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   double MyMineralization;
   double OxygenNitrogenRatio = 0.212; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (2000) - Ecolog. Modell 127: 161-181)
   Mineralization_NorganicDissolved[MyBoxNumber] = minNd * NorganicDissolved[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber)* OxygenLimitation(MyBoxNumber, KminO2d);  //micro mol L-1 d-1
   //Debugger(Mineralization_NorganicDissolved[MyBoxNumber]); Debugger(minNd); Debugger(NorganicDissolved[MyBoxNumber]);
   //Debugger(TemperatureLimitation(WaterBoxNumber)); Debugger(OxygenLimitation(MyBoxNumber, KminO2d));
   NorganicDissolvedFlux[MyBoxNumber] = NorganicDissolvedFlux[MyBoxNumber] - Mineralization_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;                          //micro mol L-1 s-1
   MyMineralization = Mineralization_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;  //micro mol N-organic L-1 s-1
   NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] + MyMineralization * FractionMineralizedToAmmonia;  //micro mol N-NH4 L-1 s-1
   O2InPoreWaterFlux[MyBoxNumber] = O2InPoreWaterFlux[MyBoxNumber] - MyMineralization * OxygenNitrogenRatio; //mg O2 L-1 s-1
}


void TSedimentsPlus::BenthicDissolvedPhosphorusMineralization(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber, WaterBoxNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   Mineralization_PorganicDissolved[MyBoxNumber] = minPd * PorganicDissolved[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber) * OxygenLimitation(MyBoxNumber, KminO2);  //micro g g-1 d-1
   //Debugger(5);
   //Debugger(Mineralization_PorganicDissolved[MyBoxNumber]);
   //Debugger(PO4InPoreWaterFlux[MyBoxNumber] *DAYSTOSECONDS);
   PorganicDissolvedFlux[MyBoxNumber] = PorganicDissolvedFlux[MyBoxNumber] - Mineralization_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;                          //micro g g-1 s-1
   PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] +
                                     Mineralization_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;  //micro mol P L-1 s-1
   //Debugger(PO4InPoreWaterFlux[MyBoxNumber] *DAYSTOSECONDS);
}

void TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer)
{
   int MyBoxNumber, MySedimentRecordNumber, MyWaterColumnBoxNumber, MyColumn, MyLine;
   MyWaterColumnBoxNumber = AWaterColumnBoxNumber;
   MyBoxNumber = ASedimentBoxNumber;
   MySedimentRecordNumber = ASedimentRecordNumber;
   double meters_to_dcm = 10.0;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyNutrientPointer != NULL)
   {
      double CorganicDissolvedInWater, Depth;
      if (MyTransportPointer != NULL)
      {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                             MyWaterColumnBoxNumber,
                                             "Box depth",
                                             ObjectCode);
      }
      else Depth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
      if ((Depth > CriticalWaterDepth) && (ALayer == 0))
      {
        if (LayerDepth[MyBoxNumber] <= 0.0)
        {
#ifdef __BORLANDC__
                MessageBox(0, "Error 1 in TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
								cerr << "Error 1 in TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
                Debugger(LayerDepth[MyBoxNumber]); Debugger(MyBoxNumber);
        }
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), CorganicDissolvedInWater,
                                             MyWaterColumnBoxNumber,
                                             "DOC",
                                             ObjectCode);

        Diffusion_CorganicDissolved[MyBoxNumber] = DiffCoeffForCorganicDissolved / pow (meters_to_dcm,2) * (CorganicDissolved[MyBoxNumber] - CorganicDissolvedInWater) /
                                     ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol CN L-1 d-1
        CorganicDissolvedFlux[MyBoxNumber] = CorganicDissolvedFlux[MyBoxNumber] - Diffusion_CorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;//micro mol C L-1 s-1
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  Diffusion_CorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth * CUBIC,
                                  MyBoxNumber,
                                  "DOC",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        if (LayerDepth[MyBoxNumber] <= 0.0) 
#ifdef __BORLANDC__
					MessageBox(0, "Error 2 in TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Error 2 in TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__

        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);

        if (LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] <= 0.0)
        {
#ifdef __BORLANDC__
           MessageBox(0, "Error 3 in TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Error 3 in TSedimentsPlus::CorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
           Debugger(MyLine); Debugger(MyColumn); Debugger(ALayer); Debugger(MyBoxNumber); Debugger(Get3DIndex(MyLine, MyColumn, ALayer - 1));
        }
        Diffusion_CorganicDissolved[MyBoxNumber] = DiffCoeffForCorganicDissolvedInSediments / pow (meters_to_dcm,2) *
                                     (CorganicDissolved[MyBoxNumber] - CorganicDissolved[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol C L-1 d-1
        CorganicDissolvedFlux[MyBoxNumber] = CorganicDissolvedFlux[MyBoxNumber] - Diffusion_CorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;//micro mol C L-1 s-1
        CorganicDissolvedFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                CorganicDissolvedFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_CorganicDissolved[MyBoxNumber] *
                LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] / DAYSTOSECONDS;//micro mol C L-1 s-1

      }
   }
}

void TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer)
{
   int MyBoxNumber, MySedimentRecordNumber, MyWaterColumnBoxNumber, MyColumn, MyLine;
   MyWaterColumnBoxNumber = AWaterColumnBoxNumber;
   MyBoxNumber = ASedimentBoxNumber;
   MySedimentRecordNumber = ASedimentRecordNumber;
   double meters_to_dcm = 10.0;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyNutrientPointer != NULL)
   {
      double NorganicDissolvedInWater, Depth;
      if (MyTransportPointer != NULL)
      {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                             MyWaterColumnBoxNumber,
                                             "Box depth",
                                             ObjectCode);
      }
      else Depth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
      if ((Depth > CriticalWaterDepth) && (ALayer == 0))
      {
        if (LayerDepth[MyBoxNumber] <= 0.0)
        {
#ifdef __BORLANDC__
                MessageBox(0, "Error 1 in TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
								cerr << "Error 1 in TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
                Debugger(LayerDepth[MyBoxNumber]); Debugger(MyBoxNumber);
        }
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), NorganicDissolvedInWater,
                                             MyWaterColumnBoxNumber,
                                             "DON",
                                             ObjectCode);

        Diffusion_NorganicDissolved[MyBoxNumber] = DiffCoeffForNorganicDissolved / pow (meters_to_dcm,2) * (NorganicDissolved[MyBoxNumber] - NorganicDissolvedInWater) /
                                     ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        //Debugger(Diffusion_NorganicDissolved[MyBoxNumber]);
        NorganicDissolvedFlux[MyBoxNumber] = NorganicDissolvedFlux[MyBoxNumber] - Diffusion_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        //Debugger(Diffusion_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  Diffusion_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth * CUBIC,
                                  MyBoxNumber,
                                  "DON",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        if (LayerDepth[MyBoxNumber] <= 0.0) 
#ifdef __BORLANDC__
					MessageBox(0, "Error 2 in TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "Error 2 in TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__

        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);

        if (LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] <= 0.0)
        {
#ifdef __BORLANDC__
           MessageBox(0, "Error 3 in TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Error 3 in TSedimentsPlus::NorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
           Debugger(MyLine); Debugger(MyColumn); Debugger(ALayer); Debugger(MyBoxNumber); Debugger(Get3DIndex(MyLine, MyColumn, ALayer - 1));
        }
        Diffusion_NorganicDissolved[MyBoxNumber] = DiffCoeffForNorganicDissolvedInSediments / pow (meters_to_dcm,2) *
                                     (NorganicDissolved[MyBoxNumber] - NorganicDissolved[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        //Debugger(Diffusion_NorganicDissolved[MyBoxNumber]);
        NorganicDissolvedFlux[MyBoxNumber] = NorganicDissolvedFlux[MyBoxNumber] - Diffusion_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        NorganicDissolvedFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                NorganicDissolvedFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_NorganicDissolved[MyBoxNumber] *
                LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] / DAYSTOSECONDS;//micro mol N L-1 s-1
        //Debugger(Diffusion_NorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);

      }
   }
}

void TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer)
{
   int MyBoxNumber, MySedimentRecordNumber, MyWaterColumnBoxNumber, MyColumn, MyLine;
   MyWaterColumnBoxNumber = AWaterColumnBoxNumber;
   MyBoxNumber = ASedimentBoxNumber;
   MySedimentRecordNumber = ASedimentRecordNumber;
   double meters_to_dcm = 10.0;
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyNutrientPointer != NULL)
   {
      double PorganicDissolvedInWater, Depth;
      if (MyTransportPointer != NULL)
      {
        MyTransportPointer->Inquiry(GetEcoDynClassName(), Depth,
                                             MyWaterColumnBoxNumber,
                                             "Box depth",
                                             ObjectCode);
      }
      else Depth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
      if ((Depth > CriticalWaterDepth) && (ALayer == 0))
      {
        if (LayerDepth[MyBoxNumber] <= 0.0)
        {
#ifdef __BORLANDC__
                MessageBox(0, "Error 1 in TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
								cerr << "Error 1 in TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
                Debugger(LayerDepth[MyBoxNumber]); Debugger(MyBoxNumber);
        }
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), PorganicDissolvedInWater,
                                             MyWaterColumnBoxNumber,
                                             "DOP",
                                             ObjectCode);

        Diffusion_PorganicDissolved[MyBoxNumber] = DiffCoeffForPorganicDissolved / pow (meters_to_dcm,2) * (PorganicDissolved[MyBoxNumber] - PorganicDissolvedInWater) /
                                     ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol P L-1 d-1
        //Debugger(Diffusion_PorganicDissolved[MyBoxNumber]);
        PorganicDissolvedFlux[MyBoxNumber] = PorganicDissolvedFlux[MyBoxNumber] - Diffusion_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;//micro mol P L-1 s-1
        //Debugger(Diffusion_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  Diffusion_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth * CUBIC,
                                  MyBoxNumber,
                                  "DOP",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        if (LayerDepth[MyBoxNumber] <= 0.0) 
#ifdef __BORLANDC__
					MessageBox(0, "Error 2 in TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "Error 2 in TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__

        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);

        if (LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] <= 0.0)
        {
#ifdef __BORLANDC__
           MessageBox(0, "Error 3 in TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "Error 3 in TSedimentsPlus::PorganicDissolvedSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
           Debugger(MyLine); Debugger(MyColumn); Debugger(ALayer); Debugger(MyBoxNumber); Debugger(Get3DIndex(MyLine, MyColumn, ALayer - 1));
        }
        Diffusion_PorganicDissolved[MyBoxNumber] = DiffCoeffForPorganicDissolvedInSediments / pow (meters_to_dcm,2) *
                                     (PorganicDissolved[MyBoxNumber] - PorganicDissolved[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol P L-1 d-1
        //Debugger(Diffusion_PorganicDissolved[MyBoxNumber]);
        PorganicDissolvedFlux[MyBoxNumber] = PorganicDissolvedFlux[MyBoxNumber] - Diffusion_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS;//micro mol P L-1 s-1
        PorganicDissolvedFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                PorganicDissolvedFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_PorganicDissolved[MyBoxNumber] *
                LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]/ DAYSTOSECONDS;//micro mol P L-1 s-1
        //Debugger(Diffusion_PorganicDissolved[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);

      }
   }
}

void TSedimentsPlus::DissolutionOfSedimentOrganics(int ABoxNumber, int ASedimentRecordNumber)
//Kittiwanich (2007). Analysis of phosphorus and nitrogen cycles in the estuarine ecosystem of Hiroshima Bay by pelagic and benthic coupled model.
//Estuarine Coastal and Shelf Science, 75: 189-204. (cf. Tabela 3)
{
   int MyBoxNumber, MySedimentRecordNumber;
   double Dissolution, MyOrganicFractionThatDissolves;
   MyBoxNumber = ABoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;

   MyOrganicFractionThatDissolves = OrganicFractionThatDissolves * TemperatureLimitation(MyBoxNumber);

   Dissolution =  MyOrganicFractionThatDissolves * Corganic[MyBoxNumber]; //micro g C  g-1 d-1
   Dissolution_CSediments[MyBoxNumber] = Dissolution;
   CorganicFlux[MyBoxNumber] = CorganicFlux[MyBoxNumber] - Dissolution / DAYSTOSECONDS; //micro g C  g-1 s-1
   CorganicDissolvedFlux[MyBoxNumber] = CorganicDissolvedFlux[MyBoxNumber] + Dissolution / DAYSTOSECONDS //micro C g g-1 s-1
                                        / CARBONATOMICWEIGHT * MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio; //micro mol C L-1 s-1

   Dissolution = MyOrganicFractionThatDissolves * Norganic[MyBoxNumber]; //micro g N  g-1 d-1
   Dissolution_NSediments[MyBoxNumber] = Dissolution;
   NorganicFlux[MyBoxNumber] = NorganicFlux[MyBoxNumber] - Dissolution / DAYSTOSECONDS; //micro g N  g-1 s-1
   NorganicDissolvedFlux[MyBoxNumber] = NorganicDissolvedFlux[MyBoxNumber] + Dissolution / DAYSTOSECONDS //micro N g g-1 s-1
                                        / NITROGENATOMICWEIGHT * MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio; //micro mol N L-1 s-1

   Dissolution = MyOrganicFractionThatDissolves * Porganic[MyBoxNumber]; //micro g P  g-1 d-1
   Dissolution_PSediments[MyBoxNumber] = Dissolution;
   PorganicFlux[MyBoxNumber] = PorganicFlux[MyBoxNumber] - Dissolution / DAYSTOSECONDS; //micro g P  g-1 s-1
   PorganicDissolvedFlux[MyBoxNumber] = PorganicDissolvedFlux[MyBoxNumber] + Dissolution / DAYSTOSECONDS //micro P g g-1 s-1
                                        / PHOSPHORUSATOMICWEIGHT * MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio; //micro mol P L-1 s-1
}

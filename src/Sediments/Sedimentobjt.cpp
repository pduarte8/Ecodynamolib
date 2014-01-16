// SPM object CPP code file
// @ Pedro Duarte
// REV A - 2005.03.06
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

TSediments::TSediments(TEcoDynClass* PEcoDynClass, char* className) :TEcoDynClass()
{
    // Receive pointer to integrate
    MyPEcoDynClass = PEcoDynClass;
    ReadParameters(className);
}



void TSediments::ReadParameters(char* className)
{
    int i,j, X,Y, Index;
    bool SedimentRecord;
    NumberOfSedimentLayers = 1; ReferenceDepth = 0.0; MaxTopDepth = 0.0; MinTopDepth = 0.0;
    //CarbonToOrganics = 2.8;  //Calculated from the stochiometric ratio of organic matter (CH2O)106(NH3)16(H3PO4) Chapelle et al. / Ecol. Modell. 127 (2000) 161-181
    OrganicMatterDensity = 1.0;
    MineralDensity = 2.3;
    strcpy(EcoDynClassName, className);
    NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
    NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
    TimeStep = MyPEcoDynClass->GetTimeStep();
    //CriticalWaterDepth = 0.05; //m
    CriticalWaterDepth = MyPEcoDynClass->GetCriticalDepth();
    DiffusionLayer = 0.01;  //m
    knit = 0.2; /*d-1*/kdenit = 0.3; /*d-1*/ knitO2 = 4; /*mg L-1*/ kdenitO2 = 2; /*mg L-1*/ Kt = 0.07;
    ProportionOfNH4FromDenitrification = 1.0; //(Chapelle et al. (1995) - Ecolog. Modell 80: 131-147) - Remaining 0.6 is lost as N2
	// Get array size
	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
    BoxArray = MyPEcoDynClass->BoxArray;
	ObjectCode = SEDIMENTOBJECTCODE; 
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
         //Debugger(NumberOfBoxesWithSediments);
         // Initialise pointers
//         TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
    	 TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Sediments");
         if (PReadWrite == NULL)
         {
#ifdef __BORLANDC__
            MessageBox(0, "Sediments parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
						cerr << "TSediments::ReadParameters - parameters file missing." << endl;
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

                    if (strcmp(MyParameter, "Kt") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, Kt);
                        //Debugger(Kt);
                    }
                    else if (strcmp(MyParameter, "KminO2") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, KminO2);
                        //Debugger(KminO2);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForN") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForN);
                        //Debugger(DiffusionCoefficientForN);
                    }
                    else if (strcmp(MyParameter, "Ka1") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, Ka1);
                        //Debugger(Ka1);
                    }
                    else if (strcmp(MyParameter, "Ka2") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, Ka2);
                        //Debugger(Ka2);
                    }
                    else if (strcmp(MyParameter, "Pmax") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, Pmax);
                        //Debugger(Pmax);
                    }
                    else if (strcmp(MyParameter, "O2thrs") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, O2thrs);
                        //Debugger(O2thrs);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForP") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForP);
                        //Debugger(DiffusionCoefficientForP);
                    }
                    else if (strcmp(MyParameter, "Kd") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, Kd);
                        //Debugger(Kd);
                    }
                    else if (strcmp(MyParameter, "minNs") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, minNs);
                        //Debugger(minNs);
                    }
                    else if (strcmp(MyParameter, "FractionMineralizedToAmmonia") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, FractionMineralizedToAmmonia);
                        //Debugger(FractionMineralizedToAmmonia);
                    }
                    else if (strcmp(MyParameter, "minPs") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, minPs);
                        //Debugger(minPs);
                    }
                    else if (strcmp(MyParameter, "FractionMineralizedToPhosphorus") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, FractionMineralizedToPhosphorus);
                        //Debugger(FractionMineralizedToPhosphorus);
                    }
                    else if (strcmp(MyParameter, "DiffusionLayer") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionLayer);
                        //Debugger(DiffusionLayer);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForO2") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForO2);
                        //Debugger(DiffusionCoefficientForO2);
                    }
                    else if (strcmp(MyParameter, "knit") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, knit);
                        //Debugger(knit);
                    }
                    else if (strcmp(MyParameter, "kdenit") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, kdenit);
                        //Debugger(kdenit);
                    }
                    else if (strcmp(MyParameter, "knitO2") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, knitO2);
                        //Debugger(knitO2);
                    }
                    else if (strcmp(MyParameter, "kdenitO2") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, kdenitO2);
                        //Debugger(kdenitO2);
                    }
                    else if (strcmp(MyParameter, "minCs") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, minCs);
                        //Debugger(minCs);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForNInSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForNInSediments);
                        //Debugger(DiffusionCoefficientForNInSediments);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForPInSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForPInSediments);
                        //Debugger(DiffusionCoefficientForPInSediments);
                    }
                    else if (strcmp(MyParameter, "DiffusionCoefficientForO2InSediments") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, DiffusionCoefficientForO2InSediments);
                        //Debugger(DiffusionCoefficientForO2InSediments);
                    }
                    else if (strcmp(MyParameter, "NumberOfSedimentLayers") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, NumberOfSedimentLayers);
                        //Debugger(NumberOfSedimentLayers);
                    }
                    else if (strcmp(MyParameter, "MinTopDepth") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, MinTopDepth);
                        //Debugger(MinTopDepth);
                    }
                    else if (strcmp(MyParameter, "MaxTopDepth") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, MaxTopDepth);
                        //Debugger(MaxTopDepth);
                    }
                    else if (strcmp(MyParameter, "ReferenceDepth") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, ReferenceDepth);
                        //Debugger(ReferenceDepth);
                    }
                    else if (strcmp(MyParameter, "ProportionOfNH4FromDenitrification") == 0)
                    {
                        PReadWrite->ReadNumber(X + 2 + i, Y + 1, ProportionOfNH4FromDenitrification);
                        //Debugger(ProportionOfNH4FromDenitrification);
                    }
                }
            }
            else
            {
#ifdef __BORLANDC__
                    MessageBox(0, "Parameters - Undefined object in Ria Formosa Sediments", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
										cerr << "TSediments::ReadParameters - Undefined object in Ria Formosa Sediments" << endl;
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
    if (MaxTopDepth < MinTopDepth) 
#ifdef __BORLANDC__
			MessageBox(0, "Error 1 in TSediments::ReadParameters", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TSediments::ReadParameters - Error 1" << endl;
#endif  // __BORLANDC__
    if (ReferenceDepth < MinTopDepth) 
#ifdef __BORLANDC__
			MessageBox(0, "Error 2 in TSediments::ReadParameters", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TSediments::ReadParameters - Error 2" << endl;
#endif  // __BORLANDC__
    if (ReferenceDepth > MaxTopDepth) 
#ifdef __BORLANDC__
			MessageBox(0, "Error 3 in TSediments::ReadParameters", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TSediments::ReadParameters - Error 3" << endl;
#endif  // __BORLANDC__
}


void TSediments::InitializeVariables()
{
        int NumberOfSedimentCells;
        if (NumberOfBoxes > 0)
        {
               NumberOfSedimentCells = NumberOfBoxes * NumberOfSedimentLayers;
               Corganic = new double[NumberOfSedimentCells];
               Mineralization_C = new double[NumberOfSedimentCells];
               CorganicFlux = new double[NumberOfSedimentCells];
               Norganic = new double[NumberOfSedimentCells];
               NH4InPoreWater = new double[NumberOfSedimentCells];
               NO3InPoreWater = new double[NumberOfSedimentCells];
               Porganic = new double[NumberOfSedimentCells];
               PO4InPoreWater = new double[NumberOfSedimentCells];
               O2InPoreWater = new double[NumberOfSedimentCells];
               Padsorbed = new double[NumberOfSedimentCells];
               NorganicFlux = new double[NumberOfSedimentCells];
               NH4InPoreWaterFlux = new double[NumberOfSedimentCells];
               PorganicFlux = new double[NumberOfSedimentCells];
               PO4InPoreWaterFlux = new double[NumberOfSedimentCells];
               O2InPoreWaterFlux = new double[NumberOfSedimentCells];
               NO3InPoreWaterFlux = new double[NumberOfSedimentCells];
               Mineralization_N = new double[NumberOfSedimentCells];
               Mineralization_P = new double[NumberOfSedimentCells];
               Adsorption_P = new double[NumberOfSedimentCells];
               Desorption_P = new double[NumberOfSedimentCells];
               Diffusion_NH4 =  new double[NumberOfSedimentCells];
               Diffusion_NO3 =  new double[NumberOfSedimentCells];
               Diffusion_P = new double[NumberOfSedimentCells];
               Diffusion_O2 = new double[NumberOfSedimentCells];
               SorptionFlux = new double[NumberOfSedimentCells];
               NitrificationFlux = new double[NumberOfSedimentCells];
               DeNitrificationFlux = new double[NumberOfSedimentCells];
               CorganicInOut = new double[NumberOfSedimentCells];
               NorganicInOut = new double[NumberOfSedimentCells];
               PorganicInOut = new double[NumberOfSedimentCells];
               OrganicMass = new double[NumberOfSedimentCells];
               InorganicMass = new double[NumberOfSedimentCells];
               OrganicMassFlux = new double[NumberOfSedimentCells];
               InorganicMassFlux = new double[NumberOfSedimentCells];
               OrganicInOut = new double[NumberOfSedimentCells];
               InorganicInOut = new double[NumberOfSedimentCells];
               ReferenceHeight = new double[NumberOfBoxes];
               LayerDepth = new double[NumberOfSedimentCells];
               SedimentMass = new double[NumberOfSedimentCells];
               SedimentDensity = new double[NumberOfSedimentCells];
               // Create generics
//               Generic     = new double[NumberOfSedimentCells];
//               GenericLoad = new double[NumberOfSedimentCells];
//               GenericFlux = new double[NumberOfSedimentCells];
               CarbonToOrganics = new double[NumberOfSedimentCells];
	}
	else
#ifdef __BORLANDC__
		    MessageBox(0,
					  "Sediments object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
				cerr << "TSediments::InitializeVariables - sediments object array not dimensioned" << endl;
#endif  // __BORLANDC__
        for (int i = 0; i < NumberOfSedimentCells; i++)
        {
            Corganic[i] = 0.0;
            CorganicFlux[i] = 0.0;
            Norganic[i] = 0.0;
            NH4InPoreWater[i] = 0.0;
            NO3InPoreWater[i] = 0.0;
            Porganic[i] = 0.0;
            PO4InPoreWater[i] = 0.0;
            O2InPoreWater[i] = 0.0;
            Padsorbed[i] = 0.0;
            NorganicFlux[i] = 0.0;
            NH4InPoreWaterFlux[i] = 0.0;
            PorganicFlux[i] = 0.0;
            PO4InPoreWaterFlux[i] = 0.0;
            O2InPoreWaterFlux[i] = 0.0;
            NO3InPoreWaterFlux[i] = 0.0;
            Mineralization_C[i] = 0.0;
            Mineralization_N[i] = 0.0;
            Mineralization_P[i] = 0.0;
            Adsorption_P[i] = 0.0;
            Desorption_P[i] = 0.0;
            Diffusion_NH4[i] = 0.0;
            Diffusion_NO3[i] = 0.0;
            Diffusion_P[i] = 0.0;
            Diffusion_O2[i] = 0.0;
            SorptionFlux[i] = 0.0;
            NitrificationFlux[i] = 0.0;
            DeNitrificationFlux[i] = 0.0;
            CorganicInOut[i]=0.0;
            NorganicInOut[i] = 0.0;
            PorganicInOut[i] = 0.0;
            OrganicMass[i] = 0.0;
            InorganicMass[i] = 0.0;
            OrganicMassFlux[i] = 0.0;
            InorganicMassFlux[i] = 0.0;
            OrganicInOut[i] = 0.0;
            InorganicInOut[i] = 0.0;
            SedimentMass[i] = 0.0;
            SedimentDensity[i] = 0.0;
//            Generic[i] = 0.0;
//            GenericLoad[i] = 0.0;
//            GenericFlux[i] = 0.0;
            if (i < NumberOfBoxes)
                ReferenceHeight[i] = 0.0;
            LayerDepth[i] = 0.0;
            CarbonToOrganics[i] = 2.8;  //Calculated from the stochiometric ratio of organic matter (CH2O)106(NH3)16(H3PO4) Chapelle et al. / Ecol. Modell. 127 (2000) 161-181
        }

}


void TSediments::ReadVariables(char* className)
{
//    TReadWrite*PReadWrite = (TReadWrite*)MyPEcoDynClass->GetVarsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenVariablesFile("Sediments");
    if (PReadWrite == NULL)
    {
#ifdef __BORLANDC__
		MessageBox(0, "Sediments variables file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TSediments::ReadVariables - Variables file missing." << endl;
#endif  // __BORLANDC__
	}
    else {
        char MyVariable[65];
        int X, Y, Index, XV, YV;
        if (PReadWrite->FindString("Sediments", X, Y))
        {
           PReadWrite->ReadNumber(X+1, Y, NumberOfVariables);
           //Debugger(NumberOfVariables);
           // Create array for state variable names
           VariableNameArray = new VNA[NumberOfVariables];
           // read in the variable names
           for (int i = 0; i < NumberOfVariables; i++)
           {
              //Debugger(i); Debugger(NumberOfBoxesWithSediments);
              PReadWrite->ReadString(X + 2 + i, Y, MyVariable);
              strcpy(VariableNameArray[i], MyVariable);
          
              if (strcmp(MyVariable, "Norganic 1") == 0)
              {
                 if (!PReadWrite->FindString("Norganic 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    //Debugger(j);
                    //Debugger(MySedimentsRecord[j].LineCoordinate); Debugger(NumberOfColumns); Debugger(MySedimentsRecord[j].ColumnCoordinate);
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, Norganic[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       Norganic[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          Norganic[Index];
                 }
                 //Debugger(Index);
                 //Debugger(Norganic[Index]);
              }
              else if (strcmp(MyVariable, "NH4InPoreWater 1") == 0)
              {
                 if (!PReadWrite->FindString("NH4InPoreWater 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                   Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                   PReadWrite->ReadNumber(XV, YV + 1 + j, NH4InPoreWater[Index]);
                   for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       NH4InPoreWater[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          NH4InPoreWater[Index];
                 }
                 //Debugger(NH4InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "Porganic 1") == 0)
              {
                 if (!PReadWrite->FindString("Porganic 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, Porganic[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       Porganic[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          Porganic[Index];

                 }
                 //Debugger(Porganic[Index]);
              }
              else if (strcmp(MyVariable, "PO4InPoreWater 1") == 0)
              {
                 if (!PReadWrite->FindString("PO4InPoreWater 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, PO4InPoreWater[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       PO4InPoreWater[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          PO4InPoreWater[Index];

                 }
                 //Debugger(PO4InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "O2InPoreWater 1") == 0)
              {
                 if (!PReadWrite->FindString("O2InPoreWater 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, O2InPoreWater[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       O2InPoreWater[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          O2InPoreWater[Index];

                 }
                 //Debugger(Index);
                 //Debugger(O2InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "Padsorbed 1") == 0)
              {
                 if (!PReadWrite->FindString("Padsorbed 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, Padsorbed[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       Padsorbed[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          Padsorbed[Index];

                 }
                 //Debugger(Index);
                 //Debugger(Padsorbed[Index]);
              }
              else if (strcmp(MyVariable, "NO3InPoreWater 1") == 0)
              {
                 if (!PReadWrite->FindString("NO3InPoreWater 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, NO3InPoreWater[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       NO3InPoreWater[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          NO3InPoreWater[Index];

                 }
                 //Debugger(Index);
                 //Debugger(NO3InPoreWater[Index]);
              }
              else if (strcmp(MyVariable, "Corganic 1") == 0)
              {
                 if (!PReadWrite->FindString("Corganic 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int j = 0; j < NumberOfBoxesWithSediments; j++)
                 {
                    Index = MySedimentsRecord[j].LineCoordinate * NumberOfColumns + MySedimentsRecord[j].ColumnCoordinate;
                    PReadWrite->ReadNumber(XV, YV + 1 + j, Corganic[Index]);
                    for (int Layer = 1; Layer < NumberOfSedimentLayers; Layer++)
                       Corganic[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] =
                          Corganic[Index];

                 }
              }
              else if (strcmp(MyVariable, "LayerDepth 1") == 0)
              {
                 double Value;
                 if (!PReadWrite->FindString("LayerDepth 1 values", XV, YV))
                 {
                    XV = X + 2 + i;
                    YV = Y;
                 }
                 for (int Layer = 0; Layer < NumberOfSedimentLayers; Layer++)
                 {
                        PReadWrite->ReadNumber(XV, YV + 1 + Layer, Value);
                        //Debugger(X); Debugger(i); Debugger(Y); Debugger(NumberOfSedimentLayers); Debugger(Value);
                        for (int j = 0; j < NumberOfBoxesWithSediments; j++) {
                           LayerDepth[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] = Value;
                           if ((LayerDepth[Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer)] > MaxTopDepth) &&
                                (Layer == 0))
#ifdef __BORLANDC__
                             MessageBox(0, "Layer depth > MaxTopDepth", "EcoDynamo alert from TSediments::ReadVariables", MB_OK);
#else  // __BORLANDC__
															cerr << "TSediments::ReadVariables - Layer depth > MaxTopDepth" << endl;
#endif  // __BORLANDC__
                        }
                 }
              }
       }
        }
        else 
#ifdef __BORLANDC__
					MessageBox(0, "Variables - Undefined object in sediments", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					cerr << "TSediments::ReadVariables - Variables: undefined object in sediments" << endl;
#endif  // __BORLANDC__
        CloseDataFile((void*)PReadWrite);
    }

    for (int j = 0; j < NumberOfBoxesWithSediments; j++)
    {
       for (int Layer = 0; Layer < NumberOfSedimentLayers; Layer++)
       {
            int Index = Get3DIndex(MySedimentsRecord[j].LineCoordinate, MySedimentsRecord[j].ColumnCoordinate, Layer);
            SedimentDensity[Index] = MySedimentsRecord[j].Density;
            /*InorganicMass[Index] = MySedimentsRecord[j].Density * CUBIC *  CUBIC * LayerDepth[Index] *      //density converted to g/m3
                                   (1 - MySedimentsRecord[j].Porosity); // g/m2 */
            SedimentMass[Index] = MySedimentsRecord[j].Density * CUBIC *  CUBIC * LayerDepth[Index] *      //density converted to g/m3
                                   (1 - MySedimentsRecord[j].Porosity); // g/m2
            //OrganicMass[Index] = Corganic[Index] * CarbonToOrganics * InorganicMass[Index] / (CUBIC * CUBIC); // g/m2
            OrganicMass[Index] = Corganic[Index] * CarbonToOrganics[Index] * SedimentMass[Index] / (CUBIC * CUBIC); // g/m2
            //SedimentMass[Index] = InorganicMass[Index] + OrganicMass[Index];
            InorganicMass[Index] = SedimentMass[Index] - OrganicMass[Index];
       }
    }
}


TSediments::~TSediments()
{
    freeMemory();
}

void TSediments::freeMemory()
{
    try {
       if (NumberOfParameters > 0) {
          delete [] ParametersNameArray;
          NumberOfParameters = 0;
       }

       if ((NumberOfBoxes > 0) && (NumberOfBoxesWithSediments > 0))
       {
          delete [] Corganic;
          delete [] Mineralization_C;
          delete [] CorganicFlux;
          delete [] Norganic;
          delete [] NH4InPoreWater;
          delete [] NO3InPoreWater;
          delete [] Porganic;
          delete [] PO4InPoreWater;
          delete [] O2InPoreWater;
          delete [] Padsorbed;
          delete [] NorganicFlux;
          delete [] NH4InPoreWaterFlux;
          delete [] PorganicFlux;
          delete [] PO4InPoreWaterFlux;
          delete [] O2InPoreWaterFlux;
          delete [] NO3InPoreWaterFlux;
          delete [] Mineralization_N;
          delete [] Mineralization_P;
          delete [] Adsorption_P;
          delete [] Desorption_P;
          delete [] Diffusion_NH4;
          delete [] Diffusion_NO3;
          delete [] Diffusion_O2;
          delete [] Diffusion_P;
          delete [] SorptionFlux;
          delete [] NitrificationFlux;
          delete [] DeNitrificationFlux;
          delete [] CorganicInOut;
          delete [] NorganicInOut;
          delete [] PorganicInOut;
          delete [] OrganicMass;
          delete [] InorganicMass;
          delete [] OrganicMassFlux;
          delete [] InorganicMassFlux;
          delete [] OrganicInOut;
          delete [] InorganicInOut;
          delete [] ReferenceHeight;
          delete [] LayerDepth;
          delete [] SedimentMass;
          delete [] SedimentDensity;
    //      delete [] Generic;
    //      delete [] GenericLoad;
    //      delete [] GenericFlux;
          delete [] CarbonToOrganics;
       }
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
				cerr << "TSediments::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}


double TSediments::GetParameterValue(char* MyParameter)
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
    else if (strcmp(MyParameter, "ProportionOfNH4FromDenitrification") == 0)
    {
        value = ProportionOfNH4FromDenitrification;
    }
    else
        value = 0.0;
    return value;
}

bool TSediments::SetParameterValue(char* MyParameter, double value)
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
    else if (strcmp(MyParameter, "ProportionOfNH4FromDenitrification") == 0)
    {
        ProportionOfNH4FromDenitrification = value;
    }
    else
        rc = false;

    return rc;
}


bool TSediments::SaveParameters()
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
	    else if (strcmp(ParametersNameArray[i], "ProportionOfNH4FromDenitrification") == 0)
	        PReadWrite->WriteCell(ProportionOfNH4FromDenitrification, 6);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);

    return true;
}


bool TSediments::SaveVariables()
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
            PReadWrite->WriteSeparator(true);
        }
    }
    CloseDataFile((void*)PReadWrite);

    return true;
}



void TSediments::Go()
{
   //Debugger(1);
   int index, ASedimentBoxNumber, i, j, m, Layer;
   char function[128];
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();

   //Debugger(NumberOfBoxesWithSediments);
 try {
   for (m = 0; m < NumberOfBoxesWithSediments; m++)
   {
      //if (m > 3551) Debugger(m);
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
                AmmoniaSedimentWaterDiffusion(index, ASedimentBoxNumber, m, Layer);
                strcpy(function, "NitrateSedimentWaterDiffusion");
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
    cerr << exc.what() << " (" << function << ", ASedimentBoxNumber: " << ASedimentBoxNumber << ", box: " << m << ", layer: " << Layer << ") Go 1/2" << endl;
  }
#endif  // __BORLANDC__
}

void TSediments::Update(char* srcName, double Value,int BoxNumber,char* ParameterName,int AnObjectCode)
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
    else
    {
#ifdef __BORLANDC__
    	static char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in this object 1");
    	MessageBox(0,Caption,"EcoDynamo Alert",MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
							cerr << "TSediments::Update - " << MyParameter << " does not exits in this object 1" << endl;
#endif  // __BORLANDC__
    }
}

bool TSediments::SetVariableValue(char* srcName, double Value,
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
    else if (strcmp(VariableName, "Diffusion_NH4 1") == 0)
       Diffusion_NH4[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_NO3 1") == 0)
       Diffusion_NO3[BoxNumber] = Value;
    else if (strcmp(VariableName, "Diffusion_P 1") == 0)
       Diffusion_P[BoxNumber] = Value;
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
    else
        rc = false;

    return rc;
}

void TSediments::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName, int AnObjectCode)
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
    else if (strcmp(MyParameter, "Diffusion_NH4 1") == 0)
       Value = Diffusion_NH4[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_NO3 1") == 0)
       Value = Diffusion_NO3[MyBoxNumber];
    else if (strcmp(MyParameter, "Diffusion_P 1") == 0)
       Value = Diffusion_P[MyBoxNumber];
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
							cerr << "TSediments::Inquiry - " << MyParameter << " does not exits in this object" << endl;
#endif  // __BORLANDC__
        }
    }
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

/*
void TSediments::Inquiry(char* srcName, double &Value, int BoxNumber, char* ParameterName, int AnObjectCode)
{
    static char MyName[65], MyParameter[65], Myjstring[5]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxNumber, MyColumn, MyLine;
    strcpy(MyParameter, ParameterName);
    Value = 0;
    MyColumn = GetColumnIndex(MyBoxNumber);
    MyLine = GetLineIndex(MyBoxNumber);
    for (int i = 0; i < NumberOfVariables; i++)
       if (strcmp(MyParameter, VariableNameArray[i]) == 0)
       {
                if (strcmp(MyParameter, "ReferenceHeight") == 0)
                        Value = ReferenceHeight[MyBoxNumber];
                else
                {
                        for (int MyLayer = 0; MyLayer < NumberOfSedimentLayers; MyLayer++)
                        {
                                strcpy( MyName, "Corganic " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Corganic[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Norganic " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Norganic[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "NH4InPoreWater " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = NH4InPoreWater[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Porganic " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Porganic[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "PO4InPoreWater " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = PO4InPoreWater[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Padsorbed " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Padsorbed[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "O2InPoreWater " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = O2InPoreWater[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Mineralization_C " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Mineralization_C[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Mineralization_N " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Mineralization_N[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Mineralization_P " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Mineralization_P[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Diffusion_NH4 " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Diffusion_NH4[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Diffusion_NO3 " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Diffusion_NO3[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Diffusion_P " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Diffusion_P[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Diffusion_O2 " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Diffusion_O2[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Adsorption_P " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Adsorption_P[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Desorption_P " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = Desorption_P[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "NO3InPoreWater " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = NO3InPoreWater[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "Nitrification " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = NitrificationFlux[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "DeNitrification " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = DeNitrificationFlux[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "OrganicMass " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = OrganicMass[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                                strcpy( MyName, "InorganicMass " );
                                sprintf( Myjstring , "%i", MyLayer + 1 );
                                strcat( MyName, Myjstring );
                                if (strcmp(MyParameter, MyName) == 0)
                                {
                                        Value = InorganicMass[Get3DIndex(MyLine, MyColumn, MyLayer)];
                                        //break;
                                }
                        }
                }
       }
       else
       {
            static char Caption[129];
	    strcpy(Caption, MyParameter);
    	    strcat(Caption, " does not exist in this object 2");
    	    MessageBox(0,Caption,"EcoDynamo Alert",MB_OK | MB_ICONHAND);
       }
       LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}   */



void TSediments::Integrate()
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
      cerr << exc.what() << " (box: " << m << ",layer: " << Layer << ",index: " << index << ") Integrate 1/2" << endl;
   }
#endif  // __BORLANDC__
   try {
       Generic = NH4InPoreWater; GenericFlux = NH4InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       NH4InPoreWater = Generic; NH4InPoreWaterFlux = GenericFlux;
       // PO4InPoreWater
       Generic = PO4InPoreWater; GenericFlux = PO4InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       PO4InPoreWater = Generic; PO4InPoreWaterFlux = GenericFlux;
       // O2InPoreWater
       Generic = O2InPoreWater; GenericFlux = O2InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       O2InPoreWater = Generic; O2InPoreWaterFlux = GenericFlux;
       // NO3InPoreWater
       Generic = NO3InPoreWater; GenericFlux = NO3InPoreWaterFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       NO3InPoreWater = Generic; NO3InPoreWaterFlux = GenericFlux;

       Generic = OrganicMass; GenericFlux = OrganicMassFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       OrganicMass = Generic; OrganicMassFlux = GenericFlux;

       Generic = InorganicMass; GenericFlux = InorganicMassFlux;
       Integration(NumberOfSedimentLayers, NumberOfBoxesWithSediments, MySedimentsRecord);
       InorganicMass = Generic; InorganicMassFlux = GenericFlux;
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

                if (SedimentMass[index] > TINNY)
                        SedimentDensity[index] = (MySedimentsRecord[m].Density * InorganicMass[index] + OrganicMatterDensity * OrganicMass[index]) /
                                                   SedimentMass[index]; //10 Fev
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

            SedimentMass[index0] = SedimentMass[index0] + SedimentMass[index1] * Delta / LayerDepth[index1];
            InorganicMass[index0] = InorganicMass[index0] + InorganicMass[index1] * Delta / LayerDepth[index1];
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
      cerr << exc.what() << "(box: " << m << ",layer: " << Layer << ",i: " << i << ",j: " << j << ") Integrate 3/4" << endl;
   }
#endif  // __BORLANDC__
}


void TSediments::BenthicNitrogenMineralization(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber, WaterBoxNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   double MyMineralization;
   double OxygenNitrogenRatio = 0.212; //g O2 consumed for 1 mmol N mineralized or mg O2 per micromol N (Chapelle et al. (2000) - Ecolog. Modell 127: 161-181)
   //if (MySedimentRecordNumber > 3551) Debugger(100);
   Mineralization_N[MyBoxNumber] = minNs * Norganic[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber)* OxygenLimitation(MyBoxNumber, KminO2);  //micro g g-1 d-1
   //if (MySedimentRecordNumber > 3551) Debugger(101);
   NorganicFlux[MyBoxNumber] = NorganicFlux[MyBoxNumber] - Mineralization_N[MyBoxNumber] / DAYSTOSECONDS;                          //micro g g-1 s-1
   //if (MySedimentRecordNumber > 3551) Debugger(102);
   MyMineralization = Mineralization_N[MyBoxNumber] / DAYSTOSECONDS / NITROGENATOMICWEIGHT *
                      MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio;  //micro mol N L-1 s-1
   //if (MySedimentRecordNumber > 3551) Debugger(103);
   NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] + MyMineralization * FractionMineralizedToAmmonia;  //micro mol N-NH4 L-1 s-1
   //if (MySedimentRecordNumber > 3551) Debugger(104);
   O2InPoreWaterFlux[MyBoxNumber] = O2InPoreWaterFlux[MyBoxNumber] - MyMineralization * OxygenNitrogenRatio; //mg O2 L-1 s-1
}


void TSediments::BenthicPhosphorusMineralization(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber, WaterBoxNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   Mineralization_P[MyBoxNumber] = minPs * Porganic[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber) * OxygenLimitation(MyBoxNumber, KminO2);  //micro g g-1 d-1
   PorganicFlux[MyBoxNumber] = PorganicFlux[MyBoxNumber] - Mineralization_P[MyBoxNumber] / DAYSTOSECONDS;                          //micro g g-1 s-1
   PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] +
                                     Mineralization_P[MyBoxNumber] / DAYSTOSECONDS / PHOSPHORUSATOMICWEIGHT *
                                     MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio /** FractionMineralizedToPhosphorus*/;  //micro mol P L-1 s-1
}

void TSediments::BenthicCarbonMineralization(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber, WaterBoxNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   Mineralization_C[MyBoxNumber] = minCs * Corganic[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber) * OxygenLimitation(MyBoxNumber, KminO2);  //micro g g-1 d-1
   CorganicFlux[MyBoxNumber] = CorganicFlux[MyBoxNumber] - Mineralization_C[MyBoxNumber] / DAYSTOSECONDS;                          //micro g g-1 s-1
   OrganicMassFlux[MyBoxNumber] = OrganicMassFlux[MyBoxNumber] - Mineralization_C[MyBoxNumber] / DAYSTOSECONDS * CarbonToOrganics[MyBoxNumber];
}

void TSediments::AmmoniaSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer)
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
      double AmmoniaInWater, Depth;
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
                MessageBox(0, "Error 1 in TSediments::AmmoniaSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
								cerr << "Error 1 in TSediments::AmmoniaSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__

                Debugger(LayerDepth[MyBoxNumber]); Debugger(MyBoxNumber);
        }
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), AmmoniaInWater,
                                             MyWaterColumnBoxNumber,
                                             "Ammonia",
                                             ObjectCode);

        Diffusion_NH4[MyBoxNumber] = DiffusionCoefficientForN / pow (meters_to_dcm,2) * (NH4InPoreWater[MyBoxNumber] - AmmoniaInWater) /
                                     ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        /*Debugger(DiffusionCoefficientForN); Debugger(meters_to_dcm);
        Debugger(NH4InPoreWater[MyBoxNumber]); Debugger(AmmoniaInWater);
        Debugger(LayerDepth[MyBoxNumber]); Debugger(Depth);
        Debugger(MySedimentsRecord[MySedimentRecordNumber].Porosity);
        Debugger(Diffusion_NH4[MyBoxNumber]); */
        NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] - Diffusion_NH4[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        //Debugger(Diffusion_NH4[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  Diffusion_NH4[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth * CUBIC,
                                  MyBoxNumber,
                                  "Ammonia",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        if (LayerDepth[MyBoxNumber] <= 0.0) 
#ifdef __BORLANDC__
					MessageBox(0, "Error 2 in TSediments::AmmoniaSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					 cerr << "Error 2 in TSediments::AmmoniaSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__

        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);

        if (LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] <= 0.0)
        {
#ifdef __BORLANDC__
           MessageBox(0, "Error 3 in TSediments::AmmoniaSedimentWaterDiffusion", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
					 cerr << "Error 3 in TSediments::AmmoniaSedimentWaterDiffusion" << endl;
#endif  // __BORLANDC__
           Debugger(MyLine); Debugger(MyColumn); Debugger(ALayer); Debugger(MyBoxNumber); Debugger(Get3DIndex(MyLine, MyColumn, ALayer - 1));
        }
        Diffusion_NH4[MyBoxNumber] = DiffusionCoefficientForNInSediments / pow (meters_to_dcm,2) *
                                     (NH4InPoreWater[MyBoxNumber] - NH4InPoreWater[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        //Debugger(Diffusion_NH4[MyBoxNumber]);
        NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] - Diffusion_NH4[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        NH4InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                NH4InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_NH4[MyBoxNumber] *
                LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] /
                DAYSTOSECONDS;//micro mol N L-1 s-1
        //Debugger(Diffusion_NH4[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);

      }
   }
}

void TSediments::NitrateSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer)
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
      double NitrateInWater, Depth;
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
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), NitrateInWater,
                                             MyWaterColumnBoxNumber,
                                             "Nitrate",
                                             ObjectCode);
        Diffusion_NO3[MyBoxNumber] = DiffusionCoefficientForN / pow (meters_to_dcm,2) * (NO3InPoreWater[MyBoxNumber] - NitrateInWater) /
                                  ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                 (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        NO3InPoreWaterFlux[MyBoxNumber] = NO3InPoreWaterFlux[MyBoxNumber] - Diffusion_NO3[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  Diffusion_NO3[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth * CUBIC,
                                  MyBoxNumber,
                                  "Nitrate",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);
        Diffusion_NO3[MyBoxNumber] = DiffusionCoefficientForNInSediments / pow (meters_to_dcm,2) *
                                     (NO3InPoreWater[MyBoxNumber] - NO3InPoreWater[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        //Debugger(Diffusion_NO3[MyBoxNumber]);
        NO3InPoreWaterFlux[MyBoxNumber] = NO3InPoreWaterFlux[MyBoxNumber] - Diffusion_NO3[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        NO3InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                NO3InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_NO3[MyBoxNumber] *
                LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] /
                DAYSTOSECONDS;//micro mol N L-1 s-1
        //Debugger(Diffusion_NO3[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);

      }
   }
}

void TSediments::PhosphorusSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer)
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
      double PhosphateInWater, Depth;
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
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), PhosphateInWater,
                                             MyWaterColumnBoxNumber,
                                             "Phosphate",
                                             ObjectCode);
        Diffusion_P[MyBoxNumber] = DiffusionCoefficientForP / pow (meters_to_dcm,2) * (PO4InPoreWater[MyBoxNumber] - PhosphateInWater) /
                                   ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                   (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol P L-1 d-1
        //Debugger(3);
        //Debugger(Diffusion_P[MyBoxNumber]);
        PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] - Diffusion_P[MyBoxNumber] / DAYSTOSECONDS;//micro mol P L-1 s-1
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  Diffusion_P[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth * CUBIC,
                                  MyBoxNumber,
                                  "Phosphate",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);
        Diffusion_P[MyBoxNumber] = DiffusionCoefficientForPInSediments / pow (meters_to_dcm,2) *
                                     (PO4InPoreWater[MyBoxNumber] - PO4InPoreWater[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        //Debugger(Diffusion_P[MyBoxNumber]);
        PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] - Diffusion_P[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        PO4InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                PO4InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_P[MyBoxNumber] *
                LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]
                / DAYSTOSECONDS;//micro mol N L-1 s-1
        //Debugger(Diffusion_P[MyBoxNumber] / DAYSTOSECONDS * MySedimentsRecord[MySedimentRecordNumber].LayerHeight / Depth * CUBIC);

      }
   }
}

void TSediments::OxygenSedimentWaterDiffusion(int AWaterColumnBoxNumber, int ASedimentBoxNumber, int ASedimentRecordNumber, int ALayer  )
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
      double OxygenInWater, Depth;
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
        MyNutrientPointer->Inquiry(GetEcoDynClassName(), OxygenInWater,
                                             MyWaterColumnBoxNumber,
                                             "Oxygen",
                                             ObjectCode);
        //Debugger(DiffusionCoefficientForO2); Debugger(OxygenInWater); Debugger(O2InPoreWater[MyBoxNumber]); Debugger(LayerDepth[MyBoxNumber]);
        //Debugger(MySedimentsRecord[MySedimentRecordNumber].LayerHeight); Debugger(MySedimentsRecord[MySedimentRecordNumber].Porosity);
        Diffusion_O2[MyBoxNumber] = DiffusionCoefficientForO2 / pow (meters_to_dcm,2) * (OxygenInWater - O2InPoreWater[MyBoxNumber]) /
                                  ((LayerDepth[MyBoxNumber] + Depth) * 0.5 * meters_to_dcm) /
                                  (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //mg O2 L-1 d-1
        //Debugger(Diffusion_O2[MyBoxNumber]);
        O2InPoreWaterFlux[MyBoxNumber] = O2InPoreWaterFlux[MyBoxNumber] + Diffusion_O2[MyBoxNumber] / DAYSTOSECONDS;//mg O2 L-1 s-1
        MyNutrientPointer->Update(GetEcoDynClassName(),
                                  -Diffusion_O2[MyBoxNumber] / DAYSTOSECONDS * LayerDepth[MyBoxNumber] / Depth,
                                  MyBoxNumber,
                                  "Oxygen",
                                  ObjectCode);
      }
      else
      if (ALayer > 0)
      {
        MyColumn = GetColumnIndex3D(MyBoxNumber, ALayer);
        MyLine = GetLineIndex3D(MyBoxNumber, ALayer);
        Diffusion_O2[MyBoxNumber] = DiffusionCoefficientForO2InSediments / pow (meters_to_dcm,2) *
                                     (O2InPoreWater[MyBoxNumber] - O2InPoreWater[Get3DIndex(MyLine, MyColumn, ALayer - 1)]) /
                                     ((LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)]+ LayerDepth[MyBoxNumber]) * 0.5 * meters_to_dcm) /
                                     (LayerDepth[MyBoxNumber] * meters_to_dcm) * MySedimentsRecord[MySedimentRecordNumber].Porosity;  //micro mol N L-1 d-1
        O2InPoreWaterFlux[MyBoxNumber] = O2InPoreWaterFlux[MyBoxNumber] - Diffusion_O2[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
        O2InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] =
                O2InPoreWaterFlux[Get3DIndex(MyLine, MyColumn, ALayer - 1)] + Diffusion_O2[MyBoxNumber] *
                                       LayerDepth[MyBoxNumber] / LayerDepth[Get3DIndex(MyLine, MyColumn, ALayer - 1)] /
                                       DAYSTOSECONDS;//micro mol N L-1 s-1
      }
   }
}

void TSediments::PhosphorusAdsorption(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int i;
   int MyBoxNumber, MySedimentRecordNumber;
   MyBoxNumber = ASedimentBoxNumber;
   MySedimentRecordNumber = ASedimentRecordNumber;
   try {
       i = 0;
       if (O2InPoreWater[MyBoxNumber] > O2thrs) {
        i = 1;
          Adsorption_P[MyBoxNumber] = Ka1 * (1.0 - Padsorbed[MyBoxNumber] / Pmax) * PO4InPoreWater[MyBoxNumber]; //micro mol P L-1 d-1
       }
       else {
        i = 2;
          Adsorption_P[MyBoxNumber] = Ka2 * (1.0 - Padsorbed[MyBoxNumber] / Pmax) * PO4InPoreWater[MyBoxNumber]; //micro mol P L-1 d-1
       }
       i = 3;
       SorptionFlux[MyBoxNumber] = SorptionFlux[MyBoxNumber] + Adsorption_P[MyBoxNumber] / DAYSTOSECONDS *
                                   PHOSPHORUSATOMICWEIGHT / MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio;//micro g P g-1 s-1
       i = 4;
       PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] - Adsorption_P[MyBoxNumber] / DAYSTOSECONDS;//micro mol P L-1 s-1
  }
#ifdef __BORLANDC__
  catch (Exception &exception) {
    ShowMessage("PhosphorusAdsorption "+exception.Message+" (MyBoxNumber:"+ASedimentBoxNumber
            +", MySedimentRecordNumber:"+ASedimentRecordNumber+", i:"+i+") ");
    ShowMessage("Adsorption_P:"+AnsiString(Adsorption_P[MyBoxNumber])+", SorptionFlux:"+SorptionFlux[MyBoxNumber]
            +", PO4InPoreWater:"+PO4InPoreWater[MyBoxNumber]+", Padsorbed:"+Padsorbed[MyBoxNumber]
            +", SedimentWaterRatio:"+MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio
            +", PO4InPoreWaterFlux:"+PO4InPoreWaterFlux[MyBoxNumber]);
  }
#else  // __BORLANDC__
  catch (exception &exc) {
    cerr << "PhosphorusAdsorption " << exc.what() << " (MyBoxNumber:" << ASedimentBoxNumber << ", MySedimentRecordNumber:" << ASedimentRecordNumber << ", i:" << i << ") " << endl;
    cerr << "Adsorption_P:" << Adsorption_P[MyBoxNumber] << ", SorptionFlux:" << SorptionFlux[MyBoxNumber] << ", PO4InPoreWater:" << PO4InPoreWater[MyBoxNumber] << ", Padsorbed:" << Padsorbed[MyBoxNumber] << ", SedimentWaterRatio:" << MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio
             << ", PO4InPoreWaterFlux:" << PO4InPoreWaterFlux[MyBoxNumber] << endl;
  }
#endif  // __BORLANDC__
}

void TSediments::PhosphorusDesorption(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, MySedimentRecordNumber;
   MyBoxNumber = ASedimentBoxNumber; MySedimentRecordNumber = ASedimentRecordNumber;
   Desorption_P[MyBoxNumber] = (Kd * Padsorbed[MyBoxNumber] / Pmax) /
                                PHOSPHORUSATOMICWEIGHT * MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio; //micro mol P L-1 d-1
   SorptionFlux[MyBoxNumber] = SorptionFlux[MyBoxNumber] - Desorption_P[MyBoxNumber] / DAYSTOSECONDS *
                               PHOSPHORUSATOMICWEIGHT / MySedimentsRecord[MySedimentRecordNumber].SedimentWaterRatio;//micro g P g-1 s-1
   PO4InPoreWaterFlux[MyBoxNumber] = PO4InPoreWaterFlux[MyBoxNumber] + Desorption_P[MyBoxNumber] / DAYSTOSECONDS;//micro mol P L-1 s-1
}

void TSediments::Nitrification(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, WaterBoxNumber, MySedimentRecordNumber;
   MyBoxNumber = ASedimentBoxNumber;
   MySedimentRecordNumber = ASedimentRecordNumber;
   double OxygenNitrogenRatioInNitrification = 0.064; //g O2 consumed for 1 mmol N nitrified or mg O2 per micromol N (Chapelle et al. (1995) - Ecolog. Modell 80: 131-147)
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   NitrificationFlux[MyBoxNumber] = knit * NH4InPoreWater[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber) * OxygenLimitation(MyBoxNumber, knitO2); //micro mol N-NO3 L-1 d-1
   NO3InPoreWaterFlux[MyBoxNumber] = NO3InPoreWaterFlux[MyBoxNumber] + NitrificationFlux[MyBoxNumber] / DAYSTOSECONDS;      //micro mol N-NO3 L-1 s-1
   O2InPoreWaterFlux[MyBoxNumber] = O2InPoreWaterFlux[MyBoxNumber] - NitrificationFlux[MyBoxNumber] * OxygenNitrogenRatioInNitrification / DAYSTOSECONDS; //mg O2 L-1 s-1
   NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] - NitrificationFlux[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
}

void TSediments::DeNitrification(int ASedimentBoxNumber, int ASedimentRecordNumber)
{
   int MyBoxNumber, WaterBoxNumber, MySedimentRecordNumber;
   MyBoxNumber = ASedimentBoxNumber;
   MySedimentRecordNumber = ASedimentRecordNumber;
   //double ProportionOfNH4FromDenitrification = 1.0; //(Chapelle et al. (1995) - Ecolog. Modell 80: 131-147) - Remaining 0.6 is lost as N2
   WaterBoxNumber = MySedimentsRecord[MySedimentRecordNumber].LineCoordinate * NumberOfColumns + MySedimentsRecord[MySedimentRecordNumber].ColumnCoordinate;
   DeNitrificationFlux[MyBoxNumber] = kdenit * NO3InPoreWater[MyBoxNumber] * TemperatureLimitation(WaterBoxNumber) * (1 - OxygenLimitation(MyBoxNumber, kdenitO2)); //micro mol N-NO3 L-1 d-1
   NO3InPoreWaterFlux[MyBoxNumber] = NO3InPoreWaterFlux[MyBoxNumber] - DeNitrificationFlux[MyBoxNumber] / DAYSTOSECONDS; //micro mol N-NO3 L-1 s-1
   NH4InPoreWaterFlux[MyBoxNumber] = NH4InPoreWaterFlux[MyBoxNumber] + ProportionOfNH4FromDenitrification * DeNitrificationFlux[MyBoxNumber] / DAYSTOSECONDS;//micro mol N L-1 s-1
}

double TSediments::OxygenLimitation(int ABoxNumber, double K)
{
   int MyBoxNumber; MyBoxNumber = ABoxNumber;
   return (O2InPoreWater[MyBoxNumber] / (O2InPoreWater[MyBoxNumber] + K));
}

double TSediments::TemperatureLimitation(int ABoxNumber)
{
   int MyBoxNumber; MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	  double WaterTemperature;
   	  MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      return exp(Kt * WaterTemperature);
   }
   else
   return 1.0;
}


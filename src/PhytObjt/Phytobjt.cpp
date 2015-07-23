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
	#include <cmath>
	#include <cstdio>
	#include <cstdlib>
	
	using namespace std;
#endif  // __BORLANDC__

#ifndef _PORT_FORTRAN_
	#include "iodll.h"
#endif  // _PORT_FORTRAN_

#include "ecodyn.rh"
#include "phytobjt.h"


// TPhytoplankton Class
//
// Constructors invoked outside EcoDynamo shell...
//

#ifndef _PORT_FORTRAN_
TPhytoplankton::TPhytoplankton(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration)
    :TProdutor(className, timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
{
    PreBuildPhytoplankton();

	NumberOfParameters = nParams;
    SetPhytoParams(pmax, iopt, slope, aEiler, bEiler, cEiler,
            kValue, ks, kAmmonia, kNitrate, pHi, maintenanceRespiration,
            respirationCoefficient, docLoss, docStressLoss,
            deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate,
            fallingSpeed, ratioLightDarkRespiration);
}

TPhytoplankton::TPhytoplankton(char* className, float timeStep, char* morphologyFilename,
                char* parametersFilename)
                        :TProdutor(className, timeStep, morphologyFilename)
{
    strcpy(ParsFileName, parametersFilename);
    BuildPhytoplankton();
}
// ...

TPhytoplankton::TPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
										 : TProdutor(APEcoDynClass, className)
{
    BuildPhytoplankton();
}

#else  // !(_PORT_FORTRAN)
TPhytoplankton::TPhytoplankton(char* className) :TProdutor(className)
{
    PreBuildPhytoplankton();
}
#endif  // !(_PORT_FORTRAN_)

void TPhytoplankton::SetPhytoParams(double pmax, double iopt,
        double slope, double aEiler, double bEiler, double cEiler,
        double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration)
{
	for (int j = 0; j < NumberOfBoxes; j++)
	{
		Pmax[j] = pmax;
		Iopt[j] = iopt;
		Slope[j] = slope;
		AEiler[j] = aEiler;
		BEiler[j] = bEiler;
		CEiler[j] = cEiler;
		Ks[j] = ks;
		KAmmonia[j] = kAmmonia;
		KNitrate[j] = kNitrate;
		MaintenanceRespiration[j] = maintenanceRespiration;
	}
	KValue = kValue;
	phi = pHi;
	RespirationCoefficient = respirationCoefficient;
	DocLoss = docLoss;
	DocStressLoss = docStressLoss;
	DeathLoss = deathLoss;
	RedfieldCFactor = redfieldCFactor;
    RedfieldNFactor = redfieldNFactor;
    RedfieldPFactor = redfieldPFactor;
    TemperatureAugmentationRate = temperatureAugmentationRate;
    FallingSpeed = fallingSpeed;
    RatioLightDarkRespiration = ratioLightDarkRespiration;
}

void TPhytoplankton::PreBuildPhytoplankton()
{
    NCounter = 0;
    PCounter = 0;
    VariableCount = 0;
    NumberOfLoads = 0;
    NumberOfDaysForLoads = 0;
    NumberOfRiverLoads = 0;

	// Get box array
    BoxArray = MyPEcoDynClass->BoxArray;
    ObjectCode = PHYTOBJECTCODE;

	// Initialise pointers
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    ChlToCarbon[i] = 35;
        ChlCarbonRatio[i] = 0.04;
        PhytoProd[i] = 0;
        Pmax[i] = 0.2; //ugC /ug C/ h
        Iopt[i] = 400; //umolm-2s-1 {4.8 Ly/hour - Parsons - P.79 T. 17 PTH}
        CriticalLight[i] = Iopt[i];
        MaintenanceRespiration[i] = 0.1 * Pmax[i];
    }
    TemperatureAugmentationRate = 0.0;


	DocLoss = 0.1; //0.1 - 10% of NPP loss of DOC due to exudation
						//- Williams (1975) in Valiela, p. 279
	DocStressLoss = 0;
	DeathLoss = 0.05; /*day-1 - Dippner, 1993- Continental
							  Shelf Research, vol 13, n? 2/3, pp. 331-355}*/
	RedfieldCFactor = 45;
	RedfieldNFactor = 7;
	RedfieldNFactorInAtoms = 16;
	RedfieldPFactor = 1;
	RespirationCoefficient = 0.3;

	KValue = 0.4;//0.1;

        RTPH = 1; // what the hell are these three doing? 97.06.17
	RTMPH =  5;
	q10PH = 0.15;
}

#ifndef _PORT_FORTRAN_
void TPhytoplankton::BuildPhytoplankton()
{
    PreBuildPhytoplankton();
    //Read in the parameters
//	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->GetParmsFileHandle();
	TReadWrite* PReadWrite = (TReadWrite*)MyPEcoDynClass->OpenParametersFile("Phytoplankton");
	if (PReadWrite == NULL)
	{
#ifdef __BORLANDC__
		MessageBox(0, "Phytoplankton parameters file missing.", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TPhytoplankton::BuildPhytoplankton - Phytoplankton parameters file missing." << endl;
#endif  // __BORLANDC__
	}
    else {

        double MyValue;
        int X, Y;
        if (PReadWrite->FindString("Phytoplankton", X, Y))
        {
            PReadWrite->ReadNumber(X+1, Y, NumberOfParameters);
            ParametersNameArray = new VNA[NumberOfParameters];

            // read in the parameter names
            char MyParameter[65];
            for (int i = Y; i < Y + NumberOfParameters; i++)
            {
                PReadWrite->ReadString(X+2, i, MyParameter);
                strcpy(ParametersNameArray[i-Y], MyParameter);

                if (strcmp(MyParameter, "Pmax") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Pmax[j] = MyValue; //ugC /ug C/ h
                }
                else if (strcmp(MyParameter, "Iopt") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Iopt[j] = MyValue; //umol l-1
                }
                else if (strcmp(MyParameter, "Slope") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                    //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Slope[j] = MyValue; //umol l-1
                }
                else if (strcmp(MyParameter, "DefaultAEiler") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        AEiler[j] = MyValue;
                }
                else if (strcmp(MyParameter, "BEiler") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        BEiler[j] = MyValue;
                }
                else if (strcmp(MyParameter, "CEiler") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        CEiler[j] = MyValue;
                }
                else if (strcmp(MyParameter, "KValue") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    KValue = MyValue;
                }
                else if (strcmp(MyParameter, "Ks") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        Ks[j] = MyValue;
                }
                else if (strcmp(MyParameter, "KAmmonia") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        KAmmonia[j] = MyValue;
                }
                else if (strcmp(MyParameter, "KNitrate") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        KNitrate[j] = MyValue;
                }
                else if (strcmp(MyParameter, "phi") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    phi = MyValue;
                }
                else if (strcmp(MyParameter, "Maintenance respiration") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    for (int j = 0; j < NumberOfBoxes; j++)
                        MaintenanceRespiration[j] = MyValue;
                }
                else if (strcmp(MyParameter, "Respiration Coefficient") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RespirationCoefficient = MyValue; // Correct to make this an array for consistency with MaintenanceRespiration - jgf/pd 96.09.24
                }
                else if (strcmp(MyParameter, "Dissolved organic carbon loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    DocLoss = MyValue;
                }
                else if (strcmp(MyParameter, "Dissolved organic carbon nutrient stress loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    DocStressLoss = MyValue;
                }
                else if (strcmp(MyParameter, "Death loss") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    DeathLoss = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldCFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RedfieldCFactor = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldNFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RedfieldNFactor = MyValue;
                }
                else if (strcmp(MyParameter, "RedfieldPFactor") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RedfieldPFactor= MyValue;
                }
                else if (strcmp(MyParameter, "TemperatureAugmentationRate") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    TemperatureAugmentationRate = MyValue;
                }
                else if (strcmp(MyParameter, "Falling speed") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    FallingSpeed = MyValue;
                }
                else if (strcmp(MyParameter, "Light/Dark resp") == 0)
                {
                    PReadWrite->ReadNumber(X+3, i, MyValue);
                //Debugger(MyValue);
                    RatioLightDarkRespiration = MyValue;
                }
                else if (strcmp(MyParameter, "NumberOfLoads") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, NumberOfLoads);
                    //Debugger(NumberOfLoads);
                }
                else if (strcmp(MyParameter, "NumberOfDaysForLoads") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, NumberOfDaysForLoads);
                    //Debugger(NumberOfDaysForLoads);
                }
                else if (strcmp(MyParameter, "NumberOfRiverLoads") == 0)
                {
                    PReadWrite->ReadNumber(X + 3, i, NumberOfRiverLoads);
                }
            }
        }
        else
        {
#ifdef __BORLANDC__
            MessageBox(0, "Parameters: Undefined object Phytoplankton", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
		cerr << "TPhytoplankton::BuildPhytoplankton - Parameters: Undefined object Phytoplankton." << endl;
#endif  // __BORLANDC__
        }
        CloseDataFile((void*)PReadWrite);
    }

        if (NumberOfLoads > 0)
            ALoadRecord = new LoadRecord[NumberOfLoads];
        if (NumberOfRiverLoads > 0)
            ALoadRiverRecord = new LoadRiverRecord[NumberOfRiverLoads];
        for (int i = 0; i < NumberOfLoads; i++)
        {
                ALoadRecord[i].PhytoplanktonCLoad = new double[NumberOfDaysForLoads];
                ALoadRecord[i].PhytoplanktonNLoad = new double[NumberOfDaysForLoads];
                ALoadRecord[i].PhytoplanktonPLoad = new double[NumberOfDaysForLoads];
        }
        for (int i = 0; i < NumberOfLoads; i++)
        {
                ALoadRecord[i].LineCoordinate = 0;
                ALoadRecord[i].ColumnCoordinate = 0;
                for (int j = 0; j < NumberOfDaysForLoads; j++)
                {
                        ALoadRecord[i].PhytoplanktonCLoad[j] = 0.0;
                        ALoadRecord[i].PhytoplanktonNLoad[j] = 0.0;
                        ALoadRecord[i].PhytoplanktonPLoad[j] = 0.0;
                }
        }
        for (int i = 0; i < NumberOfRiverLoads; i++)
        {
                ALoadRiverRecord[i].PhytoplanktonCRiverLoad = new double[NumberOfDaysForLoads];
                ALoadRiverRecord[i].PhytoplanktonNRiverLoad = new double[NumberOfDaysForLoads];
                ALoadRiverRecord[i].PhytoplanktonPRiverLoad = new double[NumberOfDaysForLoads];
                ALoadRiverRecord[i].PhytoplanktonBiomassLoad = new double[NumberOfDaysForLoads];
        }
        for (int i = 0; i < NumberOfRiverLoads; i++)
        {
                ALoadRiverRecord[i].LineCoordinate = 0;
                ALoadRiverRecord[i].ColumnCoordinate = 0;
                for (int j = 0; j < NumberOfDaysForLoads; j++)
                {
                        ALoadRiverRecord[i].PhytoplanktonCRiverLoad[j] = 0.0;
                        ALoadRiverRecord[i].PhytoplanktonNRiverLoad[j] = 0.0;
                        ALoadRiverRecord[i].PhytoplanktonPRiverLoad[j] = 0.0;
                        ALoadRiverRecord[i].PhytoplanktonBiomassLoad[j] = 0.0;
                }
        }
}
#endif  // !(_PORT_FORTRAN_)

TPhytoplankton::~TPhytoplankton()
{
    freeMemory();
}

void TPhytoplankton::freeMemory()
{
    try {
        if (NumberOfParameters > 0) {
            delete [] ParametersNameArray;
            NumberOfParameters = 0;
        }
        if ((NumberOfLoads > 0) && (NumberOfDaysForLoads > 0)) {
            for (int i = 0; i < NumberOfLoads; i++)
            {
                delete [] ALoadRecord[i].PhytoplanktonCLoad;
                delete [] ALoadRecord[i].PhytoplanktonNLoad;
                delete [] ALoadRecord[i].PhytoplanktonPLoad;
            }
            delete [] ALoadRecord;
        }
        if ((NumberOfRiverLoads > 0) && (NumberOfDaysForLoads > 0)) {
            for (int i = 0; i < NumberOfRiverLoads; i++)
            {
                delete [] ALoadRiverRecord[i].PhytoplanktonCRiverLoad;
                delete [] ALoadRiverRecord[i].PhytoplanktonNRiverLoad;
                delete [] ALoadRiverRecord[i].PhytoplanktonPRiverLoad;
                delete [] ALoadRiverRecord[i].PhytoplanktonBiomassLoad;
            }
            delete [] ALoadRiverRecord;
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
				cerr << "TPhytoplankton::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

double TPhytoplankton::GetParameterValue(char* MyParameter)
{
  double value;

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
    else
        value = 0.0;
    return value;
}

bool TPhytoplankton::SetParameterValue(char* MyParameter, double value)
{
  bool rc = true;
  int j;

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
    else
        rc = false;

    return rc;
}

#ifndef _PORT_FORTRAN_
// AP, 2007.07.12
bool TPhytoplankton::SaveParameters()
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
        PReadWrite->WriteSeparator(true);
    }
    PReadWrite->WriteSeparator(true);

    CloseDataFile((void*)PReadWrite);
    return true;
}
#endif

void TPhytoplankton::Go ()
{
	// Active methods
  	Production();
	Respiration();
	Exudation();
	Mortality();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
	}
}

bool TPhytoplankton::SetVariableValue(char* srcName, double Value,
									  int BoxNumber,
									  char* VariableName)
{
  bool rc = true;

    LogMessage("SetVariableValue", srcName, VariableName, Value, BoxNumber);

	if (strcmp(VariableName, "Phytoplankton C biomass") == 0)
		PhytoBiomass[BoxNumber] = Value;
    else if (strcmp(VariableName, "Phytop. total GPP") == 0)
		GPP[BoxNumber] = Value;
    else if (strcmp(VariableName, "Phytop. total NPP") == 0)
		NPP[BoxNumber] = Value;
	else if (strcmp(VariableName, "Chlorophyll : Carbon") == 0)
		ChlCarbonRatio[BoxNumber] = Value;
	else if (strcmp(VariableName, "Chlorophyll to biomass") == 0)
		ChlToBiomass[BoxNumber] = Value;
	else if (strcmp(VariableName, "Phyto PMax*RTPH") == 0)
		Pmax[BoxNumber] = Value / RTPH;
	else if (strcmp(VariableName, "Phyto Update") == 0)
		PhytoBalance[BoxNumber] = Value;
	else if (strcmp(VariableName, "N Counter") == 0)
		NCounter = Value;
	else if (strcmp(VariableName, "P Counter") == 0)
		PCounter = Value;
	else if (strcmp(VariableName, "Phytoplankton mortality") == 0)
		PhytoMortality[BoxNumber] = Value;
    else if (strcmp(VariableName, "Chlorophyll to Carbon") == 0)
		ChlToCarbon[BoxNumber] = Value;
    else
        rc = false;

    return rc;
}

void TPhytoplankton::Inquiry(char* srcName, double &Value,
											int BoxNumber,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
	int MyBoxNumber = BoxNumber;
	strcpy(MyParameter, ParameterName);

	if (strcmp(MyParameter, "Phytoplankton biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton C biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytoplankton N biomass") == 0)
		Value = PhytoBiomass[MyBoxNumber] * RedfieldNFactor / RedfieldCFactor;
    else if (strcmp(MyParameter, "Phytop. total GPP") == 0)
		Value = GPP[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytop. total NPP") == 0)
		Value = NPP[MyBoxNumber];
    else if (strcmp(MyParameter, "Phytop. GPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
			Value = GPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTODAYS;              // mg C/ mg C /h
    }
	else if (strcmp(MyParameter, "Phytop. NPP per unit mass") == 0)
    {
        if (PhytoBiomass[MyBoxNumber] <= 0.0)
            Value = 0.0;
        else
		    Value = NPP[MyBoxNumber]
					  / PhytoBiomass[MyBoxNumber]
					  * HOURSTODAYS;
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
	else if (strcmp(MyParameter, "Phyto PMax*RTPH") == 0)
		Value = RTPH * Pmax[MyBoxNumber];
	else if (strcmp(MyParameter, "Phyto Update") == 0)
		Value = PhytoBalance[MyBoxNumber];
	else if (strcmp(MyParameter, "N Counter") == 0)
		Value = NCounter;
	else if (strcmp(MyParameter, "P Counter") == 0)
		Value = PCounter;
	else if (strcmp(MyParameter, "Phytoplankton mortality") == 0)
		Value = PhytoMortality[MyBoxNumber];
    else if (strcmp(MyParameter, "Chlorophyll to Carbon") == 0)
		Value = ChlToCarbon[MyBoxNumber];

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
                      "EcoDynamo Alert - Inquiry 2",
                      MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
						cerr << "TPhytoplankton:Inquiry 2 - "<< MyParameter << "does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
        }
    }
#endif  // !(_PORT_FORTRAN_)
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TPhytoplankton::Inquiry(char* srcName, double &Value,
											int BoxX,
											int BoxY,
											char* ParameterName,
											int AnObjectCode)
{
	char MyParameter[65]; // 64 characters (max) for parameter name
    int MyBoxNumber = BoxX * NumberOfBoxes + BoxY;
	strcpy(MyParameter, ParameterName);
/*
	if (strcmp(MyParameter, "Phytop. advective flux matrix") == 0) // kg C for the integration interval - checked 96.06.24 JGF
    {
		Value = PhytoplanktonInterfaceExchange[MyBoxNumber].AdvectiveFlow // in tons
		    						  * TimeStep
									  * DAYSTOSECONDS
									  / (CUBIC * CUBIC);
	}
	else if (strcmp(MyParameter, "Phytop. dispersive flux matrix") == 0) // kg C for the integration interval - checked 96.06.24 JGF
    {
		Value = PhytoplanktonInterfaceExchange[MyBoxNumber].DispersiveFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
									  / (CUBIC * CUBIC);
	}
	else if (strcmp(MyParameter, "Phytop. net flux matrix") == 0) // kg C for the integration interval - checked 96.06.24 JGF
    {
		Value = PhytoplanktonInterfaceExchange[MyBoxNumber].InFlow // in tons
									  * TimeStep
									  * DAYSTOSECONDS
									  / (CUBIC * CUBIC);
	}
    else
*/
#ifndef _PORT_FORTRAN_
    {
#ifdef __BORLANDC__
    	char Caption[129];
    	strcpy(Caption, MyParameter);
    	strcat(Caption, " does not exist in ");
    	strcat(Caption, GetEcoDynClassName());
    	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Inquiry 3",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
			cerr << "TPhytoplankton:Inquiry 3 - "<< MyParameter << "does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
    }
#endif  // !(_PORT_FORTRAN_)
    LogMessage("Inquiry", srcName, MyParameter, Value, MyBoxNumber);
}

void TPhytoplankton::Update(char* srcName, double Value,
									  int BoxNumber,
									  char* ParameterName,
									  int AnObjectCode)
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

	for (int i=0; i < NumberOfVariables; i++)
		if (strcmp(MyParameter, VariableNameArray[i]) == 0)
		{
			if (strcmp(MyParameter, "Phytoplankton biomass") == 0)
				PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + Value;
			return;
		}

#ifndef _PORT_FORTRAN_
#ifdef __BORLANDC__
	char Caption[129];
	strcpy(Caption, MyParameter);
    strcat(Caption, " does not exist in ");
    strcat(Caption, GetEcoDynClassName());
	MessageBox(0,
				  Caption,
				  "EcoDynamo Alert - Update 1",
				  MB_OK | MB_ICONHAND);
#else  // __BORLANDC__
	cerr << "TPhytoplankton:Update 1 - " << MyParameter << "does not exist in " << GetEcoDynClassName() << endl;
#endif  // __BORLANDC__
#endif  // !(_PORT_FORTRAN_)
}

void TPhytoplankton::Integrate()
{
	// Integrate phytoplankton biomass within the box
//	Generic = PhytoBiomass; GenericFlux = PhytoFlux;
	Integration(PhytoBiomass, PhytoFlux);
//	PhytoBiomass = Generic; PhytoFlux = GenericFlux;
	// Transport the phytoplankton
	TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
	if (MyTransportPointer != NULL)
	{

	// Old calls

//		MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);

		// New calls for boundary exchange

		MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);

		//GenericInterfaceExchange = MyTransportPointer->Exchange;

		// Important note - The pointer variables must be passed like this (below) to a
		// pointer which does not point to the Exchange (in transport (or GIE - here) address
		// otherwise, the pointer starting address is retained, and as exchange changes, so
		// do the results in DINInterface exchange - therefore, the real values must be
		// specifically converted and assigned one by one - wow 96.06.24 JGF

		/*for (int i = 0; i < NumberOfBoxes; i++)
			for (int j = 0; j < NumberOfBoxes; j++)
			{
				double Value1 = GenericInterfaceExchange[i * NumberOfBoxes + j].AdvectiveFlow;
				PhytoplanktonInterfaceExchange[i * NumberOfBoxes + j].AdvectiveFlow = Value1;
				double Value2 = GenericInterfaceExchange[i * NumberOfBoxes + j].DispersiveFlow;
				PhytoplanktonInterfaceExchange[i * NumberOfBoxes + j].DispersiveFlow = Value2;
				PhytoplanktonInterfaceExchange[i * NumberOfBoxes + j].InFlow = Value1 + Value2; // net
			} */
	}
    Loads();
}

void TPhytoplankton::ProductionWithTemperature()
{
	//const double KValue = 0.25;
	double BoxDepth, LightAtTop, LightAtBottom, AverageLight;

	double rTempF;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				 * MySPMPointer = MyPEcoDynClass->GetSPMPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		//BoxDepth = MyPEcoDynClass->GetBoxDepth(i);

		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

	 if (MyLightPointer != NULL)
	 {

		 MyLightPointer->Inquiry(GetEcoDynClassName(), AverageLight,
											  i,
											  "Mean horizontal water PAR irradiance",
											  ObjectCode);
		 MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
											  i,
											  "PAR surface irradiance",
											  ObjectCode);

		 BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
		 //Debugger (BoxDepth);
		 MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
											  BoxDepth,
											  i,
											  "Sub-surface PAR irradiance",
											  ObjectCode);
		 /*if (AverageLight >> Iopt)
		 CriticalExposureTime[i] = st */

		 //NG 10/6/96
		 double WaterColumnTemperature;
		 TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();


		 if (MyWaterTemperaturePointer != NULL)

			MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterColumnTemperature,
																i,
																"Water temperature",
																ObjectCode);

		 else
			WaterColumnTemperature =12;

		 rTempF = 2* exp((WaterColumnTemperature-12)*q10PH);

		 RTPH = RTMPH*rTempF/(RTMPH+rTempF-1);




		 // Productivity in mgC m-3 d-1    CHECKED CORRECT 96.05.13 JGF/NG
		 Productivity = PhytoBiomass[i]
							 * Pmax[i]
							 * RTPH
							 * 2.718282
							 /( KValue * MyPEcoDynClass->GetBoxDepth(i) )
							 * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[i] )
								  -exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[i] )
							  )
							  * DAYSTOHOURS;
		 //Debugger (Productivity);
		 NutrientLimitation(i);
		 //PhytoProd[i] = Productivity;
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	 }
  }
}


void TPhytoplankton::Production()
{
	//const double KValue = 0.25;
	double BoxDepth, LightAtTop, LightAtBottom, AverageLight;

	TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
				 * MySPMPointer = MyPEcoDynClass->GetSPMPointer();
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		//BoxDepth = MyPEcoDynClass->GetBoxDepth(i);

		if (MySPMPointer != NULL)
			MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
											  i,
											  "Extinction coefficient Ktot",
											  ObjectCode);

	 if (MyLightPointer != NULL)
	 {

		 MyLightPointer->Inquiry(GetEcoDynClassName(), AverageLight,
											  i,
											  "Mean horizontal water PAR irradiance",
											  ObjectCode);
		 MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
											  i,
											  "PAR surface irradiance",
											  ObjectCode);

		 BoxDepth = MyPEcoDynClass->GetBoxDepth(i);

		 //Debugger (BoxDepth);
		 MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
											  BoxDepth,
											  i,
											  "Sub-surface PAR irradiance",
											  ObjectCode);
		 /*if (AverageLight >> Iopt)
		 CriticalExposureTime[i] = st */
		 Productivity = PhytoBiomass[i]
							 * Pmax[i]
							 * 2.718282
							 /( KValue * MyPEcoDynClass->GetBoxDepth(i) )
							 * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[i] )
								  -exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[i] )
							  )
							  * DAYSTOHOURS;
		 // Debugger (Productivity);
       TemperatureLimitation(i);
		 NutrientLimitation(i);
		 // PhytoProd[i] = Productivity;
		 GPP[i] = Productivity;
		 PhytoProd[i] = Productivity;
	 }
  }
}

void TPhytoplankton::NutrientLimitation(int ABoxNumber)
{

/*
		TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
		if (MyNutrientPointer != NULL)
			MyNutrientPointer->(Exudate/CUBIC,
									  i,
									  "DOC",
									  ObjectCode);
*/
	// NUTRIENT LIMITATION COMPONENT
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
	{
		// At the moment only DIN is used, and
		// the limitation is based on umol l-1 values passed from the
		// communicate method

		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient,
												 ABoxNumber,
												 "DIN",
												 ObjectCode);

		if (LimitingNutrient == 0)
		{
			Productivity = 0;
			return;
		}

		Productivity = Productivity               // Michaelis-Menten formulation
							* LimitingNutrient
							/ (Ks[ABoxNumber] + LimitingNutrient);

		/* To implement the law of limiting factors here (leibig's law)
			we can do one of two things:

		(i) Calculate prod sequentially for different nutrients and
			use the lowest value - that?s the limiting factor, and
			then calculate the balance based on those;

		or

		(ii) Write a 'Redfield' algorithm into dissobj which when
			  consulted will automatically tell us which nutrient
			  to use - the ratios may be 'potted' in the object or
			  overriden by the calling method;
		*/

		// REDFIELD RATIO TO CALCULATE NUTRIENTS REMOVED - BALANCE
		// production is in mgC

		// If production is already negative then do nothing - send balance

		if (Productivity > 0)
		{
			// return negative value because this is subtracted
			MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * CUBIC / (45*14/RedfieldNFactor),
												ABoxNumber,
												"DIN",
												ObjectCode);
		}
	}
}


void TPhytoplankton::TemperatureLimitation(int ABoxNumber)
{
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (MyWaterTemperaturePointer != NULL)
   {
   	double WaterTemperature;
   	int MyBoxNumber;
      MyBoxNumber = ABoxNumber;
   	MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
      Productivity = Productivity *
                           exp(TemperatureAugmentationRate * WaterTemperature);
   }
}

void TPhytoplankton::NutrientLimitationByAmmoniaAndNitrate(int ABoxNumber) // Wroblewski (1977) in Fasham et al., 1990: Journal of Marine Research, 48, 591-639
{
	double LimitingNutrient1, LimitingNutrient2, NitrateLimitation, AmmoniumLimitation;
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
	if (MyNutrientPointer != NULL)
	{
		// the limitation is based on umol l-1 values passed from the
		// communicate method
      MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient1,
												 ABoxNumber,
												 "Ammonia",
												 ObjectCode);
		MyNutrientPointer->Inquiry(GetEcoDynClassName(), LimitingNutrient2,
												 ABoxNumber,
												 "Nitrate",
												 ObjectCode);


		if ((LimitingNutrient1 == 0) && (LimitingNutrient2 == 0))
		{
			Productivity = 0;
			return;
		}

      //Debugger(LimitingNutrient1); Debugger(LimitingNutrient2);



      AmmoniumLimitation = LimitingNutrient1
                     / (KAmmonia[ABoxNumber] + LimitingNutrient1);

      NitrateLimitation = (LimitingNutrient2 * exp(-phi * LimitingNutrient1))
							     / (KNitrate[ABoxNumber] + LimitingNutrient2);               

		Productivity = Productivity  // Wroblewski (1977) in Fasham et al., 1990: Journal of Marine Research, 48, 591-639
							* (NitrateLimitation +
                        AmmoniumLimitation);

		// REDFIELD RATIO TO CALCULATE NUTRIENTS REMOVED - BALANCE
		// production is in mgC

		// If production is already negative then do nothing - send balance

		if (Productivity > 0)
		{
			// return negative value because this is subtracted
			MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * AmmoniumLimitation * CUBIC /
                                   (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
												ABoxNumber,
												"Ammonia",
												ObjectCode);
         MyNutrientPointer->Update(GetEcoDynClassName(), -Productivity * NitrateLimitation * CUBIC /
                                   (RedfieldCFactor*NITROGENATOMICWEIGHT/RedfieldNFactor),
												ABoxNumber,
												"Nitrate",
												ObjectCode);

         //Debugger(-Productivity * CUBIC / (45*14/RedfieldNFactor));
		}
	}
}


void TPhytoplankton::Respiration()
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		if (PhytoProd[i] > 0)
			PhytoProd[i] = PhytoProd[i] - ( PhytoProd[i] * RespirationCoefficient);
		else
			PhytoProd[i] = -MaintenanceRespiration[i] * PhytoBiomass[i];
	}
}

void TPhytoplankton::Exudation()
{
double  Exudate;
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


void TPhytoplankton::Senescence(int ABoxNumber)
{
}

void TPhytoplankton::Mortality()
{
	double DINRelease = 0;

	// Change by NG, 2/6/96   in order to have the corresponding balance with DIN
	TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();


	for (int i = 0; i < NumberOfBoxes; i++)
	{
		DINRelease = PhytoBiomass[i] * DeathLoss;

		PhytoFlux[i] = PhytoFlux[i] - DINRelease;

		PhytoMortality[i] = DINRelease; // flux per day for mass balance

		if (MyNutrientPointer != NULL)
			MyNutrientPointer->Update(GetEcoDynClassName(), DINRelease * CUBIC / (45*14/RedfieldNFactor),
												 i,
												 "DIN",
												 ObjectCode);

	}
}

void TPhytoplankton::Loads()
{
}


// Zoobenthos object CPP code file
// @ Pedro Duarte
// Rev. A 01.11.09
/**
 * NPI work
 * 	07 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
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
#include "zoobobjt.h"
#include "iodll.h"


// TChlamysFarreri Class
//
// Constructors invoked outside EcoDyn shell...
//

TChlamysFarreriV8::TChlamysFarreriV8(char* className, float timeStep,
        int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double aZooShellMass[],
        double aZooIndividualWeight[], double aZooClassDensity[],
        int nParams, double dryWeightOfAStandardAnimal, double allometricCoefficientForFeeding,
        double allometricCoefficientForRespiration, double meatEnergeticContents,
        double shellEnergeticContents, double intrinsicRateOfMortality,
        double summerIntrinsicRateOfMortality, double maintenanceRespiration,
        double activityRespirationCoefficient, double proportionOfEnergyAllocatedToShell,
        double proportionOfEnergyAllocatedToSoftTissue, double ammoniaToJoules,
        double softTissueEnergyToTotalEnergy, double numberOfClasses,
        double totalFWToMeatDW)
    :TChlamysFarreri(className, timeStep, nLines, nColumns,
            aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, aZooShellMass, aZooIndividualWeight,
            aZooClassDensity,
            nParams, dryWeightOfAStandardAnimal, allometricCoefficientForFeeding,
            allometricCoefficientForRespiration, meatEnergeticContents,
            shellEnergeticContents, intrinsicRateOfMortality,
            summerIntrinsicRateOfMortality, maintenanceRespiration,
            activityRespirationCoefficient, proportionOfEnergyAllocatedToShell,
            proportionOfEnergyAllocatedToSoftTissue, ammoniaToJoules,
            softTissueEnergyToTotalEnergy, numberOfClasses,
            totalFWToMeatDW)
{
    BuildChlamysFarreriV8();
}

TChlamysFarreriV8::TChlamysFarreriV8(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TChlamysFarreri(className, timeStep, morphologyFilename,
                variablesFilename, parametersFilename)
{
    BuildChlamysFarreriV8();
}
// ...

TChlamysFarreriV8::TChlamysFarreriV8(TEcoDynClass* APEcoDynClass, char* className)
							 :	TChlamysFarreri(APEcoDynClass, className)
{
    BuildChlamysFarreriV8();
}

void TChlamysFarreriV8::BuildChlamysFarreriV8()
{
	FRPHY = new double[NumberOfBoxes];
    FRPOM = new double[NumberOfBoxes];
    FRDET = new double[NumberOfBoxes];
	RRDET = new double[NumberOfBoxes];
    RRPHY = new double[NumberOfBoxes];
    for (int i = 0; i < NumberOfBoxes; i++)
    {
   	    FRPHY[i] = 0.0;
   	    FRPOM[i] = 0.0;
        FRDET[i] = 0.0;
        RRDET[i] = 0.0;
        RRPHY[i] = 0.0;
    }
}


TChlamysFarreriV8::~TChlamysFarreriV8()
{
    freeMemory();
}

void TChlamysFarreriV8::freeMemory()
{
    try {
         //delete [] VariableNameArray;
        if (NumberOfBoxes > 0) {
            delete [] FRPHY;
            delete [] FRPOM;
           delete [] FRDET;
           delete [] RRDET;
           delete [] RRPHY;
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
				cerr << "TChlamysFarreriV8::freeMemory - " << exc.what() << endl;
		}
#endif  // __BORLANDC__
}

// AP, 2006.06.30
void TChlamysFarreriV8::Reinitialize()
{
  Queue* seedingQueue;
  BUF* pBuf;
  SEED_DB* pSeed;
  SEED_REGION* pSeedRegion;
  BUF* pAux;
  Queue* pBoxes;
  int i, j, n, k, lineIndex, columnIndex, m, boxClassIndex, kSR;
  REGION_BOX* pBox;
  int aditionalBoxes;
  BenthicSpeciesRecord* oldBenthic;
  int RecordNumber, Index;
  TEcoDynClass* MyFisherPointer;

    MyFisherPointer = MyPEcoDynClass->GetFisherPointer();
    if (MyFisherPointer == NULL)
        return;

    seedingQueue = MyFisherPointer->getSeedingQueue();
    if (seedingQueue == NULL)
        return;

    for (i = 0; i < seedingQueue->size(); i++) {
        pBuf = seedingQueue->getElementAt(i);
        pSeed = (SEED_DB*)pBuf->pData;

        for (kSR = 0; kSR < pSeed->seedRegions->size(); kSR++) {
            pAux = pSeed->seedRegions->getElementAt(kSR);
            pSeedRegion = (SEED_REGION*)pAux->pData;
            if (pSeedRegion->bivalve_s.type == BV_TYPE_SCALLOP) {

                // signal TFisherMan that seed already done
                pSeed->seeding = true;

                pBoxes = pSeed->regionList->getRegion(pSeedRegion->regName)->getRegionArea();
                if (pBoxes != NULL) {
                    aditionalBoxes = 0;
                    for (n = 0; n < pBoxes->size(); n++) {
                        pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                        k = GetBoxNumber(pBox->column, pBox->line);
                        if (!boxWithScallops(k)) {   // new box with scallops
                            aditionalBoxes++;
                        }
                    }
                    if (aditionalBoxes > 0) {
                        oldBenthic = ChlamysFarreriRecord;
                        ChlamysFarreriRecord = new BenthicSpeciesRecord[
                                NumberOfBoxesWithChlamysFarreri + aditionalBoxes];
                        for (m = 0; m < NumberOfBoxesWithChlamysFarreri; m++)
                        {
                            strcpy(ChlamysFarreriRecord[m].SpeciesName, "Chlamys farreri");
                            ChlamysFarreriRecord[m].LineCoordinate =
                                    oldBenthic[m].LineCoordinate;
                            ChlamysFarreriRecord[m].ColumnCoordinate =
                                    oldBenthic[m].ColumnCoordinate;
                        }
                        delete [] oldBenthic;

                        // m stands for ChlamysFarreriRecord insert index
                        for (n = 0; n < pBoxes->size(); n++) {
                            pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                            k = GetBoxNumber(pBox->column, pBox->line);
                            if (!boxWithScallops(k)) {   // new box with scallops
                                strcpy(ChlamysFarreriRecord[m].SpeciesName, "Chlamys farreri");
                                ChlamysFarreriRecord[m].LineCoordinate = GetLineIndex(k);
                                ChlamysFarreriRecord[m].ColumnCoordinate = GetColumnIndex(k);
                                m++;
                            }
                        }
                        // new ChlamysFarreriRecord filled
                        NumberOfBoxesWithChlamysFarreri += aditionalBoxes;
                    }

                    //
                    // seed now...
                    //
                    for (k = 0; k < NumberOfClasses; k++)
                    {
                        for (n = 0; n < pBoxes->size(); n++) {
                            pBox = (REGION_BOX*)pBoxes->getElementAt(n)->pData;
                            Index = GetBoxNumber(pBox->column, pBox->line);
                            boxClassIndex = k * NumberOfBoxes + Index;

                            if (ZooClassDensity[boxClassIndex] < 0.000001) {
                                // AP, 2011.01.31 - following SangoMan code

                                ZooShellMass[boxClassIndex] = pSeedRegion->bivalve_s.weight1;
                                ShellEnergeticTotal[boxClassIndex] =
                                        pSeedRegion->bivalve_s.weight1 * ShellEnergeticContents;
                                ZooIndividualWeight[boxClassIndex] = pSeedRegion->bivalve_s.weight2;
                                SoftTissueEnergeticTotal[boxClassIndex] =
                                        pSeedRegion->bivalve_s.weight2 * MeatEnergeticContents;
                                ZooClassDensityFlux[boxClassIndex] +=
                                        (pSeedRegion->density / MyPEcoDynClass->GetTimeStep());

                                /* AP, 2011.01.31...
                                ZooClassBiomass[boxClassIndex] =
                                    ZooClassDensityFlux[boxClassIndex] *
                                    (ZooIndividualWeight[boxClassIndex] +
                                     ZooShellMass[boxClassIndex]);
                                ZooClassDensity[boxClassIndex] =
                                    ZooClassBiomass[boxClassIndex] /
                                    ZooIndividualWeight[boxClassIndex];*/
                            }
                        }
                    }
                }
            }
        }
    }
}

bool TChlamysFarreriV8::boxWithScallops(int boxNumber)
{
  int lineNumber, columnNumber, m;

    lineNumber = GetLineIndex(boxNumber);
    columnNumber = GetColumnIndex(boxNumber);

    for (m = 0; m < NumberOfBoxesWithChlamysFarreri; m++)
    {
        if (lineNumber == ChlamysFarreriRecord[m].LineCoordinate &&
                columnNumber == ChlamysFarreriRecord[m].ColumnCoordinate)
            return true;
    }
    return false;
}

void TChlamysFarreriV8::Go()
{
//   double MyYear;
   JulianDay = MyPEcoDynClass->GetJulianDay();
//   MyYear = MyPEcoDynClass->GetYear();
   int i, j, index;
   double MyDepth, MyChlorophyllCarbonRatio;
   for (int i = 0; i < NumberOfBoxes; i++)
   {
      ZooDINRelease[i] = 0;
      ZooPhytoUptake[i] = 0;
      ZooPOCUptake[i] = 0;
      OrganicPseudoFaeces[i] = 0.0;
      InorganicPseudofaeces[i] = 0.0;
   }
   TEcoDynClass * MySPMPointer = MyPEcoDynClass->GetSPMPointer(),
                * MyPhytoPointer = MyPEcoDynClass->GetPhytoplanktonPointer(),
                * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
   if ((MySPMPointer != NULL) && (MyPhytoPointer != NULL))
   {
      for (int m = 0; m < NumberOfBoxesWithChlamysFarreri; m++)
      {
         i = ChlamysFarreriRecord[m].LineCoordinate;
         j = ChlamysFarreriRecord[m].ColumnCoordinate;
         if (
                (i >= pSubDomain->ILine[j]) && (i <= pSubDomain->FLine[j]) &&
                (j >= pSubDomain->IColumn[i]) && (j <= pSubDomain->FColumn[i])
            )
         {
            index = i * NumberOfColumns + j;
            if (MyTransportPointer != NULL)
                MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,index,"Box depth",ObjectCode);
             else  MyDepth = MyPEcoDynClass->GetBoxDepth(index);
             if (MyDepth > CriticalWaterDepth)
             {
                seston_mass = 0.0; phyto_mass = 0.0;
                PHYORG = 0.0; DETORG = 0.0;
                TPM = 0.0; POM = 0.0; PIM = 0.0;
                PHYCONTENTSPOM = 0.0;
                TOTALFRTPM[index] = 0.0; TOTALFRPOM[index] = 0.0; TOTALFRPHY[index] = 0.0;
                IRPHY[index] = 0.0; IRDET[index] = 0.0; IRTPM[index] = 0.0; IRPIM[index] = 0.0;
                FRTPM[index] = 0.0; FRPIM[index] = 0.0;
                RRTPM[index] = 0.0; RRPIM[index] = 0.0;
                OIR[index] = 0.0;  OCI[index] = 0.0; OCPF[index] = 0.0;
                NAEIO[index] = 0.0;
                NEA[index] = 0.0; NEB[index] = 0.0;
                MySPMPointer->Inquiry(GetEcoDynClassName(), TPM,index,"TPM",ObjectCode);
                MySPMPointer->Inquiry(GetEcoDynClassName(), POM,index,"POM",ObjectCode);
                PIM = TPM - POM;
                MyPhytoPointer->Inquiry(GetEcoDynClassName(), phyto_mass,
                                              index,
                                           "Phytoplankton biomass",
                                                ObjectCode);
                MyPhytoPointer->Inquiry(GetEcoDynClassName(), ChlorophyllToBiomass,
                                              index,
                                           "Chlorophyll to biomass",
                                                ObjectCode);
                MyPhytoPointer->Inquiry(GetEcoDynClassName(), MyChlorophyllCarbonRatio,index,"Chlorophyll : Carbon",ObjectCode);
                // Phytoplankton returns its mass as ug chl a l-1
                // convert to mg l-1 of organic dry matter
                PHYORG = phyto_mass / CUBIC * ChlorophyllToBiomass; //mg/l
                DETORG = POM - PHYORG;
                if (TPM > 0.0000000000000001) OCS = POM / TPM;
                else  OCS = 0.0;
                if (POM > 0.0000000000000001) PHYCONTENTSPOM = PHYORG / POM;
                if (PHYCONTENTSPOM > 1.0) PHYCONTENTSPOM = 1.0;
                for (int k = 0; k < NumberOfClasses; k++)
                {
                   if (
                        ( ZooClassBiomass[k * NumberOfBoxes + index] > 0.0) &&
                        ( ZooClassDensity[k * NumberOfBoxes + index] > 0.0) &&
                        ( ZooIndividualWeight[k * NumberOfBoxes + index] > 0.0)
                      )
                   {
                      Feed(index, k);
                      TOTALFRTPM[index] = TOTALFRTPM[index] + (FRTPM[index] - FRPHY[index]) * ZooClassDensity[k * NumberOfBoxes + index] / MyDepth / CUBIC;
                      TOTALFRPOM[index] = TOTALFRPOM[index] + (FRPOM[index] - FRPHY[index]) * ZooClassDensity[k * NumberOfBoxes + index] / MyDepth / CUBIC;
                      //Subtra? o FRPHY no c?lculo do TOTALFRTPM e TOTALFRPOM porque o objecto phytoplankton de Sango invoca o Update do objecto de
                      //Mat?ria em suspens?o, enviando-lhe o seu fluxo final. Neste fluxo est? integrada a perda do fito por consumo pelos bivalves
                      // que ? o TOTALFRPHY. Deste modo, a subtrac??o acima referida evita que a massa de fito consumida pelos bivalves, seja
                      //subttra?da 2 vezes ao TPM e ao POM
                      TOTALFRPHY[index] = TOTALFRPHY[index] + FRPHY[index]    //mg Org / ind / s
                                           / ChlorophyllToBiomass   //mg Chl / indiv./ s
                                           / MyChlorophyllCarbonRatio       //mg C / indiv./ s
                                           * ZooClassDensity[k * NumberOfBoxes + index] / MyDepth; //mg C / m3 / s
                      //if(index == 207) MessageBox(0, "Go","Respire",MB_OK | MB_ICONHAND);
                      Respire(index, k);
                      Excrete(index, k);
                      Pseudofaeces(index, k);
                      Assimilate(index, k);
                      Reproduce(index, k);
                      ShellDeposition(index, k);
                      SoftTissueGrowth(index, k);
                      Die(index, k);
                   }
                }
             }
             if (MySPMPointer != NULL)
             {
                MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRTPM[index], index,"TPM",ObjectCode);
                MySPMPointer->Update(GetEcoDynClassName(), -TOTALFRPOM[index],index,"POM",ObjectCode);
             }
             if (MyPhytoPointer != NULL)
                 MyPhytoPointer->Update(GetEcoDynClassName(), -TOTALFRPHY[index], //ug Chla /l
                                                                  index,"Phytoplankton biomass", ObjectCode);
         }
      }
   }
   else
#ifdef __BORLANDC__
        MessageBox(0, "Bivalves can not run without phytoplankton and suspended matter", "EcoDynamo alert", MB_OK);
#else  // __BORLANDC__
			cerr << "TChlamysFarreriV8::Go - Bivalves can not run without phytoplankton and suspended matter" << endl;
#endif  // __BORLANDC__
}



void TChlamysFarreriV8::Feed(int ABox, int AClass)
{
	int MyBox, MyClass;
   double PHYCNFPOM, RRFRPHYORG, MyDepth;

   MyBox = ABox;
   MyClass = AClass;
   TEcoDynClass * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (MyTransportPointer != NULL)
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyDepth,
			  											  MyBox,
			 											  "Box depth",
														  ObjectCode);
   else
      MyDepth = MyPEcoDynClass->GetBoxDepth(MyBox);
   //Detritus filtration, rejection and ingestion

   if(DETORG > 0.0001)
   {
   	FRDET[MyBox] = (0.542 + 0.586 * DETORG)* TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding)
               / HOURSTOSECONDS
               /** DAYSTOHOURS*/; //mg org / s  / ind
      /*if (FRDET[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > DETORG)
      {
        FRDET[MyBox]= DETORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC/ ATimeStep;
      }
      if (FRDET[MyBox] < 0.0) FRDET[MyBox] = 0.0; */
      //Debugger(FRDET[MyBox] * DAYSTOSECONDS);
   }
   else
      FRDET[MyBox] = 0.0;


   if (FRDET[MyBox] * HOURSTOSECONDS > 0.0194)
      RRDET[MyBox] = (-0.00674 + 0.348 * FRDET[MyBox] * DAYSTOSECONDS) / DAYSTOSECONDS;
   else RRDET[MyBox] = 0.0;
   //Debugger(FRDET[MyBox] * DAYSTOSECONDS); Debugger(RRDET[MyBox] * DAYSTOSECONDS);
   IRDET[MyBox] = FRDET[MyBox] - RRDET[MyBox];

   //Phytoplankton filtration

   if((POM + 3.0 > exp(-1)) && (PHYORG > 0.0001))
   {
   	/*Debugger(POM);
   	Debugger(POM+3.0);
      Debugger(PHYORG);
      Debugger(log(POM+3.0));
      Debugger(TemperatureEffectsOnFeedingRate(MyBox));*/
      //Debugger(AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding));
      //Debugger(exp(7.5856 + (18.11*(log(log(POM + 3.0))))+(-10.5*(log(POM + 3.0)))+(0.6919*PHYORG)));
   	FRPHY[MyBox] = (exp(7.5856 + (18.11*(log(log(POM + 3.0))))+(-10.5*(log(POM + 3.0)))+(0.6919*PHYORG))*
               TemperatureEffectsOnFeedingRate(MyBox) *
               AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding))
               / HOURSTOSECONDS
               /**DAYSTOHOURS*/; //mg phy-org / s / ind

      /*if (FRPHY[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > PHYORG)
      {
        FRPHY[MyBox]= PHYORG / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC/ ATimeStep;
      } */
      if (FRPHY[MyBox] < 0.0) FRPHY[MyBox] = 0.0;
   }
   else
      FRPHY[MyBox] = 0.0;


   // POM filtration
   FRPOM[MyBox] = FRDET[MyBox] + FRPHY[MyBox];

   //Phytoplankton rejection and ingestion
   PHYCNFPOM = FRPHY[MyBox] / FRPOM[MyBox];

   RRFRPHYORG = 1.0 - 0.895 * PHYCNFPOM;
   if (RRFRPHYORG < 0.0) RRFRPHYORG = 0.0;

   RRPHY[MyBox] = FRPHY[MyBox] * RRFRPHYORG;

   IRPHY[MyBox] = FRPHY[MyBox] - RRPHY[MyBox];

   //PIM filtration, rejection and ingestion

   if (PIM > 0.0001)
   {
   	FRPIM[MyBox] = (19.06 * (1.0 - exp(-0.110 * (PIM - 1.87))) *
      	            TemperatureEffectsOnFeedingRate(MyBox) *
         	         AllometricCorrection(ZooIndividualWeight[MyClass * NumberOfBoxes + MyBox],AllometricCoefficientForFeeding))
                     / HOURSTOSECONDS
            	     /** DAYSTOHOURS*/;   //mg / d
      if (FRPIM[MyBox] * ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC * ATimeStep > PIM)
      {
        //Debugger(3);
        FRPIM[MyBox]= PIM / ZooClassDensity[MyClass * NumberOfBoxes + MyBox] / MyDepth / CUBIC / ATimeStep;
      }
      if (FRPIM[MyBox] < 0.0) FRPIM[MyBox] = 0.0;
   }
   else
   	FRPIM[MyBox] = 0.0;


   RRPIM[MyBox] = (-0.841 + 0.936 * FRPIM[MyBox] * DAYSTOSECONDS) / DAYSTOSECONDS; //mg / s
   if (RRPIM[MyBox] < 0.0)
   RRPIM[MyBox] = 0.0;

   IRPIM[MyBox] = FRPIM[MyBox] - RRPIM[MyBox];

   //TPM filtration, rejection and ingestion

   FRTPM[MyBox] = FRDET[MyBox] + FRPHY[MyBox] + FRPIM[MyBox];
   RRTPM[MyBox] = RRDET[MyBox] + RRPHY[MyBox] + RRPIM[MyBox];
   IRTPM[MyBox] = IRDET[MyBox] + IRPHY[MyBox] + IRPIM[MyBox];

   if (TPM > 0.0000000000000001)
   	ClearanceRate[MyClass * NumberOfBoxes + MyBox] = FRTPM[MyBox] / TPM * DAYSTOSECONDS; //L / d / ind
   else
      ClearanceRate[MyClass * NumberOfBoxes + MyBox] = 0.0;

   //Debugger(ClearanceRate[20]);

   OIR[MyBox] = IRPHY[MyBox] + IRDET[MyBox]; //mg / d


   if (IRTPM[MyBox] > 0.0000000000000001)
      OCI[MyBox] =  (IRDET[MyBox] + IRPHY[MyBox]) / IRTPM[MyBox]; //fraction
   else
      OCI[MyBox] = 0.0;

   if (OCI[MyBox] > 0.0000000000000001)
      OCPF[MyBox] = OCS * OCS / OCI[MyBox];  //fraction
   else
      OCPF[MyBox] = 0.0;
   if (OCPF[MyBox] > 1.0)
   	OCPF[MyBox] = 1.0;


   if (OCI[MyBox] > 0.0000000000000001)
   	NAEIO[MyBox] = 1.12 - 0.129 * 1.0 / OCI[MyBox]; //fraction
   else
      NAEIO[MyBox] = 0.0;
   if (NAEIO[MyBox] < 0.0)
   NAEIO[MyBox] = 0.0;


   NEA[MyBox] = ((23.5 * IRPHY[MyBox]) +(6.1 * IRDET[MyBox])) * NAEIO[MyBox]; //J /d / ind
}



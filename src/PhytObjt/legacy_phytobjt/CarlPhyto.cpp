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

// Carlingford phytoplankton
//
// Constructors invoked outside EcoDyn shell...
//

/*TCarlPhytoplankton::TCarlPhytoplankton(char* className,
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
    :TRiaF2DPhytoplankton(className, timeStep, nLines, nColumns,
            aDepth, aLength, aWidth, aElevation,
            aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, aChlToCarbon, riverPhyto,
            oceanPhyto, aPhytoBiomass, aPhytoLoad, aChlCarbonRatio, aChlToBiomass,
            nParams, pmax, iopt, slope, aEiler, bEiler, cEiler, kValue, ks,
            kAmmonia, kNitrate, pHi, maintenanceRespiration, respirationCoefficient,
            docLoss, docStressLoss, deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate, fallingSpeed,
            ratioLightDarkRespiration, Q10PH, rTMPH)
{
    BuildCarlPhyto();
}

TCarlPhytoplankton::TCarlPhytoplankton(char* className, float timeStep,
    char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TRiaF2DPhytoplankton(className, timeStep, morphologyFilename,
                variablesFilename, parametersFilename)
{
    BuildCarlPhyto();
}
// ... */


TCarlPhytoplankton::TCarlPhytoplankton(TEcoDynClass* APEcoDynClass, char* className)
							: TRiaF2DPhytoplankton(APEcoDynClass, className)
{
    BuildCarlPhyto();
}

void TCarlPhytoplankton::BuildCarlPhyto()
{
	KValue = 0.1;
}


void TCarlPhytoplankton::Go ()
{
    // Active methods
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    JulianDay = MyPEcoDynClass->GetJulianDay();
    MyNumberOfBoxesInSubDomain = pSubDomain->NumberOfBoxes;
    Production();
    Respiration();
    Exudation();
    Mortality();
    //LightShadeAdaptation();
    for (int i = 0; i < NumberOfBoxes; i++)
    {
        NPP[i] = PhytoProd[i];
        PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
    }
}

void TCarlPhytoplankton::Integrate()
{
    double const Tinny = 0.000001;
    double Depth;
    int index;
    SubDomain *pSubDomain = MyPEcoDynClass->GetSubDomain();
    // Integrate phytoplankton biomass within the box
//    Generic = PhytoBiomass; GenericFlux = PhytoFlux;
    Integration(PhytoBiomass, PhytoFlux);
//    PhytoBiomass = Generic; PhytoFlux = GenericFlux;

    // Trick for Carlingford outer box - 96.06.27
    // OceanPhyto = PhytoBiomass[NumberOfBoxes-1];

    // Transport the phytoplankton
    TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
    if (MyTransportPointer != NULL)
    {
        // New calls for boundary exchange
        MyTransportPointer->Go(PhytoBiomass, PhytoLoad, RiverPhyto, OceanPhyto);
    }
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
      index = pSubDomain->BoxNumber[k];
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
      }
      if (PhytoBiomass[index] < 0.0) PhytoBiomass[index] = 0.0;
    }
    Loads();
}


void TCarlPhytoplankton::Production()
{
    double BoxDepth, LightAtTop, LightAtBottom, AverageLight;

    TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
                             * MySPMPointer = MyPEcoDynClass->GetSPMPointer();
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
       int i = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
       //BoxDepth = MyPEcoDynClass->GetBoxDepth(i);
       if (MySPMPointer != NULL)
       MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,i,"Extinction coefficient Ktot",ObjectCode);
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
           Productivity = PhytoBiomass[i]* Pmax[i]* 2.718282/( KValue * MyPEcoDynClass->GetBoxDepth(i) )
                                                   * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[i] )
                                                            -exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[i] )
                                                    )* DAYSTOHOURS;
           // Debugger (Productivity);
           TemperatureLimitation(i);
           NutrientLimitation(i);
           // PhytoProd[i] = Productivity;
           GPP[i] = Productivity;
           PhytoProd[i] = Productivity;
       }
    }
}

void TCarlPhytoplankton::NutrientLimitation(int ABoxNumber)
{

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


void TCarlPhytoplankton::Respiration()
{
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
        int i = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
        if (PhytoProd[i] > 0)
           PhytoProd[i] = PhytoProd[i] - ( PhytoProd[i] * RespirationCoefficient);
        else
           PhytoProd[i] = -MaintenanceRespiration[i] * PhytoBiomass[i];
    }
}

void TCarlPhytoplankton::Exudation()
{
    double Exudate;
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
        int i = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
        DocStressLoss = DocLoss;
        if (GPP[i] > 0)
        Exudate = GPP[i] * DocStressLoss;
        else Exudate = 0;
        PhytoProd[i] = PhytoProd[i] - Exudate;
    }
}

void TCarlPhytoplankton::Mortality()
{
    double DINRelease = 0;
    // Change by NG, 2/6/96   in order to have the corresponding balance with DIN
    TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
        int i = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
            DINRelease = PhytoBiomass[i] * DeathLoss;

            PhytoFlux[i] = PhytoFlux[i] - DINRelease;

            PhytoMortality[i] = DINRelease; // flux per day for mass balance

            if (MyNutrientPointer != NULL)
                    MyNutrientPointer->Update(GetEcoDynClassName(),
                                              DINRelease * CUBIC / (45*14/RedfieldNFactor),
                                              i,
                                              "DIN",
                                              ObjectCode);
    }
}


void TCarlPhytoplankton :: LightShadeAdaptation()
{
    double MyNoonLight, MyAverageLight, Lambda;
    for (int k = 0; k < MyNumberOfBoxesInSubDomain; k++)
    {
        int i = MyPEcoDynClass->GetSubDomain()->BoxNumber[k];
        TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer();
        if (MyLightPointer != NULL)
        {
            MyLightPointer->Inquiry(GetEcoDynClassName(), MyNoonLight,
                                    i,
                                    "Noon surface PAR",
                                    ObjectCode);
            MyLightPointer->Inquiry(GetEcoDynClassName(), MyAverageLight,
                                    i,
                                    "Mean horizontal water PAR irradiance",
                                    ObjectCode);
            MyNoonLight = MyNoonLight * WATTSTOMICROEINSTEINS;
            MyAverageLight = MyAverageLight * WATTSTOMICROEINSTEINS;
            //Debugger (MyNoonLight);
            if (MyAverageLight / MyNoonLight < 0.01)
               OptimalChlCarbonRatio[i] = 0.04;
            else
               OptimalChlCarbonRatio[i] = 0.01 +0.03 *log (MyAverageLight / MyNoonLight) /log (0.01);
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
        }
    }
}



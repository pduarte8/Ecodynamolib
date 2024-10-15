// PhytoplanktonGeneric object CPP code file

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

#ifndef _PORT_FORTRAN_
	#include "iodll.h"
#endif

#include "ecodyn.rh"
#include "phytobjt.h"


// TPhytoplanktonGeneric Class
//
#ifdef _PORT_FORTRAN_
//-----------------------------------------------
// ...Methods invoked from Fortran...
//
//        only one box
//
//-----------------------------------------------
//        #include "PhytoplanktonFortran.h"

#else

// TPhytoplanktonGeneric Class
//
// Constructors invoked outside EcoDyn shell...
//
TPhytoplanktonGeneric::TPhytoplanktonGeneric(char* className,
        float timeStep, int nLines, int nColumns, double aDepth[],
        double aLength[], double aWidth[], double aElevation[], int aType[],
        int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
        int nVars, VNA aVarsNames[], double chlCarbonRatio, double chlToCarbon,
        double chlToBiomass, double aPhytoBiomass[], double riverPhyto,
        double phytoplanktonInitialBiomass, double phytoplanktonFinalBiomass,
        double simulationInterval, double nCellQuota, double pCellQuota,
        int nParams, double pmax, double iopt, double slope, double aEiler,
        double bEiler, double cEiler, double kValue, double ks, double kAmmonia,
        double kNitrate, double pHi, double maintenanceRespiration,
        double respirationCoefficient, double docLoss, double docStressLoss,
        double deathLoss, double redfieldCFactor, double redfieldNFactor,
        double redfieldPFactor, double temperatureAugmentationRate,
        double fallingSpeed, double ratioLightDarkRespiration, double minNPRatio,
        double maxNPRatio, double pMaxUptake, double nMaxUptake, double kP,
        double kNO3, double kNH4, double minPCellQuota, double maxPCellQuota,
        double minNCellQuota, double maxNCellQuota, double kPInternal,
        double kNInternal, double settlingSpeed, double carbonToOxygenProd,
        double carbonToOxygenResp, double tminRespiration, double tminPhotosynthesis)
    :TCrestumaLeverPhytoplankton2DVIntLim(className,
            timeStep, nLines, nColumns, aDepth,
            aLength, aWidth, aElevation, aType,
            aNBoundary, aEBoundary, aSBoundary, aWBoundary,
            nVars, aVarsNames, chlCarbonRatio, chlToCarbon,
            chlToBiomass, aPhytoBiomass, riverPhyto,
            phytoplanktonInitialBiomass, phytoplanktonFinalBiomass,
            simulationInterval, nCellQuota, pCellQuota,
            nParams, pmax, iopt, slope, aEiler,
            bEiler, cEiler, kValue, ks, kAmmonia,
            kNitrate, pHi, maintenanceRespiration,
            respirationCoefficient, docLoss, docStressLoss,
            deathLoss, redfieldCFactor, redfieldNFactor,
            redfieldPFactor, temperatureAugmentationRate,
            fallingSpeed, ratioLightDarkRespiration, minNPRatio,
            maxNPRatio, pMaxUptake, nMaxUptake, kP,
            kNO3, kNH4, minPCellQuota, maxPCellQuota,
            minNCellQuota, maxNCellQuota, kPInternal,
            kNInternal, settlingSpeed, carbonToOxygenProd,
            carbonToOxygenResp, tminRespiration, tminPhotosynthesis)
{
    SurfaceCell = true;
}

TPhytoplanktonGeneric::TPhytoplanktonGeneric(char* className,
    float timeStep, char* morphologyFilename, char* variablesFilename, char* parametersFilename)
        :TCrestumaLeverPhytoplankton2DVIntLim(className, timeStep, morphologyFilename,
            variablesFilename, parametersFilename)
{
    SurfaceCell = true;
}
// ...


TPhytoplanktonGeneric::TPhytoplanktonGeneric(TEcoDynClass* APEcoDynClass,
    char* className)
        :TCrestumaLeverPhytoplankton2DVIntLim(APEcoDynClass, className)
{
    SurfaceCell = true;
    aMin = 0.00000000001; 
}

#endif  //_PORT_FORTRAN_

TPhytoplanktonGeneric::~TPhytoplanktonGeneric()
{
}

void TPhytoplanktonGeneric::Go()
{
	for (int i = 0; i < NumberOfBoxes; i++)
	{
        Production(i);
        Respiration(i);
//        Exudation(i);
//        NitrogenUptake(i);
//        PhosphorusUptake(i);
//        Mortality(i);
//        Settling(i);
	}
	for (int i = 0; i < NumberOfBoxes; i++)
	{
		NPP[i] = PhytoProd[i];
		PhytoFlux[i] = PhytoFlux[i] + PhytoProd[i];
    }
}


void TPhytoplanktonGeneric::Production(int ABoxNumber)
//Argumentos necess?rios:
//N? de ordem da c?lula
//Nome da equa??o a utilizar "Steele", "Michaelis-Menten", "Eiler"
//Valores dos par?metros das equa??es: Pmax e Iopt no caso da primeira,
//Pmax e Imax, no caso da segunda
//AEiler, BEiler e CEiler, no caso da terceira
//Intensidade luminosa PAR no topo e no fundo da c?lula (LightAtTop, LightAtBottom)
//Coeficiente de extin??o da luz  (KValue)
{
   int MyBoxNumber = ABoxNumber;
    //No caso de a classe ser invocada a partir do EcoDyn...
#ifndef _PORT_FORTRAN_
    TEcoDynClass* MySPMPointer = MyPEcoDynClass->GetSPMPointer();
    GetLightIntensity();
    if (MySPMPointer != NULL)
		    MySPMPointer->Inquiry(GetEcoDynClassName(), KValue,
                                                        MyBoxNumber,
														"Extinction coefficient Ktot",
														ObjectCode);
    //...
#else
    BoxDepth = GetBoxDepth(MyBoxNumber);
#endif
    switch (EquationName)
    {
      case STEELE:	// add a list item
        Productivity = SteeleProduction();
        break;
      case MICHAELIS_MENTEN:	// add a list item
        Productivity = Michaelis_MentenProduction();
        break;
      case EILER:	// add a list item
        Productivity = EilerProduction();
        break;
    }
    //Productivity = Productivity * TemperatureArrheniusExponentialLimitation();
    //NutrientLimitation(MyBoxNumber);
    //DailyAverageProduction();
    GPP[MyBoxNumber] = Productivity;
    PhytoProd[MyBoxNumber] = Productivity;
    PhytoFlux[MyBoxNumber] = Productivity;
}

void TPhytoplanktonGeneric::GetLightIntensity()
{
   double ATop, ABottom;
   int MyBoxNumber = ABoxNumber;
   TEcoDynClass* MyLightPointer = MyPEcoDynClass->GetLightPointer(),
               * MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   if (PhytoBiomass[MyBoxNumber] > 0.0)
   {
      if (MyLightPointer != NULL)
      {
         if (MyTransportPointer != NULL)
         {
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ATop,
            			                                      MyBoxNumber,
               			                                      "Top depth",
                  			                                  ObjectCode);
              MyTransportPointer->Inquiry(GetEcoDynClassName(), ABottom,
            			                                      MyBoxNumber,
               			                                      "Bottom depth",
                  			                                  ObjectCode);
              BoxDepth = ABottom - ATop;
         }
         else
         {
            BoxDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
                  ATop = 0.0; ABottom = BoxDepth;
         }
         if (SurfaceCell)
            MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												       	MyBoxNumber,
												       	"PAR surface irradiance",
												       	ObjectCode);
         else
            MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtTop,
												       ATop,
												       MyBoxNumber,
												       "Sub-surface PAR irradiance",
												       ObjectCode);
         MyLightPointer->Inquiry(GetEcoDynClassName(), LightAtBottom,
												       ABottom,
												       MyBoxNumber,
												       "Sub-surface PAR irradiance",
												       ObjectCode);
      }
   }
}


double TPhytoplanktonGeneric::SteeleProduction()
{
   int MyBoxNumber = ABoxNumber;
   if (/*(PhytoBiomass[MyBoxNumber] > aMin) &&*/
       (BoxDepth > aMin) && (LightAtTop > aMin))
   {
         /*cout << "Light at top = "<< LightAtTop << endl;
         cout<< "Light at bottom = "<< LightAtBottom << endl;
         cout<< "Biomass= "<< PhytoBiomass[MyBoxNumber]<< endl;
         cout<< "Pmax= "<< Pmax[MyBoxNumber] << endl;
         cout<< "Iopt= "<< Iopt[MyBoxNumber] << endl;
         cout<< "BoxDepth= "<< BoxDepth << endl;
         cout<<"HOURSTOSECONDS= "<<HOURSTOSECONDS<<endl;
         cout<< "KValue= "<< KValue << endl;
         cout<<"WATTSTOMICROEINSTEINS= "<<WATTSTOMICROEINSTEINS<<endl;
         cout<<"PhytoBiomass= "<<PhytoBiomass[MyBoxNumber]<<endl;*/
         
         Productivity = PhytoBiomass[MyBoxNumber]* Pmax[MyBoxNumber]
                        * 2.718282
                        /(KValue * BoxDepth)
                        * ( exp ( -LightAtBottom * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                        -exp( -LightAtTop * WATTSTOMICROEINSTEINS / Iopt[MyBoxNumber] )
                        )/ HOURSTOSECONDS;    //mg C m-3 s-1;
         /*cout<< "Productivity= "<< Productivity/PhytoBiomass[MyBoxNumber]*HOURSTOSECONDS<< endl;
         cout<< "TemperatureAugmentationRate= " << TemperatureAugmentationRate<<endl;
         cout<< "WaterTemperature="<< WaterTemperature<<endl;
         cout<< "Tmin="<<Tmin<<endl;*/  
    }
    else Productivity = 0.0;
    return Productivity;
}

double TPhytoplanktonGeneric::Michaelis_MentenProduction()
{
   int MyBoxNumber = ABoxNumber;
   TEcoDynClass * MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   if (/*(PhytoBiomass[MyBoxNumber] > aMin) &&*/
       (BoxDepth > aMin) && (LightAtTop > aMin))
   {
  
         Productivity = PhytoBiomass[MyBoxNumber]*
                                    Pmax[MyBoxNumber] /
                                    KValue *
                                    log(fabs(Imax[MyBoxNumber] + LightAtTop * WATTSTOMICROEINSTEINS) /
                                        fabs(Imax[MyBoxNumber] + LightAtBottom * WATTSTOMICROEINSTEINS)
                                       ) *
                                    DAYSTOHOURS /
                                    BoxDepth;
   }
   else Productivity = 0.0;
   return Productivity;
}

double TPhytoplanktonGeneric::EilerProduction()
{
   int i = ABoxNumber;
   double D, B1, B2;
   if (/*(PhytoBiomass[i] > aMin) &&*/
       (BoxDepth > aMin) && (LightAtTop > aMin))
   {
         if (AEiler[i] != 0.0)
         {
            D = BEiler[i] * BEiler[i] - 4.0 * AEiler[i] * CEiler[i];
            B1 = 2*AEiler[i] * LightAtTop * WATTSTOMICROEINSTEINS + BEiler[i];
            B2 = 2*AEiler[i] * LightAtBottom * WATTSTOMICROEINSTEINS + BEiler[i];
            if (D < 0.0)
            {
                Productivity = PhytoBiomass[i] *2.0 / (KValue*sqrt(-D)) *(atan(B1/sqrt(-D))- atan(B2/sqrt(-D)))/ BoxDepth * DAYSTOHOURS;   //mg C m-3 s-1;
            }
            else if (D == 0.0)
            {
                Productivity = PhytoBiomass[i] *
											2.0 / KValue *
											(1.0 / B2 - 1.0 / B1)
											/ BoxDepth
											* DAYSTOHOURS;   //mg C m-3 s-1;

            }
            else if (D > 0.0)
            {
                Productivity = PhytoBiomass[i] *
											1.0 / (KValue*sqrt(D)) *
											log (((B1 - sqrt(D))*(B2 + sqrt(D)))/
											((B1 + sqrt(D)) * (B2 - sqrt(D))))
											/ BoxDepth
											* DAYSTOHOURS;  //mg C m-3 s-1;
            }
         }
         if (AEiler[i] == 0)
					Productivity = PhytoBiomass[i] * 1.0 / (KValue * BEiler[i]) *
										log (fabs(BEiler[i] * LightAtTop * WATTSTOMICROEINSTEINS + CEiler[i])/
										fabs(BEiler[i] * LightAtBottom * WATTSTOMICROEINSTEINS + CEiler[i]))
										/ BoxDepth
										* DAYSTOHOURS;    //mg C m-3 s-1;
   }
   else Productivity = 0.0;
   return Productivity;
}


double TPhytoplanktonGeneric::PlattProduction()
{
   int i = ABoxNumber;
   double LightLimitation, PARLight;
   double IntegrationSteps = 30;
   double DeltaZ, Soma;

   /*cout<<"i = "<<i<<endl;
   cout<<"PhytoBiomass = "<<PhytoBiomass[i]<< endl;
   cout<< "Pmax ="<< Pmax[i]<< endl;
   cout<< "Slope ="<< Slope[i]<< endl;
   cout<< "Beta ="<< beta[i]<< endl;
   cout<< "LightAtTop ="<< LightAtTop* WATTSTOMICROEINSTEINS<< endl;
   cout<< "KValue ="<< KValue<< endl;
   cout<< "BoxDepth ="<< BoxDepth<< endl;*/
   
   Productivity = 0.0;
   if (
       (BoxDepth > aMin) && (LightAtTop > aMin) && (IntegrationSteps >= 1.0))
   {  
         DeltaZ = BoxDepth / IntegrationSteps;
         Soma = 0.0;   
         PARLight = LightAtTop * WATTSTOMICROEINSTEINS; 
         for (int Step = 1; Step <= IntegrationSteps; Step++)    //Euler integration as a function of depth
         {      
            Soma = Soma + (1 - exp(-Slope[i] * PARLight / Pmax[i])) * exp(-beta[i] * PARLight/ Pmax[i]) * DeltaZ;
            PARLight = PARLight * exp(-KValue * DeltaZ);
         }
         LightLimitation = Soma / BoxDepth; 
         Productivity = Pmax[i] * LightLimitation / HOURSTOSECONDS * PhytoBiomass[i]; //mg C m-3 s-1;    
   }
   else Productivity = 0.0;
   return Productivity;
}

double TPhytoplanktonGeneric::SteeleSlope()
{
   int MyBoxNumber = ABoxNumber;
   if (Iopt[MyBoxNumber] > aMin)
      return Pmax[MyBoxNumber] * exp(1.0) / Iopt[MyBoxNumber];//[s-1/(micro mol photons m-2 s-1)]
   else
      return Slope[MyBoxNumber];
}

double TPhytoplanktonGeneric::EilersAndPeetersSlope()
{
   int MyBoxNumber = ABoxNumber;
   if (CEiler[MyBoxNumber] > aMin)
      return 1.0 / CEiler[MyBoxNumber] * HOURSTOSECONDS; //[h-1/(micro mol photons m-2 s-1)]
   else
      return Slope[MyBoxNumber];
}

double TPhytoplanktonGeneric::MichaelisMentenSlope()
{
   int MyBoxNumber = ABoxNumber;
   if (Imax[MyBoxNumber] > aMin)
      return Pmax[MyBoxNumber] / Imax[MyBoxNumber] * HOURSTOSECONDS; //[h-1/(micro mol photons m-2 s-1)] Note that Imax is the light intensity corresponding to Pmax/2 
   else
      return Slope[MyBoxNumber];
}

void TPhytoplanktonGeneric::DailyAverageProduction()
//This routine calculates daily average GPP in oxygen units and it is necessary for respiration calculation,
//when the "  " method is used
//Arguments: Cell number and Chlorophyll to Carbon conversion (when not calculated by the class)
{
   int MyBoxNumber = ABoxNumber;
   if (MyPEcoDynClass->GetJulianDay() != ADay[MyBoxNumber]) 
    {
        DailyMeanGPP[MyBoxNumber] = 0.0;
        NumberOfParcels[MyBoxNumber] = 0;
        ADay[MyBoxNumber] = MyPEcoDynClass->GetJulianDay();
    }
    DailyMeanGPP[MyBoxNumber] =
    (
        DailyMeanGPP[MyBoxNumber] * NumberOfParcels[MyBoxNumber] +
        Productivity / CarbonToOxygenProd / (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * HOURSTOSECONDS
    )  / (NumberOfParcels[MyBoxNumber] + 1);  //mg O2 / mg Chl / h
    NumberOfParcels[MyBoxNumber] = NumberOfParcels[MyBoxNumber] + 1;
}

double TPhytoplanktonGeneric::TemperatureArrheniusExponentialLimitation(int ABoxNumber)
//Needed parameters: Cell number, a temperature augmentation rate, water temperature and a minimal temperature
{
    int MyBoxNumber = ABoxNumber;
      //No caso de a classe ser invocada a partir do EcoDyn...
#ifndef _PORT_FORTRAN_
    TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
    if (MyWaterTemperaturePointer != NULL)
    {
       MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,
                                             MyBoxNumber,
                                             "Water temperature",
                                             ObjectCode);
    }

      else
         WaterTemperature = 0.0;
#endif
    //...
	return exp(TemperatureAugmentationRate * (WaterTemperature - Tmin));
}

void TPhytoplanktonGeneric::Respiration(int ABoxNumber)
{
//Needed parameters: Maintenance respiration in mmol O2 / mg Chl / h
//Water temperature
   int MyBoxNumber;
   double const OxygenMolecularWeight = 32.0;
   double Resp, OxygenProd;
   MyBoxNumber = ABoxNumber;

   //No caso de a classe ser invocada a partir do EcoDyn...
#ifndef _PORT_FORTRAN_
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   TEcoDynClass* MyWaterTemperaturePointer = MyPEcoDynClass->GetWaterTemperaturePointer();
   //...
#endif
   //cout<< "PhytoBiomass = "<< PhytoBiomass[MyBoxNumber] << endl; 
   //cout<< "WaterTemperature = "<< WaterTemperature << endl;
   Resp = 0.0;
   if (PhytoBiomass[MyBoxNumber] > aMin)
   {
      Resp = MaintenanceRespiration[MyBoxNumber] / HOURSTOSECONDS;    //mmol O2 / mgChl / s
      //No caso de a classe ser invocada a partir do EcoDyn...
#ifndef _PORT_FORTRAN_
      if (MyWaterTemperaturePointer != NULL)
         MyWaterTemperaturePointer->Inquiry(GetEcoDynClassName(), WaterTemperature,MyBoxNumber,"Water temperature", ObjectCode);
      else
         WaterTemperature = 0.0;
      //...
#endif
      if (GPP[MyBoxNumber] > 0.0)
      {
         OxygenProd = GPP[MyBoxNumber] / CarbonToOxygenProd / (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]);
         Resp = Resp + RespirationCoefficient * OxygenProd / OxygenMolecularWeight
	   * TemperatureArrheniusExponentialLimitation(MyBoxNumber);//Resp in //mmol O2 / mg Chl / s  
      }
       /*if (GPP[MyBoxNumber] <= 0.0)
         Resp = Resp + RespirationCoefficient * DailyMeanGPP[MyBoxNumber] / OxygenMolecularWeight
	   * TemperatureArrheniusExponentialLimitation(MyBoxNumber)
                / HOURSTOSECONDS;
      else
         Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient * DailyMeanGPP[MyBoxNumber] / OxygenMolecularWeight
	   * TemperatureArrheniusExponentialLimitation(MyBoxNumber)
                / HOURSTOSECONDS;//Resp in //mmol O2 / mg Chl / s  */
      Resp = Resp * CarbonToOxygenResp * (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]) * OxygenMolecularWeight;//Resp in //mg C /m3 / s

      /*cout << "Resp = " << Resp << endl;
      cout << "Maintenance respiration= "<< MaintenanceRespiration[MyBoxNumber] / HOURSTOSECONDS << endl;
      cout << "CarbonToOxygenResp = " << CarbonToOxygenResp << endl;
      cout << "RespirationCoefficient = " << RespirationCoefficient << endl;
      cout << "DailyMeanGPP[MyBoxNumber] = " << DailyMeanGPP[MyBoxNumber] << endl;
      cout << "PhytoBiomass[MyBoxNumber] = " << PhytoBiomass[MyBoxNumber] << endl;
      cout << "ChlToCarbon[MyBoxNumber] = " << ChlToCarbon[MyBoxNumber] << endl;
      cout << "TemperatureArrheniusExponentialLimitation(MyBoxNumber) = " << TemperatureArrheniusExponentialLimitation(MyBoxNumber)<< endl;*/


      PhytoProd[MyBoxNumber] = PhytoProd[MyBoxNumber] - ( Resp );
      
#ifndef _PORT_FORTRAN_
      NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - Resp * NCellQuota[MyBoxNumber];
      PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - Resp * PCellQuota[MyBoxNumber];
      //No caso de a classe ser invocada a partir do EcoDyn...
      if (MyNutrientPointer != NULL)
      {
         MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (NITROGENATOMICWEIGHT / NCellQuota[MyBoxNumber]),MyBoxNumber,"Ammonia",ObjectCode);  //Return as umol N / m3
         MyNutrientPointer->Update(GetEcoDynClassName(), Resp * CUBIC /
                                   (PHOSPHORUSATOMICWEIGHT / PCellQuota[MyBoxNumber]),MyBoxNumber,"Phosphate",ObjectCode);  //Return as umol P / m3
      }
#endif
   }
   RespiredFlux[MyBoxNumber] = Resp;
}





void TPhytoplanktonGeneric::Settling(int ABoxNumber)
//A utiliza??o deste void pressup?e que o fitopl?ncton ? caracterizado, pelo menos, em unidades de carbono, azoto e f?sforo
//Argumentos necess?rios: n? da c?lula, espessura da mesma,
//n? da c?lula superior (caso exista), espessura da mesma,
//boleana a indicar se se trata de uma c?lula junto ao fundo, velocidade de sedimenta??o em m/d

{
   double MyDepth /*espessura da celula*/,
          MyUpperDepth, /* espessura da c?lula situada acima (caso exista, se n?o existir ser? zero)*/
          NLoss, PLoss, /*vari?veis locais que s? ser?o usadas quando a classe for invocada a partir do EcoDyn*/
          MySettlingSpeed; /* velocidade de sedimenta??o em m/d*/

   int MyBoxNumber /*n? de ordem da c?lula*/,
       MyUpperBoxNumber /*n? de ordem da c?lula situada acima (caso exista, caso contr?rio preenche-se com -1)*/;

   MyBoxNumber = ABoxNumber;
   MyUpperBoxNumber = -1;

   MySettlingSpeed = SettlingSpeed / DAYSTOSECONDS; //m/s  //Convers?o da velocidade se~dimenta??o em m/s

   //No caso de a classe ser invocada a partir do EcoDyn...
#ifndef _PORT_FORTRAN_
   TEcoDynClass* MyTransportPointer = MyPEcoDynClass->GetTransportPointer();
   TEcoDynClass* MyNutrientPointer = MyPEcoDynClass->GetNutrientPointer();
   if (MyTransportPointer != NULL)
   	   MyTransportPointer->Inquiry(GetEcoDynClassName(), BoxDepth,
                                      MyBoxNumber,
                                      "Box depth",
                                      ObjectCode);
   else
       MyDepth = MyPEcoDynClass->GetBoxDepth(MyBoxNumber);
#else
   MyDepth = BoxDepth;
#endif
   NLoss = MySettlingSpeed * NPhyto[MyBoxNumber] / BoxDepth;
   PLoss = MySettlingSpeed * PPhyto[MyBoxNumber] / BoxDepth;
#ifndef _PORT_FORTRAN_
   if (
            (MyNutrientPointer != NULL) && (BottomBox)
      )
   {
         MyNutrientPointer->Update(GetEcoDynClassName(), NLoss * CUBIC / NITROGENATOMICWEIGHT,
												 MyBoxNumber,
												 "Ammonia",
												 ObjectCode);

         MyNutrientPointer->Update(GetEcoDynClassName(), PLoss * CUBIC /PHOSPHORUSATOMICWEIGHT,
												MyBoxNumber,
												"Phosphate",
												ObjectCode);      //Return as umol P / m3
   }
#endif
   //...

   PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] - MySettlingSpeed  * PhytoBiomass[MyBoxNumber] / MyDepth; //Perda de biomassa em unidades de carbono
   NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] - NLoss;   //Perda de biomassa em unidades de azoto
   PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] - PLoss;   //Perda de biomassa em unidades de f?sforo

#ifndef _PORT_FORTRAN_
   if (!SurfaceCell)  //Ganho de biomassa a partir da c?lula situada acima (caso exista)
   {
      //No caso de a classe ser invocada a partir do EcoDyn...
      MyTransportPointer->Inquiry(GetEcoDynClassName(), MyUpperDepth,
                                         MyUpperBoxNumber,
                                         "Top depth",
                                         ObjectCode);
      //...
      PhytoFlux[MyBoxNumber] = PhytoFlux[MyBoxNumber] + MySettlingSpeed * PhytoBiomass[MyUpperBoxNumber] / MyUpperDepth;
      NCellFlux[MyBoxNumber] = NCellFlux[MyBoxNumber] + MySettlingSpeed * NPhyto[MyUpperBoxNumber] / MyUpperDepth;
      PCellFlux[MyBoxNumber] = PCellFlux[MyBoxNumber] + MySettlingSpeed * PPhyto[MyUpperBoxNumber] / MyUpperDepth;
   }
#endif
}


 
double TPhytoplanktonGeneric::Platt1()
{
   int i = ABoxNumber;
   double LightLimitation, PARLight;
   double IntegrationSteps = 30;
   double DeltaZ, Soma;

   /*cout<<"i = "<<i<<endl;
   cout<<"PhytoBiomass = "<<PhytoBiomass[i]<< endl;
   cout<< "Pmax ="<< Pmax[i]<< endl;
   cout<< "Slope ="<< Slope[i]<< endl;
   cout<< "Beta ="<< beta[i]<< endl;
   cout<< "LightAtTop ="<< LightAtTop* WATTSTOMICROEINSTEINS<< endl;
   cout<< "KValue ="<< KValue<< endl;
   cout<< "BoxDepth ="<< BoxDepth<< endl;*/

   Productivity = 0.0;
   if (
       (BoxDepth > aMin) && (LightAtTop > aMin) && (IntegrationSteps >= 1.0))
   {  
         DeltaZ = BoxDepth / IntegrationSteps;
         Soma = 0.0;
         PARLight = LightAtTop * WATTSTOMICROEINSTEINS; 
         for (int Step = 1; Step <= IntegrationSteps; Step++)    //Euler integration as a function of depth^M
         {      
            Soma = Soma + (1 - exp(-Slope[i] * PARLight / Pmax[i])) * exp(-beta[i] * PARLight/ Pmax[i]) * DeltaZ;
            PARLight = PARLight * exp(-KValue * DeltaZ);
         }
         LightLimitation = Soma / BoxDepth;
	 Productivity = /*Pmax[i] **/ LightLimitation / HOURSTOSECONDS; //* PhytoBiomass[i]; //mg C m-3 s-1;    
   }
   return Productivity;
}





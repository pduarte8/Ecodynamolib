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

void phyto_getvalues__(float* phytobiomass)
{
   TPhytoplanktonGeneric* ptr;
   double Value;
   char MyParameter[65];
   ptr = TPhytoplanktonGeneric::getPhyto();

   strcpy(MyParameter, "Phytoplankton biomass\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *phytobiomass = Value;
}

void phyto_getbiomass__(float* phytobiomass)
{
   TPhytoplanktonGeneric* ptr;
   double Value;
   char MyParameter[65];
   ptr = TPhytoplanktonGeneric::getPhyto();

   strcpy(MyParameter, "Phytoplankton biomass\0");
   ptr->Inquiry(ptr->GetEcoDynClassName(), Value, 0, MyParameter, 0);
   *phytobiomass = Value;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "PHT: Phytoplankton biomass (): " << Value << endl;
}


void phyto_setdebug__(char* Debug)
{
   TPhytoplanktonGeneric* ptr;
   ptr = TPhytoplanktonGeneric::getPhyto();
   ptr->debug = *Debug;

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "PHT: Debug: " << ptr->debug << endl;
}

void phyto_setdepth__(int* box, float* depth)
{
   TPhytoplanktonGeneric* ptr;
   ptr = TPhytoplanktonGeneric::getPhyto();
   ptr->SetBoxDepth(*box, *depth);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "PHT: Depth: " << *depth << endl;
}

/*void phyto_setkvalue__(float* kvalue)
{
   TPhytoplanktonGeneric* ptr;
   ptr = TPhytoplanktonGeneric::getPhyto();
   ptr->SetKValue(*kvalue);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "PHT: K Value: " << ptr->GetKValue() << endl;


void phyto_setbiomass__(int* box, float* biomass)
{
   TPhytoplanktonGeneric* ptr;
   ptr = TPhytoplanktonGeneric::getPhyto();
   ptr->SetBiomass(*box, *biomass);

   if ((ptr->debug =='Y') || (ptr->debug == 'y'))
      cout << "PHT: Biomass: " << ptr->GetBiomass(*box) << endl;
}*/


void phyto_go__()
{
   TPhytoplanktonGeneric* ptr;
   ptr = TPhytoplanktonGeneric::getPhyto();
   ptr->Go();
   ptr->Integrate();
}

void phyto_setparams__(int* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope, double* aEiler, double* bEiler, double* cEiler, double* kValue,
                            double* Ks, double* phi, double* maintenanceRespiration, double* respirationCoefficient,double* docLoss, double* docStressLoss,
                            double* deathLoss, double* redfieldCFactor, double* redfieldNFactor,double* redfieldPFactor, double* temperatureAugmentationRate,
                            double* fallingSpeed, double* ratioLightDarkRespiration, double* nCellQuota, double* pCellQuota, double* minNPRatio,
                            double* maxNPRatio, double* pMaxUptake, double* nMaxUptake, double* kP,double* kNO3, double* kNH4, double* minPCellQuota, double* maxPCellQuota,
                            double* minNCellQuota, double* maxNCellQuota, double* kPInternal,double* kNInternal, double* settlingSpeed, double* carbonToOxygenProd,
                            double* carbonToOxygenResp, double* tminRespiration,double* tminPhotosynthesis, double* layerThickness, double* lightAtTop, double* lightAtBottom,
                            double* waterTemperature)
{
    TPhytoplanktonGeneric* ptr;
    ptr = TPhytoplanktonGeneric::getPhyto();
    ptr->SetPhytoParams(*pmax, *iopt, *slope, *aEiler, *bEiler, *cEiler,
            *kValue, *Ks, *kNO3, *kNH4, *phi, *maintenanceRespiration,
            *respirationCoefficient, *docLoss, *docStressLoss, *deathLoss,
            *redfieldCFactor, *redfieldNFactor, *redfieldPFactor,
            *temperatureAugmentationRate, *fallingSpeed,
            *ratioLightDarkRespiration);
    ptr->SetCLPhytoParams(*nCellQuota, *pCellQuota, *minNPRatio,
            *maxNPRatio, *pMaxUptake, *nMaxUptake, *kP,
            *kNO3, *kNH4, *minPCellQuota, *maxPCellQuota,
            *minNCellQuota, *maxNCellQuota, *kPInternal,
            *kNInternal, *settlingSpeed, *carbonToOxygenProd,
            *carbonToOxygenResp, *tminRespiration, *tminPhotosynthesis);
}

//-----------------------------------------------
// ...Constructors adapted to Fortran...
//
//        only one box
//
//-----------------------------------------------

//TPhytoplanktonGeneric* TPhytoplanktonGeneric::PPhytoplankton = 0;

/*
 * This constructor calls Go() and Integrate() after initialisation
 *
 * 'box' used only for debug AP 050119 - AP 050119
 */
void phytoplankton_(long* PPhytoplankton,int* box, float* depth, float* biomass,
        float* lightAtTop, float* lightAtBottom, float* waterTemperature,
        float* timeStep)
{
   TPhytoplanktonGeneric* ptr;
   ptr = TPhytoplanktonGeneric::getPhyto();
   *PPhytoplankton = (int)ptr;
   ptr->SetBoxDepth(*box, *depth);
   ptr->SetVariableValue("Fortran", *biomass,*box,"Phytoplankton initial biomass");
   ptr->SetLightAtTop(*lightAtTop);
   ptr->SetLightAtBottom(*lightAtBottom);
   ptr->SetWaterTemperature(*waterTemperature);
   ptr->SetTimeStep(*timeStep);
}

void phytoplankton_new__(long* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope, double* aEiler, double* bEiler, double* cEiler, 
                            double* maintenanceRespiration, double* respirationCoefficient,double* docStressLoss,
                            double* deathLoss, double* redfieldCFactor, double* redfieldNFactor,double* redfieldPFactor, double* temperatureAugmentationRate,
                            double* ratioLightDarkRespiration, double* minNPRatio,double* maxNPRatio, double* pMaxUptake, double* nMaxUptake, double* kP,double* kNO3, 
                            double* kNH4, double* minPCellQuota, double* maxPCellQuota,double* minNCellQuota, double* maxNCellQuota, double* kPInternal,double* kNInternal, 
                            double* settlingSpeed, double* carbonToOxygenProd,double* carbonToOxygenResp, double* tminRespiration,double* tminPhotosynthesis, 
                            int* nitrogenLimitation, int* phosphorusLimitation)
{
        TPhytoplanktonGeneric* ptr;
        //***Grid dimensions set to "ones" because this is for usage of EcoDynamo functions from ROMS (or other software) and the functions remain agnostic about grid details***/
        ptr = TPhytoplanktonGeneric::getPhyto();
        *PPhytoplankton = (long)ptr;
        ptr->SetNumberOfLines(1);
        ptr->SetNumberOfColumns(1);
        ptr->SetNumberOfLayers(1);
        ptr->SetNumberOfBoxes(1); 
                /**********************************************Parameter declaration and zero initialization*****************************************************************************/
        ptr->BuildProdutor("TPhytoplanktonGeneric"); 
        /**********************************************Variable declaration and zero initialization******************************************************************************/
        ptr->PreBuildPhytoplankton();
        /**********************************************Parameters values imported from Fortran program***************************************************************************/
        ptr->SetParameterValue("Pmax", *pmax); 
        ptr->SetParameterValue("Iopt", *iopt);
        ptr->SetParameterValue("Slope", *slope);
        ptr->SetParameterValue("DefaultAEiler", *aEiler);
        ptr->SetParameterValue("BEiler", *bEiler);
        ptr->SetParameterValue("CEiler", *cEiler);
        ptr->SetParameterValue("Maintenance respiration", *maintenanceRespiration);
        ptr->SetParameterValue("Respiration Coefficient",*respirationCoefficient);
        ptr->SetParameterValue("Dissolved organic carbon nutrient stress loss",*docStressLoss);
        ptr->SetParameterValue("Death loss",*deathLoss);
        ptr->SetParameterValue("RedfieldCFactor",*redfieldCFactor);
        ptr->SetParameterValue("RedfieldNFactor", *redfieldNFactor);
        ptr->SetParameterValue("RedfieldPFactor", *redfieldPFactor);  
        //cout << "Maintenance respiration= "<< *maintenanceRespiration; cout << "Respiration Coefficient= "<< *respirationCoefficient; 
        ptr->SetParameterValue("TemperatureAugmentationRate",*temperatureAugmentationRate);
        ptr->SetParameterValue("SettlingSpeed",*settlingSpeed);   
        ptr->SetParameterValue("Light/Dark resp", *ratioLightDarkRespiration);
        ptr->SetParameterValue("MinNPRatio", *minNPRatio);
        ptr->SetParameterValue("MaxNPRatio", *maxNPRatio); 
        ptr->SetParameterValue("PMaxUptake",*pMaxUptake);
        ptr->SetParameterValue("NMaxUptake",*nMaxUptake);
        ptr->SetParameterValue("KP",*kP);
        ptr->SetParameterValue("KNO3",*kNO3);
        ptr->SetParameterValue("KNH4",*kNH4);
        ptr->SetParameterValue("MaxNCellQuota",*maxNCellQuota);
        ptr->SetParameterValue("MinNCellQuota",*minNCellQuota);
        ptr->SetParameterValue("MaxPCellQuota",*maxPCellQuota);
        ptr->SetParameterValue("MinPCellQuota",*minPCellQuota);
        ptr->SetParameterValue("KPInternal",*kPInternal);
        ptr->SetParameterValue("KNInternal",*kNInternal);
        ptr->SetParameterValue("Carbon to Oxygen in photosynthesis",*carbonToOxygenProd);   
        ptr->SetParameterValue("Carbon to Oxygen in respiration",*carbonToOxygenResp);
        ptr->SetParameterValue("TminPhotosynthesis",*tminPhotosynthesis);
        ptr->SetParameterValue("TminRespiration",*tminRespiration);
        ptr->SetParameterValue("Nitrogen limitation",*nitrogenLimitation);
        ptr->SetParameterValue("Phosphorus limitation",*phosphorusLimitation);
        ptr->ExudatedFlux = 0.0;
        ptr->MyDay = 0;
        ptr->aMin = 0.00000000001;
}


void phytoplankton_go__(long* PPhytoplankton, double* layerThickness, double* timeStep)
{
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   /********Time step is reseted every "go". This is not necessary for ROMS but it may become necessary for othervariable time step application******************/
   ptr->SetTimeStep(*timeStep);
   /*********************SetABoxDepth resets layer thickness. This is mostly necessary for surface layers*8******************************************************/
   ptr->SetABoxDepth(*layerThickness);
   /********************************************Resetes box number to zero....just in case....*******************************************************************/
   ptr->SetABoxNumber(0);
   //cout<< "Pmax= "<< ptr->Pmax[0] << endl;
}



void phytoplankton_production__(long* PPhytoplankton, double* lightAtTop, double* lightAtBottom, double* kValue,double* waterTemperature,
                                    int* piCurveOption, double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* biomass, double *TIC, double *ASlope, double* Chl2Carbon, double *OxygenProduction)
{
   double Productivity, MyBiomass, MyNPhyto, MyPPhyto, MyNCellQuota, MyPCellQuota, MyChl2Carbon, FromChl2Carbon, CarbonOxygenRatio;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   /**********************PAR light intensity reseting at the the topo and the bottom of a model cell**************************************************************/
   ptr->SetLightAtTop(*lightAtTop);
   ptr->SetLightAtBottom(*lightAtBottom);
   /********************************************Reseting of the light extinction coefficient for proper primary production vertical integration********************/
   ptr->SetParameterValue("KValue", *kValue);
   /***********Reseting of the slope of the P-I curve, just in case this is to be recalculated as a function of chlorophyll:carbon or something else...************/
   ptr->SetParameterValue("Slope", *ASlope);
   /********************************************Reseting of water temperature to account for corresponding limitation of photosynthesis****************************/
   ptr->SetWaterTemperature(*waterTemperature);
   /********************************************Conversions from mmol/m3 (ROMS units) to mg / m3 (EcoDynamo units)*************************************************/                            
   MyBiomass = *biomass * CARBONATOMICWEIGHT; 
   MyNPhyto =  *nPhyto * NITROGENATOMICWEIGHT;
   MyPPhyto = *pPhyto * PHOSPHORUSATOMICWEIGHT;
   /*********************************************Reseting of the chlorophyll / carbon ratio************************************************************************/
   MyChl2Carbon = *Chl2Carbon;
   FromChl2Carbon = 1.0 / MyChl2Carbon; 
   /*******************************************Calculation of cell quotas for usage in the calculation of internal nutrient limitation*****************************/ 
   if (MyBiomass > ptr->aMin)
   {
       MyNCellQuota = MyNPhyto / MyBiomass;
       MyPCellQuota = MyPPhyto / MyBiomass; 
   }
   else
   {
       MyNCellQuota = 0.0;
       MyPCellQuota = 0.0;
   }
   /******************************************Reseting of variable values in the phytoplankton pointer in the units of EcoDynamo************************************/
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", FromChl2Carbon,0,"Chlorophyll to Carbon");
   ptr->SetVariableValue("Fortran", MyNCellQuota,0,"NCellQuota");
   ptr->SetVariableValue("Fortran", MyPCellQuota,0,"PCellQuota");
   ptr->SetVariableValue("Fortran", MyNPhyto,0,"NPhyto");
   ptr->SetVariableValue("Fortran", MyPPhyto,0,"PPhyto");
   /*****************************************Selection of a P-I function depending on the option "piCurveOption" in the arguments above*****************************/
   switch (*piCurveOption)
   {
      case 1: /*STEELE*/ // add a list item
         ptr->Productivity = ptr->SteeleProduction();
         ptr->Slope[0] = ptr->SteeleSlope();
         break;
      case 2: /*MICHAELIS_MENTEN*/	// add a list item
         ptr->Productivity = ptr->Michaelis_MentenProduction();
         ptr->Slope[0] = ptr->MichaelisMentenSlope();
         break;
      case 3: /*EILER*/	// add a list item
         ptr->Productivity = ptr->EilerProduction();
         ptr->Slope[0] = ptr->EilersAndPeetersSlope();
         break;
   }
   /***************************************Light limited productivity calculated in mg C / m3**********************************************************************/
   Productivity = ptr->GetParameterValue("Productivity"); 
   /***************************************Calculation of temperature limitation***********************************************************************************/
   ptr->Tmin = ptr->GetParameterValue("TminPhotosynthesis");
   Productivity = ptr->GetParameterValue("Productivity") * ptr->TemperatureArrheniusExponentialLimitation(0);
   ptr->SetParameterValue("Productivity", Productivity);
   //cout<< "Productivity temp limited = "<< Productivity << endl;
   /***************************************Calculation of nutrient limitation**************************************************************************************/
   ptr->NutrientLimitation(0);
   //Productivity = ptr->GetParameterValue("Productivity");
   //cout<< "Productivity nut limited = "<< Productivity << endl;
   /***************************************Productivity recalculated in mmol C/m3/s*******************************************************************************/
   *GrossProduction = ptr->GetParameterValue("Productivity") / CARBONATOMICWEIGHT; //Return value in mmolC/m3/s for compatibility with ROMS
   *ASlope = ptr->GetParameterValue("Slope"); //Return value in [s-1/(micro mol photons m-2 s-1)]
   /**************************************This was to calculate average daily production which is not being calculated now****************************************/
    ptr->SetJulianDay(*julianDay);
   //ptr->DailyAverageProduction();
   /*************************************Productivity expressed in oxygen units to feedback oxygen biogeochemical cycling****************************************/ 
   CarbonOxygenRatio = ptr->GetParameterValue("Carbon to Oxygen in photosynthesis");
   *OxygenProduction = ptr->GetParameterValue("Productivity") / CarbonOxygenRatio / (2.0 * OXYGENATOMICWEIGHT); //OxygenProduction in mmol O2 m-3 s-1 
}

void phytoplankton_respiration__(long* PPhytoplankton, double* waterTemperature, double* cffCRespiration, double* GrossProduction, double *biomass, double *Oxygen, double* Chl2Carbon, double *OxygenConsumption)
{
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   double MyBiomass, MyChl2Carbon, FromChl2Carbon, CarbonOxygenRatio;
   ptr->SetWaterTemperature(*waterTemperature);
   MyBiomass = *biomass * CARBONATOMICWEIGHT; //Conversions from mmol/m3 to mg / m3
   MyChl2Carbon = *Chl2Carbon;
   FromChl2Carbon = 1.0 / MyChl2Carbon;
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", FromChl2Carbon,0,"Chlorophyll to Carbon");
   ptr->GPP[0] = *GrossProduction * CARBONATOMICWEIGHT;
   if (ptr->PhytoBiomass[0] > ptr->aMin)
   {
      ptr->Tmin = ptr->GetParameterValue("TminRespiration");
      //ptr->SetParameterValue("Tmin", tmin);
      ptr->Respiration(0);
      *cffCRespiration = ptr->RespiredFlux[0] / CARBONATOMICWEIGHT/*/ ptr->PhytoBiomass[0]*/;  //Return value in mmol C m-3 s-1 for compatibility with ROMS nonlinear backward-implicit solution
      CarbonOxygenRatio = ptr->GetParameterValue("Carbon to Oxygen in respiration");
      *OxygenConsumption = ptr->RespiredFlux[0] / CarbonOxygenRatio / (2.0 * OXYGENATOMICWEIGHT) /*/ *Oxygen*/; //Return value in mmol O2 m-3 s-1 for compatibility with ROMS nonlinear backward-implicit solution
   }
   else
   {
      *cffCRespiration = 0.0;
      *OxygenConsumption = 0.0;
   }
}

void phytoplankton_exudation__(long* PPhytoplankton, double* cffCExudation, double* GrossProduction, double* biomass, double *NCellQuota, double *PCellQuota)
{
   double Exudation, MyBiomass;
   //cout << "Exudation start" << endl;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   MyBiomass = *biomass * CARBONATOMICWEIGHT; //Conversions from mmol/m3 to mg / m3
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", *NCellQuota,0,"NCellQuota");
   ptr->SetVariableValue("Fortran", *PCellQuota,0,"PCellQuota");
   ptr->GPP[0] = *GrossProduction * CARBONATOMICWEIGHT;
   
   if ((ptr->PhytoBiomass[0] > ptr->aMin) && (ptr->GPP[0] > ptr->aMin) && ((ptr->NCellQuota[0] < ptr->RedfieldNFactor) || (ptr->PCellQuota[0] < ptr->RedfieldPFactor)))
   {
      ptr->Exudation(0);   
      *cffCExudation = ptr->ExudatedFlux / CARBONATOMICWEIGHT/*/ ptr->PhytoBiomass[0]*/; //Return value in mmol C m-3 s-1 for compatibility with ROMS nonlinear backward-implicit solution
   }
   else
      *cffCExudation = 0.0;
}

void phytoplankton_nitrogen_uptake__(long* PPhytoplankton, double* Ammonia, double* Nitrate, double* Nitrite,double* cffNH4, double *cffNO3NO2, double* nPhyto, double* biomass)
{
   double MyBiomass, MyNPhyto, MyNCellQuota;
   //cout << "Nitrogen uptake start" << endl;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   ptr->AmmoniaUpTake = 0.0; 
   ptr->NitrateAndNitriteUptake = 0.0;
   MyBiomass = *biomass * CARBONATOMICWEIGHT; //Conversions from mmol/m3 to mg / m3
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   MyNPhyto =  *nPhyto * NITROGENATOMICWEIGHT;
   if (MyBiomass > ptr->aMin) MyNCellQuota = MyNPhyto / MyBiomass;
   else MyNCellQuota = 0.0;
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", MyNCellQuota,0,"NCellQuota");
   ptr->SetVariableValue("Fortran", MyNPhyto,0,"NPhyto");
   
   if (ptr->GetParameterValue("Nitrogen limitation") == 1)
      ptr->NitrogenUptake(0,*Ammonia, *Nitrate, *Nitrite);
   //cout<<"*Ammonia = " << *Ammonia << endl; 
   //cout<<"*Nitrate = " << *Nitrate << endl; 
   //cout<<"*AmmoniaUpTake = " << ptr->AmmoniaUpTake/NITROGENATOMICWEIGHT/ HOURSTOSECONDS / *Ammonia << endl;
   //cout<<"*NitrateAndNitriteUptake = " << ptr->NitrateAndNitriteUptake/NITROGENATOMICWEIGHT/ HOURSTOSECONDS / (*Nitrate + *Nitrite) << endl;
   if (*Ammonia > ptr->aMin)
      *cffNH4 = ptr->AmmoniaUpTake/NITROGENATOMICWEIGHT/ HOURSTOSECONDS /*/ *Ammonia*/; 
   else
      *cffNH4 = 0.0;
   if (*Nitrate + *Nitrite > ptr->aMin) 
      *cffNO3NO2 = ptr->NitrateAndNitriteUptake /NITROGENATOMICWEIGHT/ HOURSTOSECONDS /*/ (*Nitrate + *Nitrite)*/; 
   else
      *cffNO3NO2 = 0.0;
} 

void phytoplankton_phosphorus_uptake__(long* PPhytoplankton, double* Phosphate,double* cffPO4, double *pPhyto, double* biomass)
{
   double MyBiomass, MyPPhyto, MyPCellQuota;
   //cout << "Phosphorus uptake start" << endl;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;

   MyBiomass = *biomass * CARBONATOMICWEIGHT; //Conversions from mmol/m3 to mg / m3
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   MyPPhyto = *pPhyto * PHOSPHORUSATOMICWEIGHT;
   if (MyBiomass > ptr->aMin)MyPCellQuota = MyPPhyto / MyBiomass; 
   else MyPCellQuota = 0.0;
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", MyPCellQuota,0,"PCellQuota");
   ptr->SetVariableValue("Fortran", MyPPhyto,0,"PPhyto");

   if (ptr->GetParameterValue("Phosphorus limitation") == 1)
      ptr->PhosphorusUptake(0, *Phosphate);
   if (*Phosphate > ptr->aMin)
      *cffPO4 = ptr->PUptake[0] / PHOSPHORUSATOMICWEIGHT / HOURSTOSECONDS /*/ *Phosphate*/;
   else
      *cffPO4 = 0.0;
   //cout << "Phosphorus uptake end" << endl;
}

void phytoplankton_mortality__(long* PPhytoplankton, double* nCellQuota, double* pCellQuota,
                               double* waterTemperature, double* biomass, double* timeStep, double* cff)
{
   double Mortality;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   ptr->Mortality(0);
}


void phytoplankton_integrate__(long* PPhytoplankton,double* nCellQuota, double* pCellQuota,double* biomass)
{
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   ptr->Integrate();
}

void phytoplankton_rhochl_(long* PPhytoplankton, double* light, double* kValue, double* GrossProduction, double* slope, double* rhochl)
{

}

void TPhytoplanktonGeneric::PreBuildPhytoplanktonGeneric(char* className)
{
   strcpy(EcoDynClassName, className);
   NumberOfLines = 1;
   NumberOfColumns = 1;
   NumberOfBoxes = 1; // Size of array - i.e. number of model boxes
   if (NumberOfBoxes > 0)                       // Initialise arrays for variables
   {
	NCellQuota  = new double[NumberOfBoxes];
	PCellQuota  = new double[NumberOfBoxes];
        
	// Fluxes and conversions
	for (int i = 0; i < NumberOfBoxes; i++)
	{
	   NCellQuota[i]  = 0.0;
	   PCellQuota[i]  = 0.0;
	}
   }
}

/*
 * singleton provider - TPhytoplanktonGeneric class method
 */
TPhytoplanktonGeneric* TPhytoplanktonGeneric::getPhyto(TPhytoplanktonGeneric* pphytoplankton)
{
   /*if (PPhytoplankton == 0) {
      PPhytoplankton = new TPhytoplanktonGeneric("TPhytoplanktonGeneric");
      PPhytoplankton->debug = 'N';
   }*/
   TPhytoplanktonGeneric *PPhytoplankton = new TPhytoplanktonGeneric("TPhytoplanktonGeneric");
   return PPhytoplankton;
}

TPhytoplanktonGeneric* TPhytoplanktonGeneric::getPhyto()
{
   //if (PPhytoplankton == 0) {
      //PPhytoplankton = new TPhytoplanktonGeneric("TPhytoplanktonGeneric");
      //PPhytoplankton->debug = 'N';
   //}
   TPhytoplanktonGeneric *PPhytoplankton = new TPhytoplanktonGeneric("TPhytoplanktonGeneric");
   return PPhytoplankton;
}


TPhytoplanktonGeneric::TPhytoplanktonGeneric(char* className)
        :TCrestumaLeverPhytoplankton2DVIntLim(className)
{
    MyPEcoDynClass = (TEcoDynClass*)this;
    NumberOfVariables = 18;
    VariableNameArray = new VNA[NumberOfVariables];

    strcpy(VariableNameArray[0], "Phytoplankton biomass");
    strcpy(VariableNameArray[1], "Phytoplankton C biomass");
    strcpy(VariableNameArray[2], "Phytoplankton N biomass");
    strcpy(VariableNameArray[3], "Phytoplankton P biomass");
    strcpy(VariableNameArray[4], "NCellQuota");
    strcpy(VariableNameArray[5], "PCellQuota");
    strcpy(VariableNameArray[6], "NUptake");
    strcpy(VariableNameArray[7], "PUptake");
    strcpy(VariableNameArray[8], "Phytop. total GPP");
    strcpy(VariableNameArray[9], "Phytop. total NPP");
    strcpy(VariableNameArray[10], "Phytop. GPP per unit mass");
    strcpy(VariableNameArray[11], "Phytop. NPP per unit mass");
    strcpy(VariableNameArray[12], "Chlorophyll : Carbon");
    strcpy(VariableNameArray[13], "Chlorophyll to biomass");
    strcpy(VariableNameArray[14], "Phyto NPP/GPP ratio");
    strcpy(VariableNameArray[15], "Phytoplankton mortality");
    strcpy(VariableNameArray[16], "River temperature");
    strcpy(VariableNameArray[17], "Chlorophyll to Carbon");

    EquationName = STEELE;
    Tmin = 0.0;
}
//-----------------------------------------------

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
   if ((PhytoBiomass[MyBoxNumber] > aMin) &&
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
   if ((PhytoBiomass[MyBoxNumber] > aMin) &&
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
   if ((PhytoBiomass[i] > aMin) &&
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
      return 1.0 / CEiler[MyBoxNumber]; //[s-1/(micro mol photons m-2 s-1)]
   else
      return Slope[MyBoxNumber];
}

double TPhytoplanktonGeneric::MichaelisMentenSlope()
{
   int MyBoxNumber = ABoxNumber;
   if (Imax[MyBoxNumber] > aMin)
      return Pmax[MyBoxNumber] / Imax[MyBoxNumber]; //[s-1/(micro mol photons m-2 s-1)] Note that Imax is the light intensity corresponding to Pmax/2 
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
      if (GPP[MyBoxNumber] <= 0.0)
      {
         OxygenProd = GPP[MyBoxNumber] / CarbonToOxygenProd / (PhytoBiomass[MyBoxNumber] / ChlToCarbon[MyBoxNumber]);
         Resp = Resp + RatioLightDarkRespiration * RespirationCoefficient * OxygenProd / OxygenMolecularWeight
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


 





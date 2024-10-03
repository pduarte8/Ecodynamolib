// PhytoplanktonGeneric object CPP code file

/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
// TPhytoplanktonGeneric Class
//
//-----------------------------------------------
// ...Methods invoked from Fortran...
//
//        only one box
//
//-----------------------------------------------

#include "ecodyn.rh"
#include "phytobjt.h"
#include "../BGCLibraries/PIFunctions.cpp"
#include "../BGCLibraries/TemperatureLimitation.cpp"

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

void phyto_setparams__(int* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope, double* beta, double* aEiler, double* bEiler, double* cEiler, double* kValue,
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
    ptr->SetPhytoParams(*pmax, *iopt, *slope, *beta, *aEiler, *bEiler, *cEiler,
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

void phytoplankton_new__(long* PPhytoplankton, double* pmax, double* iopt, double* imax, double* slope,double* beta, double* aEiler, double* bEiler, double* cEiler, 
                            double* maintenanceRespiration, double* respirationCoefficient,double* docStressLoss,
                            double* deathLoss, double* redfieldCFactor, double* redfieldNFactor,double* redfieldPFactor, double* temperatureAugmentationRate,
                            double* ratioLightDarkRespiration, double* minNPRatio,double* maxNPRatio, double* pMaxUptake, double* nMaxUptake, double* kP,double* kNO3, 
                            double* kNH4, double* minPCellQuota, double* maxPCellQuota,double* minNCellQuota, double* maxNCellQuota, double* kPInternal,double* kNInternal, double* settlingSpeed, double* carbonToOxygenProd,double* carbonToOxygenResp, double* tminRespiration,double* tminPhotosynthesis,
                            int* nitrogenLimitation, int* phosphorusLimitation, int* silicaLimitation, double* maxSiCellQuota, double* minSiCellQuota,
                            double* minNSiRatio, double* siMaxUptake,
                            double* kSi, double* kSiInternal, double* redfieldSi, int* pifunction, int* nutrientLimitationType)
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
        ptr->SetParameterValue("Slope", *slope / WATTSTOMICROEINSTEINS);/* Converting from h-1 W-1 m2 s to micro Einsteins-1 m2 s */
        ptr->SetParameterValue("Beta", *beta / WATTSTOMICROEINSTEINS); /* Converting from h-1 W-1 m2 s to h-1 micro Einsteins-1 m2 s */
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
        //cout << "Maintenance respiration= "<< *maintenanceRespiration << endl;
        //cout << "Respiration Coefficient= "<< *respirationCoefficient << endl;
        //cout << "CARBONATOMICWEIGHT= "<<CARBONATOMICWEIGHT<< endl; 
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
        //cout << "carbonToOxygenProd= "<< *carbonToOxygenProd << endl;
        //cout << "carbonToOxygenResp= "<< *carbonToOxygenResp << endl;
        ptr->SetParameterValue("TminPhotosynthesis",*tminPhotosynthesis);
        ptr->SetParameterValue("TminRespiration",*tminRespiration);

        ptr->SetParameterValue("MaxSiCellQuota",*maxSiCellQuota);
        ptr->SetParameterValue("MinSiCellQuota",*minSiCellQuota);
        ptr->SetParameterValue("MinNSiRatio",*minNSiRatio);
        ptr->SetParameterValue("SiMaxUptake",*siMaxUptake);
        ptr->SetParameterValue("KSi",*kSi);
        ptr->SetParameterValue("KSiInternal",*kSiInternal);
        ptr->SetParameterValue("RedfieldSiFactor",*redfieldSi);

        ptr->SetIntParameterValue("Nitrogen limitation",*nitrogenLimitation);
        ptr->SetIntParameterValue("Phosphorus limitation",*phosphorusLimitation);
        ptr->SetIntParameterValue("Silica limitation",*silicaLimitation);

        ptr->SetIntParameterValue("PIFunction", *pifunction);
        ptr->SetIntParameterValue("NutrientLimitationType", *nutrientLimitationType);
        
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
                                double* julianDay, double* GrossProduction, double* nPhyto, double* pPhyto, double* SiPhyto, double* biomass, double *ASlope, double* Chl2Carbon, double *OxygenProduction, int *line, int *column, int *layer)
{
   double Productivity, MyBiomass, MyNPhyto, MyPPhyto, MySiPhyto, MyNCellQuota, MyPCellQuota, MySiCellQuota, MyChl2Carbon, FromChl2Carbon, CarbonOxygenRatio;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   /**********************PAR light intensity reseting at the the topo and the bottom of a model cell**************************************************************/
   ptr->SetLightAtTop(*lightAtTop);
   ptr->SetLightAtBottom(*lightAtBottom);
   /********************************************Reseting of the light extinction coefficient for proper primary production vertical integration********************/
   ptr->SetParameterValue("KValue", *kValue);
   /***********Reseting of the slope of the P-I curve, just in case this is to be recalculated as a function of chlorophyll:carbon or something else...************/
   ptr->SetParameterValue("Slope", *ASlope / WATTSTOMICROEINSTEINS);
   /********************************************Reseting of water temperature to account for corresponding limitation of photosynthesis****************************/
   ptr->SetWaterTemperature(*waterTemperature);
   /********************************************Conversions from mmol/m3 (ROMS units) to mg / m3 (EcoDynamo units)*************************************************/
   ptr->SetIntParameterValue("Line", *line);
   ptr->SetIntParameterValue("Column", *column);
   ptr->SetIntParameterValue("Layer", *layer);
                            
   MyBiomass = *biomass * CARBONATOMICWEIGHT; 
   MyNPhyto =  *nPhyto * NITROGENATOMICWEIGHT;
   MyPPhyto = *pPhyto * PHOSPHORUSATOMICWEIGHT;
   MySiPhyto = *SiPhyto * SILICAATOMICWEIGHT;
   /*********************************************Reseting of the chlorophyll / carbon ratio************************************************************************/
   MyChl2Carbon = *Chl2Carbon;
   FromChl2Carbon = 1.0 / MyChl2Carbon; 
   /*******************************************Calculation of cell quotas for usage in the calculation of internal nutrient limitation*****************************/ 
   if (MyBiomass > ptr->aMin)
   {
       MyNCellQuota = MyNPhyto / MyBiomass;
       MyPCellQuota = MyPPhyto / MyBiomass;
       MySiCellQuota = MySiPhyto / MyBiomass; 
   }
   else
   {
       MyNCellQuota = 0.0;
       MyPCellQuota = 0.0;
       MySiCellQuota = 0.0;
   }
   /******************************************Reseting of variable values in the phytoplankton pointer in the units of EcoDynamo************************************/
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", FromChl2Carbon,0,"Chlorophyll to Carbon");
   ptr->SetVariableValue("Fortran", MyNCellQuota,0,"NCellQuota");
   ptr->SetVariableValue("Fortran", MyPCellQuota,0,"PCellQuota");
   ptr->SetVariableValue("Fortran", MySiCellQuota,0,"SiCellQuota");
   ptr->SetVariableValue("Fortran", MyNPhyto,0,"NPhyto");
   ptr->SetVariableValue("Fortran", MyPPhyto,0,"PPhyto");
   /*****************************************Selection of a P-I function depending on the option "piCurveOption" in the arguments above*****************************/
   //cout<< "PIFunction = "<< ptr->GetIntParameterValue("PIFunction")<< endl;
   //switch (*piCurveOption)
   ptr->LightAtTop = ptr->LightAtTop * WATTSTOMICROEINSTEINS;
   switch (ptr->GetIntParameterValue("PIFunction"))
   {
      case 1: /*STEELE*/ // add a list item
         ptr->LightAtBottom = ptr->LightAtBottom * WATTSTOMICROEINSTEINS;
         ptr->Productivity = ptr->Pmax[0]*ptr->PhytoBiomass[0]*
                             Steele1(ptr->LightAtTop, ptr->KValue, ptr->BoxDepth, ptr->Iopt[0])/
                             HOURSTOSECONDS;
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
      case 4: /*Platt*/	// add a list item
         //Pmax = ptr->GetParameterValue("Pmax"); 
	 //beta = ptr->GetParameterValue("Beta");
         ptr->Productivity = ptr->Pmax[0]*ptr->PhytoBiomass[0]*
                        Platt1(ptr->LightAtTop, ptr->KValue, ptr->BoxDepth, ptr->Pmax[0], ptr->beta[0], ptr->Slope[0], 30)/
                        HOURSTOSECONDS;
         break;
   }
   /***************************************Light limited productivity calculated in mg C / m3**********************************************************************/
   Productivity = ptr->GetParameterValue("Productivity");
   /*if (Productivity < 0.0) {
      cout<< "Productivity light limited = "<< Productivity << endl;
   }*/
   /***************************************Calculation of temperature limitation***********************************************************************************/
   ptr->Tmin = ptr->GetParameterValue("TminPhotosynthesis");
   Productivity = ptr->GetParameterValue("Productivity") * TemperatureExponentialLimitation(ptr->WaterTemperature, ptr->Tmin, ptr->TemperatureAugmentationRate);
   /*if (Productivity < 0.0) {
      cout<< "Productivity temp limited = "<< Productivity << endl;
   }*/

   ptr->SetParameterValue("Productivity", Productivity);
   //cout<< "Productivity temp limited = "<< Productivity << endl;
   /***************************************Calculation of nutrient limitation**************************************************************************************/
   //cout<< "Nitrogen limitation ="<< ptr->GetIntParameterValue("Nitrogen limitation") << endl;
   //cout<< "Phosphorus limitation ="<< ptr->GetIntParameterValue("Phosphorus limitation") << endl;
   //cout<< "Silica limitation ="<< ptr->GetIntParameterValue("Silica limitation") << endl;
   
   if (ptr->NutrientLimitationType == 2) 	
      ptr->NutrientLimitation(0);
   
   Productivity = ptr->GetParameterValue("Productivity");
   /*if (Productivity < 0.0) {
      cout<< "Productivity nut limited = "<< Productivity << endl;
   }*/
   /***************************************Productivity recalculated in mmol C/m3/s*******************************************************************************/
   *GrossProduction = ptr->GetParameterValue("Productivity") / CARBONATOMICWEIGHT; //Return value in mmolC/m3/s for compatibility with ROMS
   //*ASlope = ptr->GetParameterValue("Slope"); // /WATTSTOMICROEINSTEINS; //Return value in [h-1/(W  m-2 s-1)]
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
   double Exudation, MyBiomass, NMassQuota, PMassQuota;
   //cout << "Exudation start" << endl;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
   MyBiomass = *biomass * CARBONATOMICWEIGHT; //Conversions from mmol/m3 to mg / m3
   NMassQuota = *NCellQuota * NITROGENATOMICWEIGHT / CARBONATOMICWEIGHT; //mg N / mg C
   PMassQuota = *PCellQuota * PHOSPHORUSATOMICWEIGHT / CARBONATOMICWEIGHT; //mg P / mg C
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   ptr->SetVariableValue("Fortran", NMassQuota,0,"NCellQuota");
   ptr->SetVariableValue("Fortran", PMassQuota,0,"PCellQuota");
   ptr->GPP[0] = *GrossProduction * CARBONATOMICWEIGHT;
   
   if ((ptr->PhytoBiomass[0] > ptr->aMin) && (ptr->GPP[0] > ptr->aMin) && ((NMassQuota < ptr->RedfieldNFactor/ptr->RedfieldCFactor) || (PMassQuota < ptr->RedfieldPFactor/ptr->RedfieldCFactor)))
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
   
   if (ptr->GetIntParameterValue("Nitrogen limitation") == 1)
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
   ptr->SetVariableValue("Fortran", MyPCellQuota,0,"PCellQuota");
   ptr->SetVariableValue("Fortran", MyPPhyto,0,"PPhyto");

   if (ptr->GetIntParameterValue("Phosphorus limitation") == 1)
      ptr->PhosphorusUptake(0, *Phosphate);
   if (*Phosphate > ptr->aMin)
      *cffPO4 = ptr->PUptake[0] / PHOSPHORUSATOMICWEIGHT / HOURSTOSECONDS /*/ *Phosphate*/;
   else
      *cffPO4 = 0.0;
   //cout << "Phosphorus uptake end" << endl;
}

void phytoplankton_external_nut_limitation__(long* PPhytoplankton, double* Ammonia, double* Nitrate,double* Nitrite, double* Phosphate, double* Silicate, double* Limitation)
{
    TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;
    *Limitation = 1.0;
    if (ptr->GetIntParameterValue("Nitrogen limitation") == 1){
       double MyKNH4, MyKNO3,inhNH4, cff1, cff2, L_NH4, L_NO3;
       MyKNH4 = ptr->GetParameterValue("KNH4");
       MyKNO3 = ptr->GetParameterValue("KNO3");
       cff1= *Ammonia / MyKNH4;
       cff2= (*Nitrate + *Nitrite) / MyKNO3;
       inhNH4=1.0/(1.0+cff1);
       L_NH4=cff1/(1.0+cff1);
       //L_NO3=cff2*inhNH4/(1.0+cff2); Laura and Pedro decided to comment this for compatibility with CICE and for the sake of simplicity
       L_NO3=cff2/(1.0+cff2); 
       *Limitation = MIN(L_NO3+L_NH4, *Limitation);  
    }
    if (ptr->GetIntParameterValue("Phosphorus limitation") == 1){
       double MyKP, L_PO4;
       MyKP = ptr->GetParameterValue("KP");
       L_PO4 = *Phosphate / (*Phosphate + MyKP);
       *Limitation = MIN(L_PO4, *Limitation);  
    }
    if (ptr->GetIntParameterValue("Silica limitation") == 1){
       double MyKSi, L_Si;
       MyKSi = ptr->GetParameterValue("KSi");
       L_Si = *Silicate / (*Silicate + MyKSi);
       *Limitation = MIN(L_Si,*Limitation);
    } 
}

void phytoplankton_silica_uptake__(long* PPhytoplankton, double* Silicate,double* cffSiOH4, double *siPhyto, double* biomass)
{
   double MyBiomass, MySiPhyto, MySiCellQuota;
   //cout << "Phosphorus uptake start" << endl;
   TPhytoplanktonGeneric* ptr = (TPhytoplanktonGeneric*) *PPhytoplankton;

   MyBiomass = *biomass * CARBONATOMICWEIGHT; //Conversions from mmol/m3 to mg / m3
   ptr->SetVariableValue("Fortran", MyBiomass,0,"Phytoplankton biomass");
   MySiPhyto = *siPhyto * SILICAATOMICWEIGHT;
   if (MyBiomass > ptr->aMin)MySiCellQuota = MySiPhyto / MyBiomass; 
   else MySiCellQuota = 0.0;
   ptr->SetVariableValue("Fortran", MySiCellQuota,0,"SiCellQuota");
   ptr->SetVariableValue("Fortran", MySiPhyto,0,"SiPhyto");

   if (ptr->GetIntParameterValue("Silica limitation") == 1)
      ptr->SilicaUptake(0, *Silicate);
   if (*Silicate > ptr->aMin)
      *cffSiOH4 = ptr->SiUptake[0] / SILICAATOMICWEIGHT / HOURSTOSECONDS /*/ *Silicate*/;
   else
      *cffSiOH4 = 0.0;
   //cout << "Silicate uptake end" << endl;
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


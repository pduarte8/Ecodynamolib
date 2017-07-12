// Producer object header file
// @ Pedro Duarte & J. Gomes Ferreira
// REV A - 96.02.01

#ifndef PRODOBJT_H
#define PRODOBJT_H

/**
 * NPI work
 * 	03 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#include "EcoDynClass.h"

// DLL header file

#ifdef __BORLANDC__
	#include <vcl.h>

#else  // __BORLANDC__
	#include <cstdio>
	#include <iostream>
	#include <cstdlib>
	using namespace std;

	#define _export 
	
#endif  // __BORLANDC__

class _export TProdutor : public TEcoDynClass
{
	public :
#ifndef _PORT_FORTRAN_
        // constructors invoked outside EcoDyn
        TProdutor(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
            double aLength[], double aWidth[], double aElevation[], int aType[],
            int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[])
            :TEcoDynClass(timeStep, nLines, nColumns, aDepth, aLength, aWidth, aElevation,
                aType, aNBoundary, aEBoundary, aSBoundary, aWBoundary)
        {
            MyPEcoDynClass = (TEcoDynClass*)this;
            BuildProdutor(className);
        };
        TProdutor(char* className, float timeStep, char* morphologyFilename)
            :TEcoDynClass(timeStep)
        {
            MyPEcoDynClass = (TEcoDynClass*)this;
            strcpy(MorphologyFileName, morphologyFilename);
            if (OpenMorphology()) {
                BuildProdutor(className);
            }
        };

		TProdutor(TEcoDynClass* APEcoDynClass, char* className) :TEcoDynClass()
        {
            // Receive pointer to integrate
        	MyPEcoDynClass = APEcoDynClass;
            BuildProdutor(className);
        };

#else
        TProdutor(char* className) :TEcoDynClass()
        {
            MyPEcoDynClass = (TEcoDynClass*)this;
            BuildProdutor(className);
        }
#endif

	~TProdutor()
        {
              if (NumberOfBoxes > 0)
        	{
	/* NPI-Work, 03 Oct 2013, AP - free memory replaces delete [] **
	 ***********
	 */
        		free(PhytoBiomass);
		        free(PhytoLoad);
        		free(PhytoFlux);
		        free(PhytoBalance);
		        free(GPP);
        		free(NPP);
		        free(POMLoss);
        		free(DOMLoss);
		        free(Respired);
        		free(RespiredFlux);
		        free(POMFlux);
        		free(DOMFlux);
        		free(ChlToCarbon);
		        free(ChlCarbonRatio);
        		free(OptimalChlCarbonRatio);
        		free(ChlToBiomass);
        		free(PhytoProd);
		        free(PhytoMortality);
        		free(CriticalLightExposure);
		        free(Pmax);
        		free(Slope);
		        free(Imax);
        		free(Iopt);
		        free(CriticalLight);
        		free(Ks);
            free(KNitrate);
            free(KAmmonia);
		        free(beta);
        		free(ResponseTime);
		        free(InhibitionTime);
        		free(DefaultAEiler);
		        free(AEiler);
        		free(BEiler);
		        free(CEiler);
        		free(MaintenanceRespiration);
		        free(GenericProduct1);
        		free(GenericProduct2);
		        free(GenericProduct3);
        		free(GenericProduct4);
		        free(GenericProduct5);
        		free(GenericProduct6);
		        free(GenericProduct7);
	 
	/*
	  ***********
        		delete [] PhytoBiomass;
		        delete [] PhytoLoad;
        		delete [] PhytoFlux;
		        delete [] PhytoBalance;
		        delete [] GPP;
        		delete [] NPP;
		        delete [] POMLoss;
        		delete [] DOMLoss;
		        delete [] Respired;
        		delete [] RespiredFlux;
		        delete [] POMFlux;
        		delete [] DOMFlux;
        		delete [] ChlToCarbon;
		        delete [] ChlCarbonRatio;
        		delete [] OptimalChlCarbonRatio;
        		delete [] ChlToBiomass;
        		delete [] PhytoProd;
		        delete [] PhytoMortality;
        		delete [] CriticalLightExposure;
		        delete [] Pmax;
        		delete [] Slope;
		        delete [] Imax;
        		delete [] Iopt;
		        delete [] CriticalLight;
        		delete [] Ks;
                delete [] KNitrate;
                delete [] KAmmonia;
		        delete [] beta;
        		delete [] ResponseTime;
		        delete [] InhibitionTime;
        		delete [] DefaultAEiler;
		        delete [] AEiler;
        		delete [] BEiler;
		        delete [] CEiler;
        		delete [] MaintenanceRespiration;
		        delete [] GenericProduct1;
        		delete [] GenericProduct2;
		        delete [] GenericProduct3;
        		delete [] GenericProduct4;
		        delete [] GenericProduct5;
        		delete [] GenericProduct6;
		        delete [] GenericProduct7;
		*************/
        		// put these in TEcoDynClass - set them to null in the constructor
		        // delete them in the destructor if not null
/*        		delete [] Generic;
		        delete [] GenericLoad;
        		delete [] GenericFlux;*/
        	}
        };
		virtual void Go(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Go always override" << endl;
	#endif  // __BORLANDC__
#endif  //_PORT_FORTRAN_
        };
		virtual void Update(double &Value, int BoxNumber,
					 char* ParameterName){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
                     ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Update always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
	virtual void Integrate(){};
	//protected :
        public:
        void BuildProdutor(char* className)
        {
#ifndef _PORT_FORTRAN_
                 strcpy(EcoDynClassName, className);
                 NumberOfLines = MyPEcoDynClass->GetNumberOfLines();
                 NumberOfColumns = MyPEcoDynClass->GetNumberOfColumns();
                 NumberOfLayers =  MyPEcoDynClass->GetNumberOfLayers();
        	// Get array size
        	NumberOfBoxes = MyPEcoDynClass->GetGridSize(); // Size of array - i.e. number of model boxes
        	VariableCount = 0;
        	TimeStep = MyPEcoDynClass->GetTimeStep();
        	ObjectCode = PRODOBJECTCODE;
#endif  // _PORT_FORTRAN_
        	// Initialise pointers
        	if (NumberOfBoxes > 0)                       // Initialise arrays for variable pairs - one
        	{                                            // for boxes and one for loads into boxes
					
	/* NPI-Work, 03 Oct 2013, AP - memory allocated and cleared here **
	 ***********
	 */
		        PhytoBiomass  = (double*) calloc (NumberOfBoxes, sizeof(double));
        		PhytoLoad     = (double*) calloc (NumberOfBoxes, sizeof(double));
		        PhytoFlux     = (double*) calloc (NumberOfBoxes, sizeof(double));
        		PhytoBalance  = (double*) calloc (NumberOfBoxes, sizeof(double));
		        GPP           = (double*) calloc (NumberOfBoxes, sizeof(double));
        		NPP           = (double*) calloc (NumberOfBoxes, sizeof(double));
		        POMLoss       = (double*) calloc (NumberOfBoxes, sizeof(double));
        		DOMLoss       = (double*) calloc (NumberOfBoxes, sizeof(double));
		        Respired      = (double*) calloc (NumberOfBoxes, sizeof(double));
        		RespiredFlux  = (double*) calloc (NumberOfBoxes, sizeof(double));
        		POMFlux       = (double*) calloc (NumberOfBoxes, sizeof(double));
		        DOMFlux       = (double*) calloc (NumberOfBoxes, sizeof(double));
		        ChlToCarbon   = (double*) calloc (NumberOfBoxes, sizeof(double));
        		ChlCarbonRatio = (double*) calloc (NumberOfBoxes, sizeof(double));
        		ChlToBiomass 	= (double*) calloc (NumberOfBoxes, sizeof(double));
		        OptimalChlCarbonRatio = (double*) calloc (NumberOfBoxes, sizeof(double));
        		PhytoMortality  = (double*) calloc (NumberOfBoxes, sizeof(double));
		        PhytoProd    		= (double*) calloc (NumberOfBoxes, sizeof(double));
		        CriticalLightExposure = (double*) calloc (NumberOfBoxes, sizeof(double));
        		Pmax 					= (double*) calloc (NumberOfBoxes, sizeof(double));
		        Slope 				= (double*) calloc (NumberOfBoxes, sizeof(double));
        		Imax 					= (double*) calloc (NumberOfBoxes, sizeof(double));
		        Iopt 					= (double*) calloc (NumberOfBoxes, sizeof(double));
        		CriticalLight = (double*) calloc (NumberOfBoxes, sizeof(double));
		        Ks 						= (double*) calloc (NumberOfBoxes, sizeof(double));
                        KNitrate 			= (double*) calloc (NumberOfBoxes, sizeof(double));
                        KAmmonia 			= (double*) calloc (NumberOfBoxes, sizeof(double));
        		beta 					= (double*) calloc (NumberOfBoxes, sizeof(double));
		        ResponseTime 	= (double*) calloc (NumberOfBoxes, sizeof(double));
        		InhibitionTime = (double*) calloc (NumberOfBoxes, sizeof(double));
		        DefaultAEiler = (double*) calloc (NumberOfBoxes, sizeof(double));
        		AEiler 				= (double*) calloc (NumberOfBoxes, sizeof(double));
		        BEiler 				= (double*) calloc (NumberOfBoxes, sizeof(double));
        		CEiler 				= (double*) calloc (NumberOfBoxes, sizeof(double));
		        MaintenanceRespiration = (double*) calloc (NumberOfBoxes, sizeof(double));
        		GenericProduct1 = (double*) calloc (NumberOfBoxes, sizeof(double));
		        GenericProduct2 = (double*) calloc (NumberOfBoxes, sizeof(double));
        		GenericProduct3 = (double*) calloc (NumberOfBoxes, sizeof(double));
		        GenericProduct4 = (double*) calloc (NumberOfBoxes, sizeof(double));
        		GenericProduct5 = (double*) calloc (NumberOfBoxes, sizeof(double));
		        GenericProduct6 = (double*) calloc (NumberOfBoxes, sizeof(double));
        		GenericProduct7 = (double*) calloc (NumberOfBoxes, sizeof(double));
	/**************/
        	}
        	else {
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
						MessageBox(0,
					  "Producer object array not dimensioned",
					  "EcoDynamo alert",
					  MB_OK | MB_ICONHAND);
	#else //__BORLANDC__
						cerr << "TProdutor::BuildProdutor - Producer object array not dimensioned" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
					}

        };
		double
							*PhytoBiomass,
							*PhytoLoad,
							*PhytoFlux,
							*PhytoProd,
							*PhytoBalance,            // NG 14/6/95
							*GPP,
							*NPP,
							*POMLoss,
							*DOMLoss,
							*PhytoMortality,               // JGF 97.07.09

							*Respired,
							*POMFlux,                // Fluxes - derivatives
							*DOMFlux,
							*RespiredFlux,
							*IMPOMLoss, *IFPOMLoss,  // Iterim mass and flux
							*IMDOMLoss, *IFDOMLoss,
							*IMRespired,
							*ChlToCarbon,
							*ChlCarbonRatio,
							*OptimalChlCarbonRatio,
							*ChlToBiomass,
							*CriticalLightExposure,
							*Pmax,               // P-I curve parameters
							*Imax,
							*Slope,
							*Iopt,
							*CriticalLight,
							*Ks,
                                                        *KNitrate,
                                                        *KAmmonia,
							*beta,
							*ResponseTime,
							*InhibitionTime,
							*DefaultAEiler,    // P-I Eiler's curve parameters
							*AEiler,           // P-I Eiler's curve parameters
							*BEiler,           // P-I Eiler's curve parameters
							*CEiler,           // P-I Eiler's curve parameters

							*MaintenanceRespiration,

							*GenericProduct1,
							*GenericProduct2,
							*GenericProduct3,
							*GenericProduct4,
							*GenericProduct5,
							*GenericProduct6,
							*GenericProduct7;


		double RiverPhyto,
				 OceanPhyto;

//		AdvectionDiffusionRecord *PhytoplanktonInterfaceExchange;

		double PhoticDepth,
				 I_timestep,
				 TidalHeight,
				 KValue,
				 Productivity,
				 LimitingNutrient,

				 // Conversion factors
				 // ChlToBiomass,
				 NPPtoGPP;

		double DocLoss,
				 DocStressLoss,
				 DeathLoss,
				 RedfieldCFactor,
				 RedfieldNFactor,
				 RedfieldPFactor,
                                 RedfieldNFactorInAtoms,
                                 RedfieldSiFactor, 
				 RespirationCoefficient,
				 q10PH,
				 RTMPH,
				 RTPH,
             TemperatureAugmentationRate,
             WaterTemperature;

        double phi;

		virtual void Production(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Production always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void Respiration(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Respiration always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void Photorespiration(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Photorespiration always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void DarkCarbonFixation(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::DarkCarbonFixation always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void Exudation(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Exudation always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void Senescence(int BoxNumber){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Senescence always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void Mortality(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::Mortality always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };
		virtual void LightShadeAdaptation(){
#ifndef _PORT_FORTRAN_
	#ifdef __BORLANDC__
            ::MessageBox(0,"Always override","EcoDynamo Alert",MB_OK);
	#else  // __BORLANDC__
						cerr << "TProdutor::LightShadeAdaptation always override" << endl;
	#endif  // __BORLANDC__
#endif  // _PORT_FORTRAN_
        };

};

#ifndef _PORT_FORTRAN_
static TEcoDynClass *PProdutor;
#endif  // _PORT_FORTRAN_

#endif //PRODOBJT_H



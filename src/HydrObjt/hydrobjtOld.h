// hydrodynamic object header file
// REV A - 95.09.25
/**
 * NPI work
 * 	14 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifndef HYDROBJT_H
#define HYDROBJT_H

#include "EcoDynClass.h"
#include "HydrodynamicModel.h"
#include "AtmosphericModel.h"

// DLL header file

#ifndef __BORLANDC__
#define _export
#endif  // __BORLANDC__

class _export TTransport : public TEcoDynClass
{
	public :
        // constructors used outside EcoDyn
        TTransport(char* className, float timeStep, char* morphologyFilename);
        TTransport(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);

		TTransport(TEcoDynClass* PEcoDynClass, char* className);
		~TTransport();
        virtual void freeMemory();

		virtual void Go(){};

		virtual void Go(double*,
				double*,
                double ARiver,
                double AOcean);

		virtual void Go(double*,
				double*,
				double*,
                double ARiver,
                double AOcean);


		virtual void Inquiry(char* srcName,
        			 double &Value,
        			 int BoxNumber,
                     char* ParameterName,
                     int AnObjectCode);
		// Make into a Go function

		virtual void EnergyAcrossSurface(double*,
										 int BoxNumber,
                                         char* ParameterName);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

	protected :
		double
                  *Salt,
                  *SaltLoad,
                  *FlowDischarge,
                  *FlowUptake,
                  *MixedLayer;
                  /**MyLoads,
                  *MyLosses*/

		double RiverSalt,
				 OceanSalt,ATimeStep;
        double MyLatitude;
		virtual void AdvectDiffuse(double* Generic);
		virtual void MixedLayerDepth();
        void BuildTransport(char* className);

        LoadsRecord *MyLoadRecord;
        LossesRecord *MyLossesRecord;
        RiversRecord *MyRiverRecord;
        SeaBoundariesRecord *MySeaBoundariesRecord;

};

class _export TMixedReactor : public TTransport
{
	public :
        TMixedReactor(TEcoDynClass* PEcoDynClass, char* className);
		~TMixedReactor();
        virtual void freeMemory();
        virtual void Inquiry(char* srcName,double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        virtual bool SaveVariables();
        virtual void Go();
        virtual void Go(double*,double*,double ARiver,double AOcean);
     protected :
        void BuildMixedReactor();
        virtual void Continuity();
        virtual void Mix(double* Generic);
        virtual void DischargeForcing(int AMoment);
        virtual void UptakeForcing(int AMoment);
        double *DynamicHeight,*PastHeight,*WaterDensity, *BoxDepth, *PastBoxDepth, *BoxLength, *BoxVolume, *PastBoxVolume,
               *BoxWidth, *BoxCrossSectionalArea, *PastBoxCrossSectionalArea;
};

class _export TMixedLayerTransport : public TTransport
{
	public :
        // constructors used outside EcoDyn
        TMixedLayerTransport(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TMixedLayerTransport(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aMixedLayerDepth[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aRichardsonNumber[], double aExchangedFraction[],
                   int nParams, double latitude, double coefManning);

		TMixedLayerTransport(TEcoDynClass* PEcoDynClass, char* className);
		~TMixedLayerTransport();
        virtual void freeMemory();
		virtual void Go();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		virtual void Inquiry(char* srcName,
        			 double &Value,
        			 int BoxNumber,
                     char* ParameterName,
					 int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
		double MixedLayerDepth, CoefManning, gravity;

		double
        	 *UVelocity,
             *VVelocity,
             *WaterDensity,
             *RichardsonNumber,
             *ExchangedFraction;

		int ConvectiveBox;
		bool Convection;

		virtual void DeterministicPhase();
		virtual void AdvectDiffuse(double* Generic);
		virtual void ConvectiveMixing();
		virtual void DynamicMixing();
		virtual double Coriolis(float ALatitude);
		virtual double WindStress(double AWindSpeed,
								  double AWindComponent);
        virtual double CoefRugosidade(int ABoxNumber);
        void PreBuildMixedLayerTransport();
        void BuildMixedLayerTransport();
};


class _export TMixedLayerTransportStoa : public TMixedLayerTransport
{
	public :
        // constructors used outside EcoDyn
        TMixedLayerTransportStoa(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TMixedLayerTransportStoa(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aMixedLayerDepth[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aRichardsonNumber[], double aExchangedFraction[],
                   int nParams, double latitude, double coefManning);

		TMixedLayerTransportStoa(TEcoDynClass* PEcoDynClass, char* className);
	protected :
		virtual void DeterministicPhase();
};





class _export TOneDimensionalDITransport : public TTransport
{
/*This object computes the one dimensional motion of a fluid with homogeneous
  density. The motion is forced by the slope of the fluid surface. The basic equation
  is a unidimensional version of the Navie-Stokes equation. Bottom friction can be eliminated
  in the parameters file.*/
	public :
        // constructors used outside EcoDyn
		TOneDimensionalDITransport(char* className, float timeStep, char* morphologyFilename);
        TOneDimensionalDITransport(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);

		TOneDimensionalDITransport(TEcoDynClass* PEcoDynClass, char* className);
		~TOneDimensionalDITransport();
        virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
		virtual void Go();
                virtual void Go(double* AGeneric,
                                double* AGenericLoad,
                                double ARiver,
                                double AOcean);
		virtual void Inquiry(char* srcName,
        			 double &Value,
                     int BoxNumber,
                     char* ParameterName,
                     int AnObjectCode);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
        void BuildOneDimensionalDITransport();
		virtual void EquationOfMotion();
		virtual void Continuity();
		virtual void AdvectDiffuse(double* Generic);
        virtual void Evaporate();
        virtual double GetSaturationVapourPressure(double ATemperature);
        virtual double WaterVapourMassTransferCoefficient(int ABoxNumber);
        virtual double CoefRugosidade(int ABoxNumber);
        double
             *DynamicHeight,
             *PastHeight,
             *UVelocity,
             *PastUVelocity,
             *FlowBetweenBoxes,
             *PastFlowBetweenBoxes,
             *WaterDensity,
             *CoefManning,
             *BoxDepth, *PastBoxDepth, *BoxLength, *BoxVolume, *PastBoxVolume,
             *BoxWidth, *BoxCrossSectionalArea, *PastBoxCrossSectionalArea,
//             *PastGeneric,
             *EvaporativeFlux;
		double Gravity, StartYear, AlfaA, AlfaB, HydraulicRadius, RiverFlow,
			   MeanWidthBeforeBox, MeanWidthAfterBox, HorizontalDiffCoef, kdiffusion;
        bool CalculateEvaporation;
        int GridColumns, GridLines, UpperLayer;
};

class _export TOneDimensionalDIExplicit : public TOneDimensionalDITransport
{
	public :
        // constructors used outside EcoDyn
        TOneDimensionalDIExplicit(char* className, float timeStep, char* morphologyFilename);
        TOneDimensionalDIExplicit(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);

		TOneDimensionalDIExplicit(TEcoDynClass* PEcoDynClass, char* className);
		virtual void Go();
	protected :
		virtual void EquationOfMotion();
};


/*This object calculates the semitimestep as half of the timestep given by the shell*/
class _export TBiDimensional : public TOneDimensionalDITransport
{
	public :
        // constructors used outside EcoDyn
		TBiDimensional(char* className, float timeStep, char* morphologyFilename);
        TBiDimensional(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);

		TBiDimensional(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensional();
        virtual void freeMemory();
		virtual void Go();
		virtual void Go(double* AGeneric,
                double* AGenericLoad,
                double ARiver,
                double AOcean);
		virtual void Inquiry(char* srcName,
        			 double &Value,
                     int BoxNumber,
                     char* ParameterName,
                     int AnObjectCode);
        VNA* Inquiry(char* srcName, int &TimeSeriesVariables);
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

	protected :
        virtual void ClearMeanSamples();
        virtual void UpdateMeanSamples();

        void BuildBiDimensional();
		virtual void EquationOfMotion();
		virtual void Continuity();
        virtual void AdvectDiffuse(double* Generic);
		virtual void TransportEquation();
        virtual void TideForcing();
        virtual void RiverForcing();
        virtual void UpdateOfTidalBoundariesVelocities();
        virtual double UPStreamUVelocity(int i, int j);
        virtual double UPStreamVVelocity(int i, int j);
        virtual double DOWNStreamUVelocity(int i, int j);
        virtual double DOWNStreamVVelocity(int i, int j);
		virtual double UpperUVelocity(int i, int j);
		virtual double LowerUVelocity(int i, int j);
		virtual double LeftVVelocity(int i, int j);
		virtual double RightVVelocity(int i, int j);
        virtual double UpperLeftCornerUVelocity(int i, int j);
        virtual double UpperRightCornerUVelocity(int i, int j);
        virtual double LowerLeftCornerUVelocity(int i, int j);
        virtual double LowerRightCornerUVelocity(int i, int j);
        virtual double UpperLeftCornerVVelocity(int i, int j);
        virtual double UpperRightCornerVVelocity(int i, int j);
        virtual double LowerLeftCornerVVelocity(int i, int j);
        virtual double LowerRightCornerVVelocity(int i, int j);
        virtual double UpperLeftCornerDepth(int i, int j);
        virtual double UpperRightCornerDepth(int i, int j);
        virtual double LowerLeftCornerDepth(int i, int j);
        virtual double LowerRightCornerDepth(int i, int j);
        virtual double Coriolis(float ALatitude);
        virtual double UCoriolis(int i, int j);
        virtual double VCoriolis(int i, int j);
        virtual double UHorizontalDiffusion(int i, int j);
        virtual double UVerticalDiffusion(int i, int j);
        virtual double VHorizontalDiffusion(int i, int j);
        virtual double VVerticalDiffusion(int i, int j);
        virtual double UAdvection(int i, int j);
        virtual double VAdvection(int i, int j);
        virtual double AdvectionOfRightSideOfUEquationOfMotion(int i, int j);
        virtual double GravityOfRightSideOfUEquationOfMotion(int i, int j);
        virtual double AdvectionOfRightSideOfVEquationOfMotion(int i, int j);
        virtual double GravityOfRightSideOfVEquationOfMotion(int i, int j);
        virtual double UForRugosity(int i, int j);
        virtual double VForRugosity(int i, int j);
        virtual double aUCalculation(int i, int j);
        virtual double bUCalculation(int i, int j);
        virtual double cUCalculation(int i, int j);
        virtual double aVCalculation(int i, int j);
        virtual double bVCalculation(int i, int j);
        virtual double cVCalculation(int i, int j);
        virtual double WesternComponentOfContinuity(int i, int j);
        virtual double EasternComponentOfContinuity(int i, int j);
        virtual double NorthernComponentOfContinuity(int i, int j);
        virtual double SouthernComponentOfContinuity(int i, int j);
        virtual double VUAdvectionOfExplicitUEquation(int i, int j);
        virtual double UVAdvectionOfExplicitVEquation(int i, int j);
        virtual double WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltWesternHorizontalDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltEasternHorizontalDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltNorthernVerticalDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltSouthernVerticalDiffusion(int i, int j, double* PastGeneric);
        virtual double CoefRugosidade(int ABoxNumber);
        virtual double UVAdvectionOfVEquation(int i, int j);
        virtual double UVAdvectionOfUEquation(int i, int j);
        double *VVelocity,
                *UVelocityPast, *VVelocityPast,
                *UFlowBetweenBoxes,*VFlowBetweenBoxes,
                *atemporary, *btemporary, *ctemporary, *rtemporary,
                *HorizontalDiffCoefU, *HorizontalDiffCoefV;

        double ATimeStep, ASpatialStep, CriticalDepthForLandBoundary;
        double AUPStreamU,AUPStreamV, ADOWNStreamU, ADOWNStreamV;
   	    double AUpperLeftDepth, AUpperRightDepth, ALowerLeftDepth, ALowerRightDepth;
   	    double AUpperLeftUVelocity, AUpperRightUVelocity, ALowerLeftUVelocity, ALowerRightUVelocity;
   	    double AUpperLeftVVelocity, AUpperRightVVelocity, ALowerLeftVVelocity, ALowerRightVVelocity;
        double SurfaceHeight;
        int GridColumns, GridLines, SemiTimeStep;
        int WesternBoundaryNumber, SouthernBoundaryNumber,
            EasternBoundaryNumber, NorthernBoundaryNumber;

        double V1, V2, V3, V4;

        double getUFlow(int i);
        double getVFlow(int i);
        double getUVelocity(int i);
        double getVVelocity(int i);

        int samples;
        double *MeanUVelocity, *MeanVVelocity,
                *MeanUFlow, *MeanVFlow;
        double CorrectionOfHorizontalDiffCoef(double ADepth, double AVelocityComponent);
};



/*This object assumes that the timestep given by the shell is the semitimestep. Therefore for the results of two simulations based
on this object and its ascendant to be equal, the timestep used with this object must be half of that used with his ascendant*/
class _export TBiDimensionalNormalTimeStep : public TBiDimensional
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalNormalTimeStep(char* className, float timeStep, char* morphologyFilename);
        TBiDimensionalNormalTimeStep(char* className, float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);

		TBiDimensionalNormalTimeStep(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalNormalTimeStep();
        virtual void Go();
        virtual void Reinitialize();
        virtual void Continuity();
        virtual void SimetryCheck();
    private:
        void BuildBiDimensionalNormalTimeStep();
};

class _export TBiDimensionalExplicit : public TBiDimensionalNormalTimeStep
{
	public :
        // constructor used outside EcoDyn
		TBiDimensionalExplicit(char* className, float timeStep, char* morphologyFilename);

		TBiDimensionalExplicit(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalExplicit();
        virtual void Go();
        virtual void Reinitialize();
   protected :
		virtual void EquationOfMotion();
};

class _export TBiDimensionalTidalTest : public TBiDimensionalNormalTimeStep
{
	public :
        // constructor used outside EcoDyn
		TBiDimensionalTidalTest(char* className, float timeStep, char* morphologyFilename);

		TBiDimensionalTidalTest(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalTidalTest();
/*        void Go();
        virtual void EquationOfMotion();
        virtual double UPStreamUVelocity(int i, int j);
        virtual double UPStreamVVelocity(int i, int j);
        virtual double DOWNStreamUVelocity(int i, int j);
        virtual double DOWNStreamVVelocity(int i, int j); **/
        virtual void TideForcing();
        virtual void RiverForcing();
        //void Reinitialize();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        double caudal;
        double *A, *B;
        int *Indx;
};


class _export TBiDimensionalSango1 : public TBiDimensionalNormalTimeStep
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalSango1(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TBiDimensionalSango1(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary, int northernTidalReference,
                   int southernTidalReference);

		TBiDimensionalSango1(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalSango1();
        virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        virtual void Go();
        virtual void Reinitialize();

        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

    protected :
        void BuildBiDimensionalSango1();
        virtual void EquationOfMotion();
        virtual void TideForcing();
        virtual void RiverForcing();
        virtual void UpdateOfTidalBoundariesVelocities();
        virtual double UCoriolis(int i, int j);
        virtual double VCoriolis(int i, int j);
        virtual double UForRugosity(int i, int j);
        virtual double VForRugosity(int i, int j);
    private:
        int NorthernTidalReference, SouthernTidalReference;
};



class _export TBiDimensionalSango2 : public TBiDimensionalNormalTimeStep
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalSango2(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TBiDimensionalSango2(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aFlow[], double aVFlow[],
                   int nParams, double latitude, double* pCoefManning,
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary, int northernTidalReference,
                   int southernTidalReference, double phaseLag);

		TBiDimensionalSango2(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalSango2();
        virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        virtual void TideForcing();
        int NorthernTidalReference, SouthernTidalReference;
        double PhaseLag;
        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

    protected :
        void BuildBiDimensionalSango2();
};


class _export TBiDimensionalSango3 : public TBiDimensionalSango1  //Object with continuosly varying harmonics from south to north
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalSango3(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TBiDimensionalSango3(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary, int northernTidalReference,
                   int southernTidalReference);

		TBiDimensionalSango3(TEcoDynClass* PEcoDynClass, char* className);
        virtual void TideForcing();
};

class _export TBiDimensionalIntertidal : public TBiDimensionalNormalTimeStep
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalIntertidal(char* className, float timeStep, char* morphologyFilename);
        TBiDimensionalIntertidal(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);

		TBiDimensionalIntertidal(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalIntertidal();
        virtual void freeMemory();
        virtual void Go();
        virtual void Reinitialize();
		virtual void Continuity();

	protected :
        void BuildBiDimensionalIntertidal();
		virtual void EquationOfMotion();
		void DoContinuity();
        virtual void AdvectDiffuse(double* Generic);
        virtual void TideForcing();
        virtual void RiverForcing();
        virtual void UpdateOfTidalBoundariesVelocities();
        virtual double UPStreamUVelocity(int i, int j);
        virtual double UPStreamVVelocity(int i, int j);
        virtual double DOWNStreamUVelocity(int i, int j);
        virtual double DOWNStreamVVelocity(int i, int j);
		virtual double UpperUVelocity(int i, int j);
		virtual double LowerUVelocity(int i, int j);
		virtual double LeftVVelocity(int i, int j);
		virtual double RightVVelocity(int i, int j);
        virtual double UpperLeftCornerUVelocity(int i, int j);
        virtual double UpperRightCornerUVelocity(int i, int j);
        virtual double LowerLeftCornerUVelocity(int i, int j);
        virtual double LowerRightCornerUVelocity(int i, int j);
        virtual double UpperLeftCornerVVelocity(int i, int j);
        virtual double UpperRightCornerVVelocity(int i, int j);
        virtual double LowerLeftCornerVVelocity(int i, int j);
        virtual double LowerRightCornerVVelocity(int i, int j);
        virtual double UpperLeftCornerDepth(int i, int j);
        virtual double UpperRightCornerDepth(int i, int j);
        virtual double LowerLeftCornerDepth(int i, int j);
        virtual double LowerRightCornerDepth(int i, int j);
        virtual double UCoriolis(int i, int j);
        virtual double VCoriolis(int i, int j);
        virtual double UHorizontalDiffusion(int i, int j);
        virtual double UVerticalDiffusion(int i, int j);
        virtual double VHorizontalDiffusion(int i, int j);
        virtual double VVerticalDiffusion(int i, int j);
        virtual double GravityOfRightSideOfUEquationOfMotion(int i, int j);
        virtual double GravityOfRightSideOfVEquationOfMotion(int i, int j);
        virtual double UForRugosity(int i, int j);
        virtual double VForRugosity(int i, int j);
        virtual double aUCalculation(int i, int j);
        virtual double bUCalculation(int i, int j);
        virtual double cUCalculation(int i, int j);
        virtual double aVCalculation(int i, int j);
        virtual double bVCalculation(int i, int j);
        virtual double cVCalculation(int i, int j);
        virtual double WesternComponentOfContinuity(int i, int j);
        virtual double EasternComponentOfContinuity(int i, int j);
        virtual double NorthernComponentOfContinuity(int i, int j);
        virtual double SouthernComponentOfContinuity(int i, int j);
        virtual double VUAdvectionOfExplicitUEquation(int i, int j);
        virtual double UVAdvectionOfExplicitVEquation(int i, int j);
        virtual double WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltWesternHorizontalDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltEasternHorizontalDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltNorthernVerticalDiffusion(int i, int j, double* PastGeneric);
        virtual double SaltSouthernVerticalDiffusion(int i, int j, double* PastGeneric);
        virtual double CoefRugosidade(int ABoxNumber);
        virtual double UVAdvectionOfVEquation(int i, int j);
        virtual double UVAdvectionOfUEquation(int i, int j);
        virtual void DepthCorrection();
        virtual double UAdvectionOfUEquation(int i, int j, int k);
        virtual double GravityOfUEquation(int i, int j, int k);
        virtual double DragOfUEquation(int i, int j);
        virtual double VAdvectionOfVEquation(int i, int j, int k);
        virtual double GravityOfVEquation(int i, int j, int k);
        virtual double DragOfVEquation(int i, int j);

        bool CriticalDepthCheck;
        double MinDepthForAdvection;
};

class _export TBiDimensionalDouro : public TBiDimensionalIntertidal
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalDouro(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TBiDimensionalDouro(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary);

		TBiDimensionalDouro(TEcoDynClass* PEcoDynClass, char* className);
        ~TBiDimensionalDouro();
        virtual void freeMemory();
        virtual void TideForcing();
        virtual void RiverForcing();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);

        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

    protected :
        void BuildBiDimensionalDouro();
};

class _export TBiDimensionalRiaFormosa : public TBiDimensionalDouro
{
	public :
        // constructors used outside EcoDyn
		TBiDimensionalRiaFormosa(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TBiDimensionalRiaFormosa(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary);

		TBiDimensionalRiaFormosa(TEcoDynClass* PEcoDynClass, char* className);

    protected :
        void BuildBiDimensionalRiaFormosa();
        virtual void RiverForcing();
        double caudal;
};


class _export TCarl2D : public TBiDimensionalDouro
{
	public :
        // constructors used outside EcoDyn
		TCarl2D(char* className, float timeStep, char* morphologyFilename,
                            char* variablesFilename, char* parametersFilename);
        TCarl2D(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[],
                   int nVars, VNA aVarsNames[], double aSalt[], double aDynamicHeight[],
                   double aWaterDensity[], double aUVelocity[], double aVVelocity[],
                   double aFlow[], double aVFlow[],
                   int nParams, double latitude, double aCoefManning[],
                   double gravity, double horizontalDiffCoef, double startYear,
                   double criticalDepthForLandBoundary);

		TCarl2D(TEcoDynClass* PEcoDynClass, char* className);

    protected :
        void BuildBiDimensionalCarl();
        virtual void RiverForcing();
        double TotalHours;
};

class _export TBiDimensionalIntertidalTransportImplicit : public TBiDimensionalRiaFormosa
{
	public :
		TBiDimensionalIntertidalTransportImplicit(TEcoDynClass* PEcoDynClass, char* className);
    protected :
        virtual void AdvectDiffuse(double* Generic);
        virtual double a1Calculation(int i, int j, int index1);
        virtual double b1Calculation(int i, int j, int index1);
        virtual double c1Calculation(int i, int j, int index1);
        virtual double a2Calculation(int i, int j, int index1);
        virtual double b2Calculation(int i, int j, int index1);
        virtual double c2Calculation(int i, int j, int index1);
        virtual double r1Calculation(int index, double MyWestUFlow, double MyEastUFlow, double MyNorthVFlow, double MySouthVFlow,
                                                double MyNorthDiffusion, double MySouthDiffusion, double* PastGeneric);
        virtual double r2Calculation(int index, double MyWestUFlow, double MyEastUFlow, double MyNorthVFlow, double MySouthVFlow,
                                                double MyWestDiffusion, double MyEastDiffusion, double* PastGeneric);
        virtual void CorrectRForRiverAndOceanBoundaries(int index1, double Correction);
};

class _export TBiDimensionalTidalTestIntertidal : public TBiDimensionalIntertidal
{
	public :
		TBiDimensionalTidalTestIntertidal(TEcoDynClass* PEcoDynClass, char* className);
      ~TBiDimensionalTidalTestIntertidal();
      virtual void TideForcing();
      virtual void RiverForcing();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
      double caudal;
};

class _export TBiDimensionalTimeSeries : public TBiDimensionalRiaFormosa
{
	public :
        // constructors used outside EcoDyn
		/*TBiDimensionalTimeSeries(char* className, float timeStep, char* morphologyFilename);
        TBiDimensionalTimeSeries(char* className,
                   float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);  */

		TBiDimensionalTimeSeries(TEcoDynClass* PEcoDynClass, char* className,
        		bool readValues, bool cyclic, int nrOfPeriods, SimulationPeriod* sPeriods);
		~TBiDimensionalTimeSeries();
        virtual void freeMemory();

        virtual void Go();
        // AP, 2006.01.17
		virtual void Go(double* AGeneric,
                double* AGenericLoad,
                double ARiver,
                double AOcean);
        // AP, 2005.05.17
        virtual void Go(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad,
                        double ARiver, double AOcean);
        //Pedro Duarte, 2005.07.07
        virtual void Go(double *A, double *ALoad, double *ARiverLoad,
                        double *B, double *BLoad, double *BRiverLoad,
                        double *C, double *CLoad, double *CRiverLoad,
                        double *D, double *DLoad, double *DRiverLoad,
                        double *E, double *ELoad, double *ERiverLoad,
                        double ARiver, double AOcean);
        //Pedro Duarte, 2005.06.20
        virtual void Go(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad,
                        double *F, double *FLoad,
                        double *G, double *GLoad,
                        double *H, double *HLoad,
                        double ARiver, double AOcean);


        virtual void Inquiry(char* srcName,
        			 double &Value,
                     int BoxNumber,
                     char* ParameterName,
                     int AnObjectCode);
        virtual void Reinitialize();
        virtual void ResetValues();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

        // AP, 2007.07.09
        virtual bool SaveVariables();

    protected:
        void TimeStepCheck();
        void ReadTimeSeriesVariables();
        bool ReadTimeSeriesValues(int fileIndex);
        long GetCyclicTime(long simulationTime);
        int GetStep(long simulationTime, int stepI, int stepF);
        virtual void ReadVelocitiesAndFlows();
        virtual void RiverForcing();
        virtual void Continuity();
        virtual void AdvectDiffuse(double* Generic, double* GenericLoad);
        virtual void AdvectDiffuse(double *A, double *ALoad,
                           double *B, double *BLoad,
                           double *C, double *CLoad,
                           double *D, double *DLoad,
                           double *E, double *ELoad);
        virtual void AdvectDiffuse(double *A, double *ALoad, double *ARiverLoad,
                           double *B, double *BLoad, double *BRiverLoad,
                           double *C, double *CLoad, double *CRiverLoad,
                           double *D, double *DLoad, double *DRiverLoad,
                           double *E, double *ELoad, double *ERiverLoad);
        virtual void AdvectDiffuse(double *A, double *ALoad,
                           double *B, double *BLoad,
                           double *C, double *CLoad,
                           double *D, double *DLoad,
                           double *E, double *ELoad,
                           double *F, double *FLoad,
                           double *G, double *GLoad,
                           double *H, double *HLoad);
        double WesternComponentOfContinuity(int i, int j);
        double EasternComponentOfContinuity(int i, int j);
        double NorthernComponentOfContinuity(int i, int j);
        double SouthernComponentOfContinuity(int i, int j);
        virtual double WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual double SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
        virtual bool CheckTemporalResolutionForAdvection(int ABoxNumber, double ABoxDepth,
                                        double MyWestUFlow, double MyEastUFlow,
                                        double MyNorthVFlow, double MySouthVFlow);
        bool CheckTemporalResolutionForMassDiffusion(int ABoxNumber, double ABoxDepth,
                                                     double MyWestDiffusion, double MyEastDiffusion,
                                                     double MyNorthDiffusion, double MySouthDiffusion,double AGeneric);

        void CorrectElevation();
        double *mUVelocity,
        	   *mVVelocity,
               *mUFlow,
               *mVFlow;

        long *timeSeries;
        int steps;
        int indexFile;
        int myTimeSeriesStep, MyNumberOfBoxes;
        long diffTime;
        double DefaultTimeStep;
        int *MyBoxNumber;
//        double *PastGenericA, *PastGenericB, *PastGenericC, *PastGenericD, *PastGenericE, *PastGenericF, *PastGenericG, *PastGenericH;

        double *InitialBoxDepth, *InitialBoxElevation;
        bool cyclicRun;     // AP, 2005.07.15
        long meanTimeDiff;
        long firstMeanTime;
        int firstIndexFileCycle;
        int cycle;

        int MeanValuesSamplesNumber;          // AP, 2007.02.15

        int startTimeIndexFile;
        long startTime;
        bool firstRead;

        SimulationPeriod periods[12];  // AP, 2005.11.30
        int numberOfPeriods;
        int periodNumber;
        long periodCycle;

        int GetPeriodIndex(long simulationTime);
        void LoadPeriodInMemory(int period);

        int NumberOfTSVariables;          // Number of TimeSeries Variables
        VNA* TSVariableNameArray;         // Names of the TimeSeries Variables

};

class _export TBiDimensionalTimeSeriesVariableDT : public TBiDimensionalTimeSeries
{
	public :
           TBiDimensionalTimeSeriesVariableDT(TEcoDynClass* PEcoDynClass, char* className,
        		bool readValues, bool cyclic, int nrOfPeriods, SimulationPeriod* sPeriods);
           ~TBiDimensionalTimeSeriesVariableDT();
           virtual void freeMemory();
           virtual void Reinitialize();
           //virtual void Integrate();
        protected:

           typedef struct CriticalCells
           {
              bool Critical;
              int ABoxNumber;
              double CellTimeStep;
              int NumberOfSteps;
              double NorthFlow, SouthFlow, WestFlow, EastFlow;
           }  CellRecord;

           typedef struct flow_critical_cells
           {
              double NorthAdvecFlow, SouthAdvecFlow, WestAdvecFlow, EastAdvecFlow,
                     NorthDiffFlow, SouthDiffFlow, WestDiffFlow, EastDiffFlow;
           }  FlowCriticalCells;

           virtual void Continuity();
           void CheckTemporalResolution(int ABoxNumber, double ABoxDepth,
                                        double MyWestUFlow, double MyEastUFlow,
                                        double MyNorthVFlow, double MySouthVFlow);
           virtual void AdvectDiffuse(double* Generic, double* GenericLoad);
           virtual void AdvectDiffuseCriticalCells(
                double* Generic, double* PastGeneric, FlowCriticalCells* flowCC, double* GenericLoad);
           virtual void AdvectDiffuseCriticalCells(
               double* GenericA, double* PastGenericA, double *ALoad, FlowCriticalCells* flowCCA,
               double* GenericB, double* PastGenericB, double *BLoad, FlowCriticalCells* flowCCB,
               double* GenericC, double* PastGenericC, double *CLoad, FlowCriticalCells* flowCCC,
               double* GenericD, double* PastGenericD, double *DLoad, FlowCriticalCells* flowCCD,
               double* GenericE, double* PastGenericE, double *ELoad, FlowCriticalCells* flowCCE);
           virtual void AdvectDiffuse(double *A, double *ALoad,
                           double *B, double *BLoad,
                           double *C, double *CLoad,
                           double *D, double *DLoad,
                           double *E, double *ELoad);
           virtual void AdvectDiffuseCriticalCells(
               double* GenericA, double* PastGenericA, double *ALoad, FlowCriticalCells* flowCCA,
               double* GenericB, double* PastGenericB, double *BLoad, FlowCriticalCells* flowCCB,
               double* GenericC, double* PastGenericC, double *CLoad, FlowCriticalCells* flowCCC,
               double* GenericD, double* PastGenericD, double *DLoad, FlowCriticalCells* flowCCD,
               double* GenericE, double* PastGenericE, double *ELoad, FlowCriticalCells* flowCCE,
               double* GenericF, double* PastGenericF, double *FLoad, FlowCriticalCells* flowCCF,
               double* GenericG, double* PastGenericG, double *GLoad, FlowCriticalCells* flowCCG,
               double* GenericH, double* PastGenericH, double *HLoad, FlowCriticalCells* flowCCH);
           virtual void AdvectDiffuse(double *A, double *ALoad,
                           double *B, double *BLoad,
                           double *C, double *CLoad,
                           double *D, double *DLoad,
                           double *E, double *ELoad,
                           double *F, double *FLoad,
                           double *G, double *GLoad,
                           double *H, double *HLoad);

           double WesternComponentOfContinuity(int i, int j);
           double EasternComponentOfContinuity(int i, int j);
           double NorthernComponentOfContinuity(int i, int j);
           double SouthernComponentOfContinuity(int i, int j);
           virtual double WesternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
           virtual double EasternComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
           virtual double NorthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
           virtual double SouthernComponentOfAdvectionDiffusion(int i, int j, double* PastGeneric);
           virtual double SaltWesternHorizontalDiffusion(int i, int j, double* PastGeneric);
           virtual double SaltEasternHorizontalDiffusion(int i, int j, double* PastGeneric);
           virtual double SaltNorthernVerticalDiffusion(int i, int j, double* PastGeneric);
           virtual double SaltSouthernVerticalDiffusion(int i, int j, double* PastGeneric);

           CellRecord* ACellRecord;
           int *Cells;
           int NumberOfCriticalCells, CriticalNumberOfSteps;
           double CriticalTimeStep;
           double *CriticalCellDepth, *CriticalCellHeight;
};


class _export TBiDimensionalTimeSeriesTransportImplicit : public TBiDimensionalTimeSeries
{
	public :
           TBiDimensionalTimeSeriesTransportImplicit(TEcoDynClass* PEcoDynClass, char* className,
        		bool readValues, bool cyclic, int nrOfPeriods, SimulationPeriod* sPeriods);
           ~TBiDimensionalTimeSeriesTransportImplicit();
           virtual void freeMemory();
        protected:
           virtual void AdvectDiffuse(double* Generic);
           double ACalculation(int i, int j);
           double BCalculation(int i, int j);
           double CCalculation(int i, int j);
           double DCalculation(int i, int j);
           double ECalculation(int i, int j);
           void FillMatrix(int n);
           void CompactMatrix(int n);
           void bandec(/*double a[][max],*/ int n, int m1, int m2/*, double *a1/*al[][max],int indx[], float dd*/);
           void banbks(/*float a[][max], */int n, int m1, int m2/*, float al[][max], int indx[], float b[]*/);
        double *DeltaX, *DeltaY, *d, *e, *M1, *M2, *M3, *CompactedMatrix, *UpperTriangularMatrix, *LowerTriangularMatrix;
        double dd;
        int *MyIndex;
        int InitialLine, FinalLine, m1, m2;
        CellsRecord *MyCellsRecord;
};

class _export TBiDimensionalVertical : public TOneDimensionalDITransport
{
	public :
		TBiDimensionalVertical(TEcoDynClass* PEcoDynClass , char* className);
       ~TBiDimensionalVertical();
       virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);

		virtual void Go();

      virtual void Inquiry(char* srcName,double &Value,
                                         int BoxNumber,
                                         char* ParameterName,
                                         int AnObjectCode);

        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);
        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
      void BuildBiDimensional();
      virtual void EquationOfMotion();
      virtual void Continuity();
      virtual void VerticalVelocity();
      virtual void SurfaceVolume();
      virtual void AdvectDiffuse(double* Generic);
      virtual void RiverForcing();
      virtual double BarotropicPressureTerm(int Line, int Column);
      virtual double BaroclinicPressureTerm(int Line, int Column);
      virtual double ConvectiveTerm(int Line, int Column);
      virtual double Diffusion(int TermOrder, int Line, int Column);
      virtual double HorizontalDiffusion(int Line, int Column);
      virtual double Drag(int i, int j);
      virtual double WindStress(int ABoxNumber);
      virtual double aUCalculation (int i, int j);
      virtual double bUCalculation (int i, int j);
      virtual double cUCalculation (int i, int j);
      virtual double UPStreamUVelocity(int i, int j);
      virtual double DOWNStreamUVelocity(int i, int j);
      virtual double VerticalMomentumDiffusivity(int i, int j);
      virtual double RichardsonNumber(int i, int j);
      virtual double LengthScale(int i, int j);
      virtual double Depth(int i, int j);
      virtual double VerticalMassAndHeatDiffusivity(int i, int j);
      virtual double CoefRugosidade(int i, int j);
      virtual double ConvectiveTransport(int i, int j, double *PastGeneric);
      virtual double DiffusiveTransport(int i, int j, double *PastGeneric);
      virtual double aDiffusiveTransport (int i, int j);
      virtual double bDiffusiveTransport (int i, int j);
      virtual double cDiffusiveTransport (int i, int j);
      virtual double aTransport (int i, int j);
      virtual double bTransport (int i, int j);
      virtual double cTransport (int i, int j);
      virtual double HorizontalDiffusionCoefficient(int i, int j);
      virtual void Reinitialize();
      virtual double CalculateWidth(int i, int j);
      virtual double CorrectionOfHorizontalDiffCoef(double ADepth, double AVelocityComponent);
      double *UVelocityPast, *WVelocity, *WVelocityPast,
             *UFlowBetweenBoxes,*WFlowBetweenBoxes,
             *HorizontalDiffCoefU, *VerticalDiffCoefU,*VerticalMassAndHeatDiffCoef,
             *VerticalDiffCoefW, *Rugosity, *TotalDepth, *TotalDepthPast, *BoxDepthPast,
             *atemporary, *btemporary, *ctemporary, *rtemporary, *OldBoxDepth, *OldBoxWidth, *DepthVariation;
      double *BaroclinicPressureValue, *BarotropicPressureValue, *ConvectionValue, *DiffusionValue, *MassDiffusion, *MassConvection;
      double *UU1, *UU2, *UU3, *UW; // Apontadores para guardar as componentes da convecção
      double *Soma0,*Soma1, *Soma2, *Soma3, *Soma4, *Soma5, *Soma6, *Soma7, *Soma8, *Soma9, *Soma10, *Soma11;  //Apontadores para guardar resultados temporários por coluna
      double *aColumns, *bColumns, *cColumns, *rColumns;
      double CriticalDepth, CriticalWidth, AUPStreamU,ADOWNStreamU, HMINC, WindStressCoef, MaxVertDiffusivity, TotalHours;
      double *MeanColumnWidth, *MeanColumnLength;
      int GridColumns, GridLines,UpstreamBoundaryNumber,
          DownstreamBoundaryNumber, TopBoundary, BottomBoundary, SemiTimeStep;
      int *SurfaceBox, *SurfaceLayer;

      //double *CaudalPorColunaPassado,*CaudalPorColunaActual, *AlturaEstimada;
};

class _export TTriDimensional : public TBiDimensionalIntertidal
{
	public :
		TTriDimensional(TEcoDynClass* PEcoDynClass, char* className);
        ~TTriDimensional();
        virtual void freeMemory();
      virtual double GetParameterValue(char* parmName);
      virtual bool SetParameterValue(char* parmName, double value);
        virtual void Go();
        virtual void Inquiry(char* srcName,
        			 double &Value,
                     int BoxNumber,
                     char* ParameterName,
                     int AnObjectCode);
        virtual void Reinitialize();
        // AP, 2007.05.28
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName);

        // AP, 2007.07.09
        virtual bool SaveVariables();
        virtual bool SaveParameters();

	protected :
        void BuildTriDimensionalIntertidal();
        virtual void Continuity();
        virtual void EquationOfMotionUImplicit();
        virtual void EquationOfMotionVImplicit();
        virtual void EquationOfMotionUExplicit();
        virtual void EquationOfMotionVExplicit();
        virtual void VerticalVelocity();
        virtual void AdvectDiffuse(double* Generic);
        int Get2DIndex(int i, int j);
        int Get2DIndex(int i, int j, int Columns);
        int Get3DIndex(int i, int j, int k);
        int Get3DIndexForUVelocity(int i, int j, int k);
        int Get3DIndexForVVelocity(int i, int j, int k);
        virtual void PartialSumInitialization();
        double SumOfAjacentDepthsInU(int ALine, int AColumn, int ASecondColumn, int ALayer);
        double SumOfAjacentDepthsInV(int ALine,int ASecondLine,int AColumn,int ALayer);
        double UBoundaryFlow(int ALine, int AColumn, int ASecondColumn, int ALayer);
        double UFlow(int ALine, int AColumn, int ASecondColumn, int ALayer);
        double VBoundaryFlow(int ALine, int ASecondLine, int AColumn, int ALayer);
        double VFlow(int ALine, int ASecondLine, int AColumn, int ALayer);
        virtual double WEBaroclinicPressureTerm(int ALine, int AColumn, int ASecondColumn, int ALayer);
        virtual double NSBaroclinicPressureTerm(int ALine, int ASecondLine, int AColumn, int ALayer);
        virtual double WEBarotropicPressureTerm (int ALine, int AColumn, int ASecondColumn, int ALayer);
        virtual double NSBarotropicPressureTerm (int ALine, int ASecondLine, int AColumn, int ALayer);
        virtual double UConvectiveTerm(int ALine, int AColumn, int ALayer);
        virtual double UPStreamUVelocity(int ALine, int AColumn, int ALayer);
        virtual double DOWNStreamUVelocity(int ALine, int AColumn, int ALayer);
        virtual double UPStreamVVelocity(int ALine, int AColumn, int ALayer);
        virtual double DOWNStreamVVelocity(int ALine, int AColumn, int ALayer);
        virtual double VConvectiveTerm(int ALine, int AColumn, int ALayer);
        virtual double UAdvectionOfUEquation(int ALine, int AColumn, int ALayer);
        virtual double UVAdvectionOfUEquation(int ALine, int AColumn, int ALayer);
        virtual double UWAdvectionOfUEquation(int ALine, int AColumn, int ALayer);
        virtual double VAdvectionOfVEquation(int ALine, int AColumn, int ALayer);
        virtual double UVAdvectionOfVEquation(int ALine, int AColumn, int ALayer);
        virtual double VWAdvectionOfVEquation(int ALine, int AColumn, int ALayer);
        virtual double UpperLeftCornerDepth(int ALine, int AColumn, int ALayer);
        virtual double UpperRightCornerDepth(int ALine, int AColumn, int ALayer);
        virtual double LowerLeftCornerDepth(int ALine, int AColumn, int ALayer);
        virtual double LowerRightCornerDepth(int ALine, int AColumn, int ALayer);
        virtual double UWEHorizontalDiffusion(int ALine, int AColumn, int ALayer);
        virtual double UNSHorizontalDiffusion(int ALine, int AColumn, int ALayer);
        virtual double VWEHorizontalDiffusion(int ALine, int AColumn, int ALayer);
        virtual double VNSHorizontalDiffusion(int ALine, int AColumn, int ALayer);
        virtual double UVerticalDiffusion(int TermOrder, int ALine, int AColumn, int ALayer);
        virtual double UVerticalDiffusionExplicit(int ALine, int AColumn, int ALayer);
        virtual double VVerticalDiffusion(int TermOrder, int ALine, int AColumn, int ALayer);
        virtual double VVerticalDiffusionExplicit(int ALine, int AColumn, int ALayer);
        virtual double DragOfUEquation(int ALine, int AColumn, int ALayer);
        virtual double DragOfVEquation(int ALine, int AColumn, int ALayer);
        virtual double UForRugosity(int ALine, int AColumn, int ALayer);
        virtual double VForRugosity(int ALine, int AColumn, int ALayer);
        virtual double CoefRugosidade(int ALine, int AColumn, int ALayer);
        virtual double Depth(int ALine, int AColumn, int ALayer);
        virtual double UImplicitForDrag(int ALine, int AColumn, int ALayer);
        virtual double VImplicitForDrag(int ALine, int AColumn, int ALayer);
        virtual double aUCalculation (int ALine, int AColumn, int ALayer);
        virtual double bUCalculation (int ALine, int AColumn, int ALayer);
        virtual double cUCalculation (int ALine, int AColumn, int ALayer);
        virtual double aVCalculation (int ALine, int AColumn, int ALayer);
        virtual double bVCalculation (int ALine, int AColumn, int ALayer);
        virtual double cVCalculation (int ALine, int AColumn, int ALayer);
        virtual double WindStress(int ABoxNumber, char* ParameterName);
        virtual double UpperUVelocity (int ALine, int AColumn, int ALayer);
        virtual double LowerUVelocity (int ALine, int AColumn, int ALayer);
        virtual double UpperVVelocity (int ALine, int AColumn, int ALayer);
        virtual double LowerVVelocity (int ALine, int AColumn, int ALayer);
        virtual double LeftVVelocity (int ALine, int AColumn, int ALayer);
        virtual double RightVVelocity (int ALine, int AColumn, int ALayer);
        virtual double VerticalMomentumDiffusivity(int i, int j, int k, double AVelocity, double ALowerVelocity, int VelocityComponent /*1 para a U, 2 para a V*/);
        virtual double LengthScale(int ALine, int AColumn, int ALayer, double AVelocity, double ALowerVelocity, int VelocityComponent);
        virtual double RichardsonNumber(int ALine, int AColumn, int ALayer, double AVelocity, double ALowerVelocity, int VelocityComponent);
        virtual double SaltWEHorizontalDiffusion(int i, int j, int k, double* PastGeneric);
        virtual double SaltNSHorizontalDiffusion(int i, int j, int k, double* PastGeneric);
        bool CheckForSeaBoundary(int ALine, int AColumn, int ALayer);
        bool CheckForSeaBoundariesInColumns(int ALine, int AColumn);
        bool CheckForRiverBoundariesInColumns(int ALine, int AColumn);
        bool CheckForRivers(int ALine, int AColumn);
        virtual void TideForcing();
        virtual void RiverForcing();
        virtual void DischargeForcing();
        virtual void UptakeForcing();
        virtual double getUVelocity3D(int ABoxNumber);
        virtual double getVVelocity3D(int ABoxNumber);
        virtual double getWVelocity3D(int ABoxNumber);
        virtual double getUFlow(int ABoxNumber);
        virtual double getVFlow(int ABoxNumber);
        virtual double VContributionToContinuity(int ALine, int AColumn, int ALayer); //Calculam a contribuição das componentes v através das paredes Sul e Norte de cada célula na Continuidade
        virtual double UContributionToContinuity(int ALine, int AColumn, int ALayer); //Calculam a contribuição das componentes u através das paredes Oeste e Este de cada célula na Continuidade
        virtual double VCoriolis(int i, int j, int k);
        virtual double UCoriolis(int i, int j, int k);
        virtual void UpdateOfTidalBoundariesVelocities();
        virtual void TidalHeightBoundaries();
        virtual void TidalHeightAndVelocityBoundaries();
        virtual void UpdateMeanSamples();
        int GridLayers;
        int *SurfaceBox, *SurfaceLayer;
        double *DepthVariation;
        double *Soma0,*Soma1, *Soma2, *Soma3, *Soma4, *Soma5, *Soma6, *Soma7, *Soma8, *Soma9, *Soma10, *Soma11, *ASum1, *ASum2;  //Apontadores para guardar resultados temporários por coluna
        double *UU1, *UU2, *UU3, *UU4, *UW, *UVU, *VV1, *VV2, *VV3, *VV4, *VW, *UVV; // Apontadores para guardar as componentes da convecção
        double *MeanColumnWidth, *MeanColumnLength;
        //double *aColumns, *bColumns, *cColumns, *rColumns;
        double *atemporaryCont, *btemporaryCont, *ctemporaryCont, *rtemporaryCont;
        //double *aLines, *bLines, *cLines, *rLines;
        double *WVelocity, *WVelocityPast, *BarotropicPressureValue, *BaroclinicPressureValue,
               *ConvectionValue,*DiffusionValue;
        double *VerticalDiffCoefU, *VerticalDiffCoefV, *Rugosity, *Pressure;
        double HMINC, WindStressCoef, WindEffect, MaxVertDiffusivity, delta;
        void Debugger(double bug);
};

class _export TTriDimensional2 : public TTriDimensional
{
	public :
		TTriDimensional2(TEcoDynClass* PEcoDynClass, char* className);
        ~TTriDimensional2();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Reinitialize();
    protected :
        virtual void Continuity();
        virtual void AdvectDiffuse(double* Generic);
        double DragOfUEquation(int ALine, int AColumn, int ALayer);
        double CoefRugosidade(int ALine, int AColumn, int ALayer, int AComponent);
        double HydraulicRadius(int ALine, int AColumn, int ALayer, int AComponent);
        double UImplicitForDrag(int ALine, int AColumn, int ALayer);
        double DragOfVEquation(int ALine, int AColumn, int ALayer);
        double VImplicitForDrag(int ALine, int AColumn, int ALayer);
        virtual void EquationOfMotionUImplicit();
        virtual void EquationOfMotionVImplicit();
        virtual void EquationOfMotionUExplicit();
        virtual void EquationOfMotionVExplicit();
        virtual void CheckSurfaceLayer();
        virtual void RemoveLayer();
        virtual void AddLayer();
        virtual void TideForcing();
        double bUCalculation(int ALine, int AColumn, int ALayer);
        double bVCalculation(int ALine, int AColumn, int ALayer);
        bool RemovingLayer, AddingLayer;
};


class _export TTriDimensionalDynamicSurfaceLayer : public TTriDimensional
{
	public :
		TTriDimensionalDynamicSurfaceLayer(TEcoDynClass* PEcoDynClass, char* className);
        ~TTriDimensionalDynamicSurfaceLayer();
        virtual void Inquiry(char* srcName, double &Value,int BoxNumber,char* ParameterName,int AnObjectCode);
        virtual void Reinitialize();
    protected :
        virtual void AdvectDiffuse(double* Generic);
        double DragOfUEquation(int ALine, int AColumn, int ALayer);
        double CoefRugosidade(int ALine, int AColumn, int ALayer, int AComponent);
        double HydraulicRadius(int ALine, int AColumn, int ALayer, int AComponent);
        double UImplicitForDrag(int ALine, int AColumn, int ALayer);
        double DragOfVEquation(int ALine, int AColumn, int ALayer);
        double VImplicitForDrag(int ALine, int AColumn, int ALayer);
        virtual void CheckSurfaceLayer();
        virtual void RemoveLayer();
        virtual void AddLayer();
        virtual void TideForcing();
        double bUCalculation(int ALine, int AColumn, int ALayer);
        double bVCalculation(int ALine, int AColumn, int ALayer);
        bool RemovingLayer, AddingLayer;
};

class _export TTriDimensionalWithBodyDrag : public TTriDimensionalDynamicSurfaceLayer
{
	public :
		TTriDimensionalWithBodyDrag(TEcoDynClass* PEcoDynClass, char* className);
        ~TTriDimensionalWithBodyDrag();
    protected :
        virtual void Continuity();
        virtual void EquationOfMotionUImplicit();
        virtual void EquationOfMotionVImplicit();
        virtual void EquationOfMotionUExplicit();
        virtual void EquationOfMotionVExplicit();
        virtual void PartialSumInitialization();
        double UImplicitForDrag(int ALine, int AColumn, int ALayer);
        double VImplicitForDrag(int ALine, int AColumn, int ALayer);
        double BodyDragOfUEquation(int ALine, int AColumn, int ALayer);
        double BodyDragOfVEquation(int ALine, int AColumn, int ALayer);
        double *Cd, *BodyDiameter, *BodyHeight, *ASum3, *ASum4;
};

class _export TTriDimensionalWithBodyDrag2 : public TTriDimensionalWithBodyDrag
{
    public :
        TTriDimensionalWithBodyDrag2(TEcoDynClass* PEcoDynClass, char* className);
        ~TTriDimensionalWithBodyDrag2();
    protected :
        virtual void AdvectDiffuse(double* Generic);
        double *FUPw,*FLWw,*TDVw;
        double ReturnR(int s, double FLMMinusOneHalf, double FLMPlusOneHalf, double FLMPlusThreeHalves);
        double Superbee(double r);
};

class _export TTriDimensionalSymbioses : public TTriDimensionalWithBodyDrag2
{
    public :
        TTriDimensionalSymbioses(TEcoDynClass* PEcoDynClass, char* className);
        ~TTriDimensionalSymbioses();
        virtual void Go();
    protected:
        virtual void Continuity();
    private:
        int *griddims;
        float *gridlats;
        float *gridlons;
        HydrodynamicModel *ocean;
        AtmosphericModel *atmo;


        void ReadVariablesFromSymbioses();
};



static TEcoDynClass *PTransport;


#endif //HYDROBJT_H



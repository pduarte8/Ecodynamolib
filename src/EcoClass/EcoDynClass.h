#ifndef _ECODYNCLASS_H_
#define _ECODYNCLASS_H_

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifdef __BORLANDC__
	#include <vcl.h>
	#include <syncobjs.hpp>
	#include <stdio.h>
	#include <string.h>
	#include <time.h>
#else  // __BORLANDC__
	#include <cstring>
	#include <cstdlib>
	#include <cstdio>
	#include <ctime>
	
	using namespace std;

	#define _export 

#endif  // __BORLANDC__

#include "globals.h"
#include "EcoDynProtocol.h"

/** @mainpage Base class for all objects used by EcoDynamo simulator 
 *
 */
typedef struct seedRegion {
    char regName[ECDP_STRING];
    BIVALVE_S bivalve_s;
    double density;
} SEED_REGION;

typedef struct seedDB {
    RegionList* regionList;
    long firstDay;
    long lastDay;
    Queue* seedRegions; // Queue with SEED_REGION elements
    bool seeding;
    bool error;
} SEED_DB;

typedef struct harvestRegion {
    MSG_ID msg_id;
    char regName[ECDP_STRING];
    BIVALVE bivalve;
    double weightHarvested;
} HARVEST_REGION;

typedef struct harvestDB {
    RegionList* regionList;
    EcoDynProtocol* ecdp;
    long firstDay;
    long lastDay;       // = firstDay [first release]
    Queue* harvestRegions; // Queue with HARVEST_REGION elements
    bool harvesting;
} HARVEST_DB;

typedef struct inspectDB {
    RegionList* regionList;
    EcoDynProtocol* ecdp;
    MSG_ID msg_id;
    char regName[ECDP_STRING];
    long day;
} INSPECT_DB;
// ... AP, 2006.06.30

class _export TEcoDynClass
{
    public :
        // Display record definition for variables
        typedef struct Variables            // List of chosen variables
        {
            char VariableName[41];
            char Units[41];
            int  ObjectCode;
            double Low, High;
            TEcoDynClass* ObjectPointer;
            bool IsStateVariable,
                 IsSelected;
        } VariableRecord;

        // Display record definition
        typedef struct DisplayBoxes         // List of chosen boxes
        {
            char BoxName[41];
            int BoxNumber;
            bool IsSelected;
        } DisplayBoxRecord;

        typedef struct AdvectDiffuse
        {
            double AdvectiveFlow,
                   DispersiveFlow,
                   InFlow;
            bool HasConnection;
        } AdvectionDiffusionRecord;

        typedef struct {
            char ClassName[100];
            int nrVariablesAvailable;
            char AvailableVariables[500][256];
            int nrVariablesSelectedFileOutput;
            char SelectedVariablesFileOutput[500][256];
            int nrVariablesSelectedGraphOutput;
            char SelectedVariablesGraphOutput[500][256];
            int nrVariablesSelectedTableOutput;
            char SelectedVariablesTableOutput[500][256];
            double Value;
            TEcoDynClass* ObjectPointer;
            int nrIndividuals;
            int dimVariables;
            VNA* pVarNameArray;
            int nTimeSeries;
            VNA* pTimeSeriesArray;
            //Ice specifics
            int nrIceLayers;
            int nrIceVariables;
            int nrIceBoxes;
            VNA* pIceVarNameArray;
            //end Ice specifics
        } VarsRecord;

        //Loads and rivers
        LoadsRecord* ALoadRecord;
        LoadsRecord* GetALoadRecord();
        LossesRecord* ALossesRecord;
        LossesRecord* GetALossesRecord();
        RiversRecord* ARiverRecord;
        RiversRecord* GetARiverRecord();
        SeaBoundariesRecord* ASeaBoundariesRecord;
        SeaBoundariesRecord* GetASeaBoundariesRecord();
        SedimentsRecord* ASedimentsRecord;
        SedimentsRecord* GetASedimentsRecord();
        IceRecord* AIceRecord;
        IceRecord* GetAIceRecord();
        BenthicSpeciesRecord* ABenthicSpeciesRecord;
        bool BenthicSpeciesRecordAllocated;
        BenthicSpeciesRecord* GetABenthicSpeciesRecord();
        bool HasALoadRecord();
        bool HasALossesRecord();
        bool HasARiverRecord();
        bool HasASedimentsRecord();
        bool HasAIceRecord();
        bool HasABenthicSpeciesRecord();
        bool HasASeaBoundariesRecord();
        bool LoadsExist, RiversExist, SedimentsExist, BenthicSpeciesExist, 
			LossesExist, SeaBoundariesExist, IceExist;
        int NumberOfLoads, NumberOfDaysForLoads, NumberOfHoursForLoads, NumberOfRivers,
            NumberOfDaysForFlowTimeSeries, NumberOfHoursForFlowTimeSeries, 
            NumberOfCellsWithSediments, NumberOfCellsWithBenthicSpecies,
            NumberOfLosses, NumberOfDaysForLosses, NumberOfHoursForLosses,
            NumberOfSeaBoundaries, NumberOfDaysForSeaBoundaries, 
            NumberOfHoursForSeaBoundaries, NumberOfCellsWithIce, IceGrid;

        TEcoDynClass();

#ifndef _PORT_FORTRAN_
        TEcoDynClass(float timeStep);
        TEcoDynClass(float timeStep, char* APathName, char *AEcosystemName);
        TEcoDynClass(float timeStep, int nLines, int nColumns, double aDepth[],
                   double aLength[], double aWidth[], double aElevation[], int aType[],
                   int aNBoundary[], int aEBoundary[], int aSBoundary[], int aWBoundary[]);
        void Debugger(int i);    //overloaded debugger
        void Debugger(long i);   //overloaded debugger
        void Debugger(float i);  //overloaded debugger
        void Debugger(double i);  //overloaded debugger
#endif
        virtual ~TEcoDynClass();

        // return functions
        void Calendar();
        double sqr(double ANumber) {return ANumber * ANumber;}
        double ten(double power);
        double exponent(double mantissa, double power);
        double GetDensity(double ASalinity,
                          double ATemperature);
        double GetDensity(double ASalinity /*psu*/,double ATemperature, double AAtmosphericPressure/*bars*/);
        double GetViscosity(double salinity,
                            double temperature);
        double GetGravity(double DegreesMinutes,
                          double heightAboveSeaLevel);
        void CalculateNumberOfObjects();
        int GetActiveObjects() {return ActiveObjects;}
        int GetActiveVariables() {return ActiveStateVariables;}
        float GetTimeStep() {return TimeStep;}
        void SetTimeStep(float tStep);
        float GetRunTime() {return MyRunTime;}
        float GetTimeBefore() {return TimeBefore;}
        void SetTime(long ATime);
        void SetMyTime(long ATime);
        float GetCurrTime() {return CurrentTime;}
        float GetTime() {return MyTime;}
        float GetCurrentTime() {return CurrentTime;}
        int GetDate() {return Date;}
        int GetJulianDay() {return JulianDay;}
        int GetMonth() {return Month;}
        int GetYear() {return Year;}
        int GetNumberOfLoads(){return NumberOfLoads;}
        int GetNumberOfDaysForLoads(){return NumberOfDaysForLoads;}
        int GetNumberOfHoursForLoads(){return NumberOfHoursForLoads;}
        int GetNumberOfLosses(){return NumberOfLosses;}
        int GetNumberOfDaysForLosses(){return NumberOfDaysForLosses;}
        int GetNumberOfHoursForLosses(){return NumberOfHoursForLosses;}
        int GetNumberOfRivers(){return NumberOfRivers;}
        int GetNumberOfDaysForFlowTimeSeries(){return NumberOfDaysForFlowTimeSeries;}
        int GetNumberOfHoursForFlowTimeSeries(){return NumberOfHoursForFlowTimeSeries;}
        int GetNumberOfDaysForSeaBoundariesTimeSeries(){return NumberOfDaysForSeaBoundaries;}
        int GetNumberOfHoursForSeaBoundariesTimeSeries(){return NumberOfHoursForSeaBoundaries;}
        int GetNumberOfSeaBoundaries(){return NumberOfSeaBoundaries;}
        double GetLoadFlows(int ALoad, int ATime);
        int GetLoadLines(int ALoad);
        int GetLoadColumns(int ALoad);
        
        double GetLossesFlows(int ALoss, int ATime);
        int GetLossesLines(int ALoss);
        int GetLossesColumns(int ALoss);

        int GetNumberOfCellsWithSediments(){return NumberOfCellsWithSediments;}
        int GetNumberOfCellsWithBenthicSpecies(){return NumberOfCellsWithBenthicSpecies;}
        int GetNumberOfCellsWithIce(){return NumberOfCellsWithIce;}
        //LoadsRecord GetLoadRecord(){return ALoadRecord);

        // AP, 2005.07.29
        void SetStartTime(long ATime);
        long GetStartTime();
        virtual void ResetValues() {}

        long GetTimeUTC() {return TimeUTC;}
        double GetBoxArea(int ABoxNumber)
        {
          return this->BoxArray[ABoxNumber].Width
            * this->BoxArray[ABoxNumber].Length;
        }
        void GetBoxTopAndBottom(int ABoxNumber,
                                double &ATop,
                                double &ABottom);
        int GetAYear(long ATime);
        int GetAMonth(long ATime);
        int GetADate(long ATime);
        int GetAJulianDay(long ATime);      // AP, 2005.11.11
        float GetAnHour(long ATime);
        long GetACurrentTime(long ATime);

        void SetJulianDay(int day) {JulianDay = day;}
        void SetCurrentTime(float time) {CurrentTime = time;}
        
        // pointer attribution functions - one per object

        // Hydrodynamics
        void SetTransportPointer(TEcoDynClass *ATP) {MyTransportPointer = ATP;};
        TEcoDynClass* GetTransportPointer() {return MyTransportPointer;};
        // Nutrients
        void SetNutrientPointer(TEcoDynClass *ANP) {MyNutrientPointer = ANP;};
        TEcoDynClass* GetNutrientPointer() {return MyNutrientPointer;};
        // SPM
        void SetSPMPointer(TEcoDynClass *ASPMP) {MySPMPointer = ASPMP;};
        TEcoDynClass* GetSPMPointer() {return MySPMPointer;};
        // Phytoplankton
        void SetPhytoplanktonPointer(TEcoDynClass *APP) {MyPhytoplanktonPointer = APP;};
        TEcoDynClass* GetPhytoplanktonPointer() {return MyPhytoplanktonPointer;};
        // Zooplankton
        void SetZooplanktonPointer(TEcoDynClass *AZP) {MyZooplanktonPointer = AZP;};
        TEcoDynClass* GetZooplanktonPointer() {return MyZooplanktonPointer;};
        // Salt-marsh
        void SetSaltMarshPointer(TEcoDynClass *ASMP) {MySaltMarshPointer = ASMP;};
        TEcoDynClass* GetSaltMarshPointer() {return NULL /* MySaltMarshPointer temporary */;};
        // Zoobenthos
        void SetZoobenthosPointer(TEcoDynClass *AZBP) {MyZoobenthosPointer = AZBP;};
        TEcoDynClass* GetZoobenthosPointer() {return MyZoobenthosPointer;};

        // Zoobenthos2
        void SetZoobenthosPointer2(TEcoDynClass *AZBP) {MyZoobenthosPointer2 = AZBP;};
        TEcoDynClass* GetZoobenthosPointer2() {return MyZoobenthosPointer2;};

        // Nekton
        void SetNektonPointer(TEcoDynClass *ANKP) {MyNektonPointer = ANKP;};
        TEcoDynClass* GetNektonPointer() {return MyNektonPointer;};
        // Man
        void SetManPointer(TEcoDynClass *AMP) {MyManPointer = AMP;};
        TEcoDynClass* GetManPointer() {return MyManPointer;};
        // Farmer
        void SetFisherPointer(TEcoDynClass *AMP) {MyFisherPointer = AMP;};
        TEcoDynClass* GetFisherPointer() {return MyFisherPointer;};
        // Xenobiotics
        //void SetXenobioticPointer(TIntegrate *AXP) { MyXenobioticPointer = AXP;};
        //TIntegrate* GetXenobioticPointer() { return MyXenobioticPointer;};
        // Macrophyte
        void SetMacrophytePointer(TEcoDynClass *AMacroP) { MyMacrophytePointer = AMacroP;};
        TEcoDynClass* GetMacrophytePointer() { return MyMacrophytePointer;};
        // Microphytobenthos
        void SetMicrophytobenthosPointer(TEcoDynClass *AMicroP) { MyMicrophytobenthosPointer = AMicroP;};
        TEcoDynClass* GetMicrophytobenthosPointer() { return MyMicrophytobenthosPointer;};
        // Light
        void SetLightPointer(TEcoDynClass *ALP) { MyLightPointer = ALP;};
        TEcoDynClass* GetLightPointer() {return MyLightPointer;};
        // Air temperature
        void SetAirTemperaturePointer(TEcoDynClass *AATP) { MyAirTemperaturePointer = AATP;};
        TEcoDynClass* GetAirTemperaturePointer() {return MyAirTemperaturePointer;};
        // Water temperature
        void SetWaterTemperaturePointer(TEcoDynClass *AWTP) { MyWaterTemperaturePointer = AWTP;};
        TEcoDynClass* GetWaterTemperaturePointer() {return MyWaterTemperaturePointer;};
        // Wind
        void SetWindPointer(TEcoDynClass *AWP) { MyWindPointer = AWP;};
        TEcoDynClass* GetWindPointer() {return MyWindPointer;};
        // Flow
        void SetFlowPointer(TEcoDynClass *AFP) { MyFlowPointer = AFP;};
        TEcoDynClass* GetFlowPointer() {return MyFlowPointer;};
        // Tide
        void SetTidePointer(TEcoDynClass *ATIP) { MyTidePointer = ATIP;};
        TEcoDynClass* GetTidePointer() {return MyTidePointer;};
        //Sediments
        void SetSedimentPointer(TEcoDynClass *ASEDP) { MySedimentPointer = ASEDP;};
        TEcoDynClass* GetSedimentPointer() {return MySedimentPointer;};

        void SetPreyPointer(TEcoDynClass *APreyP) { MyPreyPointer = APreyP;};
        TEcoDynClass* GetPreyPointer() {return MyPreyPointer;};

        void SetPredatorPointer(TEcoDynClass *APredatorP) { MyPredatorPointer = APredatorP;};
        TEcoDynClass* GetPredatorPointer() {return MyPredatorPointer;};

        void SetFishPointer(TEcoDynClass *AFishP) { MyFishPointer = AFishP;};
        TEcoDynClass* GetFishPointer() {return MyFishPointer;};

        // Bacteria
        void SetBacteriaPointer(TEcoDynClass *ABP) { MyBacteriaPointer = ABP;};
        TEcoDynClass* GetBacteriaPointer() { return MyBacteriaPointer;};

        // Sea ice
        void SetSeaIcePointer(TEcoDynClass *ASIP) { MySeaIcePointer = ASIP;};
        TEcoDynClass* GetSeaIcePointer() { return MySeaIcePointer;};

        // Ice algae
        void SetIceAlgaePointer(TEcoDynClass *AIAP) { MyIceAlgaePointer = AIAP;};
        TEcoDynClass* GetIceAlgaePointer() { return MyIceAlgaePointer;};
        //
        // Methods called from EcoDynamo when closing model:
        // this methods must be ovewritten in each class that manipulates
        // variables and/or parameters
        virtual bool SaveVariables() {return true;}
        virtual bool SaveParameters() {return true;}
        virtual void SaveTide(void* fHandle) {;}

        // child functions
        int GetNumberOfVariables() {return NumberOfVariables;}
        int GetObjectCode() {return ObjectCode;}

        // For display dialogs
        // Variables
        virtual void SetStateVariableArray(VariableRecord*, int AVarRecSize);
        VariableRecord* GetStateVariableArray() {return MyVarRec;}
        int GetNumberOfStateVariables() {return MyVarRecSize;}
        void SetNumberOfSelectedVariables(int ANSV) {NumberOfSelectedVariables = ANSV;}
        int GetNumberOfSelectedVariables() {return NumberOfSelectedVariables;}
        // Display boxes
        virtual void SetDisplayBoxArray(DisplayBoxRecord*, int ABoxRecSize);
        int GetNumberOfDisplayBoxes() {return MyDisplayBoxSize;}
        DisplayBoxRecord* GetDisplayBoxArray() {return MyDisplayBoxRec;}
        // From save period dialog
        void SetSaveStart(int AStartSaveDay) {StartSaveDay = AStartSaveDay;}
        void SetSaveEnd(int AEndSaveDay) {EndSaveDay = AEndSaveDay;}
        int GetSaveStart() {return StartSaveDay;}
        int GetSaveEnd() {return EndSaveDay;}
        // From output dialog
        void SetOutputFrequency(float AFrequency) {OutputFrequency = AFrequency;}
        float GetOutputFrequency() {return OutputFrequency;}

        // All of these are abstract member functions for this
        // base class except integrate
        virtual void Go(){};
        virtual void Go(double* AGeneric,
                        double* AGenericLoad,
                        double ARiver,
                        double AOcean){};
        virtual void Go(double* AGeneric,
                        double* AGenericLoad,
                        double* AGenericExchange,
                        double ARiver,
                        double AOcean){};
        // AP, 2005.05.17
        virtual void Go(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad,
                        double ARiver, double AOcean) {};
        // Pedro Duarte, 2005.07.07
        virtual void Go(double *A, double *ALoad, double *ARiverLoad,
                        double *B, double *BLoad, double *BRiverLoad,
                        double *C, double *CLoad, double *CRiverLoad,
                        double *D, double *DLoad, double *DRiverLoad,
                        double *E, double *ELoad, double *ERiverLoad,
                        double ARiver, double AOcean) {};
        // Pedro Duarte, 2005.06.20
        virtual void Go(double *A, double *ALoad,
                        double *B, double *BLoad,
                        double *C, double *CLoad,
                        double *D, double *DLoad,
                        double *E, double *ELoad,
                        double *F, double *FLoad,
                        double *G, double *GLoad,
                        double *H, double *HLoad,
                        double ARiver, double AOcean) {};

        virtual void Integrate() {EnableReinitializeFromIntegrate = true;}
        virtual void Inquiry(char* srcName, double &Value,
                                 int BoxNumber,
                                 char* ParameterName,
                                 int AnObjectCode){};
        virtual void Inquiry(char* srcName, double &Value,
                                 double Value1,
                                 int BoxNumber,
                                 char* ParameterName,
                                 int AnObjectCode){};
        virtual void Inquiry(char* srcName, double &Value,
                                 int BoxX,
                                 int BoxY,
                                 char* ParameterName,
                                 int AnObjectCode){};
        virtual void Inquiry(char* srcName, double &Value,
                                 int BoxNumber,
                                 double ExtraValue,
                                 int AnObjectCode){};
        virtual void Inquiry(char* srcName, double &Value1,
                                 double Value2,
                                 double Value3,
                                 double Value4,
                                 int BoxNumber,
                                 char* ParameterName,
                                 int AnObjectCode){};

        virtual void Inquiry(char* srcName, double &Value1,
                                 double Value2,
                                 double Value3,
                                 double Value4,
                                 double Value5){};

        virtual void Inquiry(char* srcName, double &Value1,
                                 double &Value2,
                                 double Value3,
                                 double Value4,
                                 double Value5,
                                 double Value6){};

        virtual void Inquiry(char* srcName, double &Value1,
                                 double Value2,
                                 double Value3,
                                 double Value4,
                                 double Value5,
                                 int AInteger){};

        virtual void Inquiry(char* srcName, int column,
                                 int line,
                                 double &Value,
                                 char* ParameterName,
                                 int AnObjectCode){};

        virtual VNA* Inquiry(char* srcName,
                             int &Individuals, int &DimVariables)
        {
            Individuals = 0; DimVariables = 0;
            return NULL;
        };

        virtual VNA* Inquiry(char* srcName, int &TimeSeriesVariables)
        {
            TimeSeriesVariables = 0;
            return NULL;
        };

        virtual VNA* InquiryIce(char* srcName,int &IceLayers, int &IceVariables)
        {
            IceLayers = 0; IceVariables = 0;
            return NULL;
        };


        virtual void Update(char* srcName, double Value,
                             int BoxNumber,
                             char* ParameterName,
                             int AnObjectCode){};
        virtual void Update(char* srcName, double Value,
                             double ExtraValue,
                             int BoxNumber,
                             char* ParameterName,
                             int AnObjectCode){};
        virtual void Update(char* srcName, double Value,
                             double ExtraValue,
                             int BoxNumber,
                             int ClassNumber,
                             int AnObjectCode){};
        virtual void Update(char* srcName, double Value,
                             double ExtraValue,
                             int BoxNumber,
                             int AnObjectCode){};
        virtual void Update(char* srcName, double ZooDensityInput,  // ind / m2 / day
                             int BoxNumber,
                             int ClassNumber,
                             int AnObjectCode){};

        // AP, 2006.05.24
        virtual bool SetVariableValue(char* srcName,
                            double Value,
                             int BoxNumber,
                             char* VariableName){return true;}

        // AP, 2005.07.11
        virtual void freeMemory() {};

        // Convert these to Go functions
        virtual void AdvectDiffuseWaterTemperature(){};
        virtual void EnergyAcrossSurface(double*,
                                         int BoxNumber,
                                         char* ParameterName){};
        virtual void EnergyAcrossSurface(double*,
                                         char* ParameterName){};

        virtual void Reinitialize() {};
        virtual void ClearMeanSamples() {};
        virtual void LogMessage(char* type, char* srcName, char* ParameterName,
                double Value, int BoxNumber);
        virtual void DebugMessage(char* text);  // AP, 2005.07.13

        int GetNumberOfVariablesInObject() {return NumberOfVariables;}
        char* GetAvailableVariables(int i);

        int GetNumberOfParametersInObject() {return NumberOfParameters;}
        char* GetAvailableParameters(int i);

        virtual double GetParameterValue(char* parmName) {return 0.0;}
        virtual bool SetParameterValue(char* parmName, double value) {return true;}

        virtual void UpdateLayerProperties(int Line, int Column, int ALayer, double *Concentration){}
        virtual void UpdateLayerPropertiesForPhytoplankton(int Line, int Column, int Class, double *Concentration){}
        virtual void SnowMeltTransport(int Line, int Column, double *Generic, double SnowConcentration){}

        //  24/3/2003
        bool OpenMorphology();
        bool OpenMorphology(char* FileName);

        bool OpenLoads();
        bool OpenLoads(char* FileName);

        bool OpenLosses();
        bool OpenLosses(char* FileName);

        bool OpenRivers();
        bool OpenRivers(char* FileName);

        bool OpenSeaBoundaries();
        bool OpenSeaBoundaries(char* FileName);

        bool OpenSediments();
        bool OpenSediments(char* FileName);

        bool OpenBenthicSpecies();
        bool OpenBenthicSpecies(char* FileName);

        bool OpenIce();
        bool OpenIce(char* FileName);

        void SetBoxDepth(int ABoxNumber, double depth);
        double GetBoxDepth(int ABoxNumber);
        double GetBoxLength(int ABoxNumber);
        double GetChannelWidth(int ABoxNumber);
        double GetBoxElevation(int ABoxNumber);
        double GetBoxVolume(int ABoxNumber);
        int GetBoxType(int ABoxNumber);
        int GetNorthernBoundary(int ABoxNumber);
        int GetEasternBoundary(int ABoxNumber);
        int GetSouthernBoundary(int ABoxNumber);
        int GetWesternBoundary(int ABoxNumber);
        double GetBoxLatitude(int ABoxNumber);
        double GetBoxLongitude(int ABoxNumber);
        int GetGridSize() {return NumberOfBoxes;}
        int GetIceGridSize() {return IceGrid;}
        int GetNumberOfLines() {return NumberOfLines;}
        int GetNumberOfColumns() {return NumberOfColumns;}
        int GetNumberOfLayers() {return NumberOfLayers;}

        int GetBoxNumber(int columnNr, int lineNr)
            {return (NumberOfLines - lineNr) * NumberOfColumns + (columnNr - 1);}
        int GetBoxNumber(int columnNr, int lineNr, int layerNr)
            {return (NumberOfLines - lineNr) * NumberOfColumns + (columnNr - 1) + (NumberOfLayers - layerNr) * NumberOfLines * NumberOfColumns;}
        int GetBoxNumber(int columnNr, int lineNr, int layerNr, int NumberOfLayers)
            {return (NumberOfLines - lineNr) * NumberOfColumns + (columnNr - 1) + (NumberOfLayers - layerNr) * NumberOfLines * NumberOfColumns;}
        int GetColumnNumber(int boxNumber)
            {return (boxNumber % NumberOfColumns) + 1;}
        int GetLineNumber(int boxNumber)
            {return NumberOfLines - (boxNumber / NumberOfColumns);}

        int GetBoxIndex(int columnIx, int lineIx)
            {return lineIx * NumberOfColumns + columnIx;}
        int GetColumnIndex(int boxNumber)
            {return boxNumber % NumberOfColumns;}
        int GetLineIndex(int boxNumber)
            {return boxNumber / NumberOfColumns;}
        int GetLayerIndex(int boxNumber)
            {return boxNumber / (GetNumberOfLines() * GetNumberOfColumns());}  //PDuarte - 30/8/2006
        int GetLayerNumber(int boxNumber)
            {return GetNumberOfLayers() - GetLayerIndex(boxNumber);}  //APereira - 28/6/2007
        int GetLineIndex3D(int boxNumber)
            {return (boxNumber - GetLayerIndex(boxNumber) * GetNumberOfLines() * GetNumberOfColumns()) / GetNumberOfColumns();}  //PDuarte - 30/8/20006
        int GetColumnIndex3D(int boxNumber)
            {return (boxNumber - GetLayerIndex(boxNumber) * GetNumberOfLines() * GetNumberOfColumns()) % GetNumberOfColumns();}
        int GetLineIndex3D(int boxNumber, int aLayer)
            {return (boxNumber - aLayer * GetNumberOfLines() * GetNumberOfColumns()) / GetNumberOfColumns();}
        int GetColumnIndex3D(int boxNumber, int aLayer)
            {return (boxNumber - aLayer * GetNumberOfLines() * GetNumberOfColumns()) % GetNumberOfColumns();}
        int Get3DIndex(int LineNr,int ColumnNr,int LayerNr)
            {return (LineNr * NumberOfColumns + ColumnNr) + LayerNr * (NumberOfLines * NumberOfColumns);}

        // PDuarte - 30/8/2006
        int GetLineNumber3D(int boxNumber)
            {return NumberOfLines - (boxNumber - GetLayerIndex(boxNumber)* GetNumberOfLines() * GetNumberOfColumns()) / GetNumberOfColumns();}
        int GetColumnNumber3D(int boxNumber)
            {return (boxNumber - GetLayerIndex(boxNumber) * GetNumberOfLines() * GetNumberOfColumns()) % GetNumberOfColumns() + 1;}
        int GetIceIndex(int Line, int Column, int Layer, int IceClass)
            {
               return (Line * GetNumberOfColumns() + Column) + Layer * (GetNumberOfLines() * GetNumberOfColumns()) +
                     IceClass * (GetNumberOfLines() * GetNumberOfColumns() * GetNumberOfIceLayers());
            }
        int GetNumberOfIceLayers() {return NumberOfIceLayers;}  
        long GetAStartTime()
            {return AStartTime;}
        long GetAFinishTime()
            {return AFinishTime;}
        // AP, 2007.04.24
        char* GetModelType() {return modelType;}

        char* GetPathName() {return MyPathName;}
        void SetPathName(char* APathName) {strcpy(MyPathName, APathName);}
        char* GetMorphologyFile() {return MorphologyFileName;}
        char* GetTideFile() {return TideFileName;}
        char* GetClassesFile() {return ClassesFileName;}
        char* GetEcoDynClassName() {return EcoDynClassName;}
        char* GetLoadsFile() {return LoadsFileName;}
        char* GetLossesFile() {return LossesFileName;}
        char* GetRiversFile() {return RiversFileName;}
        char* GetSeaBoundariesFile() {return SeaBoundariesFileName;}
        char* GetSedimentsFile() {return SedimentsFileName;}
        char* GetBenthicSpeciesFile() {return BenthicSpeciesFileName;}
        char* GetIceFile() {return IceFileName;}
        //PDuarte - 9/05/2013
        bool LatitudeBoolean, LongitudeBoolean;
        bool GetLatitudeBoolean()
          {return LatitudeBoolean;}
        bool GetLongitudeBoolean()
          {return LongitudeBoolean;}

        TBoxes::BoxRecord* BoxArray;
        bool BoxArrayAllocated;

#ifdef __BORLANDC__
        TMemoryStream *pMem;
        void setPMem(TMemoryStream* pMS) {pMem = pMS;}
        TMemoryStream *pMemDebug;       // AP, 2005.07.13
        void setPMemDebug(TMemoryStream* pMS) {pMemDebug = pMS;}
        TCriticalSection* pMemCriticalSection;
        TCriticalSection* pMemDebugCriticalSection;
#else  // __BORLANDC__
		bool logEnabled, debugEnabled;
		void enableLog(bool state) {logEnabled = state;}
		void enableDebug(bool state) {debugEnabled = state;}
#endif  // __BORLANDC__

        DefaultsRecord *pRecord;
        void setDefaultsRecord(DefaultsRecord *pRec) {pRecord = pRec;}
        DefaultsRecord *getDefaultsRecord() {return pRecord;}

        void SetSubDomain(SubDomain* sd) {subDomain = sd;}
        SubDomain *GetSubDomain() {return subDomain;}

        bool EnableContinuity;
        bool EnableReinitializeFromGo;
        bool EnableReinitializeFromIntegrate;
        bool EnableMultiThread;
        bool IsMultiThreadedVersion;

        double ECDCriticalDepthForLandBoundary;   // AP, 050309
        double GetCriticalDepth() {return ECDCriticalDepthForLandBoundary;}

        // AP, 2005.09.15
        virtual void procMsgECDP(EcoDynProtocol* ecdp,
                MESSAGE_ECDP* msg, RegionList* pRL, bool notRunning) {}
        // AP, 2006.06.30
        virtual Queue* getSeedingQueue(){return NULL;}

        // methods for manipulate TEcoDynClass descendants queue
        void setEDCQueue(Queue* pQueue) {pEDCQueue = pQueue;};
        Queue* getEDCQueue() {return pEDCQueue;};

        // ======= EcoDynMisc.cpp =======
        // AP, 2007.06.21 - moved to here
        // Handles for Variables and Parameters files
        // (void*) to avoid inclusion of "iodll.h"
        // proper cast must be made in code
        void* OpenVariablesFile(char* classPrefix);
        void* OpenParametersFile(char* classPrefix);
        void* SaveVariablesFile(char* classPrefix);
        void* SaveParametersFile(char* classPrefix);
        void CloseDataFile(void* fHandle);

        // generic database files
        void* OpenDataFile(char* classPrefix, const char* suffix);
        void* SaveDataFile(char* classPrefix, const char* suffix);

        // specific database files
        bool SaveMorphology(char* AEcosystemName);
        bool SaveBenthicSpecies(char* AEcosystemName);
        bool SaveLoads(char* AEcosystemName);
        bool SaveLosses(char* AEcosystemName);
        bool SaveRivers(char* AEcosystemName);
        bool SaveSeaBoundaries(char* AEcosystemName);
        bool SaveSediments(char* AEcosystemName);
        bool SaveIce(char* AEcosystemName);
        bool SaveTide(char* AEcosystemName);

    protected :
        char MyPathName[129];

        TEcoDynClass *MyPEcoDynClass;
        // Generic variables, below: Just for children and integration
        double *Generic,
               *GenericLoad,
               *GenericFlux,
               *GenericExchange;
               //*Loads, *Losses;

        double GenericRiver,
               GenericOcean;

        // AP, 2007.04.24
        int NumberOfBoxes;
        int NumberOfLines;
        int NumberOfColumns;
        int NumberOfLayers;
        int NumberOfIceLayers;

        char modelType[16];

        // Generic for all objects
        int ObjectCode;                 // Each object must register its own code
        int NumberOfVariables;          // Each object must read this from XLS file
                                                  // used both for state variables and forcing
                                                  // functions
        VNA* VariableNameArray;         // Each object must dynamically create an

        int NumberOfParameters;         // Each object must read this from XLS file
		VNA* ParametersNameArray;		// Each object must dynamically create on

        int VariableCount;              // For turnon member function of each object
        int ParameterCount;             // For turnon member function of each object
        typedef int BNA;                // Boundary name array - for effluent boundaries
        BNA* BoundaryNumberArray;       // Boundary number array - for effluent boundaries
        typedef double LCA;             // Load conc array - for each boundary
        LCA* LoadConcentrationArray;    // Load conc array - for each boundary
        typedef char UNA[41];
        UNA* UnitNameArray;             // Each object must dynamically create an
                                                  // array for state variable  or forcing
                                                  // function units - NEW 98.09.01
        float TimeStep;                 // Model timestep in days
        float MyRunTime;                // Model run-time in days
        double MyTime;                   // Objects get this from TIntegrate on the fly

        long TimeUTC;					// Time UTC (in seconds)
        long startTimeUTC;				// Simulation start Time UTC (in seconds)

        int Year,
            Month,
            JulianDay,
            Date;

        float CurrentTime,
              TimeBefore;

        // for tridiagonal matrices
        double
            *a,
            *b,
            *c,
            *r,
            *u,
            *gam,
            *A,
            *B;
        int *Indx;

        virtual void Integration();
        virtual void Integration3DSubDomain(int UpperLayer);
        virtual void Integration(int NumberOfClasses);
        virtual void Integration(int NumberOfLayers, int ANumberOfBoxes, SedimentsRecord *ASedimentsRecord);
        virtual void Integration(double AStep);
        // AP, 2006.07.25
        virtual void Integration(double *Generic, double *GenericFlux);
        virtual void Integration(int NumberOfClasses, double *Generic, double *GenericFlux);
        // AP, 2005.05.17
        virtual void Integration(double *A, double *AFlux,
                                 double *B, double *BFlux,
                                 double *C, double *CFlux,
                                 double *D, double *DFlux,
                                 double *E, double *EFlux);
        virtual void Integration(double *A, double *AFlux,
                                 double *B, double *BFlux,
                                 double *C, double *CFlux,
                                 double *D, double *DFlux,
                                 double *E, double *EFlux,
                                 int NumberOfBoxes);
        virtual void IntegrationTridag (/*double*, double*, double*, // tridag
                                          double*,*/ int n);

        virtual void IntegrationTridag (double *, double *, double *, // tridag
                                        double *,int n);
        virtual void IntegrationBandec(double **a, unsigned long n, int m1, int m2,
                                       double **a1, unsigned long indx[], float *d);
        virtual void Ludcmp(int n, double *, int*);
        virtual void Lubksb(int n, double *, int*, double *);
        virtual void mprovetridag(double *, double *, double *, double *, double *, int n);
#ifndef _PORT_FORTRAN_
        virtual void BuildClassVariableNames(int NumberOfVariables, int NumberOfClasses);
#endif  _PORT_FORTRAN_ 
        int GetDaysToMonth(int year, int month);
        int dayOfYear(int year, int month, int day);

        //24/3/2003
        char MorphologyFileName[129];
        char VarsFileName[129];
        char ParsFileName[129];
        char TideFileName[129];
        char ClassesFileName[129];
        char EcoDynClassName[100];       // AP, 031211
        char LoadsFileName[129];
        char LossesFileName[129];
        char RiversFileName[129];
        char SeaBoundariesFileName[129];
        char SedimentsFileName[129];
        char BenthicSpeciesFileName[129];
        char IceFileName[129];
        // Calendar
        int DaysInMonth[12];
        long AStartTime, AFinishTime, ATime, ADay;

				/* 07.October.2013, AP 
				 * general functions defined to deal with random numbers
				 * maintaining Borland names
				 * 
				 * implemented in EcoDynMisc.cpp
				 */
				/**
				 * generate new random sequence
				 */
				void randomize();   
				/**
				 * generate a random integer value between 0 and 'maxRand'
				 * @param maxRand - upper limit
				 * @return integer value between 0 and maxRand
				 */
				int random(int maxRand);
				/**
				 * generate a random double value between 0 and 'maxRand'
				 * @param maxRand - upper limit
				 * @return double value between 0 and maxRand
				 */
				double random(double maxRand);
				// ... till here, AP
				
    private:
        void clearVariables();
        void buildCalendar(float timeStep);
        // Display arrays
        int MyVarRecSize;
        VariableRecord *MyVarRec;

        int MyDisplayBoxSize;
        DisplayBoxRecord* MyDisplayBoxRec;

        // sub domain points
        SubDomain* subDomain;

        // Number of active objects
        int ActiveObjects, ActiveStateVariables;

        // Chosen variables
        int NumberOfSelectedVariables;

        // Save start and finish
        int StartSaveDay, EndSaveDay;

        // Output frequency
        float OutputFrequency;

        // Vertical box structure
        float MyBoxTop, MyBoxBottom;

        // all the pointers
        // State variable objects
        TEcoDynClass *MyTransportPointer;
        TEcoDynClass *MyNutrientPointer;
        TEcoDynClass *MySPMPointer;
        TEcoDynClass *MyPhytoplanktonPointer;
        TEcoDynClass *MyZooplanktonPointer;
        TEcoDynClass *MySaltMarshPointer;
        TEcoDynClass *MyZoobenthosPointer;
        TEcoDynClass *MyZoobenthosPointer2;
        TEcoDynClass *MyNektonPointer;
        TEcoDynClass *MyManPointer;
        TEcoDynClass *MyFisherPointer;
        //TEcoDynClass *MyXenobioticPointer;
        TEcoDynClass *MyBacteriaPointer;
        TEcoDynClass *MyMacrophytePointer;
        TEcoDynClass *MyMicrophytobenthosPointer;
        // Forcing function objects
        TEcoDynClass *MyLightPointer;
        TEcoDynClass *MyAirTemperaturePointer;
        TEcoDynClass *MyWaterTemperaturePointer;
        TEcoDynClass *MyWindPointer;
        TEcoDynClass *MyFlowPointer;
        TEcoDynClass *MyTidePointer;
        TEcoDynClass *MySedimentPointer;
        TEcoDynClass *MyPreyPointer;
        TEcoDynClass *MyPredatorPointer;
        TEcoDynClass *MyFishPointer;
        TEcoDynClass *MySeaIcePointer;
        TEcoDynClass *MyIceAlgaePointer;


        // Queue of TEcoDynClass descendants
        Queue *pEDCQueue;
};
#endif //_ECODYNCLASS_H_


// EcoDynamo globals - header file

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "ecodyn.rh"

#ifndef __BORLANDC__
	#define TObject long
#endif  // __BORLANDC__

class TBoxes
{

	public:
        typedef struct BoundaryExchange
		{
			char BoundaryName[21],
				  DummyChar1[21];
			int BoundaryNumber,
				 TypeOfBoundary,
				 DummyInt2;
			double BulkDispersionCoefficient,
					 AdvectiveFlow,
					 Concentration,
					 DummyDouble2;
			bool UseForDispersionCoefficients,
				  Dummybool2;
		} BoundaryExchangeRecord;
		// Morphology box record definition
		typedef struct Boxes              // List of box parameters including "spares"
		{
			// Morphology
			double   Volume,
                                 SurfaceArea,
                                 Depth,
                                 Length,
                                 Width,
                                 Elevation,
                                 XPosition,              // For lagrangian
                                 YPosition,              // coordinate
                                 ZPosition,              // movement
                                 DummyDouble1,
                                 DummyDouble2,
                                 DummyDouble3;
			// General box data
			int  BoxNumber,
				 NumberOfBoundaryConnections,
                 //BoxToBoundary[MaximumBoundaryConnections],
                 Type,
				 DummyInt1,
				 DummyInt2,
				 DummyInt3;
			// Box booleans
			bool BoxConnectionsOK,
				  BoundaryConnectionsOK,
				  CanBeResized,             // For lagrangian re-sizing
				  Dummybool1,
				  Dummybool2,
                  Dummybool3;
            BoundaryExchangeRecord BoxToBoundary[MaximumBoundaryConnections];
		} BoxRecord;
};
//static TBoxes *PBoxes;

// struct to save simulation periods
typedef struct simulation_period
{
    long start;
    long finish;
    long cycle;
    int unit;   // unit definition in 'DefinePeriodsDialog.h'
    int fileIndex;
} SimulationPeriod;

//struct Defaults
typedef struct defaults_record
{
    bool Default;
    bool autoInitialise;
    char EcosystemName[256];
    char EcosystemType[256];
    char PathName[256];
    char PrefixName[256];
    int nrClassesAvailable;
    char AvailableClasses[200][256];
    int nrClassesSelected;
    char SelectedClasses[200][256];
    int nrVariablesAvailable;
    char AvailableVariables[500][256];
    int nrVariablesSelectedFileOutput;
    char SelectedVariablesFileOutput[500][256];
    int nrVariablesSelectedGraphOutput;
    char SelectedVariablesGraphOutput[200][256];
    int nrVariablesSelectedTableOutput;
    char SelectedVariablesTableOutput[200][256];
    long StartTime;
    long FinishTime;
    double TimeStep, OutputFrequency;
    long OutputStartTime;
    long OutputFinishTime;
    char OutputTimeUnit[256];
    char TimeUnit[256];
    char RunMode[256];
    char Integration[256];
    // AP, 2005.08.02...
    char SimulationRunType[256];
    int NumberOfPeriods;
    SimulationPeriod period[12];
} DefaultsRecord;

//struct ModelCfg
typedef struct model_cfg
{
    char ModelName[256];
    char ModelPath[256];
    bool ModelDefault;
    bool AutoInitialise;
} ModelCfg;

//struct ModelAndPrefix
typedef struct model_and_prefix
{
    char ModelName[256];
    char ModelPath[256];
    char Prefix[256];
} ModelAndPrefix;

typedef struct Parms
{
    char ParameterName[100];
    char Units[41];
    int ObjectCode;
    double Value, LowerValue, HigherValue, Increment;
    bool VariableValue, RandomizeFlag;
} ParsRecord;

typedef struct Loads
{
    char LoadName[200];
    int LineCoordinate, ColumnCoordinate, LayerCoordinate;
    double *LoadFlow;
} LoadsRecord;

typedef struct Losses
{
    char LossesName[200];
    int LineCoordinate, ColumnCoordinate, LayerCoordinate;
    double *LossFlow;
} LossesRecord;

typedef struct PB
{
    int Line;
    int Column;
} PointBox;

typedef struct SD
{
    int NumberOfBoxes;  // number of boxes in the domain
    int FirstLine;      // sub-domain first line (default 0)
    int LastLine;       // sub-domain last line (default NL-1)
    int FirstColumn;    // sub-domain first column (default 0)
    int LastColumn;     // sub-domain last column (default NC-1)

    int* AnIndex;

    int* ILine;         // initial line of each column (default 0)
    int* FLine;         // final line of each column (default NL-1)
    int* IColumn;       // initial column of each line (default 0)
    int* FColumn;       // final column of each line (default NC-1)
    int* BoxNumber;     // boxes numbers of the domain
} SubDomain;

typedef struct Rivers
{
    char RiverName[200];
    bool HasLayers;
    int LineCoordinate, ColumnCoordinate, LayerCoordinate;
    int BoundaryNumber;
    double *RiverFlow;
} RiversRecord;

typedef struct SeaBoundaries
{
    bool HasLayers;
    bool HasVelocitySeaBoundaries;
    int LineCoordinate, ColumnCoordinate, LayerCoordinate;
    int BoundaryNumber;
    double *Velocity;
} SeaBoundariesRecord;

typedef struct Sediments
{
    char SedimentName[200];
    int LineCoordinate, ColumnCoordinate;
    double Density, Porosity, LayerHeight/*delta x in Stella*/, SedimentWaterRatio /*g sediment per litre of water*/;
} SedimentsRecord;

typedef struct BenthicSpecies
{
    char SpeciesName[200];
    int LineCoordinate, ColumnCoordinate;
} BenthicSpeciesRecord;

typedef struct Cells
{
    int LineCoordinate, ColumnCoordinate, CellNumber, AnIndex;
} CellsRecord;

typedef char VNA[500];

typedef struct CriticalCells
{
  bool Critical;
  int ABoxNumber;
  double CellTimeStep;
  int NumberOfSteps;
  double NorthFlow, SouthFlow, WestFlow, EastFlow;
}  CellRecord;

typedef struct Ice
{
    char IceName[200];
    int LineCoordinate, ColumnCoordinate, CongelationIceLayers, SkeletalIceLayers, PlateletIceLayers;
    double CriticalIceThickness;
} IceRecord;
/*
int *Cells;
//int NumberOfCriticalCells, CriticalNumberOfSteps;
double CriticalTimeStep;
CellRecord* ACellRecord;
  */

// AP, 2006.06.26
typedef struct step_time {
    long startTime;
    long finishTime;
    double interval;
    double regTime;
    int MyNumberOfRemoteRegister;
} STEP_TIME;


#define GLB_CLASSES "Classes.xls"
#define GLB_VARIABLES "Variables.xls"
#define GLB_PARAMETERS "Parameters.xls"
#define GLB_MORPHOLOGY "Morphology.xls"
#define GLB_TIDE "Tide.xls"
#define GLB_LOADS "Loads.xls"
#define GLB_RIVERS "Rivers.xls"
#define GLB_SEABOUNDARIES "SeaBoundaries.xls"
#define GLB_SEDIMENTS "Sediments.xls"
#define GLB_BENTHICSPECIES "BenthicSpecies.xls"
#define GLB_INDIVIDUALS "Individuals"
#define GLB_LOSSES "Losses.xls"
#define GLB_ICE "Ice.xls"
#define GLB_ICE1 "IceVars"

#define GLB_TIMESERIES "HydroTimeSeriesValues_"
#define GLB_TIMESERIES_STEPS 10

#define GLB_XLSP ".xls"
#define GLB_HDFP ".hdf"

#define SI_TIMESERIES "SeaIceTimeSeriesValues_"
#define WT_TIMESERIES "WaterTemperatureTimeSeriesValues_"
#define AT_TIMESERIES "AirTemperatureTimeSeriesValues_"

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif // MIN

#define DMY_HMS_FORMAT "%d-%m-%Y %H:%M:%S"

// symbols used by date-time TMaskEdit components
#define DATE_TIME_FORMAT "%d%m%Y%H%M"
#define DATE_TIME_DECODER "%02d%02d%04d%02d%02d"


#endif // _GLOBALS_H_


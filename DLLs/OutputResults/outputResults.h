//---------------------------------------------------------------------------
#ifndef OutputResultsH
#define OutputResultsH

#include <iostream>
using namespace std;

#include "globals.h"
#include "EcoDynClass.h"
#include "Queue.h"
#include "iodll.h"

#ifndef __BORLANDC__
	#define _export
#else  // __BORLANDC__
    #include "MLHDF.h"
#endif  // __BORLANDC__

typedef struct
{
	int nrVariablesSelectedFileOutput;
	char SelectedVariablesFileOutput[500][256];
} HDFOutput_record;


typedef struct {
	char outputFileName[64];		// value passed by arg (-outfile)
    char logFileName[64];    		// value passed by arg (-logsteps)
	char regsBoxFileName[64];		// value passed by arg (-outboxes)
    int MeanValuesSamplesNumber;	// value passed by arg (-meanvalues)
    int HDFSamplesNumber;			// value passed by arg (-hdfsamples)
    
    TEcoDynClass::VarsRecord* varsRecord;		// ptr to 'Main' VarsRecord
    TEcoDynClass* MyPEcoDynClass;		// ptr to 'Main' TEcoDynClass
    DefaultsRecord* defaultsRecord;

	HDFOutput_record hdfOutputWater_record;
	HDFOutput_record hdfOutputIce_record;
} OUTPUT_REGISTER;


class _export OutputResults {
  public:
		/*===  OutputResults.cpp  ===*/
	OutputResults(OUTPUT_REGISTER* pRegister);
	~OutputResults();
    bool outputToFile();
	void clearMeanSamples();
	string GetFileExtension(string AFileName);

	void BuildIndividualsRecord();
    void BuildTimeSeriesRecord();
    void BuildIceRecord();

    void BuildIndividualsFilename(char* name, char* individName);
    void BuildIceFilename(char* name, char* iceName);
    
    void FillBoxes(Queue* pQueue, char* filename);
	bool readPointsFile(char* fileName, int& size, PointBox* &filePoints);
	
    bool IsVariableAvailable(char* varName);
    bool IsVariableAvailable(TEcoDynClass::VarsRecord* pVars, char* varName);
    bool IsIndividualVariable(char* pName);
    bool IsIndividualVariable(TEcoDynClass::VarsRecord* pVars, char* pName);
    bool IsTimeSeriesVariable(char* pName);
    bool IsTimeSeriesVariable(TEcoDynClass::VarsRecord* pVars, char* pName);
	bool IsIceVariable(char* pName);
    bool IsIceVariable(TEcoDynClass::VarsRecord* pVars, char* pName);

		/*===  SetupOutput.cpp  ===*/
    void SetupOutput();
    void SetupOutputFile();
	void SetupIndividualsFile();
	void SetupTimeSeriesVariables();
	void SetupHeaderTimeSeriesFile(int fileIndex);
	
		/*===  UpdateOutput.cpp  ===*/
    void UpdateOutput(double utcTime, double ATime);
	void UpdateFile(double utcTime, double ATime);
	void UpdateIndividualsFile(double utcTime, double ATime);
	void UpdateTimeSeriesFile(double utcTime, double ATime);	
	
		/*===  FinalizeOutput.cpp  ===*/
	void FinalizeOutput();
	void FinalizeFile();
	
		/*===  HDFOutput.cpp  ===*/
	void SetupHDFOutput();
#ifndef __BORLANDC__
    void SetupHDFSeriesFile(int fileIndex);
	void UpdateHDFFile();
	void flipLinesAndLayers(double *DataSeries, int nLines, int nColumns, int nLayers);
	// 2D version
	bool SetupHDFFile(char* AFileName, int nDim, 
		int nLines, int nColumns,
        int NumberOfVariables, char names[][256]);
	void UpdateHDFDataSeries(char* AFileName, int Step, int n, 
		int nLines, int nColumns, int DataID, 
		char* variableName, double* DataSet);
	// 3D version
	bool SetupHDFFile(char* AFileName, int nDim, 
		int nLines, int nColumns, int nLayers, 
		int NumberOfVariables, char names[][256]);
	void UpdateHDFDataSeries(char* AFileName, int Step, int n, 
		int nLines, int nColumns, int nLayers, int DataID, 
		char* variableName, double* DataSet);
#endif  // __BORLANDC__

		/*===  IceOutput.cpp  ===*/
	void SetupIceFile();
	void UpdateIceFile(double utcTime, double ATime);

#ifdef __BORLANDC__
		/*===  Win_MLHDF.cpp  ===*/
        /* interface with Windows MLHDF class in Windows systems */
    void MLHDF_SetupHDFSeriesFile(int fileIndex);
    void MLHDF_UpdateHDFFile();
#endif  // __BORLANDC__

  private:
	OUTPUT_REGISTER* myRegister;
    bool individualsFile;
    bool timeSeriesFile;
    bool hdfFile;
    bool iceFile;
    int timeSeriesVariables;
    int IceLayers;
	char MyFile[512];
    char MyFileIndividuals[512];
    char MyFileTimeSeries[512];
    char MyFileHDFSeries[512];
    char MyFileIce[512];
    char MyFileIceHDFSeries[512];
    Queue* pBoxFileQueue;	// RegBox queue for output results
    Queue* pLogStepQueue;	// LogStep(RegBox) queue for logging messages
    int MyNumberOfRecordedTimeSteps;	// TimeSteps recorded in file

#ifdef __BORLANDC__
    MLHDF* MyMLHDF;
    MLHDF* MyMLHDFIce;
#endif  // __BORLANDC__
};

class _export RegBox {
  public:		// User declarations
    RegBox() {};
    RegBox(int nNumber, int nLine, int nColumn) {
		number = nNumber;
		line = nLine,
		column = nColumn;
	};
    ~RegBox() {};

    int number;
    int line;
    int column;
};

#endif //OutputResultsH

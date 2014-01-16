/*
 * main.cpp - A.Pereira (amcp.62@gmail.com) 10/2013
 * 
 */


#include <iostream>
#include <ctime>
#include <unistd.h>

using namespace std;

#include "EcoDynamo.h"

timespec diff(timespec start, timespec end);  // RunThread.cpp

void printOptions(char* cmd);

int main(int argc, char **argv)
{
char pathName[256]={0}, modelName[64]={0}, fileName[64]={0}, agentName[64]={0};
char logName[64]={0}, domainName[64]={0}, boxesName[64]={0};
int portNumber=-1, meanSamplesValue=-1, hdfSamplesValue=-1;
bool logThreads=false, debugMessages=false;
timespec t, start, end;


	clock_gettime(CLOCK_REALTIME, &start);  //register start time
	if (argc < 2) {
		printOptions(argv[0]);
		return -1;
	}
	
	int i = 1;
	while(i < argc) {
		cout << "argv["<<i<<"]="<<argv[i]<<endl;
		if (strcmp(argv[i], "-path") == 0)
			strcpy(pathName, argv[++i]);
		else if (strcmp(argv[i], "-name") == 0)
			strcpy(modelName, argv[++i]);
		else if (strcmp(argv[i], "-outfile") == 0)
			strcpy(fileName, argv[++i]);
		else if (strcmp(argv[i], "-agent") == 0)
			strcpy(agentName, argv[++i]);
		else if (strcmp(argv[i], "-port") == 0)
			portNumber = atoi(argv[++i]);
		else if (strcmp(argv[i], "-logthreads") == 0)
			logThreads = true;
		else if (strcmp(argv[i], "-debug") == 0)
			debugMessages = true;
		else if (strcmp(argv[i], "-logsteps") == 0)
			strcpy(logName, argv[++i]);
		else if (strcmp(argv[i], "-subdomain") == 0)
			strcpy(domainName, argv[++i]);
		else if (strcmp(argv[i], "-outboxes") == 0)
			strcpy(boxesName, argv[++i]);
		else if (strcmp(argv[i], "-meanvalues") == 0)
			meanSamplesValue = atoi(argv[++i]);
		else if (strcmp(argv[i], "-hdfsamples") == 0)
			hdfSamplesValue = atoi(argv[++i]);
		++i;
	}

	if (strlen(pathName) <= 0) {
		printOptions(argv[0]);
		return -1;
	}
    if (access(pathName, F_OK)) {
        cerr << "\n\t[1] Directory " << pathName << " does not exist." << endl;
        cerr << "\t    Please insert a valid path for an EcoDynamo model.\n" << endl;
        return -1;
    }

	EcoDynamo* eco = new EcoDynamo(pathName);
	eco->setModelName(modelName);
	eco->setOutputFileName(fileName);
	eco->setAgentName(agentName);
	eco->setPortNumber(portNumber);
	eco->setLogFile(logName);
	eco->setDomainFile(domainName);
	eco->setRegsBoxFileName(boxesName);
	eco->setRegisterMeanValues(meanSamplesValue);
	eco->setHDFSamplesValue(hdfSamplesValue);
	if (logThreads)
		eco->setLogThreads(logThreads);
	if (debugMessages)
		eco->setDebugMessages(debugMessages);

	if (!eco->readParameters()) {
        cerr << "\n\t[2] Error reading model properties." << endl;
        cerr << "\t     Please correct EcoDynamo.properties file in " << argv[1] 
                << " directory \n\n " << endl;
        return -1;
	}
	
	/*
	 * test functions - to remove
	 */
	eco->printDefaults();
	if (!eco->saveParameters("EcoDynamo1.properties")) {
        cerr << "\n\t[3] Error saving model properties.\n\n" << endl;
	}
	/*
	 * till here
	 */

    eco->runModel();
	
	clock_gettime(CLOCK_REALTIME, &end);	// register end time
	t = diff(start, end);
	cout << endl << argv[0] << " execution time: " << (t.tv_sec%86400)/3600 
			<< ":" << (t.tv_sec%3600)/60 << ":" << t.tv_sec%60 <<  "." 
			<< t.tv_nsec << endl << endl;;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t);	// register CPU time
	cout << argv[0] << " CPU execution time: " << (t.tv_sec%86400)/3600 
			<< ":" << (t.tv_sec%3600)/60 << ":" << t.tv_sec%60 <<  "." 
			<< t.tv_nsec << endl << endl;;

	return 0;
}

void printOptions(char* cmd)
{
	cout << "\n " << cmd << " should be invoked with mandatory and optional parameters: " << endl;
	cout << "\n    " << cmd << " -path <model_path> [optional parameters]\n" << endl;
	cout << " Available parameters are:\n" << endl;	
	cout << "   -path <model_path>                 .indicates the model path with configuration files" << endl;	
	cout << "\n Optional:" << endl;
	cout << " Optional (can be read from 'configuration.cfg' file in <model_path>)" << endl;
	cout << "   -name <model_name>                 .indicates the name of the model" << endl;	
	cout << "   -outfile <output_filename>         .indicates name of the file to save results" << endl;	
	cout << "   -subdomain <domain_filename>       .indicates name of the file with sub-domain definition" << endl;	
	cout << "   -outboxes <output_boxes_filename>  .indicates name of the file with boxes to register" << endl;	
	cout << "   -logsteps <log_filename>           .indicates name of the file with steps to log messages between DLLs" << endl;	
	cout << "   -logthreads                        .display log messages from threads" << endl;	
	cout << "   -debug                             .display debug messages from DLLs" << endl;	
	cout << "   -meanvalues <samples>              .register only mean values in files with <samples> registers (special simulation) " << endl;	
	cout << "   -hdfsamples <samples>              .results saved in HDF format in files with <samples> registers " << endl;	
	cout << "   -agent <agent_name>                .indicates the name of "<< cmd << " in EcoSimNet platform" << endl;	
	cout << "   -port <port_number>                .indicates the port number used in EcoSimNet platform [default 45000]" << endl;	
	cout << endl;
}

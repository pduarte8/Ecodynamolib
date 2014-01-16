//---------------------------------------------------------------------------

/**
 * NPI work
 * 	01 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
 
#ifdef __BORLANDC__
	#include <vcl.h>
	#pragma hdrstop
	
	#include <dos.h>
	#pragma package(smart_init)
#else  // __BORLANDC__
	#include <iostream>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
	#include "Log.h"

//---------------------------------------------------------------------------

Log::Log() {
    pLogFile = NULL;
    logType = XML;
}
//---------------------------------------------------------------------------

Log::~Log() {
    if (pLogFile != NULL) {
        pLogFile->CloseFile();
    }
}
//---------------------------------------------------------------------------

Log::Log(LOGTYPE type, bool cleared, char* file)
{
  char dir[MAXPATH];

    logType = type;
    getAbsolutePath(dir);
    pLogFile = new TReadWrite(file, dir);
    openLog(cleared);
}

//---------------------------------------------------------------------------

Log::Log(LOGTYPE type, bool cleared, char* file, char* path)
{
  char dir[MAXPATH];

    logType = type;
    if (path[0] == '\0')
        getAbsolutePath(dir);
    else
        sprintf(dir, "%s%c", path, NUL);
    pLogFile = new TReadWrite(file, dir);
    openLog(cleared);
}

//---------------------------------------------------------------------------

void Log::openLog(bool newFile)
{
    if (newFile) {
        pLogFile->SetupFile(READANDWRITENEWFILE, false);
        if (logType == XML) {
            write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n");
            write("<LOG>\n");
        }
    }
    else {
        pLogFile->SetupFile(READANDWRITEEXISTINGFILE);
        if (logType == XML) {
            pLogFile->SetFilePointer(sizeof("</LOG>\n") * (-1));
        }
    }
}

//---------------------------------------------------------------------------

void Log::closeLog()
{
    if (logType == XML) {
        pLogFile->WriteLine("</LOG>\n");
    }
}

//---------------------------------------------------------------------------
// read log from file
bool Log::read(bool next, char* text)
{
  char MyLine[BUFSIZ];

    if (pLogFile->ReadLine(next, MyLine)) {
        strcpy(text, MyLine);
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------------
// write log to file
bool Log::write(char* text)
{
    pLogFile->WriteLine(text);
    return true;
}
//---------------------------------------------------------------------------

// build current directory absolute name
void Log::getAbsolutePath(char* dir)
{
#ifdef __BORLANDC__
  char curDir[MAXPATH];
  int drive;

    if (getcurdir(0, curDir))
        return;
    if ((drive = _getdrive()) == -1)
        return;

	sprintf(dir, "%c:\\%s%c", 'A' + (drive-1), curDir, NUL);
#else  // __BORLANDC__
	strcpy(dir, "");
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------



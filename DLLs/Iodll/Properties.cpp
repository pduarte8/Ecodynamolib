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
	#include <cstdlib>
	#include <string>
	#include <cstring>
	#include <exception>
	#include <stdexcept>
	
	using namespace std;
#endif  // __BORLANDC__
#include "iodll.h"
#include "Properties.h"


//---------------------------------------------------------------------------


Properties::Properties() {
    dir[0] = '\0';
    nrOfProperties = 0;
/* AP 20131001... */
		properties = (PROPERTY*) calloc(N, sizeof(PROPERTY));	
/*
    properties = new PROPERTY[N];
    for (int i = 0; i < N; i++)
        properties[i][0] = '\0';
*/
}

//---------------------------------------------------------------------------
Properties::~Properties() {
    try {
/* AP 20131001... */
				free(properties);
//      delete [] properties;
				properties = NULL;
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
    catch (exception &e) {
        cerr << "CloseFile Standard exception: " << e.what() << endl;
    }
#endif  // __BORLANDC__
}

//---------------------------------------------------------------------------
// read properties from file
bool Properties::readProperties(char* fileName)
{
    getDir();
    return readProperties(fileName, dir);
}
//---------------------------------------------------------------------------

// read properties from file
bool Properties::readProperties(char* fileName, char* path)
{
  char MyLine[BUFSIZ];
  bool next = false;

    if (path[0] == '\0')
        return readProperties(fileName);

    TReadWrite* pReadWrite = new TReadWrite(fileName, path);
    if (pReadWrite->SetupFile(READFILE)) {
        while (pReadWrite->ReadLine(next, MyLine)) {
            strcpy(properties[nrOfProperties++], MyLine);
            next = true;
        }
    }

    pReadWrite->CloseFile();
    return next;
}
//---------------------------------------------------------------------------

// save properties in file for next startup
bool Properties::saveProperties(char* fileName)
{
    return saveProperties(fileName, dir);
}
//---------------------------------------------------------------------------

// save properties in file for next startup
bool Properties::saveProperties(char* fileName, char* path)
{
    if (path[0] == '\0')
        return saveProperties(fileName);

    TReadWrite* pReadWrite = new TReadWrite(fileName, path);
    if (!pReadWrite->SetupFile(WRITENEWFILE, false)) {
        return false;
    }

    for (int i = 0; i < nrOfProperties; i++) {
        if (strlen(properties[i]) > 0)
            pReadWrite->WriteLine(properties[i]);
    }
    pReadWrite->CloseFile();
    return true;
}
//---------------------------------------------------------------------------

// get property value
// return property index or -1 if property doesn't exist
int Properties::getProperty(char* key, char* value)
{
  int index, index2;
#ifdef __BORLANDC__
  AnsiString* property;

    for (int i = 0; i < nrOfProperties; i++) {
        property = new AnsiString(properties[i]);
        index = property->AnsiPos(EQUAL);
        if (property->SubString(1, index-1).AnsiCompare(key) != 0) {
            delete property;
          	continue;
        }
        index2 = property->AnsiPos(MY_EOL);
        strcpy(value, property->SubString(index+1, index2-(index+1)).c_str());
        delete property;
        return i;
    }
#else  // __BORLANDC__
    for (int i = 0; i < nrOfProperties; i++) {
        string property(properties[i]);
        index = property.find(EQUAL);
        if (strcmp(property.substr(0, index).c_str(), key) != 0) {
          	continue;
        }
        index2 = property.find(MY_EOL);
        // AP 20131028 - adapted to support Windows (CR+LF) and Unix (LF) end of line 
		if (property[index2-1] == CRETURN)
			index2--;

        strcpy(value, property.substr(index+1, index2-(index+1)).c_str());
        return i;
    }
#endif  // __BORLANDC__
    return -1;
}
//---------------------------------------------------------------------------

// put property value
// return number of properties
int Properties::putProperty(char* key, char* value)
{
  int index=-1;
		index = removeProperty(key);

    if (index != -1) {
        strcpy(properties[index], key);
        strcat(properties[index], "=");
        strcat(properties[index], value);
        strcat(properties[index], "\n");
    }
    else {
        strcpy(properties[nrOfProperties], key);
        strcat(properties[nrOfProperties], "=");
        strcat(properties[nrOfProperties], value);
        strcat(properties[nrOfProperties], "\n");
        nrOfProperties++;
    }
    return nrOfProperties;
}
//---------------------------------------------------------------------------

// put property value
// return number of properties
int Properties::putProperty(char* line)
{
  int n=-1;
#ifdef __BORLANDC__
  AnsiString* property = new AnsiString(line);

    int index = property->AnsiPos(EQUAL);
    int index2 = property->AnsiPos(MY_EOL);
    n = putProperty(property->SubString(1, index-1).c_str(),
                       property->SubString(index+1, index2-(index+1)).c_str());
    delete property;
#else  // __BORLANDC__
	string property (line);
	char key[128], value[128];

    int index = property.find(EQUAL);
    int index2 = property.find(MY_EOL);
		strcpy(key, property.substr(0, index).c_str());
		strcpy(value, property.substr(index+1, index2-(index+1)).c_str());
    n = putProperty(key, value);
#endif  // __BORLANDC__
    return n;
}
//---------------------------------------------------------------------------

// remove parameter from list
int Properties::removeProperty(char* key)
{
  int index;
  char param[256];

    index = getProperty(key, param);
    if (index != -1) {
        properties[index][0] = '\0';
//        properties[index] = "";
    }
    return index;
}
//---------------------------------------------------------------------------

// clear properties list
void Properties::clear()
{
/* AP, 20131001... 
    for (int i = 0; i < nrOfProperties; i++) {
        properties[i][0] = '\0';
    }
*/
		memset(properties, 0, N * sizeof(PROPERTY)); /*...*/
    nrOfProperties = 0;
}
//---------------------------------------------------------------------------

// build current directory absolute name
void Properties::getDir()
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



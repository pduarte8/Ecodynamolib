//---------------------------------------------------------------------------
#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_
//---------------------------------------------------------------------------

/**
 * NPI work
 * 	01 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
 
#ifdef __BORLANDC__
	#include <dir.h>
#else  // __BORLANDC__
	#define MAXPATH 260
	#define _export 
#endif  // __BORLANDC__

typedef char PROPERTY[512];

#define N 1000  // Max number of parameters to save

class _export Properties
{
  private:
    PROPERTY *properties;
    int nrOfProperties;
    char dir[MAXPATH];

  public:		// User declarations

    Properties();
    ~Properties();

    bool saveProperties(char* fileName);    // save properties in file for next startup
    bool readProperties(char* fileName);    // read properties from file
    bool saveProperties(char* fileName, char* path);    // save properties in file for next startup
    bool readProperties(char* fileName, char* path);    // read properties from file
    int getProperty(char* key, char* value); // get property value
    int putProperty(char* key, char* value); // put property value
    int putProperty(char* line); // put property value
    int removeProperty(char* key);    // remove property
    void clear();   // clear all properties

  protected:
    void getDir();
};

#ifdef __BORLANDC__
	extern PACKAGE Properties *pProperties;
#endif  // __BORLANDC__

#endif // PropertiesH

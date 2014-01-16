//---------------------------------------------------------------------------
#ifndef _LOG_H_
#define _LOG_H_
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

	#include "iodll.h"

#ifndef _LOGTYPE_
#define _LOGTYPE_
typedef enum {
    XML, XLS, TXT, REMOTE
} LOGTYPE;
#endif

static char LogTypeStr[3][8] = {".xml", ".xls", ".txt"};

class _export Log {
  private:
    TReadWrite* pLogFile;
    LOGTYPE logType;

    void getAbsolutePath(char* dir);
    void openLog(bool newFile);

  public:		// User declarations
    Log();
    ~Log();

    Log(LOGTYPE type, bool cleared, char* file);
    Log(LOGTYPE type, bool cleared, char* file, char* path);
    void closeLog();
    bool read(bool next, char* text);
    bool write(char* text);

};

#endif //_LOG_H_

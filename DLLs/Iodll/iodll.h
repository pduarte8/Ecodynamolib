#ifndef _IODLL_H_
#define _IODLL_H_

/**
 * NPI work
 * 	01 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
 
#ifdef __BORLANDC__
	#include <stdio.h>
	#include <direct.h>
	#include <io.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstddef>
	#include <cstdio>
	
	using namespace std;
#endif  // __BORLANDC__

#define READFILE                      1  // r
#define WRITENEWFILE                  2  // w
#define WRITEAPPENDFILE               3  // a
#define READANDWRITEEXISTINGFILE      4  // r+
#define READANDWRITENEWFILE           5  // w+
#define READANDWRITEAPPENDFILE        6  // a+

#define TABMARK  9     // tab character (used in tab separated values format)
#define NEWLINE  10    // new line character
#define CRETURN  13    // carriage return character (used in Windows environment)
#define COMMA    ','   // comma character (used in comma separated values format)
#define SPACE    ' '
#define MY_EOL   '\n'  // end of line character
#define EQUAL    '='
#define NUL      '\0'  // NUL character
#define MINUS    '-'   // minus character (45)
#define DOT      '.'   // decimal point character (46)

#define MEM_ALLOC_SIZE 0xa00000

// Excel record structure

// BOF_FILE
#define BOF_FILE            0x0009
#define BIT_BIFF5           0x0800
#define BIT_BIFF4           0x0400
#define BIT_BIFF3           0x0200
#define BOF_BIFF5           BOF | BIT_BIFF5
#define BOF_BIFF4           BOF | BIT_BIFF4
#define BOF_BIFF3           BOF | BIT_BIFF3

// EOF
#define  BIFF_EOF           0x000a

// Dimensions
#define  DIMENSIONS         0x0000
#define  DIMENSIONS_BIFF4   DIMENSIONS | BIT_BIFF3
#define  DIMENSIONS_BIFF3   DIMENSIONS | BIT_BIFF3

// Document types
#define  DOCTYPE_XLS        0x0010
#define  DOCTYPE_XLC        0x0020
#define  DOCTYPE_XLM        0x0040
#define  DOCTYPE_XLW        0x0100

// Use with output functions
#define  VER_BIFF4          0x04
#define  VER_BIFF3          0x03
#define  VER_BIFF2          0x02

// Structures
#define  LEN_RECORDHEADER   4

// Data types
#define  CellBlank          1
#define  CellInteger        2
#define  CellDouble         4
#define  CellLabel          8
#define  Cellboolean        16 // or error

// common dialog definitions

#define CM_OK               1
#define CM_SETREADONLY      1040
#define CM_VECTORFILE       1050
#define CM_DATAMATRIX       1060

#if __BORLANDC__
	#define PATH_SEPARATOR "\\" 
	#define OSCH_SEPARATOR '\\'
#else   // __BORLANDC__
	#define PATH_SEPARATOR "/" 
	#define OSCH_SEPARATOR '/'
	#define _export 
#endif   // __BORLANDC__

class _export TReadWrite
{
    public:
        TReadWrite(char* AFileName,
                   char* APathName = "",
                   double ANonValue = -1000000.0);
        ~TReadWrite();
        void CloseFile();
        virtual bool SetupFile(int WhatToDo, bool FileMustExist=true, bool report=true); // file checking
        void GetArraySize(int &AXSize, int &AYSize);
        char* SeekYPlace(int AYPlace);     // AP, 030506
        char* SeekXYPlace(int AXPlace, int AYPlace);     // AP, 040304
        void SetFilePointer(int offset);   // AP, 040109
//        void SetAppendFilePointer(int offset);   // AP, 040109
        bool ReadNumber(int AXPlace, int AYPlace, float &ANumber, float Default=0.0);
        bool ReadNumber(int AXPlace, int AYPlace, double &ANumber, double Default=0.0);
        bool ReadNumber(int AXPlace, int AYPlace, int &ANumber, int Default=0);
        bool ReadNumber(int AXPlace, int AYPlace, long &ANumber, long Default=0);
        bool ReadBool(int AXPlace, int AYPlace, bool &ANumber, bool Default=false);
        bool ReadString(int AXPlace, int AYPlace, char* AString);
        bool FindString(char* AString, int &AXPlace, int &AYPlace);
        bool FindItemInBlock(char* ABlockName,
                             char* AItemname,
                             bool ReturnNumber,
                             char* &AFullName,
                             double &AValue);
        bool FindBlock(char* ABlockStartAndEnd,
                       int StartRowNumber,
                       char* &AFullName,
                       int &ARowNumber,
                       bool StripLeaders);


        bool WriteCell(double ANumber, int Decimals = 2);
        bool WriteCell(char* AString);
        void WriteSeparator(bool EndOfLine = false);
        void WriteSeparator(char separator);
        virtual void WriteData(int AType, int ARow, int ACol, void* AData){};

        bool ReadLine(bool next, char* line); // AP, 030316
        bool WriteLine(char* line);    // AP, 030316
        int trimLeft(char* str);        // AP, 030616...
        int trimRight(char* str);
        void trim(char* dest, char* src);  // ...AP, 030616
        int GetNumberOfRows();
        int GetNumberOfColumns();

    protected :
        char MyFileName[255], MyPathName[255], FullFileName[255]; // filename must be 255 in case a path is passed
        int Row, Column;
        void *Data;        // See Swan p. 272
        char Buffer[BUFSIZ]; // See Swan p. 272
        //WORD typeRec, lenData;
        FILE* MyFilePointer;
        double *MyArray;
				double MyNonValue;
        char* pFileBuf;     // AP, 030506
        long *pIndex;      // AP, 030512
        int nDataRows, nDataColumns;  // number of rows and columns of the data

        virtual bool OpenFile(int MyJob, bool MyFileMustExist, bool report);
        virtual bool CheckFile(bool  ADud);
        bool IsSeparator(char OneChar);
        bool IsEndOfLine(char OneChar);
        bool ReadCell(int AXPlace, int AYPlace, char* ACell);

        long GetFileSize();                // AP, 030506
        char* ReadFileToMemory();          // AP, 030506
        void FillRowsAndColumns(char* pMem, long size); // AP, 041112
        void FillNumberOfRowsAndColumns(FILE* pFile);  // AP, 080331
};


#endif // _IODLL_H_


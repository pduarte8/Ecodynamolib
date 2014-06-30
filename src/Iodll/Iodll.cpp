// Input-output in a C++ class in a DLL
// @ J. Gomes Ferreira. Rev. A - 95.12.27
// Use class hierarchy to read and write to excel tab and binary files
// Use extern "C" to prevent C++ name-mangling

/**
 * NPI work
 * 	01 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifdef __BORLANDC__
	#include <stdlib.h>
	#include <math.h>
	#include <ctype.h>
	#include <vcl.h>
	#include <limits.h>
#else  // __BORLANDC__
	#include <cstdlib>
	#include <cmath>
	#include <cctype>
	#include <climits>
	#include <cstring>
	#include <exception>
	#include <stdexcept>
    #include <unistd.h>
	
	using namespace std;
#endif  // __BORLANDC__
	#include "iodll.h"

/**
 * definition of 'strupr' if it is necessary - AP, 2013.10.01
 */
#ifndef strupr
/**
 *  strupr - convert a string to uppercase
 *   definition needed to Linux
 */
char* strupr(char *str)
{
  char* ref = str;

   while(*str)
   {
       if ( *str >= 'a' && *str <= 'z' )
       {
          *str = *str - 32;
       }
       str++;
   }
   return ref;
}
#endif  // strupr

// implementation of TReadWrite

TReadWrite::TReadWrite(char* AFileName,
                       char* APathName,
                       double ANonValue)
{
/* AP, 20131001...
	memset(MyFileName, 0, sizeof(MyFileName));
    memset(MyPathName, 0, sizeof(MyPathName));
    memset(FullFileName, 0, sizeof(FullFileName));
*/
    strcpy (MyFileName, AFileName);
    strcpy (MyPathName, APathName);
    MyNonValue = ANonValue;

    if (strlen (MyPathName) != 0) // Build full name straight away
    {
        strcpy(FullFileName, MyPathName);
        strcat(FullFileName, PATH_SEPARATOR); // operating system path separator
        strcat(FullFileName, MyFileName);
    }
    else
        strcpy(FullFileName, MyFileName);
    
    MyFilePointer = NULL;
    pFileBuf = NULL;
    pIndex = NULL;
}

TReadWrite::~TReadWrite()
{
    CloseFile();
}

void TReadWrite::CloseFile()
{
    try {
        if (pFileBuf != NULL) {
            free(pFileBuf);
            pFileBuf = NULL;
        }
		if (pIndex != NULL) {
			free(pIndex);
			pIndex = NULL;
		}
        if (MyFilePointer != NULL) {
            fflush(MyFilePointer);
            fclose(MyFilePointer);
            MyFilePointer = NULL;
        }
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e) {
        //Application->ShowException(&e);
    }
    catch (Exception &exception) {
        //Application->ShowException(&exception);
    }
#else  // __BORLANDC__
    catch (runtime_error &rte) {
        cerr << "CloseFile Runtime exception: " << rte.what() << endl;
    }
    catch (exception &e) {
        cerr << "CloseFile Standard exception: " << e.what() << endl;
    }
#endif  // __BORLANDC__
}

bool TReadWrite::SetupFile(int WhatToDo, bool FileMustExist, bool report)
{
    return (OpenFile(WhatToDo, FileMustExist, report));
}

bool TReadWrite::OpenFile(int MyJob, bool MyFileMustExist, bool report)
{
    if (MyFileMustExist) // only check if required
    {
        if (!CheckFile(true))
        {
            if (report) {
#ifdef __BORLANDC__
                static char MyMessage[81];
                strcpy (MyMessage, FullFileName);
                strcat (MyMessage, " does not exist");
                MessageBox(0,MyMessage,"File Not Found",MB_OK);
#else  // __BORLANDC__
				cerr << FullFileName << " does not exist" << endl;
#endif  // __BORLANDC__
            }
            return false;
        }
    }
    switch (MyJob)
    {
        case READFILE :
        {
            MyFilePointer = fopen(FullFileName, "r");
            if ((pFileBuf = ReadFileToMemory()) == NULL)
            {
				if (report) {
#ifdef __BORLANDC__
					static char MyMessage[129];
					strcpy (MyMessage, FullFileName);
					strcat (MyMessage, ": Not enough memory to allocate reading buffer\n\t\t File read without optimization");
					MessageBox(0,MyMessage,"File Too Long",MB_OK);
#else  // __BORLANDC__
                    cerr << "OpenFile-ReadFile " << FullFileName << ": Not enough memory to allocate reading buffer!\n File read without optimization!" << endl;
#endif  // __BORLANDC__
				}
            }
            break;
        }
        case WRITENEWFILE :
        {
            MyFilePointer = fopen(FullFileName, "w");
            break;
        }
        case WRITEAPPENDFILE :
        {
            MyFilePointer = fopen(FullFileName, "a");
            break;
        }
        case READANDWRITEEXISTINGFILE :
        {
            MyFilePointer = fopen(FullFileName, "r+");
            if ((pFileBuf = ReadFileToMemory()) == NULL)
            {
				if (report) {
#ifdef __BORLANDC__
					static char MyMessage[129];
					strcpy (MyMessage, FullFileName);
					strcat (MyMessage, ": Not enough memory to allocate reading buffer\n\t\t File read without optimization");
					MessageBox(0,MyMessage,"File Too Long",MB_OK);
#else  // __BORLANDC__
					cerr << "OpenFile-ReadAndWriteExistingFile " << FullFileName << ": Not enough memory to allocate reading buffer!\n File read without optimization!" << endl;
#endif  // __BORLANDC__
				}
            }
            break;
        }
        case READANDWRITENEWFILE :
        {
            MyFilePointer = fopen(FullFileName, "w+");
            break;
        }
        case READANDWRITEAPPENDFILE :
        {
            MyFilePointer = fopen(FullFileName, "a+");
            if ((pFileBuf = ReadFileToMemory()) == NULL)
            {
				if (report) {
#ifdef __BORLANDC__
					static char MyMessage[129];
					strcpy (MyMessage, FullFileName);
					strcat (MyMessage, ": Not enough memory to allocate reading buffer\n\t\t File read without optimization");
					MessageBox(0,MyMessage,"File Too Long",MB_OK);
#else  // __BORLANDC__
					cerr << "OpenFile-ReadAndWriteAppendFile " << FullFileName << ": Not enough memory to allocate reading buffer!\n File read without optimization!" << endl;
#endif  // __BORLANDC__
				}
            }
            break;
        }
        default : return false;
    }
    return (MyFilePointer != NULL);
}

bool TReadWrite::CheckFile(bool ADud)
{
#ifndef __BORLANDC__
    return !access(FullFileName, F_OK);
#else  // !(__BORLANDC__)
    struct ffblk ffblk;
    if (ADud)
    {
        if (!findfirst(FullFileName, &ffblk, 0))
        {
            if (ffblk.ff_fsize != 0)
                return true; // check for dud (zero-size) files
        }
        else
            return false;
    }
    else if (!findfirst(FullFileName, &ffblk, 0))
        return true;
    return false; // Don't check
#endif  // !(__BORLANDC__)
}

// Return if 'OneChar' is Excel separator field
// Supported: Tab, Comma
//
// New formats only force changes here
//
// AP, 031104:
// Added support to Comma Separated Values Excel format file
//
bool TReadWrite::IsSeparator(char OneChar)
{
    if ((OneChar == TABMARK) || (OneChar == COMMA))
        return true;
    return false;
}

// Return if 'OneChar' is Excel end of record
bool TReadWrite::IsEndOfLine(char OneChar)
{
    if (OneChar == NEWLINE)
        return true;
    return false;
}

void TReadWrite::GetArraySize (int &AXSize, int &AYSize)
{
    AXSize = GetNumberOfColumns();
    AYSize = GetNumberOfRows();
}

// AP, 030122...
// This method will seek the file position indicator to the beginning of Y line
// Returns Y line pointer or -1 if an error occurs

char* TReadWrite::SeekYPlace (int AYPlace)
{
  char line[BUFSIZ];
  int nLine = 0;
  char* pChar;

    if (pFileBuf != NULL) {
        if (AYPlace >= nDataRows)
            return NULL;

        if (pIndex[AYPlace * nDataColumns] == 0)
            return NULL;

        pChar = (char*)pIndex[AYPlace * nDataColumns];
        return pChar;
    }
    else {
        rewind(MyFilePointer);
        while (nLine < AYPlace) {
            if (fgets(line, BUFSIZ, MyFilePointer) == NULL)
                return NULL;
            nLine++;
        }
        return (char*)MyFilePointer;    // something != NULL
    }
}

// AP, 040303...
// This method will seek the file position indicator to (XY) cell
// Returns XY cell pointer or NULL if an error occurs

char* TReadWrite::SeekXYPlace (int AXPlace, int AYPlace)
{
  char* pChar;

	if (AXPlace < 0 || AYPlace < 0)
		return NULL;
		
    if (pFileBuf != NULL)
    {
        if (AYPlace >= nDataRows || AXPlace >= nDataColumns)
            return NULL;

        if (pIndex[AYPlace * nDataColumns + AXPlace] == 0)
            return NULL;

        pChar = (char*)pIndex[AYPlace * nDataColumns + AXPlace];
        return pChar;
    }
    else    // read from MyFilePointer
    {
	    if ((pChar = SeekYPlace(AYPlace)) == NULL)
    	    return NULL;

    	Column = 0;
        if (fgets(Buffer, BUFSIZ, MyFilePointer) == NULL)
            return NULL;

        pChar = Buffer;
        while ((Column != AXPlace) && (*pChar != EOF)) {
            if (IsEndOfLine(*pChar))
                break;
            else if (IsSeparator(*pChar))
                Column++;
            pChar++;
        }
        return pChar;
    }
}

// AP, 040109...
// This method repositions the file pointer 'offset' bytes from current position
void TReadWrite::SetFilePointer(int offset)
{
    fseek(MyFilePointer, offset, SEEK_CUR);
}

// AP, 030506...
// This method returns the size of the file
// Returns number of bytes in file
long TReadWrite::GetFileSize()
{
    long curpos, length;

    curpos = ftell(MyFilePointer);
    fseek(MyFilePointer, 0L, SEEK_END);
    length = ftell(MyFilePointer);
    fseek(MyFilePointer, curpos, SEEK_SET);
    return length;
}

// This method read all file to allocated memory
// Returns pointer to allocated memory filled with file bytes
//   or NULL if it has no space in memory
char* TReadWrite::ReadFileToMemory()
{
    char *pMem;
    long size;
    size_t sizeRead;

    size = GetFileSize();
    try {
        pMem = (char*)malloc(size+1);	// to add EOF - AP, 2013.11.01
        if (pMem == NULL)
        {
           FillNumberOfRowsAndColumns(MyFilePointer);
           return NULL;
        }
    }
#ifdef __BORLANDC__
    catch (Exception& exception) {
        Application->ShowException(&exception);
        FillNumberOfRowsAndColumns(MyFilePointer);
        return NULL;
    }
#else  // __BORLANDC__
    catch (exception& exc) {
        cerr << "ReadFileToMemory Standard exception: "<< exc.what() << endl;
        FillNumberOfRowsAndColumns(MyFilePointer);
        return NULL;
    }
#endif  // __BORLANDC__

    sizeRead = fread(pMem, 1, size, MyFilePointer);
    pMem[sizeRead] = EOF;      // AP, 2007.05.13

    // only consider bytes efectively read: 0x0D0A is converted in 0x0A
    FillRowsAndColumns(pMem, sizeRead);
    return pMem;
}

//
// This method will fill nDataRows and nDataColumns variables
// and alloc memory and fill indexes to row/column (row*nDataColumns+col)
void TReadWrite::FillRowsAndColumns(char* pMem, long size)
{
    char *pChar;
    int Column, Row;
    long i;

    pChar = pMem;
    Column = 1;
    nDataRows = 1;
    nDataColumns = 1;
    for (i = 0; i < size; i++, pChar++) {
        if (IsSeparator(*pChar)) {
            Column++;
            if (Column > nDataColumns)
                nDataColumns = Column;
        }
        else if (IsEndOfLine(*pChar)) {
            Column = 1;
            nDataRows++;
        }
    }

    // alloc memory and fill indexes to row/column (row*nDataColumns+col)
    pIndex = (long*)calloc(nDataRows * nDataColumns, sizeof(long));
    pChar = pMem;
    Column = 0;
    Row = 0;
    pIndex[0] = (long)pChar;

    for (i = 0; i < (size - 1); i++, pChar++) {
        if (IsSeparator(*pChar)) {
            Column++;
            if (*(pChar + 1) != EOF)
	            pIndex[Row * nDataColumns + Column] = (long)(pChar + 1);
        }
        else if (IsEndOfLine(*pChar)) {
            Column = 0;
            Row++;
            if (*(pChar + 1) != EOF)
                pIndex[Row * nDataColumns] = (long)(pChar + 1);
        }
    }
}

// AP, 2008.03.31
// This method will fill nDataRows and nDataColumns variables
// without allocating memory
void TReadWrite::FillNumberOfRowsAndColumns(FILE* pFile)
{
    char ch;
    int Column, Row;

    Column = 1;
    nDataRows = 1;
    nDataColumns = 1;

    rewind(pFile);
    ch = (char)fgetc(pFile);
    while (ch != EOF) {
        if (IsSeparator(ch)) {
            Column++;
            if (Column > nDataColumns)
                nDataColumns = Column;
        }
        else if (IsEndOfLine(ch)) {
            Column = 1;
            nDataRows++;
        }
        ch = (char)fgetc(pFile);
    }
}

// read a cell value as String without heading and trailing spaces
bool TReadWrite::ReadCell (int AXPlace,
                           int AYPlace,
                           char* ACell)
{
    char OneChar, Conv[2], CellValue[129];
    char TrimmedCellValue[129];
    char* pChar;

    strcpy(ACell, "");
    if ((pChar = SeekXYPlace(AXPlace, AYPlace)) == NULL) {
        return false;
    }

    memset(CellValue, 0, sizeof(CellValue));
    memset(TrimmedCellValue, 0, sizeof(TrimmedCellValue));
    int ii;
    /* if ((AXPlace == 1) && (AYPlace == 916030)){
      cout << "ReadCell" << endl; 
      cout << pChar << endl; 
      cout << "FinishedReadCell" << endl;
      }*/
    OneChar = *pChar++;
    if ((AXPlace == 1) && (AYPlace == 916030))cout << "A" << endl;
    while (OneChar != EOF)
    {
        if (IsSeparator(OneChar) || IsEndOfLine(OneChar))
            break;
        else {
            sprintf(Conv, "%c", OneChar);
            strcat(CellValue, Conv);
        }
        OneChar = *pChar++;
    }
    //if ((AXPlace == 1) && (AYPlace == 916030))cout << "B" << endl;
    if (strlen(CellValue) != 0)
    {
    	trim(TrimmedCellValue, CellValue);
        // AP 2013.10.29 - adapted to support Windows (CR+LF) and Unix (LF) end of line
        if (TrimmedCellValue[strlen(TrimmedCellValue)-1] == CRETURN)
			TrimmedCellValue[strlen(TrimmedCellValue)-1] = '\0';
        strcpy(ACell, TrimmedCellValue);
        return true;
    }
    return false;
}

// read a double
bool TReadWrite::ReadNumber (int AXPlace,
                             int AYPlace,
                             double &ANumber,
                             double Default)
{
    char CellValue[129];
    ANumber = Default;

    // AP, 040304...
    if (!ReadCell(AXPlace, AYPlace, CellValue)) {
        return false;
    }

   	ANumber = atof(CellValue);
    return true;
    // ...AP, 040304
}

// overloaded to read float

bool TReadWrite::ReadNumber (int AXPlace,
                             int AYPlace,
                             float &ANumber,
                             float Default)
{
    char CellValue[129];
    ANumber = Default;

    // AP, 040304...
    if (!ReadCell(AXPlace, AYPlace, CellValue)) {
        return false;
    }

   	ANumber = atof(CellValue);
    return true;
    // ...AP, 040304
}

// Overloaded to read integers

bool TReadWrite::ReadNumber (int AXPlace,
                             int AYPlace,
                             int &ANumber,
                             int Default)
{
    char CellValue[129];
    ANumber = Default;

    // AP, 040304...
    if (!ReadCell(AXPlace, AYPlace, CellValue)) {
        return false;
    }

   	ANumber = atoi(CellValue);
    return true;
    // ...AP, 040304
}

// Overloaded to read longs

bool TReadWrite::ReadNumber (int AXPlace,
                             int AYPlace,
                             long &ANumber,
                             long Default)
{
    char CellValue[129];
    ANumber = Default;

    // AP, 040304...
    if (!ReadCell(AXPlace, AYPlace, CellValue)) {
        return false;
    }

   	ANumber = atol(CellValue);
    return true;
    // ...AP, 040304
}

// Overloaded to read booleans

bool TReadWrite::ReadBool (int AXPlace,
                             int AYPlace,
                             bool &ANumber,
                             bool Default)
{
    char CellValue[129];
    ANumber = Default;

    // AP, 040304...
    if (!ReadCell(AXPlace, AYPlace, CellValue)) {
        return false;
    }

   	ANumber = (atoi(CellValue) != 0);
    return true;
    // ...AP, 040304
}


bool TReadWrite::ReadString (int AXPlace,
                             int AYPlace,
                             char* AString)
{
	return ReadCell(AXPlace, AYPlace, AString); // AP, 040304
}

bool TReadWrite::FindString(char* AString, int &AXPlace, int &AYPlace)
{
    char CellName[129];
    char* pChar;

    AXPlace = -1;   // String not found
    AYPlace = -1;

    Row = 0;
    while (Row < GetNumberOfRows()) {
        Column = 0;
        while (Column < GetNumberOfColumns()) {
            ReadString(Column, Row, CellName);
			
            if (strcmp(AString, CellName) == 0) // Found match
            {
                AXPlace = Column;
                AYPlace = Row;
                return true;
            }
            Column++;
        }
        Row++;
    }
    return false;

}

bool TReadWrite::FindItemInBlock(char* ABlockName,
                                 char* AItemName,
                                 bool ReturnNumber,
                                 char* &AFullName,
                                 double &AValue )
{
    const int LeftSquareBracket = 91, RightSquareBracket = 93, equalsign = 61;
    int StartBlock, EndBlock;
    int X, Y, NumberOfLines;
    char AString[80], MySubString[80], MyNumberString[20], Temp[2];

    strcpy( AFullName, "NO ITEM" );
    AValue = -1;

    // return if nothing was sent
    if ( ( strlen (ABlockName) == 0 ) || ( strlen (AItemName) == 0 ) )
        return false;

    strcpy(MySubString, strupr(AItemName));

    GetArraySize(X, Y); // X position returned as 1
    NumberOfLines = Y;
    EndBlock = NumberOfLines;

    if (!FindString(ABlockName, X, Y)) // X position returned as zero
        return false;

    StartBlock = Y;

    // routine to find start and end of block

    for (int i = StartBlock; i <= EndBlock; i++) // check for bracketed block
    {
        ReadString(X, i, AString); // and then readstring works because x is zero
        if (
             ( AString[0] == LeftSquareBracket ) &&
             ( AString[strlen(AString)-1] == RightSquareBracket )
            )
        {
            EndBlock = i-1;
            break;
        }
    }
    for (int i = StartBlock; i <= EndBlock; i++)
    {
        ReadString(X, i, AString);
        strcpy(AString, strupr(AString));
        if (strstr(AString, MySubString) != NULL)
        {
            strcpy( AFullName, AString );
            if ( ReturnNumber )
            {
                int i = 0;
                bool PastEqual = false;
                strcpy(MyNumberString,"");
                while ( i < strlen(AString))
                {
                    if (PastEqual)
                    {
                        sprintf(Temp, "%c", AString[i]);
                        strcat(MyNumberString, Temp);
                    }
                    if (AString[i] == equalsign)
                        PastEqual = true;
                    i++;
                }
                AValue = atof(MyNumberString);
            }
            return true;
        }
    }
    return false;
}

bool TReadWrite::FindBlock(char* ABlockStartAndEnd,
                           int StartRowNumber,
                           char* &AFullName,
                           int &ARowNumber,
                           bool StripLeaders)
{
    int X, Y, NumberOfLines;
    char AString[80], MySubString[80], Temp[2], Temp1[2];
    strcpy( AFullName, "NO ITEM" );
    ARowNumber = -1;
    // return if nothing was sent
    if ( strlen (ABlockStartAndEnd) == 0 )
        return false;

    sprintf(Temp, "%c", ABlockStartAndEnd[0]);
    strcpy(MySubString, Temp);

    GetArraySize(X, Y); // X is always one row
    NumberOfLines = Y;
    if (StartRowNumber > 0)
        StartRowNumber--; // assume it does not arrive zero based
    X--; // subtract one from X to make it zero based
    for (int i = StartRowNumber; i <= NumberOfLines; i++)
    {
        ReadString(X, i, AString); // zero based
        sprintf(Temp, "%c", AString[0]);
        if (strcmp(Temp, MySubString) == 0) // open square bracket
        {
            sprintf(Temp, "%c", ABlockStartAndEnd[1]);
            sprintf(Temp1, "%c", AString[strlen(AString)-1]); // -1 for last character
            if ( strcmp(Temp,Temp1) == 0 )
            {
                if (StripLeaders)
                {
                    char StrippedString[80];
                    strcpy( StrippedString, "");
                    for (int j = 1; j < strlen(AString) - 1; j++)
                    {
                        sprintf(Temp, "%c", AString[j]);
                        if (j == 1)
                            strcpy(StrippedString, Temp);
                        else
                            strcat(StrippedString, Temp);
                        strcpy(AFullName, StrippedString);
                    }
                }
                else
                    strcpy( AFullName, AString );

                ARowNumber = i+1; // i+1 because function is called one based, not zero based
                return true;
            }
        }
    }
    return false;
}

bool TReadWrite::WriteCell(double ANumber, int Decimals)
{
    char MyNumber[20];
    sprintf(MyNumber, "%*.*f", 5, Decimals, ANumber);
    fputs(MyNumber, MyFilePointer);
    return true;
}

bool TReadWrite::WriteCell(char* AString)
{
    char MyNumber[129];
    strcpy(MyNumber, AString);
    fputs(MyNumber, MyFilePointer);
    return true;
}

void TReadWrite::WriteSeparator(bool EndOfLine)
{
    if (!EndOfLine)
        fputc(TABMARK, MyFilePointer);
    else
        fputc(MY_EOL, MyFilePointer);
}

void TReadWrite::WriteSeparator(char separator)
{
    fputc(separator, MyFilePointer);
}


// AP, 030316...
// This method will read one line from File. If not 'next' read first line
// Returns true if one line is read
//  false otherwise

bool TReadWrite::ReadLine (bool next, char* line)
{
     if (!next)
        rewind(MyFilePointer);

    if (fgets(line, BUFSIZ, MyFilePointer) == NULL)
        return false;
    // end-of-line character is in the buffer
    return true;
}

// This method will write the line
// Returns false if an error occurs
//  true otherwise

bool TReadWrite::WriteLine (char* line)
{
    fputs(line, MyFilePointer);
    return true;
}
// ...AP, 030316

// AP, 030616...
// This method return the index of the first character not space in string "str"
int TReadWrite::trimLeft (char* str)
{
    size_t left = 0;
    while (left < strlen(str)) {
        if (str[left] == ' ')
            left++;
        else
            break;
    }
    return left;
}

// This method return the index of the first trailing space in string "str"
int TReadWrite::trimRight (char* str)
{
    size_t right = strlen(str);
    while (right > 0) {
        if (str[right - 1] == ' ' || str[right-1] == CRETURN)
            right--;
        else
            break;
    }
    return right;
}

// trim returns a new string "dest" removing leading and trailing spaces from "src".
void TReadWrite::trim (char* dest, char* src)
{
    int left = trimLeft(src);
    int right = trimRight(src);
    if (right > left) {
        strncpy(dest, &src[left], (right - left));
        dest[right - left] = '\0';
    }
    else
        dest[0] = '\0';
}

int TReadWrite::GetNumberOfRows()
{
    return nDataRows;
}

int TReadWrite::GetNumberOfColumns()
{
    return nDataColumns;
}


//---------------------------------------------------------------------------
#ifndef _PARSER_H_
#define _PARSER_H_
//---------------------------------------------------------------------------

typedef char TOKEN[1024];  // changed from 64 in 2007.05.15, AP
                          // changed from 768 in 2008.01.23, AP

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export Parser {

  public:		// User declarations

    Parser();

    int getNrOfTokens(char* line, const char* delimiters);
    void parseTokens(char* line, const char* delimiters, TOKEN* tokens);
    int getNrOfNTokens(char* line);
    void parseNumbers(char* line, int* numbers);
    void supressDelimiters(char* line, const char* delimiters,
                                  char* lineResult);
    void replaceDelimiters(char* line, const char* delimiters,
                                  char replacer, char* lineResult);
    bool isNotDelimiter(char character, const char* delimiters);

    // AP, 2007.05.01
    int trimLeft (char* str);
    int trimRight (char* str);
    void trim (char* dest, char* src);

    // AP, 2007.05.23
    int* parseNumbers(char* line, int &nItems);
    void freeNumbers(int* &numbers);
};

#endif // _PARSER_H_


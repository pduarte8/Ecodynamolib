//---------------------------------------------------------------------------

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#pragma hdrstop

	#include <string.h>
#else
	#include <iostream>
	#include <cstring>
	#include <cstdlib>
	
	using namespace std;
#endif
	#include "Parser.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/**
 * Constructor
 */
Parser::Parser()
{
}

/** getNrOfTokens
 *
 * Given 'line' string and 'delimiters' characters returns the number of tokens.
 *
 */
int Parser::getNrOfTokens(char* line, const char* delimiters)
{
    int nd = 0;
    char* str = line;
    char* aux;

    while (true) {
        // find first occurrence of any of the characters in 'delimiters'
        if ((aux = strpbrk(str, delimiters)) == NULL)
            break;

        if ((aux - str) > 0) {
            nd++;
        }
        str = aux + 1;
//        str++;    // increment 'str' to next char
    }
    return (nd + 1);   // number of tokens id 'nrOfDelimiters + 1'
}
//---------------------------------------------------------------------------
/** parseTokens
 *
 * Given 'line' string and 'delimiters' characters saves the array of tokens
 * in 'tokens'.
 *
 * NOTE: 'tokens' must have enough space for all tokens (previousy call the
 *        'GetNrOfTokens(char* line, char* delimiters)' method to dimension
 *        'tokens' array).
 *
 */
void Parser::parseTokens(char* line, const char* delimiters, TOKEN* tokens)
{
    char* str1 = line;
    char* str2;
    int i, k;

    int nrOfTokens = getNrOfTokens(line, delimiters);
    i = 0;
    while (i < nrOfTokens) {
        // find first occurrence of any of the characters in 'delimiters'
        if ((str2 = strpbrk(str1, delimiters)) != NULL) {
            k = str2 - str1;
            if (k > 0) {
                strncpy(tokens[i], str1, k);
                tokens[i][k] = '\0';
                i++;
            }
            str1 = (str2 + 1);
        }
        else {
            if (strlen(str1) > 0) {
                strcpy(tokens[i], str1);
                i++;
            }
            while (i < nrOfTokens) {
                tokens[i][0] = '\0';
                i++;
            }
        }
    }
}
//---------------------------------------------------------------------------
/** GetNrOfNTokens
 *
 * Given 'line' string returns the numbers parsed.
 * The delimiters are comma(,) for individual numbers and minus(-) for ranges.
 */
int Parser::getNrOfNTokens(char* line)
{
    char strA[16];
    char strB[16];
    char* str;
    int i, size, a, b;

    int nrOfTokens = getNrOfTokens(line, ",");
    size = nrOfTokens;
    TOKEN* tokens = new TOKEN[nrOfTokens];
    parseTokens(line, ",", tokens);

    for (i = 0; i < nrOfTokens; i++) {
        if ((str = strpbrk(tokens[i], "-")) != NULL) {
            strncpy(strA, tokens[i], (str - tokens[i]));
            a = atoi(strA);
            strcpy(strB, (str + 1));
            b = atoi(strB);
            if (b > a)
                size += (b - a);
        }
    }
    delete [] tokens;
    return size;
}
//---------------------------------------------------------------------------
/** parseNumbers
 *
 * Given 'line' string fill 'numbers' array with the integers parsed.
 * The delimiters are comma(,) for individual numbers and minus(-) for ranges.
 */
void Parser::parseNumbers(char* line, int* numbers)
{
    char strA[16];
    char strB[16];
    char* str;
    int i, k, a, b;

    int nrOfTokens = getNrOfTokens(line, ",");
    TOKEN* tokens = new TOKEN[nrOfTokens];
    parseTokens(line, ",", tokens);

    k = 0;
    for (i = 0; i < nrOfTokens; i++) {
        if ((str = strpbrk(tokens[i], "-")) == NULL) {
            numbers[k++] = atoi(tokens[i]);
        }
        else {
            strncpy(strA, tokens[i], (str - tokens[i]));
            a = atoi(strA);
            strcpy(strB, (str + 1));
            b = atoi(strB);
            while (a <= b)
                numbers[k++] = a++;
        }
    }
    delete [] tokens;
}
//---------------------------------------------------------------------------
/** supressDelimiters
 *
 * Supress 'delimiters' characters from 'line' string and
 * fills the new string without delimiters.
 *
 */
void Parser::supressDelimiters(char* line, const char* delimiters,
        char* lineResult)
{
    int i;
    int nrOfTokens = getNrOfTokens(line, delimiters);
    TOKEN* tokens = new TOKEN[nrOfTokens];
    parseTokens(line, delimiters, tokens);
    lineResult[0] = '\0';
    for (i = 0; i < nrOfTokens; i++)
        strcat(lineResult, tokens[i]);
    delete [] tokens;
}
//---------------------------------------------------------------------------
/** replaceDelimiters
 *
 * Replace 'delimiters' characters from 'line' string with 'replacer' char
 * and returns the new string with delimiters replaced.
 *
 */
void Parser::replaceDelimiters(char* line, const char* delimiters,
        char replacer, char* lineResult)
{
    char* str;

    strcpy(lineResult, line);
    str = lineResult;
    while (true) {
        // find first occurrence of any of the characters in 'delimiters'
        if ((str = strpbrk(str, delimiters)) == NULL)
            break;
        *str = replacer;
    }
}
//---------------------------------------------------------------------------
/** isNotDelimiter
 *
 * Returns true if 'character' is not one of the 'delimiters' characters,
 *  false otherwise.
 *
 */
bool Parser::isNotDelimiter(char character, const char* delimiters)
{
    int i, k;

    k = strlen(delimiters);
    i = 0;
    while (i < k) {
        if (character == delimiters[i])
            return false;
    }
    return true;
}
//---------------------------------------------------------------------------
// AP, 2007.05.01...
/** trimLeft
 *
 * Returns the index of the first character not space in string "str".
 *
 */
int Parser::trimLeft (char* str)
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
//---------------------------------------------------------------------------
/** trimRight
 *
 * Returns the index of the first trailing space in string "str".
 *
 */
int Parser::trimRight (char* str)
{
    size_t right = strlen(str);
    while (right > 0) {
        if (str[right - 1] == ' ')
            right--;
        else
            break;
    }
    return right;
}
//---------------------------------------------------------------------------
/** trim
 *
 * Fills a new string "dest" removing leading and trailing spaces from "src".
 *
 */
void Parser::trim (char* dest, char* src)
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
//---------------------------------------------------------------------------
// AP, 2007.05.23...
/** parseNumbers
 *
 * Given 'line' string, create an array of integers
 * and return it with the integers parsed.
 * The delimiters are comma(,) for individual numbers and minus(-) for ranges.
 * 'nItems' is updated with the size of the integer array
 */
int* Parser::parseNumbers(char* line, int &nItems)  // return array of integers
{
    nItems = getNrOfNTokens(line);
    int *boxes = new int[nItems];
    parseNumbers(line, boxes);
    return boxes;
}
//---------------------------------------------------------------------------
/** freeNumbers
 *
 * delete array of integers created in a previous call to 'parseNumbers(line)'
 */
void Parser::freeNumbers(int* &numbers)
{
    delete [] numbers;
}
//---------------------------------------------------------------------------


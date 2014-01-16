//---------------------------------------------------------------------------

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
#ifdef __BORLANDC__
	#include <vcl.h>
	#pragma hdrstop

	#include <stdlib.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdlib>
	#include <exception>

	using namespace std;
#endif  // __BORLANDC__
#include "Queue.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

Queue::Queue() {
    pFirst = NULL;
    pLast = NULL;
    n = 0;
}
//---------------------------------------------------------------------------

Queue::~Queue() {
    clear();
}
//---------------------------------------------------------------------------

BUF* Queue::allocBuf(int size) // alloc memory for BUF with size bytes in data
{
  BUF* pBuf;
  BUF* pData;

    pBuf = (BUF*)calloc(1, size + sizeof(BUF));
    if (pBuf != NULL) {
        //memset(pBuf, 0, (size + sizeof(BUF)));
        pData = pBuf + 1;
        pBuf->pData = (long*)pData;
    }
    return pBuf;
}
//---------------------------------------------------------------------------

void Queue::freeBuf(BUF* pBuf)        // free memory buffer
{
    try {
        free(pBuf);
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e)
    {
//        ShowMessage(e.Message+" Queue.freeBuf 1");
    }
    catch (Exception &e) {
//        ShowMessage(e.Message+" Queue.freeBuf 2");
    }
#else  // __BORLANDC__
	catch(exception &e) {
		cerr << "Queue::freeBuf exception " << e.what() << endl;
	}
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------

void Queue::clear()        // clear queue
{
  BUF* pBuf;

    if (n == 0)
        return;
        
    while ((pBuf = removeElementAt(0)) != NULL) {
        freeBuf(pBuf);
    }
    pFirst = NULL;
    pLast = NULL;
    n = 0;
}
//---------------------------------------------------------------------------

int Queue::size()    // get number of queue entries
{
    return n;
}
//---------------------------------------------------------------------------

BUF* Queue::getFirst()    // get first entry from queue
{
    return pFirst;
}
//---------------------------------------------------------------------------

BUF* Queue::getLast()     // get last entry from queue
{
    return pLast;
}
//---------------------------------------------------------------------------

BUF* Queue::getElement(BUF* pElem)    // get entry "*pElem"
{
  BUF* pBuf = getFirst();

    for (int i = 0; i < size(); i++, pBuf = pBuf->pNext) {
        if (pBuf == pElem)
            return pBuf;
    }
    return NULL;
}
//---------------------------------------------------------------------------

bool Queue::insertElement(BUF* pElem) // insert entry in bottom of the queue
{
    if (pElem == NULL)
        return false;

    pElem->pNext = NULL;
    pElem->pPrevious = pLast;
    if (pLast != NULL)
        pLast->pNext = pElem;
    pLast = pElem;
    if (pFirst == NULL)
        pFirst = pLast;
    n++;
    return true;
}
//---------------------------------------------------------------------------

BUF* Queue::removeElement(BUF* pElem) // remove entry from queue
{
  BUF* pBuf;
  int position = 0;

    if ((size() == 0) || (pElem == NULL))
        return NULL;

    pBuf = pFirst;
    while ((pBuf != NULL) && (pBuf != pElem)) {
        pBuf = pBuf->pNext;
        position++;
    }
    if (pBuf == NULL)
        return NULL;

    return removeElementAt(position);
}
//---------------------------------------------------------------------------

/* get entry index for "*pElem"
 * return entry index or -1 on error
 */
int Queue::elementIndex(BUF* pElem)
{
  BUF* pBuf = getFirst();

    for (int i = 0; i < size(); i++, pBuf = pBuf->pNext) {
        if (pBuf == pElem)
            return i;
    }
    return -1;
}
//---------------------------------------------------------------------------

BUF* Queue::getElementAt(int position)    // get entry at position
{
  BUF* pBuf = getFirst();
  int i, m = MIN(position, size() - 1);

    i = 0;
    while (i < m) {
        pBuf = pBuf->pNext;
        i++;
    }
    return pBuf;
}
//---------------------------------------------------------------------------

/* insert entry "*pElem" in the "position"
 * return true; false on error
 */
bool Queue::insertElementAt(int position, BUF* pElem)
{
  BUF* pBuf;
  BUF* pP;

    if (pElem == NULL)
        return false;

    if ((size() == 0) || (size() <= position))  // last element
        return insertElement(pElem);

    pBuf = getElementAt(position);
    pP = pBuf->pPrevious;
    if (pP == NULL)  // first element
        pFirst = pElem;
    else
        pP->pNext = pElem;
    pElem->pNext = pBuf;
    pBuf->pPrevious = pElem;
    pElem->pPrevious = pP;
    n++;
    return true;
}
//---------------------------------------------------------------------------

BUF* Queue::removeElementAt(int position) // remove "position"th entry
{
  BUF* pBuf;
  BUF* pP;
  BUF* pN;

    if ((position < 0) || (size() <= position))  // out of range
        return NULL;

    pBuf = getElementAt(position);
    pP = pBuf->pPrevious;
    pN = pBuf->pNext;
    if (pP == NULL)  // first element
        pFirst = pN;
    else
        pP->pNext = pN;
    if (pN == NULL)  // last element
        pLast = pP;
    else
        pN->pPrevious = pP;
    pBuf->pNext = NULL;
    pBuf->pPrevious = NULL;
    n--;
    return pBuf;
}
//---------------------------------------------------------------------------

bool Queue::deleteElement(BUF* pElem) // delete entry from queue
{
  BUF* pBuf = removeElement(pElem);

    if (pBuf == NULL)   // element doesn't exist
        return false;

    freeBuf(pBuf);
    return true;
}
//---------------------------------------------------------------------------

bool Queue::deleteElementAt(int position) // delete "position"th entry
{
  BUF* pBuf = removeElementAt(position);

    if (pBuf == NULL)  // out of range
        return false;

    freeBuf(pBuf);
    return true;
}
//---------------------------------------------------------------------------
/*
 * returns true if object pointer inserted; false otherwise
 */
bool Queue::InsertObject(TObject* pObj, bool top)
{
  BUF* pBuf;

    if ((pBuf = allocBuf(sizeof(TObject*))) != NULL) {
        /*(TObject*)*/(*(pBuf->pData)) = (long)pObj;

        if (top)
            insertElementAt(0, pBuf);
        else
            insertElement(pBuf);
//        insertElementAt((top ? 0 : size()), pBuf);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------
TObject* Queue::RetrieveObject(int i)
{
    return (TObject*)(*(getElementAt(i)->pData));
}
//---------------------------------------------------------------------------
TObject* Queue::RemoveObject(int i)
{
    BUF* pBuf = removeElementAt(i);
    TObject* pObject = (TObject*)(*(pBuf->pData));
    freeBuf(pBuf);
    return pObject;
}
//---------------------------------------------------------------------------


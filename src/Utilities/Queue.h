//---------------------------------------------------------------------------
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "globals.h"
//---------------------------------------------------------------------------

#ifndef __BORLANDC__
#define _export 
#endif  // __BORLANDC__

class _export BUF {
  public:		// User declarations
    BUF* pNext;
    BUF* pPrevious;
    long* pData;
};

class _export Queue {
  public:		// User declarations
    Queue();
    ~Queue();
    
    static BUF* allocBuf(int size);   // alloc memory for BUF with size bytes in data
    static void freeBuf(BUF* pBuf);   // free memory for BUF 'pBuf'

    int size();     // get number of queue entries
    void clear();       // clear queue
    BUF* getFirst();    // get first entry
    BUF* getLast();     // get last entry
    BUF* getElement(BUF* pElem);    // get entry "*pElem"
    bool insertElement(BUF* pElem); // insert entry in bottom of the queue
    BUF* removeElement(BUF* pElem); // remove entry "*pElem"
    int elementIndex(BUF* pElem);    // get entry index for "*pElem"
    BUF* getElementAt(int position);    // get "position"th entry
    bool insertElementAt(int position, BUF* pElem); // insert "*pElem" in the "position"
    BUF* removeElementAt(int position); // remove "position"th entry
    bool deleteElement(BUF* pElem);    // delete entry "*pElem"
    bool deleteElementAt(int position);   // delete "position"th element

    bool InsertObject(TObject* pObj, bool top=false);  // insert object pointer
    TObject* RetrieveObject(int i);     // retrieve object pointer
    TObject* RemoveObject(int i);       // remove object pointer

  private:
    BUF* pFirst;
    BUF* pLast;
    int n;
};

//---------------------------------------------------------------------------
#endif //_QUEUE_H_

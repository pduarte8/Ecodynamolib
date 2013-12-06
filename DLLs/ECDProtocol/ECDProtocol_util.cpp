//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_util.cpp
 *
 *  PRIVATE utlity methods invoked by ECDProtocol class:
 *
 *      void saveMessageTx(BUF* pBuf)
 *      bool parseReceiverName(char* message, char* name);
 *      Queue* duplicateElementQueue(Queue* pElementQueue, int elementSize);
 *      void buildSimpleAction(MSG_ID* msg, int type);
 *      void buildActionResult(MSG_ID* msg, int type, int result);
 *      void buildVarClassResult(MSG_ID* msg, Queue* pNames, int code);
 *      int getNextNotEmptyToken(TOKEN* tokensT, int nT, int index);
 *      void appendEnclosedName(char* message, char* name);
 *      void appendSubDomain(char* message, R_DOMAIN* pDomain);
 *      void appendCells(char* message, Queue* pCells);
 *      void buildSubDomain(TOKEN* tokens, int n, int index, R_DOMAIN* pDomain);
 *
 *  added in 2005.11.04 (AP):
 *      bool hasMessages(Queue* pQ);
 *      MESSAGE_ECDP* getMessage(Queue* pQ);
 *      void freeMessage(Queue* pQ);
 *
 *  added in 2007.02.07 (AP):
 *      void replaceParenthesesByBrackets(char* destination, char* source);
 *      void replaceBracketsByParentheses(char* destination, char* source);
 *               *
 *  added in 2007.05.07 (AP):
 *      void buildBoxValues(TOKEN* tokens, int n, int index, Queue* pQueue);

 *  added in 2007.05.11 (AP):
 *      void deleteQueue(Queue* pQueue);
 */
//---------------------------------------------------------------------------

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#ifdef __BORLANDC__
	#include <vcl.h>
	#pragma hdrstop

	#include <stdio.h>
	#include <string.h>
#else  // __BORLANDC__
	#include <iostream>
	#include <cstdio>
	#include <cstring>
	#include <cstdlib>
	#include <exception>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"
#include "Queue.h"

//---------------------------------------------------------------------------
/*
 *  NAME saveMessageTx
 *
 *  Insert message in the transmission queue
 */

void EcoDynProtocol::saveMessageTx(BUF* pBuf)
{
    if (pMsgQueueTx != NULL) {
#ifdef __BORLANDC__
        pTxCriticalSection->Acquire();
        pMsgQueueTx->insertElement(pBuf);
        pTxCriticalSection->Release();
#else  // __BORLANDC__
		pthread_mutex_lock(&TxCriticalSection);
        pMsgQueueTx->insertElement(pBuf);
		pthread_mutex_unlock(&TxCriticalSection);
#endif  // __BORLANDC__
    }
}
//---------------------------------------------------------------------------
/*
 *  NAME parseReceiverName
 *
 *  Get message destination name
 */

bool EcoDynProtocol::parseReceiverName(char* message, char* name)
{
    bool rc = false;
    int n = myParser->getNrOfTokens(message, "( )");
    TOKEN* tokens = new TOKEN[n];
    myParser->parseTokens(message, "( )", tokens);
    if (n > 3) {
        strcpy(name, tokens[3]);
        rc = true;
    }
    delete [] tokens;
    return rc;
}
//---------------------------------------------------------------------------
/*
 *  NAME duplicateElementQueue
 *
 *  Duplicate a Queue containing elements, each one with 'elementSize' bytes
 */

Queue* EcoDynProtocol::duplicateElementQueue(Queue* pElementQueue, int elementSize)
{
    Queue* pNew;
    BUF* pBufNew;
    char* pElementNew;

    BUF* pBuf;
    char* pElement;
    int index;

    pNew = new Queue();
    index = 0;
    while (index < pElementQueue->size()) {
        pBuf = pElementQueue->getElementAt(index);
        pElement = (char*)pBuf->pData;

        if ((pBufNew = Queue::allocBuf(elementSize)) == NULL) {
            break;
        }
        pElementNew = (char*)pBufNew->pData;
        memcpy(pElementNew, pElement, elementSize);
        pNew->insertElement(pBufNew);
        index++;
    }
    return pNew;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSimpleAction
 *
 *  Simple action is an action with no data, only 'type'.
 *  Build a MESSAGE_ECDP structure only with 'type' message filled.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildSimpleAction(char* destination, int type)
{
    MESSAGE_ECDP* msgECDP_action;
    BUF* pBuf = allocMessageECDP(msgECDP_action);

    msgECDP_action->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_action->messageId.sender, myAgent->agentName);
    strcpy(msgECDP_action->messageId.receiver, destination);
    msgECDP_action->msgContent.type = type;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildActionResult
 *
 *  Build "ACTION_RESULT" in a MESSAGE_ECDP structure with 'type' and 'result'.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildActionResult(MSG_ID* msg, int type, int result)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = type;
    msgECDP_answer->msgContent.content.perception.result.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.result.actionResult = result;

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME buildVarClassResult
 *
 *  Build "VARCLASS_RESULT" in a MESSAGE_ECDP structure.
 *  Insert message into MsgQueueTX.
 *
 *  Returns number of message built.
 */

int EcoDynProtocol::buildVarClassResult(MSG_ID* msg, Queue* pNames, int code)
{
    MESSAGE_ECDP* msgECDP_answer;
    BUF* pBuf = allocMessageECDP(msgECDP_answer);

    msgECDP_answer->messageId.idMsg = ++msgCount;
    strcpy(msgECDP_answer->messageId.sender, msg->receiver);
    strcpy(msgECDP_answer->messageId.receiver, msg->sender);
    msgECDP_answer->msgContent.type = code;
    msgECDP_answer->msgContent.content.perception.varClassResult.id = msg->idMsg;
    msgECDP_answer->msgContent.content.perception.varClassResult.pQueue =
            duplicateElementQueue(pNames, ECDP_STRING);

    saveMessageTx(pBuf);
    return msgCount;
}
//---------------------------------------------------------------------------
/*
 *  NAME getNextNotEmptyToken
 *
 *  Returns index of the next not empty token
 *  'tokensT' - array of tokens
 *  'nT' - number of tokens
 *  'index' - index of first token to inspect
 */

int EcoDynProtocol::getNextNotEmptyToken(TOKEN* tokensT, int nT, int index)
{
    int ni;
    TOKEN tokenString;

    ni = index;
    while (ni < nT) {
        myParser->trim(tokenString, tokensT[ni]);
        if (strlen(tokenString) > 0) {
            break;
        }
        ni++;
    }
    return ni;
}
//---------------------------------------------------------------------------
/*
 *  NAME appendEnclosedName
 *
 *  Append "(name)" into "message"
 */

void EcoDynProtocol::appendEnclosedName(char* message, char* name)
{
    sprintf(&message[strlen(message)], " (%s)", name);
}
//---------------------------------------------------------------------------
/*
 *  NAME appendSubDomain
 *
 *  Append R_DOMAIN structure data into "message"
 */

void EcoDynProtocol::appendSubDomain(char* message, R_DOMAIN* pDomain)
{
    BUF* pBufName;
    char* pName;
    int i;

    if (pDomain->type == DOM_TYPE_ALL) {
        strcat(message, "subdomain all");
    }
    else {
        strcat(message, "subdomain (");
        for (i = 0; i < pDomain->pRegNames->size(); i++) {
            pBufName = pDomain->pRegNames->getElementAt(i);
            pName = (char*)pBufName->pData;
            sprintf(&message[strlen(message)], "%s ", pName);
        }
        strcat(message, ")");
    }
}
//---------------------------------------------------------------------------
/*
 *  NAME appendCells
 *
 *  Append Cells data into "message"
 */

void EcoDynProtocol::appendCells(char* message, Queue* pCells)
{
    BUF* pBufInt;
    int* pInt;
    int i;

    for (i = 0; i < pCells->size(); i++) {
        pBufInt = pCells->getElementAt(i);
        pInt = (int*)pBufInt->pData;
        sprintf(&message[strlen(message)], "%i ", *pInt);
    }
}
//---------------------------------------------------------------------------
/*
 *  NAME buildSubDomain
 *
 *  Build a R_DOMAIN structure with 'subdomain' definition.
 */

void EcoDynProtocol::buildSubDomain(TOKEN* tokens, int n, int index,
        R_DOMAIN* pDomain)
{
    int i;
    BUF* pBufName;
    char* pName;

    if (strcmp(tokens[index], "all") == 0) {
        pDomain->type = DOM_TYPE_ALL;
        pDomain->pRegNames = NULL;
    }
    else {
        pDomain->type = DOM_TYPE_REG;
        pDomain->pRegNames = new Queue();

        i = index;
        while (i < n) {
            if (strlen(tokens[i]) > 0) {
                if ((pBufName = Queue::allocBuf(ECDP_STRING)) == NULL) {
                    break;
                }
                pName = (char*)pBufName->pData;
                strcpy(pName, tokens[i]);
                pDomain->pRegNames->insertElement(pBufName);
            }
            i++;
        }
    }
}
//---------------------------------------------------------------------------
/*
 *  NAME hasMessages
 *
 *  Returns 'true' if agent has messages in queue 'pQ'.
 */

bool EcoDynProtocol::hasMessages(Queue* pQ)
{
    if (pQ == NULL)
        return false;

    return (pQ->size() > 0);
}
//---------------------------------------------------------------------------
/*
 *  NAME getMessage
 *
 *  Returns a pointer to the oldest message in queue 'pQ'.
 */

MESSAGE_ECDP* EcoDynProtocol::getMessage(Queue* pQ)
{
    if (pQ == NULL)
        return NULL;

    BUF* pBuf = pQ->getFirst();
    if (pBuf == NULL) {
        return NULL;
    }

    return (MESSAGE_ECDP*)pBuf->pData;
}
//---------------------------------------------------------------------------
/*
 *  NAME freeMessage
 *
 *  Free the oldest message in queue 'pQ' and the memory structures it used.
 */

void EcoDynProtocol::freeMessage(Queue* pQ)
{
    MESSAGE_ECDP* pMsg = getMessage(pQ);
    if (pMsg != NULL) {
        freeMsgECDP(pMsg);
        pQ->deleteElementAt(0);
    }
}
//---------------------------------------------------------------------------
/*
 *  NAME replaceParenthesesByBrackets
 *
 *  Replace parentheses "()" in 'source' by brackets "[]" in 'destination'.
 */

void EcoDynProtocol::replaceParenthesesByBrackets(char* destination, char* source)
{
    char temp[ECDP_STRING];

    myParser->replaceDelimiters(source, "(", '[', temp);
    myParser->replaceDelimiters(temp, ")", ']', destination);
}
//---------------------------------------------------------------------------
/*
 *  NAME replaceBracketsByParentheses
 *
 *  Replace brackets "[]" in 'source' by parentheses "()" in 'destination'.
 */

void EcoDynProtocol::replaceBracketsByParentheses(char* destination, char* source)
{
    char temp[ECDP_STRING];

    myParser->replaceDelimiters(source, "[", '(', temp);
    myParser->replaceDelimiters(temp, "]", ')', destination);
}
//---------------------------------------------------------------------------
/*
 *  NAME buildBoxValues
 *
 *  Build a Queue with BOX_VALUE structures with 'box_value' definitions.
 */

void EcoDynProtocol::buildBoxValues(TOKEN* tokens, int n, int index,
        Queue* pQueue)
{
  int ni;
  BUF* pBuf;
  BOX_VALUE* pBoxValue;

    /* every element has 2 tokens */
    ni = index;
    while (ni < n - 1)
    {
        if (strlen(tokens[ni]) > 0) {
            if ((pBuf = Queue::allocBuf(sizeof(BOX_VALUE))) == NULL) {
                break;
            }
            pBoxValue = (BOX_VALUE*)pBuf->pData;
            pBoxValue->cell = atoi(tokens[ni]);
            pBoxValue->value = atof(tokens[ni + 1]);
            pQueue->insertElement(pBuf);
        }
        ni += 2;
    }
}
//---------------------------------------------------------------------------
/*
 *  NAME deleteQueue
 *
 *  delete Queue object
 */

void EcoDynProtocol::deleteQueue(Queue* &pQueue)
{
    try {
        delete pQueue;
        pQueue = NULL;
    }
#ifdef __BORLANDC__
    catch (EAccessViolation &e)
    {
        //ShowMessage(e.Message+"\n deleteQueue 1");
    }
    catch (Exception &exception) {
        //ShowMessage(exception.Message+"\n deleteQueue 2");
    }
#else  // __BORLANDC__
    catch (exception &e) {
            cerr << "EcoDynProtocol::deleteQueue " << e.what() << endl;
    }
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------


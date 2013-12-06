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
	#include <cstring>
	#include <cstdlib>
	#include <cstdio>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//    EcoDynProtocol class - PUBLIC methods

//---------------------------------------------------------------------------
/*
 *  NAME EcoDynProtocol
 *
 *  Default constructor
 */

EcoDynProtocol::EcoDynProtocol()
{
    ownerSrv=NULL;
    myAgent=NULL;
    pMsgQueueRx=NULL;
    pMsgQueueTx=NULL;
    pAgentsQueue=NULL;
    msgCount = 0;
    myParser = new Parser();

#ifdef __BORLANDC__
    pRxCriticalSection = new TCriticalSection();
    pTxCriticalSection = new TCriticalSection();
    pAgentsCriticalSection = new TCriticalSection();
#else  // __BORLANDC__
    RxCriticalSection = PTHREAD_MUTEX_INITIALIZER;
    TxCriticalSection = PTHREAD_MUTEX_INITIALIZER;
    AgentsCriticalSection = PTHREAD_MUTEX_INITIALIZER;
#endif  // __BORLANDC__
    contentString = new char[ECDP_MSG_LENGTH];
};
//---------------------------------------------------------------------------
/*
 *  NAME EcoDynProtocol
 *
 *  Constructor: 'Server' - application that owns the ECDProtocol object
 *               'agentName' - name of the agent represented by this object
 *               'hostname' - computer name where agent belongs
 *               'hostname' - computer name where agent belongs
 *               'hostaddr' - computer address where agent belongs
 *               'portServer' - agent port that accepts connections
 */

EcoDynProtocol::EcoDynProtocol(TObject* Server, char* agentName, char* hostname,
        char* hostaddr, int portServer)
{
  BUF* pBuf;

    ownerSrv = Server;
    pMsgQueueRx = new Queue();
    pMsgQueueTx = new Queue();
    pAgentsQueue = new Queue();
    myParser = new Parser();

#ifdef __BORLANDC__
    pRxCriticalSection = new TCriticalSection();
    pTxCriticalSection = new TCriticalSection();
    pAgentsCriticalSection = new TCriticalSection();
#else  // __BORLANDC__
    RxCriticalSection = PTHREAD_MUTEX_INITIALIZER;
    TxCriticalSection = PTHREAD_MUTEX_INITIALIZER;
    AgentsCriticalSection = PTHREAD_MUTEX_INITIALIZER;
#endif  // __BORLANDC__
    contentString = new char[ECDP_MSG_LENGTH];

    // build first entry of AGENT_TABLE queue
    if ((pBuf = Queue::allocBuf(sizeof(AGENT_TABLE))) != NULL) {
        myAgent = (AGENT_TABLE*)pBuf->pData;

        strcpy(myAgent->agentName, agentName);
        strcpy(myAgent->hostname, hostname);
        strcpy(myAgent->hostaddr, hostaddr);
        myAgent->portServer = portServer;
        myAgent->connected = true;
        myAgent->id = 0;

#ifdef __BORLANDC__
        pAgentsCriticalSection->Acquire();
        pAgentsQueue->insertElement(pBuf);
        pAgentsCriticalSection->Release();
#else  // __BORLANDC__
		pthread_mutex_lock(&AgentsCriticalSection);
        pAgentsQueue->insertElement(pBuf);
		pthread_mutex_unlock(&AgentsCriticalSection);
#endif  // __BORLANDC__
    }

    nConnections = 0;
    msgCount = 0;
}
//---------------------------------------------------------------------------
/*
 *  NAME addAgent
 *
 *  Add agent to the agents list - pAgentsQueue:
 *      'agentName' - name of the agent
 *      'hostname' - computer name where agent belongs
 *      'hostname' - computer name where agent belongs
 *      'hostaddr' - computer address where agent belongs
 *      'portServer' - agent port that accepts connections
 *  Set Agent ID and returns true if agent included; false otherwise.
 */

bool EcoDynProtocol::addAgent(char* agentName, char* hostname,
        char* hostaddr, int portServer)
{
  BUF* pBuf;
  AGENT_TABLE* pAgent;

    pAgent = getAgentTableByName(agentName);
    if (pAgent != NULL)
    {
        if (pAgent->connected) {
            if ((strcmp(pAgent->hostname, hostname) == 0) &&
                    (strcmp(pAgent->hostaddr, hostaddr) == 0) &&
                    pAgent->portServer == portServer)
                return true;
            else
                return false;
        }

        strcpy(pAgent->hostname, hostname);
        strcpy(pAgent->hostaddr, hostaddr);
        pAgent->portServer = portServer;
        pAgent->connected = true;
        return true;
    }

    // build one entry for AGENT_TABLE queue
    if ((pBuf = Queue::allocBuf(sizeof(AGENT_TABLE))) == NULL) {
        return false;
    }

    pAgent = (AGENT_TABLE*)pBuf->pData;

    strcpy(pAgent->agentName, agentName);
    strcpy(pAgent->hostname, hostname);
    strcpy(pAgent->hostaddr, hostaddr);
    pAgent->portServer = portServer;
    pAgent->connected = true;

    for (int i = 1; i < MAX_AGENTS; i++) {
        if (getAgentTableById(i) == NULL)
        {
            pAgent->id = i;
            break;
        }
    }

#ifdef __BORLANDC__
    pAgentsCriticalSection->Acquire();
    pAgentsQueue->insertElement(pBuf);
    pAgentsCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&AgentsCriticalSection);
    pAgentsQueue->insertElement(pBuf);
	pthread_mutex_unlock(&AgentsCriticalSection);
#endif  // __BORLANDC__
    ++nConnections;
    return true;
}
//---------------------------------------------------------------------------
/*
 *  NAME EcoDynProtocol
 *
 *  Remove agent from the agents list - pAgentsQueue:
 *      'agentName' - name of the agent
 *  If agent is found retruns true; otherwise returns false.
 */

bool EcoDynProtocol::removeAgent(char* agentName)
{
  BUF *pBuf;
  AGENT_TABLE* pTable;
  int i;

    for (i = 0; i < getNrOfAgents(); i++) {
        pBuf = pAgentsQueue->getElementAt(i);
        pTable = (AGENT_TABLE*)pBuf->pData;
    	if (strcmp(pTable->agentName, agentName) == 0) {

#ifdef __BORLANDC__
            pAgentsCriticalSection->Acquire();
            pBuf = pAgentsQueue->removeElementAt(i);
            Queue::freeBuf(pBuf);
            pAgentsCriticalSection->Release();
#else  // __BORLANDC__
			pthread_mutex_lock(&AgentsCriticalSection);
            pBuf = pAgentsQueue->removeElementAt(i);
            Queue::freeBuf(pBuf);
			pthread_mutex_unlock(&AgentsCriticalSection);
#endif  // __BORLANDC__

        	return true;
        }
    }
    return false;      // agent entry not found
}
//---------------------------------------------------------------------------
/*
 *  NAME getAgentTableByName
 *
 *  Returns a pointer to the 'agentName' agent table (AGENT_TABLE)
 */

AGENT_TABLE* EcoDynProtocol::getAgentTableByName(char* agentName)
{
  BUF *pBuf;
  AGENT_TABLE* pTable;
  int i;

    for (i = 0; i < getNrOfAgents(); i++) {
        pBuf = pAgentsQueue->getElementAt(i);
        pTable = (AGENT_TABLE*)pBuf->pData;
    	if (strcmp(pTable->agentName, agentName) == 0) {
        	return pTable;
        }
    }
    return NULL;      // return null entry
}
//---------------------------------------------------------------------------
/*
 *  NAME getAgentTableById
 *
 *  Returns a pointer to the 'agentId' agent table (AGENT_TABLE)
 */

AGENT_TABLE* EcoDynProtocol::getAgentTableById(int agentId)
{
  BUF *pBuf;
  AGENT_TABLE* pTable;
  int i;
  int listSize = getNrOfAgents();

    for (i = 0; i < listSize; i++) {
        pBuf = pAgentsQueue->getElementAt(i);
        pTable = (AGENT_TABLE*)pBuf->pData;
    	if (pTable->id == agentId) {
        	return pTable;
        }
    }
    return NULL;      // return null entry
}
//---------------------------------------------------------------------------
/*
 *  NAME getAgentTableAtPosition
 *
 *  Returns a pointer to the agent table (AGENT_TABLE) at 'position'
 */

AGENT_TABLE* EcoDynProtocol::getAgentTableAtPosition(int position)
{
  BUF *pBuf;

    if (position >= getNrOfAgents())
        return NULL;

    pBuf = pAgentsQueue->getElementAt(position);
    return (AGENT_TABLE*)pBuf->pData;
}
//---------------------------------------------------------------------------
/*
 *  NAME getNrOfAgents
 *
 *  Static method: returns the number of agents defined
 */

int EcoDynProtocol::getNrOfAgents()
{
    return (pAgentsQueue->size());
}
//---------------------------------------------------------------------------
/*
 *  NAME getAgentTableIndex
 *
 *  Returns the table index of 'agentName' agent
 */

int EcoDynProtocol::getAgentIndex(char* agentName)
{
    AGENT_TABLE* pAgent = getAgentTableByName(agentName);
    if (pAgent == NULL)
        return -1;

    return pAgent->id;
}
//---------------------------------------------------------------------------
/*
 *  NAME prepareMessage
 *
 *  Fills 'at' with the pointer of the receiver agent referred in the 'message'.
 *  'message' is already the text message.
 *  Returns 'true' if agent exists; 'false' otherwise.
 */

bool EcoDynProtocol::prepareMessage(char* message, AGENT_TABLE* &at)
{
    bool rc = false;
    char receiver[ECDP_STRING];
    if (parseReceiverName(message, receiver)) {
        at = getAgentTableByName(receiver);
        rc = true;
    }
    return rc;
}
//---------------------------------------------------------------------------
/*
 *  NAME prepareMessage
 *
 *  Fills 'message' with the text message saved in 'msgECDP' and 'at' with
 *   the pointer of the receiver agent referred in the 'message'.
 *  Returns 'true' if msgECDP is consistent; 'false' otherwise.
 */

bool EcoDynProtocol::prepareMessage(MESSAGE_ECDP* msgECDP, AGENT_TABLE* &at,
        char* message)
{
    formatMessage(msgECDP, message);
    if (strlen(message) < 12)      // is this a good value ???
        return false;

    MSG_ID msgId = msgECDP->messageId;
    at = getAgentTableByName(msgId.receiver);
    return true;
}
//---------------------------------------------------------------------------
/*
 *  NAME hasMessagesRx
 *
 *  Returns 'true' if agent has messages received not yet processed.
 */

bool EcoDynProtocol::hasMessagesRx()
{
  bool notEmpty;

#ifdef __BORLANDC__
    pRxCriticalSection->Acquire();
    notEmpty = hasMessages(pMsgQueueRx);
    pRxCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&RxCriticalSection);
    notEmpty = hasMessages(pMsgQueueRx);
	pthread_mutex_unlock(&RxCriticalSection);
#endif  // __BORLANDC__
    return notEmpty;
}
//---------------------------------------------------------------------------
/*
 *  NAME getMessageRx
 *
 *  Returns a pointer to the oldest received message.
 */

MESSAGE_ECDP* EcoDynProtocol::getMessageRx()
{
  MESSAGE_ECDP* pMsg;

#ifdef __BORLANDC__
    pRxCriticalSection->Acquire();
    pMsg = getMessage(pMsgQueueRx);
    pRxCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&RxCriticalSection);
    pMsg = getMessage(pMsgQueueRx);
	pthread_mutex_unlock(&RxCriticalSection);
#endif  // __BORLANDC__
   return pMsg;
}
//---------------------------------------------------------------------------
/*
 *  NAME freeMessageRx
 *
 *  Free the memory structures of the oldest received message.
 */

void EcoDynProtocol::freeMessageRx()
{
#ifdef __BORLANDC__
    pRxCriticalSection->Acquire();
    freeMessage(pMsgQueueRx);
    pRxCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&RxCriticalSection);
    freeMessage(pMsgQueueRx);
	pthread_mutex_unlock(&RxCriticalSection);
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------
/*
 *  NAME hasMessagesTx
 *
 *  Returns 'true' if agent has messages to transmit.
 */

bool EcoDynProtocol::hasMessagesTx()
{
  bool notEmpty;

#ifdef __BORLANDC__
    pTxCriticalSection->Acquire();
    notEmpty = hasMessages(pMsgQueueTx);
    pTxCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&TxCriticalSection);
    notEmpty = hasMessages(pMsgQueueTx);
	pthread_mutex_unlock(&TxCriticalSection);
#endif  // __BORLANDC__
    return notEmpty;
}
//---------------------------------------------------------------------------
/*
 *  NAME getMessageTx
 *
 *  Returns a pointer to the oldest message to transmit.
 */

MESSAGE_ECDP* EcoDynProtocol::getMessageTx()
{
  MESSAGE_ECDP* pMsg;

#ifdef __BORLANDC__
    pTxCriticalSection->Acquire();
    pMsg = getMessage(pMsgQueueTx);
    pTxCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&TxCriticalSection);
    pMsg = getMessage(pMsgQueueTx);
	pthread_mutex_unlock(&TxCriticalSection);
#endif  // __BORLANDC__
    return pMsg;
}
//---------------------------------------------------------------------------
/*
 *  NAME freeMessageTx
 *
 *  Free the memory structures of the oldest message to transmit.
 */

void EcoDynProtocol::freeMessageTx()
{
#ifdef __BORLANDC__
    pTxCriticalSection->Acquire();
    freeMessage(pMsgQueueTx);
    pTxCriticalSection->Release();
#else  // __BORLANDC__
	pthread_mutex_lock(&TxCriticalSection);
    freeMessage(pMsgQueueTx);
	pthread_mutex_unlock(&TxCriticalSection);
#endif  // __BORLANDC__
}
//---------------------------------------------------------------------------
/*
 *  NAME allocMessageECDP
 *
 *  Fills 'ecdp' with the pointer of the allocated memory for one MESSAGE_ECDP
 *  structure.
 *  Returns the 'BUF' pointer that contains 'ecdp' message.
 */

BUF* EcoDynProtocol::allocMessageECDP(MESSAGE_ECDP* &ecdp)
{
    BUF* pBuf = Queue::allocBuf(sizeof(MESSAGE_ECDP));
    if (pBuf != NULL) {
        ecdp = (MESSAGE_ECDP*)pBuf->pData;
    }
    return pBuf;
}
//---------------------------------------------------------------------------
/*
 *  NAME allocMessageECDP_Rx
 *
 *  Fills 'ecdp' with the pointer of the allocated memory for one MESSAGE_ECDP
 *  structure.
 *  Returns the 'BUF' pointer that contains 'ecdp' message.
 */

BUF* EcoDynProtocol::allocMessageECDP_Rx(MESSAGE_ECDP* &ecdp)
{
    BUF* pBuf = Queue::allocBuf(sizeof(MESSAGE_ECDP));
    if (pBuf != NULL) {
        ecdp = (MESSAGE_ECDP*)pBuf->pData;
    }
    return pBuf;
}
//---------------------------------------------------------------------------
/*
 * NAME insertECDPString
 *
 * insert ECDP String element in 'pQueue'
 * 'pQueue' - Queue entry for ECDP Strings
 * 'pStr' - ECDP string
 * returns true if memory allocated; false otherwise
 */

bool EcoDynProtocol::insertECDPString(Queue* pQueue, char* pStr)
{
    BUF* pBuf;

    if ((pBuf = Queue::allocBuf(ECDP_STRING)) != NULL) {
        strcpy((char*)pBuf->pData, pStr);
        pQueue->insertElement(pBuf);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------


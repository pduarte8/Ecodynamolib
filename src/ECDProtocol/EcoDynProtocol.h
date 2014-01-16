//---------------------------------------------------------------------------
#ifndef _ECODYNPROTOCOL_H_
#define _ECODYNPROTOCOL_H_
//---------------------------------------------------------------------------

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */
 
#ifdef __BORLANDC__
	#include <syncobjs.hpp>
#else  // __BORLANDC__
	#include <pthread.h>
#endif  // __BORLANDC__

#include "ECDPMessages.h"
#include "ECDPAgents.h"
#include "Parser.h"
#include "Region.h"

// definition needed to GNU C++ compiler in Linux
// 16 October 2013, AP
#ifndef __BORLANDC__
    #ifndef stricmp
        #define stricmp strcasecmp
    #endif  // stricmp
#endif  // !__BORLANDC__

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
	class _export EcoDynProtocol
#else  // __BORLANDC__
	class EcoDynProtocol
#endif  // __BORLANDC__
{
  public:		// User declarations

    // ECDProtocol.cpp
    EcoDynProtocol();
    EcoDynProtocol(TObject* Server, char* agentName, char* hostname,
            char* hostaddr, int portServer);

    bool addAgent(char* agentName, char* hostname, char* hostaddr,
            int portServer);
    bool removeAgent(char* agentName);

    AGENT_TABLE* getAgentTableByName(char* agentName);
    AGENT_TABLE* getAgentTableById(int agentId);
    AGENT_TABLE* getAgentTableAtPosition(int position);
    AGENT_TABLE* getAgent()  {return myAgent;};

    int getNrOfAgents();
    int getAgentIndex(char* agentName);

    bool prepareMessage(MESSAGE_ECDP* msgECDP, AGENT_TABLE* &at, char* message);
    bool prepareMessage(char* message, AGENT_TABLE* &at);

    bool hasMessagesRx();
    MESSAGE_ECDP* getMessageRx();
    void freeMessageRx();

    bool hasMessagesTx();
    MESSAGE_ECDP* getMessageTx();
    void freeMessageTx();

    BUF* allocMessageECDP(MESSAGE_ECDP* &ecdp);     // AP, 2005.09.23
    BUF* allocMessageECDP_Rx(MESSAGE_ECDP* &ecdp);     // AP, 2006.05.24
    bool insertECDPString(Queue* pQueue, char* pStr);   // 2006.01.18

    // ECDProtocol_Custom_get.cpp
    void saveMessageRx(char* sMsg);

    // ECDProtocol_Custom_Public_1.cpp
    int buildDefinitionAction(char* destination, char* regName,
            REGION* pRegion);
    int buildSeedAction(char* destination, char* regionName,
            ECOTIME* pTime, BIVALVE_S* pBivalve_s, double density);
    int buildInspectAction(char* destination, char* regionName,
            ECOTIME* pTime);
    int buildHarvestAction(char* destination, char* regionName,
            ECOTIME* pTime, BIVALVE* pBivalve);
    int buildSeedResult(MSG_ID* msg, int result);
    int buildInspectResult(MSG_ID* msg, Queue* pBivalveQueue);
    int buildHarvestResult(MSG_ID* msg, int result, double weight);
    // added in 2007.01.30 by AP
    int buildDeleteAction(char* destination, Queue* pRegNames);
    int buildDefineResult(MSG_ID* msg, int result);
    int buildDeleteResult(MSG_ID* msg, int result);

    // ECDProtocol_Custom_Public_2.cpp
    int buildOpenModelAction(char* destination, char* modelName);
    int buildCloseModelAction(char* destination);
    int buildModelNameAction(char* destination);
    int buildInitialiseAction(char* destination);
    int buildRunAction(char* destination);
    int buildStopAction(char* destination);
    int buildStepAction(char* destination, int nSteps);
    int buildPauseAction(char* destination);
    int buildOpenResult(MSG_ID* msg, int result);
    int buildCloseResult(MSG_ID* msg, int result);
    int buildModelNameResult(MSG_ID* msg, char* modelName);
    int buildExecResult(MSG_ID* msg, int result);
    // added in 2007.07.03 by AP
    int buildSaveConfigurationAction(char* destination);
    int buildSaveResult(MSG_ID* msg, int result);

    // ECDProtocol_Custom_Public_3.cpp
    int buildGetAvailableClassesAction(char* destination);
    int buildGetSelectedClassesAction(char* destination);
    int buildSelectClassesAction(char* destination, Queue* pClassNames);
    int buildGetVariablesAction(char* destination, char* className);
    int buildGetVariableValueAction(char* destination, char* className,
            char* varName, BOXES* pBoxes);
    int buildSetVariableValueAction(char* destination, char* className,
            Queue* pVars);
    int buildGetParametersAction(char* destination, char* className);
    int buildSetParametersAction(char* destination, char* className,
            Queue* pParms);
    int buildGetTimeSpecAction(char* destination);
    int buildSetTimeSpecAction(char* destination, int step, int startT,
            int finishT);
    int buildSubDomainAction(char* destination, R_DOMAIN* pDomain);
    int buildClassesAvailableResult(MSG_ID* msg, Queue* pClassNames);
    int buildClassesSelectedResult(MSG_ID* msg, Queue* pClassNames);
    int buildVariablesResult(MSG_ID* msg, Queue* pVarNames);
    int buildVariableValueResult(MSG_ID* msg, Queue* pValues);
    int buildVariableSetResult(MSG_ID* msg, int result);
    int buildParametersValuesResult(MSG_ID* msg, Queue* pParms);
    int buildParametersSetResult(MSG_ID* msg, int result);
    int buildTimeSpecResult(MSG_ID* msg, int step, long startT, long finishT);
    int buildSubDomainResult(MSG_ID* msg, int result);

    // ECDProtocol_Custom_Public_4.cpp
    int buildOutputFileAction(char* destination, char* filename);
    int buildGetAvailableVariablesAction(char* destination);
    int buildSelectVariablesAction(char* destination, int type,
            Queue* pVarNames, BOXES* pBoxes);
    int buildLogAction(char* destination, LOGTYPE logType, Queue* pSteps);
    int buildGetOutputTimeAction(char* destination);
    int buildSetOutputTimeAction(char* destination, int step, long startT,
            long finishT);
    int buildTraceAction(char* destination);
    int buildOutputFileResult(MSG_ID* msg, int result);
    int buildVariablesAvailableResult(MSG_ID* msg, Queue* pVarNames);
    int buildSelectVariablesResult(MSG_ID* msg, int result);
    int buildLogResult(MSG_ID* msg, int result);
    int buildOutputTimeResult(MSG_ID* msg, int step, long startT, long finishT);
    int buildTraceResult(MSG_ID* msg, int traceStatus);
    int buildRegisterMsg(char* destination, int regIndex, int regTime,
            char* varName, Queue* pValues);
    int buildLoggerMsg(char* destination, int step, Queue* pLogs);
    int buildEndSimulation(char* destination);
    int buildEndStep(char* destination);
    // added in 2007.01.30 by AP
    int buildUnSelectVariablesAction(char* destination, int type,
            Queue* pVarNames);
    int buildUnSelectVariablesResult(MSG_ID* msg, int result);
    // added in 2007.02.27 by AP
    int buildRunningMsg(char* destination);
    int buildStoppedMsg(char* destination);
    int buildPausedMsg(char* destination);

    // ECDProtocol_Custom_Public_5.cpp
    int buildConnectAction(char* destination, char* hostname,
            char* hostaddr, int portServer);
    int buildDisconnectAction(char* destination);
    int buildAcceptResult(MSG_ID* msg, int result);
    // added in 2007.05.03 by AP
    int buildGetRegionNamesAction(char* destination);
    int buildGetRegionAction(char* destination, char* regName);
    int buildModelDimensionsAction(char* destination);
    int buildModelMorphologyAction(char* destination);
    int buildModelSpeciesAction(char* destination);
    int buildRegionNamesResult(MSG_ID* msg, Queue* pRegNames);
    int buildRegionResult(MSG_ID* msg, char* regName, REGION* pRegion);
    int buildDimensionsResult(MSG_ID* msg, int lines, int columns,
            int layers, char* modelType);
    int buildMorphologyResult(MSG_ID* msg, Queue* pValues);
    int buildBenthicSpeciesResult(MSG_ID* msg, Queue* pSpecies);
    // added in 2007.05.15 by AP
    int buildMorphologyEndResult(MSG_ID* msg);
    int buildBenthicSpeciesEndResult(MSG_ID* msg);
    // added in 2008.05.21 by AP
    int buildAgentsAction(char* destination);
    int buildKnownAgentsResult(MSG_ID* msg, Queue* pKnownAgents);

  private:      // User declarations

    TObject* ownerSrv;
    AGENT_TABLE* myAgent;
    Queue* pAgentsQueue;        // AGENT_TABLE elements
    Queue* pMsgQueueRx;
    Queue* pMsgQueueTx;
    Queue* pMsgQueueWaitingAnswer;
    int msgCount;
    int nConnections;
    Parser* myParser;

    char* contentString;    // buffer to old formatted message

#ifdef __BORLANDC__
    TCriticalSection* pRxCriticalSection;
    TCriticalSection* pTxCriticalSection;
    TCriticalSection* pAgentsCriticalSection;
#else  // __BORLANDC__
    pthread_mutex_t RxCriticalSection;
    pthread_mutex_t TxCriticalSection;
    pthread_mutex_t AgentsCriticalSection;
#endif  // __BORLANDC__

    // ECDProtocol_util.cpp
    void saveMessageTx(BUF* pBuf);
    bool parseReceiverName(char* message, char* name);
    Queue* duplicateElementQueue(Queue* pElementQueue, int elementSize);
    int buildSimpleAction(char* destination, int type);
    int buildActionResult(MSG_ID* msg, int type, int result);
    int getNextNotEmptyToken(TOKEN* tokensT, int nT, int index);
    void appendEnclosedName(char* message, char* name);
    void appendSubDomain(char* message, R_DOMAIN* pDomain);
    void appendCells(char* message, Queue* pCells);
    int buildVarClassResult(MSG_ID* msg, Queue* pNames, int code);
    void buildSubDomain(TOKEN* tokens, int n, int index, R_DOMAIN* pDomain);
    // added in 2005.11.04 by AP
    bool hasMessages(Queue* pQ);
    MESSAGE_ECDP* getMessage(Queue* pQ);
    void freeMessage(Queue* pQ);
    // added in 2007.02.07 by AP
    void replaceParenthesesByBrackets(char* destination, char* source);
    void replaceBracketsByParentheses(char* destination, char* source);
    // added in 2007.05.07 by AP
    void buildBoxValues(TOKEN* tokens, int n, int index, Queue* pQueue);
    // added in 2007.05.11 by AP
    void deleteQueue(Queue* &pQueue);

    // ECDProtocol_Custom_free.cpp
    void freeMsgECDP(MESSAGE_ECDP* msgECDP);

    // ECDProtocol_Custom_fmt.cpp
    void formatMessage(MESSAGE_ECDP* msgECDP, char* message);

    // ECDProtocol_fmt_1.cpp
    void fmtDefinitionMsg(DEFINITION_MSG* definition, char* contentStr);
    void fmtSeedAction(SEED_ACTION* seed, char* contentStr);
    void fmtInspectAction(INSPECT_ACTION* inspect, char* contentStr);
    void fmtHarvestAction(HARVEST_ACTION* harvest, char* contentStr);
    void fmtActionResult(ACTION_RESULT* aResult, const char* resultText,
            char* contentStr);
    void fmtSeedResult(ACTION_RESULT* seedResult, char* contentStr);
    void fmtInspectResult(INSPECT_RESULT* inspectResult, char* contentStr);
    void fmtHarvestResult(HARVEST_RESULT* harvestResult, char* contentStr);
    void fmtBivalve(BIVALVE* bivalve, char* contentStr);
    void fmtBivalve_S(BIVALVE_S* bivalve_s, char* contentStr);
    void fmtRegion(REGION* region, char* contentStr);
    void fmtSubRegion(SUB_REGION* subRegion, char* contentStr);
    void fmtWaterRegion(WATER_REGION* waterRegion, char* contentStr);
    void fmtSedimentCaract(SEDIMENT_CARACT* sCaract, char* contentStr);
    void fmtSimpleRegion(SIMPLE_REGION* simpleRegion, char* contentStr);
    void fmtEcoArc(ECOARC* arc, char* contentStr);
    void fmtEcoCircle(ECOCIRCLE* circle, char* contentStr);
    void fmtEcoSquare(ECOSQUARE* square, char* contentStr);
    void fmtEcoRect(ECORECT* rect, char* contentStr);
    void fmtEcoPoint(ECOPOINT* point, char* contentStr);
    void fmtEcoTime(ECOTIME* ecotime, char* contentStr);
    // added in 2007.01.30 by AP
    void fmtDeleteAction(SPECS_ACTION* deleteAction, char* contentStr);
    void fmtDefineResult(ACTION_RESULT* defineResult, char* contentStr);
    void fmtDeleteResult(ACTION_RESULT* deleteResult, char* contentStr);

    // ECDProtocol_get_1.cpp
    void getDefinitionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSeedActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getInspectActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getHarvestActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getActionResultData(int code, TOKEN* tokens, int index, MSG_CONTENT* msgContent);
    void getSeedResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getInspectResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getHarvestResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getBivalves(TOKEN* tokens, int n, int index, Queue* pBivalveQueue);
    void getBivalve(TOKEN* tokens, int n, int index, BIVALVE* pBivalve);
    void getBivalve_S(TOKEN* tokens, int n, int index, BIVALVE_S* pBivalve_s);
    void getRegion(TOKEN* tokens, int n, int index, REGION* pRegion);
    void getSubRegion(TOKEN* tokens, int n, int index, SUB_REGION* pSubRegion);
    void getRegionNames(TOKEN* tokens, int n, int index, Queue* pRegNames);
    void getRegionArea(TOKEN* tokens, int n, int index, Queue* pRegionArea);
    void getEcoPoint(TOKEN* tokens, int n, int index, ECOPOINT* pPoint);
    void getEcoRect(TOKEN* tokens, int n, int index, ECORECT* pRect);
    void getEcoSquare(TOKEN* tokens, int n, int index, ECOSQUARE* psquare);
    void getEcoCircle(TOKEN* tokens, int n, int index, ECOCIRCLE* pCircle);
    void getEcoArc(TOKEN* tokens, int n, int index, ECOARC* pArc);
    void getEcoTime(TOKEN* tokens, int n, int index, ECOTIME* pTime);
    // added in 2007.01.30 by AP
    void getDeleteActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getDefineResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getDeleteResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);

    // ECDProtocol_fmt_2.cpp
    void fmtOpenModelAction(MODEL_ACTION* modelAction, char* contentStr);
    void fmtCloseModelAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtModelNameAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtInitialiseAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtRunAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtStopAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtStepAction(STEP_ACTION* stepAction, char* contentStr);
    void fmtPauseAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtOpenResult(ACTION_RESULT* openResult, char* contentStr);
    void fmtCloseResult(ACTION_RESULT* closeResult, char* contentStr);
    void fmtModelResult(MODEL_RESULT* modelResult, char* contentStr);
    void fmtExecResult(ACTION_RESULT* execResult, char* contentStr);
    // added in 2007.07.03 by AP
    void fmtSaveConfigurationAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtSaveResult(ACTION_RESULT* saveResult, char* contentStr);

    // ECDProtocol_get_2.cpp
    void getOpenModelActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getCloseModelActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getModelNameActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getInitialiseActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getRunActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getStopActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getStepActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getPauseActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getOpenModelResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getCloseModelResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getModelNameResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getExecResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    // added in 2007.07.03 by AP
    void getSaveConfigurationActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSaveResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);

    // ECDProtocol_fmt_3.cpp
    void fmtGetAvailableClassesAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtGetSelectedClassesAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtSelectClassesAction(SPECS_ACTION* specsAction, char* contentStr);
    void fmtGetVariablesAction(CLASS_ACTION* clAction, char* contentStr);
    void fmtGetVariableValueAction(VAR_VALUE_ACTION* varValueAction, char* contentStr);
    void fmtSetVariableValueAction(SET_VALUE_ACTION* setValueAction, char* contentStr);
    void fmtGetParametersAction(CLASS_ACTION* clAction, char* contentStr);
    void fmtSetParametersAction(SET_VALUE_ACTION* setValueAction, char* contentStr);
    void fmtGetTimeSpecAction(MSG_CONTENT* msgContent, char* contentStr);
    void fmtSetTimeSpecAction(TIME_SPEC_ACTION* timeSpecAction, char* contentStr);
    void fmtSubDomainAction(SUBDOMAIN_ACTION* subDomainAction, char* contentStr);
    void fmtClassesAvailableResult(VARCLASS_RESULT* clsResult, char* contentStr);
    void fmtClassesSelectedResult(VARCLASS_RESULT* clsResult, char* contentStr);
    void fmtVariablesResult(VARCLASS_RESULT* varsResult, char* contentStr);
    void fmtVariableValueResult(VAR_VALUE_RESULT* varValueResult, char* contentStr);
    void fmtVariableSetResult(ACTION_RESULT* specsResult, char* contentStr);
    void fmtParametersValuesResult(PARMS_RESULT* parmsResult, char* contentStr);
    void fmtParametersSetResult(ACTION_RESULT* specsResult, char* contentStr);
    void fmtTimeSpecResult(TIME_SPEC_RESULT* timeSpecResult, char* contentStr);
    void fmtSubDomainResult(ACTION_RESULT* specsResult, char* contentStr);

    // ECDProtocol_get_3.cpp
    void getGetAvailableClassesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetSelectedClassesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSelectClassesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetVariablesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetVariableValueActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSetVariableValueActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetParametersActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSetParametersActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetTimeSpecActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSetTimeSpecActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSubDomainActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getClassesAvailableResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getClassesSelectedResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getVariablesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getVariableValueResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getVariableSetResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getParametersValuesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getParametersSetResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getTimeSpecResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSubDomainResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);

    // ECDProtocol_fmt_4.cpp
    void fmtOutputFileAction(OUTFILE_ACTION* outfileAction, char* contentStr);
    void fmtGetAvailableVariablesAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtSelectVariablesAction(SEL_VAR_ACTION* selVarAction,  char* contentStr);
    void fmtLogAction(LOG_ACTION* logAction,  char* contentStr);
    void fmtGetOutputTimeAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtSetOutputTimeAction(TIME_SPEC_ACTION* outputTimeAction,  char* contentStr);
    void fmtTraceAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtOutputFileResult(ACTION_RESULT* regResult,  char* contentStr);
    void fmtVariablesAvailableResult(VARCLASS_RESULT* varsResult, char* contentStr);
    void fmtSelectVariablesResult(ACTION_RESULT* regResult,  char* contentStr);
    void fmtLogResult(ACTION_RESULT* regResult,  char* contentStr);
    void fmtOutputTimeResult(TIME_SPEC_RESULT* timeSpecResult,  char* contentStr);
    void fmtTraceResult(ACTION_RESULT* traceResult,  char* contentStr);
    void fmtRegisterMsg(REGISTER_MSG* registerMsg,  char* contentStr);
    void fmtLoggerMsg(LOGGER_MSG* loggerMsg,  char* contentStr);
    void fmtEndSimulationMsg(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtEndStepMsg(MSG_CONTENT* msgContent,  char* contentStr);
    // added in 2007.01.30 by AP
    void fmtUnSelectVariablesAction(UNSEL_VAR_ACTION* unSelVarAction,  char* contentStr);
    void fmtUnSelectVariablesResult(ACTION_RESULT* regResult,  char* contentStr);
    // added in 2007.02.27 by AP
    void fmtRunningMsg(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtStoppedMsg(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtPausedMsg(MSG_CONTENT* msgContent,  char* contentStr);

    // ECDProtocol_get_4.cpp
    void getOutputFileActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetAvailableVariablesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSelectVariablesActionData(char* type, TOKEN* tokensT,
            int nT, int index, MSG_CONTENT* msgContent);
    void getLogActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetOutputTimeActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSetOutputTimeActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getTraceActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getOutputFileResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getVariablesAvailableResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getSelectVariablesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getLogResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getOutputTimeResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getTraceResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getRegisterMsgData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getLoggerMsgData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getEndSimulationData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getEndStepData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    // added in 2007.01.30 by AP
    void getUnSelectVariablesActionData(char* type, TOKEN* tokensT,
            int nT, int index, MSG_CONTENT* msgContent);
    void getUnSelectVariablesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    // added in 2007.02.27 by AP
    void getRunningData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getStoppedData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getPausedData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);

    // ECDProtocol_fmt_5.cpp
    void fmtConnectAction(CONNECT_ACTION* connectAction, char* contentStr);
    void fmtDisconnectAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtAcceptResult(ACTION_RESULT* acceptResult,  char* contentStr);
    // added in 2007.05.07 by AP
    void fmtGetRegionNamesAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtGetRegionAction(REGION_ACTION* regAction,  char* contentStr);
    void fmtModelDimensionsAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtModelMorphologyAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtModelSpeciesAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtRegionNamesResult(REGION_NAMES_RESULT* regNamesResult,  char* contentStr);
    void fmtRegionResult(REGION_RESULT* regionResult,  char* contentStr);
    void fmtDimensionsResult(DIMENSIONS_RESULT* dimensionsResult,  char* contentStr);
    void fmtMorphologyResult(MORPHOLOGY_RESULT* morphologyResult,  char* contentStr);
    void fmtBenthicSpeciesResult(BENTHIC_SPECIES_RESULT* benthicSpeciesResult,  char* contentStr);
    void fmtMorphologyEndResult(ACTION_RESULT* endResult, char* contentStr);
    void fmtBenthicSpeciesEndResult(ACTION_RESULT* endResult, char* contentStr);
    // added in 2008.05.21 by AP
    void fmtAgentsAction(MSG_CONTENT* msgContent,  char* contentStr);
    void fmtKnownAgentsResult(AGENTS_RESULT* agentsResult,  char* contentStr);

    // ECDProtocol_get_5.cpp
    void getConnectActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getDisconnectActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getAcceptResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    // added in 2007.05.07 by AP
    void getGetRegionNamesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getGetRegionActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getModelDimensionsActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getModelMorphologyActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getModelSpeciesActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getRegionNamesResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getRegionResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getDimensionsResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getMorphologyResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getBenthicSpeciesResultData(TOKEN* tokensT, int nT, int index, MSG_CONTENT* msgContent);
    void getMorphologyEndResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getBenthicSpeciesEndResultData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    // added in 2008.05.21 by AP
    void getAgentsActionData(TOKEN* tokens, int n, int index, MSG_CONTENT* msgContent);
    void getKnownAgentsResultData(TOKEN* tokensT, int nT, int index, MSG_CONTENT* msgContent);

};
#endif //_ECODYNPROTOCOL_H_

//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_get.cpp
 *
 *  PUBLIC method invoked by Agents
 *
 *  Every time a new message is defined some code must be included HERE:
 *      PUBLIC void saveMessageRx(char* message)
 *
 *  and in:
 *      PRIVATE void formatMessage(MESSAGE_ECDP* msgECDP, char* message)
 *                                                 ::ECDProtocol_Custom_fmt.cpp
 *      PRIVATE void freeMsgECDP(MESSAGE_ECDP* msgECDP) ::ECDProtocol_Custom_free.cpp
 *
 *
 *  EcoDynamo Protocol v0.4 - 2006.05.16 (AP)
 *    included connect, disconnect and accept messages.
 *
 *  EcoDynamo Protocol v0.4b - 2006.07.04 (AP)
 *    included end_step and end_simulation messages.
 *
 *  ECOLANG - EcoDynamo Protocol v1.0 - 2007.01.30 (AP)
 *    included messages: delete, define_result, delete_result, unselect_variables
 *          and unselect_variables_result.
 *
 *  ECOLANG - EcoDynamo Protocol v1.1 - 2007.02.27 (AP)
 *   Included new messages running, stopped and paused.
 *
 *  ECOLANG - EcoDynamo Protocol v1.2 - 2007.03.12 (AP)
 *   Message 'register' modified.
 *
 *  ECOLANG - EcoDynamo Protocol v1.3 - 2007.05.07 (AP)
 *    included new messages:  get_region_names, get_region, region_names,
 *         region, model_dimensiona, model_morphology, model_species,
 *         dimensions, morphology, benthic_species.
 *   v1.3a
 *    2007.05.15 (AP) included messages: morphology_end, benthic_species_end
 *   v1.3b
 *    2007.07.03 (AP) included messages: save_configuration, save_result
 *   v1.3c
 *    2008.05.21 (AP) included messages: agents, known_agents
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
	#include <cstdlib>
	#include <cstring>
	#include <cstdio>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//---------------------------------------------------------------------------
/*
 *  NAME saveMessageRx
 *
 *  This method parses received messages:
 *   1. confirms "message" directive
 *   2. get ID, sender and receiver
 *   3. get message type and calls <type> method processing
 */

void EcoDynProtocol::saveMessageRx(char* message)
{
    int nT;             // used for tokens delimited only by parenthesis
    TOKEN* tokensT;     // used for tokens delimited only by parenthesis

    int n = myParser->getNrOfTokens(message, "( )");
    if (n < 5)
        return;

    TOKEN* tokens = new TOKEN[n];
    myParser->parseTokens(message, "( )", tokens);

    if (stricmp(tokens[0], "message") != 0)
    {
        delete [] tokens;
        return;
    }

    MESSAGE_ECDP* ecdp;
    BUF* pBuf = allocMessageECDP_Rx(ecdp);

    ecdp->messageId.idMsg = atoi(tokens[1]);
    strcpy(ecdp->messageId.sender, tokens[2]);
    strcpy(ecdp->messageId.receiver, tokens[3]);

    /*
     * BULK 1
     */
    if (stricmp(tokens[4], "define") == 0)
        getDefinitionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "seed") == 0)
        getSeedActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "inspect") == 0)
        getInspectActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "harvest") == 0)
        getHarvestActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "seed_result") == 0)
        getSeedResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "inspect_result") == 0)
        getInspectResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "harvest_result") == 0)
        getHarvestResultData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2007.01.30
    else if (stricmp(tokens[4], "define_result") == 0)
        getDefineResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "delete") == 0)
        getDeleteActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "delete_result") == 0)
        getDeleteResultData(tokens, n, 5, &ecdp->msgContent);

    /*
     * AP, 2005.05.04 - BULK 2
     */
    else if (stricmp(tokens[4], "open_model") == 0)
        getOpenModelActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "close_model") == 0)
        getCloseModelActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "model_name") == 0)
        getModelNameActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "initialise") == 0)
        getInitialiseActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "run") == 0)
        getRunActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "stop") == 0)
        getStopActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "step") == 0)
        getStepActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "pause") == 0)
        getPauseActionData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2005.06.07
    else if (stricmp(tokens[4], "open_result") == 0)
        getOpenModelResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "close_result") == 0)
        getCloseModelResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "model") == 0)
        getModelNameResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "exec_result") == 0)
        getExecResultData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2007.07.03
    else if (stricmp(tokens[4], "save_configuration") == 0)
        getSaveConfigurationActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "save_result") == 0)
        getSaveResultData(tokens, n, 5, &ecdp->msgContent);

    /*
     * AP, 2005.06.20 - BULK 3
     */
    else if (stricmp(tokens[4], "get_available_classes") == 0)
        getGetAvailableClassesActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "get_selected_classes") == 0)
        getGetSelectedClassesActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "select_classes") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getSelectClassesActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "get_variables") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getGetVariablesActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "get_variable_value") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 5)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getGetVariableValueActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "set_variable_value") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 6)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getSetVariableValueActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "get_parameters") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getGetParametersActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "set_parameters") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 5)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getSetParametersActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "get_time_spec") == 0)
        getGetTimeSpecActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "set_time_spec") == 0)
        getSetTimeSpecActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "subdomain") == 0)
        getSubDomainActionData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2005.06.22
    else if (stricmp(tokens[4], "classes_available") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 4)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getClassesAvailableResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "classes_selected") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 4)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getClassesSelectedResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "variables") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 4)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getVariablesResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "variable_value") == 0)
        getVariableValueResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "variable_set_result") == 0)
        getVariableSetResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "parameters_values") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 4)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getParametersValuesResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "parameters_set_result") == 0)
        getParametersSetResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "time_spec") == 0)
        getTimeSpecResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "subdomain_result") == 0)
        getSubDomainResultData(tokens, n, 5, &ecdp->msgContent);

    /*
     * AP, 2005.06.27 - BULK 4
     */
    else if (stricmp(tokens[4], "output_file") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getOutputFileActionData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "get_available_variables") == 0)
        getGetAvailableVariablesActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "select_variables") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 4)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getSelectVariablesActionData(tokens[5], tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "log") == 0)
        getLogActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "get_output_time") == 0)
        getGetOutputTimeActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "set_output_time") == 0)
        getSetOutputTimeActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "trace") == 0)
        getTraceActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "output_file_result") == 0)
        getOutputFileResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "variables_available") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getVariablesAvailableResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "select_variables_result") == 0)
        getSelectVariablesResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "log_result") == 0)
        getLogResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "output_time") == 0)
        getOutputTimeResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "trace_result") == 0)
        getTraceResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "register") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 5)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getRegisterMsgData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "logger") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getLoggerMsgData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    // AP, 2006.07.04
    else if (stricmp(tokens[4], "end_step") == 0)
        getEndStepData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "end_simulation") == 0)
        getEndSimulationData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2007.01.30
    else if (stricmp(tokens[4], "unselect_variables") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 4)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getUnSelectVariablesActionData(tokens[5], tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    else if (stricmp(tokens[4], "unselect_variables_result") == 0)
        getUnSelectVariablesResultData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2007.02.27
    else if (stricmp(tokens[4], "running") == 0)
        getRunningData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "stopped") == 0)
        getStoppedData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "paused") == 0)
        getPausedData(tokens, n, 5, &ecdp->msgContent);

    /*
     * AP, 2006.05.16 - BULK 5
     */
    else if (stricmp(tokens[4], "connect") == 0)
        getConnectActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "disconnect") == 0)
        getDisconnectActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "accept") == 0)
        getAcceptResultData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2007.05.07
    else if (stricmp(tokens[4], "get_region_names") == 0)
        getGetRegionNamesActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "get_region") == 0)
        getGetRegionActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "region_names") == 0)
        getRegionNamesResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "region") == 0)
        getRegionResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "model_dimensions") == 0)
        getModelDimensionsActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "model_morphology") == 0)
        getModelMorphologyActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "model_species") == 0)
        getModelSpeciesActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "dimensions") == 0)
        getDimensionsResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "morphology") == 0)
        getMorphologyResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "benthic_species") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getBenthicSpeciesResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }
    // AP, 2007.05.15
    else if (stricmp(tokens[4], "morphology_end") == 0)
        getMorphologyEndResultData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "benthic_species_end") == 0)
        getBenthicSpeciesEndResultData(tokens, n, 5, &ecdp->msgContent);
    // AP, 2008.05.21
    else if (stricmp(tokens[4], "agents") == 0)
        getAgentsActionData(tokens, n, 5, &ecdp->msgContent);
    else if (stricmp(tokens[4], "known_agents") == 0)
    {
        nT = myParser->getNrOfTokens(message, "()");
        if (nT < 3)
            ecdp->msgContent.type = INVALID_CODE;
        else {
            tokensT = new TOKEN[nT];
            myParser->parseTokens(message, "()", tokensT);

            getKnownAgentsResultData(tokensT, nT, 2, &ecdp->msgContent);
            delete [] tokensT;
        }
    }

    /* AP - ADD CODE HERE...
     */

    if (pMsgQueueRx != NULL) {
#ifdef __BORLANDC__
        pRxCriticalSection->Acquire();
        pMsgQueueRx->insertElement(pBuf);
        pRxCriticalSection->Release();
#else  // __BORLANDC__
		pthread_mutex_lock(&RxCriticalSection);
        pMsgQueueRx->insertElement(pBuf);
		pthread_mutex_unlock(&RxCriticalSection);
#endif  // __BORLANDC__
    }

    delete [] tokens;
}
//---------------------------------------------------------------------------


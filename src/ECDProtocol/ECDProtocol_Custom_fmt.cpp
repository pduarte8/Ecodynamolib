//---------------------------------------------------------------------------
/*
 *  FILENAME: ECDProtocol_Custom_fmt.cpp
 *
 *  PRIVATE method invoked by 'prepareMessage' (ECDProtocol.cpp)
 *
 *  Every time a new message is defined some code must be included HERE:
 *      PRIVATE void formatMessage(MESSAGE_ECDP* msgECDP, char* message)
 *
 *  and in:
 *      PUBLIC void saveMessageRx(char* message)        ::ECDProtocol_Custom_get.cpp
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
 *  ECOLANG - EcoDynamo Protocol v1.3 - 2007.05.07 (AP)
 *    included new messages:  get_region_names, get_region, region_names,
 *         region, model_dimensions, model_morphology, model_species,
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
	#include <cstdio>
	#include <cstring>
	
	using namespace std;
#endif  // __BORLANDC__
#include "EcoDynProtocol.h"

//---------------------------------------------------------------------------
/*
 *  NAME formatMessage
 *
 *  This method fills 'message' string according MESSAGE_ECDP data.
 */
 
void EcoDynProtocol::formatMessage(MESSAGE_ECDP* msgECDP, char* message)
{
  MSG_CONTENT msgContent = msgECDP->msgContent;
  
    switch (msgContent.type) {
      case DEFINE_CODE:
        fmtDefinitionMsg(&msgContent.content.definition, contentString);
        break;

      case SEED_ACTION_CODE:
        fmtSeedAction(&msgContent.content.action.seed, contentString);
        break;

      case INSPECT_ACTION_CODE:
        fmtInspectAction(&msgContent.content.action.inspect, contentString);
        break;

      case HARVEST_ACTION_CODE:
        fmtHarvestAction(&msgContent.content.action.harvest, contentString);
        break;

      case SEED_RESULT_CODE:
        fmtSeedResult(&msgContent.content.perception.result, contentString);
        break;

      case INSPECT_RESULT_CODE:
        fmtInspectResult(&msgContent.content.perception.inspectResult, contentString);
        break;

      case HARVEST_RESULT_CODE:
        fmtHarvestResult(&msgContent.content.perception.harvestResult, contentString);
        break;

      // AP, 2005.05.04
      case OPEN_MODEL_CODE:
        fmtOpenModelAction(&msgContent.content.action.model, contentString);
        break;

      case CLOSE_MODEL_CODE:
        fmtCloseModelAction(&msgContent, contentString);
        break;

      case MODEL_NAME_CODE:
        fmtModelNameAction(&msgContent, contentString);
        break;

      case INITIALISE_CODE:
        fmtInitialiseAction(&msgContent, contentString);
        break;

      case RUN_CODE:
        fmtRunAction(&msgContent, contentString);
        break;

      case STOP_CODE:
        fmtStopAction(&msgContent, contentString);
        break;

      case STEP_CODE:
        fmtStepAction(&msgContent.content.action.steps, contentString);
        break;

      case PAUSE_CODE:
        fmtPauseAction(&msgContent, contentString);
        break;

      // AP, 2005.06.07
      case OPEN_RESULT_CODE:
        fmtOpenResult(&msgContent.content.perception.result, contentString);
        break;

      case CLOSE_RESULT_CODE:
        fmtCloseResult(&msgContent.content.perception.result, contentString);
        break;

      case MODEL_CODE:
        fmtModelResult(&msgContent.content.perception.modelResult, contentString);
        break;

      case EXEC_RESULT_CODE:
        fmtExecResult(&msgContent.content.perception.result, contentString);
        break;

      // AP, 2005.06.25
      case GET_AVAILABLE_CLASSES_CODE:
        fmtGetAvailableClassesAction(&msgContent, contentString);
        break;

      case GET_SELECTED_CLASSES_CODE:
        fmtGetSelectedClassesAction(&msgContent, contentString);
        break;

      case SELECT_CLASSES_CODE:
        fmtSelectClassesAction(&msgContent.content.action.specs, contentString);
        break;

      case GET_VARIABLES_CODE:
        fmtGetVariablesAction(&msgContent.content.action.clAction, contentString);
        break;

      case GET_VARIABLE_VALUE_CODE:
        fmtGetVariableValueAction(&msgContent.content.action.varValue, contentString);
        break;

      case SET_VARIABLE_VALUE_CODE:
        fmtSetVariableValueAction(&msgContent.content.action.setValue, contentString);
        break;

      case GET_PARAMETERS_CODE:
        fmtGetParametersAction(&msgContent.content.action.clAction, contentString);
        break;

      case SET_PARAMETERS_CODE:
        fmtSetParametersAction(&msgContent.content.action.setValue, contentString);
        break;

      case GET_TIME_SPEC_CODE:
        fmtGetTimeSpecAction(&msgContent, contentString);
        break;

      case SET_TIME_SPEC_CODE:
        fmtSetTimeSpecAction(&msgContent.content.action.timeSpec, contentString);
        break;

      case SUBDOMAIN_CODE:
        fmtSubDomainAction(&msgContent.content.action.subDomain, contentString);
        break;

      case CLASSES_AVAILABLE_CODE:
        fmtClassesAvailableResult(&msgContent.content.perception.varClassResult, contentString);
        break;

      case CLASSES_SELECTED_CODE:
        fmtClassesSelectedResult(&msgContent.content.perception.varClassResult, contentString);
        break;

      case VARIABLES_CODE:
        fmtVariablesResult(&msgContent.content.perception.varClassResult, contentString);
        break;

      case VARIABLE_VALUE_CODE:
        fmtVariableValueResult(&msgContent.content.perception.varValueResult, contentString);
        break;

      case VARIABLE_SET_RESULT_CODE:
        fmtVariableSetResult(&msgContent.content.perception.result, contentString);
        break;

      case PARAMETERS_VALUES_CODE:
        fmtParametersValuesResult(&msgContent.content.perception.parmsResult, contentString);
        break;

      case PARAMETERS_SET_RESULT_CODE:
        fmtParametersSetResult(&msgContent.content.perception.result, contentString);
        break;

      case TIME_SPEC_CODE:
        fmtTimeSpecResult(&msgContent.content.perception.timeSpecResult, contentString);
        break;

      case SUBDOMAIN_RESULT_CODE:
        fmtSubDomainResult(&msgContent.content.perception.result, contentString);
        break;

      // AP, 2005.06.27
      case OUTPUT_FILE_CODE:
        fmtOutputFileAction(&msgContent.content.action.outFile, contentString);
        break;

      case GET_AVAILABLE_VARIABLES_CODE:
        fmtGetAvailableVariablesAction(&msgContent, contentString);
        break;

      case SELECT_VARIABLES_CODE:
        fmtSelectVariablesAction(&msgContent.content.action.selVarAction, contentString);
        break;

      case LOG_CODE:
        fmtLogAction(&msgContent.content.action.logAction, contentString);
        break;

      case GET_OUTPUT_TIME_CODE:
        fmtGetOutputTimeAction(&msgContent, contentString);
        break;

      case SET_OUTPUT_TIME_CODE:
        fmtSetOutputTimeAction(&msgContent.content.action.timeSpec, contentString);
        break;

      case TRACE_CODE:
        fmtTraceAction(&msgContent, contentString);
        break;

      case OUTPUT_FILE_RESULT_CODE:
        fmtOutputFileResult(&msgContent.content.perception.result, contentString);
        break;

      case VARIABLES_AVAILABLE_CODE:
        fmtVariablesAvailableResult(&msgContent.content.perception.varClassResult,
                contentString);
        break;

      case SELECT_VARIABLES_RESULT_CODE:
        fmtSelectVariablesResult(&msgContent.content.perception.result, contentString);
        break;

      case LOG_RESULT_CODE:
        fmtLogResult(&msgContent.content.perception.result, contentString);
        break;

      case OUTPUT_TIME_CODE:
        fmtOutputTimeResult(&msgContent.content.perception.timeSpecResult, contentString);
        break;

      case TRACE_RESULT_CODE:
        fmtTraceResult(&msgContent.content.perception.result, contentString);
        break;

      case REGISTER_CODE:
        fmtRegisterMsg(&msgContent.content.perception.registerMsg, contentString);
        break;

      case LOGGER_CODE:
        fmtLoggerMsg(&msgContent.content.perception.loggerMsg, contentString);
        break;

      // AP, 2006.05.16
      case CONNECT_CODE:
        fmtConnectAction(&msgContent.content.action.connectAction, contentString);
        break;

      case DISCONNECT_CODE:
        fmtDisconnectAction(&msgContent, contentString);
        break;

      case ACCEPT_CODE:
        fmtAcceptResult(&msgContent.content.perception.result, contentString);
        break;

      // AP, 2006.07.04
      case END_SIMULATION_CODE:
        fmtEndSimulationMsg(&msgContent, contentString);
        break;

      case END_STEP_CODE:
        fmtEndStepMsg(&msgContent, contentString);
        break;

      // AP, 2007.01.30
      case DEFINE_RESULT_CODE:
        fmtDefineResult(&msgContent.content.perception.result, contentString);
        break;

      case DELETE_ACTION_CODE:
        fmtDeleteAction(&msgContent.content.action.specs, contentString);
        break;

      case DELETE_RESULT_CODE:
        fmtDeleteResult(&msgContent.content.perception.result, contentString);
        break;

      case UNSELECT_VARIABLES_CODE:
        fmtUnSelectVariablesAction(&msgContent.content.action.unSelVarAction,
                contentString);
        break;

      case UNSELECT_VARIABLES_RESULT_CODE:
        fmtUnSelectVariablesResult(&msgContent.content.perception.result,
                contentString);
        break;

      // AP, 2007.02.27
      case RUNNING_CODE:
        fmtRunningMsg(&msgContent, contentString);
        break;

      case STOPPED_CODE:
        fmtStoppedMsg(&msgContent, contentString);
        break;

      case PAUSED_CODE:
        fmtPausedMsg(&msgContent, contentString);
        break;

      // AP, 2007.05.07
      case REGION_NAMES_ACTION_CODE:
        fmtGetRegionNamesAction(&msgContent, contentString);
        break;

      case GET_REGION_ACTION_CODE:
        fmtGetRegionAction(&msgContent.content.action.regionAction, contentString);
        break;

      case MODEL_DIMENSIONS_ACTION_CODE:
        fmtModelDimensionsAction(&msgContent, contentString);
        break;

      case MODEL_MORPHOLOGY_ACTION_CODE:
        fmtModelMorphologyAction(&msgContent, contentString);
        break;

      case MODEL_SPECIES_ACTION_CODE:
        fmtModelSpeciesAction(&msgContent, contentString);
        break;

      case REGION_NAMES_RESULT_CODE:
        fmtRegionNamesResult(&msgContent.content.perception.regionNamesResult, contentString);
        break;

      case REGION_RESULT_CODE:
        fmtRegionResult(&msgContent.content.perception.regionResult, contentString);
        break;

      case MODEL_DIMENSIONS_RESULT_CODE:
        fmtDimensionsResult(&msgContent.content.perception.dimensionsResult, contentString);
        break;

      case MODEL_MORPHOLOGY_RESULT_CODE:
        fmtMorphologyResult(&msgContent.content.perception.morphologyResult, contentString);
        break;

      case MODEL_SPECIES_RESULT_CODE:
        fmtBenthicSpeciesResult(&msgContent.content.perception.benthicSpeciesResult, contentString);
        break;

      // AP, 2007.05.15
      case MODEL_MORPHOLOGY_END_CODE:
        fmtMorphologyEndResult(&msgContent.content.perception.result, contentString);
        break;

      case MODEL_SPECIES_END_CODE:
        fmtBenthicSpeciesEndResult(&msgContent.content.perception.result, contentString);
        break;

      // AP, 2007.07.03
      case SAVE_CONFIGURATION_CODE:
        fmtSaveConfigurationAction(&msgContent, contentString);
        break;

      case SAVE_RESULT_CODE:
        fmtSaveResult(&msgContent.content.perception.result, contentString);
        break;

      // AP, 2008.05.21
      case AGENTS_CODE:
        fmtAgentsAction(&msgContent, contentString);
        break;

      case KNOWN_AGENTS_CODE:
        fmtKnownAgentsResult(&msgContent.content.perception.agentsResult, contentString);
        break;

    /* AP - ADD CODE HERE...
     */

      default:
        break;
    }

    sprintf(message, "message(%i %s %s %s)", msgECDP->messageId.idMsg,
            msgECDP->messageId.sender, msgECDP->messageId.receiver,
            contentString);
}
//---------------------------------------------------------------------------


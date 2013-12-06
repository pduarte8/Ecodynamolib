//---------------------------------------------------------------------------
#ifndef _ECDPMESSAGES_H_
#define _ECDPMESSAGES_H_

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#include "Queue.h"
/*
 *---------------------------------------------------------------------------
 *
 *  Definitions of ECOLANG Messages
 *  Definitions of EcoDynamo Protocol structures
 *
 *  v0.4 - 2006.05.16 (AP)
 *   Included messages connect, disconnect and accept and related structures.
 *
 *  v0.4a - 2006.06.27 (AP)
 *   Changed message seed: new structure included BIVALVE_S.
 *
 *  v0.4b - 2006.07.04 (AP)
 *   Included messages end_simulation and end_step.
 *
 *  v1.0 - 2007.01.30 (AP)
 *   Included messages delete, define_result, delete_result, unselect_variables
 *          and unselect_variables_result.
 *
 *  v1.1 - 2007.02.27 (AP)
 *   Included new messages running, stopped and paused.
 *
 *  v1.2 - 2007.03.12/15 (AP)
 *   Messages 'register' & 'logger' modified:
 *       Structure BOX_VALUE included and structure REGISTER_MSG modified.
 *       Structure LOG_MSG included and structure LOGGER_MSG modified.
 *
 *  v1.3 - 2007.04.30/2007.05.02 (AP)
 *   Messages 'get_variable_value' & 'variable_value' modified:
 *       Structure VAR_VALUE_ACTION modified.
 *       Structure VAR_VALUE_RESULT modified.
 *   Messages and structures added:
 *         'get_region_names'  ==>  no structure associated
 *         'get_region'  ==>  REGION_ACTION
 *         'model_dimensions'  ==>  no structure associated
 *         'model_morphology'  ==>  no structure associated
 *         'model_species'  ==>  no structure associated
 *         'region_names'  ==>  REGION_NAMES_RESULT
 *         'region'  ==>  REGION_RESULT
 *         'dimensions'  ==>  DIMENSIONS_RESULT
 *         'morphology'  ==>  MORPHOLOGY_RESULT
 *         'benthic_species'  ==>  BENTHIC_SPECIES_RESULT
 *
 *  v1.3a - 2007.05.15 (AP)
 *   Messages included:
 *         'morphology_end'  ==>  no structure associated
 *         'benthic_species_end'  ==>  no structure associated
 *
 *  v1.3b - 2007.07.03 (AP)
 *   Messages included:
 *         'save_configuration'  ==>  no structure associated
 *         'save_result'  ==>  ACTION_RESULT
 *
 *  v1.3c - 2008.05.14 (AP)
 *   Messages included:
 *         'agents'  ==>  no structure associated
 *         'known_agents'  ==>  AGENTS_RESULT
 *
 *---------------------------------------------------------------------------
 */

// Definitions of EcoDynamo Protocol Messages (text and data structures)

#define ECDP_TIMEOUT 60000      /* 1 minute waiting for answer (ms) */

#define ECDP_STRING 128
#define ECDP_MSG_LENGTH (1024 * 20)
#define REG_BOXES 750       // control number of BOX_VALUE in one message
                            // experienced with 200 in 2008.01.23, AP
#define REG_LOG 100         // control number of LOG_MSG in one message
#define SPECIES_BOXES 150   // control number of cells in SPECIES_VALUE message

// definitions for field 'type'
#define TM_TYPE_NOW 0       //"now"
#define TM_TYPE_ECOTIME 1   //"time"
typedef struct ecotime {
	int type;
    long time;
} ECOTIME;

typedef struct ecopoint {
    int x;       // column (numbered from left to right)
    int y;       // line (numbered from top to bottom)
} ECOPOINT;

typedef struct ecorect {
    ECOPOINT p1;
    ECOPOINT p2;
} ECORECT;

typedef struct ecosquare {
    ECOPOINT p1;
    ECOPOINT p2;
    ECOPOINT p3;
    ECOPOINT p4;
} ECOSQUARE;

typedef struct ecocircle {
    ECOPOINT center;
    double radius;
} ECOCIRCLE;

typedef struct ecoarc {
    ECOPOINT center;
    double p1;
    double p2;
    double p3;
    double p4;
} ECOARC;

// definitions for field 'type'
#define SR_TYPE_POINT 0     //"point"
#define SR_TYPE_RECT 1      //"rect"
#define SR_TYPE_SQUARE 2    //"square"
#define SR_TYPE_CIRCLE 3    //"circle"
#define SR_TYPE_ARC 4       //"arc"
typedef struct simple_region {
	int type;
    union {
		ECOPOINT point;
    	ECORECT rect;
	    ECOSQUARE square;
    	ECOCIRCLE circle;
	    ECOARC arc;
    } poligon;
} SIMPLE_REGION;

// variable value in one cell
typedef struct box_value {
    int cell;
    double value;
} BOX_VALUE;

// definitions for field 'type'
#define SC_TYPE_SANDY 0         //"sandy"
#define SC_TYPE_SAND_MUDDY 1    //"sand_muddy"
#define SC_TYPE_MUDDY 2         //"muddy"
// definitions for field 'quality'
#define QUALITY_EXCELLENT 0     //"excellent"
#define QUALITY_GOOD 1          //"good"
#define QUALITY_POOR 2          //"poor"
typedef struct sediment_caract {
    int type;
    int quality;
} SEDIMENT_CARACT;

// definitions for field 'type'
#define WR_TYPE_SUBTIDAL 0      //"subtidal"
#define WR_TYPE_INTERTIDAL 1    //"intertidal"
typedef struct water_region {
	int type;
    int quality;
    SEDIMENT_CARACT sedimentCaract;
} WATER_REGION;

// definitions for field 'type'
#define RT_TYPE_WATER 0     //"water"
#define RT_TYPE_LAND 1      //"land"
typedef struct edcf_region_type {
	int type;
	WATER_REGION waterRegion;
} REGION_TYPE;

typedef struct reg {
	REGION_TYPE regionType;
    Queue* pRegionArea;  // pointer to SIMPLE_REGION queue
} SUB_REGION;

// definitions for field 'type'
#define RG_TYPE_REGION 0        //"region.reg"
#define RG_TYPE_REG_NAME 1      //"region.pRegNames"
typedef struct region {
	int type;
	union {
		SUB_REGION reg;
        Queue* pRegNames;
    } region;
} REGION;

// definitions for field 'type'
#define BV_SCALLOP "scallop"
#define BV_TYPE_SCALLOP 0   // scallop_type

#define BV_KELP "kelp"
#define BV_TYPE_KELP 1      // kelp_type

#define BV_OYSTER "oyster"
#define BV_TYPE_OYSTER 2    // oyster_type

#define BV_MUSSEL "mussel"
#define BV_TYPE_MUSSEL 3    // mussel_type

#define BV_CLAM "clam"
#define BV_TYPE_CLAM 4      // clam_type

typedef struct bivalve {
	int type;
    double length;      // replaces old "BCARACT bCaract;"
} BIVALVE;

// structure used for seed message
typedef struct bivalve_s {
	int type;
    double weight1;
    double weight2;
} BIVALVE_S;


/*
 * ACTIONS structures...
 */

typedef struct inspect_action {
    char regName[ECDP_STRING];
    ECOTIME time;
} INSPECT_ACTION;

typedef struct harvest_action {
    char regName[ECDP_STRING];
    ECOTIME time;
    BIVALVE bivalve;
} HARVEST_ACTION;

typedef struct seed_action {
    char regName[ECDP_STRING];
    ECOTIME time;
    BIVALVE_S bivalve;      // changed in 2006.06.27 (v0.4a)
    double density;
} SEED_ACTION;

typedef struct definition_msg {
    char regName[ECDP_STRING];
    REGION region;
} DEFINITION_MSG;

typedef struct model_action {
    char name[ECDP_STRING];
} MODEL_ACTION;

typedef MODEL_ACTION CLASS_ACTION;
typedef MODEL_ACTION OUTFILE_ACTION;
typedef MODEL_ACTION REGION_ACTION;

// AP, 2006.06.23 - new action
typedef struct step_action {
    int nSteps;
} STEP_ACTION;

typedef struct specs_action {
    Queue* pNames;
} SPECS_ACTION;

// definitions for field 'type'
#define DOM_TYPE_ALL 0     //"all"
#define DOM_TYPE_REG 1     //"pRegNames" - queue of region names
typedef struct domain {
    int type;
    Queue* pRegNames;      // queue of ECDP_STRINGs
} R_DOMAIN;

typedef R_DOMAIN SUBDOMAIN_ACTION;

// definitions for field 'type'
#define BX_TYPE_SUBDOMAIN 0     //"boxes.subDomain"
#define BX_TYPE_CELLS 1         //"boxes.pCells"
typedef struct boxes {
	int type;
	union {
		R_DOMAIN subDomain;
        Queue* pCells;      // queue of integers
    } domain;
} BOXES;

typedef struct var_value_action {
    char className[ECDP_STRING];
    char varName[ECDP_STRING];
    BOXES boxes;
} VAR_VALUE_ACTION;

typedef struct var_value {
    char name[ECDP_STRING];
    double value;
    BOXES boxes;
} VAR_VALUE;

typedef struct parm_value {
    char name[ECDP_STRING];
    double value;
} PARM_VALUE;

typedef struct set_value_action {
    char className[ECDP_STRING];
    Queue* pValues;       // pointer to VAR_VALUE queue or PARM_VALUE queue
} SET_VALUE_ACTION;

typedef struct time_spec_action {
    int step;
    int startTime;
    int finishTime;
} TIME_SPEC_ACTION;

//definitions for field 'type'
#define SV_TYPE_FILE 0       // file
#define SV_TYPE_GRAPH 1      // graph
#define SV_TYPE_TABLE 2      // table
#define SV_TYPE_REMOTE 3     // remote agent
typedef struct sel_var_action {
    int type;
    Queue* pVarNames;   // queue of ECDP_STRINGs
    BOXES boxes;
} SEL_VAR_ACTION;

typedef struct unsel_var_action {
    int type;
    Queue* pVarNames;   // queue of ECDP_STRINGs
} UNSEL_VAR_ACTION;

// 'type' is defined in 'Log.h'
#ifndef _LOGTYPE_
#define _LOGTYPE_
typedef enum {
    XML, XLS, TXT, REMOTE
} LOGTYPE;
#endif

typedef struct log_action {
    LOGTYPE type;
    Queue* pSteps;      // queue of integers
} LOG_ACTION;
                                // AP, 2006.05.17
typedef struct connect_action {
    char hostname[ECDP_STRING];
    char hostaddr[ECDP_STRING];
    int portServer;
} CONNECT_ACTION;


/*
 * PERCEPTIONS structures...
 */

// definitions for field 'actionResult'
#define RESULT_OK 0        //"ok", "on"
#define RESULT_FAILED 1    //"failed", "off"

typedef struct action_result {
	int id;      // idMsg from ACTION
    int actionResult;
} ACTION_RESULT;

typedef struct harvest_result {
    int id;		// idMsg from HARVEST_ACTION
    int actionResult;
    double weight;
} HARVEST_RESULT;

typedef struct inspect_result {
	int id;      // idMsg from INSPECT_ACTION
    Queue* pQueue;    // pointer to BIVALVE queue
} INSPECT_RESULT;

typedef INSPECT_RESULT VARCLASS_RESULT;  // pointer to queue of names
typedef INSPECT_RESULT PARMS_RESULT;     // pointer to PARM_VALUE queue
typedef INSPECT_RESULT VAR_VALUE_RESULT; // pointer to BOX_VALUE queue
typedef INSPECT_RESULT REGION_NAMES_RESULT; // pointer to queue of names
typedef INSPECT_RESULT MORPHOLOGY_RESULT; // pointer to BOX_VALUE queue

typedef struct model_Result {
    int id;     // idMsg from MODEL_ACTIOM
    char modelName[ECDP_STRING];
} MODEL_RESULT;

typedef struct time_spec_result {
    int id;     // idMsg from TIME_SPEC_ACTION
    int step;
    long startTime;
    long finishTime;
} TIME_SPEC_RESULT;

// AP, 2005.06.27
typedef struct register_msg {
    int regIndex;
    int regTime;
    char varName[ECDP_STRING];
    Queue* pQueue;       // pointer to BOX_VALUE queue
} REGISTER_MSG;

typedef struct logger_msg {
    int step;
    Queue* pQueue;      // pointer to LOG_MSG queue
} LOGGER_MSG;

typedef struct log_msg {
    int cell;
    double value;
    char funcType[ECDP_STRING];
    char className[ECDP_STRING];
    char dataClass[ECDP_STRING];
    char varName[ECDP_STRING];
} LOG_MSG;

// AP, 2007.05.02
typedef struct region_result {
    int id;     // idMsg from REGION_ACTION
    DEFINITION_MSG definition;
} REGION_RESULT;

typedef struct dimensions_result {
    int id;    // idMsg from DIMENSIONS_ACTION
    int lines;
    int columns;
    int layers;
    char modelType[16];
} DIMENSIONS_RESULT;

typedef struct species_value {
    char name[ECDP_STRING];
    BOXES boxes;
} SPECIES_VALUE;

typedef INSPECT_RESULT BENTHIC_SPECIES_RESULT; // pointer to SPECIES_VALUE queue

// definitions for field 'connected'
#define CONNECTED 0     // 'connected'
#define DISCONNECTED 1  // 'disconnected'

typedef struct agent_data {
    char agentName[ECDP_STRING];
    char hostName[ECDP_STRING];
    char hostAddr[ECDP_STRING];
    int serverPort;
    int connected;
} AGENT_DATA;

typedef INSPECT_RESULT AGENTS_RESULT; // pointer to AGENT_DATA queue

// Definition of messages constants for 'MSG_CONTENT.type'
#define INVALID_CODE -1         // invalid action - message with syntax error

// Actions...
#define DEFINE_CODE 1           //"define"
#define SEED_ACTION_CODE 2      //"seed"
#define INSPECT_ACTION_CODE 3   //"inspect"
#define HARVEST_ACTION_CODE 4   //"harvest"
#define DELETE_ACTION_CODE 5    //"delete"
#define REGION_NAMES_ACTION_CODE 6          // "get_region_names"
#define GET_REGION_ACTION_CODE 7            // "get_region"
#define MODEL_DIMENSIONS_ACTION_CODE 8      // "model_dimensions"
#define MODEL_MORPHOLOGY_ACTION_CODE 9      // "model_morphology"
#define MODEL_SPECIES_ACTION_CODE 10        // "model_species"
#define OPEN_MODEL_CODE 11                  //"open_model"
#define CLOSE_MODEL_CODE 12                 //"close_model"
#define MODEL_NAME_CODE 13                  //"model_name"
#define INITIALISE_CODE 14                  //"initialise"
#define RUN_CODE 15                         //"run"
#define STOP_CODE 16                        //"stop"
#define STEP_CODE 17                        //"step"   => STEP_ACTION
#define PAUSE_CODE 18                       //"pause"

#define GET_AVAILABLE_CLASSES_CODE 21       //"get_available_classes"
#define GET_SELECTED_CLASSES_CODE 22        //"get_selected_classes"
#define SELECT_CLASSES_CODE 23              //"select_classes"
#define GET_VARIABLES_CODE 24               //"get_variables"
#define GET_VARIABLE_VALUE_CODE 25          //"get_variable_value"  => var_value_action
#define SET_VARIABLE_VALUE_CODE 26          //"set_variable_code"
#define GET_PARAMETERS_CODE 27              //"get_parameters"
#define SET_PARAMETERS_CODE 28              //"set_parameters"
#define GET_TIME_SPEC_CODE 29               //"get_time_spec"
#define SET_TIME_SPEC_CODE 30               //"set_time_spec"
#define SUBDOMAIN_CODE 31                   //"subdomain"
#define SAVE_CONFIGURATION_CODE 32          // "save_configuration"

#define OUTPUT_FILE_CODE 41                 //"output_file"
#define GET_AVAILABLE_VARIABLES_CODE 42     //"get_available_variables"
#define SELECT_VARIABLES_CODE 43            //"select_variables"
#define LOG_CODE 44                         //"log"
#define GET_OUTPUT_TIME_CODE 45             //"get_output_time"
#define SET_OUTPUT_TIME_CODE 46             //"set_output_time"
#define TRACE_CODE 47                       //"trace"
#define UNSELECT_VARIABLES_CODE 48          //"unselect_variables"

#define CONNECT_CODE 51                     //"connect"
#define DISCONNECT_CODE 52                  //"disconnect"
#define AGENTS_CODE 53                      //"agents"

// Perceptions...
#define DEFINE_RESULT_CODE 1001             //"define_result"
#define SEED_RESULT_CODE 1002               //"seed_result"
#define INSPECT_RESULT_CODE 1003            //"inspect_result"
#define HARVEST_RESULT_CODE 1004            //"harvest_result"
#define DELETE_RESULT_CODE 1005             //"delete_result"
#define REGION_NAMES_RESULT_CODE 1006       // "region_names"
#define REGION_RESULT_CODE 1007             // "region"
#define MODEL_DIMENSIONS_RESULT_CODE 1008   // "dimensions"
#define MODEL_MORPHOLOGY_RESULT_CODE 1009   // "morphology"
#define MODEL_SPECIES_RESULT_CODE 1010      // "benthic_species"
#define OPEN_RESULT_CODE 1011               //"open_result"
#define CLOSE_RESULT_CODE 1012              //"close_result"
#define MODEL_CODE 1013                     //"model"
#define EXEC_RESULT_CODE 1014               //"exec_result"
#define RUNNING_CODE 1015                   //"running"
#define STOPPED_CODE 1016                   //"stopped"
#define END_STEP_CODE 1017                  //"end_step"
#define PAUSED_CODE 1018                    //"paused"
#define END_SIMULATION_CODE 1019            //"end_simulation"

#define CLASSES_AVAILABLE_CODE 1021         //"classes_available"
#define CLASSES_SELECTED_CODE 1022          //"classes_selected"
#define VARIABLES_CODE 1024                 //"variables"
#define VARIABLE_VALUE_CODE 1025            //"variable_value"
#define VARIABLE_SET_RESULT_CODE 1026       //"variable_set_result"
#define PARAMETERS_VALUES_CODE 1027         //"parameters_values"
#define PARAMETERS_SET_RESULT_CODE 1028     //"parameters_set_result"
#define TIME_SPEC_CODE 1029                 //"time_spec"
#define SUBDOMAIN_RESULT_CODE 1031          //"subdomain_result"
#define SAVE_RESULT_CODE 1032               //"save_result"

#define OUTPUT_FILE_RESULT_CODE 1041        //"output_file_result"
#define VARIABLES_AVAILABLE_CODE 1042       //"variables_available"
#define SELECT_VARIABLES_RESULT_CODE 1043   //"select_variables_result"
#define LOG_RESULT_CODE 1044                //"log_result"
#define OUTPUT_TIME_CODE 1045               //"output_time"
#define TRACE_RESULT_CODE 1047              //"trace_result"
#define UNSELECT_VARIABLES_RESULT_CODE 1048 //"unselect_variables_result"

#define ACCEPT_CODE 1051                    //"accept"
#define KNOWN_AGENTS_CODE 1053              //"known_agents"

#define MODEL_MORPHOLOGY_END_CODE 1061      // "morphology_end" - general result
#define MODEL_SPECIES_END_CODE 1062         // "benthic_species_end" - general result

#define REGISTER_CODE 1091                  //"register"
#define LOGGER_CODE 1092                    //"logger"

typedef struct msg_content {
	int type;
    union
    {
		union action_msg
        {
			SEED_ACTION seed;
		    INSPECT_ACTION inspect;
		    HARVEST_ACTION harvest;

            STEP_ACTION steps;      // AP, 2006.06.23
            MODEL_ACTION model;     // AP, 2005.05.05
            SPECS_ACTION specs;     // AP, 2005.06.17
            CLASS_ACTION clAction;
            VAR_VALUE_ACTION varValue;
            SET_VALUE_ACTION setValue;
            TIME_SPEC_ACTION timeSpec;
            SUBDOMAIN_ACTION subDomain;
            OUTFILE_ACTION outFile; // AP, 2005.06.27
            SEL_VAR_ACTION selVarAction;
            LOG_ACTION logAction;
            UNSEL_VAR_ACTION unSelVarAction; // AP, 2007.01.30

            CONNECT_ACTION connectAction; // AP, 2006.05.16

            REGION_ACTION regionAction;     // AP, 2007.05.02
  		} action;

  		union perception_msg
        {
            ACTION_RESULT result;     // general action result

		    INSPECT_RESULT inspectResult;
		    HARVEST_RESULT harvestResult;

            MODEL_RESULT modelResult;       // AP, 2005.05.05
            VARCLASS_RESULT varClassResult;  // AP, 2005.06.25
            VAR_VALUE_RESULT varValueResult;
            PARMS_RESULT parmsResult;
            TIME_SPEC_RESULT timeSpecResult;

            REGISTER_MSG registerMsg;
            LOGGER_MSG loggerMsg;

            REGION_RESULT regionResult;     // AP, 2007.05.02
            REGION_NAMES_RESULT regionNamesResult;
            DIMENSIONS_RESULT dimensionsResult;
            MORPHOLOGY_RESULT morphologyResult;
            BENTHIC_SPECIES_RESULT benthicSpeciesResult;

            AGENTS_RESULT agentsResult;     // AP, 2008.05.14
		} perception;

	    DEFINITION_MSG definition;
    } content;
} MSG_CONTENT;

typedef struct msg_id {
    int idMsg;
    char sender[ECDP_STRING];
    char receiver[ECDP_STRING];
} MSG_ID;

// General message format
typedef struct message {
    long timeout;       // not used yet
    MSG_ID messageId;
    MSG_CONTENT msgContent;
} MESSAGE_ECDP;

//---------------------------------------------------------------------------
#endif //_ECDPMESSAGES_H_


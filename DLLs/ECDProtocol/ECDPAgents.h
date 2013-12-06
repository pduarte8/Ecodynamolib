//---------------------------------------------------------------------------
#ifndef _ECDP_AGENTS_H_
#define _ECDP_AGENTS_H_

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

/*
 *---------------------------------------------------------------------------
 *
 * Definition of AGENT_TABLE structure
 * Definitions of EcoDynamo Protocol Agents (names and port numbers)
 *
 *---------------------------------------------------------------------------
 */

/*
 *---------------------------------------------------------------------------
 *
 * Definition of AGENT_TABLE structure.
 *
 *---------------------------------------------------------------------------
 */

typedef struct agent_table {
	int id;
    char agentName[32];
    char hostname[128];
    char hostaddr[32];
    int portServer;
    bool connected;
} AGENT_TABLE;

/*
 *---------------------------------------------------------------------------
 *
 * Definitions of EcoDynamo Protocol Agents (names and port numbers)
 *
 *---------------------------------------------------------------------------
 */

#define MAX_AGENTS 100      //it was 10 in 20080326, AP

// Each Agent will listen in one different port within the same machine
// The Agent server socket component must reserve that port to listen

// Base socket port definition - Simulator (EcoDynamo)
#define PORT_BASE 45000

/*
 * default names for agents
 */
 
#define ECOSIM_SERVER "EcoSim_Server"
#define ECODYN_AGENT "EcoDynamo"            // 45000
#define CALIBRATION_AGENT "Calibration"     // 46000
#define FARMER_AGENT "Farmer"               // 47000
#define VIEWER_AGENT "Viewer"               // 48000

#define DEVELOP_AGENT "Develop_Agent"       // 49000
#define DUMMY_AGENT "DummyAgent"

//---------------------------------------------------------------------------
#endif //_ECDP_AGENTS_H_

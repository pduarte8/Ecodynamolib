//---------------------------------------------------------------------------
#ifndef _AGENTS_TABLE_H_
#define _AGENTS_TABLE_H_

/**
 * NPI work
 * 	02 October 2013, AP :: inclusion of __GNUC__ or __BORLANDC__ symbols
 */

#include "ECDPAgents.h"


/*
 * ---------------------------------------------------------------------------
 *
 * Definitions of EcoDynamo Protocol Agents (names and number)
 *
 * Definition of AGENTS_TABLE values.
 *
 * CHANGES IN THIS FILE FORCES REMAKE OF THE ECDP PROJECT
 *
 * ---------------------------------------------------------------------------
 */


static AGENT_TABLE agents[] = {
	{ ECOSIM_ID, ECOSIM_SERVER, "localhost", 7970 },
	{ ECODYN_ID, ECODYN_AGENT, "localhost",7972 },
	{ CALIBRATION_ID, CALIBRATION_AGENT, "localhost", 7974 },
	{ FARMER_ID, FARMER_AGENT, "localhost", 7976 },      // TMan object
	{ VIEWER_ID, VIEWER_AGENT, "localhost", 7978 },

    { DUMMY_ID, DUMMY_AGENT, "localhost", 8970 }
};



//---------------------------------------------------------------------------
#endif //_AGENTS_TABLE_H_

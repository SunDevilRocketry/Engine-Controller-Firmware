/*******************************************************************************
*
* FILE: 
* 		prefire_purge.c
*
* DESCRIPTION: 
* 	    Performs an purge of the engine in order to remove air from tanks and 
*       propellant feed plumbing 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/
#include "main.h"
#include "valve_control.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern volatile FSM_STATE fsm_state; /* Hotfire State */


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_pre_fire_purge_state                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Performs a pre-hotfire engine purge                                    *
*                                                                              *
*******************************************************************************/
FSM_STATE run_pre_fire_purge_state 
    (
    void
    )
{
/* Close vent solenoids */
vc_close_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );

/* Crack Main valves */
vc_crack_main_valves( MAIN_VALVE_BOTH_MAINS );

/* Open pressurization valves */
vc_open_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );

/* Wait for fill and chill command */
while ( fsm_state != FSM_FILL_CHILL_STATE ){}
return FSM_FILL_CHILL_STATE;
} /* run_pre_fire_purge_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
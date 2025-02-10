/*******************************************************************************
*
* FILE: 
* 		ready.c
*
* DESCRIPTION: 
* 	    Waits for connection signal from ground station control 
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
extern volatile FSM_STATE fsm_state; /* Hotfire state */


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_ready_state                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Awaits connection from ground station                                  *
*                                                                              *
*******************************************************************************/
FSM_STATE run_ready_state
    (
    void
    )
{
/* Open Vent Solenoids */
vc_open_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );

/* Close pressurization/purge solenoids */
vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS |
                    SOLENOID_LOX_PURGE | SOLENOID_FUEL_PURGE );

/* Close the main valves */
vc_close_main_valves( MAIN_VALVE_BOTH_MAINS );

/* Wait the for pre-fire purge command */
while ( fsm_state != FSM_PRE_FIRE_PURGE_STATE ){}
return FSM_PRE_FIRE_PURGE_STATE;
} /* run_ready_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
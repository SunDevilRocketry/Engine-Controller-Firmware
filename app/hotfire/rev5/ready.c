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
*       Awaits connection from ground station              

        Ready Stage:
            CLOSE Solenoid 3
            CLOSE Solenoid 2
            CLOSE Solenoid 1
            (MANUALLY) CLOSE GN2 Vent Ball Valve                    *
*                                                                              *
*******************************************************************************/
FSM_STATE run_ready_state
    (
    void
    )
{
/*  CLOSE Solenoid 3 */ 
vc_close_solenoids( SOLENOID_FUEL_PRESS_3 );

/* Close Solenoid 2 and 1 */ 
vc_close_solenoids( SOLENOID_FUEL_VENT_1 | SOLENOID_LOX_PURGE_2);

/* Wait the for pre-fire purge command */
while ( fsm_state != FSM_PRE_FIRE_PURGE_STATE ){}
return FSM_PRE_FIRE_PURGE_STATE;
} /* run_ready_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
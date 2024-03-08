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
const int PRE_FIRE_PURGE_DURATION = 5000; /* 5 seconds */

/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_pre_fire_purge_state                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Performs a pre-hotfire engine purge             
        
        Pre-fire Purge Stage:
            POWER Solenoid 3
            NO POWER to Solenoid 2
            POWER to Solenoid 1 for 5 seconds
            NO POWER to Solenoid 1                       *
*                                                                              *
*******************************************************************************/
FSM_STATE run_pre_fire_purge_state 
    (
    void
    )
{
// Power to Solenoid 3
vc_open_solenoids( SOLENOID_FUEL_PURGE_3 );

// No power to Solenoid 2
vc_close_solenoids( SOLENOID_LOX_PURGE_2 );

// Power to Solenoid 1 for 5 seconds
vc_open_solenoids( SOLENOID_FUEL_VENT_1 );
HAL_Delay( PRE_FIRE_PURGE_DURATION );

// No power to Solenoid 1
vc_close_solenoids( SOLENOID_FUEL_VENT_1 );

/* Wait for fill and chill command */
while ( fsm_state != FSM_FILL_CHILL_STATE ){}
return FSM_FILL_CHILL_STATE;
} /* run_pre_fire_purge_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
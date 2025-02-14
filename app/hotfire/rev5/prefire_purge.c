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
#include "rs485.h"
#include "ignition.h"

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
rs485_transmit_byte(IGN_PRE_FIRE_START);

/* Close vent solenoids */
VC_STATUS closed_vents_success = vc_close_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );
HAL_Delay( VENT_PRESS_DELAY );            

/* Crack Main valves */
VC_STATUS cracked_main = vc_crack_main_valves( MAIN_VALVE_BOTH_MAINS );

/* Open pressurization valves */
VC_STATUS open_press = open_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );

/* Purge Duration */
HAL_Delay( PREFIRE_PURGE_DURATION );

/* Stop the purge */
VC_STATUS closed_press = vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );
VC_STATUS close_valves =  vc_close_main_valves( MAIN_VALVE_BOTH_MAINS );
HAL_Delay( VENT_PRESS_DELAY );
VC_STATUS open_solenoid_vent =  vc_open_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );

if(closed_vents_success + cracked_main + open_press + closed_press + close_valves + open_solenoid_vent == 0)
{
rs485_transmit_byte(IGN_PRE_FIRE_DONE); 
} else{
    rs485_transmit_byte(IGN_PRE_FIRE_FAIL);
}

/* Wait for fill and chill command */
while ( fsm_state != FSM_FILL_CHILL_STATE ){}
return FSM_FILL_CHILL_STATE;
} /* run_pre_fire_purge_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
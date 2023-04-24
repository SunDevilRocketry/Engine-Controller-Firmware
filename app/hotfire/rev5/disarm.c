/*******************************************************************************
*
* FILE: 
* 		disarm.c
*
* DESCRIPTION: 
* 	    Transitions the engine to a safe state 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/
#include <string.h>
#include <stdbool.h>


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/
#include "main.h"
#include "valve_control.h"
#include "sensor.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern bool lox_purge_flag;


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_disarm_state                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Transitions the engine to a safe state                                 *
*                                                                              *
*******************************************************************************/
FSM_STATE run_disarm_state 
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint32_t    vent_start_time; /* Time at which vent starts                     */
uint32_t    vent_time;       /* Time engine has been venting                  */
float       lox_tank_press;  /* LOX tank pressure                             */
float       fuel_tank_press; /* Fuel tank pressure                            */
SENSOR_DATA sensor_data;     /* Data from engine sensors                      */
bool        is_press_atm;    /* True when tank pressures are sufficiently low */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
vent_start_time = 0;
vent_time       = 0;
lox_tank_press  = 0;
fuel_tank_press = 0;
is_press_atm    = false;
memset( &sensor_data, 0, sizeof( SENSOR_DATA ) );


/*------------------------------------------------------------------------------
 DISARM Sequence 
------------------------------------------------------------------------------*/

/* Open Vent solenoids           */
vc_open_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );

/* Close purge solenoids         */
vc_close_solenoids( SOLENOID_LOX_PURGE | SOLENOID_FUEL_PURGE );

/* Wait for pressure to vent off */
vent_start_time = HAL_GetTick();
vent_time       = HAL_GetTick() - vent_start_time;
sensor_dump( &sensor_data );
lox_tank_press  = sensor_conv_pressure( sensor_data.pt_pressures[ PT_LOX_PRESS_INDEX ], 
                                         PT_LOX_PRESS_INDEX );
fuel_tank_press = sensor_conv_pressure( sensor_data.pt_pressures[ PT_FUEL_PRESS_INDEX ], 
                                         PT_FUEL_PRESS_INDEX );
is_press_atm    = ( lox_tank_press <= 50.0 ) && ( fuel_tank_press <= 50.0 );
while ( ( !is_press_atm ) && ( vent_time <= VENT_TIMEOUT ) )
    {
    lox_tank_press  = sensor_conv_pressure( sensor_data.pt_pressures[ PT_LOX_PRESS_INDEX ], 
                                            PT_LOX_PRESS_INDEX );
    fuel_tank_press = sensor_conv_pressure( sensor_data.pt_pressures[ PT_FUEL_PRESS_INDEX ], 
                                            PT_FUEL_PRESS_INDEX );
    is_press_atm    = ( lox_tank_press <= 50.0 ) && ( fuel_tank_press <= 50.0 );
    vent_time = HAL_GetTick() - vent_start_time; 
    }

/* Go to manual mode if venting timed out */
if ( vent_time >= VENT_TIMEOUT )
    {
    return FSM_MANUAL_STATE;
    }

/* LOX tank purge */
while ( !lox_purge_flag ){}
vc_close_solenoids( SOLENOID_LOX_VENT );
vc_open_solenoids( SOLENOID_LOX_PRESS );
vc_crack_main_valves( MAIN_VALVE_LOX_MAIN );
HAL_Delay( LOX_PURGE_DURATION );

/* Transition to safe state for manual closing of K-bottle */
vc_close_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );
vc_close_main_valves( MAIN_VALVE_BOTH_MAINS );
vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );
HAL_Delay( TANK_PRESS_DELAY );

/* Check engine pressures        */
/* Send safe to approach command */

/* Open all the solenoids */
vc_open_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS |
                   SOLENOID_LOX_VENT  | SOLENOID_FUEL_PRESS |
                   SOLENOID_LOX_PURGE | SOLENOID_FUEL_PURGE );

/* Transition to post-fire state */
return FSM_POST_FIRE_STATE;
} /* run_disarm_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
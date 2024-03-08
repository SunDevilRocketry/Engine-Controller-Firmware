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
#include "pressure.h"
#include "sensor.h"
#include "sdr_error.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern volatile bool lox_purge_flag;      /* Initiate LOX purge */
extern volatile bool kbottle_closed_flag; /* KBottle is closed  */
extern volatile bool tanks_safe_flag;     /* Engine is safe to approach */
extern volatile bool stop_purge_flag;     /* Start disarming the engine */


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_disarm_state                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Transitions the engine to a safe state    

        Disarm Stage:
            (MANUALLY) CLOSE GN2 and GNO2 Cylinders
            (MANUALLY) OPEN Manual Vent
            POWER to Solenoid 2
            NO POWER to Solenoid 3
            POWER to Solenoid 1
                                     *
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
uint32_t    safe_start_time; /* Time engine safing begins                     */
uint32_t    safe_time;       /* Time since safing begins                      */
float       lox_tank_press;  /* LOX tank pressure                             */
float       fuel_tank_press; /* Fuel tank pressure                            */
SENSOR_DATA sensor_data;     /* Data from engine sensors                      */
bool        is_press_atm;    /* True when tank pressures are sufficiently low */
float       pt_pressure;     /* PT pressure converted from adc readout        */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
vent_start_time = 0;
vent_time       = 0;
safe_start_time = 0;
safe_time       = 0;
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


/* Wait for disarm command */
while ( !stop_purge_flag ){}

/* Wait for pressure to vent off */
tanks_safe_flag = false;
vent_start_time = HAL_GetTick();
vent_time       = HAL_GetTick() - vent_start_time;
sensor_dump( &sensor_data );
lox_tank_press  = sensor_conv_pressure( sensor_data.pt_pressures[ PT_LOX_PRESS_INDEX ], 
                                         PT_LOX_PRESS_INDEX );
fuel_tank_press = sensor_conv_pressure( sensor_data.pt_pressures[ PT_FUEL_PRESS_INDEX ], 
                                         PT_FUEL_PRESS_INDEX );
is_press_atm    = ( lox_tank_press <= 100.0 ) && ( fuel_tank_press <= 100.0 );
while ( ( !is_press_atm ) && ( vent_time <= VENT_TIMEOUT ) )
    {
    lox_tank_press  = sensor_conv_pressure( sensor_data.pt_pressures[ PT_LOX_PRESS_INDEX ], 
                                            PT_LOX_PRESS_INDEX );
    fuel_tank_press = sensor_conv_pressure( sensor_data.pt_pressures[ PT_FUEL_PRESS_INDEX ], 
                                            PT_FUEL_PRESS_INDEX );
    is_press_atm    = ( lox_tank_press <= 100.0 ) && ( fuel_tank_press <= 100.0 );
    vent_time = HAL_GetTick() - vent_start_time; 
    }
tanks_safe_flag = true;

/* Go to manual mode if venting timed out */
//if ( vent_time >= VENT_TIMEOUT )
 //   {
    //Error_Handler( ERROR_FSM_INVALID_STATE_ERROR );
    //return FSM_MANUAL_STATE;
  //  }

/* LOX tank purge */
while ( !lox_purge_flag ){}
vc_close_solenoids( SOLENOID_LOX_VENT );
HAL_Delay( VENT_PRESS_DELAY );
vc_open_solenoids( SOLENOID_LOX_PRESS );
vc_crack_main_valves( MAIN_VALVE_LOX_MAIN );
HAL_Delay( LOX_PURGE_DURATION );

/* Transition to safe state for manual closing of K-bottle */
vc_open_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );
vc_close_main_valves( MAIN_VALVE_BOTH_MAINS );
HAL_Delay( VENT_PRESS_DELAY );
vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );
HAL_Delay( TANK_PRESS_DELAY );

/* Wait for safe engine pressures */
safe_start_time = HAL_GetTick();
safe_time       = HAL_GetTick() - safe_start_time;
is_press_atm    = false;
while ( ( !is_press_atm ) && ( safe_time <= SAFE_TIMEOUT ) )
    {
    safe_time = HAL_GetTick() - safe_start_time;
    sensor_dump( &sensor_data );
    for ( uint8_t i = 0; i < NUM_PTS; ++i )
        {
        pt_pressure = sensor_conv_pressure( sensor_data.pt_pressures[ i ], i );
        if ( pt_pressure >= 100.0 )
            {
            is_press_atm = false;
            break;
            }
        else if ( i == ( NUM_PTS - 1) )
            {
            is_press_atm = true;
            }
        }
    }

/* Check for timeout */
if ( safe_time >= SAFE_TIMEOUT )
    {
//    return FSM_MANUAL_STATE;
    }

/* Send safe to approach command */
tanks_safe_flag = true;

/* Wait for K-bottle is closed command */
while ( !kbottle_closed_flag ){}

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
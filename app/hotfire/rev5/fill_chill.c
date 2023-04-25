/*******************************************************************************
*
* FILE: 
* 		fill_chill.c
*
* DESCRIPTION: 
* 	    Monitors the engine state while propellant tanks are being filled 
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
#include "sensor.h"
#include "valve_control.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern volatile FSM_STATE fsm_state;       /* Hotfire State       */
extern volatile bool      tanks_safe_flag; /* Safe tank pressures */


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_fill_chill_state                                                   *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Monitors the engine state while propellant tanks are being filled      *
*                                                                              *
*******************************************************************************/
FSM_STATE run_fill_chill_state 
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
float       lox_tank_press;  /* LOX tank pressure,  psi */
float       fuel_tank_press; /* Fuel tank pressure, psi */
SENSOR_DATA sensor_data;     /* Data from engine sensors */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
lox_tank_press  = 0;
fuel_tank_press = 0;
memset( &sensor_data, 0, sizeof( SENSOR_DATA ) );


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Stop the engine purge */
vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );
vc_close_main_valves( MAIN_VALVE_BOTH_MAINS );
vc_open_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT );

/* Check Tank Pressures */
sensor_dump( &sensor_data );
lox_tank_press  = sensor_conv_pressure( sensor_data.pt_pressures[ PT_LOX_PRESS_INDEX ], 
                                         PT_LOX_PRESS_INDEX );
fuel_tank_press = sensor_conv_pressure( sensor_data.pt_pressures[ PT_FUEL_PRESS_INDEX ], 
                                         PT_FUEL_PRESS_INDEX );
HAL_Delay( FILL_CHILL_TANK_DELAY );
if ( ( lox_tank_press > 600 ) || ( fuel_tank_press > 600 ) )
    {
    /* Enter manual control mode */
    return FSM_MANUAL_STATE;
    }

/* Let the ground station know the tanks are okay */
tanks_safe_flag = true;


/* Fill the tanks */
while ( fsm_state != FSM_STANDBY_STATE )
    {
    sensor_dump( &sensor_data );
    lox_tank_press  = sensor_conv_pressure( sensor_data.pt_pressures[ PT_LOX_PRESS_INDEX ], 
                                            PT_LOX_PRESS_INDEX );
    fuel_tank_press = sensor_conv_pressure( sensor_data.pt_pressures[ PT_FUEL_PRESS_INDEX ], 
                                            PT_FUEL_PRESS_INDEX );
    if ( ( lox_tank_press > 600 ) || ( fuel_tank_press > 600 ) )
        {
        /* Send Warning Indication to ground station */
        tanks_safe_flag = false;
        }
    else
        {
        tanks_safe_flag = true;
        }
    }

/* Enter Standby state */
return FSM_STANDBY_STATE;
} /* run_fill_chill_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
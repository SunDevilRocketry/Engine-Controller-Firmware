/*******************************************************************************
*
* FILE: 
* 		fire.c
*
* DESCRIPTION: 
* 	    Control the engine during hotfire 
        Update: This will execute a new proposed 2 second hot-fire
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/
#include "main.h"
#include "valve_control.h"
#include "data_logger.h"
#include "ignition.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern FSM_STATE fsm_state;         /* State of the engine hotfire */
extern bool      stop_hotfire_flag; /* Manual Hotfire termination  */
extern bool      stop_purge_flag;   /* Manual purge termination    */


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_fire_state                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Fire the engine and log sensor data                                    *
*                                                                              *
*******************************************************************************/
FSM_STATE run_fire_state 
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint32_t            burn_time;         /* Time engine has been burning   */
DATA_LOG_DATA_FRAME sensor_data;       /* Data from engine sensors       */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
memset( &sensor_data, 0, sizeof( sensor_data ) );


/*------------------------------------------------------------------------------
 Engine Fire Sequence 
------------------------------------------------------------------------------*/


/* Open LOx Vent and LOx Pressurization Valve */
vc_open_solenoids( SOLENOID_LOX_VENT | SOLENOID_LOX_PRESS );
HAL_Delay( 8000 );

/* Open Fuel Press Valve */
vc_open_solenoids( SOLENOID_FUEL_PRESS );
HAL_Delay( 2000 );

/* Ignite the solid propellant Ignitor */
ign_ignite();
HAL_Delay( 2000 );

/* Full Speed Open LOx Main Valve */
vc_open_main_valves( MAIN_VALVE_LOX_MAIN );
HAL_Delay( 750 );

/* Gradually Open Fuel Main Valve */
// TODO
vc_open_main_valves( MAIN_VALVE_FUEL_MAIN );
HAL_Delay( 2250 );

/* Full speed Close Fuel Main Valve */
vc_close_main_valves( MAIN_VALVE_FUEL_MAIN );
HAL_Delay( 500 );

/* Close LOx Main Valve */
vc_close_main_valves( MAIN_VALVE_LOX_MAIN );
/* Open Fuel and LOx Purge */
vc_open_solenoids( SOLENOID_FUEL_PURGE | SOLENOID_LOX_PURGE );
/* Close Fuel and LOx Press */
vc_close_solenoids( SOLENOID_FUEL_PRESS | SOLENOID_LOX_PRESS );
HAL_Delay( 10000 );

/* Close Fuel and LOx Purge */
vc_close_solenoids( SOLENOID_FUEL_PURGE | SOLENOID_LOX_PURGE );
/* Open Fuel and LOx Vent */
vc_open_solenoids( SOLENOID_FUEL_VENT | SOLENOID_LOX_VENT );



// /* Engine Burn */
// data_logger_init_timer();
// burn_time = data_logger_get_time();
// while ( burn_time < ENGINE_BURN_DURATION )
//     {
//     /* Poll for abort */
//     if ( fsm_state == FSM_ABORT_STATE )
//         {
//         return FSM_ABORT_STATE;
//         }

//     /* Poll for stop fire command */
//     if ( stop_hotfire_flag )
//         {
//         break;
//         }
    
//     /* Log Data */
//     data_logger_get_data( &sensor_data );
//     data_logger_log_data( sensor_data  );


//     /* Update timer */
//     burn_time = data_logger_get_time();
//     }

// /* Close the main valves */
// vc_close_main_valves( MAIN_VALVE_BOTH_MAINS );

// /* Close the pressuriztion solenoids */
// vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS );

// /* Open the purge solenoids */
// vc_open_solenoids( SOLENOID_LOX_PURGE | SOLENOID_FUEL_PURGE );

// /* Wait for stop purge command or timeout */
// HAL_Delay( POSTFIRE_PURGE_DURATION );

/* Transition to disarm state */
return FSM_DISARM_STATE;
} /* run_fire_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
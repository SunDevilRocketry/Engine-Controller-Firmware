/*******************************************************************************
*
* FILE: 
* 		fire.c
*
* DESCRIPTION: 
* 	    Control the engine during hotfire 
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
*       Fire the engine and log sensor data

        Fire Stage:
            POWER to ignitor from flight computer
            POWER to Solenoid 2 for 5 seconds
            NO POWER to Solenoid 2
            NO POWER to Solenoid 3
            POWER to Solenoid 1
            NO POWER to Solenoid 1                                   *
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
/* Ignite the engine */
ign_ignite();
HAL_Delay( ENGINE_IGNITION_DELAY );

/* Power to Solenoid 2 for 5 seconds */
vc_open_solenoids( SOLENOID_LOX_PURGE_2 );

/* Engine Burn */
data_logger_init_timer();
burn_time = data_logger_get_time();
while ( burn_time < ENGINE_BURN_DURATION )
    {
    /* Poll for abort */
    if ( fsm_state == FSM_ABORT_STATE )
        {
        return FSM_ABORT_STATE;
        }

    /* Poll for stop fire command */
    if ( stop_hotfire_flag )
        {
        break;
        }
    
    /* Log Data */
    data_logger_get_data( &sensor_data );
    data_logger_log_data( sensor_data  );


    /* Update timer */
    burn_time = data_logger_get_time();
    }

/* No power to Solenoid 2 and 3 */
vc_close_solenoids( SOLENOID_LOX_PURGE_2 | SOLENOID_FUEL_PURGE_3);

/* Power to Solenoid 1 */
vc_open_solenoids( SOLENOID_FUEL_VENT_1 );

/* No Power to Solenoid 1 */
vc_close_solenoids( SOLENOID_FUEL_VENT_1 );

/* Transition to disarm state */
return FSM_DISARM_STATE;
} /* run_fire_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
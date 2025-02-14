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
rs485_transmit_byte(IGN_HOT_FIRE_START);

/* Close the vent solenoids */
if (vc_close_solenoids( SOLENOID_LOX_VENT | SOLENOID_FUEL_VENT ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
};
HAL_Delay( VENT_PRESS_DELAY );

/* Pressurize the LOX tank */
if (vc_open_solenoids( SOLENOID_LOX_PRESS ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
};
HAL_Delay( TANK_PRESS_DELAY );

/* Pressurize the fuel tank */
if (vc_open_solenoids( SOLENOID_FUEL_PRESS ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
};
HAL_Delay( TANK_PRESS_DELAY );

/* Crack the main LOX valve */
if (vc_crack_main_valves( MAIN_VALVE_LOX_MAIN ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
};
HAL_Delay( LOX_CRACK_DURATION );

if (vc_close_main_valves( MAIN_VALVE_LOX_MAIN ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
};
HAL_Delay( LOX_POSTCRACK_DELAY );

/* Ignite the engine */
if (ign_ignite() == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
}
HAL_Delay( ENGINE_IGNITION_DELAY );

/* Open the main valves */
vc_open_main_valves( MAIN_VALVE_BOTH_MAINS );

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

/* Close the main valves */
if (vc_close_main_valves( MAIN_VALVE_BOTH_MAINS ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
}

/* Close the pressuriztion solenoids */
if (vc_close_solenoids( SOLENOID_LOX_PRESS | SOLENOID_FUEL_PRESS ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
}

/* Open the purge solenoids */
if (vc_open_solenoids( SOLENOID_LOX_PURGE | SOLENOID_FUEL_PURGE ) == VC_OK){
    rs485_transmit_byte(IGN_HOT_FIRE_FAIL);
    return FSM_ABORT_STATE;
}

/* Wait for stop purge command or timeout */
HAL_Delay( POSTFIRE_PURGE_DURATION );


rs485_transmit_byte(IGN_HOT_FIRE_DONE);
/* Transition to disarm state */
return FSM_DISARM_STATE;
} /* run_fire_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
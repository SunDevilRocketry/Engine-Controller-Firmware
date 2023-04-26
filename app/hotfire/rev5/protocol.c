/*******************************************************************************
*
* FILE: 
* 		protocol.c
*
* DESCRIPTION: 
* 	    Implements a protocol for communicating with the ground station over 
*       the wireless interface 
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

/* Application level */
#include "main.h"
#include "valve_control.h"
#include "protocol.h"

/* Module level */
#include "commands.h"
#include "led.h"
#include "rs485.h"
#include "sensor.h"
#include "solenoid.h"
#include "valve.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/

/* Sends a signal to the PC that engine controller is responsive */
static void send_ack
	(
	void
	);

/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern volatile FSM_STATE fsm_state;           /* State of engine hotfire     */
extern volatile bool      stop_hotfire_flag;   /* Manual hotfire termination  */
extern volatile bool      stop_purge_flag;     /* Manual purge termination    */
extern volatile bool      lox_purge_flag;      /* LOX tank purge              */
extern volatile bool      kbottle_closed_flag; /* Kbottle is closed           */
extern volatile bool      tanks_safe_flag;     /* Safe tank pressures         */
extern volatile bool      telreq_wait_flag;    /* Wait flag for telreq cmds   */
extern SENSOR_DATA_PING_PONG sensor_ping_pong_buffer;


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		protocol_command_handler                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Handles commands received from the ground station                      *
*                                                                              *
*******************************************************************************/
void protocol_command_handler
    (
    uint8_t command /* Ground station command */
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t          subcommand;                  /* SDEC subcommand              */
uint8_t          sol_state;                   /* State of solenoids           */
SENSOR_DATA      sensor_data;                 /* Data from engine sensors     */
uint8_t          sensor_data_bytes[ sizeof( SENSOR_DATA ) ]; 
RS485_STATUS     rs485_status;                /* RS485 return codes           */
VALVE_STATUS     valve_status;                /* Valve module return codes    */
TANK_SAFE_STATES tanks_state;                 /* State of tank pressures      */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
rs485_status  = RS485_OK;
valve_status  = VALVE_OK;
sol_state     = 0;
memset( &sensor_data         , 0, sizeof( sensor_data       ) );
memset( &sensor_data_bytes[0], 0, sizeof( sensor_data_bytes ) );


/*------------------------------------------------------------------------------
 Command Handling 
------------------------------------------------------------------------------*/
switch( command )
    {
    /*--------------------------------------------------------------------------
     PING Command	
    --------------------------------------------------------------------------*/
    case PING_OP:
        {
        ping();
        break;
        } /* PING_OP */

    /*--------------------------------------------------------------------------
     CONNECT Command	
    --------------------------------------------------------------------------*/
    case CONNECT_OP:
        {
        ping();
        break;
        } /* CONNECT_OP */
    
    /*--------------------------------------------------------------------------
     SOL Command	
    --------------------------------------------------------------------------*/
    case SOL_OP:
        {
        /* Get subcommand */
        rs485_status = rs485_receive( &subcommand         , 
                                      sizeof( subcommand ), 
                                      RS485_DEFAULT_TIMEOUT );
        if ( rs485_status != RS485_OK )
            {
            led_set_color( LED_YELLOW );
            break;
            }
        
        /* Pass on command and subcommand to valve controller */ 
        valve_transmit( &command   , sizeof( command ), HAL_DEFAULT_TIMEOUT );
        valve_transmit( &subcommand, sizeof( command ), HAL_DEFAULT_TIMEOUT );

        /* Pass on valve state if sol getstate command */
        if ( subcommand == SOL_GETSTATE_CODE )
            {
            valve_status = valve_receive( &sol_state, 
                                          sizeof( sol_state ), 
                                          HAL_DEFAULT_TIMEOUT );
            if ( valve_status != VALVE_OK )
                {
                led_set_color( LED_YELLOW );
                break;
                }
            rs485_transmit( &sol_state, sizeof( sol_state ), HAL_DEFAULT_TIMEOUT );
            }
        break;
        } /* SOL_OP */

    /*--------------------------------------------------------------------------
     VALVE Command	
    --------------------------------------------------------------------------*/
    case VALVE_OP:
        {
        /* Get subcommand */
        rs485_status = rs485_receive( &subcommand         , 
                                      sizeof( subcommand ), 
                                      RS485_DEFAULT_TIMEOUT );
        if ( rs485_status != RS485_OK )
            {
            led_set_color( LED_YELLOW );
            break;
            }

        /* Pass on command and subcommand to valve controller */ 
        valve_transmit( &command   , sizeof( command ), HAL_DEFAULT_TIMEOUT );
        valve_transmit( &subcommand, sizeof( command ), HAL_DEFAULT_TIMEOUT );
        break;
        } /* VALVE_OP */

    /*--------------------------------------------------------------------------
     ABORT Command	
    --------------------------------------------------------------------------*/
    case ABORT_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Initiate Abort */
        run_abort_state();
        fsm_state = FSM_ABORT_STATE;
        break;
        } /* ABORT_OP */

    /*--------------------------------------------------------------------------
     TELREQ Command	
    --------------------------------------------------------------------------*/
    case TELREQ_OP:
        {
        /* Send ACK */
        send_ack();

        /* Get sensor data */
        if ( fsm_state == FSM_FILL_CHILL_STATE )
            {
            memcpy( &sensor_data, &( sensor_ping_pong_buffer.sensor_data[ sensor_ping_pong_buffer.current_index ] ), sizeof( SENSOR_DATA ) );
            }
        else
            {
            sensor_dump( &sensor_data );
            }
        memcpy( &sensor_data_bytes[0], &sensor_data, sizeof( SENSOR_DATA ) );

        /* Transmit the sensor data */
        rs485_transmit( &sensor_data_bytes[0], 
                        sizeof( SENSOR_DATA ), 
                        RS485_DEFAULT_TIMEOUT*sizeof( SENSOR_DATA ));
        break;
        } /* TELREQ_OP */

    /*--------------------------------------------------------------------------
     PFPURGE Command	
    --------------------------------------------------------------------------*/
    case PFPURGE_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Initiate Purge*/
        fsm_state = FSM_PRE_FIRE_PURGE_STATE;
        break;
        } /* PFPURGE_OP */
    
    /*--------------------------------------------------------------------------
     CHILL Command	
    --------------------------------------------------------------------------*/
    case CHILL_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Initiate Fill and Chill Sequence */
        fsm_state = FSM_FILL_CHILL_STATE;
        break;
        } /* CHILL_OP */

    /*--------------------------------------------------------------------------
     STANDBY Command	
    --------------------------------------------------------------------------*/
    case STANDBY_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Initiate Standby mode */
        fsm_state = FSM_STANDBY_STATE;
        break;
        } /* STANDBY_OP */

    /*--------------------------------------------------------------------------
     FIRE Command 
    --------------------------------------------------------------------------*/
    case FIRE_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Initiate Standby mode */
        fsm_state = FSM_FIRE_STATE;
        break;
        } /* FIRE_OP */

    /*--------------------------------------------------------------------------
     STOPPURGE Command 
    --------------------------------------------------------------------------*/
    case STOPPURGE_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Stop the engine purge */
        stop_purge_flag = true;
        break;
        } /* STOPPURGE_OP */

    /*--------------------------------------------------------------------------
     GETSTATE Command 
    --------------------------------------------------------------------------*/
    case HOTFIRE_GETSTATE_OP:
        {
        /* Send the finite state machine state back to the ground station */
        rs485_transmit( (void*) &fsm_state, sizeof( fsm_state ), RS485_DEFAULT_TIMEOUT );
        break;
        } /* HOTFIRE_GETSTATE_OP */

    /*--------------------------------------------------------------------------
     STOPHOTFIRE Command 
    --------------------------------------------------------------------------*/
    case STOP_HOTFIRE_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Set the stop hotfire flag */
        stop_hotfire_flag = true;
        break;
        } /* STOP_HOTFIRE_OP */

    /*--------------------------------------------------------------------------
     LOXPURGE Command 
    --------------------------------------------------------------------------*/
    case LOX_PURGE_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Set the lox purge flag */
        lox_purge_flag = true;
        break;
        } /* LOX_PURGE_OP */
    
    /*--------------------------------------------------------------------------
     KBOTTLE Command 
    --------------------------------------------------------------------------*/
    case KBOTTLE_CLOSED_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Set the kbottle closed flag */
        kbottle_closed_flag = true;
        break;
        } /* KBOTTLE_CLOSED_OP */

    /*--------------------------------------------------------------------------
     TANKSTAT Command 
    --------------------------------------------------------------------------*/
    case TANKSTAT_OP:
        {
        /* Check flags */
        if ( tanks_safe_flag )
            {
            tanks_state = TANKS_SAFE;
            }
        else
            {
            tanks_state = TANKS_UNSAFE;
            }
        
        /* Send response */
        rs485_transmit( &tanks_state, sizeof( tanks_state ), RS485_DEFAULT_TIMEOUT );
        break;
        }

    /*--------------------------------------------------------------------------
     MANUAL Command 
    --------------------------------------------------------------------------*/
    case MANUAL_OP:
        {
        /* Send ACK signal */
        send_ack();

        /* Set the kbottle closed flag */
        fsm_state = FSM_MANUAL_STATE;
        break;
        } /* KBOTTLE_CLOSED_OP */

    /*--------------------------------------------------------------------------
     Unrecognized Command 
    --------------------------------------------------------------------------*/
    default:
        {
        /* Unsupported command code flash the red LED */
        //Error_Handler();
        } /* default */
    } 

} /* protocol_command_handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		protocol_send_frame                                                    *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Send a frame of data over the long-range communication interface       *
*                                                                              *
*******************************************************************************/
PROTOCOL_STATUS protocol_send_frame
    (
    PROTOCOL_FRAME frame
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
#ifdef USE_RS485
    RS485_STATUS rs485_status;
#else
    RF_STATUS    rf_status;
#endif


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
#ifdef USE_RS485
    rs485_status = RS485_OK;
#else
    rf_status    = RF_OK;
#endif


/*------------------------------------------------------------------------------
 Implementation
------------------------------------------------------------------------------*/
#ifdef USE_RS485
    rs485_status = rs485_transmit( &frame, 
                                   sizeof( frame ), 
                                   RS485_DEFAULT_TIMEOUT*sizeof( frame ) );
    if ( rs485_status == RS485_TIMEOUT )
        {
        return PROTOCOL_TIMEOUT;
        }
    else if ( rs485_status != RS485_OK )
        {
        return PROTOCOL_ERROR;
        }
#else
    rf_status = rf_xbee_transmit( &frame, sizeof( frame ) );
    if ( rf_status == RF_TIMEOUT )
        {
        return PROTOCOL_TIMEOUT;
        }
    else if ( rf_status != RF_OK )
        {
        return RF_ERROR;
        }
#endif

return PROTOCOL_OK;
} /* protocol_send_frame */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		protocol_get_frame                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Get a frame of data over the long-range communication interface        *
*                                                                              *
*******************************************************************************/
PROTOCOL_STATUS protocol_get_frame
    (
    PROTOCOL_FRAME* frame_ptr
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
#ifdef USE_RS485
    RS485_STATUS rs485_status;
#else
    RF_STATUS    rf_status;
#endif


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
#ifdef USE_RS485
    rs485_status = RS485_OK;
#else
    rf_status    = RF_OK;
#endif


/*------------------------------------------------------------------------------
 Implementation
------------------------------------------------------------------------------*/
#ifdef USE_RS485
    rs485_status = rs485_receive( frame_ptr, 
                                  sizeof( PROTOCOL_FRAME ), 
                                  RS485_DEFAULT_TIMEOUT*sizeof( PROTOCOL_FRAME) );
    if ( rs485_status == RS485_TIMEOUT )
        {
        return PROTOCOL_TIMEOUT;
        }
    else if ( rs485_status != RS485_OK )
        {
        return PROTOCOL_ERROR;
        }
#else
    rf_status = rf_xbee_receive( frame_ptr, sizeof( PROTOCOL_FRAME ) );
    if ( rf_status == RF_TIMEOUT )
        {
        return PROTOCOL_TIMEOUT;
        }
    else if ( rf_status != RF_OK )
        {
        return RF_ERROR;
        }
#endif

return PROTOCOL_OK;
} /* protocol_get_frame */


/*------------------------------------------------------------------------------
 Internal Functions 
------------------------------------------------------------------------------*/

/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
*       send_ack                                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Sends a signal to the PC that engine controller is responsive          *
*                                                                              *
*******************************************************************************/
static void send_ack
	(
	void
	)

{ 
uint8_t response = ACK_OP; 
rs485_transmit( &response, sizeof( response ), RS485_DEFAULT_TIMEOUT );
} /* send_no_ack */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
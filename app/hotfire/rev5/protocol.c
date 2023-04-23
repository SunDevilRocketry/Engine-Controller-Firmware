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


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/

/* Application level */
#include "main.h"
#include "valve_control.h"
#include "protocol.h"

/* Module level */
#include "commands.h"
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
uint8_t subcommand;                           /* SDEC subcommand              */
uint8_t board_code;                           /* Ping code of connected board */
uint8_t sensor_data[ sizeof( SENSOR_DATA ) ]; /* Data from engine sensors     */
RS485_STATUS rs485_status;                    /* RS485 return codes           */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
rs485_status = RS485_OK;
memset( &sensor_data[0], 0, sizeof( sensor_data ) );


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
        break;
        } /* PING_OP */

    /*--------------------------------------------------------------------------
     CONNECT Command	
    --------------------------------------------------------------------------*/
    case CONNECT_OP:
        {
        break;
        } /* CONNECT_OP */
    
    /*--------------------------------------------------------------------------
     SOL Command	
    --------------------------------------------------------------------------*/
    case SOL_OP:
        {
        break;
        } /* SOL_OP */

    /*--------------------------------------------------------------------------
     VALVE Command	
    --------------------------------------------------------------------------*/
    case VALVE_OP:
        {
        break;
        } /* VALVE_OP */

    /*--------------------------------------------------------------------------
     ABORT Command	
    --------------------------------------------------------------------------*/
    case ABORT_OP:
        {
        break;
        } /* ABORT_OP */

    /*--------------------------------------------------------------------------
     TELREQ Command	
    --------------------------------------------------------------------------*/
    case TELREQ_OP:
        {
        break;
        } /* TELREQ_OP */

    /*--------------------------------------------------------------------------
     PFPURGE Command	
    --------------------------------------------------------------------------*/
    case PFPURGE_OP:
        {
        break;
        } /* PFPURGE_OP */
    
    /*--------------------------------------------------------------------------
     CHILL Command	
    --------------------------------------------------------------------------*/
    case CHILL_OP:
        {
        break;
        } /* CHILL_OP */

    /*--------------------------------------------------------------------------
     STANDBY Command	
    --------------------------------------------------------------------------*/
    case STANDBY_OP:
        {
        break;
        } /* STANDBY_OP */

    /*--------------------------------------------------------------------------
     FIRE Command 
    --------------------------------------------------------------------------*/
    case FIRE_OP:
        {
        } /* FIRE_OP */

    /*--------------------------------------------------------------------------
     STOPPURGE Command 
    --------------------------------------------------------------------------*/
    case STOPPURGE_OP:
        {
        break;
        } /* STOPPURGE_OP */

    /*--------------------------------------------------------------------------
     Unrecognized Command 
    ------------------------------------------------------------------*/
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
    rs485_status = rs485_transmit( &frame, sizeof( frame ) );
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
    rs485_status = rs485_receive( frame_ptr, sizeof( PROTOCOL_FRAME ) );
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
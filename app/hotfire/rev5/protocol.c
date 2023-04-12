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


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/

/* Application level */
#include "main.h"
#include "valve_control.h"
#include "protocol.h"

/* Module level */
#include "wireless.h"
#include "rs485.h"


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


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


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
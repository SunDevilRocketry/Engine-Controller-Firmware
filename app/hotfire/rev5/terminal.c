/*******************************************************************************
*
* FILE: 
* 		terminal.c
*
* DESCRIPTION: 
* 	    Contains the pre-processing, execution, and post-processing of terminal
*       commands and data for the hotfire firmware 
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

/* Application Layer */
#include "main.h"
#include "sdr_pin_defines_L0002.h"
#include "init.h"
#include "sdr_error.h"
#include "terminal.h"

/* Low-level modules */
#include "commands.h"
#include "ignition.h"
#include "flash.h"
#include "led.h"
#include "power.h"
#include "pressure.h"
#include "rs485.h"
#include "sensor.h"
#include "solenoid.h"
#include "temp.h"
#include "usb.h"
#include "valve.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		terminal_exec_cmd                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Executes a terminal command                                            *
*                                                                              *
*******************************************************************************/
TERMINAL_STATUS terminal_exec_cmd
    (
    uint8_t command
    )
{
/*------------------------------------------------------------------------------
 Local Variables                                                                
------------------------------------------------------------------------------*/
uint8_t           subcommand;       /* subcommand code                        */
uint8_t           pwr_source;       /* Power source code                      */
uint8_t           response;         /* Response to PC                         */

/* Flash */
HFLASH_BUFFER     flash_handle;                    /* Flash API buffer handle */
uint8_t           flash_buffer[ DEF_BUFFER_SIZE ]; /* Flash data buffer       */

/* Solenoids */
SOL_STATE         sol_state;         /* State of solenoids        */
MAIN_VALVE_STATES main_valve_states; /* States of the main valves */

/* Module return codes */
FLASH_STATUS      flash_status;     /* Status of flash operations             */
IGN_STATUS        ign_status;       /* Ignition status code                   */
USB_STATUS        usb_status;       /* Status of USB operations               */
VALVE_STATUS      valve_status;     /* Valve API return codes                 */
//RESET_status      reset_status;     /* Reset status of code                   */

/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/

/* Flash Buffer */
flash_handle.write_protected       = FLASH_WP_WRITE_ENABLED;
flash_handle.num_bytes             = 0;
flash_handle.address               = 0;
flash_handle.pbuffer               = &flash_buffer[0];
flash_handle.status_register       = 0xFF;
flash_handle.bpl_bits              = FLASH_BPL_NONE;
flash_handle.bpl_write_protect     = FLASH_BPL_READ_WRITE;

/* Solenoids */
sol_state                          = 0;
main_valve_states                  = 0;

/* Module return codes */
flash_status                       = FLASH_OK;
usb_status                         = USB_OK;
valve_status                       = VALVE_OK;


/*------------------------------------------------------------------------------
 Execute SDEC Command 
------------------------------------------------------------------------------*/
switch( command )
    {
    /*-----------------------------------------------------------------
        PING Command	
    ------------------------------------------------------------------*/
    case PING_OP:
        {
        response = PING_RESPONSE_CODE;
        HAL_UART_Transmit( &( USB_HUART ), 
                           &response, 
                           sizeof( response ), 
                           HAL_DEFAULT_TIMEOUT );
        break;
        } /* PING_OP */

    /*-----------------------------------------------------------------
        CONNECT Command	
    ------------------------------------------------------------------*/
    case CONNECT_OP:
        {
        response = PING_RESPONSE_CODE;
        HAL_UART_Transmit( &( USB_HUART ), 
                           &response, 
                           sizeof( response ), 
                           HAL_DEFAULT_TIMEOUT );
        break;
        } /* CONNECT_OP */

    /*-----------------------------------------------------------------
        IGNITE Command	
    ------------------------------------------------------------------*/
    case IGNITE_OP:
        {
        /* Recieve ignition subcommand over USB */
        usb_status = usb_receive( &subcommand         , 
                                    sizeof( subcommand ), 
                                    HAL_DEFAULT_TIMEOUT );

        /* Execute subcommand */
        if ( usb_status != USB_TIMEOUT )
            {
            /* Execute subcommand*/
            ign_status = ign_cmd_execute( subcommand );
            }
        else
            {
            /* Error: no subcommand recieved */
            Error_Handler( ERROR_IGN_CMD_ERROR );
            }

        /* Return response code to terminal */
        usb_transmit( &ign_status         , 
                        sizeof( ign_status ), 
                        HAL_DEFAULT_TIMEOUT );
        break;
        } /* IGNITE_OP */

    /*-----------------------------------------------------------------
        POWER Command	
    ------------------------------------------------------------------*/
    case POWER_OP:
        {
        /* Determine power source */
        pwr_source = pwr_get_source();

        /* Convert to response code and transmit to PC */
        pwr_source += 1;
        usb_transmit( &pwr_source         , 
                        sizeof( pwr_source ), 
                        HAL_DEFAULT_TIMEOUT );
        break;
        } /* POWER_OP */

    /*-----------------------------------------------------------------
     FLASH Command	
    ------------------------------------------------------------------*/
    case FLASH_OP:
        {
        /* Recieve flash subcommand over USB */
        usb_status = usb_receive( &subcommand         , 
                                    sizeof( subcommand ), 
                                    HAL_DEFAULT_TIMEOUT );
    
        /* Execute subcommand */
        if ( usb_status == USB_OK )
            {
            flash_status = flash_cmd_execute( subcommand   , 
                                                &flash_handle );
            }
        else
            {
            /* Subcommand code not recieved */
            Error_Handler( ERROR_FLASH_CMD_ERROR );
            }

        /* Transmit status code to PC */
        usb_transmit( &flash_status, 
                        sizeof( flash_status ), 
                        HAL_DEFAULT_TIMEOUT );
        break;
        } /* FLASH_OP */

    /*-----------------------------------------------------------------
     SENSOR Command	
    ------------------------------------------------------------------*/
    case SENSOR_OP:
        {
        /* Recieve subcommand from USB */
        usb_status = usb_receive( &subcommand         ,
                                    sizeof( subcommand ),
                                    HAL_DEFAULT_TIMEOUT );

        /* Execute subcommand */
        if ( usb_status != USB_OK )
            {
            /* Subcommand not recieved */
            Error_Handler( ERROR_SENSOR_CMD_ERROR );
            }
        else
            {
            sensor_cmd_execute( subcommand ); 
            }

        break;
        } /* SENSOR_OP */

    /*-----------------------------------------------------------------
     VALVE Command	
    ------------------------------------------------------------------*/
    case VALVE_OP:
        {
        /* Get subcommand */
        usb_status = usb_receive( &subcommand         ,
                                    sizeof( subcommand ),
                                    HAL_DEFAULT_TIMEOUT );
        if ( usb_status != USB_OK )
            {
            Error_Handler( ERROR_VALVE_CMD_ERROR );
            }
        
        /* Pass on command and subcommand to valve controller */
        valve_status = valve_transmit( &command         , 
                                        sizeof( command ), 
                                        HAL_DEFAULT_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            Error_Handler( ERROR_VALVE_CMD_ERROR );
            }
        valve_status = valve_transmit( &subcommand, 
                                        sizeof( command ),
                                        HAL_DEFAULT_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            Error_Handler( ERROR_VALVE_CMD_ERROR );
            }

        /* Pass on state if valve getstate command */
        if ( subcommand == VALVE_GETSTATE_CODE )
            {
            valve_status = valve_receive( &main_valve_states, 
                                            sizeof( main_valve_states ), 
                                            HAL_DEFAULT_TIMEOUT );
            if ( valve_status != VALVE_OK )
                {
                Error_Handler( ERROR_VALVE_CMD_ERROR );
                }
            
            usb_transmit( &main_valve_states, 
                            sizeof( main_valve_states ), 
                            HAL_DEFAULT_TIMEOUT );
            }
        break;
        } /* VALVE_OP */

    /*-----------------------------------------------------------------
     SOL Command 
    ------------------------------------------------------------------*/
    case SOL_OP:
        {
        /* Get Subcommand */
        usb_status = usb_receive( &subcommand, 
                        sizeof( subcommand ), 
                        HAL_DEFAULT_TIMEOUT );
        if ( usb_status != USB_OK )
            {
            Error_Handler( ERROR_SOL_CMD_ERROR );
            }

        /* Pass on command and subcommand to valve controller */
        valve_status = valve_transmit( &command         , 
                                        sizeof( command ), 
                                        HAL_DEFAULT_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            Error_Handler( ERROR_SOL_CMD_ERROR );
            }
        valve_status = valve_transmit( &subcommand      , 
                                        sizeof( command ),
                                        HAL_DEFAULT_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            Error_Handler( ERROR_SOL_CMD_ERROR );
            }

        /* Pass on valve state if sol getstate command */
        if ( subcommand == SOL_GETSTATE_CODE )
            {
            valve_status = valve_receive( &sol_state, 
                                            sizeof( sol_state ), 
                                            HAL_DEFAULT_TIMEOUT );
            if ( valve_status != VALVE_OK )
                {
                Error_Handler( ERROR_SOL_CMD_ERROR );
                }
            usb_status = usb_transmit( &sol_state,
                                        sizeof( sol_state ),
                                        HAL_DEFAULT_TIMEOUT );
            if ( usb_status != USB_OK )
                {
                Error_Handler( ERROR_SOL_CMD_ERROR );
                }
            }
        break;
        } /* Reset_OP */

    /*-----------------------------------------------------------------
     RESET Command	
    ------------------------------------------------------------------*/
    case RESET_OP:
        {

        uint8_t empty_buffer[1];
        /* Transmit status code to PC */
         usb_transmit( &empty_buffer, 
                        sizeof(empty_buffer), 
                        HAL_DEFAULT_TIMEOUT );
       
        NVIC_SystemReset();

        break;
        } /* RESET_OP */

    /*------------------------ Unrecognized Command ---------------------------*/
    default:
        {
        /* Unsupported command code flash the red LED */
        return TERMINAL_UNRECOGNIZED_CMD;
        }

    } /* case( command ) */
return TERMINAL_OK;
} /* terminal_exec_cmd */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
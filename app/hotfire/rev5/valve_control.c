/*******************************************************************************
*
* FILE: 
* 		valve_control.c
*
* DESCRIPTION: 
* 	    Provides an interface to the valve controller for opening and closing 
*       solenoids and main valves 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "valve_control.h"
#include "commands.h"
#include "solenoid.h"
#include "valve.h"


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_open_solenoids                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Open a solenoid                                                        *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_open_solenoids
    (
    SOLENOID solenoids /* Solenoids to open */
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      sol_subcommand;    /* Solenoid subcommand/number   */
uint8_t      sol_tx_buffer[2];  /* Buffer for transmitting data */
VALVE_STATUS valve_status;      /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
sol_subcommand   = SOL_OPEN_CODE; 
valve_status     = VALVE_OK;
sol_tx_buffer[0] = SOL_OP;
sol_tx_buffer[1] = 0;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Loop through each solenoid */
for ( uint8_t i = 0; i < NUM_SOLENOIDS; ++i )
    {
    if ( solenoids & ( 1 << i ) )
        {
        /* Set valve number and subcommand */
        sol_tx_buffer[1] = sol_subcommand | ( i + 1 );

        /* Send data to valve controller */ 
        valve_status = valve_transmit( &sol_tx_buffer[0]      , 
                                       sizeof( sol_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        }
    }
return VC_OK;
} /* vc_open_solenoids */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_close_solenoids                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Close a solenoid                                                       *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_close_solenoids
    (
    SOLENOID solenoids /* Solenoids to close */
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      sol_subcommand;    /* Solenoid subcommand/number   */
uint8_t      sol_tx_buffer[2];  /* Buffer for transmitting data */
VALVE_STATUS valve_status;      /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
sol_subcommand   = SOL_CLOSE_CODE; 
valve_status     = VALVE_OK;
sol_tx_buffer[0] = SOL_OP;
sol_tx_buffer[1] = 0;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Loop through each solenoid */
for ( uint8_t i = 0; i < NUM_SOLENOIDS; ++i )
    {
    if ( solenoids & ( 1 << i ) )
        {
        /* Set valve number and subcommand */
        sol_tx_buffer[1] = sol_subcommand | ( i + 1 );

        /* Send data to valve controller */ 
        valve_status = valve_transmit( &sol_tx_buffer[0]      , 
                                       sizeof( sol_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        }
    }
return VC_OK;
} /* vc_close_solenoids */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_reset_solenoids                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Reset solenoids                                                        *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_reset_solenoids
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      sol_tx_buffer[2];  /* Buffer for transmitting data */
VALVE_STATUS valve_status;      /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_status     = VALVE_OK;
sol_tx_buffer[0] = SOL_OP;
sol_tx_buffer[1] = SOL_RESET_CODE;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Send data to valve controller */
valve_status = valve_transmit( &sol_tx_buffer[0]      , 
                               sizeof( sol_tx_buffer ), 
                               VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VALVE_UART_ERROR;
    }
else
    {
    return VC_OK;
    }
} /* vc_reset_solenoids */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_open_main_valves                                                    *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Open a main valve                                                      *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_open_main_valves
    (
    MAIN_VALVE main_valves
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      valve_subcommand;   /* Main valve subcommand/number */
uint8_t      valve_tx_buffer[2]; /* Buffer for transmitting data */
VALVE_STATUS valve_status;       /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_subcommand   = VALVE_OPEN_CODE; 
valve_status       = VALVE_OK;
valve_tx_buffer[0] = VALVE_OP;
valve_tx_buffer[1] = 0;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/
/* Setup subcommand and transmit data */
switch ( main_valves )
    {
    /* Open the main LOX valve */
    case MAIN_VALVE_LOX_MAIN:
        {
        valve_tx_buffer[1] = valve_subcommand;
        valve_status = valve_transmit( &valve_tx_buffer[0], 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    /* Open the main fuel valve */
    case MAIN_VALVE_FUEL_MAIN:
        {
        valve_tx_buffer[1] = valve_subcommand + 1;
        valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    /* Open both main valves */
    case MAIN_VALVE_BOTH_MAINS:
        {
        valve_tx_buffer[1] = valve_subcommand;
        valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        valve_tx_buffer[1] += 1;
        valve_status = valve_transmit( &valve_tx_buffer[0], 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    } /* switch ( main_valves ) */
return VC_OK;
} /* vc_open_main_valves */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_crack_main_valves                                                   *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Crack open a main valve                                                *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_crack_main_valves
    (
    MAIN_VALVE main_valves
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      valve_subcommand;   /* Main valve subcommand/number */
uint8_t      valve_tx_buffer[2]; /* Buffer for transmitting data */
VALVE_STATUS valve_status;       /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_subcommand   = VALVE_CRACK_CODE; 
valve_status       = VALVE_OK;
valve_tx_buffer[0] = VALVE_OP;
valve_tx_buffer[1] = 0;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/
/* Setup subcommand and transmit data */
switch ( main_valves )
    {
    /* Open the main LOX valve */
    case MAIN_VALVE_LOX_MAIN:
        {
        valve_tx_buffer[1] = valve_subcommand;
        valve_status = valve_transmit( &valve_tx_buffer[0], 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    /* Open the main fuel valve */
    case MAIN_VALVE_FUEL_MAIN:
        {
        valve_tx_buffer[1] = valve_subcommand + 1;
        valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    /* Open both main valves */
    case MAIN_VALVE_BOTH_MAINS:
        {
        valve_tx_buffer[1] = valve_subcommand;
        valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        valve_tx_buffer[1] += 1;
        valve_status = valve_transmit( &valve_tx_buffer[0], 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    } /* switch ( main_valves ) */
return VC_OK;
} /* vc_crack_main_valves */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_close_main_valves                                                   *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Close a main valve                                                     *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_close_main_valves
    (
    MAIN_VALVE main_valve
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      valve_subcommand;   /* Main valve subcommand/number */
uint8_t      valve_tx_buffer[2]; /* Buffer for transmitting data */
VALVE_STATUS valve_status;       /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_subcommand   = VALVE_CLOSE_CODE; 
valve_status       = VALVE_OK;
valve_tx_buffer[0] = VALVE_OP;
valve_tx_buffer[1] = 0;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/
/* Setup subcommand and transmit data */
switch ( main_valve )
    {
    /* Open the main LOX valve */
    case MAIN_VALVE_LOX_MAIN:
        {
        valve_tx_buffer[1] = valve_subcommand;
        valve_status = valve_transmit( &valve_tx_buffer[0], 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    /* Open the main fuel valve */
    case MAIN_VALVE_FUEL_MAIN:
        {
        valve_tx_buffer[1] = valve_subcommand + 1;
        valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    /* Open both main valves */
    case MAIN_VALVE_BOTH_MAINS:
        {
        valve_tx_buffer[1] = valve_subcommand;
        valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        valve_tx_buffer[1] += 1;
        valve_status = valve_transmit( &valve_tx_buffer[0], 
                                       sizeof( valve_tx_buffer ), 
                                       VALVE_TIMEOUT );
        if ( valve_status != VALVE_OK )
            {
            return VC_UART_ERROR;
            }
        break;
        }

    } /* switch ( main_valves ) */
return VC_OK;
} /* vc_close_main_valves */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_calibrate_main_valves                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Calibrate the main valves                                              *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_calibrate_main_valves
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      valve_tx_buffer[2];  /* Buffer for transmitting data */
VALVE_STATUS valve_status;        /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_status       = VALVE_OK;
valve_tx_buffer[0] = VALVE_OP;
valve_tx_buffer[1] = VALVE_CALIBRATE_CODE;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Send data to valve controller */
valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                               sizeof( valve_tx_buffer ), 
                               VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VALVE_UART_ERROR;
    }
else
    {
    return VC_OK;
    }
} /* vc_calibrate_main_valves */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_enable_main_valves                                                  *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Enable the stepper motor drivers for the main valves                   *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_enable_main_valves
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      valve_tx_buffer[2];  /* Buffer for transmitting data */
VALVE_STATUS valve_status;        /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_status       = VALVE_OK;
valve_tx_buffer[0] = VALVE_OP;
valve_tx_buffer[1] = VALVE_ENABLE_CODE;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Send data to valve controller */
valve_status = valve_transmit( &valve_tx_buffer[0]      , 
                               sizeof( valve_tx_buffer ), 
                               VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VALVE_UART_ERROR;
    }
else
    {
    return VC_OK;
    }

} /* vc_enable_main_valves */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_connect                                                             *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Ping the valve controller to verify the controller can be reached      *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_connect
    (
    void
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
uint8_t      command;            /* Valve controller command     */
uint8_t      response;           /* Valve controller response    */
VALVE_STATUS valve_status;       /* Return codes from valve API  */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
valve_status = VALVE_OK;
command      = CONNECT_OP;


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Send connect command to valve controller */
valve_status = valve_transmit( &command, sizeof( command ), VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VALVE_UART_ERROR;
    }

/* Get response code and verify correctness */
valve_status = valve_receive( &response, sizeof( response ), VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VALVE_UART_ERROR;
    }
else if ( response != VALVE_CONTROLLER_REV2_ID &&
          response != VALVE_CONTROLLER_REV3_ID )
    {
    return VC_CANNOT_CONNECT;
    }
else
    {
    return VC_OK;
    }
} /* vc_connect */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		vc_getstate                                                            *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Gets the state of the engine engine valves (Open/Closed)               *
*                                                                              *
*******************************************************************************/
VC_STATUS vc_getstate
    (
    VALVE_STATES* valve_state_ptr /* Pointer to output variable */
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
SOL_STATE         solenoid_states;   /* States of solenoids       */
MAIN_VALVE_STATES main_valve_states; /* States of the main valves */
uint8_t           command;           /* SDEC sol/valve command    */
uint8_t           subcommand;        /* SDEC sol/valve subcommand */
VALVE_STATUS      valve_status;      /* Valve module return codes */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
solenoid_states   = 0;
main_valve_states = 0;
*valve_state_ptr  = 0;
command           = SOL_OP;             /* Check solenoids first */
subcommand        = SOL_GETSTATE_CODE;
valve_status      = VALVE_OK;


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/

/* Get the solenoids states   */
valve_transmit( &command   , sizeof( command    ), VALVE_TIMEOUT );
valve_transmit( &subcommand, sizeof( subcommand ), VALVE_TIMEOUT );
valve_status = valve_receive( &solenoid_states, 
                              sizeof( solenoid_states ), 
                              VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VC_UART_ERROR;
    }

/* Get the main valves states */
command    = VALVE_OP;
subcommand = VALVE_GETSTATE_CODE;
valve_transmit( &command   , sizeof( command    ), VALVE_TIMEOUT );
valve_transmit( &subcommand, sizeof( subcommand ), VALVE_TIMEOUT );
valve_status = valve_receive( &main_valve_states, 
                              sizeof( main_valve_states ), 
                              VALVE_TIMEOUT );
if ( valve_status != VALVE_OK )
    {
    return VC_UART_ERROR;
    }

/* Export the valve states */
*valve_state_ptr |= solenoid_states;
*valve_state_ptr |= main_valve_states;
return VC_OK;
} /* vc_getstate */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
/*******************************************************************************
*
* FILE: 
* 		sequence.c
*
* DESCRIPTION: 
* 	    Runs a sequence uploaded from ground station or SDEC
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/
#include "sdr_error.h"
#include "main.h"
#include "valve_control.h"
#include "commands.h"
#include "sequence.h"
#include "valve.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern volatile FSM_STATE fsm_state; /* Hotfire state */
extern SEQUENCE_NODE global_sequence_nodes[99];


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_sequence_state                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Runs a custom sequence                                                 *
*                                                                              *
*******************************************************************************/
FSM_STATE run_sequence_state
    (
    void
    )
{
    uint32_t sequence_start_time = HAL_GetTick();
    VALVE_STATUS valve_status = VALVE_OK;

    int i = 0;
    while ( i < 99) 
    {
        if ( global_sequence_nodes[i].sequence_num == 99 ) 
        {
            break;
        }

        uint32_t current_time = HAL_GetTick() - sequence_start_time;
        if ( current_time < (uint32_t)global_sequence_nodes[i].time )
        {
            continue;
        } 

        uint8_t command = global_sequence_nodes[i].subcommand_code;
        switch ( command )
        {
            case VALVE_OP:
                {
                valve_status = valve_transmit( &command         , 
                                                sizeof( command ), 
                                                HAL_DEFAULT_TIMEOUT );
                if ( valve_status != VALVE_OK )
                    {
                    Error_Handler( ERROR_VALVE_CMD_ERROR );
                    }
                valve_status = valve_transmit( &global_sequence_nodes[i].opcode, 
                                                sizeof( command ),
                                                HAL_DEFAULT_TIMEOUT );
                if ( valve_status != VALVE_OK )
                    {
                    Error_Handler( ERROR_VALVE_CMD_ERROR );
                    }

                break;
                } /* VALVE_OP */
            
            case SOL_OP:
                {

                /* Pass on command and subcommand to valve controller */
                valve_status = valve_transmit( &command         , 
                                                sizeof( command ), 
                                                HAL_DEFAULT_TIMEOUT );
                if ( valve_status != VALVE_OK )
                    {
                    Error_Handler( ERROR_SOL_CMD_ERROR );
                    }
                valve_status = valve_transmit( &global_sequence_nodes[i].opcode      , 
                                                sizeof( command ),
                                                HAL_DEFAULT_TIMEOUT );
                if ( valve_status != VALVE_OK )
                    {
                    Error_Handler( ERROR_SOL_CMD_ERROR );
                    }
                } /* SOL_OP */

        } 
    
        i += 1;
    }

    return FSM_SEQUENCE_STATE; /* Placeholder return */
} /* run_sequence_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
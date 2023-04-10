/*******************************************************************************
*
* FILE: 
* 		wireless_protocol.h
*
* DESCRIPTION: 
* 	    Implements a protocol for communicating with the ground station over 
*       the wireless interface 
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WIRELESS_PROTOCOL_H 
#define WIRELESS_PROTOCOL_H 

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 Includes                                                              
------------------------------------------------------------------------------*/
#include "valve_control.h"


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Typedefs 
------------------------------------------------------------------------------*/

/* Return codes */
typedef enum _WP_STATUS
    {
    WP_OK,   /* Function returned normally */
    WP_ERROR /* Misc error                 */
    } WP_STATUS;

/* Operation codes */
typedef enum _WP_OPCODE
    {
    WP_SOL_OPCODE     = 0b00100001, /* Manual solenoid control (VALVE type)   */
    WP_VALVE_OPCODE   = 0b00100010, /* Manual main valve control (VALVE type) */
    WP_ABORT_OPCODE   = 0b01000011, /* Abort hotfire opcode (ACTION type)     */
    WP_TELREQ_OPCODE  = 0b01100100, /* Telemetry request opcode (INFO type)   */
    WP_STATUS_OPCODE  = 0b01100101, /* Status request opcode (INFO type)      */
    WP_PFPURGE_OPCODE = 0b01000110, /* Pre-fire purge opcode (ACTION type)    */
    WP_FILL_OPCODE    = 0b01000111, /* Fill/Chill opcode (ACTION type)        */
    WP_STANDBY_OPCODE = 0b01001000, /* Standby opcode (ACTION type)           */
    WP_FIRE_OPCODE    = 0b01001001, /* Engine Fire opcode (ACTION type)       */ 
    WP_STOP_OPCODE    = 0b01001010, /* Stop purge opcode (ACTION type)        */
    WP_DISARM_OPCODE  = 0b01001011  /* Disarm opcode (ACTION type)            */
    } WP_OPCODE;

/* Solenoid subcommand codes */
typedef enum _WP_SOL_OPCODE
    {
    WP_SOL_OPEN = 1, /* Open solenoids                   */ 
    WP_SOL_CLOSE   , /* Close solenoids                  */
    WP_SOL_RESET   , /* Reset solenoids to default state */
    } WP_SOL_OPCODE;

/* Main valve subcommand codes */
typedef enum _WP_VALVE_OPCODE
    {
    WP_VALVE_OPEN = 1, /* Open main valve   */
    WP_VALVE_CLOSE   , /* Close main valve  */
    WP_VALVE_CRACK   , /* Crack main valve  */
    WP_VALVE_RESET     /* Reset main valves */
    } WP_VALVE_OPCODE;


/*------------------------------------------------------------------------------
 Public Function Prototypes 
------------------------------------------------------------------------------*/

/* Initiate a solenoid transmission (VALVE type) */
WP_STATUS wp_sol_transmit
    (
    WP_SOL_OPCODE sol_opcode, /* Solenoid subcommand  */
    SOLENOID      solenoids   /* Solenoids to actuate */ 
    );

/* Initiate a main valve transmission (VALVE type) */
WP_STATUS wp_valve_transmit
    (
    WP_VALVE_OPCODE valve_opcode, /* Valve subcommand */
    MAIN_VALVE      valves
    );

/* Initiate an ACTION type transmission */
WP_STATUS wp_action_transmit
    (
    WP_OPCODE opcode /* action type opcode */
    );

/* Initiate an INFO type transmission */


#ifdef __cplusplus
}
#endif

#endif /* WIRELESS_PROTOCOL_H */

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
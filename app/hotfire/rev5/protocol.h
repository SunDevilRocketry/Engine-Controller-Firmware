/*******************************************************************************
*
* FILE: 
* 		protocol.h
*
* DESCRIPTION: 
* 	    Implements a protocol for communicating with the ground station over 
*       the wireless interface 
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PROTOCOL_H 
#define PROTOCOL_H 

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

/* Protocol Commands */
#define ABORT_OP                ( 0x90 )
#define TELREQ_OP               ( 0x96 )
#define PFPURGE_OP              ( 0x91 )
#define CHILL_OP                ( 0x92 )
#define STANDBY_OP              ( 0x93 )
#define FIRE_OP                 ( 0x94 )
#define STOPPURGE_OP            ( 0x97 )
#define ACK_OP                  ( 0x95 )
#define NO_ACK_OP               ( 0x98 )
#define HOTFIRE_GETSTATE_OP     ( 0x99 )
#define STOP_HOTFIRE_OP         ( 0x9A )
#define LOX_PURGE_OP            ( 0x9B )
#define KBOTTLE_CLOSED_OP       ( 0x9C )
#define TANKSTAT_OP             ( 0x9D )


/*------------------------------------------------------------------------------
 Typedefs 
------------------------------------------------------------------------------*/

/* Return codes */
typedef enum _PROTOCOL_STATUS
    {
    PROTOCOL_OK,      /* Function returned normally */
    PROTOCOL_TIMEOUT, /* IO layer timeout occured   */
    PROTOCOL_ERROR    /* Misc error                 */
    } PROTOCOL_STATUS;

/* Operation codes */
typedef enum _PROTOCOL_OPCODE
    {
    PROTOCOL_SOL_OPCODE     = 0x51, /* Manual solenoid control   */
    PROTOCOL_VALVE_OPCODE   = 0x52, /* Manual main valve control */
    PROTOCOL_ABORT_OPCODE   = 0x90, /* Abort hotfire opcode      */
    PROTOCOL_TELREQ_OPCODE  = 0x96, /* Telemetry request opcode  */
    PROTOCOL_PFPURGE_OPCODE = 0x91, /* Pre-fire purge opcode     */
    PROTOCOL_FILL_OPCODE    = 0x92, /* Fill/Chill opcode         */
    PROTOCOL_STANDBY_OPCODE = 0x93, /* Standby opcode            */
    PROTOCOL_FIRE_OPCODE    = 0x94, /* Engine Fire opcode        */ 
    PROTOCOL_STOP_OPCODE    = 0x97, /* Stop purge opcode         */
    PROTOCOL_ACK_OPCODE     = 0x95  /* Frame Acknowledge opcde   */
    } PROTOCOL_OPCODE;

/* Solenoid subcommand codes */
typedef enum _PROTOCOL_SOL_SUBCMD
    {
    PROTOCOL_SOL_OPEN     = 0x28, /* Open solenoids                   */ 
    PROTOCOL_SOL_CLOSE    = 0x30, /* Close solenoids                  */
    PROTOCOL_SOL_RESET    = 0x18, /* Reset solenoids to default state */
    PROTOCOL_SOL_GETSTATE = 0x20  /* Get the state of the solenoids   */
    } PROTOCOL_SOL_SUBCMD;

/* Main valve subcommand codes */
typedef enum _PROTOCOL_VALVE_SUBCMD
    {
    PROTOCOL_VALVE_OPEN  = 0x04, /* Open main valve   */
    PROTOCOL_VALVE_CLOSE = 0x06, /* Close main valve  */
    PROTOCOL_VALVE_CRACK = 0x0A, /* Crack main valve  */
    PROTOCOL_VALVE_RESET = 0x10  /* Reset main valves */
    } PROTOCOL_VALVE_SUBCMD;

/* Frame Struct Type */
typedef struct _PROTOCOL_FRAME
    {
    uint8_t opcode;
    uint8_t control[2];
    } PROTOCOL_FRAME; 

/* Tank Safe States */
typedef enum _TANK_SAFE_STATES
    {
    TANKS_SAFE = 1, 
    TANKS_UNSAFE
    } TANK_SAFE_STATES;


/*------------------------------------------------------------------------------
 Public Function Prototypes 
------------------------------------------------------------------------------*/

/* Handles commands received from the ground station */
void protocol_command_handler
    (
    uint8_t command /* Ground station command */
    );

/* Send a frame of data over the long-range communication interface */
PROTOCOL_STATUS protocol_send_frame
    (
    PROTOCOL_FRAME frame 
    );


/* Get a frame of data over the long-range communication interface */
PROTOCOL_STATUS protocol_get_frame
    (
    PROTOCOL_FRAME* frame_ptr
    );


#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_H */

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
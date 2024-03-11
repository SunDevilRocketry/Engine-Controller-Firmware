/*******************************************************************************
*
* FILE: 
* 		disarm.c
*
* DESCRIPTION: 
* 	    Transitions the engine to a safe state 
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
#include "main.h"
#include "valve_control.h"
#include "pressure.h"
#include "sensor.h"
#include "sdr_error.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern volatile bool lox_purge_flag;      /* Initiate LOX purge */
extern volatile bool kbottle_closed_flag; /* KBottle is closed  */
extern volatile bool tanks_safe_flag;     /* Engine is safe to approach */
extern volatile bool stop_purge_flag;     /* Start disarming the engine */


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		run_disarm_state                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Transitions the engine to a safe state    

        Disarm Stage:
            (MANUALLY) CLOSE GN2 and GNO2 Cylinders
            (MANUALLY) OPEN Manual Vent
            OPEN Solenoid 2
            OPEN Solenoid 3
            OPEN Solenoid 1
                                     *
*                                                                              *
*******************************************************************************/
FSM_STATE run_disarm_state 
    (
    void
    )
{
/*------------------------------------------------------------------------------
 DISARM Sequence 
------------------------------------------------------------------------------*/

/* Open Solenoid 1,2,3 */
vc_open_solenoids( SOLENOID_FUEL_VENT_1 | SOLENOID_LOX_PURGE_2 | SOLENOID_FUEL_PRESS_3 );

/* Transition to Facility Shutdown Stage */
return FSM_SHUTDOWN_STATE;
} /* run_disarm_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
/*******************************************************************************
*
* FILE: 
* 		abort_hotfire.c
*
* DESCRIPTION: 
* 	    Aborts the hotfire in case of emergency 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/
#include "main.h"
#include "valve_control.h"
#include "led.h"


/*------------------------------------------------------------------------------
 Public Functions 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		shutdown                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Perform Facility Shutdown Stage         
        Facility Shutdown Stage:
            CLOSE Solenoid 1
            CLOSE Solenoid 2
            OPEN Solenoid 3
                                    *
*                                                                              *
*******************************************************************************/
FSM_STATE shutdown 
    (
    void
    )
{
/* Close Solenoid 1 and 2 */
vc_close_solenoids( SOLENOID_FUEL_VENT_1 | SOLENOID_LOX_PURGE_2 );

/* P[en Solenoid 3 */
vc_open_solenoids( SOLENOID_FUEL_PRESS_3 );

/* Shutdown LED indicator */
led_set_color( LED_BLUE );
while ( 1 ) 
    {
     /* Idle */
    };
} /* run_abort_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
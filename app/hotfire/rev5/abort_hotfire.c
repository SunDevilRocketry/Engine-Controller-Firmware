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
* 		run_abort_state                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Abort the engine hotfire           

        Abort Command:
            CLOSE Solenoid 2
            OPEN Solenoid 3
            OPEN Solenoid 1 for 8 seconds
            CLOSE Solenoid 1
                                    *
*                                                                              *
*******************************************************************************/
void run_abort_state 
    (
    void
    )
{
/* Close Solenoid 2 */
vc_close_solenoids( SOLENOID_LOX_PURGE_2 );

/* Open Solenoid 3 */
vc_open_solenoids( SOLENOID_FUEL_PRESS_3 );

/* Open Solenoid 1 for 8 seconds */
vc_open_solenoids( SOLENOID_FUEL_VENT_1 );
HAL_Delay( 8000 );

/* Close Solenoid 1 */
vc_close_solenoids( SOLENOID_FUEL_VENT_1 );

/* Safe State */
led_set_color( LED_RED );
while ( 1 ) 
    {
     /* Idle */
    };
} /* run_abort_state */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
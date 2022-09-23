/*******************************************************************************
*
* FILE: 
* 		sdr_assert.c
*
* DESCRIPTION: 
* 		Contains assert functions to log asserts in embedded memory or display 
*       firmware status on an led 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                                     
------------------------------------------------------------------------------*/
#include <stdbool.h>


/*------------------------------------------------------------------------------
 Project Includes                                                                     
------------------------------------------------------------------------------*/
#include "main.h"
#include "sdr_pin_defines.h"
#include "led.h"
#include "sdr_assert.h"


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		sdr_sd_assert_init                                                     *
*                                                                              *
* DESCRIPTION:                                                                 * 
*		Creates a new log file to record assert messages                       *
*                                                                              *
*******************************************************************************/
void sdr_sd_assert_init
    (	
	void
    )
{
// TODO: Implement
} /* sdr_sd_assert_init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		sdr_sd_assert                                                          *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Logs an assert message to the SD card when the expression within the   *
*       assert equates to true                                                 *
*                                                                              *
*******************************************************************************/
void sdr_sd_assert
    (	
	bool  expression, 
    char* log_message
    )
{
// TODO: Implement
} /* sdr_sd_assert */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		sdr_led_assert                                                         *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Lights up the onboard LED with a specified color when the expression   * 
*       within the assert equates to true                                      *
*                                                                              *
*******************************************************************************/
void sdr_led_assert
    (	
	bool    expression, 
    uint8_t led_color 
    )
{
// TODO: Implement
} /* sdr_led_assert */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

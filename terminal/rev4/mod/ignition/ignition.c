/*******************************************************************************
*
* FILE: 
* 		ignition.c
*
* DESCRIPTION: 
* 		Contains API function to the engine controller ignition system and 
*       contintuity readings
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
#include "ignition.h"


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		ignite                                                                 *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Asserts the ignition signal to ignite the engine ematch. Returns a     *
*       response code indicating if the ignition occured succesfully           *
*                                                                              *
*******************************************************************************/
IGN_STAT ignite
    (
	void
    )
{
/* Check for e-match/switch continuity */

/* Assert ignition signal */

return 0;
} /* ignite */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		solid_prop_cont                                                        *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Returns TRUE if there is continuity across the solid propellant wire   *
*       screw terminals                                                        *
*                                                                              *
*******************************************************************************/
bool solid_prop_cont
	(
	void
	)
{

/* Check MCU GPIO State */
uint8_t solid_prop_cont_pinstate = HAL_GPIO_ReadPin(SP_CONT_GPIO_PORT, SP_CONT_PIN);

/* Return true if GPIO state is high*/
if (solid_prop_cont_pinstate == 0)
	{
    return true;
	}
else
	{
    return false;
    }

} /* solid_prop_cont */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		nozzle_cont                                                            *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Returns TRUE if there is continuity across the nozzle wire screw       * 
*       terminals                                                              *
*                                                                              *
*******************************************************************************/
bool nozzle_cont
	(
	void
	)
{

/* Check MCU GPIO State */
uint8_t nozzle_cont_pinstate = HAL_GPIO_ReadPin(NOZ_CONT_GPIO_PORT, NOZ_CONT_PIN);

/* Return true if GPIO state is high*/
if (nozzle_cont_pinstate == 0)
	{
    return true;
	}
else
	{
    return false;
    }

} /* nozzle_cont */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		ematch_cont                                                            *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Returns TRUE if there is continuity across the ematch and switch screw * 
*       terminals                                                              *
*                                                                              *
*******************************************************************************/
bool ematch_cont
	(
	void
	)
{
/* Check MCU GPIO State */
uint8_t ematch_cont_pinstate = HAL_GPIO_ReadPin(E_CONT_GPIO_PORT, E_CONT_PIN);

/* Return true if GPIO state is high*/
if (ematch_cont_pinstate == 0)
	{
    return true;
	}
else
	{
    return false;
    }

} /* ematch_cont */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

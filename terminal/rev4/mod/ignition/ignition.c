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
} /* ematch_cont */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

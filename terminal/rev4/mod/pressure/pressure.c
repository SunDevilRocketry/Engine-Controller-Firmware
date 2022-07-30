/*******************************************************************************
*
* FILE: 
* 		pressure.c
*
* DESCRIPTION: 
* 		Contains API functions for reading data from the engine's pressure
*       transducers 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                                     
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                                     
------------------------------------------------------------------------------*/
#include "main.h"
#include "pressure.h"
#include "led.h"

/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
static uint8_t pt_gains[ NUM_PTS ];


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		pressure_get_pt_reading                                                *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Get a single pressure transducer reading                               *
*                                                                              *
*******************************************************************************/
uint16_t pressure_get_pt_reading 
	(
    PRESSURE_PT_NUM pt_num
    )
{
return 0;
} /* pressure_get_pt_reading */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		pressure_poll_pts                                                      *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Get readings from all pressure transducers                             *
*                                                                              *
*******************************************************************************/
PRESSURE_STATUS pressure_poll_pts
	(
    uint16_t* pPT_readings 
    )
{
return 0;
} /* pressure_poll_pts */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		pressure_set_gain                                                      *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Set the amplifier gain for a single pressure transducer                *
*                                                                              *
*******************************************************************************/
void pressure_set_gain
	(
    PRESSURE_PT_NUM pt_num,
    uint8_t         gain
    )
{

// TODO: Throw an assert if pt_num is out of range

/* Update gain within global pt_gains array */
pt_gains[ pt_num ] = gain;

} /* pressure_set_gain */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		pressure_set_all_gains                                                 *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Set the amplifier gain for all pressure transducers                    *
*                                                                              *
*******************************************************************************/
void pressure_set_all_gains
	(
    uint8_t gains[] 
    )
{

/* iterate over pressure transducers */
for ( int i = 0; i < NUM_PTS; ++i )
	{
	/* Update gain within global pt_gains array */
    pt_gains[i] = gains[i];
    }

} /* pressure_set_gains */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		pressure_get_gain                                                      *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Get the gain for a single pressure transducer                          *
*                                                                              *
*******************************************************************************/
uint8_t pressure_get_gain
	(
    PRESSURE_PT_NUM pt_num
    )
{

// TODO: Throw an assert if pt_num is out of range

return pt_gains[ pt_num ];

} /* pressure_get_gain */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		pressure_get_all_gains                                                 *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Get the gain for all pressure transducers                              *
*                                                                              *
*******************************************************************************/
void pressure_get_all_gains
	(
    uint8_t* pgains 
    )
{

/* Loop over all pressure transducer gains */
for ( int i = 0; i < NUM_PTS; ++i )
	{
    /* Read gain from global gains array */
    *( pgains + i ) = pt_gains[i];
    }

} /* pressure_get_all_gains */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/
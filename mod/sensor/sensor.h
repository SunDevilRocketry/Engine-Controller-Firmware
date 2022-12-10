/*******************************************************************************
*
* FILE: 
* 		sensor.h
*
* DESCRIPTION: 
* 	    Contains functions to interface between sdec terminal commands and SDR
*       sensor APIs	
*
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SENSOR_H 
#define SENSOR_H 

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
Includes 
------------------------------------------------------------------------------*/

/* GCC requires stdint.h for uint_t types */
#ifdef UNIT_TEST
	#include <stdint.h>
#endif

/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/

/* Sensor subcommand codes */
#define SENSOR_DUMP_CODE    ( 0x01 )
#define SENSOR_POLL_CODE    ( 0x02 )

/* General */
#define NUM_SENSORS         ( 10   )


/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/
typedef enum 
	{
    SENSOR_OK = 0         ,
	SENSOR_UNRECOGNIZED_OP,
	SENSOR_UNSUPPORTED_OP ,
	SENSOR_PT_FAIL        ,
    SENSOR_FAIL
    } SENSOR_STATUS;


/*------------------------------------------------------------------------------
 Public Function Prototypes 
------------------------------------------------------------------------------*/

/* Execute a sensor subcommand */
uint8_t sensor_cmd_execute
	(
	uint8_t subcommand
    );

/* Dump all sensor readings to console */
SENSOR_STATUS sensor_dump
	(
    uint32_t* pSensor_buffer 
    );


#endif /* SENSOR_H */

/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

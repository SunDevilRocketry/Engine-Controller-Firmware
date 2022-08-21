/*******************************************************************************
*
* FILE: 
* 		sensor.c
*
* DESCRIPTION: 
* 	    Contains functions to interface between sdec terminal commands and SDR
*       sensor APIs	
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                                     
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                                     
------------------------------------------------------------------------------*/
#include "main.h"
#include "sensor.h"
#include "pressure.h"
#include "led.h"

/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart1; /* USB UART handler struct        */


/*------------------------------------------------------------------------------
 Internal function prototypes 
------------------------------------------------------------------------------*/

/* Converts 32 bit ADC readings to an array of bytes to transmit over UART    */
void readings_to_bytes
	(
    uint8_t*  pDst_buffer, 
	uint32_t* pSrc_buffer
    );


/*------------------------------------------------------------------------------
 Public procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		sensor_cmd_execute                                                     *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Executes a sensor subcommand                                           *
*                                                                              *
*******************************************************************************/
uint8_t sensor_cmd_execute 
	(
    uint8_t subcommand 
    )
{
/*------------------------------------------------------------------------------
 Local Variables  
------------------------------------------------------------------------------*/
SENSOR_STATUS sensor_subcmd_status;           /* Status indicating if 
                                                 subcommand function returned 
                                                 properly                     */
uint32_t      sensor_readings[ NUM_SENSORS ]; /* Readings obtained from each 
                                                 sensor                       */
uint8_t       sensor_readings_bytes[ 4*NUM_SENSORS ];
const uint8_t       num_sensor_bytes = 4*NUM_SENSORS; /* Number of bytes to be 
                                                   transmitted back to PC     */

/*------------------------------------------------------------------------------
 Initializations  
------------------------------------------------------------------------------*/

/* Set sensor readings to zero */
for ( uint8_t i = 0; i < NUM_SENSORS; ++i )
	{
	sensor_readings[i] = 0;
    }
for ( uint8_t i =0; i < num_sensor_bytes; ++i )
	{
    sensor_readings_bytes[i] = 0;
    }


/*------------------------------------------------------------------------------
 Execute Sensor Subcommand 
------------------------------------------------------------------------------*/
switch ( subcommand )
	{

	/* Poll Sensors continuously */
    case SENSOR_POLL_CODE:
		{
		// TODO: Implement sensor poll function 
		return ( SENSOR_UNSUPPORTED_OP );
        } /* SENSOR_POLL_CODE */ 

	/* Poll sensors once and dump data on terminal */
	case SENSOR_DUMP_CODE: 
		{
		/* Tell the PC how many bytes to expect */
		HAL_UART_Transmit( &huart1,
                           &num_sensor_bytes,
                           sizeof( num_sensor_bytes ), 
                           HAL_DEFAULT_TIMEOUT );

		/* Get the sensor readings */
	    sensor_subcmd_status = sensor_dump( &sensor_readings[0] );	

		/* Transmit sensor readings to PC */
		if ( sensor_subcmd_status == SENSOR_OK )
			{
			readings_to_bytes( &sensor_readings_bytes[0], 
                               &sensor_readings[0] );
			HAL_UART_Transmit( &huart1                        , 
                               &sensor_readings_bytes[0]      , 
                               sizeof( sensor_readings_bytes ), 
                               HAL_SENSOR_TIMEOUT );
			return ( sensor_subcmd_status );
            }
		else
			{
			/* Sensor readings not recieved */
			return( SENSOR_FAIL );
            }
        } /* SENSOR_DUMP_CODE */

	/* Subcommand not recognized */
	default:
		{
		return ( SENSOR_UNRECOGNIZED_OP );
        }
    }
} /* sensor_cmd_execute */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		sensor_dump                                                            *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       reads from all sensors and transmits data back to host PC              *
*                                                                              *
*******************************************************************************/
SENSOR_STATUS sensor_dump 
	(
    uint32_t* pSensor_buffer /* Pointer to buffer where sensor data should 
                                be written */ 
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
PRESSURE_STATUS pt_status; /* Status of pressure transducers */


/*------------------------------------------------------------------------------
 Call sensor API functions 
------------------------------------------------------------------------------*/

pt_status = pressure_poll_pts( pSensor_buffer ); /* Pressure transducers */
/* Poll load cell            */
// TODO
/* Poll thermocouple         */
// TODO


/*------------------------------------------------------------------------------
 Set command status from sensor API returns 
------------------------------------------------------------------------------*/
if ( pt_status != PRESSURE_OK )
	{
	return ( SENSOR_PT_FAIL );
    }
else
	{
	return ( SENSOR_OK );
    }

} /* sensor_dump */


/*------------------------------------------------------------------------------
 Internal procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		readings_to_bytes                                                      *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Converts 32 bit ADC readings to an array of bytes to transmit over     * 
*       UART                                                                   *
*                                                                              *
* BUFFER FORMATS:                                                              *
* 		Source buffer (pSrc_buffer):                                           *
*       | reading 0 (R0) | reading 1 (R1) | reading 2 (R2) | ...               *
*       Destination buffer (pDst_buffer):                                      *
*		| R0 byte 3 | R0 byte 2 | R0 byte 1 | R0 byte 0 | R1 byte 3 | ...      *
*                                                                              *
*******************************************************************************/
void readings_to_bytes
	(
	uint8_t*  pDst_buffer, /* Output in bytes             */
	uint32_t* pSrc_buffer  /* Readings in uint32_t format */
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint8_t reading_bytes[4]; /* 32 bit reading split into 4 bytes, index 0 is the 
                             most significant byte  */

/*------------------------------------------------------------------------------
 Make Conversions 
------------------------------------------------------------------------------*/

/* Iterate over sensors */
for ( uint8_t i = 0; i < NUM_SENSORS; ++i )
	{
	/* Iterate over bytes in each reading */
	for ( uint8_t j = 0; j < sizeof(uint32_t); ++j )
		{

		/* Parse bits */
		reading_bytes[j] = (uint8_t) ( *(pSrc_buffer + i) >> 8*(3-j) );

		/* Export bytes to destination buffer */
		*( pDst_buffer + 4*i + j ) = reading_bytes[j];
        }
    }
}


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

/*******************************************************************************
*
* FILE: 
* 		data_logger.c
*
* DESCRIPTION: 
* 	    Contains procedures for logging data onto the engine controller's 
*       external flash chip	
*
*******************************************************************************/

/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/
#include <string.h>


/*------------------------------------------------------------------------------
 Project Includes                                                               
------------------------------------------------------------------------------*/
#include "data_logger.h"
#include "flash.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/

/* Flash address to use for logging flight data */
static uint32_t     data_logger_addr       = 0;
static uint32_t     data_logger_rel_addr   = 0;

/* Timer */
static uint32_t     data_logger_start_time = 0; 

/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/

/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		data_logger_init_timer                                                 *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initialize the data logger timer                                       *
*                                                                              *
*******************************************************************************/
void data_logger_init_timer
    (
    void
    )
{
data_logger_start_time = HAL_GetTick();
} /* data_logger_init_timer */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		data_logger_log_data                                                   *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Writes sensor data in flash using the flash header                     *
*                                                                              *
*******************************************************************************/
DATA_LOG_STATUS data_logger_log_data
    (
    DATA_LOG_DATA_FRAME data_frame /* data to be logged to flash */
    )
{
/*------------------------------------------------------------------------------
 Local variables 
------------------------------------------------------------------------------*/
FLASH_STATUS  flash_status;                 /* Flash API return codes         */
HFLASH_BUFFER flash_handle;                 /* Flash API struct               */
uint8_t       flash_buffer[ sizeof( DATA_LOG_DATA_FRAME ) ]; /* Buffer to be 
                                                            read by flash API */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
flash_status = FLASH_OK;
memcpy( &flash_buffer[0], &data_frame, sizeof( DATA_LOG_DATA_FRAME ) );

/* Flash write setup */
flash_handle.pbuffer   = &flash_buffer[0];
flash_handle.address   = data_logger_addr;
flash_handle.num_bytes = sizeof( DATA_LOG_DATA_FRAME );


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Out of memory check */
if ( ( data_logger_addr + sizeof( DATA_LOG_DATA_FRAME ) ) > FLASH_MAX_ADDR )
    {
    return DATA_LOG_OUT_OF_MEMORY;
    }

/* Write the data to flash */
while( flash_is_flash_busy() == FLASH_BUSY ){};
flash_status = flash_write( &flash_handle );
if ( flash_status != FLASH_OK )
    {
    return DATA_LOG_FLASH_ERROR;
    }

/* Update data logger addresses */
data_logger_addr     += sizeof( DATA_LOG_DATA_FRAME );
data_logger_rel_addr += sizeof( DATA_LOG_DATA_FRAME );
return DATA_LOG_OK;
} /* data_logger_log_data */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		data_logger_get_data                                                   *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Acquires a frame of data from sensors and timers                       *
*                                                                              *
*******************************************************************************/
DATA_LOG_STATUS data_logger_get_data
    (
    DATA_LOG_DATA_FRAME* data_ptr
    )
{
/*------------------------------------------------------------------------------
 Local variables 
------------------------------------------------------------------------------*/
SENSOR_STATUS sensor_status; /* Sensor API return codes    */
uint32_t      time;          /* Time of data acquisition   */
SENSOR_DATA   sensor_data;   /* Sensor data                */


/*------------------------------------------------------------------------------
 Initializations 
------------------------------------------------------------------------------*/
sensor_status = SENSOR_OK;
time          = 0;
memset( &sensor_data, 0, sizeof( SENSOR_DATA ) );


/*------------------------------------------------------------------------------
 Implementation 
------------------------------------------------------------------------------*/

/* Set time     */
time = HAL_GetTick() - data_logger_start_time;

/* Read sensors */
sensor_status = sensor_dump( &sensor_data );
if ( sensor_status != SENSOR_OK )
    {
    return DATA_LOG_SENSOR_ERROR;
    }

/* Export Data  */
data_ptr -> time = time;
data_ptr -> sensor_data = sensor_data;
return DATA_LOG_OK;
} /* data_logger_get_data */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		data_logger_get_time                                                   *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Returns the current time since timer was started                       *
*                                                                              *
*******************************************************************************/
uint32_t data_logger_get_time
    (
    void
    )
{
return HAL_GetTick() - data_logger_start_time;
} /* data_logger_get_time */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
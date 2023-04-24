/*******************************************************************************
*
* FILE: 
* 		data_logger.h
*
* DESCRIPTION: 
* 	    Contains procedures for logging data onto the engine controller's 
*       external flash chip	
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DATA_LOGGER_H 
#define DATA_LOGGER_H 

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
 Includes 
------------------------------------------------------------------------------*/
#include "main.h"
#include "sensor.h"


/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/

/* Status return codes */
typedef enum _DATA_LOG_STATUS
    {
    DATA_LOG_OK           ,
    DATA_LOG_OUT_OF_MEMORY,    /* Insufficient memory for data logging */
    DATA_LOG_FLASH_ERROR  ,    /* Flash API doesn't return correctly   */
    DATA_LOG_SENSOR_ERROR      /* Sensor module error                  */
    } DATA_LOG_STATUS;

/* Data frames to write to flash */
typedef struct _DATA_LOG_DATA_FRAME
    {
    uint32_t time;           /* Time of data measurement      */
    SENSOR_DATA sensor_data; /* PT, Loadcell, and Thermcouple */
    } DATA_LOG_DATA_FRAME;

/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/

/* Initialize the data logger timer */
void data_logger_init_timer
    (
    void
    );

/* Writes sensor data in flash using the flash header */
DATA_LOG_STATUS data_logger_log_data
    (
    DATA_LOG_DATA_FRAME data_frame /* data to be logged to flash */
    );

/* Acquires a frame of data from sensors and timers */
DATA_LOG_STATUS data_logger_get_data
    (
    DATA_LOG_DATA_FRAME* data_ptr
    );

/* Returns the current time since timer was started */
uint32_t data_logger_get_time
    (
    void
    );

/* Erases the flash chip and sets up pointers */
DATA_LOG_STATUS data_logger_erase_flash
    (
    void
    );

#ifdef __cplusplus
}
#endif

#endif /* DATA_LOGGER_H */

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
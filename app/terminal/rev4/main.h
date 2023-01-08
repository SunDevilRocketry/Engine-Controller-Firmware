/*******************************************************************************
*
* FILE: 
* 		main.h
*
* DESCRIPTION: 
* 		Processes commands recieved from a host PC, provides fine control over 
*       engine controller hardware resources
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 Includes                                                                    
------------------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"


/*------------------------------------------------------------------------------
 Macros  
------------------------------------------------------------------------------*/

/* General MCU HAL related macros */
#define DEF_BUFFER_SIZE        ( 16 )   /* Default size of buffer arrays      */

/* Timeouts */
#ifndef SDR_DEBUG
	#define HAL_DEFAULT_TIMEOUT    ( 10  ) /* Default timeout for polling 
											   operations                     */
	#define HAL_SENSOR_TIMEOUT     ( 40  ) /* Timeout for sensor polling      */
#else
	/* Disable timeouts when debugging */
	#define HAL_DEFAULT_TIMEOUT    ( 0xFFFFFFFF )  
	#define HAL_SENSOR_TIMEOUT     ( 0xFFFFFFFF ) 
#endif /* SDR_DEBUG */


/*------------------------------------------------------------------------------
 Exported functions prototypes                                             
------------------------------------------------------------------------------*/
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
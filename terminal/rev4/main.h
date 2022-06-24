/*******************************************************************************
*
* FILE: 
* 		main.c
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
 MCU Pin Assignments                                                                     
------------------------------------------------------------------------------*/
/* Pins */
#define STATUS_B    GPIO_PIN_12  
#define STATUS_G    GPIO_PIN_11  
#define STATUS_R    GPIO_PIN_10  

/* Ports */
#define STATUS_GPIO GPIOE
	
/*------------------------------------------------------------------------------
 Exported functions prototypes                                                                     
------------------------------------------------------------------------------*/
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/*******************************************************************************
*                                                                              *
* FILE:                                                                        *
* 		init.h                                                                 *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Contains initialization routines for MCU core and peripherals          *
*                                                                              *
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INIT_H 
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                               
------------------------------------------------------------------------------*/
#include "main.h"


/*------------------------------------------------------------------------------
 Function prototypes                                                          
------------------------------------------------------------------------------*/
void SystemClock_Config       ( void ); /* clock configuration         */
void PeriphCommonClock_Config ( void ); /* shared clock configurations */
void GPIO_Init                ( void ); /* GPIO configurations         */
void USB_UART_Init            ( void ); /* USB UART configuration      */
void FLASH_SPI_Init           ( void ); /* Flash SPI configuration     */
void PRESSURE_ADC_Init        ( void ); /* Pressure transducers ADC    */
void LOADCELL_ADC_Init        ( void ); /* Loadcell ADC                */
void Thermocouple_I2C_Init    ( void ); /* Thermocouple I2C            */


#ifdef __cplusplus
}
#endif
#endif /* INIT_H */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/
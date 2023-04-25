/*******************************************************************************
*                                                                              *
* FILE:                                                                        *
* 		stm32h7xx_it.c                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Interrupt Service routines                                             *
*                                                                              *
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/
#include "main.h"
#ifdef HOTFIRE
    #include "protocol.h"
#endif
#include "rs485.h"
#include "stm32h7xx_it.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart4;     /* Wireless/RS485 UART     */
extern uint8_t            gs_command; /* Ground station commands */


/*------------------------------------------------------------------------------
 Cortex Processor Interruption and Exception Handlers          
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		NMI_Handler                                                            *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Non-maskable interrupt                                                 *
*                                                                              *
*******************************************************************************/
void NMI_Handler
    (
    void
    )
{
while (1)
  {
  }
} /* NMI_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HardFault_Handler                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*       This function handles Hard fault interrupt                             *
*                                                                              *
*******************************************************************************/
void HardFault_Handler
    (
    void
    )
{
while (1)
    {
    }
} /* HardFault_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		MemManage_Handler                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Memory management fault                                                *
*                                                                              *
*******************************************************************************/
void MemManage_Handler
    (
    void
    )
{
while (1)
    {
    }
} /* MemManage_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		BusFault_Handler                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Pre-fetch fault, memory access fault                                   *
*                                                                              *
*******************************************************************************/
void BusFault_Handler
    (
    void
    )
{
while (1)
    {
    }
} /* BusFault_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		UsageFault_Handler                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Undefined instruction or illegal state                                 *
*                                                                              *
*******************************************************************************/
void UsageFault_Handler
    (
    void
    )
{
while (1)
    {
    }
} /* UsageFault_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		SVC_Handler                                                            *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Handles system service call via SWI instruction                        *
*                                                                              *
*******************************************************************************/
void SVC_Handler
    (
    void
    )
{
} /* SVC_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		DebugMon_Handler                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Handles debug monitor                                                  *
*                                                                              *
*******************************************************************************/
void DebugMon_Handler
    (
    void
    )
{
} /* DebugMon_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		PendSV_Handler                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Handles pendable request for system service                            *
*                                                                              *
*******************************************************************************/
void PendSV_Handler
    (
    void
    )
{
} /* PendSV_Handler */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		SysTick_Handler                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Handles system tick timer                                              *
*                                                                              *
*******************************************************************************/
void SysTick_Handler
    (
    void
    )
{
HAL_IncTick();
} /* SysTick_Handler */

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

#ifdef HOTFIRE
/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		UART4_IRQHandler                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*       Handles interrupts from the Wireless/RS485 UART interface              *
*                                                                              *
*******************************************************************************/
void UART4_IRQHandler
    (
    void
    )
{
/* HAL Error Handling */
HAL_UART_IRQHandler( &huart4 );

/* Process the ground station command */
protocol_command_handler( gs_command );

/* Continue listening when done */
rs485_receive_IT( &gs_command, sizeof( gs_command) );
} /* UART4_IRQHandler */
#endif



/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
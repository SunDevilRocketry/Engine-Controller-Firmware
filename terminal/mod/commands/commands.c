/*******************************************************************************
  * @file           : commands.c
  * @brief          : Terminal command functions
  ******************************************************************************
  *
  * This source c file contains the implementation of all command processing
  * functions for the Liquid Engine Controller
  *
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "commands.h"

/*******************************************************************************
* PROCEDURE: ping                                                              *
*                                                                              *
* DESCRIPTION: Sends a 1 byte response back to host PC to signal a             *
*              functioning serial connection                                   *
*******************************************************************************/
void ping
    (
     UART_HandleTypeDef *huart /* UART Handle */
    )
    {
        /* L0002 Rev 3.0 Response Code */
        uint8_t response = 0x01;

        /* Transmit Response to USB port */
        HAL_UART_Transmit(huart, &response, 1, 1);
    }

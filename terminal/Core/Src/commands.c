/*******************************************************************************
  * @file           : commands.c
  * @brief          : Terminal command functions
  ******************************************************************************
  *
  * This source c file contains the implementation of all command processing
  * commands for the Liquid Engine Controller
  *
  *****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "commands.h"

/* Ping Command: 
   Sends a response to the PC*/
void ping(UART_HandleTypeDef *huart){
    // L0002 Rev 3.0 Response
    uint8_t response = 0x01;

    // Send Response 
    HAL_UART_Transmit(huart, &response, 1, 1);
}

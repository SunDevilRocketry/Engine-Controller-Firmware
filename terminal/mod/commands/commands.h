/*******************************************************************************
  * @file           : commands.c
  * @brief          : Terminal command functions
  ******************************************************************************
  *
  * This source c file contains the implementation of all command processing
  * commands for the Liquid Engine Controller
  *
  *****************************************************************************/

// Command Opcodes
#define PING_OP 0x01

// Function Prototypes
void ping(UART_HandleTypeDef *huart);

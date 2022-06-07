/*******************************************************************************
*
* FILE: 
* 		commands.c
*
* DESCRIPTION: 
* 		Contains general command functions common to all embedded controllers
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/
#define PING_OP 0x01 /* ping command opcode */

/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/
void ping
	(
	UART_HandleTypeDef *huart /* UART handler */
	); /* ping command */


#endif /* COMMANDS_H */

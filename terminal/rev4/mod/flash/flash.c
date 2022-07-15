/*******************************************************************************
*
* FILE: 
* 		flash.c
*
* DESCRIPTION: 
* 		Contains API functions for writing and reading data from the engine 
*       controller's flash 
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                                     
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                                     
------------------------------------------------------------------------------*/
#include "main.h"
#include "flash.h"
#include "led.h"


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		flash_cmd_execute                                                      *
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Executes a flash subcommand based on input from the sdec terminal      *
*                                                                              *
*******************************************************************************/
FLASH_CMD_STATUS flash_cmd_execute
	(
    uint8_t       subcommand,
	PFLASH_BUFFER pflash_buffer
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint8_t          opcode;     /* Subcommand opcode                   */
uint8_t          num_bytes;  /* Number of bytes on which to operate */

/*------------------------------------------------------------------------------
 Command Input processing 
------------------------------------------------------------------------------*/
opcode    = ( subcommand & FLASH_SUBCMD_OP_BITMASK ) >>  5;
num_bytes = ( subcommand & FLASH_NBYTES_BITMASK    ); 

/*------------------------------------------------------------------------------
 Call API function 
------------------------------------------------------------------------------*/
switch(opcode)
	{
    /* READ Subcommand */
    case FLASH_SUBCMD_READ:
        {
		return FLASH_UNSUPPORTED_OP;	
        }

    /* ENABLE Subcommand */
    case FLASH_SUBCMD_ENABLE:
        {
		flash_write_enable(pflash_buffer);
		return FLASH_OK;
        }

    /* DISABLE Subcommand */
    case FLASH_SUBCMD_DISABLE:
        {
		flash_write_disable(pflash_buffer);
		return FLASH_OK;
        }

    /* WRITE Subcommand */
    case FLASH_SUBCMD_WRITE:
        {
	    return FLASH_UNSUPPORTED_OP;	
        }

    /* ERASE Subcommand */
    case FLASH_SUBCMD_ERASE:
        {
	    return FLASH_UNSUPPORTED_OP;	
        }

    /* Unrecognized subcommand code: invoke error handler */
	default:
        {
	    return FLASH_UNRECOGNIZED_OP;	
        }
    }
} /* flash_cmd_execute */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		flash_write_enable                                                     *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Enable writing to the external flash chip                              *
*                                                                              *
*******************************************************************************/
void flash_write_enable
    (
    PFLASH_BUFFER pflash_buffer
    )
{
/* Set WP MCU pin to HIGH */
HAL_GPIO_WritePin(FLASH_WP_GPIO_PORT, FLASH_WP_PIN, GPIO_PIN_SET);
} /* flash_write_enable */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		flash_write_disable                                                    *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       Disable writing to the external flash chip                             *
*                                                                              *
*******************************************************************************/
void flash_write_disable
    (
    PFLASH_BUFFER pflash_buffer
    )
{
/* Set WP MCU pin to LOW */
HAL_GPIO_WritePin(FLASH_WP_GPIO_PORT, FLASH_WP_PIN, GPIO_PIN_RESET);
} /* flash_write_disable */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		flash_write                                                            *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       writes bytes from a flash buffer to the external flash                 *
*                                                                              *
*******************************************************************************/
void flash_write 
    (
	PFLASH_BUFFER pflash_buffer
    )
{
/*------------------------------------------------------------------------------
 API function implementation
------------------------------------------------------------------------------*/


} /* flash_write */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		flash_read                                                             *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       reads a specified number of bytes using a flash buffer                 *
*                                                                              *
*******************************************************************************/
void flash_read
    (
	PFLASH_BUFFER pflash_buffer,
    uint8_t       num_bytes
    )
{
/*------------------------------------------------------------------------------
 API function implementation
------------------------------------------------------------------------------*/


} /* flash_read */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   * 
* 		flash_erase                                                            *
*                                                                              *
* DESCRIPTION:                                                                 * 
*       erases the entire flash chip                                           *
*                                                                              *
*******************************************************************************/
void flash_erase
    (
	void
    )
{
/*------------------------------------------------------------------------------
 API function implementation
------------------------------------------------------------------------------*/


} /* flash_read */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

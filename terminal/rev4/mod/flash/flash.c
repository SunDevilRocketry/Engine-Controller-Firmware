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
void flash_cmd_execute
	(
    uint8_t       subcommand,
	PFLASH_BUFFER pflash_buffer
    )
{
/*------------------------------------------------------------------------------
 Local Variables 
------------------------------------------------------------------------------*/
uint8_t opcode;     /* Subcommand opcode                   */
uint8_t num_bytes;  /* Number of bytes on which to operate */

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
		Error_Handler();
        break;
        }

    /* ENABLE Subcommand */
    case FLASH_SUBCMD_ENABLE:
        {
		Error_Handler();
        break;
        }

    /* DISABLE Subcommand */
    case FLASH_SUBCMD_DISABLE:
        {
		Error_Handler();
        break;
        }

    /* WRITE Subcommand */
    case FLASH_SUBCMD_WRITE:
        {
		Error_Handler();
        break;
        }

    /* ERASE Subcommand */
    case FLASH_SUBCMD_ERASE:
        {
		Error_Handler();
        break;
        }

    /* Unrecognized subcommand code: invoke error handler */
	default:
        {
		Error_Handler();
        break;
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
// TODO: API function implementation
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
// TODO: API function implementation
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

/*******************************************************************************
*
* FILE: 
* 		flash.h
*
* DESCRIPTION: 
* 		Contains API functions for writing and reading data from the engine 
*       controller's flash 
*
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FLASH_H 
#define FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/

/* Buffer object with descriptive data to use with flash read/write API 
   funtions */
typedef struct _FLASH_BUFFER_TAG {
	
	/* Number of bytes in buffer */
	uint8_t num_bytes;

	/* Base flash address for read/write operations */
	uint8_t address[3];

	/* Buffer */
	uint8_t buffer[256];

} FLASH_BUFFER; 

/* Buffer handle */
typedef FLASH_BUFFER* PFLASH_BUFFER;


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/

/* Flash subcommand codes */


/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/


/* Executes a flash subcommand based on user input from the sdec terminal */
void flash_cmd_execute
	(
    uint8_t       flash_subcommand,
    PFLASH_BUFFER pflash_buffer
    );

/* Write bytes from a flash buffer to the external flash */
void flash_write 
    (
	PFLASH_BUFFER pflash_buffer
    );

/* Read a specified number of bytes using a flash buffer */
void flash_read
    (
	PFLASH_BUFFER pflash_buffer,
    uint8_t       num_bytes
    );

/* Erase the entire flash chip */
void flash_erase
    (
	void
    );

#endif /* FLASH_H */

/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

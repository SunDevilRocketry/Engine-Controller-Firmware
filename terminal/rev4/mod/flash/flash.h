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
Includes 
------------------------------------------------------------------------------*/
#include <stdbool.h>


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/

/* Flash subcommand bitmasks */
#define FLASH_SUBCMD_OP_BITMASK 0b11100000 
#define FLASH_NBYTES_BITMASK    0b00011111

/* Write protection ON/OFF States */
#define FLASH_WP_READ_ONLY      false
#define FLASH_WP_WRITE_ENABLED  true

/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/

/* Buffer object with handle to a user-definable buffer. FLASH_BUFFER struct 
   should be filled with info about the buffer size, a reference to the buffer,
   and an SPI handle */
typedef struct _FLASH_BUFFER_TAG {
	
	/* Number of bytes in buffer */
	uint8_t num_bytes;

	/* Base flash address for read/write operations */
	uint32_t address;

	/* Buffer reference */
	uint8_t* pbuffer;

    /* SPI Handle */
    SPI_HandleTypeDef hspi;

    /* Write protection state */
    bool write_protect;

} FLASH_BUFFER; 

/* Buffer handle */
typedef FLASH_BUFFER* PFLASH_BUFFER;

/* Flash subcommand codes */
enum FLASH_SUBCMD_CODES {
    FLASH_SUBCMD_READ = 0,
    FLASH_SUBCMD_ENABLE  ,
    FLASH_SUBCMD_DISABLE ,
    FLASH_SUBCMD_WRITE   ,
    FLASH_SUBCMD_ERASE   
};


/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/

/* Executes a flash subcommand based on user input from the sdec terminal */
void flash_cmd_execute
	(
    uint8_t       flash_subcommand,
    PFLASH_BUFFER pflash_buffer
    );

/* Enable writing to the external flash chip */
void flash_write_enable 
    (
    PFLASH_BUFFER pflash_buffer 
    );

/* Disable writing to the external flash chip */
void flash_write_disable
    (
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

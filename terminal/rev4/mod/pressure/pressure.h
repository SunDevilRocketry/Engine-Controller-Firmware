/*******************************************************************************
*
* FILE: 
* 		pressure.h
*
* DESCRIPTION: 
* 		Contains API functions for reading data from the engine's pressure
*       transducers 
*
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PRESSURE_H 
#define PRESSURE_H 

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

/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/

/* Pressure transducer subcommand codes */
typedef enum PRESSURE_SUBCMD_CODES {
    PRESSURE_SUBCMD_DUMP = 0,
    PRESSURE_SUBCMD_POLL    
} PRESSURE_SUBCMD_CODE;

/* Pressure transducer subcommand codes */
typedef enum PRESSURE_CMD_STATUS {
	PRESSURE_OK = 0,
    PRESSURE_FAIL        
} PRESSURE_CMD_STATUS;


/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/

/* Executes a pressure subcommand based on input from the sdec terminal */
int pressure_cmd_execute
	(
    uint8_t subcommand
    );

/* template for module functions */
void procedure_template 
	(
    void
    );


#endif /* PRESSURE_H */

/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

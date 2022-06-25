/*******************************************************************************
*
* FILE: 
* 		ignition.c
*
* DESCRIPTION: 
* 		Contains API function to the engine controller ignition system and 
*       contintuity readings
*
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IGNITION_H
#define IGNITION_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/
typedef uint8_t IGN_STAT;


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/


/* Ignite Engine to initialize combustion                            */
IGN_STAT ignite
	(
    void
	); 

/* Check for continuity across solid propellant wire screw terminals */
bool solid_prop_cont
	(
    void
    );

/* Check for continuity across nozzle wire screw terminals           */
bool nozzle_cont
	(
    void
    );

/* Check for continuity across ematch and switch screw terminals     */
bool ematch_cont
	(
    void
    );


#endif /* IGNITION_H */

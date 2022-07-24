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


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/
#define NUM_PTS    ( 8 )  /* Number of pressure transducers */


/*------------------------------------------------------------------------------
 Typdefs 
------------------------------------------------------------------------------*/

/* Pressure transducer subcommand codes */
typedef enum PRESSURE_STATUS 
    {
	PRESSURE_OK = 0,
    PRESSURE_FAIL        
    } PRESSURE_STATUS;

/* Pressure transducer numbers */
// TODO: Change the enumeration tags to be descriptive of what
//       each pressure transducer measures 
typedef enum PRESSURE_PT_NUMS
	{
    PT_NUM0 = 0,
    PT_NUM1    ,
    PT_NUM2    ,
    PT_NUM3    ,
    PT_NUM4    ,
    PT_NUM5    ,
    PT_NUM6    ,
    PT_NUM7
    } PRESSURE_PT_NUM;

/*------------------------------------------------------------------------------
 Function Prototypes 
------------------------------------------------------------------------------*/

/* Get a single pressure transducer reading */
uint16_t pressure_get_pt_reading 
	(
    PRESSURE_PT_NUM pt_num
    );

/* Get readings from all pressure transducers */
PRESSURE_STATUS pressure_poll_pts
	(
    uint16_t* pPT_readings 
    );

/* Set the amplifier gain for a single pressure transducer */
void pressure_set_gain
	(
    PRESSURE_PT_NUM pt_num,
    uint8_t         gain
    );

/* Set the amplifier gain for all pressure transducers */
void pressure_set_all_gains
	(
    uint8_t gains[] 
    );

/* Get the gain for a single pressure transducer */
uint8_t pressure_get_gain
	(
    PRESSURE_PT_NUM pt_num
    );

/* Get the gain for all pressure transducers */
void pressure_get_all_gains
	(
    uint8_t* pgains 
    );


#endif /* PRESSURE_H */

/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/

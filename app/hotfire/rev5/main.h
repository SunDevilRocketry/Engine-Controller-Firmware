/*******************************************************************************
*
* FILE: 
* 		main.h
*
* DESCRIPTION: 
* 		Initiates engine ignition, monitors sequencing, and logs engine data 
*       during the hotfire	
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 Includes                                                                    
------------------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"


/*------------------------------------------------------------------------------
 Macros  
------------------------------------------------------------------------------*/

/* General MCU HAL related macros */
#define DEF_BUFFER_SIZE        ( 16 )   /* Default size of buffer arrays      */

/* Timeouts */
#ifndef SDR_DEBUG
	#define HAL_DEFAULT_TIMEOUT    ( 10  ) /* Default timeout for polling 
											   operations                     */
	#define HAL_SENSOR_TIMEOUT     ( 40  ) /* Timeout for sensor polling      */
#else
	/* Disable timeouts when debugging */
	#define HAL_DEFAULT_TIMEOUT    ( 0xFFFFFFFF )  
	#define HAL_SENSOR_TIMEOUT     ( 0xFFFFFFFF ) 
#endif /* SDR_DEBUG */

/* Sequence Timing */
#define TANK_PRESS_DELAY           ( 8000  ) /* 8s for stable tank pressure   */
#define LOX_CRACK_DURATION         ( 20000 ) /* 20s LOX bleed time            */
#define ENGINE_BURN_DURATION       ( 5000  ) /* 5s engine burn duration       */
#define ENGINE_IGNITION_DELAY      ( 4000  ) /* 4s delay between ignition and 
                                                opening main valves           */
#define POSTFIRE_PURGE_DURATION    ( 8000  ) /* 8s engine purge               */
#define VENT_TIMEOUT               ( 90000 ) /* 1.5 min engine vent           */
#define SAFE_TIMEOUT               ( 90000 ) /* 1.5 min safeing timeout       */ 
#define LOX_PURGE_DURATION         ( 8000  ) /* 8s engine purge               */


/*------------------------------------------------------------------------------
 Typedefs 
------------------------------------------------------------------------------*/

/* Finite State Machine States */
typedef enum _FSM_STATE
	{
	FSM_INIT_STATE          , /* Initialization state (1) */
	FSM_READY_STATE         , /* Ready State          (2) */
	FSM_PRE_FIRE_PURGE_STATE, /* Pre-Fire Purge state (3) */
	FSM_FILL_CHILL_STATE    , /* Fill-Chill state     (4) */
	FSM_STANDBY_STATE       , /* Standby state        (5) */
	FSM_FIRE_STATE          , /* Fire state           (6) */
	FSM_DISARM_STATE        , /* Disarm state         (7) */
	FSM_POST_FIRE_STATE     , /* Post-Fire state      (8) */
	FSM_MANUAL_STATE        , /* Manual control mode      */
	FSM_ABORT_STATE           /* Abort state              */
	} FSM_STATE;


/*------------------------------------------------------------------------------
 State Run Functions 
------------------------------------------------------------------------------*/

/* Ready state */
FSM_STATE run_ready_state
	(
	void
	);

/* Pre-Fire purge state */
FSM_STATE run_pre_fire_purge_state
	(
	void
	);

/* Fill/Chill state */
FSM_STATE run_fill_chill_state
	(
	void
	);

/* Standby state */
FSM_STATE run_standby_state
	(
	void
	);

/* Fire state */
FSM_STATE run_fire_state
	(
	void
	);

/* Disarm state */
FSM_STATE run_disarm_state
	(
	void
	);

/* Post-Fire state */
FSM_STATE run_post_fire_state
	(
	void
	);

/* Manual control mode */
FSM_STATE run_manual_state
	(
	void
	);

/* Hotfire abort */
void run_abort_state
	(
	void
	);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
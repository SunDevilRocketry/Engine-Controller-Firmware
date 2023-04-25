/*******************************************************************************
*
* FILE: 
* 		valve_control.h
*
* DESCRIPTION: 
* 	    Provides an interface to the valve controller for opening and closing 
*       solenoids and main valves 
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VALVE_CONTROL_H 
#define VALVE_CONTROL_H 

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Macros 
------------------------------------------------------------------------------*/

//#define NUM_SOLENOIDS    ( 6 ) /* Number of solenoids in engine */

/* Timeouts */
#ifndef SDR_DEBUG
    #define VALVE_TIMEOUT     ( 10 )
#else
    #define VALVE_TIMEOUT     ( 0xFFFFFFFF )
#endif /* #ifndef SDR_DEBUG */

/* Valve controller identification code */
#define VALVE_CONTROLLER_REV2_ID     ( 0x02 )
#define VALVE_CONTROLLER_REV3_ID     ( 0x07 )

/* Delay to allow valves to calibrate */
#define VALVE_CALIBRATION_TIME       ( 1000 ) /* 1 sec */


/*------------------------------------------------------------------------------
 Typedefs 
------------------------------------------------------------------------------*/

/* Status return codes */
typedef enum _VC_STATUS
    {
    VC_OK            , /* No error                            */ 
    VC_UART_ERROR    , /* Error in UART HAL                   */
    VC_CANNOT_CONNECT, /* Cannout connect to valve controller */
    VC_ERROR           /* Valve control error                 */
    } VC_STATUS;

/* Solenoid names */
typedef enum _SOLENOID
    {
    SOLENOID_LOX_PRESS  = 0b00000001, /* LOX Pressurization solenoid  (NC) */ 
    SOLENOID_FUEL_PRESS = 0b00000010, /* Fuel Pressurization solenoid (NC) */
    SOLENOID_LOX_VENT   = 0b00000100, /* LOX vent solenoid            (NO) */
    SOLENOID_FUEL_VENT  = 0b00001000, /* Fuel vent solenoid           (NO) */
    SOLENOID_LOX_PURGE  = 0b00010000, /* Purge solenoid, LOX side     (NO) */
    SOLENOID_FUEL_PURGE = 0b00100000  /* Purge solenoid, Fuel side    (NO) */
    } SOLENOID;

/* Main valve names */
typedef enum _MAIN_VALVE
    {
    MAIN_VALVE_LOX_MAIN , /* LOX main valve                    */ 
    MAIN_VALVE_FUEL_MAIN, /* Fuel main valve                   */
    MAIN_VALVE_BOTH_MAINS /* Both the LOX and Fuel main valves */
    } MAIN_VALVE;


/*------------------------------------------------------------------------------
 Public Function Prototypes 
------------------------------------------------------------------------------*/

/* Open a solenoid */
VC_STATUS vc_open_solenoids
    (
    SOLENOID solenoids /* Solenoids to open */
    );

/* Close a solenoid */
VC_STATUS vc_close_solenoids
    (
    SOLENOID solenoids /* Solenoids to close */
    );

/* Reset solenoids */
VC_STATUS vc_reset_solenoids
    (
    void
    );

/* Crack open a main valve */
VC_STATUS vc_crack_main_valves
    (
    MAIN_VALVE main_valves
    );

/* Open a main valve */
VC_STATUS vc_open_main_valves
    (
    MAIN_VALVE main_valves
    );

/* Close a main valve */
VC_STATUS vc_close_main_valves
    (
    MAIN_VALVE main_valve
    );

/* Calibrate the main valves */
VC_STATUS vc_calibrate_main_valves
    (
    void
    );

/* Enable the stepper motor drivers for the main valves */
VC_STATUS vc_enable_main_valves
    (
    void
    );

/* Ping the valve controller to verify the controller can be reached */
VC_STATUS vc_connect
    (
    void
    );


#ifdef __cplusplus
}
#endif

#endif /* VALVE_CONTROL_H */

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
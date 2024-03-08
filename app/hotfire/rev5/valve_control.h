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
    // SOLENOID_LOX_PRESS_1  = 0b00000001, /* LOX Pressurization solenoid  (NC) */ 
    // SOLENOID_FUEL_PRESS_2 = 0b00000010, /* Fuel Pressurization solenoid (NC) */
    // SOLENOID_LOX_VENT_3   = 0b00000100, /* LOX vent solenoid            (NO) */
    SOLENOID_FUEL_VENT_1  = 0b00001000, /* Fuel vent solenoid           (NO) */
    SOLENOID_LOX_PURGE_2  = 0b00010000, /* Purge solenoid, LOX side     (NO) */
    SOLENOID_FUEL_PURGE_3 = 0b00100000  /* Purge solenoid, Fuel side    (NO) */
    } SOLENOID;

/* Valve state -> Valves open/closed */
/* bit 7: Main Fuel Valve 
   bit 6: Main LOX Valve
   bit 5: Fuel Purge Line Solenoid 
   bit 4: LOX Purge Line Solenoid 
   bit 3: Fuel Venting Solenoid 
   bit 2: LOX Venting Solenoid 
   bit 1: Fuel Pressurization Solenoid 
   bit 0: LOX Pressurization Solenoid */
typedef uint8_t VALVE_STATES;


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

/* Gets the state of the engine engine valves (Open/Closed) */
VC_STATUS vc_getstate
    (
    VALVE_STATES* valve_state_ptr /* Pointer to output variable */
    );


#ifdef __cplusplus
}
#endif

#endif /* VALVE_CONTROL_H */

/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
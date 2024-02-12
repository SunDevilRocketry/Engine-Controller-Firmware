/*******************************************************************************
*
* FILE: 
* 		main.c
*
* DESCRIPTION: 
* 		Initiates engine ignition, monitors sequencing, and logs engine data 
*       during the hotfire	
*
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/
#include <stdbool.h>


/*------------------------------------------------------------------------------
 Project Includes                                                              
------------------------------------------------------------------------------*/

/* Application Layer */
#include "main.h"
#include "init.h"
#include "sdr_pin_defines_L0002.h"
#include "sdr_error.h"
#include "valve_control.h"
#include "terminal.h"

/* Low-level modules */
#include "commands.h"
#include "ignition.h"
#include "flash.h"
#include "led.h"
#include "power.h"
#include "pressure.h"
#include "rs485.h"
#include "sensor.h"
#include "temp.h"
#include "usb.h"
#include "valve.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
 MCU Peripheral Handles 
------------------------------------------------------------------------------*/
ADC_HandleTypeDef  hadc1;  /* Pressure transducer ADC handle */
ADC_HandleTypeDef  hadc2;  /* Load cell ADC handle           */
ADC_HandleTypeDef  hadc3;  /* Pressure transducer 5 ADC handle */
I2C_HandleTypeDef  hi2c1;  /* Thermocouple I2C handle        */
SPI_HandleTypeDef  hspi2;  /* Flash SPI handle               */
UART_HandleTypeDef huart1; /* USB UART handler struct        */
UART_HandleTypeDef huart2; /* Valve controller UART handle   */
UART_HandleTypeDef huart4; /* Wireless interface UART handle */


/*------------------------------------------------------------------------------
 Global Variables  
------------------------------------------------------------------------------*/
FSM_STATE fsm_state           = FSM_INIT_STATE; /* Finite State Machine state */
volatile uint8_t   gs_command          = 0;              /* Ground Station commands    */
volatile bool stop_hotfire_flag   = false;      /* Manual hotfire termination */
volatile bool stop_purge_flag     = false;      /* Manual purge termination   */
volatile bool lox_purge_flag      = false;      /* LOX tank purge             */
volatile bool kbottle_closed_flag = false;      /* KBottle is closed          */
volatile bool tanks_safe_flag     = false;      /* Tanks are safe             */
volatile bool telreq_wait_flag    = false;      /* Controller busy            */

/* Sensor data */
SENSOR_DATA_PING_PONG sensor_ping_pong_buffer = {0};


/*------------------------------------------------------------------------------
 Application entry point                                                      
------------------------------------------------------------------------------*/
int main
	(
 	void
	)
{
/*------------------------------------------------------------------------------
 Local Variables                                                                  
------------------------------------------------------------------------------*/

/* Flash */
HFLASH_BUFFER flash_handle;                    /* Flash API buffer handle     */
uint8_t       flash_buffer[ DEF_BUFFER_SIZE ]; /* Flash data buffer */

/* Thermocouple */
THERMO_CONFIG   thermo_config;    /* Thermocouple configuration settings        */

/* Terminal */
uint8_t         terminal_cmd;     /* Terminal command */

/* Module return codes */
FLASH_STATUS    flash_status;     /* Status of flash operations                 */
THERMO_STATUS   thermo_status;    /* Thermocouple status code                   */
RS485_STATUS    rs485_status;     /* RS485 Module return codes                  */
TERMINAL_STATUS terminal_status;  /* Return codes from SDEC terminal            */
USB_STATUS      usb_status;       /* Return codes from usb module               */


/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init                (); /* Reset peripherals, initialize flash interface 
                               and Systick.                                   */
SystemClock_Config      (); /* System clock                                   */
PeriphCommonClock_Config(); /* Shared peripheral clocks                       */
GPIO_Init               (); /* GPIO                                           */
USB_UART_Init           (); /* USB UART                                       */
Valve_UART_Init         (); /* Valve control UART                             */
Wireless_UART_Init      (); /* Wireless interface UART                        */
FLASH_SPI_Init          (); /* Flash SPI Bus                                  */
PRESSURE_ADC_Init       (); /* Pressure transducers ADC                       */
PRESSURE5_ADC_Init      (); /* Pressure transducers ADC                       */
LOADCELL_ADC_Init       (); /* Load Cell ADC                                  */
Thermocouple_I2C_Init   (); /* Thermocouple I2C                               */


/*------------------------------------------------------------------------------
 Variable Initializations 
------------------------------------------------------------------------------*/

/* Flash Buffer */
flash_handle.write_protected       = FLASH_WP_WRITE_ENABLED;
flash_handle.num_bytes             = 0;
flash_handle.address               = 0;
flash_handle.pbuffer               = &flash_buffer[0];
flash_handle.status_register       = 0xFF;
flash_handle.bpl_bits              = FLASH_BPL_NONE;
flash_handle.bpl_write_protect     = FLASH_BPL_READ_WRITE;

/* Thermocouple configuration */
thermo_config.type                 = THERMO_TYPE_K;
thermo_config.filter_coeff         = THERMO_FILTER_OFF; 
thermo_config.adc_resolution       = THERMO_18BIT_ADC;
thermo_config.cold_junc_resolution = THERMO_COLD_JUNC_MIN_RES;
thermo_config.burst_mode           = THERMO_BURST_MODE_1;
thermo_config.shutdown_mode        = THERMO_NORMAL_MODE;
thermo_config.status               = 0;

/* SDEC terminal */
terminal_cmd                       = 0;

/* Module return codes */
flash_status                       = FLASH_OK;
thermo_status                      = THERMO_OK;
terminal_status                    = TERMINAL_OK;


/*------------------------------------------------------------------------------
 External Hardware Initializations 
------------------------------------------------------------------------------*/

/* Flash Chip */
flash_status = flash_init( &flash_handle );
if ( flash_status != FLASH_OK )
	{
	Error_Handler( ERROR_FLASH_INIT_ERROR );
	}

/* Sensor module */
sensor_init();

/* Thermocouple */
thermo_status = temp_init( &thermo_config );
if ( thermo_status != THERMO_OK )
	{
	Error_Handler( ERROR_THERMO_INIT_ERROR );
	}

/* Indicate Successful MCU and Peripheral Hardware Setup */
led_set_color( LED_GREEN );


/*------------------------------------------------------------------------------
 USB Data Acquisition Mode 
------------------------------------------------------------------------------*/
// while ( usb_detect() )
// 	{
// 	/* Get sdec command from USB port */
// 	usb_status = usb_receive( &terminal_cmd, 
// 							  sizeof( terminal_cmd ), 
// 							  HAL_DEFAULT_TIMEOUT );

// 	/* Parse command input if HAL_UART_Receive doesn't timeout */
// 	if ( ( usb_status == USB_OK ) && ( terminal_cmd != 0 ) )
// 		{
// 		terminal_status = terminal_exec_cmd( terminal_cmd );
// 		if ( terminal_status != TERMINAL_OK )
// 			{
// 			Error_Handler( ERROR_TERMINAL_ERROR );
// 			}
// 		} /* if ( usb_status == USB_OK ) */
// 	}


/*------------------------------------------------------------------------------
 Initialization stage 
------------------------------------------------------------------------------*/

// /* Connect to the valve controller             */
// if ( vc_connect() != VC_OK )
// {
// 	Error_Handler( ERROR_VC_OFFLINE_ERROR );
// }

// /* Enable the main valve stepper motor drivers */
// if ( vc_enable_main_valves() != VC_OK )
// 	{
// 	Error_Handler( ERROR_VC_INIT_ERROR );
// 	}

// /* Calibrate the main propellant valves        */
// if ( vc_calibrate_main_valves() != VC_OK )
// 	{
// 	Error_Handler( ERROR_VC_INIT_ERROR );
// 	}

// HAL_Delay( VALVE_CALIBRATION_TIME ); /* Wait for calibration to finish */

// /* Reset solenoid positions                    */
// if ( vc_reset_solenoids() != VC_OK )
// 	{
// 	Error_Handler( ERROR_VC_INIT_ERROR );
// 	}

/* Enter the READY state                */
fsm_state = FSM_READY_STATE;

/* Start listening for commands from the ground station */
// rs485_status = rs485_receive_IT( (void*) &gs_command, sizeof( gs_command ) );
// if ( rs485_status != RS485_OK )
// 	{
// 	Error_Handler( ERROR_RS485_UART_ERROR );
// 	}

usb_status = usb_receive_IT( (void*) &gs_command, sizeof( gs_command ) );
if ( usb_status != USB_OK )
	{
	Error_Handler( ERROR_USB_UART_ERROR );
	}


/*------------------------------------------------------------------------------
 Hotfire Sequencing 
------------------------------------------------------------------------------*/

/* Nominal Sequence: READY > PRE-FIRE PURGE > FILL-CHILL > STANDBY >
                     FIRE  > DISARM         > POST-FLIGHT 
   Failure Contingencies : FILL_CHILL > MANUAL
                           FIRE       > ABORT 
						   DISARM     > MANUAL */
while (1)
	{
	/* Run the current state */
	switch ( fsm_state )
		{
		/* READY state */
		case FSM_READY_STATE:
			{
			fsm_state = run_ready_state();
			break;
			} /* case FSM_READY_STATE */
		
		/* PRE-FIRE PURGE state */
		case FSM_PRE_FIRE_PURGE_STATE:
			{
			fsm_state = run_pre_fire_purge_state();
			break;
			}
		
		/* FILL-CHILL state */
		case FSM_FILL_CHILL_STATE:
			{
			fsm_state = run_fill_chill_state();
			break;
			}

		/* STANDBY state */
		case FSM_STANDBY_STATE:
			{
			fsm_state = run_standby_state();
			break;
			}
		
		/* FIRE state */
		case FSM_FIRE_STATE:
			{
			fsm_state = run_fire_state();
			break;
			}
		
		/* DISARM state */
		case FSM_DISARM_STATE:
			{
			fsm_state = run_disarm_state();
			break;
			}
		
		/* POST-FIRE state */
		case FSM_POST_FIRE_STATE:
			{
			fsm_state = run_post_fire_state();
			break;
			}

		/* Manual control mode */
		case FSM_MANUAL_STATE:
			{
			fsm_state = run_manual_state();
			break;
			}

		/* ABORT state */
		case FSM_ABORT_STATE:
			{
			run_abort_state();
			break;
			}
		
		/* Invalid state */
		default:
			{
			Error_Handler( ERROR_FSM_INVALID_STATE_ERROR );
			}
		
		} /* case ( fsm_state ) */
	}
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
/*******************************************************************************
*
* FILE: 
* 		main.c
*
* DESCRIPTION: 
* 		Processes commands recieved from a host PC, provides fine control over 
*       engine controller hardware resources
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
#include "sdr_pin_defines_L0002.h"
#include "init.h"
#include "sdr_error.h"

/* Low-level modules */
#include "commands.h"
#include "ignition.h"
#include "flash.h"
#include "led.h"
#include "power.h"
#include "pressure.h"
#include "rs485.h"
#include "sensor.h"
#include "solenoid.h"
#include "temp.h"
#include "usb.h"
#include "valve.h"
#include "wireless.h"


/*------------------------------------------------------------------------------
 MCU Peripheral Handles 
------------------------------------------------------------------------------*/
ADC_HandleTypeDef  hadc1;  /* Pressure transducer ADC handle   */
ADC_HandleTypeDef  hadc2;  /* Load cell ADC handle             */
ADC_HandleTypeDef  hadc3;  /* Pressure transducer 5 ADC handle */
I2C_HandleTypeDef  hi2c1;  /* Thermocouple I2C handle          */
SPI_HandleTypeDef  hspi2;  /* Flash SPI handle                 */
UART_HandleTypeDef huart1; /* USB UART handler struct          */
UART_HandleTypeDef huart2; /* Valve controller UART handle     */
UART_HandleTypeDef huart4; /* Wireless interface UART handle   */


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
uint8_t           command;          /* SDEC command                           */
uint8_t           subcommand;       /* subcommand code                        */
uint8_t           pwr_source;       /* Power source code                      */

/* Flash */
HFLASH_BUFFER     flash_handle;                    /* Flash API buffer handle */
uint8_t           flash_buffer[ DEF_BUFFER_SIZE ]; /* Flash data buffer       */

/* Thermocouple */
THERMO_CONFIG     thermo_config;    /* Thermocouple configuration settings    */

/* Solenoids/Main valves */
SOL_STATE         sol_state;         /* State of solenoids                    */
MAIN_VALVE_STATES main_valve_states; /* States of main valves                 */

/* Module return codes */
FLASH_STATUS      flash_status;     /* Status of flash operations             */
IGN_STATUS        ign_status;       /* Ignition status code                   */
RS485_STATUS      rs485_status;     /* Status codes from RS485 module         */
THERMO_STATUS     thermo_status;    /* Thermocouple status code               */
USB_STATUS        usb_status;       /* Status of USB operations               */
VALVE_STATUS      valve_status;     /* Valve API return codes                 */


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

/* Solenoids */
sol_state                          = 0;
main_valve_states                  = 0;

/* Module return codes */
flash_status                       = FLASH_OK;
rs485_status                       = RS485_OK;
thermo_status                      = THERMO_OK;
usb_status                         = USB_OK;
valve_status                       = VALVE_OK;


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
 Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Read data from UART reciever */
	#ifdef USE_RS485
		rs485_status = rs485_receive( &command         , 
		                              sizeof( command ), 
									  RS485_DEFAULT_TIMEOUT );
	#else
		usb_status = usb_receive( &command, 
		                          sizeof( command ), 
								  HAL_DEFAULT_TIMEOUT );
	#endif

	/* Parse command input if HAL_UART_Receive doesn't timeout */
	if ( ( usb_status != USB_TIMEOUT ) && ( rs485_status != RS485_TIMEOUT ) )
		{
		switch( command )
			{
			/*-----------------------------------------------------------------
			 PING Command	
			------------------------------------------------------------------*/
			case PING_OP:
				{
				ping();
				break;
				} /* PING_OP */

			/*-----------------------------------------------------------------
			 CONNECT Command	
			------------------------------------------------------------------*/
			case CONNECT_OP:
				{
				ping();
				break;
				} /* CONNECT_OP */

			/*-----------------------------------------------------------------
			 IGNITE Command	
			------------------------------------------------------------------*/
			case IGNITE_OP:
				{
                /* Recieve ignition subcommand over USB */
                usb_status = usb_receive( &subcommand         , 
                                          sizeof( subcommand ), 
                                          HAL_DEFAULT_TIMEOUT );

                /* Execute subcommand */
                if ( usb_status != USB_TIMEOUT )
					{
					/* Execute subcommand*/
                    ign_status = ign_cmd_execute( subcommand );
                    }
				else
					{
                    /* Error: no subcommand recieved */
                    Error_Handler( ERROR_IGN_CMD_ERROR );
                    }

                /* Return response code to terminal */
                usb_transmit( &ign_status         , 
				              sizeof( ign_status ), 
							  HAL_DEFAULT_TIMEOUT );
				break;
				} /* IGNITE_OP */

			/*-----------------------------------------------------------------
			 POWER Command	
			------------------------------------------------------------------*/
			case POWER_OP:
				{
                /* Determine power source */
				pwr_source = pwr_get_source();

				/* Convert to response code and transmit to PC */
                pwr_source += 1;
				usb_transmit( &pwr_source         , 
				              sizeof( pwr_source ), 
							  HAL_DEFAULT_TIMEOUT );
				break;
				} /* POWER_OP */

			/*-----------------------------------------------------------------
			 FLASH Command	
			------------------------------------------------------------------*/
			case FLASH_OP:
				{
                /* Recieve flash subcommand over USB */
                usb_status = usb_receive( &subcommand         , 
                                          sizeof( subcommand ), 
                                          HAL_DEFAULT_TIMEOUT );
			
				/* Execute subcommand */
				if ( usb_status == USB_OK )
					{
				    flash_status = flash_cmd_execute( subcommand   , 
                                                     &flash_handle );
					}
				else
					{
					/* Subcommand code not recieved */
					Error_Handler( ERROR_FLASH_CMD_ERROR );
					}

				/* Transmit status code to PC */
				usb_transmit( &flash_status, 
                              sizeof( flash_status ), 
                              HAL_DEFAULT_TIMEOUT );
				break;
				} /* FLASH_OP */

			/*-----------------------------------------------------------------
			 SENSOR Command	
			------------------------------------------------------------------*/
			case SENSOR_OP:
				{
				/* Recieve subcommand from USB */
                usb_status = usb_receive( &subcommand         ,
                                          sizeof( subcommand ),
                                          HAL_DEFAULT_TIMEOUT );

				/* Execute subcommand */
				if ( usb_status != USB_OK )
					{
                    /* Subcommand not recieved */
                    Error_Handler( ERROR_SENSOR_CMD_ERROR );
                    }
				else
					{
                    sensor_cmd_execute( subcommand ); 
                    }

				break;
				} /* SENSOR_OP */

			/*-----------------------------------------------------------------
			 VALVE Command	
			------------------------------------------------------------------*/
			case VALVE_OP:
				{
				/* Get subcommand */
				usb_status = usb_receive( &subcommand         ,
				                          sizeof( subcommand ),
										  HAL_DEFAULT_TIMEOUT );
				if ( usb_status != USB_OK )
					{
					Error_Handler( ERROR_VALVE_CMD_ERROR );
					}
				
				/* Pass on command and subcommand to valve controller */
				valve_status = valve_transmit( &command         , 
				                               sizeof( command ), 
											   HAL_DEFAULT_TIMEOUT );
				if ( valve_status != VALVE_OK )
					{
					Error_Handler( ERROR_VALVE_CMD_ERROR );
					}
				valve_status = valve_transmit( &subcommand, 
				                               sizeof( command ),
											   HAL_DEFAULT_TIMEOUT );
				if ( valve_status != VALVE_OK )
					{
					Error_Handler( ERROR_VALVE_CMD_ERROR );
					}

				/* Pass on state if valve getstate command */
				if ( subcommand == VALVE_GETSTATE_CODE )
					{
					valve_status = valve_receive( &main_valve_states, 
					                              sizeof( main_valve_states ), 
												  HAL_DEFAULT_TIMEOUT );
					if ( valve_status != VALVE_OK )
						{
						Error_Handler( ERROR_VALVE_CMD_ERROR );
						}
					
					usb_transmit( &main_valve_states, 
					              sizeof( main_valve_states ), 
								  HAL_DEFAULT_TIMEOUT );
					}
				break;
				} /* VALVE_OP */

			/*-----------------------------------------------------------------
			 SOL Command 
			------------------------------------------------------------------*/
			case SOL_OP:
				{
				/* Get Subcommand */
				usb_status = usb_receive( &subcommand, 
							 sizeof( subcommand ), 
							 HAL_DEFAULT_TIMEOUT );
				if ( usb_status != USB_OK )
					{
					Error_Handler( ERROR_SOL_CMD_ERROR );
					}

				/* Pass on command and subcommand to valve controller */
				valve_status = valve_transmit( &command         , 
				                               sizeof( command ), 
											   HAL_DEFAULT_TIMEOUT );
				if ( valve_status != VALVE_OK )
					{
					Error_Handler( ERROR_SOL_CMD_ERROR );
					}
				valve_status = valve_transmit( &subcommand      , 
				                               sizeof( command ),
											   HAL_DEFAULT_TIMEOUT );
				if ( valve_status != VALVE_OK )
					{
					Error_Handler( ERROR_SOL_CMD_ERROR );
					}

				/* Pass on valve state if sol getstate command */
				if ( subcommand == SOL_GETSTATE_CODE )
					{
					valve_status = valve_receive( &sol_state, 
					                              sizeof( sol_state ), 
												  HAL_DEFAULT_TIMEOUT );
					if ( valve_status != VALVE_OK )
						{
						Error_Handler( ERROR_SOL_CMD_ERROR );
						}
					usb_status = usb_transmit( &sol_state,
					                           sizeof( sol_state ),
											   HAL_DEFAULT_TIMEOUT );
					if ( usb_status != USB_OK )
						{
						Error_Handler( ERROR_SOL_CMD_ERROR );
						}
					}
				break;
				} /* SOL_OP */

			/*-----------------------------------------------------------------
			 Unrecognized Command 
			------------------------------------------------------------------*/
			default:
				{
				/* Unsupported command code flash the red LED */
				//Error_Handler();
				} /* default */
			} 
		} 
	else /* USB connection times out */
		{
		/* Do nothing */
		}
	}
} /* main */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
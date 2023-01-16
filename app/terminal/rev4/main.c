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

/* Low-level modules */
#include "commands.h"
#include "ignition.h"
#include "flash.h"
#include "led.h"
#include "power.h"
#include "pressure.h"
#include "sensor.h"
#include "temp.h"
#include "usb.h"


/*------------------------------------------------------------------------------
 Global Variables                                                                  
------------------------------------------------------------------------------*/
ADC_HandleTypeDef  hadc1;  /* Pressure transducer ADC handle */
ADC_HandleTypeDef  hadc2;  /* Load cell ADC handle           */
I2C_HandleTypeDef  hi2c1;  /* Thermocouple I2C handle        */
SPI_HandleTypeDef  hspi2;  /* Flash SPI handle               */
UART_HandleTypeDef huart1; /* USB UART handler struct        */


/*------------------------------------------------------------------------------
 Typedefs                                                                  
------------------------------------------------------------------------------*/


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
uint8_t       data;             /* USB Incoming Data Buffer                   */
uint8_t       subcommand;       /* subcommand code                            */
uint8_t       pwr_source;       /* Power source code                          */

/* Flash */
HFLASH_BUFFER flash_handle;                    /* Flash API buffer handle     */
uint8_t       flash_buffer[ DEF_BUFFER_SIZE ]; /* Flash data buffer */

/* Thermocouple */
THERMO_CONFIG thermo_config;    /* Thermocouple configuration settings        */

/* Module return codes */
FLASH_STATUS  flash_status;     /* Status of flash operations                 */
IGN_STATUS    ign_status;       /* Ignition status code                       */
THERMO_STATUS thermo_status;    /* Thermocouple status code                   */
USB_STATUS    usb_status;       /* Status of USB operations                   */


/*------------------------------------------------------------------------------
 MCU Initialization                                                                  
------------------------------------------------------------------------------*/
HAL_Init();                 /* Reset peripherals, initialize flash interface and 
                               Systick.                                       */
SystemClock_Config();       /* System clock                                   */
PeriphCommonClock_Config(); /* Shared peripheral clocks                       */
GPIO_Init();                /* GPIO                                           */
USB_UART_Init();            /* USB UART                                       */
FLASH_SPI_Init();           /* Flash SPI Bus                                  */
PRESSURE_ADC_Init();        /* Pressure transducers ADC                       */
LOADCELL_ADC_Init();        /* Load Cell ADC                                  */
Thermocouple_I2C_Init();    /* Thermocouple I2C                               */


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
thermo_config.type                 = THERMO_TYPE_T;
thermo_config.filter_coeff         = THERMO_FILTER_OFF; 
thermo_config.adc_resolution       = THERMO_18BIT_ADC;
thermo_config.cold_junc_resolution = THERMO_COLD_JUNC_MIN_RES;
thermo_config.burst_mode           = THERMO_BURST_MODE_1;
thermo_config.shutdown_mode        = THERMO_NORMAL_MODE;
thermo_config.status               = 0;

/* Module return codes */
flash_status                       = FLASH_OK;
thermo_status                      = THERMO_OK;
usb_status                         = USB_OK;


/*------------------------------------------------------------------------------
 External Hardware Initializations 
------------------------------------------------------------------------------*/

/* Flash Chip */
flash_status = flash_init( &flash_handle );
if ( flash_status != FLASH_OK )
	{
//	Error_Handler();
	}

/* Sensor module */
sensor_init();

/* Thermocouple */
thermo_status = temp_init( &thermo_config );
if ( thermo_status != THERMO_OK )
	{
	Error_Handler();
	}

/* Indicate Successful MCU and Peripheral Hardware Setup */
led_set_color( LED_GREEN );

/*------------------------------------------------------------------------------
 Event Loop                                                                  
------------------------------------------------------------------------------*/
while (1)
	{
	/* Read data from UART reciever */
    usb_status = usb_receive( &data, sizeof( data ), HAL_DEFAULT_TIMEOUT );

	/* Parse command input if HAL_UART_Receive doesn't timeout */
	if ( usb_status != USB_TIMEOUT )
		{
		switch( data )
			{

			/*------------------------- Ping Command -------------------------*/
			case PING_OP:
				{
				ping();
				break;
				} /* PING_OP */

			/*------------------------ Connect Command ------------------------*/
			case CONNECT_OP:
				{
				ping();
				break;
				} /* CONNECT_OP */

			/*------------------------ Ignite Command -------------------------*/
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
                    Error_Handler();
                    }

                /* Return response code to terminal */
                usb_transmit( &ign_status         , 
				              sizeof( ign_status ), 
							  HAL_DEFAULT_TIMEOUT );
				break;
				} /* IGNITE_OP */

			/*------------------------ Power Command -------------------------*/
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

			/*------------------------ Flash Command -------------------------*/
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
					Error_Handler();
					}

				/* Transmit status code to PC */
				usb_transmit( &flash_status, 
                              sizeof( flash_status ), 
                              HAL_DEFAULT_TIMEOUT );
				break;
				} /* FLASH_OP */

			/*----------------------- Sensor Command -------------------------*/
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
                    Error_Handler();
                    }
				else
					{
                    sensor_cmd_execute( subcommand ); 
                    }

				break;
				} /* SENSOR_OP */

			/*-------------------- Unrecognized Command ----------------------*/
			default:
				{
				/* Unsupported command code flash the red LED */
				led_error_flash();
				} /* default */
			} 
		} 
	else /* USB connection times out */
		{
		/* Do nothing */
		}
	}
} /* main */




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
	led_error_assert();
    while (1)
    {
      /* application hangs when error handler is invoked */
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */


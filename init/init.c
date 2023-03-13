/*******************************************************************************
*                                                                              *
* FILE:                                                                        *
* 		init.c                                                                 *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Contains initialization routines for MCU core and peripherals          *
*                                                                              *
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 Project Includes                                                               
------------------------------------------------------------------------------*/
#include "sdr_pin_defines_L0002.h"
#include "main.h"
#include "init.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
extern ADC_HandleTypeDef  hadc1;  /* Pressure transducer ADC handle */
extern ADC_HandleTypeDef  hadc2;  /* Load cell ADC handle           */
extern I2C_HandleTypeDef  hi2c1;  /* Thermocouple I2C handle        */
extern SPI_HandleTypeDef  hspi2;  /* Flash SPI handle               */
extern UART_HandleTypeDef huart1; /* USB UART handler struct        */


/*------------------------------------------------------------------------------
 Procedures 
------------------------------------------------------------------------------*/


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		SystemClock_Config                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the microcontroller clock. Enables peripheral clocks and   *
*       sets prescalers                                                        *
*                                                                              *
*******************************************************************************/
void SystemClock_Config
	(
	void
	)
{
/* RCC Initialization Structs */
RCC_OscInitTypeDef RCC_OscInitStruct = {0};
RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

/* Supply configuration update enable */
HAL_PWREx_ConfigSupply( PWR_LDO_SUPPLY );

/* Configure the main internal regulator output voltage */
__HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE0 );

while( !__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY) ) 
	{
	/* Wait for PWR_FLAG_VOSRDY flag */
	}

/* Initialize the RCC Oscillators according to the specified parameters
* in the RCC_OscInitTypeDef structure. */
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
RCC_OscInitStruct.PLL.PLLM       = 2;
RCC_OscInitStruct.PLL.PLLN       = 80;
RCC_OscInitStruct.PLL.PLLP       = 2;
RCC_OscInitStruct.PLL.PLLQ       = 2;
RCC_OscInitStruct.PLL.PLLR       = 2;
RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
RCC_OscInitStruct.PLL.PLLFRACN   = 0;
if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
	{
    Error_Handler();
	}
else /* RCC Oscillator configuration is okay */
	{
	/* Do nothing */
	}

/* Initializes the CPU, AHB and APB buses clocks */
RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                  |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_4 ) != HAL_OK )
	{
	Error_Handler();
	}
else /* RCC Configuration okay */
	{
	/* Do Nothing */
	}

} /* SystemClock_Config */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		PeriphCommonClock_Config                                               *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the shared peripheral clocks.                              *
*                                                                              *
*******************************************************************************/
void PeriphCommonClock_Config
	(
	void
	)
{
/* Init Structs */
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

/* Initializes the peripherals clock */
PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
PeriphClkInitStruct.PLL2.PLL2M           = 2;
PeriphClkInitStruct.PLL2.PLL2N           = 16;
PeriphClkInitStruct.PLL2.PLL2P           = 4;
PeriphClkInitStruct.PLL2.PLL2Q           = 2;
PeriphClkInitStruct.PLL2.PLL2R           = 2;
PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_3;
PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOWIDE;
PeriphClkInitStruct.PLL2.PLL2FRACN       = 0;
PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL2;
if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
	{
	Error_Handler();
	}
} /* PeriphCommonClock_Config */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		PRESSURE_ADC_Init                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the MCU ADC for use with pressure transducers              *
*                                                                              *
*******************************************************************************/
void PRESSURE_ADC_Init
	(
    void
    )
{

/* Initialize config structs */
ADC_MultiModeTypeDef   multimode = {0};
ADC_ChannelConfTypeDef sConfig   = {0};

/* Configuration settings */
hadc1.Instance                      = ADC1;
hadc1.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
hadc1.Init.Resolution               = ADC_RESOLUTION_16B;
hadc1.Init.ScanConvMode             = ADC_SCAN_DISABLE;
hadc1.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
hadc1.Init.LowPowerAutoWait         = DISABLE;
hadc1.Init.ContinuousConvMode       = DISABLE;
hadc1.Init.NbrOfConversion          = 1;
hadc1.Init.DiscontinuousConvMode    = DISABLE;
hadc1.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
hadc1.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
hadc1.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
hadc1.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
hadc1.Init.OversamplingMode         = DISABLE;

/* Call MSPinit function to configure ADC registers */
if ( HAL_ADC_Init( &hadc1 ) != HAL_OK )
	{
	Error_Handler();
	}
else
	{
    /* ADC config okay, do nothing */
    }

/* Configure the ADC multi-mode */
multimode.Mode = ADC_MODE_INDEPENDENT;
if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
	Error_Handler();
	}
else
	{
    /* multi-mode configuration okay, no nothing */
    }

/* Configure Regular Channel */
sConfig.Channel                = ADC_CHANNEL_10;
sConfig.Rank                   = ADC_REGULAR_RANK_1;
sConfig.SamplingTime           = ADC_SAMPLETIME_1CYCLE_5;
sConfig.SingleDiff             = ADC_SINGLE_ENDED;
sConfig.OffsetNumber           = ADC_OFFSET_NONE;
sConfig.Offset                 = 0;
sConfig.OffsetSignedSaturation = DISABLE;
if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
else
	{
    /* ADC channel configuration okay, do nothing */
    }

} /* PRESSURE_ADC_Init */



/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		LOADCELL_ADC_Init                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the MCU ADC for use with the load cell                     *
*                                                                              *
*******************************************************************************/
void LOADCELL_ADC_Init 
	(
	void
	)
{
/* Init Structs */
ADC_ChannelConfTypeDef sConfig = {0};

/* Common config */
hadc2.Instance                      = ADC2;
hadc2.Init.ClockPrescaler           = ADC_CLOCK_ASYNC_DIV1;
hadc2.Init.Resolution               = ADC_RESOLUTION_16B;
hadc2.Init.ScanConvMode             = ADC_SCAN_DISABLE;
hadc2.Init.EOCSelection             = ADC_EOC_SINGLE_CONV;
hadc2.Init.LowPowerAutoWait         = DISABLE;
hadc2.Init.ContinuousConvMode       = DISABLE;
hadc2.Init.NbrOfConversion          = 1;
hadc2.Init.DiscontinuousConvMode    = DISABLE;
hadc2.Init.ExternalTrigConv         = ADC_SOFTWARE_START;
hadc2.Init.ExternalTrigConvEdge     = ADC_EXTERNALTRIGCONVEDGE_NONE;
hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
hadc2.Init.Overrun                  = ADC_OVR_DATA_PRESERVED;
hadc2.Init.LeftBitShift             = ADC_LEFTBITSHIFT_NONE;
hadc2.Init.OversamplingMode         = DISABLE;
if ( HAL_ADC_Init( &hadc2 ) != HAL_OK )
	{
	Error_Handler();
	}

/* Configure Regular Channel */
sConfig.Channel                     = ADC_CHANNEL_11;
sConfig.Rank                        = ADC_REGULAR_RANK_1;
sConfig.SamplingTime                = ADC_SAMPLETIME_1CYCLE_5;
sConfig.SingleDiff                  = ADC_SINGLE_ENDED;
sConfig.OffsetNumber                = ADC_OFFSET_NONE;
sConfig.Offset                      = 0;
sConfig.OffsetSignedSaturation      = DISABLE;
if ( HAL_ADC_ConfigChannel( &hadc2, &sConfig ) != HAL_OK )
	{
	Error_Handler();
	}

} /* LOADCELL_ADC_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		Thermocouple_I2C_Init                                                  *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the I2C peripheral used for communication with the         *
*       thermocouple cold-junction compensation chip                           *
*                                                                              *
*******************************************************************************/
void Thermocouple_I2C_Init
	(
	void
	)
{
/* Fill init struct */
hi2c1.Instance              = I2C1;
hi2c1.Init.Timing           = 0x307075B1;
hi2c1.Init.OwnAddress1      = 0;
hi2c1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
hi2c1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
hi2c1.Init.OwnAddress2      = 0;
hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
hi2c1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
hi2c1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
if ( HAL_I2C_Init( &hi2c1 ) != HAL_OK )
	{
	Error_Handler();
	}

/* Configure Analogue filter */
if ( HAL_I2CEx_ConfigAnalogFilter( &hi2c1, I2C_ANALOGFILTER_ENABLE ) != HAL_OK )
	{
	Error_Handler();
	}

/* Configure Digital filter */
if ( HAL_I2CEx_ConfigDigitalFilter( &hi2c1, 0 ) != HAL_OK )
	{
	Error_Handler();
	}
} /* Thermocouple_I2C_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		FLASH_SPI_Init                                                         *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the SPI peripheral used for communication with the         *
*       external flash chip                                                    *
*                                                                              *
*       Uses MCU SPI2 peripheral                                               *
*                                                                              *
*******************************************************************************/
void FLASH_SPI_Init(void)
{

/* SPI2 parameter configuration */
hspi2.Instance                        = SPI2;
hspi2.Init.Mode                       = SPI_MODE_MASTER;
hspi2.Init.Direction                  = SPI_DIRECTION_2LINES;
hspi2.Init.DataSize                   = SPI_DATASIZE_8BIT;
hspi2.Init.CLKPolarity                = SPI_POLARITY_LOW;
hspi2.Init.CLKPhase                   = SPI_PHASE_1EDGE;
hspi2.Init.NSS                        = SPI_NSS_SOFT;
hspi2.Init.BaudRatePrescaler          = SPI_BAUDRATEPRESCALER_2;
hspi2.Init.FirstBit                   = SPI_FIRSTBIT_MSB;
hspi2.Init.TIMode                     = SPI_TIMODE_DISABLE;
hspi2.Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
hspi2.Init.CRCPolynomial              = 0x0;
hspi2.Init.NSSPMode                   = SPI_NSS_PULSE_ENABLE;
hspi2.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
hspi2.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
hspi2.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
hspi2.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
hspi2.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
hspi2.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
hspi2.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
if ( HAL_SPI_Init( &hspi2 ) != HAL_OK )
	{
	Error_Handler();
	}

} /* FLASH_SPI_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE NAME:                                                              *
* 		USB_UART_Init                                                          *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the UART interface used for USB communication with a host  *
*       PC                                                                     *
*                                                                              *
*       Uses MCU USART1 peripheral                                             *
*                                                                              *
*******************************************************************************/
void USB_UART_Init
	(
 	void
	)
{
/* UART handler instance */
huart1.Instance = USART1;

/* Initialization settings */
huart1.Init.BaudRate               = 921600;
huart1.Init.WordLength             = UART_WORDLENGTH_8B;
huart1.Init.StopBits               = UART_STOPBITS_1;
huart1.Init.Parity                 = UART_PARITY_NONE;
huart1.Init.Mode                   = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
huart1.Init.OverSampling           = UART_OVERSAMPLING_16;
huart1.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
huart1.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

/* Write to registers and call error handler if initialization fails */
if (HAL_UART_Init(&huart1) != HAL_OK)
	{
	Error_Handler();
	}
if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
	Error_Handler();
	}
if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
	Error_Handler();
	}
if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
	{
	Error_Handler();
	}

} /* USB_UART_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		Valve_UART_Init                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the UART interface used for serial communication with      *
*       the valve controller                                                   *
*                                                                              *
*******************************************************************************/
void Valve_UART_Init
	(
	void
	)
{
/* Initialization Configuration */
huart2.Instance                    = USART2;
huart2.Init.BaudRate               = 921600;
huart2.Init.WordLength             = UART_WORDLENGTH_8B;
huart2.Init.StopBits               = UART_STOPBITS_1;
huart2.Init.Parity                 = UART_PARITY_NONE;
huart2.Init.Mode                   = UART_MODE_TX_RX;
huart2.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
huart2.Init.OverSampling           = UART_OVERSAMPLING_16;
huart2.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
huart2.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

/* Initialize the UART interface */
if ( HAL_UART_Init( &huart2 ) != HAL_OK )
	{
	Error_Handler();
	}
if ( HAL_UARTEx_SetTxFifoThreshold( &huart2, UART_TXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler();
	}
if ( HAL_UARTEx_SetRxFifoThreshold( &huart2, UART_RXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler();
	}
if ( HAL_UARTEx_DisableFifoMode( &huart2 ) != HAL_OK )
	{
	Error_Handler();
	}
} /* Valve_UART_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		Wireless_UART_Init                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
* 		Initializes the UART interface used for serial communication over      *
*       the wireless interface                                                 *
*                                                                              *
*******************************************************************************/
void Wireless_UART_Init
	(
	void
	)
{
/* UART Initialization configuration */
huart4.Instance                    = UART4;
huart4.Init.BaudRate               = 9600;
huart4.Init.WordLength             = UART_WORDLENGTH_8B;
huart4.Init.StopBits               = UART_STOPBITS_1;
huart4.Init.Parity                 = UART_PARITY_NONE;
huart4.Init.Mode                   = UART_MODE_TX_RX;
huart4.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
huart4.Init.OverSampling           = UART_OVERSAMPLING_16;
huart4.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
huart4.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

/* Initialize the UART peripheral */
if ( HAL_UART_Init( &huart4 ) != HAL_OK )
	{
	Error_Handler();
	}
if ( HAL_UARTEx_SetTxFifoThreshold( &huart4, UART_TXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler();
	}
if ( HAL_UARTEx_SetRxFifoThreshold( &huart4, UART_RXFIFO_THRESHOLD_1_8 ) != HAL_OK )
	{
	Error_Handler();
	}
if ( HAL_UARTEx_DisableFifoMode( &huart4 ) != HAL_OK )
	{
	Error_Handler();
	}
} /* Wireless_UART_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		GPIO_Init                                                              * 
*                                                                              *
* DESCRIPTION:                                                                 * 
* 		Initializes all GPIO pins and sets alternate functions                 *
*                                                                              *
*******************************************************************************/
void GPIO_Init
	(
 	void
	)
{
/* GPIO Initialization Struct */
GPIO_InitTypeDef GPIO_InitStruct = {0};

/* GPIO Ports Clock Enable */
__HAL_RCC_GPIOA_CLK_ENABLE();
__HAL_RCC_GPIOB_CLK_ENABLE();
__HAL_RCC_GPIOC_CLK_ENABLE();
__HAL_RCC_GPIOD_CLK_ENABLE();
__HAL_RCC_GPIOE_CLK_ENABLE();
__HAL_RCC_GPIOH_CLK_ENABLE();


/*--------------------------- LED MCU PINS -----------------------------------*/

/* Configure GPIO pin Output Level */
HAL_GPIO_WritePin(
                 STATUS_GPIO_PORT, 
                 STATUS_R_PIN | 
                 STATUS_B_PIN | 
                 STATUS_G_PIN ,
                 GPIO_PIN_SET
                 );

/* Configure GPIO pin : PE2 --> Status LED pin */
GPIO_InitStruct.Pin   = STATUS_R_PIN | 
                        STATUS_B_PIN | 
                        STATUS_G_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD; /* open-drain output   */
GPIO_InitStruct.Pull  = GPIO_NOPULL;         /* no pull up resistor */
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; /* Low Frequency       */
HAL_GPIO_Init(STATUS_GPIO_PORT, &GPIO_InitStruct);      /* Write to registers  */


/*------------------------- IGNITION MCU PIN ---------------------------------*/

/* Configure Output Level */
HAL_GPIO_WritePin(FIRE_GPIO_PORT, FIRE_PIN, GPIO_PIN_RESET);

/* Configure pin */
GPIO_InitStruct.Pin   = FIRE_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; /* push-pull output    */
GPIO_InitStruct.Pull  = GPIO_NOPULL;         /* no pull up resistor */
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; /* Low Frequency       */
HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);      /* Write to registers  */


/*--------------------- IGNITION CONTINUITY MCU PIN --------------------------*/

/* Configure pin */
GPIO_InitStruct.Pin   = E_CONT_PIN   |
                        NOZ_CONT_PIN |
                        SP_CONT_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;           /* push-pull output    */
GPIO_InitStruct.Pull  = GPIO_NOPULL;               /* no pull up resistor */
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;       /* Low Frequency       */
HAL_GPIO_Init(E_CONT_GPIO_PORT, &GPIO_InitStruct); /* Write to registers  */


/*----------------------- 5V SOURCE INDICATION PIN ----------------------------*/

/* Configure pin */
GPIO_InitStruct.Pin   = PWR_SRC_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;            /* push-pull output    */
GPIO_InitStruct.Pull  = GPIO_NOPULL;                /* no pull up resistor */
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;        /* Low Frequency       */
HAL_GPIO_Init(PWR_SRC_GPIO_PORT, &GPIO_InitStruct); /* Write to registers  */


/*--------------------------- FLASH CHIP PINS ---------------------------------*/

/* SPI Chip Select Pin */

/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin(FLASH_SS_GPIO_PORT, FLASH_SS_PIN, GPIO_PIN_SET);

/*Configure GPIO pin : PB12 */
GPIO_InitStruct.Pin   = FLASH_SS_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(FLASH_SS_GPIO_PORT, &GPIO_InitStruct);

/* Flash write protect Pin */

/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin(FLASH_WP_GPIO_PORT, FLASH_WP_PIN, GPIO_PIN_RESET);

/*Configure GPIO pin : PD12 */
GPIO_InitStruct.Pin   = FLASH_WP_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(FLASH_WP_GPIO_PORT, &GPIO_InitStruct);

/* Flash hold pin */

/*Configure GPIO pin Output Level */
HAL_GPIO_WritePin(FLASH_HOLD_GPIO_PORT, FLASH_HOLD_PIN, GPIO_PIN_SET);

/*Configure GPIO pin : PD13 */
GPIO_InitStruct.Pin   = FLASH_HOLD_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(FLASH_HOLD_GPIO_PORT, &GPIO_InitStruct);


/*--------------------------- PRESSURE TRANSDUCER PINS ------------------------*/

/* Pressure transducer gain and multiplexor pins */
HAL_GPIO_WritePin      (
                        PRESSURE_GPIO_PORT  , 
                        PRESSURE_GAIN0_PIN | 
                        PRESSURE_GAIN1_PIN | 
                        PRESSURE_GAIN2_PIN | 
                        PRESSURE_GAIN3_PIN | 
                        PRESSURE_GAIN4_PIN | 
                        PRESSURE_GAIN5_PIN | 
                        PRESSURE_GAIN6_PIN | 
                        PRESSURE_GAIN7_PIN | 
                        PRESSURE_MUXA_PIN  |
                        PRESSURE_MUXB_PIN  |
                        PRESSURE_MUXC_PIN   , 
                        GPIO_PIN_RESET 
                        );
GPIO_InitStruct.Pin   = PRESSURE_GAIN0_PIN | 
                        PRESSURE_GAIN1_PIN | 
                        PRESSURE_GAIN2_PIN | 
                        PRESSURE_GAIN3_PIN | 
                        PRESSURE_GAIN4_PIN | 
                        PRESSURE_GAIN5_PIN | 
                        PRESSURE_GAIN6_PIN | 
                        PRESSURE_GAIN7_PIN | 
                        PRESSURE_MUXA_PIN  |
                        PRESSURE_MUXB_PIN  |
                        PRESSURE_MUXC_PIN;
GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull  = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
HAL_GPIO_Init( PRESSURE_GPIO_PORT, &GPIO_InitStruct );


/*----------------------------- THERMOCOUPLE PINS ----------------------------*/

/* Thermocouple alert pins */
GPIO_InitStruct.Pin  = THERMO_ALERT_PIN | THERMO_OC_PIN | THERMO_SC_PIN;
GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init( THERMO_ALERT_GPIO_PORT, &GPIO_InitStruct );

} /* GPIO_Init */


/*******************************************************************************
* END OF FILE                                                                  * 
*******************************************************************************/
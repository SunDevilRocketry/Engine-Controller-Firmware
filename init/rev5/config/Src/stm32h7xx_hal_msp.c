/*******************************************************************************
*                                                                              *
* FILE:                                                                        *
* 		stm32h7xx_hal_msp.c                                                    *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Contains implementation of MSP initialization and de-initialization    *
*                routines                                                      *
*                                                                              *
*******************************************************************************/


/*------------------------------------------------------------------------------
 Standard Includes                                                              
------------------------------------------------------------------------------*/
#include "main.h"
#include "sdr_error.h"


/*------------------------------------------------------------------------------
 Global Variables 
------------------------------------------------------------------------------*/
static uint32_t HAL_RCC_ADC12_CLK_ENABLED = 0;


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_MspInit                                                            *
*                                                                              *
* DESCRIPTION:                                                                 *
*		Initializes the Global MSP                                             *
*                                                                              *
*******************************************************************************/
void HAL_MspInit
	(
    void
    )
{
__HAL_RCC_SYSCFG_CLK_ENABLE();
/* System interrupt init*/
} /* HAL_MspInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_ADC_MspInit                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*		ADC MSP initialization                                                 *
*                                                                              *
*******************************************************************************/
void HAL_ADC_MspInit
	(
    ADC_HandleTypeDef* hadc
    )
{
/* Initialization Structs */
GPIO_InitTypeDef         GPIO_InitStruct     = {0};

/* Pressure Transducer ADC Setup -> ADC1 */
if ( hadc->Instance == ADC1 )
	{
	/* Peripheral clock enable */
    HAL_RCC_ADC12_CLK_ENABLED++;
    if( HAL_RCC_ADC12_CLK_ENABLED ==1 )
		{
		__HAL_RCC_ADC12_CLK_ENABLE();
		}
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* ADC1 GPIO Configuration
	PC0     ------> ADC1_INP10 */
	GPIO_InitStruct.Pin  = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );
	}
/* Load Cell ADC Setup -> ADC2 */
else if( hadc->Instance == ADC2 )
	{
	/* Peripheral clock enable */
	HAL_RCC_ADC12_CLK_ENABLED++;
	if( HAL_RCC_ADC12_CLK_ENABLED ==1 )
		{
		__HAL_RCC_ADC12_CLK_ENABLE();
		}
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* ADC2 GPIO Configuration
	PC1     ------> ADC2_INP11 */
	GPIO_InitStruct.Pin  = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );
	}
/* Pressure Transducer 5 ADC Setup -> ADC3 */
else if( hadc->Instance == ADC3 )
	{
	/* Peripheral clock enable */
	__HAL_RCC_ADC3_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* ADC3 GPIO Configuration
	PC2_C     ------> ADC3_INP0 */
	HAL_SYSCFG_AnalogSwitchConfig( SYSCFG_SWITCH_PC2, SYSCFG_SWITCH_PC2_OPEN );
	}

} /* HAL_ADC_MspInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_ADC_MspDeInit                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*		ADC MSP de-initialization                                              *
*                                                                              *
*******************************************************************************/
void HAL_ADC_MspDeInit
	(
	ADC_HandleTypeDef* hadc
	)
{
/* Pressure Transducer ADC Disable */
if( hadc -> Instance == ADC1 )
	{
	/* Peripheral clock disable */
    HAL_RCC_ADC12_CLK_ENABLED--;
    if( HAL_RCC_ADC12_CLK_ENABLED == 0)
		{
		__HAL_RCC_ADC12_CLK_DISABLE();
		}

	/* ADC1 GPIO Configuration
	PC0     ------> ADC1_INP10 */
	HAL_GPIO_DeInit( GPIOC, GPIO_PIN_0 );
	}
/* Load Cell ADC Disable */
else if ( hadc->Instance == ADC2 )
	{
	/* Peripheral clock disable */
	HAL_RCC_ADC12_CLK_ENABLED--;
	if( HAL_RCC_ADC12_CLK_ENABLED == 0 )
		{
		__HAL_RCC_ADC12_CLK_DISABLE();
		}

	/* ADC2 GPIO Configuration
	PC1     ------> ADC2_INP11 */
	HAL_GPIO_DeInit( GPIOC, GPIO_PIN_1 );
	}
/* Pressure Transducer 5 ADC Disable */
else if( hadc->Instance == ADC3 )
	{
	/* Peripheral clock disable */
	__HAL_RCC_ADC3_CLK_DISABLE();
	}

} /* HAL_ADC_MspDeInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_I2C_MspInit                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*		I2C MSP initialization                                                 *
*                                                                              *
*******************************************************************************/
void HAL_I2C_MspInit
	(
	I2C_HandleTypeDef* hi2c
	)
{
/* Init Structs */
GPIO_InitTypeDef         GPIO_InitStruct     = {0};
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

/* Thermcouple I2C Config */
if( hi2c->Instance == I2C1 )
	{
	/* Initializes the peripherals clock */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
	if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
		{
		Error_Handler( ERROR_I2C_HAL_MSP_ERROR );
		}
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* I2C1 GPIO Configuration
	PB6     ------> I2C1_SCL
	PB7     ------> I2C1_SDA */
	GPIO_InitStruct.Pin       = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

	/* Peripheral clock enable */
	__HAL_RCC_I2C1_CLK_ENABLE();
	}
} /* HAL_I2C_Init */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_I2C_MspDeInit                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*		I2C MSP De-initialization                                              *
*                                                                              *
*******************************************************************************/
void HAL_I2C_MspDeInit
	(
	I2C_HandleTypeDef* hi2c
	)
{
/* Thermocouple I2C */
if( hi2c->Instance == I2C1 )
	{
	/* Peripheral clock disable */
	__HAL_RCC_I2C1_CLK_DISABLE();

	/* I2C1 GPIO Configuration
	PB6     ------> I2C1_SCL
	PB7     ------> I2C1_SDA */
	HAL_GPIO_DeInit( GPIOB, GPIO_PIN_6 );
	HAL_GPIO_DeInit( GPIOB, GPIO_PIN_7 );
	}

}  /* HAL_I2C_MspDeInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_SPI_MspInit                                                        *
*                                                                              *
* DESCRIPTION:                                                                 *
*		SPI MSP initialization                                                 *
*                                                                              *
*******************************************************************************/
void HAL_SPI_MspInit
	(
    SPI_HandleTypeDef* hspi
    )
{
/* Initialization Structs */
GPIO_InitTypeDef         GPIO_InitStruct     = {0};
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

/* Flash SPI Setup */
if( hspi->Instance == SPI2 )
	{
	/* Clock Setup */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI2;
    PeriphClkInitStruct.PLL3.PLL3M           = 2;
    PeriphClkInitStruct.PLL3.PLL3N           = 16;
    PeriphClkInitStruct.PLL3.PLL3P           = 4;
    PeriphClkInitStruct.PLL3.PLL3Q           = 2;
    PeriphClkInitStruct.PLL3.PLL3R           = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE         = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL      = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN       = 0;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL3;
    if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
		{
		Error_Handler( ERROR_SPI_HAL_MSP_ERROR );
		}

	/* Peripheral clock enable */
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* SPI2 GPIO Configuration
	PB10     ------> SPI2_SCK
	PB14     ------> SPI2_MISO
	PB15     ------> SPI2_MOSI */
	GPIO_InitStruct.Pin       = GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

	}

} /* HAL_SPI_MspInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_SPI_MspDeInit                                                      *
*                                                                              *
* DESCRIPTION:                                                                 *
*		SPI MSP de-initialization                                              *
*                                                                              *
*******************************************************************************/
void HAL_SPI_MspDeInit
	(
	SPI_HandleTypeDef* hspi
	)
{
if(hspi->Instance==SPI2)
	{
	/* Peripheral clock disable */
	__HAL_RCC_SPI2_CLK_DISABLE();

	/**SPI2 GPIO Configuration
	PB10     ------> SPI2_SCK
	PB14     ------> SPI2_MISO
	PB15     ------> SPI2_MOSI
	*/
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15);
	}

} /* HAL_SPI_MspDeInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_UART_MspInit                                                       *
*                                                                              *
* DESCRIPTION:                                                                 *
*		SPI MSP initialization                                                 *
*                                                                              *
*******************************************************************************/
void HAL_UART_MspInit
	(
	UART_HandleTypeDef* huart
	)
{
/* Initialization structs */
GPIO_InitTypeDef         GPIO_InitStruct     = {0};
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

/* Wireless interface UART */
if ( huart->Instance == UART4 )
	{
	/* Initializes the peripherals clock */
	PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_UART4;
	PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
	if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
		{
		Error_Handler( ERROR_UART_HAL_MSP_ERROR );
		}

	/* Peripheral clock enable */
	__HAL_RCC_UART4_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* UART4 GPIO Configuration
	PA0     ------> UART4_TX
	PA1     ------> UART4_RX */
	GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
	} /* if ( huart->Instance == UART4 )*/

/* USB UART */
else if ( huart->Instance == USART1 )
	{
	/* Initializes the peripherals clock */
	PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_USART1;
	PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
		{
		Error_Handler( ERROR_UART_HAL_MSP_ERROR );
		}

	/* Peripheral clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* USART1 GPIO Configuration
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX */
	GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
	} /* if ( huart->instance == USART1 ) */

/* Valve control UART */
else if( huart->Instance == USART2 )
	{
	/* Initializes the peripherals clock */
	PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_USART2;
	PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
	if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct ) != HAL_OK )
		{
		Error_Handler( ERROR_UART_HAL_MSP_ERROR );
		}

	/* Peripheral clock enable */
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX */
	GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
	} /* else if ( huart->Instance == USART2 ) */

} /* HAL_UART_MspInit */


/*******************************************************************************
*                                                                              *
* PROCEDURE:                                                                   *
* 		HAL_UART_DeMspInit                                                     *
*                                                                              *
* DESCRIPTION:                                                                 *
*		SPI MSP De-initialization                                              *
*                                                                              *
*******************************************************************************/
void HAL_UART_MspDeInit
	(
	UART_HandleTypeDef* huart
	)
{
/* Wireless interface UART */
if ( huart->Instance == UART4 )
	{
	/* Peripheral clock disable */
	__HAL_RCC_UART4_CLK_DISABLE();

	/* UART4 GPIO Configuration
	PA0     ------> UART4_TX
	PA1     ------> UART4_RX */
	HAL_GPIO_DeInit( GPIOA, GPIO_PIN_0 | GPIO_PIN_1 );
	}

/* USB UART */
if ( huart->Instance == USART1 )
	{
	/* Peripheral clock disable */
	__HAL_RCC_USART1_CLK_DISABLE();

	/* USART1 GPIO Configuration
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX */
	HAL_GPIO_DeInit( GPIOA, GPIO_PIN_9 | GPIO_PIN_10 );
	} /* if ( huart->Instance == USART1 ) */

/* Valve Control UART */
else if( huart->Instance == USART2 )
	{
	/* Peripheral clock disable */
	__HAL_RCC_USART2_CLK_DISABLE();

	/* USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX */
	HAL_GPIO_DeInit( GPIOA, GPIO_PIN_2 | GPIO_PIN_3 );
	} /* else if ( huart->Instance == USART2 ) */
 
} /* HAL_UART_DeMspInit */


/*******************************************************************************
* END OF FILE                                                                  *
*******************************************************************************/
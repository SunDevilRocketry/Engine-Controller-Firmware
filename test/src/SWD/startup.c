/************************************************************* 
* startup -- Code to initialize microcontroller specific     *
*            operations including the vector table and       *
*            default exception handlers                      *
*                                                            * 
* MCU: 	STM32L443VCT6 ARM Cortex-M4                          *
*       - 64 KBytes of SRAM                                  *
*       - 256 KBytes of Flash                                *
*                                                            * 
* Author: Colton Acosta, cacost12@asu.edu                    *
*                                                            * 
* Date: 8/1/2021                                             *
*************************************************************/ 
#include <stdint.h>

// SRAM Preprocessor Directives
#define SRAM_START 0x20000000U
#define SRAM_SIZE (64U * 1024U) // 64 KB
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))

// Initialize Stack pointer to end of SRAM
#define STACK_START    SRAM_END

// Use linker symbols to access .text and .data regions
// of Flash memory 
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _ebss;
extern uint32_t _sbss;

// main prototype
int main(void);

// System Exception Prototypes
void Reset_Handler(void);
void NMI_Handler(void)                  __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)            __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler(void)            __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)             __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)           __attribute__ ((weak, alias("Default_Handler")));
void SVCall_Handler(void)               __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler(void)             __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)               __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)              __attribute__ ((weak, alias("Default_Handler")));

// Interrupt Prototypes
void WWDG_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void RTC_TAMP_STAMP_IRQHandler(void)    __attribute__ ((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)             __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH1_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH2_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH3_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH4_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH5_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH6_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA1_CH7_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void CAN1_TX_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler(void)    __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void)      __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void USART3_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void RTCAlarm_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void SDMMC1_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void TIM6_DACUNDER_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH1_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH2_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH3_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH4_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH5_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void COMP_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void LPTIM1_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void LPTIM2_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void USB_FS_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH6_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DMA2_CH7_IRQHandler(void)          __attribute__ ((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void QUADSPI_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void)              __attribute__ ((weak, alias("Default_Handler")));
void SWPMI1_IRQHandler(void)            __attribute__ ((weak, alias("Default_Handler")));
void TSC_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void LCD_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void AES_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void RNG_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void FPU_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));
void CRS_IRQHandler(void)               __attribute__ ((weak, alias("Default_Handler")));

// Instantiate Vector table array
uint32_t vectors[] __attribute__ ((section(".isr_vector"))) = {
    STACK_START,
    (uint32_t)&Reset_Handler, 
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&SVCall_Handler,
    (uint32_t)&DebugMon_Handler,
    0, 
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,
    (uint32_t)&WWDG_IRQHandler,
    (uint32_t)&PVD_PVM_IRQHandler,
    (uint32_t)&RTC_TAMP_STAMP_IRQHandler,
    (uint32_t)&RTC_WKUP_IRQHandler,
    (uint32_t)&FLASH_IRQHandler,
    (uint32_t)&RCC_IRQHandler,
    (uint32_t)&EXTI0_IRQHandler,
    (uint32_t)&EXTI1_IRQHandler,
    (uint32_t)&EXTI2_IRQHandler,
    (uint32_t)&EXTI3_IRQHandler,
    (uint32_t)&EXTI4_IRQHandler,
    (uint32_t)&DMA1_CH1_IRQHandler,
    (uint32_t)&DMA1_CH2_IRQHandler,
    (uint32_t)&DMA1_CH3_IRQHandler,
    (uint32_t)&DMA1_CH4_IRQHandler,
    (uint32_t)&DMA1_CH5_IRQHandler,
    (uint32_t)&DMA1_CH6_IRQHandler,
    (uint32_t)&DMA1_CH7_IRQHandler,
    (uint32_t)&ADC1_2_IRQHandler,
    (uint32_t)&CAN1_TX_IRQHandler,
    (uint32_t)&CAN1_RX0_IRQHandler,
    (uint32_t)&CAN1_RX1_IRQHandler,
    (uint32_t)&CAN1_SCE_IRQHandler,
    (uint32_t)&EXTI9_5_IRQHandler,
    (uint32_t)&TIM1_BRK_IRQHandler,
    (uint32_t)&TIM1_UP_IRQHandler,
    (uint32_t)&TIM1_TRG_COM_IRQHandler,
    (uint32_t)&TIM1_CC_IRQHandler, 
    (uint32_t)&TIM2_IRQHandler,
    0,
    0,
    (uint32_t)&I2C1_EV_IRQHandler,
    (uint32_t)&I2C1_ER_IRQHandler,
    (uint32_t)&I2C2_EV_IRQHandler,
    (uint32_t)&I2C2_ER_IRQHandler,
    (uint32_t)&SPI1_IRQHandler,
    (uint32_t)&SPI2_IRQHandler,
    (uint32_t)&USART1_IRQHandler,
    (uint32_t)&USART2_IRQHandler,
    (uint32_t)&USART3_IRQHandler,
    (uint32_t)&EXTI15_10_IRQHandler,
    (uint32_t)&RTCAlarm_IRQHandler,
    0, 
    0,
    0,
    0,
    0,
    0,
    0,
    (uint32_t)&SDMMC1_IRQHandler,
    0,
    (uint32_t)&SPI3_IRQHandler,
    0,
    0, 
    (uint32_t)&TIM6_DACUNDER,
    (uint32_t)&TIM7_IRQHandler,
    (uint32_t)&DMA2_CH1_IRQHandler,
    (uint32_t)&DMA2_CH2_IRQHandler,
    (uint32_t)&DMA2_CH3_IRQHandler,
    (uint32_t)&DMA2_CH4_IRQHandler,
    (uint32_t)&DMA2_CH5_IRQHandler,
    0, 
    0,
    0,
    (uint32_t)&COMP_IRQHandler,
    (uint32_t)&LPTIM1_IRQHandler,
    (uint32_t)&LPTIM2_IRQHandler,
    (uint32_t)&USB_FS_IRQHandler,
    (uint32_t)&DMA2_CH6,
    (uint32_t)&DMA2_CH7,
    (uint32_t)&LPUART1_IRQHandler,
    (uint32_t)&QUADSPI_IRQHandler,
    (uint32_t)&I2C3_EV_IRQHandler,
    (uint32_t)&I2C3_ER_IRQHandler,
    (uint32_t)&SAI1_IRQHandler,
    0,
    (uint32_t)&SWPMI1_IRQHandler,
    (uint32_t)&TSC_IRQHandler,
    (uint32_t)&LCD_IRQhandler,
    (uint32_t)&AES_IRQHandler,
    (uint32_t)&RNG_IRQHandler,
    (uint32_t)&FPU_IRQHandler,
    (uint32_t)&CRS_IRQhandler
}; 

void Default_Handler(void){

    // Function Hangs when fault occurs
    while(1);
}

void Reset_Handler(void){
    // copy the .data section to SRAM
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint32_t *pDest = (uint32_t*) &_sdata; // Flash
    uint32_t *pSrc = (uint32_t*) &_etext; // SRAM
    for (uint32_t i = 0; i < size; i++){
        *pDest++ = *pSrc++;
    }

    // Initialize the .bss section to zero in SRAM
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDest = &_sbss;
    for (uint32_t i = 0; i < size; i++){
        *pDest++ = 0;
    }

    // Call init function of std. library

    // Call main()
    main();
}

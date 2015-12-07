/*
 * main.c
 *
 *  Created on: Dec 7, 2015
 *      Author: petera
 */

#include <target.h>

static void delay(uint32_t loops);
static void processor_init(void);

int main(void) {
  GPIO_InitTypeDef gpio_init_structure;

  processor_init();

  __HAL_RCC_GPIOI_CLK_ENABLE();

  HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1);

  gpio_init_structure.Pin = GPIO_PIN_1;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  //gpio_init_structure.Alternate = 0; // nevermind
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  while (1) {
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
    delay(100000);
  }

  return 0;
}

static void delay(uint32_t loops) {
  volatile uint32_t a = loops;
  while (--a)
    ;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

static void MPU_Config (void) {
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void processor_init(void) {
  // enable caches
  SCB_EnableICache();
  SCB_EnableDCache();

  // test for avoiding systemclock_config usagefault hang
  //MPU_Config();

  // test for avoiding systemclock_config usagefault hang
  //HAL_Init();

  // makes ourselves hang in a usage fault after x cycles in delay()
  // pump up the jam to 200MHz
  //SystemClock_Config();
}

static void def_irq(void) {
  while(1);
}

void WWDG_IRQHandler(void) { def_irq(); }
void PVD_IRQHandler(void) { def_irq(); }
void TAMP_STAMP_IRQHandler(void) { def_irq(); }
void RTC_WKUP_IRQHandler(void) { def_irq(); }
void FLASH_IRQHandler(void) { def_irq(); }
void RCC_IRQHandler(void) { def_irq(); }
void EXTI0_IRQHandler(void) { def_irq(); }
void EXTI1_IRQHandler(void) { def_irq(); }
void EXTI2_IRQHandler(void) { def_irq(); }
void EXTI3_IRQHandler(void) { def_irq(); }
void EXTI4_IRQHandler(void) { def_irq(); }
void DMA1_Stream0_IRQHandler(void) { def_irq(); }
void DMA1_Stream1_IRQHandler(void) { def_irq(); }
void DMA1_Stream2_IRQHandler(void) { def_irq(); }
void DMA1_Stream3_IRQHandler(void) { def_irq(); }
void DMA1_Stream4_IRQHandler(void) { def_irq(); }
void DMA1_Stream5_IRQHandler(void) { def_irq(); }
void DMA1_Stream6_IRQHandler(void) { def_irq(); }
void ADC_IRQHandler(void) { def_irq(); }
void CAN1_TX_IRQHandler(void) { def_irq(); }
void CAN1_RX0_IRQHandler(void) { def_irq(); }
void CAN1_RX1_IRQHandler(void) { def_irq(); }
void CAN1_SCE_IRQHandler(void) { def_irq(); }
void EXTI9_5_IRQHandler(void) { def_irq(); }
void TIM1_BRK_TIM9_IRQHandler(void) { def_irq(); }
void TIM1_UP_TIM10_IRQHandler(void) { def_irq(); }
void TIM1_TRG_COM_TIM11_IRQHandler(void) { def_irq(); }
void TIM1_CC_IRQHandler(void) { def_irq(); }
void TIM2_IRQHandler(void) { def_irq(); }
void TIM3_IRQHandler(void) { def_irq(); }
void TIM4_IRQHandler(void) { def_irq(); }
void I2C1_EV_IRQHandler(void) { def_irq(); }
void I2C1_ER_IRQHandler(void) { def_irq(); }
void I2C2_EV_IRQHandler(void) { def_irq(); }
void I2C2_ER_IRQHandler(void) { def_irq(); }
void SPI1_IRQHandler(void) { def_irq(); }
void SPI2_IRQHandler(void) { def_irq(); }
void USART1_IRQHandler(void) { def_irq(); }
void USART2_IRQHandler(void) { def_irq(); }
void USART3_IRQHandler(void) { def_irq(); }
void EXTI15_10_IRQHandler(void) { def_irq(); }
void RTC_Alarm_IRQHandler(void) { def_irq(); }
void OTG_FS_WKUP_IRQHandler(void) { def_irq(); }
void TIM8_BRK_TIM12_IRQHandler(void) { def_irq(); }
void TIM8_UP_TIM13_IRQHandler(void) { def_irq(); }
void TIM8_TRG_COM_TIM14_IRQHandler(void) { def_irq(); }
void TIM8_CC_IRQHandler(void) { def_irq(); }
void DMA1_Stream7_IRQHandler(void) { def_irq(); }
void FMC_IRQHandler(void) { def_irq(); }
void SDMMC1_IRQHandler(void) { def_irq(); }
void TIM5_IRQHandler(void) { def_irq(); }
void SPI3_IRQHandler(void) { def_irq(); }
void UART4_IRQHandler(void) { def_irq(); }
void UART5_IRQHandler(void) { def_irq(); }
void TIM6_DAC_IRQHandler(void) { def_irq(); }
void TIM7_IRQHandler(void) { def_irq(); }
void DMA2_Stream0_IRQHandler(void) { def_irq(); }
void DMA2_Stream1_IRQHandler(void) { def_irq(); }
void DMA2_Stream2_IRQHandler(void) { def_irq(); }
void DMA2_Stream3_IRQHandler(void) { def_irq(); }
void DMA2_Stream4_IRQHandler(void) { def_irq(); }
void ETH_IRQHandler(void) { def_irq(); }
void ETH_WKUP_IRQHandler(void) { def_irq(); }
void CAN2_TX_IRQHandler(void) { def_irq(); }
void CAN2_RX0_IRQHandler(void) { def_irq(); }
void CAN2_RX1_IRQHandler(void) { def_irq(); }
void CAN2_SCE_IRQHandler(void) { def_irq(); }
void OTG_FS_IRQHandler(void) { def_irq(); }
void DMA2_Stream5_IRQHandler(void) { def_irq(); }
void DMA2_Stream6_IRQHandler(void) { def_irq(); }
void DMA2_Stream7_IRQHandler(void) { def_irq(); }
void USART6_IRQHandler(void) { def_irq(); }
void I2C3_EV_IRQHandler(void) { def_irq(); }
void I2C3_ER_IRQHandler(void) { def_irq(); }
void OTG_HS_EP1_OUT_IRQHandler(void) { def_irq(); }
void OTG_HS_EP1_IN_IRQHandler(void) { def_irq(); }
void OTG_HS_WKUP_IRQHandler(void) { def_irq(); }
void OTG_HS_IRQHandler(void) { def_irq(); }
void DCMI_IRQHandler(void) { def_irq(); }
void RNG_IRQHandler(void) { def_irq(); }
void FPU_IRQHandler(void) { def_irq(); }
void UART7_IRQHandler(void) { def_irq(); }
void UART8_IRQHandler(void) { def_irq(); }
void SPI4_IRQHandler(void) { def_irq(); }
void SPI5_IRQHandler(void) { def_irq(); }
void SPI6_IRQHandler(void) { def_irq(); }
void SAI1_IRQHandler(void) { def_irq(); }
void LTDC_IRQHandler(void) { def_irq(); }
void LTDC_ER_IRQHandler(void) { def_irq(); }
void DMA2D_IRQHandler(void) { def_irq(); }
void SAI2_IRQHandler(void) { def_irq(); }
void QUADSPI_IRQHandler(void) { def_irq(); }
void LPTIM1_IRQHandler(void) { def_irq(); }
void CEC_IRQHandler(void) { def_irq(); }
void I2C4_EV_IRQHandler(void) { def_irq(); }
void I2C4_ER_IRQHandler(void) { def_irq(); }
void SPDIF_RX_IRQHandler(void) { def_irq(); }


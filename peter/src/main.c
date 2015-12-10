/*
 * main.c
 *
 *  Created on: Dec 7, 2015
 *      Author: petera
 */

#include <target.h>
#include <uart_driver.h>
#include <miniutils.h>
#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include <stdarg.h>
#include <app.h>

extern uint8_t FLASH_START, FLASH_SIZE, RAM_START, RAM_SIZE;

static void uart_cb(void *arg, u8_t c);
static void SystemClock_Config(void);
static void MPU_Config(void);
static void LCD_Config(void);
static void sdram_init_seq(SDRAM_HandleTypeDef *hsdram,
    FMC_SDRAM_CommandTypeDef *Command);
static void sdram_test(void);

SDRAM_HandleTypeDef      hsdram;
FMC_SDRAM_TimingTypeDef  SDRAM_Timing;
FMC_SDRAM_CommandTypeDef command;

static void print_hello() {
  print("\nProcessor running...\n\n");
  print("FLASH: 0x%08x -- 0x%08x (%ikB)\n",
      (u32_t)&FLASH_START, (u32_t)&FLASH_START+(u32_t)&FLASH_SIZE, (u32_t)&FLASH_SIZE/1024);
  print("RAM:   0x%08x -- 0x%08x (%ikB)\n",
      (u32_t)&RAM_START, (u32_t)&RAM_START+(u32_t)&RAM_SIZE, (u32_t)&RAM_SIZE/1024);
  print("\nProcessor clock speeds\n"
        "======================\n"
        "HCLK:   %i\n"
        "PCLK1:  %i\n"
        "PCLK2:  %i\n"
        "SysClk: %i\n",
        HAL_RCC_GetHCLKFreq(),
        HAL_RCC_GetPCLK1Freq(),
        HAL_RCC_GetPCLK2Freq(),
        HAL_RCC_GetSysClockFreq()
        );


}

static void init_processor(void) {
  // enable I&D caches
  SCB_EnableICache();
  SCB_EnableDCache();

  MPU_Config();

  HAL_Init();

  // pump up the jam to 216MHz
  SystemClock_Config();
}

static void init_sdram(void) {
  GPIO_InitTypeDef  GPIO_Init_Structure;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clocks */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /* Enable FMC clock */
  __HAL_RCC_FMC_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;


  /* GPIOC configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  /* GPIOD configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 |
                              GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  /* GPIOE configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9       |\
                              GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

  /* GPIOF configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4      |\
                              GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);

  /* GPIOG configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_8 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);

  /* GPIOH configuration */
  GPIO_Init_Structure.Pin   = GPIO_PIN_3 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);

  /*##-1- Configure the SDRAM device #########################################*/
  /* SDRAM device configuration */
  hsdram.Instance = FMC_SDRAM_DEVICE;

  SDRAM_Timing.LoadToActiveDelay    = 2;
  SDRAM_Timing.ExitSelfRefreshDelay = 6;
  SDRAM_Timing.SelfRefreshTime      = 4;
  SDRAM_Timing.RowCycleDelay        = 6;
  SDRAM_Timing.WriteRecoveryTime    = 2;
  SDRAM_Timing.RPDelay              = 2;
  SDRAM_Timing.RCDDelay             = 2;

  hsdram.Init.SDBank             = FMC_SDRAM_BANK1;
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth    = SDRAM_MEMORY_WIDTH;
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = SDCLOCK_PERIOD;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

  /* Initialize the SDRAM controller */
  if(HAL_SDRAM_Init(&hsdram, &SDRAM_Timing) != HAL_OK)
  {
    /* Initialization Error */
    halt("sdram init");
  }

  /* Program the SDRAM external device */
  sdram_init_seq(&hsdram, &command);
}

static void init_display(void) {
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Configure LCD : Only one layer is used */
  LCD_Config();
}

static void init_led(void) {
  GPIO_InitTypeDef gpio_init_structure;
  // LED
  __HAL_RCC_GPIOI_CLK_ENABLE();

  HAL_GPIO_DeInit(GPIOI, GPIO_PIN_1);

  gpio_init_structure.Pin = GPIO_PIN_1;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  //gpio_init_structure.Alternate = 0; // nevermind
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);
}

static void init_uart(void) {
  GPIO_InitTypeDef gpio_init_structure;

  // UART
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_USART6_CLK_ENABLE();

  gpio_init_structure.Pin = GPIO_PIN_6; // tx
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  gpio_init_structure.Pin = GPIO_PIN_7; // rx
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOC, &gpio_init_structure);

  UART_init();
  UART_assure_tx(_UART(IOSTD), TRUE);
  UART_set_callback(_UART(IOSTD), uart_cb, NULL);

  NVIC_EnableIRQ(USART6_IRQn);
}

int main(void) {

  init_processor();

  init_led();

  init_uart();

  print_hello();

  init_sdram();

  init_display();

  //sdram_test();

  print("\n\napp start\n\n");
  app_start();

  print("\nstartup finished\n");

  while(1) {
    __WFI();
  }

  return 0;
}

static void uart_cb(void *arg, u8_t c) {
  print("got:%02x\n", c);
}

static void sdram_test(void) {
  const u32_t M = 0xa72e1069;
  u32_t addr = 0xc0000000;
  u8_t *pb = (u8_t *)addr;
  u16_t *ps = (u16_t *)addr;
  u32_t *pw = (u32_t *)addr;
  const u32_t size = 1024*1024*4;
  u32_t i;

  print("testing sdram @ %08x, %i bytes\n", addr, size);
  print("8 bit\n");
  i = 0;
  while (i < size) {
    pb[i] = (M*i);
    i++;
  }
  i = 0;
  while (i < size) {
    if (pb[i] != (u8_t)(M*i)) {
      halt("8 bit @ %08x %02x!=%02x",  &pb[i], pb[i], (u8_t)(M*i));
    }
    i++;
  }
  print("16 bit\n");
  i = 0;
  while (i < size/2) {
    ps[i] = (M*i);
    i++;
  }
  i = 0;
  while (i < size) {
    if (ps[i] != (u16_t)(M*i)) {
      halt("16 bit @ %08x %04x!=%04x",  &ps[i], ps[i], (u16_t)(M*i));
    }
    i++;
  }
  print("32 bit\n");
  i = 0;
  while (i < size/4) {
    pw[i] = (M*i);
    i++;
  }
  i = 0;
  while (i < size/4) {
    if (pw[i] != (u32_t)(M*i)) {
      halt("32 bit @ %08x %08x!=%08x",  &pw[i], pw[i], (u32_t)(M*i));
    }
    i++;
  }
  print("OK\n");
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
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
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    halt("RCC osc config");
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    halt("overdrive");
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    halt("clockconfig");
  }
}

#if 0
/* Override */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
  // nop
  return HAL_OK;
}

/* Override */
void HAL_Delay(__IO uint32_t Delay) {
  volatile u64_t q = HAL_RCC_GetHCLKFreq() / 1000;
  q *= Delay;
  q /= 2;
  while(q--);
}
#endif

/**
  * @brief LCD Configuration.
  * @note  This function Configure tha LTDC peripheral :
  *        1) Configure the Pixel Clock for the LCD
  *        2) Configure the LTDC Timing and Polarity
  *        3) Configure the LTDC Layer 1 :
  *           - The frame buffer is located at FLASH memory
  *           - The Layer size configuration : 480x272
  * @retval
  *  None
  */
static void LCD_Config(void)
{
  static LTDC_HandleTypeDef hltdc_F;
  LTDC_LayerCfgTypeDef      pLayerCfg;

  /* LTDC Initialization -------------------------------------------------------*/

  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  hltdc_F.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */
  hltdc_F.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  /* Initialize the data enable polarity as active low */
  hltdc_F.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  /* Initialize the pixel clock polarity as input pixel clock */
  hltdc_F.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

  /* The RK043FN48H LCD 480x272 is selected */
  /* Timing Configuration */
  hltdc_F.Init.HorizontalSync = (RK043FN48H_HSYNC - 1);
  hltdc_F.Init.VerticalSync = (RK043FN48H_VSYNC - 1);
  hltdc_F.Init.AccumulatedHBP = (RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
  hltdc_F.Init.AccumulatedVBP = (RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
  hltdc_F.Init.AccumulatedActiveH = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
  hltdc_F.Init.AccumulatedActiveW = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
  hltdc_F.Init.TotalHeigh = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP + RK043FN48H_VFP - 1);
  hltdc_F.Init.TotalWidth = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP + RK043FN48H_HFP - 1);

  /* Configure R,G,B component values for LCD background color : all black background */
  hltdc_F.Init.Backcolor.Blue = 0;
  hltdc_F.Init.Backcolor.Green = 0;
  hltdc_F.Init.Backcolor.Red = 0;

  hltdc_F.Instance = LTDC;

/* Layer1 Configuration ------------------------------------------------------*/

  /* Windowing configuration */
  /* In this case all the active display area is used to display a picture then :
     Horizontal start = horizontal synchronization + Horizontal back porch = 43
     Vertical start   = vertical synchronization + vertical back porch     = 12
     Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1
     Vertical stop   = Vertical start + window height -1  = 12 + 272 -1      */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = LCD_WW;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = LCD_WH;

  /* Pixel Format configuration*/
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;

  /* Start Address configuration : frame buffer is located at FLASH memory */
  pLayerCfg.FBStartAdress = (uint32_t)FBUF_ADDR;

  /* Alpha constant (255 == totally opaque) */
  pLayerCfg.Alpha = 255;

  /* Default Color configuration (configure A,R,G,B component values) : no background color */
  pLayerCfg.Alpha0 = 0; /* fully transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;

  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = LCD_W;
  pLayerCfg.ImageHeight = LCD_H;

  /* Configure the LTDC */
  if(HAL_LTDC_Init(&hltdc_F) != HAL_OK)
  {
    /* Initialization Error */
    halt("ltdc init");
  }

  /* Configure the Layer*/
  if(HAL_LTDC_ConfigLayer(&hltdc_F, &pLayerCfg, 1) != HAL_OK)
  {
    /* Initialization Error */
    halt("ltdc config layer");
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  Perform the SDRAM exernal memory inialization sequence
  * @param  hsdram: SDRAM handle
  * @param  Command: Pointer to SDRAM command structure
  * @retval None
  */
static void sdram_init_seq(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd =0;
  /* Step 3:  Configure a clock configuration enable command */
  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 4: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 5: Configure a PALL (precharge all) command */
  Command->CommandMode = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 6 : Configure a Auto-Refresh command */
  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 8;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 7: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_2           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 8: Set the refresh rate counter */
  /* (15.62 us x Freq) - 20 */
  /* Set the device refresh counter */
  hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));

}


void SysTick_Handler(void) { HAL_IncTick(); }

#if 0
static volatile uint32_t last_irq = 0;

static void def_irq(uint32_t irq) {
  last_irq = irq;
  while(1);
}

void HardFault_Handler(void) { def_irq(-999); }
void UsageFault_Handler(void) { def_irq(UsageFault_IRQn); }
void WWDG_IRQHandler(void) { def_irq(WWDG_IRQn); }
void PVD_IRQHandler(void) { def_irq(PVD_IRQn); }
void TAMP_STAMP_IRQHandler(void) { def_irq(TAMP_STAMP_IRQn); }
void RTC_WKUP_IRQHandler(void) { def_irq(RTC_WKUP_IRQn); }
void FLASH_IRQHandler(void) { def_irq(FLASH_IRQn); }
void RCC_IRQHandler(void) { def_irq(RCC_IRQn); }
void EXTI0_IRQHandler(void) { def_irq(EXTI0_IRQn); }
void EXTI1_IRQHandler(void) { def_irq(EXTI1_IRQn); }
void EXTI2_IRQHandler(void) { def_irq(EXTI2_IRQn); }
void EXTI3_IRQHandler(void) { def_irq(EXTI3_IRQn); }
void EXTI4_IRQHandler(void) { def_irq(EXTI4_IRQn); }
void DMA1_Stream0_IRQHandler(void) { def_irq(DMA1_Stream0_IRQn); }
void DMA1_Stream1_IRQHandler(void) { def_irq(DMA1_Stream1_IRQn); }
void DMA1_Stream2_IRQHandler(void) { def_irq(DMA1_Stream2_IRQn); }
void DMA1_Stream3_IRQHandler(void) { def_irq(DMA1_Stream3_IRQn); }
void DMA1_Stream4_IRQHandler(void) { def_irq(DMA1_Stream4_IRQn); }
void DMA1_Stream5_IRQHandler(void) { def_irq(DMA1_Stream5_IRQn); }
void DMA1_Stream6_IRQHandler(void) { def_irq(DMA1_Stream6_IRQn); }
void ADC_IRQHandler(void) { def_irq(ADC_IRQn); }
void CAN1_TX_IRQHandler(void) { def_irq(CAN1_TX_IRQn); }
void CAN1_RX0_IRQHandler(void) { def_irq(CAN1_RX0_IRQn); }
void CAN1_RX1_IRQHandler(void) { def_irq(CAN1_RX1_IRQn); }
void CAN1_SCE_IRQHandler(void) { def_irq(CAN1_SCE_IRQn); }
void EXTI9_5_IRQHandler(void) { def_irq(EXTI9_5_IRQn); }
void TIM1_BRK_TIM9_IRQHandler(void) { def_irq(TIM1_BRK_TIM9_IRQn); }
void TIM1_UP_TIM10_IRQHandler(void) { def_irq(TIM1_UP_TIM10_IRQn); }
void TIM1_TRG_COM_TIM11_IRQHandler(void) { def_irq(TIM1_TRG_COM_TIM11_IRQn); }
void TIM1_CC_IRQHandler(void) { def_irq(TIM1_CC_IRQn); }
void TIM2_IRQHandler(void) { def_irq(TIM2_IRQn); }
void TIM3_IRQHandler(void) { def_irq(TIM3_IRQn); }
void TIM4_IRQHandler(void) { def_irq(TIM4_IRQn); }
void I2C1_EV_IRQHandler(void) { def_irq(I2C1_EV_IRQn); }
void I2C1_ER_IRQHandler(void) { def_irq(I2C1_ER_IRQn); }
void I2C2_EV_IRQHandler(void) { def_irq(I2C2_EV_IRQn); }
void I2C2_ER_IRQHandler(void) { def_irq(I2C2_ER_IRQn); }
void SPI1_IRQHandler(void) { def_irq(SPI1_IRQn); }
void SPI2_IRQHandler(void) { def_irq(SPI2_IRQn); }
void USART1_IRQHandler(void) { def_irq(USART1_IRQn); }
void USART2_IRQHandler(void) { def_irq(USART2_IRQn); }
void USART3_IRQHandler(void) { def_irq(USART3_IRQn); }
//void USART6_IRQHandler(void) { def_irq(USART6_IRQn); }
void EXTI15_10_IRQHandler(void) { def_irq(EXTI15_10_IRQn); }
void RTC_Alarm_IRQHandler(void) { def_irq(RTC_Alarm_IRQn); }
void OTG_FS_WKUP_IRQHandler(void) { def_irq(OTG_FS_WKUP_IRQn); }
void TIM8_BRK_TIM12_IRQHandler(void) { def_irq(TIM8_BRK_TIM12_IRQn); }
void TIM8_UP_TIM13_IRQHandler(void) { def_irq(TIM8_UP_TIM13_IRQn); }
void TIM8_TRG_COM_TIM14_IRQHandler(void) { def_irq(TIM8_TRG_COM_TIM14_IRQn); }
void TIM8_CC_IRQHandler(void) { def_irq(TIM8_CC_IRQn); }
void DMA1_Stream7_IRQHandler(void) { def_irq(DMA1_Stream7_IRQn); }
void FMC_IRQHandler(void) { def_irq(FMC_IRQn); }
void SDMMC1_IRQHandler(void) { def_irq(SDMMC1_IRQn); }
void TIM5_IRQHandler(void) { def_irq(TIM5_IRQn); }
void SPI3_IRQHandler(void) { def_irq(SPI3_IRQn); }
void UART4_IRQHandler(void) { def_irq(UART4_IRQn); }
void UART5_IRQHandler(void) { def_irq(UART5_IRQn); }
void TIM6_DAC_IRQHandler(void) { def_irq(TIM6_DAC_IRQn); }
void TIM7_IRQHandler(void) { def_irq(TIM7_IRQn); }
void DMA2_Stream0_IRQHandler(void) { def_irq(DMA2_Stream0_IRQn); }
void DMA2_Stream1_IRQHandler(void) { def_irq(DMA2_Stream1_IRQn); }
void DMA2_Stream2_IRQHandler(void) { def_irq(DMA2_Stream2_IRQn); }
void DMA2_Stream3_IRQHandler(void) { def_irq(DMA2_Stream3_IRQn); }
void DMA2_Stream4_IRQHandler(void) { def_irq(DMA2_Stream4_IRQn); }
void ETH_IRQHandler(void) { def_irq(ETH_IRQn); }
void ETH_WKUP_IRQHandler(void) { def_irq(ETH_WKUP_IRQn); }
void CAN2_TX_IRQHandler(void) { def_irq(CAN2_TX_IRQn); }
void CAN2_RX0_IRQHandler(void) { def_irq(CAN2_RX0_IRQn); }
void CAN2_RX1_IRQHandler(void) { def_irq(CAN2_RX1_IRQn); }
void CAN2_SCE_IRQHandler(void) { def_irq(CAN2_SCE_IRQn); }
void OTG_FS_IRQHandler(void) { def_irq(OTG_FS_IRQn); }
void DMA2_Stream5_IRQHandler(void) { def_irq(DMA2_Stream5_IRQn); }
void DMA2_Stream6_IRQHandler(void) { def_irq(DMA2_Stream6_IRQn); }
void DMA2_Stream7_IRQHandler(void) { def_irq(DMA2_Stream7_IRQn); }
void I2C3_EV_IRQHandler(void) { def_irq(I2C3_EV_IRQn); }
void I2C3_ER_IRQHandler(void) { def_irq(I2C3_ER_IRQn); }
void OTG_HS_EP1_OUT_IRQHandler(void) { def_irq(OTG_HS_EP1_OUT_IRQn); }
void OTG_HS_EP1_IN_IRQHandler(void) { def_irq(OTG_HS_EP1_IN_IRQn); }
void OTG_HS_WKUP_IRQHandler(void) { def_irq(OTG_HS_WKUP_IRQn); }
void OTG_HS_IRQHandler(void) { def_irq(OTG_HS_IRQn); }
void DCMI_IRQHandler(void) { def_irq(DCMI_IRQn); }
void RNG_IRQHandler(void) { def_irq(RNG_IRQn); }
void FPU_IRQHandler(void) { def_irq(FPU_IRQn); }
void UART7_IRQHandler(void) { def_irq(UART7_IRQn); }
void UART8_IRQHandler(void) { def_irq(UART8_IRQn); }
void SPI4_IRQHandler(void) { def_irq(SPI4_IRQn); }
void SPI5_IRQHandler(void) { def_irq(SPI5_IRQn); }
void SPI6_IRQHandler(void) { def_irq(SPI6_IRQn); }
void SAI1_IRQHandler(void) { def_irq(SAI1_IRQn); }
void LTDC_IRQHandler(void) { def_irq(LTDC_IRQn); }
void LTDC_ER_IRQHandler(void) { def_irq(LTDC_ER_IRQn); }
void DMA2D_IRQHandler(void) { def_irq(DMA2D_IRQn); }
void SAI2_IRQHandler(void) { def_irq(SAI2_IRQn); }
void QUADSPI_IRQHandler(void) { def_irq(QUADSPI_IRQn); }
void LPTIM1_IRQHandler(void) { def_irq(LPTIM1_IRQn); }
void CEC_IRQHandler(void) { def_irq(CEC_IRQn); }
void I2C4_EV_IRQHandler(void) { def_irq(I2C4_EV_IRQn); }
void I2C4_ER_IRQHandler(void) { def_irq(I2C4_ER_IRQn); }
void SPDIF_RX_IRQHandler(void) { def_irq(SPDIF_RX_IRQn); }
#endif

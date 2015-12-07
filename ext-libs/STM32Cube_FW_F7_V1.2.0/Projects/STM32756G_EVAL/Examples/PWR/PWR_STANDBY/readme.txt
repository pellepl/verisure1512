/**
  @page PWR_STANDBY PWR standby example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    PWR/PWR_STANDBY/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the PWR STANDBY example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 
 
This example shows how to enters the system to STANDBY mode and wake-up from this
mode using: external RESET, RTC Alarm A or WKUP pin.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 216 MHz.

When a falling edge is detected on the EXTI line an interrupt is generated. In the 
EXTI handler routine the RTC is configured to generate an Alarm event in 5 seconds
then the system enters STANDBY mode causing the LED1 to stop toggling. 
A rising edge on WKUP pin or an external RESET will wake-up the system from
STANDBY. If within 5 seconds neither rising edge on WKUP pin nor external RESET
are generated, the RTC Alarm A will wake-up the system. 

After wake-up from STANDBY mode, program execution restarts in the same way as after
a RESET, the RTC configuration (clock source, prescaler,...) is kept and LED1 restarts
toggling. As result there is no need to configure the RTC.

two leds LED1 and LED3 are used to monitor the system state as following:
 - LED3 On: configuration failed (system will go to an infinite loop)
 - LED1 On: system in RUN mode
 - LED1 Off : system in STANDBY mode

These Steps are repeated in an infinite loop.

@note To measure the current consumption in STANDBY mode, please refer to 
      @subpage PWR_CurrentConsumption example.

@note This example can not be used in DEBUG mode, this is due to the fact 
      that the Cortex-M7 core is no longer clocked during low power mode 
      so debugging features are disabled.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
@note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
       case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
       registers (including the backup registers) and RCC_BDCR register are set to their reset values.

@par Directory contents 

  - PWR/PWR_STANDBY/Inc/stm32f7xx_hal_conf.h         HAL Configuration file
  - PWR/PWR_STANDBY/Inc/stm32f7xx_it.h           Header for stm32f7xx_it.c
  - PWR/PWR_STANDBY/Inc/main.h                   Header file for main.c
  - PWR/PWR_STANDBY/Src/system_stm32f7xx.c       STM32F7xx system clock configuration file
  - PWR/PWR_STANDBY/Src/stm32f7xx_it.c           Interrupt handlers
  - PWR/PWR_STANDBY/Src/main.c                   Main program
  - PWR/PWR_STANDBY/Src/stm32f7xx_hal_msp.c      HAL MSP module

@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx devices
    
      
  - This example has been tested with STMicroelectronics STM327x6G-EVAL revB
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM327x6G-EVAL revB Set-up
    - Use LED1 and LED3 connected respectively to N/A and N/A pins
    - Use the Tamper push-button connected to pin PC.13 (EXTI15_10)
    - WakeUp Pin PWR_WAKEUP_PIN4 connected to PC13

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

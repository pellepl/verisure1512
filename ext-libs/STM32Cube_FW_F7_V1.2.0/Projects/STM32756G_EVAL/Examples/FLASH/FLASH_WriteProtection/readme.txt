/**
  @page FLASH_WriteProtection FLASH write protection
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FLASH/FLASH_WriteProtection/readme.txt
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the FLASH write protection example.
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

This example guides you through the different configuration steps by mean of 
HAL API how to enable and disable the write protection for internal FLASH memory 
integrated within STM32F7xx devices, mounted on STM327x6G-EVAL board revB.
  
At the beginning of the main program the HAL_Init() function is called to 
reset all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system clock (SYSCLK) 
to run at 200 MHz.

Each time the User Tamper push-button is pressed, the program will check the 
write protection status of FLASH_WRP_SECTORS (defined in main.c) 
  - If FLASH_WRP_SECTORS are write protected, the write protection will be 
    disabled.
    Then the following message will be displayed on LCD, if protection disable 
    operation is done correctly: 
      "Write protection is disabled"
    Otherwise the following message will be displayed on LCD:
      "Write protection is not disabled"
  - If FLASH_WRP_SECTORS are not write protected, the write protection will 
    be enabled.
    Then the following message will be displayed on LCD, if protection enable 
    operation is done correctly:
      "Write protection is enabled"
    Otherwise the following message will be displayed on LCD:
      "Write protection is not enabled"

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds) 
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from 
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower) 
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set 
      to 1 millisecond to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Directory contents 

  - FLASH/FLASH_WriteProtection/Inc/stm32f7xx_hal_conf.h        HAL Configuration file  
  - FLASH/FLASH_WriteProtection/Inc/stm32f7xx_it.h              Header for stm32f7xx_it.c
  - FLASH/FLASH_WriteProtection/Inc/main.h                      Header for main.c module 
  - FLASH/FLASH_WriteProtection/Src/stm32f7xx_it.c              Interrupt handlers
  - FLASH/FLASH_WriteProtection/Src/main.c                      Main program
  - FLASH/FLASH_WriteProtection/Src/system_stm32f7xx.c          STM32F7xx system clock configuration file

@par Hardware and Software environment 
  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

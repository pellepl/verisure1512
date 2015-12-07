/**
  @page QSPI_ExecuteInPlace QSPI execute in place example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    QSPI/QSPI_ExecuteInPlace/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the code execution from QSPI memory example.
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

This example describes how to configure and use QPSI through the STM32F7xx HAL API. 

This example shows the execution of a part of the code from the QSPI memory. To do this, 
a section is created where the function is stored.

During startup, the QSPI memory is erased. Data are then copied from the initialization
section of the flash to the QSPI memory. Finally, the QSPI is configured in memory-mapped 
mode and the code is executed in a forever loop.

LED1 is toggled in a forever loop.
As soon as an error is returned by HAL API, LED1 is turned on.

In this example, HCLK is configured at 216 MHz.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on a variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise, the caller ISR process will be blocked.
      To change the SysTick interrupt priority, you have to use the HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond,
      to have correct HAL operation.

@par Directory contents 

  - QSPI/QSPI_ExecuteInPlace/Inc/stm32f7xx_hal_conf.h HAL configuration file
  - QSPI/QSPI_ExecuteInPlace/Inc/stm32f7xx_it.h       Interrupt handlers header file
  - QSPI/QSPI_ExecuteInPlace/Inc/main.h               Header for main.c module  
  - QSPI/QSPI_ExecuteInPlace/Src/stm32f7xx_it.c       Interrupt handlers
  - QSPI/QSPI_ExecuteInPlace/Src/main.c               Main program
  - QSPI/QSPI_ExecuteInPlace/Src/system_stm32f7xx.c   STM32F7xx system source file
  - QSPI/QSPI_ExecuteInPlace/Src/stm32f7xx_hal_msp.c  HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F7xx devices.
    
  - This example has been tested on STM32746G-DISCOVERY board and can be
    easily tailored to any other supported device and/or development board.

  - STM32746G-DISCOVERY Set-up :
    - Board is configured by default to access QSPI memory
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

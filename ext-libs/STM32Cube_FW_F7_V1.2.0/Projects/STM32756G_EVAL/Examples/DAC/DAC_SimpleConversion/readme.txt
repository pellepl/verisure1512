/**
  @page DAC_SimpleConversion DAC Simple Conversion example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    DAC/DAC_SimpleConversion/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the DAC Simple Conversion example.
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

This example provides a short description of how to use the DAC peripheral to perform a
simple conversion of the 0xFF value in 8-bit right alignment mode. The result of the
conversion can be obtained by connecting PA4 (DAC channel1) to an oscilloscope.

STM327x6G-EVAL board revB's LEDs can be used to monitor the process status:
  - LED3 is ON and example is stopped (using infinite loop)
  when there is an error during process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - DAC/DAC_Simple_Conversion/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - DAC/DAC_Simple_Conversion/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - DAC/DAC_Simple_Conversion/Inc/main.h                  Header for main.c module  
  - DAC/DAC_Simple_Conversion/Src/stm32f7xx_it.c          DMA interrupt handlers
  - DAC/DAC_Simple_Conversion/Src/main.c                  Main program
  - DAC/DAC_Simple_Conversion/Src/stm32f7xx_hal_msp.c     HAL MSP file
  - DAC/DAC_Simple_Conversion/Src/system_stm32f7xx.c      STM32F7xx system source file
  

@par Hardware and Software environment  
  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.

  - STM327x6G-EVAL Set-up 	
     - Connect PA4 (DAC Channel1), pin 21 in CN5,  to an oscilloscope.

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */


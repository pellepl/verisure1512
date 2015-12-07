/**
  @page ADC_InjectedConversion_Interrupt conversion using interrupt

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    ADC/ADC_InjectedConversion_Interrupt/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the ADC RegularConversion interrupt example.
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

This example describes how to interrupt continuous ADC3 regular ADC_CHANNEL_8 conversion 
using ADC3 injected ADC_CHANNEL_12 and how to get the converted value of this conversion.

The ADC3 is configured to convert continuously regular ADC_CHANNEL_8
and convert injected ADC_CHANNEL_12 (connected to PC.02) when injected conversion starts
(after one second of the start of regular channel).

Each time a regular channel end of conversion occurs an interrupt is generated 
and the converted data of ADC3 DR register is affected to the uhADCxConvertedRegValue 
variable in the ADC conversion complete callback function. After one second of
the start of regular ADC_CHANNEL_8 conversion the injected ADC_CHANNEL_12 conversion starts 
so regular conversion is temporary interrupted until the end of injected conversion. 
When the injected conversion is finished, an interrupt is generated, and the  
converted injected data of ADC3 DR register is affected to the uhADCxConvertedInjValue  
variable in the ADC conversion complete callback function.

In this example, the system clock is 216MHz, APB2 = 108MHz and ADC clock = APB2/4. 
Since ADC3 clock is 27 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/27= 0.55us (1.8Msps).

User can vary the ADC_CHANNEL_8 voltage using the Eval Board potentiometer (RV1) connected to PF.10.
The converted values could be monitered through debugger:
  - uhADCxConvertedRegValue for regular conversion
  - uhADCxConvertedInjValue for injected conversion

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED3 is ON when there are an error in initialization.

@par Directory contents 

  - ADC/ADC_InjectedConversion_Interrupt/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_InjectedConversion_Interrupt/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_InjectedConversion_Interrupt/Inc/main.h                  Header for main.c module  
  - ADC/ADC_InjectedConversion_Interrupt/Src/stm32f7xx_it.c          DMA interrupt handlers
  - ADC/ADC_InjectedConversion_Interrupt/Src/main.c                  Main program
  - ADC/ADC_InjectedConversion_Interrupt/Src/stm32f7xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_InjectedConversion_Interrupt/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.

  - STM327x6G-EVAL revB Set-up
    - Use the Potentiometer (RV1) of the Eval board (connected to  PF.10).
    - Connect PC.02 to a power supply (do not forget to connect the power supply 
      GND to the EVAL board GND)
    - JP24 must be fitted in 1-2 positions
      
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

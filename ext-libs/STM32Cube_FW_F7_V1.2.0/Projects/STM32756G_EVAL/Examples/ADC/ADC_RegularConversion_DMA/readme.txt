/**
  @page ADC_RegularConversion_DMA conversion using DMA for Data transfer

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_DMA/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the ADC RegularConversion DMA example.
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

This example describes how to use the ADC3 and DMA to transfer continuously 
converted data from ADC3 to memory.

The ADC3 is configured to convert continuously ADC_CHANNEL_8.

Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC3 DR register to the uhADCxConvertedValue variable.

In this example, the system clock is 216MHz, APB2 = 108MHz and ADC clock = APB2/4. 
Since ADC3 clock is 27 MHz and sampling time is set to 3 cycles, the conversion 
time to 12bit data is 12 cycles so the total conversion time is (12+3)/27= 0.55us (1.8Msps).

User can vary the ADC_CHANNEL_8 voltage using the Eval Board potentiometer (RV1) connected to PF.10.

The uhADCxConvertedValue read value is coded on 12 bits, the Vref+ reference voltage is connected 
on the board to VDD (+3.3V), the Vref- reference voltage is connected on the board to the ground.
To convert the read value in volts, here is the equation to apply :
Voltage = uhADCxConvertedValue * (Vref+ - Vref-) / (2^12) = uhADCxConvertedValue * 3.3 / 4096

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED3 is ON when the conversion is complete.
  - LED3 is toggling when there are an error in initialization.

@par Directory contents 

  - ADC/ADC_RegularConversion_DMA/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_DMA/Inc/stm32f7xx_it.h          DMA interrupt handlers header file
  - ADC/ADC_RegularConversion_DMA/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_DMA/Src/stm32f7xx_it.c          DMA interrupt handlers
  - ADC/ADC_RegularConversion_DMA/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_DMA/Src/stm32f7xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_DMA/Src/system_stm32f7xx.c      STM32F7xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.

  - STM327x6G-EVAL revB Set-up   
        - To use LED3, ensure that JP23 is in position 2-3.
        - Use the Potentiometer (RV1) of the Eval board (connected to  PF.10).
        - JP24 must be fitted in 1-2 positions

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

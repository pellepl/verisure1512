/**
  @page LTDC_ColorKeying LTDC Color Keying example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    LTDC/LTDC_ColorKeying/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the LTDC Color Keying example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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

  This example describe how to enable and use the LTDC color keying functionality.

  At the beginning of the main program the HAL_Init() function is called to reset 
  all the peripherals, initialize the Flash interface and the systick.
  Then the SystemClock_Config() function is used to configure the system
  clock (SYSCLK) to run at 216 MHz.
 
 After LCD initialization, LCD layer 1 and color keying configuration, an image 
 of size 480x272 in ARGB4444 format featuring ST logo on light blue background (color key ARGB4444==0xFFFF)
 is displayed on LCD.
 Color keying is enabled/disabled on the image when the tamper button is pressed. 
 The effect is that at each button press the light blue background is made transparent or not thanks
 to color keying. At time when the color key is enabled, the background of the logo is the background color
 configured for layer 1, which is all zeroes (black color).
 Note that user button press is scanned in polling in the main.c, no use of EXTI in this case.
      
 Color Keying :
 --------------
 If the Color Keying is enabled, all pixels of the image are compared to that color key pixel value
 (0xFFFF), pratically only the background of the logo correspond to that color key in the image. 
 For the pixels that match the color key RGB value, all channels (ARGB) of those pixels 
 are set to 0 (those pixels are then becoming transparent and does not appear on LCD screen, the background
 of image : black is seen instead).

STM32 Eval board's LED can be used to monitor the transfer status:
 - LED3 is ON when there is an error in Init process.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

    - LTDC/LTDC_ColorKeying/Inc/stm32f7xx_hal_conf.h    HAL configuration file
    - LTDC/LTDC_ColorKeying/Inc/stm32f7xx_it.h          Interrupt handlers header file
    - LTDC/LTDC_ColorKeying/Inc/main.h                  Main configuration file
    - LTDC/LTDC_ColorKeying/Src/stm32f7xx_it.c          Interrupt handlers
    - LTDC/LTDC_ColorKeying/Src/main.c                  Main program 
    - LTDC/LTDC_ColorKeying/Src/stm32f7xx_hal_msp.c     HAL MSP module	
    - LTDC/LTDC_ColorKeying/Src/system_stm32f7xx.c      STM32F7xx system clock configuration file
    - LTDC/LTDC_ColorKeying/Inc/ARGB4444_480x272.h      Image (single layer 1) of size 480x272 and format ARGB4444 to be displayed on LCD

@par Hardware and Software environment  

  - This example runs on STM32F756xx/STM32F746xx devices.
  
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.    

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
                           
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
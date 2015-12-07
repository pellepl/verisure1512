/**
  @page STemWin_SampleDemo Readme file
 
  @verbatim
  ******************************************************************************
  * @file    STemWin/STemWin_SampleDemo/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    21-September-2015
  * @brief   Description of STemWin Sample Demo application.  
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

@par Application Description

This directory contains a set of source files that implement a sample
demonstration application allowing to show some of the STemWin Library capabilities 
on STM32F7x6 devices.

The list of modules to be used is configured into the file Demo/GUIDEMO.h

At the first run, after power OFF/ON, a touch screen calibration is needed: in
order to this, user is just requested to follow the displayed instructions.
Then all the demo modules are run sequentially. Throughout the demonstration, a
control frame window is displayed at the bottom right of the touch screen: it
includes a progress bar and two control buttons (Next + Halt) allowing to skip
or pause the currently running module.

LEDs 1 to 4 are continuously toggling to indicate that the Demo runs properly.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select the RTC clock source; in this 
      case the Backup domain will be reset in order to modify the RTC Clock source, as consequence RTC  
      registers (including the backup registers) and RCC_BDCR register are set to their reset values.
	   
@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this application uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.

     
@par Directory contents 

  - STemWin/SampleDemo/Inc/GUIConf.h                        Header for GUIConf.c
  - STemWin/SampleDemo/Inc/LCDConf.h                        Header for LCDConf*.c
  - STemWin/SampleDemo/Inc/rtc.h                            Header for rtc.c
  - STemWin/SampleDemo/Inc/calibration.h                    Header for calibration.c
  - STemWin/SampleDemo/Inc/main.h                           Main program header file
  - STemWin/SampleDemo/Inc/stm32f7xx_hal_conf.h             Library Configuration file
  - STemWin/SampleDemo/Inc/stm32f7xx_it.h                   Interrupt handlers header file
  - STemWin/SampleDemo/Src/rtc.c                            Main file to configure RTC clock
  - STemWin/SampleDemo/Src/calibration.c                    Main file to calibrate TS
  - STemWin/SampleDemo/Src/GUIConf.c                        Display controller initialization file
  - STemWin/SampleDemo/Src/LCDConf_STM327x6G_EVAL_MB1063.c  Configuration file for the GUI library (MB1063 LCD)
  - STemWin/SampleDemo/Src/main.c                           Main program file
  - STemWin/SampleDemo/Src/stm32f7xx_it.c                   STM32F7xx Interrupt handlers
  - STemWin/SampleDemo/Src/system_stm32f7xx.c               STM32F7xx system file
  - STemWin/SampleDemo/Demo/GUIDEMO.c                       All the sources files provided for SEGGER DEMO
  - STemWin/SampleDemo/Demo/GUIDEMO.h                       DEMO modules configuration file


@par Hardware and Software environment 

  - This application runs on STM32F756xx/STM32F746xx devices.
    
  - This application has been tested with STMicroelectronics STM327x6G_EVAL
    (STM32F7x6 Devices) evaluation boards and can be easily 
    tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
  - Open your preferred toolchain 
  - Rebuild all files and load your image into target memory
  - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

/**
  @page FreeRTOS_DelayUntil FreeRTOS Delay Until example
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_DelayUntil/readme.txt
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the FreeRTOS Delay Until example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

@par Description

This directory contains a set of source files that implement thread delaying using 
osDelayUntil function.

This example creates a thread that waits for 200ms starting from a reference absolute time "PreviousWakeTime"
using osDelayUntil then toggle LED1.

the result is that the thread execution frequency is ensured to be every 200ms 
independently from the time that the thread treatment takes

osDelayUntil function differs from osDelay() in one important aspect:  osDelay () will
cause a thread to block for the specified time in ms from the time osDelay () is
called.  It is therefore difficult to use osDelay () by itself to generate a fixed
execution frequency as the time between a thread starting to execute and that thread
calling osDelay () may not be fixed [the thread may take a different path though the
code between calls, or may get interrupted or preempted a different number of times
each time it executes].

Whereas osDelay () specifies a wake time relative to the time at which the function
is called, osDelayUntil () specifies the absolute (exact) time at which it wishes to
unblock.
PreviousWakeTime must be initialised with the current time prior to its first use 
(PreviousWakeTime = osKernelSysTick ) 

LED1 must blink every 200ms   


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to 
      the OS resources memory requirements of the application with +10% margin and rounded to the 
	  upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Directory contents

    - FreeRTOS/FreeRTOS_DelayUntil/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/stm32f7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_DelayUntil/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_DelayUntil/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_DelayUntil/Src/stm32f7xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F746xx devices.
    
  - This example has been tested with STM32746G_DISCOVERY evaluation board and can be
    easily tailored to any other supported device and development board.

  - STM32746G_DISCOVERY Set-up
    - None

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

/**
  @page FreeRTOS_SemaphoreFromISR FreeRTOS semaphore from ISR application
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_SemaphoreFromISR/readme.txt
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the FreeRTOS semaphore from ISR application.
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

This directory contains a set of source files that implement an application that 
uses semaphore from ISR with CMSIS RTOS API 

This application creates a thread that toggles LED through semaphore given from ISR.
 
Each time the user pushes the USER button of the Discovery board the semaphore 
is given to the SemaphoreTest Thread to toggle the LED1  


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to 
      the OS resources memory requirements of the application with +10% margin and rounded to the 
	  upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@par Directory contents

    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/main.h                 Main program header file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/stm32f7xx_hal_conf.h   HAL Library Configuration file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/stm32f7xx_it.h         Interrupt handlers header file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Inc/FreeRTOSConfig.h       FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Src/main.c                 Main program
    - FreeRTOS/FreeRTOS_SemaphoreFromISR/Src/stm32f7xx_it.c         Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F746xx devices.
    
  - This example has been tested with STM32746G_DISCOVERY boards RevB and can be
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

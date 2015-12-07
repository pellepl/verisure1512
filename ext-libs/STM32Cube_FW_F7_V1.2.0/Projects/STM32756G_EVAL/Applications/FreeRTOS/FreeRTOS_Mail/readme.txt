/**
  @page FreeRTOS_Mail FreeRTOS Mail example
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Mail/readme.txt
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    21-September-2015
  * @brief   Description of the FreeRTOS Mail example.
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

This directory contains a set of source files that implement an example that uses 
Mail queues with CMSIS RTOS API

This example creates two threads that send and receive mail the mail to send/receive
is a structure that holds three variables (var1 and var2 are uint32, var3 is a uint8)

One thread acts as a producer and the other as the consumer.  
The consumer has a higher priority than the producer and is set to block on mail receiving.

The Mail queue has space for one item. The producer allocates the mail and puts it in the mail queue. 
As soon as the producer posts a mail to the queue the consumer will unblock, preempt the producer, 
get the mail and free it.  

Add the following variables to LiveWatch, the three producer values must respectively remain 
equal to the three consumer values all the time:
 - ConsumerValue1 must remain equal to ProducerValue1
 - ConsumerValue2 must remain equal to ProducerValue2
 - ConsumerValue3 must remain equal to ProducerValue3
  
STM32 Eval board's LEDs can be used to monitor the example status:
  - LED1 should toggle when the example runs successfully.
  - LED3 is ON when any error occurs.  


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

    - FreeRTOS/FreeRTOS_Mail/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f7xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Mail/Inc/stm32f7xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Mail/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Mail/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Mail/Src/stm32f7xx_it.c        Interrupt handlers


@par Hardware and Software environment

  - This application runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G_EVAL evaluation board and can be
    easily tailored to any other supported device and development board.

  - STM327x6G_EVAL Set-up
    - Ensure that JP24 is in position 2-3 to use LED1.
    - Ensure that JP23 is in position 2-3 to use LED3. 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

/**
  @page CEC CEC_DataExchange example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    CEC/CEC_DataExchange/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the CEC Data Exchange example.
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

This example shows how to configure and use the CEC peripheral to receive and 
transmit messages.


- Hardware Description

To use this example, two STM32746G-DISCOVERY boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines

 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.

@verbatim
*------------------------------------------------------------------------------*
|       STM32746G_DISCOVERY RevB                STM32746G_DISCOVERY RevB       |
|         Device Address :0x01                    Device Address :0x03         |
|         _____________________                  _____________________         |
|        |                     |                |                     |        |
|        |                     |                |                     |        | 
|        |  __________         |                |         __________  |        |
|        | |   CEC    |________|____CECLine_____|________|   CEC    | |        |
|        | | Device_1 | PA15   |                | PA15   | Device_2 | |        |
|        | |__________| CN7.D9 |                | CN7.D9 |__________| |        |
|        |                     |                |                     |        |
|        |   _____________     |                |   _____________     |        |
|        |  | Screen      |    |                |  | Screen      |    |        |
|        |  |             |    |                |  |             |    |        |
|        |  |_____________|    |                |  |_____________|    |        |
|        |                     |                |                     |        |
|        |             GND O---|----------------|--O GND              |        |
|        |_____________________|                |_____________________|        |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------**
@endverbatim


- Software Description

The test unrolls as follows.

On TX side, four possible messages can be transmitted and are indicated on the screen :
 - when touching top of the screen, send CEC command with opcode 0x44 and operand 0x41
 - when touching bottom of the screen, send CEC command with opcode 0x44 and operand 0x42
 - when touching left of the screen, send CEC command with opcode 0x46
 - when touching right of the screen, send CEC command with opcode 0x9F

On the RX side in case of successful reception, it displays the content of the received 
CEC command or displays an error message if transmition fails.

The two boards transmits commands to other by touching the screen and receives the 
commands from the other and displays the received command on the screen.

In this example, HCLK is configured at 200 MHz.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Directory contents 

  - CEC/CEC_DataExchange/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CEC/CEC_DataExchange/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CEC/CEC_DataExchange/Inc/main.h                  Header for main.c module  
  - CEC/CEC_DataExchange/Src/stm32f7xx_it.c          Interrupt handlers
  - CEC/CEC_DataExchange/Src/system_stm32f7xx.c      STM32F7xx system source file
  - CEC/CEC_DataExchange/Src/main.c                  Main program
  - CEC/CEC_DataExchange/Src/stm32f7xx_hal_msp.c     IP hardware resources initialization
  
@par Hardware and Software environment

  - This example runs on STM32F756xx/STM32F746xx  Devices.
    
  - This example has been tested with STM32746G-DISCOVERY RevB board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - in main.h, uncomment DEVICE_1 for first board, uncomment DEVICE_2 for second board
 - Rebuild all files and load your image into target memory
 - With a wire, connect PA15 signal connected to CN7.D9 between the 2 boards
 - Connect the ground of the 2 boards
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

/**
  @page CRYP_AESModes  Encrypt and Decrypt data using AES Algo in ECB/CBC/CTR
  chaining modes with all possible key sizes.
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    CRYP/CRYP_AESModes/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the CRYP AES Algorithm in all modes and all key sizes
             Example
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

This example provides a short description of how to use the CRYPTO peripheral to
encrypt and decrypt data using AES in chaining modes (ECB, CBC, CTR) and all key
sizes (128, 192, 256) Algorithm.

For this example, a firmware is used to interface with the Crypto peripheral
in polling mode.

This example behaves as follows:

1. ECB mode:
   - AES128 ECB Encryption
   - AES192 ECB Encryption
   - AES256 ECB Encryption

   - AES128 ECB Decryption
   - AES192 ECB Decryption
   - AES256 ECB Decryption

2. CBC mode:
   - AES128 CBC Encryption
   - AES192 CBC Encryption
   - AES256 CBC Encryption

   - AES128 CBC Decryption
   - AES192 CBC Decryption
   - AES256 CBC Decryption

3. CTR mode:
   - AES128 CTR Encryption
   - AES192 CTR Encryption
   - AES256 CTR Encryption

   - AES128 CTR Decryption
   - AES192 CTR Decryption
   - AES256 CTR Decryption

The Plain data, encrypted data and decrypted data can be displayed on a PC
HyperTerminal using the UART.

The USARTx is configured as follows:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

@par Directory contents 

  - CRYP/CRYP_AESModes/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - CRYP/CRYP_AESModes/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - CRYP/CRYP_AESModes/Inc/main.h                  Header for main.c module  
  - CRYP/CRYP_AESModes/Src/stm32f7xx_it.c          Interrupt handlers
  - CRYP/CRYP_AESModes/Src/main.c                  Main program
  - CRYP/CRYP_AESModes/Src/stm32f7xx_hal_msp.c     HAL MSP module
  - CRYP/CRYP_AESModes/Src/system_stm32f7xx.c      STM32F7xx system source file


@par Hardware and Software environment

  - This example runs on STM32F756xx devices.

  - This example has been tested with STM32756G-EVAL board revB and can be
    easily tailored to any other supported device and development board.
  
  - Hyperterminal configuration:
    - BaudRate = 115200 baud  
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

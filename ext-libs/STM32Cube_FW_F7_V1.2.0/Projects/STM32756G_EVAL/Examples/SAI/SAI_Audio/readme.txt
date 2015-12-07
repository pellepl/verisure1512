/**
  @page SAI_AUDIO  How to use the Audio features with SAI peripheral
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    SAI/SAI_Audio/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Description of the STM32F7xx SAI Audio example.
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

This example provides basic implementation of audio features using BSP_AUDIO.
It allows to play two audio files at the same time through the SAI peripheral
and using the external codec WM8994 implemented on the STM327x6G-EVAL board revB.

In this example the SAI input clock, provided by a dedicated PLL (PLLI2S), is
configured to have an audio sampling frequency at 48 KHz on four channels:
stereo on AUDIO1 (headset) and stereo on AUDIO2 (speakers).

This example uses an audio codec driver which consists of three independent layers:
 - Codec Hardware layer: which controls and communicates with the audio codec 
   (WM8994) implemented on the evaluation board.
 - MAL (Media Access Layer): which controls and interfaces with storage media 
   storing or providing the audio file/stream.
 - The high layer: which implements overall control and interfacing API allowing
   to perform basic audio operations (Init, Play, Pause, Resume, Stop, Volume 
   control and audio file pointer management)
  
In this example the audio data is stored in the internal flash memory (Stereo, 
16-bit, 48 KHz).
The example also manages information display and control interface
through push buttons: 
 - When the application is Playing audio file:
     Short press on User push-button : Volume DOWN until looping
     Long press on User push-button : Pause/Resume

This example plays the audio data stored in internal flash memory and replays it
when it reaches end of file. But it can be tailored to use different media storage
devices; SDCard (through SDIO), external Memory (through FMC),... or to play 
in streaming mode (ie. from USB port in device or host mode). 

The WM8994 codec support the TDM format, for this the SAI peripheral is configured 
with 4 time slot (2 time slots left/right channels for headphone and 2 time slots 
left/right channels for speaker).
In the WM8994 configuration the SLOT0 (2 time slots left/right) is dedicated for 
the headphone and SLOT1 (2 time slots left/right) is dedicated for speaker.

List of Known Limitations and more detailed user notes are provided in file 
stm32756g_eval_audio.c   (under Drivers\BSP\STM32756G_EVAL)

The provided sample audio files (stored in internal flash memory) are:
- Downloaded from http://www.freesound.org
- Licensed under the Creative Commons 0 License:
  http://creativecommons.org/publicdomain/zero/1.0/


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this example uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      
@par Directory contents 

  - SAI/SAI_Audio/Inc/stm32f7xx_hal_conf.h    HAL configuration file
  - SAI/SAI_Audio/Inc/stm32f7xx_it.h          Interrupt handlers header file
  - SAI/SAI_Audio/Inc/main.h                  Header for main.c module  
  - SAI/SAI_Audio/Src/stm32f7xx_it.c          Interrupt handlers
  - SAI/SAI_Audio/Src/main.c                  Main program
  - SAI/SAI_Audio/Src/system_stm32f7xx.c      STM32F7xx system source file

      
@par Hardware and Software environment 

  - This example runs on STM32F756xx/STM32F746xx devices.
    
  - This example has been tested with STM327x6G-EVAL board revB and can be
    easily tailored to any other supported device and development board.
   
  - STM327x6G-EVAL Set-up :
    - jumper JP21 must be on the position 1-2 (Audio)
  	- Connect a headphone on the Audio_Out1 connector (CN26)  


@par How to use it ? 
 - Use STLink utility, available on www.st.com or any other in system programming
   tool to load "SAI/SAI_Audio/Binary/audio_sample_tdm.bin" file to the STM32 internal flash at the address 0x08080000.

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
         @note Make sure that the tool flash loader does not erase or overwrite the
            loaded audio file at address 0x08080000 by limiting the application
            end address to 0x08080000-1. This is already done for the example project
         @note This example runs only with (no optimization) on stm32f7xx_hal_dma.c file. 
 - Connect a headphone or a speaker to the audio jack connector (CN26).

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

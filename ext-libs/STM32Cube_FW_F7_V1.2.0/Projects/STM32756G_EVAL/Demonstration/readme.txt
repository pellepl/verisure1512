/**
  @page Demo   STM327x6G-EVAL Demonstration Firmware
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    Demonstrations/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-September-2015
  * @brief   Description of STM327x6G-EVAL Demonstration
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

@par Demo Description

The STM32Cube Demonstration platform comes on top of the STM32CubeTM as a firmware
package that offers a full set of software components based on a modules architecture
allowing re-using them separately in standalone applications. All these modules are
managed by the STM32Cube Demonstration kernel allowing to dynamically adding new
modules and access to common resources (storage, graphical components and widgets,
memory management, Real-Time operating system)

The STM32Cube Demonstration platform is built around the powerful graphical library
STemWin and the FreeRTOS real time operating system and uses almost the whole STM32
capability to offer a large scope of usage based on the STM32Cube HAL BSP and several
middleware components.

@par Demonstration Overview

The STM32 F7 demonstration is running on STM32756G/STM32746 evaluation boards 
  
Below you find an overview of the different offered module in the demonstration:

 + Video
 -------
 The video player module provides a video solution based on the STM32F7xx and STemWin
 movie API. It supports playing movie in emf format.
 You can use the *.emf video file provided under "Utilities/Media/Video".
 
 + Audio
 -------
 The audio player module provides a complete audio solution based on the STM32F7xx and
 delivers a high-quality music experience. It supports playing music in WAV format but may
 be extended to support other compressed formats such as MP3 and WMA audio formats.
 You can use the *.wav audio provided under "Utilities/Media/Audio" or any other ones.
  
 + Home alarm
 ------------ 
 Control of Home alarm system, equipped with cameras.
 Static picture shown when a room is selected and then the camera icon pressed
 General room alarm activation/deactivation when pressed.
 
 + Gardening control
 -------------------
 The gardening control module provides a graphic garden watering system behaviour
 
 + Game
 ------
 The game coming in the STM32Cube demonstration is based on the Reversi game. It is a
 strategy board game for two players, played on an 8×8 board. The goal of the game is to
 have the majority of disks turned to display your colour when the last playable empty square
 is filled.
 
 + System Info
 --------------  
 The system info module provides information about the core, supported eval boards, 
 CPU speed and demonstration version.  

 For more details about the demonstration modules please refers to  STM32CubeF7 demonstration (UM1906)
    
@note The STM32F7xx devices can reach a maximum clock frequency of 216MHz but as this demonstration uses SDRAM,
      the system clock is limited to 200MHz. Indeed proper functioning of the SDRAM is only guaranteed 
      at a maximum system clock frequency of 200MHz.
      

@par Hardware and Software environment

  - This demonstration runs on STM32F756xx and STM32F746xx devices.    
  - This demonstration has been tested with STM327x6G-EVAL RevB evaluation board.
  - Jumpers configuration:
      - JP5:  <1-2> ==> PD6 is as FMC_NWAIT (For NOR write)
      - JP10: must be not fitted ==> NOR write protection is disabled
      - JP18: <1-2> ==> Data signal on digital microphone is connected to audio codec(used for audio player module)   
      - JP19: <1-2> ==> Clock signal on digital microphone is connected to audio codec(used for audio player module)	   	  
      - JP21: <1-2> ==> PA2 is connected to SAI2_SCKB(used for audio player module)   
      - JP22: <1-2> ==> PC1 is connected to SAI1_SDA(used for audio player module)   
  - Make sure to connect an external power supply to CN17 (PSU jumper must be fitted in JP13)

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files
 - Open STM32 ST-Link Utility V3.7, click on "External Loader" from the bar menu then check "M29W128GL_STM32F756G-EVAL" box 
 - Connect the STM327x6G-EVAL board to PC with USB cable through CN21
 - Use "STM32CubeDemo_STM327x6G-EVAL_V1.2.0.hex" file with STM32 ST-Link Utility to program both internal Flash and external NOR memory
 - Run the demonstration
 - copy the audio and video files provided under "Utilities/Media/" in the USB key
 - Plug a USB micro A-Male to A-Female cable on CN8 connector 
    
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 

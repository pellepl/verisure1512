/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/Inc/main.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Header for main.c module
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32756g_eval.h"
#include "stm32756g_eval_lcd.h"
#include "stm32756g_eval_sram.h"
#include "stm32756g_eval_camera.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* LCD Frame Buffer address */
#define CAMERA_FRAME_BUFFER               0xC0260000
#define LCD_FRAME_BUFFER                  0xC0130000

/* #define TEST_RESOLUTION_R160x120         */
/*          */#define TEST_RESOLUTION_R320x240
/* #define TEST_RESOLUTION_R480x272         */
/* The following resolusion is valid only on MB1063 */
/* #define TEST_RESOLUTION_R640x480         */       

#if defined(TEST_RESOLUTION_R160x120)
#define resolution      RESOLUTION_R160x120
#define xsize           160
#define ysize           120
#define xoffset         240
#define yoffset         180
#endif

#if defined(TEST_RESOLUTION_R320x240)
#define resolution      RESOLUTION_R320x240
#define xsize           320
#define ysize           240
#define xoffset         160
#define yoffset         120
#endif

#if defined(TEST_RESOLUTION_R480x272)
#define resolution      RESOLUTION_R480x272
#define xsize           480
#define ysize           272
#define xoffset         80
#define yoffset         104
#endif

#if defined(TEST_RESOLUTION_R640x480)
#define resolution      RESOLUTION_R640x480
#define xsize           640
#define ysize           480
#define xoffset         0
#define yoffset         0
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

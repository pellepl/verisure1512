/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-September-2015  
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

/* EVAL includes component */
#include "stm32756g_eval.h"
#include "stm32756g_eval_sdram.h"
#include "stm32756g_eval_ts.h"
#include "stm32756g_eval_io.h"
#include "stm32756g_eval_sd.h"
#include "stm32756g_eval_audio.h"
#include "stm32756g_eval_nor.h"	 
   
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "sd_diskio.h"
#include "gui.h"
         
#include "ST_AnimatedIconView.h"   
#include "k_modules_res.h"
   
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define NUM_DISK_UNITS       2
#define USB_DISK_UNIT        0
#define MSD_DISK_UNIT        1
#define AUDIO_DEFAULT_VOLUME 70

#define CALIBRATION_AUDIOPLAYER_SETTING_BKP   RTC_BKP_DR0
#define CALIBRATION_AUDIOPLAYER_EQU1_BKP      RTC_BKP_DR1
#define CALIBRATION_AUDIOPLAYER_EQU2_BKP      RTC_BKP_DR2
#define CALIBRATION_AUDIOPLAYER_EQU3_BKP      RTC_BKP_DR3
#define CALIBRATION_AUDIOPLAYER_EQU4_BKP      RTC_BKP_DR4
#define CALIBRATION_AUDIOPLAYER_EQU5_BKP      RTC_BKP_DR5
#define CALIBRATION_AUDIOPLAYER_LOUD_BKP      RTC_BKP_DR6

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
extern GUI_CONST_STORAGE GUI_BITMAP bmF7Logo;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

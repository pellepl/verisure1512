/**
  ******************************************************************************
  * @file    k_modules_res.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-September-2015  
  * @brief   This file contains the Hex dumps of the images available 
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

/* Includes ------------------------------------------------------------------*/  
#include "k_modules_res.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MODULES_RES
  * @brief Kernel modules resources 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  draw a 3D rectangle
  * @param  x0: Upper left X-position
  * @param  y0: Upper left Y-position
  * @param  width: width  of the rectangle
  * @param  height: height  of the rectangle
  * @retval None.
  */
void DrawRect3D(int x0, int y0, int width, int height)
{
  GUI_SetColor(GUI_BLACK);
  GUI_DrawRect(x0, y0, (x0 + width), (y0 + height));

  GUI_DrawHLine((y0 + height - 1), (x0 + 1), (x0 + width - 1));
  GUI_DrawVLine((x0 + width - 1),  (y0 + 1), (y0 + height - 1));

  GUI_SetColor(GUI_WHITE);
  GUI_DrawVLine((x0 + 1), (y0 + 1), (y0 + height - 2));
  GUI_DrawHLine((y0 + 1), (x0 + 1), (x0 + width - 2));
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


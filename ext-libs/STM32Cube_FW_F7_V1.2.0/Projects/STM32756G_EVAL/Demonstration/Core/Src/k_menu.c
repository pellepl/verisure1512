/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-September-2015  
  * @brief   This file provides the kernel menu functions 
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dialog.h"
#include "messagebox.h"
#include "k_rtc.h"
#include "k_storage.h"
#include "k_module.h"

#include "cpu_utils.h"

#include "font_icon.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel menu routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define ID_ICONVIEW_MENU               (GUI_ID_USER + 0x00)
#define ID_FEATURES_FPU                (GUI_ID_USER + 0x44)
#define ID_FEATURES_IC                 (GUI_ID_USER + 0x45)
#define ID_FEATURES_DC                 (GUI_ID_USER + 0x46)
#define ID_FEATURES_CPU                (GUI_ID_USER + 0x47)
#define ID_FEATURES_FREQ               (GUI_ID_USER + 0x48)

uint32_t module_active = 0;

/**
  * @brief  Callback routine of desktop window.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  
  uint32_t NCode, Id;
  uint8_t sel;
    
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    /*H/W Information */
    GUI_SetColor(0x007B10D3);
    GUI_AA_FillRoundedRect(520, 220, 780, 478, 30);
    GUI_DrawBitmap(&bmF7Logo, 530, 240); 
    break;
       
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    NCode = pMsg->Data.v;  
    
    switch (NCode) 
    {  
      
    case WM_NOTIFICATION_RELEASED:
      if (Id == ID_ICONVIEW_MENU)
      {
        sel = ST_AnimatedIconView_GetSel(pMsg->hWinSrc);
        
        if(sel < k_ModuleGetNumber())
        {
          ST_AnimatedIconView_SetSel(pMsg->hWinSrc, -1);
          if(module_active == 0)
          {
            module_prop[sel].module->startup(pMsg->hWin, 0, 0);
            if(sel != 5)
            {
              module_active = 1;
            }
            sel = 0;
          }
        }
        else
        {
          WM_InvalidateWindow (pMsg->hWinSrc);
        }
      }
      break;

    case WM_NOTIFICATION_CHILD_DELETED:
      module_active = 0;
        break;
      
    default:
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  set the GUI profile
  * @param  None.
  * @retval None.
  */
void k_SetGuiProfile(void) 
{
  SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
  
  FRAMEWIN_SetDefaultTextColor(0, GUI_WHITE);
  FRAMEWIN_SetDefaultTextColor(1, GUI_WHITE);      
  
  FRAMEWIN_SetDefaultBarColor(0, 0x00DCA939);
  FRAMEWIN_SetDefaultBarColor(1, 0x00DCA939);
  
  FRAMEWIN_SetDefaultFont(&GUI_FontITCAvantGardeStdBk20);
  
  FRAMEWIN_SetDefaultClientColor(GUI_WHITE);  
  FRAMEWIN_SetDefaultTitleHeight(25);
  
  
  WINDOW_SetDefaultBkColor(GUI_WHITE);
  
  LISTVIEW_SetDefaultGridColor(GUI_WHITE);
  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_SEL, 0x00DCA939);
  DROPDOWN_SetDefaultColor(DROPDOWN_CI_SEL, 0x00DCA939);
  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_SELFOCUS, 0x00DCA939);
  DROPDOWN_SetDefaultColor(DROPDOWN_CI_SELFOCUS, 0x00DCA939);
  SCROLLBAR_SetDefaultWidth(16);
  
  HEADER_SetDefaultBkColor(0x00DCA939);
  HEADER_SetDefaultTextColor(GUI_WHITE);
  HEADER_SetDefaultFont(&GUI_FontITCAvantGardeStdBk20);
  SCROLLBAR_SetDefaultColor(0x00DCA939, SCROLLBAR_CI_THUMB);  
  SCROLLBAR_SetDefaultColor(0x00DCA939, SCROLLBAR_CI_SHAFT);  
  SCROLLBAR_SetDefaultColor(0x00DCA939, SCROLLBAR_CI_ARROW);    
  CHOOSEFILE_SetDelim('/');
  GUI_SetDefaultFont(GUI_FONT_20_ASCII);
  
  TEXT_SetDefaultTextColor(0x00DCA939);
  TEXT_SetDefaultFont(&GUI_FontLubalGraphXLtBT20);
}

/**
  * @brief  Initializes the menu.
  * @param  None.
  * @retval None.
  */
void k_InitMenu(void) 
{
  uint8_t i = 0;
  WM_HWIN   hItem;
  ICONVIEW_Handle hIcon ;
  
  WM_SetCallback(WM_GetDesktopWindowEx(0), _cbBk);
  
  hIcon = ST_AnimatedIconView_CreateEx(0, 
                                       0, 
                                       LCD_GetXSize() - 0, 
                                       LCD_GetYSize() - 0, 
                                       WM_GetDesktopWindowEx(0), 
                                       WM_CF_SHOW | WM_CF_HASTRANS ,
                                       0,
                                       ID_ICONVIEW_MENU, 
                                       150, 
                                       180, 5, 5);  
  
  ST_AnimatedIconView_SetDualFont(hIcon, &GUI_FontLubalGraphXLtBT20, &GUI_FontITCAvantGardeStdBk20);
  
  ST_AnimatedIconView_SetSpace(hIcon, GUI_COORD_Y, 5);
  ST_AnimatedIconView_SetSpace(hIcon, GUI_COORD_X, 5);
  ST_AnimatedIconView_SetFrame(hIcon, GUI_COORD_Y, 10);
  ST_AnimatedIconView_SetFrame(hIcon, GUI_COORD_X, 5);
  
  ST_AnimatedIconView_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, 0x00DCA939);
  ST_AnimatedIconView_SetBkColor(hIcon, ICONVIEW_CI_UNSEL, GUI_WHITE);
  ST_AnimatedIconView_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE);
  ST_AnimatedIconView_SetSel(hIcon, -1);
  ST_AnimatedIconView_SetDualTextColor(hIcon, ICONVIEW_CI_SEL, 0x00DCA939, 0x00522000);  
  
  for (i = 0; i < k_ModuleGetNumber(); i++)
  {   
    ST_AnimatedIconView_AddIcon(hIcon, module_prop[i].module->open_icon, module_prop[i].module->close_icon, (char *)module_prop[i].module->name);
  } 
  
  hItem = TEXT_CreateEx(548, 360, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_FPU, "FPU : ON");
  TEXT_SetFont(hItem, GUI_FONT_13B_1);
  TEXT_SetTextColor(hItem, GUI_WHITE); 
  
  hItem = TEXT_CreateEx(548, 380, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_IC, "I-Cache : ON");
  TEXT_SetFont(hItem, GUI_FONT_13B_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);     
  
  hItem = TEXT_CreateEx(548, 400, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_DC, "D-Cache : ON");
  TEXT_SetFont(hItem, GUI_FONT_13B_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);  
  
  hItem = TEXT_CreateEx(548, 420, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_CPU, "MCU Load < 1%");
  TEXT_SetFont(hItem, GUI_FONT_13B_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);  
  
  hItem = TEXT_CreateEx(548, 440, 260, 20, WM_GetDesktopWindowEx(0), WM_CF_SHOW, TEXT_CF_LEFT, ID_FEATURES_FREQ, "Clk : 200 MHz");
  TEXT_SetFont(hItem, GUI_FONT_13B_1);
  TEXT_SetTextColor(hItem, GUI_WHITE);    
  
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

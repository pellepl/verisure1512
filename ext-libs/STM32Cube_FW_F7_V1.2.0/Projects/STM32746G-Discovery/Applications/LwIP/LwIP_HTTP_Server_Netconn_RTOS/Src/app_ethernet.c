/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/app_ethernet.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    21-September-2015
  * @brief   Ethernet specefic module
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
#include "lwip/opt.h"
#include "main.h"
#include "lcd_log.h"
#include "lwip/dhcp.h"
#include "app_ethernet.h"
#include "ethernetif.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USE_DHCP
#define MAX_DHCP_TRIES  4
__IO uint8_t DHCP_state;
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Notify the User about the network interface config status
  * @param  netif: the network interface
  * @retval None
  */
void User_notification(struct netif *netif) 
{
  if (netif_is_up(netif))
  {
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
#else
    uint8_t iptxt[20];
    
    sprintf((char*)iptxt, "%d.%d.%d.%d", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    
    LCD_UsrLog ("Static IP address: %s\n", iptxt);
#endif /* USE_DHCP */
  }
  else
  {  
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
#endif  /* USE_DHCP */
    LCD_UsrLog ("The network cable is not connected \n");
  } 
}

#ifdef USE_DHCP
/**
* @brief  DHCP Process
* @param  argument: network interface
* @retval None
*/
void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  uint32_t IPaddress;
  
  for (;;)
  {
    switch (DHCP_state)
    {
    case DHCP_START:
      {
        netif->ip_addr.addr = 0;
        netif->netmask.addr = 0;
        netif->gw.addr = 0;
        IPaddress = 0;
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;
        LCD_UsrLog ("  State: Looking for DHCP server ...\n");
      }
      break;
      
    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = netif->ip_addr.addr;
        
        if (IPaddress!=0) 
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;	
          
          /* Stop DHCP */
          dhcp_stop(netif);
          
          uint8_t iptab[4];
          uint8_t iptxt[20];
          
          iptab[0] = (uint8_t)(IPaddress >> 24);
          iptab[1] = (uint8_t)(IPaddress >> 16);
          iptab[2] = (uint8_t)(IPaddress >> 8);
          iptab[3] = (uint8_t)(IPaddress);
          
          sprintf((char*)iptxt, "%d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);       
          
          LCD_UsrLog ("IP address assigned by a DHCP server: %s\n", iptxt);  
        }
        else
        {
          /* DHCP timeout */
          if (netif->dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;
            
            /* Stop DHCP */
            dhcp_stop(netif);
            
            /* Static address used */
            IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
            IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
            IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
            netif_set_addr(netif, &ipaddr , &netmask, &gw);
            
            uint8_t iptxt[20];
            
            sprintf((char*)iptxt, "%d.%d.%d.%d", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
            LCD_UsrLog ("DHCP timeout !!\n");
            LCD_UsrLog ("Static IP address  : %s\n", iptxt);
          }
        }
      }
      break;
      
    default: break;
    }
    
    /* wait 250 ms */
    osDelay(250);
  }
}
#endif  /* USE_DHCP */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

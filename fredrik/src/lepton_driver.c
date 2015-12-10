
#include <target.h>
#include <uart_driver.h>
#include <miniutils.h>

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"

#include "lepton_driver.h"

/* Definition for SPIx clock resources */
#define SPIx                             SPI2
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_CS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_1
#define SPIx_SCK_GPIO_PORT               GPIOI
#define SPIx_SCK_AF                      GPIO_AF5_SPI2

#define SPIx_MISO_PIN                    GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_AF                     GPIO_AF5_SPI2

#define SPIx_MOSI_PIN                    GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_AF                     GPIO_AF5_SPI2
// extra CS
#define SPIx_CS_PIN                      GPIO_PIN_15
#define SPIx_CS_GPIO_PORT                GPIOA

/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  if (hspi->Instance == SPIx)
  {
    print("MSP SPI INITING...\n");
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    SPIx_CS_GPIO_CLK_ENABLE();

    /* Enable SPI clock */
    SPIx_CLK_ENABLE(); 
    
    /*##-2- Configure peripheral GPIO ##########################################*/

    /* SPI CS GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_CS_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, GPIO_PIN_SET);
    
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * @brief SPI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPIx)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();
    
    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
  }
}

//---------------------------------------------------
void lepton_init(void)
{
  int spi_res;

  /*##-1- Configure the SPI peripheral #######################################*/
  /* Set the SPI parameters */
  SpiHandle.Instance               = SPIx;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  //SpiHandle.Init.NSS               = SPI_NSS_HARD_OUTPUT;

  SpiHandle.Init.Mode = SPI_MODE_MASTER;
  
  spi_res = HAL_SPI_Init(&SpiHandle);
  if (spi_res != HAL_OK)
  {
    /* Initialization Error */
    print("Init SPI error: %d\n", spi_res);
    while (1);
  }

  /*##-2- Start the Full Duplex Communication process ########################*/  
  /* While the SPI in TransmitReceive process, user can transmit data through 
     "aTxBuffer" buffer & receive data through "aRxBuffer" */
  /* Timeout is set to 5S */
  
  HAL_Delay(200);
  HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, GPIO_PIN_RESET); // set low  
  HAL_Delay(200);
}

//------------------------------------------------
// calculate crc16-ccitt very fast
// Idea from: http://www.ccsinfo.com/forum/viewtopic.php?t=24977
static uint16_t lepton_crc16(const uint8_t *buf, uint16_t len) 
{
  uint16_t x;
  uint16_t crc = 0;
  while (len--) {
    x = (crc >> 8) ^ *buf++;
    x ^= x >> 4;
    crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
  }
  return crc;
}

//---------------------------------------------
void lepton_get_video_packet(struct lepton_video_packet_s *packet)
{
  int resync = 0;
  int done = 0;
  while (! done)
  {
    int spi_res = HAL_SPI_Receive(&SpiHandle,
                                  (uint8_t*)packet,
                                  sizeof(struct lepton_video_packet_s),
                                  5000);

    switch (spi_res)
    {
    case HAL_OK: {
      // The 4 most sig bits not relevant
      packet->id[0] = packet->id[0] & 0x0F;
      uint16_t id = LEPTON_GET_ID(packet);
      if ((id & 0xFF) != 0xFF) {
        uint16_t crc_pkt = LEPTON_GET_CRC(packet);
        // clear CRC when do calc
        packet->crc[0] = 0;
        packet->crc[1] = 0;
        uint16_t crc_test = lepton_crc16((uint8_t*)packet,
                                         sizeof(struct lepton_video_packet_s));
        if (crc_pkt == crc_test) {
          done = 1;
        }
      }
      else {
        resync = 1;
      }
      break;
    }
    case HAL_TIMEOUT:
      /* An Error Occur ______________________________________________________ */
    case HAL_ERROR:
      /* Call Timeout Handler */
      print("SPI err: %d\n", spi_res);
      resync = 1;
      break;
    default:
      print("SPI err unknown? %d\n", spi_res);
      resync = 1;
      break;
    }

    if (resync) {
      print("RESYNC\n");      
      HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, GPIO_PIN_SET);
      HAL_Delay(200);
      HAL_GPIO_WritePin(SPIx_CS_GPIO_PORT, SPIx_CS_PIN, GPIO_PIN_RESET);      
    }    
  }
}

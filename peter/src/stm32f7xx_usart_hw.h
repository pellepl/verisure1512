/*
 * stm32f7xx_usart_hw.h
 *
 *  Created on: Dec 8, 2015
 *      Author: petera
 */

#ifndef STM32F7XX_USART_HW_H_
#define STM32F7XX_USART_HW_H_

#define USART_WORDLENGTH_7B                  ((uint32_t)USART_CR1_M_1)
#define USART_WORDLENGTH_8B                  ((uint32_t)0x00000000)
#define USART_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M_0)

#define USART_STOPBITS_1                     ((uint32_t)0x0000)
#define USART_STOPBITS_2                     ((uint32_t)USART_CR2_STOP_1)
#define USART_STOPBITS_1_5                   ((uint32_t)(USART_CR2_STOP_0 | USART_CR2_STOP_1))

#define USART_PARITY_NONE                   ((uint32_t)0x0000)
#define USART_PARITY_EVEN                   ((uint32_t)USART_CR1_PCE)
#define USART_PARITY_ODD                    ((uint32_t)(USART_CR1_PCE | USART_CR1_PS))

#define USART_MODE_RX                       ((uint32_t)USART_CR1_RE)
#define USART_MODE_TX                       ((uint32_t)USART_CR1_TE)
#define USART_MODE_TX_RX                    ((uint32_t)(USART_CR1_TE |USART_CR1_RE))

#define USART_OVERSAMPLING_16               ((uint32_t)0x0000)
#define USART_OVERSAMPLING_8                ((uint32_t)USART_CR1_OVER8)

#define USART_CLOCK_DISABLE                 ((uint32_t)0x0000)
#define USART_CLOCK_ENABLE                  ((uint32_t)USART_CR2_CLKEN)

#define USART_POLARITY_LOW                  ((uint32_t)0x0000)
#define USART_POLARITY_HIGH                 ((uint32_t)USART_CR2_CPOL)

#define USART_PHASE_1EDGE                   ((uint32_t)0x0000)
#define USART_PHASE_2EDGE                   ((uint32_t)USART_CR2_CPHA)

#define USART_LASTBIT_DISABLE               ((uint32_t)0x0000)
#define USART_LASTBIT_ENABLE                ((uint32_t)USART_CR2_LBCL)

#define UART_AUTOBAUD_REQUEST               ((uint32_t)USART_RQR_ABRRQ)        /*!< Auto-Baud Rate Request */
#define UART_SENDBREAK_REQUEST              ((uint32_t)USART_RQR_SBKRQ)        /*!< Send Break Request */
#define UART_MUTE_MODE_REQUEST              ((uint32_t)USART_RQR_MMRQ)         /*!< Mute Mode Request */
#define UART_RXDATA_FLUSH_REQUEST           ((uint32_t)USART_RQR_RXFRQ)        /*!< Receive Data flush Request */
#define UART_TXDATA_FLUSH_REQUEST           ((uint32_t)USART_RQR_TXFRQ)        /*!< Transmit data flush Request */

#define USART_FLAG_REACK                     ((uint32_t)0x00400000)
#define USART_FLAG_TEACK                     ((uint32_t)0x00200000)
#define USART_FLAG_BUSY                      ((uint32_t)0x00010000)
#define USART_FLAG_CTS                       ((uint32_t)0x00000400)
#define USART_FLAG_CTSIF                     ((uint32_t)0x00000200)
#define USART_FLAG_LBDF                      ((uint32_t)0x00000100)
#define USART_FLAG_TXE                       ((uint32_t)0x00000080)
#define USART_FLAG_TC                        ((uint32_t)0x00000040)
#define USART_FLAG_RXNE                      ((uint32_t)0x00000020)
#define USART_FLAG_IDLE                      ((uint32_t)0x00000010)
#define USART_FLAG_ORE                       ((uint32_t)0x00000008)
#define USART_FLAG_NE                        ((uint32_t)0x00000004)
#define USART_FLAG_FE                        ((uint32_t)0x00000002)
#define USART_FLAG_PE                        ((uint32_t)0x00000001)

#define USART_IT_PE                          ((uint16_t)0x0028)
#define USART_IT_TXE                         ((uint16_t)0x0727)
#define USART_IT_TC                          ((uint16_t)0x0626)
#define USART_IT_RXNE                        ((uint16_t)0x0525)
#define USART_IT_IDLE                        ((uint16_t)0x0424)
#define USART_IT_ERR                         ((uint16_t)0x0060)

#define USART_IT_ORE                         ((uint16_t)0x0300)
#define USART_IT_NE                          ((uint16_t)0x0200)
#define USART_IT_FE                          ((uint16_t)0x0100)

#define USART_CLEAR_PEF                       USART_ICR_PECF            /*!< Parity Error Clear Flag */
#define USART_CLEAR_FEF                       USART_ICR_FECF            /*!< Framing Error Clear Flag */
#define USART_CLEAR_NEF                       USART_ICR_NCF             /*!< Noise detected Clear Flag */
#define USART_CLEAR_OREF                      USART_ICR_ORECF           /*!< OverRun Error Clear Flag */
#define USART_CLEAR_IDLEF                     USART_ICR_IDLECF          /*!< IDLE line detected Clear Flag */
#define USART_CLEAR_TCF                       USART_ICR_TCCF            /*!< Transmission Complete Clear Flag */
#define USART_CLEAR_CTSF                      USART_ICR_CTSCF           /*!< CTS Interrupt Clear Flag */

#define UART_HWCONTROL_NONE                  ((uint32_t)0x00000000)
#define UART_HWCONTROL_RTS                   ((uint32_t)USART_CR3_RTSE)
#define UART_HWCONTROL_CTS                   ((uint32_t)USART_CR3_CTSE)
#define UART_HWCONTROL_RTS_CTS               ((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE))

#define USART_CR1_FIELDS  ((uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | \
                                     USART_CR1_TE | USART_CR1_RE  | USART_CR1_OVER8))
#define USART_CR2_FIELDS       ((uint32_t)(USART_CR2_CPHA | USART_CR2_CPOL | \
                            USART_CR2_CLKEN | USART_CR2_LBCL | USART_CR2_STOP))

#define _UART_DIV_SAMPLING8(_PCLK_, _BAUD_)             (((_PCLK_)*2)/((_BAUD_)))

typedef enum
{
  UART_CLOCKSOURCE_PCLK1      = 0x00,    /*!< PCLK1 clock source  */
  UART_CLOCKSOURCE_PCLK2      = 0x01,    /*!< PCLK2 clock source  */
  UART_CLOCKSOURCE_HSI        = 0x02,    /*!< HSI clock source    */
  UART_CLOCKSOURCE_SYSCLK     = 0x04,    /*!< SYSCLK clock source */
  UART_CLOCKSOURCE_LSE        = 0x08,    /*!< LSE clock source       */
  UART_CLOCKSOURCE_UNDEFINED  = 0x10     /*!< Undefined clock source */
} UART_ClockSourceTypeDef;

/*
 * How horrible
 */
#define _UART_GETCLOCKSOURCE(__USART__,__CLOCKSOURCE__) \
  do {                                                        \
    if((__USART__) == USART1)                      \
    {                                                         \
       switch(__HAL_RCC_GET_USART1_SOURCE())                  \
       {                                                      \
        case RCC_USART1CLKSOURCE_PCLK2:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK2;         \
          break;                                              \
        case RCC_USART1CLKSOURCE_HSI:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_USART1CLKSOURCE_SYSCLK:                      \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_USART1CLKSOURCE_LSE:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if((__USART__) == USART2)                 \
    {                                                         \
       switch(__HAL_RCC_GET_USART2_SOURCE())                  \
       {                                                      \
        case RCC_USART2CLKSOURCE_PCLK1:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK1;         \
          break;                                              \
        case RCC_USART2CLKSOURCE_HSI:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_USART2CLKSOURCE_SYSCLK:                      \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_USART2CLKSOURCE_LSE:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if((__USART__) == USART3)                 \
    {                                                         \
       switch(__HAL_RCC_GET_USART3_SOURCE())                  \
       {                                                      \
        case RCC_USART3CLKSOURCE_PCLK1:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK1;         \
          break;                                              \
        case RCC_USART3CLKSOURCE_HSI:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_USART3CLKSOURCE_SYSCLK:                      \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_USART3CLKSOURCE_LSE:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if((__USART__) == UART4)                  \
    {                                                         \
       switch(__HAL_RCC_GET_UART4_SOURCE())                   \
       {                                                      \
        case RCC_UART4CLKSOURCE_PCLK1:                        \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK1;         \
          break;                                              \
        case RCC_UART4CLKSOURCE_HSI:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_UART4CLKSOURCE_SYSCLK:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_UART4CLKSOURCE_LSE:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if ((__USART__) == UART5)                 \
    {                                                         \
       switch(__HAL_RCC_GET_UART5_SOURCE())                   \
       {                                                      \
        case RCC_UART5CLKSOURCE_PCLK1:                        \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK1;         \
          break;                                              \
        case RCC_UART5CLKSOURCE_HSI:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_UART5CLKSOURCE_SYSCLK:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_UART5CLKSOURCE_LSE:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if((__USART__) == USART6)                 \
    {                                                         \
       switch(__HAL_RCC_GET_USART6_SOURCE())                  \
       {                                                      \
        case RCC_USART6CLKSOURCE_PCLK2:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK2;         \
          break;                                              \
        case RCC_USART6CLKSOURCE_HSI:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_USART6CLKSOURCE_SYSCLK:                      \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_USART6CLKSOURCE_LSE:                         \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if ((__USART__) == UART7)                 \
    {                                                         \
       switch(__HAL_RCC_GET_UART7_SOURCE())                   \
       {                                                      \
        case RCC_UART7CLKSOURCE_PCLK1:                        \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK1;         \
          break;                                              \
        case RCC_UART7CLKSOURCE_HSI:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_UART7CLKSOURCE_SYSCLK:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_UART7CLKSOURCE_LSE:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
    else if ((__USART__) == UART8)                 \
    {                                                         \
       switch(__HAL_RCC_GET_UART8_SOURCE())                   \
       {                                                      \
        case RCC_UART8CLKSOURCE_PCLK1:                        \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_PCLK1;         \
          break;                                              \
        case RCC_UART8CLKSOURCE_HSI:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_HSI;           \
          break;                                              \
        case RCC_UART8CLKSOURCE_SYSCLK:                       \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_SYSCLK;        \
          break;                                              \
        case RCC_UART8CLKSOURCE_LSE:                          \
          (__CLOCKSOURCE__) = UART_CLOCKSOURCE_LSE;           \
          break;                                              \
        default:                                              \
          break;                                              \
       }                                                      \
    }                                                         \
  } while(0)

#define UART_IT_MASK                        ((uint32_t)0x001F)

#define UART_IT_PE                          ((uint32_t)0x0028)
#define UART_IT_TXE                         ((uint32_t)0x0727)
#define UART_IT_TC                          ((uint32_t)0x0626)
#define UART_IT_RXNE                        ((uint32_t)0x0525)
#define UART_IT_IDLE                        ((uint32_t)0x0424)
#define UART_IT_LBD                         ((uint32_t)0x0846)
#define UART_IT_CTS                         ((uint32_t)0x096A)
#define UART_IT_CM                          ((uint32_t)0x112E)
#define UART_IT_ERR                         ((uint32_t)0x0060)

#define UART_CLEAR_PEF                       USART_ICR_PECF            /*!< Parity Error Clear Flag */
#define UART_CLEAR_FEF                       USART_ICR_FECF            /*!< Framing Error Clear Flag */
#define UART_CLEAR_NEF                       USART_ICR_NCF             /*!< Noise detected Clear Flag */
#define UART_CLEAR_OREF                      USART_ICR_ORECF           /*!< OverRun Error Clear Flag */
#define UART_CLEAR_IDLEF                     USART_ICR_IDLECF          /*!< IDLE line detected Clear Flag */
#define UART_CLEAR_TCF                       USART_ICR_TCCF            /*!< Transmission Complete Clear Flag */
#define UART_CLEAR_LBDF                      USART_ICR_LBDCF           /*!< LIN Break Detection Clear Flag */
#define UART_CLEAR_CTSF                      USART_ICR_CTSCF           /*!< CTS Interrupt Clear Flag */
#define UART_CLEAR_RTOF                      USART_ICR_RTOCF           /*!< Receiver Time Out Clear Flag */
#define UART_CLEAR_EOBF                      USART_ICR_EOBCF           /*!< End Of Block Clear Flag */
#define UART_CLEAR_CMF                       USART_ICR_CMCF            /*!< Character Match Clear Flag */

#define __HAL_UART_CLEAR_IT(__USART__, __FLAG__) ((__USART__)->ICR = ~(__FLAG__))

/** @brief  Enables the specified UART interrupt.
  * @param  __USART__: specifies the UART Handle.
  * @param  __INTERRUPT__: specifies the UART interrupt source to enable.
  *          This parameter can be one of the following values:
  *            @arg UART_IT_WUF:  Wakeup from stop mode interrupt
  *            @arg UART_IT_CM:   Character match interrupt
  *            @arg UART_IT_CTS:  CTS change interrupt
  *            @arg UART_IT_LBD:  LIN Break detection interrupt
  *            @arg UART_IT_TXE:  Transmit Data Register empty interrupt
  *            @arg UART_IT_TC:   Transmission complete interrupt
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg UART_IT_IDLE: Idle line detection interrupt
  *            @arg UART_IT_PE:   Parity Error interrupt
  *            @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @retval None
  */
#define _UART_ENABLE_IT(__USART__, __INTERRUPT__)   (((((uint8_t)(__INTERRUPT__)) >> 5U) == 1)? ((__USART__)->CR1 |= (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((((uint8_t)(__INTERRUPT__)) >> 5U) == 2)? ((__USART__)->CR2 |= (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((__USART__)->CR3 |= (1U << ((__INTERRUPT__) & UART_IT_MASK))))


/** @brief  Disables the specified UART interrupt.
  * @param  __USART__: specifies the UART Handle.
  * @param  __INTERRUPT__: specifies the UART interrupt source to disable.
  *          This parameter can be one of the following values:
  *            @arg UART_IT_CM:   Character match interrupt
  *            @arg UART_IT_CTS:  CTS change interrupt
  *            @arg UART_IT_LBD:  LIN Break detection interrupt
  *            @arg UART_IT_TXE:  Transmit Data Register empty interrupt
  *            @arg UART_IT_TC:   Transmission complete interrupt
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg UART_IT_IDLE: Idle line detection interrupt
  *            @arg UART_IT_PE:   Parity Error interrupt
  *            @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @retval None
  */
#define _UART_DISABLE_IT(__USART__, __INTERRUPT__)  (((((uint8_t)(__INTERRUPT__)) >> 5U) == 1)? ((__USART__)->CR1 &= ~ (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((((uint8_t)(__INTERRUPT__)) >> 5U) == 2)? ((__USART__)->CR2 &= ~ (1U << ((__INTERRUPT__) & UART_IT_MASK))): \
                                                           ((__USART__)->CR3 &= ~ (1U << ((__INTERRUPT__) & UART_IT_MASK))))

#define __HAL_UART_SEND_REQ(__USART__, __REQ__) ((__USART__)->RQR |= (uint32_t)(__REQ__))

#endif /* STM32F7XX_USART_HW_H_ */

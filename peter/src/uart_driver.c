#include "uart_driver.h"
#include "system.h"
#include "stm32f7xx_usart_hw.h"

#define UART_HW(u)      ((USART_TypeDef *)((u)->hw))

#define UART_CHECK_RX(u) (UART_HW(u)->ISR & USART_FLAG_RXNE)
#define UART_CHECK_TX(u) (UART_HW(u)->ISR & USART_FLAG_TXE)
#define UART_CHECK_OR(u) (UART_HW(u)->ISR & USART_FLAG_ORE)
#define UART_SET_RX_IRQ_OFF(u) UART_HW(u)->CR1 &= ~USART_CR1_RXNEIE
#define UART_SET_RX_IRQ_ON(u)  UART_HW(u)->CR1 |= USART_CR1_RXNEIE
#define UART_SET_TX_IRQ_OFF(u) UART_HW(u)->CR1 &= ~USART_CR1_TXEIE
#define UART_SET_TX_IRQ_ON(u)  UART_HW(u)->CR1 |= USART_CR1_TXEIE
#define UART_IS_RX_IRQ_ON(u)  (UART_HW(u)->CR1 & USART_CR1_RXNEIE)
#define UART_IS_TX_IRQ_ON(u)  (UART_HW(u)->CR1 & USART_CR1_TXEIE)

void UART_irq(uart *u) {

  if (u->hw == NULL) return;

  if (UART_CHECK_RX(u) && UART_IS_RX_IRQ_ON(u)) {
    u8_t c = UART_HW(u)->RDR;
    u->rx.buf[u->rx.wix++] = c;
    if (u->rx.wix >= UART_RX_BUFFER) {
      u->rx.wix = 0;
    }
    // not needed acc to spec
    __HAL_UART_SEND_REQ(UART_HW(u), UART_RXDATA_FLUSH_REQUEST);

    if (u->rx_f) {
      u->rx_f(u->arg, c);
    }

  }
  if (UART_CHECK_TX(u)) {
    if (UART_ALWAYS_SYNC_TX || u->sync_tx) {
      //_UART_DISABLE_IT(UART_HW(u), USART_IT_TXE);
      //UART_SET_TX_IRQ_OFF(u);
      _UART_DISABLE_IT(UART_HW(u), USART_IT_TC);
    } else {
      if (u->tx.wix != u->tx.rix) {
        UART_HW(u)->TDR = u->tx.buf[u->tx.rix++];
        if (u->tx.rix >= UART_TX_BUFFER) {
          u->tx.rix = 0;
        }
      }
      if (u->tx.wix == u->tx.rix) {
        UART_SET_TX_IRQ_OFF(u);
        _UART_DISABLE_IT(UART_HW(u), USART_IT_TXE);
        _UART_DISABLE_IT(UART_HW(u), USART_IT_TC);
      }
    }
  }
  // TODO
  //if (UART_CHECK_OR(u)) {
  //  __HAL_UART_CLEAR_IT(UART_HW(u), UART_CLEAR_OREF);
  //}

}

u16_t UART_rx_available(uart *u) {
  volatile u16_t r = u->rx.rix;
  volatile u16_t w = u->rx.wix;
  if (w >= r) {
    return w - r;
  } else {
    return w + UART_RX_BUFFER - r;
  }
}

u16_t UART_tx_available(uart *u) {
  volatile u16_t r = u->tx.rix;
  volatile u16_t w = u->tx.wix;
  if (w > r) {
    return UART_TX_BUFFER - (w - r) - 1;
  } else {
    return UART_TX_BUFFER - (w + UART_TX_BUFFER - r) - 1;
  }
}

void UART_tx_drain(uart *uart) {
#if !UART_SYNC_TX
  uart->tx.rix = 0;
  uart->tx.wix = 0;
#endif
}

void UART_tx_flush(uart *u) {
  UART_SET_RX_IRQ_OFF(u);
  u16_t rix = u->tx.rix;
  u16_t wix = u->tx.wix;
  while (rix != wix) {
    u8_t c = u->tx.buf[rix];
    UART_tx_force_char(u, c);
    if (rix >= UART_RX_BUFFER - 1) {
      rix = 0;
    } else {
      rix++;
    }
  }
  u->tx.rix = rix;
  UART_SET_RX_IRQ_ON(u);
}

s32_t UART_get_char(uart *u) {
  s32_t c = -1;
  UART_SET_RX_IRQ_OFF(u);
  if (u->rx.rix != u->rx.wix) {
    c = u->rx.buf[u->rx.rix];
    if (u->rx.rix >= UART_RX_BUFFER - 1) {
      u->rx.rix = 0;
    } else {
      u->rx.rix++;
    }
  }
  UART_SET_RX_IRQ_ON(u);
  return c;
}

s32_t UART_get_buf(uart *u, u8_t* dst, u16_t len) {

  UART_SET_RX_IRQ_OFF(u);
  u16_t avail = UART_rx_available(u);
  s32_t len_to_read = MIN(avail, len);
  if (len_to_read == 0) {
    UART_SET_RX_IRQ_ON(u);
    return 0;
  }
  u32_t remaining = len_to_read;
  u32_t len_to_end = UART_RX_BUFFER - u->rx.rix;
  if (remaining > len_to_end) {
    if (dst) {
      memcpy(dst, &u->rx.buf[u->rx.rix], len_to_end);
      dst += len_to_end;
    }
    remaining -= len_to_end;
    u->rx.rix = 0;
  }
  if (dst) {
    memcpy(dst, &u->rx.buf[u->rx.rix], remaining);
  }
  u->rx.rix += remaining;
  if (u->rx.rix >= UART_RX_BUFFER) {
    u->rx.rix = 0;
  }

  UART_SET_RX_IRQ_ON(u);
  return len_to_read;
}

s32_t UART_put_char(uart *u, u8_t c) {
  if (UART_ALWAYS_SYNC_TX || u->sync_tx) {
    UART_tx_force_char(u, c);
    return 0;
  } else {
    s32_t res;
    int max_tries = 0x10000;
    do {
      res = 0;

      if (UART_tx_available(u) > 0) {
        u->tx.buf[u->tx.wix++] = c;
        if (u->tx.wix >= UART_TX_BUFFER) {
          u->tx.wix = 0;
        }
      } else {
        res = -1;
      }
      UART_SET_TX_IRQ_ON(u);
    } while (u->assure_tx && res != 0 && --max_tries > 0);
    return res;
  }
}

void UART_tx_force_char(uart *u, u8_t c) {
  while (UART_CHECK_TX(u) == 0)
    ;
  UART_HW(u)->TDR = (uint8_t) c;
}

s32_t UART_put_buf(uart *u, u8_t* c, u16_t len) {
  if (UART_ALWAYS_SYNC_TX || u->sync_tx) {
    u16_t tlen = len;
    while (tlen-- > 0) {
      UART_put_char(u, *c++);
    }
    return len;
  } else {
    u32_t written = 0;
    u16_t guard = 0;
    do {
      u16_t avail = UART_tx_available(u);
      s32_t len_to_write = MIN(len - written, avail);
      guard++;
      if (len_to_write == 0) {
        while (UART_CHECK_TX(u) == 0)
          ;
        len_to_write = 1;
      }
      guard = 0;
      u32_t remaining = len_to_write;
      u32_t len_to_end = UART_TX_BUFFER - u->tx.wix;
      if (remaining > len_to_end) {
        memcpy(&u->tx.buf[u->tx.wix], c, len_to_end);
        c += len_to_end;
        remaining -= len_to_end;
        u->tx.wix = 0;
      }
      memcpy(&u->tx.buf[u->tx.wix], c, remaining);
      c += remaining;
      u->tx.wix += remaining;
      if (u->tx.wix >= UART_RX_BUFFER) {
        u->tx.wix = 0;
      }
      UART_SET_TX_IRQ_ON(u);
      written += len_to_write;
    } while (u->assure_tx && written < len && guard < 0xff);

    return written;
  }
}

void UART_set_callback(uart *u, uart_rx_callback rx_f, void* arg) {
  u->rx_f = rx_f;
  u->arg = arg;
}

void UART_get_callback(uart *u, uart_rx_callback *rx_f, void **arg) {
  *rx_f = u->rx_f;
  *arg = u->arg;
}

uart __uart_vec[CONFIG_UART_CNT];

bool UART_assure_tx(uart *u, bool on) {
  bool old = u->assure_tx;
  u->assure_tx = on;
  return old;
}

bool UART_sync_tx(uart *u, bool on) {
  bool old = u->sync_tx;
  u->sync_tx = on;
  return old;
}

bool UART_config(uart *uart, u32_t baud, UART_databits databits,
    UART_stopbits stopbits, UART_parity parity, UART_flowcontrol flowcontrol,
    bool activate) {
  UART_HW(uart)->CR1 &= ~USART_CR1_UE; // disable uart

  if (activate) {
    u32_t sdatabits = 0;
    u32_t sstopbits = 0;
    u32_t sparity = 0;
    u32_t sflowcontrol = 0;
    uint16_t usartdiv                   = 0x0000;

    UART_ClockSourceTypeDef clocksource = UART_CLOCKSOURCE_UNDEFINED;
    switch (databits) {
    case UART_DATABITS_8: sdatabits = USART_WORDLENGTH_8B; break;
    case UART_DATABITS_9: sdatabits = USART_WORDLENGTH_9B; break;
    default: return FALSE;
    }
    switch (stopbits) {
    case UART_STOPBITS_0_5: sstopbits = USART_STOPBITS_1; break;
    case UART_STOPBITS_1: sstopbits = USART_STOPBITS_1; break;
    case UART_STOPBITS_1_5: sstopbits = USART_STOPBITS_1_5; break;
    case UART_STOPBITS_2: sstopbits = USART_STOPBITS_2; break;
    default: return FALSE;
    }
    switch (parity) {
    case UART_PARITY_NONE: sparity = USART_PARITY_NONE; break;
    case UART_PARITY_EVEN: sparity = USART_PARITY_EVEN; break;
    case UART_PARITY_ODD: sparity = USART_PARITY_ODD; break;
    default: return FALSE;
    }
    switch (flowcontrol) {
    case UART_FLOWCONTROL_NONE: sflowcontrol = UART_HWCONTROL_NONE; break;
    case UART_FLOWCONTROL_CTS: sflowcontrol = UART_HWCONTROL_CTS; break;
    case UART_FLOWCONTROL_RTS: sflowcontrol = UART_HWCONTROL_RTS; break;
    case UART_FLOWCONTROL_RTS_CTS: sflowcontrol = UART_HWCONTROL_RTS_CTS; break;
    default: return FALSE;
    }
    UART_HW(uart)->CR1 = 0;
    UART_HW(uart)->CR2 = 0;
    UART_HW(uart)->CR3 = 0;
    /*-------------------------- USART CR1 Configuration -----------------------*/
    MODIFY_REG(UART_HW(uart)->CR1, USART_CR1_FIELDS,
        sdatabits | sparity | USART_CR1_OVER8 | USART_CR1_RE | USART_CR1_TE);

    /*---------------------------- USART CR2 Configuration ---------------------*/
    MODIFY_REG(UART_HW(uart)->CR2, USART_CR2_STOP,
        sstopbits);

    UART_HW(uart)->CR2 &= ~USART_CR2_LINEN;

    /*-------------------------- USART CR3 Configuration -----------------------*/
    MODIFY_REG(UART_HW(uart)->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT),
        sflowcontrol);

    _UART_GETCLOCKSOURCE(UART_HW(uart), clocksource);

    switch (clocksource)
    {
    case UART_CLOCKSOURCE_PCLK1:
        usartdiv = (uint16_t)(_UART_DIV_SAMPLING8(HAL_RCC_GetPCLK1Freq(), baud));
      break;
    case UART_CLOCKSOURCE_PCLK2:
        usartdiv = (uint16_t)(_UART_DIV_SAMPLING8(HAL_RCC_GetPCLK2Freq(), baud));
      break;
    case UART_CLOCKSOURCE_HSI:
        usartdiv = (uint16_t)(_UART_DIV_SAMPLING8(HSI_VALUE, baud));
      break;
    case UART_CLOCKSOURCE_SYSCLK:
        usartdiv = (uint16_t)(_UART_DIV_SAMPLING8(HAL_RCC_GetSysClockFreq(), baud));
      break;
    case UART_CLOCKSOURCE_LSE:
        usartdiv = (uint16_t)(_UART_DIV_SAMPLING8(LSE_VALUE, baud));
      break;
      case UART_CLOCKSOURCE_UNDEFINED:
    default:
        return FALSE;
      break;
    }

    u32_t brrtemp = usartdiv & 0xFFF0;
    brrtemp |= (uint16_t)((usartdiv & (uint16_t)0x000F) >> 1U);
    UART_HW(uart)->BRR = brrtemp;
    // enable usart interrupts
    _UART_ENABLE_IT(UART_HW(uart), USART_IT_TC);
    _UART_ENABLE_IT(UART_HW(uart), USART_IT_TXE);
    _UART_ENABLE_IT(UART_HW(uart), USART_IT_RXNE);
    UART_HW(uart)->CR1 |= USART_CR1_UE; // enable uart
  } else {
    // disable usart interrupts
    _UART_DISABLE_IT(UART_HW(uart), USART_IT_TC);
    _UART_DISABLE_IT(UART_HW(uart), USART_IT_TXE);
    _UART_DISABLE_IT(UART_HW(uart), USART_IT_RXNE);
    UART_HW(uart)->CR1 &= ~USART_CR1_UE; // disable uart
  }
  return TRUE;
}


void UART_init() {
  memset(__uart_vec, 0, sizeof(__uart_vec));
  int uc = 0;

#ifdef CONFIG_UART6
  _UART(uc++)->hw = USART6;
  UART_config(_UART(uc-1), 115200, UART_DATABITS_8, UART_STOPBITS_1,
      UART_PARITY_NONE, UART_FLOWCONTROL_NONE, TRUE);
  UART_SET_TX_IRQ_OFF(_UART(uc-1));
#endif
}

void USART6_IRQHandler(void) {
  UART_irq(_UART(0));
}




#if 0
static bool hw_uart_config(uart *u, u32_t baud, UART_databits databits,
    UART_stopbits stopbits, UART_parity parity, UART_flowcontrol flowcontrol)
{
  uint32_t tmpreg      = 0x0;
  USART_ClockSourceTypeDef clocksource = USART_CLOCKSOURCE_UNDEFINED;
  HAL_StatusTypeDef ret                = HAL_OK;
  uint16_t brrtemp                     = 0x0000;
  uint16_t usartdiv                    = 0x0000;

  /*-------------------------- USART CR1 Configuration -----------------------*/
   /* Clear M, PCE, PS, TE and RE bits and configure
   *  the USART Word Length, Parity, Mode and OverSampling:
   *  set the M bits according to husart->Init.WordLength value
   *  set PCE and PS bits according to husart->Init.Parity value
   *  set TE and RE bits according to husart->Init.Mode value
   *  force OVER8 to 1 to allow to reach the maximum speed (Fclock/8) */
  tmpreg = (uint32_t)husart->Init.WordLength | husart->Init.Parity | husart->Init.Mode | USART_CR1_OVER8;
  MODIFY_REG(UART_HW(uart), USART_CR1_FIELDS, tmpreg);

  /*---------------------------- USART CR2 Configuration ---------------------*/
  /* Clear and configure the USART Clock, CPOL, CPHA, LBCL and STOP bits:
   * set CPOL bit according to husart->Init.CLKPolarity value
   * set CPHA bit according to husart->Init.CLKPhase value
   * set LBCL bit according to husart->Init.CLKLastBit value
   * set STOP[13:12] bits according to husart->Init.StopBits value */
  tmpreg = (uint32_t)(USART_CLOCK_ENABLE);
  tmpreg |= ((uint32_t)husart->Init.CLKPolarity | (uint32_t)husart->Init.CLKPhase);
  tmpreg |= ((uint32_t)husart->Init.CLKLastBit | (uint32_t)husart->Init.StopBits);
  MODIFY_REG(UART_HW(uart)->CR2, USART_CR2_FIELDS, tmpreg);

  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* no CR3 register configuration                                            */

  /*-------------------------- USART BRR Configuration -----------------------*/
  /* BRR is filled-up according to OVER8 bit setting which is forced to 1     */
  USART_GETCLOCKSOURCE(husart, clocksource);
  switch (clocksource)
  {
    case USART_CLOCKSOURCE_PCLK1:
      usartdiv = (uint16_t)((2*HAL_RCC_GetPCLK1Freq()) / husart->Init.BaudRate);
      break;
    case USART_CLOCKSOURCE_PCLK2:
      usartdiv = (uint16_t)((2*HAL_RCC_GetPCLK2Freq()) / husart->Init.BaudRate);
      break;
    case USART_CLOCKSOURCE_HSI:
      usartdiv = (uint16_t)((2*HSI_VALUE) / husart->Init.BaudRate);
      break;
    case USART_CLOCKSOURCE_SYSCLK:
      usartdiv = (uint16_t)((2*HAL_RCC_GetSysClockFreq()) / husart->Init.BaudRate);
      break;
    case USART_CLOCKSOURCE_LSE:
      usartdiv = (uint16_t)((2*LSE_VALUE) / husart->Init.BaudRate);
      break;
    case USART_CLOCKSOURCE_UNDEFINED:
    default:
      ret = HAL_ERROR;
      break;
  }

  brrtemp = usartdiv & 0xFFF0;
  brrtemp |= (uint16_t)((usartdiv & (uint16_t)0x000F) >> 1U);
  UART_HW(uart)->BRR = brrtemp;

  return ret;
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include "GUI.h"
#include "WM.h"
#include "FRAMEWIN.h"
#include "LCDConf.h"

#include "NuMicro.h"
#include "platform.h"
#include "thermostat.h"

#include "M48XTouchPanel.h"

extern volatile GUI_TIMER_TIME OS_TimeMS;
extern int ts_writefile(void);
extern int ts_readfile(void);

int ts_calibrate(int xsize, int ysize);

/* Private typedef -----------------------------------------------------------*/   

/* Private define ------------------------------------------------------------*/   

/* Private macro -------------------------------------------------------------*/   

/* Private variables ---------------------------------------------------------*/   

/**********************************************************************************************
 *  Global I2C Contexts
 **********************************************************************************************/
I2C_CONTEXT I2C1_Ctx = { .base = I2C1 };
I2C_CONTEXT I2C2_Ctx = { .base = I2C2 };

static I2C_FUNC s_I2C2HandlerFn = NULL;

uint8_t g_u8DeviceAddr;
uint8_t g_au8TxData[3];
uint8_t g_u8RxData;
volatile uint8_t g_u8DataLen;
volatile uint8_t g_u8EndFlag = 0;

volatile int g_enable_Touch;

const I2C_ACCESS_ENTRY g_I2C_AccessTable[] = {
    // SlaveAddr,        Offset,     Length, R/W
    {TOUCH_SLAVE_ADDR,   eWD608_RESULT,   1, I2C_RW_READ,  &ThermostatData.TouchResult,  NULL},  // Touch result
    {TOUCH_SLAVE_ADDR,   eWD608_CURSOR_L, 1, I2C_RW_READ,  &ThermostatData.TouchCursorL, NULL},  // Cursor L
    {TOUCH_SLAVE_ADDR,   eWD608_CURSOR_H, 1, I2C_RW_READ,  &ThermostatData.TouchCursorH, NULL},  // Cursor H
    {TOUCH_SLAVE_ADDR,   eWD608_VECTOR,   1, I2C_RW_READ,  &ThermostatData.TouchVector,  NULL},  // Vector
    //{EEPROM_SLAVE_ADDR,              0,   2, I2C_RW_READ,  &ThermostatData.EepromRxData, ThermostatData.EepromTxData},  // 24LC64 EEPROM
    //{EEPROM_SLAVE_ADDR,              0,   3, I2C_RW_WRITE,                         NULL, ThermostatData.EepromTxData},  // 24LC64 EEPROM
};
const uint8_t g_I2C_AccessCount = sizeof(g_I2C_AccessTable) / sizeof(I2C_ACCESS_ENTRY);

/*---------------------------------------------------------------------------------------------------------*/
/* ISR to handle UART Channel 0 interrupt event                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{

  return;
}

/*---------------------------------------------------------------------------------------------------------*/
/*   TMR0_IRQHandler                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void TMR0_IRQHandler(void){
  int Key;
  OS_TimeMS++;
	
  if (TIMER_GetIntFlag(TIMER0)) {
#if GUI_SUPPORT_TOUCH
    if ( OS_TimeMS % 10 == 0 ) {
        if ( g_enable_Touch == 1 ) {
            GUI_TOUCH_Exec();
        }
    }
#endif
    if ( (g_enable_Touch == 1) && (OS_TimeMS % 200 == 0) ) {
        if (PA0 == 0) {
            Key = GUI_KEY_TAB;
            GUI_StoreKeyMsg(Key, 1);
        }
        if ((PA1 == 0) || (PG3 == 0)) {
            Key = GUI_KEY_ENTER;
            GUI_StoreKeyMsg(Key, 1);
        }
        if (PG2 == 0) {
            Key = GUI_KEY_UP;
            GUI_StoreKeyMsg(Key, 1);
        }
        if (PC10 == 0) {
            Key = GUI_KEY_DOWN;
            GUI_StoreKeyMsg(Key, 1);
        }
        if (PC9 == 0) {
            Key = GUI_KEY_LEFT;
            GUI_StoreKeyMsg(Key, 1);
        }
        if (PG4 == 0) {
            Key = GUI_KEY_RIGHT;
            GUI_StoreKeyMsg(Key, 1);
        }
    }
    TIMER_ClearIntFlag(TIMER0);   /* Clear Timer0 time-out interrupt flag */
  }

  return;
}

/*---------------------------------------------------------------------------------------------------------*/
/*   TMR1_IRQHandler                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void TMR1_IRQHandler(void) {
  TIMER_CTRL_STRC *ptr = TimerCtrlPtr;
	
  if (TIMER_GetIntFlag(TIMER1)){

    if (ptr->FMC_STATE_TIMER < 0xFFFFFFF0)
        ptr->FMC_STATE_TIMER++;

    if (ptr->GUI_EXEC_TIMER < 0xFFFFFFF0)
        ptr->GUI_EXEC_TIMER++;

    if (ptr->TOUCH_PAD_TIMER < 0xFFFFFFF0)
        ptr->TOUCH_PAD_TIMER++;

    if (ptr->GUI_ROTARY_TIMER < 0xFFFFFFF0)
        ptr->GUI_ROTARY_TIMER++;

    if (ptr->I2C_TOUCH_TIMER < 0xFFFFFFF0)
        ptr->I2C_TOUCH_TIMER++;

    if (ptr->EEPROM_READ_TIMER < 0xFFFFFFF0)
        ptr->EEPROM_READ_TIMER++;

    if (ptr->EEPROM_WRITE_TIMER < 0xFFFFFFF0)
        ptr->EEPROM_WRITE_TIMER++;

       TIMER_ClearIntFlag(TIMER1);   /* Clear Timer1 time-out interrupt flag */
  }

  return;
}

/*----------------------------------------------------------------------------
  TIMER3_IRQHandler handler
 *----------------------------------------------------------------------------*/
void TMR3_IRQHandler (void){
  
  if (TIMER_GetIntFlag(TIMER3)) {

       TIMER_ClearIntFlag(TIMER3);   /* Clear Timer3 time-out interrupt flag */
  }

  return;
}

/**********************************************************************************************
 *  Generic I2C Interrupt Handler
 **********************************************************************************************/
static void I2C_IRQHandler_Common(I2C_CONTEXT *ctx) {
  uint32_t Status = I2C_GET_STATUS(ctx->base);

  if (I2C_GET_TIMEOUT_FLAG(ctx->base)) {
      I2C_ClearTimeoutFlag(ctx->base);
      return;
  }

  if (ctx->handler != NULL) {
      ctx->handler(Status);
  }

  return;
}

/**********************************************************************************************
 *  I2C1 IRQ Wrapper
 **********************************************************************************************/
void I2C1_IRQHandler(void) {

  I2C_IRQHandler_Common(&I2C1_Ctx);

  return;
}

/**********************************************************************************************
 *  I2C2 IRQ Wrapper
 **********************************************************************************************/
void I2C2_IRQHandler(void) {

  I2C_IRQHandler_Common(&I2C2_Ctx);

  return;
}

/**********************************************************************************************
 *  I2cReadByte
 **********************************************************************************************/
static void I2cReadByte (uint32_t Status) {
  I2C_CONTEXT *ctx = &I2C2_Ctx;
  I2C_T *i2c = ctx->base;

  //printf("I2cReadByte - Status: 0x%x\n", Status);
  switch (Status) {
      case 0x08:  /* START has been transmitted and prepare SLA+W */
          I2C_SET_DATA(i2c, (ctx->slaveAddr << 1));
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          break;
      case 0x18:  /* SLA+W has been transmitted and ACK has been received */
          I2C_SET_DATA(i2c, ctx->txBuf[ctx->byteCount++]);
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          break;
      case 0x20:  /* SLA+W has been transmitted and NACK has been received */
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_STO | I2C_CTL_SI);
          break;
      case 0x28:  /* DATA has been transmitted and ACK has been received */
          if (ctx->byteCount < ctx->length) {
              I2C_SET_DATA(i2c, ctx->txBuf[ctx->byteCount++]);
              I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          } else {
              I2C_SET_CONTROL_REG(i2c, I2C_CTL_STA | I2C_CTL_SI);
          }
          break;
      case 0x10: /* Repeat START has been transmitted and prepare SLA+R */
          I2C_SET_DATA(i2c, (ctx->slaveAddr << 1) | 0x01);
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          break;
      case 0x40:  /* SLA+R has been transmitted and ACK has been received */
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          break;
      case 0x58:  /* DATA has been received and NACK has been returned */
          *ctx->rxData = I2C_GET_DATA(i2c);
          ctx->txrxDone = TRUE;
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_STO | I2C_CTL_SI);
          break;
      default:
          printf("Status 0x%x is NOT processed\n", Status);
          break;
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  I2c2WriteByte Callback Function                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
void I2c2WriteByte (uint32_t Status) {
  I2C_CONTEXT *ctx = &I2C2_Ctx;
  I2C_T *i2c = ctx->base;

  //printf("I2c2WriteByte - Status: 0x%x\n", Status);
  switch (Status) {
      case 0x08:  /* START has been transmitted */
          I2C_SET_DATA(i2c, (ctx->slaveAddr << 1));  /* Write SLA+W to Register I2CDAT */
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          break;
      case 0x18:  /* SLA+W has been transmitted and ACK has been received */
          I2C_SET_DATA(i2c, ctx->txBuf[ctx->byteCount++]);
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          break;
      case 0x20:  /* SLA+W has been transmitted and NACK has been received */
          I2C_SET_CONTROL_REG(i2c, I2C_CTL_STA | I2C_CTL_STO | I2C_CTL_SI);
          break;
      case 0x28:
          if (ctx->byteCount < ctx->length) {
            I2C_SET_DATA(i2c, ctx->txBuf[ctx->byteCount++]);
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_SI);
          } else {
            I2C_SET_CONTROL_REG(i2c, I2C_CTL_STO | I2C_CTL_SI);
            ctx->txrxDone = TRUE;
          }
          break;
      default:  /* TO DO */
          printf("Status 0x%x is NOT processed\n", Status);
          break;
  }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  ReadingTouchSensor Function                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
void ReadingTouchSensor (I2C_CONTEXT *ctx, const I2C_ACCESS_ENTRY *entry) {

  /* I2C function to read data from slave */
  ctx->handler = (entry->Rw) ? (I2C_FUNC)I2cReadByte : I2c2WriteByte;

  if (ctx->handler != NULL) {
      ctx->slaveAddr = entry->SlaveAddr;
      if (entry->SlaveAddr == EEPROM_SLAVE_ADDR) {
          ctx->txBuf[0]  = entry->TxData[1];
          ctx->txBuf[1]  = entry->TxData[0];
          ctx->txBuf[2]  = entry->TxData[2];
      } else {
          ctx->txBuf[0]  = entry->Offset;
      }
      ctx->length    = entry->Length;
      ctx->byteCount = 0;
      ctx->rxData    = entry->RxData;
      ctx->txrxDone  = FALSE;

      /* I2C as master sends START signal */
      I2C_SET_CONTROL_REG(ctx->base, I2C_CTL_STA);
  }

  return;
}

/*---------------------------------------------------------------------------------------------------------*/
/*  EepromReadByte Callback Function                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
void EepromReadByte(uint32_t u32Status)
{
    //printf("I2C2 - u32Status: 0x%x\n", u32Status);
    if (u32Status == 0x08)                      /* START has been transmitted and prepare SLA+W */
    {
        //printf("I2C2 - W - Slave Addr: 0x%x\n", g_u8DeviceAddr);
        I2C_SET_DATA(I2C2, (g_u8DeviceAddr << 1)); /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    }
    else if (u32Status == 0x18)                 /* SLA+W has been transmitted and ACK has been received */
    {
        //printf("I2C2 - TxData[%d]: 0x%x\n", g_u8DataLen, g_au8TxData[g_u8DataLen]);
        I2C_SET_DATA(I2C2, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    }
    else if (u32Status == 0x20)                 /* SLA+W has been transmitted and NACK has been received */
    {
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_STO | I2C_CTL_SI);
    }
    else if (u32Status == 0x28)                 /* DATA has been transmitted and ACK has been received */
    {
        if (g_u8DataLen < 2)
        {
            I2C_SET_DATA(I2C2, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C2, I2C_CTL_STA | I2C_CTL_SI);
        }
    }
    else if (u32Status == 0x10)                 /* Repeat START has been transmitted and prepare SLA+R */
    {
        //printf("I2C2 - R - Slave Addr: 0x%x\n", g_u8DeviceAddr);
        I2C_SET_DATA(I2C2, (g_u8DeviceAddr << 1) | 0x01);  /* Write SLA+R to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    }
    else if (u32Status == 0x40)                 /* SLA+R has been transmitted and ACK has been received */
    {
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    }
    else if (u32Status == 0x58)                 /* DATA has been received and NACK has been returned */
    {
        g_u8RxData = I2C_GET_DATA(I2C2);
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_STO | I2C_CTL_SI);
        g_u8EndFlag = 1;
        //printf("I2C2 - R - RxData: 0x%x\n", g_u8RxData);
    }
    else
    {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  EepromWriteByte Callback Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void EepromWriteByte(uint32_t u32Status)
{
    if (u32Status == 0x08)                      /* START has been transmitted */
    {
        I2C_SET_DATA(I2C2, g_u8DeviceAddr << 1);  /* Write SLA+W to Register I2CDAT */
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    }
    else if (u32Status == 0x18)                 /* SLA+W has been transmitted and ACK has been received */
    {
        I2C_SET_DATA(I2C2, g_au8TxData[g_u8DataLen++]);
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
    }
    else if (u32Status == 0x20)                 /* SLA+W has been transmitted and NACK has been received */
    {
        I2C_SET_CONTROL_REG(I2C2, I2C_CTL_STA | I2C_CTL_STO | I2C_CTL_SI);
    }
    else if (u32Status == 0x28)                 /* DATA has been transmitted and ACK has been received */
    {
        if (g_u8DataLen != 3)
        {
            I2C_SET_DATA(I2C2, g_au8TxData[g_u8DataLen++]);
            I2C_SET_CONTROL_REG(I2C2, I2C_CTL_SI);
        }
        else
        {
            I2C_SET_CONTROL_REG(I2C2, I2C_CTL_STO | I2C_CTL_SI);
            g_u8EndFlag = 1;
        }
    }
    else
    {
        /* TO DO */
        printf("Status 0x%x is NOT processed\n", u32Status);
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*  EepromRwSetting Function                                                                               */
/*---------------------------------------------------------------------------------------------------------*/
void RwEepromDataByte (uint32_t Addr, uint8_t Data, uint8_t Rw) {

  if (Rw) {
      /* I2C function to read data from slave */
      s_I2C2HandlerFn = (I2C_FUNC)EepromReadByte;
  } else {
      /* I2C function to write data from slave */
      s_I2C2HandlerFn = (I2C_FUNC)EepromWriteByte;
      g_au8TxData[2] = Data;
  }

  g_au8TxData[0] = (uint8_t)((Addr & 0xFF00) >> 8);
  g_au8TxData[1] = (uint8_t)(Addr & 0x00FF);

  g_u8DeviceAddr = 0x10;
  g_u8DataLen = 0;
  g_u8EndFlag = 0;

  /* I2C as master sends START signal */
  I2C_SET_CONTROL_REG(I2C2, I2C_CTL_STA);

  return;
}

/*---------------------------------------------------------------------------------------------------------*/
/*   EINT0_IRQHandler                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void EINT0_IRQHandler(void){

  return;
}

/*---------------------------------------------------------------------------------------------------------*/
/*   GPE_IRQHandler                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void GPE_IRQHandler (void) {
	
  return;
}

/*----------------------------------------------------------------------------
  SysClock_Initialize handler
 *----------------------------------------------------------------------------*/
void SysClock_Initialize (void) {

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable HXT */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(FREQ_192MHZ);

    /* Set SysTick source to HCLK/2 */
    CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLKSEL_HCLK_DIV2);

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    //CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV1 | CLK_PCLKDIV_APB1DIV_DIV1;
 
    /* Enable peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(EBI_MODULE);
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_EnableModuleClock(TMR1_MODULE);
    CLK_EnableModuleClock(TMR3_MODULE);
    /* Enable I2C1/I2C2 clock */
    CLK_EnableModuleClock(I2C1_MODULE);
    CLK_EnableModuleClock(I2C2_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HXT, 0);
    CLK_SetModuleClock(TMR3_MODULE, CLK_CLKSEL1_TMR3SEL_HXT, 0);

    CLK->AHBCLK |= (1 << 14);  // 128k~160k for SPIM
    outpw(0x40007004,0x16);  // control SPIM can write

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();

    /* Lock protected registers */
    SYS_LockReg();

  return;
}

/*----------------------------------------------------------------------------
  Uart_Initialize handler
 *----------------------------------------------------------------------------*/
void Uart_Initialize (void) {

  /* Reset IP */
  SYS_ResetModule(UART0_RST);
 
  /* Configure UART0 and set UART0 Baudrate */
  UART_Open(UART0, 115200);

  UART_SetTimeoutCnt(UART0, 0x10); // Set Rx Time-out counter

  // Set RX FIFO Interrupt Trigger Level
  //UART0->FIFO &= ~ UART_FIFO_RFITL_Msk;
  //UART0->FIFO |= UART_FIFO_RFITL_4BYTES;

  /* Enable UART RDA/THRE/Time-out interrupt */
  //NVIC_EnableIRQ(UART0_IRQn);

  //UART_EnableInt(UART0, (UART_INTEN_RDAIEN_Msk | UART_INTEN_THREIEN_Msk | UART_INTEN_RXTOIEN_Msk));
  //UART_EnableInt(UART0, (UART_INTEN_RDAIEN_Msk | UART_INTEN_RXTOIEN_Msk));

  UART_MSG("\n\nCPU @ %d Hz\n", SystemCoreClock);

  return;
}

/*----------------------------------------------------------------------------
  Gpio_Initialize handler
 *----------------------------------------------------------------------------*/
void Gpio_Initialize (void) {

  /*---------------------------------------------------------------------------------------------------------*/
  /* Init I/O Multi-function                                                                                 */
  /*---------------------------------------------------------------------------------------------------------*/

  /*=== UART mult-function pins ===*/
  /* Set PB.12 and PB.13 multi-function pins for UART0 RXD, TXD */
  SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
  SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

  /* Set PB.0 and PB.1 multi-function pins for I2C1 SCL/SDA */
  SYS->GPB_MFPL &= ~(SYS_GPB_MFPL_PB0MFP_Msk | SYS_GPB_MFPL_PB1MFP_Msk);
  SYS->GPB_MFPL |= (SYS_GPB_MFPL_PB0MFP_I2C1_SDA | SYS_GPB_MFPL_PB1MFP_I2C1_SCL);

  /* Set I2C2 multi-function pins */
  SYS->GPD_MFPL = (SYS->GPD_MFPL & ~(SYS_GPD_MFPL_PD0MFP_Msk | SYS_GPD_MFPL_PD1MFP_Msk)) |
                  (SYS_GPD_MFPL_PD0MFP_I2C2_SDA | SYS_GPD_MFPL_PD1MFP_I2C2_SCL);

  /* I2C pin enable schmitt trigger */
  PD->SMTEN |= GPIO_SMTEN_SMTEN0_Msk | GPIO_SMTEN_SMTEN1_Msk;

  GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT);   // SW1 Key Button
  GPIO_SetMode(PA, BIT1, GPIO_MODE_INPUT);   // SW2 Key Button
  GPIO_SetMode(PG, BIT2, GPIO_MODE_INPUT);   // Up (Joystick)
  GPIO_SetMode(PG, BIT3, GPIO_MODE_INPUT);   // Center (Joystick)
  GPIO_SetMode(PG, BIT4, GPIO_MODE_INPUT);   // Right (Joystick)
  GPIO_SetMode(PC, BIT9, GPIO_MODE_INPUT);   // Left (Joystick)
  GPIO_SetMode(PC, BIT10, GPIO_MODE_INPUT);  // Down (Joystick)
  GPIO_SetMode(PD, BIT10, GPIO_MODE_INPUT);  // Touch INT

  return;
}

/*----------------------------------------------------------------------------
  Timer0_Initialize handler
 *----------------------------------------------------------------------------*/
void Timer0_Initialize (void) {

    /* Open Timer0 in periodic mode, enable interrupt and 1000 interrupt tick per second */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER0);
  
    /* Enable Timer0 NVIC */
    NVIC_SetPriority(TMR0_IRQn, 1);
    NVIC_EnableIRQ(TMR0_IRQn);
  
    /* Start Timer0 counting */
    TIMER_Start(TIMER0);
  
    return;
}

/*----------------------------------------------------------------------------
  Timer1_Initialize handler
 *----------------------------------------------------------------------------*/
void Timer1_Initialize (void) {  

    /* Open Timer1 in periodic mode, enable interrupt and 10000 interrupt tick per second */
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER1);

    /* Enable Timer1 NVIC */
    NVIC_SetPriority(TMR1_IRQn, 1);
    NVIC_EnableIRQ(TMR1_IRQn);

    /* Start Timer1 counting */
    TIMER_Start(TIMER1);
  
    return;
}

/*----------------------------------------------------------------------------
  Timer3_Initialize handler
 *----------------------------------------------------------------------------*/
void Timer3_Initialize (void) {  

  /* Open Timer3 in periodic mode, enable interrupt and 10000 interrupt tick per second */
  TIMER_Open(TIMER3, TIMER_PERIODIC_MODE, 10000);
  TIMER_EnableInt(TIMER3);

  /* Enable Timer1 NVIC */
  NVIC_SetPriority(TMR3_IRQn, 1);
  NVIC_EnableIRQ(TMR3_IRQn);

  /* Start Timer1 counting */
  TIMER_Start(TIMER3);
  
  return;
}

/*----------------------------------------------------------------------------
  I2C1_Initialize handler
 *----------------------------------------------------------------------------*/
void I2C1_Initialize (void) {

  /* Reset I2C1 */
  SYS_ResetModule(I2C1_RST);

  /* Open I2C1 and set clock to 100k */
  I2C_Open(I2C1, 100000);

  /* Get I2C1 Bus Clock */
  printf("I2C1 clock %d Hz\n", I2C_GetBusClockFreq(I2C1));

  I2C_EnableInt(I2C1);
  NVIC_EnableIRQ(I2C1_IRQn);
}

/*----------------------------------------------------------------------------
  I2C2_Initialize handler
 *----------------------------------------------------------------------------*/
void I2C2_Initialize (void) {

  /* Reset I2C2 */
  SYS_ResetModule(I2C2_RST);

  /* Open I2C2 and set clock to 100k */
  I2C_Open(I2C2, 100000);

  /* Get I2C2 Bus Clock */
  printf("I2C2 clock %d Hz\n", I2C_GetBusClockFreq(I2C2));

  I2C_EnableInt(I2C2);
  NVIC_EnableIRQ(I2C2_IRQn);
}

/*----------------------------------------------------------------------------
  Platform_Initialize handler
 *----------------------------------------------------------------------------*/
void Platform_Initialize (void) {

#if defined(NUCONSOLE_SUPPORT) && (NUCONSOLE_SUPPORT == 1)
  NuConsole_Init ();
#endif

  SysClock_Initialize ();

  Uart_Initialize ();

  Gpio_Initialize ();
	 
  Timer0_Initialize ();
	 
  Timer1_Initialize ();

  Timer3_Initialize ();

  I2C1_Initialize ();

  I2C2_Initialize ();
  
  g_enable_Touch = 0;

#if GUI_SUPPORT_TOUCH
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();
    WM_MULTIBUF_Enable(1);

    Init_TouchPanel();
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable FMC ISP function */
    FMC_Open();

    /* SPI flash 256KB + 0x1C marker address */
    if (FMC_Read(__DEMO_TSFILE_ADDR__ + 0x1C) != 0x55AAA55A)
    //if ( 1 )
    {
        FMC_ENABLE_AP_UPDATE();
        ts_calibrate(__DEMO_TS_WIDTH__, __DEMO_TS_HEIGHT__);
        // Erase page
        FMC_Erase(__DEMO_TSFILE_ADDR__);
        ts_writefile();
        FMC_DISABLE_AP_UPDATE();
    }
    else
    {
        ts_readfile();
    }

    /* Disable FMC ISP function */
    FMC_Close();

    /* Lock protected registers */
    SYS_LockReg();
#endif

  g_enable_Touch = 1;

  return;
}

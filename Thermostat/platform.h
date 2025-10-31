/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#define UART_DEBUG                  1
#define TMR3_DEBUG                  0

#if defined(UART_DEBUG) && (UART_DEBUG == 1)
#define UART_MSG                    printf
#else
#define UART_MSG                    1?0:printf
#endif

#if defined(TMR3_DEBUG) && (TMR3_DEBUG == 1)
#define TMR3_MSG                    printf
#else
#define TMR3_MSG                    1?0:printf
#endif

#define TOUCH_SLAVE_ADDR            0x10
#define TOUCH_INT                   PD10

#define KEY_TOUCH                   PA1
#define KEY_ENTER                   PG3
#define KEY_LEFT                    PC9
#define KEY_UP                      PG2
#define KEY_RIGHT                   PG4
#define KEY_DOWN                    PC10

typedef void (*I2C_FUNC)(uint32_t Status);

/**********************************************************************************************
 *  Unified I2C Data Structure
 **********************************************************************************************/
typedef struct {
    I2C_T        *base;          // I2C peripheral instance (e.g., I2C1, I2C2)
    I2C_FUNC      handler;       // Callback function pointer for interrupt handling
    uint8_t       slaveAddr;     // Slave device address
    uint8_t       txBuf[3];      // Transmit buffer
    uint8_t       rxData;        // Received data byte
    volatile uint8_t byteCount;  // Byte count
    volatile uint8_t length;     // Current transmit data length
    volatile uint8_t txrxDone;   // Transaction complete flag
} I2C_CONTEXT;

extern I2C_CONTEXT I2C1_Ctx, I2C2_Ctx;

extern uint8_t I2c1_DeviceAddr;
extern uint8_t I2c1_TxData[2];
extern uint8_t I2c1_RxData;
extern volatile uint8_t I2c1_DataLen;
extern volatile uint8_t I2c1_EndFlag;

extern uint8_t g_u8DeviceAddr;
extern uint8_t g_au8TxData[];
extern uint8_t g_u8RxData;
extern volatile uint8_t g_u8DataLen;
extern volatile uint8_t g_u8EndFlag;

void ReadingTouchSensor (I2C_CONTEXT *ctx, uint8_t SlaveAddr, uint8_t Command, uint8_t Lenght, uint8_t Rw);
void RwI2cTouchSensor (uint8_t Command, uint8_t Rw);
void RwEepromDataByte (uint32_t Addr, uint8_t Data, uint8_t Rw);
void Platform_Initialize (void);

#endif // _PLATFORM_H_


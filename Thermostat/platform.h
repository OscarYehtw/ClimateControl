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

#define EEPROM_SLAVE_ADDR           0x50

#define TOUCH_SLAVE_ADDR            0x10
#define TOUCH_INT                   PD10

#define eWD608_RESULT               0xC0   // Theresult of the tray
#define eWD608_CURSOR_L             0xC1   // The cursor of the tray
#define eWD608_CURSOR_H             0xC2   // The cursor of the tray
#define eWD608_VECTOR               0xC3   // The vector of the tray, Forward：0x01 Reverse：0xFF
#define eWD608_FORWARD              0x01   // Forward：0x01 
#define eWD608_REVERSE              0xFF   // Reverse：0xFF

#define TOUCH_EVENT                 BIT7   // Bit 7(Gesture):Touch Status. If this bit is “1”, It represent to touch event happened.
#define NEW_CURSOR_EVENT            BIT6   // Bit 6(NewCursor):New Cursor Status. If this bit is “1”, It represent to new Cursor event happened.
#define CURSOR_0_EVENT              BIT0   // Bit 0 (Cur0): Cursor 0 status. If this bit is “1”，It represent to cursor 0 is touched. Valid if Touch Status is “1”

#define KEY_TOUCH                   PA1
#define KEY_ENTER                   PG3
#define KEY_LEFT                    PC9
#define KEY_UP                      PG2
#define KEY_RIGHT                   PG4
#define KEY_DOWN                    PC10

typedef enum {
    I2C_RW_READ  = 0x01,
    I2C_RW_WRITE = 0x00
} I2C_RW_t;

typedef struct {
    uint8_t   SlaveAddr;     // I2C Slave Address
    uint8_t   Offset;        // Register offset (Command)
    uint8_t   Length;        // Data length to read/write
    I2C_RW_t  Rw;            // Read / Write attribute
    uint8_t   *RxData;
    uint8_t   *TxData;
} I2C_ACCESS_ENTRY;

typedef void (*I2C_FUNC)(uint32_t Status);

/**********************************************************************************************
 *  Unified I2C Data Structure
 **********************************************************************************************/
typedef struct {
    I2C_T        *base;          // I2C peripheral instance (e.g., I2C1, I2C2)
    I2C_FUNC      handler;       // Callback function pointer for interrupt handling
    uint8_t       slaveAddr;     // Slave device address
    uint8_t       txBuf[3];      // Transmit buffer
    uint8_t       *rxData;       // Received data byte
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

extern const I2C_ACCESS_ENTRY g_I2C_AccessTable[];
extern const uint8_t g_I2C_AccessCount;

//void ReadingTouchSensor (I2C_CONTEXT *ctx, uint8_t SlaveAddr, uint8_t Command, uint8_t Lenght, uint8_t Rw);
void ReadingTouchSensor (I2C_CONTEXT *ctx, const I2C_ACCESS_ENTRY *entry);
void RwI2cTouchSensor (uint8_t Command, uint8_t Rw);
void RwEepromDataByte (uint32_t Addr, uint8_t Data, uint8_t Rw);
void Platform_Initialize (void);

#endif // _PLATFORM_H_


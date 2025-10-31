/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef _GAUGE_H_
#define _GAUGE_H_

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#define FMC_STATE_DEBUG               0
#define GUIEXEC_STATE_DEBUG           0
#define TOUCH_PAD_STATE_DEBUG         0
#define GUI_ROTARY_STATE_DEBUG        1
#define JOYSTICK_STATE_DEBUG          0
#define EEPROM_READ_STATE_DEBUG       0
#define EEPROM_WRITE_STATE_DEBUG      0

#if defined(FMC_STATE_DEBUG) && (FMC_STATE_DEBUG == 1)
#define FMC_STATE_MSG                 printf
#else
#define FMC_STATE_MSG                 1?0:printf
#endif

#if defined(GUIEXEC_STATE_DEBUG) && (GUIEXEC_STATE_DEBUG == 1)
#define GUIEXEC_STATE_MSG             printf
#else
#define GUIEXEC_STATE_MSG             1?0:printf
#endif

#if defined(TOUCH_PAD_STATE_DEBUG) && (TOUCH_PAD_STATE_DEBUG == 1)
#define TOUCH_PAD_STATE_MSG           printf
#else
#define TOUCH_PAD_STATE_MSG           1?0:printf
#endif

#if defined(GUI_ROTARY_STATE_DEBUG) && (GUI_ROTARY_STATE_DEBUG == 1)
#define GUI_ROTARY_STATE_MSG          printf
#else
#define GUI_ROTARY_STATE_MSG          1?0:printf
#endif

#if defined(JOYSTICK_STATE_DEBUG) && (JOYSTICK_STATE_DEBUG == 1)
#define JOYSTICK_STATE_MSG            printf
#else
#define JOYSTICK_STATE_MSG            1?0:printf
#endif

#if defined(EEPROM_READ_STATE_DEBUG) && (EEPROM_READ_STATE_DEBUG == 1)
#define EEPROM_READ_STATE_MSG         printf
#else
#define EEPROM_READ_STATE_MSG         1?0:printf
#endif

#if defined(EEPROM_WRITE_STATE_DEBUG) && (EEPROM_WRITE_STATE_DEBUG == 1)
#define EEPROM_WRITE_STATE_MSG        printf
#else
#define EEPROM_WRITE_STATE_MSG        1?0:printf
#endif

#define DEFAULT_BRIGHTNESS            80

#define eWD608_RESULT                 0xC0   // Theresult of the tray
#define eWD608_CURSOR_L               0xC1   // The cursor of the tray
#define eWD608_CURSOR_H               0xC2   // The cursor of the tray
#define eWD608_VECTOR                 0xC3   // The vector of the tray, Forward：0x01 Reverse：0xFF
#define eWD608_FORWARD                0x01   // Forward：0x01 
#define eWD608_REVERSE                0xFF   // Reverse：0xFF

#define TOUCH_EVENT                   BIT7   // Bit 7(Gesture):Touch Status. If this bit is “1”, It represent to touch event happened.
#define NEW_CURSOR_EVENT              BIT6   // Bit 6(NewCursor):New Cursor Status. If this bit is “1”, It represent to new Cursor event happened.
#define CURSOR_0_EVENT                BIT0   // Bit 0 (Cur0): Cursor 0 status. If this bit is “1”，It represent to cursor 0 is touched. Valid if Touch Status is “1”

typedef enum {
  CHANGE_STATE, 
  WAIT_CMD_STATE, 
  INIT_STATE, 
  HOLD_STATE,
  ROTARY_CLICK_STATE,
  ROTARY_CLICKED_HOLD_STATE,
  ROTARY_SET_VALUE_STATE,
  ROTARY_SET_VALUE_HOLD_STATE,
  ROTARY_RELEASE_STATE,
  DONE
} CTRL_STATE;

typedef struct {
  uint32_t             Brightness;
  uint8_t              TouchRawData0;
  uint8_t              TouchRawData1;

  union {
      uint32_t Misc;
      struct {
           uint32_t GUI_INIT_READY:1;
           uint32_t GUI_READY:1;
           uint32_t TOUCH_PRESSED:1;
           uint32_t TOUCH_RELEASED:1;
           uint32_t TOUCH_FORWARD:1;
           uint32_t TOUCH_REVERSE:1;
           uint32_t EEPROM_WRITE_DONE:1;
           uint32_t RESERVED:25;
      } MISC;
  };

} THERMOSTAT_DATA_STRC;

typedef struct {
  uint32_t             FMC_STATE_TIMER;
  uint32_t             GUI_EXEC_TIMER;
  uint32_t             TOUCH_PAD_TIMER;
  uint32_t             GUI_ROTARY_TIMER;
  uint32_t             I2C_TOUCH_TIMER;
  uint32_t             EEPROM_READ_TIMER;
  uint32_t             EEPROM_WRITE_TIMER;
} TIMER_CTRL_STRC;

typedef struct {
  uint32_t             NvramBrightness;
} FLASH_DATA_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Fmc_State;
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  FLASH_DATA_STRC      *FlashDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} FMC_CTRL_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Gui_State;
  uint32_t             GuiRefreshRate;
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} GUIEXEC_CTRL_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Touch_State;
  uint8_t              Offset;
  I2C_CONTEXT          *ctx;
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} TOUCH_PAD_CTRL_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Rotary_State;
  int32_t              target;
  int32_t              rotaryVal;

  uint8_t              pressed;
  uint8_t              last_pressed;
  int32_t              last_rotaryVal;
  
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} GUI_ROTARY_CTRL_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Joystick_State;
  uint8_t              last_pressed;
  uint8_t              last_up;
  uint8_t              last_down;
  int32_t              last_val;
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} JOYSTICK_CTRL_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Read_State;
  uint32_t             Offset;
  uint8_t              *DevAddr;
  uint8_t              *TxDataBuff;
  uint8_t              *RxData;
  volatile uint8_t     *DataLen;
  volatile uint8_t     *DataReady;
  I2C_CONTEXT          *ctx;
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} EEPROM_READ_CTRL_STRC;

typedef struct {
  uint8_t              Valid;
  uint8_t              Write_State;
  uint32_t             Offset;
  uint8_t              Dummy;
  uint8_t              *DevAddr;
  uint8_t              *TxDataBuff;
  uint8_t              *RxData;
  volatile uint8_t     *DataLen;
  volatile uint8_t     *DataReady;
  THERMOSTAT_DATA_STRC *ThermostatDataPtr;
  TIMER_CTRL_STRC      *TimerPtr;
} EEPROM_WRITE_CTRL_STRC;

extern THERMOSTAT_DATA_STRC ThermostatData, *ThermostatDataPtr;
extern TIMER_CTRL_STRC      TimerCtrl, *TimerCtrlPtr;

void MainTask (void);

#endif // _GAUGE_H_


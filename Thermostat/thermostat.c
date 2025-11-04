/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/   
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "NuMicro.h"
#include "platform.h"
#include "thermostat.h"

#include "GUI.h"
#include "WM.h"
#include "FRAMEWIN.h"
#include "LCDConf.h"
#include "M48XTouchPanel.h"
#include "Resource.h"

/* Private typedef -----------------------------------------------------------*/   

/* Private define ------------------------------------------------------------*/   

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
THERMOSTAT_DATA_STRC    ThermostatData  = {0}, *ThermostatDataPtr  = &ThermostatData;
FLASH_DATA_STRC         FlashData       = {0}, *FlashDataPtr       = &FlashData;
TIMER_CTRL_STRC         TimerCtrl       = {0}, *TimerCtrlPtr       = &TimerCtrl;

FMC_CTRL_STRC           FmcCtrl         = {0}, *FmcCtrlPtr         = &FmcCtrl;
GUIEXEC_CTRL_STRC       GuiCtrl         = {0}, *GuiCtrlPtr         = &GuiCtrl;
TOUCH_PAD_CTRL_STRC     TouchPadCtrl    = {0}, *TouchPadCtrlPtr    = &TouchPadCtrl;
GUI_ROTARY_CTRL_STRC    RotaryCtrl      = {0}, *RotaryCtrlPtr      = &RotaryCtrl;
JOYSTICK_CTRL_STRC      JoystickCtrl    = {0}, *JoystickCtrlPtr    = &JoystickCtrl;
EEPROM_READ_CTRL_STRC   EepromRead      = {0}, *EepromReadPtr      = &EepromRead;
EEPROM_WRITE_CTRL_STRC  EepromWrite     = {0}, *EepromWritePtr     = &EepromWrite;

/*----------------------------------------------------------------------------
  DataFlashRead handler
  NOTE:
       Data Flash is shared with APROM and size is configurable. 
       The base address of Data Flash is determined by DFBA (CONFIG1[19:0]).
       All of embedded flash memory is 2KB page erased.
 *----------------------------------------------------------------------------*/
uint8_t DataFlashRead (FMC_CTRL_STRC *Ctrl) {
  uint32_t   u32DataFlashBase = NULL;
  uint32_t   *DataPtr;	
  uint32_t   i;

  DataPtr = (uint32_t *)Ctrl->FlashDataPtr;

  SYS_UnlockReg();
  FMC_Open();
  
  /* Read Data Flash base address */
  u32DataFlashBase = FMC_ReadDataFlashBaseAddr();

  FMC_STATE_MSG("FLASH DATA Base Address: %x\n", u32DataFlashBase);

  //Build-in 40KB flash ROM
  if (u32DataFlashBase == NULL || u32DataFlashBase >= 0x0A000) {
      //Data flash was no longer available, restore default setting
      Ctrl->FlashDataPtr->NvramBrightness = 0xFFFFFFFF;
      return FALSE;
  }

  for (i=u32DataFlashBase; \
       i<(u32DataFlashBase + sizeof(FLASH_DATA_STRC)); \
       i+=4, DataPtr++) {

       *DataPtr = FMC_Read(i);
	   
	   FMC_STATE_MSG("OFFSET: %x DATA: %x \n", i, *DataPtr);
  }

  FMC_Close();
  SYS_LockReg();
  
  return TRUE;
}

/*----------------------------------------------------------------------------
  DataFlashWrite handler
  NOTE:
       Data Flash is shared with APROM and size is configurable. 
       The base address of Data Flash is determined by DFBA (CONFIG1[19:0]).
       All of embedded flash memory is 2KB page erased.
 *----------------------------------------------------------------------------*/
void DataFlashWrite (FMC_CTRL_STRC *Ctrl) {
  uint32_t   u32DataFlashBase;
  uint32_t   *DataPtr;  
  uint32_t   i;

  DataPtr = (uint32_t *)Ctrl->FlashDataPtr;

  SYS_UnlockReg();
  FMC_Open();

  /* Read Data Flash base address */
  u32DataFlashBase = FMC_ReadDataFlashBaseAddr();

  FMC_STATE_MSG("DataFlashWrite - Base Addr: %x\n", u32DataFlashBase);
  
  //Data flash was no longer available
  if (u32DataFlashBase == NULL || u32DataFlashBase >= 0x0A000)
      return;

  FMC_Erase(u32DataFlashBase);

  for (i=0; i<sizeof(FLASH_DATA_STRC) / sizeof(uint32_t); i++) {
	   FMC_Write(u32DataFlashBase + i*4, DataPtr[i]);
  }

  FMC_Close();
  SYS_LockReg();

  return;
}

/*----------------------------------------------------------------------------
  FlashDataToVariable handler
 *----------------------------------------------------------------------------*/
void FlashDataToVariable (FMC_CTRL_STRC *Ctrl) {

  if (Ctrl->FlashDataPtr->NvramBrightness == 0xffffffff) {
      Ctrl->ThermostatDataPtr->Brightness = DEFAULT_BRIGHTNESS;
  } else {
      Ctrl->ThermostatDataPtr->Brightness = Ctrl->FlashDataPtr->NvramBrightness;
  }
  
  FMC_STATE_MSG("FlashDataToVariable - Brightness: %d\n", \
                Ctrl->ThermostatDataPtr->Brightness);

  return;
}

/*----------------------------------------------------------------------------
  VariableToFlashData handler
 *----------------------------------------------------------------------------*/
void VariableToFlashData (FMC_CTRL_STRC *Ctrl) {

  Ctrl->FlashDataPtr->NvramBrightness = Ctrl->ThermostatDataPtr->Brightness;

  FMC_STATE_MSG("VariableToFlashData - Brightness: %d\n", \
                 Ctrl->ThermostatDataPtr->Brightness);

  return;
}

/*----------------------------------------------------------------------------
  UpdateFlashData handler
 *----------------------------------------------------------------------------*/
uint8_t UpdateFlashData (FMC_CTRL_STRC *Ctrl) {
  uint8_t  Status = FALSE;

  if (Ctrl->FlashDataPtr->NvramBrightness != Ctrl->ThermostatDataPtr->Brightness) {
      Status = TRUE;
  }
  
  FMC_STATE_MSG("UpdateFlashData - Status: %x\n", Status);

  return Status;
}

/*----------------------------------------------------------------------------
  FmcStateInit handler
 *----------------------------------------------------------------------------*/
void FmcStateInit (FMC_CTRL_STRC *Ctrl) {
  Ctrl->Fmc_State = INIT_STATE;
  return;
}

/*----------------------------------------------------------------------------
  FmcStateTask handler
 *----------------------------------------------------------------------------*/
void FmcStateTask (FMC_CTRL_STRC *Ctrl) {

  if (Ctrl->Fmc_State == INIT_STATE   || \
      Ctrl->Fmc_State == CHANGE_STATE || \
      Ctrl->Fmc_State == HOLD_STATE   || \
      Ctrl->Valid) {
      return;
  }

  Ctrl->Valid = TRUE;

  return;
}

/*----------------------------------------------------------------------------
  FmcStateCtrl handler
 *----------------------------------------------------------------------------*/
void FmcStateCtrl (FMC_CTRL_STRC *Ctrl) {
  
  switch(Ctrl->Fmc_State) {
         case INIT_STATE:
              Ctrl->ThermostatDataPtr = ThermostatDataPtr;
              Ctrl->FlashDataPtr      = FlashDataPtr;
              Ctrl->TimerPtr          = TimerCtrlPtr;
              Ctrl->Valid             = 0;
              Ctrl->Fmc_State         = WAIT_CMD_STATE;
              if (DataFlashRead (Ctrl)) {
                  FlashDataToVariable(Ctrl);
              }
              break;
         case CHANGE_STATE:
              if (Ctrl->TimerPtr->FMC_STATE_TIMER >= 30000) {
                Ctrl->Fmc_State = WAIT_CMD_STATE;
              }
              break;
         case HOLD_STATE:
              Ctrl->Fmc_State = WAIT_CMD_STATE;
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  if (UpdateFlashData (Ctrl)) {
                      VariableToFlashData (Ctrl);
                      DataFlashWrite (Ctrl);
                      DataFlashRead (Ctrl);
                  }
                  Ctrl->TimerPtr->FMC_STATE_TIMER = 0;
                  Ctrl->Fmc_State = CHANGE_STATE;
                  Ctrl->Valid = FALSE;
                  FMC_STATE_MSG("FmcStateCtrl - Checking Flash Data!\n");
              }
			  break;
         default:
              break;
  }
  
  return;
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void APPW_X_FS_Init(void)
{
    /* TODO */
    ;
}

/*----------------------------------------------------------------------------
   GuiExecStateInit handler
 *----------------------------------------------------------------------------*/
void GuiExecStateInit (GUIEXEC_CTRL_STRC *Ctrl) {
  GUI_ALLOC_INFO Info;
  char  acVersion[40] = "Framewin: Version of emWin: ";

  /* Init of GUI internal data structures & variables */
  //
  // Setup configuration dependent pointers
  //
  APPW_X_Setup();
  //
  // Initialize AppWizard
  //
  APPW_Init(APPW_PROJECT_PATH);
  //
  // Create all persistent screens except initial screen
  //
  APPW_CreatePersistentScreens();
  //
  // Create initial screen...
  //
  APPW_CreateRoot(APPW_INITIAL_SCREEN, WM_HBKWIN);

  GUI_ALLOC_GetMemInfo(&Info);
  strcat(acVersion, GUI_GetVersionString());
  GUIEXEC_STATE_MSG("emWin Memory Info:\n");
  GUIEXEC_STATE_MSG("  Total Memory      : %lu bytes\n", Info.TotalBytes);
  GUIEXEC_STATE_MSG("  Used Bytes        : %lu bytes\n", Info.UsedBytes);
  GUIEXEC_STATE_MSG("  Free Bytes        : %lu bytes\n", Info.FreeBytes);
  GUIEXEC_STATE_MSG("  Bytes available   : %lu bytes\n", Info.AllocSize);
  GUIEXEC_STATE_MSG("  Fixed bytes       : %lu bytes\n", Info.NumFixedBytes);
  GUIEXEC_STATE_MSG("  Peak of used bytes: %lu bytes\n", Info.MaxUsedBytes);
  GUIEXEC_STATE_MSG("%s\n", acVersion);

  Ctrl->ThermostatDataPtr = ThermostatDataPtr;
  Ctrl->ThermostatDataPtr->MISC.GUI_INIT_READY = TRUE;
  Ctrl->ThermostatDataPtr->MISC.GUI_READY = TRUE;
  Ctrl->Gui_State = INIT_STATE;

  return;
}

/*----------------------------------------------------------------------------
   GuiExecStateTask handler
 *----------------------------------------------------------------------------*/
void GuiExecStateTask (GUIEXEC_CTRL_STRC *Ctrl) {
  
  if (Ctrl->Gui_State == INIT_STATE   || \
      Ctrl->Gui_State == CHANGE_STATE || \
  	  Ctrl->Gui_State == HOLD_STATE   || \
      Ctrl->Valid) {
      return;
  }
 
  //GUI_Exec1();
  APPW_Exec();
  //GUI_X_Delay(5);

  Ctrl->Valid = TRUE;

  #if 0
  GUIEXEC_STATE_MSG("GuiExecStateTask\n");
  #endif

  return;
}

/*----------------------------------------------------------------------------
   GuiExecStateCtrl handler
 *----------------------------------------------------------------------------*/
void GuiExecStateCtrl (GUIEXEC_CTRL_STRC *Ctrl) {

  switch(Ctrl->Gui_State) {
         case INIT_STATE:
              Ctrl->TimerPtr = TimerCtrlPtr;
              Ctrl->GuiRefreshRate = 1;
              Ctrl->Gui_State         = WAIT_CMD_STATE;
              Ctrl->Valid             = FALSE;
              break;
         case CHANGE_STATE:
              Ctrl->Gui_State = WAIT_CMD_STATE;
              break;
         case HOLD_STATE:
              // to execute GUI per 5ms
              if (Ctrl->TimerPtr->GUI_EXEC_TIMER >= Ctrl->GuiRefreshRate) {
                  Ctrl->Gui_State = WAIT_CMD_STATE;
                  //GUIEXEC_STATE_MSG("GuiExecStateCtrl - to execute GUI per 5ms\n");
              }
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  Ctrl->Gui_State = HOLD_STATE;
                  Ctrl->TimerPtr->GUI_EXEC_TIMER = 0;
                  Ctrl->Valid = FALSE;
              }
              break;
         default:
              break;		 	
  }
  
  return;
}

/*----------------------------------------------------------------------------
  TouchPadStateInit handler
 *----------------------------------------------------------------------------*/
void TouchPadStateInit (TOUCH_PAD_CTRL_STRC *Ctrl) {
  Ctrl->ThermostatDataPtr = ThermostatDataPtr;
  Ctrl->Touch_State = INIT_STATE;
  return;
}

/*----------------------------------------------------------------------------
  TouchPadStateTask handler
 *----------------------------------------------------------------------------*/
void TouchPadStateTask (TOUCH_PAD_CTRL_STRC *Ctrl) {
  if (Ctrl->Touch_State == INIT_STATE   || \
      Ctrl->Touch_State == CHANGE_STATE || \
      Ctrl->Touch_State == HOLD_STATE   || \
      Ctrl->Valid                       || \
      Ctrl->ThermostatDataPtr->MISC.GUI_INIT_READY == FALSE) {
      return;
  }

  ReadingTouchSensor (Ctrl->ctx, &Ctrl->entry[Ctrl->Index]);
  Ctrl->Valid = TRUE;
  TOUCH_PAD_STATE_MSG("ReadingTouchSensor, Index: %d\n", Ctrl->Index);

  return;
}

/*----------------------------------------------------------------------------
  TouchPadStateCtrl handler
 *----------------------------------------------------------------------------*/
void TouchPadStateCtrl (TOUCH_PAD_CTRL_STRC *Ctrl) {
  
  switch(Ctrl->Touch_State) {
         case INIT_STATE:
              Ctrl->TimerPtr = TimerCtrlPtr;
              Ctrl->Index    = 0;
              Ctrl->entry    = g_I2C_AccessTable;
              Ctrl->ctx      = &I2C2_Ctx;
              Ctrl->ThermostatDataPtr->EepromTxData[0] = 0x05;  // EEPROM Offset (low byte)
              Ctrl->ThermostatDataPtr->EepromTxData[1] = 0x00;  // EEPROM Offset (high byte)
              Ctrl->ThermostatDataPtr->EepromTxData[2] = 0xA5;  // EEPROM write data
              Ctrl->Valid = FALSE;
              Ctrl->Touch_State = WAIT_CMD_STATE;
              break;
         case CHANGE_STATE:
              if (Ctrl->TimerPtr->TOUCH_PAD_TIMER >= 50 && Ctrl->ctx->txrxDone == TRUE) {
                  if (Ctrl->entry[Ctrl->Index].SlaveAddr == TOUCH_SLAVE_ADDR) {
                      switch (Ctrl->entry[Ctrl->Index].Offset) {
                              case eWD608_RESULT:
                                   if (*Ctrl->entry[Ctrl->Index].RxData == (TOUCH_EVENT | NEW_CURSOR_EVENT | CURSOR_0_EVENT)) {
                                       Ctrl->ThermostatDataPtr->MISC.TOUCH_PRESSED = TRUE;
                                   } else {
                                       Ctrl->ThermostatDataPtr->MISC.TOUCH_PRESSED = FALSE;
                                   }
                                   Ctrl->ThermostatDataPtr->MISC.TOUCH_RESULT_AVAILABLE = TRUE;
                                   break;
                              case eWD608_CURSOR_L:
                                   Ctrl->ThermostatDataPtr->MISC.TOUCH_CURSOR_L_AVAILABLE = TRUE;
                                   break;
                              case eWD608_CURSOR_H:
                                   Ctrl->ThermostatDataPtr->MISC.TOUCH_CURSOR_H_AVAILABLE = TRUE;
                                   break;
                              case eWD608_VECTOR:
                                   if (*Ctrl->entry[Ctrl->Index].RxData == eWD608_FORWARD) {
                                       Ctrl->ThermostatDataPtr->MISC.TOUCH_FORWARD = TRUE;
                                       Ctrl->ThermostatDataPtr->MISC.TOUCH_REVERSE = FALSE;
                                   } else if (*Ctrl->entry[Ctrl->Index].RxData == eWD608_REVERSE) {
                                       Ctrl->ThermostatDataPtr->MISC.TOUCH_REVERSE = TRUE;
                                       Ctrl->ThermostatDataPtr->MISC.TOUCH_FORWARD = FALSE;
                                   }
                                   Ctrl->ThermostatDataPtr->MISC.TOUCH_VECTOR_AVAILABLE = TRUE;
                                   break;
                      }
                  }

                  if (Ctrl->entry[Ctrl->Index].Rw == I2C_RW_READ)
                      TOUCH_PAD_STATE_MSG("[%d] Offset: 0x%x Data: 0x%x\n", Ctrl->Index, Ctrl->entry[Ctrl->Index].Offset, *Ctrl->entry[Ctrl->Index].RxData);
                  else
                      TOUCH_PAD_STATE_MSG("[%d] Offset: 0x%x\n", Ctrl->Index, Ctrl->entry[Ctrl->Index].Offset);

                  Ctrl->Index = (++Ctrl->Index < g_I2C_AccessCount) ? Ctrl->Index : 0;
                  Ctrl->ctx->txrxDone = FALSE;
                  Ctrl->Touch_State = WAIT_CMD_STATE;
              }
              break;
         case HOLD_STATE:
              Ctrl->Touch_State = WAIT_CMD_STATE;
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  Ctrl->TimerPtr->TOUCH_PAD_TIMER = 0;
                  Ctrl->Touch_State = CHANGE_STATE;
                  Ctrl->Valid = FALSE;
              }
			  break;
         default:
              break;
  }
  
  return;
}

void APP_Target_Rotary_Clicked(void);
void APP_Target_Rotary_Released(void);
void APP_Target_Rotary_SetValue(I32 Value);
I32 APP_Target_Rotary_GetValue(void);

/*----------------------------------------------------------------------------
  RotaryStateInit handler
 *----------------------------------------------------------------------------*/
void RotaryStateInit (GUI_ROTARY_CTRL_STRC *Ctrl) {
  Ctrl->ThermostatDataPtr = ThermostatDataPtr;
  Ctrl->Rotary_State = INIT_STATE;
  return;
}

/*----------------------------------------------------------------------------
  RotaryStateTask handler
 *----------------------------------------------------------------------------*/
void RotaryStateTask (GUI_ROTARY_CTRL_STRC *Ctrl) {
  uint16_t cursorVal = 0;

  if (Ctrl->Rotary_State == INIT_STATE                  || \
      Ctrl->Rotary_State == CHANGE_STATE                || \
      Ctrl->Rotary_State == HOLD_STATE                  || \
      Ctrl->Rotary_State == ROTARY_CLICK_STATE          || \
      Ctrl->Rotary_State == ROTARY_CLICKED_HOLD_STATE   || \
      Ctrl->Rotary_State == ROTARY_SET_VALUE_STATE      || \
      Ctrl->Rotary_State == ROTARY_SET_VALUE_HOLD_STATE || \
      Ctrl->Rotary_State == ROTARY_RELEASE_STATE        || \
      Ctrl->Valid                                       || \
      Ctrl->ThermostatDataPtr->MISC.GUI_INIT_READY == FALSE) {
      return;
  }

  if (Ctrl->ThermostatDataPtr->MISC.TOUCH_CURSOR_L_AVAILABLE == TRUE && \
      Ctrl->ThermostatDataPtr->MISC.TOUCH_CURSOR_H_AVAILABLE == TRUE) {
      cursorVal = (Ctrl->ThermostatDataPtr->TouchCursorL | \
                  (Ctrl->ThermostatDataPtr->TouchCursorH << 8));

      Ctrl->target = (I32)(1500 + (cursorVal * 50));
      Ctrl->rotaryVal = APP_Target_Rotary_GetValue();

      if (Ctrl->rotaryVal != Ctrl->target) {
          Ctrl->Rotary_State = ROTARY_CLICK_STATE;
          GUI_ROTARY_STATE_MSG("cursorVal: %d, rotaryVal: %d, taget: %d\n", cursorVal, Ctrl->rotaryVal, Ctrl->target);
      }
      Ctrl->ThermostatDataPtr->MISC.TOUCH_CURSOR_L_AVAILABLE = FALSE;
      Ctrl->ThermostatDataPtr->MISC.TOUCH_CURSOR_H_AVAILABLE = FALSE;
  }

  //Ctrl->Valid = TRUE;

  return;
}

/*----------------------------------------------------------------------------
  RotaryStateCtrl handler
 *----------------------------------------------------------------------------*/
void RotaryStateCtrl (GUI_ROTARY_CTRL_STRC *Ctrl) {

  switch(Ctrl->Rotary_State) {
         case INIT_STATE:
              Ctrl->TimerPtr       = TimerCtrlPtr;
              Ctrl->last_rotaryVal = 0;
              Ctrl->pressed        = FALSE;
              Ctrl->last_pressed   = FALSE;
              Ctrl->Valid          = FALSE;
              Ctrl->Rotary_State  = WAIT_CMD_STATE;
              break;
         case CHANGE_STATE:
              if (Ctrl->TimerPtr->GUI_ROTARY_TIMER >= 300) {
                  Ctrl->Rotary_State = WAIT_CMD_STATE;
              }
              break;
         case HOLD_STATE:
              Ctrl->Rotary_State = WAIT_CMD_STATE;
              break;
         case ROTARY_CLICK_STATE:
              APP_Target_Rotary_Clicked();
              Ctrl->TimerPtr->GUI_ROTARY_TIMER = 0;
              Ctrl->Rotary_State = ROTARY_CLICKED_HOLD_STATE;
              //GUI_ROTARY_STATE_MSG("ROTARY_CLICK_STATE\n");
              break;
         case ROTARY_CLICKED_HOLD_STATE:
              if (Ctrl->TimerPtr->GUI_ROTARY_TIMER >= 10) {
                  Ctrl->Rotary_State = ROTARY_SET_VALUE_STATE;
              }
              break;
         case ROTARY_SET_VALUE_STATE:
              if (Ctrl->rotaryVal < Ctrl->target)
                  Ctrl->rotaryVal += 50;
              else if (Ctrl->rotaryVal > Ctrl->target)
                  Ctrl->rotaryVal -= 50;

              APP_Target_Rotary_SetValue(Ctrl->rotaryVal);
              Ctrl->TimerPtr->GUI_ROTARY_TIMER = 0;
              Ctrl->Rotary_State = ROTARY_SET_VALUE_HOLD_STATE;
              //GUI_ROTARY_STATE_MSG("ROTARY_SET_VALUE_STATE - %d\n", Ctrl->rotaryVal);
              break;
         case ROTARY_SET_VALUE_HOLD_STATE:
              if (Ctrl->TimerPtr->GUI_ROTARY_TIMER >= 50) {
                  Ctrl->Rotary_State = ROTARY_RELEASE_STATE;
                  if (Ctrl->rotaryVal != Ctrl->target)
                      Ctrl->Rotary_State = ROTARY_SET_VALUE_STATE;
              }
              break;
         case ROTARY_RELEASE_STATE:
              APP_Target_Rotary_Released();
              Ctrl->Rotary_State = WAIT_CMD_STATE;
              //GUI_ROTARY_STATE_MSG("ROTARY_RELEASE_STATE\n");
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  Ctrl->TimerPtr->GUI_ROTARY_TIMER = 0;
                  Ctrl->Rotary_State = CHANGE_STATE;
                  Ctrl->Valid = FALSE;
              }
			  break;
         default:
              break;
  }
  
  return;
}

/*----------------------------------------------------------------------------
  JoystickStateInit handler
 *----------------------------------------------------------------------------*/
void JoystickStateInit (JOYSTICK_CTRL_STRC *Ctrl) {
  Ctrl->ThermostatDataPtr = ThermostatDataPtr;
  Ctrl->Joystick_State = INIT_STATE;
  return;
}

/*----------------------------------------------------------------------------
  JoystickStateTask handler
 *----------------------------------------------------------------------------*/
void JoystickStateTask (JOYSTICK_CTRL_STRC *Ctrl) {
  uint8_t  pressed = !KEY_TOUCH, up = !KEY_UP, down = !KEY_DOWN;
  I32      val = APP_Target_Rotary_GetValue();

  if (Ctrl->Joystick_State == INIT_STATE   || \
      Ctrl->Joystick_State == CHANGE_STATE || \
      Ctrl->Joystick_State == HOLD_STATE   || \
      Ctrl->Valid                          || \
      Ctrl->ThermostatDataPtr->MISC.GUI_INIT_READY == FALSE) {
      return;
  }

  if (pressed == TRUE && Ctrl->last_pressed == FALSE) {
      APP_Target_Rotary_Clicked();
      JOYSTICK_STATE_MSG("APP_Target_Rotary_Clicked\n");
  }

  if (pressed == TRUE) {
     if (up == TRUE && Ctrl->last_up == FALSE) {
         val +=50;
     }
     if (down == TRUE && Ctrl->last_down == FALSE) {
         val -=50;
     }
     if (val != Ctrl->last_val) {
         APP_Target_Rotary_SetValue(val);
         JOYSTICK_STATE_MSG("APP_Target_Rotary_SetValue: %d\n", val);
     }
     //JOYSTICK_STATE_MSG("Value: %d, Up: %x, Down: %x\n", val, up, down);
  }

  if (pressed == FALSE && Ctrl->last_pressed == TRUE) {
      APP_Target_Rotary_Released();
      JOYSTICK_STATE_MSG("APP_Target_Rotary_Released\n");
  }

  Ctrl->last_pressed = pressed;
  Ctrl->last_up      = up;
  Ctrl->last_down    = down;
  Ctrl->last_val     = val;
  Ctrl->Valid        = TRUE;

  return;
}

/*----------------------------------------------------------------------------
  JoystickStateCtrl handler
 *----------------------------------------------------------------------------*/
void JoystickStateCtrl (JOYSTICK_CTRL_STRC *Ctrl) {
  
  switch(Ctrl->Joystick_State) {
         case INIT_STATE:
              Ctrl->TimerPtr     = TimerCtrlPtr;
              Ctrl->last_val     = 0;
              Ctrl->last_pressed = FALSE;
              Ctrl->last_up      = FALSE;
              Ctrl->last_down    = FALSE;
              Ctrl->Valid        = FALSE;
              Ctrl->Joystick_State  = WAIT_CMD_STATE;
              break;
         case CHANGE_STATE:
              if (Ctrl->TimerPtr->I2C_TOUCH_TIMER >= 50) {
                  Ctrl->Joystick_State = WAIT_CMD_STATE;
              }
              break;
         case HOLD_STATE:
              Ctrl->Joystick_State = WAIT_CMD_STATE;
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  Ctrl->TimerPtr->I2C_TOUCH_TIMER = 0;
                  Ctrl->Joystick_State = CHANGE_STATE;
                  Ctrl->Valid = FALSE;
              }
			  break;
         default:
              break;
  }
  
  return;
}

/*----------------------------------------------------------------------------
   EepromReadStateInit handler
 *----------------------------------------------------------------------------*/
void EepromReadStateInit (EEPROM_READ_CTRL_STRC *Ctrl) {

  Ctrl->Read_State = INIT_STATE;

  EEPROM_READ_STATE_MSG("EepromReadStateInit\n");

  return;
}

/*----------------------------------------------------------------------------
   EepromReadStateTask handler
 *----------------------------------------------------------------------------*/
void EepromReadStateTask (EEPROM_READ_CTRL_STRC *Ctrl) {
  
  if (Ctrl->Read_State == INIT_STATE   || \
      Ctrl->Read_State == CHANGE_STATE || \
  	  Ctrl->Read_State == HOLD_STATE   || \
      Ctrl->Valid                      || \
      Ctrl->ThermostatDataPtr->MISC.EEPROM_WRITE_DONE == FALSE) {
      return;
  }
 
  if (Ctrl->Offset < (64 * 1024)) {
      RwEepromDataByte (Ctrl->Offset, 0, TRUE);
      Ctrl->Valid = TRUE;
      EEPROM_READ_STATE_MSG("Eeprom Read Start, Offset: 0x%x\n", Ctrl->Offset);
  }

  return;
}

/*----------------------------------------------------------------------------
   EepromReadStateCtrl handler
 *----------------------------------------------------------------------------*/
void EepromReadStateCtrl (EEPROM_READ_CTRL_STRC *Ctrl) {

  switch(Ctrl->Read_State) {
         case INIT_STATE:
              Ctrl->ThermostatDataPtr = ThermostatDataPtr;
              Ctrl->TimerPtr          = TimerCtrlPtr;
              Ctrl->DevAddr           = &g_u8DeviceAddr;
              Ctrl->TxDataBuff        = g_au8TxData;
              Ctrl->RxData            = &g_u8RxData;
              Ctrl->DataLen           = &g_u8DataLen;
              Ctrl->DataReady         = &g_u8EndFlag;
              Ctrl->ctx               = &I2C2_Ctx;
              Ctrl->Offset            = 0xC0;
              Ctrl->Read_State        = WAIT_CMD_STATE;
              Ctrl->Valid             = FALSE;
              break;
         case CHANGE_STATE:
              Ctrl->Read_State = WAIT_CMD_STATE;
              break;
         case HOLD_STATE:
              // to execute EEPROM read per 5ms
              if (Ctrl->TimerPtr->EEPROM_READ_TIMER >= 10 && \
                  *Ctrl->DataReady == TRUE) {
                  EEPROM_READ_STATE_MSG("R - DATA: 0x%x\n", *Ctrl->RxData);
                  Ctrl->Offset++;
                  *Ctrl->DataReady = FALSE;
                  Ctrl->Read_State = WAIT_CMD_STATE;
              }
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  Ctrl->Read_State = HOLD_STATE;
                  Ctrl->TimerPtr->EEPROM_READ_TIMER = 0;
                  Ctrl->Valid = FALSE;
              }
              break;
         default:
              break;
  }

  return;
}

/*----------------------------------------------------------------------------
   EepromWriteStateInit handler
 *----------------------------------------------------------------------------*/
void EepromWriteStateInit (EEPROM_WRITE_CTRL_STRC *Ctrl) {

  Ctrl->Write_State = INIT_STATE;

  return;
}

/*----------------------------------------------------------------------------
   EepromWriteStateTask handler
 *----------------------------------------------------------------------------*/
void EepromWriteStateTask (EEPROM_WRITE_CTRL_STRC *Ctrl) {
  
  if (Ctrl->Write_State == INIT_STATE   || \
      Ctrl->Write_State == CHANGE_STATE || \
  	  Ctrl->Write_State == HOLD_STATE   || \
      Ctrl->Valid) {
      return;
  }
 
  if (Ctrl->Offset < (1024)) {
      RwEepromDataByte (Ctrl->Offset, Ctrl->Dummy++, FALSE);
      EEPROM_WRITE_STATE_MSG("Eeprom WRITE Start, Offset: 0x%x\n", Ctrl->Offset);
      Ctrl->Valid = TRUE;
  } else {
      Ctrl->ThermostatDataPtr->MISC.EEPROM_WRITE_DONE = TRUE;
  }

  return;
}

/*----------------------------------------------------------------------------
   EepromWriteStateCtrl handler
 *----------------------------------------------------------------------------*/
void EepromWriteStateCtrl (EEPROM_WRITE_CTRL_STRC *Ctrl) {

  switch(Ctrl->Write_State) {
         case INIT_STATE:
              Ctrl->ThermostatDataPtr = ThermostatDataPtr;
              Ctrl->TimerPtr          = TimerCtrlPtr;
              Ctrl->Dummy             = 0;
              Ctrl->DevAddr           = &g_u8DeviceAddr;
              Ctrl->TxDataBuff        = g_au8TxData;
              Ctrl->RxData            = &g_u8RxData;
              Ctrl->DataLen           = &g_u8DataLen;
              Ctrl->DataReady         = &g_u8EndFlag;
              Ctrl->Offset            = 0;
              Ctrl->Write_State       = WAIT_CMD_STATE;
              Ctrl->Valid             = FALSE;
              Ctrl->ThermostatDataPtr->MISC.EEPROM_WRITE_DONE = FALSE;
              break;
         case CHANGE_STATE:
              Ctrl->Write_State = WAIT_CMD_STATE;
              break;
         case HOLD_STATE:
              // to execute EEPROM read per 5ms
              if (Ctrl->TimerPtr->EEPROM_WRITE_TIMER >= 1 && \
                  *Ctrl->DataReady == TRUE) {
                  EEPROM_WRITE_STATE_MSG("W - Offset(0x%x): 0x%x\n", Ctrl->Offset, Ctrl->Dummy);
                  Ctrl->Offset++;
                  *Ctrl->DataReady = FALSE;
                  Ctrl->Write_State = WAIT_CMD_STATE;
              }
              break;
         case WAIT_CMD_STATE:
              if (Ctrl->Valid) {
                  Ctrl->Write_State = HOLD_STATE;
                  Ctrl->TimerPtr->EEPROM_WRITE_TIMER = 0;
                  Ctrl->Valid = FALSE;
              }
              break;
         default:
              break;
  }
  
  return;
}

/*----------------------------------------------------------------------------
  ThermostatInit handler
 *----------------------------------------------------------------------------*/
void ThermostatInit (void) {

  /* Initialize all of task structures */
  FmcStateInit (FmcCtrlPtr);

  GuiExecStateInit (GuiCtrlPtr);

  TouchPadStateInit (TouchPadCtrlPtr);

  RotaryStateInit (RotaryCtrlPtr);

  JoystickStateInit (JoystickCtrlPtr);

  EepromReadStateInit (EepromReadPtr);

  EepromWriteStateInit (EepromWritePtr);

  return;
}

/*----------------------------------------------------------------------------
  ThermostatTask handler
 *----------------------------------------------------------------------------*/
void ThermostatTask (void) {
  
  FmcStateTask (FmcCtrlPtr);

  GuiExecStateTask (GuiCtrlPtr);

  TouchPadStateTask (TouchPadCtrlPtr);

  RotaryStateTask (RotaryCtrlPtr);

  JoystickStateTask (JoystickCtrlPtr);

  //EepromReadStateTask (EepromReadPtr);

  //EepromWriteStateTask (EepromWritePtr);

  return;
}

/*----------------------------------------------------------------------------
  ThermostatCtrl handler
 *----------------------------------------------------------------------------*/
void ThermostatCtrl (void) {

  FmcStateCtrl (FmcCtrlPtr);

  GuiExecStateCtrl (GuiCtrlPtr);

  TouchPadStateCtrl (TouchPadCtrlPtr);

  RotaryStateCtrl (RotaryCtrlPtr);

  JoystickStateCtrl (JoystickCtrlPtr);

  //EepromReadStateCtrl (EepromReadPtr);

  //EepromWriteStateCtrl (EepromWritePtr);

  return;
}

/*----------------------------------------------------------------------------
  MainTask handler
 *----------------------------------------------------------------------------*/
void MainTask (void) {
  
  /* Gauge State Init */
  ThermostatInit ();

  while (TRUE) {

    ThermostatTask ();

    ThermostatCtrl ();

   }

}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void) {

  Platform_Initialize ();

  MainTask ();

}

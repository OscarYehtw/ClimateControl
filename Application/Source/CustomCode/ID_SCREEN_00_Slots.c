/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2025  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : ID_SCREEN_00_Slots.c
Purpose     : AppWizard managed file, function content could be changed
---------------------------END-OF-HEADER------------------------------
*/

#include <stdio.h>
#include <string.h>

#include "Application.h"
#include "../Generated/Resource.h"
#include "../Generated/ID_SCREEN_00.h"

/*** Begin of user code area ***/
/*** End of user code area ***/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       cbID_SCREEN_00
*/
void cbID_SCREEN_00(WM_MESSAGE * pMsg) {
  GUI_USE_PARA(pMsg);
  //WM_HWIN hDialog;
  WM_HWIN hRotary;
	//int Value;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
      //WM_CreateTimer(pMsg->hWin, 0, 7000, 0);
      break;

  case WM_NOTIFY_PARENT:
      switch (WM_GetId(pMsg->hWinSrc)) {
      case ID_ROTARY_TARGET:
          switch (pMsg->Data.v) {
          case WM_NOTIFICATION_VALUE_CHANGED:
              //Value = ROTARY_GetValue(pMsg->hWinSrc);
              //WM_NotifyParent(hRotary, WM_NOTIFICATION_RELEASED);
              //printf("ID_ROTARY_TARGET value: %d\n", Value);
              break;
          }
          break;
      }
      break;

  case WM_TIMER: {
      hRotary = WM_GetDialogItem(pMsg->hWin, ID_ROTARY_TARGET);
      if (!hRotary) break;

      static int s_tempC = 10;
      static int s_dir = 5;
      if (s_tempC >= 30) s_dir = -5;
      else if (s_tempC <= 10) s_dir = 5;
      s_tempC += s_dir;

      int val = s_tempC * 100;

      //GUI_MEMDEV_Select(hRotaryMem);
      ROTARY_SetValue(hRotary, val);
  		WM_InvalidateWindow(hRotary);
		
      WM_RestartTimer(pMsg->Data.v, 7000);
      WM_NotifyParent(hRotary, WM_NOTIFICATION_RELEASED);
      break;
  }
  default:
      //WM_DefaultProc(pMsg);
      break;
  }
}

/*********************************************************************
*
*       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR
*/
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  APP_GetBoxColor(pAction);
  WM_SetHasTrans(pMsg->hWin);

}

/*********************************************************************
*
*       ID_SCREEN_00__ID_ROTARY_ACTUAL__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR
*/
void ID_SCREEN_00__ID_ROTARY_ACTUAL__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  APP_GetBoxColor(pAction);
  WM_SetHasTrans(pMsg->hWin);

}

/*********************************************************************
*
*       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_L__APPW_JOB_SETVALUE
*/
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_L__APPW_JOB_SETVALUE(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  APP_OnValueChanged(pAction);
}

/*********************************************************************
*
*       ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_ROTARY_TARGET__APPW_JOB_SETVALUE
*/
void ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_ROTARY_TARGET__APPW_JOB_SETVALUE(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  APP_OnInitDialog(hScreen);
}

/*********************************************************************
*
*       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_RELEASED__ID_ROTARY_TARGET__APPW_JOB_SETENABLE
*/
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_RELEASED__ID_ROTARY_TARGET__APPW_JOB_SETENABLE(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  APP_OnReleased();
  *pResult = 1;
}

/*********************************************************************
*
*       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_CLICKED__ID_BOX__APPW_JOB_SETENABLE
*/
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_CLICKED__ID_BOX__APPW_JOB_SETENABLE(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  APP_OnClicked();
  *pResult = 1;
}

/*********************************************************************
*
*       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_MOTION_STOPPED__ID_BOX__APPW_JOB_SETENABLE
*/
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_MOTION_STOPPED__ID_BOX__APPW_JOB_SETENABLE(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  APP_OnReleased();
  *pResult = 1;
}

/*************************** End of file ****************************/

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
extern GUI_COLOR _GetBkColor(void);

static GUI_GIF_INFO _GIF;
//static int _NumFrames, _FrameIndex, _Delay;
static int _NumFrames, _FrameIndex;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       _cbImageLeaf
*/
static void _cbImageLeaf(WM_MESSAGE * pMsg) {

   switch (pMsg->MsgId) {
   case WM_CREATE:
        //if (GUI_GIF_GetInfo(_acFan_40x40, sizeof(_acFan_40x40), &_GIF) == 0) {
        //    _NumFrames = _GIF.NumImages;
        //    _FrameIndex = 0;
        //}
        //WM_CreateTimer(pMsg->hWin, 1, 0, 0);
        //printf("_cbImageLeaf - WM_CREATE\n");
        break;

   case WM_PAINT:
        //WM_SetHasTrans(pMsg->hWin);
        GUI_SetBkColor(_GetBkColor());

        if (_NumFrames > 0)
            GUI_GIF_DrawSub(_acFan_40x40, sizeof(_acFan_40x40), 0, 0, _FrameIndex);
        else
            GUI_GIF_Draw(_acFan_40x40, sizeof(_acFan_40x40), 0, 0);

        //printf("_cbImageLeaf - WM_PAINT: %d\n", _NumFrames);
        break;

   case WM_TIMER:
        //if (_NumFrames > 0) {
        //    _Delay = GUI_GIF_DrawSub(_acFan_40x40, sizeof(_acFan_40x40), 0, 0, _FrameIndex);
        //    _FrameIndex = (_FrameIndex + 1) % _NumFrames;
        //    WM_RestartTimer(pMsg->Data.v, (_Delay > 0) ? _Delay : 100);
        //    WM_InvalidateWindow(pMsg->hWin);
        //}
        //printf("_cbImageLeaf - WM_TIMER\n");
        break;

   case WM_DELETE:
        _NumFrames = 0;
        break;

   default:
        IMAGE_Callback(pMsg);
        break;
  }
}

/*********************************************************************
*
*       cbID_SCREEN_00
*/
void cbID_SCREEN_00(WM_MESSAGE * pMsg) {
  GUI_USE_PARA(pMsg);
  //WM_HWIN hDialog;
  //WM_HWIN hRotary;
	//int Value;
  WM_HWIN hLeaf;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
      hLeaf = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LEAF);
      WM_SetCallback(hLeaf, _cbImageLeaf);
      if (GUI_GIF_GetInfo(_acFan_40x40, sizeof(_acFan_40x40), &_GIF) == 0) {
          _NumFrames = _GIF.NumImages;
          _FrameIndex = 0;
      }
      WM_CreateTimer(pMsg->hWin, 0, 1000, 0);
      //printf("_NumFrames: %d\n", _NumFrames);
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

  case WM_PAINT:
      //switch (WM_GetId(pMsg->hWinSrc)) {
      //case ID_IMAGE_LEAF:
      //    //GUI_GIF_Draw(_acFan_40x40, sizeof(_acFan_40x40), 0, 0); // Drawing gif
      //    break;
      //default:
      //    break;
      //}
      //printf("WM_PAINT: %X\n", WM_GetId(pMsg->hWinSrc));
      break;
  case WM_TIMER: {
      #if 0
      hRotary = WM_GetDialogItem(pMsg->hWin, ID_ROTARY_TARGET);
      if (!hRotary) break;

      static int s_tempC = 10;
      static int s_dir = 5;
      if (s_tempC >= 30) s_dir = -5;
      else if (s_tempC <= 10) s_dir = 5;
      s_tempC += s_dir;

      int val = s_tempC * 100;

      GUI_MEMDEV_Select(hRotaryMem);
      ROTARY_SetValue(hRotary, val);
  		WM_InvalidateWindow(hRotary);
		
      WM_NotifyParent(hRotary, WM_NOTIFICATION_RELEASED);
      #endif
      if (_NumFrames > 0) {
          //_Delay = GUI_GIF_DrawSub(_acFan_40x40, sizeof(_acFan_40x40), 0, 0, _FrameIndex);
          _FrameIndex = (_FrameIndex + 10) % _NumFrames;
          //WM_RestartTimer(pMsg->Data.v, (_Delay > 0) ? _Delay : 200);
          //WM_InvalidateWindow(pMsg->hWin);
      }
      hLeaf = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LEAF);
      WM_RestartTimer(pMsg->Data.v, 100);
  		WM_InvalidateWindow(hLeaf);
      //printf("cbID_SCREEN_00 - WM_TIMER: Frames:%d Delay:%d\n", _NumFrames, _Delay);
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

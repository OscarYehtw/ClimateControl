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
File        : ID_SCREEN_00.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef ID_SCREEN_00_H
#define ID_SCREEN_00_H

#include "AppWizard.h"

/*********************************************************************
*
*       Objects
*/
#define ID_IMAGE_BG      (GUI_ID_USER + 11)
#define ID_BOX           (GUI_ID_USER + 2)
#define ID_IMAGE_SCALE   (GUI_ID_USER + 4)
#define ID_IMAGE_LEAF    (GUI_ID_USER + 5)
#define ID_TEXT_L        (GUI_ID_USER + 6)
#define ID_TEXT_L5       (GUI_ID_USER + 7)
#define ID_TEXT_S        (GUI_ID_USER + 8)
#define ID_TEXT_S5       (GUI_ID_USER + 9)
#define ID_TEXT_HINT     (GUI_ID_USER + 10)
#define ID_ROTARY_ACTUAL (GUI_ID_USER + 3)
#define ID_ROTARY_TARGET (GUI_ID_USER + 1)

/*********************************************************************
*
*       Slots
*/
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR      (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_ROTARY_ACTUAL__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR      (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_L__APPW_JOB_SETVALUE   (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_ROTARY_TARGET__APPW_JOB_SETVALUE               (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_RELEASED__ID_ROTARY_TARGET__APPW_JOB_SETENABLE(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_CLICKED__ID_BOX__APPW_JOB_SETENABLE           (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_MOTION_STOPPED__ID_BOX__APPW_JOB_SETENABLE    (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);

/*********************************************************************
*
*       Callback
*/
void cbID_SCREEN_00(WM_MESSAGE * pMsg);

#endif  // ID_SCREEN_00_H

/*************************** End of file ****************************/

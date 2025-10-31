/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2022  SEGGER Microcontroller GmbH                *
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
#define ID_BOX_00    (GUI_ID_USER + 1)
#define ID_TEXT_00   (GUI_ID_USER + 2)
#define ID_EDIT_00   (GUI_ID_USER + 3)
#define ID_BUTTON_00 (GUI_ID_USER + 4)
#define ID_BUTTON_01 (GUI_ID_USER + 5)
#define ID_BUTTON_02 (GUI_ID_USER + 6)
#define ID_BUTTON_03 (GUI_ID_USER + 7)

/*********************************************************************
*
*       Slots
*/
void ID_SCREEN_00__ID_BUTTON_00__WM_NOTIFICATION_RELEASED                        (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE  (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_BUTTON_01__WM_NOTIFICATION_RELEASED                        (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE_0(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_BUTTON_02__WM_NOTIFICATION_RELEASED                        (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE_1(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__ID_BUTTON_03__WM_NOTIFICATION_RELEASED                        (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE_2(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);

/*********************************************************************
*
*       Callback
*/
void cbID_SCREEN_00(WM_MESSAGE * pMsg);

#endif  // ID_SCREEN_00_H

/*************************** End of file ****************************/

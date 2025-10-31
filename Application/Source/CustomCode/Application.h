/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2020  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : Application.h
Purpose     : Content to be managed by customer
---------------------------END-OF-HEADER------------------------------
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "WM.h"
#include "AppWizard.h"

void APP_GetBoxColor   (APPW_ACTION_ITEM * pAction);
void APP_OnClicked     (void);
void APP_OnReleased    (void);
void APP_OnValueChanged(APPW_ACTION_ITEM * pAction);
void APP_OnInitDialog  (WM_HWIN hScreen);

#endif  // APPLICATION_H

/*************************** End of file ****************************/

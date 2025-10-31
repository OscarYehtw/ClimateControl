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
File        : Resource.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef RESOURCE_H
#define RESOURCE_H

#include "AppWizard.h"

/*********************************************************************
*
*       Text
*/
#define ID_RTEXT_0 0
#define ID_RTEXT_1 1
#define ID_RTEXT_2 2
#define ID_RTEXT_3 3
#define ID_RTEXT_4 4

#define APPW_MANAGE_TEXT APPW_MANAGE_TEXT_EXT
extern GUI_CONST_STORAGE unsigned char acAPPW_Language_0[];

/*********************************************************************
*
*       Images
*/
extern GUI_CONST_STORAGE unsigned char acDARK_Button_Up_100x30[];
extern GUI_CONST_STORAGE unsigned char acDARK_Button_Down_100x30[];

/*********************************************************************
*
*       Variables
*/
#define ID_VAR_00 (GUI_ID_USER + 2048)
#define ID_VAR_01 (GUI_ID_USER + 2049)
#define ID_VAR_02 (GUI_ID_USER + 2050)
#define ID_VAR_03 (GUI_ID_USER + 2051)

/*********************************************************************
*
*       Screens
*/
#define ID_SCREEN_00 (GUI_ID_USER + 4096)

extern APPW_ROOT_INFO ID_SCREEN_00_RootInfo;

#define APPW_INITIAL_SCREEN &ID_SCREEN_00_RootInfo

/*********************************************************************
*
*       Project path
*/
#define APPW_PROJECT_PATH "C:/AppWizard/Project1/SimpleDemo"

#endif  // RESOURCE_H

/*************************** End of file ****************************/

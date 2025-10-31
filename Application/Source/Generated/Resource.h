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
#define ID_TEXT_0 0
#define ID_TEXT_1 1
#define ID_TEXT_2 2
#define ID_TEXT_3 3
#define ID_TEXT_4 4

extern GUI_CONST_STORAGE unsigned char acAPPW_Language_0[];

/*********************************************************************
*
*       Font data
*/
extern APPW_FONT APPW__aFont[5];

/*********************************************************************
*
*       Fonts
*/
extern GUI_CONST_STORAGE unsigned char acRoboto_150_Normal_EXT_AA4[];
extern GUI_CONST_STORAGE unsigned char acRoboto_40_Normal_EXT_AA4[];
extern GUI_CONST_STORAGE unsigned char acNettoOT_32_Normal_EXT_AA4[];
extern GUI_CONST_STORAGE unsigned char acNettoOT_16_Normal_EXT_AA4[];
extern GUI_CONST_STORAGE unsigned char acRoboto_16_Normal_EXT_AA4[];

/*********************************************************************
*
*       Images
*/
extern GUI_CONST_STORAGE unsigned char acScale_240x240[];
extern GUI_CONST_STORAGE unsigned char acLeaf_30x39[];
extern GUI_CONST_STORAGE unsigned char acIndicatorActual_18x2[];
extern GUI_CONST_STORAGE unsigned char acIndicatorTarget_24x3[];

/*********************************************************************
*
*       Content
*/
extern GUI_CONST_STORAGE APPW_CONTENT ID_CONTENT_000_Data;
extern GUI_CONST_STORAGE APPW_CONTENT ID_CONTENT_001_Data;
extern GUI_CONST_STORAGE APPW_CONTENT ID_CONTENT_002_Data;

/*********************************************************************
*
*       Screens
*/
#define ID_SCREEN_00 (GUI_ID_USER + 0)

extern APPW_ROOT_INFO ID_SCREEN_00_RootInfo;

#define APPW_INITIAL_SCREEN &ID_SCREEN_00_RootInfo

/*********************************************************************
*
*       Project path
*/
#define APPW_PROJECT_PATH "C:/temp/AppWizard/ClimateControl8"

#endif  // RESOURCE_H

/*************************** End of file ****************************/

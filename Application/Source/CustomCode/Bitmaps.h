/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : Bitmaps.h
Purpose     : Prototypes for bitmaps of the dotted animated line
---------------------------END-OF-HEADER------------------------------
*/

#ifndef BITMAPS_H
#define BITMAPS_H

#include "GUI.h"

extern GUI_CONST_STORAGE GUI_BITMAP bmDot_20_5x5;
extern GUI_CONST_STORAGE GUI_BITMAP bmDot_40_5x5;
extern GUI_CONST_STORAGE GUI_BITMAP bmDot_60_5x5;
extern GUI_CONST_STORAGE GUI_BITMAP bmDot_80_5x5;
extern GUI_CONST_STORAGE GUI_BITMAP bmDot_100_5x5;

extern const unsigned char acHeat_390x390[];
extern const unsigned char acCool_390x390[];

extern U32 acHeat_390x390_Size;
extern U32 acCool_390x390_Size;

#endif  // BITMAPS_H

/*************************** End of file ****************************/

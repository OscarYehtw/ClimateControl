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
File        : APPWConf.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "AppWizard.h"
#include "Resource.h"

#ifdef WIN32
  #include "GUIDRV_Win32R.h"
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define XSIZE_PHYS       320
#define YSIZE_PHYS       240
#define COLOR_CONVERSION GUICC_M565
#define DISPLAY_DRIVER   GUIDRV_WIN32R
#define NUM_BUFFERS      2
#define _appDrawing      NULL
#define _NumDrawings     0
#define _aScrollerList   NULL
#define _NumScrollers    0

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _apRootList
*/
static APPW_ROOT_INFO * _apRootList[] = {
  &ID_SCREEN_00_RootInfo,
};

/*********************************************************************
*
*       _NumScreens
*/
static unsigned _NumScreens = GUI_COUNTOF(_apRootList);

/*********************************************************************
*
*       _Calc_ID_VAR_00
*/
static GUI_CONST_STORAGE APPW_ATOM _aItem_ID_VAR_00[] = {
  { 0x00000064, 0, ATOM_CONSTANT, 0 },
  { 0x18000803, 0, ATOM_OBJECT_VALUE, 0 }
};

static I32 _aCalc_ID_VAR_00[GUI_COUNTOF(_aItem_ID_VAR_00)];

static GUI_CONST_STORAGE APPW_CALC _Calc_ID_VAR_00 = {
  "A+B", 2, _aItem_ID_VAR_00, _aCalc_ID_VAR_00
};

/*********************************************************************
*
*       _Calc_ID_VAR_01
*/
static GUI_CONST_STORAGE APPW_ATOM _aItem_ID_VAR_01[] = {
  { 0xffffff9c, 0, ATOM_CONSTANT, 0 },
  { 0x18000803, 0, ATOM_OBJECT_VALUE, 0 }
};

static I32 _aCalc_ID_VAR_01[GUI_COUNTOF(_aItem_ID_VAR_01)];

static GUI_CONST_STORAGE APPW_CALC _Calc_ID_VAR_01 = {
  "A+B", 2, _aItem_ID_VAR_01, _aCalc_ID_VAR_01
};

/*********************************************************************
*
*       _Calc_ID_VAR_02
*/
static GUI_CONST_STORAGE APPW_ATOM _aItem_ID_VAR_02[] = {
  { 0x0000000a, 0, ATOM_CONSTANT, 0 },
  { 0x18000803, 0, ATOM_OBJECT_VALUE, 0 }
};

static I32 _aCalc_ID_VAR_02[GUI_COUNTOF(_aItem_ID_VAR_02)];

static GUI_CONST_STORAGE APPW_CALC _Calc_ID_VAR_02 = {
  "A+B", 2, _aItem_ID_VAR_02, _aCalc_ID_VAR_02
};

/*********************************************************************
*
*       _Calc_ID_VAR_03
*/
static GUI_CONST_STORAGE APPW_ATOM _aItem_ID_VAR_03[] = {
  { 0xfffffff6, 0, ATOM_CONSTANT, 0 },
  { 0x18000803, 0, ATOM_OBJECT_VALUE, 0 }
};

static I32 _aCalc_ID_VAR_03[GUI_COUNTOF(_aItem_ID_VAR_03)];

static GUI_CONST_STORAGE APPW_CALC _Calc_ID_VAR_03 = {
  "A+B", 2, _aItem_ID_VAR_03, _aCalc_ID_VAR_03
};

/*********************************************************************
*
*       _aVarList
*/
static APPW_VAR_OBJECT _aVarList[] = {
  { ID_VAR_00, 0, 0, &_Calc_ID_VAR_00 },
  { ID_VAR_01, 0, 0, &_Calc_ID_VAR_01 },
  { ID_VAR_02, 0, 0, &_Calc_ID_VAR_02 },
  { ID_VAR_03, 0, 0, &_Calc_ID_VAR_03 },
};

/*********************************************************************
*
*       _NumVars
*/
static unsigned _NumVars = GUI_COUNTOF(_aVarList);

/*********************************************************************
*
*       Multibuffering
*/
static U8 _MultibufEnable = 0;

/*********************************************************************
*
*       _ShowMissingCharacters
*/
static U8 _ShowMissingCharacters = 0;

/*********************************************************************
*
*       _apLang
*/
static GUI_CONST_STORAGE char * _apLang[] = {
  (GUI_CONST_STORAGE char *)acAPPW_Language_0,
};

/*********************************************************************
*
*       _TextInit
*/
static GUI_CONST_STORAGE APPW_TEXT_INIT _TextInit = {
  _apLang,
  GUI_COUNTOF(_apLang),
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitText
*/
static void _InitText(void) {
  APPW_TextInitMem(&_TextInit);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW_X_Setup
*/
void APPW_X_Setup(void) {
  APPW_SetpfInitText(_InitText);
  APPW_X_FS_Init();
  APPW_MULTIBUF_Enable(_MultibufEnable);
  APPW_SetData(_apRootList, _NumScreens, _aVarList, _NumVars, _aScrollerList, _NumScrollers, (APPW_DRAWING_ITEM **)_appDrawing, _NumDrawings);
  APPW_SetSupportScroller(0);
  GUI_ShowMissingCharacters(_ShowMissingCharacters);
}

/*********************************************************************
*
*       APPW_X_Config
*/
#ifdef WIN32
void APPW_X_Config(void) {
  GUI_MULTIBUF_Config(NUM_BUFFERS);
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
}
#endif

/*************************** End of file ****************************/

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
File        : ID_SCREEN_00.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "Resource.h"
#include "ID_SCREEN_00.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _aCreate
*/
static APPW_CREATE_ITEM _aCreate[] = {
  { WM_OBJECT_WINDOW_Create,
    ID_SCREEN_00, 0,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BOX_Create,
    ID_BOX_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 113, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 100, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_EDIT_Create,
    ID_EDIT_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 213, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 100, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 32, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_00, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 8, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 8, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 30, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_01, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 8, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 70, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 30, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_02, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 8, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 132, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 30, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BUTTON_Create,
    ID_BUTTON_03, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 8, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 200, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      100, 30, 0, 0, 0, 0
    },
    { 0, 0 }
  },
};

/*********************************************************************
*
*       _aSetup
*/
static GUI_CONST_STORAGE APPW_SETUP_ITEM _aSetup[] = {
  { ID_BOX_00,    APPW_SET_PROP_COLOR,        { ARG_V(GUI_DARKGRAY) } },
  { ID_TEXT_00,   APPW_SET_PROP_COLOR,        { ARG_V(0xffc0c0c0) } },
  { ID_TEXT_00,   APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_TEXT_00,   APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_0) } },
  { ID_EDIT_00,   APPW_SET_PROP_COLORS,       { ARG_V(GUI_BLACK),
                                                ARG_V(GUI_INVALID_COLOR),
                                                ARG_V(GUI_WHITE) } },
  { ID_EDIT_00,   APPW_SET_PROP_ALIGN,        { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER) } },
  { ID_EDIT_00,   APPW_SET_PROP_BKCOLORS,     { ARG_V(GUI_WHITE),
                                                ARG_V(GUI_INVALID_COLOR),
                                                ARG_V(GUI_BLACK) } },
  { ID_EDIT_00,   APPW_SET_PROP_PERIOD,       { ARG_V(300) } },
  { ID_EDIT_00,   APPW_SET_PROP_INVERT,       { ARG_V(0) } },
  { ID_EDIT_00,   APPW_SET_PROP_DECMODE,      { ARG_V(0),
                                                ARG_V(0),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_EDIT_00,   APPW_SET_PROP_RANGE,        { ARG_V(-32768),
                                                ARG_V(32767) } },
  { ID_BUTTON_00, APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_00, APPW_SET_PROP_COLORS,       { ARG_V(0xffc0c0c0),
                                                ARG_V(0xffc0c0c0),
                                                ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_00, APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_1) } },
  { ID_BUTTON_01, APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_01, APPW_SET_PROP_COLORS,       { ARG_V(0xffc0c0c0),
                                                ARG_V(0xffc0c0c0),
                                                ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_01, APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_2) } },
  { ID_BUTTON_02, APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_02, APPW_SET_PROP_COLORS,       { ARG_V(0xffc0c0c0),
                                                ARG_V(0xffc0c0c0),
                                                ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_02, APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_3) } },
  { ID_BUTTON_03, APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, acDARK_Button_Up_100x30),
                                                ARG_VP(0, acDARK_Button_Down_100x30), } },
  { ID_BUTTON_03, APPW_SET_PROP_COLORS,       { ARG_V(0xffc0c0c0),
                                                ARG_V(0xffc0c0c0),
                                                ARG_V(GUI_INVALID_COLOR) } },
  { ID_BUTTON_03, APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_4) } },
};

/*********************************************************************
*
*       _aAction
*/
static GUI_CONST_STORAGE APPW_ACTION_ITEM _aAction[] = {
  { ID_BUTTON_00, WM_NOTIFICATION_RELEASED,         ID_VAR_00,    APPW_JOB_CALC,           ID_SCREEN_00__ID_BUTTON_00__WM_NOTIFICATION_RELEASED,
  },
  { ID_VAR_00,    WM_NOTIFICATION_VALUE_CHANGED,    ID_EDIT_00,   APPW_JOB_SETVALUE,       ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE,
    { ARG_V(0),
    }, 0, NULL
  },
  { ID_BUTTON_01, WM_NOTIFICATION_RELEASED,         ID_VAR_01,    APPW_JOB_CALC,           ID_SCREEN_00__ID_BUTTON_01__WM_NOTIFICATION_RELEASED,
  },
  { ID_VAR_01,    WM_NOTIFICATION_VALUE_CHANGED,    ID_EDIT_00,   APPW_JOB_SETVALUE,       ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE_0,
    { ARG_V(0),
    }, 0, NULL
  },
  { ID_BUTTON_02, WM_NOTIFICATION_RELEASED,         ID_VAR_02,    APPW_JOB_CALC,           ID_SCREEN_00__ID_BUTTON_02__WM_NOTIFICATION_RELEASED,
  },
  { ID_VAR_02,    WM_NOTIFICATION_VALUE_CHANGED,    ID_EDIT_00,   APPW_JOB_SETVALUE,       ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE_1,
    { ARG_V(0),
    }, 0, NULL
  },
  { ID_BUTTON_03, WM_NOTIFICATION_RELEASED,         ID_VAR_03,    APPW_JOB_CALC,           ID_SCREEN_00__ID_BUTTON_03__WM_NOTIFICATION_RELEASED,
  },
  { ID_VAR_03,    WM_NOTIFICATION_VALUE_CHANGED,    ID_EDIT_00,   APPW_JOB_SETVALUE,       ID_SCREEN_00__WM_NOTIFICATION_VALUE_CHANGED__ID_EDIT_00__APPW_JOB_SETVALUE_2,
    { ARG_V(0),
    }, 0, NULL
  },
};

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       ID_SCREEN_00_RootInfo
*/
APPW_ROOT_INFO ID_SCREEN_00_RootInfo = {
  ID_SCREEN_00,
  _aCreate, GUI_COUNTOF(_aCreate),
  _aSetup,  GUI_COUNTOF(_aSetup),
  _aAction, GUI_COUNTOF(_aAction),
  cbID_SCREEN_00,
  0
};

/*************************** End of file ****************************/

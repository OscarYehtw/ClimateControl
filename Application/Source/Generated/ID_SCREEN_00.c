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
  { WM_OBJECT_IMAGE_Create,
    ID_IMAGE_BG, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      320, 240, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BOX_Create,
    ID_BOX, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      320, 240, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_IMAGE_Create,
    ID_IMAGE_SCALE, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 40, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      240, 240, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_IMAGE_Create,
    ID_IMAGE_LEAF, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 145, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 195, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      30, 39, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_L, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 88, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 60, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      140, 120, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_L5, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 230, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 102, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      20, 35, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_S, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 145, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 8, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      26, 24, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_S5, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 170, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 9, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      8, 12, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_HINT, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 115, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 40, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      90, 20, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_ROTARY_Create,
    ID_ROTARY_ACTUAL, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 40, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      240, 240, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_ROTARY_Create,
    ID_ROTARY_TARGET, ID_SCREEN_00,
    { { { DISPOSE_MODE_REL_PARENT, 40, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      240, 240, 0, 0, 0, 0
    },
    { 0, 0 }
  },
};

/*********************************************************************
*
*       _aSetup
*/
static GUI_CONST_STORAGE APPW_SETUP_ITEM _aSetup[] = {
  { ID_BOX,           APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  //{ ID_IMAGE_SCALE,   APPW_SET_PROP_SBITMAP,      { ARG_VP(0, acScale_240x240),
  //                                                  ARG_V(23365), } },
  { ID_IMAGE_LEAF,    APPW_SET_PROP_SBITMAP,      { ARG_VP(0, acLeaf_30x39),
                                                    ARG_V(1673), } },
  { ID_TEXT_L,        APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_L,        APPW_SET_PROP_DECMODE,      { ARG_V(2),
                                                    ARG_V(0),
                                                    ARG_V(0),
                                                    ARG_V(1) } },
  { ID_TEXT_L,        APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                    ARG_V(0),
                                                    ARG_V(0) } },
  { ID_TEXT_L,        APPW_SET_PROP_FONT,         { ARG_VPF(0, acRoboto_150_Normal_EXT_AA4, &APPW__aFont[0]) } },
  { ID_TEXT_L5,       APPW_SET_PROP_FONT,         { ARG_VPF(0, acRoboto_40_Normal_EXT_AA4, &APPW__aFont[1]) } },
  { ID_TEXT_L5,       APPW_SET_PROP_TEXTID,       { ARG_V(ID_TEXT_1) } },
  { ID_TEXT_L5,       APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_S,        APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_S,        APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                    ARG_V(0),
                                                    ARG_V(0) } },
  { ID_TEXT_S,        APPW_SET_PROP_FONT,         { ARG_VPF(0, acNettoOT_32_Normal_EXT_AA4, &APPW__aFont[2]) } },
  { ID_TEXT_S,        APPW_SET_PROP_DECMODE,      { ARG_V(2),
                                                    ARG_V(0),
                                                    ARG_V(0),
                                                    ARG_V(1) } },
  { ID_TEXT_S5,       APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_S5,       APPW_SET_PROP_TEXTID,       { ARG_V(ID_TEXT_1) } },
  { ID_TEXT_S5,       APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                    ARG_V(0),
                                                    ARG_V(0) } },
  { ID_TEXT_S5,       APPW_SET_PROP_FONT,         { ARG_VPF(0, acNettoOT_16_Normal_EXT_AA4, &APPW__aFont[3]) } },
  { ID_TEXT_HINT,     APPW_SET_PROP_TEXTID,       { ARG_V(ID_TEXT_2) } },
  { ID_TEXT_HINT,     APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_HINT,     APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                    ARG_V(0),
                                                    ARG_V(0) } },
  { ID_TEXT_HINT,     APPW_SET_PROP_FONT,         { ARG_VPF(0, acRoboto_16_Normal_EXT_AA4, &APPW__aFont[4]) } },
  { ID_ROTARY_ACTUAL, APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, NULL),
                                                    ARG_VP(0, acIndicatorActual_18x2), } },
  { ID_ROTARY_ACTUAL, APPW_SET_PROP_POS,          { ARG_V(115) } },
  { ID_ROTARY_ACTUAL, APPW_SET_PROP_RANGE,        { ARG_V(1500),
                                                    ARG_V(1500) } },
  { ID_ROTARY_ACTUAL, APPW_SET_PROP_SPAN,         { ARG_V(1000),
                                                    ARG_V(3000) } },
  { ID_ROTARY_ACTUAL, APPW_SET_PROP_OFFSET,       { ARG_V(900) } },
  { ID_ROTARY_ACTUAL, APPW_SET_PROP_ROTATE,       { ARG_V(0) } },
  { ID_ROTARY_TARGET, APPW_SET_PROP_SBITMAPS,     { ARG_VP(0, NULL),
                                                    ARG_VP(0, acIndicatorTarget_24x3), } },
  { ID_ROTARY_TARGET, APPW_SET_PROP_POS,          { ARG_V(111) } },
  { ID_ROTARY_TARGET, APPW_SET_PROP_ROTATE,       { ARG_V(0) } },
  { ID_ROTARY_TARGET, APPW_SET_PROP_OFFSET,       { ARG_V(900) } },
  { ID_ROTARY_TARGET, APPW_SET_PROP_RANGE,        { ARG_V(1500),
                                                    ARG_V(1500) } },
  { ID_ROTARY_TARGET, APPW_SET_PROP_SPAN,         { ARG_V(1000),
                                                    ARG_V(3000) } },
};

/*********************************************************************
*
*       _aAction
*/
static GUI_CONST_STORAGE APPW_ACTION_ITEM _aAction[] = {
  { ID_ROTARY_TARGET, WM_NOTIFICATION_VALUE_CHANGED,    ID_BOX,           APPW_JOB_SETCOLOR,       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR,
    { ARG_V(GUI_BLACK),
      ARG_V(0),
    }, 0, NULL
  },
  { ID_ROTARY_ACTUAL, WM_NOTIFICATION_VALUE_CHANGED,    ID_BOX,           APPW_JOB_SETCOLOR,       ID_SCREEN_00__ID_ROTARY_ACTUAL__WM_NOTIFICATION_VALUE_CHANGED__ID_BOX__APPW_JOB_SETCOLOR,
    { ARG_V(GUI_BLACK),
      ARG_V(0),
    }, 0, NULL
  },
  { ID_ROTARY_TARGET, WM_NOTIFICATION_VALUE_CHANGED,    ID_TEXT_L,        APPW_JOB_SETVALUE,       ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_VALUE_CHANGED__ID_TEXT_L__APPW_JOB_SETVALUE,
    { ARG_V(0),
    }, 0, NULL
  },
  { ID_SCREEN_00,     APPW_NOTIFICATION_INITDIALOG,     ID_ROTARY_TARGET, APPW_JOB_SETVALUE,       ID_SCREEN_00__APPW_NOTIFICATION_INITDIALOG__ID_ROTARY_TARGET__APPW_JOB_SETVALUE,
    { ARG_V(2001),
    }, 0, NULL
  },
  { ID_ROTARY_TARGET, WM_NOTIFICATION_RELEASED,         ID_ROTARY_TARGET, APPW_JOB_SETENABLE,      ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_RELEASED__ID_ROTARY_TARGET__APPW_JOB_SETENABLE,
    { ARG_V(APPW_SET_OFF),
    }, 0, NULL
  },
  { ID_ROTARY_TARGET, WM_NOTIFICATION_CLICKED,          ID_BOX,           APPW_JOB_SETENABLE,      ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_CLICKED__ID_BOX__APPW_JOB_SETENABLE,
    { ARG_V(APPW_SET_OFF),
    }, 0, NULL
  },
  { ID_ROTARY_TARGET, WM_NOTIFICATION_MOTION_STOPPED,   ID_BOX,           APPW_JOB_SETENABLE,      ID_SCREEN_00__ID_ROTARY_TARGET__WM_NOTIFICATION_MOTION_STOPPED__ID_BOX__APPW_JOB_SETENABLE,
    { ARG_V(APPW_SET_OFF),
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

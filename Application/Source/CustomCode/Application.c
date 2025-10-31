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
File        : Application.c
Purpose     : Content to be managed by customer
---------------------------END-OF-HEADER------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WM.h"

#include "../Generated/ID_SCREEN_00.h"
#include "Application.h"
#include "Bitmaps.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Outer radius and center
//
//#define R0 190
//#define MX 195
//#define MY 195
#define R0 123
#define MX 160
#define MY 120

//
// Value of pi
//
#define APP_PI 3.1415926f

//
// MIN and MAX in hundredth of degrees centigrade
//
#define MIN100 1000
#define MAX100 3000

//
// Period of dot animation
//
#define PERIOD_DOT_ANIM 500

//
// Maximum depth for glowing
//
#define MAX_DEPTH_GLOW   64
#define PERIOD_GLOW    2000

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// Application context
//
typedef struct {
  //
  // Handles of objects
  //
  WM_HWIN hTextS;          // Small actual temperature
  WM_HWIN hTextS5;         // Small '5'
  WM_HWIN hTextL5;         // Large '5'
  WM_HWIN hTextHINT;       // Text hint
  WM_HWIN hBox;            // Background box
  WM_HWIN hImageBG;        // Background image
  WM_HWIN hRotary;         // Rotary showing actual marker
  WM_HWIN hTargetRotary;   // Rotary showing target marker
  WM_HWIN hWinDottedLine;  // Window for drawing animated dotted line
  //
  // Temperature values
  //
  I32 vActual100;  // Actual temperature
  I32 vTarget100;  // Target temperature
  //
  // Temperature animation
  //
  GUI_ANIM_HANDLE hAnimActual;  // Animation handle
  I32 vAnimStart;               // Start value of animation
  //
  // Dot animation
  //
  GUI_ANIM_HANDLE hAnimDots;  // Animation handle
  int IndexDot;               // Index into _aIndex[]
  I32 IndexPos;               // Position of animation
  //
  // Glowing animation
  //
  WM_HWIN hWinGlow;  // Handle of glowing window
  int GlowDelay;     // Delay for first creation
} APP_CONTEXT;

/*********************************************************************
*
*       Static (const) data
*
**********************************************************************
*/
static const GUI_BITMAP * _apBm[] = {
  &bmDot_100_5x5,
  &bmDot_80_5x5,
  &bmDot_60_5x5,
  &bmDot_40_5x5,
  &bmDot_20_5x5,
};

static const U8 _aIndex[] = {
  0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static APP_CONTEXT _Context;

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

#define DEG2RAD(a) ((a) * 3.1415926f / 180.0f)
/*********************************************************************
*
*       _cbScale
*       Draw transparent circular tick marks (240x240)
*/
static void _cbScale(WM_MESSAGE * pMsg) {
  int cx = 120;
  int cy = 120;
  int r_outer = 120;
  int r_inner = 112;

  switch (pMsg->MsgId) {
  case WM_PAINT:
    //GUI_SetBkColor(GUI_TRANSPARENT);
    //GUI_Clear();
    GUI_SetColor(GUI_GRAY);
    for (int deg = 240; deg >= -60; deg -= 3) {
        float rad = DEG2RAD(deg);
        int x1 = cx + cos(rad) * r_inner;
        int y1 = cy - sin(rad) * r_inner;
        int x2 = cx + cos(rad) * r_outer;
        int y2 = cy - sin(rad) * r_outer;
        GUI_DrawLine(x1, y1, x2, y2);
    } 
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       _Abs
*/
static I32 _Abs(I32 v) {
  return v < 0 ? -v : v;
}

/*********************************************************************
*
*       _Value100ToAngle1000
*/
static I32 _Value100ToAngle1000(I32 Value100) {
  return 240000 - (Value100 - 1000) * 150;
}

/*********************************************************************
*
*       _DrawDottedLine
*/
static void _DrawDottedLine(int r, I32 Ang0, I32 Ang1) {
  int Step;
  I32 Diff, Current;
  int x, y;
  I32 SinHQ, CosHQ;
  int Index;

  Diff = Ang1 - Ang0;
  //
  // Calculate step
  //
  if        (Diff > 0) {
    //Step = +3000;
    Step = +6000;
  } else if (Diff < 0) {
    //Step = -3000;
    Step = -6000;
  } else {
    return;
  }
  //
  // Determine first position
  //
  if (Ang1 > Ang0) {
    Current = ((Ang0 + 1500) / 3000) * 3000;
    if (Current < Ang0) {
      Current += 3000;
    }
  } else {
    Current = ((Ang0 - 1500) / 3000) * 3000;
    if (Current > Ang0) {
      Current -= 3000;
    }
  }
  //
  // Draw dots between start- and end position
  //
  Index = _Context.IndexDot;
  while (_Abs(Current - Ang0) < _Abs(Diff)) {
    SinHQ = GUI__SinHQ(Current);
    CosHQ = GUI__CosHQ(Current);
    x = ((MX << 16) + (CosHQ * r) + 32768) >> 16;
    y = ((MY << 16) - (SinHQ * r) + 32768) >> 16;
    GUI_DrawBitmap(_apBm[_aIndex[Index]], x - 3, y - 3);
    Index++;
    if (Index == GUI_COUNTOF(_aIndex)) {
      Index = 0;
    }
    Current += Step;
  }
}

/*********************************************************************
*
*       _cbDottedLine
*/
static void _cbDottedLine(WM_MESSAGE * pMsg) {
  I32 AngleTarget1000;
  I32 AngleActual1000;

  if (pMsg->MsgId != WM_PAINT) {
    WM_DefaultProc(pMsg);
    return;
  }
  AngleTarget1000 = _Value100ToAngle1000(_Context.vTarget100);
  AngleActual1000 = _Value100ToAngle1000(_Context.vActual100);
  //
  // Draw line of animated dots
  //
  _DrawDottedLine(R0 - 15, AngleActual1000, AngleTarget1000);
}

#if 0
/*********************************************************************
*
*       _AnimGlow
*/
static void _AnimGlow(GUI_ANIM_INFO * pInfo, void * pVoid) {
  int * pDepth;

  if (_Context.hWinGlow == 0) {
    printf("_Context.hWinGlow is 0!\n");
    return;
  }

  pDepth = (int *)pVoid;
  if (pInfo->Pos < (GUI_ANIM_RANGE >> 1)) {
    *pDepth = (pInfo->Pos * MAX_DEPTH_GLOW) / (GUI_ANIM_RANGE >> 1);
  } else {
    *pDepth = MAX_DEPTH_GLOW - ((pInfo->Pos - (GUI_ANIM_RANGE >> 1)) * MAX_DEPTH_GLOW) / (GUI_ANIM_RANGE >> 1);
  }
  printf("Depth=%d\n", *pDepth);
  WM_Invalidate(_Context.hWinGlow);
}

/*********************************************************************
*
*       _cbGlow
*
* Purpose:
*   This window reads the content of the target temperature and generates
*   a pulsating glowing effect of the number.
*/
static void _cbGlow(WM_MESSAGE * pMsg) {
  int xSize, ySize;
  int xPos, yPos;
  //OscarYeh U32 * pData;
  //U16 * pData;  //OscarYeh 
  U16 * pDst;  //OscarYeh 
  U32 i, NumPixels, Data;
  static GUI_MEMDEV_Handle hMem;
  static GUI_MEMDEV_Handle hMemBlur;
  static GUI_ANIM_HANDLE hAnim;
  static int Depth;
  GUI_RECT rect = {0, 0, 170-1, 100-1};

  switch (pMsg->MsgId) {
  case WM_TOUCH:
    if (_Context.hWinGlow) {
      WM_DeleteWindow(_Context.hWinGlow);
      _Context.hWinGlow = 0;
    }
    break;
  case WM_CREATE:
    //
    // Create and start animation for glowing depth
    //
    printf("_cbGlow - WM_CREATE\n");
    hAnim = GUI_ANIM_Create(PERIOD_GLOW, 40, &Depth, NULL);
    GUI_ANIM_AddItem(hAnim, 0, PERIOD_GLOW, ANIM_LINEAR, &Depth, _AnimGlow);
    GUI_ANIM_StartEx(hAnim, -1, NULL);
    break;
  case WM_DELETE:
    //
    // Remove all stuff from memory
    //
    printf("_cbGlow - WM_DELETE\n");
    Depth = 0;
    if (hMem) {
      GUI_MEMDEV_Delete(hMem);
      hMem = 0;
    }
    if (hAnim) {
      GUI_ANIM_Stop(hAnim);
      GUI_ANIM_Delete(hAnim);
      hAnim = 0;
    }
    if (hMemBlur) {
      GUI_MEMDEV_Delete(hMemBlur);
      hMemBlur = 0;
    }
    break;
  case WM_PRE_PAINT:
    //
    // Create memory devices
    //
    if (hMem == 0) {
      xSize = WM_GetXSize(pMsg->hWin);
      ySize = WM_GetYSize(pMsg->hWin);
      xPos = WM_GetWindowOrgX(pMsg->hWin);
      yPos = WM_GetWindowOrgY(pMsg->hWin);
      printf("Pos: %d, yPos: %d, xSize: %d, ySize: %d\n", xPos, yPos, xSize, ySize);
      printf("GUI_ALLOC_GetNumFreeBytes: %lu\n", GUI_ALLOC_GetNumFreeBytes());
      //OscarYeh hMem = GUI_MEMDEV_CreateFixed32(xPos, yPos, xSize, ySize);
      hMem = GUI_MEMDEV_CreateFixed(xPos, yPos, xSize, ySize, GUI_MEMDEV_HASTRANS, GUI_MEMDEV_APILIST_16, GUI_COLOR_CONV_565);  //OscarYeh 
      
      GUI_MEMDEV_CopyFromLCD(hMem);
      //OscarYeh pData = (U32 *)GUI_MEMDEV_GetDataPtr(hMem);
      #if 0  //OscarYeh 
      pData = (U16 *)GUI_MEMDEV_GetDataPtr(hMem);  //OscarYeh 
      NumPixels = xSize * ySize;
      //
      // Use expand bw color to alpha value
      //
      for (i = 0; i < NumPixels; i++) {
        Data = *pData;
        *pData++ = (Data & 0x000000FF) * 0x01010101;
      }
      //OscarYeh - start
      for (i = 0; i < NumPixels; i++) {
        Data = *pData;
        // 取出 RGB565 各分量
        U16 r = (Data >> 11) & 0x1F;
        U16 g = (Data >> 5)  & 0x3F;
        U16 b =  Data        & 0x1F;
        // 提高亮度（簡易 Glow 增益）
        r = (r + Depth / 8 > 31) ? 31 : (r + Depth / 8);
        g = (g + Depth / 6 > 63) ? 63 : (g + Depth / 6);
        b = (b + Depth / 8 > 31) ? 31 : (b + Depth / 8);
        *pData++ = (r << 11) | (g << 5) | b;
      }
      //OscarYeh - end
      #endif //OscarYeh 

      //
      // Important: Make the window opaque to optimize performance
      //
      WM_ClrHasTrans(pMsg->hWin);
    }
    //
    // Create blurred memory device for glowing effect
    //
    //OscarYeh hMemBlur = GUI_MEMDEV_CreateBlurredDevice32(hMem, Depth);
    //OscarYeh GUI_MEMDEV_ClearAlpha(hMemBlur, 0);
    //OscarYeh - start
    if (!hMemBlur) {
        hMemBlur = GUI_MEMDEV_CreateFixed(xPos, yPos, xSize, ySize,
                                          GUI_MEMDEV_HASTRANS,
                                          GUI_MEMDEV_APILIST_16,
                                          GUI_COLOR_CONV_565);
        GUI_MEMDEV_CopyFromLCD(hMemBlur);
    }
    GUI_MEMDEV_CopyRect(hMem, hMemBlur, &rect, 0, 0);

    pDst = (U16 *)GUI_MEMDEV_GetDataPtr(hMemBlur);
    NumPixels = (U32)xSize * (U32)ySize;

    for (i = 0; i < NumPixels; i++) {
      U16 pixel = pDst[i];

      // RGB565 解碼
      U16 r = (pixel >> 11) & 0x1F;
      U16 g = (pixel >> 5) & 0x3F;
      U16 b = pixel & 0x1F;

      // 根據 Depth（0~255）亮度增強 (Depth%值週期控制)
      int gain = (Depth % 255) / 32; // 控制亮度振幅
      if (gain < 1) gain = 1;

      r = (r + gain) > 31 ? 31 : (r + gain);
      g = (g + gain * 2) > 63 ? 63 : (g + gain * 2);
      b = (b + gain) > 31 ? 31 : (b + gain);

      // 重新封裝回 RGB565
      pDst[i] = (r << 11) | (g << 5) | b;
    }
    //OscarYeh - end
    break;
  case WM_POST_PAINT:
    //
    // Delete blurred device
    //
    printf("_cbGlow - WM_POST_PAINT\n");
    GUI_MEMDEV_Delete(hMemBlur);
    hMemBlur = 0;
    break;
  case WM_PAINT:
    printf("_cbGlow - WM_PAINT\n");
    GUI_MEMDEV_Write(hMemBlur);
    GUI_MEMDEV_Write(hMem);
    break;
  }
}
#endif

#if 1
static void _AnimGlow(GUI_ANIM_INFO * pInfo, void * pVoid) {
  int *pDepth = (int *)pVoid;
    *pDepth = pInfo->Pos;

  if (pInfo->Pos < (GUI_ANIM_RANGE >> 1)) {
    *pDepth = (pInfo->Pos * MAX_DEPTH_GLOW) / (GUI_ANIM_RANGE >> 1);
  } else {
    *pDepth = MAX_DEPTH_GLOW - ((pInfo->Pos - (GUI_ANIM_RANGE >> 1)) * MAX_DEPTH_GLOW) / (GUI_ANIM_RANGE >> 1);
  }

  if (_Context.hWinGlow) {
        WM_Invalidate(_Context.hWinGlow); // 通知窗口重繪
    }
    printf("_AnimGlow, Value: %d\n", *pDepth);
}

static void _cbGlow(WM_MESSAGE * pMsg) {
    int xSize, ySize;
    int xPos, yPos;
    U16 * pDst;
    U32 i, NumPixels;
    static GUI_MEMDEV_Handle hMem;
    static GUI_MEMDEV_Handle hMemBlur;
    static GUI_ANIM_HANDLE hAnim;
    static int Depth;
    GUI_RECT rect;

    switch (pMsg->MsgId) {
    case WM_TOUCH:
        if (_Context.hWinGlow) {
            WM_DeleteWindow(_Context.hWinGlow);
            _Context.hWinGlow = 0;
        }
        break;

    case WM_CREATE:
        printf("_cbGlow - WM_CREATE\n");
        Depth = 0;
        hAnim = GUI_ANIM_Create(PERIOD_GLOW, 40, &Depth, NULL);
        GUI_ANIM_AddItem(hAnim, 0, PERIOD_GLOW, ANIM_LINEAR, &Depth, (GUI_ANIMATION_FUNC *)_AnimGlow);
        GUI_ANIM_StartEx(hAnim, -1, NULL);
        break;

    case WM_DELETE:
        printf("_cbGlow - WM_DELETE\n");
        Depth = 0;
        if (hMem) { GUI_MEMDEV_Delete(hMem); hMem = 0; }
        if (hMemBlur) { GUI_MEMDEV_Delete(hMemBlur); hMemBlur = 0; }
        if (hAnim) { GUI_ANIM_Stop(hAnim); GUI_ANIM_Delete(hAnim); hAnim = 0; }
        break;

    case WM_PRE_PAINT:
         printf("WM_PRE_PAINT: %d\n", Depth);
        if (!hMem) {
            xSize = WM_GetXSize(pMsg->hWin);
            ySize = WM_GetYSize(pMsg->hWin);
            xPos = WM_GetWindowOrgX(pMsg->hWin);
            yPos = WM_GetWindowOrgY(pMsg->hWin);
            rect.x0 = 0; rect.y0 = 0; rect.x1 = xSize - 1; rect.y1 = ySize - 1;

            // 建立 Memory Device (RGB565)
            hMem = GUI_MEMDEV_CreateFixed(
                xPos, yPos, xSize, ySize,
                GUI_MEMDEV_HASTRANS,
                GUI_MEMDEV_APILIST_16,
                GUI_COLOR_CONV_565
            );
            GUI_MEMDEV_CopyFromLCD(hMem);

            hMemBlur = GUI_MEMDEV_CreateFixed(
                xPos, yPos, xSize, ySize,
                GUI_MEMDEV_HASTRANS,
                GUI_MEMDEV_APILIST_16,
                GUI_COLOR_CONV_565
            );
            GUI_MEMDEV_CopyRect(hMem, hMemBlur, &rect, 0, 0);

            WM_ClrHasTrans(pMsg->hWin);
        }
        
        //GUI_MEMDEV_CopyRect(hMem, hMemBlur, &rect, 0, 0);
        if (hMemBlur) {
        // 更新 blur device 的亮度 (根據 Depth)
        pDst = (U16 *)GUI_MEMDEV_GetDataPtr(hMemBlur);
        xSize = WM_GetXSize(pMsg->hWin);
        ySize = WM_GetYSize(pMsg->hWin);
        NumPixels = (U32)xSize * (U32)ySize;

        for (i = 0; i < NumPixels; i++) {
            U16 pixel = pDst[i];
            U16 r = (pixel >> 11) & 0x1F;
            U16 g = (pixel >> 5)  & 0x3F;
            U16 b = pixel & 0x1F;

            // 線性比例增亮
            r = r + ((31 - r) * Depth / MAX_DEPTH_GLOW);
            g = g + ((63 - g) * Depth / MAX_DEPTH_GLOW);
            b = b + ((31 - b) * Depth / MAX_DEPTH_GLOW);

            pDst[i] = (r << 11) | (g << 5) | b;
        }
        }
        break;

    case WM_PAINT:
        printf("WM_PAINT\n");
        GUI_MEMDEV_Write(hMemBlur);
        GUI_MEMDEV_Write(hMem);
        break;
    }
}
#endif

/*********************************************************************
*
*       _StopAnim
*/
static void _StopAnim(void) {
  if (_Context.hAnimActual) {
    GUI_ANIM_Stop(_Context.hAnimActual);
  }
  if (_Context.hAnimDots) {
    GUI_ANIM_Stop(_Context.hAnimDots);
  }
}

/*********************************************************************
*
*       _CreateGlowingWindow
*/
static void _CreateGlowingWindow(GUI_TIMER_MESSAGE* pTM) {
  if (_Context.hWinGlow == 0) {
    //_Context.hWinGlow = WM_CreateWindowAsChild(125, 135, 175, 120, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbGlow, 0);
    _Context.hWinGlow = WM_CreateWindowAsChild(80, 70, 170, 100, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbGlow, 0);
    _Context.GlowDelay = 0;
  }
  GUI_TIMER_Delete(pTM->hTimer);
}

/*********************************************************************
*
*       _HideTextAndStopAnim
*/
static void _HideTextAndStopAnim(void) {
  if (_Context.vActual100 == _Context.vTarget100) {
    WM_HideWindow(_Context.hTextS);
    WM_HideWindow(_Context.hTextS5);
    WM_HideWindow(_Context.hTextHINT);
    _StopAnim();
    //
    // Create glowing window
    //
    //OscarYeh GUI_TIMER_Create(_CreateGlowingWindow, _Context.GlowDelay, 0, 0);
  }
}

#if 0
/*********************************************************************
*
*       _GetBkColor
*/
static GUI_COLOR _GetBkColor(void) {
  static APPW_SETUP_ITEM Setup;
  int Diff, Intens;
  GUI_COLOR Color;
  void * pVoid;
  U32 FileSize;

  //
  // Make sure right image is drawn in background
  //
  Diff = _Context.vTarget100 - _Context.vActual100;
  if (Diff < 0) {
    pVoid = (void *)acCool_390x390;
    FileSize = acCool_390x390_Size;
  } else {
    pVoid = (void *)acHeat_390x390;
    FileSize = acHeat_390x390_Size;
  }
  if (Setup.aPara[0].p != pVoid) {
    Setup.Command    = APPW_SET_PROP_SBITMAP;
    Setup.aPara[0].p = pVoid;
    Setup.aPara[0].v = 0;
    Setup.aPara[1].v = FileSize;
    WM_OBJECT_IMAGE_Setup(_Context.hImageBG, &Setup);
  }
  //
  // Calculate intensity of darken
  //
  Diff = _Abs(Diff);
  Intens = (Diff * 255) / 1000;
  Intens = Intens > 255 ? 0 : 255 - Intens;
  Color = (U32)Intens << 24;
  return Color;
}
#endif

static GUI_COLOR _GetBkColor(void) {
  int Diff, AbsDiff, Intens;
  float ratio;
  U8 rBase, gBase, bBase;
  U8 r, g, b;

  Diff = _Context.vTarget100 - _Context.vActual100;
  AbsDiff = _Abs(Diff);
  if (AbsDiff > 1000) AbsDiff = 1000;

  if (Diff >= 0) {
    rBase = 0x20; gBase = 0x60; bBase = 0xFF;
  } else {
    rBase = 0xFF; gBase = 0x50; bBase = 0x20;
  }

  ratio = (float)AbsDiff / 1000.0f;   // 0.0~1.0
  if (ratio > 1.0f) ratio = 1.0f;

  r = (U8)(rBase * ratio);
  g = (U8)(gBase * ratio);
  b = (U8)(bBase * ratio);

  Intens = (AbsDiff * 255) / 1000;    
  if (Intens > 255) Intens = 255;

  r = (r * Intens) >> 8;
  g = (g * Intens) >> 8;
  b = (b * Intens) >> 8;

  return GUI_MAKE_COLOR((r << 16) | (g << 8) | b);
}

/*********************************************************************
*
*       _SetTextPos
*/
static void _SetTextPos(void) {
  I32 SinHQ, CosHQ;
  I32 Angle;
  int x, y;

  if (_Context.vActual100 != _Context.vTarget100) {
    //
    // Calculate position
    //
    Angle = _Value100ToAngle1000(_Context.vActual100);
    Angle += (_Context.vActual100 > _Context.vTarget100) ? -8000 : +8000;
    SinHQ = GUI__SinHQ(Angle);
    CosHQ = GUI__CosHQ(Angle);
    x = ((MX << 16) + (CosHQ * (R0 - 16)) + 32768) >> 16;
    y = ((MY << 16) - (SinHQ * (R0 - 16)) + 32768) >> 16;
    //
    // Set text position
    //
    APPW_SetPos(_Context.hTextS,  x - 13, y - 12);
    APPW_SetPos(_Context.hTextS5, x + 13, y - 12);
  } else {
    //
    // Hide items if actual and target temperature are equal
    //
    _HideTextAndStopAnim();
  }
}

/*********************************************************************
*
*       _AnimActual
*/
static void _AnimActual(GUI_ANIM_INFO * pInfo, void * pVoid) {
  APPW_SETUP_ITEM Setup;
  I32 Degree100;

  GUI_USE_PARA(pVoid);
  //
  // Adjust value
  //
  _Context.vActual100 = _Context.vAnimStart + ((_Context.vTarget100 - _Context.vAnimStart) * pInfo->Pos) / GUI_ANIM_RANGE;
  //
  // Set actual temperature
  //
  Setup.Command = APPW_SET_PROP_VALUE;
  Degree100 = _Context.vActual100;
  Setup.aPara[0].v = Degree100 / 100;
  WM_OBJECT_TEXT_Setup(_Context.hTextS, &Setup);
  Setup.aPara[0].v = _Context.vActual100;
  WM_OBJECT_ROTARY_Setup(_Context.hRotary, &Setup);/**/
  //
  // Set visibility of 'S5'
  //
  if ((Degree100 % 100) < 50) {
    WM_HideWindow(_Context.hTextS5);
  } else {
    WM_ShowWindow(_Context.hTextS5);
  }
  //
  // Set background color
  //
  Setup.Command = APPW_SET_PROP_COLOR;
  Setup.aPara[0].v = _GetBkColor();
  WM_OBJECT_BOX_Setup(_Context.hBox, &Setup);
  //
  // Set position of actual temperature
  //
  _SetTextPos();
}

/*********************************************************************
*
*       _AnimDots
*/
static void _AnimDots(GUI_ANIM_INFO * pInfo, void * pVoid) {
  static int LastPos;
  int MaxIndex;

  GUI_USE_PARA(pVoid);
  if (pInfo->State == GUI_ANIM_START) {
    LastPos = 0;
  }
  _Context.IndexPos += pInfo->Pos - LastPos;
  while (_Context.IndexPos > GUI_ANIM_RANGE) {
    _Context.IndexPos -= GUI_ANIM_RANGE;
  }
  MaxIndex = GUI_COUNTOF(_aIndex) - 1;
  _Context.IndexDot = MaxIndex - (MaxIndex * _Context.IndexPos) / GUI_ANIM_RANGE;
  LastPos = pInfo->Pos;
}

/*********************************************************************
*
*       _OnAnimDelete
*/
static void _OnAnimDelete(void * pVoid) {
  GUI_USE_PARA(pVoid);
  _Context.hAnimActual = 0;
  _HideTextAndStopAnim();
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APP_GetBoxColor
*/
void APP_GetBoxColor(APPW_ACTION_ITEM * pAction) {
  pAction->aPara[0].v = _GetBkColor();
}

/*********************************************************************
*
*       APP_OnClicked
*/
void APP_OnClicked(void) {
  _StopAnim();
  //
  // Delete glowing window
  //
  if (_Context.hWinGlow) {
    WM_DeleteWindow(_Context.hWinGlow);
    _Context.hWinGlow = 0;
  }
}

/*********************************************************************
*
*       APP_OnReleased
*/
void APP_OnReleased(void) {
  GUI_TIMER_TIME Period;

  //printf("APP_OnReleased\n");
  //
  // Start animation(s) on demand
  //
  if (_Context.vActual100 != _Context.vTarget100) {
    //
    // Remove animations if already exists
    //
    if (_Context.hAnimActual) {
      GUI_ANIM_Delete(_Context.hAnimActual);
      _Context.hAnimActual = 0;
    }
    if (_Context.hAnimDots) {
      GUI_ANIM_Delete(_Context.hAnimDots);
      _Context.hAnimDots = 0;
    }
    //
    // Start animation of actual temperature
    //
    _Context.vAnimStart = _Context.vActual100;
    Period = _Abs(_Context.vTarget100 - _Context.vActual100) * 10;
    _Context.hAnimActual = GUI_ANIM_Create(Period, 400, NULL, NULL);
    GUI_ANIM_AddItem(_Context.hAnimActual, 0, Period, ANIM_DECEL, NULL, _AnimActual);
    GUI_ANIM_StartEx(_Context.hAnimActual, 1, _OnAnimDelete);
    //
    // Start animation of dots
    //
    Period = PERIOD_DOT_ANIM;
    _Context.hAnimDots = GUI_ANIM_Create(Period, 400, NULL, NULL);
    GUI_ANIM_AddItem(_Context.hAnimDots, 0, Period, ANIM_LINEAR, NULL, _AnimDots);
    GUI_ANIM_StartEx(_Context.hAnimDots, -1, NULL);
  }
}

/*********************************************************************
*
*       APP_OnValueChanged
*/
void APP_OnValueChanged(APPW_ACTION_ITEM * pAction) {
  APPW_SETUP_ITEM Setup;
  I32 Degree100;

  _StopAnim();
  //
  // Remember target temperature
  //
  _Context.vTarget100 = pAction->aPara[0].v;
  if (_Context.vActual100 == 0) {
    //
    // Initial Initialization of actual temperature
    //
    _Context.vActual100 = _Context.vTarget100;
    Setup.Command = APPW_SET_PROP_VALUE;
    Degree100 = _Context.vActual100;
    Setup.aPara[0].v = Degree100 / 100;
    WM_OBJECT_TEXT_Setup(_Context.hTextS, &Setup);
  }
  //
  // Calculate value for text object
  //
  Degree100 = _Context.vTarget100;
  pAction->aPara[0].v = Degree100 / 100;
  //printf("vTarget100: %d, v: %d\n", _Context.vTarget100, pAction->aPara[0].v);
  //
  // Set visibility of 'L5'
  //
  if ((Degree100 % 100) < 50) {
    WM_HideWindow(_Context.hTextL5);
  } else {
    WM_ShowWindow(_Context.hTextL5);
  }
  //
  // Show hint & actual temperature on demand
  //
  if (_Context.vActual100 != _Context.vTarget100) {
    WM_ShowWindow(_Context.hTextHINT);
    WM_ShowWindow(_Context.hTextS);
    Degree100 = _Context.vActual100;
    //
    // Set visibility of 'S5'
    //
    if ((Degree100 % 100) >= 50) {
      WM_ShowWindow(_Context.hTextS5);
    }
  }
  //
  // Set position of actual temperature
  //
  _SetTextPos();
}

/*********************************************************************
*
*       APP_OnInitDialog
*/
void APP_OnInitDialog(WM_HWIN hScreen) {
  WM_HWIN hScale = WM_GetDialogItem(hScreen, ID_IMAGE_SCALE);
  WM_SetCallback(hScale, _cbScale);

  //
  // Get object Id's
  //
  _Context.hTextL5   = WM_GetDialogItem(hScreen, ID_TEXT_L5);
  _Context.hTextS    = WM_GetDialogItem(hScreen, ID_TEXT_S);
  _Context.hTextS5   = WM_GetDialogItem(hScreen, ID_TEXT_S5);
  _Context.hTextHINT = WM_GetDialogItem(hScreen, ID_TEXT_HINT);
  _Context.hBox      = WM_GetDialogItem(hScreen, ID_BOX);
  _Context.hImageBG  = WM_GetDialogItem(hScreen, ID_IMAGE_BG);
  _Context.hRotary   = WM_GetDialogItem(hScreen, ID_ROTARY_ACTUAL);
  _Context.hTargetRotary = WM_GetDialogItem(hScreen, ID_ROTARY_TARGET);
  _Context.GlowDelay = 500;
  //
  // Create window responsible for drawing animated dotted line
  //
  //_Context.hWinDottedLine = WM_CreateWindowAsChild(0, 0, 390, 390, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbDottedLine, 0);
  _Context.hWinDottedLine = WM_CreateWindowAsChild(0, 0, 320, 240, WM_HBKWIN, WM_CF_SHOW | WM_CF_HASTRANS, _cbDottedLine, 0);
  WM_SetUntouchable(_Context.hWinDottedLine, 1);
  //
  // Hide 'S', 'S5' and hint
  //
  WM_HideWindow(_Context.hTextS);
  WM_HideWindow(_Context.hTextS5);
  WM_HideWindow(_Context.hTextHINT);
}

/*********************************************************************
*
*       APP_Target_Rotary_Clicked
*/
void APP_Target_Rotary_Clicked(void) {
  WM_NotifyParent(_Context.hTargetRotary, WM_NOTIFICATION_CLICKED);
}

/*********************************************************************
*
*       APP_Target_Rotary_Released
*/
void APP_Target_Rotary_Released(void) {
  WM_NotifyParent(_Context.hTargetRotary, WM_NOTIFICATION_RELEASED);
}

/*********************************************************************
*
*       APP_Target_Rotary_SetValue
*/
void APP_Target_Rotary_SetValue(I32 Value) {
  ROTARY_SetValue(_Context.hTargetRotary, Value);
  WM_NotifyParent(_Context.hTargetRotary, WM_NOTIFICATION_VALUE_CHANGED);
}

/*********************************************************************
*
*       APP_Target_Rotary_GetValue
*/
I32 APP_Target_Rotary_GetValue(void) {
  return ROTARY_GetValue(_Context.hTargetRotary);
}

/*************************** End of file ****************************/

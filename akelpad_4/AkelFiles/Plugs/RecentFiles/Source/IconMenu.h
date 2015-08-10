/******************************************************************
 *                 IconMenu functions header v2.8                 *
 *                                                                *
 *  2015 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                                *
 *                                                                *
 *           Header provide support for images in menu.           *
 *                                                                *
 ******************************************************************/

#ifndef _ICONMENU_H_
#define _ICONMENU_H_

//Defines
#define ICONMENU_CXCHECKMARK             7
#define ICONMENU_CYCHECKMARK             7
#define ICONMENU_CXICON                  16
#define ICONMENU_CYICON                  16
#define ICONMENU_CXICONMARGIN            2
#define ICONMENU_CYICONMARGIN            2
#define ICONMENU_FROMMENUEDGE_TOICONEDGE 1
#define ICONMENU_FROMICONEDGE_TOSTRING   4
#define ICONMENU_FROMSTRING_TOTAB        12
#define ICONMENU_FROMTAB_TOMENUEDGE      9

#define ICONMENU_SETMENU_UNICODE   0x1
#define ICONMENU_SETMENU_ANSI      0x2
#define ICONMENU_SETMENU_INSERT    0x4
#define ICONMENU_SETMENU_MODIFY    0x8

#ifndef SPI_GETKEYBOARDCUES
  #define SPI_GETKEYBOARDCUES 0x100A
#endif
#ifndef DSS_HIDEPREFIX
  #define DSS_HIDEPREFIX 0x0200
#endif
#ifndef WM_THEMECHANGED
  #define WM_THEMECHANGED 0x031A
#endif


//Structures
DECLARE_HANDLE (HICONMENU);
DECLARE_HANDLE (HICONSUBMENU);
#ifndef HTHEME
  //uxtheme.h
  typedef HANDLE HTHEME;
  typedef BOOL (WINAPI *ISAPPTHEMED)(void);
  typedef HTHEME (WINAPI *OPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
  typedef HRESULT (WINAPI *CLOSETHEMEDATA)(HTHEME IconMenu_hUxTheme);
  typedef HRESULT (WINAPI *GETTHEMEPARTSIZE)(HTHEME IconMenu_hUxTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, int eSize, SIZE *psz);
  typedef HRESULT (WINAPI *DRAWTHEMEBACKGROUND)(HTHEME IconMenu_hUxTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
#endif

typedef struct _ICONMENUITEM {
  struct _ICONMENUITEM *next;
  struct _ICONMENUITEM *prev;
  HICONSUBMENU hIconSubMenu; //ICONMENUSUBMENU pointer
  HIMAGELIST hImageList;
  INT_PTR nIconIndex;
  int nIconWidth;
  int nIconHeight;
  int nItemHeight;
  UINT uFlags;
  INT_PTR nItemID;
  wchar_t wszStr[MAX_PATH];
  int nStrLen;
  int nStrWidth;
  wchar_t *wpTab;
  int nTabLen;
  int nTabWidth;
  char chKey;
  wchar_t wchKey;
} ICONMENUITEM;

typedef struct _ICONMENUSUBMENU {
  struct _ICONMENUSUBMENU *next;
  struct _ICONMENUSUBMENU *prev;
  HICONMENU hIconMenu; //ICONMENUHANDLE pointer
  ICONMENUITEM *first;
  ICONMENUITEM *last;
  HMENU hMenu;
  int nItemCount;
  int nStrWidthMax;
  int nTabWidthMax;
  int nTextOffsetMax;
} ICONMENUSUBMENU;

typedef struct _ICONMENUHANDLE {
  struct _ICONMENUHANDLE *next;
  struct _ICONMENUHANDLE *prev;
  ICONMENUSUBMENU *first;
  ICONMENUSUBMENU *last;
  HWND hWndOwner;
  HFONT hFont;
  int nTextHeight;
} ICONMENUHANDLE;

typedef struct {
  ICONMENUHANDLE *first;
  ICONMENUHANDLE *last;
} ICONMENUSTACK;

//Internal functions prototypes
void IconMenu_Initialize(HWND hWnd);
void IconMenu_Uninitialize();
HFONT IconMenu_GetMenuFont();
ICONMENUSUBMENU* IconMenu_GetMenuByHandle(HICONMENU hIconMenu, HMENU hMenu);
BOOL IconMenu_IsItem(HICONMENU hIconMenu, ICONMENUITEM *lpItem);
ICONMENUITEM* IconMenu_GetItemById(HICONMENU hIconMenu, INT_PTR nItemID);
ICONMENUITEM* IconMenu_GetSubMenuItem(ICONMENUSUBMENU *lpSubMenu, INT_PTR nItemID, UINT uFlags);
void IconMenu_GetSubMenuSize(ICONMENUSUBMENU *lpSubMenu);
void IconMenu_RoundRect(HDC hDC, RECT *rcRect, COLORREF crEdge, COLORREF crBk);
HICON IconMenu_MixIcons(HDC hDC, HICON hIconInput, HICON hIconOverlay, BOOL bWhiteInOverlayAsMaskInInput);
HICON IconMenu_AdjustIcon(HDC hDC, HICON hIcon, BOOL bGrayscale, int nBrightnessPercent, int nContrastPercent);
COLORREF IconMenu_Grayscale(COLORREF crColor);
COLORREF IconMenu_Brightness(COLORREF crColor, int nPercent);
COLORREF IconMenu_Contrast(COLORREF crColor, int nPercent);

//External functions prototypes
HICONMENU IconMenu_Alloc(HWND hWnd);
void IconMenu_Free(HICONMENU hIconMenu, HMENU hMenu);
BOOL IconMenu_AddItemA(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const char *lpNewItem);
BOOL IconMenu_AddItemW(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *lpNewItem);
BOOL IconMenu_ModifyItemA(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const char *lpNewItem);
BOOL IconMenu_ModifyItemW(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *lpNewItem);
BOOL IconMenu_SetItem(DWORD dwFlags, HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const void *lpNewItem);
BOOL IconMenu_DeleteItem(HICONMENU hIconMenu, HMENU hMenu, INT_PTR nPosition, UINT uFlags);
BOOL IconMenu_RemoveItem(HICONMENU hIconMenu, HMENU hMenu, INT_PTR nPosition, UINT uFlags);
BOOL IconMenu_UnsetItem(HICONMENU hIconMenu, HMENU hMenu, INT_PTR nPosition, UINT uFlags, BOOL bDeleteSubmenu);
LRESULT IconMenu_Messages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//Global variables
extern BOOL IconMenu_bInitialized;
extern BOOL IconMenu_bOldWindows;
extern BOOL IconMenu_bIsThemed;
extern BOOL IconMenu_bMouseMenu;
extern ICONMENUSTACK IconMenu_hStackMenuHandles;

#endif //_ICONMENU_H_


#ifdef ICONMENU_INCLUDE
#ifndef ICONMENU_INCLUDED
#define ICONMENU_INCLUDED


//// Global variables

BOOL IconMenu_bInitialized=-1;
BOOL IconMenu_bOldWindows=-1;
BOOL IconMenu_bMouseMenu=FALSE;
ICONMENUSTACK IconMenu_hStackMenuHandles={0};
BOOL IconMenu_bIsThemed=FALSE;
HMODULE IconMenu_hUxModule=NULL;
HTHEME IconMenu_hUxTheme=NULL;
ISAPPTHEMED IconMenu_IsAppThemed=NULL;
OPENTHEMEDATA IconMenu_OpenThemeData=NULL;
CLOSETHEMEDATA IconMenu_CloseThemeData=NULL;
GETTHEMEPARTSIZE IconMenu_GetThemePartSize=NULL;
DRAWTHEMEBACKGROUND IconMenu_DrawThemeBackground=NULL;


//// Internal functions

void IconMenu_Initialize(HWND hWnd)
{
  if (IconMenu_bInitialized <= 0)
  {
    IconMenu_bInitialized=TRUE;
    IconMenu_bOldWindows=!GetWindowsDirectoryW(NULL, 0);
    if (!IconMenu_bOldWindows)
    {
      if (!IconMenu_hUxModule)
        IconMenu_hUxModule=GetModuleHandleW(L"uxtheme.dll");

      if (IconMenu_hUxModule)
      {
        if (!IconMenu_IsAppThemed)
          IconMenu_IsAppThemed=(ISAPPTHEMED)GetProcAddress(IconMenu_hUxModule, "IsAppThemed");
        if (!IconMenu_OpenThemeData)
          IconMenu_OpenThemeData=(OPENTHEMEDATA)GetProcAddress(IconMenu_hUxModule, "OpenThemeData");
        if (!IconMenu_GetThemePartSize)
          IconMenu_GetThemePartSize=(GETTHEMEPARTSIZE)GetProcAddress(IconMenu_hUxModule, "GetThemePartSize");
        if (!IconMenu_DrawThemeBackground)
          IconMenu_DrawThemeBackground=(DRAWTHEMEBACKGROUND)GetProcAddress(IconMenu_hUxModule, "DrawThemeBackground");
        if (!IconMenu_CloseThemeData)
          IconMenu_CloseThemeData=(CLOSETHEMEDATA)GetProcAddress(IconMenu_hUxModule, "CloseThemeData");

        IconMenu_bIsThemed=IconMenu_IsAppThemed();
        if (IconMenu_bIsThemed && !IconMenu_hUxTheme)
          IconMenu_hUxTheme=IconMenu_OpenThemeData(hWnd, L"MENU");
      }
    }
  }
}

void IconMenu_Uninitialize()
{
  if (IconMenu_bInitialized == TRUE)
  {
    IconMenu_bInitialized=FALSE;

    if (IconMenu_hUxTheme)
    {
      IconMenu_CloseThemeData(IconMenu_hUxTheme);
      IconMenu_hUxTheme=NULL;
    }
  }
}

HFONT IconMenu_GetMenuFont()
{
  if (IconMenu_bOldWindows == TRUE)
  {
    NONCLIENTMETRICSA ncm;

    ncm.cbSize=sizeof(NONCLIENTMETRICSA);
    SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSA), &ncm, 0);
    return CreateFontIndirectA(&ncm.lfMenuFont);
  }
  else if (IconMenu_bOldWindows == FALSE)
  {
    NONCLIENTMETRICSW ncm;

    ncm.cbSize=sizeof(NONCLIENTMETRICSW);
    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICSW), &ncm, 0);
    return CreateFontIndirectW(&ncm.lfMenuFont);
  }
  else
  {
    if (IconMenu_bInitialized == -1)
    {
      IconMenu_bInitialized=FALSE;
      MessageBoxW(NULL, L"IconMenu_Initialize() required.", L"IconMenu.h header", MB_ICONERROR|MB_OK);
    }
  }
  return NULL;
}

ICONMENUSUBMENU* IconMenu_GetMenuByHandle(HICONMENU hIconMenu, HMENU hMenu)
{
  ICONMENUHANDLE *lpMenuHandle=(ICONMENUHANDLE *)hIconMenu;
  ICONMENUSUBMENU *lpSubMenu;

  if (!lpMenuHandle)
    lpMenuHandle=(ICONMENUHANDLE *)IconMenu_hStackMenuHandles.first;

  while (lpMenuHandle)
  {
    for (lpSubMenu=lpMenuHandle->first; lpSubMenu; lpSubMenu=lpSubMenu->next)
    {
      if (lpSubMenu->hMenu == hMenu)
      {
        //Move submenu in first place in their group for faster further access
        StackMoveBefore((stack **)&lpMenuHandle->first, (stack **)&lpMenuHandle->last, (stack *)lpSubMenu, (stack *)lpMenuHandle->first);
        return lpSubMenu;
      }
    }
    if (hIconMenu) break;

    lpMenuHandle=lpMenuHandle->next;
  }
  return NULL;
}

BOOL IconMenu_IsItem(HICONMENU hIconMenu, ICONMENUITEM *lpItem)
{
  ICONMENUHANDLE *lpMenuHandle=(ICONMENUHANDLE *)hIconMenu;
  ICONMENUSUBMENU *lpSubMenu;
  ICONMENUITEM *lpMenuItem;

  if (!lpItem)
    return FALSE;
  if (!lpMenuHandle)
    lpMenuHandle=(ICONMENUHANDLE *)IconMenu_hStackMenuHandles.first;

  while (lpMenuHandle)
  {
    for (lpSubMenu=lpMenuHandle->first; lpSubMenu; lpSubMenu=lpSubMenu->next)
    {
      for (lpMenuItem=lpSubMenu->first; lpMenuItem; lpMenuItem=lpMenuItem->next)
      {
        if (lpMenuItem == lpItem)
        {
          //Move handle and submenu in first place in their group for faster further access
          StackMoveBefore((stack **)&IconMenu_hStackMenuHandles.first, (stack **)&IconMenu_hStackMenuHandles.last, (stack *)lpMenuHandle, (stack *)IconMenu_hStackMenuHandles.first);
          StackMoveBefore((stack **)&lpMenuHandle->first, (stack **)&lpMenuHandle->last, (stack *)lpSubMenu, (stack *)lpMenuHandle->first);
          return TRUE;
        }
      }
    }
    if (hIconMenu) break;

    lpMenuHandle=lpMenuHandle->next;
  }
  return FALSE;
}

ICONMENUITEM* IconMenu_GetItemById(HICONMENU hIconMenu, INT_PTR nItemID)
{
  ICONMENUHANDLE *lpMenuHandle=(ICONMENUHANDLE *)hIconMenu;
  ICONMENUSUBMENU *lpSubMenu;
  ICONMENUITEM *lpMenuItem;

  if (!lpMenuHandle)
    lpMenuHandle=(ICONMENUHANDLE *)IconMenu_hStackMenuHandles.first;

  while (lpMenuHandle)
  {
    for (lpSubMenu=lpMenuHandle->first; lpSubMenu; lpSubMenu=lpSubMenu->next)
    {
      if (lpMenuItem=IconMenu_GetSubMenuItem(lpSubMenu, nItemID, MF_BYCOMMAND))
      {
        //Move handle and submenu in first place in their group for faster further access
        StackMoveBefore((stack **)&IconMenu_hStackMenuHandles.first, (stack **)&IconMenu_hStackMenuHandles.last, (stack *)lpMenuHandle, (stack *)IconMenu_hStackMenuHandles.first);
        StackMoveBefore((stack **)&lpMenuHandle->first, (stack **)&lpMenuHandle->last, (stack *)lpSubMenu, (stack *)lpMenuHandle->first);
        return lpMenuItem;
      }
    }
    if (hIconMenu) break;

    lpMenuHandle=lpMenuHandle->next;
  }
  return NULL;
}

ICONMENUITEM* IconMenu_GetSubMenuItem(ICONMENUSUBMENU *lpSubMenu, INT_PTR nItemID, UINT uFlags)
{
  ICONMENUITEM *lpMenuItem;
  INT_PTR nIndex=0;

  if ((uFlags & MF_BYPOSITION) && nItemID == -1)
    return NULL;

  for (lpMenuItem=lpSubMenu->first; lpMenuItem; lpMenuItem=lpMenuItem->next)
  {
    if (uFlags & MF_BYPOSITION)
    {
      if (nItemID == nIndex)
        return lpMenuItem;
    }
    else
    {
      if (nItemID == lpMenuItem->nItemID)
        return lpMenuItem;
    }
    ++nIndex;
  }
  return NULL;
}

void IconMenu_GetSubMenuSize(ICONMENUSUBMENU *lpSubMenu)
{
  ICONMENUHANDLE *lpMenuHandle=(ICONMENUHANDLE *)lpSubMenu->hIconMenu;
  ICONMENUITEM *lpMenuItem;
  HFONT hMenuFontOld;
  HDC hDC;
  SIZE sizeWidth={0};

  lpSubMenu->nStrWidthMax=0;
  lpSubMenu->nTabWidthMax=0;
  lpSubMenu->nTextOffsetMax=GetSystemMetrics(SM_CXMENUCHECK) + ICONMENU_FROMICONEDGE_TOSTRING;

  if (hDC=GetDC(lpMenuHandle->hWndOwner))
  {
    hMenuFontOld=(HFONT)SelectObject(hDC, lpMenuHandle->hFont);

    for (lpMenuItem=lpSubMenu->first; lpMenuItem; lpMenuItem=lpMenuItem->next)
    {
      if (lpMenuItem->nIconIndex != -1 || IconMenu_hUxTheme)
        lpSubMenu->nTextOffsetMax=max(lpSubMenu->nTextOffsetMax, ICONMENU_FROMMENUEDGE_TOICONEDGE + ICONMENU_CXICONMARGIN + lpMenuItem->nIconWidth + ICONMENU_CXICONMARGIN + ICONMENU_FROMICONEDGE_TOSTRING);

      //String width
      if (!lpMenuItem->nStrWidth && lpMenuItem->nStrLen)
      {
        GetTextExtentPoint32W(hDC, lpMenuItem->wszStr, lpMenuItem->nStrLen, &sizeWidth);
        lpMenuItem->nStrWidth=sizeWidth.cx;
      }
      lpSubMenu->nStrWidthMax=max(lpMenuItem->nStrWidth, lpSubMenu->nStrWidthMax);

      //Tabbed text width
      if (!lpMenuItem->nTabWidth && lpMenuItem->nTabLen)
      {
        GetTextExtentPoint32W(hDC, lpMenuItem->wpTab, lpMenuItem->nTabLen, &sizeWidth);
        lpMenuItem->nTabWidth=sizeWidth.cx;
      }
      lpSubMenu->nTabWidthMax=max(lpMenuItem->nTabWidth, lpSubMenu->nTabWidthMax);

      //Item height
      if (!lpMenuItem->nItemHeight)
      {
        if (lpMenuItem->uFlags & MF_SEPARATOR)
          lpMenuItem->nItemHeight=(ICONMENU_CYICONMARGIN + lpMenuHandle->nTextHeight + ICONMENU_CYICONMARGIN) / 2 + 1;
        else
          lpMenuItem->nItemHeight=ICONMENU_CYICONMARGIN + max(lpMenuHandle->nTextHeight, lpMenuItem->nIconHeight) + ICONMENU_CYICONMARGIN;
      }
    }

    if (hMenuFontOld) SelectObject(hDC, hMenuFontOld);
    ReleaseDC(lpMenuHandle->hWndOwner, hDC);
  }
}

void IconMenu_RoundRect(HDC hDC, RECT *rcRect, COLORREF crEdge, COLORREF crBk)
{
  HBRUSH hBrush;
  HBRUSH hBrushOld;
  HPEN hPen;
  HPEN hPenOld;

  if (hBrush=CreateSolidBrush(crBk))
  {
    hBrushOld=(HBRUSH)SelectObject(hDC, hBrush);

    if (hPen=CreatePen(PS_SOLID, 0, crEdge))
    {
      hPenOld=(HPEN)SelectObject(hDC, hPen);

      RoundRect(hDC, rcRect->left, rcRect->top, rcRect->right, rcRect->bottom, 5, 5);

      SelectObject(hDC, hPenOld);
      DeleteObject(hPen);
    }
    SelectObject(hDC, hBrushOld);
    DeleteObject(hBrush);
  }
}

HICON IconMenu_MixIcons(HDC hDC, HICON hIconInput, HICON hIconOverlay, BOOL bWhiteInOverlayAsMaskInInput)
{
  ICONINFO iiInput;
  ICONINFO iiOverlay;
  ICONINFO iiOutput;
  BITMAP bmInput;
  BITMAP bmOverlay;
  BITMAPINFO bmi;
  DWORD dwIconWidth;
  DWORD dwIconHeight;
  HICON hOutputIcon=NULL;
  BYTE *lpInputColorBits;
  BYTE *lpInputMaskBits;
  BYTE *lpOverlayColorBits;
  BYTE *lpOverlayMaskBits;
  DWORD *lpInputColorPixel;
  DWORD *lpInputMaskPixel;
  DWORD *lpOverlayColorPixel;
  DWORD *lpOverlayMaskPixel;
  DWORD x;
  DWORD y;
  INT_PTR nOffset;
  BOOL bFreeDC=FALSE;

  //Get icon bitmaps
  if (!GetIconInfo(hIconInput, &iiInput))
    return NULL;
  if (!GetIconInfo(hIconOverlay, &iiOverlay))
    return NULL;

  if (iiInput.hbmColor && iiInput.hbmMask &&
      iiOverlay.hbmColor && iiOverlay.hbmMask &&
      GetObjectA(iiInput.hbmColor, sizeof(BITMAP), &bmInput) &&
      GetObjectA(iiOverlay.hbmColor, sizeof(BITMAP), &bmOverlay) &&
      bmInput.bmHeight == bmOverlay.bmHeight &&
      bmInput.bmWidth == bmOverlay.bmWidth)
  {
    if (!hDC)
    {
      hDC=GetDC(NULL);
      bFreeDC=TRUE;
    }
    if (hDC)
    {
      dwIconWidth=bmInput.bmWidth;
      dwIconHeight=bmInput.bmHeight;

      bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth=dwIconWidth;
      bmi.bmiHeader.biHeight=dwIconHeight;
      bmi.bmiHeader.biPlanes=1;
      bmi.bmiHeader.biBitCount=32;
      bmi.bmiHeader.biCompression=BI_RGB;
      bmi.bmiHeader.biSizeImage=0;
      bmi.bmiHeader.biXPelsPerMeter=0;
      bmi.bmiHeader.biYPelsPerMeter=0;
      bmi.bmiHeader.biClrUsed=0;
      bmi.bmiHeader.biClrImportant=0;

      GetDIBits(hDC, iiOverlay.hbmColor, 0, bmOverlay.bmHeight, (void *)NULL, &bmi, DIB_RGB_COLORS);
      if (lpOverlayColorBits=(BYTE *)GlobalAlloc(GPTR, bmi.bmiHeader.biSizeImage))
        GetDIBits(hDC, iiOverlay.hbmColor, 0, bmOverlay.bmHeight, (void *)lpOverlayColorBits, &bmi, DIB_RGB_COLORS);
      GetDIBits(hDC, iiOverlay.hbmMask, 0, bmOverlay.bmHeight, (void *)NULL, &bmi, DIB_RGB_COLORS);
      if (lpOverlayMaskBits=(BYTE *)GlobalAlloc(GPTR, bmi.bmiHeader.biSizeImage))
        GetDIBits(hDC, iiOverlay.hbmMask, 0, bmOverlay.bmHeight, (void *)lpOverlayMaskBits, &bmi, DIB_RGB_COLORS);

      GetDIBits(hDC, iiInput.hbmColor, 0, bmInput.bmHeight, (void *)NULL, &bmi, DIB_RGB_COLORS);
      if (lpInputColorBits=(BYTE *)GlobalAlloc(GPTR, bmi.bmiHeader.biSizeImage))
        GetDIBits(hDC, iiInput.hbmColor, 0, bmInput.bmHeight, (void *)lpInputColorBits, &bmi, DIB_RGB_COLORS);
      GetDIBits(hDC, iiInput.hbmMask, 0, bmInput.bmHeight, (void *)NULL, &bmi, DIB_RGB_COLORS);
      if (lpInputMaskBits=(BYTE *)GlobalAlloc(GPTR, bmi.bmiHeader.biSizeImage))
        GetDIBits(hDC, iiInput.hbmMask, 0, bmInput.bmHeight, (void *)lpInputMaskBits, &bmi, DIB_RGB_COLORS);

      if (lpInputColorBits && lpInputMaskBits && lpOverlayColorBits && lpOverlayMaskBits)
      {
        lpOverlayMaskPixel=(DWORD *)lpOverlayMaskBits;

        for (y=0; y < dwIconHeight; ++y)
        {
          for (x=0; x < dwIconWidth; ++x)
          {
            nOffset=(BYTE *)lpOverlayMaskPixel - (BYTE *)lpOverlayMaskBits;
            lpInputColorPixel=(DWORD *)(lpInputColorBits + nOffset);
            lpInputMaskPixel=(DWORD *)(lpInputMaskBits + nOffset);

            if (!*lpOverlayMaskPixel)
            {
              //Copy pixel from Overlay to Input
              lpOverlayColorPixel=(DWORD *)(lpOverlayColorBits + nOffset);

              //Remove alpha and compare with white color
              if (bWhiteInOverlayAsMaskInInput && (*lpOverlayColorPixel & 0x00FFFFFF) == 0xFFFFFF)
              {
                *lpInputColorPixel=0xFFFFFF;
                *lpInputMaskPixel=0xFFFFFF;
              }
              else
              {
                *lpInputColorPixel=*lpOverlayColorPixel;
                *lpInputMaskPixel=0;
              }
            }
            if (!*lpInputMaskPixel)
            {
              //Fill pixel alpha channel, otherwise non 32-bit icon not painted.
              if (!((BYTE *)lpInputColorPixel)[3])
                ((BYTE *)lpInputColorPixel)[3]=0xFF;
            }
            ++lpOverlayMaskPixel;
          }
        }
        SetDIBits(hDC, iiInput.hbmColor, 0, bmInput.bmHeight, (void *)lpInputColorBits, &bmi, DIB_RGB_COLORS);
        SetDIBits(hDC, iiInput.hbmMask, 0, bmInput.bmHeight, (void *)lpInputMaskBits, &bmi, DIB_RGB_COLORS);

        //Create new icon
        iiOutput.xHotspot=iiInput.xHotspot;
        iiOutput.yHotspot=iiInput.yHotspot;
        iiOutput.hbmColor=iiInput.hbmColor;
        iiOutput.hbmMask=iiInput.hbmMask;
        iiOutput.fIcon=TRUE;
        hOutputIcon=CreateIconIndirect(&iiOutput);

        GlobalFree((HGLOBAL)lpInputColorBits);
        GlobalFree((HGLOBAL)lpInputMaskBits);
        GlobalFree((HGLOBAL)lpOverlayColorBits);
        GlobalFree((HGLOBAL)lpOverlayMaskBits);
      }
      if (bFreeDC) ReleaseDC(NULL, hDC);
    }
  }
  DeleteObject(iiInput.hbmColor);
  DeleteObject(iiInput.hbmMask);
  DeleteObject(iiOverlay.hbmColor);
  DeleteObject(iiOverlay.hbmMask);
  return hOutputIcon;
}

HICON IconMenu_AdjustIcon(HDC hDC, HICON hIcon, BOOL bGrayscale, int nBrightnessPercent, int nContrastPercent)
{
  ICONINFO iiInput;
  ICONINFO iiOutput;
  DWORD dwIconWidth;
  DWORD dwIconHeight;
  HICON hOutputIcon=NULL;
  BITMAP bm;
  BITMAPINFO bmi;
  BYTE *lpColorBits;
  BYTE *lpMaskBits;
  BYTE *lpColorPixel;
  BYTE *lpMaskPixel;
  COLORREF crColorPixel;
  DWORD x;
  DWORD y;
  BOOL bFreeDC=FALSE;

  //Get icon bitmaps
  if (!GetIconInfo(hIcon, &iiInput))
    return NULL;

  if (iiInput.hbmColor && iiInput.hbmMask &&
      GetObjectA(iiInput.hbmColor, sizeof(BITMAP), &bm))
  {
    if (!hDC)
    {
      hDC=GetDC(NULL);
      bFreeDC=TRUE;
    }
    if (hDC)
    {
      dwIconWidth=bm.bmWidth;
      dwIconHeight=bm.bmHeight;

      bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth=dwIconWidth;
      bmi.bmiHeader.biHeight=dwIconHeight;
      bmi.bmiHeader.biPlanes=1;
      bmi.bmiHeader.biBitCount=32;
      bmi.bmiHeader.biCompression=BI_RGB;
      bmi.bmiHeader.biSizeImage=0;
      bmi.bmiHeader.biXPelsPerMeter=0;
      bmi.bmiHeader.biYPelsPerMeter=0;
      bmi.bmiHeader.biClrUsed=0;
      bmi.bmiHeader.biClrImportant=0;

      GetDIBits(hDC, iiInput.hbmColor, 0, dwIconHeight, (void *)NULL, &bmi, DIB_RGB_COLORS);
      if (lpColorBits=(BYTE *)GlobalAlloc(GPTR, bmi.bmiHeader.biSizeImage))
        GetDIBits(hDC, iiInput.hbmColor, 0, dwIconHeight, (void *)lpColorBits, &bmi, DIB_RGB_COLORS);
      GetDIBits(hDC, iiInput.hbmMask, 0, dwIconHeight, (void *)NULL, &bmi, DIB_RGB_COLORS);
      if (lpMaskBits=(BYTE *)GlobalAlloc(GPTR, bmi.bmiHeader.biSizeImage))
        GetDIBits(hDC, iiInput.hbmMask, 0, dwIconHeight, (void *)lpMaskBits, &bmi, DIB_RGB_COLORS);

      if (lpColorBits && lpMaskBits)
      {
        lpColorPixel=lpColorBits;
        lpMaskPixel=lpMaskBits;

        for (y=0; y < dwIconHeight; ++y)
        {
          for (x=0; x < dwIconWidth; ++x)
          {
            if (!lpMaskPixel[0])
            {
              crColorPixel=RGB(lpColorPixel[2], lpColorPixel[1], lpColorPixel[0]);

              //Change brightness and contrast of pixel
              if (bGrayscale) crColorPixel=IconMenu_Grayscale(crColorPixel);
              crColorPixel=IconMenu_Brightness(crColorPixel, nBrightnessPercent);
              crColorPixel=IconMenu_Contrast(crColorPixel, nContrastPercent);

              lpColorPixel[0]=GetBValue(crColorPixel);
              lpColorPixel[1]=GetGValue(crColorPixel);
              lpColorPixel[2]=GetRValue(crColorPixel);

              //Alpha channel
              //lpColorPixel[3]=0;
            }
            lpColorPixel+=4;
            lpMaskPixel+=4;
          }
        }
        SetDIBits(hDC, iiInput.hbmColor, 0, dwIconHeight, (void *)lpColorBits, &bmi, DIB_RGB_COLORS);

        //Create new icon
        iiOutput.xHotspot=iiInput.xHotspot;
        iiOutput.yHotspot=iiInput.yHotspot;
        iiOutput.hbmColor=iiInput.hbmColor;
        iiOutput.hbmMask=iiInput.hbmMask;
        iiOutput.fIcon=TRUE;
        hOutputIcon=CreateIconIndirect(&iiOutput);

        GlobalFree((HGLOBAL)lpColorBits);
        GlobalFree((HGLOBAL)lpMaskBits);
      }
      if (bFreeDC) ReleaseDC(NULL, hDC);
    }
  }
  DeleteObject(iiInput.hbmColor);
  DeleteObject(iiInput.hbmMask);
  return hOutputIcon;
}

COLORREF IconMenu_Grayscale(COLORREF crColor)
{
  int nGray;

  nGray=(GetRValue(crColor) * 30 + GetGValue(crColor) * 59 + GetBValue(crColor) * 11) / 100;
  nGray=min(nGray, 255);
  nGray=max(nGray, 0);
  return RGB(nGray, nGray, nGray);
}

COLORREF IconMenu_Brightness(COLORREF crColor, int nPercent)
{
  BYTE r=GetRValue(crColor);
  BYTE g=GetGValue(crColor);
  BYTE b=GetBValue(crColor);

  if (nPercent > 0 && nPercent <= 100)
  {
    r=(BYTE)(r + (nPercent * (255 - r) / 100));
    g=(BYTE)(g + (nPercent * (255 - g) / 100));
    b=(BYTE)(b + (nPercent * (255 - b) / 100));
  }
  else if (nPercent >= -100 && nPercent < 0)
  {
    r=(BYTE)(r + (nPercent * r / 100));
    g=(BYTE)(g + (nPercent * g / 100));
    b=(BYTE)(b + (nPercent * b / 100));
  }
  return RGB(r, g, b);
}

COLORREF IconMenu_Contrast(COLORREF crColor, int nPercent)
{
  int r=GetRValue(crColor);
  int g=GetGValue(crColor);
  int b=GetBValue(crColor);

  //Percent only positive
  nPercent+=100;
  nPercent=max(nPercent, 0);

  r=128 + (nPercent * (r - 128) / 100);
  r=min(r, 255);
  r=max(r, 0);
  g=128 + (nPercent * (g - 128) / 100);
  g=min(g, 255);
  g=max(g, 0);
  b=128 + (nPercent * (b - 128) / 100);
  b=min(b, 255);
  b=max(b, 0);
  return RGB(r, g, b);
}


//// External functions

/********************************************************************
 *
 *  IconMenu_Alloc
 *
 *Create icon menu handle.
 *
 * [in] HWND hWnd -menu owner.
 *
 *Returns: allocated icon menu handle.
 ********************************************************************/
HICONMENU IconMenu_Alloc(HWND hWnd)
{
  ICONMENUHANDLE *lpMenuHandle=NULL;

  IconMenu_Initialize(hWnd);
  if (!StackInsertAfter((stack **)&IconMenu_hStackMenuHandles.first, (stack **)&IconMenu_hStackMenuHandles.last, (stack *)IconMenu_hStackMenuHandles.last, (stack **)&lpMenuHandle, sizeof(ICONMENUHANDLE)))
    lpMenuHandle->hWndOwner=hWnd;
  return (HICONMENU)lpMenuHandle;
}

/********************************************************************
 *
 *  IconMenu_Free
 *
 *Frees memory associated with specified submenu. Function doesn't destroy submenu.
 *
 * [in] HICONMENU hIconMenu -handle returned by IconMenu_Alloc.
 * [in] HMENU hMenu         -submenu handle. If NULL, free all submenus and invalidates hIconMenu handle.
 *
 *Returns: no return value.
 ********************************************************************/
void IconMenu_Free(HICONMENU hIconMenu, HMENU hMenu)
{
  ICONMENUHANDLE *lpMenuHandle=(ICONMENUHANDLE *)hIconMenu;
  ICONMENUSUBMENU *lpSubMenu;

  if (hMenu)
  {
    if (lpSubMenu=IconMenu_GetMenuByHandle(hIconMenu, hMenu))
    {
      StackClear((stack **)&lpSubMenu->first, (stack **)&lpSubMenu->last);
      StackDelete((stack **)&lpMenuHandle->first, (stack **)&lpMenuHandle->last, (stack *)lpSubMenu);
    }
  }
  else
  {
    for (lpSubMenu=lpMenuHandle->first; lpSubMenu; lpSubMenu=lpSubMenu->next)
    {
      StackClear((stack **)&lpSubMenu->first, (stack **)&lpSubMenu->last);
    }
    StackClear((stack **)&lpMenuHandle->first, (stack **)&lpMenuHandle->last);

    if (lpMenuHandle->hFont)
    {
      DeleteObject(lpMenuHandle->hFont);
      lpMenuHandle->hFont=NULL;
    }
    StackDelete((stack **)&IconMenu_hStackMenuHandles.first, (stack **)&IconMenu_hStackMenuHandles.last, (stack *)lpMenuHandle);

    if (!IconMenu_hStackMenuHandles.first)
      IconMenu_Uninitialize();
  }
}

/********************************************************************
 *
 *  IconMenu_AddItemA, IconMenu_AddItemW, IconMenu_ModifyItemA, IconMenu_ModifyItemW
 *
 *Insert or modify menu item.
 *
 * [in] HICONMENU hIconMenu   -handle returned by IconMenu_Alloc.
 * [in] HIMAGELIST hImageList -handle to the image list. If NULL, image list not used.
 * [in] INT_PTR nIconIndex    -icon zero-based index in image list.
 *                              If hImageList is NULL, nIconIndex specifies icon handle.
 *                              If -1, item doesn't have icon.
 * [in] int nIconWidth        -icon width. If zero, use default menu width.
 * [in] int nIconHeight       -icon height. If zero, use default menu height.
 *
 * [in] HMENU hMenu           -see InsertMenu description in MSDN.
 * [in] INT_PTR nPosition     -see InsertMenu description in MSDN.
 *                              If -1, appends a new item to the end of the specified menu.
 * [in] UINT uFlags           -see InsertMenu description in MSDN.
 *                              MF_BITMAP, MF_MENUBARBREAK and MF_MENUBREAK flags doesn't supported.
 * [in] UINT_PTR uIDNewItem   -see InsertMenu description in MSDN.
 * [in] const void *lpNewItem -see InsertMenu description in MSDN.
 *
 *Returns: If the function succeeds, the return value is nonzero.
 *         If the function fails, the return value is zero.
 ********************************************************************/
BOOL IconMenu_AddItemA(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const char *lpNewItem)
{
  return IconMenu_SetItem(ICONMENU_SETMENU_ANSI|ICONMENU_SETMENU_INSERT, hIconMenu, hImageList, nIconIndex, nIconWidth, nIconHeight, hMenu, nPosition, uFlags, uIDNewItem, (const void *)lpNewItem);
}

BOOL IconMenu_AddItemW(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *lpNewItem)
{
  return IconMenu_SetItem(ICONMENU_SETMENU_UNICODE|ICONMENU_SETMENU_INSERT, hIconMenu, hImageList, nIconIndex, nIconWidth, nIconHeight, hMenu, nPosition, uFlags, uIDNewItem, (const void *)lpNewItem);
}

BOOL IconMenu_ModifyItemA(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const char *lpNewItem)
{
  return IconMenu_SetItem(ICONMENU_SETMENU_ANSI|ICONMENU_SETMENU_MODIFY, hIconMenu, hImageList, nIconIndex, nIconWidth, nIconHeight, hMenu, nPosition, uFlags, uIDNewItem, (const void *)lpNewItem);
}

BOOL IconMenu_ModifyItemW(HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const wchar_t *lpNewItem)
{
  return IconMenu_SetItem(ICONMENU_SETMENU_UNICODE|ICONMENU_SETMENU_MODIFY, hIconMenu, hImageList, nIconIndex, nIconWidth, nIconHeight, hMenu, nPosition, uFlags, uIDNewItem, (const void *)lpNewItem);
}

BOOL IconMenu_SetItem(DWORD dwFlags, HICONMENU hIconMenu, HIMAGELIST hImageList, INT_PTR nIconIndex, int nIconWidth, int nIconHeight, HMENU hMenu, INT_PTR nPosition, UINT uFlags, UINT_PTR uIDNewItem, const void *lpNewItem)
{
  ICONMENUHANDLE *lpMenuHandle=(ICONMENUHANDLE *)hIconMenu;
  ICONMENUSUBMENU *lpSubMenu;
  ICONMENUITEM *lpOldMenuItem;
  ICONMENUITEM *lpNewMenuItem=NULL;
  char szStr[MAX_PATH];
  BOOL bResult=TRUE;

  if (!lpMenuHandle->hFont)
  {
    HFONT hMenuFontOld;
    HDC hDC;
    SIZE sizeWidth={0};

    //Get menu font hanle
    lpMenuHandle->hFont=IconMenu_GetMenuFont();

    //Get menu text height
    if (hDC=GetDC(lpMenuHandle->hWndOwner))
    {
      hMenuFontOld=(HFONT)SelectObject(hDC, lpMenuHandle->hFont);
      GetTextExtentPoint32W(hDC, L"A", 1, &sizeWidth);
      lpMenuHandle->nTextHeight=sizeWidth.cy;
      if (hMenuFontOld) SelectObject(hDC, hMenuFontOld);
      ReleaseDC(lpMenuHandle->hWndOwner, hDC);
    }
  }
  if ((uFlags & MF_BITMAP) || (uFlags & MF_MENUBARBREAK) || (uFlags & MF_MENUBREAK))
    return FALSE;

  if (!(lpSubMenu=IconMenu_GetMenuByHandle(hIconMenu, hMenu)))
  {
    if (!StackInsertAfter((stack **)&lpMenuHandle->first, (stack **)&lpMenuHandle->last, (stack *)lpMenuHandle->last, (stack **)&lpSubMenu, sizeof(ICONMENUSUBMENU)))
    {
      lpSubMenu->hIconMenu=(HICONMENU)lpMenuHandle;
      lpSubMenu->hMenu=hMenu;
    }
  }
  lpOldMenuItem=IconMenu_GetSubMenuItem(lpSubMenu, nPosition, uFlags);
  if (!(uFlags & MF_BYPOSITION) && !lpOldMenuItem)
    return FALSE;

  if (dwFlags & ICONMENU_SETMENU_INSERT)
    StackInsertBefore((stack **)&lpSubMenu->first, (stack **)&lpSubMenu->last, (stack *)lpOldMenuItem, (stack **)&lpNewMenuItem, sizeof(ICONMENUITEM));
  else
    lpNewMenuItem=lpOldMenuItem;
  if (!lpNewMenuItem)
    return FALSE;

  lpNewMenuItem->hIconSubMenu=(HICONSUBMENU)lpSubMenu;
  lpNewMenuItem->uFlags=uFlags;
  lpNewMenuItem->hImageList=hImageList;
  lpNewMenuItem->nIconIndex=nIconIndex;
  if (nIconIndex != -1)
  {
    lpNewMenuItem->nIconWidth=nIconWidth?nIconWidth:ICONMENU_CXICON;
    lpNewMenuItem->nIconHeight=nIconHeight?nIconHeight:ICONMENU_CYICON;
  }
  else
  {
    lpNewMenuItem->nIconWidth=IconMenu_hUxTheme?ICONMENU_CXICON:ICONMENU_CXCHECKMARK;
    lpNewMenuItem->nIconHeight=IconMenu_hUxTheme?ICONMENU_CYICON:ICONMENU_CYCHECKMARK;
  }
  lpNewMenuItem->nItemID=uIDNewItem;
  if (dwFlags & ICONMENU_SETMENU_ANSI)
    lpNewMenuItem->nStrLen=MultiByteToWideChar(CP_ACP, 0, lpNewItem?(char *)lpNewItem:"", -1, lpNewMenuItem->wszStr, MAX_PATH) - 1;
  else
    lpNewMenuItem->nStrLen=(int)xstrcpynW(lpNewMenuItem->wszStr, lpNewItem?(wchar_t *)lpNewItem:L"", MAX_PATH);

  //Parse '&' and '\t'
  {
    wchar_t *wpCountIn=lpNewMenuItem->wszStr;
    wchar_t *wpCountOut=lpNewMenuItem->wszStr;
    wchar_t *wpCountInEnd=lpNewMenuItem->wszStr + lpNewMenuItem->nStrLen;

    while (wpCountIn < wpCountInEnd)
    {
      if (*wpCountIn == '&')
      {
        *wpCountOut++=*wpCountIn++;
        lpNewMenuItem->wchKey=*wpCountIn;
        WideCharToMultiByte(CP_ACP, 0, &lpNewMenuItem->wchKey, 1, &lpNewMenuItem->chKey, 1, NULL, NULL);
      }
      else if (*wpCountIn == '\t')
      {
        wpCountOut++;
        lpNewMenuItem->wpTab=wpCountOut;
        ++wpCountIn;
      }
      else *wpCountOut++=*wpCountIn++;
    }
    *wpCountOut='\0';

    lpNewMenuItem->nStrLen=(int)((lpNewMenuItem->wpTab?lpNewMenuItem->wpTab - 1:wpCountOut) - lpNewMenuItem->wszStr);
    lpNewMenuItem->nTabLen=(int)(lpNewMenuItem->wpTab?wpCountOut - lpNewMenuItem->wpTab:0);
  }

  //Insert/Modify menu
  if (IconMenu_bOldWindows)
    WideCharToMultiByte(CP_ACP, 0, lpNewMenuItem->wszStr, -1, szStr, MAX_PATH, NULL, NULL);

  if ((uFlags & MF_BYPOSITION) && nPosition == -1)
  {
    if (dwFlags & ICONMENU_SETMENU_INSERT)
    {
      if (IconMenu_bOldWindows)
        bResult=AppendMenuA(hMenu, uFlags, uIDNewItem, szStr);
      else
        bResult=AppendMenuW(hMenu, uFlags, uIDNewItem, lpNewMenuItem->wszStr);
    }
    nPosition=lpSubMenu->nItemCount;
  }
  else
  {
    if (dwFlags & ICONMENU_SETMENU_INSERT)
    {
      if (IconMenu_bOldWindows)
        bResult=InsertMenuA(hMenu, (UINT)nPosition, uFlags, uIDNewItem, szStr);
      else
        bResult=InsertMenuW(hMenu, (UINT)nPosition, uFlags, uIDNewItem, lpNewMenuItem->wszStr);
    }
  }

  if (bResult)
  {
    if (dwFlags & ICONMENU_SETMENU_MODIFY)
    {
      if (IconMenu_bOldWindows)
        bResult=ModifyMenuA(hMenu, (UINT)nPosition, uFlags, uIDNewItem, szStr);
      else
        bResult=ModifyMenuW(hMenu, (UINT)nPosition, uFlags, uIDNewItem, lpNewMenuItem->wszStr);
    }
    if (bResult)
    {
      if (bResult=ModifyMenuA(hMenu, (UINT)nPosition, MF_OWNERDRAW|uFlags, uIDNewItem, (const char *)lpNewMenuItem))
      {
        if (dwFlags & ICONMENU_SETMENU_INSERT)
          ++lpSubMenu->nItemCount;
        IconMenu_GetSubMenuSize(lpSubMenu);
      }
    }
  }
  if (!bResult)
  {
    if (lpNewMenuItem != lpOldMenuItem)
      StackDelete((stack **)&lpSubMenu->first, (stack **)&lpSubMenu->last, (stack *)lpNewMenuItem);
  }
  return bResult;
}

/********************************************************************
 *
 *  IconMenu_DeleteItem, IconMenu_RemoveItem
 *
 *Delete or remove item from menu.
 *
 * [in] HICONMENU hIconMenu  -handle returned by IconMenu_Alloc.
 * [in] HMENU hMenu          -see DeleteMenu description in MSDN.
 * [in] INT_PTR nPosition    -see DeleteMenu description in MSDN.
 * [in] UINT uFlags          -see DeleteMenu description in MSDN.
 *
 *Returns: If the function succeeds, the return value is nonzero.
 *         If the function fails, the return value is zero.
 ********************************************************************/

BOOL IconMenu_DeleteItem(HICONMENU hIconMenu, HMENU hMenu, INT_PTR nPosition, UINT uFlags)
{
  return IconMenu_UnsetItem(hIconMenu, hMenu, nPosition, uFlags, TRUE);
}

BOOL IconMenu_RemoveItem(HICONMENU hIconMenu, HMENU hMenu, INT_PTR nPosition, UINT uFlags)
{
  return IconMenu_UnsetItem(hIconMenu, hMenu, nPosition, uFlags, FALSE);
}

BOOL IconMenu_UnsetItem(HICONMENU hIconMenu, HMENU hMenu, INT_PTR nPosition, UINT uFlags, BOOL bDeleteSubmenu)
{
  ICONMENUSUBMENU *lpSubMenu;
  ICONMENUITEM *lpMenuItem;
  BOOL bResult=FALSE;

  if (bDeleteSubmenu ? DeleteMenu(hMenu, (UINT)nPosition, uFlags) : RemoveMenu(hMenu, (UINT)nPosition, uFlags))
  {
    if (lpSubMenu=IconMenu_GetMenuByHandle(hIconMenu, hMenu))
    {
      if (lpMenuItem=IconMenu_GetSubMenuItem(lpSubMenu, nPosition, uFlags))
      {
        if (!StackDelete((stack **)&lpSubMenu->first, (stack **)&lpSubMenu->last, (stack *)lpMenuItem))
        {
          --lpSubMenu->nItemCount;
          IconMenu_GetSubMenuSize(lpSubMenu);

          bResult=TRUE;
        }
      }
    }
  }
  return bResult;
}

/********************************************************************
 *
 *  IconMenu_Messages
 *
 *Process menu message.
 *
 *Returns: If a message has been processed, return value is non-zero.
 ********************************************************************/
LRESULT IconMenu_Messages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_MEASUREITEM:
    {
      LPMEASUREITEMSTRUCT lpmis=(LPMEASUREITEMSTRUCT)lParam;
      ICONMENUHANDLE *lpMenuHandle=NULL;
      ICONMENUSUBMENU *lpSubMenu=NULL;
      ICONMENUITEM *lpMenuItem=NULL;

      if (lpmis && (lpmis->CtlType == ODT_MENU))
      {
        lpMenuItem=(ICONMENUITEM *)lpmis->itemData;

        //Windows API drawback: MEASUREITEMSTRUCT.itemID is UINT, but must be UINT_PTR.
        //Submenus MEASUREITEMSTRUCT.itemID on x64 is trimmed to UINT.
        if (IconMenu_IsItem(NULL, lpMenuItem) && (UINT)lpMenuItem->nItemID == lpmis->itemID)
        {
          lpSubMenu=(ICONMENUSUBMENU *)lpMenuItem->hIconSubMenu;
          lpMenuHandle=(ICONMENUHANDLE *)lpSubMenu->hIconMenu;

          lpmis->itemHeight=lpMenuItem->nItemHeight;
          lpmis->itemWidth=lpSubMenu->nTextOffsetMax + lpMenuItem->nStrWidth + (lpSubMenu->nTabWidthMax?(ICONMENU_FROMSTRING_TOTAB + lpSubMenu->nTabWidthMax):0) + ICONMENU_FROMTAB_TOMENUEDGE;
          return 1;
        }
      }
      return 0;
    }
    case WM_DRAWITEM:
    {
      LPDRAWITEMSTRUCT lpdis=(LPDRAWITEMSTRUCT)lParam;
      ICONMENUHANDLE *lpMenuHandle=NULL;
      ICONMENUSUBMENU *lpSubMenu=NULL;
      ICONMENUITEM *lpMenuItem=NULL;

      if (lpdis && (lpdis->CtlType == ODT_MENU))
      {
        lpMenuItem=(ICONMENUITEM *)lpdis->itemData;

        //Windows API drawback: DRAWITEMSTRUCT.itemID is UINT, but must be UINT_PTR.
        //Submenus DRAWITEMSTRUCT.itemID on x64 is trimmed to UINT.
        if (IconMenu_IsItem(NULL, lpMenuItem) && (UINT)lpMenuItem->nItemID == lpdis->itemID)
        {
          HDC hMemDC;
          HBITMAP hMemBitmap;
          HBITMAP hOldBitmap;
          HFONT hOldFont;
          HICON hIcon=NULL;
          HPEN hPen;
          HPEN hPenOld;
          POINT ptString;
          POINT ptTab;
          RECT rcItem;
          RECT rcImageRect;
          RECT rcImageEdge;
          RECT rcArrow;
          COLORREF crText;
          COLORREF crBk;
          COLORREF crIconEdge;
          int nModeBkOld;
          int x;
          int y;
          DWORD dwFlags=0;
          BOOL bShowPrefixAlways=FALSE;
          BOOL bArrow=FALSE;

          lpSubMenu=(ICONMENUSUBMENU *)lpMenuItem->hIconSubMenu;
          lpMenuHandle=(ICONMENUHANDLE *)lpSubMenu->hIconMenu;

          if (lpMenuItem->uFlags & MF_SEPARATOR)
          {
            rcImageEdge.left=lpdis->rcItem.left + ICONMENU_FROMMENUEDGE_TOICONEDGE;
            rcImageEdge.top=lpdis->rcItem.top + (lpdis->rcItem.bottom - lpdis->rcItem.top) / 2;
            rcImageEdge.right=lpdis->rcItem.right - ICONMENU_FROMMENUEDGE_TOICONEDGE;
            rcImageEdge.bottom=lpdis->rcItem.bottom;

            if (IconMenu_hUxTheme)
            {
              SIZE sizeSeparator;

              if (IconMenu_GetThemePartSize(IconMenu_hUxTheme, lpdis->hDC, 15 /*MENU_POPUPSEPARATOR*/, 0, NULL, 1 /*TS_TRUE*/, &sizeSeparator) == S_OK)
              {
                rcImageEdge.top-=sizeSeparator.cy / 2;
                rcImageEdge.bottom=rcImageEdge.top + sizeSeparator.cy;
              }
              IconMenu_DrawThemeBackground(IconMenu_hUxTheme, lpdis->hDC, 15 /*MENU_POPUPSEPARATOR*/, 0, &rcImageEdge, NULL);
            }
            else
            {
              rcImageEdge.top-=1;
              DrawEdge(lpdis->hDC, &rcImageEdge, EDGE_ETCHED, BF_TOP);
            }
          }
          else
          {
            rcItem.left=0;
            rcItem.top=0;
            rcItem.right=lpdis->rcItem.right - lpdis->rcItem.left;
            rcItem.bottom=lpdis->rcItem.bottom - lpdis->rcItem.top;

            //Create memory DC to avoid flashing
            hMemDC=CreateCompatibleDC(lpdis->hDC);
            hMemBitmap=CreateCompatibleBitmap(lpdis->hDC, rcItem.right, rcItem.bottom);
            hOldBitmap=(HBITMAP)SelectObject(hMemDC, hMemBitmap);
            hOldFont=(HFONT)SelectObject(hMemDC, ((ICONMENUHANDLE *)lpSubMenu->hIconMenu)->hFont);

            //Load icon
            if (lpMenuItem->nIconIndex != -1)
            {
              if (lpMenuItem->hImageList)
                hIcon=ImageList_GetIcon(lpMenuItem->hImageList, (int)lpMenuItem->nIconIndex, ILD_NORMAL);
              else
                hIcon=(HICON)lpMenuItem->nIconIndex;
            }

            //Get dimentions
            if (lpSubMenu->nTextOffsetMax > ICONMENU_FROMMENUEDGE_TOICONEDGE + ICONMENU_CXICONMARGIN + lpMenuItem->nIconWidth + ICONMENU_CXICONMARGIN + ICONMENU_FROMICONEDGE_TOSTRING)
              rcImageEdge.left=(lpSubMenu->nTextOffsetMax - ICONMENU_FROMICONEDGE_TOSTRING) / 2 - (ICONMENU_CXICONMARGIN + lpMenuItem->nIconWidth + ICONMENU_CXICONMARGIN) / 2;
            else
              rcImageEdge.left=ICONMENU_FROMMENUEDGE_TOICONEDGE;
            rcImageEdge.top=rcItem.top;
            rcImageEdge.right=rcImageEdge.left + ICONMENU_CXICONMARGIN + lpMenuItem->nIconWidth + ICONMENU_CXICONMARGIN;
            rcImageEdge.bottom=rcItem.bottom;

            rcImageRect.left=rcImageEdge.left + ICONMENU_CXICONMARGIN + (hIcon || IconMenu_hUxTheme?0:1);
            rcImageRect.top=rcImageEdge.top + ((rcImageEdge.bottom - rcImageEdge.top) / 2 - lpMenuItem->nIconHeight / 2) + (hIcon || IconMenu_hUxTheme?0:1);
            rcImageRect.right=rcImageRect.left + lpMenuItem->nIconWidth;
            rcImageRect.bottom=rcImageRect.top + lpMenuItem->nIconHeight;

            ptString.x=lpSubMenu->nTextOffsetMax;
            ptString.y=rcItem.top + ((rcItem.bottom - rcItem.top) / 2 - lpMenuHandle->nTextHeight / 2) - 1;
            ptTab.x=ptString.x + lpSubMenu->nStrWidthMax + ICONMENU_FROMSTRING_TOTAB;
            ptTab.y=ptString.y;

            //Colors
            crText=GetSysColor(COLOR_MENUTEXT);
            crBk=GetSysColor(COLOR_MENU);
            crIconEdge=GetSysColor(COLOR_HIGHLIGHT);

            if (!IconMenu_hUxTheme)
            {
              if (lpdis->itemState & ODS_SELECTED)
                crText=GetSysColor(COLOR_HIGHLIGHTTEXT);
              if (lpdis->itemState & ODS_GRAYED)
                crText=GetSysColor(COLOR_GRAYTEXT);
              if (lpdis->itemState & ODS_SELECTED)
                crBk=GetSysColor(COLOR_HIGHLIGHT);
            }
            SetTextColor(hMemDC, crText);
            SetBkColor(hMemDC, crBk);

            //Fill background
            FillRect(hMemDC, &rcItem, GetSysColorBrush(COLOR_MENU));

            if (lpdis->itemState & ODS_SELECTED)
            {
              RECT rcHot=rcItem;

              rcHot.left=rcItem.left + 1;

              if (IconMenu_hUxTheme)
              {
                IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 14 /*MENU_POPUPITEM*/, (lpdis->itemState & ODS_GRAYED)?4 /*MPI_DISABLEDHOT*/:2 /*MPI_HOT*/, &rcHot, NULL);
              }
              else
              {
                FillRect(hMemDC, &rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
                //IconMenu_RoundRect(hMemDC, &rcHot, crBk, IconMenu_Brightness(crBk, 85));
              }
            }

            //Draw text
            nModeBkOld=SetBkMode(hMemDC, TRANSPARENT);
            dwFlags=0;
            if ((lpdis->itemState & ODS_GRAYED) && (IconMenu_hUxTheme || !(lpdis->itemState & ODS_SELECTED)))
              dwFlags|=DSS_DISABLED;
            SystemParametersInfoA(SPI_GETKEYBOARDCUES, 0, &bShowPrefixAlways, 0);
            if (!bShowPrefixAlways && IconMenu_bMouseMenu)
              dwFlags|=DSS_HIDEPREFIX;

            if (IconMenu_bOldWindows)
            {
              char szString[MAX_PATH];
              int nStrLen=0;
              int nTabLen;

              if (lpMenuItem->nStrLen)
              {
                nStrLen=WideCharToMultiByte(CP_ACP, 0, lpMenuItem->wszStr, lpMenuItem->nStrLen, szString, MAX_PATH, NULL, NULL);
                DrawStateA(hMemDC, NULL, (DRAWSTATEPROC)NULL, (LPARAM)szString, (WPARAM)nStrLen, ptString.x, ptString.y, 0, 0, DST_PREFIXTEXT|dwFlags);
              }
              if (lpMenuItem->nTabLen)
              {
                nTabLen=WideCharToMultiByte(CP_ACP, 0, lpMenuItem->wpTab, lpMenuItem->nTabLen, szString + nStrLen, MAX_PATH - nStrLen, NULL, NULL);
                DrawStateA(hMemDC, NULL, (DRAWSTATEPROC)NULL, (LPARAM)(szString + nStrLen), (WPARAM)nTabLen, ptTab.x, ptTab.y, 0, 0, DST_PREFIXTEXT|dwFlags);
              }
            }
            else
            {
              if (lpMenuItem->nStrLen)
                DrawStateW(hMemDC, NULL, (DRAWSTATEPROC)NULL, (LPARAM)lpMenuItem->wszStr, (WPARAM)lpMenuItem->nStrLen, ptString.x, ptString.y, 0, 0, DST_PREFIXTEXT|dwFlags);
              if (lpMenuItem->nTabLen)
                DrawStateW(hMemDC, NULL, (DRAWSTATEPROC)NULL, (LPARAM)lpMenuItem->wpTab, (WPARAM)lpMenuItem->nTabLen, ptTab.x, ptTab.y, 0, 0, DST_PREFIXTEXT|dwFlags);
            }
            SetBkMode(hMemDC, nModeBkOld);

            //Draw icon
            if (hIcon)
            {
              HICON hDrawIcon=hIcon;
              HICON hAdjustIcon=NULL;
              int nDrawIndex;

              //Icon background
              if (lpdis->itemState & ODS_CHECKED)
              {
                if (IconMenu_hUxTheme)
                {
                  IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 12 /*MENU_POPUPCHECKBACKGROUND*/, (lpdis->itemState & ODS_GRAYED)?1 /*MCB_DISABLED*/:2 /*MCB_NORMAL*/, &rcImageEdge, NULL);
                }
                else
                {
                  if (GetSysColor(COLOR_BTNHIGHLIGHT) == GetSysColor(COLOR_MENU))
                  {
                    //WinXP with Luna theme
                    IconMenu_RoundRect(hMemDC, &rcImageEdge, crIconEdge, IconMenu_Brightness(crIconEdge, 85));
                  }
                  else
                  {
                    if (!(lpdis->itemState & ODS_SELECTED))
                    {
                      //Draw chessboard
                      if (hPen=CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNHIGHLIGHT)))
                      {
                        hPenOld=(HPEN)SelectObject(hMemDC, hPen);

                        for (y=rcImageRect.top; y < rcImageRect.bottom; ++y)
                        {
                          for (x=rcImageRect.left + (y - rcImageRect.top) % 2; x < rcImageRect.right; x+=2)
                          {
                            MoveToEx(hMemDC, x, y, NULL);
                            LineTo(hMemDC, x, y + 1);
                          }
                        }
                        SelectObject(hMemDC, hPenOld);
                        DeleteObject(hPen);
                      }
                    }
                    else if (!(lpdis->itemState & ODS_GRAYED))
                    {
                      FillRect(hMemDC, &rcImageEdge, GetSysColorBrush(COLOR_MENU));
                    }
                    DrawEdge(hMemDC, &rcImageEdge, BDR_SUNKENOUTER, BF_RECT);
                  }
                }
              }

              //Icon image
              if (lpdis->itemState & ODS_GRAYED)
              {
                if (hAdjustIcon=IconMenu_AdjustIcon(hMemDC, hIcon, TRUE, 30, 0))
                  hDrawIcon=hAdjustIcon;
              }
              else if (lpdis->itemState & ODS_SELECTED)
              {
                if (hAdjustIcon=IconMenu_AdjustIcon(hMemDC, hIcon, FALSE, 15, 15))
                  hDrawIcon=hAdjustIcon;
              }

              if (lpMenuItem->hImageList)
              {
                if (hAdjustIcon)
                {
                  //Draw icon without alpha channel, because ImageList_GetIcon remove it.
                  nDrawIndex=ImageList_AddIcon(lpMenuItem->hImageList, hDrawIcon);
                  ImageList_DrawEx(lpMenuItem->hImageList, nDrawIndex, hMemDC, rcImageRect.left, rcImageRect.top, lpMenuItem->nIconWidth, lpMenuItem->nIconHeight, CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
                  ImageList_Remove(lpMenuItem->hImageList, nDrawIndex);
                }
                else
                {
                  //Draw icon with alpha channel.
                  ImageList_DrawEx(lpMenuItem->hImageList, (int)lpMenuItem->nIconIndex, hMemDC, rcImageRect.left, rcImageRect.top, lpMenuItem->nIconWidth, lpMenuItem->nIconHeight, CLR_NONE, CLR_NONE, ILD_TRANSPARENT);
                }
                DestroyIcon(hIcon);
              }
              else
              {
                DrawIconEx(hMemDC, rcImageRect.left, rcImageRect.top, hDrawIcon, lpMenuItem->nIconWidth, lpMenuItem->nIconHeight, 0, NULL, DI_NORMAL);
              }
              if (hAdjustIcon) DestroyIcon(hAdjustIcon);
            }
            else
            {
              //Draw standard checkmarks
              if (lpdis->itemState & ODS_CHECKED)
              {
                if (IconMenu_hUxTheme)
                {
                  if (GetMenuState(lpSubMenu->hMenu, lpdis->itemID, MF_BYCOMMAND) & MF_USECHECKBITMAPS)
                  {
                    IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 12 /*MENU_POPUPCHECKBACKGROUND*/, (lpdis->itemState & ODS_GRAYED)?1 /*MCB_DISABLED*/:2 /*MCB_NORMAL*/, &rcImageEdge, NULL);
                    IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 11 /*MENU_POPUPCHECK*/, (lpdis->itemState & ODS_GRAYED)?4 /*MC_BULLETDISABLED*/:3 /*MC_BULLETNORMAL*/, &rcImageRect, NULL);
                  }
                  else
                  {
                    IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 12 /*MENU_POPUPCHECKBACKGROUND*/, (lpdis->itemState & ODS_GRAYED)?1 /*MCB_DISABLED*/:2 /*MCB_NORMAL*/, &rcImageEdge, NULL);
                    IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 11 /*MENU_POPUPCHECK*/, (lpdis->itemState & ODS_GRAYED)?2 /*MC_CHECKMARKDISABLED*/:1 /*MC_CHECKMARKNORMAL*/, &rcImageRect, NULL);
                  }
                }
                else
                {
                  if (hPen=CreatePen(PS_SOLID, 0, crText))
                  {
                    hPenOld=(HPEN)SelectObject(hMemDC, hPen);
                    x=rcImageRect.left;
                    y=rcImageRect.top;

                    //Draw checkmarks manually
                    {
                      POINT ptRadio[]={{1, 0}, {5, 0},
                                       {0, 1}, {6, 1},
                                       {0, 2}, {6, 2},
                                       {0, 3}, {6, 3},
                                       {0, 4}, {6, 4},
                                       {1, 5}, {5, 5}};
                      POINT ptCheck[]={{0, 2}, {0, 5},
                                       {1, 3}, {1, 6},
                                       {2, 4}, {2, 7},
                                       {3, 3}, {3, 6},
                                       {4, 2}, {4, 5},
                                       {5, 1}, {5, 4},
                                       {6, 0}, {6, 3}};
                      POINT *ptDraw;
                      int nElements;
                      int i;

                      if (GetMenuState(lpSubMenu->hMenu, lpdis->itemID, MF_BYCOMMAND) & MF_USECHECKBITMAPS)
                      {
                        //Radio button
                        ptDraw=ptRadio;
                        nElements=sizeof(ptRadio) / sizeof(POINT);
                      }
                      else
                      {
                        //Check box
                        ptDraw=ptCheck;
                        nElements=sizeof(ptCheck) / sizeof(POINT);
                      }

                      for (i=0; i < nElements; i+=2)
                      {
                        MoveToEx(hMemDC, x + ptDraw[i].x, y + ptDraw[i].y, NULL);
                        LineTo(hMemDC, x + ptDraw[i + 1].x, y + ptDraw[i + 1].y);
                      }
                    }
                    SelectObject(hMemDC, hPenOld);
                    DeleteObject(hPen);
                  }
                }
              }
            }

            //Draw standard submenu arrow
            bArrow=FALSE;
            if (lpMenuItem->uFlags & MF_POPUP)
            {
              if (IconMenu_hUxTheme)
              {
                SIZE sizeArrow;

                if (IconMenu_GetThemePartSize(IconMenu_hUxTheme, hMemDC, 16 /*MENU_POPUPSUBMENU*/, (lpdis->itemState & ODS_GRAYED)?2 /*MSM_DISABLED*/:1 /*MSM_NORMAL*/, NULL, 1 /*TS_TRUE*/, &sizeArrow) == S_OK)
                {
                  rcArrow.left=rcItem.right - sizeArrow.cx * 2 + sizeArrow.cx / 2;
                  rcArrow.top=rcItem.top + (rcItem.bottom - rcItem.top) / 2 - sizeArrow.cy / 2 - 2;
                  rcArrow.right=rcArrow.left + sizeArrow.cx;
                  rcArrow.bottom=rcArrow.top + sizeArrow.cy;
                  IconMenu_DrawThemeBackground(IconMenu_hUxTheme, hMemDC, 16 /*MENU_POPUPSUBMENU*/, (lpdis->itemState & ODS_GRAYED)?2 /*MSM_DISABLED*/:1 /*MSM_NORMAL*/, &rcArrow, NULL);
                  bArrow=TRUE;
                }
              }
            }

            //Copy from memory DC
            BitBlt(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, rcItem.right, rcItem.bottom, hMemDC, 0, 0, SRCCOPY);

            if (bArrow)
            {
              //Prevent system to draw submenu arrow
              ExcludeClipRect(lpdis->hDC, lpdis->rcItem.left + rcArrow.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom);
              bArrow=FALSE;
            }

            SelectObject(hMemDC, hOldFont);
            SelectObject(hMemDC, hOldBitmap);
            DeleteObject(hMemBitmap);
            DeleteDC(hMemDC);
          }
          return 1;
        }
      }
      return 0;
    }
    case WM_THEMECHANGED:
    {
      if (IconMenu_hUxTheme)
      {
        IconMenu_CloseThemeData(IconMenu_hUxTheme);
        IconMenu_hUxTheme=NULL;
      }
      IconMenu_bIsThemed=IconMenu_IsAppThemed();
      return 0;
    }
    case WM_CONTEXTMENU:
    {
      if (lParam == -1)
        IconMenu_bMouseMenu=FALSE;
      else
        IconMenu_bMouseMenu=TRUE;
      return 0;
    }
    case WM_SYSCOMMAND:
    {
      if (lParam <= 0)
        IconMenu_bMouseMenu=FALSE;
      else
        IconMenu_bMouseMenu=TRUE;
      return 0;
    }
    case WM_EXITMENULOOP:
    {
      IconMenu_bMouseMenu=FALSE;
      return 0;
    }
    case WM_MENUCHAR:
    {
      ICONMENUSUBMENU *lpSubMenu=NULL;
      ICONMENUITEM *lpMenuItem=NULL;
      INT_PTR nChar;
      int nPos=0;
      BOOL bUnicodeWindow;

      if (bUnicodeWindow=IsWindowUnicode(hWnd))
        nChar=(INT_PTR)CharUpperW((wchar_t *)(UINT_PTR)LOWORD(wParam));
      else
        nChar=(INT_PTR)CharUpperA((char *)(UINT_PTR)LOWORD(wParam));

      if (lpSubMenu=IconMenu_GetMenuByHandle(NULL, (HMENU)lParam))
      {
        for (lpMenuItem=lpSubMenu->first; lpMenuItem; lpMenuItem=lpMenuItem->next)
        {
          if (bUnicodeWindow)
          {
            if (nChar == (INT_PTR)CharUpperW((wchar_t *)(UINT_PTR)(WORD)lpMenuItem->wchKey))
              return MAKELRESULT(nPos, MNC_EXECUTE);
          }
          else
          {
            if (nChar == (INT_PTR)CharUpperA((char *)(UINT_PTR)(WORD)lpMenuItem->chKey))
              return MAKELRESULT(nPos, MNC_EXECUTE);
          }
          ++nPos;
        }
      }
      return 0;
    }
  }
  return 0;
}
#endif //ICONMENU_INCLUDED
#endif //ICONMENU_INCLUDE



/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "IconMenu.h"

//Include stack functions
#define StackInsertAfter
#define StackMoveBefore
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#define xstrcpynW
#include "StrFunc.h"

//Include icon menu functions
#define ICONMENU_INCLUDE
#include "IconMenu.h"

//If window procedure
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (lResult=IconMenu_Messages(hWnd, uMsg, wParam, lParam))
    return lResult;

  if (uMsg == WM_CONTEXTMENU)
  {
    HICONMENU hIconMenu;
    HIMAGELIST hImageList;
    HMENU hMenu;
    HMENU hSubMenu;
    HICON hIcon;
    POINT pt;
    int nIcon;

    GetCursorPos(&pt);

    //Create image list
    hImageList=ImageList_Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 0);
    ImageList_SetBkColor(hImageList, GetSysColor(COLOR_MENU));

    //Load icons
    for (nIcon=IDI_ICON_OPEN; nIcon <= IDI_ICON_EXIT; ++nIcon)
    {
      hIcon=(HICON)LoadImageA(hInstance, MAKEINTRESOURCEA(nIcon), IMAGE_ICON, 16, 16, 0);
      ImageList_AddIcon(hImageList, hIcon);
      DestroyIcon(hIcon);
    }

    //Add menu items
    hIconMenu=IconMenu_Alloc(hWnd);

    if (hMenu=CreatePopupMenu())
    {
      IconMenu_AddItemW(hIconMenu, hImageList, 0, 16, 16, hMenu, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_OPEN, L"Item1");
      IconMenu_AddItemW(hIconMenu, hImageList, -1, 0, 0, hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, (UINT_PTR)-1, NULL);
      IconMenu_AddItemW(hIconMenu, hImageList, 1, 16, 16, hMenu, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_SAVE, L"Item2");
      IconMenu_AddItemW(hIconMenu, hImageList, 2, 16, 16, hMenu, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_SAVEAS, L"Item3");
      IconMenu_AddItemW(hIconMenu, hImageList, -1, 0, 0, hMenu, -1, MF_BYPOSITION|MF_SEPARATOR, (UINT_PTR)-1, NULL);

      if (hSubMenu=CreatePopupMenu())
      {
        IconMenu_AddItemW(hIconMenu, hImageList, -1, 0, 0, hMenu, -1, MF_BYPOSITION|MF_POPUP, (UINT_PTR)hSubMenu, L"SubMenu");
        IconMenu_AddItemW(hIconMenu, hImageList, 0, 16, 16, hSubMenu, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_SUBOPEN, L"Item1");
      }
      IconMenu_AddItemW(hIconMenu, hImageList, 3, 16, 16, hMenu, -1, MF_BYPOSITION|MF_STRING, IDC_ITEM_EXIT, L"Exit");
    }

    //Show menu
    TrackPopupMenu(hMenu, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

    //To remove menu item - use IconMenu_DelItem:
    IconMenu_DelItem(hIconMenu, hMenu, 0, MF_BYPOSITION);

    //Free resources
    DestroyMenu(hMenu);
    IconMenu_Free(hIconMenu, NULL);
  }

  //...
}

//If dialog procedure
BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (lResult=IconMenu_Messages(hDlg, uMsg, wParam, lParam))
  {
    SetWindowLongA(hDlg, DWL_MSGRESULT, lResult);
    return TRUE;
  }

  //...
}

*/

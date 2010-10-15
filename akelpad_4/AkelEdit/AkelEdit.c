/***********************************************************************************
 *                      AkelEdit text control v1.5.0                               *
 *                                                                                 *
 * Copyright 2007-2010 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 *                                                                                 *
 *  Features:                                                                      *
 * - RichEdit control compatibility.                                               *
 * - File size only limited by available memory.                                   *
 * - Full unicode strings support.                                                 *
 * - Support of UTF-16 surrogate pairs.                                            *
 * - Support of DOS/Windows, Unix and Mac newline formats.                         *
 * - Support of any length lines.                                                  *
 * - Unlimited undo/redo.                                                          *
 * - Column text selection.                                                        *
 * - Editing and correct saving of binary files.                                   *
 * - Syntax highlighing.                                                           *
 * - URL detection and highlighing.                                                *
 * - Code folding.                                                                 *
 * - Split edit window.                                                            *
 * - Color printing.                                                               *
 * - Support of proportional and non-proportional fonts.                           *
 * - Support of Input Method Editors (IME).                                        *
 * - OLE drag'n'drop editing capabilities.                                         *
 * - Check a saving correctness of all symbols in the specified encoding.          *
 *                                                                                 *
 *  Not implemented:                                                               *
 * - No BiDi (right-to-left) support.                                              *
 * - No highlighing for multi-line "Quotes".                                       *
 ***********************************************************************************/


//// Includes

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stddef.h>
#include <imm.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "AkelBuild.h"
#include "Resources\resource.h"
#include "Resources\version.h"


//Include stack functions
#define StackJoin
#define StackSplit
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemcmp
#define xmemset
#define xstrcmpiA
#define xstrcmpiW
#define xstrcpynA
#define xstrcpynW
#include "StrFunc.h"


//// Global variables

HANDLE hAkelEditProcessHeap=0;
HANDLE hAkelEditHeapCount=0;
HSTACK hAkelEditWindowsStack={0};
HSTACK hAkelEditFontsStackA={0};
HSTACK hAkelEditFontsStackW={0};
HSTACK hAkelEditBitmapsStack={0};
HSTACK hAkelEditThemesStack={0};
BOOL bAkelEditClassRegisteredA=FALSE;
BOOL bAkelEditClassRegisteredW=FALSE;
BOOL bRichEditClassRegisteredA=FALSE;
BOOL bRichEditClassRegisteredW=FALSE;
HCURSOR hAkelEditCursorIBeam=NULL;
HCURSOR hAkelEditCursorArrow=NULL;
HCURSOR hAkelEditCursorMargin=NULL;
HCURSOR hAkelEditCursorHand=NULL;
HCURSOR hAkelEditCursorMCenterAll=NULL;
HCURSOR hAkelEditCursorMCenterLeftRight=NULL;
HCURSOR hAkelEditCursorMCenterTopBottom=NULL;
HCURSOR hAkelEditCursorMLeft=NULL;
HCURSOR hAkelEditCursorMLeftTop=NULL;
HCURSOR hAkelEditCursorMTop=NULL;
HCURSOR hAkelEditCursorMRightTop=NULL;
HCURSOR hAkelEditCursorMRight=NULL;
HCURSOR hAkelEditCursorMRightBottom=NULL;
HCURSOR hAkelEditCursorMBottom=NULL;
HCURSOR hAkelEditCursorMLeftBottom=NULL;
HBITMAP hAkelEditBitmapMCenterAll=NULL;
HBITMAP hAkelEditBitmapMCenterLeftRight=NULL;
HBITMAP hAkelEditBitmapMCenterTopBottom=NULL;
AKELEDIT *lpAkelEditPrev=NULL;
AKELEDIT *lpAkelEditDrag=NULL;
UINT cfAkelEditColumnSel=0;
UINT cfAkelEditText=0;


//// Entry point

#ifndef AKELEDIT_STATICBUILD
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    #ifdef AKELEDIT_NOREGISTERRICHEDIT
      OleInitialize(0);
      AE_RegisterClassA(hinstDLL, FALSE);
      AE_RegisterClassW(hinstDLL, FALSE);
    #else
      OleInitialize(0);
      AE_RegisterClassA(hinstDLL, TRUE);
      AE_RegisterClassW(hinstDLL, TRUE);
    #endif
  }
  else if (fdwReason == DLL_THREAD_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_DETACH)
  {
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
    AE_UnregisterClassA(hinstDLL);
    AE_UnregisterClassW(hinstDLL);
    OleUninitialize();
  }
  return TRUE;
}
#endif


//// AkelEdit control

BOOL AE_RegisterClassA(HINSTANCE hInstance, BOOL bRegisterRichEdit)
{
  if (!bAkelEditClassRegisteredA)
  {
    WNDCLASSA wndclass={0};

    //AkelEdit class
    wndclass.style        =CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditShellProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursorA(NULL, (char *)IDC_IBEAM);
    wndclass.hbrBackground=NULL;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AES_AKELEDITCLASSA;
    bAkelEditClassRegisteredA=RegisterClassA(&wndclass);

    //RichEdit class
    if (bRegisterRichEdit)
    {
      wndclass.lpszClassName=AES_RICHEDITCLASSA;
      bRichEditClassRegisteredA=RegisterClassA(&wndclass);
    }

    if (!hAkelEditProcessHeap) hAkelEditProcessHeap=GetProcessHeap();
    if (!cfAkelEditColumnSel) cfAkelEditColumnSel=RegisterClipboardFormatA("MSDEVColumnSelect");
    if (!cfAkelEditText) cfAkelEditText=RegisterClipboardFormatA("AkelEditText");
    if (!hAkelEditCursorIBeam) hAkelEditCursorIBeam=LoadCursorA(NULL, (char *)IDC_IBEAM);
    if (!hAkelEditCursorArrow) hAkelEditCursorArrow=LoadCursorA(NULL, (char *)IDC_ARROW);
    if (!hAkelEditCursorMargin) hAkelEditCursorMargin=LoadCursorA(hInstance, (char *)IDC_AEMARGIN);
    if (!hAkelEditCursorHand) hAkelEditCursorHand=LoadCursorA(hInstance, (char *)IDC_AEHAND);
    if (!hAkelEditCursorMCenterAll) hAkelEditCursorMCenterAll=LoadCursorA(hInstance, (char *)IDC_AEMCENTERALL);
    if (!hAkelEditCursorMCenterLeftRight) hAkelEditCursorMCenterLeftRight=LoadCursorA(hInstance, (char *)IDC_AEMCENTERLEFTRIGHT);
    if (!hAkelEditCursorMCenterTopBottom) hAkelEditCursorMCenterTopBottom=LoadCursorA(hInstance, (char *)IDC_AEMCENTERTOPBOTTOM);
    if (!hAkelEditCursorMLeft) hAkelEditCursorMLeft=LoadCursorA(hInstance, (char *)IDC_AEMLEFT);
    if (!hAkelEditCursorMLeftTop) hAkelEditCursorMLeftTop=LoadCursorA(hInstance, (char *)IDC_AEMLEFTTOP);
    if (!hAkelEditCursorMTop) hAkelEditCursorMTop=LoadCursorA(hInstance, (char *)IDC_AEMTOP);
    if (!hAkelEditCursorMRightTop) hAkelEditCursorMRightTop=LoadCursorA(hInstance, (char *)IDC_AEMRIGHTTOP);
    if (!hAkelEditCursorMRight) hAkelEditCursorMRight=LoadCursorA(hInstance, (char *)IDC_AEMRIGHT);
    if (!hAkelEditCursorMRightBottom) hAkelEditCursorMRightBottom=LoadCursorA(hInstance, (char *)IDC_AEMRIGHTBOTTOM);
    if (!hAkelEditCursorMBottom) hAkelEditCursorMBottom=LoadCursorA(hInstance, (char *)IDC_AEMBOTTOM);
    if (!hAkelEditCursorMLeftBottom) hAkelEditCursorMLeftBottom=LoadCursorA(hInstance, (char *)IDC_AEMLEFTBOTTOM);
    if (!hAkelEditBitmapMCenterAll) hAkelEditBitmapMCenterAll=(HBITMAP)LoadImageA(hInstance, MAKEINTRESOURCEA(IDB_BITMAP_MCENTERALL), IMAGE_BITMAP, 0, 0, 0);
    if (!hAkelEditBitmapMCenterLeftRight) hAkelEditBitmapMCenterLeftRight=(HBITMAP)LoadImageA(hInstance, MAKEINTRESOURCEA(IDB_BITMAP_MCENTERLEFTRIGHT), IMAGE_BITMAP, 0, 0, 0);
    if (!hAkelEditBitmapMCenterTopBottom) hAkelEditBitmapMCenterTopBottom=(HBITMAP)LoadImageA(hInstance, MAKEINTRESOURCEA(IDB_BITMAP_MCENTERTOPBOTTOM), IMAGE_BITMAP, 0, 0, 0);
  }
  return bAkelEditClassRegisteredA;
}

BOOL AE_RegisterClassW(HINSTANCE hInstance, BOOL bRegisterRichEdit)
{
  if (!bAkelEditClassRegisteredW)
  {
    WNDCLASSW wndclass={0};

    //AkelEdit class
    wndclass.style        =CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditShellProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursorW(NULL, (wchar_t *)IDC_IBEAM);
    wndclass.hbrBackground=NULL;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AES_AKELEDITCLASSW;
    bAkelEditClassRegisteredW=RegisterClassW(&wndclass);

    //RichEdit class
    if (bRegisterRichEdit)
    {
      wndclass.lpszClassName=AES_RICHEDITCLASSW;
      bRichEditClassRegisteredW=RegisterClassW(&wndclass);
    }

    if (!hAkelEditProcessHeap) hAkelEditProcessHeap=GetProcessHeap();
    if (!cfAkelEditColumnSel) cfAkelEditColumnSel=RegisterClipboardFormatW(L"MSDEVColumnSelect");
    if (!cfAkelEditText) cfAkelEditText=RegisterClipboardFormatW(L"AkelEditText");
    if (!hAkelEditCursorIBeam) hAkelEditCursorIBeam=LoadCursorW(NULL, (wchar_t *)IDC_IBEAM);
    if (!hAkelEditCursorArrow) hAkelEditCursorArrow=LoadCursorW(NULL, (wchar_t *)IDC_ARROW);
    if (!hAkelEditCursorMargin) hAkelEditCursorMargin=LoadCursorW(hInstance, (wchar_t *)IDC_AEMARGIN);
    if (!hAkelEditCursorHand) hAkelEditCursorHand=LoadCursorW(hInstance, (wchar_t *)IDC_AEHAND);
    if (!hAkelEditCursorMCenterAll) hAkelEditCursorMCenterAll=LoadCursorW(hInstance, (wchar_t *)IDC_AEMCENTERALL);
    if (!hAkelEditCursorMCenterLeftRight) hAkelEditCursorMCenterLeftRight=LoadCursorW(hInstance, (wchar_t *)IDC_AEMCENTERLEFTRIGHT);
    if (!hAkelEditCursorMCenterTopBottom) hAkelEditCursorMCenterTopBottom=LoadCursorW(hInstance, (wchar_t *)IDC_AEMCENTERTOPBOTTOM);
    if (!hAkelEditCursorMLeft) hAkelEditCursorMLeft=LoadCursorW(hInstance, (wchar_t *)IDC_AEMLEFT);
    if (!hAkelEditCursorMLeftTop) hAkelEditCursorMLeftTop=LoadCursorW(hInstance, (wchar_t *)IDC_AEMLEFTTOP);
    if (!hAkelEditCursorMTop) hAkelEditCursorMTop=LoadCursorW(hInstance, (wchar_t *)IDC_AEMTOP);
    if (!hAkelEditCursorMRightTop) hAkelEditCursorMRightTop=LoadCursorW(hInstance, (wchar_t *)IDC_AEMRIGHTTOP);
    if (!hAkelEditCursorMRight) hAkelEditCursorMRight=LoadCursorW(hInstance, (wchar_t *)IDC_AEMRIGHT);
    if (!hAkelEditCursorMRightBottom) hAkelEditCursorMRightBottom=LoadCursorW(hInstance, (wchar_t *)IDC_AEMRIGHTBOTTOM);
    if (!hAkelEditCursorMBottom) hAkelEditCursorMBottom=LoadCursorW(hInstance, (wchar_t *)IDC_AEMBOTTOM);
    if (!hAkelEditCursorMLeftBottom) hAkelEditCursorMLeftBottom=LoadCursorW(hInstance, (wchar_t *)IDC_AEMLEFTBOTTOM);
    if (!hAkelEditBitmapMCenterAll) hAkelEditBitmapMCenterAll=(HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP_MCENTERALL), IMAGE_BITMAP, 0, 0, 0);
    if (!hAkelEditBitmapMCenterLeftRight) hAkelEditBitmapMCenterLeftRight=(HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP_MCENTERLEFTRIGHT), IMAGE_BITMAP, 0, 0, 0);
    if (!hAkelEditBitmapMCenterTopBottom) hAkelEditBitmapMCenterTopBottom=(HBITMAP)LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_BITMAP_MCENTERTOPBOTTOM), IMAGE_BITMAP, 0, 0, 0);
  }
  return bAkelEditClassRegisteredW;
}

BOOL AE_UnregisterClassA(HINSTANCE hInstance)
{
  if (hAkelEditBitmapMCenterAll)
  {
    DeleteObject(hAkelEditBitmapMCenterAll);
    hAkelEditBitmapMCenterAll=NULL;
  }
  if (hAkelEditBitmapMCenterLeftRight)
  {
    DeleteObject(hAkelEditBitmapMCenterLeftRight);
    hAkelEditBitmapMCenterLeftRight=NULL;
  }
  if (hAkelEditBitmapMCenterTopBottom)
  {
    DeleteObject(hAkelEditBitmapMCenterTopBottom);
    hAkelEditBitmapMCenterTopBottom=NULL;
  }
  AE_HighlightDeleteThemeAll();
  AE_StackFontItemsFreeA(&hAkelEditFontsStackA);
  AE_StackBitmapItemsFree(&hAkelEditBitmapsStack);
  AE_StackWindowFree(&hAkelEditWindowsStack);

  if (bAkelEditClassRegisteredA)
  {
    if (UnregisterClassA(AES_AKELEDITCLASSA, hInstance))
      bAkelEditClassRegisteredA=FALSE;
    if (UnregisterClassA(AES_RICHEDITCLASSA, hInstance))
      bRichEditClassRegisteredA=FALSE;
  }
  return !bAkelEditClassRegisteredA;
}

BOOL AE_UnregisterClassW(HINSTANCE hInstance)
{
  if (hAkelEditBitmapMCenterAll)
  {
    DeleteObject(hAkelEditBitmapMCenterAll);
    hAkelEditBitmapMCenterAll=NULL;
  }
  if (hAkelEditBitmapMCenterLeftRight)
  {
    DeleteObject(hAkelEditBitmapMCenterLeftRight);
    hAkelEditBitmapMCenterLeftRight=NULL;
  }
  if (hAkelEditBitmapMCenterTopBottom)
  {
    DeleteObject(hAkelEditBitmapMCenterTopBottom);
    hAkelEditBitmapMCenterTopBottom=NULL;
  }
  AE_HighlightDeleteThemeAll();
  AE_StackFontItemsFreeW(&hAkelEditFontsStackW);
  AE_StackBitmapItemsFree(&hAkelEditBitmapsStack);
  AE_StackWindowFree(&hAkelEditWindowsStack);

  if (bAkelEditClassRegisteredW)
  {
    if (UnregisterClassW(AES_AKELEDITCLASSW, hInstance))
      bAkelEditClassRegisteredW=FALSE;
    if (UnregisterClassW(AES_RICHEDITCLASSW, hInstance))
      bRichEditClassRegisteredW=FALSE;
  }
  return !bAkelEditClassRegisteredW;
}

LRESULT CALLBACK AE_EditShellProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AKELEDIT *ae;

  if (uMsg == WM_CREATE)
  {
    if (ae=AE_CreateWindowData(hWnd, (CREATESTRUCTA *)lParam, (AEEditProc)AE_EditProc))
    {
      //Register drop window
      //CoLockObjectExternal((LPUNKNOWN)&ae->idt, TRUE, FALSE);
      RegisterDragDrop(ae->hWndEdit, (LPDROPTARGET)&ae->idt);
      return 0;
    }
    return -1;
  }

  if (ae=AE_StackWindowGet(&hAkelEditWindowsStack, hWnd))
  {
    //Move founded AKELEDIT in first place. To make next AE_StackWindowGet calls faster.
    AE_StackWindowMakeFirst(&hAkelEditWindowsStack, ae);

    //if (ae->bSkipMessages)
    //  return 0;

    return AE_EditProc(ae, uMsg, wParam, lParam);
  }

  if (!IsWindowUnicode(hWnd))
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  else
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK AE_EditProc(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //// Clone window processing

  AE_ActivateClone(lpAkelEditPrev, ae);
  lpAkelEditPrev=ae;


  //// Character input: Alt + NumPad

  if (uMsg != WM_CHAR)
  {
    if (uMsg == WM_KILLFOCUS)
      ae->nAltChar=AEAC_NONE;
    else if (ae->nAltChar == AEAC_KEYUP)
      ae->nAltChar=AEAC_NONE;
  }


  //// AEM_* AkelEdit control messages

  if (uMsg >= WM_USER)
  {
    if (uMsg >= AEM_CANUNDO)
      goto CanUndo;

    //Text retrieval and modification
    if (uMsg == AEM_SETTEXTA)
    {
      return AE_SetTextAnsi(ae, CP_ACP, (char *)lParam, wParam, AELB_ASINPUT);
    }
    if (uMsg == AEM_SETTEXTW)
    {
      return AE_SetText(ae, (wchar_t *)lParam, wParam, AELB_ASINPUT, FALSE);
    }
    if (uMsg == AEM_APPENDTEXTA)
    {
      AEAPPENDTEXTA *at=(AEAPPENDTEXTA *)lParam;

      AE_AppendTextAnsi(ae, at->nCodePage, at->pText, at->dwTextLen, at->nNewLine);
      return 0;
    }
    if (uMsg == AEM_APPENDTEXTW)
    {
      AEAPPENDTEXTW *at=(AEAPPENDTEXTW *)lParam;

      AE_AppendText(ae, at->pText, at->dwTextLen, at->nNewLine);
      return 0;
    }
    if (uMsg == AEM_REPLACESELA)
    {
      AEREPLACESELA *rs=(AEREPLACESELA *)lParam;

      AE_ReplaceSelAnsi(ae, rs->nCodePage, rs->pText, rs->dwTextLen, rs->nNewLine, rs->bColumnSel, rs->ciInsertStart, rs->ciInsertEnd);
      return 0;
    }
    if (uMsg == AEM_REPLACESELW)
    {
      AEREPLACESELW *rs=(AEREPLACESELW *)lParam;

      AE_ReplaceSel(ae, rs->pText, rs->dwTextLen, rs->nNewLine, rs->bColumnSel, rs->ciInsertStart, rs->ciInsertEnd);
      return 0;
    }
    if (uMsg == AEM_GETTEXTRANGEA)
    {
      AETEXTRANGEA *tr=(AETEXTRANGEA *)lParam;

      return AE_GetTextRangeAnsi(ae, tr->nCodePage, tr->lpDefaultChar, tr->lpUsedDefChar, &tr->cr.ciMin, &tr->cr.ciMax, tr->pBuffer, tr->dwBufferMax, tr->nNewLine, tr->bColumnSel, tr->bFillSpaces);
    }
    if (uMsg == AEM_GETTEXTRANGEW)
    {
      AETEXTRANGEW *tr=(AETEXTRANGEW *)lParam;

      return AE_GetTextRange(ae, &tr->cr.ciMin, &tr->cr.ciMax, tr->pBuffer, tr->dwBufferMax, tr->nNewLine, tr->bColumnSel, tr->bFillSpaces);
    }
    if (uMsg == AEM_STREAMIN)
    {
      AESTREAMIN *aesi=(AESTREAMIN *)lParam;

      return AE_StreamIn(ae, wParam, aesi);
    }
    if (uMsg == AEM_STREAMOUT)
    {
      AESTREAMOUT *aeso=(AESTREAMOUT *)lParam;

      return AE_StreamOut(ae, wParam, aeso);
    }
    if (uMsg == AEM_CANPASTE)
    {
      return AE_EditCanPaste(ae);
    }
    if (uMsg == AEM_PASTE)
    {
      return AE_EditPasteFromClipboard(ae, lParam);
    }
    if (uMsg == AEM_CUT)
    {
      AE_EditCut(ae);
      return 0;
    }
    if (uMsg == AEM_COPY)
    {
      AE_EditCopyToClipboard(ae);
      return 0;
    }
    if (uMsg == AEM_CHECKCODEPAGE)
    {
      return AE_CheckCodepage(ae, wParam);
    }
    if (uMsg == AEM_FINDTEXTA)
    {
      return AE_FindTextAnsi(ae, CP_ACP, (AEFINDTEXTA *)lParam);
    }
    if (uMsg == AEM_FINDTEXTW)
    {
      return AE_FindText(ae, (AEFINDTEXTW *)lParam);
    }
    if (uMsg == AEM_ISMATCHA)
    {
      return AE_IsMatchAnsi(ae, CP_ACP, (AEFINDTEXTA *)lParam, (AECHARINDEX *)wParam);
    }
    if (uMsg == AEM_ISMATCHW)
    {
      return AE_IsMatch(ae, (AEFINDTEXTW *)lParam, (AECHARINDEX *)wParam);
    }
    if (uMsg == AEM_KEYDOWN)
    {
      if (ae->popt->dwRichEventMask & ENM_KEYEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      AE_KeyDown(ae, wParam, (lParam & AEMOD_ALT), (lParam & AEMOD_SHIFT), (lParam & AEMOD_CONTROL));
      return 0;
    }
    if (uMsg == AEM_DRAGDROP)
    {
      if (wParam == AEDD_GETDRAGWINDOW)
      {
        if (lpAkelEditDrag)
          return (LRESULT)lpAkelEditDrag->hWndEdit;
        return (LRESULT)NULL;
      }
      if (wParam == AEDD_STOPDRAG)
      {
        lpAkelEditDrag=NULL;
        return 0;
      }
      return 0;
    }
    if (uMsg == AEM_CHARAT)
    {
      AECHARINDEX *ciPos=(AECHARINDEX *)wParam;

      return AE_CharAtIndex(ciPos);
    }

    //Undo and Redo
    CanUndo:
    if (uMsg >= AEM_EXGETSEL)
      goto ExGetSel;

    if (uMsg == AEM_CANUNDO)
    {
      if (ae->popt->dwOptions & AECO_READONLY)
        return FALSE;
      return AE_EditCanUndo(ae);
    }
    if (uMsg == AEM_CANREDO)
    {
      if (ae->popt->dwOptions & AECO_READONLY)
        return FALSE;
      return AE_EditCanRedo(ae);
    }
    if (uMsg == AEM_UNDO)
    {
      AE_EditUndo(ae);
      return 0;
    }
    if (uMsg == AEM_REDO)
    {
      AE_EditRedo(ae);
      return 0;
    }
    if (uMsg == AEM_EMPTYUNDOBUFFER)
    {
      AE_EmptyUndoBuffer(ae);
      return 0;
    }
    if (uMsg == AEM_STOPGROUPTYPING)
    {
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    if (uMsg == AEM_BEGINUNDOACTION)
    {
      ae->ptxt->bLockGroupStopExt=TRUE;
      return 0;
    }
    if (uMsg == AEM_ENDUNDOACTION)
    {
      ae->ptxt->bLockGroupStopExt=FALSE;
      return 0;
    }
    if (uMsg == AEM_LOCKCOLLECTUNDO)
    {
      ae->ptxt->bLockCollectUndo=wParam;
      return 0;
    }
    if (uMsg == AEM_GETUNDOLIMIT)
    {
      return ae->ptxt->dwUndoLimit;
    }
    if (uMsg == AEM_SETUNDOLIMIT)
    {
      ae->ptxt->dwUndoLimit=wParam;
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    if (uMsg == AEM_GETMODIFY)
    {
      return ae->ptxt->bModified;
    }
    if (uMsg == AEM_SETMODIFY)
    {
      AE_SetModify(ae, wParam);
      return 0;
    }
    if (uMsg == AEM_UNDOBUFFERSIZE)
    {
      return AE_StackUndoSize(ae);
    }

    //Text coordinates
    ExGetSel:
    if (uMsg >= AEM_CHARFROMPOS)
      goto CharFromPos;

    if (uMsg == AEM_EXGETSEL)
    {
      AECHARINDEX *ciMin=(AECHARINDEX *)wParam;
      AECHARINDEX *ciMax=(AECHARINDEX *)lParam;

      if (ciMin) *ciMin=ae->ciSelStartIndex;
      if (ciMax) *ciMax=ae->ciSelEndIndex;

      if (!AE_IndexCompareEx(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        return FALSE;
      return TRUE;
    }
    if (uMsg == AEM_EXSETSEL)
    {
      AE_SetSelectionPos(ae, (AECHARINDEX *)lParam, (AECHARINDEX *)wParam, FALSE, 0, AESCT_SETSELMESSAGE);
      return 0;
    }
    if (uMsg == AEM_GETSEL)
    {
      AECHARINDEX *lpciCaret=(AECHARINDEX *)wParam;
      AESELECTION *aes=(AESELECTION *)lParam;

      return AE_AkelEditGetSel(ae, aes, lpciCaret);
    }
    if (uMsg == AEM_SETSEL)
    {
      AECHARINDEX *lpciCaret=(AECHARINDEX *)wParam;
      AESELECTION *aes=(AESELECTION *)lParam;

      AE_AkelEditSetSel(ae, aes, lpciCaret);
      return 0;
    }
    if (uMsg == AEM_GETCOLUMNSEL)
    {
      return ae->bColumnSel;
    }
    if (uMsg == AEM_UPDATESEL)
    {
      AE_UpdateSelection(ae, wParam);
      return 0;
    }
    if (uMsg == AEM_GETLINENUMBER)
    {
      if (wParam == AEGL_LINECOUNT)
        return ae->ptxt->nLineCount + 1;
      else if (wParam == AEGL_FIRSTSELLINE)
        return ae->ciSelStartIndex.nLine;
      else if (wParam == AEGL_LASTSELLINE)
        return ae->ciSelEndIndex.nLine;
      else if (wParam == AEGL_CARETLINE)
        return ae->ciCaretIndex.nLine;
      else if (wParam == AEGL_FIRSTVISIBLELINE)
        return AE_GetFirstVisibleLine(ae);
      else if (wParam == AEGL_LASTVISIBLELINE)
        return AE_GetLastVisibleLine(ae);
      else if (wParam == AEGL_FIRSTFULLVISIBLELINE)
        return AE_GetFirstFullVisibleLine(ae);
      else if (wParam == AEGL_LASTFULLVISIBLELINE)
        return AE_GetLastFullVisibleLine(ae);
      return 0;
    }
    if (uMsg == AEM_GETINDEX)
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return (LRESULT)AE_GetIndex(ae, wParam, ciCharIndex, ciCharIndex, FALSE);
    }
    if (uMsg == AEM_GETLINEINDEX)
    {
      AELINEDATA *lpLine;
      int nLine=wParam;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      if (lpLine=AE_GetLineData(ae, nLine))
      {
        ciCharIndex->nLine=nLine;
        ciCharIndex->lpLine=lpLine;
        ciCharIndex->nCharInLine=0;
        return TRUE;
      }
      return FALSE;
    }
    if (uMsg == AEM_INDEXUPDATE)
    {
      return AE_IndexUpdate(ae, (AECHARINDEX *)lParam);
    }
    if (uMsg == AEM_INDEXCOMPARE)
    {
      return AE_IndexCompare((AECHARINDEX *)wParam, (AECHARINDEX *)lParam);
    }
    if (uMsg == AEM_INDEXSUBTRACT)
    {
      AEINDEXSUBTRACT *aeis=(AEINDEXSUBTRACT *)lParam;

      return AE_IndexSubtract(ae, aeis->ciChar1, aeis->ciChar2, aeis->nNewLine, aeis->bColumnSel, TRUE);
    }
    if (uMsg == AEM_INDEXOFFSET)
    {
      AEINDEXOFFSET *aeio=(AEINDEXOFFSET *)lParam;

      return AE_IndexOffset(ae, aeio->ciCharIn, aeio->ciCharOut, aeio->nOffset, aeio->nNewLine);
    }
    if (uMsg == AEM_INDEXTORICHOFFSET)
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_AkelIndexToRichOffset(ae, ciCharIndex);
    }
    if (uMsg == AEM_RICHOFFSETTOINDEX)
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      AE_RichOffsetToAkelIndex(ae, wParam, ciCharIndex);
      return 0;
    }
    if (uMsg == AEM_ADDPOINT)
    {
      AEPOINT *lpSourcePoint=(AEPOINT *)lParam;
      AEPOINT *lpTargetPoint;

      if (lpTargetPoint=AE_StackPointInsert(ae, &lpSourcePoint->ciPoint))
      {
        if (lpSourcePoint->nPointOffset == AEPTO_CALC)
          lpTargetPoint->nPointOffset=AE_AkelIndexToRichOffset(ae, &lpSourcePoint->ciPoint);
        else
          lpTargetPoint->nPointOffset=lpSourcePoint->nPointOffset;
        lpTargetPoint->nPointLen=lpSourcePoint->nPointLen;
        lpTargetPoint->dwUserData=lpSourcePoint->dwUserData;
        lpTargetPoint->dwFlags=lpSourcePoint->dwFlags;
      }
      return (LRESULT)lpTargetPoint;
    }
    if (uMsg == AEM_DELPOINT)
    {
      AEPOINT *lpPoint=(AEPOINT *)wParam;

      AE_StackPointDelete(ae, lpPoint);
      return 0;
    }
    if (uMsg == AEM_GETPOINTSTACK)
    {
      return (LRESULT)&ae->ptxt->hPointsStack;
    }
    if (uMsg == AEM_GETWRAPLINE)
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetWrapLine(ae, wParam, ciCharIndex);
    }
    if (uMsg == AEM_GETUNWRAPLINE)
    {
      return AE_GetUnwrapLine(ae, wParam);
    }
    if (uMsg == AEM_GETNEXTBREAK)
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetNextBreak(ae, ciCharIndex, ciCharIndex, FALSE, wParam?wParam:ae->popt->dwWordBreak);
    }
    if (uMsg == AEM_GETPREVBREAK)
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetPrevBreak(ae, ciCharIndex, ciCharIndex, FALSE, wParam?wParam:ae->popt->dwWordBreak);
    }
    if (uMsg == AEM_ISDELIMITER)
    {
      return AE_IsDelimiter(ae, (AECHARINDEX *)lParam, wParam);
    }
    if (uMsg == AEM_INDEXTOCOLUMN)
    {
      AECHARINDEX ciChar=*(AECHARINDEX *)lParam;
      int nTabStop=LOWORD(wParam);
      BOOL bWrappedScan=HIWORD(wParam);
      int nScanLimit;
      int nColumn=0;
      int nResult;

      //Tab current size if zero
      if (!nTabStop) nTabStop=ae->ptxt->nTabStop;

      nScanLimit=min(ciChar.nCharInLine, ciChar.lpLine->nLineLen);
      nResult=ciChar.nCharInLine - nScanLimit;

      while (ciChar.lpLine)
      {
        if (nTabStop == 1)
        {
          nColumn+=nScanLimit;
        }
        else
        {
          for (ciChar.nCharInLine=0; ciChar.nCharInLine < nScanLimit; ++ciChar.nCharInLine)
          {
            if (ciChar.lpLine->wpLine[ciChar.nCharInLine] == L'\t')
              nColumn+=nTabStop - nColumn % nTabStop;
            else
              ++nColumn;
          }
        }
        nResult+=nColumn;

        if (bWrappedScan && AE_PrevLine(&ciChar) && ciChar.lpLine->nLineBreak == AELB_WRAP)
        {
          nScanLimit=ciChar.lpLine->nLineLen;
          nColumn=0;
        }
        else break;
      }
      return nResult;
    }
    if (uMsg == AEM_COLUMNTOINDEX)
    {
      AECHARINDEX *ciInput=(AECHARINDEX *)lParam;
      AECHARINDEX ciChar=*ciInput;
      int nTabStop=LOWORD(wParam);
      BOOL bWrappedScan=HIWORD(wParam);
      int nColumn=ciChar.nCharInLine;
      int nCount=0;

      //Tab current size if zero
      if (!nTabStop) nTabStop=ae->ptxt->nTabStop;

      while (ciChar.lpLine)
      {
        if (nTabStop == 1)
        {
          if (nColumn - nCount <= ciChar.lpLine->nLineLen)
          {
            ciChar.nCharInLine=nColumn - nCount;
            nCount=nColumn;
            goto ColumnToIndexEnd;
          }
          nCount+=ciChar.lpLine->nLineLen;
        }
        else
        {
          for (ciChar.nCharInLine=0; ciChar.nCharInLine < ciChar.lpLine->nLineLen; ++ciChar.nCharInLine)
          {
            if (nCount >= nColumn)
              goto ColumnToIndexEnd;

            if (ciChar.lpLine->wpLine[ciChar.nCharInLine] == L'\t')
              nCount+=nTabStop - nCount % nTabStop;
            else
              ++nCount;
          }
        }

        if (bWrappedScan && ciChar.lpLine->nLineBreak == AELB_WRAP)
        {
          AE_NextLine(&ciChar);
        }
        else
        {
          ciChar.nCharInLine=ciChar.lpLine->nLineLen;
          goto ColumnToIndexEnd;
        }
      }
      ColumnToIndexEnd:
      *ciInput=ciChar;
      return nCount;
    }
    if (uMsg == AEM_INDEXINURL)
    {
      AECHARINDEX *ciChar=(AECHARINDEX *)wParam;
      AECHARRANGE *crRange=(AECHARRANGE *)lParam;

      return AE_HighlightFindUrl(ae, ciChar, AEHF_FINDFIRSTCHAR, ae->ptxt->nLineCount, crRange);
    }

    //Screen coordinates
    CharFromPos:
    if (uMsg >= AEM_CONTROLCLASS)
      goto ControlClass;

    if (uMsg == AEM_CHARFROMPOS)
    {
      POINT *pt=(POINT *)wParam;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetCharFromPos(ae, pt, ciCharIndex, NULL, ae->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
    }
    if (uMsg == AEM_POSFROMCHAR)
    {
      POINT *pt=(POINT *)wParam;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      AE_GetPosFromChar(ae, ciCharIndex, NULL, pt);
      return 0;
    }
    if (uMsg == AEM_GETRECT)
    {
      RECT *rcDraw=(RECT *)lParam;

      *rcDraw=ae->rcDraw;
      return 0;
    }
    if (uMsg == AEM_SETRECT)
    {
      RECT *rcDraw=(RECT *)lParam;

      AE_SetDrawRect(ae, rcDraw, wParam);
      if (ae->ptxt->dwWordWrap)
      {
        AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        AE_StackUpdateClones(ae);
      }
      AE_UpdateScrollBars(ae, SB_BOTH);
      AE_NotifySetRect(ae);
      return 0;
    }
    if (uMsg == AEM_GETSCROLLPOS)
    {
      POINT *ptMax=(POINT *)wParam;
      POINT *ptPos=(POINT *)lParam;

      if (ptMax)
      {
        ptMax->x=ae->ptxt->nHScrollMax;
        ptMax->y=ae->ptxt->nVScrollMax;
      }
      if (ptPos)
      {
        ptPos->x=ae->nHScrollPos;
        ptPos->y=ae->nVScrollPos;
      }
      return 0;
    }
    if (uMsg == AEM_SETSCROLLPOS)
    {
      POINT *pt=(POINT *)lParam;

      if (pt->x != ae->nHScrollPos)
        AE_ScrollEditWindow(ae, SB_HORZ, pt->x);
      if (pt->y != ae->nVScrollPos)
        AE_ScrollEditWindow(ae, SB_VERT, pt->y);
      return 0;
    }
    if (uMsg == AEM_SCROLL)
    {
      int nResult=0;

      if (wParam & AESB_HORZ)
      {
        nResult=AE_HScroll(ae, lParam, wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nAveCharWidth;
      }
      else if (wParam & AESB_VERT)
      {
        nResult=AE_VScroll(ae, lParam, wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nCharHeight;
      }
      return nResult;
    }
    if (uMsg == AEM_LINESCROLL)
    {
      int nResult=0;

      if (wParam & AESB_HORZ)
      {
        nResult=AE_HScrollLine(ae, lParam, wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nAveCharWidth;
      }
      else if (wParam & AESB_VERT)
      {
        nResult=AE_VScrollLine(ae, lParam, wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nCharHeight;
      }
      return nResult;
    }
    if (uMsg == AEM_SCROLLTOPOINT)
    {
      AESCROLLTOPOINT *stp=(AESCROLLTOPOINT *)lParam;

      return AE_ScrollToPointEx(ae, stp->dwFlags, &stp->ptPos, stp->nOffsetX, stp->nOffsetY);
    }
    if (uMsg == AEM_LOCKSCROLL)
    {
      if (wParam == SB_BOTH || wParam == SB_HORZ)
        ae->popt->bHScrollLock=lParam;
      if (wParam == SB_BOTH || wParam == SB_VERT)
        ae->popt->bVScrollLock=lParam;
      return 0;
    }
    if (uMsg == AEM_LOCKERASERECT)
    {
      AEERASE *lpErase=(AEERASE *)ae->hEraseStack.first;
      AEERASE *lpEraseNext;
      RECT *rcLockErase=(RECT *)lParam;
      RECT rcErase;
      BOOL bDelete;
      BOOL bResult=FALSE;

      //WM_ERASEBKGND came not from WM_PAINT - use all edit area
      if (!lpErase)
        lpErase=AE_StackEraseInsert(ae, &ae->rcEdit);

      while (lpErase)
      {
        if (IntersectRect(&rcErase, rcLockErase, &lpErase->rcErase))
        {
          if (lpErase->rcErase.left < rcLockErase->right && lpErase->rcErase.right > rcLockErase->left)
          {
            if (lpErase->rcErase.right > rcLockErase->right)
            {
              rcErase.left=rcLockErase->right;
              rcErase.top=lpErase->rcErase.top;
              rcErase.right=lpErase->rcErase.right;
              rcErase.bottom=lpErase->rcErase.bottom;
              AE_StackEraseInsert(ae, &rcErase);
            }
            if (lpErase->rcErase.left < rcLockErase->left)
            {
              rcErase.left=lpErase->rcErase.left;
              rcErase.top=lpErase->rcErase.top;
              rcErase.right=rcLockErase->left;
              rcErase.bottom=lpErase->rcErase.bottom;
              AE_StackEraseInsert(ae, &rcErase);
            }
          }
          if (lpErase->rcErase.top < rcLockErase->bottom && lpErase->rcErase.bottom > rcLockErase->top)
          {
            if (lpErase->rcErase.bottom > rcLockErase->bottom)
            {
              rcErase.left=lpErase->rcErase.left;
              rcErase.top=rcLockErase->bottom;
              rcErase.right=lpErase->rcErase.right;
              rcErase.bottom=lpErase->rcErase.bottom;
              AE_StackEraseInsert(ae, &rcErase);
            }
            if (lpErase->rcErase.top < rcLockErase->top)
            {
              rcErase.left=lpErase->rcErase.left;
              rcErase.top=lpErase->rcErase.top;
              rcErase.right=lpErase->rcErase.right;
              rcErase.bottom=rcLockErase->top;
              AE_StackEraseInsert(ae, &rcErase);
            }
          }
          bDelete=TRUE;
          bResult=TRUE;
        }
        else bDelete=FALSE;

        //Next erase rectangle
        lpEraseNext=lpErase->next;
        if (bDelete) AE_StackEraseDelete(ae, lpErase);
        lpErase=lpEraseNext;
      }
      return bResult;
    }
    if (uMsg == AEM_GETCHARSIZE)
    {
      if (wParam == AECS_HEIGHT)
        return ae->ptxt->nCharHeight;
      if (wParam == AECS_AVEWIDTH)
        return ae->ptxt->nAveCharWidth;
      if (wParam == AECS_INDEXWIDTH)
      {
        AECHARINDEX *ciChar=(AECHARINDEX *)lParam;
        POINT ptGlobal;

        if (ciChar->nCharInLine < ciChar->lpLine->nLineLen)
        {
          if (ciChar->lpLine->wpLine[ciChar->nCharInLine] == L'\t')
          {
            AE_GetPosFromChar(ae, ciChar, &ptGlobal, NULL);
            return ae->ptxt->nTabWidth - ptGlobal.x % ae->ptxt->nTabWidth;
          }
          else return AE_GetCharWidth(ae, ciChar->lpLine->wpLine + ciChar->nCharInLine, NULL);
        }
        return 0;
      }
      return 0;
    }
    if (uMsg == AEM_GETSTRWIDTH)
    {
      return AE_GetStringWidth(ae, (wchar_t *)wParam, lParam, 0);
    }
    if (uMsg == AEM_GETCARETPOS)
    {
      POINT *ptClient=(POINT *)wParam;
      POINT *ptGlobal=(POINT *)lParam;

      if (ptClient)
      {
        AE_GlobalToClient(ae, &ae->ptCaret, ptClient);
      }
      if (ptGlobal)
      {
        *ptGlobal=ae->ptCaret;
      }
      return ae->bCaretVisible;
    }
    if (uMsg == AEM_GETCARETHORZINDENT)
    {
      return ae->nCaretHorzIndent;
    }
    if (uMsg == AEM_SETCARETHORZINDENT)
    {
      ae->nCaretHorzIndent=wParam;
      return 0;
    }
    if (uMsg == AEM_CONVERTPOINT)
    {
      POINT *pt=(POINT *)lParam;

      if (wParam == AECPT_GLOBALTOCLIENT)
      {
        AE_GlobalToClient(ae, pt, pt);
      }
      if (wParam == AECPT_CLIENTTOGLOBAL)
      {
        AE_ClientToGlobal(ae, pt, pt);
      }
      return 0;
    }
    if (uMsg == AEM_POINTONMARGIN)
    {
      POINT *pt=(POINT *)wParam;

      return AE_IsPointOnMargin(ae, pt);
    }
    if (uMsg == AEM_POINTONSELECTION)
    {
      POINT *pt=(POINT *)wParam;

      return AE_IsPointOnSelection(ae, pt);
    }
    if (uMsg == AEM_POINTONURL)
    {
      POINT *pt=(POINT *)wParam;
      AECHARRANGE *lpcrLink=(AECHARRANGE *)lParam;
      AECHARRANGE crLink;
      DWORD dwResult;

      dwResult=AE_IsPointOnUrl(ae, pt, &crLink);
      if (lpcrLink) *lpcrLink=crLink;
      return dwResult;
    }
    if (uMsg == AEM_LINEFROMVPOS)
    {
      if (wParam == AECT_GLOBAL)
        return AE_VPos(ae, lParam, AEVPF_LINEFROMVPOS);
      if (wParam == AECT_CLIENT)
        return AE_VPos(ae, ae->nVScrollPos + (lParam - ae->rcDraw.top), AEVPF_LINEFROMVPOS);
    }
    if (uMsg == AEM_VPOSFROMLINE)
    {
      if (wParam == AECT_GLOBAL)
        return AE_VPos(ae, lParam, AEVPF_VPOSFROMLINE);
      if (wParam == AECT_CLIENT)
        return ae->rcDraw.top + (AE_VPos(ae, lParam, AEVPF_VPOSFROMLINE) - ae->nVScrollPos);
    }
    if (uMsg == AEM_GETMOUSESTATE)
    {
      if (wParam == AEMS_CAPTURE)
        return ae->dwMouseCapture;
      if (wParam == AEMS_SELECTION)
        return ae->dwMouseSelType;
      return 0;
    }

    //Options
    ControlClass:
    if (uMsg >= AEM_SHOWSCROLLBAR)
      goto ShowScrollbar;

    if (uMsg == AEM_CONTROLCLASS)
    {
      if (ae->bRichEditClass)
        return AECLASS_RICHEDIT;
      else
        return AECLASS_AKELEDIT;
    }
    if (uMsg == AEM_CONTROLVERSION)
    {
      DWORD ver[4]={AKELEDIT_ID};

      return MAKE_IDENTIFIER(ver[0], ver[1], ver[2], ver[3]);
    }
    if (uMsg == AEM_GETEVENTMASK)
    {
      return ae->popt->dwEventMask;
    }
    if (uMsg == AEM_SETEVENTMASK)
    {
      DWORD dwPrevMask=ae->popt->dwEventMask;

      ae->popt->dwEventMask=lParam;
      return dwPrevMask;
    }
    if (uMsg == AEM_GETOPTIONS)
    {
      return ae->popt->dwOptions;
    }
    if (uMsg == AEM_SETOPTIONS)
    {
      DWORD dwOptionsOld=ae->popt->dwOptions;
      DWORD dwOptionsNew=ae->popt->dwOptions;

      if (wParam == AECOOP_SET)
        dwOptionsNew=lParam;
      else if (wParam == AECOOP_OR)
        dwOptionsNew|=lParam;
      else if (wParam == AECOOP_AND)
        dwOptionsNew&=lParam;
      else if (wParam == AECOOP_XOR)
        dwOptionsNew&=~lParam;

      if (!(dwOptionsOld & AECO_ACTIVECOLUMN) && (dwOptionsNew & AECO_ACTIVECOLUMN))
      {
        AE_ActiveColumnCreate(ae);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackUpdateClones(ae);
      }
      else if ((dwOptionsOld & AECO_ACTIVECOLUMN) && !(dwOptionsNew & AECO_ACTIVECOLUMN))
      {
        AE_ActiveColumnErase(ae);
        AE_StackUpdateClones(ae);
      }
      if ((dwOptionsOld & AECO_CARETOUTEDGE) && !(dwOptionsNew & AECO_CARETOUTEDGE))
      {
        AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL);
      }
      if (!(dwOptionsOld & AECO_PAINTGROUP) != !(dwOptionsNew & AECO_PAINTGROUP))
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackUpdateClones(ae);
      }

      ae->popt->dwOptions=dwOptionsNew;
      return ae->popt->dwOptions;
    }
    if (uMsg == AEM_GETNEWLINE)
    {
      return MAKELONG(ae->popt->nInputNewLine, ae->popt->nOutputNewLine);
    }
    if (uMsg == AEM_SETNEWLINE)
    {
      if (wParam & AENL_INPUT)
        ae->popt->nInputNewLine=LOWORD(lParam);
      if (wParam & AENL_OUTPUT)
        ae->popt->nOutputNewLine=HIWORD(lParam);
      return 0;
    }
    if (uMsg == AEM_GETCOLORS)
    {
      AE_GetColors(ae, (AECOLORS *)lParam);
      return 0;
    }
    if (uMsg == AEM_SETCOLORS)
    {
      AE_SetColors(ae, (AECOLORS *)lParam);
      return 0;
    }
    if (uMsg == AEM_GETDETECTURL)
    {
      return ae->popt->bDetectUrl;
    }
    if (uMsg == AEM_SETDETECTURL)
    {
      ae->popt->bDetectUrl=wParam;
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackUpdateClones(ae);
      return 0;
    }
    if (uMsg == AEM_GETOVERTYPE)
    {
      return ae->popt->bOverType;
    }
    if (uMsg == AEM_SETOVERTYPE)
    {
      if (ae->popt->bOverType != (int)wParam)
      {
        ae->popt->bOverType=wParam;
        AE_UpdateCaret(ae, ae->bFocus);
      }
      return 0;
    }
    if (uMsg == AEM_GETCARETWIDTH)
    {
      POINT *pt=(POINT *)lParam;

      pt->x=ae->popt->nCaretInsertWidth;
      pt->y=ae->popt->nCaretOvertypeHeight;
      return 0;
    }
    if (uMsg == AEM_SETCARETWIDTH)
    {
      POINT *pt=(POINT *)lParam;

      ae->popt->nCaretInsertWidth=pt->x;
      ae->popt->nCaretOvertypeHeight=pt->y;
      AE_UpdateCaret(ae, ae->bFocus);
      return 0;
    }
    if (uMsg == AEM_GETTABSTOP)
    {
      return ae->ptxt->nTabStop;
    }
    if (uMsg == AEM_SETTABSTOP)
    {
      int nTabStop=(int)wParam;

      //Tab default size if zero
      if (nTabStop <= 0) nTabStop=AETAB_DEFAULTSIZE;

      if (ae->ptxt->nTabStop != nTabStop)
      {
        ae->ptxt->nTabStop=nTabStop;
        ae->ptxt->nTabWidth=ae->ptxt->nSpaceCharWidth * ae->ptxt->nTabStop;

        AE_CalcLinesWidth(ae, NULL, NULL, AECLW_FRESH);
        ae->ptCaret.x=0;
        ae->ptCaret.y=0;
        AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL);
        AE_UpdateCaret(ae, ae->bFocus);

        if (ae->ptxt->dwWordWrap) AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackUpdateClones(ae);
      }
      return 0;
    }
    if (uMsg == AEM_GETWORDWRAP)
    {
      DWORD *lpdwWrapLimit=(DWORD *)lParam;

      if (lpdwWrapLimit) *lpdwWrapLimit=ae->ptxt->dwWrapLimit;

      return ae->ptxt->dwWordWrap;
    }
    if (uMsg == AEM_SETWORDWRAP)
    {
      DWORD dwWordWrap=wParam;
      BOOL bUpdateWrap=FALSE;

      AE_NotifyChanging(ae, AETCT_WRAP);

      if ((dwWordWrap & AEWW_WORD) || (dwWordWrap & AEWW_SYMBOL))
      {
        if (!(dwWordWrap & AEWW_LIMITSYMBOL))
          dwWordWrap|=AEWW_LIMITPIXEL;
      }
      else dwWordWrap=0;

      if (ae->ptxt->dwWordWrap != dwWordWrap)
      {
        if (ae->ptxt->dwWordWrap && dwWordWrap)
          AE_UpdateWrap(ae, NULL, NULL, 0);

        ae->ptxt->dwWordWrap=dwWordWrap;
        bUpdateWrap=TRUE;
      }
      if (ae->ptxt->dwWrapLimit != (DWORD)lParam)
      {
        if (lParam >= 0)
        {
          ae->ptxt->dwWrapLimit=lParam;
          bUpdateWrap=TRUE;
        }
      }

      if (bUpdateWrap)
      {
        AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackUpdateClones(ae);
      }
      AE_NotifyChanged(ae); //AETCT_WRAP

      return 0;
    }
    if (uMsg == AEM_GETWORDDELIMITERS)
    {
      if (wParam) xmemcpy((wchar_t *)wParam, ae->popt->wszWordDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->popt->wszWordDelimiters)));
      return 0;
    }
    if (uMsg == AEM_SETWORDDELIMITERS)
    {
      if (lParam)
        xmemcpy(ae->popt->wszWordDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
      else
        xmemcpy(ae->popt->wszWordDelimiters, AES_WORDDELIMITERSW, sizeof(AES_WORDDELIMITERSW));
      return 0;
    }
    if (uMsg == AEM_GETWRAPDELIMITERS)
    {
      if (wParam) xmemcpy((wchar_t *)wParam, ae->ptxt->wszWrapDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->ptxt->wszWrapDelimiters)));
      return 0;
    }
    if (uMsg == AEM_SETWRAPDELIMITERS)
    {
      if (lParam)
        xmemcpy(ae->ptxt->wszWrapDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
      else
        xmemcpy(ae->ptxt->wszWrapDelimiters, AES_WRAPDELIMITERSW, sizeof(AES_WRAPDELIMITERSW));

      if (ae->ptxt->dwWordWrap & AEWW_WORD)
      {
        AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackUpdateClones(ae);
      }
      return 0;
    }
    if (uMsg == AEM_GETURLLEFTDELIMITERS)
    {
      if (wParam) xmemcpy((wchar_t *)wParam, ae->popt->wszUrlLeftDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->popt->wszUrlLeftDelimiters)));
      return 0;
    }
    if (uMsg == AEM_SETURLLEFTDELIMITERS)
    {
      if (lParam)
        xmemcpy(ae->popt->wszUrlLeftDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
      else
        xmemcpy(ae->popt->wszUrlLeftDelimiters, AES_URLLEFTDELIMITERSW, sizeof(AES_URLLEFTDELIMITERSW));
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackUpdateClones(ae);
      return 0;
    }
    if (uMsg == AEM_GETURLRIGHTDELIMITERS)
    {
      if (wParam) xmemcpy((wchar_t *)wParam, ae->popt->wszUrlRightDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->popt->wszUrlRightDelimiters)));
      return 0;
    }
    if (uMsg == AEM_SETURLRIGHTDELIMITERS)
    {
      if (lParam)
        xmemcpy(ae->popt->wszUrlRightDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
      else
        xmemcpy(ae->popt->wszUrlRightDelimiters, AES_URLRIGHTDELIMITERSW, sizeof(AES_URLRIGHTDELIMITERSW));
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackUpdateClones(ae);
      return 0;
    }
    if (uMsg == AEM_GETURLPREFIXES)
    {
      if (wParam) xmemcpy((wchar_t *)wParam, ae->popt->wszUrlPrefixes, min(lParam * sizeof(wchar_t), sizeof(ae->popt->wszUrlPrefixes)));
      return 0;
    }
    if (uMsg == AEM_SETURLPREFIXES)
    {
      int nPrefix;

      if (lParam)
        xmemcpy(ae->popt->wszUrlPrefixes, (wchar_t *)lParam, sizeof(ae->popt->wszUrlPrefixes));
      else
        xmemcpy(ae->popt->wszUrlPrefixes, AES_URLPREFIXESW, sizeof(AES_URLPREFIXESW));
      nPrefix=AE_GetUrlPrefixes(ae);
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackUpdateClones(ae);
      return nPrefix;
    }
    if (uMsg == AEM_GETURLMAXLENGTH)
    {
      return ae->popt->dwUrlMaxLength;
    }
    if (uMsg == AEM_SETURLMAXLENGTH)
    {
      ae->popt->dwUrlMaxLength=wParam;
      return 0;
    }
    if (uMsg == AEM_GETWORDBREAK)
    {
      return ae->popt->dwWordBreak;
    }
    if (uMsg == AEM_SETWORDBREAK)
    {
      DWORD dwWordBreak=ae->popt->dwWordBreak;

      ae->popt->dwWordBreak=wParam;
      return dwWordBreak;
    }
    if (uMsg == AEM_GETMARKER)
    {
      DWORD *lpdwColumnMarkerType=(DWORD *)wParam;

      if (lpdwColumnMarkerType) *lpdwColumnMarkerType=ae->popt->dwColumnMarkerType;

      return (LRESULT)ae->popt->dwColumnMarkerPos;
    }
    if (uMsg == AEM_SETMARKER)
    {
      if (ae->popt->dwColumnMarkerType != wParam ||
          ae->popt->dwColumnMarkerPos != (DWORD)lParam)
      {
        if (lParam >= 0)
        {
          AE_ColumnMarkerErase(ae);
          ae->popt->dwColumnMarkerType=wParam;
          ae->popt->dwColumnMarkerPos=lParam;
          AE_ColumnMarkerDraw(ae);
          AE_StackUpdateClones(ae);
        }
      }
      return 0;
    }
    if (uMsg == AEM_GETLINEGAP)
    {
      return (LRESULT)ae->ptxt->nLineGap;
    }
    if (uMsg == AEM_SETLINEGAP)
    {
      if (ae->ptxt->nLineGap != (int)wParam)
      {
        if ((int)wParam >= 0)
        {
          int nFirstVisibleLine=0;

          if (!ae->popt->bVScrollLock)
            nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
          ae->ptxt->nCharHeight=(ae->ptxt->nCharHeight - ae->ptxt->nLineGap) + wParam;
          ae->ptxt->nLineGap=wParam;

          ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
          AE_UpdateScrollBars(ae, SB_VERT);
          ae->ptCaret.x=0;
          ae->ptCaret.y=0;
          AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL);
          if (!ae->popt->bVScrollLock)
            AE_VScrollLine(ae, nFirstVisibleLine - AE_GetFirstVisibleLine(ae), AESB_ALIGNTOP);
          AE_UpdateCaret(ae, ae->bFocus);

          InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
          AE_StackUpdateClones(ae);
        }
      }
      return 0;
    }
    if (uMsg == AEM_GETTEXTLIMIT)
    {
      return ae->ptxt->dwTextLimit;
    }
    if (uMsg == AEM_SETTEXTLIMIT)
    {
      ae->ptxt->dwTextLimit=wParam;
      return 0;
    }

    //Draw
    ShowScrollbar:
    if (uMsg >= AEM_GETFOLDSTACK)
      goto GetFoldStack;

    if (uMsg == AEM_SHOWSCROLLBAR)
    {
      BOOL bUpdate=FALSE;

      if (wParam == SB_BOTH || wParam == SB_HORZ)
      {
        if (ae->bHScrollShow != lParam)
        {
          ae->bHScrollShow=lParam;
          bUpdate=TRUE;
        }
      }
      if (wParam == SB_BOTH || wParam == SB_VERT)
      {
        if (ae->bVScrollShow != lParam)
        {
          ae->bVScrollShow=lParam;
          bUpdate=TRUE;
        }
      }
      if (bUpdate)
        ShowScrollBar(ae->hWndEdit, wParam, lParam);

      return 0;
    }
    if (uMsg == AEM_UPDATESCROLLBAR)
    {
      AE_UpdateScrollBars(ae, wParam);
      return 0;
    }
    if (uMsg == AEM_UPDATECARET)
    {
      return AE_UpdateCaret(ae, ae->bFocus);
    }
    if (uMsg == AEM_UPDATESIZE)
    {
      AE_UpdateSize(ae);
      return 0;
    }
    if (uMsg == AEM_LOCKUPDATE)
    {
      if (wParam & AELU_SCROLLBAR)
      {
        if (lParam)
          ae->popt->dwLockUpdate|=AELU_SCROLLBAR;
        else
          ae->popt->dwLockUpdate&=~AELU_SCROLLBAR;
      }
      if (wParam & AELU_CARET)
      {
        if (lParam)
          ae->popt->dwLockUpdate|=AELU_CARET;
        else
          ae->popt->dwLockUpdate&=~AELU_CARET;
      }
      return ae->popt->dwLockUpdate;
    }
    if (uMsg == AEM_HIDESELECTION)
    {
      AE_HideSelection(ae, wParam);
      return 0;
    }

    //Folding
    GetFoldStack:
    if (uMsg >= AEM_CREATEDOCUMENT)
      goto CreateWindowData;

    if (uMsg == AEM_GETFOLDSTACK)
    {
      return (LRESULT)&ae->ptxt->hFoldsStack;
    }
    if (uMsg == AEM_GETFOLDCOUNT)
    {
      return ae->ptxt->nFoldCount;
    }
    if (uMsg == AEM_ADDFOLD)
    {
      AEPOINT *lpMinPoint=(AEPOINT *)wParam;
      AEPOINT *lpMaxPoint=(AEPOINT *)lParam;

      return (LRESULT)AE_StackFoldInsert(ae, lpMinPoint, lpMaxPoint);
    }
    if (uMsg == AEM_DELETEFOLD)
    {
      int nFirstVisibleLine=-1;
      int nResult;
      BOOL bUpdate=lParam;

      if (bUpdate)
      {
        if (!ae->popt->bVScrollLock)
          nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
      }

      if (wParam)
        nResult=AE_StackFoldDelete(ae, (AEFOLD *)wParam);
      else
        nResult=AE_StackFoldFree(ae);

      if (bUpdate && nResult)
      {
        AE_StackFoldUpdate(ae, nFirstVisibleLine);
      }
      return nResult;
    }
    if (uMsg == AEM_ISFOLDVALID)
    {
      return (LRESULT)AE_StackFoldIsValid(ae, (AEFOLD *)wParam);
    }
    if (uMsg == AEM_FINDFOLD)
    {
      AEFINDFOLD *ff=(AEFINDFOLD *)wParam;

      AE_StackFindFold(ae, ff->dwFlags, ff->dwFindIt, NULL, &ff->lpParent, &ff->lpPrevSubling);
      return 0;
    }
    if (uMsg == AEM_NEXTFOLD)
    {
      return (LRESULT)AE_NextFold((AEFOLD *)wParam, lParam);
    }
    if (uMsg == AEM_PREVFOLD)
    {
      return (LRESULT)AE_PrevFold((AEFOLD *)wParam, lParam);
    }
    if (uMsg == AEM_ISLINECOLLAPSED)
    {
      return (LRESULT)AE_StackIsLineCollapsed(ae, wParam);
    }
    if (uMsg == AEM_COLLAPSELINE ||
        uMsg == AEM_COLLAPSEFOLD)
    {
      int nFirstVisibleLine=-1;
      int nResult;

      if (!(lParam & AECF_NOUPDATE))
      {
        if (!ae->popt->bVScrollLock)
          nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
      }

      if (uMsg == AEM_COLLAPSELINE)
        nResult=AE_StackLineCollapse(ae, wParam, lParam);
      else
        nResult=AE_StackFoldCollapse(ae, (AEFOLD *)wParam, lParam);
      ae->ptxt->lpVPosFold=NULL;

      if (nResult && !(lParam & AECF_NOUPDATE))
      {
        AE_StackFoldUpdate(ae, nFirstVisibleLine);
      }
      return nResult;
    }
    if (uMsg == AEM_UPDATEFOLD)
    {
      return AE_StackFoldUpdate(ae, lParam);
    }

    //Window data
    CreateWindowData:
    if (uMsg >= AEM_ADDCLONE)
      goto AddClone;

    if (uMsg == AEM_CREATEDOCUMENT)
    {
      AKELEDIT *aeNew;

      if (aeNew=AE_CreateWindowData(ae->hWndEdit, (CREATESTRUCTA *)lParam, (AEEditProc)AE_EditProc))
      {
        //Initially unassigned. Later assigned with AEM_SETDOCUMENT.
        aeNew->hWndEdit=NULL;
      }
      return (LRESULT)aeNew;
    }
    if (uMsg == AEM_DELETEDOCUMENT)
    {
      AE_DestroyWindowData((AKELEDIT *)wParam);
      return 0;
    }
    if (uMsg == AEM_GETDOCUMENT)
    {
      return (LRESULT)ae;
    }
    if (uMsg == AEM_SETDOCUMENT)
    {
      return (LRESULT)AE_SetWindowData(ae, (AKELEDIT *)wParam, lParam);
    }
    if (uMsg == AEM_GETDOCUMENTPROC)
    {
      AKELEDIT *aeHandle=(AKELEDIT *)wParam;

      if (aeHandle)
        return (LRESULT)aeHandle->lpEditProc;
      else
        return (LRESULT)ae->lpEditProc;
    }
    if (uMsg == AEM_GETDOCUMENTWINDOW)
    {
      AKELEDIT *aeHandle=(AKELEDIT *)wParam;

      if (aeHandle)
        return (LRESULT)aeHandle->hWndEdit;
      else
        return (LRESULT)ae->hWndEdit;
    }
    if (uMsg == AEM_SENDMESSAGE)
    {
      AESENDMESSAGE *psm=(AESENDMESSAGE *)lParam;
      HWND hWndEdit=ae->hWndEdit;
      LRESULT lResult;

      if (psm->hDoc && (AKELEDIT *)psm->hDoc != ae)
        AE_SetWindowData(ae, (AKELEDIT *)psm->hDoc, AESWD_NOALL);

      if (!ae->bUnicodeWindow)
        lResult=SendMessageA(hWndEdit, psm->uMsg, psm->wParam, psm->lParam);
      else
        lResult=SendMessageW(hWndEdit, psm->uMsg, psm->wParam, psm->lParam);

      if (psm->hDoc && (AKELEDIT *)psm->hDoc != ae)
        AE_SetWindowData((AKELEDIT *)psm->hDoc, ae, AESWD_NOALL);
      return lResult;
    }

    //Clones
    AddClone:
    if (uMsg >= AEM_STARTPRINTDOC)
      goto StartPrintDoc;

    if (uMsg == AEM_ADDCLONE)
    {
      AKELEDIT *aeClone;

      if (aeClone=AE_StackWindowGet(&hAkelEditWindowsStack, (HWND)wParam))
      {
        AE_StackCloneAdd(ae, aeClone);
        return TRUE;
      }
      return FALSE;
    }
    if (uMsg == AEM_DELCLONE)
    {
      AECLONE *aec;

      if (wParam)
      {
        if (aec=AE_StackCloneGet(ae, (HWND)wParam))
          AE_StackCloneDelete(aec);
      }
      else AE_StackCloneDeleteAll(ae);

      return 0;
    }
    if (uMsg == AEM_GETMASTER)
    {
      if (ae->lpMaster)
        return (LRESULT)ae->lpMaster->hWndEdit;
      if (ae->nCloneCount > 0)
        return (LRESULT)ae->hWndEdit;

      return (LRESULT)NULL;
    }
    if (uMsg == AEM_GETCLONE)
    {
      AECLONE *aec;

      if (aec=AE_StackCloneIndex(ae, wParam))
        return (LRESULT)aec->aeClone->hWndEdit;

      return (LRESULT)NULL;
    }

    //Print
    StartPrintDoc:
    if (uMsg >= AEM_HLCREATETHEMEA)
      goto CreateTheme;

    if (uMsg == AEM_STARTPRINTDOC)
    {
      AEPRINT *prn=(AEPRINT *)lParam;

      if (!ae->bUnicodeWindow)
        return (LRESULT)AE_StartPrintDocA(ae, prn);
      else
        return (LRESULT)AE_StartPrintDocW(ae, prn);
    }
    if (uMsg == AEM_PRINTPAGE)
    {
      AEPRINTHANDLE *ph=(AEPRINTHANDLE *)wParam;
      AEPRINT *prn=(AEPRINT *)lParam;

      return (LRESULT)AE_PrintPage(ae, ph, prn);
    }
    if (uMsg == AEM_ENDPRINTDOC)
    {
      AEPRINTHANDLE *ph=(AEPRINTHANDLE *)wParam;
      AEPRINT *prn=(AEPRINT *)lParam;

      AE_EndPrintDoc(ae, ph, prn);
      return 0;
    }

    //HighLight
    CreateTheme:

    if (uMsg == AEM_HLCREATETHEMEA)
    {
      char *pThemeName=(char *)lParam;
      wchar_t wszThemeName[MAX_PATH];

      MultiByteToWideChar(CP_ACP, 0, pThemeName, -1, wszThemeName, MAX_PATH);
      return (LRESULT)AE_HighlightCreateTheme(wszThemeName);
    }
    if (uMsg == AEM_HLCREATETHEMEW)
    {
      wchar_t *wpThemeName=(wchar_t *)lParam;

      return (LRESULT)AE_HighlightCreateTheme(wpThemeName);
    }
    if (uMsg == AEM_HLGETTHEMEA)
    {
      char *pThemeName=(char *)lParam;
      wchar_t wszThemeName[MAX_PATH];

      if (!pThemeName)
        return (LRESULT)ae->popt->lpActiveTheme;
      MultiByteToWideChar(CP_ACP, 0, pThemeName, -1, wszThemeName, MAX_PATH);
      return (LRESULT)AE_HighlightGetTheme(wszThemeName);
    }
    if (uMsg == AEM_HLGETTHEMEW)
    {
      wchar_t *wpThemeName=(wchar_t *)lParam;

      if (!wpThemeName)
        return (LRESULT)ae->popt->lpActiveTheme;
      return (LRESULT)AE_HighlightGetTheme(wpThemeName);
    }
    if (uMsg == AEM_HLGETTHEMENAMEA)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (!lParam)
        return lstrlenW(lpTheme->wszThemeName) + 1;
      return WideCharToMultiByte(CP_ACP, 0, lpTheme->wszThemeName, -1, (char *)lParam, MAX_PATH, NULL, NULL) - 1;
    }
    if (uMsg == AEM_HLGETTHEMENAMEW)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (!lParam)
        return lstrlenW(lpTheme->wszThemeName) + 1;
      return xstrcpynW((wchar_t *)lParam, lpTheme->wszThemeName, MAX_PATH);
    }
    if (uMsg == AEM_HLGETTHEMESTACK)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (lParam == AEHLE_DELIMITER)
        return (LRESULT)&lpTheme->hDelimiterStack;
      if (lParam == AEHLE_WORD)
        return (LRESULT)&lpTheme->hWordStack;
      if (lParam == AEHLE_QUOTE)
        return (LRESULT)&lpTheme->hQuoteStack;
      if (lParam == AEHLE_MARKRANGE)
        return (LRESULT)&lpTheme->hMarkRangeStack;
      if (lParam == AEHLE_MARKTEXT)
        return (LRESULT)&lpTheme->hMarkTextStack;
    }
    if (uMsg == AEM_HLTHEMEEXISTS)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      return AE_HighlightIsThemeExists(lpTheme);
    }
    if (uMsg == AEM_HLSETTHEME)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      ae->popt->lpActiveTheme=lpTheme;
      if (lParam) InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      return 0;
    }
    if (uMsg == AEM_HLDELETETHEME)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (lpTheme)
      {
        AE_HighlightUnsetTheme(lpTheme);
        AE_HighlightDeleteTheme(lpTheme);
      }
      else
      {
        AE_HighlightUnsetTheme(NULL);
        AE_HighlightDeleteThemeAll();
      }
      return 0;
    }
    if (uMsg == AEM_HLADDDELIMITERA)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEDELIMITEMA *lpDelimSrc=(AEDELIMITEMA *)lParam;
      AEDELIMITEMW *lpDelimDst;

      if (lpDelimDst=AE_HighlightInsertDelimiter(lpTheme, lpDelimSrc->nDelimiterLen, lpDelimSrc->nIndex))
      {
        if (lpDelimDst->pDelimiter=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpDelimSrc->nDelimiterLen + 1) * sizeof(wchar_t)))
          MultiByteToWideChar(CP_ACP, 0, lpDelimSrc->pDelimiter, lpDelimSrc->nDelimiterLen + 1, lpDelimDst->pDelimiter, lpDelimSrc->nDelimiterLen + 1);
        lpDelimDst->nDelimiterLen=lpDelimSrc->nDelimiterLen;

        lpDelimDst->dwFlags=lpDelimSrc->dwFlags;
        lpDelimDst->crText=lpDelimSrc->crText;
        lpDelimDst->crBk=lpDelimSrc->crBk;
        lpDelimDst->dwFontStyle=lpDelimSrc->dwFontStyle;
      }
      return (LRESULT)lpDelimDst;
    }
    if (uMsg == AEM_HLADDDELIMITERW)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEDELIMITEMW *lpDelimSrc=(AEDELIMITEMW *)lParam;
      AEDELIMITEMW *lpDelimDst;

      if (lpDelimDst=AE_HighlightInsertDelimiter(lpTheme, lpDelimSrc->nDelimiterLen, lpDelimSrc->nIndex))
      {
        if (lpDelimDst->pDelimiter=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpDelimSrc->nDelimiterLen + 1) * sizeof(wchar_t)))
          xmemcpy(lpDelimDst->pDelimiter, lpDelimSrc->pDelimiter, (lpDelimSrc->nDelimiterLen + 1) * sizeof(wchar_t));
        lpDelimDst->nDelimiterLen=lpDelimSrc->nDelimiterLen;

        lpDelimDst->dwFlags=lpDelimSrc->dwFlags;
        lpDelimDst->crText=lpDelimSrc->crText;
        lpDelimDst->crBk=lpDelimSrc->crBk;
        lpDelimDst->dwFontStyle=lpDelimSrc->dwFontStyle;
      }
      return (LRESULT)lpDelimDst;
    }
    if (uMsg == AEM_HLDELETEDELIMITER)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEDELIMITEMW *lpDelim=(AEDELIMITEMW *)lParam;

      if (lpDelim)
        AE_HighlightDeleteDelimiter(lpTheme, lpDelim);
      else
        AE_HighlightDeleteDelimiterAll(lpTheme);
      return 0;
    }
    if (uMsg == AEM_HLADDWORDA)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEWORDITEMA *lpWordSrc=(AEWORDITEMA *)lParam;
      AEWORDITEMW *lpWordDst;
      int nWordLen;

      if (!(lpWordSrc->dwFlags & AEHLF_WORDCOMPOSITION))
        nWordLen=lpWordSrc->nWordLen;
      else
        nWordLen=0;

      if (lpWordDst=AE_HighlightInsertWord(lpTheme, nWordLen))
      {
        if (lpWordDst->pWord=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpWordSrc->nWordLen + 1) * sizeof(wchar_t)))
          MultiByteToWideChar(CP_ACP, 0, lpWordSrc->pWord, lpWordSrc->nWordLen + 1, lpWordDst->pWord, lpWordSrc->nWordLen + 1);
        lpWordDst->nWordLen=nWordLen;

        lpWordDst->dwFlags=lpWordSrc->dwFlags;
        lpWordDst->crText=lpWordSrc->crText;
        lpWordDst->crBk=lpWordSrc->crBk;
        lpWordDst->dwFontStyle=lpWordSrc->dwFontStyle;
      }
      return (LRESULT)lpWordDst;
    }
    if (uMsg == AEM_HLADDWORDW)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEWORDITEMW *lpWordSrc=(AEWORDITEMW *)lParam;
      AEWORDITEMW *lpWordDst;
      int nWordLen;

      if (!(lpWordSrc->dwFlags & AEHLF_WORDCOMPOSITION))
        nWordLen=lpWordSrc->nWordLen;
      else
        nWordLen=0;

      if (lpWordDst=AE_HighlightInsertWord(lpTheme, nWordLen))
      {
        if (lpWordDst->pWord=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpWordSrc->nWordLen + 1) * sizeof(wchar_t)))
          xmemcpy(lpWordDst->pWord, lpWordSrc->pWord, (lpWordSrc->nWordLen + 1) * sizeof(wchar_t));
        lpWordDst->nWordLen=nWordLen;

        lpWordDst->dwFlags=lpWordSrc->dwFlags;
        lpWordDst->crText=lpWordSrc->crText;
        lpWordDst->crBk=lpWordSrc->crBk;
        lpWordDst->dwFontStyle=lpWordSrc->dwFontStyle;
      }
      return (LRESULT)lpWordDst;
    }
    if (uMsg == AEM_HLDELETEWORD)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEWORDITEMW *lpWord=(AEWORDITEMW *)lParam;

      if (lpWord)
        AE_HighlightDeleteWord(lpTheme, lpWord);
      else
        AE_HighlightDeleteWordAll(lpTheme);
      return 0;
    }
    if (uMsg == AEM_HLADDQUOTEA)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEQUOTEITEMA *lpQuoteSrc=(AEQUOTEITEMA *)lParam;
      AEQUOTEITEMW *lpQuoteDst;

      if (lpQuoteDst=AE_HighlightInsertQuote(lpTheme, lpQuoteSrc->nIndex))
      {
        if (lpQuoteDst->pQuoteStart=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteStartLen + 1) * sizeof(wchar_t)))
          MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteStart, lpQuoteSrc->nQuoteStartLen + 1, lpQuoteDst->pQuoteStart, lpQuoteSrc->nQuoteStartLen + 1);
        lpQuoteDst->nQuoteStartLen=lpQuoteSrc->nQuoteStartLen;

        if (lpQuoteSrc->pQuoteEnd)
        {
          if (lpQuoteDst->pQuoteEnd=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteEndLen + 1) * sizeof(wchar_t)))
            MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteEnd, lpQuoteSrc->nQuoteEndLen + 1, lpQuoteDst->pQuoteEnd, lpQuoteSrc->nQuoteEndLen + 1);
          lpQuoteDst->nQuoteEndLen=lpQuoteSrc->nQuoteEndLen;
        }
        else lpQuoteDst->pQuoteEnd=NULL;

        MultiByteToWideChar(CP_ACP, 0, &lpQuoteSrc->chEscape, 1, &lpQuoteDst->chEscape, 1);

        if (lpQuoteSrc->pQuoteInclude)
        {
          if (lpQuoteDst->pQuoteInclude=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteIncludeLen + 1) * sizeof(wchar_t)))
            MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteInclude, lpQuoteSrc->nQuoteIncludeLen + 1, lpQuoteDst->pQuoteInclude, lpQuoteSrc->nQuoteIncludeLen + 1);
          lpQuoteDst->nQuoteIncludeLen=lpQuoteSrc->nQuoteIncludeLen;
        }
        else lpQuoteDst->pQuoteInclude=NULL;

        if (lpQuoteSrc->pQuoteExclude)
        {
          if (lpQuoteDst->pQuoteExclude=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteExcludeLen + 1) * sizeof(wchar_t)))
            MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteExclude, lpQuoteSrc->nQuoteExcludeLen + 1, lpQuoteDst->pQuoteExclude, lpQuoteSrc->nQuoteExcludeLen + 1);
          lpQuoteDst->nQuoteExcludeLen=lpQuoteSrc->nQuoteExcludeLen;
        }
        else lpQuoteDst->pQuoteExclude=NULL;

        lpQuoteDst->dwFlags=lpQuoteSrc->dwFlags;
        lpQuoteDst->crText=lpQuoteSrc->crText;
        lpQuoteDst->crBk=lpQuoteSrc->crBk;
        lpQuoteDst->dwFontStyle=lpQuoteSrc->dwFontStyle;
      }
      return (LRESULT)lpQuoteDst;
    }
    if (uMsg == AEM_HLADDQUOTEW)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEQUOTEITEMW *lpQuoteSrc=(AEQUOTEITEMW *)lParam;
      AEQUOTEITEMW *lpQuoteDst;

      if (lpQuoteDst=AE_HighlightInsertQuote(lpTheme, lpQuoteSrc->nIndex))
      {
        if (lpQuoteDst->pQuoteStart=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteStartLen + 1) * sizeof(wchar_t)))
          xmemcpy(lpQuoteDst->pQuoteStart, lpQuoteSrc->pQuoteStart, (lpQuoteSrc->nQuoteStartLen + 1) * sizeof(wchar_t));
        lpQuoteDst->nQuoteStartLen=lpQuoteSrc->nQuoteStartLen;

        if (lpQuoteSrc->pQuoteEnd)
        {
          if (lpQuoteDst->pQuoteEnd=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteEndLen + 1) * sizeof(wchar_t)))
            xmemcpy(lpQuoteDst->pQuoteEnd, lpQuoteSrc->pQuoteEnd, (lpQuoteSrc->nQuoteEndLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteEndLen=lpQuoteSrc->nQuoteEndLen;
        }
        else lpQuoteDst->pQuoteEnd=NULL;

        lpQuoteDst->chEscape=lpQuoteSrc->chEscape;

        if (lpQuoteSrc->pQuoteInclude)
        {
          if (lpQuoteDst->pQuoteInclude=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteIncludeLen + 1) * sizeof(wchar_t)))
            xmemcpy(lpQuoteDst->pQuoteInclude, lpQuoteSrc->pQuoteInclude, (lpQuoteSrc->nQuoteIncludeLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteIncludeLen=lpQuoteSrc->nQuoteIncludeLen;
        }
        else lpQuoteDst->pQuoteInclude=NULL;

        if (lpQuoteSrc->pQuoteExclude)
        {
          if (lpQuoteDst->pQuoteExclude=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteExcludeLen + 1) * sizeof(wchar_t)))
            xmemcpy(lpQuoteDst->pQuoteExclude, lpQuoteSrc->pQuoteExclude, (lpQuoteSrc->nQuoteExcludeLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteExcludeLen=lpQuoteSrc->nQuoteExcludeLen;
        }
        else lpQuoteDst->pQuoteExclude=NULL;

        lpQuoteDst->dwFlags=lpQuoteSrc->dwFlags;
        lpQuoteDst->crText=lpQuoteSrc->crText;
        lpQuoteDst->crBk=lpQuoteSrc->crBk;
        lpQuoteDst->dwFontStyle=lpQuoteSrc->dwFontStyle;
      }
      return (LRESULT)lpQuoteDst;
    }
    if (uMsg == AEM_HLDELETEQUOTE)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEQUOTEITEMW *lpQuote=(AEQUOTEITEMW *)lParam;

      if (lpQuote)
        AE_HighlightDeleteQuote(lpTheme, lpQuote);
      else
        AE_HighlightDeleteQuoteAll(lpTheme);
      return 0;
    }
    if (uMsg == AEM_HLADDMARKTEXTA)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKTEXTITEMA *lpMarkTextSrc=(AEMARKTEXTITEMA *)lParam;
      AEMARKTEXTITEMW *lpMarkTextDst;

      if (lpMarkTextDst=AE_HighlightInsertMarkText(lpTheme, lpMarkTextSrc->nIndex))
      {
        if (lpMarkTextDst->pMarkText=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpMarkTextSrc->nMarkTextLen + 1) * sizeof(wchar_t)))
          MultiByteToWideChar(CP_ACP, 0, lpMarkTextSrc->pMarkText, lpMarkTextSrc->nMarkTextLen + 1, lpMarkTextDst->pMarkText, lpMarkTextSrc->nMarkTextLen + 1);
        lpMarkTextDst->nMarkTextLen=lpMarkTextSrc->nMarkTextLen;

        lpMarkTextDst->dwFlags=lpMarkTextSrc->dwFlags;
        lpMarkTextDst->crText=lpMarkTextSrc->crText;
        lpMarkTextDst->crBk=lpMarkTextSrc->crBk;
        lpMarkTextDst->dwFontStyle=lpMarkTextSrc->dwFontStyle;
      }
      return (LRESULT)lpMarkTextDst;
    }
    if (uMsg == AEM_HLADDMARKTEXTW)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKTEXTITEMW *lpMarkTextSrc=(AEMARKTEXTITEMW *)lParam;
      AEMARKTEXTITEMW *lpMarkTextDst;

      if (lpMarkTextDst=AE_HighlightInsertMarkText(lpTheme, lpMarkTextSrc->nIndex))
      {
        if (lpMarkTextDst->pMarkText=(wchar_t *)AE_HeapAlloc(NULL, 0, (lpMarkTextSrc->nMarkTextLen + 1) * sizeof(wchar_t)))
          xmemcpy(lpMarkTextDst->pMarkText, lpMarkTextSrc->pMarkText, (lpMarkTextSrc->nMarkTextLen + 1) * sizeof(wchar_t));
        lpMarkTextDst->nMarkTextLen=lpMarkTextSrc->nMarkTextLen;

        lpMarkTextDst->dwFlags=lpMarkTextSrc->dwFlags;
        lpMarkTextDst->crText=lpMarkTextSrc->crText;
        lpMarkTextDst->crBk=lpMarkTextSrc->crBk;
        lpMarkTextDst->dwFontStyle=lpMarkTextSrc->dwFontStyle;
      }
      return (LRESULT)lpMarkTextDst;
    }
    if (uMsg == AEM_HLDELETEMARKTEXT)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKTEXTITEMW *lpMarkText=(AEMARKTEXTITEMW *)lParam;

      if (lpMarkText)
        AE_HighlightDeleteMarkText(lpTheme, lpMarkText);
      else
        AE_HighlightDeleteMarkTextAll(lpTheme);
      return 0;
    }
    if (uMsg == AEM_HLADDMARKRANGE)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKRANGEITEM *lpMarkRangeSrc=(AEMARKRANGEITEM *)lParam;
      AEMARKRANGEITEM *lpMarkRangeDst;

      if (lpMarkRangeDst=AE_HighlightInsertMarkRange(lpTheme, lpMarkRangeSrc->nIndex))
      {
        lpMarkRangeDst->crMarkRange=lpMarkRangeSrc->crMarkRange;
        lpMarkRangeDst->dwFlags=lpMarkRangeSrc->dwFlags;
        lpMarkRangeDst->crText=lpMarkRangeSrc->crText;
        lpMarkRangeDst->crBk=lpMarkRangeSrc->crBk;
        lpMarkRangeDst->dwFontStyle=lpMarkRangeSrc->dwFontStyle;
      }
      return (LRESULT)lpMarkRangeDst;
    }
    if (uMsg == AEM_HLDELETEMARKRANGE)
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKRANGEITEM *lpMarkRange=(AEMARKRANGEITEM *)lParam;

      if (lpMarkRange)
        AE_HighlightDeleteMarkRange(lpTheme, lpMarkRange);
      else
        AE_HighlightDeleteMarkRangeAll(lpTheme);
      return 0;
    }
  }


  //// EM_* rich edit messages

  //Text retrieval and modification
  if (uMsg == EM_STREAMIN)
  {
    EDITSTREAM *es=(EDITSTREAM *)lParam;
    AESTREAMIN aesi;
    DWORD dwFlags=0;
    DWORD dwResult=0;

    if ((wParam & SF_TEXT) && (wParam & SF_UNICODE))
    {
      aesi.dwCookie=es->dwCookie;
      aesi.lpCallback=(AEStreamCallback)es->pfnCallback;
      aesi.nNewLine=AELB_ASINPUT;
      if (wParam & SFF_SELECTION)
        dwFlags|=AESF_SELECTION;

      dwResult=AE_StreamIn(ae, dwFlags, &aesi);

      es->dwError=aesi.dwError;
    }
    return dwResult;
  }
  if (uMsg == EM_STREAMOUT)
  {
    EDITSTREAM *es=(EDITSTREAM *)lParam;
    AESTREAMOUT aeso;
    DWORD dwFlags=0;
    DWORD dwResult=0;

    if ((wParam & SF_TEXT) && (wParam & SF_UNICODE))
    {
      aeso.dwCookie=es->dwCookie;
      aeso.lpCallback=(AEStreamCallback)es->pfnCallback;
      aeso.nNewLine=AELB_ASOUTPUT;
      aeso.bColumnSel=FALSE;
      if (wParam & SFF_SELECTION)
        dwFlags|=AESF_SELECTION;

      dwResult=AE_StreamOut(ae, dwFlags, &aeso);

      es->dwError=aeso.dwError;
    }
    return dwResult;
  }
  if (uMsg == EM_REPLACESEL ||
      uMsg == EM_REPLACESELA ||
      uMsg == EM_REPLACESELW)
  {
    if (uMsg == EM_REPLACESELA || (!ae->bUnicodeWindow && uMsg == EM_REPLACESEL))
      AE_ReplaceSelAnsi(ae, CP_ACP, (char *)lParam, (DWORD)-1, AELB_ASINPUT, FALSE, NULL, NULL);
    else
      AE_ReplaceSel(ae, (wchar_t *)lParam, (DWORD)-1, AELB_ASINPUT, FALSE, NULL, NULL);
    return 0;
  }
  if (uMsg == EM_GETTEXTRANGE ||
      uMsg == EM_GETTEXTRANGEA ||
      uMsg == EM_GETTEXTRANGEW)
  {
    if (uMsg == EM_GETTEXTRANGEA || (!ae->bUnicodeWindow && uMsg == EM_GETTEXTRANGE))
    {
      TEXTRANGEA *trRE=(TEXTRANGEA *)lParam;
      AECHARRANGE cr;

      if ((DWORD)trRE->chrg.cpMin < (DWORD)trRE->chrg.cpMax)
      {
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMin, &cr.ciMin);
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMax, &cr.ciMax);
        return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, trRE->lpstrText, (DWORD)-1, AELB_R, FALSE, FALSE);
      }
      else if ((DWORD)trRE->chrg.cpMin == (DWORD)trRE->chrg.cpMax)
      {
        if (trRE->lpstrText)
          trRE->lpstrText[0]='\0';
        else
          return 1;
      }
    }
    else
    {
      TEXTRANGEW *trRE=(TEXTRANGEW *)lParam;
      AECHARRANGE cr;

      if ((DWORD)trRE->chrg.cpMin < (DWORD)trRE->chrg.cpMax)
      {
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMin, &cr.ciMin);
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMax, &cr.ciMax);
        return AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, trRE->lpstrText, (DWORD)-1, AELB_R, FALSE, FALSE);
      }
      else if ((DWORD)trRE->chrg.cpMin == (DWORD)trRE->chrg.cpMax)
      {
        if (trRE->lpstrText)
          trRE->lpstrText[0]=L'\0';
        else
          return 1;
      }
    }
    return 0;
  }
  if (uMsg == EM_GETSELTEXT ||
      uMsg == EM_GETSELTEXTA ||
      uMsg == EM_GETSELTEXTW)
  {
    if (uMsg == EM_GETSELTEXTA || (!ae->bUnicodeWindow && uMsg == EM_GETSELTEXT))
      return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (char *)lParam, (DWORD)-1, AELB_R, FALSE, FALSE);
    else
      return AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)lParam, (DWORD)-1, AELB_R, FALSE, FALSE);
  }
  if (uMsg == EM_GETLINE ||
      uMsg == EM_GETLINEA ||
      uMsg == EM_GETLINEW)
  {
    if (uMsg == EM_GETLINEA || (!ae->bUnicodeWindow && uMsg == EM_GETLINE))
    {
      AELINEDATA *lpLine;
      char *szBuffer=(char *)lParam;
      int nMaxBuffer=*(WORD *)lParam;
      int nResult=0;

      szBuffer[0]='\0';

      if (lpLine=AE_GetLineData(ae, wParam))
      {
        nResult=WideCharToMultiByte(CP_ACP, 0, lpLine->wpLine, lpLine->nLineLen, szBuffer, nMaxBuffer, NULL, NULL);
        if (nResult < nMaxBuffer)
          szBuffer[nResult++]='\r';
      }
      return nResult;
    }
    else
    {
      AELINEDATA *lpLine;
      wchar_t *wszBuffer=(wchar_t *)lParam;
      int nMaxBuffer=*(WORD *)lParam;
      int nResult=0;

      wszBuffer[0]='\0';

      if (lpLine=AE_GetLineData(ae, wParam))
      {
        nResult=min(nMaxBuffer, lpLine->nLineLen);
        xmemcpy(wszBuffer, lpLine->wpLine, nResult * sizeof(wchar_t));
        if (nResult < nMaxBuffer)
          wszBuffer[nResult++]=L'\r';
      }
      return nResult;
    }
  }
  if (uMsg == EM_GETTEXTLENGTHEX)
  {
    GETTEXTLENGTHEX *gtl=(GETTEXTLENGTHEX *)wParam;
    DWORD dwResult=0;

    //Answer is always GTL_PRECISE
    if ((gtl->flags & GTL_PRECISE) && (gtl->flags & GTL_CLOSE))
      return E_INVALIDARG;
    if ((gtl->flags & GTL_NUMCHARS) && (gtl->flags & GTL_NUMBYTES))
      return E_INVALIDARG;

    if (!(gtl->flags & GTL_NUMBYTES))
    {
      dwResult=ae->ptxt->nLastCharOffset;
      if (gtl->flags & GTL_USECRLF)
        dwResult+=ae->ptxt->nLineCount;
    }
    else
    {
      if (gtl->codepage == 1200 || gtl->codepage == 1201)
      {
        dwResult=ae->ptxt->nLastCharOffset;
        if (gtl->flags & GTL_USECRLF)
          dwResult+=ae->ptxt->nLineCount;
        dwResult*=sizeof(wchar_t);
      }
      else
      {
        AELINEDATA *lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;

        while (lpLine)
        {
          if (lpLine->nLineLen)
          {
            dwResult+=WideCharToMultiByte(gtl->codepage, 0, lpLine->wpLine, lpLine->nLineLen, NULL, 0, NULL, NULL);
          }
          lpLine=lpLine->next;
        }
        dwResult+=ae->ptxt->nLineCount;
        if (gtl->flags & GTL_USECRLF)
          dwResult+=ae->ptxt->nLineCount;
      }
    }
    return dwResult;
  }
  if (uMsg == EM_GETTEXTEX)
  {
    GETTEXTEX *gt=(GETTEXTEX *)wParam;
    AECHARRANGE cr;
    int nNewLine;
    DWORD dwResult=0;

    if (gt->flags & GT_SELECTION)
    {
      cr.ciMin=ae->ciSelStartIndex;
      cr.ciMax=ae->ciSelEndIndex;
    }
    else
    {
      AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin, FALSE);
      AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax, FALSE);
    }

    if (gt->flags & GT_USECRLF)
      nNewLine=AELB_RN;
    else
      nNewLine=AELB_R;

    if (gt->codepage == 1200)
    {
      dwResult=AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, (wchar_t *)lParam, gt->cb / sizeof(wchar_t), nNewLine, FALSE, FALSE);
    }
    else if (gt->codepage == 1201)
    {
      dwResult=AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, (wchar_t *)lParam, gt->cb / sizeof(wchar_t), nNewLine, FALSE, FALSE);
      AE_ChangeTwoBytesOrder((unsigned char *)lParam, dwResult * sizeof(wchar_t));
    }
    else dwResult=AE_GetTextRangeAnsi(ae, gt->codepage, gt->lpDefaultChar, gt->lpUsedDefChar, &cr.ciMin, &cr.ciMax, (char *)lParam, gt->cb, nNewLine, FALSE, FALSE);

    return dwResult;
  }
  if (uMsg == EM_SETTEXTEX)
  {
    SETTEXTEX *st=(SETTEXTEX *)wParam;
    BOOL bLockCollectUndo=ae->ptxt->bLockCollectUndo;

    if (!(st->flags & ST_KEEPUNDO))
    {
      AE_EmptyUndoBuffer(ae);
      ae->ptxt->bLockCollectUndo=TRUE;
    }
    if (!(st->flags & ST_SELECTION))
      AE_EditSelectAll(ae, AESELT_LOCKNOTIFY, 0);

    if (st->codepage == 1200)
    {
      AE_ReplaceSel(ae, (wchar_t *)lParam, (DWORD)-1, AELB_ASINPUT, FALSE, NULL, NULL);
    }
    else if (st->codepage == 1201)
    {
      wchar_t *wszText;
      DWORD dwUnicodeBytes;

      dwUnicodeBytes=lstrlenW((wchar_t *)lParam) * sizeof(wchar_t);

      if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, dwUnicodeBytes + 2))
      {
        xmemcpy(wszText, (wchar_t *)lParam, dwUnicodeBytes + 2);
        AE_ChangeTwoBytesOrder((unsigned char *)wszText, dwUnicodeBytes);
        AE_ReplaceSel(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), AELB_ASINPUT, FALSE, NULL, NULL);
        AE_HeapFree(NULL, 0, (LPVOID)wszText);
      }
    }
    else AE_ReplaceSelAnsi(ae, st->codepage, (char *)lParam, (DWORD)-1, AELB_ASINPUT, FALSE, NULL, NULL);

    if (!(st->flags & ST_KEEPUNDO))
      ae->ptxt->bLockCollectUndo=bLockCollectUndo;
    if (!(st->flags & ST_SELECTION))
      AE_SetModify(ae, FALSE);
    return 1;
  }
  if (uMsg == EM_FINDTEXT ||
      uMsg == EM_FINDTEXTW ||
      uMsg == EM_FINDTEXTEX ||
      uMsg == EM_FINDTEXTEXW)
  {
    if (uMsg == EM_FINDTEXT ||
        uMsg == EM_FINDTEXTEX)
    {
      //Ansi
      if (!ae->bUnicodeWindow)
      {
        FINDTEXTEXA *ftRE=(FINDTEXTEXA *)lParam;
        AEFINDTEXTA ft={0};
        CHARRANGE crFoundRE;

        if (wParam & FR_DOWN)
          ft.dwFlags|=AEFR_DOWN;
        if (wParam & FR_MATCHCASE)
          ft.dwFlags|=AEFR_MATCHCASE;
        if (wParam & FR_WHOLEWORD)
          ft.dwFlags|=AEFR_WHOLEWORD;
        ft.pText=(char *)ftRE->lpstrText;
        ft.dwTextLen=(DWORD)-1;
        ft.nNewLine=AELB_R;
        AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMin, &ft.crSearch.ciMin);
        AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMax, &ft.crSearch.ciMax);

        if (AE_FindTextAnsi(ae, CP_ACP, &ft))
        {
          crFoundRE.cpMin=AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMin);
          crFoundRE.cpMax=AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMax);

          if (uMsg == EM_FINDTEXTEX)
          {
            ftRE->chrgText=crFoundRE;
          }
          return crFoundRE.cpMin;
        }
        return -1;
      }
    }

    //Unicode
    {
      FINDTEXTEXW *ftRE=(FINDTEXTEXW *)lParam;
      AEFINDTEXTW ft={0};
      CHARRANGE crFoundRE;

      if (wParam & FR_DOWN)
        ft.dwFlags|=AEFR_DOWN;
      if (wParam & FR_MATCHCASE)
        ft.dwFlags|=AEFR_MATCHCASE;
      if (wParam & FR_WHOLEWORD)
        ft.dwFlags|=AEFR_WHOLEWORD;
      ft.pText=(wchar_t *)ftRE->lpstrText;
      ft.dwTextLen=(DWORD)-1;
      ft.nNewLine=AELB_R;
      AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMin, &ft.crSearch.ciMin);
      AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMax, &ft.crSearch.ciMax);

      if (AE_FindText(ae, &ft))
      {
        crFoundRE.cpMin=AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMin);
        crFoundRE.cpMax=AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMax);

        if (uMsg == EM_FINDTEXTEX || uMsg == EM_FINDTEXTEXW)
        {
          ftRE->chrgText=crFoundRE;
        }
        return crFoundRE.cpMin;
      }
      return -1;
    }
  }
  if (uMsg == EM_CANPASTE)
  {
    return AE_EditCanPaste(ae);
  }
  if (uMsg == EM_GETLIMITTEXT)
  {
    return ae->ptxt->dwTextLimit;
  }
  if (uMsg == EM_SETLIMITTEXT ||
      uMsg == EM_LIMITTEXT)
  {
    if (!wParam)
      ae->ptxt->dwTextLimit=65536;
    else
      ae->ptxt->dwTextLimit=wParam;
    return 0;
  }
  if (uMsg == EM_EXLIMITTEXT)
  {
    if (!lParam)
      ae->ptxt->dwTextLimit=65536;
    else
      ae->ptxt->dwTextLimit=lParam;
    return 0;
  }

  //Undo and Redo
  if (uMsg == EM_CANUNDO)
  {
    if (ae->popt->dwOptions & AECO_READONLY)
      return FALSE;
    return AE_EditCanUndo(ae);
  }
  if (uMsg == EM_CANREDO)
  {
    if (ae->popt->dwOptions & AECO_READONLY)
      return FALSE;
    return AE_EditCanRedo(ae);
  }
  if (uMsg == EM_UNDO)
  {
    AE_EditUndo(ae);
    return 0;
  }
  if (uMsg == EM_REDO)
  {
    AE_EditRedo(ae);
    return 0;
  }
  if (uMsg == EM_GETMODIFY)
  {
    return ae->ptxt->bModified;
  }
  if (uMsg == EM_SETMODIFY)
  {
    AE_SetModify(ae, wParam);
    return 0;
  }
  if (uMsg == EM_SETUNDOLIMIT)
  {
    ae->ptxt->dwUndoLimit=wParam;
    AE_StackUndoGroupStop(ae);
    return 0;
  }
  if (uMsg == EM_EMPTYUNDOBUFFER)
  {
    AE_EmptyUndoBuffer(ae);
    return 0;
  }
  if (uMsg == EM_STOPGROUPTYPING)
  {
    AE_StackUndoGroupStop(ae);
    return 0;
  }

  //Text coordinates
  if (uMsg == EM_GETSEL)
  {
    CHARRANGE crRE;

    AE_RichEditGetSel(ae, &crRE.cpMin, &crRE.cpMax);
    if (wParam) *(LONG *)wParam=crRE.cpMin;
    if (lParam) *(LONG *)lParam=crRE.cpMax;

    if ((DWORD)crRE.cpMin > 0xFFFF0000)
      return -1;
    if ((DWORD)crRE.cpMax > 0xFFFF0000)
      return -1;
    return MAKELONG(crRE.cpMin, crRE.cpMax);
  }
  if (uMsg == EM_EXGETSEL)
  {
    CHARRANGE *crRE=(CHARRANGE *)lParam;

    AE_RichEditGetSel(ae, &crRE->cpMin, &crRE->cpMax);
    return 0;
  }
  if (uMsg == EM_SETSEL)
  {
    AE_RichEditSetSel(ae, wParam, lParam);
    return 0;
  }
  if (uMsg == EM_EXSETSEL)
  {
    CHARRANGE *crRE=(CHARRANGE *)lParam;

    AE_RichEditSetSel(ae, crRE->cpMin, crRE->cpMax);
    return ae->nSelEndCharOffset;
  }
  if (uMsg == EM_SELECTIONTYPE)
  {
    DWORD dwSelType=0;

    if (ae->nSelStartCharOffset == ae->nSelEndCharOffset)
    {
      dwSelType=SEL_EMPTY;
    }
    else
    {
      dwSelType=SEL_TEXT;
      if (ae->nSelEndCharOffset - ae->nSelStartCharOffset > 1)
        dwSelType|=SEL_MULTICHAR;
    }
    return dwSelType;
  }
  if (uMsg == EM_GETLINECOUNT)
  {
    return ae->ptxt->nLineCount + 1;
  }
  if (uMsg == EM_GETFIRSTVISIBLELINE)
  {
    return AE_GetFirstVisibleLine(ae);
  }
  if (uMsg == EM_LINEFROMCHAR)
  {
    AECHARINDEX ci={0};

    if (wParam == (DWORD)-1)
      ci.nLine=ae->ciCaretIndex.nLine;
    else
      AE_RichOffsetToAkelIndex(ae, wParam, &ci);
    return ci.nLine;
  }
  if (uMsg == EM_EXLINEFROMCHAR)
  {
    AECHARINDEX ci={0};

    if (lParam == -1)
      ci.nLine=ae->ciCaretIndex.nLine;
    else
      AE_RichOffsetToAkelIndex(ae, lParam, &ci);
    return ci.nLine;
  }
  if (uMsg == EM_LINEINDEX)
  {
    AECHARINDEX ci;

    if (wParam == (DWORD)-1)
    {
      ci.nLine=ae->ciCaretIndex.nLine;
      ci.lpLine=ae->ciCaretIndex.lpLine;
      ci.nCharInLine=0;
    }
    else
    {
      if (wParam <= (DWORD)ae->ptxt->nLineCount)
      {
        ci.nLine=wParam;
        ci.lpLine=AE_GetLineData(ae, ci.nLine);
        ci.nCharInLine=0;
      }
      else return -1;
    }
    return AE_AkelIndexToRichOffset(ae, &ci);
  }
  if (uMsg == EM_LINELENGTH)
  {
    AECHARINDEX ci={0};

    if (wParam <= (DWORD)ae->ptxt->nLastCharOffset)
    {
      AE_RichOffsetToAkelIndex(ae, wParam, &ci);
      if (ci.lpLine)
        return ci.lpLine->nLineLen;
    }
    return 0;
  }
  if (uMsg == EM_POSFROMCHAR)
  {
    AECHARINDEX ci={0};

    if (lParam || !IsBadWritePtr((void *)wParam, sizeof(POINT)))
    {
      //RichEdit 3.0 syntax
      POINT *pt=(POINT *)wParam;

      AE_RichOffsetToAkelIndex(ae, lParam, &ci);
      AE_GetPosFromChar(ae, &ci, NULL, pt);
      return 0;
    }
    else
    {
      //RichEdit 2.0 syntax
      POINT pt;

      AE_RichOffsetToAkelIndex(ae, wParam, &ci);
      AE_GetPosFromChar(ae, &ci, NULL, &pt);
      return MAKELONG(pt.x, pt.y);
    }
    return 0;
  }
  if (uMsg == EM_CHARFROMPOS)
  {
    POINT *pt=(POINT *)lParam;
    AECHARINDEX ci={0};

    AE_GetCharFromPos(ae, pt, &ci, NULL, FALSE);
    return AE_AkelIndexToRichOffset(ae, &ci);
  }
  if (uMsg == EM_GETRECT)
  {
    RECT *rcDraw=(RECT *)lParam;

    *rcDraw=ae->rcDraw;
    return 0;
  }
  if (uMsg == EM_SETRECT ||
      uMsg == EM_SETRECTNP)
  {
    RECT *lprcDraw=(RECT *)lParam;
    RECT rcDraw;
    int nBorderX;
    int nBorderY;

    if (lprcDraw)
    {
      if (wParam)
      {
        rcDraw.left=ae->rcDraw.left + lprcDraw->left;
        rcDraw.top=ae->rcDraw.top + lprcDraw->top;
        rcDraw.right=ae->rcDraw.right + lprcDraw->right;
        rcDraw.bottom=ae->rcDraw.bottom + lprcDraw->bottom;
        lprcDraw=&rcDraw;
      }
      else
      {
        if (GetWindowLongA(ae->hWndEdit, GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
        {
          nBorderX=GetSystemMetrics(SM_CXBORDER);
          nBorderY=GetSystemMetrics(SM_CYBORDER);

          rcDraw.left=lprcDraw->left - nBorderX;
          rcDraw.top=lprcDraw->top - nBorderY;
          rcDraw.right=lprcDraw->right + nBorderX;
          rcDraw.bottom=lprcDraw->bottom + nBorderY;
          lprcDraw=&rcDraw;
        }
      }
    }
    AE_SetDrawRect(ae, lprcDraw, (uMsg == EM_SETRECT)?TRUE:FALSE);
    if (ae->ptxt->dwWordWrap)
    {
      AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
      AE_StackUpdateClones(ae);
    }
    AE_UpdateScrollBars(ae, SB_BOTH);
    AE_NotifySetRect(ae);
    return 0;
  }
  if (uMsg == EM_GETMARGINS)
  {
    return MAKELONG(ae->rcDraw.left - ae->rcEdit.left, ae->rcEdit.right - ae->rcDraw.right);
  }
  if (uMsg == EM_SETMARGINS)
  {
    RECT rcDraw=ae->rcDraw;

    if (wParam & EC_LEFTMARGIN)
      rcDraw.left=ae->rcEdit.left + LOWORD(lParam);
    if (wParam & EC_RIGHTMARGIN)
      rcDraw.right=ae->rcEdit.right - HIWORD(lParam);

    AE_SetDrawRect(ae, &rcDraw, TRUE);
    if (ae->ptxt->dwWordWrap)
    {
      AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
      AE_StackUpdateClones(ae);
    }
    AE_UpdateScrollBars(ae, SB_BOTH);
    AE_NotifySetRect(ae);
    return 0;
  }
  if (uMsg == EM_GETSCROLLPOS)
  {
    POINT *pt=(POINT *)lParam;

    pt->x=ae->nHScrollPos;
    pt->y=ae->nVScrollPos;
    return 1;
  }
  if (uMsg == EM_SETSCROLLPOS)
  {
    POINT *pt=(POINT *)lParam;

    if (pt->x != ae->nHScrollPos)
      AE_ScrollEditWindow(ae, SB_HORZ, pt->x);
    if (pt->y != ae->nVScrollPos)
      AE_ScrollEditWindow(ae, SB_VERT, pt->y);
    return 1;
  }
  if (uMsg == EM_SCROLL)
  {
    int nScrolled=0;
    BOOL bResult=FALSE;

    if (wParam == SB_LINEUP ||
        wParam == SB_LINEDOWN ||
        wParam == SB_PAGEUP ||
        wParam == SB_PAGEDOWN)
    {
      nScrolled=AE_VScroll(ae, wParam, 0);
      bResult=TRUE;
    }
    return MAKELONG(mod(nScrolled) / ae->ptxt->nCharHeight, bResult);
  }
  if (uMsg == EM_LINESCROLL)
  {
    AE_VScrollLine(ae, lParam, AESB_ALIGNTOP);
    return 0;
  }
  if (uMsg == EM_SCROLLCARET)
  {
    AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
    return 0;
  }
  if (uMsg == EM_GETTHUMB)
  {
    return ae->nVScrollPos;
  }
  if (uMsg == EM_FINDWORDBREAK)
  {
    AECHARINDEX ciCharIn;
    AECHARINDEX ciCharOut;
    DWORD dwFlags=0;

    if (wParam == WB_CLASSIFY)
      return 0;

    AE_RichOffsetToAkelIndex(ae, lParam, &ciCharIn);

    if (wParam == WB_ISDELIMITER)
      return AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ciCharIn.lpLine->wpLine[ciCharIn.nCharInLine], TRUE);

    if (wParam == WB_LEFT ||
        wParam == WB_LEFTBREAK ||
        wParam == WB_PREVBREAK ||
        wParam == WB_MOVEWORDLEFT ||
        wParam == WB_MOVEWORDPREV)
    {
      if (wParam == WB_LEFT)
        dwFlags=AEWB_LEFTWORDSTART;
      else if (wParam == WB_LEFTBREAK || wParam == WB_PREVBREAK)
        dwFlags=AEWB_LEFTWORDEND;
      else if (wParam == WB_MOVEWORDLEFT || wParam == WB_MOVEWORDPREV)
        dwFlags=ae->popt->dwWordBreak;

      if (AE_GetPrevBreak(ae, &ciCharIn, &ciCharOut, FALSE, dwFlags))
        return AE_AkelIndexToRichOffset(ae, &ciCharOut);
      else
        return lParam;
    }

    if (wParam == WB_RIGHT ||
        wParam == WB_RIGHTBREAK ||
        wParam == WB_NEXTBREAK ||
        wParam == WB_MOVEWORDRIGHT ||
        wParam == WB_MOVEWORDNEXT)
    {
      if (wParam == WB_RIGHT)
        dwFlags=AEWB_RIGHTWORDSTART;
      else if (wParam == WB_RIGHTBREAK || wParam == WB_NEXTBREAK)
        dwFlags=AEWB_RIGHTWORDEND;
      else if (wParam == WB_MOVEWORDRIGHT || wParam == WB_MOVEWORDNEXT)
        dwFlags=ae->popt->dwWordBreak;

      if (AE_GetNextBreak(ae, &ciCharIn, &ciCharOut, FALSE, dwFlags))
        return AE_AkelIndexToRichOffset(ae, &ciCharOut);
      else
        return lParam;
    }
    return 0;
  }

  //Options
  if (uMsg == EM_GETEVENTMASK)
  {
    return ae->popt->dwRichEventMask;
  }
  if (uMsg == EM_SETEVENTMASK)
  {
    DWORD dwPrevMask=ae->popt->dwRichEventMask;

    ae->popt->dwRichEventMask=lParam;
    return dwPrevMask;
  }
  if (uMsg == EM_GETOPTIONS)
  {
    DWORD dwOptions=0;

    if (ae->popt->dwOptions & AECO_READONLY)
      dwOptions|=ECO_READONLY;
    if (ae->popt->dwOptions & AECO_NOHIDESEL)
      dwOptions|=ECO_NOHIDESEL;
    if (ae->popt->dwOptions & AECO_WANTRETURN)
      dwOptions|=ECO_WANTRETURN;
    return dwOptions;
  }
  if (uMsg == EM_SETOPTIONS)
  {
    DWORD dwOptions=0;

    if (lParam & ECO_READONLY)
      dwOptions|=AECO_READONLY;
    if (lParam & ECO_NOHIDESEL)
      dwOptions|=AECO_NOHIDESEL;
    if (lParam & ECO_WANTRETURN)
      dwOptions|=AECO_WANTRETURN;

    if (wParam == AECOOP_SET)
      ae->popt->dwOptions=dwOptions;
    else if (wParam == AECOOP_OR)
      ae->popt->dwOptions|=dwOptions;
    else if (wParam == AECOOP_AND)
      ae->popt->dwOptions&=dwOptions;
    else if (wParam == AECOOP_XOR)
      ae->popt->dwOptions&=~dwOptions;
    return ae->popt->dwOptions;
  }
  if (uMsg == EM_SETREADONLY)
  {
    if (wParam)
      ae->popt->dwOptions|=AECO_READONLY;
    else
      ae->popt->dwOptions&=~AECO_READONLY;
    return 1;
  }
  if (uMsg == EM_SETBKGNDCOLOR)
  {
    AECOLORS aec;

    aec.dwFlags=AECLR_BASICBK|AECLR_ACTIVELINEBK;

    if (!wParam)
    {
      aec.crBasicBk=lParam;
      aec.crActiveLineBk=lParam;
    }
    else aec.dwFlags|=AECLR_DEFAULT;

    AE_SetColors(ae, &aec);
    return 0;
  }
  if (uMsg == EM_GETAUTOURLDETECT)
  {
    return ae->popt->bDetectUrl;
  }
  if (uMsg == EM_AUTOURLDETECT)
  {
    ae->popt->bDetectUrl=wParam;
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
    AE_StackUpdateClones(ae);
    return 0;
  }

  //Draw
  if (uMsg == EM_SHOWSCROLLBAR)
  {
    BOOL bUpdate=FALSE;

    if (wParam == SB_BOTH || wParam == SB_HORZ)
    {
      if (ae->bHScrollShow != lParam)
      {
        ae->bHScrollShow=lParam;
        bUpdate=TRUE;
      }
    }
    if (wParam == SB_BOTH || wParam == SB_VERT)
    {
      if (ae->bVScrollShow != lParam)
      {
        ae->bVScrollShow=lParam;
        bUpdate=TRUE;
      }
    }
    if (bUpdate)
      ShowScrollBar(ae->hWndEdit, wParam, lParam);

    return 0;
  }
  if (uMsg == EM_HIDESELECTION)
  {
    if (wParam)
      ae->popt->dwOptions&=~AECO_NOHIDESEL;
    else
      ae->popt->dwOptions|=AECO_NOHIDESEL;
    AE_HideSelection(ae, wParam);
    return 0;
  }


  //// WM_* system messages

  if (uMsg == WM_GETFONT)
  {
    if (ae->ptxt->hFont == (HFONT)GetStockObject(SYSTEM_FONT))
      return (LRESULT)NULL;
    else
      return (LRESULT)ae->ptxt->hFont;
  }
  else if (uMsg == WM_SETFONT)
  {
    int nFirstVisibleLine=0;

    if (!ae->popt->bVScrollLock)
      nFirstVisibleLine=AE_GetFirstVisibleLine(ae);

    if (!ae->bUnicodeWindow)
      AE_SetEditFontA(ae, (HFONT)wParam, FALSE);
    else
      AE_SetEditFontW(ae, (HFONT)wParam, FALSE);

    ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
    AE_UpdateScrollBars(ae, SB_VERT);
    AE_CalcLinesWidth(ae, NULL, NULL, AECLW_FRESH);
    ae->ptCaret.x=0;
    ae->ptCaret.y=0;
    AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL|AESELT_LOCKNOTIFY);
    if (!ae->popt->bVScrollLock)
      AE_VScrollLine(ae, nFirstVisibleLine - AE_GetFirstVisibleLine(ae), AESB_ALIGNTOP);
    AE_UpdateCaret(ae, ae->bFocus);

    if (ae->ptxt->dwWordWrap) AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, !lParam);
    AE_StackUpdateClones(ae);
    return 0;
  }
  else if (uMsg == WM_CUT)
  {
    AE_EditCut(ae);
    return 0;
  }
  else if (uMsg == WM_COPY)
  {
    AE_EditCopyToClipboard(ae);
    return 0;
  }
  else if (uMsg == WM_PASTE)
  {
    AE_EditPasteFromClipboard(ae, FALSE);
    return 0;
  }
  else if (uMsg == WM_CLEAR)
  {
    AE_EditKeyDelete(ae, FALSE);
    return 0;
  }
  else if (uMsg == WM_UNDO)
  {
    AE_EditUndo(ae);
    return 0;
  }
  else if (uMsg == WM_GETTEXTLENGTH)
  {
    if (ae->popt->nOutputNewLine == AELB_ASIS)
      return -AE_IndexSubtract(ae, NULL, NULL, ae->popt->nOutputNewLine, FALSE, FALSE);
    if (ae->popt->nOutputNewLine == AELB_R || ae->popt->nOutputNewLine == AELB_N)
      return ae->ptxt->nLastCharOffset;
    if (ae->popt->nOutputNewLine == AELB_RN)
      return ae->ptxt->nLastCharOffset + ae->ptxt->nLineCount;
    if (ae->popt->nOutputNewLine == AELB_RRN)
      return ae->ptxt->nLastCharOffset + ae->ptxt->nLineCount * 2;
    return 0;
  }
  else if (uMsg == WM_GETTEXT)
  {
    AECHARRANGE cr;
    DWORD dwResult;

    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin, FALSE);
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax, FALSE);

    if (!ae->bUnicodeWindow)
      dwResult=AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, (char *)lParam, wParam, AELB_ASOUTPUT, FALSE, FALSE);
    else
      dwResult=AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, (wchar_t *)lParam, wParam, AELB_ASOUTPUT, FALSE, FALSE);
    return dwResult;
  }
  else if (uMsg == WM_SETTEXT)
  {
    if (!ae->bUnicodeWindow)
      AE_SetTextAnsi(ae, CP_ACP, (char *)lParam, (DWORD)-1, AELB_ASINPUT);
    else
      AE_SetText(ae, (wchar_t *)lParam, (DWORD)-1, AELB_ASINPUT, FALSE);
    return TRUE;
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      AE_UpdateSize(ae);
    }
    return 0;
  }
  else if (uMsg == WM_GETDLGCODE)
  {
    UINT uCode=DLGC_WANTCHARS|DLGC_WANTARROWS;

    if (lParam && (((MSG *)lParam)->message == WM_KEYDOWN))
    {
      int nVk=(int)((MSG *)lParam)->wParam;

      if (nVk == VK_RETURN && (ae->popt->dwOptions & AECO_WANTRETURN))
      {
        uCode|=DLGC_WANTMESSAGE;
      }
      else if (nVk == VK_TAB && (GetKeyState(VK_CONTROL) < 0))
      {
        uCode|=DLGC_WANTMESSAGE;
      }
    }
    return uCode;
  }
  else if (uMsg == WM_KEYDOWN ||
           uMsg == WM_SYSKEYDOWN)
  {
    BOOL bAlt=FALSE;
    BOOL bShift=FALSE;
    BOOL bControl=FALSE;

    if (ae->popt->dwRichEventMask & ENM_KEYEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    if (GetKeyState(VK_MENU) < 0)
      bAlt=TRUE;
    if (GetKeyState(VK_SHIFT) < 0)
      bShift=TRUE;
    if (GetKeyState(VK_CONTROL) < 0)
      bControl=TRUE;

    //Character input: Alt + NumPad
    if (bAlt && !bShift && !bControl)
    {
      if (ae->nAltChar != AEAC_DODEFAULT)
      {
        if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
        {
          if (ae->nAltChar != AEAC_DECINPUT && ae->nAltChar <= AEAC_NONE)
          {
            if (wParam == VK_NUMPAD0)
            {
              if (ae->nAltChar == AEAC_NUMPAD0)
                ae->nAltChar=AEAC_DECINPUT;
              else
                ae->nAltChar=AEAC_NUMPAD0;
            }
            else
            {
              if ((ae->popt->dwOptions & AECO_ALTDECINPUT) && ae->nAltChar != AEAC_NUMPAD0)
                ae->nAltChar=wParam - VK_NUMPAD0;
              else
                ae->nAltChar=AEAC_DODEFAULT;
            }
          }
          else
          {
            if (ae->nAltChar == AEAC_DECINPUT)
            {
              if (wParam != VK_NUMPAD0)
                ae->nAltChar=wParam - VK_NUMPAD0;
            }
            else ae->nAltChar=ae->nAltChar * 10 + (wParam - VK_NUMPAD0);

            if (ae->nAltChar > 65536)
              ae->nAltChar=AEAC_KEYDOWN;
          }
        }
        else ae->nAltChar=AEAC_KEYDOWN;
      }
    }

    //Process virtual key
    if (AE_KeyDown(ae, wParam, bAlt, bShift, bControl))
      return 0;
  }
  else if (uMsg == WM_CHAR)
  {
    LRESULT lResult=0;

    if (ae->popt->dwRichEventMask & ENM_KEYEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    if (ae->nAltChar)
    {
      if (ae->nAltChar > AEAC_NONE)
      {
        if (GetKeyState(VK_NUMLOCK))
        {
          AE_EditChar(ae, ae->nAltChar, TRUE);
          lResult=1;
        }
      }
      ae->nAltChar=AEAC_NONE;
      return lResult;
    }

    if (wParam == VK_TAB || (wParam >= 0x20 && wParam != 0x7F))
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditChar(ae, wParam, ae->bUnicodeWindow);
        lResult=1;
      }
      return lResult;
    }
  }
  else if (uMsg == WM_DEADCHAR ||
           uMsg == WM_SYSCHAR ||
           uMsg == WM_SYSDEADCHAR ||
           uMsg == WM_KEYUP ||
           uMsg == WM_SYSKEYUP)
  {
    if (ae->popt->dwRichEventMask & ENM_KEYEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    if (wParam == VK_MENU)
    {
      if (ae->nAltChar == AEAC_DODEFAULT)
        ae->nAltChar=AEAC_NONE;
      else if (ae->nAltChar == AEAC_KEYDOWN)
        ae->nAltChar=AEAC_KEYUP;
    }
    if (uMsg == WM_SYSCHAR)
    {
      //WM_KEYUP and WM_SYSKEYUP with VK_MENU doesn't sended after menu open with Alt+Key
      ae->nAltChar=AEAC_NONE;
    }
  }
  else if (uMsg == WM_INPUTLANGCHANGE)
  {
    ae->dwInputLocale=lParam;
  }
  else if (uMsg == WM_IME_STARTCOMPOSITION)
  {
    HIMC hImc;

    if (PRIMARYLANGID(ae->dwInputLocale) == LANG_KOREAN)
      return 0;

    if (hImc=ImmGetContext(ae->hWndEdit))
    {
      AE_UpdateCompositionPos(ae, hImc);
      if (!ae->bUnicodeWindow)
        ImmSetCompositionFontA(hImc, &ae->ptxt->lfFontA);
      else
        ImmSetCompositionFontW(hImc, &ae->ptxt->lfFontW);

      ImmReleaseContext(ae->hWndEdit, hImc);
    }
  }
  else if (uMsg == WM_IME_COMPOSITION)
  {
    if (!AE_IsReadOnly(ae))
    {
      if (PRIMARYLANGID(ae->dwInputLocale) == LANG_KOREAN)
      {
        AECHARINDEX ciSelStart;
        wchar_t wszCompStr[2];
        HIMC hImc;
        int nStrLen;

        if (hImc=ImmGetContext(ae->hWndEdit))
        {
          if (lParam & GCS_RESULTSTR)
          {
            ae->ptxt->bLockGroupStopInt=TRUE;

            if ((nStrLen=ImmGetCompositionStringW(hImc, GCS_RESULTSTR, wszCompStr, 2 * sizeof(wchar_t))) > 0)
            {
              if ((DWORD)nStrLen > sizeof(wchar_t) || wszCompStr[0] != ae->dwImeChar)
              {
                if (nStrLen == sizeof(wchar_t))
                  AE_EditChar(ae, wszCompStr[0], TRUE);
                else
                  AE_ReplaceSel(ae, wszCompStr, nStrLen / sizeof(wchar_t), AELB_ASINPUT, FALSE, NULL, NULL);

                if (ae->popt->dwOptions & AECO_DETAILEDUNDO)
                {
                  ae->ptxt->bLockGroupStopInt=FALSE;
                  AE_StackUndoGroupStop(ae);
                  ae->ptxt->bLockGroupStopInt=TRUE;
                }
              }
              else
              {
                if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
                  AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ae->ciSelEndIndex, FALSE, 0, AESCT_IME);
              }
            }

            if ((nStrLen=ImmGetCompositionStringW(hImc, GCS_COMPSTR, wszCompStr, 2 * sizeof(wchar_t))) > 0)
            {
              AE_EditChar(ae, wszCompStr[0], TRUE);
              ae->dwImeChar=wszCompStr[0];

              ciSelStart=ae->ciSelStartIndex;
              AE_IndexDec(&ciSelStart);
              AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ciSelStart, FALSE, 0, AESCT_IME);
            }
            else ae->ptxt->bLockGroupStopInt=FALSE;
          }
          if (lParam & GCS_COMPSTR)
          {
            ae->ptxt->bLockGroupStopInt=TRUE;

            if (wParam)
            {
              if (wParam != ae->dwImeChar)
              {
                if (ImmGetCompositionStringW(hImc, GCS_RESULTSTR, NULL, 0) <= 0)
                {
                  AE_EditChar(ae, wParam, TRUE);
                  ae->dwImeChar=wParam;

                  ciSelStart=ae->ciSelStartIndex;
                  AE_IndexDec(&ciSelStart);
                  AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ciSelStart, FALSE, 0, AESCT_IME);
                }
              }
            }
            else
            {
              AE_EditKeyBackspace(ae, FALSE);

              ae->ptxt->bLockGroupStopInt=FALSE;
              AE_StackUndoGroupStop(ae);
              if (ae->ptxt->bModified != AE_GetModify(ae))
                AE_SetModify(ae, !ae->ptxt->bModified);
            }
          }
          ImmReleaseContext(ae->hWndEdit, hImc);
        }
        return 0;
      }
      else
      {
        if (lParam & GCS_RESULTSTR)
        {
          LRESULT lResult;

          if (!ae->bUnicodeWindow)
            lResult=DefWindowProcA(ae->hWndEdit, uMsg, wParam, lParam);
          else
            lResult=DefWindowProcW(ae->hWndEdit, uMsg, wParam, lParam);
          AE_UpdateCompositionPos(ae, 0);
          return lResult;
        }
      }
    }
  }
  else if (uMsg == WM_IME_ENDCOMPOSITION)
  {
  }
  else if (uMsg == WM_IME_NOTIFY)
  {
    if (PRIMARYLANGID(ae->dwInputLocale) == LANG_KOREAN)
    {
      if (wParam == IMN_OPENCANDIDATE)
      {
        AE_UpdateCandidatePos(ae, 0);
      }
    }
  }
  else if (uMsg == WM_IME_KEYDOWN)
  {
    if (PRIMARYLANGID(ae->dwInputLocale) == LANG_KOREAN)
    {
      if (wParam == VK_HANJA)
      {
        AECHARRANGE crSel;
        HIMC hImc;

        if (hImc=ImmGetContext(ae->hWndEdit))
        {
          AE_GetIndex(ae, AEGI_NEXTCHAR, &ae->ciSelStartIndex, &crSel.ciMax, FALSE);
          AE_GetIndex(ae, AEGI_PREVCHAR, &crSel.ciMax, &crSel.ciMin, FALSE);
          ae->dwImeChar=*(crSel.ciMin.lpLine->wpLine + crSel.ciMin.nCharInLine);

          if (ImmEscapeW((HKL)ae->dwInputLocale, hImc, IME_ESC_HANJA_MODE, &ae->dwImeChar))
          {
            AE_SetSelectionPos(ae, &crSel.ciMax, &crSel.ciMin, FALSE, 0, AESCT_IME);
            AE_UpdateCandidatePos(ae, hImc);
          }
          ImmReleaseContext(ae->hWndEdit, hImc);
        }
      }
    }
  }
  else if (uMsg == WM_IME_CHAR)
  {
    if (!AE_IsReadOnly(ae))
    {
      if (!ae->bUnicodeWindow)
      {
        if (HIBYTE(wParam))
          AE_EditChar(ae, HIBYTE(wParam), FALSE);
        AE_EditChar(ae, LOBYTE(wParam), FALSE);
      }
      else AE_EditChar(ae, wParam, TRUE);
    }
    return 0;
  }
  else if (uMsg == WM_HSCROLL)
  {
    if (ae->popt->dwRichEventMask & ENM_SCROLLEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    AE_HScroll(ae, LOWORD(wParam), 0);
    return 0;
  }
  else if (uMsg == WM_VSCROLL)
  {
    if (ae->popt->dwRichEventMask & ENM_SCROLLEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    AE_VScroll(ae, LOWORD(wParam), 0);
    return 0;
  }
  else if (uMsg == WM_LBUTTONDOWN ||
           uMsg == WM_LBUTTONUP ||
           uMsg == WM_LBUTTONDBLCLK ||
           uMsg == WM_RBUTTONDOWN ||
           uMsg == WM_RBUTTONUP ||
           uMsg == WM_RBUTTONDBLCLK ||
           uMsg == WM_MBUTTONDOWN ||
           uMsg == WM_MBUTTONUP ||
           uMsg == WM_MBUTTONDBLCLK ||
           uMsg == WM_MOUSEMOVE ||
           uMsg == WM_CAPTURECHANGED)
  {
    //Notify
    if (uMsg == WM_LBUTTONDOWN ||
        uMsg == WM_LBUTTONDBLCLK ||
        uMsg == WM_RBUTTONDOWN ||
        uMsg == WM_RBUTTONDBLCLK ||
        uMsg == WM_MBUTTONDOWN ||
        uMsg == WM_MBUTTONDBLCLK ||
        uMsg == WM_MOUSEMOVE)
    {
      if (ae->popt->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
      if (ae->nCurrentCursor == AECC_URL)
        if (AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink))
          return 0;
    }

    //Stop scroll
    if (uMsg == WM_LBUTTONDOWN ||
        uMsg == WM_RBUTTONDOWN ||
        uMsg == WM_MBUTTONDOWN)
    {
      if (ae->bMButtonDown)
      {
        if (ae->dwMouseScrollTimer)
        {
          KillTimer(ae->hWndEdit, ae->dwMouseScrollTimer);
          ae->dwMouseScrollTimer=0;
          AE_ReleaseMouseCapture(ae, AEMSC_MOUSESCROLL);
        }
        AE_MButtonErase(ae);
        ae->bMButtonDown=FALSE;
        ae->nCurrentCursor=AE_SetCursor(ae);
        return 0;
      }
    }

    if (uMsg == WM_LBUTTONDOWN ||
        uMsg == WM_LBUTTONDBLCLK)
    {
      POINT ptPos;
      int nLButtonDownCurTime=GetMessageTime();
      BOOL bAlt=FALSE;
      BOOL bShift=FALSE;
      BOOL bControl=FALSE;

      if (GetKeyState(VK_MENU) < 0)
        bAlt=TRUE;
      if (wParam & MK_SHIFT)
        bShift=TRUE;
      if (wParam & MK_CONTROL)
        bControl=TRUE;

      GetCursorPos(&ptPos);
      ScreenToClient(ae->hWndEdit, &ptPos);
      if (LOWORD(ptPos.x) != LOWORD(lParam) || LOWORD(ptPos.y) != HIWORD(lParam))
      {
        //Current mouse position doesn't match message position.
        return 0;
      }

      if (ae->ptLButtonDownPrevPos.x == ptPos.x && ae->ptLButtonDownPrevPos.y == ptPos.y &&
          GetDoubleClickTime() >= (DWORD)(nLButtonDownCurTime - ae->nLButtonDownPrevTime))
      {
        ++ae->nLButtonDownCount;

        //We support only three clicks
        ae->nLButtonDownCount=ae->nLButtonDownCount % 3;
      }
      else ae->nLButtonDownCount=0;

      ae->ptLButtonDownPrevPos=ptPos;
      ae->nLButtonDownPrevTime=nLButtonDownCurTime;

      //One click
      if (ae->nLButtonDownCount == 0)
      {
        if (GetFocus() != ae->hWndEdit)
          SetFocus(ae->hWndEdit);

        //Start margin selection capture
        if (!bAlt && !bShift && ae->nCurrentCursor == AECC_MARGIN)
        {
          AECHARRANGE cr;
          AECHARINDEX ciCharIndex;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            AE_SetMouseCapture(ae, AEMSC_MOUSEMOVE);
          }

          AE_GetCharFromPos(ae, &ptPos, &ciCharIndex, NULL, ae->bColumnSel);
          cr.ciMin=ciCharIndex;
          cr.ciMax=ciCharIndex;
          cr.ciMin.nCharInLine=0;
          if (!AE_NextLineEx(&cr.ciMax, &cr.ciMax))
            cr.ciMax.nCharInLine=cr.ciMax.lpLine->nLineLen;

          ae->ciMouseSelClick=ciCharIndex;
          ae->ciMouseSelStart=cr.ciMin;
          ae->ciMouseSelEnd=cr.ciMax;
          ae->dwMouseSelType=AEMSS_LINES;
          AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, FALSE, AESELT_MOUSE, AESCT_MOUSELEFTMARGIN|AESCT_MOUSESINGLECLK);
        }
        //Start drag source capture
        else if (!ae->bDragging && !bAlt && !bShift && ae->nCurrentCursor == AECC_SELECTION)
        {
          AE_SetMouseCapture(ae, AEMSC_MOUSEDRAG);
          ae->bDragging=TRUE;
          ae->nMoveBeforeBeginDrag=AEMMB_BEGINDRAG;
        }
        //Start selection change capture
        else
        {
          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            AE_SetMouseCapture(ae, AEMSC_MOUSEMOVE);
          }

          if (!bShift)
            ae->dwMouseSelType=AEMSS_CHARS;
          AE_SetMouseSelection(ae, &ptPos, bAlt, bShift);
        }
      }
      //Two clicks
      else if (ae->nLButtonDownCount == 1)
      {
        if (ae->nCurrentCursor == AECC_MARGIN)
        {
          if (!bAlt && !bShift)
          {
            //Two clicks in left margin are ignored
          }
        }
        else
        {
          AECHARINDEX ciPrevWord;
          AECHARINDEX ciNextWord;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            AE_SetMouseCapture(ae, AEMSC_MOUSEMOVE);
          }

          if (!AE_GetPrevBreak(ae, &ae->ciCaretIndex, &ciPrevWord, ae->bColumnSel, AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND|AEWB_STOPSPACESTART|AEWB_STOPSPACEEND|(!ae->ciCaretIndex.lpLine->nLineLen?0:AEWB_STOPNEWLINE)))
            ciPrevWord=ae->ciCaretIndex;
          if (!AE_GetNextBreak(ae, &ciPrevWord, &ciNextWord, ae->bColumnSel, AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND|(bControl?AEWB_SKIPSPACESTART:AEWB_STOPSPACESTART)|AEWB_STOPSPACEEND|(!ae->ciCaretIndex.lpLine->nLineLen?0:AEWB_STOPNEWLINE)))
            ciNextWord=ae->ciCaretIndex;

          ae->ciMouseSelClick=ae->ciCaretIndex;
          ae->ciMouseSelStart=ciPrevWord;
          ae->ciMouseSelEnd=ciNextWord;
          ae->dwMouseSelType=AEMSS_WORDS;
          AE_SetSelectionPos(ae, &ciNextWord, &ciPrevWord, ae->bColumnSel, AESELT_MOUSE, AESCT_MOUSEDOUBLECLK);
        }
      }
      //Three clicks
      else if (ae->nLButtonDownCount == 2)
      {
        if (ae->nCurrentCursor == AECC_MARGIN)
        {
          if (!bAlt && !bShift)
          {
            AE_EditSelectAll(ae, 0, AESCT_MOUSELEFTMARGIN|AESCT_MOUSETRIPLECLK);
          }
        }
        else
        {
          AECHARRANGE cr;
          AECHARINDEX ciCharIndex;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            AE_SetMouseCapture(ae, AEMSC_MOUSEMOVE);
          }

          AE_GetCharFromPos(ae, &ptPos, &ciCharIndex, NULL, ae->bColumnSel);
          AE_GetIndex(ae, AEGI_WRAPLINEBEGIN, &ciCharIndex, &cr.ciMin, FALSE);
          AE_GetIndex(ae, AEGI_WRAPLINEEND, &ciCharIndex, &cr.ciMax, FALSE);
          AE_NextLineEx(&cr.ciMax, &cr.ciMax);

          ae->ciMouseSelClick=ciCharIndex;
          ae->ciMouseSelStart=cr.ciMin;
          ae->ciMouseSelEnd=cr.ciMax;
          ae->dwMouseSelType=AEMSS_LINES;
          AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, FALSE, AESELT_MOUSE, AESCT_MOUSETRIPLECLK);
        }
      }
      return 0;
    }
    else if (uMsg == WM_MBUTTONDOWN)
    {
      if (!ae->bMButtonDown)
      {
        if (ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left ||
            ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
        {
          if (ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left &&
              ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
            ae->hMButtonBitmap=hAkelEditBitmapMCenterAll;
          else if (ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
            ae->hMButtonBitmap=hAkelEditBitmapMCenterLeftRight;
          else if (ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
            ae->hMButtonBitmap=hAkelEditBitmapMCenterTopBottom;

          GetCursorPos(&ae->ptMButtonDown);
          ScreenToClient(ae->hWndEdit, &ae->ptMButtonDown);
          ae->bMButtonDown=TRUE;
          ae->bMButtonUp=FALSE;
          ae->nMButtonMoveBeforeScroll=AEMMB_MBUTTONSCROLL;

          AE_MButtonDraw(ae);

          if (!ae->dwMouseScrollTimer)
          {
            //Timer
            ae->dwMouseScrollTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSESCROLL, 32, NULL);
            AE_SetMouseCapture(ae, AEMSC_MOUSESCROLL);
          }
        }
      }
      return 0;
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
      if (ae->bDragging)
      {
        if (ae->nMoveBeforeBeginDrag > 0)
          --ae->nMoveBeforeBeginDrag;

        if (ae->nMoveBeforeBeginDrag == 0)
        {
          DWORD dwEffectIn;
          DWORD dwEffectOut=DROPEFFECT_NONE;
          DWORD dwResult=DRAGDROP_S_CANCEL;

          dwEffectIn=DROPEFFECT_COPY;
          if (!(ae->popt->dwOptions & AECO_READONLY))
            dwEffectIn|=DROPEFFECT_MOVE;

          if (!AE_NotifyDropSource(ae, AEDS_SOURCEBEGIN, &dwEffectIn, 0))
          {
            lpAkelEditDrag=ae;
            ae->bDragSelectionDelete=TRUE;
            ae->dwDragSelectionLength=AE_DataObjectCopySelection(ae);

            dwResult=DoDragDrop((IDataObject *)&ae->ido, (IDropSource *)&ae->ids, dwEffectIn, &dwEffectOut);

            if (!AE_NotifyDropSource(ae, AEDS_SOURCEEND, &dwEffectOut, dwResult))
            {
              if (dwResult == DRAGDROP_S_DROP)
              {
                if (dwEffectOut & DROPEFFECT_MOVE)
                {
                  if (ae->bDragSelectionDelete)
                  {
                    AE_NotifyChanging(ae, AETCT_DRAGDELETE);
                    AE_StackUndoGroupStop(ae);
                    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
                    AE_StackUndoGroupStop(ae);
                    AE_NotifyChanged(ae); //AETCT_DRAGDELETE
                  }
                }
              }
            }
            AE_DataObjectFreeSelection(ae);
            ((IDataObject *)&ae->ido)->lpVtbl->Release((IDataObject *)&ae->ido);
            ((IDropSource *)&ae->ids)->lpVtbl->Release((IDropSource *)&ae->ids);
            lpAkelEditDrag=NULL;
            ae->bDragSelectionDelete=FALSE;
            ae->dwDragSelectionLength=0;
          }
          ae->bDragging=FALSE;
          AE_ReleaseMouseCapture(ae, AEMSC_MOUSEDRAG);
          AE_NotifyDropSource(ae, AEDS_SOURCEDONE, &dwEffectOut, dwResult);
        }
      }
      else
      {
        if (ae->dwMouseScrollTimer)
          ae->nCurrentCursor=AE_SetCursor(ae);
        AE_MouseMove(ae);

        if (ae->bMButtonDown)
        {
          if (ae->nMButtonMoveBeforeScroll > 0)
            --ae->nMButtonMoveBeforeScroll;
        }
      }
      return 0;
    }
    else if (uMsg == WM_LBUTTONUP)
    {
      if (ae->bDragging)
      {
        ae->bDragging=FALSE;
        AE_ReleaseMouseCapture(ae, AEMSC_MOUSEDRAG);

        //Set selection if nMoveBeforeBeginDrag not reached
        {
          POINT ptPos;

          GetCursorPos(&ptPos);
          ScreenToClient(ae->hWndEdit, &ptPos);

          if (!(wParam & MK_SHIFT))
            ae->dwMouseSelType=AEMSS_CHARS;
          AE_SetMouseSelection(ae, &ptPos, ae->bColumnSel, FALSE);
        }
      }
      if (ae->dwMouseMoveTimer)
      {
        KillTimer(ae->hWndEdit, ae->dwMouseMoveTimer);
        ae->dwMouseMoveTimer=0;
        AE_ReleaseMouseCapture(ae, AEMSC_MOUSEMOVE);
      }
      if (!(ae->popt->dwOptions & AECO_LBUTTONUPCONTINUECAPTURE))
        ae->dwMouseSelType=AEMSS_CHARS;
      if (ae->dwMouseSelType)
        ae->dwMouseSelType|=AEMSS_LBUTTONUP;
    }
    else if (uMsg == WM_MBUTTONUP)
    {
      if (ae->bMButtonDown)
      {
        if (!ae->bMButtonUp)
        {
          if (!ae->nMButtonMoveBeforeScroll)
          {
            if (ae->dwMouseScrollTimer)
            {
              KillTimer(ae->hWndEdit, ae->dwMouseScrollTimer);
              ae->dwMouseScrollTimer=0;
              AE_ReleaseMouseCapture(ae, AEMSC_MOUSESCROLL);
            }
            AE_MButtonErase(ae);
            ae->bMButtonDown=FALSE;
            ae->nCurrentCursor=AE_SetCursor(ae);
          }
          else ae->bMButtonUp=TRUE;
        }
      }
    }
    else if (uMsg == WM_CAPTURECHANGED)
    {
      if (ae->dwMouseScrollTimer)
      {
        KillTimer(ae->hWndEdit, ae->dwMouseScrollTimer);
        ae->dwMouseScrollTimer=0;
        AE_ReleaseMouseCapture(ae, AEMSC_MOUSESCROLL);
      }
      if (ae->dwMouseMoveTimer)
      {
        KillTimer(ae->hWndEdit, ae->dwMouseMoveTimer);
        ae->dwMouseMoveTimer=0;
        AE_ReleaseMouseCapture(ae, AEMSC_MOUSEMOVE);
      }
    }

    //Notify
    if (uMsg == WM_LBUTTONUP ||
        uMsg == WM_RBUTTONUP ||
        uMsg == WM_MBUTTONUP)
    {
      if (ae->popt->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
      if (ae->nCurrentCursor == AECC_URL)
        if (AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink))
          return 0;
    }
  }
  else if (uMsg == WM_MOUSEWHEEL)
  {
    DWORD dwLines;

    if (ae->popt->dwRichEventMask & ENM_MOUSEEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    SystemParametersInfoA(SPI_GETWHEELSCROLLLINES, 0, &dwLines, 0);

    if ((short)HIWORD(wParam) < 0)
    {
      if (dwLines == (DWORD)-1)
        AE_VScroll(ae, SB_PAGEDOWN, 0);
      else
        AE_VScrollLine(ae, dwLines, AESB_ALIGNTOP);
    }
    else
    {
      if (dwLines == (DWORD)-1)
        AE_VScroll(ae, SB_PAGEUP, 0);
      else
        AE_VScrollLine(ae, -(int)dwLines, AESB_ALIGNTOP);
    }
    return 0;
  }
  else if (uMsg == WM_SETCURSOR)
  {
    if (!ae->bDragging && !ae->dwMouseMoveTimer && !ae->dwMouseScrollTimer)
    {
      ae->nCurrentCursor=AE_SetCursor(ae);

      if (ae->nCurrentCursor == AECC_URL)
        AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink);
      if (ae->nCurrentCursor != AECC_IBEAM)
        return TRUE;
    }
  }
  else if (uMsg == WM_TIMER)
  {
    if (wParam == AETIMERID_MOUSEMOVE ||
        wParam == AETIMERID_MOUSESCROLL)
    {
      AE_MouseMove(ae);
    }
    return 0;
  }
  else if (uMsg == WM_DROPFILES)
  {
    if (!AE_NotifyDropFiles(ae, (HDROP)wParam))
      return 0;
  }
  else if (uMsg == WM_SETFOCUS)
  {
    ae->bFocus=TRUE;
    AE_UpdateCaret(ae, ae->bFocus);

    if (!(ae->popt->dwOptions & AECO_NOHIDESEL))
    {
      AE_HideSelection(ae, FALSE);
    }
    AE_NotifySetFocus(ae, (HWND)wParam);
    return 0;
  }
  else if (uMsg == WM_KILLFOCUS)
  {
    if (!(ae->popt->dwOptions & AECO_NOHIDESEL))
    {
      AE_HideSelection(ae, TRUE);
    }
    ae->bFocus=FALSE;
    DestroyCaret();
    AE_StackUndoGroupStop(ae);
    AE_NotifyKillFocus(ae, (HWND)wParam);
    return 0;
  }
  else if (uMsg == WM_SYSCOLORCHANGE)
  {
    if (ae->popt->bDefaultColors)
    {
      AECOLORS aec;

      aec.dwFlags=AECLR_DEFAULT|AECLR_ALL;
      AE_SetColors(ae, &aec);
      ae->popt->bDefaultColors=TRUE;
    }
  }
  else if (uMsg == WM_ENABLE)
  {
    if (wParam)
    {
      AECOLORS aec;

      aec.dwFlags=AECLR_BASICBK|AECLR_SELBK|AECLR_ACTIVELINEBK;
      aec.crBasicBk=ae->popt->crEnableBasicBk;
      aec.crSelBk=ae->popt->crEnableSelBk;
      aec.crActiveLineBk=ae->popt->crEnableActiveLineBk;
      AE_SetColors(ae, &aec);
    }
    else
    {
      AECOLORS aec;

      aec.dwFlags=AECLR_BASICBK|AECLR_SELBK|AECLR_ACTIVELINEBK;
      AE_GetColors(ae, &aec);
      ae->popt->crEnableBasicBk=aec.crBasicBk;
      ae->popt->crEnableSelBk=aec.crSelBk;
      ae->popt->crEnableActiveLineBk=aec.crActiveLineBk;

      aec.dwFlags=AECLR_BASICBK|AECLR_SELBK|AECLR_ACTIVELINEBK;
      aec.crBasicBk=GetSysColor(COLOR_BTNFACE);
      aec.crSelBk=GetSysColor(COLOR_BTNFACE);
      aec.crActiveLineBk=GetSysColor(COLOR_BTNFACE);
      AE_SetColors(ae, &aec);
    }
    return 0;
  }
  else if (uMsg == WM_ERASEBKGND)
  {
    RECT rcErase;
    HBRUSH hBasicBk;
    AEERASE *lpErase=(AEERASE *)ae->hEraseStack.first;
    AEERASE *lpEraseNext;

    hBasicBk=CreateSolidBrush(ae->popt->crBasicBk);

    //Message came not from WM_PAINT - use all edit area
    if (!lpErase)
      lpErase=AE_StackEraseInsert(ae, &ae->rcEdit);

    while (lpErase)
    {
      if (lpErase->rcErase.right - lpErase->rcErase.left > 0 &&
          lpErase->rcErase.bottom - lpErase->rcErase.top > 0)
      {
        if (lpErase->rcErase.left < ae->rcDraw.left)
        {
          rcErase=lpErase->rcErase;
          rcErase.right=min(rcErase.right, ae->rcDraw.left);
          FillRect((HDC)wParam, &rcErase, hBasicBk);
          lpErase->rcErase.left=rcErase.right;
        }
        if (lpErase->rcErase.top < ae->rcDraw.top)
        {
          rcErase=lpErase->rcErase;
          rcErase.bottom=min(rcErase.bottom, ae->rcDraw.top);
          FillRect((HDC)wParam, &rcErase, hBasicBk);
          lpErase->rcErase.top=rcErase.bottom;
        }
        if (lpErase->rcErase.right > ae->rcDraw.right)
        {
          rcErase=lpErase->rcErase;
          rcErase.left=max(rcErase.left, ae->rcDraw.right);
          FillRect((HDC)wParam, &rcErase, hBasicBk);
          lpErase->rcErase.right=rcErase.left;
        }
        if (lpErase->rcErase.bottom > ae->rcDraw.bottom)
        {
          rcErase=lpErase->rcErase;
          rcErase.top=max(rcErase.top, ae->rcDraw.bottom);
          FillRect((HDC)wParam, &rcErase, hBasicBk);
          lpErase->rcErase.bottom=rcErase.top;
        }

        //Erase only a space after the last line
        rcErase=lpErase->rcErase;
        rcErase.top=max(rcErase.top, ae->rcDraw.top + (ae->ptxt->nVScrollMax - ae->nVScrollPos));

        if (rcErase.top < rcErase.bottom)
        {
          FillRect((HDC)wParam, &rcErase, hBasicBk);
        }
      }

      //Next erase rectangle
      lpEraseNext=lpErase->next;
      AE_StackEraseDelete(ae, lpErase);
      lpErase=lpEraseNext;
    }
    DeleteObject(hBasicBk);

    return 1;
  }
  else if (uMsg == WM_PAINT)
  {
    HDC hDC=ae->hDC;
    HFONT hFontOld=NULL;

    //Get DC
    if (!ae->hDC)
    {
      if (ae->hDC=GetDC(ae->hWndEdit))
        if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(ae->hDC, ae->ptxt->hFont);
    }
    if (ae->hDC)
    {
      AE_ActiveColumnErase(ae);
      AE_Paint(ae);
      AE_ColumnMarkerDraw(ae);

      if (ae->bMButtonDown)
      {
        AE_MButtonErase(ae);
        ae->bMButtonDown=FALSE;
        UpdateWindow(ae->hWndEdit);
        ae->bMButtonDown=TRUE;
        AE_MButtonDraw(ae);
      }
      if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
      {
        if (!ae->popt->bActiveColumnDraw)
        {
          //Draw new vertical line
          AE_GlobalToClient(ae, &ae->ptCaret, &ae->ptActiveColumnDraw);
          AE_ActiveColumnDraw(ae);
          ae->popt->bActiveColumnDraw=TRUE;
        }
      }

      //Release DC
      if (!hDC)
      {
        if (hFontOld) SelectObject(ae->hDC, hFontOld);
        ReleaseDC(ae->hWndEdit, ae->hDC);
        ae->hDC=NULL;
      }
    }
    return 0;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Unregister drop window
    RevokeDragDrop(ae->hWndEdit);
    //CoLockObjectExternal((LPUNKNOWN)&ae->idt, FALSE, TRUE);
    //((IDropTarget *)&ae->idt)->Release();

    AE_DestroyWindowData(ae);
    lpAkelEditPrev=NULL;
    ae=NULL;
    return 0;
  }

  if (!ae->bUnicodeWindow)
    return DefWindowProcA(ae->hWndEdit, uMsg, wParam, lParam);
  else
    return DefWindowProcW(ae->hWndEdit, uMsg, wParam, lParam);
}

AKELEDIT* AE_CreateWindowData(HWND hWnd, CREATESTRUCTA *cs, AEEditProc lpEditProc)
{
  AKELEDIT *ae;

  if (ae=AE_StackWindowInsert(&hAkelEditWindowsStack))
  {
    ae->lpEditProc=lpEditProc;
    ae->hWndEdit=hWnd;
    ae->hWndParent=GetParent(ae->hWndEdit);
    ae->nEditCtrlID=GetWindowLongA(ae->hWndEdit, GWL_ID);
    ae->bUnicodeWindow=IsWindowUnicode(ae->hWndEdit);
    ae->ptxt=&ae->txt;
    ae->popt=&ae->opt;
    ae->ptxt->hHeap=NULL;
    ae->ptxt->nTabStop=AETAB_DEFAULTSIZE;
    ae->ptxt->bSavePointExist=TRUE;
    ae->ptxt->dwTextLimit=(DWORD)-1;
    ae->ptxt->dwUndoLimit=(DWORD)-1;
    ae->ptxt->nHideMinLineOffset=1;
    ae->ptxt->nHideMaxLineOffset=-1;
    ae->bHScrollShow=TRUE;
    ae->bVScrollShow=TRUE;
    ae->popt->crCaret=RGB(0x00, 0x00, 0x00);
    ae->popt->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
    ae->popt->crBasicBk=GetSysColor(COLOR_WINDOW);
    ae->popt->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
    ae->popt->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
    ae->popt->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
    ae->popt->crActiveLineBk=GetSysColor(COLOR_WINDOW);
    ae->popt->crUrlText=RGB(0x00, 0x00, 0xFF);
    ae->popt->crActiveColumn=RGB(0x00, 0x00, 0x00);
    ae->popt->crColumnMarker=GetSysColor(COLOR_BTNFACE);
    ae->popt->bDefaultColors=TRUE;
    ae->popt->nCaretInsertWidth=1;
    ae->popt->nCaretOvertypeHeight=2;
    ae->popt->dwUrlMaxLength=AEMAX_LINKLENGTH;
    ae->popt->dwWordBreak=AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND|AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND|AEWB_SKIPSPACESTART|AEWB_STOPSPACEEND;
    ae->dwInputLocale=(DWORD)GetKeyboardLayout(0);
    ae->bCaretVisible=TRUE;
    ae->nCurrentCursor=AECC_IBEAM;

    //OLE Drag'n'Drop
    ae->idtVtbl.QueryInterface=AEIDropTarget_QueryInterface;
    ae->idtVtbl.AddRef=AEIDropTarget_AddRef;
    ae->idtVtbl.Release=AEIDropTarget_Release;
    ae->idtVtbl.DragEnter=AEIDropTarget_DragEnter;
    ae->idtVtbl.DragOver=AEIDropTarget_DragOver;
    ae->idtVtbl.DragLeave=AEIDropTarget_DragLeave;
    ae->idtVtbl.Drop=AEIDropTarget_Drop;

    ae->idsVtbl.QueryInterface=AEIDropSource_QueryInterface;
    ae->idsVtbl.AddRef=AEIDropSource_AddRef;
    ae->idsVtbl.Release=AEIDropSource_Release;
    ae->idsVtbl.QueryContinueDrag=AEIDropSource_QueryContinueDrag;
    ae->idsVtbl.GiveFeedback=AEIDropSource_GiveFeedback;

    ae->idoVtbl.QueryInterface=AEIDataObject_QueryInterface;
    ae->idoVtbl.AddRef=AEIDataObject_AddRef;
    ae->idoVtbl.Release=AEIDataObject_Release;
    ae->idoVtbl.GetData=AEIDataObject_GetData;
    ae->idoVtbl.GetDataHere=AEIDataObject_GetDataHere;
    ae->idoVtbl.QueryGetData=AEIDataObject_QueryGetData;
    ae->idoVtbl.GetCanonicalFormatEtc=AEIDataObject_GetCanonicalFormatEtc;
    ae->idoVtbl.SetData=AEIDataObject_SetData;
    ae->idoVtbl.EnumFormatEtc=AEIDataObject_EnumFormatEtc;
    ae->idoVtbl.DAdvise=AEIDataObject_DAdvise;
    ae->idoVtbl.DUnadvise=AEIDataObject_DUnadvise;
    ae->idoVtbl.EnumDAdvise=AEIDataObject_EnumDAdvise;

    ae->idt.lpTable=&ae->idtVtbl;
    ae->idt.uRefCount=0;
    ae->idt.ae=ae;
    ae->idt.bAllowDrop=FALSE;

    ae->ids.lpTable=&ae->idsVtbl;
    ae->ids.uRefCount=0;
    ae->ids.ae=ae;

    ae->ido.lpTable=&ae->idoVtbl;
    ae->ido.uRefCount=0;
    ae->ido.ae=ae;
    ae->ido.fmtetc[0].cfFormat=CF_UNICODETEXT;
    ae->ido.fmtetc[0].ptd=0;
    ae->ido.fmtetc[0].dwAspect=DVASPECT_CONTENT;
    ae->ido.fmtetc[0].lindex=-1;
    ae->ido.fmtetc[0].tymed=TYMED_HGLOBAL;
    ae->ido.stgmed[0].hGlobal=NULL;
    ae->ido.stgmed[0].pUnkForRelease=0;
    ae->ido.fmtetc[1].cfFormat=CF_TEXT;
    ae->ido.fmtetc[1].ptd=0;
    ae->ido.fmtetc[1].dwAspect=DVASPECT_CONTENT;
    ae->ido.fmtetc[1].lindex=-1;
    ae->ido.fmtetc[1].tymed=TYMED_HGLOBAL;
    ae->ido.stgmed[1].hGlobal=NULL;
    ae->ido.stgmed[1].pUnkForRelease=0;
    ae->ido.fmtetc[2].cfFormat=0;
    ae->ido.fmtetc[2].ptd=0;
    ae->ido.fmtetc[2].dwAspect=DVASPECT_CONTENT;
    ae->ido.fmtetc[2].lindex=-1;
    ae->ido.fmtetc[2].tymed=TYMED_HGLOBAL;
    ae->ido.stgmed[2].hGlobal=NULL;
    ae->ido.stgmed[2].pUnkForRelease=0;
    ae->ido.nNumFormats=3;

    if (!ae->bUnicodeWindow)
    {
      if (!xstrcmpiA((char *)cs->lpszClass, AES_RICHEDITCLASSA))
        ae->bRichEditClass=TRUE;
    }
    else
    {
      if (!xstrcmpiW((wchar_t *)cs->lpszClass, AES_RICHEDITCLASSW))
        ae->bRichEditClass=TRUE;
    }

    if (!ae->bRichEditClass)
    {
      ae->popt->nInputNewLine=AELB_ASIS;
      ae->popt->nOutputNewLine=AELB_ASIS;
    }
    else
    {
      ae->popt->nInputNewLine=AELB_R;
      ae->popt->nOutputNewLine=AELB_RN;
    }

    if (cs->style & ES_READONLY)
      ae->popt->dwOptions|=AECO_READONLY;
    if (cs->style & ES_DISABLENOSCROLL)
      ae->popt->dwOptions|=AECO_DISABLENOSCROLL;
    if (cs->style & ES_NOHIDESEL)
      ae->popt->dwOptions|=AECO_NOHIDESEL;
    if (cs->style & ES_WANTRETURN)
      ae->popt->dwOptions|=AECO_WANTRETURN;
    if (cs->style & ES_HEAP_SERIALIZE)
      ae->popt->bHeapSerialize=TRUE;
    xmemcpy(ae->ptxt->wszWrapDelimiters, AES_WRAPDELIMITERSW, sizeof(AES_WRAPDELIMITERSW));
    xmemcpy(ae->popt->wszWordDelimiters, AES_WORDDELIMITERSW, sizeof(AES_WORDDELIMITERSW));
    xmemcpy(ae->popt->wszUrlLeftDelimiters, AES_URLLEFTDELIMITERSW, sizeof(AES_URLLEFTDELIMITERSW));
    xmemcpy(ae->popt->wszUrlRightDelimiters, AES_URLRIGHTDELIMITERSW, sizeof(AES_URLRIGHTDELIMITERSW));
    xmemcpy(ae->popt->wszUrlPrefixes, AES_URLPREFIXESW, sizeof(AES_URLPREFIXESW));
    AE_GetUrlPrefixes(ae);

    GetClientRect(ae->hWndEdit, &ae->rcEdit);
    AE_SetDrawRect(ae, NULL, FALSE);

    if (!ae->bUnicodeWindow)
      AE_SetEditFontA(ae, NULL, FALSE);
    else
      AE_SetEditFontW(ae, NULL, FALSE);

    AE_SetText(ae, L"", 0, AELB_ASINPUT, TRUE);

    //Scrollbars updated in WM_SIZE
  }
  return ae;
}

AKELEDIT* AE_SetWindowData(AKELEDIT *aeOld, AKELEDIT *aeNew, DWORD dwFlags)
{
  if (aeOld != aeNew)
  {
    //Assign new AKELEDIT.
    aeNew->hWndEdit=aeOld->hWndEdit;

    //Unassign current AKELEDIT.
    aeOld->hWndEdit=NULL;

    //Update focus state.
    if (GetFocus() == aeNew->hWndEdit)
      aeNew->bFocus=TRUE;
    else
      aeNew->bFocus=FALSE;

    //Register Drag'n'Drop with new idt pointer.
    if (!(dwFlags & AESWD_NODRAGDROP))
    {
      RevokeDragDrop(aeNew->hWndEdit);
      RegisterDragDrop(aeNew->hWndEdit, (LPDROPTARGET)&aeNew->idt);
    }

    //Redraw edit window.
    if (!(dwFlags & AESWD_NOSHOWSCROLLBARS))
    {
      if (aeNew->bHScrollShow != aeOld->bHScrollShow)
        ShowScrollBar(aeNew->hWndEdit, SB_HORZ, aeNew->bHScrollShow);
      if (aeNew->bVScrollShow != aeOld->bVScrollShow)
        ShowScrollBar(aeNew->hWndEdit, SB_VERT, aeNew->bVScrollShow);
    }
    if (!(dwFlags & AESWD_NOUPDATESCROLLBARS))
    {
      AE_UpdateScrollBars(aeNew, SB_BOTH);
    }
    if (!(dwFlags & AESWD_NOUPDATECARET))
    {
      AE_UpdateCaret(aeNew, aeNew->bFocus);
    }
    if (!(dwFlags & AESWD_NOINVALIDATERECT))
    {
      InvalidateRect(aeNew->hWndEdit, NULL, TRUE);
    }
  }
  return aeOld;
}

void AE_DestroyWindowData(AKELEDIT *ae)
{
  if (!ae->lpMaster)
  {
    //Destroying master - uncloning all clones
    AE_StackCloneDeleteAll(ae);
  }
  else
  {
    //Destroying clone - uncloning current clone
    AECLONE *aec;

    if (aec=AE_StackCloneGet(ae->lpMaster, ae->hWndEdit))
      AE_StackCloneDelete(aec);
  }

  if (ae->ptxt->hHeap)
  {
    if (HeapDestroy(ae->ptxt->hHeap))
    {
      ae->ptxt->hHeap=NULL;
      --hAkelEditHeapCount;
    }
  }
  else AE_StackLineFree(ae);

  AE_StackFoldFree(ae);
  AE_StackPointFree(ae);
  AE_StackEraseFree(ae);

  AE_HeapStackDelete(NULL, (stack **)&hAkelEditWindowsStack.first, (stack **)&hAkelEditWindowsStack.last, (stack *)ae);
}

HANDLE AE_HeapCreate(AKELEDIT *ae)
{
  //Free memory
  if (ae->ptxt->hHeap)
  {
    if (HeapDestroy(ae->ptxt->hHeap))
    {
      ae->ptxt->hHeap=NULL;
      --hAkelEditHeapCount;
    }
  }
  else AE_StackLineFree(ae);

  //Reset variables
  ae->ptxt->hLinesStack.first=0;
  ae->ptxt->hLinesStack.last=0;
  ae->ptxt->hUndoStack.first=0;
  ae->ptxt->hUndoStack.last=0;
  ae->ptxt->lpCurrentUndo=NULL;
  ae->ptxt->lpSavePoint=NULL;
  ae->ptxt->bSavePointExist=TRUE;
  ae->ptxt->dwUndoCount=0;
  ae->ptxt->liMaxWidthLine.nLine=0;
  ae->ptxt->liMaxWidthLine.lpLine=NULL;
  ae->ptxt->liLineUnwrapLastCall.nLine=0;
  ae->ptxt->liLineUnwrapLastCall.lpLine=NULL;
  ae->ptxt->nLastCharOffset=0;
  ae->ptxt->nLineCount=0;
  ae->ptxt->nLineUnwrapCount=0;
  ae->ptxt->nLineUnwrapLastCall=0;
  ae->ptxt->nHScrollMax=0;
  ae->ptxt->nVScrollMax=0;
  ae->liFirstDrawLine.nLine=0;
  ae->liFirstDrawLine.lpLine=NULL;
  ae->ciSelStartIndex.nLine=0;
  ae->ciSelStartIndex.nCharInLine=0;
  ae->ciSelStartIndex.lpLine=NULL;
  ae->ciSelEndIndex.nLine=0;
  ae->ciSelEndIndex.nCharInLine=0;
  ae->ciSelEndIndex.lpLine=NULL;
  ae->ciCaretIndex.nLine=0;
  ae->ciCaretIndex.nCharInLine=0;
  ae->ciCaretIndex.lpLine=NULL;
  ae->ciLastCallIndex.nLine=0;
  ae->ciLastCallIndex.nCharInLine=0;
  ae->ciLastCallIndex.lpLine=NULL;
  ae->nFirstDrawLineOffset=0;
  ae->nSelStartCharOffset=0;
  ae->nSelEndCharOffset=0;
  ae->nLastCallOffset=0;
  ae->nHScrollPos=0;
  ae->nVScrollPos=0;
  ae->nLastHScrollPos=0;
  ae->nLastVScrollPos=0;
  ae->ptCaret.x=0;
  ae->ptCaret.y=0;
  ae->nCaretHorzIndent=0;
  ae->bColumnSel=FALSE;
  ae->hEraseStack.first=0;
  ae->hEraseStack.last=0;

  //Create heap
  if (hAkelEditHeapCount < AEMAX_HEAPCREATE)
  {
    if (ae->ptxt->hHeap=HeapCreate(ae->popt->bHeapSerialize?0:HEAP_NO_SERIALIZE, 0, 0))
    {
      ++hAkelEditHeapCount;
    }
  }
  else
  {
    //HeapCreate return NULL, if HeapCreate called more than ~8000 times for process.
    //After HeapCreate return NULL, program became unstable, so use less limit.
    //At this point current AKELEDIT will use process heap.
    ae->ptxt->hHeap=NULL;
  }
  return ae->ptxt->hHeap;
}

LPVOID AE_HeapAlloc(AKELEDIT *ae, DWORD dwFlags, SIZE_T dwBytes)
{
  LPVOID lpResult;
  HANDLE hHeap;

  if (!ae || !ae->ptxt->hHeap)
    hHeap=hAkelEditProcessHeap;
  else
    hHeap=ae->ptxt->hHeap;

  if (!(lpResult=HeapAlloc(hHeap, dwFlags, dwBytes)))
  {
    if (ae) AE_NotifyErrSpace(ae, dwBytes);
  }
  return lpResult;
}

BOOL AE_HeapFree(AKELEDIT *ae, DWORD dwFlags, LPVOID lpMem)
{
  HANDLE hHeap;

  if (!ae || !ae->ptxt->hHeap)
    hHeap=hAkelEditProcessHeap;
  else
    hHeap=ae->ptxt->hHeap;

  return HeapFree(hHeap, dwFlags, lpMem);
}

int AE_HeapStackInsertIndex(AKELEDIT *ae, stack **first, stack **last, stack **element, int nIndex, int nBytes)
{
  stack *tmp;
  int nCount;

  *element=NULL;

  if (nIndex > 0)
  {
    tmp=*first;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*last;
    nCount=-1;
  }
  else return 1;

  while (tmp || nCount == nIndex)
  {
    if (nCount == nIndex)
    {
      if (nIndex > 0)
        return AE_HeapStackInsertBefore(ae, first, last, tmp, element, nBytes);
      else
        return AE_HeapStackInsertAfter(ae, first, last, tmp, element, nBytes);
    }
    if (nIndex > 0)
    {
      tmp=tmp->next;
      ++nCount;
    }
    else
    {
      tmp=tmp->prev;
      --nCount;
    }
  }
  return 1;
}

int AE_HeapStackInsertBefore(AKELEDIT *ae, stack **first, stack **last, stack *index, stack **element, int nBytes)
{
  *element=NULL;

  if (*element=(stack *)AE_HeapAlloc(ae, HEAP_ZERO_MEMORY, nBytes))
  {
    if (!index)
    {
      if (*last)
      {
        (*last)->next=*element;
        (*element)->prev=*last;
      }
      else
      {
        *first=*element;
      }
      *last=*element;
    }
    else
    {
      if (index == *first) *first=*element;
      else index->prev->next=*element;

      (*element)->next=index;
      (*element)->prev=index->prev;
      index->prev=*element;
    }
  }
  else return 2;

  return 0;
}

int AE_HeapStackInsertAfter(AKELEDIT *ae, stack **first, stack **last, stack *index, stack **element, int nBytes)
{
  *element=NULL;

  if (*element=(stack *)AE_HeapAlloc(ae, HEAP_ZERO_MEMORY, nBytes))
  {
    if (!index)
    {
      if (*first)
      {
        (*first)->prev=*element;
        (*element)->next=*first;
      }
      else
      {
        *last=*element;
      }
      *first=*element;
    }
    else
    {
      if (index == *last) *last=*element;
      else index->next->prev=*element;

      (*element)->prev=index;
      (*element)->next=index->next;
      index->next=*element;
    }
  }
  else return 2;

  return 0;
}

int AE_HeapStackMoveBefore(AKELEDIT *ae, stack **first, stack **last, stack *src, stack *dst)
{
  if (src == dst) return 2;

  //Unlink src
  if (src == *first)
  {
    *first=src->next;
    if (*first) (*first)->prev=NULL;
    else *last=NULL;
  }
  else if (src == *last)
  {
    *last=src->prev;
    if (*last) (*last)->next=NULL;
    else *first=NULL;
  }
  else
  {
    src->prev->next=src->next;
    src->next->prev=src->prev;
  }
  src->next=NULL;
  src->prev=NULL;

  //Insert element
  if (!dst)
  {
    if (*last)
    {
      (*last)->next=src;
      src->prev=*last;
    }
    else
    {
      *first=src;
    }
    *last=src;
  }
  else
  {
    if (dst == *first) *first=src;
    else dst->prev->next=src;

    src->next=dst;
    src->prev=dst->prev;
    dst->prev=src;
  }
  return 0;
}

int AE_HeapStackDelete(AKELEDIT *ae, stack **first, stack **last, stack *element)
{
  if (!element) return 1;

  if (element == *first)
  {
    *first=element->next;
    if (*first) (*first)->prev=NULL;
    else *last=NULL;
  }
  else if (element == *last)
  {
    *last=element->prev;
    if (*last) (*last)->next=NULL;
    else *first=NULL;
  }
  else
  {
    element->prev->next=element->next;
    element->next->prev=element->prev;
  }
  AE_HeapFree(ae, 0, (LPVOID)element);
  return 0;
}

void AE_HeapStackClear(AKELEDIT *ae, stack **first, stack **last)
{
  stack *tmp1=*first;
  stack *tmp2;

  while (tmp1)
  {
    tmp2=tmp1->next;
    AE_HeapFree(ae, 0, (LPVOID)tmp1);
    tmp1=tmp2;
  }
  *last=NULL;
  *first=NULL;
}

AKELEDIT* AE_StackWindowInsert(HSTACK *hStack)
{
  AKELEDIT *lpElement=NULL;

  AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AKELEDIT));
  return lpElement;
}

AKELEDIT* AE_StackWindowGet(HSTACK *hStack, HWND hWndEdit)
{
  AKELEDIT *lpElement=(AKELEDIT *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hWndEdit == hWndEdit)
      return lpElement;

    lpElement=lpElement->next;
  }
  return NULL;
}

void AE_StackWindowMakeFirst(HSTACK *hStack, AKELEDIT *ae)
{
  AE_HeapStackMoveBefore(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack *)ae, (stack *)hStack->first);
}

void AE_StackWindowFree(HSTACK *hStack)
{
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

void AE_ActivateClone(AKELEDIT *lpAkelEditPrev, AKELEDIT *ae)
{
  if (lpAkelEditPrev && ae->hWndEdit != lpAkelEditPrev->hWndEdit)
  {
    //Save previous window info
    if (lpAkelEditPrev->nCloneCount > 0 || lpAkelEditPrev->lpMaster)
    {
      AKELEDIT *aeSource;

      if (lpAkelEditPrev->lpMaster)
        aeSource=lpAkelEditPrev->lpMaster;
      else
        aeSource=lpAkelEditPrev;

      if (aeSource)
      {
        if (!lpAkelEditPrev->lpSelStartPoint)
        {
          //Get selection start from master
          lpAkelEditPrev->lpSelStartPoint=AE_StackPointInsert(aeSource, &aeSource->ciSelStartIndex);
        }
        else
        {
          AE_StackPointDelete(lpAkelEditPrev, lpAkelEditPrev->lpSelStartPoint);
          lpAkelEditPrev->lpSelStartPoint=AE_StackPointInsert(lpAkelEditPrev, &lpAkelEditPrev->ciSelStartIndex);
        }

        if (!lpAkelEditPrev->lpSelEndPoint)
        {
          //Get selection end from master
          lpAkelEditPrev->lpSelEndPoint=AE_StackPointInsert(aeSource, &aeSource->ciSelEndIndex);
        }
        else
        {
          AE_StackPointDelete(lpAkelEditPrev, lpAkelEditPrev->lpSelEndPoint);
          lpAkelEditPrev->lpSelEndPoint=AE_StackPointInsert(lpAkelEditPrev, &lpAkelEditPrev->ciSelEndIndex);
        }

        if (!lpAkelEditPrev->lpCaretPoint)
        {
          //Get caret index from master
          lpAkelEditPrev->lpCaretPoint=AE_StackPointInsert(aeSource, &aeSource->ciCaretIndex);
        }
        else
        {
          AE_StackPointDelete(lpAkelEditPrev, lpAkelEditPrev->lpCaretPoint);
          lpAkelEditPrev->lpCaretPoint=AE_StackPointInsert(lpAkelEditPrev, &lpAkelEditPrev->ciCaretIndex);
        }

        //Clear lines selection
        if (AE_IndexCompare(&lpAkelEditPrev->ciSelStartIndex, &lpAkelEditPrev->ciSelEndIndex))
        {
          AELINEDATA *lpLine=lpAkelEditPrev->ciSelStartIndex.lpLine;

          while (lpLine)
          {
            lpLine->nSelStart=0;
            lpLine->nSelEnd=0;
            if (lpLine == lpAkelEditPrev->ciSelEndIndex.lpLine) break;

            lpLine=lpLine->next;
          }
        }
      }
    }
  }

  if (!lpAkelEditPrev || ae->hWndEdit != lpAkelEditPrev->hWndEdit)
  {
    //Set current window info
    if (ae->nCloneCount > 0 || ae->lpMaster)
    {
      if (ae->lpSelStartPoint && ae->lpSelEndPoint && ae->lpCaretPoint)
      {
        ae->liFirstDrawLine.nLine=0;
        ae->liFirstDrawLine.lpLine=NULL;
        ae->nFirstDrawLineOffset=0;
        ae->ciSelStartIndex.lpLine=NULL;
        ae->nSelStartCharOffset=0;
        ae->ciSelEndIndex.lpLine=NULL;
        ae->nSelEndCharOffset=0;
        ae->ciCaretIndex.lpLine=NULL;
        ae->ciLastCallIndex.lpLine=NULL;
        ae->nLastCallOffset=0;
        ae->ptCaret.x=0;
        ae->ptCaret.y=0;

        ae->nSelStartCharOffset=AE_AkelIndexToRichOffset(ae, &ae->lpSelStartPoint->ciPoint);
        ae->ciSelStartIndex=ae->lpSelStartPoint->ciPoint;

        ae->nSelEndCharOffset=AE_AkelIndexToRichOffset(ae, &ae->lpSelEndPoint->ciPoint);
        ae->ciSelEndIndex=ae->lpSelEndPoint->ciPoint;

        ae->ciCaretIndex=ae->lpCaretPoint->ciPoint;
        AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL);

        AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING|AESELT_RESETSELECTION);
      }
    }
  }
}

AECLONE* AE_StackCloneIndex(AKELEDIT *ae, DWORD dwIndex)
{
  AECLONE *lpElement=(AECLONE *)ae->hClonesStack.first;

  while (lpElement)
  {
    if (dwIndex-- == 0)
      break;

    lpElement=lpElement->next;
  }
  return lpElement;
}

AECLONE* AE_StackCloneGet(AKELEDIT *ae, HWND hWnd)
{
  AECLONE *lpElement=(AECLONE *)ae->hClonesStack.first;

  while (lpElement)
  {
    if (lpElement->aeClone->hWndEdit == hWnd)
      break;

    lpElement=lpElement->next;
  }
  return lpElement;
}

AECLONE* AE_StackCloneAdd(AKELEDIT *aeMaster, AKELEDIT *aeClone)
{
  AECLONE *aec=NULL;

  if (!aeClone->lpMaster)
  {
    AE_HeapStackInsertIndex(NULL, (stack **)&aeMaster->hClonesStack.first, (stack **)&aeMaster->hClonesStack.last, (stack **)&aec, -1, sizeof(AECLONE));

    if (aec)
    {
      aec->aeClone=aeClone;
      aeClone->lpMaster=aeMaster;
      ++aeMaster->nCloneCount;

      //Associate AKELTEXT and AKELOPTIONS pointers
      aeClone->ptxt=aeMaster->ptxt;
      aeClone->popt=aeMaster->popt;

      ////Save margins info
      //aeClone->rcCloneMargins.left=aeClone->rcDraw.left - aeClone->rcEdit.left;
      //aeClone->rcCloneMargins.top=aeClone->rcDraw.top - aeClone->rcEdit.top;
      //aeClone->rcCloneMargins.right=aeClone->rcEdit.right - aeClone->rcDraw.right;
      //aeClone->rcCloneMargins.bottom=aeClone->rcEdit.bottom - aeClone->rcDraw.bottom;

      ////Copy margins info
      //aeClone->rcDraw.left=aeClone->rcEdit.left + (aeMaster->rcDraw.left - aeMaster->rcEdit.left);
      //aeClone->rcDraw.top=aeClone->rcEdit.top + (aeMaster->rcDraw.top - aeMaster->rcEdit.top);
      //aeClone->rcDraw.right=aeClone->rcEdit.right - (aeMaster->rcEdit.right - aeMaster->rcDraw.right);
      //aeClone->rcDraw.bottom=aeClone->rcEdit.bottom - (aeMaster->rcEdit.bottom - aeMaster->rcDraw.bottom);

      //Copy selection info
      xmemcpy(&aeClone->liFirstDrawLine, &aeMaster->liFirstDrawLine, (BYTE *)&aeMaster->hDC - (BYTE *)&aeMaster->liFirstDrawLine);

      AE_UpdateScrollBars(aeClone, SB_BOTH);
    }
  }
  return aec;
}

void AE_StackCloneDelete(AECLONE *aec)
{
  AKELEDIT *aeMaster=aec->aeClone->lpMaster;
  AKELEDIT *aeClone=aec->aeClone;

  if (aeClone->lpMaster)
  {
    --aeMaster->nCloneCount;
    aeClone->lpMaster=NULL;
    AE_HeapStackDelete(NULL, (stack **)&aeMaster->hClonesStack.first, (stack **)&aeMaster->hClonesStack.last, (stack *)aec);

    if (aeClone->lpSelStartPoint)
    {
      AE_StackPointDelete(aeMaster, aeClone->lpSelStartPoint);
      aeClone->lpSelStartPoint=NULL;
    }
    if (aeClone->lpSelEndPoint)
    {
      AE_StackPointDelete(aeMaster, aeClone->lpSelEndPoint);
      aeClone->lpSelEndPoint=NULL;
    }
    if (aeClone->lpCaretPoint)
    {
      AE_StackPointDelete(aeMaster, aeClone->lpCaretPoint);
      aeClone->lpCaretPoint=NULL;
    }

    //Deassociate AKELTEXT and AKELOPTIONS pointers
    aeClone->ptxt=&aeClone->txt;
    aeClone->popt=&aeClone->opt;

    ////Restore margins info
    //aeClone->rcDraw.left=aeClone->rcEdit.left + aeClone->rcCloneMargins.left;
    //aeClone->rcDraw.top=aeClone->rcEdit.top + aeClone->rcCloneMargins.top;
    //aeClone->rcDraw.right=aeClone->rcEdit.right - aeClone->rcCloneMargins.right;
    //aeClone->rcDraw.bottom=aeClone->rcEdit.bottom - aeClone->rcCloneMargins.bottom;

    //Reset selection info
    xmemset(&aeClone->liFirstDrawLine, 0, (BYTE *)&aeClone->hDC - (BYTE *)&aeClone->liFirstDrawLine);
    AE_GetIndex(aeClone, AEGI_FIRSTCHAR, NULL, &aeClone->ciSelStartIndex, FALSE);
    AE_GetIndex(aeClone, AEGI_FIRSTCHAR, NULL, &aeClone->ciSelEndIndex, FALSE);
    AE_GetIndex(aeClone, AEGI_FIRSTCHAR, NULL, &aeClone->ciCaretIndex, FALSE);
    AE_UpdateScrollBars(aeClone, SB_BOTH);

    if (!aeMaster->nCloneCount)
    {
      //Last clone deleted - update selection of master window
      AE_UpdateSelection(aeMaster, AESELT_COLUMNASIS|AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING|AESELT_RESETSELECTION);
    }
  }
}

void AE_StackCloneDeleteAll(AKELEDIT *ae)
{
  AECLONE *lpElement=(AECLONE *)ae->hClonesStack.first;
  AECLONE *lpTmp;

  while (lpElement)
  {
    lpTmp=lpElement->next;
    AE_StackCloneDelete(lpElement);
    lpElement=lpTmp;
  }
}

void AE_StackUpdateClones(AKELEDIT *ae)
{
  AECLONE *lpElement;
  HWND hWndSender=ae->hWndEdit;

  if (ae->lpMaster)
    ae=ae->lpMaster;

  //Master
  if (hWndSender != ae->hWndEdit)
  {
    AE_UpdateScrollBars(ae, SB_BOTH);
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
  }

  //Clones
  lpElement=(AECLONE *)ae->hClonesStack.first;

  while (lpElement)
  {
    if (hWndSender != lpElement->aeClone->hWndEdit)
    {
      AE_UpdateScrollBars(lpElement->aeClone, SB_BOTH);
      InvalidateRect(lpElement->aeClone->hWndEdit, &lpElement->aeClone->rcDraw, TRUE);
    }

    lpElement=lpElement->next;
  }
}

AKELEDIT* AE_StackDraggingGet(AKELEDIT *ae)
{
  AECLONE *lpElement;

  if (ae->bDragging) return ae;
  if (ae->lpMaster) ae=ae->lpMaster;
  if (ae->bDragging) return ae;
  lpElement=(AECLONE *)ae->hClonesStack.first;

  while (lpElement)
  {
    if (lpElement->aeClone->bDragging)
      return lpElement->aeClone;

    lpElement=lpElement->next;
  }
  return NULL;
}

AEERASE* AE_StackEraseInsert(AKELEDIT *ae, RECT *rcErase)
{
  AEERASE *lpElement=NULL;

  //Insert at the beginning
  if (!AE_HeapStackInsertIndex(NULL, (stack **)&ae->hEraseStack.first, (stack **)&ae->hEraseStack.last, (stack **)&lpElement, 1, sizeof(AEERASE)))
  {
    lpElement->rcErase=*rcErase;
  }
  return lpElement;
}

AEERASE* AE_StackEraseGet(AKELEDIT *ae, int nIndex)
{
  AEERASE *lpElement=(AEERASE *)ae->hEraseStack.first;
  int nItem=0;

  while (lpElement)
  {
    if (nIndex == nItem)
      return lpElement;

    ++nItem;
    lpElement=lpElement->next;
  }
  return NULL;
}

void AE_StackEraseDelete(AKELEDIT *ae, AEERASE *lpErase)
{
  AE_HeapStackDelete(NULL, (stack **)&ae->hEraseStack.first, (stack **)&ae->hEraseStack.last, (stack *)lpErase);
}

void AE_StackEraseFree(AKELEDIT *ae)
{
  AE_HeapStackClear(NULL, (stack **)&ae->hEraseStack.first, (stack **)&ae->hEraseStack.last);
}

AEFONTITEMA* AE_StackFontItemInsertA(HSTACK *hStack, LOGFONTA *lfFont)
{
  AEFONTITEMA *lpElement=NULL;
  LOGFONTA lfTmp;

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AEFONTITEMA)))
  {
    xmemcpy(&lpElement->lfFont, lfFont, offsetof(LOGFONTA, lfFaceName));
    xstrcpynA(lpElement->lfFont.lfFaceName, lfFont->lfFaceName, LF_FACESIZE);
    xmemcpy(&lfTmp, &lpElement->lfFont, sizeof(LOGFONTA));

    //Create normal font
    lfTmp.lfWeight=FW_NORMAL;
    lfTmp.lfItalic=FALSE;
    lpElement->hFontNormal=(HFONT)CreateFontIndirectA(&lfTmp);

    //Create bold font
    lfTmp.lfWeight=FW_BOLD;
    lfTmp.lfItalic=FALSE;
    lpElement->hFontBold=(HFONT)CreateFontIndirectA(&lfTmp);

    //Create italic font
    lfTmp.lfWeight=FW_NORMAL;
    lfTmp.lfItalic=TRUE;
    lpElement->hFontItalic=(HFONT)CreateFontIndirectA(&lfTmp);

    //Create bold italic font
    lfTmp.lfWeight=FW_BOLD;
    lfTmp.lfItalic=TRUE;
    lpElement->hFontBoldItalic=(HFONT)CreateFontIndirectA(&lfTmp);

    //Create URL font
    lfTmp.lfWeight=lfFont->lfWeight;
    lfTmp.lfItalic=lfFont->lfItalic;
    lfTmp.lfUnderline=TRUE;
    lpElement->hFontUrl=(HFONT)CreateFontIndirectA(&lfTmp);

    return lpElement;
  }
  return NULL;
}

AEFONTITEMW* AE_StackFontItemInsertW(HSTACK *hStack, LOGFONTW *lfFont)
{
  AEFONTITEMW *lpElement=NULL;
  LOGFONTW lfTmp;

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AEFONTITEMW)))
  {
    xmemcpy(&lpElement->lfFont, lfFont, offsetof(LOGFONTW, lfFaceName));
    xstrcpynW(lpElement->lfFont.lfFaceName, lfFont->lfFaceName, LF_FACESIZE);
    xmemcpy(&lfTmp, &lpElement->lfFont, sizeof(LOGFONTW));

    //Create normal font
    lfTmp.lfWeight=FW_NORMAL;
    lfTmp.lfItalic=FALSE;
    lpElement->hFontNormal=(HFONT)CreateFontIndirectW(&lfTmp);

    //Create bold font
    lfTmp.lfWeight=FW_BOLD;
    lfTmp.lfItalic=FALSE;
    lpElement->hFontBold=(HFONT)CreateFontIndirectW(&lfTmp);

    //Create italic font
    lfTmp.lfWeight=FW_NORMAL;
    lfTmp.lfItalic=TRUE;
    lpElement->hFontItalic=(HFONT)CreateFontIndirectW(&lfTmp);

    //Create bold italic font
    lfTmp.lfWeight=FW_BOLD;
    lfTmp.lfItalic=TRUE;
    lpElement->hFontBoldItalic=(HFONT)CreateFontIndirectW(&lfTmp);

    //Create URL font
    lfTmp.lfWeight=lfFont->lfWeight;
    lfTmp.lfItalic=lfFont->lfItalic;
    lfTmp.lfUnderline=TRUE;
    lpElement->hFontUrl=(HFONT)CreateFontIndirectW(&lfTmp);

    return lpElement;
  }
  return NULL;
}

AEFONTITEMA* AE_StackFontItemGetA(HSTACK *hStack, LOGFONTA *lfFont)
{
  AEFONTITEMA *lpElement=(AEFONTITEMA *)hStack->first;

  while (lpElement)
  {
    if (lpElement->lfFont.lfHeight == lfFont->lfHeight &&
        lpElement->lfFont.lfWeight == lfFont->lfWeight &&
        lpElement->lfFont.lfItalic == lfFont->lfItalic &&
        lpElement->lfFont.lfCharSet == lfFont->lfCharSet)
    {
      if (!xstrcmpiA(lpElement->lfFont.lfFaceName, lfFont->lfFaceName))
        return lpElement;
    }
    lpElement=lpElement->next;
  }
  return NULL;
}

AEFONTITEMW* AE_StackFontItemGetW(HSTACK *hStack, LOGFONTW *lfFont)
{
  AEFONTITEMW *lpElement=(AEFONTITEMW *)hStack->first;

  while (lpElement)
  {
    if (lpElement->lfFont.lfHeight == lfFont->lfHeight &&
        lpElement->lfFont.lfWeight == lfFont->lfWeight &&
        lpElement->lfFont.lfItalic == lfFont->lfItalic &&
        lpElement->lfFont.lfCharSet == lfFont->lfCharSet)
    {
      if (!xstrcmpiW(lpElement->lfFont.lfFaceName, lfFont->lfFaceName))
        return lpElement;
    }
    lpElement=lpElement->next;
  }
  return NULL;
}

void AE_StackFontItemsFreeA(HSTACK *hStack)
{
  AEFONTITEMA *lpElement=(AEFONTITEMA *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hFontNormal) DeleteObject(lpElement->hFontNormal);
    if (lpElement->hFontBold) DeleteObject(lpElement->hFontBold);
    if (lpElement->hFontItalic) DeleteObject(lpElement->hFontItalic);
    if (lpElement->hFontBoldItalic) DeleteObject(lpElement->hFontBoldItalic);
    if (lpElement->hFontUrl) DeleteObject(lpElement->hFontUrl);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

void AE_StackFontItemsFreeW(HSTACK *hStack)
{
  AEFONTITEMW *lpElement=(AEFONTITEMW *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hFontNormal) DeleteObject(lpElement->hFontNormal);
    if (lpElement->hFontBold) DeleteObject(lpElement->hFontBold);
    if (lpElement->hFontItalic) DeleteObject(lpElement->hFontItalic);
    if (lpElement->hFontBoldItalic) DeleteObject(lpElement->hFontBoldItalic);
    if (lpElement->hFontUrl) DeleteObject(lpElement->hFontUrl);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

AEBITMAPITEM* AE_StackBitmapItemInsert(HSTACK *hStack, AEBITMAPDATA *bd)
{
  AEBITMAPITEM *lpElement=NULL;

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AEBITMAPITEM)))
  {
    xmemcpy(&lpElement->bd, bd, sizeof(AEBITMAPDATA));

    lpElement->hBitmap=AE_CreateBitmap(bd->nWidth, bd->nHeight, bd->crBasic, bd->crInvert, bd->bZebra);

    return lpElement;
  }
  return NULL;
}

AEBITMAPITEM* AE_StackBitmapItemGet(HSTACK *hStack, AEBITMAPDATA *bd)
{
  AEBITMAPITEM *lpElement=(AEBITMAPITEM *)hStack->first;

  while (lpElement)
  {
    if (!xmemcmp(&lpElement->bd, bd, sizeof(AEBITMAPDATA)))
      return lpElement;

    lpElement=lpElement->next;
  }
  return NULL;
}

void AE_StackBitmapItemsFree(HSTACK *hStack)
{
  AEBITMAPITEM *lpElement=(AEBITMAPITEM *)hStack->first;

  while (lpElement)
  {
    if (lpElement->hBitmap) DeleteObject(lpElement->hBitmap);
    if (lpElement->hPatternBrush) DeleteObject(lpElement->hPatternBrush);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

AEFOLD* AE_StackFoldInsert(AKELEDIT *ae, AEPOINT *lpMinPoint, AEPOINT *lpMaxPoint)
{
  AEFOLD *lpMinParent;
  AEFOLD *lpMaxParent;
  AEFOLD *lpMinPrevSubling;
  AEFOLD *lpMaxPrevSubling;
  AEFOLD *lpMinSubling;
  AEFOLD *lpMaxSubling;
  AEFOLD **lppFirstChild;
  AEFOLD **lppLastChild;
  AEFOLD *lpElement=NULL;
  AEFOLD *lpNewElement=NULL;

  if (AE_IndexCompare(&lpMinPoint->ciPoint, &lpMaxPoint->ciPoint) > 0)
    return NULL;

  if (lpMinPoint->nPointOffset == AEPTO_CALC)
    lpMinPoint->nPointOffset=AE_AkelIndexToRichOffset(ae, &lpMinPoint->ciPoint);
  if (lpMaxPoint->nPointOffset == AEPTO_CALC)
    lpMaxPoint->nPointOffset=AE_AkelIndexToRichOffset(ae, &lpMaxPoint->ciPoint);

  //Fold start
  AE_StackFindFold(ae, AEFF_FINDOFFSET|AEFF_RECURSE, lpMinPoint->nPointOffset, NULL, &lpMinParent, &lpMinPrevSubling);

  //Fold end
  if (lpMinParent && lpMinParent->lpMaxPoint->nPointOffset == lpMaxPoint->nPointOffset)
  {
    lpMaxParent=lpMinParent;
    lpMaxPrevSubling=lpMinParent->lastChild;
  }
  else AE_StackFindFold(ae, AEFF_FINDOFFSET|AEFF_RECURSE, lpMaxPoint->nPointOffset + lpMaxPoint->nPointLen, lpMinParent, &lpMaxParent, &lpMaxPrevSubling);

  if (lpMinParent == lpMaxParent)
  {
    if (lpMinPrevSubling == lpMaxPrevSubling)
    {
      //Insert standalone fold
      if (!lpMinParent)
      {
        lppFirstChild=&ae->ptxt->hFoldsStack.first;
        lppLastChild=&ae->ptxt->hFoldsStack.last;
      }
      else
      {
        lppFirstChild=&lpMinParent->firstChild;
        lppLastChild=&lpMinParent->lastChild;
      }

      if (!AE_HeapStackInsertAfter(NULL, (stack **)lppFirstChild, (stack **)lppLastChild, (stack *)lpMinPrevSubling, (stack **)&lpNewElement, sizeof(AEFOLD)))
      {
        lpNewElement->parent=lpMinParent;
        lpNewElement->firstChild=NULL;
        lpNewElement->lastChild=NULL;
        ++ae->ptxt->nFoldCount;
      }
    }
    else
    {
      //Inserted fold became parent
      if (!lpMinParent)
      {
        lppFirstChild=&ae->ptxt->hFoldsStack.first;
        lppLastChild=&ae->ptxt->hFoldsStack.last;
      }
      else
      {
        lppFirstChild=&lpMinParent->firstChild;
        lppLastChild=&lpMinParent->lastChild;
      }

      //Split folds that became childrens
      if (!lpMinPrevSubling)
        lpMinSubling=*lppFirstChild;
      else
        lpMinSubling=lpMinPrevSubling->next;
      lpMaxSubling=lpMaxPrevSubling;
      StackSplit((stack **)lppFirstChild, (stack **)lppLastChild, (stack *)lpMinSubling, (stack *)lpMaxSubling);

      if (!AE_HeapStackInsertAfter(NULL, (stack **)lppFirstChild, (stack **)lppLastChild, (stack *)lpMinPrevSubling, (stack **)&lpNewElement, sizeof(AEFOLD)))
      {
        lpNewElement->parent=lpMinParent;
        lpNewElement->firstChild=lpMinSubling;
        lpNewElement->lastChild=lpMaxSubling;
        ++ae->ptxt->nFoldCount;
      }

      //Change parent for childrens
      for (lpElement=lpMinSubling; lpElement; lpElement=lpElement->next)
      {
        lpElement->parent=lpNewElement;
      }
    }
  }

  if (lpNewElement)
  {
    if (lpNewElement->lpMinPoint=AE_StackPointInsert(ae, &lpMinPoint->ciPoint))
    {
      lpNewElement->lpMinPoint->nPointOffset=lpMinPoint->nPointOffset;
      lpNewElement->lpMinPoint->nPointLen=lpMinPoint->nPointLen;
      lpNewElement->lpMinPoint->dwUserData=lpMinPoint->dwUserData;
      lpNewElement->lpMinPoint->dwFlags=lpMinPoint->dwFlags|AEPTF_FOLD;
    }
    if (lpNewElement->lpMaxPoint=AE_StackPointInsert(ae, &lpMaxPoint->ciPoint))
    {
      lpNewElement->lpMaxPoint->nPointOffset=lpMaxPoint->nPointOffset;
      lpNewElement->lpMaxPoint->nPointLen=lpMaxPoint->nPointLen;
      lpNewElement->lpMaxPoint->dwUserData=lpMaxPoint->dwUserData;
      lpNewElement->lpMaxPoint->dwFlags=lpMaxPoint->dwFlags|AEPTF_FOLD;
    }
    lpNewElement->bCollapse=FALSE;
    lpNewElement->crText=(DWORD)RGB(0xFF, 0x00, 0x00);
    lpNewElement->crBk=(DWORD)-1;
    lpNewElement->dwFontStyle=AEHLS_NONE;
  }
  ae->ptxt->lpVPosFold=NULL;
  return lpNewElement;
}

AEFOLD* AE_NextFold(AEFOLD *lpFold, BOOL bRecursive)
{
  if (lpFold)
  {
    if (bRecursive)
    {
      if (lpFold->firstChild)
        return lpFold->firstChild;
    }

    do
    {
      if (lpFold->next)
        return lpFold->next;
    }
    while (lpFold=lpFold->parent);
  }
  return lpFold;
}

AEFOLD* AE_PrevFold(AEFOLD *lpFold, BOOL bRecursive)
{
  if (lpFold)
  {
    if (bRecursive)
    {
      if (lpFold->lastChild)
        return lpFold->lastChild;
    }

    do
    {
      if (lpFold->prev)
        return lpFold->prev;
    }
    while (lpFold=lpFold->parent);
  }
  return lpFold;
}

void AE_StackFindFold(AKELEDIT *ae, DWORD dwFlags, DWORD dwFindIt, AEFOLD *lpForce, AEFOLD **lpParentOut, AEFOLD **lpPrevSublingOut)
{
  AEFOLD *lpParent=NULL;
  AEFOLD *lpPrevSubling=NULL;
  AEFOLD *lpSubling=NULL;
  int nFindOffset=0;
  int nFindLine=0;
  DWORD dwFirst;
  DWORD dwSecond;
  DWORD dwThird;
  DWORD dwFourth;
  BOOL bGoRoot=FALSE;

  if (ae->ptxt->hFoldsStack.first)
  {
    if (dwFlags & AEFF_FINDOFFSET)
      nFindOffset=(int)dwFindIt;
    else if (dwFlags & AEFF_FINDINDEX)
      nFindOffset=AE_AkelIndexToRichOffset(ae, (const AECHARINDEX *)dwFindIt);
    else if (dwFlags & AEFF_FINDLINE)
      nFindLine=(int)dwFindIt;
    else
      return;

    if (!lpForce)
    {
      dwFirst=(DWORD)-1;
      dwSecond=(DWORD)-1;
      dwThird=(DWORD)-1;
      dwFourth=(DWORD)-1;

      if (dwFlags & AEFF_FINDLINE)
      {
        dwFirst=mod(nFindLine - ae->ptxt->hFoldsStack.first->lpMinPoint->ciPoint.nLine);
        dwSecond=mod(nFindLine - ae->ptxt->hFoldsStack.last->lpMinPoint->ciPoint.nLine);
        if (ae->ptxt->lpVPosFold)
          dwThird=mod(nFindLine - ae->ptxt->lpVPosFold->lpMinPoint->ciPoint.nLine);
        if (ae->ptxt->lpIsCollapsedLastCall)
          dwFourth=mod(nFindLine - ae->ptxt->lpIsCollapsedLastCall->lpMinPoint->ciPoint.nLine);
      }
      else
      {
        dwFirst=mod(nFindOffset - ae->ptxt->hFoldsStack.first->lpMinPoint->nPointOffset);
        dwSecond=mod(nFindOffset - ae->ptxt->hFoldsStack.last->lpMinPoint->nPointOffset);
        if (ae->ptxt->lpVPosFold)
          dwThird=mod(nFindOffset - ae->ptxt->lpVPosFold->lpMinPoint->nPointOffset);
        if (ae->ptxt->lpIsCollapsedLastCall)
          dwFourth=mod(nFindOffset - ae->ptxt->lpIsCollapsedLastCall->lpMinPoint->nPointOffset);
      }

      if (dwFirst <= dwSecond && dwFirst <= dwThird && dwFirst <= dwFourth)
      {
        lpSubling=ae->ptxt->hFoldsStack.first;
      }
      else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth)
      {
        lpSubling=ae->ptxt->hFoldsStack.last;
      }
      else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth)
      {
        lpSubling=ae->ptxt->lpVPosFold;
      }
      else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird)
      {
        lpSubling=ae->ptxt->lpIsCollapsedLastCall;
      }
    }
    else lpSubling=lpForce;

    if (lpSubling->parent)
      bGoRoot=TRUE;

    if (!(dwFlags & AEFF_FINDLINE) ?
           //AEFF_FINDOFFSET or AEFF_FINDINDEX
           ((nFindOffset > lpSubling->lpMinPoint->nPointOffset) ||
            ((dwFlags & AEFF_FOLDSTART) && nFindOffset == lpSubling->lpMinPoint->nPointOffset)) :
           //AEFF_FINDLINE
           ((nFindLine > lpSubling->lpMinPoint->ciPoint.nLine) ||
            ((dwFlags & AEFF_FOLDSTART) && nFindLine == lpSubling->lpMinPoint->ciPoint.nLine)))
    {
      while (lpSubling)
      {
        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               ((nFindOffset < lpSubling->lpMinPoint->nPointOffset) ||
                (!(dwFlags & AEFF_FOLDSTART) && nFindOffset == lpSubling->lpMinPoint->nPointOffset)) :
               //AEFF_FINDLINE
               ((nFindLine < lpSubling->lpMinPoint->ciPoint.nLine) ||
                (!(dwFlags & AEFF_FOLDSTART) && nFindLine == lpSubling->lpMinPoint->ciPoint.nLine)))
        {
          break;
        }

        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               ((nFindOffset < lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen) ||
                ((dwFlags & AEFF_FOLDEND) && nFindOffset == lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen)) :
               //AEFF_FINDLINE
               ((nFindLine < lpSubling->lpMaxPoint->ciPoint.nLine) ||
                ((dwFlags & AEFF_FOLDEND) && nFindLine == lpSubling->lpMaxPoint->ciPoint.nLine)))
        {
          lpParent=lpSubling;
          lpPrevSubling=NULL;
          if (!(dwFlags & AEFF_RECURSE))
            break;

          //Recursive
          bGoRoot=FALSE;
          lpSubling=lpSubling->firstChild;
          continue;
        }
        if (bGoRoot)
        {
          lpSubling=lpSubling->parent;
          if (!lpSubling->parent)
            bGoRoot=FALSE;
        }
        else
        {
          lpPrevSubling=lpSubling;
          lpSubling=lpSubling->next;
        }
      }
    }
    else
    {
      while (lpSubling)
      {
        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               ((nFindOffset > lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen) ||
                (!(dwFlags & AEFF_FOLDEND) && nFindOffset == lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen)) :
               //AEFF_FINDLINE
               ((nFindLine > lpSubling->lpMaxPoint->ciPoint.nLine) ||
                (!(dwFlags & AEFF_FOLDEND) && nFindLine == lpSubling->lpMaxPoint->ciPoint.nLine)))
        {
          break;
        }

        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               ((nFindOffset > lpSubling->lpMinPoint->nPointOffset) ||
                ((dwFlags & AEFF_FOLDSTART) && nFindOffset == lpSubling->lpMinPoint->nPointOffset)) :
               //AEFF_FINDLINE
               ((nFindLine > lpSubling->lpMinPoint->ciPoint.nLine) ||
                ((dwFlags & AEFF_FOLDSTART) && nFindLine == lpSubling->lpMinPoint->ciPoint.nLine)))
        {
          lpParent=lpSubling;
          if (!(dwFlags & AEFF_RECURSE))
          {
            lpSubling=NULL;
            break;
          }

          //Recursive
          bGoRoot=FALSE;
          lpSubling=lpSubling->lastChild;
          continue;
        }
        if (bGoRoot)
        {
          lpSubling=lpSubling->parent;
          if (!lpSubling->parent)
            bGoRoot=FALSE;
        }
        else lpSubling=lpSubling->prev;
      }
      lpPrevSubling=lpSubling;
    }
  }
  if ((dwFlags & AEFF_GETROOT) && lpParent)
  {
    while (lpParent->parent) lpParent=lpParent->parent;
  }
  if (lpParentOut) *lpParentOut=lpParent;
  if (lpPrevSublingOut) *lpPrevSublingOut=lpPrevSubling;
}

AEFOLD* AE_StackIsLineCollapsed(AKELEDIT *ae, int nLine)
{
  AEFOLD *lpSubling=NULL;
  AEFOLD *lpPrevSubling=NULL;

  if (ae->ptxt->hFoldsStack.first)
  {
    //Check input fold
    if (ae->ptxt->lpIsCollapsedLastCall && ae->ptxt->lpIsCollapsedLastCall->bCollapse)
    {
      if (AE_FirstCollapsibleLine(ae, ae->ptxt->lpIsCollapsedLastCall) <= nLine &&
          AE_LastCollapsibleLine(ae, ae->ptxt->lpIsCollapsedLastCall) >= nLine)
      {
        return ae->ptxt->lpIsCollapsedLastCall;
      }
    }

    //Find fold by line
    AE_StackFindFold(ae, AEFF_FINDLINE|AEFF_FOLDSTART|AEFF_GETROOT, nLine, NULL, &lpSubling, &lpPrevSubling);
    if (lpSubling)
      ae->ptxt->lpIsCollapsedLastCall=lpSubling;
    else
      ae->ptxt->lpIsCollapsedLastCall=lpPrevSubling;

    while (lpSubling)
    {
      if (nLine < lpSubling->lpMinPoint->ciPoint.nLine)
        break;

      if (nLine <= lpSubling->lpMaxPoint->ciPoint.nLine)
      {
        if (lpSubling->bCollapse)
        {
          if (AE_FirstCollapsibleLine(ae, lpSubling) <= nLine &&
              AE_LastCollapsibleLine(ae, lpSubling) >= nLine)
          {
            ae->ptxt->lpIsCollapsedLastCall=lpSubling;
            return lpSubling;
          }
        }
        //Recursive
        lpSubling=lpSubling->firstChild;
        continue;
      }
      lpSubling=lpSubling->next;
    }
  }
  return NULL;
}

int AE_StackLineCollapse(AKELEDIT *ae, int nLine, DWORD dwFlags)
{
  AEFOLD *lpFold=NULL;
  AEFOLD *lpSubling=NULL;
  AEFOLD *lpPrevSubling=NULL;
  int nResult=0;

  if (ae->ptxt->hFoldsStack.first)
  {
    AE_StackFindFold(ae, AEFF_FINDLINE|AEFF_FOLDSTART|AEFF_GETROOT, nLine, NULL, &lpFold, &lpPrevSubling);
    if (!lpFold)
      lpSubling=lpPrevSubling;
    else
      lpSubling=lpFold;

    while (lpSubling)
    {
      if (nLine < lpSubling->lpMinPoint->ciPoint.nLine)
        break;

      if (nLine <= lpSubling->lpMaxPoint->ciPoint.nLine)
      {
        if (!lpSubling->bCollapse != !(dwFlags & AECF_COLLAPSE))
        {
          if (AE_FirstCollapsibleLine(ae, lpSubling) <= nLine &&
              AE_LastCollapsibleLine(ae, lpSubling) >= nLine)
          {
            lpSubling->bCollapse=!lpSubling->bCollapse;
            ++nResult;
          }
        }
        //Recursive
        lpSubling=lpSubling->firstChild;
        continue;
      }
      lpSubling=lpSubling->next;
    }
  }
  AE_StackFoldScroll(ae, lpFold, dwFlags);

  return nResult;
}

int AE_StackFoldCollapse(AKELEDIT *ae, AEFOLD *lpFold, DWORD dwFlags)
{
  AEFOLD *lpCount;
  int nResult=0;

  if (lpFold)
    lpCount=lpFold;
  else
    lpCount=ae->ptxt->hFoldsStack.first;

  while (lpCount)
  {
    if (!lpCount->bCollapse != !(dwFlags & AECF_COLLAPSE))
    {
      lpCount->bCollapse=!lpCount->bCollapse;
      ++nResult;
    }
    if (dwFlags & AECF_RECURSE)
    {
      //Custom AE_NextFold implementation
      if (lpCount->firstChild)
      {
        lpCount=lpCount->firstChild;
      }
      else
      {
         do
         {
           if (lpCount == lpFold)
             goto FoldScroll;
           if (lpCount->next)
           {
             lpCount=lpCount->next;
             break;
           }
         }
         while (lpCount=lpCount->parent);
      }
    }
    else if (!lpFold)
    {
      lpCount=lpCount->next;
    }
    else break;
  }

  //Find caret fold
  if (!lpFold && (dwFlags & AECF_COLLAPSE) && !(dwFlags & AECF_NOCARETCORRECT))
    AE_StackFindFold(ae, AEFF_FINDINDEX|AEFF_GETROOT, (DWORD)&ae->ciCaretIndex, NULL, &lpFold, NULL);

  FoldScroll:
  AE_StackFoldScroll(ae, lpFold, dwFlags);

  return nResult;
}

void AE_StackFoldScroll(AKELEDIT *ae, AEFOLD *lpFold, DWORD dwFlags)
{
  if (lpFold)
  {
    if ((dwFlags & AECF_COLLAPSE) && !(dwFlags & AECF_NOCARETCORRECT))
    {
      POINT ptPos;
      POINT *lpPos=&ptPos;

      if (AE_FirstCollapsibleLine(ae, lpFold) <= ae->ciCaretIndex.nLine &&
          AE_LastCollapsibleLine(ae, lpFold) >= ae->ciCaretIndex.nLine)
      {
        AE_SetSelectionPos(ae, &lpFold->lpMinPoint->ciPoint, &lpFold->lpMinPoint->ciPoint, FALSE, AESELT_LOCKSCROLL, 0);
        lpPos=NULL;
      }
      else
      {
        if (!(dwFlags & AECF_NOSCROLLCORRECT))
          AE_GetPosFromChar(ae, &lpFold->lpMinPoint->ciPoint, NULL, lpPos);
      }
      if (!(dwFlags & AECF_NOSCROLLCORRECT))
        AE_ScrollToPoint(ae, lpPos);
    }
  }
}

int AE_StackFoldUpdate(AKELEDIT *ae, int nFirstVisibleLine)
{
  int nFirstVisiblePos;
  int nScrolled=0;

  ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
  AE_UpdateScrollBars(ae, SB_VERT);
  ae->ptCaret.x=0;
  ae->ptCaret.y=0;
  AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
  if (!ae->popt->bVScrollLock && nFirstVisibleLine >= 0)
  {
    nFirstVisiblePos=AE_VPos(ae, nFirstVisibleLine, AEVPF_VPOSFROMLINE);
    nScrolled=AE_ScrollEditWindow(ae, SB_VERT, nFirstVisiblePos);
  }
  InvalidateRect(ae->hWndEdit, NULL, TRUE);
  AE_StackUpdateClones(ae);
  return nScrolled;
}

BOOL AE_StackFoldIsValid(AKELEDIT *ae, AEFOLD *lpFold)
{
  AEFOLD *lpElement;

  //Get root fold
  if (IsBadCodePtr((FARPROC)lpFold))
    return FALSE;

  while (lpFold->parent)
  {
    if (IsBadCodePtr((FARPROC)lpFold->parent))
      return FALSE;
    lpFold=lpFold->parent;
  }

  //Find root fold
  for (lpElement=ae->ptxt->hFoldsStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement == lpFold)
      return TRUE;
  }
  return FALSE;
}

BOOL AE_StackFoldDelete(AKELEDIT *ae, AEFOLD *lpFold)
{
  AEFOLD *lpElement;
  AEFOLD **lppFirstChild;
  AEFOLD **lppLastChild;
  BOOL bCollapse=lpFold->bCollapse;

  if (lpFold == ae->ptxt->lpIsCollapsedLastCall)
    ae->ptxt->lpIsCollapsedLastCall=NULL;
  ae->ptxt->lpVPosFold=NULL;

  //Change parent for childrens
  for (lpElement=lpFold->firstChild; lpElement; lpElement=lpElement->next)
  {
    lpElement->parent=lpFold->parent;
  }

  //Move childrens up to parent level
  if (!lpFold->parent)
  {
    lppFirstChild=&ae->ptxt->hFoldsStack.first;
    lppLastChild=&ae->ptxt->hFoldsStack.last;
  }
  else
  {
    lppFirstChild=&lpFold->parent->firstChild;
    lppLastChild=&lpFold->parent->lastChild;
  }
  StackJoin((stack **)lppFirstChild, (stack **)lppLastChild, (stack *)lpFold, (stack *)lpFold->firstChild, (stack *)lpFold->lastChild);

  AE_StackPointDelete(ae, lpFold->lpMinPoint);
  AE_StackPointDelete(ae, lpFold->lpMaxPoint);
  AE_HeapStackDelete(NULL, (stack **)lppFirstChild, (stack **)lppLastChild, (stack *)lpFold);
  --ae->ptxt->nFoldCount;
  return bCollapse;
}

int AE_StackFoldFree(AKELEDIT *ae)
{
  AEFOLD *lpParent=NULL;
  AEFOLD *lpSubling=ae->ptxt->hFoldsStack.first;
  AEFOLD *lpNextSubling;
  int nCollapse=0;

  ae->ptxt->lpVPosFold=NULL;
  ae->ptxt->lpIsCollapsedLastCall=NULL;

  while (lpSubling)
  {
    NextParent:
    if (lpSubling->firstChild)
    {
      lpSubling=lpSubling->firstChild;
      continue;
    }

    //Fold doesn't have childrens
    lpParent=lpSubling->parent;
    lpNextSubling=lpSubling->next;

    //Delete fold
    if (lpSubling->bCollapse) ++nCollapse;
    AE_StackPointDelete(ae, lpSubling->lpMinPoint);
    AE_StackPointDelete(ae, lpSubling->lpMaxPoint);
    if (!lpParent)
      AE_HeapStackDelete(NULL, (stack **)&ae->ptxt->hFoldsStack.first, (stack **)&ae->ptxt->hFoldsStack.last, (stack *)lpSubling);
    else
      AE_HeapStackDelete(NULL, (stack **)&lpParent->firstChild, (stack **)&lpParent->lastChild, (stack *)lpSubling);

    lpSubling=lpNextSubling;
  }
  if (lpParent)
  {
    lpSubling=lpParent;
    goto NextParent;
  }
  ae->ptxt->nFoldCount=0;

  return nCollapse;
}

int AE_VPos(AKELEDIT *ae, int nValue, DWORD dwFlags)
{
  AEFOLD *lpSubling=NULL;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  int nCurMinLine;
  int nCurMaxLine;
  int nLine=0;
  int nHiddenLines=0;

  if (dwFlags & AEVPF_VPOSFROMLINE)
    nLine=nValue;
  else if (dwFlags & AEVPF_LINEFROMVPOS)
    nLine=nValue / ae->ptxt->nCharHeight;
  else
    return -1;

  if (ae->ptxt->hFoldsStack.first)
  {
    dwFirst=mod(nLine - ae->ptxt->hFoldsStack.first->lpMinPoint->ciPoint.nLine);
    if (ae->ptxt->lpVPosFold)
      dwSecond=mod(nLine - ae->ptxt->lpVPosFold->lpMinPoint->ciPoint.nLine);

    if (dwFirst <= dwSecond)
    {
      lpSubling=ae->ptxt->hFoldsStack.first;
      nHiddenLines=0;
    }
    else if (dwSecond <= dwFirst)
    {
      lpSubling=ae->ptxt->lpVPosFold;
      nHiddenLines=ae->ptxt->nVPosFoldHiddenLines;
      if (dwFlags & AEVPF_LINEFROMVPOS)
        nLine+=ae->ptxt->nVPosFoldHiddenLines;
    }

    if (nLine >= lpSubling->lpMinPoint->ciPoint.nLine)
    {
      while (lpSubling)
      {
        if (nLine < lpSubling->lpMinPoint->ciPoint.nLine)
          break;
        ae->ptxt->lpVPosFold=lpSubling;
        ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;

        if (lpSubling->bCollapse)
        {
          //Get collapsible range
          nCurMinLine=AE_FirstCollapsibleLine(ae, lpSubling);
          nCurMaxLine=AE_LastCollapsibleLine(ae, lpSubling);
          if (nLine < nCurMinLine) break;

          if (nCurMinLine <= nCurMaxLine)
          {
            //Hidden count
            if (nLine <= nCurMaxLine && (dwFlags & AEVPF_VPOSFROMLINE))
            {
              nHiddenLines+=nLine - nCurMinLine + 1;
              break;
            }
            else
            {
              nHiddenLines+=nCurMaxLine - nCurMinLine + 1;
              if (dwFlags & AEVPF_LINEFROMVPOS)
                nLine+=nCurMaxLine - nCurMinLine + 1;
            }
          }
          lpSubling=AE_NextFold(lpSubling, FALSE);
          continue;
        }
        lpSubling=AE_NextFold(lpSubling, TRUE);
      }
    }
    else
    {
      lpSubling=AE_PrevFold(lpSubling, FALSE);

      while (lpSubling)
      {
        if (nLine > lpSubling->lpMaxPoint->ciPoint.nLine)
          break;

        if (lpSubling->bCollapse)
        {
          //Get collapsible range
          nCurMinLine=AE_FirstCollapsibleLine(ae, lpSubling);
          nCurMaxLine=AE_LastCollapsibleLine(ae, lpSubling);
          if (nLine > nCurMaxLine) break;

          if (nCurMinLine <= nCurMaxLine)
          {
            //Hidden count
            if (nLine >= nCurMinLine && (dwFlags & AEVPF_VPOSFROMLINE))
            {
              nHiddenLines-=nCurMaxLine - nLine + 1;
              break;
            }
            else
            {
              nHiddenLines-=nCurMaxLine - nCurMinLine + 1;
              if (dwFlags & AEVPF_LINEFROMVPOS)
                nLine-=nCurMaxLine - nCurMinLine + 1;
            }
          }
          ae->ptxt->lpVPosFold=lpSubling;
          ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;
          lpSubling=AE_PrevFold(lpSubling, FALSE);
          continue;
        }
        if (!lpSubling->firstChild)
        {
          ae->ptxt->lpVPosFold=lpSubling;
          ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;
        }
        lpSubling=AE_PrevFold(lpSubling, TRUE);
      }
    }
  }
  if (dwFlags & AEVPF_LINEFROMVPOS)
    return nLine;
  if (dwFlags & AEVPF_VPOSFROMLINE)
    return (nLine - nHiddenLines) * ae->ptxt->nCharHeight;
  return -1;
}

AEPOINT* AE_StackPointInsert(AKELEDIT *ae, AECHARINDEX *ciPoint)
{
  AEPOINT *lpElement=(AEPOINT *)ae->ptxt->hPointsStack.last;
  AEPOINT *lpNewElement=NULL;

  while (lpElement)
  {
    if (ciPoint->nLine > lpElement->ciPoint.nLine)
      break;
    if (ciPoint->nLine == lpElement->ciPoint.nLine && ciPoint->nCharInLine >= lpElement->ciPoint.nCharInLine)
      break;

    lpElement=lpElement->prev;
  }
  AE_HeapStackInsertAfter(NULL, (stack **)&ae->ptxt->hPointsStack.first, (stack **)&ae->ptxt->hPointsStack.last, (stack *)lpElement, (stack **)&lpNewElement, sizeof(AEPOINT));

  if (lpNewElement)
  {
    lpNewElement->ciPoint=*ciPoint;
    lpNewElement->nPointOffset=AEPTO_IGNORE;
    lpNewElement->nPointLen=0;
    lpNewElement->nReserved=AEPTO_IGNORE;
  }
  return lpNewElement;
}

void AE_StackPointUnset(AKELEDIT *ae, DWORD dwFlags)
{
  AEPOINT *lpElement=(AEPOINT *)ae->ptxt->hPointsStack.first;

  while (lpElement)
  {
    lpElement->dwFlags&=~dwFlags;

    lpElement=lpElement->next;
  }
}

void AE_StackPointUnreserve(AKELEDIT *ae)
{
  AEPOINT *lpElement=(AEPOINT *)ae->ptxt->hPointsStack.first;

  while (lpElement)
  {
    if (lpElement->nReserved >= 0)
    {
      lpElement->nPointOffset=lpElement->nReserved;
      lpElement->nReserved=AEPTO_IGNORE;
    }
    lpElement=lpElement->next;
  }
}

void AE_StackPointReset(AKELEDIT *ae, DWORD dwType)
{
  AEPOINT *lpElement=(AEPOINT *)ae->ptxt->hPointsStack.first;

  while (lpElement)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &lpElement->ciPoint, FALSE);
    if (lpElement->nPointOffset >= 0)
    {
      lpElement->nPointOffset=0;
      lpElement->nPointLen=0;
      lpElement->nReserved=AEPTO_IGNORE;
    }
    lpElement->dwFlags|=AEPTF_MOVED|AEPTF_MODIFY|AEPTF_DELETE;

    lpElement=lpElement->next;
  }
  AE_NotifyPoint(ae, dwType, NULL);
}

void AE_StackPointDelete(AKELEDIT *ae, AEPOINT *lpElement)
{
  AE_HeapStackDelete(NULL, (stack **)&ae->ptxt->hPointsStack.first, (stack **)&ae->ptxt->hPointsStack.last, (stack *)lpElement);
}

void AE_StackPointFree(AKELEDIT *ae)
{
  AE_HeapStackClear(NULL, (stack **)&ae->ptxt->hPointsStack.first, (stack **)&ae->ptxt->hPointsStack.last);
}

AEUNDOITEM* AE_StackUndoItemInsert(AKELEDIT *ae)
{
  AEUNDOITEM *lpElement=NULL;

  if (AE_EditCanRedo(ae))
    AE_StackRedoDeleteAll(ae, ae->ptxt->lpCurrentUndo);

  if (!AE_HeapStackInsertIndex(ae, (stack **)&ae->ptxt->hUndoStack.first, (stack **)&ae->ptxt->hUndoStack.last, (stack **)&lpElement, -1, sizeof(AEUNDOITEM)))
    ae->ptxt->lpCurrentUndo=lpElement;
  return lpElement;
}

void AE_StackUndoItemDelete(AKELEDIT *ae, AEUNDOITEM *lpItem)
{
  if (lpItem == ae->ptxt->lpSavePoint)
  {
    ae->ptxt->lpSavePoint=NULL;
    ae->ptxt->bSavePointExist=FALSE;
  }
  if (lpItem == ae->ptxt->lpCurrentUndo)
    ae->ptxt->lpCurrentUndo=NULL;

  if (lpItem->wpText) AE_HeapFree(ae, 0, (LPVOID)lpItem->wpText);
  AE_HeapStackDelete(ae, (stack **)&ae->ptxt->hUndoStack.first, (stack **)&ae->ptxt->hUndoStack.last, (stack *)lpItem);
}

void AE_StackRedoDeleteAll(AKELEDIT *ae, AEUNDOITEM *lpItem)
{
  AEUNDOITEM *lpElement=lpItem;
  AEUNDOITEM *lpTmp;

  if (!lpElement)
    lpElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.first;
  else
    lpElement=lpElement->next;

  while (lpElement)
  {
    if (lpElement->dwFlags & AEUN_STOPGROUP)
      --ae->ptxt->dwUndoCount;

    lpTmp=lpElement->next;
    AE_StackUndoItemDelete(ae, lpElement);
    lpElement=lpTmp;
  }
}

DWORD AE_StackUndoSize(AKELEDIT *ae)
{
  AEUNDOITEM *lpElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.first;
  DWORD dwSize=0;

  while (lpElement)
  {
    dwSize+=lpElement->dwTextLen;

    lpElement=lpElement->next;
  }
  return dwSize;
}

void AE_StackUndoGroupStop(AKELEDIT *ae)
{
  AEUNDOITEM *lpStopElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.last;

  if (lpStopElement)
  {
    if (!(lpStopElement->dwFlags & AEUN_STOPGROUP))
    {
      if (!ae->ptxt->bLockGroupStopInt && !ae->ptxt->bLockGroupStopExt)
      {
        //Clearing mutually exclusive actions after using ae->ptxt->bLockGroupStop
        AEUNDOITEM *lpElement=lpStopElement;
        AEUNDOITEM *lpExclusiveOneElement;
        AEUNDOITEM *lpExclusiveTwoElement;

        while (lpElement)
        {
          lpExclusiveOneElement=lpElement;
          lpExclusiveTwoElement=lpElement->prev;

          if (!lpExclusiveOneElement || (lpExclusiveOneElement->dwFlags & AEUN_STOPGROUP))
            break;
          if (!lpExclusiveTwoElement || (lpExclusiveTwoElement->dwFlags & AEUN_STOPGROUP))
            break;

          if ((lpExclusiveOneElement->dwFlags & AEUN_INSERT) &&
              (lpExclusiveTwoElement->dwFlags & AEUN_DELETE))
          {
            if (lpExclusiveOneElement->nActionStartOffset == lpExclusiveTwoElement->nActionStartOffset &&
                lpExclusiveOneElement->nActionEndOffset == lpExclusiveTwoElement->nActionEndOffset &&
                lpExclusiveOneElement->nExtraStartOffset == lpExclusiveTwoElement->nExtraStartOffset &&
                lpExclusiveOneElement->nExtraEndOffset == lpExclusiveTwoElement->nExtraEndOffset)
            {
              lpElement=lpExclusiveTwoElement->prev;
              AE_StackUndoItemDelete(ae, lpExclusiveOneElement);
              AE_StackUndoItemDelete(ae, lpExclusiveTwoElement);
              continue;
            }
          }
          lpElement=lpElement->prev;
        }
        lpStopElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.last;
        ae->ptxt->lpCurrentUndo=lpStopElement;
      }
    }
  }

  if (lpStopElement)
  {
    if (!(lpStopElement->dwFlags & AEUN_STOPGROUP))
    {
      if (lpStopElement->dwFlags & AEUN_SINGLECHAR)
      {
        //Merge typing characters to one undo action
        AEUNDOITEM *lpElement=lpStopElement;
        AEUNDOITEM *lpUndoElement;
        int nDeleteStart=0;
        int nDeleteEnd=0;
        wchar_t *wpUndoText;
        DWORD dwUndoTextLen=0;
        DWORD i;

        //Get count of typing characters
        while (lpElement)
        {
          if (lpElement->dwFlags & AEUN_SINGLECHAR)
            ++dwUndoTextLen;
          else
            break;

          lpElement=lpElement->prev;

          if (lpElement)
            if (lpElement->dwFlags & AEUN_STOPGROUP)
              break;
        }
        lpElement=lpStopElement;

        //Allocate string
        if (dwUndoTextLen)
        {
          if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
          {
            //Last index
            if (lpElement)
            {
              nDeleteEnd=lpElement->nActionEndOffset;
            }

            //Get string
            wpUndoText[dwUndoTextLen]=L'\0';
            i=dwUndoTextLen;

            while (lpElement)
            {
              if (lpElement->dwFlags & AEUN_SINGLECHAR)
              {
                if (i > 0)
                  wpUndoText[--i]=lpElement->wpText[0];
                else
                  break;
              }
              else break;

              lpElement=lpElement->prev;

              if (lpElement)
                if (lpElement->dwFlags & AEUN_STOPGROUP)
                  break;
            }
            ae->ptxt->lpCurrentUndo=lpElement;

            //First index
            if (!lpElement)
              lpElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.first;
            else
              lpElement=lpElement->next;
            if (lpElement) nDeleteStart=lpElement->nActionStartOffset;

            //Insert complite string (AE_StackUndoItemInsert will erase all forward AEUN_SINGLECHAR actions)
            if (lpUndoElement=AE_StackUndoItemInsert(ae))
            {
              lpUndoElement->dwFlags=AEUN_DELETE;
              lpUndoElement->nActionStartOffset=nDeleteStart;
              lpUndoElement->nActionEndOffset=nDeleteEnd;
              lpUndoElement->wpText=wpUndoText;
              lpUndoElement->dwTextLen=dwUndoTextLen;

              lpStopElement=lpUndoElement;
            }
          }
        }
      }
      if (!ae->ptxt->bLockGroupStopInt && !ae->ptxt->bLockGroupStopExt)
      {
        lpStopElement->dwFlags|=AEUN_STOPGROUP;
        ++ae->ptxt->dwUndoCount;
      }
    }

    //Undo limit is reached
    if (ae->ptxt->dwUndoCount > ae->ptxt->dwUndoLimit)
    {
      if (AE_GetModify(ae))
      {
        if (!ae->ptxt->lpSavePoint)
          ae->ptxt->bSavePointExist=FALSE;
      }

      while (ae->ptxt->dwUndoCount > ae->ptxt->dwUndoLimit)
      {
        AEUNDOITEM *lpElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.first;
        AEUNDOITEM *lpTmp;

        //Delete first undo group
        while (lpElement)
        {
          if (lpElement->dwFlags & AEUN_STOPGROUP)
          {
            AE_StackUndoItemDelete(ae, lpElement);
            break;
          }
          lpTmp=lpElement->next;
          AE_StackUndoItemDelete(ae, lpElement);
          lpElement=lpTmp;
        }
        --ae->ptxt->dwUndoCount;
      }
      if (!ae->ptxt->lpCurrentUndo)
        AE_EmptyUndoBuffer(ae);
    }
  }
}

AELINEDATA* AE_StackLineAdd(AKELEDIT *ae)
{
  AELINEDATA *lpElement=NULL;

  AE_HeapStackInsertIndex(ae, (stack **)&ae->ptxt->hLinesStack.first, (stack **)&ae->ptxt->hLinesStack.last, (stack **)&lpElement, -1, sizeof(AELINEDATA));
  return lpElement;
}

AELINEDATA* AE_StackLineInsertBefore(AKELEDIT *ae, AELINEDATA *lpLine)
{
  AELINEDATA *lpElement=NULL;

  AE_HeapStackInsertBefore(ae, (stack **)&ae->ptxt->hLinesStack.first, (stack **)&ae->ptxt->hLinesStack.last, (stack *)lpLine, (stack **)&lpElement, sizeof(AELINEDATA));
  return lpElement;
}

AELINEDATA* AE_StackLineInsertAfter(AKELEDIT *ae, AELINEDATA *lpLine)
{
  AELINEDATA *lpElement=NULL;

  AE_HeapStackInsertAfter(ae, (stack **)&ae->ptxt->hLinesStack.first, (stack **)&ae->ptxt->hLinesStack.last, (stack *)lpLine, (stack **)&lpElement, sizeof(AELINEDATA));
  return lpElement;
}

void AE_StackLineDelete(AKELEDIT *ae, AELINEDATA *lpElement)
{
  if (lpElement == ae->liFirstDrawLine.lpLine)
  {
    ae->liFirstDrawLine.nLine=0;
    ae->liFirstDrawLine.lpLine=NULL;
    ae->nFirstDrawLineOffset=0;
  }
  if (lpElement == ae->ptxt->liMaxWidthLine.lpLine)
  {
    ae->ptxt->liMaxWidthLine.nLine=0;
    ae->ptxt->liMaxWidthLine.lpLine=NULL;
  }
  if (lpElement == ae->ptxt->liLineUnwrapLastCall.lpLine)
  {
    ae->ptxt->liLineUnwrapLastCall.nLine=0;
    ae->ptxt->liLineUnwrapLastCall.lpLine=NULL;
    ae->ptxt->nLineUnwrapLastCall=0;
  }
  if (lpElement == ae->ciCaretIndex.lpLine)
  {
    ae->ptCaret.x=0;
    ae->ptCaret.y=0;
    ae->ciCaretIndex.lpLine=NULL;
  }
  if (lpElement == ae->ciSelStartIndex.lpLine)
  {
    ae->ciSelStartIndex.lpLine=NULL;
    ae->nSelStartCharOffset=0;
  }
  if (lpElement == ae->ciSelEndIndex.lpLine)
  {
    ae->ciSelEndIndex.lpLine=NULL;
    ae->nSelEndCharOffset=0;
  }
  if (lpElement == ae->ciLastCallIndex.lpLine)
  {
    ae->ciLastCallIndex.lpLine=NULL;
    ae->nLastCallOffset=0;
  }
  if (lpElement->wpLine) AE_HeapFree(ae, 0, (LPVOID)lpElement->wpLine);
  AE_HeapStackDelete(ae, (stack **)&ae->ptxt->hLinesStack.first, (stack **)&ae->ptxt->hLinesStack.last, (stack *)lpElement);
}

void AE_StackLineFree(AKELEDIT *ae)
{
  AELINEDATA *lpElement=(AELINEDATA *)ae->ptxt->hLinesStack.first;
  AELINEDATA *lpTmp;

  while (lpElement)
  {
    lpTmp=lpElement->next;
    AE_StackLineDelete(ae, lpElement);
    lpElement=lpTmp;
  }
}

AELINEDATA* AE_GetLineData(AKELEDIT *ae, int nLine)
{
  AECHARINDEX ciElement;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  int nElementLineOffset=0;
  int nCompare;

  if (nLine < 0) nLine=ae->ptxt->nLineCount + nLine + 1;
  if (nLine < 0 || nLine > ae->ptxt->nLineCount) return NULL;

  //Find nearest element in stack
  dwFirst=mod(nLine - 0);
  dwSecond=mod(nLine - ae->ptxt->nLineCount);
  if (ae->liFirstDrawLine.lpLine)
    dwThird=mod(nLine - ae->liFirstDrawLine.nLine);
  if (ae->ciSelStartIndex.lpLine)
    dwFourth=mod(nLine - ae->ciSelStartIndex.nLine);
  if (ae->ciSelEndIndex.lpLine)
    dwFifth=mod(nLine - ae->ciSelEndIndex.nLine);
  if (ae->ciLastCallIndex.lpLine)
    dwSixth=mod(nLine - ae->ciLastCallIndex.nLine);

  if (dwFirst <= dwSecond && dwFirst <= dwThird && dwFirst <= dwFourth && dwFirst <= dwFifth && dwFirst <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciElement, FALSE);
    nElementLineOffset=0;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciElement, FALSE);
    nElementLineOffset=ae->ptxt->nLastCharOffset;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth && dwThird <= dwFifth && dwThird <= dwSixth)
  {
    ciElement.nLine=ae->liFirstDrawLine.nLine;
    ciElement.lpLine=ae->liFirstDrawLine.lpLine;
    ciElement.nCharInLine=0;
    nElementLineOffset=ae->nFirstDrawLineOffset;
  }
  else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird && dwFourth <= dwFifth && dwFourth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelStartIndex.nLine;
    ciElement.lpLine=ae->ciSelStartIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelStartIndex.nCharInLine, ae->ciSelStartIndex.lpLine->nLineLen);
    nElementLineOffset=ae->nSelStartCharOffset;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelEndIndex.nLine;
    ciElement.lpLine=ae->ciSelEndIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelEndIndex.nCharInLine, ae->ciSelEndIndex.lpLine->nLineLen);
    nElementLineOffset=ae->nSelEndCharOffset;
  }
  else if (dwSixth <= dwFirst && dwSixth <= dwSecond && dwSixth <= dwThird && dwSixth <= dwFourth && dwSixth <= dwFifth)
  {
    ciElement=ae->ciLastCallIndex;
    nElementLineOffset=ae->nLastCallOffset;
  }
  nCompare=nLine - ciElement.nLine;

  if (nCompare == 0)
  {
    nElementLineOffset-=ciElement.nCharInLine;
    ciElement.nCharInLine=0;
  }
  else if (nCompare > 0)
  {
    nElementLineOffset-=ciElement.nCharInLine;
    ciElement.nCharInLine=0;

    while (ciElement.lpLine)
    {
      if (ciElement.nLine == nLine)
        break;

      nElementLineOffset+=ciElement.lpLine->nLineLen;

      //Next line
      if (ciElement.lpLine->nLineBreak != AELB_WRAP && ciElement.lpLine->nLineBreak != AELB_EOF)
        nElementLineOffset+=1;

      ciElement.nLine+=1;
      ciElement.lpLine=ciElement.lpLine->next;
    }
  }
  else if (nCompare < 0)
  {
    nElementLineOffset+=ciElement.lpLine->nLineLen - ciElement.nCharInLine;
    ciElement.nCharInLine=0;

    while (ciElement.lpLine)
    {
      nElementLineOffset-=ciElement.lpLine->nLineLen;

      if (ciElement.nLine == nLine)
        break;

      //Previous line
      ciElement.nLine-=1;
      ciElement.lpLine=ciElement.lpLine->prev;

      if (ciElement.lpLine && ciElement.lpLine->nLineBreak != AELB_WRAP && ciElement.lpLine->nLineBreak != AELB_EOF)
        nElementLineOffset-=1;
    }
  }

  if (ae->ciLastCallIndex.nLine != ciElement.nLine)
  {
    ae->ciLastCallIndex=ciElement;
    ae->nLastCallOffset=nElementLineOffset;
  }
  return ciElement.lpLine;
}

int AE_GetWrapLine(AKELEDIT *ae, int nLine, AECHARINDEX *ciCharOut)
{
  AELINEINDEX liLine;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nElementLine=0;

  if (nLine < 0) nLine=ae->ptxt->nLineUnwrapCount + nLine + 1;
  if (nLine < 0 || nLine > ae->ptxt->nLineUnwrapCount) return -1;
  if (!ae->ptxt->dwWordWrap) return nLine;

  //Find nearest element in stack
  dwFirst=mod(nLine - 0);
  dwSecond=mod(nLine - ae->ptxt->nLineUnwrapCount);
  if (ae->ptxt->liLineUnwrapLastCall.lpLine)
    dwThird=mod(nLine - ae->ptxt->nLineUnwrapLastCall);

  if (dwFirst <= dwSecond && dwFirst <= dwThird)
  {
    nElementLine=nLine - 0;
    liLine.nLine=0;
    liLine.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird)
  {
    nElementLine=nLine - ae->ptxt->nLineUnwrapCount;
    liLine.nLine=ae->ptxt->nLineCount;
    liLine.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.last;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond)
  {
    nElementLine=nLine - ae->ptxt->nLineUnwrapLastCall;
    liLine.nLine=ae->ptxt->liLineUnwrapLastCall.nLine;
    liLine.lpLine=ae->ptxt->liLineUnwrapLastCall.lpLine;
  }

  if (nElementLine > 0)
  {
    while (liLine.lpLine)
    {
      if (liLine.lpLine->nLineBreak != AELB_WRAP)
      {
        if (--nElementLine == 0)
        {
          ++liLine.nLine;
          liLine.lpLine=liLine.lpLine->next;
          break;
        }
      }
      ++liLine.nLine;
      liLine.lpLine=liLine.lpLine->next;
    }
  }
  else if (nElementLine <= 0)
  {
    if (liLine.lpLine)
    {
      while (liLine.lpLine->prev)
      {
        if (liLine.lpLine->prev->nLineBreak != AELB_WRAP)
        {
          if (nElementLine++ == 0)
            break;
        }
        --liLine.nLine;
        liLine.lpLine=liLine.lpLine->prev;
      }
    }
  }

  if (liLine.lpLine)
  {
    if (ciCharOut)
    {
      ciCharOut->nLine=liLine.nLine;
      ciCharOut->lpLine=liLine.lpLine;
      ciCharOut->nCharInLine=0;
    }
    return liLine.nLine;
  }
  else return -1;
}

int AE_GetUnwrapLine(AKELEDIT *ae, int nLine)
{
  AELINEINDEX liLine;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nUnwrappedLine=0;
  int nElementLine=0;

  if (nLine < 0) nLine=ae->ptxt->nLineCount + nLine + 1;
  if (nLine < 0 || nLine > ae->ptxt->nLineCount) return -1;
  if (!ae->ptxt->dwWordWrap) return nLine;

  //Find nearest element in stack
  dwFirst=mod(nLine - 0);
  dwSecond=mod(nLine - ae->ptxt->nLineCount);
  if (ae->ptxt->liLineUnwrapLastCall.lpLine)
    dwThird=mod(nLine - ae->ptxt->liLineUnwrapLastCall.nLine);

  if (dwFirst <= dwSecond && dwFirst <= dwThird)
  {
    nUnwrappedLine=0;
    nElementLine=nLine - 0;
    liLine.nLine=0;
    liLine.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird)
  {
    nUnwrappedLine=ae->ptxt->nLineUnwrapCount;
    nElementLine=nLine - ae->ptxt->nLineCount;
    liLine.nLine=ae->ptxt->nLineCount;
    liLine.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.last;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond)
  {
    nUnwrappedLine=ae->ptxt->nLineUnwrapLastCall;
    nElementLine=nLine - ae->ptxt->liLineUnwrapLastCall.nLine;
    liLine.nLine=ae->ptxt->liLineUnwrapLastCall.nLine;
    liLine.lpLine=ae->ptxt->liLineUnwrapLastCall.lpLine;
  }

  if (nElementLine > 0)
  {
    while (liLine.lpLine)
    {
      if (nElementLine-- == 0)
        break;
      if (liLine.lpLine->nLineBreak != AELB_WRAP)
        ++nUnwrappedLine;

      ++liLine.nLine;
      liLine.lpLine=liLine.lpLine->next;
    }
  }
  else if (nElementLine < 0)
  {
    if (liLine.lpLine)
    {
      while (liLine.lpLine->prev)
      {
        if (nElementLine++ == 0)
          break;
        if (liLine.lpLine->prev->nLineBreak != AELB_WRAP)
          --nUnwrappedLine;

        --liLine.nLine;
        liLine.lpLine=liLine.lpLine->prev;
      }
    }
  }

  if (liLine.lpLine)
  {
    ae->ptxt->liLineUnwrapLastCall=liLine;
    ae->ptxt->nLineUnwrapLastCall=nUnwrappedLine;
    return nUnwrappedLine;
  }
  else return -1;
}

void AE_RichOffsetToAkelIndex(AKELEDIT *ae, DWORD dwOffset, AECHARINDEX *ciCharIndex)
{
  AECHARINDEX ciElement;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  int nElementLineOffset=0;

  dwOffset=min(dwOffset, (DWORD)ae->ptxt->nLastCharOffset);

  //Find nearest element in stack
  dwFirst=mod(dwOffset - 0);
  dwSecond=mod(dwOffset - ae->ptxt->nLastCharOffset);
  if (ae->liFirstDrawLine.lpLine && ae->nFirstDrawLineOffset)
    dwThird=mod(dwOffset - ae->nFirstDrawLineOffset);
  if (ae->ciSelStartIndex.lpLine && ae->nSelStartCharOffset)
    dwFourth=mod(dwOffset - ae->nSelStartCharOffset);
  if (ae->ciSelEndIndex.lpLine && ae->nSelEndCharOffset)
    dwFifth=mod(dwOffset - ae->nSelEndCharOffset);
  if (ae->ciLastCallIndex.lpLine && ae->nLastCallOffset)
    dwSixth=mod(dwOffset - ae->nLastCallOffset);

  if (dwFirst <= dwSecond && dwFirst <= dwThird && dwFirst <= dwFourth && dwFirst <= dwFifth && dwFirst <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciElement, FALSE);
    nElementLineOffset=dwOffset - 0;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciElement, FALSE);
    nElementLineOffset=dwOffset - ae->ptxt->nLastCharOffset;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth && dwThird <= dwFifth && dwThird <= dwSixth)
  {
    ciElement.nLine=ae->liFirstDrawLine.nLine;
    ciElement.lpLine=ae->liFirstDrawLine.lpLine;
    ciElement.nCharInLine=0;
    nElementLineOffset=dwOffset - ae->nFirstDrawLineOffset;
  }
  else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird && dwFourth <= dwFifth && dwFourth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelStartIndex.nLine;
    ciElement.lpLine=ae->ciSelStartIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelStartIndex.nCharInLine, ae->ciSelStartIndex.lpLine->nLineLen);
    nElementLineOffset=dwOffset - ae->nSelStartCharOffset;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelEndIndex.nLine;
    ciElement.lpLine=ae->ciSelEndIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelEndIndex.nCharInLine, ae->ciSelEndIndex.lpLine->nLineLen);
    nElementLineOffset=dwOffset - ae->nSelEndCharOffset;
  }
  else if (dwSixth <= dwFirst && dwSixth <= dwSecond && dwSixth <= dwThird && dwSixth <= dwFourth && dwSixth <= dwFifth)
  {
    ciElement=ae->ciLastCallIndex;
    nElementLineOffset=dwOffset - ae->nLastCallOffset;
  }

  AE_IndexOffset(ae, &ciElement, &ciElement, nElementLineOffset, AELB_R);

  if (nElementLineOffset)
  {
    ae->ciLastCallIndex=ciElement;
    ae->nLastCallOffset=dwOffset;
  }
  ciCharIndex->nLine=ciElement.nLine;
  ciCharIndex->lpLine=ciElement.lpLine;
  ciCharIndex->nCharInLine=ciElement.nCharInLine;
}

int AE_AkelIndexToRichOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIndex)
{
  AECHARINDEX ciChar=*ciCharIndex;
  AECHARINDEX ciElement;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  int nElementLineOffset=0;
  int nResult;

  //Normalize
  ciChar.nCharInLine=min(ciChar.nCharInLine, ciChar.lpLine->nLineLen);

  if (ciChar.nLine < 0)
    return 0;
  if (ciChar.nLine > ae->ptxt->nLineCount)
    return ae->ptxt->nLastCharOffset;

  //Find nearest element in stack
  dwFirst=mod(ciChar.nLine - 0);
  dwSecond=mod(ciChar.nLine - ae->ptxt->nLineCount);
  if (ae->liFirstDrawLine.lpLine && ae->nFirstDrawLineOffset)
    dwThird=mod(ciChar.nLine - ae->liFirstDrawLine.nLine);
  if (ae->ciSelStartIndex.lpLine && ae->nSelStartCharOffset)
    dwFourth=mod(ciChar.nLine - ae->ciSelStartIndex.nLine);
  if (ae->ciSelEndIndex.lpLine && ae->nSelEndCharOffset)
    dwFifth=mod(ciChar.nLine - ae->ciSelEndIndex.nLine);
  if (ae->ciLastCallIndex.lpLine && ae->nLastCallOffset)
    dwSixth=mod(ciChar.nLine - ae->ciLastCallIndex.nLine);

  if (dwFirst <= dwSecond && dwFirst <= dwThird && dwFirst <= dwFourth && dwFirst <= dwFifth && dwFirst <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciElement, FALSE);
    nElementLineOffset=0;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciElement, FALSE);
    nElementLineOffset=ae->ptxt->nLastCharOffset;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth && dwThird <= dwFifth && dwThird <= dwSixth)
  {
    ciElement.nLine=ae->liFirstDrawLine.nLine;
    ciElement.lpLine=ae->liFirstDrawLine.lpLine;
    ciElement.nCharInLine=0;
    nElementLineOffset=ae->nFirstDrawLineOffset;
  }
  else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird && dwFourth <= dwFifth && dwFourth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelStartIndex.nLine;
    ciElement.lpLine=ae->ciSelStartIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelStartIndex.nCharInLine, ae->ciSelStartIndex.lpLine->nLineLen);
    nElementLineOffset=ae->nSelStartCharOffset;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelEndIndex.nLine;
    ciElement.lpLine=ae->ciSelEndIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelEndIndex.nCharInLine, ae->ciSelEndIndex.lpLine->nLineLen);
    nElementLineOffset=ae->nSelEndCharOffset;
  }
  else if (dwSixth <= dwFirst && dwSixth <= dwSecond && dwSixth <= dwThird && dwSixth <= dwFourth && dwSixth <= dwFifth)
  {
    ciElement=ae->ciLastCallIndex;
    nElementLineOffset=ae->nLastCallOffset;
  }

  if (AE_IndexCompare(&ciChar, &ciElement))
  {
    nResult=nElementLineOffset + AE_IndexSubtract(ae, &ciChar, &ciElement, AELB_R, FALSE, FALSE);
  }
  else return nElementLineOffset;

  ae->ciLastCallIndex=ciChar;
  ae->nLastCallOffset=nResult;
  return nResult;
}

AELINEDATA* AE_GetIndex(AKELEDIT *ae, int nType, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, BOOL bColumnSel)
{
  if (nType == AEGI_FIRSTCHAR)
  {
    ciCharOut->nLine=0;
    ciCharOut->lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
    ciCharOut->nCharInLine=0;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_LASTCHAR)
  {
    ciCharOut->nLine=ae->ptxt->nLineCount;
    ciCharOut->lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.last;
    ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_FIRSTSELCHAR)
  {
    if (ae->bColumnSel)
    {
      ciCharOut->nLine=ae->ciSelStartIndex.nLine;
      ciCharOut->lpLine=ae->ciSelStartIndex.lpLine;
      ciCharOut->nCharInLine=ae->ciSelStartIndex.lpLine->nSelStart;
    }
    else *ciCharOut=ae->ciSelStartIndex;

    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_LASTSELCHAR)
  {
    if (ae->bColumnSel)
    {
      ciCharOut->nLine=ae->ciSelEndIndex.nLine;
      ciCharOut->lpLine=ae->ciSelEndIndex.lpLine;
      ciCharOut->nCharInLine=ae->ciSelStartIndex.lpLine->nSelEnd;
    }
    else *ciCharOut=ae->ciSelEndIndex;

    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_CARETCHAR)
  {
    *ciCharOut=ae->ciCaretIndex;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_NEXTCHAR ||
           nType == AEGI_NEXTVISIBLECHAR)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    if (bColumnSel)
    {
      AE_IndexInc(&ciCharTmp);
      *ciCharOut=ciCharTmp;
      return ciCharOut->lpLine;
    }
    else
    {
      AE_NextChar(&ciCharTmp);

      if (nType == AEGI_NEXTVISIBLECHAR)
      {
        do
        {
          if (!AE_StackIsLineCollapsed(ae, ciCharTmp.nLine))
            break;
        }
        while (AE_NextLine(&ciCharTmp));
      }

      if (ciCharTmp.lpLine)
      {
        *ciCharOut=ciCharTmp;
        return ciCharOut->lpLine;
      }
      else
      {
        *ciCharOut=*ciCharIn;
        return NULL;
      }
    }
  }
  else if (nType == AEGI_PREVCHAR ||
           nType == AEGI_PREVVISIBLECHAR)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    if (bColumnSel)
    {
      AE_IndexDec(&ciCharTmp);
      *ciCharOut=ciCharTmp;
      ciCharOut->nCharInLine=max(ciCharTmp.nCharInLine, 0);
      return ciCharOut->lpLine;
    }
    else
    {
      AE_PrevChar(&ciCharTmp);

      if (nType == AEGI_PREVVISIBLECHAR)
      {
        do
        {
          if (!AE_StackIsLineCollapsed(ae, ciCharTmp.nLine))
            break;
        }
        while (AE_PrevLine(&ciCharTmp));
      }

      if (ciCharTmp.lpLine)
      {
        *ciCharOut=ciCharTmp;
        return ciCharOut->lpLine;
      }
      else
      {
        *ciCharOut=*ciCharIn;
        return NULL;
      }
    }
  }
  else if (nType == AEGI_NEXTLINE ||
           nType == AEGI_NEXTVISIBLELINE)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    while (AE_NextLine(&ciCharTmp))
    {
      if (nType == AEGI_NEXTVISIBLELINE)
      {
        if (!AE_StackIsLineCollapsed(ae, ciCharTmp.nLine))
          break;
      }
      else break;
    }

    if (ciCharTmp.lpLine)
    {
      *ciCharOut=ciCharTmp;
      return ciCharOut->lpLine;
    }
    else
    {
      *ciCharOut=*ciCharIn;
      return NULL;
    }
  }
  else if (nType == AEGI_PREVLINE ||
           nType == AEGI_PREVVISIBLELINE)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    while (AE_PrevLine(&ciCharTmp))
    {
      if (nType == AEGI_PREVVISIBLELINE)
      {
        if (!AE_StackIsLineCollapsed(ae, ciCharTmp.nLine))
          break;
      }
      else break;
    }

    if (ciCharTmp.lpLine)
    {
      *ciCharOut=ciCharTmp;
      return ciCharOut->lpLine;
    }
    else
    {
      *ciCharOut=*ciCharIn;
      return NULL;
    }
  }
  else if (nType == AEGI_FIRSTVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetFirstVisibleLine(ae);
    ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=0;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_LASTVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetLastVisibleLine(ae);
    ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_FIRSTFULLVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetFirstFullVisibleLine(ae);
    ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=0;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_LASTFULLVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetLastFullVisibleLine(ae);
    ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
    return ciCharOut->lpLine;
  }
  else if (nType == AEGI_WRAPLINEBEGIN)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;
    int nCount=ciCharTmp.nCharInLine;

    if (ciCharTmp.lpLine)
    {
      while (ciCharTmp.lpLine->prev)
      {
        if (ciCharTmp.lpLine->prev->nLineBreak != AELB_WRAP)
          break;

        --ciCharTmp.nLine;
        ciCharTmp.lpLine=ciCharTmp.lpLine->prev;
        nCount+=ciCharTmp.lpLine->nLineLen;
      }
    }
    ciCharOut->nLine=ciCharTmp.nLine;
    ciCharOut->lpLine=ciCharTmp.lpLine;
    ciCharOut->nCharInLine=0;
    return (AELINEDATA *)nCount;
  }
  else if (nType == AEGI_WRAPLINEEND)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;
    int nCount=ciCharTmp.lpLine->nLineLen - ciCharTmp.nCharInLine;

    while (ciCharTmp.lpLine)
    {
      if (ciCharTmp.lpLine->nLineBreak != AELB_WRAP)
        break;

      ++ciCharTmp.nLine;
      ciCharTmp.lpLine=ciCharTmp.lpLine->next;
      nCount+=ciCharTmp.lpLine->nLineLen;
    }
    ciCharOut->nLine=ciCharTmp.nLine;
    ciCharOut->lpLine=ciCharTmp.lpLine;
    ciCharOut->nCharInLine=ciCharTmp.lpLine->nLineLen;
    return (AELINEDATA *)nCount;
  }
  else if (nType == AEGI_NEXTCHARINLINE)
  {
    return AE_NextCharInLineEx(ciCharIn, ciCharOut);
  }
  else if (nType == AEGI_PREVCHARINLINE)
  {
    return AE_PrevCharInLineEx(ciCharIn, ciCharOut);
  }
  return NULL;
}

int AE_CopyChar(wchar_t *wszTarget, DWORD dwTargetSize, const wchar_t *wpSource)
{
  if (AE_IsSurrogate(*wpSource))
  {
    if (dwTargetSize >= 2)
    {
      if (AE_IsHighSurrogate(*wpSource) && AE_IsLowSurrogate(*(wpSource + 1)))
      {
        if (wszTarget)
        {
          *wszTarget=*wpSource;
          *(wszTarget + 1)=*(wpSource + 1);
        }
        return 2;
      }
    }
  }
  else
  {
    if (wszTarget) *wszTarget=*wpSource;
    return 1;
  }
  return 0;
}

int AE_IndexInc(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine >= 0)
  {
    if (ciChar->nCharInLine + 1 < ciChar->lpLine->nLineLen)
    {
      if (AE_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine]))
        if (AE_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine + 1]))
          ++ciChar->nCharInLine;
    }
  }
  return ++ciChar->nCharInLine;
}

int AE_IndexDec(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine - 2 >= 0)
  {
    if (ciChar->nCharInLine - 1 < ciChar->lpLine->nLineLen)
    {
      if (AE_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine - 1]))
        if (AE_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine - 2]))
          --ciChar->nCharInLine;
    }
  }
  return --ciChar->nCharInLine;
}

int AE_IndexLen(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine >= 0)
  {
    if (ciChar->nCharInLine + 1 < ciChar->lpLine->nLineLen)
    {
      if (AE_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine]))
        if (AE_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine + 1]))
          return 2;
    }
  }
  return 1;
}

int AE_IndexCompare(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2)
{
  if (ciChar1->nLine == ciChar2->nLine &&
      ciChar1->nCharInLine == ciChar2->nCharInLine)
  {
    return 0;
  }
  if ((ciChar1->nLine < ciChar2->nLine) ||
      (ciChar1->nLine == ciChar2->nLine &&
       ciChar1->nCharInLine < ciChar2->nCharInLine))
  {
    return -1;
  }
  return 1;
}

int AE_IndexCompareEx(const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2)
{
  if ((ciChar1->nLine == ciChar2->nLine &&
       ciChar1->nCharInLine == ciChar2->nCharInLine) ||
      (ciChar1->lpLine && ciChar2->lpLine &&
        ((ciChar1->lpLine->next == ciChar2->lpLine &&
          ciChar1->lpLine->nLineBreak == AELB_WRAP &&
          ciChar1->nCharInLine == ciChar1->lpLine->nLineLen &&
          ciChar2->nCharInLine == 0) ||
         (ciChar2->lpLine->next == ciChar1->lpLine &&
          ciChar2->lpLine->nLineBreak == AELB_WRAP &&
          ciChar2->nCharInLine == ciChar2->lpLine->nLineLen &&
          ciChar1->nCharInLine == 0))))
  {
    return 0;
  }
  if ((ciChar1->nLine < ciChar2->nLine) ||
      (ciChar1->nLine == ciChar2->nLine &&
       ciChar1->nCharInLine < ciChar2->nCharInLine))
  {
    return -1;
  }
  return 1;
}

AELINEDATA* AE_NextLine(AECHARINDEX *ciChar)
{
  if (ciChar->lpLine)
  {
    ciChar->nLine+=1;
    ciChar->lpLine=ciChar->lpLine->next;
    ciChar->nCharInLine=0;
  }
  return ciChar->lpLine;
}

AELINEDATA* AE_PrevLine(AECHARINDEX *ciChar)
{
  if (ciChar->lpLine)
  {
    ciChar->nLine-=1;
    ciChar->lpLine=ciChar->lpLine->prev;
    if (ciChar->lpLine)
      ciChar->nCharInLine=ciChar->lpLine->nLineLen;
    else
      ciChar->nCharInLine=0;
  }
  return ciChar->lpLine;
}

AELINEDATA* AE_NextLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
{
  AECHARINDEX ciTmp=*ciIn;

  if (AE_NextLine(&ciTmp))
  {
    *ciOut=ciTmp;
    return ciOut->lpLine;
  }
  else
  {
    *ciOut=*ciIn;
    return NULL;
  }
}

AELINEDATA* AE_PrevLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
{
  AECHARINDEX ciTmp=*ciIn;

  if (AE_PrevLine(&ciTmp))
  {
    *ciOut=ciTmp;
    return ciOut->lpLine;
  }
  else
  {
    *ciOut=*ciIn;
    return NULL;
  }
}

AELINEDATA* AE_NextChar(AECHARINDEX *ciChar)
{
  AE_IndexInc(ciChar);

  if (ciChar->nCharInLine > ciChar->lpLine->nLineLen)
  {
    AE_NextLine(ciChar);

    if (ciChar->lpLine)
    {
      if (ciChar->lpLine->prev->nLineBreak == AELB_WRAP)
        AE_IndexInc(ciChar);
    }
  }
  return ciChar->lpLine;
}

AELINEDATA* AE_PrevChar(AECHARINDEX *ciChar)
{
  AE_IndexDec(ciChar);

  if (ciChar->nCharInLine < 0)
  {
    AE_PrevLine(ciChar);

    if (ciChar->lpLine)
    {
      if (ciChar->lpLine->nLineBreak == AELB_WRAP)
        AE_IndexDec(ciChar);
    }
  }
  return ciChar->lpLine;
}

AELINEDATA* AE_NextCharEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
{
  AECHARINDEX ciTmp=*ciIn;

  if (AE_NextChar(&ciTmp))
  {
    *ciOut=ciTmp;
    return ciOut->lpLine;
  }
  else
  {
    *ciOut=*ciIn;
    return NULL;
  }
}

AELINEDATA* AE_PrevCharEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
{
  AECHARINDEX ciTmp=*ciIn;

  if (AE_PrevChar(&ciTmp))
  {
    *ciOut=ciTmp;
    return ciOut->lpLine;
  }
  else
  {
    *ciOut=*ciIn;
    return NULL;
  }
}

AELINEDATA* AE_NextCharInLine(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen - 1)
  {
    if (ciChar->lpLine->nLineBreak != AELB_WRAP)
      return NULL;
  }
  AE_NextChar(ciChar);
  return ciChar->lpLine;
}

AELINEDATA* AE_PrevCharInLine(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine == 0)
  {
    if (!ciChar->lpLine->prev || ciChar->lpLine->prev->nLineBreak != AELB_WRAP)
      return NULL;
  }
  AE_PrevChar(ciChar);
  return ciChar->lpLine;
}

AELINEDATA* AE_NextCharInLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
{
  AECHARINDEX ciTmp=*ciIn;

  if (AE_NextCharInLine(&ciTmp))
  {
    *ciOut=ciTmp;
    return ciOut->lpLine;
  }
  else
  {
    *ciOut=*ciIn;
    return NULL;
  }
}

AELINEDATA* AE_PrevCharInLineEx(const AECHARINDEX *ciIn, AECHARINDEX *ciOut)
{
  AECHARINDEX ciTmp=*ciIn;

  if (AE_PrevCharInLine(&ciTmp))
  {
    *ciOut=ciTmp;
    return ciOut->lpLine;
  }
  else
  {
    *ciOut=*ciIn;
    return NULL;
  }
}

int AE_IndexSubtract(AKELEDIT *ae, const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  AECHARINDEX ciStart;
  AECHARINDEX ciEnd;
  AECHARINDEX ciTmp;
  AELINEDATA *lpElement;
  int nCount=0;
  int nCompare;
  int nLineBreak;

  if (bColumnSel == -1) bColumnSel=ae->bColumnSel;

  //Set new line
  if (nNewLine < AELB_ASIS)
  {
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->popt->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->popt->nOutputNewLine;
  }

  if (!ciChar1)
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciStart, FALSE);
  else
    ciStart=*ciChar1;

  if (!ciChar2)
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciEnd, FALSE);
  else
    ciEnd=*ciChar2;

  //Exchange indexes
  if ((nCompare=AE_IndexCompare(&ciStart, &ciEnd)) > 0)
  {
    ciTmp=ciStart;
    ciStart=ciEnd;
    ciEnd=ciTmp;
  }

  if (bColumnSel)
  {
    lpElement=ciStart.lpLine;

    while (lpElement)
    {
      if (bFillSpaces)
        nCount+=lpElement->nSelEnd - lpElement->nSelStart;
      else
        nCount+=min(lpElement->nSelEnd, lpElement->nLineLen) - min(lpElement->nSelStart, lpElement->nLineLen);

      if (lpElement == ciEnd.lpLine) break;

      if (lpElement->nLineBreak == AELB_WRAP)
      {
        if (nNewLine == AELB_ASIS)
          nLineBreak=AELB_RN;
        else
          nLineBreak=nNewLine;
      }
      else
      {
        if (nNewLine == AELB_ASIS)
          nLineBreak=lpElement->nLineBreak;
        else
          nLineBreak=nNewLine;
      }

      if (nLineBreak == AELB_R)
      {
        nCount+=1;
      }
      else if (nLineBreak == AELB_N)
      {
        nCount+=1;
      }
      else if (nLineBreak == AELB_RN)
      {
        nCount+=2;
      }
      else if (nLineBreak == AELB_RRN)
      {
        nCount+=3;
      }
      lpElement=lpElement->next;
    }
  }
  else
  {
    //Normalize
    ciStart.nCharInLine=min(ciStart.nCharInLine, ciStart.lpLine->nLineLen);
    ciEnd.nCharInLine=min(ciEnd.nCharInLine, ciEnd.lpLine->nLineLen);

    if (ciStart.nLine == ciEnd.nLine)
    {
      nCount=ciEnd.nCharInLine - ciStart.nCharInLine;
    }
    else
    {
      lpElement=ciStart.lpLine;

      while (lpElement)
      {
        if (lpElement == ciEnd.lpLine)
        {
          nCount+=ciEnd.nCharInLine;
          break;
        }

        if (lpElement == ciStart.lpLine)
          nCount+=ciStart.lpLine->nLineLen - ciStart.nCharInLine;
        else
          nCount+=lpElement->nLineLen;

        if (lpElement->nLineBreak != AELB_WRAP)
        {
          if (nNewLine == AELB_ASIS)
            nLineBreak=lpElement->nLineBreak;
          else
            nLineBreak=nNewLine;

          if (nLineBreak == AELB_R)
          {
            nCount+=1;
          }
          else if (nLineBreak == AELB_N)
          {
            nCount+=1;
          }
          else if (nLineBreak == AELB_RN)
          {
            nCount+=2;
          }
          else if (nLineBreak == AELB_RRN)
          {
            nCount+=3;
          }
        }
        lpElement=lpElement->next;
      }
    }
  }
  return (nCompare < 0)?-nCount:nCount;
}

DWORD AE_IndexOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, int nOffset, int nNewLine)
{
  AECHARINDEX ciCount=*ciCharIn;
  int nLineBreak;
  int nOffsetCount=nOffset;

  //Set new line
  if (nNewLine < AELB_ASIS)
  {
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->popt->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->popt->nOutputNewLine;
  }

  if (nOffsetCount == 0)
  {
    if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
    {
      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AE_NextLine(&ciCount);
    }
    *ciCharOut=ciCount;
    return TRUE;
  }
  else if (nOffsetCount > 0)
  {
    for (;;)
    {
      while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
      {
        if (nOffsetCount <= 0)
          goto PlusOffEnd;

        --nOffsetCount;
        ++ciCount.nCharInLine;
      }

      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
      {
        nLineBreak=AELB_WRAP;
      }
      else
      {
        if (nOffsetCount <= 0)
          goto PlusOffEnd;

        if (nNewLine == AELB_ASIS)
          nLineBreak=ciCount.lpLine->nLineBreak;
        else
          nLineBreak=nNewLine;
      }

      if (ciCount.lpLine->next)
        AE_NextLine(&ciCount);
      else
        goto PlusOffEnd;

      if (nLineBreak == AELB_R)
      {
        nOffsetCount-=1;
      }
      else if (nLineBreak == AELB_N)
      {
        nOffsetCount-=1;
      }
      else if (nLineBreak == AELB_RN)
      {
        nOffsetCount-=2;
      }
      else if (nLineBreak == AELB_RRN)
      {
        nOffsetCount-=3;
      }
    }

    PlusOffEnd:
    ciCount.nCharInLine=min(ciCount.nCharInLine, ciCount.lpLine->nLineLen);
    *ciCharOut=ciCount;
    return nOffset - nOffsetCount;
  }
  else if (nOffsetCount < 0)
  {
    for (;;)
    {
      while (ciCount.nCharInLine >= 0)
      {
        if (nOffsetCount >= 0)
          goto MinusOffEnd;

        ++nOffsetCount;
        --ciCount.nCharInLine;
      }
      --nOffsetCount;

      if (ciCount.lpLine->prev)
        AE_PrevLine(&ciCount);
      else
        goto MinusOffEnd;

      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
      {
        nLineBreak=AELB_WRAP;
      }
      else
      {
        if (nNewLine == AELB_ASIS)
          nLineBreak=ciCount.lpLine->nLineBreak;
        else
          nLineBreak=nNewLine;
      }

      if (nLineBreak == AELB_R)
      {
        nOffsetCount+=1;
      }
      else if (nLineBreak == AELB_N)
      {
        nOffsetCount+=1;
      }
      else if (nLineBreak == AELB_RN)
      {
        nOffsetCount+=2;
      }
      else if (nLineBreak == AELB_RRN)
      {
        nOffsetCount+=3;
      }
    }

    MinusOffEnd:
    ciCount.nCharInLine=max(ciCount.nCharInLine, 0);
    *ciCharOut=ciCount;
    return mod(nOffset - nOffsetCount);
  }
  return 0;
}

BOOL AE_IndexNormalize(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine > 0 && ciChar->nCharInLine < ciChar->lpLine->nLineLen)
  {
    if (AE_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine]))
    {
      if (AE_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine - 1]))
      {
        --ciChar->nCharInLine;
        return TRUE;
      }
    }
  }
  return FALSE;
}

BOOL AE_IndexUpdate(AKELEDIT *ae, AECHARINDEX *ciChar)
{
  ciChar->nLine=max(ciChar->nLine, 0);

  if (ciChar->nLine > ae->ptxt->nLineCount)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, ciChar, FALSE);
    return TRUE;
  }
  if (ciChar->lpLine=AE_GetLineData(ae, ciChar->nLine))
  {
    AE_IndexNormalize(ciChar);
    return TRUE;
  }
  return FALSE;
}

int AE_UpdateWrap(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwWrap)
{
  AECHARINDEX ciSelStart=ae->ciSelStartIndex;
  AECHARINDEX ciSelEnd=ae->ciSelEndIndex;
  AECHARINDEX ciFirstVisibleLineAfterWrap;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  AEPOINT *lpPointThree;
  int nWrapCount;
  BOOL bNotify=FALSE;

  if (!ae->dwNotifySelChange && !ae->dwNotifyTextChange)
  {
    //Only if AE_UpdateWrap is not part of the other text modification operation
    bNotify=TRUE;
    AE_NotifyChanging(ae, AETCT_WRAP);
  }
  AE_GetIndex(ae, AEGI_FIRSTVISIBLELINE, NULL, &ciFirstVisibleLineAfterWrap, FALSE);
  lpPointOne=AE_StackPointInsert(ae, &ciSelStart);
  lpPointTwo=AE_StackPointInsert(ae, &ciSelEnd);
  lpPointThree=AE_StackPointInsert(ae, &ciFirstVisibleLineAfterWrap);
  nWrapCount=AE_WrapLines(ae, liWrapStart, liWrapEnd, dwWrap);
  ciSelStart=lpPointOne->ciPoint;
  ciSelEnd=lpPointTwo->ciPoint;
  ciFirstVisibleLineAfterWrap=lpPointThree->ciPoint;
  AE_StackPointDelete(ae, lpPointOne);
  AE_StackPointDelete(ae, lpPointTwo);
  AE_StackPointDelete(ae, lpPointThree);

  ae->ptCaret.x=0;
  ae->ptCaret.y=0;
  if (AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
    ae->ciCaretIndex=ciSelEnd;
  else
    ae->ciCaretIndex=ciSelStart;
  ae->ciSelStartIndex=ciSelStart;
  ae->ciSelEndIndex=ciSelEnd;

  //Update scroll bars
  if (!ae->ptxt->liMaxWidthLine.lpLine)
    AE_CalcLinesWidth(ae, NULL, NULL, 0);
  else
    AE_CalcLinesWidth(ae, liWrapStart, liWrapEnd, 0);

  if (nWrapCount)
  {
    ae->ptxt->nLineCount+=nWrapCount;
    ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
    AE_UpdateScrollBars(ae, SB_VERT);
  }
  AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL|AESELT_RESETSELECTION|AESELT_LOCKNOTIFY);
  AE_VScrollLine(ae, ciFirstVisibleLineAfterWrap.nLine - AE_GetFirstVisibleLine(ae), AESB_ALIGNTOP);

  if (bNotify)
  {
    AE_NotifyChanged(ae); //AETCT_WRAP
    bNotify=FALSE;
  }
  return nWrapCount;
}

int AE_WrapLines(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwWrap)
{
  AKELEDIT *lpSource=ae;
  AEPOINT *lpPoint=(AEPOINT *)ae->ptxt->hPointsStack.first;
  AELINEINDEX liFirst;
  AELINEINDEX liCount;
  DWORD dwMaxWidth=0;
  DWORD dwStartTime=GetTickCount();
  DWORD dwProgressTime=0;
  DWORD dwCurrentTime=0;
  int nLineCount=0;
  int nWrapped=0;
  int nUnwrapped=0;
  int nStopLine;
  BOOL bPrevLine=FALSE;
  BOOL bStopProgress=FALSE;

  if (dwWrap)
  {
    if (ae->lpMaster)
      lpSource=ae->lpMaster;

    if (lpSource->ptxt->dwWrapLimit)
    {
      if (dwWrap & AEWW_LIMITPIXEL)
        dwMaxWidth=lpSource->ptxt->dwWrapLimit;
      else if (dwWrap & AEWW_LIMITSYMBOL)
        dwMaxWidth=lpSource->ptxt->dwWrapLimit * lpSource->ptxt->nAveCharWidth;
    }
    else dwMaxWidth=(lpSource->rcDraw.right - lpSource->rcDraw.left) - lpSource->ptxt->nAveCharWidth;
  }
  else dwMaxWidth=(DWORD)-1;

  if (!liWrapStart)
  {
    liCount.nLine=0;
    liCount.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
    liFirst=liCount;
  }
  else
  {
    if (liWrapStart->lpLine->prev)
    {
      if (liWrapStart->lpLine->prev->nLineBreak == AELB_WRAP)
      {
        liCount.nLine=liWrapStart->nLine - 1;
        liCount.lpLine=liWrapStart->lpLine->prev;
        bPrevLine=TRUE;
      }
    }
    if (!bPrevLine)
      liCount=*liWrapStart;
    liFirst=liCount;
  }

  if (!liWrapEnd)
  {
    nStopLine=ae->ptxt->nLineCount;
  }
  else
  {
    nStopLine=liWrapEnd->nLine;
  }

  //Start progress
  if (ae->popt->dwEventMask & AENM_PROGRESS)
  {
    if (!liWrapStart && !liWrapEnd)
    {
      if (bStopProgress=AE_NotifyProgress(ae, AEPGS_WRAPTEXT, GetTickCount() - dwStartTime, 0, nStopLine))
        return 0;
    }
  }

  while (liCount.lpLine)
  {
    if (dwWrap)
    {
      if (liCount.lpLine->nLineWidth == -1)
        AE_GetLineWidth(ae, liCount.lpLine);

      if (liCount.lpLine->nLineLen > 1 && (DWORD)liCount.lpLine->nLineWidth > dwMaxWidth)
      {
        if (nWrapped=AE_LineWrap(ae, &liCount, (liCount.nLine == liFirst.nLine)?&liFirst:NULL, &liCount, dwMaxWidth, dwWrap, &lpPoint))
        {
          nLineCount+=nWrapped;
        }
        else break;
      }
    }
    if (nUnwrapped=AE_LineUnwrap(ae, &liCount, dwMaxWidth, &lpPoint))
    {
      nLineCount+=nUnwrapped;

      if (liCount.nLine == liFirst.nLine)
        liFirst.lpLine=liCount.lpLine;
      if (dwWrap) continue;
    }

    if (liCount.nLine >= nStopLine + nLineCount)
      break;
    ++liCount.nLine;
    liCount.lpLine=liCount.lpLine->next;

    //Progressing
    if (ae->popt->dwEventMask & AENM_PROGRESS)
    {
      if (!liWrapStart && !liWrapEnd)
      {
        dwCurrentTime=GetTickCount();

        if (dwCurrentTime - dwProgressTime > AETIME_BEFOREPROGRESS)
        {
          if (bStopProgress=AE_NotifyProgress(ae, AEPGS_WRAPTEXT, dwCurrentTime - dwStartTime, liCount.nLine - nLineCount, nStopLine))
            break;
          dwProgressTime=GetTickCount();
        }
      }
    }
  }

  //End progress
  if (ae->popt->dwEventMask & AENM_PROGRESS)
  {
    if (!liWrapStart && !liWrapEnd)
    {
      if (!bStopProgress)
      {
        AE_NotifyProgress(ae, AEPGS_WRAPTEXT, GetTickCount() - dwStartTime, nStopLine, nStopLine);
      }
    }
  }

  if (liWrapStart) *liWrapStart=liFirst;
  if (liWrapEnd) *liWrapEnd=liCount;

  //Update control points
  if (ae->liFirstDrawLine.lpLine)
  {
    if (ae->liFirstDrawLine.nLine > nStopLine)
    {
      ae->liFirstDrawLine.nLine+=nLineCount;
    }
    else if (ae->liFirstDrawLine.nLine >= liFirst.nLine)
    {
      ae->liFirstDrawLine.nLine=0;
      ae->liFirstDrawLine.lpLine=NULL;
      ae->nFirstDrawLineOffset=0;
    }
  }
  if (ae->ptxt->liMaxWidthLine.lpLine)
  {
    if (ae->ptxt->liMaxWidthLine.nLine > nStopLine)
    {
      ae->ptxt->liMaxWidthLine.nLine+=nLineCount;
    }
    else if (ae->ptxt->liMaxWidthLine.nLine >= liFirst.nLine)
    {
      ae->ptxt->liMaxWidthLine.nLine=0;
      ae->ptxt->liMaxWidthLine.lpLine=NULL;
    }
  }
  if (ae->ptxt->liLineUnwrapLastCall.lpLine)
  {
    if (ae->ptxt->liLineUnwrapLastCall.nLine > nStopLine)
    {
      ae->ptxt->liLineUnwrapLastCall.nLine+=nLineCount;
    }
    else if (ae->ptxt->liLineUnwrapLastCall.nLine >= liFirst.nLine)
    {
      ae->ptxt->liLineUnwrapLastCall.nLine=0;
      ae->ptxt->liLineUnwrapLastCall.lpLine=NULL;
      ae->ptxt->nLineUnwrapLastCall=0;
    }
  }
  if (ae->ciLastCallIndex.lpLine)
  {
    if (ae->ciLastCallIndex.nLine > nStopLine)
    {
      ae->ciLastCallIndex.nLine+=nLineCount;
    }
    else if (ae->ciLastCallIndex.nLine >= liFirst.nLine)
    {
      ae->ciLastCallIndex.lpLine=NULL;
      ae->nLastCallOffset=0;
    }
  }
  return nLineCount;
}

int AE_LineWrap(AKELEDIT *ae, const AELINEINDEX *liLine, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwMaxWidth, DWORD dwWrap, AEPOINT **lpPoint)
{
  AELINEINDEX liStart=*liLine;
  AELINEINDEX liEnd=*liLine;
  AELINEDATA ldLine;
  AELINEDATA *lpInitialElement=liLine->lpLine;
  AELINEDATA *lpNewElement=NULL;
  AEPOINT *lpTmpPoint=*lpPoint;
  int nCharStart=0;
  int nCharEnd=0;
  int nLineCount=0;
  int i;

  if (lpInitialElement->nLineLen > 1)
  {
    while (nCharEnd < lpInitialElement->nLineLen)
    {
      ldLine.wpLine=lpInitialElement->wpLine + nCharStart;
      ldLine.nLineLen=lpInitialElement->nLineLen - nCharStart;
      ldLine.nLineWidth=-1;

      if (AE_GetCharInLine(ae, &ldLine, dwMaxWidth, AECIL_USELINEBEGINPOS, &nCharEnd, NULL, FALSE))
      {
        if (nCharEnd == 0)
        {
          //Minimum wrap line length is 1
          nCharEnd=1;
        }
        nCharEnd+=nCharStart;

        //Find end of word
        if (dwWrap & AEWW_WORD)
        {
          if (nCharEnd < lpInitialElement->nLineLen)
          {
            for (i=nCharEnd - 1; i >= nCharStart; --i)
            {
              if (AE_IsInDelimiterList(ae->ptxt->wszWrapDelimiters, lpInitialElement->wpLine[i], TRUE))
                break;
            }
            if (i >= nCharStart)
              nCharEnd=i + 1;
          }
        }

        //Wrap
        if (lpNewElement=AE_StackLineInsertBefore(ae, lpInitialElement))
        {
          if (!nCharStart)
          {
            liStart.lpLine=lpNewElement;
          }

          lpNewElement->nLineWidth=-1;
          if (nCharEnd < lpInitialElement->nLineLen)
            lpNewElement->nLineBreak=AELB_WRAP;
          else
            lpNewElement->nLineBreak=lpInitialElement->nLineBreak;
          lpNewElement->nLineLen=nCharEnd - nCharStart;

          if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
          {
            xmemcpy(lpNewElement->wpLine, lpInitialElement->wpLine + nCharStart, lpNewElement->nLineLen * sizeof(wchar_t));
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
            AE_GetLineWidth(ae, lpNewElement);

            //Update points
            for (; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
            {
              if (lpTmpPoint->ciPoint.nLine > liStart.nLine)
                break;

              if (lpTmpPoint->ciPoint.nLine == liStart.nLine)
              {
                if (lpTmpPoint->ciPoint.nCharInLine >= nCharStart &&
                    (lpTmpPoint->ciPoint.nCharInLine <= nCharEnd || nCharEnd == lpInitialElement->nLineLen))
                {
                  lpTmpPoint->ciPoint.nLine+=nLineCount;
                  lpTmpPoint->ciPoint.lpLine=lpNewElement;
                  lpTmpPoint->ciPoint.nCharInLine-=nCharStart;
                  lpTmpPoint->dwFlags|=AEPTF_MOVED;

                  if (nCharEnd < lpInitialElement->nLineLen)
                    *lpPoint=lpTmpPoint;
                }
                else if (lpTmpPoint->ciPoint.nCharInLine > nCharEnd)
                {
                  break;
                }
              }
              else *lpPoint=lpTmpPoint;
            }
            nCharStart=nCharEnd;

            if (nCharEnd < lpInitialElement->nLineLen)
            {
              ++nLineCount;
              continue;
            }
          }
        }
      }
      break;
    }

    if (nLineCount)
    {
      AE_StackLineDelete(ae, lpInitialElement);

      liEnd.lpLine=lpNewElement;
      liEnd.nLine+=nLineCount;

      //Update points
      for (; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
      {
        if (lpTmpPoint->ciPoint.nLine > liStart.nLine)
        {
          lpTmpPoint->ciPoint.nLine+=nLineCount;
          lpTmpPoint->dwFlags|=AEPTF_MOVED;
        }
      }
    }
  }
  if (liWrapStart) *liWrapStart=liStart;
  if (liWrapEnd) *liWrapEnd=liEnd;
  return nLineCount;
}

int AE_LineUnwrap(AKELEDIT *ae, AELINEINDEX *liLine, DWORD dwMaxWidth, AEPOINT **lpPoint)
{
  AELINEINDEX liCurLine;
  AELINEDATA *lpNewElement;
  AELINEDATA *lpCurElement;
  AELINEDATA *lpNextElement;
  AEPOINT *lpTmpPoint=*lpPoint;
  DWORD dwUnwrapLineWidth=0;
  DWORD dwUnwrapLineBreak=AELB_EOF;
  DWORD dwUnwrapLineLen=0;
  DWORD dwCountWidth=0;
  DWORD dwCountLen=0;
  int nLineCount=0;

  if (liLine->lpLine->nLineWidth == -1)
    AE_GetLineWidth(ae, liLine->lpLine);

  if (liLine->lpLine->nLineBreak == AELB_WRAP)
  {
    if ((DWORD)liLine->lpLine->nLineWidth < dwMaxWidth)
    {
      if (ae->ptxt->dwWordWrap & AEWW_SYMBOL)
      {
        if (liLine->lpLine->next->nLineLen)
        {
          if ((DWORD)liLine->lpLine->nLineWidth + AE_GetCharWidth(ae, liLine->lpLine->next->wpLine, NULL) > dwMaxWidth)
            return nLineCount;
        }
      }

      //Calculate unwrapped line size
      if (liLine->lpLine->nLineWidth == -1)
        AE_GetLineWidth(ae, liLine->lpLine);
      dwUnwrapLineWidth+=liLine->lpLine->nLineWidth;
      dwUnwrapLineLen+=liLine->lpLine->nLineLen;
      lpCurElement=liLine->lpLine->next;

      while (lpCurElement)
      {
        //Set new line width (necessary for tabs)
        lpCurElement->nLineWidth=AE_GetStringWidth(ae, lpCurElement->wpLine, lpCurElement->nLineLen, dwUnwrapLineWidth);
        dwUnwrapLineWidth+=lpCurElement->nLineWidth;
        dwUnwrapLineLen+=lpCurElement->nLineLen;

        if (dwUnwrapLineWidth >= dwMaxWidth || lpCurElement->nLineBreak != AELB_WRAP)
        {
          dwUnwrapLineBreak=lpCurElement->nLineBreak;
          break;
        }
        lpCurElement=lpCurElement->next;
      }

      //Unwrap line
      if (lpNewElement=AE_StackLineInsertBefore(ae, liLine->lpLine))
      {
        lpNewElement->nLineWidth=dwUnwrapLineWidth;
        lpNewElement->nLineBreak=dwUnwrapLineBreak;
        lpNewElement->nLineLen=dwUnwrapLineLen;

        if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
        {
          liCurLine=*liLine;

          while (liCurLine.lpLine)
          {
            xmemcpy(lpNewElement->wpLine + dwCountLen, liCurLine.lpLine->wpLine, liCurLine.lpLine->nLineLen * sizeof(wchar_t));
            dwCountWidth+=liCurLine.lpLine->nLineWidth;
            dwCountLen+=liCurLine.lpLine->nLineLen;

            //Update points
            for (; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
            {
              if (lpTmpPoint->ciPoint.nLine > liCurLine.nLine)
                break;

              if (lpTmpPoint->ciPoint.nLine == liCurLine.nLine)
              {
                lpTmpPoint->ciPoint.nLine+=nLineCount;
                lpTmpPoint->ciPoint.lpLine=lpNewElement;
                lpTmpPoint->ciPoint.nCharInLine+=(dwCountLen - liCurLine.lpLine->nLineLen);
                lpTmpPoint->dwFlags|=AEPTF_MOVED;
              }
              else if (!nLineCount)
                *lpPoint=lpTmpPoint;
            }

            if (dwCountWidth >= dwMaxWidth || liCurLine.lpLine->nLineBreak != AELB_WRAP)
            {
              AE_StackLineDelete(ae, liCurLine.lpLine);
              break;
            }

            --nLineCount;
            lpNextElement=liCurLine.lpLine->next;
            AE_StackLineDelete(ae, liCurLine.lpLine);
            liCurLine.nLine+=1;
            liCurLine.lpLine=lpNextElement;
          }
          lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
        }
      }

      if (nLineCount)
      {
        liLine->lpLine=lpNewElement;

        //Update points
        for (; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
        {
          if (lpTmpPoint->ciPoint.nLine > liLine->nLine)
          {
            lpTmpPoint->ciPoint.nLine+=nLineCount;
            lpTmpPoint->dwFlags|=AEPTF_MOVED;
          }
        }
      }
    }
  }
  return nLineCount;
}

void AE_CalcLinesWidth(AKELEDIT *ae, const AELINEINDEX *liStartLine, const AELINEINDEX *liEndLine, DWORD dwFlags)
{
  AELINEINDEX liStart;
  AELINEINDEX liEnd;
  AELINEINDEX liMaxWidthLine;
  int nHScrollMax=0;

  if (!liStartLine)
  {
    liStart.nLine=0;
    liStart.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
  }
  else liStart=*liStartLine;

  if (!liEndLine)
  {
    liEnd.nLine=ae->ptxt->nLineCount;
    liEnd.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.last;
  }
  else liEnd=*liEndLine;

  while (liStart.lpLine)
  {
    if ((dwFlags & AECLW_FRESH) || liStart.lpLine->nLineWidth == -1)
    {
      AE_GetLineWidth(ae, liStart.lpLine);
    }
    if (liStart.lpLine->nLineWidth + ae->ptxt->nAveCharWidth > nHScrollMax)
    {
      nHScrollMax=liStart.lpLine->nLineWidth + ae->ptxt->nAveCharWidth;
      liMaxWidthLine=liStart;
    }
    if (liStart.lpLine == liEnd.lpLine) break;

    ++liStart.nLine;

    liStart.lpLine=liStart.lpLine->next;
  }

  if (!liStartLine && !liEndLine)
  {
    ae->ptxt->liMaxWidthLine=liMaxWidthLine;

    if (nHScrollMax != ae->ptxt->nHScrollMax)
    {
      ae->ptxt->nHScrollMax=nHScrollMax;
      if (!(dwFlags & AECLW_LOCKUPDATE))
        AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }
  else
  {
    if (nHScrollMax > ae->ptxt->nHScrollMax)
    {
      ae->ptxt->liMaxWidthLine=liMaxWidthLine;
      ae->ptxt->nHScrollMax=nHScrollMax;
      if (!(dwFlags & AECLW_LOCKUPDATE))
        AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }
}

int AE_CheckCodepage(AKELEDIT *ae, int nCodePage)
{
  AELINEDATA *lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
  int nLine=1;
  BOOL bUsedDefaultChar;

  while (lpLine)
  {
    if (lpLine->nLineLen)
    {
      bUsedDefaultChar=FALSE;
      WideCharToMultiByte(nCodePage, WC_NO_BEST_FIT_CHARS, lpLine->wpLine, lpLine->nLineLen, NULL, 0, NULL, &bUsedDefaultChar);
      if (bUsedDefaultChar)
        return nLine;
    }
    ++nLine;
    lpLine=lpLine->next;
  }
  return 0;
}

void AE_SetDrawRect(AKELEDIT *ae, const RECT *lprcDraw, BOOL bRedraw)
{
  if (lprcDraw)
  {
    ae->rcDraw=*lprcDraw;
  }
  else
  {
    ae->rcDraw=ae->rcEdit;
    ae->rcDraw.left+=1;
  }

  if (bRedraw) InvalidateRect(ae->hWndEdit, NULL, TRUE);
}

void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bRedraw)
{
  AEFONTITEMA *fi;
  TEXTMETRICA tmEdit;
  SIZE sizeWidth;
  HFONT hFontSystem=(HFONT)GetStockObject(SYSTEM_FONT);
  HDC hDC=ae->hDC;
  HFONT hFontOld=NULL;

  if (hDC || (hDC=GetDC(ae->hWndEdit)))
  {
    if (hFont)
    {
      hFontOld=(HFONT)SelectObject(hDC, hFont);
      GetObjectA(hFont, sizeof(LOGFONTA), &ae->ptxt->lfFontA);
      if (!GetTextMetricsA(hDC, &tmEdit))
        hFont=NULL;
    }
    if (!hFont)
    {
      hFontOld=(HFONT)SelectObject(hDC, hFontSystem);
      GetObjectA((HGDIOBJ)hFontSystem, sizeof(LOGFONTA), &ae->ptxt->lfFontA);
      if (!GetTextMetricsA(hDC, &tmEdit))
        return;
      hFont=hFontSystem;
    }

    ae->ptxt->hFont=hFont;
    ae->ptxt->lfFontA.lfHeight=-mod(ae->ptxt->lfFontA.lfHeight);
    ae->ptxt->lfFontA.lfWidth=0;
    if (!(fi=AE_StackFontItemGetA(&hAkelEditFontsStackA, &ae->ptxt->lfFontA)))
      fi=AE_StackFontItemInsertA(&hAkelEditFontsStackA, &ae->ptxt->lfFontA);
    ae->ptxt->lpCharWidths=fi->lpCharWidths;
    ae->ptxt->hFontNormal=fi->hFontNormal;
    ae->ptxt->hFontBold=fi->hFontBold;
    ae->ptxt->hFontItalic=fi->hFontItalic;
    ae->ptxt->hFontBoldItalic=fi->hFontBoldItalic;
    ae->ptxt->hFontUrl=fi->hFontUrl;

    ae->ptxt->nCharHeight=tmEdit.tmHeight + ae->ptxt->nLineGap;
    GetTextExtentPoint32W(hDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeWidth);
    ae->ptxt->nAveCharWidth=sizeWidth.cx / 52;
    GetTextExtentPoint32W(hDC, L" ", 1, &sizeWidth);
    ae->ptxt->nSpaceCharWidth=sizeWidth.cx;
    ae->ptxt->nTabWidth=ae->ptxt->nSpaceCharWidth * ae->ptxt->nTabStop;

    InvalidateRect(ae->hWndEdit, &ae->rcDraw, bRedraw);

    if (!ae->hDC)
    {
      if (hFontOld) SelectObject(hDC, hFontOld);
      ReleaseDC(ae->hWndEdit, hDC);
    }
  }
}

void AE_SetEditFontW(AKELEDIT *ae, HFONT hFont, BOOL bRedraw)
{
  AEFONTITEMW *fi;
  TEXTMETRICW tmEdit;
  SIZE sizeWidth;
  HFONT hFontSystem=(HFONT)GetStockObject(SYSTEM_FONT);
  HDC hDC=ae->hDC;
  HFONT hFontOld=NULL;

  if (hDC || (hDC=GetDC(ae->hWndEdit)))
  {
    if (hFont)
    {
      hFontOld=(HFONT)SelectObject(hDC, hFont);
      GetObjectW(hFont, sizeof(LOGFONTW), &ae->ptxt->lfFontW);
      if (!GetTextMetricsW(hDC, &tmEdit))
        hFont=NULL;
    }
    if (!hFont)
    {
      hFontOld=(HFONT)SelectObject(hDC, hFontSystem);
      GetObjectW((HGDIOBJ)hFontSystem, sizeof(LOGFONTW), &ae->ptxt->lfFontW);
      if (!GetTextMetricsW(hDC, &tmEdit))
        return;
      hFont=hFontSystem;
    }

    ae->ptxt->hFont=hFont;
    ae->ptxt->lfFontW.lfHeight=-mod(ae->ptxt->lfFontW.lfHeight);
    ae->ptxt->lfFontW.lfWidth=0;
    if (!(fi=AE_StackFontItemGetW(&hAkelEditFontsStackW, &ae->ptxt->lfFontW)))
      fi=AE_StackFontItemInsertW(&hAkelEditFontsStackW, &ae->ptxt->lfFontW);
    ae->ptxt->lpCharWidths=fi->lpCharWidths;
    ae->ptxt->hFontNormal=fi->hFontNormal;
    ae->ptxt->hFontBold=fi->hFontBold;
    ae->ptxt->hFontItalic=fi->hFontItalic;
    ae->ptxt->hFontBoldItalic=fi->hFontBoldItalic;
    ae->ptxt->hFontUrl=fi->hFontUrl;

    ae->ptxt->nCharHeight=tmEdit.tmHeight + ae->ptxt->nLineGap;
    GetTextExtentPoint32W(hDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeWidth);
    ae->ptxt->nAveCharWidth=sizeWidth.cx / 52;
    GetTextExtentPoint32W(hDC, L" ", 1, &sizeWidth);
    ae->ptxt->nSpaceCharWidth=sizeWidth.cx;
    ae->ptxt->nTabWidth=ae->ptxt->nSpaceCharWidth * ae->ptxt->nTabStop;

    InvalidateRect(ae->hWndEdit, &ae->rcDraw, bRedraw);

    if (!ae->hDC)
    {
      if (hFontOld) SelectObject(hDC, hFontOld);
      ReleaseDC(ae->hWndEdit, hDC);
    }
  }
}

int AE_GetLineSelection(AKELEDIT *ae, const AELINEINDEX *liLine, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, const POINT *ptSelStart, const POINT *ptSelEnd, int *nSelStartIndexInLine, int *nSelEndIndexInLine, BOOL bColumnSel)
{
  if (ciSelStart->nLine > liLine->nLine || ciSelEnd->nLine < liLine->nLine)
    goto Empty;
  if (ciSelStart->nLine == ciSelEnd->nLine && ciSelStart->nCharInLine == ciSelEnd->nCharInLine)
    goto Empty;

  if (bColumnSel)
  {
    int nStartX=min(ptSelStart->x, ptSelEnd->x);
    int nEndX=max(ptSelStart->x, ptSelEnd->x);
    int nSelStart;
    int nSelEnd;
    int i;

    AE_GetCharRangeInLine(ae, liLine->lpLine, nStartX, nEndX, &nSelStart, NULL, &nSelEnd, NULL, bColumnSel);

    if (nSelStart > nSelEnd)
    {
      i=nSelStart;
      nSelStart=nSelEnd;
      nSelEnd=i;
    }

    if (nSelStart == nSelEnd)
      goto Empty;
    if (nSelStart == 0 && nSelEnd == liLine->lpLine->nLineLen)
      goto Full;

    *nSelStartIndexInLine=nSelStart;
    *nSelEndIndexInLine=nSelEnd;
    return AELS_PARTLY;
  }
  else
  {
    if (ciSelEnd->nLine == liLine->nLine && ciSelEnd->nCharInLine == 0)
      goto Empty;
    if ((ciSelStart->nLine < liLine->nLine || (ciSelStart->nLine == liLine->nLine && ciSelStart->nCharInLine == 0)) &&
        (ciSelEnd->nLine > liLine->nLine || (ciSelEnd->nLine == liLine->nLine && ciSelEnd->nCharInLine == ciSelEnd->lpLine->nLineLen)))
      goto Full;

    if (ciSelStart->nLine == liLine->nLine)
      *nSelStartIndexInLine=ciSelStart->nCharInLine;
    else
      *nSelStartIndexInLine=0;
    if (ciSelEnd->nLine == liLine->nLine)
      *nSelEndIndexInLine=ciSelEnd->nCharInLine;
    else
      *nSelEndIndexInLine=liLine->lpLine->nLineLen;
    return AELS_PARTLY;
  }

  Full:
  *nSelStartIndexInLine=0;
  *nSelEndIndexInLine=liLine->lpLine->nLineLen;
  return AELS_FULL;

  Empty:
  *nSelStartIndexInLine=0;
  *nSelEndIndexInLine=0;
  return AELS_EMPTY;
}

void AE_GetSelLines(AKELEDIT *ae, const AECHARINDEX *ciMin, const AECHARINDEX *ciMax, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, const POINT *ptSelStart, const POINT *ptSelEnd, BOOL bColumnSel)
{
  AELINEINDEX liLine;

  liLine.lpLine=ciMin->lpLine;
  liLine.nLine=ciMin->nLine;

  while (liLine.lpLine)
  {
    AE_GetLineSelection(ae, &liLine, ciSelStart, ciSelEnd, ptSelStart, ptSelEnd, &liLine.lpLine->nSelStart, &liLine.lpLine->nSelEnd, bColumnSel);
    if (liLine.lpLine == ciMax->lpLine) break;
    ++liLine.nLine;

    liLine.lpLine=liLine.lpLine->next;
  }
}

void AE_ClearSelLines(AKELEDIT *ae, const AECHARINDEX *ciMin, const AECHARINDEX *ciMax)
{
  AELINEDATA *lpElement=ciMin->lpLine;

  while (lpElement)
  {
    lpElement->nSelStart=0;
    lpElement->nSelEnd=0;
    if (lpElement == ciMax->lpLine) break;

    lpElement=lpElement->next;
  }
}

void AE_SetSelectionPos(AKELEDIT *ae, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, BOOL bColumnSel, DWORD dwSelFlags, DWORD dwSelType)
{
  AECHARINDEX ciSelStartOld;
  AECHARINDEX ciSelEndOld;
  AECHARINDEX ciCaretOld;
  AECHARINDEX ciCaretNew;
  AECHARINDEX ciSelStartNew;
  AECHARINDEX ciSelEndNew;
  AECHARINDEX ciTmp;
  POINT ptSelStart;
  POINT ptSelEnd;
  BOOL bColumnSelOld;

  if (!(dwSelFlags & AESELT_LOCKUNDOGROUPING))
  {
    AE_StackUndoGroupStop(ae);
  }
  if (!(dwSelFlags & AESELT_MOUSE))
    ae->dwMouseSelType=AEMSS_CHARS;

  if (ciSelStart->lpLine && ciSelEnd->lpLine)
  {
    ciSelStartOld=ae->ciSelStartIndex;
    ciSelEndOld=ae->ciSelEndIndex;
    ciCaretOld=ae->ciCaretIndex;
    ciSelStartNew=*ciSelStart;
    ciSelEndNew=*ciSelEnd;
    ciCaretNew=*ciSelStart;
    bColumnSelOld=ae->bColumnSel;

    if (dwSelFlags & AESELT_COLUMNON)
      bColumnSel=TRUE;
    else if (dwSelFlags & AESELT_COLUMNASIS)
      bColumnSel=ae->bColumnSel;
    ae->bColumnSel=bColumnSel;

    //Exchange indexes
    if (AE_IndexCompare(&ciSelStartNew, &ciSelEndNew) > 0)
    {
      ciTmp=ciSelStartNew;
      ciSelStartNew=ciSelEndNew;
      ciSelEndNew=ciTmp;
    }

    //Normalize indexes
    AE_IndexNormalize(&ciSelStartNew);
    AE_IndexNormalize(&ciSelEndNew);
    AE_IndexNormalize(&ciCaretNew);

    if (bColumnSel)
    {
      AE_GetPosFromChar(ae, &ciSelStartNew, &ptSelStart, NULL);
      if (AE_IndexCompare(&ae->ciSelStartIndex, &ciSelStartNew))
        if (ptSelStart.x > max(ae->ptxt->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
          return;

      AE_GetPosFromChar(ae, &ciSelEndNew, &ptSelEnd, NULL);
      if (AE_IndexCompare(&ae->ciSelEndIndex, &ciSelEndNew))
        if (ptSelEnd.x > max(ae->ptxt->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
          return;
    }
    else
    {
      ciSelStartNew.nCharInLine=min(ciSelStartNew.nCharInLine, ciSelStartNew.lpLine->nLineLen);
      ciSelEndNew.nCharInLine=min(ciSelEndNew.nCharInLine, ciSelEndNew.lpLine->nLineLen);

      if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
      {
        if (AE_IndexCompare(&ciCaretOld, &ciCaretNew))
        {
          AE_GetPosFromChar(ae, &ciCaretNew, &ptSelStart, NULL);
          if (ptSelStart.x > max(ae->ptxt->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
            return;
        }
      }
      else ciCaretNew.nCharInLine=min(ciCaretNew.nCharInLine, ciCaretNew.lpLine->nLineLen);
    }

    //Notification
    if (!(dwSelFlags & AESELT_LOCKNOTIFY))
    {
      AE_NotifySelChanging(ae, dwSelType);
    }

    //Clear old lines selection and set new lines selection
    if ((dwSelFlags & AESELT_RESETSELECTION) ||
        bColumnSel || bColumnSelOld ||
        ciSelStartOld.nLine > ciSelEndNew.nLine ||
        ciSelEndOld.nLine < ciSelStartNew.nLine)
    {
      AE_ClearSelLines(ae, &ciSelStartOld, &ciSelEndOld);
      AE_GetSelLines(ae, &ciSelStartNew, &ciSelEndNew, &ciSelStartNew, &ciSelEndNew, &ptSelStart, &ptSelEnd, bColumnSel);
    }
    else
    {
      if (AE_IndexCompare(&ciSelStartOld, &ciSelStartNew))
      {
        if (ciSelStartOld.nLine < ciSelStartNew.nLine)
        {
          AE_ClearSelLines(ae, &ciSelStartOld, &ciSelStartNew);
          AE_GetSelLines(ae, &ciSelStartNew, &ciSelStartNew, &ciSelStartNew, &ciSelEndNew, &ptSelStart, &ptSelEnd, bColumnSel);
        }
        else AE_GetSelLines(ae, &ciSelStartNew, &ciSelStartOld, &ciSelStartNew, &ciSelEndNew, &ptSelStart, &ptSelEnd, bColumnSel);
      }
      if (AE_IndexCompare(&ciSelEndNew, &ciSelEndOld))
      {
        if (ciSelEndNew.nLine < ciSelEndOld.nLine)
        {
          AE_ClearSelLines(ae, &ciSelEndNew, &ciSelEndOld);
          AE_GetSelLines(ae, &ciSelEndNew, &ciSelEndNew, &ciSelStartNew, &ciSelEndNew, &ptSelStart, &ptSelEnd, bColumnSel);
        }
        else AE_GetSelLines(ae, &ciSelEndOld, &ciSelEndNew, &ciSelStartNew, &ciSelEndNew, &ptSelStart, &ptSelEnd, bColumnSel);
      }
    }

    ae->nSelStartCharOffset=AE_AkelIndexToRichOffset(ae, &ciSelStartNew);
    ae->ciSelStartIndex=ciSelStartNew;
    ae->nSelEndCharOffset=AE_AkelIndexToRichOffset(ae, &ciSelEndNew);
    ae->ciSelEndIndex=ciSelEndNew;
    AE_GetPosFromChar(ae, &ciCaretNew, &ae->ptCaret, NULL);
    ae->ciCaretIndex=ciCaretNew;

    if (!(dwSelFlags & AESELT_LOCKSCROLL))
    {
      //Set caret position
      AE_ScrollToCaret(ae, &ae->ptCaret, !(dwSelFlags & AESELT_NOVERTSCROLLCORRECT));
    }
    if (!(dwSelFlags & AESELT_LOCKCARET))
    {
      //Set caret position
      if (!(dwSelFlags & AESELT_NOCARETHORZINDENT))
        ae->nCaretHorzIndent=ae->ptCaret.x;
      if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
    }
    if (!(dwSelFlags & AESELT_LOCKUPDATE))
    {
      //Redraw lines
      if (ciSelStartOld.nLine > ciSelEndNew.nLine ||
          ciSelEndOld.nLine < ciSelStartNew.nLine)
      {
        AE_RedrawLineRange(ae, ciSelStartOld.nLine, ciSelEndOld.nLine, FALSE);
        AE_RedrawLineRange(ae, ciSelStartNew.nLine, ciSelEndNew.nLine, FALSE);
      }
      else
      {
        if (bColumnSel || bColumnSelOld)
        {
          AE_RedrawLineRange(ae, min(ciSelStartOld.nLine, ciSelStartNew.nLine), max(ciSelEndOld.nLine, ciSelEndNew.nLine), FALSE);
        }
        else
        {
          if (AE_IndexCompare(&ciSelStartOld, &ciSelStartNew))
            AE_RedrawLineRange(ae, min(ciSelStartOld.nLine, ciSelStartNew.nLine), max(ciSelStartOld.nLine, ciSelStartNew.nLine), FALSE);
          if (AE_IndexCompare(&ciSelEndOld, &ciSelEndNew))
            AE_RedrawLineRange(ae, min(ciSelEndOld.nLine, ciSelEndNew.nLine), max(ciSelEndOld.nLine, ciSelEndNew.nLine), FALSE);

          if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
          {
            if (!AE_IndexCompare(&ciSelStartOld, &ciSelStartNew) &&
                !AE_IndexCompare(&ciSelEndOld, &ciSelEndNew) &&
                 AE_IndexCompare(&ciCaretOld, &ciCaretNew))
            {
              AE_RedrawLineRange(ae, ciCaretNew.nLine, ciCaretNew.nLine, FALSE);
            }
          }
        }
      }
    }
    if (!(dwSelFlags & AESELT_LOCKNOTIFY))
    {
      AE_NotifySelChanged(ae);
    }
  }
}

void AE_UpdateSelection(AKELEDIT *ae, DWORD dwSelFlags)
{
  if (AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelStartIndex) <= 0)
    AE_SetSelectionPos(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, FALSE, dwSelFlags, AESCT_UPDATESELECTION);
  else
    AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ae->ciSelStartIndex, FALSE, dwSelFlags, AESCT_UPDATESELECTION);
}

void AE_SetMouseSelection(AKELEDIT *ae, const POINT *ptPos, BOOL bColumnSel, BOOL bShift)
{
  AECHARINDEX ciCharIndex;
  AECHARINDEX ciSelEnd;
  int dwMouseSelType=ae->dwMouseSelType;

  if (ae->rcDraw.bottom - ae->rcDraw.top > 0 && ae->rcDraw.right - ae->rcDraw.left > 0)
  {
    AE_GetCharFromPos(ae, ptPos, &ciCharIndex, NULL, bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

    if (ae->nCurrentCursor == AECC_MARGIN && (dwMouseSelType & AEMSS_WORDS))
      dwMouseSelType=AEMSS_LINES;

    //if ((dwMouseSelType & AEMSS_WORDS) && (dwMouseSelType & AEMSS_LBUTTONUP))
    //{
    //  if (bShift)
    //  {
    //    if (!AE_IndexCompare(&ae->ciSelStartIndex, &ciCharIndex) ||
    //        !AE_IndexCompare(&ae->ciSelEndIndex, &ciCharIndex))
    //    {
    //      ae->dwMouseSelType=AEMSS_CHARS;
    //      dwMouseSelType=AEMSS_CHARS;
    //    }
    //  }
    //}

    //One click (capture)
    if (dwMouseSelType & AEMSS_CHARS)
    {
      if (bShift)
      {
        if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciSelEnd=ae->ciCaretIndex;
        else if (AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
          ciSelEnd=ae->ciSelStartIndex;
        else
          ciSelEnd=ae->ciSelEndIndex;

        //Margin selection
        if (ae->nCurrentCursor == AECC_MARGIN)
        {
          if (AE_IndexCompare(&ciCharIndex, &ciSelEnd) < 0)
            ciCharIndex.nCharInLine=0;
          else
          {
            if (!AE_NextLineEx(&ciCharIndex, &ciCharIndex))
              ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
          }
        }
        //Characters selection
        else if (!AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
        {
          if (ptPos->x < ae->rcDraw.left)
          {
            AE_IndexDec(&ciCharIndex);
            ciCharIndex.nCharInLine=max(ciCharIndex.nCharInLine, 0);
          }
          else if (ptPos->x > ae->rcDraw.right)
          {
            AE_IndexInc(&ciCharIndex);
          }
        }

        if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex) || ae->bColumnSel != bColumnSel)
        {
          AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, bColumnSel, AESELT_MOUSE, AESCT_MOUSESINGLECLK|AESCT_MOUSECAPTURE);
        }
      }
      else AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, bColumnSel, AESELT_MOUSE, AESCT_MOUSESINGLECLK);
    }
    //Two clicks (capture)
    else if (dwMouseSelType & AEMSS_WORDS)
    {
      if (bShift)
      {
        if (AE_IndexCompare(&ciCharIndex, &ae->ciMouseSelClick) < 0)
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciMouseSelStart) < 0)
            AE_GetPrevBreak(ae, &ciCharIndex, &ciCharIndex, bColumnSel, ae->popt->dwWordBreak);
          else
            ciCharIndex=ae->ciMouseSelStart;
          ciSelEnd=ae->ciMouseSelEnd;
        }
        else
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciMouseSelEnd) > 0)
            AE_GetNextBreak(ae, &ciCharIndex, &ciCharIndex, bColumnSel, ae->popt->dwWordBreak);
          else
            ciCharIndex=ae->ciMouseSelEnd;
          ciSelEnd=ae->ciMouseSelStart;
        }

        if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
        {
          AE_IndexUpdate(ae, &ciCharIndex);
          AE_IndexUpdate(ae, &ciSelEnd);
          AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, bColumnSel, AESELT_MOUSE, AESCT_MOUSEDOUBLECLK|AESCT_MOUSECAPTURE);
        }
      }
    }
    //Three clicks (capture)
    else if (dwMouseSelType & AEMSS_LINES)
    {
      if (bShift)
      {
        if (AE_IndexCompare(&ciCharIndex, &ae->ciMouseSelClick) < 0)
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciMouseSelStart) < 0)
            ciCharIndex.nCharInLine=0;
          else
            ciCharIndex=ae->ciMouseSelStart;
          ciSelEnd=ae->ciMouseSelEnd;
        }
        else
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciMouseSelEnd) >= 0)
          {
            if (!AE_NextLineEx(&ciCharIndex, &ciCharIndex))
              ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
          }
          else
            ciCharIndex=ae->ciMouseSelEnd;
          ciSelEnd=ae->ciMouseSelStart;
        }

        if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
        {
          AE_IndexUpdate(ae, &ciCharIndex);
          AE_IndexUpdate(ae, &ciSelEnd);
          AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, bColumnSel, AESELT_MOUSE, AESCT_MOUSETRIPLECLK|AESCT_MOUSECAPTURE);
        }
      }
    }
  }
}

void AE_MouseMove(AKELEDIT *ae)
{
  POINT ptPos;

  if (ae->dwMouseScrollTimer)
  {
    GetCursorPos(&ptPos);
    ScreenToClient(ae->hWndEdit, &ptPos);

    if (ae->nCurrentCursor == AECC_MCENTER)
    {
    }
    else if (ae->nCurrentCursor == AECC_MLEFT)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos - (ae->ptMButtonDown.x - ptPos.x - 5));
    }
    else if (ae->nCurrentCursor == AECC_MLEFTTOP)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos - (ae->ptMButtonDown.x - ptPos.x - 5));
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->ptMButtonDown.y - ptPos.y - 5));
    }
    else if (ae->nCurrentCursor == AECC_MTOP)
    {
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->ptMButtonDown.y - ptPos.y - 5));
    }
    else if (ae->nCurrentCursor == AECC_MRIGHTTOP)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos + (ptPos.x - ae->ptMButtonDown.x - 5));
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->ptMButtonDown.y - ptPos.y - 5));
    }
    else if (ae->nCurrentCursor == AECC_MRIGHT)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos + (ptPos.x - ae->ptMButtonDown.x - 5));
    }
    else if (ae->nCurrentCursor == AECC_MRIGHTBOTTOM)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos + (ptPos.x - ae->ptMButtonDown.x - 5));
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ptPos.y - ae->ptMButtonDown.y - 5));
    }
    else if (ae->nCurrentCursor == AECC_MBOTTOM)
    {
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ptPos.y - ae->ptMButtonDown.y - 5));
    }
    else if (ae->nCurrentCursor == AECC_MLEFTBOTTOM)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos - (ae->ptMButtonDown.x - ptPos.x - 5));
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ptPos.y - ae->ptMButtonDown.y - 5));
    }
  }
  if (ae->dwMouseMoveTimer)
  {
    GetCursorPos(&ptPos);
    ScreenToClient(ae->hWndEdit, &ptPos);
    AE_SetMouseSelection(ae, &ptPos, ae->bColumnSel, TRUE);
  }
}

void AE_SetMouseCapture(AKELEDIT *ae, DWORD dwType)
{
  if (!ae->dwMouseCapture)
    SetCapture(ae->hWndEdit);
  ae->dwMouseCapture|=dwType;
}

void AE_ReleaseMouseCapture(AKELEDIT *ae, DWORD dwType)
{
  if (ae->dwMouseCapture & dwType)
  {
    ae->dwMouseCapture&=~dwType;
    if (!ae->dwMouseCapture)
      ReleaseCapture();
  }
}

void AE_GlobalToClient(AKELEDIT *ae, const POINT *ptGlobal, POINT *ptClient)
{
  ptClient->x=ae->rcDraw.left + (ptGlobal->x - ae->nHScrollPos);
  ptClient->y=ae->rcDraw.top + (ptGlobal->y - ae->nVScrollPos);
}

void AE_ClientToGlobal(AKELEDIT *ae, const POINT *ptClient, POINT *ptGlobal)
{
  ptGlobal->x=ae->nHScrollPos + (ptClient->x - ae->rcDraw.left);
  ptGlobal->y=ae->nVScrollPos + (ptClient->y - ae->rcDraw.top);
}

int AE_SetCursor(AKELEDIT *ae)
{
  POINT ptPos;
  BOOL bAlt=FALSE;
  BOOL bShift=FALSE;
  int nResult=0;

  if (GetKeyState(VK_MENU) < 0)
    bAlt=TRUE;
  if (GetKeyState(VK_SHIFT) < 0)
    bShift=TRUE;

  GetCursorPos(&ptPos);
  ScreenToClient(ae->hWndEdit, &ptPos);

  if (ae->dwMouseScrollTimer)
  {
    if (ptPos.x >= ae->ptMButtonDown.x - 5 &&
        ptPos.x <= ae->ptMButtonDown.x + 5 &&
        ptPos.y >= ae->ptMButtonDown.y - 5 &&
        ptPos.y <= ae->ptMButtonDown.y + 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
        SetCursor(hAkelEditCursorMCenterAll);
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
        SetCursor(hAkelEditCursorMCenterLeftRight);
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
        SetCursor(hAkelEditCursorMCenterTopBottom);
      nResult=AECC_MCENTER;
    }
    else if (ptPos.x < ae->ptMButtonDown.x - 5 &&
             ptPos.y >= ae->ptMButtonDown.y - 5 &&
             ptPos.y <= ae->ptMButtonDown.y + 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMLeft);
        nResult=AECC_MLEFT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMLeft);
        nResult=AECC_MLEFT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMCenterTopBottom);
        nResult=AECC_MCENTER;
      }
    }
    else if (ptPos.x < ae->ptMButtonDown.x - 5 &&
             ptPos.y < ae->ptMButtonDown.y - 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMLeftTop);
        nResult=AECC_MLEFTTOP;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMLeft);
        nResult=AECC_MLEFT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMTop);
        nResult=AECC_MTOP;
      }
    }
    else if (ptPos.x >= ae->ptMButtonDown.x - 5 &&
             ptPos.x <= ae->ptMButtonDown.x + 5 &&
             ptPos.y < ae->ptMButtonDown.y - 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMTop);
        nResult=AECC_MTOP;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMCenterLeftRight);
        nResult=AECC_MCENTER;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMTop);
        nResult=AECC_MTOP;
      }
    }
    else if (ptPos.x > ae->ptMButtonDown.x + 5 &&
             ptPos.y < ae->ptMButtonDown.y - 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMRightTop);
        nResult=AECC_MRIGHTTOP;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMRight);
        nResult=AECC_MRIGHT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMTop);
        nResult=AECC_MTOP;
      }
    }
    else if (ptPos.x > ae->ptMButtonDown.x + 5 &&
             ptPos.y >= ae->ptMButtonDown.y - 5 &&
             ptPos.y <= ae->ptMButtonDown.y + 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMRight);
        nResult=AECC_MRIGHT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMRight);
        nResult=AECC_MRIGHT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMCenterTopBottom);
        nResult=AECC_MCENTER;
      }
    }
    else if (ptPos.x > ae->ptMButtonDown.x + 5 &&
             ptPos.y > ae->ptMButtonDown.y + 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMRightBottom);
        nResult=AECC_MRIGHTBOTTOM;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMRight);
        nResult=AECC_MRIGHT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMBottom);
        nResult=AECC_MBOTTOM;
      }
    }
    else if (ptPos.x >= ae->ptMButtonDown.x - 5 &&
             ptPos.x <= ae->ptMButtonDown.x + 5 &&
             ptPos.y > ae->ptMButtonDown.y + 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMBottom);
        nResult=AECC_MBOTTOM;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMCenterLeftRight);
        nResult=AECC_MCENTER;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMBottom);
        nResult=AECC_MBOTTOM;
      }
    }
    else if (ptPos.x < ae->ptMButtonDown.x - 5 &&
             ptPos.y > ae->ptMButtonDown.y + 5)
    {
      if (ae->hMButtonBitmap == hAkelEditBitmapMCenterAll)
      {
        SetCursor(hAkelEditCursorMLeftBottom);
        nResult=AECC_MLEFTBOTTOM;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterLeftRight)
      {
        SetCursor(hAkelEditCursorMLeft);
        nResult=AECC_MLEFT;
      }
      else if (ae->hMButtonBitmap == hAkelEditBitmapMCenterTopBottom)
      {
        SetCursor(hAkelEditCursorMBottom);
        nResult=AECC_MBOTTOM;
      }
    }
  }
  else if (ae->dwMouseMoveTimer)
  {
    SetCursor(hAkelEditCursorIBeam);
    nResult=AECC_IBEAM;
  }
  else
  {
    nResult=AECC_IBEAM;

    if (!bAlt && AE_IsPointOnMargin(ae, &ptPos) == AESIDE_LEFT)
    {
      if (!(ae->popt->dwOptions & AECO_NOMARGINSEL))
      {
        SetCursor(hAkelEditCursorMargin);
        nResult=AECC_MARGIN;
      }
    }
    else if (!bAlt && !bShift && AE_IsPointOnSelection(ae, &ptPos))
    {
      if (!(ae->popt->dwOptions & AECO_DISABLEDRAG))
      {
        SetCursor(hAkelEditCursorArrow);
        nResult=AECC_SELECTION;
      }
    }
    else if (!bAlt && !bShift && AE_IsPointOnUrl(ae, &ptPos, &ae->crMouseOnLink))
    {
      SetCursor(hAkelEditCursorHand);
      nResult=AECC_URL;
    }
  }
  return nResult;
}

DWORD AE_IsPointOnMargin(AKELEDIT *ae, const POINT *ptPos)
{
  DWORD dwSide=0;

  if (ptPos->x >= ae->rcEdit.left && ptPos->x < ae->rcDraw.left && ptPos->y > ae->rcEdit.top && ptPos->y < ae->rcEdit.bottom)
  {
    dwSide|=AESIDE_LEFT;
  }
  if (ptPos->y >= ae->rcEdit.top && ptPos->y < ae->rcDraw.top && ptPos->x > ae->rcEdit.left && ptPos->x < ae->rcEdit.right)
  {
    dwSide|=AESIDE_TOP;
  }
  if (ptPos->x > ae->rcDraw.right && ptPos->x <= ae->rcEdit.right && ptPos->y > ae->rcEdit.top && ptPos->y < ae->rcEdit.bottom)
  {
    dwSide|=AESIDE_RIGHT;
  }
  if (ptPos->y > ae->rcDraw.bottom && ptPos->y <= ae->rcEdit.bottom && ptPos->x > ae->rcEdit.left && ptPos->x < ae->rcEdit.right)
  {
    dwSide|=AESIDE_BOTTOM;
  }
  return dwSide;
}

BOOL AE_IsPointOnSelection(AKELEDIT *ae, const POINT *ptPos)
{
  AECHARINDEX ciCharIndex;
  POINT ptChar;
  int nSelStartY;
  int nSelEndY;
  int nResult;

  if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    nSelStartY=(AE_VPos(ae, ae->ciSelStartIndex.nLine, AEVPF_VPOSFROMLINE) - ae->nVScrollPos) + ae->rcDraw.top;
    nSelEndY=(AE_VPos(ae, ae->ciSelEndIndex.nLine + 1, AEVPF_VPOSFROMLINE) - ae->nVScrollPos) + ae->rcDraw.top;

    if (ptPos->y >= nSelStartY && ptPos->y <= nSelEndY)
    {
      if (nResult=AE_GetCharFromPos(ae, ptPos, &ciCharIndex, &ptChar, ae->bColumnSel))
      {
        if (ciCharIndex.lpLine->nSelStart != ciCharIndex.lpLine->nSelEnd &&
            (ciCharIndex.nCharInLine > ciCharIndex.lpLine->nSelStart ||
             ciCharIndex.nCharInLine == ciCharIndex.lpLine->nSelStart && (nResult == AEPC_BEFORE || nResult == AEPC_EQUAL)) &&
            (ciCharIndex.nCharInLine < ciCharIndex.lpLine->nSelEnd ||
             ciCharIndex.nCharInLine == ciCharIndex.lpLine->nSelEnd && (nResult == AEPC_AFTER || nResult == AEPC_EQUAL)))
        {
          return TRUE;
        }

        //Is on new line
        if (ciCharIndex.nLine >= ae->ciSelStartIndex.nLine &&
            ciCharIndex.nLine < ae->ciSelEndIndex.nLine &&
            ciCharIndex.nCharInLine == ciCharIndex.lpLine->nLineLen)
        {
          AE_GlobalToClient(ae, &ptChar, &ptChar);

          if (ptPos->x >= ptChar.x && ptPos->x <= ptChar.x + ae->ptxt->nAveCharWidth)
            return TRUE;
        }
      }
    }
  }
  return FALSE;
}

DWORD AE_IsPointOnUrl(AKELEDIT *ae, const POINT *ptPos, AECHARRANGE *crLink)
{
  if (ae->popt->bDetectUrl)
  {
    AECHARINDEX ciCharIndex;
    int nResult;

    if (PtInRect(&ae->rcDraw, *ptPos))
    {
      if (ptPos->y <= ae->ptxt->nVScrollMax)
      {
        if (nResult=AE_GetCharFromPos(ae, ptPos, &ciCharIndex, NULL, FALSE))
        {
          if (nResult == AEPC_AFTER)
          {
            AE_IndexDec(&ciCharIndex);
            ciCharIndex.nCharInLine=max(ciCharIndex.nCharInLine, 0);
          }
          xmemset(crLink, 0, sizeof(AECHARRANGE));
          return AE_HighlightFindUrl(ae, &ciCharIndex, AEHF_FINDFIRSTCHAR, ae->ptxt->nLineCount, crLink);
        }
      }
    }
  }
  return 0;
}

DWORD AE_HighlightFindUrl(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, int nLastLine, AECHARRANGE *crRange)
{
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  int nPrefix;
  DWORD dwLinkLen=0;
  DWORD dwBackwardMatch=0;

  crRange->ciMin.lpLine=NULL;
  crRange->ciMax.lpLine=NULL;
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen) return 0;

  ft.dwFlags=0;
  ft.pText=NULL;
  ft.dwTextLen=(DWORD)-1;
  ft.nNewLine=AELB_ASIS;

  //Find URL beginning (backward)
  ciCount=*ciChar;

  while (ciCount.lpLine)
  {
    while (ciCount.nCharInLine >= 0)
    {
      if (AE_IndexCompare(&ciCount, &crRange->ciMax) < 0)
        goto FindUrlEnding;
      dwLinkLen+=AE_IndexLen(&ciCount);
      if (dwLinkLen > ae->popt->dwUrlMaxLength)
        goto FindUrlEnding;
      if (AE_IsInDelimiterList(ae->popt->wszUrlRightDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine], TRUE))
        goto FindUrlEnding;

      if (AE_IsDelimiter(ae, &ciCount, AEDLM_URLLEFT|AEDLM_PREVCHAR))
      {
        for (nPrefix=0; ae->popt->lpUrlPrefixes[nPrefix]; ++nPrefix)
        {
          ft.pText=ae->popt->lpUrlPrefixes[nPrefix];

          if (AE_IsMatch(ae, &ft, &ciCount))
          {
            crRange->ciMin=ciCount;
            dwBackwardMatch=dwLinkLen;
            break;
          }
        }
      }
      if (dwSearchType & AEHF_ISFIRSTCHAR)
        goto FindUrlEnding;
      AE_IndexDec(&ciCount);
    }

    if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
    {
      AE_PrevLine(&ciCount);
      AE_IndexDec(&ciCount);
    }
    else goto FindUrlEnding;
  }

  //Find URL ending (forward)
  FindUrlEnding:
  if (!dwBackwardMatch) return 0;
  dwLinkLen=dwBackwardMatch;
  ciCount=*ciChar;
  AE_IndexInc(&ciCount);

  while (ciCount.nLine <= nLastLine)
  {
    while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
    {
      if (AE_IsInDelimiterList(ae->popt->wszUrlRightDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine], TRUE))
        goto End;
      dwLinkLen+=AE_IndexLen(&ciCount);
      if (dwLinkLen > ae->popt->dwUrlMaxLength)
        goto End;
      AE_IndexInc(&ciCount);
    }

    if (ciCount.lpLine->nLineBreak == AELB_WRAP)
      AE_NextLine(&ciCount);
    else
      goto End;
  }

  End:
  crRange->ciMax=ciCount;
  return dwLinkLen;
}

int AE_HighlightFindMarkText(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEMARKTEXTMATCH *mtm)
{
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  AEMARKTEXTITEMW *lpMarkTextElement=(AEMARKTEXTITEMW *)ae->popt->lpActiveTheme->hMarkTextStack.first;

  mtm->lpMarkText=NULL;
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen) return 0;

  if (lpMarkTextElement)
  {
    //Find mark beginning (backward)
    ciCount=*ciChar;

    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine >= 0)
      {
        if (AE_IndexCompare(&ciCount, &mtm->crMarkText.ciMax) < 0)
          return 0;

        //Is mark
        if (lpMarkTextElement=AE_HighlightIsMarkText(ae, &ft, &ciCount))
        {
          mtm->lpMarkText=lpMarkTextElement;
          mtm->crMarkText=ft.crFound;
          return mtm->lpMarkText->nMarkTextLen;
        }
        if (dwSearchType & AEHF_ISFIRSTCHAR)
          return 0;
        AE_IndexDec(&ciCount);
      }

      if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
      {
        AE_PrevLine(&ciCount);
        AE_IndexDec(&ciCount);
      }
      else break;
    }
  }
  return 0;
}

AEMARKTEXTITEMW* AE_HighlightIsMarkText(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar)
{
  AEMARKTEXTITEMW *lpMarkTextElement;
  AEFINDTEXTW ftMatch;

  if (!ft) ft=&ftMatch;

  lpMarkTextElement=(AEMARKTEXTITEMW *)ae->popt->lpActiveTheme->hMarkTextStack.first;

  while (lpMarkTextElement)
  {
    ft->pText=lpMarkTextElement->pMarkText;
    ft->dwTextLen=lpMarkTextElement->nMarkTextLen;
    ft->dwFlags=(lpMarkTextElement->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;
    ft->nNewLine=AELB_ASIS;

    if (AE_IsMatch(ae, ft, ciChar))
    {
      return lpMarkTextElement;
    }
    lpMarkTextElement=lpMarkTextElement->next;
  }
  return NULL;
}

int AE_HighlightFindMarkRange(AKELEDIT *ae, int nCharOffset, AEMARKRANGEMATCH *mrm)
{
  AEMARKRANGEITEM *lpMarkRangeElement;

  lpMarkRangeElement=(AEMARKRANGEITEM *)ae->popt->lpActiveTheme->hMarkRangeStack.first;

  while (lpMarkRangeElement)
  {
    if (nCharOffset >= lpMarkRangeElement->crMarkRange.cpMin &&
        nCharOffset < lpMarkRangeElement->crMarkRange.cpMax)
    {
      mrm->lpMarkRange=lpMarkRangeElement;
      mrm->crMarkRange=lpMarkRangeElement->crMarkRange;
      return mrm->crMarkRange.cpMax - mrm->crMarkRange.cpMin;
    }
    lpMarkRangeElement=lpMarkRangeElement->next;
  }
  return 0;
}

int AE_HighlightFindQuote(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEQUOTEMATCH *qm)
{
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  AECHARINDEX ciTmpCount;
  AECHARRANGE crTmpQuoteStart;
  AEQUOTEITEMW *lpQuoteElement=(AEQUOTEITEMW *)ae->popt->lpActiveTheme->hQuoteStack.first;
  AEDELIMITEMW *lpDelimiterElement=NULL;
  int nQuoteLen=0;
  int nTmpQuoteLen;

  qm->lpQuote=NULL;
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen && ciChar->lpLine->nLineBreak != AELB_WRAP)
    return 0;

  if (lpQuoteElement)
  {
    ft.dwFlags=0;
    ft.pText=NULL;
    ft.dwTextLen=(DWORD)-1;
    ft.nNewLine=AELB_ASIS;

    //Find quote beginning (backward)
    ciCount=*ciChar;
    if (dwSearchType & AEHF_FINDFIRSTCHAR)
    {
      AE_GetIndex(ae, AEGI_WRAPLINEBEGIN, &ciCount, &ciCount, FALSE);
      if (AE_IndexCompare(&qm->crQuoteEnd.ciMax, &ciCount) > 0)
        ciCount=qm->crQuoteEnd.ciMax;
    }

    Begin:
    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
      {
        if (!qm->lpQuote)
        {
          if (AE_IndexCompare(&ciCount, ciChar) > 0)
            return 0;
          if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
            break;

          for (lpQuoteElement=(AEQUOTEITEMW *)ae->popt->lpActiveTheme->hQuoteStack.first; lpQuoteElement; lpQuoteElement=lpQuoteElement->next)
          {
            //Quote start
            if (lpQuoteElement->dwFlags & AEHLF_QUOTESTART_ISDELIMITER)
            {
              if ((lpDelimiterElement=AE_HighlightIsDelimiter(ae, &ft, &ciCount, FALSE)) || AE_IsFirstCharInLine(&ciCount))
              {
                if (lpDelimiterElement)
                {
                  ciTmpCount=ft.crFound.ciMax;
                  nTmpQuoteLen=lpDelimiterElement->nDelimiterLen;
                  crTmpQuoteStart=ft.crFound;
                }
                else
                {
                  ciTmpCount=ciCount;
                  nTmpQuoteLen=0;
                  crTmpQuoteStart.ciMin=ciCount;
                  crTmpQuoteStart.ciMax=ciCount;
                }
                goto CheckQuoteStart;
              }
            }
            else
            {
              if (!lpQuoteElement->pQuoteStart || !*lpQuoteElement->pQuoteStart)
              {
                if (AE_IsFirstCharInLine(&ciCount))
                {
                  //Quote start is empty == ""
                  ciTmpCount=ciCount;
                  nTmpQuoteLen=0;
                  crTmpQuoteStart.ciMin=ciCount;
                  crTmpQuoteStart.ciMax=ciCount;
                  goto CheckQuoteStart;
                }
              }
              else
              {
                ft.pText=lpQuoteElement->pQuoteStart;
                ft.dwTextLen=lpQuoteElement->nQuoteStartLen;
                ft.dwFlags=(lpQuoteElement->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;

                if (AE_IsMatch(ae, &ft, &ciCount))
                {
                  if (!AE_IsEscaped(&ciCount, lpQuoteElement->chEscape))
                  {
                    if (!(lpQuoteElement->dwFlags & AEHLF_QUOTESTART_ISWORD) ||
                        ((AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMax, FALSE) || AE_IsLastCharInLine(&ft.crFound.ciMax)) &&
                         (AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMin, TRUE) || AE_IsFirstCharInLine(&ft.crFound.ciMin))))
                    {
                      ciTmpCount=ft.crFound.ciMax;
                      nTmpQuoteLen=lpQuoteElement->nQuoteStartLen;
                      crTmpQuoteStart=ft.crFound;
                      goto CheckQuoteStart;
                    }
                  }
                }
              }
            }
            continue;

            CheckQuoteStart:
            if (lpQuoteElement->dwFlags & AEHLF_ATLINESTART)
            {
              if (!AE_IsSpacesFromLeft(&crTmpQuoteStart.ciMin))
                continue;
            }
            ciCount=ciTmpCount;
            nQuoteLen=nTmpQuoteLen;
            qm->crQuoteStart=crTmpQuoteStart;
            qm->lpQuote=lpQuoteElement;
            goto Begin;

            QuoteStartNext:
            ciCount=qm->crQuoteStart.ciMin;
            nQuoteLen=0;
            qm->crQuoteStart.ciMax=qm->crQuoteStart.ciMin;
            qm->crQuoteEnd=qm->crQuoteStart;
            qm->lpQuote=NULL;
          }
          if (dwSearchType & AEHF_ISFIRSTCHAR)
            return 0;
        }
        else
        {
          //Quote end
          if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_ISDELIMITER)
          {
            if ((lpDelimiterElement=AE_HighlightIsDelimiter(ae, &ft, &ciCount, FALSE)) || AE_IsLastCharInLine(&ciCount))
            {
              if (lpDelimiterElement)
              {
                nQuoteLen+=lpDelimiterElement->nDelimiterLen;
                qm->crQuoteEnd=ft.crFound;
                ciCount=qm->crQuoteEnd.ciMax;
              }
              else
              {
                qm->crQuoteEnd.ciMin=ciCount;
                qm->crQuoteEnd.ciMax=ciCount;
              }
              goto SetQuote;
            }
          }
          else
          {
            if (!qm->lpQuote->pQuoteEnd || !*qm->lpQuote->pQuoteEnd)
            {
              nQuoteLen+=(int)AE_GetIndex(ae, AEGI_WRAPLINEEND, &ciCount, &ciCount, FALSE);
              qm->crQuoteEnd.ciMin=ciCount;
              qm->crQuoteEnd.ciMax=ciCount;
              goto SetQuote;
            }
            else
            {
              ft.pText=qm->lpQuote->pQuoteEnd;
              ft.dwTextLen=qm->lpQuote->nQuoteEndLen;
              ft.dwFlags=(qm->lpQuote->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;

              if (AE_IsMatch(ae, &ft, &ciCount))
              {
                if (!AE_IsEscaped(&ciCount, qm->lpQuote->chEscape))
                {
                  if (!(qm->lpQuote->dwFlags & AEHLF_QUOTEEND_ISWORD) ||
                      ((AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMax, FALSE) || AE_IsLastCharInLine(&ft.crFound.ciMax)) &&
                       (AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMin, TRUE) || AE_IsFirstCharInLine(&ft.crFound.ciMin))))
                  {
                    nQuoteLen+=qm->lpQuote->nQuoteEndLen;
                    qm->crQuoteEnd=ft.crFound;
                    ciCount=qm->crQuoteEnd.ciMax;
                    goto SetQuote;
                  }
                }
              }
              if (qm->lpQuote->dwFlags & AEHLF_QUOTESTART_ISDELIMITER)
              {
                if (AE_HighlightIsDelimiter(ae, NULL, &ciCount, FALSE))
                  goto QuoteStartNext;
              }
              if (qm->lpQuote->dwFlags & AEHLF_QUOTEWITHOUTDELIMITERS)
              {
                if (AE_HighlightIsDelimiter(ae, NULL, &ciCount, FALSE))
                  goto QuoteStartNext;
              }
            }
          }
          if (qm->lpQuote->dwFlags & AEHLF_QUOTEINCLUDE)
          {
            if (qm->lpQuote->pQuoteInclude && *qm->lpQuote->pQuoteInclude)
            {
              if (!AE_IsInDelimiterList(qm->lpQuote->pQuoteInclude, ciCount.lpLine->wpLine[ciCount.nCharInLine], (qm->lpQuote->dwFlags & AEHLF_MATCHCASE)))
                goto QuoteStartNext;
            }
          }
          if (qm->lpQuote->dwFlags & AEHLF_QUOTEEXCLUDE)
          {
            if (qm->lpQuote->pQuoteExclude && *qm->lpQuote->pQuoteExclude)
            {
              if (AE_IsInDelimiterList(qm->lpQuote->pQuoteExclude, ciCount.lpLine->wpLine[ciCount.nCharInLine], (qm->lpQuote->dwFlags & AEHLF_MATCHCASE)))
                goto QuoteStartNext;
            }
          }

          if (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
            nQuoteLen+=AE_IndexLen(&ciCount);
        }
        AE_IndexInc(&ciCount);
      }

      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AE_NextLine(&ciCount);
      else
        break;
    }
    if (qm->lpQuote)
    {
      if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_REQUIRED)
      {
        goto QuoteStartNext;
      }
      else
      {
        qm->crQuoteEnd.ciMin=ciCount;
        qm->crQuoteEnd.ciMax=ciCount;
      }
    }
    goto End;

    SetQuote:
    if (qm->lpQuote)
    {
      if (qm->lpQuote->dwFlags & AEHLF_ATLINEEND)
      {
        if (!AE_IsSpacesFromRight(&qm->crQuoteEnd.ciMax))
          goto QuoteStartNext;
      }

      if (dwSearchType & AEHF_FINDFIRSTCHAR)
      {
        if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_NOMATCH)
          ciCount=qm->crQuoteEnd.ciMin;

        if (AE_IndexCompare(&ciCount, ciChar) <= 0)
        {
          qm->lpQuote=NULL;

          if (AE_IndexCompare(&ciCount, &qm->crQuoteStart.ciMin) <= 0)
            goto QuoteStartNext;
          else
            goto Begin;
        }
      }
    }

    End:
    if (qm->lpQuote)
    {
      if (qm->lpQuote->dwFlags & AEHLF_QUOTESTART_NOCATCH)
      {
        qm->crQuoteStart.ciMin=qm->crQuoteStart.ciMax;
        nQuoteLen-=qm->lpQuote->nQuoteStartLen;
      }
      if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_NOMATCH)
      {
        qm->crQuoteEnd.ciMax=qm->crQuoteEnd.ciMin;
        nQuoteLen-=qm->lpQuote->nQuoteEndLen;
      }
    }
  }
  return nQuoteLen;
}

int AE_HighlightFindWord(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEWORDMATCH *wm, AEQUOTEMATCH *qm)
{
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  AEWORDITEMW *lpWordElement=(AEWORDITEMW *)ae->popt->lpActiveTheme->hWordStack.first;
  AEDELIMITEMW *lpDelimiterElement=(AEDELIMITEMW *)ae->popt->lpActiveTheme->hDelimiterStack.first;
  int nWordLen=0;

  wm->lpDelim1=NULL;
  wm->lpWord=NULL;
  wm->lpDelim2=NULL;
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen) return 0;

  if (lpWordElement || lpDelimiterElement)
  {
    ft.dwFlags=0;
    ft.pText=NULL;
    ft.dwTextLen=(DWORD)-1;
    ft.nNewLine=AELB_ASIS;

    //Find word beginning (backward)
    ciCount=*ciChar;

    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine >= 0)
      {
        nWordLen+=AE_IndexLen(&ciCount);
        if (nWordLen > AEMAX_WORDLENGTH)
          return 0;
        if (AE_IndexCompare(&ciCount, &qm->crQuoteEnd.ciMax) < 0 &&
            AE_IndexCompare(&ciCount, &qm->crQuoteStart.ciMax) >= 0)
          return 0;

        //Is delimiter
        if (lpDelimiterElement=AE_HighlightIsDelimiter(ae, &ft, &ciCount, FALSE))
        {
          if (lpDelimiterElement->dwFlags & AEHLF_ATLINESTART)
          {
            if (!AE_IsSpacesFromLeft(&ft.crFound.ciMin))
              goto PrevChar;
          }
          if (lpDelimiterElement && lpDelimiterElement->dwFlags & AEHLF_ATLINEEND)
          {
            if (!AE_IsSpacesFromRight(&ft.crFound.ciMax))
              goto PrevChar;
          }
          wm->lpDelim1=lpDelimiterElement;
          wm->crDelim1=ft.crFound;
          nWordLen=max(nWordLen - wm->lpDelim1->nDelimiterLen, 0);
          goto FindWordEnding;
        }

        PrevChar:
        if (AE_IndexCompare(&ciCount, &wm->crDelim2.ciMax) <= 0)
          goto SetEmptyFirstDelim;
        if (dwSearchType & AEHF_ISFIRSTCHAR)
          return 0;
        AE_IndexDec(&ciCount);
      }

      if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
      {
        AE_PrevLine(&ciCount);
        AE_IndexDec(&ciCount);
      }
      else break;
    }

    //Empty delimiter
    SetEmptyFirstDelim:
    wm->crDelim1.ciMin.nLine=ciCount.nLine;
    wm->crDelim1.ciMin.lpLine=ciCount.lpLine;
    wm->crDelim1.ciMin.nCharInLine=max(ciCount.nCharInLine, 0);
    wm->crDelim1.ciMax=wm->crDelim1.ciMin;

    //Find word ending (forward)
    FindWordEnding:
    if (AE_IndexCompare(&wm->crDelim1.ciMax, ciChar) > 0)
    {
      ciCount=wm->crDelim1.ciMax;
      nWordLen=0;
    }
    else
    {
      ciCount=*ciChar;
      AE_IndexInc(&ciCount);
    }

    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
      {
        //Is delimiter
        if (lpDelimiterElement=AE_HighlightIsDelimiter(ae, &ft, &ciCount, FALSE))
        {
          if (lpDelimiterElement->dwFlags & AEHLF_ATLINESTART)
          {
            if (!AE_IsSpacesFromLeft(&ft.crFound.ciMin))
              goto NextChar;
          }
          if (lpDelimiterElement->dwFlags & AEHLF_ATLINEEND)
          {
            if (!AE_IsSpacesFromRight(&ft.crFound.ciMax))
              goto NextChar;
          }
          if (!nWordLen)
          {
            wm->crDelim2.ciMin=wm->crDelim1.ciMax;
            wm->crDelim2.ciMax=wm->crDelim1.ciMax;
          }
          else
          {
            wm->lpDelim2=lpDelimiterElement;
            wm->crDelim2=ft.crFound;
          }
          goto SetWord;
        }

        NextChar:
        nWordLen+=AE_IndexLen(&ciCount);
        if (nWordLen > AEMAX_WORDLENGTH)
          return 0;
        AE_IndexInc(&ciCount);
      }

      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AE_NextLine(&ciCount);
      else
        break;
    }

    //Empty delimiter
    wm->crDelim2.ciMin.nLine=ciCount.nLine;
    wm->crDelim2.ciMin.lpLine=ciCount.lpLine;
    wm->crDelim2.ciMin.nCharInLine=ciCount.lpLine->nLineLen;
    wm->crDelim2.ciMax=wm->crDelim2.ciMin;

    SetWord:
    wm->crWord.ciMin=wm->crDelim1.ciMax;
    wm->crWord.ciMax=wm->crDelim2.ciMin;
    wm->lpWord=AE_HighlightIsWord(ae, NULL, &wm->crWord, nWordLen);
  }
  return nWordLen;
}

AEDELIMITEMW* AE_HighlightIsDelimiter(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, BOOL bBack)
{
  AEDELIMITEMW *lpDelimiterElement;
  AECHARINDEX ciDelimStart=*ciChar;
  AEFINDTEXTW ftMatch;

  if (!ft) ft=&ftMatch;

  lpDelimiterElement=(AEDELIMITEMW *)ae->popt->lpActiveTheme->hDelimiterStack.first;

  while (lpDelimiterElement)
  {
    ft->pText=lpDelimiterElement->pDelimiter;
    ft->dwTextLen=lpDelimiterElement->nDelimiterLen;
    ft->dwFlags=(lpDelimiterElement->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;
    ft->nNewLine=AELB_ASIS;

    if (!bBack || AE_IndexOffset(ae, ciChar, &ciDelimStart, -(int)ft->dwTextLen, ft->nNewLine) == ft->dwTextLen)
    {
      if (AE_IsMatch(ae, ft, &ciDelimStart))
      {
        return lpDelimiterElement;
      }
    }
    lpDelimiterElement=lpDelimiterElement->next;
  }
  return NULL;
}

AEWORDITEMW* AE_HighlightIsWord(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARRANGE *crWord, int nWordLen)
{
  AEWORDITEMW *lpWordElement;
  AEFINDTEXTW ftMatch;
  AECHARINDEX ciCount;

  if (!nWordLen) return NULL;
  if (!ft) ft=&ftMatch;

  if ((DWORD)nWordLen < sizeof(ae->popt->lpActiveTheme->hWordStack.lpWordLens) / sizeof(int))
  {
    //Composition words
    lpWordElement=(AEWORDITEMW *)ae->popt->lpActiveTheme->hWordStack.lpWordLens[0];

    while (lpWordElement)
    {
      if (lpWordElement->nWordLen == 0)
      {
        if (lpWordElement->dwFlags & AEHLF_WORDCOMPOSITION)
        {
          ciCount=crWord->ciMin;

          while (AE_IndexCompare(&ciCount, &crWord->ciMax) < 0)
          {
            if (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
            {
              if (!AE_IsInDelimiterList(lpWordElement->pWord, ciCount.lpLine->wpLine[ciCount.nCharInLine], (lpWordElement->dwFlags & AEHLF_MATCHCASE)))
                break;
              AE_IndexInc(&ciCount);
            }
            else
            {
              if (ciCount.lpLine->nLineBreak == AELB_WRAP)
                AE_NextLine(&ciCount);
              else
                break;
            }
          }
          if (!AE_IndexCompare(&ciCount, &crWord->ciMax))
            goto CheckWord;
        }
      }
      else break;

      lpWordElement=lpWordElement->next;
    }

    //Standard words
    lpWordElement=(AEWORDITEMW *)ae->popt->lpActiveTheme->hWordStack.lpWordLens[nWordLen];

    while (lpWordElement)
    {
      if (lpWordElement->nWordLen == nWordLen)
      {
        ft->pText=lpWordElement->pWord;
        ft->dwTextLen=lpWordElement->nWordLen;
        ft->dwFlags=(lpWordElement->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;
        ft->nNewLine=AELB_ASIS;

        if (AE_IsMatch(ae, ft, &crWord->ciMin))
        {
          goto CheckWord;
        }
      }
      else break;

      lpWordElement=lpWordElement->next;
    }
  }
  return NULL;

  CheckWord:
  if (lpWordElement->dwFlags & AEHLF_ATLINESTART)
  {
    if (!AE_IsSpacesFromLeft(&crWord->ciMin))
      return NULL;
  }
  if (lpWordElement->dwFlags & AEHLF_ATLINEEND)
  {
    if (!AE_IsSpacesFromRight(&crWord->ciMax))
      return NULL;
  }
  return lpWordElement;
}

AETHEMEITEMW* AE_HighlightCreateTheme(wchar_t *wpThemeName)
{
  AETHEMEITEMW *lpElement=NULL;

  if (*wpThemeName)
  {
    if (!AE_HeapStackInsertIndex(NULL, (stack **)&hAkelEditThemesStack.first, (stack **)&hAkelEditThemesStack.last, (stack **)&lpElement, -1, sizeof(AETHEMEITEMW)))
    {
      xstrcpynW(lpElement->wszThemeName, wpThemeName, MAX_PATH);
    }
  }
  return lpElement;
}

AETHEMEITEMW* AE_HighlightGetTheme(wchar_t *wpThemeName)
{
  AETHEMEITEMW *lpElement=(AETHEMEITEMW *)hAkelEditThemesStack.first;

  while (lpElement)
  {
    if (!xstrcmpiW(lpElement->wszThemeName, wpThemeName))
      return lpElement;

    lpElement=lpElement->next;
  }
  return NULL;
}

BOOL AE_HighlightIsThemeExists(AETHEMEITEMW *aeti)
{
  AETHEMEITEMW *lpElement=(AETHEMEITEMW *)hAkelEditThemesStack.first;

  while (lpElement)
  {
    if (lpElement == aeti)
      return TRUE;

    lpElement=lpElement->next;
  }
  return FALSE;
}

void AE_HighlightDeleteTheme(AETHEMEITEMW *aeti)
{
  AE_HighlightDeleteDelimiterAll(aeti);
  AE_HighlightDeleteWordAll(aeti);
  AE_HighlightDeleteQuoteAll(aeti);
  AE_HighlightDeleteMarkTextAll(aeti);
  AE_HighlightDeleteMarkRangeAll(aeti);
  AE_HeapStackDelete(NULL, (stack **)&hAkelEditThemesStack.first, (stack **)&hAkelEditThemesStack.last, (stack *)aeti);
}

void AE_HighlightDeleteThemeAll()
{
  AETHEMEITEMW *lpElement=(AETHEMEITEMW *)hAkelEditThemesStack.first;

  while (lpElement)
  {
    AE_HighlightDeleteDelimiterAll(lpElement);
    AE_HighlightDeleteWordAll(lpElement);
    AE_HighlightDeleteQuoteAll(lpElement);
    AE_HighlightDeleteMarkTextAll(lpElement);
    AE_HighlightDeleteMarkRangeAll(lpElement);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&hAkelEditThemesStack.first, (stack **)&hAkelEditThemesStack.last);
}

void AE_HighlightUnsetTheme(AETHEMEITEMW *aeti)
{
  AKELEDIT *lpElement=(AKELEDIT *)hAkelEditWindowsStack.first;

  while (lpElement)
  {
    if (!aeti || lpElement->popt->lpActiveTheme == aeti)
      lpElement->popt->lpActiveTheme=NULL;

    lpElement=lpElement->next;
  }
}

AEDELIMITEMW* AE_HighlightInsertDelimiter(AETHEMEITEMW *aeti, int nDelimiterLen, int nIndex)
{
  AEDELIMITEMW *lpElement=NULL;

  AE_HeapStackInsertIndex(NULL, (stack **)&aeti->hDelimiterStack.first, (stack **)&aeti->hDelimiterStack.last, (stack **)&lpElement, nIndex, sizeof(AEDELIMITEMW));
  return lpElement;
}

void AE_HighlightDeleteDelimiter(AETHEMEITEMW *aeti, AEDELIMITEMW *aedi)
{
  if (aedi->pDelimiter) AE_HeapFree(NULL, 0, (LPVOID)aedi->pDelimiter);
  AE_HeapStackDelete(NULL, (stack **)&aeti->hDelimiterStack.first, (stack **)&aeti->hDelimiterStack.last, (stack *)aedi);
}

void AE_HighlightDeleteDelimiterAll(AETHEMEITEMW *aeti)
{
  AEDELIMITEMW *lpElement=(AEDELIMITEMW *)aeti->hDelimiterStack.first;

  while (lpElement)
  {
    if (lpElement->pDelimiter) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pDelimiter);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&aeti->hDelimiterStack.first, (stack **)&aeti->hDelimiterStack.last);
}

AEWORDITEMW* AE_HighlightInsertWord(AETHEMEITEMW *aeti, int nWordLen)
{
  AEWORDITEMW *lpTmp;
  AEWORDITEMW *lpElement=NULL;

  if ((DWORD)nWordLen < sizeof(aeti->hWordStack.lpWordLens) / sizeof(int))
  {
    if (aeti->hWordStack.lpWordLens[nWordLen])
    {
      lpTmp=(AEWORDITEMW *)aeti->hWordStack.lpWordLens[nWordLen];
    }
    else
    {
      lpTmp=(AEWORDITEMW *)aeti->hWordStack.first;

      while (lpTmp)
      {
        if (lpTmp->nWordLen >= nWordLen)
          break;

        lpTmp=lpTmp->next;
      }
    }
    AE_HeapStackInsertBefore(NULL, (stack **)&aeti->hWordStack.first, (stack **)&aeti->hWordStack.last, (stack *)lpTmp, (stack **)&lpElement, sizeof(AEWORDITEMW));

    if (lpElement)
      aeti->hWordStack.lpWordLens[nWordLen]=(int)lpElement;
  }
  return lpElement;
}

void AE_HighlightDeleteWord(AETHEMEITEMW *aeti, AEWORDITEMW *aewi)
{
  if (aeti->hWordStack.lpWordLens[aewi->nWordLen] == (int)aewi)
  {
    //Word item is first in length group
    if (aewi->next && aewi->next->nWordLen == aewi->nWordLen)
      aeti->hWordStack.lpWordLens[aewi->nWordLen]=(int)aewi->next;
    else
      aeti->hWordStack.lpWordLens[aewi->nWordLen]=0;
  }
  if (aewi->pWord) AE_HeapFree(NULL, 0, (LPVOID)aewi->pWord);
  AE_HeapStackDelete(NULL, (stack **)&aeti->hWordStack.first, (stack **)&aeti->hWordStack.last, (stack *)aewi);
}

void AE_HighlightDeleteWordAll(AETHEMEITEMW *aeti)
{
  AEWORDITEMW *lpElement=(AEWORDITEMW *)aeti->hWordStack.first;

  while (lpElement)
  {
    if (lpElement->pWord) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pWord);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&aeti->hWordStack.first, (stack **)&aeti->hWordStack.last);
  xmemset(aeti->hWordStack.lpWordLens, 0, MAX_PATH * sizeof(int));
}

AEQUOTEITEMW* AE_HighlightInsertQuote(AETHEMEITEMW *aeti, int nIndex)
{
  AEQUOTEITEMW *lpElement=NULL;

  AE_HeapStackInsertIndex(NULL, (stack **)&aeti->hQuoteStack.first, (stack **)&aeti->hQuoteStack.last, (stack **)&lpElement, nIndex, sizeof(AEQUOTEITEMW));
  return lpElement;
}

void AE_HighlightDeleteQuote(AETHEMEITEMW *aeti, AEQUOTEITEMW *aeqi)
{
  if (aeqi->pQuoteStart) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteStart);
  if (aeqi->pQuoteEnd) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteEnd);
  if (aeqi->pQuoteInclude) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteInclude);
  if (aeqi->pQuoteExclude) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteExclude);
  AE_HeapStackDelete(NULL, (stack **)&aeti->hQuoteStack.first, (stack **)&aeti->hQuoteStack.last, (stack *)aeqi);
}

void AE_HighlightDeleteQuoteAll(AETHEMEITEMW *aeti)
{
  AEQUOTEITEMW *lpElement=(AEQUOTEITEMW *)aeti->hQuoteStack.first;

  while (lpElement)
  {
    if (lpElement->pQuoteStart) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pQuoteStart);
    if (lpElement->pQuoteEnd) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pQuoteEnd);
    if (lpElement->pQuoteInclude) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pQuoteInclude);
    if (lpElement->pQuoteExclude) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pQuoteExclude);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&aeti->hQuoteStack.first, (stack **)&aeti->hQuoteStack.last);
}

AEMARKTEXTITEMW* AE_HighlightInsertMarkText(AETHEMEITEMW *aeti, int nIndex)
{
  AEMARKTEXTITEMW *lpElement=NULL;

  AE_HeapStackInsertIndex(NULL, (stack **)&aeti->hMarkTextStack.first, (stack **)&aeti->hMarkTextStack.last, (stack **)&lpElement, nIndex, sizeof(AEMARKTEXTITEMW));
  return lpElement;
}

void AE_HighlightDeleteMarkText(AETHEMEITEMW *aeti, AEMARKTEXTITEMW *aemti)
{
  if (aemti->pMarkText) AE_HeapFree(NULL, 0, (LPVOID)aemti->pMarkText);
  AE_HeapStackDelete(NULL, (stack **)&aeti->hMarkTextStack.first, (stack **)&aeti->hMarkTextStack.last, (stack *)aemti);
}

void AE_HighlightDeleteMarkTextAll(AETHEMEITEMW *aeti)
{
  AEMARKTEXTITEMW *lpElement=(AEMARKTEXTITEMW *)aeti->hMarkTextStack.first;

  while (lpElement)
  {
    if (lpElement->pMarkText) AE_HeapFree(NULL, 0, (LPVOID)lpElement->pMarkText);

    lpElement=lpElement->next;
  }
  AE_HeapStackClear(NULL, (stack **)&aeti->hMarkTextStack.first, (stack **)&aeti->hMarkTextStack.last);
}

AEMARKRANGEITEM* AE_HighlightInsertMarkRange(AETHEMEITEMW *aeti, int nIndex)
{
  AEMARKRANGEITEM *lpElement=NULL;

  AE_HeapStackInsertIndex(NULL, (stack **)&aeti->hMarkRangeStack.first, (stack **)&aeti->hMarkRangeStack.last, (stack **)&lpElement, nIndex, sizeof(AEMARKRANGEITEM));
  return lpElement;
}

void AE_HighlightDeleteMarkRange(AETHEMEITEMW *aeti, AEMARKRANGEITEM *aemri)
{
  AE_HeapStackDelete(NULL, (stack **)&aeti->hMarkRangeStack.first, (stack **)&aeti->hMarkRangeStack.last, (stack *)aemri);
}

void AE_HighlightDeleteMarkRangeAll(AETHEMEITEMW *aeti)
{
  AE_HeapStackClear(NULL, (stack **)&aeti->hMarkRangeStack.first, (stack **)&aeti->hMarkRangeStack.last);
}

HBITMAP AE_CreateBitmap(int nWidth, int nHeight, COLORREF crBasic, COLORREF crInvert, BOOL bZebra)
{
  BITMAPFILEHEADER *lpBmpFileHeader;
  BITMAPINFOHEADER *lpBmpInfoHeader;
  BYTE *lpBitmapBits;
  BYTE *lpBmpFileData;
  HBITMAP hBitmap=NULL;
  int nBmpFileData;
  DWORD a;
  int b;

  //Bitmap data size
  nBmpFileData=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  if ((nWidth * 3) % 4 == 0)
    nBmpFileData+=(nWidth * 3) * nHeight;
  else
    nBmpFileData+=((nWidth * 3) + (4 - ((nWidth * 3) % 4))) * nHeight;

  //Allocate bitmap data
  if (lpBmpFileData=(BYTE *)AE_HeapAlloc(NULL, 0, nBmpFileData))
  {
    lpBmpFileHeader=(BITMAPFILEHEADER *)lpBmpFileData;
    lpBmpInfoHeader=(BITMAPINFOHEADER *)(lpBmpFileData + sizeof(BITMAPFILEHEADER));
    lpBitmapBits=(BYTE *)(lpBmpFileData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

    lpBmpFileHeader->bfType=0x4D42;
    lpBmpFileHeader->bfSize=nBmpFileData;
    lpBmpFileHeader->bfReserved1=0;
    lpBmpFileHeader->bfReserved2=0;
    lpBmpFileHeader->bfOffBits=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    lpBmpInfoHeader->biSize=sizeof(BITMAPINFOHEADER);
    lpBmpInfoHeader->biWidth=nWidth;
    lpBmpInfoHeader->biHeight=nHeight;
    lpBmpInfoHeader->biPlanes=1;
    lpBmpInfoHeader->biBitCount=24;
    lpBmpInfoHeader->biCompression=BI_RGB;
    lpBmpInfoHeader->biSizeImage=lpBmpFileHeader->bfSize - lpBmpFileHeader->bfOffBits;
    lpBmpInfoHeader->biXPelsPerMeter=0;
    lpBmpInfoHeader->biYPelsPerMeter=0;
    lpBmpInfoHeader->biClrUsed=0;
    lpBmpInfoHeader->biClrImportant=0;

    for (a=0; a < lpBmpInfoHeader->biSizeImage;)
    {
      //Fill line
      for (b=0; b < lpBmpInfoHeader->biWidth * 3; b+=3)
      {
        lpBitmapBits[a + 0]=mod(GetBValue(crInvert) - GetBValue(crBasic));
        lpBitmapBits[a + 1]=mod(GetGValue(crInvert) - GetGValue(crBasic));
        lpBitmapBits[a + 2]=mod(GetRValue(crInvert) - GetRValue(crBasic));
        a+=3;
      }
      while (a % 4) lpBitmapBits[a++]=0x00;

      if (bZebra)
      {
        for (b=0; b < lpBmpInfoHeader->biWidth * 3; b+=3)
        {
          lpBitmapBits[a + 0]=0x00;
          lpBitmapBits[a + 1]=0x00;
          lpBitmapBits[a + 2]=0x00;
          a+=3;
        }
        while (a % 4) lpBitmapBits[a++]=0x00;
      }
    }

    hBitmap=AE_LoadBitmapFromMemory((unsigned char *)lpBmpFileData);

    AE_HeapFree(NULL, 0, (LPVOID)lpBmpFileData);
  }
  return hBitmap;
}

HBITMAP AE_LoadBitmapFromMemory(const BYTE *lpBmpFileData)
{
  BITMAPFILEHEADER *lpBmpFileHeader=(BITMAPFILEHEADER *)lpBmpFileData;
  BITMAPINFOHEADER *lpBmpInfoHeader=(BITMAPINFOHEADER *)(lpBmpFileData + sizeof(BITMAPFILEHEADER));
  BYTE *lpBitmapBits=(BYTE *)(lpBmpFileData + lpBmpFileHeader->bfOffBits);
  BYTE *lpSectionBits=NULL;
  BITMAPINFO bi={0};
  HBITMAP hBitmap=NULL;
  DWORD a;
  int b;

  bi.bmiHeader=*lpBmpInfoHeader;

  if (hBitmap=CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void **)&lpSectionBits, NULL, 0))
  {
    for (a=0; a < bi.bmiHeader.biSizeImage;)
    {
      for (b=0; b < bi.bmiHeader.biWidth * 3; b+=3)
      {
        //Copy bits
        lpSectionBits[a + 0]=lpBitmapBits[a + 0];
        lpSectionBits[a + 1]=lpBitmapBits[a + 1];
        lpSectionBits[a + 2]=lpBitmapBits[a + 2];
        a+=3;
      }
      while (a % 4) lpSectionBits[a++]=0x00;
    }
  }
  return hBitmap;
}

BOOL AE_UpdateCaret(AKELEDIT *ae, BOOL bFocus)
{
  AEBITMAPITEM *bi;
  AEBITMAPDATA bd;
  HBITMAP hCaretBitmap=NULL;
  int nCaretWidth;
  int nCaretHeight;

  if (!(ae->popt->dwLockUpdate & AELU_CARET))
  {
    if (bFocus)
    {
      DestroyCaret();
    }

    if (!ae->popt->bOverType)
    {
      nCaretWidth=ae->popt->nCaretInsertWidth;
      nCaretHeight=ae->ptxt->nCharHeight;
    }
    else
    {
      nCaretWidth=ae->ptxt->nAveCharWidth;
      nCaretHeight=ae->popt->nCaretOvertypeHeight;
    }

    if (ae->popt->crCaret != RGB(0x00, 0x00, 0x00))
    {
      bd.nWidth=nCaretWidth;
      bd.nHeight=nCaretHeight;
      bd.crBasic=ae->popt->crCaret;
      bd.crInvert=ae->popt->crActiveLineBk;
      bd.bZebra=FALSE;

      if (!(bi=AE_StackBitmapItemGet(&hAkelEditBitmapsStack, &bd)))
        bi=AE_StackBitmapItemInsert(&hAkelEditBitmapsStack, &bd);
      hCaretBitmap=bi->hBitmap;

      if (!ae->popt->bOverType)
        ae->popt->hCaretInsert=hCaretBitmap;
      else
        ae->popt->hCaretOvertype=hCaretBitmap;
    }

    if (bFocus)
    {
      if (CreateCaret(ae->hWndEdit, (HBITMAP)hCaretBitmap, nCaretWidth, nCaretHeight))
      {
        ae->bCaretVisible=FALSE;
        AE_SetCaretPos(ae, &ae->ptCaret);
      }
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_SetCaretPos(AKELEDIT *ae, const POINT *ptCaret)
{
  POINT ptClient;
  BOOL bResult=FALSE;

  AE_GlobalToClient(ae, ptCaret, &ptClient);

  if (!(ae->popt->dwLockUpdate & AELU_CARET))
  {
    if (ae->popt->bOverType)
      bResult=SetCaretPos(ptClient.x, ptClient.y + max(ae->ptxt->nCharHeight - ae->popt->nCaretOvertypeHeight, 0));
    else
      bResult=SetCaretPos(ptClient.x, ptClient.y);
  }

  AE_SetCaretVis(ae, ptCaret);
  return bResult;
}

void AE_SetCaretVis(AKELEDIT *ae, const POINT *ptCaret)
{
  POINT ptClient;

  AE_GlobalToClient(ae, ptCaret, &ptClient);

  if (ptClient.x < ae->rcDraw.left ||
      ptClient.x > ae->rcDraw.right ||
      ptClient.y < ae->rcDraw.top - ae->ptxt->nCharHeight ||
      ptClient.y > ae->rcDraw.bottom)
  {
    if (ae->bCaretVisible)
    {
      if (!(ae->popt->dwLockUpdate & AELU_CARET))
        HideCaret(ae->hWndEdit);
      ae->bCaretVisible=FALSE;
    }
  }
  else
  {
    if (!ae->bCaretVisible)
    {
      if (!(ae->popt->dwLockUpdate & AELU_CARET))
        ShowCaret(ae->hWndEdit);
      ae->bCaretVisible=TRUE;
    }
  }
}

void AE_ScrollToCaret(AKELEDIT *ae, const POINT *ptCaret, BOOL bVertCorrect)
{
  //X
  if (ae->bDropping)
  {
    if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth * 3)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + ae->ptxt->nAveCharWidth * 2 + 1, 0));
    }
    else if (ptCaret->x < ae->nHScrollPos + ae->ptxt->nAveCharWidth * 2)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - ae->ptxt->nAveCharWidth * 2, 0));
    }
  }
  else if (ae->dwMouseMoveTimer)
  {
    if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + 1, 0));
    }
    else if (ptCaret->x < ae->nHScrollPos)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ptCaret->x);
    }
  }
  else
  {
    if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + (ae->rcDraw.right - ae->rcDraw.left) / 3, 0));
    }
    else if (ptCaret->x < ae->nHScrollPos)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) / 3, 0));
    }
  }

  //Y
  if (ae->bDropping)
  {
    if (ptCaret->y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - ae->ptxt->nCharHeight * 2)
    {
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->ptxt->nCharHeight * 3 + 1, 0));
    }
    else if (ptCaret->y < ae->nVScrollPos + ae->ptxt->nCharHeight)
    {
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - ae->ptxt->nCharHeight * 2, 0));
    }
  }
  else
  {
    if (ptCaret->y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - ae->ptxt->nCharHeight)
    {
      if (bVertCorrect && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex) && !AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelStartIndex))
        AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->ptxt->nCharHeight * 2 + 1, 0));
      else
        AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->ptxt->nCharHeight + 1, 0));
    }
    else if (ptCaret->y < ae->nVScrollPos)
    {
      if (bVertCorrect && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex) && !AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex))
        AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - ae->ptxt->nCharHeight, 0));
      else
        AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y, 0));
    }
  }
}

DWORD AE_ScrollToPoint(AKELEDIT *ae, POINT *ptPosition)
{
  DWORD dwTest;

  //Test scroll
  dwTest=AE_ScrollToPointEx(ae, AESC_TEST|(ptPosition?AESC_POINTCLIENT:AESC_POINTCARET)|AESC_OFFSETCHARX|AESC_OFFSETCHARY, ptPosition, 1, 0);

  //Scroll
  return AE_ScrollToPointEx(ae, (ptPosition?AESC_POINTCLIENT:AESC_POINTCARET)|(dwTest & AECSE_SCROLLEDX?AESC_OFFSETRECTDIVX:0)|(dwTest & AECSE_SCROLLEDY?AESC_OFFSETRECTDIVY:0), ptPosition, 3, 2);
}

DWORD AE_ScrollToPointEx(AKELEDIT *ae, DWORD dwFlags, POINT *ptPosition, int nOffsetX, int nOffsetY)
{
  POINT ptPos;
  int x=-1;
  int y=-1;
  DWORD dwResult=0;

  if (dwFlags & AESC_POINTCARET)
    ptPos=ae->ptCaret;
  else if (dwFlags & AESC_POINTGLOBAL)
    ptPos=*ptPosition;
  else
    AE_ClientToGlobal(ae, ptPosition, &ptPos);

  if (dwFlags & AESC_OFFSETPIXELX)
    x=nOffsetX;
  else if (dwFlags & AESC_OFFSETCHARX)
    x=ae->ptxt->nAveCharWidth * nOffsetX;
  else if (dwFlags & AESC_OFFSETRECTDIVX)
    x=(ae->rcDraw.right - ae->rcDraw.left) / nOffsetX;

  if (dwFlags & AESC_OFFSETPIXELY)
    y=nOffsetY;
  else if (dwFlags & AESC_OFFSETCHARY)
    y=ae->ptxt->nCharHeight * nOffsetY;
  else if (dwFlags & AESC_OFFSETRECTDIVY)
    y=(ae->rcDraw.bottom - ae->rcDraw.top) / nOffsetY;

  if (x != -1)
  {
    if (dwFlags & AESC_FORCELEFT)
      ptPos.x=max(ptPos.x - x, 0);
    else if (dwFlags & AESC_FORCERIGHT)
      ptPos.x=max(ptPos.x - (ae->rcDraw.right - ae->rcDraw.left) + x + 1, 0);
    else
    {
      if (ptPos.x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - x)
        ptPos.x=max(ptPos.x - (ae->rcDraw.right - ae->rcDraw.left) + x + 1, 0);
      else if (ptPos.x < ae->nHScrollPos + x)
        ptPos.x=max(ptPos.x - x, 0);
      else
        ptPos.x=ae->nHScrollPos;
    }
    if (ptPos.x != ae->nHScrollPos)
    {
      if (ptPos.x < ae->nHScrollPos)
      {
        if (ae->nHScrollPos > 0)
        {
          if (!(dwFlags & AESC_TEST)) AE_ScrollEditWindow(ae, SB_HORZ, ptPos.x);
          dwResult|=AECSE_SCROLLEDX|AECSE_SCROLLEDLEFT;
        }
      }
      else if (ptPos.x > ae->nHScrollPos)
      {
        if (ae->nHScrollPos < ae->ptxt->nHScrollMax)
        {
          if (!(dwFlags & AESC_TEST)) AE_ScrollEditWindow(ae, SB_HORZ, ptPos.x);
          dwResult|=AECSE_SCROLLEDX|AECSE_SCROLLEDRIGHT;
        }
      }
    }
  }

  if (y != -1)
  {
    if (dwFlags & AESC_FORCETOP)
      ptPos.y=max(ptPos.y - y, 0);
    else if (dwFlags & AESC_FORCEBOTTOM)
      ptPos.y=max(ptPos.y - (ae->rcDraw.bottom - ae->rcDraw.top) + y + 1, 0);
    else
    {
      if (ptPos.y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - ae->ptxt->nCharHeight - y)
        ptPos.y=max(ptPos.y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->ptxt->nCharHeight + y + 1, 0);
      else if (ptPos.y < ae->nVScrollPos + y)
        ptPos.y=max(ptPos.y - y, 0);
      else
        ptPos.y=ae->nVScrollPos;
    }
    if (ptPos.y != ae->nVScrollPos)
    {
      if (ptPos.y < ae->nVScrollPos)
      {
        if (ae->nVScrollPos > 0)
        {
          if (!(dwFlags & AESC_TEST)) AE_ScrollEditWindow(ae, SB_VERT, ptPos.y);
          dwResult|=AECSE_SCROLLEDY|AECSE_SCROLLEDUP;
        }
      }
      else if (ptPos.y > ae->nVScrollPos)
      {
        if (ae->nHScrollPos < ae->ptxt->nVScrollMax)
        {
          if (!(dwFlags & AESC_TEST)) AE_ScrollEditWindow(ae, SB_VERT, ptPos.y);
          dwResult|=AECSE_SCROLLEDY|AECSE_SCROLLEDDOWN;
        }
      }
    }
  }

  if (dwFlags & AESC_POINTOUT)
  {
    if (ptPosition)
    {
      if (!(dwFlags & AESC_TEST))
      {
        ptPos.x=ae->nHScrollPos;
        ptPos.y=ae->nVScrollPos;
      }
      if (dwFlags & AESC_POINTGLOBAL)
        *ptPosition=ptPos;
      else
        AE_GlobalToClient(ae, &ptPos, ptPosition);
    }
  }
  return dwResult;
}

void AE_UpdateScrollBars(AKELEDIT *ae, int nBar)
{
  SCROLLINFO si;
  BOOL bSetScroll;
  BOOL bUpdateScroll=!(ae->popt->dwLockUpdate & AELU_SCROLLBAR);

  if (ae->hWndEdit)
  {
    if (nBar == SB_BOTH || nBar == SB_HORZ)
    {
      if (ae->bHScrollShow && ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
      {
        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
        si.nMin=0;
        si.nMax=ae->ptxt->nHScrollMax;
        si.nPage=ae->rcDraw.right - ae->rcDraw.left;
        si.nPos=ae->nHScrollPos;
        SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, bUpdateScroll);

        si.fMask=SIF_POS;
        GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);
        ae->nHScrollPos=si.nPos;
      }
      else
      {
        bSetScroll=FALSE;

        if (!ae->bHScrollShow)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_RANGE;
          GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);
          if (si.nMin || si.nMax)
            bSetScroll=TRUE;
        }
        else bSetScroll=TRUE;

        if (bSetScroll)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_RANGE|SIF_PAGE|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);

          //Set to non-disabled state
          si.nMin=0;
          si.nMax=0;
          si.nPage=1;
          SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, FALSE);

          //Set to disabled state
          si.nMin=0;
          si.nMax=0;
          si.nPage=0;
          SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, bUpdateScroll);

          ae->nHScrollPos=0;
        }
      }

      if (ae->nHScrollPos != ae->nLastHScrollPos)
      {
        AE_NotifyHScroll(ae);
        ae->nLastHScrollPos=ae->nHScrollPos;
      }
    }

    if (nBar == SB_BOTH || nBar == SB_VERT)
    {
      if (ae->bVScrollShow && ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
      {
        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
        si.nMin=0;
        si.nMax=ae->ptxt->nVScrollMax;
        si.nPage=ae->rcDraw.bottom - ae->rcDraw.top;
        si.nPos=ae->nVScrollPos;
        SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

        si.fMask=SIF_POS;
        GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
        ae->nVScrollPos=si.nPos;
      }
      else
      {
        bSetScroll=FALSE;

        if (!ae->bVScrollShow)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_RANGE;
          GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
          if (si.nMin || si.nMax)
            bSetScroll=TRUE;
        }
        else bSetScroll=TRUE;

        if (bSetScroll)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_RANGE|SIF_PAGE|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);

          //Set to non-disabled state
          si.nMin=0;
          si.nMax=0;
          si.nPage=1;
          SetScrollInfo(ae->hWndEdit, SB_VERT, &si, FALSE);

          //Set to disabled state
          si.nMin=0;
          si.nMax=0;
          si.nPage=0;
          SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

          ae->nVScrollPos=0;
        }
      }

      if (ae->nVScrollPos != ae->nLastVScrollPos)
      {
        AE_NotifyVScroll(ae);
        ae->nLastVScrollPos=ae->nVScrollPos;
      }
    }
  }
  if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
}

int AE_ScrollEditWindow(AKELEDIT *ae, int nBar, int nPos)
{
  SCROLLINFO si;
  int nScrollPos=0;
  BOOL bUpdateScroll=!(ae->popt->dwLockUpdate & AELU_SCROLLBAR);

  if (nBar == SB_HORZ)
  {
    if (!ae->popt->bHScrollLock)
    {
      if (ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
      {
        if (ae->bHScrollShow)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_POS|SIF_DISABLENOSCROLL;
          si.nPos=nPos;
          SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, bUpdateScroll);

          si.fMask=SIF_POS;
          GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);
          nPos=si.nPos;
        }
        else
        {
          nPos=min(nPos, ae->ptxt->nHScrollMax - (ae->rcDraw.right - ae->rcDraw.left));
          nPos=max(nPos, 0);
        }
        nScrollPos=ae->nHScrollPos;

        if (nPos != ae->nHScrollPos)
        {
          AE_MButtonErase(ae);
          AE_ActiveColumnErase(ae);

          ScrollWindow(ae->hWndEdit, ae->nHScrollPos - nPos, 0, NULL, &ae->rcDraw);
          ae->nHScrollPos=nPos;
          //UpdateWindow(ae->hWndEdit);

          if (ae->nHScrollPos != ae->nLastHScrollPos)
          {
            AE_NotifyHScroll(ae);
            ae->nLastHScrollPos=ae->nHScrollPos;
          }
        }
        nScrollPos=ae->nHScrollPos - nScrollPos;
      }
    }
  }
  else if (nBar == SB_VERT)
  {
    if (!ae->popt->bVScrollLock)
    {
      if (ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
      {
        if (ae->bVScrollShow)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_POS|SIF_DISABLENOSCROLL;
          si.nPos=nPos;
          SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

          si.fMask=SIF_POS;
          GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
          nPos=si.nPos;
        }
        else
        {
          nPos=min(nPos, ae->ptxt->nVScrollMax - (ae->rcDraw.bottom - ae->rcDraw.top));
          nPos=max(nPos, 0);
        }
        nScrollPos=ae->nVScrollPos;

        if (nPos != ae->nVScrollPos)
        {
          AE_MButtonErase(ae);
          AE_ActiveColumnErase(ae);

          ScrollWindow(ae->hWndEdit, 0, ae->nVScrollPos - nPos, NULL, &ae->rcDraw);
          ae->nVScrollPos=nPos;
          //UpdateWindow(ae->hWndEdit);

          if (ae->nVScrollPos != ae->nLastVScrollPos)
          {
            AE_NotifyVScroll(ae);
            ae->nLastVScrollPos=ae->nVScrollPos;
          }
        }
        nScrollPos=ae->nVScrollPos - nScrollPos;
      }
    }
  }

  AE_SetCaretVis(ae, &ae->ptCaret);
  return nScrollPos;
}

int AE_HScroll(AKELEDIT *ae, int nAction, DWORD dwAlign)
{
  SCROLLINFO si;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);

  if (nAction == SB_LEFT)
  {
    si.nPos=si.nMin;
  }
  else if (nAction == SB_RIGHT)
  {
    si.nPos=si.nMax;
  }
  else if (nAction == SB_LINELEFT)
  {
    si.nPos-=ae->ptxt->nAveCharWidth;
  }
  else if (nAction == SB_LINERIGHT)
  {
    si.nPos+=ae->ptxt->nAveCharWidth;
  }
  else if (nAction == SB_PAGELEFT)
  {
    si.nPos-=si.nPage;
  }
  else if (nAction == SB_PAGERIGHT)
  {
    si.nPos+=si.nPage;
  }
  else if (nAction == SB_THUMBTRACK)
  {
    si.nPos=si.nTrackPos;
  }
  else if (nAction == SB_THUMBPOSITION)
  {
  }
  si.nPos=AE_AlignPos(ae, si.nPos, dwAlign);

  return AE_ScrollEditWindow(ae, SB_HORZ, si.nPos);
}

int AE_VScroll(AKELEDIT *ae, int nAction, DWORD dwAlign)
{
  SCROLLINFO si;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(ae->hWndEdit, SB_VERT, &si);

  if (nAction == SB_TOP)
  {
    si.nPos=si.nMin;
  }
  else if (nAction == SB_BOTTOM)
  {
    si.nPos=si.nMax;
  }
  else if (nAction == SB_LINEUP)
  {
    si.nPos-=ae->ptxt->nCharHeight;
  }
  else if (nAction == SB_LINEDOWN)
  {
    si.nPos+=ae->ptxt->nCharHeight;
  }
  else if (nAction == SB_PAGEUP)
  {
    si.nPos-=si.nPage;
  }
  else if (nAction == SB_PAGEDOWN)
  {
    si.nPos+=si.nPage;
  }
  else if (nAction == SB_THUMBTRACK)
  {
    si.nPos=si.nTrackPos;
  }
  else if (nAction == SB_THUMBPOSITION)
  {
  }
  si.nPos=AE_AlignPos(ae, si.nPos, dwAlign);

  return AE_ScrollEditWindow(ae, SB_VERT, si.nPos);
}

int AE_HScrollLine(AKELEDIT *ae, int nChar, DWORD dwAlign)
{
  int nPos;

  nPos=ae->nHScrollPos;
  nPos+=ae->ptxt->nAveCharWidth * nChar;
  nPos=AE_AlignPos(ae, nPos, dwAlign);
  return AE_ScrollEditWindow(ae, SB_HORZ, nPos);
}

int AE_VScrollLine(AKELEDIT *ae, int nLine, DWORD dwAlign)
{
  int nPos;

  nPos=ae->nVScrollPos;
  nPos+=ae->ptxt->nCharHeight * nLine;
  nPos=AE_AlignPos(ae, nPos, dwAlign);
  return AE_ScrollEditWindow(ae, SB_VERT, nPos);
}

int AE_AlignPos(AKELEDIT *ae, int nPos, DWORD dwAlign)
{
  if (dwAlign & AESB_ALIGNLEFT)
  {
    nPos=(nPos / ae->ptxt->nAveCharWidth) * ae->ptxt->nAveCharWidth;
  }
  else if (dwAlign & AESB_ALIGNRIGHT)
  {
    nPos+=(ae->rcDraw.right - ae->rcDraw.left) - 1;
    nPos=(nPos / ae->ptxt->nAveCharWidth) * ae->ptxt->nAveCharWidth;
    nPos-=(ae->rcDraw.right - ae->rcDraw.left) - 1;
  }
  else if (dwAlign & AESB_ALIGNTOP)
  {
    nPos=(nPos / ae->ptxt->nCharHeight) * ae->ptxt->nCharHeight;
  }
  else if (dwAlign & AESB_ALIGNBOTTOM)
  {
    nPos+=(ae->rcDraw.bottom - ae->rcDraw.top) - 1;
    nPos=(nPos / ae->ptxt->nCharHeight) * ae->ptxt->nCharHeight;
    nPos-=(ae->rcDraw.bottom - ae->rcDraw.top) - 1;
  }
  return nPos;
}

AEPRINTHANDLE* AE_StartPrintDocA(AKELEDIT *ae, AEPRINT *prn)
{
  AEPRINTHANDLE *ph;
  AEFONTITEMA *fi;
  TEXTMETRICA tmEditA;
  TEXTMETRICA tmPrintA;
  SIZE sizeWidth;
  HDC hEditDC;
  HFONT hEditFontOld;
  HFONT hPrintFontOld;
  int nPointSize=0;

  if (!prn->hPrinterDC || !prn->hEditFont)
    return NULL;

  if (ph=(AEPRINTHANDLE *)AE_HeapAlloc(ae, HEAP_ZERO_MEMORY, sizeof(AEPRINTHANDLE)))
  {
    //Clone AKELEDIT data for printing
    xmemcpy(&ph->aePrint, ae, sizeof(AKELEDIT));
    ph->aePrint.ptxt=&ph->aePrint.txt;
    ph->aePrint.popt=&ph->aePrint.opt;
    ph->aePrint.hDC=prn->hPrinterDC;

    //Calculate print font
    GetObjectA(prn->hEditFont, sizeof(LOGFONTA), &ph->aePrint.ptxt->lfFontA);
    if (hEditDC=GetDC(ae->hWndEdit))
    {
      hEditFontOld=(HFONT)SelectObject(hEditDC, prn->hEditFont);
      GetTextMetricsA(hEditDC, &tmEditA);
      if (hEditFontOld) SelectObject(hEditDC, hEditFontOld);
      nPointSize=MulDiv(ph->aePrint.ptxt->lfFontA.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
      ReleaseDC(ae->hWndEdit, hEditDC);
    }
    ph->aePrint.ptxt->lfFontA.lfHeight=MulDiv(nPointSize, GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY), 72);
    ph->aePrint.ptxt->lfFontA.lfHeight=-mod(ph->aePrint.ptxt->lfFontA.lfHeight);
    ph->aePrint.ptxt->lfFontA.lfWidth=0;

    //Create print font
    prn->hPrintFont=CreateFontIndirectA(&ph->aePrint.ptxt->lfFontA);
    hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);

    //Set print font
    ph->aePrint.ptxt->hFont=prn->hPrintFont;
    if (!(fi=AE_StackFontItemGetA(&hAkelEditFontsStackA, &ph->aePrint.ptxt->lfFontA)))
      fi=AE_StackFontItemInsertA(&hAkelEditFontsStackA, &ph->aePrint.ptxt->lfFontA);
    ph->aePrint.ptxt->lpCharWidths=fi->lpCharWidths;
    ph->aePrint.ptxt->hFontNormal=fi->hFontNormal;
    ph->aePrint.ptxt->hFontBold=fi->hFontBold;
    ph->aePrint.ptxt->hFontItalic=fi->hFontItalic;
    ph->aePrint.ptxt->hFontBoldItalic=fi->hFontBoldItalic;
    ph->aePrint.ptxt->hFontUrl=fi->hFontUrl;

    //Get print font sizes
    GetTextMetricsA(prn->hPrinterDC, &tmPrintA);
    ph->aePrint.ptxt->nLineGap=MulDiv(tmPrintA.tmHeight, ph->aePrint.ptxt->nLineGap, tmEditA.tmHeight);
    ph->aePrint.ptxt->nCharHeight=tmPrintA.tmHeight + ph->aePrint.ptxt->nLineGap;
    prn->nCharHeight=ph->aePrint.ptxt->nCharHeight;
    GetTextExtentPoint32W(prn->hPrinterDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeWidth);
    ph->aePrint.ptxt->nAveCharWidth=sizeWidth.cx / 52;
    prn->nAveCharWidth=ph->aePrint.ptxt->nAveCharWidth;
    GetTextExtentPoint32W(prn->hPrinterDC, L" ", 1, &sizeWidth);
    ph->aePrint.ptxt->nSpaceCharWidth=sizeWidth.cx;
    prn->nSpaceCharWidth=ph->aePrint.ptxt->nSpaceCharWidth;
    ph->aePrint.ptxt->nTabWidth=ph->aePrint.ptxt->nSpaceCharWidth * ph->aePrint.ptxt->nTabStop;
    prn->nTabWidth=ph->aePrint.ptxt->nTabWidth;

    //Return font
    if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);

    //Get print page size
    AE_GetPrintRect(prn, NULL, &prn->rcPageFull);
    AE_GetPrintRect(prn, &prn->rcMargins, &prn->rcPageIn);
    ph->aePrint.rcEdit=prn->rcPageFull;
    ph->aePrint.rcDraw=prn->rcPageIn;
  }
  return ph;
}

AEPRINTHANDLE* AE_StartPrintDocW(AKELEDIT *ae, AEPRINT *prn)
{
  AEPRINTHANDLE *ph;
  AEFONTITEMW *fi;
  TEXTMETRICW tmEditW;
  TEXTMETRICW tmPrintW;
  SIZE sizeWidth;
  HDC hEditDC;
  HFONT hEditFontOld;
  HFONT hPrintFontOld;
  int nPointSize=0;

  if (!prn->hPrinterDC || !prn->hEditFont)
    return NULL;

  if (ph=(AEPRINTHANDLE *)AE_HeapAlloc(ae, HEAP_ZERO_MEMORY, sizeof(AEPRINTHANDLE)))
  {
    //Clone AKELEDIT data for printing
    xmemcpy(&ph->aePrint, ae, sizeof(AKELEDIT));
    ph->aePrint.ptxt=&ph->aePrint.txt;
    ph->aePrint.popt=&ph->aePrint.opt;
    ph->aePrint.hDC=prn->hPrinterDC;

    //Calculate print font
    GetObjectW(prn->hEditFont, sizeof(LOGFONTW), &ph->aePrint.ptxt->lfFontW);
    if (hEditDC=GetDC(ae->hWndEdit))
    {
      hEditFontOld=(HFONT)SelectObject(hEditDC, prn->hEditFont);
      GetTextMetricsW(hEditDC, &tmEditW);
      if (hEditFontOld) SelectObject(hEditDC, hEditFontOld);
      nPointSize=MulDiv(ph->aePrint.ptxt->lfFontW.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
      ReleaseDC(ae->hWndEdit, hEditDC);
    }
    ph->aePrint.ptxt->lfFontW.lfHeight=MulDiv(nPointSize, GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY), 72);
    ph->aePrint.ptxt->lfFontW.lfHeight=-mod(ph->aePrint.ptxt->lfFontW.lfHeight);
    ph->aePrint.ptxt->lfFontW.lfWidth=0;

    //Create print font
    prn->hPrintFont=CreateFontIndirectW(&ph->aePrint.ptxt->lfFontW);
    hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);

    //Set print font
    ph->aePrint.ptxt->hFont=prn->hPrintFont;
    if (!(fi=AE_StackFontItemGetW(&hAkelEditFontsStackW, &ph->aePrint.ptxt->lfFontW)))
      fi=AE_StackFontItemInsertW(&hAkelEditFontsStackW, &ph->aePrint.ptxt->lfFontW);
    ph->aePrint.ptxt->lpCharWidths=fi->lpCharWidths;
    ph->aePrint.ptxt->hFontNormal=fi->hFontNormal;
    ph->aePrint.ptxt->hFontBold=fi->hFontBold;
    ph->aePrint.ptxt->hFontItalic=fi->hFontItalic;
    ph->aePrint.ptxt->hFontBoldItalic=fi->hFontBoldItalic;
    ph->aePrint.ptxt->hFontUrl=fi->hFontUrl;

    //Get print font sizes
    GetTextMetricsW(prn->hPrinterDC, &tmPrintW);
    ph->aePrint.ptxt->nLineGap=MulDiv(tmPrintW.tmHeight, ph->aePrint.ptxt->nLineGap, tmEditW.tmHeight);
    ph->aePrint.ptxt->nCharHeight=tmPrintW.tmHeight + ph->aePrint.ptxt->nLineGap;
    prn->nCharHeight=ph->aePrint.ptxt->nCharHeight;
    GetTextExtentPoint32W(prn->hPrinterDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeWidth);
    ph->aePrint.ptxt->nAveCharWidth=sizeWidth.cx / 52;
    prn->nAveCharWidth=ph->aePrint.ptxt->nAveCharWidth;
    GetTextExtentPoint32W(prn->hPrinterDC, L" ", 1, &sizeWidth);
    ph->aePrint.ptxt->nSpaceCharWidth=sizeWidth.cx;
    prn->nSpaceCharWidth=ph->aePrint.ptxt->nSpaceCharWidth;
    ph->aePrint.ptxt->nTabWidth=ph->aePrint.ptxt->nSpaceCharWidth * ph->aePrint.ptxt->nTabStop;
    prn->nTabWidth=ph->aePrint.ptxt->nTabWidth;

    //Return font
    if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);

    //Get print page size
    AE_GetPrintRect(prn, NULL, &prn->rcPageFull);
    AE_GetPrintRect(prn, &prn->rcMargins, &prn->rcPageIn);
    ph->aePrint.rcEdit=prn->rcPageFull;
    ph->aePrint.rcDraw=prn->rcPageIn;
  }
  return ph;
}

void AE_GetPrintRect(AEPRINT *prn, const RECT *rcMargins, RECT *rcPage)
{
  RECT rcPhys={0};
  RECT rcUser;
  RECT rcAdjUser;
  POINT ptDpi;
  POINT ptPage;
  int nExtent;

  nExtent=(prn->dwFlags & AEPRN_INHUNDREDTHSOFMILLIMETERS)?2540:1000;

  ptDpi.x=GetDeviceCaps(prn->hPrinterDC, LOGPIXELSX);
  ptDpi.y=GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY);

  ptPage.x=GetDeviceCaps(prn->hPrinterDC, PHYSICALWIDTH);
  ptPage.y=GetDeviceCaps(prn->hPrinterDC, PHYSICALHEIGHT);

  if (ptPage.x && ptPage.y)
  {
    rcPhys.left=GetDeviceCaps(prn->hPrinterDC, PHYSICALOFFSETX);
    rcPhys.top=GetDeviceCaps(prn->hPrinterDC, PHYSICALOFFSETY);
    rcPhys.right=ptPage.x - GetDeviceCaps(prn->hPrinterDC, HORZRES) - rcPhys.left;
    rcPhys.bottom=ptPage.y - GetDeviceCaps(prn->hPrinterDC, VERTRES) - rcPhys.top;
  }
  else
  {
    //hPrinterDC is not printer handle
    ptPage.x=GetDeviceCaps(prn->hPrinterDC, HORZRES);
    ptPage.y=GetDeviceCaps(prn->hPrinterDC, VERTRES);
  }

  if (!rcMargins)
  {
    rcUser.left=0;
    rcUser.top=0;
    rcUser.right=0;
    rcUser.bottom=0;
  }
  else
  {
    rcUser.left=MulDiv(rcMargins->left, ptDpi.x, nExtent);
    rcUser.top=MulDiv(rcMargins->top, ptDpi.y, nExtent);
    rcUser.right=MulDiv(rcMargins->right, ptDpi.x, nExtent);
    rcUser.bottom=MulDiv(rcMargins->bottom, ptDpi.y, nExtent);
  }

  rcAdjUser.left=max(rcPhys.left, rcUser.left);
  rcAdjUser.top=max(rcPhys.top, rcUser.top);
  rcAdjUser.right=max(rcPhys.right, rcUser.right);
  rcAdjUser.bottom=max(rcPhys.bottom, rcUser.bottom);

  rcPage->left=rcAdjUser.left - rcPhys.left;
  rcPage->top=rcAdjUser.top - rcPhys.top;
  rcPage->right=ptPage.x - rcAdjUser.right - rcPhys.left;
  rcPage->bottom=ptPage.y - rcAdjUser.bottom - rcPhys.top;
}

BOOL AE_PrintPage(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn)
{
  AEHLPAINT hlp={0};
  AETEXTOUT to={0};
  AECHARINDEX ciCount;
  AECHARINDEX ciTmp;
  HBRUSH hBasicBk;
  HBRUSH hTab;
  HFONT hPrintFontOld;
  RECT rcSpace;
  int nCharWidth=0;
  int nLineWidth;
  int nMaxLineWidth=0;
  int nLineLen;
  int nLineCount=0;
  int nMaxDrawCharOffset=0;
  int nTmp;
  BOOL bFormFeed=FALSE;
  BOOL bContinuePrint=TRUE;

  if (!prn->hPrintFont || !prn->crText.ciMin.lpLine || !prn->crText.ciMax.lpLine)
    return FALSE;

  //Select print font
  hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);

  //Set AETEXTOUT
  to.hDC=ph->aePrint.hDC;
  to.ptFirstCharInLine.x=prn->rcPageIn.left;
  to.ptFirstCharInLine.y=prn->rcPageIn.top;
  to.nDrawCharOffset=AE_AkelIndexToRichOffset(ae, &prn->crText.ciMin);
  to.dwPrintFlags=prn->dwFlags;
  nMaxDrawCharOffset=to.nDrawCharOffset;

  //Set AEHLPAINT
  hBasicBk=CreateSolidBrush(ae->popt->crBasicBk);
  hlp.dwDefaultText=ae->popt->crBasicText;
  hlp.dwDefaultBG=ae->popt->crBasicBk;
  hlp.hbrDefaultBG=hBasicBk;
  hlp.dwActiveText=hlp.dwDefaultText;
  hlp.dwActiveBG=hlp.dwDefaultBG;
  hlp.hbrActiveBG=hlp.hbrDefaultBG;
  hlp.dwPaintType=0;
  hlp.dwFontStyle=AEHLS_NONE;

  //Fill page rectangle
  if (!(prn->dwFlags & AEPRN_TEST))
  {
    if (prn->dwFlags & AEPRN_COLOREDBACKGROUND)
    {
      FillRect(ph->aePrint.hDC, &prn->rcPageIn, hBasicBk);
    }
  }

  //Calculate line
  CalculateLine:
  to.ciDrawLine=prn->crText.ciMin;
  ciCount=prn->crText.ciMin;
  nLineWidth=0;
  nLineLen=0;

  while (ciCount.lpLine)
  {
    while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
    {
      if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\t')
        nCharWidth=prn->nTabWidth - nLineWidth % prn->nTabWidth;
      else
        nCharWidth=AE_GetCharWidth(&ph->aePrint, ciCount.lpLine->wpLine + ciCount.nCharInLine, NULL);
      nLineWidth+=nCharWidth;

      if (!(prn->dwFlags & AEPRN_IGNOREFORMFEED) && ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\f')
        bFormFeed=TRUE;

      if (nLineWidth > prn->rcPageIn.right - prn->rcPageIn.left ||
          AE_IndexCompare(&ciCount, &prn->crText.ciMax) >= 0 ||
          bFormFeed)
      {
        if (bFormFeed)
        {
          if (AE_NextCharEx(&ciCount, &prn->crText.ciMin))
            if (AE_IsLastCharInLine(&prn->crText.ciMin))
              AE_NextCharEx(&prn->crText.ciMin, &prn->crText.ciMin);
        }
        else
        {
          if (prn->dwFlags & AEPRN_WRAPNONE)
          {
            if (!AE_NextLineEx(&ciCount, &prn->crText.ciMin))
              bContinuePrint=FALSE;
          }
          else if (prn->dwFlags & AEPRN_WRAPSYMBOL)
          {
            prn->crText.ciMin=ciCount;
          }
          else
          {
            //prn->dwFlags & AEPRN_WRAPWORD
            if (nLineWidth > prn->rcPageIn.right - prn->rcPageIn.left)
            {
              //Find end of word
              ciTmp=ciCount;
              nTmp=nLineLen;

              while (AE_PrevCharInLine(&ciTmp) && --nTmp >= 0)
              {
                if (AE_IsInDelimiterList(ph->aePrint.ptxt->wszWrapDelimiters, ciTmp.lpLine->wpLine[ciTmp.nCharInLine], TRUE))
                {
                  nLineLen=++nTmp;
                  AE_NextCharInLineEx(&ciTmp, &ciCount);
                  prn->crText.ciMin=ciCount;
                  bFormFeed=FALSE;
                  goto CheckTextEnd;
                }
              }
            }
            prn->crText.ciMin=ciCount;
          }
        }

        CheckTextEnd:
        if (AE_IndexCompare(&prn->crText.ciMin, &prn->crText.ciMax) >= 0)
          bContinuePrint=FALSE;
        goto PrintLine;
      }

      //Increment char count
      nLineLen+=AE_IndexLen(&ciCount);
      AE_IndexInc(&ciCount);
    }

    //Next line
    if (ciCount.lpLine->nLineBreak == AELB_WRAP)
    {
      AE_NextLine(&ciCount);
    }
    else
    {
      //Custom AEGI_NEXTVISIBLELINE implementation
      AECHARINDEX ciCharTmp=ciCount;

      while (AE_NextLine(&ciCharTmp))
      {
        if (ae->popt->dwOptions & AECO_NOPRINTCOLLAPSED)
        {
          if (!AE_StackIsLineCollapsed(&ph->aePrint, ciCharTmp.nLine))
            break;
        }
        else break;
      }

      if (ciCharTmp.lpLine)
      {
        prn->crText.ciMin=ciCharTmp;
        bContinuePrint=TRUE;
      }
      else
      {
        prn->crText.ciMin=ciCount;
        bContinuePrint=FALSE;
      }
      goto PrintLine;
    }
  }
  bContinuePrint=FALSE;

  //Print line
  PrintLine:
  nMaxDrawCharOffset=to.nDrawCharOffset + nLineLen;
  to.nDrawLineWidth=0;
  to.wpStartDraw=to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine;
  to.nStartDrawWidth=to.nDrawLineWidth;
  to.nMaxDrawCharsCount=0;
  hlp.dwFindFirst=AEHPT_MARKTEXT|AEHPT_LINK|AEHPT_QUOTE|AEHPT_DELIM1;

  while (to.ciDrawLine.lpLine)
  {
    while (to.ciDrawLine.nCharInLine <= to.ciDrawLine.lpLine->nLineLen)
    {
      if (to.nDrawCharOffset >= nMaxDrawCharOffset)
        goto PrintLineEnd;

      if (to.ciDrawLine.nCharInLine < to.ciDrawLine.lpLine->nLineLen)
      {
        if (to.ciDrawLine.lpLine->wpLine[to.ciDrawLine.nCharInLine] == L'\t')
          nCharWidth=prn->nTabWidth - to.nDrawLineWidth % prn->nTabWidth;
        else
          nCharWidth=AE_GetCharWidth(&ph->aePrint, to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine, NULL);
      }

      if (!(prn->dwFlags & AEPRN_TEST))
      {
        if (prn->dwFlags & AEPRN_COLOREDTEXT)
        {
          //Check highlight close
          AE_PaintCheckHighlightCloseItem(&ph->aePrint, &to, &hlp);

          //Check highlight open
          AE_PaintCheckHighlightOpenItem(&ph->aePrint, &to, &hlp, ae->ptxt->nLineCount);
        }
      }
      if (to.ciDrawLine.nCharInLine == to.ciDrawLine.lpLine->nLineLen) break;

      //Draw text up to tab character
      if (to.ciDrawLine.lpLine->wpLine[to.ciDrawLine.nCharInLine] == L'\t')
      {
        AE_PaintTextOut(&ph->aePrint, &to, &hlp);

        //Draw tab character
        rcSpace.left=to.ptFirstCharInLine.x + to.nStartDrawWidth;
        rcSpace.top=to.ptFirstCharInLine.y;
        rcSpace.right=rcSpace.left + nCharWidth;
        rcSpace.bottom=rcSpace.top + ph->aePrint.ptxt->nCharHeight;

        if (!(prn->dwFlags & AEPRN_TEST))
        {
          if (prn->dwFlags & AEPRN_COLOREDBACKGROUND)
          {
            if (hTab=CreateSolidBrush(hlp.dwActiveBG))
            {
              FillRect(to.hDC, &rcSpace, hTab);
              DeleteObject(hTab);
            }
          }
        }
        to.nStartDrawWidth+=nCharWidth;
        to.wpStartDraw+=1;
      }

      //Draw only 2048 characters at once
      if (to.nMaxDrawCharsCount >= 2048)
      {
        AE_PaintTextOut(ae, &to, &hlp);
      }
      else ++to.nMaxDrawCharsCount;

      //Increment line width
      to.nDrawLineWidth+=nCharWidth;

      //Increment char count
      to.nDrawCharOffset+=AE_IndexLen(&to.ciDrawLine);
      AE_IndexInc(&to.ciDrawLine);
    }

    //Next line
    if (to.ciDrawLine.lpLine->nLineBreak == AELB_WRAP)
    {
      AE_PaintTextOut(&ph->aePrint, &to, &hlp);
      AE_NextLine(&to.ciDrawLine);
      to.wpStartDraw=to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine;
    }
    else
    {
      ++to.nDrawCharOffset;
      break;
    }
  }

  PrintLineEnd:
  to.nDrawCharOffset+=AE_IndexSubtract(&ph->aePrint, &prn->crText.ciMin, &to.ciDrawLine, AELB_R, FALSE, FALSE);
  AE_PaintTextOut(&ph->aePrint, &to, &hlp);

  if (!(prn->dwFlags & AEPRN_TEST))
  {
    if (prn->dwFlags & AEPRN_COLOREDTEXT)
    {
      //Close all previous items
      AE_PaintCheckHighlightCleanUp(&ph->aePrint, &to, &hlp, &prn->crText.ciMin);
    }
  }

  //Next line
  to.ptFirstCharInLine.y+=ph->aePrint.ptxt->nCharHeight;
  nMaxLineWidth=max(nLineWidth, nMaxLineWidth);
  ++nLineCount;

  if (bContinuePrint && !bFormFeed && to.ptFirstCharInLine.y + ph->aePrint.ptxt->nCharHeight <= prn->rcPageIn.bottom)
    goto CalculateLine;

  //Fill rcPageOut structure
  prn->rcPageOut.left=prn->rcPageIn.left;
  prn->rcPageOut.top=prn->rcPageIn.top;
  prn->rcPageOut.right=prn->rcPageIn.left + nMaxLineWidth;
  prn->rcPageOut.bottom=to.ptFirstCharInLine.y;

  //Free resources
  if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
  DeleteObject(hBasicBk);

  return bContinuePrint;
}

void AE_EndPrintDoc(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn)
{
  DeleteObject(prn->hPrintFont);
  prn->hPrintFont=NULL;

  AE_HeapFree(ae, 0, (LPVOID)ph);
}

void AE_Paint(AKELEDIT *ae)
{
  PAINTSTRUCT ps;
  RECT rcErase;

  if (GetUpdateRect(ae->hWndEdit, &rcErase, FALSE))
  {
    AE_StackEraseInsert(ae, &rcErase);

    if (BeginPaint(ae->hWndEdit, &ps))
    {
      AEHLPAINT hlp={0};
      AETEXTOUT to={0};
      HBRUSH hBasicBk;
      HBRUSH hSelBk;
      HBRUSH hActiveLineBk;
      HBRUSH hTab;
      HBITMAP hBitmap;
      HBITMAP hBitmapOld=NULL;
      HFONT hFontOld=NULL;
      HRGN hDrawRgn;
      HRGN hDrawRgnOld=NULL;
      RECT rcDraw;
      RECT rcSpace;
      int nLineSelection;
      int nMinPaintWidth=0;
      int nMaxPaintWidth=0;
      int nCharWidth=0;
      int nLastDrawLine=0;
      BOOL bUseBufferDC=TRUE;

      //Create GDI objects
      hDrawRgn=CreateRectRgn(ae->rcDraw.left, ae->rcDraw.top, ae->rcDraw.right, ae->rcDraw.bottom);
      hDrawRgnOld=(HRGN)SelectObject(ps.hdc, hDrawRgn);
      hBasicBk=CreateSolidBrush(ae->popt->crBasicBk);
      hSelBk=CreateSolidBrush(ae->popt->crSelBk);
      hActiveLineBk=CreateSolidBrush(ae->popt->crActiveLineBk);

      //Set DCs
      to.hDC=ps.hdc;

      if (bUseBufferDC)
      {
        to.hDC=CreateCompatibleDC(ps.hdc);
        hBitmap=CreateCompatibleBitmap(ps.hdc, ae->rcEdit.right, ae->rcEdit.bottom);
        hBitmapOld=(HBITMAP)SelectObject(to.hDC, hBitmap);
      }
      if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(to.hDC, ae->ptxt->hFont);

      rcDraw=ps.rcPaint;
      if (rcDraw.top + ae->ptxt->nCharHeight <= ae->rcDraw.top)
        rcDraw.top=ae->rcDraw.top;
      if (rcDraw.bottom > ae->rcDraw.bottom)
        rcDraw.bottom=ae->rcDraw.bottom;

      if (rcDraw.right > ae->rcDraw.left &&
          rcDraw.bottom > ae->rcDraw.top)
      {
        to.ciDrawLine.nLine=AE_VPos(ae, ae->nVScrollPos + (rcDraw.top - ae->rcDraw.top), AEVPF_LINEFROMVPOS);
        to.ciDrawLine.lpLine=AE_GetLineData(ae, to.ciDrawLine.nLine);
        to.ciDrawLine.nCharInLine=0;

        nLastDrawLine=AE_VPos(ae, ae->nVScrollPos + (rcDraw.bottom - ae->rcDraw.top), AEVPF_LINEFROMVPOS);
        nLastDrawLine=min(nLastDrawLine, ae->ptxt->nLineCount);

        to.ptFirstCharInLine.x=ae->rcDraw.left - ae->nHScrollPos;
        to.ptFirstCharInLine.y=(AE_VPos(ae, to.ciDrawLine.nLine, AEVPF_VPOSFROMLINE) - ae->nVScrollPos) + ae->rcDraw.top;
        nMinPaintWidth=ae->nHScrollPos + max(rcDraw.left - ae->rcDraw.left, 0);
        nMaxPaintWidth=ae->nHScrollPos + max(rcDraw.right - ae->rcDraw.left, 0);

        if (to.ciDrawLine.lpLine)
        {
          ae->nFirstDrawLineOffset=AE_AkelIndexToRichOffset(ae, &to.ciDrawLine);
          ae->liFirstDrawLine.nLine=to.ciDrawLine.nLine;
          ae->liFirstDrawLine.lpLine=to.ciDrawLine.lpLine;
        }
        to.nDrawCharOffset=ae->nFirstDrawLineOffset;
      }
      else to.ciDrawLine.lpLine=NULL;

      //Check that active highlight theme exists
      if (ae->popt->lpActiveTheme)
      {
        if (!AE_HighlightIsThemeExists(ae->popt->lpActiveTheme))
          ae->popt->lpActiveTheme=NULL;
      }
      to.dwPrintFlags=AEPRN_COLOREDTEXT|AEPRN_COLOREDBACKGROUND;

      while (to.ciDrawLine.lpLine)
      {
        //Line must be visible
        if (!AE_StackIsLineCollapsed(ae, to.ciDrawLine.nLine))
        {
          //Close all previous items
          AE_PaintCheckHighlightCleanUp(ae, &to, &hlp, &to.ciDrawLine);

          //Get first paint char in line
          AE_GetCharInLine(ae, to.ciDrawLine.lpLine, nMinPaintWidth - ae->ptxt->nAveCharWidth, AECIL_ALLPOS, &to.ciDrawLine.nCharInLine, &to.nDrawLineWidth, FALSE);
          to.nDrawCharOffset+=min(to.ciDrawLine.nCharInLine, to.ciDrawLine.lpLine->nLineLen);
          to.nStartDrawWidth=to.nDrawLineWidth;
          to.nMaxDrawCharsCount=0;
          to.wpStartDraw=to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine;
          hlp.dwFindFirst=AEHPT_SELECTION|AEHPT_MARKTEXT|AEHPT_LINK|AEHPT_QUOTE|AEHPT_DELIM1;

          //Set initial colors
          if (to.ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
          {
            hlp.dwDefaultText=ae->popt->crActiveLineText;
            hlp.dwDefaultBG=ae->popt->crActiveLineBk;
            hlp.hbrDefaultBG=hActiveLineBk;
          }
          else
          {
            hlp.dwDefaultText=ae->popt->crBasicText;
            hlp.dwDefaultBG=ae->popt->crBasicBk;
            hlp.hbrDefaultBG=hBasicBk;
          }
          hlp.dwActiveText=hlp.dwDefaultText;
          hlp.dwActiveBG=hlp.dwDefaultBG;
          hlp.hbrActiveBG=hlp.hbrDefaultBG;
          hlp.dwPaintType=0;
          hlp.dwFontStyle=AEHLS_NONE;

          if (ae->popt->bHideSelection)
          {
            nLineSelection=AELS_EMPTY;
          }
          else
          {
            if (to.ciDrawLine.lpLine->nSelStart == to.ciDrawLine.lpLine->nSelEnd)
              nLineSelection=AELS_EMPTY;
            else if (to.ciDrawLine.lpLine->nSelStart == 0 && to.ciDrawLine.lpLine->nSelEnd == to.ciDrawLine.lpLine->nLineLen)
              nLineSelection=AELS_FULL;
            else
              nLineSelection=AELS_PARTLY;
          }

          //Erase space where text will be drawn.
          rcSpace.left=rcDraw.left;
          rcSpace.top=to.ptFirstCharInLine.y;
          rcSpace.right=rcDraw.right;
          rcSpace.bottom=to.ptFirstCharInLine.y + ae->ptxt->nCharHeight;
          FillRect(to.hDC, &rcSpace, hlp.hbrActiveBG);

          //Fill space after line end, before text line is drawn.
          if (to.ciDrawLine.lpLine->nLineWidth <= nMaxPaintWidth)
          {
            rcSpace.right=ae->rcDraw.left + (to.ciDrawLine.lpLine->nLineWidth - ae->nHScrollPos);

            if (ae->bColumnSel)
            {
              //Draw column selection
              if (nLineSelection == AELS_PARTLY)
              {
                if (to.ciDrawLine.lpLine->nSelStart >= to.ciDrawLine.lpLine->nLineLen)
                {
                  rcSpace.left=rcSpace.right;
                  rcSpace.top=to.ptFirstCharInLine.y;
                  rcSpace.right=rcSpace.left + (to.ciDrawLine.lpLine->nSelStart - to.ciDrawLine.lpLine->nLineLen) * ae->ptxt->nSpaceCharWidth;
                  rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                  FillRect(to.hDC, &rcSpace, hlp.hbrActiveBG);
                }
                if (to.ciDrawLine.lpLine->nSelEnd > to.ciDrawLine.lpLine->nLineLen)
                {
                  if (rcSpace.right < ae->rcDraw.right)
                  {
                    rcSpace.left=rcSpace.right;
                    rcSpace.top=to.ptFirstCharInLine.y;
                    rcSpace.right=rcSpace.left + (to.ciDrawLine.lpLine->nSelEnd - max(to.ciDrawLine.lpLine->nSelStart, to.ciDrawLine.lpLine->nLineLen)) * ae->ptxt->nSpaceCharWidth;
                    rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                    FillRect(to.hDC, &rcSpace, hSelBk);
                  }
                }
              }
            }
            else
            {
              if (!ae->popt->bHideSelection)
              {
                //Draw new line selection
                if (to.ciDrawLine.nLine >= ae->ciSelStartIndex.nLine &&
                    to.ciDrawLine.nLine < ae->ciSelEndIndex.nLine)
                {
                  if (ae->popt->dwOptions & AECO_ENTIRENEWLINEDRAW)
                  {
                    //Draw selection to the right edge
                    if (rcSpace.right < ae->rcDraw.right)
                    {
                      rcSpace.left=rcSpace.right;
                      rcSpace.top=to.ptFirstCharInLine.y;
                      rcSpace.right=ae->rcDraw.right;
                      rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                      FillRect(to.hDC, &rcSpace, hSelBk);
                    }
                  }
                  else if (!(ae->popt->dwOptions & AECO_NONEWLINEDRAW) &&
                           to.ciDrawLine.lpLine->nLineBreak != AELB_WRAP)
                  {
                    if (to.ciDrawLine.lpLine->nLineWidth + ae->ptxt->nAveCharWidth > ae->nHScrollPos)
                    {
                      rcSpace.left=rcSpace.right;
                      rcSpace.top=to.ptFirstCharInLine.y;
                      rcSpace.right=rcSpace.left + ae->ptxt->nAveCharWidth;
                      rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                      FillRect(to.hDC, &rcSpace, hSelBk);
                    }
                  }
                }
              }
            }

            //Fill space to the right edge
            if (rcSpace.right < ae->rcDraw.right)
            {
              rcSpace.left=rcSpace.right;
              rcSpace.top=to.ptFirstCharInLine.y;
              rcSpace.right=ae->rcDraw.right;
              rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
              FillRect(to.hDC, &rcSpace, hlp.hbrActiveBG);
            }
          }

          //Scan line
          while (to.ciDrawLine.nCharInLine <= to.ciDrawLine.lpLine->nLineLen)
          {
            if (to.ciDrawLine.nCharInLine < to.ciDrawLine.lpLine->nLineLen)
            {
              if (to.ciDrawLine.lpLine->wpLine[to.ciDrawLine.nCharInLine] == L'\t')
                nCharWidth=ae->ptxt->nTabWidth - to.nDrawLineWidth % ae->ptxt->nTabWidth;
              else
                nCharWidth=AE_GetCharWidth(ae, to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine, NULL);
            }

            //Selection
            if (nLineSelection == AELS_FULL || nLineSelection == AELS_PARTLY)
            {
              if (to.ciDrawLine.lpLine->nSelStart == to.ciDrawLine.nCharInLine ||
                  ((hlp.dwFindFirst & AEHPT_SELECTION) && AE_IsCharInSelection(&to.ciDrawLine)))
              {
                hlp.dwFindFirst&=~AEHPT_SELECTION;

                //Draw text up to selection start
                AE_PaintTextOut(ae, &to, &hlp);

                hlp.dwActiveText=ae->popt->crSelText;
                hlp.dwActiveBG=ae->popt->crSelBk;
                hlp.hbrActiveBG=hSelBk;
                hlp.dwPaintType|=AEHPT_SELECTION;
                hlp.dwFontStyle=AEHLS_NONE;
              }
              else if (to.ciDrawLine.lpLine->nSelEnd == to.ciDrawLine.nCharInLine)
              {
                //Draw text up to selection end
                AE_PaintTextOut(ae, &to, &hlp);

                hlp.dwActiveText=hlp.dwDefaultText;
                hlp.dwActiveBG=hlp.dwDefaultBG;
                hlp.hbrActiveBG=hlp.hbrDefaultBG;
                hlp.dwPaintType&=~AEHPT_SELECTION;
                hlp.dwFontStyle=AEHLS_NONE;
              }
            }

            //Check highlight close
            AE_PaintCheckHighlightCloseItem(ae, &to, &hlp);

            //Check highlight open
            AE_PaintCheckHighlightOpenItem(ae, &to, &hlp, nLastDrawLine);

            if (to.ciDrawLine.nCharInLine == to.ciDrawLine.lpLine->nLineLen) break;

            //Draw text up to tab character
            if (to.ciDrawLine.lpLine->wpLine[to.ciDrawLine.nCharInLine] == L'\t')
            {
              AE_PaintTextOut(ae, &to, &hlp);

              //Draw tab character
              rcSpace.left=to.ptFirstCharInLine.x + to.nStartDrawWidth;
              rcSpace.top=to.ptFirstCharInLine.y;
              rcSpace.right=rcSpace.left + nCharWidth;
              rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;

              if (hTab=CreateSolidBrush(hlp.dwActiveBG))
              {
                FillRect(to.hDC, &rcSpace, hTab);
                DeleteObject(hTab);
              }
              to.nStartDrawWidth+=nCharWidth;
              to.wpStartDraw+=1;
            }

            //Draw only 2048 characters at once
            if (to.nMaxDrawCharsCount >= 2048)
            {
              AE_PaintTextOut(ae, &to, &hlp);
            }
            else ++to.nMaxDrawCharsCount;

            //Stop line checking and draw it, if it's outside draw area
            if (to.nDrawLineWidth > nMaxPaintWidth)
              break;

            //Increment line width
            to.nDrawLineWidth+=nCharWidth;

            //Increment char count
            to.nDrawCharOffset+=AE_IndexLen(&to.ciDrawLine);
            AE_IndexInc(&to.ciDrawLine);
          }
          if (to.ciDrawLine.nCharInLine < to.ciDrawLine.lpLine->nLineLen)
            to.nDrawCharOffset+=to.ciDrawLine.lpLine->nLineLen - to.ciDrawLine.nCharInLine;

          //Draw text line
          AE_PaintTextOut(ae, &to, &hlp);

          //Copy line from buffer DC
          if (bUseBufferDC)
          {
            rcSpace.left=max(rcDraw.left, ae->rcDraw.left);
            rcSpace.top=to.ptFirstCharInLine.y;
            rcSpace.right=min(rcDraw.right - rcSpace.left, ae->rcDraw.right - rcSpace.left);
            rcSpace.bottom=ae->ptxt->nCharHeight;
            BitBlt(ps.hdc, rcSpace.left, rcSpace.top, rcSpace.right, rcSpace.bottom, to.hDC, rcSpace.left, rcSpace.top, SRCCOPY);
          }

          //Next line
          to.ptFirstCharInLine.y+=ae->ptxt->nCharHeight;
          if (to.ptFirstCharInLine.y >= rcDraw.bottom)
            break;

          if (to.ciDrawLine.lpLine->nLineBreak != AELB_WRAP)
            ++to.nDrawCharOffset;
        }
        AE_NextLine(&to.ciDrawLine);
      }
      if (hFontOld) SelectObject(to.hDC, hFontOld);

      if (bUseBufferDC)
      {
        if (hBitmapOld) SelectObject(to.hDC, hBitmapOld);
        DeleteDC(to.hDC);
        DeleteObject(hBitmap);
      }
      if (hDrawRgnOld) SelectObject(ps.hdc, hDrawRgnOld);
      DeleteObject(hDrawRgn);
      DeleteObject(hBasicBk);
      DeleteObject(hSelBk);
      DeleteObject(hActiveLineBk);

      EndPaint(ae->hWndEdit, &ps);
    }
  }
}

void AE_PaintTextOut(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp)
{
  RECT rcTextOut;
  HBRUSH hBrush;
  char *szText;
  char szChar[4];
  wchar_t *wpText=to->wpStartDraw;
  int nTextLen=to->ciDrawLine.nCharInLine - (to->wpStartDraw - to->ciDrawLine.lpLine->wpLine);
  int nTextWidth=to->nDrawLineWidth - to->nStartDrawWidth;
  int nCharLen;
  int nBytes;
  int i;
  DWORD dwTextOutFlags=0;

  if (nTextLen)
  {
    if (!(to->dwPrintFlags & AEPRN_TEST))
    {
      if (to->dwPrintFlags & AEPRN_COLOREDTEXT)
      {
        SetTextColor(to->hDC, hlp->dwActiveText);
      }
      if (to->dwPrintFlags & AEPRN_COLOREDBACKGROUND)
      {
        SetBkColor(to->hDC, hlp->dwActiveBG);
      }
    }
    rcTextOut.left=to->ptFirstCharInLine.x + to->nStartDrawWidth;
    rcTextOut.top=to->ptFirstCharInLine.y;
    rcTextOut.right=ae->rcDraw.right/*rcTextOut.left + nTextWidth*/;
    rcTextOut.bottom=rcTextOut.top + ae->ptxt->nCharHeight;

    if (rcTextOut.right > ae->rcDraw.left &&
        rcTextOut.left < ae->rcDraw.right)
    {
      if (!(to->dwPrintFlags & AEPRN_TEST))
      {
        if (to->dwPrintFlags & AEPRN_COLOREDTEXT)
        {
          if (!(hlp->dwPaintType & AEHPT_SELECTION))
          {
            if (hlp->dwPaintType & AEHPT_LINK)
            {
              if (ae->ptxt->hFontUrl) SelectObject(to->hDC, ae->ptxt->hFontUrl);
            }
            if (hlp->dwFontStyle)
            {
              if (hlp->dwFontStyle == AEHLS_FONTNORMAL)
              {
                if (ae->ptxt->hFontNormal) SelectObject(to->hDC, ae->ptxt->hFontNormal);
              }
              else if (hlp->dwFontStyle == AEHLS_FONTBOLD)
              {
                if (ae->ptxt->hFontBold) SelectObject(to->hDC, ae->ptxt->hFontBold);
              }
              else if (hlp->dwFontStyle == AEHLS_FONTITALIC)
              {
                if (ae->ptxt->hFontItalic) SelectObject(to->hDC, ae->ptxt->hFontItalic);
              }
              else if (hlp->dwFontStyle == AEHLS_FONTBOLDITALIC)
              {
                if (ae->ptxt->hFontBoldItalic) SelectObject(to->hDC, ae->ptxt->hFontBoldItalic);
              }
            }
            if (hlp->dwActiveBG != hlp->dwDefaultBG)
            {
              //Fill space of the external leading
              dwTextOutFlags=ETO_OPAQUE;
              rcTextOut.right=rcTextOut.left + nTextWidth;
            }
            else SetBkMode(to->hDC, TRANSPARENT);
          }
          else
          {
            //Fill space of the external leading
            dwTextOutFlags=ETO_OPAQUE;
            rcTextOut.right=rcTextOut.left + nTextWidth;
          }
        }

        if (!(ae->popt->dwOptions & AECO_PAINTGROUP))
        {
          if (dwTextOutFlags & ETO_OPAQUE)
          {
            if (hBrush=CreateSolidBrush(hlp->dwActiveBG))
            {
              FillRect(to->hDC, &rcTextOut, hBrush);
              DeleteObject(hBrush);
            }
            SetBkMode(to->hDC, TRANSPARENT);
          }

          for (i=0; i < nTextLen; ++i)
          {
            if (nCharLen=AE_CopyChar(NULL, nTextLen - i, wpText + i))
            {
              if (!(to->dwPrintFlags & AEPRN_ANSI))
              {
                TextOutW(to->hDC, rcTextOut.left, rcTextOut.top, wpText + i, nCharLen);
              }
              else
              {
                nBytes=WideCharToMultiByte(CP_ACP, 0, wpText + i, 1, szChar, sizeof(szChar), NULL, NULL);
                TextOutA(to->hDC, rcTextOut.left, rcTextOut.top, szChar, nBytes);
              }
              rcTextOut.left+=AE_GetCharWidth(ae, wpText + i, NULL);
              i+=nCharLen - 1;
            }
          }
        }
        else
        {
          if (!(to->dwPrintFlags & AEPRN_ANSI))
          {
            ExtTextOutW(to->hDC, rcTextOut.left, rcTextOut.top, dwTextOutFlags, &rcTextOut, wpText, nTextLen, NULL);
          }
          else
          {
            nBytes=WideCharToMultiByte(CP_ACP, 0, wpText, nTextLen, NULL, 0, NULL, NULL);

            if (szText=(char *)AE_HeapAlloc(ae, 0, nBytes))
            {
              WideCharToMultiByte(CP_ACP, 0, wpText, nTextLen, szText, nBytes, NULL, NULL);
              ExtTextOutA(to->hDC, rcTextOut.left, rcTextOut.top, dwTextOutFlags, &rcTextOut, szText, nBytes, NULL);
              AE_HeapFree(ae, 0, (LPVOID)szText);
            }
          }
        }

        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if ((hlp->dwPaintType & AEHPT_LINK) || hlp->dwFontStyle)
          {
            if (ae->ptxt->hFont) SelectObject(to->hDC, ae->ptxt->hFont);
          }
        }
        if (GetBkMode(to->hDC) == TRANSPARENT)
          SetBkMode(to->hDC, OPAQUE);
      }
    }
    to->wpStartDraw+=nTextLen;
    to->nStartDrawWidth+=nTextWidth;
  }
  to->nMaxDrawCharsCount=0;
}

void AE_PaintCheckHighlightOpenItem(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, int nLastDrawLine)
{
  if (ae->popt->bDetectUrl)
  {
    //Url find
    if (AE_IndexCompare(&hlp->crLink.ciMax, &to->ciDrawLine) <= 0)
    {
      if (hlp->dwFindFirst & AEHPT_LINK)
      {
        hlp->dwFindFirst&=~AEHPT_LINK;
        if (!AE_HighlightFindUrl(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, nLastDrawLine, &hlp->crLink))
        {
          hlp->crLink.ciMin=to->ciDrawLine;
          hlp->crLink.ciMin.lpLine=NULL;
          hlp->crLink.ciMax=hlp->crLink.ciMin;
        }
      }
      else AE_HighlightFindUrl(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, nLastDrawLine, &hlp->crLink);
    }

    //Check url start
    if (hlp->crLink.ciMin.lpLine && hlp->crLink.ciMax.lpLine)
    {
      if (!(hlp->dwPaintType & AEHPT_LINK))
      {
        if (AE_IndexCompare(&to->ciDrawLine, &hlp->crLink.ciMax) < 0)
        {
          if (AE_IndexCompare(&hlp->crLink.ciMin, &to->ciDrawLine) <= 0)
          {
            if (!(hlp->dwPaintType & AEHPT_SELECTION))
            {
              //Draw text before range for highlight
              AE_PaintTextOut(ae, to, hlp);
            }
            hlp->dwPaintType|=AEHPT_LINK;
          }
        }
      }
      if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_LINK))
      {
        hlp->dwActiveText=ae->popt->crUrlText;
        hlp->dwActiveBG=hlp->dwDefaultBG;
        hlp->dwFontStyle=AEHLS_NONE;
      }
    }
  }

  //Fold find
  if (hlp->crFold.cpMax <= to->nDrawCharOffset)
  {
    AEFOLD *lpFoldParent;
    AEFOLD *lpFoldPrevSubling;

    AE_StackFindFold(ae, AEFF_FINDOFFSET|AEFF_RECURSE, to->nDrawCharOffset, NULL, &lpFoldParent, &lpFoldPrevSubling);
  }

  //Check fold start
  if (hlp->lpFold)
  {
    if (!(hlp->dwPaintType & AEHPT_FOLD))
    {
      if (to->nDrawCharOffset < hlp->crFold.cpMax)
      {
        if (hlp->crFold.cpMin <= to->nDrawCharOffset)
        {
          if (!(hlp->dwPaintType & AEHPT_SELECTION))
          {
            //Draw text before mark
            AE_PaintTextOut(ae, to, hlp);
          }
          hlp->dwPaintType|=AEHPT_FOLD;
        }
      }
    }
    if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_FOLD))
    {
      if (hlp->lpFold->crText != (DWORD)-1)
        hlp->dwActiveText=hlp->lpFold->crText;
      else
        hlp->dwActiveText=hlp->dwDefaultText;
      if (hlp->lpFold->crBk != (DWORD)-1)
        hlp->dwActiveBG=hlp->lpFold->crBk;
      else
        hlp->dwActiveBG=hlp->dwDefaultBG;
      if (hlp->lpFold->dwFontStyle != AEHLS_NONE)
        hlp->dwFontStyle=hlp->lpFold->dwFontStyle;
    }
  }

  if (ae->popt->lpActiveTheme)
  {
    //Only if char not in URL
    if (!(hlp->dwPaintType & AEHPT_LINK))
    {
      //Quote find
      if (AE_IndexCompare(&hlp->qm.crQuoteEnd.ciMax, &to->ciDrawLine) <= 0)
      {
        if (hlp->dwFindFirst & AEHPT_QUOTE)
        {
          hlp->dwFindFirst&=~AEHPT_QUOTE;
          if (!AE_HighlightFindQuote(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, &hlp->qm))
          {
            hlp->qm.lpQuote=NULL;
            hlp->qm.crQuoteStart.ciMin=to->ciDrawLine;
            hlp->qm.crQuoteStart.ciMax=to->ciDrawLine;
            hlp->qm.crQuoteEnd.ciMin=to->ciDrawLine;
            hlp->qm.crQuoteEnd.ciMax=to->ciDrawLine;
          }
        }
        else AE_HighlightFindQuote(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, &hlp->qm);
      }

      //Check quote start
      if (hlp->qm.lpQuote)
      {
        if (!(hlp->dwPaintType & AEHPT_QUOTE))
        {
          if (AE_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteEnd.ciMax) < 0)
          {
            if ((!(hlp->qm.lpQuote->dwFlags & AEHLF_QUOTESTART_NOHIGHLIGHT) && AE_IndexCompare(&hlp->qm.crQuoteStart.ciMin, &to->ciDrawLine) <= 0) ||
                ((hlp->qm.lpQuote->dwFlags & AEHLF_QUOTESTART_NOHIGHLIGHT) && AE_IndexCompare(&hlp->qm.crQuoteStart.ciMax, &to->ciDrawLine) <= 0))
            {
              if (!(hlp->dwPaintType & AEHPT_SELECTION))
              {
                //Draw text before range for highlight
                AE_PaintTextOut(ae, to, hlp);
              }
              hlp->dwPaintType|=AEHPT_QUOTE;
            }
          }
        }
        if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_QUOTE))
        {
          if (hlp->qm.lpQuote->crText != (DWORD)-1)
            hlp->dwActiveText=hlp->qm.lpQuote->crText;
          else
            hlp->dwActiveText=hlp->dwDefaultText;
          if (hlp->qm.lpQuote->crBk != (DWORD)-1)
            hlp->dwActiveBG=hlp->qm.lpQuote->crBk;
          else
            hlp->dwActiveBG=hlp->dwDefaultBG;
          //if (hlp->qm.lpQuote->dwFontStyle != AEHLS_NONE)
            hlp->dwFontStyle=hlp->qm.lpQuote->dwFontStyle;
        }
      }

      //Only if char not in quote
      if (!hlp->qm.lpQuote || AE_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteStart.ciMin) < 0)
      {
        //Word find
        if (AE_IndexCompare(&hlp->wm.crDelim2.ciMax, &to->ciDrawLine) <= 0)
        {
          if (hlp->dwFindFirst & AEHPT_DELIM1)
          {
            hlp->dwFindFirst&=~AEHPT_DELIM1;
            AE_HighlightFindWord(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, &hlp->wm, &hlp->qm);
          }
          else AE_HighlightFindWord(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, &hlp->wm, &hlp->qm);
        }

        //Check delimiters and word start
        if (hlp->wm.lpDelim1)
        {
          if (!(hlp->dwPaintType & AEHPT_DELIM1))
          {
            if (AE_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim1.ciMax) < 0)
            {
              if (AE_IndexCompare(&hlp->wm.crDelim1.ciMin, &to->ciDrawLine) <= 0)
              {
                if (!(hlp->dwPaintType & AEHPT_SELECTION))
                {
                  //Draw text before range for highlight
                  AE_PaintTextOut(ae, to, hlp);
                }
                hlp->dwPaintType|=AEHPT_DELIM1;
              }
            }
          }
          if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_DELIM1))
          {
            if (hlp->wm.lpDelim1->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->wm.lpDelim1->crText;
            else
              hlp->dwActiveText=hlp->dwDefaultText;
            if (hlp->wm.lpDelim1->crBk != (DWORD)-1)
              hlp->dwActiveBG=hlp->wm.lpDelim1->crBk;
            else
              hlp->dwActiveBG=hlp->dwDefaultBG;
            //if (hlp->wm.lpDelim1->dwFontStyle != AEHLS_NONE)
              hlp->dwFontStyle=hlp->wm.lpDelim1->dwFontStyle;
          }
        }
        if (hlp->wm.lpWord)
        {
          if (!(hlp->dwPaintType & AEHPT_WORD))
          {
            if (AE_IndexCompare(&to->ciDrawLine, &hlp->wm.crWord.ciMax) < 0)
            {
              if (AE_IndexCompare(&hlp->wm.crWord.ciMin, &to->ciDrawLine) <= 0)
              {
                if (!(hlp->dwPaintType & AEHPT_SELECTION))
                {
                  //Draw text before range for highlight
                  AE_PaintTextOut(ae, to, hlp);
                }
                hlp->dwPaintType|=AEHPT_WORD;
              }
            }
          }
          if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_WORD))
          {
            if (hlp->wm.lpWord->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->wm.lpWord->crText;
            else
              hlp->dwActiveText=hlp->dwDefaultText;
            if (hlp->wm.lpWord->crBk != (DWORD)-1)
              hlp->dwActiveBG=hlp->wm.lpWord->crBk;
            else
              hlp->dwActiveBG=hlp->dwDefaultBG;
            //if (hlp->wm.lpWord->dwFontStyle != AEHLS_NONE)
              hlp->dwFontStyle=hlp->wm.lpWord->dwFontStyle;
          }
        }
        if (hlp->wm.lpDelim2)
        {
          if (!(hlp->dwPaintType & AEHPT_DELIM2))
          {
            if (AE_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim2.ciMax) < 0)
            {
              if (AE_IndexCompare(&hlp->wm.crDelim2.ciMin, &to->ciDrawLine) <= 0)
              {
                if (!(hlp->dwPaintType & AEHPT_SELECTION))
                {
                  //Draw text before range for highlight
                  AE_PaintTextOut(ae, to, hlp);
                }
                hlp->dwPaintType|=AEHPT_DELIM2;
              }
            }
          }
          if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_DELIM2))
          {
            if (hlp->wm.lpDelim2->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->wm.lpDelim2->crText;
            else
              hlp->dwActiveText=hlp->dwDefaultText;
            if (hlp->wm.lpDelim2->crBk != (DWORD)-1)
              hlp->dwActiveBG=hlp->wm.lpDelim2->crBk;
            else
              hlp->dwActiveBG=hlp->dwDefaultBG;
            //if (hlp->wm.lpDelim2->dwFontStyle != AEHLS_NONE)
              hlp->dwFontStyle=hlp->wm.lpDelim2->dwFontStyle;
          }
        }
      }
    }

    //Mark range find
    if (hlp->mrm.crMarkRange.cpMax <= to->nDrawCharOffset)
    {
      AE_HighlightFindMarkRange(ae, to->nDrawCharOffset, &hlp->mrm);
    }

    //Check mark range start
    if (hlp->mrm.lpMarkRange)
    {
      if (!(hlp->dwPaintType & AEHPT_MARKRANGE))
      {
        if (to->nDrawCharOffset < hlp->mrm.crMarkRange.cpMax)
        {
          if (hlp->mrm.crMarkRange.cpMin <= to->nDrawCharOffset)
          {
            if (!(hlp->dwPaintType & AEHPT_SELECTION))
            {
              //Draw text before mark
              AE_PaintTextOut(ae, to, hlp);
            }
            hlp->dwPaintType|=AEHPT_MARKRANGE;
          }
        }
      }
      if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_MARKRANGE))
      {
        if (hlp->mrm.lpMarkRange->crText != (DWORD)-1)
          hlp->dwActiveText=hlp->mrm.lpMarkRange->crText;
        else
          hlp->dwActiveText=hlp->dwDefaultText;
        if (hlp->mrm.lpMarkRange->crBk != (DWORD)-1)
          hlp->dwActiveBG=hlp->mrm.lpMarkRange->crBk;
        else
          hlp->dwActiveBG=hlp->dwDefaultBG;
        if (hlp->mrm.lpMarkRange->dwFontStyle != AEHLS_NONE)
          hlp->dwFontStyle=hlp->mrm.lpMarkRange->dwFontStyle;
      }
    }

    //Mark text find
    if (AE_IndexCompare(&hlp->mtm.crMarkText.ciMax, &to->ciDrawLine) <= 0)
    {
      if (hlp->dwFindFirst & AEHPT_MARKTEXT)
      {
        hlp->dwFindFirst&=~AEHPT_MARKTEXT;
        if (!AE_HighlightFindMarkText(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, &hlp->mtm))
        {
          hlp->mtm.crMarkText.ciMin=to->ciDrawLine;
          hlp->mtm.crMarkText.ciMax=to->ciDrawLine;
        }
      }
      else AE_HighlightFindMarkText(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, &hlp->mtm);
    }

    //Check mark text start
    if (hlp->mtm.lpMarkText)
    {
      if (!(hlp->dwPaintType & AEHPT_MARKTEXT))
      {
        if (AE_IndexCompare(&to->ciDrawLine, &hlp->mtm.crMarkText.ciMax) < 0)
        {
          if (AE_IndexCompare(&hlp->mtm.crMarkText.ciMin, &to->ciDrawLine) <= 0)
          {
            if (!(hlp->dwPaintType & AEHPT_SELECTION))
            {
              //Draw text before mark
              AE_PaintTextOut(ae, to, hlp);
            }
            hlp->dwPaintType|=AEHPT_MARKTEXT;
          }
        }
      }
      if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_MARKTEXT))
      {
        if (hlp->mtm.lpMarkText->crText != (DWORD)-1)
          hlp->dwActiveText=hlp->mtm.lpMarkText->crText;
        else
          hlp->dwActiveText=hlp->dwDefaultText;
        if (hlp->mtm.lpMarkText->crBk != (DWORD)-1)
          hlp->dwActiveBG=hlp->mtm.lpMarkText->crBk;
        else
          hlp->dwActiveBG=hlp->dwDefaultBG;
        if (hlp->mtm.lpMarkText->dwFontStyle != AEHLS_NONE)
          hlp->dwFontStyle=hlp->mtm.lpMarkText->dwFontStyle;
      }
    }
  }
  if (!hlp->dwPaintType)
  {
    hlp->dwActiveText=hlp->dwDefaultText;
    hlp->dwActiveBG=hlp->dwDefaultBG;
    hlp->dwFontStyle=AEHLS_NONE;
  }
}

void AE_PaintCheckHighlightCloseItem(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp)
{
  if (ae->popt->bDetectUrl)
  {
    if (hlp->crLink.ciMin.lpLine && hlp->crLink.ciMax.lpLine)
    {
      if (AE_IndexCompare(&hlp->crLink.ciMax, &to->ciDrawLine) <= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AE_IndexCompare(&hlp->crLink.ciMax, &to->ciDrawLine) == 0)
          {
            //Draw full highlighted text or last part of it
            AE_PaintTextOut(ae, to, hlp);
          }
        }
        hlp->dwPaintType&=~AEHPT_LINK;
        hlp->crLink.ciMin.lpLine=NULL;
        hlp->crLink.ciMax.lpLine=NULL;
      }
    }
  }
  if (ae->popt->lpActiveTheme)
  {
    if (hlp->mtm.lpMarkText)
    {
      if (AE_IndexCompare(&hlp->mtm.crMarkText.ciMax, &to->ciDrawLine) <= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AE_IndexCompare(&hlp->mtm.crMarkText.ciMax, &to->ciDrawLine) == 0)
          {
            //Draw full highlighted text or last part of it
            AE_PaintTextOut(ae, to, hlp);
          }
        }
        hlp->dwPaintType&=~AEHPT_MARKTEXT;
        hlp->mtm.lpMarkText=NULL;
      }
    }
    if (hlp->mrm.lpMarkRange)
    {
      if (hlp->mrm.crMarkRange.cpMax <= to->nDrawCharOffset)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (hlp->mrm.crMarkRange.cpMax == to->nDrawCharOffset)
          {
            //Draw full highlighted text or last part of it
            AE_PaintTextOut(ae, to, hlp);
          }
        }
        hlp->dwPaintType&=~AEHPT_MARKRANGE;
        hlp->mrm.lpMarkRange=NULL;
      }
    }
    if (hlp->qm.lpQuote)
    {
      if (((hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AE_IndexCompare(&hlp->qm.crQuoteEnd.ciMin, &to->ciDrawLine) <= 0) ||
          (!(hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AE_IndexCompare(&hlp->qm.crQuoteEnd.ciMax, &to->ciDrawLine) <= 0))
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (((hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AE_IndexCompare(&hlp->qm.crQuoteEnd.ciMin, &to->ciDrawLine) == 0) ||
              (!(hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AE_IndexCompare(&hlp->qm.crQuoteEnd.ciMax, &to->ciDrawLine) == 0))
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText)
            {
              AE_PaintTextOut(ae, to, hlp);
            }
          }
        }
        hlp->dwPaintType&=~AEHPT_QUOTE;
        hlp->qm.lpQuote=NULL;
      }
    }
    if (hlp->wm.lpDelim1)
    {
      if (AE_IndexCompare(&hlp->wm.crDelim1.ciMax, &to->ciDrawLine) <= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AE_IndexCompare(&hlp->wm.crDelim1.ciMax, &to->ciDrawLine) == 0)
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText && !hlp->qm.lpQuote)
            {
              AE_PaintTextOut(ae, to, hlp);
            }
          }
        }
        hlp->dwPaintType&=~AEHPT_DELIM1;
        hlp->wm.lpDelim1=NULL;
      }
    }
    if (hlp->wm.lpWord)
    {
      if (AE_IndexCompare(&hlp->wm.crWord.ciMax, &to->ciDrawLine) <= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AE_IndexCompare(&hlp->wm.crWord.ciMax, &to->ciDrawLine) == 0)
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText && !hlp->qm.lpQuote)
            {
              AE_PaintTextOut(ae, to, hlp);
            }
          }
        }
        hlp->dwPaintType&=~AEHPT_WORD;
        hlp->wm.lpWord=NULL;
      }
    }
    if (hlp->wm.lpDelim2)
    {
      if (AE_IndexCompare(&hlp->wm.crDelim2.ciMax, &to->ciDrawLine) <= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AE_IndexCompare(&hlp->wm.crDelim2.ciMax, &to->ciDrawLine) == 0)
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText && !hlp->qm.lpQuote)
            {
              AE_PaintTextOut(ae, to, hlp);
            }
          }
        }
        hlp->dwPaintType&=~AEHPT_DELIM2;
        hlp->wm.lpDelim2=NULL;
      }
    }
  }
}

void AE_PaintCheckHighlightCleanUp(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, AECHARINDEX *ciChar)
{
  if (ae->popt->lpActiveTheme)
  {
    if (hlp->wm.lpDelim1)
    {
      if (AE_IndexCompare(&hlp->wm.crDelim1.ciMax, ciChar) <= 0)
      {
        hlp->dwPaintType&=~AEHPT_DELIM1;
        hlp->wm.lpDelim1=NULL;
      }
    }
    if (hlp->wm.lpWord)
    {
      if (AE_IndexCompare(&hlp->wm.crWord.ciMax, ciChar) <= 0)
      {
        hlp->dwPaintType&=~AEHPT_WORD;
        hlp->wm.lpWord=NULL;
      }
    }
    if (hlp->wm.lpDelim2)
    {
      if (AE_IndexCompare(&hlp->wm.crDelim2.ciMax, ciChar) <= 0)
      {
        hlp->dwPaintType&=~AEHPT_DELIM2;
        hlp->wm.lpDelim2=NULL;
      }
    }
    if (hlp->qm.lpQuote)
    {
      if (AE_IndexCompare(&hlp->qm.crQuoteEnd.ciMax, ciChar) <= 0)
      {
        hlp->dwPaintType&=~AEHPT_QUOTE;
        hlp->qm.lpQuote=NULL;
      }
    }
    if (hlp->mrm.lpMarkRange)
    {
      if (hlp->mrm.crMarkRange.cpMax <= to->nDrawCharOffset)
      {
        hlp->dwPaintType&=~AEHPT_MARKRANGE;
        hlp->mrm.lpMarkRange=NULL;
      }
    }
    if (hlp->mtm.lpMarkText)
    {
      if (AE_IndexCompare(&hlp->mtm.crMarkText.ciMax, ciChar) <= 0)
      {
        hlp->dwPaintType&=~AEHPT_MARKTEXT;
        hlp->mtm.lpMarkText=NULL;
      }
    }
  }
  if (ae->popt->bDetectUrl)
  {
    if (hlp->crLink.ciMin.lpLine && hlp->crLink.ciMax.lpLine)
    {
      if (AE_IndexCompare(&hlp->crLink.ciMax, ciChar) <= 0)
      {
        hlp->dwPaintType&=~AEHPT_LINK;
        hlp->crLink.ciMin.lpLine=NULL;
        hlp->crLink.ciMax.lpLine=NULL;
      }
    }
  }
}

void AE_MButtonDraw(AKELEDIT *ae)
{
  if (ae->bMButtonDown)
  {
    if (!ae->bMButtonBitmapDraw)
    {
      HDC hdcSkin;
      HDC hDC=ae->hDC;
      HBITMAP hBitmapOld=NULL;

      if (hDC || (hDC=GetDC(ae->hWndEdit)))
      {
        if (hdcSkin=CreateCompatibleDC(hDC))
        {
          hBitmapOld=(HBITMAP)SelectObject(hdcSkin, ae->hMButtonBitmap);
          BitBlt(hDC, ae->ptMButtonDown.x - 11, ae->ptMButtonDown.y - 11, 22, 22, hdcSkin, 0, 0, NOTSRCINVERT);
          if (hBitmapOld) SelectObject(hdcSkin, hBitmapOld);
          DeleteDC(hdcSkin);
        }
        if (!ae->hDC) ReleaseDC(ae->hWndEdit, hDC);
      }
      ae->ptMButtonScroll.x=ae->nHScrollPos;
      ae->ptMButtonScroll.y=ae->nVScrollPos;
      ae->bMButtonBitmapDraw=TRUE;
    }
  }
}

void AE_MButtonErase(AKELEDIT *ae)
{
  if (ae->bMButtonDown)
  {
    if (ae->bMButtonBitmapDraw)
    {
      RECT rcMButtonDown;

      rcMButtonDown.left=ae->ptMButtonDown.x + (ae->ptMButtonScroll.x - ae->nHScrollPos) - 11;
      rcMButtonDown.top=ae->ptMButtonDown.y + (ae->ptMButtonScroll.y - ae->nVScrollPos) - 11;
      rcMButtonDown.right=ae->ptMButtonDown.x + (ae->ptMButtonScroll.x - ae->nHScrollPos) + 11;
      rcMButtonDown.bottom=ae->ptMButtonDown.y + (ae->ptMButtonScroll.y - ae->nVScrollPos) + 11;

      InvalidateRect(ae->hWndEdit, &rcMButtonDown, TRUE);

      ae->bMButtonBitmapDraw=FALSE;
    }
  }
}

void AE_ActiveColumnCreate(AKELEDIT *ae)
{
  AEBITMAPITEM *bi;
  AEBITMAPDATA bd;

  bd.nWidth=8;
  bd.nHeight=8;
  bd.crBasic=ae->popt->crActiveColumn;
  bd.crInvert=ae->popt->crBasicBk;
  bd.bZebra=TRUE;

  if (!(bi=AE_StackBitmapItemGet(&hAkelEditBitmapsStack, &bd)))
    bi=AE_StackBitmapItemInsert(&hAkelEditBitmapsStack, &bd);
  if (!bi->hPatternBrush)
    bi->hPatternBrush=CreatePatternBrush(bi->hBitmap);
  ae->popt->hActiveColumn=bi->hPatternBrush;
}

void AE_ActiveColumnDraw(AKELEDIT *ae)
{
  if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
  {
    HBRUSH hBrushOld;
    HDC hDC=ae->hDC;

    if (ae->ptActiveColumnDraw.x >= ae->rcDraw.left && ae->ptActiveColumnDraw.x <= ae->rcDraw.right)
    {
      if (hDC || (hDC=GetDC(ae->hWndEdit)))
      {
        hBrushOld=(HBRUSH)SelectObject(hDC, ae->popt->hActiveColumn);
        if (ae->ptActiveColumnDraw.y > ae->rcDraw.top)
          PatBlt(hDC, ae->ptActiveColumnDraw.x, ae->rcDraw.top, 1, ae->ptActiveColumnDraw.y - ae->rcDraw.top, PATINVERT);
        if (ae->rcDraw.bottom > ae->ptActiveColumnDraw.y + ae->ptxt->nCharHeight)
          PatBlt(hDC, ae->ptActiveColumnDraw.x, ae->ptActiveColumnDraw.y + ae->ptxt->nCharHeight, 1, ae->rcDraw.bottom - (ae->ptActiveColumnDraw.y + ae->ptxt->nCharHeight), PATINVERT);
        SelectObject(hDC, hBrushOld);

        if (!ae->hDC) ReleaseDC(ae->hWndEdit, hDC);
      }
    }
  }
}

void AE_ActiveColumnErase(AKELEDIT *ae)
{
  if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
  {
    if (ae->popt->bActiveColumnDraw)
    {
      AE_ActiveColumnDraw(ae);
      ae->popt->bActiveColumnDraw=FALSE;
    }
  }
}

void AE_ColumnMarkerDraw(AKELEDIT *ae)
{
  if (ae->popt->dwColumnMarkerPos)
  {
    int nMarkerPos=0;
    HDC hDC=ae->hDC;
    HPEN hPen;
    HPEN hPenOld;

    if (ae->popt->dwColumnMarkerType == AEMT_SYMBOL)
      nMarkerPos=ae->popt->dwColumnMarkerPos * ae->ptxt->nAveCharWidth;
    else if (ae->popt->dwColumnMarkerType == AEMT_PIXEL)
      nMarkerPos=ae->popt->dwColumnMarkerPos;

    if (ae->nHScrollPos < nMarkerPos && nMarkerPos < ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left))
    {
      if (hDC || (hDC=GetDC(ae->hWndEdit)))
      {
        if (hPen=CreatePen(PS_SOLID, 0, ae->popt->crColumnMarker))
        {
          hPenOld=(HPEN)SelectObject(hDC, hPen);
          MoveToEx(hDC, ae->rcDraw.left + (nMarkerPos - ae->nHScrollPos), ae->rcDraw.top, NULL);
          LineTo(hDC, ae->rcDraw.left + (nMarkerPos - ae->nHScrollPos), ae->rcDraw.bottom);
          if (hPenOld) SelectObject(hDC, hPenOld);
          DeleteObject(hPen);
        }
        if (!ae->hDC) ReleaseDC(ae->hWndEdit, hDC);
      }
    }
  }
}

void AE_ColumnMarkerErase(AKELEDIT *ae)
{
  if (ae->popt->dwColumnMarkerPos)
  {
    RECT rcMarker;
    int nMarkerPos=0;

    if (ae->popt->dwColumnMarkerType == AEMT_SYMBOL)
      nMarkerPos=ae->popt->dwColumnMarkerPos * ae->ptxt->nAveCharWidth;
    else if (ae->popt->dwColumnMarkerType == AEMT_PIXEL)
      nMarkerPos=ae->popt->dwColumnMarkerPos;

    if (ae->nHScrollPos < nMarkerPos && nMarkerPos < ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left))
    {
      rcMarker.left=ae->rcDraw.left + (nMarkerPos - ae->nHScrollPos);
      rcMarker.top=ae->rcDraw.top;
      rcMarker.right=rcMarker.left + 1;
      rcMarker.bottom=ae->rcDraw.bottom;
      InvalidateRect(ae->hWndEdit, &rcMarker, TRUE);
    }
  }
}

void AE_UpdateSize(AKELEDIT *ae)
{
  RECT rcClient;
  int nDrawWidth=ae->rcDraw.right - ae->rcDraw.left;

  GetClientRect(ae->hWndEdit, &rcClient);
  ae->rcDraw.right+=rcClient.right - ae->rcEdit.right;
  ae->rcDraw.bottom+=rcClient.bottom - ae->rcEdit.bottom;
  ae->rcEdit.right=rcClient.right;
  ae->rcEdit.bottom=rcClient.bottom;

  AE_SetDrawRect(ae, &ae->rcDraw, FALSE);
  if (ae->ptxt->dwWordWrap)
  {
    if (nDrawWidth != ae->rcDraw.right - ae->rcDraw.left)
    {
      AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
      AE_StackUpdateClones(ae);
    }
  }
  AE_UpdateScrollBars(ae, SB_BOTH);
  AE_UpdateEditWindow(ae->hWndEdit, TRUE);
}

void AE_UpdateEditWindow(HWND hWndEdit, BOOL bErase)
{
  HWND hWndParent;
  RECT rcUpdate;

  GetWindowRect(hWndEdit, &rcUpdate);

  hWndParent=GetParent(hWndEdit);
  ScreenToClient(hWndParent, (POINT *)&rcUpdate.left);
  ScreenToClient(hWndParent, (POINT *)&rcUpdate.right);
  InvalidateRect(hWndParent, &rcUpdate, bErase);
}

void AE_RedrawLineRange(AKELEDIT *ae, int nFirstLine, int nLastLine, BOOL bErase)
{
  RECT rcRedraw;

  rcRedraw.left=ae->rcDraw.left;
  rcRedraw.right=ae->rcDraw.right;
  if (nFirstLine == -1)
    rcRedraw.top=ae->rcDraw.top;
  else
    rcRedraw.top=(AE_VPos(ae, nFirstLine, AEVPF_VPOSFROMLINE) - ae->nVScrollPos) + ae->rcDraw.top;
  if (nLastLine == -1)
    rcRedraw.bottom=ae->rcDraw.bottom;
  else
    rcRedraw.bottom=(AE_VPos(ae, nLastLine, AEVPF_VPOSFROMLINE) + ae->ptxt->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;

  rcRedraw.top=max(rcRedraw.top, ae->rcDraw.top);
  rcRedraw.bottom=min(rcRedraw.bottom, ae->rcDraw.bottom);

  InvalidateRect(ae->hWndEdit, &rcRedraw, bErase);
}

void AE_HideSelection(AKELEDIT *ae, BOOL bHide)
{
  ae->popt->bHideSelection=bHide;
  AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
  UpdateWindow(ae->hWndEdit);
}

int AE_GetFirstVisibleLine(AKELEDIT *ae)
{
  int nFirstLine;

  nFirstLine=AE_VPos(ae, ae->nVScrollPos, AEVPF_LINEFROMVPOS);

  return max(nFirstLine, 0);
}

int AE_GetLastVisibleLine(AKELEDIT *ae)
{
  int nLastLine;

  nLastLine=AE_VPos(ae, ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top), AEVPF_LINEFROMVPOS);

  return min(nLastLine, ae->ptxt->nLineCount);
}

int AE_GetFirstFullVisibleLine(AKELEDIT *ae)
{
  int nFirstLine;

  nFirstLine=AE_GetFirstVisibleLine(ae);
  if (AE_VPos(ae, nFirstLine, AEVPF_VPOSFROMLINE) < ae->nVScrollPos)
     nFirstLine=min(nFirstLine + 1, ae->ptxt->nLineCount);

  return nFirstLine;
}

int AE_GetLastFullVisibleLine(AKELEDIT *ae)
{
  int nLastLine;

  nLastLine=AE_GetLastVisibleLine(ae);
  if (AE_VPos(ae, nLastLine + 1, AEVPF_VPOSFROMLINE) > ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top))
     nLastLine=max(nLastLine - 1, 0);

  return nLastLine;
}

int AE_GetTextExtentPoint32(AKELEDIT *ae, const wchar_t *wpString, int nStringLen, SIZE *lpSize)
{
  HDC hDC=ae->hDC;
  SIZE sizeChar;
  int nStringWidth=0;
  int i;
  int nResult=0;
  HFONT hFontOld=NULL;

  for (i=0; i < nStringLen; ++i)
  {
    if (ae->ptxt->lpCharWidths[wpString[i]])
    {
      nStringWidth+=ae->ptxt->lpCharWidths[wpString[i]];
      ++nResult;
    }
    else
    {
      if (!hDC)
      {
        if (hDC=GetDC(ae->hWndEdit))
          if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(hDC, ae->ptxt->hFont);
      }
      if (hDC)
      {
        if (wpString[i] != '\t')
        {
          //Surrogate character
          if (AE_IsSurrogate(wpString[i]))
          {
            if (i + 1 < nStringLen)
            {
              if (AE_IsHighSurrogate(wpString[i]) && AE_IsLowSurrogate(wpString[i + 1]))
              {
                GetTextExtentPoint32W(hDC, wpString + i, 2, &sizeChar);
                ++i;
                nStringWidth+=sizeChar.cx;
                ++nResult;
              }
            }
          }
          else
          {
            GetTextExtentPoint32W(hDC, &wpString[i], 1, &sizeChar);
            ae->ptxt->lpCharWidths[wpString[i]]=(WORD)sizeChar.cx;
            nStringWidth+=sizeChar.cx;
            ++nResult;
          }
        }
      }
    }
  }
  if (!ae->hDC && hDC)
  {
    if (hFontOld) SelectObject(hDC, hFontOld);
    ReleaseDC(ae->hWndEdit, hDC);
  }

  if (nResult)
  {
    lpSize->cx=nStringWidth;
    lpSize->cy=ae->ptxt->nCharHeight;
  }
  return nResult;
}

int AE_GetCharWidth(AKELEDIT *ae, wchar_t *wpChar, int *nDone)
{
  SIZE sizeChar;
  int nCharLen;

  if (AE_IsHighSurrogate(*wpChar))
  {
    if (AE_IsLowSurrogate(*(wpChar + 1)))
      nCharLen=2;
    else
      nCharLen=0;
  }
  else nCharLen=1;

  if (nCharLen)
    AE_GetTextExtentPoint32(ae, wpChar, nCharLen, &sizeChar);
  else
    sizeChar.cx=0;
  if (nDone) *nDone=nCharLen;
  return sizeChar.cx;
}

int AE_GetStringWidth(AKELEDIT *ae, wchar_t *wpString, int nStringLen, int nFirstCharExtent)
{
  SIZE sizeChar;
  int nStringWidth=nFirstCharExtent;
  int nStringCount=0;
  int nTabWidth;
  int i;

  for (i=0; i < nStringLen; ++i)
  {
    if (wpString[i] == L'\t')
    {
      if (AE_GetTextExtentPoint32(ae, wpString + nStringCount, i - nStringCount, &sizeChar))
        nStringWidth+=sizeChar.cx;
      nStringCount=i + 1;

      nTabWidth=ae->ptxt->nTabWidth - nStringWidth % ae->ptxt->nTabWidth;
      nStringWidth+=nTabWidth;
    }
  }
  if (AE_GetTextExtentPoint32(ae, wpString + nStringCount, i - nStringCount, &sizeChar))
    nStringWidth+=sizeChar.cx;
  return nStringWidth - nFirstCharExtent;
}

void AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine)
{
  lpLine->nLineWidth=AE_GetStringWidth(ae, lpLine->wpLine, lpLine->nLineLen, 0);
}

BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos)
{
  AECHARINDEX ciInitial;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nOffset=0;
  int nStringWidth=0;
  int nStartChar=0;
  int i;

  if (ciCharIndex->lpLine)
  {
    //Normalize
    ciInitial=*ciCharIndex;
    AE_IndexNormalize(&ciInitial);

    dwFirst=mod(ciInitial.nCharInLine - 0);
    if (ciInitial.lpLine->nLineWidth != -1)
      dwSecond=mod(ciInitial.nCharInLine - ciInitial.lpLine->nLineLen);
    if (ae->ptCaret.x && ciInitial.lpLine == ae->ciCaretIndex.lpLine)
      dwThird=mod(ciInitial.nCharInLine - ae->ciCaretIndex.nCharInLine);

    if (dwFirst <= dwSecond && dwFirst <= dwThird)
    {
      nOffset=ciInitial.nCharInLine - 0;
      nStringWidth=0;
      nStartChar=0;
    }
    else if (dwSecond <= dwFirst && dwSecond <= dwThird)
    {
      nOffset=ciInitial.nCharInLine - ciInitial.lpLine->nLineLen;
      nStringWidth=ciInitial.lpLine->nLineWidth;
      nStartChar=ciInitial.lpLine->nLineLen;
    }
    else if (dwThird <= dwFirst && dwThird <= dwSecond)
    {
      nOffset=ciInitial.nCharInLine - ae->ciCaretIndex.nCharInLine;
      nStringWidth=ae->ptCaret.x;
      nStartChar=ae->ciCaretIndex.nCharInLine;
    }

    Begin:
    if (nOffset > 0)
    {
      if (nStartChar < ciInitial.lpLine->nLineLen)
        nStringWidth+=AE_GetStringWidth(ae, ciInitial.lpLine->wpLine + nStartChar, min(ciInitial.nCharInLine, ciInitial.lpLine->nLineLen) - nStartChar, nStringWidth);
      if (ciInitial.nCharInLine > ciInitial.lpLine->nLineLen)
        nStringWidth+=(ciInitial.nCharInLine - max(ciInitial.lpLine->nLineLen, nStartChar)) * ae->ptxt->nSpaceCharWidth;
    }
    else if (nOffset < 0)
    {
      if (nStartChar > ciInitial.lpLine->nLineLen)
        nStringWidth-=(nStartChar - max(ciInitial.lpLine->nLineLen, ciInitial.nCharInLine)) * ae->ptxt->nSpaceCharWidth;

      if (ciInitial.nCharInLine < ciInitial.lpLine->nLineLen)
      {
        for (i=min(nStartChar, ciInitial.lpLine->nLineLen) - 1; i >= 0; --i)
        {
          if (ciInitial.lpLine->wpLine[i] == L'\t')
          {
            nOffset=ciInitial.nCharInLine;
            nStringWidth=0;
            nStartChar=0;
            goto Begin;
          }
          if (i <= ciInitial.nCharInLine)
            break;
        }
        nStringWidth-=AE_GetStringWidth(ae, ciInitial.lpLine->wpLine + ciInitial.nCharInLine, min(nStartChar, ciInitial.lpLine->nLineLen) - ciInitial.nCharInLine, 0);
      }
    }

    if (ptGlobalPos)
    {
      ptGlobalPos->x=nStringWidth;
      ptGlobalPos->y=AE_VPos(ae, ciInitial.nLine, AEVPF_VPOSFROMLINE);
    }
    if (ptClientPos)
    {
      ptClientPos->x=(nStringWidth - ae->nHScrollPos) + ae->rcDraw.left;
      ptClientPos->y=(AE_VPos(ae, ciInitial.nLine, AEVPF_VPOSFROMLINE) - ae->nVScrollPos) + ae->rcDraw.top;
    }
    return TRUE;
  }
  return FALSE;
}

int AE_GetCharInLine(AKELEDIT *ae, const AELINEDATA *lpLine, int nMaxExtent, DWORD dwFlags, int *nCharIndex, int *nCharPos, BOOL bColumnSel)
{
  AECHARINDEX ciChar;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nOffset=0;
  int nStringWidth=0;
  int nStartChar=0;
  int nCharWidth=0;
  int nTabWidth;
  int nHScrollMax;

  if (lpLine)
  {
    ciChar.nLine=0;
    ciChar.lpLine=(AELINEDATA *)lpLine;
    ciChar.nCharInLine=0;
    nMaxExtent=max(nMaxExtent, 0);

    if (dwFlags & AECIL_USELINEBEGINPOS)
    {
      dwFirst=mod(nMaxExtent - 0);
    }
    if (dwFlags & AECIL_USELINEENDPOS)
    {
      if (lpLine->nLineWidth != -1)
        dwSecond=mod(nMaxExtent - lpLine->nLineWidth);
    }
    if (dwFlags & AECIL_USECARETPOS)
    {
      if (ae->ptCaret.x && lpLine == ae->ciCaretIndex.lpLine)
        dwThird=mod(nMaxExtent - ae->ptCaret.x);
    }

    if (dwFirst <= dwSecond && dwFirst <= dwThird)
    {
      nOffset=nMaxExtent - 0;
      nStringWidth=0;
      nStartChar=0;
    }
    else if (dwSecond <= dwFirst && dwSecond <= dwThird)
    {
      nOffset=nMaxExtent - lpLine->nLineWidth;
      nStringWidth=lpLine->nLineWidth;
      nStartChar=lpLine->nLineLen;
    }
    else if (dwThird <= dwFirst && dwThird <= dwSecond)
    {
      nOffset=nMaxExtent - ae->ptCaret.x;
      nStringWidth=ae->ptCaret.x;
      nStartChar=ae->ciCaretIndex.nCharInLine;
    }

    Begin:
    if (nOffset > 0)
    {
      for (ciChar.nCharInLine=nStartChar; nStringWidth < nMaxExtent; AE_IndexInc(&ciChar))
      {
        if (ciChar.nCharInLine < ciChar.lpLine->nLineLen)
        {
          if (ciChar.lpLine->wpLine[ciChar.nCharInLine] == L'\t')
          {
            nTabWidth=ae->ptxt->nTabWidth - nStringWidth % ae->ptxt->nTabWidth;
            nCharWidth=nTabWidth;
            nStringWidth+=nCharWidth;
          }
          else
          {
            if (nCharWidth=AE_GetCharWidth(ae, ciChar.lpLine->wpLine + ciChar.nCharInLine, NULL))
              nStringWidth+=nCharWidth;
          }
        }
        else
        {
          if (bColumnSel)
          {
            nCharWidth=ae->ptxt->nSpaceCharWidth;
            nStringWidth+=nCharWidth;
          }
          else
          {
            if (lpLine->nLineWidth != -1)
            {
              nStringWidth=lpLine->nLineWidth;
              ciChar.nCharInLine=lpLine->nLineLen;
            }
            goto End;
          }
        }
      }

      if (ciChar.nCharInLine > 0)
      {
        if (nStringWidth > nMaxExtent)
        {
          nHScrollMax=max(ae->ptxt->nHScrollMax, (ae->rcDraw.right - ae->rcDraw.left));

          if ((dwFlags & AECIL_HALFFIT) && nStringWidth + nCharWidth <= nHScrollMax - ae->ptxt->nAveCharWidth)
          {
            if (nStringWidth - nMaxExtent > nCharWidth / 2)
            {
              nStringWidth-=nCharWidth;
              AE_IndexDec(&ciChar);
            }
          }
          else
          {
            nStringWidth-=nCharWidth;
            AE_IndexDec(&ciChar);
          }
        }
      }
    }
    else if (nOffset < 0)
    {
      ciChar.nCharInLine=nStartChar;
      AE_IndexDec(&ciChar);

      for (; ciChar.nCharInLine >= 0 && nStringWidth > nMaxExtent; AE_IndexDec(&ciChar))
      {
        if (ciChar.nCharInLine < ciChar.lpLine->nLineLen)
        {
          if (ciChar.lpLine->wpLine[ciChar.nCharInLine] == L'\t')
          {
            nOffset=nMaxExtent;
            nStringWidth=0;
            nStartChar=0;
            goto Begin;
          }
          else
          {
            if (nCharWidth=AE_GetCharWidth(ae, ciChar.lpLine->wpLine + ciChar.nCharInLine, NULL))
              nStringWidth-=nCharWidth;
          }
        }
        else
        {
          if (bColumnSel)
          {
            nCharWidth=ae->ptxt->nSpaceCharWidth;
            nStringWidth-=nCharWidth;
          }
          else
          {
            nStringWidth=ciChar.lpLine->nLineWidth;
            ciChar.nCharInLine=ciChar.lpLine->nLineLen;
            goto End;
          }
        }
      }
      AE_IndexInc(&ciChar);

      if (nStringWidth < nMaxExtent)
      {
        if ((dwFlags & AECIL_HALFFIT))
        {
          if (nMaxExtent - nStringWidth >= nCharWidth / 2)
          {
            nStringWidth+=nCharWidth;
            AE_IndexInc(&ciChar);
          }
        }
      }
    }
    else
    {
      ciChar.nCharInLine=nStartChar;

      if (!bColumnSel)
      {
        if (ciChar.nCharInLine >= ciChar.lpLine->nLineLen)
        {
          nStringWidth=ciChar.lpLine->nLineWidth;
          ciChar.nCharInLine=ciChar.lpLine->nLineLen;
          goto End;
        }
      }
    }

    End:
    if (nCharPos) *nCharPos=nStringWidth;
    if (nCharIndex) *nCharIndex=ciChar.nCharInLine;

    if (nMaxExtent == nStringWidth)
      return AEPC_EQUAL;
    if (nMaxExtent > nStringWidth)
      return AEPC_BEFORE;
    return AEPC_AFTER;
  }
  return AEPC_ERROR;
}

BOOL AE_GetCharRangeInLine(AKELEDIT *ae, AELINEDATA *lpLine, int nMinExtent, int nMaxExtent, int *nMinCharIndex, int *nMinCharPos, int *nMaxCharIndex, int *nMaxCharPos, BOOL bColumnSel)
{
  int nMinIndex=0;
  int nMaxIndex=0;
  int nMinPos=0;
  int nMaxPos=0;

  if (nMinCharIndex || nMinCharPos)
  {
    if (!AE_GetCharInLine(ae, lpLine, nMinExtent, AECIL_HALFFIT|AECIL_ALLPOS, &nMinIndex, &nMinPos, bColumnSel))
      return FALSE;
  }
  if (nMaxCharIndex || nMaxCharPos)
  {
    if (!AE_GetCharInLine(ae, lpLine, nMaxExtent, AECIL_HALFFIT|AECIL_ALLPOS, &nMaxIndex, &nMaxPos, bColumnSel))
      return FALSE;
  }
  if (nMinCharPos) *nMinCharPos=nMinPos;
  if (nMaxCharPos) *nMaxCharPos=nMaxPos;
  if (nMinCharIndex) *nMinCharIndex=nMinIndex;
  if (nMaxCharIndex) *nMaxCharIndex=nMaxIndex;
  return TRUE;
}

int AE_GetCharFromPos(AKELEDIT *ae, const POINT *ptClientPos, AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, BOOL bColumnSel)
{
  int nCharPos;
  int nMaxExtent;
  int nFirstLine=0;
  int nLastLine=ae->ptxt->nLineCount;
  int nHScrollMax;
  int nResult;

  ciCharIndex->nLine=AE_VPos(ae, ae->nVScrollPos + (ptClientPos->y - ae->rcDraw.top), AEVPF_LINEFROMVPOS);
  ciCharIndex->nLine=max(ciCharIndex->nLine, nFirstLine);
  ciCharIndex->nLine=min(ciCharIndex->nLine, nLastLine);
  ciCharIndex->nCharInLine=0;
  if (ptGlobalPos) ptGlobalPos->y=AE_VPos(ae, ciCharIndex->nLine, AEVPF_VPOSFROMLINE);

  nMaxExtent=ae->nHScrollPos + (ptClientPos->x - ae->rcDraw.left);
  nHScrollMax=max(ae->ptxt->nHScrollMax, (ae->rcDraw.right - ae->rcDraw.left));
  nMaxExtent=min(nHScrollMax - ae->ptxt->nAveCharWidth, nMaxExtent);

  if (AE_IndexUpdate(ae, ciCharIndex))
  {
    if (nResult=AE_GetCharInLine(ae, ciCharIndex->lpLine, nMaxExtent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharIndex->nCharInLine, &nCharPos, bColumnSel))
    {
      if (ptGlobalPos) ptGlobalPos->x=nCharPos;
      return nResult;
    }
  }
  return AEPC_ERROR;
}

int AE_GetNextBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciNextBreak, BOOL bColumnSel, DWORD dwFlags)
{
  AECHARINDEX ciCount=*ciChar;
  int nLen=0;
  BOOL bInList;
  BOOL bIsSpacePrevious=FALSE;
  BOOL bIsSpaceCurrent;

  if (ciCount.nCharInLine > ciCount.lpLine->nLineLen)
    return 0;

  if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
  {
    if (ciCount.lpLine->next)
    {
      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AE_NextLine(&ciCount);
    }
    else goto End;
  }

  if (ciCount.nCharInLine >= ciCount.lpLine->nLineLen)
  {
    if (bInList=AE_IsInDelimiterList(ae->popt->wszWordDelimiters, L'\n', TRUE))
      bIsSpacePrevious=TRUE;

    AE_NextLine(&ciCount);
    ++nLen;
  }
  else
  {
    if (bInList=AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine], TRUE))
      bIsSpacePrevious=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);

    nLen+=AE_IndexLen(&ciCount);
    AE_IndexInc(&ciCount);
  }

  for (;;)
  {
    while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
    {
      bIsSpaceCurrent=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);

      if (bInList)
      {
        if (dwFlags & AEWB_STOPSPACESTART)
        {
          if (!bIsSpacePrevious && bIsSpaceCurrent)
            goto End;
        }
        if (dwFlags & AEWB_STOPSPACEEND)
        {
          if (bIsSpacePrevious && !bIsSpaceCurrent)
            goto End;
        }
      }
      if (bInList != AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine], TRUE))
      {
        if (bInList)
        {
          if (!((dwFlags & AEWB_SKIPSPACEEND) && bIsSpacePrevious))
          {
            if (dwFlags & AEWB_RIGHTWORDSTART)
              goto End;
          }
          bInList=FALSE;
        }
        else
        {
          if (!((dwFlags & AEWB_SKIPSPACESTART) && bIsSpaceCurrent))
          {
            if (dwFlags & AEWB_RIGHTWORDEND)
              goto End;
          }
          bInList=TRUE;
        }
      }
      bIsSpacePrevious=bIsSpaceCurrent;

      nLen+=AE_IndexLen(&ciCount);
      AE_IndexInc(&ciCount);
    }
    if (bColumnSel) goto End;
    if (ciCount.lpLine->nLineBreak != AELB_WRAP)
    {
      if (bInList)
      {
        if (dwFlags & AEWB_STOPNEWLINE)
          goto End;
        if (dwFlags & AEWB_STOPSPACESTART)
        {
          if (!bIsSpacePrevious)
            goto End;
        }
        bIsSpacePrevious=TRUE;
      }
      if (bInList != AE_IsInDelimiterList(ae->popt->wszWordDelimiters, L'\n', TRUE))
      {
        if (bInList)
        {
          if (dwFlags & AEWB_RIGHTWORDSTART)
            goto End;
        }
        else
        {
          if (dwFlags & AEWB_RIGHTWORDEND)
            goto End;
        }
      }
      ++nLen;
    }

    if (ciCount.lpLine->next)
      AE_NextLine(&ciCount);
    else
      goto End;
  }

  End:
  ciCount.nCharInLine=min(ciCount.nCharInLine, ciCount.lpLine->nLineLen);

  if (AE_IndexCompare(ciChar, &ciCount))
  {
    *ciNextBreak=ciCount;
    return nLen;
  }
  return 0;
}

int AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel, DWORD dwFlags)
{
  AECHARINDEX ciCount=*ciChar;
  int nLen=0;
  BOOL bInList;
  BOOL bIsSpacePrevious=FALSE;
  BOOL bIsSpaceCurrent;

  if (ciCount.nCharInLine > ciCount.lpLine->nLineLen)
    return 0;

  if (ciCount.nCharInLine - 1 < 0)
  {
    if (ciCount.lpLine->prev)
    {
      if (ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
        AE_PrevLine(&ciCount);
    }
    else goto End;
  }
  AE_IndexDec(&ciCount);

  if (ciCount.nCharInLine < 0)
  {
    if (bInList=AE_IsInDelimiterList(ae->popt->wszWordDelimiters, L'\n', TRUE))
      bIsSpacePrevious=TRUE;

    AE_PrevLine(&ciCount);
    AE_IndexDec(&ciCount);
    ++nLen;
  }
  else
  {
    if (bInList=AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine], TRUE))
      bIsSpacePrevious=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);

    AE_IndexDec(&ciCount);
    ++nLen;
  }

  for (;;)
  {
    while (ciCount.nCharInLine >= 0)
    {
      bIsSpaceCurrent=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);

      if (bInList)
      {
        if (dwFlags & AEWB_STOPSPACEEND)
        {
          if (!bIsSpacePrevious && bIsSpaceCurrent)
          {
            AE_IndexInc(&ciCount);
            goto End;
          }
        }
        if (dwFlags & AEWB_STOPSPACESTART)
        {
          if (bIsSpacePrevious && !bIsSpaceCurrent)
          {
            AE_IndexInc(&ciCount);
            goto End;
          }
        }
      }
      if (bInList != AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine], TRUE))
      {
        if (bInList)
        {
          if (!((dwFlags & AEWB_SKIPSPACESTART) && bIsSpacePrevious))
          {
            if (dwFlags & AEWB_LEFTWORDEND)
            {
              AE_IndexInc(&ciCount);
              goto End;
            }
          }
          bInList=FALSE;
        }
        else
        {
          if (!((dwFlags & AEWB_SKIPSPACEEND) && bIsSpaceCurrent))
          {
            if (dwFlags & AEWB_LEFTWORDSTART)
            {
              AE_IndexInc(&ciCount);
              goto End;
            }
          }
          bInList=TRUE;
        }
      }
      bIsSpacePrevious=bIsSpaceCurrent;

      AE_IndexDec(&ciCount);
      ++nLen;
    }
    if (bColumnSel) goto End;
    if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak != AELB_WRAP)
    {
      if (bInList)
      {
        if (dwFlags & AEWB_STOPNEWLINE)
          goto End;
        if (dwFlags & AEWB_STOPSPACEEND)
        {
          if (!bIsSpacePrevious)
            goto End;
        }
        bIsSpacePrevious=TRUE;
      }
      if (bInList != AE_IsInDelimiterList(ae->popt->wszWordDelimiters, L'\n', TRUE))
      {
        if (bInList)
        {
          if (dwFlags & AEWB_LEFTWORDEND)
            goto End;
        }
        else
        {
          if (dwFlags & AEWB_LEFTWORDSTART)
            goto End;
        }
      }
      ++nLen;
    }

    if (ciCount.lpLine->prev)
    {
      AE_PrevLine(&ciCount);
      AE_IndexDec(&ciCount);
    }
    else goto End;
  }

  End:
  ciCount.nCharInLine=max(ciCount.nCharInLine, 0);

  if (AE_IndexCompare(ciChar, &ciCount))
  {
    *ciPrevBreak=ciCount;
    return nLen;
  }
  return 0;
}

int AE_CharAtIndex(const AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
  {
    if (ciChar->lpLine->nLineBreak == AELB_WRAP)
      return ciChar->lpLine->next->wpLine[0];
    return -ciChar->lpLine->nLineBreak;
  }
  return ciChar->lpLine->wpLine[ciChar->nCharInLine];
}

BOOL AE_IsCharInSelection(const AECHARINDEX *ciChar)
{
  if (ciChar->lpLine->nSelStart <= ciChar->nCharInLine && ciChar->nCharInLine < ciChar->lpLine->nSelEnd)
    return TRUE;
  return FALSE;
}

BOOL AE_IsFirstCharInLine(const AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine == 0 && (!ciChar->lpLine->prev || ciChar->lpLine->prev->nLineBreak != AELB_WRAP))
    return TRUE;
  return FALSE;
}

BOOL AE_IsLastCharInLine(const AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine == ciChar->lpLine->nLineLen && ciChar->lpLine->nLineBreak != AELB_WRAP)
    return TRUE;
  return FALSE;
}

BOOL AE_IsSpacesFromLeft(const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;
  wchar_t wchChar;

  if (AE_IsFirstCharInLine(&ciCount))
    return TRUE;

  while (AE_PrevCharInLine(&ciCount))
  {
    wchChar=*(ciCount.lpLine->wpLine + ciCount.nCharInLine);
    if (wchChar != L' ' && wchChar != L'\t')
      return FALSE;
  }
  return TRUE;
}

BOOL AE_IsSpacesFromRight(const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;
  wchar_t wchChar;

  if (AE_IsLastCharInLine(&ciCount))
    return TRUE;

  do
  {
    wchChar=*(ciCount.lpLine->wpLine + ciCount.nCharInLine);
    if (wchChar != L' ' && wchChar != L'\t')
      return FALSE;
  }
  while (AE_NextCharInLine(&ciCount));

  return TRUE;
}

BOOL AE_IsEscaped(const AECHARINDEX *ciChar, wchar_t wchEscape)
{
  AECHARINDEX ciCount=*ciChar;
  int nEscapeCount=0;

  while (AE_PrevCharInLine(&ciCount))
  {
    if (*(ciCount.lpLine->wpLine + ciCount.nCharInLine) == wchEscape)
      ++nEscapeCount;
    else
      break;
  }
  if (nEscapeCount % 2)
    return TRUE;
  return FALSE;
}

BOOL AE_IsDelimiter(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwType)
{
  AECHARINDEX ciTmp=*ciChar;
  int nChar=-AELB_EOF;

  if (dwType & AEDLM_PREVCHAR)
  {
    if (AE_PrevChar(&ciTmp))
      nChar=AE_CharAtIndex(&ciTmp);
  }
  else nChar=AE_CharAtIndex(&ciTmp);

  //Start of file, end of file or end of line
  if (nChar <= -AELB_EOF)
  {
    if (nChar == -AELB_EOF)
      return TRUE;
    nChar=L'\n';
  }

  if (dwType & AEDLM_WORD)
    return AE_IsInDelimiterList(ae->popt->wszWordDelimiters, (wchar_t)nChar, TRUE);
  if (dwType & AEDLM_WRAP)
    return AE_IsInDelimiterList(ae->ptxt->wszWrapDelimiters, (wchar_t)nChar, TRUE);
  if (dwType & AEDLM_URLLEFT)
    return AE_IsInDelimiterList(ae->popt->wszUrlLeftDelimiters, (wchar_t)nChar, TRUE);
  if (dwType & AEDLM_URLRIGHT)
    return AE_IsInDelimiterList(ae->popt->wszUrlRightDelimiters, (wchar_t)nChar, TRUE);
  return FALSE;
}

BOOL AE_IsInDelimiterList(const wchar_t *wpList, wchar_t c, BOOL bMatchCase)
{
  if (AE_wcschr(wpList, c, bMatchCase) != NULL)
    return TRUE;
  else
    return FALSE;
}

BOOL AE_IsSpace(wchar_t c)
{
  if (c == L' ' || c == L'\t')
    return TRUE;
  else
    return FALSE;
}

int AE_GetUrlPrefixes(AKELEDIT *ae)
{
  int nPrefix=0;
  int nStart=0;
  int nEnd=0;

  for (;;)
  {
    if (!ae->popt->wszUrlPrefixes[nEnd])
    {
      if (nStart == nEnd)
      {
        ae->popt->lpUrlPrefixes[nPrefix]=NULL;
        break;
      }
      ae->popt->lpUrlPrefixes[nPrefix++]=ae->popt->wszUrlPrefixes + nStart;
      nStart=++nEnd;
      continue;
    }
    ++nEnd;
  }
  return nPrefix;
}

DWORD AE_GetTextRangeAnsi(AKELEDIT *ae, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefaultChar, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, char *szBuffer, DWORD dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  wchar_t *wszText;
  DWORD dwAnsiBytes;
  DWORD dwUnicodeLen;

  if (dwUnicodeLen=AE_GetTextRange(ae, ciRangeStart, ciRangeEnd, NULL, 0, nNewLine, bColumnSel, bFillSpaces))
  {
    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeLen * sizeof(wchar_t)))
    {
      dwUnicodeLen=AE_GetTextRange(ae, ciRangeStart, ciRangeEnd, wszText, (DWORD)-1, nNewLine, bColumnSel, bFillSpaces);

      //Get Ansi text
      dwAnsiBytes=WideCharToMultiByte(nCodePage, 0, wszText, dwUnicodeLen + 1, NULL, 0, lpDefaultChar, lpUsedDefaultChar);

      if (szBuffer)
      {
        dwAnsiBytes=min(dwAnsiBytes, dwBufferSize);

        if (dwAnsiBytes)
        {
          WideCharToMultiByte(nCodePage, 0, wszText, dwUnicodeLen + 1, szBuffer, dwAnsiBytes, lpDefaultChar, lpUsedDefaultChar);
          szBuffer[--dwAnsiBytes]='\0';
        }
      }
      AE_HeapFree(ae, 0, (LPVOID)wszText);
      return dwAnsiBytes;
    }
  }
  return 0;
}

DWORD AE_GetTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, wchar_t *wszBuffer, DWORD dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  AECHARINDEX ciStart=*ciRangeStart;
  AECHARINDEX ciEnd=*ciRangeEnd;
  AECHARINDEX ciTmp;
  AELINEDATA *lpElement;
  DWORD dwCopied=0;
  int nLineBreak;
  int nCharsToCopy;
  int i;

  if (ciStart.lpLine && ciEnd.lpLine)
  {
    if (bColumnSel == -1) bColumnSel=ae->bColumnSel;

    //Exchange indexes
    if (AE_IndexCompare(&ciStart, &ciEnd) > 0)
    {
      ciTmp=ciStart;
      ciStart=ciEnd;
      ciEnd=ciTmp;
    }

    if (wszBuffer)
    {
      if (dwBufferSize)
      {
        if (AE_IndexCompare(&ciStart, &ciEnd))
        {
          //Set new line
          if (nNewLine < AELB_ASIS)
          {
            if (nNewLine == AELB_ASINPUT)
              nNewLine=ae->popt->nInputNewLine;
            else if (nNewLine == AELB_ASOUTPUT)
              nNewLine=ae->popt->nOutputNewLine;
          }

          if (bColumnSel)
          {
            lpElement=ciStart.lpLine;

            while (lpElement)
            {
              for (i=lpElement->nSelStart; i < lpElement->nLineLen && i < lpElement->nSelEnd; ++i)
              {
                wszBuffer[dwCopied++]=lpElement->wpLine[i];
                if (dwCopied >= dwBufferSize) goto End;
              }
              if (bFillSpaces)
              {
                while (i < lpElement->nSelEnd)
                {
                  wszBuffer[dwCopied++]=L' ';
                  if (dwCopied >= dwBufferSize) goto End;

                  ++i;
                }
              }
              if (lpElement == ciEnd.lpLine) break;

              if (lpElement->nLineBreak == AELB_WRAP)
              {
                if (nNewLine == AELB_ASIS)
                  nLineBreak=AELB_RN;
                else
                  nLineBreak=nNewLine;
              }
              else
              {
                if (nNewLine == AELB_ASIS)
                  nLineBreak=lpElement->nLineBreak;
                else
                  nLineBreak=nNewLine;
              }

              if (nLineBreak == AELB_R)
              {
                wszBuffer[dwCopied++]=L'\r';
                if (dwCopied >= dwBufferSize) goto End;
              }
              else if (nLineBreak == AELB_N)
              {
                wszBuffer[dwCopied++]=L'\n';
                if (dwCopied >= dwBufferSize) goto End;
              }
              else if (nLineBreak == AELB_RN)
              {
                wszBuffer[dwCopied++]=L'\r';
                if (dwCopied >= dwBufferSize) goto End;
                wszBuffer[dwCopied++]=L'\n';
                if (dwCopied >= dwBufferSize) goto End;
              }
              else if (nLineBreak == AELB_RRN)
              {
                wszBuffer[dwCopied++]=L'\r';
                if (dwCopied >= dwBufferSize) goto End;
                wszBuffer[dwCopied++]=L'\r';
                if (dwCopied >= dwBufferSize) goto End;
                wszBuffer[dwCopied++]=L'\n';
                if (dwCopied >= dwBufferSize) goto End;
              }
              lpElement=lpElement->next;
            }
          }
          else
          {
            //Normalize
            ciStart.nCharInLine=min(ciStart.nCharInLine, ciStart.lpLine->nLineLen);
            ciEnd.nCharInLine=min(ciEnd.nCharInLine, ciEnd.lpLine->nLineLen);

            if (ciStart.nLine == ciEnd.nLine)
            {
              nCharsToCopy=min((DWORD)(ciEnd.nCharInLine - ciStart.nCharInLine), dwBufferSize - dwCopied);
              xmemcpy(wszBuffer + dwCopied, ciStart.lpLine->wpLine + ciStart.nCharInLine, nCharsToCopy * sizeof(wchar_t));
              dwCopied+=nCharsToCopy;
              if (dwCopied >= dwBufferSize) goto End;
            }
            else
            {
              lpElement=ciStart.lpLine;

              while (lpElement)
              {
                if (lpElement == ciEnd.lpLine)
                {
                  //Last line
                  nCharsToCopy=min((DWORD)ciEnd.nCharInLine, dwBufferSize - dwCopied);
                  xmemcpy(wszBuffer + dwCopied, ciEnd.lpLine->wpLine, nCharsToCopy * sizeof(wchar_t));
                  dwCopied+=nCharsToCopy;
                  break;
                }

                if (lpElement == ciStart.lpLine)
                {
                  //First line
                  nCharsToCopy=min((DWORD)(ciStart.lpLine->nLineLen - ciStart.nCharInLine), dwBufferSize - dwCopied);
                  xmemcpy(wszBuffer + dwCopied, ciStart.lpLine->wpLine + ciStart.nCharInLine, nCharsToCopy * sizeof(wchar_t));
                  dwCopied+=nCharsToCopy;
                  if (dwCopied >= dwBufferSize) goto End;
                }
                else
                {
                  //Middle
                  nCharsToCopy=min((DWORD)lpElement->nLineLen, dwBufferSize - dwCopied);
                  xmemcpy(wszBuffer + dwCopied, lpElement->wpLine, nCharsToCopy * sizeof(wchar_t));
                  dwCopied+=nCharsToCopy;
                  if (dwCopied >= dwBufferSize) goto End;
                }

                if (lpElement->nLineBreak != AELB_WRAP)
                {
                  if (nNewLine == AELB_ASIS)
                    nLineBreak=lpElement->nLineBreak;
                  else
                    nLineBreak=nNewLine;

                  if (nLineBreak == AELB_R)
                  {
                    wszBuffer[dwCopied++]=L'\r';
                    if (dwCopied >= dwBufferSize) goto End;
                  }
                  else if (nLineBreak == AELB_N)
                  {
                    wszBuffer[dwCopied++]=L'\n';
                    if (dwCopied >= dwBufferSize) goto End;
                  }
                  else if (nLineBreak == AELB_RN)
                  {
                    wszBuffer[dwCopied++]=L'\r';
                    if (dwCopied >= dwBufferSize) goto End;
                    wszBuffer[dwCopied++]=L'\n';
                    if (dwCopied >= dwBufferSize) goto End;
                  }
                  else if (nLineBreak == AELB_RRN)
                  {
                    wszBuffer[dwCopied++]=L'\r';
                    if (dwCopied >= dwBufferSize) goto End;
                    wszBuffer[dwCopied++]=L'\r';
                    if (dwCopied >= dwBufferSize) goto End;
                    wszBuffer[dwCopied++]=L'\n';
                    if (dwCopied >= dwBufferSize) goto End;
                  }
                }
                lpElement=lpElement->next;
              }
            }
          }
        }
        End:
        if (dwCopied >= dwBufferSize && dwCopied > 0)
          --dwCopied;
        wszBuffer[dwCopied]=L'\0';
      }
    }
    else dwCopied=AE_IndexSubtract(ae, &ciEnd, &ciStart, nNewLine, bColumnSel, bFillSpaces) + 1;
  }
  return dwCopied;
}

DWORD AE_SetTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, DWORD dwTextLen, int nNewLine)
{
  wchar_t *wszText;
  DWORD dwUnicodeLen;
  DWORD dwResult=0;

  if (dwUnicodeLen=MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, NULL, 0))
  {
    if (dwTextLen == (DWORD)-1) dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeLen + 1);
      dwResult=AE_SetText(ae, wszText, dwUnicodeLen, nNewLine, FALSE);

      AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
  }
  return dwResult;
}

DWORD AE_SetText(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bOnInitWindow)
{
  AECHARINDEX ciCaretChar;
  AELINEDATA *lpElement=NULL;
  wchar_t *wpLineStart=(wchar_t *)wpText;
  wchar_t *wpLineEnd=(wchar_t *)wpText;
  HDC hDC=ae->hDC;
  HFONT hFontOld=NULL;
  DWORD dwTextCount=0;
  DWORD dwStartTime=GetTickCount();
  DWORD dwProgressTime=0;
  DWORD dwCurrentTime=0;
  int nLinesInPage;
  int nLineBreak;
  BOOL bUpdated=FALSE;
  BOOL bStopProgress=FALSE;

  if (!bOnInitWindow)
  {
    AE_NotifyChanging(ae, AETCT_SETTEXT);
  }

  //Set new line
  if (nNewLine < AELB_ASIS)
  {
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->popt->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->popt->nOutputNewLine;
  }

  //Free old and create new heap
  //ae->bSkipMessages=TRUE;
  AE_HeapCreate(ae);

  //Get DC for faster AE_GetTextExtentPoint32
  if (!ae->hDC)
  {
    if (ae->hDC=GetDC(ae->hWndEdit))
      if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(ae->hDC, ae->ptxt->hFont);
  }
  if (ae->hDC)
  {
    //Get lines in page
    nLinesInPage=(ae->rcDraw.bottom - ae->rcDraw.top) / ae->ptxt->nCharHeight;

    //Parse text
    if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);

    while (dwTextCount < dwTextLen)
    {
      //Progressing
      if (ae->popt->dwEventMask & AENM_PROGRESS)
      {
        dwCurrentTime=GetTickCount();

        if (dwCurrentTime - dwProgressTime > AETIME_BEFOREPROGRESS)
        {
          if (bStopProgress=AE_NotifyProgress(ae, AEPGS_SETTEXT, dwCurrentTime - dwStartTime, dwTextCount, dwTextLen))
            break;
          dwProgressTime=GetTickCount();
        }
      }

      if (lpElement=AE_StackLineAdd(ae))
      {
        wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &lpElement->nLineLen, &lpElement->nLineBreak);
        dwTextCount+=wpLineEnd - wpLineStart;
        ae->ptxt->nLastCharOffset+=lpElement->nLineLen + 1;

        if (lpElement->nLineBreak != AELB_EOF)
        {
          if (nNewLine != AELB_ASIS)
            lpElement->nLineBreak=nNewLine;
        }
        else --ae->ptxt->nLastCharOffset;

        if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpElement->nLineLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy(lpElement->wpLine, wpLineStart, lpElement->nLineLen * sizeof(wchar_t));
          lpElement->wpLine[lpElement->nLineLen]=L'\0';
          lpElement->nLineWidth=-1;
          AE_GetLineWidth(ae, lpElement);
        }
        else break;
      }
      else break;

      if (!bUpdated)
      {
        if (ae->ptxt->nLineCount > nLinesInPage)
        {
          if (lpElement->nLineBreak != AELB_EOF)
          {
            dwCurrentTime=GetTickCount();

            if (dwCurrentTime - dwStartTime > AETIME_BEFOREUPDATE)
            {
              bUpdated=TRUE;
              nLineBreak=lpElement->nLineBreak;
              lpElement->nLineBreak=AELB_EOF;
              --ae->ptxt->nLastCharOffset;

              ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
              AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
              ae->ciCaretIndex=ciCaretChar;
              ae->ciSelStartIndex=ciCaretChar;
              ae->ciSelEndIndex=ciCaretChar;

              if (!(ae->popt->dwOptions & AECO_DISABLENOSCROLL))
                AE_UpdateScrollBars(ae, SB_VERT);

              if (ae->ptxt->dwWordWrap)
              {
                ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

                ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
                AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
                ae->ciCaretIndex=ciCaretChar;
                ae->ciSelStartIndex=ciCaretChar;
                ae->ciSelEndIndex=ciCaretChar;
              }
              //ae->bSkipMessages=FALSE;
              InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
              UpdateWindow(ae->hWndEdit);

              //Make sure that WM_PAINT will be send after setting text
              {
                RECT rcOnePixel={0, 0, 1, 1};

                InvalidateRect(ae->hWndEdit, &rcOnePixel, TRUE);
              }
              //ae->bSkipMessages=TRUE;

              //Restore variables
              ++ae->ptxt->nLastCharOffset;
              ((AELINEDATA *)ae->ptxt->hLinesStack.last)->nLineBreak=nLineBreak;
              ae->liFirstDrawLine.nLine=0;
              ae->liFirstDrawLine.lpLine=NULL;
              ae->nFirstDrawLineOffset=0;
              ae->ciLastCallIndex.nLine=0;
              ae->ciLastCallIndex.nCharInLine=0;
              ae->ciLastCallIndex.lpLine=NULL;
              ae->nLastCallOffset=0;
              ae->ptxt->liLineUnwrapLastCall.nLine=0;
              ae->ptxt->liLineUnwrapLastCall.lpLine=NULL;
              ae->ptxt->nLineUnwrapLastCall=0;
            }
          }
        }
      }
      wpLineStart=wpLineEnd;
      ++ae->ptxt->nLineCount;
      ++ae->ptxt->nLineUnwrapCount;
    }

    //Last line
    if (!lpElement || lpElement->nLineBreak != AELB_EOF)
    {
      if (lpElement=AE_StackLineAdd(ae))
      {
        if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, sizeof(wchar_t)))
        {
          lpElement->nLineWidth=0;
          lpElement->wpLine[0]=L'\0';

          ++ae->ptxt->nLineCount;
          ++ae->ptxt->nLineUnwrapCount;
        }
        lpElement->nLineBreak=AELB_EOF;
      }
    }
    if (ae->ptxt->nLineCount)
    {
      --ae->ptxt->nLineCount;
      --ae->ptxt->nLineUnwrapCount;
    }

    //End progress
    if (ae->popt->dwEventMask & AENM_PROGRESS)
    {
      if (!bOnInitWindow && !bStopProgress)
      {
        AE_NotifyProgress(ae, AEPGS_SETTEXT, GetTickCount() - dwStartTime, dwTextLen, dwTextLen);
      }
    }
    ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
    ae->ciCaretIndex=ciCaretChar;
    ae->ciSelStartIndex=ciCaretChar;
    ae->ciSelEndIndex=ciCaretChar;

    if (!bOnInitWindow)
    {
      if (ae->ptxt->dwWordWrap)
      {
        ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

        ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
        ae->ciCaretIndex=ciCaretChar;
        ae->ciSelStartIndex=ciCaretChar;
        ae->ciSelEndIndex=ciCaretChar;
      }
      AE_UpdateScrollBars(ae, SB_VERT);
      AE_CalcLinesWidth(ae, NULL, NULL, 0);
      if (!bUpdated) InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    }

    //Set caret position
    if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

    //Release DC
    if (!hDC)
    {
      if (hFontOld) SelectObject(ae->hDC, hFontOld);
      ReleaseDC(ae->hWndEdit, ae->hDC);
      ae->hDC=NULL;
    }
    //ae->bSkipMessages=FALSE;

    if (!bOnInitWindow)
    {
      AE_StackPointReset(ae, AEPTT_SETTEXT);
      ae->dwNotifyFlags=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
      ae->dwNotifyTextChange|=AETCT_DELETEALL;
    }
  }
  //ae->bSkipMessages=FALSE;

  if (!bOnInitWindow)
  {
    AE_NotifyChanged(ae); //AETCT_SETTEXT
  }
  return dwTextLen;
}

DWORD AE_StreamIn(AKELEDIT *ae, DWORD dwFlags, AESTREAMIN *aesi)
{
  AECHARINDEX ciCaretChar;
  AELINEDATA *lpElement=NULL;
  wchar_t *wszBuf;
  wchar_t *wpLineStart;
  wchar_t *wpLineEnd;
  HDC hDC=ae->hDC;
  HFONT hFontOld=NULL;
  DWORD dwTextLen=0;
  DWORD dwTextCount=0;
  DWORD dwBlockLen=0;
  DWORD dwBlockCount=0;
  DWORD dwStartTime=GetTickCount();
  DWORD dwProgressTime=0;
  DWORD dwCurrentTime=0;
  DWORD dwBufLen=2048;
  DWORD dwBufDone;
  DWORD dwResult=0;
  int nNewLine=aesi->nNewLine;
  int nLinesInPage;
  int nApproxLinesCount=0;
  int nLineBreak;
  BOOL bUpdated=FALSE;
  BOOL bStopProgress=FALSE;

  AE_NotifyChanging(ae, AETCT_STREAMIN);
  aesi->dwError=0;

  //Set new line
  if (nNewLine < AELB_ASIS)
  {
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->popt->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->popt->nOutputNewLine;
  }

  if (wszBuf=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwBufLen + 1) * sizeof(wchar_t)))
  {
    if (dwFlags & AESF_SELECTION)
    {
      AECHARINDEX ciInsertStart;
      AECHARINDEX ciInsertEnd;
      AECHARINDEX ciWrapStart;
      AECHARINDEX ciWrapEnd;
      DWORD dwWordWrap=ae->ptxt->dwWordWrap;
      DWORD dwWrapLimit=ae->ptxt->dwWrapLimit;

      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATE);

      //When word wrap is off, AESF_SELECTION flag is slow for very long lines
      if (!dwWordWrap)
      {
        ae->ptxt->dwWordWrap=AEWW_SYMBOL|AEWW_LIMITSYMBOL;
        ae->ptxt->dwWrapLimit=dwBufLen;
        ciWrapStart=ae->ciCaretIndex;
      }

      for (;;)
      {
        dwBufDone=0;

        if ((aesi->dwError=aesi->lpCallback(aesi->dwCookie, wszBuf, dwBufLen * sizeof(wchar_t), &dwBufDone)) || !dwBufDone)
        {
          //Stop callbacking
          if (dwResult) ciWrapEnd=ciInsertEnd;
          break;
        }
        dwResult+=dwBufDone;
        AE_InsertText(ae, &ae->ciCaretIndex, wszBuf, dwBufDone / sizeof(wchar_t), nNewLine, FALSE, AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, &ciInsertStart, &ciInsertEnd);
      }

      if (!dwWordWrap)
      {
        ae->ptxt->dwWordWrap=dwWordWrap;
        ae->ptxt->dwWrapLimit=dwWrapLimit;
        if (dwResult)
        {
          AE_IndexUpdate(ae, &ciWrapStart);
          AE_UpdateWrap(ae, (AELINEINDEX *)&ciWrapStart, (AELINEINDEX *)&ciWrapEnd, ae->ptxt->dwWordWrap);
        }
      }
      AE_StackUndoGroupStop(ae);
      AE_UpdateScrollBars(ae, SB_BOTH);
      AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
      ae->nCaretHorzIndent=ae->ptCaret.x;
      if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    }
    else
    {
      //Free old and create new heap
      //ae->bSkipMessages=TRUE;
      AE_HeapCreate(ae);

      //Get DC for faster AE_GetTextExtentPoint32
      if (!ae->hDC)
      {
        if (ae->hDC=GetDC(ae->hWndEdit))
          if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(ae->hDC, ae->ptxt->hFont);
      }
      if (ae->hDC)
      {
        //Get lines in page
        nLinesInPage=(ae->rcDraw.bottom - ae->rcDraw.top) / ae->ptxt->nCharHeight;
        if (ae->popt->dwEventMask & AENM_PROGRESS)
          dwTextLen=aesi->dwTextLen;

        for (;;)
        {
          //Progressing
          if (ae->popt->dwEventMask & AENM_PROGRESS)
          {
            dwCurrentTime=GetTickCount();

            if (dwCurrentTime - dwProgressTime > AETIME_BEFOREPROGRESS)
            {
              if (bStopProgress=AE_NotifyProgress(ae, AEPGS_STREAMIN, dwCurrentTime - dwStartTime, dwTextCount, dwTextLen))
              {
                if (lpElement) lpElement->nLineBreak=LOBYTE(lpElement->nLineBreak);
                break;
              }
              dwProgressTime=GetTickCount();
            }
          }

          //Receiving text
          dwBufDone=0;

          if ((aesi->dwError=aesi->lpCallback(aesi->dwCookie, wszBuf, dwBufLen * sizeof(wchar_t), &dwBufDone)) || !dwBufDone)
          {
            //Stop callbacking
            if (lpElement) lpElement->nLineBreak=LOBYTE(lpElement->nLineBreak);
            break;
          }
          dwResult+=dwBufDone;
          dwBlockCount=0;
          dwBlockLen=dwBufDone / sizeof(wchar_t);
          wpLineStart=wszBuf;
          wpLineEnd=wszBuf;
          lpElement=NULL;

          //Parse text
          while (dwBlockCount < dwBlockLen)
          {
            if (lpElement=AE_StackLineAdd(ae))
            {
              wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwBlockLen - dwBlockCount, &lpElement->nLineLen, &lpElement->nLineBreak);
              dwBlockCount+=wpLineEnd - wpLineStart;
              ae->ptxt->nLastCharOffset+=lpElement->nLineLen + 1;

              if (lpElement->nLineBreak != AELB_EOF)
              {
                if (nNewLine != AELB_ASIS)
                  lpElement->nLineBreak=nNewLine;
                ++nApproxLinesCount;
              }
              else --ae->ptxt->nLastCharOffset;

              if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                xmemcpy(lpElement->wpLine, wpLineStart, lpElement->nLineLen * sizeof(wchar_t));
                lpElement->wpLine[lpElement->nLineLen]=L'\0';
                lpElement->nLineWidth=-1;
                AE_GetLineWidth(ae, lpElement);
              }
              else break;
            }
            else break;

            wpLineStart=wpLineEnd;
            ++ae->ptxt->nLineCount;
            ++ae->ptxt->nLineUnwrapCount;
          }

          if (!bUpdated)
          {
            if (nApproxLinesCount > nLinesInPage)
            {
              dwCurrentTime=GetTickCount();

              if (dwCurrentTime - dwStartTime > AETIME_BEFOREUPDATE)
              {
                bUpdated=TRUE;

                nLineBreak=((AELINEDATA *)ae->ptxt->hLinesStack.last)->nLineBreak;
                ((AELINEDATA *)ae->ptxt->hLinesStack.last)->nLineBreak=AELB_EOF;
                --ae->ptxt->nLastCharOffset;
                AE_JoinNewLines(ae);

                ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
                AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
                ae->ciCaretIndex=ciCaretChar;
                ae->ciSelStartIndex=ciCaretChar;
                ae->ciSelEndIndex=ciCaretChar;

                if (!(ae->popt->dwOptions & AECO_DISABLENOSCROLL))
                  AE_UpdateScrollBars(ae, SB_VERT);

                if (ae->ptxt->dwWordWrap)
                {
                  ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

                  ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
                  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
                  ae->ciCaretIndex=ciCaretChar;
                  ae->ciSelStartIndex=ciCaretChar;
                  ae->ciSelEndIndex=ciCaretChar;
                }
                //ae->bSkipMessages=FALSE;
                InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
                UpdateWindow(ae->hWndEdit);

                //Make sure that WM_PAINT will be send after setting text
                {
                  RECT rcOnePixel={0, 0, 1, 1};

                  InvalidateRect(ae->hWndEdit, &rcOnePixel, TRUE);
                }
                //ae->bSkipMessages=TRUE;

                //Restore variables
                ++ae->ptxt->nLastCharOffset;
                ((AELINEDATA *)ae->ptxt->hLinesStack.last)->nLineBreak=nLineBreak;
                ae->liFirstDrawLine.nLine=0;
                ae->liFirstDrawLine.lpLine=NULL;
                ae->nFirstDrawLineOffset=0;
                ae->ciLastCallIndex.nLine=0;
                ae->ciLastCallIndex.nCharInLine=0;
                ae->ciLastCallIndex.lpLine=NULL;
                ae->nLastCallOffset=0;
                ae->ptxt->liLineUnwrapLastCall.nLine=0;
                ae->ptxt->liLineUnwrapLastCall.lpLine=NULL;
                ae->ptxt->nLineUnwrapLastCall=0;
              }
            }
          }
          lpElement=(AELINEDATA *)ae->ptxt->hLinesStack.last;
          if (lpElement) lpElement->nLineBreak=MAKEWORD(lpElement->nLineBreak, AELB_SPLIT);
          dwTextCount+=dwBlockLen;
        }

        //Last line
        if (!lpElement || lpElement->nLineBreak != AELB_EOF)
        {
          if (lpElement=AE_StackLineAdd(ae))
          {
            if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, sizeof(wchar_t)))
            {
              lpElement->nLineWidth=0;
              lpElement->wpLine[0]=L'\0';

              ++ae->ptxt->nLineCount;
              ++ae->ptxt->nLineUnwrapCount;
            }
            lpElement->nLineBreak=AELB_EOF;
          }
        }
        if (ae->ptxt->nLineCount)
        {
          --ae->ptxt->nLineCount;
          --ae->ptxt->nLineUnwrapCount;
        }
        AE_JoinNewLines(ae);

        //End progress
        if (ae->popt->dwEventMask & AENM_PROGRESS)
        {
          if (!bStopProgress)
          {
            AE_NotifyProgress(ae, AEPGS_STREAMIN, GetTickCount() - dwStartTime, dwTextLen, dwTextLen);
          }
        }

        ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
        ae->ciCaretIndex=ciCaretChar;
        ae->ciSelStartIndex=ciCaretChar;
        ae->ciSelEndIndex=ciCaretChar;
        aesi->nFirstNewLine=ciCaretChar.lpLine->nLineBreak;

        if (ae->ptxt->dwWordWrap)
        {
          ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

          ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
          AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
          ae->ciCaretIndex=ciCaretChar;
          ae->ciSelStartIndex=ciCaretChar;
          ae->ciSelEndIndex=ciCaretChar;
        }
        AE_UpdateScrollBars(ae, SB_VERT);
        AE_CalcLinesWidth(ae, NULL, NULL, 0);
        if (!bUpdated) InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);

        //Set caret position
        if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

        //Release DC
        if (!hDC)
        {
          if (hFontOld) SelectObject(ae->hDC, hFontOld);
          ReleaseDC(ae->hWndEdit, ae->hDC);
          ae->hDC=NULL;
        }
        //ae->bSkipMessages=FALSE;

        AE_StackPointReset(ae, AEPTT_STREAMIN);
        ae->dwNotifyFlags=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
        ae->dwNotifyTextChange|=AETCT_DELETEALL;
      }
    }
    AE_HeapFree(NULL, 0, (LPVOID)wszBuf);
  }
  //ae->bSkipMessages=FALSE;

  AE_NotifyChanged(ae); //AETCT_STREAMIN

  return dwResult;
}

int AE_JoinNewLines(AKELEDIT *ae)
{
  AELINEDATA *lpNewElement;
  AELINEDATA *lpCurElement;
  AELINEDATA *lpNextElement;
  AELINEINDEX liLineStart;
  AELINEINDEX liLineEnd;
  DWORD dwCountLen=0;
  int nLineCount=0;
  int nSplitFound=0;

  liLineStart.nLine=0;
  liLineStart.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;
  liLineEnd.nLine=0;
  liLineEnd.lpLine=(AELINEDATA *)ae->ptxt->hLinesStack.first;

  while (liLineEnd.lpLine)
  {
    dwCountLen+=liLineEnd.lpLine->nLineLen;

    if (HIBYTE(liLineEnd.lpLine->nLineBreak) == AELB_SPLIT)
      ++nSplitFound;

    if (liLineEnd.lpLine->nLineBreak != MAKEWORD(AELB_EOF, AELB_SPLIT))
    {
      if (!nSplitFound)
      {
        liLineStart.nLine=liLineEnd.nLine + 1;
        liLineStart.lpLine=liLineEnd.lpLine->next;
        liLineEnd=liLineStart;
        dwCountLen=0;
        continue;
      }
      else
      {
        //Join split new line
        if (HIBYTE(liLineEnd.lpLine->nLineBreak) == AELB_SPLIT)
        {
          liLineEnd.lpLine->nLineBreak=LOBYTE(liLineEnd.lpLine->nLineBreak);

          if (!liLineEnd.lpLine->nLineLen)
          {
            if (liLineEnd.lpLine->nLineBreak == AELB_R &&
                liLineEnd.lpLine->prev && liLineEnd.lpLine->prev->nLineBreak == AELB_R &&
                liLineEnd.lpLine->next && !liLineEnd.lpLine->next->nLineLen && liLineEnd.lpLine->next->nLineBreak == AELB_N)
            {
              //  text\r
              //  \r|
              //  \n
              liLineEnd.nLine-=2;
              liLineEnd.lpLine=liLineEnd.lpLine->prev;
              liLineEnd.lpLine->nLineBreak=AELB_RRN;
              nLineCount-=2;
              ae->ptxt->nLastCharOffset-=2;
              AE_StackLineDelete(ae, liLineEnd.lpLine->next->next);
              AE_StackLineDelete(ae, liLineEnd.lpLine->next);
            }
            else if (liLineEnd.lpLine->prev && liLineEnd.lpLine->prev->nLineBreak == MAKEWORD(AELB_EOF, AELB_SPLIT))
            {
              //  text\0|
              //  \r or \n or \r\n or \r\r\n
              liLineEnd.nLine-=1;
              liLineEnd.lpLine=liLineEnd.lpLine->prev;
              liLineEnd.lpLine->nLineBreak=liLineEnd.lpLine->next->nLineBreak;
              nLineCount-=1;
              AE_StackLineDelete(ae, liLineEnd.lpLine->next);
            }
          }
          if (liLineEnd.lpLine->nLineBreak == AELB_R)
          {
            if (liLineEnd.lpLine->next && !liLineEnd.lpLine->next->nLineLen)
            {
              if (liLineEnd.lpLine->next->nLineBreak == AELB_RN)
              {
                //  text\r|
                //  \r\n
                liLineEnd.lpLine->nLineBreak=AELB_RRN;
                nLineCount-=1;
                ae->ptxt->nLastCharOffset-=1;
                AE_StackLineDelete(ae, liLineEnd.lpLine->next);
              }
              else if (liLineEnd.lpLine->next->nLineBreak == AELB_N)
              {
                //  text\r|
                //  \n
                liLineEnd.lpLine->nLineBreak=AELB_RN;
                nLineCount-=1;
                ae->ptxt->nLastCharOffset-=1;
                AE_StackLineDelete(ae, liLineEnd.lpLine->next);
              }
            }
          }
          if (nSplitFound == 1)
          {
            liLineEnd.nLine+=1;
            liLineEnd.lpLine=liLineEnd.lpLine->next;
            liLineStart=liLineEnd;
            dwCountLen=0;
            continue;
          }
        }

        //Join split parts
        if (lpNewElement=AE_StackLineInsertBefore(ae, liLineStart.lpLine))
        {
          lpNewElement->nLineWidth=-1;
          lpNewElement->nLineBreak=liLineEnd.lpLine->nLineBreak;
          lpNewElement->nLineLen=dwCountLen;

          if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
          {
            lpCurElement=liLineStart.lpLine;
            dwCountLen=0;

            while (lpCurElement)
            {
              xmemcpy(lpNewElement->wpLine + dwCountLen, lpCurElement->wpLine, lpCurElement->nLineLen * sizeof(wchar_t));
              dwCountLen+=lpCurElement->nLineLen;

              if (HIBYTE(lpCurElement->nLineBreak) != AELB_SPLIT)
              {
                AE_StackLineDelete(ae, lpCurElement);
                break;
              }
              lpNextElement=lpCurElement->next;
              AE_StackLineDelete(ae, lpCurElement);
              lpCurElement=lpNextElement;
            }
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
            AE_GetLineWidth(ae, lpNewElement);
          }
        }
        nLineCount+=liLineStart.nLine - liLineEnd.nLine;
        nSplitFound=0;
      }
      liLineStart.nLine+=1;
      liLineStart.lpLine=lpNewElement->next;
      liLineEnd=liLineStart;
      dwCountLen=0;
      continue;
    }
    ++liLineEnd.nLine;
    liLineEnd.lpLine=liLineEnd.lpLine->next;
  }
  ae->ptxt->nLineCount+=nLineCount;
  ae->ptxt->nLineUnwrapCount+=nLineCount;

  return nLineCount;
}

DWORD AE_StreamOut(AKELEDIT *ae, DWORD dwFlags, AESTREAMOUT *aeso)
{
  AECHARINDEX ciCount;
  AECHARINDEX ciEnd;
  wchar_t *wszBuf;
  int nNewLine=aeso->nNewLine;
  int nLineBreak;
  BOOL bColumnSel=aeso->bColumnSel;
  DWORD dwBufLen=2048;
  DWORD dwBufCount=0;
  DWORD dwResult=0;

  if (bColumnSel == -1) bColumnSel=ae->bColumnSel;
  aeso->dwError=0;

  //Set new line
  if (nNewLine < AELB_ASIS)
  {
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->popt->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->popt->nOutputNewLine;
  }

  if (dwFlags & AESF_SELECTION)
  {
    AE_GetIndex(ae, AEGI_FIRSTSELCHAR, NULL, &ciCount, FALSE);
    AE_GetIndex(ae, AEGI_LASTSELCHAR, NULL, &ciEnd, FALSE);
  }
  else
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCount, FALSE);
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciEnd, FALSE);
  }

  if (wszBuf=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwBufLen + 1) * sizeof(wchar_t)))
  {
    while (ciCount.lpLine)
    {
      if (bColumnSel)
      {
        ciCount.nCharInLine=ciCount.lpLine->nSelStart;

        while (ciCount.nCharInLine < ciCount.lpLine->nLineLen && ciCount.nCharInLine < ciCount.lpLine->nSelEnd)
        {
          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=ciCount.lpLine->wpLine[ciCount.nCharInLine++];
        }
        if (dwFlags & AESF_FILLSPACES)
        {
          while (ciCount.nCharInLine < ciCount.lpLine->nSelEnd)
          {
            if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
            wszBuf[dwBufCount++]=L' ';
            ++ciCount.nCharInLine;
          }
        }
      }
      else
      {
        while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
        {
          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=ciCount.lpLine->wpLine[ciCount.nCharInLine++];
        }
      }
      if (ciCount.lpLine->nLineBreak == AELB_EOF) break;

      if (nNewLine == AELB_ASIS)
        nLineBreak=ciCount.lpLine->nLineBreak;
      else
        nLineBreak=nNewLine;

      if (ciCount.lpLine->nLineBreak != AELB_WRAP)
      {
        if (nLineBreak == AELB_R)
        {
          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\r';
        }
        else if (nLineBreak == AELB_N)
        {
          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\n';
        }
        else if (nLineBreak == AELB_RN)
        {
          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\r';

          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\n';
        }
        else if (nLineBreak == AELB_RRN)
        {
          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\r';

          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\r';

          if (!AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=L'\n';
        }
      }
      AE_NextLine(&ciCount);
    }
    AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult);

    End:
    AE_HeapFree(NULL, 0, (LPVOID)wszBuf);
  }

  return dwResult;
}

BOOL AE_StreamOutHelper(AESTREAMOUT *aeso, const AECHARINDEX *ciCount, const AECHARINDEX *ciEnd, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufCount, DWORD *dwResult)
{
  DWORD dwBufDone=0;

  if (AE_IndexCompare(ciCount, ciEnd) >= 0)
  {
    if (*dwBufCount > 0)
    {
      wszBuf[*dwBufCount]=L'\0';
      if (aeso->dwError=aeso->lpCallback(aeso->dwCookie, wszBuf, *dwBufCount * sizeof(wchar_t), &dwBufDone)) return FALSE;
      *dwResult+=dwBufDone;
      *dwBufCount=0;
    }
    return FALSE;
  }
  if (*dwBufCount >= dwBufLen)
  {
    wszBuf[dwBufLen]=L'\0';
    *dwBufCount=0;
    if (aeso->dwError=aeso->lpCallback(aeso->dwCookie, wszBuf, dwBufLen * sizeof(wchar_t), &dwBufDone)) return FALSE;
    if (!dwBufDone) return FALSE;
    *dwResult+=dwBufDone;
  }
  return TRUE;
}

void AE_AppendTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, DWORD dwTextLen, int nNewLine)
{
  wchar_t *wszText;
  DWORD dwUnicodeLen;

  if (dwUnicodeLen=MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, NULL, 0))
  {
    if (dwTextLen == (DWORD)-1) dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeLen + 1);
      AE_AppendText(ae, wszText, dwUnicodeLen, nNewLine);

      AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
  }
}

void AE_AppendText(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int nNewLine)
{
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciSelStartIndex;
  AECHARINDEX ciSelEndIndex;
  AECHARINDEX ciLastChar;
  int nSelStartCharOffset;
  int nSelEndCharOffset;

  AE_NotifyChanging(ae, AETCT_APPENDTEXT);
  AE_StackUndoGroupStop(ae);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciLastChar, FALSE);

  //Save selection points
  ciCaretIndex=ae->ciCaretIndex;
  ciSelStartIndex=ae->ciSelStartIndex;
  ciSelEndIndex=ae->ciSelEndIndex;
  nSelStartCharOffset=ae->nSelStartCharOffset;
  nSelEndCharOffset=ae->nSelEndCharOffset;

  if (AE_InsertText(ae, &ciLastChar, wpText, dwTextLen, nNewLine, FALSE, AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, NULL, NULL))
  {
    //Restore selection points
    AE_IndexUpdate(ae, &ciSelStartIndex);
    ae->nSelStartCharOffset=nSelStartCharOffset;
    ae->ciSelStartIndex=ciSelStartIndex;
    AE_IndexUpdate(ae, &ciSelEndIndex);
    ae->nSelEndCharOffset=nSelEndCharOffset;
    ae->ciSelEndIndex=ciSelEndIndex;
    AE_IndexUpdate(ae, &ciCaretIndex);
    AE_GetPosFromChar(ae, &ciCaretIndex, &ae->ptCaret, NULL);
    ae->ciCaretIndex=ciCaretIndex;

    AE_UpdateScrollBars(ae, SB_BOTH);
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    AE_StackUndoGroupStop(ae);
  }
  AE_NotifyChanged(ae); //AETCT_APPENDTEXT
}

void AE_ReplaceSelAnsi(AKELEDIT *ae, int nCodePage, const char *pText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  wchar_t *wszText;
  DWORD dwUnicodeLen;

  if (dwUnicodeLen=MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, NULL, 0))
  {
    if (dwTextLen == (DWORD)-1) dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeLen + 1);
      AE_ReplaceSel(ae, wszText, dwUnicodeLen, nNewLine, bColumnSel, ciInsertStart, ciInsertEnd);

      AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
  }
}

void AE_ReplaceSel(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AECHARINDEX ciStart={0};
  AECHARINDEX ciEnd={0};

  if (bColumnSel == -1) bColumnSel=ae->bColumnSel;

  AE_NotifyChanging(ae, AETCT_REPLACESEL);
  AE_StackUndoGroupStop(ae);
  AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
  if (!AE_InsertText(ae, &ae->ciCaretIndex, wpText, dwTextLen, nNewLine, bColumnSel, 0, &ciStart, &ciEnd))
  {
    //Set caret position
    AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
  }
  AE_StackUndoGroupStop(ae);

  if (bColumnSel)
  {
    if (ae->popt->dwOptions & AECO_PASTESELECTCOLUMN)
      AE_SetSelectionPos(ae, &ciStart, &ciEnd, bColumnSel, AESELT_LOCKNOTIFY, 0);
  }
  if (ciInsertStart) *ciInsertStart=ciStart;
  if (ciInsertEnd) *ciInsertEnd=ciEnd;
  AE_NotifyChanged(ae); //AETCT_REPLACESEL
}

DWORD AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, DWORD dwDeleteFlags)
{
  AECHARINDEX ciDeleteStart=*ciRangeStart;
  AECHARINDEX ciDeleteEnd=*ciRangeEnd;
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  AECHARINDEX ciTmp;
  AELINEDATA *lpFirstElement=NULL;
  AELINEDATA *lpNewElement=NULL;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpNextElement=NULL;
  AEPOINT *lpPoint;
  AEPOINT *lpBreak;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  AEUNDOITEM *lpSetSelUndo=NULL;
  DWORD dwCalcLinesWidthFlags=0;
  int nRichTextCount=0;
  int nLastLineSelStart=0;
  int nLineCount=0;
  int nLineUnwrapCount=0;
  int nWrapCount=0;
  int nHScrollPos;
  int nVScrollPos;
  int nStartOffset;
  int nEndOffset;
  int nDelLength;
  int nExtraStartOffset;
  int nExtraEndOffset;
  int nLineOffsetNew;
  int nLineOffsetOld;
  int nLineDelStartOffsetOld;
  int nLineDelEndOffsetOld;
  int nLineDelLength;
  int nFirstRedrawLine;
  int nLastRedrawLine;
  int nElementLine;
  int nCompare;

  if (ciRangeStart->lpLine && ciRangeEnd->lpLine)
  {
    //Exchange indexes
    if ((nCompare=AE_IndexCompare(&ciDeleteStart, &ciDeleteEnd)) > 0)
    {
      ciTmp=ciDeleteStart;
      ciDeleteStart=ciDeleteEnd;
      ciDeleteEnd=ciTmp;
    }
    else if (nCompare == 0) return 0;

    //Force left-top to right-bottom selection
    if (bColumnSel)
    {
      if (ciDeleteStart.lpLine->nSelStart != ciDeleteStart.lpLine->nSelEnd)
        ciDeleteStart.nCharInLine=min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nSelStart);
      if (ciDeleteEnd.lpLine->nSelStart != ciDeleteEnd.lpLine->nSelEnd)
        ciDeleteEnd.nCharInLine=max(ciDeleteEnd.nCharInLine, ciDeleteEnd.lpLine->nSelEnd);
    }

    //Set AE_CalcLinesWidth flags
    if (dwDeleteFlags & AEDELT_LOCKUPDATE)
      dwCalcLinesWidthFlags=AECLW_LOCKUPDATE;

    nStartOffset=AE_AkelIndexToRichOffset(ae, &ciDeleteStart);
    nEndOffset=AE_AkelIndexToRichOffset(ae, &ciDeleteEnd);
    nDelLength=nEndOffset - nStartOffset;
    nLineOffsetNew=nStartOffset - min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nLineLen);
    nLineOffsetOld=nLineOffsetNew;
    nExtraStartOffset=max(ciDeleteStart.nCharInLine - ciDeleteStart.lpLine->nLineLen, 0);
    nExtraEndOffset=max(ciDeleteEnd.nCharInLine - ciDeleteEnd.lpLine->nLineLen, 0);

    //Send AEN_TEXTDELETEBEGIN
    if (ae->popt->dwEventMask & AENM_TEXTDELETE)
    {
      AENTEXTDELETE td;

      td.hdr.hwndFrom=ae->hWndEdit;
      td.hdr.idFrom=ae->nEditCtrlID;
      td.hdr.code=AEN_TEXTDELETEBEGIN;
      td.hdr.docFrom=(AEHDOC)ae;
      td.dwType=ae->dwNotifyTextChange;
      td.bColumnSel=bColumnSel;
      td.dwDeleteFlags=dwDeleteFlags;
      td.crAkelRange.ciMin=*ciRangeStart;
      td.crAkelRange.ciMax=*ciRangeEnd;
      td.crRichRange.cpMin=nStartOffset;
      td.crRichRange.cpMax=nEndOffset;
      AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&td);
    }

    if (bColumnSel)
    {
      //Note:
      //If bColumnSel == TRUE, then text must be selected first before AE_DeleteTextRange call.

      //Lines after deletion
      lpElement=ciDeleteStart.lpLine;
      nElementLine=ciDeleteStart.nLine;
      lpPoint=(AEPOINT *)ae->ptxt->hPointsStack.first;

      while (lpElement)
      {
        nLineDelStartOffsetOld=nLineOffsetOld + min(lpElement->nSelStart, lpElement->nLineLen);
        nLineDelEndOffsetOld=nLineOffsetOld + min(lpElement->nSelEnd, lpElement->nLineLen);
        nLineDelLength=nLineDelEndOffsetOld - nLineDelStartOffsetOld;

        if (lpNewElement=AE_StackLineInsertBefore(ae, ciDeleteStart.lpLine))
        {
          if (!lpFirstElement)
            lpFirstElement=lpNewElement;

          lpNewElement->nLineWidth=-1;
          lpNewElement->nLineBreak=lpElement->nLineBreak;
          lpNewElement->nLineLen=lpElement->nLineLen - nLineDelLength;
          nRichTextCount+=nLineDelLength;

          if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
          {
            xmemcpy(lpNewElement->wpLine, lpElement->wpLine, min(lpElement->nSelStart, lpNewElement->nLineLen) * sizeof(wchar_t));
            if (lpElement->nSelEnd < lpElement->nLineLen)
              xmemcpy(lpNewElement->wpLine + min(lpElement->nSelStart, lpNewElement->nLineLen), lpElement->wpLine + lpElement->nSelEnd, (lpElement->nLineLen - lpElement->nSelEnd) * sizeof(wchar_t));
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

            //Update points
            for (lpBreak=NULL; lpPoint; lpPoint=lpPoint->next)
            {
              if (lpPoint->ciPoint.nLine > nElementLine && nElementLine != ciDeleteEnd.nLine)
                break;

              if (lpPoint->ciPoint.nLine == nElementLine)
              {
                lpPoint->ciPoint.lpLine=lpNewElement;

                if (lpPoint->ciPoint.nCharInLine >= lpElement->nSelStart &&
                    lpElement->nSelStart < lpElement->nLineLen)
                {
                  lpPoint->ciPoint.nCharInLine-=nLineDelLength;
                  lpPoint->ciPoint.nCharInLine=max(lpPoint->ciPoint.nCharInLine, lpElement->nSelStart);
                }
                lpPoint->dwFlags|=AEPTF_MOVED;
              }

              //Offsets
              if (lpPoint->nPointOffset >= 0)
              {
                if (lpPoint->nPointOffset + lpPoint->nPointLen > nLineDelStartOffsetOld)
                {
                  if (lpPoint->nPointOffset < nLineDelEndOffsetOld)
                  {
                    lpPoint->nPointLen-=min(lpPoint->nPointOffset + lpPoint->nPointLen, nLineDelEndOffsetOld) - max(lpPoint->nPointOffset, nLineDelStartOffsetOld);
                    if (lpPoint->nReserved == AEPTO_IGNORE)
                    {
                      lpPoint->nReserved=min(lpPoint->nPointOffset, nLineDelStartOffsetOld) - (nRichTextCount - nLineDelLength);
                      lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_DELETE;
                      AE_NotifyPoint(ae, AEPTT_DELETE, lpPoint);
                    }
                  }
                  else if (lpPoint->nReserved == AEPTO_IGNORE)
                    lpPoint->nReserved=lpPoint->nPointOffset - nRichTextCount;

                  if (!lpBreak) lpBreak=lpPoint;
                }
              }
            }
            if (lpBreak) lpPoint=lpBreak;

            //Add undo
            if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
            {
              if (!ae->ptxt->bLockCollectUndo)
              {
                if (ae->ptxt->dwUndoLimit)
                {
                  AEUNDOITEM *lpUndoElement;
                  wchar_t *wpUndoText;
                  DWORD dwUndoTextLen=nLineDelLength;

                  if (dwUndoTextLen)
                  {
                    //Set selection
                    if (!lpSetSelUndo)
                    {
                      if (!(lpSetSelUndo=AE_StackUndoItemInsert(ae)))
                        break;
                    }

                    //Add text
                    if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
                    {
                      xmemcpy(wpUndoText, lpElement->wpLine + lpElement->nSelStart, dwUndoTextLen * sizeof(wchar_t));
                      wpUndoText[dwUndoTextLen]=L'\0';

                      if (lpUndoElement=AE_StackUndoItemInsert(ae))
                      {
                        lpUndoElement->dwFlags=AEUN_INSERT|AEUN_COLUMNGROUP;
                        lpUndoElement->nActionStartOffset=nLineOffsetNew + min(lpElement->nSelStart, lpElement->nLineLen);
                        lpUndoElement->nActionEndOffset=nLineOffsetNew + min(lpElement->nSelEnd, lpElement->nLineLen);
                        lpUndoElement->wpText=wpUndoText;
                        lpUndoElement->dwTextLen=dwUndoTextLen;
                      }
                      else break;
                    }
                    else break;
                  }
                }
              }
            }
          }
          else break;
        }
        else break;

        if (lpElement == ciDeleteEnd.lpLine)
        {
          nLastLineSelStart=ciDeleteEnd.lpLine->nSelStart;
          break;
        }

        if (lpNewElement->nLineBreak == AELB_WRAP)
          nLineOffsetNew+=lpNewElement->nLineLen;
        else
          nLineOffsetNew+=lpNewElement->nLineLen + 1;
        if (lpElement->nLineBreak == AELB_WRAP)
          nLineOffsetOld+=lpElement->nLineLen;
        else
          nLineOffsetOld+=lpElement->nLineLen + 1;

        //Next line
        nElementLine+=1;
        lpElement=lpElement->next;
      }
      AE_StackPointUnreserve(ae);

      //Delete lines in range
      lpElement=ciDeleteStart.lpLine;

      while (lpElement)
      {
        if (lpElement == ciDeleteEnd.lpLine)
        {
          AE_StackLineDelete(ae, lpElement);
          break;
        }

        lpNextElement=lpElement->next;
        AE_StackLineDelete(ae, lpElement);
        lpElement=lpNextElement;
      }

      ciFirstChar.nLine=ciDeleteStart.nLine;
      ciFirstChar.lpLine=lpFirstElement;
      ciFirstChar.nCharInLine=ciDeleteStart.nCharInLine;
      ciLastChar.nLine=ciDeleteEnd.nLine;
      ciLastChar.lpLine=lpNewElement;
      ciLastChar.nCharInLine=nLastLineSelStart;

      //Update control points
      if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciDeleteStart.nLine)
        ae->nFirstDrawLineOffset-=nRichTextCount;
      if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciDeleteStart.nLine)
        ae->nLastCallOffset-=nRichTextCount;
      ae->ptxt->nLastCharOffset-=nRichTextCount;

      //Set global
      nHScrollPos=ae->nHScrollPos;
      nVScrollPos=ae->nVScrollPos;
      AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
      ae->ciCaretIndex=ciFirstChar;
      ae->nSelStartCharOffset=nStartOffset;
      ae->ciSelStartIndex=ciFirstChar;
      ae->nSelEndCharOffset=nStartOffset;
      ae->ciSelEndIndex=ciFirstChar;

      //Post processing
      lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
      lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

      if (ae->ptxt->dwWordWrap)
      {
        AELINEINDEX liWrapStart;
        AELINEINDEX liWrapEnd;

        liWrapStart.nLine=ciFirstChar.nLine;
        liWrapStart.lpLine=ciFirstChar.lpLine;
        liWrapEnd.nLine=ciLastChar.nLine;
        liWrapEnd.lpLine=ciLastChar.lpLine;
        nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->ptxt->dwWordWrap);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;

        //Set redraw lines
        nFirstRedrawLine=liWrapStart.nLine;
        nLastRedrawLine=-1;

        //Set control points to "delete from" position
        AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
        ae->ciCaretIndex=ciFirstChar;
        ae->nSelStartCharOffset=nStartOffset;
        ae->ciSelStartIndex=ciFirstChar;
        ae->nSelEndCharOffset=nStartOffset;
        ae->ciSelEndIndex=ciFirstChar;

        //Update scroll bars
        ae->ptxt->nLineCount+=nWrapCount;
        ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
        if (nWrapCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
          {
            AE_UpdateScrollBars(ae, SB_VERT);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
        }
        if (!ae->ptxt->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
      }
      else
      {
        //Set redraw lines
        nFirstRedrawLine=ciFirstChar.nLine;
        nLastRedrawLine=ciLastChar.nLine;

        //Update scroll bars
        if (!ae->ptxt->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;
      }
      AE_StackPointDelete(ae, lpPointOne);
      AE_StackPointDelete(ae, lpPointTwo);

      //Add undo
      if (nRichTextCount > 0)
      {
        if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
        {
          if (!ae->ptxt->bLockCollectUndo)
          {
            if (ae->ptxt->dwUndoLimit)
            {
              //Set undo selection
              if (lpSetSelUndo)
              {
                lpSetSelUndo->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_EXTRAOFFSET|AEUN_UNDOONLY;
                lpSetSelUndo->nActionStartOffset=nStartOffset;
                lpSetSelUndo->nActionEndOffset=nEndOffset;
                lpSetSelUndo->nExtraStartOffset=nExtraStartOffset;
                lpSetSelUndo->nExtraEndOffset=nExtraEndOffset;
                lpSetSelUndo->wpText=NULL;
                lpSetSelUndo->dwTextLen=0;
              }

              //Set redo selection
              {
                AEUNDOITEM *lpUndoElement;

                if (lpUndoElement=AE_StackUndoItemInsert(ae))
                {
                  lpUndoElement->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_EXTRAOFFSET|AEUN_REDOONLY;
                  lpUndoElement->nActionStartOffset=nStartOffset;
                  lpUndoElement->nActionEndOffset=nStartOffset;
                  lpUndoElement->nExtraStartOffset=nExtraStartOffset;
                  lpUndoElement->nExtraEndOffset=nExtraStartOffset;
                  lpUndoElement->wpText=NULL;
                  lpUndoElement->dwTextLen=0;
                }
              }
            }
          }
        }
      }

      //Update end offset
      nEndOffset-=nRichTextCount;
    }
    else
    {
      //Add undo
      if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
      {
        if (!ae->ptxt->bLockCollectUndo)
        {
          if (ae->ptxt->dwUndoLimit)
          {
            AEUNDOITEM *lpUndoElement;
            wchar_t *wpUndoText;
            DWORD dwUndoTextLen;

            if (dwUndoTextLen=AE_GetTextRange(ae, &ciDeleteStart, &ciDeleteEnd, NULL, 0, AELB_ASIS, bColumnSel, FALSE))
            {
              if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t)))
              {
                dwUndoTextLen=AE_GetTextRange(ae, &ciDeleteStart, &ciDeleteEnd, wpUndoText, (DWORD)-1, AELB_ASIS, bColumnSel, FALSE);

                if (lpUndoElement=AE_StackUndoItemInsert(ae))
                {
                  lpUndoElement->dwFlags=AEUN_INSERT;
                  lpUndoElement->nActionStartOffset=nStartOffset;
                  lpUndoElement->nActionEndOffset=nEndOffset;
                  lpUndoElement->wpText=wpUndoText;
                  lpUndoElement->dwTextLen=dwUndoTextLen;
                }
              }
            }
          }
        }
      }

      //Normalize delete position
      ciDeleteStart.nCharInLine=min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nLineLen);
      ciDeleteEnd.nCharInLine=min(ciDeleteEnd.nCharInLine, ciDeleteEnd.lpLine->nLineLen);

      //Line after deletion
      if (lpNewElement=AE_StackLineInsertBefore(ae, ciDeleteStart.lpLine))
      {
        lpNewElement->nLineWidth=-1;
        lpNewElement->nLineBreak=ciDeleteEnd.lpLine->nLineBreak;
        lpNewElement->nLineLen=ciDeleteStart.nCharInLine + (ciDeleteEnd.lpLine->nLineLen - ciDeleteEnd.nCharInLine);
        nRichTextCount-=lpNewElement->nLineLen;
        if (lpNewElement->nLineBreak != AELB_WRAP)
          --nRichTextCount;

        if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy(lpNewElement->wpLine, ciDeleteStart.lpLine->wpLine, ciDeleteStart.nCharInLine * sizeof(wchar_t));
          xmemcpy(lpNewElement->wpLine + ciDeleteStart.nCharInLine, ciDeleteEnd.lpLine->wpLine + ciDeleteEnd.nCharInLine, (ciDeleteEnd.lpLine->nLineLen - ciDeleteEnd.nCharInLine) * sizeof(wchar_t));
          lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
        }
      }

      //Update points
      for (lpPoint=(AEPOINT *)ae->ptxt->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
      {
        //if (lpPoint->ciPoint.nLine < ciDeleteStart.nLine)
        //  break;

        if (lpPoint->ciPoint.nLine >= ciDeleteStart.nLine)
        {
          if (lpPoint->ciPoint.nLine <= ciDeleteEnd.nLine)
          {
            if (lpPoint->ciPoint.nLine > ciDeleteStart.nLine || lpPoint->ciPoint.nCharInLine >= ciDeleteStart.nCharInLine)
            {
              if (lpPoint->ciPoint.nLine == ciDeleteEnd.nLine && lpPoint->ciPoint.nCharInLine >= ciDeleteEnd.nCharInLine)
                lpPoint->ciPoint.nCharInLine=ciDeleteStart.nCharInLine + (lpPoint->ciPoint.nCharInLine - ciDeleteEnd.nCharInLine);
              else
                lpPoint->ciPoint.nCharInLine=ciDeleteStart.nCharInLine;
            }
            lpPoint->ciPoint.nLine=ciDeleteStart.nLine;
            lpPoint->ciPoint.lpLine=lpNewElement;
          }
          else
          {
            lpPoint->ciPoint.nLine-=(ciDeleteEnd.nLine - ciDeleteStart.nLine);
          }
          lpPoint->dwFlags|=AEPTF_MOVED;
        }

        //Offsets
        if (lpPoint->nPointOffset >= 0)
        {
          if (lpPoint->nPointOffset + lpPoint->nPointLen > nStartOffset)
          {
            if (lpPoint->nPointOffset < nEndOffset)
            {
              lpPoint->nPointLen-=min(lpPoint->nPointOffset + lpPoint->nPointLen, nEndOffset) - max(lpPoint->nPointOffset, nStartOffset);
              lpPoint->nPointOffset=min(lpPoint->nPointOffset, nStartOffset);
              lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_DELETE;
              AE_NotifyPoint(ae, AEPTT_DELETE, lpPoint);
            }
            else lpPoint->nPointOffset-=nDelLength;
          }
        }
      }

      //Delete lines in range
      lpElement=ciDeleteStart.lpLine;

      while (lpElement)
      {
        nRichTextCount+=lpElement->nLineLen;
        if (lpElement->nLineBreak != AELB_WRAP)
          ++nRichTextCount;

        if (lpElement == ciDeleteEnd.lpLine)
        {
          AE_StackLineDelete(ae, lpElement);
          break;
        }
        if (lpElement->nLineBreak != AELB_WRAP)
          --nLineUnwrapCount;
        --nLineCount;
        lpNextElement=lpElement->next;
        AE_StackLineDelete(ae, lpElement);
        lpElement=lpNextElement;
      }
      ciDeleteStart.lpLine=lpNewElement;
      ciFirstChar=ciDeleteStart;
      ciLastChar=ciDeleteStart;

      //Update control points
      if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciFirstChar.nLine)
      {
        ae->liFirstDrawLine.nLine+=nLineCount;
        ae->nFirstDrawLineOffset-=nRichTextCount;
      }
      if (ae->ptxt->liMaxWidthLine.lpLine && ae->ptxt->liMaxWidthLine.nLine > ciFirstChar.nLine)
      {
        ae->ptxt->liMaxWidthLine.nLine+=nLineCount;
      }
      if (ae->ptxt->liLineUnwrapLastCall.lpLine && ae->ptxt->liLineUnwrapLastCall.nLine > ciFirstChar.nLine)
      {
        ae->ptxt->liLineUnwrapLastCall.nLine+=nLineCount;
        ae->ptxt->nLineUnwrapLastCall+=nLineUnwrapCount;
      }
      if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciFirstChar.nLine)
      {
        ae->ciLastCallIndex.nLine+=nLineCount;
        ae->nLastCallOffset-=nRichTextCount;
      }
      ae->ptxt->nLastCharOffset-=nRichTextCount;

      //Set global
      nHScrollPos=ae->nHScrollPos;
      nVScrollPos=ae->nVScrollPos;
      ae->ptxt->nLineCount+=nLineCount;
      ae->ptxt->nLineUnwrapCount+=nLineUnwrapCount;
      ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
      AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
      ae->ciCaretIndex=ciFirstChar;
      ae->nSelStartCharOffset=nStartOffset;
      ae->ciSelStartIndex=ciFirstChar;
      ae->nSelEndCharOffset=nStartOffset;
      ae->ciSelEndIndex=ciFirstChar;

      //Post processing
      lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
      lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

      if (ae->ptxt->dwWordWrap)
      {
        AELINEINDEX liWrapStart;
        AELINEINDEX liWrapEnd;

        liWrapStart.nLine=ciFirstChar.nLine;
        liWrapStart.lpLine=ciFirstChar.lpLine;
        liWrapEnd.nLine=ciLastChar.nLine;
        liWrapEnd.lpLine=ciLastChar.lpLine;
        nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->ptxt->dwWordWrap);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;

        //Set redraw lines
        nFirstRedrawLine=liWrapStart.nLine;
        nLastRedrawLine=-1;

        //Set control points to "delete from" position
        AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
        ae->ciCaretIndex=ciFirstChar;
        ae->nSelStartCharOffset=nStartOffset;
        ae->ciSelStartIndex=ciFirstChar;
        ae->nSelEndCharOffset=nStartOffset;
        ae->ciSelEndIndex=ciFirstChar;

        //Update scroll bars
        ae->ptxt->nLineCount+=nWrapCount;
        ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
        if (nLineCount + nWrapCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
          {
            AE_UpdateScrollBars(ae, SB_VERT);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
        }
        if (!ae->ptxt->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
      }
      else
      {
        //Set redraw lines
        nFirstRedrawLine=ciFirstChar.nLine;
        nLastRedrawLine=-1;
        if (!nLineCount && ciFirstChar.nLine == ciLastChar.nLine)
          nLastRedrawLine=ciLastChar.nLine;

        //Update scroll bars
        if (nLineCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
          {
            AE_UpdateScrollBars(ae, SB_VERT);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
        }
        if (!ae->ptxt->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;
      }
      AE_StackPointDelete(ae, lpPointOne);
      AE_StackPointDelete(ae, lpPointTwo);

      //Update end offset
      nEndOffset=nStartOffset;
    }

    if (!(dwDeleteFlags & AEDELT_LOCKSCROLL))
    {
      //Set caret position
      AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
    }
    if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
    {
      //Set caret position
      ae->nCaretHorzIndent=ae->ptCaret.x;
      if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

      //Redraw lines
      if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }
      else
      {
        AE_RedrawLineRange(ae, nFirstRedrawLine, nLastRedrawLine, TRUE);
      }
    }

    if (nRichTextCount > 0)
    {
      ae->dwNotifyFlags|=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
      if (!ae->ptxt->nLastCharOffset)
        ae->dwNotifyTextChange|=AETCT_DELETEALL;
    }

    //Send AEN_TEXTDELETEEND
    if (ae->popt->dwEventMask & AENM_TEXTDELETE)
    {
      AENTEXTDELETE td;

      td.hdr.hwndFrom=ae->hWndEdit;
      td.hdr.idFrom=ae->nEditCtrlID;
      td.hdr.code=AEN_TEXTDELETEEND;
      td.hdr.docFrom=(AEHDOC)ae;
      td.dwType=ae->dwNotifyTextChange;
      td.bColumnSel=bColumnSel;
      td.dwDeleteFlags=dwDeleteFlags;
      td.crAkelRange.ciMin=ciFirstChar;
      td.crAkelRange.ciMax=ciLastChar;
      td.crRichRange.cpMin=nStartOffset;
      td.crRichRange.cpMax=nEndOffset;
      AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&td);
    }
  }
  return nRichTextCount;
}

DWORD AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, const wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, DWORD dwInsertFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AECHARINDEX ciInsertFrom=*ciInsertPos;
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpNewElement=NULL;
  AELINEDATA *lpNextElement=NULL;
  AELINEDATA *lpInsertToElement=NULL;
  AEPOINT *lpPoint;
  AEPOINT *lpBreak;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  POINT ptInsertFrom;
  wchar_t *wpLineStart=(wchar_t *)wpText;
  wchar_t *wpLineEnd;
  DWORD dwTextCount=0;
  DWORD dwRichTextCount=0;
  DWORD dwCalcLinesWidthFlags=0;
  int nLineLen=0;
  int nLineBreak;
  int nLineCount=0;
  int nWrapCount=0;
  int nCaretIndexInLine=0;
  int nSpaces=0;
  int nHScrollPos=0;
  int nVScrollPos=0;
  int nInsertOffset;
  int nStartOffset;
  int nEndOffset;
  int nLineOffsetNew;
  int nLineOffsetOld;
  int nLineInsertOffsetOld;
  int nFirstRedrawLine=0;
  int nLastRedrawLine=0;
  int i;

  if (ciInsertFrom.lpLine)
  {
    if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);
    if (ae->ptxt->dwTextLimit - ae->ptxt->nLastCharOffset >= 0)
    {
      if (ae->ptxt->dwTextLimit - ae->ptxt->nLastCharOffset < dwTextLen)
      {
        dwTextLen=ae->ptxt->dwTextLimit - ae->ptxt->nLastCharOffset;
        ae->dwNotifyFlags|=AENM_MAXTEXT;
      }
    }
    else dwTextLen=0;

    if (dwTextLen)
    {
      //Set new line
      if (nNewLine < AELB_ASIS)
      {
        if (nNewLine == AELB_ASINPUT)
          nNewLine=ae->popt->nInputNewLine;
        else if (nNewLine == AELB_ASOUTPUT)
          nNewLine=ae->popt->nOutputNewLine;
      }

      //Set AE_CalcLinesWidth flags
      if (dwInsertFlags & AEINST_LOCKUPDATE)
        dwCalcLinesWidthFlags=AECLW_LOCKUPDATE;

      nInsertOffset=AE_AkelIndexToRichOffset(ae, &ciInsertFrom);
      nLineOffsetNew=nInsertOffset - min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen);
      nLineOffsetOld=nLineOffsetNew;
      nStartOffset=nLineOffsetNew + ciInsertFrom.nCharInLine;
      nEndOffset=nStartOffset;

      //Send AEN_TEXTINSERTBEGIN
      if (ae->popt->dwEventMask & AENM_TEXTINSERT)
      {
        AENTEXTINSERT ti;

        ti.hdr.hwndFrom=ae->hWndEdit;
        ti.hdr.idFrom=ae->nEditCtrlID;
        ti.hdr.code=AEN_TEXTINSERTBEGIN;
        ti.hdr.docFrom=(AEHDOC)ae;
        ti.dwType=ae->dwNotifyTextChange;
        ti.wpText=wpText;
        ti.dwTextLen=dwTextLen;
        ti.nNewLine=nNewLine;
        ti.bColumnSel=bColumnSel;
        ti.dwInsertFlags=dwInsertFlags;
        ti.crAkelRange.ciMin=*ciInsertPos;
        ti.crAkelRange.ciMax=*ciInsertPos;
        ti.crRichRange.cpMin=nInsertOffset;
        ti.crRichRange.cpMax=nInsertOffset;
        AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&ti);
      }

      if (bColumnSel)
      {
        AE_GetPosFromChar(ae, &ciInsertFrom, &ptInsertFrom, NULL);
        ciFirstChar.nLine=ciInsertFrom.nLine;
        ciFirstChar.lpLine=NULL;
        ciFirstChar.nCharInLine=ciInsertFrom.nCharInLine;
        ciLastChar.nLine=ciInsertFrom.nLine;
        ciLastChar.lpLine=NULL;
        ciLastChar.nCharInLine=ciInsertFrom.nCharInLine;
        lpElement=ciInsertFrom.lpLine;
        lpPoint=(AEPOINT *)ae->ptxt->hPointsStack.first;

        while (dwTextCount <= dwTextLen)
        {
          if (lpNewElement=AE_StackLineInsertBefore(ae, ciInsertFrom.lpLine))
          {
            wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &nLineLen, &nLineBreak);
            dwTextCount+=wpLineEnd - wpLineStart;
            dwRichTextCount+=nLineLen;

            if (nLineBreak != AELB_EOF)
            {
              if (nNewLine != AELB_ASIS)
                nLineBreak=nNewLine;
            }

            if (lpElement)
            {
              //First insert line
              if (!ciFirstChar.lpLine)
                ciFirstChar.lpLine=lpNewElement;
              else
                AE_GetCharInLine(ae, lpElement, ptInsertFrom.x, AECIL_HALFFIT|AECIL_ALLPOS, &ciInsertFrom.nCharInLine, NULL, bColumnSel);
              nLineInsertOffsetOld=nLineOffsetOld + min(ciInsertFrom.nCharInLine, lpElement->nLineLen);

              lpNewElement->nLineWidth=-1;
              if (lpElement->nLineBreak != AELB_EOF)
                lpNewElement->nLineBreak=lpElement->nLineBreak;
              else
                lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=lpElement->nLineLen + max(ciInsertFrom.nCharInLine - lpElement->nLineLen, 0) + nLineLen;
              nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                xmemcpy(lpNewElement->wpLine, lpElement->wpLine, min(ciInsertFrom.nCharInLine, lpElement->nLineLen) * sizeof(wchar_t));

                if (ciInsertFrom.nCharInLine > lpElement->nLineLen)
                {
                  dwRichTextCount+=ciInsertFrom.nCharInLine - lpElement->nLineLen;

                  for (i=lpElement->nLineLen; i < ciInsertFrom.nCharInLine; ++i)
                    lpNewElement->wpLine[i]=L' ';
                  xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                }
                else
                {
                  xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine + nLineLen, lpElement->wpLine + ciInsertFrom.nCharInLine, (lpElement->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                }

                //Update points
                for (lpBreak=NULL; lpPoint; lpPoint=lpPoint->next)
                {
                  if (lpPoint->ciPoint.nLine > ciLastChar.nLine && nLineBreak != AELB_EOF)
                    break;

                  if (lpPoint->ciPoint.nLine == ciLastChar.nLine)
                  {
                    lpPoint->ciPoint.lpLine=lpNewElement;
                    if (lpPoint->ciPoint.nCharInLine >= ciInsertFrom.nCharInLine)
                      lpPoint->ciPoint.nCharInLine+=nLineLen;
                    lpPoint->dwFlags|=AEPTF_MOVED;
                  }

                  //Offsets
                  if (lpPoint->nPointOffset >= 0)
                  {
                    if (lpPoint->nPointOffset + lpPoint->nPointLen > nLineInsertOffsetOld)
                    {
                      if (lpPoint->nPointOffset < nLineInsertOffsetOld)
                      {
                        lpPoint->nPointLen+=nLineLen;
                        lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_INSERT;
                        AE_NotifyPoint(ae, AEPTT_INSERT, lpPoint);
                      }
                      else if (lpPoint->nReserved == AEPTO_IGNORE)
                        lpPoint->nReserved=lpPoint->nPointOffset + dwRichTextCount;

                      if (!lpBreak) lpBreak=lpPoint;
                    }
                  }
                }
                if (lpBreak) lpPoint=lpBreak;

                //Add undo
                if (!(dwInsertFlags & AEINST_LOCKUNDO))
                {
                  if (!ae->ptxt->bLockCollectUndo)
                  {
                    if (ae->ptxt->dwUndoLimit)
                    {
                      AEUNDOITEM *lpUndoElement;
                      wchar_t *wpUndoText;
                      DWORD dwUndoTextLen=max(ciInsertFrom.nCharInLine - lpElement->nLineLen, 0) + nLineLen;

                      if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
                      {
                        nSpaces=ciInsertFrom.nCharInLine - lpElement->nLineLen;

                        for (i=0; i < nSpaces; ++i)
                          wpUndoText[i]=L' ';
                        xmemcpy(wpUndoText + i, wpLineStart, nLineLen * sizeof(wchar_t));
                        wpUndoText[dwUndoTextLen]=L'\0';

                        if (lpUndoElement=AE_StackUndoItemInsert(ae))
                        {
                          lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                          lpUndoElement->nActionStartOffset=nLineOffsetNew + min(ciInsertFrom.nCharInLine, lpElement->nLineLen);
                          lpUndoElement->nActionEndOffset=nLineOffsetNew + ciInsertFrom.nCharInLine + nLineLen;
                          lpUndoElement->wpText=wpUndoText;
                          lpUndoElement->dwTextLen=dwUndoTextLen;
                        }
                        else break;
                      }
                      else break;
                    }
                  }
                }
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
              }
              else break;

              lpInsertToElement=lpElement;

              if (!lpElement->next)
              {
                if (nLineBreak != AELB_EOF)
                {
                  ++dwRichTextCount;

                  //Add undo (insert new line)
                  if (!(dwInsertFlags & AEINST_LOCKUNDO))
                  {
                    if (!ae->ptxt->bLockCollectUndo)
                    {
                      if (ae->ptxt->dwUndoLimit)
                      {
                        AEUNDOITEM *lpUndoElement;
                        wchar_t *wpUndoText;
                        wchar_t *wpLineBreak;
                        DWORD dwUndoTextLen=AE_GetNewLineString(ae, lpNewElement->nLineBreak, &wpLineBreak);

                        if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
                        {
                          xmemcpy(wpUndoText, wpLineBreak, dwUndoTextLen * sizeof(wchar_t));
                          wpUndoText[dwUndoTextLen]=L'\0';

                          if (lpUndoElement=AE_StackUndoItemInsert(ae))
                          {
                            lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                            lpUndoElement->nActionStartOffset=nLineOffsetNew + lpNewElement->nLineLen;
                            lpUndoElement->nActionEndOffset=nLineOffsetNew + lpNewElement->nLineLen + 1;
                            lpUndoElement->wpText=wpUndoText;
                            lpUndoElement->dwTextLen=dwUndoTextLen;
                          }
                          else break;
                        }
                        else break;
                      }
                    }
                  }
                }
              }
              if (lpElement->nLineBreak == AELB_WRAP)
                nLineOffsetOld+=lpElement->nLineLen;
              else
                nLineOffsetOld+=lpElement->nLineLen + 1;

              lpElement=lpElement->next;
            }
            else
            {
              ciInsertFrom.nCharInLine=ptInsertFrom.x / ae->ptxt->nSpaceCharWidth;

              lpNewElement->nLineWidth=-1;
              lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=ciInsertFrom.nCharInLine + nLineLen;
              nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                dwRichTextCount+=ciInsertFrom.nCharInLine;
                if (nLineBreak != AELB_EOF)
                  ++dwRichTextCount;

                for (i=0; i < ciInsertFrom.nCharInLine; ++i)
                  lpNewElement->wpLine[i]=L' ';
                xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));

                //Add undo
                if (!(dwInsertFlags & AEINST_LOCKUNDO))
                {
                  if (!ae->ptxt->bLockCollectUndo)
                  {
                    if (ae->ptxt->dwUndoLimit)
                    {
                      AEUNDOITEM *lpUndoElement;
                      wchar_t *wpUndoText;
                      DWORD dwUndoTextLen=lpNewElement->nLineLen + 1;

                      if (nLineBreak == AELB_EOF)
                        --dwUndoTextLen;

                      if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
                      {
                        nSpaces=ciInsertFrom.nCharInLine;

                        for (i=0; i < nSpaces; ++i)
                          wpUndoText[i]=L' ';
                        xmemcpy(wpUndoText + i, wpLineStart, nLineLen * sizeof(wchar_t));
                        if (nLineBreak != AELB_EOF)
                          wpUndoText[lpNewElement->nLineLen]=L'\n';
                        wpUndoText[dwUndoTextLen]=L'\0';

                        if (lpUndoElement=AE_StackUndoItemInsert(ae))
                        {
                          lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                          lpUndoElement->nActionStartOffset=nLineOffsetNew;
                          if (nLineBreak != AELB_EOF)
                            lpUndoElement->nActionEndOffset=nLineOffsetNew + lpNewElement->nLineLen + 1;
                          else
                            lpUndoElement->nActionEndOffset=nLineOffsetNew + lpNewElement->nLineLen;
                          lpUndoElement->wpText=wpUndoText;
                          lpUndoElement->dwTextLen=dwUndoTextLen;
                        }
                        else break;
                      }
                      else break;
                    }
                  }
                }
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

                ++nLineCount;
              }
              else break;
            }
            if (nLineBreak == AELB_EOF) break;

            if (lpNewElement->nLineBreak == AELB_WRAP)
              nLineOffsetNew+=lpNewElement->nLineLen;
            else
              nLineOffsetNew+=lpNewElement->nLineLen + 1;
            ++ciLastChar.nLine;
            wpLineStart=wpLineEnd;
          }
          else break;
        }
        AE_StackPointUnreserve(ae);

        if (dwTextCount)
        {
          //Last insert line
          ciLastChar.lpLine=lpNewElement;
          ciLastChar.nCharInLine=nCaretIndexInLine;

          //Delete lines in range
          lpElement=ciInsertFrom.lpLine;

          while (lpElement)
          {
            if (lpElement == lpInsertToElement)
            {
              AE_StackLineDelete(ae, lpElement);
              break;
            }

            lpNextElement=lpElement->next;
            AE_StackLineDelete(ae, lpElement);
            lpElement=lpNextElement;
          }

          //Update control points
          if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciInsertFrom.nLine)
            ae->nFirstDrawLineOffset+=dwRichTextCount;
          if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciInsertFrom.nLine)
            ae->nLastCallOffset+=dwRichTextCount;
          ae->ptxt->nLastCharOffset+=dwRichTextCount;

          //Set global
          nHScrollPos=ae->nHScrollPos;
          nVScrollPos=ae->nVScrollPos;
          ae->ptxt->nLineCount+=nLineCount;
          ae->ptxt->nLineUnwrapCount+=nLineCount;
          ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
          AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
          ae->ciCaretIndex=ciFirstChar;
          ae->nSelStartCharOffset=nStartOffset;
          ae->ciSelStartIndex=ciFirstChar;
          ae->nSelEndCharOffset=nStartOffset;
          ae->ciSelEndIndex=ciFirstChar;
          nEndOffset=nLineOffsetNew + ciInsertFrom.nCharInLine + nLineLen;

          //Post processing
          lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
          lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

          if (ae->ptxt->dwWordWrap)
          {
            AELINEINDEX liWrapStart;
            AELINEINDEX liWrapEnd;

            liWrapStart.nLine=ciFirstChar.nLine;
            liWrapStart.lpLine=ciFirstChar.lpLine;
            liWrapEnd.nLine=ciLastChar.nLine;
            liWrapEnd.lpLine=ciLastChar.lpLine;
            nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->ptxt->dwWordWrap);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;

            //Set redraw lines
            nFirstRedrawLine=liWrapStart.nLine;
            nLastRedrawLine=-1;

            //Set control points to "insert from" position
            AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciFirstChar;
            ae->nSelStartCharOffset=nStartOffset;
            ae->ciSelStartIndex=ciFirstChar;
            ae->nSelEndCharOffset=nStartOffset;
            ae->ciSelEndIndex=ciFirstChar;

            //Update scroll bars
            ae->ptxt->nLineCount+=nWrapCount;
            ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
            if (nLineCount + nWrapCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATE))
              {
                AE_UpdateScrollBars(ae, SB_VERT);
                ciFirstChar=lpPointOne->ciPoint;
                ciLastChar=lpPointTwo->ciPoint;
              }
            }
            if (!ae->ptxt->liMaxWidthLine.lpLine)
              AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
            else
              AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
          }
          else
          {
            //Set redraw lines
            nFirstRedrawLine=ciFirstChar.nLine;
            nLastRedrawLine=ciLastChar.nLine;

            //Update scroll bars
            if (nLineCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATE))
              {
                AE_UpdateScrollBars(ae, SB_VERT);
                ciFirstChar=lpPointOne->ciPoint;
                ciLastChar=lpPointTwo->ciPoint;
              }
            }
            if (!ae->ptxt->liMaxWidthLine.lpLine)
              AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
            else
              AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
          AE_StackPointDelete(ae, lpPointOne);
          AE_StackPointDelete(ae, lpPointTwo);

          //Add undo
          if (!(dwInsertFlags & AEINST_LOCKUNDO))
          {
            if (!ae->ptxt->bLockCollectUndo)
            {
              if (ae->ptxt->dwUndoLimit)
              {
                //Set redo selection
                {
                  AEUNDOITEM *lpUndoElement;

                  if (lpUndoElement=AE_StackUndoItemInsert(ae))
                  {
                    lpUndoElement->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_REDOONLY;
                    lpUndoElement->nActionStartOffset=nStartOffset;
                    lpUndoElement->nActionEndOffset=nEndOffset;
                    lpUndoElement->wpText=NULL;
                    lpUndoElement->dwTextLen=0;
                  }
                }
              }
            }
          }
        }
      }
      else
      {
        lpNewElement=ciInsertFrom.lpLine;

        while (dwTextCount < dwTextLen)
        {
          if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
          {
            wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &nLineLen, &nLineBreak);
            dwTextCount+=wpLineEnd - wpLineStart;
            dwRichTextCount+=nLineLen + 1;

            if (nLineBreak != AELB_EOF)
            {
              if (nNewLine != AELB_ASIS)
                nLineBreak=nNewLine;
            }
            else --dwRichTextCount;

            //First line
            if (!nLineCount)
            {
              //Start index
              ciFirstChar.nLine=ciInsertFrom.nLine;
              ciFirstChar.lpLine=lpNewElement;
              ciFirstChar.nCharInLine=min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen);

              if (nLineBreak == AELB_EOF)
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                lpNewElement->nLineLen=ciInsertFrom.lpLine->nLineLen + max(ciInsertFrom.nCharInLine - ciInsertFrom.lpLine->nLineLen, 0) + nLineLen;
                nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;
              }
              else
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=nLineBreak;
                lpNewElement->nLineLen=min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen) + max(ciInsertFrom.nCharInLine - ciInsertFrom.lpLine->nLineLen, 0) + nLineLen;
                nCaretIndexInLine=0;
              }

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                xmemcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine, min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen) * sizeof(wchar_t));

                if (ciInsertFrom.nCharInLine > ciInsertFrom.lpLine->nLineLen)
                {
                  dwRichTextCount+=ciInsertFrom.nCharInLine - ciInsertFrom.lpLine->nLineLen;
                  nSpaces=ciInsertFrom.nCharInLine - ciInsertFrom.lpLine->nLineLen;

                  for (i=ciInsertFrom.lpLine->nLineLen; i < ciInsertFrom.nCharInLine; ++i)
                    lpNewElement->wpLine[i]=L' ';
                }

                if (nLineBreak == AELB_EOF)
                {
                  //wpText - one line
                  xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                    xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine + nLineLen, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                }
                else
                {
                  xmemcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

                  if (dwTextCount >= dwTextLen)
                  {
                    //End of line as new line
                    if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
                    {
                      lpNewElement->nLineWidth=-1;
                      lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                      lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine, 0);
                      nCaretIndexInLine=0;

                      if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                      {
                        if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                          xmemcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                        lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                      }
                      else break;
                    }
                    else break;
                  }
                }
              }
              else break;

              if (nLineBreak == AELB_EOF) break;
              ++nLineCount;
              wpLineStart=wpLineEnd;
              continue;
            }

            //Last line
            if (dwTextCount >= dwTextLen)
            {
              if (nLineBreak == AELB_EOF)
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine, 0) + nLineLen;
                nCaretIndexInLine=nLineLen;

                if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                {
                  xmemcpy(lpNewElement->wpLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                    xmemcpy(lpNewElement->wpLine + nLineLen, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                }
                else break;
              }
              else
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=nLineBreak;
                lpNewElement->nLineLen=nLineLen;
                nCaretIndexInLine=0;

                if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                {
                  xmemcpy(lpNewElement->wpLine, wpLineStart, lpNewElement->nLineLen * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

                  ++nLineCount;
                }
                else break;

                //End of line as new line
                if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
                {
                  lpNewElement->nLineWidth=-1;
                  lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                  lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine, 0);
                  nCaretIndexInLine=0;

                  if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                  {
                    if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                      xmemcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                    lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                  }
                  else break;
                }
                else break;
              }
              break;
            }

            //Center lines
            {
              lpNewElement->nLineWidth=-1;
              lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=nLineLen;
              nCaretIndexInLine=0;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                xmemcpy(lpNewElement->wpLine, wpLineStart, lpNewElement->nLineLen * sizeof(wchar_t));
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

                ++nLineCount;
              }
              else break;

              wpLineStart=wpLineEnd;
            }
          }
          else break;
        }

        if (dwTextCount)
        {
          //Update points
          for (lpPoint=(AEPOINT *)ae->ptxt->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
          {
            //if (lpPoint->ciPoint.nLine < ciInsertFrom.nLine)
            //  break;

            if (lpPoint->ciPoint.nLine > ciInsertFrom.nLine)
            {
              lpPoint->ciPoint.nLine+=nLineCount;
              lpPoint->dwFlags|=AEPTF_MOVED;
            }
            else if (lpPoint->ciPoint.nLine == ciInsertFrom.nLine)
            {
              if (lpPoint->ciPoint.nCharInLine < ciInsertFrom.nCharInLine)
              {
                lpPoint->ciPoint.lpLine=ciFirstChar.lpLine;
              }
              else
              {
                lpPoint->ciPoint.nLine+=nLineCount;
                lpPoint->ciPoint.lpLine=lpNewElement;
                lpPoint->ciPoint.nCharInLine=nCaretIndexInLine + (lpPoint->ciPoint.nCharInLine - ciInsertFrom.nCharInLine);
              }
              lpPoint->dwFlags|=AEPTF_MOVED;
            }

            //Offsets
            if (lpPoint->nPointOffset >= 0)
            {
              if (lpPoint->nPointOffset + lpPoint->nPointLen > nInsertOffset)
              {
                if (lpPoint->nPointOffset < nInsertOffset)
                {
                  lpPoint->nPointLen+=dwRichTextCount;
                  lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_INSERT;
                  AE_NotifyPoint(ae, AEPTT_INSERT, lpPoint);
                }
                else lpPoint->nPointOffset+=dwRichTextCount;
              }
            }
          }

          AE_StackLineDelete(ae, ciInsertFrom.lpLine);

          //Update control points
          if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciInsertFrom.nLine)
          {
            ae->liFirstDrawLine.nLine+=nLineCount;
            ae->nFirstDrawLineOffset+=dwRichTextCount;
          }
          if (ae->ptxt->liMaxWidthLine.lpLine && ae->ptxt->liMaxWidthLine.nLine > ciInsertFrom.nLine)
          {
            ae->ptxt->liMaxWidthLine.nLine+=nLineCount;
          }
          if (ae->ptxt->liLineUnwrapLastCall.lpLine && ae->ptxt->liLineUnwrapLastCall.nLine > ciInsertFrom.nLine)
          {
            ae->ptxt->liLineUnwrapLastCall.nLine+=nLineCount;
            ae->ptxt->nLineUnwrapLastCall+=nLineCount;
          }
          if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciInsertFrom.nLine)
          {
            ae->ciLastCallIndex.nLine+=nLineCount;
            ae->nLastCallOffset+=dwRichTextCount;
          }
          ae->ptxt->nLastCharOffset+=dwRichTextCount;

          //Set global
          nHScrollPos=ae->nHScrollPos;
          nVScrollPos=ae->nVScrollPos;
          ae->ptxt->nLineCount+=nLineCount;
          ae->ptxt->nLineUnwrapCount+=nLineCount;
          ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
          ciLastChar.nLine=ciInsertFrom.nLine + nLineCount;
          ciLastChar.lpLine=lpNewElement;
          ciLastChar.nCharInLine=nCaretIndexInLine;
          nEndOffset=nStartOffset - nSpaces + dwRichTextCount;

          //Post processing
          lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
          lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

          if (ae->ptxt->dwWordWrap)
          {
            AELINEINDEX liWrapStart;
            AELINEINDEX liWrapEnd;

            liWrapStart.nLine=ciFirstChar.nLine;
            liWrapStart.lpLine=ciFirstChar.lpLine;
            liWrapEnd.nLine=ciLastChar.nLine;
            liWrapEnd.lpLine=ciLastChar.lpLine;
            nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->ptxt->dwWordWrap);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;

            //Set redraw lines
            nFirstRedrawLine=liWrapStart.nLine;
            nLastRedrawLine=-1;

            //Set control points to "insert to" position
            AE_GetPosFromChar(ae, &ciLastChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciLastChar;
            ae->nSelStartCharOffset=nEndOffset;
            ae->ciSelStartIndex=ciLastChar;
            ae->nSelEndCharOffset=nEndOffset;
            ae->ciSelEndIndex=ciLastChar;

            //Update scroll bars
            ae->ptxt->nLineCount+=nWrapCount;
            ae->ptxt->nVScrollMax=AE_VPos(ae, ae->ptxt->nLineCount + 1, AEVPF_VPOSFROMLINE);
            if (nLineCount + nWrapCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATE))
              {
                AE_UpdateScrollBars(ae, SB_VERT);
                ciFirstChar=lpPointOne->ciPoint;
                ciLastChar=lpPointTwo->ciPoint;
              }
            }
            if (!ae->ptxt->liMaxWidthLine.lpLine)
              AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
            else
              AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
          }
          else
          {
            //Set redraw lines
            nFirstRedrawLine=ciFirstChar.nLine;
            nLastRedrawLine=-1;
            if (!nLineCount && ciFirstChar.nLine == ciLastChar.nLine)
              nLastRedrawLine=ciLastChar.nLine;

            AE_GetPosFromChar(ae, &ciLastChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciLastChar;
            ae->nSelStartCharOffset=nEndOffset;
            ae->ciSelStartIndex=ciLastChar;
            ae->nSelEndCharOffset=nEndOffset;
            ae->ciSelEndIndex=ciLastChar;

            //Update scroll bars
            if (nLineCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATE))
              {
                AE_UpdateScrollBars(ae, SB_VERT);
                ciFirstChar=lpPointOne->ciPoint;
                ciLastChar=lpPointTwo->ciPoint;
              }
            }
            if (!ae->ptxt->liMaxWidthLine.lpLine)
              AE_CalcLinesWidth(ae, NULL, NULL, dwCalcLinesWidthFlags);
            else
              AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, dwCalcLinesWidthFlags);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
          AE_StackPointDelete(ae, lpPointOne);
          AE_StackPointDelete(ae, lpPointTwo);

          //Add undo
          if (!(dwInsertFlags & AEINST_LOCKUNDO))
          {
            if (!ae->ptxt->bLockCollectUndo)
            {
              if (ae->ptxt->dwUndoLimit)
              {
                AEUNDOITEM *lpUndoElement;
                wchar_t *wpUndoText;

                if (nSpaces > 0)
                {
                  if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (nSpaces + 1) * sizeof(wchar_t)))
                  {
                    for (i=0; i < nSpaces; ++i)
                      wpUndoText[i]=L' ';
                    wpUndoText[nSpaces]=L'\0';

                    if (lpUndoElement=AE_StackUndoItemInsert(ae))
                    {
                      lpUndoElement->dwFlags=AEUN_DELETE;
                      lpUndoElement->nActionStartOffset=nStartOffset - nSpaces;
                      lpUndoElement->nActionEndOffset=nStartOffset;
                      lpUndoElement->wpText=wpUndoText;
                      lpUndoElement->dwTextLen=nSpaces;
                    }
                  }
                }

                if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwTextLen + 1) * sizeof(wchar_t)))
                {
                  xmemcpy(wpUndoText, wpText, dwTextLen * sizeof(wchar_t));
                  wpUndoText[dwTextLen]=L'\0';

                  if (lpUndoElement=AE_StackUndoItemInsert(ae))
                  {
                    lpUndoElement->dwFlags=AEUN_DELETE;
                    lpUndoElement->nActionStartOffset=nStartOffset;
                    lpUndoElement->nActionEndOffset=nEndOffset;
                    lpUndoElement->wpText=wpUndoText;
                    lpUndoElement->dwTextLen=dwTextLen;
                  }
                }
              }
            }
          }
        }
      }

      if (dwTextCount)
      {
        //Result indexes
        if (ciInsertStart)
          *ciInsertStart=ciFirstChar;
        if (ciInsertEnd)
          *ciInsertEnd=ciLastChar;

        if (!(dwInsertFlags & AEINST_LOCKSCROLL))
        {
          //Set caret position
          AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
        }
        if (!(dwInsertFlags & AEINST_LOCKUPDATE))
        {
          //Set caret position
          ae->nCaretHorzIndent=ae->ptCaret.x;
          if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

          //Redraw lines
          if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
          {
            InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
          }
          else
          {
            AE_RedrawLineRange(ae, nFirstRedrawLine, nLastRedrawLine, TRUE);
          }
        }

        ae->dwNotifyFlags|=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
      }

      //Send AEN_TEXTINSERTEND
      if (ae->popt->dwEventMask & AENM_TEXTINSERT)
      {
        AENTEXTINSERT ti;

        ti.hdr.hwndFrom=ae->hWndEdit;
        ti.hdr.idFrom=ae->nEditCtrlID;
        ti.hdr.code=AEN_TEXTINSERTEND;
        ti.hdr.docFrom=(AEHDOC)ae;
        ti.dwType=ae->dwNotifyTextChange;
        ti.wpText=wpText;
        ti.dwTextLen=dwTextLen;
        ti.nNewLine=nNewLine;
        ti.bColumnSel=bColumnSel;
        ti.dwInsertFlags=dwInsertFlags;
        ti.crAkelRange.ciMin=ciFirstChar;
        ti.crAkelRange.ciMax=ciLastChar;
        ti.crRichRange.cpMin=nStartOffset;
        ti.crRichRange.cpMax=nEndOffset;
        AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&ti);
      }
    }
  }
  return dwTextCount;
}

wchar_t* AE_GetNextLine(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int *nLineLen, int *nLineBreak)
{
  wchar_t *wpLineStart=(wchar_t *)wpText;
  wchar_t *wpLineEnd;
  DWORD i;

  if (dwTextLen == (DWORD)-1)
  {
    for (i=0; wpLineStart[i] && wpLineStart[i] != L'\r' && wpLineStart[i] != L'\n'; ++i);

    if (nLineLen) *nLineLen=i;
    if (nLineBreak) *nLineBreak=AELB_EOF;
    wpLineEnd=wpLineStart + i;

    if (wpLineStart[i])
    {
      if (wpLineStart[i] == L'\r' && wpLineStart[i + 1] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_RN;
        wpLineEnd=wpLineStart + i + 2;
      }
      else if (wpLineStart[i] == L'\r' && wpLineStart[i + 1] == L'\r' && wpLineStart[i + 2] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_RRN;
        wpLineEnd=wpLineStart + i + 3;
      }
      else if (wpLineStart[i] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_N;
        wpLineEnd=wpLineStart + i + 1;
      }
      else if (wpLineStart[i] == L'\r')
      {
        if (nLineBreak) *nLineBreak=AELB_R;
        wpLineEnd=wpLineStart + i + 1;
      }
    }
  }
  else
  {
    for (i=0; i < dwTextLen && wpLineStart[i] != L'\r' && wpLineStart[i] != L'\n'; ++i);

    if (nLineLen) *nLineLen=i;
    if (nLineBreak) *nLineBreak=AELB_EOF;
    wpLineEnd=wpLineStart + i;

    if (i < dwTextLen)
    {
      if (i + 1 < dwTextLen && (wpLineStart[i] == L'\r' && wpLineStart[i + 1] == L'\n'))
      {
        if (nLineBreak) *nLineBreak=AELB_RN;
        wpLineEnd=wpLineStart + i + 2;
      }
      else if (i + 2 < dwTextLen && (wpLineStart[i] == L'\r' && wpLineStart[i + 1] == L'\r' && wpLineStart[i + 2] == L'\n'))
      {
        if (nLineBreak) *nLineBreak=AELB_RRN;
        wpLineEnd=wpLineStart + i + 3;
      }
      else if (wpLineStart[i] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_N;
        wpLineEnd=wpLineStart + i + 1;
      }
      else if (wpLineStart[i] == L'\r')
      {
        if (nLineBreak) *nLineBreak=AELB_R;
        wpLineEnd=wpLineStart + i + 1;
      }
    }
  }
  return wpLineEnd;
}

int AE_GetNewLineString(AKELEDIT *ae, int nNewLine, wchar_t **wpNewLine)
{
  if (nNewLine == AELB_R)
  {
    if (wpNewLine) *wpNewLine=L"\r";
    return 1;
  }
  else if (nNewLine == AELB_N)
  {
    if (wpNewLine) *wpNewLine=L"\n";
    return 1;
  }
  else if (nNewLine == AELB_RN)
  {
    if (wpNewLine) *wpNewLine=L"\r\n";
    return 2;
  }
  else if (nNewLine == AELB_RRN)
  {
    if (wpNewLine) *wpNewLine=L"\r\r\n";
    return 3;
  }
  if (wpNewLine) *wpNewLine=L"\r\n";
  return 2;
}

BOOL AE_FindTextAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA)
{
  AEFINDTEXTW ftW={0};
  wchar_t *wszText;
  DWORD dwUnicodeLen;
  BOOL bResult=FALSE;

  if (dwUnicodeLen=MultiByteToWideChar(nCodePage, 0, ftA->pText, ftA->dwTextLen, NULL, 0))
  {
    if (ftA->dwTextLen == (DWORD)-1) dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar(nCodePage, 0, ftA->pText, ftA->dwTextLen, wszText, dwUnicodeLen + 1);
      ftW.dwFlags=ftA->dwFlags;
      ftW.pText=wszText;
      ftW.dwTextLen=dwUnicodeLen;
      ftW.nNewLine=ftA->nNewLine;
      ftW.crSearch=ftA->crSearch;
      bResult=AE_FindText(ae, &ftW);
      ftA->crFound=ftW.crFound;

      AE_HeapFree(ae, 0, (LPVOID)wszText);
    }
  }
  return bResult;
}

BOOL AE_FindText(AKELEDIT *ae, AEFINDTEXTW *ft)
{
  AECHARINDEX ciCount;
  AECHARINDEX ciCountEnd;

  if (ft->dwTextLen == (DWORD)-1)
    ft->dwTextLen=lstrlenW(ft->pText);
  if (!ft->dwTextLen)
    return FALSE;

  if (ft->dwFlags & AEFR_DOWN)
  {
    if (AE_IndexCompare(&ft->crSearch.ciMin, &ft->crSearch.ciMax) <= 0)
    {
      ciCount=ft->crSearch.ciMin;
      ciCountEnd=ft->crSearch.ciMax;
    }
    else
    {
      ciCount=ft->crSearch.ciMax;
      ciCountEnd=ft->crSearch.ciMin;
    }
  }
  else
  {
    if (AE_IndexCompare(&ft->crSearch.ciMin, &ft->crSearch.ciMax) <= 0)
    {
      ciCount=ft->crSearch.ciMax;
      ciCountEnd=ft->crSearch.ciMin;
    }
    else
    {
      ciCount=ft->crSearch.ciMin;
      ciCountEnd=ft->crSearch.ciMax;
    }
  }
  ciCount.nCharInLine=min(ciCount.nCharInLine, ciCount.lpLine->nLineLen);
  ciCountEnd.nCharInLine=min(ciCountEnd.nCharInLine, ciCountEnd.lpLine->nLineLen);

  if (ft->dwFlags & AEFR_DOWN)
  {
    if (AE_IndexOffset(ae, &ciCountEnd, &ciCountEnd, -(int)ft->dwTextLen, ft->nNewLine))
    {
      for (;;)
      {
        while (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
        {
          if (AE_IndexCompare(&ciCount, &ciCountEnd) > 0)
            return FALSE;

          if (AE_IsMatch(ae, ft, &ciCount))
            return TRUE;

          AE_IndexInc(&ciCount);
        }

        if (ciCount.lpLine->next)
          AE_NextLine(&ciCount);
        else
          return FALSE;
      }
    }
  }
  else
  {
    if (AE_IndexOffset(ae, &ciCount, &ciCount, -(int)ft->dwTextLen, ft->nNewLine))
    {
      for (;;)
      {
        while (ciCount.nCharInLine >= 0)
        {
          if (AE_IndexCompare(&ciCount, &ciCountEnd) < 0)
            return FALSE;

          if (AE_IsMatch(ae, ft, &ciCount))
            return TRUE;

          AE_IndexDec(&ciCount);
        }

        if (ciCount.lpLine->prev)
          AE_PrevLine(&ciCount);
        else
          return FALSE;
      }
    }
  }
  return FALSE;
}

DWORD AE_IsMatchAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA, const AECHARINDEX *ciChar)
{
  AEFINDTEXTW ftW={0};
  wchar_t *wszText;
  DWORD dwUnicodeLen;
  DWORD dwResult=0;

  if (dwUnicodeLen=MultiByteToWideChar(nCodePage, 0, ftA->pText, ftA->dwTextLen, NULL, 0))
  {
    if (ftA->dwTextLen == (DWORD)-1) dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar(nCodePage, 0, ftA->pText, ftA->dwTextLen, wszText, dwUnicodeLen + 1);
      ftW.dwFlags=ftA->dwFlags;
      ftW.pText=wszText;
      ftW.dwTextLen=dwUnicodeLen;
      ftW.nNewLine=ftA->nNewLine;
      ftW.crSearch=ftA->crSearch;
      dwResult=AE_IsMatch(ae, &ftW, ciChar);
      ftA->crFound=ftW.crFound;

      AE_HeapFree(ae, 0, (LPVOID)wszText);
    }
  }
  return dwResult;
}

DWORD AE_IsMatch(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount;
  int nLineBreak;
  int nNewLine;
  DWORD dwCount;

  if (ft->dwFlags & AEFR_WHOLEWORD)
  {
    if (!AE_IsDelimiter(ae, ciChar, AEDLM_WORD|AEDLM_PREVCHAR))
      return 0;
  }
  ciCount=*ciChar;
  nNewLine=ft->nNewLine;
  dwCount=0;

  //Set new line
  if (nNewLine < AELB_ASIS)
  {
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->popt->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->popt->nOutputNewLine;
  }

  for (;;)
  {
    for (; ciCount.nCharInLine < ciCount.lpLine->nLineLen; ++ciCount.nCharInLine)
    {
      if (ft->dwFlags & AEFR_MATCHCASE)
      {
        if (ciCount.lpLine->wpLine[ciCount.nCharInLine] != ft->pText[dwCount])
          return 0;
      }
      else
      {
        if (WideCharLower(ciCount.lpLine->wpLine[ciCount.nCharInLine]) != WideCharLower(ft->pText[dwCount]))
          return 0;
      }

      if (ft->dwTextLen == (DWORD)-1)
      {
        if (!ft->pText[++dwCount])
        {
          ++ciCount.nCharInLine;
          goto Founded;
        }
      }
      else if (++dwCount >= ft->dwTextLen)
      {
        ++ciCount.nCharInLine;
        goto Founded;
      }
    }

    if (ciCount.lpLine->nLineBreak != AELB_WRAP)
    {
      if (ciCount.lpLine->nLineBreak == AELB_EOF)
        return 0;
      if (nNewLine == AELB_ASIS)
        nLineBreak=ciCount.lpLine->nLineBreak;
      else
        nLineBreak=nNewLine;

      if (nLineBreak == AELB_R)
      {
        if (ft->pText[dwCount++] != L'\r') return 0;
      }
      else if (nLineBreak == AELB_N)
      {
        if (ft->pText[dwCount++] != L'\n') return 0;
      }
      else if (nLineBreak == AELB_RN)
      {
        if (ft->pText[dwCount++] != L'\r') return 0;
        if (ft->pText[dwCount++] != L'\n') return 0;
      }
      else if (nLineBreak == AELB_RRN)
      {
        if (ft->pText[dwCount++] != L'\r') return 0;
        if (ft->pText[dwCount++] != L'\n') return 0;
      }
    }

    if (ciCount.lpLine->next)
      AE_NextLine(&ciCount);
    else
      return 0;

    if (ft->dwTextLen == (DWORD)-1)
    {
      if (!ft->pText[dwCount])
        goto Founded;
    }
    else
    {
      if (dwCount >= ft->dwTextLen)
        goto Founded;
    }
  }

  Founded:
  if (ft->dwFlags & AEFR_WHOLEWORD)
  {
    if (!AE_IsDelimiter(ae, &ciCount, AEDLM_WORD))
      return 0;
  }
  ft->crFound.ciMin=*ciChar;
  ft->crFound.ciMax=ciCount;
  return dwCount;
}

void AE_UpdateCompositionPos(AKELEDIT *ae, HIMC hImc)
{
  COMPOSITIONFORM cf;
  HIMC hImcCur=hImc;

  if (hImcCur || (hImcCur=ImmGetContext(ae->hWndEdit)))
  {
    cf.dwStyle=CFS_POINT;
    AE_GlobalToClient(ae, &ae->ptCaret, &cf.ptCurrentPos);
    ImmSetCompositionWindow(hImcCur, &cf);

    if (!hImc) ImmReleaseContext(ae->hWndEdit, hImcCur);
  }
}

void AE_UpdateCandidatePos(AKELEDIT *ae, HIMC hImc)
{
  CANDIDATEFORM cf;
  HIMC hImcCur=hImc;

  if (hImcCur || (hImcCur=ImmGetContext(ae->hWndEdit)))
  {
    cf.dwIndex=0;
    cf.dwStyle=CFS_CANDIDATEPOS;
    AE_GlobalToClient(ae, &ae->ptCaret, &cf.ptCurrentPos);
    cf.ptCurrentPos.y+=ae->ptxt->nCharHeight;
    ImmSetCandidateWindow(hImcCur, &cf);

    if (!hImc) ImmReleaseContext(ae->hWndEdit, hImcCur);
  }
}

BOOL AE_GetModify(AKELEDIT *ae)
{
  if (ae->ptxt->bSavePointExist)
  {
    if (ae->ptxt->lpCurrentUndo == ae->ptxt->lpSavePoint)
      return FALSE;
  }
  return TRUE;
}

void AE_SetModify(AKELEDIT *ae, BOOL bState)
{
  if (ae->ptxt->bModified != bState)
  {
    ae->ptxt->bModified=!ae->ptxt->bModified;

    if (ae->ptxt->bModified)
    {
      ae->ptxt->lpSavePoint=NULL;
      ae->ptxt->bSavePointExist=FALSE;
    }
    else
    {
      AE_StackUndoGroupStop(ae);
      ae->ptxt->lpSavePoint=ae->ptxt->lpCurrentUndo;
      ae->ptxt->bSavePointExist=TRUE;
    }
    AE_NotifyModify(ae);
  }
}

void AE_EmptyUndoBuffer(AKELEDIT *ae)
{
  if (AE_GetModify(ae))
    ae->ptxt->bSavePointExist=FALSE;
  ae->ptxt->lpSavePoint=NULL;

  AE_StackRedoDeleteAll(ae, NULL);
  ae->ptxt->lpCurrentUndo=NULL;
}

BOOL AE_IsReadOnly(AKELEDIT *ae)
{
  if (ae->popt->dwOptions & AECO_READONLY)
  {
    if (!(ae->popt->dwOptions & AECO_DISABLEBEEP))
      MessageBeep(MB_OK);
    return TRUE;
  }
  return FALSE;
}

BOOL AE_KeyDown(AKELEDIT *ae, int nVk, BOOL bAlt, BOOL bShift, BOOL bControl)
{
  AECHARINDEX ciCharIn=ae->ciCaretIndex;
  AECHARINDEX ciCharOut=ae->ciCaretIndex;
  int nCaretHorzIndent=ae->nCaretHorzIndent;
  BOOL bSetCaretHorzIndent=TRUE;

  if (nVk == VK_TAB)
  {
    if (!bAlt && bControl)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditChar(ae, VK_TAB, TRUE);
      }
      return TRUE;
    }
  }
  if (nVk == VK_RETURN)
  {
    if (!bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditKeyReturn(ae);
      }
    }
    return TRUE;
  }
  if (nVk == VK_BACK)
  {
    if (!bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditKeyBackspace(ae, bControl);
      }
    }
    else if (!bControl && !bShift)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditUndo(ae);
      }
    }
    return TRUE;
  }
  if (nVk == VK_DELETE)
  {
    if (!bAlt)
    {
      if (!bShift)
      {
        if (!AE_IsReadOnly(ae))
        {
          AE_EditKeyDelete(ae, bControl);
        }
      }
      else if (!bControl)
      {
        if (!AE_IsReadOnly(ae))
        {
          AE_EditCut(ae);
        }
      }
    }
    return TRUE;
  }
  if (nVk == VK_INSERT)
  {
    if (!bAlt)
    {
      if (!bControl && !bShift)
      {
        ae->popt->bOverType=!ae->popt->bOverType;
        AE_UpdateCaret(ae, ae->bFocus);
      }
      else
      {
        if (bControl && !bShift)
        {
          AE_EditCopyToClipboard(ae);
        }
        else if (!bControl && bShift)
        {
          if (!AE_IsReadOnly(ae))
          {
            AE_EditPasteFromClipboard(ae, FALSE);
          }
        }
        else if (bControl && bShift)
        {
          if (!AE_IsReadOnly(ae))
          {
            AE_EditPasteFromClipboard(ae, TRUE);
          }
        }
      }
    }
    return TRUE;
  }
  if (nVk == 'X')
  {
    if (bControl && !bShift && !bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditCut(ae);
      }
    }
    return TRUE;
  }
  if (nVk == 'C')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditCopyToClipboard(ae);
    }
    return TRUE;
  }
  if (nVk == 'V')
  {
    if (bControl && !bShift && !bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditPasteFromClipboard(ae, FALSE);
      }
    }
    else if (bControl && bShift && !bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditPasteFromClipboard(ae, TRUE);
      }
    }
    return TRUE;
  }
  if (nVk == 'Z')
  {
    if (bControl && !bShift && !bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditUndo(ae);
      }
    }
    else if (bControl && bShift && !bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditRedo(ae);
      }
    }
    return TRUE;
  }
  if (nVk == 'Y')
  {
    if (bControl && !bShift && !bAlt)
    {
      if (!AE_IsReadOnly(ae))
      {
        AE_EditRedo(ae);
      }
    }
    return TRUE;
  }
  if (nVk == 'A')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditSelectAll(ae, 0, AESCT_KEYBOARD);
    }
    return TRUE;
  }

  if ((nVk == VK_HOME ||
       nVk == VK_END ||
       nVk == VK_LEFT ||
       nVk == VK_RIGHT ||
       nVk == VK_UP ||
       nVk == VK_DOWN ||
       nVk == VK_PRIOR ||
       nVk == VK_NEXT) &&
       ae->rcDraw.bottom - ae->rcDraw.top > 0 &&
       ae->rcDraw.right - ae->rcDraw.left > 0)
  {
    if (bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
      nCaretHorzIndent=ae->ptCaret.x;

    if (nVk == VK_HOME)
    {
      if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelStartIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCharOut, FALSE);
      }
      else
      {
        ciCharOut.nLine=ciCharIn.nLine;
        ciCharOut.lpLine=ciCharIn.lpLine;
        ciCharOut.nCharInLine=0;
      }
    }
    else if (nVk == VK_END)
    {
      if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelEndIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciCharOut, FALSE);
      }
      else
      {
        ciCharOut.nLine=ciCharIn.nLine;
        ciCharOut.lpLine=ciCharIn.lpLine;
        ciCharOut.nCharInLine=ciCharIn.lpLine->nLineLen;
      }
    }
    else if (nVk == VK_LEFT)
    {
      if (bControl)
      {
        if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciCharIn=ae->ciSelStartIndex;
        if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
          ciCharIn.nCharInLine=min(ciCharIn.nCharInLine, ciCharIn.lpLine->nLineLen);

        AE_GetPrevBreak(ae, &ciCharIn, &ciCharOut, bAlt, ae->popt->dwWordBreak);
      }
      else
      {
        if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        {
          ciCharIn=ae->ciSelStartIndex;
          ciCharOut=ae->ciSelStartIndex;
        }
        else AE_GetIndex(ae, AEGI_PREVVISIBLECHAR, &ciCharIn, &ciCharOut, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
      }
    }
    else if (nVk == VK_RIGHT)
    {
      if (bControl)
      {
        if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciCharIn=ae->ciSelEndIndex;
        if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
          ciCharIn.nCharInLine=min(ciCharIn.nCharInLine, ciCharIn.lpLine->nLineLen);

        AE_GetNextBreak(ae, &ciCharIn, &ciCharOut, bAlt, ae->popt->dwWordBreak);
      }
      else
      {
        if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        {
          ciCharIn=ae->ciSelEndIndex;
          ciCharOut=ae->ciSelEndIndex;
        }
        else AE_GetIndex(ae, AEGI_NEXTVISIBLECHAR, &ciCharIn, &ciCharOut, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
      }
    }
    else if (nVk == VK_UP)
    {
      if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelStartIndex;

      if (AE_GetIndex(ae, AEGI_PREVVISIBLELINE, &ciCharIn, &ciCharOut, FALSE))
      {
        if (!bControl)
        {
          AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
          bSetCaretHorzIndent=FALSE;
        }
        else
        {
          if (!AE_GetIndex(ae, AEGI_WRAPLINEBEGIN, &ciCharIn, &ciCharOut, FALSE))
          {
            AE_GetIndex(ae, AEGI_PREVVISIBLELINE, &ciCharIn, &ciCharOut, FALSE);
            AE_GetIndex(ae, AEGI_WRAPLINEBEGIN, &ciCharOut, &ciCharOut, FALSE);
          }
        }
      }
    }
    else if (nVk == VK_DOWN)
    {
      if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelEndIndex;

      if (AE_GetIndex(ae, AEGI_NEXTVISIBLELINE, &ciCharIn, &ciCharOut, FALSE))
      {
        if (!bControl)
        {
          AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
          bSetCaretHorzIndent=FALSE;
        }
        else
        {
          AE_GetIndex(ae, AEGI_WRAPLINEEND, &ciCharIn, &ciCharOut, FALSE);
          AE_GetIndex(ae, AEGI_NEXTVISIBLELINE, &ciCharOut, &ciCharOut, FALSE);
        }
      }
    }
    else if (nVk == VK_PRIOR)
    {
      int nVertPos;

      if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelStartIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_FIRSTFULLVISIBLELINE, NULL, &ciCharOut, FALSE);
        if (bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
          ciCharOut.nCharInLine=ciCharIn.nCharInLine;
        else
          ciCharOut.nCharInLine=0;
      }
      else
      {
        nVertPos=AE_VPos(ae, ciCharIn.nLine, AEVPF_VPOSFROMLINE) - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->ptxt->nCharHeight;
        ciCharOut.nLine=AE_VPos(ae, nVertPos, AEVPF_LINEFROMVPOS);
        AE_IndexUpdate(ae, &ciCharOut);
        AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

        if (ciCharIn.nLine >= AE_GetFirstVisibleLine(ae) && ciCharIn.nLine <= AE_GetLastVisibleLine(ae))
          AE_ScrollEditWindow(ae, SB_VERT, AE_VPos(ae, ciCharOut.nLine, AEVPF_VPOSFROMLINE) - (AE_VPos(ae, ciCharIn.nLine, AEVPF_VPOSFROMLINE) - ae->nVScrollPos));
        bSetCaretHorzIndent=FALSE;
      }
    }
    else if (nVk == VK_NEXT)
    {
      int nVertPos;

      if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelEndIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_LASTFULLVISIBLELINE, NULL, &ciCharOut, FALSE);
        if (bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
          ciCharOut.nCharInLine=ciCharIn.nCharInLine;
        else
          ciCharOut.nCharInLine=ciCharOut.lpLine->nLineLen;
      }
      else
      {
        nVertPos=AE_VPos(ae, ciCharIn.nLine, AEVPF_VPOSFROMLINE) + (ae->rcDraw.bottom - ae->rcDraw.top);
        ciCharOut.nLine=AE_VPos(ae, nVertPos, AEVPF_LINEFROMVPOS);
        AE_IndexUpdate(ae, &ciCharOut);
        AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

        if (ciCharIn.nLine >= AE_GetFirstVisibleLine(ae) && ciCharIn.nLine <= AE_GetLastVisibleLine(ae))
          AE_ScrollEditWindow(ae, SB_VERT, AE_VPos(ae, ciCharOut.nLine, AEVPF_VPOSFROMLINE) - (AE_VPos(ae, ciCharIn.nLine, AEVPF_VPOSFROMLINE) - ae->nVScrollPos));
        bSetCaretHorzIndent=FALSE;
      }
    }

    //Make beep
    if (!(ae->popt->dwOptions & AECO_DISABLEBEEP))
    {
      if ((bShift || !AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex)) &&
          !AE_IndexCompare(&ciCharIn, &ciCharOut))
      {
        MessageBeep(MB_OK);
      }
    }

    //Set selection
    {
      AECHARINDEX ciSelEnd={0};

      if (bShift)
      {
        if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciSelEnd=ae->ciCaretIndex;
        else if (AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
          ciSelEnd=ae->ciSelStartIndex;
        else
          ciSelEnd=ae->ciSelEndIndex;

        AE_SetSelectionPos(ae, &ciCharOut, &ciSelEnd, bAlt, AESELT_NOVERTSCROLLCORRECT, AESCT_KEYBOARD);
      }
      else AE_SetSelectionPos(ae, &ciCharOut, &ciCharOut, bAlt, AESELT_NOVERTSCROLLCORRECT, AESCT_KEYBOARD);
    }

    //Set horizontal caret
    if (!bSetCaretHorzIndent)
    {
      ae->nCaretHorzIndent=nCaretHorzIndent;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_EditCanPaste(AKELEDIT *ae)
{
  BOOL bResult=FALSE;

  if (!(ae->popt->dwOptions & AECO_READONLY))
  {
    if (IsClipboardFormatAvailable(CF_UNICODETEXT) ||
        IsClipboardFormatAvailable(CF_TEXT))
    {
      bResult=TRUE;
    }
  }
  return bResult;
}

BOOL AE_EditCanRedo(AKELEDIT *ae)
{
  if (ae->ptxt->lpCurrentUndo != (AEUNDOITEM *)ae->ptxt->hUndoStack.last)
  {
    if (!ae->ptxt->lpCurrentUndo)
    {
      if (ae->ptxt->hUndoStack.first)
        return TRUE;
      return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_EditCanUndo(AKELEDIT *ae)
{
  if (ae->ptxt->lpCurrentUndo)
    return TRUE;
  return FALSE;
}

void AE_EditUndo(AKELEDIT *ae)
{
  AEUNDOITEM *lpCurElement;
  AEUNDOITEM *lpNextElement;
  AECHARINDEX ciActionStart;
  AECHARINDEX ciActionEnd;
  AECHARINDEX ciInsertStart;
  AECHARINDEX ciInsertEnd;
  BOOL bColumnSel;

  AE_NotifyChanging(ae, AETCT_UNDO);
  AE_StackUndoGroupStop(ae);
  lpCurElement=ae->ptxt->lpCurrentUndo;

  while (lpCurElement)
  {
    lpNextElement=lpCurElement->prev;

    if (!(lpCurElement->dwFlags & AEUN_REDOONLY))
    {
      if (lpCurElement->dwFlags & AEUN_COLUMNSEL)
        bColumnSel=TRUE;
      else
        bColumnSel=FALSE;

      if (lpCurElement->dwFlags & AEUN_INSERT)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
        AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        if (AE_InsertText(ae, &ciActionStart, lpCurElement->wpText, lpCurElement->dwTextLen, AELB_ASINPUT, bColumnSel, AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, &ciInsertStart, &ciInsertEnd))
        {
          if (!lpNextElement || (lpNextElement->dwFlags & AEUN_STOPGROUP))
          {
            if (lpCurElement->dwFlags & AEUN_BACKSPACEKEY)
              AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
            else if (lpCurElement->dwFlags & AEUN_DELETEKEY)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
            else if (lpCurElement->dwFlags & AEUN_OVERTYPECHAR)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
            else
              AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
          }
        }
      }
      else if (lpCurElement->dwFlags & AEUN_DELETE)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionEndOffset, &ciActionEnd);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
        {
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
          ciActionEnd.nCharInLine+=lpCurElement->nExtraEndOffset;
        }

        if (bColumnSel)
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        else
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        AE_DeleteTextRange(ae, &ciActionStart, &ciActionEnd, bColumnSel, AEDELT_LOCKUNDO|AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATE);
      }
      else if (lpCurElement->dwFlags & AEUN_SETSEL)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionEndOffset, &ciActionEnd);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
        {
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
          ciActionEnd.nCharInLine+=lpCurElement->nExtraEndOffset;
        }

        AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
      }

      //Update window
      if (!lpNextElement || (lpNextElement->dwFlags & AEUN_STOPGROUP))
      {
        AE_UpdateScrollBars(ae, SB_BOTH);
        AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
        ae->nCaretHorzIndent=ae->ptCaret.x;
        if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }
    }
    lpCurElement=lpNextElement;

    //Stop undo
    ae->ptxt->lpCurrentUndo=lpCurElement;
    if (ae->ptxt->lpCurrentUndo)
      if (ae->ptxt->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
        break;
  }

  AE_NotifyChanged(ae); //AETCT_UNDO
}

void AE_EditRedo(AKELEDIT *ae)
{
  AEUNDOITEM *lpCurElement=ae->ptxt->lpCurrentUndo;
  AEUNDOITEM *lpNextElement;
  AECHARINDEX ciActionStart;
  AECHARINDEX ciActionEnd;
  AECHARINDEX ciInsertStart;
  AECHARINDEX ciInsertEnd;
  BOOL bColumnSel;

  AE_NotifyChanging(ae, AETCT_REDO);

  if (!lpCurElement)
    lpCurElement=(AEUNDOITEM *)ae->ptxt->hUndoStack.first;
  else
    lpCurElement=lpCurElement->next;

  while (lpCurElement)
  {
    lpNextElement=lpCurElement->next;

    if (!(lpCurElement->dwFlags & AEUN_UNDOONLY))
    {
      if (lpCurElement->dwFlags & AEUN_COLUMNSEL)
        bColumnSel=TRUE;
      else
        bColumnSel=FALSE;

      if (lpCurElement->dwFlags & AEUN_INSERT)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionEndOffset, &ciActionEnd);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
        {
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
          ciActionEnd.nCharInLine+=lpCurElement->nExtraEndOffset;
        }

        if (bColumnSel)
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        else
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        AE_DeleteTextRange(ae, &ciActionStart, &ciActionEnd, bColumnSel, AEDELT_LOCKUNDO|AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATE);
      }
      else if (lpCurElement->dwFlags & AEUN_DELETE)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
        AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        if (AE_InsertText(ae, &ciActionStart, lpCurElement->wpText, lpCurElement->dwTextLen, AELB_ASINPUT, bColumnSel, AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, &ciInsertStart, &ciInsertEnd))
        {
          //if (!lpNextElement || (lpCurElement->dwFlags & AEUN_STOPGROUP))
          //{
          //  AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
          //}
        }
      }
      else if (lpCurElement->dwFlags & AEUN_SETSEL)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionEndOffset, &ciActionEnd);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
        {
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
          ciActionEnd.nCharInLine+=lpCurElement->nExtraEndOffset;
        }

        AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
      }

      //Update window
      if (!lpNextElement || (lpCurElement->dwFlags & AEUN_STOPGROUP))
      {
        AE_UpdateScrollBars(ae, SB_BOTH);
        AE_ScrollToCaret(ae, &ae->ptCaret, TRUE);
        ae->nCaretHorzIndent=ae->ptCaret.x;
        if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }
    }

    //Stop undo
    ae->ptxt->lpCurrentUndo=lpCurElement;
    if (ae->ptxt->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
      break;

    lpCurElement=lpNextElement;
  }

  AE_NotifyChanged(ae); //AETCT_REDO
}

void AE_EditCut(AKELEDIT *ae)
{
  AE_EditCopyToClipboard(ae);

  AE_NotifyChanging(ae, AETCT_CUT);
  AE_StackUndoGroupStop(ae);
  AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
  AE_StackUndoGroupStop(ae);
  AE_NotifyChanged(ae); //AETCT_CUT
}

void AE_EditCopyToClipboard(AKELEDIT *ae)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  HGLOBAL hDataInfo=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  AECLIPBOARDINFO *pDataInfo;
  DWORD dwAnsiLen=0;
  DWORD dwUnicodeLen=0;

  if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (OpenClipboard(NULL))
    {
      EmptyClipboard();

      if (dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, AELB_ASOUTPUT, ae->bColumnSel, TRUE))
      {
        if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, dwUnicodeLen * sizeof(wchar_t)))
        {
          if (pDataTargetW=GlobalLock(hDataTargetW))
          {
            AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)pDataTargetW, (DWORD)-1, AELB_ASOUTPUT, ae->bColumnSel, TRUE);

            //Get Ansi text
            dwAnsiLen=WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, NULL, 0, NULL, NULL);

            if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, dwAnsiLen))
            {
              if (pDataTargetA=GlobalLock(hDataTargetA))
              {
                WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, (char *)pDataTargetA, dwAnsiLen, NULL, NULL);
                GlobalUnlock(hDataTargetA);
              }
            }
            GlobalUnlock(hDataTargetW);
          }
        }
      }
      if (hDataTargetW) SetClipboardData(CF_UNICODETEXT, hDataTargetW);
      if (hDataTargetA) SetClipboardData(CF_TEXT, hDataTargetA);

      //Special clipboard formats
      if (ae->bColumnSel)
      {
        SetClipboardData(cfAkelEditColumnSel, NULL);
      }
      if (hDataInfo=GlobalAlloc(GMEM_MOVEABLE, sizeof(AECLIPBOARDINFO)))
      {
        if (pDataInfo=(AECLIPBOARDINFO *)GlobalLock(hDataInfo))
        {
          pDataInfo->dwAnsiLen=dwAnsiLen?dwAnsiLen - 1:0;
          pDataInfo->dwUnicodeLen=dwUnicodeLen?dwUnicodeLen - 1:0;
          GlobalUnlock(pDataInfo);
        }
        SetClipboardData(cfAkelEditText, hDataInfo);
      }
      CloseClipboard();
    }
  }
}

BOOL AE_EditPasteFromClipboard(AKELEDIT *ae, BOOL bAnsi)
{
  HGLOBAL hDataInfo;
  HGLOBAL hData;
  LPVOID pData;
  AECLIPBOARDINFO *pDataInfo;
  AECLIPBOARDINFO cbi;
  BOOL bColumnSel;
  BOOL bResult=FALSE;

  bColumnSel=IsClipboardFormatAvailable(cfAkelEditColumnSel);

  if (OpenClipboard(NULL))
  {
    //Get AkelEdit clipboard text length
    cbi.dwAnsiLen=(DWORD)-1;
    cbi.dwUnicodeLen=(DWORD)-1;

    if (hDataInfo=GetClipboardData(cfAkelEditText))
    {
      if (pDataInfo=(AECLIPBOARDINFO *)GlobalLock(hDataInfo))
      {
        cbi.dwAnsiLen=pDataInfo->dwAnsiLen;
        cbi.dwUnicodeLen=pDataInfo->dwUnicodeLen;
        GlobalUnlock(pDataInfo);
      }
    }

    //Get clipboard text
    if (!bAnsi && (hData=GetClipboardData(CF_UNICODETEXT)))
    {
      if (pData=GlobalLock(hData))
      {
        AE_ReplaceSel(ae, (wchar_t *)pData, cbi.dwUnicodeLen, AELB_ASINPUT, bColumnSel, NULL, NULL);
        bResult=TRUE;
        GlobalUnlock(hData);
      }
    }
    else if (hData=GetClipboardData(CF_TEXT))
    {
      if (pData=GlobalLock(hData))
      {
        AE_ReplaceSelAnsi(ae, CP_ACP, (char *)pData, cbi.dwAnsiLen, AELB_ASINPUT, bColumnSel, NULL, NULL);
        bResult=TRUE;
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
  return bResult;
}

void AE_EditChar(AKELEDIT *ae, WPARAM wParam, BOOL bUnicode)
{
  AE_NotifyChanging(ae, AETCT_CHAR);

  if (!bUnicode)
  {
    char chChar=(char)(WORD)wParam;
    wchar_t wchChar;

    if (ae->popt->bOverType)
    {
      if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
      {
        AECHARINDEX ciCharIndex=ae->ciCaretIndex;

        if (AE_IndexInc(&ciCharIndex) <= ciCharIndex.lpLine->nLineLen)
        {
          AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
          ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_OVERTYPECHAR;
        }
      }
    }
    MultiByteToWideChar(CP_ACP, 0, &chChar, 1, &wchChar, 1);
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
    if (AE_InsertText(ae, &ae->ciCaretIndex, &wchChar, 1, AELB_ASINPUT, FALSE, 0, NULL, NULL))
    {
      if (!ae->ptxt->bLockCollectUndo)
      {
        if (ae->ptxt->dwUndoLimit)
        {
          if (ae->popt->dwOptions & AECO_DETAILEDUNDO)
            AE_StackUndoGroupStop(ae);
          else
            ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
        }
      }
    }
  }
  else
  {
    wchar_t wchChar=(wchar_t)(WORD)wParam;

    if (ae->popt->bOverType)
    {
      if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
      {
        AECHARINDEX ciCharIndex=ae->ciCaretIndex;

        if (AE_IndexInc(&ciCharIndex) <= ciCharIndex.lpLine->nLineLen)
        {
          AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
          ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_OVERTYPECHAR;
        }
      }
    }
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
    if (AE_InsertText(ae, &ae->ciCaretIndex, &wchChar, 1, AELB_ASINPUT, FALSE, 0, NULL, NULL))
    {
      if (!ae->ptxt->bLockCollectUndo)
      {
        if (ae->ptxt->dwUndoLimit)
        {
          if (ae->popt->dwOptions & AECO_DETAILEDUNDO)
            AE_StackUndoGroupStop(ae);
          else
            ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
        }
      }
    }
  }
  AE_NotifyChanged(ae); //AETCT_CHAR
}

void AE_EditKeyReturn(AKELEDIT *ae)
{
  AECHARINDEX ciCharIndex;
  wchar_t *wpNewLine;
  int nNewLine;

  AE_NotifyChanging(ae, AETCT_KEYRETURN);

  nNewLine=AE_GetNewLineString(ae, AELB_ASOUTPUT, &wpNewLine);
  AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
  ciCharIndex=ae->ciCaretIndex;
  ciCharIndex.nCharInLine=min(ciCharIndex.nCharInLine, ciCharIndex.lpLine->nLineLen);
  if (AE_InsertText(ae, &ciCharIndex, wpNewLine, nNewLine, AELB_ASINPUT, FALSE, 0, NULL, NULL))
  {
    if (!ae->ptxt->bLockCollectUndo)
    {
      if (ae->ptxt->dwUndoLimit)
      {
        if (ae->popt->dwOptions & AECO_DETAILEDUNDO)
          AE_StackUndoGroupStop(ae);
        else
          ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
      }
    }
  }
  AE_NotifyChanged(ae); //AETCT_KEYRETURN
}

void AE_EditKeyBackspace(AKELEDIT *ae, BOOL bControl)
{
  AECHARINDEX ciCharIndex=ae->ciCaretIndex;

  AE_NotifyChanging(ae, AETCT_KEYBACKSPACE);

  if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (ae->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
    {
      if (ciCharIndex.nCharInLine > ciCharIndex.lpLine->nLineLen)
      {
        ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
        AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, ae->bColumnSel, AESELT_LOCKNOTIFY, 0);
        goto End;
      }
    }

    if ((bControl && AE_GetPrevBreak(ae, &ae->ciSelStartIndex, &ciCharIndex, FALSE, ae->popt->dwWordBreak)) ||
        (!bControl && AE_GetIndex(ae, AEGI_PREVCHAR, &ae->ciSelStartIndex, &ciCharIndex, FALSE)))
    {
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ciCharIndex, &ae->ciSelStartIndex, FALSE, 0);
      AE_StackUndoGroupStop(ae);

      if (!ae->ptxt->bLockCollectUndo)
      {
        if (ae->ptxt->dwUndoLimit)
        {
          ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_BACKSPACEKEY;
        }
      }
    }
    else if (!(ae->popt->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
  }
  else
  {
    AE_StackUndoGroupStop(ae);
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
    AE_StackUndoGroupStop(ae);
  }

  End:
  AE_NotifyChanged(ae); //AETCT_KEYBACKSPACE
}

void AE_EditKeyDelete(AKELEDIT *ae, BOOL bControl)
{
  AECHARINDEX ciCharIndex=ae->ciCaretIndex;
  int nSpaces=0;

  AE_NotifyChanging(ae, AETCT_KEYDELETE);

  if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (ae->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
    {
      wchar_t *wpSpaces;
      int i;

      nSpaces=ciCharIndex.nCharInLine - ciCharIndex.lpLine->nLineLen;

      if (nSpaces > 0)
      {
        if (wpSpaces=(wchar_t *)AE_HeapAlloc(ae, 0, (nSpaces + 1) * sizeof(wchar_t)))
        {
          for (i=0; i < nSpaces; ++i)
            wpSpaces[i]=L' ';
          wpSpaces[nSpaces]=L'\0';

          ciCharIndex.nCharInLine=min(ciCharIndex.nCharInLine, ciCharIndex.lpLine->nLineLen);
          AE_StackUndoGroupStop(ae);
          AE_InsertText(ae, &ciCharIndex, wpSpaces, nSpaces, AELB_ASIS, FALSE, 0, NULL, NULL);

          AE_HeapFree(ae, 0, (LPVOID)wpSpaces);
        }
      }
    }

    if ((bControl && AE_GetNextBreak(ae, &ae->ciSelStartIndex, &ciCharIndex, FALSE, ae->popt->dwWordBreak)) ||
        (!bControl && AE_GetIndex(ae, AEGI_NEXTCHAR, &ae->ciSelStartIndex, &ciCharIndex, FALSE)))
    {
      if (!nSpaces) AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, FALSE, 0);
      AE_StackUndoGroupStop(ae);

      if (!ae->ptxt->bLockCollectUndo)
      {
        if (ae->ptxt->dwUndoLimit)
        {
          ae->ptxt->lpCurrentUndo->dwFlags|=AEUN_DELETEKEY;
        }
      }
    }
    else if (!(ae->popt->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
  }
  else
  {
    AE_StackUndoGroupStop(ae);
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
    AE_StackUndoGroupStop(ae);
  }
  AE_NotifyChanged(ae); //AETCT_KEYDELETE
}

void AE_EditSelectAll(AKELEDIT *ae, DWORD dwSelFlags, DWORD dwSelType)
{
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;

  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciFirstChar, FALSE);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciLastChar, FALSE);
  AE_SetSelectionPos(ae, &ciLastChar, &ciFirstChar, FALSE, dwSelFlags, dwSelType);
}

BOOL AE_AkelEditGetSel(AKELEDIT *ae, AESELECTION *aes, AECHARINDEX *lpciCaret)
{
  if (aes)
  {
    aes->crSel.ciMin=ae->ciSelStartIndex;
    aes->crSel.ciMax=ae->ciSelEndIndex;
    aes->dwFlags=ae->bColumnSel;
  }
  if (lpciCaret)
  {
    *lpciCaret=ae->ciCaretIndex;
  }
  if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
    return FALSE;
  return TRUE;
}

void AE_AkelEditSetSel(AKELEDIT *ae, const AESELECTION *aes, const AECHARINDEX *lpciCaret)
{
  if (!lpciCaret)
  {
    if (AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
      AE_SetSelectionPos(ae, &aes->crSel.ciMax, &aes->crSel.ciMin, FALSE, aes->dwFlags, AESCT_SETSELMESSAGE);
    else
      AE_SetSelectionPos(ae, &aes->crSel.ciMin, &aes->crSel.ciMax, FALSE, aes->dwFlags, AESCT_SETSELMESSAGE);
  }
  else
  {
    if (AE_IndexCompare(lpciCaret, &aes->crSel.ciMax) >= 0)
      AE_SetSelectionPos(ae, &aes->crSel.ciMax, &aes->crSel.ciMin, FALSE, aes->dwFlags, AESCT_SETSELMESSAGE);
    else
      AE_SetSelectionPos(ae, &aes->crSel.ciMin, &aes->crSel.ciMax, FALSE, aes->dwFlags, AESCT_SETSELMESSAGE);
  }
}

BOOL AE_RichEditGetSel(AKELEDIT *ae, LONG *nMin, LONG *nMax)
{
  *nMin=AE_AkelIndexToRichOffset(ae, &ae->ciSelStartIndex);
  *nMax=AE_AkelIndexToRichOffset(ae, &ae->ciSelEndIndex);
  if (*nMin == *nMax)
    return FALSE;
  return TRUE;
}

void AE_RichEditSetSel(AKELEDIT *ae, LONG nMin, LONG nMax)
{
  AECHARRANGE cr;

  AE_RichOffsetToAkelIndex(ae, nMin, &cr.ciMin);
  AE_RichOffsetToAkelIndex(ae, nMax, &cr.ciMax);
  AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, FALSE, 0, AESCT_SETSELMESSAGE);
}

void AE_GetColors(AKELEDIT *ae, AECOLORS *aec)
{
  if (aec->dwFlags & AECLR_BASICTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
    else
      aec->crBasicText=ae->popt->crBasicText;
  }
  if (aec->dwFlags & AECLR_BASICBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crBasicBk=GetSysColor(COLOR_WINDOW);
    else
      aec->crBasicBk=ae->popt->crBasicBk;
  }
  if (aec->dwFlags & AECLR_SELTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
    else
      aec->crSelText=ae->popt->crSelText;
  }
  if (aec->dwFlags & AECLR_SELBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
    else
      aec->crSelBk=ae->popt->crSelBk;
  }
  if (aec->dwFlags & AECLR_ACTIVELINETEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
    else
      aec->crActiveLineText=ae->popt->crActiveLineText;
  }
  if (aec->dwFlags & AECLR_ACTIVELINEBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crActiveLineBk=GetSysColor(COLOR_WINDOW);
    else
      aec->crActiveLineBk=ae->popt->crActiveLineBk;
  }
  if (aec->dwFlags & AECLR_ACTIVECOLUMN)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crActiveColumn=RGB(0x00, 0x00, 0x00);
    else
      aec->crActiveColumn=ae->popt->crActiveColumn;
  }
  if (aec->dwFlags & AECLR_COLUMNMARKER)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crColumnMarker=GetSysColor(COLOR_BTNFACE);
    else
      aec->crColumnMarker=ae->popt->crColumnMarker;
  }
  if (aec->dwFlags & AECLR_CARET)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crCaret=RGB(0x00, 0x00, 0x00);
    else
      aec->crCaret=ae->popt->crCaret;
  }
  if (aec->dwFlags & AECLR_URLTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crUrlText=RGB(0x00, 0x00, 0xFF);
    else
      aec->crUrlText=ae->popt->crUrlText;
  }
}

void AE_SetColors(AKELEDIT *ae, const AECOLORS *aec)
{
  BOOL bUpdateDrawRect=FALSE;
  BOOL bUpdateEditRect=FALSE;

  if (aec->dwFlags)
  {
    if (aec->dwFlags & AECLR_BASICTEXT)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
      }
      else
      {
        ae->popt->crBasicText=aec->crBasicText;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;
    }
    if (aec->dwFlags & AECLR_BASICBK)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crBasicBk=GetSysColor(COLOR_WINDOW);
      }
      else
      {
        ae->popt->crBasicBk=aec->crBasicBk;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateEditRect=TRUE;
    }
    if (aec->dwFlags & AECLR_SELTEXT)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
      }
      else
      {
        ae->popt->crSelText=aec->crSelText;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;
    }
    if (aec->dwFlags & AECLR_SELBK)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
      }
      else
      {
        ae->popt->crSelBk=aec->crSelBk;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;
    }
    if (aec->dwFlags & AECLR_ACTIVELINETEXT)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
      }
      else
      {
        ae->popt->crActiveLineText=aec->crActiveLineText;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;
    }
    if (aec->dwFlags & AECLR_ACTIVELINEBK)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crActiveLineBk=GetSysColor(COLOR_WINDOW);
      }
      else
      {
        ae->popt->crActiveLineBk=aec->crActiveLineBk;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;

      AE_UpdateCaret(ae, ae->bFocus);
    }
    if (aec->dwFlags & AECLR_ACTIVECOLUMN)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crActiveColumn=RGB(0x00, 0x00, 0x00);
      }
      else
      {
        ae->popt->crActiveColumn=aec->crActiveColumn;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;

      if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
        AE_ActiveColumnCreate(ae);
    }
    if (aec->dwFlags & AECLR_COLUMNMARKER)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crColumnMarker=GetSysColor(COLOR_BTNFACE);
      }
      else
      {
        ae->popt->crColumnMarker=aec->crColumnMarker;
        ae->popt->bDefaultColors=FALSE;
      }
      AE_ColumnMarkerErase(ae);
    }
    if (aec->dwFlags & AECLR_CARET)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crCaret=RGB(0x00, 0x00, 0x00);
      }
      else
      {
        ae->popt->crCaret=aec->crCaret;
        ae->popt->bDefaultColors=FALSE;
      }
      AE_UpdateCaret(ae, ae->bFocus);
    }
    if (aec->dwFlags & AECLR_URLTEXT)
    {
      if (aec->dwFlags & AECLR_DEFAULT)
      {
        ae->popt->crUrlText=RGB(0x00, 0x00, 0xFF);
      }
      else
      {
        ae->popt->crUrlText=aec->crUrlText;
        ae->popt->bDefaultColors=FALSE;
      }
      bUpdateDrawRect=TRUE;
    }

    if (bUpdateEditRect)
      InvalidateRect(ae->hWndEdit, &ae->rcEdit, TRUE);
    else if (bUpdateDrawRect)
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    AE_StackUpdateClones(ae);
  }
}

void AE_NotifyErrSpace(AKELEDIT *ae, DWORD dwBytes)
{
  //Send AEN_ERRSPACE
  {
    AENERRSPACE es;

    es.hdr.hwndFrom=ae->hWndEdit;
    es.hdr.idFrom=ae->nEditCtrlID;
    es.hdr.code=AEN_ERRSPACE;
    es.hdr.docFrom=(AEHDOC)ae;
    es.dwBytes=dwBytes;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&es);
  }

  //Send EN_ERRSPACE
  AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_ERRSPACE), (LPARAM)ae->hWndEdit);
}

void AE_NotifySetFocus(AKELEDIT *ae, HWND hWndLost)
{
  //Send AEN_SETFOCUS
  {
    AENFOCUS fcs;

    fcs.hdr.hwndFrom=ae->hWndEdit;
    fcs.hdr.idFrom=ae->nEditCtrlID;
    fcs.hdr.code=AEN_SETFOCUS;
    fcs.hdr.docFrom=(AEHDOC)ae;
    fcs.hWndChange=hWndLost;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&fcs);
  }

  //Send EN_SETFOCUS
  AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_SETFOCUS), (LPARAM)ae->hWndEdit);
}

void AE_NotifyKillFocus(AKELEDIT *ae, HWND hWndReceive)
{
  //Send AEN_KILLFOCUS
  {
    AENFOCUS fcs;

    fcs.hdr.hwndFrom=ae->hWndEdit;
    fcs.hdr.idFrom=ae->nEditCtrlID;
    fcs.hdr.code=AEN_KILLFOCUS;
    fcs.hdr.docFrom=(AEHDOC)ae;
    fcs.hWndChange=hWndReceive;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&fcs);
  }

  //Send EN_KILLFOCUS
  AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_KILLFOCUS), (LPARAM)ae->hWndEdit);
}

void AE_NotifyHScroll(AKELEDIT *ae)
{
  //Send AEN_HSCROLL
  if (ae->popt->dwEventMask & AENM_SCROLL)
  {
    AENSCROLL scr;

    scr.hdr.hwndFrom=ae->hWndEdit;
    scr.hdr.idFrom=ae->nEditCtrlID;
    scr.hdr.code=AEN_HSCROLL;
    scr.hdr.docFrom=(AEHDOC)ae;
    scr.nPosNew=ae->nHScrollPos;
    scr.nPosOld=ae->nLastHScrollPos;
    scr.nPosMax=ae->ptxt->nHScrollMax;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&scr);
  }

  //Send EN_HSCROLL
  if (ae->popt->dwRichEventMask & ENM_SCROLL)
  {
    AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_HSCROLL), (LPARAM)ae->hWndEdit);
  }
}

void AE_NotifyVScroll(AKELEDIT *ae)
{
  //Send AEN_VSCROLL
  if (ae->popt->dwEventMask & AENM_SCROLL)
  {
    AENSCROLL scr;

    scr.hdr.hwndFrom=ae->hWndEdit;
    scr.hdr.idFrom=ae->nEditCtrlID;
    scr.hdr.code=AEN_VSCROLL;
    scr.hdr.docFrom=(AEHDOC)ae;
    scr.nPosNew=ae->nVScrollPos;
    scr.nPosOld=ae->nLastVScrollPos;
    scr.nPosMax=ae->ptxt->nVScrollMax;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&scr);
  }

  //Send EN_VSCROLL
  if (ae->popt->dwRichEventMask & ENM_SCROLL)
  {
    AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_VSCROLL), (LPARAM)ae->hWndEdit);
  }
}

void AE_NotifySetRect(AKELEDIT *ae)
{
  //Send AEN_SETRECT
  {
    AENSETRECT sr;

    sr.hdr.hwndFrom=ae->hWndEdit;
    sr.hdr.idFrom=ae->nEditCtrlID;
    sr.hdr.code=AEN_SETRECT;
    sr.hdr.docFrom=(AEHDOC)ae;
    sr.rcDraw=ae->rcDraw;
    sr.rcEdit=ae->rcEdit;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sr);
  }
}

void AE_NotifyMaxText(AKELEDIT *ae)
{
  //Send AEN_MAXTEXT
  {
    AENMAXTEXT mt;

    mt.hdr.hwndFrom=ae->hWndEdit;
    mt.hdr.idFrom=ae->nEditCtrlID;
    mt.hdr.code=AEN_MAXTEXT;
    mt.hdr.docFrom=(AEHDOC)ae;
    mt.dwTextLimit=ae->ptxt->dwTextLimit;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&mt);
  }

  //Send EN_MAXTEXT
  AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_MAXTEXT), (LPARAM)ae->hWndEdit);
}

BOOL AE_NotifyProgress(AKELEDIT *ae, DWORD dwType, DWORD dwTimeElapsed, int nCurrent, int nMaximum)
{
  BOOL bResult=FALSE;

  //Send AEN_PROGRESS
  {
    AENPROGRESS pgs;

    pgs.hdr.hwndFrom=ae->hWndEdit;
    pgs.hdr.idFrom=ae->nEditCtrlID;
    pgs.hdr.code=AEN_PROGRESS;
    pgs.hdr.docFrom=(AEHDOC)ae;
    pgs.dwType=dwType;
    pgs.dwTimeElapsed=dwTimeElapsed;
    pgs.nCurrent=nCurrent;
    pgs.nMaximum=nMaximum;
    bResult=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&pgs);
  }
  return bResult;
}

void AE_NotifyModify(AKELEDIT *ae)
{
  //Send AEN_MODIFY
  if (ae->popt->dwEventMask & AENM_MODIFY)
  {
    AENMODIFY mdf;

    mdf.hdr.hwndFrom=ae->hWndEdit;
    mdf.hdr.idFrom=ae->nEditCtrlID;
    mdf.hdr.code=AEN_MODIFY;
    mdf.hdr.docFrom=(AEHDOC)ae;
    mdf.bModified=ae->ptxt->bModified;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&mdf);
  }
}

void AE_NotifySelChanging(AKELEDIT *ae, DWORD dwType)
{
  ae->dwNotifySelChange=dwType;

  //Send AEN_SELCHANGING
  if (ae->popt->dwEventMask & AENM_SELCHANGE)
  {
    AENSELCHANGE sc;

    sc.hdr.hwndFrom=ae->hWndEdit;
    sc.hdr.idFrom=ae->nEditCtrlID;
    sc.hdr.code=AEN_SELCHANGING;
    sc.hdr.docFrom=(AEHDOC)ae;
    sc.dwType=dwType;
    AE_AkelEditGetSel(ae, &sc.aes, &sc.ciCaret);
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }
}

void AE_NotifySelChanged(AKELEDIT *ae)
{
  //Send AEN_SELCHANGED
  if (ae->popt->dwEventMask & AENM_SELCHANGE)
  {
    AENSELCHANGE sc;

    sc.hdr.hwndFrom=ae->hWndEdit;
    sc.hdr.idFrom=ae->nEditCtrlID;
    sc.hdr.code=AEN_SELCHANGED;
    sc.hdr.docFrom=(AEHDOC)ae;
    sc.dwType=ae->dwNotifySelChange;
    AE_AkelEditGetSel(ae, &sc.aes, &sc.ciCaret);
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }

  //Send EN_SELCHANGE
  if (ae->popt->dwRichEventMask & ENM_SELCHANGE)
  {
    SELCHANGE sc;

    sc.nmhdr.hwndFrom=ae->hWndEdit;
    sc.nmhdr.idFrom=ae->nEditCtrlID;
    sc.nmhdr.code=EN_SELCHANGE;
    sc.chrg.cpMin=ae->nSelStartCharOffset;
    sc.chrg.cpMax=ae->nSelEndCharOffset;

    if (sc.chrg.cpMin == sc.chrg.cpMax)
    {
      sc.seltyp=SEL_EMPTY;
    }
    else
    {
      sc.seltyp=SEL_TEXT;
      if (sc.chrg.cpMax - sc.chrg.cpMin > 1)
        sc.seltyp|=SEL_MULTICHAR;
    }
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }

  ae->dwNotifySelChange=0;
}

void AE_NotifyTextChanging(AKELEDIT *ae, DWORD dwType)
{
  ae->dwNotifyTextChange=dwType;

  //Send AEN_TEXTCHANGING
  if (ae->popt->dwEventMask & AENM_TEXTCHANGE)
  {
    AENTEXTCHANGE tc;

    tc.hdr.hwndFrom=ae->hWndEdit;
    tc.hdr.idFrom=ae->nEditCtrlID;
    tc.hdr.code=AEN_TEXTCHANGING;
    tc.hdr.docFrom=(AEHDOC)ae;
    tc.dwType=ae->dwNotifyTextChange;
    AE_AkelEditGetSel(ae, &tc.aes, &tc.ciCaret);
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }
}

void AE_NotifyTextChanged(AKELEDIT *ae)
{
  AE_StackUpdateClones(ae);

  //Send AEN_TEXTCHANGED
  if (ae->popt->dwEventMask & AENM_TEXTCHANGE)
  {
    AENTEXTCHANGE tc;

    tc.hdr.hwndFrom=ae->hWndEdit;
    tc.hdr.idFrom=ae->nEditCtrlID;
    tc.hdr.code=AEN_TEXTCHANGED;
    tc.hdr.docFrom=(AEHDOC)ae;
    tc.dwType=ae->dwNotifyTextChange;
    AE_AkelEditGetSel(ae, &tc.aes, &tc.ciCaret);
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }

  //Send EN_CHANGE
  if (ae->popt->dwRichEventMask & ENM_CHANGE)
  {
    AE_SendMessage(ae, ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_CHANGE), (LPARAM)ae->hWndEdit);
  }

  ae->dwNotifyTextChange=0;
}

void AE_NotifyChanging(AKELEDIT *ae, DWORD dwType)
{
  AE_NotifySelChanging(ae, LOWORD(dwType));
  AE_NotifyTextChanging(ae, dwType);
}

void AE_NotifyChanged(AKELEDIT *ae)
{
  //SelChanged
  if (ae->dwNotifyFlags & AENM_SELCHANGE)
  {
    ae->dwNotifyFlags&=~AENM_SELCHANGE;
  }
  AE_NotifySelChanged(ae);

  //TextChanged
  if (ae->dwNotifyFlags & AENM_TEXTCHANGE)
  {
    ae->dwNotifyFlags&=~AENM_TEXTCHANGE;

    if (!ae->ptxt->dwUndoLimit)
    {
      if (!ae->ptxt->lpSavePoint)
        ae->ptxt->bSavePointExist=FALSE;
    }
  }
  else ae->dwNotifyTextChange|=AETCT_NONE;

  AE_NotifyTextChanged(ae);

  //Modify
  if (ae->dwNotifyFlags & AENM_MODIFY)
  {
    ae->dwNotifyFlags&=~AENM_MODIFY;

    if (ae->ptxt->bModified != AE_GetModify(ae))
    {
      ae->ptxt->bModified=!ae->ptxt->bModified;
      AE_NotifyModify(ae);
    }
  }

  //MaxText
  if (ae->dwNotifyFlags & AENM_MAXTEXT)
  {
    ae->dwNotifyFlags&=~AENM_MAXTEXT;
    AE_NotifyMaxText(ae);
  }

  ae->dwNotifyFlags=0;
}

void AE_NotifyPoint(AKELEDIT *ae, DWORD dwType, AEPOINT *lpPoint)
{
  //Send AEN_POINT
  if (ae->popt->dwEventMask & AENM_POINT)
  {
    AENPOINT pnt;

    pnt.hdr.hwndFrom=ae->hWndEdit;
    pnt.hdr.idFrom=ae->nEditCtrlID;
    pnt.hdr.code=AEN_POINT;
    pnt.hdr.docFrom=(AEHDOC)ae;
    pnt.dwType=dwType;
    pnt.lpPoint=lpPoint;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&pnt);
  }
}

BOOL AE_NotifyDropFiles(AKELEDIT *ae, HDROP hDrop)
{
  BOOL bResult1=TRUE;
  BOOL bResult2=TRUE;

  //Send AEN_DROPFILES
  if (ae->popt->dwEventMask & AENM_DROPFILES)
  {
    AENDROPFILES df;
    AECHARINDEX ciCharIndex;
    POINT pt;

    GetCursorPos(&pt);
    ScreenToClient(ae->hWndEdit, &pt);
    AE_GetCharFromPos(ae, &pt, &ciCharIndex, NULL, FALSE);

    df.hdr.hwndFrom=ae->hWndEdit;
    df.hdr.idFrom=ae->nEditCtrlID;
    df.hdr.code=AEN_DROPFILES;
    df.hdr.docFrom=(AEHDOC)ae;
    df.hDrop=hDrop;
    df.ciChar=ciCharIndex;

    bResult1=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&df);
  }

  //Send EN_DROPFILES
  if (bResult1)
  {
    if (ae->popt->dwRichEventMask & ENM_DROPFILES)
    {
      ENDROPFILES df;
      AECHARINDEX ciCharIndex;
      POINT pt;

      GetCursorPos(&pt);
      ScreenToClient(ae->hWndEdit, &pt);
      AE_GetCharFromPos(ae, &pt, &ciCharIndex, NULL, FALSE);

      df.nmhdr.hwndFrom=ae->hWndEdit;
      df.nmhdr.idFrom=ae->nEditCtrlID;
      df.nmhdr.code=EN_DROPFILES;
      df.hDrop=hDrop;
      df.cp=AE_AkelIndexToRichOffset(ae, &ciCharIndex);
      df.fProtected=FALSE;

      bResult2=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&df);
    }
  }

  if (!bResult1 || !bResult2)
    return FALSE;
  return TRUE;
}

BOOL AE_NotifyDropSource(AKELEDIT *ae, int nAction, DWORD *lpdwEffect, DWORD dwDropResult)
{
  BOOL bResult=FALSE;

  //Send AEN_DROPSOURCE
  if (ae->popt->dwEventMask & AENM_DRAGDROP)
  {
    AENDROPSOURCE ds;

    ds.hdr.hwndFrom=ae->hWndEdit;
    ds.hdr.idFrom=ae->nEditCtrlID;
    ds.hdr.code=AEN_DROPSOURCE;
    ds.hdr.docFrom=(AEHDOC)ae;
    ds.nAction=nAction;
    if (lpdwEffect)
      ds.dwEffect=*lpdwEffect;
    else
      ds.dwEffect=0;
    ds.dwDropResult=dwDropResult;

    bResult=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&ds);

    if (lpdwEffect) *lpdwEffect=ds.dwEffect;
  }

  //Send EN_DRAGDROPDONE
  if (ae->popt->dwRichEventMask & ENM_DRAGDROPDONE)
  {
    if (nAction == AEDS_SOURCEDONE)
    {
      NMHDR hdr;

      hdr.hwndFrom=ae->hWndEdit;
      hdr.idFrom=ae->nEditCtrlID;
      hdr.code=EN_DRAGDROPDONE;

      AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&hdr);
    }
  }
  return bResult;
}

BOOL AE_NotifyDropTarget(AKELEDIT *ae, int nAction, POINT *pt, DWORD *lpdwEffect)
{
  BOOL bResult=FALSE;

  //Send AEN_DROPTARGET
  if (ae->popt->dwEventMask & AENM_DRAGDROP)
  {
    AENDROPTARGET dt;

    dt.hdr.hwndFrom=ae->hWndEdit;
    dt.hdr.idFrom=ae->nEditCtrlID;
    dt.hdr.code=AEN_DROPTARGET;
    dt.hdr.docFrom=(AEHDOC)ae;
    dt.nAction=nAction;
    if (pt)
      dt.pt=*pt;
    else
      GetCursorPos(&dt.pt);
    if (lpdwEffect)
      dt.dwEffect=*lpdwEffect;
    else
      dt.dwEffect=0;

    bResult=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&dt);

    if (pt) *pt=dt.pt;
    if (lpdwEffect) *lpdwEffect=dt.dwEffect;
  }
  return bResult;
}

BOOL AE_NotifyLink(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam, const AECHARRANGE *crLink)
{
  AECHARRANGE crText=*crLink;
  LRESULT lResult1=0;
  LRESULT lResult2=0;

  //Send AEN_LINK
  if (ae->popt->dwEventMask & AENM_LINK)
  {
    AENLINK lnk;

    lnk.hdr.hwndFrom=ae->hWndEdit;
    lnk.hdr.idFrom=ae->nEditCtrlID;
    lnk.hdr.code=AEN_LINK;
    lnk.hdr.docFrom=(AEHDOC)ae;
    lnk.uMsg=uMsg;
    lnk.wParam=wParam;
    lnk.lParam=lParam;
    lnk.crLink=crText;
    lResult1=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&lnk);
  }

  //Send EN_LINK
  if (!lResult1)
  {
    if (ae->popt->dwRichEventMask & ENM_LINK)
    {
      ENLINK enl;

      enl.nmhdr.hwndFrom=ae->hWndEdit;
      enl.nmhdr.idFrom=ae->nEditCtrlID;
      enl.nmhdr.code=EN_LINK;
      enl.msg=uMsg;
      enl.wParam=wParam;
      enl.lParam=lParam;
      enl.chrg.cpMin=AE_AkelIndexToRichOffset(ae, &crText.ciMin);
      enl.chrg.cpMax=AE_AkelIndexToRichOffset(ae, &crText.ciMax);
      lResult2=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&enl);
    }
  }

  if (lResult1 || lResult2)
    return TRUE;
  return FALSE;
}

BOOL AE_NotifyMsgFilter(AKELEDIT *ae, UINT uMsg, WPARAM *wParam, LPARAM *lParam)
{
  BOOL bResult=FALSE;

  //Send EN_MSGFILTER
  {
    MSGFILTER mf;

    mf.nmhdr.hwndFrom=ae->hWndEdit;
    mf.nmhdr.idFrom=ae->nEditCtrlID;
    mf.nmhdr.code=EN_MSGFILTER;
    mf.msg=uMsg;
    mf.wParam=*wParam;
    mf.lParam=*lParam;

    if (!(bResult=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&mf)))
    {
      *wParam=mf.wParam;
      *lParam=mf.lParam;
    }
  }
  return bResult;
}

LRESULT AE_SendMessage(AKELEDIT *ae, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  lResult=SendMessageA(hWnd, uMsg, wParam, lParam);

  AE_ActivateClone(lpAkelEditPrev, ae);
  lpAkelEditPrev=ae;
  return lResult;
}

void AE_ChangeTwoBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen)
{
  unsigned char *lpBufferEnd=lpBuffer + nBufferLen;
  unsigned char *lpByte=lpBuffer;
  unsigned char ch;

  for (; lpByte + 1 < lpBufferEnd; lpByte+=2)
  {
    ch=*lpByte;
    *lpByte=*(lpByte + 1);
    *(lpByte + 1)=ch;
  }
}

wchar_t* AE_wcschr(const wchar_t *s, wchar_t c, BOOL bMatchCase)
{
  if (c == L'\r' || c == L'\n')
  {
    while (*s != L'\0' && *s != L'\r' && *s != L'\n')
    {
      ++s;
    }
    if (*s != L'\0')
      return ((wchar_t *)s);
    return NULL;
  }
  else
  {
    while (*s != L'\0' && ((bMatchCase && *s != c) ||
                           (!bMatchCase && WideCharLower(*s) != WideCharLower(c))))
    {
      ++s;
    }
    if (*s != L'\0')
      return ((wchar_t *)s);
    return NULL;
  }
}


//// OLE Drag'n'Drop

HRESULT WINAPI AEIDropTarget_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj)
{
  if (AE_IsEqualIID(riid, &IID_IUnknown) || AE_IsEqualIID(riid, &IID_IDropTarget))
  {
    *ppvObj=lpTable;
    AEIDropTarget_AddRef((LPUNKNOWN)*ppvObj);
    return S_OK;
  }
  else
  {
    *ppvObj=NULL;
    return E_NOINTERFACE;
  }
}

ULONG WINAPI AEIDropTarget_AddRef(LPUNKNOWN lpTable)
{
  return ++(((AEIDropTarget *)lpTable)->uRefCount);
}

ULONG WINAPI AEIDropTarget_Release(LPUNKNOWN lpTable)
{
  return --(((AEIDropTarget *)lpTable)->uRefCount);
}

HRESULT WINAPI AEIDropTarget_DragEnter(LPUNKNOWN lpTable, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
  AEIDropTarget *pDropTarget=(AEIDropTarget *)lpTable;
  AKELEDIT *ae=(AKELEDIT *)pDropTarget->ae;
  FORMATETC fmtetc;
  BOOL bNotify;

  *pdwEffect=AE_DropTargetDropEffect(grfKeyState, *pdwEffect);
  bNotify=AE_NotifyDropTarget(ae, AEDT_TARGETENTER, (POINT *)&pt, pdwEffect);

  if (!bNotify && !(ae->popt->dwOptions & AECO_DISABLEDROP) && !(ae->popt->dwOptions & AECO_READONLY))
  {
    fmtetc.cfFormat=CF_UNICODETEXT;
    fmtetc.ptd=0;
    fmtetc.dwAspect=DVASPECT_CONTENT;
    fmtetc.lindex=-1;
    fmtetc.tymed=TYMED_HGLOBAL;

    if (pDataObject->lpVtbl->QueryGetData(pDataObject, &fmtetc) == S_OK)
    {
      pDropTarget->bAllowDrop=TRUE;
    }
    else
    {
      fmtetc.cfFormat=CF_TEXT;

      if (pDataObject->lpVtbl->QueryGetData(pDataObject, &fmtetc) == S_OK)
        pDropTarget->bAllowDrop=TRUE;
      else
        pDropTarget->bAllowDrop=FALSE;
    }
  }
  else pDropTarget->bAllowDrop=FALSE;

  if (pDropTarget->bAllowDrop)
  {
    fmtetc.cfFormat=cfAkelEditColumnSel;

    if (pDataObject->lpVtbl->QueryGetData(pDataObject, &fmtetc) == S_OK)
      pDropTarget->bColumnSel=TRUE;
    else
      pDropTarget->bColumnSel=FALSE;

    if (!ae->bFocus)
    {
      if (GetFocus())
        SetFocus(ae->hWndEdit);
      else
        AE_UpdateCaret(ae, TRUE);
    }
    ae->bDropping=TRUE;
    ae->nMoveBeforeDropScroll=AEMMB_DROPSCROLL;
    ScreenToClient(ae->hWndEdit, (POINT *)&pt);
    AE_DropTargetDropCursor(pDropTarget, &pt, pdwEffect);
  }
  else *pdwEffect=DROPEFFECT_NONE;

  return S_OK;
}

HRESULT WINAPI AEIDropTarget_DragOver(LPUNKNOWN lpTable, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
  AEIDropTarget *pDropTarget=(AEIDropTarget *)lpTable;
  AKELEDIT *ae=(AKELEDIT *)pDropTarget->ae;
  BOOL bNotify;

  *pdwEffect=AE_DropTargetDropEffect(grfKeyState, *pdwEffect);
  bNotify=AE_NotifyDropTarget(ae, AEDT_TARGETOVER, (POINT *)&pt, pdwEffect);

  if (!bNotify && pDropTarget->bAllowDrop)
  {
    ScreenToClient(ae->hWndEdit, (POINT *)&pt);
    AE_DropTargetDropCursor(pDropTarget, &pt, pdwEffect);
  }
  else *pdwEffect=DROPEFFECT_NONE;

  return S_OK;
}

HRESULT WINAPI AEIDropTarget_DragLeave(LPUNKNOWN lpTable)
{
  AEIDropTarget *pDropTarget=(AEIDropTarget *)lpTable;
  AKELEDIT *ae=(AKELEDIT *)pDropTarget->ae;

  AE_DropTargetDropCursor(pDropTarget, NULL, NULL);
  ae->bDropping=FALSE;
  if (!ae->bFocus) DestroyCaret();
  AE_NotifyDropTarget(ae, AEDT_TARGETLEAVE, NULL, NULL);
  return S_OK;
}

HRESULT WINAPI AEIDropTarget_Drop(LPUNKNOWN lpTable, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
  AEIDropTarget *pDropTarget=(AEIDropTarget *)lpTable;
  AKELEDIT *ae=(AKELEDIT *)pDropTarget->ae;
  BOOL bNotify;

  *pdwEffect=AE_DropTargetDropEffect(grfKeyState, *pdwEffect);
  ae->bDropping=FALSE;
  if (!ae->bFocus) DestroyCaret();
  bNotify=AE_NotifyDropTarget(ae, AEDT_TARGETDROP, (POINT *)&pt, pdwEffect);

  if (!bNotify && pDropTarget->bAllowDrop)
  {
    AECHARINDEX ciCharIndex;
    FORMATETC fmtetc;
    STGMEDIUM stgmed;
    LPVOID pData;

    ScreenToClient(ae->hWndEdit, (POINT *)&pt);
    AE_GetCharFromPos(ae, (POINT *)&pt, &ciCharIndex, NULL, pDropTarget->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

    //Get unicode text
    fmtetc.cfFormat=CF_UNICODETEXT;
    fmtetc.ptd=0;
    fmtetc.dwAspect=DVASPECT_CONTENT;
    fmtetc.lindex=-1;
    fmtetc.tymed=TYMED_HGLOBAL;

    if (pDataObject->lpVtbl->QueryGetData(pDataObject, &fmtetc) == S_OK)
    {
      if (pDataObject->lpVtbl->GetData(pDataObject, &fmtetc, &stgmed) == S_OK)
      {
        if (pData=GlobalLock(stgmed.hGlobal))
        {
          AECHARINDEX ciStart={0};
          AECHARINDEX ciEnd={0};

          AE_NotifyChanging(ae, AETCT_DROPINSERT);
          AE_StackUndoGroupStop(ae);

          //Delete
          if (*pdwEffect & DROPEFFECT_MOVE)
          {
            AKELEDIT *aeSource;
            AEPOINT *lpPoint;

            if (aeSource=AE_StackDraggingGet(ae))
            {
              if (aeSource != ae)
              {
                AE_ActivateClone(lpAkelEditPrev, aeSource);
                lpAkelEditPrev=aeSource;
                AE_NotifyChanging(aeSource, AETCT_DRAGDELETE);
              }
              lpPoint=AE_StackPointInsert(aeSource, &ciCharIndex);
              AE_DeleteTextRange(aeSource, &aeSource->ciSelStartIndex, &aeSource->ciSelEndIndex, aeSource->bColumnSel, AEDELT_LOCKSCROLL);
              ciCharIndex=lpPoint->ciPoint;
              AE_StackPointDelete(aeSource, lpPoint);
              aeSource->bDragSelectionDelete=FALSE;

              if (aeSource != ae)
              {
                AE_NotifyChanged(aeSource); //AETCT_DRAGDELETE
                AE_ActivateClone(lpAkelEditPrev, ae);
                lpAkelEditPrev=ae;
              }
            }
          }

          //Insert
          {
            AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, FALSE, AESELT_LOCKNOTIFY|AESELT_LOCKUNDOGROUPING, 0);
            AE_InsertText(ae, &ciCharIndex, (wchar_t *)pData, ae->dwDragSelectionLength?ae->dwDragSelectionLength:(DWORD)-1, AELB_ASINPUT, pDropTarget->bColumnSel, 0, &ciStart, &ciEnd);

            if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
            {
              AE_IndexUpdate(ae, &ciCharIndex);
              ciStart=ciCharIndex;
            }
          }
          AE_StackUndoGroupStop(ae);
          AE_SetSelectionPos(ae, &ciEnd, &ciStart, pDropTarget->bColumnSel, AESELT_LOCKNOTIFY, 0);
          AE_NotifyChanged(ae); //AETCT_DROPINSERT

          GlobalUnlock(stgmed.hGlobal);
        }
        ReleaseStgMedium(&stgmed);
      }
    }
    else
    {
      //Get ansi text
      fmtetc.cfFormat=CF_TEXT;

      if (pDataObject->lpVtbl->QueryGetData(pDataObject, &fmtetc) == S_OK)
      {
        if (pDataObject->lpVtbl->GetData(pDataObject, &fmtetc, &stgmed) == S_OK)
        {
          if (pData=GlobalLock(stgmed.hGlobal))
          {
            AECHARINDEX ciStart={0};
            AECHARINDEX ciEnd={0};

            AE_NotifyChanging(ae, AETCT_DROPINSERT);
            AE_StackUndoGroupStop(ae);

            //Delete
            if (*pdwEffect & DROPEFFECT_MOVE)
            {
              AKELEDIT *aeSource;
              AEPOINT *lpPoint;

              if (aeSource=AE_StackDraggingGet(ae))
              {
                if (aeSource != ae)
                {
                  AE_ActivateClone(lpAkelEditPrev, aeSource);
                  lpAkelEditPrev=aeSource;
                  AE_NotifyChanging(aeSource, AETCT_DRAGDELETE);
                }
                lpPoint=AE_StackPointInsert(aeSource, &ciCharIndex);
                AE_DeleteTextRange(aeSource, &aeSource->ciSelStartIndex, &aeSource->ciSelEndIndex, aeSource->bColumnSel, AEDELT_LOCKSCROLL);
                ciCharIndex=lpPoint->ciPoint;
                AE_StackPointDelete(aeSource, lpPoint);
                aeSource->bDragSelectionDelete=FALSE;

                if (aeSource != ae)
                {
                  AE_NotifyChanged(aeSource); //AETCT_DRAGDELETE
                  AE_ActivateClone(lpAkelEditPrev, ae);
                  lpAkelEditPrev=ae;
                }
              }
            }

            //Insert
            {
              wchar_t *wszText;
              DWORD dwUnicodeBytes;

              dwUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, (char *)pData, -1, NULL, 0) * sizeof(wchar_t);

              if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
              {
                MultiByteToWideChar(CP_ACP, 0, (char *)pData, -1, wszText, dwUnicodeBytes / sizeof(wchar_t));
                AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, FALSE, AESELT_LOCKNOTIFY|AESELT_LOCKUNDOGROUPING, 0);
                AE_InsertText(ae, &ciCharIndex, wszText, dwUnicodeBytes / sizeof(wchar_t) - 1, AELB_ASINPUT, pDropTarget->bColumnSel, 0, &ciStart, &ciEnd);

                if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
                {
                  AE_IndexUpdate(ae, &ciCharIndex);
                  ciStart=ciCharIndex;
                }
                AE_HeapFree(ae, 0, (LPVOID)wszText);
              }
            }
            AE_StackUndoGroupStop(ae);
            AE_SetSelectionPos(ae, &ciEnd, &ciStart, pDropTarget->bColumnSel, AESELT_LOCKNOTIFY, 0);
            AE_NotifyChanged(ae); //AETCT_DROPINSERT

            GlobalUnlock(stgmed.hGlobal);
          }
          ReleaseStgMedium(&stgmed);
        }
      }
    }
  }
  else *pdwEffect=DROPEFFECT_NONE;

  return S_OK;
}

DWORD AE_DropTargetDropEffect(DWORD grfKeyState, DWORD dwAllowed)
{
  DWORD dwEffect=DROPEFFECT_NONE;

  if (grfKeyState & MK_CONTROL)
  {
    dwEffect=dwAllowed & DROPEFFECT_COPY;
  }
  else if (grfKeyState & MK_SHIFT)
  {
    dwEffect=dwAllowed & DROPEFFECT_MOVE;
  }

  if (dwEffect == DROPEFFECT_NONE)
  {
    if (dwAllowed & DROPEFFECT_COPY) dwEffect=DROPEFFECT_COPY;
    if (dwAllowed & DROPEFFECT_MOVE) dwEffect=DROPEFFECT_MOVE;
  }
  return dwEffect;
}

void AE_DropTargetDropCursor(AEIDropTarget *pDropTarget, POINTL *pt, DWORD *pdwEffect)
{
  AKELEDIT *ae=(AKELEDIT *)pDropTarget->ae;

  if (!pdwEffect || *pdwEffect == DROPEFFECT_NONE)
  {
    AE_ScrollToCaret(ae, &ae->ptCaret, FALSE);
    AE_SetCaretPos(ae, &ae->ptCaret);
    ae->nMoveBeforeDropScroll=AEMMB_DROPSCROLL;
  }
  else
  {
    if (!PtInRect(&ae->rcDraw, *(POINT *)pt))
    {
      //Deny dropping in non-rect
      AE_SetCaretPos(ae, &ae->ptCaret);
      ae->nMoveBeforeDropScroll=AEMMB_DROPSCROLL;
      *pdwEffect=DROPEFFECT_NONE;
    }
    else
    {
      AECHARINDEX ciCharIndex;
      POINT ptGlobal;
      DWORD dwScrollTest;

      AE_GetCharFromPos(ae, (POINT *)pt, &ciCharIndex, &ptGlobal, pDropTarget->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

      //Scroll to dropping point
      dwScrollTest=AE_ScrollToPointEx(ae, AESC_TEST|AESC_POINTGLOBAL|AESC_OFFSETCHARX|AESC_OFFSETCHARY, &ptGlobal, 2, 1);

      if ((dwScrollTest & AECSE_SCROLLEDX) || (dwScrollTest & AECSE_SCROLLEDY))
      {
        if (ae->nMoveBeforeDropScroll > 0)
          ae->nMoveBeforeDropScroll-=3;
      }
      else
      {
        dwScrollTest=AE_ScrollToPointEx(ae, AESC_TEST|AESC_POINTGLOBAL|AESC_OFFSETCHARX|AESC_OFFSETCHARY, &ptGlobal, 3, 2);

        if ((dwScrollTest & AECSE_SCROLLEDX) || (dwScrollTest & AECSE_SCROLLEDY))
        {
          if (ae->nMoveBeforeDropScroll > 0)
            ae->nMoveBeforeDropScroll-=2;
        }
        else ae->nMoveBeforeDropScroll=min(ae->nMoveBeforeDropScroll + 1, AEMMB_DROPSCROLL);
      }
      if (ae->nMoveBeforeDropScroll <= 0)
        AE_ScrollToPointEx(ae, AESC_POINTGLOBAL|AESC_OFFSETCHARX|AESC_OFFSETCHARY, &ptGlobal, 3, 2);

      //Set caret position
      if (ae->bDragging &&
          (((*pdwEffect & DROPEFFECT_COPY) &&
             ciCharIndex.nCharInLine > ciCharIndex.lpLine->nSelStart &&
             ciCharIndex.nCharInLine < ciCharIndex.lpLine->nSelEnd) ||
           ((*pdwEffect & DROPEFFECT_MOVE) &&
             ciCharIndex.nLine >= ae->ciSelStartIndex.nLine &&
             ciCharIndex.nLine <= ae->ciSelEndIndex.nLine &&
             ciCharIndex.nCharInLine >= ciCharIndex.lpLine->nSelStart &&
             ciCharIndex.nCharInLine <= ciCharIndex.lpLine->nSelEnd)))
      {
        //Deny dropping in selection
        AE_SetCaretPos(ae, &ae->ptCaret);
        *pdwEffect=DROPEFFECT_NONE;
      }
      else
      {
        AE_SetCaretPos(ae, &ptGlobal);
      }
    }
  }
}

HRESULT WINAPI AEIDropSource_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj)
{
  if (AE_IsEqualIID(riid, &IID_IUnknown) || AE_IsEqualIID(riid, &IID_IDropSource))
  {
    *ppvObj=lpTable;
    AEIDropSource_AddRef((LPUNKNOWN)*ppvObj);
    return S_OK;
  }
  else
  {
    *ppvObj=NULL;
    return E_NOINTERFACE;
  }
}

ULONG WINAPI AEIDropSource_AddRef(LPUNKNOWN lpTable)
{
  return ++(((AEIDropSource *)lpTable)->uRefCount);
}

ULONG WINAPI AEIDropSource_Release(LPUNKNOWN lpTable)
{
  return --(((AEIDropSource *)lpTable)->uRefCount);
}

HRESULT WINAPI AEIDropSource_QueryContinueDrag(LPUNKNOWN lpTable, BOOL fEscapePressed, DWORD grfKeyState)
{
  if (!lpAkelEditDrag)
    return DRAGDROP_S_CANCEL;
  if (fEscapePressed == TRUE)
    return DRAGDROP_S_CANCEL;
  if (!(grfKeyState & MK_LBUTTON))
    return DRAGDROP_S_DROP;

  return S_OK;
}

HRESULT WINAPI AEIDropSource_GiveFeedback(LPUNKNOWN lpTable, DWORD dwEffect)
{
  return DRAGDROP_S_USEDEFAULTCURSORS;
}

HRESULT WINAPI AEIDataObject_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj)
{
  if (AE_IsEqualIID(riid, &IID_IUnknown) || AE_IsEqualIID(riid, &IID_IDataObject))
  {
    *ppvObj=lpTable;
    AEIDataObject_AddRef((LPUNKNOWN)*ppvObj);
    return S_OK;
  }
  else
  {
    *ppvObj=NULL;
    return E_NOINTERFACE;
  }
}

ULONG WINAPI AEIDataObject_AddRef(LPUNKNOWN lpTable)
{
  return ++(((AEIDataObject *)lpTable)->uRefCount);
}

ULONG WINAPI AEIDataObject_Release(LPUNKNOWN lpTable)
{
  return --(((AEIDataObject *)lpTable)->uRefCount);
}

HRESULT WINAPI AEIDataObject_GetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
  AEIDataObject *pDataObj=(AEIDataObject *)lpTable;
  int nIndex;

  if ((nIndex=AE_DataObjectLookupFormatEtc(pDataObj, pFormatEtc)) == -1)
    return DV_E_FORMATETC;

  pMedium->tymed=pDataObj->fmtetc[nIndex].tymed;
  pMedium->hGlobal=NULL;
  pMedium->pUnkForRelease=0;

  if (pFormatEtc->cfFormat != cfAkelEditColumnSel)
  {
    if (pDataObj->fmtetc[nIndex].tymed == TYMED_HGLOBAL)
    {
      PVOID pSource;
      PVOID pTarget;
      DWORD dwSourceSize;

      if (dwSourceSize=GlobalSize(pDataObj->stgmed[nIndex].hGlobal))
      {
        if (pSource=GlobalLock(pDataObj->stgmed[nIndex].hGlobal))
        {
          if (pMedium->hGlobal=GlobalAlloc(GMEM_MOVEABLE, dwSourceSize))
          {
            if (pTarget=GlobalLock(pMedium->hGlobal))
            {
              xmemcpy(pTarget, pSource, dwSourceSize);
              GlobalUnlock(pMedium->hGlobal);
            }
          }
          GlobalUnlock(pDataObj->stgmed[nIndex].hGlobal);
        }
      }
    }
    else return DV_E_FORMATETC;
  }
  return S_OK;
}

HRESULT WINAPI AEIDataObject_GetDataHere(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium)
{
  return DATA_E_FORMATETC;
}

HRESULT WINAPI AEIDataObject_QueryGetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc)
{
  AEIDataObject *pDataObj=(AEIDataObject *)lpTable;

  if (AE_DataObjectLookupFormatEtc(pDataObj, pFormatEtc) == -1)
    return DV_E_FORMATETC;

  return S_OK;
}

HRESULT WINAPI AEIDataObject_GetCanonicalFormatEtc(LPUNKNOWN lpTable, FORMATETC *pFormatEct, FORMATETC *pFormatEtcOut)
{
  pFormatEtcOut->ptd=NULL;
  return E_NOTIMPL;
}

HRESULT WINAPI AEIDataObject_SetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease)
{
  return E_NOTIMPL;
}

HRESULT WINAPI AEIDataObject_EnumFormatEtc(LPUNKNOWN lpTable, DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
  return E_NOTIMPL;
}

HRESULT WINAPI AEIDataObject_DAdvise(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT WINAPI AEIDataObject_DUnadvise(LPUNKNOWN lpTable, DWORD dwConnection)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT WINAPI AEIDataObject_EnumDAdvise(LPUNKNOWN lpTable, IEnumSTATDATA **ppEnumAdvise)
{
  return OLE_E_ADVISENOTSUPPORTED;
}

int AE_DataObjectLookupFormatEtc(AEIDataObject *pDataObj, FORMATETC *pFormatEtc)
{
  int i;

  for (i=0; i < pDataObj->nNumFormats; ++i)
  {
    if ((pFormatEtc->tymed & pDataObj->fmtetc[i].tymed) &&
        pFormatEtc->cfFormat == pDataObj->fmtetc[i].cfFormat &&
        pFormatEtc->dwAspect == pDataObj->fmtetc[i].dwAspect)
    {
      return i;
    }
  }
  return -1;
}

DWORD AE_DataObjectCopySelection(AKELEDIT *ae)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  DWORD dwAnsiLen=0;
  DWORD dwUnicodeLen=0;

  if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, AELB_ASOUTPUT, ae->bColumnSel, TRUE))
    {
      if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, dwUnicodeLen * sizeof(wchar_t)))
      {
        if (pDataTargetW=GlobalLock(hDataTargetW))
        {
          AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)pDataTargetW, (DWORD)-1, AELB_ASOUTPUT, ae->bColumnSel, TRUE);

          //Get Ansi text
          dwAnsiLen=WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, NULL, 0, NULL, NULL);

          if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, dwAnsiLen))
          {
            if (pDataTargetA=GlobalLock(hDataTargetA))
            {
              WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, (char *)pDataTargetA, dwAnsiLen, NULL, NULL);
              GlobalUnlock(hDataTargetA);
            }
          }
          GlobalUnlock(hDataTargetW);
        }
      }
    }
  }
  ae->ido.stgmed[0].hGlobal=hDataTargetW;
  ae->ido.stgmed[1].hGlobal=hDataTargetA;

  if (ae->bColumnSel)
    ae->ido.fmtetc[2].cfFormat=cfAkelEditColumnSel;
  else
    ae->ido.fmtetc[2].cfFormat=0;

  if (hDataTargetW && hDataTargetA)
    return dwUnicodeLen?dwUnicodeLen - 1:0;
  else
    return 0;
}

void AE_DataObjectFreeSelection(AKELEDIT *ae)
{
  if (ae->ido.stgmed[0].hGlobal)
  {
    GlobalFree(ae->ido.stgmed[0].hGlobal);
    ae->ido.stgmed[0].hGlobal=NULL;
  }
  if (ae->ido.stgmed[1].hGlobal)
  {
    GlobalFree(ae->ido.stgmed[1].hGlobal);
    ae->ido.stgmed[1].hGlobal=NULL;
  }
}

BOOL AE_IsEqualIID(const GUID *rguid1, const GUID *rguid2)
{
  return !xmemcmp(rguid1, rguid2, sizeof(GUID));
}

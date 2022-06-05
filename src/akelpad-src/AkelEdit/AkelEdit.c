/***********************************************************************************
 *                      AkelEdit text control v1.9.8                               *
 *                                                                                 *
 * Copyright 2007-2018 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
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
 * - Syntax highlighting.                                                          *
 * - URL detection and highlighting.                                               *
 * - Code folding.                                                                 *
 * - Regular expressions.                                                          *
 * - Splitting edit window.                                                        *
 * - Color printing.                                                               *
 * - Support of proportional and non-proportional fonts.                           *
 * - Support of Input Method Editors (IME).                                        *
 * - OLE drag'n'drop editing capabilities.                                         *
 * - Check a saving correctness of all symbols in the specified encoding.          *
 *                                                                                 *
 *  Not implemented:                                                               *
 * - No BiDi (right-to-left) support.                                              *
 ***********************************************************************************/


//// Includes

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stddef.h>
#include <imm.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "x64Func.h"
#include "AkelBuild.h"
#include "RegExpFunc.h"
#include "Resources\resource.h"
#include "Resources\version.h"

/*
//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"

//Include RegExp functions
#define RE_FUNCTIONS
#include "RegExpFunc.h"

//Include stack functions
#define StackInsertBefore
#define StackDelete
#define StackJoin
#define StackSplit
#include "StackFunc.h"

//Include string functions
#ifdef AKELEDIT_LINGUISTICCASING
  #define WideCharLower_LINGUISTICCASING
  #define WideCharUpper_LINGUISTICCASING
#endif
#define WideCharLower
#define WideCharUpper
#define xmemcpy
#define xmemcmp
#define xmemset
#define xarrlenW
#define xarrcpynW
#define xstrlenA
#define xstrlenW
#define xstrcmpW
#define xstrcmpiA
#define xstrcmpiW
#define xstrcmpnW
#define xstrcmpinW
#define xstrcpyW
#define xstrcpynA
#define xstrcpynW
#define xatoiW
#define hex2decW
#define UTF8toUTF16
#define UTF16toUTF8
#include "StrFunc.h"

//Include x64 functions
#define MultiByteToWideChar64
#define WideCharToMultiByte64
#include "x64Func.h"
//*/


//// Global variables

HANDLE hAkelEditProcessHeap=0;
int nAkelEditHeapCount=0;
AESTACKEDIT hAkelEditWindowsStack={0};
HSTACK hAkelEditFontsStackA={0};
HSTACK hAkelEditFontsStackW={0};
HSTACK hAkelEditBitmapDataStack={0};
HSTACK hAkelEditBitmapDcStack={0};
HSTACK hAkelEditPensStack={0};
AESTACKTHEME hAkelEditThemesStack={0};
ATOM nAkelEditClassAtomA=0;
ATOM nRichEditClassAtomA=0;
ATOM nAkelEditClassAtomW_Ansi=0;
ATOM nRichEditClassAtomW_Ansi=0;
ATOM nAkelEditClassAtomW=0;
ATOM nRichEditClassAtomW=0;
BOOL bAkelEditWindows9x=-1;
HCURSOR hAkelEditCursorIBeam=NULL;
HCURSOR hAkelEditCursorArrow=NULL;
HCURSOR hAkelEditCursorMargin=NULL;
HCURSOR hAkelEditCursorMarker=NULL;
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
HANDLE hAkelEditMsimg32=NULL;
BOOL bAkelEditMsimg32Free=FALSE;
BOOL (WINAPI *AkelEditAlphaBlendPtr)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION)=NULL;


//// Entry point

#ifndef AKELEDIT_STATICBUILD
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    #ifdef AKELEDIT_NOREGISTERRICHEDIT
      OleInitialize(0);
      AE_RegisterClassA(hinstDLL, FALSE, FALSE, FALSE);
      AE_RegisterClassW(hinstDLL, FALSE);
    #else
      OleInitialize(0);
      AE_RegisterClassA(hinstDLL, TRUE, FALSE, FALSE);
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

BOOL AE_RegisterClassA(HINSTANCE hInstance, BOOL bRegRichEdit, BOOL bRegAkelEditW, BOOL bRegRichEditW)
{
  if (!nAkelEditClassAtomA)
  {
    WNDCLASSA wndclass;

    AE_RegisterClassCommon(hInstance);

    //AkelEditA class
    wndclass.style        =CS_GLOBALCLASS|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditShellProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =hAkelEditCursorIBeam;
    wndclass.hbrBackground=NULL;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AES_AKELEDITA;
    if (!(nAkelEditClassAtomA=RegisterClassA(&wndclass)))
      return FALSE;

    //RichEdit20A class
    if (bRegRichEdit)
    {
      wndclass.lpszClassName=AES_RICHEDIT20A;
      if (!(nRichEditClassAtomA=RegisterClassA(&wndclass)))
        return FALSE;
    }
    //AkelEditW class
    if (bRegAkelEditW)
    {
      wndclass.lpszClassName=AES_AKELEDITW_ANSI;
      if (!(nAkelEditClassAtomW_Ansi=RegisterClassA(&wndclass)))
        return FALSE;
    }
    //RichEdit20W class
    if (bRegRichEditW)
    {
      wndclass.lpszClassName=AES_RICHEDIT20W_ANSI;
      if (!(nRichEditClassAtomW_Ansi=RegisterClassA(&wndclass)))
        return FALSE;
    }
  }
  return TRUE;
}

BOOL AE_RegisterClassW(HINSTANCE hInstance, BOOL bRegRichEdit)
{
  if (!nAkelEditClassAtomW)
  {
    WNDCLASSW wndclass;

    AE_RegisterClassCommon(hInstance);

    //AkelEditW class
    wndclass.style        =CS_GLOBALCLASS|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditShellProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =hAkelEditCursorIBeam;
    wndclass.hbrBackground=NULL;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AES_AKELEDITW;
    if (!(nAkelEditClassAtomW=RegisterClassW(&wndclass)))
      return FALSE;

    //RichEdit20W class
    if (bRegRichEdit)
    {
      wndclass.lpszClassName=AES_RICHEDIT20W;
      if (!(nRichEditClassAtomW=RegisterClassW(&wndclass)))
        return FALSE;
    }
  }
  return TRUE;
}

void AE_RegisterClassCommon(HINSTANCE hInstance)
{
  if (!hAkelEditProcessHeap) hAkelEditProcessHeap=GetProcessHeap();
  if (!cfAkelEditColumnSel) cfAkelEditColumnSel=RegisterClipboardFormatA("MSDEVColumnSelect");
  if (!cfAkelEditText) cfAkelEditText=RegisterClipboardFormatA("AkelEditText");
  if (!hAkelEditCursorIBeam) hAkelEditCursorIBeam=LoadCursorA(NULL, (char *)(UINT_PTR)IDC_IBEAM);
  if (!hAkelEditCursorArrow) hAkelEditCursorArrow=LoadCursorA(NULL, (char *)(UINT_PTR)IDC_ARROW);
  if (!hAkelEditCursorMargin) hAkelEditCursorMargin=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMARGIN);
  if (!hAkelEditCursorMarker) hAkelEditCursorMarker=LoadCursorA(NULL, (char *)(UINT_PTR)IDC_SIZEWE);
  if (!hAkelEditCursorHand) hAkelEditCursorHand=LoadCursorA(NULL, (char *)(UINT_PTR)IDC_HAND);
  if (!hAkelEditCursorHand) hAkelEditCursorHand=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEHAND);
  if (!hAkelEditCursorMCenterAll) hAkelEditCursorMCenterAll=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMCENTERALL);
  if (!hAkelEditCursorMCenterLeftRight) hAkelEditCursorMCenterLeftRight=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMCENTERLEFTRIGHT);
  if (!hAkelEditCursorMCenterTopBottom) hAkelEditCursorMCenterTopBottom=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMCENTERTOPBOTTOM);
  if (!hAkelEditCursorMLeft) hAkelEditCursorMLeft=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMLEFT);
  if (!hAkelEditCursorMLeftTop) hAkelEditCursorMLeftTop=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMLEFTTOP);
  if (!hAkelEditCursorMTop) hAkelEditCursorMTop=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMTOP);
  if (!hAkelEditCursorMRightTop) hAkelEditCursorMRightTop=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMRIGHTTOP);
  if (!hAkelEditCursorMRight) hAkelEditCursorMRight=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMRIGHT);
  if (!hAkelEditCursorMRightBottom) hAkelEditCursorMRightBottom=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMRIGHTBOTTOM);
  if (!hAkelEditCursorMBottom) hAkelEditCursorMBottom=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMBOTTOM);
  if (!hAkelEditCursorMLeftBottom) hAkelEditCursorMLeftBottom=LoadCursorA(hInstance, (char *)(UINT_PTR)IDC_AEMLEFTBOTTOM);
  if (!hAkelEditBitmapMCenterAll) hAkelEditBitmapMCenterAll=(HBITMAP)LoadImageA(hInstance, MAKEINTRESOURCEA(IDB_BITMAP_MCENTERALL), IMAGE_BITMAP, 0, 0, 0);
  if (!hAkelEditBitmapMCenterLeftRight) hAkelEditBitmapMCenterLeftRight=(HBITMAP)LoadImageA(hInstance, MAKEINTRESOURCEA(IDB_BITMAP_MCENTERLEFTRIGHT), IMAGE_BITMAP, 0, 0, 0);
  if (!hAkelEditBitmapMCenterTopBottom) hAkelEditBitmapMCenterTopBottom=(HBITMAP)LoadImageA(hInstance, MAKEINTRESOURCEA(IDB_BITMAP_MCENTERTOPBOTTOM), IMAGE_BITMAP, 0, 0, 0);

  //Is Windows 95/98/Me?
  if (bAkelEditWindows9x == -1)
  {
    OSVERSIONINFO ovi;

    ovi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
    GetVersionEx(&ovi);
    if (ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
      bAkelEditWindows9x=TRUE;
    else
      bAkelEditWindows9x=FALSE;
  }
}

BOOL AE_UnregisterClassA(HINSTANCE hInstance)
{
  BOOL bResult=TRUE;

  AE_UnregisterClassCommon(hInstance);
  AE_StackFontItemsFreeA(&hAkelEditFontsStackA);

  if (nAkelEditClassAtomA)
  {
    if (UnregisterClassA(AES_AKELEDITA, hInstance))
      nAkelEditClassAtomA=0;
    else
      bResult=FALSE;
  }
  if (nRichEditClassAtomA)
  {
    if (UnregisterClassA(AES_RICHEDIT20A, hInstance))
      nRichEditClassAtomA=0;
    else
      bResult=FALSE;
  }
  if (nAkelEditClassAtomW_Ansi)
  {
    if (UnregisterClassA(AES_AKELEDITW_ANSI, hInstance))
      nAkelEditClassAtomW_Ansi=0;
    else
      bResult=FALSE;
  }
  if (nRichEditClassAtomW_Ansi)
  {
    if (UnregisterClassA(AES_RICHEDIT20W_ANSI, hInstance))
      nRichEditClassAtomW_Ansi=0;
    else
      bResult=FALSE;
  }
  return bResult;
}

BOOL AE_UnregisterClassW(HINSTANCE hInstance)
{
  BOOL bResult=TRUE;

  AE_UnregisterClassCommon(hInstance);
  AE_StackFontItemsFreeW(&hAkelEditFontsStackW);

  if (nAkelEditClassAtomW)
  {
    if (UnregisterClassW(AES_AKELEDITW, hInstance))
      nAkelEditClassAtomW=0;
    else
      bResult=FALSE;
  }
  if (nRichEditClassAtomW)
  {
    if (UnregisterClassW(AES_RICHEDIT20W, hInstance))
      nRichEditClassAtomW=0;
    else
      bResult=FALSE;
  }
  return bResult;
}

void AE_UnregisterClassCommon(HINSTANCE hInstance)
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
  AE_StackBitmapItemsFree(&hAkelEditBitmapDataStack);
  AE_StackDcItemsFree(&hAkelEditBitmapDcStack);
  AE_StackPenItemsFree(&hAkelEditPensStack);
  AE_StackWindowFree(&hAkelEditWindowsStack);

  if (bAkelEditMsimg32Free)
  {
    bAkelEditMsimg32Free=FALSE;
    FreeLibrary(hAkelEditMsimg32);
  }
  hAkelEditMsimg32=NULL;
}

LRESULT CALLBACK AE_EditShellProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AKELEDIT *ae;
  HANDLE hThread=GetCurrentThread();
  LRESULT lResult;

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

  GetAkelEdit:
  if (ae=AE_StackWindowGet(&hAkelEditWindowsStack, hWnd))
  {
    //Move found AKELEDIT in first place. To make next AE_StackWindowGet calls faster.
    AE_StackWindowMakeFirst(&hAkelEditWindowsStack, ae);

    if (ae->hThreadWork == NULL || ae->hThreadWork == hThread)
    {
      //Assign working thread
      ae->hThreadWork=hThread;
      ++ae->nThreadCount;
    }
    else
    {
      //Other threads is waiting for signal
      if (!ae->hThreadMutex) ae->hThreadMutex=CreateEventA(NULL, FALSE, FALSE, NULL);
      WaitForSingleObject(ae->hThreadMutex, INFINITE);

      //Check that window still exist
      goto GetAkelEdit;
    }
    lResult=AE_EditProc(ae, uMsg, wParam, lParam);

    if (AE_StackWindowValid(&hAkelEditWindowsStack, ae))
    {
      if (!--ae->nThreadCount)
      {
        //No messages in working thread, send signal
        ae->hThreadWork=NULL;
        if (ae->hThreadMutex) SetEvent(ae->hThreadMutex);
      }
    }
    return lResult;
  }

  if (!IsWindowUnicode(hWnd))
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  else
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK AE_EditProc(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //// Clone window processing

  AE_CloneActivate(lpAkelEditPrev, ae);
  lpAkelEditPrev=ae;


  //// Character input: Alt + NumPad

  if (uMsg != WM_CHAR)
  {
    if (uMsg == WM_KILLFOCUS)
      ae->nAltChar=AEAC_NONE;
    else if (ae->nAltChar == AEAC_KEYUP)
      ae->nAltChar=AEAC_NONE;
  }

  switch (uMsg)
  {
    //// AEM_* AkelEdit control messages (uMsg >= WM_USER)

    //Text retrieval and modification
    case AEM_EXSETTEXTA:
    {
      AESETTEXTA *at=(AESETTEXTA *)lParam;

      return AE_SetTextAnsi(ae, at->nCodePage, at->pText, at->dwTextLen, at->nNewLine);
    }
    case AEM_EXSETTEXTW:
    {
      AESETTEXTW *at=(AESETTEXTW *)lParam;

      return AE_SetText(ae, at->pText, at->dwTextLen, at->nNewLine, FALSE);
    }
    case AEM_SETTEXTA:
    {
      return AE_SetTextAnsi(ae, CP_ACP, (char *)lParam, wParam, AELB_ASINPUT);
    }
    case AEM_SETTEXTW:
    {
      return AE_SetText(ae, (wchar_t *)lParam, wParam, AELB_ASINPUT, FALSE);
    }
    case AEM_APPENDTEXTA:
    {
      AEAPPENDTEXTA *at=(AEAPPENDTEXTA *)lParam;

      AE_AppendTextAnsi(ae, at->nCodePage, at->pText, at->dwTextLen, at->nNewLine);
      return 0;
    }
    case AEM_APPENDTEXTW:
    {
      AEAPPENDTEXTW *at=(AEAPPENDTEXTW *)lParam;

      AE_AppendText(ae, at->pText, at->dwTextLen, at->nNewLine);
      return 0;
    }
    case AEM_REPLACESELA:
    {
      AEREPLACESELA *rs=(AEREPLACESELA *)lParam;

      AE_ReplaceSelAnsi(ae, rs->nCodePage, rs->pText, rs->dwTextLen, rs->nNewLine, rs->dwFlags, rs->ciInsertStart, rs->ciInsertEnd);
      return 0;
    }
    case AEM_REPLACESELW:
    {
      AEREPLACESELW *rs=(AEREPLACESELW *)lParam;

      AE_ReplaceSel(ae, rs->pText, rs->dwTextLen, rs->nNewLine, rs->dwFlags, rs->ciInsertStart, rs->ciInsertEnd);
      return 0;
    }
    case AEM_GETTEXTRANGEA:
    {
      AETEXTRANGEA *tr=(AETEXTRANGEA *)lParam;

      return AE_GetTextRangeAnsi(ae, tr->nCodePage, tr->lpDefaultChar, tr->lpUsedDefChar, &tr->cr.ciMin, &tr->cr.ciMax, tr->pBuffer, tr->dwBufferMax, tr->nNewLine, tr->bColumnSel, tr->bFillSpaces);
    }
    case AEM_GETTEXTRANGEW:
    {
      AETEXTRANGEW *tr=(AETEXTRANGEW *)lParam;

      return AE_GetTextRange(ae, &tr->cr.ciMin, &tr->cr.ciMax, tr->pBuffer, tr->dwBufferMax, tr->nNewLine, tr->bColumnSel, tr->bFillSpaces);
    }
    case AEM_STREAMIN:
    {
      AESTREAMIN *aesi=(AESTREAMIN *)lParam;

      return AE_StreamIn(ae, (DWORD)wParam, aesi);
    }
    case AEM_STREAMOUT:
    {
      AESTREAMOUT *aeso=(AESTREAMOUT *)lParam;

      return AE_StreamOut(ae, (DWORD)wParam, aeso);
    }
    case AEM_CANPASTE:
    {
      return AE_EditCanPaste(ae);
    }
    case AEM_PASTE:
    {
      int nNewLine=(int)wParam;

      if (!nNewLine) nNewLine=AELB_ASINPUT;
      return AE_EditPasteFromClipboard(ae, (DWORD)lParam, nNewLine);
    }
    case AEM_CUT:
    {
      int nNewLine=(int)wParam;

      if (!nNewLine) nNewLine=AELB_ASOUTPUT;
      return AE_EditCut(ae, (DWORD)lParam, nNewLine);
    }
    case AEM_COPY:
    {
      AECHARRANGE cr;
      int nNewLine=(int)wParam;
      BOOL bColumnSel=ae->bColumnSel;

      if (AE_NoSelectionRange(ae, &cr, (DWORD)lParam))
        bColumnSel=FALSE;
      if (!nNewLine) nNewLine=AELB_ASOUTPUT;
      return AE_EditCopyToClipboard(ae, &cr, nNewLine, bColumnSel);
    }
    case AEM_CHECKCODEPAGE:
    {
      return AE_CheckCodepage(ae, (int)wParam, (int *)lParam);
    }
    case AEM_FINDTEXTA:
    {
      return AE_FindTextAnsi(ae, CP_ACP, (AEFINDTEXTA *)lParam);
    }
    case AEM_FINDTEXTW:
    {
      return AE_FindText(ae, (AEFINDTEXTW *)lParam);
    }
    case AEM_ISMATCHA:
    {
      return AE_IsMatchAnsi(ae, CP_ACP, (AEFINDTEXTA *)lParam, (AECHARINDEX *)wParam);
    }
    case AEM_ISMATCHW:
    {
      return AE_IsMatch(ae, (AEFINDTEXTW *)lParam, (AECHARINDEX *)wParam);
    }
    case AEM_KEYDOWN:
    {
      return AE_KeyDown(ae, (int)wParam, (BOOL)(lParam & AEMOD_ALT), (BOOL)(lParam & AEMOD_SHIFT), (BOOL)(lParam & AEMOD_CONTROL));
    }
    case AEM_INSERTCHAR:
    {
      AE_EditChar(ae, (int)wParam, TRUE);
      return 0;
    }
    case AEM_CHARAT:
    {
      AECHARINDEX *ciPos=(AECHARINDEX *)wParam;

      return AEC_CharAtIndex(ciPos);
    }
    case AEM_INPUTLANGUAGE:
    {
      return (LRESULT)GetKeyboardLayout(0);
    }
    case AEM_DRAGDROP:
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

    //Undo and Redo
    case AEM_CANUNDO:
    {
      if (ae->popt->dwOptions & AECO_READONLY)
        return FALSE;
      return AE_EditCanUndo(ae);
    }
    case AEM_CANREDO:
    {
      if (ae->popt->dwOptions & AECO_READONLY)
        return FALSE;
      return AE_EditCanRedo(ae);
    }
    case AEM_UNDO:
    {
      return AE_EditUndo(ae);
    }
    case AEM_REDO:
    {
      return AE_EditRedo(ae);
    }
    case AEM_EMPTYUNDOBUFFER:
    {
      if (lParam)
      {
        AEUNDOATTACH *hUndoAttach=(AEUNDOATTACH *)lParam;
        AEUNDOITEM *lpUndoItem;

        for (lpUndoItem=hUndoAttach->first; lpUndoItem; lpUndoItem=lpUndoItem->next)
        {
          if (lpUndoItem->wpText) AE_HeapFree(ae->aeUndo, 0, (LPVOID)lpUndoItem->wpText);
        }
        AE_HeapStackClear(ae->aeUndo, (stack **)&hUndoAttach->first, (stack **)&hUndoAttach->last);
      }
      else
      {
        if (wParam)
        {
          if (AE_EditCanRedo(ae))
            AE_StackRedoDeleteAll(ae, ae->ptxt->lpCurrentUndo);
        }
        else AE_EmptyUndoBuffer(ae);
      }
      return 0;
    }
    case AEM_STOPGROUPTYPING:
    {
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    case AEM_BEGINUNDOACTION:
    {
      ae->ptxt->bLockGroupStopExt=TRUE;
      return 0;
    }
    case AEM_ENDUNDOACTION:
    {
      ae->ptxt->bLockGroupStopExt=FALSE;
      return 0;
    }
    case AEM_LOCKCOLLECTUNDO:
    {
      ae->ptxt->bLockCollectUndo=(BOOL)wParam;
      return 0;
    }
    case AEM_GETUNDOLIMIT:
    {
      DWORD *lpdwUndoCount=(DWORD *)lParam;

      if (lpdwUndoCount)
        *lpdwUndoCount=ae->ptxt->dwUndoCount;
      return ae->ptxt->dwUndoLimit;
    }
    case AEM_SETUNDOLIMIT:
    {
      ae->ptxt->dwUndoLimit=(DWORD)wParam;
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    case AEM_GETMODIFY:
    {
      return ae->ptxt->bModified;
    }
    case AEM_SETMODIFY:
    {
      AE_SetModify(ae, (BOOL)wParam);
      return 0;
    }
    case AEM_UNDOBUFFERSIZE:
    {
      return AE_StackUndoSize(ae);
    }
    case AEM_ISRANGEMODIFIED:
    {
      return AE_StackIsRangeModified(ae, (const CHARRANGE64 *)lParam);
    }
    case AEM_DETACHUNDO:
    {
      return (LRESULT)AE_StackUndoDetach(ae);
    }
    case AEM_ATTACHUNDO:
    {
      return AE_StackUndoAttach(ae, (AEUNDOATTACH *)lParam);
    }
    case AEM_GETUNDOPOS:
    {
      AEUNDOITEM **lppUndoItem=(AEUNDOITEM **)lParam;
      AEUNDOITEM *lpUndoItem=*lppUndoItem;
      AEUNDOITEM *lpCurrentUndo=ae->ptxt->lpCurrentUndo;
      INT_PTR nOffset;
      int nMoveId=0;

      if (wParam & AEGUP_NEXT)
      {
        if (!lpUndoItem && lpCurrentUndo == ae->ptxt->hUndoStack.last)
          return -1;
        if (!lpCurrentUndo)
        {
          lpCurrentUndo=ae->ptxt->hUndoStack.first;
          ++nMoveId;
        }
        else if (lpCurrentUndo->next)
        {
          lpCurrentUndo=lpCurrentUndo->next;
          ++nMoveId;
        }
        if (!lpUndoItem)
          lpUndoItem=lpCurrentUndo;
        else
          lpUndoItem=lpUndoItem->next;
      }
      else if (wParam & AEGUP_PREV)
      {
        if (!lpUndoItem && !lpCurrentUndo)
          return -1;
        if (!lpCurrentUndo)
          lpCurrentUndo=ae->ptxt->hUndoStack.first;
        if (!lpUndoItem)
          lpUndoItem=lpCurrentUndo;
        else
          lpUndoItem=lpUndoItem->prev;
      }
      else if (wParam & AEGUP_FIRST)
      {
        *lppUndoItem=ae->ptxt->hUndoStack.first;
        return -1;
      }
      else if (wParam & AEGUP_LAST)
      {
        *lppUndoItem=ae->ptxt->hUndoStack.last;
        return -1;
      }
      else if (wParam & AEGUP_SAVED)
      {
        *lppUndoItem=ae->ptxt->lpSavePoint;
        return -1;
      }
      else
      {
        //AEGUP_CURRENT
        if (!lpCurrentUndo)
          lpCurrentUndo=ae->ptxt->hUndoStack.first;
        if (!lpUndoItem)
          lpUndoItem=lpCurrentUndo;
      }
      if (!lpUndoItem || !lpCurrentUndo) return -1;
      if (((wParam & AEGUP_NOUNDO) && lpCurrentUndo->nItemId - nMoveId > lpUndoItem->nItemId) ||
          ((wParam & AEGUP_NOREDO) && lpCurrentUndo->nItemId - nMoveId < lpUndoItem->nItemId))
        return -1;

      if ((nOffset=AE_StackGetUndoPos(ae, lpCurrentUndo, lpUndoItem)) == -1)
        *lppUndoItem=NULL;
      else
        *lppUndoItem=lpUndoItem;
      return nOffset;
    }

    //Text coordinates
    case AEM_EXGETSEL:
    {
      AECHARINDEX *ciMin=(AECHARINDEX *)wParam;
      AECHARINDEX *ciMax=(AECHARINDEX *)lParam;

      if (ciMin) *ciMin=ae->ciSelStartIndex;
      if (ciMax) *ciMax=ae->ciSelEndIndex;

      if (!AEC_IndexCompareEx(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        return FALSE;
      return TRUE;
    }
    case AEM_EXSETSEL:
    {
      AE_SetSelectionPos(ae, (AECHARINDEX *)lParam, (AECHARINDEX *)wParam, FALSE, 0, AESCT_SETSELMESSAGE);
      return 0;
    }
    case AEM_GETSEL:
    {
      AECHARINDEX *lpciCaret=(AECHARINDEX *)wParam;
      AESELECTION *aes=(AESELECTION *)lParam;

      return AE_AkelEditGetSel(ae, aes, lpciCaret);
    }
    case AEM_SETSEL:
    {
      AECHARINDEX *lpciCaret=(AECHARINDEX *)wParam;
      AESELECTION *aes=(AESELECTION *)lParam;

      if (aes->dwFlags & AESELT_INDEXUPDATE)
      {
        aes->crSel.ciMin.lpLine=AE_GetLineData(ae, aes->crSel.ciMin.nLine);
        if (aes->crSel.ciMin.nLine == aes->crSel.ciMax.nLine)
          aes->crSel.ciMax.lpLine=aes->crSel.ciMin.lpLine;
        else
          aes->crSel.ciMax.lpLine=AE_GetLineData(ae, aes->crSel.ciMax.nLine);
        if (lpciCaret)
        {
          if (aes->crSel.ciMin.nLine == lpciCaret->nLine)
            lpciCaret->lpLine=aes->crSel.ciMin.lpLine;
          else if (aes->crSel.ciMax.nLine == lpciCaret->nLine)
            lpciCaret->lpLine=aes->crSel.ciMax.lpLine;
        }
      }
      AE_AkelEditSetSel(ae, aes, lpciCaret);
      return 0;
    }
    case AEM_GETCOLUMNSEL:
    {
      return ae->bColumnSel;
    }
    case AEM_UPDATESEL:
    {
      AE_UpdateSelection(ae, (DWORD)wParam);
      return 0;
    }
    case AEM_GETLINENUMBER:
    {
      return AE_GetLineNumber(ae, (int)wParam, lParam);
    }
    case AEM_GETINDEX:
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return (LRESULT)AE_GetIndex(ae, (int)wParam, ciCharIndex, ciCharIndex);
    }
    case AEM_GETLINEINDEX:
    {
      AELINEDATA *lpLine;
      int nLine=(int)wParam;
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
    case AEM_INDEXUPDATE:
    {
      return AE_IndexUpdate(ae, (AECHARINDEX *)lParam);
    }
    case AEM_INDEXCOMPARE:
    {
      return AEC_IndexCompare((AECHARINDEX *)wParam, (AECHARINDEX *)lParam);
    }
    case AEM_INDEXSUBTRACT:
    {
      AEINDEXSUBTRACT *aeis=(AEINDEXSUBTRACT *)lParam;

      return AE_IndexSubtract(ae, aeis->ciChar1, aeis->ciChar2, aeis->nNewLine, aeis->bColumnSel, TRUE);
    }
    case AEM_INDEXOFFSET:
    {
      AEINDEXOFFSET *aeio=(AEINDEXOFFSET *)lParam;

      return AE_IndexOffset(ae, aeio->ciCharIn, aeio->ciCharOut, aeio->nOffset, aeio->nNewLine);
    }
    case AEM_INDEXTORICHOFFSET:
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      if (ciCharIndex && ciCharIndex->lpLine)
        return AE_AkelIndexToRichOffset(ae, ciCharIndex);
      return -1;
    }
    case AEM_RICHOFFSETTOINDEX:
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      AE_RichOffsetToAkelIndex(ae, wParam, ciCharIndex);
      return 0;
    }
    case AEM_GETRICHOFFSET:
    {
      return AE_GetRichOffset(ae, (int)wParam, lParam);
    }
    case AEM_GETWRAPLINE:
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetWrapLine(ae, (int)wParam, ciCharIndex);
    }
    case AEM_GETUNWRAPLINE:
    {
      return AE_GetUnwrapLine(ae, (int)wParam);
    }
    case AEM_GETNEXTBREAK:
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetNextBreak(ae, ciCharIndex, ciCharIndex, FALSE, wParam?(DWORD)wParam:ae->popt->dwWordBreak);
    }
    case AEM_GETPREVBREAK:
    {
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetPrevBreak(ae, ciCharIndex, ciCharIndex, FALSE, wParam?(DWORD)wParam:ae->popt->dwWordBreak);
    }
    case AEM_ISDELIMITER:
    {
      return AE_IsDelimiter(ae, (AECHARINDEX *)lParam, (DWORD)wParam);
    }
    case AEM_INDEXTOCOLUMN:
    {
      AECHARINDEX *lpChar=(AECHARINDEX *)lParam;
      AECHARINDEX ciChar;
      AECHARINDEX ciCount;
      int nTabStop=LOWORD(wParam);
      WORD wFlags=HIWORD(wParam);
      int nColumn=0;

      //Tab current size if zero
      if (nTabStop == 0) nTabStop=ae->ptxt->nTabStop;

      ciChar=*lpChar;
      if (ciChar.nCharInLine > ciChar.lpLine->nLineLen)
        ciChar.nCharInLine=ciChar.lpLine->nLineLen;

      if (!(wFlags & AECTI_WRAPLINEBEGIN))
      {
        ciCount.nLine=ciChar.nLine;
        ciCount.lpLine=ciChar.lpLine;
        ciCount.nCharInLine=0;
      }
      else AEC_WrapLineBeginEx(&ciChar, &ciCount);

      while (AEC_IndexCompare(&ciCount, &ciChar) < 0)
      {
        if (ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\t')
          nColumn+=nTabStop - nColumn % nTabStop;
        else
          nColumn+=1;

        AEC_NextChar(&ciCount);
      }
      nColumn+=lpChar->nCharInLine - ciChar.nCharInLine;

      return nColumn;
    }
    case AEM_COLUMNTOINDEX:
    {
      AECHARINDEX *lpChar=(AECHARINDEX *)lParam;
      int nTabStop=LOWORD(wParam);
      WORD wFlags=HIWORD(wParam);
      int nColumn=0;
      int nColumnEnd=lpChar->nCharInLine;

      lpChar->nCharInLine=0;
      if (wFlags & AECTI_WRAPLINEBEGIN)
        AEC_WrapLineBegin(lpChar);

      //Tab current size if zero
      if (nTabStop == 0) nTabStop=ae->ptxt->nTabStop;

      while (nColumn < nColumnEnd)
      {
        if (lpChar->lpLine->wpLine[lpChar->nCharInLine] == L'\t')
          nColumn+=nTabStop - nColumn % nTabStop;
        else
          ++nColumn;

        //Next char in line
        if (lpChar->nCharInLine >= lpChar->lpLine->nLineLen)
          if (lpChar->lpLine->nLineBreak != AELB_WRAP)
            return FALSE;
        AEC_NextChar(lpChar);
      }
      if (wFlags & AECTI_FIT)
      {
        if (nColumn > nColumnEnd)
          AEC_PrevChar(lpChar);
      }
      return TRUE;
    }
    case AEM_INDEXINURL:
    {
      AECHARINDEX *ciChar=(AECHARINDEX *)wParam;
      AECHARRANGE *crRange=(AECHARRANGE *)lParam;

      xmemset(crRange, 0, sizeof(AECHARRANGE));
      return AE_HighlightFindUrl(ae, ciChar, AEHF_FINDFIRSTCHAR, ae->ptxt->nLineCount, crRange);
    }
    case AEM_URLVISIT:
    {
      if (wParam == AEUV_STACK)
      {
        return (LRESULT)&ae->ptxt->hUrlVisitStack;
      }
      else if (wParam == AEUV_GETBYRANGE)
      {
        return (LRESULT)AE_UrlVisitGetByRange(ae, (AECHARRANGE *)lParam);
      }
      else if (wParam == AEUV_GETBYTEXT)
      {
        return (LRESULT)AE_UrlVisitGetByText(ae, (const wchar_t *)lParam);
      }
      else if (wParam == AEUV_ADD)
      {
        AEURLVISIT *lpUrlVisit;

        if (!(lpUrlVisit=AE_UrlVisitGetByRange(ae, (AECHARRANGE *)lParam)))
          lpUrlVisit=AE_UrlVisitInsert(ae, (AECHARRANGE *)lParam);
        if (lpUrlVisit)
          ++lpUrlVisit->nVisitCount;
        return (LRESULT)lpUrlVisit;
      }
      else if (wParam == AEUV_DEL)
      {
        AE_UrlVisitDelete(ae, (AEURLVISIT *)lParam);
        return 0;
      }
      else if (wParam == AEUV_FREE)
      {
        AE_UrlVisitFree(ae);
        return 0;
      }
      return 0;
    }
    case AEM_ADDPOINT:
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
        lpTargetPoint->dwFlags=lpSourcePoint->dwFlags;
        lpTargetPoint->dwUserData=lpSourcePoint->dwUserData;
      }
      return (LRESULT)lpTargetPoint;
    }
    case AEM_DELPOINT:
    {
      AEPOINT *lpPoint=(AEPOINT *)wParam;

      AE_StackPointDelete(ae, lpPoint);
      return 0;
    }
    case AEM_GETPOINTSTACK:
    {
      return (LRESULT)&ae->ptxt->hPointsStack;
    }

    //Screen coordinates
    case AEM_CHARFROMGLOBALPOS:
    {
      POINT64 *ptGlobal=(POINT64 *)wParam;
      POINT64 ptClient64;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      AE_GlobalToClient(ae, ptGlobal, &ptClient64, NULL);
      return AE_GetCharFromPos(ae, ptClient64.x, ptClient64.y, ciCharIndex, ptGlobal, ae->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
    }
    case AEM_GLOBALPOSFROMCHAR:
    {
      POINT64 *ptGlobal=(POINT64 *)wParam;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetPosFromChar(ae, ciCharIndex, ptGlobal, NULL);
    }
    case AEM_CHARFROMPOS:
    {
      POINT64 ptGlobal;
      POINT *ptClient=(POINT *)wParam;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;
      BOOL bResult;

      if (bResult=AE_GetCharFromPos(ae, ptClient->x, ptClient->y, ciCharIndex, &ptGlobal, ae->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE)))
        AE_GlobalToClient(ae, &ptGlobal, NULL, ptClient);
      return bResult;
    }
    case AEM_POSFROMCHAR:
    {
      POINT *ptClient=(POINT *)wParam;
      AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

      return AE_GetPosFromChar(ae, ciCharIndex, NULL, ptClient);
    }
    case AEM_GETRECT:
    {
      RECT *lprcDraw=(RECT *)lParam;
      RECT rcDraw;

      if (wParam & AERC_MARGINS)
      {
        rcDraw.left=ae->rcDraw.left - ae->rcEdit.left;
        rcDraw.top=ae->rcDraw.top - ae->rcEdit.top;
        rcDraw.right=ae->rcEdit.right - ae->rcDraw.right;
        rcDraw.bottom=ae->rcEdit.bottom - ae->rcDraw.bottom;
      }
      else rcDraw=ae->rcDraw;

      if (!(wParam & AERC_NOLEFT))
        lprcDraw->left=rcDraw.left;
      if (!(wParam & AERC_NOTOP))
        lprcDraw->top=rcDraw.top;
      if (!(wParam & AERC_NORIGHT))
        lprcDraw->right=rcDraw.right;
      if (!(wParam & AERC_NOBOTTOM))
        lprcDraw->bottom=rcDraw.bottom;

      return 0;
    }
    case AEM_SETRECT:
    {
      RECT *lprcDraw=(RECT *)lParam;
      RECT rcDraw;

      if (lprcDraw)
      {
        if (wParam & AERC_MARGINS)
        {
          rcDraw.left=ae->rcEdit.left + lprcDraw->left;
          rcDraw.top=ae->rcEdit.top + lprcDraw->top;
          rcDraw.right=ae->rcEdit.right -  lprcDraw->right;
          rcDraw.bottom=ae->rcEdit.bottom - lprcDraw->bottom;
        }
        else rcDraw=*lprcDraw;

        if (wParam & AERC_NOLEFT)
          rcDraw.left=ae->rcDraw.left;
        if (wParam & AERC_NOTOP)
          rcDraw.top=ae->rcDraw.top;
        if (wParam & AERC_NORIGHT)
          rcDraw.right=ae->rcDraw.right;
        if (wParam & AERC_NOBOTTOM)
          rcDraw.bottom=ae->rcDraw.bottom;
        lprcDraw=&rcDraw;
      }
      AE_SetDrawRect(ae, lprcDraw, (BOOL)(wParam & AERC_UPDATE));
      if (ae->ptxt->dwWordWrap)
      {
        AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        AE_StackCloneUpdate(ae);
      }
      AE_UpdateScrollBars(ae, SB_BOTH);
      AE_NotifySetRect(ae);
      return 0;
    }
    case AEM_GETSCROLLPOS:
    {
      POINT64 *ptMax=(POINT64 *)wParam;
      POINT64 *ptPos=(POINT64 *)lParam;

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
    case AEM_SETSCROLLPOS:
    {
      POINT64 *pt=(POINT64 *)lParam;

      AE_SetScrollPos(ae, (pt->x == -1?ae->nHScrollPos:pt->x), (pt->y == -1?ae->nVScrollPos:pt->y));
      return 0;
    }
    case AEM_SCROLL:
    {
      INT_PTR nResult=0;

      if (wParam & AESB_HORZ)
      {
        nResult=AE_HScroll(ae, (int)lParam, (DWORD)wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nAveCharWidth;
      }
      else if (wParam & AESB_VERT)
      {
        nResult=AE_VScroll(ae, (int)lParam, (DWORD)wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nCharHeight;
      }
      return nResult;
    }
    case AEM_LINESCROLL:
    {
      INT_PTR nResult=0;

      if (wParam & AESB_HORZ)
      {
        nResult=AE_HScrollChar(ae, (int)lParam, (DWORD)wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nAveCharWidth;
      }
      else if (wParam & AESB_VERT)
      {
        nResult=AE_VScrollLine(ae, (int)lParam, (DWORD)wParam);

        if (wParam & AESB_RETURNUNITS)
          nResult=nResult / ae->ptxt->nCharHeight;
      }
      return nResult;
    }
    case AEM_SCROLLTOPOINT:
    {
      AESCROLLTOPOINT *stp=(AESCROLLTOPOINT *)lParam;

      if (!stp)
        return AE_ScrollToPoint(ae, NULL);
      return AE_ScrollToPointEx(ae, stp->dwFlags, &stp->ptPos, stp->nOffsetX, stp->nOffsetY);
    }
    case AEM_LOCKSCROLL:
    {
      int nResult=-1;

      if (ae->popt->nHScrollLock && ae->popt->nVScrollLock)
        nResult=SB_BOTH;
      else if (ae->popt->nHScrollLock)
        nResult=SB_HORZ;
      else if (ae->popt->nVScrollLock)
        nResult=SB_VERT;

      if (wParam == SB_BOTH || wParam == SB_HORZ)
      {
        if ((BOOL)lParam)
          ++ae->popt->nHScrollLock;
        else if (ae->popt->nHScrollLock)
          --ae->popt->nHScrollLock;
      }
      if (wParam == SB_BOTH || wParam == SB_VERT)
      {
        if ((BOOL)lParam)
          ++ae->popt->nVScrollLock;
        else if (ae->popt->nVScrollLock)
          --ae->popt->nVScrollLock;
      }
      return nResult;
    }
    case AEM_GETCHARSIZE:
    {
      if (wParam == AECS_HEIGHT)
        return ae->ptxt->nCharHeight;
      if (wParam == AECS_AVEWIDTH)
        return ae->ptxt->nAveCharWidth;
      if (wParam == AECS_INDEXWIDTH)
      {
        AECHARINDEX *ciChar=(AECHARINDEX *)lParam;
        POINT64 ptGlobal;

        if (ciChar->nCharInLine < ciChar->lpLine->nLineLen)
        {
          if (ciChar->lpLine->wpLine[ciChar->nCharInLine] == L'\t')
          {
            AE_GetPosFromChar(ae, ciChar, &ptGlobal, NULL);
            return AE_GetCharWidth(ae, ciChar->lpLine->wpLine + ciChar->nCharInLine, ptGlobal.x);
          }
          return AE_GetCharWidth(ae, ciChar->lpLine->wpLine + ciChar->nCharInLine, 0);
        }
        return 0;
      }
      if (wParam == AECS_POINTSIZE)
        return ae->ptxt->nPointSize;
      if (wParam == AECS_SPACEWIDTH)
        return ae->ptxt->nSpaceCharWidth;
      if (wParam == AECS_TABWIDTH)
        return ae->ptxt->nTabWidth;
      if (wParam == AECS_MAXWIDTH)
        return ae->ptxt->nMaxCharWidth;
      if (wParam == AECS_FIXEDCHARWIDTH)
        return ae->ptxt->nInitFixedCharWidth;
      if (wParam == AECS_FIXEDTABWIDTH)
        return ae->ptxt->nFixedTabWidth;
      return 0;
    }
    case AEM_GETSTRWIDTH:
    {
      return AE_GetStringWidth(ae, (wchar_t *)wParam, (int)lParam, 0);
    }
    case AEM_GETCARETPOS:
    {
      POINT *ptClient=(POINT *)wParam;
      POINT64 *ptGlobal=(POINT64 *)lParam;

      if (ptClient)
        AE_GlobalToClient(ae, &ae->ptCaret, NULL, ptClient);
      if (ptGlobal)
        *ptGlobal=ae->ptCaret;
      return ae->bCaretVisible;
    }
    case AEM_GETCARETHORZINDENT:
    {
      return ae->nCaretHorzIndent;
    }
    case AEM_SETCARETHORZINDENT:
    {
      ae->nCaretHorzIndent=(int)wParam;
      return 0;
    }
    case AEM_CONVERTPOINT:
    {
      POINT64 *pt=(POINT64 *)lParam;

      if (wParam == AECPT_GLOBALTOCLIENT)
      {
        AE_GlobalToClient(ae, pt, pt, NULL);
      }
      if (wParam == AECPT_CLIENTTOGLOBAL)
      {
        AE_ClientToGlobal(ae, pt->x, pt->y, pt);
      }
      return 0;
    }
    case AEM_POINTONMARGIN:
    {
      POINT *pt=(POINT *)wParam;

      return AE_IsPointOnMargin(ae, pt->x, pt->y);
    }
    case AEM_POINTONSELECTION:
    {
      POINT *pt=(POINT *)wParam;

      return AE_IsPointOnSelection(ae, pt->x, pt->y);
    }
    case AEM_POINTONURL:
    {
      POINT *pt=(POINT *)wParam;
      AECHARRANGE *lpcrLink=(AECHARRANGE *)lParam;
      AECHARRANGE crLink;
      DWORD dwResult;

      dwResult=AE_IsPointOnUrl(ae, pt->x, pt->y, &crLink);
      if (lpcrLink) xmemcpy(lpcrLink, &crLink, sizeof(AECHARRANGE));
      return dwResult;
    }
    case AEM_LINEFROMVPOS:
    {
      if (wParam == AECT_GLOBAL)
        return AE_LineFromVPos(ae, lParam);
      if (wParam == AECT_CLIENT)
        return AE_LineFromVPos(ae, ae->nVScrollPos + (lParam - ae->rcDraw.top));
    }
    case AEM_VPOSFROMLINE:
    {
      if (wParam == AECT_GLOBAL)
        return AE_VPosFromLine(ae, (int)lParam);
      if (wParam == AECT_CLIENT)
        return ae->rcDraw.top + (AE_VPosFromLine(ae, (int)lParam) - ae->nVScrollPos);
    }
    case AEM_GETMOUSESTATE:
    {
      if (wParam == AEMS_CAPTURE)
        return ae->dwMouseCapture;
      if (wParam == AEMS_SELECTION)
        return ae->dwMouseSelType;
      return 0;
    }

    //Options
    case AEM_CONTROLCLASS:
    {
      if (ae->bRichEditClass)
        return AECLASS_RICHEDIT;
      else
        return AECLASS_AKELEDIT;
    }
    case AEM_CONTROLVERSION:
    {
      DWORD ver[4]={RC_AKELEDITID};

      return MAKE_IDENTIFIER(ver[0], ver[1], ver[2], ver[3]);
    }
    case AEM_GETEVENTMASK:
    {
      return ae->popt->dwEventMask;
    }
    case AEM_SETEVENTMASK:
    {
      DWORD dwPrevMask=ae->popt->dwEventMask;

      ae->popt->dwEventMask=(DWORD)lParam;
      return dwPrevMask;
    }
    case AEM_GETOPTIONS:
    {
      return ae->popt->dwOptions;
    }
    case AEM_SETOPTIONS:
    {
      DWORD dwOptionsOld=ae->popt->dwOptions;
      DWORD dwOptionsNew=ae->popt->dwOptions;

      if (wParam == AECOOP_SET)
        dwOptionsNew=(DWORD)lParam;
      else if (wParam == AECOOP_OR)
        dwOptionsNew|=(DWORD)lParam;
      else if (wParam == AECOOP_AND)
        dwOptionsNew&=(DWORD)lParam;
      else if (wParam == AECOOP_XOR)
        dwOptionsNew&=~(DWORD)lParam;

      if (!(dwOptionsOld & AECO_ACTIVECOLUMN) && (dwOptionsNew & AECO_ACTIVECOLUMN))
      {
        AEPENITEM *lpPenItem;

        if (!(lpPenItem=AE_StackPenItemGet(&hAkelEditPensStack, ae->popt->aec.crActiveColumn, ae->popt->aec.crBasicBk)))
          lpPenItem=AE_StackPenItemInsert(&hAkelEditPensStack, ae->popt->aec.crActiveColumn, ae->popt->aec.crBasicBk);
        ae->popt->hActiveColumnPen=lpPenItem->hPen;

        ae->ptActiveColumnDraw.x=-0x7fffffff;
        ae->ptActiveColumnDraw.y=-0x7fffffff;
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      else if ((dwOptionsOld & AECO_ACTIVECOLUMN) && !(dwOptionsNew & AECO_ACTIVECOLUMN))
      {
        //Erase active column
        AE_ActiveColumnDraw(ae, ae->hDC, ae->rcDraw.top, ae->rcDraw.bottom);
        AE_StackCloneUpdate(ae);
      }
      if ((dwOptionsOld & AECO_CARETOUTEDGE) && !(dwOptionsNew & AECO_CARETOUTEDGE))
      {
        AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL);
      }
      if ((!(dwOptionsOld & AECO_ACTIVELINE) != !(dwOptionsNew & AECO_ACTIVELINE)) ||
          (!(dwOptionsOld & AECO_ACTIVELINEBORDER) != !(dwOptionsNew & AECO_ACTIVELINEBORDER)) ||
          (!(dwOptionsOld & AECO_ALTLINEBORDER) != !(dwOptionsNew & AECO_ALTLINEBORDER)) ||
          (!(dwOptionsOld & AECO_PAINTGROUP) != !(dwOptionsNew & AECO_PAINTGROUP)))
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }

      ae->popt->dwOptions=dwOptionsNew;
      return ae->popt->dwOptions;
    }
    case AEM_GETNEWLINE:
    {
      return MAKELONG(ae->popt->nInputNewLine, ae->popt->nOutputNewLine);
    }
    case AEM_SETNEWLINE:
    {
      if (wParam & AENL_INPUT)
        ae->popt->nInputNewLine=LOWORD(lParam);
      if (wParam & AENL_OUTPUT)
        ae->popt->nOutputNewLine=HIWORD(lParam);
      return 0;
    }
    case AEM_GETCOLORS:
    {
      AE_GetColors(ae, (AECOLORS *)lParam);
      return 0;
    }
    case AEM_SETCOLORS:
    {
      AE_SetColors(ae, (AECOLORS *)lParam, TRUE);
      return 0;
    }
    case AEM_EXGETOPTIONS:
    {
      return ae->popt->dwOptionsEx;
    }
    case AEM_EXSETOPTIONS:
    {
      DWORD dwOptionsExOld=ae->popt->dwOptionsEx;
      DWORD dwOptionsExNew=ae->popt->dwOptionsEx;

      if (wParam == AECOOP_SET)
        dwOptionsExNew=(DWORD)lParam;
      else if (wParam == AECOOP_OR)
        dwOptionsExNew|=(DWORD)lParam;
      else if (wParam == AECOOP_AND)
        dwOptionsExNew&=(DWORD)lParam;
      else if (wParam == AECOOP_XOR)
        dwOptionsExNew&=~(DWORD)lParam;
      ae->popt->dwOptionsEx=dwOptionsExNew;

      if (!(dwOptionsExOld & AECOE_OVERTYPE) != !(dwOptionsExNew & AECOE_OVERTYPE))
      {
        AE_UpdateCaret(ae, ae->bFocus);
      }
      if (!(dwOptionsExOld & AECOE_DETECTURL) != !(dwOptionsExNew & AECOE_DETECTURL))
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      return ae->popt->dwOptionsEx;
    }
    case AEM_GETCARETWIDTH:
    {
      POINT *pt=(POINT *)lParam;

      pt->x=ae->popt->nCaretInsertWidth;
      pt->y=ae->popt->nCaretOvertypeHeight;
      return 0;
    }
    case AEM_SETCARETWIDTH:
    {
      POINT *pt=(POINT *)lParam;

      ae->popt->nCaretInsertWidth=pt->x;
      ae->popt->nCaretOvertypeHeight=pt->y;
      AE_UpdateCaret(ae, ae->bFocus);
      return 0;
    }
    case AEM_GETTABSTOP:
    {
      return ae->ptxt->nTabStop;
    }
    case AEM_SETTABSTOP:
    {
      int nTabStop=(int)wParam;

      //Tab default size if zero
      if (nTabStop <= 0) nTabStop=AETAB_DEFAULTSIZE;

      if (ae->ptxt->nTabStop != nTabStop)
      {
        ae->ptxt->nTabStop=nTabStop;
        ae->ptxt->nTabWidth=ae->ptxt->nSpaceCharWidth * ae->ptxt->nTabStop;
        if (ae->ptxt->nFixedCharWidth)
          ae->ptxt->nFixedTabWidth=ae->ptxt->nFixedCharWidth * ae->ptxt->nTabStop;

        AE_CalcLinesWidth(ae, NULL, NULL, AECLW_FRESH);
        ae->ptCaret.x=0;
        ae->ptCaret.y=0;
        AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL);
        AE_UpdateCaret(ae, ae->bFocus);

        if (ae->ptxt->dwWordWrap) AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      return 0;
    }
    case AEM_GETWORDWRAP:
    {
      DWORD *lpdwWrapLimit=(DWORD *)lParam;

      if (lpdwWrapLimit) *lpdwWrapLimit=ae->ptxt->dwWrapLimit;

      return ae->ptxt->dwWordWrap;
    }
    case AEM_SETWORDWRAP:
    {
      DWORD dwWordWrap=(DWORD)wParam;
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
          ae->ptxt->dwWrapLimit=(DWORD)lParam;
          bUpdateWrap=TRUE;
        }
      }

      if (bUpdateWrap)
      {
        AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      AE_NotifyChanged(ae); //AETCT_WRAP

      return 0;
    }
    case AEM_GETWORDDELIMITERS:
    {
      return xarrcpynW((wchar_t *)lParam, ae->popt->wszWordDelimiters, wParam);
    }
    case AEM_SETWORDDELIMITERS:
    {
      ae->popt->nWordDelimitersLen=(int)xarrcpynW(ae->popt->wszWordDelimiters, lParam?(wchar_t *)lParam:AES_WORDDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
      return 0;
    }
    case AEM_GETWRAPDELIMITERS:
    {
      return xarrcpynW((wchar_t *)lParam, ae->ptxt->wszWrapDelimiters, wParam);
    }
    case AEM_SETWRAPDELIMITERS:
    {
      ae->ptxt->nWrapDelimitersLen=(int)xarrcpynW(ae->ptxt->wszWrapDelimiters, lParam?(wchar_t *)lParam:AES_WRAPDELIMITERSW, AEMAX_DELIMLENGTH) - 2;

      if (ae->ptxt->dwWordWrap & AEWW_WORD)
      {
        AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      return 0;
    }
    case AEM_GETURLLEFTDELIMITERS:
    {
      return xarrcpynW((wchar_t *)lParam, ae->popt->wszUrlLeftDelimiters, wParam);
    }
    case AEM_SETURLLEFTDELIMITERS:
    {
      ae->popt->nUrlLeftDelimitersLen=(int)xarrcpynW(ae->popt->wszUrlLeftDelimiters, lParam?(wchar_t *)lParam:AES_URLLEFTDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackCloneUpdate(ae);
      return 0;
    }
    case AEM_GETURLRIGHTDELIMITERS:
    {
      return xarrcpynW((wchar_t *)lParam, ae->popt->wszUrlRightDelimiters, wParam);
    }
    case AEM_SETURLRIGHTDELIMITERS:
    {
      ae->popt->nUrlRightDelimitersLen=(int)xarrcpynW(ae->popt->wszUrlRightDelimiters, lParam?(wchar_t *)lParam:AES_URLRIGHTDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackCloneUpdate(ae);
      return 0;
    }
    case AEM_GETURLPREFIXES:
    {
      return xarrcpynW((wchar_t *)lParam, ae->popt->wszUrlPrefixes, wParam);
    }
    case AEM_SETURLPREFIXES:
    {
      int nPrefix;

      ae->popt->nUrlPrefixesLen=(int)xarrcpynW(ae->popt->wszUrlPrefixes, lParam?(wchar_t *)lParam:AES_URLPREFIXESW, AEMAX_DELIMLENGTH);
      nPrefix=AE_GetUrlPrefixes(ae);
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      AE_StackCloneUpdate(ae);
      return nPrefix;
    }
    case AEM_GETURLMAXLENGTH:
    {
      return ae->popt->dwUrlMaxLength;
    }
    case AEM_SETURLMAXLENGTH:
    {
      ae->popt->dwUrlMaxLength=(DWORD)wParam;
      return 0;
    }
    case AEM_GETWORDBREAK:
    {
      return ae->popt->dwWordBreak;
    }
    case AEM_SETWORDBREAK:
    {
      DWORD dwWordBreak=ae->popt->dwWordBreak;

      ae->popt->dwWordBreak=(DWORD)wParam;
      return dwWordBreak;
    }
    case AEM_GETMARKER:
    {
      DWORD *lpdwColumnMarkerType=(DWORD *)wParam;

      if (lpdwColumnMarkerType) *lpdwColumnMarkerType=ae->popt->dwColumnMarkerType;

      return (LRESULT)ae->popt->dwColumnMarkerPos;
    }
    case AEM_SETMARKER:
    {
      AEPENITEM *lpPenItem;

      if (!(lpPenItem=AE_StackPenItemGet(&hAkelEditPensStack, ae->popt->aec.crColumnMarker, (COLORREF)-1)))
        lpPenItem=AE_StackPenItemInsert(&hAkelEditPensStack, ae->popt->aec.crColumnMarker, (COLORREF)-1);
      ae->popt->hColumnMarkerPen=lpPenItem->hPen;

      return AE_ColumnMarkerSet(ae, (DWORD)wParam, (int)lParam, FALSE);
    }
    case AEM_GETLINEGAP:
    {
      return (LRESULT)ae->ptxt->nLineGap;
    }
    case AEM_SETLINEGAP:
    {
      if (ae->ptxt->nLineGap != (int)wParam)
      {
        //if ((int)wParam >= 0)
        {
          POINT64 ptFirstVisLine;
          int nFirstVisibleLine=0;

          if (!ae->popt->nVScrollLock)
            nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
          ae->ptxt->nCharHeight=(ae->ptxt->nCharHeight - ae->ptxt->nLineGap) + (int)wParam;
          ae->ptxt->nCharHeight=max(ae->ptxt->nCharHeight, 1);
          ae->ptxt->nLineGap=(int)wParam;

          ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
          AE_UpdateScrollBars(ae, SB_VERT);
          ae->ptCaret.x=0;
          ae->ptCaret.y=0;
          AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL);
          if (!ae->popt->nVScrollLock)
          {
            ptFirstVisLine.y=AE_VPosFromLine(ae, nFirstVisibleLine);
            AE_ScrollToPointEx(ae, AESC_POINTGLOBAL|AESC_OFFSETPIXELY|AESC_FORCETOP, &ptFirstVisLine, 0, 0);
          }
          AE_UpdateCaret(ae, ae->bFocus);

          InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
          AE_StackCloneUpdate(ae);
        }
      }
      return 0;
    }
    case AEM_GETTEXTLIMIT:
    {
      return ae->ptxt->dwTextLimit;
    }
    case AEM_SETTEXTLIMIT:
    {
      ae->ptxt->dwTextLimit=wParam;
      return 0;
    }
    case AEM_GETFONT:
    {
      if (wParam == AEGF_CURRENT)
        return (LRESULT)ae->ptxt->hFont;
      if (wParam == AEGF_NORMAL)
        return (LRESULT)ae->ptxt->hFontNormal;
      if (wParam == AEGF_BOLD)
        return (LRESULT)ae->ptxt->hFontBold;
      if (wParam == AEGF_ITALIC)
        return (LRESULT)ae->ptxt->hFontItalic;
      if (wParam == AEGF_BOLDITALIC)
        return (LRESULT)ae->ptxt->hFontBoldItalic;
      if (wParam == AEGF_URL)
        return (LRESULT)ae->ptxt->hFontUrl;
      return 0;
    }
    case AEM_GETALTLINE:
    {
      return MAKELONG(ae->popt->dwAltLineSkip, ae->popt->dwAltLineFill);
    }
    case AEM_SETALTLINE:
    {
      if (ae->popt->dwAltLineSkip != LOWORD(wParam) ||
          ae->popt->dwAltLineFill != HIWORD(wParam))
      {
        ae->popt->dwAltLineSkip=LOWORD(wParam);
        ae->popt->dwAltLineFill=HIWORD(wParam);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      return 0;
    }
    case AEM_GETCHARCOLORS:
    {
      return AE_GetHighLightCharColors(ae, (const AECHARINDEX *)wParam, (AECHARCOLORS *)lParam);
    }
    case AEM_SCROLLCARETOPTIONS:
    {
      AESCROLLCARETOPTIONS *sco=(AESCROLLCARETOPTIONS *)lParam;

      if (wParam)
      {
        if (!sco)
        {
          ae->popt->dwCaretScrollFlags=0;
          ae->popt->dwCaretScrollSelFlags=0;
          ae->popt->dwCaretScrollSelType=0;
          ae->popt->nCaretScrollOffsetX=0;
          ae->popt->nCaretScrollOffsetY=0;
        }
        else
        {
          ae->popt->dwCaretScrollFlags=sco->dwFlags;
          ae->popt->dwCaretScrollSelFlags=sco->dwSelFlags;
          ae->popt->dwCaretScrollSelType=sco->dwSelType;
          ae->popt->nCaretScrollOffsetX=sco->nOffsetX;
          ae->popt->nCaretScrollOffsetY=sco->nOffsetY;
        }
      }
      else
      {
        sco->dwFlags=ae->popt->dwCaretScrollFlags;
        sco->dwSelFlags=ae->popt->dwCaretScrollSelFlags;
        sco->dwSelType=ae->popt->dwCaretScrollSelType;
        sco->nOffsetX=ae->popt->nCaretScrollOffsetX;
        sco->nOffsetY=ae->popt->nCaretScrollOffsetY;
      }
      return 0;
    }
    case AEM_FIXEDCHARWIDTH:
    {
      int nPrevWidth=ae->ptxt->nInitFixedCharWidth;

      ae->ptxt->nInitFixedCharWidth=(int)wParam;
      ae->ptxt->nFixedCharWidth=AE_GetFixedCharWidth(ae);
      ae->ptxt->nFixedTabWidth=ae->ptxt->nFixedCharWidth * ae->ptxt->nTabStop;
      return nPrevWidth;
    }
    case AEM_GETSCROLLSPEED:
    {
      return ae->popt->dwMScrollSpeed;
    }
    case AEM_SETSCROLLSPEED:
    {
      ae->popt->dwMScrollSpeed=(DWORD)wParam;
      return 0;
    }

    //Draw
    case AEM_SHOWSCROLLBAR:
    {
      BOOL bUpdate=FALSE;
      int nResult=-1;

      if (ae->bHScrollShow && ae->bVScrollShow)
        nResult=SB_BOTH;
      else if (ae->bHScrollShow)
        nResult=SB_HORZ;
      else if (ae->bVScrollShow)
        nResult=SB_VERT;

      if (wParam == SB_BOTH || wParam == SB_HORZ)
      {
        if (ae->bHScrollShow != lParam)
        {
          ae->bHScrollShow=(BOOL)lParam;
          bUpdate=TRUE;
        }
      }
      if (wParam == SB_BOTH || wParam == SB_VERT)
      {
        if (ae->bVScrollShow != lParam)
        {
          ae->bVScrollShow=(BOOL)lParam;
          bUpdate=TRUE;
        }
      }
      if (bUpdate)
        ShowScrollBar(ae->hWndEdit, (int)wParam, (BOOL)lParam);

      return nResult;
    }
    case AEM_UPDATESCROLLBAR:
    {
      AE_UpdateScrollBars(ae, (int)wParam);
      return 0;
    }
    case AEM_UPDATECARET:
    {
      return AE_UpdateCaret(ae, ae->bFocus);
    }
    case AEM_UPDATESIZE:
    {
      AE_UpdateSize(ae);
      return 0;
    }
    case AEM_LOCKUPDATE:
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
    case AEM_LOCKERASERECT:
    {
      AEERASE *lpErase=(AEERASE *)ae->hEraseStack.first;
      AEERASE *lpEraseNext;
      const RECT *rcLockErase=(const RECT *)lParam;
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
    case AEM_GETERASERECT:
    {
      RECT *lprcErase=(RECT *)lParam;
      RECT rcErase;

      if (wParam & AERC_MARGINS)
      {
        rcErase.left=ae->rcErase.left - ae->rcEdit.left;
        rcErase.top=ae->rcErase.top - ae->rcEdit.top;
        rcErase.right=ae->rcEdit.right - ae->rcErase.right;
        rcErase.bottom=ae->rcEdit.bottom - ae->rcErase.bottom;
      }
      else rcErase=ae->rcErase;

      if (!(wParam & AERC_NOLEFT))
        lprcErase->left=rcErase.left;
      if (!(wParam & AERC_NOTOP))
        lprcErase->top=rcErase.top;
      if (!(wParam & AERC_NORIGHT))
        lprcErase->right=rcErase.right;
      if (!(wParam & AERC_NOBOTTOM))
        lprcErase->bottom=rcErase.bottom;

      return 0;
    }
    case AEM_SETERASERECT:
    {
      RECT *lprcErase=lParam?(RECT *)lParam:&ae->rcEdit;
      RECT rcErase;

      if (wParam & AERC_MARGINS)
      {
        rcErase.left=ae->rcEdit.left + lprcErase->left;
        rcErase.top=ae->rcEdit.top + lprcErase->top;
        rcErase.right=ae->rcEdit.right -  lprcErase->right;
        rcErase.bottom=ae->rcEdit.bottom - lprcErase->bottom;
      }
      else rcErase=*lprcErase;

      if (wParam & AERC_NOLEFT)
        rcErase.left=ae->rcErase.left;
      if (wParam & AERC_NOTOP)
        rcErase.top=ae->rcErase.top;
      if (wParam & AERC_NORIGHT)
        rcErase.right=ae->rcErase.right;
      if (wParam & AERC_NOBOTTOM)
        rcErase.bottom=ae->rcErase.bottom;
      ae->rcErase=rcErase;

      if (wParam & AERC_UPDATE)
        InvalidateRect(ae->hWndEdit, NULL, TRUE);
      return 0;
    }
    case AEM_HIDESELECTION:
    {
      AE_HideSelection(ae, (BOOL)wParam);
      return 0;
    }
    case AEM_REDRAWLINERANGE:
    {
      AE_RedrawLineRange(ae, (int)wParam, (int)lParam, TRUE);
      return 0;
    }
    case AEM_GETBACKGROUNDIMAGE:
    {
      if (ae->popt->lpBkImage)
        return (LRESULT)ae->popt->lpBkImage->hBitmap;
      return (LRESULT)NULL;
    }
    case AEM_SETBACKGROUNDIMAGE:
    {
      BOOL bResult=FALSE;

      if (wParam)
      {
        if (ae->popt->lpBkImage)
        {
          if (ae->popt->lpBkImage->hBitmap == (HBITMAP)wParam)
            goto CheckAlpha;

          //Remove from stack, if handle not assigned to any other edit window.
          if (!--ae->popt->lpBkImage->nRefCount)
            AE_StackDcItemDelete(&hAkelEditBitmapDcStack, ae->popt->lpBkImage);
        }

        if (!(ae->popt->lpBkImage=AE_StackDcItemGet(&hAkelEditBitmapDcStack, (HBITMAP)wParam)))
          ae->popt->lpBkImage=AE_StackDcItemInsert(&hAkelEditBitmapDcStack, (HBITMAP)wParam);
        if (ae->popt->lpBkImage)
        {
          if (ae->popt->lpBkImage->hDC)
          {
            ++ae->popt->lpBkImage->nRefCount;
            bResult=TRUE;
          }
          else
          {
            AE_StackDcItemDelete(&hAkelEditBitmapDcStack, ae->popt->lpBkImage);
            ae->popt->lpBkImage=NULL;
            return FALSE;
          }
        }
        else return FALSE;
      }
      else if (ae->popt->lpBkImage)
      {
        //Remove from stack, if handle not assigned to any other edit window.
        if (!--ae->popt->lpBkImage->nRefCount)
          AE_StackDcItemDelete(&hAkelEditBitmapDcStack, ae->popt->lpBkImage);
        ae->popt->lpBkImage=NULL;
        bResult=TRUE;
      }
      else return FALSE;

      CheckAlpha:
      if (lParam != ae->popt->nBkImageAlpha)
      {
        ae->popt->nBkImageAlpha=(int)lParam;
        bResult=TRUE;
      }

      if (bResult)
        InvalidateRect(ae->hWndEdit, NULL, TRUE);
      return bResult;
    }

    //Folding
    case AEM_GETFOLDSTACK:
    {
      return (LRESULT)&ae->ptxt->hFoldsStack;
    }
    case AEM_GETFOLDCOUNT:
    {
      if (wParam == AEFC_ALL)
        return ae->ptxt->nFoldAllCount;
      if (wParam == AEFC_COLLAPSED)
        return ae->ptxt->nFoldCollapseCount;
      if (wParam == AEFC_COLORED)
        return ae->ptxt->nFoldColorCount;
      if (wParam == AEFC_WITHID)
        return ae->ptxt->nFoldWithIdCount;
      if (wParam == AEFC_WITHTHEME)
        return ae->ptxt->nFoldWithThemeCount;
      return 0;
    }
    case AEM_ADDFOLD:
    {
      AEFOLD *lpFold=(AEFOLD *)lParam;

      return (LRESULT)AE_StackFoldInsert(ae, lpFold);
    }
    case AEM_DELETEFOLD:
    {
      int nFirstVisibleLine=-1;
      int nResult;
      BOOL bUpdate=(BOOL)lParam;

      if (bUpdate)
      {
        if (!ae->popt->nVScrollLock)
          nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
      }

      if (wParam)
        nResult=AE_StackFoldDelete(ae, (AEFOLD *)wParam);
      else
        nResult=AE_StackFoldFree(ae);

      if (bUpdate && nResult)
      {
        AE_FoldUpdate(ae, nFirstVisibleLine);
      }
      return nResult;
    }
    case AEM_ISFOLDVALID:
    {
      return (LRESULT)AE_StackFoldIsValid(ae, (AEFOLD *)wParam);
    }
    case AEM_FINDFOLD:
    {
      AEFINDFOLD *ff=(AEFINDFOLD *)wParam;

      AE_StackFindFold(ae, ff->dwFlags, ff->dwFindIt, NULL, &ff->lpParent, &ff->lpPrevSubling);
      return 0;
    }
    case AEM_NEXTFOLD:
    {
      return (LRESULT)AEC_NextFold((AEFOLD *)wParam, (BOOL)lParam);
    }
    case AEM_PREVFOLD:
    {
      return (LRESULT)AEC_PrevFold((AEFOLD *)wParam, (BOOL)lParam);
    }
    case AEM_ISLINECOLLAPSED:
    {
      return (LRESULT)AE_IsLineCollapsed(ae, (int)wParam);
    }
    case AEM_COLLAPSELINE:
    case AEM_COLLAPSEFOLD:
    {
      int nFirstVisibleLine=-1;
      int nResult;

      if (!(lParam & AECF_NOUPDATE))
      {
        if (!ae->popt->nVScrollLock)
          nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
      }

      if (uMsg == AEM_COLLAPSELINE)
        nResult=AE_LineCollapse(ae, (int)wParam, (DWORD)lParam);
      else
        nResult=AE_FoldCollapse(ae, (AEFOLD *)wParam, (DWORD)lParam);
      ae->ptxt->lpVPosFold=NULL;

      if (nResult && !(lParam & AECF_NOUPDATE))
      {
        AE_FoldUpdate(ae, nFirstVisibleLine);
      }
      return nResult;
    }
    case AEM_UPDATEFOLD:
    {
      return AE_FoldUpdate(ae, (int)lParam);
    }
    case AEM_GETFOLDHIDEOFFSET:
    {
      return MAKELONG(ae->ptxt->nHideMinLineOffset, ae->ptxt->nHideMaxLineOffset);
    }
    case AEM_SETFOLDHIDEOFFSET:
    {
      ae->ptxt->nHideMinLineOffset=max((short)LOWORD(wParam), 1);
      ae->ptxt->nHideMaxLineOffset=min((short)HIWORD(wParam), 0);
      return 0;
    }

    //Window data
    case AEM_CREATEDOCUMENT:
    {
      AKELEDIT *aeNew;

      if (aeNew=AE_CreateWindowData(ae->hWndEdit, (CREATESTRUCTA *)lParam, (AEEditProc)AE_EditProc))
      {
        //Initially unassigned. Later assigned with AEM_SETDOCUMENT.
        aeNew->hWndEdit=NULL;
      }
      return (LRESULT)aeNew;
    }
    case AEM_DELETEDOCUMENT:
    {
      AE_DestroyWindowData((AKELEDIT *)wParam);
      if (lpAkelEditPrev == (AKELEDIT *)wParam)
        lpAkelEditPrev=NULL;
      return 0;
    }
    case AEM_GETDOCUMENT:
    {
      return (LRESULT)ae;
    }
    case AEM_SETDOCUMENT:
    {
      return (LRESULT)AE_SetWindowData(ae, (AKELEDIT *)wParam, (DWORD)lParam);
    }
    case AEM_GETDOCUMENTPROC:
    {
      AKELEDIT *aeHandle=(AKELEDIT *)wParam;

      if (aeHandle)
        return (LRESULT)aeHandle->lpEditProc;
      else
        return (LRESULT)ae->lpEditProc;
    }
    case AEM_GETDOCUMENTWINDOW:
    {
      AKELEDIT *aeHandle=(AKELEDIT *)wParam;

      if (aeHandle)
        return (LRESULT)aeHandle->hWndEdit;
      else
        return (LRESULT)ae->hWndEdit;
    }
    case AEM_SENDMESSAGE:
    {
      AESENDMESSAGE *psm=(AESENDMESSAGE *)lParam;
      HWND hWndEdit=ae->hWndEdit;

      if (psm->uMsg != AEM_SENDMESSAGE)
      {
        if (psm->hDoc && (AKELEDIT *)psm->hDoc != ae)
          AE_SetWindowData(ae, (AKELEDIT *)psm->hDoc, AESWD_NOALL);

        if (!ae->bUnicodeWindow)
          psm->lResult=SendMessageA(hWndEdit, psm->uMsg, psm->wParam, psm->lParam);
        else
          psm->lResult=SendMessageW(hWndEdit, psm->uMsg, psm->wParam, psm->lParam);

        if (psm->hDoc && (AKELEDIT *)psm->hDoc != ae && ((AKELEDIT *)psm->hDoc)->hWndEdit == hWndEdit)
          AE_SetWindowData((AKELEDIT *)psm->hDoc, ae, AESWD_NOALL);
        return TRUE;
      }
      return FALSE;
    }

    //Clones
    case AEM_ADDCLONE:
    {
      AKELEDIT *aeClone;

      if (aeClone=AE_StackWindowGet(&hAkelEditWindowsStack, (HWND)wParam))
      {
        AE_StackCloneAdd(ae, aeClone);
        return TRUE;
      }
      return FALSE;
    }
    case AEM_DELCLONE:
    {
      AKELEDIT *aeClone;
      AECLONE *aec;

      if (wParam)
      {
        if (aeClone=AE_StackWindowGet(&hAkelEditWindowsStack, (HWND)wParam))
        {
          if (aec=AE_StackCloneGet(ae, aeClone))
          {
            AE_StackCloneDelete(aec);
            return TRUE;
          }
        }
      }
      else
      {
        AE_StackCloneDeleteAll(ae);
        return TRUE;
      }
      return FALSE;
    }
    case AEM_GETMASTER:
    {
      if (ae->lpMaster)
        return (LRESULT)ae->lpMaster->hWndEdit;
      if (ae->nCloneCount > 0)
        return (LRESULT)ae->hWndEdit;

      return (LRESULT)NULL;
    }
    case AEM_GETCLONE:
    {
      AECLONE *aec;

      if (aec=AE_StackCloneIndex(ae, (DWORD)wParam))
        return (LRESULT)aec->aeClone->hWndEdit;

      return (LRESULT)NULL;
    }

    //Print
    case AEM_STARTPRINTDOC:
    {
      AEPRINT *prn=(AEPRINT *)lParam;

      if (!ae->bUnicodeWindow)
        return (LRESULT)AE_StartPrintDocA(ae, prn);
      else
        return (LRESULT)AE_StartPrintDocW(ae, prn);
    }
    case AEM_PRINTPAGE:
    {
      AEPRINTHANDLE *ph=(AEPRINTHANDLE *)wParam;
      AEPRINT *prn=(AEPRINT *)lParam;

      return (LRESULT)AE_PrintPage(ae, ph, prn);
    }
    case AEM_ENDPRINTDOC:
    {
      AEPRINTHANDLE *ph=(AEPRINTHANDLE *)wParam;
      AEPRINT *prn=(AEPRINT *)lParam;

      AE_EndPrintDoc(ae, ph, prn);
      return 0;
    }

    //HighLight
    case AEM_HLCREATETHEMEA:
    {
      char *pThemeName=(char *)lParam;
      wchar_t wszThemeName[MAX_PATH];

      MultiByteToWideChar(CP_ACP, 0, pThemeName, -1, wszThemeName, MAX_PATH);
      return (LRESULT)AE_HighlightCreateTheme(wParam, wszThemeName);
    }
    case AEM_HLCREATETHEMEW:
    {
      wchar_t *wpThemeName=(wchar_t *)lParam;

      return (LRESULT)AE_HighlightCreateTheme(wParam, wpThemeName);
    }
    case AEM_HLFINDTHEME:
    {
      if (wParam == AEHLFT_CURRENT)
      {
        return (LRESULT)ae->popt->lpActiveTheme;
      }
      else if (wParam == AEHLFT_BYNAMEA)
      {
        char *pThemeName=(char *)lParam;
        wchar_t wszThemeName[MAX_PATH];

        if (!pThemeName)
          return (LRESULT)ae->popt->lpActiveTheme;
        MultiByteToWideChar(CP_ACP, 0, pThemeName, -1, wszThemeName, MAX_PATH);
        return (LRESULT)AE_HighlightGetTheme(wszThemeName);
      }
      else if (wParam == AEHLFT_BYNAMEW)
      {
        wchar_t *wpThemeName=(wchar_t *)lParam;

        if (!wpThemeName)
          return (LRESULT)ae->popt->lpActiveTheme;
        return (LRESULT)AE_HighlightGetTheme(wpThemeName);
      }
      else if (wParam == AEHLFT_BYFOLD)
      {
        AEFINDFOLD *ff=(AEFINDFOLD *)lParam;

        if (ae->ptxt->nFoldWithThemeCount)
        {
          AE_StackFindFold(ae, ff->dwFlags, ff->dwFindIt, NULL, &ff->lpParent, &ff->lpPrevSubling);

          while (ff->lpParent)
          {
            if (ff->lpParent->hRuleTheme)
              return (LRESULT)ff->lpParent->hRuleTheme;
            ff->lpParent=ff->lpParent->parent;
          }
        }
        else
        {
          ff->lpParent=NULL;
          ff->lpPrevSubling=NULL;
        }
        return (LRESULT)ae->popt->lpActiveTheme;
      }
      return 0;
    }
    case AEM_HLGETTHEMENAMEA:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (!lParam)
        return xstrlenW(lpTheme->wszThemeName) + 1;
      return WideCharToMultiByte(CP_ACP, 0, lpTheme->wszThemeName, -1, (char *)lParam, MAX_PATH, NULL, NULL) - 1;
    }
    case AEM_HLGETTHEMENAMEW:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (!lParam)
        return xstrlenW(lpTheme->wszThemeName) + 1;
      return xstrcpynW((wchar_t *)lParam, lpTheme->wszThemeName, MAX_PATH);
    }
    case AEM_HLGETTHEMESTACK:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (lParam == AEHLE_DELIMITER)
        return (LRESULT)(lpTheme?&lpTheme->hDelimiterStack:&ae->ptxt->hDelimiterStack);
      if (lParam == AEHLE_WORD)
        return (LRESULT)(lpTheme?&lpTheme->hWordStack:&ae->ptxt->hWordStack);
      if (lParam == AEHLE_QUOTE)
        return (LRESULT)(lpTheme?&lpTheme->hQuoteStack:&ae->ptxt->hQuoteStack);
      if (lParam == AEHLE_MARKTEXT)
        return (LRESULT)(lpTheme?&lpTheme->hMarkTextStack:&ae->ptxt->hMarkTextStack);
      if (lParam == AEHLE_MARKRANGE)
        return (LRESULT)(lpTheme?&lpTheme->hMarkRangeStack:&ae->ptxt->hMarkRangeStack);
    }
    case AEM_HLTHEMEEXISTS:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      return AE_HighlightIsThemeExists(lpTheme);
    }
    case AEM_HLSETTHEME:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      ae->popt->lpActiveTheme=lpTheme;
      if (lParam) InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      return 0;
    }
    case AEM_HLDELETETHEME:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;

      if (lpTheme)
        AE_HighlightUnsetTheme(lpTheme);
      AE_HighlightDeleteTheme(ae, lpTheme);
      return 0;
    }
    case AEM_HLDELETETHEMEALL:
    {
      AE_HighlightUnsetTheme(NULL);
      AE_HighlightDeleteThemeAll();
      return 0;
    }
    case AEM_HLADDDELIMITERA:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEDELIMITEMA *lpDelimSrc=(AEDELIMITEMA *)lParam;
      AEDELIMITEMW *lpDelimDst;

      if (lpDelimDst=AE_HighlightInsertDelimiter(ae, lpTheme, lpDelimSrc->nDelimiterLen, lpDelimSrc->nIndex))
      {
        if (lpDelimDst->pDelimiter=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpDelimSrc->nDelimiterLen + 1) * sizeof(wchar_t)))
          lpDelimDst->nDelimiterLen=MultiByteToWideChar(CP_ACP, 0, lpDelimSrc->pDelimiter, lpDelimSrc->nDelimiterLen + 1, (wchar_t *)lpDelimDst->pDelimiter, lpDelimSrc->nDelimiterLen + 1) - 1;

        lpDelimDst->dwFlags=lpDelimSrc->dwFlags;
        lpDelimDst->dwFontStyle=lpDelimSrc->dwFontStyle;
        lpDelimDst->crText=lpDelimSrc->crText;
        lpDelimDst->crBk=lpDelimSrc->crBk;
        lpDelimDst->nParentID=lpDelimSrc->nParentID;
      }
      return (LRESULT)lpDelimDst;
    }
    case AEM_HLADDDELIMITERW:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEDELIMITEMW *lpDelimSrc=(AEDELIMITEMW *)lParam;
      AEDELIMITEMW *lpDelimDst;

      if (lpDelimDst=AE_HighlightInsertDelimiter(ae, lpTheme, lpDelimSrc->nDelimiterLen, lpDelimSrc->nIndex))
      {
        if (lpDelimDst->pDelimiter=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpDelimSrc->nDelimiterLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpDelimDst->pDelimiter, lpDelimSrc->pDelimiter, (lpDelimSrc->nDelimiterLen + 1) * sizeof(wchar_t));
          lpDelimDst->nDelimiterLen=lpDelimSrc->nDelimiterLen;
        }
        lpDelimDst->dwFlags=lpDelimSrc->dwFlags;
        lpDelimDst->dwFontStyle=lpDelimSrc->dwFontStyle;
        lpDelimDst->crText=lpDelimSrc->crText;
        lpDelimDst->crBk=lpDelimSrc->crBk;
        lpDelimDst->nParentID=lpDelimSrc->nParentID;
      }
      return (LRESULT)lpDelimDst;
    }
    case AEM_HLDELETEDELIMITER:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEDELIMITEMW *lpDelim=(AEDELIMITEMW *)lParam;

      if (lpDelim)
        AE_HighlightDeleteDelimiter(ae, lpTheme, lpDelim);
      else
        AE_HighlightDeleteDelimiterAll(ae, lpTheme);
      return 0;
    }
    case AEM_HLADDWORDA:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEWORDITEMA *lpWordSrc=(AEWORDITEMA *)lParam;
      AEWORDITEMW *lpWordDst;
      int nWordLen;

      if (!(lpWordSrc->dwFlags & AEHLF_WORDCOMPOSITION))
        nWordLen=lpWordSrc->nWordLen;
      else
        nWordLen=0;

      if (lpWordDst=AE_HighlightInsertWord(ae, lpTheme, nWordLen))
      {
        if (lpWordDst->pWord=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpWordSrc->nWordLen + 1) * sizeof(wchar_t)))
          MultiByteToWideChar(CP_ACP, 0, lpWordSrc->pWord, lpWordSrc->nWordLen + 1, (wchar_t *)lpWordDst->pWord, lpWordSrc->nWordLen + 1);
        lpWordDst->nWordLen=nWordLen;

        lpWordDst->dwFlags=lpWordSrc->dwFlags;
        lpWordDst->dwFontStyle=lpWordSrc->dwFontStyle;
        lpWordDst->crText=lpWordSrc->crText;
        lpWordDst->crBk=lpWordSrc->crBk;
        lpWordDst->nParentID=lpWordSrc->nParentID;
      }
      return (LRESULT)lpWordDst;
    }
    case AEM_HLADDWORDW:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEWORDITEMW *lpWordSrc=(AEWORDITEMW *)lParam;
      AEWORDITEMW *lpWordDst;
      int nWordLen;

      if (!(lpWordSrc->dwFlags & AEHLF_WORDCOMPOSITION))
        nWordLen=lpWordSrc->nWordLen;
      else
        nWordLen=0;

      if (lpWordDst=AE_HighlightInsertWord(ae, lpTheme, nWordLen))
      {
        if (lpWordDst->pWord=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpWordSrc->nWordLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpWordDst->pWord, lpWordSrc->pWord, (lpWordSrc->nWordLen + 1) * sizeof(wchar_t));
          lpWordDst->nWordLen=nWordLen;
        }
        lpWordDst->dwFlags=lpWordSrc->dwFlags;
        lpWordDst->dwFontStyle=lpWordSrc->dwFontStyle;
        lpWordDst->crText=lpWordSrc->crText;
        lpWordDst->crBk=lpWordSrc->crBk;
        lpWordDst->nParentID=lpWordSrc->nParentID;
      }
      return (LRESULT)lpWordDst;
    }
    case AEM_HLDELETEWORD:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEWORDITEMW *lpWord=(AEWORDITEMW *)lParam;

      if (lpWord)
        AE_HighlightDeleteWord(ae, lpTheme, lpWord);
      else
        AE_HighlightDeleteWordAll(ae, lpTheme);
      return 0;
    }
    case AEM_HLADDQUOTEA:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEQUOTEITEMA *lpQuoteSrc=(AEQUOTEITEMA *)lParam;
      AEQUOTEITEMW *lpQuoteDst;

      if (lpQuoteDst=AE_HighlightInsertQuote(ae, lpTheme, lpQuoteSrc->nIndex))
      {
        if (lpQuoteSrc->pQuoteStart && !(lpQuoteSrc->dwFlags & AEHLF_QUOTESTART_ISDELIMITER))
        {
          if (lpQuoteDst->pQuoteStart=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteStartLen + 1) * sizeof(wchar_t)))
            lpQuoteDst->nQuoteStartLen=MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteStart, lpQuoteSrc->nQuoteStartLen + 1, (wchar_t *)lpQuoteDst->pQuoteStart, lpQuoteSrc->nQuoteStartLen + 1) - 1;
        }
        if (lpQuoteSrc->pQuoteEnd && !(lpQuoteSrc->dwFlags & AEHLF_QUOTEEND_ISDELIMITER))
        {
          if (lpQuoteDst->pQuoteEnd=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteEndLen + 1) * sizeof(wchar_t)))
            lpQuoteDst->nQuoteEndLen=MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteEnd, lpQuoteSrc->nQuoteEndLen + 1, (wchar_t *)lpQuoteDst->pQuoteEnd, lpQuoteSrc->nQuoteEndLen + 1) - 1;
        }

        MultiByteToWideChar(CP_ACP, 0, &lpQuoteSrc->chEscape, 1, &lpQuoteDst->chEscape, 1);

        if (lpQuoteSrc->pQuoteInclude)
        {
          if (lpQuoteDst->pQuoteInclude=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteIncludeLen + 1) * sizeof(wchar_t)))
            lpQuoteDst->nQuoteIncludeLen=MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteInclude, lpQuoteSrc->nQuoteIncludeLen + 1, (wchar_t *)lpQuoteDst->pQuoteInclude, lpQuoteSrc->nQuoteIncludeLen + 1) - 1;
        }
        if (lpQuoteSrc->pQuoteExclude)
        {
          if (lpQuoteDst->pQuoteExclude=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteExcludeLen + 1) * sizeof(wchar_t)))
            lpQuoteDst->nQuoteExcludeLen=MultiByteToWideChar(CP_ACP, 0, lpQuoteSrc->pQuoteExclude, lpQuoteSrc->nQuoteExcludeLen + 1, (wchar_t *)lpQuoteDst->pQuoteExclude, lpQuoteSrc->nQuoteExcludeLen + 1) - 1;
        }

        lpQuoteDst->dwFlags=lpQuoteSrc->dwFlags;
        lpQuoteDst->dwFontStyle=lpQuoteSrc->dwFontStyle;
        lpQuoteDst->crText=lpQuoteSrc->crText;
        lpQuoteDst->crBk=lpQuoteSrc->crBk;
        lpQuoteDst->nParentID=lpQuoteSrc->nParentID;
        lpQuoteDst->nRuleID=lpQuoteSrc->nRuleID;

        if (!AE_HighlightAddQuote(ae, lpTheme, lpQuoteDst, lpQuoteDst))
        {
          lpQuoteSrc->nCompileErrorOffset=lpQuoteDst->nCompileErrorOffset;
          AE_HighlightDeleteQuote(ae, lpTheme, lpQuoteDst);
          lpQuoteDst=NULL;
        }
      }
      return (LRESULT)lpQuoteDst;
    }
    case AEM_HLADDQUOTEW:
    {
      return (LRESULT)AE_HighlightAddQuote(ae, (AETHEMEITEMW *)wParam, (AEQUOTEITEMW *)lParam, NULL);
    }
    case AEM_HLDELETEQUOTE:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEQUOTEITEMW *lpQuote=(AEQUOTEITEMW *)lParam;

      if (lpQuote)
        AE_HighlightDeleteQuote(ae, lpTheme, lpQuote);
      else
        AE_HighlightDeleteQuoteAll(ae, lpTheme);
      return 0;
    }
    case AEM_HLADDMARKTEXTA:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKTEXTITEMA *lpMarkTextSrc=(AEMARKTEXTITEMA *)lParam;
      AEMARKTEXTITEMW *lpMarkTextDst;

      if (lpMarkTextDst=AE_HighlightInsertMarkText(ae, lpTheme, lpMarkTextSrc->nIndex))
      {
        if (lpMarkTextDst->pMarkText=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpMarkTextSrc->nMarkTextLen + 1) * sizeof(wchar_t)))
          lpMarkTextDst->nMarkTextLen=MultiByteToWideChar(CP_ACP, 0, lpMarkTextSrc->pMarkText, lpMarkTextSrc->nMarkTextLen + 1, (wchar_t *)lpMarkTextDst->pMarkText, lpMarkTextSrc->nMarkTextLen + 1) - 1;

        if (!AE_HighlightAddMarkText(ae, lpTheme, lpMarkTextDst, lpMarkTextDst))
        {
          lpMarkTextSrc->nCompileErrorOffset=lpMarkTextDst->nCompileErrorOffset;
          AE_HighlightDeleteMarkText(ae, lpTheme, lpMarkTextDst);
          lpMarkTextDst=NULL;
        }
      }
      return (LRESULT)lpMarkTextDst;
    }
    case AEM_HLADDMARKTEXTW:
    {
      return (LRESULT)AE_HighlightAddMarkText(ae, (AETHEMEITEMW *)wParam, (AEMARKTEXTITEMW *)lParam, NULL);
    }
    case AEM_HLDELETEMARKTEXT:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKTEXTITEMW *lpMarkText=(AEMARKTEXTITEMW *)lParam;

      if (lpMarkText)
        AE_HighlightDeleteMarkText(ae, lpTheme, lpMarkText);
      else
        AE_HighlightDeleteMarkTextAll(ae, lpTheme);
      return 0;
    }
    case AEM_HLADDMARKRANGE:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKRANGEITEM *lpMarkRangeSrc=(AEMARKRANGEITEM *)lParam;
      AEMARKRANGEITEM *lpMarkRangeDst;

      if (lpMarkRangeDst=AE_HighlightInsertMarkRange(ae, lpTheme, lpMarkRangeSrc->nIndex))
      {
        lpMarkRangeDst->crMarkRange=lpMarkRangeSrc->crMarkRange;
        lpMarkRangeDst->dwFlags=lpMarkRangeSrc->dwFlags;
        lpMarkRangeDst->dwFontStyle=lpMarkRangeSrc->dwFontStyle;
        lpMarkRangeDst->crText=lpMarkRangeSrc->crText;
        lpMarkRangeDst->crBk=lpMarkRangeSrc->crBk;
      }
      return (LRESULT)lpMarkRangeDst;
    }
    case AEM_HLDELETEMARKRANGE:
    {
      AETHEMEITEMW *lpTheme=(AETHEMEITEMW *)wParam;
      AEMARKRANGEITEM *lpMarkRange=(AEMARKRANGEITEM *)lParam;

      if (lpMarkRange)
        AE_HighlightDeleteMarkRange(ae, lpTheme, lpMarkRange);
      else
        AE_HighlightDeleteMarkRangeAll(ae, lpTheme);
      return 0;
    }
    case AEM_HLGETHIGHLIGHT:
    {
      AEGETHIGHLIGHT *aegh=(AEGETHIGHLIGHT *)lParam;

      AE_GetHighLight(ae, aegh);
      return 0;
    }
    case AEM_HLGETOPTIONS:
    {
      return ae->popt->dwHLOptions;
    }
    case AEM_HLSETOPTIONS:
    {
      DWORD dwHLOptionsOld=ae->popt->dwHLOptions;
      DWORD dwHLOptionsNew=ae->popt->dwHLOptions;

      if (wParam == AECOOP_SET)
        dwHLOptionsNew=(DWORD)lParam;
      else if (wParam == AECOOP_OR)
        dwHLOptionsNew|=(DWORD)lParam;
      else if (wParam == AECOOP_AND)
        dwHLOptionsNew&=(DWORD)lParam;
      else if (wParam == AECOOP_XOR)
        dwHLOptionsNew&=~(DWORD)lParam;

      if (((dwHLOptionsOld & AEHLO_IGNOREFONTNORMAL) && !(dwHLOptionsNew & AEHLO_IGNOREFONTNORMAL)) ||
          ((dwHLOptionsOld & AEHLO_IGNOREFONTBOLD) && !(dwHLOptionsNew & AEHLO_IGNOREFONTBOLD)) ||
          ((dwHLOptionsOld & AEHLO_IGNOREFONTITALIC) && !(dwHLOptionsNew & AEHLO_IGNOREFONTITALIC)))
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        AE_StackCloneUpdate(ae);
      }
      ae->popt->dwHLOptions=dwHLOptionsNew;
      return ae->popt->dwHLOptions;
    }


    //// EM_* rich edit messages

    //Text retrieval and modification
    case EM_STREAMIN:
    {
      EDITSTREAM *es=(EDITSTREAM *)lParam;
      AESTREAMIN aesi;
      DWORD dwFlags=0;
      UINT_PTR dwResult=0;

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
    case EM_STREAMOUT:
    {
      EDITSTREAM *es=(EDITSTREAM *)lParam;
      AESTREAMOUT aeso;
      DWORD dwFlags=0;
      UINT_PTR dwResult=0;

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
    case EM_REPLACESEL:
    case EM_REPLACESELA:
    case EM_REPLACESELW:
    {
      if (uMsg == EM_REPLACESELA || (!ae->bUnicodeWindow && uMsg == EM_REPLACESEL))
        AE_ReplaceSelAnsi(ae, CP_ACP, (char *)lParam, (UINT_PTR)-1, AELB_ASINPUT, 0, NULL, NULL);
      else
        AE_ReplaceSel(ae, (wchar_t *)lParam, (UINT_PTR)-1, AELB_ASINPUT, 0, NULL, NULL);
      return 0;
    }
    case EM_GETTEXTRANGE:
    {
      TEXTRANGEW *trRE=(TEXTRANGEW *)lParam;
      AECHARRANGE cr;

      if ((UINT)trRE->chrg.cpMin < (UINT)trRE->chrg.cpMax)
      {
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMin, &cr.ciMin);
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMax, &cr.ciMax);

        if (!ae->bUnicodeWindow)
          return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, (char *)trRE->lpstrText, (UINT_PTR)-1, AELB_R, FALSE, FALSE);
        else
          return AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, trRE->lpstrText, (UINT_PTR)-1, AELB_R, FALSE, FALSE);
      }
      else if ((UINT)trRE->chrg.cpMin == (UINT)trRE->chrg.cpMax)
      {
        if (trRE->lpstrText)
        {
          if (!ae->bUnicodeWindow)
            *(char *)trRE->lpstrText='\0';
          else
            *trRE->lpstrText=L'\0';
        }
        else return 1;
      }
      return 0;
    }
    case EM_GETTEXTRANGE64:
    case EM_GETTEXTRANGE64A:
    case EM_GETTEXTRANGE64W:
    {
      TEXTRANGE64W *trRE=(TEXTRANGE64W *)lParam;
      AECHARRANGE cr;

      if ((UINT_PTR)trRE->chrg.cpMin < (UINT_PTR)trRE->chrg.cpMax)
      {
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMin, &cr.ciMin);
        AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMax, &cr.ciMax);

        if (uMsg == EM_GETTEXTRANGE64A || (!ae->bUnicodeWindow && uMsg == EM_GETTEXTRANGE64))
          return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, (char *)trRE->lpstrText, (UINT_PTR)-1, AELB_R, FALSE, FALSE);
        else
          return AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, trRE->lpstrText, (UINT_PTR)-1, AELB_R, FALSE, FALSE);
      }
      else if ((UINT_PTR)trRE->chrg.cpMin == (UINT_PTR)trRE->chrg.cpMax)
      {
        if (trRE->lpstrText)
        {
          if (uMsg == EM_GETTEXTRANGE64A || (!ae->bUnicodeWindow && uMsg == EM_GETTEXTRANGE64))
            *(char *)trRE->lpstrText='\0';
          else
            *trRE->lpstrText=L'\0';
        }
        else return 1;
      }
      return 0;
    }
    case EM_GETSELTEXT:
    case EM_GETSELTEXTA:
    case EM_GETSELTEXTW:
    {
      if (uMsg == EM_GETSELTEXTA || (!ae->bUnicodeWindow && uMsg == EM_GETSELTEXT))
        return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (char *)lParam, (UINT_PTR)-1, AELB_R, FALSE, FALSE);
      else
        return AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)lParam, (UINT_PTR)-1, AELB_R, FALSE, FALSE);
    }
    case EM_GETLINE:
    case EM_GETLINEA:
    case EM_GETLINEW:
    {
      if (uMsg == EM_GETLINEA || (!ae->bUnicodeWindow && uMsg == EM_GETLINE))
      {
        AELINEDATA *lpLine;
        char *szBuffer=(char *)lParam;
        int nMaxBuffer=*(WORD *)lParam;
        int nResult=0;

        szBuffer[0]='\0';

        if (lpLine=AE_GetLineData(ae, (int)wParam))
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

        wszBuffer[0]=L'\0';

        if (lpLine=AE_GetLineData(ae, (int)wParam))
        {
          nResult=min(nMaxBuffer, lpLine->nLineLen);
          xmemcpy(wszBuffer, lpLine->wpLine, nResult * sizeof(wchar_t));
          if (nResult < nMaxBuffer)
            wszBuffer[nResult++]=L'\r';
        }
        return nResult;
      }
    }
    case EM_GETTEXTLENGTHEX:
    {
      GETTEXTLENGTHEX *gtl=(GETTEXTLENGTHEX *)wParam;
      UINT_PTR dwResult=0;

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
          AELINEDATA *lpLine;

          for (lpLine=ae->ptxt->hLinesStack.first; lpLine; lpLine=lpLine->next)
          {
            if (lpLine->nLineLen)
            {
              dwResult+=WideCharToMultiByte(gtl->codepage, 0, lpLine->wpLine, lpLine->nLineLen, NULL, 0, NULL, NULL);
            }

          }
          dwResult+=ae->ptxt->nLineCount;
          if (gtl->flags & GTL_USECRLF)
            dwResult+=ae->ptxt->nLineCount;
        }
      }
      return dwResult;
    }
    case EM_GETTEXTEX:
    {
      GETTEXTEX *gt=(GETTEXTEX *)wParam;
      AECHARRANGE cr;
      int nNewLine;
      UINT_PTR dwResult=0;

      if (gt->flags & GT_SELECTION)
      {
        cr.ciMin=ae->ciSelStartIndex;
        cr.ciMax=ae->ciSelEndIndex;
      }
      else
      {
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin);
        AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax);
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
        AE_ChangeTwoBytesOrder((unsigned char *)lParam, dwResult * sizeof(wchar_t), NULL);
      }
      else dwResult=AE_GetTextRangeAnsi(ae, gt->codepage, gt->lpDefaultChar, gt->lpUsedDefChar, &cr.ciMin, &cr.ciMax, (char *)lParam, gt->cb, nNewLine, FALSE, FALSE);

      return dwResult;
    }
    case EM_GETTEXTEX64:
    {
      GETTEXTEX64 *gt=(GETTEXTEX64 *)wParam;
      AECHARRANGE cr;
      int nNewLine;
      UINT_PTR dwResult=0;

      if (gt->flags & GT_SELECTION)
      {
        cr.ciMin=ae->ciSelStartIndex;
        cr.ciMax=ae->ciSelEndIndex;
      }
      else
      {
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin);
        AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax);
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
        AE_ChangeTwoBytesOrder((unsigned char *)lParam, dwResult * sizeof(wchar_t), NULL);
      }
      else dwResult=AE_GetTextRangeAnsi(ae, gt->codepage, gt->lpDefaultChar, gt->lpUsedDefChar, &cr.ciMin, &cr.ciMax, (char *)lParam, gt->cb, nNewLine, FALSE, FALSE);

      return dwResult;
    }
    case EM_SETTEXTEX:
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
        AE_ReplaceSel(ae, (wchar_t *)lParam, (UINT_PTR)-1, AELB_ASINPUT, 0, NULL, NULL);
      }
      else if (st->codepage == 1201)
      {
        wchar_t *wszText;
        UINT_PTR dwUnicodeBytes;

        dwUnicodeBytes=xstrlenW((wchar_t *)lParam) * sizeof(wchar_t);

        if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, dwUnicodeBytes + 2))
        {
          xmemcpy(wszText, (wchar_t *)lParam, dwUnicodeBytes + 2);
          AE_ChangeTwoBytesOrder((unsigned char *)wszText, dwUnicodeBytes, NULL);
          AE_ReplaceSel(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), AELB_ASINPUT, 0, NULL, NULL);
          AE_HeapFree(NULL, 0, (LPVOID)wszText);
        }
      }
      else AE_ReplaceSelAnsi(ae, st->codepage, (char *)lParam, (UINT_PTR)-1, AELB_ASINPUT, 0, NULL, NULL);

      if (!(st->flags & ST_KEEPUNDO))
        ae->ptxt->bLockCollectUndo=bLockCollectUndo;
      if (!(st->flags & ST_SELECTION))
        AE_SetModify(ae, FALSE);

      return 1;
    }
    case EM_FINDTEXT:
    case EM_FINDTEXTW:
    case EM_FINDTEXTEX:
    case EM_FINDTEXTEXW:
    {
      FINDTEXTEXW *ftRE=(FINDTEXTEXW *)lParam;
      AEFINDTEXTW ft;
      BOOL bResult;

      ft.dwFlags=0;
      if (wParam & FR_DOWN)
        ft.dwFlags|=AEFR_DOWN;
      if (wParam & FR_MATCHCASE)
        ft.dwFlags|=AEFR_MATCHCASE;
      if (wParam & FR_WHOLEWORD)
        ft.dwFlags|=AEFR_WHOLEWORD;
      ft.pText=(wchar_t *)ftRE->lpstrText;
      ft.dwTextLen=(UINT_PTR)-1;
      ft.nNewLine=AELB_R;
      AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMin, &ft.crSearch.ciMin);
      AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMax, &ft.crSearch.ciMax);

      if (!ae->bUnicodeWindow && (uMsg == EM_FINDTEXT || uMsg == EM_FINDTEXTEX))
        bResult=AE_FindTextAnsi(ae, CP_ACP, (AEFINDTEXTA *)&ft);
      else
        bResult=AE_FindText(ae, &ft);

      if (bResult)
      {
        if (uMsg == EM_FINDTEXTEX || uMsg == EM_FINDTEXTEXW)
        {
          ftRE->chrgText.cpMin=(int)AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMin);
          ftRE->chrgText.cpMax=(int)AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMax);
          return ftRE->chrgText.cpMin;
        }
        return AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMin);
      }
      return -1;
    }
    case EM_FINDTEXTEX64:
    case EM_FINDTEXTEX64A:
    case EM_FINDTEXTEX64W:
    {
      FINDTEXTEX64W *ftRE=(FINDTEXTEX64W *)lParam;
      AEFINDTEXTW ft;
      BOOL bResult;

      ft.dwFlags=0;
      if (wParam & FR_DOWN)
        ft.dwFlags|=AEFR_DOWN;
      if (wParam & FR_MATCHCASE)
        ft.dwFlags|=AEFR_MATCHCASE;
      if (wParam & FR_WHOLEWORD)
        ft.dwFlags|=AEFR_WHOLEWORD;
      ft.pText=(wchar_t *)ftRE->lpstrText;
      ft.dwTextLen=(UINT_PTR)-1;
      ft.nNewLine=AELB_R;
      AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMin, &ft.crSearch.ciMin);
      AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMax, &ft.crSearch.ciMax);

      if (uMsg == EM_FINDTEXTEX64A || (!ae->bUnicodeWindow && uMsg == EM_FINDTEXTEX64))
        bResult=AE_FindTextAnsi(ae, CP_ACP, (AEFINDTEXTA *)&ft);
      else
        bResult=AE_FindText(ae, &ft);

      if (bResult)
      {
        ftRE->chrgText.cpMin=AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMin);
        ftRE->chrgText.cpMax=AE_AkelIndexToRichOffset(ae, &ft.crFound.ciMax);
        return ftRE->chrgText.cpMin;
      }
      return -1;
    }
    case EM_CANPASTE:
    {
      return AE_EditCanPaste(ae);
    }
    case EM_GETLIMITTEXT:
    {
      return ae->ptxt->dwTextLimit;
    }
    case EM_SETLIMITTEXT:
    //case EM_LIMITTEXT:
    {
      if (!wParam)
        ae->ptxt->dwTextLimit=65536;
      else
        ae->ptxt->dwTextLimit=wParam;
      return 0;
    }
    case EM_EXLIMITTEXT:
    {
      if (!lParam)
        ae->ptxt->dwTextLimit=65536;
      else
        ae->ptxt->dwTextLimit=lParam;
      return 0;
    }

    //Undo and Redo
    case EM_CANUNDO:
    {
      if (ae->popt->dwOptions & AECO_READONLY)
        return FALSE;
      return AE_EditCanUndo(ae);
    }
    case EM_CANREDO:
    {
      if (ae->popt->dwOptions & AECO_READONLY)
        return FALSE;
      return AE_EditCanRedo(ae);
    }
    case EM_UNDO:
    {
      return AE_EditUndo(ae);
    }
    case EM_REDO:
    {
      return AE_EditRedo(ae);
    }
    case EM_GETMODIFY:
    {
      return ae->ptxt->bModified;
    }
    case EM_SETMODIFY:
    {
      AE_SetModify(ae, (BOOL)wParam);
      return 0;
    }
    case EM_SETUNDOLIMIT:
    {
      ae->ptxt->dwUndoLimit=(DWORD)wParam;
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    case EM_EMPTYUNDOBUFFER:
    {
      AE_EmptyUndoBuffer(ae);
      return 0;
    }
    case EM_STOPGROUPTYPING:
    {
      AE_StackUndoGroupStop(ae);
      return 0;
    }

    //Text coordinates
    case EM_GETSEL:
    {
      CHARRANGE64 crRE;

      AE_RichEditGetSel(ae, &crRE.cpMin, &crRE.cpMax);
      if (wParam) *(int *)wParam=(int)crRE.cpMin;
      if (lParam) *(int *)lParam=(int)crRE.cpMax;

      if ((UINT)crRE.cpMin > 0xFFFF0000)
        return -1;
      if ((UINT)crRE.cpMax > 0xFFFF0000)
        return -1;
      return MAKELONG(crRE.cpMin, crRE.cpMax);
    }
    case EM_GETSEL64:
    {
      CHARRANGE64 crRE;
      INT_PTR nResult;

      nResult=AE_RichEditGetSel(ae, &crRE.cpMin, &crRE.cpMax);
      if (wParam) *(INT_PTR *)wParam=crRE.cpMin;
      if (lParam) *(INT_PTR *)lParam=crRE.cpMax;

      return nResult;
    }
    case EM_SETSEL:
    {
      AE_RichEditSetSel(ae, wParam, lParam);
      return 0;
    }
    case EM_EXGETSEL:
    {
      CHARRANGE64 crRE;

      AE_RichEditGetSel(ae, &crRE.cpMin, &crRE.cpMax);
      ((CHARRANGE *)lParam)->cpMin=(int)crRE.cpMin;
      ((CHARRANGE *)lParam)->cpMax=(int)crRE.cpMax;
      return 0;
    }
    case EM_EXGETSEL64:
    {
      CHARRANGE64 *crRE=(CHARRANGE64 *)lParam;

      return AE_RichEditGetSel(ae, &crRE->cpMin, &crRE->cpMax);
    }
    case EM_EXSETSEL:
    {
      CHARRANGE *crRE=(CHARRANGE *)lParam;

      AE_RichEditSetSel(ae, crRE->cpMin, crRE->cpMax);
      return ae->nSelEndCharOffset;
    }
    case EM_EXSETSEL64:
    {
      CHARRANGE64 *crRE=(CHARRANGE64 *)lParam;

      AE_RichEditSetSel(ae, crRE->cpMin, crRE->cpMax);
      return (ae->nSelEndCharOffset - ae->nSelStartCharOffset);
    }
    case EM_SELECTIONTYPE:
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
    case EM_GETLINECOUNT:
    {
      return ae->ptxt->nLineCount + 1;
    }
    case EM_GETFIRSTVISIBLELINE:
    {
      return AE_GetFirstVisibleLine(ae);
    }
    case EM_LINEFROMCHAR:
    {
      return AE_GetLineNumber(ae, AEGL_LINEFROMRICHOFFSET, wParam);
    }
    case EM_EXLINEFROMCHAR:
    {
      return AE_GetLineNumber(ae, AEGL_LINEFROMRICHOFFSET, lParam);
    }
    case EM_LINEINDEX:
    {
      return AE_GetRichOffset(ae, AEGI_RICHOFFSETFROMLINE, wParam);
    }
    case EM_LINELENGTH:
    {
      AECHARINDEX ci={0};

      if (wParam <= (UINT_PTR)ae->ptxt->nLastCharOffset)
      {
        AE_RichOffsetToAkelIndex(ae, wParam, &ci);
        if (ci.lpLine)
          return ci.lpLine->nLineLen;
      }
      return 0;
    }
    case EM_POSFROMCHAR:
    {
      AECHARINDEX ci={0};

      if (lParam || !IsBadWritePtr((void *)wParam, sizeof(POINT)))
      {
        //RichEdit 3.0 syntax
        POINT *ptClient=(POINT *)wParam;

        AE_RichOffsetToAkelIndex(ae, lParam, &ci);
        AE_GetPosFromChar(ae, &ci, NULL, ptClient);
        return 0;
      }
      else
      {
        //RichEdit 2.0 syntax
        POINT ptClient;

        AE_RichOffsetToAkelIndex(ae, wParam, &ci);
        AE_GetPosFromChar(ae, &ci, NULL, &ptClient);
        return MAKELONG(ptClient.x, ptClient.y);
      }
    }
    case EM_CHARFROMPOS:
    {
      POINT *ptClient=(POINT *)lParam;
      AECHARINDEX ci={0};

      AE_GetCharFromPos(ae, ptClient->x, ptClient->y, &ci, NULL, FALSE);
      return AE_AkelIndexToRichOffset(ae, &ci);
    }
    case EM_GETRECT:
    {
      RECT *rcDraw=(RECT *)lParam;

      *rcDraw=ae->rcDraw;
      return 0;
    }
    case EM_SETRECT:
    case EM_SETRECTNP:
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
        AE_StackCloneUpdate(ae);
      }
      AE_UpdateScrollBars(ae, SB_BOTH);
      AE_NotifySetRect(ae);
      return 0;
    }
    case EM_GETMARGINS:
    {
      return MAKELONG(ae->rcDraw.left - ae->rcEdit.left, ae->rcEdit.right - ae->rcDraw.right);
    }
    case EM_SETMARGINS:
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
        AE_StackCloneUpdate(ae);
      }
      AE_UpdateScrollBars(ae, SB_BOTH);
      AE_NotifySetRect(ae);
      return 0;
    }
    case EM_GETSCROLLPOS:
    {
      POINT *pt=(POINT *)lParam;

      pt->x=(int)ae->nHScrollPos;
      pt->y=(int)ae->nVScrollPos;
      return 1;
    }
    case EM_SETSCROLLPOS:
    {
      POINT *pt=(POINT *)lParam;

      AE_SetScrollPos(ae, pt->x, pt->y);
      return 1;
    }
    case EM_SCROLL:
    {
      INT_PTR nScrolled=0;
      BOOL bResult=FALSE;

      if (wParam == SB_LINEUP ||
          wParam == SB_LINEDOWN ||
          wParam == SB_PAGEUP ||
          wParam == SB_PAGEDOWN)
      {
        nScrolled=AE_VScroll(ae, (int)wParam, 0);
        bResult=TRUE;
      }
      return MAKELONG(mod(nScrolled) / ae->ptxt->nCharHeight, bResult);
    }
    case EM_LINESCROLL:
    {
      AE_VScrollLine(ae, (int)lParam, AESB_ALIGNTOP);
      return 0;
    }
    case EM_SCROLLCARET:
    {
      AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
      return 0;
    }
    case EM_GETTHUMB:
    {
      return ae->nVScrollPos;
    }
    case EM_FINDWORDBREAK:
    {
      AECHARINDEX ciCharIn;
      AECHARINDEX ciCharOut;
      DWORD dwFlags=0;

      if (wParam == WB_CLASSIFY)
        return 0;

      AE_RichOffsetToAkelIndex(ae, lParam, &ciCharIn);

      if (wParam == WB_ISDELIMITER)
        return AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ae->popt->nWordDelimitersLen, ciCharIn.lpLine->wpLine[ciCharIn.nCharInLine]);

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
    case EM_GETEVENTMASK:
    {
      return ae->popt->dwRichEventMask;
    }
    case EM_SETEVENTMASK:
    {
      DWORD dwPrevMask=ae->popt->dwRichEventMask;

      ae->popt->dwRichEventMask=(DWORD)lParam;
      return dwPrevMask;
    }
    case EM_GETOPTIONS:
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
    case EM_SETOPTIONS:
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
    case EM_SETREADONLY:
    {
      if (wParam)
        ae->popt->dwOptions|=AECO_READONLY;
      else
        ae->popt->dwOptions&=~AECO_READONLY;
      return 1;
    }
    case EM_SETBKGNDCOLOR:
    {
      AECOLORS aec;

      aec.dwFlags=AECLR_BASICBK;
      if (!wParam)
        aec.crBasicBk=(COLORREF)lParam;
      else
        aec.dwFlags|=AECLR_DEFAULT;

      AE_SetColors(ae, &aec, TRUE);
      return 0;
    }
    case EM_GETAUTOURLDETECT:
    {
      if (ae->popt->dwOptionsEx & AECOE_DETECTURL)
        return TRUE;
      return FALSE;
    }
    case EM_AUTOURLDETECT:
    {
      if (!wParam != !(ae->popt->dwOptionsEx & AECOE_DETECTURL))
      {
        if (wParam)
          ae->popt->dwOptionsEx|=AECOE_DETECTURL;
        else
          ae->popt->dwOptionsEx&=~AECOE_DETECTURL;
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        AE_StackCloneUpdate(ae);
      }
      return 0;
    }

    //Draw
    case EM_SHOWSCROLLBAR:
    {
      BOOL bUpdate=FALSE;

      if (wParam == SB_BOTH || wParam == SB_HORZ)
      {
        if (ae->bHScrollShow != lParam)
        {
          ae->bHScrollShow=(BOOL)lParam;
          bUpdate=TRUE;
        }
      }
      if (wParam == SB_BOTH || wParam == SB_VERT)
      {
        if (ae->bVScrollShow != lParam)
        {
          ae->bVScrollShow=(BOOL)lParam;
          bUpdate=TRUE;
        }
      }
      if (bUpdate)
        ShowScrollBar(ae->hWndEdit, (int)wParam, (BOOL)lParam);

      return 0;
    }
    case EM_HIDESELECTION:
    {
      if (wParam)
        ae->popt->dwOptions&=~AECO_NOHIDESEL;
      else
        ae->popt->dwOptions|=AECO_NOHIDESEL;
      AE_HideSelection(ae, (BOOL)wParam);
      return 0;
    }


    //// WM_* system messages

    case WM_GETFONT:
    {
      if (ae->ptxt->hFont == (HFONT)GetStockObject(SYSTEM_FONT))
        return (LRESULT)NULL;
      else
        return (LRESULT)ae->ptxt->hFont;
    }
    case WM_SETFONT:
    {
      POINT64 ptFirstVisLine;
      int nFirstVisibleLine=0;

      if (!ae->popt->nVScrollLock)
        nFirstVisibleLine=AE_GetFirstVisibleLine(ae);

      if (!ae->bUnicodeWindow)
        AE_SetEditFontA(ae, (HFONT)wParam, FALSE);
      else
        AE_SetEditFontW(ae, (HFONT)wParam, FALSE);

      ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
      AE_UpdateScrollBars(ae, SB_VERT);
      AE_CalcLinesWidth(ae, NULL, NULL, AECLW_FRESH);
      ae->ptCaret.x=0;
      ae->ptCaret.y=0;
      AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL|AESELT_LOCKNOTIFY);
      if (!ae->popt->nVScrollLock)
      {
        ptFirstVisLine.y=AE_VPosFromLine(ae, nFirstVisibleLine);
        AE_ScrollToPointEx(ae, AESC_POINTGLOBAL|AESC_OFFSETPIXELY|AESC_FORCETOP, &ptFirstVisLine, 0, 0);
      }
      AE_UpdateCaret(ae, ae->bFocus);

      if (ae->ptxt->dwWordWrap) AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, !lParam);
      AE_StackCloneUpdate(ae);
      return 0;
    }
    case WM_CUT:
    {
      AE_EditCut(ae, 0, AELB_ASOUTPUT);
      return 0;
    }
    case WM_COPY:
    {
      AE_EditCopyToClipboard(ae, NULL, AELB_ASOUTPUT, ae->bColumnSel);
      return 0;
    }
    case WM_PASTE:
    {
      AE_EditPasteFromClipboard(ae, 0, AELB_ASINPUT);
      return 0;
    }
    case WM_CLEAR:
    {
      AE_EditKeyDelete(ae, FALSE);
      return 0;
    }
    case WM_UNDO:
    {
      return AE_EditUndo(ae);
    }
    case WM_GETTEXTLENGTH:
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
    case WM_GETTEXT:
    {
      AECHARRANGE cr;
      UINT_PTR dwResult;

      AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin);
      AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax);

      if (!ae->bUnicodeWindow)
        dwResult=AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, (char *)lParam, wParam, AELB_ASOUTPUT, FALSE, FALSE);
      else
        dwResult=AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, (wchar_t *)lParam, wParam, AELB_ASOUTPUT, FALSE, FALSE);
      return dwResult;
    }
    case WM_SETTEXT:
    {
      if (!ae->bUnicodeWindow)
        AE_SetTextAnsi(ae, CP_ACP, (char *)lParam, (UINT_PTR)-1, AELB_ASINPUT);
      else
        AE_SetText(ae, (wchar_t *)lParam, (UINT_PTR)-1, AELB_ASINPUT, FALSE);
      return TRUE;
    }
    case WM_SIZE:
    {
      if (lParam)
      {
        AE_UpdateSize(ae);
      }
      return 0;
    }
    case WM_GETDLGCODE:
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
    case WM_IME_REQUEST:
    {
      if (wParam == IMR_CANDIDATEWINDOW)
      {
        CANDIDATEFORM *lpcf=(CANDIDATEFORM *)lParam;

        if (lpcf->dwStyle == CFS_CANDIDATEPOS)
        {
          AE_GlobalToClient(ae, &ae->ptCaret, NULL, &lpcf->ptCurrentPos);
          lpcf->ptCurrentPos.y+=ae->ptxt->nCharHeight;
          return 1;
        }
      }
      else if (wParam == IMR_COMPOSITIONFONT)
      {
        if (!ae->bUnicodeWindow)
          xmemcpy((LOGFONTA *)lParam, &ae->ptxt->lfFontA, sizeof(LOGFONTA));
        else
          xmemcpy((LOGFONTW *)lParam, &ae->ptxt->lfFontW, sizeof(LOGFONTW));
        return 1;
      }
      else if (wParam == IMR_COMPOSITIONWINDOW)
      {
        COMPOSITIONFORM *lpcf=(COMPOSITIONFORM *)lParam;

        if (lpcf->dwStyle == CFS_POINT)
        {
          AE_GlobalToClient(ae, &ae->ptCaret, NULL, &lpcf->ptCurrentPos);
          return 1;
        }
      }
      else if (wParam == IMR_RECONVERTSTRING)
      {
        RECONVERTSTRING *lprs=(RECONVERTSTRING *)lParam;
        UINT_PTR dwChars;

        if (!ae->bColumnSel)
        {
          if (!lprs)
          {
            if (!ae->bUnicodeWindow)
            {
              if (dwChars=AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, AELB_ASOUTPUT, FALSE, FALSE))
                return sizeof(RECONVERTSTRING) + dwChars;
            }
            else
            {
              if (dwChars=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, AELB_ASOUTPUT, FALSE, FALSE))
                return sizeof(RECONVERTSTRING) + dwChars * sizeof(wchar_t);
            }
          }
          else
          {
            if (!ae->bUnicodeWindow)
              dwChars=AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (char *)((BYTE *)lprs + sizeof(RECONVERTSTRING)), lprs->dwSize - sizeof(RECONVERTSTRING), AELB_ASOUTPUT, FALSE, FALSE);
            else
              dwChars=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)((BYTE *)lprs + sizeof(RECONVERTSTRING)), lprs->dwSize - sizeof(RECONVERTSTRING), AELB_ASOUTPUT, FALSE, FALSE);

            if (dwChars)
            {
              lprs->dwVersion=0;
              lprs->dwStrLen=(DWORD)dwChars;
              lprs->dwStrOffset=sizeof(RECONVERTSTRING);
              lprs->dwCompStrLen=(DWORD)dwChars;
              lprs->dwCompStrOffset=0;
              lprs->dwTargetStrLen=lprs->dwCompStrLen;
              lprs->dwTargetStrOffset=lprs->dwCompStrOffset;

              if (!ae->bUnicodeWindow)
                return sizeof(RECONVERTSTRING) + dwChars;
              else
                return sizeof(RECONVERTSTRING) + dwChars * sizeof(wchar_t);
            }
          }
        }
        return 0;
      }
      break;
    }
    case WM_IME_STARTCOMPOSITION:
    {
      HIMC hImc;

      if (PRIMARYLANGID((UINT_PTR)GetKeyboardLayout(0)) == LANG_KOREAN)
        return 0;
      ae->bImeComposition=TRUE;

      if (hImc=ImmGetContext(ae->hWndEdit))
      {
        AE_UpdateCompositionPos(ae, hImc);
        if (!ae->bUnicodeWindow)
          ImmSetCompositionFontA(hImc, &ae->ptxt->lfFontA);
        else
          ImmSetCompositionFontW(hImc, &ae->ptxt->lfFontW);

        ImmReleaseContext(ae->hWndEdit, hImc);
      }
      break;
    }
    case WM_IME_COMPOSITION:
    {
      if (!AE_IsReadOnly(ae))
      {
        if (PRIMARYLANGID((UINT_PTR)GetKeyboardLayout(0)) == LANG_KOREAN)
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
                    AE_ReplaceSel(ae, wszCompStr, nStrLen / sizeof(wchar_t), AELB_ASINPUT, 0, NULL, NULL);

                  if (ae->popt->dwOptions & AECO_DETAILEDUNDO)
                  {
                    ae->ptxt->bLockGroupStopInt=FALSE;
                    AE_StackUndoGroupStop(ae);
                    ae->ptxt->bLockGroupStopInt=TRUE;
                  }
                }
                else
                {
                  if (AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
                    AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ae->ciSelEndIndex, FALSE, 0, AESCT_IME);
                }
              }

              if ((nStrLen=ImmGetCompositionStringW(hImc, GCS_COMPSTR, wszCompStr, 2 * sizeof(wchar_t))) > 0)
              {
                AE_EditChar(ae, wszCompStr[0], TRUE);
                ae->dwImeChar=wszCompStr[0];

                ciSelStart=ae->ciSelStartIndex;
                AEC_IndexDec(&ciSelStart);
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
                    ae->dwImeChar=(DWORD)wParam;

                    ciSelStart=ae->ciSelStartIndex;
                    AEC_IndexDec(&ciSelStart);
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
      break;
    }
    case WM_IME_ENDCOMPOSITION:
    {
      ae->bImeComposition=FALSE;
      break;
    }
    case WM_IME_NOTIFY:
    {
      if (PRIMARYLANGID((UINT_PTR)GetKeyboardLayout(0)) == LANG_KOREAN)
      {
        if (wParam == IMN_OPENCANDIDATE)
        {
          AE_UpdateCandidatePos(ae, 0);
        }
      }
      break;
    }
    case WM_IME_KEYDOWN:
    {
      if (PRIMARYLANGID((UINT_PTR)GetKeyboardLayout(0)) == LANG_KOREAN)
      {
        if (wParam == VK_HANJA)
        {
          AECHARRANGE crSel;
          HIMC hImc;

          if (hImc=ImmGetContext(ae->hWndEdit))
          {
            crSel.ciMax=ae->ciSelStartIndex;
            AEC_ValidCharInLine(&crSel.ciMax);
            AEC_NextCharEx(&crSel.ciMax, &crSel.ciMax);
            AEC_PrevCharEx(&crSel.ciMax, &crSel.ciMin);
            ae->dwImeChar=*(crSel.ciMin.lpLine->wpLine + crSel.ciMin.nCharInLine);

            if (ImmEscapeW(GetKeyboardLayout(0), hImc, IME_ESC_HANJA_MODE, &ae->dwImeChar))
            {
              AE_SetSelectionPos(ae, &crSel.ciMax, &crSel.ciMin, FALSE, 0, AESCT_IME);
              AE_UpdateCandidatePos(ae, hImc);
            }
            ImmReleaseContext(ae->hWndEdit, hImc);
          }
        }
      }
      break;
    }
    case WM_IME_CHAR:
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
    case WM_HSCROLL:
    {
      if (ae->popt->dwRichEventMask & ENM_SCROLLEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      AE_HScroll(ae, LOWORD(wParam), 0);
      return 0;
    }
    case WM_VSCROLL:
    {
      if (ae->popt->dwRichEventMask & ENM_SCROLLEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      AE_VScroll(ae, LOWORD(wParam), 0);
      return 0;
    }
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    case WM_CAPTURECHANGED:
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
        int nLineCapture=0;

        if (GetKeyState(VK_MENU) < 0)
          bAlt=TRUE;
        if (wParam & MK_SHIFT)
          bShift=TRUE;
        if (wParam & MK_CONTROL)
          bControl=TRUE;

        if (ae->nMouseActive == AEMA_IGNORE)
        {
          ae->nMouseActive=AEMA_NONE;
          return 0;
        }
        ae->nMouseActive=AEMA_NONE;

        GetCursorPos(&ptPos);
        ScreenToClient(ae->hWndEdit, &ptPos);

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

          //Marker movement capture
          if (!bAlt && bShift && ae->nCurrentCursor == AECC_MARKER)
          {
            if (!ae->dwMouseMoveTimer)
            {
              //Timer
              ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
              AE_SetMouseCapture(ae, AEMSC_MOUSEMARKER);

              ae->dwInitMarkerPos=ae->popt->dwColumnMarkerPos;
            }
          }
          //Start margin selection capture
          else if (!bAlt && !bShift && ae->nCurrentCursor == AECC_MARGIN)
          {
            nLineCapture=AESCT_MOUSELEFTMARGIN|AESCT_MOUSESINGLECLK;
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
          if (ae->nCurrentCursor == AECC_MARKER)
          {
            //Two clicks on column marker are ignored
          }
          else if (ae->nCurrentCursor == AECC_MARGIN)
          {
            //Two clicks on left margin are ignored
          }
          else
          {
            AECHARINDEX ciPrevWord;
            AECHARINDEX ciNextWord;
            AECHARINDEX ciCharIndex;
            int nPosResult;

            if (!ae->dwMouseMoveTimer)
            {
              //Timer
              ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
              AE_SetMouseCapture(ae, AEMSC_MOUSEMOVE);
            }
            nPosResult=AE_GetCharFromPos(ae, ptPos.x, ptPos.y, &ciCharIndex, NULL, ae->bColumnSel);

            if ((ae->ciCaretIndex.lpLine->nLineLen && !ae->ciCaretIndex.nCharInLine) ||
                !AE_GetPrevBreak(ae, &ae->ciCaretIndex, &ciPrevWord, ae->bColumnSel, AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND|AEWB_STOPSPACESTART|AEWB_STOPSPACEEND|(ae->ciCaretIndex.lpLine->nLineLen?AEWB_STOPNEWLINE:0)|(nPosResult == AEPC_BEFORE?AEWB_MINMOVE:0)))
            {
              ciPrevWord=ae->ciCaretIndex;
            }
            if (!AE_GetNextBreak(ae, &ciPrevWord, &ciNextWord, ae->bColumnSel, AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND|(bControl?AEWB_SKIPSPACESTART:AEWB_STOPSPACESTART)|AEWB_STOPSPACEEND|(ae->ciCaretIndex.lpLine->nLineLen?AEWB_STOPNEWLINE:0)))
            {
              ciNextWord=ae->ciCaretIndex;
            }
            if (nPosResult == AEPC_AFTER)
              AEC_PrevCharEx(&ae->ciCaretIndex, &ae->ciMouseSelClick);
            else
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
          if (ae->nCurrentCursor == AECC_MARKER)
          {
            //Three clicks on column marker are ignored
          }
          else if (ae->nCurrentCursor == AECC_MARGIN)
          {
            if (!bAlt && !bShift)
            {
              AE_EditSelectAll(ae, 0, AESCT_MOUSELEFTMARGIN|AESCT_MOUSETRIPLECLK);
            }
          }
          else
          {
            nLineCapture=AESCT_MOUSETRIPLECLK;
          }
        }

        if (nLineCapture)
        {
          AECHARRANGE cr;
          AECHARINDEX ciCharIndex;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            AE_SetMouseCapture(ae, AEMSC_MOUSEMOVE);
          }

          AE_GetCharFromPos(ae, ptPos.x, ptPos.y, &ciCharIndex, NULL, ae->bColumnSel);
          if (ae->popt->dwOptions & AECO_SELUNWRAPLINE)
          {
            AEC_WrapLineBeginEx(&ciCharIndex, &cr.ciMin);
            AEC_WrapLineEndEx(&ciCharIndex, &cr.ciMax);
          }
          else
          {
            cr.ciMin=ciCharIndex;
            cr.ciMax=ciCharIndex;
            cr.ciMin.nCharInLine=0;
            cr.ciMax.nCharInLine=cr.ciMax.lpLine->nLineLen;
          }
          if (!(ae->popt->dwOptions & AECO_NONEWLINEMOUSESELECT))
            AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDLINE, &cr.ciMax, &cr.ciMax);

          ae->ciMouseSelClick=ciCharIndex;
          ae->ciMouseSelStart=cr.ciMin;
          ae->ciMouseSelEnd=cr.ciMax;
          ae->dwMouseSelType=AEMSS_LINES;
          AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, FALSE, AESELT_MOUSE, nLineCapture);
        }
        return 0;
      }
      else if (uMsg == WM_RBUTTONDOWN)
      {
        if (ae->popt->dwOptions & AECO_RBUTTONDOWNMOVECARET)
        {
          POINT ptPos;

          GetCursorPos(&ptPos);
          ScreenToClient(ae->hWndEdit, &ptPos);

          if (!(ae->popt->dwOptions & AECO_DISABLEDRAG) ?
              ae->nCurrentCursor != AECC_SELECTION :
              !AE_IsPointOnSelection(ae, ptPos.x, ptPos.y))
          {
            ae->dwMouseSelType=AEMSS_CHARS;
            AE_SetMouseSelection(ae, &ptPos, ae->bColumnSel, FALSE);
          }
        }
      }
      else if (uMsg == WM_MBUTTONDOWN)
      {
        if (!(ae->popt->dwOptions & AECO_MBUTTONDOWNNOSCROLL) && !ae->bMButtonDown)
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
                      if (!AE_IsReadOnly(ae))
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
          AE_ReleaseMouseCapture(ae, AEMSC_MOUSEMOVE|AEMSC_MOUSEMARKER);
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
          AE_ReleaseMouseCapture(ae, AEMSC_MOUSEMOVE|AEMSC_MOUSEMARKER);
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
      break;
    }
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    {
      if (ae->popt->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      if (uMsg == WM_MOUSEWHEEL)
      {
        DWORD dwLines=0;

        SystemParametersInfoA(SPI_GETWHEELSCROLLLINES, 0, &dwLines, 0);
        if (!dwLines) dwLines=3;

        if (!(ae->popt->dwOptionsEx & AECOE_INVERTVERTWHEEL) ? (short)HIWORD(wParam) < 0 : (short)HIWORD(wParam) >= 0)
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
      else
      {
        DWORD dwChars=0;

        SystemParametersInfoA(SPI_GETWHEELSCROLLCHARS, 0, &dwChars, 0);
        if (!dwChars) dwChars=3;

        if (!(ae->popt->dwOptionsEx & AECOE_INVERTHORZWHEEL) ? (short)HIWORD(wParam) < 0 : (short)HIWORD(wParam) >= 0)
        {
          if (dwChars == (DWORD)-1)
            AE_HScroll(ae, SB_PAGELEFT, 0);
          else
            AE_HScrollChar(ae, -(int)dwChars, AESB_ALIGNLEFT);
        }
        else
        {
          if (dwChars == (DWORD)-1)
            AE_HScroll(ae, SB_PAGERIGHT, 0);
          else
            AE_HScrollChar(ae, dwChars, AESB_ALIGNLEFT);
        }
        return 1;
      }
      break;
    }
    case WM_SETCURSOR:
    {
      if (!ae->bDragging && !ae->dwMouseMoveTimer && !ae->dwMouseScrollTimer)
      {
        ae->nCurrentCursor=AE_SetCursor(ae);

        if (ae->nCurrentCursor == AECC_URL)
          AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink);
        if (ae->nCurrentCursor != AECC_IBEAM)
          return TRUE;
      }
      break;
    }
    case WM_TIMER:
    {
      if (wParam == AETIMERID_MOUSEMOVE ||
          wParam == AETIMERID_MOUSESCROLL)
      {
        AE_MouseMove(ae);
      }
      return 0;
    }
    case WM_SETFOCUS:
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
    case WM_KILLFOCUS:
    {
      AE_ImeComplete(ae);

      if (ae->nMouseActive == AEMA_LBUTTONDOWN)
      {
        //We lost focus, so ignore next WM_LBUTTONDOWN message posted in WM_MOUSEACTIVATE
        ae->nMouseActive=AEMA_IGNORE;
      }
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
    case WM_MOUSEACTIVATE:
    {
      LRESULT lResult;

      if (!ae->bUnicodeWindow)
        lResult=DefWindowProcA(ae->hWndEdit, uMsg, wParam, lParam);
      else
        lResult=DefWindowProcW(ae->hWndEdit, uMsg, wParam, lParam);

      if (lResult != MA_ACTIVATEANDEAT &&
          lResult != MA_NOACTIVATEANDEAT &&
          HIWORD(lParam) == WM_LBUTTONDOWN)
      {
        ae->nMouseActive=AEMA_LBUTTONDOWN;
      }
      return lResult;
    }
    case WM_DROPFILES:
    {
      if (!AE_NotifyDropFiles(ae, (HDROP)wParam))
        return 0;
      break;
    }
    case WM_SYSCOLORCHANGE:
    {
      if (ae->popt->bDefaultColors)
      {
        AECOLORS aecDefault;

        aecDefault.dwFlags=AECLR_DEFAULT|AECLR_ALL;
        AE_SetColors(ae, &aecDefault, TRUE);
        ae->popt->bDefaultColors=TRUE;
      }
      break;
    }
    case WM_ENABLE:
    {
      if (wParam)
      {
        AE_SetColors(ae, &ae->popt->aecEnable, TRUE);
      }
      else
      {
        AECOLORS aecDisable;
        COLORREF crDisableText=GetSysColor(COLOR_BTNTEXT);
        COLORREF crDisableBk=GetSysColor(COLOR_BTNFACE);

        //Backup colors
        xmemcpy(&ae->popt->aecEnable, &ae->popt->aec, sizeof(AECOLORS));
        ae->popt->aecEnable.dwFlags=AECLR_BASICTEXT|AECLR_BASICBK|AECLR_SELTEXT|AECLR_SELBK|AECLR_ACTIVELINETEXT|AECLR_ACTIVELINEBK;

        //Set disabled colors
        aecDisable.dwFlags=ae->popt->aecEnable.dwFlags;
        aecDisable.crBasicText=crDisableText;
        aecDisable.crBasicBk=crDisableBk;
        aecDisable.crSelText=crDisableText;
        aecDisable.crSelBk=crDisableBk;
        aecDisable.crActiveLineText=crDisableText;
        aecDisable.crActiveLineBk=crDisableBk;
        AE_SetColors(ae, &aecDisable, TRUE);
        if (ae->bFocus) SetFocus(NULL);
      }
      return 0;
    }
    case WM_ERASEBKGND:
    {
      AEERASE *lpErase=(AEERASE *)ae->hEraseStack.first;
      AEERASE *lpEraseNext;
      RECT rcErase;

      ae->popt->hbrBasicBk=CreateSolidBrush(ae->popt->aec.crBasicBk);

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
            rcErase.left=max(rcErase.left, ae->rcErase.left);
            rcErase.right=min(rcErase.right, ae->rcDraw.left);
            AE_FillRect(ae, (HDC)wParam, &rcErase, ae->popt->hbrBasicBk);
            lpErase->rcErase.left=rcErase.right;
          }
          if (lpErase->rcErase.top < ae->rcDraw.top)
          {
            rcErase=lpErase->rcErase;
            rcErase.top=max(rcErase.top, ae->rcErase.top);
            rcErase.bottom=min(rcErase.bottom, ae->rcDraw.top);
            AE_FillRect(ae, (HDC)wParam, &rcErase, ae->popt->hbrBasicBk);
            lpErase->rcErase.top=rcErase.bottom;
          }
          if (lpErase->rcErase.right > ae->rcDraw.right)
          {
            rcErase=lpErase->rcErase;
            rcErase.left=max(rcErase.left, ae->rcDraw.right);
            rcErase.right=min(rcErase.right, ae->rcErase.right);
            AE_FillRect(ae, (HDC)wParam, &rcErase, ae->popt->hbrBasicBk);
            lpErase->rcErase.right=rcErase.left;
          }
          if (lpErase->rcErase.bottom > ae->rcDraw.bottom)
          {
            rcErase=lpErase->rcErase;
            rcErase.top=max(rcErase.top, ae->rcDraw.bottom);
            rcErase.bottom=min(rcErase.bottom, ae->rcErase.bottom);
            AE_FillRect(ae, (HDC)wParam, &rcErase, ae->popt->hbrBasicBk);
            lpErase->rcErase.bottom=rcErase.top;
          }

          //Erase only a space after the last line
          rcErase=lpErase->rcErase;
          rcErase.top=max(rcErase.top, ae->rcDraw.top + (int)(ae->ptxt->nVScrollMax - ae->nVScrollPos));

          if (rcErase.top < rcErase.bottom)
          {
            AE_FillRect(ae, (HDC)wParam, &rcErase, ae->popt->hbrBasicBk);

            //Draw column marker on non-text lines
            if (!(ae->popt->dwOptions & AECO_NOMARKERAFTERLASTLINE))
              AE_ColumnMarkerDraw(ae, (HDC)wParam, rcErase.top, rcErase.bottom);

            //Draw active column on non-text lines
            if ((ae->popt->dwOptions & AECO_ACTIVECOLUMN) && ae->ptActiveColumnDraw.x >= rcErase.left && ae->ptActiveColumnDraw.x <= rcErase.right)
            {
              //If ptActiveColumnDraw.y is under the last line, draw active column without skiping caret space
              if (rcErase.top <= ae->ptActiveColumnDraw.y + ae->ptxt->nCharHeight)
                ae->ptActiveColumnDraw.y=-0x7fffffff;
              AE_ActiveColumnDraw(ae, (HDC)wParam, rcErase.top, rcErase.bottom);
            }
          }
        }

        //Next erase rectangle
        lpEraseNext=lpErase->next;
        AE_StackEraseDelete(ae, lpErase);
        lpErase=lpEraseNext;
      }
      DeleteObject(ae->popt->hbrBasicBk);
      ae->popt->hbrBasicBk=NULL;
      return 1;
    }
    case WM_NCPAINT:
    case WM_PAINT:
    {
      RECT rcUpdate;

      if (GetUpdateRect(ae->hWndEdit, &rcUpdate, FALSE))
      {
        if (AE_StackEraseMore(ae, &rcUpdate))
          AE_StackEraseInsert(ae, &rcUpdate);

        //Get DC
        if (uMsg == WM_PAINT)
        {
          HDC hDC=ae->hDC;
          HFONT hFontOld=NULL;

          if (!hDC)
          {
            if (ae->hDC=GetDC(ae->hWndEdit))
              if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(ae->hDC, ae->ptxt->hFont);
          }
          if (ae->hDC)
          {
            AE_Paint(ae, &rcUpdate);

            if (ae->bMButtonDown)
            {
              AE_MButtonErase(ae);
              ae->bMButtonDown=FALSE;
              UpdateWindow(ae->hWndEdit);
              ae->bMButtonDown=TRUE;
              AE_MButtonDraw(ae);
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
      }
      break;
    }
    case WM_DESTROY:
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
  }

  if (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)
  {
    if (ae->popt->dwRichEventMask & ENM_KEYEVENTS)
      if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
        return 0;

    if (uMsg == WM_KEYDOWN ||
        uMsg == WM_SYSKEYDOWN)
    {
      BOOL bAlt=FALSE;
      BOOL bShift=FALSE;
      BOOL bControl=FALSE;
      BOOL bResult;

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
                if ((ae->popt->dwOptionsEx & AECOE_ALTDECINPUT) && ae->nAltChar != AEAC_NUMPAD0)
                  ae->nAltChar=(int)wParam - VK_NUMPAD0;
                else
                  ae->nAltChar=AEAC_DODEFAULT;
              }
            }
            else
            {
              if (ae->nAltChar == AEAC_DECINPUT)
              {
                if (wParam != VK_NUMPAD0)
                  ae->nAltChar=(int)wParam - VK_NUMPAD0;
              }
              else ae->nAltChar=ae->nAltChar * 10 + ((int)wParam - VK_NUMPAD0);

              if (ae->nAltChar > 65536)
                ae->nAltChar=AEAC_KEYDOWN;
            }
          }
          else ae->nAltChar=AEAC_KEYDOWN;
        }
      }

      //Process virtual key
      ae->bBeepEnable=TRUE;
      bResult=AE_KeyDown(ae, (int)wParam, bAlt, bShift, bControl);
      ae->bBeepEnable=FALSE;
      if (bResult) return 0;
    }
    else if (uMsg == WM_CHAR)
    {
      LRESULT lResult=0;

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

      if (wParam == VK_RETURN)
      {
        AE_EditKeyReturn(ae);
        return 1;
      }
      if (wParam == VK_TAB || (wParam >= 0x20 && wParam != 0x7F))
      {
        AE_EditChar(ae, wParam, ae->bUnicodeWindow);
        return 1;
      }
    }
    else if (uMsg == WM_SYSCHAR ||
             uMsg == WM_DEADCHAR ||
             uMsg == WM_SYSDEADCHAR ||
             uMsg == WM_KEYUP ||
             uMsg == WM_SYSKEYUP)
    {
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
    ae->ptxt->dwTextLimit=(UINT_PTR)-1;
    ae->ptxt->dwUndoLimit=(DWORD)-1;
    ae->ptxt->nHideMinLineOffset=1;
    ae->ptxt->nHideMaxLineOffset=-1;

    ae->popt->aec.dwFlags=AECLR_DEFAULT|AECLR_ALL;
    AE_GetColors(ae, &ae->popt->aec);
    AE_SetColors(ae, &ae->popt->aec, FALSE);
    ae->popt->bDefaultColors=TRUE;
    ae->popt->nCaretInsertWidth=1;
    ae->popt->nCaretOvertypeHeight=2;
    ae->popt->dwUrlMaxLength=AEMAX_LINKLENGTH;
    ae->popt->dwWordBreak=AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND|AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND|AEWB_SKIPSPACESTART|AEWB_STOPSPACEEND;
    ae->bCaretVisible=TRUE;
    ae->nCurrentCursor=AECC_IBEAM;
    ae->popt->dwVScrollMaxOffset=0;
    ae->popt->dwMScrollSpeed=10;

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

    ae->iefVtbl.QueryInterface=AEIEnumFORMATETC_QueryInterface;
    ae->iefVtbl.AddRef=AEIEnumFORMATETC_AddRef;
    ae->iefVtbl.Release=AEIEnumFORMATETC_Release;
    ae->iefVtbl.Next=AEIEnumFORMATETC_Next;
    ae->iefVtbl.Skip=AEIEnumFORMATETC_Skip;
    ae->iefVtbl.Reset=AEIEnumFORMATETC_Reset;
    ae->iefVtbl.Clone=AEIEnumFORMATETC_Clone;

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

    //Detect RichEdit class
    {
      wchar_t wszRichEditClass[16];
      wchar_t *wpRichEditClass=wszRichEditClass;

      if (!ae->bUnicodeWindow)
        MultiByteToWideChar(CP_ACP, 0, (char *)cs->lpszClass, -1, wszRichEditClass, 16);
      else
        wpRichEditClass=(wchar_t *)cs->lpszClass;
      if (!xstrcmpiW(wpRichEditClass, AES_RICHEDIT20W) || !xstrcmpiW(wpRichEditClass, AES_RICHEDIT20A_UNICODE))
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

    if (cs->style & WS_HSCROLL)
      ae->bHScrollShow=TRUE;
    if (cs->style & WS_VSCROLL)
      ae->bVScrollShow=TRUE;
    if (cs->style & ES_READONLY)
      ae->popt->dwOptions|=AECO_READONLY;
    if (cs->style & ES_DISABLENOSCROLL)
      ae->popt->dwOptions|=AECO_DISABLENOSCROLL;
    if (cs->style & ES_NOHIDESEL)
      ae->popt->dwOptions|=AECO_NOHIDESEL;
    if (cs->style & ES_WANTRETURN)
      ae->popt->dwOptions|=AECO_WANTRETURN;
    if (cs->style & ES_MULTILINE)
      ae->popt->dwOptions|=AECO_MULTILINE;
    if (cs->style & ES_HEAPSERIALIZE)
      ae->popt->bHeapSerialize=TRUE;
    if (cs->style & ES_GLOBALUNDO)
      ae->aeUndo=NULL;
    else
      ae->aeUndo=ae;

    ae->popt->nWordDelimitersLen=(int)xarrcpynW(ae->popt->wszWordDelimiters, AES_WORDDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
    ae->ptxt->nWrapDelimitersLen=(int)xarrcpynW(ae->ptxt->wszWrapDelimiters, AES_WRAPDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
    ae->popt->nUrlLeftDelimitersLen=(int)xarrcpynW(ae->popt->wszUrlLeftDelimiters, AES_URLLEFTDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
    ae->popt->nUrlRightDelimitersLen=(int)xarrcpynW(ae->popt->wszUrlRightDelimiters, AES_URLRIGHTDELIMITERSW, AEMAX_DELIMLENGTH) - 2;
    ae->popt->nUrlPrefixesLen=(int)xarrcpynW(ae->popt->wszUrlPrefixes, AES_URLPREFIXESW, AEMAX_DELIMLENGTH);
    AE_GetUrlPrefixes(ae);

    //ae->rcEdit.left=cs->x;
    //ae->rcEdit.top=cs->y;
    //ae->rcEdit.right=cs->cx;
    //ae->rcEdit.bottom=cs->cy;
    GetClientRect(ae->hWndEdit, &ae->rcEdit);
    AE_SetDrawRect(ae, NULL, FALSE);
    ae->rcErase=ae->rcEdit;

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
  if (ae->hThreadMutex)
  {
    SetEvent(ae->hThreadMutex);
    CloseHandle(ae->hThreadMutex);
    ae->hThreadMutex=NULL;
  }

  if (!ae->lpMaster)
  {
    //Destroying master - uncloning all clones
    AE_StackCloneDeleteAll(ae);
  }
  else
  {
    //Destroying clone - uncloning current clone
    AECLONE *aec;

    if (aec=AE_StackCloneGet(ae->lpMaster, ae))
      AE_StackCloneDelete(aec);
  }

  if (ae->ptxt->hHeap)
  {
    if (HeapDestroy(ae->ptxt->hHeap))
    {
      ae->ptxt->hHeap=NULL;
      --nAkelEditHeapCount;
    }
    if (!ae->aeUndo)
      AE_EmptyUndoBuffer(ae);
  }
  else
  {
    AE_StackLineFree(ae);
    AE_EmptyUndoBuffer(ae);
    AE_UrlVisitFree(ae);
  }

  AE_HighlightDeleteTheme(ae, NULL);
  AE_StackFoldFree(ae);
  AE_StackPointFree(ae);
  AE_StackEraseFree(ae);

  AE_HeapStackDelete(NULL, (stack **)&hAkelEditWindowsStack.first, (stack **)&hAkelEditWindowsStack.last, (stack *)ae);
}

HANDLE AE_HeapCreate(AKELEDIT *ae)
{
  //HeapDestroy should do the job, but without AE_StackEraseFree BoundsChecker report memory leak
  AE_StackEraseFree(ae);

  //Free memory
  if (ae->ptxt->hHeap)
  {
    if (HeapDestroy(ae->ptxt->hHeap))
    {
      ae->ptxt->hHeap=NULL;
      --nAkelEditHeapCount;
    }
    if (!ae->aeUndo)
      AE_EmptyUndoBuffer(ae);
  }
  else
  {
    AE_StackLineFree(ae);
    AE_EmptyUndoBuffer(ae);
    AE_UrlVisitFree(ae);
  }

  //Reset variables
  ae->ptxt->hLinesStack.first=0;
  ae->ptxt->hLinesStack.last=0;
  ae->ptxt->hUndoStack.first=0;
  ae->ptxt->hUndoStack.last=0;
  ae->ptxt->lpCurrentUndo=NULL;
  ae->ptxt->lpSavePoint=NULL;
  ae->ptxt->bSavePointExist=TRUE;
  ae->ptxt->dwUndoCount=0;
  ae->ptxt->hUrlVisitStack.first=0;
  ae->ptxt->hUrlVisitStack.last=0;
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
  if (nAkelEditHeapCount < AEMAX_HEAPCREATE)
  {
    if (ae->ptxt->hHeap=HeapCreate(ae->popt->bHeapSerialize?0:HEAP_NO_SERIALIZE, 0, 0))
    {
      ++nAkelEditHeapCount;
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
  #ifdef _DEBUG
  element->prev=NULL;
  element->next=NULL;
  #endif
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

AKELEDIT* AE_StackWindowInsert(AESTACKEDIT *hStack)
{
  AKELEDIT *lpElement=NULL;

  AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AKELEDIT));
  return lpElement;
}

AKELEDIT* AE_StackWindowGet(AESTACKEDIT *hStack, HWND hWndEdit)
{
  AKELEDIT *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hWndEdit == hWndEdit)
      return lpElement;
  }
  return NULL;
}

BOOL AE_StackWindowValid(AESTACKEDIT *hStack, AKELEDIT *ae)
{
  AKELEDIT *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement == ae)
      return TRUE;
  }
  return FALSE;
}

void AE_StackWindowMakeFirst(AESTACKEDIT *hStack, AKELEDIT *ae)
{
  AE_HeapStackMoveBefore(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack *)ae, (stack *)hStack->first);
}

void AE_StackWindowFree(AESTACKEDIT *hStack)
{
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

void AE_CloneActivate(AKELEDIT *lpAkelEditPrev, AKELEDIT *ae)
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
        AE_ClearSelLines(&lpAkelEditPrev->ciSelStartIndex, &lpAkelEditPrev->ciSelEndIndex);
      }
    }
  }

  if (!lpAkelEditPrev || ae->hWndEdit != lpAkelEditPrev->hWndEdit)
  {
    //Set current window info
    if (ae->nCloneCount > 0 || ae->lpMaster)
    {
      AE_CloneRestoreSelection(ae);
    }
  }
}

void AE_CloneRestoreSelection(AKELEDIT *ae)
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

    AE_UpdateSelection(ae, (ae->dwMouseSelType?AESELT_MOUSE:0)|AESELT_COLUMNASIS|AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING|AESELT_RESETSELECTION);
  }
}

AECLONE* AE_StackCloneIndex(AKELEDIT *ae, DWORD dwIndex)
{
  AECLONE *lpElement;

  for (lpElement=(AECLONE *)ae->hClonesStack.first; lpElement; lpElement=lpElement->next)
  {
    if (dwIndex-- == 0)
      break;
  }
  return lpElement;
}

AECLONE* AE_StackCloneGet(AKELEDIT *aeMaster, AKELEDIT *aeClone)
{
  AECLONE *lpElement;

  for (lpElement=(AECLONE *)aeMaster->hClonesStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->aeClone == aeClone)
      break;
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

      //Copy selection and scroll info
      xmemcpy(&aeClone->liFirstDrawLine, &aeMaster->liFirstDrawLine, (BYTE *)&aeMaster->lpEditProc - (BYTE *)&aeMaster->liFirstDrawLine);

      AE_UpdateScrollBars(aeClone, SB_BOTH);
    }
  }
  return aec;
}

void AE_StackCloneDelete(AECLONE *aec)
{
  AKELEDIT *aeMaster=aec->aeClone->lpMaster;
  AKELEDIT *aeClone=aec->aeClone;

  if (aeMaster)
  {
    --aeMaster->nCloneCount;
    aeClone->lpMaster=NULL;
    AE_HeapStackDelete(NULL, (stack **)&aeMaster->hClonesStack.first, (stack **)&aeMaster->hClonesStack.last, (stack *)aec);

    if (aeClone->lpSelStartPoint && aeClone->lpSelEndPoint)
    {
      //Clear lines selection
      AE_ClearSelLines(&aeClone->lpSelStartPoint->ciPoint, &aeClone->lpSelEndPoint->ciPoint);
    }
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
    xmemset(&aeClone->liFirstDrawLine, 0, (BYTE *)&aeClone->lpEditProc - (BYTE *)&aeClone->liFirstDrawLine);
    AE_GetIndex(aeClone, AEGI_FIRSTCHAR, NULL, &aeClone->ciSelStartIndex);
    AE_GetIndex(aeClone, AEGI_FIRSTCHAR, NULL, &aeClone->ciSelEndIndex);
    AE_GetIndex(aeClone, AEGI_FIRSTCHAR, NULL, &aeClone->ciCaretIndex);
    AE_UpdateScrollBars(aeClone, SB_BOTH);

    if (!aeMaster->nCloneCount)
    {
      //Last clone deleted - update selection of master window
      AE_CloneRestoreSelection(aeMaster);
      AE_UpdateSelection(aeMaster, AESELT_COLUMNASIS|AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING|AESELT_RESETSELECTION);
    }
  }
}

void AE_StackCloneDeleteAll(AKELEDIT *ae)
{
  AECLONE *lpElement;
  AECLONE *lpTmp;

  for (lpElement=(AECLONE *)ae->hClonesStack.first; lpElement; lpElement=lpTmp)
  {
    lpTmp=lpElement->next;
    AE_StackCloneDelete(lpElement);
  }
}

void AE_StackCloneUpdate(AKELEDIT *ae)
{
  AKELEDIT *aeMaster;
  AECLONE *lpElement;

  //Get master
  if (ae->lpMaster)
    aeMaster=ae->lpMaster;
  else
    aeMaster=ae;

  //Update master
  if (ae != aeMaster)
  {
    AE_UpdateScrollBars(aeMaster, SB_BOTH);
    InvalidateRect(aeMaster->hWndEdit, &aeMaster->rcDraw, TRUE);
  }

  //Update clones
  for (lpElement=(AECLONE *)aeMaster->hClonesStack.first; lpElement; lpElement=lpElement->next)
  {
    if (ae != lpElement->aeClone)
    {
      AE_UpdateScrollBars(lpElement->aeClone, SB_BOTH);
      InvalidateRect(lpElement->aeClone->hWndEdit, &lpElement->aeClone->rcDraw, TRUE);
    }
  }
}

AKELEDIT* AE_StackDraggingGet(AKELEDIT *ae)
{
  AECLONE *lpElement;

  if (ae->bDragging) return ae;
  if (ae->lpMaster) ae=ae->lpMaster;
  if (ae->bDragging) return ae;

  for (lpElement=(AECLONE *)ae->hClonesStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->aeClone->bDragging)
      return lpElement->aeClone;
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

BOOL AE_StackEraseMore(AKELEDIT *ae, RECT *rcErase)
{
  AEERASE *lpElement;

  for (lpElement=(AEERASE *)ae->hEraseStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->rcErase.left <= rcErase->left &&
        lpElement->rcErase.top <= rcErase->top &&
        lpElement->rcErase.right >= rcErase->right &&
        lpElement->rcErase.bottom >= rcErase->bottom)
      return FALSE;
  }
  return TRUE;
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
  AEFONTITEMA *lpElement;

  for (lpElement=(AEFONTITEMA *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->lfFont.lfHeight == lfFont->lfHeight &&
        lpElement->lfFont.lfWeight == lfFont->lfWeight &&
        lpElement->lfFont.lfItalic == lfFont->lfItalic &&
        lpElement->lfFont.lfCharSet == lfFont->lfCharSet)
    {
      if (!xstrcmpiA(lpElement->lfFont.lfFaceName, lfFont->lfFaceName))
        return lpElement;
    }
  }
  return NULL;
}

AEFONTITEMW* AE_StackFontItemGetW(HSTACK *hStack, LOGFONTW *lfFont)
{
  AEFONTITEMW *lpElement;

  for (lpElement=(AEFONTITEMW *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->lfFont.lfHeight == lfFont->lfHeight &&
        lpElement->lfFont.lfWeight == lfFont->lfWeight &&
        lpElement->lfFont.lfItalic == lfFont->lfItalic &&
        lpElement->lfFont.lfCharSet == lfFont->lfCharSet)
    {
      if (!xstrcmpiW(lpElement->lfFont.lfFaceName, lfFont->lfFaceName))
        return lpElement;
    }
  }
  return NULL;
}

void AE_StackFontItemsFreeA(HSTACK *hStack)
{
  AEFONTITEMA *lpElement;

  for (lpElement=(AEFONTITEMA *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hFontNormal) DeleteObject(lpElement->hFontNormal);
    if (lpElement->hFontBold) DeleteObject(lpElement->hFontBold);
    if (lpElement->hFontItalic) DeleteObject(lpElement->hFontItalic);
    if (lpElement->hFontBoldItalic) DeleteObject(lpElement->hFontBoldItalic);
    if (lpElement->hFontUrl) DeleteObject(lpElement->hFontUrl);
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

void AE_StackFontItemsFreeW(HSTACK *hStack)
{
  AEFONTITEMW *lpElement;

  for (lpElement=(AEFONTITEMW *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hFontNormal) DeleteObject(lpElement->hFontNormal);
    if (lpElement->hFontBold) DeleteObject(lpElement->hFontBold);
    if (lpElement->hFontItalic) DeleteObject(lpElement->hFontItalic);
    if (lpElement->hFontBoldItalic) DeleteObject(lpElement->hFontBoldItalic);
    if (lpElement->hFontUrl) DeleteObject(lpElement->hFontUrl);
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

AEBITMAPITEM* AE_StackBitmapItemInsert(HSTACK *hStack, AEBITMAPDATA *bd)
{
  AEBITMAPITEM *lpElement=NULL;

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AEBITMAPITEM)))
  {
    xmemcpy(&lpElement->bd, bd, sizeof(AEBITMAPDATA));

    lpElement->hBitmap=AE_CreateBitmap(bd->nWidth, bd->nHeight, bd->crBasic, bd->crInvert);

    return lpElement;
  }
  return NULL;
}

AEBITMAPITEM* AE_StackBitmapItemGet(HSTACK *hStack, AEBITMAPDATA *bd)
{
  AEBITMAPITEM *lpElement;

  for (lpElement=(AEBITMAPITEM *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xmemcmp(&lpElement->bd, bd, sizeof(AEBITMAPDATA)))
      return lpElement;
  }
  return NULL;
}

void AE_StackBitmapItemsFree(HSTACK *hStack)
{
  AEBITMAPITEM *lpElement;

  for (lpElement=(AEBITMAPITEM *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hBitmap) DeleteObject(lpElement->hBitmap);
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

AEDCITEM* AE_StackDcItemInsert(HSTACK *hStack, HBITMAP hBitmap)
{
  AEDCITEM *lpElement=NULL;
  BITMAP bmpImage;

  //Get AlphaBlend address
  if (!AkelEditAlphaBlendPtr)
  {
    if (!(hAkelEditMsimg32=GetModuleHandleA("msimg32.dll")))
      if (hAkelEditMsimg32=LoadLibraryA("msimg32.dll"))
        bAkelEditMsimg32Free=TRUE;
    if (hAkelEditMsimg32)
      AkelEditAlphaBlendPtr=(BOOL (WINAPI *)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION))GetProcAddress(hAkelEditMsimg32, "AlphaBlend");
  }

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AEDCITEM)))
  {
    if (GetObjectA(hBitmap, sizeof(BITMAP), &bmpImage))
    {
      lpElement->hBitmap=hBitmap;
      lpElement->nBitmapX=bmpImage.bmWidth;
      lpElement->nBitmapY=bmpImage.bmHeight;
      if (lpElement->hDC=CreateCompatibleDC(NULL))
        lpElement->hBitmapOld=(HBITMAP)SelectObject(lpElement->hDC, lpElement->hBitmap);
    }
    return lpElement;
  }
  return NULL;
}

AEDCITEM* AE_StackDcItemGet(HSTACK *hStack, HBITMAP hBitmap)
{
  AEDCITEM *lpElement;

  for (lpElement=(AEDCITEM *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hBitmap == hBitmap)
      return lpElement;
  }
  return NULL;
}

void AE_StackDcItemDelete(HSTACK *hStack, AEDCITEM *lpElement)
{
  if (lpElement->hDC)
  {
    if (lpElement->hBitmapOld) SelectObject(lpElement->hDC, lpElement->hBitmapOld);
    DeleteDC(lpElement->hDC);
  }
  AE_HeapStackDelete(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement);
}

void AE_StackDcItemsFree(HSTACK *hStack)
{
  AEDCITEM *lpElement;

  for (lpElement=(AEDCITEM *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hDC)
    {
      if (lpElement->hBitmapOld) SelectObject(lpElement->hDC, lpElement->hBitmapOld);
      DeleteDC(lpElement->hDC);
    }
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

AEPENITEM* AE_StackPenItemInsert(HSTACK *hStack, COLORREF crPenColor, COLORREF crInvertColor)
{
  AEPENITEM *lpElement=NULL;

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(AEPENITEM)))
  {
    if (crInvertColor != (COLORREF)-1)
    {
      //Invert for R2_XORPEN
      BYTE r=GetRValue(crInvertColor) ^ GetRValue(crPenColor);
      BYTE g=GetGValue(crInvertColor) ^ GetGValue(crPenColor);
      BYTE b=GetBValue(crInvertColor) ^ GetBValue(crPenColor);

      crPenColor=RGB(r, g, b);
    }
    lpElement->crPenColor=crPenColor;
    lpElement->hPen=CreatePen(PS_SOLID, 0, crPenColor);

    return lpElement;
  }
  return NULL;
}

AEPENITEM* AE_StackPenItemGet(HSTACK *hStack, COLORREF crPenColor, COLORREF crInvertColor)
{
  AEPENITEM *lpElement;

  if (crInvertColor != (COLORREF)-1)
  {
    //Invert for R2_XORPEN
    BYTE r=GetRValue(crInvertColor) ^ GetRValue(crPenColor);
    BYTE g=GetGValue(crInvertColor) ^ GetGValue(crPenColor);
    BYTE b=GetBValue(crInvertColor) ^ GetBValue(crPenColor);

    crPenColor=RGB(r, g, b);
  }

  for (lpElement=(AEPENITEM *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->crPenColor == crPenColor)
      return lpElement;
  }
  return NULL;
}

void AE_StackPenItemsFree(HSTACK *hStack)
{
  AEPENITEM *lpElement;

  for (lpElement=(AEPENITEM *)hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hPen) DeleteObject(lpElement->hPen);
  }
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}


AEFOLD* AE_StackFoldInsert(AKELEDIT *ae, const AEFOLD *lpFold)
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

  if (AEC_IndexCompare(&lpFold->lpMinPoint->ciPoint, &lpFold->lpMaxPoint->ciPoint) > 0)
    return NULL;

  if (lpFold->lpMinPoint->nPointOffset == AEPTO_CALC)
    lpFold->lpMinPoint->nPointOffset=AE_AkelIndexToRichOffset(ae, &lpFold->lpMinPoint->ciPoint);
  if (lpFold->lpMaxPoint->nPointOffset == AEPTO_CALC)
    lpFold->lpMaxPoint->nPointOffset=AE_AkelIndexToRichOffset(ae, &lpFold->lpMaxPoint->ciPoint);

  //Fold start
  AE_StackFindFold(ae, AEFF_FINDOFFSET|AEFF_RECURSE, lpFold->lpMinPoint->nPointOffset, NULL, &lpMinParent, &lpMinPrevSubling);

  //Fold end
  if (lpMinParent && lpMinParent->lpMaxPoint->nPointOffset + lpMinParent->lpMaxPoint->nPointLen == lpFold->lpMaxPoint->nPointOffset + lpFold->lpMaxPoint->nPointLen)
  {
    lpMaxParent=lpMinParent;
    lpMaxPrevSubling=lpMinParent->lastChild;
  }
  else AE_StackFindFold(ae, AEFF_FINDOFFSET|AEFF_RECURSE, lpFold->lpMaxPoint->nPointOffset + lpFold->lpMaxPoint->nPointLen, lpMinParent, &lpMaxParent, &lpMaxPrevSubling);

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
    if (lpNewElement->lpMinPoint=AE_StackPointInsert(ae, &lpFold->lpMinPoint->ciPoint))
    {
      lpNewElement->lpMinPoint->nPointOffset=lpFold->lpMinPoint->nPointOffset;
      lpNewElement->lpMinPoint->nPointLen=lpFold->lpMinPoint->nPointLen;
      lpNewElement->lpMinPoint->dwFlags=lpFold->lpMinPoint->dwFlags|AEPTF_FOLD|AEPTF_WRAPMOVE;
      lpNewElement->lpMinPoint->dwUserData=lpFold->lpMinPoint->dwUserData;
    }
    if (lpNewElement->lpMaxPoint=AE_StackPointInsert(ae, &lpFold->lpMaxPoint->ciPoint))
    {
      lpNewElement->lpMaxPoint->nPointOffset=lpFold->lpMaxPoint->nPointOffset;
      lpNewElement->lpMaxPoint->nPointLen=lpFold->lpMaxPoint->nPointLen;
      lpNewElement->lpMaxPoint->dwFlags=lpFold->lpMaxPoint->dwFlags|AEPTF_FOLD|AEPTF_WRAPMOVE;
      lpNewElement->lpMaxPoint->dwUserData=lpFold->lpMaxPoint->dwUserData;
    }
    lpNewElement->dwFlags=lpFold->dwFlags;
    lpNewElement->dwFontStyle=lpFold->dwFontStyle;
    lpNewElement->crText=lpFold->crText;
    lpNewElement->crBk=lpFold->crBk;
    lpNewElement->nParentID=lpFold->nParentID;
    lpNewElement->nRuleID=lpFold->nRuleID;
    lpNewElement->hRuleTheme=lpFold->hRuleTheme;
    lpNewElement->dwUserData=lpFold->dwUserData;

    if (lpNewElement->dwFontStyle != AEHLS_NONE ||
        lpNewElement->crText != (DWORD)-1 ||
        lpNewElement->crBk != (DWORD)-1)
    {
      lpNewElement->dwFlags|=AEFOLDF_STYLED;
      ++ae->ptxt->nFoldColorCount;
    }
    if (lpNewElement->nRuleID)
      ++ae->ptxt->nFoldWithIdCount;
    if (lpNewElement->hRuleTheme)
      ++ae->ptxt->nFoldWithThemeCount;
    ++ae->ptxt->nFoldAllCount;
    if (lpNewElement->dwFlags & AEFOLDF_COLLAPSED)
      ++ae->ptxt->nFoldCollapseCount;
  }
  ae->ptxt->lpVPosFold=NULL;
  return lpNewElement;
}

void AE_StackFindFold(AKELEDIT *ae, DWORD dwFlags, UINT_PTR dwFindIt, AEFOLD *lpForce, AEFOLD **lpParentOut, AEFOLD **lpPrevSublingOut)
{
  AEFOLD *lpParent=NULL;
  AEFOLD *lpPrevSubling=NULL;
  AEFOLD *lpPrevChild=NULL;
  AEFOLD *lpSubling=NULL;
  INT_PTR nFindOffset=0;
  INT_PTR nFindLine=0;
  UINT_PTR dwFirst;
  UINT_PTR dwSecond;
  UINT_PTR dwThird;
  UINT_PTR dwFourth;
  UINT_PTR dwFifth;
  BOOL bGoRoot=FALSE;

  if (ae->ptxt->hFoldsStack.first)
  {
    if (dwFlags & AEFF_FINDOFFSET)
      nFindOffset=(INT_PTR)dwFindIt;
    else if (dwFlags & AEFF_FINDINDEX)
      nFindOffset=AE_AkelIndexToRichOffset(ae, (const AECHARINDEX *)dwFindIt);
    else if (dwFlags & AEFF_FINDLINE)
      nFindLine=(INT_PTR)dwFindIt;
    else
      return;

    if (!lpForce)
    {
      dwFirst=(UINT_PTR)-1;
      dwSecond=(UINT_PTR)-1;
      dwThird=(UINT_PTR)-1;
      dwFourth=(UINT_PTR)-1;
      dwFifth=(UINT_PTR)-1;

      if (dwFlags & AEFF_FINDLINE)
      {
        dwFirst=mod(nFindLine - ae->ptxt->hFoldsStack.first->lpMinPoint->ciPoint.nLine);
        dwSecond=mod(nFindLine - ae->ptxt->hFoldsStack.last->lpMinPoint->ciPoint.nLine);
        if (ae->ptxt->lpVPosFold)
          dwThird=mod(nFindLine - ae->ptxt->lpVPosFold->lpMinPoint->ciPoint.nLine);
        if (ae->ptxt->lpIsCollapsedLastCall)
          dwFourth=mod(nFindLine - ae->ptxt->lpIsCollapsedLastCall->lpMinPoint->ciPoint.nLine);
        if (ae->ptxt->lpFindFoldLastCall)
          dwFifth=mod(nFindLine - ae->ptxt->lpFindFoldLastCall->lpMinPoint->ciPoint.nLine);
      }
      else
      {
        dwFirst=mod(nFindOffset - ae->ptxt->hFoldsStack.first->lpMinPoint->nPointOffset);
        dwSecond=mod(nFindOffset - ae->ptxt->hFoldsStack.last->lpMinPoint->nPointOffset);
        if (ae->ptxt->lpVPosFold)
          dwThird=mod(nFindOffset - ae->ptxt->lpVPosFold->lpMinPoint->nPointOffset);
        if (ae->ptxt->lpIsCollapsedLastCall)
          dwFourth=mod(nFindOffset - ae->ptxt->lpIsCollapsedLastCall->lpMinPoint->nPointOffset);
        if (ae->ptxt->lpFindFoldLastCall)
          dwFifth=mod(nFindOffset - ae->ptxt->lpFindFoldLastCall->lpMinPoint->nPointOffset);
      }

      if (dwFirst <= dwSecond && dwFirst <= dwThird && dwFirst <= dwFourth && dwFirst <= dwFifth)
      {
        lpSubling=ae->ptxt->hFoldsStack.first;
      }
      else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth)
      {
        lpSubling=ae->ptxt->hFoldsStack.last;
      }
      else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth && dwThird <= dwFifth)
      {
        lpSubling=ae->ptxt->lpVPosFold;
      }
      else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird && dwFourth <= dwFifth)
      {
        lpSubling=ae->ptxt->lpIsCollapsedLastCall;
      }
      else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth)
      {
        lpSubling=ae->ptxt->lpFindFoldLastCall;
      }
    }
    else lpSubling=lpForce;

    if (lpSubling->parent)
      bGoRoot=TRUE;

    FindDirection:
    if (!(dwFlags & AEFF_FINDLINE) ?
           //AEFF_FINDOFFSET or AEFF_FINDINDEX
           (nFindOffset > lpSubling->lpMinPoint->nPointOffset ||
            ((dwFlags & AEFF_FOLDSTART) && nFindOffset == lpSubling->lpMinPoint->nPointOffset)) :
           //AEFF_FINDLINE
           (nFindLine > lpSubling->lpMinPoint->ciPoint.nLine ||
            ((dwFlags & AEFF_FOLDSTART) && nFindLine == lpSubling->lpMinPoint->ciPoint.nLine)))
    {
      NextForward:
      while (lpSubling)
      {
        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               (nFindOffset < lpSubling->lpMinPoint->nPointOffset ||
                (!(dwFlags & AEFF_FOLDSTART) && nFindOffset == lpSubling->lpMinPoint->nPointOffset)) :
               //AEFF_FINDLINE
               (nFindLine < lpSubling->lpMinPoint->ciPoint.nLine ||
                (!(dwFlags & AEFF_FOLDSTART) && nFindLine == lpSubling->lpMinPoint->ciPoint.nLine)))
        {
          break;
        }

        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               (nFindOffset < lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen ||
                (((dwFlags & AEFF_FOLDEND) && nFindOffset == lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen) &&
                 (!(dwFlags & AEFF_FOLDSTART) || !lpSubling->next || nFindOffset != lpSubling->next->lpMinPoint->nPointOffset))) :
               //AEFF_FINDLINE
               (nFindLine < lpSubling->lpMaxPoint->ciPoint.nLine ||
                (((dwFlags & AEFF_FOLDEND) && nFindLine == lpSubling->lpMaxPoint->ciPoint.nLine) &&
                 (!(dwFlags & AEFF_FOLDSTART) || !lpSubling->next || nFindLine != lpSubling->next->lpMinPoint->ciPoint.nLine))))
        {
          lpParent=lpSubling;
          lpPrevSubling=NULL;
          if (!(dwFlags & AEFF_RECURSE) || !lpSubling->firstChild)
            break;
          goto FindChild;
        }

        //Next
        if (bGoRoot)
        {
          lpPrevChild=lpSubling;
          lpSubling=lpSubling->parent;
          if (!lpSubling->parent)
            bGoRoot=FALSE;
        }
        else
        {
          lpPrevChild=NULL;
          lpPrevSubling=lpSubling;
          lpSubling=lpSubling->next;
        }
      }
    }
    else
    {
      NextBackward:
      while (lpSubling)
      {
        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               (nFindOffset > lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen ||
                (!(dwFlags & AEFF_FOLDEND) && nFindOffset == lpSubling->lpMaxPoint->nPointOffset + lpSubling->lpMaxPoint->nPointLen)) :
               //AEFF_FINDLINE
               (nFindLine > lpSubling->lpMaxPoint->ciPoint.nLine ||
                (!(dwFlags & AEFF_FOLDEND) && nFindLine == lpSubling->lpMaxPoint->ciPoint.nLine)))
        {
          break;
        }

        if (!(dwFlags & AEFF_FINDLINE) ?
               //AEFF_FINDOFFSET or AEFF_FINDINDEX
               (nFindOffset > lpSubling->lpMinPoint->nPointOffset ||
                ((dwFlags & AEFF_FOLDSTART) && nFindOffset == lpSubling->lpMinPoint->nPointOffset)) :
               //AEFF_FINDLINE
               (nFindLine > lpSubling->lpMinPoint->ciPoint.nLine ||
                ((dwFlags & AEFF_FOLDSTART) && nFindLine == lpSubling->lpMinPoint->ciPoint.nLine)))
        {
          lpParent=lpSubling;
          if (!(dwFlags & AEFF_RECURSE) || !lpSubling->firstChild)
          {
            lpSubling=NULL;
            break;
          }
          lpPrevSubling=NULL;
          goto FindChild;
        }

        //Next
        if (bGoRoot)
        {
          lpPrevChild=lpSubling;
          lpSubling=lpSubling->parent;
          if (!lpSubling->parent)
            bGoRoot=FALSE;
        }
        else
        {
          lpPrevChild=NULL;
          lpSubling=lpSubling->prev;
        }
      }
      lpPrevSubling=lpSubling;
    }
  }
  goto End;

  FindChild:
  bGoRoot=FALSE;

  //Find nearest child
  if (dwFlags & AEFF_FINDLINE)
  {
    dwFirst=mod(nFindLine - lpSubling->firstChild->lpMinPoint->ciPoint.nLine);
    dwSecond=mod(nFindLine - lpSubling->lastChild->lpMinPoint->ciPoint.nLine);
    if (lpPrevChild)
      dwThird=mod(nFindLine - lpPrevChild->lpMinPoint->ciPoint.nLine);
    else
      dwThird=(UINT_PTR)-1;
  }
  else
  {
    dwFirst=mod(nFindOffset - lpSubling->firstChild->lpMinPoint->nPointOffset);
    dwSecond=mod(nFindOffset - lpSubling->lastChild->lpMinPoint->nPointOffset);
    if (lpPrevChild)
      dwThird=mod(nFindOffset - lpPrevChild->lpMinPoint->nPointOffset);
    else
      dwThird=(UINT_PTR)-1;
  }

  if (dwFirst <= dwSecond && dwFirst <= dwThird)
  {
    lpSubling=lpSubling->firstChild;
    lpPrevChild=NULL;
    goto NextForward;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird)
  {
    lpSubling=lpSubling->lastChild;
    lpPrevChild=NULL;
    goto NextBackward;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond)
  {
    lpSubling=lpPrevChild;
    lpPrevChild=NULL;
    goto FindDirection;
  }

  End:
  if (lpPrevSubling)
    ae->ptxt->lpFindFoldLastCall=lpPrevSubling;
  else if (lpParent)
    ae->ptxt->lpFindFoldLastCall=lpParent;

  if ((dwFlags & AEFF_GETROOT) && lpParent)
  {
    while (lpParent->parent) lpParent=lpParent->parent;
  }
  if (lpParentOut) *lpParentOut=lpParent;
  if (lpPrevSublingOut) *lpPrevSublingOut=lpPrevSubling;
}

BOOL AE_StackFoldIsValid(AKELEDIT *ae, AEFOLD *lpFold)
{
  AEFOLD *lpElement;

  //Get root fold
  if (IsBadCodePtr((FARPROC)(UINT_PTR)lpFold))
    return FALSE;

  while (lpFold->parent)
  {
    if (IsBadCodePtr((FARPROC)(UINT_PTR)lpFold->parent))
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
  BOOL bCollapsed=(lpFold->dwFlags & AEFOLDF_COLLAPSED);

  if (lpFold == ae->ptxt->lpIsCollapsedLastCall)
    ae->ptxt->lpIsCollapsedLastCall=NULL;
  if (lpFold == ae->ptxt->lpFindFoldLastCall)
    ae->ptxt->lpFindFoldLastCall=NULL;
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

  //Update counts
  if (lpFold->dwFontStyle != AEHLS_NONE ||
      lpFold->crText != (DWORD)-1 ||
      lpFold->crBk != (DWORD)-1)
  {
    --ae->ptxt->nFoldColorCount;
  }
  if (lpFold->nRuleID)
    --ae->ptxt->nFoldWithIdCount;
  if (lpFold->hRuleTheme)
    --ae->ptxt->nFoldWithThemeCount;
  --ae->ptxt->nFoldAllCount;
  if (lpFold->dwFlags & AEFOLDF_COLLAPSED)
    --ae->ptxt->nFoldCollapseCount;

  AE_StackPointDelete(ae, lpFold->lpMinPoint);
  AE_StackPointDelete(ae, lpFold->lpMaxPoint);
  AE_HeapStackDelete(NULL, (stack **)lppFirstChild, (stack **)lppLastChild, (stack *)lpFold);
  return bCollapsed;
}

int AE_StackFoldFree(AKELEDIT *ae)
{
  AEFOLD *lpParent=NULL;
  AEFOLD *lpSubling=ae->ptxt->hFoldsStack.first;
  AEFOLD *lpNextSubling;
  int nCollapse=0;

  ae->ptxt->lpVPosFold=NULL;
  ae->ptxt->lpIsCollapsedLastCall=NULL;
  ae->ptxt->lpFindFoldLastCall=NULL;

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
    if (lpSubling->dwFlags & AEFOLDF_COLLAPSED) ++nCollapse;
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
  ae->ptxt->nFoldColorCount=0;
  ae->ptxt->nFoldWithIdCount=0;
  ae->ptxt->nFoldWithThemeCount=0;
  ae->ptxt->nFoldAllCount=0;
  ae->ptxt->nFoldCollapseCount=0;

  return nCollapse;
}

int AE_LastCollapsibleLine(AKELEDIT *ae, AEFOLD *lpFold)
{
  if (ae->ptxt->nHideMaxLineOffset == 0 && lpFold->next && lpFold->lpMaxPoint->ciPoint.nLine == lpFold->next->lpMinPoint->ciPoint.nLine)
  {
    //Deny to hide last fold line, if next subling begins on the same line.
    return lpFold->lpMaxPoint->ciPoint.nLine + (-1);
  }
  return lpFold->lpMaxPoint->ciPoint.nLine + ae->ptxt->nHideMaxLineOffset;
}

AEFOLD* AE_IsLineCollapsed(AKELEDIT *ae, int nLine)
{
  AEFOLD *lpCollapsedParent=NULL;
  AEFOLD *lpSubling=NULL;
  AEFOLD *lpPrevSubling=NULL;

  if (ae->ptxt->hFoldsStack.first && ae->ptxt->nFoldCollapseCount)
  {
    //Check input fold
    if (ae->ptxt->lpIsCollapsedLastCall && (ae->ptxt->lpIsCollapsedLastCall->dwFlags & AEFOLDF_COLLAPSED))
    {
      if (AE_FirstCollapsibleLine(ae, ae->ptxt->lpIsCollapsedLastCall) <= nLine &&
          AE_LastCollapsibleLine(ae, ae->ptxt->lpIsCollapsedLastCall) >= nLine)
      {
        return ae->ptxt->lpIsCollapsedLastCall;
      }
    }

    //Find fold by line
    AE_StackFindFold(ae, AEFF_FINDLINE|AEFF_FOLDSTART|AEFF_RECURSE|(ae->ptxt->nHideMaxLineOffset >= 0?AEFF_FOLDEND:0), nLine, NULL, &lpSubling, &lpPrevSubling);

    while (lpSubling)
    {
      if (lpSubling->dwFlags & AEFOLDF_COLLAPSED)
      {
        if (AE_FirstCollapsibleLine(ae, lpSubling) <= nLine &&
            AE_LastCollapsibleLine(ae, lpSubling) >= nLine)
        {
          //Fold is collapsed
          lpCollapsedParent=lpSubling;
        }
      }
      if (lpSubling->parent)
      {
        //Check the parent
        lpSubling=lpSubling->parent;
        continue;
      }
      lpSubling=lpCollapsedParent;
      break;
    }
    ae->ptxt->lpIsCollapsedLastCall=lpSubling;
  }
  return lpSubling;
}

int AE_LineCollapse(AKELEDIT *ae, int nLine, DWORD dwFlags)
{
  AEFOLD *lpFold=NULL;
  AEFOLD *lpSubling=NULL;
  AEFOLD *lpPrevSubling=NULL;
  int nResult=0;

  if (ae->ptxt->hFoldsStack.first)
  {
    AE_StackFindFold(ae, AEFF_FINDLINE|AEFF_FOLDSTART|AEFF_RECURSE|(ae->ptxt->nHideMaxLineOffset >= 0?AEFF_FOLDEND:0), nLine, NULL, &lpSubling, &lpPrevSubling);
    lpFold=lpSubling;

    while (lpSubling)
    {
      if (!(lpSubling->dwFlags & AEFOLDF_COLLAPSED) != !(dwFlags & AECF_COLLAPSE))
      {
        if (AE_FirstCollapsibleLine(ae, lpSubling) <= nLine &&
            AE_LastCollapsibleLine(ae, lpSubling) >= nLine)
        {
          if (lpSubling->dwFlags & AEFOLDF_COLLAPSED)
          {
            lpSubling->dwFlags&=~AEFOLDF_COLLAPSED;
            --ae->ptxt->nFoldCollapseCount;
          }
          else
          {
            lpSubling->dwFlags|=AEFOLDF_COLLAPSED;
            ++ae->ptxt->nFoldCollapseCount;
          }
          ++nResult;
        }
      }
      if (lpSubling->parent)
      {
        //Check the parent
        lpSubling=lpSubling->parent;
        continue;
      }
      break;
    }
  }
  AE_FoldScroll(ae, lpFold, dwFlags);

  return nResult;
}

int AE_FoldCollapse(AKELEDIT *ae, AEFOLD *lpFold, DWORD dwFlags)
{
  AEFOLD *lpCount;
  int nResult=0;

  if (lpFold)
    lpCount=lpFold;
  else
    lpCount=ae->ptxt->hFoldsStack.first;

  while (lpCount)
  {
    if (!(lpCount->dwFlags & AEFOLDF_COLLAPSED) != !(dwFlags & AECF_COLLAPSE))
    {
      if (lpCount->dwFlags & AEFOLDF_COLLAPSED)
      {
        lpCount->dwFlags&=~AEFOLDF_COLLAPSED;
        --ae->ptxt->nFoldCollapseCount;
      }
      else
      {
        lpCount->dwFlags|=AEFOLDF_COLLAPSED;
        ++ae->ptxt->nFoldCollapseCount;
      }
      ++nResult;
    }
    if (dwFlags & AECF_RECURSE)
    {
      //Custom AEC_NextFold implementation
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
    AE_StackFindFold(ae, AEFF_FINDINDEX|AEFF_GETROOT|(ae->ptxt->nHideMaxLineOffset >= 0?AEFF_FOLDEND:0), (UINT_PTR)&ae->ciCaretIndex, NULL, &lpFold, NULL);

  FoldScroll:
  AE_FoldScroll(ae, lpFold, dwFlags);

  return nResult;
}

void AE_FoldScroll(AKELEDIT *ae, AEFOLD *lpFold, DWORD dwFlags)
{
  if (lpFold)
  {
    if ((dwFlags & AECF_COLLAPSE) && !(dwFlags & AECF_NOCARETCORRECT))
    {
      POINT64 ptPos;
      POINT64 *lpPos=&ptPos;

      if (AE_FirstCollapsibleLine(ae, lpFold) <= ae->ciCaretIndex.nLine &&
          AE_LastCollapsibleLine(ae, lpFold) >= ae->ciCaretIndex.nLine)
      {
        AE_SetSelectionPos(ae, &lpFold->lpMinPoint->ciPoint, &lpFold->lpMinPoint->ciPoint, FALSE, AESELT_LOCKSCROLL, 0);
        lpPos=NULL;
      }
      else
      {
        if (!(dwFlags & AECF_NOUPDATE))
          AE_GetPosFromChar(ae, &lpFold->lpMinPoint->ciPoint, lpPos, NULL);
      }
      if (!(dwFlags & AECF_NOUPDATE))
        AE_ScrollToPoint(ae, lpPos);
    }
  }
}

INT_PTR AE_FoldUpdate(AKELEDIT *ae, int nFirstVisibleLine)
{
  INT_PTR nFirstVisiblePos;
  INT_PTR nScrolled=0;

  ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
  AE_UpdateScrollBars(ae, SB_VERT);
  ae->ptCaret.x=0;
  ae->ptCaret.y=0;
  AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
  if (!ae->popt->nVScrollLock && nFirstVisibleLine >= 0)
  {
    nFirstVisiblePos=AE_VPosFromLine(ae, nFirstVisibleLine);
    nScrolled=AE_ScrollEditWindow(ae, SB_VERT, nFirstVisiblePos);
  }
  InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
  AE_StackCloneUpdate(ae);
  return nScrolled;
}

int AE_LineFromVPos(AKELEDIT *ae, INT_PTR nVPos)
{
  AEFOLD *lpSubling=NULL;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  int nCurMinLine;
  int nCurMaxLine;
  int nLine=(int)(nVPos / ae->ptxt->nCharHeight);
  int nHiddenLines=0;

  if (ae->ptxt->hFoldsStack.first && ae->ptxt->nFoldCollapseCount)
  {
    dwFirst=mod(nLine - ae->ptxt->hFoldsStack.first->lpMinPoint->ciPoint.nLine);
    if (ae->ptxt->lpVPosFold)
      dwSecond=mod(nLine - (ae->ptxt->lpVPosFold->lpMinPoint->ciPoint.nLine - ae->ptxt->nVPosFoldHiddenLines));

    if (dwFirst <= dwSecond)
    {
      lpSubling=ae->ptxt->hFoldsStack.first;
      nHiddenLines=0;
    }
    else if (dwSecond <= dwFirst)
    {
      lpSubling=ae->ptxt->lpVPosFold;
      nHiddenLines=ae->ptxt->nVPosFoldHiddenLines;
    }

    if (nLine > lpSubling->lpMinPoint->ciPoint.nLine - nHiddenLines)
    {
      nLine+=nHiddenLines;

      while (lpSubling)
      {
        ae->ptxt->lpVPosFold=lpSubling;
        ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;

        if (lpSubling->dwFlags & AEFOLDF_COLLAPSED)
        {
          //Get collapsible range
          nCurMinLine=AE_FirstCollapsibleLine(ae, lpSubling);
          nCurMaxLine=AE_LastCollapsibleLine(ae, lpSubling);
          if (nLine < nCurMinLine) break;

          //Hidden count
          if (nCurMinLine <= nCurMaxLine)
          {
            nHiddenLines+=nCurMaxLine - nCurMinLine + 1;
            nLine+=nCurMaxLine - nCurMinLine + 1;
          }
          lpSubling=AEC_NextFold(lpSubling, FALSE);
          continue;
        }
        if (nLine < lpSubling->lpMinPoint->ciPoint.nLine)
          break;
        lpSubling=AEC_NextFold(lpSubling, TRUE);
      }
    }
    else
    {
      lpSubling=AEC_PrevFold(lpSubling, FALSE);

      while (lpSubling)
      {
        if (lpSubling->dwFlags & AEFOLDF_COLLAPSED)
        {
          //Get collapsible range
          nCurMinLine=AE_FirstCollapsibleLine(ae, lpSubling);
          nCurMaxLine=AE_LastCollapsibleLine(ae, lpSubling);
          if (nLine > nCurMaxLine - nHiddenLines) break;

          //Hidden count
          if (nCurMinLine <= nCurMaxLine)
            nHiddenLines-=nCurMaxLine - nCurMinLine + 1;
          ae->ptxt->lpVPosFold=lpSubling;
          ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;
          lpSubling=AEC_PrevFold(lpSubling, FALSE);
          continue;
        }
        if (nLine > lpSubling->lpMaxPoint->ciPoint.nLine - nHiddenLines)
          break;
        if (!lpSubling->firstChild)
        {
          ae->ptxt->lpVPosFold=lpSubling;
          ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;
        }
        lpSubling=AEC_PrevFold(lpSubling, TRUE);
      }
      nLine+=nHiddenLines;
    }
  }
  return nLine;
}

INT_PTR AE_VPosFromLine(AKELEDIT *ae, int nLine)
{
  AEFOLD *lpSubling=NULL;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  int nCurMinLine;
  int nCurMaxLine;
  int nHiddenLines=0;

  if (ae->ptxt->hFoldsStack.first && ae->ptxt->nFoldCollapseCount)
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
    }

    if (nLine >= lpSubling->lpMinPoint->ciPoint.nLine)
    {
      while (lpSubling)
      {
        ae->ptxt->lpVPosFold=lpSubling;
        ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;

        if (lpSubling->dwFlags & AEFOLDF_COLLAPSED)
        {
          //Get collapsible range
          nCurMinLine=AE_FirstCollapsibleLine(ae, lpSubling);
          nCurMaxLine=AE_LastCollapsibleLine(ae, lpSubling);
          if (nLine < nCurMinLine) break;

          //Hidden count
          if (nCurMinLine <= nCurMaxLine)
          {
            if (nLine <= nCurMaxLine)
            {
              nHiddenLines+=nLine - nCurMinLine + 1;
              break;
            }
            nHiddenLines+=nCurMaxLine - nCurMinLine + 1;
          }
          lpSubling=AEC_NextFold(lpSubling, FALSE);
          continue;
        }
        if (nLine <= lpSubling->lpMinPoint->ciPoint.nLine)
          break;
        lpSubling=AEC_NextFold(lpSubling, TRUE);
      }
    }
    else
    {
      lpSubling=AEC_PrevFold(lpSubling, FALSE);

      while (lpSubling)
      {
        if (lpSubling->dwFlags & AEFOLDF_COLLAPSED)
        {
          //Get collapsible range
          nCurMinLine=AE_FirstCollapsibleLine(ae, lpSubling);
          nCurMaxLine=AE_LastCollapsibleLine(ae, lpSubling);
          if (nLine > nCurMaxLine) break;

          //Hidden count
          if (nCurMinLine <= nCurMaxLine)
          {
            if (nLine >= nCurMinLine)
            {
              nHiddenLines-=nCurMaxLine - nLine + 1;
              break;
            }
            nHiddenLines-=nCurMaxLine - nCurMinLine + 1;
          }
          ae->ptxt->lpVPosFold=lpSubling;
          ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;
          lpSubling=AEC_PrevFold(lpSubling, FALSE);
          continue;
        }
        if (nLine > lpSubling->lpMaxPoint->ciPoint.nLine)
          break;
        if (!lpSubling->firstChild)
        {
          ae->ptxt->lpVPosFold=lpSubling;
          ae->ptxt->nVPosFoldHiddenLines=nHiddenLines;
        }
        lpSubling=AEC_PrevFold(lpSubling, TRUE);
      }
    }
  }
  return (nLine - nHiddenLines) * ae->ptxt->nCharHeight;
}

INT_PTR AE_GetVScrollMax(AKELEDIT *ae)
{
  return AE_VPosFromLine(ae, ae->ptxt->nLineCount + 1) + (ae->ptxt->nCharHeight * ae->popt->dwVScrollMaxOffset);
}

AEPOINT* AE_StackPointInsert(AKELEDIT *ae, AECHARINDEX *ciPoint)
{
  AEPOINT *lpElement;
  AEPOINT *lpNewElement=NULL;

  for (lpElement=ae->ptxt->hPointsStack.last; lpElement; lpElement=lpElement->prev)
  {
    if (ciPoint->nLine > lpElement->ciPoint.nLine)
      break;
    if (ciPoint->nLine == lpElement->ciPoint.nLine && ciPoint->nCharInLine >= lpElement->ciPoint.nCharInLine)
      break;
  }
  AE_HeapStackInsertAfter(NULL, (stack **)&ae->ptxt->hPointsStack.first, (stack **)&ae->ptxt->hPointsStack.last, (stack *)lpElement, (stack **)&lpNewElement, sizeof(AEPOINT));

  if (lpNewElement)
  {
    lpNewElement->ciPoint=*ciPoint;
    lpNewElement->nPointOffset=AEPTO_IGNORE;
    lpNewElement->nPointLen=0;
    lpNewElement->nTmpPointOffset=AEPTO_CALC;
    lpNewElement->nTmpPointLen=AEPTO_CALC;
  }
  return lpNewElement;
}

void AE_StackPointUnset(AKELEDIT *ae, DWORD dwFlags)
{
  AEPOINT *lpElement;

  for (lpElement=ae->ptxt->hPointsStack.first; lpElement; lpElement=lpElement->next)
  {
    lpElement->dwFlags&=~dwFlags;
  }
}

void AE_StackPointUnreserve(AKELEDIT *ae)
{
  AEPOINT *lpElement;

  for (lpElement=ae->ptxt->hPointsStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->nTmpPointOffset >= 0)
    {
      lpElement->nPointOffset=lpElement->nTmpPointOffset;
      lpElement->nTmpPointOffset=AEPTO_CALC;
    }
    if (lpElement->nTmpPointLen >= 0)
    {
      lpElement->nPointLen=lpElement->nTmpPointLen;
      lpElement->nTmpPointLen=AEPTO_CALC;
    }
    if (lpElement->dwFlags & AEPTF_NOTIFYDELETE)
    {
      lpElement->dwFlags&=~AEPTF_NOTIFYDELETE;
      AE_NotifyPoint(ae, AEPTT_DELETE, lpElement);
    }
    if (lpElement->dwFlags & AEPTF_NOTIFYINSERT)
    {
      lpElement->dwFlags&=~AEPTF_NOTIFYINSERT;
      AE_NotifyPoint(ae, AEPTT_INSERT, lpElement);
    }
  }
}

void AE_StackPointReset(AKELEDIT *ae, DWORD dwType)
{
  AEPOINT *lpElement;

  for (lpElement=ae->ptxt->hPointsStack.first; lpElement; lpElement=lpElement->next)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &lpElement->ciPoint);
    lpElement->nPointOffset=0;
    lpElement->nPointLen=0;
    lpElement->nTmpPointOffset=AEPTO_CALC;
    lpElement->nTmpPointLen=AEPTO_CALC;
    lpElement->dwFlags|=AEPTF_MODIFY|AEPTF_DELETE|AEPTF_MOVEOFFSET|AEPTF_MOVELINE;
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

  if (!AE_HeapStackInsertIndex(ae->aeUndo, (stack **)&ae->ptxt->hUndoStack.first, (stack **)&ae->ptxt->hUndoStack.last, (stack **)&lpElement, -1, sizeof(AEUNDOITEM)))
  {
    lpElement->nItemId=++ae->ptxt->hUndoStack.nMaxItemId;
    ae->ptxt->lpCurrentUndo=lpElement;
  }
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

  if (lpItem->wpText) AE_HeapFree(ae->aeUndo, 0, (LPVOID)lpItem->wpText);
  AE_HeapStackDelete(ae->aeUndo, (stack **)&ae->ptxt->hUndoStack.first, (stack **)&ae->ptxt->hUndoStack.last, (stack *)lpItem);
}

void AE_StackRedoDeleteAll(AKELEDIT *ae, AEUNDOITEM *lpItem)
{
  AEUNDOITEM *lpElement=lpItem;
  AEUNDOITEM *lpTmp;

  if (!lpElement)
    lpElement=ae->ptxt->hUndoStack.first;
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

UINT_PTR AE_StackUndoSize(AKELEDIT *ae)
{
  AEUNDOITEM *lpElement;
  UINT_PTR dwSize=0;

  for (lpElement=ae->ptxt->hUndoStack.first; lpElement; lpElement=lpElement->next)
  {
    dwSize+=lpElement->dwTextLen;
  }
  return dwSize;
}

void AE_StackUndoResetId(AKELEDIT *ae)
{
  AEUNDOITEM *lpElement;
  int nItemId=0;

  for (lpElement=ae->ptxt->hUndoStack.first; lpElement; lpElement=lpElement->next)
  {
    lpElement->nItemId=++nItemId;
  }
  ae->ptxt->hUndoStack.nMaxItemId=nItemId;
}

int AE_StackIsRangeModified(AKELEDIT *ae, const CHARRANGE64 *lpcrRange)
{
  AEUNDOITEM *lpElement;
  CHARRANGE64 crRange=*lpcrRange;
  INT_PTR nActionLen;
  int nResult=AEIRM_MODIFIEDUNSAVED;

  for (lpElement=ae->ptxt->lpCurrentUndo; lpElement; lpElement=lpElement->prev)
  {
    if (lpElement == ae->ptxt->lpSavePoint)
      nResult=AEIRM_MODIFIEDSAVED;

    if (lpElement->dwFlags & AEUN_DELETE)
    {
      if (crRange.cpMax > lpElement->nActionStartOffset &&
          crRange.cpMin < lpElement->nActionEndOffset)
      {
        return nResult;
      }
      else if (crRange.cpMin >= lpElement->nActionEndOffset)
      {
        nActionLen=lpElement->nActionEndOffset - lpElement->nActionStartOffset;
        crRange.cpMin-=nActionLen;
        crRange.cpMax-=nActionLen;
      }
    }
    else if (lpElement->dwFlags & AEUN_INSERT)
    {
      if (crRange.cpMax > lpElement->nActionStartOffset &&
          crRange.cpMin <= lpElement->nActionStartOffset)
      {
        return nResult;
      }
      else if (crRange.cpMin >= lpElement->nActionStartOffset)
      {
        nActionLen=lpElement->nActionEndOffset - lpElement->nActionStartOffset;
        crRange.cpMin+=nActionLen;
        crRange.cpMax+=nActionLen;
      }
    }
  }
  return AEIRM_UNMODIFIED;
}

INT_PTR AE_StackGetUndoPos(AKELEDIT *ae, const AEUNDOITEM *lpCurrentUndo, const AEUNDOITEM *lpUndoItem)
{
  const AEUNDOITEM *lpElement;
  INT_PTR nOffset;
  BOOL bRedo;

  if (lpCurrentUndo->nItemId < lpUndoItem->nItemId)
    bRedo=TRUE;
  else
    bRedo=FALSE;
  nOffset=lpUndoItem->nActionStartOffset;

  for (lpElement=lpCurrentUndo; lpElement;)
  {
    if (lpElement == lpUndoItem)
      return nOffset;

    if (bRedo)
    {
      if (lpElement->dwFlags & AEUN_INSERT)
      {
        if (nOffset >= lpElement->nActionStartOffset)
          nOffset+=(lpElement->nActionEndOffset - lpElement->nActionStartOffset);
      }
      else if (lpElement->dwFlags & AEUN_DELETE)
      {
        if (nOffset > lpElement->nActionStartOffset)
          nOffset=max(nOffset - (lpElement->nActionEndOffset - lpElement->nActionStartOffset), lpElement->nActionStartOffset);
      }
      lpElement=lpElement->next;
    }
    else
    {
      if (lpElement->dwFlags & AEUN_INSERT)
      {
        if (nOffset > lpElement->nActionStartOffset)
          nOffset=max(nOffset - (lpElement->nActionEndOffset - lpElement->nActionStartOffset), lpElement->nActionStartOffset);
      }
      else if (lpElement->dwFlags & AEUN_DELETE)
      {
        if (nOffset >= lpElement->nActionStartOffset)
          nOffset+=(lpElement->nActionEndOffset - lpElement->nActionStartOffset);
      }
      lpElement=lpElement->prev;
    }
  }
  return -1;
}

AEUNDOATTACH* AE_StackUndoDetach(AKELEDIT *ae)
{
  AEUNDOATTACH *hUndoAttach=NULL;
  AEUNDOITEM *lpUndoElement;
  wchar_t *wpUndoText;
  UINT_PTR dwUndoTextLen;
  DWORD dwUndoFlags=0;

  //Only if ES_GLOBALUNDO was set
  if (ae->aeUndo == NULL)
  {
    AE_StackUndoGroupStop(ae);

    //Set undo flags
    if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) < 0)
      dwUndoFlags=AEUN_CARETATSTART;

    if (wpUndoText=AE_GetAllTextForUndo(ae, &dwUndoTextLen))
    {
      if (lpUndoElement=AE_StackUndoItemInsert(ae))
      {
        lpUndoElement->dwFlags=AEUN_INSERT|dwUndoFlags;
        lpUndoElement->nActionStartOffset=0;
        lpUndoElement->nActionEndOffset=ae->ptxt->nLastCharOffset;
        lpUndoElement->wpText=wpUndoText;
        lpUndoElement->dwTextLen=dwUndoTextLen;
        lpUndoElement->nNewLine=AELB_ASIS;

        if (hUndoAttach=(AEUNDOATTACH *)AE_HeapAlloc(NULL, 0, sizeof(AEUNDOATTACH)))
        {
          hUndoAttach->first=ae->ptxt->hUndoStack.first;
          hUndoAttach->last=ae->ptxt->hUndoStack.last;
          hUndoAttach->dwUndoCount=ae->ptxt->dwUndoCount;
          ae->ptxt->hUndoStack.first=NULL;
          ae->ptxt->hUndoStack.last=NULL;
          ae->ptxt->dwUndoCount=0;
          AE_EmptyUndoBuffer(ae);
        }
      }
    }
  }
  return hUndoAttach;
}

BOOL AE_StackUndoAttach(AKELEDIT *ae, AEUNDOATTACH *hUndoAttach)
{
  AEUNDOITEM *lpUndoElement;
  wchar_t *wpUndoText;
  UINT_PTR dwUndoTextLen;
  DWORD dwUndoFlags=0;
  BOOL bModified;

  if (hUndoAttach)
  {
    //Only if ES_GLOBALUNDO was set
    if (ae->aeUndo == NULL)
    {
      bModified=AE_GetModify(ae);

      AE_EmptyUndoBuffer(ae);
      ae->ptxt->hUndoStack.first=hUndoAttach->first;
      ae->ptxt->hUndoStack.last=hUndoAttach->last;
      ae->ptxt->dwUndoCount=hUndoAttach->dwUndoCount;
      ae->ptxt->lpCurrentUndo=ae->ptxt->hUndoStack.last;
      AE_HeapFree(NULL, 0, (LPVOID)hUndoAttach);

      //Set undo flags
      if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) < 0)
        dwUndoFlags=AEUN_CARETATSTART;

      if (wpUndoText=AE_GetAllTextForUndo(ae, &dwUndoTextLen))
      {
        lpUndoElement=ae->ptxt->hUndoStack.last;

        if (lpUndoElement->dwTextLen == dwUndoTextLen && !xmemcmp(lpUndoElement->wpText, wpUndoText, dwUndoTextLen * sizeof(wchar_t)))
        {
          //Last detached undo item is equal to the current text. Remove this item to save memory.
          AE_StackUndoItemDelete(ae, lpUndoElement);
          ae->ptxt->lpCurrentUndo=ae->ptxt->hUndoStack.last;
          AE_HeapFree(ae->aeUndo, 0, (LPVOID)wpUndoText);
        }
        else
        {
          if (lpUndoElement=AE_StackUndoItemInsert(ae))
          {
            lpUndoElement->dwFlags=AEUN_DELETE|dwUndoFlags;
            lpUndoElement->nActionStartOffset=0;
            lpUndoElement->nActionEndOffset=ae->ptxt->nLastCharOffset;
            lpUndoElement->wpText=wpUndoText;
            lpUndoElement->dwTextLen=dwUndoTextLen;
            lpUndoElement->nNewLine=AELB_ASIS;

            AE_StackUndoGroupStop(ae);
          }
        }
      }

      if (bModified)
      {
        ae->ptxt->lpSavePoint=NULL;
        ae->ptxt->bSavePointExist=FALSE;
      }
      else
      {
        ae->ptxt->lpSavePoint=ae->ptxt->lpCurrentUndo;
        ae->ptxt->bSavePointExist=TRUE;
      }
      AE_StackUndoResetId(ae);
      return TRUE;
    }
  }
  return FALSE;
}

wchar_t* AE_GetAllTextForUndo(AKELEDIT *ae, UINT_PTR *lpdwUndoTextLen)
{
  AECHARINDEX ciRangeStart;
  AECHARINDEX ciRangeEnd;
  wchar_t *wpUndoText=NULL;
  UINT_PTR dwUndoTextLen;

  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciRangeStart);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciRangeEnd);

  if (dwUndoTextLen=AE_GetTextRange(ae, &ciRangeStart, &ciRangeEnd, NULL, 0, AELB_ASIS, FALSE, FALSE))
  {
    if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, dwUndoTextLen * sizeof(wchar_t)))
    {
      dwUndoTextLen=AE_GetTextRange(ae, &ciRangeStart, &ciRangeEnd, wpUndoText, (UINT_PTR)-1, AELB_ASIS, FALSE, FALSE);
    }
  }
  *lpdwUndoTextLen=dwUndoTextLen;
  return wpUndoText;
}

void AE_StackUndoGroupStop(AKELEDIT *ae)
{
  AEUNDOITEM *lpStopElement=ae->ptxt->hUndoStack.last;

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
        lpStopElement=ae->ptxt->hUndoStack.last;
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
        INT_PTR nDeleteStart=0;
        INT_PTR nDeleteEnd=0;
        wchar_t *wpUndoText;
        UINT_PTR dwUndoTextLen=0;
        INT_PTR nCount;

        //Get count of typing characters
        while (lpElement)
        {
          if (lpElement->dwFlags & AEUN_SINGLECHAR)
            dwUndoTextLen+=lpElement->dwTextLen;
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
          if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
          {
            //Last index
            if (lpElement)
            {
              nDeleteEnd=lpElement->nActionEndOffset;
            }

            //Get string
            wpUndoText[dwUndoTextLen]=L'\0';
            nCount=dwUndoTextLen;

            while (lpElement)
            {
              if (lpElement->dwFlags & AEUN_SINGLECHAR)
              {
                if (nCount > 0)
                {
                  nCount-=lpElement->dwTextLen;
                  xmemcpy(wpUndoText + nCount, lpElement->wpText, lpElement->dwTextLen * sizeof(wchar_t));
                }
                else break;
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
              lpElement=ae->ptxt->hUndoStack.first;
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
              lpUndoElement->nNewLine=AELB_ASIS;

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
        AEUNDOITEM *lpElement;
        AEUNDOITEM *lpTmp;

        //Delete first undo group
        for (lpElement=ae->ptxt->hUndoStack.first; lpElement; lpElement=lpTmp)
        {
          if (lpElement->dwFlags & AEUN_STOPGROUP)
          {
            AE_StackUndoItemDelete(ae, lpElement);
            break;
          }
          lpTmp=lpElement->next;
          AE_StackUndoItemDelete(ae, lpElement);
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
  if (lpElement->wpLine)
  {
    AE_HeapFree(ae, 0, (LPVOID)lpElement->wpLine);
    #ifdef _DEBUG
    lpElement->wpLine=NULL;
    #endif
  }
  AE_HeapStackDelete(ae, (stack **)&ae->ptxt->hLinesStack.first, (stack **)&ae->ptxt->hLinesStack.last, (stack *)lpElement);
}

void AE_StackLineFree(AKELEDIT *ae)
{
  AELINEDATA *lpElement;
  AELINEDATA *lpTmp;

  for (lpElement=ae->ptxt->hLinesStack.first; lpElement; lpElement=lpTmp)
  {
    lpTmp=lpElement->next;
    AE_StackLineDelete(ae, lpElement);
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
  INT_PTR nElementLineOffset=0;
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
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciElement);
    nElementLineOffset=0;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciElement);
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

int AE_GetLineNumber(AKELEDIT *ae, int nType, INT_PTR nCharOffset)
{
  switch (nType)
  {
    case AEGL_LINECOUNT:
      return ae->ptxt->nLineCount + 1;
    case AEGL_FIRSTSELLINE:
      return ae->ciSelStartIndex.nLine;
    case AEGL_LASTSELLINE:
      return ae->ciSelEndIndex.nLine;
    case AEGL_CARETLINE:
      return ae->ciCaretIndex.nLine;
    case AEGL_FIRSTVISIBLELINE:
      return AE_GetFirstVisibleLine(ae);
    case AEGL_LASTVISIBLELINE:
      return AE_GetLastVisibleLine(ae);
    case AEGL_FIRSTFULLVISIBLELINE:
      return AE_GetFirstFullVisibleLine(ae);
    case AEGL_LASTFULLVISIBLELINE:
      return AE_GetLastFullVisibleLine(ae);
    case AEGL_LINEUNWRAPCOUNT:
      return ae->ptxt->nLineUnwrapCount + 1;
    case AEGL_UNWRAPSELMULTILINE:
    {
      AECHARINDEX ciCount;

      if (ae->ciSelStartIndex.nLine < ae->ciSelEndIndex.nLine)
      {
        for (ciCount=ae->ciSelStartIndex; ciCount.nLine < ae->ciSelEndIndex.nLine; AEC_NextLine(&ciCount))
        {
          if (ciCount.lpLine->nLineBreak != AELB_WRAP)
            return TRUE;
        }
      }
      return FALSE;
    }
    case AEGL_LINEFROMRICHOFFSET:
    case AEGL_UNWRAPLINEFROMRICHOFFSET:
    {
      AECHARINDEX ciChar={0};

      if (nCharOffset == -1)
        ciChar.nLine=ae->ciCaretIndex.nLine;
      else
        AE_RichOffsetToAkelIndex(ae, nCharOffset, &ciChar);
      if (nType == AEGL_UNWRAPLINEFROMRICHOFFSET)
        return AE_GetUnwrapLine(ae, ciChar.nLine);
      else
        return ciChar.nLine;
    }
  }
  return -1;
}

int AE_GetWrapLine(AKELEDIT *ae, int nLine, AECHARINDEX *ciCharOut)
{
  AELINEINDEX liLine={0};
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nElementLine=0;

  if (nLine < 0) nLine=ae->ptxt->nLineUnwrapCount + nLine + 1;
  if (nLine < 0 || nLine > ae->ptxt->nLineUnwrapCount) return -1;
  if (!ae->ptxt->dwWordWrap)
  {
    if (ciCharOut)
    {
      ciCharOut->nLine=nLine;
      ciCharOut->lpLine=AE_GetLineData(ae, nLine);
      ciCharOut->nCharInLine=0;
    }
    return nLine;
  }

  //Find nearest element in stack
  dwFirst=mod(nLine - 0);
  dwSecond=mod(nLine - ae->ptxt->nLineUnwrapCount);
  if (ae->ptxt->liLineUnwrapLastCall.lpLine)
    dwThird=mod(nLine - ae->ptxt->nLineUnwrapLastCall);

  if (dwFirst <= dwSecond && dwFirst <= dwThird)
  {
    nElementLine=nLine - 0;
    liLine.nLine=0;
    liLine.lpLine=ae->ptxt->hLinesStack.first;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird)
  {
    nElementLine=nLine - ae->ptxt->nLineUnwrapCount;
    liLine.nLine=ae->ptxt->nLineCount;
    liLine.lpLine=ae->ptxt->hLinesStack.last;
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
    liLine.lpLine=ae->ptxt->hLinesStack.first;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird)
  {
    nUnwrappedLine=ae->ptxt->nLineUnwrapCount;
    nElementLine=nLine - ae->ptxt->nLineCount;
    liLine.nLine=ae->ptxt->nLineCount;
    liLine.lpLine=ae->ptxt->hLinesStack.last;
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

void AE_RichOffsetToAkelIndex(AKELEDIT *ae, INT_PTR nOffset, AECHARINDEX *ciCharIndex)
{
  AECHARINDEX ciElement;
  UINT_PTR dwFirst=(UINT_PTR)-1;
  UINT_PTR dwSecond=(UINT_PTR)-1;
  UINT_PTR dwThird=(UINT_PTR)-1;
  UINT_PTR dwFourth=(UINT_PTR)-1;
  UINT_PTR dwFifth=(UINT_PTR)-1;
  UINT_PTR dwSixth=(UINT_PTR)-1;
  INT_PTR nElementLineOffset=0;

  nOffset=min((UINT_PTR)nOffset, (UINT_PTR)ae->ptxt->nLastCharOffset);

  //Find nearest element in stack
  dwFirst=mod(nOffset - 0);
  dwSecond=mod(nOffset - ae->ptxt->nLastCharOffset);
  if (ae->liFirstDrawLine.lpLine && ae->nFirstDrawLineOffset)
    dwThird=mod(nOffset - ae->nFirstDrawLineOffset);
  if (ae->ciSelStartIndex.lpLine && ae->nSelStartCharOffset)
    dwFourth=mod(nOffset - ae->nSelStartCharOffset);
  if (ae->ciSelEndIndex.lpLine && ae->nSelEndCharOffset)
    dwFifth=mod(nOffset - ae->nSelEndCharOffset);
  if (ae->ciLastCallIndex.lpLine && ae->nLastCallOffset)
    dwSixth=mod(nOffset - ae->nLastCallOffset);

  if (dwFirst <= dwSecond && dwFirst <= dwThird && dwFirst <= dwFourth && dwFirst <= dwFifth && dwFirst <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciElement);
    nElementLineOffset=nOffset - 0;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciElement);
    nElementLineOffset=nOffset - ae->ptxt->nLastCharOffset;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth && dwThird <= dwFifth && dwThird <= dwSixth)
  {
    ciElement.nLine=ae->liFirstDrawLine.nLine;
    ciElement.lpLine=ae->liFirstDrawLine.lpLine;
    ciElement.nCharInLine=0;
    nElementLineOffset=nOffset - ae->nFirstDrawLineOffset;
  }
  else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird && dwFourth <= dwFifth && dwFourth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelStartIndex.nLine;
    ciElement.lpLine=ae->ciSelStartIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelStartIndex.nCharInLine, ae->ciSelStartIndex.lpLine->nLineLen);
    nElementLineOffset=nOffset - ae->nSelStartCharOffset;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    ciElement.nLine=ae->ciSelEndIndex.nLine;
    ciElement.lpLine=ae->ciSelEndIndex.lpLine;
    ciElement.nCharInLine=min(ae->ciSelEndIndex.nCharInLine, ae->ciSelEndIndex.lpLine->nLineLen);
    nElementLineOffset=nOffset - ae->nSelEndCharOffset;
  }
  else if (dwSixth <= dwFirst && dwSixth <= dwSecond && dwSixth <= dwThird && dwSixth <= dwFourth && dwSixth <= dwFifth)
  {
    ciElement=ae->ciLastCallIndex;
    nElementLineOffset=nOffset - ae->nLastCallOffset;
  }

  AE_IndexOffset(ae, &ciElement, &ciElement, nElementLineOffset, AELB_R);

  if (nElementLineOffset)
  {
    ae->ciLastCallIndex=ciElement;
    ae->nLastCallOffset=nOffset;
  }
  ciCharIndex->nLine=ciElement.nLine;
  ciCharIndex->lpLine=ciElement.lpLine;
  ciCharIndex->nCharInLine=ciElement.nCharInLine;
}

INT_PTR AE_AkelIndexToRichOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIndex)
{
  AECHARINDEX ciChar=*ciCharIndex;
  AECHARINDEX ciElement;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  INT_PTR nElementLineOffset=0;
  INT_PTR nResult;

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
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciElement);
    nElementLineOffset=0;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciElement);
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

  if (AEC_IndexCompare(&ciChar, &ciElement))
  {
    nResult=nElementLineOffset + AE_IndexSubtract(ae, &ciChar, &ciElement, AELB_R, FALSE, FALSE);
  }
  else return nElementLineOffset;

  ae->ciLastCallIndex=ciChar;
  ae->nLastCallOffset=nResult;
  return nResult;
}

AELINEDATA* AE_GetIndex(AKELEDIT *ae, int nType, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut)
{
  switch (nType)
  {
    case AEGI_FIRSTCHAR:
    {
      ciCharOut->nLine=0;
      ciCharOut->lpLine=ae->ptxt->hLinesStack.first;
      ciCharOut->nCharInLine=0;
      return ciCharOut->lpLine;
    }
    case AEGI_LASTCHAR:
    {
      ciCharOut->nLine=ae->ptxt->nLineCount;
      ciCharOut->lpLine=ae->ptxt->hLinesStack.last;
      ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
      return ciCharOut->lpLine;
    }
    case AEGI_FIRSTSELCHAR:
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
    case AEGI_LASTSELCHAR:
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
    case AEGI_CARETCHAR:
    {
      *ciCharOut=ae->ciCaretIndex;
      return ciCharOut->lpLine;
    }
    case AEGI_FIRSTVISIBLECHAR:
    {
      AECHARINDEX ciCharTmp;

      if (AE_GetCharFromPos(ae, ae->rcDraw.left, ae->rcDraw.top, &ciCharTmp, NULL, FALSE))
      {
        *ciCharOut=ciCharTmp;
        return ciCharOut->lpLine;
      }
      return NULL;
    }
    case AEGI_LASTVISIBLECHAR:
    {
      AECHARINDEX ciCharTmp;

      if (AE_GetCharFromPos(ae, ae->rcDraw.right, ae->rcDraw.bottom, &ciCharTmp, NULL, FALSE))
      {
        *ciCharOut=ciCharTmp;
        return ciCharOut->lpLine;
      }
      return NULL;
    }
    case AEGI_FIRSTFULLVISIBLECHAR:
    {
      AECHARINDEX ciCharTmp;
      INT_PTR nVertPos;
      int nResult;

      nVertPos=ae->nVScrollPos / ae->ptxt->nCharHeight * ae->ptxt->nCharHeight;
      if (nVertPos < ae->nVScrollPos) nVertPos+=ae->ptxt->nCharHeight;
      nVertPos=ae->rcDraw.top + (nVertPos - ae->nVScrollPos);

      if (nResult=AE_GetCharFromPos(ae, ae->rcDraw.left, nVertPos, &ciCharTmp, NULL, FALSE))
      {
        if (nResult == AEPC_BEFORE)
          AEC_NextCharInLineEx(&ciCharTmp, ciCharOut);
        else
          *ciCharOut=ciCharTmp;
        return ciCharOut->lpLine;
      }
      return NULL;
    }
    case AEGI_LASTFULLVISIBLECHAR:
    {
      AECHARINDEX ciCharTmp;
      INT_PTR nVertPos;
      int nResult;

      nVertPos=(ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top)) / ae->ptxt->nCharHeight * ae->ptxt->nCharHeight;
      nVertPos=ae->rcDraw.top + (nVertPos - ae->nVScrollPos - ae->ptxt->nCharHeight);

      if (nResult=AE_GetCharFromPos(ae, ae->rcDraw.right, nVertPos, &ciCharTmp, NULL, FALSE))
      {
        if (nResult == AEPC_AFTER)
          AEC_PrevCharInLineEx(&ciCharTmp, ciCharOut);
        else
          *ciCharOut=ciCharTmp;
        return ciCharOut->lpLine;
      }
      return NULL;
    }
    case AEGI_FIRSTVISIBLELINE:
    {
      ciCharOut->nLine=AE_GetFirstVisibleLine(ae);
      ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
      ciCharOut->nCharInLine=0;
      return ciCharOut->lpLine;
    }
    case AEGI_LASTVISIBLELINE:
    {
      ciCharOut->nLine=AE_GetLastVisibleLine(ae);
      ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
      if (ciCharOut->lpLine)
        ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
      return ciCharOut->lpLine;
    }
    case AEGI_FIRSTFULLVISIBLELINE:
    {
      ciCharOut->nLine=AE_GetFirstFullVisibleLine(ae);
      ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
      ciCharOut->nCharInLine=0;
      return ciCharOut->lpLine;
    }
    case AEGI_LASTFULLVISIBLELINE:
    {
      ciCharOut->nLine=AE_GetLastFullVisibleLine(ae);
      ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
      if (ciCharOut->lpLine)
        ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
      return ciCharOut->lpLine;
    }

    //Next flags require pointer to the input index in lParam.
    case AEGI_VALIDCHARINLINE:
    {
      AECHARINDEX ciCharTmp=*ciCharIn;

      if (AEC_ValidCharInLine(&ciCharTmp))
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
    case AEGI_LINEBEGIN:
    {
      ciCharOut->nLine=ciCharIn->nLine;
      ciCharOut->lpLine=ciCharIn->lpLine;
      ciCharOut->nCharInLine=0;
      return ciCharOut->lpLine;
    }
    case AEGI_LINEEND:
    {
      ciCharOut->nLine=ciCharIn->nLine;
      ciCharOut->lpLine=ciCharIn->lpLine;
      ciCharOut->nCharInLine=ciCharIn->lpLine->nLineLen;
      return ciCharOut->lpLine;
    }
    case AEGI_WRAPLINEBEGIN:
    {
      return (AELINEDATA *)(UINT_PTR)AEC_WrapLineBeginEx(ciCharIn, ciCharOut);
    }
    case AEGI_WRAPLINEEND:
    {
      return (AELINEDATA *)(UINT_PTR)AEC_WrapLineEndEx(ciCharIn, ciCharOut);
    }
    case AEGI_NEXTCHARINLINE:
    {
      return AEC_NextCharInLineEx(ciCharIn, ciCharOut);
    }
    case AEGI_PREVCHARINLINE:
    {
      return AEC_PrevCharInLineEx(ciCharIn, ciCharOut);
    }
    case AEGI_NEXTCHAR:
    case AEGI_NEXTUNCOLLAPSEDCHAR:
    {
      AECHARINDEX ciCharTmp=*ciCharIn;
      AEFOLD *lpCollapsed;

      if (AEC_NextChar(&ciCharTmp))
      {
        if (nType == AEGI_NEXTUNCOLLAPSEDCHAR)
        {
          if (lpCollapsed=AE_IsLineCollapsed(ae, ciCharTmp.nLine))
          {
            ciCharTmp=lpCollapsed->lpMaxPoint->ciPoint;
            while (ciCharTmp.nLine > AE_LastCollapsibleLine(ae, lpCollapsed))
              AEC_PrevLine(&ciCharTmp);
            AEC_NextLine(&ciCharTmp);
          }
        }
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
    case AEGI_PREVCHAR:
    case AEGI_PREVUNCOLLAPSEDCHAR:
    {
      AECHARINDEX ciCharTmp=*ciCharIn;
      AEFOLD *lpCollapsed;


      if (AEC_PrevChar(&ciCharTmp))
      {
        if (nType == AEGI_PREVUNCOLLAPSEDCHAR)
        {
          if (lpCollapsed=AE_IsLineCollapsed(ae, ciCharTmp.nLine))
          {
            ciCharTmp=lpCollapsed->lpMinPoint->ciPoint;
            while (ciCharTmp.nLine < AE_FirstCollapsibleLine(ae, lpCollapsed))
              AEC_NextLine(&ciCharTmp);
            AEC_PrevLine(&ciCharTmp);
          }
        }
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
    case AEGI_NEXTLINE:
    case AEGI_NEXTUNCOLLAPSEDLINE:
    {
      AECHARINDEX ciCharTmp=*ciCharIn;
      AEFOLD *lpCollapsed;

      if (AEC_NextLine(&ciCharTmp))
      {
        if (nType == AEGI_NEXTUNCOLLAPSEDLINE)
        {
          while (lpCollapsed=AE_IsLineCollapsed(ae, ciCharTmp.nLine))
          {
            ciCharTmp=lpCollapsed->lpMaxPoint->ciPoint;
            while (ciCharTmp.nLine > AE_LastCollapsibleLine(ae, lpCollapsed))
              AEC_PrevLine(&ciCharTmp);
            if (!AEC_NextLine(&ciCharTmp))
              break;
          }
        }
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
    case AEGI_PREVLINE:
    case AEGI_PREVUNCOLLAPSEDLINE:
    {
      AECHARINDEX ciCharTmp=*ciCharIn;
      AEFOLD *lpCollapsed;

      if (AEC_PrevLine(&ciCharTmp))
      {
        if (nType == AEGI_PREVUNCOLLAPSEDLINE)
        {
          while (lpCollapsed=AE_IsLineCollapsed(ae, ciCharTmp.nLine))
          {
            ciCharTmp=lpCollapsed->lpMinPoint->ciPoint;
            while (ciCharTmp.nLine < AE_FirstCollapsibleLine(ae, lpCollapsed))
              AEC_NextLine(&ciCharTmp);
            if (!AEC_PrevLine(&ciCharTmp))
              break;
          }
        }
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
  return NULL;
}

INT_PTR AE_GetRichOffset(AKELEDIT *ae, int nType, INT_PTR nCharOffsetOrLine)
{
  switch (nType)
  {
    case AEGI_FIRSTCHAR:
      return 0;
    case AEGI_LASTCHAR:
      return ae->ptxt->nLastCharOffset;
    case AEGI_FIRSTSELCHAR:
      return ae->nSelStartCharOffset;
    case AEGI_LASTSELCHAR:
      return ae->nSelEndCharOffset;
    case AEGI_CARETCHAR:
    {
      if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
        return ae->nSelEndCharOffset;
      else
        return ae->nSelStartCharOffset;
    }
    case AEGI_FIRSTVISIBLECHAR:
    case AEGI_LASTVISIBLECHAR:
    case AEGI_FIRSTFULLVISIBLECHAR:
    case AEGI_LASTFULLVISIBLECHAR:
    case AEGI_FIRSTVISIBLELINE:
    case AEGI_LASTVISIBLELINE:
    case AEGI_FIRSTFULLVISIBLELINE:
    case AEGI_LASTFULLVISIBLELINE:
    {
      AECHARINDEX ciChar;

      if (!AE_GetIndex(ae, nType, NULL, &ciChar))
        return -1;
      return AE_AkelIndexToRichOffset(ae, &ciChar);
    }
    case AEGI_RICHOFFSETFROMLINE:
    case AEGI_RICHOFFSETFROMUNWRAPLINE:
    {
      AECHARINDEX ciChar;

      if (nCharOffsetOrLine == -1)
      {
        ciChar.nLine=ae->ciCaretIndex.nLine;
        ciChar.lpLine=ae->ciCaretIndex.lpLine;
        ciChar.nCharInLine=0;
        if (nType == AEGI_RICHOFFSETFROMUNWRAPLINE)
          AEC_WrapLineBeginEx(&ciChar, &ciChar);
      }
      else
      {
        if (nType == AEGI_RICHOFFSETFROMUNWRAPLINE)
        {
          if ((nCharOffsetOrLine=AE_GetWrapLine(ae, (int)nCharOffsetOrLine, &ciChar)) == -1)
            return -1;
        }
        else
        {
          if (nCharOffsetOrLine <= ae->ptxt->nLineCount)
          {
            ciChar.nLine=(int)nCharOffsetOrLine;
            ciChar.lpLine=AE_GetLineData(ae, ciChar.nLine);
            ciChar.nCharInLine=0;
          }
          else return -1;
        }
      }
      return AE_AkelIndexToRichOffset(ae, &ciChar);
    }
    case AEGI_WRAPLINEBEGIN:
    case AEGI_WRAPLINEEND:
    {
      AECHARINDEX ciChar;

      AE_RichOffsetToAkelIndex(ae, nCharOffsetOrLine, &ciChar);

      if (nType == AEGI_WRAPLINEBEGIN)
        AEC_WrapLineBeginEx(&ciChar, &ciChar);
      else //if (nType == AEGI_WRAPLINEEND)
        AEC_WrapLineEndEx(&ciChar, &ciChar);
      return AE_AkelIndexToRichOffset(ae, &ciChar);
    }
    default:
    {
      //Other AEGI_*
      AECHARINDEX ciChar;

      AE_RichOffsetToAkelIndex(ae, nCharOffsetOrLine, &ciChar);

      if (!AE_GetIndex(ae, nType, &ciChar, &ciChar))
        return -1;
      return AE_AkelIndexToRichOffset(ae, &ciChar);
    }
  }
}

INT_PTR AE_IndexSubtract(AKELEDIT *ae, const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  AECHARINDEX ciStart;
  AECHARINDEX ciEnd;
  AECHARINDEX ciTmp;
  AELINEDATA *lpElement;
  INT_PTR nCount=0;
  int nCompare;
  BYTE nLineBreak;

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
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciStart);
  else
    ciStart=*ciChar1;

  if (!ciChar2)
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciEnd);
  else
    ciEnd=*ciChar2;

  //Exchange indexes
  if ((nCompare=AEC_IndexCompare(&ciStart, &ciEnd)) > 0)
  {
    ciTmp=ciStart;
    ciStart=ciEnd;
    ciEnd=ciTmp;
  }

  if (bColumnSel)
  {
    for (lpElement=ciStart.lpLine; lpElement; lpElement=lpElement->next)
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
          nLineBreak=(BYTE)nNewLine;
      }
      else
      {
        if (nNewLine == AELB_ASIS)
          nLineBreak=lpElement->nLineBreak;
        else
          nLineBreak=(BYTE)nNewLine;
      }

      if (nLineBreak == AELB_R)
        nCount+=1;
      else if (nLineBreak == AELB_N)
        nCount+=1;
      else if (nLineBreak == AELB_RN)
        nCount+=2;
      else if (nLineBreak == AELB_RRN)
        nCount+=3;
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
      for (lpElement=ciStart.lpLine; lpElement; lpElement=lpElement->next)
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
            nLineBreak=(BYTE)nNewLine;

          if (nLineBreak == AELB_R)
            nCount+=1;
          else if (nLineBreak == AELB_N)
            nCount+=1;
          else if (nLineBreak == AELB_RN)
            nCount+=2;
          else if (nLineBreak == AELB_RRN)
            nCount+=3;
        }
      }
    }
  }
  return (nCompare < 0)?-nCount:nCount;
}

UINT_PTR AE_IndexOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, INT_PTR nOffset, int nNewLine)
{
  AECHARINDEX ciCount=*ciCharIn;
  INT_PTR nOffsetCount=nOffset;
  int nSub;
  BYTE nLineBreak;

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
        AEC_NextLine(&ciCount);
    }
  }
  else if (nOffsetCount > 0)
  {
    do
    {
      if ((nSub=ciCount.lpLine->nLineLen - ciCount.nCharInLine) > 0)
      {
        if (nSub >= nOffsetCount)
        {
          ciCount.nCharInLine+=(int)nOffsetCount;
          nOffsetCount=0;
          goto End;
        }
        nOffsetCount-=nSub;
      }

      if (ciCount.lpLine->next)
      {
        nLineBreak=ciCount.lpLine->nLineBreak;
        AEC_NextLine(&ciCount);
      }
      else
      {
        ciCount.nCharInLine=ciCount.lpLine->nLineLen;
        goto End;
      }

      if (nLineBreak != AELB_WRAP)
      {
        if (nNewLine != AELB_ASIS)
          nLineBreak=(BYTE)nNewLine;

        if (nLineBreak == AELB_R)
          nOffsetCount-=1;
        else if (nLineBreak == AELB_N)
          nOffsetCount-=1;
        else if (nLineBreak == AELB_RN)
          nOffsetCount-=2;
        else if (nLineBreak == AELB_RRN)
          nOffsetCount-=3;
      }
    }
    while (nOffsetCount > 0);
  }
  else if (nOffsetCount < 0)
  {
    //Normalize
    ciCount.nCharInLine=min(ciCount.nCharInLine, ciCount.lpLine->nLineLen);

    do
    {
      if (ciCount.nCharInLine > 0)
      {
        if (ciCount.nCharInLine >= -nOffsetCount)
        {
          ciCount.nCharInLine+=(int)nOffsetCount;
          nOffsetCount=0;
          goto End;
        }
        nOffsetCount+=ciCount.nCharInLine;
      }

      if (ciCount.lpLine->prev)
      {
        AEC_PrevLine(&ciCount);
        nLineBreak=ciCount.lpLine->nLineBreak;
      }
      else
      {
        ciCount.nCharInLine=0;
        goto End;
      }

      if (nLineBreak != AELB_WRAP)
      {
        if (nNewLine != AELB_ASIS)
          nLineBreak=(BYTE)nNewLine;

        if (nLineBreak == AELB_R)
          nOffsetCount+=1;
        else if (nLineBreak == AELB_N)
          nOffsetCount+=1;
        else if (nLineBreak == AELB_RN)
          nOffsetCount+=2;
        else if (nLineBreak == AELB_RRN)
          nOffsetCount+=3;
      }
    }
    while (nOffsetCount < 0);
  }

  End:
  *ciCharOut=ciCount;
  return mod(nOffset - nOffsetCount);
}

BOOL AE_IndexNormalize(AECHARINDEX *ciChar)
{
  if (ciChar->nCharInLine > 0 && ciChar->nCharInLine < ciChar->lpLine->nLineLen)
  {
    if (AE_IsLowSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine]) &&
        AE_IsHighSurrogate(ciChar->lpLine->wpLine[ciChar->nCharInLine - 1]))
    {
      --ciChar->nCharInLine;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_IndexUpdate(AKELEDIT *ae, AECHARINDEX *ciChar)
{
  ciChar->nLine=max(ciChar->nLine, 0);

  if (ciChar->nLine > ae->ptxt->nLineCount)
  {
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, ciChar);
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
  AECHARINDEX ciFirstVisibleLineAfterWrap={0};
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  AEPOINT *lpPointThree=NULL;
  POINT64 ptFirstVisLine;
  int nWrapCount;
  BOOL bNotify=FALSE;

  if (!ae->dwNotifySelChange && !ae->dwNotifyTextChange)
  {
    //Only if AE_UpdateWrap is not part of the other text modification operation
    bNotify=TRUE;
    AE_NotifyChanging(ae, AETCT_WRAP);
  }
  lpPointOne=AE_StackPointInsert(ae, &ciSelStart);
  lpPointTwo=AE_StackPointInsert(ae, &ciSelEnd);
  if (!ae->popt->nVScrollLock)
  {
    AE_GetIndex(ae, AEGI_FIRSTVISIBLELINE, NULL, &ciFirstVisibleLineAfterWrap);
    lpPointThree=AE_StackPointInsert(ae, &ciFirstVisibleLineAfterWrap);
  }
  nWrapCount=AE_WrapLines(ae, liWrapStart, liWrapEnd, dwWrap);

  //First point
  ciSelStart=lpPointOne->ciPoint;
  AE_StackPointDelete(ae, lpPointOne);

  //Second point
  ciSelEnd=lpPointTwo->ciPoint;
  AE_StackPointDelete(ae, lpPointTwo);

  //Third point
  if (!ae->popt->nVScrollLock)
  {
    ciFirstVisibleLineAfterWrap=lpPointThree->ciPoint;
    AE_StackPointDelete(ae, lpPointThree);
  }

  ae->ptCaret.x=0;
  ae->ptCaret.y=0;
  if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
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
    ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
    AE_UpdateScrollBars(ae, SB_VERT);
  }
  AE_UpdateSelection(ae, AESELT_COLUMNASIS|AESELT_LOCKSCROLL|AESELT_RESETSELECTION|AESELT_LOCKNOTIFY);
  if (!ae->popt->nVScrollLock)
  {
    ptFirstVisLine.y=AE_VPosFromLine(ae, ciFirstVisibleLineAfterWrap.nLine);
    AE_ScrollToPointEx(ae, AESC_POINTGLOBAL|AESC_OFFSETPIXELY|AESC_FORCETOP, &ptFirstVisLine, 0, 0);
  }

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
  AEPOINT *lpPoint;
  AEPOINT *lpTmpPoint;
  AELINEINDEX liFirst;
  AELINEINDEX liCount;
  DWORD dwMaxWidth=0;
  DWORD dwStartTime=GetTickCount();
  DWORD dwProgressTime=0;
  DWORD dwCurrentTime=0;
  int nLineOffset=0;
  int nWrapped=0;
  int nUnwrapped=0;
  int nStopLine;
  BOOL bPrevLine=FALSE;
  DWORD dwStopProgress=0;

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
    liCount.lpLine=ae->ptxt->hLinesStack.first;
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
      if (dwStopProgress=AE_NotifyProgress(ae, AEPGS_WRAPTEXT, GetTickCount() - dwStartTime, 0, nStopLine))
        return 0;
    }
  }

  //Skip points that located before start wrap
  for (lpPoint=ae->ptxt->hPointsStack.first; lpPoint; lpPoint=lpPoint->next)
  {
    if (lpPoint->ciPoint.nLine >= liFirst.nLine)
      break;
  }

  while (liCount.lpLine)
  {
    if (dwWrap)
    {
      if (liCount.lpLine->nLineWidth == -1)
        AE_GetLineWidth(ae, liCount.lpLine);

      if (liCount.lpLine->nLineLen > 1 && (DWORD)liCount.lpLine->nLineWidth > dwMaxWidth)
      {
        if (nWrapped=AE_LineWrap(ae, &liCount, (liCount.nLine == liFirst.nLine)?&liFirst:NULL, &liCount, dwMaxWidth, dwWrap, &lpPoint, nLineOffset))
        {
          nLineOffset+=nWrapped;
        }
        else break;
      }
    }
    if (nUnwrapped=AE_LineUnwrap(ae, &liCount, dwMaxWidth, &lpPoint, nLineOffset))
    {
      nLineOffset+=nUnwrapped;

      if (liCount.nLine == liFirst.nLine)
        liFirst.lpLine=liCount.lpLine;
      if (dwWrap) continue;
    }

    if (liCount.nLine >= nStopLine + nLineOffset)
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
          if (dwStopProgress=AE_NotifyProgress(ae, AEPGS_WRAPTEXT, dwCurrentTime - dwStartTime, liCount.nLine - nLineOffset, nStopLine))
            break;
          dwProgressTime=GetTickCount();
        }
      }
    }
  }

  //Update points
  for (lpTmpPoint=lpPoint; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
  {
    if (!(lpTmpPoint->dwFlags & AEPTF_VALIDLINE))
    {
      lpTmpPoint->ciPoint.nLine+=nLineOffset;
      lpTmpPoint->dwFlags|=AEPTF_MOVELINE;
    }
  }

  //Remove AEPTF_VALIDLINE flag
  if (!lpPoint)
    lpPoint=ae->ptxt->hPointsStack.last;
  for (lpTmpPoint=lpPoint; lpTmpPoint; lpTmpPoint=lpTmpPoint->prev)
  {
    if (lpTmpPoint->ciPoint.nLine < liFirst.nLine)
      break;
    lpTmpPoint->dwFlags&=~AEPTF_VALIDLINE;
  }
  for (lpTmpPoint=lpPoint; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
  {
    if (lpTmpPoint->ciPoint.nLine > liCount.nLine)
      break;
    lpTmpPoint->dwFlags&=~AEPTF_VALIDLINE;
  }
  //for (lpTmpPoint=ae->ptxt->hPointsStack.first; lpTmpPoint; lpTmpPoint=lpTmpPoint->next)
  //{
  //  if (lpTmpPoint->dwFlags & AEPTF_VALIDLINE)
  //    MessageBox(NULL, "Error: found AEPTF_VALIDLINE", NULL, 0);
  //}

  //End progress
  if (ae->popt->dwEventMask & AENM_PROGRESS)
  {
    if (!liWrapStart && !liWrapEnd)
    {
      if (!dwStopProgress)
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
      ae->liFirstDrawLine.nLine+=nLineOffset;
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
      ae->ptxt->liMaxWidthLine.nLine+=nLineOffset;
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
      ae->ptxt->liLineUnwrapLastCall.nLine+=nLineOffset;
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
      ae->ciLastCallIndex.nLine+=nLineOffset;
    }
    else if (ae->ciLastCallIndex.nLine >= liFirst.nLine)
    {
      ae->ciLastCallIndex.lpLine=NULL;
      ae->nLastCallOffset=0;
    }
  }
  if (nLineOffset)
    ae->ptxt->lpVPosFold=NULL;
  return nLineOffset;
}

int AE_LineWrap(AKELEDIT *ae, const AELINEINDEX *liLine, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwMaxWidth, DWORD dwWrap, AEPOINT **lppPoint, int nLineOffset)
{
  AELINEINDEX liStart=*liLine;
  AELINEINDEX liEnd=*liLine;
  AELINEDATA ldLine;
  AELINEDATA *lpInitialElement=liLine->lpLine;
  AELINEDATA *lpNewElement=NULL;
  AEPOINT *lpFirstPoint=NULL;
  AEPOINT *lpTmpPoint=*lppPoint;
  AEPOINT *lpCountPoint;
  int nCharStart=0;
  int nCharEnd=0;
  int nWrappedLines=0;
  int nWrapMoveSet=0;
  int nTmpLineOffset;
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
              if (AE_IsInDelimiterList(ae->ptxt->wszWrapDelimiters, ae->ptxt->nWrapDelimitersLen, lpInitialElement->wpLine[i]))
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
              if (lpTmpPoint->dwFlags & AEPTF_VALIDLINE)
                nTmpLineOffset=0;
              else
                nTmpLineOffset=nLineOffset;

              if (lpTmpPoint->ciPoint.nLine + nTmpLineOffset > liStart.nLine)
                break;

              if (lpTmpPoint->ciPoint.nLine + nTmpLineOffset == liStart.nLine)
              {
                //if (lpTmpPoint->ciPoint.nCharInLine > lpInitialElement->nLineLen)
                //  MessageBox(NULL, "Point wrap error", NULL, 0);
                if (lpTmpPoint->ciPoint.nCharInLine > nCharEnd)
                  break;

                if (lpTmpPoint->ciPoint.nCharInLine >= nCharStart)
                {
                  lpTmpPoint->ciPoint.nLine+=(nTmpLineOffset + nWrappedLines);
                  lpTmpPoint->ciPoint.lpLine=lpNewElement;
                  lpTmpPoint->ciPoint.nCharInLine-=nCharStart;
                  lpTmpPoint->dwFlags|=AEPTF_MOVELINE|AEPTF_VALIDLINE;
                  if ((lpTmpPoint->dwFlags & AEPTF_WRAPMOVE) && lpNewElement->nLineLen == lpTmpPoint->ciPoint.nCharInLine && lpNewElement->nLineBreak == AELB_WRAP)
                  {
                    lpTmpPoint->dwFlags|=AEPTF_WRAPMOVESET;
                    ++nWrapMoveSet;
                  }
                  if (nCharEnd == lpInitialElement->nLineLen && !lpFirstPoint)
                    lpFirstPoint=lpTmpPoint;
                }
              }
              else
              {
                lpTmpPoint->ciPoint.nLine+=nTmpLineOffset;
                lpTmpPoint->dwFlags|=AEPTF_MOVELINE|AEPTF_VALIDLINE;
              }
            }
            nCharStart=nCharEnd;

            if (nCharEnd < lpInitialElement->nLineLen)
            {
              ++nWrappedLines;
              continue;
            }
          }
          else
          {
            AE_StackLineDelete(ae, lpNewElement);
            lpNewElement=NULL;
          }
        }
      }
      break;
    }

    if (nWrappedLines)
    {
      AE_StackLineDelete(ae, lpInitialElement);

      liEnd.lpLine=lpNewElement;
      liEnd.nLine+=nWrappedLines;

      if (nWrapMoveSet)
      {
        //Move point to the next line if it located at wrap place.
        for (lpCountPoint=*lppPoint; lpCountPoint; lpCountPoint=lpCountPoint->next)
        {
          if (lpCountPoint->dwFlags & AEPTF_WRAPMOVESET)
          {
            AEC_NextLine(&lpCountPoint->ciPoint);
            lpCountPoint->dwFlags&=~AEPTF_WRAPMOVESET;
            //Include corrected points to lpTmpPoint for following AE_LineUnwrap
            if (lpCountPoint->ciPoint.nLine == liEnd.nLine && (!lpTmpPoint || lpTmpPoint->ciPoint.nLine > liEnd.nLine))
              lpTmpPoint=lpCountPoint;
          }
        }
      }
    }
    if (!lpFirstPoint) lpFirstPoint=lpTmpPoint;
    *lppPoint=lpFirstPoint;
  }
  if (liWrapStart) *liWrapStart=liStart;
  if (liWrapEnd) *liWrapEnd=liEnd;
  return nWrappedLines;
}

int AE_LineUnwrap(AKELEDIT *ae, AELINEINDEX *liLine, DWORD dwMaxWidth, AEPOINT **lppPoint, int nLineOffset)
{
  AELINEINDEX liCurLine;
  AELINEDATA *lpNewElement;
  AELINEDATA *lpCurElement;
  AELINEDATA *lpNextElement;
  AEPOINT *lpTmpPoint=*lppPoint;
  AEPOINT *lpFirstPoint=NULL;
  DWORD dwUnwrapLineWidth=0;
  DWORD dwUnwrapLineLen=0;
  DWORD dwCountWidth=0;
  DWORD dwCountLen=0;
  BYTE nUnwrapLineBreak=AELB_EOF;
  int nUnwrappedLines=0;
  int nTmpLineOffset;

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
          if ((DWORD)liLine->lpLine->nLineWidth + AE_GetCharWidth(ae, liLine->lpLine->next->wpLine, 0) > dwMaxWidth)
            return 0;
        }
      }

      //Calculate unwrapped line size
      if (liLine->lpLine->nLineWidth == -1)
        AE_GetLineWidth(ae, liLine->lpLine);
      dwUnwrapLineWidth+=liLine->lpLine->nLineWidth;
      dwUnwrapLineLen+=liLine->lpLine->nLineLen;

      for (lpCurElement=liLine->lpLine->next; lpCurElement; lpCurElement=lpCurElement->next)
      {
        //Set new line width (necessary for tabs)
        lpCurElement->nLineWidth=(int)AE_GetStringWidth(ae, lpCurElement->wpLine, lpCurElement->nLineLen, dwUnwrapLineWidth);
        dwUnwrapLineWidth+=lpCurElement->nLineWidth;
        dwUnwrapLineLen+=lpCurElement->nLineLen;

        if (dwUnwrapLineWidth >= dwMaxWidth || lpCurElement->nLineBreak != AELB_WRAP)
        {
          nUnwrapLineBreak=lpCurElement->nLineBreak;
          break;
        }
      }

      //Unwrap line
      if (lpNewElement=AE_StackLineInsertBefore(ae, liLine->lpLine))
      {
        lpNewElement->nLineWidth=dwUnwrapLineWidth;
        lpNewElement->nLineBreak=nUnwrapLineBreak;
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
              if (lpTmpPoint->dwFlags & AEPTF_VALIDLINE)
                nTmpLineOffset=0;
              else
                nTmpLineOffset=nLineOffset;

              if (lpTmpPoint->ciPoint.nLine + nTmpLineOffset > liCurLine.nLine)
                break;

              if (lpTmpPoint->ciPoint.nLine + nTmpLineOffset == liCurLine.nLine)
              {
                lpTmpPoint->ciPoint.nLine+=(nTmpLineOffset + nUnwrappedLines);
                lpTmpPoint->ciPoint.lpLine=lpNewElement;
                lpTmpPoint->ciPoint.nCharInLine+=(dwCountLen - liCurLine.lpLine->nLineLen);
                lpTmpPoint->dwFlags|=AEPTF_MOVELINE|AEPTF_VALIDLINE;
                if (!lpFirstPoint) lpFirstPoint=lpTmpPoint;
              }
              else
              {
                lpTmpPoint->ciPoint.nLine+=nTmpLineOffset;
                lpTmpPoint->dwFlags|=AEPTF_MOVELINE|AEPTF_VALIDLINE;
              }
            }

            if (dwCountWidth >= dwMaxWidth || liCurLine.lpLine->nLineBreak != AELB_WRAP)
            {
              AE_StackLineDelete(ae, liCurLine.lpLine);
              break;
            }

            --nUnwrappedLines;
            lpNextElement=liCurLine.lpLine->next;
            AE_StackLineDelete(ae, liCurLine.lpLine);
            liCurLine.nLine+=1;
            liCurLine.lpLine=lpNextElement;
          }
          lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
        }
        else
        {
          AE_StackLineDelete(ae, lpNewElement);
          lpNewElement=NULL;
        }
      }

      if (nUnwrappedLines)
      {
        liLine->lpLine=lpNewElement;
      }
      if (!lpFirstPoint) lpFirstPoint=lpTmpPoint;
      *lppPoint=lpFirstPoint;
    }
  }
  return nUnwrappedLines;
}

void AE_CalcLinesWidth(AKELEDIT *ae, const AELINEINDEX *liStartLine, const AELINEINDEX *liEndLine, DWORD dwFlags)
{
  AELINEINDEX liStart;
  AELINEINDEX liEnd;
  AELINEINDEX liMaxWidthLine;
  INT_PTR nHScrollMax=0;

  if (!liStartLine)
  {
    liStart.nLine=0;
    liStart.lpLine=ae->ptxt->hLinesStack.first;
  }
  else liStart=*liStartLine;

  if (!liEndLine)
  {
    liEnd.nLine=ae->ptxt->nLineCount;
    liEnd.lpLine=ae->ptxt->hLinesStack.last;
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
      if (!(dwFlags & AECLW_LOCKUPDATEHSCROLL))
        AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }
  else
  {
    if (nHScrollMax > ae->ptxt->nHScrollMax)
    {
      ae->ptxt->liMaxWidthLine=liMaxWidthLine;
      ae->ptxt->nHScrollMax=nHScrollMax;
      if (!(dwFlags & AECLW_LOCKUPDATEHSCROLL))
        AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }
}

int AE_CheckCodepage(AKELEDIT *ae, int nCodePage, int *lpdwCharInLine)
{
  AELINEDATA *lpLine=ae->ptxt->hLinesStack.first;
  char *szLine;
  char *pCount;
  char chChar=0x00;
  int nLine=1;
  int nLineLen;
  BOOL bUsedDefaultChar=FALSE;

  while (lpLine)
  {
    if (lpLine->nLineLen)
    {
      WideCharToMultiByte(nCodePage, WC_NO_BEST_FIT_CHARS, lpLine->wpLine, lpLine->nLineLen, NULL, 0, NULL, &bUsedDefaultChar);

      if (bUsedDefaultChar)
      {
        //Get character in line
        if (lpdwCharInLine)
        {
          nLineLen=WideCharToMultiByte(nCodePage, WC_NO_BEST_FIT_CHARS, lpLine->wpLine, lpLine->nLineLen, NULL, 0, NULL, NULL);

          if (szLine=(char *)AE_HeapAlloc(ae, 0, nLineLen + 1))
          {
            WideCharToMultiByte(nCodePage, WC_NO_BEST_FIT_CHARS, lpLine->wpLine, lpLine->nLineLen + 1, szLine, nLineLen + 1, &chChar, NULL);
            for (pCount=szLine; *pCount != chChar; ++pCount);
            *lpdwCharInLine=(DWORD)(pCount - szLine) + 1;
            AE_HeapFree(ae, 0, (LPVOID)szLine);
          }
        }
        return nLine;
      }
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
    ae->rcDraw.top+=1;
  }

  if (bRedraw) InvalidateRect(ae->hWndEdit, NULL, TRUE);
}

void AE_GetFontCharWidth(AKELEDIT *ae, HDC hDC)
{
  SIZE64 sizeWidth;
  HDC hTempDC=ae->hDC;
  int nStrLen=52;
  const wchar_t *wpStr=L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  const wchar_t *wpMaxStr=wpStr + nStrLen;
  const wchar_t *wpCount;

  ae->hDC=hDC;
  ae->ptxt->nMaxCharWidth=0;
  ae->ptxt->nAveCharWidth=0;
  AE_GetTextExtentPoint32(ae, wpStr, nStrLen, &sizeWidth);
  for (wpCount=wpStr; wpCount < wpMaxStr; ++wpCount)
  {
    if (ae->ptxt->nMaxCharWidth < ae->ptxt->lpCharWidths[*wpCount])
      ae->ptxt->nMaxCharWidth=ae->ptxt->lpCharWidths[*wpCount];
    ae->ptxt->nAveCharWidth+=ae->ptxt->lpCharWidths[*wpCount];
  }
  ae->ptxt->nAveCharWidth/=nStrLen;

  AE_GetTextExtentPoint32(ae, L" ", 1, &sizeWidth);
  ae->ptxt->nSpaceCharWidth=(int)sizeWidth.cx;
  ae->ptxt->nTabWidth=ae->ptxt->nSpaceCharWidth * ae->ptxt->nTabStop;
  ae->ptxt->nFixedCharWidth=AE_GetFixedCharWidth(ae);
  ae->ptxt->nFixedTabWidth=ae->ptxt->nFixedCharWidth * ae->ptxt->nTabStop;
  ae->hDC=hTempDC;
}

int AE_GetFixedCharWidth(AKELEDIT *ae)
{
  if (ae->ptxt->nInitFixedCharWidth < 0)
  {
    if (ae->ptxt->nInitFixedCharWidth == -AECS_AVEWIDTH)
      return ae->ptxt->nAveCharWidth;
    if (ae->ptxt->nInitFixedCharWidth == -AECS_SPACEWIDTH)
      return ae->ptxt->nSpaceCharWidth;
    if (ae->ptxt->nInitFixedCharWidth == -AECS_MAXWIDTH)
      return ae->ptxt->nMaxCharWidth;
  }
  else return ae->ptxt->nInitFixedCharWidth;

  return 0;
}

void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bRedraw)
{
  AEFONTITEMA *fi;
  TEXTMETRICA tmEdit;
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

    ae->ptxt->nPointSize=MulDiv(mod(ae->ptxt->lfFontA.lfHeight), 72, GetDeviceCaps(hDC, LOGPIXELSY));
    ae->ptxt->nCharHeight=tmEdit.tmHeight + ae->ptxt->nLineGap;
    ae->ptxt->nCharHeight=max(ae->ptxt->nCharHeight, 1);
    AE_GetFontCharWidth(ae, hDC);
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

    ae->ptxt->nPointSize=MulDiv(mod(ae->ptxt->lfFontW.lfHeight), 72, GetDeviceCaps(hDC, LOGPIXELSY));
    ae->ptxt->nCharHeight=tmEdit.tmHeight + ae->ptxt->nLineGap;
    ae->ptxt->nCharHeight=max(ae->ptxt->nCharHeight, 1);
    AE_GetFontCharWidth(ae, hDC);
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, bRedraw);

    if (!ae->hDC)
    {
      if (hFontOld) SelectObject(hDC, hFontOld);
      ReleaseDC(ae->hWndEdit, hDC);
    }
  }
}

void AE_GetLineSelection(AKELEDIT *ae, const AELINEINDEX *liLine, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, INT_PTR nSelStartX, INT_PTR nSelEndX, int *nSelStartIndexInLine, int *nSelEndIndexInLine, BOOL bColumnSel)
{
  if (ciSelStart->nLine > liLine->nLine || ciSelEnd->nLine < liLine->nLine)
  {
    *nSelStartIndexInLine=0;
    *nSelEndIndexInLine=0;
    return;
  }
  if (ciSelStart->nLine == ciSelEnd->nLine && ciSelStart->nCharInLine == ciSelEnd->nCharInLine)
  {
    *nSelStartIndexInLine=ciSelStart->nCharInLine;
    *nSelEndIndexInLine=ciSelEnd->nCharInLine;
    return;
  }

  if (bColumnSel)
  {
    INT_PTR nStartX=min(nSelStartX, nSelEndX);
    INT_PTR nEndX=max(nSelStartX, nSelEndX);
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
    *nSelStartIndexInLine=nSelStart;
    *nSelEndIndexInLine=nSelEnd;
  }
  else
  {
    if ((ciSelStart->nLine < liLine->nLine || (ciSelStart->nLine == liLine->nLine && ciSelStart->nCharInLine == 0)) &&
        (ciSelEnd->nLine > liLine->nLine || (ciSelEnd->nLine == liLine->nLine && ciSelEnd->nCharInLine == ciSelEnd->lpLine->nLineLen)))
    {
      *nSelStartIndexInLine=0;
      *nSelEndIndexInLine=liLine->lpLine->nLineLen;
      return;
    }
    if (ciSelStart->nLine == liLine->nLine)
      *nSelStartIndexInLine=ciSelStart->nCharInLine;
    else
      *nSelStartIndexInLine=0;
    if (ciSelEnd->nLine == liLine->nLine)
      *nSelEndIndexInLine=ciSelEnd->nCharInLine;
    else
      *nSelEndIndexInLine=liLine->lpLine->nLineLen;
  }
}

void AE_GetSelLines(AKELEDIT *ae, const AECHARINDEX *ciMin, const AECHARINDEX *ciMax, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, INT_PTR nSelStartX, INT_PTR nSelEndX, BOOL bColumnSel)
{
  AELINEINDEX liLine;

  liLine.nLine=ciMin->nLine;
  liLine.lpLine=ciMin->lpLine;

  while (liLine.lpLine)
  {
    AE_GetLineSelection(ae, &liLine, ciSelStart, ciSelEnd, nSelStartX, nSelEndX, &liLine.lpLine->nSelStart, &liLine.lpLine->nSelEnd, bColumnSel);
    if (liLine.lpLine == ciMax->lpLine) break;

    ++liLine.nLine;
    liLine.lpLine=liLine.lpLine->next;
  }
}

void AE_ClearSelLines(const AECHARINDEX *ciMin, const AECHARINDEX *ciMax)
{
  AELINEDATA *lpElement;

  if (AEC_IndexCompare(ciMin, ciMax))
  {
    for (lpElement=ciMin->lpLine; lpElement; lpElement=lpElement->next)
    {
      lpElement->nSelStart=0;
      lpElement->nSelEnd=0;
      if (lpElement == ciMax->lpLine) break;
    }
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
  POINT64 ptSelStart;
  POINT64 ptSelEnd;
  BOOL bColumnSelOld;

  if (ae->popt->dwOptionsEx & AECOE_LOCKSELECTION)
    return;
  if (ae->popt->dwOptions & AECO_NOSCROLLSELECTALL)
  {
    //Is first char
    if (ciSelStart->nLine == 0 &&
        ciSelStart->lpLine == ae->ptxt->hLinesStack.first &&
        ciSelStart->nCharInLine == 0)
    {
      //Is last char
      if (ciSelEnd->nLine == ae->ptxt->nLineCount &&
          ciSelEnd->lpLine == ae->ptxt->hLinesStack.last &&
          ciSelEnd->nCharInLine == ciSelEnd->lpLine->nLineLen)
      {
        dwSelFlags|=AESELT_LOCKSCROLL;
      }
    }
  }
  if (!(dwSelFlags & AESELT_LOCKUNDOGROUPING))
    AE_StackUndoGroupStop(ae);
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
    if (AEC_IndexCompare(&ciSelStartNew, &ciSelEndNew) > 0)
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
      if (AEC_IndexCompare(&ae->ciSelStartIndex, &ciSelStartNew))
        if (ptSelStart.x > max(ae->ptxt->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
          return;

      AE_GetPosFromChar(ae, &ciSelEndNew, &ptSelEnd, NULL);
      if (AEC_IndexCompare(&ae->ciSelEndIndex, &ciSelEndNew))
        if (ptSelEnd.x > max(ae->ptxt->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->ptxt->nAveCharWidth)
          return;
    }
    else
    {
      ciSelStartNew.nCharInLine=min(ciSelStartNew.nCharInLine, ciSelStartNew.lpLine->nLineLen);
      ciSelEndNew.nCharInLine=min(ciSelEndNew.nCharInLine, ciSelEndNew.lpLine->nLineLen);
      ptSelStart.x=0;
      ptSelEnd.x=0;

      if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
      {
        if (AEC_IndexCompare(&ciCaretOld, &ciCaretNew))
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
      AE_ClearSelLines(&ciSelStartOld, &ciSelEndOld);
      AE_GetSelLines(ae, &ciSelStartNew, &ciSelEndNew, &ciSelStartNew, &ciSelEndNew, ptSelStart.x, ptSelEnd.x, bColumnSel);
    }
    else
    {
      if (AEC_IndexCompare(&ciSelStartOld, &ciSelStartNew))
      {
        if (ciSelStartOld.nLine < ciSelStartNew.nLine)
        {
          AE_ClearSelLines(&ciSelStartOld, &ciSelStartNew);
          AE_GetSelLines(ae, &ciSelStartNew, &ciSelStartNew, &ciSelStartNew, &ciSelEndNew, ptSelStart.x, ptSelEnd.x, bColumnSel);
        }
        else AE_GetSelLines(ae, &ciSelStartNew, &ciSelStartOld, &ciSelStartNew, &ciSelEndNew, ptSelStart.x, ptSelEnd.x, bColumnSel);
      }
      if (AEC_IndexCompare(&ciSelEndNew, &ciSelEndOld))
      {
        if (ciSelEndNew.nLine < ciSelEndOld.nLine)
        {
          AE_ClearSelLines(&ciSelEndNew, &ciSelEndOld);
          AE_GetSelLines(ae, &ciSelEndNew, &ciSelEndNew, &ciSelStartNew, &ciSelEndNew, ptSelStart.x, ptSelEnd.x, bColumnSel);
        }
        else AE_GetSelLines(ae, &ciSelEndOld, &ciSelEndNew, &ciSelStartNew, &ciSelEndNew, ptSelStart.x, ptSelEnd.x, bColumnSel);
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
      AE_ScrollToCaret(ae, &ae->ptCaret, dwSelFlags, dwSelType);
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
          if (AEC_IndexCompare(&ciSelStartOld, &ciSelStartNew))
            AE_RedrawLineRange(ae, min(ciSelStartOld.nLine, ciSelStartNew.nLine), max(ciSelStartOld.nLine, ciSelStartNew.nLine), FALSE);
          if (AEC_IndexCompare(&ciSelEndOld, &ciSelEndNew))
            AE_RedrawLineRange(ae, min(ciSelEndOld.nLine, ciSelEndNew.nLine), max(ciSelEndOld.nLine, ciSelEndNew.nLine), FALSE);

          if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
          {
            if (!AEC_IndexCompare(&ciSelStartOld, &ciSelStartNew) &&
                !AEC_IndexCompare(&ciSelEndOld, &ciSelEndNew) &&
                 AEC_IndexCompare(&ciCaretOld, &ciCaretNew))
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
  if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelStartIndex) <= 0)
    AE_SetSelectionPos(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, FALSE, dwSelFlags, AESCT_UPDATESELECTION);
  else
    AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ae->ciSelStartIndex, FALSE, dwSelFlags, AESCT_UPDATESELECTION);
}

void AE_SetMouseSelection(AKELEDIT *ae, const POINT *ptPos, BOOL bColumnSel, BOOL bShift)
{
  AECHARINDEX ciCharIndex;
  AECHARINDEX ciSelEnd;
  int dwMouseSelType=ae->dwMouseSelType;
  int nPosResult;

  if (ae->rcDraw.bottom - ae->rcDraw.top > 0 && ae->rcDraw.right - ae->rcDraw.left > 0)
  {
    nPosResult=AE_GetCharFromPos(ae, ptPos->x, ptPos->y, &ciCharIndex, NULL, bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

    if (ae->nCurrentCursor == AECC_MARGIN && (dwMouseSelType & AEMSS_WORDS))
      dwMouseSelType=AEMSS_LINES;

    //One click (capture)
    if (dwMouseSelType & AEMSS_CHARS)
    {
      if (bShift)
      {
        if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciSelEnd=ae->ciCaretIndex;
        else if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
          ciSelEnd=ae->ciSelStartIndex;
        else
          ciSelEnd=ae->ciSelEndIndex;

        //Margin selection
        if (ae->nCurrentCursor == AECC_MARGIN)
        {
          if (AEC_IndexCompare(&ciCharIndex, &ciSelEnd) < 0)
          {
            if (ae->popt->dwOptions & AECO_SELUNWRAPLINE)
              AEC_WrapLineBeginEx(&ciCharIndex, &ciCharIndex);
            else
              ciCharIndex.nCharInLine=0;
          }
          else
          {
            if (ae->popt->dwOptions & AECO_SELUNWRAPLINE)
              AEC_WrapLineEndEx(&ciCharIndex, &ciCharIndex);
            else
              ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
            if (!(ae->popt->dwOptions & AECO_NONEWLINEMOUSESELECT))
              AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDLINE, &ciCharIndex, &ciCharIndex);
          }
        }
        //Characters selection
        else if (!AEC_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
        {
          if (ptPos->x < ae->rcDraw.left)
          {
            AEC_IndexDec(&ciCharIndex);
            ciCharIndex.nCharInLine=max(ciCharIndex.nCharInLine, 0);
          }
          else if (ptPos->x > ae->rcDraw.right)
          {
            AEC_IndexInc(&ciCharIndex);
          }
        }

        if (AEC_IndexCompare(&ae->ciCaretIndex, &ciCharIndex) || ae->bColumnSel != bColumnSel)
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
        if (AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelClick) < 0 ||
            (!AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelClick) && nPosResult == AEPC_AFTER))
        {
          if (AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelStart) < 0 ||
             (!AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelStart) && nPosResult == AEPC_AFTER))
            AE_GetPrevBreak(ae, &ciCharIndex, &ciCharIndex, bColumnSel, ae->popt->dwWordBreak|(nPosResult == AEPC_BEFORE?AEWB_MINMOVE:0));
          else
            ciCharIndex=ae->ciMouseSelStart;
          ciSelEnd=ae->ciMouseSelEnd;
        }
        else
        {
          if (AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelEnd) > 0 ||
             (!AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelEnd) && nPosResult == AEPC_BEFORE))
            AE_GetNextBreak(ae, &ciCharIndex, &ciCharIndex, bColumnSel, ae->popt->dwWordBreak|(nPosResult == AEPC_AFTER?AEWB_MINMOVE:0));
          else
            ciCharIndex=ae->ciMouseSelEnd;
          ciSelEnd=ae->ciMouseSelStart;
        }

        if (AEC_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
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
        if (AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelClick) < 0)
        {
          if (AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelStart) < 0)
          {
            if (ae->popt->dwOptions & AECO_SELUNWRAPLINE)
              AEC_WrapLineBeginEx(&ciCharIndex, &ciCharIndex);
            else
              ciCharIndex.nCharInLine=0;
          }
          else
            ciCharIndex=ae->ciMouseSelStart;
          ciSelEnd=ae->ciMouseSelEnd;
        }
        else
        {
          if (AEC_IndexCompare(&ciCharIndex, &ae->ciMouseSelEnd) >= 0)
          {
            if (ae->popt->dwOptions & AECO_SELUNWRAPLINE)
              AEC_WrapLineEndEx(&ciCharIndex, &ciCharIndex);
            else
              ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
            if (!(ae->popt->dwOptions & AECO_NONEWLINEMOUSESELECT))
              AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDLINE, &ciCharIndex, &ciCharIndex);
          }
          else
            ciCharIndex=ae->ciMouseSelEnd;
          ciSelEnd=ae->ciMouseSelStart;
        }

        if (AEC_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
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
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos - (ae->ptMButtonDown.x - ptPos.x - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MLEFTTOP)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos - (ae->ptMButtonDown.x - ptPos.x - 5) * ae->popt->dwMScrollSpeed / 100);
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->ptMButtonDown.y - ptPos.y - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MTOP)
    {
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->ptMButtonDown.y - ptPos.y - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MRIGHTTOP)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos + (ptPos.x - ae->ptMButtonDown.x - 5) * ae->popt->dwMScrollSpeed / 100);
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->ptMButtonDown.y - ptPos.y - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MRIGHT)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos + (ptPos.x - ae->ptMButtonDown.x - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MRIGHTBOTTOM)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos + (ptPos.x - ae->ptMButtonDown.x - 5) * ae->popt->dwMScrollSpeed / 100);
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ptPos.y - ae->ptMButtonDown.y - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MBOTTOM)
    {
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ptPos.y - ae->ptMButtonDown.y - 5) * ae->popt->dwMScrollSpeed / 100);
    }
    else if (ae->nCurrentCursor == AECC_MLEFTBOTTOM)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos - (ae->ptMButtonDown.x - ptPos.x - 5) * ae->popt->dwMScrollSpeed / 100);
      AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ptPos.y - ae->ptMButtonDown.y - 5) * ae->popt->dwMScrollSpeed / 100);
    }
  }
  if (ae->dwMouseMoveTimer)
  {
    GetCursorPos(&ptPos);
    ScreenToClient(ae->hWndEdit, &ptPos);

    if (ae->nCurrentCursor == AECC_MARKER)
    {
      INT_PTR nMarkerPos;

      nMarkerPos=ae->nHScrollPos + (ptPos.x - ae->rcDraw.left);
      if (ae->popt->dwColumnMarkerType == AEMT_SYMBOL)
        nMarkerPos=(nMarkerPos / ae->ptxt->nAveCharWidth) + nMarkerPos % ae->ptxt->nAveCharWidth / (ae->ptxt->nAveCharWidth / 2);

      AE_ColumnMarkerSet(ae, ae->popt->dwColumnMarkerType, (int)nMarkerPos, TRUE);
    }
    else
    {
      AE_SetMouseSelection(ae, &ptPos, ae->bColumnSel, TRUE);
    }
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

void AE_GlobalToClient(AKELEDIT *ae, const POINT64 *ptGlobal, POINT64 *ptClient64, POINT *ptClient)
{
  if (ptClient64)
  {
    ptClient64->x=ae->rcDraw.left + (ptGlobal->x - ae->nHScrollPos);
    ptClient64->y=ae->rcDraw.top + (ptGlobal->y - ae->nVScrollPos);
  }
  if (ptClient)
  {
    ptClient->x=ae->rcDraw.left + (int)(ptGlobal->x - ae->nHScrollPos);
    ptClient->y=ae->rcDraw.top + (int)(ptGlobal->y - ae->nVScrollPos);
  }
}

void AE_ClientToGlobal(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY, POINT64 *ptGlobal)
{
  ptGlobal->x=ae->nHScrollPos + (nClientX - ae->rcDraw.left);
  ptGlobal->y=ae->nVScrollPos + (nClientY - ae->rcDraw.top);
}

int AE_SetCursor(AKELEDIT *ae)
{
  AECHARRANGE crLink;
  POINT ptPos;
  BOOL bAlt=FALSE;
  BOOL bShift=FALSE;
  int nResult=AECC_IBEAM;

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
    //nResult=AECC_IBEAM;
  }
  else
  {
    if (!bAlt && AE_IsPointOnMargin(ae, ptPos.x, ptPos.y) == AESIDE_LEFT)
    {
      if (!(ae->popt->dwOptions & AECO_NOMARGINSEL))
      {
        SetCursor(hAkelEditCursorMargin);
        nResult=AECC_MARGIN;
      }
    }
    else if (!bAlt && !bShift && AE_IsPointOnSelection(ae, ptPos.x, ptPos.y))
    {
      if (!(ae->popt->dwOptions & AECO_DISABLEDRAG))
      {
        SetCursor(hAkelEditCursorArrow);
        nResult=AECC_SELECTION;
      }
    }
    else if (!bAlt && !bShift && AE_IsPointOnUrl(ae, ptPos.x, ptPos.y, &crLink))
    {
      SetCursor(hAkelEditCursorHand);
      nResult=AECC_URL;
    }
    else if (!bAlt && bShift && AE_IsPointOnMarker(ae, ptPos.x, ptPos.y))
    {
      if (!(ae->popt->dwOptions & AECO_NOMARKERMOVE))
      {
        SetCursor(hAkelEditCursorMarker);
        nResult=AECC_MARKER;
      }
    }
  }

  if (ae->nCurrentCursor == AECC_URL || nResult == AECC_URL)
  {
    if (nResult == AECC_URL)
    {
      if (xmemcmp(&ae->crMouseOnLink, &crLink, sizeof(AECHARRANGE)))
      {
        //Mouse moved from one URL to another - remove highlight of first URL.
        if (ae->nCurrentCursor == AECC_URL)
          AE_RedrawLineRange(ae, ae->crMouseOnLink.ciMin.nLine, ae->crMouseOnLink.ciMax.nLine, TRUE);

        //Highlight current URL.
        xmemcpy(&ae->crMouseOnLink, &crLink, sizeof(AECHARRANGE));
        AE_RedrawLineRange(ae, ae->crMouseOnLink.ciMin.nLine, ae->crMouseOnLink.ciMax.nLine, TRUE);
      }
    }
    else
    {
      //Remove URL highlight.
      AE_RedrawLineRange(ae, ae->crMouseOnLink.ciMin.nLine, ae->crMouseOnLink.ciMax.nLine, TRUE);
      xmemset(&ae->crMouseOnLink, 0, sizeof(AECHARRANGE));
    }
  }
  return nResult;
}

DWORD AE_IsPointOnMargin(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY)
{
  DWORD dwSide=0;

  if (nClientX >= ae->rcEdit.left && nClientX < ae->rcDraw.left && nClientY > ae->rcEdit.top && nClientY < ae->rcEdit.bottom)
  {
    dwSide|=AESIDE_LEFT;
  }
  if (nClientY >= ae->rcEdit.top && nClientY < ae->rcDraw.top && nClientX > ae->rcEdit.left && nClientX < ae->rcEdit.right)
  {
    dwSide|=AESIDE_TOP;
  }
  if (nClientX > ae->rcDraw.right && nClientX <= ae->rcEdit.right && nClientY > ae->rcEdit.top && nClientY < ae->rcEdit.bottom)
  {
    dwSide|=AESIDE_RIGHT;
  }
  if (nClientY > ae->rcDraw.bottom && nClientY <= ae->rcEdit.bottom && nClientX > ae->rcEdit.left && nClientX < ae->rcEdit.right)
  {
    dwSide|=AESIDE_BOTTOM;
  }
  return dwSide;
}

BOOL AE_IsPointOnSelection(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY)
{
  AECHARINDEX ciCharIndex;
  POINT64 ptGlobal;
  POINT64 ptClient64;
  INT_PTR nSelStartY;
  INT_PTR nSelEndY;
  int nResult;

  if (AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    nSelStartY=(AE_VPosFromLine(ae, ae->ciSelStartIndex.nLine) - ae->nVScrollPos) + ae->rcDraw.top;
    nSelEndY=(AE_VPosFromLine(ae, ae->ciSelEndIndex.nLine + 1) - ae->nVScrollPos) + ae->rcDraw.top;

    if (nClientY >= nSelStartY && nClientY <= nSelEndY)
    {
      if (nResult=AE_GetCharFromPos(ae, nClientX, nClientY, &ciCharIndex, &ptGlobal, ae->bColumnSel))
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
            ciCharIndex.nLine < ae->ciSelEndIndex.nLine)
        {
          if (ae->popt->dwOptions & AECO_ENTIRENEWLINEDRAW)
            return TRUE;

          if (ciCharIndex.nCharInLine == ciCharIndex.lpLine->nLineLen)
          {
            AE_GlobalToClient(ae, &ptGlobal, &ptClient64, NULL);

            if (nClientX >= ptClient64.x && nClientX <= ptClient64.x + ae->ptxt->nAveCharWidth)
              return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

DWORD AE_IsPointOnUrl(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY, AECHARRANGE *crLink)
{
  if (ae->popt->dwOptionsEx & AECOE_DETECTURL)
  {
    AECHARINDEX ciCharIndex;
    POINT ptClient;
    int nResult;

    ptClient.x=(int)nClientX;
    ptClient.y=(int)nClientY;

    if (PtInRect(&ae->rcDraw, ptClient))
    {
      if (nClientY - ae->rcDraw.top <= ae->ptxt->nVScrollMax)
      {
        if (nResult=AE_GetCharFromPos(ae, nClientX, nClientY, &ciCharIndex, NULL, FALSE))
        {
          if (nResult == AEPC_AFTER)
          {
            AEC_IndexDec(&ciCharIndex);
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

BOOL AE_IsPointOnMarker(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY)
{
  if (ae->popt->dwColumnMarkerPos)
  {
    INT_PTR nMarkerPos;

    nMarkerPos=ae->popt->dwColumnMarkerPos;
    if (ae->popt->dwColumnMarkerType == AEMT_SYMBOL)
      nMarkerPos*=ae->ptxt->nAveCharWidth;
    nMarkerPos=ae->rcDraw.left + (nMarkerPos - ae->nHScrollPos);

    if (nMarkerPos >= nClientX - 1 && nMarkerPos <= nClientX + 1 &&
        nClientY >= ae->rcDraw.top && nClientY <= ae->rcDraw.bottom)
    {
      return TRUE;
    }
  }
  return FALSE;
}

AEURLVISIT* AE_UrlVisitInsert(AKELEDIT *ae, const AECHARRANGE *crUrl)
{
  AEURLVISIT *lpUrlVisit=NULL;

  if (!AE_HeapStackInsertIndex(ae, (stack **)&ae->ptxt->hUrlVisitStack.first, (stack **)&ae->ptxt->hUrlVisitStack.last, (stack **)&lpUrlVisit, -1, sizeof(AEURLVISIT)))
  {
    if (crUrl)
    {
      if (lpUrlVisit->nUrlTextLen=AE_GetTextRange(ae, &crUrl->ciMin, &crUrl->ciMax, NULL, 0, AELB_ASIS, FALSE, FALSE))
      {
        if (lpUrlVisit->pUrlText=(wchar_t *)AE_HeapAlloc(ae, 0, lpUrlVisit->nUrlTextLen * sizeof(wchar_t)))
        {
          lpUrlVisit->nUrlTextLen=AE_GetTextRange(ae, &crUrl->ciMin, &crUrl->ciMax, lpUrlVisit->pUrlText, (UINT_PTR)-1, AELB_ASIS, FALSE, FALSE);
        }
      }
    }
  }
  return lpUrlVisit;
}

AEURLVISIT* AE_UrlVisitGetByRange(AKELEDIT *ae, const AECHARRANGE *crUrl)
{
  AEFINDTEXTW ft;
  AEURLVISIT *lpUrlVisit;
  INT_PTR nUrlTextLen=0;

  if (!crUrl) return NULL;

  for (lpUrlVisit=ae->ptxt->hUrlVisitStack.first; lpUrlVisit; lpUrlVisit=lpUrlVisit->next)
  {
    if (!nUrlTextLen)
    {
      if (!(nUrlTextLen=AE_IndexSubtract(ae, &crUrl->ciMax, &crUrl->ciMin, AELB_R, FALSE, FALSE)))
        break;
    }

    if (lpUrlVisit->nUrlTextLen == nUrlTextLen)
    {
      ft.pText=lpUrlVisit->pUrlText;
      ft.dwTextLen=lpUrlVisit->nUrlTextLen;
      ft.dwFlags=0;
      ft.nNewLine=AELB_R;

      if (AE_IsMatch(ae, &ft, &crUrl->ciMin))
        return lpUrlVisit;
    }
  }
  return NULL;
}

AEURLVISIT* AE_UrlVisitGetByText(AKELEDIT *ae, const wchar_t *wpText)
{
  AEURLVISIT *lpUrlVisit;
  INT_PTR nUrlTextLen=xstrlenW(wpText);

  for (lpUrlVisit=ae->ptxt->hUrlVisitStack.first; lpUrlVisit; lpUrlVisit=lpUrlVisit->next)
  {
    if (lpUrlVisit->nUrlTextLen == nUrlTextLen)
    {
      if (!xstrcmpiW(lpUrlVisit->pUrlText, wpText))
        return lpUrlVisit;
    }
  }
  return NULL;
}

void AE_UrlVisitDelete(AKELEDIT *ae, AEURLVISIT *lpUrlVisit)
{
  if (lpUrlVisit->pUrlText) AE_HeapFree(NULL, 0, (LPVOID)lpUrlVisit->pUrlText);
  AE_HeapStackDelete(ae, (stack **)&ae->ptxt->hUrlVisitStack.first, (stack **)&ae->ptxt->hUrlVisitStack.last, (stack *)lpUrlVisit);
}

void AE_UrlVisitFree(AKELEDIT *ae)
{
  AEURLVISIT *lpUrlVisit;

  for (lpUrlVisit=ae->ptxt->hUrlVisitStack.first; lpUrlVisit; lpUrlVisit=lpUrlVisit->next)
  {
    if (lpUrlVisit->pUrlText) AE_HeapFree(NULL, 0, (LPVOID)lpUrlVisit->pUrlText);
  }
  AE_HeapStackClear(ae, (stack **)&ae->ptxt->hUrlVisitStack.first, (stack **)&ae->ptxt->hUrlVisitStack.last);
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
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    return 0;

  ft.dwFlags=0;
  ft.pText=NULL;
  ft.dwTextLen=(UINT_PTR)-1;
  ft.nNewLine=AELB_ASIS;

  //Find URL beginning (backward)
  ciCount=*ciChar;

  while (ciCount.lpLine)
  {
    while (ciCount.nCharInLine >= 0)
    {
      if (AEC_IndexCompare(&ciCount, &crRange->ciMax) < 0)
        goto FindUrlEnding;
      dwLinkLen+=AEC_IndexLen(&ciCount);
      if (dwLinkLen > ae->popt->dwUrlMaxLength)
        goto FindUrlEnding;
      if (AE_IsInDelimiterList(ae->popt->wszUrlRightDelimiters, ae->popt->nUrlRightDelimitersLen, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
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
      AEC_IndexDec(&ciCount);
    }

    if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
    {
      AEC_PrevLine(&ciCount);
      AEC_IndexDec(&ciCount);
    }
    else goto FindUrlEnding;
  }

  //Find URL ending (forward)
  FindUrlEnding:
  if (!dwBackwardMatch) return 0;
  dwLinkLen=dwBackwardMatch;
  ciCount=*ciChar;
  AEC_IndexInc(&ciCount);

  while (ciCount.nLine <= nLastLine)
  {
    while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
    {
      if (AE_IsInDelimiterList(ae->popt->wszUrlRightDelimiters, ae->popt->nUrlRightDelimitersLen, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
        goto End;
      dwLinkLen+=AEC_IndexLen(&ciCount);
      if (dwLinkLen > ae->popt->dwUrlMaxLength)
        goto End;
      AEC_IndexInc(&ciCount);
    }

    if (ciCount.lpLine->nLineBreak == AELB_WRAP)
      AEC_NextLine(&ciCount);
    else
      goto End;
  }

  End:
  crRange->ciMax=ciCount;
  return dwLinkLen;
}

BOOL AE_HighlightFindMarkText(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEMARKTEXTMATCH *mtm)
{
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  AECHARINDEX ciMaxRE;
  AESTACKMARKTEXT *lpMarkTextStack;
  AEMARKTEXTITEMW *lpMarkTextItem;
  BOOL bDefaultTheme=FALSE;

  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    return FALSE;

  NextTheme:
  if (bDefaultTheme || !ae->popt->lpActiveTheme)
  {
    lpMarkTextStack=&ae->ptxt->hMarkTextStack;
    bDefaultTheme=TRUE;
  }
  else lpMarkTextStack=&ae->popt->lpActiveTheme->hMarkTextStack;

  mtm->lpMarkText=NULL;

  if (lpMarkTextStack->first)
  {
    //Find mark beginning (backward)
    ciCount=*ciChar;
    ciMaxRE.lpLine=NULL;

    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine >= 0)
      {
        if (AEC_IndexCompare(&ciCount, &mtm->crMarkText.ciMax) < 0)
          goto EndTheme;

        //Is mark
        if (lpMarkTextItem=AE_HighlightIsMarkText(ae, &ft, &ciCount, &ciMaxRE, lpMarkTextStack))
        {
          mtm->lpMarkText=lpMarkTextItem;
          mtm->crMarkText.ciMin=ft.crFound.ciMin;
          mtm->crMarkText.ciMax=ft.crFound.ciMax;
          return TRUE;
        }
        if (dwSearchType & AEHF_ISFIRSTCHAR)
          goto EndTheme;
        AEC_IndexDec(&ciCount);
      }

      if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
      {
        AEC_PrevLine(&ciCount);
        AEC_IndexDec(&ciCount);
      }
      else break;
    }
  }

  EndTheme:
  if (!bDefaultTheme)
  {
    bDefaultTheme=TRUE;
    goto NextTheme;
  }
  return FALSE;
}

AEMARKTEXTITEMW* AE_HighlightIsMarkText(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, AECHARINDEX *ciMaxRE, AESTACKMARKTEXT *lpMarkTextStack)
{
  AEMARKTEXTITEMW *lpMarkTextItem;
  AEFINDTEXTW ftMatch;

  if (!ft) ft=&ftMatch;

  for (lpMarkTextItem=lpMarkTextStack->first; lpMarkTextItem; lpMarkTextItem=lpMarkTextItem->next)
  {
    if (lpMarkTextItem->dwFlags & AEHLF_REGEXP)
    {
      //One line limit
      if (!ciMaxRE->lpLine)
        AEC_WrapLineEndEx(ciChar, ciMaxRE);
      if (AE_IsMatchRE(lpMarkTextItem->lpREGroupStack, &ft->crFound, ciChar, ciMaxRE))
        return lpMarkTextItem;
    }
    else
    {
      ft->pText=lpMarkTextItem->pMarkText;
      ft->dwTextLen=lpMarkTextItem->nMarkTextLen;
      ft->dwFlags=0;
      if (lpMarkTextItem->dwFlags & AEHLF_MATCHCASE)
        ft->dwFlags|=AEFR_MATCHCASE;
      if (lpMarkTextItem->dwFlags & AEHLF_WHOLEWORD)
        ft->dwFlags|=AEFR_WHOLEWORD;
      ft->nNewLine=AELB_ASIS;

      if (AE_IsMatch(ae, ft, ciChar))
        return lpMarkTextItem;
    }
  }
  return NULL;
}

INT_PTR AE_HighlightFindMarkRange(AKELEDIT *ae, INT_PTR nCharOffset, AEMARKRANGEMATCH *mrm)
{
  AEMARKRANGEITEM *lpMarkRangeItem;
  BOOL bDefaultTheme=FALSE;

  NextTheme:
  if (bDefaultTheme || !ae->popt->lpActiveTheme)
  {
    lpMarkRangeItem=ae->ptxt->hMarkRangeStack.first;
    bDefaultTheme=TRUE;
  }
  else lpMarkRangeItem=ae->popt->lpActiveTheme->hMarkRangeStack.first;

  while (lpMarkRangeItem)
  {
    if (nCharOffset >= lpMarkRangeItem->crMarkRange.cpMin &&
        nCharOffset < lpMarkRangeItem->crMarkRange.cpMax)
    {
      mrm->lpMarkRange=lpMarkRangeItem;
      mrm->crMarkRange=lpMarkRangeItem->crMarkRange;
      return mrm->crMarkRange.cpMax - mrm->crMarkRange.cpMin;
    }
    lpMarkRangeItem=lpMarkRangeItem->next;
  }
  if (!bDefaultTheme)
  {
    bDefaultTheme=TRUE;
    goto NextTheme;
  }
  return 0;
}

INT_PTR AE_HighlightFindQuote(AKELEDIT *ae, const AECHARINDEX *ciChar, DWORD dwSearchType, AEHLPAINT *hlp)
{
  AEQUOTEMATCH *qm=&hlp->qm;
  AEFOLDMATCH *fm=&hlp->fm;
  AEQUOTEITEMW *lpParentQuote=qm->lpQuote;
  AECHARRANGE crParentQuoteStart;
  AECHARRANGE crParentQuoteEnd;
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  AECHARINDEX ciMaxLine;
  AECHARINDEX ciTmpCount;
  AECHARRANGE crTmpQuoteStart;
  STACKREGROUP *lpREGroupStack;
  AESTACKQUOTE *lpQuoteStack;
  AESTACKQUOTESTART *lpQuoteStartStack;
  AEQUOTEITEMW *lpQuoteItem;
  AEQUOTESTART *lpQuoteStart;
  AEQUOTEITEMHANDLE *lpQuoteItemHandle;
  AEDELIMITEMW *lpDelimItem=NULL;
  INT_PTR nParentQuoteLen=0;
  INT_PTR nQuoteLen=0;
  INT_PTR nTmpQuoteLen;
  int nParentType=AEHAP_NONE;
  BOOL bDefaultTheme=FALSE;

  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen && ciChar->lpLine->nLineBreak != AELB_WRAP)
    return 0;
  if (dwSearchType & AEHF_FINDCHILD)
  {
    if (!qm->lpQuote || !qm->lpQuote->nRuleID)
      return 0;
    crParentQuoteStart.ciMin=qm->crQuoteStart.ciMin;
    crParentQuoteStart.ciMax=qm->crQuoteStart.ciMax;
    crParentQuoteEnd.ciMin=qm->crQuoteEnd.ciMin;
    crParentQuoteEnd.ciMax=qm->crQuoteEnd.ciMax;
    nParentQuoteLen=qm->nQuoteLen;
  }

  NextTheme:
  if (bDefaultTheme || !ae->popt->lpActiveTheme)
  {
    lpQuoteStack=&ae->ptxt->hQuoteStack;
    lpQuoteStartStack=&ae->ptxt->hQuoteStartStack;
    bDefaultTheme=TRUE;
  }
  else
  {
    lpQuoteStack=&ae->popt->lpActiveTheme->hQuoteStack;
    lpQuoteStartStack=&ae->popt->lpActiveTheme->hQuoteStartStack;
  }
  qm->lpQuote=NULL;
  lpQuoteStart=NULL;

  if (lpQuoteStartStack->first)
  {
    ft.dwFlags=0;
    ft.pText=NULL;
    ft.dwTextLen=(UINT_PTR)-1;
    ft.nNewLine=AELB_ASIS;

    if (lpQuoteStack->nElementsRegExp)
    {
      //Get end of line
      ciMaxLine=*ciChar;
      AEC_WrapLineEnd(&ciMaxLine);
    }

    //Find quote beginning (backward)
    ciCount=*ciChar;
    if (dwSearchType & AEHF_FINDFIRSTCHAR)
    {
      AEC_WrapLineBegin(&ciCount);
      if (AEC_IndexCompare(&qm->ciFindFirst, &ciCount) > 0)
        ciCount=qm->ciFindFirst;
      if (AEC_IndexCompare(&qm->crQuoteEnd.ciMax, &ciCount) > 0)
        ciCount=qm->crQuoteEnd.ciMax;
    }

    BeginQuoteParse:
    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
      {
        if (!lpQuoteStart)
        {
          if (AEC_IndexCompare(&ciCount, ciChar) > 0)
          {
            nQuoteLen=0;
            goto EndTheme;
          }
          if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
            break;

          for (lpQuoteStart=lpQuoteStartStack->first; lpQuoteStart; lpQuoteStart=lpQuoteStart->next)
          {
            //Quote start
            if ((dwSearchType & AEHF_FINDCHILD) && !lpQuoteStart->nParentID)
              continue;
            if (!(nParentType=AE_HighlightAllowed(lpParentQuote, (dwSearchType & AEHF_FINDCHILD ? NULL : fm), lpQuoteStart->nParentID, lpQuoteStart->nRuleID, &ciCount)))
              continue;

            if (lpQuoteStart->dwFlags & AEHLF_REGEXP)
            {
              lpQuoteItem=lpQuoteStart->hQuoteItemHandleStack.first->lpQuoteItem;
              lpREGroupStack=(STACKREGROUP *)lpQuoteItem->lpREGroupStack;
              if (dwSearchType & AEHF_FINDFIRSTCHAR)
                AE_PatReset(lpREGroupStack);

              if (AE_PatExec(lpREGroupStack, lpREGroupStack->first, &ciCount, &ciMaxLine) && lpREGroupStack->first->nStrLen)
              {
                qm->lpQuote=lpQuoteItem;
                qm->crQuoteStart.ciMin=lpREGroupStack->first->ciStrStart;
                qm->crQuoteStart.ciMax=lpREGroupStack->first->ciStrStart;
                qm->crQuoteEnd.ciMin=lpREGroupStack->first->ciStrEnd;
                qm->crQuoteEnd.ciMax=lpREGroupStack->first->ciStrEnd;
                qm->nQuoteLen=lpREGroupStack->first->nStrLen;
                nQuoteLen=qm->nQuoteLen;

                if (dwSearchType & AEHF_FINDFIRSTCHAR)
                {
                  if (AEC_IndexCompare(&qm->crQuoteEnd.ciMax, ciChar) <= 0)
                  {
                    ciCount=qm->crQuoteEnd.ciMax;
                    qm->lpQuote=NULL;
                    lpQuoteStart=NULL;
                    goto BeginQuoteParse;
                  }
                }
                goto End;
              }
              else AE_PatReset(lpREGroupStack);
            }
            else if (lpQuoteStart->dwFlags & AEHLF_QUOTESTART_ISDELIMITER)
            {
              if ((lpDelimItem=AE_HighlightIsDelimiter(ae, &ft, &ciCount, 0, lpParentQuote, fm)) || AEC_IsFirstCharInLine(&ciCount))
              {
                if (lpDelimItem)
                {
                  ciTmpCount=ft.crFound.ciMax;
                  nTmpQuoteLen=lpDelimItem->nDelimiterLen;
                  crTmpQuoteStart.ciMin=ft.crFound.ciMin;
                  crTmpQuoteStart.ciMax=ft.crFound.ciMax;
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
              if (!lpQuoteStart->pQuoteStart || !*lpQuoteStart->pQuoteStart)
              {
                if (!(lpQuoteStart->dwFlags & AEHLF_ATLINESTART) || AEC_IsFirstCharInLine(&ciCount))
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
                ft.pText=lpQuoteStart->pQuoteStart;
                ft.dwTextLen=lpQuoteStart->nQuoteStartLen;
                ft.dwFlags=(lpQuoteStart->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;

                if (AE_IsMatch(ae, &ft, &ciCount))
                {
                  if (!AE_IsEscaped(&ciCount, lpQuoteStart->chEscape))
                  {
                    if (!(lpQuoteStart->dwFlags & AEHLF_QUOTESTART_ISWORD) ||
                        ((AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMax, 0, lpParentQuote, fm) || AEC_IsLastCharInLine(&ft.crFound.ciMax)) &&
                         (AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMin, AEHID_BACK, lpParentQuote, fm) || AEC_IsFirstCharInLine(&ft.crFound.ciMin))))
                    {
                      ciTmpCount=ft.crFound.ciMax;
                      nTmpQuoteLen=lpQuoteStart->nQuoteStartLen;
                      crTmpQuoteStart.ciMin=ft.crFound.ciMin;
                      crTmpQuoteStart.ciMax=ft.crFound.ciMax;
                      goto CheckQuoteStart;
                    }
                  }
                }
              }
            }
            continue;

            CheckQuoteStart:
            if (lpQuoteStart->dwFlags & AEHLF_ATLINESTART)
            {
              if (!AE_IsSpacesFromLeft(&crTmpQuoteStart.ciMin))
                continue;
            }
            ciCount=ciTmpCount;
            nQuoteLen=nTmpQuoteLen;
            qm->crQuoteStart.ciMin=crTmpQuoteStart.ciMin;
            qm->crQuoteStart.ciMax=crTmpQuoteStart.ciMax;
            goto BeginQuoteParse;

            QuoteStartNext:
            ciCount=qm->crQuoteStart.ciMin;
            nQuoteLen=0;
            qm->crQuoteStart.ciMax=qm->crQuoteStart.ciMin;
            qm->crQuoteEnd.ciMin=qm->crQuoteStart.ciMin;
            qm->crQuoteEnd.ciMax=qm->crQuoteStart.ciMax;
            qm->lpQuote=NULL;
          }
          if (dwSearchType & AEHF_ISFIRSTCHAR)
          {
            nQuoteLen=0;
            goto EndTheme;
          }
        }
        else
        {
          //Quote end
          for (lpQuoteItemHandle=lpQuoteStart->hQuoteItemHandleStack.first; lpQuoteItemHandle; lpQuoteItemHandle=lpQuoteItemHandle->next)
          {
            qm->lpQuote=lpQuoteItemHandle->lpQuoteItem;

            if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_ISDELIMITER)
            {
              if ((lpDelimItem=AE_HighlightIsDelimiter(ae, &ft, &ciCount, 0, lpParentQuote, fm)) || AEC_IsLastCharInLine(&ciCount))
              {
                if (lpDelimItem)
                {
                  nQuoteLen+=lpDelimItem->nDelimiterLen;
                  qm->crQuoteEnd.ciMin=ft.crFound.ciMin;
                  qm->crQuoteEnd.ciMax=ft.crFound.ciMax;
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
                if (dwSearchType & AEHF_FINDCHILD)
                {
                  nQuoteLen+=AE_IndexSubtract(ae, &crParentQuoteEnd.ciMax, &ciCount, AELB_R, FALSE, FALSE);
                  ciCount=crParentQuoteEnd.ciMax;
                }
                else nQuoteLen+=AEC_WrapLineEnd(&ciCount);

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
                        ((AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMax, 0, lpParentQuote, fm) || AEC_IsLastCharInLine(&ft.crFound.ciMax)) &&
                         (AE_HighlightIsDelimiter(ae, NULL, &ft.crFound.ciMin, AEHID_BACK, lpParentQuote, fm) || AEC_IsFirstCharInLine(&ft.crFound.ciMin))))
                    {
                      nQuoteLen+=qm->lpQuote->nQuoteEndLen;
                      qm->crQuoteEnd.ciMin=ft.crFound.ciMin;
                      qm->crQuoteEnd.ciMax=ft.crFound.ciMax;
                      ciCount=qm->crQuoteEnd.ciMax;
                      goto SetQuote;
                    }
                  }
                }
              }
            }
            if (qm->lpQuote->dwFlags & AEHLF_QUOTEEMPTY)
              goto QuoteStartNext;
            if (qm->lpQuote->dwFlags & AEHLF_QUOTESTART_ISDELIMITER)
            {
              if (AE_HighlightIsDelimiter(ae, NULL, &ciCount, 0, lpParentQuote, fm))
                goto QuoteStartNext;
            }
            if (qm->lpQuote->dwFlags & AEHLF_QUOTEWITHOUTDELIMITERS)
            {
              if (AE_HighlightIsDelimiter(ae, NULL, &ciCount, 0, lpParentQuote, fm))
                goto QuoteStartNext;
            }
            if (qm->lpQuote->dwFlags & AEHLF_QUOTEINCLUDE)
            {
              if (qm->lpQuote->pQuoteInclude && *qm->lpQuote->pQuoteInclude)
              {
                if (!AE_wcschr(qm->lpQuote->pQuoteInclude, ciCount.lpLine->wpLine[ciCount.nCharInLine], (qm->lpQuote->dwFlags & AEHLF_MATCHCASE)))
                  goto QuoteStartNext;
              }
            }
            if (qm->lpQuote->dwFlags & AEHLF_QUOTEEXCLUDE)
            {
              if (qm->lpQuote->pQuoteExclude && *qm->lpQuote->pQuoteExclude)
              {
                if (AE_wcschr(qm->lpQuote->pQuoteExclude, ciCount.lpLine->wpLine[ciCount.nCharInLine], (qm->lpQuote->dwFlags & AEHLF_MATCHCASE)))
                  goto QuoteStartNext;
              }
            }
          }

          if (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
            nQuoteLen+=AEC_IndexLen(&ciCount);
        }
        AEC_IndexInc(&ciCount);
        if ((dwSearchType & AEHF_FINDCHILD) && AEC_IndexCompare(&ciCount, &crParentQuoteEnd.ciMax) >= 0)
          goto NoQuoteEndMatch;
      }

      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AEC_NextLine(&ciCount);
      else
        break;
    }
    NoQuoteEndMatch:
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
    goto EndQuoteParse;

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
        if (qm->lpQuote->dwFlags & AEHLF_QUOTESTART_CATCHONLY)
          ciCount=qm->crQuoteStart.ciMax;
        else if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_NOCATCH)
          ciCount=qm->crQuoteEnd.ciMin;

        if (AEC_IndexCompare(&ciCount, ciChar) <= 0)
        {
          if (AEC_IndexCompare(&ciCount, &qm->crQuoteStart.ciMin) > 0)
          {
            qm->lpQuote=NULL;
            lpQuoteStart=NULL;
            goto BeginQuoteParse;
          }
          else goto QuoteStartNext;
        }
      }
    }

    EndQuoteParse:
    if (qm->lpQuote)
    {
      if (qm->lpQuote->dwFlags & AEHLF_QUOTESTART_CATCHONLY)
      {
        qm->crQuoteEnd.ciMin=qm->crQuoteStart.ciMax;
        qm->crQuoteEnd.ciMax=qm->crQuoteStart.ciMax;
        nQuoteLen=qm->lpQuote->nQuoteStartLen;
      }
      else if (qm->lpQuote->dwFlags & AEHLF_QUOTEEND_NOCATCH)
      {
        if (AEC_IndexCompare(&qm->crQuoteEnd.ciMin, &qm->crQuoteEnd.ciMax) < 0)
        {
          qm->crQuoteEnd.ciMax=qm->crQuoteEnd.ciMin;
          nQuoteLen-=qm->lpQuote->nQuoteEndLen;
        }
      }
      if (qm->lpQuote->dwFlags & AEHLF_QUOTESTART_NOCATCH)
      {
        if (AEC_IndexCompare(&qm->crQuoteStart.ciMin, &qm->crQuoteStart.ciMax) < 0)
        {
          qm->crQuoteStart.ciMin=qm->crQuoteStart.ciMax;
          nQuoteLen-=qm->lpQuote->nQuoteStartLen;
        }
      }
      qm->nQuoteLen=nQuoteLen;
    }
  }

  EndTheme:
  if (!nQuoteLen && !bDefaultTheme)
  {
    bDefaultTheme=TRUE;
    goto NextTheme;
  }

  End:
  if (dwSearchType & AEHF_FINDCHILD)
  {
    if (nQuoteLen)
    {
      AEQUOTEMATCHITEM *lpQuoteMatchItem;

      if (!AE_HeapStackInsertBefore(ae, (stack **)&qm->hParentStack.first, (stack **)&qm->hParentStack.last, NULL, (stack **)&lpQuoteMatchItem, sizeof(AEQUOTEMATCHITEM)))
      {
        lpQuoteMatchItem->lpQuote=lpParentQuote;
        lpQuoteMatchItem->crQuoteStart.ciMin=crParentQuoteStart.ciMin;
        lpQuoteMatchItem->crQuoteStart.ciMax=crParentQuoteStart.ciMax;
        lpQuoteMatchItem->crQuoteEnd.ciMin=crParentQuoteEnd.ciMin;
        lpQuoteMatchItem->crQuoteEnd.ciMax=crParentQuoteEnd.ciMax;
        lpQuoteMatchItem->nQuoteLen=nParentQuoteLen;
        lpQuoteMatchItem->dwActiveText=hlp->qm.dwActiveText;
        lpQuoteMatchItem->dwActiveBk=hlp->qm.dwActiveBk;
        lpQuoteMatchItem->dwFontStyle=hlp->qm.dwFontStyle;
      }
    }
    else
    {
      qm->lpQuote=lpParentQuote;
      qm->crQuoteStart.ciMin=crParentQuoteStart.ciMin;
      qm->crQuoteStart.ciMax=crParentQuoteStart.ciMax;
      qm->crQuoteEnd.ciMin=crParentQuoteEnd.ciMin;
      qm->crQuoteEnd.ciMax=crParentQuoteEnd.ciMax;
      qm->nQuoteLen=nParentQuoteLen;
    }
  }
  else
  {
    if (nQuoteLen)
    {
      hlp->qm.dwActiveText=hlp->dwDefaultText;
      hlp->qm.dwActiveBk=hlp->dwDefaultBk;
      hlp->qm.dwFontStyle=AEHLS_NONE;
    }
  }
  if (qm->lpQuote)
    qm->nParentType=nParentType;
  return nQuoteLen;
}

int AE_HighlightFindWord(AKELEDIT *ae, const AECHARINDEX *ciChar, INT_PTR nCharOffset, DWORD dwSearchType, AEWORDMATCH *wm, AEQUOTEMATCH *qm, AEFOLDMATCH *fm)
{
  AEFINDTEXTW ft;
  AECHARINDEX ciCount;
  AEQUOTEITEMW *lpQuote=qm->lpQuote;
  AEDELIMITEMW *lpDelimItem;
  AEFOLDMATCH *fmTmp;
  INT_PTR nCountOffset;
  int nCharLen;
  int nWordLeft=0;
  int nWordRight=0;
  int nWordLen=0;

  wm->lpDelim1=NULL;
  wm->lpWord=NULL;
  wm->lpDelim2=NULL;
  if (ciChar->nCharInLine >= ciChar->lpLine->nLineLen)
    return 0;

  if ((ae->popt->lpActiveTheme &&
        (ae->popt->lpActiveTheme->hWordStack.first ||
         ae->popt->lpActiveTheme->hDelimiterStack.first)) ||
      (ae->ptxt->hWordStack.first ||
       ae->ptxt->hDelimiterStack.first))
  {
    ft.dwFlags=0;
    ft.pText=NULL;
    ft.dwTextLen=(UINT_PTR)-1;
    ft.nNewLine=AELB_ASIS;

    //Find word beginning (backward)
    ciCount=*ciChar;

    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine >= 0)
      {
        nCharLen=AEC_IndexLen(&ciCount);
        nWordLeft+=nCharLen;
        if (nWordLeft > AEMAX_WORDLENGTH)
          goto End;

        if (qm->lpQuote)
        {
          if (qm->nQuoteLen &&
              AEC_IndexCompare(&ciCount, &qm->crQuoteEnd.ciMax) < 0 &&
              AEC_IndexCompare(&ciCount, &qm->crQuoteStart.ciMin) >= 0)
          {
            lpQuote=qm->lpQuote;
          }
          else lpQuote=NULL;
        }

        //Is delimiter
        if (lpDelimItem=AE_HighlightIsDelimiter(ae, &ft, &ciCount, AEHID_LINEEDGE, lpQuote, fm))
        {
          wm->lpDelim1=lpDelimItem;
          wm->crDelim1.ciMin=ft.crFound.ciMin;
          wm->crDelim1.ciMax=ft.crFound.ciMax;
          nWordLeft=max(nWordLeft - wm->lpDelim1->nDelimiterLen, 0);
          goto FindWordEnding;
        }

        if (AEC_IndexCompare(&ciCount, &wm->crDelim2.ciMax) <= 0 ||
            AEC_IndexCompare(&ciCount, &wm->crDelim1.ciMax) == 0)
          goto SetEmptyFirstDelim;
        nCountOffset=nCharOffset - (nWordLeft - nCharLen);

        if ((qm->nQuoteLen && (!AEC_IndexCompare(&ciCount, &qm->crQuoteEnd.ciMax) ||
                               !AEC_IndexCompare(&ciCount, &qm->crQuoteStart.ciMin))) ||
            nCountOffset == fm->crFoldEnd.cpMax ||
            nCountOffset == fm->crFoldStart.cpMin)
        {
          if (!AEC_IndexCompare(&ciCount, &qm->crQuoteEnd.ciMax))
            lpQuote=NULL;
          if (nCountOffset == fm->crFoldEnd.cpMax)
            fmTmp=NULL;
          else
            fmTmp=fm;
          if (lpDelimItem=AE_HighlightIsDelimiter(ae, &ft, &ciCount, AEHID_BACK|AEHID_LINEEDGE, lpQuote, fmTmp))
            goto SetEmptyFirstDelim;
        }
        if (dwSearchType & AEHF_ISFIRSTCHAR)
          goto End;
        AEC_IndexDec(&ciCount);
      }

      if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
      {
        AEC_PrevLine(&ciCount);
        AEC_IndexDec(&ciCount);
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
    if (AEC_IndexCompare(&wm->crDelim1.ciMax, ciChar) > 0)
    {
      wm->crDelim2.ciMin=wm->crDelim1.ciMin;
      wm->crDelim2.ciMax=wm->crDelim1.ciMax;
      goto End;
    }
    ciCount=*ciChar;
    nCharLen=AEC_IndexInc(&ciCount);

    while (ciCount.lpLine)
    {
      while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
      {
        if (qm->lpQuote)
        {
          if (qm->nQuoteLen &&
              AEC_IndexCompare(&ciCount, &qm->crQuoteEnd.ciMax) < 0 &&
              AEC_IndexCompare(&ciCount, &qm->crQuoteStart.ciMin) >= 0)
          {
            if (AEC_IndexCompare(&ciCount, &qm->crQuoteStart.ciMin) == 0)
            {
              if (lpDelimItem=AE_HighlightIsDelimiter(ae, &ft, &ciCount, AEHID_LINEEDGE, NULL, fm))
              {
                //Empty delimiter
                wm->lpDelim2=lpDelimItem;
                wm->crDelim2.ciMin=ciCount;
                wm->crDelim2.ciMax=ciCount;
                goto SetWord;
              }
            }
            lpQuote=qm->lpQuote;
          }
          else lpQuote=NULL;
        }

        //Is delimiter
        if (lpDelimItem=AE_HighlightIsDelimiter(ae, &ft, &ciCount, AEHID_LINEEDGE, lpQuote, fm))
        {
          wm->lpDelim2=lpDelimItem;
          wm->crDelim2.ciMin=ft.crFound.ciMin;
          wm->crDelim2.ciMax=ft.crFound.ciMax;
          goto SetWord;
        }
        nWordRight+=AEC_IndexInc(&ciCount);
        if (nWordRight > AEMAX_WORDLENGTH)
          goto End;
      }

      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AEC_NextLine(&ciCount);
      else
        break;
    }

    //Empty delimiter
    wm->crDelim2.ciMin.nLine=ciCount.nLine;
    wm->crDelim2.ciMin.lpLine=ciCount.lpLine;
    wm->crDelim2.ciMin.nCharInLine=ciCount.lpLine->nLineLen;
    wm->crDelim2.ciMax=wm->crDelim2.ciMin;

    SetWord:
    nWordLen=nWordLeft + nWordRight;
    wm->crWord.ciMin=wm->crDelim1.ciMax;
    wm->crWord.ciMax=wm->crDelim2.ciMin;
    if (qm->lpQuote)
    {
      if (qm->nQuoteLen &&
          AEC_IndexCompare(&wm->crWord.ciMax, &qm->crQuoteEnd.ciMax) <= 0 &&
          AEC_IndexCompare(&wm->crWord.ciMin, &qm->crQuoteStart.ciMin) >= 0)
      {
        lpQuote=qm->lpQuote;
      }
      else lpQuote=NULL;
    }
    wm->lpWord=AE_HighlightIsWord(ae, NULL, &wm->crWord, nWordLen, lpQuote, fm);
  }

  End:
  return nWordLen;
}

AEDELIMITEMW* AE_HighlightIsDelimiter(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar, DWORD dwFlags, AEQUOTEITEMW *lpQuote, AEFOLDMATCH *fm)
{
  AEDELIMITEMW *lpDelimItem;
  AECHARINDEX ciDelimStart=*ciChar;
  AEFINDTEXTW ftMatch;
  BOOL bDefaultTheme=FALSE;

  if (!ft) ft=&ftMatch;

  NextTheme:
  if (bDefaultTheme || !ae->popt->lpActiveTheme)
  {
    lpDelimItem=ae->ptxt->hDelimiterStack.first;
    bDefaultTheme=TRUE;
  }
  else lpDelimItem=ae->popt->lpActiveTheme->hDelimiterStack.first;

  for (; lpDelimItem; lpDelimItem=lpDelimItem->next)
  {
    if (!AE_HighlightAllowed(lpQuote, fm, lpDelimItem->nParentID, 0, &ciDelimStart))
      continue;

    ft->pText=lpDelimItem->pDelimiter;
    ft->dwTextLen=lpDelimItem->nDelimiterLen;
    ft->dwFlags=(lpDelimItem->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;
    ft->nNewLine=AELB_ASIS;

    if (!(dwFlags & AEHID_BACK) || AE_IndexOffset(ae, ciChar, &ciDelimStart, -(INT_PTR)ft->dwTextLen, ft->nNewLine) == ft->dwTextLen)
    {
      if (AE_IsMatch(ae, ft, &ciDelimStart))
      {
        if (dwFlags & AEHID_LINEEDGE)
        {
          if (lpDelimItem->dwFlags & AEHLF_ATLINESTART)
          {
            if (!AE_IsSpacesFromLeft(&ft->crFound.ciMin))
              continue;
          }
          if (lpDelimItem->dwFlags & AEHLF_ATLINEEND)
          {
            if (!AE_IsSpacesFromRight(&ft->crFound.ciMax))
              continue;
          }
        }
        return lpDelimItem;
      }
    }
  }
  if (!bDefaultTheme)
  {
    bDefaultTheme=TRUE;
    goto NextTheme;
  }
  return NULL;
}

AEWORDITEMW* AE_HighlightIsWord(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARRANGE *crWord, int nWordLen, AEQUOTEITEMW *lpQuote, AEFOLDMATCH *fm)
{
  AESTACKWORD *lpWordStack;
  AEWORDITEMW *lpWordItem;
  AEFINDTEXTW ftMatch;
  AECHARINDEX ciCount;
  BOOL bDefaultTheme=FALSE;

  if (!nWordLen) return NULL;
  if (!ft) ft=&ftMatch;

  NextTheme:
  if (bDefaultTheme || !ae->popt->lpActiveTheme)
  {
    lpWordStack=&ae->ptxt->hWordStack;
    bDefaultTheme=TRUE;
  }
  else lpWordStack=&ae->popt->lpActiveTheme->hWordStack;

  if ((DWORD)nWordLen < sizeof(lpWordStack->lpWordLens) / sizeof(INT_PTR))
  {
    //Composition words
    for (lpWordItem=(AEWORDITEMW *)lpWordStack->lpWordLens[0]; lpWordItem; lpWordItem=lpWordItem->next)
    {
      if (lpWordItem->nWordLen == 0)
      {
        if (lpWordItem->dwFlags & AEHLF_WORDCOMPOSITION)
        {
          if (!AE_HighlightAllowed(lpQuote, fm, lpWordItem->nParentID, 0, &crWord->ciMin))
            continue;
          ciCount=crWord->ciMin;

          while (AEC_IndexCompare(&ciCount, &crWord->ciMax) < 0)
          {
            if (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
            {
              if (!AE_wcschr(lpWordItem->pWord, ciCount.lpLine->wpLine[ciCount.nCharInLine], (lpWordItem->dwFlags & AEHLF_MATCHCASE)))
                break;
              AEC_IndexInc(&ciCount);
            }
            else
            {
              if (ciCount.lpLine->nLineBreak == AELB_WRAP)
                AEC_NextLine(&ciCount);
              else
                break;
            }
          }
          if (!AEC_IndexCompare(&ciCount, &crWord->ciMax))
            goto CheckWord;
        }
      }
      else break;
    }

    //Standard words
    for (lpWordItem=(AEWORDITEMW *)lpWordStack->lpWordLens[nWordLen]; lpWordItem; lpWordItem=lpWordItem->next)
    {
      if (lpWordItem->nWordLen == nWordLen)
      {
        if (!AE_HighlightAllowed(lpQuote, fm, lpWordItem->nParentID, 0, &crWord->ciMin))
          continue;

        ft->pText=lpWordItem->pWord;
        ft->dwTextLen=lpWordItem->nWordLen;
        ft->dwFlags=(lpWordItem->dwFlags & AEHLF_MATCHCASE)?AEFR_MATCHCASE:0;
        ft->nNewLine=AELB_ASIS;

        if (AE_IsMatch(ae, ft, &crWord->ciMin))
        {
          goto CheckWord;
        }
      }
      else break;
    }
  }
  lpWordItem=NULL;
  goto EndTheme;

  CheckWord:
  if (lpWordItem->dwFlags & AEHLF_ATLINESTART)
  {
    if (!AE_IsSpacesFromLeft(&crWord->ciMin))
    {
      lpWordItem=NULL;
      goto EndTheme;
    }
  }
  if (lpWordItem->dwFlags & AEHLF_ATLINEEND)
  {
    if (!AE_IsSpacesFromRight(&crWord->ciMax))
    {
      lpWordItem=NULL;
      goto EndTheme;
    }
  }

  EndTheme:
  if (!lpWordItem && !bDefaultTheme)
  {
    bDefaultTheme=TRUE;
    goto NextTheme;
  }
  return lpWordItem;
}

AETHEMEITEMW* AE_HighlightCreateTheme(WPARAM wParam, wchar_t *wpThemeName)
{
  AETHEMEITEMW *lpElement=NULL;

  if (*wpThemeName)
  {
    if (!AE_HeapStackInsertIndex(NULL, (stack **)&hAkelEditThemesStack.first, (stack **)&hAkelEditThemesStack.last, (stack **)&lpElement, -1, sizeof(AETHEMEITEMW)))
    {
      xstrcpynW(lpElement->wszThemeName, wpThemeName, MAX_PATH);
      lpElement->wParam=wParam;
    }
  }
  return lpElement;
}

AETHEMEITEMW* AE_HighlightGetTheme(wchar_t *wpThemeName)
{
  AETHEMEITEMW *lpElement;

  for (lpElement=hAkelEditThemesStack.first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wszThemeName, wpThemeName))
      return lpElement;
  }
  return NULL;
}

BOOL AE_HighlightIsThemeExists(AETHEMEITEMW *aeti)
{
  AETHEMEITEMW *lpElement;

  for (lpElement=hAkelEditThemesStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement == aeti)
      return TRUE;
  }
  return FALSE;
}

void AE_HighlightDeleteTheme(AKELEDIT *ae, AETHEMEITEMW *aeti)
{
  AE_HighlightDeleteDelimiterAll(ae, aeti);
  AE_HighlightDeleteWordAll(ae, aeti);
  AE_HighlightDeleteQuoteAll(ae, aeti);
  AE_HighlightDeleteMarkTextAll(ae, aeti);
  AE_HighlightDeleteMarkRangeAll(ae, aeti);
  if (aeti)
    AE_HeapStackDelete(NULL, (stack **)&hAkelEditThemesStack.first, (stack **)&hAkelEditThemesStack.last, (stack *)aeti);
}

void AE_HighlightDeleteThemeAll()
{
  AETHEMEITEMW *lpElement;

  for (lpElement=hAkelEditThemesStack.first; lpElement; lpElement=lpElement->next)
  {
    AE_HighlightDeleteDelimiterAll(NULL, lpElement);
    AE_HighlightDeleteWordAll(NULL, lpElement);
    AE_HighlightDeleteQuoteAll(NULL, lpElement);
    AE_HighlightDeleteMarkTextAll(NULL, lpElement);
    AE_HighlightDeleteMarkRangeAll(NULL, lpElement);
  }
  AE_HeapStackClear(NULL, (stack **)&hAkelEditThemesStack.first, (stack **)&hAkelEditThemesStack.last);
}

void AE_HighlightUnsetTheme(AETHEMEITEMW *aeti)
{
  AKELEDIT *lpElement;

  for (lpElement=(AKELEDIT *)hAkelEditWindowsStack.first; lpElement; lpElement=lpElement->next)
  {
    if (!aeti || lpElement->popt->lpActiveTheme == aeti)
      lpElement->popt->lpActiveTheme=NULL;
  }
}

AEDELIMITEMW* AE_HighlightInsertDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, int nDelimiterLen, int nIndex)
{
  AESTACKDELIM *lpDelimiterStack=aeti?&aeti->hDelimiterStack:&ae->ptxt->hDelimiterStack;
  AEDELIMITEMW *lpDelimItem;

  AE_HeapStackInsertIndex(NULL, (stack **)&lpDelimiterStack->first, (stack **)&lpDelimiterStack->last, (stack **)&lpDelimItem, nIndex, sizeof(AEDELIMITEMW));

  return lpDelimItem;
}

void AE_HighlightDeleteDelimiter(AKELEDIT *ae, AETHEMEITEMW *aeti, AEDELIMITEMW *aedi)
{
  AESTACKDELIM *lpDelimiterStack=aeti?&aeti->hDelimiterStack:&ae->ptxt->hDelimiterStack;

  if (aedi->pDelimiter) AE_HeapFree(NULL, 0, (LPVOID)aedi->pDelimiter);
  AE_HeapStackDelete(NULL, (stack **)&lpDelimiterStack->first, (stack **)&lpDelimiterStack->last, (stack *)aedi);
}

void AE_HighlightDeleteDelimiterAll(AKELEDIT *ae, AETHEMEITEMW *aeti)
{
  AESTACKDELIM *lpDelimiterStack=aeti?&aeti->hDelimiterStack:&ae->ptxt->hDelimiterStack;
  AEDELIMITEMW *lpDelimItem;

  for (lpDelimItem=lpDelimiterStack->first; lpDelimItem; lpDelimItem=lpDelimItem->next)
  {
    if (lpDelimItem->pDelimiter) AE_HeapFree(NULL, 0, (LPVOID)lpDelimItem->pDelimiter);
  }
  AE_HeapStackClear(NULL, (stack **)&lpDelimiterStack->first, (stack **)&lpDelimiterStack->last);
}

AEWORDITEMW* AE_HighlightInsertWord(AKELEDIT *ae, AETHEMEITEMW *aeti, int nWordLen)
{
  AESTACKWORD *lpWordStack=aeti?&aeti->hWordStack:&ae->ptxt->hWordStack;
  AEWORDITEMW *lpWordItem=NULL;
  AEWORDITEMW *lpTmp;

  if ((DWORD)nWordLen < sizeof(lpWordStack->lpWordLens) / sizeof(INT_PTR))
  {
    if (lpWordStack->lpWordLens[nWordLen])
    {
      lpTmp=(AEWORDITEMW *)lpWordStack->lpWordLens[nWordLen];
    }
    else
    {
      for (lpTmp=lpWordStack->first; lpTmp; lpTmp=lpTmp->next)
      {
        if (lpTmp->nWordLen >= nWordLen)
          break;
      }
    }
    AE_HeapStackInsertBefore(NULL, (stack **)&lpWordStack->first, (stack **)&lpWordStack->last, (stack *)lpTmp, (stack **)&lpWordItem, sizeof(AEWORDITEMW));

    if (lpWordItem)
      lpWordStack->lpWordLens[nWordLen]=(INT_PTR)lpWordItem;
  }
  return lpWordItem;
}

void AE_HighlightDeleteWord(AKELEDIT *ae, AETHEMEITEMW *aeti, AEWORDITEMW *aewi)
{
  AESTACKWORD *lpWordStack=aeti?&aeti->hWordStack:&ae->ptxt->hWordStack;

  if (lpWordStack->lpWordLens[aewi->nWordLen] == (INT_PTR)aewi)
  {
    //Word item is first in length group
    if (aewi->next && aewi->next->nWordLen == aewi->nWordLen)
      lpWordStack->lpWordLens[aewi->nWordLen]=(INT_PTR)aewi->next;
    else
      lpWordStack->lpWordLens[aewi->nWordLen]=0;
  }
  if (aewi->pWord) AE_HeapFree(NULL, 0, (LPVOID)aewi->pWord);
  AE_HeapStackDelete(NULL, (stack **)&lpWordStack->first, (stack **)&lpWordStack->last, (stack *)aewi);
}

void AE_HighlightDeleteWordAll(AKELEDIT *ae, AETHEMEITEMW *aeti)
{
  AESTACKWORD *lpWordStack=aeti?&aeti->hWordStack:&ae->ptxt->hWordStack;
  AEWORDITEMW *lpWordItem;

  for (lpWordItem=lpWordStack->first; lpWordItem; lpWordItem=lpWordItem->next)
  {
    if (lpWordItem->pWord) AE_HeapFree(NULL, 0, (LPVOID)lpWordItem->pWord);
  }
  AE_HeapStackClear(NULL, (stack **)&lpWordStack->first, (stack **)&lpWordStack->last);
  xmemset(lpWordStack->lpWordLens, 0, MAX_PATH * sizeof(INT_PTR));
}

AEQUOTEITEMW* AE_HighlightAddQuote(AKELEDIT *ae, AETHEMEITEMW *lpTheme, AEQUOTEITEMW *lpQuoteSrc, AEQUOTEITEMW *lpQuoteDst)
{
  AESTACKQUOTE *lpQuoteStack=lpTheme?&lpTheme->hQuoteStack:&ae->ptxt->hQuoteStack;
  DWORD dwFontStyle=lpQuoteSrc->dwFontStyle;
  COLORREF crText=lpQuoteSrc->crText;
  COLORREF crBk=lpQuoteSrc->crBk;

  if (!lpQuoteDst)
    lpQuoteDst=AE_HighlightInsertQuote(ae, lpTheme, lpQuoteSrc->nIndex);
  if (lpQuoteDst)
  {
    if (lpQuoteSrc != lpQuoteDst)
    {
      if (lpQuoteSrc->pQuoteStart && !(lpQuoteSrc->dwFlags & AEHLF_QUOTESTART_ISDELIMITER))
      {
        if (lpQuoteDst->pQuoteStart=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteStartLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpQuoteDst->pQuoteStart, lpQuoteSrc->pQuoteStart, (lpQuoteSrc->nQuoteStartLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteStartLen=lpQuoteSrc->nQuoteStartLen;
        }
      }
      if (lpQuoteSrc->pQuoteEnd && !(lpQuoteSrc->dwFlags & AEHLF_QUOTEEND_ISDELIMITER))
      {
        if (lpQuoteDst->pQuoteEnd=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteEndLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpQuoteDst->pQuoteEnd, lpQuoteSrc->pQuoteEnd, (lpQuoteSrc->nQuoteEndLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteEndLen=lpQuoteSrc->nQuoteEndLen;
        }
      }

      lpQuoteDst->chEscape=lpQuoteSrc->chEscape;

      if (lpQuoteSrc->pQuoteInclude)
      {
        if (lpQuoteDst->pQuoteInclude=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteIncludeLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpQuoteDst->pQuoteInclude, lpQuoteSrc->pQuoteInclude, (lpQuoteSrc->nQuoteIncludeLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteIncludeLen=lpQuoteSrc->nQuoteIncludeLen;
        }
      }
      if (lpQuoteSrc->pQuoteExclude)
      {
        if (lpQuoteDst->pQuoteExclude=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpQuoteSrc->nQuoteExcludeLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpQuoteDst->pQuoteExclude, lpQuoteSrc->pQuoteExclude, (lpQuoteSrc->nQuoteExcludeLen + 1) * sizeof(wchar_t));
          lpQuoteDst->nQuoteExcludeLen=lpQuoteSrc->nQuoteExcludeLen;
        }
      }
    }
    lpQuoteDst->dwFlags=lpQuoteSrc->dwFlags;
    lpQuoteDst->dwFontStyle=dwFontStyle;
    lpQuoteDst->crText=crText;
    lpQuoteDst->crBk=crBk;
    lpQuoteDst->nParentID=lpQuoteSrc->nParentID;
    lpQuoteDst->nRuleID=lpQuoteSrc->nRuleID;
    ++lpQuoteStack->nElementsAll;

    if (lpQuoteDst->dwFlags & AEHLF_REGEXP)
    {
      AEREGROUPCOLOR *lpREGroupColor=NULL;
      STACKREGROUP *lpREGroupStack=NULL;
      REGROUP *lpREGroupRef;
      INT_PTR nCompileErrorOffset;
      int nIndex;
      const wchar_t *wpCount;
      const wchar_t *wpCountMax;
      DWORD dwFlags;

      if (!lpQuoteDst->pQuoteStart || !*lpQuoteDst->pQuoteStart)
        goto FreeQuote;

      if (lpREGroupStack=(STACKREGROUP *)AE_HeapAlloc(NULL, 0, sizeof(STACKREGROUP)))
      {
        lpREGroupStack->first=0;
        lpREGroupStack->last=0;
        lpREGroupStack->dwOptions=REO_MULTILINE;
        if (lpQuoteDst->dwFlags & AEHLF_MATCHCASE)
          lpREGroupStack->dwOptions|=REO_MATCHCASE;
        lpREGroupStack->wpDelim=NULL;
        lpREGroupStack->wpMaxDelim=NULL;
        if (nCompileErrorOffset=PatCompile(lpREGroupStack, lpQuoteDst->pQuoteStart, lpQuoteDst->pQuoteStart + lpQuoteDst->nQuoteStartLen))
        {
          AE_HeapFree(NULL, 0, (LPVOID)lpREGroupStack);
          lpQuoteSrc->nCompileErrorOffset=nCompileErrorOffset;
          goto FreeQuote;
        }

        //Copy word delimiters to own buffer, because it could be changed.
        if (lpREGroupStack->wpDelim=(wchar_t *)AE_HeapAlloc(NULL, 0, (ae->popt->nWordDelimitersLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy((wchar_t *)lpREGroupStack->wpDelim, ae->popt->wszWordDelimiters, (ae->popt->nWordDelimitersLen + 1) * sizeof(wchar_t));
          lpREGroupStack->wpMaxDelim=lpREGroupStack->wpDelim + ae->popt->nWordDelimitersLen;
        }

        //RESE_ISMATCH
        lpREGroupStack->first->dwFlags&=~REGF_ROOTANY;

        //Parse pQuoteEnd: \BackRef1=(FontStyle,ColorText,ColorBk) \BackRef2=(FontStyle,ColorText,ColorBk)
        if (lpQuoteDst->pQuoteEnd)
        {
          wpCountMax=lpQuoteDst->pQuoteEnd + lpQuoteDst->nQuoteEndLen;

          for (wpCount=lpQuoteDst->pQuoteEnd; wpCount < wpCountMax; ++wpCount)
          {
            if (*wpCount == L'\\')
            {
              ++wpCount;

              if ((nIndex=PatRefIndex(&wpCount)) != -1)
              {
                if (lpREGroupRef=PatGetGroup(lpREGroupStack, nIndex))
                {
                  if (*wpCount == L'=' && *++wpCount == L'(')
                  {
                    dwFlags=0;
                    dwFontStyle=(DWORD)xatoiW(++wpCount, &wpCount);

                    if (*wpCount == L',')
                    {
                      if (*++wpCount == L'0')
                      {
                        ++wpCount;
                        crText=(DWORD)-1;
                      }
                      else if (*wpCount == L'\\')
                      {
                        ++wpCount;
                        if ((crText=PatRefIndex(&wpCount)) != (DWORD)-1)
                          dwFlags|=AEREGCF_BACKREFCOLORTEXT;
                      }
                      else if (*wpCount == L'#')
                      {
                        ++wpCount;
                        if (wpCountMax - wpCount < 3) break;
                        crText=AE_GetColorFromStr(wpCount, &wpCount);
                      }
                      else break;

                      if (*wpCount == L',')
                      {
                        if (*++wpCount == L'0')
                        {
                          ++wpCount;
                          crBk=(DWORD)-1;
                        }
                        else if (*wpCount == L'\\')
                        {
                          ++wpCount;
                          if ((crBk=PatRefIndex(&wpCount)) != (DWORD)-1)
                            dwFlags|=AEREGCF_BACKREFCOLORBK;
                        }
                        else if (*wpCount == L'#')
                        {
                          ++wpCount;
                          if (wpCountMax - wpCount < 3) break;
                          crBk=AE_GetColorFromStr(wpCount, &wpCount);
                        }
                        else break;

                        if (lpREGroupColor=(AEREGROUPCOLOR *)AE_HeapAlloc(NULL, 0, sizeof(AEREGROUPCOLOR)))
                        {
                          lpREGroupColor->dwFlags=dwFlags;
                          lpREGroupColor->dwFontStyle=dwFontStyle;
                          lpREGroupColor->crText=crText;
                          lpREGroupColor->crBk=crBk;
                          lpREGroupRef->dwUserData=(UINT_PTR)lpREGroupColor;

                          if (dwFontStyle != AEHLS_NONE ||
                              crText != (DWORD)-1 ||
                              crBk != (DWORD)-1)
                          {
                            lpQuoteDst->dwFlags|=AEHLF_STYLED;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        lpQuoteDst->lpREGroupStack=(void *)lpREGroupStack;
        ++lpQuoteStack->nElementsRegExp;
      }
      else goto FreeQuote;
    }
    else
    {
      if (dwFontStyle != AEHLS_NONE ||
          crText != (DWORD)-1 ||
          crBk != (DWORD)-1)
      {
        lpQuoteDst->dwFlags|=AEHLF_STYLED;
      }
    }
    lpQuoteDst->lpQuoteStart=(void *)AE_HighlightInsertQuoteStart(ae, lpTheme, lpQuoteDst);
  }
  return lpQuoteDst;

  FreeQuote:
  AE_HighlightDeleteQuote(ae, lpTheme, lpQuoteDst);
  return NULL;
}

AEQUOTEITEMW* AE_HighlightInsertQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, int nIndex)
{
  AESTACKQUOTE *lpQuoteStack=aeti?&aeti->hQuoteStack:&ae->ptxt->hQuoteStack;
  AEQUOTEITEMW *lpQuoteItem;

  AE_HeapStackInsertIndex(NULL, (stack **)&lpQuoteStack->first, (stack **)&lpQuoteStack->last, (stack **)&lpQuoteItem, nIndex, sizeof(AEQUOTEITEMW));

  return lpQuoteItem;
}

AEQUOTESTART* AE_HighlightInsertQuoteStart(AKELEDIT *ae, AETHEMEITEMW *aeti, AEQUOTEITEMW *lpQuoteItem)
{
  AESTACKQUOTESTART *lpQuoteStartStack=aeti?&aeti->hQuoteStartStack:&ae->ptxt->hQuoteStartStack;
  AEQUOTESTART *lpQuoteStart=NULL;
  AEQUOTEITEMHANDLE *lpQuoteItemHandle;

  if (!(lpQuoteItem->dwFlags & AEHLF_REGEXP))
  {
    for (lpQuoteStart=lpQuoteStartStack->first; lpQuoteStart; lpQuoteStart=lpQuoteStart->next)
    {
      if (lpQuoteStart->nQuoteStartLen == lpQuoteItem->nQuoteStartLen &&
          lpQuoteStart->dwFlags == lpQuoteItem->dwFlags &&
          lpQuoteStart->chEscape == lpQuoteItem->chEscape &&
          lpQuoteStart->nParentID == lpQuoteItem->nParentID &&
          lpQuoteStart->nRuleID == lpQuoteItem->nRuleID &&
          //AEQUOTEITEMs with filter flags should not be merged
          !(lpQuoteStart->dwFlags & (AEHLF_QUOTEINCLUDE|AEHLF_QUOTEEXCLUDE|AEHLF_QUOTEEMPTY|AEHLF_QUOTEWITHOUTDELIMITERS)))
      {
        if (!lpQuoteItem->nQuoteStartLen)
          break;

        if ((lpQuoteItem->dwFlags & AEHLF_MATCHCASE) ?
              !xstrcmpW(lpQuoteStart->pQuoteStart, lpQuoteItem->pQuoteStart) :
              !xstrcmpiW(lpQuoteStart->pQuoteStart, lpQuoteItem->pQuoteStart))
        {
          break;
        }
      }
    }
  }

  if (!lpQuoteStart)
  {
    if (!AE_HeapStackInsertIndex(NULL, (stack **)&lpQuoteStartStack->first, (stack **)&lpQuoteStartStack->last, (stack **)&lpQuoteStart, -1, sizeof(AEQUOTESTART)))
    {
      lpQuoteStart->pQuoteStart=lpQuoteItem->pQuoteStart;
      lpQuoteStart->nQuoteStartLen=lpQuoteItem->nQuoteStartLen;
      lpQuoteStart->chEscape=lpQuoteItem->chEscape;
      lpQuoteStart->dwFlags=lpQuoteItem->dwFlags;
      lpQuoteStart->nParentID=lpQuoteItem->nParentID;
      lpQuoteStart->nRuleID=lpQuoteItem->nRuleID;
    }
  }

  if (!AE_HeapStackInsertIndex(NULL, (stack **)&lpQuoteStart->hQuoteItemHandleStack.first, (stack **)&lpQuoteStart->hQuoteItemHandleStack.last, (stack **)&lpQuoteItemHandle, -1, sizeof(AEQUOTEITEMHANDLE)))
  {
    lpQuoteItemHandle->lpQuoteItem=lpQuoteItem;
  }
  return lpQuoteStart;
}

void AE_HighlightDeleteQuoteData(AEQUOTEITEMW *aeqi)
{
  if (aeqi->pQuoteStart) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteStart);
  if (aeqi->pQuoteEnd) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteEnd);
  if (aeqi->pQuoteInclude) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteInclude);
  if (aeqi->pQuoteExclude) AE_HeapFree(NULL, 0, (LPVOID)aeqi->pQuoteExclude);
  if (aeqi->lpREGroupStack)
  {
    STACKREGROUP *lpREGroupStack=(STACKREGROUP *)aeqi->lpREGroupStack;
    REGROUP *lpREGroup;

    //Free AEREGROUPCOLOR
    for (lpREGroup=lpREGroupStack->first; lpREGroup; lpREGroup=PatNextGroup(lpREGroup))
    {
      if (lpREGroup->dwUserData)
        AE_HeapFree(NULL, 0, (LPVOID)lpREGroup->dwUserData);
    }

    //Free delimiters
    if (lpREGroupStack->wpDelim)
      AE_HeapFree(NULL, 0, (LPVOID)lpREGroupStack->wpDelim);

    //Free group stack
    PatFree(lpREGroupStack);
    AE_HeapFree(NULL, 0, (LPVOID)lpREGroupStack);
  }
}

void AE_HighlightDeleteQuote(AKELEDIT *ae, AETHEMEITEMW *aeti, AEQUOTEITEMW *aeqi)
{
  AESTACKQUOTE *lpQuoteStack=aeti?&aeti->hQuoteStack:&ae->ptxt->hQuoteStack;
  AEQUOTESTART *lpQuoteStart;
  AEQUOTEITEMHANDLE *lpQuoteItemHandle;

  //Find and clear quote start handle
  if (aeqi->lpQuoteStart)
  {
    lpQuoteStart=(AEQUOTESTART *)aeqi->lpQuoteStart;

    for (lpQuoteItemHandle=lpQuoteStart->hQuoteItemHandleStack.first; lpQuoteItemHandle; lpQuoteItemHandle=lpQuoteItemHandle->next)
    {
      if (lpQuoteItemHandle->lpQuoteItem == aeqi)
      {
        AE_HeapStackDelete(NULL, (stack **)&lpQuoteStart->hQuoteItemHandleStack.first, (stack **)&lpQuoteStart->hQuoteItemHandleStack.last, (stack *)lpQuoteItemHandle);
        break;
      }
    }
  }
  AE_HighlightDeleteQuoteData(aeqi);
  AE_HeapStackDelete(NULL, (stack **)&lpQuoteStack->first, (stack **)&lpQuoteStack->last, (stack *)aeqi);
}

void AE_HighlightDeleteQuoteAll(AKELEDIT *ae, AETHEMEITEMW *aeti)
{
  AESTACKQUOTE *lpQuoteStack;
  AESTACKQUOTESTART *lpQuoteStartStack;
  AEQUOTEITEMW *lpQuoteItem;
  AEQUOTESTART *lpQuoteStart;

  if (aeti)
  {
    lpQuoteStack=&aeti->hQuoteStack;
    lpQuoteStartStack=&aeti->hQuoteStartStack;
  }
  else
  {
    lpQuoteStack=&ae->ptxt->hQuoteStack;
    lpQuoteStartStack=&ae->ptxt->hQuoteStartStack;
  }

  //Clear quotes start
  for (lpQuoteStart=lpQuoteStartStack->first; lpQuoteStart; lpQuoteStart=lpQuoteStart->next)
  {
    AE_HeapStackClear(NULL, (stack **)&lpQuoteStart->hQuoteItemHandleStack.first, (stack **)&lpQuoteStart->hQuoteItemHandleStack.last);
  }
  AE_HeapStackClear(NULL, (stack **)&lpQuoteStartStack->first, (stack **)&lpQuoteStartStack->last);

  //Clear quotes
  for (lpQuoteItem=lpQuoteStack->first; lpQuoteItem; lpQuoteItem=lpQuoteItem->next)
  {
    AE_HighlightDeleteQuoteData(lpQuoteItem);
  }
  AE_HeapStackClear(NULL, (stack **)&lpQuoteStack->first, (stack **)&lpQuoteStack->last);
}

AEMARKTEXTITEMW* AE_HighlightAddMarkText(AKELEDIT *ae, AETHEMEITEMW *lpTheme, AEMARKTEXTITEMW *lpMarkTextSrc, AEMARKTEXTITEMW *lpMarkTextDst)
{
  STACKREGROUP *lpREGroupStack=NULL;
  INT_PTR nCompileErrorOffset;

  if (!lpMarkTextDst)
    lpMarkTextDst=AE_HighlightInsertMarkText(ae, lpTheme, lpMarkTextSrc->nIndex);
  if (lpMarkTextDst)
  {
    if (lpMarkTextSrc != lpMarkTextDst)
    {
      if (lpMarkTextDst->pMarkText=(const wchar_t *)AE_HeapAlloc(NULL, 0, (lpMarkTextSrc->nMarkTextLen + 1) * sizeof(wchar_t)))
      {
        xmemcpy((wchar_t *)lpMarkTextDst->pMarkText, lpMarkTextSrc->pMarkText, (lpMarkTextSrc->nMarkTextLen + 1) * sizeof(wchar_t));
        lpMarkTextDst->nMarkTextLen=lpMarkTextSrc->nMarkTextLen;
      }
    }
    lpMarkTextDst->dwFlags=lpMarkTextSrc->dwFlags;
    lpMarkTextDst->dwFontStyle=lpMarkTextSrc->dwFontStyle;
    lpMarkTextDst->crText=lpMarkTextSrc->crText;
    lpMarkTextDst->crBk=lpMarkTextSrc->crBk;
  }

  if (lpMarkTextDst->dwFlags & AEHLF_REGEXP)
  {
    if (!lpMarkTextDst->pMarkText || !*lpMarkTextDst->pMarkText)
      goto FreeMarkText;

    if (lpREGroupStack=(STACKREGROUP *)AE_HeapAlloc(NULL, 0, sizeof(STACKREGROUP)))
    {
      lpREGroupStack->first=0;
      lpREGroupStack->last=0;
      lpREGroupStack->dwOptions=REO_MULTILINE;
      if (lpMarkTextDst->dwFlags & AEHLF_MATCHCASE)
        lpREGroupStack->dwOptions|=REO_MATCHCASE;
      if (lpMarkTextDst->dwFlags & AEHLF_WHOLEWORD)
        lpREGroupStack->dwOptions|=REO_WHOLEWORD;
      lpREGroupStack->wpDelim=NULL;
      lpREGroupStack->wpMaxDelim=NULL;
      if (nCompileErrorOffset=PatCompile(lpREGroupStack, lpMarkTextDst->pMarkText, lpMarkTextDst->pMarkText + lpMarkTextDst->nMarkTextLen))
      {
        AE_HeapFree(NULL, 0, (LPVOID)lpREGroupStack);
        lpMarkTextSrc->nCompileErrorOffset=nCompileErrorOffset;
        goto FreeMarkText;
      }

      //Copy word delimiters to own buffer, because it could be changed.
      if (lpREGroupStack->wpDelim=(wchar_t *)AE_HeapAlloc(NULL, 0, (ae->popt->nWordDelimitersLen + 1) * sizeof(wchar_t)))
      {
        xmemcpy((wchar_t *)lpREGroupStack->wpDelim, ae->popt->wszWordDelimiters, (ae->popt->nWordDelimitersLen + 1) * sizeof(wchar_t));
        lpREGroupStack->wpMaxDelim=lpREGroupStack->wpDelim + ae->popt->nWordDelimitersLen;
      }

      //RESE_ISMATCH
      lpREGroupStack->first->dwFlags&=~REGF_ROOTANY;

      lpMarkTextDst->lpREGroupStack=(void *)lpREGroupStack;
    }
    else goto FreeMarkText;
  }
  return lpMarkTextDst;

  FreeMarkText:
  AE_HighlightDeleteMarkText(ae, lpTheme, lpMarkTextDst);
  return NULL;
}

AEMARKTEXTITEMW* AE_HighlightInsertMarkText(AKELEDIT *ae, AETHEMEITEMW *aeti, int nIndex)
{
  AESTACKMARKTEXT *lpMarkTextStack=aeti?&aeti->hMarkTextStack:&ae->ptxt->hMarkTextStack;
  AEMARKTEXTITEMW *lpMarkTextItem;

  AE_HeapStackInsertIndex(NULL, (stack **)&lpMarkTextStack->first, (stack **)&lpMarkTextStack->last, (stack **)&lpMarkTextItem, nIndex, sizeof(AEMARKTEXTITEMW));

  return lpMarkTextItem;
}

void AE_HighlightDeleteMarkTextData(AEMARKTEXTITEMW *aemti)
{
  if (aemti->pMarkText) AE_HeapFree(NULL, 0, (LPVOID)aemti->pMarkText);
  if (aemti->lpREGroupStack)
  {
    STACKREGROUP *lpREGroupStack=(STACKREGROUP *)aemti->lpREGroupStack;

    //Free delimiters
    if (lpREGroupStack->wpDelim)
      AE_HeapFree(NULL, 0, (LPVOID)lpREGroupStack->wpDelim);

    //Free group stack
    PatFree(lpREGroupStack);
    AE_HeapFree(NULL, 0, (LPVOID)lpREGroupStack);
  }
}

void AE_HighlightDeleteMarkText(AKELEDIT *ae, AETHEMEITEMW *aeti, AEMARKTEXTITEMW *aemti)
{
  AESTACKMARKTEXT *lpMarkTextStack=aeti?&aeti->hMarkTextStack:&ae->ptxt->hMarkTextStack;

  AE_HighlightDeleteMarkTextData(aemti);
  AE_HeapStackDelete(NULL, (stack **)&lpMarkTextStack->first, (stack **)&lpMarkTextStack->last, (stack *)aemti);
}

void AE_HighlightDeleteMarkTextAll(AKELEDIT *ae, AETHEMEITEMW *aeti)
{
  AESTACKMARKTEXT *lpMarkTextStack=aeti?&aeti->hMarkTextStack:&ae->ptxt->hMarkTextStack;
  AEMARKTEXTITEMW *lpMarkTextItem;

  for (lpMarkTextItem=lpMarkTextStack->first; lpMarkTextItem; lpMarkTextItem=lpMarkTextItem->next)
  {
    AE_HighlightDeleteMarkTextData(lpMarkTextItem);
  }
  AE_HeapStackClear(NULL, (stack **)&lpMarkTextStack->first, (stack **)&lpMarkTextStack->last);
}

AEMARKRANGEITEM* AE_HighlightInsertMarkRange(AKELEDIT *ae, AETHEMEITEMW *aeti, int nIndex)
{
  AESTACKMARKRANGE *lpMarkRangeStack=aeti?&aeti->hMarkRangeStack:&ae->ptxt->hMarkRangeStack;
  AEMARKRANGEITEM *lpMarkRangeItem;

  AE_HeapStackInsertIndex(NULL, (stack **)&lpMarkRangeStack->first, (stack **)&lpMarkRangeStack->last, (stack **)&lpMarkRangeItem, nIndex, sizeof(AEMARKRANGEITEM));

  return lpMarkRangeItem;
}

void AE_HighlightDeleteMarkRange(AKELEDIT *ae, AETHEMEITEMW *aeti, AEMARKRANGEITEM *aemri)
{
  AESTACKMARKRANGE *lpMarkRangeStack=aeti?&aeti->hMarkRangeStack:&ae->ptxt->hMarkRangeStack;

  AE_HeapStackDelete(NULL, (stack **)&lpMarkRangeStack->first, (stack **)&lpMarkRangeStack->last, (stack *)aemri);
}

void AE_HighlightDeleteMarkRangeAll(AKELEDIT *ae, AETHEMEITEMW *aeti)
{
  AESTACKMARKRANGE *lpMarkRangeStack=aeti?&aeti->hMarkRangeStack:&ae->ptxt->hMarkRangeStack;

  AE_HeapStackClear(NULL, (stack **)&lpMarkRangeStack->first, (stack **)&lpMarkRangeStack->last);
}

HBITMAP AE_CreateBitmap(int nWidth, int nHeight, COLORREF crBasic, COLORREF crInvert)
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
        lpBitmapBits[a + 0]=GetBValue(crInvert) ^ GetBValue(crBasic);
        lpBitmapBits[a + 1]=GetGValue(crInvert) ^ GetGValue(crBasic);
        lpBitmapBits[a + 2]=GetRValue(crInvert) ^ GetRValue(crBasic);
        a+=3;
      }
      while (a % 4) lpBitmapBits[a++]=0x00;
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
  BITMAPINFO bi;
  HBITMAP hBitmap=NULL;
  DWORD a;
  int b;

  xmemset(&bi, 0, sizeof(BITMAPINFO));
  xmemcpy(&bi.bmiHeader, lpBmpInfoHeader, sizeof(BITMAPINFOHEADER));

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

    if (!(ae->popt->dwOptionsEx & AECOE_OVERTYPE))
    {
      nCaretWidth=ae->popt->nCaretInsertWidth;
      nCaretHeight=ae->ptxt->nCharHeight;
    }
    else
    {
      nCaretWidth=ae->ptxt->nAveCharWidth;
      nCaretHeight=ae->popt->nCaretOvertypeHeight;
    }

    if (ae->popt->aec.crCaret != RGB(0x00, 0x00, 0x00))
    {
      bd.nWidth=nCaretWidth;
      bd.nHeight=nCaretHeight;
      bd.crBasic=ae->popt->aec.crCaret;
      bd.crInvert=(ae->popt->dwOptions & AECO_ACTIVELINE)?ae->popt->aec.crActiveLineBk:ae->popt->aec.crBasicBk;

      if (!(bi=AE_StackBitmapItemGet(&hAkelEditBitmapDataStack, &bd)))
        bi=AE_StackBitmapItemInsert(&hAkelEditBitmapDataStack, &bd);
      hCaretBitmap=bi->hBitmap;

      if (!(ae->popt->dwOptionsEx & AECOE_OVERTYPE))
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

BOOL AE_SetCaretPos(AKELEDIT *ae, const POINT64 *ptCaret)
{
  POINT ptClient;
  BOOL bResult=FALSE;

  AE_GlobalToClient(ae, ptCaret, NULL, &ptClient);

  if (!(ae->popt->dwLockUpdate & AELU_CARET))
  {
    if (ae->popt->dwOptionsEx & AECOE_OVERTYPE)
      bResult=SetCaretPos(ptClient.x, ptClient.y + max(ae->ptxt->nCharHeight - ae->popt->nCaretOvertypeHeight, 0));
    else
      bResult=SetCaretPos(ptClient.x, ptClient.y);
  }

  AE_SetCaretVis(ae, ptCaret);
  return bResult;
}

void AE_SetCaretVis(AKELEDIT *ae, const POINT64 *ptCaret)
{
  POINT ptClient;

  AE_GlobalToClient(ae, ptCaret, NULL, &ptClient);

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

void AE_ScrollToCaret(AKELEDIT *ae, const POINT64 *ptCaret, DWORD dwSelFlags, DWORD dwSelType)
{
  int nDrawWidth=ae->rcDraw.right - ae->rcDraw.left;
  int nDrawHeight=ae->rcDraw.bottom - ae->rcDraw.top;
  int nOffsetPixelX=0;
  int nOffsetPixelY=0;

  if (ae->popt->dwCaretScrollFlags &&
      (dwSelType & ae->popt->dwCaretScrollSelType) &&
      (!ae->popt->dwCaretScrollSelFlags || (dwSelFlags & ae->popt->dwCaretScrollSelFlags)))
  {
    if (ae->popt->dwCaretScrollFlags & AESC_OFFSETPIXELX)
      nOffsetPixelX=ae->popt->nCaretScrollOffsetX;
    else if (ae->popt->dwCaretScrollFlags & AESC_OFFSETCHARX)
      nOffsetPixelX=ae->ptxt->nAveCharWidth * ae->popt->nCaretScrollOffsetX;
    else if (ae->popt->dwCaretScrollFlags & AESC_OFFSETRECTDIVX)
      nOffsetPixelX=nDrawWidth / ae->popt->nCaretScrollOffsetX;

    if (ae->popt->dwCaretScrollFlags & AESC_OFFSETPIXELY)
      nOffsetPixelY=ae->popt->nCaretScrollOffsetY;
    else if (ae->popt->dwCaretScrollFlags & AESC_OFFSETCHARY)
      nOffsetPixelY=ae->ptxt->nCharHeight * ae->popt->nCaretScrollOffsetY;
    else if (ae->popt->dwCaretScrollFlags & AESC_OFFSETRECTDIVY)
      nOffsetPixelY=nDrawHeight / ae->popt->nCaretScrollOffsetY;
  }

  //X
  if (ae->dwMouseMoveTimer)
  {
    if (ptCaret->x >= ae->nHScrollPos + nDrawWidth - ae->ptxt->nAveCharWidth - (nOffsetPixelX?nOffsetPixelX:0))
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - nDrawWidth + (nOffsetPixelX?nOffsetPixelX:0) + 1, 0));
    }
    else if (ptCaret->x < ae->nHScrollPos + (nOffsetPixelX?nOffsetPixelX:0))
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (nOffsetPixelX?nOffsetPixelX:0), 0));
    }
  }
  else
  {
    if (ptCaret->x >= ae->nHScrollPos + nDrawWidth - ae->ptxt->nAveCharWidth - (nOffsetPixelX?nOffsetPixelX:0))
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - nDrawWidth + (nOffsetPixelX?nOffsetPixelX:nDrawWidth / 3) + 1, 0));
    }
    else if (ptCaret->x < ae->nHScrollPos + (nOffsetPixelX?nOffsetPixelX:0))
    {
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (nOffsetPixelX?nOffsetPixelX:nDrawWidth / 3), 0));
    }
  }

  //Y
  if (ptCaret->y >= ae->nVScrollPos + nDrawHeight - ae->ptxt->nCharHeight - (nOffsetPixelY?nOffsetPixelY:0))
  {
    if ((ae->popt->dwOptions & AECO_VSCROLLBYLINE) || (!(dwSelFlags & AESELT_NOVERTSCROLLCORRECT) && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex) && !AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelStartIndex)))
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - nDrawHeight + ae->ptxt->nCharHeight + (nOffsetPixelY?nOffsetPixelY:ae->ptxt->nCharHeight) + 1, 0));
    else
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - nDrawHeight + (nOffsetPixelY?nOffsetPixelY:ae->ptxt->nCharHeight) + 1, 0));
  }
  else if (ptCaret->y < ae->nVScrollPos + (nOffsetPixelY?nOffsetPixelY:0))
  {
    if (!(dwSelFlags & AESELT_NOVERTSCROLLCORRECT) && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex) && !AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex))
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - ae->ptxt->nCharHeight - (nOffsetPixelY?nOffsetPixelY:0), 0));
    else
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (nOffsetPixelY?nOffsetPixelY:0), 0));
  }
}

DWORD AE_ScrollToPoint(AKELEDIT *ae, POINT64 *ptPosition)
{
  DWORD dwTest;

  //Test scroll
  dwTest=AE_ScrollToPointEx(ae, AESC_TEST|(ptPosition?AESC_POINTCLIENT:AESC_POINTCARET)|AESC_OFFSETCHARX|AESC_OFFSETCHARY, ptPosition, 1, 0);

  //Scroll
  return AE_ScrollToPointEx(ae, (ptPosition?AESC_POINTCLIENT:AESC_POINTCARET)|(dwTest & AECSE_SCROLLEDX?AESC_OFFSETRECTDIVX:0)|(dwTest & AECSE_SCROLLEDY?AESC_OFFSETRECTDIVY:0), ptPosition, 3, 2);
}

DWORD AE_ScrollToPointEx(AKELEDIT *ae, DWORD dwFlags, POINT64 *ptPosition, int nOffsetX, int nOffsetY)
{
  POINT64 ptPos;
  INT_PTR x=-1;
  INT_PTR y=-1;
  DWORD dwResult=0;

  if (dwFlags & AESC_POINTCARET)
    ptPos=ae->ptCaret;
  else if (dwFlags & AESC_POINTGLOBAL)
    ptPos=*ptPosition;
  else
    AE_ClientToGlobal(ae, ptPosition->x, ptPosition->y, &ptPos);

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
        if (ae->nVScrollPos < ae->ptxt->nVScrollMax)
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
        AE_GlobalToClient(ae, &ptPos, ptPosition, NULL);
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
      if (!ae->popt->nHScrollLock)
      {
        if (ae->bHScrollShow && ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
          si.nMin=0;
          si.nMax=(int)ae->ptxt->nHScrollMax;
          si.nPage=ae->rcDraw.right - ae->rcDraw.left;
          si.nPos=(int)ae->nHScrollPos;
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
            {
              bSetScroll=TRUE;
            }
            else
            {
              if (ae->ptxt->nHScrollMax <= ae->rcDraw.right - ae->rcDraw.left)
                ae->nHScrollPos=0;
              AE_ScrollEditWindow(ae, SB_HORZ, ae->nHScrollPos);
            }
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
    }

    if (nBar == SB_BOTH || nBar == SB_VERT)
    {
      if (!ae->popt->nVScrollLock)
      {
        if (ae->bVScrollShow && ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
        {
          if (ae->ptxt->nVScrollMax > 0x7fffffff)
          {
            //Vertical scroll exceeds maximum 32-bit value
            ae->popt->dwOptions|=AECO_VSCROLLBYLINE;
          }

          if (ae->popt->dwOptions & AECO_VSCROLLBYLINE)
          {
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
            si.nMin=0;
            si.nMax=(int)(ae->ptxt->nVScrollMax / ae->ptxt->nCharHeight - 1);
            si.nPage=(ae->rcDraw.bottom - ae->rcDraw.top) / ae->ptxt->nCharHeight;
            si.nPos=(int)(ae->nVScrollPos / ae->ptxt->nCharHeight);
            SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

            si.fMask=SIF_POS;
            GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
            ae->nVScrollPos=((INT_PTR)si.nPos) * ae->ptxt->nCharHeight;
          }
          else
          {
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
            si.nMin=0;
            si.nMax=(int)ae->ptxt->nVScrollMax;
            si.nPage=ae->rcDraw.bottom - ae->rcDraw.top;
            si.nPos=(int)ae->nVScrollPos;
            SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

            si.fMask=SIF_POS;
            GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
            ae->nVScrollPos=si.nPos;
          }
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
            {
              bSetScroll=TRUE;
            }
            else
            {
              if (ae->ptxt->nVScrollMax <= ae->rcDraw.bottom - ae->rcDraw.top)
                ae->nVScrollPos=0;
              AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos);
            }
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
  }
  if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
}

INT_PTR AE_ScrollEditWindow(AKELEDIT *ae, int nBar, INT_PTR nPos)
{
  SCROLLINFO si;
  INT_PTR nScrollPos=0;
  BOOL bUpdateScroll=!(ae->popt->dwLockUpdate & AELU_SCROLLBAR);

  if (nBar == SB_HORZ)
  {
    if (!ae->popt->nHScrollLock)
    {
      if (ae->ptxt->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
      {
        if (ae->bHScrollShow)
        {
          si.cbSize=sizeof(SCROLLINFO);
          si.fMask=SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
          si.nPos=(int)nPos;
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
          ae->ptActiveColumnDraw.x+=(int)(ae->nHScrollPos - nPos);

          ScrollWindow(ae->hWndEdit, (int)(ae->nHScrollPos - nPos), 0, NULL, &ae->rcDraw);
          if (bAkelEditWindows9x)
          {
            //Sometimes ScrollWindow don't invalidates scrolling rectangle
            if (mod(ae->nHScrollPos - nPos) > ae->rcDraw.right - ae->rcDraw.left)
              InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
          }
          ae->nHScrollPos=nPos;
          //UpdateWindow(ae->hWndEdit);

          if (ae->nHScrollPos != ae->nLastHScrollPos)
          {
            //Update edit margins
            if (ae->popt->lpBkImage)
              AE_SendEraseBackground(ae, ae->hDC);
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
    if (!ae->popt->nVScrollLock)
    {
      if (ae->ptxt->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
      {
        if (ae->bVScrollShow)
        {
          if (ae->popt->dwOptions & AECO_VSCROLLBYLINE)
          {
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
            si.nPos=(int)(nPos / ae->ptxt->nCharHeight);
            SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

            si.fMask=SIF_POS;
            GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
            nPos=((INT_PTR)si.nPos) * ae->ptxt->nCharHeight;
          }
          else
          {
            si.cbSize=sizeof(SCROLLINFO);
            si.fMask=SIF_POS|(ae->popt->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
            si.nPos=(int)nPos;
            SetScrollInfo(ae->hWndEdit, SB_VERT, &si, bUpdateScroll);

            si.fMask=SIF_POS;
            GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
            nPos=si.nPos;
          }
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
          ae->ptActiveColumnDraw.y+=(int)(ae->nVScrollPos - nPos);

          ScrollWindow(ae->hWndEdit, 0, (int)(ae->nVScrollPos - nPos), NULL, &ae->rcDraw);
          if (bAkelEditWindows9x)
          {
            //Sometimes ScrollWindow don't invalidates scrolling rectangle
            if (mod(ae->nVScrollPos - nPos) > ae->rcDraw.bottom - ae->rcDraw.top)
              InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
          }
          ae->nVScrollPos=nPos;
          //UpdateWindow(ae->hWndEdit);

          if (ae->nVScrollPos != ae->nLastVScrollPos)
          {
            //Update edit margins
            if (ae->popt->lpBkImage)
              AE_SendEraseBackground(ae, ae->hDC);
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

void AE_SetScrollPos(AKELEDIT *ae, INT_PTR nHPos, INT_PTR nVPos)
{
  if (nHPos != ae->nHScrollPos)
    AE_ScrollEditWindow(ae, SB_HORZ, nHPos);
  if (nVPos != ae->nVScrollPos)
    AE_ScrollEditWindow(ae, SB_VERT, nVPos);
}

INT_PTR AE_HScroll(AKELEDIT *ae, int nAction, DWORD dwAlign)
{
  SCROLLINFO si;
  INT_PTR nPos;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);
  nPos=si.nPos;

  if (nAction == SB_LEFT)
  {
    nPos=si.nMin;
  }
  else if (nAction == SB_RIGHT)
  {
    nPos=si.nMax;
  }
  else if (nAction == SB_LINELEFT)
  {
    nPos-=ae->ptxt->nAveCharWidth;
  }
  else if (nAction == SB_LINERIGHT)
  {
    nPos+=ae->ptxt->nAveCharWidth;
  }
  else if (nAction == SB_PAGELEFT)
  {
    nPos-=si.nPage;
  }
  else if (nAction == SB_PAGERIGHT)
  {
    nPos+=si.nPage;
  }
  else if (nAction == SB_THUMBTRACK)
  {
    nPos=si.nTrackPos;
  }
  else if (nAction == SB_THUMBPOSITION)
  {
  }
  nPos=AE_AlignPos(ae, nPos, dwAlign);

  return AE_ScrollEditWindow(ae, SB_HORZ, nPos);
}

INT_PTR AE_VScroll(AKELEDIT *ae, int nAction, DWORD dwAlign)
{
  SCROLLINFO si;
  INT_PTR nPos;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_ALL;
  GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
  nPos=si.nPos;

  if (nAction == SB_TOP)
  {
    nPos=si.nMin;
  }
  else if (nAction == SB_BOTTOM)
  {
    nPos=si.nMax;
  }
  else if (nAction == SB_LINEUP)
  {
    if (ae->popt->dwOptions & AECO_VSCROLLBYLINE)
      nPos-=1;
    else
      nPos-=ae->ptxt->nCharHeight;
  }
  else if (nAction == SB_LINEDOWN)
  {
    if (ae->popt->dwOptions & AECO_VSCROLLBYLINE)
      nPos+=1;
    else
      nPos+=ae->ptxt->nCharHeight;
  }
  else if (nAction == SB_PAGEUP)
  {
    nPos-=si.nPage;
  }
  else if (nAction == SB_PAGEDOWN)
  {
    nPos+=si.nPage;
  }
  else if (nAction == SB_THUMBTRACK)
  {
    nPos=si.nTrackPos;
  }
  else if (nAction == SB_THUMBPOSITION)
  {
  }
  if (ae->popt->dwOptions & AECO_VSCROLLBYLINE)
    nPos*=ae->ptxt->nCharHeight;
  nPos=AE_AlignPos(ae, nPos, dwAlign);

  return AE_ScrollEditWindow(ae, SB_VERT, nPos);
}

INT_PTR AE_HScrollChar(AKELEDIT *ae, int nChar, DWORD dwAlign)
{
  INT_PTR nPos;

  nPos=ae->nHScrollPos;
  nPos+=ae->ptxt->nAveCharWidth * nChar;
  nPos=AE_AlignPos(ae, nPos, dwAlign);
  return AE_ScrollEditWindow(ae, SB_HORZ, nPos);
}

INT_PTR AE_VScrollLine(AKELEDIT *ae, int nLine, DWORD dwAlign)
{
  INT_PTR nPos;

  nPos=ae->nVScrollPos;
  nPos+=ae->ptxt->nCharHeight * nLine;
  nPos=AE_AlignPos(ae, nPos, dwAlign);
  return AE_ScrollEditWindow(ae, SB_VERT, nPos);
}

INT_PTR AE_AlignPos(AKELEDIT *ae, INT_PTR nPos, DWORD dwAlign)
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
  HDC hEditDC;
  HFONT hEditFontOld;
  HFONT hPrintFontOld;

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
      ph->aePrint.ptxt->nPointSize=MulDiv(ph->aePrint.ptxt->lfFontA.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
      ReleaseDC(ae->hWndEdit, hEditDC);
    }
    ph->aePrint.ptxt->lfFontA.lfHeight=MulDiv(ph->aePrint.ptxt->nPointSize, GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY), 72);
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
    ph->aePrint.ptxt->nCharHeight=max(ph->aePrint.ptxt->nCharHeight, 1);
    prn->nCharHeight=ph->aePrint.ptxt->nCharHeight;
    AE_GetFontCharWidth(&ph->aePrint, prn->hPrinterDC);
    if (ae->ptxt->nFixedCharWidth)
    {
      prn->nAveCharWidth=ph->aePrint.ptxt->nFixedCharWidth;
      prn->nSpaceCharWidth=ph->aePrint.ptxt->nFixedCharWidth;
      prn->nTabWidth=ph->aePrint.ptxt->nFixedTabWidth;
    }
    else
    {
      prn->nAveCharWidth=ph->aePrint.ptxt->nAveCharWidth;
      prn->nSpaceCharWidth=ph->aePrint.ptxt->nSpaceCharWidth;
      prn->nTabWidth=ph->aePrint.ptxt->nTabWidth;
    }

    //Return font
    if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);

    //Get print page size
    AE_GetPrintRect(prn, NULL, &prn->rcPageFull);
    AE_GetPrintRect(prn, &prn->rcMargins, &prn->rcPageIn);
    ph->aePrint.rcEdit=prn->rcPageFull;
    ph->aePrint.rcDraw=prn->rcPageIn;
    ph->aePrint.rcErase=ph->aePrint.rcEdit;
  }
  return ph;
}

AEPRINTHANDLE* AE_StartPrintDocW(AKELEDIT *ae, AEPRINT *prn)
{
  AEPRINTHANDLE *ph;
  AEFONTITEMW *fi;
  TEXTMETRICW tmEditW;
  TEXTMETRICW tmPrintW;
  HDC hEditDC;
  HFONT hEditFontOld;
  HFONT hPrintFontOld;

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
      ph->aePrint.ptxt->nPointSize=MulDiv(ph->aePrint.ptxt->lfFontW.lfHeight, 72, GetDeviceCaps(hEditDC, LOGPIXELSY));
      ReleaseDC(ae->hWndEdit, hEditDC);
    }
    ph->aePrint.ptxt->lfFontW.lfHeight=MulDiv(ph->aePrint.ptxt->nPointSize, GetDeviceCaps(prn->hPrinterDC, LOGPIXELSY), 72);
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
    ph->aePrint.ptxt->nCharHeight=max(ph->aePrint.ptxt->nCharHeight, 1);
    prn->nCharHeight=ph->aePrint.ptxt->nCharHeight;
    AE_GetFontCharWidth(&ph->aePrint, prn->hPrinterDC);
    if (ae->ptxt->nFixedCharWidth)
    {
      prn->nAveCharWidth=ph->aePrint.ptxt->nFixedCharWidth;
      prn->nSpaceCharWidth=ph->aePrint.ptxt->nFixedCharWidth;
      prn->nTabWidth=ph->aePrint.ptxt->nFixedTabWidth;
    }
    else
    {
      prn->nAveCharWidth=ph->aePrint.ptxt->nAveCharWidth;
      prn->nSpaceCharWidth=ph->aePrint.ptxt->nSpaceCharWidth;
      prn->nTabWidth=ph->aePrint.ptxt->nTabWidth;
    }

    //Return font
    if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);

    //Get print page size
    AE_GetPrintRect(prn, NULL, &prn->rcPageFull);
    AE_GetPrintRect(prn, &prn->rcMargins, &prn->rcPageIn);
    ph->aePrint.rcEdit=prn->rcPageFull;
    ph->aePrint.rcDraw=prn->rcPageIn;
    ph->aePrint.rcErase=ph->aePrint.rcEdit;
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
  AEHLPAINT hlp;
  AETEXTOUT to;
  AECHARINDEX ciCount;
  AECHARINDEX ciTmp;
  HBRUSH hbrTab;
  HFONT hPrintFontOld;
  RECT rcSpace;
  int nCharWidth=0;
  INT_PTR nLineWidth;
  INT_PTR nMaxLineWidth=0;
  int nLineLen;
  int nLineCount=0;
  INT_PTR nMaxDrawCharOffset=0;
  int nTmp;
  BOOL bFormFeed=FALSE;
  BOOL bContinuePrint=TRUE;

  if (!prn->hPrintFont || !prn->crText.ciMin.lpLine || !prn->crText.ciMax.lpLine)
    return FALSE;

  //Prepare variables
  ae->bPrinting=TRUE;
  hPrintFontOld=(HFONT)SelectObject(prn->hPrinterDC, prn->hPrintFont);

  //Set AETEXTOUT
  xmemset(&to, 0, sizeof(AETEXTOUT));
  to.hDC=ph->aePrint.hDC;
  to.ptFirstCharInLine.x=prn->rcPageIn.left;
  to.ptFirstCharInLine.y=prn->rcPageIn.top;
  to.nDrawCharOffset=AE_AkelIndexToRichOffset(ae, &prn->crText.ciMin);
  to.dwPrintFlags=prn->dwFlags;
  nMaxDrawCharOffset=to.nDrawCharOffset;

  //Set AEHLPAINT
  xmemset(&hlp, 0, sizeof(AEHLPAINT));
  hlp.cb=sizeof(AEHLPAINT);
  ae->popt->hbrBasicBk=CreateSolidBrush(ae->popt->aec.crBasicBk);
  hlp.dwDefaultText=ae->popt->aec.crBasicText;
  hlp.dwDefaultBk=ae->popt->aec.crBasicBk;
  hlp.dwActiveText=hlp.dwDefaultText;
  hlp.dwActiveBk=hlp.dwDefaultBk;
  hlp.dwFontStyle=AEHLS_NONE;
  hlp.fm.hDoc=(AEHDOC)&ph->aePrint;
  hlp.fm.hActiveThemeBegin=(AEHTHEME)ph->aePrint.popt->lpActiveTheme;

  //Fill page rectangle
  if (!(prn->dwFlags & AEPRN_TEST))
  {
    if (prn->dwFlags & AEPRN_COLOREDBACKGROUND)
    {
      FillRect(to.hDC, &prn->rcPageIn, ae->popt->hbrBasicBk);
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
      if (nCharWidth=AE_GetCharWidth(&ph->aePrint, ciCount.lpLine->wpLine + ciCount.nCharInLine, nLineWidth))
        nLineWidth+=nCharWidth;

      if (!(prn->dwFlags & AEPRN_IGNOREFORMFEED) && ciCount.lpLine->wpLine[ciCount.nCharInLine] == L'\f')
        bFormFeed=TRUE;

      if (nLineWidth > prn->rcPageIn.right - prn->rcPageIn.left ||
          AEC_IndexCompare(&ciCount, &prn->crText.ciMax) >= 0 ||
          bFormFeed)
      {
        if (bFormFeed)
        {
          if (AEC_NextCharEx(&ciCount, &prn->crText.ciMin))
            if (AEC_IsLastCharInLine(&prn->crText.ciMin))
              AEC_NextCharEx(&prn->crText.ciMin, &prn->crText.ciMin);
        }
        else
        {
          if (prn->dwFlags & AEPRN_WRAPNONE)
          {
            if (!AEC_NextLineEx(&ciCount, &prn->crText.ciMin))
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

              while (AEC_PrevCharInLine(&ciTmp) && --nTmp >= 0)
              {
                if (AE_IsInDelimiterList(ph->aePrint.ptxt->wszWrapDelimiters, ph->aePrint.ptxt->nWrapDelimitersLen, ciTmp.lpLine->wpLine[ciTmp.nCharInLine]))
                {
                  nLineLen=++nTmp;
                  AEC_NextCharInLineEx(&ciTmp, &ciCount);
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
        if (AEC_IndexCompare(&prn->crText.ciMin, &prn->crText.ciMax) >= 0)
          bContinuePrint=FALSE;
        goto PrintLine;
      }

      //Increment char count
      nLineLen+=AEC_IndexInc(&ciCount);
    }

    //Next line
    if (ciCount.lpLine->nLineBreak == AELB_WRAP)
    {
      AEC_NextLine(&ciCount);
    }
    else
    {
      if (AE_GetIndex(&ph->aePrint, (ae->popt->dwOptionsEx & AECOE_NOPRINTCOLLAPSED)?AEGI_NEXTUNCOLLAPSEDLINE:AEGI_NEXTLINE, &ciCount, &prn->crText.ciMin))
        bContinuePrint=TRUE;
      else
        bContinuePrint=FALSE;
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

  while (to.ciDrawLine.lpLine)
  {
    if (AEC_IsFirstCharInLine(&to.ciDrawLine))
    {
      if (!(prn->dwFlags & AEPRN_TEST))
      {
        if ((prn->dwFlags & AEPRN_COLOREDTEXT))
        {
          //Close all previous items
          AE_PaintCheckHighlightCleanUp(&ph->aePrint, &to, &hlp, &prn->crText.ciMin);
        }
      }
      hlp.dwFindFirst=AEHPT_MARKTEXT|AEHPT_LINK|AEHPT_QUOTE|AEHPT_WORD;
      hlp.dwFindFirst&=~hlp.dwPaintType;
    }

    while (to.ciDrawLine.nCharInLine <= to.ciDrawLine.lpLine->nLineLen)
    {
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
      if (to.nDrawCharOffset >= nMaxDrawCharOffset) goto PrintLineEnd;
      if (to.ciDrawLine.nCharInLine == to.ciDrawLine.lpLine->nLineLen) break;
      nCharWidth=AE_GetCharWidth(&ph->aePrint, to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine, to.nDrawLineWidth);

      if (to.ciDrawLine.lpLine->wpLine[to.ciDrawLine.nCharInLine] == L'\t')
      {
        //Fill tab space
        rcSpace.left=(int)(to.ptFirstCharInLine.x + to.nDrawLineWidth);
        rcSpace.top=(int)to.ptFirstCharInLine.y;
        rcSpace.right=rcSpace.left + nCharWidth;
        rcSpace.bottom=rcSpace.top + ph->aePrint.ptxt->nCharHeight;

        if (!(prn->dwFlags & AEPRN_TEST))
        {
          if (prn->dwFlags & AEPRN_COLOREDBACKGROUND)
          {
            if (hbrTab=CreateSolidBrush(hlp.dwActiveBk))
            {
              FillRect(to.hDC, &rcSpace, hbrTab);
              DeleteObject(hbrTab);
            }
          }
        }

        //Draw text up to tab character
        AE_PaintTextOut(&ph->aePrint, &to, &hlp);

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
      to.nDrawCharOffset+=AEC_IndexInc(&to.ciDrawLine);
    }

    //Next line
    if (to.ciDrawLine.lpLine->nLineBreak != AELB_WRAP)
    {
      ++to.nDrawCharOffset;
      break;
    }
    AE_PaintTextOut(&ph->aePrint, &to, &hlp);
    AEC_NextLine(&to.ciDrawLine);
    to.wpStartDraw=to.ciDrawLine.lpLine->wpLine;
  }

  PrintLineEnd:
  to.nDrawCharOffset+=AE_IndexSubtract(&ph->aePrint, &prn->crText.ciMin, &to.ciDrawLine, AELB_R, FALSE, FALSE);
  AE_PaintTextOut(&ph->aePrint, &to, &hlp);

  //Next line
  to.ptFirstCharInLine.y+=ph->aePrint.ptxt->nCharHeight;
  nMaxLineWidth=max(nLineWidth, nMaxLineWidth);
  ++nLineCount;

  if (bContinuePrint && !bFormFeed && to.ptFirstCharInLine.y + ph->aePrint.ptxt->nCharHeight <= prn->rcPageIn.bottom)
    goto CalculateLine;

  //Fill rcPageOut structure
  prn->rcPageOut.left=prn->rcPageIn.left;
  prn->rcPageOut.top=prn->rcPageIn.top;
  prn->rcPageOut.right=(int)(prn->rcPageIn.left + nMaxLineWidth);
  prn->rcPageOut.bottom=(int)to.ptFirstCharInLine.y;

  //Free resources
  if (hPrintFontOld) SelectObject(prn->hPrinterDC, hPrintFontOld);
  DeleteObject(ae->popt->hbrBasicBk);
  ae->popt->hbrBasicBk=NULL;
  ph->aePrint.popt->lpActiveTheme=(AETHEMEITEMW *)hlp.fm.hActiveThemeBegin;
  ae->bPrinting=FALSE;
  AE_HeapStackClear(ae, (stack **)&hlp.qm.hParentStack.first, (stack **)&hlp.qm.hParentStack.last);
  AE_HeapStackClear(ae, (stack **)&hlp.fm.hParentStack.first, (stack **)&hlp.fm.hParentStack.last);

  return bContinuePrint;
}

void AE_EndPrintDoc(AKELEDIT *ae, AEPRINTHANDLE *ph, AEPRINT *prn)
{
  DeleteObject(prn->hPrintFont);
  prn->hPrintFont=NULL;

  AE_HeapFree(ae, 0, (LPVOID)ph);
}

void AE_FillRect(AKELEDIT *ae, HDC hDC, const RECT *lpRect, HBRUSH hbr)
{
  RECT rcFill;

  rcFill.left=max(lpRect->left, ae->rcErase.left);
  rcFill.top=max(lpRect->top, ae->rcErase.top);
  rcFill.right=min(lpRect->right, ae->rcErase.right);
  rcFill.bottom=min(lpRect->bottom, ae->rcErase.bottom);

  if (ae->popt->lpBkImage)
  {
    //Bitmap as background
    BLENDFUNCTION bf;
    RECT rcCopy;
    int nImageX=(int)((rcFill.left - ae->rcErase.left) + ae->nHScrollPos) % ae->popt->lpBkImage->nBitmapX;
    int nImageY=(int)((rcFill.top - ae->rcErase.top) + ae->nVScrollPos) % ae->popt->lpBkImage->nBitmapY;
    int nImageWidth=ae->popt->lpBkImage->nBitmapX - nImageX;
    int nImageHeight=ae->popt->lpBkImage->nBitmapY - nImageY;

    if (ae->popt->hbrBasicBk != hbr)
    {
      FillRect(hDC, &rcFill, hbr);
      if (!AkelEditAlphaBlendPtr) return;

      bf.BlendOp=AC_SRC_OVER;
      bf.BlendFlags=0;
      bf.SourceConstantAlpha=(BYTE)ae->popt->nBkImageAlpha;
      bf.AlphaFormat=0;
    }
    rcCopy.top=rcFill.top;
    rcCopy.bottom=rcFill.bottom - rcCopy.top;
    if (rcCopy.bottom > nImageHeight)
      rcCopy.bottom=nImageHeight;

    while (rcCopy.bottom > 0)
    {
      //Restore initial X variables
      nImageX=(int)((rcFill.left - ae->rcErase.left) + ae->nHScrollPos) % ae->popt->lpBkImage->nBitmapX;
      rcCopy.left=rcFill.left;
      rcCopy.right=rcFill.right - rcCopy.left;
      if (rcCopy.right > nImageWidth)
        rcCopy.right=nImageWidth;

      //Draw line
      while (rcCopy.right > 0)
      {
        if (ae->popt->hbrBasicBk != hbr)
          AkelEditAlphaBlendPtr(hDC, rcCopy.left, rcCopy.top, rcCopy.right, rcCopy.bottom, ae->popt->lpBkImage->hDC, nImageX, nImageY, rcCopy.right, rcCopy.bottom, bf);
        else
          BitBlt(hDC, rcCopy.left, rcCopy.top, rcCopy.right, rcCopy.bottom, ae->popt->lpBkImage->hDC, nImageX, nImageY, SRCCOPY);

        rcCopy.left+=rcCopy.right;
        rcCopy.right=rcFill.right - rcCopy.left;
        if (rcCopy.right > ae->popt->lpBkImage->nBitmapX)
          rcCopy.right=ae->popt->lpBkImage->nBitmapX;
        nImageX=0;
      }

      //Next line
      rcCopy.top+=rcCopy.bottom;
      rcCopy.bottom=rcFill.bottom - rcCopy.top;
      if (rcCopy.bottom > ae->popt->lpBkImage->nBitmapY)
        rcCopy.bottom=ae->popt->lpBkImage->nBitmapY;
      nImageY=0;
    }
  }
  else FillRect(hDC, &rcFill, hbr);
}

void AE_FillRectWithBorder(AKELEDIT *ae, HDC hDC, const RECT *lpRect, HBRUSH hbrDefaultBk, HBRUSH hbrBorderTop, HBRUSH hbrBorderBottom)
{
  RECT rcBorder;

  if (hbrDefaultBk)
  {
    AE_FillRect(ae, hDC, lpRect, hbrDefaultBk);
  }

  //Draw top and bottom borders
  rcBorder.left=lpRect->left;
  rcBorder.right=lpRect->right;

  if (hbrBorderTop)
  {
    rcBorder.top=lpRect->top;
    rcBorder.bottom=lpRect->top + 1;
    AE_FillRect(ae, hDC, &rcBorder, hbrBorderTop);
  }
  if (hbrBorderBottom)
  {
    rcBorder.top=lpRect->bottom - 1;
    rcBorder.bottom=lpRect->bottom;
    AE_FillRect(ae, hDC, &rcBorder, hbrBorderBottom);
  }
}

void AE_Paint(AKELEDIT *ae, const RECT *lprcUpdate)
{
  AETEXTOUT to;
  AEHLPAINT hlp;
  AENPAINT pntNotify;
  PAINTSTRUCT ps;
  RECT rcUpdate;
  POINT ptActiveColumnDrawOld={0};
  POINT ptActiveColumnDrawNew;
  HBITMAP hBitmap=NULL;
  HBITMAP hBitmapOld=NULL;
  HFONT hFontOld=NULL;
  HRGN hDrawRgn;
  HRGN hDrawRgnOld=NULL;

  //Initialize
  xmemset(&to, 0, sizeof(AETEXTOUT));
  xmemset(&hlp, 0, sizeof(AEHLPAINT));
  xmemset(&pntNotify, 0, sizeof(AENPAINT));

  //Update rectangle
  rcUpdate=*lprcUpdate;
  if (rcUpdate.top + ae->ptxt->nCharHeight <= ae->rcDraw.top)
    rcUpdate.top=ae->rcDraw.top;
  if (rcUpdate.bottom > ae->rcDraw.bottom)
    rcUpdate.bottom=ae->rcDraw.bottom;

  //Set DCs
  to.hDC=ae->hDC;

  if (!(ae->popt->dwOptions & AECO_NODCBUFFER))
  {
    to.hDC=CreateCompatibleDC(ae->hDC);
    hBitmap=CreateCompatibleBitmap(ae->hDC, ae->rcEdit.right, ae->rcEdit.bottom);
    hBitmapOld=(HBITMAP)SelectObject(to.hDC, hBitmap);
  }
  if (ae->ptxt->hFont) hFontOld=(HFONT)SelectObject(to.hDC, ae->ptxt->hFont);

  //Send AEN_PAINT
  pntNotify.hDC=to.hDC;
  AE_NotifyPaint(ae, AEPNT_BEGIN, &pntNotify);

  if (BeginPaint(ae->hWndEdit, &ps))
  {
    if (rcUpdate.right > ae->rcDraw.left &&
        rcUpdate.bottom > ae->rcDraw.top)
    {
      AEFOLD *lpCollapsed;
      HBRUSH hbrDefaultBk;
      HBRUSH hbrSelBk;
      HBRUSH hbrActiveLineBk;
      HBRUSH hbrActiveLineBorder;
      HBRUSH hbrAltLineBk=NULL;
      HBRUSH hbrAltLineBorder=NULL;
      HBRUSH hbrActiveLineBkWithAltBk=NULL;
      HBRUSH hbrActiveLineBorderWithAltBk=NULL;
      HBRUSH hbrActiveLineBorderWithAltBorder=NULL;
      HBRUSH hbrBorderTop;
      HBRUSH hbrBorderBottom;
      HBRUSH hbrTab;
      RECT rcSpace;
      INT_PTR nVertPos;
      INT_PTR nMinPaintWidth=0;
      INT_PTR nMaxPaintWidth=0;
      int nCharWidth=0;
      int nLastDrawLine=0;
      DWORD dwAltModule=0;

      //Avoid graphic rudiments
      hDrawRgn=CreateRectRgn(ae->rcDraw.left, ae->rcDraw.top, ae->rcDraw.right, ae->rcDraw.bottom);
      if (!(ae->popt->dwOptions & AECO_NODCBUFFER))
        hDrawRgnOld=(HRGN)SelectObject(ps.hdc, hDrawRgn);
      else
        hDrawRgnOld=(HRGN)SelectObject(to.hDC, hDrawRgn);

      //Erase active column at old position (on all edit)
      if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
      {
        ptActiveColumnDrawOld=ae->ptActiveColumnDraw;
        AE_GlobalToClient(ae, &ae->ptCaret, NULL, &ptActiveColumnDrawNew);

        if (ptActiveColumnDrawOld.x != -0x7fffffff && ptActiveColumnDrawOld.x != ptActiveColumnDrawNew.x &&
            ptActiveColumnDrawOld.x >= ae->rcDraw.left && ptActiveColumnDrawOld.x <= ae->rcDraw.right)
        {
          //Transfer 1-pixel column to buffer DC
          BitBlt(to.hDC, ptActiveColumnDrawOld.x, ae->rcDraw.top, 1, ae->rcDraw.bottom - ae->rcDraw.top, ae->hDC, ptActiveColumnDrawOld.x, ae->rcDraw.top, SRCCOPY);

          //Erase column at old caret position. Use buffer DC, because many MoveToEx, LineTo calls cause slow painting in Win7 with aero.
          AE_ActiveColumnDraw(ae, to.hDC, ae->rcDraw.top, ae->rcDraw.bottom);

          //Transfer 1-pixel column back to edit DC
          BitBlt(ae->hDC, ptActiveColumnDrawOld.x, ae->rcDraw.top, 1, ae->rcDraw.bottom - ae->rcDraw.top, to.hDC, ptActiveColumnDrawOld.x, ae->rcDraw.top, SRCCOPY);
        }
        ae->ptActiveColumnDraw=ptActiveColumnDrawNew;
      }

      //Create GDI objects
      ae->popt->hbrBasicBk=CreateSolidBrush(ae->popt->aec.crBasicBk);
      hbrSelBk=CreateSolidBrush(ae->popt->aec.crSelBk);
      hbrActiveLineBk=CreateSolidBrush(ae->popt->aec.crActiveLineBk);
      hbrActiveLineBorder=CreateSolidBrush(ae->popt->aec.crActiveLineBorder);
      if (ae->popt->dwAltLineSkip && ae->popt->dwAltLineFill)
      {
        hbrAltLineBk=CreateSolidBrush(ae->popt->aec.crAltLineBk);
        hbrAltLineBorder=CreateSolidBrush(ae->popt->aec.crAltLineBorder);
        hbrActiveLineBkWithAltBk=CreateSolidBrush(ae->popt->crActiveLineBkWithAltBk);
        hbrActiveLineBorderWithAltBk=CreateSolidBrush(ae->popt->crActiveLineBorderWithAltBk);
        hbrActiveLineBorderWithAltBorder=CreateSolidBrush(ae->popt->crActiveLineBorderWithAltBorder);
      }

      //First char in line coordinates
      nVertPos=(ae->nVScrollPos + (rcUpdate.top - ae->rcDraw.top)) / ae->ptxt->nCharHeight * ae->ptxt->nCharHeight;
      to.ptFirstCharInLine.x=ae->rcDraw.left - ae->nHScrollPos;
      to.ptFirstCharInLine.y=(nVertPos - ae->nVScrollPos) + ae->rcDraw.top;

      //First paint line
      to.ciDrawLine.nLine=(int)AE_LineFromVPos(ae, nVertPos);
      to.ciDrawLine.lpLine=AE_GetLineData(ae, to.ciDrawLine.nLine);
      to.ciDrawLine.nCharInLine=0;

      //Last paint line
      nLastDrawLine=AE_LineFromVPos(ae, ae->nVScrollPos + (rcUpdate.bottom - ae->rcDraw.top));
      nLastDrawLine=min(nLastDrawLine, ae->ptxt->nLineCount);

      nMinPaintWidth=ae->nHScrollPos + max(rcUpdate.left - ae->rcDraw.left, 0);
      nMaxPaintWidth=ae->nHScrollPos + max(rcUpdate.right - ae->rcDraw.left, 0);

      //Remember first paint line
      if (to.ciDrawLine.lpLine)
      {
        ae->nFirstDrawLineOffset=AE_AkelIndexToRichOffset(ae, &to.ciDrawLine);
        ae->liFirstDrawLine.nLine=to.ciDrawLine.nLine;
        ae->liFirstDrawLine.lpLine=to.ciDrawLine.lpLine;
      }
      to.nDrawCharOffset=ae->nFirstDrawLineOffset;

      //Check that active highlight theme exists
      if (ae->popt->lpActiveTheme)
      {
        if (!AE_HighlightIsThemeExists(ae->popt->lpActiveTheme))
          ae->popt->lpActiveTheme=NULL;
      }
      hlp.cb=sizeof(AEHLPAINT);
      hlp.fm.hDoc=(AEHDOC)ae;
      hlp.fm.hActiveThemeBegin=(AEHTHEME)ae->popt->lpActiveTheme;
      to.dwPrintFlags=AEPRN_COLOREDTEXT|AEPRN_COLOREDBACKGROUND|(ae->popt->bHideSelection?0:AEPRN_COLOREDSELECTION);

      //Paint lines
      while (to.ciDrawLine.lpLine)
      {
        //Line must be visible
        if (!(lpCollapsed=AE_IsLineCollapsed(ae, to.ciDrawLine.nLine)))
        {
          //Close all previous items
          AE_PaintCheckHighlightCleanUp(ae, &to, &hlp, &to.ciDrawLine);
          hlp.dwFindFirst=AEHPT_MARKTEXT|AEHPT_LINK|AEHPT_QUOTE|AEHPT_WORD;
          hlp.dwFindFirst&=~hlp.dwPaintType;

          //Get first paint char in line
          AE_GetCharInLine(ae, to.ciDrawLine.lpLine, nMinPaintWidth - ae->ptxt->nAveCharWidth, AECIL_ALLPOS, &to.ciDrawLine.nCharInLine, &to.nDrawLineWidth, FALSE);
          to.nDrawCharOffset+=min(to.ciDrawLine.nCharInLine, to.ciDrawLine.lpLine->nLineLen);
          to.nStartDrawWidth=to.nDrawLineWidth;
          to.nMaxDrawCharsCount=0;
          to.wpStartDraw=to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine;

          //Remember first paint char in line for notify
          pntNotify.ciMinDraw=to.ciDrawLine;
          pntNotify.nMinDrawOffset=to.nDrawCharOffset;
          pntNotify.ptMinDraw.x=(int)(to.ptFirstCharInLine.x + to.nStartDrawWidth);
          pntNotify.ptMinDraw.y=(int)to.ptFirstCharInLine.y;

          //Is line alternating
          hbrBorderTop=NULL;
          hbrBorderBottom=NULL;

          if (ae->popt->dwAltLineSkip && ae->popt->dwAltLineFill)
          {
            dwAltModule=to.ciDrawLine.nLine % (ae->popt->dwAltLineSkip + ae->popt->dwAltLineFill);

            if (dwAltModule >= ae->popt->dwAltLineSkip)
            {
              if (ae->popt->dwOptions & AECO_ALTLINEBORDER)
              {
                if (dwAltModule == ae->popt->dwAltLineSkip)
                  hbrBorderTop=hbrAltLineBorder;
                if (dwAltModule == ae->popt->dwAltLineSkip + ae->popt->dwAltLineFill - 1)
                  hbrBorderBottom=hbrAltLineBorder;
              }
            }
            else dwAltModule=0;
          }

          //Set initial colors
          if ((ae->popt->dwOptions & AECO_ACTIVELINE) && to.ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
          {
            if (dwAltModule)
            {
              hlp.dwDefaultText=ae->popt->crActiveLineTextWithAltText;
              hlp.dwDefaultBk=ae->popt->crActiveLineBkWithAltBk;
              hbrDefaultBk=hbrActiveLineBkWithAltBk;
              if (ae->popt->dwOptions & AECO_ACTIVELINEBORDER)
              {
                if (hbrBorderTop)
                  hbrBorderTop=hbrActiveLineBorderWithAltBorder;
                else
                  hbrBorderTop=hbrActiveLineBorderWithAltBk;
                if (hbrBorderBottom)
                  hbrBorderBottom=hbrActiveLineBorderWithAltBorder;
                else
                  hbrBorderBottom=hbrActiveLineBorderWithAltBk;
              }
            }
            else
            {
              hlp.dwDefaultText=ae->popt->aec.crActiveLineText;
              hlp.dwDefaultBk=ae->popt->aec.crActiveLineBk;
              hbrDefaultBk=hbrActiveLineBk;
              if (ae->popt->dwOptions & AECO_ACTIVELINEBORDER)
              {
                hbrBorderTop=hbrActiveLineBorder;
                hbrBorderBottom=hbrActiveLineBorder;
              }
            }
          }
          else if (dwAltModule)
          {
            hlp.dwDefaultText=ae->popt->aec.crAltLineText;
            hlp.dwDefaultBk=ae->popt->aec.crAltLineBk;
            hbrDefaultBk=hbrAltLineBk;
          }
          else
          {
            hlp.dwDefaultText=ae->popt->aec.crBasicText;
            hlp.dwDefaultBk=ae->popt->aec.crBasicBk;
            hbrDefaultBk=ae->popt->hbrBasicBk;
          }
          hlp.dwActiveText=hlp.dwDefaultText;
          hlp.dwActiveBk=hlp.dwDefaultBk;
          hlp.dwFontStyle=AEHLS_NONE;

          //Erase space where text will be drawn.
          rcSpace.left=rcUpdate.left;
          rcSpace.top=(int)to.ptFirstCharInLine.y;
          rcSpace.right=rcUpdate.right;
          rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
          AE_FillRectWithBorder(ae, to.hDC, &rcSpace, hbrDefaultBk, hbrBorderTop, hbrBorderBottom);

          //Fill space after line end, before text line is drawn.
          if (to.ciDrawLine.lpLine->nLineWidth <= nMaxPaintWidth)
          {
            rcSpace.right=ae->rcDraw.left + (int)(to.ciDrawLine.lpLine->nLineWidth - ae->nHScrollPos);

            if (to.dwPrintFlags & AEPRN_COLOREDSELECTION)
            {
              if (ae->bColumnSel)
              {
                //Draw column selection
                if (to.ciDrawLine.lpLine->nSelStart != to.ciDrawLine.lpLine->nSelEnd)
                {
                  if (to.ciDrawLine.lpLine->nSelStart >= to.ciDrawLine.lpLine->nLineLen)
                  {
                    rcSpace.left=rcSpace.right;
                    rcSpace.top=(int)to.ptFirstCharInLine.y;
                    rcSpace.right=rcSpace.left + (to.ciDrawLine.lpLine->nSelStart - to.ciDrawLine.lpLine->nLineLen) * (ae->ptxt->nFixedCharWidth?ae->ptxt->nFixedCharWidth:ae->ptxt->nSpaceCharWidth);
                    rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                    AE_FillRectWithBorder(ae, to.hDC, &rcSpace, hbrDefaultBk, hbrBorderTop, hbrBorderBottom);
                  }
                  if (to.ciDrawLine.lpLine->nSelEnd > to.ciDrawLine.lpLine->nLineLen)
                  {
                    if (rcSpace.right < ae->rcDraw.right)
                    {
                      rcSpace.left=rcSpace.right;
                      rcSpace.top=(int)to.ptFirstCharInLine.y;
                      rcSpace.right=rcSpace.left + (to.ciDrawLine.lpLine->nSelEnd - max(to.ciDrawLine.lpLine->nSelStart, to.ciDrawLine.lpLine->nLineLen)) * (ae->ptxt->nFixedCharWidth?ae->ptxt->nFixedCharWidth:ae->ptxt->nSpaceCharWidth);
                      rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                      AE_FillRect(ae, to.hDC, &rcSpace, hbrSelBk);
                    }
                  }
                }
              }
              else
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
                      rcSpace.top=(int)to.ptFirstCharInLine.y;
                      rcSpace.right=ae->rcDraw.right;
                      rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                      AE_FillRect(ae, to.hDC, &rcSpace, hbrSelBk);
                    }
                  }
                  else if (!(ae->popt->dwOptions & AECO_NONEWLINEDRAW) &&
                           to.ciDrawLine.lpLine->nLineBreak != AELB_WRAP)
                  {
                    if (to.ciDrawLine.lpLine->nLineWidth + ae->ptxt->nAveCharWidth > ae->nHScrollPos)
                    {
                      rcSpace.left=rcSpace.right;
                      rcSpace.top=(int)to.ptFirstCharInLine.y;
                      rcSpace.right=rcSpace.left + ae->ptxt->nAveCharWidth;
                      rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
                      AE_FillRect(ae, to.hDC, &rcSpace, hbrSelBk);
                    }
                  }
                }
              }
            }

            //Fill space to the right edge
            if (rcSpace.right < ae->rcDraw.right)
            {
              rcSpace.left=rcSpace.right;
              rcSpace.top=(int)to.ptFirstCharInLine.y;
              rcSpace.right=ae->rcDraw.right;
              rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;
              AE_FillRectWithBorder(ae, to.hDC, &rcSpace, hbrDefaultBk, hbrBorderTop, hbrBorderBottom);
            }
          }

          //Scan line
          while (to.ciDrawLine.nCharInLine <= to.ciDrawLine.lpLine->nLineLen)
          {
            //Check highlight close
            AE_PaintCheckHighlightCloseItem(ae, &to, &hlp);

            //Check highlight open
            AE_PaintCheckHighlightOpenItem(ae, &to, &hlp, nLastDrawLine);

            if (to.ciDrawLine.nCharInLine == to.ciDrawLine.lpLine->nLineLen) break;
            nCharWidth=AE_GetCharWidth(ae, to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine, to.nDrawLineWidth);

            //Draw text up to tab character
            if (to.ciDrawLine.lpLine->wpLine[to.ciDrawLine.nCharInLine] == L'\t')
            {
              if (hlp.dwActiveBk != hlp.dwDefaultBk)
              {
                //Fill tab space
                rcSpace.left=(int)(to.ptFirstCharInLine.x + to.nDrawLineWidth);
                rcSpace.top=(int)to.ptFirstCharInLine.y;
                rcSpace.right=rcSpace.left + nCharWidth;
                rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;

                if (hbrTab=CreateSolidBrush(hlp.dwActiveBk))
                {
                  AE_FillRect(ae, to.hDC, &rcSpace, hbrTab);
                  DeleteObject(hbrTab);
                }
              }

              //Draw text up to tab character
              AE_PaintTextOut(ae, &to, &hlp);

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
            to.nDrawCharOffset+=AEC_IndexInc(&to.ciDrawLine);
          }
          if (to.ciDrawLine.nCharInLine < to.ciDrawLine.lpLine->nLineLen)
            to.nDrawCharOffset+=to.ciDrawLine.lpLine->nLineLen - to.ciDrawLine.nCharInLine;

          //Draw text line
          AE_PaintTextOut(ae, &to, &hlp);

          //Line rectangle
          rcSpace.left=max(rcUpdate.left, ae->rcDraw.left);
          rcSpace.top=(int)to.ptFirstCharInLine.y;
          rcSpace.right=rcSpace.left + min(rcUpdate.right - rcSpace.left, ae->rcDraw.right - rcSpace.left);
          rcSpace.bottom=rcSpace.top + ae->ptxt->nCharHeight;

          //Send AEN_PAINT
          if (ae->popt->dwEventMask & AENM_PAINT)
          {
            pntNotify.ciMaxDraw=to.ciDrawLine;
            pntNotify.nMaxDrawOffset=to.nDrawCharOffset;
            pntNotify.ptMaxDraw.x=(int)(to.ptFirstCharInLine.x + to.nStartDrawWidth);
            pntNotify.ptMaxDraw.y=(int)to.ptFirstCharInLine.y;
            AE_NotifyPaint(ae, AEPNT_DRAWLINE, &pntNotify);
          }

          //Draw column marker
          AE_ColumnMarkerDraw(ae, to.hDC, rcSpace.top, rcSpace.bottom);

          //Draw active column at new position (on line)
          if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
          {
            if (ptActiveColumnDrawOld.x == ptActiveColumnDrawNew.x)
              AE_ActiveColumnDraw(ae, to.hDC, rcSpace.top, rcSpace.bottom);
          }

          if (!(ae->popt->dwOptions & AECO_NODCBUFFER))
          {
            //Copy line from buffer DC
            BitBlt(ps.hdc, rcSpace.left, rcSpace.top, rcSpace.right - rcSpace.left, rcSpace.bottom - rcSpace.top, to.hDC, rcSpace.left, rcSpace.top, SRCCOPY);
          }

          //Next line
          to.ptFirstCharInLine.y+=ae->ptxt->nCharHeight;
          if (to.ptFirstCharInLine.y >= rcUpdate.bottom)
            break;

          if (to.ciDrawLine.lpLine->nLineBreak != AELB_WRAP)
            ++to.nDrawCharOffset;
          AEC_NextLine(&to.ciDrawLine);
        }
        else
        {
          if (AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDLINE, &to.ciDrawLine, &to.ciDrawLine))
            to.nDrawCharOffset=lpCollapsed->lpMaxPoint->nPointOffset - AE_IndexSubtract(ae, &lpCollapsed->lpMaxPoint->ciPoint, &to.ciDrawLine, AELB_R, FALSE, FALSE);
          else
            break;
        }
      }

      //Draw active column at new position (on all edit)
      if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
      {
        if (ptActiveColumnDrawOld.x != ptActiveColumnDrawNew.x && ae->ptActiveColumnDraw.x >= ae->rcDraw.left && ae->ptActiveColumnDraw.x <= ae->rcDraw.right)
        {
          //Transfer 1-pixel column to buffer DC
          BitBlt(to.hDC, ae->ptActiveColumnDraw.x, ae->rcDraw.top, 1, ae->rcDraw.bottom - ae->rcDraw.top, ae->hDC, ae->ptActiveColumnDraw.x, ae->rcDraw.top, SRCCOPY);

          //Draw column at new caret position. Use buffer DC, because many MoveToEx, LineTo calls cause slow painting in Win7 with aero.
          AE_ActiveColumnDraw(ae, to.hDC, ae->rcDraw.top, ae->rcDraw.bottom);

          //Transfer 1-pixel column back to edit DC
          BitBlt(ae->hDC, ae->ptActiveColumnDraw.x, ae->rcDraw.top, 1, ae->rcDraw.bottom - ae->rcDraw.top, to.hDC, ae->ptActiveColumnDraw.x, ae->rcDraw.top, SRCCOPY);
        }
      }

      //Clean-up
      if (hDrawRgnOld)
      {
        if (!(ae->popt->dwOptions & AECO_NODCBUFFER))
          SelectObject(ps.hdc, hDrawRgnOld);
        else
          SelectObject(to.hDC, hDrawRgnOld);
      }
      DeleteObject(hDrawRgn);

      if (hbrActiveLineBorderWithAltBorder) DeleteObject(hbrActiveLineBorderWithAltBorder);
      if (hbrActiveLineBorderWithAltBk) DeleteObject(hbrActiveLineBorderWithAltBk);
      if (hbrActiveLineBkWithAltBk) DeleteObject(hbrActiveLineBkWithAltBk);
      if (hbrAltLineBorder) DeleteObject(hbrAltLineBorder);
      if (hbrAltLineBk) DeleteObject(hbrAltLineBk);
      DeleteObject(hbrActiveLineBorder);
      DeleteObject(hbrActiveLineBk);
      DeleteObject(hbrSelBk);
      DeleteObject(ae->popt->hbrBasicBk);
      ae->popt->hbrBasicBk=NULL;
      ae->popt->lpActiveTheme=(AETHEMEITEMW *)hlp.fm.hActiveThemeBegin;
    }
    EndPaint(ae->hWndEdit, &ps);
  }

  //Send AEN_PAINT
  AE_NotifyPaint(ae, AEPNT_END, &pntNotify);

  //Free DC
  if (hFontOld) SelectObject(to.hDC, hFontOld);

  if (!(ae->popt->dwOptions & AECO_NODCBUFFER))
  {
    if (hBitmapOld) SelectObject(to.hDC, hBitmapOld);
    DeleteObject(hBitmap);
    DeleteDC(to.hDC);
  }
  AE_HeapStackClear(ae, (stack **)&hlp.qm.hParentStack.first, (stack **)&hlp.qm.hParentStack.last);
  AE_HeapStackClear(ae, (stack **)&hlp.fm.hParentStack.first, (stack **)&hlp.fm.hParentStack.last);
}

void AE_PaintTextOut(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp)
{
  RECT rcTextOut;
  SIZE sizeChar;
  HBRUSH hBrush;
  HFONT hFontPrev;
  char *szText;
  char szChar[4];
  wchar_t *wpText=to->wpStartDraw;
  int nTextLen=to->ciDrawLine.nCharInLine - (int)(to->wpStartDraw - to->ciDrawLine.lpLine->wpLine);
  int nTextWidth=(int)(to->nDrawLineWidth - to->nStartDrawWidth);
  int nCharLen;
  int nLeftOffset;
  int nBytes;
  int i;
  DWORD dwFontStyle;
  DWORD dwTextOutFlags=0;

  if (to->gh)
  {
    if (nTextLen || !to->ciDrawLine.lpLine->nLineLen || (to->dwPrintFlags & AEPRN_CALLEMPTY))
    {
      AECHARRANGE crAkelRange;
      CHARRANGE64 crRichRange;

      crAkelRange.ciMax=to->ciDrawLine;
      crAkelRange.ciMin=to->ciDrawLine;
      crAkelRange.ciMin.nCharInLine-=nTextLen;
      crRichRange.cpMax=to->nDrawCharOffset;
      crRichRange.cpMin=to->nDrawCharOffset - nTextLen;
      to->gh->dwError=to->gh->lpCallback(to->gh->dwCookie, &crAkelRange, &crRichRange, hlp);
    }
  }
  if (nTextLen)
  {
    if (!(to->dwPrintFlags & AEPRN_TEST))
    {
      if (to->dwPrintFlags & AEPRN_COLOREDTEXT)
        SetTextColor(to->hDC, hlp->dwActiveText);
      if (to->dwPrintFlags & AEPRN_COLOREDBACKGROUND)
        SetBkColor(to->hDC, hlp->dwActiveBk);

      rcTextOut.left=(int)(to->ptFirstCharInLine.x + to->nStartDrawWidth);
      rcTextOut.top=(int)to->ptFirstCharInLine.y;
      rcTextOut.right=ae->rcDraw.right/*rcTextOut.left + nTextWidth*/;
      rcTextOut.bottom=rcTextOut.top + ae->ptxt->nCharHeight;

      if (rcTextOut.right > ae->rcDraw.left &&
          rcTextOut.left < ae->rcDraw.right)
      {
        hFontPrev=ae->ptxt->hFont;

        if (to->dwPrintFlags & AEPRN_COLOREDTEXT)
        {
          if (!(hlp->dwPaintType & AEHPT_SELECTION))
          {
            //Font
            if (hlp->dwPaintType & AEHPT_LINK)
            {
              hFontPrev=ae->ptxt->hFontUrl;
            }
            dwFontStyle=hlp->dwFontStyle;
            if (ae->popt->dwHLOptions)
            {
              if (ae->popt->dwHLOptions & AEHLO_IGNOREFONTITALIC)
              {
                if (dwFontStyle == AEHLS_FONTITALIC)
                  dwFontStyle=AEHLS_FONTNORMAL;
                else if (dwFontStyle == AEHLS_FONTBOLDITALIC)
                  dwFontStyle=AEHLS_FONTBOLD;
              }
              if (ae->popt->dwHLOptions & AEHLO_IGNOREFONTBOLD)
              {
                if (dwFontStyle == AEHLS_FONTBOLD)
                  dwFontStyle=AEHLS_FONTNORMAL;
                else if (dwFontStyle == AEHLS_FONTBOLDITALIC)
                  dwFontStyle=AEHLS_FONTITALIC;
              }
              if (ae->popt->dwHLOptions & AEHLO_IGNOREFONTNORMAL)
              {
                if (dwFontStyle == AEHLS_FONTNORMAL)
                  dwFontStyle=AEHLS_NONE;
              }
            }
            if (dwFontStyle)
            {
              if (dwFontStyle == AEHLS_FONTNORMAL)
                hFontPrev=ae->ptxt->hFontNormal;
              else if (dwFontStyle == AEHLS_FONTBOLD)
                hFontPrev=ae->ptxt->hFontBold;
              else if (dwFontStyle == AEHLS_FONTITALIC)
                hFontPrev=ae->ptxt->hFontItalic;
              else if (dwFontStyle == AEHLS_FONTBOLDITALIC)
                hFontPrev=ae->ptxt->hFontBoldItalic;
            }
            if (hFontPrev != ae->ptxt->hFont && hFontPrev)
              SelectObject(to->hDC, hFontPrev);

            //Background
            if (hlp->dwActiveBk != hlp->dwDefaultBk)
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
            if (hBrush=CreateSolidBrush(hlp->dwActiveBk))
            {
              AE_FillRect(ae, to->hDC, &rcTextOut, hBrush);
              DeleteObject(hBrush);
            }
            SetBkMode(to->hDC, TRANSPARENT);
          }

          for (i=0; i < nTextLen; ++i)
          {
            if (nCharLen=AEC_CopyChar(NULL, nTextLen - i, wpText + i))
            {
              if (ae->ptxt->nFixedCharWidth)
              {
                if (nCharLen > 1)
                  GetTextExtentPoint32W(to->hDC, wpText + i, nCharLen, &sizeChar);
                else
                  sizeChar.cx=ae->ptxt->lpCharWidths[wpText[i]];
                nLeftOffset=ae->ptxt->nFixedCharWidth / 2 - sizeChar.cx / 2;
              }
              else nLeftOffset=0;

              if (!(to->dwPrintFlags & AEPRN_ANSI))
              {
                TextOutW(to->hDC, rcTextOut.left + nLeftOffset, rcTextOut.top, wpText + i, nCharLen);
              }
              else
              {
                nBytes=WideCharToMultiByte(CP_ACP, 0, wpText + i, 1, szChar, sizeof(szChar), NULL, NULL);
                TextOutA(to->hDC, rcTextOut.left + nLeftOffset, rcTextOut.top, szChar, nBytes);
              }
              rcTextOut.left+=AE_GetCharWidth(ae, wpText + i, 0);
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

        if (hFontPrev != ae->ptxt->hFont && ae->ptxt->hFont)
          SelectObject(to->hDC, ae->ptxt->hFont);
        if (GetBkMode(to->hDC) == TRANSPARENT)
          SetBkMode(to->hDC, OPAQUE);
      }
    }
    to->wpStartDraw+=nTextLen;
    to->nStartDrawWidth+=nTextWidth;
  }
  to->nMaxDrawCharsCount=0;
}

REGROUP* AE_PatCharInGroup(STACKREGROUP *hStack, const AECHARINDEX *ciChar, REGROUP **lppREGroupEnd)
{
  REGROUP *lpREGroupItem;
  int nCompare;

  for (lpREGroupItem=hStack->first; lpREGroupItem;)
  {
    if (lpREGroupItem->nStrLen)
    {
      if (lpREGroupItem->dwUserData)
      {
        if (AEC_IndexCompare(ciChar, &lpREGroupItem->ciStrStart) < 0)
          return NULL;

        nCompare=AEC_IndexCompare(ciChar, &lpREGroupItem->ciStrEnd);
        if (nCompare == 0)
          *lppREGroupEnd=lpREGroupItem;
        else if (nCompare < 0)
          break;
      }
      lpREGroupItem=PatNextGroup(lpREGroupItem);
    }
    else lpREGroupItem=PatNextGroupNoChild(lpREGroupItem);
  }
  return lpREGroupItem;
}

void AE_PaintCheckHighlightOpenItem(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, int nLastDrawLine)
{
  AEFOLD *lpColored=NULL;
  INT_PTR nFoundChild=0;
  BOOL bLockHighLight=FALSE;

  if (to->dwPrintFlags & AEPRN_COLOREDSELECTION)
  {
    if (AEC_IsCharInSelection(&to->ciDrawLine))
    {
      if (!(hlp->dwPaintType & AEHPT_SELECTION))
      {
        //Draw text up to selection start
        AE_PaintTextOut(ae, to, hlp);
        hlp->dwPaintType|=AEHPT_SELECTION;
      }
      hlp->dwActiveText=ae->popt->aec.crSelText;
      hlp->dwActiveBk=ae->popt->aec.crSelBk;
      hlp->dwPaintType|=AEHPT_SELECTION;
      hlp->dwFontStyle=AEHLS_NONE;
    }
  }
  if (hlp->dwPaintType & (AEHPT_SELECTION|AEHPT_MARKRANGE|AEHPT_MARKTEXT))
    bLockHighLight=TRUE;

  if (ae->popt->dwOptionsEx & AECOE_DETECTURL)
  {
    //Url find
    if (AEC_IndexCompare(&hlp->crLink.ciMax, &to->ciDrawLine) <= 0)
    {
      if (hlp->dwFindFirst & AEHPT_LINK)
      {
        if (!AE_HighlightFindUrl(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, nLastDrawLine, &hlp->crLink))
        {
          hlp->crLink.ciMin=to->ciDrawLine;
          hlp->crLink.ciMin.lpLine=NULL;
          hlp->crLink.ciMax=hlp->crLink.ciMin;
        }
        hlp->dwFindFirst&=~AEHPT_LINK;
      }
      else AE_HighlightFindUrl(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, nLastDrawLine, &hlp->crLink);
    }

    //Check url start
    if (hlp->crLink.ciMin.lpLine && hlp->crLink.ciMax.lpLine)
    {
      if (!(hlp->dwPaintType & AEHPT_LINK))
      {
        if (AEC_IndexCompare(&to->ciDrawLine, &hlp->crLink.ciMin) >= 0 &&
            AEC_IndexCompare(&to->ciDrawLine, &hlp->crLink.ciMax) < 0)
        {
          if (!bLockHighLight)
          {
            //Draw text before range for highlight
            AE_PaintTextOut(ae, to, hlp);
          }
          hlp->dwPaintType|=AEHPT_LINK;
        }
      }
      if (!bLockHighLight && (hlp->dwPaintType & AEHPT_LINK))
      {
        AEURLVISIT *lpUrlVisit;

        //Is cursor on URL
        if (ae->nCurrentCursor == AECC_URL && !xmemcmp(&ae->crMouseOnLink, &hlp->crLink, sizeof(AECHARRANGE)))
          hlp->dwActiveText=ae->popt->aec.crUrlCursorText;
        else if ((lpUrlVisit=AE_UrlVisitGetByRange(ae, &hlp->crLink)) && lpUrlVisit->nVisitCount > 0)
          hlp->dwActiveText=ae->popt->aec.crUrlVisitText;
        else
          hlp->dwActiveText=ae->popt->aec.crUrlText;
        hlp->dwActiveBk=hlp->dwDefaultBk;
        hlp->dwFontStyle=AEHLS_NONE;
      }
    }
  }

  if (ae->ptxt->nFoldColorCount || ae->ptxt->nFoldWithIdCount || ae->ptxt->nFoldWithThemeCount)
  {
    //Only if char not in URL
    if (!(hlp->dwPaintType & AEHPT_LINK))
    {
      //Fold find
      if (to->nDrawCharOffset < hlp->fm.crFoldEnd.cpMin || to->nDrawCharOffset >= hlp->fm.crFoldEnd.cpMax)
      {
        AEFOLD *lpThemed=NULL;
        AEFOLD *lpCount;
        AEFOLD *lpFold;

        AE_StackFindFold(ae, AEFF_FINDOFFSET|AEFF_FOLDSTART|AEFF_RECURSE, to->nDrawCharOffset, NULL, &lpFold, NULL);
        if (hlp->fm.lpFold != lpFold)
        {
          if (lpFold)
          {
            if (ae->ptxt->nFoldWithThemeCount || ae->ptxt->nFoldColorCount)
            {
              for (lpCount=lpFold; lpCount; lpCount=lpCount->parent)
              {
                if (lpCount->dwFlags & AEFOLDF_STYLED)
                {
                  //Fold has highlighting information
                  if (!lpColored) lpColored=lpCount;
                }
                if (lpCount->hRuleTheme)
                {
                  //Fold has highlight theme
                  lpThemed=lpCount;
                  break;
                }
              }
              if (lpThemed)
              {
                if (to->nDrawCharOffset >= lpThemed->lpMinPoint->nPointOffset &&
                    to->nDrawCharOffset < lpThemed->lpMaxPoint->nPointOffset + lpThemed->lpMaxPoint->nPointLen)
                {
                  if (to->nDrawCharOffset >= lpThemed->lpMinPoint->nPointOffset + lpThemed->lpMinPoint->nPointLen &&
                      to->nDrawCharOffset < lpThemed->lpMaxPoint->nPointOffset)
                  {
                    if ((AEHTHEME)ae->popt->lpActiveTheme != lpThemed->hRuleTheme)
                    {
                      //Activate child fold with own theme
                      AEACTIVETHEMEITEM *lpActiveThemeItem;

                      if (!AE_HeapStackInsertBefore(ae, (stack **)&hlp->fm.hParentStack.first, (stack **)&hlp->fm.hParentStack.last, NULL, (stack **)&lpActiveThemeItem, sizeof(AEACTIVETHEMEITEM)))
                        lpActiveThemeItem->hActiveTheme=(AEHTHEME)ae->popt->lpActiveTheme;
                      ae->popt->lpActiveTheme=(AETHEMEITEMW *)lpThemed->hRuleTheme;
                    }
                  }
                  else
                  {
                    //Return to main theme
                    if ((AEHTHEME)ae->popt->lpActiveTheme == lpThemed->hRuleTheme)
                    {
                      ae->popt->lpActiveTheme=(AETHEMEITEMW *)(hlp->fm.hParentStack.last?hlp->fm.hParentStack.last->hActiveTheme:NULL);
                      AE_HeapStackDelete(ae, (stack **)&hlp->fm.hParentStack.first, (stack **)&hlp->fm.hParentStack.last, (stack *)hlp->fm.hParentStack.last);
                      AE_PaintCheckHighlightReset(ae, to, hlp, &lpThemed->lpMaxPoint->ciPoint);
                    }
                  }
                }
                else
                {
                  lpFold=NULL;
                  lpColored=NULL;
                }
              }
              if (lpColored)
                lpFold=lpColored;
            }
            if (lpFold)
            {
              hlp->fm.crFoldStart.cpMin=lpFold->lpMinPoint->nPointOffset;
              hlp->fm.crFoldStart.cpMax=lpFold->lpMinPoint->nPointOffset + lpFold->lpMinPoint->nPointLen;
              hlp->fm.crFoldEnd.cpMin=lpFold->lpMaxPoint->nPointOffset;
              hlp->fm.crFoldEnd.cpMax=lpFold->lpMaxPoint->nPointOffset + lpFold->lpMaxPoint->nPointLen;
            }
          }
          hlp->fm.lpFold=lpFold;
        }
      }

      //Check fold start
      if (hlp->fm.lpFold && (hlp->fm.lpFold->dwFlags & AEFOLDF_STYLED))
      {
        if (!(hlp->dwPaintType & AEHPT_FOLD) || lpColored)
        {
          if (to->nDrawCharOffset >= hlp->fm.crFoldStart.cpMin &&
              to->nDrawCharOffset < hlp->fm.crFoldEnd.cpMax)
          {
            if (!bLockHighLight)
            {
              //Draw text before fold
              AE_PaintTextOut(ae, to, hlp);
            }
            hlp->dwPaintType|=AEHPT_FOLD;
          }
        }

        if (!bLockHighLight && (hlp->dwPaintType & AEHPT_FOLD))
        {
          if (hlp->fm.lpFold->crText != (DWORD)-1)
            hlp->dwActiveText=hlp->fm.lpFold->crText;
          else
            hlp->dwActiveText=hlp->dwDefaultText;
          if (hlp->fm.lpFold->crBk != (DWORD)-1)
            hlp->dwActiveBk=hlp->fm.lpFold->crBk;
          else
            hlp->dwActiveBk=hlp->dwDefaultBk;
          //if (hlp->fm.lpFold->dwFontStyle != AEHLS_NONE)
            hlp->dwFontStyle=hlp->fm.lpFold->dwFontStyle;
        }
      }
    }
  }

  //if (ae->popt->lpActiveTheme)
  {
    //Only if char not in URL and not in color fold without ID
    if (!(hlp->dwPaintType & AEHPT_LINK) && !(hlp->fm.lpFold && (hlp->fm.lpFold->dwFlags & AEFOLDF_STYLED) && !hlp->fm.lpFold->nRuleID))
    {
      //Quote find
      if (hlp->dwFindFirst & AEHPT_QUOTE)
      {
        if (!AE_HighlightFindQuote(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, hlp))
        {
          hlp->qm.lpQuote=NULL;
          hlp->qm.crQuoteStart.ciMin=to->ciDrawLine;
          hlp->qm.crQuoteStart.ciMax=to->ciDrawLine;
          hlp->qm.crQuoteEnd.ciMin=to->ciDrawLine;
          hlp->qm.crQuoteEnd.ciMax=to->ciDrawLine;
          hlp->qm.nQuoteLen=0;
        }
        hlp->dwFindFirst&=~AEHPT_QUOTE;
        hlp->qm.ciFindFirst=to->ciDrawLine;
        hlp->qm.ciChildScan=hlp->qm.crQuoteStart.ciMax;
      }
      else if (AEC_IndexCompare(&hlp->qm.crQuoteEnd.ciMax, &to->ciDrawLine) <= 0)
      {
        if (AE_HighlightFindQuote(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, hlp))
        {
          hlp->qm.ciChildScan=hlp->qm.crQuoteStart.ciMax;
        }
      }

      if (hlp->qm.lpQuote && hlp->qm.lpQuote->nRuleID)
      {
        //Horizontal scroling when <...> is the parent for "...":
        //<a href="Long string...">
        AECHARINDEX ciDrawLine=to->ciDrawLine;
        DWORD dwSelFlag=(hlp->dwPaintType & AEHPT_SELECTION);
        DWORD dwColSelFlag=(to->dwPrintFlags & AEPRN_COLOREDSELECTION);
        AEQUOTEITEMW *lpQuotePrev;

        hlp->dwPaintType|=AEHPT_SELECTION;
        to->dwPrintFlags&=~AEPRN_COLOREDSELECTION;

        for (to->ciDrawLine=hlp->qm.ciChildScan; AEC_IndexCompare(&to->ciDrawLine, &ciDrawLine) <= 0; )
        {
          AE_PaintCheckHighlightCloseItem(ae, to, hlp);
          lpQuotePrev=hlp->qm.lpQuote;

          if (nFoundChild=AE_HighlightFindQuote(ae, &to->ciDrawLine, AEHF_FINDCHILD, hlp))
          {
            //Empty quote start. Avoid infinite recursion with nParentID == -3.
            if (!AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteStart.ciMax) && hlp->qm.lpQuote != lpQuotePrev)
              continue;
          }
          AEC_NextCharInLine(&to->ciDrawLine);
        }
        hlp->qm.ciChildScan=to->ciDrawLine;
        to->ciDrawLine=ciDrawLine;
        if (!dwSelFlag) hlp->dwPaintType&=~AEHPT_SELECTION;
        if (dwColSelFlag) to->dwPrintFlags|=AEPRN_COLOREDSELECTION;
      }

      //Check quote start
      if (hlp->qm.lpQuote)
      {
        if (!(hlp->dwPaintType & AEHPT_QUOTE) || nFoundChild)
        {
          if (AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteEnd.ciMax) < 0)
          {
            if ((!(hlp->qm.lpQuote->dwFlags & AEHLF_QUOTESTART_NOHIGHLIGHT) && AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteStart.ciMin) >= 0) ||
                ((hlp->qm.lpQuote->dwFlags & AEHLF_QUOTESTART_NOHIGHLIGHT) && AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteStart.ciMax) >= 0))
            {
              if (!bLockHighLight)
              {
                //Draw text before range for highlight
                AE_PaintTextOut(ae, to, hlp);
              }
              hlp->dwPaintType|=AEHPT_QUOTE;
            }
          }
        }
        if (!bLockHighLight && (hlp->dwPaintType & AEHPT_QUOTE) && AEC_IndexCompare(&hlp->qm.crQuoteStart.ciMin, &hlp->qm.crQuoteEnd.ciMax))
        {
          if (hlp->qm.lpQuote->dwFlags & AEHLF_REGEXP)
          {
            REGROUP *lpREGroup;
            REGROUP *lpREGroupEnd=NULL;
            AEREGROUPCOLOR *lpREGroupColor;
            wchar_t wszColor[12];
            wchar_t *wpColor=wszColor;
            INT_PTR nColorLen;

            lpREGroup=AE_PatCharInGroup((STACKREGROUP *)hlp->qm.lpQuote->lpREGroupStack, &to->ciDrawLine, &lpREGroupEnd);

            if ((lpREGroup && lpREGroup->dwUserData) || (lpREGroupEnd && lpREGroupEnd->dwUserData))
            {
              if (!bLockHighLight)
              {
                if ((lpREGroup && !AEC_IndexCompare(&to->ciDrawLine, &lpREGroup->ciStrStart)) || lpREGroupEnd)
                {
                  //Draw text before group color is started or closed
                  AE_PaintTextOut(ae, to, hlp);
                }
              }
              AE_SetDefaultStyle(hlp, hlp->qm.nParentType);

              if (lpREGroup && (lpREGroupColor=(AEREGROUPCOLOR *)lpREGroup->dwUserData))
              {
                if (lpREGroupColor->crText != (DWORD)-1)
                {
                  if (lpREGroupColor->dwFlags & AEREGCF_BACKREFCOLORTEXT)
                  {
                    if (lpREGroup=PatGetGroup((STACKREGROUP *)hlp->qm.lpQuote->lpREGroupStack, lpREGroupColor->crText))
                    {
                      nColorLen=AE_PatStrCopy(&lpREGroup->ciStrStart, &lpREGroup->ciStrEnd, wszColor, wszColor + 12);
                      if (*wpColor == L'#')
                      {
                        ++wpColor;
                        --nColorLen;
                      }
                      if (nColorLen == 6 || nColorLen == 3)
                        hlp->dwActiveText=AE_GetColorFromStr(wpColor, NULL);
                    }
                  }
                  else hlp->dwActiveText=lpREGroupColor->crText;
                }
                if (lpREGroupColor->crBk != (DWORD)-1)
                {
                  if (lpREGroupColor->dwFlags & AEREGCF_BACKREFCOLORBK)
                  {
                    if (lpREGroup=PatGetGroup((STACKREGROUP *)hlp->qm.lpQuote->lpREGroupStack, lpREGroupColor->crBk))
                    {
                      nColorLen=AE_PatStrCopy(&lpREGroup->ciStrStart, &lpREGroup->ciStrEnd, wszColor, wszColor + 12);
                      if (*wpColor == L'#')
                      {
                        ++wpColor;
                        --nColorLen;
                      }
                      if (nColorLen == 6 || nColorLen == 3)
                        hlp->dwActiveBk=AE_GetColorFromStr(wpColor, NULL);
                    }
                  }
                  else hlp->dwActiveBk=lpREGroupColor->crBk;
                }
                //if (lpREGroupColor->dwFontStyle != AEHLS_NONE)
                  hlp->dwFontStyle=lpREGroupColor->dwFontStyle;
              }
            }
            else AE_SetDefaultStyle(hlp, hlp->qm.nParentType);
          }
          else
          {
            AE_SetDefaultStyle(hlp, hlp->qm.nParentType);
            if (hlp->qm.lpQuote->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->qm.lpQuote->crText;
            if (hlp->qm.lpQuote->crBk != (DWORD)-1)
              hlp->dwActiveBk=hlp->qm.lpQuote->crBk;
            //if (hlp->qm.lpQuote->dwFontStyle != AEHLS_NONE)
              hlp->dwFontStyle=hlp->qm.lpQuote->dwFontStyle;
          }
          //Save active style to AEQUOTEMATCH
          hlp->qm.dwActiveText=hlp->dwActiveText;
          hlp->qm.dwActiveBk=hlp->dwActiveBk;
          hlp->qm.dwFontStyle=hlp->dwFontStyle;
        }
      }

      //Only if char not in quote
      if (!hlp->qm.lpQuote || !(hlp->qm.lpQuote->dwFlags & AEHLF_STYLED) || AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteStart.ciMin) < 0)
      {
        //Word find
        if (AEC_IndexCompare(&hlp->wm.crDelim2.ciMax, &to->ciDrawLine) <= 0)
        {
          if (hlp->dwFindFirst & AEHPT_WORD)
          {
            AE_HighlightFindWord(ae, &to->ciDrawLine, to->nDrawCharOffset, AEHF_FINDFIRSTCHAR, &hlp->wm, &hlp->qm, &hlp->fm);
            hlp->dwFindFirst&=~AEHPT_WORD;
          }
          else AE_HighlightFindWord(ae, &to->ciDrawLine, to->nDrawCharOffset, AEHF_ISFIRSTCHAR, &hlp->wm, &hlp->qm, &hlp->fm);
        }

        //Check delimiters and word start
        if (hlp->wm.lpDelim1)
        {
          if (!(hlp->dwPaintType & AEHPT_DELIM1))
          {
            if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim1.ciMin) >= 0 &&
                AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim1.ciMax) < 0)
            {
              if (!bLockHighLight)
              {
                //Draw text before range for highlight
                AE_PaintTextOut(ae, to, hlp);
              }
              hlp->dwPaintType|=AEHPT_DELIM1;
            }
          }
          if (!bLockHighLight && (hlp->dwPaintType & AEHPT_DELIM1))
          {
            if (hlp->wm.lpDelim1->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->wm.lpDelim1->crText;
            else
              hlp->dwActiveText=hlp->dwDefaultText;
            if (hlp->wm.lpDelim1->crBk != (DWORD)-1)
              hlp->dwActiveBk=hlp->wm.lpDelim1->crBk;
            else
              hlp->dwActiveBk=hlp->dwDefaultBk;
            //if (hlp->wm.lpDelim1->dwFontStyle != AEHLS_NONE)
              hlp->dwFontStyle=hlp->wm.lpDelim1->dwFontStyle;
          }
        }
        if (hlp->wm.lpWord)
        {
          if (!(hlp->dwPaintType & AEHPT_WORD))
          {
            if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crWord.ciMin) >= 0 &&
                AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crWord.ciMax) < 0)
            {
              if (!bLockHighLight)
              {
                //Draw text before range for highlight
                AE_PaintTextOut(ae, to, hlp);
              }
              hlp->dwPaintType|=AEHPT_WORD;
            }
          }
          if (!bLockHighLight && (hlp->dwPaintType & AEHPT_WORD))
          {
            if (hlp->wm.lpWord->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->wm.lpWord->crText;
            else
              hlp->dwActiveText=hlp->dwDefaultText;
            if (hlp->wm.lpWord->crBk != (DWORD)-1)
              hlp->dwActiveBk=hlp->wm.lpWord->crBk;
            else
              hlp->dwActiveBk=hlp->dwDefaultBk;
            //if (hlp->wm.lpWord->dwFontStyle != AEHLS_NONE)
              hlp->dwFontStyle=hlp->wm.lpWord->dwFontStyle;
          }
        }
        if (hlp->wm.lpDelim2)
        {
          if (!(hlp->dwPaintType & AEHPT_DELIM2))
          {
            if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim2.ciMin) >= 0 &&
                AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim2.ciMax) < 0)
            {
              if (!bLockHighLight)
              {
                //Draw text before range for highlight
                AE_PaintTextOut(ae, to, hlp);
              }
              hlp->dwPaintType|=AEHPT_DELIM2;
            }
          }
          if (!bLockHighLight && (hlp->dwPaintType & AEHPT_DELIM2))
          {
            if (hlp->wm.lpDelim2->crText != (DWORD)-1)
              hlp->dwActiveText=hlp->wm.lpDelim2->crText;
            else
              hlp->dwActiveText=hlp->dwDefaultText;
            if (hlp->wm.lpDelim2->crBk != (DWORD)-1)
              hlp->dwActiveBk=hlp->wm.lpDelim2->crBk;
            else
              hlp->dwActiveBk=hlp->dwDefaultBk;
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
        if (to->nDrawCharOffset >= hlp->mrm.crMarkRange.cpMin &&
            to->nDrawCharOffset < hlp->mrm.crMarkRange.cpMax)
        {
          if (!(hlp->dwPaintType & (AEHPT_SELECTION|AEHPT_MARKTEXT)))
          {
            //Draw text before mark
            AE_PaintTextOut(ae, to, hlp);
          }
          hlp->dwPaintType|=AEHPT_MARKRANGE;
        }
      }
      if (!(hlp->dwPaintType & (AEHPT_SELECTION|AEHPT_MARKTEXT)) && (hlp->dwPaintType & AEHPT_MARKRANGE))
      {
        if (hlp->mrm.lpMarkRange->crText != (DWORD)-1)
          hlp->dwActiveText=hlp->mrm.lpMarkRange->crText;
        else
          hlp->dwActiveText=hlp->dwDefaultText;
        if (hlp->mrm.lpMarkRange->crBk != (DWORD)-1)
          hlp->dwActiveBk=hlp->mrm.lpMarkRange->crBk;
        else
          hlp->dwActiveBk=hlp->dwDefaultBk;
        if (hlp->mrm.lpMarkRange->dwFontStyle != AEHLS_NONE)
          hlp->dwFontStyle=hlp->mrm.lpMarkRange->dwFontStyle;
      }
    }

    //Mark text find
    if (AEC_IndexCompare(&hlp->mtm.crMarkText.ciMax, &to->ciDrawLine) <= 0)
    {
      if (hlp->dwFindFirst & AEHPT_MARKTEXT)
      {
        if (!AE_HighlightFindMarkText(ae, &to->ciDrawLine, AEHF_FINDFIRSTCHAR, &hlp->mtm))
        {
          hlp->mtm.lpMarkText=NULL;
          hlp->mtm.crMarkText.ciMin=to->ciDrawLine;
          hlp->mtm.crMarkText.ciMax=to->ciDrawLine;
        }
        hlp->dwFindFirst&=~AEHPT_MARKTEXT;
      }
      else AE_HighlightFindMarkText(ae, &to->ciDrawLine, AEHF_ISFIRSTCHAR, &hlp->mtm);
    }

    //Check mark text start
    if (hlp->mtm.lpMarkText)
    {
      if (!(hlp->dwPaintType & AEHPT_MARKTEXT))
      {
        if (AEC_IndexCompare(&to->ciDrawLine, &hlp->mtm.crMarkText.ciMin) >= 0 &&
            AEC_IndexCompare(&to->ciDrawLine, &hlp->mtm.crMarkText.ciMax) < 0)
        {
          if (!(hlp->dwPaintType & AEHPT_SELECTION))
          {
            //Draw text before mark
            AE_PaintTextOut(ae, to, hlp);
          }
          hlp->dwPaintType|=AEHPT_MARKTEXT;
        }
      }
      if (!(hlp->dwPaintType & AEHPT_SELECTION) && (hlp->dwPaintType & AEHPT_MARKTEXT))
      {
        if (hlp->mtm.lpMarkText->crText != (DWORD)-1)
          hlp->dwActiveText=hlp->mtm.lpMarkText->crText;
        else
          hlp->dwActiveText=hlp->dwDefaultText;
        if (hlp->mtm.lpMarkText->crBk != (DWORD)-1)
          hlp->dwActiveBk=hlp->mtm.lpMarkText->crBk;
        else
          hlp->dwActiveBk=hlp->dwDefaultBk;
        if (hlp->mtm.lpMarkText->dwFontStyle != AEHLS_NONE)
          hlp->dwFontStyle=hlp->mtm.lpMarkText->dwFontStyle;
      }
    }
  }
  if (!hlp->dwPaintType)
  {
    hlp->dwActiveText=hlp->dwDefaultText;
    hlp->dwActiveBk=hlp->dwDefaultBk;
    hlp->dwFontStyle=AEHLS_NONE;
  }
}

void AE_PaintCheckHighlightCloseItem(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp)
{
  if (to->dwPrintFlags & AEPRN_COLOREDSELECTION)
  {
    if (hlp->dwPaintType & AEHPT_SELECTION)
    {
      if (to->ciDrawLine.nCharInLine >= to->ciDrawLine.lpLine->nSelEnd)
      {
        if (to->ciDrawLine.nCharInLine > to->ciDrawLine.lpLine->nSelStart &&
            to->ciDrawLine.nCharInLine == to->ciDrawLine.lpLine->nSelEnd)
        {
          //Draw text up to selection end
          AE_PaintTextOut(ae, to, hlp);
        }
        hlp->dwActiveText=hlp->dwDefaultText;
        hlp->dwActiveBk=hlp->dwDefaultBk;
        hlp->dwPaintType&=~AEHPT_SELECTION;
        hlp->dwFontStyle=AEHLS_NONE;
      }
    }
  }
  if (ae->popt->dwOptionsEx & AECOE_DETECTURL)
  {
    if (hlp->crLink.ciMin.lpLine && hlp->crLink.ciMax.lpLine)
    {
      if (AEC_IndexCompare(&to->ciDrawLine, &hlp->crLink.ciMax) >= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AEC_IndexCompare(&to->ciDrawLine, &hlp->crLink.ciMax) == 0)
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
  if (hlp->fm.lpFold)
  {
    if (to->nDrawCharOffset >= hlp->fm.crFoldEnd.cpMax ||
        (hlp->fm.lpFold->hRuleTheme && to->nDrawCharOffset >= hlp->fm.crFoldEnd.cpMin))
    {
      AEFOLD *lpFold=hlp->fm.lpFold;

      if (!(hlp->dwPaintType & AEHPT_SELECTION))
      {
        if (to->nDrawCharOffset == hlp->fm.crFoldEnd.cpMax ||
            (lpFold->hRuleTheme && to->nDrawCharOffset == hlp->fm.crFoldEnd.cpMin))
        {
          //Draw full highlighted text or last part of it
          AE_PaintTextOut(ae, to, hlp);
        }
      }
      hlp->dwPaintType&=~AEHPT_FOLD;
      hlp->fm.lpFold=NULL;

      //Return to main theme
      if (lpFold->hRuleTheme && (AEHTHEME)ae->popt->lpActiveTheme == lpFold->hRuleTheme)
      {
        ae->popt->lpActiveTheme=(AETHEMEITEMW *)(hlp->fm.hParentStack.last?hlp->fm.hParentStack.last->hActiveTheme:NULL);
        AE_HeapStackDelete(ae, (stack **)&hlp->fm.hParentStack.first, (stack **)&hlp->fm.hParentStack.last, (stack *)hlp->fm.hParentStack.last);
        AE_PaintCheckHighlightReset(ae, to, hlp, &lpFold->lpMaxPoint->ciPoint);
      }
    }
  }
  //if (ae->popt->lpActiveTheme)
  {
    if (hlp->mtm.lpMarkText)
    {
      if (AEC_IndexCompare(&to->ciDrawLine, &hlp->mtm.crMarkText.ciMax) >= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AEC_IndexCompare(&to->ciDrawLine, &hlp->mtm.crMarkText.ciMax) == 0)
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
      if (to->nDrawCharOffset >= hlp->mrm.crMarkRange.cpMax)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (to->nDrawCharOffset == hlp->mrm.crMarkRange.cpMax)
          {
            //Draw full highlighted text or last part of it
            AE_PaintTextOut(ae, to, hlp);
          }
        }
        hlp->dwPaintType&=~AEHPT_MARKRANGE;
        hlp->mrm.lpMarkRange=NULL;
      }
    }
    while (hlp->qm.lpQuote)
    {
      if (((hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteEnd.ciMin) >= 0) ||
          (!(hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteEnd.ciMax) >= 0))
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (((hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteEnd.ciMin) == 0) ||
              (!(hlp->qm.lpQuote->dwFlags & AEHLF_QUOTEEND_NOHIGHLIGHT) && AEC_IndexCompare(&to->ciDrawLine, &hlp->qm.crQuoteEnd.ciMax) == 0))
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText)
            {
              AE_PaintTextOut(ae, to, hlp);
            }
          }
        }
        if (hlp->qm.lpQuote->dwFlags & AEHLF_REGEXP)
          AE_PatReset((STACKREGROUP *)hlp->qm.lpQuote->lpREGroupStack);

        if (hlp->qm.hParentStack.last)
        {
          AEQUOTEMATCHITEM *lpQuoteMatchItem=hlp->qm.hParentStack.last;

          hlp->qm.ciChildScan=hlp->qm.crQuoteEnd.ciMax;
          hlp->qm.lpQuote=lpQuoteMatchItem->lpQuote;
          hlp->qm.crQuoteStart.ciMin=lpQuoteMatchItem->crQuoteStart.ciMin;
          hlp->qm.crQuoteStart.ciMax=lpQuoteMatchItem->crQuoteStart.ciMax;
          hlp->qm.crQuoteEnd.ciMin=lpQuoteMatchItem->crQuoteEnd.ciMin;
          hlp->qm.crQuoteEnd.ciMax=lpQuoteMatchItem->crQuoteEnd.ciMax;
          hlp->qm.nQuoteLen=lpQuoteMatchItem->nQuoteLen;
          hlp->qm.dwActiveText=lpQuoteMatchItem->dwActiveText;
          hlp->qm.dwActiveBk=lpQuoteMatchItem->dwActiveBk;
          hlp->qm.dwFontStyle=lpQuoteMatchItem->dwFontStyle;

          AE_HeapStackDelete(ae, (stack **)&hlp->qm.hParentStack.first, (stack **)&hlp->qm.hParentStack.last, (stack *)hlp->qm.hParentStack.last);
          continue;
        }
        hlp->dwPaintType&=~AEHPT_QUOTE;
        hlp->qm.lpQuote=NULL;
      }
      break;
    }
    if (hlp->wm.lpDelim1)
    {
      if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim1.ciMax) >= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim1.ciMax) == 0)
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText && !hlp->crLink.ciMin.lpLine)
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
      if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crWord.ciMax) >= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crWord.ciMax) == 0)
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText && !hlp->crLink.ciMin.lpLine)
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
      if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim2.ciMax) >= 0)
      {
        if (!(hlp->dwPaintType & AEHPT_SELECTION))
        {
          if (AEC_IndexCompare(&to->ciDrawLine, &hlp->wm.crDelim2.ciMax) == 0)
          {
            //Draw full highlighted text or last part of it
            if (!hlp->mtm.lpMarkText && !hlp->crLink.ciMin.lpLine)
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

  //Activate root fold with own theme
  if (hlp->fm.lpFold && hlp->fm.lpFold->hRuleTheme && to->nDrawCharOffset >= hlp->fm.crFoldStart.cpMax)
  {
    if ((AEHTHEME)ae->popt->lpActiveTheme != hlp->fm.lpFold->hRuleTheme)
    {
      AEACTIVETHEMEITEM *lpActiveThemeItem;

      if (!AE_HeapStackInsertBefore(ae, (stack **)&hlp->fm.hParentStack.first, (stack **)&hlp->fm.hParentStack.last, NULL, (stack **)&lpActiveThemeItem, sizeof(AEACTIVETHEMEITEM)))
        lpActiveThemeItem->hActiveTheme=(AEHTHEME)ae->popt->lpActiveTheme;
      ae->popt->lpActiveTheme=(AETHEMEITEMW *)hlp->fm.lpFold->hRuleTheme;
    }
  }
}

void AE_PaintCheckHighlightCleanUp(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, AECHARINDEX *ciChar)
{
  if (to->dwPrintFlags & AEPRN_COLOREDSELECTION)
  {
    if (hlp->dwPaintType & AEHPT_SELECTION)
    {
      if (!AEC_IsCharInSelection(ciChar))
      {
        hlp->dwPaintType&=~AEHPT_SELECTION;
        hlp->dwFontStyle=AEHLS_NONE;
      }
    }
  }
  //if (ae->popt->lpActiveTheme)
  {
    if (hlp->wm.lpDelim1)
    {
      if (AEC_IndexCompare(ciChar, &hlp->wm.crDelim1.ciMax) > 0)
      {
        hlp->dwPaintType&=~AEHPT_DELIM1;
        hlp->wm.lpDelim1=NULL;
      }
    }
    if (hlp->wm.lpWord)
    {
      if (AEC_IndexCompare(ciChar, &hlp->wm.crWord.ciMax) > 0)
      {
        hlp->dwPaintType&=~AEHPT_WORD;
        hlp->wm.lpWord=NULL;
      }
    }
    if (hlp->wm.lpDelim2)
    {
      if (AEC_IndexCompare(ciChar, &hlp->wm.crDelim2.ciMax) > 0)
      {
        hlp->dwPaintType&=~AEHPT_DELIM2;
        hlp->wm.lpDelim2=NULL;
      }
    }
    if (hlp->qm.lpQuote)
    {
      if (AEC_IndexCompare(ciChar, &hlp->qm.crQuoteEnd.ciMax) > 0)
      {
        hlp->dwPaintType&=~AEHPT_QUOTE;
        hlp->qm.lpQuote=NULL;
      }
    }
    if (hlp->mrm.lpMarkRange)
    {
      if (to->nDrawCharOffset > hlp->mrm.crMarkRange.cpMax)
      {
        hlp->dwPaintType&=~AEHPT_MARKRANGE;
        hlp->mrm.lpMarkRange=NULL;
      }
    }
    if (hlp->mtm.lpMarkText)
    {
      if (AEC_IndexCompare(ciChar, &hlp->mtm.crMarkText.ciMax) > 0)
      {
        hlp->dwPaintType&=~AEHPT_MARKTEXT;
        hlp->mtm.lpMarkText=NULL;
      }
    }
  }
  if (hlp->fm.lpFold)
  {
    if (to->nDrawCharOffset > hlp->fm.crFoldEnd.cpMax ||
        (hlp->fm.lpFold->hRuleTheme && to->nDrawCharOffset > hlp->fm.crFoldEnd.cpMin))
    {
      AEFOLD *lpFold=hlp->fm.lpFold;

      hlp->dwPaintType&=~AEHPT_FOLD;
      hlp->fm.lpFold=NULL;

      //Return to main theme
      if (lpFold->hRuleTheme && (AEHTHEME)ae->popt->lpActiveTheme == lpFold->hRuleTheme)
      {
        ae->popt->lpActiveTheme=(AETHEMEITEMW *)(hlp->fm.hParentStack.last?hlp->fm.hParentStack.last->hActiveTheme:NULL);
        AE_HeapStackDelete(ae, (stack **)&hlp->fm.hParentStack.first, (stack **)&hlp->fm.hParentStack.last, (stack *)hlp->fm.hParentStack.last);
        AE_PaintCheckHighlightReset(ae, to, hlp, &lpFold->lpMaxPoint->ciPoint);
      }
    }
  }
  if (ae->popt->dwOptionsEx & AECOE_DETECTURL)
  {
    if (hlp->crLink.ciMin.lpLine && hlp->crLink.ciMax.lpLine)
    {
      if (AEC_IndexCompare(ciChar, &hlp->crLink.ciMax) > 0)
      {
        hlp->dwPaintType&=~AEHPT_LINK;
        hlp->crLink.ciMin.lpLine=NULL;
        hlp->crLink.ciMax.lpLine=NULL;
      }
    }
  }
}

void AE_PaintCheckHighlightReset(AKELEDIT *ae, AETEXTOUT *to, AEHLPAINT *hlp, AECHARINDEX *ciChar)
{
  AECHARINDEX ciReset=*ciChar;

  //Reset delimiters, word, quote when theme is changed.
  if (AEC_IndexCompare(&ciReset, &hlp->wm.crDelim1.ciMin) < 0)
    hlp->wm.crDelim1.ciMin=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->wm.crDelim1.ciMax) < 0)
    hlp->wm.crDelim1.ciMax=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->wm.crWord.ciMin) < 0)
    hlp->wm.crWord.ciMin=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->wm.crWord.ciMax) < 0)
    hlp->wm.crWord.ciMax=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->wm.crDelim2.ciMin) < 0)
    hlp->wm.crDelim2.ciMin=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->wm.crDelim2.ciMax) < 0)
    hlp->wm.crDelim2.ciMax=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->qm.crQuoteStart.ciMin) < 0)
    hlp->qm.crQuoteStart.ciMin=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->qm.crQuoteStart.ciMax) < 0)
    hlp->qm.crQuoteStart.ciMax=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->qm.crQuoteEnd.ciMin) < 0)
    hlp->qm.crQuoteEnd.ciMin=ciReset;
  if (AEC_IndexCompare(&ciReset, &hlp->qm.crQuoteEnd.ciMax) < 0)
    hlp->qm.crQuoteEnd.ciMax=ciReset;
  AEC_IndexInc(&ciReset);
  AE_PaintCheckHighlightCleanUp(ae, to, hlp, &ciReset);
}

int AE_HighlightAllowed(AEQUOTEITEMW *lpQuote, AEFOLDMATCH *fm, int nParentID, int nQuoteRuleID, const AECHARINDEX *ciChar)
{
  if (lpQuote)
  {
    if (nParentID && lpQuote->nRuleID)
    {
      if (nParentID == lpQuote->nRuleID)
        return AEHAP_QUOTE;
      if (nParentID == -1)
        return AEHAP_NONE;
      if (nParentID == -2 || nParentID == -3)
      {
        if (nQuoteRuleID == lpQuote->nRuleID)
          return AEHAP_NONE;
        return AEHAP_QUOTE;
      }
    }
    if (lpQuote->dwFlags & AEHLF_STYLED)
      return AEHAP_NONE;
  }
  if (fm && fm->lpFold)
  {
    int nFoldRuleID;

    if (fm->lpFold->hRuleTheme == (AEHTHEME)((AKELEDIT *)fm->hDoc)->popt->lpActiveTheme)
      nFoldRuleID=0;
    else
      nFoldRuleID=fm->lpFold->nRuleID;

    if (nParentID && nFoldRuleID)
    {
      if (nParentID == nFoldRuleID)
        return AEHAP_FOLD;
      if (nParentID == -1)
        return AEHAP_NONE;
      if (nParentID == -2 || nParentID == -3)
        return AEHAP_FOLD;
    }
    if (fm->lpFold->dwFlags & AEFOLDF_STYLED)
    {
      if (!fm->lpFold->hRuleTheme)
        return AEHAP_NONE;

      //Beginning and ending of fold with own theme is outside of this theme.
      if (AEC_IndexCompare(ciChar, &fm->lpFold->lpMaxPoint->ciPoint) < 0)
      {
        if (fm->nFoldStartMax != fm->lpFold->lpMinPoint->nPointOffset + fm->lpFold->lpMinPoint->nPointLen)
        {
          AE_IndexOffset(NULL, &fm->lpFold->lpMinPoint->ciPoint, &fm->ciFoldStartMax, fm->lpFold->lpMinPoint->nPointLen, AELB_R);
          fm->nFoldStartMax=fm->lpFold->lpMinPoint->nPointOffset + fm->lpFold->lpMinPoint->nPointLen;
        }
        if (AEC_IndexCompare(ciChar, &fm->ciFoldStartMax) >= 0)
          return AEHAP_NONE;
      }
    }
  }
  if (nParentID > 0 || nParentID == -2)
    return AEHAP_NONE;
  return AEHAP_ROOT;
}

void AE_SetDefaultStyle(AEHLPAINT *hlp, int nParentType)
{
  if (nParentType == AEHAP_QUOTE)
  {
    AEQUOTEMATCHITEM *lpQuoteMatchItem=(AEQUOTEMATCHITEM *)hlp->qm.hParentStack.last;

    if (lpQuoteMatchItem)
    {
      if (lpQuoteMatchItem->dwActiveText != (DWORD)-1)
        hlp->dwActiveText=lpQuoteMatchItem->dwActiveText;
      else
        hlp->dwActiveText=hlp->dwDefaultText;
      if (lpQuoteMatchItem->dwActiveBk != (DWORD)-1)
        hlp->dwActiveBk=lpQuoteMatchItem->dwActiveBk;
      else
        hlp->dwActiveBk=hlp->dwDefaultBk;
      hlp->dwFontStyle=lpQuoteMatchItem->dwFontStyle;
      return;
    }
  }
  hlp->dwActiveText=hlp->dwDefaultText;
  hlp->dwActiveBk=hlp->dwDefaultBk;
  //hlp->dwFontStyle=AEHLS_NONE;
}

void AE_GetHighLight(AKELEDIT *ae, AEGETHIGHLIGHT *gh)
{
  AEHLPAINT hlp;
  AETEXTOUT to;
  AECHARCOLORS aecc;

  gh->dwError=0;
  if (!gh->crText.ciMin.lpLine || !gh->crText.ciMax.lpLine || !gh->lpCallback)
    return;

  //Set AETEXTOUT
  xmemset(&to, 0, sizeof(AETEXTOUT));
  to.ciDrawLine=gh->crText.ciMin;
  to.nDrawCharOffset=AE_AkelIndexToRichOffset(ae, &gh->crText.ciMin);
  to.wpStartDraw=to.ciDrawLine.lpLine->wpLine + to.ciDrawLine.nCharInLine;
  to.dwPrintFlags=AEPRN_TEST|(gh->dwFlags & AEGHF_NOSELECTION?0:AEPRN_COLOREDSELECTION);
  to.gh=gh;

  //Set AEHLPAINT
  xmemset(&hlp, 0, sizeof(AEHLPAINT));
  hlp.cb=sizeof(AEHLPAINT);
  hlp.fm.hDoc=(AEHDOC)ae;
  hlp.fm.hActiveThemeBegin=(AEHTHEME)ae->popt->lpActiveTheme;

  //Set AECHARCOLORS flags
  aecc.dwFlags=gh->dwFlags;

  while (to.ciDrawLine.lpLine)
  {
    //Close all previous items
    AE_PaintCheckHighlightCleanUp(ae, &to, &hlp, &to.ciDrawLine);
    hlp.dwFindFirst=AEHPT_MARKTEXT|AEHPT_LINK|AEHPT_QUOTE|AEHPT_WORD;
    hlp.dwFindFirst&=~hlp.dwPaintType;

    //Default colors
    AE_GetBasicCharColors(ae, &to.ciDrawLine, &aecc);
    hlp.dwDefaultText=aecc.crText;
    hlp.dwDefaultBk=aecc.crBk;
    hlp.dwActiveText=hlp.dwDefaultText;
    hlp.dwActiveBk=hlp.dwDefaultBk;
    hlp.dwFontStyle=AEHLS_NONE;

    while (to.ciDrawLine.nCharInLine <= to.ciDrawLine.lpLine->nLineLen)
    {
      //Check highlight close
      AE_PaintCheckHighlightCloseItem(ae, &to, &hlp);
      if (to.gh->dwError) goto End;

      //Check highlight open
      AE_PaintCheckHighlightOpenItem(ae, &to, &hlp, ae->ptxt->nLineCount);
      if (to.gh->dwError) goto End;

      if (AEC_IndexCompare(&to.ciDrawLine, &gh->crText.ciMax) >= 0)
        goto LastPaint;
      if (to.ciDrawLine.nCharInLine == to.ciDrawLine.lpLine->nLineLen) break;

      //Increment char count
      to.nDrawCharOffset+=AEC_IndexInc(&to.ciDrawLine);
    }
    to.dwPrintFlags|=(gh->dwFlags & AEGHF_CALLENDLINE?AEPRN_CALLEMPTY:0);
    AE_PaintTextOut(ae, &to, &hlp);
    to.dwPrintFlags&=~AEPRN_CALLEMPTY;
    if (to.gh->dwError) goto End;

    //Next line
    if (to.ciDrawLine.lpLine->nLineBreak != AELB_WRAP)
      ++to.nDrawCharOffset;
    AEC_NextLine(&to.ciDrawLine);
    to.wpStartDraw=to.ciDrawLine.lpLine->wpLine;
  }

  LastPaint:
  to.dwPrintFlags|=(gh->dwFlags & AEGHF_CALLENDRANGE?AEPRN_CALLEMPTY:0);
  AE_PaintTextOut(ae, &to, &hlp);
  to.dwPrintFlags&=~AEPRN_CALLEMPTY;
  //if (to.gh->dwError) goto End;

  End:
  ae->popt->lpActiveTheme=(AETHEMEITEMW *)hlp.fm.hActiveThemeBegin;
  AE_HeapStackClear(ae, (stack **)&hlp.qm.hParentStack.first, (stack **)&hlp.qm.hParentStack.last);
  AE_HeapStackClear(ae, (stack **)&hlp.fm.hParentStack.first, (stack **)&hlp.fm.hParentStack.last);
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

      rcMButtonDown.left=ae->ptMButtonDown.x + (int)(ae->ptMButtonScroll.x - ae->nHScrollPos) - 11;
      rcMButtonDown.top=ae->ptMButtonDown.y + (int)(ae->ptMButtonScroll.y - ae->nVScrollPos) - 11;
      rcMButtonDown.right=ae->ptMButtonDown.x + (int)(ae->ptMButtonScroll.x - ae->nHScrollPos) + 11;
      rcMButtonDown.bottom=ae->ptMButtonDown.y + (int)(ae->ptMButtonScroll.y - ae->nVScrollPos) + 11;

      InvalidateRect(ae->hWndEdit, &rcMButtonDown, TRUE);

      ae->bMButtonBitmapDraw=FALSE;
    }
  }
}

void AE_ActiveColumnDraw(AKELEDIT *ae, HDC hDC, int nTop, int nBottom)
{
  if (ae->ptActiveColumnDraw.x >= ae->rcDraw.left && ae->ptActiveColumnDraw.x <= ae->rcDraw.right)
  {
    HDC hInputDC=hDC;
    HPEN hPenOld;
    int nModeOld;
    int i;

    if (hDC || (hDC=GetDC(ae->hWndEdit)))
    {
      hPenOld=(HPEN)SelectObject(hDC, ae->popt->hActiveColumnPen);
      nModeOld=SetROP2(hDC, R2_XORPEN);

      nTop=max(ae->rcDraw.top, nTop);
      nBottom=min(ae->rcDraw.bottom, nBottom);
      nTop+=(int)((ae->nVScrollPos + (nTop - ae->rcDraw.top)) % 2);

      for (i=nTop; i < nBottom; i+=2)
      {
        if (i >= ae->ptActiveColumnDraw.y && i < ae->ptActiveColumnDraw.y + ae->ptxt->nCharHeight)
        {
          //Skip caret height
          i=ae->ptActiveColumnDraw.y + ae->ptxt->nCharHeight;
          i+=(int)((ae->nVScrollPos + (i - ae->rcDraw.top)) % 2);
          if (i >= nBottom) break;
        }

        //Draw dot
        MoveToEx(hDC, ae->ptActiveColumnDraw.x, i, NULL);
        LineTo(hDC, ae->ptActiveColumnDraw.x + 1, i + 1);
      }

      SetROP2(hDC, nModeOld);
      if (hPenOld) SelectObject(hDC, hPenOld);
      if (!hInputDC) ReleaseDC(ae->hWndEdit, hDC);
    }
  }
}

BOOL AE_ColumnMarkerSet(AKELEDIT *ae, DWORD dwType, int nPos, BOOL bMouse)
{
  if (nPos >= 0)
  {
    if (ae->popt->dwColumnMarkerType != dwType ||
        ae->popt->dwColumnMarkerPos != (DWORD)nPos)
    {
      HDC hDC=ae->hDC;

      AE_ColumnMarkerErase(ae);
      ae->popt->dwColumnMarkerType=dwType;
      ae->popt->dwColumnMarkerPos=nPos;

      if (hDC || (hDC=GetDC(ae->hWndEdit)))
      {
        AE_ColumnMarkerDraw(ae, hDC, ae->rcDraw.top, ae->rcDraw.bottom);
        if (!ae->hDC) ReleaseDC(ae->hWndEdit, hDC);
      }
      AE_StackCloneUpdate(ae);
      AE_NotifyMarker(ae, bMouse);
      return TRUE;
    }
  }
  return FALSE;
}

void AE_ColumnMarkerDraw(AKELEDIT *ae, HDC hDC, int nTop, int nBottom)
{
  if (ae->popt->dwColumnMarkerPos && nTop < nBottom)
  {
    HPEN hPenOld;
    INT_PTR nMarkerPos;

    nMarkerPos=ae->popt->dwColumnMarkerPos;
    if (ae->popt->dwColumnMarkerType == AEMT_SYMBOL)
      nMarkerPos*=ae->ptxt->nAveCharWidth;

    if (ae->nHScrollPos < nMarkerPos && nMarkerPos < ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left))
    {
      hPenOld=(HPEN)SelectObject(hDC, ae->popt->hColumnMarkerPen);
      MoveToEx(hDC, ae->rcDraw.left + (int)(nMarkerPos - ae->nHScrollPos), nTop, NULL);
      LineTo(hDC, ae->rcDraw.left + (int)(nMarkerPos - ae->nHScrollPos), nBottom);
      if (hPenOld) SelectObject(hDC, hPenOld);
    }
  }
}

void AE_ColumnMarkerErase(AKELEDIT *ae)
{
  if (ae->popt->dwColumnMarkerPos)
  {
    RECT rcMarker;
    INT_PTR nMarkerPos;

    nMarkerPos=ae->popt->dwColumnMarkerPos;
    if (ae->popt->dwColumnMarkerType == AEMT_SYMBOL)
      nMarkerPos*=ae->ptxt->nAveCharWidth;

    if (ae->nHScrollPos < nMarkerPos && nMarkerPos < ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left))
    {
      rcMarker.left=ae->rcDraw.left + (int)(nMarkerPos - ae->nHScrollPos);
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
  int nDrawWidth;

  GetClientRect(ae->hWndEdit, &rcClient);

  if (rcClient.right && rcClient.bottom)
  {
    if (rcClient.right != ae->rcEdit.right || rcClient.bottom != ae->rcEdit.bottom)
    {
      nDrawWidth=ae->rcDraw.right - ae->rcDraw.left;
      ae->rcDraw.right+=rcClient.right - ae->rcEdit.right;
      ae->rcDraw.bottom+=rcClient.bottom - ae->rcEdit.bottom;
      ae->rcErase.right+=rcClient.right - ae->rcEdit.right;
      ae->rcErase.bottom+=rcClient.bottom - ae->rcEdit.bottom;
      ae->rcEdit.right=rcClient.right;
      ae->rcEdit.bottom=rcClient.bottom;

      AE_SetDrawRect(ae, &ae->rcDraw, FALSE);
      if (ae->ptxt->dwWordWrap)
      {
        if (nDrawWidth != ae->rcDraw.right - ae->rcDraw.left)
        {
          AE_UpdateWrap(ae, NULL, NULL, ae->ptxt->dwWordWrap);
          AE_StackCloneUpdate(ae);
        }
      }
      AE_UpdateScrollBars(ae, SB_BOTH);
      AE_UpdateEditWindow(ae->hWndEdit, TRUE);
    }
  }
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
    rcRedraw.top=(int)(AE_VPosFromLine(ae, nFirstLine) - ae->nVScrollPos) + ae->rcDraw.top;
  if (nLastLine == -1)
    rcRedraw.bottom=ae->rcDraw.bottom;
  else
    rcRedraw.bottom=(int)(AE_VPosFromLine(ae, nLastLine) + ae->ptxt->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;

  rcRedraw.top=max(rcRedraw.top, ae->rcDraw.top);
  rcRedraw.bottom=min(rcRedraw.bottom, ae->rcDraw.bottom);

  InvalidateRect(ae->hWndEdit, &rcRedraw, bErase);
}

void AE_RedrawIndexRange(AKELEDIT *ae, const AECHARINDEX *ciFirstChar, const AECHARINDEX *ciLastChar, BOOL bErase)
{
  AELINEDATA *lpLine;
  int nFirstRedrawLine;
  int nLastRedrawLine;
  int nLinesInPage=(ae->rcDraw.bottom - ae->rcDraw.top) / ae->ptxt->nCharHeight;
  int nMinRedrawLine;
  int nMaxRedrawLine;

  if (ciFirstChar)
  {
    nFirstRedrawLine=ciFirstChar->nLine;
    if (ae->ptxt->dwWordWrap)
    {
      nMinRedrawLine=nFirstRedrawLine - nLinesInPage;
      for (lpLine=ciFirstChar->lpLine->prev; lpLine; lpLine=lpLine->prev)
      {
        if (lpLine->nLineBreak == AELB_WRAP)
          if (--nFirstRedrawLine < nMinRedrawLine)
            break;
      }
    }
  }
  else nFirstRedrawLine=-1;

  if (ciLastChar)
  {
    nLastRedrawLine=ciLastChar->nLine;
    if (ae->ptxt->dwWordWrap)
    {
      nMaxRedrawLine=nLastRedrawLine + nLinesInPage;
      for (lpLine=ciLastChar->lpLine; lpLine; lpLine=lpLine->next)
      {
        if (lpLine->nLineBreak == AELB_WRAP)
          if (++nLastRedrawLine > nMaxRedrawLine)
            break;
      }
    }
  }
  else nLastRedrawLine=-1;

  AE_RedrawLineRange(ae, nFirstRedrawLine, nLastRedrawLine, bErase);
}

void AE_HideSelection(AKELEDIT *ae, BOOL bHide)
{
  ae->popt->bHideSelection=bHide;
  AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
  //UpdateWindow(ae->hWndEdit);
}

int AE_GetFirstVisibleLine(AKELEDIT *ae)
{
  int nFirstLine;

  nFirstLine=AE_LineFromVPos(ae, ae->nVScrollPos);

  return max(nFirstLine, 0);
}

int AE_GetLastVisibleLine(AKELEDIT *ae)
{
  int nLastLine;

  nLastLine=AE_LineFromVPos(ae, ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top));

  return min(nLastLine, ae->ptxt->nLineCount);
}

int AE_GetFirstFullVisibleLine(AKELEDIT *ae)
{
  INT_PTR nVertPos;
  int nFirstLine;

  nVertPos=ae->nVScrollPos / ae->ptxt->nCharHeight * ae->ptxt->nCharHeight;
  if (nVertPos < ae->nVScrollPos) nVertPos+=ae->ptxt->nCharHeight;
  nFirstLine=AE_LineFromVPos(ae, nVertPos);

  return max(nFirstLine, 0);
}

int AE_GetLastFullVisibleLine(AKELEDIT *ae)
{
  INT_PTR nVertPos;
  int nLastLine;

  nVertPos=(ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top)) / ae->ptxt->nCharHeight * ae->ptxt->nCharHeight;
  nVertPos-=ae->ptxt->nCharHeight;
  nLastLine=AE_LineFromVPos(ae, nVertPos);

  return min(nLastLine, ae->ptxt->nLineCount);
}

int AE_GetTextExtentPoint32(AKELEDIT *ae, const wchar_t *wpString, int nStringLen, SIZE64 *lpSize)
{
  HDC hDC=ae->hDC;
  SIZE sizeChar;
  INT_PTR nStringWidth=0;
  int i;
  int nResult=0;
  HFONT hFontOld=NULL;

  for (i=0; i < nStringLen; ++i)
  {
    if (ae->ptxt->lpCharWidths[wpString[i]])
    {
      if (ae->ptxt->nFixedCharWidth)
        nStringWidth+=ae->ptxt->nFixedCharWidth;
      else
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
        if (wpString[i] != L'\t')
        {
          //Surrogate character
          if (AE_IsSurrogate(wpString[i]))
          {
            if (i + 1 < nStringLen)
            {
              if (AE_IsHighSurrogate(wpString[i]) && AE_IsLowSurrogate(wpString[i + 1]))
              {
                if (ae->ptxt->nFixedCharWidth)
                  nStringWidth+=ae->ptxt->nFixedCharWidth;
                else
                {
                  GetTextExtentPoint32W(hDC, wpString + i, 2, &sizeChar);
                  nStringWidth+=sizeChar.cx;
                }
                ++i;
                ++nResult;
              }
            }
          }
          else
          {
            GetTextExtentPoint32W(hDC, &wpString[i], 1, &sizeChar);
            ae->ptxt->lpCharWidths[wpString[i]]=(WORD)sizeChar.cx;
            if (ae->ptxt->nFixedCharWidth)
              nStringWidth+=ae->ptxt->nFixedCharWidth;
            else
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

int AE_GetCharWidth(AKELEDIT *ae, wchar_t *wpChar, INT_PTR nCharExtent)
{
  SIZE64 sizeChar;
  int nCharLen;

  if (*wpChar == L'\t')
  {
    if (ae->ptxt->nFixedCharWidth)
      return (int)(ae->ptxt->nFixedTabWidth - nCharExtent % ae->ptxt->nFixedTabWidth);
    return (int)(ae->ptxt->nTabWidth - nCharExtent % ae->ptxt->nTabWidth);
  }

  if (AE_IsHighSurrogate(*wpChar))
  {
    if (AE_IsLowSurrogate(*(wpChar + 1)))
      nCharLen=2;
    else
      nCharLen=0;
  }
  else nCharLen=1;

  if (nCharLen)
  {
    if (AE_GetTextExtentPoint32(ae, wpChar, nCharLen, &sizeChar))
      return (int)sizeChar.cx;
  }
  return 0;
}

INT_PTR AE_GetStringWidth(AKELEDIT *ae, wchar_t *wpString, int nStringLen, INT_PTR nFirstCharExtent)
{
  SIZE64 sizeChar;
  INT_PTR nStringWidth=nFirstCharExtent;
  int nStringCount=0;
  int nTabWidth;
  int i;

  for (i=0; i < nStringLen; ++i)
  {
    if (wpString[i] == L'\t')
    {
      //Width before tabulation
      if (AE_GetTextExtentPoint32(ae, wpString + nStringCount, i - nStringCount, &sizeChar))
        nStringWidth+=sizeChar.cx;
      nStringCount=i + 1;

      if (ae->ptxt->nFixedCharWidth)
        nTabWidth=(int)(ae->ptxt->nFixedTabWidth - nStringWidth % ae->ptxt->nFixedTabWidth);
      else
        nTabWidth=(int)(ae->ptxt->nTabWidth - nStringWidth % ae->ptxt->nTabWidth);
      nStringWidth+=nTabWidth;
    }
  }
  if (AE_GetTextExtentPoint32(ae, wpString + nStringCount, i - nStringCount, &sizeChar))
    nStringWidth+=sizeChar.cx;
  return nStringWidth - nFirstCharExtent;
}

void AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine)
{
  lpLine->nLineWidth=(int)AE_GetStringWidth(ae, lpLine->wpLine, lpLine->nLineLen, 0);
}

BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT64 *ptGlobalPos, POINT *ptClientPos)
{
  AECHARINDEX ciInitial;
  UINT_PTR dwFirst=(UINT_PTR)-1;
  UINT_PTR dwSecond=(UINT_PTR)-1;
  UINT_PTR dwThird=(UINT_PTR)-1;
  int nOffset=0;
  INT_PTR nStringWidth=0;
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
        nStringWidth+=(ciInitial.nCharInLine - max(ciInitial.lpLine->nLineLen, nStartChar)) * (ae->ptxt->nFixedCharWidth?ae->ptxt->nFixedCharWidth:ae->ptxt->nSpaceCharWidth);
    }
    else if (nOffset < 0)
    {
      if (nStartChar > ciInitial.lpLine->nLineLen)
        nStringWidth-=(nStartChar - max(ciInitial.lpLine->nLineLen, ciInitial.nCharInLine)) * (ae->ptxt->nFixedCharWidth?ae->ptxt->nFixedCharWidth:ae->ptxt->nSpaceCharWidth);

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
      ptGlobalPos->y=AE_VPosFromLine(ae, ciInitial.nLine);
    }
    if (ptClientPos)
    {
      ptClientPos->x=(int)(nStringWidth - ae->nHScrollPos) + ae->rcDraw.left;
      ptClientPos->y=(int)(AE_VPosFromLine(ae, ciInitial.nLine) - ae->nVScrollPos) + ae->rcDraw.top;
    }
    return TRUE;
  }
  return FALSE;
}

int AE_GetCharInLine(AKELEDIT *ae, const AELINEDATA *lpLine, INT_PTR nMaxExtent, DWORD dwFlags, int *nCharIndex, INT_PTR *nCharPos, BOOL bColumnSel)
{
  AECHARINDEX ciChar;
  UINT_PTR dwFirst=(UINT_PTR)-1;
  UINT_PTR dwSecond=(UINT_PTR)-1;
  UINT_PTR dwThird=(UINT_PTR)-1;
  INT_PTR nOffset=0;
  INT_PTR nStringWidth=0;
  int nStartChar=0;
  int nCharWidth=0;
  INT_PTR nInitMaxExtent=nMaxExtent;
  INT_PTR nHScrollMax;

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
      for (ciChar.nCharInLine=nStartChar; nStringWidth < nMaxExtent; AEC_IndexInc(&ciChar))
      {
        if (ciChar.nCharInLine < ciChar.lpLine->nLineLen)
        {
          if (nCharWidth=AE_GetCharWidth(ae, ciChar.lpLine->wpLine + ciChar.nCharInLine, nStringWidth))
            nStringWidth+=nCharWidth;
        }
        else
        {
          if (bColumnSel)
          {
            if (ae->ptxt->nFixedCharWidth)
              nCharWidth=ae->ptxt->nFixedCharWidth;
            else
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
              AEC_IndexDec(&ciChar);
            }
          }
          else
          {
            nStringWidth-=nCharWidth;
            AEC_IndexDec(&ciChar);
          }
        }
      }
    }
    else if (nOffset < 0)
    {
      ciChar.nCharInLine=nStartChar;
      AEC_IndexDec(&ciChar);

      for (; ciChar.nCharInLine >= 0 && nStringWidth > nMaxExtent; AEC_IndexDec(&ciChar))
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
            if (nCharWidth=AE_GetCharWidth(ae, ciChar.lpLine->wpLine + ciChar.nCharInLine, 0))
              nStringWidth-=nCharWidth;
          }
        }
        else
        {
          if (bColumnSel)
          {
            if (ae->ptxt->nFixedCharWidth)
              nCharWidth=ae->ptxt->nFixedCharWidth;
            else
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
      AEC_IndexInc(&ciChar);

      if (nStringWidth < nMaxExtent)
      {
        if (dwFlags & AECIL_HALFFIT)
        {
          if (nMaxExtent - nStringWidth >= nCharWidth / 2)
          {
            nStringWidth+=nCharWidth;
            AEC_IndexInc(&ciChar);
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

    if (nInitMaxExtent == nStringWidth)
      return AEPC_EQUAL;
    if (nInitMaxExtent > nStringWidth)
      return AEPC_BEFORE;
    return AEPC_AFTER;
  }
  return AEPC_ERROR;
}

BOOL AE_GetCharRangeInLine(AKELEDIT *ae, AELINEDATA *lpLine, INT_PTR nMinExtent, INT_PTR nMaxExtent, int *nMinCharIndex, INT_PTR *nMinCharPos, int *nMaxCharIndex, INT_PTR *nMaxCharPos, BOOL bColumnSel)
{
  int nMinIndex=0;
  int nMaxIndex=0;
  INT_PTR nMinPos=0;
  INT_PTR nMaxPos=0;

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

int AE_GetCharFromPos(AKELEDIT *ae, INT_PTR nClientX, INT_PTR nClientY, AECHARINDEX *ciCharIndex, POINT64 *ptGlobalPos, BOOL bColumnSel)
{
  INT_PTR nCharPos;
  INT_PTR nMaxExtent;
  int nFirstLine=0;
  int nLastLine=ae->ptxt->nLineCount;
  INT_PTR nHScrollMax;
  int nResult;

  ciCharIndex->nLine=AE_LineFromVPos(ae, ae->nVScrollPos + (nClientY - ae->rcDraw.top));
  ciCharIndex->nLine=max(ciCharIndex->nLine, nFirstLine);
  ciCharIndex->nLine=min(ciCharIndex->nLine, nLastLine);
  ciCharIndex->nCharInLine=0;
  if (ptGlobalPos) ptGlobalPos->y=AE_VPosFromLine(ae, ciCharIndex->nLine);

  nMaxExtent=ae->nHScrollPos + (nClientX - ae->rcDraw.left);
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
  const wchar_t *wpDelim;
  const wchar_t *wpMaxDelim=ae->popt->wszWordDelimiters + ae->popt->nWordDelimitersLen;
  wchar_t wchChar;
  int nLen=0;
  BOOL bInList;
  BOOL bIsSpacePrev=FALSE;
  BOOL bIsSpaceCur=FALSE;
  BOOL bInListNewLine=FALSE;
  BOOL dwInListSpaces=0;

  if (ciCount.nCharInLine > ciCount.lpLine->nLineLen)
    return 0;
  for (wpDelim=ae->popt->wszWordDelimiters; wpDelim < wpMaxDelim; ++wpDelim)
  {
    if (*wpDelim == L'\n') bInListNewLine=TRUE;
    else if (*wpDelim == L' ') dwInListSpaces|=AEWBS_SPACE;
    else if (*wpDelim == L'\t') dwInListSpaces|=AEWBS_TAB;
  }

  if (dwFlags & AEWB_MINMOVE)
  {
    if (!AEC_PrevCharEx(&ciCount, &ciCount))
      return 0;
  }

  if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
  {
    if (ciCount.lpLine->next)
    {
      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
        AEC_NextLine(&ciCount);
    }
    else goto End;
  }

  if (ciCount.nCharInLine >= ciCount.lpLine->nLineLen)
  {
    AEC_NextLine(&ciCount);
    ++nLen;
    if (dwFlags & AEWB_STOPNEWLINE)
      goto End;

    bInList=bInListNewLine;
    bIsSpacePrev=bInListNewLine;
  }
  else
  {
    wchChar=ciCount.lpLine->wpLine[ciCount.nCharInLine];
    bInList=AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ae->popt->nWordDelimitersLen, wchChar);
    bIsSpacePrev=AE_IsSpace(wchChar, dwInListSpaces);

    nLen+=AEC_IndexInc(&ciCount);
  }

  for (;;)
  {
    while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
    {
      wchChar=ciCount.lpLine->wpLine[ciCount.nCharInLine];
      bIsSpaceCur=AE_IsSpace(wchChar, dwInListSpaces);

      if (dwFlags & AEWB_STOPSPACESTART)
      {
        if (!bIsSpacePrev && bIsSpaceCur)
          goto End;
      }
      if (dwFlags & AEWB_STOPSPACEEND)
      {
        if (bIsSpacePrev && !bIsSpaceCur)
          goto End;
      }
      if (bInList != AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ae->popt->nWordDelimitersLen, wchChar))
      {
        if (bInList)
        {
          if (!(dwFlags & AEWB_SKIPSPACEEND) || !bIsSpacePrev)
          {
            if (dwFlags & AEWB_RIGHTWORDSTART)
              goto End;
          }
        }
        else
        {
          if (!(dwFlags & AEWB_SKIPSPACESTART) || !bIsSpaceCur)
          {
            if (dwFlags & AEWB_RIGHTWORDEND)
              goto End;
          }
        }
        bInList=!bInList;
      }
      bIsSpacePrev=bIsSpaceCur;

      nLen+=AEC_IndexInc(&ciCount);
    }
    if (bColumnSel) goto End;

    if (ciCount.lpLine->nLineBreak != AELB_WRAP)
    {
      if (dwFlags & AEWB_STOPNEWLINE)
        goto End;

      if (dwFlags & AEWB_STOPSPACESTART)
      {
        if (!bIsSpacePrev && bInListNewLine)
          goto End;
      }
      if (bInList != bInListNewLine)
      {
        if (bInList)
        {
          if (!(dwFlags & AEWB_SKIPSPACEEND) || !bIsSpacePrev)
          {
            if (dwFlags & AEWB_RIGHTWORDSTART)
              goto End;
          }
        }
        else
        {
          if (!(dwFlags & AEWB_SKIPSPACESTART))
          {
            if (dwFlags & AEWB_RIGHTWORDEND)
              goto End;
          }
        }
      }
      bInList=bInListNewLine;
      bIsSpacePrev=bInListNewLine;

      ++nLen;
    }

    if (ciCount.lpLine->next)
      AEC_NextLine(&ciCount);
    else
      goto End;
  }

  End:
  AEC_ValidCharInLine(&ciCount);

  if (AEC_IndexCompare(ciChar, &ciCount))
  {
    *ciNextBreak=ciCount;
    return nLen;
  }
  return 0;
}

int AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel, DWORD dwFlags)
{
  AECHARINDEX ciCount=*ciChar;
  const wchar_t *wpDelim;
  const wchar_t *wpMaxDelim=ae->popt->wszWordDelimiters + ae->popt->nWordDelimitersLen;
  wchar_t wchChar;
  int nLen=0;
  BOOL bInList;
  BOOL bIsSpacePrev=FALSE;
  BOOL bIsSpaceCur=FALSE;
  BOOL bInListNewLine=FALSE;
  BOOL dwInListSpaces=0;

  if (ciCount.nCharInLine > ciCount.lpLine->nLineLen)
    return 0;
  for (wpDelim=ae->popt->wszWordDelimiters; wpDelim < wpMaxDelim; ++wpDelim)
  {
    if (*wpDelim == L'\n') bInListNewLine=TRUE;
    else if (*wpDelim == L' ') dwInListSpaces|=AEWBS_SPACE;
    else if (*wpDelim == L'\t') dwInListSpaces|=AEWBS_TAB;
  }

  if (dwFlags & AEWB_MINMOVE)
  {
    if (!AEC_NextCharEx(&ciCount, &ciCount))
      return 0;
  }

  if (ciCount.nCharInLine <= 0)
  {
    if (ciCount.lpLine->prev)
    {
      if (ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
        AEC_PrevLine(&ciCount);
    }
    else return 0;
  }
  nLen+=AEC_IndexDec(&ciCount);

  if (ciCount.nCharInLine < 0)
  {
    AEC_PrevLine(&ciCount);
    nLen+=AEC_IndexDec(&ciCount);
    if (dwFlags & AEWB_STOPNEWLINE)
      goto End;

    bInList=bInListNewLine;
    bIsSpacePrev=bInListNewLine;
  }
  else
  {
    wchChar=ciCount.lpLine->wpLine[ciCount.nCharInLine];
    bInList=AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ae->popt->nWordDelimitersLen, wchChar);
    bIsSpacePrev=AE_IsSpace(wchChar, dwInListSpaces);

    nLen+=AEC_IndexDec(&ciCount);
  }

  for (;;)
  {
    while (ciCount.nCharInLine >= 0)
    {
      wchChar=ciCount.lpLine->wpLine[ciCount.nCharInLine];
      bIsSpaceCur=AE_IsSpace(wchChar, dwInListSpaces);

      if (dwFlags & AEWB_STOPSPACESTART)
      {
        if (bIsSpacePrev && !bIsSpaceCur)
          goto End;
      }
      if (dwFlags & AEWB_STOPSPACEEND)
      {
        if (!bIsSpacePrev && bIsSpaceCur)
          goto End;
      }
      if (bInList != AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ae->popt->nWordDelimitersLen, wchChar))
      {
        if (bInList)
        {
          if (!(dwFlags & AEWB_SKIPSPACESTART) || !bIsSpacePrev)
          {
            if (dwFlags & AEWB_LEFTWORDEND)
              goto End;
          }
        }
        else
        {
          if (!(dwFlags & AEWB_SKIPSPACEEND) || !bIsSpaceCur)
          {
            if (dwFlags & AEWB_LEFTWORDSTART)
              goto End;
          }
        }
        bInList=!bInList;
      }
      bIsSpacePrev=bIsSpaceCur;

      nLen+=AEC_IndexDec(&ciCount);
    }
    if (bColumnSel) goto End;

    if (ciCount.lpLine->prev && ciCount.lpLine->prev->nLineBreak != AELB_WRAP)
    {
      if (dwFlags & AEWB_STOPNEWLINE)
        goto End;

      if (dwFlags & AEWB_STOPSPACEEND)
      {
        if (!bIsSpacePrev &&  bInListNewLine)
          goto End;
      }
      if (bInList != bInListNewLine)
      {
        if (bInList)
        {
          if (!(dwFlags & AEWB_SKIPSPACESTART) || !bIsSpacePrev)
          {
            if (dwFlags & AEWB_LEFTWORDEND)
              goto End;
          }
        }
        else
        {
          if (!(dwFlags & AEWB_SKIPSPACEEND))
          {
            if (dwFlags & AEWB_LEFTWORDSTART)
              goto End;
          }
        }
      }
      bInList=bInListNewLine;
      bIsSpacePrev=bInListNewLine;

      ++nLen;
    }

    if (ciCount.lpLine->prev)
    {
      AEC_PrevLine(&ciCount);
      AEC_IndexDec(&ciCount);
    }
    else goto End;
  }

  End:
  nLen-=AEC_IndexInc(&ciCount);
  AEC_ValidCharInLine(&ciCount);

  if (AEC_IndexCompare(ciChar, &ciCount))
  {
    *ciPrevBreak=ciCount;
    return nLen;
  }
  return 0;
}

BOOL AE_IsSpacesFromLeft(const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;
  wchar_t wchChar;

  if (AEC_IsFirstCharInLine(&ciCount))
    return TRUE;

  while (AEC_PrevCharInLine(&ciCount))
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

  if (AEC_IsLastCharInLine(&ciCount))
    return TRUE;

  do
  {
    wchChar=*(ciCount.lpLine->wpLine + ciCount.nCharInLine);
    if (wchChar != L' ' && wchChar != L'\t')
      return FALSE;
  }
  while (AEC_NextCharInLine(&ciCount));

  return TRUE;
}

BOOL AE_IsEscaped(const AECHARINDEX *ciChar, wchar_t wchEscape)
{
  AECHARINDEX ciCount=*ciChar;
  int nEscapeCount=0;

  while (AEC_PrevCharInLine(&ciCount))
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
    if (AEC_PrevChar(&ciTmp))
      nChar=AEC_CharAtIndex(&ciTmp);
  }
  else nChar=AEC_CharAtIndex(&ciTmp);

  //Start of file, end of file or end of line
  if (nChar <= -AELB_EOF)
  {
    if (nChar == -AELB_EOF)
      return TRUE;
    nChar=L'\n';
  }

  if (dwType & AEDLM_WORD)
    return AE_IsInDelimiterList(ae->popt->wszWordDelimiters, ae->popt->nWordDelimitersLen, (wchar_t)nChar);
  if (dwType & AEDLM_WRAP)
    return AE_IsInDelimiterList(ae->ptxt->wszWrapDelimiters, ae->ptxt->nWrapDelimitersLen, (wchar_t)nChar);
  if (dwType & AEDLM_URLLEFT)
    return AE_IsInDelimiterList(ae->popt->wszUrlLeftDelimiters, ae->popt->nUrlLeftDelimitersLen, (wchar_t)nChar);
  if (dwType & AEDLM_URLRIGHT)
    return AE_IsInDelimiterList(ae->popt->wszUrlRightDelimiters, ae->popt->nUrlRightDelimitersLen, (wchar_t)nChar);
  return FALSE;
}

BOOL AE_IsInDelimiterList(const wchar_t *wpList, int nListLen, wchar_t c)
{
  const wchar_t *wpMaxList=wpList + nListLen;

  if (c == L'\r' || c == L'\n')
  {
    while (wpList < wpMaxList && *wpList != L'\r' && *wpList != L'\n')
      ++wpList;
    return (wpList < wpMaxList);
  }
  else
  {
    while (wpList < wpMaxList && *wpList != c)
      ++wpList;
    return (wpList < wpMaxList);
  }
}

BOOL AE_IsSpace(wchar_t c, DWORD dwSpacesFlags)
{
  if ((c == L' ' && (dwSpacesFlags & AEWBS_SPACE)) ||
      (c == L'\t' && (dwSpacesFlags & AEWBS_TAB)))
  {
    return TRUE;
  }
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

UINT_PTR AE_GetTextRangeAnsi(AKELEDIT *ae, int nCodePage, const char *lpDefaultChar, BOOL *lpUsedDefaultChar, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, char *szBuffer, UINT_PTR dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  wchar_t *wszText;
  UINT_PTR dwAnsiBytes;
  UINT_PTR dwUnicodeLen;

  if (dwUnicodeLen=AE_GetTextRange(ae, ciRangeStart, ciRangeEnd, NULL, 0, nNewLine, bColumnSel, bFillSpaces))
  {
    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeLen * sizeof(wchar_t)))
    {
      dwUnicodeLen=AE_GetTextRange(ae, ciRangeStart, ciRangeEnd, wszText, (UINT_PTR)-1, nNewLine, bColumnSel, bFillSpaces);

      //Get Ansi text
      dwAnsiBytes=WideCharToMultiByte64(nCodePage, 0, wszText, dwUnicodeLen + 1, NULL, 0, lpDefaultChar, lpUsedDefaultChar);

      if (szBuffer)
      {
        dwAnsiBytes=min(dwAnsiBytes, dwBufferSize);

        if (dwAnsiBytes)
        {
          WideCharToMultiByte64(nCodePage, 0, wszText, dwUnicodeLen + 1, szBuffer, dwAnsiBytes, lpDefaultChar, lpUsedDefaultChar);
          szBuffer[--dwAnsiBytes]='\0';
        }
      }
      AE_HeapFree(ae, 0, (LPVOID)wszText);
      return dwAnsiBytes;
    }
  }
  return 0;
}

UINT_PTR AE_GetTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, wchar_t *wszBuffer, UINT_PTR dwBufferSize, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  AECHARINDEX ciStart=*ciRangeStart;
  AECHARINDEX ciEnd=*ciRangeEnd;
  AECHARINDEX ciTmp;
  AELINEDATA *lpLine;
  UINT_PTR dwCopied=0;
  UINT_PTR dwCharsToCopy;
  BYTE nLineBreak;
  int i;

  if (ciStart.lpLine && ciEnd.lpLine)
  {
    if (bColumnSel == -1) bColumnSel=ae->bColumnSel;

    //Exchange indexes
    if (AEC_IndexCompare(&ciStart, &ciEnd) > 0)
    {
      ciTmp=ciStart;
      ciStart=ciEnd;
      ciEnd=ciTmp;
    }

    if (wszBuffer)
    {
      if (dwBufferSize)
      {
        if (AEC_IndexCompare(&ciStart, &ciEnd))
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
            for (lpLine=ciStart.lpLine; lpLine; lpLine=lpLine->next)
            {
              for (i=lpLine->nSelStart; i < lpLine->nLineLen && i < lpLine->nSelEnd; ++i)
              {
                wszBuffer[dwCopied++]=lpLine->wpLine[i];
                if (dwCopied >= dwBufferSize) goto End;
              }
              if (bFillSpaces)
              {
                while (i < lpLine->nSelEnd)
                {
                  wszBuffer[dwCopied++]=L' ';
                  if (dwCopied >= dwBufferSize) goto End;

                  ++i;
                }
              }
              if (lpLine == ciEnd.lpLine) break;

              if (lpLine->nLineBreak == AELB_WRAP)
              {
                if (nNewLine == AELB_ASIS)
                  nLineBreak=AELB_RN;
                else
                  nLineBreak=(BYTE)nNewLine;
              }
              else
              {
                if (nNewLine == AELB_ASIS)
                  nLineBreak=lpLine->nLineBreak;
                else
                  nLineBreak=(BYTE)nNewLine;
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
            }
          }
          else
          {
            //Normalize
            ciStart.nCharInLine=min(ciStart.nCharInLine, ciStart.lpLine->nLineLen);
            ciEnd.nCharInLine=min(ciEnd.nCharInLine, ciEnd.lpLine->nLineLen);

            if (ciStart.nLine == ciEnd.nLine)
            {
              dwCharsToCopy=min((DWORD)(ciEnd.nCharInLine - ciStart.nCharInLine), dwBufferSize - dwCopied);
              xmemcpy(wszBuffer + dwCopied, ciStart.lpLine->wpLine + ciStart.nCharInLine, dwCharsToCopy * sizeof(wchar_t));
              dwCopied+=dwCharsToCopy;
              if (dwCopied >= dwBufferSize) goto End;
            }
            else
            {
              for (lpLine=ciStart.lpLine; lpLine; lpLine=lpLine->next)
              {
                if (lpLine == ciEnd.lpLine)
                {
                  //Last line
                  dwCharsToCopy=min((DWORD)ciEnd.nCharInLine, dwBufferSize - dwCopied);
                  xmemcpy(wszBuffer + dwCopied, ciEnd.lpLine->wpLine, dwCharsToCopy * sizeof(wchar_t));
                  dwCopied+=dwCharsToCopy;
                  break;
                }

                if (lpLine == ciStart.lpLine)
                {
                  //First line
                  dwCharsToCopy=min((DWORD)(ciStart.lpLine->nLineLen - ciStart.nCharInLine), dwBufferSize - dwCopied);
                  xmemcpy(wszBuffer + dwCopied, ciStart.lpLine->wpLine + ciStart.nCharInLine, dwCharsToCopy * sizeof(wchar_t));
                  dwCopied+=dwCharsToCopy;
                  if (dwCopied >= dwBufferSize) goto End;
                }
                else
                {
                  //Middle
                  dwCharsToCopy=min((DWORD)lpLine->nLineLen, dwBufferSize - dwCopied);
                  xmemcpy(wszBuffer + dwCopied, lpLine->wpLine, dwCharsToCopy * sizeof(wchar_t));
                  dwCopied+=dwCharsToCopy;
                  if (dwCopied >= dwBufferSize) goto End;
                }

                if (lpLine->nLineBreak != AELB_WRAP)
                {
                  if (nNewLine == AELB_ASIS)
                    nLineBreak=lpLine->nLineBreak;
                  else
                    nLineBreak=(BYTE)nNewLine;

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

UINT_PTR AE_SetTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, UINT_PTR dwTextLen, int nNewLine)
{
  wchar_t *wszText;
  UINT_PTR dwUnicodeLen;
  UINT_PTR dwResult=0;

  if (dwUnicodeLen=MultiByteToWideChar64(nCodePage, 0, pText, dwTextLen, NULL, 0))
  {
    if (dwTextLen == (UINT_PTR)-1)
      dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar64(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeLen + 1);
      dwResult=AE_SetText(ae, wszText, dwUnicodeLen, nNewLine, FALSE);

      AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
  }
  return dwResult;
}

UINT_PTR AE_SetText(AKELEDIT *ae, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine, BOOL bOnInitWindow)
{
  AECHARINDEX ciCaretChar;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpLastLine;
  wchar_t *wpLineStart=(wchar_t *)wpText;
  wchar_t *wpLineEnd=(wchar_t *)wpText;
  HDC hDC=ae->hDC;
  HFONT hFontOld=NULL;
  UINT_PTR dwTextCount=0;
  DWORD dwStartTime=GetTickCount();
  DWORD dwProgressTime=0;
  DWORD dwCurrentTime=0;
  int nLinesInPage;
  BYTE nLineBreak;
  BOOL bUpdated=FALSE;
  DWORD dwStopProgress=0;

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
    if (dwTextLen == (UINT_PTR)-1)
      dwTextLen=xstrlenW(wpText);

    if (!(ae->popt->dwOptions & AECO_MULTILINE))
      AE_GetNextLine(ae, wpText, (DWORD)dwTextLen, (int *)&dwTextLen, NULL);

    while (dwTextCount < dwTextLen)
    {
      //Progressing
      if (ae->popt->dwEventMask & AENM_PROGRESS)
      {
        dwCurrentTime=GetTickCount();

        if (dwCurrentTime - dwProgressTime > AETIME_BEFOREPROGRESS)
        {
          if (dwStopProgress=AE_NotifyProgress(ae, AEPGS_SETTEXT, dwCurrentTime - dwStartTime, dwTextCount, dwTextLen))
          {
            lpElement=NULL;
            break;
          }
          dwProgressTime=GetTickCount();
        }
      }

      if (lpElement=AE_StackLineAdd(ae))
      {
        wpLineEnd=AE_GetNextLine(ae, wpLineStart, (DWORD)(dwTextLen - dwTextCount), &lpElement->nLineLen, &lpElement->nLineBreak);
        dwTextCount+=wpLineEnd - wpLineStart;
        ae->ptxt->nLastCharOffset+=lpElement->nLineLen + 1;

        if (lpElement->nLineBreak != AELB_EOF)
        {
          if (nNewLine != AELB_ASIS)
            lpElement->nLineBreak=(BYTE)nNewLine;
        }
        else --ae->ptxt->nLastCharOffset;

        if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpElement->nLineLen + 1) * sizeof(wchar_t)))
        {
          xmemcpy(lpElement->wpLine, wpLineStart, lpElement->nLineLen * sizeof(wchar_t));
          lpElement->wpLine[lpElement->nLineLen]=L'\0';
          lpElement->nLineWidth=-1;
          AE_GetLineWidth(ae, lpElement);
        }
        else
        {
          AE_StackLineDelete(ae, lpElement);
          lpElement=NULL;
          break;
        }
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

              --ae->ptxt->nLastCharOffset;
              nLineBreak=lpElement->nLineBreak;
              AE_FixEdit(ae, FALSE);

              InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
              UpdateWindow(ae->hWndEdit);

              //Make sure that WM_PAINT will be send after setting text
              {
                RECT rcOnePixel={0, 0, 1, 1};

                InvalidateRect(ae->hWndEdit, &rcOnePixel, TRUE);
              }

              //Restore variables
              ++ae->ptxt->nLastCharOffset;
              (ae->ptxt->hLinesStack.last)->nLineBreak=nLineBreak;
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
      if (lpLastLine=AE_StackLineAdd(ae))
      {
        if (lpLastLine->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, sizeof(wchar_t)))
        {
          lpLastLine->nLineWidth=0;
          lpLastLine->wpLine[0]=L'\0';

          ++ae->ptxt->nLineCount;
          ++ae->ptxt->nLineUnwrapCount;
        }
        else
        {
          AE_StackLineDelete(ae, lpLastLine);
          lpLastLine=lpElement;
        }
      }
      else lpLastLine=lpElement;

      if (lpLastLine)
        lpLastLine->nLineBreak=AELB_EOF;
    }
    if (ae->ptxt->nLineCount)
    {
      --ae->ptxt->nLineCount;
      --ae->ptxt->nLineUnwrapCount;
    }

    //End progress
    if (ae->popt->dwEventMask & AENM_PROGRESS)
    {
      if (!bOnInitWindow && !dwStopProgress)
      {
        AE_NotifyProgress(ae, AEPGS_SETTEXT, GetTickCount() - dwStartTime, dwTextLen, dwTextLen);
      }
    }
    ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar);
    ae->ciCaretIndex=ciCaretChar;
    ae->ciSelStartIndex=ciCaretChar;
    ae->ciSelEndIndex=ciCaretChar;

    if (!bOnInitWindow)
    {
      if (ae->ptxt->dwWordWrap)
      {
        ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

        ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar);
        ae->ciCaretIndex=ciCaretChar;
        ae->ciSelStartIndex=ciCaretChar;
        ae->ciSelEndIndex=ciCaretChar;
      }
      AE_StackPointReset(ae, AEPTT_SETTEXT);
      AE_UpdateScrollBars(ae, SB_VERT);
      AE_CalcLinesWidth(ae, NULL, NULL, 0);
      if (!bUpdated) InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);

      ae->dwNotifyFlags=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
      ae->dwNotifyTextChange|=AETCT_DELETEALL;
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
  }

  if (!bOnInitWindow)
  {
    AE_NotifyChanged(ae); //AETCT_SETTEXT
  }
  return dwTextCount;
}

UINT_PTR AE_StreamIn(AKELEDIT *ae, DWORD dwFlags, AESTREAMIN *aesi)
{
  AECHARINDEX ciCaretChar;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpLastLine;
  wchar_t *wszBuf;
  wchar_t *wpLineStart;
  wchar_t *wpLineEnd;
  HDC hDC=ae->hDC;
  HFONT hFontOld=NULL;
  UINT_PTR dwTextLen=0;
  UINT_PTR dwTextCount=0;
  DWORD dwBlockLen=0;
  DWORD dwBlockCount=0;
  DWORD dwStartTime=GetTickCount();
  DWORD dwProgressTime=0;
  DWORD dwCurrentTime=0;
  DWORD dwBufLen=2048;
  DWORD dwBufDone;
  UINT_PTR dwResult=0;
  int nNewLine=aesi->nNewLine;
  int nLinesInPage;
  int nApproxLinesCount=0;
  BYTE nLineBreak;
  BOOL bUpdated=FALSE;
  DWORD dwStopProgress=0;

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
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATEALL);

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
        AE_InsertText(ae, &ae->ciCaretIndex, wszBuf, dwBufDone / sizeof(wchar_t), nNewLine, FALSE, AEINST_LOCKSCROLL|AEINST_LOCKUPDATEALL, &ciInsertStart, &ciInsertEnd);
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
      AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
      ae->nCaretHorzIndent=ae->ptCaret.x;
      if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    }
    else
    {
      //Free old and create new heap
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
              if (dwStopProgress=AE_NotifyProgress(ae, AEPGS_STREAMIN, dwCurrentTime - dwStartTime, dwTextCount, dwTextLen))
              {
                aesi->dwError=dwStopProgress;
                lpElement=NULL;
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
            lpElement=NULL;
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
              dwBlockCount+=(DWORD)(wpLineEnd - wpLineStart);
              ae->ptxt->nLastCharOffset+=lpElement->nLineLen + 1;

              if (lpElement->nLineBreak != AELB_EOF)
              {
                if (nNewLine != AELB_ASIS)
                  lpElement->nLineBreak=(BYTE)nNewLine;
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
              else
              {
                AE_StackLineDelete(ae, lpElement);
                lpElement=ae->ptxt->hLinesStack.last;
                if (lpElement) lpElement->nLineBreak=AELB_EOF;
                goto LastLine;
              }
            }
            else goto LastLine;

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

                --ae->ptxt->nLastCharOffset;
                nLineBreak=(ae->ptxt->hLinesStack.last)->nLineBreak;
                AE_FixEdit(ae, TRUE);

                InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
                UpdateWindow(ae->hWndEdit);

                //Make sure that WM_PAINT will be send after setting text
                {
                  RECT rcOnePixel={0, 0, 1, 1};

                  InvalidateRect(ae->hWndEdit, &rcOnePixel, TRUE);
                }

                //Restore variables
                ++ae->ptxt->nLastCharOffset;
                (ae->ptxt->hLinesStack.last)->nLineBreak=nLineBreak;
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
          lpElement=ae->ptxt->hLinesStack.last;
          if (lpElement) lpElement->nLineFlags=AELF_SPLIT;
          dwTextCount+=dwBlockLen;
        }

        //Last line
        LastLine:
        if (lpElement) lpElement->nLineFlags=0;

        if (!lpElement || lpElement->nLineBreak != AELB_EOF)
        {
          if (lpLastLine=AE_StackLineAdd(ae))
          {
            if (lpLastLine->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, sizeof(wchar_t)))
            {
              lpLastLine->nLineWidth=0;
              lpLastLine->wpLine[0]=L'\0';

              ++ae->ptxt->nLineCount;
              ++ae->ptxt->nLineUnwrapCount;
            }
            else
            {
              AE_StackLineDelete(ae, lpLastLine);
              lpLastLine=lpElement;
            }
          }
          else lpLastLine=lpElement;

          if (lpLastLine)
            lpLastLine->nLineBreak=AELB_EOF;
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
          if (!dwStopProgress)
          {
            AE_NotifyProgress(ae, AEPGS_STREAMIN, GetTickCount() - dwStartTime, dwTextLen, dwTextLen);
          }
        }

        ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar);
        ae->ciCaretIndex=ciCaretChar;
        ae->ciSelStartIndex=ciCaretChar;
        ae->ciSelEndIndex=ciCaretChar;
        aesi->nFirstNewLine=ciCaretChar.lpLine->nLineBreak;

        if (ae->ptxt->dwWordWrap)
        {
          ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

          ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
          AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar);
          ae->ciCaretIndex=ciCaretChar;
          ae->ciSelStartIndex=ciCaretChar;
          ae->ciSelEndIndex=ciCaretChar;
        }
        AE_StackPointReset(ae, AEPTT_STREAMIN);
        AE_UpdateScrollBars(ae, SB_VERT);
        AE_CalcLinesWidth(ae, NULL, NULL, 0);
        if (!bUpdated) InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);

        ae->dwNotifyFlags=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
        ae->dwNotifyTextChange|=AETCT_DELETEALL;

        //Set caret position
        if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

        //Release DC
        if (!hDC)
        {
          if (hFontOld) SelectObject(ae->hDC, hFontOld);
          ReleaseDC(ae->hWndEdit, ae->hDC);
          ae->hDC=NULL;
        }
      }
    }
    AE_HeapFree(NULL, 0, (LPVOID)wszBuf);
  }

  AE_NotifyChanged(ae); //AETCT_STREAMIN

  return dwResult;
}

void AE_FixEdit(AKELEDIT *ae, BOOL bJoinNewLines)
{
  AECHARINDEX ciCaretChar;

  (ae->ptxt->hLinesStack.last)->nLineBreak=AELB_EOF;
  if (bJoinNewLines) AE_JoinNewLines(ae);

  ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar);
  ae->ciCaretIndex=ciCaretChar;
  ae->ciSelStartIndex=ciCaretChar;
  ae->ciSelEndIndex=ciCaretChar;

  if (!(ae->popt->dwOptions & AECO_DISABLENOSCROLL))
    AE_UpdateScrollBars(ae, SB_VERT);

  if (ae->ptxt->dwWordWrap)
  {
    ae->ptxt->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->ptxt->dwWordWrap);

    ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar);
    ae->ciCaretIndex=ciCaretChar;
    ae->ciSelStartIndex=ciCaretChar;
    ae->ciSelEndIndex=ciCaretChar;
  }
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
  liLineStart.lpLine=ae->ptxt->hLinesStack.first;
  liLineEnd.nLine=0;
  liLineEnd.lpLine=ae->ptxt->hLinesStack.first;

  while (liLineEnd.lpLine)
  {
    dwCountLen+=liLineEnd.lpLine->nLineLen;

    if (liLineEnd.lpLine->nLineFlags == AELF_SPLIT)
      ++nSplitFound;

    if (!(liLineEnd.lpLine->nLineBreak == AELB_EOF &&
          liLineEnd.lpLine->nLineFlags == AELF_SPLIT))
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
        if (liLineEnd.lpLine->nLineFlags == AELF_SPLIT)
        {
          liLineEnd.lpLine->nLineFlags=0;

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
            else if (liLineEnd.lpLine->prev && (liLineEnd.lpLine->prev->nLineBreak == AELB_EOF &&
                                                liLineEnd.lpLine->prev->nLineFlags == AELF_SPLIT))
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
            dwCountLen=0;

            for (lpCurElement=liLineStart.lpLine; lpCurElement; lpCurElement=lpNextElement)
            {
              xmemcpy(lpNewElement->wpLine + dwCountLen, lpCurElement->wpLine, lpCurElement->nLineLen * sizeof(wchar_t));
              dwCountLen+=lpCurElement->nLineLen;

              if (lpCurElement->nLineFlags != AELF_SPLIT)
              {
                AE_StackLineDelete(ae, lpCurElement);
                break;
              }
              lpNextElement=lpCurElement->next;
              AE_StackLineDelete(ae, lpCurElement);
            }
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
            AE_GetLineWidth(ae, lpNewElement);
          }
          else
          {
            AE_StackLineDelete(ae, lpNewElement);
            lpNewElement=NULL;
            break;
          }
        }
        else break;

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

UINT_PTR AE_StreamOut(AKELEDIT *ae, DWORD dwFlags, AESTREAMOUT *aeso)
{
  AECHARINDEX ciCount;
  AECHARINDEX ciEnd;
  wchar_t *wszBuf;
  int nNewLine=aeso->nNewLine;
  BYTE nLineBreak;
  BOOL bColumnSel=aeso->bColumnSel;
  DWORD dwBufLen=2048;
  DWORD dwBufCount=0;
  UINT_PTR dwResult=0;

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
    AE_GetIndex(ae, AEGI_FIRSTSELCHAR, NULL, &ciCount);
    AE_GetIndex(ae, AEGI_LASTSELCHAR, NULL, &ciEnd);
  }
  else
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCount);
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciEnd);
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
        nLineBreak=(BYTE)nNewLine;

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
      AEC_NextLine(&ciCount);
    }
    AE_StreamOutHelper(aeso, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult);

    End:
    AE_HeapFree(NULL, 0, (LPVOID)wszBuf);
  }

  return dwResult;
}

BOOL AE_StreamOutHelper(AESTREAMOUT *aeso, AECHARINDEX *ciCount, const AECHARINDEX *ciEnd, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufCount, UINT_PTR *dwResult)
{
  DWORD dwBufDone=0;

  if (AEC_IndexCompare(ciCount, ciEnd) >= 0)
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
    if (ciCount->nCharInLine > 0 && AEC_IsHighSurrogate(ciCount->lpLine->wpLine[ciCount->nCharInLine - 1]))
    {
      --ciCount->nCharInLine;
      --dwBufLen;
    }
    wszBuf[dwBufLen]=L'\0';
    *dwBufCount=0;
    if (aeso->dwError=aeso->lpCallback(aeso->dwCookie, wszBuf, dwBufLen * sizeof(wchar_t), &dwBufDone)) return FALSE;
    if (!dwBufDone) return FALSE;
    *dwResult+=dwBufDone;
  }
  return TRUE;
}

void AE_AppendTextAnsi(AKELEDIT *ae, int nCodePage, const char *pText, UINT_PTR dwTextLen, int nNewLine)
{
  wchar_t *wszText;
  UINT_PTR dwUnicodeLen;

  if (dwUnicodeLen=MultiByteToWideChar64(nCodePage, 0, pText, dwTextLen, NULL, 0))
  {
    if (dwTextLen == (UINT_PTR)-1)
      dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar64(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeLen + 1);
      AE_AppendText(ae, wszText, dwUnicodeLen, nNewLine);

      AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
  }
}

void AE_AppendText(AKELEDIT *ae, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine)
{
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciSelStartIndex;
  AECHARINDEX ciSelEndIndex;
  AECHARINDEX ciLastChar;
  INT_PTR nSelStartCharOffset;
  INT_PTR nSelEndCharOffset;

  AE_NotifyChanging(ae, AETCT_APPENDTEXT);
  AE_StackUndoGroupStop(ae);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciLastChar);

  //Save selection points
  ciCaretIndex=ae->ciCaretIndex;
  ciSelStartIndex=ae->ciSelStartIndex;
  ciSelEndIndex=ae->ciSelEndIndex;
  nSelStartCharOffset=ae->nSelStartCharOffset;
  nSelEndCharOffset=ae->nSelEndCharOffset;

  if (AE_InsertText(ae, &ciLastChar, wpText, dwTextLen, nNewLine, FALSE, AEINST_LOCKSCROLL|AEINST_LOCKUPDATEALL, NULL, NULL))
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

void AE_ReplaceSelAnsi(AKELEDIT *ae, int nCodePage, const char *pText, UINT_PTR dwTextLen, int nNewLine, DWORD dwFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  wchar_t *wszText;
  UINT_PTR dwUnicodeLen;

  if (dwUnicodeLen=MultiByteToWideChar64(nCodePage, 0, pText, dwTextLen, NULL, 0))
  {
    if (dwTextLen == (UINT_PTR)-1)
      dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar64(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeLen + 1);
      AE_ReplaceSel(ae, wszText, dwUnicodeLen, nNewLine, dwFlags, ciInsertStart, ciInsertEnd);

      AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
  }
}

void AE_ReplaceSel(AKELEDIT *ae, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine, DWORD dwFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AECHARINDEX ciStart={0};
  AECHARINDEX ciEnd={0};
  INT_PTR nHScrollPos;
  INT_PTR nVScrollPos;
  INT_PTR nVScrollMax;
  BOOL bColumnSel=FALSE;
  BOOL bUpdateVScroll=FALSE;
  BOOL bUpdateCaret=FALSE;
  BOOL bCaretAtStart=FALSE;

  AE_NotifyChanging(ae, AETCT_REPLACESEL);
  if (!(dwFlags & AEREPT_UNDOGROUPING))
    AE_StackUndoGroupStop(ae);
  if (dwFlags & AEREPT_SELECT)
  {
    if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex))
      bCaretAtStart=TRUE;
  }

  if (dwFlags & AEREPT_COLUMNON)
    bColumnSel=TRUE;
  else if (dwFlags & AEREPT_COLUMNASIS)
    bColumnSel=ae->bColumnSel;
  nHScrollPos=ae->nHScrollPos;
  nVScrollPos=ae->nVScrollPos;
  nVScrollMax=ae->ptxt->nVScrollMax;
  if (AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_SAVECOLUMNSEL|AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATECARET|AEDELT_LOCKUPDATEVSCROLL))
  {
    if (nVScrollMax != ae->ptxt->nVScrollMax)
    {
      //VScroll is changed
      nVScrollMax=ae->ptxt->nVScrollMax;
      bUpdateVScroll=TRUE;
    }
    bUpdateCaret=TRUE;
  }
  if (AE_InsertText(ae, &ae->ciCaretIndex, wpText, dwTextLen, nNewLine, bColumnSel, AEINST_LOCKSCROLL|AEINST_LOCKUPDATECARET, &ciStart, &ciEnd))
  {
    bUpdateCaret=TRUE;
  }
  else if (AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    //Reset saved selection after AEDELT_SAVECOLUMNSEL
    AE_SetSelectionPos(ae, &ae->ciCaretIndex, &ae->ciCaretIndex, FALSE, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
  }

  if (bUpdateVScroll && nVScrollMax == ae->ptxt->nVScrollMax)
  {
    //VScroll is not updated in AE_InsertText
    AE_UpdateScrollBars(ae, SB_VERT);
    if (nVScrollPos != ae->nVScrollPos)
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
  }
  if (bUpdateCaret)
  {
    if (!(dwFlags & AEREPT_LOCKSCROLL))
      AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
    else
      AE_SetScrollPos(ae, nHScrollPos, nVScrollPos);
    ae->nCaretHorzIndent=ae->ptCaret.x;
    if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
  }
  if (!(dwFlags & AEREPT_UNDOGROUPING))
    AE_StackUndoGroupStop(ae);

  if (bColumnSel)
  {
    if (ae->popt->dwOptions & AECO_PASTESELECTCOLUMN)
    {
      AE_SetSelectionPos(ae, &ciStart, &ciEnd, bColumnSel, AESELT_LOCKNOTIFY, 0);
      dwFlags&=~AEREPT_SELECT;
    }
  }
  if (dwFlags & AEREPT_SELECT)
  {
    if (bCaretAtStart)
      AE_SetSelectionPos(ae, &ciStart, &ciEnd, bColumnSel, AESELT_LOCKNOTIFY, 0);
    else
      AE_SetSelectionPos(ae, &ciEnd, &ciStart, bColumnSel, AESELT_LOCKNOTIFY, 0);
  }
  if (ciInsertStart) *ciInsertStart=ciStart;
  if (ciInsertEnd) *ciInsertEnd=ciEnd;
  AE_NotifyChanged(ae); //AETCT_REPLACESEL
}

INT_PTR AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, DWORD dwDeleteFlags)
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
  AEPOINT *lpRestartFrom;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  AEUNDOITEM *lpSetSelUndo=NULL;
  DWORD dwUndoFlags=0;
  DWORD dwCalcLinesWidthFlags=0;
  INT_PTR nRichTextCount=0;
  int nLinesForDelete;
  int nLastLineSelStart=0;
  int nLineCount=0;
  int nLineUnwrapCount=0;
  int nWrapCount=0;
  INT_PTR nHScrollPos;
  INT_PTR nVScrollPos;
  INT_PTR nStartOffset;
  INT_PTR nEndOffset;
  INT_PTR nDelLength;
  int nExtraStartOffset;
  int nExtraEndOffset;
  INT_PTR nLineOffsetNew;
  INT_PTR nLineOffsetOld;
  INT_PTR nLineDelStartOffsetOld;
  INT_PTR nLineDelEndOffsetOld;
  int nLineDelLength;
  int nElementLine;
  int nCompare;

  if (ciRangeStart->lpLine && ciRangeEnd->lpLine)
  {
    if (!bColumnSel)
    {
      AEC_ValidCharInLine(&ciDeleteStart);
      AEC_ValidCharInLine(&ciDeleteEnd);
    }

    //Exchange indexes
    if ((nCompare=AEC_IndexCompare(&ciDeleteStart, &ciDeleteEnd)) > 0)
    {
      ciTmp=ciDeleteStart;
      ciDeleteStart=ciDeleteEnd;
      ciDeleteEnd=ciTmp;
    }
    else if (nCompare == 0) return 0;

    //Set undo flags
    if (AEC_IndexCompare(&ae->ciCaretIndex, &ciDeleteEnd) < 0)
      dwUndoFlags=AEUN_CARETATSTART;

    //Force left-top to right-bottom selection
    if (bColumnSel)
    {
      if (ciDeleteStart.lpLine->nSelStart != ciDeleteStart.lpLine->nSelEnd)
        ciDeleteStart.nCharInLine=min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nSelStart);
      if (ciDeleteEnd.lpLine->nSelStart != ciDeleteEnd.lpLine->nSelEnd)
        ciDeleteEnd.nCharInLine=max(ciDeleteEnd.nCharInLine, ciDeleteEnd.lpLine->nSelEnd);
    }

    //Set AE_CalcLinesWidth flags
    if (dwDeleteFlags & AEDELT_LOCKUPDATEHSCROLL)
      dwCalcLinesWidthFlags=AECLW_LOCKUPDATEHSCROLL;

    nStartOffset=AE_AkelIndexToRichOffset(ae, &ciDeleteStart);
    nEndOffset=AE_AkelIndexToRichOffset(ae, &ciDeleteEnd);
    nDelLength=nEndOffset - nStartOffset;
    nLineOffsetNew=nStartOffset - min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nLineLen);
    nLineOffsetOld=nLineOffsetNew;
    nExtraStartOffset=max(ciDeleteStart.nCharInLine - ciDeleteStart.lpLine->nLineLen, 0);
    nExtraEndOffset=max(ciDeleteEnd.nCharInLine - ciDeleteEnd.lpLine->nLineLen, 0);
    nLinesForDelete=ciDeleteEnd.nLine - ciDeleteStart.nLine;

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
      lpPoint=ae->ptxt->hPointsStack.first;

      while (lpElement)
      {
        nLineDelStartOffsetOld=nLineOffsetOld + min(lpElement->nSelStart, lpElement->nLineLen);
        nLineDelEndOffsetOld=nLineOffsetOld + min(lpElement->nSelEnd, lpElement->nLineLen);
        nLineDelLength=(int)(nLineDelEndOffsetOld - nLineDelStartOffsetOld);

        if (lpNewElement=AE_StackLineInsertBefore(ae, ciDeleteStart.lpLine))
        {
          if (!lpFirstElement)
            lpFirstElement=lpNewElement;

          lpNewElement->nLineWidth=-1;
          lpNewElement->nLineBreak=lpElement->nLineBreak;
          lpNewElement->nLineLen=lpElement->nLineLen - nLineDelLength;
          if (dwDeleteFlags & AEDELT_SAVECOLUMNSEL)
          {
            lpNewElement->nSelStart=lpElement->nSelStart;
            lpNewElement->nSelEnd=lpElement->nSelStart;
          }
          nRichTextCount+=nLineDelLength;

          if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
          {
            xmemcpy(lpNewElement->wpLine, lpElement->wpLine, min(lpElement->nSelStart, lpNewElement->nLineLen) * sizeof(wchar_t));
            if (lpElement->nSelEnd < lpElement->nLineLen)
              xmemcpy(lpNewElement->wpLine + min(lpElement->nSelStart, lpNewElement->nLineLen), lpElement->wpLine + lpElement->nSelEnd, (lpElement->nLineLen - lpElement->nSelEnd) * sizeof(wchar_t));
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

            //Update points
            for (lpRestartFrom=NULL; lpPoint; lpPoint=lpPoint->next)
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
                lpPoint->dwFlags|=AEPTF_MOVEOFFSET;
              }

              //Offsets
              if (lpPoint->nPointOffset + lpPoint->nPointLen >= nLineOffsetOld)
              {
                if (lpPoint->nPointOffset < nLineDelEndOffsetOld)
                {
                  if (lpPoint->nPointOffset + lpPoint->nPointLen <= nLineDelStartOffsetOld)
                  {
                    //--<--->-[---]--
                    if (lpPoint->nTmpPointOffset == AEPTO_CALC)
                      lpPoint->nTmpPointOffset=lpPoint->nPointOffset - (nRichTextCount - nLineDelLength);
                  }
                  else
                  {
                    //----[--<-]->--
                    //--<-[----]->--
                    //--<-[->--]----
                    if (lpPoint->nTmpPointOffset == AEPTO_CALC)
                      lpPoint->nTmpPointOffset=min(lpPoint->nPointOffset, nLineDelStartOffsetOld) - (nRichTextCount - nLineDelLength);
                    if (lpPoint->nTmpPointLen == AEPTO_CALC)
                      lpPoint->nTmpPointLen=lpPoint->nPointLen;
                    lpPoint->nTmpPointLen-=(int)min(lpPoint->nPointOffset + lpPoint->nPointLen, nLineDelEndOffsetOld) - (int)max(lpPoint->nPointOffset, nLineDelStartOffsetOld);
                    lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_DELETE|AEPTF_NOTIFYDELETE;
                    if (!lpRestartFrom) lpRestartFrom=lpPoint;
                  }
                }
                else
                {
                  //--[---]-<--->--
                  if (lpPoint->nTmpPointOffset == AEPTO_CALC)
                    lpPoint->nTmpPointOffset=lpPoint->nPointOffset - nRichTextCount;

                  if (lpPoint->ciPoint.nLine > nElementLine && nElementLine != ciDeleteEnd.nLine)
                  {
                    //Parse next line
                    if (!lpRestartFrom) lpRestartFrom=lpPoint;
                    break;
                  }
                }
              }
            }
            if (lpRestartFrom) lpPoint=lpRestartFrom;

            //Add undo
            if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
            {
              if (!ae->ptxt->bLockCollectUndo)
              {
                if (ae->ptxt->dwUndoLimit)
                {
                  AEUNDOITEM *lpUndoElement;
                  wchar_t *wpUndoText;
                  UINT_PTR dwUndoTextLen=nLineDelLength;

                  if (dwUndoTextLen)
                  {
                    //Set selection
                    if (!lpSetSelUndo)
                    {
                      if (!(lpSetSelUndo=AE_StackUndoItemInsert(ae)))
                        break;
                    }

                    //Add text
                    if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
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
                        lpUndoElement->nNewLine=AELB_ASIS;
                      }
                      else break;
                    }
                    else break;
                  }
                }
              }
            }
          }
          else
          {
            AE_StackLineDelete(ae, lpNewElement);
            lpNewElement=NULL;
            break;
          }
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

      //Delete lines in range
      for (lpElement=ciDeleteStart.lpLine; lpElement; lpElement=lpNextElement)
      {
        if (lpElement == ciDeleteEnd.lpLine)
        {
          AE_StackLineDelete(ae, lpElement);
          break;
        }

        lpNextElement=lpElement->next;
        AE_StackLineDelete(ae, lpElement);
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
      ae->nSelEndCharOffset=nEndOffset - nRichTextCount;
      ae->ciSelEndIndex=ciLastChar;

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

        //Set control points to "delete from" position
        AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
        ae->ciCaretIndex=ciFirstChar;
        ae->nSelStartCharOffset=nStartOffset;
        ae->ciSelStartIndex=ciFirstChar;
        ae->nSelEndCharOffset=nEndOffset - nRichTextCount;
        ae->ciSelEndIndex=ciLastChar;

        //Update scroll bars
        ae->ptxt->nLineCount+=nWrapCount;
        ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
        if (nWrapCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATEVSCROLL))
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
                lpSetSelUndo->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_EXTRAOFFSET|AEUN_UNDOONLY|dwUndoFlags;
                lpSetSelUndo->nActionStartOffset=nStartOffset;
                lpSetSelUndo->nActionEndOffset=nEndOffset;
                lpSetSelUndo->nExtraStartOffset=nExtraStartOffset;
                lpSetSelUndo->nExtraEndOffset=nExtraEndOffset;
                lpSetSelUndo->wpText=NULL;
                lpSetSelUndo->dwTextLen=0;
                lpSetSelUndo->nNewLine=AELB_ASIS;
              }

              //Set redo selection
              {
                AEUNDOITEM *lpUndoElement;

                if (lpUndoElement=AE_StackUndoItemInsert(ae))
                {
                  lpUndoElement->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_EXTRAOFFSET|AEUN_REDOONLY|dwUndoFlags;
                  lpUndoElement->nActionStartOffset=nStartOffset;
                  lpUndoElement->nActionEndOffset=nStartOffset;
                  lpUndoElement->nExtraStartOffset=nExtraStartOffset;
                  lpUndoElement->nExtraEndOffset=nExtraStartOffset;
                  lpUndoElement->wpText=NULL;
                  lpUndoElement->dwTextLen=0;
                  lpUndoElement->nNewLine=AELB_ASIS;
                }
              }
            }
          }
        }
      }

      if (!(dwDeleteFlags & AEDELT_SAVECOLUMNSEL))
      {
        ae->nSelEndCharOffset=ae->nSelStartCharOffset;
        ae->ciSelEndIndex=ae->ciSelStartIndex;
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
            UINT_PTR dwUndoTextLen;

            if (dwUndoTextLen=AE_GetTextRange(ae, &ciDeleteStart, &ciDeleteEnd, NULL, 0, AELB_ASIS, bColumnSel, FALSE))
            {
              if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, dwUndoTextLen * sizeof(wchar_t)))
              {
                dwUndoTextLen=AE_GetTextRange(ae, &ciDeleteStart, &ciDeleteEnd, wpUndoText, (UINT_PTR)-1, AELB_ASIS, bColumnSel, FALSE);

                if (lpUndoElement=AE_StackUndoItemInsert(ae))
                {
                  lpUndoElement->dwFlags=AEUN_INSERT|dwUndoFlags;
                  lpUndoElement->nActionStartOffset=nStartOffset;
                  lpUndoElement->nActionEndOffset=nEndOffset;
                  lpUndoElement->wpText=wpUndoText;
                  lpUndoElement->dwTextLen=dwUndoTextLen;
                  lpUndoElement->nNewLine=AELB_ASIS;
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
        else
        {
          AE_StackLineDelete(ae, lpNewElement);
          lpNewElement=NULL;
        }
      }

      //Update points
      for (lpPoint=ae->ptxt->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
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
              lpPoint->dwFlags|=AEPTF_MOVEOFFSET;
            }
            if (lpPoint->ciPoint.nLine != ciDeleteStart.nLine)
              lpPoint->dwFlags|=AEPTF_MOVELINE;
            lpPoint->ciPoint.nLine=ciDeleteStart.nLine;
            lpPoint->ciPoint.lpLine=lpNewElement;
          }
          else
          {
            lpPoint->ciPoint.nLine-=nLinesForDelete;
            lpPoint->dwFlags|=AEPTF_MOVEOFFSET|(nLinesForDelete?AEPTF_MOVELINE:0);
          }
        }

        //Offsets
        if (lpPoint->nPointOffset >= 0)
        {
          if (lpPoint->nPointOffset < nEndOffset)
          {
            if (lpPoint->nPointOffset + lpPoint->nPointLen > nStartOffset)
            {
              lpPoint->nPointLen-=(int)min(lpPoint->nPointOffset + lpPoint->nPointLen, nEndOffset) - (int)max(lpPoint->nPointOffset, nStartOffset);
              lpPoint->nPointOffset=min(lpPoint->nPointOffset, nStartOffset);
              lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_DELETE|AEPTF_NOTIFYDELETE;
            }
          }
          else lpPoint->nPointOffset-=nDelLength;
        }
      }

      //Delete lines in range
      for (lpElement=ciDeleteStart.lpLine; lpElement; lpElement=lpNextElement)
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
      ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
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

        //Set control points to "delete from" position
        AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
        ae->ciCaretIndex=ciFirstChar;
        ae->nSelStartCharOffset=nStartOffset;
        ae->ciSelStartIndex=ciFirstChar;
        ae->nSelEndCharOffset=nStartOffset;
        ae->ciSelEndIndex=ciFirstChar;

        //Update scroll bars
        ae->ptxt->nLineCount+=nWrapCount;
        ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
        if (nLineCount + nWrapCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATEVSCROLL))
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
        //Update scroll bars
        if (nLineCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATEVSCROLL))
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
      AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
    }
    if (!(dwDeleteFlags & AEDELT_LOCKUPDATECARET))
    {
      //Set caret position
      ae->nCaretHorzIndent=ae->ptCaret.x;
      if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
    }
    if (!(dwDeleteFlags & AEDELT_LOCKUPDATETEXT))
    {
      //Redraw lines
      if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }
      else
      {
        AE_RedrawIndexRange(ae, &ciFirstChar, (nLineCount + nWrapCount)?NULL:&ciLastChar, TRUE);
      }
    }

    if (nRichTextCount > 0)
    {
      ae->dwNotifyFlags|=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
      if (!ae->ptxt->nLastCharOffset)
        ae->dwNotifyTextChange|=AETCT_DELETEALL;
    }

    //Send AEN_POINT
    AE_StackPointUnreserve(ae);

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

UINT_PTR AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, const wchar_t *wpText, UINT_PTR dwTextLen, int nNewLine, BOOL bColumnSel, DWORD dwInsertFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AECHARINDEX ciInsertFrom=*ciInsertPos;
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpNewElement=NULL;
  AELINEDATA *lpNextElement=NULL;
  AELINEDATA *lpInsertToElement=NULL;
  AEPOINT *lpPoint;
  AEPOINT *lpRestartFrom;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  POINT64 ptInsertFrom;
  wchar_t *wpLineStart=(wchar_t *)wpText;
  wchar_t *wpLineEnd;
  UINT_PTR dwTextCount=0;
  UINT_PTR dwRichTextCount=0;
  DWORD dwCalcLinesWidthFlags=0;
  DWORD dwUndoFlags=0;
  BYTE nLineBreak;
  int nLineLen=0;
  int nLineCount=0;
  int nWrapCount=0;
  int nCaretIndexInLine=0;
  int nSpaces=0;
  INT_PTR nHScrollPos=0;
  INT_PTR nVScrollPos=0;
  INT_PTR nInsertOffset;
  int nInsertCharInLine;
  INT_PTR nInsertOffsetInLine;
  INT_PTR nStartOffset;
  INT_PTR nEndOffset;
  INT_PTR nLineOffsetNew;
  INT_PTR nLineOffsetOld;
  int i;

  if (ciInsertFrom.lpLine)
  {
    if (dwTextLen == (UINT_PTR)-1)
      dwTextLen=xstrlenW(wpText);

    if (ae->ptxt->dwTextLimit >= (UINT_PTR)ae->ptxt->nLastCharOffset)
    {
      if (ae->ptxt->dwTextLimit - ae->ptxt->nLastCharOffset < dwTextLen)
      {
        dwTextLen=ae->ptxt->dwTextLimit - ae->ptxt->nLastCharOffset;
        ae->dwNotifyFlags|=AENM_MAXTEXT;
      }
    }
    else dwTextLen=0;

    if (!(ae->popt->dwOptions & AECO_MULTILINE))
      AE_GetNextLine(ae, wpLineStart, (DWORD)dwTextLen, (int *)&dwTextLen, NULL);

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

      //Set undo flags
      if (AEC_IndexCompare(&ae->ciCaretIndex, &ciInsertFrom) < 0)
        dwUndoFlags=AEUN_CARETATSTART;

      //Set AE_CalcLinesWidth flags
      if (dwInsertFlags & AEINST_LOCKUPDATEHSCROLL)
        dwCalcLinesWidthFlags=AECLW_LOCKUPDATEHSCROLL;

      nInsertOffset=AE_AkelIndexToRichOffset(ae, &ciInsertFrom);
      nInsertCharInLine=ciInsertFrom.nCharInLine;
      nLineOffsetNew=nInsertOffset - min(nInsertCharInLine, ciInsertFrom.lpLine->nLineLen);
      nLineOffsetOld=nLineOffsetNew;
      nStartOffset=nLineOffsetNew + nInsertCharInLine;
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
        ciFirstChar.nCharInLine=nInsertCharInLine;
        ciLastChar.nLine=ciInsertFrom.nLine;
        ciLastChar.lpLine=NULL;
        ciLastChar.nCharInLine=nInsertCharInLine;
        lpElement=ciInsertFrom.lpLine;
        lpPoint=ae->ptxt->hPointsStack.first;

        while (dwTextCount <= dwTextLen)
        {
          if (lpNewElement=AE_StackLineInsertBefore(ae, ciInsertFrom.lpLine))
          {
            wpLineEnd=AE_GetNextLine(ae, wpLineStart, (DWORD)(dwTextLen - dwTextCount), &nLineLen, &nLineBreak);
            dwTextCount+=wpLineEnd - wpLineStart;
            dwRichTextCount+=nLineLen;

            if (nLineBreak != AELB_EOF)
            {
              if (nNewLine != AELB_ASIS)
                nLineBreak=(BYTE)nNewLine;
            }

            if (lpElement)
            {
              if (!ciFirstChar.lpLine)
              {
                //First insert line
                ciFirstChar.lpLine=lpNewElement;
              }
              else
              {
                //Second and next lines. AE_WrapLines called in AE_DeleteTextRange sets nSelStart to zero, so check is line wrapped.
                if (ciLastChar.nLine <= ae->ciSelEndIndex.nLine && (lpElement->nSelStart || lpElement->prev->nLineBreak != AELB_WRAP))
                  nInsertCharInLine=lpElement->nSelStart;
                else
                  AE_GetCharInLine(ae, lpElement, ptInsertFrom.x, AECIL_HALFFIT|AECIL_ALLPOS, &nInsertCharInLine, NULL, bColumnSel);
              }
              nInsertOffsetInLine=nLineOffsetOld + min(nInsertCharInLine, lpElement->nLineLen);

              lpNewElement->nLineWidth=-1;
              if (lpElement->nLineBreak != AELB_EOF)
                lpNewElement->nLineBreak=lpElement->nLineBreak;
              else
                lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=lpElement->nLineLen + max(nInsertCharInLine - lpElement->nLineLen, 0) + nLineLen;
              nCaretIndexInLine=nInsertCharInLine + nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                xmemcpy(lpNewElement->wpLine, lpElement->wpLine, min(nInsertCharInLine, lpElement->nLineLen) * sizeof(wchar_t));

                if (nInsertCharInLine > lpElement->nLineLen)
                {
                  dwRichTextCount+=nInsertCharInLine - lpElement->nLineLen;

                  for (i=lpElement->nLineLen; i < nInsertCharInLine; ++i)
                    lpNewElement->wpLine[i]=L' ';
                  xmemcpy(lpNewElement->wpLine + nInsertCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                }
                else
                {
                  xmemcpy(lpNewElement->wpLine + nInsertCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  xmemcpy(lpNewElement->wpLine + nInsertCharInLine + nLineLen, lpElement->wpLine + nInsertCharInLine, (lpElement->nLineLen - nInsertCharInLine) * sizeof(wchar_t));
                }

                //Update points
                for (lpRestartFrom=NULL; lpPoint; lpPoint=lpPoint->next)
                {
                  if (lpPoint->ciPoint.nLine > ciLastChar.nLine && nLineBreak != AELB_EOF)
                    break;

                  if (lpPoint->ciPoint.nLine == ciLastChar.nLine)
                  {
                    lpPoint->ciPoint.lpLine=lpNewElement;
                    if (lpPoint->ciPoint.nCharInLine >= nInsertCharInLine)
                      lpPoint->ciPoint.nCharInLine+=nLineLen;
                    lpPoint->dwFlags|=AEPTF_MOVEOFFSET;
                  }

                  //Offsets
                  if (lpPoint->nPointOffset + lpPoint->nPointLen >= nLineOffsetOld)
                  {
                    if (lpPoint->nPointOffset < nInsertOffsetInLine)
                    {
                      if (lpPoint->nTmpPointOffset == AEPTO_CALC)
                        lpPoint->nTmpPointOffset=lpPoint->nPointOffset + dwRichTextCount - (lpNewElement->nLineLen - lpElement->nLineLen);

                      if (lpPoint->nPointOffset + lpPoint->nPointLen <= nInsertOffsetInLine)
                      {
                        //--<--->--|--
                      }
                      else
                      {
                        //--<--|-->--
                        if (lpPoint->nTmpPointLen == AEPTO_CALC)
                          lpPoint->nTmpPointLen=lpPoint->nPointLen;
                        lpPoint->nTmpPointLen+=nLineLen;
                        lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_INSERT|AEPTF_NOTIFYINSERT;
                        if (!lpRestartFrom) lpRestartFrom=lpPoint;
                      }
                    }
                    else
                    {
                      //--|--<--->--
                      if (lpPoint->nTmpPointOffset == AEPTO_CALC)
                        lpPoint->nTmpPointOffset=lpPoint->nPointOffset + dwRichTextCount;

                      if (lpPoint->ciPoint.nLine > ciLastChar.nLine && nLineBreak != AELB_EOF)
                      {
                        //Parse next line
                        if (!lpRestartFrom) lpRestartFrom=lpPoint;
                        break;
                      }
                    }
                  }
                }
                if (lpRestartFrom) lpPoint=lpRestartFrom;

                //Add undo
                if (!(dwInsertFlags & AEINST_LOCKUNDO))
                {
                  if (!ae->ptxt->bLockCollectUndo)
                  {
                    if (ae->ptxt->dwUndoLimit)
                    {
                      AEUNDOITEM *lpUndoElement;
                      wchar_t *wpUndoText;
                      UINT_PTR dwUndoTextLen=max(nInsertCharInLine - lpElement->nLineLen, 0) + nLineLen;

                      if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
                      {
                        nSpaces=nInsertCharInLine - lpElement->nLineLen;

                        for (i=0; i < nSpaces; ++i)
                          wpUndoText[i]=L' ';
                        xmemcpy(wpUndoText + i, wpLineStart, nLineLen * sizeof(wchar_t));
                        wpUndoText[dwUndoTextLen]=L'\0';

                        if (lpUndoElement=AE_StackUndoItemInsert(ae))
                        {
                          lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                          lpUndoElement->nActionStartOffset=nLineOffsetNew + min(nInsertCharInLine, lpElement->nLineLen);
                          lpUndoElement->nActionEndOffset=nLineOffsetNew + nInsertCharInLine + nLineLen;
                          lpUndoElement->wpText=wpUndoText;
                          lpUndoElement->dwTextLen=dwUndoTextLen;
                          lpUndoElement->nNewLine=nNewLine;
                        }
                        else break;
                      }
                      else break;
                    }
                  }
                }
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
              }
              else
              {
                AE_StackLineDelete(ae, lpNewElement);
                lpNewElement=NULL;
                break;
              }
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
                        const wchar_t *wpLineBreak;
                        UINT_PTR dwUndoTextLen=AE_GetNewLineString(lpNewElement->nLineBreak, &wpLineBreak);

                        if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
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
                            lpUndoElement->nNewLine=nNewLine;
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
              if (ae->ptxt->nFixedCharWidth)
                nInsertCharInLine=(int)(ptInsertFrom.x / ae->ptxt->nFixedCharWidth);
              else
                nInsertCharInLine=(int)(ptInsertFrom.x / ae->ptxt->nSpaceCharWidth);

              lpNewElement->nLineWidth=-1;
              lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=nInsertCharInLine + nLineLen;
              nCaretIndexInLine=nInsertCharInLine + nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                dwRichTextCount+=nInsertCharInLine;
                if (nLineBreak != AELB_EOF)
                  ++dwRichTextCount;

                for (i=0; i < nInsertCharInLine; ++i)
                  lpNewElement->wpLine[i]=L' ';
                xmemcpy(lpNewElement->wpLine + nInsertCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));

                //Add undo
                if (!(dwInsertFlags & AEINST_LOCKUNDO))
                {
                  if (!ae->ptxt->bLockCollectUndo)
                  {
                    if (ae->ptxt->dwUndoLimit)
                    {
                      AEUNDOITEM *lpUndoElement;
                      wchar_t *wpUndoText;
                      UINT_PTR dwUndoTextLen=lpNewElement->nLineLen + 1;

                      if (nLineBreak == AELB_EOF)
                        --dwUndoTextLen;

                      if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (dwUndoTextLen + 1) * sizeof(wchar_t)))
                      {
                        nSpaces=nInsertCharInLine;

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
                          lpUndoElement->nNewLine=nNewLine;
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
              else
              {
                AE_StackLineDelete(ae, lpNewElement);
                lpNewElement=NULL;
                break;
              }
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

        if (dwTextCount)
        {
          //Last insert line
          ciLastChar.lpLine=lpNewElement;
          ciLastChar.nCharInLine=nCaretIndexInLine;

          //Delete lines in range
          for (lpElement=ciInsertFrom.lpLine; lpElement; lpElement=lpNextElement)
          {
            if (lpElement == lpInsertToElement)
            {
              AE_StackLineDelete(ae, lpElement);
              break;
            }

            lpNextElement=lpElement->next;
            AE_StackLineDelete(ae, lpElement);
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
          ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
          AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
          ae->ciCaretIndex=ciFirstChar;
          ae->nSelStartCharOffset=nStartOffset;
          ae->ciSelStartIndex=ciFirstChar;
          ae->nSelEndCharOffset=nStartOffset;
          ae->ciSelEndIndex=ciFirstChar;
          nEndOffset=nLineOffsetNew + nInsertCharInLine + nLineLen;

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

            //Set control points to "insert from" position
            AE_GetPosFromChar(ae, &ciFirstChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciFirstChar;
            ae->nSelStartCharOffset=nStartOffset;
            ae->ciSelStartIndex=ciFirstChar;
            ae->nSelEndCharOffset=nStartOffset;
            ae->ciSelEndIndex=ciFirstChar;

            //Update scroll bars
            ae->ptxt->nLineCount+=nWrapCount;
            ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
            if (nLineCount + nWrapCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATEVSCROLL))
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
            //Update scroll bars
            if (nLineCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATEVSCROLL))
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
                AEUNDOITEM *lpUndoElement;

                if (lpUndoElement=AE_StackUndoItemInsert(ae))
                {
                  lpUndoElement->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_REDOONLY|dwUndoFlags;
                  lpUndoElement->nActionStartOffset=nStartOffset;
                  lpUndoElement->nActionEndOffset=nEndOffset;
                  lpUndoElement->wpText=NULL;
                  lpUndoElement->dwTextLen=0;
                  lpUndoElement->nNewLine=nNewLine;
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
            wpLineEnd=AE_GetNextLine(ae, wpLineStart, (DWORD)(dwTextLen - dwTextCount), &nLineLen, &nLineBreak);
            dwTextCount+=wpLineEnd - wpLineStart;
            dwRichTextCount+=nLineLen + 1;

            if (nLineBreak != AELB_EOF)
            {
              if (nNewLine != AELB_ASIS)
                nLineBreak=(BYTE)nNewLine;
            }
            else --dwRichTextCount;

            //First line
            if (!nLineCount)
            {
              //Start index
              ciFirstChar.nLine=ciInsertFrom.nLine;
              ciFirstChar.lpLine=lpNewElement;
              ciFirstChar.nCharInLine=min(nInsertCharInLine, ciInsertFrom.lpLine->nLineLen);

              if (nLineBreak == AELB_EOF)
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                lpNewElement->nLineLen=ciInsertFrom.lpLine->nLineLen + max(nInsertCharInLine - ciInsertFrom.lpLine->nLineLen, 0) + nLineLen;
                nCaretIndexInLine=nInsertCharInLine + nLineLen;
              }
              else
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=nLineBreak;
                lpNewElement->nLineLen=min(nInsertCharInLine, ciInsertFrom.lpLine->nLineLen) + max(nInsertCharInLine - ciInsertFrom.lpLine->nLineLen, 0) + nLineLen;
                nCaretIndexInLine=0;
              }

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
              {
                xmemcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine, min(nInsertCharInLine, ciInsertFrom.lpLine->nLineLen) * sizeof(wchar_t));

                if (nInsertCharInLine > ciInsertFrom.lpLine->nLineLen)
                {
                  dwRichTextCount+=nInsertCharInLine - ciInsertFrom.lpLine->nLineLen;
                  nSpaces=nInsertCharInLine - ciInsertFrom.lpLine->nLineLen;

                  for (i=ciInsertFrom.lpLine->nLineLen; i < nInsertCharInLine; ++i)
                    lpNewElement->wpLine[i]=L' ';
                }

                if (nLineBreak == AELB_EOF)
                {
                  //wpText - one line
                  xmemcpy(lpNewElement->wpLine + nInsertCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  if (nInsertCharInLine < ciInsertFrom.lpLine->nLineLen)
                    xmemcpy(lpNewElement->wpLine + nInsertCharInLine + nLineLen, ciInsertFrom.lpLine->wpLine + nInsertCharInLine, (ciInsertFrom.lpLine->nLineLen - nInsertCharInLine) * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                }
                else
                {
                  xmemcpy(lpNewElement->wpLine + nInsertCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

                  if (dwTextCount >= dwTextLen)
                  {
                    //End of line as new line
                    if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
                    {
                      lpNewElement->nLineWidth=-1;
                      lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                      lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - nInsertCharInLine, 0);
                      nCaretIndexInLine=0;

                      if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                      {
                        if (nInsertCharInLine < ciInsertFrom.lpLine->nLineLen)
                          xmemcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + nInsertCharInLine, (ciInsertFrom.lpLine->nLineLen - nInsertCharInLine) * sizeof(wchar_t));
                        lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                      }
                      else break;
                    }
                    else break;
                  }
                }
              }
              else
              {
                AE_StackLineDelete(ae, lpNewElement);
                lpNewElement=NULL;
                break;
              }
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
                lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - nInsertCharInLine, 0) + nLineLen;
                nCaretIndexInLine=nLineLen;

                if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                {
                  xmemcpy(lpNewElement->wpLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  if (nInsertCharInLine < ciInsertFrom.lpLine->nLineLen)
                    xmemcpy(lpNewElement->wpLine + nLineLen, ciInsertFrom.lpLine->wpLine + nInsertCharInLine, (ciInsertFrom.lpLine->nLineLen - nInsertCharInLine) * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                }
                else
                {
                  AE_StackLineDelete(ae, lpNewElement);
                  lpNewElement=NULL;
                  break;
                }
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
                else
                {
                  AE_StackLineDelete(ae, lpNewElement);
                  lpNewElement=NULL;
                  break;
                }

                //End of line as new line
                if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
                {
                  lpNewElement->nLineWidth=-1;
                  lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                  lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - nInsertCharInLine, 0);
                  nCaretIndexInLine=0;

                  if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, (lpNewElement->nLineLen + 1) * sizeof(wchar_t)))
                  {
                    if (nInsertCharInLine < ciInsertFrom.lpLine->nLineLen)
                      xmemcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + nInsertCharInLine, (ciInsertFrom.lpLine->nLineLen - nInsertCharInLine) * sizeof(wchar_t));
                    lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                  }
                  else
                  {
                    AE_StackLineDelete(ae, lpNewElement);
                    lpNewElement=NULL;
                    break;
                  }
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
              else
              {
                AE_StackLineDelete(ae, lpNewElement);
                lpNewElement=NULL;
                break;
              }
              wpLineStart=wpLineEnd;
            }
          }
          else break;
        }

        if (dwTextCount)
        {
          //Update points
          for (lpPoint=ae->ptxt->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
          {
            //if (lpPoint->ciPoint.nLine < ciInsertFrom.nLine)
            //  break;

            if (lpPoint->ciPoint.nLine > ciInsertFrom.nLine)
            {
              lpPoint->ciPoint.nLine+=nLineCount;
              lpPoint->dwFlags|=AEPTF_MOVEOFFSET|(nLineCount?AEPTF_MOVELINE:0);
            }
            else if (lpPoint->ciPoint.nLine == ciInsertFrom.nLine)
            {
              if (lpPoint->ciPoint.nCharInLine < nInsertCharInLine)
              {
                lpPoint->ciPoint.lpLine=ciFirstChar.lpLine;
              }
              else
              {
                lpPoint->ciPoint.nLine+=nLineCount;
                lpPoint->ciPoint.lpLine=lpNewElement;
                lpPoint->ciPoint.nCharInLine=nCaretIndexInLine + (lpPoint->ciPoint.nCharInLine - nInsertCharInLine);
                lpPoint->dwFlags|=AEPTF_MOVEOFFSET|(nLineCount?AEPTF_MOVELINE:0);
              }
            }

            //Offsets
            if (lpPoint->nPointOffset >= 0)
            {
              if (lpPoint->nPointOffset < nInsertOffset)
              {
                if (lpPoint->nPointOffset + lpPoint->nPointLen > nInsertOffset)
                {
                  lpPoint->nPointLen+=(int)dwRichTextCount;
                  lpPoint->dwFlags|=AEPTF_MODIFY|AEPTF_INSERT|AEPTF_NOTIFYINSERT;
                }
              }
              else lpPoint->nPointOffset+=dwRichTextCount;
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
          ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
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

            //Set control points to "insert to" position
            AE_GetPosFromChar(ae, &ciLastChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciLastChar;
            ae->nSelStartCharOffset=nEndOffset;
            ae->ciSelStartIndex=ciLastChar;
            ae->nSelEndCharOffset=nEndOffset;
            ae->ciSelEndIndex=ciLastChar;

            //Update scroll bars
            ae->ptxt->nLineCount+=nWrapCount;
            ae->ptxt->nVScrollMax=AE_GetVScrollMax(ae);
            if (nLineCount + nWrapCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATEVSCROLL))
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
            AE_GetPosFromChar(ae, &ciLastChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciLastChar;
            ae->nSelStartCharOffset=nEndOffset;
            ae->ciSelStartIndex=ciLastChar;
            ae->nSelEndCharOffset=nEndOffset;
            ae->ciSelEndIndex=ciLastChar;

            //Update scroll bars
            if (nLineCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATEVSCROLL))
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
                  if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (nSpaces + 1) * sizeof(wchar_t)))
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
                      lpUndoElement->nNewLine=nNewLine;
                    }
                  }
                }

                if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae->aeUndo, 0, (dwTextLen + 1) * sizeof(wchar_t)))
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
                    lpUndoElement->nNewLine=nNewLine;
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
          AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
        }
        if (!(dwInsertFlags & AEINST_LOCKUPDATECARET))
        {
          //Set caret position
          ae->nCaretHorzIndent=ae->ptCaret.x;
          if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
        }
        if (!(dwInsertFlags & AEINST_LOCKUPDATETEXT))
        {
          //Redraw lines
          if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
          {
            InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
          }
          else
          {
            AE_RedrawIndexRange(ae, &ciFirstChar, (nLineCount + nWrapCount)?NULL:&ciLastChar, TRUE);
          }
        }

        ae->dwNotifyFlags|=AENM_SELCHANGE|AENM_TEXTCHANGE|AENM_MODIFY;
      }

      //Send AEN_POINT
      AE_StackPointUnreserve(ae);

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

wchar_t* AE_GetNextLine(AKELEDIT *ae, const wchar_t *wpText, DWORD dwTextLen, int *nLineLen, BYTE *nLineBreak)
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

DWORD AE_GetLinesCount(const wchar_t *wpText, INT_PTR nTextLen)
{
  const wchar_t *wpEnd=wpText + nTextLen;
  DWORD dwLines=1;

  while (wpText < wpEnd)
  {
    if (*wpText == L'\r' || *wpText == L'\n')
    {
      if (*wpText == L'\r' && *(wpText + 1) == L'\n')
        wpText+=2;
      else if (*wpText == L'\r' && *(wpText + 1) == L'\r' && *(wpText + 2) == L'\n')
        wpText+=3;
      else if (*wpText == L'\n')
        wpText+=1;
      else if (*wpText == L'\r')
        wpText+=1;
      if (wpText < wpEnd) ++dwLines;
    }
    else ++wpText;
  }
  return dwLines;
}

int AE_GetNewLineString(int nNewLine, const wchar_t **wpNewLine)
{
  if (nNewLine == AELB_RN)
  {
    if (wpNewLine) *wpNewLine=L"\r\n";
    return 2;
  }
  else if (nNewLine == AELB_N)
  {
    if (wpNewLine) *wpNewLine=L"\n";
    return 1;
  }
  else if (nNewLine == AELB_R)
  {
    if (wpNewLine) *wpNewLine=L"\r";
    return 1;
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
  AEFINDTEXTW ftW;
  wchar_t *wszText;
  UINT_PTR dwUnicodeLen;
  BOOL bResult=FALSE;

  if (dwUnicodeLen=MultiByteToWideChar64(nCodePage, 0, ftA->pText, ftA->dwTextLen, NULL, 0))
  {
    if (ftA->dwTextLen == (UINT_PTR)-1)
      dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar64(nCodePage, 0, ftA->pText, ftA->dwTextLen, wszText, dwUnicodeLen + 1);
      ftW.dwFlags=ftA->dwFlags;
      ftW.pText=wszText;
      ftW.dwTextLen=dwUnicodeLen;
      ftW.nNewLine=ftA->nNewLine;
      ftW.crSearch.ciMin=ftA->crSearch.ciMin;
      ftW.crSearch.ciMax=ftA->crSearch.ciMax;
      bResult=AE_FindText(ae, &ftW);
      ftA->crFound.ciMin=ftW.crFound.ciMin;
      ftA->crFound.ciMax=ftW.crFound.ciMax;

      AE_HeapFree(ae, 0, (LPVOID)wszText);
    }
  }
  return bResult;
}

BOOL AE_FindText(AKELEDIT *ae, AEFINDTEXTW *ft)
{
  AECHARINDEX ciCount;
  AECHARINDEX ciCountEnd;

  if (ft->dwTextLen == (UINT_PTR)-1)
    ft->dwTextLen=xstrlenW(ft->pText);
  if (!ft->dwTextLen)
    return FALSE;

  if (ft->dwFlags & AEFR_REGEXP)
  {
    STACKREGROUP hREGroupStack;
    BOOL bFound=FALSE;

    hREGroupStack.first=0;
    hREGroupStack.last=0;
    hREGroupStack.dwOptions=(ft->dwFlags & AEFR_MATCHCASE?REO_MATCHCASE:0) |
                            (ft->dwFlags & AEFR_REGEXPNONEWLINEDOT?REO_NONEWLINEDOT:0) |
                             REO_MULTILINE;
    hREGroupStack.wpDelim=ae->popt->wszWordDelimiters;
    hREGroupStack.wpMaxDelim=ae->popt->wszWordDelimiters + ae->popt->nWordDelimitersLen;

    if (!(ft->nCompileErrorOffset=PatCompile(&hREGroupStack, ft->pText, ft->pText + ft->dwTextLen)))
    {
      ciCount=ft->crSearch.ciMin;
      ciCountEnd=ft->crSearch.ciMax;

      while (AE_PatExec(&hREGroupStack, hREGroupStack.first, &ciCount, &ciCountEnd))
      {
        if ((!(ft->dwFlags & AEFR_WHOLEWORD) ||
             (AE_IsDelimiter(ae, &hREGroupStack.first->ciStrStart, AEDLM_WORD|AEDLM_PREVCHAR) &&
              AE_IsDelimiter(ae, &hREGroupStack.first->ciStrEnd, AEDLM_WORD))) &&
            (hREGroupStack.first->nStrLen || ((ft->dwFlags & AEFR_REGEXPMINMATCH) ||
             ((ft->dwFlags & AEFR_DOWN) ? AEC_IndexCompare(&ft->crSearch.ciMin, &hREGroupStack.first->ciStrEnd) :
                                          AEC_IndexCompare(&ft->crSearch.ciMax, &hREGroupStack.first->ciStrStart)))))
        {
          ft->crFound.ciMin=hREGroupStack.first->ciStrStart;
          ft->crFound.ciMax=hREGroupStack.first->ciStrEnd;
          bFound=TRUE;
          if (ft->dwFlags & AEFR_DOWN) break;
        }

        //Next match
        if (AEC_IndexCompare(&ciCount, &hREGroupStack.first->ciStrEnd) < 0)
          ciCount=hREGroupStack.first->ciStrEnd;
        else if (!AEC_NextChar(&ciCount)) //ft->pText == "^" or "$"
          break;

        if (AEC_IndexCompare(&ciCount, &ciCountEnd) > 0)
          break;
        //AE_PatReset(&hREGroupStack);
      }
      PatFree(&hREGroupStack);
    }
    return bFound;
  }
  else
  {
    if (ft->dwFlags & AEFR_DOWN)
    {
      if (AEC_IndexCompare(&ft->crSearch.ciMin, &ft->crSearch.ciMax) <= 0)
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
      if (AEC_IndexCompare(&ft->crSearch.ciMin, &ft->crSearch.ciMax) <= 0)
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
      if (AE_IndexOffset(ae, &ciCountEnd, &ciCountEnd, -(INT_PTR)ft->dwTextLen, ft->nNewLine))
      {
        for (;;)
        {
          while (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
          {
            if (AEC_IndexCompare(&ciCount, &ciCountEnd) > 0)
              return FALSE;

            if (AE_IsMatch(ae, ft, &ciCount))
              return TRUE;

            AEC_IndexInc(&ciCount);
          }

          if (ciCount.lpLine->next)
            AEC_NextLine(&ciCount);
          else
            return FALSE;
        }
      }
    }
    else
    {
      if (AE_IndexOffset(ae, &ciCount, &ciCount, -(INT_PTR)ft->dwTextLen, ft->nNewLine))
      {
        for (;;)
        {
          while (ciCount.nCharInLine >= 0)
          {
            if (AEC_IndexCompare(&ciCount, &ciCountEnd) < 0)
              return FALSE;

            if (AE_IsMatch(ae, ft, &ciCount))
              return TRUE;

            AEC_IndexDec(&ciCount);
          }

          if (ciCount.lpLine->prev)
            AEC_PrevLine(&ciCount);
          else
            return FALSE;
        }
      }
    }
  }
  return FALSE;
}

UINT_PTR AE_IsMatchAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA, const AECHARINDEX *ciChar)
{
  AEFINDTEXTW ftW;
  wchar_t *wszText;
  UINT_PTR dwUnicodeLen;
  UINT_PTR dwResult=0;

  if (dwUnicodeLen=MultiByteToWideChar64(nCodePage, 0, ftA->pText, ftA->dwTextLen, NULL, 0))
  {
    if (ftA->dwTextLen == (UINT_PTR)-1)
      dwUnicodeLen-=1;

    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
    {
      MultiByteToWideChar64(nCodePage, 0, ftA->pText, ftA->dwTextLen, wszText, dwUnicodeLen + 1);
      ftW.dwFlags=ftA->dwFlags;
      ftW.pText=wszText;
      ftW.dwTextLen=dwUnicodeLen;
      ftW.nNewLine=ftA->nNewLine;
      ftW.crSearch.ciMin=ftA->crSearch.ciMin;
      ftW.crSearch.ciMax=ftA->crSearch.ciMax;
      dwResult=AE_IsMatch(ae, &ftW, ciChar);
      ftA->crFound.ciMin=ftW.crFound.ciMin;
      ftA->crFound.ciMax=ftW.crFound.ciMax;

      AE_HeapFree(ae, 0, (LPVOID)wszText);
    }
  }
  return dwResult;
}

UINT_PTR AE_IsMatch(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount;
  UINT_PTR dwCount;

  if (ft->dwFlags & AEFR_WHOLEWORD)
  {
    if (!AE_IsDelimiter(ae, ciChar, AEDLM_WORD|AEDLM_PREVCHAR))
      return 0;
  }
  ciCount=*ciChar;
  dwCount=0;

  if (ft->dwFlags & AEFR_REGEXP)
  {
    STACKREGROUP hREGroupStack;
    AECHARINDEX ciCountEnd;

    hREGroupStack.first=0;
    hREGroupStack.last=0;
    hREGroupStack.dwOptions=(ft->dwFlags & AEFR_MATCHCASE?REO_MATCHCASE:0) |
                            (ft->dwFlags & AEFR_REGEXPNONEWLINEDOT?REO_NONEWLINEDOT:0) |
                             REO_MULTILINE;
    hREGroupStack.wpDelim=ae->popt->wszWordDelimiters;
    hREGroupStack.wpMaxDelim=ae->popt->wszWordDelimiters + ae->popt->nWordDelimitersLen;

    if (!(ft->nCompileErrorOffset=PatCompile(&hREGroupStack, ft->pText, ft->pText + ft->dwTextLen)))
    {
      hREGroupStack.first->dwFlags&=~REGF_ROOTANY;
      AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciCountEnd);

      if (AE_PatExec(&hREGroupStack, hREGroupStack.first, &ciCount, &ciCountEnd))
      {
        dwCount=hREGroupStack.first->nStrLen;
        ciCount=hREGroupStack.first->ciStrEnd;
      }
      PatFree(&hREGroupStack);
    }
    if (!dwCount) return 0;
  }
  else
  {
    int nNewLine;
    BYTE nLineBreak;

    //Set new line
    nNewLine=ft->nNewLine;

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

        if (ft->dwTextLen == (UINT_PTR)-1)
        {
          if (!ft->pText[++dwCount])
          {
            ++ciCount.nCharInLine;
            goto Found;
          }
        }
        else if (++dwCount >= ft->dwTextLen)
        {
          ++ciCount.nCharInLine;
          goto Found;
        }
      }

      if (ciCount.lpLine->nLineBreak != AELB_WRAP)
      {
        if (ciCount.lpLine->nLineBreak == AELB_EOF)
          return 0;
        if (nNewLine == AELB_ASIS)
          nLineBreak=ciCount.lpLine->nLineBreak;
        else
          nLineBreak=(BYTE)nNewLine;

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
        AEC_NextLine(&ciCount);
      else
        return 0;

      if (ft->dwTextLen == (UINT_PTR)-1)
      {
        if (!ft->pText[dwCount])
          goto Found;
      }
      else
      {
        if (dwCount >= ft->dwTextLen)
          goto Found;
      }
    }
  }

  Found:
  if (ft->dwFlags & AEFR_WHOLEWORD)
  {
    if (!AE_IsDelimiter(ae, &ciCount, AEDLM_WORD))
      return 0;
  }
  ft->crFound.ciMin=*ciChar;
  ft->crFound.ciMax=ciCount;
  return dwCount;
}

UINT_PTR AE_IsMatchRE(void *lpREGroupStack, AECHARRANGE *crFound, const AECHARINDEX *ciChar, const AECHARINDEX *ciMaxRE)
{
  STACKREGROUP *sreg=(STACKREGROUP *)lpREGroupStack;
  AECHARINDEX ciStr=*ciChar;
  UINT_PTR dwCount=0;

  if (!sreg->first) return 0;

  if (AE_PatExec(sreg, sreg->first, &ciStr, ciMaxRE))
  {
    crFound->ciMin=sreg->first->ciStrStart;
    crFound->ciMax=sreg->first->ciStrEnd;
    dwCount=sreg->first->nStrLen;
  }
  AE_PatReset(sreg);

  return dwCount;
}

void AE_UpdateCompositionPos(AKELEDIT *ae, HIMC hImc)
{
  COMPOSITIONFORM cf;
  HIMC hImcCur=hImc;

  if (hImcCur || (hImcCur=ImmGetContext(ae->hWndEdit)))
  {
    cf.dwStyle=CFS_POINT;
    if (AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
      AE_GetPosFromChar(ae, &ae->ciSelStartIndex, NULL, &cf.ptCurrentPos);
    else
      AE_GlobalToClient(ae, &ae->ptCaret, NULL, &cf.ptCurrentPos);
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
    if (AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
      AE_GetPosFromChar(ae, &ae->ciSelStartIndex, NULL, &cf.ptCurrentPos);
    else
      AE_GlobalToClient(ae, &ae->ptCaret, NULL, &cf.ptCurrentPos);
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
    AE_MessageBeep(ae, MB_OK);
    return TRUE;
  }
  return FALSE;
}

BOOL AE_MessageBeep(AKELEDIT *ae, UINT uType)
{
  if (ae->bBeepEnable && !(ae->popt->dwOptions & AECO_DISABLEBEEP))
    return MessageBeep(uType);
  return FALSE;
}

BOOL AE_KeyDown(AKELEDIT *ae, int nVk, BOOL bAlt, BOOL bShift, BOOL bControl)
{
  AECHARINDEX ciCharIn=ae->ciCaretIndex;
  AECHARINDEX ciCharOut=ae->ciCaretIndex;
  INT_PTR nCaretHorzIndent=ae->nCaretHorzIndent;
  BOOL bSetCaretHorzIndent=TRUE;

  if (nVk == VK_TAB)
  {
    if (!bAlt && bControl)
    {
      AE_EditChar(ae, VK_TAB, TRUE);
      return TRUE;
    }
  }
  if (nVk == VK_BACK)
  {
    if (!bAlt)
    {
      AE_EditKeyBackspace(ae, bControl);
    }
    else if (!bControl && !bShift)
    {
      AE_EditUndo(ae);
    }
    return TRUE;
  }
  if (nVk == VK_DELETE)
  {
    if (!bAlt)
    {
      if (!bShift)
      {
        AE_EditKeyDelete(ae, bControl);
      }
      else if (!bControl)
      {
        AE_EditCut(ae, 0, AELB_ASOUTPUT);
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
        if (ae->popt->dwOptionsEx & AECOE_OVERTYPE)
          ae->popt->dwOptionsEx&=~AECOE_OVERTYPE;
        else
          ae->popt->dwOptionsEx|=AECOE_OVERTYPE;
        AE_UpdateCaret(ae, ae->bFocus);
      }
      else
      {
        if (bControl && !bShift)
        {
          AE_EditCopyToClipboard(ae, NULL, AELB_ASOUTPUT, ae->bColumnSel);
        }
        else if (!bControl && bShift)
        {
          AE_EditPasteFromClipboard(ae, 0, AELB_ASINPUT);
        }
        else if (bControl && bShift)
        {
          AE_EditPasteFromClipboard(ae, AEPFC_ANSI, AELB_ASINPUT);
        }
      }
    }
    return TRUE;
  }
  if (nVk == 'X')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditCut(ae, 0, AELB_ASOUTPUT);
    }
    return TRUE;
  }
  if (nVk == 'C')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditCopyToClipboard(ae, NULL, AELB_ASOUTPUT, ae->bColumnSel);
    }
    return TRUE;
  }
  if (nVk == 'V')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditPasteFromClipboard(ae, 0, AELB_ASINPUT);
    }
    else if (bControl && bShift && !bAlt)
    {
      AE_EditPasteFromClipboard(ae, AEPFC_ANSI, AELB_ASINPUT);
    }
    else if (!bControl && !bShift && bAlt)
    {
      if (!(ae->popt->dwOptions & AECO_NOCOLUMNPASTEHOTKEY))
        AE_EditPasteFromClipboard(ae, AEPFC_COLUMN|AEPFC_SELECT, AELB_ASINPUT);
    }
    return TRUE;
  }
  if (nVk == 'Z')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditUndo(ae);
    }
    else if (bControl && bShift && !bAlt)
    {
      AE_EditRedo(ae);
    }
    return TRUE;
  }
  if (nVk == 'Y')
  {
    if (bControl && !bShift && !bAlt)
    {
      AE_EditRedo(ae);
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
      if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelStartIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCharOut);
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
      if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelEndIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciCharOut);
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
        if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciCharIn=ae->ciSelStartIndex;
        if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
          ciCharIn.nCharInLine=min(ciCharIn.nCharInLine, ciCharIn.lpLine->nLineLen);

        AE_GetPrevBreak(ae, &ciCharIn, &ciCharOut, bAlt, ae->popt->dwWordBreak);
      }
      else
      {
        if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        {
          ciCharIn=ae->ciSelStartIndex;
          ciCharOut=ae->ciSelStartIndex;
        }
        else
        {
          if (bAlt || ((ae->popt->dwOptions & AECO_CARETOUTEDGE) && ae->ciCaretIndex.nCharInLine > 0))
          {
            AEC_IndexDec(&ciCharOut);
            ciCharOut.nCharInLine=max(ciCharOut.nCharInLine, 0);
          }
          else AE_GetIndex(ae, AEGI_PREVUNCOLLAPSEDCHAR, &ciCharIn, &ciCharOut);
        }
      }
    }
    else if (nVk == VK_RIGHT)
    {
      if (bControl)
      {
        if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciCharIn=ae->ciSelEndIndex;
        if (ae->popt->dwOptions & AECO_CARETOUTEDGE)
          ciCharIn.nCharInLine=min(ciCharIn.nCharInLine, ciCharIn.lpLine->nLineLen);

        AE_GetNextBreak(ae, &ciCharIn, &ciCharOut, bAlt, ae->popt->dwWordBreak);
      }
      else
      {
        if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        {
          ciCharIn=ae->ciSelEndIndex;
          ciCharOut=ae->ciSelEndIndex;
        }
        else
        {
          if (bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
            AEC_IndexInc(&ciCharOut);
          else
            AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDCHAR, &ciCharIn, &ciCharOut);
        }
      }
    }
    else if (nVk == VK_UP)
    {
      if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelStartIndex;

      if (AE_GetIndex(ae, AEGI_PREVUNCOLLAPSEDLINE, &ciCharIn, &ciCharOut))
      {
        if (!bControl)
        {
          AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
          bSetCaretHorzIndent=FALSE;
        }
        else
        {
          if (!AEC_WrapLineBeginEx(&ciCharIn, &ciCharOut))
          {
            AE_GetIndex(ae, AEGI_PREVUNCOLLAPSEDLINE, &ciCharIn, &ciCharOut);
            AEC_WrapLineBegin(&ciCharOut);
          }
        }
      }
    }
    else if (nVk == VK_DOWN)
    {
      if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelEndIndex;

      if (AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDLINE, &ciCharIn, &ciCharOut))
      {
        if (!bControl)
        {
          AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));
          bSetCaretHorzIndent=FALSE;
        }
        else
        {
          AEC_WrapLineEndEx(&ciCharIn, &ciCharOut);
          AE_GetIndex(ae, AEGI_NEXTUNCOLLAPSEDLINE, &ciCharOut, &ciCharOut);
        }
      }
    }
    else if (nVk == VK_PRIOR)
    {
      INT_PTR nVertPos;

      if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelStartIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_FIRSTFULLVISIBLECHAR, NULL, &ciCharOut);
        if (bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
          ciCharOut.nCharInLine=ciCharIn.nCharInLine;
        else
          ciCharOut.nCharInLine=0;
      }
      else
      {
        nVertPos=AE_VPosFromLine(ae, ciCharIn.nLine) - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->ptxt->nCharHeight;
        ciCharOut.nLine=AE_LineFromVPos(ae, nVertPos);
        AE_IndexUpdate(ae, &ciCharOut);
        AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

        if (ciCharIn.nLine >= AE_GetFirstVisibleLine(ae) && ciCharIn.nLine <= AE_GetLastVisibleLine(ae))
          AE_ScrollEditWindow(ae, SB_VERT, AE_VPosFromLine(ae, ciCharOut.nLine) - (AE_VPosFromLine(ae, ciCharIn.nLine) - ae->nVScrollPos));
        bSetCaretHorzIndent=FALSE;
      }
    }
    else if (nVk == VK_NEXT)
    {
      INT_PTR nVertPos;

      if (!bShift && AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        ciCharIn=ae->ciSelEndIndex;

      if (bControl)
      {
        AE_GetIndex(ae, AEGI_LASTFULLVISIBLECHAR, NULL, &ciCharOut);
        if (bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE))
          ciCharOut.nCharInLine=ciCharIn.nCharInLine;
        else
          ciCharOut.nCharInLine=ciCharOut.lpLine->nLineLen;
      }
      else
      {
        nVertPos=AE_VPosFromLine(ae, ciCharIn.nLine) + (ae->rcDraw.bottom - ae->rcDraw.top);
        ciCharOut.nLine=AE_LineFromVPos(ae, nVertPos);
        AE_IndexUpdate(ae, &ciCharOut);
        AE_GetCharInLine(ae, ciCharOut.lpLine, nCaretHorzIndent, AECIL_HALFFIT|AECIL_ALLPOS, &ciCharOut.nCharInLine, NULL, bAlt || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

        if (ciCharIn.nLine >= AE_GetFirstVisibleLine(ae) && ciCharIn.nLine <= AE_GetLastVisibleLine(ae))
          AE_ScrollEditWindow(ae, SB_VERT, AE_VPosFromLine(ae, ciCharOut.nLine) - (AE_VPosFromLine(ae, ciCharIn.nLine) - ae->nVScrollPos));
        bSetCaretHorzIndent=FALSE;
      }
    }

    //Make beep
    if (!(ae->popt->dwOptions & AECO_DISABLEBEEP))
    {
      if ((bShift || !AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex)) &&
          !AEC_IndexCompare(&ciCharIn, &ciCharOut))
      {
        AE_MessageBeep(ae, MB_OK);
      }
    }

    //Set selection
    {
      AECHARINDEX ciSelEnd={0};

      if (bShift)
      {
        if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
          ciSelEnd=ae->ciCaretIndex;
        else if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
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
  if (nVk == VK_ESCAPE)
  {
    if (ae->nCurrentCursor == AECC_MARKER && ae->dwMouseMoveTimer)
    {
      AE_ColumnMarkerSet(ae, ae->popt->dwColumnMarkerType, (int)ae->dwInitMarkerPos, TRUE);

      KillTimer(ae->hWndEdit, ae->dwMouseMoveTimer);
      ae->dwMouseMoveTimer=0;
      AE_ReleaseMouseCapture(ae, AEMSC_MOUSEMOVE|AEMSC_MOUSEMARKER);
      return TRUE;
    }
  }
  return FALSE;
}

void AE_ImeComplete(AKELEDIT *ae)
{
  if (ae->bImeComposition)
  {
    HIMC hImc;

    if (hImc=ImmGetContext(ae->hWndEdit))
    {
      ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0);
      ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
      ImmReleaseContext(ae->hWndEdit, hImc);
    }
  }
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
  if (ae->ptxt->lpCurrentUndo != ae->ptxt->hUndoStack.last)
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

BOOL AE_EditUndo(AKELEDIT *ae)
{
  AEUNDOITEM *lpCurElement;
  AEUNDOITEM *lpNextElement;
  AECHARINDEX ciActionStart;
  AECHARINDEX ciActionEnd;
  AECHARINDEX ciInsertStart;
  AECHARINDEX ciInsertEnd;
  POINT64 ptFirstVisLine;
  int nFirstVisibleLine=0;
  BOOL bColumnSel;

  if (AE_IsReadOnly(ae) || !AE_EditCanUndo(ae))
    return FALSE;
  AE_NotifyChanging(ae, AETCT_UNDO);
  AE_StackUndoGroupStop(ae);
  lpCurElement=ae->ptxt->lpCurrentUndo;

  if (ae->popt->dwOptions & AECO_NOSCROLLDELETEALL)
  {
    if (!ae->popt->nVScrollLock)
      nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
  }

  while (lpCurElement)
  {
    lpNextElement=lpCurElement->prev;

    if (!(lpCurElement->dwFlags & AEUN_REDOONLY))
    {
      if (lpCurElement->dwFlags & AEUN_COLUMNSEL)
        bColumnSel=TRUE;
      else
        bColumnSel=FALSE;
      if (lpCurElement->dwFlags & AEUN_COLUMNGROUP)
        ae->dwNotifyTextChange|=AETCT_COLUMNGROUP;

      if (lpCurElement->dwFlags & AEUN_INSERT)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
        AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        if (AE_InsertText(ae, &ciActionStart, lpCurElement->wpText, lpCurElement->dwTextLen, lpCurElement->nNewLine, bColumnSel, AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATEALL, &ciInsertStart, &ciInsertEnd))
        {
          if (!lpNextElement || (lpNextElement->dwFlags & AEUN_STOPGROUP))
          {
            if (lpCurElement->dwFlags & AEUN_BACKSPACEKEY)
              AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
            else if (lpCurElement->dwFlags & AEUN_DELETEKEY)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
            else if (lpCurElement->dwFlags & AEUN_OVERTYPECHAR)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
            else if (lpCurElement->dwFlags & AEUN_CARETATSTART)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
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
        AE_DeleteTextRange(ae, &ciActionStart, &ciActionEnd, bColumnSel, AEDELT_LOCKUNDO|AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATEALL);
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

        if (lpCurElement->dwFlags & AEUN_CARETATSTART)
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        else
          AE_SetSelectionPos(ae, &ciActionEnd, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
      }

      //Update window
      if (!lpNextElement || (lpNextElement->dwFlags & AEUN_STOPGROUP))
      {
        AE_UpdateScrollBars(ae, SB_BOTH);
        if (!(ae->popt->dwOptions & AECO_NOSCROLLDELETEALL) || !(ae->dwNotifyTextChange & AETCT_DELETEALL))
          AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
        ae->nCaretHorzIndent=ae->ptCaret.x;
        if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }

      if (lpCurElement->dwFlags & AEUN_COLUMNGROUP)
        ae->dwNotifyTextChange&=~AETCT_COLUMNGROUP;
    }
    lpCurElement=lpNextElement;

    //Stop undo
    ae->ptxt->lpCurrentUndo=lpCurElement;
    if (ae->ptxt->lpCurrentUndo)
      if (ae->ptxt->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
        break;
  }

  if ((ae->popt->dwOptions & AECO_NOSCROLLDELETEALL) && (ae->dwNotifyTextChange & AETCT_DELETEALL))
  {
    if (!ae->popt->nVScrollLock)
    {
      ptFirstVisLine.y=AE_VPosFromLine(ae, nFirstVisibleLine);
      AE_ScrollToPointEx(ae, AESC_POINTGLOBAL|AESC_OFFSETPIXELY|AESC_FORCETOP, &ptFirstVisLine, 0, 0);
    }
  }

  AE_NotifyChanged(ae); //AETCT_UNDO
  return TRUE;
}

BOOL AE_EditRedo(AKELEDIT *ae)
{
  AEUNDOITEM *lpCurElement=ae->ptxt->lpCurrentUndo;
  AEUNDOITEM *lpNextElement;
  AECHARINDEX ciActionStart;
  AECHARINDEX ciActionEnd;
  AECHARINDEX ciInsertStart;
  AECHARINDEX ciInsertEnd;
  POINT64 ptFirstVisLine;
  int nFirstVisibleLine=0;
  BOOL bColumnSel;

  if (AE_IsReadOnly(ae) || !AE_EditCanRedo(ae))
    return FALSE;
  AE_NotifyChanging(ae, AETCT_REDO);

  if (!lpCurElement)
    lpCurElement=ae->ptxt->hUndoStack.first;
  else
    lpCurElement=lpCurElement->next;

  if (ae->popt->dwOptions & AECO_NOSCROLLDELETEALL)
  {
    if (!ae->popt->nVScrollLock)
      nFirstVisibleLine=AE_GetFirstVisibleLine(ae);
  }

  while (lpCurElement)
  {
    lpNextElement=lpCurElement->next;

    if (!(lpCurElement->dwFlags & AEUN_UNDOONLY))
    {
      if (lpCurElement->dwFlags & AEUN_COLUMNSEL)
        bColumnSel=TRUE;
      else
        bColumnSel=FALSE;
      if (lpCurElement->dwFlags & AEUN_COLUMNGROUP)
        ae->dwNotifyTextChange|=AETCT_COLUMNGROUP;

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
        AE_DeleteTextRange(ae, &ciActionStart, &ciActionEnd, bColumnSel, AEDELT_LOCKUNDO|AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATEALL);
      }
      else if (lpCurElement->dwFlags & AEUN_DELETE)
      {
        AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
        if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
          ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;
        AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        if (AE_InsertText(ae, &ciActionStart, lpCurElement->wpText, lpCurElement->dwTextLen, lpCurElement->nNewLine, bColumnSel, AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATEALL, &ciInsertStart, &ciInsertEnd))
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

        if (lpCurElement->dwFlags & AEUN_CARETATSTART)
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
        else
          AE_SetSelectionPos(ae, &ciActionEnd, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE|AESELT_LOCKCARET|AESELT_LOCKUNDOGROUPING, 0);
      }

      //Update window
      if (!lpNextElement || (lpCurElement->dwFlags & AEUN_STOPGROUP))
      {
        AE_UpdateScrollBars(ae, SB_BOTH);
        if (!(ae->popt->dwOptions & AECO_NOSCROLLDELETEALL) || !(ae->dwNotifyTextChange & AETCT_DELETEALL))
          AE_ScrollToCaret(ae, &ae->ptCaret, 0, 0);
        ae->nCaretHorzIndent=ae->ptCaret.x;
        if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }

      if (lpCurElement->dwFlags & AEUN_COLUMNGROUP)
        ae->dwNotifyTextChange&=~AETCT_COLUMNGROUP;
    }

    //Stop undo
    ae->ptxt->lpCurrentUndo=lpCurElement;
    if (ae->ptxt->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
      break;

    lpCurElement=lpNextElement;
  }

  if ((ae->popt->dwOptions & AECO_NOSCROLLDELETEALL) && (ae->dwNotifyTextChange & AETCT_DELETEALL))
  {
    if (!ae->popt->nVScrollLock)
    {
      ptFirstVisLine.y=AE_VPosFromLine(ae, nFirstVisibleLine);
      AE_ScrollToPointEx(ae, AESC_POINTGLOBAL|AESC_OFFSETPIXELY|AESC_FORCETOP, &ptFirstVisLine, 0, 0);
    }
  }

  AE_NotifyChanged(ae); //AETCT_REDO
  return TRUE;
}

BOOL AE_NoSelectionRange(AKELEDIT *ae, AECHARRANGE *cr, DWORD dwFlags)
{
  cr->ciMin=ae->ciSelStartIndex;
  cr->ciMax=ae->ciSelEndIndex;

  if (!AEC_IndexCompare(&cr->ciMin, &cr->ciMax))
  {
    if (dwFlags & AECFC_WORD)
    {
      cr->ciMin=ae->ciCaretIndex;
      cr->ciMax=ae->ciCaretIndex;

      if (!AE_IsDelimiter(ae, &cr->ciMin, AEDLM_WORD|AEDLM_PREVCHAR))
        AE_GetPrevBreak(ae, &cr->ciMin, &cr->ciMin, FALSE, AEWB_LEFTWORDSTART);
      if (!AE_IsDelimiter(ae, &cr->ciMax, AEDLM_WORD))
        AE_GetNextBreak(ae, &cr->ciMax, &cr->ciMax, FALSE, AEWB_RIGHTWORDEND);
    }
    else if (dwFlags & AECFC_LINE)
    {
      cr->ciMin=ae->ciCaretIndex;
      cr->ciMax=ae->ciCaretIndex;

      cr->ciMin.nCharInLine=0;
      cr->ciMax.nCharInLine=cr->ciMax.lpLine->nLineLen;
      if ((dwFlags & AECFC_NEWLINE) && cr->ciMax.lpLine->nLineBreak != AELB_WRAP)
        AEC_NextCharEx(&cr->ciMax, &cr->ciMax);
    }
    else if (dwFlags & AECFC_UNWRAPLINE)
    {
      cr->ciMin=ae->ciCaretIndex;
      cr->ciMax=ae->ciCaretIndex;

      AEC_WrapLineBeginEx(&cr->ciMin, &cr->ciMin);
      AEC_WrapLineEndEx(&cr->ciMax, &cr->ciMax);
      if (dwFlags & AECFC_NEWLINE)
        AEC_NextCharEx(&cr->ciMax, &cr->ciMax);
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_EditCut(AKELEDIT *ae, DWORD dwFlags, int nNewLine)
{
  AECHARRANGE cr;
  BOOL bColumnSel=ae->bColumnSel;

  if (AE_NoSelectionRange(ae, &cr, dwFlags))
    bColumnSel=FALSE;
  if (AE_EditCopyToClipboard(ae, &cr, nNewLine, bColumnSel))
  {
    if (!AE_IsReadOnly(ae))
    {
      AE_NotifyChanging(ae, AETCT_CUT);
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &cr.ciMin, &cr.ciMax, bColumnSel, 0);
      AE_StackUndoGroupStop(ae);
      AE_NotifyChanged(ae); //AETCT_CUT
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_EditCopyToClipboard(AKELEDIT *ae, AECHARRANGE *cr, int nNewLine, BOOL bColumnSel)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  HGLOBAL hDataInfo=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  AECLIPBOARDINFO *pDataInfo;
  UINT_PTR dwAnsiLen=0;
  UINT_PTR dwUnicodeLen=0;
  AECHARINDEX *lpSelStart;
  AECHARINDEX *lpSelEnd;

  if (!cr)
  {
    lpSelStart=&ae->ciSelStartIndex;
    lpSelEnd=&ae->ciSelEndIndex;
  }
  else
  {
    lpSelStart=&cr->ciMin;
    lpSelEnd=&cr->ciMax;
  }

  if (AEC_IndexCompare(lpSelStart, lpSelEnd))
  {
    if (OpenClipboard(NULL))
    {
      EmptyClipboard();

      if (dwUnicodeLen=AE_GetTextRange(ae, lpSelStart, lpSelEnd, NULL, 0, nNewLine, bColumnSel, TRUE))
      {
        if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, dwUnicodeLen * sizeof(wchar_t)))
        {
          if (pDataTargetW=GlobalLock(hDataTargetW))
          {
            AE_GetTextRange(ae, lpSelStart, lpSelEnd, (wchar_t *)pDataTargetW, (UINT_PTR)-1, nNewLine, bColumnSel, TRUE);

            //Get Ansi text
            dwAnsiLen=WideCharToMultiByte64(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, NULL, 0, NULL, NULL);

            if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, dwAnsiLen))
            {
              if (pDataTargetA=GlobalLock(hDataTargetA))
              {
                WideCharToMultiByte64(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, (char *)pDataTargetA, dwAnsiLen, NULL, NULL);
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
      if (bColumnSel)
      {
        SetClipboardData(cfAkelEditColumnSel, NULL);
      }
      if (hDataInfo=GlobalAlloc(GMEM_MOVEABLE, sizeof(AECLIPBOARDINFO)))
      {
        if (pDataInfo=(AECLIPBOARDINFO *)GlobalLock(hDataInfo))
        {
          pDataInfo->dwAnsiLen64=dwAnsiLen?dwAnsiLen - 1:0;
          pDataInfo->dwUnicodeLen64=dwUnicodeLen?dwUnicodeLen - 1:0;
          pDataInfo->dwAnsiLen=(DWORD)pDataInfo->dwAnsiLen64;
          pDataInfo->dwUnicodeLen=(DWORD)pDataInfo->dwUnicodeLen64;
          GlobalUnlock(pDataInfo);
        }
        SetClipboardData(cfAkelEditText, hDataInfo);
      }
      CloseClipboard();
    }
  }
  if (hDataInfo)
    return TRUE;
  return FALSE;
}

INT_PTR AE_EditPasteFromClipboard(AKELEDIT *ae, DWORD dwFlags, int nNewLine)
{
  HGLOBAL hDataInfo;
  HGLOBAL hData=NULL;
  LPVOID pData;
  AECLIPBOARDINFO *pDataInfo;
  wchar_t *wszText=NULL;
  UINT_PTR dwAnsiLen=(UINT_PTR)-1;
  UINT_PTR dwUnicodeLen=(UINT_PTR)-1;
  BOOL bColumnSel;
  BOOL bFreeText=FALSE;
  INT_PTR nResult=-1;

  if (AE_IsReadOnly(ae)) return nResult;
  bColumnSel=IsClipboardFormatAvailable(cfAkelEditColumnSel);

  if (OpenClipboard(NULL))
  {
    //Get AkelEdit clipboard text length
    if (hDataInfo=GetClipboardData(cfAkelEditText))
    {
      if (pDataInfo=(AECLIPBOARDINFO *)GlobalLock(hDataInfo))
      {
        #ifdef _WIN64
          //x64
          dwAnsiLen=pDataInfo->dwAnsiLen64;
          dwUnicodeLen=pDataInfo->dwUnicodeLen64;
        #else
          //x86
          dwAnsiLen=pDataInfo->dwAnsiLen;
          dwUnicodeLen=pDataInfo->dwUnicodeLen;
        #endif
        GlobalUnlock(pDataInfo);
      }
    }

    //Get clipboard text
    if (!(dwFlags & AEPFC_ANSI) && (hData=GetClipboardData(CF_UNICODETEXT)))
    {
      if (pData=GlobalLock(hData))
      {
        wszText=(wchar_t *)pData;
      }
    }
    else if (hData=GetClipboardData(CF_TEXT))
    {
      if (pData=GlobalLock(hData))
      {
        if (dwUnicodeLen=MultiByteToWideChar64(CP_ACP, 0, (char *)pData, dwAnsiLen, NULL, 0))
        {
          if (dwAnsiLen == (UINT_PTR)-1)
            dwUnicodeLen-=1;

          if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, (dwUnicodeLen + 1) * sizeof(wchar_t)))
          {
            MultiByteToWideChar64(CP_ACP, 0, (char *)pData, dwAnsiLen, wszText, dwUnicodeLen + 1);
            bFreeText=TRUE;
          }
        }
      }
    }

    if (wszText)
    {
      if (dwFlags & AEPFC_COLUMN)
      {
        wchar_t *wpSource=wszText;
        wchar_t *wpTarget;
        int nLineSelStart;
        int nLineSelEnd;
        int nLineSelRange;
        int nLineSourceRange;
        int nLineTargetRange;
        INT_PTR nSourceLen;
        INT_PTR nTargetLen;
        INT_PTR nTargetCount;
        int i;

        nLineSelStart=AE_GetUnwrapLine(ae, ae->ciSelStartIndex.nLine);
        nLineSelEnd=AE_GetUnwrapLine(ae, ae->ciSelEndIndex.nLine);
        nLineSelRange=(nLineSelEnd - nLineSelStart) + 1;

        nSourceLen=xstrlenW(wpSource);
        while (nSourceLen > 0)
        {
          if (wpSource[nSourceLen - 1] == L'\r' ||
              wpSource[nSourceLen - 1] == L'\n')
          {
            --nSourceLen;
          }
          else break;
        }
        if (nSourceLen)
        {
          nLineSourceRange=AE_GetLinesCount(wpSource, nSourceLen);
          nLineTargetRange=nLineSelRange / nLineSourceRange;
          if (nLineSelRange % nLineSourceRange)
            ++nLineTargetRange;
          nTargetLen=(nSourceLen + 1) * nLineTargetRange - 1;

          if (wpTarget=(wchar_t *)AE_HeapAlloc(ae, 0, (nTargetLen + 1) * sizeof(wchar_t)))
          {
            for (i=0; i < nLineTargetRange; ++i)
            {
              nTargetCount=i * (nSourceLen + 1);
              xmemcpy(wpTarget + nTargetCount, wpSource, nSourceLen * sizeof(wchar_t));
              wpTarget[nTargetCount + nSourceLen]=L'\r';
            }
            wpTarget[nTargetLen]=L'\0';

            AE_ReplaceSel(ae, wpTarget, nTargetLen, nNewLine, AEREPT_COLUMNON|(dwFlags & AEPFC_SELECT?AEREPT_SELECT:0), NULL, NULL);
            nResult=nTargetLen;

            AE_HeapFree(ae, 0, (LPVOID)wpTarget);
          }
        }
      }
      else
      {
        AE_ReplaceSel(ae, wszText, dwUnicodeLen, nNewLine, (bColumnSel?AEREPT_COLUMNON:0)|(dwFlags & AEPFC_SELECT?AEREPT_SELECT:0), NULL, NULL);
        nResult=dwUnicodeLen;
      }
      if (bFreeText) AE_HeapFree(NULL, 0, (LPVOID)wszText);
    }
    if (hData) GlobalUnlock(hData);

    CloseClipboard();
  }
  return nResult;
}

void AE_EditChar(AKELEDIT *ae, WPARAM wParam, BOOL bUnicode)
{
  if (AE_IsReadOnly(ae)) return;
  AE_NotifyChanging(ae, AETCT_CHAR);

  if (!bUnicode)
  {
    char chChar=(char)(WORD)wParam;
    wchar_t wchChar;

    if (ae->popt->dwOptionsEx & AECOE_OVERTYPE)
    {
      if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
      {
        AECHARINDEX ciCharIndex=ae->ciCaretIndex;

        AEC_IndexInc(&ciCharIndex);

        if (ciCharIndex.nCharInLine <= ciCharIndex.lpLine->nLineLen)
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

    if (ae->popt->dwOptionsEx & AECOE_OVERTYPE)
    {
      if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
      {
        AECHARINDEX ciCharIndex=ae->ciCaretIndex;

        AEC_IndexInc(&ciCharIndex);

        if (ciCharIndex.nCharInLine <= ciCharIndex.lpLine->nLineLen)
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
  const wchar_t *wpNewLine;
  int nLineBreakLen;

  if (AE_IsReadOnly(ae)) return;
  AE_NotifyChanging(ae, AETCT_KEYRETURN);

  nLineBreakLen=AE_GetNewLineString(ae->popt->nInputNewLine, &wpNewLine);
  AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
  ciCharIndex=ae->ciCaretIndex;
  ciCharIndex.nCharInLine=min(ciCharIndex.nCharInLine, ciCharIndex.lpLine->nLineLen);
  if (AE_InsertText(ae, &ciCharIndex, wpNewLine, nLineBreakLen, AELB_ASINPUT, FALSE, 0, NULL, NULL))
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

  if (AE_IsReadOnly(ae)) return;
  AE_NotifyChanging(ae, AETCT_KEYBACKSPACE);

  if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
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
        (!bControl && AEC_PrevCharEx(&ae->ciSelStartIndex, &ciCharIndex)))
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
    else AE_MessageBeep(ae, MB_OK);
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

  if (AE_IsReadOnly(ae)) return;
  AE_NotifyChanging(ae, AETCT_KEYDELETE);

  if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
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
    ciCharIndex=ae->ciSelStartIndex;
    AEC_ValidCharInLine(&ciCharIndex);

    if ((bControl && AE_GetNextBreak(ae, &ciCharIndex, &ciCharIndex, FALSE, ae->popt->dwWordBreak)) ||
        (!bControl && AEC_NextCharEx(&ciCharIndex, &ciCharIndex)))
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
    else AE_MessageBeep(ae, MB_OK);
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

  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciFirstChar);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciLastChar);
  AE_SetSelectionPos(ae, &ciLastChar, &ciFirstChar, FALSE, dwSelFlags, dwSelType);
}

BOOL AE_AkelEditGetSel(AKELEDIT *ae, AESELECTION *aes, AECHARINDEX *lpciCaret)
{
  if (aes)
  {
    aes->crSel.ciMin=ae->ciSelStartIndex;
    aes->crSel.ciMax=ae->ciSelEndIndex;
    aes->dwFlags=ae->bColumnSel;
    aes->dwType=0;
  }
  if (lpciCaret)
  {
    *lpciCaret=ae->ciCaretIndex;
  }
  if (!AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
    return FALSE;
  return TRUE;
}

void AE_AkelEditSetSel(AKELEDIT *ae, const AESELECTION *aes, const AECHARINDEX *lpciCaret)
{
  if (!lpciCaret)
  {
    if (AEC_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex) >= 0)
      AE_SetSelectionPos(ae, &aes->crSel.ciMax, &aes->crSel.ciMin, FALSE, aes->dwFlags, aes->dwType|AESCT_SETSELMESSAGE);
    else
      AE_SetSelectionPos(ae, &aes->crSel.ciMin, &aes->crSel.ciMax, FALSE, aes->dwFlags, aes->dwType|AESCT_SETSELMESSAGE);
  }
  else
  {
    if (AEC_IndexCompare(lpciCaret, &aes->crSel.ciMax) >= 0)
      AE_SetSelectionPos(ae, &aes->crSel.ciMax, &aes->crSel.ciMin, FALSE, aes->dwFlags, aes->dwType|AESCT_SETSELMESSAGE);
    else
      AE_SetSelectionPos(ae, &aes->crSel.ciMin, &aes->crSel.ciMax, FALSE, aes->dwFlags, aes->dwType|AESCT_SETSELMESSAGE);
  }
}

INT_PTR AE_RichEditGetSel(AKELEDIT *ae, INT_PTR *nMin, INT_PTR *nMax)
{
  *nMin=AE_AkelIndexToRichOffset(ae, &ae->ciSelStartIndex);
  *nMax=AE_AkelIndexToRichOffset(ae, &ae->ciSelEndIndex);
  return (*nMax - *nMin);
}

void AE_RichEditSetSel(AKELEDIT *ae, INT_PTR nMin, INT_PTR nMax)
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
      aec->crBasicText=ae->popt->aec.crBasicText;
  }
  if (aec->dwFlags & AECLR_BASICBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crBasicBk=GetSysColor(COLOR_WINDOW);
    else
      aec->crBasicBk=ae->popt->aec.crBasicBk;
  }
  if (aec->dwFlags & AECLR_SELTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
    else
      aec->crSelText=ae->popt->aec.crSelText;
  }
  if (aec->dwFlags & AECLR_SELBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
    else
      aec->crSelBk=ae->popt->aec.crSelBk;
  }
  if (aec->dwFlags & AECLR_ACTIVELINETEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
    else
      aec->crActiveLineText=ae->popt->aec.crActiveLineText;
  }
  if (aec->dwFlags & AECLR_ACTIVELINEBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
    {
      aec->crActiveLineBk=AE_ColorBrightness(GetSysColor(COLOR_HIGHLIGHT), 94);
      aec->crActiveLineBk=AE_ColorCombine(aec->crActiveLineBk, GetSysColor(COLOR_WINDOW), RGB(0xFF, 0xFF, 0xFF));
    }
    else aec->crActiveLineBk=ae->popt->aec.crActiveLineBk;
  }
  if (aec->dwFlags & AECLR_ACTIVELINEBORDER)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
    {
      aec->crActiveLineBorder=AE_ColorBrightness(GetSysColor(COLOR_HIGHLIGHT), 90);
      aec->crActiveLineBorder=AE_ColorCombine(aec->crActiveLineBorder, GetSysColor(COLOR_WINDOW), RGB(0xFF, 0xFF, 0xFF));
    }
    else aec->crActiveLineBorder=ae->popt->aec.crActiveLineBorder;
  }
  if (aec->dwFlags & AECLR_ALTLINETEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crAltLineText=GetSysColor(COLOR_WINDOWTEXT);
    else
      aec->crAltLineText=ae->popt->aec.crAltLineText;
  }
  if (aec->dwFlags & AECLR_ALTLINEBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crAltLineBk=AE_ColorSmooth(GetSysColor(COLOR_WINDOW), 3);
    else
      aec->crAltLineBk=ae->popt->aec.crAltLineBk;
  }
  if (aec->dwFlags & AECLR_ALTLINEBORDER)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crAltLineBorder=AE_ColorSmooth(GetSysColor(COLOR_WINDOW), 7);
    else
      aec->crAltLineBorder=ae->popt->aec.crAltLineBorder;
  }
  if (aec->dwFlags & AECLR_ACTIVECOLUMN)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crActiveColumn=GetSysColor(COLOR_GRAYTEXT);
    else
      aec->crActiveColumn=ae->popt->aec.crActiveColumn;
  }
  if (aec->dwFlags & AECLR_COLUMNMARKER)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crColumnMarker=GetSysColor(COLOR_BTNFACE);
    else
      aec->crColumnMarker=ae->popt->aec.crColumnMarker;
  }
  if (aec->dwFlags & AECLR_CARET)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crCaret=RGB(0x00, 0x00, 0x00);
    else
      aec->crCaret=ae->popt->aec.crCaret;
  }
  if (aec->dwFlags & AECLR_URLTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
    {
      if (GetSysColorBrush(COLOR_HOTLIGHT))
        aec->crUrlText=GetSysColor(COLOR_HOTLIGHT);
      else
        aec->crUrlText=RGB(0x00, 0x00, 0xFF);
    }
    else aec->crUrlText=ae->popt->aec.crUrlText;
  }
  if (aec->dwFlags & AECLR_URLCURSORTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crUrlCursorText=GetSysColor(COLOR_HIGHLIGHT);
    else
      aec->crUrlCursorText=ae->popt->aec.crUrlCursorText;
  }
  if (aec->dwFlags & AECLR_URLVISITTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crUrlVisitText=GetSysColor(COLOR_HIGHLIGHT);
    else
      aec->crUrlVisitText=ae->popt->aec.crUrlVisitText;
  }
}

void AE_SetColors(AKELEDIT *ae, const AECOLORS *aec, BOOL bUpdate)
{
  if (aec->dwFlags)
  {
    AECOLORS aecDefault;

    if (aec->dwFlags & AECLR_DEFAULT)
    {
      aecDefault.dwFlags=aec->dwFlags;
      AE_GetColors(ae, &aecDefault);
      aec=&aecDefault;
    }
    else ae->popt->bDefaultColors=FALSE;

    if (aec->dwFlags & AECLR_BASICTEXT)
    {
      ae->popt->aec.crBasicText=aec->crBasicText;
    }
    if (aec->dwFlags & AECLR_BASICBK)
    {
      ae->popt->aec.crBasicBk=aec->crBasicBk;
    }
    if (aec->dwFlags & AECLR_SELTEXT)
    {
      ae->popt->aec.crSelText=aec->crSelText;
    }
    if (aec->dwFlags & AECLR_SELBK)
    {
      ae->popt->aec.crSelBk=aec->crSelBk;
    }
    if (aec->dwFlags & AECLR_ACTIVELINETEXT)
    {
      ae->popt->aec.crActiveLineText=aec->crActiveLineText;
    }
    if (aec->dwFlags & AECLR_ACTIVELINEBK)
    {
      ae->popt->aec.crActiveLineBk=aec->crActiveLineBk;
    }
    if (aec->dwFlags & AECLR_ACTIVELINEBORDER)
    {
      ae->popt->aec.crActiveLineBorder=aec->crActiveLineBorder;
    }
    if (aec->dwFlags & AECLR_ALTLINETEXT)
    {
      ae->popt->aec.crAltLineText=aec->crAltLineText;
    }
    if (aec->dwFlags & AECLR_ALTLINEBK)
    {
      ae->popt->aec.crAltLineBk=aec->crAltLineBk;
    }
    if (aec->dwFlags & AECLR_ALTLINEBORDER)
    {
      ae->popt->aec.crAltLineBorder=aec->crAltLineBorder;
    }
    if (aec->dwFlags & AECLR_ACTIVECOLUMN)
    {
      ae->popt->aec.crActiveColumn=aec->crActiveColumn;

      if (ae->popt->dwOptions & AECO_ACTIVECOLUMN)
      {
        AEPENITEM *lpPenItem;

        if (!(lpPenItem=AE_StackPenItemGet(&hAkelEditPensStack, ae->popt->aec.crActiveColumn, ae->popt->aec.crBasicBk)))
          lpPenItem=AE_StackPenItemInsert(&hAkelEditPensStack, ae->popt->aec.crActiveColumn, ae->popt->aec.crBasicBk);
        ae->popt->hActiveColumnPen=lpPenItem->hPen;
      }
    }
    if (aec->dwFlags & AECLR_COLUMNMARKER)
    {
      ae->popt->aec.crColumnMarker=aec->crColumnMarker;

      if (ae->popt->dwColumnMarkerPos)
      {
        AEPENITEM *lpPenItem;

        if (!(lpPenItem=AE_StackPenItemGet(&hAkelEditPensStack, ae->popt->aec.crColumnMarker, (COLORREF)-1)))
          lpPenItem=AE_StackPenItemInsert(&hAkelEditPensStack, ae->popt->aec.crColumnMarker, (COLORREF)-1);
        ae->popt->hColumnMarkerPen=lpPenItem->hPen;
      }
    }
    if (aec->dwFlags & AECLR_CARET)
    {
      ae->popt->aec.crCaret=aec->crCaret;
    }
    if (aec->dwFlags & AECLR_URLTEXT)
    {
      ae->popt->aec.crUrlText=aec->crUrlText;
    }
    if (aec->dwFlags & AECLR_URLCURSORTEXT)
    {
      ae->popt->aec.crUrlCursorText=aec->crUrlCursorText;
    }
    if (aec->dwFlags & AECLR_URLVISITTEXT)
    {
      ae->popt->aec.crUrlVisitText=aec->crUrlVisitText;
    }
    ae->popt->crActiveLineTextWithAltText=AE_ColorCombine(ae->popt->aec.crActiveLineText, ae->popt->aec.crAltLineText, ae->popt->aec.crBasicText);
    ae->popt->crActiveLineBkWithAltBk=AE_ColorCombine(ae->popt->aec.crActiveLineBk, ae->popt->aec.crAltLineBk, ae->popt->aec.crBasicBk);
    ae->popt->crActiveLineBorderWithAltBk=AE_ColorCombine(ae->popt->aec.crActiveLineBorder, ae->popt->aec.crAltLineBk, ae->popt->aec.crBasicBk);
    ae->popt->crActiveLineBorderWithAltBorder=AE_ColorCombine(ae->popt->aec.crActiveLineBorder, ae->popt->aec.crAltLineBorder, ae->popt->aec.crBasicBk);

    if ((aec->dwFlags & AECLR_CARET) || (aec->dwFlags & AECLR_ACTIVELINEBK))
    {
      AE_UpdateCaret(ae, ae->bFocus);
    }
    if (aec->dwFlags & ~AECLR_CARET)
    {
      if (aec->dwFlags & AECLR_BASICBK)
        InvalidateRect(ae->hWndEdit, &ae->rcEdit, TRUE);
      else
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      AE_StackCloneUpdate(ae);
    }
  }
}

COLORREF AE_ColorCombine(COLORREF crColor1Cur, COLORREF crColor2NewBase, COLORREF crColor3CurBase)
{
  int r1=GetRValue(crColor1Cur);
  int g1=GetGValue(crColor1Cur);
  int b1=GetBValue(crColor1Cur);
  int r2=GetRValue(crColor2NewBase);
  int g2=GetGValue(crColor2NewBase);
  int b2=GetBValue(crColor2NewBase);
  int r3=GetRValue(crColor3CurBase);
  int g3=GetGValue(crColor3CurBase);
  int b3=GetBValue(crColor3CurBase);

  if ((r1 < 0x80 && r2 < 0x80 && r3 < 0x80) || (r1 > 0x80 && r2 > 0x80 && r3 > 0x80))
  {
    r1+=(r2 - r3);
    r1=min(r1, 0xFF);
    r1=max(r1, 0x00);
  }
  else
  {
    if (r1 < r2)
      r1+=(r2 - r1) / 6;
    else
      r1-=(r1 - r2) / 6;
  }

  if ((g1 < 0x80 && g2 < 0x80 && g3 < 0x80) || (g1 > 0x80 && g2 > 0x80 && g3 > 0x80))
  {
    g1+=(g2 - g3);
    g1=min(g1, 0xFF);
    g1=max(g1, 0x00);
  }
  else
  {
    if (g1 < g2)
      g1+=(g2 - g1) / 6;
    else
      g1-=(g1 - g2) / 6;
  }

  if ((b1 < 0x80 && b2 < 0x80 && b3 < 0x80) || (b1 > 0x80 && b2 > 0x80 && b3 > 0x80))
  {
    b1+=(b2 - b3);
    b1=min(b1, 0xFF);
    b1=max(b1, 0x00);
  }
  else
  {
    if (b1 < b2)
      b1+=(b2 - b1) / 6;
    else
      b1-=(b1 - b2) / 6;
  }

  return RGB(r1, g1, b1);
}

COLORREF AE_ColorBrightness(COLORREF crColor, int nPercent)
{
  //From -100% to 0% - to dark, from 0% to 100% - to light
  BYTE r=GetRValue(crColor);
  BYTE g=GetGValue(crColor);
  BYTE b=GetBValue(crColor);

  if  (nPercent > 0 && nPercent <= 100)
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

COLORREF AE_ColorSmooth(COLORREF crColor, int nPercent)
{
  //From 0% to 100%
  if (GetRValue(crColor) + GetGValue(crColor) + GetBValue(crColor) > 0xC0)
    return AE_ColorBrightness(crColor, -nPercent);
  else
    return AE_ColorBrightness(crColor, +nPercent * 8);
}

COLORREF AE_GetColorFromStr(const wchar_t *wpColor, const wchar_t **wpNext)
{
  COLORREF crColor;

  if ((crColor=(COLORREF)hex2decW(wpColor, 6, NULL)) != (DWORD)-1)
  {
    //RRGGBB
    if (wpNext) *wpNext=wpColor + 6;
    crColor=RGB(GetBValue(crColor), GetGValue(crColor), GetRValue(crColor));
  }
  else
  {
    //RGB->RRGGBB
    wchar_t wszRBG[6];
    wchar_t *wpRBG=wszRBG;
    wchar_t *wpMaxRBG=wszRBG + 6;

    while (wpRBG < wpMaxRBG)
    {
      if ((*wpColor >= '0' && *wpColor <= '9') ||
          (*wpColor >= 'a' && *wpColor <= 'f') ||
          (*wpColor >= 'A' && *wpColor <= 'F'))
      {
        *wpRBG++=*wpColor;
        *wpRBG++=*wpColor++;
      }
      else return (COLORREF)-1;
    }
    if (wpNext) *wpNext=wpColor;
    if ((crColor=(COLORREF)hex2decW(wszRBG, 6, NULL)) != (COLORREF)-1)
      crColor=RGB(GetBValue(crColor), GetGValue(crColor), GetRValue(crColor));
  }
  return crColor;
}

BOOL AE_GetBasicCharColors(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARCOLORS *aecc)
{
  if (!(aecc->dwFlags & AEGHF_NOSELECTION) && AEC_IsCharInSelection(ciChar))
  {
    aecc->crText=ae->popt->aec.crSelText;
    aecc->crBk=ae->popt->aec.crSelBk;
    aecc->crBorderTop=ae->popt->aec.crSelBk;
    aecc->crBorderBottom=ae->popt->aec.crSelBk;
    return TRUE;
  }
  else
  {
    DWORD dwAltModule=0;

    aecc->crBorderTop=(DWORD)-1;
    aecc->crBorderBottom=(DWORD)-1;

    //Is line alternating
    if (!(aecc->dwFlags & AEGHF_NOALTLINE) && ae->popt->dwAltLineSkip && ae->popt->dwAltLineFill)
    {
      dwAltModule=ciChar->nLine % (ae->popt->dwAltLineSkip + ae->popt->dwAltLineFill);

      if (dwAltModule >= ae->popt->dwAltLineSkip)
      {
        if (ae->popt->dwOptions & AECO_ALTLINEBORDER)
        {
          if (dwAltModule == ae->popt->dwAltLineSkip)
            aecc->crBorderTop=ae->popt->aec.crAltLineBorder;
          if (dwAltModule == ae->popt->dwAltLineSkip + ae->popt->dwAltLineFill - 1)
            aecc->crBorderBottom=ae->popt->aec.crAltLineBorder;
        }
      }
      else dwAltModule=0;
    }

    //Set initial colors
    if (!(aecc->dwFlags & AEGHF_NOACTIVELINE) && (ae->popt->dwOptions & AECO_ACTIVELINE) && ciChar->nLine == ae->ciCaretIndex.nLine)
    {
      if (dwAltModule)
      {
        aecc->crText=ae->popt->crActiveLineTextWithAltText;
        aecc->crBk=ae->popt->crActiveLineBkWithAltBk;
        if (aecc->crBorderTop != (DWORD)-1)
          aecc->crBorderTop=ae->popt->crActiveLineBorderWithAltBorder;
        else
          aecc->crBorderTop=ae->popt->crActiveLineBorderWithAltBk;
        if (aecc->crBorderBottom != (DWORD)-1)
          aecc->crBorderBottom=ae->popt->crActiveLineBorderWithAltBorder;
        else
          aecc->crBorderBottom=ae->popt->crActiveLineBorderWithAltBk;
      }
      else
      {
        aecc->crText=ae->popt->aec.crActiveLineText;
        aecc->crBk=ae->popt->aec.crActiveLineBk;
        if (ae->popt->dwOptions & AECO_ACTIVELINEBORDER)
        {
          aecc->crBorderTop=ae->popt->aec.crActiveLineBorder;
          aecc->crBorderBottom=ae->popt->aec.crActiveLineBorder;
        }
      }
    }
    else if (dwAltModule)
    {
      aecc->crText=ae->popt->aec.crAltLineText;
      aecc->crBk=ae->popt->aec.crAltLineBk;
    }
    else
    {
      aecc->crText=ae->popt->aec.crBasicText;
      aecc->crBk=ae->popt->aec.crBasicBk;
    }
    return FALSE;
  }
}

BOOL AE_GetHighLightCharColors(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARCOLORS *aecc)
{
  aecc->dwFontStyle=AEHLS_NONE;

  if (AE_GetBasicCharColors(ae, ciChar, aecc))
  {
    //Char in selection
    return TRUE;
  }
  else
  {
    //Get char highlighting
    AEGETHIGHLIGHT aegh;
    AECHARCOLORSCOOKIE aecck;

    aecck.ae=ae;
    aecck.aecc=aecc;

    aegh.dwCookie=(UINT_PTR)&aecck;
    aegh.lpCallback=AE_GetHighLightCharColorCallback;
    aegh.crText.ciMin=*ciChar;
    AEC_NextCharEx(&aegh.crText.ciMin, &aegh.crText.ciMax);
    aegh.dwFlags=aecc->dwFlags;
    AE_GetHighLight(ae, &aegh);
    return FALSE;
  }
}

DWORD CALLBACK AE_GetHighLightCharColorCallback(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp)
{
  AECHARCOLORSCOOKIE *aecck=(AECHARCOLORSCOOKIE *)dwCookie;
  AKELEDIT *ae=aecck->ae;
  AECHARCOLORS *aecc=aecck->aecc;
  DWORD dwFontStyle;

  dwFontStyle=hlp->dwFontStyle;
  if (ae->popt->dwHLOptions)
  {
    if (ae->popt->dwHLOptions & AEHLO_IGNOREFONTITALIC)
    {
      if (dwFontStyle == AEHLS_FONTITALIC)
        dwFontStyle=AEHLS_FONTNORMAL;
      else if (dwFontStyle == AEHLS_FONTBOLDITALIC)
        dwFontStyle=AEHLS_FONTBOLD;
    }
    if (ae->popt->dwHLOptions & AEHLO_IGNOREFONTBOLD)
    {
      if (dwFontStyle == AEHLS_FONTBOLD)
        dwFontStyle=AEHLS_FONTNORMAL;
      else if (dwFontStyle == AEHLS_FONTBOLDITALIC)
        dwFontStyle=AEHLS_FONTITALIC;
    }
    if (ae->popt->dwHLOptions & AEHLO_IGNOREFONTNORMAL)
    {
      if (dwFontStyle == AEHLS_FONTNORMAL)
        dwFontStyle=AEHLS_NONE;
    }
  }

  aecc->dwFontStyle=dwFontStyle;
  aecc->crText=hlp->dwActiveText;
  aecc->crBk=hlp->dwActiveBk;

  return 0;
}

void AE_NotifyErrSpace(AKELEDIT *ae, SIZE_T dwBytes)
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

void AE_NotifyPaint(AKELEDIT *ae, DWORD dwType, AENPAINT *pnt)
{
  //Send AEN_PAINT
  if (ae->popt->dwEventMask & AENM_PAINT)
  {
    pnt->hdr.hwndFrom=ae->hWndEdit;
    pnt->hdr.idFrom=ae->nEditCtrlID;
    pnt->hdr.code=AEN_PAINT;
    pnt->hdr.docFrom=(AEHDOC)ae;
    pnt->dwType=dwType;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)pnt);
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

DWORD AE_NotifyProgress(AKELEDIT *ae, DWORD dwType, DWORD dwTimeElapsed, INT_PTR nCurrent, INT_PTR nMaximum)
{
  DWORD dwError=0;

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
    dwError=(DWORD)AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&pgs);
  }
  return dwError;
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
    sc.crSel.ciMin=ae->ciSelStartIndex;
    sc.crSel.ciMax=ae->ciSelEndIndex;
    sc.ciCaret=ae->ciCaretIndex;
    sc.dwType=dwType;
    sc.bColumnSel=ae->bColumnSel;
    sc.crRichSel.cpMin=ae->nSelStartCharOffset;
    sc.crRichSel.cpMax=ae->nSelEndCharOffset;
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
    sc.crSel.ciMin=ae->ciSelStartIndex;
    sc.crSel.ciMax=ae->ciSelEndIndex;
    sc.ciCaret=ae->ciCaretIndex;
    sc.dwType=ae->dwNotifySelChange;
    sc.bColumnSel=ae->bColumnSel;
    sc.crRichSel.cpMin=ae->nSelStartCharOffset;
    sc.crRichSel.cpMax=ae->nSelEndCharOffset;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }

  //Send EN_SELCHANGE
  if (ae->popt->dwRichEventMask & ENM_SELCHANGE)
  {
    SELCHANGE64 sc;

    sc.nmhdr.hwndFrom=ae->hWndEdit;
    sc.nmhdr.idFrom=ae->nEditCtrlID;
    sc.nmhdr.code=EN_SELCHANGE;
    sc.chrg64.cpMin=ae->nSelStartCharOffset;
    sc.chrg64.cpMax=ae->nSelEndCharOffset;
    sc.chrg.cpMin=(int)ae->nSelStartCharOffset;
    sc.chrg.cpMax=(int)ae->nSelEndCharOffset;

    if (ae->nSelStartCharOffset == ae->nSelEndCharOffset)
    {
      sc.seltyp=SEL_EMPTY;
    }
    else
    {
      sc.seltyp=SEL_TEXT;
      if (ae->nSelEndCharOffset - ae->nSelStartCharOffset > 1)
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
    tc.crSel.ciMin=ae->ciSelStartIndex;
    tc.crSel.ciMax=ae->ciSelEndIndex;
    tc.ciCaret=ae->ciCaretIndex;
    tc.dwType=ae->dwNotifyTextChange;
    tc.bColumnSel=ae->bColumnSel;
    tc.crRichSel.cpMin=ae->nSelStartCharOffset;
    tc.crRichSel.cpMax=ae->nSelEndCharOffset;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }
}

void AE_NotifyTextChanged(AKELEDIT *ae)
{
  AE_StackCloneUpdate(ae);

  //Send AEN_TEXTCHANGED
  if (ae->popt->dwEventMask & AENM_TEXTCHANGE)
  {
    AENTEXTCHANGE tc;

    tc.hdr.hwndFrom=ae->hWndEdit;
    tc.hdr.idFrom=ae->nEditCtrlID;
    tc.hdr.code=AEN_TEXTCHANGED;
    tc.hdr.docFrom=(AEHDOC)ae;
    tc.crSel.ciMin=ae->ciSelStartIndex;
    tc.crSel.ciMax=ae->ciSelEndIndex;
    tc.ciCaret=ae->ciCaretIndex;
    tc.dwType=ae->dwNotifyTextChange;
    tc.bColumnSel=ae->bColumnSel;
    tc.crRichSel.cpMin=ae->nSelStartCharOffset;
    tc.crRichSel.cpMax=ae->nSelEndCharOffset;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }

  //Send EN_CHANGE
  if (ae->popt->dwRichEventMask & ENM_CHANGE)
  {
    if (!(ae->dwNotifyTextChange & AETCT_NONE))
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
    POINT ptClient;

    GetCursorPos(&ptClient);
    ScreenToClient(ae->hWndEdit, &ptClient);
    AE_GetCharFromPos(ae, ptClient.x, ptClient.y, &ciCharIndex, NULL, FALSE);

    df.hdr.hwndFrom=ae->hWndEdit;
    df.hdr.idFrom=ae->nEditCtrlID;
    df.hdr.code=AEN_DROPFILES;
    df.hdr.docFrom=(AEHDOC)ae;
    df.hDrop=hDrop;
    df.ciChar=ciCharIndex;

    bResult1=(BOOL)AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&df);
  }

  //Send EN_DROPFILES
  if (bResult1)
  {
    if (ae->popt->dwRichEventMask & ENM_DROPFILES)
    {
      ENDROPFILES64 df;
      AECHARINDEX ciCharIndex;
      POINT ptClient;

      GetCursorPos(&ptClient);
      ScreenToClient(ae->hWndEdit, &ptClient);
      AE_GetCharFromPos(ae, ptClient.x, ptClient.y, &ciCharIndex, NULL, FALSE);

      df.nmhdr.hwndFrom=ae->hWndEdit;
      df.nmhdr.idFrom=ae->nEditCtrlID;
      df.nmhdr.code=EN_DROPFILES;
      df.hDrop=hDrop;
      df.cp64=AE_AkelIndexToRichOffset(ae, &ciCharIndex);
      df.cp=(int)df.cp64;
      df.fProtected=FALSE;

      bResult2=(BOOL)AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&df);
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

    bResult=(BOOL)AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&ds);

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

    bResult=(BOOL)AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&dt);

    if (pt) *pt=dt.pt;
    if (lpdwEffect) *lpdwEffect=dt.dwEffect;
  }
  return bResult;
}

BOOL AE_NotifyLink(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam, const AECHARRANGE *crLink)
{
  AECHARRANGE crText;
  LRESULT lResult1=0;
  LRESULT lResult2=0;

  xmemcpy(&crText, crLink, sizeof(AECHARRANGE));

  //Send AEN_LINK
  if (ae->popt->dwEventMask & AENM_LINK)
  {
    AENLINK lnk;
    AEURLVISIT *lpUrlVisit;

    lnk.hdr.hwndFrom=ae->hWndEdit;
    lnk.hdr.idFrom=ae->nEditCtrlID;
    lnk.hdr.code=AEN_LINK;
    lnk.hdr.docFrom=(AEHDOC)ae;
    lnk.uMsg=uMsg;
    lnk.wParam=wParam;
    lnk.lParam=lParam;
    lnk.crLink.ciMin=crText.ciMin;
    lnk.crLink.ciMax=crText.ciMax;
    if (lpUrlVisit=AE_UrlVisitGetByRange(ae, &crText))
      lnk.nVisitCount=lpUrlVisit->nVisitCount;
    else
      lnk.nVisitCount=0;
    lResult1=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&lnk);

    if (lpUrlVisit)
    {
      if (lnk.nVisitCount)
        lpUrlVisit->nVisitCount=lnk.nVisitCount;
      else if (!lpUrlVisit->bStatic)
        AE_UrlVisitDelete(ae, lpUrlVisit);
    }
    else if (lnk.nVisitCount)
    {
      if (lpUrlVisit=AE_UrlVisitInsert(ae, &crText))
        lpUrlVisit->nVisitCount=lnk.nVisitCount;
    }
  }

  //Send EN_LINK
  if (!lResult1)
  {
    if (ae->popt->dwRichEventMask & ENM_LINK)
    {
      ENLINK64 enl;

      enl.nmhdr.hwndFrom=ae->hWndEdit;
      enl.nmhdr.idFrom=ae->nEditCtrlID;
      enl.nmhdr.code=EN_LINK;
      enl.msg=uMsg;
      enl.wParam=wParam;
      enl.lParam=lParam;
      enl.chrg64.cpMin=AE_AkelIndexToRichOffset(ae, &crText.ciMin);
      enl.chrg64.cpMax=AE_AkelIndexToRichOffset(ae, &crText.ciMax);
      enl.chrg.cpMin=(int)enl.chrg64.cpMin;
      enl.chrg.cpMax=(int)enl.chrg64.cpMax;
      lResult2=AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&enl);
    }
  }

  if (lResult1 || lResult2)
    return TRUE;
  return FALSE;
}

void AE_NotifyMarker(AKELEDIT *ae, BOOL bMouse)
{
  //Send AEN_MARKER
  if (ae->popt->dwEventMask & AENM_MARKER)
  {
    AENMARKER aenm;

    aenm.hdr.hwndFrom=ae->hWndEdit;
    aenm.hdr.idFrom=ae->nEditCtrlID;
    aenm.hdr.code=AEN_MARKER;
    aenm.hdr.docFrom=(AEHDOC)ae;
    aenm.dwType=ae->popt->dwColumnMarkerType;
    aenm.dwPos=ae->popt->dwColumnMarkerPos;
    aenm.bMouse=bMouse;
    AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&aenm);
  }
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

    if (!(bResult=(BOOL)AE_SendMessage(ae, ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&mf)))
    {
      *wParam=mf.wParam;
      *lParam=mf.lParam;
    }
  }
  return bResult;
}

void AE_SendEraseBackground(AKELEDIT *ae, HDC hDC)
{
  HDC hInputDC=hDC;

  if (hDC || (hDC=GetDC(ae->hWndEdit)))
  {
    AE_SendMessage(ae, ae->hWndEdit, WM_ERASEBKGND, (WPARAM)hDC, 0);
    if (!hInputDC) ReleaseDC(ae->hWndEdit, hDC);
  }
}

LRESULT AE_SendMessage(AKELEDIT *ae, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  lResult=SendMessageA(hWnd, uMsg, wParam, lParam);

  AE_CloneActivate(lpAkelEditPrev, ae);
  lpAkelEditPrev=ae;
  return lResult;
}

void AE_ChangeTwoBytesOrder(unsigned char *pSrc, UINT_PTR dwSrcSize, unsigned char *pDst)
{
  unsigned char *pLast=pSrc + dwSrcSize - 2;
  unsigned char *pSrcByte=pSrc;
  unsigned char *pDstByte=pDst?pDst:pSrc;
  unsigned char ch2;

  while (pSrcByte <= pLast)
  {
    ch2=*pSrcByte++;
    *pDstByte++=*pSrcByte++;
    *pDstByte++=ch2;
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
    fmtetc.cfFormat=(CLIPFORMAT)cfAkelEditColumnSel;

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

  if (ae->bDropping)
  {
    AE_DropTargetDropCursor(pDropTarget, NULL, NULL);
    ae->bDropping=FALSE;
    if (!ae->bFocus) DestroyCaret();
  }
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
    AE_GetCharFromPos(ae, pt.x, pt.y, &ciCharIndex, NULL, pDropTarget->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

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
                AE_CloneActivate(lpAkelEditPrev, aeSource);
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
                AE_CloneActivate(lpAkelEditPrev, ae);
                lpAkelEditPrev=ae;
              }
            }
          }

          //Insert
          {
            AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, FALSE, AESELT_LOCKNOTIFY|AESELT_LOCKUNDOGROUPING, 0);
            AE_InsertText(ae, &ciCharIndex, (wchar_t *)pData, ae->dwDragSelectionLength?ae->dwDragSelectionLength:(UINT_PTR)-1, AELB_ASINPUT, pDropTarget->bColumnSel, 0, &ciStart, &ciEnd);

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
                  AE_CloneActivate(lpAkelEditPrev, aeSource);
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
                  AE_CloneActivate(lpAkelEditPrev, ae);
                  lpAkelEditPrev=ae;
                }
              }
            }

            //Insert
            {
              wchar_t *wszText;
              UINT_PTR dwUnicodeBytes;

              dwUnicodeBytes=MultiByteToWideChar64(CP_ACP, 0, (char *)pData, -1, NULL, 0) * sizeof(wchar_t);

              if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
              {
                MultiByteToWideChar64(CP_ACP, 0, (char *)pData, -1, wszText, dwUnicodeBytes / sizeof(wchar_t));
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
    AE_ScrollToCaret(ae, &ae->ptCaret, AESELT_NOVERTSCROLLCORRECT, 0);
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
      POINT64 ptGlobal;
      DWORD dwScrollTest;

      AE_GetCharFromPos(ae, pt->x, pt->y, &ciCharIndex, &ptGlobal, pDropTarget->bColumnSel || (ae->popt->dwOptions & AECO_CARETOUTEDGE));

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
    PVOID pSource;
    PVOID pTarget;
    UINT_PTR dwSourceSize;

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

HRESULT WINAPI AEIDataObject_GetCanonicalFormatEtc(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, FORMATETC *pFormatEtcOut)
{
  AEIDataObject *pDataObj=(AEIDataObject *)lpTable;

  if (AE_DataObjectLookupFormatEtc(pDataObj, pFormatEtc) == -1)
    return DV_E_FORMATETC;

  pFormatEtcOut->cfFormat=pFormatEtc->cfFormat;
  pFormatEtcOut->ptd=NULL;
  pFormatEtcOut->dwAspect=DVASPECT_CONTENT;
  pFormatEtcOut->lindex=-1;
  pFormatEtcOut->tymed=TYMED_HGLOBAL;
  return S_OK;
}

HRESULT WINAPI AEIDataObject_SetData(LPUNKNOWN lpTable, FORMATETC *pFormatEtc, STGMEDIUM *pMedium, BOOL fRelease)
{
  return E_NOTIMPL;
}

HRESULT WINAPI AEIDataObject_EnumFormatEtc(LPUNKNOWN lpTable, DWORD dwDirection, IEnumFORMATETC **ppEnumFormatEtc)
{
  AKELEDIT *ae=(AKELEDIT *)((AEIDataObject *)lpTable)->ae;
  AEIEnumFORMATETC *objIEnumFORMATETC;

  *ppEnumFormatEtc=NULL;

  if (dwDirection = DATADIR_GET)
  {
    if ((objIEnumFORMATETC=(AEIEnumFORMATETC *)GlobalAlloc(GPTR, sizeof(AEIEnumFORMATETC))))
    {
      objIEnumFORMATETC->lpTable=&ae->iefVtbl;
      objIEnumFORMATETC->uRefCount=0;
      objIEnumFORMATETC->ae=ae;
      objIEnumFORMATETC->nPos=0;
      return AEIEnumFORMATETC_QueryInterface((LPUNKNOWN)objIEnumFORMATETC, &IID_IEnumFORMATETC, (void **)ppEnumFormatEtc);
    }
    else return E_OUTOFMEMORY;
  }
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

  if (pFormatEtc->ptd || pFormatEtc->lindex != -1)
    return -1;

  for (i=0; i < pDataObj->nNumFormats; ++i)
  {
    if (pFormatEtc->cfFormat == pDataObj->fmtetc[i].cfFormat &&
        (pFormatEtc->dwAspect & pDataObj->fmtetc[i].dwAspect) &&
        (pFormatEtc->tymed & pDataObj->fmtetc[i].tymed))
    {
      return i;
    }
  }
  return -1;
}

UINT_PTR AE_DataObjectCopySelection(AKELEDIT *ae)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  UINT_PTR dwAnsiLen=0;
  UINT_PTR dwUnicodeLen=0;

  if (AEC_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, AELB_ASOUTPUT, ae->bColumnSel, TRUE))
    {
      if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, dwUnicodeLen * sizeof(wchar_t)))
      {
        if (pDataTargetW=GlobalLock(hDataTargetW))
        {
          AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)pDataTargetW, (UINT_PTR)-1, AELB_ASOUTPUT, ae->bColumnSel, TRUE);

          //Get Ansi text
          dwAnsiLen=WideCharToMultiByte64(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, NULL, 0, NULL, NULL);

          if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, dwAnsiLen))
          {
            if (pDataTargetA=GlobalLock(hDataTargetA))
            {
              WideCharToMultiByte64(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen, (char *)pDataTargetA, dwAnsiLen, NULL, NULL);
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
    ae->ido.fmtetc[2].cfFormat=(CLIPFORMAT)cfAkelEditColumnSel;
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

HRESULT WINAPI AEIEnumFORMATETC_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj)
{
  if (AE_IsEqualIID(riid, &IID_IUnknown) || AE_IsEqualIID(riid, &IID_IEnumFORMATETC))
  {
    *ppvObj=lpTable;
    AEIEnumFORMATETC_AddRef((LPUNKNOWN)*ppvObj);
    return S_OK;
  }
  else
  {
    *ppvObj=NULL;
    return E_NOINTERFACE;
  }
}

ULONG WINAPI AEIEnumFORMATETC_AddRef(LPUNKNOWN lpTable)
{
  return ++(((AEIEnumFORMATETC *)lpTable)->uRefCount);
}

ULONG WINAPI AEIEnumFORMATETC_Release(LPUNKNOWN lpTable)
{
  if (--((AEIEnumFORMATETC *)lpTable)->uRefCount == 0)
  {
    GlobalFree((HGLOBAL)lpTable);
    return 0;
  }
  return ((AEIEnumFORMATETC *)lpTable)->uRefCount;
}

HRESULT WINAPI AEIEnumFORMATETC_Next(LPUNKNOWN lpTable, ULONG celt, FORMATETC *rgelt, ULONG *pceltFetched)
{
  AEIEnumFORMATETC *lpEnumFormatEtc=(AEIEnumFORMATETC *)lpTable;
  AKELEDIT *ae=(AKELEDIT *)lpEnumFormatEtc->ae;
  ULONG nNumFormats;

  if (!rgelt) return E_POINTER;

  if (lpEnumFormatEtc->nPos < ae->ido.nNumFormats)
  {
    nNumFormats=min(celt, (ULONG)(ae->ido.nNumFormats - lpEnumFormatEtc->nPos));
    xmemcpy(rgelt, &ae->ido.fmtetc[lpEnumFormatEtc->nPos], nNumFormats * sizeof(FORMATETC));
    lpEnumFormatEtc->nPos+=nNumFormats;

    if (pceltFetched)
      *pceltFetched=nNumFormats;
    if (nNumFormats)
      return S_OK;
  }
  return S_FALSE;
}

HRESULT WINAPI AEIEnumFORMATETC_Skip(LPUNKNOWN lpTable, ULONG celt)
{
  AEIEnumFORMATETC *lpEnumFormatEtc=(AEIEnumFORMATETC *)lpTable;

  lpEnumFormatEtc->nPos+=celt;
  return S_OK;
}

HRESULT WINAPI AEIEnumFORMATETC_Reset(LPUNKNOWN lpTable)
{
  AEIEnumFORMATETC *lpEnumFormatEtc=(AEIEnumFORMATETC *)lpTable;

  lpEnumFormatEtc->nPos=0;
  return S_OK;
}

HRESULT WINAPI AEIEnumFORMATETC_Clone(LPUNKNOWN lpTable, IEnumFORMATETC **ppEnum)
{
  AEIEnumFORMATETC *lpEnumFormatEtc=(AEIEnumFORMATETC *)lpTable;
  AEIEnumFORMATETC *objIEnumFORMATETC;

  if ((objIEnumFORMATETC=(AEIEnumFORMATETC *)GlobalAlloc(GPTR, sizeof(AEIEnumFORMATETC))))
  {
    objIEnumFORMATETC->lpTable=lpEnumFormatEtc->lpTable;
    objIEnumFORMATETC->uRefCount=0;
    objIEnumFORMATETC->ae=lpEnumFormatEtc->ae;
    objIEnumFORMATETC->nPos=lpEnumFormatEtc->nPos;
    return AEIEnumFORMATETC_QueryInterface((LPUNKNOWN)objIEnumFORMATETC, &IID_IEnumFORMATETC, (void **)ppEnum);
  }
  else return E_OUTOFMEMORY;
}

BOOL AE_IsEqualIID(const GUID *rguid1, const GUID *rguid2)
{
  return !xmemcmp(rguid1, rguid2, sizeof(GUID));
}

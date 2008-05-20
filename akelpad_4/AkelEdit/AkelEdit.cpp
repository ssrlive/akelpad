/***********************************************************************************
 *               AkelEdit text control v1.0 alpha 1                                *
 *                                                                                 *
 * Copyright 2007-2008 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 ***********************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "AkelEdit.h"
#include "Resources\resource.h"


//// Global variables

HSTACK hWindowsStack={0};
BOOL bAkelEditClassRegisteredA=FALSE;
BOOL bAkelEditClassRegisteredW=FALSE;
UINT cfColumnSel=0;


//// Entry point

#ifndef STATIC_BUILD
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    AE_RegisterClassA(hinstDLL);
    AE_RegisterClassW(hinstDLL);
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
  }
  return TRUE;
}
#endif


//// AkelEdit control

BOOL AE_RegisterClassA(HINSTANCE hInstance)
{
  if (!bAkelEditClassRegisteredA)
  {
    WNDCLASSA wndclass={0};

    wndclass.style        =CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursorA(NULL, (char *)IDC_IBEAM);
    wndclass.hbrBackground=NULL;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AES_CLASSA;
    bAkelEditClassRegisteredA=RegisterClassA(&wndclass);

    if (!cfColumnSel) cfColumnSel=RegisterClipboardFormatA("MSDEVColumnSelect");
  }
  return bAkelEditClassRegisteredA;
}

BOOL AE_RegisterClassW(HINSTANCE hInstance)
{
  if (!bAkelEditClassRegisteredW)
  {
    WNDCLASSW wndclass={0};

    wndclass.style        =CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditProc;
    wndclass.cbClsExtra   =0;
    wndclass.cbWndExtra   =DLGWINDOWEXTRA;
    wndclass.hInstance    =hInstance;
    wndclass.hIcon        =NULL;
    wndclass.hCursor      =LoadCursorW(NULL, (wchar_t *)IDC_IBEAM);
    wndclass.hbrBackground=NULL;
    wndclass.lpszMenuName =NULL;
    wndclass.lpszClassName=AES_CLASSW;
    bAkelEditClassRegisteredW=RegisterClassW(&wndclass);

    if (!cfColumnSel) cfColumnSel=RegisterClipboardFormatW(L"MSDEVColumnSelect");
  }
  return bAkelEditClassRegisteredW;
}

BOOL AE_UnregisterClassA(HINSTANCE hInstance)
{
  if (bAkelEditClassRegisteredA)
  {
    if (UnregisterClassA(AES_CLASSA, hInstance))
      bAkelEditClassRegisteredA=FALSE;
  }
  return !bAkelEditClassRegisteredA;
}

BOOL AE_UnregisterClassW(HINSTANCE hInstance)
{
  if (bAkelEditClassRegisteredW)
  {
    if (UnregisterClassW(AES_CLASSW, hInstance))
      bAkelEditClassRegisteredW=FALSE;
  }
  return !bAkelEditClassRegisteredW;
}

LRESULT CALLBACK AE_EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AKELEDIT *ae;

  if (uMsg == WM_CREATE)
  {
    if (ae=AE_StackWindowInsert(&hWindowsStack))
    {
      ae->bUnicodeWindow=IsWindowUnicode(hWnd);
      ae->hWndEdit=hWnd;
      ae->hWndParent=GetParent(ae->hWndEdit);
      ae->nEditCtrlID=GetDlgCtrlID(ae->hWndEdit);
      ae->hHeap=HeapCreate(0, 0, 0);
      ae->hDC=GetDC(ae->hWndEdit);
      ae->nCaretInsertWidth=1;
      ae->nCaretOvertypeHeight=2;
      ae->bCaretVisible=TRUE;
      ae->crCaret=RGB(0x00, 0x00, 0x00);
      ae->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
      ae->crBasicBk=GetSysColor(COLOR_WINDOW);
      ae->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
      ae->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
      ae->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
      ae->crActiveLineBk=GetSysColor(COLOR_WINDOW);
      ae->hBasicBk=CreateSolidBrush(ae->crBasicBk);
      ae->hSelBk=CreateSolidBrush(ae->crSelBk);
      ae->hActiveLineBk=CreateSolidBrush(ae->crActiveLineBk);
      ae->nTabStop=8;
      ae->bSavePointExist=TRUE;
      ae->dwUndoLimit=(DWORD)-1;
      ae->nInputNewLine=AELB_ASIS;
      ae->nOutputNewLine=AELB_ASIS;
      AE_memcpy(ae->wszWordDelimiters, AES_WORD_DELIMITERSW, (lstrlenW(AES_WORD_DELIMITERSW) + 1) * sizeof(wchar_t));

      GetClientRect(ae->hWndEdit, &ae->rcEdit);
      AE_SetDrawRect(ae, NULL, FALSE);

      if (!ae->bUnicodeWindow)
        AE_SetEditFontA(ae, NULL, FALSE);
      else
        AE_SetEditFontW(ae, NULL, FALSE);

      AE_SetText(ae, L"", 0, ae->nInputNewLine);
      return 0;
    }
    return -1;
  }

  if (ae=AE_StackWindowGet(&hWindowsStack, hWnd))
  {
    if (uMsg >= WM_USER)
    {
      //// AEM_* AkelEdit control messages

      if (uMsg == AEM_SETTEXTA)
      {
        return AE_SetTextAnsi(ae, (char *)lParam, wParam, ae->nInputNewLine);
      }
      if (uMsg == AEM_SETTEXTW)
      {
        return AE_SetText(ae, (wchar_t *)lParam, wParam, ae->nInputNewLine);
      }
      if (uMsg == AEM_REPLACESELA)
      {
        AE_ReplaceSelAnsi(ae, (char *)lParam, wParam, FALSE);
        return 0;
      }
      if (uMsg == AEM_REPLACESELW)
      {
        AE_ReplaceSel(ae, (wchar_t *)lParam, wParam, FALSE);
        return 0;
      }
      if (uMsg == AEM_GETTEXTRANGEA)
      {
        AETEXTRANGEA *tr=(AETEXTRANGEA *)lParam;

        return AE_GetTextRangeAnsi(ae, &tr->cr.ciMin, &tr->cr.ciMax, tr->pText, tr->nNewLine, FALSE, TRUE);
      }
      if (uMsg == AEM_GETTEXTRANGEW)
      {
        AETEXTRANGEW *tr=(AETEXTRANGEW *)lParam;

        return AE_GetTextRange(ae, &tr->cr.ciMin, &tr->cr.ciMax, tr->wpText, tr->nNewLine, FALSE, TRUE);
      }
      if (uMsg == AEM_STREAMOUT)
      {
        AEStreamOut lpCallback=(AEStreamOut)wParam;

        return AE_StreamOut(ae, lpCallback, lParam);
      }
      if (uMsg == AEM_CANPASTE)
      {
        return AE_EditCanPaste(ae);
      }
      if (uMsg == AEM_CANUNDO)
      {
        return AE_EditCanUndo(ae);
      }
      if (uMsg == AEM_CANREDO)
      {
        return AE_EditCanRedo(ae);
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
      if (uMsg == AEM_PASTE)
      {
        AE_EditPasteFromClipboard(ae, lParam);
        return 0;
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
        AE_StackRedoDeleteAll(ae, NULL);
        return 0;
      }
      if (uMsg == AEM_STOPGROUPTYPING)
      {
        AE_StackUndoGroupStop(ae);
        return 0;
      }
      if (uMsg == AEM_GETSEL)
      {
        AECHARINDEX *ciCaret=(AECHARINDEX *)wParam;
        AECHARRANGE *cr=(AECHARRANGE *)lParam;

        if (ciCaret) *ciCaret=ae->ciCaretIndex;
        cr->ciMin=ae->ciSelStartIndex;
        cr->ciMax=ae->ciSelEndIndex;
        return 0;
      }
      if (uMsg == AEM_SETSEL)
      {
        AECHARINDEX *ciCaret=(AECHARINDEX *)wParam;
        AECHARRANGE *cr=(AECHARRANGE *)lParam;

        if (!ciCaret)
        {
          if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciCaretIndex))
            AE_SetSelectionPos(ae, &cr->ciMin, &cr->ciMax, FALSE);
          else
            AE_SetSelectionPos(ae, &cr->ciMax, &cr->ciMin, FALSE);
        }
        else
        {
          if (!AE_IndexCompare(&cr->ciMin, ciCaret))
            AE_SetSelectionPos(ae, &cr->ciMin, &cr->ciMax, FALSE);
          else
            AE_SetSelectionPos(ae, &cr->ciMax, &cr->ciMin, FALSE);
        }
        return 0;
      }
      if (uMsg == AEM_UPDATESEL)
      {
        AE_UpdateSelection(ae);
        return 0;
      }
      if (uMsg == AEM_GETLINECOUNT)
      {
        return ae->nLineCount;
      }
      if (uMsg == AEM_GETINDEX)
      {
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        return AE_GetIndex(ae, wParam, ciCharIndex, ciCharIndex, FALSE);
      }
      if (uMsg == AEM_GETLINEINDEX)
      {
        AELINEDATA *lpLine;
        int nLine=wParam;
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        if (lpLine=AE_StackLineGet(ae, nLine))
        {
          ciCharIndex->nLine=nLine;
          ciCharIndex->lpLine=lpLine;
          ciCharIndex->nCharInLine=0;
          return TRUE;
        }
        return FALSE;
      }
      if (uMsg == AEM_UPDATEINDEX)
      {
        return AE_UpdateIndex(ae, (AECHARINDEX *)lParam);
      }
      if (uMsg == AEM_INDEXCOMPARE)
      {
        return AE_IndexCompare((AECHARINDEX *)wParam, (AECHARINDEX *)lParam);
      }
      if (uMsg == AEM_INDEXSUBTRACT)
      {
        AEINDEXSUBTRACT *aeis=(AEINDEXSUBTRACT *)lParam;

        return AE_IndexSubtract(ae, aeis->ciChar1, aeis->ciChar2, aeis->nNewLine, ae->bColumnSel, TRUE);
      }
      if (uMsg == AEM_INDEXOFFSET)
      {
        AEINDEXOFFSET *aeio=(AEINDEXOFFSET *)lParam;

        return AE_IndexOffset(ae, aeio->ciCharIn, aeio->ciCharOut, aeio->nOffset, aeio->nNewLine);
      }
      if (uMsg == AEM_CHARFROMPOS)
      {
        POINT *pt=(POINT *)wParam;
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        return AE_GetCharFromPos(ae, pt, FALSE, ciCharIndex, NULL, FALSE);
      }
      if (uMsg == AEM_POSFROMCHAR)
      {
        POINT *pt=(POINT *)wParam;
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        AE_GetPosFromChar(ae, ciCharIndex, NULL, pt, TRUE);
        return 0;
      }
      if (uMsg == AEM_LINESCROLL)
      {
        SCROLLINFO si;

        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_ALL;
        GetScrollInfo(ae->hWndEdit, SB_VERT, &si);

        si.nPos+=ae->nCharHeight * lParam;
        si.nPos=(si.nPos / ae->nCharHeight) * ae->nCharHeight;

        AE_ScrollEditWindow(ae, SB_VERT, si.nPos);
        return 0;
      }
      if (uMsg == AEM_SCROLLCARET)
      {
        AE_ScrollToCaret(ae, &ae->ptCaret);
        return 0;
      }
      if (uMsg == AEM_FINDTEXTA)
      {
        return AE_FindTextAnsi(ae, (AEFINDTEXTA *)lParam);
      }
      if (uMsg == AEM_FINDTEXTW)
      {
        return AE_FindText(ae, (AEFINDTEXTW *)lParam);
      }
      if (uMsg == AEM_GETOPTIONS)
      {
        return ae->dwOptions;
      }
      if (uMsg == AEM_SETOPTIONS)
      {
        if (wParam == AECOOP_SET)
          ae->dwOptions=lParam;
        else if (wParam == AECOOP_OR)
          ae->dwOptions|=lParam;
        else if (wParam == AECOOP_AND)
          ae->dwOptions&=lParam;
        else if (wParam == AECOOP_XOR)
          ae->dwOptions&=~lParam;
        return ae->dwOptions;
      }
      if (uMsg == AEM_GETUNDOLIMIT)
      {
        return ae->dwUndoLimit;
      }
      if (uMsg == AEM_SETUNDOLIMIT)
      {
        ae->dwUndoLimit=wParam;
        AE_StackUndoGroupStop(ae);
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

        AE_SetDrawRect(ae, rcDraw, FALSE);
        AE_UpdateScrollBars(ae, SB_BOTH);
        return 0;
      }
      if (uMsg == AEM_GETMARGINS)
      {
        return MAKELONG(ae->rcDraw.left - ae->rcEdit.left, ae->rcEdit.right - ae->rcDraw.right);
      }
      if (uMsg == AEM_SETMARGINS)
      {
        RECT rcDraw=ae->rcDraw;

        if (wParam & EC_LEFTMARGIN)
          rcDraw.left=ae->rcEdit.left + LOWORD(lParam);
        if (wParam & EC_RIGHTMARGIN)
          rcDraw.right=ae->rcEdit.right - HIWORD(lParam);

        AE_SetDrawRect(ae, &rcDraw, FALSE);
        AE_UpdateScrollBars(ae, SB_BOTH);
        return 0;
      }
      if (uMsg == AEM_GETNEWLINE)
      {
        return MAKELONG(ae->nInputNewLine, ae->nOutputNewLine);
      }
      if (uMsg == AEM_SETNEWLINE)
      {
        if (wParam & AENL_INPUT)
          ae->nInputNewLine=LOWORD(lParam);
        if (wParam & AENL_OUTPUT)
          ae->nOutputNewLine=HIWORD(lParam);
        return 0;
      }
      if (uMsg == AEM_GETMODIFY)
      {
        return ae->bModified;
      }
      if (uMsg == AEM_SETMODIFY)
      {
        if (ae->bModified != (int)wParam)
        {
          if (wParam)
          {
            ae->lpSavePoint=NULL;
            ae->bSavePointExist=FALSE;
          }
          else
          {
            AE_StackUndoGroupStop(ae);
            ae->lpSavePoint=ae->lpCurrentUndo;
            ae->bSavePointExist=TRUE;
          }
          AE_SetModify(ae, wParam);
        }
        return 0;
      }
      if (uMsg == AEM_GETCOLORS)
      {
        AECOLORS *aec=(AECOLORS *)lParam;

        if (aec->dwFlags & AECLR_CARET)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crCaret=RGB(0x00, 0x00, 0x00);
          else
            aec->crCaret=ae->crCaret;
        }
        if (aec->dwFlags & AECLR_BASICTEXT)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
          else
            aec->crBasicText=ae->crBasicText;
        }
        if (aec->dwFlags & AECLR_BASICBK)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crBasicBk=GetSysColor(COLOR_WINDOW);
          else
            aec->crBasicBk=ae->crBasicBk;
        }
        if (aec->dwFlags & AECLR_SELTEXT)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
          else
            aec->crSelText=ae->crSelText;
        }
        if (aec->dwFlags & AECLR_SELBK)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
          else
            aec->crSelBk=ae->crSelBk;
        }
        if (aec->dwFlags & AECLR_ACTIVELINETEXT)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
          else
            aec->crActiveLineText=ae->crActiveLineText;
        }
        if (aec->dwFlags & AECLR_ACTIVELINEBK)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            aec->crActiveLineBk=GetSysColor(COLOR_WINDOW);
          else
            aec->crActiveLineBk=ae->crActiveLineBk;
        }
        return 0;
      }
      if (uMsg == AEM_SETCOLORS)
      {
        AECOLORS *aec=(AECOLORS *)lParam;

        if (aec->dwFlags & AECLR_CARET)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crCaret=RGB(0x00, 0x00, 0x00);
          else
            ae->crCaret=aec->crCaret;

          AE_UpdateCaret(ae, TRUE);
        }
        if (aec->dwFlags & AECLR_BASICTEXT)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
          else
            ae->crBasicText=aec->crBasicText;

          InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        }
        if (aec->dwFlags & AECLR_BASICBK)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crBasicBk=GetSysColor(COLOR_WINDOW);
          else
            ae->crBasicBk=aec->crBasicBk;

          if (ae->hBasicBk) DeleteObject(ae->hBasicBk);
          ae->hBasicBk=CreateSolidBrush(ae->crBasicBk);
          InvalidateRect(ae->hWndEdit, &ae->rcEdit, TRUE);
        }
        if (aec->dwFlags & AECLR_SELTEXT)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
          else
            ae->crSelText=aec->crSelText;

          InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        }
        if (aec->dwFlags & AECLR_SELBK)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
          else
            ae->crSelBk=aec->crSelBk;

          if (ae->hSelBk) DeleteObject(ae->hSelBk);
          ae->hSelBk=CreateSolidBrush(ae->crSelBk);
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        }
        if (aec->dwFlags & AECLR_ACTIVELINETEXT)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
          else
            ae->crActiveLineText=aec->crActiveLineText;

          InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        }
        if (aec->dwFlags & AECLR_ACTIVELINEBK)
        {
          if (aec->dwFlags & AECLR_DEFAULT)
            ae->crActiveLineBk=GetSysColor(COLOR_WINDOW);
          else
            ae->crActiveLineBk=aec->crActiveLineBk;

          if (ae->hActiveLineBk) DeleteObject(ae->hActiveLineBk);
          ae->hActiveLineBk=CreateSolidBrush(ae->crActiveLineBk);
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        }
        return 0;
      }
      if (uMsg == AEM_GETOVERTYPE)
      {
        return ae->bOverType;
      }
      if (uMsg == AEM_SETOVERTYPE)
      {
        ae->bOverType=wParam;
        AE_UpdateCaret(ae, FALSE);
        return 0;
      }
      if (uMsg == AEM_GETTABSTOP)
      {
        return ae->nTabStop;
      }
      if (uMsg == AEM_SETTABSTOP)
      {
        ae->nTabStop=wParam;

        AE_UpdateSelection(ae);
        AE_CalcLinesWidth(ae, NULL, NULL, TRUE);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, lParam);
        return 0;
      }
      if (uMsg == AEM_GETWORDDELIMITERS)
      {
        return (LPARAM)ae->wszWordDelimiters;
      }
      if (uMsg == AEM_SETWORDDELIMITERS)
      {
        if (wParam)
          AE_memcpy(ae->wszWordDelimiters, (wchar_t *)wParam, (lstrlenW((wchar_t *)wParam) + 1) * sizeof(wchar_t));
        else
          AE_memcpy(ae->wszWordDelimiters, AES_WORD_DELIMITERSW, (lstrlenW(AES_WORD_DELIMITERSW) + 1) * sizeof(wchar_t));
        return 0;
      }
      if (uMsg == AEM_CHECKCODEPAGE)
      {
        return AE_CheckCodepage(ae, wParam);
      }
    }


    //// WM_* system messages

    if (uMsg == WM_GETFONT)
    {
      return (LRESULT)ae->hFont;
    }
    else if (uMsg == WM_SETFONT)
    {
      if (!ae->bUnicodeWindow)
        AE_SetEditFontA(ae, (HFONT)wParam, TRUE);
      else
        AE_SetEditFontW(ae, (HFONT)wParam, TRUE);

      AE_UpdateSelection(ae);
      AE_CalcLinesWidth(ae, NULL, NULL, TRUE);
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, !lParam);
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
      AE_EditKeyDelete(ae);
      return 0;
    }
    else if (uMsg == WM_UNDO)
    {
      AE_EditUndo(ae);
      return 0;
    }
    else if (uMsg == WM_GETTEXTLENGTH)
    {
      return AE_IndexSubtract(ae, NULL, NULL, ae->nOutputNewLine, FALSE, TRUE);
    }
    else if (uMsg == WM_GETTEXT)
    {
      if (!ae->bUnicodeWindow)
        AE_GetTextAnsi(ae, (char *)lParam, wParam);
      else
        AE_GetText(ae, (wchar_t *)lParam, wParam);
      return 0;
    }
    else if (uMsg == WM_SETTEXT)
    {
      if (!ae->bUnicodeWindow)
        AE_SetTextAnsi(ae, (char *)lParam, (DWORD)-1, ae->nInputNewLine);
      else
        AE_SetText(ae, (wchar_t *)lParam, (DWORD)-1, ae->nInputNewLine);
      return 0;
    }
    else if (uMsg == WM_SIZE)
    {
      ae->rcDraw.right+=LOWORD(lParam) - ae->rcEdit.right;
      ae->rcDraw.bottom+=HIWORD(lParam) - ae->rcEdit.bottom;
      ae->rcEdit.right=LOWORD(lParam);
      ae->rcEdit.bottom=HIWORD(lParam);

      if (lParam)
      {
        AE_SetDrawRect(ae, &ae->rcDraw, FALSE);
/*
        AE_WrapLines(ae);
*/
        AE_UpdateScrollBars(ae, SB_BOTH);
        AE_UpdateEditWindow(ae->hWndEdit, TRUE);
      }
      return 0;
    }
    else if (uMsg == WM_GETDLGCODE)
    {
      UINT uCode=DLGC_WANTCHARS|DLGC_WANTARROWS;

      if (lParam && (((MSG *)lParam)->message == WM_KEYDOWN))
      {
        int nVk=(int)((MSG *)lParam)->wParam;

        if (nVk == VK_RETURN && (ae->dwOptions & AECO_WANTRETURN))
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
    else if (uMsg == WM_CHAR)
    {
      if (wParam != VK_RETURN &&
          wParam != VK_BACK &&
          wParam != VK_ESCAPE)
      {
        if (GetKeyState(VK_CONTROL) >= 0)
        {
          AE_EditChar(ae, wParam);
        }
      }
      return 0;
    }
    else if (uMsg == WM_KEYDOWN ||
             uMsg == WM_SYSKEYDOWN)
    {
      AECHARINDEX ciCharIn=ae->ciCaretIndex;
      AECHARINDEX ciCharOut=ae->ciCaretIndex;
      BOOL bAlt=FALSE;
      BOOL bShift=FALSE;
      BOOL bControl=FALSE;
      BOOL bRedrawAllSelection=FALSE;

      if (GetKeyState(VK_MENU) < 0)
        bAlt=TRUE;
      if (GetKeyState(VK_SHIFT) < 0)
        bShift=TRUE;
      if (GetKeyState(VK_CONTROL) < 0)
        bControl=TRUE;

      if (wParam == VK_RETURN)
      {
        AE_EditKeyReturn(ae);
        return 0;
      }
      if (wParam == VK_BACK)
      {
        AE_EditKeyBackspace(ae);
        return 0;
      }
      if (wParam == VK_DELETE)
      {
        AE_EditKeyDelete(ae);
        return 0;
      }
      if (wParam == VK_TAB)
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditChar(ae, VK_TAB);
        }
        return 0;
      }
      if (wParam == VK_INSERT)
      {
        if (!bControl && !bShift && !bAlt)
        {
          ae->bOverType=!ae->bOverType;
          AE_UpdateCaret(ae, FALSE);
        }
        return 0;
      }
      if (wParam == 'X')
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditCut(ae);
        }
        return 0;
      }
      if (wParam == 'C')
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditCopyToClipboard(ae);
        }
        return 0;
      }
      if (wParam == 'V')
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditPasteFromClipboard(ae, FALSE);
        }
        if (bControl && bShift && !bAlt)
        {
          AE_EditPasteFromClipboard(ae, TRUE);
        }
        return 0;
      }
      if (wParam == 'Z')
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditUndo(ae);
        }
        if (bControl && bShift && !bAlt)
        {
          AE_EditRedo(ae);
        }
        return 0;
      }
      if (wParam == 'Y')
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditRedo(ae);
        }
        return 0;
      }
      if (wParam == 'A')
      {
        if (bControl && !bShift && !bAlt)
        {
          AE_EditSelectAll(ae);
        }
        return 0;
      }

      if ((wParam == VK_HOME ||
           wParam == VK_END ||
           wParam == VK_LEFT ||
           wParam == VK_RIGHT ||
           wParam == VK_UP ||
           wParam == VK_DOWN ||
           wParam == VK_PRIOR ||
           wParam == VK_NEXT) &&
           ae->rcDraw.bottom - ae->rcDraw.top > 0 &&
           ae->rcDraw.right - ae->rcDraw.left > 0)
      {
        if (ae->bColumnSel != bAlt)
          bRedrawAllSelection=TRUE;
        ae->bColumnSel=bAlt;

        if (wParam == VK_HOME)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelStartIndex;

          if (bControl)
          {
            AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCharOut, ae->bColumnSel);
          }
          else
          {
            ciCharOut.nLine=ciCharIn.nLine;
            ciCharOut.lpLine=ciCharIn.lpLine;
            ciCharOut.nCharInLine=0;
          }
        }
        else if (wParam == VK_END)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelEndIndex;

          if (bControl)
          {
            AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciCharOut, ae->bColumnSel);
          }
          else
          {
            ciCharOut.nLine=ciCharIn.nLine;
            ciCharOut.lpLine=ciCharIn.lpLine;
            ciCharOut.nCharInLine=ciCharIn.lpLine->nLineLen;
          }
        }
        else if (wParam == VK_LEFT)
        {
          if (bControl)
          {
            if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
              ciCharIn=ae->ciSelStartIndex;

            AE_GetPrevWord(ae, &ciCharIn, &ciCharOut, NULL, ae->bColumnSel);
//            AE_GetPrevBreak(ae, &ciCharIn, &ciCharOut, ae->bColumnSel);
          }
          else
          {
            if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            {
              ciCharIn=ae->ciSelStartIndex;
              ciCharOut=ae->ciSelStartIndex;
            }
            else AE_GetIndex(ae, AEGI_PREVCHAR, &ciCharIn, &ciCharOut, ae->bColumnSel);
          }
        }
        else if (wParam == VK_RIGHT)
        {
          if (bControl)
          {
            if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
              ciCharIn=ae->ciSelEndIndex;

            AE_GetNextWord(ae, &ciCharIn, NULL, &ciCharOut, ae->bColumnSel);
//            AE_GetNextBreak(ae, &ciCharIn, &ciCharOut, ae->bColumnSel);
          }
          else
          {
            if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            {
              ciCharIn=ae->ciSelEndIndex;
              ciCharOut=ae->ciSelEndIndex;
            }
            else AE_GetIndex(ae, AEGI_NEXTCHAR, &ciCharIn, &ciCharOut, ae->bColumnSel);
          }
        }
        else if (wParam == VK_UP)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelStartIndex;

          if (AE_GetIndex(ae, AEGI_PREVLINE, &ciCharIn, &ciCharOut, ae->bColumnSel))
          {
            if (bControl)
              ae->nHorizCaretPos=ae->nHScrollPos;
            else
              AE_GetCharInLine(ae, ciCharOut.lpLine->wpLine, ciCharOut.lpLine->nLineLen, ae->nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
          }
        }
        else if (wParam == VK_DOWN)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelEndIndex;

          if (AE_GetIndex(ae, AEGI_NEXTLINE, &ciCharIn, &ciCharOut, ae->bColumnSel))
          {
            if (bControl)
              ae->nHorizCaretPos=ae->nHScrollPos;
            else
              AE_GetCharInLine(ae, ciCharOut.lpLine->wpLine, ciCharOut.lpLine->nLineLen, ae->nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
          }
        }
        else if (wParam == VK_PRIOR)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelStartIndex;

          if (bControl)
          {
            AE_GetIndex(ae, AEGI_FIRSTVISIBLELINE, NULL, &ciCharOut, ae->bColumnSel);
          }
          else
          {
            ciCharOut.nLine=max(ciCharIn.nLine - (ae->rcDraw.bottom - ae->rcDraw.top) / ae->nCharHeight, 0);

            if (ciCharIn.nLine != ciCharOut.nLine)
            {
              AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos - (ae->rcDraw.bottom - ae->rcDraw.top));
              AE_UpdateIndex(ae, &ciCharOut);
              AE_GetCharInLine(ae, ciCharOut.lpLine->wpLine, ciCharOut.lpLine->nLineLen, ae->nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
            }
          }
        }
        else if (wParam == VK_NEXT)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelEndIndex;

          if (bControl)
          {
            AE_GetIndex(ae, AEGI_LASTVISIBLELINE, NULL, &ciCharOut, ae->bColumnSel);
          }
          else
          {
            ciCharOut.nLine=min(ciCharIn.nLine + (ae->rcDraw.bottom - ae->rcDraw.top) / ae->nCharHeight, ae->nLineCount);

            if (ciCharIn.nLine != ciCharOut.nLine)
            {
              AE_ScrollEditWindow(ae, SB_VERT, ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top));
              AE_UpdateIndex(ae, &ciCharOut);
              AE_GetCharInLine(ae, ciCharOut.lpLine->wpLine, ciCharOut.lpLine->nLineLen, ae->nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
            }
          }
        }

        //Make beep
        if (!(ae->dwOptions & AECO_DISABLEBEEP))
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
            if (!AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelStartIndex))
            {
              ciSelEnd=ae->ciSelEndIndex;
            }
            else if (!AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex))
            {
              ciSelEnd=ae->ciSelStartIndex;
            }
            AE_SetSelectionPos(ae, &ciCharOut, &ciSelEnd, ae->bColumnSel);
          }
          else AE_SetSelectionPos(ae, &ciCharOut, &ciCharOut, ae->bColumnSel);

          AE_StackUndoGroupStop(ae);
        }

        //Set horizontal caret
        if (wParam == VK_HOME ||
            wParam == VK_END ||
            wParam == VK_LEFT ||
            wParam == VK_RIGHT)
        {
          ae->nHorizCaretPos=ae->ptCaret.x;
        }

        //Redraw lines
        if (bRedrawAllSelection)
        {
          AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
        }
        return 0;
      }
    }
    else if (uMsg == WM_HSCROLL)
    {
      SCROLLINFO si;

      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_ALL;
      GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);

      if (LOWORD(wParam) == SB_LINELEFT)
      {
        // user clicked left arrow
        si.nPos-=ae->nAveCharWidth;
      }
      else if (LOWORD(wParam) == SB_LINERIGHT)
      {
        // user clicked right arrow
        si.nPos+=ae->nAveCharWidth;
      }
      else if (LOWORD(wParam) == SB_PAGELEFT)
      {
        // user clicked shaft left of the scroll box
        si.nPos-=si.nPage;
      }
      else if (LOWORD(wParam) == SB_PAGERIGHT)
      {
        // user clicked shaft right of the scroll box
        si.nPos+=si.nPage;
      }
      else if (LOWORD(wParam) == SB_THUMBTRACK)
      {
        // user dragged the scroll box
        si.nPos=si.nTrackPos;
      }
      AE_ScrollEditWindow(ae, SB_HORZ, si.nPos);
      return 0;
    }
    else if (uMsg == WM_VSCROLL)
    {
      SCROLLINFO si;

      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_ALL;
      GetScrollInfo(ae->hWndEdit, SB_VERT, &si);

      if (LOWORD(wParam) == SB_TOP)
      {
        // user clicked the HOME keyboard key
        si.nPos=si.nMin;
      }
      else if (LOWORD(wParam) == SB_BOTTOM)
      {
        // user clicked the END keyboard key
        si.nPos=si.nMax;
      }
      else if (LOWORD(wParam) == SB_LINEUP)
      {
        // user clicked the top arrow
        si.nPos-=ae->nCharHeight;
      }
      else if (LOWORD(wParam) == SB_LINEDOWN)
      {
        // user clicked the bottom arrow
        si.nPos+=ae->nCharHeight;
      }
      else if (LOWORD(wParam) == SB_PAGEUP)
      {
        // user clicked the shaft above the scroll box
        si.nPos-=si.nPage;
      }
      else if (LOWORD(wParam) == SB_PAGEDOWN)
      {
        // user clicked the shaft below the scroll box
        si.nPos+=si.nPage;
      }
      else if (LOWORD(wParam) == SB_THUMBTRACK)
      {
        // user dragged the scroll box
        si.nPos=si.nTrackPos;
      }
      AE_ScrollEditWindow(ae, SB_VERT, si.nPos);
      return 0;
    }
    else if (uMsg == WM_LBUTTONDOWN)
    {
      if (!ae->dwMouseMoveTimer)
      {
        POINT ptPos;
        BOOL bAlt=FALSE;
        BOOL bShift=FALSE;
        BOOL bRedrawAllSelection=FALSE;

        if (GetKeyState(VK_MENU) < 0)
          bAlt=TRUE;
        if (GetKeyState(VK_SHIFT) < 0)
          bShift=TRUE;
        if (ae->bColumnSel != bAlt)
          bRedrawAllSelection=TRUE;
        ae->bColumnSel=bAlt;

        if (GetFocus() != ae->hWndEdit)
          SetFocus(ae->hWndEdit);
        ptPos.x=LOWORD(lParam);
        ptPos.y=HIWORD(lParam);
        AE_SetMouseSelection(ae, &ptPos, bShift, ae->bColumnSel);

        //Redraw lines
        if (bRedrawAllSelection)
        {
          AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
        }

        //Timer
        ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
        SetCapture(ae->hWndEdit);
      }
      return 0;
    }
    else if (uMsg == WM_LBUTTONDBLCLK)
    {
      AECHARINDEX ciPrevWord;
      AECHARINDEX ciNextWord;

      if (!AE_GetPrevBreak(ae, &ae->ciCaretIndex, &ciPrevWord, ae->bColumnSel))
        ciPrevWord=ae->ciCaretIndex;
      if (!AE_GetNextBreak(ae, &ciPrevWord, &ciNextWord, ae->bColumnSel))
        ciNextWord=ae->ciCaretIndex;
      AE_SetSelectionPos(ae, &ciNextWord, &ciPrevWord, ae->bColumnSel);
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    else if (uMsg == WM_TIMER)
    {
      if (wParam == AETIMERID_MOUSEMOVE)
      {
        POINT ptPos;

        GetCursorPos(&ptPos);
        ScreenToClient(ae->hWndEdit, &ptPos);
        AE_SetMouseSelection(ae, &ptPos, TRUE, ae->bColumnSel);
      }
      return 0;
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
      if (ae->dwMouseMoveTimer)
      {
        POINT ptPos;

        GetCursorPos(&ptPos);
        ScreenToClient(ae->hWndEdit, &ptPos);
        AE_SetMouseSelection(ae, &ptPos, TRUE, ae->bColumnSel);
      }
      else
      {
        POINT ptPos;

        ptPos.x=LOWORD(lParam);
        ptPos.y=HIWORD(lParam);
/*
        if (AE_IsCursorOnSelection(ae, &ptPos))
        {
          SetCursor(LoadCursor(NULL, IDC_ARROW));
        }
*/
      }
      return 0;
    }
    else if (uMsg == WM_LBUTTONUP)
    {
      if (ae->dwMouseMoveTimer)
      {
        KillTimer(ae->hWndEdit, ae->dwMouseMoveTimer);
        ae->dwMouseMoveTimer=0;
        ReleaseCapture();
      }
      return 0;
    }
    else if (uMsg == WM_CAPTURECHANGED)
    {
      if (ae->dwMouseMoveTimer)
      {
        KillTimer(ae->hWndEdit, ae->dwMouseMoveTimer);
        ae->dwMouseMoveTimer=0;
        ReleaseCapture();
      }
    }
    else if (uMsg == WM_MOUSEWHEEL)
    {
      DWORD dwLines;

      SystemParametersInfoA(SPI_GETWHEELSCROLLLINES, 0, &dwLines, 0);

      if ((short)HIWORD(wParam) < 0)
      {
        if (dwLines == (DWORD)-1)
          SendMessage(ae->hWndEdit, WM_VSCROLL, SB_PAGEDOWN, 0);
        else
          SendMessage(ae->hWndEdit, AEM_LINESCROLL, 0, dwLines);
      }
      else
      {
        if (dwLines == (DWORD)-1)
          SendMessage(ae->hWndEdit, WM_VSCROLL, SB_PAGEUP, 0);
        else
          SendMessage(ae->hWndEdit, AEM_LINESCROLL, 0, -(int)dwLines);
      }
      return 0;
    }
    else if (uMsg == WM_SETFOCUS)
    {
      ae->bFocus=TRUE;
      AE_UpdateCaret(ae, FALSE);
      return 0;
    }
    else if (uMsg == WM_KILLFOCUS)
    {
      ae->bFocus=FALSE;
      AE_StackUndoGroupStop(ae);
      DestroyCaret();
      return 0;
    }
    else if (uMsg == WM_SYSCOLORCHANGE)
    {
    }
    else if (uMsg == WM_ENABLE)
    {
      if (wParam)
      {
        if (ae->hBasicBk) DeleteObject(ae->hBasicBk);
        ae->hBasicBk=CreateSolidBrush(ae->crBasicBk);
        if (ae->hSelBk) DeleteObject(ae->hSelBk);
        ae->hSelBk=CreateSolidBrush(ae->crSelBk);
        if (ae->hActiveLineBk) DeleteObject(ae->hActiveLineBk);
        ae->hActiveLineBk=CreateSolidBrush(ae->crActiveLineBk);

        InvalidateRect(ae->hWndEdit, &ae->rcEdit, TRUE);
      }
      else
      {
        if (ae->hBasicBk) DeleteObject(ae->hBasicBk);
        ae->hBasicBk=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        if (ae->hSelBk) DeleteObject(ae->hSelBk);
        ae->hSelBk=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        if (ae->hActiveLineBk) DeleteObject(ae->hActiveLineBk);
        ae->hActiveLineBk=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

        InvalidateRect(ae->hWndEdit, &ae->rcEdit, TRUE);
      }
      return 0;
    }
    else if (uMsg == WM_ERASEBKGND)
    {
      FillRect((HDC)wParam, &ae->rcPaint, ae->hBasicBk);
      return 1;
    }
    else if (uMsg == WM_PAINT)
    {
      PAINTSTRUCT ps;

      if (GetUpdateRect(ae->hWndEdit, &ae->rcPaint, FALSE))
      {
        if (BeginPaint(ae->hWndEdit, &ps))
        {
          HRGN hDrawRgn;
          AELINEINDEX liDrawLine;
          RECT rcDraw;
          POINT ptDraw;
          SIZE sizeLine;
          RECT rcSpace;
          wchar_t *wpLine;
          int nLineSelection;
          int nLineLen;
          int nLineWidth;
          DWORD dwColorBG;
          DWORD dwColorText;
          HBRUSH hbrBG;
          int nMaxDrawCharsCount;
          int nTabWidth;
          int nLastTabIndexInLine;
          int i;

          //Set region
          hDrawRgn=CreateRectRgn(ae->rcDraw.left, ae->rcDraw.top, ae->rcDraw.right, ae->rcDraw.bottom);
          if (hDrawRgn) SelectObject(ps.hdc, hDrawRgn);

          //Set font
          if (ae->hFont) SelectObject(ps.hdc, ae->hFont);

          rcDraw=ps.rcPaint;
          if (rcDraw.top + ae->nCharHeight <= ae->rcDraw.top)
            rcDraw.top=ae->rcDraw.top;
          if (rcDraw.bottom > ae->rcDraw.bottom)
            rcDraw.bottom=ae->rcDraw.bottom;

          if (rcDraw.right > ae->rcDraw.left &&
              rcDraw.bottom > ae->rcDraw.top)
          {
            liDrawLine.nLine=(ae->nVScrollPos + (rcDraw.top - ae->rcDraw.top)) / ae->nCharHeight;
            liDrawLine.lpLine=AE_StackLineGet(ae, liDrawLine.nLine);
            ptDraw.y=(liDrawLine.nLine * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;
            ae->liFirstDrawLine=liDrawLine;
          }
          else liDrawLine.lpLine=NULL;

          while (liDrawLine.lpLine)
          {
            //Draw line
            ptDraw.x=ae->rcDraw.left - ae->nHScrollPos;
            nLineWidth=0;
            nMaxDrawCharsCount=0;
            wpLine=liDrawLine.lpLine->wpLine;
            nLastTabIndexInLine=-1;

            if (liDrawLine.lpLine->nSelStart == 0 && liDrawLine.lpLine->nSelEnd == liDrawLine.lpLine->nLineLen)
            {
              nLineSelection=AELS_FULL;
              dwColorText=ae->crSelText;
              dwColorBG=ae->crSelBk;
              hbrBG=ae->hSelBk;
            }
            else if (liDrawLine.lpLine->nSelStart == liDrawLine.lpLine->nSelEnd)
            {
              nLineSelection=AELS_EMPTY;

              if (liDrawLine.lpLine == ae->ciCaretIndex.lpLine)
              {
                dwColorText=ae->crActiveLineText;
                dwColorBG=ae->crActiveLineBk;
                hbrBG=ae->hActiveLineBk;
              }
              else
              {
                dwColorText=ae->crBasicText;
                dwColorBG=ae->crBasicBk;
                hbrBG=ae->hBasicBk;
              }
            }
            else
            {
              nLineSelection=AELS_PARTLY;

              if (liDrawLine.lpLine == ae->ciCaretIndex.lpLine)
              {
                dwColorText=ae->crActiveLineText;
                dwColorBG=ae->crActiveLineBk;
                hbrBG=ae->hActiveLineBk;
              }
              else
              {
                dwColorText=ae->crBasicText;
                dwColorBG=ae->crBasicBk;
                hbrBG=ae->hBasicBk;
              }
            }

            for (i=0; i < liDrawLine.lpLine->nLineLen; ++i)
            {
              if (nLineSelection == AELS_PARTLY)
              {
                if (liDrawLine.lpLine->nSelStart == i)
                {
                  SetTextColor(ps.hdc, dwColorText);
                  SetBkColor(ps.hdc, dwColorBG);
                  nLineLen=i - (wpLine - liDrawLine.lpLine->wpLine);

                  if (nLineLen)
                  {
                    if (AE_GetTextExtentPoint32(ae, wpLine, nLineLen, &sizeLine))
                    {
                      if (ptDraw.x + nLineWidth + sizeLine.cx > ae->rcDraw.left && ptDraw.x + nLineWidth < ae->rcDraw.right)
                        TextOutW(ps.hdc, ptDraw.x + nLineWidth, ptDraw.y, wpLine, nLineLen);
                      nLineWidth+=sizeLine.cx;
                      wpLine+=nLineLen;
                    }
                  }
                  nMaxDrawCharsCount=0;
                  dwColorText=ae->crSelText;
                  dwColorBG=ae->crSelBk;
                  hbrBG=ae->hSelBk;
                }
                else if (liDrawLine.lpLine->nSelEnd == i)
                {
                  SetTextColor(ps.hdc, dwColorText);
                  SetBkColor(ps.hdc, dwColorBG);
                  nLineLen=i - (wpLine - liDrawLine.lpLine->wpLine);

                  if (nLineLen)
                  {
                    if (AE_GetTextExtentPoint32(ae, wpLine, nLineLen, &sizeLine))
                    {
                      if (ptDraw.x + nLineWidth + sizeLine.cx > ae->rcDraw.left && ptDraw.x + nLineWidth < ae->rcDraw.right)
                        TextOutW(ps.hdc, ptDraw.x + nLineWidth, ptDraw.y, wpLine, nLineLen);
                      nLineWidth+=sizeLine.cx;
                      wpLine+=nLineLen;
                    }
                  }
                  nMaxDrawCharsCount=0;

                  if (liDrawLine.lpLine == ae->ciCaretIndex.lpLine)
                  {
                    dwColorText=ae->crActiveLineText;
                    dwColorBG=ae->crActiveLineBk;
                    hbrBG=ae->hActiveLineBk;
                  }
                  else
                  {
                    dwColorText=ae->crBasicText;
                    dwColorBG=ae->crBasicBk;
                    hbrBG=ae->hBasicBk;
                  }
                }
              }

              if (liDrawLine.lpLine->wpLine[i] == L'\t')
              {
                nLineLen=i - (wpLine - liDrawLine.lpLine->wpLine);

                if (nLineLen)
                {
                  if (AE_GetTextExtentPoint32(ae, wpLine, nLineLen, &sizeLine))
                  {
                    if (ptDraw.x + nLineWidth + sizeLine.cx > ae->rcDraw.left && ptDraw.x + nLineWidth < ae->rcDraw.right)
                    {
                      SetTextColor(ps.hdc, dwColorText);
                      SetBkColor(ps.hdc, dwColorBG);
                      TextOutW(ps.hdc, ptDraw.x + nLineWidth, ptDraw.y, wpLine, nLineLen);
                    }
                    nLineWidth+=sizeLine.cx;
                    wpLine+=nLineLen;
                  }
                }
                nMaxDrawCharsCount=0;

                if (nLastTabIndexInLine == -1 || i - nLastTabIndexInLine > ae->nTabStop)
                  nTabWidth=ae->nAveCharWidth * (ae->nTabStop - i % ae->nTabStop);
                else
                  nTabWidth=ae->nAveCharWidth * (ae->nTabStop - (i - nLastTabIndexInLine - 1));

                rcSpace.left=ptDraw.x + nLineWidth;
                rcSpace.top=ptDraw.y;
                rcSpace.right=rcSpace.left + nTabWidth;
                rcSpace.bottom=rcSpace.top + ae->nCharHeight;
                FillRect(ps.hdc, &rcSpace, hbrBG);
                nLineWidth+=nTabWidth;
                wpLine+=1;
                nLastTabIndexInLine=i;
              }

              if (nMaxDrawCharsCount == 2048)
              {
                nLineLen=i - (wpLine - liDrawLine.lpLine->wpLine);

                if (nLineLen)
                {
                  if (AE_GetTextExtentPoint32(ae, wpLine, nLineLen, &sizeLine))
                  {
                    if (ptDraw.x + nLineWidth + sizeLine.cx > ae->rcDraw.left && ptDraw.x + nLineWidth < ae->rcDraw.right)
                    {
                      SetTextColor(ps.hdc, dwColorText);
                      SetBkColor(ps.hdc, dwColorBG);
                      TextOutW(ps.hdc, ptDraw.x + nLineWidth, ptDraw.y, wpLine, nLineLen);
                    }
                    nLineWidth+=sizeLine.cx;
                    wpLine+=nLineLen;
                  }
                }
                nMaxDrawCharsCount=0;
              }
              ++nMaxDrawCharsCount;
            }

            SetTextColor(ps.hdc, dwColorText);
            SetBkColor(ps.hdc, dwColorBG);
            nLineLen=i - (wpLine - liDrawLine.lpLine->wpLine);

            if (nLineLen)
            {
              if (AE_GetTextExtentPoint32(ae, wpLine, nLineLen, &sizeLine))
              {
                if (ptDraw.x + nLineWidth + sizeLine.cx > ae->rcDraw.left && ptDraw.x + nLineWidth < ae->rcDraw.right)
                  TextOutW(ps.hdc, ptDraw.x + nLineWidth, ptDraw.y, wpLine, nLineLen);
                nLineWidth+=sizeLine.cx;
                wpLine+=nLineLen;
              }
            }

            if (ae->bColumnSel)
            {
              if (nLineSelection == AELS_PARTLY)
              {
                nLineLen=liDrawLine.lpLine->nLineLen;

                if (liDrawLine.lpLine->nSelStart >= liDrawLine.lpLine->nLineLen)
                {
                  if (liDrawLine.lpLine == ae->ciCaretIndex.lpLine)
                  {
                    dwColorText=ae->crActiveLineText;
                    dwColorBG=ae->crActiveLineBk;
                    hbrBG=ae->hActiveLineBk;
                  }
                  else
                  {
                    dwColorText=ae->crBasicText;
                    dwColorBG=ae->crBasicBk;
                    hbrBG=ae->hBasicBk;
                  }
                  sizeLine.cx=(liDrawLine.lpLine->nSelStart - nLineLen) * ae->nSpaceCharWidth;

                  rcSpace.left=ptDraw.x + nLineWidth;
                  rcSpace.top=ptDraw.y;
                  rcSpace.right=rcSpace.left + sizeLine.cx;
                  rcSpace.bottom=rcSpace.top + ae->nCharHeight;
                  FillRect(ps.hdc, &rcSpace, hbrBG);
                  nLineWidth+=sizeLine.cx;
                  nLineLen+=(liDrawLine.lpLine->nSelStart - nLineLen);
                }
                if (liDrawLine.lpLine->nSelEnd > liDrawLine.lpLine->nLineLen)
                {
                  dwColorText=ae->crSelText;
                  dwColorBG=ae->crSelBk;
                  hbrBG=ae->hSelBk;
                  sizeLine.cx=(liDrawLine.lpLine->nSelEnd - nLineLen) * ae->nSpaceCharWidth;

                  rcSpace.left=ptDraw.x + nLineWidth;
                  rcSpace.top=ptDraw.y;
                  rcSpace.right=rcSpace.left + sizeLine.cx;
                  rcSpace.bottom=rcSpace.top + ae->nCharHeight;
                  FillRect(ps.hdc, &rcSpace, hbrBG);
                  nLineWidth+=sizeLine.cx;
                  nLineLen+=(liDrawLine.lpLine->nSelEnd - nLineLen);
                }
              }
            }
            else
            {
              //Select new line space
              if (liDrawLine.nLine >= ae->ciSelStartIndex.nLine &&
                  liDrawLine.nLine < ae->ciSelEndIndex.nLine)
              {
                hbrBG=ae->hSelBk;

                rcSpace.left=ptDraw.x + nLineWidth;
                rcSpace.top=ptDraw.y;
                rcSpace.right=rcSpace.left + ae->nAveCharWidth;
                rcSpace.bottom=rcSpace.top + ae->nCharHeight;
                FillRect(ps.hdc, &rcSpace, hbrBG);
                nLineWidth+=ae->nAveCharWidth;
                nLineLen+=1;
              }
            }

            if (liDrawLine.lpLine == ae->ciCaretIndex.lpLine)
            {
              dwColorText=ae->crActiveLineText;
              dwColorBG=ae->crActiveLineBk;
              hbrBG=ae->hActiveLineBk;
            }
            else
            {
              dwColorText=ae->crBasicText;
              dwColorBG=ae->crBasicBk;
              hbrBG=ae->hBasicBk;
            }
            rcSpace.left=ptDraw.x + nLineWidth;
            rcSpace.top=ptDraw.y;
            rcSpace.right=ae->rcDraw.right;
            rcSpace.bottom=rcSpace.top + ae->nCharHeight;
            if (rcSpace.left < rcSpace.right)
              FillRect(ps.hdc, &rcSpace, hbrBG);

            ptDraw.y+=ae->nCharHeight;
            if (ptDraw.y >= rcDraw.bottom)
              break;

            ++liDrawLine.nLine;

            liDrawLine.lpLine=liDrawLine.lpLine->next;
          }
          if (hDrawRgn) DeleteObject(hDrawRgn);
          EndPaint(ae->hWndEdit, &ps);
        }
      }
      return 0;
    }
    else if (uMsg == WM_DESTROY)
    {
      if (ae->hDC)
      {
        if (ae->hBasicBk)
        {
          DeleteObject(ae->hBasicBk);
          ae->hBasicBk=NULL;
        }
        if (ae->hSelBk)
        {
          DeleteObject(ae->hSelBk);
          ae->hSelBk=NULL;
        }
        if (ae->hActiveLineBk)
        {
          DeleteObject(ae->hActiveLineBk);
          ae->hActiveLineBk=NULL;
        }
        if (ae->hCaretInsert)
        {
          DeleteObject(ae->hCaretInsert);
          ae->hCaretInsert=NULL;
        }
        if (ae->hCaretOvertype)
        {
          DeleteObject(ae->hCaretOvertype);
          ae->hCaretOvertype=NULL;
        }
        ReleaseDC(ae->hWndEdit, ae->hDC);
        ae->hDC=NULL;
      }
      AE_StackWindowDelete(&hWindowsStack, hWnd);
      ae=NULL;
      return 0;
    }
  }

  if (ae)
  {
    if (!ae->bUnicodeWindow)
      return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    else
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }
  else
  {
    if (!IsWindowUnicode(hWnd))
      return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    else
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }
}

LPVOID AE_HeapAlloc(AKELEDIT *ae, DWORD dwFlags, SIZE_T dwBytes)
{
  LPVOID lpResult;
  HANDLE hHeap;

  if (!ae)
    hHeap=GetProcessHeap();
  else
    hHeap=ae->hHeap;

  if (!(lpResult=HeapAlloc(hHeap, dwFlags, dwBytes)))
  {
    //Send AEN_ERRSPACE
    if (ae)
    {
      NMHDR hdr;

      hdr.hwndFrom=ae->hWndEdit;
      hdr.idFrom=ae->nEditCtrlID;
      hdr.code=AEN_ERRSPACE;
      SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&hdr);
    }
  }
  return lpResult;
}

BOOL AE_HeapFree(AKELEDIT *ae, DWORD dwFlags, LPVOID lpMem)
{
  HANDLE hHeap;

  if (!ae)
    hHeap=GetProcessHeap();
  else
    hHeap=ae->hHeap;

  return HeapFree(hHeap, 0, lpMem);
}

int AE_HeapStackInsert(AKELEDIT *ae, stack **first, stack **last, stack **element, int nIndex, int nBytes)
{
  stack *tmp;
  int nCount;

  *element=NULL;

  if (nIndex > 0)
  {
    tmp=*last;
    nCount=1;
  }
  else if (nIndex < 0)
  {
    tmp=*first;
    nCount=-1;
  }
  else return 1;

  while ((tmp) || (nCount == nIndex))
  {
    if (nCount == nIndex)
    {
      if (*element=(stack *)AE_HeapAlloc(ae, HEAP_ZERO_MEMORY, nBytes))
      {
        if (!tmp)
        {
          if (nIndex > 0)
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
        }
        else if (nIndex > 0)
        {
          if (tmp == *last) *last=*element;
          else tmp->next->prev=*element;

          (*element)->prev=tmp;
          (*element)->next=tmp->next;
          tmp->next=*element;
        }
        else
        {
          if (tmp == *first) *first=*element;
          else tmp->prev->next=*element;

          (*element)->next=tmp;
          (*element)->prev=tmp->prev;
          tmp->prev=*element;
        }
      }
      else return 2;

      return 0;
    }
    if (nIndex > 0)
    {
      tmp=tmp->prev;
      ++nCount;
    }
    else
    {
      tmp=tmp->next;
      --nCount;
    }
  }
  return 1;
}

int AE_HeapStackInsertBefore(AKELEDIT *ae, stack **first, stack **last, stack **element, stack *index, int nBytes)
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

int AE_HeapStackInsertAfter(AKELEDIT *ae, stack **first, stack **last, stack **element, stack *index, int nBytes)
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
  stack *tmp1=*last;
  stack *tmp2;

  while (tmp1)
  {
    tmp2=tmp1->prev;
    AE_HeapFree(ae, 0, (LPVOID)tmp1);
    tmp1=tmp2;
  }
  *last=NULL;
  *first=NULL;
}

AKELEDIT* AE_StackWindowInsert(HSTACK *hStack)
{
  AKELEDIT *lpElement=NULL;

  AE_HeapStackInsert(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, 1, sizeof(AKELEDIT));
  return lpElement;
}

AKELEDIT* AE_StackWindowGet(HSTACK *hStack, HWND hWndEdit)
{
  AKELEDIT *lpElement=(AKELEDIT *)hStack->last;

  while (lpElement)
  {
    if (lpElement->hWndEdit == hWndEdit)
      break;

    lpElement=lpElement->prev;
  }
  return lpElement;
}

void AE_StackWindowDelete(HSTACK *hStack, HWND hWndEdit)
{
  AKELEDIT *lpElement=(AKELEDIT *)hStack->last;

  while (lpElement)
  {
    if (lpElement->hWndEdit == hWndEdit)
    {
      if (lpElement->hHeap)
      {
        if (HeapDestroy(lpElement->hHeap))
          lpElement->hHeap=NULL;

//        AE_StackLineFree(lpElement);
      }
      AE_HeapStackDelete(NULL, (stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement);
      return;
    }
    lpElement=lpElement->prev;
  }
}

void AE_StackWindowFree(HSTACK *hStack)
{
  AE_HeapStackClear(NULL, (stack **)&hStack->first, (stack **)&hStack->last);
}

AEUNDOITEM* AE_StackUndoItemInsert(AKELEDIT *ae)
{
  AEUNDOITEM *lpElement=NULL;

  if (AE_EditCanRedo(ae))
  {
    AE_StackRedoDeleteAll(ae, ae->lpCurrentUndo);
  }
  AE_HeapStackInsert(ae, (stack **)&ae->hUndoStack.first, (stack **)&ae->hUndoStack.last, (stack **)&lpElement, 1, sizeof(AEUNDOITEM));
  return lpElement;
}

void AE_StackUndoItemDelete(AKELEDIT *ae, AEUNDOITEM *lpItem)
{
  if (lpItem == ae->lpSavePoint)
  {
    ae->lpSavePoint=NULL;
    ae->bSavePointExist=FALSE;
  }
  if (lpItem->wpText) AE_HeapFree(ae, 0, (LPVOID)lpItem->wpText);
  AE_HeapStackDelete(ae, (stack **)&ae->hUndoStack.first, (stack **)&ae->hUndoStack.last, (stack *)lpItem);
}

void AE_StackRedoDeleteAll(AKELEDIT *ae, AEUNDOITEM *lpItem)
{
  AEUNDOITEM *lpElement=lpItem;
  AEUNDOITEM *lpTmp;

  if (!lpElement)
    lpElement=(AEUNDOITEM *)ae->hUndoStack.first;
  else
    lpElement=lpElement->next;

  while (lpElement)
  {
    if (lpElement->dwFlags & AEUN_STOPGROUP)
      --ae->dwUndoCount;

    lpTmp=lpElement->next;
    AE_StackUndoItemDelete(ae, lpElement);
    lpElement=lpTmp;
  }
}

void AE_StackUndoGroupStop(AKELEDIT *ae)
{
  AEUNDOITEM *lpStopElement=(AEUNDOITEM *)ae->hUndoStack.last;

  if (lpStopElement)
  {
    if (!(lpStopElement->dwFlags & AEUN_STOPGROUP))
    {
      if (lpStopElement->dwFlags & AEUN_SINGLECHAR)
      {
        //Merge typing characters to one undo action
        AEUNDOITEM *lpElement=lpStopElement;
        AEUNDOITEM *lpUndoElement;
        AECHARINDEX ciDeleteStart;
        AECHARINDEX ciDeleteEnd;
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
          if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
          {
            //Last index
            if (lpElement)
            {
              ciDeleteEnd=lpElement->ciActionEnd;
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
            ae->lpCurrentUndo=lpElement;

            //First index
            if (!lpElement)
              lpElement=(AEUNDOITEM *)ae->hUndoStack.first;
            else
              lpElement=lpElement->next;
            if (lpElement) ciDeleteStart=lpElement->ciActionStart;

            //Insert complite string
            if (lpUndoElement=AE_StackUndoItemInsert(ae))
            {
              lpUndoElement->dwFlags=AEUN_DELETE;
              lpUndoElement->ciActionStart=ciDeleteStart;
              lpUndoElement->ciActionEnd=ciDeleteEnd;
              lpUndoElement->wpText=wpUndoText;
              lpUndoElement->dwTextLen=dwUndoTextLen;

              ae->lpCurrentUndo=lpUndoElement;
              lpStopElement=lpUndoElement;
            }
          }
        }
      }
      lpStopElement->dwFlags|=AEUN_STOPGROUP;
      ++ae->dwUndoCount;
    }
  }

  //Undo limit is reached
  while (ae->dwUndoCount > ae->dwUndoLimit)
  {
    AEUNDOITEM *lpElement=(AEUNDOITEM *)ae->hUndoStack.first;
    AEUNDOITEM *lpTmp;

    if (!ae->lpSavePoint)
    {
      ae->lpSavePoint=NULL;
      ae->bSavePointExist=FALSE;
    }

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
    --ae->dwUndoCount;
  }
}

AELINEDATA* AE_StackLineAdd(AKELEDIT *ae)
{
  AELINEDATA *lpElement=NULL;

  AE_HeapStackInsert(ae, (stack **)&ae->hLinesStack.first, (stack **)&ae->hLinesStack.last, (stack **)&lpElement, 1, sizeof(AELINEDATA));
  return lpElement;
}

AELINEDATA* AE_StackLineInsertBefore(AKELEDIT *ae, AELINEDATA *lpLine)
{
  AELINEDATA *lpElement=NULL;

  AE_HeapStackInsertBefore(ae, (stack **)&ae->hLinesStack.first, (stack **)&ae->hLinesStack.last, (stack **)&lpElement, (stack *)lpLine, sizeof(AELINEDATA));
  return lpElement;
}

AELINEDATA* AE_StackLineInsertAfter(AKELEDIT *ae, AELINEDATA *lpLine)
{
  AELINEDATA *lpElement=NULL;

  AE_HeapStackInsertAfter(ae, (stack **)&ae->hLinesStack.first, (stack **)&ae->hLinesStack.last, (stack **)&lpElement, (stack *)lpLine, sizeof(AELINEDATA));
  return lpElement;
}

AELINEDATA* AE_StackLineGet(AKELEDIT *ae, int nLine)
{
  AELINEDATA *lpElement=NULL;
  int nFirst;
  int nSecond;
  int nThird;
  int nFourth;
  int nFifth;
  int nElement=0;
  int a;

  if (nLine < 0) nLine=ae->nLineCount + nLine + 1;
  if (nLine < 0 || nLine > ae->nLineCount) return NULL;

  //Find nearest element in stack
  nFirst=mod(nLine - 0);
  nSecond=mod(nLine - ae->nLineCount);
  nThird=mod(nLine - ae->liFirstDrawLine.nLine);
  nFourth=mod(nLine - ae->ciSelStartIndex.nLine);
  nFifth=mod(nLine - ae->ciSelEndIndex.nLine);

  if (nFirst <= nSecond && nFirst <= nThird && nFirst <= nFourth && nFirst <= nFifth)
  {
    lpElement=(AELINEDATA *)ae->hLinesStack.first;
    nElement=nLine - 0;
  }
  else if (nSecond <= nFirst && nSecond <= nThird && nSecond <= nFourth && nSecond <= nFifth)
  {
    lpElement=(AELINEDATA *)ae->hLinesStack.last;
    nElement=nLine - ae->nLineCount;
  }
  else if (nThird <= nFirst && nThird <= nSecond && nThird <= nFourth && nThird <= nFifth)
  {
    lpElement=ae->liFirstDrawLine.lpLine;
    nElement=nLine - ae->liFirstDrawLine.nLine;
  }
  else if (nFourth <= nFirst && nFourth <= nSecond && nFourth <= nThird && nFourth <= nFifth)
  {
    lpElement=ae->ciSelStartIndex.lpLine;
    nElement=nLine - ae->ciSelStartIndex.nLine;
  }
  else if (nFifth <= nFirst && nFifth <= nSecond && nFifth <= nThird && nFifth <= nFourth)
  {
    lpElement=ae->ciSelEndIndex.lpLine;
    nElement=nLine - ae->ciSelEndIndex.nLine;
  }

  if (nElement > 0)
  {
    for (a=nElement; lpElement; --a)
    {
      if (a == 0)
        break;
      lpElement=lpElement->next;
    }
  }
  else if (nElement < 0)
  {
    for (a=nElement; lpElement; ++a)
    {
      if (a == 0)
        break;
      lpElement=lpElement->prev;
    }
  }
  return lpElement;
}

void AE_StackLineDelete(AKELEDIT *ae, AELINEDATA *lpElement)
{
  if (lpElement == ae->liFirstDrawLine.lpLine)
  {
    ae->liFirstDrawLine.nLine=0;
    ae->liFirstDrawLine.lpLine=NULL;
  }
  if (lpElement == ae->liMaxWidthLine.lpLine)
  {
    ae->liMaxWidthLine.nLine=0;
    ae->liMaxWidthLine.lpLine=NULL;
  }
  if (lpElement->wpLine) AE_HeapFree(ae, 0, (LPVOID)lpElement->wpLine);
  AE_HeapStackDelete(ae, (stack **)&ae->hLinesStack.first, (stack **)&ae->hLinesStack.last, (stack *)lpElement);
}

void AE_StackLineFree(AKELEDIT *ae)
{
  AELINEDATA *lpElement=(AELINEDATA *)ae->hLinesStack.last;
  AELINEDATA *lpTmp;

  while (lpElement)
  {
    lpTmp=lpElement->prev;
    AE_StackLineDelete(ae, lpElement);
    lpElement=lpTmp;
  }
}

BOOL AE_GetIndex(AKELEDIT *ae, int nType, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, BOOL bColumnSel)
{
  if (nType == AEGI_FIRSTCHAR)
  {
    ciCharOut->nLine=0;
    ciCharOut->lpLine=(AELINEDATA *)ae->hLinesStack.first;
    ciCharOut->nCharInLine=0;
    return ciCharOut->lpLine?TRUE:FALSE;
  }
  else if (nType == AEGI_LASTCHAR)
  {
    ciCharOut->nLine=ae->nLineCount;
    ciCharOut->lpLine=(AELINEDATA *)ae->hLinesStack.last;
    ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
    return ciCharOut->lpLine?TRUE:FALSE;
  }
  else if (nType == AEGI_FIRSTSELCHAR)
  {
    *ciCharOut=ae->ciSelStartIndex;
    return TRUE;
  }
  else if (nType == AEGI_LASTSELCHAR)
  {
    *ciCharOut=ae->ciSelEndIndex;
    return TRUE;
  }
  else if (nType == AEGI_CARETCHAR)
  {
    *ciCharOut=ae->ciCaretIndex;
    return TRUE;
  }
  else if (nType == AEGI_FIRSTVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetFirstVisibleLine(ae);
    ciCharOut->lpLine=AE_StackLineGet(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=0;
    return ciCharOut->lpLine?TRUE:FALSE;
  }
  else if (nType == AEGI_LASTVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetLastVisibleLine(ae);
    ciCharOut->lpLine=AE_StackLineGet(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
    return ciCharOut->lpLine?TRUE:FALSE;
  }
  else if (nType == AEGI_NEXTLINE)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    ciCharOut->nLine=min(ciCharTmp.nLine + 1, ae->nLineCount);

    if (ciCharTmp.nLine != ciCharOut->nLine)
    {
      ciCharOut->lpLine=ciCharTmp.lpLine->next;
      ciCharOut->nCharInLine=0;
      return TRUE;
    }
    else
    {
      ciCharOut->lpLine=ciCharTmp.lpLine;
      ciCharOut->nCharInLine=ciCharTmp.nCharInLine;
      return FALSE;
    }
  }
  else if (nType == AEGI_PREVLINE)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    ciCharOut->nLine=max(ciCharTmp.nLine - 1, 0);

    if (ciCharTmp.nLine != ciCharOut->nLine)
    {
      ciCharOut->lpLine=ciCharTmp.lpLine->prev;
      ciCharOut->nCharInLine=0;
      return TRUE;
    }
    else
    {
      ciCharOut->lpLine=ciCharTmp.lpLine;
      ciCharOut->nCharInLine=ciCharTmp.nCharInLine;
      return FALSE;
    }
  }
  else if (nType == AEGI_NEXTCHAR)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    if (bColumnSel)
    {
      ciCharOut->nLine=ciCharTmp.nLine;
      ciCharOut->lpLine=ciCharTmp.lpLine;
      ciCharOut->nCharInLine=ciCharTmp.nCharInLine + 1;
      return TRUE;
    }
    else
    {
      if (ciCharTmp.nCharInLine + 1 <= ciCharTmp.lpLine->nLineLen)
      {
        ciCharOut->nLine=ciCharTmp.nLine;
        ciCharOut->lpLine=ciCharTmp.lpLine;
        ciCharOut->nCharInLine=ciCharTmp.nCharInLine + 1;
        return TRUE;
      }
      else
      {
        ciCharOut->nLine=min(ciCharTmp.nLine + 1, ae->nLineCount);

        if (ciCharTmp.nLine != ciCharOut->nLine)
        {
          ciCharOut->lpLine=ciCharTmp.lpLine->next;
          ciCharOut->nCharInLine=0;
          return TRUE;
        }
        else
        {
          *ciCharOut=ciCharTmp;
          return FALSE;
        }
      }
    }
  }
  else if (nType == AEGI_PREVCHAR)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    if (bColumnSel)
    {
      ciCharOut->nLine=ciCharTmp.nLine;
      ciCharOut->lpLine=ciCharTmp.lpLine;
      ciCharOut->nCharInLine=max(ciCharTmp.nCharInLine - 1, 0);
      return TRUE;
    }
    else
    {
      if (ciCharTmp.nCharInLine - 1 >= 0)
      {
        if (ciCharTmp.nCharInLine - 1 > ciCharTmp.lpLine->nLineLen)
        {
          ciCharOut->nLine=ciCharTmp.nLine;
          ciCharOut->lpLine=ciCharTmp.lpLine;
          ciCharOut->nCharInLine=ciCharTmp.lpLine->nLineLen;
        }
        else
        {
          ciCharOut->nLine=ciCharTmp.nLine;
          ciCharOut->lpLine=ciCharTmp.lpLine;
          ciCharOut->nCharInLine=ciCharTmp.nCharInLine - 1;
        }
        return TRUE;
      }
      else
      {
        ciCharOut->nLine=max(ciCharTmp.nLine - 1, 0);

        if (ciCharTmp.nLine != ciCharOut->nLine)
        {
          ciCharOut->lpLine=ciCharTmp.lpLine->prev;
          ciCharOut->nCharInLine=ciCharOut->lpLine->nLineLen;
          return TRUE;
        }
        else
        {
          *ciCharOut=ciCharTmp;
          return FALSE;
        }
      }
    }
  }
  else if (nType == AEGI_NEXTBREAK)
  {
    return AE_GetNextBreak(ae, ciCharIn, ciCharOut, bColumnSel);
  }
  else if (nType == AEGI_PREVBREAK)
  {
    return AE_GetPrevBreak(ae, ciCharIn, ciCharOut, bColumnSel);
  }
  return FALSE;
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

DWORD AE_IndexSubtract(AKELEDIT *ae, const AECHARINDEX *ciChar1, const AECHARINDEX *ciChar2, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  AECHARINDEX ciStart;
  AECHARINDEX ciEnd;
  AECHARINDEX ciTmp;
  AELINEDATA *lpElement;
  DWORD dwCount=0;
  int nLineBreak;

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

  if (!ciChar1)
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciStart, FALSE);
  else
    ciStart=*ciChar1;

  if (!ciChar2)
    AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciEnd, FALSE);
  else
    ciEnd=*ciChar2;

  if (AE_IndexCompare(&ciStart, &ciEnd) > 0)
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
        dwCount+=lpElement->nSelEnd - lpElement->nSelStart;
      else
        dwCount+=min(lpElement->nSelEnd, lpElement->nLineLen) - min(lpElement->nSelStart, lpElement->nLineLen);

      if (lpElement == ciEnd.lpLine) break;

      if (nNewLine == AELB_ASIS)
        nLineBreak=lpElement->nLineBreak;
      else
        nLineBreak=nNewLine;

      if (nLineBreak == AELB_R)
      {
        dwCount+=1;
      }
      else if (nLineBreak == AELB_N)
      {
        dwCount+=1;
      }
      else if (nLineBreak == AELB_RN)
      {
        dwCount+=2;
      }
      else if (nLineBreak == AELB_RRN)
      {
        dwCount+=3;
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
      return ciEnd.nCharInLine - ciStart.nCharInLine;
    }
    else
    {
      lpElement=ciStart.lpLine;

      while (lpElement)
      {
        if (lpElement == ciEnd.lpLine)
        {
          dwCount+=ciEnd.nCharInLine;
          break;
        }

        if (lpElement == ciStart.lpLine)
          dwCount+=ciStart.lpLine->nLineLen - ciStart.nCharInLine;
        else
          dwCount+=lpElement->nLineLen;

        if (nNewLine == AELB_ASIS)
          nLineBreak=lpElement->nLineBreak;
        else
          nLineBreak=nNewLine;

        if (nLineBreak == AELB_R)
        {
          dwCount+=1;
        }
        else if (nLineBreak == AELB_N)
        {
          dwCount+=1;
        }
        else if (nLineBreak == AELB_RN)
        {
          dwCount+=2;
        }
        else if (nLineBreak == AELB_RRN)
        {
          dwCount+=3;
        }
        lpElement=lpElement->next;
      }
    }
  }
  return dwCount;
}

DWORD AE_IndexOffset(AKELEDIT *ae, const AECHARINDEX *ciCharIn, AECHARINDEX *ciCharOut, int nOffset, int nNewLine)
{
  AECHARINDEX ciCount=*ciCharIn;
  int nLineBreak;
  int nOffsetCount=nOffset;

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

  if (!nOffsetCount)
  {
    *ciCharOut=ciCount;
    return TRUE;
  }
  else if (nOffsetCount > 0)
  {
    while (1)
    {
      while (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
      {
        if (nOffsetCount <= 0)
          goto PlusOffEnd;

        --nOffsetCount;
        ++ciCount.nCharInLine;
      }
      ++nOffsetCount;

      if (nNewLine == AELB_ASIS)
        nLineBreak=ciCount.lpLine->nLineBreak;
      else
        nLineBreak=nNewLine;

      if (ciCount.lpLine->next)
      {
        ciCount.nLine+=1;
        ciCount.lpLine=ciCount.lpLine->next;
        ciCount.nCharInLine=0;
      }
      else goto PlusOffEnd;

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
    while (1)
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
      {
        ciCount.nLine-=1;
        ciCount.lpLine=ciCount.lpLine->prev;
        ciCount.nCharInLine=ciCount.lpLine->nLineLen;
      }
      else goto MinusOffEnd;

      if (nNewLine == AELB_ASIS)
        nLineBreak=ciCount.lpLine->nLineBreak;
      else
        nLineBreak=nNewLine;

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

BOOL AE_UpdateIndex(AKELEDIT *ae, AECHARINDEX *ciChar)
{
  if (ciChar->lpLine=AE_StackLineGet(ae, ciChar->nLine))
    return TRUE;
  return FALSE;
}

void AE_WrapLines(AKELEDIT *ae)
{
  AECHARINDEX ciStart;
  AECHARINDEX ciEnd;
  int nMaxWidth=ae->rcDraw.right - ae->rcDraw.left;

  ciStart.lpLine=(AELINEDATA *)ae->hLinesStack.first;
  ciStart.nLine=0;
  ciEnd.lpLine=(AELINEDATA *)ae->hLinesStack.last;
  ciEnd.nLine=ae->nLineCount;

  while (ciStart.lpLine)
  {
    if (ciStart.lpLine->nLineWidth + ae->nAveCharWidth > nMaxWidth)
    {
      AE_GetCharInLine(ae, ciStart.lpLine->wpLine, ciStart.lpLine->nLineLen, nMaxWidth - ae->nAveCharWidth, FALSE, &ciStart.nCharInLine, NULL, FALSE);
      AE_InsertText(ae, &ciStart, L"\r\n", 2, ae->nInputNewLine, FALSE, &ciStart, NULL, TRUE);
    }
    if (ciStart.lpLine == ciEnd.lpLine) break;

    ++ciStart.nLine;

    ciStart.lpLine=ciStart.lpLine->next;
  }
}

void AE_CalcLinesWidth(AKELEDIT *ae, AELINEINDEX *liStartLine, AELINEINDEX *liEndLine, BOOL bFresh)
{
  AELINEINDEX liStart;
  AELINEINDEX liEnd;
  AELINEINDEX liMaxWidthLine;
  int nHScrollMax=0;

  if (!liStartLine)
  {
    liStart.lpLine=(AELINEDATA *)ae->hLinesStack.first;
    liStart.nLine=0;
  }
  else liStart=*liStartLine;

  if (!liEndLine)
  {
    liEnd.lpLine=(AELINEDATA *)ae->hLinesStack.last;
    liEnd.nLine=ae->nLineCount;
  }
  else liEnd=*liEndLine;

  while (liStart.lpLine)
  {
    if (bFresh || liStart.lpLine->nLineWidth == -1)
    {
      AE_GetLineWidth(ae, liStart.lpLine);
    }
    if (liStart.lpLine->nLineWidth + ae->nAveCharWidth > nHScrollMax)
    {
      nHScrollMax=liStart.lpLine->nLineWidth + ae->nAveCharWidth;
      liMaxWidthLine=liStart;
    }
    if (liStart.lpLine == liEnd.lpLine) break;

    ++liStart.nLine;

    liStart.lpLine=liStart.lpLine->next;
  }

  if (!liStartLine && !liEndLine)
  {
    ae->liMaxWidthLine=liMaxWidthLine;

    if (nHScrollMax != ae->nHScrollMax)
    {
      ae->nHScrollMax=nHScrollMax;
      AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }
  else
  {
    if (nHScrollMax > ae->nHScrollMax)
    {
      ae->liMaxWidthLine=liMaxWidthLine;
      ae->nHScrollMax=nHScrollMax;
      AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }
}

int AE_CheckCodepage(AKELEDIT *ae, int nCodePage)
{
  AELINEDATA *lpLine=(AELINEDATA *)ae->hLinesStack.first;
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

void AE_SetDrawRect(AKELEDIT *ae, RECT *rcDraw, BOOL bRedraw)
{
  if (rcDraw)
    AE_memcpy(&ae->rcDraw, rcDraw, sizeof(RECT));
  else
    AE_memcpy(&ae->rcDraw, &ae->rcEdit, sizeof(RECT));

  if (bRedraw) InvalidateRect(ae->hWndEdit, NULL, TRUE);
}

void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bNoRedraw)
{
  TEXTMETRICA tmEdit;
  SIZE sizeAverageWidth;
  HFONT hFontSystem=(HFONT)GetStockObject(SYSTEM_FONT);

  ae->hFont=hFont;

  if (ae->hFont)
  {
    SelectObject(ae->hDC, ae->hFont);
    GetObjectA(ae->hFont, sizeof(LOGFONTA), &ae->lfEditA);
  }
  else
  {
    SelectObject(ae->hDC, hFontSystem);
    GetObjectA((HGDIOBJ)hFontSystem, sizeof(LOGFONTA), &ae->lfEditA);
  }

  ae->lfEditA.lfHeight=-mod(ae->lfEditA.lfHeight);
  ae->lfEditA.lfWidth=0;

  GetTextMetricsA(ae->hDC, &tmEdit);
  ae->nCharHeight=tmEdit.tmHeight;
  GetTextExtentPoint32A(ae->hDC, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeAverageWidth);
  ae->nAveCharWidth=sizeAverageWidth.cx / 52;
  GetTextExtentPoint32A(ae->hDC, " ", 1, &sizeAverageWidth);
  ae->nSpaceCharWidth=sizeAverageWidth.cx;
  ae->bFixedCharWidth=!(tmEdit.tmPitchAndFamily & TMPF_FIXED_PITCH);

  AE_UpdateCaret(ae, TRUE);

  InvalidateRect(ae->hWndEdit, &ae->rcDraw, !bNoRedraw);
}

void AE_SetEditFontW(AKELEDIT *ae, HFONT hFont, BOOL bNoRedraw)
{
  TEXTMETRICW tmEdit;
  SIZE sizeAverageWidth;
  HFONT hFontSystem=(HFONT)GetStockObject(SYSTEM_FONT);

  ae->hFont=hFont;

  if (ae->hFont)
  {
    SelectObject(ae->hDC, ae->hFont);
    GetObjectW(ae->hFont, sizeof(LOGFONTW), &ae->lfEditW);
  }
  else
  {
    SelectObject(ae->hDC, hFontSystem);
    GetObjectW((HGDIOBJ)hFontSystem, sizeof(LOGFONTW), &ae->lfEditW);
  }

  ae->lfEditW.lfHeight=-mod(ae->lfEditW.lfHeight);
  ae->lfEditW.lfWidth=0;

  GetTextMetricsW(ae->hDC, &tmEdit);
  ae->nCharHeight=tmEdit.tmHeight;
  GetTextExtentPoint32W(ae->hDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeAverageWidth);
  ae->nAveCharWidth=sizeAverageWidth.cx / 52;
  GetTextExtentPoint32W(ae->hDC, L" ", 1, &sizeAverageWidth);
  ae->nSpaceCharWidth=sizeAverageWidth.cx;
  ae->bFixedCharWidth=!(tmEdit.tmPitchAndFamily & TMPF_FIXED_PITCH);

  AE_UpdateCaret(ae, TRUE);

  InvalidateRect(ae->hWndEdit, &ae->rcDraw, !bNoRedraw);
}

void AE_SetSelectionPos(AKELEDIT *ae, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, BOOL bColumnSel)
{
  AECHARINDEX ciSelStartOld;
  AECHARINDEX ciSelEndOld;
  AECHARINDEX ciCaretNew;
  AECHARINDEX ciSelStartNew;
  AECHARINDEX ciSelEndNew;
  AECHARINDEX ciTmp;
  AELINEINDEX liLine;
  POINT ptSelStart;
  POINT ptSelEnd;

  if (ciSelStart->lpLine && ciSelEnd->lpLine)
  {
    ae->bColumnSel=bColumnSel;
    ciSelStartOld=ae->ciSelStartIndex;
    ciSelEndOld=ae->ciSelEndIndex;
    ciSelStartNew=*ciSelStart;
    ciSelEndNew=*ciSelEnd;
    ciCaretNew=*ciSelStart;

    //Exchange indexes
    if (AE_IndexCompare(&ciSelStartNew, &ciSelEndNew) > 0)
    {
      ciTmp=ciSelStartNew;
      ciSelStartNew=ciSelEndNew;
      ciSelEndNew=ciTmp;
    }

    //Normalize indexes
    if (bColumnSel)
    {
      AE_GetPosFromChar(ae, &ciSelStartNew, &ptSelStart, NULL, bColumnSel);
      if (ptSelStart.x > ae->nHScrollMax - ae->nAveCharWidth)
        return;

      AE_GetPosFromChar(ae, &ciSelEndNew, &ptSelEnd, NULL, bColumnSel);
      if (ptSelEnd.x > ae->nHScrollMax - ae->nAveCharWidth)
        return;
    }
    else
    {
      ciSelStartNew.nCharInLine=min(ciSelStartNew.nCharInLine, ciSelStartNew.lpLine->nLineLen);
      ciSelEndNew.nCharInLine=min(ciSelEndNew.nCharInLine, ciSelEndNew.lpLine->nLineLen);
    }

    //Clear old lines selection
    if (AE_IndexCompare(&ciSelStartOld, &ciSelEndOld))
    {
      liLine.lpLine=ciSelStartOld.lpLine;

      while (liLine.lpLine)
      {
        liLine.lpLine->nSelStart=0;
        liLine.lpLine->nSelEnd=0;
        if (liLine.lpLine == ciSelEndOld.lpLine) break;

        liLine.lpLine=liLine.lpLine->next;
      }
    }

    //Set new lines selection
    if (AE_IndexCompare(&ciSelStartNew, &ciSelEndNew))
    {
      liLine.nLine=ciSelStartNew.nLine;
      liLine.lpLine=ciSelStartNew.lpLine;

      while (liLine.lpLine)
      {
        AE_GetLineSelection(ae, &liLine, &ciSelStartNew, &ciSelEndNew, &ptSelStart, &ptSelEnd, &liLine.lpLine->nSelStart, &liLine.lpLine->nSelEnd, bColumnSel);
        if (liLine.lpLine == ciSelEndNew.lpLine) break;
        ++liLine.nLine;

        liLine.lpLine=liLine.lpLine->next;
      }
    }

    //Redraw lines
    if (ciSelStartOld.nLine > ciSelEndNew.nLine ||
        ciSelEndOld.nLine < ciSelStartNew.nLine)
    {
      AE_RedrawLineRange(ae, ciSelStartOld.nLine, ciSelEndOld.nLine, FALSE);
      AE_RedrawLineRange(ae, ciSelStartNew.nLine, ciSelEndNew.nLine, FALSE);
    }
    else
    {
      if (bColumnSel)
      {
        if (ciSelStartOld.nCharInLine != ciSelStartNew.nCharInLine ||
            ciSelEndOld.nCharInLine != ciSelEndNew.nCharInLine)
        {
          AE_RedrawLineRange(ae, min(ciSelStartOld.nLine, ciSelStartNew.nLine), max(ciSelEndOld.nLine, ciSelEndNew.nLine), FALSE);
        }
        else
        {
          if (AE_IndexCompare(&ciSelStartOld, &ciSelStartNew))
            AE_RedrawLineRange(ae, min(ciSelStartOld.nLine, ciSelStartNew.nLine), max(ciSelStartOld.nLine, ciSelStartNew.nLine), FALSE);
          if (AE_IndexCompare(&ciSelEndOld, &ciSelEndNew))
            AE_RedrawLineRange(ae, min(ciSelEndOld.nLine, ciSelEndNew.nLine), max(ciSelEndOld.nLine, ciSelEndNew.nLine), FALSE);
        }
      }
      else
      {
        if (AE_IndexCompare(&ciSelStartOld, &ciSelStartNew))
          AE_RedrawLineRange(ae, min(ciSelStartOld.nLine, ciSelStartNew.nLine), max(ciSelStartOld.nLine, ciSelStartNew.nLine), FALSE);
        if (AE_IndexCompare(&ciSelEndOld, &ciSelEndNew))
          AE_RedrawLineRange(ae, min(ciSelEndOld.nLine, ciSelEndNew.nLine), max(ciSelEndOld.nLine, ciSelEndNew.nLine), FALSE);
      }
    }

    ae->ciSelStartIndex=ciSelStartNew;
    ae->ciSelEndIndex=ciSelEndNew;
    ae->ciCaretIndex=ciCaretNew;
    AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL, bColumnSel);
    AE_ScrollToCaret(ae, &ae->ptCaret);
    AE_SetCaretPos(ae);
    if (!ae->bCaretVisible)
    {
      ShowCaret(ae->hWndEdit);
      ae->bCaretVisible=TRUE;
    }

    //Send AEN_SELCHANGE
    {
      AENSELCHANGE sc;

      sc.hdr.hwndFrom=ae->hWndEdit;
      sc.hdr.idFrom=ae->nEditCtrlID;
      sc.hdr.code=AEN_SELCHANGE;
      sc.crSel.ciMin=ae->ciSelStartIndex;
      sc.crSel.ciMax=ae->ciSelEndIndex;
      sc.ciCaret=ae->ciCaretIndex;
      SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
    }
  }
}

void AE_UpdateSelection(AKELEDIT *ae)
{
  if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciCaretIndex))
    AE_SetSelectionPos(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel);
  else
    AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ae->ciSelStartIndex, ae->bColumnSel);
}

void AE_SetMouseSelection(AKELEDIT *ae, POINT *ptPos, BOOL bShift, BOOL bColumnSel)
{
  AECHARINDEX ciCharIndex;

  if (ae->rcDraw.bottom - ae->rcDraw.top > 0 && ae->rcDraw.right - ae->rcDraw.left > 0)
  {
    AE_GetCharFromPos(ae, ptPos, FALSE, &ciCharIndex, &ae->ptCaret, bColumnSel);
    ae->nHorizCaretPos=ae->ptCaret.x;

    //Set selection
    {
      AECHARINDEX ciSelEnd={0};

      if (bShift)
      {
        if (!AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelStartIndex))
        {
          ciSelEnd=ae->ciSelEndIndex;
        }
        else if (!AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex))
        {
          ciSelEnd=ae->ciSelStartIndex;
        }
        AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, ae->bColumnSel);
      }
      else AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, ae->bColumnSel);

      AE_StackUndoGroupStop(ae);
    }
  }
}

BOOL AE_IsCursorOnSelection(AKELEDIT *ae, POINT *ptPos)
{
  AECHARINDEX ciCharIndex;
  POINT ptChar;

  if (AE_GetCharFromPos(ae, ptPos, FALSE, &ciCharIndex, &ptChar, ae->bColumnSel))
  {
    if (ciCharIndex.lpLine->nSelStart != ciCharIndex.lpLine->nSelEnd &&
        ciCharIndex.nCharInLine >= ciCharIndex.lpLine->nSelStart &&
        ciCharIndex.nCharInLine <= ciCharIndex.lpLine->nSelEnd)
    {
      return TRUE;
    }
  }
  return FALSE;
}

HBITMAP AE_CreateCaretBitmap(AKELEDIT *ae, COLORREF crCaret, int nCaretWidth, int nCaretHeight)
{
  BITMAPFILEHEADER *lpBmpFileHeader;
  BITMAPINFOHEADER *lpBmpInfoHeader;
  BYTE *lpBitmapBits;
  BYTE *lpBmpFileData;
  HBITMAP hCaretBitmap=NULL;
  int nBmpFileData;
  DWORD a;
  int b;

  //Bitmap data size
  nBmpFileData=sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  if ((nCaretWidth * 3) % 4 == 0)
    nBmpFileData+=(nCaretWidth * 3) * nCaretHeight;
  else
    nBmpFileData+=((nCaretWidth * 3) + (4 - ((nCaretWidth * 3) % 4))) * nCaretHeight;

  //Allocate bitmap data
  if (lpBmpFileData=(BYTE *)AE_HeapAlloc(ae, 0, nBmpFileData))
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
    lpBmpInfoHeader->biWidth=nCaretWidth;
    lpBmpInfoHeader->biHeight=nCaretHeight;
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
      for (b=0; b < lpBmpInfoHeader->biWidth * 3; b+=3)
      {
        lpBitmapBits[a + 0]=GetBValue(crCaret);
        lpBitmapBits[a + 1]=GetGValue(crCaret);
        lpBitmapBits[a + 2]=GetRValue(crCaret);
        a+=3;
      }
      while (a % 4) lpBitmapBits[a++]=0x00;
    }

    hCaretBitmap=AE_LoadBitmapFromMemory(ae->hDC, (unsigned char *)lpBmpFileData);

    AE_HeapFree(ae, 0, (LPVOID)lpBmpFileData);
  }
  return hCaretBitmap;
}

HBITMAP AE_LoadBitmapFromMemory(HDC hDC, BYTE *lpBmpFileData)
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

  if (hBitmap=CreateDIBSection(hDC, &bi, DIB_RGB_COLORS, (void **)&lpSectionBits, NULL, 0))
  {
    for (a=0; a < bi.bmiHeader.biSizeImage;)
    {
      for (b=0; b < bi.bmiHeader.biWidth * 3; b+=3)
      {
        //Copy inverted bits
        lpSectionBits[a + 0]=255 - lpBitmapBits[a + 0];
        lpSectionBits[a + 1]=255 - lpBitmapBits[a + 1];
        lpSectionBits[a + 2]=255 - lpBitmapBits[a + 2];
        a+=3;
      }
      while (a % 4) lpSectionBits[a++]=0x00;
    }
  }
  return hBitmap;
}

BOOL AE_UpdateCaret(AKELEDIT *ae, BOOL bFresh)
{
  HBITMAP hCaretBitmap=NULL;
  int nCaretWidth;
  int nCaretHeight;

  if (ae->bFocus)
  {
    DestroyCaret();
  }

  if (bFresh)
  {
    if (ae->hCaretInsert)
    {
      DeleteObject(ae->hCaretInsert);
      ae->hCaretInsert=NULL;
    }
    if (ae->hCaretOvertype)
    {
      DeleteObject(ae->hCaretOvertype);
      ae->hCaretOvertype=NULL;
    }
  }

  if (!ae->bOverType)
  {
    hCaretBitmap=ae->hCaretInsert;
    nCaretWidth=ae->nCaretInsertWidth;
    nCaretHeight=ae->nCharHeight;

    if (!hCaretBitmap)
    {
      if (ae->crCaret)
      {
        if (hCaretBitmap=AE_CreateCaretBitmap(ae, ae->crCaret, nCaretWidth, nCaretHeight))
        {
          ae->hCaretInsert=hCaretBitmap;
        }
      }
    }
  }
  else
  {
    hCaretBitmap=ae->hCaretOvertype;
    nCaretWidth=ae->nAveCharWidth;
    nCaretHeight=ae->nCaretOvertypeHeight;

    if (!hCaretBitmap)
    {
      if (ae->crCaret)
      {
        if (hCaretBitmap=AE_CreateCaretBitmap(ae, ae->crCaret, nCaretWidth, nCaretHeight))
        {
          ae->hCaretOvertype=hCaretBitmap;
        }
      }
    }
  }

  if (ae->bFocus)
  {
    if (CreateCaret(ae->hWndEdit, (HBITMAP)hCaretBitmap, nCaretWidth, nCaretHeight))
    {
      AE_SetCaretPos(ae);
      if (ae->bCaretVisible) ShowCaret(ae->hWndEdit);
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_SetCaretPos(AKELEDIT *ae)
{
  if (ae->bOverType)
    return SetCaretPos((ae->ptCaret.x - ae->nHScrollPos) + ae->rcDraw.left, (ae->ptCaret.y - ae->nVScrollPos) + ae->rcDraw.top + max(ae->nCharHeight - ae->nCaretOvertypeHeight, 0));
  else
    return SetCaretPos((ae->ptCaret.x - ae->nHScrollPos) + ae->rcDraw.left, (ae->ptCaret.y - ae->nVScrollPos) + ae->rcDraw.top);
}

void AE_ScrollToCaret(AKELEDIT *ae, POINT *ptCaret)
{
  if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth)
  {
    if (ae->dwMouseMoveTimer)
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + 1, 0));
    else
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + (ae->rcDraw.right - ae->rcDraw.left) / 3, 0));
  }
  else if (ptCaret->x < ae->nHScrollPos)
  {
    if (ae->dwMouseMoveTimer)
      AE_ScrollEditWindow(ae, SB_HORZ, ptCaret->x);
    else
      AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) / 3, 0));
  }

  if (ptCaret->y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - ae->nCharHeight)
  {
    AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->nCharHeight + 1, 0));
  }
  else if (ptCaret->y < ae->nVScrollPos)
  {
    AE_ScrollEditWindow(ae, SB_VERT, ptCaret->y);
  }
}

void AE_ScrollEditWindow(AKELEDIT *ae, int nBar, int nPos)
{
  SCROLLINFO si;
  POINT pt;

  si.cbSize=sizeof(SCROLLINFO);
  si.fMask=SIF_POS|SIF_DISABLENOSCROLL;
  si.nPos=nPos;
  SetScrollInfo(ae->hWndEdit, nBar, &si, TRUE);

  si.fMask=SIF_POS;
  GetScrollInfo(ae->hWndEdit, nBar, &si);

  if (nBar == SB_VERT)
  {
    if (si.nPos != ae->nVScrollPos)
    {
      ScrollWindow(ae->hWndEdit, 0, ae->nVScrollPos - si.nPos, NULL, &ae->rcDraw);
      ae->nVScrollPos=si.nPos;
      UpdateWindow(ae->hWndEdit);
    }
  }
  else if (nBar == SB_HORZ)
  {
    if (si.nPos != ae->nHScrollPos)
    {
      ScrollWindow(ae->hWndEdit, ae->nHScrollPos - si.nPos, 0, NULL, &ae->rcDraw);
      ae->nHScrollPos=si.nPos;
      UpdateWindow(ae->hWndEdit);

      //Change caret visibility
      GetCaretPos(&pt);

      if (pt.x < ae->rcDraw.left || pt.x > ae->rcDraw.right)
      {
        if (ae->bCaretVisible)
        {
          HideCaret(ae->hWndEdit);
          ae->bCaretVisible=FALSE;
        }
      }
      else
      {
        if (!ae->bCaretVisible)
        {
          ShowCaret(ae->hWndEdit);
          ae->bCaretVisible=TRUE;
        }
      }
    }
  }
}

void AE_UpdateScrollBars(AKELEDIT *ae, int nBar)
{
  SCROLLINFO si;

  if (nBar == SB_BOTH || nBar == SB_VERT)
  {
    if (ae->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->dwOptions & AECO_HIDENOSCROLL?0:SIF_DISABLENOSCROLL);
      si.nMin=0;
      si.nMax=ae->nVScrollMax;
      si.nPage=ae->rcDraw.bottom - ae->rcDraw.top;
      si.nPos=ae->nVScrollPos;
      SetScrollInfo(ae->hWndEdit, SB_VERT, &si, TRUE);

      si.fMask=SIF_POS;
      GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
      ae->nVScrollPos=si.nPos;
    }
    else
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_RANGE|(ae->dwOptions & AECO_HIDENOSCROLL?0:SIF_DISABLENOSCROLL);
      si.nMin=0;
      si.nMax=0;
      SetScrollInfo(ae->hWndEdit, SB_VERT, &si, TRUE);

      ae->nVScrollPos=0;
    }
  }

  if (nBar == SB_BOTH || nBar == SB_HORZ)
  {
    if (ae->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->dwOptions & AECO_HIDENOSCROLL?0:SIF_DISABLENOSCROLL);
      si.nMin=0;
      si.nMax=ae->nHScrollMax;
      si.nPage=ae->rcDraw.right - ae->rcDraw.left;
      si.nPos=ae->nHScrollPos;
      SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, TRUE);

      si.fMask=SIF_POS;
      GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);
      ae->nHScrollPos=si.nPos;
    }
    else
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_RANGE|(ae->dwOptions & AECO_HIDENOSCROLL?0:SIF_DISABLENOSCROLL);
      si.nMin=0;
      si.nMax=0;
      SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, TRUE);

      ae->nHScrollPos=0;
    }
  }
  AE_SetCaretPos(ae);
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
    rcRedraw.top=(nFirstLine * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;
  if (nLastLine == -1)
    rcRedraw.bottom=ae->rcDraw.bottom;
  else
    rcRedraw.bottom=((nLastLine + 1) * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;

  rcRedraw.top=max(rcRedraw.top, ae->rcDraw.top);
  rcRedraw.bottom=min(rcRedraw.bottom, ae->rcDraw.bottom);

  InvalidateRect(ae->hWndEdit, &rcRedraw, bErase);
}

int AE_GetFirstVisibleLine(AKELEDIT *ae)
{
  int nFirstLine;

  nFirstLine=ae->nVScrollPos / ae->nCharHeight;

  return max(nFirstLine, 0);
}

int AE_GetLastVisibleLine(AKELEDIT *ae)
{
  int nVScrollLastLine;
  int nLastLine;

  nVScrollLastLine=ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top);
  nLastLine=nVScrollLastLine / ae->nCharHeight;

  return min(nLastLine, ae->nLineCount);
}

BOOL AE_GetTextExtentPoint32(AKELEDIT *ae, wchar_t *wpString, int nStringLen, SIZE *lpSize)
{
  if (ae->bFixedCharWidth)
  {
    lpSize->cx=ae->nAveCharWidth * nStringLen;
    lpSize->cy=ae->nCharHeight;
    return TRUE;
  }
  else
  {
    return GetTextExtentPoint32W(ae->hDC, wpString, nStringLen, lpSize);
  }
}

BOOL AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine)
{
  AECHARINDEX ciCharIndex;
  POINT ptLineWidth;

  ciCharIndex.nLine=0;
  ciCharIndex.nCharInLine=lpLine->nLineLen;
  ciCharIndex.lpLine=lpLine;

  if (AE_GetPosFromChar(ae, &ciCharIndex, &ptLineWidth, NULL, FALSE))
  {
    lpLine->nLineWidth=ptLineWidth.x;
    return TRUE;
  }
  return FALSE;
}

BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos, BOOL bColumnSel)
{
  SIZE sizeString;
  wchar_t *wpStringCount;
  int nStringChars;
  int nStringLen;
  int nStringWidth=0;
  int nMaxCharsCount=0;
  int nTabWidth;
  int nLastTabIndexInLine=-1;
  int i;

  if (ciCharIndex->lpLine)
  {
    nStringChars=min(ciCharIndex->nCharInLine, ciCharIndex->lpLine->nLineLen);
    wpStringCount=ciCharIndex->lpLine->wpLine;

    for (i=0; i < nStringChars; ++i)
    {
      if (ciCharIndex->lpLine->wpLine[i] == L'\t')
      {
        nStringLen=i - (wpStringCount - ciCharIndex->lpLine->wpLine);

        if (nStringLen)
        {
          if (AE_GetTextExtentPoint32(ae, wpStringCount, nStringLen, &sizeString))
          {
            nStringWidth+=sizeString.cx;
            wpStringCount+=nStringLen;
          }
          else return FALSE;
        }
        nMaxCharsCount=0;

        if (nLastTabIndexInLine == -1 || i - nLastTabIndexInLine > ae->nTabStop)
          nTabWidth=ae->nAveCharWidth * (ae->nTabStop - i % ae->nTabStop);
        else
          nTabWidth=ae->nAveCharWidth * (ae->nTabStop - (i - nLastTabIndexInLine - 1));

        nStringWidth+=nTabWidth;
        wpStringCount+=1;
        nLastTabIndexInLine=i;
      }
      if (nMaxCharsCount == 2048)
      {
        nStringLen=i - (wpStringCount - ciCharIndex->lpLine->wpLine);

        if (nStringLen)
        {
          if (AE_GetTextExtentPoint32(ae, wpStringCount, nStringLen, &sizeString))
          {
            nStringWidth+=sizeString.cx;
            wpStringCount+=nStringLen;
          }
          else return FALSE;
        }
        nMaxCharsCount=0;
      }
      ++nMaxCharsCount;
    }
    nStringLen=i - (wpStringCount - ciCharIndex->lpLine->wpLine);

    if (nStringLen)
    {
      if (AE_GetTextExtentPoint32(ae, wpStringCount, nStringLen, &sizeString))
      {
        nStringWidth+=sizeString.cx;
        wpStringCount+=nStringLen;
      }
      else return FALSE;
    }

    if (bColumnSel)
    {
      if (ciCharIndex->nCharInLine > ciCharIndex->lpLine->nLineLen)
      {
        nStringWidth+=(ciCharIndex->nCharInLine - ciCharIndex->lpLine->nLineLen) * ae->nSpaceCharWidth;
      }
    }

    if (ptGlobalPos)
    {
      ptGlobalPos->x=nStringWidth;
      ptGlobalPos->y=ciCharIndex->nLine * ae->nCharHeight;
    }
    if (ptClientPos)
    {
      ptClientPos->x=(nStringWidth - ae->nHScrollPos) + ae->rcDraw.left;
      ptClientPos->y=(ciCharIndex->nLine * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_GetCharInLine(AKELEDIT *ae, const wchar_t *wpString, int nStringChars, int nMaxExtent, BOOL bHalfFit, int *nCharIndex, int *nCharPos, BOOL bColumnSel)
{
  SIZE sizeChar={0};
  int nStringWidth=0;
  int nTabWidth;
  int nLastTabIndexInLine=-1;
  int i;

  for (i=0; i < nStringChars && nStringWidth < nMaxExtent; ++i)
  {
    if (wpString[i] == L'\t')
    {
      if (nLastTabIndexInLine == -1 || i - nLastTabIndexInLine > ae->nTabStop)
        nTabWidth=ae->nAveCharWidth * (ae->nTabStop - i % ae->nTabStop);
      else
        nTabWidth=ae->nAveCharWidth * (ae->nTabStop - (i - nLastTabIndexInLine - 1));

      sizeChar.cx=nTabWidth;
      nStringWidth+=sizeChar.cx;
      nLastTabIndexInLine=i;
    }
    else
    {
      if (AE_GetTextExtentPoint32(ae, (wchar_t *)&wpString[i], 1, &sizeChar))
      {
        nStringWidth+=sizeChar.cx;
      }
      else return FALSE;
    }
  }

  if (bColumnSel)
  {
    if (nStringWidth < nMaxExtent)
    {
      sizeChar.cx=ae->nSpaceCharWidth;
      i+=(nMaxExtent - nStringWidth) / sizeChar.cx;
      nStringWidth+=(nMaxExtent - nStringWidth) / sizeChar.cx * sizeChar.cx;
    }
  }
  if (i)
  {
    if (nStringWidth > nMaxExtent)
    {
      if (bHalfFit)
      {
        if (nStringWidth - nMaxExtent > sizeChar.cx / 2)
        {
          nStringWidth-=sizeChar.cx;
          --i;
        }
      }
      else
      {
        nStringWidth-=sizeChar.cx;
        --i;
      }
    }
  }
  if (nCharPos) *nCharPos=nStringWidth;
  if (nCharIndex) *nCharIndex=i;
  return TRUE;
}

BOOL AE_GetCharRangeInLine(AKELEDIT *ae, const wchar_t *wpString, int nStringChars, int nMinExtent, int nMaxExtent, int *nMinCharIndex, int *nMinCharPos, int *nMaxCharIndex, int *nMaxCharPos, BOOL bColumnSel)
{
  SIZE sizeChar={0};
  int nStringWidth=0;
  int nTabWidth;
  int nLastTabIndexInLine=-1;
  int i=0;

  if (nMinCharPos || nMinCharIndex)
  {
    for (; i < nStringChars && nStringWidth < nMinExtent; ++i)
    {
      if (wpString[i] == L'\t')
      {
        if (nLastTabIndexInLine == -1 || i - nLastTabIndexInLine > ae->nTabStop)
          nTabWidth=ae->nAveCharWidth * (ae->nTabStop - i % ae->nTabStop);
        else
          nTabWidth=ae->nAveCharWidth * (ae->nTabStop - (i - nLastTabIndexInLine - 1));

        sizeChar.cx=nTabWidth;
        nStringWidth+=sizeChar.cx;
        nLastTabIndexInLine=i;
      }
      else
      {
        if (AE_GetTextExtentPoint32(ae, (wchar_t *)&wpString[i], 1, &sizeChar))
        {
          nStringWidth+=sizeChar.cx;
        }
        else return FALSE;
      }
    }
    if (bColumnSel)
    {
      if (nStringWidth < nMinExtent)
      {
        sizeChar.cx=ae->nSpaceCharWidth;
        i+=(nMinExtent - nStringWidth) / sizeChar.cx;
        nStringWidth+=(nMinExtent - nStringWidth) / sizeChar.cx * sizeChar.cx;
      }
    }
    if (nMinCharPos) *nMinCharPos=nStringWidth;
    if (nMinCharIndex) *nMinCharIndex=i;
  }

  if (nMaxCharPos || nMaxCharIndex)
  {
    for (; i < nStringChars && nStringWidth < nMaxExtent; ++i)
    {
      if (wpString[i] == L'\t')
      {
        if (nLastTabIndexInLine == -1 || i - nLastTabIndexInLine > ae->nTabStop)
          nTabWidth=ae->nAveCharWidth * (ae->nTabStop - i % ae->nTabStop);
        else
          nTabWidth=ae->nAveCharWidth * (ae->nTabStop - (i - nLastTabIndexInLine - 1));

        sizeChar.cx=nTabWidth;
        nStringWidth+=sizeChar.cx;
        nLastTabIndexInLine=i;
      }
      else
      {
        if (AE_GetTextExtentPoint32(ae, (wchar_t *)&wpString[i], 1, &sizeChar))
        {
          nStringWidth+=sizeChar.cx;
        }
        else return FALSE;
      }
    }
    if (bColumnSel)
    {
      if (nStringWidth < nMaxExtent)
      {
        sizeChar.cx=ae->nSpaceCharWidth;
        i+=(nMaxExtent - nStringWidth) / sizeChar.cx;
        nStringWidth+=(nMaxExtent - nStringWidth) / sizeChar.cx * sizeChar.cx;
      }
    }
    if (i)
    {
      if (nStringWidth > nMaxExtent && nStringWidth - sizeChar.cx >= nMinExtent)
      {
        if (nStringWidth - nMaxExtent > sizeChar.cx / 2)
        {
          nStringWidth-=sizeChar.cx;
          --i;
        }
      }
    }
    if (nMaxCharPos) *nMaxCharPos=nStringWidth;
    if (nMaxCharIndex) *nMaxCharIndex=i;
  }
  return TRUE;
}

BOOL AE_GetCharFromPos(AKELEDIT *ae, POINT *ptClientPos, BOOL bOnlyVisible, AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, BOOL bColumnSel)
{
  int nCharPos;
  int nMaxExtent;
  int nFirstLine;
  int nLastLine;

  if (bOnlyVisible)
  {
    nFirstLine=AE_GetFirstVisibleLine(ae);
    nLastLine=AE_GetLastVisibleLine(ae);
  }
  else
  {
    nFirstLine=0;
    nLastLine=ae->nLineCount;
  }
  ciCharIndex->nLine=(ae->nVScrollPos + (ptClientPos->y - ae->rcDraw.top)) / ae->nCharHeight;
  ciCharIndex->nLine=max(ciCharIndex->nLine, nFirstLine);
  ciCharIndex->nLine=min(ciCharIndex->nLine, nLastLine);
  ciCharIndex->nCharInLine=0;
  if (ptGlobalPos) ptGlobalPos->y=ciCharIndex->nLine * ae->nCharHeight;
  nMaxExtent=max(ae->nHScrollPos + (ptClientPos->x - ae->rcDraw.left), 0);
  nMaxExtent=min(ae->nHScrollMax - ae->nAveCharWidth, nMaxExtent);

  if (AE_UpdateIndex(ae, ciCharIndex))
  {
    if (AE_GetCharInLine(ae, ciCharIndex->lpLine->wpLine, ciCharIndex->lpLine->nLineLen, nMaxExtent, TRUE, &ciCharIndex->nCharInLine, &nCharPos, bColumnSel))
    {
      if (ptGlobalPos) ptGlobalPos->x=nCharPos;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_GetNextBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciNextBreak, BOOL bColumnSel)
{
  AECHARINDEX ciCount=*ciChar;
  BOOL bInList;

  if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
    bInList=AE_IsInDelimiterList(ae, L'\n');
  else
    bInList=AE_IsInDelimiterList(ae, ciCount.lpLine->wpLine[ciCount.nCharInLine]);

  if (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
  {
    while (1)
    {
      while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
      {
        if (bInList != AE_IsInDelimiterList(ae, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
          goto End;

        ++ciCount.nCharInLine;
      }
      if (bColumnSel) goto End;
      if (bInList != AE_IsInDelimiterList(ae, L'\n')) goto End;

      if (ciCount.lpLine->next)
      {
        ciCount.nLine+=1;
        ciCount.lpLine=ciCount.lpLine->next;
        ciCount.nCharInLine=0;
      }
      else goto End;
    }

    End:
    ciCount.nCharInLine=min(ciCount.nCharInLine, ciCount.lpLine->nLineLen);

    if (AE_IndexCompare(ciChar, &ciCount))
    {
      *ciNextBreak=ciCount;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel)
{
  AECHARINDEX ciCount=*ciChar;
  BOOL bInList;

  if (--ciCount.nCharInLine < 0)
    bInList=AE_IsInDelimiterList(ae, L'\n');
  else
    bInList=AE_IsInDelimiterList(ae, ciCount.lpLine->wpLine[ciCount.nCharInLine]);

  if (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
  {
    while (1)
    {
      while (ciCount.nCharInLine >= 0)
      {
        if (bInList != AE_IsInDelimiterList(ae, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
        {
          ++ciCount.nCharInLine;
          goto End;
        }
        --ciCount.nCharInLine;
      }
      if (bColumnSel) goto End;
      if (bInList != AE_IsInDelimiterList(ae, L'\n')) goto End;

      if (ciCount.lpLine->prev)
      {
        ciCount.nLine-=1;
        ciCount.lpLine=ciCount.lpLine->prev;
        ciCount.nCharInLine=ciCount.lpLine->nLineLen - 1;
      }
      else goto End;
    }

    End:
    ciCount.nCharInLine=max(ciCount.nCharInLine, 0);

    if (AE_IndexCompare(ciChar, &ciCount))
    {
      *ciPrevBreak=ciCount;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_GetNextWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel)
{
  AECHARRANGE cr;
  AECHARINDEX ciCount=*ciChar;

  if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
  {
    if (AE_IsInDelimiterList(ae, L'\n'))
      if (!AE_GetNextBreak(ae, &ciCount, &ciCount, bColumnSel))
        return FALSE;
  }
  else
  {
    if (AE_IsInDelimiterList(ae, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
      if (!AE_GetNextBreak(ae, &ciCount, &ciCount, bColumnSel))
        return FALSE;
  }

  cr.ciMin=ciCount;

  if (AE_GetNextBreak(ae, &ciCount, &cr.ciMax, bColumnSel))
  {
    if (ciWordStart) *ciWordStart=cr.ciMin;
    if (ciWordEnd) *ciWordEnd=cr.ciMax;
    return TRUE;
  }
  return FALSE;
}

BOOL AE_GetPrevWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel)
{
  AECHARRANGE cr;
  AECHARINDEX ciCount=*ciChar;

  if (ciCount.nCharInLine - 1 < 0)
  {
    if (AE_IsInDelimiterList(ae, L'\n'))
      if (!AE_GetPrevBreak(ae, &ciCount, &ciCount, bColumnSel))
        return FALSE;
  }
  else
  {
    if (AE_IsInDelimiterList(ae, ciCount.lpLine->wpLine[ciCount.nCharInLine - 1]))
      if (!AE_GetPrevBreak(ae, &ciCount, &ciCount, bColumnSel))
        return FALSE;
  }

  cr.ciMax=ciCount;

  if (AE_GetPrevBreak(ae, &ciCount, &cr.ciMin, bColumnSel))
  {
    if (ciWordStart) *ciWordStart=cr.ciMin;
    if (ciWordEnd) *ciWordEnd=cr.ciMax;
    return TRUE;
  }
  return FALSE;
}

BOOL AE_IsInDelimiterList(AKELEDIT *ae, wchar_t c)
{
  if (AE_wcschr(ae->wszWordDelimiters, c) != NULL)
    return TRUE;
  else
    return FALSE;
}

int AE_GetLineSelection(AKELEDIT *ae, const AELINEINDEX *liLine, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, POINT *ptSelStart, POINT *ptSelEnd, int *nSelStartIndexInLine, int *nSelEndIndexInLine, BOOL bColumnSel)
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

    AE_GetCharRangeInLine(ae, liLine->lpLine->wpLine, liLine->lpLine->nLineLen, nStartX, nEndX, &nSelStart, NULL, &nSelEnd, NULL, bColumnSel);

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
    if (ciSelStart->nLine < liLine->nLine && ciSelEnd->nLine > liLine->nLine)
      goto Full;
    if (ciSelStart->nLine == liLine->nLine && ciSelStart->nCharInLine == 0 &&
        ciSelEnd->nLine == liLine->nLine && ciSelEnd->nCharInLine == ciSelEnd->lpLine->nLineLen)
      goto Full;
    if (ciSelStart->nLine == liLine->nLine && ciSelStart->nCharInLine == 0 &&
        (ciSelEnd->nLine > liLine->nLine))
      goto Full;
    if (ciSelEnd->nLine == liLine->nLine && ciSelEnd->nCharInLine == ciSelEnd->lpLine->nLineLen &&
        ciSelStart->nLine < liLine->nLine)
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

DWORD AE_GetTextRangeAnsi(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, char *szBuffer, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  wchar_t *wszText;
  DWORD dwTextLen;
  DWORD dwAnsiBytes;
  DWORD dwUnicodeBytes;

  if (dwUnicodeBytes=AE_IndexSubtract(ae, ciRangeStart, ciRangeEnd, nNewLine, bColumnSel, bFillSpaces) * sizeof(wchar_t) + 2)
  {
    if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
    {
      dwTextLen=AE_GetTextRange(ae, ciRangeStart, ciRangeEnd, wszText, nNewLine, bColumnSel, bFillSpaces);

      dwAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wszText, dwTextLen + 1, NULL, 0, NULL, NULL);

      if (szBuffer)
      {
        WideCharToMultiByte(CP_ACP, 0, wszText, dwTextLen + 1, szBuffer, dwAnsiBytes, NULL, NULL);
      }
      AE_HeapFree(ae, 0, (LPVOID)wszText);
      return dwAnsiBytes;
    }
  }
  return 0;
}

DWORD AE_GetTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, wchar_t *wszBuffer, int nNewLine, BOOL bColumnSel, BOOL bFillSpaces)
{
  AECHARINDEX ciStart=*ciRangeStart;
  AECHARINDEX ciEnd=*ciRangeEnd;
  AELINEDATA *lpElement;
  wchar_t *wszBufferCount=&wszBuffer[0];
  int nLineBreak;
  int nCharsToCopy;
  int i;

  if (ciStart.lpLine && ciEnd.lpLine)
  {
    if (wszBuffer)
    {
      if (AE_IndexCompare(&ciStart, &ciEnd))
      {
        //Set new line
        if (nNewLine == AELB_ASINPUT)
          nNewLine=ae->nInputNewLine;
        else if (nNewLine == AELB_ASOUTPUT)
          nNewLine=ae->nOutputNewLine;

        if (bColumnSel)
        {
          lpElement=ciStart.lpLine;

          while (lpElement)
          {
            for (i=lpElement->nSelStart; i < lpElement->nLineLen && i < lpElement->nSelEnd; ++i)
            {
              *wszBufferCount++=lpElement->wpLine[i];
            }
            if (bFillSpaces)
            {
              while (i < lpElement->nSelEnd)
              {
                *wszBufferCount++=' ';
                ++i;
              }
            }
            if (lpElement == ciEnd.lpLine) break;

            if (nNewLine == AELB_ASIS)
              nLineBreak=lpElement->nLineBreak;
            else
              nLineBreak=nNewLine;

            if (nLineBreak == AELB_R)
            {
              *wszBufferCount++=L'\r';
            }
            else if (nLineBreak == AELB_N)
            {
              *wszBufferCount++=L'\n';
            }
            else if (nLineBreak == AELB_RN)
            {
              *wszBufferCount++=L'\r';
              *wszBufferCount++=L'\n';
            }
            else if (nLineBreak == AELB_RRN)
            {
              *wszBufferCount++=L'\r';
              *wszBufferCount++=L'\r';
              *wszBufferCount++=L'\n';
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
            nCharsToCopy=ciEnd.nCharInLine - ciStart.nCharInLine;
            AE_memcpy(wszBufferCount, ciStart.lpLine->wpLine + ciStart.nCharInLine, nCharsToCopy * sizeof(wchar_t));
            wszBufferCount+=nCharsToCopy;
          }
          else
          {
            lpElement=ciStart.lpLine;

            while (lpElement)
            {
              if (lpElement == ciEnd.lpLine)
              {
                //Last line
                AE_memcpy(wszBufferCount, ciEnd.lpLine->wpLine, ciEnd.nCharInLine * sizeof(wchar_t));
                wszBufferCount+=ciEnd.nCharInLine;
                break;
              }

              if (lpElement == ciStart.lpLine)
              {
                //First line
                nCharsToCopy=ciStart.lpLine->nLineLen - ciStart.nCharInLine;
                AE_memcpy(wszBufferCount, ciStart.lpLine->wpLine + ciStart.nCharInLine, nCharsToCopy * sizeof(wchar_t));
                wszBufferCount+=nCharsToCopy;
              }
              else
              {
                //Middle
                AE_memcpy(wszBufferCount, lpElement->wpLine, lpElement->nLineLen * sizeof(wchar_t));
                wszBufferCount+=lpElement->nLineLen;
              }

              if (nNewLine == AELB_ASIS)
                nLineBreak=lpElement->nLineBreak;
              else
                nLineBreak=nNewLine;

              if (nLineBreak == AELB_R)
              {
                *wszBufferCount++=L'\r';
              }
              else if (nLineBreak == AELB_N)
              {
                *wszBufferCount++=L'\n';
              }
              else if (nLineBreak == AELB_RN)
              {
                *wszBufferCount++=L'\r';
                *wszBufferCount++=L'\n';
              }
              else if (nLineBreak == AELB_RRN)
              {
                *wszBufferCount++=L'\r';
                *wszBufferCount++=L'\r';
                *wszBufferCount++=L'\n';
              }
              lpElement=lpElement->next;
            }
          }
        }
      }
      *wszBufferCount=L'\0';
    }
    else return AE_IndexSubtract(ae, &ciStart, &ciEnd, nNewLine, bColumnSel, bFillSpaces);
  }
  return wszBufferCount - wszBuffer;
}

void AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, BOOL bEnableUndo)
{
  AECHARINDEX ciDeleteStart=*ciRangeStart;
  AECHARINDEX ciDeleteEnd=*ciRangeEnd;
  AECHARINDEX ciTmp;
  AELINEDATA *lpFirstElement=NULL;
  AELINEDATA *lpNewElement=NULL;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpNextElement=NULL;
  int nLastLineSelStart=0;
  int nLineCount=0;
  int nHScrollPos;
  int nVScrollPos;
  int nResult;

  if (ciRangeStart->lpLine && ciRangeEnd->lpLine)
  {
    //Exchange indexes
    nResult=AE_IndexCompare(&ciDeleteStart, &ciDeleteEnd);

    if (nResult > 0)
    {
      ciTmp=ciDeleteStart;
      ciDeleteStart=ciDeleteEnd;
      ciDeleteEnd=ciTmp;
    }
    else if (nResult == 0) return;

    //Add undo
    if (bEnableUndo)
    {
      AE_SetModify(ae, TRUE);

      if (ae->dwUndoLimit)
      {
        AEUNDOITEM *lpUndoElement;
        wchar_t *wpUndoText;
        DWORD dwUndoTextLen;

        dwUndoTextLen=AE_IndexSubtract(ae, &ciDeleteStart, &ciDeleteEnd, AELB_ASIS, bColumnSel, FALSE);

        if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
        {
          AE_GetTextRange(ae, &ciDeleteStart, &ciDeleteEnd, wpUndoText, AELB_ASIS, bColumnSel, FALSE);

          if (lpUndoElement=AE_StackUndoItemInsert(ae))
          {
            lpUndoElement->dwFlags=AEUN_INSERT|(bColumnSel?AEUN_COLUMNSEL:0);
            lpUndoElement->ciActionStart=ciDeleteStart;
            lpUndoElement->ciActionEnd=ciDeleteEnd;
            lpUndoElement->wpText=wpUndoText;
            lpUndoElement->dwTextLen=dwUndoTextLen;

            ae->lpCurrentUndo=lpUndoElement;
          }
        }
      }
    }

    if (bColumnSel)
    {
      //Caret location
      nResult=AE_IndexCompare(&ciDeleteStart, &ae->ciCaretIndex);

      //Lines after deletion
      lpElement=ciDeleteStart.lpLine;

      while (lpElement)
      {
        if (lpNewElement=AE_StackLineInsertBefore(ae, ciDeleteStart.lpLine))
        {
          if (!lpFirstElement)
            lpFirstElement=lpNewElement;

          lpNewElement->nLineWidth=-1;
          lpNewElement->nLineBreak=lpElement->nLineBreak;
          lpNewElement->nLineLen=lpElement->nSelStart + max(lpElement->nLineLen - lpElement->nSelEnd, 0);
          lpNewElement->nLineLen=min(lpNewElement->nLineLen, lpElement->nLineLen);

          if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
          {
            AE_memcpy(lpNewElement->wpLine, lpElement->wpLine, min(lpElement->nSelStart, lpNewElement->nLineLen) * sizeof(wchar_t));
            if (lpElement->nSelEnd < lpElement->nLineLen)
              AE_memcpy(lpNewElement->wpLine + min(lpElement->nSelStart, lpNewElement->nLineLen), lpElement->wpLine + lpElement->nSelEnd, (lpElement->nLineLen - lpElement->nSelEnd) * sizeof(wchar_t));
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
          }
        }
        if (lpElement == ciDeleteEnd.lpLine)
        {
          nLastLineSelStart=ciDeleteEnd.lpLine->nSelStart;
          break;
        }
        ++nLineCount;

        lpElement=lpElement->next;
      }

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

      //Set global
      nHScrollPos=ae->nHScrollPos;
      nVScrollPos=ae->nVScrollPos;

      if (!nResult)
      {
        ae->ciCaretIndex.nLine=ciDeleteStart.nLine;
        ae->ciCaretIndex.lpLine=lpFirstElement;
        ae->ciCaretIndex.nCharInLine=ciDeleteStart.nCharInLine;
      }
      else
      {
        ae->ciCaretIndex.nLine=ciDeleteEnd.nLine;
        ae->ciCaretIndex.lpLine=lpNewElement;
        ae->ciCaretIndex.nCharInLine=nLastLineSelStart;
      }
      ae->ciSelStartIndex=ae->ciCaretIndex;
      ae->ciSelEndIndex=ae->ciCaretIndex;

      if (!ae->liMaxWidthLine.lpLine)
        AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
      else
        AE_CalcLinesWidth(ae, (AELINEINDEX *)&ae->ciCaretIndex, (AELINEINDEX *)&ae->ciCaretIndex, FALSE);

      //Get new caret position
      AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL, bColumnSel);
      AE_ScrollToCaret(ae, &ae->ptCaret);
      AE_SetCaretPos(ae);
      ae->nHorizCaretPos=ae->ptCaret.x;
      if (!ae->bCaretVisible)
      {
        ShowCaret(ae->hWndEdit);
        ae->bCaretVisible=TRUE;
      }

      //Redraw lines
      if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }
      else
      {
        AE_RedrawLineRange(ae, ciDeleteStart.nLine, ciDeleteEnd.nLine, FALSE);
      }
    }
    else
    {
      //Normalize delete position
      ciDeleteStart.nCharInLine=min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nLineLen);
      ciDeleteEnd.nCharInLine=min(ciDeleteEnd.nCharInLine, ciDeleteEnd.lpLine->nLineLen);

      //Line after deletion
      if (lpNewElement=AE_StackLineInsertBefore(ae, ciDeleteStart.lpLine))
      {
        lpNewElement->nLineWidth=-1;
        lpNewElement->nLineBreak=ciDeleteEnd.lpLine->nLineBreak;
        lpNewElement->nLineLen=ciDeleteStart.nCharInLine + (ciDeleteEnd.lpLine->nLineLen - ciDeleteEnd.nCharInLine);

        if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
        {
          AE_memcpy(lpNewElement->wpLine, ciDeleteStart.lpLine->wpLine, ciDeleteStart.nCharInLine * sizeof(wchar_t));
          AE_memcpy(lpNewElement->wpLine + ciDeleteStart.nCharInLine, ciDeleteEnd.lpLine->wpLine + ciDeleteEnd.nCharInLine, (ciDeleteEnd.lpLine->nLineLen - ciDeleteEnd.nCharInLine) * sizeof(wchar_t));
          lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
        }
      }

      //Delete lines in range
      lpElement=ciDeleteStart.lpLine;

      while (lpElement)
      {
        if (lpElement == ciDeleteEnd.lpLine)
        {
          AE_StackLineDelete(ae, lpElement);
          break;
        }
        ++nLineCount;
        lpNextElement=lpElement->next;
        AE_StackLineDelete(ae, lpElement);
        lpElement=lpNextElement;
      }

      //Set global
      nHScrollPos=ae->nHScrollPos;
      nVScrollPos=ae->nVScrollPos;
      ae->nLineCount-=nLineCount;
      ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
      ae->ciCaretIndex.nLine=ciDeleteStart.nLine;
      ae->ciCaretIndex.lpLine=lpNewElement;
      ae->ciCaretIndex.nCharInLine=ciDeleteStart.nCharInLine;
      ae->ciSelStartIndex=ae->ciCaretIndex;
      ae->ciSelEndIndex=ae->ciCaretIndex;
      AE_UpdateScrollBars(ae, SB_VERT);

      if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciDeleteStart.nLine)
        ae->liFirstDrawLine.nLine-=nLineCount;
      if (ae->liMaxWidthLine.lpLine && ae->liMaxWidthLine.nLine > ciDeleteStart.nLine)
        ae->liMaxWidthLine.nLine-=nLineCount;

      if (!ae->liMaxWidthLine.lpLine)
        AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
      else
        AE_CalcLinesWidth(ae, (AELINEINDEX *)&ae->ciCaretIndex, (AELINEINDEX *)&ae->ciCaretIndex, FALSE);

      //Get new caret position
      AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL, bColumnSel);
      AE_ScrollToCaret(ae, &ae->ptCaret);
      AE_SetCaretPos(ae);
      ae->nHorizCaretPos=ae->ptCaret.x;
      if (!ae->bCaretVisible)
      {
        ShowCaret(ae->hWndEdit);
        ae->bCaretVisible=TRUE;
      }

      //Redraw lines
      if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
      {
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
      }
      else
      {
        if (!nLineCount)
          AE_RedrawLineRange(ae, ciDeleteStart.nLine, ciDeleteStart.nLine, FALSE);
        else
          AE_RedrawLineRange(ae, ciDeleteStart.nLine, -1, TRUE);
      }
    }
  }

  //Send AEN_SELCHANGE
  {
    AENSELCHANGE sc;

    sc.hdr.hwndFrom=ae->hWndEdit;
    sc.hdr.idFrom=ae->nEditCtrlID;
    sc.hdr.code=AEN_SELCHANGE;
    sc.crSel.ciMin=ae->ciSelStartIndex;
    sc.crSel.ciMax=ae->ciSelEndIndex;
    sc.ciCaret=ae->ciCaretIndex;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }

  //Send AEN_TEXTCHANGE
  {
    AENTEXTCHANGE tc;

    tc.hdr.hwndFrom=ae->hWndEdit;
    tc.hdr.idFrom=ae->nEditCtrlID;
    tc.hdr.code=AEN_TEXTCHANGE;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }
}

DWORD AE_GetTextAnsi(AKELEDIT *ae, char *szText, DWORD dwMaxTextLen)
{
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  wchar_t *wszText;
  DWORD dwTextLen;
  DWORD dwAnsiBytes;
  DWORD dwUnicodeBytes;

  if (dwMaxTextLen)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciFirstChar, FALSE);

    if (dwUnicodeBytes=AE_IndexOffset(ae, &ciFirstChar, &ciLastChar, dwMaxTextLen - 1, ae->nOutputNewLine) * sizeof(wchar_t) + 2)
    {
      if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
      {
        dwTextLen=AE_GetTextRange(ae, &ciFirstChar, &ciLastChar, wszText, ae->nOutputNewLine, FALSE, TRUE);

        dwAnsiBytes=WideCharToMultiByte(CP_ACP, 0, wszText, dwTextLen + 1, NULL, 0, NULL, NULL);
        dwAnsiBytes=min(dwAnsiBytes - 1, dwMaxTextLen - 1);
        WideCharToMultiByte(CP_ACP, 0, wszText, dwTextLen + 1, szText, dwAnsiBytes + 1, NULL, NULL);
        szText[dwAnsiBytes]=L'\0';

        AE_HeapFree(ae, 0, (LPVOID)wszText);
        return dwAnsiBytes;
      }
    }
  }
  return 0;
}

DWORD AE_GetText(AKELEDIT *ae, wchar_t *wszText, DWORD dwMaxTextLen)
{
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;

  if (dwMaxTextLen)
  {
    AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciFirstChar, FALSE);

    if (AE_IndexOffset(ae, &ciFirstChar, &ciLastChar, dwMaxTextLen, ae->nOutputNewLine))
    {
      return AE_GetTextRange(ae, &ciFirstChar, &ciLastChar, wszText, ae->nOutputNewLine, FALSE, TRUE);
    }
  }
  return 0;
}

DWORD AE_SetTextAnsi(AKELEDIT *ae, char *pText, DWORD dwTextLen, int nNewLine)
{
  wchar_t *wszText;
  DWORD dwUnicodeBytes;
  DWORD dwResult=0;

  dwUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pText, dwTextLen, NULL, 0) * sizeof(wchar_t);
  if (dwTextLen == (DWORD)-1) dwUnicodeBytes-=2;

  if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(CP_ACP, 0, pText, dwTextLen, wszText, dwUnicodeBytes / sizeof(wchar_t));
    dwResult=AE_SetText(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), nNewLine);

    AE_HeapFree(NULL, 0, (LPVOID)wszText);
  }
  return dwResult;
}

DWORD AE_SetText(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, int nNewLine)
{
  wchar_t *wpLineStart=wpText;
  wchar_t *wpLineEnd=wpText;
  AELINEDATA *lpElement=NULL;
  DWORD dwTextCount=0;
  int nMaxLineLen=0;

  //Free memory
  if (ae->hHeap)
  {
    if (HeapDestroy(ae->hHeap))
      ae->hHeap=NULL;

    ae->hUndoStack.first=0;
    ae->hUndoStack.last=0;
    ae->hLinesStack.first=0;
    ae->hLinesStack.last=0;
    ae->lpCurrentUndo=NULL;
    ae->ciCaretIndex.nLine=0;
    ae->ciCaretIndex.nCharInLine=0;
    ae->ciCaretIndex.lpLine=NULL;
    ae->ciSelStartIndex.nLine=0;
    ae->ciSelStartIndex.nCharInLine=0;
    ae->ciSelStartIndex.lpLine=NULL;
    ae->ciSelEndIndex.nLine=0;
    ae->ciSelEndIndex.nCharInLine=0;
    ae->ciSelEndIndex.lpLine=NULL;
    ae->liFirstDrawLine.nLine=0;
    ae->liFirstDrawLine.lpLine=NULL;
    ae->liMaxWidthLine.nLine=0;
    ae->liMaxWidthLine.lpLine=NULL;
  }
  ae->hHeap=HeapCreate(0, 0, 0);

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

  //Parse text
  if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);

  for (ae->nLineCount=0; dwTextCount < dwTextLen; ++ae->nLineCount)
  {
    if (lpElement=AE_StackLineAdd(ae))
    {
      wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &lpElement->nLineLen, &lpElement->nLineBreak);
      dwTextCount+=wpLineEnd - wpLineStart;
      if (lpElement->nLineBreak != AELB_EOF && nNewLine != AELB_ASIS)
        lpElement->nLineBreak=nNewLine;

      if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpElement->nLineLen * sizeof(wchar_t) + 2))
      {
        AE_memcpy(lpElement->wpLine, wpLineStart, lpElement->nLineLen * sizeof(wchar_t));
        lpElement->wpLine[lpElement->nLineLen]=L'\0';
        nMaxLineLen=max(nMaxLineLen, lpElement->nLineLen);
        lpElement->nLineWidth=-1;
      }
    }
    wpLineStart=wpLineEnd;
  }

  if (!lpElement || lpElement->nLineBreak != AELB_EOF)
  {
    if (lpElement=AE_StackLineAdd(ae))
    {
      if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, sizeof(wchar_t)))
      {
        lpElement->wpLine[0]=L'\0';
        lpElement->nLineWidth=-1;
      }
      lpElement->nLineBreak=AELB_EOF;
    }
    ++ae->nLineCount;
  }
  if (ae->nLineCount) --ae->nLineCount;

  ae->nHScrollMax=(nMaxLineLen + 1) * ae->nAveCharWidth;
  ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ae->ciCaretIndex, FALSE);
  ae->ciSelStartIndex=ae->ciCaretIndex;
  ae->ciSelEndIndex=ae->ciCaretIndex;
  AE_UpdateScrollBars(ae, SB_BOTH);
  InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
  UpdateWindow(ae->hWndEdit);

  //Get new caret position
  AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL, FALSE);
  AE_ScrollToCaret(ae, &ae->ptCaret);
  AE_SetCaretPos(ae);
  ae->nHorizCaretPos=ae->ptCaret.x;
  if (!ae->bCaretVisible)
  {
    ShowCaret(ae->hWndEdit);
    ae->bCaretVisible=TRUE;
  }

  //Calculate widths
  AE_CalcLinesWidth(ae, NULL, NULL, FALSE);

  return dwTextLen;
}

void AE_ReplaceSelAnsi(AKELEDIT *ae, char *pText, DWORD dwTextLen, BOOL bColumnSel)
{
  wchar_t *wszText;
  DWORD dwUnicodeBytes;

  dwUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, pText, dwTextLen, NULL, 0) * sizeof(wchar_t);
  if (dwTextLen == (DWORD)-1) dwUnicodeBytes-=2;

  if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(CP_ACP, 0, pText, dwTextLen, wszText, dwUnicodeBytes / sizeof(wchar_t));
    AE_ReplaceSel(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), bColumnSel);

    AE_HeapFree(ae, 0, (LPVOID)wszText);
  }
}

void AE_ReplaceSel(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, BOOL bColumnSel)
{
  AECHARINDEX ciInsertStart={0};
  AECHARINDEX ciInsertEnd={0};

  AE_StackUndoGroupStop(ae);
  AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
  AE_InsertText(ae, &ae->ciSelStartIndex, wpText, dwTextLen, ae->nInputNewLine, bColumnSel, &ciInsertStart, &ciInsertEnd, TRUE);
  AE_StackUndoGroupStop(ae);

  if (bColumnSel) AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertStart, bColumnSel);
}

DWORD AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd, BOOL bEnableUndo)
{
  AECHARINDEX ciInsertFrom=*ciInsertPos;
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  AECHARINDEX ciFirstNewLine;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpNewElement=NULL;
  AELINEDATA *lpNextElement=NULL;
  AELINEDATA *lpInsertToElement=NULL;
  POINT ptInsertFrom;
  wchar_t *wpLineStart=wpText;
  wchar_t *wpLineEnd;
  int nLineLen=0;
  int nLineBreak;
  int nLineCount=0;
  int nCaretIndexInLine=0;
  int nSpaces=0;
  int nHScrollPos;
  int nVScrollPos;
  int i;
  DWORD dwTextCount=0;

  if (ciInsertPos->lpLine)
  {
    //Set new line
    if (nNewLine == AELB_ASINPUT)
      nNewLine=ae->nInputNewLine;
    else if (nNewLine == AELB_ASOUTPUT)
      nNewLine=ae->nOutputNewLine;

    if (bColumnSel)
    {
      //Parse text
      if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);

      AE_GetPosFromChar(ae, &ciInsertFrom, &ptInsertFrom, NULL, bColumnSel);
      ciFirstChar.nLine=ciInsertFrom.nLine;
      ciFirstChar.lpLine=NULL;
      ciFirstChar.nCharInLine=ciInsertFrom.nCharInLine;
      ciLastChar.nLine=ciInsertFrom.nLine;
      ciLastChar.lpLine=NULL;
      ciLastChar.nCharInLine=ciInsertFrom.nCharInLine;
      lpElement=ciInsertFrom.lpLine;

      while (dwTextCount < dwTextLen)
      {
        if (lpNewElement=AE_StackLineInsertBefore(ae, ciInsertFrom.lpLine))
        {
          wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &nLineLen, &nLineBreak);
          dwTextCount+=wpLineEnd - wpLineStart;
          if (nLineBreak != AELB_EOF && nNewLine != AELB_ASIS)
            nLineBreak=nNewLine;

          if (lpElement)
          {
            //First insert line
            if (!ciFirstChar.lpLine)
              ciFirstChar.lpLine=lpNewElement;
            else
              AE_GetCharRangeInLine(ae, lpElement->wpLine, lpElement->nLineLen, ptInsertFrom.x, 0, &ciInsertFrom.nCharInLine, NULL, NULL, NULL, bColumnSel);

            lpNewElement->nLineWidth=-1;
            if (lpElement->nLineBreak != AELB_EOF)
              lpNewElement->nLineBreak=lpElement->nLineBreak;
            else
              lpNewElement->nLineBreak=nLineBreak;
            if (!nLineLen)
              lpNewElement->nLineLen=lpElement->nLineLen;
            else
              lpNewElement->nLineLen=lpElement->nLineLen + max(ciInsertFrom.nCharInLine - lpElement->nLineLen, 0) + nLineLen;
            nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;

            if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
            {
              if (!nLineLen)
              {
                AE_memcpy(lpNewElement->wpLine, lpElement->wpLine, lpElement->nLineLen * sizeof(wchar_t));
              }
              else
              {
                AE_memcpy(lpNewElement->wpLine, lpElement->wpLine, min(ciInsertFrom.nCharInLine, lpElement->nLineLen) * sizeof(wchar_t));

                if (ciInsertFrom.nCharInLine > lpElement->nLineLen)
                {
                  for (i=lpElement->nLineLen; i < ciInsertFrom.nCharInLine; ++i)
                    lpNewElement->wpLine[i]=' ';
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                }
                else
                {
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine + nLineLen, lpElement->wpLine + ciInsertFrom.nCharInLine, (lpElement->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                }

                //Add undo
                if (bEnableUndo)
                {
                  if (ae->dwUndoLimit)
                  {
                    AEUNDOITEM *lpUndoElement;
                    wchar_t *wpUndoText;
                    DWORD dwUndoTextLen=max(ciInsertFrom.nCharInLine - lpElement->nLineLen, 0) + nLineLen;

                    if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
                    {
                      nSpaces=ciInsertFrom.nCharInLine - lpElement->nLineLen;

                      for (i=0; i < nSpaces; ++i)
                        wpUndoText[i]=' ';
                      AE_memcpy(wpUndoText + i, wpLineStart, nLineLen * sizeof(wchar_t));
                      wpUndoText[dwUndoTextLen]=L'\0';

                      if (lpUndoElement=AE_StackUndoItemInsert(ae))
                      {
                        lpUndoElement->dwFlags=AEUN_DELETE;
                        lpUndoElement->ciActionStart.nLine=ciLastChar.nLine;
                        lpUndoElement->ciActionStart.lpLine=lpNewElement;
                        lpUndoElement->ciActionStart.nCharInLine=min(ciInsertFrom.nCharInLine, lpElement->nLineLen);
                        lpUndoElement->ciActionEnd.nLine=ciLastChar.nLine;
                        lpUndoElement->ciActionEnd.lpLine=lpNewElement;
                        lpUndoElement->ciActionEnd.nCharInLine=min(ciInsertFrom.nCharInLine, lpElement->nLineLen) + dwUndoTextLen;
                        lpUndoElement->wpText=wpUndoText;
                        lpUndoElement->dwTextLen=dwUndoTextLen;

                        ae->lpCurrentUndo=lpUndoElement;
                      }
                    }
                  }
                }
              }
              lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
            }
            lpInsertToElement=lpElement;

            if (!lpElement->next)
            {
              ciFirstNewLine.nLine=ciLastChar.nLine;
              ciFirstNewLine.lpLine=lpNewElement;
              ciFirstNewLine.nCharInLine=lpNewElement->nLineLen;
            }
            lpElement=lpElement->next;
          }
          else
          {
            AE_GetCharRangeInLine(ae, NULL, 0, ptInsertFrom.x, 0, &ciInsertFrom.nCharInLine, NULL, NULL, NULL, bColumnSel);

            lpNewElement->nLineWidth=-1;
            lpNewElement->nLineBreak=nLineBreak;
            if (!nLineLen)
              lpNewElement->nLineLen=0;
            else
              lpNewElement->nLineLen=ciInsertFrom.nCharInLine + nLineLen;
            nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;

            if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
            {
              if (nLineLen)
              {
                for (i=0; i < ciInsertFrom.nCharInLine; ++i)
                  lpNewElement->wpLine[i]=' ';
                AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
              }
              lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
            }
            ++nLineCount;
          }
          if (dwTextCount >= dwTextLen) break;
          if (nLineBreak == AELB_EOF) break;
          ++ciLastChar.nLine;
          wpLineStart=wpLineEnd;
        }
      }

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

        //Set global
        nHScrollPos=ae->nHScrollPos;
        nVScrollPos=ae->nVScrollPos;
        ae->nLineCount+=nLineCount;
        ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
        ae->ciCaretIndex=ciLastChar;
        ae->ciSelStartIndex=ae->ciCaretIndex;
        ae->ciSelEndIndex=ae->ciCaretIndex;
        AE_UpdateScrollBars(ae, SB_VERT);

        if (!ae->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, FALSE);

        //Get new caret position
        AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL, bColumnSel);
        AE_ScrollToCaret(ae, &ae->ptCaret);
        AE_SetCaretPos(ae);
        ae->nHorizCaretPos=ae->ptCaret.x;
        if (!ae->bCaretVisible)
        {
          ShowCaret(ae->hWndEdit);
          ae->bCaretVisible=TRUE;
        }

        //Redraw lines
        if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
        {
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        }
        else
        {
          AE_RedrawLineRange(ae, ciFirstChar.nLine, ciLastChar.nLine, FALSE);
        }

        //Add undo
        if (bEnableUndo)
        {
          AE_SetModify(ae, TRUE);

          if (ae->dwUndoLimit)
          {
            //Delete spaces
            if (nLineCount)
            {
              AEUNDOITEM *lpUndoElement;

              if (lpUndoElement=AE_StackUndoItemInsert(ae))
              {
                lpUndoElement->dwFlags=AEUN_DELETE;
                lpUndoElement->ciActionStart=ciFirstNewLine;
                lpUndoElement->ciActionEnd.nLine=ciLastChar.nLine;
                lpUndoElement->ciActionEnd.lpLine=lpNewElement;
                lpUndoElement->ciActionEnd.nCharInLine=lpNewElement->nLineLen;
                lpUndoElement->wpText=NULL;
                lpUndoElement->dwTextLen=0;

                ae->lpCurrentUndo=lpUndoElement;
              }
            }

            //Set selection
            {
              AEUNDOITEM *lpUndoElement;

              if (lpUndoElement=AE_StackUndoItemInsert(ae))
              {
                lpUndoElement->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL;
                lpUndoElement->ciActionStart=ciFirstChar;
                lpUndoElement->ciActionEnd=ciLastChar;
                lpUndoElement->wpText=NULL;
                lpUndoElement->dwTextLen=0;

                ae->lpCurrentUndo=lpUndoElement;
              }
            }
          }
        }
      }
    }
    else
    {
      //Parse text
      if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);

      lpNewElement=ciInsertFrom.lpLine;

      while (dwTextCount < dwTextLen)
      {
        if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
        {
          wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &nLineLen, &nLineBreak);
          dwTextCount+=wpLineEnd - wpLineStart;
          if (nLineBreak != AELB_EOF && nNewLine != AELB_ASIS)
            nLineBreak=nNewLine;

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

            if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
            {
              AE_memcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine, min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen) * sizeof(wchar_t));

              if (ciInsertFrom.nCharInLine > ciInsertFrom.lpLine->nLineLen)
              {
                nSpaces=ciInsertFrom.nCharInLine - ciInsertFrom.lpLine->nLineLen;

                for (i=ciInsertFrom.lpLine->nLineLen; i < ciInsertFrom.nCharInLine; ++i)
                  lpNewElement->wpLine[i]=' ';
              }

              if (nLineBreak == AELB_EOF)
              {
                //wpText - one line
                AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine + nLineLen, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
              }
              else
              {
                AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
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

                    if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
                    {
                      if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                        AE_memcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                      lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                    }
                  }
                }
              }
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
              lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine, 0) + nLineLen;
              nCaretIndexInLine=nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
              {
                AE_memcpy(lpNewElement->wpLine, wpLineStart, nLineLen * sizeof(wchar_t));
                if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                  AE_memcpy(lpNewElement->wpLine + nLineLen, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
              }
            }
            else
            {
              lpNewElement->nLineWidth=-1;
              lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=nLineLen;
              nCaretIndexInLine=0;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
              {
                AE_memcpy(lpNewElement->wpLine, wpLineStart, lpNewElement->nLineLen * sizeof(wchar_t));
                lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
              }
              ++nLineCount;

              //End of line as new line
              if (lpNewElement=AE_StackLineInsertAfter(ae, lpNewElement))
              {
                lpNewElement->nLineWidth=-1;
                lpNewElement->nLineBreak=ciInsertFrom.lpLine->nLineBreak;
                lpNewElement->nLineLen=max(ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine, 0);
                nCaretIndexInLine=0;

                if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
                {
                  if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                    AE_memcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                  lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
                }
              }
            }
            break;
          }

          //Center lines
          {
            lpNewElement->nLineWidth=-1;
            lpNewElement->nLineBreak=nLineBreak;
            lpNewElement->nLineLen=nLineLen;
            nCaretIndexInLine=0;

            if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
            {
              AE_memcpy(lpNewElement->wpLine, wpLineStart, lpNewElement->nLineLen * sizeof(wchar_t));
              lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
            }
            ++nLineCount;
            wpLineStart=wpLineEnd;
          }
        }
      }

      if (dwTextCount)
      {
        AE_StackLineDelete(ae, ciInsertFrom.lpLine);

        //Set global
        nHScrollPos=ae->nHScrollPos;
        nVScrollPos=ae->nVScrollPos;
        ae->nLineCount+=nLineCount;
        ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
        ae->ciCaretIndex.nLine=ciInsertFrom.nLine + nLineCount;
        ae->ciCaretIndex.lpLine=lpNewElement;
        ae->ciCaretIndex.nCharInLine=nCaretIndexInLine;
        ae->ciSelStartIndex=ae->ciCaretIndex;
        ae->ciSelEndIndex=ae->ciCaretIndex;
        ciLastChar=ae->ciCaretIndex;
        AE_UpdateScrollBars(ae, SB_VERT);

        if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciInsertFrom.nLine)
          ae->liFirstDrawLine.nLine+=nLineCount;
        if (ae->liMaxWidthLine.lpLine && ae->liMaxWidthLine.nLine > ciInsertFrom.nLine)
          ae->liMaxWidthLine.nLine+=nLineCount;

        if (!ae->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, FALSE);

        //Get new caret position
        AE_GetPosFromChar(ae, &ae->ciCaretIndex, &ae->ptCaret, NULL, bColumnSel);
        AE_ScrollToCaret(ae, &ae->ptCaret);
        AE_SetCaretPos(ae);
        ae->nHorizCaretPos=ae->ptCaret.x;
        if (!ae->bCaretVisible)
        {
          ShowCaret(ae->hWndEdit);
          ae->bCaretVisible=TRUE;
        }

        //Redraw lines
        if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
        {
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        }
        else
        {
          if (!nLineCount)
            AE_RedrawLineRange(ae, ciInsertFrom.nLine, ciInsertFrom.nLine, FALSE);
          else
            AE_RedrawLineRange(ae, ciInsertFrom.nLine, -1, TRUE);
        }

        //Add undo
        if (bEnableUndo)
        {
          AE_SetModify(ae, TRUE);

          if (ae->dwUndoLimit)
          {
            AEUNDOITEM *lpUndoElement;
            wchar_t *wpUndoText;

            if (nSpaces > 0)
            {
              if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, nSpaces * sizeof(wchar_t) + 2))
              {
                for (i=0; i < nSpaces; ++i)
                  wpUndoText[i]=' ';
                wpUndoText[nSpaces]=L'\0';

                if (lpUndoElement=AE_StackUndoItemInsert(ae))
                {
                  lpUndoElement->dwFlags=AEUN_DELETE;
                  lpUndoElement->ciActionStart=ciFirstChar;
                  lpUndoElement->ciActionEnd.nLine=ciFirstChar.nLine;
                  lpUndoElement->ciActionEnd.lpLine=ciFirstChar.lpLine;
                  lpUndoElement->ciActionEnd.nCharInLine=ciFirstChar.nCharInLine + nSpaces;
                  lpUndoElement->wpText=wpUndoText;
                  lpUndoElement->dwTextLen=nSpaces;

                  ae->lpCurrentUndo=lpUndoElement;
                }
              }
            }

            if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwTextLen * sizeof(wchar_t) + 2))
            {
              AE_memcpy(wpUndoText, wpText, dwTextLen * sizeof(wchar_t));
              wpUndoText[dwTextLen]=L'\0';

              if (lpUndoElement=AE_StackUndoItemInsert(ae))
              {
                lpUndoElement->dwFlags=AEUN_DELETE;
                lpUndoElement->ciActionStart.nLine=ciFirstChar.nLine;
                lpUndoElement->ciActionStart.lpLine=ciFirstChar.lpLine;
                lpUndoElement->ciActionStart.nCharInLine=ciFirstChar.nCharInLine + nSpaces;
                lpUndoElement->ciActionEnd=ciLastChar;
                lpUndoElement->wpText=wpUndoText;
                lpUndoElement->dwTextLen=dwTextLen;

                ae->lpCurrentUndo=lpUndoElement;
              }
            }
          }
        }
      }
    }

    if (dwTextCount)
    {
      //Result indexes
      if (ciInsertStart) *ciInsertStart=ciFirstChar;
      if (ciInsertEnd) *ciInsertEnd=ciLastChar;

      //Send AEN_SELCHANGE
      {
        AENSELCHANGE sc;

        sc.hdr.hwndFrom=ae->hWndEdit;
        sc.hdr.idFrom=ae->nEditCtrlID;
        sc.hdr.code=AEN_SELCHANGE;
        sc.crSel.ciMin=ae->ciSelStartIndex;
        sc.crSel.ciMax=ae->ciSelEndIndex;
        sc.ciCaret=ae->ciCaretIndex;
        SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
      }

      //Send AEN_TEXTCHANGE
      {
        AENTEXTCHANGE tc;

        tc.hdr.hwndFrom=ae->hWndEdit;
        tc.hdr.idFrom=ae->nEditCtrlID;
        tc.hdr.code=AEN_TEXTCHANGE;
        SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
      }
    }
  }
  return dwTextCount;
}

wchar_t* AE_GetNextLine(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, int *nLineLen, int *nLineBreak)
{
  wchar_t *wpLineEnd;
  DWORD i;

  if (dwTextLen == (DWORD)-1)
  {
    for (i=0; wpText[i] && wpText[i] != L'\r' && wpText[i] != L'\n'; ++i);

    if (nLineLen) *nLineLen=i;
    if (nLineBreak) *nLineBreak=AELB_EOF;
    wpLineEnd=wpText + i;

    if (wpText[i])
    {
      if (wpText[i] == L'\r' && wpText[i + 1] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_RN;
        wpLineEnd=wpText + i + 2;
      }
      else if (wpText[i] == L'\r' && wpText[i + 1] == L'\r' && wpText[i + 2] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_RRN;
        wpLineEnd=wpText + i + 3;
      }
      else if (wpText[i] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_N;
        wpLineEnd=wpText + i + 1;
      }
      else if (wpText[i] == L'\r')
      {
        if (nLineBreak) *nLineBreak=AELB_R;
        wpLineEnd=wpText + i + 1;
      }
    }
  }
  else
  {
    for (i=0; i < dwTextLen && wpText[i] != L'\r' && wpText[i] != L'\n'; ++i);

    if (nLineLen) *nLineLen=i;
    if (nLineBreak) *nLineBreak=AELB_EOF;
    wpLineEnd=wpText + i;

    if (i < dwTextLen)
    {
      if (i + 1 < dwTextLen && (wpText[i] == L'\r' && wpText[i + 1] == L'\n'))
      {
        if (nLineBreak) *nLineBreak=AELB_RN;
        wpLineEnd=wpText + i + 2;
      }
      else if (i + 2 < dwTextLen && (wpText[i] == L'\r' && wpText[i + 1] == L'\r' && wpText[i + 2] == L'\n'))
      {
        if (nLineBreak) *nLineBreak=AELB_RRN;
        wpLineEnd=wpText + i + 3;
      }
      else if (wpText[i] == L'\n')
      {
        if (nLineBreak) *nLineBreak=AELB_N;
        wpLineEnd=wpText + i + 1;
      }
      else if (wpText[i] == L'\r')
      {
        if (nLineBreak) *nLineBreak=AELB_R;
        wpLineEnd=wpText + i + 1;
      }
    }
  }
  return wpLineEnd;
}

int AE_GetNewLineString(AKELEDIT *ae, int nNewLine, wchar_t **wpNewLine)
{
  if (nNewLine == AELB_R)
  {
    *wpNewLine=L"\r";
    return 1;
  }
  else if (nNewLine == AELB_N)
  {
    *wpNewLine=L"\n";
    return 1;
  }
  else if (nNewLine == AELB_RN)
  {
    *wpNewLine=L"\r\n";
    return 2;
  }
  else if (nNewLine == AELB_RRN)
  {
    *wpNewLine=L"\r\r\n";
    return 3;
  }
  *wpNewLine=L"\r\n";
  return 2;
}

BOOL AE_StreamOut(AKELEDIT *ae, AEStreamOut lpCallback, LPARAM lParam)
{
  AELINEDATA *lpLine=(AELINEDATA *)ae->hLinesStack.first;
  wchar_t *wszBuf;
  DWORD dwBufLen=4096;
  DWORD dwBufCount=0;
  int nCharInLine;
  int nLineBreak;
  BOOL bResult=FALSE;

  if (wszBuf=(wchar_t *)AE_HeapAlloc(ae, 0, dwBufLen * sizeof(wchar_t) + 2))
  {
    while (lpLine)
    {
      for (nCharInLine=0; nCharInLine < lpLine->nLineLen; ++nCharInLine)
      {
        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=lpLine->wpLine[nCharInLine];
      }
      if (lpLine->nLineBreak == AELB_EOF) break;

      if (ae->nOutputNewLine == AELB_ASIS)
        nLineBreak=lpLine->nLineBreak;
      else
        nLineBreak=ae->nOutputNewLine;

      if (nLineBreak == AELB_R)
      {
        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\r';
      }
      else if (nLineBreak == AELB_N)
      {
        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\n';
      }
      else if (nLineBreak == AELB_RN)
      {
        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\r';

        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\n';
      }
      else if (nLineBreak == AELB_RRN)
      {
        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\r';

        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\r';

        if (dwBufCount >= dwBufLen)
        {
          wszBuf[dwBufLen]=L'\0';
          dwBufCount=0;
          if (!lpCallback(lParam, wszBuf, dwBufLen)) goto End;
        }
        wszBuf[dwBufCount++]=L'\n';
      }
      lpLine=lpLine->next;
    }

    if (dwBufCount > 0)
    {
      wszBuf[dwBufCount]=L'\0';
      if (!lpCallback(lParam, wszBuf, dwBufCount)) goto End;
      dwBufCount=0;
    }
    bResult=TRUE;

    End:
    AE_HeapFree(ae, 0, (LPVOID)wszBuf);
  }
  return bResult;
}

BOOL AE_FindTextAnsi(AKELEDIT *ae, AEFINDTEXTA *ftA)
{
  AEFINDTEXTW ftW={0};
  wchar_t *wszText;
  DWORD dwUnicodeBytes;
  BOOL bResult=FALSE;

  if (ftA->dwTextLen == (DWORD)-1)
    ftA->dwTextLen=lstrlenA(ftA->pText);

  dwUnicodeBytes=MultiByteToWideChar(CP_ACP, 0, ftA->pText, ftA->dwTextLen + 1, NULL, 0) * sizeof(wchar_t);

  if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(CP_ACP, 0, ftA->pText, ftA->dwTextLen + 1, wszText, dwUnicodeBytes / sizeof(wchar_t));

    ftW.dwFlags=ftA->dwFlags;
    ftW.nNewLine=ftA->nNewLine;
    ftW.crSearch=ftA->crSearch;
    ftW.wpText=wszText;
    ftW.dwTextLen=dwUnicodeBytes / sizeof(wchar_t) - 1;
    bResult=AE_FindText(ae, &ftW);
    ftA->crFound=ftW.crFound;

    AE_HeapFree(ae, 0, (LPVOID)wszText);
  }
  return bResult;
}

BOOL AE_FindText(AKELEDIT *ae, AEFINDTEXTW *ft)
{
  AECHARINDEX ciCount;
  AECHARINDEX ciCountEnd;
  AECHARRANGE cr;

  if (ft->dwTextLen == (DWORD)-1)
    ft->dwTextLen=lstrlenW(ft->wpText);

  if (ft->dwFlags & AEFR_DOWN)
  {
    ciCount=ft->crSearch.ciMin;
    ciCountEnd=ft->crSearch.ciMax;
  }
  else
  {
    ciCount=ft->crSearch.ciMax;
    ciCountEnd=ft->crSearch.ciMin;
  }

  if (ft->dwFlags & AEFR_WHOLEWORD)
  {
    if (ft->dwFlags & AEFR_DOWN)
    {
      while (1)
      {
        if (AE_GetNextWord(ae, &ciCount, &cr.ciMin, &cr.ciMax, ae->bColumnSel))
        {
          if (AE_IndexCompare(&cr.ciMax, &ciCountEnd) >= 0)
            return FALSE;

          if (AE_IndexSubtract(ae, &cr.ciMin, &cr.ciMax, ft->nNewLine, FALSE, TRUE) == ft->dwTextLen)
          {
            if (AE_IsMatch(ae, ft, &cr.ciMin))
              return TRUE;
          }
          ciCount=cr.ciMax;
        }
        else return FALSE;
      }
    }
    else
    {
      while (1)
      {
        if (AE_GetPrevWord(ae, &ciCount, &cr.ciMin, &cr.ciMax, ae->bColumnSel))
        {
          if (AE_IndexCompare(&cr.ciMin, &ciCountEnd) < 0)
            return FALSE;

          if (AE_IndexSubtract(ae, &cr.ciMin, &cr.ciMax, ft->nNewLine, FALSE, TRUE) == ft->dwTextLen)
          {
            if (AE_IsMatch(ae, ft, &cr.ciMin))
              return TRUE;
          }
          ciCount=cr.ciMin;
        }
        else return FALSE;
      }
    }
  }
  else
  {
    if (ft->dwFlags & AEFR_DOWN)
    {
      if (AE_IndexOffset(ae, &ciCountEnd, &ciCountEnd, -(int)ft->dwTextLen, ft->nNewLine))
      {
        while (1)
        {
          while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
          {
            if (AE_IndexCompare(&ciCount, &ciCountEnd) > 0)
              return FALSE;

            if (AE_IsMatch(ae, ft, &ciCount))
              return TRUE;

            ++ciCount.nCharInLine;
          }

          if (ciCount.lpLine->next)
          {
            ciCount.nLine+=1;
            ciCount.lpLine=ciCount.lpLine->next;
            ciCount.nCharInLine=0;
          }
          else return FALSE;
        }
      }
    }
    else
    {
      if (AE_IndexOffset(ae, &ciCount, &ciCount, -(int)ft->dwTextLen, ft->nNewLine))
      {
        while (1)
        {
          while (ciCount.nCharInLine >= 0)
          {
            if (AE_IndexCompare(&ciCount, &ciCountEnd) < 0)
              return FALSE;

            if (AE_IsMatch(ae, ft, &ciCount))
              return TRUE;

            --ciCount.nCharInLine;
          }

          if (ciCount.lpLine->prev)
          {
            ciCount.nLine-=1;
            ciCount.lpLine=ciCount.lpLine->prev;
            ciCount.nCharInLine=ciCount.lpLine->nLineLen - 1;
          }
          else return FALSE;
        }
      }
    }
  }
  return FALSE;
}

BOOL AE_IsMatch(AKELEDIT *ae, AEFINDTEXTW *ft, const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;
  wchar_t *wpStrCount=ft->wpText;
  int nLineBreak;
  int nNewLine=ft->nNewLine;

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

  while (1)
  {
    for (; ciCount.nCharInLine < ciCount.lpLine->nLineLen; ++ciCount.nCharInLine)
    {
      if (((ft->dwFlags & AEFR_MATCHCASE) && ciCount.lpLine->wpLine[ciCount.nCharInLine] == *wpStrCount) ||
          (!(ft->dwFlags & AEFR_MATCHCASE) && AE_WideCharUpper(ciCount.lpLine->wpLine[ciCount.nCharInLine]) == AE_WideCharUpper(*wpStrCount)))
      {
        if (!*++wpStrCount) goto Founded;
      }
      else return FALSE;
    }
    if (ciCount.lpLine->nLineBreak == AELB_EOF) return FALSE;

    if (nNewLine == AELB_ASIS)
      nLineBreak=ciCount.lpLine->nLineBreak;
    else
      nLineBreak=nNewLine;

    if (nLineBreak == AELB_R)
    {
      if (*wpStrCount != L'\r') return FALSE;
    }
    else if (nLineBreak == AELB_N)
    {
      if (*wpStrCount != L'\n') return FALSE;
    }
    else if (nLineBreak == AELB_RN)
    {
      if (*wpStrCount != L'\r') return FALSE;
      if (*++wpStrCount != L'\n') return FALSE;
    }
    else if (nLineBreak == AELB_RRN)
    {
      if (*wpStrCount != L'\r') return FALSE;
      if (*++wpStrCount != L'\n') return FALSE;
    }
    if (!*++wpStrCount) goto Founded;

    if (ciCount.lpLine->next)
    {
      ciCount.nLine+=1;
      ciCount.lpLine=ciCount.lpLine->next;
      ciCount.nCharInLine=0;
    }
    else return FALSE;
  }

  Founded:
  ft->crFound.ciMin=*ciChar;
  ft->crFound.ciMax.nLine=ciCount.nLine;
  ft->crFound.ciMax.lpLine=ciCount.lpLine;
  ft->crFound.ciMax.nCharInLine=ciCount.nCharInLine + 1;
  return TRUE;
}

BOOL AE_GetModify(AKELEDIT *ae)
{
  if (ae->bSavePointExist)
  {
    if (ae->lpCurrentUndo == ae->lpSavePoint)
      return FALSE;
  }
  return TRUE;
}

void AE_SetModify(AKELEDIT *ae, BOOL bState)
{
  if (ae->bModified != bState)
  {
    ae->bModified=!ae->bModified;

    //Send AEN_MODIFYCHANGE
    {
      AENMODIFYCHANGE mc;

      mc.hdr.hwndFrom=ae->hWndEdit;
      mc.hdr.idFrom=ae->nEditCtrlID;
      mc.hdr.code=AEN_MODIFYCHANGE;
      mc.bModified=ae->bModified;
      SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&mc);
    }
  }
}

BOOL AE_EditCanPaste(AKELEDIT *ae)
{
  BOOL bResult=FALSE;

  if (!(ae->dwOptions & AECO_READONLY))
  {
    if (OpenClipboard(NULL))
    {
      if (GetClipboardData(CF_UNICODETEXT))
      {
        bResult=TRUE;
      }
      else if (GetClipboardData(CF_TEXT))
      {
        bResult=TRUE;
      }
      CloseClipboard();
    }
  }
  return bResult;
}

BOOL AE_EditCanRedo(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    if (ae->lpCurrentUndo != (AEUNDOITEM *)ae->hUndoStack.last)
    {
      if (!ae->lpCurrentUndo)
      {
        if (ae->hUndoStack.first)
          return TRUE;
        return FALSE;
      }
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_EditCanUndo(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    if (ae->lpCurrentUndo)
      return TRUE;
  }
  return FALSE;
}

void AE_EditUndo(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AEUNDOITEM *lpElement;
    AECHARINDEX ciInsertStart;
    AECHARINDEX ciInsertEnd;
    BOOL bColumnSel;

    AE_StackUndoGroupStop(ae);
    lpElement=ae->lpCurrentUndo;

    while (lpElement)
    {
      if (lpElement->dwFlags & AEUN_COLUMNSEL)
        bColumnSel=TRUE;
      else
        bColumnSel=FALSE;

      if (lpElement->dwFlags & AEUN_INSERT)
      {
        if (AE_UpdateIndex(ae, &lpElement->ciActionStart))
        {
          ciInsertStart.lpLine=NULL;
          ciInsertEnd.lpLine=NULL;
          AE_SetSelectionPos(ae, &lpElement->ciActionStart, &lpElement->ciActionStart, bColumnSel);
          AE_InsertText(ae, &lpElement->ciActionStart, lpElement->wpText, lpElement->dwTextLen, AELB_ASIS, bColumnSel, &ciInsertStart, &ciInsertEnd, FALSE);

          if (lpElement->dwFlags & AEUN_BACKSPACEKEY)
            AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertEnd, bColumnSel);
          else if (lpElement->dwFlags & AEUN_DELETEKEY)
            AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel);
          else if (lpElement->dwFlags & AEUN_OVERTYPECHAR)
            AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel);
          else
          {
            if (bColumnSel)
            {
              if (AE_UpdateIndex(ae, &lpElement->ciActionStart) &&
                  AE_UpdateIndex(ae, &lpElement->ciActionEnd))
              {
                AE_SetSelectionPos(ae, &lpElement->ciActionEnd, &lpElement->ciActionStart, bColumnSel);
              }
            }
            else AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertStart, bColumnSel);
          }
        }
      }
      else if (lpElement->dwFlags & AEUN_DELETE)
      {
        if (AE_UpdateIndex(ae, &lpElement->ciActionStart) &&
            AE_UpdateIndex(ae, &lpElement->ciActionEnd))
        {
          if (bColumnSel)
            AE_SetSelectionPos(ae, &lpElement->ciActionStart, &lpElement->ciActionEnd, bColumnSel);
          else
            AE_SetSelectionPos(ae, &lpElement->ciActionStart, &lpElement->ciActionStart, bColumnSel);
          AE_DeleteTextRange(ae, &lpElement->ciActionStart, &lpElement->ciActionEnd, bColumnSel, FALSE);
        }
      }
      lpElement=lpElement->prev;

      //Stop undo
      ae->lpCurrentUndo=lpElement;
      if (ae->lpCurrentUndo)
        if (ae->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
          break;
    }

    if (ae->bModified != AE_GetModify(ae))
    {
      AE_SetModify(ae, !ae->bModified);
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditRedo(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AEUNDOITEM *lpElement=ae->lpCurrentUndo;
    AECHARINDEX ciInsertStart;
    AECHARINDEX ciInsertEnd;
    BOOL bColumnSel;

    if (!lpElement)
      lpElement=(AEUNDOITEM *)ae->hUndoStack.first;
    else
      lpElement=lpElement->next;

    while (lpElement)
    {
      if (lpElement->dwFlags & AEUN_COLUMNSEL)
        bColumnSel=TRUE;
      else
        bColumnSel=FALSE;

      if (lpElement->dwFlags & AEUN_INSERT)
      {
        if (AE_UpdateIndex(ae, &lpElement->ciActionStart) &&
            AE_UpdateIndex(ae, &lpElement->ciActionEnd))
        {
          if (bColumnSel)
            AE_SetSelectionPos(ae, &lpElement->ciActionStart, &lpElement->ciActionEnd, bColumnSel);
          else
            AE_SetSelectionPos(ae, &lpElement->ciActionStart, &lpElement->ciActionStart, bColumnSel);
          AE_DeleteTextRange(ae, &lpElement->ciActionStart, &lpElement->ciActionEnd, bColumnSel, FALSE);
        }
      }
      else if (lpElement->dwFlags & AEUN_DELETE)
      {
        if (AE_UpdateIndex(ae, &lpElement->ciActionStart))
        {
          ciInsertStart.lpLine=NULL;
          ciInsertEnd.lpLine=NULL;
          AE_SetSelectionPos(ae, &lpElement->ciActionStart, &lpElement->ciActionStart, bColumnSel);
          AE_InsertText(ae, &lpElement->ciActionStart, lpElement->wpText, lpElement->dwTextLen, AELB_ASIS, bColumnSel, &ciInsertStart, &ciInsertEnd, FALSE);
        }
      }
      else if (lpElement->dwFlags & AEUN_SETSEL)
      {
        if (AE_UpdateIndex(ae, &lpElement->ciActionStart) &&
            AE_UpdateIndex(ae, &lpElement->ciActionEnd))
        {
          AE_SetSelectionPos(ae, &lpElement->ciActionEnd, &lpElement->ciActionStart, bColumnSel);
        }
      }

      //Stop undo
      ae->lpCurrentUndo=lpElement;
      if (ae->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
        break;

      lpElement=lpElement->next;
    }

    if (ae->bModified != AE_GetModify(ae))
    {
      AE_SetModify(ae, !ae->bModified);
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditCut(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AE_EditCopyToClipboard(ae);
    AE_StackUndoGroupStop(ae);
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
    AE_StackUndoGroupStop(ae);
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditCopyToClipboard(AKELEDIT *ae)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  int nLen;

  if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (OpenClipboard(ae->hWndEdit))
    {
      EmptyClipboard();

      if (nLen=AE_IndexSubtract(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->nOutputNewLine, ae->bColumnSel, TRUE))
      {
        if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, nLen * sizeof(wchar_t) + 2))
        {
          if (pDataTargetW=GlobalLock(hDataTargetW))
          {
            AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)pDataTargetW, ae->nOutputNewLine, ae->bColumnSel, TRUE);

            if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, nLen + 1))
            {
              if (pDataTargetA=GlobalLock(hDataTargetA))
              {
                WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, nLen + 1, (char *)pDataTargetA, nLen + 1, NULL, NULL);
                GlobalUnlock(hDataTargetA);
              }
            }
            GlobalUnlock(hDataTargetW);
          }
        }
      }
      if (hDataTargetW) SetClipboardData(CF_UNICODETEXT, hDataTargetW);
      if (hDataTargetA) SetClipboardData(CF_TEXT, hDataTargetA);
      if (ae->bColumnSel) SetClipboardData(cfColumnSel, NULL);
      CloseClipboard();
    }
  }
}

void AE_EditPasteFromClipboard(AKELEDIT *ae, BOOL bAnsi)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    HGLOBAL hData;
    LPVOID pData;
    BOOL bColumnSel;

    bColumnSel=IsClipboardFormatAvailable(cfColumnSel);

    if (OpenClipboard(NULL))
    {
      if (!bAnsi && (hData=GetClipboardData(CF_UNICODETEXT)))
      {
        if (pData=GlobalLock(hData))
        {
          AE_ReplaceSel(ae, (wchar_t *)pData, (DWORD)-1, bColumnSel);
          GlobalUnlock(hData);
        }
      }
      else if (hData=GetClipboardData(CF_TEXT))
      {
        if (pData=GlobalLock(hData))
        {
          AE_ReplaceSelAnsi(ae, (char *)pData, (DWORD)-1, bColumnSel);
          GlobalUnlock(hData);
        }
      }
      CloseClipboard();
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditChar(AKELEDIT *ae, WPARAM wParam)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    if (!ae->bUnicodeWindow)
    {
      char chChar=(char)(WORD)wParam;
      wchar_t wchChar;

      if (ae->bOverType)
      {
        if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        {
          AECHARINDEX ciCharIndex=ae->ciCaretIndex;

          if (ciCharIndex.nCharInLine + 1 <= ciCharIndex.lpLine->nLineLen)
          {
            ++ciCharIndex.nCharInLine;
            AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel, TRUE);
            ae->lpCurrentUndo->dwFlags|=AEUN_OVERTYPECHAR;
          }
        }
      }
      MultiByteToWideChar(CP_ACP, 0, &chChar, 1, &wchChar, 1);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
      AE_InsertText(ae, &ae->ciSelStartIndex, &wchChar, 1, ae->nInputNewLine, FALSE, NULL, NULL, TRUE);

      if (ae->dwUndoLimit)
      {
        if (ae->dwOptions & AECO_DETAILEDUNDO)
          AE_StackUndoGroupStop(ae);
        else
          ae->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
      }
    }
    else
    {
      wchar_t wchChar=(wchar_t)(WORD)wParam;

      if (ae->bOverType)
      {
        if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
        {
          AECHARINDEX ciCharIndex=ae->ciCaretIndex;

          if (ciCharIndex.nCharInLine + 1 <= ciCharIndex.lpLine->nLineLen)
          {
            ++ciCharIndex.nCharInLine;
            AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel, TRUE);
            ae->lpCurrentUndo->dwFlags|=AEUN_OVERTYPECHAR;
          }
        }
      }
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
      AE_InsertText(ae, &ae->ciSelStartIndex, &wchChar, 1, ae->nInputNewLine, FALSE, NULL, NULL, TRUE);

      if (ae->dwUndoLimit)
      {
        if (ae->dwOptions & AECO_DETAILEDUNDO)
          AE_StackUndoGroupStop(ae);
        else
          ae->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
      }
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditKeyReturn(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    wchar_t *wpNewLine;
    int nNewLine;

    nNewLine=AE_GetNewLineString(ae, ae->nOutputNewLine, &wpNewLine);
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
    AE_InsertText(ae, &ae->ciSelStartIndex, wpNewLine, nNewLine, ae->nInputNewLine, FALSE, NULL, NULL, TRUE);

    if (ae->dwUndoLimit)
    {
      if (ae->dwOptions & AECO_DETAILEDUNDO)
        AE_StackUndoGroupStop(ae);
      else
        ae->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditKeyBackspace(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AECHARINDEX ciCharIndex=ae->ciCaretIndex;

    if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
    {
      if (AE_GetIndex(ae, AEGI_PREVCHAR, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel))
      {
        if (ae->bColumnSel)
        {
          ciCharIndex.nCharInLine=min(ciCharIndex.nCharInLine, ciCharIndex.lpLine->nLineLen);
          AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, ae->bColumnSel);
        }
        else
        {
          AE_DeleteTextRange(ae, &ciCharIndex, &ae->ciSelStartIndex, ae->bColumnSel, TRUE);

          if (ae->dwUndoLimit)
          {
            AE_StackUndoGroupStop(ae);
            ae->lpCurrentUndo->dwFlags|=AEUN_BACKSPACEKEY;
          }
        }
      }
      else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
    }
    else
    {
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
      AE_StackUndoGroupStop(ae);
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditKeyDelete(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AECHARINDEX ciCharIndex=ae->ciCaretIndex;

    if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
    {
      if (ae->bColumnSel)
      {
        wchar_t *wpSpaces;
        int nSpaces=ciCharIndex.nCharInLine - ciCharIndex.lpLine->nLineLen;
        int i;

        if (nSpaces > 0)
        {
          if (wpSpaces=(wchar_t *)AE_HeapAlloc(ae, 0, nSpaces * sizeof(wchar_t) + 2))
          {
            for (i=0; i < nSpaces; ++i)
              wpSpaces[i]=' ';
            wpSpaces[nSpaces]=L'\0';

            ciCharIndex.nCharInLine=min(ciCharIndex.nCharInLine, ciCharIndex.lpLine->nLineLen);
            AE_InsertText(ae, &ciCharIndex, wpSpaces, nSpaces, AELB_ASIS, FALSE, NULL, NULL, TRUE);

            AE_HeapFree(ae, 0, (LPVOID)wpSpaces);
          }
        }
      }

      if (AE_GetIndex(ae, AEGI_NEXTCHAR, &ae->ciSelStartIndex, &ciCharIndex, FALSE))
      {
        AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, FALSE, TRUE);

        if (ae->dwUndoLimit)
        {
          AE_StackUndoGroupStop(ae);
          ae->lpCurrentUndo->dwFlags|=AEUN_DELETEKEY;
        }
      }
      else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
    }
    else
    {
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, TRUE);
      AE_StackUndoGroupStop(ae);
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditSelectAll(AKELEDIT *ae)
{
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;

  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciFirstChar, FALSE);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciLastChar, FALSE);
  AE_SetSelectionPos(ae, &ciLastChar, &ciFirstChar, FALSE);
}

wchar_t AE_WideCharUpper(wchar_t c)
{
  if (c < 0x100)
  {
    if (c == 0x00b5)
      return 0x039c;

    if ((c >= 0x00e0 && c <= 0x00fe) ||
        (c >= 0x0061 && c <= 0x007a))
      return (c - 0x20);

    if (c == 0xff)
      return 0x0178;

    return c;
  }
  else if (c < 0x300)
  {
    if ((c >= 0x0101 && c <= 0x012f) ||
        (c >= 0x0133 && c <= 0x0137) ||
        (c >= 0x014b && c <= 0x0177) ||
        (c >= 0x01df && c <= 0x01ef) ||
        (c >= 0x01f9 && c <= 0x021f) ||
        (c >= 0x0223 && c <= 0x0233))
    {
      if (c & 0x01)
        return (c - 1);
      return c;
    }

    if ((c >= 0x013a && c <= 0x0148) ||
        (c >= 0x01ce && c <= 0x1dc))
    {
      if (!(c & 0x01))
        return (c - 1);
      return c;
    }

    if (c == 0x0131)
      return 0x0049;

    if (c == 0x017a || c == 0x017c || c == 0x017e)
      return (c - 1);

    if (c >= 0x017f && c <= 0x0292)
    {
      wchar_t k;

      switch (c)
      {
        case 0x017f:
          k=0x0053;
          break;
        case 0x0183:
          k=0x0182;
          break;
        case 0x0185:
          k=0x0184;
          break;
        case 0x0188:
          k=0x0187;
          break;
        case 0x018c:
          k=0x018b;
          break;
        case 0x0192:
          k=0x0191;
          break;
        case 0x0195:
          k=0x01f6;
          break;
        case 0x0199:
          k=0x0198;
          break;
        case 0x019e:
          k=0x0220;
          break;
        case 0x01a1:
        case 0x01a3:
        case 0x01a5:
        case 0x01a8:
        case 0x01ad:
        case 0x01b0:
        case 0x01b4:
        case 0x01b6:
        case 0x01b9:
        case 0x01bd:
        case 0x01c5:
        case 0x01c8:
        case 0x01cb:
        case 0x01f2:
        case 0x01f5:
          k=c - 1;
          break;
        case 0x01bf:
          k=0x01f7;
          break;
        case 0x01c6:
        case 0x01c9:
        case 0x01cc:
          k=c - 2;
          break;
        case 0x01dd:
          k=0x018e;
          break;
        case 0x01f3:
          k=0x01f1;
          break;
        case 0x0253:
          k=0x0181;
          break;
        case 0x0254:
          k=0x0186;
          break;
        case 0x0256:
          k=0x0189;
          break;
        case 0x0257:
          k=0x018a;
          break;
        case 0x0259:
          k=0x018f;
          break;
        case 0x025b:
          k=0x0190;
          break;
        case 0x0260:
          k=0x0193;
          break;
        case 0x0263:
          k=0x0194;
          break;
        case 0x0268:
          k=0x0197;
          break;
        case 0x0269:
          k=0x0196;
          break;
        case 0x026f:
          k=0x019c;
          break;
        case 0x0272:
          k=0x019d;
          break;
        case 0x0275:
          k=0x019f;
          break;
        case 0x0280:
          k=0x01a6;
          break;
        case 0x0283:
          k=0x01a9;
          break;
        case 0x0288:
          k=0x01ae;
          break;
        case 0x028a:
          k=0x01b1;
          break;
        case 0x028b:
          k=0x01b2;
          break;
        case 0x0292:
          k=0x01b7;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else if (c < 0x0400)
  {
    if (c == 0x03ac)
      return 0x0386;

    if ((c & 0xfff0) == 0x03a0 && c >= 0x03ad)
      return (c - 0x15);

    if (c >= 0x03b1 && c <= 0x03cb && c != 0x03c2)
      return (c - 0x20);

    if (c == 0x03c2)
      return 0x03a3;

    if (c >= 0x03cc && c <= 0x03f5)
    {
      wchar_t k;

      switch (c)
      {
        case 0x03cc:
          k=0x038c;
          break;
        case 0x03cd:
        case 0x03ce:
          k=c - 0x3f;
          break;
        case 0x03d0:
          k=0x0392;
          break;
        case 0x03d1:
          k=0x0398;
          break;
        case 0x03d5:
          k=0x03a6;
          break;
        case 0x03d6:
          k=0x03a0;
          break;
        case 0x03d9:
        case 0x03db:
        case 0x03dd:
        case 0x03df:
        case 0x03e1:
        case 0x03e3:
        case 0x03e5:
        case 0x03e7:
        case 0x03e9:
        case 0x03eb:
        case 0x03ed:
        case 0x03ef:
          k=c - 1;
          break;
        case 0x03f0:
          k=0x039a;
          break;
        case 0x03f1:
          k=0x03a1;
          break;
        case 0x03f2:
          k=0x03a3;
          break;
        case 0x03f5:
          k=0x0395;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else if (c < 0x500)
  {
    if (c >= 0x0450 && c <= 0x045f)
      return (c - 0x50);

    if (c >= 0x0430 && c <= 0x044f)
      return (c - 0x20);

    if ((c >= 0x0461 && c <= 0x0481) ||
        (c >= 0x048b && c <= 0x04bf) ||
        (c >= 0x04d1 && c <= 0x04f5))
    {
      if (c & 0x01)
        return (c - 1);
      return c;
    }

    if (c >= 0x04c2 && c <= 0x04ce)
    {
      if (!(c & 0x01))
        return (c - 1);
      return c;
    }

    if (c == 0x04f9)
      return 0x04f8;
  }
  else if (c < 0x1f00)
  {
    if ((c >= 0x0501 && c <= 0x050f) ||
        (c >= 0x1e01 && c <= 0x1e95) ||
        (c >= 0x1ea1 && c <= 0x1ef9))
    {
      if (c & 0x01)
        return (c - 1);
      return c;
    }

    if (c >= 0x0561 && c <= 0x0586)
      return (c - 0x30);

    if (c == 0x1e9b)
      return 0x1e60;
  }
  else if (c < 0x2000)
  {
    if ((c >= 0x1f00 && c <= 0x1f07) ||
        (c >= 0x1f10 && c <= 0x1f15) ||
        (c >= 0x1f20 && c <= 0x1f27) ||
        (c >= 0x1f30 && c <= 0x1f37) ||
        (c >= 0x1f40 && c <= 0x1f45) ||
        (c >= 0x1f60 && c <= 0x1f67) ||
        (c >= 0x1f80 && c <= 0x1f87) ||
        (c >= 0x1f90 && c <= 0x1f97) ||
        (c >= 0x1fa0 && c <= 0x1fa7))
      return (c + 0x08);

    if (c >= 0x1f51 && c <= 0x1f57 && (c & 0x01))
      return (c + 0x08);

    if (c >= 0x1f70 && c <= 0x1ff3)
    {
      wchar_t k;

      switch (c)
      {
        case 0x1fb0:
          k=0x1fb8;
          break;
        case 0x1fb1:
          k=0x1fb9;
          break;
        case 0x1f70:
          k=0x1fba;
          break;
        case 0x1f71:
          k=0x1fbb;
          break;
        case 0x1fb3:
          k=0x1fbc;
          break;
        case 0x1fbe:
          k=0x0399;
          break;
        case 0x1f72:
          k=0x1fc8;
          break;
        case 0x1f73:
          k=0x1fc9;
          break;
        case 0x1f74:
          k=0x1fca;
          break;
        case 0x1f75:
          k=0x1fcb;
          break;
        case 0x1fd0:
          k=0x1fd8;
          break;
        case 0x1fd1:
          k=0x1fd9;
          break;
        case 0x1f76:
          k=0x1fda;
          break;
        case 0x1f77:
          k=0x1fdb;
          break;
        case 0x1fe0:
          k=0x1fe8;
          break;
        case 0x1fe1:
          k=0x1fe9;
          break;
        case 0x1f7a:
          k=0x1fea;
          break;
        case 0x1f7b:
          k=0x1feb;
          break;
        case 0x1fe5:
          k=0x1fec;
          break;
        case 0x1f78:
          k=0x1ff8;
          break;
        case 0x1f79:
          k=0x1ff9;
          break;
        case 0x1f7c:
          k=0x1ffa;
          break;
        case 0x1f7d:
          k=0x1ffb;
          break;
        case 0x1ff3:
          k=0x1ffc;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else
  {
    if (c >= 0x2170 && c <= 0x217f)
      return (c - 0x10);

    if (c >= 0x24d0 && c <= 0x24e9)
      return (c - 0x1a);

    if (c >= 0xff41 && c <= 0xff5a)
      return (c - 0x20);

//    if (c >= 0x10428 && c <= 0x1044d)
//      return (c - 0x28);
  }

  if (c < 0x00ff)
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
  else
    return c;
}

int AE_WideStrCmp(const wchar_t *wpString, const wchar_t *wpString2)
{
  while (*wpString && *wpString == *wpString2)
  {
    ++wpString;
    ++wpString2;
  }
  if (*wpString == *wpString2) return 0;
  if ((DWORD)*wpString < (DWORD)*wpString2) return -1;
  return 1;
}

int AE_WideStrCmpI(const wchar_t *wpString, const wchar_t *wpString2)
{
  while (*wpString && AE_WideCharUpper(*wpString) == AE_WideCharUpper(*wpString2))
  {
    ++wpString;
    ++wpString2;
  }
  if (*wpString == *wpString2) return 0;
  if ((DWORD)AE_WideCharUpper(*wpString) < (DWORD)AE_WideCharUpper(*wpString2)) return -1;
  return 1;
}

wchar_t* AE_wcschr(const wchar_t *s, wchar_t c)
{
  if (c == L'\r' || c == L'\n')
  {
    while (*s != L'\r' && *s != L'\n' && *s != L'\0') ++s;

    if (*s == L'\r' || *s == L'\n')
      return ((wchar_t *)s);
    return NULL;
  }
  else
  {
    while (*s != c && *s != L'\0') ++s;

    if (*s == c)
      return ((wchar_t *)s);
    return NULL;
  }
}

void* AE_memcpy(void *dest, const void *src, unsigned int count)
{
  unsigned char *byte_dest=(unsigned char *)dest;
  unsigned char *byte_src=(unsigned char *)src;
  
  if (byte_dest != byte_src)
    while (count--) *byte_dest++=*byte_src++;
  
  return dest;
}

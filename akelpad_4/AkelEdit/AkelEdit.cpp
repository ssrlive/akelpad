/***********************************************************************************
 *                      AkelEdit text control v1.2                                 *
 *                                                                                 *
 * Copyright 2007-2008 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 ***********************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "AkelBuild.h"
#include "Resources\resource.h"


//// Global variables

HANDLE hAkelEditProcessHeap=0;
HSTACK hAkelEditWindowsStack={0};
BOOL bAkelEditClassRegisteredA=FALSE;
BOOL bAkelEditClassRegisteredW=FALSE;
UINT cfAkelEditColumnSel=0;
HCURSOR hAkelEditCursorArrow=NULL;
HCURSOR hAkelEditCursorMargin=NULL;
HCURSOR hAkelEditCursorHand=NULL;


//// Entry point

#ifndef STATIC_BUILD
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    OleInitialize(0);
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
    OleUninitialize();
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

    //AkelEdit class
    wndclass.style        =CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditProc;
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
    wndclass.lpszClassName=AES_RICHEDITCLASSA;
    RegisterClassA(&wndclass);

    if (!hAkelEditProcessHeap) hAkelEditProcessHeap=GetProcessHeap();
    if (!cfAkelEditColumnSel) cfAkelEditColumnSel=RegisterClipboardFormatA("MSDEVColumnSelect");
    if (!hAkelEditCursorArrow) hAkelEditCursorArrow=LoadCursorA(NULL, (char *)IDC_ARROW);
    if (!hAkelEditCursorMargin) hAkelEditCursorMargin=LoadCursorA(hInstance, (char *)IDC_AEMARGIN);
    if (!hAkelEditCursorHand) hAkelEditCursorHand=LoadCursorA(hInstance, (char *)IDC_AEHAND);
  }
  return bAkelEditClassRegisteredA;
}

BOOL AE_RegisterClassW(HINSTANCE hInstance)
{
  if (!bAkelEditClassRegisteredW)
  {
    WNDCLASSW wndclass={0};

    //AkelEdit class
    wndclass.style        =CS_GLOBALCLASS|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wndclass.lpfnWndProc  =AE_EditProc;
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
    wndclass.lpszClassName=AES_RICHEDITCLASSW;
    RegisterClassW(&wndclass);

    if (!hAkelEditProcessHeap) hAkelEditProcessHeap=GetProcessHeap();
    if (!cfAkelEditColumnSel) cfAkelEditColumnSel=RegisterClipboardFormatW(L"MSDEVColumnSelect");
    if (!hAkelEditCursorArrow) hAkelEditCursorArrow=LoadCursorW(NULL, (wchar_t *)IDC_ARROW);
    if (!hAkelEditCursorMargin) hAkelEditCursorMargin=LoadCursorW(hInstance, (wchar_t *)IDC_AEMARGIN);
    if (!hAkelEditCursorHand) hAkelEditCursorHand=LoadCursorW(hInstance, (wchar_t *)IDC_AEHAND);
  }
  return bAkelEditClassRegisteredW;
}

BOOL AE_UnregisterClassA(HINSTANCE hInstance)
{
  if (hAkelEditCursorMargin) DestroyCursor(hAkelEditCursorMargin);
  if (hAkelEditCursorHand) DestroyCursor(hAkelEditCursorHand);

  if (bAkelEditClassRegisteredA)
  {
    if (UnregisterClassA(AES_AKELEDITCLASSA, hInstance))
      bAkelEditClassRegisteredA=FALSE;
    UnregisterClassA(AES_RICHEDITCLASSA, hInstance);
  }
  return !bAkelEditClassRegisteredA;
}

BOOL AE_UnregisterClassW(HINSTANCE hInstance)
{
  if (hAkelEditCursorMargin) DestroyCursor(hAkelEditCursorMargin);
  if (hAkelEditCursorHand) DestroyCursor(hAkelEditCursorHand);

  if (bAkelEditClassRegisteredW)
  {
    if (UnregisterClassW(AES_AKELEDITCLASSW, hInstance))
      bAkelEditClassRegisteredW=FALSE;
    UnregisterClassW(AES_RICHEDITCLASSW, hInstance);
  }
  return !bAkelEditClassRegisteredW;
}

LRESULT CALLBACK AE_EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  AKELEDIT *ae;

  if (uMsg == WM_CREATE)
  {
    CREATESTRUCTA *cs=(CREATESTRUCTA *)lParam;

    if (ae=AE_StackWindowInsert(&hAkelEditWindowsStack))
    {
      ae->bUnicodeWindow=IsWindowUnicode(hWnd);
      ae->hWndEdit=hWnd;
      ae->hWndParent=GetParent(ae->hWndEdit);
      ae->nEditCtrlID=GetWindowLongA(ae->hWndEdit, GWL_ID);
      ae->hHeap=NULL;
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
      ae->crUrlText=RGB(0x00, 0x00, 0xFF);
      ae->hBasicBk=CreateSolidBrush(ae->crBasicBk);
      ae->hSelBk=CreateSolidBrush(ae->crSelBk);
      ae->hActiveLineBk=CreateSolidBrush(ae->crActiveLineBk);
      ae->nTabStop=8;
      ae->bSavePointExist=TRUE;
      ae->dwUndoLimit=(DWORD)-1;
      ae->bVScrollShow=TRUE;
      ae->bHScrollShow=TRUE;
      ae->dwWordBreak=AEWB_LEFTWORDSTART|AEWB_LEFTWORDEND|AEWB_RIGHTWORDSTART|AEWB_RIGHTWORDEND|AEWB_SKIPSPACESTART|AEWB_STOPSPACEEND;
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
        if (!lstrcmpiA((char *)cs->lpszClass, AES_RICHEDITCLASSA))
          ae->bRichEditClass=TRUE;
      }
      else
      {
        if (!lstrcmpiW((wchar_t *)cs->lpszClass, AES_RICHEDITCLASSW))
          ae->bRichEditClass=TRUE;
      }

      if (!ae->bRichEditClass)
      {
        ae->nInputNewLine=AELB_ASIS;
        ae->nOutputNewLine=AELB_ASIS;
      }
      else
      {
        ae->nInputNewLine=AELB_R;
        ae->nOutputNewLine=AELB_RN;
      }

      if (cs->style & ES_READONLY)
        ae->dwOptions|=AECO_READONLY;
      if (cs->style & ES_DISABLENOSCROLL)
        ae->dwOptions|=AECO_DISABLENOSCROLL;
      if (cs->style & ES_NOHIDESEL)
        ae->dwOptions|=AECO_NOHIDESEL;
      if (cs->style & ES_WANTRETURN)
        ae->dwOptions|=AECO_WANTRETURN;
      AE_memcpy(ae->wszWordDelimiters, AES_WORDDELIMITERSW, sizeof(AES_WORDDELIMITERSW));
      AE_memcpy(ae->wszWrapDelimiters, AES_WRAPDELIMITERSW, sizeof(AES_WRAPDELIMITERSW));
      AE_memcpy(ae->wszUrlDelimiters, AES_URLDELIMITERSW, sizeof(AES_URLDELIMITERSW));
      AE_memcpy(ae->wszUrlPrefixes, AES_URLPREFIXESW, sizeof(AES_URLPREFIXESW));
      AE_GetUrlPrefixes(ae);

      GetClientRect(ae->hWndEdit, &ae->rcEdit);
      AE_SetDrawRect(ae, NULL, FALSE);

      if (!ae->bUnicodeWindow)
        AE_SetEditFontA(ae, NULL, FALSE);
      else
        AE_SetEditFontW(ae, NULL, FALSE);

      AE_SetText(ae, L"", 0, ae->nInputNewLine);

      //Register drop window
      CoLockObjectExternal((LPUNKNOWN)&ae->idt, TRUE, FALSE);
      RegisterDragDrop(ae->hWndEdit, (LPDROPTARGET)&ae->idt);

      //Scrollbars updated in WM_SIZE
      return 0;
    }
    return -1;
  }

  if (ae=AE_StackWindowGet(&hAkelEditWindowsStack, hWnd))
  {
    if (uMsg >= WM_USER)
    {
      //// AEM_* AkelEdit control messages

      //Text retrieval and modification
      if (uMsg == AEM_SETTEXTA)
      {
        return AE_SetTextAnsi(ae, CP_ACP, (char *)lParam, wParam, ae->nInputNewLine);
      }
      if (uMsg == AEM_SETTEXTW)
      {
        return AE_SetText(ae, (wchar_t *)lParam, wParam, ae->nInputNewLine);
      }
      if (uMsg == AEM_APPENDTEXTA)
      {
        AEAPPENDTEXTA *at=(AEAPPENDTEXTA *)lParam;

        AE_AppendTextAnsi(ae, CP_ACP, at->pText, at->dwTextLen, at->bColumnSel);
        return 0;
      }
      if (uMsg == AEM_APPENDTEXTW)
      {
        AEAPPENDTEXTW *at=(AEAPPENDTEXTW *)lParam;

        AE_AppendText(ae, at->wpText, at->dwTextLen, at->bColumnSel);
        return 0;
      }
      if (uMsg == AEM_REPLACESELA)
      {
        AEREPLACESELA *rs=(AEREPLACESELA *)lParam;

        AE_ReplaceSelAnsi(ae, CP_ACP, rs->pText, rs->dwTextLen, rs->bColumnSel, rs->ciInsertStart, rs->ciInsertEnd);
        return 0;
      }
      if (uMsg == AEM_REPLACESELW)
      {
        AEREPLACESELW *rs=(AEREPLACESELW *)lParam;

        AE_ReplaceSel(ae, rs->wpText, rs->dwTextLen, rs->bColumnSel, rs->ciInsertStart, rs->ciInsertEnd);
        return 0;
      }
      if (uMsg == AEM_GETTEXTRANGEA)
      {
        AETEXTRANGEA *tr=(AETEXTRANGEA *)lParam;

        return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &tr->cr.ciMin, &tr->cr.ciMax, tr->pBuffer, tr->dwBufferMax, tr->nNewLine, tr->bColumnSel, FALSE);
      }
      if (uMsg == AEM_GETTEXTRANGEW)
      {
        AETEXTRANGEW *tr=(AETEXTRANGEW *)lParam;

        return AE_GetTextRange(ae, &tr->cr.ciMin, &tr->cr.ciMax, tr->wpBuffer, tr->dwBufferMax, tr->nNewLine, tr->bColumnSel, FALSE);
      }
      if (uMsg == AEM_STREAMIN)
      {
        AESTREAM *aes=(AESTREAM *)lParam;

        return AE_StreamIn(ae, wParam, aes);
      }
      if (uMsg == AEM_STREAMOUT)
      {
        AESTREAM *aes=(AESTREAM *)lParam;

        return AE_StreamOut(ae, wParam, aes);
      }
      if (uMsg == AEM_CANPASTE)
      {
        return AE_EditCanPaste(ae);
      }
      if (uMsg == AEM_PASTE)
      {
        AE_EditPasteFromClipboard(ae, lParam);
        return 0;
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
      if (uMsg == AEM_FINDTEXTA)
      {
        return AE_FindTextAnsi(ae, CP_ACP, (AEFINDTEXTA *)lParam);
      }
      if (uMsg == AEM_FINDTEXTW)
      {
        return AE_FindText(ae, (AEFINDTEXTW *)lParam);
      }
      if (uMsg == AEM_CHECKCODEPAGE)
      {
        return AE_CheckCodepage(ae, wParam);
      }

      //Undo and Redo
      if (uMsg == AEM_CANUNDO)
      {
        return AE_EditCanUndo(ae);
      }
      if (uMsg == AEM_CANREDO)
      {
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
      if (uMsg == AEM_BEGINUNDOACTION)
      {
        AE_StackUndoGroupStop(ae);
        ae->bLockGroupStop=TRUE;
        return 0;
      }
      if (uMsg == AEM_ENDUNDOACTION)
      {
        ae->bLockGroupStop=FALSE;
        AE_StackUndoGroupStop(ae);
        return 0;
      }
      if (uMsg == AEM_LOCKCOLLECTUNDO)
      {
        ae->bLockCollectUndo=wParam;
        return 0;
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
      if (uMsg == AEM_GETMODIFY)
      {
        return ae->bModified;
      }
      if (uMsg == AEM_SETMODIFY)
      {
        AE_SetModify(ae, wParam, TRUE);
        return 0;
      }

      //Text coordinates
      if (uMsg == AEM_GETSEL)
      {
        AECHARINDEX *lpciCaret=(AECHARINDEX *)wParam;
        AESELECTION *aes=(AESELECTION *)lParam;

        AE_AkelEditGetSel(ae, aes, lpciCaret);
        return 0;
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
        AE_UpdateSelection(ae, 0);
        return 0;
      }
      if (uMsg == AEM_GETLINECOUNT)
      {
        return ae->nLineCount + 1;
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

        if (lpLine=AE_GetLineData(ae, nLine))
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
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        return (LRESULT)AE_StackPointInsert(ae, ciCharIndex);
      }
      if (uMsg == AEM_DELPOINT)
      {
        AEPOINT *lpPoint=(AEPOINT *)wParam;

        AE_StackPointDelete(ae, lpPoint);
        return 0;
      }

      //Screen coordinates
      if (uMsg == AEM_CHARFROMPOS)
      {
        POINT *pt=(POINT *)wParam;
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        return AE_GetCharFromPos(ae, pt, ciCharIndex, NULL, FALSE);
      }
      if (uMsg == AEM_POSFROMCHAR)
      {
        POINT *pt=(POINT *)wParam;
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        AE_GetPosFromCharEx(ae, ciCharIndex, NULL, pt);
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

        AE_SetDrawRect(ae, rcDraw, TRUE);
        if (ae->bWordWrap) AE_UpdateWrap(ae, ae->bWordWrap);
        AE_UpdateScrollBars(ae, SB_BOTH);
        return 0;
      }
      if (uMsg == AEM_GETSCROLLPOS)
      {
        POINT *pt=(POINT *)lParam;

        pt->x=ae->nHScrollPos;
        pt->y=ae->nVScrollPos;
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

        if (wParam == SB_HORZ)
        {
          nResult=AE_HScroll(ae, lParam) / ae->nAveCharWidth;
        }
        else if (wParam == SB_VERT)
        {
          nResult=AE_VScroll(ae, lParam) / ae->nCharHeight;
        }
        return nResult;
      }
      if (uMsg == AEM_LINESCROLL)
      {
        int nResult=0;

        if (wParam == SB_HORZ)
        {
          nResult=AE_HScrollLine(ae, lParam) / ae->nAveCharWidth;
        }
        else if (wParam == SB_VERT)
        {
          nResult=AE_VScrollLine(ae, lParam) / ae->nCharHeight;
        }
        return nResult;
      }
      if (uMsg == AEM_SCROLLCARETTEST)
      {
        return AE_ScrollToCaretEx(ae, &ae->ptCaret, wParam, LOWORD(lParam), HIWORD(lParam), TRUE);
      }
      if (uMsg == AEM_SCROLLCARET)
      {
        return AE_ScrollToCaretEx(ae, &ae->ptCaret, wParam, LOWORD(lParam), HIWORD(lParam), FALSE);
      }
      if (uMsg == AEM_LOCKSCROLL)
      {
        if (wParam == SB_BOTH || wParam == SB_HORZ)
          ae->bHScrollLock=lParam;
        if (wParam == SB_BOTH || wParam == SB_VERT)
          ae->bVScrollLock=lParam;
        return 0;
      }

      //Options
      if (uMsg == AEM_GETEVENTMASK)
      {
        return ae->dwEventMask;
      }
      if (uMsg == AEM_SETEVENTMASK)
      {
        DWORD dwPrevMask=ae->dwEventMask;

        ae->dwEventMask=lParam;
        return dwPrevMask;
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
        return ae->bDetectUrl;
      }
      if (uMsg == AEM_SETDETECTURL)
      {
        ae->bDetectUrl=wParam;
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        return 0;
      }
      if (uMsg == AEM_GETOVERTYPE)
      {
        return ae->bOverType;
      }
      if (uMsg == AEM_SETOVERTYPE)
      {
        if (ae->bOverType != (int)wParam)
        {
          ae->bOverType=wParam;
          AE_UpdateCaret(ae, ae->bFocus, FALSE);
        }
        return 0;
      }
      if (uMsg == AEM_GETCARETWIDTH)
      {
        POINT *pt=(POINT *)lParam;

        pt->x=ae->nCaretInsertWidth;
        pt->y=ae->nCaretOvertypeHeight;
        return 0;
      }
      if (uMsg == AEM_SETCARETWIDTH)
      {
        POINT *pt=(POINT *)lParam;

        ae->nCaretInsertWidth=pt->x;
        ae->nCaretOvertypeHeight=pt->y;
        AE_UpdateCaret(ae, ae->bFocus, TRUE);
        return 0;
      }
      if (uMsg == AEM_GETTABSTOP)
      {
        return ae->nTabStop;
      }
      if (uMsg == AEM_SETTABSTOP)
      {
        if (ae->nTabStop != (int)wParam)
        {
          ae->nTabStop=wParam;
          ae->nTabWidth=ae->nAveCharWidth * ae->nTabStop;

          AE_CalcLinesWidth(ae, NULL, NULL, TRUE);
          ae->ptCaret.x=0;
          ae->ptCaret.y=0;
          AE_UpdateSelection(ae, 0);
          AE_UpdateCaret(ae, ae->bFocus, TRUE);

          if (ae->bWordWrap) AE_UpdateWrap(ae, ae->bWordWrap);
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, lParam);
        }
        return 0;
      }
      if (uMsg == AEM_GETWORDWRAP)
      {
        return ae->bWordWrap;
      }
      if (uMsg == AEM_SETWORDWRAP)
      {
        if (ae->bWordWrap != (int)wParam)
        {
          ae->bWordWrap=wParam;
          AE_UpdateWrap(ae, ae->bWordWrap);
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, lParam);
        }
        return 0;
      }
      if (uMsg == AEM_GETWORDDELIMITERS)
      {
        if (wParam) AE_memcpy((wchar_t *)wParam, ae->wszWordDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->wszWordDelimiters)));
        return ae->dwWordBreak;
      }
      if (uMsg == AEM_SETWORDDELIMITERS)
      {
        if (lParam)
          AE_memcpy(ae->wszWordDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
        else
          AE_memcpy(ae->wszWordDelimiters, AES_WORDDELIMITERSW, sizeof(AES_WORDDELIMITERSW));
        ae->dwWordBreak=wParam;
        return 0;
      }
      if (uMsg == AEM_GETWRAPDELIMITERS)
      {
        if (wParam) AE_memcpy((wchar_t *)wParam, ae->wszWrapDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->wszWrapDelimiters)));
        return 0;
      }
      if (uMsg == AEM_SETWRAPDELIMITERS)
      {
        if (lParam)
          AE_memcpy(ae->wszWrapDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
        else
          AE_memcpy(ae->wszWrapDelimiters, AES_WRAPDELIMITERSW, sizeof(AES_WRAPDELIMITERSW));
        if (ae->bWordWrap) AE_UpdateWrap(ae, ae->bWordWrap);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        return 0;
      }
      if (uMsg == AEM_GETURLDELIMITERS)
      {
        if (wParam) AE_memcpy((wchar_t *)wParam, ae->wszUrlDelimiters, min(lParam * sizeof(wchar_t), sizeof(ae->wszUrlDelimiters)));
        return 0;
      }
      if (uMsg == AEM_SETURLDELIMITERS)
      {
        if (lParam)
          AE_memcpy(ae->wszUrlDelimiters, (wchar_t *)lParam, (lstrlenW((wchar_t *)lParam) + 1) * sizeof(wchar_t));
        else
          AE_memcpy(ae->wszUrlDelimiters, AES_URLDELIMITERSW, sizeof(AES_URLDELIMITERSW));
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        return 0;
      }
      if (uMsg == AEM_GETURLPREFIXES)
      {
        if (wParam) AE_memcpy((wchar_t *)wParam, ae->wszUrlPrefixes, min(lParam * sizeof(wchar_t), sizeof(ae->wszUrlPrefixes)));
        return 0;
      }
      if (uMsg == AEM_SETURLPREFIXES)
      {
        int nPrefix;

        if (lParam)
          AE_memcpy(ae->wszUrlPrefixes, (wchar_t *)lParam, sizeof(ae->wszUrlPrefixes));
        else
          AE_memcpy(ae->wszUrlPrefixes, AES_URLPREFIXESW, sizeof(AES_URLPREFIXESW));
        nPrefix=AE_GetUrlPrefixes(ae);
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
        return nPrefix;
      }
      if (uMsg == AEM_ISDELIMITER)
      {
        AECHARINDEX *ciCharIndex=(AECHARINDEX *)lParam;

        if (wParam == AEDLM_WORD)
          return AE_IsInDelimiterList(ae->wszWordDelimiters, ciCharIndex->lpLine->wpLine[ciCharIndex->nCharInLine]);
        if (wParam == AEDLM_WRAP)
          return AE_IsInDelimiterList(ae->wszWrapDelimiters, ciCharIndex->lpLine->wpLine[ciCharIndex->nCharInLine]);
        if (wParam == AEDLM_URL)
          return AE_IsInDelimiterList(ae->wszUrlDelimiters, ciCharIndex->lpLine->wpLine[ciCharIndex->nCharInLine]);
        return -1;
      }
      if (uMsg == AEM_SHOWSCROLLBAR)
      {
        if (wParam == SB_BOTH || wParam == SB_HORZ)
          ae->bHScrollShow=lParam;
        if (wParam == SB_BOTH || wParam == SB_VERT)
          ae->bVScrollShow=lParam;
        ShowScrollBar(ae->hWndEdit, wParam, lParam);
        return 0;
      }
      if (uMsg == AEM_UPDATESCROLLBAR)
      {
        AE_UpdateScrollBars(ae, wParam);
        return 0;
      }
      if (uMsg == AEM_HIDESELECTION)
      {
        AE_HideSelection(ae, wParam);
        return 0;
      }
    }


    //// EM_* rich edit messages

    if (uMsg == EM_STREAMIN ||
        uMsg == EM_STREAMOUT)
    {
      EDITSTREAM *es=(EDITSTREAM *)lParam;
      AESTREAM aes;
      DWORD dwFlags=0;
      DWORD dwResult=0;

      if ((wParam & SF_TEXT) && (wParam & SF_UNICODE))
      {
        aes.dwCookie=es->dwCookie;
        aes.lpCallback=(AEStreamCallback)es->pfnCallback;
        if (uMsg == EM_STREAMIN)
          aes.nNewLine=AELB_ASINPUT;
        else
          aes.nNewLine=AELB_ASOUTPUT;
        aes.bColumnSel=FALSE;

        if (wParam & SFF_SELECTION)
          dwFlags|=AESF_SELECTION;

        if (uMsg == EM_STREAMIN)
          dwResult=AE_StreamIn(ae, dwFlags, &aes);
        else
          dwResult=AE_StreamOut(ae, dwFlags, &aes);

        es->dwError=aes.dwError;
      }
      return dwResult;
    }
    if (uMsg == EM_REPLACESEL)
    {
      if (!ae->bUnicodeWindow)
        AE_ReplaceSelAnsi(ae, CP_ACP, (char *)lParam, (DWORD)-1, FALSE, NULL, NULL);
      else
        AE_ReplaceSel(ae, (wchar_t *)lParam, (DWORD)-1, FALSE, NULL, NULL);
      return 0;
    }
    if (uMsg == EM_GETTEXTRANGE)
    {
      if (!ae->bUnicodeWindow)
      {
        TEXTRANGEA *trRE=(TEXTRANGEA *)lParam;
        AECHARRANGE cr;

        if ((DWORD)trRE->chrg.cpMin < (DWORD)trRE->chrg.cpMax)
        {
          AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMin, &cr.ciMin);
          AE_RichOffsetToAkelIndex(ae, trRE->chrg.cpMax, &cr.ciMax);
          return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, trRE->lpstrText, (DWORD)-1, AELB_R, FALSE, FALSE);
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
      }
      return 0;
    }
    if (uMsg == EM_GETSELTEXT)
    {
      if (!ae->bUnicodeWindow)
        return AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (char *)lParam, (DWORD)-1, AELB_R, FALSE, FALSE);
      else
        return AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)lParam, (DWORD)-1, AELB_R, FALSE, FALSE);
    }
    if (uMsg == EM_GETLINE)
    {
      if (!ae->bUnicodeWindow)
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
          AE_memcpy(wszBuffer, lpLine->wpLine, nResult * sizeof(wchar_t));
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
        dwResult=ae->nLastCharOffset;
        if (gtl->flags & GTL_USECRLF)
          dwResult+=ae->nLineCount;
      }
      else
      {
        if (gtl->codepage == 1200 || gtl->codepage == 1201)
        {
          dwResult=ae->nLastCharOffset;
          if (gtl->flags & GTL_USECRLF)
            dwResult+=ae->nLineCount;
          dwResult*=sizeof(wchar_t);
        }
        else
        {
          AELINEDATA *lpLine=(AELINEDATA *)ae->hLinesStack.first;

          while (lpLine)
          {
            if (lpLine->nLineLen)
            {
              dwResult+=WideCharToMultiByte(gtl->codepage, 0, lpLine->wpLine, lpLine->nLineLen, NULL, 0, NULL, NULL);
            }
            lpLine=lpLine->next;
          }
          dwResult+=ae->nLineCount;
          if (gtl->flags & GTL_USECRLF)
            dwResult+=ae->nLineCount;
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
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin, ae->bColumnSel);
        AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax, ae->bColumnSel);
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
        AE_ChangeByteOrder((unsigned char *)lParam, dwResult * sizeof(wchar_t));
      }
      else dwResult=AE_GetTextRangeAnsi(ae, gt->codepage, gt->lpDefaultChar, gt->lpUsedDefChar, &cr.ciMin, &cr.ciMax, (char *)lParam, gt->cb, nNewLine, FALSE, FALSE);

      return dwResult;
    }
    if (uMsg == EM_SETTEXTEX)
    {
      SETTEXTEX *st=(SETTEXTEX *)wParam;
      BOOL bLockCollectUndo=ae->bLockCollectUndo;

      if (!(st->flags & ST_KEEPUNDO))
      {
        AE_EmptyUndoBuffer(ae);
        ae->bLockCollectUndo=TRUE;
      }
      if (!(st->flags & ST_SELECTION))
        AE_EditSelectAll(ae);

      if (st->codepage == 1200)
      {
        AE_ReplaceSel(ae, (wchar_t *)lParam, (DWORD)-1, FALSE, NULL, NULL);
      }
      else if (st->codepage == 1201)
      {
        wchar_t *wszText;
        DWORD dwUnicodeBytes;

        dwUnicodeBytes=lstrlenW((wchar_t *)lParam) * sizeof(wchar_t);

        if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, dwUnicodeBytes + 2))
        {
          AE_memcpy(wszText, (wchar_t *)lParam, dwUnicodeBytes + 2);
          AE_ChangeByteOrder((unsigned char *)wszText, dwUnicodeBytes);
          AE_ReplaceSel(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), FALSE, NULL, NULL);
          AE_HeapFree(NULL, 0, (LPVOID)wszText);
        }
      }
      else AE_ReplaceSelAnsi(ae, st->codepage, (char *)lParam, (DWORD)-1, FALSE, NULL, NULL);

      if (!(st->flags & ST_KEEPUNDO))
        ae->bLockCollectUndo=bLockCollectUndo;
      if (!(st->flags & ST_SELECTION))
        AE_SetModify(ae, FALSE, TRUE);
      return 1;
    }
    if (uMsg == EM_CANPASTE)
    {
      return AE_EditCanPaste(ae);
    }
    if (uMsg == EM_CANUNDO)
    {
      return AE_EditCanUndo(ae);
    }
    if (uMsg == EM_CANREDO)
    {
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
    if (uMsg == EM_EMPTYUNDOBUFFER)
    {
      if (!ae->lpSavePoint)
      {
        ae->lpSavePoint=NULL;
        ae->bSavePointExist=FALSE;
      }
      AE_StackRedoDeleteAll(ae, NULL);
      ae->lpCurrentUndo=NULL;
      return 0;
    }
    if (uMsg == EM_STOPGROUPTYPING)
    {
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    if (uMsg == EM_GETSEL)
    {
      AE_RichEditGetSel(ae, (LONG *)wParam, (LONG *)lParam);
      return 0;
    }
    if (uMsg == EM_EXGETSEL)
    {
      CHARRANGE *crRE=(CHARRANGE *)lParam;

      AE_RichEditGetSel(ae, &crRE->cpMin, &crRE->cpMax);
      return 0;
    }
    if (uMsg == EM_SETSEL)
    {
      AE_RichEditSetSel(ae, wParam, lParam, FALSE);
      return 0;
    }
    if (uMsg == EM_EXSETSEL)
    {
      CHARRANGE *crRE=(CHARRANGE *)lParam;

      AE_RichEditSetSel(ae, crRE->cpMin, crRE->cpMax, FALSE);
      return 0;
    }
    if (uMsg == EM_GETLINECOUNT)
    {
      return ae->nLineCount + 1;
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
        if (wParam <= (DWORD)ae->nLineCount)
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

      if (wParam <= (DWORD)ae->nLastCharOffset)
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

      if (lParam || !IsBadCodePtr((FARPROC)wParam))
      {
        //RichEdit 3.0 syntax
        POINT *pt=(POINT *)wParam;

        AE_RichOffsetToAkelIndex(ae, lParam, &ci);
        AE_GetPosFromCharEx(ae, &ci, NULL, pt);
        return 0;
      }
      else
      {
        //RichEdit 2.0 syntax
        POINT pt;

        AE_RichOffsetToAkelIndex(ae, wParam, &ci);
        AE_GetPosFromCharEx(ae, &ci, NULL, &pt);
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
        nScrolled=AE_VScroll(ae, wParam);
        bResult=TRUE;
      }
      return MAKELONG(mod(nScrolled) / ae->nCharHeight, bResult);
    }
    if (uMsg == EM_LINESCROLL)
    {
      AE_VScrollLine(ae, lParam);
      return 0;
    }
    if (uMsg == EM_SCROLLCARET)
    {
      AE_ScrollToCaret(ae, &ae->ptCaret);
      return 0;
    }
    if (uMsg == EM_GETTHUMB)
    {
      return ae->nVScrollPos;
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
          ft.nNewLine=AELB_R;
          AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMin, &ft.crSearch.ciMin);
          AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMax, &ft.crSearch.ciMax);
          ft.pText=(char *)ftRE->lpstrText;
          ft.dwTextLen=(DWORD)-1;

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
        ft.nNewLine=AELB_R;
        AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMin, &ft.crSearch.ciMin);
        AE_RichOffsetToAkelIndex(ae, ftRE->chrg.cpMax, &ft.crSearch.ciMax);
        ft.wpText=(wchar_t *)ftRE->lpstrText;
        ft.dwTextLen=(DWORD)-1;

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
    if (uMsg == EM_FINDWORDBREAK)
    {
      AECHARINDEX ciCharIn;
      AECHARINDEX ciCharOut;
      DWORD dwFlags=0;

      if (wParam == WB_CLASSIFY)
        return 0;

      AE_RichOffsetToAkelIndex(ae, lParam, &ciCharIn);

      if (wParam == WB_ISDELIMITER)
        return AE_IsInDelimiterList(ae->wszWordDelimiters, ciCharIn.lpLine->wpLine[ciCharIn.nCharInLine]);

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
          dwFlags=ae->dwWordBreak;

        if (AE_GetPrevWord(ae, &ciCharIn, &ciCharOut, NULL, FALSE, dwFlags, FALSE))
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
          dwFlags=ae->dwWordBreak;

        if (AE_GetNextWord(ae, &ciCharIn, NULL, &ciCharOut, FALSE, dwFlags, FALSE))
          return AE_AkelIndexToRichOffset(ae, &ciCharOut);
        else
          return lParam;
      }
      return 0;
    }
    if (uMsg == EM_SETUNDOLIMIT)
    {
      ae->dwUndoLimit=wParam;
      AE_StackUndoGroupStop(ae);
      return 0;
    }
    if (uMsg == EM_GETEVENTMASK)
    {
      return ae->dwRichEventMask;
    }
    if (uMsg == EM_SETEVENTMASK)
    {
      DWORD dwPrevMask=ae->dwRichEventMask;

      ae->dwRichEventMask=lParam;
      return dwPrevMask;
    }
    if (uMsg == EM_GETOPTIONS)
    {
      DWORD dwOptions=0;

      if (ae->dwOptions & AECO_READONLY)
        dwOptions|=ECO_READONLY;
      if (ae->dwOptions & AECO_NOHIDESEL)
        dwOptions|=ECO_NOHIDESEL;
      if (ae->dwOptions & AECO_WANTRETURN)
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
        ae->dwOptions=dwOptions;
      else if (wParam == AECOOP_OR)
        ae->dwOptions|=dwOptions;
      else if (wParam == AECOOP_AND)
        ae->dwOptions&=dwOptions;
      else if (wParam == AECOOP_XOR)
        ae->dwOptions&=~dwOptions;
      return ae->dwOptions;
    }
    if (uMsg == EM_SETREADONLY)
    {
      if (wParam)
        ae->dwOptions|=AECO_READONLY;
      else
        ae->dwOptions&=~AECO_READONLY;
      return 1;
    }
    if (uMsg == EM_GETMODIFY)
    {
      return ae->bModified;
    }
    if (uMsg == EM_SETMODIFY)
    {
      AE_SetModify(ae, wParam, TRUE);
      return 0;
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
      if (ae->bWordWrap) AE_UpdateWrap(ae, ae->bWordWrap);
      AE_UpdateScrollBars(ae, SB_BOTH);
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
      AE_UpdateScrollBars(ae, SB_BOTH);
      return 0;
    }
    if (uMsg == EM_SHOWSCROLLBAR)
    {
      if (wParam == SB_BOTH || wParam == SB_HORZ)
        ae->bHScrollShow=lParam;
      if (wParam == SB_BOTH || wParam == SB_VERT)
        ae->bVScrollShow=lParam;
      ShowScrollBar(ae->hWndEdit, wParam, lParam);
      return 0;
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
    if (uMsg == EM_HIDESELECTION)
    {
      if (wParam)
        ae->dwOptions&=~AECO_NOHIDESEL;
      else
        ae->dwOptions|=AECO_NOHIDESEL;
      AE_HideSelection(ae, wParam);
      return 0;
    }
    if (uMsg == EM_GETAUTOURLDETECT)
    {
      return ae->bDetectUrl;
    }
    if (uMsg == EM_AUTOURLDETECT)
    {
      ae->bDetectUrl=wParam;
      InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
      return 0;
    }


    //// WM_* system messages

    if (uMsg == WM_GETFONT)
    {
      return (LRESULT)ae->hFont;
    }
    else if (uMsg == WM_SETFONT)
    {
      if (!ae->bUnicodeWindow)
        AE_SetEditFontA(ae, (HFONT)wParam, FALSE);
      else
        AE_SetEditFontW(ae, (HFONT)wParam, FALSE);

      ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
      AE_UpdateScrollBars(ae, SB_VERT);
      AE_CalcLinesWidth(ae, NULL, NULL, TRUE);
      ae->ptCaret.x=0;
      ae->ptCaret.y=0;
      AE_UpdateSelection(ae, 0);
      AE_UpdateCaret(ae, ae->bFocus, TRUE);

      if (ae->bWordWrap) AE_UpdateWrap(ae, ae->bWordWrap);
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
      return AE_IndexSubtract(ae, NULL, NULL, ae->nOutputNewLine, FALSE, FALSE);
    }
    else if (uMsg == WM_GETTEXT)
    {
      AECHARRANGE cr;
      DWORD dwResult;

      AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &cr.ciMin, FALSE);
      AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &cr.ciMax, FALSE);

      if (!ae->bUnicodeWindow)
        dwResult=AE_GetTextRangeAnsi(ae, CP_ACP, NULL, NULL, &cr.ciMin, &cr.ciMax, (char *)lParam, wParam, ae->nOutputNewLine, FALSE, FALSE);
      else
        dwResult=AE_GetTextRange(ae, &cr.ciMin, &cr.ciMax, (wchar_t *)lParam, wParam, ae->nOutputNewLine, FALSE, FALSE);
      return dwResult;
    }
    else if (uMsg == WM_SETTEXT)
    {
      if (!ae->bUnicodeWindow)
        AE_SetTextAnsi(ae, CP_ACP, (char *)lParam, (DWORD)-1, ae->nInputNewLine);
      else
        AE_SetText(ae, (wchar_t *)lParam, (DWORD)-1, ae->nInputNewLine);
      return TRUE;
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
        if (ae->bWordWrap) AE_UpdateWrap(ae, ae->bWordWrap);
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
    else if (uMsg == WM_KEYDOWN ||
             uMsg == WM_SYSKEYDOWN)
    {
      AECHARINDEX ciCharIn=ae->ciCaretIndex;
      AECHARINDEX ciCharOut=ae->ciCaretIndex;
      int nHorizCaretPos=ae->nHorizCaretPos;
      BOOL bAlt=FALSE;
      BOOL bShift=FALSE;
      BOOL bControl=FALSE;
      BOOL bRedrawAllSelection=FALSE;

      if (ae->dwRichEventMask & ENM_KEYEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      if (GetKeyState(VK_MENU) < 0)
        bAlt=TRUE;
      if (GetKeyState(VK_SHIFT) < 0)
        bShift=TRUE;
      if (GetKeyState(VK_CONTROL) < 0)
        bControl=TRUE;

      if (wParam == VK_TAB)
      {
        if (!bAlt)
        {
          AE_EditChar(ae, VK_TAB);
        }
        return 0;
      }
      if (wParam == VK_RETURN)
      {
        if (!bAlt)
        {
          AE_EditKeyReturn(ae);
        }
        return 0;
      }
      if (wParam == VK_BACK)
      {
        if (!bAlt)
        {
          AE_EditKeyBackspace(ae, bControl);
        }
        else if (!bControl && !bShift)
        {
          AE_EditUndo(ae);
        }
        return 0;
      }
      if (wParam == VK_DELETE)
      {
        if (!bAlt)
        {
          if (!bShift)
          {
            AE_EditKeyDelete(ae, bControl);
          }
          else if (!bControl)
          {
            AE_EditCut(ae);
          }
        }
        return 0;
      }
      if (wParam == VK_INSERT)
      {
        if (!bAlt)
        {
          if (!bControl && !bShift)
          {
            ae->bOverType=!ae->bOverType;
            AE_UpdateCaret(ae, ae->bFocus, FALSE);
          }
          else
          {
            if (bControl && !bShift)
            {
              AE_EditCopyToClipboard(ae);
            }
            else if (!bControl && bShift)
            {
              AE_EditPasteFromClipboard(ae, FALSE);
            }
            else if (bControl && bShift)
            {
              AE_EditPasteFromClipboard(ae, TRUE);
            }
          }
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
        else if (bControl && bShift && !bAlt)
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
        else if (bControl && bShift && !bAlt)
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

            AE_GetPrevWord(ae, &ciCharIn, &ciCharOut, NULL, ae->bColumnSel, ae->dwWordBreak, FALSE);
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

            AE_GetNextWord(ae, &ciCharIn, NULL, &ciCharOut, ae->bColumnSel, ae->dwWordBreak, FALSE);
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
              nHorizCaretPos=ae->nHScrollPos;
            else
              AE_GetCharInLineEx(ae, ciCharOut.lpLine, nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
          }
        }
        else if (wParam == VK_DOWN)
        {
          if (!bShift && AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
            ciCharIn=ae->ciSelEndIndex;

          if (AE_GetIndex(ae, AEGI_NEXTLINE, &ciCharIn, &ciCharOut, ae->bColumnSel))
          {
            if (bControl)
              nHorizCaretPos=ae->nHScrollPos;
            else
              AE_GetCharInLineEx(ae, ciCharOut.lpLine, nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
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
              AE_GetCharInLineEx(ae, ciCharOut.lpLine, nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
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
              AE_GetCharInLineEx(ae, ciCharOut.lpLine, nHorizCaretPos, TRUE, &ciCharOut.nCharInLine, NULL, ae->bColumnSel);
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
            AE_SetSelectionPos(ae, &ciCharOut, &ciSelEnd, ae->bColumnSel, 0);
          }
          else AE_SetSelectionPos(ae, &ciCharOut, &ciCharOut, ae->bColumnSel, 0);
        }

        //Set horizontal caret
        if (wParam == VK_UP ||
            wParam == VK_DOWN ||
            wParam == VK_PRIOR ||
            wParam == VK_NEXT)
        {
          ae->nHorizCaretPos=nHorizCaretPos;
        }

        //Redraw lines
        if (bRedrawAllSelection)
        {
          AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
        }
        return 0;
      }
    }
    else if (uMsg == WM_CHAR)
    {
      if (ae->dwRichEventMask & ENM_KEYEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      if (wParam == VK_TAB || (wParam >= 0x20 && wParam != 0x7F))
      {
        AE_EditChar(ae, wParam);
        return 0;
      }
      return 0;
    }
    else if (uMsg == WM_DEADCHAR ||
             uMsg == WM_SYSCHAR ||
             uMsg == WM_SYSDEADCHAR ||
             uMsg == WM_KEYUP ||
             uMsg == WM_SYSKEYUP)
    {
      if (ae->dwRichEventMask & ENM_KEYEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
    }
    else if (uMsg == WM_HSCROLL)
    {
      if (ae->dwRichEventMask & ENM_SCROLLEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      AE_HScroll(ae, LOWORD(wParam));
      return 0;
    }
    else if (uMsg == WM_VSCROLL)
    {
      if (ae->dwRichEventMask & ENM_SCROLLEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      AE_VScroll(ae, LOWORD(wParam));
      return 0;
    }
    else if (uMsg == WM_LBUTTONDOWN ||
             uMsg == WM_LBUTTONDBLCLK)
    {
      POINT ptPos;
      int nLButtonDownCurTime=GetMessageTime();
      BOOL bAlt=FALSE;
      BOOL bShift=FALSE;
      BOOL bControl=FALSE;
      BOOL bRedrawAllSelection=FALSE;

      if (ae->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
      if (ae->nCurrentCursor == AECC_URL)
        if (AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink))
          return 0;

      ptPos.x=LOWORD(lParam);
      ptPos.y=HIWORD(lParam);

      if (GetKeyState(VK_MENU) < 0)
        bAlt=TRUE;
      if (wParam & MK_SHIFT)
        bShift=TRUE;
      if (wParam & MK_CONTROL)
        bControl=TRUE;

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
        //Start margin selection capture
        if (!ae->bMarginSelect && !bAlt && !bShift && ae->nCurrentCursor == AECC_MARGIN)
        {
          AECHARRANGE cr;
          AECHARINDEX ciCharIndex;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            SetCapture(ae->hWndEdit);
          }
          ae->bMarginSelect=TRUE;

          AE_GetCharFromPos(ae, &ptPos, &ciCharIndex, NULL, ae->bColumnSel);
          cr.ciMin=ciCharIndex;
          cr.ciMax=ciCharIndex;
          cr.ciMin.nCharInLine=0;
          if (!AE_GetIndex(ae, AEGI_NEXTLINE, &cr.ciMax, &cr.ciMax, FALSE))
            cr.ciMax.nCharInLine=cr.ciMax.lpLine->nLineLen;

          ae->ciLButtonClick=ciCharIndex;
          ae->ciLButtonStart=cr.ciMin;
          ae->ciLButtonEnd=cr.ciMax;
          AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, FALSE, 0);
        }
        //Start drag source capture
        else if (!ae->bDragging && !bAlt && !bShift && ae->nCurrentCursor == AECC_SELECTION)
        {
          SetCapture(ae->hWndEdit);
          ae->bDragging=TRUE;
          ae->nMoveBeforeDragging=5;
        }
        //Start selection change capture
        else if (!ae->dwMouseMoveTimer)
        {
          if (ae->bColumnSel != bAlt)
            bRedrawAllSelection=TRUE;
          ae->bColumnSel=bAlt;

          if (GetFocus() != ae->hWndEdit)
            SetFocus(ae->hWndEdit);

          //Timer
          ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
          SetCapture(ae->hWndEdit);

          AE_SetMouseSelection(ae, &ptPos, ae->bColumnSel, bShift);

          //Redraw lines
          if (bRedrawAllSelection)
          {
            AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
          }
        }
      }
      //Two clicks
      else if (ae->nLButtonDownCount == 1)
      {
        if (!ae->bMarginSelect && !bAlt && !bShift && ae->nCurrentCursor == AECC_MARGIN)
        {
          //Two clicks in left margin are ignored
        }
        else
        {
          AECHARINDEX ciPrevWord;
          AECHARINDEX ciNextWord;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            SetCapture(ae->hWndEdit);
          }

          if (!bControl)
          {
            if (!AE_GetPrevBreak(ae, &ae->ciCaretIndex, &ciPrevWord, ae->bColumnSel, 0))
              ciPrevWord=ae->ciCaretIndex;
            if (!AE_GetNextBreak(ae, &ciPrevWord, &ciNextWord, ae->bColumnSel, 0))
              ciNextWord=ae->ciCaretIndex;
          }
          else
          {
            if (!AE_GetPrevWord(ae, &ae->ciCaretIndex, &ciPrevWord, NULL, ae->bColumnSel, AEWB_LEFTWORDSTART|AEWB_RIGHTWORDSTART, FALSE))
              ciPrevWord=ae->ciCaretIndex;
            if (!AE_GetNextWord(ae, &ciPrevWord, NULL, &ciNextWord, ae->bColumnSel, AEWB_LEFTWORDSTART|AEWB_RIGHTWORDSTART, FALSE))
              ciNextWord=ae->ciCaretIndex;
          }
          ae->ciLButtonClick=ae->ciCaretIndex;
          ae->ciLButtonStart=ciPrevWord;
          ae->ciLButtonEnd=ciNextWord;
          AE_SetSelectionPos(ae, &ciNextWord, &ciPrevWord, ae->bColumnSel, 0);
        }
      }
      //Three clicks
      else if (ae->nLButtonDownCount == 2)
      {
        if (!ae->bMarginSelect && !bAlt && !bShift && ae->nCurrentCursor == AECC_MARGIN)
        {
          AE_EditSelectAll(ae);
        }
        else
        {
          AECHARRANGE cr;

          if (!ae->dwMouseMoveTimer)
          {
            //Timer
            ae->dwMouseMoveTimer=SetTimer(ae->hWndEdit, AETIMERID_MOUSEMOVE, 100, NULL);
            SetCapture(ae->hWndEdit);
          }

          cr.ciMin=ae->ciCaretIndex;
          cr.ciMax=ae->ciCaretIndex;
          cr.ciMin.nCharInLine=0;
          if (!AE_GetIndex(ae, AEGI_NEXTLINE, &cr.ciMax, &cr.ciMax, FALSE))
            cr.ciMax.nCharInLine=cr.ciMax.lpLine->nLineLen;

          ae->ciLButtonClick=ae->ciCaretIndex;
          ae->ciLButtonStart=cr.ciMin;
          ae->ciLButtonEnd=cr.ciMax;
          AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, FALSE, 0);
        }
      }
      return 0;
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
      POINT ptPos;

      if (ae->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
      if (ae->nCurrentCursor == AECC_URL)
        if (AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink))
          return 0;

      ptPos.x=LOWORD(lParam);
      ptPos.y=HIWORD(lParam);

      if (ae->bDragging)
      {
        if (ae->nMoveBeforeDragging > 0)
          --ae->nMoveBeforeDragging;

        if (ae->nMoveBeforeDragging == 0)
        {
          DWORD dwEffectIn;
          DWORD dwEffectOut=0;
          DWORD dwResult=0;

          dwEffectIn=DROPEFFECT_COPY;
          if (!(ae->dwOptions & AECO_READONLY))
            dwEffectIn|=DROPEFFECT_MOVE;

          if (!AE_NotifyDropSource(ae, AEDS_SOURCEBEGIN, &dwEffectIn, 0))
          {
            ae->bDeleteSelection=TRUE;
            AE_DataObjectCopySelection(ae);

            dwResult=DoDragDrop((IDataObject *)&ae->ido, (IDropSource *)&ae->ids, dwEffectIn, &dwEffectOut);

            if (!AE_NotifyDropSource(ae, AEDS_SOURCEEND, &dwEffectOut, dwResult))
            {
              if (dwResult == DRAGDROP_S_DROP)
              {
                if (dwEffectOut & DROPEFFECT_MOVE)
                {
                  if (ae->bDeleteSelection)
                  {
                    AE_StackUndoGroupStop(ae);
                    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
                    AE_StackUndoGroupStop(ae);
                  }
                }
              }
            }
            AE_DataObjectFreeSelection(ae);
            ((IDataObject *)&ae->ido)->Release();
            ((IDropSource *)&ae->ids)->Release();
            ae->bDeleteSelection=FALSE;
          }
          ae->bDragging=FALSE;
          ReleaseCapture();
          AE_NotifyDragDropDone(ae);
        }
      }
      else if (ae->dwMouseMoveTimer)
      {
        POINT ptScreen;

        GetCursorPos(&ptScreen);
        ScreenToClient(ae->hWndEdit, &ptScreen);
        AE_SetMouseSelection(ae, &ptScreen, ae->bColumnSel, TRUE);
      }
      return 0;
    }
    else if (uMsg == WM_LBUTTONUP)
    {
      POINT ptPos;

      ptPos.x=LOWORD(lParam);
      ptPos.y=HIWORD(lParam);

      if (ae->bDragging)
      {
        ae->bDragging=FALSE;
        ReleaseCapture();

        if (GetFocus() != ae->hWndEdit)
          SetFocus(ae->hWndEdit);
        AE_SetMouseSelection(ae, &ptPos, ae->bColumnSel, FALSE);
      }
      if (ae->dwMouseMoveTimer)
      {
        KillTimer(ae->hWndEdit, ae->dwMouseMoveTimer);
        ae->dwMouseMoveTimer=0;
        ReleaseCapture();
      }
      ae->bMarginSelect=FALSE;

      if (ae->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
      if (ae->nCurrentCursor == AECC_URL)
        if (AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink))
          return 0;
      return 0;
    }
    else if (uMsg == WM_RBUTTONDOWN ||
             uMsg == WM_RBUTTONUP ||
             uMsg == WM_RBUTTONDBLCLK ||
             uMsg == WM_MBUTTONDOWN ||
             uMsg == WM_MBUTTONUP ||
             uMsg == WM_MBUTTONDBLCLK)
    {
      if (ae->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;
      if (ae->nCurrentCursor == AECC_URL)
        if (AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink))
          return 0;
    }
    else if (uMsg == WM_MOUSEWHEEL)
    {
      DWORD dwLines;

      if (ae->dwRichEventMask & ENM_MOUSEEVENTS)
        if (AE_NotifyMsgFilter(ae, uMsg, &wParam, &lParam))
          return 0;

      SystemParametersInfoA(SPI_GETWHEELSCROLLLINES, 0, &dwLines, 0);

      if ((short)HIWORD(wParam) < 0)
      {
        if (dwLines == (DWORD)-1)
          AE_VScroll(ae, SB_PAGEDOWN);
        else
          AE_VScrollLine(ae, dwLines);
      }
      else
      {
        if (dwLines == (DWORD)-1)
          AE_VScroll(ae, SB_PAGEUP);
        else
          AE_VScrollLine(ae, -(int)dwLines);
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
      ae->bMarginSelect=FALSE;
    }
    else if (uMsg == WM_SETCURSOR)
    {
      if (!ae->bDragging && !ae->dwMouseMoveTimer)
      {
        POINT ptScreen;
        BOOL bAlt=FALSE;
        BOOL bShift=FALSE;

        if (GetKeyState(VK_MENU) < 0)
          bAlt=TRUE;
        if (GetKeyState(VK_SHIFT) < 0)
          bShift=TRUE;

        GetCursorPos(&ptScreen);
        ScreenToClient(ae->hWndEdit, &ptScreen);

        if (!bAlt && !bShift && AE_IsCursorOnLeftMargin(ae, &ptScreen))
        {
          SetCursor(hAkelEditCursorMargin);
          ae->nCurrentCursor=AECC_MARGIN;
          return TRUE;
        }
        if (!bAlt && !bShift && AE_IsCursorOnSelection(ae, &ptScreen))
        {
          SetCursor(hAkelEditCursorArrow);
          ae->nCurrentCursor=AECC_SELECTION;
          return TRUE;
        }
        if (!bAlt && !bShift && AE_IsCursorOnUrl(ae, &ptScreen, &ae->crMouseOnLink))
        {
          SetCursor(hAkelEditCursorHand);
          ae->nCurrentCursor=AECC_URL;

          AE_NotifyLink(ae, uMsg, wParam, lParam, &ae->crMouseOnLink);
          return TRUE;
        }
        ae->nCurrentCursor=AECC_IBEAM;
      }
    }
    else if (uMsg == WM_TIMER)
    {
      if (wParam == AETIMERID_MOUSEMOVE)
      {
        POINT ptScreen;

        GetCursorPos(&ptScreen);
        ScreenToClient(ae->hWndEdit, &ptScreen);
        AE_SetMouseSelection(ae, &ptScreen, ae->bColumnSel, TRUE);
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
      AE_UpdateCaret(ae, ae->bFocus, FALSE);

      if (!(ae->dwOptions & AECO_NOHIDESEL))
      {
        AE_HideSelection(ae, FALSE);
      }
      AE_NotifySetFocus(ae);
      return 0;
    }
    else if (uMsg == WM_KILLFOCUS)
    {
      if (!(ae->dwOptions & AECO_NOHIDESEL))
      {
        AE_HideSelection(ae, TRUE);
      }
      ae->bFocus=FALSE;
      DestroyCaret();
      AE_StackUndoGroupStop(ae);
      AE_NotifyKillFocus(ae);
      return 0;
    }
    else if (uMsg == WM_SYSCOLORCHANGE)
    {
    }
    else if (uMsg == WM_ENABLE)
    {
      if (wParam)
      {
        AECOLORS aec;

        aec.dwFlags=AECLR_BASICBK|AECLR_SELBK|AECLR_ACTIVELINEBK;
        aec.crBasicBk=ae->crEnableBasicBk;
        aec.crSelBk=ae->crEnableSelBk;
        aec.crActiveLineBk=ae->crEnableActiveLineBk;
        AE_SetColors(ae, &aec);
      }
      else
      {
        AECOLORS aec;

        aec.dwFlags=AECLR_BASICBK|AECLR_SELBK|AECLR_ACTIVELINEBK;
        AE_GetColors(ae, &aec);
        ae->crEnableBasicBk=aec.crBasicBk;
        ae->crEnableSelBk=aec.crSelBk;
        ae->crEnableActiveLineBk=aec.crActiveLineBk;

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

      if (ae->rcErase.left < ae->rcDraw.left)
      {
        rcErase=ae->rcErase;
        rcErase.right=min(rcErase.right, ae->rcDraw.left);
        FillRect((HDC)wParam, &rcErase, ae->hBasicBk);
        ae->rcErase.left=rcErase.right;
      }
      if (ae->rcErase.top < ae->rcDraw.top)
      {
        rcErase=ae->rcErase;
        rcErase.bottom=min(rcErase.bottom, ae->rcDraw.top);
        FillRect((HDC)wParam, &rcErase, ae->hBasicBk);
        ae->rcErase.top=rcErase.bottom;
      }
      if (ae->rcErase.right > ae->rcDraw.right)
      {
        rcErase=ae->rcErase;
        rcErase.left=max(rcErase.left, ae->rcDraw.right);
        FillRect((HDC)wParam, &rcErase, ae->hBasicBk);
        ae->rcErase.right=rcErase.left;
      }
      if (ae->rcErase.bottom > ae->rcDraw.bottom)
      {
        rcErase=ae->rcErase;
        rcErase.top=max(rcErase.top, ae->rcDraw.bottom);
        FillRect((HDC)wParam, &rcErase, ae->hBasicBk);
        ae->rcErase.bottom=rcErase.top;
      }

      //Erase only a space after the last line
      rcErase=ae->rcErase;
      rcErase.top=max(rcErase.top, ae->nLineCount * ae->nCharHeight - ae->nVScrollPos);

      if (rcErase.top < rcErase.bottom)
      {
        FillRect((HDC)wParam, &rcErase, ae->hBasicBk);
      }
      return 1;
    }
    else if (uMsg == WM_PAINT)
    {
      AE_Paint(ae);
      return 0;
    }
    else if (uMsg == WM_DESTROY)
    {
      //Unregister drop window
      RevokeDragDrop(ae->hWndEdit);
      CoLockObjectExternal((LPUNKNOWN)&ae->idt, FALSE, TRUE);
      ((IDropTarget *)&ae->idt)->Release();

      if (ae->hDC)
      {
        if (ae->hFontUrl)
        {
          DeleteObject(ae->hFontUrl);
          ae->hFontUrl=NULL;
        }
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
      AE_StackWindowDelete(&hAkelEditWindowsStack, hWnd);
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
    hHeap=hAkelEditProcessHeap;
  else
    hHeap=ae->hHeap;

  if (!(lpResult=HeapAlloc(hHeap, dwFlags, dwBytes)))
  {
    if (ae) AE_NotifyErrSpace(ae);
  }
  return lpResult;
}

BOOL AE_HeapFree(AKELEDIT *ae, DWORD dwFlags, LPVOID lpMem)
{
  HANDLE hHeap;

  if (!ae)
    hHeap=hAkelEditProcessHeap;
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

        AE_StackPointFree(lpElement);
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

AEPOINT* AE_StackPointInsert(AKELEDIT *ae, AECHARINDEX *ciPoint)
{
  AEPOINT *lpElement=NULL;

  AE_HeapStackInsert(NULL, (stack **)&ae->hPointsStack.first, (stack **)&ae->hPointsStack.last, (stack **)&lpElement, 1, sizeof(AEPOINT));

  if (lpElement)
  {
    lpElement->ciPoint=*ciPoint;
  }
  return lpElement;
}

void AE_StackPointSetModify(AKELEDIT *ae, BOOL bModify)
{
  AEPOINT *lpElement=(AEPOINT *)ae->hPointsStack.last;

  while (lpElement)
  {
    lpElement->bModify=bModify;

    lpElement=lpElement->prev;
  }
}

void AE_StackPointDelete(AKELEDIT *ae, AEPOINT *lpElement)
{
  AE_HeapStackDelete(NULL, (stack **)&ae->hPointsStack.first, (stack **)&ae->hPointsStack.last, (stack *)lpElement);
}

void AE_StackPointFree(AKELEDIT *ae)
{
  AE_HeapStackClear(NULL, (stack **)&ae->hPointsStack.first, (stack **)&ae->hPointsStack.last);
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
          if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
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
            ae->lpCurrentUndo=lpElement;

            //First index
            if (!lpElement)
              lpElement=(AEUNDOITEM *)ae->hUndoStack.first;
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

              ae->lpCurrentUndo=lpUndoElement;
              lpStopElement=lpUndoElement;
            }
          }
        }
      }
      if (!ae->bLockGroupStop)
      {
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

void AE_StackLineDelete(AKELEDIT *ae, AELINEDATA *lpElement)
{
  if (lpElement == ae->liFirstDrawLine.lpLine)
  {
    ae->liFirstDrawLine.nLine=0;
    ae->liFirstDrawLine.lpLine=NULL;
    ae->nFirstDrawLineOffset=0;
  }
  if (lpElement == ae->liMaxWidthLine.lpLine)
  {
    ae->liMaxWidthLine.nLine=0;
    ae->liMaxWidthLine.lpLine=NULL;
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

AELINEDATA* AE_GetLineData(AKELEDIT *ae, int nLine)
{
  AELINEDATA *lpElementData=NULL;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  int nElementLine=0;

  if (nLine < 0) nLine=ae->nLineCount + nLine + 1;
  if (nLine < 0 || nLine > ae->nLineCount) return NULL;

  //Find nearest element in stack
  dwFirst=mod(nLine - 0);
  dwSecond=mod(nLine - ae->nLineCount);
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
    nElementLine=nLine - 0;
    lpElementData=(AELINEDATA *)ae->hLinesStack.first;
  }
  else if (dwSecond <= dwFirst && dwSecond <= dwThird && dwSecond <= dwFourth && dwSecond <= dwFifth && dwSecond <= dwSixth)
  {
    nElementLine=nLine - ae->nLineCount;
    lpElementData=(AELINEDATA *)ae->hLinesStack.last;
  }
  else if (dwThird <= dwFirst && dwThird <= dwSecond && dwThird <= dwFourth && dwThird <= dwFifth && dwThird <= dwSixth)
  {
    nElementLine=nLine - ae->liFirstDrawLine.nLine;
    lpElementData=ae->liFirstDrawLine.lpLine;
  }
  else if (dwFourth <= dwFirst && dwFourth <= dwSecond && dwFourth <= dwThird && dwFourth <= dwFifth && dwFourth <= dwSixth)
  {
    nElementLine=nLine - ae->ciSelStartIndex.nLine;
    lpElementData=ae->ciSelStartIndex.lpLine;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    nElementLine=nLine - ae->ciSelEndIndex.nLine;
    lpElementData=ae->ciSelEndIndex.lpLine;
  }
  else if (dwSixth <= dwFirst && dwSixth <= dwSecond && dwSixth <= dwThird && dwSixth <= dwFourth && dwSixth <= dwFifth)
  {
    nElementLine=nLine - ae->ciLastCallIndex.nLine;
    lpElementData=ae->ciLastCallIndex.lpLine;
  }

  if (nElementLine > 0)
  {
    while (lpElementData)
    {
      if (nElementLine-- == 0)
        break;
      lpElementData=lpElementData->next;
    }
  }
  else if (nElementLine < 0)
  {
    while (lpElementData)
    {
      if (nElementLine++ == 0)
        break;
      lpElementData=lpElementData->prev;
    }
  }
  return lpElementData;
}

void AE_RichOffsetToAkelIndex(AKELEDIT *ae, DWORD dwOffset, AECHARINDEX *ciCharIndex)
{
  AECHARINDEX ciElement={0};
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  int nElementLineOffset=0;

  dwOffset=min(dwOffset, (DWORD)ae->nLastCharOffset);

  //Find nearest element in stack
  dwFirst=mod(dwOffset - 0);
  dwSecond=mod(dwOffset - ae->nLastCharOffset);
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
    nElementLineOffset=dwOffset - ae->nLastCharOffset;
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
    AE_GetIndex(ae, AEGI_FIRSTSELCHAR, NULL, &ciElement, FALSE);
    ciElement.nCharInLine=min(ciElement.nCharInLine, ciElement.lpLine->nLineLen);
    nElementLineOffset=dwOffset - ae->nSelStartCharOffset;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTSELCHAR, NULL, &ciElement, FALSE);
    ciElement.nCharInLine=min(ciElement.nCharInLine, ciElement.lpLine->nLineLen);
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
  AECHARINDEX ciElement={0};
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  DWORD dwFourth=(DWORD)-1;
  DWORD dwFifth=(DWORD)-1;
  DWORD dwSixth=(DWORD)-1;
  int nElementLineOffset=0;
  int nCompare;
  int nSubtract;
  int nResult;

  //Normalize
  ciChar.nCharInLine=min(ciChar.nCharInLine, ciChar.lpLine->nLineLen);

  if (ciChar.nLine < 0)
    return 0;
  if (ciChar.nLine > ae->nLineCount)
    return ae->nLastCharOffset;

  //Find nearest element in stack
  dwFirst=mod(ciChar.nLine - 0);
  dwSecond=mod(ciChar.nLine - ae->nLineCount);
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
    nElementLineOffset=ae->nLastCharOffset;
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
    AE_GetIndex(ae, AEGI_FIRSTSELCHAR, NULL, &ciElement, FALSE);
    ciElement.nCharInLine=min(ciElement.nCharInLine, ciElement.lpLine->nLineLen);
    nElementLineOffset=ae->nSelStartCharOffset;
  }
  else if (dwFifth <= dwFirst && dwFifth <= dwSecond && dwFifth <= dwThird && dwFifth <= dwFourth && dwFifth <= dwSixth)
  {
    AE_GetIndex(ae, AEGI_LASTSELCHAR, NULL, &ciElement, FALSE);
    ciElement.nCharInLine=min(ciElement.nCharInLine, ciElement.lpLine->nLineLen);
    nElementLineOffset=ae->nSelEndCharOffset;
  }
  else if (dwSixth <= dwFirst && dwSixth <= dwSecond && dwSixth <= dwThird && dwSixth <= dwFourth && dwSixth <= dwFifth)
  {
    ciElement=ae->ciLastCallIndex;
    nElementLineOffset=ae->nLastCallOffset;
  }

  nCompare=AE_IndexCompare(&ciElement, &ciChar);

  if (nCompare)
  {
    nSubtract=AE_IndexSubtract(ae, &ciElement, &ciChar, AELB_R, FALSE, FALSE);
    if (nCompare > 0)
      nResult=nElementLineOffset - nSubtract;
    else
      nResult=nElementLineOffset + nSubtract;
  }
  else return nElementLineOffset;

  ae->ciLastCallIndex=ciChar;
  ae->nLastCallOffset=nResult;
  return nResult;
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
    ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
    ciCharOut->nCharInLine=0;
    return ciCharOut->lpLine?TRUE:FALSE;
  }
  else if (nType == AEGI_LASTVISIBLELINE)
  {
    ciCharOut->nLine=AE_GetLastVisibleLine(ae);
    ciCharOut->lpLine=AE_GetLineData(ae, ciCharOut->nLine);
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
          if (ciCharTmp.lpLine->nLineBreak == AELB_WRAP)
            ciCharOut->nCharInLine=min(ciCharOut->nCharInLine + 1, ciCharOut->lpLine->nLineLen);
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
          if (ciCharOut->lpLine->nLineBreak == AELB_WRAP)
            ciCharOut->nCharInLine=max(ciCharOut->nCharInLine - 1, 0);
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
    return AE_GetNextBreak(ae, ciCharIn, ciCharOut, bColumnSel, ae->dwWordBreak);
  }
  else if (nType == AEGI_PREVBREAK)
  {
    return AE_GetPrevBreak(ae, ciCharIn, ciCharOut, bColumnSel, ae->dwWordBreak);
  }
  else if (nType == AEGI_NEXTWORDSTART)
  {
    return AE_GetNextWord(ae, ciCharIn, NULL, ciCharOut, bColumnSel, AEWB_RIGHTWORDSTART, FALSE);
  }
  else if (nType == AEGI_NEXTWORDEND)
  {
    return AE_GetNextWord(ae, ciCharIn, NULL, ciCharOut, bColumnSel, AEWB_RIGHTWORDEND, FALSE);
  }
  else if (nType == AEGI_PREVWORDSTART)
  {
    return AE_GetPrevWord(ae, ciCharIn, ciCharOut, NULL, bColumnSel, AEWB_LEFTWORDSTART, FALSE);
  }
  else if (nType == AEGI_PREVWORDEND)
  {
    return AE_GetPrevWord(ae, ciCharIn, ciCharOut, NULL, bColumnSel, AEWB_LEFTWORDEND, FALSE);
  }
  else if (nType == AEGI_WRAPLINEBEGIN)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    if (ciCharTmp.lpLine)
    {
      while (ciCharTmp.lpLine->prev)
      {
        if (ciCharTmp.lpLine->prev->nLineBreak != AELB_WRAP)
          break;

        --ciCharTmp.nLine;
        ciCharTmp.lpLine=ciCharTmp.lpLine->prev;
      }
    }
    ciCharOut->nLine=ciCharTmp.nLine;
    ciCharOut->lpLine=ciCharTmp.lpLine;
    ciCharOut->nCharInLine=0;
    return TRUE;
  }
  else if (nType == AEGI_WRAPLINEEND)
  {
    AECHARINDEX ciCharTmp=*ciCharIn;

    while (ciCharTmp.lpLine)
    {
      if (ciCharTmp.lpLine->nLineBreak != AELB_WRAP)
        break;

      ++ciCharTmp.nLine;
      ciCharTmp.lpLine=ciCharTmp.lpLine->next;
    }
    ciCharOut->nLine=ciCharTmp.nLine;
    ciCharOut->lpLine=ciCharTmp.lpLine;
    ciCharOut->nCharInLine=ciCharTmp.lpLine->nLineLen;
    return TRUE;
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

        if (lpElement->nLineBreak != AELB_WRAP)
        {
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

  if (nOffsetCount == 0)
  {
    if (ciCount.lpLine->nLineBreak == AELB_WRAP)
    {
      if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
      {
        if (ciCount.lpLine->next)
        {
          ciCount.nLine+=1;
          ciCount.lpLine=ciCount.lpLine->next;
          ciCount.nCharInLine=0;
        }
      }
    }
    *ciCharOut=ciCount;
    return TRUE;
  }
  else if (nOffsetCount > 0)
  {
    while (1)
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

BOOL AE_UpdateIndex(AKELEDIT *ae, AECHARINDEX *ciChar)
{
  if (ciChar->lpLine=AE_GetLineData(ae, ciChar->nLine))
    return TRUE;
  return FALSE;
}

int AE_UpdateWrap(AKELEDIT *ae, BOOL bWrap)
{
  AECHARINDEX ciSelStart=ae->ciSelStartIndex;
  AECHARINDEX ciSelEnd=ae->ciSelEndIndex;
  AECHARINDEX ciFirstVisibleLineAfterWrap;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  AEPOINT *lpPointThree;
  int nResult;

  AE_GetIndex(ae, AEGI_FIRSTVISIBLELINE, NULL, &ciFirstVisibleLineAfterWrap, FALSE);
  lpPointOne=AE_StackPointInsert(ae, &ciSelStart);
  lpPointTwo=AE_StackPointInsert(ae, &ciSelEnd);
  lpPointThree=AE_StackPointInsert(ae, &ciFirstVisibleLineAfterWrap);
  nResult=AE_WrapLines(ae, NULL, NULL, bWrap);
  ciSelStart=lpPointOne->ciPoint;
  ciSelEnd=lpPointTwo->ciPoint;
  ciFirstVisibleLineAfterWrap=lpPointThree->ciPoint;
  AE_StackPointDelete(ae, lpPointOne);
  AE_StackPointDelete(ae, lpPointTwo);
  AE_StackPointDelete(ae, lpPointThree);

  ae->ptCaret.x=0;
  ae->ptCaret.y=0;
  if (!AE_IndexCompare(&ae->ciSelEndIndex, &ae->ciCaretIndex))
    ae->ciCaretIndex=ciSelEnd;
  else
    ae->ciCaretIndex=ciSelStart;
  ae->ciSelStartIndex=ciSelStart;
  ae->ciSelEndIndex=ciSelEnd;

  if (!bWrap)
  {
    //Calculate max width line
    AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
  }
  else
  {
    if (ae->nHScrollMax)
    {
      ae->liMaxWidthLine.nLine=0;
      ae->liMaxWidthLine.lpLine=NULL;
      ae->nHScrollPos=0;
      ae->nHScrollMax=0;
      AE_UpdateScrollBars(ae, SB_HORZ);
    }
  }

  if (nResult)
  {
    ae->nLineCount+=nResult;
    ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
    AE_UpdateScrollBars(ae, SB_VERT);
  }
  AE_UpdateSelection(ae, AESELT_LOCKSCROLL);
  AE_VScrollLine(ae, ciFirstVisibleLineAfterWrap.nLine - AE_GetFirstVisibleLine(ae));
  return nResult;
}

int AE_WrapLines(AKELEDIT *ae, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, BOOL bWrap)
{
  AELINEINDEX liFirst;
  AELINEINDEX liCount;
  DWORD dwMaxWidth;
  int nLineCount=0;
  int nWrapped=0;
  int nUnwrapped=0;
  int nStopLine;
  int nProgressStep=1000;
  BOOL bPrevLine=FALSE;

  if (bWrap)
    dwMaxWidth=(ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth;
  else
    dwMaxWidth=(DWORD)-1;

  if (!liWrapStart)
  {
    liCount.nLine=0;
    liCount.lpLine=(AELINEDATA *)ae->hLinesStack.first;
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
    nStopLine=ae->nLineCount;
  }
  else
  {
    nStopLine=liWrapEnd->nLine;
  }

  //Start progress
  if (ae->dwEventMask & AENM_PROGRESS)
  {
    if (!liWrapStart && !liWrapEnd)
    {
      if (AE_NotifyProgress(ae, AEPGS_WRAPTEXT, &nProgressStep, liCount.nLine, nStopLine))
        return 0;
    }
  }

  while (liCount.lpLine)
  {
    if (bWrap)
    {
      if (liCount.lpLine->nLineWidth == -1)
        AE_GetLineWidth(ae, liCount.lpLine);

      if ((DWORD)liCount.lpLine->nLineWidth > dwMaxWidth)
      {
        if (nWrapped=AE_LineWrap(ae, &liCount, (liCount.nLine == liFirst.nLine)?&liFirst:NULL, &liCount, dwMaxWidth))
        {
          nLineCount+=nWrapped;
        }
        else break;
      }
    }
    if (liCount.lpLine->nLineBreak == AELB_WRAP)
    {
      if (nUnwrapped=AE_LineUnwrap(ae, &liCount, dwMaxWidth))
      {
        nLineCount+=nUnwrapped;

        if (liCount.nLine == liFirst.nLine)
          liFirst.lpLine=liCount.lpLine;
        if (bWrap) continue;
      }
    }

    if (liCount.nLine >= nStopLine + nLineCount)
      break;
    ++liCount.nLine;
    liCount.lpLine=liCount.lpLine->next;

    //Progressing
    if (ae->dwEventMask & AENM_PROGRESS)
    {
      if (!liWrapStart && !liWrapEnd)
      {
        if (!(liCount.nLine % nProgressStep))
        {
          if (AE_NotifyProgress(ae, AEPGS_WRAPTEXT, &nProgressStep, liCount.nLine - nLineCount, nStopLine))
            break;
        }
      }
    }
  }

  //End progress
  if (ae->dwEventMask & AENM_PROGRESS)
  {
    if (!liWrapStart && !liWrapEnd)
    {
      nProgressStep=0;
      AE_NotifyProgress(ae, AEPGS_WRAPTEXT, &nProgressStep, nStopLine, nStopLine);
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
  if (ae->liMaxWidthLine.lpLine)
  {
    if (ae->liMaxWidthLine.nLine > nStopLine)
    {
      ae->liMaxWidthLine.nLine+=nLineCount;
    }
    else if (ae->liMaxWidthLine.nLine >= liFirst.nLine)
    {
      ae->liMaxWidthLine.nLine=0;
      ae->liMaxWidthLine.lpLine=NULL;
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

int AE_LineWrap(AKELEDIT *ae, const AELINEINDEX *liLine, AELINEINDEX *liWrapStart, AELINEINDEX *liWrapEnd, DWORD dwMaxWidth)
{
  AELINEINDEX liStart=*liLine;
  AELINEINDEX liEnd=*liLine;
  AELINEDATA *lpInitialElement=liLine->lpLine;
  AELINEDATA *lpNewElement=NULL;
  AEPOINT *lpPoint;
  int nCharStart=0;
  int nCharEnd=0;
  int nCharPos=0;
  int nLineCount=0;
  int i;

  AE_StackPointSetModify(ae, FALSE);

  NextLine:
  if (nCharEnd < lpInitialElement->nLineLen)
  {
    if (AE_GetCharInLine(ae, lpInitialElement->wpLine + nCharStart, lpInitialElement->nLineLen - nCharStart, dwMaxWidth, FALSE, &nCharEnd, &nCharPos, FALSE))
    {
      nCharEnd+=nCharStart;

      //Find end of word
      if (nCharEnd < lpInitialElement->nLineLen)
      {
        for (i=nCharEnd - 1; i >= nCharStart; --i)
        {
          if (AE_IsInDelimiterList(ae->wszWrapDelimiters, lpInitialElement->wpLine[i]))
            break;
        }
        if (i >= nCharStart)
          nCharEnd=i + 1;
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

        if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
        {
          AE_memcpy(lpNewElement->wpLine, lpInitialElement->wpLine + nCharStart, lpNewElement->nLineLen * sizeof(wchar_t));
          lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
          AE_GetLineWidth(ae, lpNewElement);

          //Update points
          for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
          {
            if (!lpPoint->bModify)
            {
              if (lpPoint->ciPoint.lpLine == lpInitialElement)
              {
                if ((lpPoint->ciPoint.nCharInLine >= nCharStart && lpPoint->ciPoint.nCharInLine <= nCharEnd) || nCharEnd >= lpInitialElement->nLineLen)
                {
                  lpPoint->ciPoint.nLine+=nLineCount;
                  lpPoint->ciPoint.lpLine=lpNewElement;
                  lpPoint->ciPoint.nCharInLine-=nCharStart;
                  lpPoint->bModify=TRUE;
                }
              }
            }
          }

          nCharStart=nCharEnd;
          if (nCharEnd < lpInitialElement->nLineLen)
            ++nLineCount;
          goto NextLine;
        }
      }
    }
  }

  if (nLineCount)
  {
    AE_StackLineDelete(ae, lpInitialElement);

    liEnd.lpLine=lpNewElement;
    liEnd.nLine+=nLineCount;

    //Update points
    for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
    {
      if (!lpPoint->bModify)
      {
        if (lpPoint->ciPoint.nLine > liStart.nLine)
        {
          lpPoint->ciPoint.nLine+=nLineCount;
        }
      }
    }
  }

  if (liWrapStart) *liWrapStart=liStart;
  if (liWrapEnd) *liWrapEnd=liEnd;
  return nLineCount;
}

int AE_LineUnwrap(AKELEDIT *ae, AELINEINDEX *liLine, DWORD dwMaxWidth)
{
  AELINEDATA *lpNewElement;
  AELINEDATA *lpCurElement;
  AELINEDATA *lpNextElement;
  AEPOINT *lpPoint;
  DWORD dwUnwrapLineWidth=0;
  DWORD dwUnwrapLineBreak=AELB_EOF;
  DWORD dwUnwrapLineLen=0;
  DWORD dwCountWidth=0;
  DWORD dwCountLen=0;
  int nLineCount=0;

  AE_StackPointSetModify(ae, FALSE);

  if (liLine->lpLine->nLineWidth == -1)
    AE_GetLineWidth(ae, liLine->lpLine);

  if ((DWORD)(liLine->lpLine->nLineWidth + ae->nAveCharWidth) < dwMaxWidth)
  {
    //Calculate unwrapped line info
    lpCurElement=liLine->lpLine;

    while (lpCurElement)
    {
      if (lpCurElement->nLineWidth == -1)
        AE_GetLineWidth(ae, lpCurElement);
      dwUnwrapLineWidth+=lpCurElement->nLineWidth;
      dwUnwrapLineLen+=lpCurElement->nLineLen;

      if (dwUnwrapLineWidth + ae->nAveCharWidth >= dwMaxWidth || lpCurElement->nLineBreak != AELB_WRAP)
      {
        dwUnwrapLineBreak=lpCurElement->nLineBreak;
        break;
      }

      lpCurElement=lpCurElement->next;
    }

    //Unwrap line
    lpCurElement=liLine->lpLine;

    if (lpNewElement=AE_StackLineInsertBefore(ae, liLine->lpLine))
    {
      lpNewElement->nLineWidth=dwUnwrapLineWidth;
      lpNewElement->nLineBreak=dwUnwrapLineBreak;
      lpNewElement->nLineLen=dwUnwrapLineLen;

      if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
      {
        while (lpCurElement)
        {
          AE_memcpy(lpNewElement->wpLine + dwCountLen, lpCurElement->wpLine, lpCurElement->nLineLen * sizeof(wchar_t));
          dwCountWidth+=lpCurElement->nLineWidth;
          dwCountLen+=lpCurElement->nLineLen;

          //Update points
          for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
          {
            if (!lpPoint->bModify)
            {
              if (lpPoint->ciPoint.lpLine == lpCurElement)
              {
                lpPoint->ciPoint.nLine+=nLineCount;
                lpPoint->ciPoint.lpLine=lpNewElement;
                lpPoint->ciPoint.nCharInLine+=(dwCountLen - lpCurElement->nLineLen);
                lpPoint->bModify=TRUE;
              }
            }
          }

          if (dwCountWidth + ae->nAveCharWidth >= dwMaxWidth || lpCurElement->nLineBreak != AELB_WRAP)
          {
            AE_StackLineDelete(ae, lpCurElement);
            break;
          }

          --nLineCount;
          lpNextElement=lpCurElement->next;
          AE_StackLineDelete(ae, lpCurElement);
          lpCurElement=lpNextElement;
        }
        lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
      }
    }

    if (nLineCount)
    {
      liLine->lpLine=lpNewElement;

      //Update points
      for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
      {
        if (!lpPoint->bModify)
        {
          if (lpPoint->ciPoint.nLine > liLine->nLine)
          {
            lpPoint->ciPoint.nLine+=nLineCount;
          }
        }
      }
    }
  }
  return nLineCount;
}

void AE_CalcLinesWidth(AKELEDIT *ae, AELINEINDEX *liStartLine, AELINEINDEX *liEndLine, BOOL bFresh)
{
  AELINEINDEX liStart;
  AELINEINDEX liEnd;
  AELINEINDEX liMaxWidthLine;
  int nHScrollMax=0;

  if (!liStartLine)
  {
    liStart.nLine=0;
    liStart.lpLine=(AELINEDATA *)ae->hLinesStack.first;
  }
  else liStart=*liStartLine;

  if (!liEndLine)
  {
    liEnd.nLine=ae->nLineCount;
    liEnd.lpLine=(AELINEDATA *)ae->hLinesStack.last;
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

void AE_SetDrawRect(AKELEDIT *ae, RECT *lprcDraw, BOOL bRedraw)
{
  if (lprcDraw)
    ae->rcDraw=*lprcDraw;
  else
    ae->rcDraw=ae->rcEdit;

  if (bRedraw) InvalidateRect(ae->hWndEdit, NULL, TRUE);
}

void AE_SetEditFontA(AKELEDIT *ae, HFONT hFont, BOOL bRedraw)
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

  //Create URL font
  if (ae->hFontUrl) DeleteObject(ae->hFontUrl);
  AE_memcpy(&ae->lfEditUrlA, &ae->lfEditA, sizeof(LOGFONTA));
  ae->lfEditUrlA.lfUnderline=TRUE;
  ae->hFontUrl=(HFONT)CreateFontIndirectA(&ae->lfEditUrlA);

  GetTextMetricsA(ae->hDC, &tmEdit);
  ae->nCharHeight=tmEdit.tmHeight;
  ae->bFixedCharWidth=!(tmEdit.tmPitchAndFamily & TMPF_FIXED_PITCH);

  GetTextExtentPoint32A(ae->hDC, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeAverageWidth);
  ae->nAveCharWidth=sizeAverageWidth.cx / 52;
  ae->nTabWidth=ae->nAveCharWidth * ae->nTabStop;
  GetTextExtentPoint32A(ae->hDC, " ", 1, &sizeAverageWidth);
  ae->nSpaceCharWidth=sizeAverageWidth.cx;

  InvalidateRect(ae->hWndEdit, &ae->rcDraw, bRedraw);
}

void AE_SetEditFontW(AKELEDIT *ae, HFONT hFont, BOOL bRedraw)
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

  //Create URL font
  if (ae->hFontUrl) DeleteObject(ae->hFontUrl);
  AE_memcpy(&ae->lfEditUrlW, &ae->lfEditW, sizeof(LOGFONTW));
  ae->lfEditUrlW.lfUnderline=TRUE;
  ae->hFontUrl=(HFONT)CreateFontIndirectW(&ae->lfEditUrlW);

  GetTextMetricsW(ae->hDC, &tmEdit);
  ae->nCharHeight=tmEdit.tmHeight;
  ae->bFixedCharWidth=!(tmEdit.tmPitchAndFamily & TMPF_FIXED_PITCH);

  GetTextExtentPoint32W(ae->hDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &sizeAverageWidth);
  ae->nAveCharWidth=sizeAverageWidth.cx / 52;
  ae->nTabWidth=ae->nAveCharWidth * ae->nTabStop;
  GetTextExtentPoint32W(ae->hDC, L" ", 1, &sizeAverageWidth);
  ae->nSpaceCharWidth=sizeAverageWidth.cx;

  InvalidateRect(ae->hWndEdit, &ae->rcDraw, bRedraw);
}

void AE_SetSelectionPos(AKELEDIT *ae, const AECHARINDEX *ciSelStart, const AECHARINDEX *ciSelEnd, BOOL bColumnSel, DWORD dwSelFlags)
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

  AE_StackUndoGroupStop(ae);

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
      AE_GetPosFromCharEx(ae, &ciSelStartNew, &ptSelStart, NULL);
      if (AE_IndexCompare(&ae->ciSelStartIndex, &ciSelStartNew))
        if (ptSelStart.x > max(ae->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth)
          return;

      AE_GetPosFromCharEx(ae, &ciSelEndNew, &ptSelEnd, NULL);
      if (AE_IndexCompare(&ae->ciSelEndIndex, &ciSelEndNew))
        if (ptSelEnd.x > max(ae->nHScrollMax, ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth)
          return;
    }
    else
    {
      ciSelStartNew.nCharInLine=min(ciSelStartNew.nCharInLine, ciSelStartNew.lpLine->nLineLen);
      ciSelEndNew.nCharInLine=min(ciSelEndNew.nCharInLine, ciSelEndNew.lpLine->nLineLen);
      ciCaretNew.nCharInLine=min(ciCaretNew.nCharInLine, ciCaretNew.lpLine->nLineLen);
    }

    //Notification
    if (!(dwSelFlags & AESELT_LOCKNOTIFY))
    {
      AE_NotifySelChanging(ae);
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

    ae->nSelStartCharOffset=AE_AkelIndexToRichOffset(ae, &ciSelStartNew);
    ae->ciSelStartIndex=ciSelStartNew;
    ae->nSelEndCharOffset=AE_AkelIndexToRichOffset(ae, &ciSelEndNew);
    ae->ciSelEndIndex=ciSelEndNew;
    AE_GetPosFromCharEx(ae, &ciCaretNew, &ae->ptCaret, NULL);
    ae->ciCaretIndex=ciCaretNew;

    if (!(dwSelFlags & AESELT_LOCKSCROLL))
    {
      //Set caret position
      AE_ScrollToCaret(ae, &ae->ptCaret);
    }
    if (!(dwSelFlags & AESELT_LOCKUPDATE))
    {
      //Set caret position
      ae->nHorizCaretPos=ae->ptCaret.x;
      if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

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
    }
    if (!(dwSelFlags & AESELT_LOCKNOTIFY))
    {
      AE_NotifySelChanged(ae);
    }
  }
}

void AE_UpdateSelection(AKELEDIT *ae, DWORD dwSelFlags)
{
  if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciCaretIndex))
    AE_SetSelectionPos(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, dwSelFlags);
  else
    AE_SetSelectionPos(ae, &ae->ciSelEndIndex, &ae->ciSelStartIndex, ae->bColumnSel, dwSelFlags);
}

void AE_SetMouseSelection(AKELEDIT *ae, POINT *ptPos, BOOL bColumnSel, BOOL bShift)
{
  AECHARINDEX ciCharIndex;
  AECHARINDEX ciSelEnd;

  if (ae->rcDraw.bottom - ae->rcDraw.top > 0 && ae->rcDraw.right - ae->rcDraw.left > 0)
  {
    AE_GetCharFromPos(ae, ptPos, &ciCharIndex, NULL, bColumnSel);

    //One click (capture)
    if (ae->nLButtonDownCount == 0)
    {
      if (ae->bMarginSelect == TRUE)
      {
        //Margin selection, same as three clicks (capture)
        if (bShift)
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonClick) < 0)
          {
            if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonStart) < 0)
              ciCharIndex.nCharInLine=0;
            else
              ciCharIndex=ae->ciLButtonStart;
            ciSelEnd=ae->ciLButtonEnd;
          }
          else
          {
            if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonEnd) >= 0)
            {
              if (!AE_GetIndex(ae, AEGI_NEXTLINE, &ciCharIndex, &ciCharIndex, FALSE))
                ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
            }
            else
              ciCharIndex=ae->ciLButtonEnd;
            ciSelEnd=ae->ciLButtonStart;
          }

          if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
          {
            AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, ae->bColumnSel, 0);
          }
        }
      }
      else
      {
        //Characters selection
        if (bShift)
        {
          if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
          {
            if (!AE_IndexCompare(&ae->ciCaretIndex, &ae->ciSelEndIndex))
              ciSelEnd=ae->ciSelStartIndex;
            else
              ciSelEnd=ae->ciSelEndIndex;
            AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, ae->bColumnSel, 0);
          }
        }
        else AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, ae->bColumnSel, 0);
      }
    }
    //Two clicks (capture)
    else if (ae->nLButtonDownCount == 1)
    {
      if (bShift)
      {
        if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonClick) < 0)
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonStart) < 0)
            AE_GetPrevWord(ae, &ciCharIndex, &ciCharIndex, NULL, bColumnSel, ae->dwWordBreak, FALSE);
          else
            ciCharIndex=ae->ciLButtonStart;
          ciSelEnd=ae->ciLButtonEnd;
        }
        else
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonEnd) > 0)
            AE_GetNextWord(ae, &ciCharIndex, NULL, &ciCharIndex, bColumnSel, ae->dwWordBreak, FALSE);
          else
            ciCharIndex=ae->ciLButtonEnd;
          ciSelEnd=ae->ciLButtonStart;
        }

        if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
        {
          AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, ae->bColumnSel, 0);
        }
      }
    }
    //Three clicks (capture)
    else if (ae->nLButtonDownCount == 2)
    {
      if (bShift)
      {
        if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonClick) < 0)
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonStart) < 0)
            ciCharIndex.nCharInLine=0;
          else
            ciCharIndex=ae->ciLButtonStart;
          ciSelEnd=ae->ciLButtonEnd;
        }
        else
        {
          if (AE_IndexCompare(&ciCharIndex, &ae->ciLButtonEnd) >= 0)
          {
            if (!AE_GetIndex(ae, AEGI_NEXTLINE, &ciCharIndex, &ciCharIndex, FALSE))
              ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
          }
          else
            ciCharIndex=ae->ciLButtonEnd;
          ciSelEnd=ae->ciLButtonStart;
        }

        if (AE_IndexCompare(&ae->ciCaretIndex, &ciCharIndex))
        {
          AE_SetSelectionPos(ae, &ciCharIndex, &ciSelEnd, ae->bColumnSel, 0);
        }
      }
    }
  }
}

BOOL AE_IsCursorOnLeftMargin(AKELEDIT *ae, POINT *ptPos)
{
  if (ptPos->x >= 0 && ptPos->x < ae->rcDraw.left && ptPos->y > ae->rcDraw.top && ptPos->y < ae->rcDraw.bottom)
  {
    return TRUE;
  }
  return FALSE;
}

BOOL AE_IsCursorOnSelection(AKELEDIT *ae, POINT *ptPos)
{
  if (!(ae->dwOptions & AECO_DISABLEDRAG))
  {
    AECHARINDEX ciCharIndex;
    int nSelStartY;
    int nSelEndY;
    int nResult;

    if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
    {
      nSelStartY=(ae->ciSelStartIndex.nLine * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;
      nSelEndY=((ae->ciSelEndIndex.nLine + 1) * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;

      if (ptPos->y >= nSelStartY && ptPos->y <= nSelEndY)
      {
        if (nResult=AE_GetCharFromPos(ae, ptPos, &ciCharIndex, NULL, ae->bColumnSel))
        {
          if (ciCharIndex.lpLine->nSelStart != ciCharIndex.lpLine->nSelEnd &&
              (ciCharIndex.nCharInLine > ciCharIndex.lpLine->nSelStart ||
               ciCharIndex.nCharInLine == ciCharIndex.lpLine->nSelStart && (nResult == AEPC_BEFORE || nResult == AEPC_EQUAL)) &&
              (ciCharIndex.nCharInLine < ciCharIndex.lpLine->nSelEnd ||
               ciCharIndex.nCharInLine == ciCharIndex.lpLine->nSelEnd && (nResult == AEPC_AFTER || nResult == AEPC_EQUAL)))
          {
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

BOOL AE_IsCursorOnUrl(AKELEDIT *ae, POINT *ptPos, AECHARRANGE *crLink)
{
  if (ae->bDetectUrl)
  {
    if ((ae->dwEventMask & AENM_LINK) ||
        (ae->dwRichEventMask & ENM_LINK))
    {
      AECHARINDEX ciCharIndex;
      int nStartUrl;
      int nEndUrl;
      int nPrefix;
      int nResult;

      if (PtInRect(&ae->rcDraw, *ptPos))
      {
        if (nResult=AE_GetCharFromPos(ae, ptPos, &ciCharIndex, NULL, FALSE))
        {
          if (nResult == AEPC_AFTER)
            ciCharIndex.nCharInLine=max(ciCharIndex.nCharInLine - 1, 0);

          if (ciCharIndex.nCharInLine < ciCharIndex.lpLine->nSelStart || ciCharIndex.nCharInLine >= ciCharIndex.lpLine->nSelEnd)
          {
            for (nStartUrl=ciCharIndex.nCharInLine; nStartUrl >= 0; --nStartUrl)
            {
              if (AE_IsInDelimiterList(ae->wszUrlDelimiters, ciCharIndex.lpLine->wpLine[nStartUrl]))
                return FALSE;

              for (nPrefix=0; ae->lpUrlPrefixes[nPrefix]; ++nPrefix)
              {
                if (!AE_WideStrCmpLenI(ae->lpUrlPrefixes[nPrefix], ciCharIndex.lpLine->wpLine + nStartUrl, (DWORD)-1))
                {
                  if (nStartUrl == 0 || AE_IsInDelimiterList(ae->wszUrlDelimiters, ciCharIndex.lpLine->wpLine[nStartUrl - 1]))
                  {
                    //Found end of URL
                    for (nEndUrl=ciCharIndex.nCharInLine; nEndUrl < ciCharIndex.lpLine->nLineLen; ++nEndUrl)
                    {
                      if (AE_IsInDelimiterList(ae->wszUrlDelimiters, ciCharIndex.lpLine->wpLine[nEndUrl]))
                        break;
                    }

                    if (crLink)
                    {
                      crLink->ciMin.nLine=ciCharIndex.nLine;
                      crLink->ciMin.lpLine=ciCharIndex.lpLine;
                      crLink->ciMin.nCharInLine=nStartUrl;
                      crLink->ciMax.nLine=ciCharIndex.nLine;
                      crLink->ciMax.lpLine=ciCharIndex.lpLine;
                      crLink->ciMax.nCharInLine=nEndUrl;
                    }
                    return TRUE;
                  }
                }
              }
            }
          }
        }
      }
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

BOOL AE_UpdateCaret(AKELEDIT *ae, BOOL bFocus, BOOL bFresh)
{
  HBITMAP hCaretBitmap=NULL;
  int nCaretWidth;
  int nCaretHeight;

  if (bFocus)
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

  if (bFocus)
  {
    if (CreateCaret(ae->hWndEdit, (HBITMAP)hCaretBitmap, nCaretWidth, nCaretHeight))
    {
      ae->bCaretVisible=FALSE;
      AE_SetCaretPos(ae, &ae->ptCaret);
    }
    return TRUE;
  }
  return FALSE;
}

BOOL AE_SetCaretPos(AKELEDIT *ae, POINT *ptCaret)
{
  BOOL bResult;

  if (ae->bOverType)
    bResult=SetCaretPos((ptCaret->x - ae->nHScrollPos) + ae->rcDraw.left, (ptCaret->y - ae->nVScrollPos) + ae->rcDraw.top + max(ae->nCharHeight - ae->nCaretOvertypeHeight, 0));
  else
    bResult=SetCaretPos((ptCaret->x - ae->nHScrollPos) + ae->rcDraw.left, (ptCaret->y - ae->nVScrollPos) + ae->rcDraw.top);

  SetCaretVis(ae, ptCaret);
  return bResult;
}

void SetCaretVis(AKELEDIT *ae, POINT *ptCaret)
{
  if ((ptCaret->x - ae->nHScrollPos) + ae->rcDraw.left < ae->rcDraw.left ||
      (ptCaret->x - ae->nHScrollPos) + ae->rcDraw.left > ae->rcDraw.right ||
      (ptCaret->y - ae->nVScrollPos) + ae->rcDraw.top < ae->rcDraw.top - ae->nCharHeight ||
      (ptCaret->y - ae->nVScrollPos) + ae->rcDraw.top > ae->rcDraw.bottom)
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

void AE_ScrollToCaret(AKELEDIT *ae, POINT *ptCaret)
{
  if (ae->bWordWrap)
  {
    if (ptCaret->x < ae->nHScrollPos)
    {
      AE_ScrollEditWindow(ae, SB_HORZ, 0);
    }
  }
  else
  {
    if (ae->bDropping)
    {
      if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth * 3)
      {
        AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + ae->nAveCharWidth * 2 + 1, 0));
      }
      else if (ptCaret->x < ae->nHScrollPos + ae->nAveCharWidth * 2)
      {
        AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - ae->nAveCharWidth * 2, 0));
      }
    }
    else if (ae->dwMouseMoveTimer)
    {
      if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth)
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
      if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - ae->nAveCharWidth)
      {
        AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + (ae->rcDraw.right - ae->rcDraw.left) / 3, 0));
      }
      else if (ptCaret->x < ae->nHScrollPos)
      {
        AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) / 3, 0));
      }
    }
  }

  if (ae->bDropping)
  {
    if (ptCaret->y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - ae->nCharHeight * 2)
    {
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->nCharHeight * 3 + 1, 0));
    }
    else if (ptCaret->y < ae->nVScrollPos + ae->nCharHeight)
    {
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - ae->nCharHeight * 2, 0));
    }
  }
  else
  {
    if (ptCaret->y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - ae->nCharHeight)
    {
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + ae->nCharHeight + 1, 0));
    }
    else if (ptCaret->y < ae->nVScrollPos)
    {
      AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y, 0));
    }
  }
}

DWORD AE_ScrollToCaretEx(AKELEDIT *ae, POINT *ptCaret, DWORD dwFlags, WORD wUnitX, WORD wUnitY, BOOL bTest)
{
  int x=-1;
  int y=-1;
  DWORD dwResult=0;

  if (dwFlags & AECS_UNITPIXELX)
  {
    x=wUnitX;
  }
  else if (dwFlags & AECS_UNITCHARX)
  {
    x=ae->nAveCharWidth * wUnitX;
  }
  else if (dwFlags & AECS_UNITRECTDIVX)
  {
    x=(ae->rcDraw.right - ae->rcDraw.left) / wUnitX;
  }

  if (dwFlags & AECS_UNITPIXELY)
  {
    y=wUnitY;
  }
  else if (dwFlags & AECS_UNITCHARY)
  {
    y=ae->nCharHeight * wUnitY;
  }
  else if (dwFlags & AECS_UNITRECTDIVY)
  {
    y=(ae->rcDraw.bottom - ae->rcDraw.top) / wUnitY;
  }

  if (x != -1)
  {
    if (ae->bWordWrap)
    {
      if (ptCaret->x < ae->nHScrollPos)
      {
        if (!bTest) AE_ScrollEditWindow(ae, SB_HORZ, 0);
        dwResult|=AECSE_SCROLLEDX;
      }
    }
    else
    {
      if (dwFlags & AECS_FORCELEFT)
      {
        if (!bTest) AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - x, 0));
        dwResult|=AECSE_SCROLLEDX;
      }
      else if (dwFlags & AECS_FORCERIGHT)
      {
        if (!bTest) AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + x + 1, 0));
        dwResult|=AECSE_SCROLLEDX;
      }
      else
      {
        if (ptCaret->x >= ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left) - x)
        {
          if (!bTest) AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - (ae->rcDraw.right - ae->rcDraw.left) + x + 1, 0));
          dwResult|=AECSE_SCROLLEDX;
        }
        else if (ptCaret->x < ae->nHScrollPos + x)
        {
          if (!bTest) AE_ScrollEditWindow(ae, SB_HORZ, max(ptCaret->x - x, 0));
          dwResult|=AECSE_SCROLLEDX;
        }
      }
    }
  }

  if (y != -1)
  {
    if (dwFlags & AECS_FORCETOP)
    {
      if (!bTest) AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - y, 0));
      dwResult|=AECSE_SCROLLEDY;
    }
    else if (dwFlags & AECS_FORCEBOTTOM)
    {
      if (!bTest) AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + y + 1, 0));
      dwResult|=AECSE_SCROLLEDY;
    }
    else
    {
      if (ptCaret->y >= ae->nVScrollPos + (ae->rcDraw.bottom - ae->rcDraw.top) - y)
      {
        if (!bTest) AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - (ae->rcDraw.bottom - ae->rcDraw.top) + y + 1, 0));
        dwResult|=AECSE_SCROLLEDY;
      }
      else if (ptCaret->y < ae->nVScrollPos + y)
      {
        if (!bTest) AE_ScrollEditWindow(ae, SB_VERT, max(ptCaret->y - y, 0));
        dwResult|=AECSE_SCROLLEDY;
      }
    }
  }
  return dwResult;
}

void AE_UpdateScrollBars(AKELEDIT *ae, int nBar)
{
  SCROLLINFO si;

  if (nBar == SB_BOTH || nBar == SB_HORZ)
  {
    if (ae->nHScrollMax > ae->rcDraw.right - ae->rcDraw.left)
    {
      if (ae->bHScrollShow)
      {
        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
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
        ae->nHScrollPos=min(ae->nHScrollPos, ae->nHScrollMax - (ae->rcDraw.right - ae->rcDraw.left));
        ae->nHScrollPos=max(ae->nHScrollPos, 0);
      }
    }
    else
    {
      if (ae->bHScrollShow)
      {
        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_RANGE|(ae->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
        si.nMin=0;
        si.nMax=0;
        SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, TRUE);
      }
      ae->nHScrollPos=0;
    }
  }

  if (nBar == SB_BOTH || nBar == SB_VERT)
  {
    if (ae->nVScrollMax > ae->rcDraw.bottom - ae->rcDraw.top)
    {
      if (ae->bVScrollShow)
      {
        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_RANGE|SIF_PAGE|SIF_POS|(ae->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
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
        ae->nVScrollPos=min(ae->nVScrollPos, ae->nVScrollMax - (ae->rcDraw.bottom - ae->rcDraw.top));
        ae->nVScrollPos=max(ae->nVScrollPos, 0);
      }
    }
    else
    {
      if (ae->bVScrollShow)
      {
        si.cbSize=sizeof(SCROLLINFO);
        si.fMask=SIF_RANGE|(ae->dwOptions & AECO_DISABLENOSCROLL?SIF_DISABLENOSCROLL:0);
        si.nMin=0;
        si.nMax=0;
        SetScrollInfo(ae->hWndEdit, SB_VERT, &si, TRUE);
      }
      ae->nVScrollPos=0;
    }
  }

  if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
}

int AE_ScrollEditWindow(AKELEDIT *ae, int nBar, int nPos)
{
  SCROLLINFO si;
  int nScrollPos=0;

  if (nBar == SB_HORZ)
  {
    if (ae->bVScrollShow)
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_POS|SIF_DISABLENOSCROLL;
      si.nPos=nPos;
      SetScrollInfo(ae->hWndEdit, SB_HORZ, &si, TRUE);

      si.fMask=SIF_POS;
      GetScrollInfo(ae->hWndEdit, SB_HORZ, &si);
      nPos=si.nPos;
    }
    else
    {
      nPos=min(nPos, ae->nHScrollMax - (ae->rcDraw.right - ae->rcDraw.left));
      nPos=max(nPos, 0);
    }
    nScrollPos=ae->nHScrollPos;

    if (!ae->bHScrollLock)
    {
      if (nPos != ae->nHScrollPos)
      {
        if (!AE_NotifyHScroll(ae, nPos - ae->nHScrollPos))
        {
          ScrollWindow(ae->hWndEdit, ae->nHScrollPos - nPos, 0, NULL, &ae->rcDraw);
          ae->nHScrollPos=nPos;
          UpdateWindow(ae->hWndEdit);
        }
      }
    }
    nScrollPos=ae->nHScrollPos - nScrollPos;
  }
  else if (nBar == SB_VERT)
  {
    if (ae->bVScrollShow)
    {
      si.cbSize=sizeof(SCROLLINFO);
      si.fMask=SIF_POS|SIF_DISABLENOSCROLL;
      si.nPos=nPos;
      SetScrollInfo(ae->hWndEdit, SB_VERT, &si, TRUE);

      si.fMask=SIF_POS;
      GetScrollInfo(ae->hWndEdit, SB_VERT, &si);
      nPos=si.nPos;
    }
    else
    {
      nPos=min(nPos, ae->nVScrollMax - (ae->rcDraw.bottom - ae->rcDraw.top));
      nPos=max(nPos, 0);
    }
    nScrollPos=ae->nVScrollPos;

    if (!ae->bVScrollLock)
    {
      if (nPos != ae->nVScrollPos)
      {
        if (!AE_NotifyVScroll(ae, nPos - ae->nVScrollPos))
        {
          ScrollWindow(ae->hWndEdit, 0, ae->nVScrollPos - nPos, NULL, &ae->rcDraw);
          ae->nVScrollPos=nPos;
          UpdateWindow(ae->hWndEdit);
        }
      }
    }
    nScrollPos=ae->nVScrollPos - nScrollPos;
  }

  SetCaretVis(ae, &ae->ptCaret);
  return nScrollPos;
}

int AE_HScroll(AKELEDIT *ae, int nAction)
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
    si.nPos-=ae->nAveCharWidth;
  }
  else if (nAction == SB_LINERIGHT)
  {
    si.nPos+=ae->nAveCharWidth;
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
  return AE_ScrollEditWindow(ae, SB_HORZ, si.nPos);
}

int AE_VScroll(AKELEDIT *ae, int nAction)
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
    si.nPos-=ae->nCharHeight;
  }
  else if (nAction == SB_LINEDOWN)
  {
    si.nPos+=ae->nCharHeight;
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
  return AE_ScrollEditWindow(ae, SB_VERT, si.nPos);
}

int AE_HScrollLine(AKELEDIT *ae, int nChar)
{
  int nPos;

  nPos=ae->nHScrollPos;
  nPos+=ae->nAveCharWidth * nChar;
  nPos=(nPos / ae->nAveCharWidth) * ae->nAveCharWidth;

  return AE_ScrollEditWindow(ae, SB_HORZ, nPos);
}

int AE_VScrollLine(AKELEDIT *ae, int nLine)
{
  int nPos;

  nPos=ae->nVScrollPos;
  nPos+=ae->nCharHeight * nLine;
  nPos=(nPos / ae->nCharHeight) * ae->nCharHeight;

  return AE_ScrollEditWindow(ae, SB_VERT, nPos);
}

void AE_Paint(AKELEDIT *ae)
{
  PAINTSTRUCT ps;

  if (GetUpdateRect(ae->hWndEdit, &ae->rcErase, FALSE))
  {
    if (BeginPaint(ae->hWndEdit, &ps))
    {
      AECHARRANGE crLink={0};
      AECHARINDEX ciDrawLine;
      RECT rcDraw;
      POINT ptDraw;
      SIZE sizeLine;
      RECT rcSpace;
      DWORD dwColorBG;
      DWORD dwColorText;
      HRGN hDrawRgn;
      HBRUSH hbrBG;
      wchar_t *wpStartDraw;
      int nLineSelection;
      int nLineLen;
      int nStartDrawWidth;
      int nMaxLineWidth=0;
      int nMaxDrawCharsCount;
      int nLineWidth;
      int nTabWidth=0;

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
        ciDrawLine.nLine=(ae->nVScrollPos + (rcDraw.top - ae->rcDraw.top)) / ae->nCharHeight;
        ciDrawLine.lpLine=AE_GetLineData(ae, ciDrawLine.nLine);
        ciDrawLine.nCharInLine=0;
        ptDraw.y=(ciDrawLine.nLine * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;
        nMaxLineWidth=ae->nHScrollPos + (ae->rcDraw.right - ae->rcDraw.left);

        if (ciDrawLine.lpLine)
        {
          ae->nFirstDrawLineOffset=AE_AkelIndexToRichOffset(ae, &ciDrawLine);
          ae->liFirstDrawLine.nLine=ciDrawLine.nLine;
          ae->liFirstDrawLine.lpLine=ciDrawLine.lpLine;
        }
      }
      else ciDrawLine.lpLine=NULL;

      while (ciDrawLine.lpLine)
      {
        //Draw line
        ptDraw.x=ae->rcDraw.left - ae->nHScrollPos;
        nStartDrawWidth=0;
        nLineWidth=0;
        nMaxDrawCharsCount=0;
        wpStartDraw=ciDrawLine.lpLine->wpLine;

        if (ae->bHideSelection)
        {
          nLineSelection=AELS_EMPTY;
          dwColorText=ae->crBasicText;
          dwColorBG=ae->crBasicBk;
          hbrBG=ae->hBasicBk;
        }
        else
        {
          if (ciDrawLine.lpLine->nSelStart == 0 && ciDrawLine.lpLine->nSelEnd == ciDrawLine.lpLine->nLineLen)
          {
            nLineSelection=AELS_FULL;
            dwColorText=ae->crSelText;
            dwColorBG=ae->crSelBk;
            hbrBG=ae->hSelBk;
          }
          else if (ciDrawLine.lpLine->nSelStart == ciDrawLine.lpLine->nSelEnd)
          {
            nLineSelection=AELS_EMPTY;

            if (ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
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

            if (ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
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

        for (ciDrawLine.nCharInLine=0; ciDrawLine.nCharInLine <= ciDrawLine.lpLine->nLineLen; ++ciDrawLine.nCharInLine)
        {
          if (ae->bDetectUrl)
          {
            //Detect URL
            if (!crLink.ciMin.lpLine || !crLink.ciMax.lpLine)
            {
              if (!(ciDrawLine.nCharInLine >= ciDrawLine.lpLine->nSelStart && ciDrawLine.lpLine->nSelEnd == ciDrawLine.lpLine->nLineLen))
              {
                int nPrefix;

                for (nPrefix=0; ae->lpUrlPrefixes[nPrefix]; ++nPrefix)
                {
                  if (!AE_WideStrCmpLenI(ae->lpUrlPrefixes[nPrefix], ciDrawLine.lpLine->wpLine + ciDrawLine.nCharInLine, (DWORD)-1))
                  {
                    if (ciDrawLine.nCharInLine == 0 || AE_IsInDelimiterList(ae->wszUrlDelimiters, ciDrawLine.lpLine->wpLine[ciDrawLine.nCharInLine - 1]))
                    {
                      crLink.ciMin=ciDrawLine;
                      crLink.ciMax=ciDrawLine;

                      while (crLink.ciMax.nCharInLine < crLink.ciMax.lpLine->nLineLen)
                      {
                        if (AE_IsInDelimiterList(ae->wszUrlDelimiters, crLink.ciMax.lpLine->wpLine[crLink.ciMax.nCharInLine]))
                          break;

                        ++crLink.ciMax.nCharInLine;
                      }

                      //Draw text before URL
                      SetTextColor(ps.hdc, dwColorText);
                      SetBkColor(ps.hdc, dwColorBG);
                      AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
                      nMaxDrawCharsCount=0;
                      break;
                    }
                  }
                }
              }
            }

            //Highlight URL
            if (crLink.ciMin.lpLine && crLink.ciMax.lpLine)
            {
              if (ciDrawLine.lpLine->nSelStart == ciDrawLine.nCharInLine)
              {
                if (AE_IndexCompare(&crLink.ciMin, &ciDrawLine) < 0)
                {
                  //Draw URL before selection start
                  if (ae->hFontUrl) SelectObject(ps.hdc, ae->hFontUrl);
                  SetTextColor(ps.hdc, ae->crUrlText);
                  SetBkColor(ps.hdc, dwColorBG);
                  AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
                  nMaxDrawCharsCount=0;
                  if (ae->hFont) SelectObject(ps.hdc, ae->hFont);
                }
              }
              if (AE_IndexCompare(&crLink.ciMax, &ciDrawLine) == 0)
              {
                crLink.ciMin.lpLine=NULL;
                crLink.ciMax.lpLine=NULL;

                if (crLink.ciMax.nCharInLine <= ciDrawLine.lpLine->nSelStart || crLink.ciMax.nCharInLine > ciDrawLine.lpLine->nSelEnd)
                {
                  //Draw full URL or last part of it
                  if (ae->hFontUrl) SelectObject(ps.hdc, ae->hFontUrl);
                  SetTextColor(ps.hdc, ae->crUrlText);
                  SetBkColor(ps.hdc, dwColorBG);
                  AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
                  nMaxDrawCharsCount=0;
                  if (ae->hFont) SelectObject(ps.hdc, ae->hFont);
                }
              }
            }
          }
          if (ciDrawLine.nCharInLine == ciDrawLine.lpLine->nLineLen) break;

          if (nLineSelection == AELS_PARTLY)
          {
            if (ciDrawLine.lpLine->nSelStart == ciDrawLine.nCharInLine)
            {
              SetTextColor(ps.hdc, dwColorText);
              SetBkColor(ps.hdc, dwColorBG);
              AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
              nMaxDrawCharsCount=0;

              dwColorText=ae->crSelText;
              dwColorBG=ae->crSelBk;
              hbrBG=ae->hSelBk;
            }
            else if (ciDrawLine.lpLine->nSelEnd == ciDrawLine.nCharInLine)
            {
              SetTextColor(ps.hdc, dwColorText);
              SetBkColor(ps.hdc, dwColorBG);
              AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
              nMaxDrawCharsCount=0;

              if (ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
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

          if (ciDrawLine.lpLine->wpLine[ciDrawLine.nCharInLine] == L'\t')
          {
            SetTextColor(ps.hdc, dwColorText);
            SetBkColor(ps.hdc, dwColorBG);
            AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
            nMaxDrawCharsCount=0;

            nTabWidth=ae->nTabWidth - nStartDrawWidth % ae->nTabWidth;
            rcSpace.left=ptDraw.x + nStartDrawWidth;
            rcSpace.top=ptDraw.y;
            rcSpace.right=rcSpace.left + nTabWidth;
            rcSpace.bottom=rcSpace.top + ae->nCharHeight;
            FillRect(ps.hdc, &rcSpace, hbrBG);
            nStartDrawWidth+=nTabWidth;
            wpStartDraw+=1;
          }

          //Draw only 2048 characters at once
          if (nMaxDrawCharsCount >= 2048)
          {
            SetTextColor(ps.hdc, dwColorText);
            SetBkColor(ps.hdc, dwColorBG);
            AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
            nMaxDrawCharsCount=0;
          }
          else ++nMaxDrawCharsCount;

          //Stop line drawing if it's outside draw area
          if (nLineWidth > nMaxLineWidth)
          {
            if (ae->bDetectUrl)
            {
              //Highlight URL
              if (crLink.ciMin.lpLine && crLink.ciMax.lpLine)
              {
                crLink.ciMin.lpLine=NULL;
                crLink.ciMax.lpLine=NULL;

                if (ciDrawLine.nCharInLine <= ciDrawLine.lpLine->nSelStart || ciDrawLine.nCharInLine > ciDrawLine.lpLine->nSelEnd)
                {
                  //Draw full URL or last part of it
                  if (ae->hFontUrl) SelectObject(ps.hdc, ae->hFontUrl);
                  SetTextColor(ps.hdc, ae->crUrlText);
                  SetBkColor(ps.hdc, dwColorBG);
                  AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
                  nMaxDrawCharsCount=0;
                  if (ae->hFont) SelectObject(ps.hdc, ae->hFont);
                  goto NextLine;
                }
              }
            }
            SetTextColor(ps.hdc, dwColorText);
            SetBkColor(ps.hdc, dwColorBG);
            AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);
            goto NextLine;
          }
          if (ciDrawLine.lpLine->wpLine[ciDrawLine.nCharInLine] == L'\t')
            nLineWidth+=nTabWidth;
          else
            nLineWidth+=AE_GetCharWidth(ae, ciDrawLine.lpLine->wpLine[ciDrawLine.nCharInLine]);
        }

        SetTextColor(ps.hdc, dwColorText);
        SetBkColor(ps.hdc, dwColorBG);
        AE_PaintTextOut(ae, ps.hdc, &ptDraw, ciDrawLine.lpLine->wpLine, ciDrawLine.nCharInLine, nLineWidth, &wpStartDraw, &nStartDrawWidth);

        if (ae->bColumnSel)
        {
          if (nLineSelection == AELS_PARTLY)
          {
            nLineLen=ciDrawLine.lpLine->nLineLen;

            if (ciDrawLine.lpLine->nSelStart >= ciDrawLine.lpLine->nLineLen)
            {
              if (ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
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
              sizeLine.cx=(ciDrawLine.lpLine->nSelStart - nLineLen) * ae->nSpaceCharWidth;

              rcSpace.left=ptDraw.x + nStartDrawWidth;
              rcSpace.top=ptDraw.y;
              rcSpace.right=rcSpace.left + sizeLine.cx;
              rcSpace.bottom=rcSpace.top + ae->nCharHeight;
              FillRect(ps.hdc, &rcSpace, hbrBG);
              nStartDrawWidth+=sizeLine.cx;
              nLineLen+=(ciDrawLine.lpLine->nSelStart - nLineLen);
            }
            if (ciDrawLine.lpLine->nSelEnd > ciDrawLine.lpLine->nLineLen)
            {
              dwColorText=ae->crSelText;
              dwColorBG=ae->crSelBk;
              hbrBG=ae->hSelBk;
              sizeLine.cx=(ciDrawLine.lpLine->nSelEnd - nLineLen) * ae->nSpaceCharWidth;

              rcSpace.left=ptDraw.x + nStartDrawWidth;
              rcSpace.top=ptDraw.y;
              rcSpace.right=rcSpace.left + sizeLine.cx;
              rcSpace.bottom=rcSpace.top + ae->nCharHeight;
              FillRect(ps.hdc, &rcSpace, hbrBG);
              nStartDrawWidth+=sizeLine.cx;
              nLineLen+=(ciDrawLine.lpLine->nSelEnd - nLineLen);
            }
          }
        }
        else
        {
          //Select new line space
          if (ciDrawLine.nLine >= ae->ciSelStartIndex.nLine &&
              ciDrawLine.nLine < ae->ciSelEndIndex.nLine)
          {
            if (!ae->bHideSelection)
            {
              hbrBG=ae->hSelBk;

              rcSpace.left=ptDraw.x + nStartDrawWidth;
              rcSpace.top=ptDraw.y;
              rcSpace.right=rcSpace.left + ae->nAveCharWidth;
              rcSpace.bottom=rcSpace.top + ae->nCharHeight;
              FillRect(ps.hdc, &rcSpace, hbrBG);
              nStartDrawWidth+=ae->nAveCharWidth;
            }
          }
        }

        if (ciDrawLine.lpLine == ae->ciCaretIndex.lpLine)
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
        rcSpace.left=ptDraw.x + nStartDrawWidth;
        rcSpace.top=ptDraw.y;
        rcSpace.right=ae->rcDraw.right;
        rcSpace.bottom=rcSpace.top + ae->nCharHeight;
        if (rcSpace.left < rcSpace.right)
          FillRect(ps.hdc, &rcSpace, hbrBG);

        NextLine:
        ptDraw.y+=ae->nCharHeight;
        if (ptDraw.y >= rcDraw.bottom)
          break;

        ++ciDrawLine.nLine;
        ciDrawLine.lpLine=ciDrawLine.lpLine->next;
        ciDrawLine.nCharInLine=0;
      }
      if (hDrawRgn) DeleteObject(hDrawRgn);
      EndPaint(ae->hWndEdit, &ps);
    }
  }
}

void AE_PaintTextOut(AKELEDIT *ae, HDC hDC, POINT *ptDraw, wchar_t *wpLine, int nLineLen, int nLineWidth, wchar_t **wpTextInLine, int *nTextInLineWidth)
{
  int nTextLen=nLineLen - (*wpTextInLine - wpLine);
  int nTextWidth=nLineWidth - *nTextInLineWidth;

  if (nTextLen)
  {
    if (ptDraw->x + *nTextInLineWidth + nTextWidth > ae->rcDraw.left &&
        ptDraw->x + *nTextInLineWidth < ae->rcDraw.right)
    {
      TextOutW(hDC, ptDraw->x + *nTextInLineWidth, ptDraw->y, *wpTextInLine, nTextLen);
    }
    *nTextInLineWidth+=nTextWidth;
    *wpTextInLine+=nTextLen;
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
    rcRedraw.top=(nFirstLine * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;
  if (nLastLine == -1)
    rcRedraw.bottom=ae->rcDraw.bottom;
  else
    rcRedraw.bottom=((nLastLine + 1) * ae->nCharHeight - ae->nVScrollPos) + ae->rcDraw.top;

  rcRedraw.top=max(rcRedraw.top, ae->rcDraw.top);
  rcRedraw.bottom=min(rcRedraw.bottom, ae->rcDraw.bottom);

  InvalidateRect(ae->hWndEdit, &rcRedraw, bErase);
}

void AE_HideSelection(AKELEDIT *ae, BOOL bHide)
{
  ae->bHideSelection=bHide;
  AE_RedrawLineRange(ae, ae->ciSelStartIndex.nLine, ae->ciSelEndIndex.nLine, FALSE);
  UpdateWindow(ae->hWndEdit);
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
/*
  //Some fonts have TMPF_FIXED_PITCH flag, but not actually have fixed width characters
  if (ae->bFixedCharWidth)
  {
    lpSize->cx=ae->nAveCharWidth * nStringLen;
    lpSize->cy=ae->nCharHeight;
    return TRUE;
  }
  else
*/
  {
    return GetTextExtentPoint32W(ae->hDC, wpString, nStringLen, lpSize);
  }
}

int AE_GetCharWidth(AKELEDIT *ae, wchar_t wchChar)
{
  SIZE sizeChar;

  if (AE_GetTextExtentPoint32(ae, &wchChar, 1, &sizeChar))
    return sizeChar.cx;
  return 0;
}

BOOL AE_GetLineWidth(AKELEDIT *ae, AELINEDATA *lpLine)
{
  AECHARINDEX ciCharIndex;
  POINT ptLineWidth;

  ciCharIndex.nLine=0;
  ciCharIndex.nCharInLine=lpLine->nLineLen;
  ciCharIndex.lpLine=lpLine;

  if (AE_GetPosFromChar(ae, &ciCharIndex, &ptLineWidth, NULL))
  {
    lpLine->nLineWidth=ptLineWidth.x;
    return TRUE;
  }
  return FALSE;
}

BOOL AE_GetPosFromChar(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos)
{
  SIZE sizeString;
  wchar_t *wpStringCount;
  int nStringChars;
  int nStringLen;
  int nStringWidth=0;
  int nMaxCharsCount=0;
  int nTabWidth;
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

        nTabWidth=ae->nTabWidth - nStringWidth % ae->nTabWidth;
        nStringWidth+=nTabWidth;
        wpStringCount+=1;
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

    if (ciCharIndex->nCharInLine > ciCharIndex->lpLine->nLineLen)
    {
      nStringWidth+=(ciCharIndex->nCharInLine - ciCharIndex->lpLine->nLineLen) * ae->nSpaceCharWidth;
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

BOOL AE_GetPosFromCharEx(AKELEDIT *ae, const AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, POINT *ptClientPos)
{
  SIZE sizeChar;
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nOffset=0;
  int nStringWidth=0;
  int nStartChar=0;
  int nTabWidth;
  int i;

  if (ciCharIndex->lpLine)
  {
    dwFirst=mod(ciCharIndex->nCharInLine - 0);
    if (ciCharIndex->lpLine->nLineWidth != -1)
      dwSecond=mod(ciCharIndex->nCharInLine - ciCharIndex->lpLine->nLineLen);
    if (ae->ptCaret.x && ciCharIndex->lpLine == ae->ciCaretIndex.lpLine)
      dwThird=mod(ciCharIndex->nCharInLine - ae->ciCaretIndex.nCharInLine);

    if (dwFirst <= dwSecond && dwFirst <= dwThird)
    {
      nOffset=ciCharIndex->nCharInLine - 0;
      nStringWidth=0;
      nStartChar=0;
    }
    else if (dwSecond <= dwFirst && dwSecond <= dwThird)
    {
      nOffset=ciCharIndex->nCharInLine - ciCharIndex->lpLine->nLineLen;
      nStringWidth=ciCharIndex->lpLine->nLineWidth;
      nStartChar=ciCharIndex->lpLine->nLineLen;
    }
    else if (dwThird <= dwFirst && dwThird <= dwSecond)
    {
      nOffset=ciCharIndex->nCharInLine - ae->ciCaretIndex.nCharInLine;
      nStringWidth=ae->ptCaret.x;
      nStartChar=ae->ciCaretIndex.nCharInLine;
    }

    Begin:
    if (nOffset > 0)
    {
      for (i=nStartChar; i < ciCharIndex->lpLine->nLineLen; ++i)
      {
        if (i == ciCharIndex->nCharInLine)
          break;

        if (ciCharIndex->lpLine->wpLine[i] == L'\t')
        {
          nTabWidth=ae->nTabWidth - nStringWidth % ae->nTabWidth;
          nStringWidth+=nTabWidth;
        }
        else
        {
          if (AE_GetTextExtentPoint32(ae, (wchar_t *)&ciCharIndex->lpLine->wpLine[i], 1, &sizeChar))
          {
            nStringWidth+=sizeChar.cx;
          }
          else return FALSE;
        }
      }

      if (ciCharIndex->nCharInLine > ciCharIndex->lpLine->nLineLen)
      {
        nStringWidth+=(ciCharIndex->nCharInLine - max(ciCharIndex->lpLine->nLineLen, nStartChar)) * ae->nSpaceCharWidth;
      }
    }
    else if (nOffset < 0)
    {
      if (nStartChar > ciCharIndex->lpLine->nLineLen)
      {
        nStringWidth-=(nStartChar - max(ciCharIndex->lpLine->nLineLen, ciCharIndex->nCharInLine)) * ae->nSpaceCharWidth;
      }

      if (ciCharIndex->nCharInLine < ciCharIndex->lpLine->nLineLen)
      {
        for (i=min(ciCharIndex->lpLine->nLineLen, nStartChar) - 1; i >= 0; --i)
        {
          if (ciCharIndex->lpLine->wpLine[i] == L'\t')
          {
            nOffset=ciCharIndex->nCharInLine - 0;
            nStringWidth=0;
            nStartChar=0;
            goto Begin;
          }
          else
          {
            if (AE_GetTextExtentPoint32(ae, (wchar_t *)&ciCharIndex->lpLine->wpLine[i], 1, &sizeChar))
            {
              nStringWidth-=sizeChar.cx;
            }
            else return FALSE;
          }

          if (i == ciCharIndex->nCharInLine)
            break;
        }
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

int AE_GetCharInLine(AKELEDIT *ae, const wchar_t *wpString, int nStringLen, int nMaxExtent, BOOL bHalfFit, int *nCharIndex, int *nCharPos, BOOL bColumnSel)
{
  SIZE sizeChar={0};
  int nStringWidth=0;
  int nTabWidth;
  int nHScrollMax;
  int i=0;

  if (wpString)
  {
    for (; i < nStringLen && nStringWidth < nMaxExtent; ++i)
    {
      if (wpString[i] == L'\t')
      {
        nTabWidth=ae->nTabWidth - nStringWidth % ae->nTabWidth;
        sizeChar.cx=nTabWidth;
        nStringWidth+=sizeChar.cx;
      }
      else
      {
        if (AE_GetTextExtentPoint32(ae, (wchar_t *)&wpString[i], 1, &sizeChar))
        {
          nStringWidth+=sizeChar.cx;
        }
        else return AEPC_ERROR;
      }
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

  if (i > 0)
  {
    if (nStringWidth > nMaxExtent)
    {
      nHScrollMax=max(ae->nHScrollMax, (ae->rcDraw.right - ae->rcDraw.left));

      if (bHalfFit && nStringWidth + sizeChar.cx <= nHScrollMax - ae->nAveCharWidth)
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

  if (nMaxExtent == nStringWidth)
    return AEPC_EQUAL;
  if (nMaxExtent > nStringWidth)
    return AEPC_BEFORE;
  return AEPC_AFTER;
}

int AE_GetCharInLineEx(AKELEDIT *ae, const AELINEDATA *lpLine, int nMaxExtent, BOOL bHalfFit, int *nCharIndex, int *nCharPos, BOOL bColumnSel)
{
  SIZE sizeChar={0};
  DWORD dwFirst=(DWORD)-1;
  DWORD dwSecond=(DWORD)-1;
  DWORD dwThird=(DWORD)-1;
  int nOffset=0;
  int nStringWidth=0;
  int nStartChar=0;
  int nTabWidth;
  int nHScrollMax;
  int i;

  if (lpLine)
  {
    dwFirst=mod(nMaxExtent - 0);
    if (lpLine->nLineWidth != -1)
      dwSecond=mod(nMaxExtent - lpLine->nLineWidth);
    if (ae->ptCaret.x && lpLine == ae->ciCaretIndex.lpLine)
      dwThird=mod(nMaxExtent - ae->ptCaret.x);

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
      for (i=nStartChar; nStringWidth < nMaxExtent; ++i)
      {
        if (i < lpLine->nLineLen)
        {
          if (lpLine->wpLine[i] == L'\t')
          {
            nTabWidth=ae->nTabWidth - nStringWidth % ae->nTabWidth;
            sizeChar.cx=nTabWidth;
            nStringWidth+=sizeChar.cx;
          }
          else
          {
            if (AE_GetTextExtentPoint32(ae, (wchar_t *)&lpLine->wpLine[i], 1, &sizeChar))
            {
              nStringWidth+=sizeChar.cx;
            }
            else return AEPC_ERROR;
          }
        }
        else
        {
          if (bColumnSel)
          {
            sizeChar.cx=ae->nSpaceCharWidth;
            nStringWidth+=sizeChar.cx;
          }
          else
          {
            nStringWidth=lpLine->nLineWidth;
            i=lpLine->nLineLen;
            goto End;
          }
        }
      }

      if (i > 0)
      {
        if (nStringWidth > nMaxExtent)
        {
          nHScrollMax=max(ae->nHScrollMax, (ae->rcDraw.right - ae->rcDraw.left));

          if (bHalfFit && nStringWidth + sizeChar.cx <= nHScrollMax - ae->nAveCharWidth)
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
    }
    else if (nOffset < 0)
    {
      for (i=nStartChar - 1; i >= 0 && nStringWidth > nMaxExtent; --i)
      {
        if (i < lpLine->nLineLen)
        {
          if (lpLine->wpLine[i] == L'\t')
          {
            nOffset=nMaxExtent - 0;
            nStringWidth=0;
            nStartChar=0;
            goto Begin;
          }
          else
          {
            if (AE_GetTextExtentPoint32(ae, (wchar_t *)&lpLine->wpLine[i], 1, &sizeChar))
            {
              nStringWidth-=sizeChar.cx;
            }
            else return AEPC_ERROR;
          }
        }
        else
        {
          if (bColumnSel)
          {
            sizeChar.cx=ae->nSpaceCharWidth;
            nStringWidth-=sizeChar.cx;
          }
          else
          {
            nStringWidth=lpLine->nLineWidth;
            i=lpLine->nLineLen;
            goto End;
          }
        }
      }
      ++i;

      if (nStringWidth < nMaxExtent)
      {
        if (bHalfFit)
        {
          if (nMaxExtent - nStringWidth >= sizeChar.cx / 2)
          {
            nStringWidth+=sizeChar.cx;
            ++i;
          }
        }
        else
        {
          nStringWidth+=sizeChar.cx;
          ++i;
        }
      }
    }
    else
    {
      i=nStartChar;

      if (!bColumnSel)
      {
        if (i >= lpLine->nLineLen)
        {
          nStringWidth=lpLine->nLineWidth;
          i=lpLine->nLineLen;
          goto End;
        }
      }
    }

    End:
    if (nCharPos) *nCharPos=nStringWidth;
    if (nCharIndex) *nCharIndex=i;

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
    if (!AE_GetCharInLineEx(ae, lpLine, nMinExtent, TRUE, &nMinIndex, &nMinPos, bColumnSel))
      return FALSE;
  }
  if (nMaxCharIndex || nMaxCharPos)
  {
    if (!AE_GetCharInLineEx(ae, lpLine, nMaxExtent, TRUE, &nMaxIndex, &nMaxPos, bColumnSel))
      return FALSE;
  }
  if (nMinCharPos) *nMinCharPos=nMinPos;
  if (nMaxCharPos) *nMaxCharPos=nMaxPos;
  if (nMinCharIndex) *nMinCharIndex=nMinIndex;
  if (nMaxCharIndex) *nMaxCharIndex=nMaxIndex;
  return TRUE;
}

int AE_GetCharFromPos(AKELEDIT *ae, POINT *ptClientPos, AECHARINDEX *ciCharIndex, POINT *ptGlobalPos, BOOL bColumnSel)
{
  int nCharPos;
  int nMaxExtent;
  int nFirstLine=0;
  int nLastLine=ae->nLineCount;
  int nHScrollMax;
  int nResult;

  ciCharIndex->nLine=(ae->nVScrollPos + (ptClientPos->y - ae->rcDraw.top)) / ae->nCharHeight;
  ciCharIndex->nLine=max(ciCharIndex->nLine, nFirstLine);
  ciCharIndex->nLine=min(ciCharIndex->nLine, nLastLine);
  ciCharIndex->nCharInLine=0;
  if (ptGlobalPos) ptGlobalPos->y=ciCharIndex->nLine * ae->nCharHeight;

  nMaxExtent=ae->nHScrollPos + (ptClientPos->x - ae->rcDraw.left);
  nHScrollMax=max(ae->nHScrollMax, (ae->rcDraw.right - ae->rcDraw.left));
  nMaxExtent=min(nHScrollMax - ae->nAveCharWidth, nMaxExtent);

  if (AE_UpdateIndex(ae, ciCharIndex))
  {
    if (nMaxExtent <= 0)
    {
      ciCharIndex->nCharInLine=0;
      if (ptGlobalPos) ptGlobalPos->x=0;

      if (nMaxExtent == 0)
        return AEPC_EQUAL;
      else
        return AEPC_AFTER;
    }
    if (!bColumnSel)
    {
      if (nMaxExtent >= ciCharIndex->lpLine->nLineWidth)
      {
        ciCharIndex->nCharInLine=ciCharIndex->lpLine->nLineLen;
        if (ptGlobalPos) ptGlobalPos->x=ciCharIndex->lpLine->nLineWidth;

        if (nMaxExtent == ciCharIndex->lpLine->nLineWidth)
          return AEPC_EQUAL;
        else
          return AEPC_BEFORE;
      }
    }
    if (nResult=AE_GetCharInLineEx(ae, ciCharIndex->lpLine, nMaxExtent, TRUE, &ciCharIndex->nCharInLine, &nCharPos, bColumnSel))
    {
      if (ptGlobalPos) ptGlobalPos->x=nCharPos;
      return nResult;
    }
  }
  return AEPC_ERROR;
}

BOOL AE_GetNextBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciNextBreak, BOOL bColumnSel, DWORD dwFlags)
{
  AECHARINDEX ciCount=*ciChar;
  AELINEDATA *lpNextLine=NULL;
  BOOL bInList;
  BOOL bIsSpacePrevious=FALSE;
  BOOL bIsSpaceCurrent;

  if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
  {
    if (ciCount.lpLine->next)
    {
      if (ciCount.lpLine->nLineBreak == AELB_WRAP)
      {
        ciCount.nLine+=1;
        ciCount.lpLine=ciCount.lpLine->next;
        ciCount.nCharInLine=0;
      }
    }
    else goto End;
  }

  if (ciCount.nCharInLine == ciCount.lpLine->nLineLen)
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, L'\n'))
      bIsSpacePrevious=TRUE;
  }
  else
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
      bIsSpacePrevious=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);
  }

  if (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
  {
    while (1)
    {
      lpNextLine=ciCount.lpLine->next;

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
        if (bInList != AE_IsInDelimiterList(ae->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
        {
          if (bInList)
          {
            if (!((dwFlags & AEWB_SKIPSPACEEND) && bIsSpacePrevious))
              goto End;
            bInList=FALSE;
          }
          else
          {
            if (!((dwFlags & AEWB_SKIPSPACESTART) && bIsSpaceCurrent))
              goto End;
            bInList=TRUE;
          }
        }
        bIsSpacePrevious=bIsSpaceCurrent;

        ++ciCount.nCharInLine;
      }
      if (bColumnSel) goto End;
      if (ciCount.lpLine->nLineBreak != AELB_WRAP)
      {
        if (bInList)
        {
          if (dwFlags & AEWB_STOPSPACESTART)
          {
            if (!bIsSpacePrevious)
              goto End;
          }
          bIsSpacePrevious=TRUE;
        }
        if (bInList != AE_IsInDelimiterList(ae->wszWordDelimiters, L'\n'))
          goto End;
      }

      if (lpNextLine)
      {
        ciCount.nLine+=1;
        ciCount.lpLine=lpNextLine;
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

BOOL AE_GetPrevBreak(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciPrevBreak, BOOL bColumnSel, DWORD dwFlags)
{
  AECHARINDEX ciCount=*ciChar;
  AELINEDATA *lpPrevLine=NULL;
  BOOL bInList;
  BOOL bIsSpacePrevious=FALSE;
  BOOL bIsSpaceCurrent;

  if (ciCount.nCharInLine - 1 < 0)
  {
    if (ciCount.lpLine->prev)
    {
      if (ciCount.lpLine->prev->nLineBreak == AELB_WRAP)
      {
        ciCount.nLine-=1;
        ciCount.lpLine=ciCount.lpLine->prev;
        ciCount.nCharInLine=ciCount.lpLine->nLineLen - 1;
      }
    }
    else goto End;
  }

  if (--ciCount.nCharInLine < 0)
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, L'\n'))
      bIsSpacePrevious=TRUE;
  }
  else
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
      bIsSpacePrevious=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);
  }

  if (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
  {
    while (1)
    {
      lpPrevLine=ciCount.lpLine->prev;

      while (ciCount.nCharInLine >= 0)
      {
        bIsSpaceCurrent=AE_IsSpace(ciCount.lpLine->wpLine[ciCount.nCharInLine]);

        if (bInList)
        {
          if (dwFlags & AEWB_STOPSPACEEND)
          {
            if (!bIsSpacePrevious && bIsSpaceCurrent)
            {
              ++ciCount.nCharInLine;
              goto End;
            }
          }
          if (dwFlags & AEWB_STOPSPACESTART)
          {
            if (bIsSpacePrevious && !bIsSpaceCurrent)
            {
              ++ciCount.nCharInLine;
              goto End;
            }
          }
        }
        if (bInList != AE_IsInDelimiterList(ae->wszWordDelimiters, ciCount.lpLine->wpLine[ciCount.nCharInLine]))
        {
          if (bInList)
          {
            if (!((dwFlags & AEWB_SKIPSPACESTART) && bIsSpacePrevious))
            {
              ++ciCount.nCharInLine;
              goto End;
            }
            bInList=FALSE;
          }
          else
          {
            if (!((dwFlags & AEWB_SKIPSPACEEND) && bIsSpaceCurrent))
            {
              ++ciCount.nCharInLine;
              goto End;
            }
            bInList=TRUE;
          }
        }
        bIsSpacePrevious=bIsSpaceCurrent;

        --ciCount.nCharInLine;
      }
      if (bColumnSel) goto End;
      if (lpPrevLine && lpPrevLine->nLineBreak != AELB_WRAP)
      {
        if (bInList)
        {
          if (dwFlags & AEWB_STOPSPACEEND)
          {
            if (!bIsSpacePrevious)
              goto End;
          }
          bIsSpacePrevious=TRUE;
        }
        if (bInList != AE_IsInDelimiterList(ae->wszWordDelimiters, L'\n'))
          goto End;
      }

      if (lpPrevLine)
      {
        ciCount.nLine-=1;
        ciCount.lpLine=lpPrevLine;
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

BOOL AE_GetNextWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel, DWORD dwFlags, BOOL bSearch)
{
  AECHARINDEX ciStart=*ciChar;
  AECHARINDEX ciEnd=*ciChar;
  BOOL bInList;

  if (ciEnd.nCharInLine == ciEnd.lpLine->nLineLen)
  {
    if (ciEnd.lpLine->next)
    {
      if (ciEnd.lpLine->nLineBreak == AELB_WRAP)
      {
        ciEnd.nLine+=1;
        ciEnd.lpLine=ciEnd.lpLine->next;
        ciEnd.nCharInLine=0;
      }
    }
    else return FALSE;
  }

  if (ciEnd.nCharInLine == ciEnd.lpLine->nLineLen)
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, L'\n'))
    {
      if (AE_GetNextBreak(ae, &ciEnd, &ciEnd, bColumnSel, dwFlags))
      {
        if (!bSearch && (dwFlags & AEWB_RIGHTWORDSTART))
        {
          if (ciWordStart) *ciWordStart=ciStart;
          if (ciWordEnd) *ciWordEnd=ciEnd;
          return TRUE;
        }
      }
      else return FALSE;
    }
  }
  else
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, ciEnd.lpLine->wpLine[ciEnd.nCharInLine]))
    {
      if (AE_GetNextBreak(ae, &ciEnd, &ciEnd, bColumnSel, dwFlags))
      {
        if (!bSearch && (dwFlags & AEWB_RIGHTWORDSTART))
        {
          if (ciWordStart) *ciWordStart=ciStart;
          if (ciWordEnd) *ciWordEnd=ciEnd;
          return TRUE;
        }
      }
      else return FALSE;
    }
    else
    {
      if (!bSearch)
      {
        if (AE_GetNextBreak(ae, &ciEnd, &ciEnd, bColumnSel, dwFlags))
        {
          if (dwFlags & AEWB_RIGHTWORDEND)
          {
            if (ciWordStart) *ciWordStart=ciStart;
            if (ciWordEnd) *ciWordEnd=ciEnd;
            return TRUE;
          }
        }
        else return FALSE;
      }
    }
  }

  ciStart=ciEnd;

  if (AE_GetNextBreak(ae, &ciEnd, &ciEnd, bColumnSel, dwFlags))
  {
    if (bSearch || (bInList && (dwFlags & AEWB_RIGHTWORDEND)) ||
                   (!bInList && (dwFlags & AEWB_RIGHTWORDSTART)))
    {
      if (ciWordStart) *ciWordStart=ciStart;
      if (ciWordEnd) *ciWordEnd=ciEnd;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_GetPrevWord(AKELEDIT *ae, const AECHARINDEX *ciChar, AECHARINDEX *ciWordStart, AECHARINDEX *ciWordEnd, BOOL bColumnSel, DWORD dwFlags, BOOL bSearch)
{
  AECHARINDEX ciStart=*ciChar;
  AECHARINDEX ciEnd=*ciChar;
  BOOL bInList;

  if (ciStart.nCharInLine - 1 < 0)
  {
    if (ciStart.lpLine->prev)
    {
      if (ciStart.lpLine->prev->nLineBreak == AELB_WRAP)
      {
        ciStart.nLine-=1;
        ciStart.lpLine=ciStart.lpLine->prev;
        ciStart.nCharInLine=ciStart.lpLine->nLineLen - 1;
      }
    }
    else return FALSE;
  }

  if (ciStart.nCharInLine - 1 < 0)
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, L'\n'))
    {
      if (AE_GetPrevBreak(ae, &ciStart, &ciStart, bColumnSel, dwFlags))
      {
        if (!bSearch && (dwFlags & AEWB_LEFTWORDEND))
        {
          if (ciWordStart) *ciWordStart=ciStart;
          if (ciWordEnd) *ciWordEnd=ciEnd;
          return TRUE;
        }
      }
      else return FALSE;
    }
  }
  else
  {
    if (bInList=AE_IsInDelimiterList(ae->wszWordDelimiters, ciStart.lpLine->wpLine[ciStart.nCharInLine - 1]))
    {
      if (AE_GetPrevBreak(ae, &ciStart, &ciStart, bColumnSel, dwFlags))
      {
        if (!bSearch && (dwFlags & AEWB_LEFTWORDEND))
        {
          if (ciWordStart) *ciWordStart=ciStart;
          if (ciWordEnd) *ciWordEnd=ciEnd;
          return TRUE;
        }
      }
      else return FALSE;
    }
    else
    {
      if (!bSearch)
      {
        if (AE_GetPrevBreak(ae, &ciStart, &ciStart, bColumnSel, dwFlags))
        {
          if (dwFlags & AEWB_LEFTWORDSTART)
          {
            if (ciWordStart) *ciWordStart=ciStart;
            if (ciWordEnd) *ciWordEnd=ciEnd;
            return TRUE;
          }
        }
        else return FALSE;
      }
    }
  }

  ciEnd=ciStart;

  if (AE_GetPrevBreak(ae, &ciStart, &ciStart, bColumnSel, dwFlags))
  {
    if (bSearch || (bInList && (dwFlags & AEWB_LEFTWORDSTART)) ||
                   (!bInList && (dwFlags & AEWB_LEFTWORDEND)))
    {
      if (ciWordStart) *ciWordStart=ciStart;
      if (ciWordEnd) *ciWordEnd=ciEnd;
      return TRUE;
    }
  }
  return FALSE;
}

BOOL AE_IsInDelimiterList(wchar_t *wpList, wchar_t c)
{
  if (AE_wcschr(wpList, c) != NULL)
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

  while (1)
  {
    if (!ae->wszUrlPrefixes[nEnd])
    {
      if (nStart == nEnd)
      {
        ae->lpUrlPrefixes[nPrefix]=NULL;
        break;
      }
      ae->lpUrlPrefixes[nPrefix++]=ae->wszUrlPrefixes + nStart;
      nStart=++nEnd;
      continue;
    }
    ++nEnd;
  }
  return nPrefix;
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
  AELINEDATA *lpElement;
  DWORD dwCopied=0;
  int nLineBreak;
  int nCharsToCopy;
  int i;

  if (ciStart.lpLine && ciEnd.lpLine)
  {
    if (wszBuffer)
    {
      if (dwBufferSize)
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
              AE_memcpy(wszBuffer + dwCopied, ciStart.lpLine->wpLine + ciStart.nCharInLine, nCharsToCopy * sizeof(wchar_t));
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
                  AE_memcpy(wszBuffer + dwCopied, ciEnd.lpLine->wpLine, nCharsToCopy * sizeof(wchar_t));
                  dwCopied+=nCharsToCopy;
                  break;
                }

                if (lpElement == ciStart.lpLine)
                {
                  //First line
                  nCharsToCopy=min((DWORD)(ciStart.lpLine->nLineLen - ciStart.nCharInLine), dwBufferSize - dwCopied);
                  AE_memcpy(wszBuffer + dwCopied, ciStart.lpLine->wpLine + ciStart.nCharInLine, nCharsToCopy * sizeof(wchar_t));
                  dwCopied+=nCharsToCopy;
                  if (dwCopied >= dwBufferSize) goto End;
                }
                else
                {
                  //Middle
                  nCharsToCopy=min((DWORD)lpElement->nLineLen, dwBufferSize - dwCopied);
                  AE_memcpy(wszBuffer + dwCopied, lpElement->wpLine, nCharsToCopy * sizeof(wchar_t));
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
    else dwCopied=AE_IndexSubtract(ae, &ciStart, &ciEnd, nNewLine, bColumnSel, bFillSpaces) + 1;
  }
  return dwCopied;
}

DWORD AE_SetTextAnsi(AKELEDIT *ae, int nCodePage, char *pText, DWORD dwTextLen, int nNewLine)
{
  wchar_t *wszText;
  DWORD dwUnicodeBytes;
  DWORD dwResult=0;

  dwUnicodeBytes=MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, NULL, 0) * sizeof(wchar_t);
  if (dwTextLen == (DWORD)-1) dwUnicodeBytes-=2;

  if (wszText=(wchar_t *)AE_HeapAlloc(NULL, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeBytes / sizeof(wchar_t));
    dwResult=AE_SetText(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), nNewLine);

    AE_HeapFree(NULL, 0, (LPVOID)wszText);
  }
  return dwResult;
}

DWORD AE_SetText(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, int nNewLine)
{
  AECHARINDEX ciCaretChar;
  AELINEDATA *lpElement=NULL;
  wchar_t *wpLineStart=wpText;
  wchar_t *wpLineEnd=wpText;
  HANDLE hHeap=ae->hHeap;
  DWORD dwTextCount=0;
  int nLinesInPage;
  int nProgressStep=1000;
  BOOL bUpdated=FALSE;

  //Free memory
  if (ae->hHeap)
  {
    if (HeapDestroy(ae->hHeap))
      ae->hHeap=NULL;

    ae->hUndoStack.first=0;
    ae->hUndoStack.last=0;
    ae->hLinesStack.first=0;
    ae->hLinesStack.last=0;
    ae->nLineCount=0;
    ae->nHScrollPos=0;
    ae->nVScrollPos=0;
    ae->nHScrollMax=0;
    ae->nVScrollMax=0;
    ae->hPointsStack.first=0;
    ae->hPointsStack.last=0;
    ae->lpCurrentUndo=NULL;
    ae->bSavePointExist=TRUE;
    ae->bModified=FALSE;
    ae->dwUndoCount=0;
    ae->liFirstDrawLine.nLine=0;
    ae->liFirstDrawLine.lpLine=NULL;
    ae->liMaxWidthLine.nLine=0;
    ae->liMaxWidthLine.lpLine=NULL;
    ae->ptCaret.x=0;
    ae->ptCaret.y=0;
    ae->nHorizCaretPos=0;
    ae->ciCaretIndex.nLine=0;
    ae->ciCaretIndex.nCharInLine=0;
    ae->ciCaretIndex.lpLine=NULL;
    ae->ciSelStartIndex.nLine=0;
    ae->ciSelStartIndex.nCharInLine=0;
    ae->ciSelStartIndex.lpLine=NULL;
    ae->ciSelEndIndex.nLine=0;
    ae->ciSelEndIndex.nCharInLine=0;
    ae->ciSelEndIndex.lpLine=NULL;
    ae->ciLastCallIndex.nLine=0;
    ae->ciLastCallIndex.nCharInLine=0;
    ae->ciLastCallIndex.lpLine=NULL;
    ae->nLastCharOffset=0;
    ae->nFirstDrawLineOffset=0;
    ae->nSelStartCharOffset=0;
    ae->nSelEndCharOffset=0;
    ae->nLastCallOffset=0;
  }
  ae->hHeap=HeapCreate(0, 0, 0);

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

  //Get lines in page
  nLinesInPage=(ae->rcDraw.bottom - ae->rcDraw.top) / ae->nCharHeight;

  //Parse text
  if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);

  while (dwTextCount < dwTextLen)
  {
    if (ae->dwEventMask & AENM_PROGRESS)
    {
      if (!(ae->nLineCount % nProgressStep))
      {
        if (AE_NotifyProgress(ae, AEPGS_SETTEXT, &nProgressStep, dwTextCount, dwTextLen))
          break;
      }
    }

    if (lpElement=AE_StackLineAdd(ae))
    {
      wpLineEnd=AE_GetNextLine(ae, wpLineStart, dwTextLen - dwTextCount, &lpElement->nLineLen, &lpElement->nLineBreak);
      dwTextCount+=wpLineEnd - wpLineStart;
      ae->nLastCharOffset+=lpElement->nLineLen + 1;

      if (lpElement->nLineBreak != AELB_EOF)
      {
        if (nNewLine != AELB_ASIS)
          lpElement->nLineBreak=nNewLine;
      }
      else --ae->nLastCharOffset;

      if (lpElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpElement->nLineLen * sizeof(wchar_t) + 2))
      {
        AE_memcpy(lpElement->wpLine, wpLineStart, lpElement->nLineLen * sizeof(wchar_t));
        lpElement->wpLine[lpElement->nLineLen]=L'\0';
        lpElement->nLineWidth=-1;
        AE_GetLineWidth(ae, lpElement);
      }
      else break;
    }
    else break;

    if (!bUpdated)
    {
      if (ae->nLineCount > nLinesInPage)
      {
        ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
        AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
        ae->ciCaretIndex=ciCaretChar;
        ae->ciSelStartIndex=ciCaretChar;
        ae->ciSelEndIndex=ciCaretChar;

        if (!(ae->dwOptions & AECO_DISABLENOSCROLL))
          AE_UpdateScrollBars(ae, SB_VERT);

        if (ae->bWordWrap)
        {
          ae->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->bWordWrap);

          ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
          AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
          ae->ciCaretIndex=ciCaretChar;
          ae->ciSelStartIndex=ciCaretChar;
          ae->ciSelEndIndex=ciCaretChar;
        }
        InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        UpdateWindow(ae->hWndEdit);
        bUpdated=TRUE;

        //Restore variables
        ae->liFirstDrawLine.nLine=0;
        ae->liFirstDrawLine.lpLine=NULL;
        ae->ciLastCallIndex.nLine=0;
        ae->ciLastCallIndex.nCharInLine=0;
        ae->ciLastCallIndex.lpLine=NULL;
        ae->nFirstDrawLineOffset=0;
        ae->nLastCallOffset=0;
      }
    }
    wpLineStart=wpLineEnd;
    ++ae->nLineCount;
  }

  //End progress
  if (ae->dwEventMask & AENM_PROGRESS)
  {
    nProgressStep=0;
    AE_NotifyProgress(ae, AEPGS_SETTEXT, &nProgressStep, dwTextLen, dwTextLen);
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

        ++ae->nLineCount;
      }
      lpElement->nLineBreak=AELB_EOF;
    }
  }
  if (ae->nLineCount) --ae->nLineCount;

  ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
  AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
  ae->ciCaretIndex=ciCaretChar;
  ae->ciSelStartIndex=ciCaretChar;
  ae->ciSelEndIndex=ciCaretChar;

  if (hHeap)
  {
    if (ae->bWordWrap)
    {
      ae->nLineCount+=AE_WrapLines(ae, NULL, NULL, ae->bWordWrap);

      ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
      AE_GetIndex(ae, AEGI_FIRSTCHAR, NULL, &ciCaretChar, FALSE);
      ae->ciCaretIndex=ciCaretChar;
      ae->ciSelStartIndex=ciCaretChar;
      ae->ciSelEndIndex=ciCaretChar;

      AE_UpdateScrollBars(ae, SB_VERT);
      if (!bUpdated) InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    }
    else
    {
      AE_UpdateScrollBars(ae, SB_VERT);
      AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
      if (!bUpdated) InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
    }
    AE_SetModify(ae, FALSE, TRUE);
  }

  //Set caret position
  if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

  return dwTextLen;
}

void AE_AppendTextAnsi(AKELEDIT *ae, int nCodePage, char *pText, DWORD dwTextLen, BOOL bColumnSel)
{
  wchar_t *wszText;
  DWORD dwUnicodeBytes;

  dwUnicodeBytes=MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, NULL, 0) * sizeof(wchar_t);
  if (dwTextLen == (DWORD)-1) dwUnicodeBytes-=2;

  if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeBytes / sizeof(wchar_t));
    AE_AppendText(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), bColumnSel);

    AE_HeapFree(ae, 0, (LPVOID)wszText);
  }
}

void AE_AppendText(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, BOOL bColumnSel)
{
  AECHARINDEX ciCaretIndex;
  AECHARINDEX ciSelStartIndex;
  AECHARINDEX ciSelEndIndex;
  AECHARINDEX ciLastChar;
  int nSelStartCharOffset;
  int nSelEndCharOffset;

  AE_StackUndoGroupStop(ae);
  AE_GetIndex(ae, AEGI_LASTCHAR, NULL, &ciLastChar, FALSE);

  //Save selection points
  ciCaretIndex=ae->ciCaretIndex;
  ciSelStartIndex=ae->ciSelStartIndex;
  ciSelEndIndex=ae->ciSelEndIndex;
  nSelStartCharOffset=ae->nSelStartCharOffset;
  nSelEndCharOffset=ae->nSelEndCharOffset;

  AE_InsertText(ae, &ciLastChar, wpText, dwTextLen, ae->nInputNewLine, bColumnSel, AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, NULL, NULL);

  //Restore selection points
  AE_UpdateIndex(ae, &ciSelStartIndex);
  ae->nSelStartCharOffset=nSelStartCharOffset;
  ae->ciSelStartIndex=ciSelStartIndex;
  AE_UpdateIndex(ae, &ciSelEndIndex);
  ae->nSelEndCharOffset=nSelEndCharOffset;
  ae->ciSelEndIndex=ciSelEndIndex;
  AE_UpdateIndex(ae, &ciCaretIndex);
  AE_GetPosFromCharEx(ae, &ciCaretIndex, &ae->ptCaret, NULL);
  ae->ciCaretIndex=ciCaretIndex;

  AE_UpdateScrollBars(ae, SB_VERT);
  InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
  AE_StackUndoGroupStop(ae);
}

void AE_ReplaceSelAnsi(AKELEDIT *ae, int nCodePage, char *pText, DWORD dwTextLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  wchar_t *wszText;
  DWORD dwUnicodeBytes;

  dwUnicodeBytes=MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, NULL, 0) * sizeof(wchar_t);
  if (dwTextLen == (DWORD)-1) dwUnicodeBytes-=2;

  if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(nCodePage, 0, pText, dwTextLen, wszText, dwUnicodeBytes / sizeof(wchar_t));
    AE_ReplaceSel(ae, wszText, dwUnicodeBytes / sizeof(wchar_t), bColumnSel, ciInsertStart, ciInsertEnd);

    AE_HeapFree(ae, 0, (LPVOID)wszText);
  }
}

void AE_ReplaceSel(AKELEDIT *ae, wchar_t *wpText, DWORD dwTextLen, BOOL bColumnSel, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AECHARINDEX ciStart={0};
  AECHARINDEX ciEnd={0};

  AE_StackUndoGroupStop(ae);
  AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
  if (!AE_InsertText(ae, &ae->ciSelStartIndex, wpText, dwTextLen, ae->nInputNewLine, bColumnSel, 0, &ciStart, &ciEnd))
  {
    //Set caret position
    AE_ScrollToCaret(ae, &ae->ptCaret);
  }
  AE_StackUndoGroupStop(ae);

  if (bColumnSel)
  {
    if (ae->dwOptions & AECO_PASTESELECTCOLUMN)
      AE_SetSelectionPos(ae, &ciStart, &ciEnd, bColumnSel, 0);
  }
  if (ciInsertStart) *ciInsertStart=ciStart;
  if (ciInsertEnd) *ciInsertEnd=ciEnd;
}

void AE_DeleteTextRange(AKELEDIT *ae, const AECHARINDEX *ciRangeStart, const AECHARINDEX *ciRangeEnd, BOOL bColumnSel, DWORD dwDeleteFlags)
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
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  AEUNDOITEM *lpSetSelUndo=NULL;
  int nRichTextCount=0;
  int nLastLineSelStart=0;
  int nLineCount=0;
  int nWrapCount=0;
  int nHScrollPos;
  int nVScrollPos;
  int nStartOffset;
  int nEndOffset;
  int nExtraStartOffset;
  int nExtraEndOffset;
  int nLineOffset;
  int nFirstRedrawLine;
  int nLastRedrawLine;
  int nResult;

  AE_StackPointSetModify(ae, FALSE);

  if (ciRangeStart->lpLine && ciRangeEnd->lpLine)
  {
    AE_NotifySelChanging(ae);
    AE_NotifyTextChanging(ae);

    //Exchange indexes
    nResult=AE_IndexCompare(&ciDeleteStart, &ciDeleteEnd);

    if (nResult > 0)
    {
      ciTmp=ciDeleteStart;
      ciDeleteStart=ciDeleteEnd;
      ciDeleteEnd=ciTmp;
    }
    else if (nResult == 0) return;

    //Force left-top to right-bottom selection
    if (bColumnSel)
    {
      ciDeleteStart.nCharInLine=min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nSelStart);
      ciDeleteEnd.nCharInLine=max(ciDeleteEnd.nCharInLine, ciDeleteEnd.lpLine->nSelEnd);
    }

    nStartOffset=AE_AkelIndexToRichOffset(ae, &ciDeleteStart);
    nEndOffset=AE_AkelIndexToRichOffset(ae, &ciDeleteEnd);
    nLineOffset=nStartOffset - min(ciDeleteStart.nCharInLine, ciDeleteStart.lpLine->nLineLen);
    nExtraStartOffset=max(ciDeleteStart.nCharInLine - ciDeleteStart.lpLine->nLineLen, 0);
    nExtraEndOffset=max(ciDeleteEnd.nCharInLine - ciDeleteEnd.lpLine->nLineLen, 0);

    if (bColumnSel)
    {
      //Note:
      //If bColumnSel == TRUE, then text must be selected first before AE_DeleteTextRange call.

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
          nRichTextCount-=(lpElement->nLineLen - lpNewElement->nLineLen);

          if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
          {
            AE_memcpy(lpNewElement->wpLine, lpElement->wpLine, min(lpElement->nSelStart, lpNewElement->nLineLen) * sizeof(wchar_t));
            if (lpElement->nSelEnd < lpElement->nLineLen)
              AE_memcpy(lpNewElement->wpLine + min(lpElement->nSelStart, lpNewElement->nLineLen), lpElement->wpLine + lpElement->nSelEnd, (lpElement->nLineLen - lpElement->nSelEnd) * sizeof(wchar_t));
            lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';

            //Update points
            for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
            {
              if (!lpPoint->bModify)
              {
                if (lpPoint->ciPoint.lpLine == lpElement)
                {
                  lpPoint->ciPoint.lpLine=lpNewElement;

                  if (lpPoint->ciPoint.nCharInLine > lpElement->nSelStart &&
                      lpElement->nSelStart < lpElement->nLineLen)
                  {
                    lpPoint->ciPoint.nCharInLine-=(min(lpElement->nSelEnd, lpElement->nLineLen) - lpElement->nSelStart);
                    lpPoint->ciPoint.nCharInLine=max(lpPoint->ciPoint.nCharInLine, lpElement->nSelStart);
                  }
                  lpPoint->bModify=TRUE;
                }
              }
            }

            //Add undo
            if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
            {
              if (!ae->bLockCollectUndo)
              {
                if (ae->dwUndoLimit)
                {
                  AEUNDOITEM *lpUndoElement;
                  wchar_t *wpUndoText;
                  DWORD dwUndoTextLen=min(lpElement->nSelEnd, lpElement->nLineLen) - min(lpElement->nSelStart, lpElement->nLineLen);

                  if (dwUndoTextLen)
                  {
                    //Set selection
                    if (!lpSetSelUndo)
                    {
                      if (lpSetSelUndo=AE_StackUndoItemInsert(ae))
                      {
                        ae->lpCurrentUndo=lpSetSelUndo;
                      }
                      else break;
                    }

                    //Add text
                    if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
                    {
                      AE_memcpy(wpUndoText, lpElement->wpLine + lpElement->nSelStart, (min(lpElement->nSelEnd, lpElement->nLineLen) - lpElement->nSelStart) * sizeof(wchar_t));
                      wpUndoText[dwUndoTextLen]=L'\0';

                      if (lpUndoElement=AE_StackUndoItemInsert(ae))
                      {
                        lpUndoElement->dwFlags=AEUN_INSERT|AEUN_COLUMNGROUP;
                        lpUndoElement->nActionStartOffset=nLineOffset + lpElement->nSelStart;
                        lpUndoElement->nActionEndOffset=nLineOffset + min(lpElement->nSelEnd, lpElement->nLineLen);
                        lpUndoElement->wpText=wpUndoText;
                        lpUndoElement->dwTextLen=dwUndoTextLen;

                        ae->lpCurrentUndo=lpUndoElement;
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
          nLineOffset+=lpNewElement->nLineLen;
        else
          nLineOffset+=lpNewElement->nLineLen + 1;

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

      ciFirstChar.nLine=ciDeleteStart.nLine;
      ciFirstChar.lpLine=lpFirstElement;
      ciFirstChar.nCharInLine=ciDeleteStart.nCharInLine;
      ciLastChar.nLine=ciDeleteEnd.nLine;
      ciLastChar.lpLine=lpNewElement;
      ciLastChar.nCharInLine=nLastLineSelStart;

      //Update control points
      if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciDeleteStart.nLine)
        ae->nFirstDrawLineOffset+=nRichTextCount;
      if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciDeleteStart.nLine)
        ae->nLastCallOffset+=nRichTextCount;
      ae->nLastCharOffset+=nRichTextCount;

      //Set global
      nHScrollPos=ae->nHScrollPos;
      nVScrollPos=ae->nVScrollPos;
      AE_GetPosFromCharEx(ae, &ciFirstChar, &ae->ptCaret, NULL);
      ae->ciCaretIndex=ciFirstChar;
      ae->nSelStartCharOffset=nStartOffset;
      ae->ciSelStartIndex=ciFirstChar;
      ae->nSelEndCharOffset=nStartOffset;
      ae->ciSelEndIndex=ciFirstChar;

      //Post processing
      lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
      lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

      if (ae->bWordWrap)
      {
        AELINEINDEX liWrapStart;
        AELINEINDEX liWrapEnd;

        liWrapStart.nLine=ciFirstChar.nLine;
        liWrapStart.lpLine=ciFirstChar.lpLine;
        liWrapEnd.nLine=ciLastChar.nLine;
        liWrapEnd.lpLine=ciLastChar.lpLine;
        nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->bWordWrap);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;

        //Set redraw lines
        nFirstRedrawLine=liWrapStart.nLine;
        nLastRedrawLine=-1;

        //Set control points to "delete from" position
        AE_GetPosFromCharEx(ae, &ciFirstChar, &ae->ptCaret, NULL);
        ae->ciCaretIndex=ciFirstChar;
        ae->nSelStartCharOffset=nStartOffset;
        ae->ciSelStartIndex=ciFirstChar;
        ae->nSelEndCharOffset=nStartOffset;
        ae->ciSelEndIndex=ciFirstChar;

        //Update scroll bars
        ae->nLineCount+=nWrapCount;
        ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
        if (nWrapCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
          {
            AE_UpdateScrollBars(ae, SB_VERT);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
        }
      }
      else
      {
        //Set redraw lines
        nFirstRedrawLine=ciFirstChar.nLine;
        nLastRedrawLine=ciLastChar.nLine;

        //Update scroll bars
        if (!ae->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, FALSE);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;
      }
      AE_StackPointDelete(ae, lpPointOne);
      AE_StackPointDelete(ae, lpPointTwo);

      //Add undo
      if (nRichTextCount < 0)
      {
        if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
        {
          AE_SetModify(ae, TRUE, FALSE);

          if (!ae->bLockCollectUndo)
          {
            if (ae->dwUndoLimit)
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

                  ae->lpCurrentUndo=lpUndoElement;
                }
              }
            }
          }
        }
      }
    }
    else
    {
      //Add undo
      if (!(dwDeleteFlags & AEDELT_LOCKUNDO))
      {
        AE_SetModify(ae, TRUE, FALSE);

        if (!ae->bLockCollectUndo)
        {
          if (ae->dwUndoLimit)
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

                  ae->lpCurrentUndo=lpUndoElement;
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
        nRichTextCount+=lpNewElement->nLineLen;
        if (lpNewElement->nLineBreak != AELB_WRAP)
          ++nRichTextCount;

        if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
        {
          AE_memcpy(lpNewElement->wpLine, ciDeleteStart.lpLine->wpLine, ciDeleteStart.nCharInLine * sizeof(wchar_t));
          AE_memcpy(lpNewElement->wpLine + ciDeleteStart.nCharInLine, ciDeleteEnd.lpLine->wpLine + ciDeleteEnd.nCharInLine, (ciDeleteEnd.lpLine->nLineLen - ciDeleteEnd.nCharInLine) * sizeof(wchar_t));
          lpNewElement->wpLine[lpNewElement->nLineLen]=L'\0';
        }
      }

      //Update points
      for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
      {
        if (!lpPoint->bModify)
        {
          if (lpPoint->ciPoint.nLine >= ciDeleteStart.nLine)
          {
            if (lpPoint->ciPoint.nLine <= ciDeleteEnd.nLine)
            {
              if (lpPoint->ciPoint.nCharInLine > ciDeleteStart.nCharInLine)
              {
                if (lpPoint->ciPoint.nLine == ciDeleteEnd.nLine && lpPoint->ciPoint.nCharInLine > ciDeleteEnd.nCharInLine)
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
            lpPoint->bModify=TRUE;
          }
        }
      }

      //Delete lines in range
      lpElement=ciDeleteStart.lpLine;

      while (lpElement)
      {
        nRichTextCount-=lpElement->nLineLen;
        if (lpElement->nLineBreak != AELB_WRAP)
          --nRichTextCount;

        if (lpElement == ciDeleteEnd.lpLine)
        {
          AE_StackLineDelete(ae, lpElement);
          break;
        }
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
        ae->nFirstDrawLineOffset+=nRichTextCount;
      }
      if (ae->liMaxWidthLine.lpLine && ae->liMaxWidthLine.nLine > ciFirstChar.nLine)
      {
        ae->liMaxWidthLine.nLine+=nLineCount;
      }
      if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciFirstChar.nLine)
      {
        ae->ciLastCallIndex.nLine+=nLineCount;
        ae->nLastCallOffset+=nRichTextCount;
      }
      ae->nLastCharOffset+=nRichTextCount;

      //Set global
      nHScrollPos=ae->nHScrollPos;
      nVScrollPos=ae->nVScrollPos;
      ae->nLineCount+=nLineCount;
      ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
      AE_GetPosFromCharEx(ae, &ciFirstChar, &ae->ptCaret, NULL);
      ae->ciCaretIndex=ciFirstChar;
      ae->nSelStartCharOffset=nStartOffset;
      ae->ciSelStartIndex=ciFirstChar;
      ae->nSelEndCharOffset=nStartOffset;
      ae->ciSelEndIndex=ciFirstChar;

      //Post processing
      lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
      lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

      if (ae->bWordWrap)
      {
        AELINEINDEX liWrapStart;
        AELINEINDEX liWrapEnd;

        liWrapStart.nLine=ciFirstChar.nLine;
        liWrapStart.lpLine=ciFirstChar.lpLine;
        liWrapEnd.nLine=ciLastChar.nLine;
        liWrapEnd.lpLine=ciLastChar.lpLine;
        nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->bWordWrap);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;

        //Set redraw lines
        nFirstRedrawLine=liWrapStart.nLine;
        nLastRedrawLine=-1;

        //Set control points to "delete from" position
        AE_GetPosFromCharEx(ae, &ciFirstChar, &ae->ptCaret, NULL);
        ae->ciCaretIndex=ciFirstChar;
        ae->nSelStartCharOffset=nStartOffset;
        ae->ciSelStartIndex=ciFirstChar;
        ae->nSelEndCharOffset=nStartOffset;
        ae->ciSelEndIndex=ciFirstChar;

        //Update scroll bars
        ae->nLineCount+=nWrapCount;
        ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
        if (nLineCount + nWrapCount)
        {
          if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
          {
            AE_UpdateScrollBars(ae, SB_VERT);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
        }
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
        if (!ae->liMaxWidthLine.lpLine)
          AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
        else
          AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, FALSE);
        ciFirstChar=lpPointOne->ciPoint;
        ciLastChar=lpPointTwo->ciPoint;
      }
      AE_StackPointDelete(ae, lpPointOne);
      AE_StackPointDelete(ae, lpPointTwo);
    }

    if (!(dwDeleteFlags & AEDELT_LOCKSCROLL))
    {
      //Set caret position
      AE_ScrollToCaret(ae, &ae->ptCaret);
    }
    if (!(dwDeleteFlags & AEDELT_LOCKUPDATE))
    {
      //Set caret position
      ae->nHorizCaretPos=ae->ptCaret.x;
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

    AE_NotifySelChanged(ae);
    AE_NotifyTextChanged(ae);
  }
}

DWORD AE_InsertText(AKELEDIT *ae, const AECHARINDEX *ciInsertPos, wchar_t *wpText, DWORD dwTextLen, int nNewLine, BOOL bColumnSel, DWORD dwInsertFlags, AECHARINDEX *ciInsertStart, AECHARINDEX *ciInsertEnd)
{
  AECHARINDEX ciInsertFrom=*ciInsertPos;
  AECHARINDEX ciFirstChar;
  AECHARINDEX ciLastChar;
  AELINEDATA *lpElement=NULL;
  AELINEDATA *lpNewElement=NULL;
  AELINEDATA *lpNextElement=NULL;
  AELINEDATA *lpInsertToElement=NULL;
  AEPOINT *lpPoint;
  AEPOINT *lpPointOne;
  AEPOINT *lpPointTwo;
  POINT ptInsertFrom;
  wchar_t *wpLineStart=wpText;
  wchar_t *wpLineEnd;
  int nLineLen=0;
  int nLineBreak;
  int nLineCount=0;
  int nWrapCount=0;
  int nCaretIndexInLine=0;
  int nSpaces=0;
  int nHScrollPos=0;
  int nVScrollPos=0;
  int nStartOffset;
  int nEndOffset;
  int nLineOffset;
  int nFirstRedrawLine=0;
  int nLastRedrawLine=0;
  int i;
  DWORD dwTextCount=0;
  DWORD dwRichTextCount=0;

  if (ciInsertFrom.lpLine)
  {
    if (dwTextLen == (DWORD)-1) dwTextLen=lstrlenW(wpText);

    if (dwTextLen)
    {
      AE_NotifySelChanging(ae);
      AE_NotifyTextChanging(ae);

      //Set new line
      if (nNewLine == AELB_ASINPUT)
        nNewLine=ae->nInputNewLine;
      else if (nNewLine == AELB_ASOUTPUT)
        nNewLine=ae->nOutputNewLine;

      nLineOffset=AE_AkelIndexToRichOffset(ae, &ciInsertFrom) - min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen);
      nStartOffset=nLineOffset + ciInsertFrom.nCharInLine;
      nEndOffset=nStartOffset;

      if (bColumnSel)
      {
        AE_GetPosFromCharEx(ae, &ciInsertFrom, &ptInsertFrom, NULL);
        ciFirstChar.nLine=ciInsertFrom.nLine;
        ciFirstChar.lpLine=NULL;
        ciFirstChar.nCharInLine=ciInsertFrom.nCharInLine;
        ciLastChar.nLine=ciInsertFrom.nLine;
        ciLastChar.lpLine=NULL;
        ciLastChar.nCharInLine=ciInsertFrom.nCharInLine;
        lpElement=ciInsertFrom.lpLine;

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
                AE_GetCharInLineEx(ae, lpElement, ptInsertFrom.x, TRUE, &ciInsertFrom.nCharInLine, NULL, bColumnSel);

              lpNewElement->nLineWidth=-1;
              if (lpElement->nLineBreak != AELB_EOF)
                lpNewElement->nLineBreak=lpElement->nLineBreak;
              else
                lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=lpElement->nLineLen + max(ciInsertFrom.nCharInLine - lpElement->nLineLen, 0) + nLineLen;
              nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
              {
                AE_memcpy(lpNewElement->wpLine, lpElement->wpLine, min(ciInsertFrom.nCharInLine, lpElement->nLineLen) * sizeof(wchar_t));

                if (ciInsertFrom.nCharInLine > lpElement->nLineLen)
                {
                  dwRichTextCount+=ciInsertFrom.nCharInLine - lpElement->nLineLen;

                  for (i=lpElement->nLineLen; i < ciInsertFrom.nCharInLine; ++i)
                    lpNewElement->wpLine[i]=L' ';
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                }
                else
                {
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine + nLineLen, lpElement->wpLine + ciInsertFrom.nCharInLine, (lpElement->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
                }

                //Update points
                for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
                {
                  if (!lpPoint->bModify)
                  {
                    if (lpPoint->ciPoint.lpLine == lpElement)
                    {
                      lpPoint->ciPoint.lpLine=lpNewElement;

                      if (lpPoint->ciPoint.nCharInLine >= ciInsertFrom.nCharInLine)
                      {
                        lpPoint->ciPoint.nCharInLine+=nLineLen;
                      }
                      lpPoint->bModify=TRUE;
                    }
                  }
                }

                //Add undo
                if (!(dwInsertFlags & AEINST_LOCKUNDO))
                {
                  if (!ae->bLockCollectUndo)
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
                          wpUndoText[i]=L' ';
                        AE_memcpy(wpUndoText + i, wpLineStart, nLineLen * sizeof(wchar_t));
                        wpUndoText[dwUndoTextLen]=L'\0';

                        if (lpUndoElement=AE_StackUndoItemInsert(ae))
                        {
                          lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                          lpUndoElement->nActionStartOffset=nLineOffset + min(ciInsertFrom.nCharInLine, lpElement->nLineLen);
                          lpUndoElement->nActionEndOffset=nLineOffset + ciInsertFrom.nCharInLine + nLineLen;
                          lpUndoElement->wpText=wpUndoText;
                          lpUndoElement->dwTextLen=dwUndoTextLen;

                          ae->lpCurrentUndo=lpUndoElement;
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

                  //Insert new line
                  if (!(dwInsertFlags & AEINST_LOCKUNDO))
                  {
                    if (!ae->bLockCollectUndo)
                    {
                      if (ae->dwUndoLimit)
                      {
                        AEUNDOITEM *lpUndoElement;
                        wchar_t *wpUndoText;
                        wchar_t *wpLineBreak;
                        DWORD dwUndoTextLen=AE_GetNewLineString(ae, lpNewElement->nLineBreak, &wpLineBreak);

                        if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
                        {
                          AE_memcpy(wpUndoText, wpLineBreak, dwUndoTextLen * sizeof(wchar_t));
                          wpUndoText[dwUndoTextLen]=L'\0';

                          if (lpUndoElement=AE_StackUndoItemInsert(ae))
                          {
                            lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                            lpUndoElement->nActionStartOffset=nLineOffset + lpNewElement->nLineLen;
                            lpUndoElement->nActionEndOffset=nLineOffset + lpNewElement->nLineLen + 1;
                            lpUndoElement->wpText=wpUndoText;
                            lpUndoElement->dwTextLen=dwUndoTextLen;

                            ae->lpCurrentUndo=lpUndoElement;
                          }
                          else break;
                        }
                        else break;
                      }
                    }
                  }
                }
              }
              lpElement=lpElement->next;
            }
            else
            {
              ciInsertFrom.nCharInLine=ptInsertFrom.x / ae->nSpaceCharWidth;

              lpNewElement->nLineWidth=-1;
              lpNewElement->nLineBreak=nLineBreak;
              lpNewElement->nLineLen=ciInsertFrom.nCharInLine + nLineLen;
              nCaretIndexInLine=ciInsertFrom.nCharInLine + nLineLen;

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
              {
                dwRichTextCount+=ciInsertFrom.nCharInLine;
                if (nLineBreak != AELB_EOF)
                  ++dwRichTextCount;

                for (i=0; i < ciInsertFrom.nCharInLine; ++i)
                  lpNewElement->wpLine[i]=L' ';
                AE_memcpy(lpNewElement->wpLine + ciInsertFrom.nCharInLine, wpLineStart, nLineLen * sizeof(wchar_t));

                //Add undo
                if (!(dwInsertFlags & AEINST_LOCKUNDO))
                {
                  if (!ae->bLockCollectUndo)
                  {
                    if (ae->dwUndoLimit)
                    {
                      AEUNDOITEM *lpUndoElement;
                      wchar_t *wpUndoText;
                      DWORD dwUndoTextLen=lpNewElement->nLineLen + 1;

                      if (nLineBreak == AELB_EOF)
                        --dwUndoTextLen;

                      if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUndoTextLen * sizeof(wchar_t) + 2))
                      {
                        nSpaces=ciInsertFrom.nCharInLine;

                        for (i=0; i < nSpaces; ++i)
                          wpUndoText[i]=L' ';
                        AE_memcpy(wpUndoText + i, wpLineStart, nLineLen * sizeof(wchar_t));
                        if (nLineBreak != AELB_EOF)
                          wpUndoText[lpNewElement->nLineLen]=L'\n';
                        wpUndoText[dwUndoTextLen]=L'\0';

                        if (lpUndoElement=AE_StackUndoItemInsert(ae))
                        {
                          lpUndoElement->dwFlags=AEUN_DELETE|AEUN_COLUMNGROUP;
                          lpUndoElement->nActionStartOffset=nLineOffset;
                          if (nLineBreak != AELB_EOF)
                            lpUndoElement->nActionEndOffset=nLineOffset + lpNewElement->nLineLen + 1;
                          else
                            lpUndoElement->nActionEndOffset=nLineOffset + lpNewElement->nLineLen;
                          lpUndoElement->wpText=wpUndoText;
                          lpUndoElement->dwTextLen=dwUndoTextLen;

                          ae->lpCurrentUndo=lpUndoElement;
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
              nLineOffset+=lpNewElement->nLineLen;
            else
              nLineOffset+=lpNewElement->nLineLen + 1;
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
          ae->nLastCharOffset+=dwRichTextCount;

          //Set global
          nHScrollPos=ae->nHScrollPos;
          nVScrollPos=ae->nVScrollPos;
          ae->nLineCount+=nLineCount;
          ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
          AE_GetPosFromCharEx(ae, &ciFirstChar, &ae->ptCaret, NULL);
          ae->ciCaretIndex=ciFirstChar;
          ae->nSelStartCharOffset=nStartOffset;
          ae->ciSelStartIndex=ciFirstChar;
          ae->nSelEndCharOffset=nStartOffset;
          ae->ciSelEndIndex=ciFirstChar;
          nEndOffset=nLineOffset + ciInsertFrom.nCharInLine + nLineLen;

          //Post processing
          lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
          lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

          if (ae->bWordWrap)
          {
            AELINEINDEX liWrapStart;
            AELINEINDEX liWrapEnd;

            liWrapStart.nLine=ciFirstChar.nLine;
            liWrapStart.lpLine=ciFirstChar.lpLine;
            liWrapEnd.nLine=ciLastChar.nLine;
            liWrapEnd.lpLine=ciLastChar.lpLine;
            nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->bWordWrap);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;

            //Set redraw lines
            nFirstRedrawLine=liWrapStart.nLine;
            nLastRedrawLine=-1;

            //Set control points to "insert from" position
            AE_GetPosFromCharEx(ae, &ciFirstChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciFirstChar;
            ae->nSelStartCharOffset=nStartOffset;
            ae->ciSelStartIndex=ciFirstChar;
            ae->nSelEndCharOffset=nStartOffset;
            ae->ciSelEndIndex=ciFirstChar;

            //Update scroll bars
            ae->nLineCount+=nWrapCount;
            ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
            if (nLineCount + nWrapCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATE))
              {
                AE_UpdateScrollBars(ae, SB_VERT);
                ciFirstChar=lpPointOne->ciPoint;
                ciLastChar=lpPointTwo->ciPoint;
              }
            }
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
            if (!ae->liMaxWidthLine.lpLine)
              AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
            else
              AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, FALSE);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
          AE_StackPointDelete(ae, lpPointOne);
          AE_StackPointDelete(ae, lpPointTwo);

          //Add undo
          if (!(dwInsertFlags & AEINST_LOCKUNDO))
          {
            AE_SetModify(ae, TRUE, FALSE);

            if (!ae->bLockCollectUndo)
            {
              if (ae->dwUndoLimit)
              {
                //Set selection
                {
                  AEUNDOITEM *lpUndoElement;

                  if (lpUndoElement=AE_StackUndoItemInsert(ae))
                  {
                    lpUndoElement->dwFlags=AEUN_SETSEL|AEUN_COLUMNSEL|AEUN_REDOONLY;
                    lpUndoElement->nActionStartOffset=nStartOffset;
                    lpUndoElement->nActionEndOffset=nEndOffset;
                    lpUndoElement->wpText=NULL;
                    lpUndoElement->dwTextLen=0;

                    ae->lpCurrentUndo=lpUndoElement;
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

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
              {
                AE_memcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine, min(ciInsertFrom.nCharInLine, ciInsertFrom.lpLine->nLineLen) * sizeof(wchar_t));

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

                if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
                {
                  AE_memcpy(lpNewElement->wpLine, wpLineStart, nLineLen * sizeof(wchar_t));
                  if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                    AE_memcpy(lpNewElement->wpLine + nLineLen, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
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

                if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
                {
                  AE_memcpy(lpNewElement->wpLine, wpLineStart, lpNewElement->nLineLen * sizeof(wchar_t));
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

                  if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
                  {
                    if (ciInsertFrom.nCharInLine < ciInsertFrom.lpLine->nLineLen)
                      AE_memcpy(lpNewElement->wpLine, ciInsertFrom.lpLine->wpLine + ciInsertFrom.nCharInLine, (ciInsertFrom.lpLine->nLineLen - ciInsertFrom.nCharInLine) * sizeof(wchar_t));
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

              if (lpNewElement->wpLine=(wchar_t *)AE_HeapAlloc(ae, 0, lpNewElement->nLineLen * sizeof(wchar_t) + 2))
              {
                AE_memcpy(lpNewElement->wpLine, wpLineStart, lpNewElement->nLineLen * sizeof(wchar_t));
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
          for (lpPoint=(AEPOINT *)ae->hPointsStack.last; lpPoint; lpPoint=lpPoint->prev)
          {
            if (!lpPoint->bModify)
            {
              if (lpPoint->ciPoint.nLine > ciInsertFrom.nLine)
              {
                lpPoint->ciPoint.nLine+=nLineCount;
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
              }
              lpPoint->bModify=TRUE;
            }
          }

          AE_StackLineDelete(ae, ciInsertFrom.lpLine);

          //Update control points
          if (ae->liFirstDrawLine.lpLine && ae->liFirstDrawLine.nLine > ciInsertFrom.nLine)
          {
            ae->liFirstDrawLine.nLine+=nLineCount;
            ae->nFirstDrawLineOffset+=dwRichTextCount;
          }
          if (ae->liMaxWidthLine.lpLine && ae->liMaxWidthLine.nLine > ciInsertFrom.nLine)
          {
            ae->liMaxWidthLine.nLine+=nLineCount;
          }
          if (ae->ciLastCallIndex.lpLine && ae->ciLastCallIndex.nLine > ciInsertFrom.nLine)
          {
            ae->ciLastCallIndex.nLine+=nLineCount;
            ae->nLastCallOffset+=dwRichTextCount;
          }
          ae->nLastCharOffset+=dwRichTextCount;

          //Set global
          nHScrollPos=ae->nHScrollPos;
          nVScrollPos=ae->nVScrollPos;
          ae->nLineCount+=nLineCount;
          ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
          ciLastChar.nLine=ciInsertFrom.nLine + nLineCount;
          ciLastChar.lpLine=lpNewElement;
          ciLastChar.nCharInLine=nCaretIndexInLine;
          nEndOffset=nStartOffset - nSpaces + dwRichTextCount;

          //Post processing
          lpPointOne=AE_StackPointInsert(ae, &ciFirstChar);
          lpPointTwo=AE_StackPointInsert(ae, &ciLastChar);

          if (ae->bWordWrap)
          {
            AELINEINDEX liWrapStart;
            AELINEINDEX liWrapEnd;

            liWrapStart.nLine=ciFirstChar.nLine;
            liWrapStart.lpLine=ciFirstChar.lpLine;
            liWrapEnd.nLine=ciLastChar.nLine;
            liWrapEnd.lpLine=ciLastChar.lpLine;
            nWrapCount=AE_WrapLines(ae, &liWrapStart, &liWrapEnd, ae->bWordWrap);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;

            //Set redraw lines
            nFirstRedrawLine=liWrapStart.nLine;
            nLastRedrawLine=-1;

            //Set control points to "insert to" position
            AE_GetPosFromCharEx(ae, &ciLastChar, &ae->ptCaret, NULL);
            ae->ciCaretIndex=ciLastChar;
            ae->nSelStartCharOffset=nEndOffset;
            ae->ciSelStartIndex=ciLastChar;
            ae->nSelEndCharOffset=nEndOffset;
            ae->ciSelEndIndex=ciLastChar;

            //Update scroll bars
            ae->nLineCount+=nWrapCount;
            ae->nVScrollMax=(ae->nLineCount + 1) * ae->nCharHeight;
            if (nLineCount + nWrapCount)
            {
              if (!(dwInsertFlags & AEINST_LOCKUPDATE))
              {
                AE_UpdateScrollBars(ae, SB_VERT);
                ciFirstChar=lpPointOne->ciPoint;
                ciLastChar=lpPointTwo->ciPoint;
              }
            }
          }
          else
          {
            //Set redraw lines
            nFirstRedrawLine=ciFirstChar.nLine;
            nLastRedrawLine=-1;
            if (!nLineCount && ciFirstChar.nLine == ciLastChar.nLine)
              nLastRedrawLine=ciLastChar.nLine;

            AE_GetPosFromCharEx(ae, &ciLastChar, &ae->ptCaret, NULL);
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
            if (!ae->liMaxWidthLine.lpLine)
              AE_CalcLinesWidth(ae, NULL, NULL, FALSE);
            else
              AE_CalcLinesWidth(ae, (AELINEINDEX *)&ciFirstChar, (AELINEINDEX *)&ciLastChar, FALSE);
            ciFirstChar=lpPointOne->ciPoint;
            ciLastChar=lpPointTwo->ciPoint;
          }
          AE_StackPointDelete(ae, lpPointOne);
          AE_StackPointDelete(ae, lpPointTwo);

          //Add undo
          if (!(dwInsertFlags & AEINST_LOCKUNDO))
          {
            AE_SetModify(ae, TRUE, FALSE);

            if (!ae->bLockCollectUndo)
            {
              if (ae->dwUndoLimit)
              {
                AEUNDOITEM *lpUndoElement;
                wchar_t *wpUndoText;

                if (nSpaces > 0)
                {
                  if (wpUndoText=(wchar_t *)AE_HeapAlloc(ae, 0, nSpaces * sizeof(wchar_t) + 2))
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
                    lpUndoElement->nActionStartOffset=nStartOffset;
                    lpUndoElement->nActionEndOffset=nEndOffset;
                    lpUndoElement->wpText=wpUndoText;
                    lpUndoElement->dwTextLen=dwTextLen;

                    ae->lpCurrentUndo=lpUndoElement;
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
        {
          ciInsertStart->nLine=ciFirstChar.nLine;
          ciInsertStart->lpLine=ciFirstChar.lpLine;
          ciInsertStart->nCharInLine=ciFirstChar.nCharInLine;
        }
        if (ciInsertEnd)
        {
          ciInsertEnd->nLine=ciLastChar.nLine;
          ciInsertEnd->lpLine=ciLastChar.lpLine;
          ciInsertEnd->nCharInLine=ciLastChar.nCharInLine;
        }

        if (!(dwInsertFlags & AEINST_LOCKSCROLL))
        {
          //Set caret position
          AE_ScrollToCaret(ae, &ae->ptCaret);
        }
        if (!(dwInsertFlags & AEINST_LOCKUPDATE))
        {
          //Set caret position
          ae->nHorizCaretPos=ae->ptCaret.x;
          if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);

          //Redraw lines
          if (nHScrollPos != ae->nHScrollPos || nVScrollPos != ae->nVScrollPos)
          {
            InvalidateRect(ae->hWndEdit, &ae->rcDraw, FALSE);
          }
          else
          {
            AE_RedrawLineRange(ae, nFirstRedrawLine, nLastRedrawLine, FALSE);
          }
        }

        AE_NotifySelChanged(ae);
        AE_NotifyTextChanged(ae);
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

DWORD AE_StreamIn(AKELEDIT *ae, DWORD dwFlags, AESTREAM *aes)
{
  wchar_t *wszBuf;
  int nNewLine=aes->nNewLine;
  DWORD dwBufLen=4096;
  DWORD dwBufDone;
  DWORD dwInsertFlags;
  DWORD dwResult=0;

  aes->dwError=0;

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

  //Set insert flags
  if (dwFlags & AESF_SELECTION)
    dwInsertFlags=AEINST_LOCKSCROLL|AEINST_LOCKUPDATE;
  else
    dwInsertFlags=AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATE;

  if (wszBuf=(wchar_t *)AE_HeapAlloc(NULL, 0, dwBufLen * sizeof(wchar_t) + 2))
  {
    if (dwFlags & AESF_SELECTION)
    {
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATE);
    }
    else AE_SetText(ae, L"", 0, nNewLine);

    while (1)
    {
      dwBufDone=0;

      if (aes->dwError=aes->lpCallback(aes->dwCookie, wszBuf, dwBufLen * sizeof(wchar_t), &dwBufDone)) break;
      if (!dwBufDone) break;
      dwResult+=dwBufDone;
      AE_InsertText(ae, &ae->ciSelStartIndex, wszBuf, dwBufDone / sizeof(wchar_t), nNewLine, aes->bColumnSel, dwInsertFlags, NULL, NULL);
    }

    if (dwFlags & AESF_SELECTION)
    {
      AE_StackUndoGroupStop(ae);
    }
    AE_UpdateScrollBars(ae, SB_VERT);
    AE_ScrollToCaret(ae, &ae->ptCaret);
    ae->nHorizCaretPos=ae->ptCaret.x;
    if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);

    AE_HeapFree(NULL, 0, (LPVOID)wszBuf);
  }
  return dwResult;
}

DWORD AE_StreamOut(AKELEDIT *ae, DWORD dwFlags, AESTREAM *aes)
{
  AECHARINDEX ciCount;
  AECHARINDEX ciEnd;
  wchar_t *wszBuf;
  int nNewLine=aes->nNewLine;
  int nLineBreak;
  DWORD dwBufLen=4096;
  DWORD dwBufCount=0;
  DWORD dwResult=0;

  aes->dwError=0;

  //Set new line
  if (nNewLine == AELB_ASINPUT)
    nNewLine=ae->nInputNewLine;
  else if (nNewLine == AELB_ASOUTPUT)
    nNewLine=ae->nOutputNewLine;

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

  if (wszBuf=(wchar_t *)AE_HeapAlloc(NULL, 0, dwBufLen * sizeof(wchar_t) + 2))
  {
    while (ciCount.lpLine)
    {
      if (aes->bColumnSel)
      {
        ciCount.nCharInLine=ciCount.lpLine->nSelStart;

        while (ciCount.nCharInLine < ciCount.lpLine->nLineLen && ciCount.nCharInLine < ciCount.lpLine->nSelEnd)
        {
          if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=ciCount.lpLine->wpLine[ciCount.nCharInLine++];
        }
        if (dwFlags & AESF_FILLSPACES)
        {
          while (ciCount.nCharInLine < ciCount.lpLine->nSelEnd)
          {
            if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
            wszBuf[dwBufCount++]=L' ';
            ++ciCount.nCharInLine;
          }
        }
      }
      else
      {
        while (ciCount.nCharInLine < ciCount.lpLine->nLineLen)
        {
          if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
          wszBuf[dwBufCount++]=ciCount.lpLine->wpLine[ciCount.nCharInLine++];
        }
      }
      if (ciCount.lpLine->nLineBreak == AELB_EOF) break;

      if (nNewLine == AELB_ASIS)
        nLineBreak=ciCount.lpLine->nLineBreak;
      else
        nLineBreak=nNewLine;

      if (nLineBreak == AELB_R)
      {
        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\r';
      }
      else if (nLineBreak == AELB_N)
      {
        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\n';
      }
      else if (nLineBreak == AELB_RN)
      {
        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\r';

        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\n';
      }
      else if (nLineBreak == AELB_RRN)
      {
        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\r';

        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\r';

        if (!AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult)) goto End;
        wszBuf[dwBufCount++]=L'\n';
      }
      ++ciCount.nLine;
      ciCount.lpLine=ciCount.lpLine->next;
      ciCount.nCharInLine=0;
    }
    AE_StreamOutHelper(aes, &ciCount, &ciEnd, wszBuf, dwBufLen, &dwBufCount, &dwResult);

    End:
    AE_HeapFree(NULL, 0, (LPVOID)wszBuf);
  }
  return dwResult;
}

BOOL AE_StreamOutHelper(AESTREAM *aes, AECHARINDEX *ciCount, AECHARINDEX *ciEnd, wchar_t *wszBuf, DWORD dwBufLen, DWORD *dwBufCount, DWORD *dwResult)
{
  DWORD dwBufDone=0;

  if (AE_IndexCompare(ciCount, ciEnd) >= 0)
  {
    if (*dwBufCount > 0)
    {
      wszBuf[*dwBufCount]=L'\0';
      if (aes->dwError=aes->lpCallback(aes->dwCookie, wszBuf, *dwBufCount * sizeof(wchar_t), &dwBufDone)) return FALSE;
      *dwResult+=dwBufDone;
      *dwBufCount=0;
    }
    return FALSE;
  }
  if (*dwBufCount >= dwBufLen)
  {
    wszBuf[dwBufLen]=L'\0';
    *dwBufCount=0;
    if (aes->dwError=aes->lpCallback(aes->dwCookie, wszBuf, dwBufLen * sizeof(wchar_t), &dwBufDone)) return FALSE;
    if (!dwBufDone) return FALSE;
    *dwResult+=dwBufDone;
  }
  return TRUE;
}

BOOL AE_FindTextAnsi(AKELEDIT *ae, int nCodePage, AEFINDTEXTA *ftA)
{
  AEFINDTEXTW ftW={0};
  wchar_t *wszText;
  DWORD dwUnicodeBytes;
  BOOL bResult=FALSE;

  if (ftA->dwTextLen == (DWORD)-1)
    ftA->dwTextLen=lstrlenA(ftA->pText);

  dwUnicodeBytes=MultiByteToWideChar(nCodePage, 0, ftA->pText, ftA->dwTextLen + 1, NULL, 0) * sizeof(wchar_t);

  if (wszText=(wchar_t *)AE_HeapAlloc(ae, 0, dwUnicodeBytes))
  {
    MultiByteToWideChar(nCodePage, 0, ftA->pText, ftA->dwTextLen + 1, wszText, dwUnicodeBytes / sizeof(wchar_t));

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

  if (ft->dwFlags & AEFR_WHOLEWORD)
  {
    if (ft->dwFlags & AEFR_DOWN)
    {
      while (1)
      {
        if (AE_GetNextWord(ae, &ciCount, &cr.ciMin, &cr.ciMax, ae->bColumnSel, AEWB_LEFTWORDSTART|AEWB_RIGHTWORDEND, TRUE))
        {
          if (AE_IndexCompare(&cr.ciMax, &ciCountEnd) >= 0)
            return FALSE;

          if (AE_IndexSubtract(ae, &cr.ciMin, &cr.ciMax, ft->nNewLine, FALSE, FALSE) == ft->dwTextLen)
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
        if (AE_GetPrevWord(ae, &ciCount, &cr.ciMin, &cr.ciMax, ae->bColumnSel, AEWB_LEFTWORDSTART|AEWB_RIGHTWORDEND, TRUE))
        {
          if (AE_IndexCompare(&cr.ciMin, &ciCountEnd) < 0)
            return FALSE;

          if (AE_IndexSubtract(ae, &cr.ciMin, &cr.ciMax, ft->nNewLine, FALSE, FALSE) == ft->dwTextLen)
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
          while (ciCount.nCharInLine <= ciCount.lpLine->nLineLen)
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
            ciCount.nCharInLine=ciCount.lpLine->nLineLen;
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
        if (!*++wpStrCount)
        {
          ++ciCount.nCharInLine;
          goto Founded;
        }
      }
      else return FALSE;
    }
    if (ciCount.lpLine->nLineBreak == AELB_EOF) return FALSE;

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

    if (ciCount.lpLine->next)
    {
      ciCount.nLine+=1;
      ciCount.lpLine=ciCount.lpLine->next;
      ciCount.nCharInLine=0;
    }
    else return FALSE;

    if (nLineBreak == AELB_R)
    {
      if (*wpStrCount++ != L'\r') return FALSE;
    }
    else if (nLineBreak == AELB_N)
    {
      if (*wpStrCount++ != L'\n') return FALSE;
    }
    else if (nLineBreak == AELB_RN)
    {
      if (*wpStrCount++ != L'\r') return FALSE;
      if (*wpStrCount++ != L'\n') return FALSE;
    }
    else if (nLineBreak == AELB_RRN)
    {
      if (*wpStrCount++ != L'\r') return FALSE;
      if (*wpStrCount++ != L'\n') return FALSE;
    }
    if (!*wpStrCount) goto Founded;
  }

  Founded:
  ft->crFound.ciMin=*ciChar;
  ft->crFound.ciMax.nLine=ciCount.nLine;
  ft->crFound.ciMax.lpLine=ciCount.lpLine;
  ft->crFound.ciMax.nCharInLine=ciCount.nCharInLine;
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

void AE_SetModify(AKELEDIT *ae, BOOL bState, BOOL bMessage)
{
  if (ae->bModified != bState)
  {
    ae->bModified=!ae->bModified;

    if (bMessage)
    {
      if (ae->bModified)
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
    }

    //Send AEN_MODIFY
    if (ae->dwEventMask & AENM_MODIFY)
    {
      AENMODIFY aenm;

      aenm.hdr.hwndFrom=ae->hWndEdit;
      aenm.hdr.idFrom=ae->nEditCtrlID;
      aenm.hdr.code=AEN_MODIFY;
      aenm.bModified=ae->bModified;
      SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&aenm);
    }
  }
}

void AE_EmptyUndoBuffer(AKELEDIT *ae)
{
  if (!ae->lpSavePoint)
  {
    ae->lpSavePoint=NULL;
    ae->bSavePointExist=FALSE;
  }
  AE_StackRedoDeleteAll(ae, NULL);
  ae->lpCurrentUndo=NULL;
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
    AEUNDOITEM *lpCurElement;
    AEUNDOITEM *lpNextElement;
    AECHARINDEX ciActionStart;
    AECHARINDEX ciActionEnd;
    AECHARINDEX ciInsertStart;
    AECHARINDEX ciInsertEnd;
    BOOL bColumnSel;

    AE_StackUndoGroupStop(ae);
    lpCurElement=ae->lpCurrentUndo;

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
          AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
          AE_InsertText(ae, &ciActionStart, lpCurElement->wpText, lpCurElement->dwTextLen, ae->nInputNewLine, bColumnSel, AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, &ciInsertStart, &ciInsertEnd);

          if (!lpNextElement || (lpNextElement->dwFlags & AEUN_STOPGROUP) || !(lpNextElement->dwFlags & AEUN_INSERT))
          {
            if (lpCurElement->dwFlags & AEUN_BACKSPACEKEY)
              AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
            else if (lpCurElement->dwFlags & AEUN_DELETEKEY)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
            else if (lpCurElement->dwFlags & AEUN_OVERTYPECHAR)
              AE_SetSelectionPos(ae, &ciInsertStart, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
            else
              AE_SetSelectionPos(ae, &ciInsertEnd, &ciInsertStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
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
            AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
          else
            AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
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

          AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
        }

        //Update window
        if (!lpNextElement || (lpNextElement->dwFlags & AEUN_STOPGROUP))
        {
          AE_UpdateScrollBars(ae, SB_VERT);
          AE_ScrollToCaret(ae, &ae->ptCaret);
          ae->nHorizCaretPos=ae->ptCaret.x;
          if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        }
      }
      lpCurElement=lpNextElement;

      //Stop undo
      ae->lpCurrentUndo=lpCurElement;
      if (ae->lpCurrentUndo)
        if (ae->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
          break;
    }

    if (ae->bModified != AE_GetModify(ae))
    {
      AE_SetModify(ae, !ae->bModified, FALSE);
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditRedo(AKELEDIT *ae)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AEUNDOITEM *lpCurElement=ae->lpCurrentUndo;
    AEUNDOITEM *lpNextElement;
    AECHARINDEX ciActionStart;
    AECHARINDEX ciActionEnd;
    AECHARINDEX ciInsertStart;
    AECHARINDEX ciInsertEnd;
    BOOL bColumnSel;

    if (!lpCurElement)
      lpCurElement=(AEUNDOITEM *)ae->hUndoStack.first;
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
            AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
          else
            AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
          AE_DeleteTextRange(ae, &ciActionStart, &ciActionEnd, bColumnSel, AEDELT_LOCKUNDO|AEDELT_LOCKSCROLL|AEDELT_LOCKUPDATE);
        }
        else if (lpCurElement->dwFlags & AEUN_DELETE)
        {
          AE_RichOffsetToAkelIndex(ae, lpCurElement->nActionStartOffset, &ciActionStart);
          if (lpCurElement->dwFlags & AEUN_EXTRAOFFSET)
            ciActionStart.nCharInLine+=lpCurElement->nExtraStartOffset;

          AE_SetSelectionPos(ae, &ciActionStart, &ciActionStart, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
          AE_InsertText(ae, &ciActionStart, lpCurElement->wpText, lpCurElement->dwTextLen, ae->nInputNewLine, bColumnSel, AEINST_LOCKUNDO|AEINST_LOCKSCROLL|AEINST_LOCKUPDATE, &ciInsertStart, &ciInsertEnd);
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

          AE_SetSelectionPos(ae, &ciActionStart, &ciActionEnd, bColumnSel, AESELT_LOCKNOTIFY|AESELT_LOCKSCROLL|AESELT_LOCKUPDATE);
        }

        //Update window
        if (!lpNextElement || (lpCurElement->dwFlags & AEUN_STOPGROUP))
        {
          AE_UpdateScrollBars(ae, SB_VERT);
          AE_ScrollToCaret(ae, &ae->ptCaret);
          ae->nHorizCaretPos=ae->ptCaret.x;
          if (ae->bFocus) AE_SetCaretPos(ae, &ae->ptCaret);
          InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
        }
      }

      //Stop undo
      ae->lpCurrentUndo=lpCurElement;
      if (ae->lpCurrentUndo->dwFlags & AEUN_STOPGROUP)
        break;

      lpCurElement=lpNextElement;
    }

    if (ae->bModified != AE_GetModify(ae))
    {
      AE_SetModify(ae, !ae->bModified, FALSE);
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
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
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
  DWORD dwAnsiBytes;
  DWORD dwUnicodeLen;

  if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (OpenClipboard(NULL))
    {
      EmptyClipboard();

      if (dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, ae->nOutputNewLine, ae->bColumnSel, TRUE))
      {
        if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, dwUnicodeLen * sizeof(wchar_t)))
        {
          if (pDataTargetW=GlobalLock(hDataTargetW))
          {
            dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)pDataTargetW, (DWORD)-1, ae->nOutputNewLine, ae->bColumnSel, TRUE);

            //Get Ansi text
            dwAnsiBytes=WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen + 1, NULL, 0, NULL, NULL);

            if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, dwAnsiBytes))
            {
              if (pDataTargetA=GlobalLock(hDataTargetA))
              {
                WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen + 1, (char *)pDataTargetA, dwAnsiBytes, NULL, NULL);
                GlobalUnlock(hDataTargetA);
              }
            }
            GlobalUnlock(hDataTargetW);
          }
        }
      }
      if (hDataTargetW) SetClipboardData(CF_UNICODETEXT, hDataTargetW);
      if (hDataTargetA) SetClipboardData(CF_TEXT, hDataTargetA);
      if (ae->bColumnSel) SetClipboardData(cfAkelEditColumnSel, NULL);
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

    bColumnSel=IsClipboardFormatAvailable(cfAkelEditColumnSel);

    if (OpenClipboard(NULL))
    {
      if (!bAnsi && (hData=GetClipboardData(CF_UNICODETEXT)))
      {
        if (pData=GlobalLock(hData))
        {
          AE_ReplaceSel(ae, (wchar_t *)pData, (DWORD)-1, bColumnSel, NULL, NULL);
          GlobalUnlock(hData);
        }
      }
      else if (hData=GetClipboardData(CF_TEXT))
      {
        if (pData=GlobalLock(hData))
        {
          AE_ReplaceSelAnsi(ae, CP_ACP, (char *)pData, (DWORD)-1, bColumnSel, NULL, NULL);
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
            AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
            ae->lpCurrentUndo->dwFlags|=AEUN_OVERTYPECHAR;
          }
        }
      }
      MultiByteToWideChar(CP_ACP, 0, &chChar, 1, &wchChar, 1);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
      AE_InsertText(ae, &ae->ciSelStartIndex, &wchChar, 1, ae->nInputNewLine, FALSE, 0, NULL, NULL);

      if (!ae->bLockCollectUndo)
      {
        if (ae->dwUndoLimit)
        {
          if (ae->dwOptions & AECO_DETAILEDUNDO)
            AE_StackUndoGroupStop(ae);
          else
            ae->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
        }
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
            AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
            ae->lpCurrentUndo->dwFlags|=AEUN_OVERTYPECHAR;
          }
        }
      }
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
      AE_InsertText(ae, &ae->ciSelStartIndex, &wchChar, 1, ae->nInputNewLine, FALSE, 0, NULL, NULL);

      if (!ae->bLockCollectUndo)
      {
        if (ae->dwUndoLimit)
        {
          if (ae->dwOptions & AECO_DETAILEDUNDO)
            AE_StackUndoGroupStop(ae);
          else
            ae->lpCurrentUndo->dwFlags|=AEUN_SINGLECHAR;
        }
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
    AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
    AE_InsertText(ae, &ae->ciSelStartIndex, wpNewLine, nNewLine, ae->nInputNewLine, FALSE, 0, NULL, NULL);

    if (!ae->bLockCollectUndo)
    {
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

void AE_EditKeyBackspace(AKELEDIT *ae, BOOL bControl)
{
  if (!(ae->dwOptions & AECO_READONLY))
  {
    AECHARINDEX ciCharIndex=ae->ciCaretIndex;

    if (!AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
    {
      if (ae->bColumnSel)
      {
        if (ciCharIndex.nCharInLine > ciCharIndex.lpLine->nLineLen)
        {
          ciCharIndex.nCharInLine=ciCharIndex.lpLine->nLineLen;
          AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, ae->bColumnSel, 0);
          return;
        }
      }

      if ((bControl && AE_GetPrevWord(ae, &ae->ciSelStartIndex, &ciCharIndex, NULL, FALSE, ae->dwWordBreak, FALSE)) ||
          (!bControl && AE_GetIndex(ae, AEGI_PREVCHAR, &ae->ciSelStartIndex, &ciCharIndex, FALSE)))
      {
        AE_StackUndoGroupStop(ae);
        AE_DeleteTextRange(ae, &ciCharIndex, &ae->ciSelStartIndex, FALSE, 0);
        AE_StackUndoGroupStop(ae);

        if (!ae->bLockCollectUndo)
        {
          if (ae->dwUndoLimit)
          {
            ae->lpCurrentUndo->dwFlags|=AEUN_BACKSPACEKEY;
          }
        }
      }
      else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
    }
    else
    {
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
      AE_StackUndoGroupStop(ae);
    }
  }
  else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
}

void AE_EditKeyDelete(AKELEDIT *ae, BOOL bControl)
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
              wpSpaces[i]=L' ';
            wpSpaces[nSpaces]=L'\0';

            ciCharIndex.nCharInLine=min(ciCharIndex.nCharInLine, ciCharIndex.lpLine->nLineLen);
            AE_InsertText(ae, &ciCharIndex, wpSpaces, nSpaces, AELB_ASIS, FALSE, 0, NULL, NULL);

            AE_HeapFree(ae, 0, (LPVOID)wpSpaces);
          }
        }
      }

      if ((bControl && AE_GetNextWord(ae, &ae->ciSelStartIndex, NULL, &ciCharIndex, FALSE, ae->dwWordBreak, FALSE)) ||
          (!bControl && AE_GetIndex(ae, AEGI_NEXTCHAR, &ae->ciSelStartIndex, &ciCharIndex, FALSE)))
      {
        AE_StackUndoGroupStop(ae);
        AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ciCharIndex, FALSE, 0);
        AE_StackUndoGroupStop(ae);

        if (!ae->bLockCollectUndo)
        {
          if (ae->dwUndoLimit)
          {
            ae->lpCurrentUndo->dwFlags|=AEUN_DELETEKEY;
          }
        }
      }
      else if (!(ae->dwOptions & AECO_DISABLEBEEP)) MessageBeep(MB_OK);
    }
    else
    {
      AE_StackUndoGroupStop(ae);
      AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, 0);
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
  AE_SetSelectionPos(ae, &ciLastChar, &ciFirstChar, FALSE, 0);
}

void AE_AkelEditGetSel(AKELEDIT *ae, AESELECTION *aes, AECHARINDEX *lpciCaret)
{
  if (aes)
  {
    aes->crSel.ciMin=ae->ciSelStartIndex;
    aes->crSel.ciMax=ae->ciSelEndIndex;
    aes->bColumnSel=ae->bColumnSel;
  }
  if (lpciCaret)
  {
    *lpciCaret=ae->ciCaretIndex;
  }
}

void AE_AkelEditSetSel(AKELEDIT *ae, AESELECTION *aes, AECHARINDEX *lpciCaret)
{
  if (!lpciCaret)
  {
    if (!AE_IndexCompare(&ae->ciSelEndIndex, &ae->ciCaretIndex))
      AE_SetSelectionPos(ae, &aes->crSel.ciMax, &aes->crSel.ciMin, aes->bColumnSel, 0);
    else
      AE_SetSelectionPos(ae, &aes->crSel.ciMin, &aes->crSel.ciMax, aes->bColumnSel, 0);
  }
  else
  {
    if (!AE_IndexCompare(&aes->crSel.ciMax, lpciCaret))
      AE_SetSelectionPos(ae, &aes->crSel.ciMax, &aes->crSel.ciMin, aes->bColumnSel, 0);
    else
      AE_SetSelectionPos(ae, &aes->crSel.ciMin, &aes->crSel.ciMax, aes->bColumnSel, 0);
  }
}

void AE_RichEditGetSel(AKELEDIT *ae, LONG *nMin, LONG *nMax)
{
  *nMin=AE_AkelIndexToRichOffset(ae, &ae->ciSelStartIndex);
  *nMax=AE_AkelIndexToRichOffset(ae, &ae->ciSelEndIndex);
}

void AE_RichEditSetSel(AKELEDIT *ae, LONG nMin, LONG nMax, BOOL bColumnSel)
{
  AECHARRANGE cr;

  AE_RichOffsetToAkelIndex(ae, nMin, &cr.ciMin);
  AE_RichOffsetToAkelIndex(ae, nMax, &cr.ciMax);

  if (!AE_IndexCompare(&ae->ciSelEndIndex, &ae->ciCaretIndex))
    AE_SetSelectionPos(ae, &cr.ciMax, &cr.ciMin, bColumnSel, 0);
  else
    AE_SetSelectionPos(ae, &cr.ciMin, &cr.ciMax, bColumnSel, 0);
}

void AE_GetColors(AKELEDIT *ae, AECOLORS *aec)
{
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
  if (aec->dwFlags & AECLR_URLTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      aec->crUrlText=RGB(0x00, 0x00, 0xFF);
    else
      aec->crUrlText=ae->crUrlText;
  }
}

void AE_SetColors(AKELEDIT *ae, AECOLORS *aec)
{
  BOOL bUpdateDrawRect=FALSE;
  BOOL bUpdateEditRect=FALSE;

  if (aec->dwFlags & AECLR_CARET)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crCaret=RGB(0x00, 0x00, 0x00);
    else
      ae->crCaret=aec->crCaret;

    AE_UpdateCaret(ae, ae->bFocus, TRUE);
  }
  if (aec->dwFlags & AECLR_BASICTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crBasicText=GetSysColor(COLOR_WINDOWTEXT);
    else
      ae->crBasicText=aec->crBasicText;

    bUpdateDrawRect=TRUE;
  }
  if (aec->dwFlags & AECLR_BASICBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crBasicBk=GetSysColor(COLOR_WINDOW);
    else
      ae->crBasicBk=aec->crBasicBk;

    if (ae->hBasicBk) DeleteObject(ae->hBasicBk);
    ae->hBasicBk=CreateSolidBrush(ae->crBasicBk);
    bUpdateEditRect=TRUE;
  }
  if (aec->dwFlags & AECLR_SELTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
    else
      ae->crSelText=aec->crSelText;

    bUpdateDrawRect=TRUE;
  }
  if (aec->dwFlags & AECLR_SELBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crSelBk=GetSysColor(COLOR_HIGHLIGHT);
    else
      ae->crSelBk=aec->crSelBk;

    if (ae->hSelBk) DeleteObject(ae->hSelBk);
    ae->hSelBk=CreateSolidBrush(ae->crSelBk);
    bUpdateDrawRect=TRUE;
  }
  if (aec->dwFlags & AECLR_ACTIVELINETEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crActiveLineText=GetSysColor(COLOR_WINDOWTEXT);
    else
      ae->crActiveLineText=aec->crActiveLineText;

    bUpdateDrawRect=TRUE;
  }
  if (aec->dwFlags & AECLR_ACTIVELINEBK)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crActiveLineBk=GetSysColor(COLOR_WINDOW);
    else
      ae->crActiveLineBk=aec->crActiveLineBk;

    if (ae->hActiveLineBk) DeleteObject(ae->hActiveLineBk);
    ae->hActiveLineBk=CreateSolidBrush(ae->crActiveLineBk);
    bUpdateDrawRect=TRUE;
  }
  if (aec->dwFlags & AECLR_URLTEXT)
  {
    if (aec->dwFlags & AECLR_DEFAULT)
      ae->crUrlText=RGB(0x00, 0x00, 0xFF);
    else
      ae->crUrlText=aec->crUrlText;

    bUpdateDrawRect=TRUE;
  }

  if (bUpdateEditRect)
    InvalidateRect(ae->hWndEdit, &ae->rcEdit, TRUE);
  else if (bUpdateDrawRect)
    InvalidateRect(ae->hWndEdit, &ae->rcDraw, TRUE);
}

void AE_NotifyErrSpace(AKELEDIT *ae)
{
  //Send AEN_ERRSPACE
  {
    AENERRSPACE es;

    es.hdr.hwndFrom=ae->hWndEdit;
    es.hdr.idFrom=ae->nEditCtrlID;
    es.hdr.code=AEN_ERRSPACE;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&es);
  }

  //Send EN_ERRSPACE
  SendMessage(ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_ERRSPACE), (LPARAM)ae->hWndEdit);
}

void AE_NotifySetFocus(AKELEDIT *ae)
{
  //Send AEN_SETFOCUS
  {
    NMHDR hdr;

    hdr.hwndFrom=ae->hWndEdit;
    hdr.idFrom=ae->nEditCtrlID;
    hdr.code=AEN_SETFOCUS;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&hdr);
  }

  //Send EN_SETFOCUS
  SendMessage(ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_SETFOCUS), (LPARAM)ae->hWndEdit);
}

void AE_NotifyKillFocus(AKELEDIT *ae)
{
  //Send AEN_KILLFOCUS
  {
    NMHDR hdr;

    hdr.hwndFrom=ae->hWndEdit;
    hdr.idFrom=ae->nEditCtrlID;
    hdr.code=AEN_KILLFOCUS;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&hdr);
  }

  //Send EN_KILLFOCUS
  SendMessage(ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_KILLFOCUS), (LPARAM)ae->hWndEdit);
}

void AE_NotifySelChanging(AKELEDIT *ae)
{
  //Send AEN_SELCHANGING
  if (ae->dwEventMask & AENM_SELCHANGE)
  {
    AENSELCHANGE sc;

    sc.hdr.hwndFrom=ae->hWndEdit;
    sc.hdr.idFrom=ae->nEditCtrlID;
    sc.hdr.code=AEN_SELCHANGING;
    AE_AkelEditGetSel(ae, &sc.aes, &sc.ciCaret);
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }
}

void AE_NotifySelChanged(AKELEDIT *ae)
{
  //Send AEN_SELCHANGED
  if (ae->dwEventMask & AENM_SELCHANGE)
  {
    AENSELCHANGE sc;

    sc.hdr.hwndFrom=ae->hWndEdit;
    sc.hdr.idFrom=ae->nEditCtrlID;
    sc.hdr.code=AEN_SELCHANGED;
    AE_AkelEditGetSel(ae, &sc.aes, &sc.ciCaret);
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }

  //Send EN_SELCHANGE
  if (ae->dwRichEventMask & ENM_SELCHANGE)
  {
    SELCHANGE sc;

    sc.nmhdr.hwndFrom=ae->hWndEdit;
    sc.nmhdr.idFrom=ae->nEditCtrlID;
    sc.nmhdr.code=EN_SELCHANGE;
    sc.chrg.cpMin=ae->nSelStartCharOffset;
    sc.chrg.cpMax=ae->nSelEndCharOffset;
    sc.seltyp=0;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&sc);
  }
}

void AE_NotifyTextChanging(AKELEDIT *ae)
{
  //Send AEN_TEXTCHANGING
  if (ae->dwEventMask & AENM_TEXTCHANGE)
  {
    AENTEXTCHANGE tc;

    tc.hdr.hwndFrom=ae->hWndEdit;
    tc.hdr.idFrom=ae->nEditCtrlID;
    tc.hdr.code=AEN_TEXTCHANGING;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }
}

void AE_NotifyTextChanged(AKELEDIT *ae)
{
  //Send AEN_TEXTCHANGED
  if (ae->dwEventMask & AENM_TEXTCHANGE)
  {
    AENTEXTCHANGE tc;

    tc.hdr.hwndFrom=ae->hWndEdit;
    tc.hdr.idFrom=ae->nEditCtrlID;
    tc.hdr.code=AEN_TEXTCHANGED;
    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&tc);
  }

  //Send EN_CHANGE
  if (ae->dwRichEventMask & ENM_CHANGE)
  {
    SendMessage(ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_CHANGE), (LPARAM)ae->hWndEdit);
  }
}

BOOL AE_NotifyHScroll(AKELEDIT *ae, int nScrollOffset)
{
  BOOL bResult=FALSE;

  //Send AEN_HSCROLL
  if (ae->dwEventMask & AENM_SCROLL)
  {
    AENSCROLL aens;

    aens.hdr.hwndFrom=ae->hWndEdit;
    aens.hdr.idFrom=ae->nEditCtrlID;
    aens.hdr.code=AEN_HSCROLL;
    aens.nScrollPos=ae->nHScrollPos;
    aens.nScrollOffset=nScrollOffset;
    bResult=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&aens);
  }

  //Send EN_HSCROLL
  if (ae->dwRichEventMask & ENM_SCROLL)
  {
    SendMessage(ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_HSCROLL), (LPARAM)ae->hWndEdit);
  }
  return bResult;
}

BOOL AE_NotifyVScroll(AKELEDIT *ae, int nScrollOffset)
{
  BOOL bResult=FALSE;

  //Send AEN_VSCROLL
  if (ae->dwEventMask & AENM_SCROLL)
  {
    AENSCROLL aens;

    aens.hdr.hwndFrom=ae->hWndEdit;
    aens.hdr.idFrom=ae->nEditCtrlID;
    aens.hdr.code=AEN_VSCROLL;
    aens.nScrollPos=ae->nVScrollPos;
    aens.nScrollOffset=nScrollOffset;
    bResult=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&aens);
  }

  //Send EN_VSCROLL
  if (ae->dwRichEventMask & ENM_SCROLL)
  {
    SendMessage(ae->hWndParent, WM_COMMAND, MAKELONG(ae->nEditCtrlID, EN_VSCROLL), (LPARAM)ae->hWndEdit);
  }
  return bResult;
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

    if (!(bResult=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&mf)))
    {
      *wParam=mf.wParam;
      *lParam=mf.lParam;
    }
  }
  return bResult;
}

BOOL AE_NotifyDropFiles(AKELEDIT *ae, HDROP hDrop)
{
  BOOL bResult1=TRUE;
  BOOL bResult2=TRUE;

  //Send AEN_DROPFILES
  if (ae->dwEventMask & AENM_DROPFILES)
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
    df.hDrop=hDrop;
    df.ciChar=ciCharIndex;

    bResult1=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&df);
  }

  //Send EN_DROPFILES
  if (ae->dwRichEventMask & ENM_DROPFILES)
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

    bResult2=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&df);
  }

  if (!bResult1 || !bResult2)
    return FALSE;
  return TRUE;
}

void AE_NotifyDragDropDone(AKELEDIT *ae)
{
  //Send EN_DRAGDROPDONE
  if (ae->dwRichEventMask & ENM_DRAGDROPDONE)
  {
    NMHDR hdr;

    hdr.hwndFrom=ae->hWndEdit;
    hdr.idFrom=ae->nEditCtrlID;
    hdr.code=EN_DRAGDROPDONE;

    SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&hdr);
  }
}

BOOL AE_NotifyDropSource(AKELEDIT *ae, int nAction, DWORD *lpdwEffect, DWORD dwDropResult)
{
  BOOL bResult=FALSE;

  //Send AEN_DROPSOURCE
  if (ae->dwEventMask & AENM_DRAGDROP)
  {
    AENDROPSOURCE ds;

    ds.hdr.hwndFrom=ae->hWndEdit;
    ds.hdr.idFrom=ae->nEditCtrlID;
    ds.hdr.code=AEN_DROPSOURCE;
    ds.nAction=nAction;
    if (lpdwEffect)
      ds.dwEffect=*lpdwEffect;
    else
      ds.dwEffect=0;
    ds.dwDropResult=dwDropResult;

    bResult=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&ds);

    if (lpdwEffect) *lpdwEffect=ds.dwEffect;
  }
  return bResult;
}

BOOL AE_NotifyDropTarget(AKELEDIT *ae, int nAction, POINT *pt, DWORD *lpdwEffect)
{
  BOOL bResult=FALSE;

  //Send AEN_DROPTARGET
  if (ae->dwEventMask & AENM_DRAGDROP)
  {
    AENDROPTARGET dt;

    dt.hdr.hwndFrom=ae->hWndEdit;
    dt.hdr.idFrom=ae->nEditCtrlID;
    dt.hdr.code=AEN_DROPTARGET;
    dt.nAction=nAction;
    if (pt)
      dt.pt=*pt;
    else
      GetCursorPos(&dt.pt);
    if (lpdwEffect)
      dt.dwEffect=*lpdwEffect;
    else
      dt.dwEffect=0;

    bResult=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&dt);

    if (pt) *pt=dt.pt;
    if (lpdwEffect) *lpdwEffect=dt.dwEffect;
  }
  return bResult;
}

BOOL AE_NotifyLink(AKELEDIT *ae, UINT uMsg, WPARAM wParam, LPARAM lParam, AECHARRANGE *crLink)
{
  LRESULT lResult1=0;
  LRESULT lResult2=0;

  //Send AEN_LINK
  if (ae->dwEventMask & AENM_LINK)
  {
    AENLINK aenl;

    aenl.hdr.hwndFrom=ae->hWndEdit;
    aenl.hdr.idFrom=ae->nEditCtrlID;
    aenl.hdr.code=AEN_LINK;
    aenl.uMsg=uMsg;
    aenl.wParam=wParam;
    aenl.lParam=lParam;
    aenl.crLink=*crLink;
    lResult1=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&aenl);
  }

  //Send EN_LINK
  if (ae->dwRichEventMask & ENM_LINK)
  {
    ENLINK enl;

    enl.nmhdr.hwndFrom=ae->hWndEdit;
    enl.nmhdr.idFrom=ae->nEditCtrlID;
    enl.nmhdr.code=EN_LINK;
    enl.msg=uMsg;
    enl.wParam=wParam;
    enl.lParam=lParam;
    enl.chrg.cpMin=AE_AkelIndexToRichOffset(ae, &crLink->ciMin);
    enl.chrg.cpMax=AE_AkelIndexToRichOffset(ae, &crLink->ciMax);
    lResult2=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&enl);
  }

  if (lResult1 || lResult2)
    return TRUE;
  return FALSE;
}

BOOL AE_NotifyProgress(AKELEDIT *ae, DWORD dwType, int *nStep, int nCurrent, int nMaximum)
{
  BOOL bResult=FALSE;

  //Send AEN_PROGRESS
  AENPROGRESS aenp;

  aenp.hdr.hwndFrom=ae->hWndEdit;
  aenp.hdr.idFrom=ae->nEditCtrlID;
  aenp.hdr.code=AEN_PROGRESS;
  aenp.dwType=dwType;
  aenp.nStep=*nStep;
  aenp.nCurrent=nCurrent;
  aenp.nMaximum=nMaximum;
  bResult=SendMessage(ae->hWndParent, WM_NOTIFY, ae->nEditCtrlID, (LPARAM)&aenp);

  *nStep=aenp.nStep;

  return bResult;
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

int AE_WideStrCmpLen(const wchar_t *wpString, const wchar_t *wpString2, DWORD dwMaxLength)
{
  DWORD i;

  for (i=0; i < dwMaxLength && wpString[i] && wpString[i] == wpString2[i]; ++i);

  if (dwMaxLength == (DWORD)-1 && !wpString[i]) return 0;
  if (i >= dwMaxLength) return 0;
  if (wpString[i] == wpString2[i]) return 0;
  if ((DWORD)wpString[i] < (DWORD)wpString2[i])
    return -1;
  return 1;
}

int AE_WideStrCmpLenI(const wchar_t *wpString, const wchar_t *wpString2, DWORD dwMaxLength)
{
  DWORD i;

  for (i=0; i < dwMaxLength && wpString[i] && AE_WideCharUpper(wpString[i]) == AE_WideCharUpper(wpString2[i]); ++i);

  if (dwMaxLength == (DWORD)-1 && !wpString[i]) return 0;
  if (i >= dwMaxLength) return 0;
  if (wpString[i] == wpString2[i]) return 0;
  if ((DWORD)AE_WideCharUpper(wpString[i]) < (DWORD)AE_WideCharUpper(wpString2[i]))
    return -1;
  return 1;
}

void AE_ChangeByteOrder(unsigned char *lpBuffer, unsigned int nBufferLen)
{
  unsigned char ch;
  unsigned int a;
  unsigned int b;

  for (a=0, b=1; b < nBufferLen; a+=2, b+=2)
  {
    ch=lpBuffer[a];
    lpBuffer[a]=lpBuffer[b];
    lpBuffer[b]=ch;
  }
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


//// OLE Drag'n'Drop

HRESULT WINAPI AEIDropTarget_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj)
{
  if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
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

  if (!bNotify && !(ae->dwOptions & AECO_DISABLEDROP) && !(ae->dwOptions & AECO_READONLY))
  {
    fmtetc.cfFormat=CF_UNICODETEXT;
    fmtetc.ptd=0;
    fmtetc.dwAspect=DVASPECT_CONTENT;
    fmtetc.lindex=-1;
    fmtetc.tymed=TYMED_HGLOBAL;

    if (pDataObject->QueryGetData(&fmtetc) == S_OK)
    {
      pDropTarget->bAllowDrop=TRUE;
    }
    else
    {
      fmtetc.cfFormat=CF_TEXT;

      if (pDataObject->QueryGetData(&fmtetc) == S_OK)
        pDropTarget->bAllowDrop=TRUE;
      else
        pDropTarget->bAllowDrop=FALSE;
    }
  }
  else pDropTarget->bAllowDrop=FALSE;

  if (pDropTarget->bAllowDrop)
  {
    fmtetc.cfFormat=cfAkelEditColumnSel;

    if (pDataObject->QueryGetData(&fmtetc) == S_OK)
      pDropTarget->bColumnSel=TRUE;
    else
      pDropTarget->bColumnSel=FALSE;

    if (!ae->bFocus)
    {
      if (GetFocus())
        SetFocus(ae->hWndEdit);
      else
        AE_UpdateCaret(ae, TRUE, FALSE);
    }
    ae->bDropping=TRUE;
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
    AE_GetCharFromPos(ae, (POINT *)&pt, &ciCharIndex, NULL, pDropTarget->bColumnSel);

    fmtetc.cfFormat=CF_UNICODETEXT;
    fmtetc.ptd=0;
    fmtetc.dwAspect=DVASPECT_CONTENT;
    fmtetc.lindex=-1;
    fmtetc.tymed=TYMED_HGLOBAL;

    if (pDataObject->QueryGetData(&fmtetc) == S_OK)
    {
      if (pDataObject->GetData(&fmtetc, &stgmed) == S_OK)
      {
        if (pData=GlobalLock(stgmed.hGlobal))
        {
          AECHARINDEX ciStart={0};
          AECHARINDEX ciEnd={0};
          BOOL bSetSel=TRUE;

          AE_StackUndoGroupStop(ae);

          //Delete
          if (*pdwEffect & DROPEFFECT_MOVE)
          {
            if (ae->bDragging)
            {
              AEPOINT *lpPoint;

              lpPoint=AE_StackPointInsert(ae, &ciCharIndex);
              AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
              ciCharIndex=lpPoint->ciPoint;
              AE_StackPointDelete(ae, lpPoint);

              ae->bDeleteSelection=FALSE;
              bSetSel=FALSE;
            }
          }

          //Insert
          {
            if (bSetSel) AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, FALSE, 0);
            AE_InsertText(ae, &ciCharIndex, (wchar_t *)pData, (DWORD)-1, ae->nInputNewLine, pDropTarget->bColumnSel, 0, &ciStart, &ciEnd);
          }
          AE_StackUndoGroupStop(ae);
          AE_SetSelectionPos(ae, &ciEnd, &ciStart, pDropTarget->bColumnSel, 0);

          GlobalUnlock(stgmed.hGlobal);
        }
        ReleaseStgMedium(&stgmed);
      }
    }
    else
    {
      fmtetc.cfFormat=CF_TEXT;

      if (pDataObject->QueryGetData(&fmtetc) == S_OK)
      {
        if (pDataObject->GetData(&fmtetc, &stgmed) == S_OK)
        {
          if (pData=GlobalLock(stgmed.hGlobal))
          {
            AECHARINDEX ciStart={0};
            AECHARINDEX ciEnd={0};
            BOOL bSetSel=TRUE;

            AE_StackUndoGroupStop(ae);

            //Delete
            if (*pdwEffect & DROPEFFECT_MOVE)
            {
              if (ae->bDragging)
              {
                AEPOINT *lpPoint;

                lpPoint=AE_StackPointInsert(ae, &ciCharIndex);
                AE_DeleteTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, ae->bColumnSel, AEDELT_LOCKSCROLL);
                ciCharIndex=lpPoint->ciPoint;
                AE_StackPointDelete(ae, lpPoint);

                ae->bDeleteSelection=FALSE;
                bSetSel=FALSE;
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
                if (bSetSel) AE_SetSelectionPos(ae, &ciCharIndex, &ciCharIndex, FALSE, 0);
                AE_InsertText(ae, &ciCharIndex, wszText, dwUnicodeBytes / sizeof(wchar_t) - 1, ae->nInputNewLine, pDropTarget->bColumnSel, 0, &ciStart, &ciEnd);

                AE_HeapFree(ae, 0, (LPVOID)wszText);
              }
            }
            AE_StackUndoGroupStop(ae);
            AE_SetSelectionPos(ae, &ciEnd, &ciStart, pDropTarget->bColumnSel, 0);

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
    AE_ScrollToCaret(ae, &ae->ptCaret);
    AE_SetCaretPos(ae, &ae->ptCaret);
  }
  else
  {
    if (!PtInRect(&ae->rcDraw, *(POINT *)pt))
    {
      //Deny dropping in non-rect
      AE_SetCaretPos(ae, &ae->ptCaret);
      *pdwEffect=DROPEFFECT_NONE;
    }
    else
    {
      AECHARINDEX ciCharIndex;
      POINT ptGlobal;

      AE_GetCharFromPos(ae, (POINT *)pt, &ciCharIndex, &ptGlobal, pDropTarget->bColumnSel);

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
        //Scroll, but deny dropping in selection
        AE_ScrollToCaret(ae, &ptGlobal);
        AE_SetCaretPos(ae, &ae->ptCaret);
        *pdwEffect=DROPEFFECT_NONE;
      }
      else
      {
        AE_ScrollToCaret(ae, &ptGlobal);
        AE_SetCaretPos(ae, &ptGlobal);
      }
    }
  }
}

HRESULT WINAPI AEIDropSource_QueryInterface(LPUNKNOWN lpTable, REFIID riid, void **ppvObj)
{
  if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource))
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
  if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDataObject))
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
  {
    return DV_E_FORMATETC;
  }

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
              AE_memcpy(pTarget, pSource, dwSourceSize);
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
  else
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

void AE_DataObjectCopySelection(AKELEDIT *ae)
{
  HGLOBAL hDataTargetA=NULL;
  HGLOBAL hDataTargetW=NULL;
  LPVOID pDataTargetA;
  LPVOID pDataTargetW;
  DWORD dwAnsiBytes;
  DWORD dwUnicodeLen;

  if (AE_IndexCompare(&ae->ciSelStartIndex, &ae->ciSelEndIndex))
  {
    if (dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, NULL, 0, ae->nOutputNewLine, ae->bColumnSel, TRUE))
    {
      if (hDataTargetW=GlobalAlloc(GMEM_MOVEABLE, dwUnicodeLen * sizeof(wchar_t)))
      {
        if (pDataTargetW=GlobalLock(hDataTargetW))
        {
          dwUnicodeLen=AE_GetTextRange(ae, &ae->ciSelStartIndex, &ae->ciSelEndIndex, (wchar_t *)pDataTargetW, (DWORD)-1, ae->nOutputNewLine, ae->bColumnSel, TRUE);

          //Get Ansi text
          dwAnsiBytes=WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen + 1, NULL, 0, NULL, NULL);

          if (hDataTargetA=GlobalAlloc(GMEM_MOVEABLE, dwAnsiBytes))
          {
            if (pDataTargetA=GlobalLock(hDataTargetA))
            {
              WideCharToMultiByte(CP_ACP, 0, (wchar_t *)pDataTargetW, dwUnicodeLen + 1, (char *)pDataTargetA, dwAnsiBytes, NULL, NULL);
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

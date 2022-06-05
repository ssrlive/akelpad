#include "AnyRichEdit.h"
#ifdef AKELPAD_X64
  #include "AkelEdit.h"
#endif


INT_X AnyRichEdit_ExGetSelPos(HWND hEd, INT_X* pnStartPos, INT_X* pnEndPos)
{
  CHARRANGE_X cr;

  SendMessage(hEd, EM_EXGETSEL_X, 0, (LPARAM) &cr);
  if (pnStartPos != NULL)
    *pnStartPos = (INT_X) cr.cpMin;
  if (pnEndPos != NULL)
    *pnEndPos = (INT_X) cr.cpMax;

  return cr.cpMin;
}

void AnyRichEdit_ExLimitText(HWND hEd, INT_X nMaxLength)
{
  SendMessage(hEd, EM_EXLIMITTEXT, 0, (LPARAM) nMaxLength);
}

INT AnyRichEdit_ExLineFromChar(HWND hEd, INT_X nCharacterIndex)
{
  return ((INT) SendMessage(hEd, EM_EXLINEFROMCHAR, 0, (LPARAM) nCharacterIndex));
}

void AnyRichEdit_ExSetSelPos(HWND hEd, INT_X nStartPos, INT_X nEndPos)
{
  CHARRANGE_X cr;

  cr.cpMin = nStartPos;
  cr.cpMax = nEndPos;
  SendMessage(hEd, EM_EXSETSEL_X, 0, (LPARAM) &cr);
}

INT_X AnyRichEdit_FirstVisibleCharIndex(HWND hEd)
{
  RECT editRect;
  if (AnyRichEdit_GetEditRect(hEd, &editRect))
  {
    POINTL pt;
    pt.x = editRect.left;
    pt.y = editRect.top;
    return ((INT_X) SendMessage(hEd, EM_CHARFROMPOS, 0, (LPARAM) &pt));
  }
  return -1;
}

INT AnyRichEdit_FirstVisibleLine(HWND hEd)
{
  INT_X i = AnyRichEdit_FirstVisibleCharIndex(hEd);
  if (i >= 0)
  {
    i = AnyRichEdit_ExLineFromChar(hEd, i);
  }
  return (INT) i;
}

TCHAR AnyRichEdit_GetCharAt(HWND hEd, INT_X nPos)
{
  TCHAR     ch = 0;
  TCHAR     str[4];

  if (AnyRichEdit_GetTextAt(hEd, nPos, 1, str) > 0)
  {
    ch = str[0];
  }

  return ch;
}

BOOL AnyRichEdit_GetEditRect(HWND hEd, RECT* lpRect)
{
  if (!lpRect)
    return FALSE;

  SendMessage(hEd, EM_GETRECT, 0, (LPARAM) lpRect);
  return TRUE;
}

DWORD AnyRichEdit_GetEventMask(HWND hEd)
{
  return ((DWORD) SendMessage(hEd, EM_GETEVENTMASK, 0, 0));
}

INT AnyRichEdit_GetFirstVisibleLine(HWND hEd)
{
  return ((INT) SendMessage(hEd, EM_GETFIRSTVISIBLELINE, 0, 0));
}

INT AnyRichEdit_GetLine(HWND hEd, INT nLine, TCHAR* lpTextBuf, WORD wTextBufSize)
{
  INT   nLen;
  WORD* p;

  p = (WORD*) lpTextBuf;
  *p = wTextBufSize;
  nLen = (INT) SendMessage(hEd, EM_GETLINE, (WPARAM) nLine, (LPARAM) lpTextBuf);
  lpTextBuf[nLen] = 0;
  return nLen;
}

INT AnyRichEdit_GetLineCount(HWND hEd)
{
  return ((INT) SendMessage(hEd, EM_GETLINECOUNT, 0, 0));
}

INT_X AnyRichEdit_GetTextAt(HWND hEd, INT_X nPos, INT_X nLen, TCHAR* lpText)
{
  INT_X       nRet;
  TEXTRANGE_X tr;

  tr.lpstrText = lpText;
  tr.chrg.cpMin = nPos;
  tr.chrg.cpMax = nPos + nLen;
  nRet = (INT_X) SendMessage(hEd, EM_GETTEXTRANGE_X, 0, (LPARAM) &tr);

  return nRet;
}

INT_X AnyRichEdit_LastVisibleCharIndex(HWND hEd)
{
  RECT editRect;
  if (AnyRichEdit_GetEditRect(hEd, &editRect))
  {
    POINTL pt;
    pt.x = editRect.right;
    pt.y = editRect.bottom;
    return ((INT_X) SendMessage(hEd, EM_CHARFROMPOS, 0, (LPARAM) &pt));
  }
  return -1;
}

INT AnyRichEdit_LastVisibleLine(HWND hEd)
{
  INT_X i = AnyRichEdit_LastVisibleCharIndex(hEd);
  if (i >= 0)
  {
    i = AnyRichEdit_ExLineFromChar(hEd, i);
  }
  return (INT) i;
}

INT_X AnyRichEdit_LineIndex(HWND hEd, INT nLine)
{
  return ((INT_X) SendMessage(hEd, EM_LINEINDEX, (WPARAM) nLine, 0));
}

INT AnyRichEdit_LineLength(HWND hEd, INT_X nCharacterIndex)
{
  return ((INT) SendMessage(hEd, EM_LINELENGTH, (WPARAM) nCharacterIndex, 0));
}

void AnyRichEdit_ReplaceSelText(HWND hEd, const TCHAR* cszText, BOOL bCanUndo )
{
  SendMessage(hEd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM) cszText);
}

BOOL AnyRichEdit_SetCharFormat(HWND hEd, DWORD dwMask,
  DWORD dwEffects, COLORREF color, DWORD dwOptions)
{
  BOOL       bRet;
  CHARFORMAT cf;

  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = dwMask;
  cf.dwEffects = dwEffects;
  cf.crTextColor = color;
  bRet = (BOOL) SendMessage(hEd, EM_SETCHARFORMAT, (WPARAM) dwOptions, (LPARAM) &cf);

  return bRet;
}

DWORD AnyRichEdit_SetEventMask(HWND hEd, DWORD nEventMask)
{
  DWORD dwRet;

  dwRet = (DWORD) SendMessage(hEd, EM_SETEVENTMASK, 0, (LPARAM) nEventMask);

  return dwRet;
}

///////////////////////////////////////////////////////////////////////////
// Additional Unicode functions

#if any_ctrl_enable_w_members

  INT_X AnyRichEdit_ExGetSelPosW(HWND hEd, INT_X* pnStartPos, INT_X* pnEndPos)
  {
    CHARRANGE_X cr;

    SendMessageW(hEd, EM_EXGETSEL_X, 0, (LPARAM) &cr);
    if (pnStartPos != NULL)
      *pnStartPos = (INT_X) cr.cpMin;
    if (pnEndPos != NULL)
      *pnEndPos = (INT_X) cr.cpMax;

    return cr.cpMin;
  }

  void AnyRichEdit_ExLimitTextW(HWND hEd, INT_X nMaxLength)
  {
    SendMessageW(hEd, EM_EXLIMITTEXT, 0, (LPARAM) nMaxLength);
  }

  INT AnyRichEdit_ExLineFromCharW(HWND hEd, INT_X nCharacterIndex)
  {
    return ((INT) SendMessageW(hEd, EM_EXLINEFROMCHAR, 0, (LPARAM) nCharacterIndex));
  }

  void AnyRichEdit_ExSetSelPosW(HWND hEd, INT_X nStartPos, INT_X nEndPos)
  {
    CHARRANGE_X cr;

    cr.cpMin = nStartPos;
    cr.cpMax = nEndPos;
    SendMessageW(hEd, EM_EXSETSEL_X, 0, (LPARAM) &cr);
  }

  INT_X AnyRichEdit_FirstVisibleCharIndexW(HWND hEd)
  {
    RECT editRect;
    if (AnyRichEdit_GetEditRect(hEd, &editRect))
    {
      POINTL pt;
      pt.x = editRect.left;
      pt.y = editRect.top;
      return ((INT_X) SendMessageW(hEd, EM_CHARFROMPOS, 0, (LPARAM) &pt));
    }
    return -1;
  }

  INT AnyRichEdit_FirstVisibleLineW(HWND hEd)
  {
    INT_X i = AnyRichEdit_FirstVisibleCharIndexW(hEd);
    if (i >= 0)
    {
      i = AnyRichEdit_ExLineFromCharW(hEd, i);
    }
    return (INT) i;
  }

  WCHAR AnyRichEdit_GetCharAtW(HWND hEd, INT_X nPos)
  {
    WCHAR     wch = 0;
    WCHAR     wstr[4];

    if (AnyRichEdit_GetTextAtW(hEd, nPos, 1, wstr) > 0)
    {
      wch = wstr[0];
    }

    return wch;
  }

  DWORD AnyRichEdit_GetEventMaskW(HWND hEd)
  {
    return ((DWORD) SendMessageW(hEd, EM_GETEVENTMASK, 0, 0));
  }

  INT AnyRichEdit_GetFirstVisibleLineW(HWND hEd)
  {
    return ((INT) SendMessageW(hEd, EM_GETFIRSTVISIBLELINE, 0, 0));
  }

  INT AnyRichEdit_GetLineW(HWND hEd, INT nLine, WCHAR* lpTextBuf, WORD wTextBufSize)
  {
    INT   nLen;
    WORD* p;

    p = (WORD*) lpTextBuf;
    *p = wTextBufSize;
    nLen = (INT) SendMessageW(hEd, EM_GETLINE, (WPARAM) nLine, (LPARAM) lpTextBuf);
    lpTextBuf[nLen] = 0;
    return nLen;
  }

  INT AnyRichEdit_GetLineCountW(HWND hEd)
  {
    return ((INT) SendMessageW(hEd, EM_GETLINECOUNT, 0, 0));
  }

  INT_X AnyRichEdit_GetTextAtW(HWND hEd, INT_X nPos, INT_X nLen, WCHAR* lpText)
  {
    INT_X        nRet;
    TEXTRANGEW_X trw;

    trw.lpstrText = lpText;
    trw.chrg.cpMin = nPos;
    trw.chrg.cpMax = nPos + nLen;
    nRet = (INT_X) SendMessageW(hEd, EM_GETTEXTRANGE_X, 0, (LPARAM) &trw);

    return nRet;
  }

  INT_X AnyRichEdit_LastVisibleCharIndexW(HWND hEd)
  {
    RECT editRect;
    if (AnyRichEdit_GetEditRect(hEd, &editRect))
    {
      POINTL pt;
      pt.x = editRect.right;
      pt.y = editRect.bottom;
      return ((INT_X) SendMessageW(hEd, EM_CHARFROMPOS, 0, (LPARAM) &pt));
    }
    return -1;
  }

  INT AnyRichEdit_LastVisibleLineW(HWND hEd)
  {
    INT_X i = AnyRichEdit_LastVisibleCharIndexW(hEd);
    if (i >= 0)
    {
      i = AnyRichEdit_ExLineFromCharW(hEd, i);
    }
    return (INT) i;
  }

  INT_X AnyRichEdit_LineIndexW(HWND hEd, INT nLine)
  {
    return ((INT_X) SendMessageW(hEd, EM_LINEINDEX, (WPARAM) nLine, 0));
  }

  INT AnyRichEdit_LineLengthW(HWND hEd, INT_X nCharacterIndex)
  {
    return ((INT) SendMessageW(hEd, EM_LINELENGTH, (WPARAM) nCharacterIndex, 0));
  }

  void AnyRichEdit_ReplaceSelTextW(HWND hEd, const WCHAR* cszText, BOOL bCanUndo )
  {
    SendMessageW(hEd, EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM) cszText);
  }

  BOOL AnyRichEdit_SetCharFormatW(HWND hEd, DWORD dwMask,
    DWORD dwEffects, COLORREF color, DWORD dwOptions)
  {
    BOOL       bRet;
    CHARFORMAT cf;

    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = dwMask;
    cf.dwEffects = dwEffects;
    cf.crTextColor = color;
    bRet = (BOOL) SendMessageW(hEd, EM_SETCHARFORMAT, (WPARAM) dwOptions, (LPARAM) &cf);

    return bRet;
  }

  DWORD AnyRichEdit_SetEventMaskW(HWND hEd, DWORD nEventMask)
  {
    DWORD dwRet;

    dwRet = (DWORD) SendMessageW(hEd, EM_SETEVENTMASK, 0, (LPARAM) nEventMask);

    return dwRet;
  }

#endif


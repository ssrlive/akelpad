#ifndef _winapi_c_any_rich_edit_h_
#define _winapi_c_any_rich_edit_h_
//---------------------------------------------------------------------------

// "base.h" must be the first header file
#include "base.h"
#include "AnyWindow.h"
#include <richedit.h>

#ifdef AKELPAD_X64
  // types and structs
  #define INT_X INT_PTR
  #define CHARRANGE_X CHARRANGE64
  #define TEXTRANGE_X TEXTRANGE64
  #define TEXTRANGEA_X TEXTRANGE64A
  #define TEXTRANGEW_X TEXTRANGE64W
  // messages
  #define EM_EXGETSEL_X EM_EXGETSEL64
  #define EM_EXSETSEL_X EM_EXSETSEL64
  #define EM_GETTEXTRANGE_X EM_GETTEXTRANGE64
#else
  // types and structs
  #define INT_X INT
  #define CHARRANGE_X CHARRANGE
  #define TEXTRANGE_X TEXTRANGE
  #define TEXTRANGEA_X TEXTRANGEA
  #define TEXTRANGEW_X TEXTRANGEW
  // messages
  #define EM_EXGETSEL_X EM_EXGETSEL
  #define EM_EXSETSEL_X EM_EXSETSEL
  #define EM_GETTEXTRANGE_X EM_GETTEXTRANGE
#endif


INT_X   AnyRichEdit_ExGetSelPos(HWND hEd, INT_X* pnStartPos /* = NULL */, INT_X* pnEndPos /* = NULL */);
void    AnyRichEdit_ExLimitText(HWND hEd, INT_X nMaxLength);
INT     AnyRichEdit_ExLineFromChar(HWND hEd, INT_X nCharacterIndex);
void    AnyRichEdit_ExSetSelPos(HWND hEd, INT_X nStartPos, INT_X nEndPos);
INT_X   AnyRichEdit_FirstVisibleCharIndex(HWND hEd);
INT     AnyRichEdit_FirstVisibleLine(HWND hEd);
TCHAR   AnyRichEdit_GetCharAt(HWND hEd, INT_X nPos);
BOOL    AnyRichEdit_GetEditRect(HWND hEd, RECT* lpRect); // always use it instead of GetClientRect!!!
DWORD   AnyRichEdit_GetEventMask(HWND hEd);
INT     AnyRichEdit_GetFirstVisibleLine(HWND hEd);
INT     AnyRichEdit_GetLine(HWND hEd, INT nLine, TCHAR* lpTextBuf, WORD wTextBufSize);
INT     AnyRichEdit_GetLineCount(HWND hEd);
INT_X   AnyRichEdit_GetTextAt(HWND hEd, INT_X nPos, INT_X nLen, TCHAR* lpText);
INT_X   AnyRichEdit_LastVisibleCharIndex(HWND hEd);
INT     AnyRichEdit_LastVisibleLine(HWND hEd);
INT_X   AnyRichEdit_LineIndex(HWND hEd, INT nLine); // returns index of the 1st character
INT     AnyRichEdit_LineLength(HWND hEd, INT_X nCharacterIndex);
void    AnyRichEdit_ReplaceSelText(HWND hEd, const TCHAR* cszText, BOOL bCanUndo /* = FALSE */);
BOOL    AnyRichEdit_SetCharFormat(HWND hEd, DWORD dwMask, DWORD dwEffects,
          COLORREF color /* = RGB(0,0,0) */, DWORD dwOptions /* = SCF_SELECTION */);
DWORD   AnyRichEdit_SetEventMask(HWND hEd, DWORD nEventMask);

#if any_ctrl_enable_w_members
INT_X   AnyRichEdit_ExGetSelPosW(HWND hEd, INT_X* pnStartPos /* = NULL */, INT_X* pnEndPos /* = NULL */);
void    AnyRichEdit_ExLimitTextW(HWND hEd, INT_X nMaxLength);
INT     AnyRichEdit_ExLineFromCharW(HWND hEd, INT_X nCharacterIndex);
void    AnyRichEdit_ExSetSelPosW(HWND hEd, INT_X nStartPos, INT_X nEndPos);
INT_X   AnyRichEdit_FirstVisibleCharIndexW(HWND hEd);
INT     AnyRichEdit_FirstVisibleLineW(HWND hEd);
WCHAR   AnyRichEdit_GetCharAtW(HWND hEd, INT_X nPos);
DWORD   AnyRichEdit_GetEventMaskW(HWND hEd);
INT     AnyRichEdit_GetFirstVisibleLineW(HWND hEd);
INT     AnyRichEdit_GetLineW(HWND hEd, INT nLine, WCHAR* lpTextBuf, WORD wTextBufSize);
INT     AnyRichEdit_GetLineCountW(HWND hEd);
INT_X   AnyRichEdit_GetTextAtW(HWND hEd, INT_X nPos, INT_X nLen, WCHAR* lpText);
INT_X   AnyRichEdit_LastVisibleCharIndexW(HWND hEd);
INT     AnyRichEdit_LastVisibleLineW(HWND hEd);
INT_X   AnyRichEdit_LineIndexW(HWND hEd, INT nLine); // returns index of the 1st character
INT     AnyRichEdit_LineLengthW(HWND hEd, INT_X nCharacterIndex);
void    AnyRichEdit_ReplaceSelTextW(HWND hEd, const WCHAR* cszText, BOOL bCanUndo /* = FALSE */);
BOOL    AnyRichEdit_SetCharFormatW(HWND hEd, DWORD dwMask, DWORD dwEffects,
          COLORREF color /* = RGB(0,0,0) */, DWORD dwOptions /* = SCF_SELECTION */);
DWORD   AnyRichEdit_SetEventMaskW(HWND hEd, DWORD nEventMask);
#endif


//---------------------------------------------------------------------------
#endif

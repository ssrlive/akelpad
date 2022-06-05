#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0500
#include <windows.h>
#include <richedit.h>
#include <commctrl.h>
#include "StackFunc.h"
#include "StrFunc.h"
#include "x64Func.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "RegExpFunc.h"
#include "AkelDLL.h"
#include "Coder.h"
#include "HighLight.h"
#include "CodeFold.h"
#include "AutoComplete.h"
#include "Resources\Resource.h"


/*
//Include stack functions
#define StackGetElement
#define StackGetIndex
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackMoveAfter
#define StackMoveBefore
#define StackMoveIndex
#define StackDelete
#define StackClear
#define StackCopy
#define StackJoin
#include "StackFunc.h"

//Include string functions
#define WideCharUpper
#define WideCharLower
#define xmemcmp
#define xmemcpy
#define xmemset
#define xstrcmpW
#define xstrcmpnW
#define xstrcmpinW
#define xstrcmpiW
#define xstrlenA
#define xstrlenW
#define xstrcpyA
#define xstrcpyW
#define xstrcpynA
#define xstrcpynW
#define xstrstrW
#define xstrrepW
#define xarrlenA
#define xarrlenW
#define hex2decA
#define hex2decW
#define xatoiA
#define xatoiW
#define xitoaA
#define xitoaW
#define xuitoaA
#define xuitoaW
#define dec2hexA
#define dec2hexW
#define xprintfA
#define xprintfW
#define UTF16toUTF8
#include "StrFunc.h"

//Include x64 functions
#define WideCharToMultiByte64
#define WriteFile64
#include "x64Func.h"

//Include wide functions
#define AppendMenuWide
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define ComboBox_FindStringWide
#define ComboBox_GetLBTextWide
#define CreateDialogWide
#define CreateFileWide
#define CreateFontIndirectWide
#define CreateWindowExWide
#define DefWindowProcWide
#define DialogBoxParamWide
#define DialogBoxWide
#define ExpandEnvironmentStringsWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetSaveFileNameWide
#define GetWindowLongPtrWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define IconExtractWide
#define ListBox_AddStringWide
#define ListView_GetItemWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define LoadImageWide
#define LoadLibraryExWide
#define PropertySheetWide
#define RegisterClassWide
#define SearchPathWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define TreeView_GetItemWide
#define TreeView_InsertItemWide
#define TreeView_SetItemWide
#define UnregisterClassWide
#include "WideFunc.h"

//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"
//*/

//Include RegExp functions
#define RE_FUNCTIONS
#include "RegExpFunc.h"


//// Global variables
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszMessage[MAX_PATH];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
char szCoderDir[MAX_PATH];
wchar_t wszCoderDir[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
HINSTANCE hInstanceDLL;
HINSTANCE hInstanceEXE;
HWND hMainWnd;
HWND hWndEdit;
HICON hMainIcon;
HWND hMdiClient;
BOOL bOldWindows;
BOOL bOldRichEdit;
BOOL bAkelEdit;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
int nInitMain=0;
DWORD dwSaveFlags=0;
BOOL bInitHighLight=FALSE;
int nInitCodeFold=0;
BOOL bInitAutoComplete=FALSE;
DWORD dwUseCache=UC_FAST;
DWORD dwSaveCache=SC_NONE;
BOOL bDefaultAliasEnable=FALSE;
wchar_t wszDefaultAlias[MAX_PATH]=L".cpp";
STACKSYNTAXFILE hSyntaxFilesStack={0};
STACKMANUALSET hManualStack={0};
STACKVARTHEME hVarThemesStack={0};
VARTHEME hVarThemeGlobal;
SYNTAXFILE *lpLoadSyntaxFile=NULL;
VARTHEME *lpVarThemeActive=NULL;
VARINFO *lpVarInfoFastCheck=NULL;
wchar_t *wszVarThemeLink=NULL;
int nVarThemesInternalCount=0;
POINT ptTopVarIndex={0};
BOOL bTopVarOpenScroll;
BOOL bSyntaxFileLoadError;
BOOL bSyntaxFileLoadMute=FALSE;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;
WNDPROCDATA *NewEditProcData=NULL;

//Options dialog
PROPSHEETHEADERW psh={0};
PROPSHEETPAGEW psp[PAGE_MAX]={0};
HWND hWndPropSheet=NULL;
HIMAGELIST hImageList;
HHOOK hHookOptions;
int nPropMaxVisitPage;
BOOL bSaveOptions;
int nColumnWidth1=218;
int nColumnWidth2=110;
int nColumnWidth3=100;
RECT rcLinkMinMaxDialog={299, 254, 0, 0};
RECT rcLinkCurrentDialog={0};


//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Coder";
}

void __declspec(dllexport) Settings(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_CODER_SETTINGS)
    {
      int nStartPage=0;

      if (IsExtCallParamValid(pd->lParam, 2))
        nStartPage=(int)GetExtCallParam(pd->lParam, 2);

      SettingsSheet(nStartPage);
    }
    else if (nAction == DLLA_CODER_EXPORT)
    {
      DWORD dwFlags=HLEXP_COPY|HLEXP_COPYTEXT;
      unsigned char *pRtfFile=NULL;
      wchar_t wszRtfFile[MAX_PATH];

      if (IsExtCallParamValid(pd->lParam, 2))
        dwFlags=(DWORD)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        pRtfFile=(unsigned char *)GetExtCallParam(pd->lParam, 3);

      if (pRtfFile)
      {
        if (pd->dwSupport & PDS_STRANSI)
          MultiByteToWideChar(CP_ACP, 0, (char *)pRtfFile, -1, wszRtfFile, MAX_PATH);
        else
          xstrcpynW(wszRtfFile, (wchar_t *)pRtfFile, MAX_PATH);
      }
      else wszRtfFile[0]=L'\0';

      ExportToRtf(pd->hWndEdit, dwFlags, wszRtfFile);
    }
    else
    {
      if (nInitMain)
      {
        if (nAction == DLLA_CODER_SETEXTENSION ||
            nAction == DLLA_CODER_SETALIAS)
        {
          MANUALSET *lpManual;
          unsigned char *pExtOrAlias=NULL;
          EDITINFO ei;

          if (IsExtCallParamValid(pd->lParam, 2))
            pExtOrAlias=(unsigned char *)GetExtCallParam(pd->lParam, 2);

          if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)pd->hWndEdit, (LPARAM)&ei))
          {
            if (pExtOrAlias)
            {
              if (!(lpManual=StackGetManual(&hManualStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit)))
                lpManual=StackInsertManual(&hManualStack);

              if (lpManual)
              {
                if (ei.hWndMaster)
                {
                  lpManual->hWndEdit=ei.hWndMaster;
                  lpManual->hDocEdit=ei.hDocMaster;
                }
                else
                {
                  lpManual->hWndEdit=ei.hWndEdit;
                  lpManual->hDocEdit=ei.hDocEdit;
                }

                if (nAction == DLLA_CODER_SETEXTENSION)
                {
                  lpManual->wszAlias[0]=L'.';
                  if (pd->dwSupport & PDS_STRANSI)
                    MultiByteToWideChar(CP_ACP, 0, (char *)pExtOrAlias, -1, lpManual->wszAlias + 1, MAX_PATH);
                  else
                    xstrcpynW(lpManual->wszAlias + 1, (wchar_t *)pExtOrAlias, MAX_PATH);
                  if (!lpManual->wszAlias[1]) lpManual->wszAlias[0]=L'\0';

                  if (!xstrcmpW(lpManual->wszAlias, L".?"))
                    xstrcpynW(lpManual->wszAlias, L"?", MAX_PATH);
                }
                else
                {
                  if (pd->dwSupport & PDS_STRANSI)
                    MultiByteToWideChar(CP_ACP, 0, (char *)pExtOrAlias, -1, lpManual->wszAlias, MAX_PATH);
                  else
                    xstrcpynW(lpManual->wszAlias, (wchar_t *)pExtOrAlias, MAX_PATH);
                }
              }
            }
            else
            {
              if (lpManual=StackGetManual(&hManualStack, ei.hWndMaster?ei.hWndMaster:ei.hWndEdit, ei.hDocMaster?ei.hDocMaster:ei.hDocEdit))
                StackDeleteManual(&hManualStack, lpManual, CODER_ALL);
            }

            //Update
            if (nInitCodeFold)
            {
              lpCurrentFoldWindow=SetActiveEdit(pd->hWndEdit, hWndCodeFoldList, SAE_RESETFOLD|SAE_RESETLIST);
              InvalidateRect(pd->hWndEdit, NULL, TRUE);
            }
            if (bInitHighLight)
            {
              bUpdateTheme=TRUE;
              UpdateEditAndClones(pd->hWndEdit, UE_DRAWRECT);
            }
          }
        }
        else if (nAction == DLLA_CODER_SETVARTHEME)
        {
          unsigned char *pVarThemeName=NULL;
          wchar_t wszVarThemeName[MAX_PATH];
          VARTHEME *lpVarTheme;

          if (IsExtCallParamValid(pd->lParam, 2))
            pVarThemeName=(unsigned char *)GetExtCallParam(pd->lParam, 2);

          if (pVarThemeName)
          {
            if (pd->dwSupport & PDS_STRANSI)
              MultiByteToWideChar(CP_ACP, 0, (char *)pVarThemeName, -1, wszVarThemeName, MAX_PATH);
            else
              xstrcpynW(wszVarThemeName, (wchar_t *)pVarThemeName, MAX_PATH);

            if (lpVarTheme=RequestVarTheme(&hVarThemesStack, wszVarThemeName))
            {
              lpVarThemeActive=lpVarTheme;
              GetVarThemeGlobals(lpVarThemeActive);
              SaveOptions(OF_GENERAL_ACTIVETHEME);
              SaveCache(dwSaveCache);
              ClearCache(FALSE);
            }
          }
        }
        else if (nAction == DLLA_CODER_CLEARCACHE)
        {
          DWORD *lpdwUseCache=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            lpdwUseCache=(DWORD *)GetExtCallParam(pd->lParam, 2);

          if (lpdwUseCache)
            *lpdwUseCache=dwUseCache;
          else
            ClearCache(TRUE);
        }
        else if (nAction == DLLA_CODER_CHECKALIAS)
        {
          wchar_t wszAlias[MAX_PATH];
          const unsigned char *pAlias=NULL;
          BOOL *lpbActive=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            pAlias=(const unsigned char *)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            lpbActive=(BOOL *)GetExtCallParam(pd->lParam, 3);

          if (pAlias && lpbActive)
          {
            if (pd->dwSupport & PDS_STRANSI)
              MultiByteToWideChar(CP_ACP, 0, (char *)pAlias, -1, wszAlias, MAX_PATH);
            else
              xstrcpynW(wszAlias, (wchar_t *)pAlias, MAX_PATH);

            if (hWndEdit=GetFocusEdit())
            {
              if (StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWndEdit, NULL, NULL) == StackGetSyntaxFileByFile(&hSyntaxFilesStack, wszAlias))
                *lpbActive=TRUE;
              else
                *lpbActive=FALSE;
            }
          }
        }
        else if (nAction == DLLA_CODER_CHECKVARTHEME)
        {
          SYNTAXFILE *lpSyntaxFile;
          VARTHEME *lpVarTheme;
          wchar_t wszVarThemeName[MAX_PATH];
          const unsigned char *pVarThemeName=NULL;
          BOOL *lpbActive=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            pVarThemeName=(const unsigned char *)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            lpbActive=(BOOL *)GetExtCallParam(pd->lParam, 3);

          if (pVarThemeName && lpbActive)
          {
            if (pd->dwSupport & PDS_STRANSI)
              MultiByteToWideChar(CP_ACP, 0, (char *)pVarThemeName, -1, wszVarThemeName, MAX_PATH);
            else
              xstrcpynW(wszVarThemeName, (wchar_t *)pVarThemeName, MAX_PATH);

            if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, NULL, NULL, NULL))
            {
              lpVarTheme=lpSyntaxFile->lpVarThemeLink;
              if (!lpVarTheme) lpVarTheme=lpVarThemeActive;
              *lpbActive=!xstrcmpiW(lpVarTheme->wszVarThemeName, wszVarThemeName);
            }
            else *lpbActive=FALSE;
          }
        }
        else if (nAction == DLLA_CODER_GETSYNTAXFILE)
        {
          SYNTAXFILE *lpSyntaxFile;
          HWND hWnd=NULL;
          const unsigned char *pSyntaxFileName=NULL;
          int nSyntaxFileNameMax=0;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            pSyntaxFileName=(const unsigned char *)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            nSyntaxFileNameMax=(int)GetExtCallParam(pd->lParam, 4);

          if (pSyntaxFileName && nSyntaxFileNameMax)
          {
            if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, NULL, NULL))
            {
              if (pd->dwSupport & PDS_STRANSI)
                WideCharToMultiByte(CP_ACP, 0, lpSyntaxFile->wszSyntaxFileName, -1, (char *)pSyntaxFileName, nSyntaxFileNameMax, NULL, NULL);
              else
                xstrcpynW((wchar_t *)pSyntaxFileName, lpSyntaxFile->wszSyntaxFileName, nSyntaxFileNameMax);
            }
          }
        }
        else if (nAction == DLLA_CODER_GETALIAS)
        {
          HWND hWnd=NULL;
          AEHDOC hDoc=NULL;
          const wchar_t *wpAlias=NULL;
          const unsigned char *pAlias=NULL;
          INT_PTR *lpnAliasLen=NULL;
          INT_PTR nAliasLen=0;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDoc=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            pAlias=(const unsigned char *)GetExtCallParam(pd->lParam, 4);
          if (IsExtCallParamValid(pd->lParam, 5))
            lpnAliasLen=(INT_PTR *)GetExtCallParam(pd->lParam, 5);

          StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, hDoc, &wpAlias);
          if (pd->dwSupport & PDS_STRANSI)
            nAliasLen=WideCharToMultiByte(CP_ACP, 0, wpAlias, -1, (char *)pAlias, MAX_PATH, NULL, NULL);
          else
            nAliasLen=xstrcpynW((wchar_t *)pAlias, wpAlias, MAX_PATH) + (pAlias?1:0);
          if (lpnAliasLen) *lpnAliasLen=nAliasLen;
        }
        else if (nAction == DLLA_CODER_GETVARTHEME)
        {
          SYNTAXFILE *lpSyntaxFile;
          VARTHEME *lpVarTheme;
          HWND hWnd=NULL;
          const unsigned char *pVarTheme=NULL;
          int nVarThemeMax=0;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            pVarTheme=(const unsigned char *)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            nVarThemeMax=(int)GetExtCallParam(pd->lParam, 4);

          if (pVarTheme && nVarThemeMax)
          {
            if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, NULL, NULL))
            {
              lpVarTheme=lpSyntaxFile->lpVarThemeLink;
              if (!lpVarTheme) lpVarTheme=lpVarThemeActive;

              if (pd->dwSupport & PDS_STRANSI)
                WideCharToMultiByte(CP_ACP, 0, lpVarTheme->wszVarThemeName, -1, (char *)pVarTheme, nVarThemeMax, NULL, NULL);
              else
                xstrcpynW((wchar_t *)pVarTheme, lpVarTheme->wszVarThemeName, nVarThemeMax);
            }
            else xprintfW((wchar_t *)pVarTheme, L"");
          }
        }
        else if (nAction == DLLA_CODER_GETVARIABLE)
        {
          SYNTAXFILE *lpSyntaxFile;
          VARTHEME *lpVarTheme=NULL;
          VARINFO *lpVarInfo;
          HWND hWnd=NULL;
          AEHDOC hDoc=NULL;
          wchar_t wszVariableName[MAX_PATH];
          const unsigned char *pVariableName=NULL;
          unsigned char *pVariableValue=NULL;
          INT_PTR *lpnVariableValueLen=NULL;
          INT_PTR nVariableValueLen=0;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDoc=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            pVariableName=(const unsigned char *)GetExtCallParam(pd->lParam, 4);
          if (IsExtCallParamValid(pd->lParam, 5))
            pVariableValue=(unsigned char *)GetExtCallParam(pd->lParam, 5);
          if (IsExtCallParamValid(pd->lParam, 6))
            lpnVariableValueLen=(INT_PTR *)GetExtCallParam(pd->lParam, 6);

          if (pVariableName)
          {
            if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, hDoc, NULL))
            {
              lpVarTheme=lpSyntaxFile->lpVarThemeLink;
              if (!lpVarTheme) lpVarTheme=lpVarThemeActive;

              if (lpVarTheme)
              {
                if (pd->dwSupport & PDS_STRANSI)
                  MultiByteToWideChar(CP_ACP, 0, (char *)pVariableName, -1, wszVariableName, MAX_PATH);
                else
                  xstrcpynW(wszVariableName, (wchar_t *)pVariableName, MAX_PATH);

                if (lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, wszVariableName, -1))
                {
                  if (pd->dwSupport & PDS_STRANSI)
                    nVariableValueLen=WideCharToMultiByte(CP_ACP, 0, lpVarInfo->wpVarValue, lpVarInfo->nVarValueLen + 1, (char *)pVariableValue, 0x3FFFFFFF, NULL, NULL);
                  else
                    nVariableValueLen=xstrcpynW((wchar_t *)pVariableValue, lpVarInfo->wpVarValue, lpVarInfo->nVarValueLen + 1) + (pVariableValue?1:0);
                }
              }
            }
            if (lpnVariableValueLen) *lpnVariableValueLen=nVariableValueLen;
          }
        }
        else if (nAction == DLLA_CODER_FILLVARLIST)
        {
          SYNTAXFILE *lpSyntaxFile;
          VARTHEME *lpVarTheme=NULL;
          VARINFO *lpVarInfo;
          HWND hWnd=NULL;
          AEHDOC hDoc=NULL;
          CODERTHEMEITEM *cti=NULL;
          COLORREF crColor;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDoc=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            cti=(CODERTHEMEITEM *)GetExtCallParam(pd->lParam, 4);

          if (cti)
          {
            if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, hDoc, NULL))
            {
              lpVarTheme=lpSyntaxFile->lpVarThemeLink;
              if (!lpVarTheme) lpVarTheme=lpVarThemeActive;

              if (lpVarTheme)
              {
                lpVarInfoFastCheck=NULL;

                for (; cti->wpVarName; ++cti)
                {
                  if (lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, cti->wpVarName, -1))
                  {
                    if (cti->dwVarFlags & VARF_EXTSTRING)
                      cti->nVarValue=(INT_PTR)lpVarInfo->wpVarValue;
                    else if (cti->dwVarFlags & VARF_EXTINTCOLOR ||
                             cti->dwVarFlags & VARF_EXTLPINTCOLOR)
                    {
                      if (*lpVarInfo->wpVarValue == L'#')
                      {
                        crColor=GetColorFromStr(lpVarInfo->wpVarValue + 1, NULL);

                        if (cti->dwVarFlags & VARF_EXTINTCOLOR)
                          cti->nVarValue=crColor;
                        else if (cti->dwVarFlags & VARF_EXTLPINTCOLOR)
                          *(COLORREF *)cti->nVarValue=crColor;
                      }
                    }
                    cti->dwVarFlags=lpVarInfo->dwVarFlags | (cti->dwVarFlags & (VARF_EXTSTRING|VARF_EXTINTCOLOR|VARF_EXTLPINTCOLOR));
                    lpVarInfoFastCheck=lpVarInfo->next;
                  }
                  else break;
                }
              }
            }
          }
        }
        else if (nAction == DLLA_CODER_GETVARTHEMEDATA)
        {
          SYNTAXFILE *lpSyntaxFile;
          VARTHEME **lppVarThemeGlobal=NULL;
          VARTHEME **lppVarThemeActive=NULL;
          HWND hWnd=NULL;
          AEHDOC hDoc=NULL;

          if (IsExtCallParamValid(pd->lParam, 2))
            hWnd=(HWND)GetExtCallParam(pd->lParam, 2);
          if (IsExtCallParamValid(pd->lParam, 3))
            hDoc=(AEHDOC)GetExtCallParam(pd->lParam, 3);
          if (IsExtCallParamValid(pd->lParam, 4))
            lppVarThemeGlobal=(VARTHEME **)GetExtCallParam(pd->lParam, 4);
          if (IsExtCallParamValid(pd->lParam, 5))
            lppVarThemeActive=(VARTHEME **)GetExtCallParam(pd->lParam, 5);

          *lppVarThemeGlobal=&hVarThemeGlobal;
          if (lpSyntaxFile=StackGetSyntaxFileByWindow(&hSyntaxFilesStack, hWnd, hDoc, NULL))
          {
            *lppVarThemeActive=lpSyntaxFile->lpVarThemeLink;
            if (!*lppVarThemeActive) *lppVarThemeActive=lpVarThemeActive;
          }
        }
      }
      else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_CODER_NOTRUNNING), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
    }
  }
  else
  {
    SettingsSheet(0);
  }

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}


//// Export to RTF

void ExportToRtf(HWND hWndEdit, DWORD dwFlags, wchar_t *wszRtfFile)
{
  AECHARRANGE crRange;
  HGLOBAL hRtf;
  INT_PTR nRtfLen;
  BOOL bSelection=TRUE;
  BOOL bExport=TRUE;

  bSelection=(BOOL)SendMessage(hWndEdit, AEM_EXGETSEL, (WPARAM)&crRange.ciMin, (LPARAM)&crRange.ciMax);
  if (dwFlags & HLEXP_ONLYSELECTION)
  {
    if (!bSelection)
      bExport=FALSE;
  }
  else if ((dwFlags & HLEXP_ALLDOCUMENT) || !bSelection)
  {
    SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&crRange.ciMin);
    SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&crRange.ciMax);
  }

  if (bExport)
  {
    if (hRtf=GetRTF(hWndEdit, &crRange, dwFlags, &nRtfLen))
    {
      if (dwFlags & HLEXP_FILE)
      {
        HANDLE hFile;
        char *szRTF;
        UINT_PTR dwBytesWritten;

        if (!*wszRtfFile)
        {
          OPENFILENAMEW efnW;

          xmemset(&efnW, 0, sizeof(OPENFILENAMEW));
          efnW.lStructSize  =sizeof(OPENFILENAMEW);
          efnW.hwndOwner    =hMainWnd;
          efnW.lpstrFile    =wszRtfFile;
          efnW.nMaxFile     =MAX_PATH;
          efnW.lpstrFilter  =L"*.rtf\0*.rtf\0*.*\0*.*\0\0";
          efnW.nFilterIndex =1;
          efnW.Flags        =OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT;
          efnW.lpstrDefExt  =L"rtf";

          GetSaveFileNameWide(&efnW);
        }

        if (*wszRtfFile)
        {
          if ((hFile=CreateFileWide(wszRtfFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
          {
            if (szRTF=(char *)GlobalLock(hRtf))
            {
              WriteFile64(hFile, szRTF, nRtfLen, &dwBytesWritten, NULL);
              GlobalUnlock(hRtf);
            }
            CloseHandle(hFile);
          }
        }
      }
      if (dwFlags & HLEXP_COPY)
      {
        //Set RTF text
        if (OpenClipboard(NULL))
        {
          AETEXTRANGEW tr;
          HGLOBAL hDataA=NULL;
          HGLOBAL hDataW=NULL;
          char *pAnsiStr;
          INT_PTR nAnsiLen;
          UINT cf=RegisterClipboardFormatA("Rich Text Format");

          EmptyClipboard();
          SetClipboardData(cf, hRtf);

          //Set clipboard plain-text
          if (dwFlags & HLEXP_COPYTEXT)
          {
            xmemcpy(&tr.cr, &crRange, sizeof(AECHARRANGE));
            tr.bColumnSel=FALSE;
            tr.pBuffer=NULL;
            tr.nNewLine=AELB_ASOUTPUT;
            tr.bFillSpaces=FALSE;

            if (tr.dwBufferMax=SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
            {
              if (hDataW=GlobalAlloc(GMEM_MOVEABLE, tr.dwBufferMax * sizeof(wchar_t)))
              {
                if (tr.pBuffer=(wchar_t *)GlobalLock(hDataW))
                {
                  SendMessage(hWndEdit, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr);

                  //ANSI
                  nAnsiLen=WideCharToMultiByte64(CP_ACP, 0, tr.pBuffer, tr.dwBufferMax, NULL, 0, NULL, NULL);

                  if (hDataA=GlobalAlloc(GMEM_MOVEABLE, nAnsiLen))
                  {
                    if (pAnsiStr=(char *)GlobalLock(hDataA))
                    {
                      WideCharToMultiByte64(CP_ACP, 0, tr.pBuffer, tr.dwBufferMax, pAnsiStr, nAnsiLen, NULL, NULL);
                      GlobalUnlock(hDataA);
                    }
                    else
                    {
                      GlobalFree(hDataA);
                      hDataA=NULL;
                    }
                  }
                  GlobalUnlock(hDataW);
                }
                else
                {
                  GlobalFree(hDataW);
                  hDataW=NULL;
                }
              }
            }
            if (hDataW) SetClipboardData(CF_UNICODETEXT, hDataW);
            if (hDataA) SetClipboardData(CF_TEXT, hDataA);
          }
          else if (dwFlags & HLEXP_COPYDATA)
          {
            SetClipboardData(CF_TEXT, hRtf);
          }
          CloseClipboard();
        }
      }
    }
  }
}

HANDLE GetRTF(HWND hWndEdit, AECHARRANGE *crRange, DWORD dwFlags, INT_PTR *lpnRtfLen)
{
  AEGETHIGHLIGHT aegh;
  RTFDATA rtf;
  HGLOBAL hRtf=NULL;
  int nTableLen;

  //Get font
  SendMessage(hMainWnd, AKD_GETFONTW, (WPARAM)hWndEdit, (LPARAM)&rtf.lfFont);
  rtf.nPointSize=(int)SendMessage(hWndEdit, AEM_GETCHARSIZE, AECS_POINTSIZE, (LPARAM)NULL);
  xprintfA(rtf.szDefStyle, "\\b%s\\i%s", rtf.lfFont.lfWeight == FW_BOLD?"":"0", rtf.lfFont.lfItalic?"":"0");

  //Get colors
  rtf.aec.dwFlags=AECLR_ALL;
  SendMessage(hWndEdit, AEM_GETCOLORS, 0, (LPARAM)&rtf.aec);

  xmemset(&rtf.hColorStack, 0, sizeof(STACKRTFCOLOR));
  StackInsertRTFColor(&rtf.hColorStack, rtf.aec.crBasicText);
  StackInsertRTFColor(&rtf.hColorStack, rtf.aec.crBasicBk);
  xprintfA(rtf.szDefTextColor, "\\cf%d", (dwFlags & HLEXP_NOBASICTEXTCOLOR)?0:1);
  xprintfA(rtf.szDefBkColor, "\\highlight%d\\chcbpat%d", (dwFlags & HLEXP_NOBASICBKCOLOR)?0:2, (dwFlags & HLEXP_NOBASICBKCOLOR)?0:2);

  //Calculate rtf.nRtfLen
  rtf.szRtf=NULL;
  rtf.nRtfLen=0;
  xmemcpy(&rtf.crRange, crRange, sizeof(AECHARRANGE));
  rtf.dwFlags=dwFlags;

  //Get flags
  aegh.dwFlags=AEGHF_NOSELECTION|AEGHF_NOACTIVELINE|AEGHF_NOALTLINE;
  if (dwFlags & HLEXP_SELECTIONCOLOR)
    aegh.dwFlags&=~AEGHF_NOSELECTION;
  if (dwFlags & HLEXP_ACTIVELINECOLOR)
    aegh.dwFlags&=~AEGHF_NOACTIVELINE;
  if (dwFlags & HLEXP_ALTLINECOLOR)
    aegh.dwFlags&=~AEGHF_NOALTLINE;

  aegh.dwCookie=(UINT_PTR)&rtf;
  aegh.lpCallback=GetHighLightCallback;
  xmemcpy(&aegh.crText, crRange, sizeof(AECHARRANGE));
  SendMessage(hWndEdit, AEM_HLGETHIGHLIGHT, 0, (LPARAM)&aegh);

  if (!rtf.szRtf)
  {
    //Get RTF table size
    nTableLen=(int)xprintfA(NULL, "{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0\\fcharset%d %S;}}\r\n{\\colortbl;", rtf.lfFont.lfCharSet, rtf.lfFont.lfFaceName);
    nTableLen+=rtf.hColorStack.nTableLen;
    nTableLen+=(int)xprintfA(NULL, "}\r\n\\f0\\fs%d%s\\ulnone%s%s ", rtf.nPointSize * 2, rtf.szDefStyle, rtf.szDefTextColor, rtf.szDefBkColor);

    if (hRtf=GlobalAlloc(GMEM_MOVEABLE, (nTableLen + rtf.nRtfLen + 3 + 1) * sizeof(char)))
    {
      if (rtf.szRtf=(char *)GlobalLock(hRtf))
      {
        RTFCOLORITEM *lpColorItem;

        //Get RTF table text
        rtf.nRtfLen=xprintfA(rtf.szRtf, "{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0\\fcharset%d %S;}}\r\n{\\colortbl;", rtf.lfFont.lfCharSet, rtf.lfFont.lfFaceName);
        for (lpColorItem=rtf.hColorStack.first; lpColorItem; lpColorItem=lpColorItem->next)
          rtf.nRtfLen+=xstrcpyA(rtf.szRtf + rtf.nRtfLen, lpColorItem->szRGB);
        rtf.nRtfLen+=xprintfA(rtf.szRtf + rtf.nRtfLen, "}\r\n\\f0\\fs%d%s\\ulnone%s%s ", rtf.nPointSize * 2, rtf.szDefStyle, rtf.szDefTextColor, rtf.szDefBkColor);
      }
      else
      {
        GlobalFree(hRtf);
        hRtf=NULL;
      }
    }
  }
  if (rtf.szRtf)
  {
    //Get RTF text
    SendMessage(hWndEdit, AEM_HLGETHIGHLIGHT, 0, (LPARAM)&aegh);

    //Close RTF
    rtf.nRtfLen+=xstrcpyA(rtf.szRtf + rtf.nRtfLen, "}\r\n");
    GlobalUnlock(hRtf);
  }
  StackFreeRTFColors(&rtf.hColorStack);
  if (lpnRtfLen) *lpnRtfLen=rtf.nRtfLen;
  return hRtf;
}

DWORD CALLBACK GetHighLightCallback(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp)
{
  RTFDATA *rtf=(RTFDATA *)dwCookie;
  char szColorText[32];
  char szColorBk[32];
  char *pCurStyle;
  char *szRange;
  char *pNewLine;
  wchar_t *wpLineCount;
  wchar_t *wpLineMin;
  wchar_t *wpLineMax;
  int nIndexText;
  int nIndexBk;
  INT_PTR nRtfLen;

  szColorText[0]=L'\0';
  szColorBk[0]=L'\0';
  pCurStyle="";
  szRange=NULL;
  pNewLine=NULL;

  if (crRichRange->cpMin < crRichRange->cpMax)
  {
    //Current text color
    //if (hlp->dwActiveText != hlp->dwDefaultText)
    if (hlp->dwActiveText != rtf->aec.crBasicText)
    {
      if (!(nIndexText=StackGetRTFColor(&rtf->hColorStack, hlp->dwActiveText)))
        nIndexText=StackInsertRTFColor(&rtf->hColorStack, hlp->dwActiveText);
      if (nIndexText)
        xprintfA(szColorText, "\\cf%d", nIndexText);
    }

    //Current background color
    //if (hlp->dwActiveBk != hlp->dwDefaultBk)
    if (hlp->dwActiveBk != rtf->aec.crBasicBk)
    {
      if (!(nIndexBk=StackGetRTFColor(&rtf->hColorStack, hlp->dwActiveBk)))
        nIndexBk=StackInsertRTFColor(&rtf->hColorStack, hlp->dwActiveBk);
      if (nIndexBk)
        xprintfA(szColorBk, "\\highlight%d\\chcbpat%d", nIndexBk, nIndexBk);
    }

    //Current font style
    if (hlp->dwFontStyle)
    {
      if (hlp->dwFontStyle == AEHLS_FONTNORMAL)
        pCurStyle="\\b0\\i0";
      else if (hlp->dwFontStyle == AEHLS_FONTBOLD)
        pCurStyle="\\b\\i0";
      else if (hlp->dwFontStyle == AEHLS_FONTITALIC)
        pCurStyle="\\b0\\i";
      else if (hlp->dwFontStyle == AEHLS_FONTBOLDITALIC)
        pCurStyle="\\b\\i";
    }

    //Calculate RTF range length
    wpLineMin=crAkelRange->ciMin.lpLine->wpLine + crAkelRange->ciMin.nCharInLine;
    wpLineMax=wpLineMin + (crRichRange->cpMax - crRichRange->cpMin);

    Loop:
    for (wpLineCount=wpLineMin, nRtfLen=0; wpLineCount < wpLineMax; ++wpLineCount)
    {
      if (*wpLineCount > 0x80)
      {
        nRtfLen+=xprintfA(szRange?szRange + nRtfLen:NULL, "\\u%d?", *wpLineCount);
      }
      else if (*wpLineCount == L'\t')
      {
        if (szRange) xstrcpyA(szRange + nRtfLen, "\\tab ");
        nRtfLen+=5;
      }
      else
      {
        if (*wpLineCount == L'{' ||
            *wpLineCount == L'}' ||
            *wpLineCount == L'\\')
        {
          if (szRange) szRange[nRtfLen]='\\';
          ++nRtfLen;
        }
        if (szRange) szRange[nRtfLen]=(char)*wpLineCount;
        ++nRtfLen;
      }
    }
    if (!szRange)
    {
      if (!(szRange=(char *)GlobalAlloc(GPTR, nRtfLen + 1)))
        return 1;
      //Retrieve RTF range
      goto Loop;
    }
  }

  if (AEC_IsLastCharInLine(&crAkelRange->ciMax) && AEC_IndexCompare(&rtf->crRange.ciMax, &crAkelRange->ciMax))
    pNewLine="\\par\r\n";
  if (szColorText[0] || szColorBk[0] || pCurStyle[0] || (hlp->dwPaintType & AEHPT_LINK))
    rtf->nRtfLen+=xprintfA(rtf->szRtf?rtf->szRtf + rtf->nRtfLen:NULL, "%s%s%s%s %s%s%s%s%s%s", pCurStyle, (hlp->dwPaintType & AEHPT_LINK)?"\\ul":"", szColorText, szColorBk, szRange, szColorBk[0]?rtf->szDefBkColor:"", szColorText[0]?rtf->szDefTextColor:"", (hlp->dwPaintType & AEHPT_LINK)?"\\ulnone":"", pCurStyle[0]?rtf->szDefStyle:"", pNewLine?pNewLine:" ");
  else
    rtf->nRtfLen+=xprintfA(rtf->szRtf?rtf->szRtf + rtf->nRtfLen:NULL, "%s%s", szRange, pNewLine);

  if (szRange) GlobalFree((HGLOBAL)szRange);
  return 0;
}

int StackInsertRTFColor(STACKRTFCOLOR *hStack, DWORD dwColor)
{
  RTFCOLORITEM *lpColorItem;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpColorItem, -1, sizeof(RTFCOLORITEM)))
  {
    lpColorItem->dwColor=dwColor;
    lpColorItem->nRGBLen=(int)xprintfA(lpColorItem->szRGB, "\\red%d\\green%d\\blue%d;", GetRValue(dwColor), GetGValue(dwColor), GetBValue(dwColor));
    hStack->nTableLen+=lpColorItem->nRGBLen;
    ++hStack->nCount;
  }
  return hStack->nCount;
}

int StackGetRTFColor(STACKRTFCOLOR *hStack, DWORD dwColor)
{
  RTFCOLORITEM *lpColorItem;
  int nIndex=1;

  for (lpColorItem=hStack->first; lpColorItem; lpColorItem=lpColorItem->next)
  {
    if (lpColorItem->dwColor == dwColor)
      return nIndex;
    ++nIndex;
  }
  return 0;
}

void StackFreeRTFColors(STACKRTFCOLOR *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}


//// Settings sheet

void SettingsSheet(int nStartPage)
{
  HICON hIconHighLight=NULL;
  HICON hIconCodeFold1=NULL;
  HICON hIconCodeFold2=NULL;
  HICON hIconAutoComplete1=NULL;
  HICON hIconAutoComplete2=NULL;
  POINT ptSmallIcon;

  //Create image list
  ptSmallIcon.x=16 /*GetSystemMetrics(SM_CXSMICON)*/;
  ptSmallIcon.y=16 /*GetSystemMetrics(SM_CYSMICON)*/;

  if (hImageList=ImageList_Create(ptSmallIcon.x, ptSmallIcon.y, ILC_COLOR32|ILC_MASK, 0, 0))
  {
    hIconHighLight=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_HIGHLIGHT_ICON), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconCodeFold1=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_CODEFOLD_ICON), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconCodeFold2=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_CODEFOLD_ICON), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconAutoComplete1=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_AUTOCOMPLETE_ICON), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconAutoComplete2=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_AUTOCOMPLETE_ICON), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    ImageList_AddIcon(hImageList, hIconHighLight);
    ImageList_AddIcon(hImageList, hIconCodeFold1);
    ImageList_AddIcon(hImageList, hIconCodeFold2);
    ImageList_AddIcon(hImageList, hIconAutoComplete1);
    ImageList_AddIcon(hImageList, hIconAutoComplete2);
  }

  bSaveOptions=FALSE;

  //Show property sheet
  psp[PAGE_GENERAL].dwSize            =sizeof(PROPSHEETPAGEW);
  psp[PAGE_GENERAL].dwFlags           =PSP_DEFAULT|PSP_USETITLE;
  psp[PAGE_GENERAL].hInstance         =hInstanceDLL;
  psp[PAGE_GENERAL].pszTemplate       =MAKEINTRESOURCEW(IDD_GENERAL_SETUP);
  psp[PAGE_GENERAL].pszTitle          =GetLangStringW(wLangModule, STRID_GENERAL);
  psp[PAGE_GENERAL].pfnDlgProc        =(DLGPROC)GeneralSetupDlgProc;

  psp[PAGE_HIGHLIGHT].dwSize          =sizeof(PROPSHEETPAGEW);
  psp[PAGE_HIGHLIGHT].dwFlags         =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_HIGHLIGHT].hInstance       =hInstanceDLL;
  psp[PAGE_HIGHLIGHT].pszTemplate     =MAKEINTRESOURCEW(IDD_HIGHLIGHT_SETUP);
  psp[PAGE_HIGHLIGHT].hIcon           =hIconHighLight;
  psp[PAGE_HIGHLIGHT].pfnDlgProc      =(DLGPROC)HighLightSetupDlgProc;

  psp[PAGE_CODEFOLD1].dwSize          =sizeof(PROPSHEETPAGEW);
  psp[PAGE_CODEFOLD1].dwFlags         =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_CODEFOLD1].hInstance       =hInstanceDLL;
  psp[PAGE_CODEFOLD1].pszTemplate     =MAKEINTRESOURCEW(IDD_CODEFOLD1_SETUP);
  psp[PAGE_CODEFOLD1].hIcon           =hIconCodeFold1;
  psp[PAGE_CODEFOLD1].pfnDlgProc      =(DLGPROC)CodeFold1SetupDlgProc;

  psp[PAGE_CODEFOLD2].dwSize          =sizeof(PROPSHEETPAGEW);
  psp[PAGE_CODEFOLD2].dwFlags         =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_CODEFOLD2].hInstance       =hInstanceDLL;
  psp[PAGE_CODEFOLD2].pszTemplate     =MAKEINTRESOURCEW(IDD_CODEFOLD2_SETUP);
  psp[PAGE_CODEFOLD2].hIcon           =hIconCodeFold2;
  psp[PAGE_CODEFOLD2].pfnDlgProc      =(DLGPROC)CodeFold2SetupDlgProc;

  psp[PAGE_AUTOCOMPLETE1].dwSize      =sizeof(PROPSHEETPAGEW);
  psp[PAGE_AUTOCOMPLETE1].dwFlags     =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_AUTOCOMPLETE1].hInstance   =hInstanceDLL;
  psp[PAGE_AUTOCOMPLETE1].pszTemplate =MAKEINTRESOURCEW(IDD_AUTOCOMPLETE1_SETUP);
  psp[PAGE_AUTOCOMPLETE1].hIcon       =hIconAutoComplete1;
  psp[PAGE_AUTOCOMPLETE1].pfnDlgProc  =(DLGPROC)AutoCompleteSetup1DlgProc;

  psp[PAGE_AUTOCOMPLETE2].dwSize      =sizeof(PROPSHEETPAGEW);
  psp[PAGE_AUTOCOMPLETE2].dwFlags     =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_AUTOCOMPLETE2].hInstance   =hInstanceDLL;
  psp[PAGE_AUTOCOMPLETE2].pszTemplate =MAKEINTRESOURCEW(IDD_AUTOCOMPLETE2_SETUP);
  psp[PAGE_AUTOCOMPLETE2].hIcon       =hIconAutoComplete2;
  psp[PAGE_AUTOCOMPLETE2].pfnDlgProc  =(DLGPROC)AutoCompleteSetup2DlgProc;

  psh.dwSize      =sizeof(PROPSHEETHEADERW);
  psh.dwFlags     =PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_USECALLBACK;
  psh.hwndParent  =hMainWnd;
  psh.hInstance   =hInstanceEXE;
  psh.pszIcon     =MAKEINTRESOURCEW(IDI_ICON_MAIN);
  psh.pszCaption  =wszPluginTitle;
  psh.nPages      =PAGE_MAX;
  psh.nStartPage  =nStartPage;
  psh.ppsp        =&psp[0];
  psh.pfnCallback =PropSheetProc;

  hHookOptions=SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
  nPropMaxVisitPage=nStartPage;
  PropertySheetWide(&psh);

  if (bSaveOptions)
    UpdateAllOptions();

  //Destroy image list
  if (hImageList)
  {
    if (ImageList_Destroy(hImageList))
      hImageList=NULL;
  }
}

LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam)
{
  //Center PropertySheet
  if (iCode == HCBT_ACTIVATE)
  {
    RECT rcEdit;
    RECT rcSheet;

    if (hHookOptions)
    {
      if (UnhookWindowsHookEx(hHookOptions))
        hHookOptions=NULL;
    }
    GetWindowRect(hMainWnd, &rcEdit);
    GetWindowRect((HWND)wParam, &rcSheet);
    rcSheet.left=rcEdit.left + ((rcEdit.right - rcEdit.left) / 2) - ((rcSheet.right - rcSheet.left) / 2);
    rcSheet.top=rcEdit.top + ((rcEdit.bottom - rcEdit.top) / 2) - ((rcSheet.bottom - rcSheet.top) / 2);
    if (rcSheet.left < 0) rcSheet.left=0;
    if (rcSheet.top < 0) rcSheet.top=0;

    SetWindowPos((HWND)wParam, NULL, rcSheet.left, rcSheet.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
  }
  return CallNextHookEx(hHookOptions, iCode, wParam, lParam);
}

int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam)
{
  static HWND hWndPropTab;

  if (uMsg == PSCB_PRECREATE)
  {
    //Remove "?"
    ((DLGTEMPLATE *)lParam)->style&=~DS_CONTEXTHELP;
  }
  else if (uMsg == PSCB_INITIALIZED)
  {
    HIMAGELIST hImageListOld;

    hWndPropSheet=hDlg;

    //Set 32-bit hImageList
    if (hWndPropTab=(HWND)SendMessage(hDlg, PSM_GETTABCONTROL, 0, 0))
    {
      hImageListOld=(HIMAGELIST)SendMessage(hWndPropTab, TCM_SETIMAGELIST, 0, (LPARAM)hImageList);
      if (hImageListOld) ImageList_Destroy(hImageListOld);
    }
  }
  else if (uMsg == PSCB_BUTTONPRESSED)
  {
    if (lParam == PSBTN_OK ||
        lParam == PSBTN_CANCEL ||
        lParam == PSBTN_FINISH)
    {
      //Detach hImageList otherwise ImageList_Destroy failed
      SendMessage(hWndPropTab, TCM_SETIMAGELIST, 0, (LPARAM)NULL);
    }
  }
  return TRUE;
}

BOOL CALLBACK GeneralSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCacheCombo;
  static HWND hWndDefaultAliasEnable;
  static HWND hWndDefaultAliasEdit;
  static HWND hWndVarThemeName;
  static HWND hWndVarThemeNameEdit;
  static HWND hWndVarThemeSave;
  static HWND hWndVarThemeDelete;
  static HWND hWndVarThemeList;
  static HMENU hMenuList;
  static VARTHEME vtCur;
  static VARTHEME *lpCurVarTheme;
  static int nCurThemeIndex;
  static int nPrevThemeIndex;
  static BOOL bVarThemeGlobal;
  static BOOL bVarThemeChanged;

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndCacheCombo=GetDlgItem(hDlg, IDC_GENERAL_CACHE);
    hWndDefaultAliasEnable=GetDlgItem(hDlg, IDC_GENERAL_DEFAULTALIAS_ENABLE);
    hWndDefaultAliasEdit=GetDlgItem(hDlg, IDC_GENERAL_DEFAULTALIAS_EDIT);
    hWndVarThemeName=GetDlgItem(hDlg, IDC_GENERAL_VARTHEME_NAME);
    hWndVarThemeNameEdit=GetDlgItem(hWndVarThemeName, IDC_COMBOBOXEDIT);
    hWndVarThemeSave=GetDlgItem(hDlg, IDC_GENERAL_VARTHEME_SAVE);
    hWndVarThemeDelete=GetDlgItem(hDlg, IDC_GENERAL_VARTHEME_DELETE);
    hWndVarThemeList=GetDlgItem(hDlg, IDC_GENERAL_VARTHEME_LIST);

    SetDlgItemTextWide(hDlg, IDC_GENERAL_DEFAULTALIAS_ENABLE, GetLangStringW(wLangModule, STRID_DEFAULTALIAS_ENABLE));
    SetDlgItemTextWide(hDlg, IDC_GENERAL_VARTHEME_NAME_LABEL, GetLangStringW(wLangModule, STRID_VARTHEME_NAME));
    SetDlgItemTextWide(hDlg, IDC_GENERAL_VARTHEME_SAVE, GetLangStringW(wLangModule, STRID_VARTHEME_SAVE));
    SetDlgItemTextWide(hDlg, IDC_GENERAL_VARTHEME_DELETE, GetLangStringW(wLangModule, STRID_VARTHEME_DELETE));
    SetDlgItemTextWide(hDlg, IDC_GENERAL_VARTHEME_LINK, GetLangStringW(wLangModule, STRID_VARTHEME_LINK));

    SendMessage(hWndVarThemeList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
    EnableWindow(hWndVarThemeSave, FALSE);
    EnableWindow(hWndVarThemeDelete, FALSE);

    ComboBox_AddStringWide(hWndCacheCombo, GetLangStringW(wLangModule, STRID_CACHE_NONE));
    ComboBox_AddStringWide(hWndCacheCombo, GetLangStringW(wLangModule, STRID_CACHE_FAST));
    ComboBox_AddStringWide(hWndCacheCombo, GetLangStringW(wLangModule, STRID_CACHE_SMART));
    SendMessage(hWndCacheCombo, CB_SETCURSEL, dwUseCache, 0);

    if (bDefaultAliasEnable)
      SendMessage(hWndDefaultAliasEnable, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndDefaultAliasEdit, FALSE);
    SetWindowTextWide(hWndDefaultAliasEdit, wszDefaultAlias);

    //Popup menu
    if (hMenuList=CreatePopupMenu())
    {
      AppendMenuWide(hMenuList, MF_STRING, IDC_GENERAL_ADDITEM, GetLangStringW(wLangModule, STRID_MENU_ADDITEM));
      AppendMenuWide(hMenuList, MF_STRING, IDC_GENERAL_MODITEM, GetLangStringW(wLangModule, STRID_MENU_MODITEM));
      AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuList, MF_STRING, IDC_GENERAL_MOVEUPITEM, GetLangStringW(wLangModule, STRID_MENU_MOVEUPITEM));
      AppendMenuWide(hMenuList, MF_STRING, IDC_GENERAL_MOVEDOWNITEM, GetLangStringW(wLangModule, STRID_MENU_MOVEDOWNITEM));
      AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
      AppendMenuWide(hMenuList, MF_STRING, IDC_GENERAL_DELITEM, GetLangStringW(wLangModule, STRID_MENU_DELITEM));
    }

    //Columns
    {
      LVCOLUMNW lvc;

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_VARIABLE);
      lvc.cx=nColumnWidth1;
      lvc.iSubItem=LVSI_LIST_VARIABLE;
      ListView_InsertColumnWide(hWndVarThemeList, LVSI_LIST_VARIABLE, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_VALUE);
      lvc.cx=nColumnWidth2;
      lvc.iSubItem=LVSI_LIST_VALUE;
      ListView_InsertColumnWide(hWndVarThemeList, LVSI_LIST_VALUE, &lvc);

      lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_COLOR);
      lvc.cx=nColumnWidth3;
      lvc.iSubItem=LVSI_LIST_COLOR;
      ListView_InsertColumnWide(hWndVarThemeList, LVSI_LIST_COLOR, &lvc);
    }

    //Fill ComboBox
    {
      VARTHEME *lpVarTheme;
      int nIndex=1;

      //Add var themes
      RequestVarTheme(&hVarThemesStack, NULL);

      nCurThemeIndex=1;
      lpCurVarTheme=lpVarThemeActive;
      bVarThemeChanged=FALSE;
      bTopVarOpenScroll=TRUE;

      //Global theme
      xprintfW(wszBuffer, L"<%s>", hVarThemeGlobal.wszVarThemeName);
      ComboBox_AddStringWide(hWndVarThemeName, wszBuffer);

      //Other themes
      for (lpVarTheme=hVarThemesStack.first; lpVarTheme; lpVarTheme=lpVarTheme->next)
      {
        if (nIndex < nVarThemesInternalCount + 1)
        {
          xprintfW(wszBuffer, L"<%s>", lpVarTheme->wszVarThemeName);
          ComboBox_AddStringWide(hWndVarThemeName, wszBuffer);
        }
        else ComboBox_AddStringWide(hWndVarThemeName, lpVarTheme->wszVarThemeName);

        if (lpVarThemeActive == lpVarTheme)
          nCurThemeIndex=nIndex;
        ++nIndex;
      }
      SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)nCurThemeIndex, 0);
      PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_GENERAL_VARTHEME_NAME, CBN_SELCHANGE), 0);
    }
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndVarThemeList)
    {
      LVHITTESTINFO lvhti;
      LVITEMW lvi;
      POINT ptScreen={0};
      RECT rcItem;

      if (lParam == -1)
      {
        if ((lvhti.iItem=(int)SendMessage(hWndVarThemeList, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED)) != -1)
        {
          rcItem.left=LVIR_LABEL;
          SendMessage(hWndVarThemeList, LVM_GETITEMRECT, (WPARAM)lvhti.iItem, (LPARAM)&rcItem);
          ptScreen.x=rcItem.left;
          ptScreen.y=rcItem.bottom;
        }
        ClientToScreen(hWndVarThemeList, &ptScreen);
      }
      else
      {
        GetCursorPos(&ptScreen);
        lvhti.pt=ptScreen;
        ScreenToClient(hWndVarThemeList, &lvhti.pt);
        SendMessage(hWndVarThemeList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        lvi.stateMask=LVIS_SELECTED;
        lvi.state=LVIS_SELECTED;
        SendMessage(hWndVarThemeList, LVM_SETITEMSTATE, (WPARAM)lvhti.iItem, (LPARAM)&lvi);
      }

      EnableMenuItem(hMenuList, IDC_GENERAL_MODITEM, lvhti.iItem >= 0?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_GENERAL_MOVEDOWNITEM, lvhti.iItem >= 0?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_GENERAL_MOVEUPITEM, lvhti.iItem >= 0?MF_ENABLED:MF_GRAYED);
      EnableMenuItem(hMenuList, IDC_GENERAL_DELITEM, lvhti.iItem >= 0?MF_ENABLED:MF_GRAYED);
      TrackPopupMenu(hMenuList, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 0, hDlg, NULL);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_GENERAL_VARTHEME_NAME)
    {
      if (HIWORD(wParam) == CBN_EDITCHANGE)
      {
        VARTHEME *lpVarTheme=NULL;
        int nVarThemeIndex=-1;
        int nNameLen;
        int i;

        if (nNameLen=GetWindowTextWide(hWndVarThemeName, wszBuffer, BUFFER_SIZE))
        {
          if (lpVarTheme=StackGetVarThemeByName(&hVarThemesStack, wszBuffer))
            nVarThemeIndex=StackGetVarThemeIndex(&hVarThemesStack, lpVarTheme) - 1;
          else
          {
            for (i=0; wszBuffer[i]; ++i)
            {
              if (wszBuffer[i] == L'<' || wszBuffer[i] == L'>')
                break;
            }
            //Name must not contain < and > characters
            if (wszBuffer[i])
              nNameLen=0;
          }
        }

        if ((nVarThemeIndex >= 0 && nVarThemeIndex < nVarThemesInternalCount + 1) || !nNameLen)
        {
          EnableWindow(hWndVarThemeSave, FALSE);
          EnableWindow(hWndVarThemeDelete, FALSE);
        }
        else
        {
          EnableWindow(hWndVarThemeSave, TRUE);
          EnableWindow(hWndVarThemeDelete, nVarThemeIndex > 0?TRUE:FALSE);
        }
      }
      else if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        if (bVarThemeChanged)
        {
          xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_LOSTPROMPT), lpCurVarTheme?lpCurVarTheme->wszVarThemeName:L"");
          if (MessageBoxW(hDlg, wszMessage, wszPluginTitle, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDNO)
          {
            SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)nCurThemeIndex, 0);
            return 0;
          }
          bVarThemeChanged=FALSE;
        }
        if (bVarThemeGlobal)
        {
          SetWindowTextWide(hWndVarThemeDelete, GetLangStringW(wLangModule, STRID_VARTHEME_DELETE));
          SendMessage(hWndVarThemeNameEdit, EM_SETREADONLY, FALSE, 0);
        }
        lpCurVarTheme=NULL;
        bVarThemeGlobal=FALSE;
        nPrevThemeIndex=nCurThemeIndex;

        if ((nCurThemeIndex=(int)SendMessage(hWndVarThemeName, CB_GETCURSEL, 0, 0)) != CB_ERR)
        {
          if (nCurThemeIndex == 0)
          {
            lpCurVarTheme=&hVarThemeGlobal;
            bVarThemeGlobal=TRUE;
            SendMessage(hWndVarThemeNameEdit, EM_SETREADONLY, TRUE, 0);
          }
          else if (nCurThemeIndex < nVarThemesInternalCount + 1)
          {
            lpCurVarTheme=StackGetVarThemeByIndex(&hVarThemesStack, nCurThemeIndex);
          }
          else
          {
            if (ComboBox_GetLBTextWide(hWndVarThemeName, nCurThemeIndex, wszBuffer))
              lpCurVarTheme=StackGetVarThemeByName(&hVarThemesStack, wszBuffer);
          }

          if (lpCurVarTheme)
          {
            StackCopyVarTheme(lpCurVarTheme, &vtCur);

            if (!bTopVarOpenScroll)
            {
              RECT rc;

              SendMessage(hWndVarThemeList, LVM_GETVIEWRECT, 0, (LPARAM)&rc);
              ptTopVarIndex.x=mod(rc.right);
              ptTopVarIndex.y=mod(rc.bottom);
            }
            else bTopVarOpenScroll=FALSE;

            FillVarThemeList(hWndVarThemeList, &vtCur);
            InvalidateRect(hWndVarThemeList, NULL, FALSE);

            SendMessage(hWndVarThemeList, LVM_SCROLL, (WPARAM)ptTopVarIndex.x, (LPARAM)ptTopVarIndex.y);
          }
        }

        EnableWindow(hWndVarThemeSave, FALSE);
        if (nCurThemeIndex == 0)
        {
          EnableWindow(hWndVarThemeDelete, TRUE);
          SetWindowTextWide(hWndVarThemeDelete, GetLangStringW(wLangModule, STRID_VARTHEME_RETURN));
        }
        else if (nCurThemeIndex > 0 && nCurThemeIndex < nVarThemesInternalCount + 1)
          EnableWindow(hWndVarThemeDelete, FALSE);
        else
          EnableWindow(hWndVarThemeDelete, TRUE);

        if (!bVarThemeGlobal && lpVarThemeActive != lpCurVarTheme)
          SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_GENERAL_VARTHEME_SAVE)
    {
      wchar_t wszThemeName[MAX_PATH];
      VARTHEME *lpVarTheme=NULL;
      BOOL bProcess=FALSE;

      if (bVarThemeGlobal)
      {
        bProcess=TRUE;
      }
      else if (GetWindowTextWide(hWndVarThemeName, wszThemeName, MAX_PATH))
      {
        if (lpVarTheme=StackGetVarThemeByName(&hVarThemesStack, wszThemeName))
        {
          if (SendMessage(hWndVarThemeNameEdit, EM_GETMODIFY, 0, 0))
          {
            xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_OVERWRITEPROMPT), lpVarTheme->wszVarThemeName);
            if (MessageBoxW(hDlg, wszMessage, wszPluginTitle, MB_YESNO|MB_ICONQUESTION) == IDYES)
              bProcess=TRUE;
          }
          else bProcess=TRUE;
        }
        else
        {
          lpVarTheme=StackInsertVarTheme(&hVarThemesStack, -1);
          ComboBox_AddStringWide(hWndVarThemeName, wszThemeName);
          bProcess=TRUE;
        }
        if (bProcess)
        {
          vtCur.nVarThemeNameLen=(int)xstrcpynW(vtCur.wszVarThemeName, wszThemeName, MAX_PATH);
          lpCurVarTheme=lpVarTheme;
        }
      }

      if (bProcess)
      {
        StackCopyVarTheme(&vtCur, lpCurVarTheme);
        SetFocus(hWndVarThemeName);
        SendMessage(hWndVarThemeNameEdit, EM_SETMODIFY, FALSE, 0);
        EnableWindow(hWndVarThemeSave, FALSE);
        EnableWindow(hWndVarThemeDelete, !bVarThemeGlobal);
        bVarThemeChanged=FALSE;
      }
    }
    else if (LOWORD(wParam) == IDC_GENERAL_VARTHEME_DELETE)
    {
      VARTHEME *lpVarTheme;
      int nItem;

      if (bVarThemeGlobal)
      {
        //Switch to previous theme
        SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)nPrevThemeIndex, 0);
        PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_GENERAL_VARTHEME_NAME, CBN_SELCHANGE), 0);
      }
      else
      {
        if (GetWindowTextWide(hWndVarThemeName, wszBuffer, BUFFER_SIZE))
        {
          if (lpVarTheme=StackGetVarThemeByName(&hVarThemesStack, wszBuffer))
          {
            xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_DELETEPROMPT), lpVarTheme->wszVarThemeName);
            if (MessageBoxW(hDlg, wszMessage, wszPluginTitle, MB_YESNO|MB_ICONQUESTION) == IDYES)
            {
              nItem=ComboBox_FindStringWide(hWndVarThemeName, -1, lpVarTheme->wszVarThemeName);
              SendMessageW(hWndVarThemeName, CB_DELETESTRING, nItem, 0);
              StackDeleteVarTheme(&hVarThemesStack, lpVarTheme);
              SetFocus(hWndVarThemeName);
              bVarThemeChanged=FALSE;

              //Set to default theme
              SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)1, 0);
              PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_GENERAL_VARTHEME_NAME, CBN_SELCHANGE), 0);
            }
          }
        }
      }
    }
    else if (LOWORD(wParam) == IDC_GENERAL_VARTHEME_LINK)
    {
      INT_PTR nResult;

      nResult=DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_GENERAL_LINK), hDlg, (DLGPROC)GeneralLinkDlgProc, 0);

      if (nResult)
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }
    else if (LOWORD(wParam) == IDC_GENERAL_ADDITEM ||
             LOWORD(wParam) == IDC_GENERAL_MODITEM ||
             LOWORD(wParam) == IDC_GENERAL_MOVEUPITEM ||
             LOWORD(wParam) == IDC_GENERAL_MOVEDOWNITEM ||
             LOWORD(wParam) == IDC_GENERAL_DELITEM)
    {
      LVITEMW lvi;
      VARINFO *lpVarInfo;
      wchar_t wszVarName[MAX_PATH];
      wchar_t wszVarValue[MAX_PATH];
      int nSelItem;
      int nCmd=LOWORD(wParam);
      BOOL bListChanged=FALSE;

      //Selected item
      nSelItem=(int)SendMessage(hWndVarThemeList, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);
      lpVarInfo=(VARINFO *)ListViewItemParam(hWndVarThemeList, nSelItem);

      if (lpVarInfo || nCmd == IDC_GENERAL_ADDITEM)
      {
        if (nCmd == IDC_GENERAL_DELITEM)
        {
          StackDeleteVar(&vtCur.hVarStack, lpVarInfo);
          SendMessage(hWndVarThemeList, LVM_DELETEITEM, (WPARAM)nSelItem, 0);
          bListChanged=TRUE;
        }
        else
        {
          if (nCmd == IDC_GENERAL_ADDITEM ||
              nCmd == IDC_GENERAL_MODITEM)
          {
            VARINFO vi;
            INT_PTR nResult;

            //Because vi.next and vi.prev are unused, use them to transfer some local variables
            xmemset(&vi, 0, sizeof(VARINFO));
            vi.next=(VARINFO *)(UINT_PTR)nCmd;
            vi.prev=(VARINFO *)(UINT_PTR)bVarThemeGlobal;
            vi.wpVarName=wszVarName;
            vi.wpVarValue=wszVarValue;
            if (lpVarInfo)
            {
              xstrcpynW(wszVarName, lpVarInfo->wpVarName, MAX_PATH);
              xstrcpynW(wszVarValue, lpVarInfo->wpVarValue, MAX_PATH);
              vi.nVarNameLen=lpVarInfo->nVarNameLen;
              vi.nVarValueLen=lpVarInfo->nVarValueLen;
              vi.dwVarFlags=lpVarInfo->dwVarFlags;
            }
            nResult=DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_GENERAL_VAREDIT), hDlg, (DLGPROC)GeneralVarEditDlgProc, (LPARAM)&vi);
            if (!bVarThemeGlobal && vi.prev)
            {
              StackInsertVar(&hVarThemeGlobal.hVarStack, vi.wpVarName, vi.nVarNameLen, vi.wpVarValue, vi.nVarValueLen, FALSE);
              nResult=0;
            }

            SetFocus(hWndVarThemeList);

            if (nResult)
            {
              if (nCmd == IDC_GENERAL_ADDITEM)
              {
                if (lpVarInfo=StackInsertVar(&vtCur.hVarStack, vi.wpVarName, vi.nVarNameLen, vi.wpVarValue, vi.nVarValueLen, FALSE))
                {
                  lpVarInfo->dwVarFlags=vi.dwVarFlags;

                  lvi.mask=LVIF_TEXT|LVIF_STATE|LVIF_PARAM;
                  lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
                  lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
                  lvi.pszText=wszVarName;
                  lvi.cchTextMax=MAX_PATH;
                  lvi.iItem=(nSelItem > -1)?nSelItem + 1:0x7FFFFFFF;
                  lvi.iSubItem=LVSI_LIST_VARIABLE;
                  lvi.lParam=(LPARAM)lpVarInfo;
                  nSelItem=ListView_InsertItemWide(hWndVarThemeList, &lvi);
                }
              }
              else
              {
                GlobalFree((HGLOBAL)lpVarInfo->wpVarName);
                if (lpVarInfo->wpVarName=(wchar_t *)GlobalAlloc(GPTR, (vi.nVarNameLen + 1) * sizeof(wchar_t)))
                  lpVarInfo->nVarNameLen=(int)xstrcpynW(lpVarInfo->wpVarName, vi.wpVarName, vi.nVarNameLen + 1);
                GlobalFree((HGLOBAL)lpVarInfo->wpVarValue);
                if (lpVarInfo->wpVarValue=(wchar_t *)GlobalAlloc(GPTR, (vi.nVarValueLen + 1) * sizeof(wchar_t)))
                  lpVarInfo->nVarValueLen=(int)xstrcpynW(lpVarInfo->wpVarValue, vi.wpVarValue, vi.nVarValueLen + 1);
                lpVarInfo->dwVarFlags=vi.dwVarFlags;

                lvi.mask=LVIF_TEXT;
                lvi.pszText=wszVarName;
                lvi.cchTextMax=MAX_PATH;
                lvi.iItem=nSelItem;
                lvi.iSubItem=LVSI_LIST_VARIABLE;
                ListView_SetItemWide(hWndVarThemeList, &lvi);
              }
              lvi.mask=LVIF_TEXT;
              lvi.pszText=wszVarValue;
              lvi.cchTextMax=MAX_PATH;
              lvi.iItem=nSelItem;
              lvi.iSubItem=LVSI_LIST_VALUE;
              ListView_SetItemWide(hWndVarThemeList, &lvi);

              bListChanged=TRUE;
            }
          }
          else
          {
            //nCmd == IDC_GENERAL_MOVEUPITEM ||
            //nCmd == IDC_GENERAL_MOVEDOWNITEM
            SendMessage(hWndVarThemeList, LVM_DELETEITEM, (WPARAM)nSelItem, 0);

            if (nCmd == IDC_GENERAL_MOVEUPITEM)
            {
              if (lpVarInfo->prev)
              {
                nSelItem=max(nSelItem - 1, 0);
                StackMoveBefore((stack **)&vtCur.hVarStack.first, (stack **)&vtCur.hVarStack.last, (stack *)lpVarInfo, (stack *)lpVarInfo->prev);
              }
            }
            else if (nCmd == IDC_GENERAL_MOVEDOWNITEM)
            {
              if (lpVarInfo->next)
              {
                nSelItem=min(nSelItem + 1, (int)SendMessage(hWndVarThemeList, LVM_GETITEMCOUNT, 0, 0));
                StackMoveAfter((stack **)&vtCur.hVarStack.first, (stack **)&vtCur.hVarStack.last, (stack *)lpVarInfo, (stack *)lpVarInfo->next);
              }
            }
            lvi.mask=LVIF_TEXT|LVIF_STATE|LVIF_PARAM;
            lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
            lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
            lvi.pszText=lpVarInfo->wpVarName;
            lvi.iItem=nSelItem;
            lvi.iSubItem=LVSI_LIST_VARIABLE;
            lvi.lParam=(LPARAM)lpVarInfo;
            ListView_InsertItemWide(hWndVarThemeList, &lvi);

            lvi.mask=LVIF_TEXT;
            lvi.pszText=lpVarInfo->wpVarValue;
            lvi.iItem=nSelItem;
            lvi.iSubItem=LVSI_LIST_VALUE;
            ListView_SetItemWide(hWndVarThemeList, &lvi);

            bListChanged=TRUE;
          }
        }

        if (bListChanged)
        {
          if (nCurThemeIndex > 0 && nCurThemeIndex < nVarThemesInternalCount + 1)
          {
            lpCurVarTheme=NULL;
            nCurThemeIndex=-1;
            SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)nCurThemeIndex, 0);
            EnableWindow(hWndVarThemeSave, FALSE);
            EnableWindow(hWndVarThemeDelete, FALSE);
          }
          else EnableWindow(hWndVarThemeSave, TRUE);

          SendMessage(hWndVarThemeList, LVM_UPDATE, (WPARAM)nSelItem, 0);
          bVarThemeChanged=TRUE;
          SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_GENERAL_CACHE)
    {
      if (HIWORD(wParam) == CBN_SELCHANGE)
      {
        dwUseCache=(DWORD)SendMessage(hWndCacheCombo, CB_GETCURSEL, 0, 0);
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
      }
    }
    else if (LOWORD(wParam) == IDC_GENERAL_DEFAULTALIAS_ENABLE)
    {
      bDefaultAliasEnable=(BOOL)SendMessage(hWndDefaultAliasEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndDefaultAliasEdit, bDefaultAliasEnable);
      SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
    }
    else if (LOWORD(wParam) == IDC_GENERAL_DEFAULTALIAS_EDIT)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_GENERAL_VARTHEME_LIST)
    {
      if (((NMHDR *)lParam)->code == (UINT)NM_CUSTOMDRAW)
      {
        LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)lParam;
        LVITEMW lvi;
        VARINFO *lpVarInfo;
        LRESULT lResult;

        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
          lResult=CDRF_NOTIFYITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
          lResult=CDRF_NOTIFYSUBITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == (CDDS_SUBITEM|CDDS_ITEMPREPAINT))
        {
          if (!bVarThemeGlobal)
          {
            if (lplvcd->iSubItem == LVSI_LIST_VARIABLE ||
                lplvcd->iSubItem == LVSI_LIST_VALUE)
            {
              lvi.mask=LVIF_TEXT;
              lvi.pszText=wszBuffer;
              lvi.cchTextMax=BUFFER_SIZE;
              lvi.iItem=(int)lplvcd->nmcd.dwItemSpec;
              lvi.iSubItem=LVSI_LIST_VARIABLE;
              if (ListView_GetItemWide(hWndVarThemeList, &lvi))
              {
                if (lpVarInfo=StackGetVarByName(&hVarThemeGlobal.hVarStack, wszBuffer, -1))
                {
                  if (!(lpVarInfo->dwVarFlags & VARF_LOWPRIORITY))
                    lplvcd->clrText=GetSysColor(COLOR_GRAYTEXT);
                }
              }
            }
          }
          else
          {
            if (lpVarInfo=(VARINFO *)lplvcd->nmcd.lItemlParam)
            {
              if (lpVarInfo->dwVarFlags & VARF_LOWPRIORITY)
                lplvcd->clrText=GetSysColor(COLOR_GRAYTEXT);
            }
          }

          if (lplvcd->iSubItem == LVSI_LIST_COLOR)
          {
            lvi.mask=LVIF_TEXT;
            lvi.pszText=wszBuffer;
            lvi.cchTextMax=BUFFER_SIZE;
            lvi.iItem=(int)lplvcd->nmcd.dwItemSpec;
            lvi.iSubItem=LVSI_LIST_VALUE;
            if (ListView_GetItemWide(hWndVarThemeList, &lvi))
            {
              if (wszBuffer[0] == L'#')
              {
                lplvcd->clrTextBk=GetColorFromStr(wszBuffer + 1, NULL);
              }
            }
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, (LONG)lResult);
        return TRUE;
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_CLICK ||
               ((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        LVHITTESTINFO lvhti;
        LVITEMW lvi;
        VARINFO *lpVarInfo;
        COLORREF crColor;

        GetCursorPos(&lvhti.pt);
        ScreenToClient(hWndVarThemeList, &lvhti.pt);
        SendMessage(hWndVarThemeList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        if (lvhti.iItem >= 0)
        {
          if (((NMHDR *)lParam)->code == (UINT)NM_CLICK)
          {
            if (lvhti.iSubItem == LVSI_LIST_COLOR)
            {
              lvi.mask=LVIF_TEXT|LVIF_PARAM;
              lvi.pszText=wszBuffer;
              lvi.cchTextMax=BUFFER_SIZE;
              lvi.iItem=lvhti.iItem;
              lvi.iSubItem=LVSI_LIST_VALUE;
              if (ListView_GetItemWide(hWndVarThemeList, &lvi))
              {
                if (wszBuffer[0] == L'#')
                {
                  crColor=GetColorFromStr(wszBuffer + 1, NULL);
                  if (SelectColorDialog(hDlg, &crColor))
                  {
                    wszBuffer[0]=L'#';
                    GetStrFromColor(crColor, wszBuffer + 1);
                    if (lpVarInfo=(VARINFO *)lvi.lParam)
                    {
                      GlobalFree((HGLOBAL)lpVarInfo->wpVarValue);
                      lpVarInfo->nVarValueLen=(int)xstrlenW(wszBuffer);
                      if (lpVarInfo->wpVarValue=(wchar_t *)GlobalAlloc(GPTR, (lpVarInfo->nVarValueLen + 1) * sizeof(wchar_t)))
                        xstrcpynW(lpVarInfo->wpVarValue, wszBuffer, lpVarInfo->nVarValueLen + 1);
                    }
                    lvi.mask&=~LVIF_PARAM;
                    ListView_SetItemWide(hWndVarThemeList, &lvi);
                    SendMessage(hWndVarThemeList, LVM_UPDATE, (WPARAM)lvhti.iItem, 0);

                    if (nCurThemeIndex > 0 && nCurThemeIndex < nVarThemesInternalCount + 1)
                    {
                      lpCurVarTheme=NULL;
                      nCurThemeIndex=-1;
                      SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)nCurThemeIndex, 0);
                      EnableWindow(hWndVarThemeSave, FALSE);
                      EnableWindow(hWndVarThemeDelete, FALSE);
                    }
                    else EnableWindow(hWndVarThemeSave, TRUE);

                    bVarThemeChanged=TRUE;
                    SendMessage(hWndPropSheet, PSM_CHANGED, (WPARAM)hDlg, 0);
                  }
                }
              }
            }
          }
          else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
          {
            if (lvhti.iSubItem == LVSI_LIST_VARIABLE || lvhti.iSubItem == LVSI_LIST_VALUE)
            {
              lvi.stateMask=LVIS_SELECTED;
              lvi.state=LVIS_SELECTED;
              SendMessage(hWndVarThemeList, LVM_SETITEMSTATE, (WPARAM)lvhti.iItem, (LPARAM)&lvi);
              PostMessage(hDlg, WM_COMMAND, IDC_GENERAL_MODITEM, 0);
            }
          }
        }
      }
      else if (((NMHDR *)lParam)->code == LVN_KEYDOWN)
      {
        NMLVKEYDOWN *pnkd=(NMLVKEYDOWN *)lParam;
        BOOL bAlt=FALSE;
        BOOL bShift=FALSE;
        BOOL bControl=FALSE;

        if (GetKeyState(VK_MENU) < 0)
          bAlt=TRUE;
        if (GetKeyState(VK_SHIFT) < 0)
          bShift=TRUE;
        if (GetKeyState(VK_CONTROL) < 0)
          bControl=TRUE;

        if (pnkd->wVKey == VK_INSERT)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_GENERAL_ADDITEM, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_F2)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_GENERAL_MODITEM, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_DELETE)
        {
          if (!bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_GENERAL_DELITEM, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_UP)
        {
          if (bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_GENERAL_MOVEUPITEM, 0);
            return TRUE;
          }
        }
        else if (pnkd->wVKey == VK_DOWN)
        {
          if (bAlt && !bShift && !bControl)
          {
            PostMessage(hDlg, WM_COMMAND, IDC_GENERAL_MOVEDOWNITEM, 0);
            return TRUE;
          }
        }
      }
    }

    if (((NMHDR *)lParam)->code == (UINT)PSN_SETACTIVE)
    {
      if (nPropMaxVisitPage < PAGE_GENERAL)
        nPropMaxVisitPage=PAGE_GENERAL;
    }
    else if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      PSHNOTIFY *pshn=(PSHNOTIFY *)lParam;
      int nVarThemeIndex;

      if (bVarThemeChanged)
      {
        if (bVarThemeGlobal || GetWindowTextLengthWide(hWndVarThemeName))
        {
          SendMessage(hDlg, WM_COMMAND, IDC_GENERAL_VARTHEME_SAVE, 0);
        }
        else
        {
          xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_LOSTPROMPT), L"");
          if (MessageBoxW(hDlg, wszMessage, wszPluginTitle, MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2) == IDNO)
          {
            SetWindowLongPtrWide(hDlg, DWLP_MSGRESULT, (LONG)PSNRET_INVALID);
            return TRUE;
          }
        }
        bVarThemeChanged=FALSE;
      }
      if (!bVarThemeGlobal && lpCurVarTheme)
        lpVarThemeActive=lpCurVarTheme;
      GetWindowTextWide(hWndDefaultAliasEdit, wszDefaultAlias, MAX_PATH);

      if (pshn->lParam)
      {
        //OK button pressed
        bSaveOptions=TRUE;
      }
      else
      {
        //Apply button pressed
        if (nPropMaxVisitPage == PAGE_GENERAL)
          UpdateAllOptions();

        if (!bVarThemeGlobal)
        {
          nVarThemeIndex=StackGetVarThemeIndex(&hVarThemesStack, lpVarThemeActive);
          SendMessage(hWndVarThemeName, CB_SETCURSEL, (WPARAM)nVarThemeIndex, 0);
          PostMessage(hDlg, WM_COMMAND, MAKELONG(IDC_GENERAL_VARTHEME_NAME, CBN_SELCHANGE), 0);
        }
        return FALSE;
      }
    }

    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY ||
        ((NMHDR *)lParam)->code == (UINT)PSN_RESET)
    {
      RECT rc;

      SendMessage(hWndVarThemeList, LVM_GETVIEWRECT, 0, (LPARAM)&rc);
      ptTopVarIndex.x=mod(rc.right);
      ptTopVarIndex.y=mod(rc.bottom);

      DestroyMenu(hMenuList);
      StackFreeVars(&vtCur.hVarStack);
    }
  }
  return FALSE;
}

BOOL CALLBACK GeneralVarEditDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndName;
  static HWND hWndValue;
  static HWND hWndCheckbox;
  static HWND hWndOK;
  static VARINFO *vi;
  static int nCmd;
  static BOOL bVarThemeGlobal;

  if (uMsg == WM_INITDIALOG)
  {
    vi=(VARINFO *)lParam;
    nCmd=(int)(INT_PTR)vi->next;
    bVarThemeGlobal=(BOOL)(INT_PTR)vi->prev;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndName=GetDlgItem(hDlg, IDC_GENERAL_VARNAME);
    hWndValue=GetDlgItem(hDlg, IDC_GENERAL_VARVALUE);
    hWndCheckbox=GetDlgItem(hDlg, IDC_GENERAL_VARCHECKBOX);
    hWndOK=GetDlgItem(hDlg, IDOK);

    if (nCmd == IDC_GENERAL_ADDITEM)
      SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_ADDVAR));
    else
      SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_EDITVAR));
    SetDlgItemTextWide(hDlg, IDC_GENERAL_VARNAME_LABEL, GetLangStringW(wLangModule, STRID_VARIABLE));
    SetDlgItemTextWide(hDlg, IDC_GENERAL_VARVALUE_LABEL, GetLangStringW(wLangModule, STRID_VALUE));
    if (bVarThemeGlobal)
      SetDlgItemTextWide(hDlg, IDC_GENERAL_VARCHECKBOX, GetLangStringW(wLangModule, STRID_LOWPRIORITY));
    else
      SetDlgItemTextWide(hDlg, IDC_GENERAL_VARCHECKBOX, GetLangStringW(wLangModule, STRID_MAKEGLOBAL));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (bVarThemeGlobal)
    {
      if (vi->dwVarFlags & VARF_LOWPRIORITY)
        SendMessage(hWndCheckbox, BM_SETCHECK, BST_CHECKED, 0);
    }
    else
    {
      if (StackGetVarByName(&hVarThemeGlobal.hVarStack, vi->wpVarName, -1))
        ShowWindow(hWndCheckbox, SW_HIDE);
    }

    if (nCmd == IDC_GENERAL_MODITEM)
    {
      SetWindowTextWide(hWndName, vi->wpVarName);
      SetWindowTextWide(hWndValue, vi->wpVarValue);
    }
    else SetFocus(hWndName);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_GENERAL_VARNAME)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        EnableWindow(hWndOK, GetWindowTextLengthWide(hWndName) > 0);
      }
    }
    else if (LOWORD(wParam) == IDOK)
    {
      vi->nVarNameLen=GetWindowTextWide(hWndName, vi->wpVarName, MAX_PATH);
      vi->nVarValueLen=GetWindowTextWide(hWndValue, vi->wpVarValue, MAX_PATH);
      if (bVarThemeGlobal)
      {
        if (SendMessage(hWndCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED)
          vi->dwVarFlags|=VARF_LOWPRIORITY;
        else
          vi->dwVarFlags&=~VARF_LOWPRIORITY;
      }
      else
      {
        //Make global
        if (SendMessage(hWndCheckbox, BM_GETCHECK, 0, 0) == BST_CHECKED)
          vi->prev=(VARINFO *)(UINT_PTR)TRUE;
      }

      //Remove '"' from name to avoid save/read problems.
      xstrrepW(vi->wpVarName, -1, L"\"", -1, L"", -1, TRUE, vi->wpVarName, NULL);

      EndDialog(hDlg, 1);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, 0);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL CALLBACK GeneralLinkDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndLinkList;
  static HWND hWndClose;
  static HMENU hMenuList;
  static int nFirstVarThemeIndex;
  static int nLastVarThemeIndex;
  static BOOL bListChanged;
  static RESIZEDIALOG rds[]={{&hWndLinkList,        RDS_SIZE|RDS_X, 0},
                             {&hWndLinkList,        RDS_SIZE|RDS_Y, 0},
                             {&hWndClose,           RDS_MOVE|RDS_X, 0},
                             {&hWndClose,           RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hMainIcon);
    hWndLinkList=GetDlgItem(hDlg, IDC_GENERAL_LINKLIST);
    hWndClose=GetDlgItem(hDlg, IDCANCEL);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_VARTHEME_LINK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CLOSE));
    SendMessage(hWndLinkList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);

    //Columns
    {
      LVCOLUMNW lvcW;

      lvcW.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
      lvcW.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_SYNTAXFILE);
      lvcW.cx=120;
      lvcW.iSubItem=LVSI_LINK_SYNTAXFILE;
      ListView_InsertColumnWide(hWndLinkList, LVSI_LINK_SYNTAXFILE, &lvcW);

      lvcW.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_VARTHEMENAME);
      lvcW.cx=120;
      lvcW.iSubItem=LVSI_LINK_VARTHEME;
      ListView_InsertColumnWide(hWndLinkList, LVSI_LINK_VARTHEME, &lvcW);
    }

    //Rows
    {
      LVITEMW lvi;
      int nIndex=0;
      SYNTAXFILE *lpSyntaxFile;

      for (lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpSyntaxFile->next)
      {
        lvi.mask=LVIF_TEXT;
        lvi.pszText=lpSyntaxFile->wszSyntaxFileName;
        lvi.iItem=nIndex;
        lvi.iSubItem=LVSI_LIST_VARIABLE;
        ListView_InsertItemWide(hWndLinkList, &lvi);

        if (lpSyntaxFile->lpVarThemeLink)
        {
          lvi.mask=LVIF_TEXT;
          lvi.pszText=lpSyntaxFile->lpVarThemeLink->wszVarThemeName;
          lvi.iItem=nIndex;
          lvi.iSubItem=LVSI_LIST_VALUE;
          ListView_SetItemWide(hWndLinkList, &lvi);
        }

        ++nIndex;
      }
    }

    //Popup menu
    if (hMenuList=CreatePopupMenu())
    {
      VARTHEME *lpVarTheme;

      nFirstVarThemeIndex=1;
      nLastVarThemeIndex=0;
      AppendMenuWide(hMenuList, MF_STRING, IDC_GENERAL_LINKCLEARITEM, GetLangStringW(wLangModule, STRID_MENU_CLEARITEM));
      AppendMenuWide(hMenuList, MF_SEPARATOR, (UINT)-1, NULL);
      for (lpVarTheme=hVarThemesStack.first; lpVarTheme; lpVarTheme=lpVarTheme->next)
        AppendMenuWide(hMenuList, MF_STRING, ++nLastVarThemeIndex, lpVarTheme->wszVarThemeName);
    }
    bListChanged=FALSE;
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hWndLinkList)
    {
      LVHITTESTINFO lvhti;
      LVITEMW lvi;
      POINT ptScreen={0};
      RECT rcItem;
      SYNTAXFILE *lpSyntaxFile;
      int nVarThemeIndex;
      int nCmd;

      if (lParam == -1)
      {
        if ((lvhti.iItem=(int)SendMessage(hWndLinkList, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED)) != -1)
        {
          rcItem.left=LVIR_LABEL;
          SendMessage(hWndLinkList, LVM_GETITEMRECT, (WPARAM)lvhti.iItem, (LPARAM)&rcItem);
          ptScreen.x=rcItem.left;
          ptScreen.y=rcItem.bottom;
        }
        ClientToScreen(hWndLinkList, &ptScreen);
      }
      else
      {
        GetCursorPos(&ptScreen);
        lvhti.pt=ptScreen;
        ScreenToClient(hWndLinkList, &lvhti.pt);
        SendMessage(hWndLinkList, LVM_SUBITEMHITTEST, 0, (LPARAM)&lvhti);

        lvi.stateMask=LVIS_SELECTED;
        lvi.state=LVIS_SELECTED;
        SendMessage(hWndLinkList, LVM_SETITEMSTATE, (WPARAM)lvhti.iItem, (LPARAM)&lvi);
      }
      if (lvhti.iItem >= 0 && (lpSyntaxFile=StackGetSyntaxFileByIndex(&hSyntaxFilesStack, lvhti.iItem + 1)))
      {
        if (nVarThemeIndex=StackGetVarThemeIndex(&hVarThemesStack, lpSyntaxFile->lpVarThemeLink))
          CheckMenuRadioItem(hMenuList, nFirstVarThemeIndex, nLastVarThemeIndex, nVarThemeIndex, MF_BYCOMMAND);
        else
          CheckMenuRadioItem(hMenuList, nFirstVarThemeIndex, nLastVarThemeIndex, (UINT)-1, MF_BYCOMMAND);
        nCmd=TrackPopupMenu(hMenuList, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, 0, hDlg, NULL);

        if (nCmd > 0)
        {
          if (nCmd == IDC_GENERAL_LINKCLEARITEM)
            lpSyntaxFile->lpVarThemeLink=NULL;
          else
            lpSyntaxFile->lpVarThemeLink=StackGetVarThemeByIndex(&hVarThemesStack, nCmd);

          lvi.mask=LVIF_TEXT;
          lvi.pszText=(wchar_t *)(lpSyntaxFile->lpVarThemeLink?lpSyntaxFile->lpVarThemeLink->wszVarThemeName:L"");
          lvi.iItem=lvhti.iItem;
          lvi.iSubItem=LVSI_LIST_VALUE;
          ListView_SetItemWide(hWndLinkList, &lvi);

          bListChanged=TRUE;
        }
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_GENERAL_LINKLIST)
    {
      if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
      {
        PostMessage(hDlg, WM_CONTEXTMENU, (WPARAM)hWndLinkList, 0);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      if (bListChanged)
      {
        SYNTAXFILE *lpSyntaxFile;
        DWORD dwSize;

        if (wszVarThemeLink)
        {
          GlobalFree((HGLOBAL)wszVarThemeLink);
          wszVarThemeLink=NULL;
        }

        for (dwSize=0, lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpSyntaxFile->next)
        {
          if (lpSyntaxFile->lpVarThemeLink)
            dwSize+=(DWORD)xprintfW(NULL, L"%s>%s|", lpSyntaxFile->wszSyntaxFileName, lpSyntaxFile->lpVarThemeLink->wszVarThemeName) - 1;
        }
        if (dwSize)
        {
          if (wszVarThemeLink=(wchar_t *)GlobalAlloc(GMEM_FIXED, (dwSize + 2) * sizeof(wchar_t)))
          {
            for (dwSize=0, lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpSyntaxFile->next)
            {
              if (lpSyntaxFile->lpVarThemeLink)
                dwSize+=(DWORD)xprintfW(wszVarThemeLink + dwSize, L"%s>%s|", lpSyntaxFile->wszSyntaxFileName, lpSyntaxFile->lpVarThemeLink->wszVarThemeName);
            }
            if (dwSize) --dwSize;
            wszVarThemeLink[dwSize]=L'\0';
          }
        }
      }
      EndDialog(hDlg, bListChanged);
      return TRUE;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy resources
    DestroyMenu(hMenuList);
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcLinkMinMaxDialog, &rcLinkCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm);
  }

  return FALSE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult=0;

  if (uMsg == AKDN_EDIT_ONFINISH ||
      uMsg == AKDN_EDIT_ONCLOSE)
  {
    MANUALSET *lpElement;
    HWND hWndMaster=(HWND)SendMessage((HWND)wParam, AEM_GETMASTER, 0, 0);

    if (!hWndMaster || hWndMaster == (HWND)wParam)
    {
      if (lpElement=StackGetManual(&hManualStack, (HWND)wParam, (AEHDOC)lParam))
        StackDeleteManual(&hManualStack, lpElement, CODER_ALL);
    }
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
  }

  if (ParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  if (uMsg == AKDN_MAIN_ONFINISH)
  {
    if (bInitHighLight)
    {
      UninitMain();
      UninitHighLight();
    }
    if (nInitCodeFold)
    {
      UninitMain();
      UninitCodeFold((DWORD)-1);
    }
    if (bInitAutoComplete)
    {
      UninitMain();
      UninitAutoComplete();
    }
    return FALSE;
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult=0;

  if (ParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  //Call next procedure
  return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewUserParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  MANUALSET *lpManual;
  LRESULT lResult;

  if (lpManual=StackGetManualByUserParent(&hManualStack, hWnd))
  {
    lResult=(LRESULT)lpManual->hWndEdit;

    //ParentMessages
    if (bInitHighLight && (lpManual->dwDllFunction & CODER_HIGHLIGHT))
    {
      if (HighLightParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
        return lResult;
    }
    if (nInitCodeFold && (lpManual->dwDllFunction & CODER_CODEFOLD))
    {
      if (CodeFoldParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
        return lResult;
    }
    if (bInitAutoComplete && (lpManual->dwDllFunction & CODER_AUTOCOMPLETE))
    {
      if (AutoCompleteParentMessages(hWnd, uMsg, wParam, lParam, &lResult))
        return lResult;
    }

    //Call next procedure
    return CallWindowProcWide(lpManual->lpOldParentProc, hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

BOOL CALLBACK ParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (bInitHighLight)
  {
    if (HighLightParentMessages(hWnd, uMsg, wParam, lParam, lResult))
      return TRUE;
  }
  if (nInitCodeFold)
  {
    if (CodeFoldParentMessages(hWnd, uMsg, wParam, lParam, lResult))
      return TRUE;
  }
  if (bInitAutoComplete)
  {
    if (AutoCompleteParentMessages(hWnd, uMsg, wParam, lParam, lResult))
      return TRUE;
  }
  return FALSE;
}

LRESULT CALLBACK NewEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (EditMessages(hWnd, uMsg, wParam, lParam, &lResult))
    return lResult;

  //Call next procedure
  return NewEditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewUserEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  MANUALSET *lpManual;
  LRESULT lResult;

  if (lpManual=StackGetManual(&hManualStack, hWnd, NULL))
  {
    if (uMsg == WM_DESTROY)
    {
      StackDeleteManual(&hManualStack, lpManual, CODER_ALL);
      return 0;
    }

    //EditMessages
    if (bInitHighLight && (lpManual->dwDllFunction & CODER_HIGHLIGHT))
    {
      if (HighLightEditMessages(hWnd, uMsg, wParam, lParam, &lResult))
        return lResult;
    }
    if (nInitCodeFold && (lpManual->dwDllFunction & CODER_CODEFOLD))
    {
      if (CodeFoldEditMessages(hWnd, uMsg, wParam, lParam, &lResult))
        return lResult;
    }
    if (bInitAutoComplete && (lpManual->dwDllFunction & CODER_AUTOCOMPLETE))
    {
      if (AutoCompleteEditMessages(hWnd, uMsg, wParam, lParam, &lResult))
        return lResult;
    }

    //Call next procedure
    return CallWindowProcWide(lpManual->lpOldEditProc, hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

BOOL CALLBACK EditMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  if (bInitHighLight)
  {
    if (HighLightEditMessages(hWnd, uMsg, wParam, lParam, lResult))
      return TRUE;
  }
  if (nInitCodeFold)
  {
    if (CodeFoldEditMessages(hWnd, uMsg, wParam, lParam, lResult))
      return TRUE;
  }
  if (bInitAutoComplete)
  {
    if (AutoCompleteEditMessages(hWnd, uMsg, wParam, lParam, lResult))
      return TRUE;
  }
  return FALSE;
}

SYNTAXFILE* StackLoadSyntaxFile(STACKSYNTAXFILE *hStack, SYNTAXFILE *lpSyntaxFile)
{
  HANDLE hFile;
  DELIMITERINFO *lpDelimElement=NULL;
  WORDINFO *lpWordElement=NULL;
  QUOTEINFO *lpQuoteElement=NULL;
  WILDCARDINFO *lpWildElement=NULL;
  FOLDINFO *lpFoldInfo=NULL;
  SKIPINFO *lpSkipInfo=NULL;
  STACKVAR *lpVarStack;
  wchar_t wszFile[MAX_PATH];
  const wchar_t *wpFileName;
  wchar_t *wszText;
  const wchar_t *wpText;
  const wchar_t *wpTextStart;
  const wchar_t *wpSectionStart;
  const wchar_t *wpLineStart;
  wchar_t *wpWildcard;
  wchar_t *wpDelimiter;
  wchar_t *wpWord;
  wchar_t *wpQuoteStart;
  wchar_t *wpQuoteEnd;
  wchar_t *wpQuoteInclude;
  wchar_t *wpQuoteExclude;
  wchar_t *wpSkipStart;
  wchar_t *wpSkipEnd;
  wchar_t *wpFoldStart;
  wchar_t *wpFoldEnd;
  wchar_t *wpFoldDelimiters;
  wchar_t *wpRuleFile;
  wchar_t wchEscape;
  DWORD dwFileSize;
  DWORD dwBytesRead;
  DWORD dwUnicodeLen;
  int nWildcardLen;
  int nDelimiterLen;
  int nWordLen;
  int nQuoteStartLen;
  int nQuoteEndLen;
  int nQuoteIncludeLen;
  int nQuoteExcludeLen;
  int nSkipStartLen;
  int nSkipEndLen;
  int nFoldStartLen;
  int nFoldEndLen;
  int nDelimitersLen;
  int nRuleLen;
  int nBlockLen;
  int nBlockParsedLen;
  int nLinesInBlock;
  INT_PTR nExpandLen;
  DWORD dwFlags;
  DWORD dwFontStyle;
  DWORD dwColor1;
  DWORD dwColor2;
  int nParentID;
  int nRuleID;
  BOOL bQuoteString;
  BOOL bExactTitle;

  xprintfW(wszFile, L"%s\\%s", wszCoderDir, lpSyntaxFile->wszSyntaxFileName);
  if ((hFile=CreateFileWide(wszFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL)) == INVALID_HANDLE_VALUE)
    return NULL;

  if ((dwFileSize=GetFileSize(hFile, NULL)) != INVALID_FILE_SIZE)
  {
    xmemset(&lpSyntaxFile->ftTimeStamp, 0, sizeof(FILETIME));
    GetFileTime(hFile, NULL, NULL, &lpSyntaxFile->ftTimeStamp);

    if (dwFileSize >= 2)
    {
      if (wszText=(wchar_t *)GlobalAlloc(GPTR, dwFileSize + 2))
      {
        if (ReadFile(hFile, wszText, dwFileSize, &dwBytesRead, NULL))
        {
          wpText=(const wchar_t *)wszText;
          dwUnicodeLen=dwBytesRead / sizeof(wchar_t);
          wszText[dwUnicodeLen++]=L'\0';
          if (wszText[0] == 0xFEFF)
          {
            ++wpText;
            --dwUnicodeLen;
          }
          wpTextStart=wpText;

          lpLoadSyntaxFile=lpSyntaxFile;
          bSyntaxFileLoadError=FALSE;

          //Set variable theme associated with syntax file
          wpFileName=GetFileName(wszFile, -1);
          if (!lpSyntaxFile->lpVarThemeLink)
            lpVarStack=&lpVarThemeActive->hVarStack;
          else
            lpVarStack=&lpSyntaxFile->lpVarThemeLink->hVarStack;

          if (lpSyntaxFile)
          {
            SectionStart:
            if (!SkipComment(&wpText)) goto FreeText;
            if (*wpText == L'\r' || *wpText == L'\n')
            {
              do
              {
                ++wpText;
              }
              while (*wpText == L'\r' || *wpText == L'\n');

              goto SectionStart;
            }
            wpSectionStart=wpText;
            GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack);
            if (!NextLine(&wpText)) goto FreeText;
            if (!SkipComment(&wpText)) goto FreeText;
            if (bSyntaxFileLoadError) goto FreeText;

            if (!xstrcmpiW(wszBuffer, L"Files:"))
            {
              for (;;)
              {
                //Parse line
                lpWildElement=NULL;
                wpWildcard=NULL;

                for (;;)
                {
                  //Wildcard
                  if ((nWildcardLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                  {
                    if (!bQuoteString && wszBuffer[0] == L';') break;
                    if (lpSyntaxFile->bReserveCacheWildcard) break;

                    if (wpWildcard=(wchar_t *)GlobalAlloc(GPTR, (nWildcardLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpWildcard, wszBuffer, (nWildcardLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else goto SectionStart;

                  //Add to stack
                  if (lpWildElement=StackInsertWildcard(&lpSyntaxFile->hWildcardStack, nWildcardLen))
                  {
                    lpWildElement->wpWildcard=wpWildcard;
                    lpWildElement->nWildcardLen=nWildcardLen;
                  }
                  break;
                }
                if (!lpWildElement && wpWildcard) GlobalFree((HGLOBAL)wpWildcard);
                if (!NextLine(&wpText)) goto FreeText;
                if (bSyntaxFileLoadError) goto FreeText;
              }
            }

            //HighLight
            else if (!xstrcmpiW(wszBuffer, L"Delimiters:"))
            {
              for (;;)
              {
                //Parse line
                lpDelimElement=NULL;
                wpDelimiter=NULL;
                dwFlags=AEHLF_MATCHCASE;
                dwFontStyle=0;
                dwColor1=(DWORD)-1;
                dwColor2=(DWORD)-1;
                nParentID=0;

                for (;;)
                {
                  //Highlight flags
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFlags=(DWORD)xatoiW(wszBuffer, NULL);
                    if (dwFlags == 0 && wszBuffer[0] != L'0') break;
                  }
                  else goto SectionStart;

                  //Font style
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                  }
                  else break;

                  //Color
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (*wszBuffer == L'#')
                    {
                      dwColor1=GetColorFromStr(wszBuffer + 1, NULL);
                    }
                    else if (*wszBuffer != L'0') break;
                  }
                  else break;

                  //Background color
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (*wszBuffer == L'#')
                    {
                      dwColor2=GetColorFromStr(wszBuffer + 1, NULL);
                    }
                    else if (*wszBuffer != L'0') break;
                  }
                  else break;

                  //Delimiter
                  if (nDelimiterLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (wpDelimiter=(wchar_t *)GlobalAlloc(GPTR, (nDelimiterLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpDelimiter, wszBuffer, (nDelimiterLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Parent ID
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    nParentID=(int)xatoiW(wszBuffer, NULL);
                  }

                  //Add to stack
                  if (lpDelimElement=StackInsertDelimiter(&lpSyntaxFile->hDelimiterStack, nDelimiterLen))
                  {
                    lpDelimElement->wpDelimiter=wpDelimiter;
                    lpDelimElement->nDelimiterLen=nDelimiterLen;
                    lpDelimElement->dwFlags=dwFlags;
                    lpDelimElement->dwFontStyle=dwFontStyle;
                    lpDelimElement->dwColor1=dwColor1;
                    lpDelimElement->dwColor2=dwColor2;
                    lpDelimElement->nParentID=nParentID;
                  }
                  break;
                }
                if (!lpDelimElement && wpDelimiter) GlobalFree((HGLOBAL)wpDelimiter);
                if (!NextLine(&wpText)) goto FreeText;
                if (bSyntaxFileLoadError) goto FreeText;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Font:"))
            {
              //Parse line
              for (;;)
              {
                HDC hDC;
                int nFontPointSize=0;

                lpSyntaxFile->dwFontFlags=0;
                lpSyntaxFile->lfFont.lfHeight=0;
                lpSyntaxFile->lfFont.lfWidth=0;
                lpSyntaxFile->lfFont.lfEscapement=0;
                lpSyntaxFile->lfFont.lfOrientation=0;
                lpSyntaxFile->lfFont.lfWeight=0;
                lpSyntaxFile->lfFont.lfItalic=FALSE;
                lpSyntaxFile->lfFont.lfUnderline=FALSE;
                lpSyntaxFile->lfFont.lfStrikeOut=FALSE;
                lpSyntaxFile->lfFont.lfCharSet=DEFAULT_CHARSET;
                lpSyntaxFile->lfFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
                lpSyntaxFile->lfFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
                lpSyntaxFile->lfFont.lfQuality=DEFAULT_QUALITY;
                lpSyntaxFile->lfFont.lfPitchAndFamily=DEFAULT_PITCH;
                lpSyntaxFile->lfFont.lfFaceName[0]=L'\0';

                //Font style
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  dwFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                  if (dwFontStyle)
                  {
                    lpSyntaxFile->dwFontFlags|=FF_FONTSTYLE;
                    lpSyntaxFile->lfFont.lfWeight=(dwFontStyle == AEHLS_FONTBOLD || dwFontStyle == AEHLS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
                    lpSyntaxFile->lfFont.lfItalic=(dwFontStyle == AEHLS_FONTITALIC || dwFontStyle == AEHLS_FONTBOLDITALIC)?TRUE:FALSE;
                  }
                  else if (wszBuffer[0] != L'0') break;
                }

                //Font size
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  nFontPointSize=(int)xatoiW(wszBuffer, NULL);
                  if (nFontPointSize)
                  {
                    if (hDC=GetDC(hMainWnd))
                    {
                      lpSyntaxFile->dwFontFlags|=FF_FONTSIZE;
                      lpSyntaxFile->lfFont.lfHeight=-MulDiv(nFontPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
                      ReleaseDC(hMainWnd, hDC);
                    }
                  }
                  else if (wszBuffer[0] != L'0') break;
                }

                //Face name
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->dwFontFlags|=FF_FONTFACE;
                  xstrcpynW(lpSyntaxFile->lfFont.lfFaceName, wszBuffer, LF_FACESIZE);
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Colors:"))
            {
              //Parse line
              for (;;)
              {
                lpSyntaxFile->aecColors.dwFlags=0;

                //Basic text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crBasicText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_BASICTEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Basic bkgrnd color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crBasicBk=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_BASICBK;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Sel text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crSelText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_SELTEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Sel bkgrnd color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crSelBk=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_SELBK;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Line text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crActiveLineText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ACTIVELINETEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Line bkgrnd color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crActiveLineBk=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ACTIVELINEBK;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Line border color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crActiveLineBorder=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ACTIVELINEBORDER;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Alt line text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crAltLineText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ALTLINETEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Alt line bkgrnd color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crAltLineBk=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ALTLINEBK;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Alt border color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crAltLineBorder=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ALTLINEBORDER;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Column color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crActiveColumn=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_ACTIVECOLUMN;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Marker color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crColumnMarker=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_COLUMNMARKER;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Caret color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crCaret=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_CARET;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //URL color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crUrlText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_URLTEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Active URL color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crUrlCursorText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_URLCURSORTEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Visit URL color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->aecColors.crUrlVisitText=GetColorFromStr(wszBuffer + 1, NULL);
                    lpSyntaxFile->aecColors.dwFlags|=AECLR_URLVISITTEXT;
                  }
                  else if (*wszBuffer != L'0') break;
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"BkImage:"))
            {
              //Parse line
              for (;;)
              {
                //BkImage file
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  xstrcpynW(lpSyntaxFile->wszBkImageFile, wszBuffer, BUFFER_SIZE);
                }

                //BkImage alpha transparency
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->nBkImageAlpha=(DWORD)xatoiW(wszBuffer, NULL);
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"AutoMark:"))
            {
              //Parse line
              for (;;)
              {
                lpSyntaxFile->dwAutoMarkFlags=AEHLF_MATCHCASE;
                lpSyntaxFile->dwAutoMarkFontStyle=AEHLS_NONE;
                lpSyntaxFile->dwAutoMarkTextColor=(DWORD)-1;
                lpSyntaxFile->dwAutoMarkBkColor=(DWORD)-1;

                //AutoMark flags
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->dwAutoMarkFlags=(DWORD)xatoiW(wszBuffer, NULL);
                  if (lpSyntaxFile->dwAutoMarkFlags == 0 && wszBuffer[0] != L'0') break;
                }

                //AutoMark font style
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->dwAutoMarkFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                }

                //AutoMark text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwAutoMarkTextColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //AutoMark background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwAutoMarkBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Quotes:") ||
                     !xstrcmpiW(wszBuffer, L"QuotesRE:"))
            {
              for (;;)
              {
                //Parse line
                lpQuoteElement=NULL;
                wpQuoteStart=NULL;
                wpQuoteEnd=NULL;
                wpQuoteInclude=NULL;
                nQuoteIncludeLen=0;
                wpQuoteExclude=NULL;
                nQuoteExcludeLen=0;
                wchEscape=0;
                dwFlags=AEHLF_MATCHCASE;
                dwFontStyle=0;
                dwColor1=(DWORD)-1;
                dwColor2=(DWORD)-1;
                nParentID=0;
                nRuleID=0;

                for (;;)
                {
                  //Highlight flags
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFlags=(DWORD)xatoiW(wszBuffer, NULL);
                    if (dwFlags == 0 && wszBuffer[0] != L'0') break;
                  }
                  else goto SectionStart;

                  //Regular expression pattern, if quoted or font style, if number.
                  if (!(nQuoteStartLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)))
                    break;

                  if (bQuoteString)
                  {
                    dwFlags|=AEHLF_REGEXP;

                    //Pattern
                    if (wpQuoteStart=(wchar_t *)GlobalAlloc(GPTR, (nQuoteStartLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpQuoteStart, wszBuffer, (nQuoteStartLen + 1) * sizeof(wchar_t));
                    else
                      break;

                    //Match map
                    if ((nQuoteEndLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                    {
                      if (nExpandLen=ExpandVars(wszBuffer, nQuoteEndLen, NULL, 0, lpVarStack))
                        if (wpQuoteEnd=(wchar_t *)GlobalAlloc(GPTR, nExpandLen * sizeof(wchar_t)))
                          nQuoteEndLen=(int)ExpandVars(wszBuffer, nQuoteEndLen, wpQuoteEnd, nExpandLen, lpVarStack);
                    }
                    else break;
                  }
                  else
                  {
                    //Font style
                    dwFontStyle=(DWORD)xatoiW(wszBuffer, NULL);

                    //Color
                    if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                    {
                      if (*wszBuffer == L'#')
                      {
                        dwColor1=GetColorFromStr(wszBuffer + 1, NULL);
                      }
                      else if (*wszBuffer != L'0') break;
                    }
                    else break;

                    //Background color
                    if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                    {
                      if (*wszBuffer == L'#')
                      {
                        dwColor2=GetColorFromStr(wszBuffer + 1, NULL);
                      }
                      else if (*wszBuffer != L'0') break;
                    }
                    else break;

                    //Quote start
                    if ((nQuoteStartLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                    {
                      if (wpQuoteStart=(wchar_t *)GlobalAlloc(GPTR, (nQuoteStartLen + 1) * sizeof(wchar_t)))
                        xmemcpy(wpQuoteStart, wszBuffer, (nQuoteStartLen + 1) * sizeof(wchar_t));
                      else
                        break;
                    }
                    else break;

                    //Quote end
                    if ((nQuoteEndLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                    {
                      if (wpQuoteEnd=(wchar_t *)GlobalAlloc(GPTR, (nQuoteEndLen + 1) * sizeof(wchar_t)))
                        xmemcpy(wpQuoteEnd, wszBuffer, (nQuoteEndLen + 1) * sizeof(wchar_t));
                      else
                        break;
                    }
                    else break;

                    //Escape
                    if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack) || bQuoteString)
                      wchEscape=wszBuffer[0];
                    else
                      break;

                    //Quote include
                    if ((nQuoteIncludeLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                    {
                      if (wpQuoteInclude=(wchar_t *)GlobalAlloc(GPTR, (nQuoteIncludeLen + 1) * sizeof(wchar_t)))
                        xmemcpy(wpQuoteInclude, wszBuffer, (nQuoteIncludeLen + 1) * sizeof(wchar_t));
                      else
                        break;
                    }
                    else break;

                    //Quote exclude
                    if ((nQuoteExcludeLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                    {
                      if (wpQuoteExclude=(wchar_t *)GlobalAlloc(GPTR, (nQuoteExcludeLen + 1) * sizeof(wchar_t)))
                        xmemcpy(wpQuoteExclude, wszBuffer, (nQuoteExcludeLen + 1) * sizeof(wchar_t));
                      else
                        break;
                    }
                    else break;
                  }

                  //Parent ID
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    nParentID=(int)xatoiW(wszBuffer, NULL);

                    //Rule ID
                    if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                    {
                      nRuleID=(int)xatoiW(wszBuffer, NULL);
                    }
                  }

                  //Add to stack
                  if (lpQuoteElement=StackInsertQuote(&lpSyntaxFile->hQuoteStack, nQuoteStartLen))
                  {
                    lpQuoteElement->wpQuoteStart=wpQuoteStart;
                    lpQuoteElement->nQuoteStartLen=nQuoteStartLen;
                    lpQuoteElement->wpQuoteEnd=wpQuoteEnd;
                    lpQuoteElement->nQuoteEndLen=nQuoteEndLen;
                    lpQuoteElement->wchEscape=wchEscape;
                    lpQuoteElement->wpQuoteInclude=wpQuoteInclude;
                    lpQuoteElement->nQuoteIncludeLen=nQuoteIncludeLen;
                    lpQuoteElement->wpQuoteExclude=wpQuoteExclude;
                    lpQuoteElement->nQuoteExcludeLen=nQuoteExcludeLen;
                    lpQuoteElement->dwFlags=dwFlags;
                    lpQuoteElement->dwFontStyle=dwFontStyle;
                    lpQuoteElement->dwColor1=dwColor1;
                    lpQuoteElement->dwColor2=dwColor2;
                    lpQuoteElement->nParentID=nParentID;
                    lpQuoteElement->nRuleID=nRuleID;
                  }
                  break;
                }
                if (!lpQuoteElement)
                {
                  if (wpQuoteStart) GlobalFree((HGLOBAL)wpQuoteStart);
                  if (wpQuoteEnd) GlobalFree((HGLOBAL)wpQuoteEnd);
                  if (wpQuoteInclude) GlobalFree((HGLOBAL)wpQuoteInclude);
                  if (wpQuoteExclude) GlobalFree((HGLOBAL)wpQuoteExclude);
                }
                if (!NextLine(&wpText)) goto FreeText;
                if (bSyntaxFileLoadError) goto FreeText;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Words:"))
            {
              for (;;)
              {
                //Parse line
                lpWordElement=NULL;
                wpWord=NULL;
                dwFlags=AEHLF_MATCHCASE;
                dwFontStyle=0;
                dwColor1=(DWORD)-1;
                dwColor2=(DWORD)-1;
                nParentID=0;

                for (;;)
                {
                  //Highlight flags
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFlags=(DWORD)xatoiW(wszBuffer, NULL);
                    if (dwFlags == 0 && wszBuffer[0] != L'0') break;
                  }
                  else goto SectionStart;

                  //Font style
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                  }
                  else break;

                  //Color
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (*wszBuffer == L'#')
                    {
                      dwColor1=GetColorFromStr(wszBuffer + 1, NULL);
                    }
                    else if (*wszBuffer != L'0') break;
                  }
                  else break;

                  //Background color
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (*wszBuffer == L'#')
                    {
                      dwColor2=GetColorFromStr(wszBuffer + 1, NULL);
                    }
                    else if (*wszBuffer != L'0') break;
                  }
                  else break;

                  //Word
                  if (nWordLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (wpWord=(wchar_t *)GlobalAlloc(GPTR, (nWordLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpWord, wszBuffer, (nWordLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Parent ID
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    nParentID=(int)xatoiW(wszBuffer, NULL);
                  }

                  //Add to stack
                  if (lpWordElement=StackInsertWord(&lpSyntaxFile->hWordStack, &lpSyntaxFile->hWordAlphaStack, &lpSyntaxFile->hWordOrderStack, wpWord, nWordLen))
                  {
                    lpWordElement->wpWord=wpWord;
                    lpWordElement->nWordLen=nWordLen;
                    lpWordElement->dwFlags=dwFlags;
                    lpWordElement->dwFontStyle=dwFontStyle;
                    lpWordElement->dwColor1=dwColor1;
                    lpWordElement->dwColor2=dwColor2;
                    lpWordElement->nParentID=nParentID;
                  }
                  break;
                }
                if (!lpWordElement && wpWord) GlobalFree((HGLOBAL)wpWord);
                if (!NextLine(&wpText)) goto FreeText;
                if (bSyntaxFileLoadError) goto FreeText;
              }
            }

            //CodeFold
            else if (!xstrcmpiW(wszBuffer, L"FoldPanel:"))
            {
              //Parse line
              for (;;)
              {
                lpSyntaxFile->dwFoldPanelFirstBkColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelSecondBkColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelNormalFoldColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelActiveFoldColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelNormalNodeOpenBkColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelNormalNodeCloseBkColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelActiveNodeOpenBkColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelActiveNodeCloseBkColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelNormalNodeOpenSignColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelNormalNodeCloseSignColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelActiveNodeOpenSignColor=(DWORD)-1;
                lpSyntaxFile->dwFoldPanelActiveNodeCloseSignColor=(DWORD)-1;

                //First background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelFirstBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Second background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelSecondBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Normal fold color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelNormalFoldColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Active fold color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelActiveFoldColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Normal node open background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelNormalNodeOpenBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Normal node close background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelNormalNodeCloseBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Active node open background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelActiveNodeOpenBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Active node close background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelActiveNodeCloseBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Normal node open sign color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelNormalNodeOpenSignColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Normal node close sign color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelNormalNodeCloseSignColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Active node open sign color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelActiveNodeOpenSignColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //Active node close sign color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldPanelActiveNodeCloseSignColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"FoldList:"))
            {
              //Parse line
              for (;;)
              {
                lpSyntaxFile->dwFoldListTextColor=(DWORD)-1;
                lpSyntaxFile->dwFoldListBkColor=(DWORD)-1;

                //List text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldListTextColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //List background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwFoldListBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"TagMark:"))
            {
              //Parse line
              for (;;)
              {
                lpSyntaxFile->dwTagMarkFlags=0;
                lpSyntaxFile->dwTagMarkFontStyle=AEHLS_NONE;
                lpSyntaxFile->dwTagMarkTextColor=(DWORD)-1;
                lpSyntaxFile->dwTagMarkBkColor=(DWORD)-1;

                //TagMark flags
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->dwTagMarkFlags=(DWORD)xatoiW(wszBuffer, NULL);
                  if (lpSyntaxFile->dwTagMarkFlags == 0 && wszBuffer[0] != L'0') break;
                }

                //TagMark font style
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->dwTagMarkFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                }

                //TagMark text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwTagMarkTextColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //TagMark background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwTagMarkBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Skips:"))
            {
              for (;;)
              {
                //Parse line
                lpSkipInfo=NULL;
                wpSkipStart=NULL;
                wpSkipEnd=NULL;
                dwFlags=FIF_MATCHCASE;

                for (;;)
                {
                  //Skip flags
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFlags=(DWORD)xatoiW(wszBuffer, NULL);
                    if (dwFlags == 0 && wszBuffer[0] != L'0') break;
                  }
                  else goto SectionStart;

                  //Skip start
                  if ((nSkipStartLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                  {
                    if (wpSkipStart=(wchar_t *)GlobalAlloc(GPTR, (nSkipStartLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpSkipStart, wszBuffer, (nSkipStartLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Skip end
                  if ((nSkipEndLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                  {
                    if (wpSkipEnd=(wchar_t *)GlobalAlloc(GPTR, (nSkipEndLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpSkipEnd, wszBuffer, (nSkipEndLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Escape
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack) || bQuoteString)
                    wchEscape=wszBuffer[0];
                  else
                    break;

                  //Add to stack
                  if (lpSkipInfo=StackInsertSkipInfo(&lpSyntaxFile->hSkipStack))
                  {
                    lpSkipInfo->dwFlags=dwFlags;
                    if (!(lpSkipInfo->lpSkipStart=StackInsertSkipStart(&lpSyntaxFile->hSkipStartStack, lpSkipInfo, wpSkipStart, nSkipStartLen)))
                      goto FreeSkip;

                    if (dwFlags & FIF_REGEXPEND)
                    {
                      lpSkipInfo->sregEnd.dwOptions=REO_MULTILINE|(dwFlags & FIF_MATCHCASE?REO_MATCHCASE:0);
                      lpSkipInfo->sregEnd.ref100=&lpSkipInfo->lpSkipStart->sregStart;
                      if (nSkipEndLen && !PatCompile(&lpSkipInfo->sregEnd, wpSkipEnd, wpSkipEnd + nSkipEndLen))
                      {
                        lpSkipInfo->sregEnd.first->dwFlags&=~REGF_ROOTANY;
                        nSkipEndLen=(int)lpSkipInfo->sregEnd.first->nGroupLen;
                      }
                      else
                      {
                        xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_COMPILEERROR), lpSyntaxFile->wszSyntaxFileName, nSkipEndLen, wpSkipEnd);
                        MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
                        goto FreeSkip;
                      }
                    }
                    lpSkipInfo->wpSkipEnd=wpSkipEnd;
                    lpSkipInfo->nSkipEndLen=nSkipEndLen;
                    lpSkipInfo->wchEscape=wchEscape;
                    break;

                    FreeSkip:
                    StackDelete((stack **)&lpSyntaxFile->hSkipStack.first, (stack **)&lpSyntaxFile->hSkipStack.last, (stack *)lpSkipInfo);
                    lpSkipInfo=NULL;
                  }
                  break;
                }
                if (!lpSkipInfo)
                {
                  if (wpSkipStart) GlobalFree((HGLOBAL)wpSkipStart);
                  if (wpSkipEnd) GlobalFree((HGLOBAL)wpSkipEnd);
                }
                if (!NextLine(&wpText)) goto FreeText;
                if (bSyntaxFileLoadError) goto FreeText;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Folds:"))
            {
              for (;;)
              {
                //Parse line
                lpFoldInfo=NULL;
                wpFoldStart=NULL;
                wpFoldEnd=NULL;
                wpFoldDelimiters=NULL;
                wpRuleFile=NULL;
                dwFlags=FIF_MATCHCASE;
                dwFontStyle=0;
                dwColor1=(DWORD)-1;
                dwColor2=(DWORD)-1;
                nParentID=0;
                nRuleID=0;
                wpLineStart=wpText;

                for (;;)
                {
                  //Fold flags
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFlags=(DWORD)xatoiW(wszBuffer, NULL);
                    if (dwFlags == 0 && wszBuffer[0] != L'0') break;
                  }
                  else goto SectionStart;

                  //Font style
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    dwFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                  }
                  else break;

                  //Color
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (*wszBuffer == L'#')
                    {
                      dwColor1=GetColorFromStr(wszBuffer + 1, NULL);
                    }
                    else if (*wszBuffer != L'0') break;
                  }
                  else break;

                  //Background color
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    if (*wszBuffer == L'#')
                    {
                      dwColor2=GetColorFromStr(wszBuffer + 1, NULL);
                    }
                    else if (*wszBuffer != L'0') break;
                  }
                  else break;

                  //Fold start
                  if ((nFoldStartLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                  {
                    if (wpFoldStart=(wchar_t *)GlobalAlloc(GPTR, (nFoldStartLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpFoldStart, wszBuffer, (nFoldStartLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Fold end
                  if ((nFoldEndLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                  {
                    if (wpFoldEnd=(wchar_t *)GlobalAlloc(GPTR, (nFoldEndLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpFoldEnd, wszBuffer, (nFoldEndLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Delimiters
                  if ((nDelimitersLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, &bQuoteString, lpVarStack)) || bQuoteString)
                  {
                    if (wpFoldDelimiters=(wchar_t *)GlobalAlloc(GPTR, (nDelimitersLen + 1) * sizeof(wchar_t)))
                      xmemcpy(wpFoldDelimiters, wszBuffer, (nDelimitersLen + 1) * sizeof(wchar_t));
                    else
                      break;
                  }
                  else break;

                  //Parent ID
                  if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                  {
                    nParentID=(int)xatoiW(wszBuffer, NULL);

                    //Rule ID
                    if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                    {
                      nRuleID=(int)xatoiW(wszBuffer, NULL);

                      //Rule file
                      if ((nRuleLen=GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack)))
                      {
                        if (wpRuleFile=(wchar_t *)GlobalAlloc(GPTR, (nRuleLen + 1) * sizeof(wchar_t)))
                          xmemcpy(wpRuleFile, wszBuffer, (nRuleLen + 1) * sizeof(wchar_t));
                        else
                          break;
                      }
                    }
                  }

                  //Add to stack
                  if (lpFoldInfo=StackInsertFoldInfo(&lpSyntaxFile->hFoldStack))
                  {
                    if (dwFlags & FIF_XMLTAG)
                    {
                      if (*wpFoldStart == L'<')
                      {
                        if (nFoldStartLen == 1)
                        {
                          if (!xstrcmpW(wpFoldEnd, L"</"))
                           dwFlags|=FIF_XMLNONAME_TWOTAG;
                          else if (!xstrcmpW(wpFoldEnd, L"/>"))
                            dwFlags|=FIF_XMLNONAME_ONETAG;
                        }
                        else if (nFoldEndLen > 0)
                        {
                          if (!xstrcmpnW(wpFoldEnd, L"</", 2))
                            dwFlags|=FIF_XMLNAMED_TWOTAG;
                          else if (wpFoldEnd[nFoldEndLen - 1] == L'>')
                            dwFlags|=FIF_XMLNAMED_ONETAG;
                        }
                      }
                      else if (nParentID)
                      {
                        dwFlags|=FIF_XMLCHILD;
                      }
                    }

                    lpFoldInfo->dwFlags=dwFlags;
                    lpFoldInfo->wpFoldEnd=wpFoldEnd;
                    lpFoldInfo->nFoldEndLen=nFoldEndLen;
                    lpFoldInfo->wpDelimiters=wpFoldDelimiters;
                    lpFoldInfo->dwFontStyle=dwFontStyle;
                    lpFoldInfo->dwColor1=dwColor1;
                    lpFoldInfo->dwColor2=dwColor2;
                    lpFoldInfo->nParentID=nParentID;
                    lpFoldInfo->nRuleID=nRuleID;
                    lpFoldInfo->wpRuleFile=wpRuleFile;
                    lpFoldInfo->crSyntaxFileLine.cpMin=wpLineStart - wpTextStart;
                    lpFoldInfo->crSyntaxFileLine.cpMax=wpText - wpTextStart;
                    if (!(lpFoldInfo->lpFoldStart=StackInsertFoldStart(&lpSyntaxFile->hFoldStartStack, lpFoldInfo, wpFoldStart, nFoldStartLen)))
                      goto FreeFold;

                    if (dwFlags & FIF_REGEXPEND)
                    {
                      lpFoldInfo->sregEnd.dwOptions=REO_MULTILINE|(dwFlags & FIF_MATCHCASE?REO_MATCHCASE:0);
                      lpFoldInfo->sregEnd.ref100=&lpFoldInfo->lpFoldStart->sregStart;
                      if (nFoldEndLen && !PatCompile(&lpFoldInfo->sregEnd, wpFoldEnd, wpFoldEnd + nFoldEndLen))
                      {
                        lpFoldInfo->sregEnd.first->dwFlags&=~REGF_ROOTANY;
                        lpFoldInfo->nFoldEndPointLen=(int)lpFoldInfo->sregEnd.first->nGroupLen;
                      }
                      else
                      {
                        xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_COMPILEERROR), lpSyntaxFile->wszSyntaxFileName, nFoldEndLen, wpFoldEnd);
                        MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
                        goto FreeFold;
                      }
                    }
                    else lpFoldInfo->nFoldEndPointLen=nFoldEndLen;

                    if (dwFlags & FIF_REGEXPSTART)
                      lpSyntaxFile->hFoldStack.nCommonFirstChar=-1;
                    else if (lpSyntaxFile->hFoldStack.nCommonFirstChar != -1)
                    {
                      if (!lpSyntaxFile->hFoldStack.nCommonFirstChar)
                        lpSyntaxFile->hFoldStack.nCommonFirstChar=*wpFoldStart;
                      else if ((wchar_t)lpSyntaxFile->hFoldStack.nCommonFirstChar != *lpFoldInfo->lpFoldStart->wpFoldStart)
                        lpSyntaxFile->hFoldStack.nCommonFirstChar=-1;
                    }
                    if (!(dwFlags & FIF_NOLISTFOLD))
                      lpSyntaxFile->hFoldStack.bVisible=TRUE;
                    if (lpFoldInfo->wpRuleFile)
                      ++lpSyntaxFile->hFoldStack.nFoldWithThemeCount;
                    break;

                    FreeFold:
                    StackDeleteFoldInfo(&lpSyntaxFile->hFoldStack, &lpSyntaxFile->hFoldStartStack, lpFoldInfo);
                    lpFoldInfo=NULL;
                    wpFoldStart=NULL;
                    wpFoldEnd=NULL;
                    wpFoldDelimiters=NULL;
                  }
                  break;
                }
                if (!lpFoldInfo)
                {
                  if (wpFoldStart) GlobalFree((HGLOBAL)wpFoldStart);
                  if (wpFoldEnd) GlobalFree((HGLOBAL)wpFoldEnd);
                  if (wpFoldDelimiters) GlobalFree((HGLOBAL)wpFoldDelimiters);
                }
                if (!NextLine(&wpText)) goto FreeText;
                if (bSyntaxFileLoadError) goto FreeText;
              }
            }

            //AutoComplete
            else if (!xstrcmpiW(wszBuffer, L"CompleteList:"))
            {
              //Parse line
              for (;;)
              {
                lpSyntaxFile->dwCompleteListFontStyle=0;
                lpSyntaxFile->nCompleteListFontSize=0;
                lpSyntaxFile->wszCompleteListFaceName[0]=L'\0';
                lpSyntaxFile->nCompleteListLineGap=0;
                lpSyntaxFile->wszCompleteListBlockIcon[0]=L'\0';
                lpSyntaxFile->dwCompleteListBlockIconMargins=0;
                lpSyntaxFile->wszCompleteListHlBaseIcon[0]=L'\0';
                lpSyntaxFile->dwCompleteListHlBaseIconMargins=0;
                lpSyntaxFile->wszCompleteListDocWordIcon[0]=L'\0';
                lpSyntaxFile->dwCompleteListDocWordIconMargins=0;
                lpSyntaxFile->dwCompleteListIcons=0;
                lpSyntaxFile->dwCompleteListBasicTextColor=(DWORD)-1;
                lpSyntaxFile->dwCompleteListBasicBkColor=(DWORD)-1;
                lpSyntaxFile->dwCompleteListSelTextColor=(DWORD)-1;
                lpSyntaxFile->dwCompleteListSelBkColor=(DWORD)-1;

                //Font style
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->dwCompleteListFontStyle=(DWORD)xatoiW(wszBuffer, NULL);
                }

                //Font size
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->nCompleteListFontSize=(int)xatoiW(wszBuffer, NULL);
                }

                //Face name
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  xstrcpynW(lpSyntaxFile->wszCompleteListFaceName, wszBuffer, LF_FACESIZE);
                }

                //Line gap
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  lpSyntaxFile->nCompleteListLineGap=(int)xatoiW(wszBuffer, NULL);
                }

                //Block icon
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  xstrcpynW(lpSyntaxFile->wszCompleteListBlockIcon, wszBuffer, MAX_PATH);
                  if (*lpSyntaxFile->wszCompleteListBlockIcon)
                    lpSyntaxFile->dwCompleteListIcons|=BIT_BLOCK;
                }

                //Block icon margins
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  StrToArray(wszBuffer, &lpSyntaxFile->dwCompleteListBlockIconMargins, sizeof(DWORD), sizeof(WORD), NULL);
                }

                //HighLight base icon
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  xstrcpynW(lpSyntaxFile->wszCompleteListHlBaseIcon, wszBuffer, MAX_PATH);
                  if (*lpSyntaxFile->wszCompleteListHlBaseIcon)
                    lpSyntaxFile->dwCompleteListIcons|=BIT_HLBASE;
                }

                //HighLight base icon margins
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  StrToArray(wszBuffer, &lpSyntaxFile->dwCompleteListHlBaseIconMargins, sizeof(DWORD), sizeof(WORD), NULL);
                }

                //Document word icon
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  xstrcpynW(lpSyntaxFile->wszCompleteListDocWordIcon, wszBuffer, MAX_PATH);
                  if (*lpSyntaxFile->wszCompleteListDocWordIcon)
                    lpSyntaxFile->dwCompleteListIcons|=BIT_DOCWORD;
                }

                //Document word icon margins
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  StrToArray(wszBuffer, &lpSyntaxFile->dwCompleteListDocWordIconMargins, sizeof(DWORD), sizeof(WORD), NULL);
                }

                //List basic text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwCompleteListBasicTextColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //List basic background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwCompleteListBasicBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //List sel text color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwCompleteListSelTextColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                //List sel background color
                if (GetWord(wpText, wszBuffer, BUFFER_SIZE, &wpText, NULL, lpVarStack))
                {
                  if (*wszBuffer == L'#')
                  {
                    lpSyntaxFile->dwCompleteListSelBkColor=GetColorFromStr(wszBuffer + 1, NULL);
                  }
                  else if (*wszBuffer != L'0') break;
                }

                break;
              }
            }
            else if (!xstrcmpiW(wszBuffer, L"Blocks:"))
            {
              BLOCKINFO *lpBlockMaster;
              BLOCKINFO *lpBlockInfo;
              BLOCKINFOHANDLE *lpBlockInfoHandle;
              TITLEINFO *lpTitleInfo;
              STACKREGROUP *sregTitle;
              const wchar_t *wpTitleBegin;
              wchar_t *wszTitle;
              const wchar_t *wpBlockBegin;
              const wchar_t *wpBlockEnd;
              wchar_t *wszBlockParsed;
              STACKTITLE hTitleStack;
              HSTACK hHotSpotStack;
              DWORD dwTitleFlags;
              int nTitleLen;

              while (*wpText)
              {
                lpBlockMaster=NULL;
                lpBlockInfo=NULL;
                lpTitleInfo=NULL;
                sregTitle=NULL;
                wpTitleBegin=NULL;
                wszTitle=NULL;
                wszBlockParsed=NULL;
                hTitleStack.first=0;
                hTitleStack.last=0;
                hHotSpotStack.first=0;
                hHotSpotStack.last=0;
                dwTitleFlags=0;
                bExactTitle=-1;

                //Title
                while (*wpText)
                {
                  if (*wpText == L'$')
                  {
                    if (*++wpText == L';' || *wpText == L'#')
                    {
                      //Skip comment
                      if (!NextLine(&wpText))
                        break;
                      continue;
                    }
                    if (*wpText == L'(')
                    {
                      dwTitleFlags=(DWORD)xatoiW(wpText + 1, &wpText);
                      if (*wpText == L')')
                        ++wpText;
                      else
                        break;
                    }
                    if (*wpText == L'~' || *wpText == L'=')
                    {
                      if (*wpText == L'~')
                        bExactTitle=FALSE;
                      else
                        bExactTitle=TRUE;
                      wpTitleBegin=++wpText;
                      while (*wpText != L' ' && *wpText != L'\t' && *wpText != L'\r' && *wpText != L'\0') ++wpText;
                      nTitleLen=(int)(wpText - wpTitleBegin);

                      if (nTitleLen)
                      {
                        if (wszTitle=(wchar_t *)GlobalAlloc(GPTR, (nTitleLen + 1) * sizeof(wchar_t)))
                        {
                          xmemcpy(wszTitle, wpTitleBegin, nTitleLen * sizeof(wchar_t));
                          wszTitle[nTitleLen]=L'\0';

                          if (lpTitleInfo=StackInsertTitle(&hTitleStack))
                          {
                            if (bExactTitle && (dwTitleFlags & TF_REGEXP))
                            {
                              if (sregTitle=(STACKREGROUP *)GlobalAlloc(GPTR, sizeof(STACKREGROUP)))
                              {
                                sregTitle->dwOptions=REO_MULTILINE;
                                if (bCompleteCaseSensitive)
                                  sregTitle->dwOptions|=REO_MATCHCASE;
                                if (dwTitleFlags & TF_FORCECASESENSITIVE)
                                  sregTitle->dwOptions|=REO_MATCHCASE;
                                else if (dwTitleFlags & TF_FORCECASEINSENSITIVE)
                                  sregTitle->dwOptions&=~REO_MATCHCASE;
                                if (PatCompile(sregTitle, wszTitle, wszTitle + nTitleLen))
                                {
                                  xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_REGEXP_COMPILEERROR), lpSyntaxFile->wszSyntaxFileName, nTitleLen, wszTitle);
                                  MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
                                  goto FreeBlock;
                                }
                                //sregTitle->first->dwFlags&=~REGF_ROOTANY;
                                lpTitleInfo->sregTitle=sregTitle;
                                sregTitle=NULL;
                              }
                              else goto FreeBlock;
                            }
                            lpTitleInfo->wpTitle=wszTitle;
                            lpTitleInfo->nTitleLen=nTitleLen;
                            lpTitleInfo->dwTitleFlags=dwTitleFlags;
                            lpTitleInfo->bExactTitle=bExactTitle;
                          }
                          else break;

                          wszTitle=NULL;
                        }
                        else break;
                      }
                    }
                    else break;
                  }
                  if (*wpText == L'\r')
                  {
                    NextLine(&wpText);
                    break;
                  }
                  ++wpText;
                }

                //Title block
                if (*wpText && hTitleStack.first)
                {
                  wpBlockBegin=wpText;
                  while (*wpText)
                  {
                    if (*wpText == L'$')
                    {
                      //Find next abbreviation definition
                      if (*++wpText == L'~' || *wpText == L'=' || *wpText == L'(')
                      {
                        --wpText;
                        break;
                      }
                    }
                    if (!NextLine(&wpText))
                      break;
                  }
                  wpBlockEnd=wpText;
                  nBlockLen=(int)(wpBlockEnd - wpBlockBegin);

                  if (nBlockParsedLen=ParseBlock(lpSyntaxFile, &hHotSpotStack, wpBlockBegin, nBlockLen, NULL, NULL))
                  {
                    if (wszBlockParsed=(wchar_t *)GlobalAlloc(GPTR, nBlockParsedLen * sizeof(wchar_t)))
                    {
                      nBlockParsedLen=ParseBlock(lpSyntaxFile, &hHotSpotStack, wpBlockBegin, nBlockLen, wszBlockParsed, &nLinesInBlock);
                      if (nBlockParsedLen > 0 && wszBlockParsed[nBlockParsedLen - 1] == L'\r') wszBlockParsed[--nBlockParsedLen]=L'\0';
                      if (nBlockParsedLen > 0 && wszBlockParsed[nBlockParsedLen - 1] == L'\r') wszBlockParsed[--nBlockParsedLen]=L'\0';

                      for (lpTitleInfo=(TITLEINFO *)hTitleStack.first; lpTitleInfo; lpTitleInfo=lpTitleInfo->next)
                      {
                        if (lpTitleInfo->bExactTitle)
                          lpBlockInfo=StackInsertExactBlock(&lpSyntaxFile->hExactBlockStack);
                        else
                          lpBlockInfo=StackInsertBlock(&lpSyntaxFile->hBlockStack, &lpSyntaxFile->hBlockOrderStack, lpTitleInfo->wpTitle);

                        if (lpBlockInfo)
                        {
                          lpBlockInfo->wpTitle=lpTitleInfo->wpTitle;
                          lpBlockInfo->nTitleLen=lpTitleInfo->nTitleLen;
                          lpBlockInfo->dwTitleFlags=lpTitleInfo->dwTitleFlags;
                          lpBlockInfo->bExactTitle=lpTitleInfo->bExactTitle;
                          lpBlockInfo->sregTitle=lpTitleInfo->sregTitle;

                          if (lpTitleInfo == (TITLEINFO *)hTitleStack.first)
                          {
                            lpBlockMaster=lpBlockInfo;
                            lpBlockInfo->master=NULL;
                            lpBlockInfo->wpBlock=wszBlockParsed;
                            lpBlockInfo->nBlockLen=nBlockParsedLen;
                            lpBlockInfo->nLinesInBlock=nLinesInBlock;
                            lpBlockInfo->hHotSpotStack=hHotSpotStack;
                          }
                          else lpBlockInfo->master=lpBlockMaster;

                          if (hTitleStack.first != hTitleStack.last)
                          {
                            //Multiple abbreviations
                            if (!StackInsertIndex((stack **)&lpBlockMaster->firstHandle, (stack **)&lpBlockMaster->lastHandle, (stack **)&lpBlockInfoHandle, -1, sizeof(BLOCKINFOHANDLE)))
                            {
                              lpBlockInfoHandle->lpBlockInfo=lpBlockInfo;
                            }
                          }
                        }
                      }
                      wszBlockParsed=NULL;
                    }
                  }
                }

                //Free
                FreeBlock:
                if (!lpBlockInfo)
                  StackFreeHotSpot(&hHotSpotStack);
                StackFreeTitle(&hTitleStack);
                if (sregTitle) GlobalFree((HGLOBAL)sregTitle);
                if (wszTitle) GlobalFree((HGLOBAL)wszTitle);
                if (wszBlockParsed) GlobalFree((HGLOBAL)wszBlockParsed);
              }
            }
            else
            {
              xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_UNKNOWNSECTION), lpSyntaxFile->wszSyntaxFileName, wpSectionStart - wpTextStart, wszBuffer);
              MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
              goto FreeText;
            }
            goto SectionStart;
          }
          lpLoadSyntaxFile=NULL;
        }
        FreeText:
        GlobalFree((HGLOBAL)wszText);
      }
    }
  }
  CloseHandle(hFile);

  return lpSyntaxFile;
}

void StackRequestSyntaxFile(SYNTAXFILE *lpSyntaxFile)
{
  if (lpSyntaxFile->bCache)
  {
    lpSyntaxFile->bCache=FALSE;
    if (!lpSyntaxFile->bReserveCacheWildcard)
      StackFreeWildcard(&lpSyntaxFile->hWildcardStack);
    StackLoadSyntaxFile(&hSyntaxFilesStack, lpSyntaxFile);
    dwSaveCache|=SC_SAVE;
  }
}

SYNTAXFILE* StackAddSyntaxFile(STACKSYNTAXFILE *hStack, const wchar_t *wpFile)
{
  SYNTAXFILE *lpElement;

  if (!StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, NULL, (stack **)&lpElement, sizeof(SYNTAXFILE)))
    xstrcpynW(lpElement->wszSyntaxFileName, wpFile, MAX_PATH);
  return lpElement;
}

SYNTAXFILE* StackPushSortSyntaxFile(STACKSYNTAXFILE *hStack, const wchar_t *wpFile, int nUpDown)
{
  SYNTAXFILE *lpElement;
  SYNTAXFILE *lpNewElement;
  int i;

  if (nUpDown != 1 && nUpDown != -1) return NULL;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    i=xstrcmpiW(lpElement->wszSyntaxFileName, wpFile);

    if (i == 0 || i == nUpDown)
      break;
  }
  if (!StackInsertBefore((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement, (stack **)&lpNewElement, sizeof(SYNTAXFILE)))
    xstrcpynW(lpNewElement->wszSyntaxFileName, wpFile, MAX_PATH);
  return lpNewElement;
}

SYNTAXFILE* StackGetSyntaxFileByFile(STACKSYNTAXFILE *hStack, const wchar_t *wpFile)
{
  SYNTAXFILE *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (StackGetWildcard(&lpElement->hWildcardStack, wpFile))
      return lpElement;
  }
  return NULL;
}

SYNTAXFILE* StackGetSyntaxFileByName(STACKSYNTAXFILE *hStack, const wchar_t *wpSyntaxFileName)
{
  SYNTAXFILE *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wszSyntaxFileName, wpSyntaxFileName))
      return lpElement;
  }
  return NULL;
}

SYNTAXFILE* StackGetSyntaxFileByTheme(STACKSYNTAXFILE *hStack, HANDLE hTheme)
{
  SYNTAXFILE *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hThemeHighLight == hTheme)
      return lpElement;
  }
  return NULL;
}

SYNTAXFILE* StackGetSyntaxFileByIndex(STACKSYNTAXFILE *hStack, int nIndex)
{
  SYNTAXFILE *lpElement;

  StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpElement, nIndex);
  return lpElement;
}

SYNTAXFILE* StackGetSyntaxFileByWindow(STACKSYNTAXFILE *hStack, HWND hWnd, AEHDOC hDoc, const wchar_t **wppAlias)
{
  SYNTAXFILE *hTheme=NULL;
  MANUALSET *lpManual;
  FRAMEDATA *fd;
  const wchar_t *wpFile=L"";

  if (hDoc)
    fd=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYEDITDOCUMENT, (LPARAM)hDoc);
  else
    fd=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYEDITWINDOW, (LPARAM)hWnd);

  if (fd)
  {
    if (lpManual=StackGetManual(&hManualStack, fd->ei.hWndMaster?fd->ei.hWndMaster:fd->ei.hWndEdit, fd->ei.hDocMaster?fd->ei.hDocMaster:fd->ei.hDocEdit))
      wpFile=lpManual->wszAlias;
    else
      wpFile=GetFileName(fd->ei.wszFile, -1);
  }
  else
  {
    //Is window assigned with DLLA_HIGHLIGHT_ADDWINDOW, DLLA_CODEFOLD_ADDWINDOW?
    if (lpManual=StackGetManual(&hManualStack, hWnd, NULL))
      wpFile=lpManual->wszAlias;
  }

  if (xstrcmpW(wpFile, L"?"))
  {
    if (!(hTheme=StackGetSyntaxFileByFile(hStack, wpFile)))
    {
      if (bDefaultAliasEnable)
      {
        if (hTheme=StackGetSyntaxFileByFile(hStack, wszDefaultAlias))
          wpFile=wszDefaultAlias;
      }
    }
  }
  if (wppAlias) *wppAlias=wpFile;

  return hTheme;
}

void StackUnsetLink(STACKSYNTAXFILE *hStack, VARTHEME *lpVarTheme)
{
  SYNTAXFILE *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->lpVarThemeLink == lpVarTheme)
      lpElement->lpVarThemeLink=NULL;
  }
}

void StackFreeSyntaxFiles(STACKSYNTAXFILE *hStack)
{
  SYNTAXFILE *lpElement=hStack->first;
  HWND hWndCurEdit=GetCurEdit();

  while (lpElement)
  {
    StackFreeWildcard(&lpElement->hWildcardStack);

    StackFreeDelimiter(&lpElement->hDelimiterStack);
    StackFreeWord(&lpElement->hWordStack, &lpElement->hWordAlphaStack, &lpElement->hWordOrderStack);
    StackFreeQuote(&lpElement->hQuoteStack);
    if (hWndCurEdit && lpElement->hThemeHighLight)
      SendMessage(hWndCurEdit, AEM_HLDELETETHEME, (WPARAM)lpElement->hThemeHighLight, 0);

    StackFreeSkipInfo(&lpElement->hSkipStack, &lpElement->hSkipStartStack);
    StackFreeFoldInfo(&lpElement->hFoldStack, &lpElement->hFoldStartStack);

    if (lpElement->hCompleteListFont)
      DeleteObject(lpElement->hCompleteListFont);
    if (lpElement->hCompleteListBlockIcon)
      DestroyIcon(lpElement->hCompleteListBlockIcon);
    if (lpElement->hCompleteListHlBaseIcon)
      DestroyIcon(lpElement->hCompleteListHlBaseIcon);
    if (lpElement->hCompleteListDocWordIcon)
      DestroyIcon(lpElement->hCompleteListDocWordIcon);

    StackFreeTitle(&lpElement->hTitleStack);
    StackFreeBlock(&lpElement->hBlockStack, &lpElement->hBlockOrderStack);
    StackFreeBlock(&lpElement->hExactBlockStack, NULL);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);

  //Empty lpSyntaxFile pointer
  if (nInitCodeFold)
  {
    FOLDWINDOW *lpFoldWindow;

    for (lpFoldWindow=(FOLDWINDOW *)hFoldWindowsStack.first; lpFoldWindow; lpFoldWindow=lpFoldWindow->next)
    {
      lpFoldWindow->pfwd->lpSyntaxFile=NULL;
    }
  }

  if (hFoldFilterBkBrush)
  {
    DeleteObject(hFoldFilterBkBrush);
    hFoldFilterBkBrush=NULL;
  }
}

WILDCARDINFO* StackInsertWildcard(STACKWILDCARD *hStack, int nWildcardLen)
{
  WILDCARDINFO *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(WILDCARDINFO));
  return lpElement;
}

WILDCARDINFO* StackGetWildcard(STACKWILDCARD *hStack, const wchar_t *wpFile)
{
  WILDCARDINFO *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (FileMaskCmp(lpElement->wpWildcard, wpFile))
      return lpElement;
  }
  return NULL;
}

void StackFreeWildcard(STACKWILDCARD *hStack)
{
  WILDCARDINFO *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    GlobalFree((HGLOBAL)lpElement->wpWildcard);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

DELIMITERINFO* StackInsertDelimiter(STACKDELIM *hStack, int nDelimiterLen)
{
  DELIMITERINFO *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(DELIMITERINFO));
  return lpElement;
}

DELIMITERINFO* StackGetDelimiter(STACKDELIM *hStack, wchar_t wchDelimiter)
{
  DELIMITERINFO *lpElement=(DELIMITERINFO *)hStack->first;

  while (lpElement)
  {
    if (lpElement->wpDelimiter[0] == wchDelimiter ||
        (!(lpElement->dwFlags & AEHLF_MATCHCASE) && WideCharLower(lpElement->wpDelimiter[0]) == WideCharLower(wchDelimiter)))
      return lpElement;

    lpElement=lpElement->next;
  }
  return NULL;
}

void StackFreeDelimiter(STACKDELIM *hStack)
{
  DELIMITERINFO *lpElement=(DELIMITERINFO *)hStack->first;

  while (lpElement)
  {
    GlobalFree((HGLOBAL)lpElement->wpDelimiter);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

wchar_t* AKD_wcschr(const wchar_t *s, wchar_t c)
{
  while (*s != L'\0' && *s != c)
    ++s;
  if (*s != L'\0')
    return ((wchar_t *)s);
  return NULL;
}

BOOL IsInDelimiterList(const wchar_t *wpList, int nListLen, wchar_t c)
{
  const wchar_t *wpMaxList=wpList + nListLen;

  while (wpList < wpMaxList && *wpList != c)
    ++wpList;
  return (wpList < wpMaxList);
}

BOOL IsDelimiter(STACKDELIM *hDelimiterStack, HWND hWnd, int nChar)
{
  if (nChar < 0)
    return TRUE;
  if (nChar == L'\r' || nChar == L'\n')
    return TRUE;

  if (hDelimiterStack)
  {
    if (StackGetDelimiter(hDelimiterStack, (wchar_t)nChar))
      return TRUE;
  }
  else
  {
    static wchar_t wszDelimiters[128];
    static HWND hWndPrev=NULL;
    static int nDelimitersLen;

    if (hWnd != hWndPrev)
    {
      hWndPrev=hWnd;
      nDelimitersLen=(int)SendMessage(hWnd, AEM_GETWORDDELIMITERS, 128, (LPARAM)wszDelimiters) - 2;
    }
    return IsInDelimiterList(wszDelimiters, nDelimitersLen, (wchar_t)nChar);
  }
  return FALSE;
}

BOOL IsDelimiterFromLeft(STACKDELIM *hDelimiterStack, HWND hWnd, const AECHARINDEX *ciChar)
{
  AECHARINDEX ciCount=*ciChar;

  if (AEC_IsFirstCharInLine(&ciCount))
    return TRUE;
  if (AEC_PrevCharInLine(&ciCount))
    return IsDelimiter(hDelimiterStack, hWnd, AEC_CharAtIndex(&ciCount));
  return TRUE;
}

BOOL IsDelimiterFromRight(STACKDELIM *hDelimiterStack, HWND hWnd, const AECHARINDEX *ciChar)
{
  if (AEC_IsLastCharInLine(ciChar))
    return TRUE;
  return IsDelimiter(hDelimiterStack, hWnd, AEC_CharAtIndex(ciChar));
}

MANUALSET* StackInsertManual(STACKMANUALSET *hStack)
{
  MANUALSET *lpElement=NULL;

  StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(MANUALSET));

  return lpElement;
}

MANUALSET* StackGetManual(STACKMANUALSET *hStack, HWND hWndMaster, AEHDOC hDocMaster)
{
  MANUALSET *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hWndEdit == hWndMaster)
    {
      if (lpElement->hWndParent)
        return lpElement;
      break;
    }
  }

  if (lpElement && nMDI == WMD_PMDI)
  {
    for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
    {
      if (lpElement->hDocEdit == hDocMaster)
        return lpElement;
    }
  }
  return lpElement;
}

MANUALSET* StackGetManualByUserParent(STACKMANUALSET *hStack, HWND hWndParent)
{
  MANUALSET *lpElement=hStack->first;

  while (lpElement)
  {
    if (lpElement->hWndParent == hWndParent)
      return lpElement;

    lpElement=lpElement->next;
  }
  return NULL;
}

void StackDeleteManual(STACKMANUALSET *hStack, MANUALSET *lpManual, DWORD dwDllFunction)
{
  if (lpManual->hWndParent)
  {
    if ((lpManual->dwDllFunction & CODER_HIGHLIGHT) && (dwDllFunction & CODER_HIGHLIGHT))
    {
      if (lpManual->lpHighlightWindow)
      {
        StackDeleteHighLightWindow(&hHighLightWindowsStack, lpManual->lpHighlightWindow);
        lpManual->lpHighlightWindow=NULL;
      }
      lpManual->dwDllFunction&=~CODER_HIGHLIGHT;
    }
    if ((lpManual->dwDllFunction & CODER_CODEFOLD) && (dwDllFunction & CODER_CODEFOLD))
    {
      if (lpManual->lpFoldWindow)
      {
        StackEndBoard(&hFoldWindowsStack, lpManual->lpFoldWindow);
        StackDeleteFoldWindow(&hFoldWindowsStack, lpManual->lpFoldWindow);
        lpManual->lpFoldWindow=NULL;
      }
      lpManual->dwDllFunction&=~CODER_CODEFOLD;
    }
    if ((lpManual->dwDllFunction & CODER_AUTOCOMPLETE) && (dwDllFunction & CODER_AUTOCOMPLETE))
    {
      lpManual->dwDllFunction&=~CODER_AUTOCOMPLETE;
    }

    if (!lpManual->dwDllFunction)
    {
      SetWindowLongPtrWide(lpManual->hWndEdit, GWLP_WNDPROC, (UINT_PTR)lpManual->lpOldEditProc);
      lpManual->lpOldEditProc=NULL;
      SetWindowLongPtrWide(lpManual->hWndParent, GWLP_WNDPROC, (UINT_PTR)lpManual->lpOldParentProc);
      lpManual->lpOldParentProc=NULL;
    }
    else return;
  }
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpManual);
}

VARTHEME* StackInsertVarTheme(STACKVARTHEME *hStack, int nIndex)
{
  VARTHEME *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, nIndex, sizeof(VARTHEME)))
  {
    lpElement->hVarStack.lpVarThemeOwner=lpElement;
  }
  return lpElement;
}

VARTHEME* StackGetVarThemeByName(STACKVARTHEME *hStack, const wchar_t *wpVarThemeName)
{
  VARTHEME *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(wpVarThemeName, lpElement->wszVarThemeName))
      return lpElement;
  }
  return NULL;
}

VARTHEME* StackGetVarThemeByIndex(STACKVARTHEME *hStack, int nIndex)
{
  VARTHEME *lpElement;

  StackGetElement((stack *)hStack->first, (stack *)hStack->last, (stack **)&lpElement, nIndex);
  return lpElement;
}

int StackGetVarThemeIndex(STACKVARTHEME *hStack, VARTHEME *lpVarTheme)
{
  return StackGetIndex((stack *)hStack->first, (stack *)hStack->last, (stack *)lpVarTheme, TRUE);
}

void StackMoveVarTheme(STACKVARTHEME *hStack, VARTHEME *lpVarTheme, int nIndex)
{
  StackMoveIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpVarTheme, nIndex);
}

void StackCopyVarTheme(VARTHEME *vtSrc, VARTHEME *vtDst)
{
  VARINFO *viSrc;
  VARINFO *viDsc;

  StackFreeVars(&vtDst->hVarStack);

  for (viSrc=vtSrc->hVarStack.first; viSrc; viSrc=viSrc->next)
  {
    if (!StackInsertBefore((stack **)&vtDst->hVarStack.first, (stack **)&vtDst->hVarStack.last, NULL, (stack **)&viDsc, sizeof(VARINFO)))
    {
      if (viDsc->wpVarName=(wchar_t *)GlobalAlloc(GPTR, (viSrc->nVarNameLen + 1) * sizeof(wchar_t)))
        viDsc->nVarNameLen=(int)xstrcpynW(viDsc->wpVarName, viSrc->wpVarName, viSrc->nVarNameLen + 1);
      if (viDsc->wpVarValue=(wchar_t *)GlobalAlloc(GPTR, (viSrc->nVarValueLen + 1) * sizeof(wchar_t)))
        viDsc->nVarValueLen=(int)xstrcpynW(viDsc->wpVarValue, viSrc->wpVarValue, viSrc->nVarValueLen + 1);
      viDsc->dwVarFlags=viSrc->dwVarFlags;
    }
    else break;
  }
  if (!viSrc)
  {
    vtDst->hVarStack.lpVarThemeOwner=vtDst;
    vtDst->nVarThemeNameLen=(int)xstrcpynW(vtDst->wszVarThemeName, vtSrc->wszVarThemeName, MAX_PATH);
    vtDst->wpTextData=NULL;
  }
}

void StackDeleteVarTheme(STACKVARTHEME *hStack, VARTHEME *lpVarTheme)
{
  if (lpVarTheme == lpVarThemeActive)
  {
    //Set to default theme
    lpVarThemeActive=(VARTHEME *)hStack->first;
  }
  StackFreeVars(&lpVarTheme->hVarStack);
  StackUnsetLink(&hSyntaxFilesStack, lpVarTheme);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpVarTheme);
}

void StackFreeVarThemes(STACKVARTHEME *hStack)
{
  VARTHEME *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    StackFreeVars(&lpElement->hVarStack);
  }
  nVarThemesInternalCount=0;
  lpVarThemeActive=NULL;

  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

void AddInternalVarThemes(STACKVARTHEME *hStack)
{
  const wchar_t *lpThemes[][2]={{L"Default", TXT_DEFAULT_VARTHEME},
                                {L"Active4D", TXT_ACTIVE4D_VARTHEME},
                                {L"Bespin", TXT_BESPIN_VARTHEME},
                                {L"Cobalt", TXT_COBALT_VARTHEME},
                                {L"Dawn", TXT_DAWN_VARTHEME},
                                {L"Earth", TXT_EARTH_VARTHEME},
                                {L"iPlastic", TXT_IPLASTIC_VARTHEME},
                                {L"Lazy", TXT_LAZY_VARTHEME},
                                {L"Mac Classic", TXT_MACCLASSIC_VARTHEME},
                                {L"Monokai", TXT_MONOKAI_VARTHEME},
                                {L"Obsidian", TXT_OBSIDIAN_VARTHEME},
                                {L"Solarized Light", TXT_SOLARIZEDLIGHT_VARTHEME},
                                {L"Solarized Dark", TXT_SOLARIZEDDARK_VARTHEME},
                                {L"SpaceCadet", TXT_SPACECADET_VARTHEME},
                                {L"Sunburst", TXT_SUNBURST_VARTHEME},
                                {L"Twilight", TXT_TWILIGHT_VARTHEME},
                                {L"Zenburn", TXT_ZENBURN_VARTHEME},
                                {0, 0}};
  VARTHEME *lpVarTheme;
  int i;

  for (i=0; lpThemes[i][0]; ++i)
  {
    if (lpVarTheme=StackInsertVarTheme(hStack, -1))
    {
      lpVarTheme->nVarThemeNameLen=(int)xstrcpyW(lpVarTheme->wszVarThemeName, lpThemes[i][0]);
      lpVarTheme->wpTextData=lpThemes[i][1];
    }
    ++nVarThemesInternalCount;
  }
}

VARTHEME* RequestVarTheme(STACKVARTHEME *hStack, const wchar_t *wpVarThemeName)
{
  VARTHEME *lpVarTheme;

  for (lpVarTheme=hStack->first; lpVarTheme; lpVarTheme=lpVarTheme->next)
  {
    if (!wpVarThemeName || !xstrcmpiW(wpVarThemeName, lpVarTheme->wszVarThemeName))
    {
      if (!lpVarTheme->hVarStack.first)
        ParseStringToVars(&lpVarTheme->hVarStack, lpVarTheme->wpTextData);
      if (wpVarThemeName) return lpVarTheme;
    }
  }
  return NULL;
}

void GetVarThemeGlobals(VARTHEME *lpVarTheme)
{
  VARINFO *lpVarInfo;

  //Set default values
  dwAutoMarkFlags=AEHLF_MATCHCASE;
  dwAutoMarkFontStyle=AEHLS_NONE;
  dwAutoMarkTextColor=RGB(0x00, 0x00, 0x00);
  dwAutoMarkBkColor=RGB(0x75, 0xD9, 0x75);
  dwFoldListTextColor=(DWORD)-1;
  dwFoldListBkColor=(DWORD)-1;

  //Non-default variable theme
  for (;;)
  {
    //AutoMark:
    if (!(lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, L"HighLight_AutoMarkFlags", -1)))
      break;
    dwAutoMarkFlags=(DWORD)xatoiW(lpVarInfo->wpVarValue, NULL);

    lpVarInfoFastCheck=lpVarInfo->next;
    if (!(lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, L"HighLight_AutoMarkFontStyle", -1)))
      break;
    dwAutoMarkFontStyle=(DWORD)xatoiW(lpVarInfo->wpVarValue, NULL);

    lpVarInfoFastCheck=lpVarInfo->next;
    if (!(lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, L"HighLight_AutoMarkTextColor", -1)))
      break;
    if (lpVarInfo->wpVarValue[0] == L'#')
      dwAutoMarkTextColor=GetColorFromStr(lpVarInfo->wpVarValue + 1, NULL);
    else
      dwAutoMarkTextColor=(DWORD)-1;

    lpVarInfoFastCheck=lpVarInfo->next;
    if (!(lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, L"HighLight_AutoMarkBkColor", -1)))
      break;
    if (lpVarInfo->wpVarValue[0] == L'#')
      dwAutoMarkBkColor=GetColorFromStr(lpVarInfo->wpVarValue + 1, NULL);
    else
      dwAutoMarkBkColor=(DWORD)-1;

    //Panel colors
    lpVarInfoFastCheck=lpVarInfo->next;
    if (!(lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, L"CodeFold_ListTextColor", -1)))
      break;
    if (lpVarInfo->wpVarValue[0] == L'#')
      dwFoldListTextColor=GetColorFromStr(lpVarInfo->wpVarValue + 1, NULL);
    else
      dwFoldListTextColor=(DWORD)-1;

    lpVarInfoFastCheck=lpVarInfo->next;
    if (!(lpVarInfo=StackGetVarByName(&lpVarTheme->hVarStack, L"CodeFold_ListBkColor", -1)))
      break;
    if (lpVarInfo->wpVarValue[0] == L'#')
      dwFoldListBkColor=GetColorFromStr(lpVarInfo->wpVarValue + 1, NULL);
    else
      dwFoldListBkColor=(DWORD)-1;

    break;
  }
}

int FillVarThemeList(HWND hWnd, VARTHEME *lpVarTheme)
{
  VARINFO *lpVarInfo;
  LVITEMW lvi;
  int nIndex=0;

  SendMessage(hWnd, LVM_DELETEALLITEMS, 0, 0);

  for (lpVarInfo=lpVarTheme->hVarStack.first; lpVarInfo; lpVarInfo=lpVarInfo->next)
  {
    lvi.mask=LVIF_TEXT|LVIF_PARAM;
    lvi.pszText=lpVarInfo->wpVarName;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVSI_LIST_VARIABLE;
    lvi.lParam=(LPARAM)lpVarInfo;
    ListView_InsertItemWide(hWnd, &lvi);

    lvi.mask=LVIF_TEXT;
    lvi.pszText=lpVarInfo->wpVarValue;
    lvi.iItem=nIndex;
    lvi.iSubItem=LVSI_LIST_VALUE;
    ListView_SetItemWide(hWnd, &lvi);

    ++nIndex;
  }
  return nIndex;
}

LPARAM ListViewItemParam(HWND hWnd, int nItem)
{
  LVITEMW lvi;

  lvi.mask=LVIF_PARAM;
  lvi.iItem=nItem;
  if (ListView_GetItemWide(hWnd, &lvi))
    return lvi.lParam;
  return 0;
}

VARINFO* StackInsertVar(STACKVAR *hStack, const wchar_t *wpVarName, int nVarNameLen, const wchar_t *wpVarValue, int nVarValueLen, BOOL bUnescape)
{
  VARINFO *lpVarInfo=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpVarInfo, -1, sizeof(VARINFO)))
  {
    if (lpVarInfo->wpVarName=(wchar_t *)GlobalAlloc(GPTR, (nVarNameLen + 1) * sizeof(wchar_t)))
      lpVarInfo->nVarNameLen=(int)xstrcpynW(lpVarInfo->wpVarName, wpVarName, nVarNameLen + 1);
    if (lpVarInfo->wpVarValue=(wchar_t *)GlobalAlloc(GPTR, (nVarValueLen + 1) * sizeof(wchar_t)))
    {
      if (bUnescape)
        lpVarInfo->nVarValueLen=(int)UnescapeString(wpVarValue, nVarValueLen, lpVarInfo->wpVarValue);
      else
        lpVarInfo->nVarValueLen=(int)xstrcpynW(lpVarInfo->wpVarValue, wpVarValue, nVarValueLen + 1);
    }
  }
  return lpVarInfo;
}

VARINFO* StackGetVarByName(STACKVAR *hStack, const wchar_t *wpVarName, int nVarNameLen)
{
  STACKVAR *hStackCurrent;
  VARINFO *lpElement=lpVarInfoFastCheck;
  VARINFO *lpLowPriority=NULL;

  if (lpElement)
  {
    lpVarInfoFastCheck=NULL;

    if ((nVarNameLen == lpElement->nVarNameLen && !xstrcmpnW(wpVarName, lpElement->wpVarName, nVarNameLen)) ||
        (nVarNameLen == -1 && !xstrcmpW(wpVarName, lpElement->wpVarName)))
    {
      return lpElement;
    }
  }

  //Firstly search in GLOBAL var theme
  hStackCurrent=&hVarThemeGlobal.hVarStack;

  for (;;)
  {
    for (lpElement=hStackCurrent->first; lpElement; lpElement=lpElement->next)
    {
      if ((nVarNameLen == lpElement->nVarNameLen && !xstrcmpnW(wpVarName, lpElement->wpVarName, nVarNameLen)) ||
          (nVarNameLen == -1 && !xstrcmpW(wpVarName, lpElement->wpVarName)))
      {
        if ((lpElement->dwVarFlags & VARF_LOWPRIORITY) && hStackCurrent == &hVarThemeGlobal.hVarStack)
        {
          lpLowPriority=lpElement;
          break;
        }
        return lpElement;
      }
    }
    if (hStackCurrent == hStack)
      break;
    //Secondly search in input var theme
    hStackCurrent=hStack;
  }
  if (lpLowPriority)
    return lpLowPriority;
  return NULL;
}

VARINFO* StackGetVarByIndex(STACKVAR *hStack, int nIndex)
{
  VARINFO *lpElement;
  int nCount=0;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (nCount++ == nIndex)
      break;
  }
  return lpElement;
}

void StackDeleteVar(STACKVAR *hStack, VARINFO *lpElement)
{
  if (lpElement->wpVarName) GlobalFree((HGLOBAL)lpElement->wpVarName);
  if (lpElement->wpVarValue) GlobalFree((HGLOBAL)lpElement->wpVarValue);
  StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement);
}

void StackFreeVars(STACKVAR *hStack)
{
  VARINFO *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->wpVarName) GlobalFree((HGLOBAL)lpElement->wpVarName);
    if (lpElement->wpVarValue) GlobalFree((HGLOBAL)lpElement->wpVarValue);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
}

int ParseStringToVars(STACKVAR *lpVarStack, const wchar_t *wpText)
{
  VARINFO *lpVarInfo;
  wchar_t wszVarName[MAX_PATH];
  const wchar_t *wpCount=wpText;
  const wchar_t *wpVarValueStart;
  const wchar_t *wpVarValueEnd;
  int nVarNameLen;
  int nVarValueLen;
  int nVarCount=0;
  DWORD dwVarFlags;

  do
  {
    //Var name
    if (nVarNameLen=GetWord(wpCount, wszVarName, MAX_PATH, &wpCount, NULL, lpVarStack))
    {
      if (wszVarName[0] == L';')
        continue;
    }
    else break;

    //Var value
    if (nVarValueLen=(int)GetEscapeParam(wpCount, &wpVarValueStart, &wpVarValueEnd, &wpCount))
    {
      if (wpVarValueStart[0] == L'#')
      {
        //Make length at least 7 wide characters for "#RRGGBB"
        nVarValueLen=max(nVarValueLen, 7);
      }
    }

    //Var flags
    if (GetWord(wpCount, wszBuffer, BUFFER_SIZE, &wpCount, NULL, lpVarStack))
      dwVarFlags=(DWORD)xatoiW(wszBuffer, NULL);
    else
      dwVarFlags=0;

    if (lpVarInfo=StackInsertVar(lpVarStack, wszVarName, nVarNameLen, wpVarValueStart, nVarValueLen, TRUE))
    {
      lpVarInfo->dwVarFlags=dwVarFlags;

      ++nVarCount;
    }
  }
  while (NextLine(&wpCount));

  return nVarCount;
}

DWORD ParseVarsToString(STACKVAR *lpVarStack, wchar_t **wpText)
{
  VARINFO *lpVarInfo;
  wchar_t *wszVarThemeValue;
  DWORD dwSize;

  //Variable theme data
  for (dwSize=0, lpVarInfo=lpVarStack->first; lpVarInfo; lpVarInfo=lpVarInfo->next)
  {
    dwSize+=lpVarInfo->nVarNameLen + 2 + (lpVarInfo->nVarValueLen * 2) + 2 + 10 + 1; //wpVarName "wpVarValue" dwFlags\r
  }
  dwSize+=sizeof(wchar_t);

  if (wpText)
  {
    if (wszVarThemeValue=(wchar_t *)GlobalAlloc(GPTR, dwSize * sizeof(wchar_t)))
    {
      for (dwSize=0, lpVarInfo=lpVarStack->first; lpVarInfo; lpVarInfo=lpVarInfo->next)
      {
        dwSize+=(DWORD)xprintfW(wszVarThemeValue + dwSize, L"%s \"", lpVarInfo->wpVarName);
        dwSize+=(DWORD)EscapeString(lpVarInfo->wpVarValue, lpVarInfo->nVarValueLen, wszVarThemeValue + dwSize);
        if (lpVarStack == &hVarThemeGlobal.hVarStack)
          dwSize+=(DWORD)xprintfW(wszVarThemeValue + dwSize, L"\" %d\r", lpVarInfo->dwVarFlags);
        else
          dwSize+=(DWORD)xprintfW(wszVarThemeValue + dwSize, L"\"\r");
      }
      dwSize+=sizeof(wchar_t);
    }
    *wpText=wszVarThemeValue;
  }
  return dwSize;
}

int GetWord(const wchar_t *wpText, wchar_t *wszWord, int nWordMax, const wchar_t **wppNextWord, BOOL *lpbQuote, STACKVAR *lpVarStack)
{
  VARINFO *lpVarInfo=NULL;
  const wchar_t *wpCount;
  wchar_t wchStopChar;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'\"' || *wpText == L'\'' || *wpText == L'`')
  {
    if (lpbQuote) *lpbQuote=TRUE;
    wchStopChar=*wpText;
    wpCount=++wpText;

    //Parse: "param" or 'param' or `param`
    while (*wpCount != wchStopChar && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
    {
      *wppNextWord=wpCount;
      if (*wpCount == wchStopChar)
        ++*wppNextWord;
    }
  }
  else
  {
    if (lpbQuote) *lpbQuote=FALSE;
    wpCount=wpText;

    //Variable
    if (*wpText == L'$' && *(wpText + 1) == L'{')
    {
      for (wpCount=wpText + 2; *wpCount != L'}' && *wpCount != L'\0'; ++wpCount);

      if (!lpVarStack || !(lpVarInfo=StackGetVarByName(lpVarStack, wpText + 2, (int)(wpCount - (wpText + 2)))))
      {
        ++wpCount;
        xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_VARMISSING), lpLoadSyntaxFile?lpLoadSyntaxFile->wszSyntaxFileName:L"", wpCount - wpText, wpText, ((VARTHEME *)lpVarStack->lpVarThemeOwner)->wszVarThemeName);
        if (!bSyntaxFileLoadMute && MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
          bSyntaxFileLoadMute=TRUE;
        bSyntaxFileLoadError=TRUE;
        wpText=wpCount;
      }
    }

    //Parse: param1 param2 param3
    while (*wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;

    if (wppNextWord)
      *wppNextWord=wpCount;
  }
  if (lpVarInfo)
    return (int)xstrcpynW(wszWord, lpVarInfo->wpVarValue, nWordMax);
  return (int)xstrcpynW(wszWord, wpText, min(nWordMax, wpCount - wpText + 1));
}

INT_PTR ExpandVars(const wchar_t *wpString, INT_PTR nStringLen, wchar_t *wszBuffer, INT_PTR nBufferSize, STACKVAR *lpVarStack)
{
  const wchar_t *wpSource=wpString;
  const wchar_t *wpSourceMax=wpString + nStringLen;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=wszBuffer + (wszBuffer?nBufferSize:0x3FFFFFFF);
  const wchar_t *wpVarCount;
  VARINFO *lpVarInfo;

  while (wpSource < wpSourceMax && wpTarget < wpTargetMax)
  {
    if (*wpSource == L'$' && *(wpSource + 1) == L'{')
    {
      for (wpVarCount=wpSource + 2; *wpVarCount != L'}' && *wpVarCount != L'\0'; ++wpVarCount);

      if (lpVarStack && (lpVarInfo=StackGetVarByName(lpVarStack, wpSource + 2, (int)(wpVarCount - (wpSource + 2)))))
      {
        nBufferSize=min(wpTargetMax - wpTarget, lpVarInfo->nVarValueLen);
        if (wszBuffer)
          xmemcpy(wpTarget, lpVarInfo->wpVarValue, nBufferSize * sizeof(wchar_t));
        wpTarget+=nBufferSize;
        wpSource=++wpVarCount;
      }
      else
      {
        ++wpVarCount;
        xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_VARMISSING), lpLoadSyntaxFile?lpLoadSyntaxFile->wszSyntaxFileName:L"", wpVarCount - wpSource, wpSource, ((VARTHEME *)lpVarStack->lpVarThemeOwner)->wszVarThemeName);
        if (!bSyntaxFileLoadMute && MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OKCANCEL|MB_ICONEXCLAMATION) == IDCANCEL)
          bSyntaxFileLoadMute=TRUE;
        bSyntaxFileLoadError=TRUE;
        wpSource=wpVarCount;
      }
    }
    else
    {
      if (wszBuffer) *wpTarget=*wpSource;
      ++wpTarget;
      ++wpSource;
    }
  }
  if (wpTarget < wpTargetMax)
  {
    if (wszBuffer)
      *wpTarget=L'\0';
    else
      ++wpTarget;
  }
  return (wpTarget - wszBuffer);
}

BOOL NextLine(const wchar_t **wpText)
{
  while (**wpText != L'\r' && **wpText != L'\n' && **wpText != L'\0') ++*wpText;
  if (**wpText == L'\0') return FALSE;
  if (**wpText == L'\r') ++*wpText;
  if (**wpText == L'\n') ++*wpText;
  return TRUE;
}

BOOL SkipComment(const wchar_t **wpText)
{
  for (;;)
  {
    while (**wpText == L' ' || **wpText == L'\t') ++*wpText;

    if (**wpText == L';')
    {
      if (!NextLine(wpText))
        return FALSE;
    }
    else break;
  }
  if (**wpText == L'\0')
    return FALSE;
  return TRUE;
}

BOOL FileMaskCmp(const wchar_t *wpMaskStr, const wchar_t *wpFileStr)
{
  const wchar_t *wpMemMask=NULL;
  const wchar_t *wpMemFile=NULL;

  while (*wpFileStr)
  {
    if (*wpMaskStr == L'*')
    {
      do
      {
        if (!*++wpMaskStr) return TRUE;
      }
      while (*wpMaskStr == L'*' || *wpMaskStr == L'?');

      while (WideCharLower(*wpMaskStr) != WideCharLower(*wpFileStr))
      {
        if (!*++wpFileStr) goto End;
      }
      if (*wpMaskStr == L'.' && *(wpMaskStr + 1) == L'\0')
        return !*(wpFileStr + 1);
      wpMemMask=wpMaskStr++;
      wpMemFile=wpFileStr++;
    }
    else if (*wpMaskStr == L'?' || WideCharLower(*wpMaskStr) == WideCharLower(*wpFileStr))
    {
      ++wpMaskStr;
      ++wpFileStr;
    }
    else
    {
      if (!wpMemMask) return FALSE;
      wpMaskStr=wpMemMask;
      wpFileStr=++wpMemFile;
    }
  }

  End:
  if ((*wpMaskStr == L'.' && *(wpMaskStr + 1) == L'\0') ||
      (*wpMaskStr == L'*' && *(wpMaskStr + 1) == L'\0') ||
      (*wpMaskStr == L'*' && *(wpMaskStr + 1) == L'.' && *(wpMaskStr + 2) == L'\0'))
    return TRUE;
  return !*wpMaskStr;
}

const wchar_t* GetFileName(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; (INT_PTR)wpCount >= (INT_PTR)wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      return wpCount + 1;
  }
  return wpFile;
}

HWND GetCurEdit()
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    return ei.hWndEdit;
  return NULL;
}

HWND GetFocusEdit()
{
  MANUALSET *lpManual;
  HWND hWndFocus=GetFocus();

  if (hWndFocus)
  {
    if (lpManual=StackGetManual(&hManualStack, hWndFocus, NULL))
      return lpManual->hWndEdit;
  }
  return GetCurEdit();
}

void UpdateEdit(HWND hWnd, DWORD dwFlags)
{
  RECT rc;
  BOOL bErase=FALSE;

  if (dwFlags & UE_ERASE)
    bErase=TRUE;

  if (dwFlags & UE_ALLRECT)
  {
    InvalidateRect(hWnd, NULL, bErase);
  }
  else if (dwFlags & UE_DRAWRECT)
  {
    SendMessage(hWnd, EM_GETRECT, 0, (LPARAM)&rc);
    InvalidateRect(hWnd, &rc, bErase);
  }
  else if (dwFlags & UE_FIRSTPIXEL)
  {
    rc.left=0;
    rc.top=0;
    rc.right=1;
    rc.bottom=1;
    InvalidateRect(hWnd, &rc, bErase);
  }
  if (dwFlags & UE_UPDATEWINDOW)
    UpdateWindow(hWnd);
}

void UpdateEditAndClones(HWND hWnd, DWORD dwFlags)
{
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
  {
    if (ei.hWndMaster)
    {
      UpdateEdit(ei.hWndMaster, dwFlags);
      if (ei.hWndClone1)
        UpdateEdit(ei.hWndClone1, dwFlags);
      if (ei.hWndClone2)
        UpdateEdit(ei.hWndClone2, dwFlags);
      if (ei.hWndClone3)
        UpdateEdit(ei.hWndClone3, dwFlags);
    }
    else UpdateEdit(ei.hWndEdit, dwFlags);
  }
  else UpdateEdit(hWnd, dwFlags);
}

void UpdateEditAll(DWORD dwFlags)
{
  EDITINFO ei;
  BOOL bMaximized=TRUE;

  if (nMDI == WMD_MDI)
    SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);

  if (bMaximized)
  {
    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    {
      if (ei.hWndMaster)
      {
        UpdateEdit(ei.hWndMaster, dwFlags);
        if (ei.hWndClone1)
          UpdateEdit(ei.hWndClone1, dwFlags);
        if (ei.hWndClone2)
          UpdateEdit(ei.hWndClone2, dwFlags);
        if (ei.hWndClone3)
          UpdateEdit(ei.hWndClone3, dwFlags);
      }
      else UpdateEdit(ei.hWndEdit, dwFlags);
    }
  }
  else
  {
    //nMDI == WMD_MDI in non-maximized state
    FRAMEDATA *lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0);
    FRAMEDATA *lpFrameInit=lpFrameCount;

    while (lpFrameCount)
    {
      if (lpFrameCount->ei.hWndMaster)
      {
        UpdateEdit(lpFrameCount->ei.hWndMaster, dwFlags);
        if (lpFrameCount->ei.hWndClone1)
          UpdateEdit(lpFrameCount->ei.hWndClone1, dwFlags);
        if (lpFrameCount->ei.hWndClone2)
          UpdateEdit(lpFrameCount->ei.hWndClone2, dwFlags);
        if (lpFrameCount->ei.hWndClone3)
          UpdateEdit(lpFrameCount->ei.hWndClone3, dwFlags);
      }
      else UpdateEdit(lpFrameCount->ei.hWndEdit, dwFlags);

      if (lpFrameInit == (lpFrameCount=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_NEXT, (LPARAM)lpFrameCount)))
        break;
    }
  }
}

HFONT CreateFontMethod(const wchar_t *wpFaceName, DWORD dwFontStyle, int nPointSize)
{
  LOGFONTW lfNew;
  FRAMEDATA *lpFrame;
  HDC hDC;
  HFONT hFont=NULL;

  if (lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_CURRENT, 0))
  {
    xmemcpy(&lfNew, &lpFrame->lf, sizeof(LOGFONTW));

    if (nPointSize)
    {
      if (hDC=GetDC(hMainWnd))
      {
        lfNew.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
        ReleaseDC(hMainWnd, hDC);
      }
    }
    if (dwFontStyle != AEHLS_NONE)
    {
      lfNew.lfWeight=(dwFontStyle == AEHLS_FONTBOLD || dwFontStyle == AEHLS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
      lfNew.lfItalic=(dwFontStyle == AEHLS_FONTITALIC || dwFontStyle == AEHLS_FONTBOLDITALIC)?TRUE:FALSE;
    }
    if (*wpFaceName != L'\0')
    {
      xstrcpynW(lfNew.lfFaceName, wpFaceName, LF_FACESIZE);
    }
    hFont=CreateFontIndirectWide(&lfNew);
  }
  return hFont;
}

HICON GetIconMethod(wchar_t *wszIconFile, int nFileIconIndex, BOOL bBigIcons)
{
  wchar_t wszPath[MAX_PATH];
  SIZE sizeIcon;
  wchar_t *wpFileName;
  HICON hIcon=NULL;

  if (bBigIcons)
  {
    sizeIcon.cx=32;
    sizeIcon.cy=32;
  }
  else
  {
    sizeIcon.cx=16;
    sizeIcon.cy=16;
  }

  if (!*wszIconFile)
  {
    hIcon=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(nFileIconIndex + 100), IMAGE_ICON, sizeIcon.cx, sizeIcon.cy, 0);
  }
  else
  {
    if (TranslateFileString(wszIconFile, wszPath, MAX_PATH))
    {
      if (SearchPathWide(NULL, wszPath, NULL, MAX_PATH, wszIconFile, &wpFileName))
      {
        //if (bBigIcons)
        //  ExtractIconExWide(wszPath, nFileIconIndex, &hIcon, NULL, 1);
        //else
        //  ExtractIconExWide(wszPath, nFileIconIndex, NULL, &hIcon, 1);
        hIcon=IconExtractWide(wszPath, nFileIconIndex, sizeIcon.cx, sizeIcon.cy);
      }
    }
  }
  return hIcon;
}

void GetIconParameters(const wchar_t *wpText, wchar_t *wszIconFile, int nMaxIconFile, int *nIconIndex, const wchar_t **wppText)
{
  wchar_t wchStopChar;
  int i;

  wszIconFile[0]=L'\0';
  *nIconIndex=0;

  //File
  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'\"' || *wpText == L'\'' || *wpText == L'`')
  {
    wchStopChar=*wpText++;

    for (i=0; i < nMaxIconFile && *wpText != wchStopChar && *wpText != L'\0'; ++i, ++wpText)
    {
      wszIconFile[i]=*wpText;
    }
    wszIconFile[i]=L'\0';

    while (*wpText != L',' && *wpText != L')' && *wpText != L'\0')
      ++wpText;
    if (*wpText == L',')
      ++wpText;
    else
      goto End;
  }

  //Index
  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  *nIconIndex=(int)xatoiW(wpText, NULL);

  while (*wpText != L',' && *wpText != L')' && *wpText != L'\0')
    ++wpText;
  if (*wpText == L',')
    ++wpText;
  else
    goto End;

  //End
  End:
  if (*wpText == L')')
    ++wpText;
  if (wppText) *wppText=wpText;
}

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%a -AkelPad directory, %% -%
  wchar_t *wpExeDir=wszExeDir;
  wchar_t *wszSource;
  wchar_t *wpSource;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=(wszBuffer ? (wszBuffer + nBufferSize) : (wchar_t *)MAXUINT_PTR);
  int nStringLen;
  BOOL bStringStart=TRUE;

  //Expand environment strings
  nStringLen=ExpandEnvironmentStringsWide(wpString, NULL, 0);

  if (wszSource=(wchar_t *)GlobalAlloc(GPTR, nStringLen * sizeof(wchar_t)))
  {
    ExpandEnvironmentStringsWide(wpString, wszSource, nStringLen);

    //Expand plugin variables
    for (wpSource=wszSource; *wpSource && wpTarget < wpTargetMax;)
    {
      if (bStringStart && *wpSource == L'%')
      {
        if (*++wpSource == L'%')
        {
          ++wpSource;
          if (wszBuffer) *wpTarget=L'%';
          ++wpTarget;
        }
        else if (*wpSource == L'a' || *wpSource == L'A')
        {
          ++wpSource;
          wpTarget+=xstrcpynW(wszBuffer?wpTarget:NULL, wpExeDir, wpTargetMax - wpTarget) - !wszBuffer;
        }
      }
      else
      {
        if (*wpSource != L'\"' && *wpSource != L'\'' && *wpSource != L'`')
          bStringStart=FALSE;
        if (wszBuffer) *wpTarget=*wpSource;
        ++wpTarget;
        ++wpSource;
      }
    }
    if (wpTarget < wpTargetMax)
    {
      if (wszBuffer)
        *wpTarget=L'\0';
      else
        ++wpTarget;
    }
    GlobalFree((HGLOBAL)wszSource);
  }
  return (int)(wpTarget - wszBuffer);
}

INT_PTR EscapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput)
{
  //Escape: \ -> \\ and " -> \"
  const wchar_t *wpInputMax=wpInput + nInputLen;
  wchar_t *wpOutput=wszOutput;

  for (; wpInput < wpInputMax; ++wpInput)
  {
    if (*wpInput == L'\\')
    {
      if (wszOutput)
      {
        *wpOutput++=L'\\';
        *wpOutput++=L'\\';
      }
      else wpOutput+=2;
    }
    else if (*wpInput == L'\"')
    {
      if (wszOutput)
      {
        *wpOutput++=L'\\';
        *wpOutput++=L'\"';
      }
      else wpOutput+=2;
    }
    else
    {
      if (wszOutput)
        *wpOutput++=*wpInput;
      else
        wpOutput+=1;
    }
  }
  return (wpOutput - wszOutput);
}

INT_PTR UnescapeString(const wchar_t *wpInput, INT_PTR nInputLen, wchar_t *wszOutput)
{
  //Unescape: \\ -> \ and \" -> "
  const wchar_t *wpInputMax=wpInput + nInputLen;
  wchar_t *wpOutput=wszOutput;

  for (; wpInput < wpInputMax; ++wpInput)
  {
    if (*wpInput == L'\\')
      ++wpInput;

    if (wszOutput)
      *wpOutput++=*wpInput;
    else
      wpOutput+=1;
  }
  return (wpOutput - wszOutput);
}

INT_PTR GetEscapeParam(const wchar_t *wpText, const wchar_t **wpParamStart, const wchar_t **wpParamEnd, const wchar_t **wpTextNext)
{
  const wchar_t *wpCount;
  wchar_t wchStopChar;

  while (*wpText == L' ' || *wpText == L'\t') ++wpText;

  if (*wpText == L'\'' || *wpText == L'`')
  {
    wchStopChar=*wpText;
    wpCount=++wpText;

    //Parse: 'param' or `param`
    while (*wpCount != wchStopChar && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;
  }
  else if (*wpText == L'\"')
  {
    wpCount=++wpText;

    //Parse: "escape param \"C:\\1.txt\""
    while (*wpCount != L'\"' && *wpCount != L'\r' && *wpCount != L'\0')
    {
      if (*wpCount == L'\\')
      {
        if (*++wpCount == L'\r' || *wpCount == L'\0')
          break;
      }
      ++wpCount;
    }
  }
  else
  {
    wpCount=wpText;

    //Parse: param1 param2 param3
    while (*wpCount != L' ' && *wpCount != L'\t' && *wpCount != L'\r' && *wpCount != L'\0')
      ++wpCount;
  }
  *wpParamStart=wpText;
  *wpParamEnd=wpCount;

  if (*wpCount != L'\r' && *wpCount != L'\0')
    *wpTextNext=wpCount + 1;
  else
    *wpTextNext=wpCount;
  return *wpParamEnd - *wpParamStart;
}

INT_PTR StrToArray(const wchar_t *wpStr, void *lpArray, INT_PTR nArraySize, int nArrayItemSize, const wchar_t **wpNextStr)
{
  const wchar_t *wpCount;
  BYTE *lpCount=lpArray;
  BYTE *lpMaxArray=lpCount + nArraySize;
  INT_PTR nNumber;

  for (wpCount=wpStr; *wpCount && lpCount < lpMaxArray; ++wpCount, lpCount+=nArrayItemSize)
  {
    nNumber=xatoiW(wpCount, &wpCount);
    xmemcpy(lpCount, &nNumber, nArrayItemSize);
    if (*wpCount != L';') break;
  }
  if (wpNextStr) *wpNextStr=wpCount;
  return wpCount - wpStr;
}

BOOL SelectColorDialog(HWND hWndOwner, COLORREF *crColor)
{
  COLORREF crCustColors[16];
  COLORREF crReserve=*crColor;

  xmemset(&crCustColors, 0, sizeof(crCustColors));

  if (bOldWindows)
  {
    CHOOSECOLORA ccA;

    xmemset(&ccA, 0, sizeof(CHOOSECOLORA));
    ccA.lStructSize =sizeof(CHOOSECOLORA);
    ccA.lpCustColors=crCustColors;
    ccA.Flags       =CC_FULLOPEN|CC_RGBINIT;
    ccA.hwndOwner   =hWndOwner;
    ccA.rgbResult   =*crColor;

    if (ChooseColorA(&ccA))
    {
      *crColor=ccA.rgbResult;
      return TRUE;
    }
  }
  else
  {
    CHOOSECOLORW ccW;

    xmemset(&ccW, 0, sizeof(CHOOSECOLORW));
    ccW.lStructSize =sizeof(CHOOSECOLORW);
    ccW.lpCustColors=crCustColors;
    ccW.Flags       =CC_FULLOPEN|CC_RGBINIT;
    ccW.hwndOwner   =hWndOwner;
    ccW.rgbResult   =*crColor;

    if (ChooseColorW(&ccW))
    {
      *crColor=ccW.rgbResult;
      return TRUE;
    }
  }
  *crColor=crReserve;
  return FALSE;
}

void GetPosFromChar(HWND hWnd, INT_PTR nCharIndex, POINT *pt)
{
  DWORD dwPosition;

  if (bOldRichEdit)
  {
    dwPosition=(DWORD)SendMessage(hWnd, EM_POSFROMCHAR, nCharIndex, (LPARAM)NULL);
    pt->x=LOWORD(dwPosition);
    pt->y=HIWORD(dwPosition);
  }
  else SendMessage(hWnd, EM_POSFROMCHAR, (WPARAM)pt, nCharIndex);
}


//// Options

void ReadSyntaxFiles()
{
  WIN32_FIND_DATAW wfd;
  SYNTAXFILE *lpSyntaxFile;
  SYNTAXFILE* lpNextSyntaxFile;
  VARTHEME *lpVarTheme;
  WILDCARDINFO *lpWildElement;
  HINIFILE hIniFile=NULL;
  INISECTION *lpIniSection;
  INIKEY *lpKey;
  HANDLE hSearch;
  wchar_t wszSyntaxFileName[MAX_PATH];
  wchar_t wszVarThemeName[MAX_PATH];
  const wchar_t *wpParamStart;
  const wchar_t *wpCount;
  wchar_t *wpWildcard;
  BOOL bQuoteString;
  int nWildcardLen;
  int nParamCount;
  DWORD a;
  DWORD b;

  //Open cache file
  if (dwUseCache)
  {
    xprintfW(wszBuffer, L"%s\\cache", wszCoderDir);
    hIniFile=(HINIFILE)SendMessage(hMainWnd, AKD_INIOPENW, POB_READ, (LPARAM)wszBuffer);
  }
  bSyntaxFileLoadMute=FALSE;

  if (hIniFile)
  {
    //Load cache info
    if (lpIniSection=(INISECTION *)SendMessage(hMainWnd, AKD_INIGETSECTIONW, (WPARAM)hIniFile, (LPARAM)L"Cache"))
    {
      for (lpKey=lpIniSection->first; lpKey; lpKey=lpKey->next)
      {
        if (lpSyntaxFile=StackAddSyntaxFile(&hSyntaxFilesStack, lpKey->wszKey))
        {
          lpSyntaxFile->bCache=TRUE;

          //Parse line
          //bat.coder=3B9F9A0001CE66CC,1:"*.bat" "*.cmd"
          wpParamStart=lpKey->wszString;
          nParamCount=0;

          for (wpCount=wpParamStart; *wpCount != L'\"'; ++wpCount)
          {
            if (*wpCount == L',' || *wpCount == L':' || *wpCount == L'\0')
            {
              if (nParamCount == 0)
              {
                if (wpCount - wpParamStart == 16)
                {
                  lpSyntaxFile->ftTimeStamp.dwLowDateTime=(DWORD)hex2decW(lpKey->wszString, 8, NULL);
                  lpSyntaxFile->ftTimeStamp.dwHighDateTime=(DWORD)hex2decW(lpKey->wszString + 8, 8, NULL);
                  wpParamStart=wpCount + 1;
                  ++nParamCount;
                }
              }
              else if (nParamCount == 1)
              {
                lpSyntaxFile->bReserveCacheWildcard=(BOOL)xatoiW(wpParamStart, NULL);
                wpParamStart=wpCount + 1;
                ++nParamCount;
              }
              if (*wpCount == L':')
              {
                ++wpCount;
                break;
              }
              if (!*wpCount) break;
            }
          }

          for (;;)
          {
            lpWildElement=NULL;
            wpWildcard=NULL;

            //Wildcard
            if ((nWildcardLen=GetWord(wpCount, wszBuffer, BUFFER_SIZE, &wpCount, &bQuoteString, NULL)) || bQuoteString)
            {
              if (wpWildcard=(wchar_t *)GlobalAlloc(GPTR, (nWildcardLen + 1) * sizeof(wchar_t)))
                xmemcpy(wpWildcard, wszBuffer, (nWildcardLen + 1) * sizeof(wchar_t));
              else
                break;
            }
            else break;

            //Add to stack
            if (lpWildElement=StackInsertWildcard(&lpSyntaxFile->hWildcardStack, nWildcardLen))
            {
              lpWildElement->wpWildcard=wpWildcard;
              lpWildElement->nWildcardLen=nWildcardLen;
            }
          }
        }
      }
    }
    SendMessage(hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);

    if (dwUseCache == UC_SMART)
    {
      //Scan for added/modified/removed *.coder files
      xprintfW(wszBuffer, L"%s\\*.coder", wszCoderDir);
      if ((hSearch=FindFirstFileWide(wszBuffer, &wfd)) != INVALID_HANDLE_VALUE)
      {
        do
        {
          for (lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpSyntaxFile->next)
          {
            if (!xstrcmpiW(wfd.cFileName, lpSyntaxFile->wszSyntaxFileName))
            {
              lpSyntaxFile->bExists=TRUE;

              if (xmemcmp(&wfd.ftLastWriteTime, &lpSyntaxFile->ftTimeStamp, sizeof(FILETIME)))
              {
                //Syntax file was modified
                lpSyntaxFile->bCache=FALSE;
              }
              break;
            }
          }

          if (!lpSyntaxFile)
          {
            if (lpSyntaxFile=StackAddSyntaxFile(&hSyntaxFilesStack, wfd.cFileName))
            {
              //New file, adding to the cache
              lpSyntaxFile->bCache=FALSE;
              lpSyntaxFile->bExists=TRUE;
            }
          }
        }
        while (FindNextFileWide(hSearch, &wfd));

        FindClose(hSearch);
      }

      //Exclude non-existed (removed) files form the cache
      for (lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpNextSyntaxFile)
      {
        lpNextSyntaxFile=lpSyntaxFile->next;
        if (!lpSyntaxFile->bExists)
        {
          StackFreeWildcard(&lpSyntaxFile->hWildcardStack);
          StackDelete((stack **)&hSyntaxFilesStack.first, (stack **)&hSyntaxFilesStack.last, (stack *)lpSyntaxFile);
          dwSaveCache|=SC_SAVE|SC_CLEAR;
        }
      }
    }
  }
  else
  {
    //Scan for *.coder files
    xprintfW(wszBuffer, L"%s\\*.coder", wszCoderDir);
    if ((hSearch=FindFirstFileWide(wszBuffer, &wfd)) != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (lpSyntaxFile=StackPushSortSyntaxFile(&hSyntaxFilesStack, wfd.cFileName, 1))
        {
          lpSyntaxFile->bCache=FALSE;
          dwSaveCache|=SC_SAVE|SC_CLEAR;
        }
      }
      while (FindNextFileWide(hSearch, &wfd));

      FindClose(hSearch);
    }
  }

  //Get var theme link
  if (wszVarThemeLink)
  {
    wszVarThemeName[0]=L'\0';
    wszSyntaxFileName[0]=L'\0';

    for (a=0, b=0; b < BUFFER_SIZE; ++b)
    {
      if (wszVarThemeLink[b] == L'>')
      {
        if (b - a > 0)
        {
          xstrcpynW(wszSyntaxFileName, wszVarThemeLink + a, (b - a) + 1);
          a=b + 1;
        }
      }
      else if (wszVarThemeLink[b] == L'|' || wszVarThemeLink[b] == L'\0')
      {
        if (b - a > 0 && wszSyntaxFileName[0])
        {
          xstrcpynW(wszVarThemeName, wszVarThemeLink + a, (b - a) + 1);
          a=b + 1;

          if (lpSyntaxFile=StackGetSyntaxFileByName(&hSyntaxFilesStack, wszSyntaxFileName))
          {
            if (!(lpVarTheme=RequestVarTheme(&hVarThemesStack, wszVarThemeName)))
            {
              xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_UNKNOWNVARTHEME), wszVarThemeName, wszSyntaxFileName);
              MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
            }
            lpSyntaxFile->lpVarThemeLink=lpVarTheme;
          }
          else
          {
            xprintfW(wszMessage, GetLangStringW(wLangModule, STRID_UNKNOWNSYNTAXFILE), wszVarThemeName, wszSyntaxFileName);
            MessageBoxW(hMainWnd, wszMessage, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
          }
        }
        if (wszVarThemeLink[b] == L'\0')
          break;
        wszVarThemeName[0]=L'\0';
        wszSyntaxFileName[0]=L'\0';
      }
    }
  }

  //Load syntax files
  for (lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpSyntaxFile->next)
  {
    if (!lpSyntaxFile->bCache)
    {
      if (!lpSyntaxFile->bReserveCacheWildcard)
        StackFreeWildcard(&lpSyntaxFile->hWildcardStack);
      StackLoadSyntaxFile(&hSyntaxFilesStack, lpSyntaxFile);
      dwSaveCache|=SC_SAVE;
    }
  }
}

void SaveCache(DWORD dwFlags)
{
  if (dwUseCache && (dwFlags & SC_SAVE))
  {
    SYNTAXFILE *lpSyntaxFile;
    WILDCARDINFO *lpWildElement;
    INIVALUEW iv;
    HINIFILE hIniFile;
    HINISECTION hIniSection;
    wchar_t *wszStr;
    int nStrLen;

    //Open cache file
    xprintfW(wszBuffer, L"%s\\cache", wszCoderDir);
    if (hIniFile=(HINIFILE)SendMessage(hMainWnd, AKD_INIOPENW, POB_SAVE, (LPARAM)wszBuffer))
    {
      //Clear cache
      if (dwFlags & SC_CLEAR)
      {
        if (hIniSection=(HINISECTION)SendMessage(hMainWnd, AKD_INIGETSECTIONW, (WPARAM)hIniFile, (LPARAM)L"Cache"))
          SendMessage(hMainWnd, AKD_INICLEARSECTION, (WPARAM)hIniFile, (LPARAM)hIniSection);
      }

      //Scan themes
      for (lpSyntaxFile=hSyntaxFilesStack.first; lpSyntaxFile; lpSyntaxFile=lpSyntaxFile->next)
      {
        //Get file types
        for (nStrLen=0, lpWildElement=lpSyntaxFile->hWildcardStack.first; lpWildElement; lpWildElement=lpWildElement->next)
        {
          nStrLen+=(int)xprintfW(NULL, L"\"%s\" ", lpWildElement->wpWildcard) - 1;
        }
        if (wszStr=(wchar_t *)GlobalAlloc(GMEM_FIXED, (30 + nStrLen + 2) * sizeof(wchar_t)))
        {
          nStrLen=(int)xprintfW(wszStr, L"%08X%08X%s:", lpSyntaxFile->ftTimeStamp.dwLowDateTime, lpSyntaxFile->ftTimeStamp.dwHighDateTime, lpSyntaxFile->bReserveCacheWildcard?L",1":L"");

          for (lpWildElement=lpSyntaxFile->hWildcardStack.first; lpWildElement; lpWildElement=lpWildElement->next)
          {
            nStrLen+=(int)xprintfW(wszStr + nStrLen, L"\"%s\" ", lpWildElement->wpWildcard);
          }
          if (nStrLen) --nStrLen;
          wszStr[nStrLen]=L'\0';
        }

        //Save extentions
        iv.pSection=L"Cache";
        iv.pKey=lpSyntaxFile->wszSyntaxFileName;
        iv.dwType=INI_STRINGUNICODE;
        iv.lpData=(LPBYTE)wszStr;
        iv.dwData=(nStrLen + 1) * sizeof(wchar_t);
        SendMessage(hMainWnd, AKD_INISETVALUEW, (WPARAM)hIniFile, (LPARAM)&iv);

        GlobalFree((HGLOBAL)wszStr);
      }
      SendMessage(hMainWnd, AKD_INICLOSE, (WPARAM)hIniFile, 0);
    }
  }
  dwSaveCache=0;
}

void ClearCache(BOOL bForceNewCache)
{
  if (nInitMain)
  {
    HWND hWndCurEdit=GetCurEdit();
    DWORD dwTmp=UC_NONE;

    //Free syntax files
    StackFreeSyntaxFiles(&hSyntaxFilesStack);

    if (bInitHighLight)
    {
      StackFreeHighLightWindow(&hHighLightWindowsStack);
      bUpdateTheme=TRUE;
    }
    if (nInitCodeFold)
    {
      StackEndBoard(&hFoldWindowsStack, NULL);
      StackFreeFoldWindows(&hFoldWindowsStack);
    }
    if (bInitAutoComplete)
    {
      lpSyntaxFileAutoComplete=NULL;
    }

    //Read syntax files
    if (bForceNewCache)
    {
      dwTmp=dwUseCache;
      dwUseCache=UC_NONE;
    }
    ReadOptions(OF_HIGHLIGHT|OF_CODEFOLD|OF_AUTOCOMPLETE);
    ReadSyntaxFiles();
    if (bForceNewCache)
    {
      dwUseCache=dwTmp;
    }

    //Update edit rectangle
    UpdateEditAll(UE_DRAWRECT|UE_UPDATEWINDOW);
    if (hWndCurEdit)
      SendMessage(hWndCurEdit, AEM_UPDATESEL, AESELT_LOCKSCROLL|AESELT_COLUMNASIS, 0);

    if (nInitCodeFold)
    {
      if (hWndCurEdit)
        lpCurrentFoldWindow=SetActiveEdit(hWndCurEdit, hWndCodeFoldList, SAE_RESETLIST);
    }
  }
}

void UpdateAllOptions()
{
  GetVarThemeGlobals(lpVarThemeActive);
  SaveOptions(OF_CLEAR|OF_GENERAL|OF_HIGHLIGHT|OF_CODEFOLD|OF_AUTOCOMPLETE);
  SaveCache(SC_SAVE|SC_CLEAR);
  ClearCache(FALSE);
}

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData)
{
  PLUGINOPTIONW po;

  po.pOptionName=pOptionName;
  po.dwType=dwType;
  po.lpData=lpData;
  po.dwData=dwData;
  return SendMessage(hMainWnd, AKD_OPTIONW, (WPARAM)hOptions, (LPARAM)&po);
}

void ReadOptions(DWORD dwFlags)
{
  HANDLE hOptions;
  VARTHEME *lpVarTheme;
  wchar_t wszVarThemeName[MAX_PATH];
  wchar_t *wszVarThemeValue=NULL;
  int nSize;
  int a;
  int b;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_GENERAL_ALLTHEMES)
    {
      //Variable themes
      if (WideOption(hOptions, L"VarThemeList", PO_STRING, (LPBYTE)wszBuffer, BUFFER_SIZE * sizeof(wchar_t)))
      {
        for (a=0, b=0; ; ++b)
        {
          if (wszBuffer[b] == L'|' || wszBuffer[b] == L'\0')
          {
            if (b - a > 0)
            {
              wszVarThemeName[0]=L'/';
              xstrcpynW(wszVarThemeName + 1, wszBuffer + a, (b - a) + 1);

              if ((nSize=(int)WideOption(hOptions, wszVarThemeName, PO_BINARY, (LPBYTE)NULL, 0)) > 0)
              {
                if (wszVarThemeValue=(wchar_t *)GlobalAlloc(GPTR, nSize + sizeof(wchar_t)))
                {
                  WideOption(hOptions, wszVarThemeName, PO_BINARY, (LPBYTE)wszVarThemeValue, nSize);
                  wszVarThemeValue[nSize / sizeof(wchar_t)]=L'\0';

                  if (lpVarTheme=StackInsertVarTheme(&hVarThemesStack, -1))
                  {
                    lpVarTheme->nVarThemeNameLen=(int)xstrcpynW(lpVarTheme->wszVarThemeName, wszVarThemeName + 1, MAX_PATH);
                    ParseStringToVars(&lpVarTheme->hVarStack, wszVarThemeValue);
                  }
                  GlobalFree((HGLOBAL)wszVarThemeValue);
                }
              }
              a=b + 1;
            }
            if (wszBuffer[b] == L'\0')
              break;
          }
        }
      }
    }
    if ((dwFlags & OF_GENERAL_ACTIVETHEME) || (dwFlags & OF_GENERAL_ALLTHEMES))
    {
      //Read active variable theme data
      if (WideOption(hOptions, L"VarThemeActive", PO_STRING, (LPBYTE)wszVarThemeName, MAX_PATH * sizeof(wchar_t)))
        lpVarThemeActive=RequestVarTheme(&hVarThemesStack, wszVarThemeName);
    }
    if ((dwFlags & OF_GENERAL_GLOBALTHEME) || (dwFlags & OF_GENERAL_ALLTHEMES))
    {
      if ((nSize=(int)WideOption(hOptions, L"VarThemeGlobal", PO_BINARY, (LPBYTE)NULL, 0)) > 0)
      {
        if (wszVarThemeValue=(wchar_t *)GlobalAlloc(GPTR, nSize + sizeof(wchar_t)))
        {
          WideOption(hOptions, L"VarThemeGlobal", PO_BINARY, (LPBYTE)wszVarThemeValue, nSize);
          wszVarThemeValue[nSize / sizeof(wchar_t)]=L'\0';

          ParseStringToVars(&hVarThemeGlobal.hVarStack, wszVarThemeValue);
          GlobalFree((HGLOBAL)wszVarThemeValue);
        }
      }
    }
    if (dwFlags & OF_GENERAL_THEMELINK)
    {
      if ((nSize=(int)WideOption(hOptions, L"VarThemeLink", PO_STRING, (LPBYTE)NULL, 0)) > 0)
      {
        if (wszVarThemeLink=(wchar_t *)GlobalAlloc(GMEM_FIXED, nSize))
          WideOption(hOptions, L"VarThemeLink", PO_STRING, (LPBYTE)wszVarThemeLink, nSize);
      }
    }
    if (dwFlags & OF_GENERAL_SETTINGS)
    {
      WideOption(hOptions, L"UseCache", PO_DWORD, (LPBYTE)&dwUseCache, sizeof(DWORD));
      WideOption(hOptions, L"DefaultAliasEnable", PO_DWORD, (LPBYTE)&bDefaultAliasEnable, sizeof(DWORD));
      WideOption(hOptions, L"DefaultAlias", PO_STRING, (LPBYTE)wszDefaultAlias, MAX_PATH * sizeof(wchar_t));
    }
    if (dwFlags & OF_HIGHLIGHT)
      ReadHighLightOptions(hOptions);
    if (dwFlags & OF_CODEFOLD)
      ReadCodeFoldOptions(hOptions);
    if (dwFlags & OF_AUTOCOMPLETE)
      ReadAutoCompleteOptions(hOptions);

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;
  VARTHEME *lpVarTheme;
  wchar_t wszVarThemeName[MAX_PATH];
  wchar_t *wszVarThemeValue;
  DWORD dwListLen;
  DWORD dwSize;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE|((dwFlags & OF_CLEAR)?POB_CLEAR:0), (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_GENERAL_ALLTHEMES)
    {
      //Variable themes
      wszBuffer[0]=L'\0';
      lpVarTheme=StackGetVarThemeByIndex(&hVarThemesStack, nVarThemesInternalCount + 1);

      for (dwListLen=0; lpVarTheme; lpVarTheme=lpVarTheme->next)
      {
        //VarThemeList
        dwListLen+=(DWORD)xprintfW(wszBuffer + dwListLen, L"%s%s", lpVarTheme->wszVarThemeName, lpVarTheme->next?L"|":L"");

        //Variable theme data
        dwSize=ParseVarsToString(&lpVarTheme->hVarStack, &wszVarThemeValue);
        xprintfW(wszVarThemeName, L"/%s", lpVarTheme->wszVarThemeName);
        WideOption(hOptions, wszVarThemeName, PO_BINARY, (LPBYTE)wszVarThemeValue, dwSize * sizeof(wchar_t));
        GlobalFree((HGLOBAL)wszVarThemeValue);
      }
      WideOption(hOptions, L"VarThemeList", PO_STRING, (LPBYTE)wszBuffer, (dwListLen + 1) * sizeof(wchar_t));
    }
    if ((dwFlags & OF_GENERAL_ACTIVETHEME) || (dwFlags & OF_GENERAL_ALLTHEMES))
    {
      if (lpVarThemeActive)
        WideOption(hOptions, L"VarThemeActive", PO_STRING, (LPBYTE)lpVarThemeActive->wszVarThemeName, ((int)xstrlenW(lpVarThemeActive->wszVarThemeName) + 1) * sizeof(wchar_t));
    }
    if ((dwFlags & OF_GENERAL_GLOBALTHEME) || (dwFlags & OF_GENERAL_ALLTHEMES))
    {
      dwSize=ParseVarsToString(&hVarThemeGlobal.hVarStack, &wszVarThemeValue);
      WideOption(hOptions, L"VarThemeGlobal", PO_BINARY, (LPBYTE)wszVarThemeValue, dwSize * sizeof(wchar_t));
      GlobalFree((HGLOBAL)wszVarThemeValue);
    }
    if (dwFlags & OF_GENERAL_THEMELINK)
    {
      if (wszVarThemeLink)
        WideOption(hOptions, L"VarThemeLink", PO_STRING, (LPBYTE)wszVarThemeLink, ((int)xstrlenW(wszVarThemeLink) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_GENERAL_SETTINGS)
    {
      WideOption(hOptions, L"UseCache", PO_DWORD, (LPBYTE)&dwUseCache, sizeof(DWORD));
      WideOption(hOptions, L"DefaultAliasEnable", PO_DWORD, (LPBYTE)&bDefaultAliasEnable, sizeof(DWORD));
      WideOption(hOptions, L"DefaultAlias", PO_STRING, (LPBYTE)wszDefaultAlias, ((int)xstrlenW(wszDefaultAlias) + 1) * sizeof(wchar_t));
    }

    SaveHighLightOptions(hOptions, dwFlags);
    SaveCodeFoldOptions(hOptions, dwFlags);
    SaveAutoCompleteOptions(hOptions, dwFlags);

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

const char* GetLangStringA(LANGID wLangID, int nStringID)
{
  static char szStringBuf[MAX_PATH];

  WideCharToMultiByte(CP_ACP, 0, GetLangStringW(wLangID, nStringID), -1, szStringBuf, MAX_PATH, NULL, NULL);
  return szStringBuf;
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    //Common
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
    if (nStringID == STRID_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C";
    if (nStringID == STRID_ENABLE)
      return L"\x0412\x043A\x043B\x044E\x0447\x0435\x043D\x043E";

    //Coder
    if (nStringID == STRID_GENERAL)
      return L"\x041E\x0431\x0449\x0438\x0435";
    if (nStringID == STRID_CODER_NOTRUNNING)
      return L"\x0043\x006F\x0064\x0065\x0072\x0020\x043F\x043B\x0430\x0433\x0438\x043D\x0020\x043D\x0435\x0020\x0437\x0430\x043F\x0443\x0449\x0435\x043D.";
    if (nStringID == STRID_LOADFIRST)
      return L"\x0417\x0430\x0433\x0440\x0443\x0437\x0438\x0442\x0435\x0020\x0441\x043F\x0435\x0440\x0432\x0430 %s.";
    if (nStringID == STRID_CACHE_NONE)
      return L"\x0411\x0435\x0437\x0020\x043A\x0435\x0448\x0430";
    if (nStringID == STRID_CACHE_FAST)
      return L"\x0411\x044B\x0441\x0442\x0440\x044B\x0439\x0020\x043A\x0435\x0448";
    if (nStringID == STRID_CACHE_SMART)
      return L"\x0423\x043C\x043D\x044B\x0439\x0020\x043A\x0435\x0448";
    if (nStringID == STRID_DEFAULTALIAS_ENABLE)
      return L"\x041F\x0441\x0435\x0432\x0434\x043E\x043D\x0438\x043C\x0020\x0434\x043B\x044F\x0020\x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x043E\x0433\x043E\x0020\x0444\x0430\x0439\x043B\x0430 ";
    if (nStringID == STRID_VARTHEME_NAME)
      return L"\x0422\x0435\x043C\x0430:";
    if (nStringID == STRID_VARTHEME_SAVE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_VARTHEME_DELETE)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_VARTHEME_RETURN)
      return L"\x0412\x0435\x0440\x043D\x0443\x0442\x044C\x0441\x044F";
    if (nStringID == STRID_VARTHEME_GLOBAL)
      return L"\x0413\x043B\x043E\x0431\x0430\x043B\x044C\x043D\x044B\x0435\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x043D\x043D\x044B\x0435";
    if (nStringID == STRID_VARTHEME_LINK)
      return L"\x0421\x0432\x044F\x0437\x044C...";
    if (nStringID == STRID_MENU_ADDITEM)
      return L"\x0414\x043E\x0431\x0430\x0432\x0438\x0442\x044C...\tInsert";
    if (nStringID == STRID_MENU_MODITEM)
      return L"\x0418\x0437\x043C\x0435\x043D\x0438\x0442\x044C...\tF2";
    if (nStringID == STRID_MENU_MOVEUPITEM)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x0432\x0435\x0440\x0445\tAlt+Up";
    if (nStringID == STRID_MENU_MOVEDOWNITEM)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0432\x043D\x0438\x0437\tAlt+Down";
    if (nStringID == STRID_MENU_DELITEM)
      return L"\x0423\x0434\x0430\x043B\x0438\x0442\x044C\tDelete";
    if (nStringID == STRID_MENU_CLEARITEM)
      return L"\x041E\x0447\x0438\x0441\x0442\x0438\x0442\x044C";
    if (nStringID == STRID_VARIABLE)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x043D\x043D\x0430\x044F";
    if (nStringID == STRID_VALUE)
      return L"\x0417\x043D\x0430\x0447\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_MAKEGLOBAL)
      return L"\x0421\x0434\x0435\x043B\x0430\x0442\x044C\x0020\x0433\x043B\x043E\x0431\x0430\x043B\x044C\x043D\x043E\x0439";
    if (nStringID == STRID_LOWPRIORITY)
      return L"\x041D\x0438\x0437\x043A\x0438\x0439\x0020\x043F\x0440\x0438\x043E\x0440\x0438\x0442\x0435\x0442";
    if (nStringID == STRID_COLOR)
      return L"\x0426\x0432\x0435\x0442";
    if (nStringID == STRID_SYNTAXFILE)
      return L"\x0424\x0430\x0439\x043B\x0020\x0441\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0441\x0430";
    if (nStringID == STRID_VARTHEMENAME)
      return L"\x0422\x0435\x043C\x0430";
    if (nStringID == STRID_ADDVAR)
      return L"\x0414\x043E\x0431\x0430\x0432\x043B\x0435\x043D\x0438\x0435\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x043D\x043D\x043E\x0439";
    if (nStringID == STRID_EDITVAR)
      return L"\x0418\x0437\x043C\x0435\x043D\x0435\x043D\x0438\x0435\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x043D\x043D\x043E\x0439";
    if (nStringID == STRID_LOSTPROMPT)
      return L"\x0418\x0437\x043C\x0435\x043D\x0435\x043D\x0438\x044F\x0020\x0432\x0020\x0442\x0435\x043C\x0435 \"%s\" \x0431\x0443\x0434\x0443\x0442\x0020\x043F\x043E\x0442\x0435\x0440\x044F\x043D\x044B\x002E\x0020\x041F\x0440\x043E\x0434\x043E\x043B\x0436\x0438\x0442\x044C\x003F";
    if (nStringID == STRID_OVERWRITEPROMPT)
      return L"\"%s\" \x0443\x0436\x0435\x0020\x0441\x0443\x0449\x0435\x0441\x0442\x0432\x0443\x0435\x0442\x002E\x0020\x041F\x0435\x0440\x0435\x0437\x0430\x043F\x0438\x0441\x0430\x0442\x044C\x003F";
    if (nStringID == STRID_DELETEPROMPT)
      return L"\x0412\x044B\x0020\x0443\x0432\x0435\x0440\x0435\x043D\x044B\x002C\x0020\x0447\x0442\x043E\x0020\x0445\x043E\x0442\x0438\x0442\x0435\x0020\x0443\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x0442\x0435\x043C\x0443 \"%s\"?";
    if (nStringID == STRID_VARMISSING)
      return L"\"%s\" \x0441\x043E\x0434\x0435\x0440\x0436\x0438\x0442\x0020\x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x0443\x044E\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x043D\x043D\x0443\x044E \"%.%ds\", \x043A\x043E\x0442\x043E\x0440\x0430\x044F\x0020\x043E\x0442\x0441\x0443\x0442\x0441\x0442\x0432\x0443\x0435\x0442\x0020\x0432\x0020\x0430\x043A\x0442\x0438\x0432\x043D\x043E\x0439\x0020\x0442\x0435\x043C\x0435 \"%s\". \x041F\x0440\x043E\x0434\x043E\x043B\x0436\x0438\x0442\x044C\x003F";
    if (nStringID == STRID_REGEXP_COMPILEERROR)
      return L"\"%s\" \x0441\x043E\x0434\x0435\x0440\x0436\x0438\x0442\x0020\x043E\x0448\x0438\x0431\x043A\x0443\x0020\x0432\x0020\x0440\x0435\x0433\x0443\x043B\x044F\x0440\x043D\x043E\x043C\x0020\x0432\x044B\x0440\x0430\x0436\x0435\x043D\x0438\x0438 \"%.%ds\"";
    if (nStringID == STRID_UNKNOWNSECTION)
      return L"\"%s\" (\x0441\x043C\x0435\x0449\x0435\x043D\x0438\x0435 %Id) \x0441\x043E\x0434\x0435\x0440\x0436\x0438\x0442\x0020\x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x043E\x0435\x0020\x0438\x043C\x044F\x0020\x0441\x0435\x043A\x0446\x0438\x0438 \"%s\".";
    if (nStringID == STRID_UNKNOWNSYNTAXFILE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0430\x0441\x0441\x043E\x0446\x0438\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x0442\x0435\x043C\x0443 \"%s\" \x0441\x0020\x0444\x0430\x0439\x043B\x043E\x043C \"%s\": \x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x044B\x0439\x0020\x0441\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0447\x0435\x0441\x043A\x0438\x0439\x0020\x0444\x0430\x0439\x043B.";
    if (nStringID == STRID_UNKNOWNVARTHEME)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0430\x0441\x0441\x043E\x0446\x0438\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x0442\x0435\x043C\x0443 \"%s\" \x0441\x0020\x0444\x0430\x0439\x043B\x043E\x043C \"%s\": \x043D\x0435\x0438\x0437\x0432\x0435\x0441\x0442\x043D\x0430\x044F\x0020\x0442\x0435\x043C\x0430.";

    //HighLight
    if (nStringID == STRID_IGNORE_FONTSTYLE_GROUP)
      return L"\x0418\x0433\x043D\x043E\x0440\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x0441\x0442\x0438\x043B\x044C\x0020\x0448\x0440\x0438\x0444\x0442\x0430";
    if (nStringID == STRID_NORMAL)
      return L"\x041E\x0431\x044B\x0447\x043D\x044B\x0439";
    if (nStringID == STRID_BOLD)
      return L"\x0416\x0438\x0440\x043D\x044B\x0439";
    if (nStringID == STRID_ITALIC)
      return L"\x041A\x0443\x0440\x0441\x0438\x0432";
    if (nStringID == STRID_AUTOMARK_GROUP)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0438\x0020\x043E\x0442\x043C\x0435\x0447\x0430\x0442\x044C\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x043E\x0435";
    if (nStringID == STRID_SYMBOLS)
      return L"\x0421\x0438\x043C\x0432\x043E\x043B\x044B";
    if (nStringID == STRID_WORDS)
      return L"\x0421\x043B\x043E\x0432\x0430";
    if (nStringID == STRID_MAXSEL)
      return L"\x041B\x0438\x043C\x0438\x0442\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x0438\x044F:";
    if (nStringID == STRID_TEXTCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0442\x0435\x043A\x0441\x0442\x0430\x0020\x0028\x0052\x0052\x0047\x0047\x0042\x0042\x0029\x003A";
    if (nStringID == STRID_BGCOLOR)
      return L"\x0426\x0432\x0435\x0442\x0020\x0444\x043E\x043D\x0430\x0020\x0028\x0052\x0052\x0047\x0047\x0042\x0042\x0029\x003A";

    //CodeFold
    if (nStringID == STRID_RICHEDIT_NOTSUPPORTED)
      return L"\x0041\x006B\x0065\x006C\x0050\x0061\x0064\x0020\x0033\x002E\x0078\x002E\x0078\x0020\x043D\x0435\x0020\x043F\x043E\x0434\x0434\x0435\x0440\x0436\x0438\x0432\x0430\x0435\x0442\x0441\x044F\x002E";
    if (nStringID == STRID_GOBEGIN)
      return L"\x041F\x0435\x0440\x0435\x0439\x0442\x0438\x0020\x043A\x0020\x043D\x0430\x0447\x0430\x043B\x0443";
    if (nStringID == STRID_GOEND)
      return L"\x041F\x0435\x0440\x0435\x0439\x0442\x0438\x0020\x043A\x0020\x043A\x043E\x043D\x0446\x0443";
    if (nStringID == STRID_SELECT)
      return L"\x0412\x044B\x0434\x0435\x043B\x0438\x0442\x044C";
    if (nStringID == STRID_INVIEW)
      return L"\x041F\x043E\x043A\x0430\x0437\x0430\x0442\x044C";
    if (nStringID == STRID_LISTEXPAND)
      return L"\x0420\x0430\x0437\x0432\x0435\x0440\x043D\x0443\x0442\x044C\x0020\x0441\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_LISTCOLLAPSE)
      return L"\x0421\x0432\x0435\x0440\x043D\x0443\x0442\x044C\x0020\x0441\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_LISTCOPY)
      return L"\x041A\x043E\x043F\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x0441\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_STATISTICS)
      return L"\x0421\x0442\x0430\x0442\x0438\x0441\x0442\x0438\x043A\x0430";
    if (nStringID == STRID_STATISTICS_MSG)
      return L"\x0412\x0441\x0435\x0433\x043E\x0020\x0431\x043B\x043E\x043A\x043E\x0432\x003A %d\n    \x041A\x043E\x0440\x043D\x0435\x0432\x044B\x0445\x0020\x0431\x043B\x043E\x043A\x043E\x0432\x003A %d\n    \x0414\x043E\x0447\x0435\x0440\x043D\x0438\x0445\x0020\x0431\x043B\x043E\x043A\x043E\x0432\x003A %d\n\x0414\x043E\x0447\x0435\x0440\x043D\x0438\x0445\x0020\x0431\x043B\x043E\x043A\x043E\x0432\x0020\x0442\x0435\x043A\x0443\x0449\x0435\x0433\x043E\x0020\x0431\x043B\x043E\x043A\x0430\x003A %d\n";
    if (nStringID == STRID_GORULE)
      return L"\x041F\x0435\x0440\x0435\x0439\x0442\x0438\x0020\x043A\x0020\x043F\x0440\x0430\x0432\x0438\x043B\x0443";
    if (nStringID == STRID_SETUP)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_SHOWDOCK_GROUP)
      return L"\x041F\x043E\x043A\x0430\x0437\x044B\x0432\x0430\x0442\x044C\x0020\x0441\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_AUTO)
      return L"\x0410\x0432\x0442\x043E";
    if (nStringID == STRID_ALWAYS)
      return L"\x0412\x0441\x0435\x0433\x0434\x0430";
    if (nStringID == STRID_ASLIST)
      return L"\x041A\x0430\x043A\x0020\x0441\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_NONE)
      return L"\x041D\x0435\x0442";
    if (nStringID == STRID_FOLDLIMIT)
      return L"\x041B\x0438\x043C\x0438\x0442\x0020\x0431\x043B\x043E\x043A\x043E\x0432\x003A";
    if (nStringID == STRID_FOLLOWCARET_GROUP)
      return L"\x0421\x043B\x0435\x0434\x043E\x0432\x0430\x0442\x044C\x0020\x0437\x0430\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x043E\x0439";
    if (nStringID == STRID_ANYWHERE)
      return L"\x0412\x0435\x0437\x0434\x0435";
    if (nStringID == STRID_ROOT)
      return L"\x041F\x043E\x0020\x043A\x043E\x0440\x043D\x044E";
    if (nStringID == STRID_SHOWNODES_GROUP)
      return L"\x041F\x043E\x043A\x0430\x0437\x044B\x0432\x0430\x0442\x044C\x0020\x0443\x0437\x043B\x044B";
    if (nStringID == STRID_DRAWNODE)
      return L"\x0420\x0438\x0441\x043E\x0432\x0430\x0442\x044C\x0020\x0443\x0437\x0435\x043B\x0020\x043A\x0430\x043A";
    if (nStringID == STRID_ROUND)
      return L"\x041A\x0440\x0443\x0433";
    if (nStringID == STRID_SQUARE)
      return L"\x041A\x0432\x0430\x0434\x0440\x0430\x0442";
    if (nStringID == STRID_TAGMARK)
      return L"\x041E\x0442\x043C\x0435\x0447\x0430\x0442\x044C\x0020\x0442\x0435\x0433\x0438";
    if (nStringID == STRID_COLLAPSEONOPEN)
      return L"\x0421\x0432\x043E\x0440\x0430\x0447\x0438\x0432\x0430\x0442\x044C\x0020\x0432\x0441\x0435\x0020\x043F\x0440\x0438\x0020\x043E\x0442\x043A\x0440\x044B\x0442\x0438\x0438";
    if (nStringID == STRID_NOPRINTCOLLAPSED)
      return L"\x041D\x0435\x0020\x043F\x0435\x0447\x0430\x0442\x0430\x0442\x044C\x0020\x0441\x0432\x0435\x0440\x043D\x0443\x0442\x044B\x0435\x0020\x0441\x0442\x0440\x043E\x043A\x0438";
    if (nStringID == STRID_HIDEFOLDEND)
      return L"\x0421\x043A\x0440\x044B\x0432\x0430\x0442\x044C\x0020\x043F\x043E\x0441\x043B\x0435\x0434\x043D\x044E\x044E\x0020\x0441\x0442\x0440\x043E\x043A\x0443\x0020\x0431\x043B\x043E\x043A\x0430";
    if (nStringID == STRID_FINDROOT)
      return L"\x041D\x0430\x0439\x0442\x0438\x0020\x043A\x043E\x0440\x0435\x043D\x044C";
    if (nStringID == STRID_HOTKEY)
      return L"\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_DEPTH)
      return L"\x0413\x043B\x0443\x0431\x0438\x043D\x0430";
    if (nStringID == STRID_MATCHCASE)
      return L"\x0423\x0447\x0438\x0442\x044B\x0432\x0430\x0442\x044C\x0020\x0440\x0435\x0433\x0438\x0441\x0442\x0440";
    if (nStringID == STRID_WHOLEWORD)
      return L"\x0421\x043B\x043E\x0432\x043E\x0020\x0446\x0435\x043B\x0438\x043A\x043E\x043C";
    if (nStringID == STRID_HOTKEYCURRENT)
      return L"\x0422\x0435\x043A\x0443\x0449\x0438\x0439\x0020\x0443\x0440\x043E\x0432\x0435\x043D\x044C";
    if (nStringID == STRID_HOTKEYALL)
      return L"\x0412\x0441\x0435\x0020\x0443\x0440\x043E\x0432\x043D\x0438";
    if (nStringID == STRID_COLLAPSE)
      return L"\x0421\x0432\x0435\x0440\x043D\x0443\x0442\x044C";
    if (nStringID == STRID_NEXTLEVEL)
      return L"\x041A\x0020\x0441\x043B\x0435\x0434\x0443\x044E\x0449\x0435\x043C\x0443";
    if (nStringID == STRID_PREVLEVEL)
      return L"\x041A\x0020\x043F\x0440\x0435\x0434\x044B\x0434\x0443\x0449\x0435\x043C\x0443";

    //AutoComplete
    if (nStringID == STRID_AUTOLIST)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0438\x0439\x0020\x0441\x043F\x0438\x0441\x043E\x043A";
    if (nStringID == STRID_AUTOLIST_PRE)
      return L"\x041F\x043E\x043A\x0430\x0437\x0430\x0442\x044C\x0020\x043F\x043E\x0441\x043B\x0435";
    if (nStringID == STRID_AFTERCHAR_POST)
      return L"\x0441\x0438\x043C\x0432\x043E\x043B\x043E\x0432";
    if (nStringID == STRID_TRANSPARENCY)
      return L"\x041F\x0440\x043E\x0437\x0440\x0430\x0447\x043D\x043E\x0441\x0442\x044C";
    if (nStringID == STRID_ALPHA)
      return L"\x0410\x043B\x044C\x0444\x0430";
    if (nStringID == STRID_HOTKEYS)
      return L"\x0413\x043E\x0440\x044F\x0447\x0438\x0435\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438";
    if (nStringID == STRID_COMPLETEWITHLIST)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x0438\x0442\x044C\x0020\x0441\x043E\x0020\x0441\x043F\x0438\x0441\x043A\x043E\x043C";
    if (nStringID == STRID_COMPLETEWITHOUTLIST)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x0438\x0442\x044C\x0020\x0431\x0435\x0437\x0020\x0441\x043F\x0438\x0441\x043A\x0430";
    if (nStringID == STRID_COMPLETEWITHOUTLIST_PRE)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x0438\x0442\x044C\x0020\x0431\x0435\x0437\x0020\x0441\x043F\x0438\x0441\x043A\x0430\x0020\x043F\x043E\x0441\x043B\x0435";
    if (nStringID == STRID_COMPLETENEXT)
      return L"\x041A\x0020\x0441\x043B\x0435\x0434\x0443\x044E\x0449\x0435\x0439\x0020\x043F\x043E\x0437\x0438\x0446\x0438\x0438";
    if (nStringID == STRID_COMPLETEPREV)
      return L"\x041A\x0020\x043F\x0440\x0435\x0434\x044B\x0434\x0443\x044E\x0449\x0435\x0439\x0020\x043F\x043E\x0437\x0438\x0446\x0438\x0438";
    if (nStringID == STRID_DOCUMENT)
      return L"\x0414\x043E\x043A\x0443\x043C\x0435\x043D\x0442";
    if (nStringID == STRID_ADDDOCUMENTWORDS)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x044F\x0442\x044C\x0020\x0441\x043B\x043E\x0432\x0430\x043C\x0438\x0020\x0438\x0437\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442\x0430\x0020\x0028\x043E\x0442\x043C\x0435\x0447\x0435\x043D\x044B\x0020\x043F\x043B\x044E\x0441\x043E\x043C\x0020\x002B\x0029";
    if (nStringID == STRID_MAXDOCUMENT)
      return L"\x041C\x0430\x043A\x0441\x0438\x043C\x0430\x043B\x044C\x043D\x044B\x0439\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442";
    if (nStringID == STRID_CHARS)
      return L"\x0441\x0438\x043C\x0432\x043E\x043B\x043E\x0432";
    if (nStringID == STRID_LISTSYSTEMCOLORS)
      return L"\x0421\x0438\x0441\x0442\x0435\x043C\x043D\x044B\x0435\x0020\x0446\x0432\x0435\x0442\x0430\x0020\x0432\x0020\x0441\x043F\x0438\x0441\x043A\x0435";
    if (nStringID == STRID_LISTSYSTEMFONT)
      return L"\x0421\x0438\x0441\x0442\x0435\x043C\x043D\x044B\x0439\x0020\x0448\x0440\x0438\x0444\x0442\x0020\x0432\x0020\x0441\x043F\x0438\x0441\x043A\x0435";
    if (nStringID == STRID_COMPLETENONSYNTAXDOCUMENT)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x044F\x0442\x044C\x0020\x0442\x0430\x043A\x0436\x0435\x0020\x0438\x0437\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442\x0430\x0020\x0431\x0435\x0437\x0020\x0441\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0447\x0435\x0441\x043A\x043E\x0439\x0020\x0442\x0435\x043C\x044B";
    if (nStringID == STRID_SAVETYPEDCASE)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x044F\x0442\x044C\x0020\x0440\x0435\x0433\x0438\x0441\x0442\x0440\x0020\x0432\x0432\x0435\x0434\x0451\x043D\x043D\x044B\x0445\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x043E\x0432";
    if (nStringID == STRID_INHERITTYPEDCASE)
      return L"\x041D\x0430\x0441\x043B\x0435\x0434\x043E\x0432\x0430\x0442\x044C\x0020\x0440\x0435\x0433\x0438\x0441\x0442\x0440\x0020\x0432\x0432\x0435\x0434\x0451\x043D\x043D\x044B\x0445\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x043E\x0432";
    if (nStringID == STRID_ADDHIGHLIGHTWORDS)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x044F\x0442\x044C\x0020\x0441\x043B\x043E\x0432\x0430\x043C\x0438\x0020\x0438\x0437\x0020\x0431\x0430\x0437\x044B\x0020\x0048\x0069\x0067\x0068\x004C\x0069\x0067\x0068\x0074\x0027\x0430\x0020\x0028\x043E\x0442\x043C\x0435\x0447\x0435\x043D\x044B\x0020\x0437\x0432\x0435\x0437\x0434\x043E\x0447\x043A\x043E\x0439\x0020\x002A\x0029";
    if (nStringID == STRID_HLBASECOLORS)
      return L"\x0418\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020\x0446\x0432\x0435\x0442\x0020\x0434\x043B\x044F\x0020\x043F\x0443\x043D\x043A\x0442\x0430\x0020\x0438\x0437\x0020\x0431\x0430\x0437\x044B\x0020\x0048\x0069\x0067\x0068\x004C\x0069\x0067\x0068\x0074\x0027\x0430";
    if (nStringID == STRID_NOSYMBOLMARK)
      return L"\x0411\x0435\x0437\x0020\x043E\x0442\x043C\x0435\x0442\x043A\x0438\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x043E\x043C\x0020\x0028\x002B\x002A\x0029\x002C\x0020\x0435\x0441\x043B\x0438\x0020\x0438\x0441\x043F\x043E\x043B\x044C\x0437\x0443\x0435\x0442\x0441\x044F\x0020\x0438\x043A\x043E\x043D\x043A\x0430\x0020\x0432\x0020\x0441\x043F\x0438\x0441\x043A\x0435";
    if (nStringID == STRID_RIGHTDELIMITERS)
      return L"\x0423\x0447\x0438\x0442\x044B\x0432\x0430\x0442\x044C\x0020\x0440\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x0435\x043B\x0438\x0020\x0441\x043F\x0440\x0430\x0432\x0430\x0020\x043E\x0442\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x0438";
    if (nStringID == STRID_SYNTAXDELIMITERS)
      return L"\x0418\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020\x0440\x0430\x0437\x0434\x0435\x043B\x0438\x0442\x0435\x043B\x0438\x0020\x0438\x0437\x0020\x0441\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0447\x0435\x0441\x043A\x043E\x0439\x0020\x0442\x0435\x043C\x044B";
    if (nStringID == STRID_COMPLETECASESENSITIVE)
      return L"\x0414\x043E\x043F\x043E\x043B\x043D\x044F\x0442\x044C\x0020\x0441\x0020\x0443\x0447\x0451\x0442\x043E\x043C\x0020\x0440\x0435\x0433\x0438\x0441\x0442\x0440\x0430";
  }
  else
  {
    //Common
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
    if (nStringID == STRID_CLOSE)
      return L"Close";
    if (nStringID == STRID_ENABLE)
      return L"Enable";

    //Coder
    if (nStringID == STRID_GENERAL)
      return L"General";
    if (nStringID == STRID_CODER_NOTRUNNING)
      return L"Coder plugin isn't running.";
    if (nStringID == STRID_LOADFIRST)
      return L"Load %s first.";
    if (nStringID == STRID_CACHE_NONE)
      return L"Without cache";
    if (nStringID == STRID_CACHE_FAST)
      return L"Fast cache";
    if (nStringID == STRID_CACHE_SMART)
      return L"Smart cache";
    if (nStringID == STRID_DEFAULTALIAS_ENABLE)
      return L"Alias for unknown file ";
    if (nStringID == STRID_VARTHEME_NAME)
      return L"Theme name:";
    if (nStringID == STRID_VARTHEME_SAVE)
      return L"Save";
    if (nStringID == STRID_VARTHEME_DELETE)
      return L"Delete";
    if (nStringID == STRID_VARTHEME_RETURN)
      return L"Return";
    if (nStringID == STRID_VARTHEME_GLOBAL)
      return L"Global variables";
    if (nStringID == STRID_VARTHEME_LINK)
      return L"Link...";
    if (nStringID == STRID_MENU_ADDITEM)
      return L"Add...\tInsert";
    if (nStringID == STRID_MENU_MODITEM)
      return L"Modify...\tF2";
    if (nStringID == STRID_MENU_MOVEUPITEM)
      return L"Move up\tAlt+Up";
    if (nStringID == STRID_MENU_MOVEDOWNITEM)
      return L"Move down\tAlt+Down";
    if (nStringID == STRID_MENU_DELITEM)
      return L"Delete\tDelete";
    if (nStringID == STRID_MENU_CLEARITEM)
      return L"Clear";
    if (nStringID == STRID_VARIABLE)
      return L"Variable";
    if (nStringID == STRID_VALUE)
      return L"Value";
    if (nStringID == STRID_MAKEGLOBAL)
      return L"Make global";
    if (nStringID == STRID_LOWPRIORITY)
      return L"Low priority";
    if (nStringID == STRID_COLOR)
      return L"Color";
    if (nStringID == STRID_SYNTAXFILE)
      return L"Syntax file";
    if (nStringID == STRID_VARTHEMENAME)
      return L"Theme";
    if (nStringID == STRID_ADDVAR)
      return L"Add variable";
    if (nStringID == STRID_EDITVAR)
      return L"Change variable";
    if (nStringID == STRID_LOSTPROMPT)
      return L"Changes in \"%s\" theme will be lost. Continue?";
    if (nStringID == STRID_OVERWRITEPROMPT)
      return L"\"%s\" already exists. Overwrite it?";
    if (nStringID == STRID_DELETEPROMPT)
      return L"Are you sure you want to delete \"%s\" theme?";
    if (nStringID == STRID_VARMISSING)
      return L"\"%s\" contain unknown variable \"%.%ds\" that doesn't exist in current theme \"%s\". Continue?";
    if (nStringID == STRID_REGEXP_COMPILEERROR)
      return L"\"%s\" contain non valid regular expression \"%.%ds\"";
    if (nStringID == STRID_UNKNOWNSECTION)
      return L"\"%s\" (offset %Id) contain unknown section name \"%s\".";
    if (nStringID == STRID_UNKNOWNSYNTAXFILE)
      return L"Can't link \"%s\" theme to \"%s\" file: unknown syntax file.";
    if (nStringID == STRID_UNKNOWNVARTHEME)
      return L"Can't link \"%s\" theme to \"%s\" file: unknown theme.";

    //HighLight
    if (nStringID == STRID_IGNORE_FONTSTYLE_GROUP)
      return L"Ignore font style";
    if (nStringID == STRID_NORMAL)
      return L"Normal";
    if (nStringID == STRID_BOLD)
      return L"Bold";
    if (nStringID == STRID_ITALIC)
      return L"Italic";
    if (nStringID == STRID_AUTOMARK_GROUP)
      return L"Automatic selection marking";
    if (nStringID == STRID_SYMBOLS)
      return L"Symbols";
    if (nStringID == STRID_WORDS)
      return L"Words";
    if (nStringID == STRID_MAXSEL)
      return L"Selection limit:";
    if (nStringID == STRID_TEXTCOLOR)
      return L"Text color (RRGGBB):";
    if (nStringID == STRID_BGCOLOR)
      return L"Background color (RRGGBB):";

    //CodeFold
    if (nStringID == STRID_RICHEDIT_NOTSUPPORTED)
      return L"AkelPad 3.x.x isn't supported.";
    if (nStringID == STRID_GOBEGIN)
      return L"Go to begin";
    if (nStringID == STRID_GOEND)
      return L"Go to end";
    if (nStringID == STRID_SELECT)
      return L"Select";
    if (nStringID == STRID_INVIEW)
      return L"In view";
    if (nStringID == STRID_LISTEXPAND)
      return L"Expand list";
    if (nStringID == STRID_LISTCOLLAPSE)
      return L"Collapse list";
    if (nStringID == STRID_LISTCOPY)
      return L"Copy list";
    if (nStringID == STRID_STATISTICS)
      return L"Statistics";
    if (nStringID == STRID_STATISTICS_MSG)
      return L"All folds: %d\n    Root folds: %d\n    Children folds: %d\nCurrent fold children: %d\n";
    if (nStringID == STRID_GORULE)
      return L"Go to rule";
    if (nStringID == STRID_SETUP)
      return L"Settings...";
    if (nStringID == STRID_NONE)
      return L"None";
    if (nStringID == STRID_SHOWDOCK_GROUP)
      return L"Show list";
    if (nStringID == STRID_AUTO)
      return L"Auto";
    if (nStringID == STRID_ALWAYS)
      return L"Always";
    if (nStringID == STRID_ASLIST)
      return L"As list";
    if (nStringID == STRID_FOLDLIMIT)
      return L"Folds limit:";
    if (nStringID == STRID_FOLLOWCARET_GROUP)
      return L"Follow caret";
    if (nStringID == STRID_ANYWHERE)
      return L"Anywhere";
    if (nStringID == STRID_ROOT)
      return L"On root";
    if (nStringID == STRID_SHOWNODES_GROUP)
      return L"Show nodes";
    if (nStringID == STRID_DRAWNODE)
      return L"Draw node as";
    if (nStringID == STRID_ROUND)
      return L"Round";
    if (nStringID == STRID_SQUARE)
      return L"Square";
    if (nStringID == STRID_TAGMARK)
      return L"Mark tag";
    if (nStringID == STRID_COLLAPSEONOPEN)
      return L"Collapse all on open";
    if (nStringID == STRID_NOPRINTCOLLAPSED)
      return L"No print collapsed lines";
    if (nStringID == STRID_HIDEFOLDEND)
      return L"Hide fold last line";
    if (nStringID == STRID_FINDROOT)
      return L"Find root level";
    if (nStringID == STRID_HOTKEY)
      return L"Hotkey";
    if (nStringID == STRID_DEPTH)
      return L"Depth";
    if (nStringID == STRID_MATCHCASE)
      return L"Match case";
    if (nStringID == STRID_WHOLEWORD)
      return L"Whole word";
    if (nStringID == STRID_HOTKEYCURRENT)
      return L"Current level";
    if (nStringID == STRID_HOTKEYALL)
      return L"All levels";
    if (nStringID == STRID_COLLAPSE)
      return L"Collapse";
    if (nStringID == STRID_NEXTLEVEL)
      return L"Next level";
    if (nStringID == STRID_PREVLEVEL)
      return L"Previous level";

    //AutoComplete
    if (nStringID == STRID_AUTOLIST)
      return L"Automatic list";
    if (nStringID == STRID_AUTOLIST_PRE)
      return L"Show after";
    if (nStringID == STRID_AFTERCHAR_POST)
      return L"characters";
    if (nStringID == STRID_TRANSPARENCY)
      return L"Transparency";
    if (nStringID == STRID_ALPHA)
      return L"Alpha";
    if (nStringID == STRID_HOTKEYS)
      return L"Hotkeys";
    if (nStringID == STRID_COMPLETEWITHLIST)
      return L"Complete with list";
    if (nStringID == STRID_COMPLETEWITHOUTLIST)
      return L"Complete without list";
    if (nStringID == STRID_COMPLETEWITHOUTLIST_PRE)
      return L"Complete without list after";
    if (nStringID == STRID_COMPLETENEXT)
      return L"Go to next position";
    if (nStringID == STRID_COMPLETEPREV)
      return L"Go to previous position";
    if (nStringID == STRID_DOCUMENT)
      return L"Document";
    if (nStringID == STRID_ADDDOCUMENTWORDS)
      return L"Add words from document (marked with plus +)";
    if (nStringID == STRID_MAXDOCUMENT)
      return L"Maximum document";
    if (nStringID == STRID_CHARS)
      return L"characters";
    if (nStringID == STRID_LISTSYSTEMCOLORS)
      return L"System colors in list";
    if (nStringID == STRID_LISTSYSTEMFONT)
      return L"System font in list";
    if (nStringID == STRID_COMPLETENONSYNTAXDOCUMENT)
      return L"Complete also document without syntax theme";
    if (nStringID == STRID_SAVETYPEDCASE)
      return L"Save typed symbols case";
    if (nStringID == STRID_INHERITTYPEDCASE)
      return L"Inherit typed case";
    if (nStringID == STRID_ADDHIGHLIGHTWORDS)
      return L"Add words from HighLight base (marked with asterisk *)";
    if (nStringID == STRID_HLBASECOLORS)
      return L"Use item colors from HighLight base";
    if (nStringID == STRID_NOSYMBOLMARK)
      return L"No symbol mark (+*) if icon used in list";
    if (nStringID == STRID_RIGHTDELIMITERS)
      return L"Consider delimiters from right at the caret";
    if (nStringID == STRID_SYNTAXDELIMITERS)
      return L"Use delimiters from syntax theme";
    if (nStringID == STRID_COMPLETECASESENSITIVE)
      return L"Case sensitive completion";
  }
  return L"";
}

BOOL IsExtCallParamValid(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return TRUE;
  return FALSE;
}

INT_PTR GetExtCallParam(LPARAM lParam, int nIndex)
{
  if (*((INT_PTR *)lParam) >= (INT_PTR)((nIndex + 1) * sizeof(INT_PTR)))
    return *(((INT_PTR *)lParam) + nIndex);
  return 0;
}

void InitCommon(PLUGINDATA *pd)
{
  bInitCommon=TRUE;
  hInstanceDLL=pd->hInstanceDLL;
  hInstanceEXE=pd->hInstanceEXE;
  hMainWnd=pd->hMainWnd;
  hMdiClient=pd->hMdiClient;
  hMainIcon=pd->hMainIcon;
  bOldWindows=pd->bOldWindows;
  bOldRichEdit=pd->bOldRichEdit;
  bAkelEdit=pd->bAkelEdit;
  nMDI=pd->nMDI;
  wLangModule=PRIMARYLANGID(pd->wLangModule);

  //Initialize WideFunc.h header
  WideInitialize();

  //Plugin name
  {
    int i;

    for (i=0; pd->wszFunction[i] != L':'; ++i)
      wszPluginName[i]=pd->wszFunction[i];
    wszPluginName[i]=L'\0';
  }
  xprintfW(wszPluginTitle, GetLangStringW(wLangModule, STRID_PLUGIN), wszPluginName);
  xstrcpynW(wszExeDir, pd->wszAkelDir, MAX_PATH);
  xprintfW(wszCoderDir, L"%s\\AkelFiles\\Plugs\\Coder", pd->wszAkelDir);

  //GLOBAL variable theme
  hVarThemeGlobal.hVarStack.first=NULL;
  hVarThemeGlobal.hVarStack.last=NULL;
  hVarThemeGlobal.nVarThemeNameLen=(int)xstrcpynW(hVarThemeGlobal.wszVarThemeName, GetLangStringW(wLangModule, STRID_VARTHEME_GLOBAL), MAX_PATH);

  //Read active variable theme
  lpVarThemeActive=NULL;
  AddInternalVarThemes(&hVarThemesStack);
  ReadOptions(OF_GENERAL_ALLTHEMES|OF_GENERAL_THEMELINK|OF_GENERAL_SETTINGS|OF_HIGHLIGHT|OF_CODEFOLD|OF_AUTOCOMPLETE);
  if (!lpVarThemeActive)
    lpVarThemeActive=RequestVarTheme(&hVarThemesStack, L"Default");
  GetVarThemeGlobals(lpVarThemeActive);
}

void InitMain()
{
  if (!nInitMain++)
  {
    ReadSyntaxFiles();

    //SubClass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

    if (nMDI == WMD_MDI)
    {
      NewFrameProcData=NULL;
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
    }

    NewEditProcData=NULL;
    SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NewEditProc, (LPARAM)&NewEditProcData);
  }
}

void UninitMain()
{
  if (!--nInitMain)
  {
    //Remove subclass
    if (NewMainProcData)
    {
      SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
      NewMainProcData=NULL;
    }
    if (NewFrameProcData)
    {
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&NewFrameProcData);
      NewFrameProcData=NULL;
    }
    if (NewEditProcData)
    {
      SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&NewEditProcData);
      NewEditProcData=NULL;
    }

    SaveCache(dwSaveCache);
    StackFreeSyntaxFiles(&hSyntaxFilesStack);
    StackFreeVarThemes(&hVarThemesStack);
    StackFreeVars(&hVarThemeGlobal.hVarStack);
  }
}

//Entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_DETACH)
  {
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
    if (wszVarThemeLink)
    {
      GlobalFree((HGLOBAL)wszVarThemeLink);
      wszVarThemeLink=NULL;
    }
  }
  return TRUE;
}

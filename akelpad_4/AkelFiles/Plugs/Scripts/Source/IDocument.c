#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "Scripts.h"


//Global variables
CALLBACKSTACK g_hSubclassCallbackStack={0};
CALLBACKSTACK g_hHookCallbackStack={0};
ITypeInfo *g_DocumentTypeInfo=NULL;
BOOL g_bScriptArg=FALSE;
WNDPROCDATA *g_lpSubclassMainProc=NULL;
WNDPROCDATA *g_lpSubclassEditProc=NULL;
WNDPROCDATA *g_lpSubclassFrameProc=NULL;

const IDocumentVtbl MyIDocumentVtbl={
  Document_QueryInterface,
  Document_AddRef,
  Document_Release,
  Document_GetTypeInfoCount,
  Document_GetTypeInfo,
  Document_GetIDsOfNames,
  Document_Invoke,
  Document_Constants,
  Document_Global,
  Document_GetMainWnd,
  Document_GetAkelDir,
  Document_GetInstanceExe,
  Document_GetInstanceDll,
  Document_GetLangId,
  Document_IsOldWindows,
  Document_IsAkelEdit,
  Document_IsMDI,
  Document_GetEditWnd,
  Document_SetEditWnd,
  Document_GetEditDoc,
  Document_GetEditFile,
  Document_GetFilePath,
  Document_GetEditCodePage,
  Document_GetEditBOM,
  Document_GetEditNewLine,
  Document_GetEditModified,
  Document_GetEditReadOnly,
  Document_SetFrameInfo,
  Document_SendMessage,
  Document_MessageBox,
  Document_InputBox,
  Document_GetSelStart,
  Document_GetSelEnd,
  Document_SetSel,
  Document_GetSelText,
  Document_GetTextRange,
  Document_ReplaceSel,
  Document_TextFind,
  Document_TextReplace,
  Document_GetClipboardText,
  Document_SetClipboardText,
  Document_IsPluginRunning,
  Document_Call,
  Document_CallA,
  Document_CallW,
  Document_CallEx,
  Document_CallExA,
  Document_CallExW,
  Document_Exec,
  Document_Command,
  Document_Font,
  Document_Recode,
  Document_Include,
  Document_IsInclude,
  Document_OpenFile,
  Document_ReadFile,
  Document_WriteFile,
  Document_SaveFile,
  Document_ScriptSettings,
  Document_SystemFunction,
  Document_MemAlloc,
  Document_MemCopy,
  Document_MemRead,
  Document_MemStrPtr,
  Document_MemPtrStr,
  Document_MemFree,
  Document_DebugJIT,
  Document_Debug,
  Document_VarType,
  Document_VarDispatch,
  Document_GetArgLine,
  Document_GetArgValue,
  Document_CreateDialog,
  Document_WindowRegisterClass,
  Document_WindowUnregisterClass,
  Document_WindowRegisterDialog,
  Document_WindowUnregisterDialog,
  Document_WindowGetMessage,
  Document_WindowSubClass,
  Document_WindowNextProc,
  Document_WindowNoNextProc,
  Document_WindowUnsubClass,
  Document_ThreadHook,
  Document_ThreadUnhook,
  Document_ScriptNoMutex,
  Document_ScriptExitCode,
  Document_ScriptHandle,
  Document_ConnectObject,
  Document_DisconnectObject,
  Document_ActiveXThis
};

CALLBACKBUSYNESS g_cbHook[]={{(INT_PTR)HookCallback1Proc,  FALSE},
                             {(INT_PTR)HookCallback2Proc,  FALSE},
                             {(INT_PTR)HookCallback3Proc,  FALSE},
                             {(INT_PTR)HookCallback4Proc,  FALSE},
                             {(INT_PTR)HookCallback5Proc,  FALSE},
                             {(INT_PTR)HookCallback6Proc,  FALSE},
                             {(INT_PTR)HookCallback7Proc,  FALSE},
                             {(INT_PTR)HookCallback8Proc,  FALSE},
                             {(INT_PTR)HookCallback9Proc,  FALSE},
                             {(INT_PTR)HookCallback10Proc, FALSE},
                             {(INT_PTR)HookCallback11Proc, FALSE},
                             {(INT_PTR)HookCallback12Proc, FALSE},
                             {(INT_PTR)HookCallback13Proc, FALSE},
                             {(INT_PTR)HookCallback14Proc, FALSE},
                             {(INT_PTR)HookCallback15Proc, FALSE},
                             {(INT_PTR)HookCallback16Proc, FALSE},
                             {(INT_PTR)HookCallback17Proc, FALSE},
                             {(INT_PTR)HookCallback18Proc, FALSE},
                             {(INT_PTR)HookCallback19Proc, FALSE},
                             {(INT_PTR)HookCallback20Proc, FALSE},
                             {(INT_PTR)HookCallback21Proc, FALSE},
                             {(INT_PTR)HookCallback22Proc, FALSE},
                             {(INT_PTR)HookCallback23Proc, FALSE},
                             {(INT_PTR)HookCallback24Proc, FALSE},
                             {(INT_PTR)HookCallback25Proc, FALSE},
                             {(INT_PTR)HookCallback26Proc, FALSE},
                             {(INT_PTR)HookCallback27Proc, FALSE},
                             {(INT_PTR)HookCallback28Proc, FALSE},
                             {(INT_PTR)HookCallback29Proc, FALSE},
                             {(INT_PTR)HookCallback30Proc, FALSE},
                             {0, 0}};


//// IDocument

HRESULT STDMETHODCALLTYPE Document_QueryInterface(IDocument *this, REFIID vTableGuid, void **ppv)
{
  if (AKD_IsEqualIID(vTableGuid, &IID_IUnknown) || AKD_IsEqualIID(vTableGuid, &IID_IDocument) || AKD_IsEqualIID(vTableGuid, &IID_IDispatch))
  {
    *ppv=this;
    this->lpVtbl->Document_AddRef(this);
    return NOERROR;
  }
  *ppv=NULL;
  return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE Document_AddRef(IDocument *this)
{
  return ++((IRealDocument *)this)->dwCount;
}

ULONG STDMETHODCALLTYPE Document_Release(IDocument *this)
{
  if (--((IRealDocument *)this)->dwCount == 0)
  {
    GlobalFree(this);
    InterlockedDecrement(&g_nObjs);
    return 0;
  }
  return ((IRealDocument *)this)->dwCount;
}


//// IDispatch

HRESULT STDMETHODCALLTYPE Document_GetTypeInfoCount(IDocument *this, UINT *pCount)
{
  *pCount=1;
  return S_OK;
}

HRESULT STDMETHODCALLTYPE Document_GetTypeInfo(IDocument *this, UINT itinfo, LCID lcid, ITypeInfo **pTypeInfo)
{
  HRESULT hr;

  *pTypeInfo=NULL;

  if (itinfo)
  {
    hr=ResultFromScode(DISP_E_BADINDEX);
  }
  else if (g_DocumentTypeInfo)
  {
    g_DocumentTypeInfo->lpVtbl->AddRef(g_DocumentTypeInfo);
    hr=S_OK;
  }
  else
  {
    hr=LoadTypeInfoFromFile(NULL, NULL);
  }
  if (hr == S_OK) *pTypeInfo=g_DocumentTypeInfo;

  return hr;
}

HRESULT STDMETHODCALLTYPE Document_GetIDsOfNames(IDocument *this, REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid)
{
  if (!g_DocumentTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispGetIDsOfNames(g_DocumentTypeInfo, rgszNames, cNames, rgdispid);
}

HRESULT STDMETHODCALLTYPE Document_Invoke(IDocument *this, DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *pexcepinfo, UINT *puArgErr)
{
  if (!AKD_IsEqualIID(riid, &IID_NULL))
    return DISP_E_UNKNOWNINTERFACE;

  if (!g_DocumentTypeInfo)
  {
    HRESULT hr;

    if ((hr=LoadTypeInfoFromFile(NULL, NULL)) != S_OK)
      return hr;
  }
  return DispInvoke(this, g_DocumentTypeInfo, dispid, wFlags, params, result, pexcepinfo, puArgErr);
}


//// IDocument methods

HRESULT STDMETHODCALLTYPE Document_Constants(IDocument *this, IDispatch **objConstants)
{
  return Class_CreateInstance(NULL, NULL, &IID_IConstants, (void **)objConstants);
}

HRESULT STDMETHODCALLTYPE Document_Global(IDocument *this, IDispatch **objGlobal)
{
  return Class_CreateInstance(NULL, NULL, &IID_IGlobal, (void **)objGlobal);
}

HRESULT STDMETHODCALLTYPE Document_GetMainWnd(IDocument *this, VARIANT *vtWnd)
{
  SetVariantInt(vtWnd, (INT_PTR)hMainWnd);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetAkelDir(IDocument *this, int nSubDir, BSTR *wpDir)
{
  HRESULT hr=NOERROR;

  if (nSubDir == ADTYPE_ROOT)
    xprintfW(wszBuffer, L"%s", wszAkelPadDir);
  else if (nSubDir == ADTYPE_AKELFILES)
    xprintfW(wszBuffer, L"%s\\AkelFiles", wszAkelPadDir);
  else if (nSubDir == ADTYPE_DOCS)
    xprintfW(wszBuffer, L"%s\\AkelFiles\\Docs", wszAkelPadDir);
  else if (nSubDir == ADTYPE_LANGS)
    xprintfW(wszBuffer, L"%s\\AkelFiles\\Langs", wszAkelPadDir);
  else if (nSubDir == ADTYPE_PLUGS)
    xprintfW(wszBuffer, L"%s\\AkelFiles\\Plugs", wszAkelPadDir);
  else if (nSubDir == ADTYPE_SCRIPTS)
    xprintfW(wszBuffer, L"%s\\AkelFiles\\Plugs\\Scripts", wszAkelPadDir);
  else if (nSubDir == ADTYPE_INCLUDE)
    xprintfW(wszBuffer, L"%s\\AkelFiles\\Plugs\\Scripts\\Include", wszAkelPadDir);
  else
    wszBuffer[0]=L'\0';

  if (!(*wpDir=SysAllocString(wszBuffer)))
    hr=E_OUTOFMEMORY;

  return hr;
}

HRESULT STDMETHODCALLTYPE Document_GetInstanceExe(IDocument *this, VARIANT *vtInstance)
{
  SetVariantInt(vtInstance, (INT_PTR)hInstanceEXE);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetInstanceDll(IDocument *this, VARIANT *vtInstance)
{
  SetVariantInt(vtInstance, (INT_PTR)hInstanceDLL);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetLangId(IDocument *this, int nType, int *nLangModule)
{
  if (nType == LANGID_FULL)
    *nLangModule=(int)wLangModule;
  else if (nType == LANGID_PRIMARY)
    *nLangModule=(int)PRIMARYLANGID(wLangModule);
  else if (nType == LANGID_SUB)
    *nLangModule=(int)SUBLANGID(wLangModule);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_IsOldWindows(IDocument *this, BOOL *bIsOld)
{
  *bIsOld=bOldWindows;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_IsAkelEdit(IDocument *this, VARIANT vtWnd, int *nIsAkelEdit)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);

  if (hWnd)
  {
    char szName[MAX_PATH];

    if (GetClassNameA(hWnd, szName, MAX_PATH) &&
        (!xstrcmpinA("AkelEdit", szName, (UINT_PTR)-1) ||
         !xstrcmpinA("RichEdit20", szName, (UINT_PTR)-1)))
    {
      if (SendMessage(hMainWnd, AKD_FRAMEFIND, FWF_BYEDITWINDOW, (LPARAM)hWnd))
        *nIsAkelEdit=ISAEW_PROGRAM;
      else
        *nIsAkelEdit=ISAEW_PLUGIN;
    }
    else *nIsAkelEdit=ISAEW_NONE;
  }
  else *nIsAkelEdit=bAkelEdit;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_IsMDI(IDocument *this, int *nIsMDI)
{
  *nIsMDI=nMDI;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditWnd(IDocument *this, VARIANT *vtWnd)
{
  SetVariantInt(vtWnd, (INT_PTR)GetCurEdit(this));
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_SetEditWnd(IDocument *this, VARIANT vtWnd, VARIANT *vtWndResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  int nIsAkelEdit;

  if (hWnd)
  {
    if ((INT_PTR)hWnd == SEW_FOCUS)
      hWnd=(HWND)SendMessage(hMainWnd, AKD_GETFOCUS, 0, 0);
    SetVariantInt(&vtWnd, (INT_PTR)hWnd);
    Document_IsAkelEdit(this, vtWnd, &nIsAkelEdit);

    if (nIsAkelEdit != ISAEW_NONE)
      lpScriptThread->hWndPluginEdit=hWnd;
    else
      lpScriptThread->hWndPluginEdit=NULL;
  }
  else lpScriptThread->hWndPluginEdit=NULL;

  SetVariantInt(vtWndResult, (INT_PTR)lpScriptThread->hWndPluginEdit);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditDoc(IDocument *this, VARIANT *vtDoc)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  AEHDOC hDoc;

  if (lpScriptThread && lpScriptThread->hWndPluginEdit)
    hDoc=(AEHDOC)SendMessage(lpScriptThread->hWndPluginEdit, AEM_GETDOCUMENT, 0, 0);
  else
    hDoc=(AEHDOC)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_DOCEDIT, (LPARAM)NULL);
  SetVariantInt(vtDoc, (INT_PTR)hDoc);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditFile(IDocument *this, VARIANT vtWnd, BSTR *wpFile)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  EDITINFO ei;
  HRESULT hr=NOERROR;

  *wpFile=NULL;
  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
  {
    if (!(*wpFile=SysAllocString(ei.wszFile)))
      hr=E_OUTOFMEMORY;
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_GetFilePath(IDocument *this, BSTR wpFile, int nPart, BSTR *wpPath)
{
  int nFileLen;
  HRESULT hr=NOERROR;

  *wpPath=NULL;
  nFileLen=SysStringLen(wpFile);

  if (nPart == CPF_DIR)
  {
    if (!GetFileDir(wpFile, nFileLen, wszBuffer, BUFFER_SIZE))
      wszBuffer[0]=L'\0';
    if (!(*wpPath=SysAllocString(wszBuffer)))
      hr=E_OUTOFMEMORY;
  }
  else if (nPart == CPF_FILENAME)
  {
    const wchar_t *wpFileName;

    if (!(wpFileName=GetFileNameWide(wpFile, nFileLen)))
      wpFileName=L"";
    if (!(*wpPath=SysAllocString(wpFileName)))
      hr=E_OUTOFMEMORY;
  }
  else if (nPart == CPF_FILEBASENAME)
  {
    if (!GetBaseName(wpFile, nFileLen, wszBuffer, BUFFER_SIZE))
      wszBuffer[0]=L'\0';
    if (!(*wpPath=SysAllocString(wszBuffer)))
      hr=E_OUTOFMEMORY;
  }
  else if (nPart == CPF_FILEEXT)
  {
    const wchar_t *wpExt;

    if (!(wpExt=GetFileExt(wpFile, nFileLen)))
      wpExt=L"";
    if (!(*wpPath=SysAllocString(wpExt)))
      hr=E_OUTOFMEMORY;
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_GetEditCodePage(IDocument *this, VARIANT vtWnd, int *nCodePage)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
    *nCodePage=ei.nCodePage;
  else
    *nCodePage=0;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditBOM(IDocument *this, VARIANT vtWnd, BOOL *bBOM)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
    *bBOM=ei.bBOM;
  else
    *bBOM=0;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditNewLine(IDocument *this, VARIANT vtWnd, int *nNewLine)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
    *nNewLine=ei.nNewLine;
  else
    *nNewLine=0;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditModified(IDocument *this, VARIANT vtWnd, BOOL *bModified)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
    *bModified=ei.bModified;
  else
    *bModified=0;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetEditReadOnly(IDocument *this, VARIANT vtWnd, BOOL *bReadOnly)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
    *bReadOnly=ei.bReadOnly;
  else
    *bReadOnly=0;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_SetFrameInfo(IDocument *this, VARIANT vtFrame, int nType, VARIANT vtData, BOOL *bResult)
{
  FRAMEDATA *lpFrame=(FRAMEDATA *)GetVariantInt(&vtFrame, NULL);
  UINT_PTR dwData=GetVariantInt(&vtData, NULL);
  FRAMEINFO fi;

  fi.nType=nType;
  fi.dwData=dwData;
  *bResult=(BOOL)SendMessage(hMainWnd, AKD_SETFRAMEINFO, (WPARAM)&fi, (LPARAM)lpFrame);

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_SendMessage(IDocument *this, VARIANT vtWnd, UINT uMsg, VARIANT vtWParam, VARIANT vtLParam, VARIANT *vtResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  VARIANT *pvtWParam=&vtWParam;
  VARIANT *pvtLParam=&vtLParam;
  WPARAM wParam;
  LPARAM lParam;
  LRESULT lResult;

  if (lpScriptThread->bLockSendMessage)
  {
    wParam=GetVariantValue(pvtWParam, &pvtWParam, bOldWindows);
    lParam=GetVariantValue(pvtLParam, &pvtLParam, bOldWindows);

    if (bOldWindows)
    {
      lResult=SendMessageA(hWnd, uMsg, wParam, lParam);
      if (pvtWParam->vt == VT_BSTR) GlobalFree((HGLOBAL)wParam);
      if (pvtLParam->vt == VT_BSTR) GlobalFree((HGLOBAL)lParam);
    }
    else lResult=SendMessageW(hWnd, uMsg, wParam, lParam);
  }
  else
  {
    MSGMUTEX mm;

    if (!nMutexMsgCount++)
    {
      //Optimization: don't create first mutex, if it already created
      if (!hMutexMsgFirst)
      {
        xprintfW(wszBuffer, wszMutexMsgName, nMutexMsgCount);
        hMutexMsgFirst=CreateEventWide(NULL, FALSE, FALSE, wszBuffer);
      }
      else ResetEvent(hMutexMsgFirst);

      mm.hMutex=hMutexMsgFirst;
    }
    else
    {
      xprintfW(wszBuffer, wszMutexMsgName, nMutexMsgCount);
      mm.hMutex=CreateEventWide(NULL, FALSE, FALSE, wszBuffer);
    }
    mm.bSignaled=FALSE;

    if (mm.hMutex)
    {
      MSG msg;
      BOOL bExitLoop=FALSE;

      wParam=GetVariantValue(pvtWParam, &pvtWParam, bOldWindows);
      lParam=GetVariantValue(pvtLParam, &pvtLParam, bOldWindows);

      if (bOldWindows)
      {
        SendMessageCallbackA(hWnd, uMsg, wParam, lParam, (SENDASYNCPROC)SendMessageAsyncProc, (LPARAM)&mm);
        if (pvtWParam->vt == VT_BSTR) GlobalFree((HGLOBAL)wParam);
        if (pvtLParam->vt == VT_BSTR) GlobalFree((HGLOBAL)lParam);
      }
      else SendMessageCallbackW(hWnd, uMsg, wParam, lParam, (SENDASYNCPROC)SendMessageAsyncProc, (LPARAM)&mm);

      if (!mm.bSignaled)
      {
        //Wait for mm.hMutex and process messages.
        for (;;)
        {
          while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
          {
            if (msg.message == WM_QUIT)
              bExitLoop=TRUE;
            else
            {
              if (TranslateMessageProc)
                TranslateMessageProc(TMSG_ALL, &msg);
            }
          }
          if (bExitLoop)
            break;
          if (MsgWaitForMultipleObjects(1, &mm.hMutex, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
            break;
        }
      }
      if (mm.hMutex != hMutexMsgFirst)
        CloseHandle(mm.hMutex);
      lResult=mm.lResult;
    }
    --nMutexMsgCount;
  }

  SetVariantInt(vtResult, lResult);
  return NOERROR;
}

void CALLBACK SendMessageAsyncProc(HWND hWnd, UINT uMsg, UINT_PTR dwData, LRESULT lResult)
{
  MSGMUTEX *mm=(MSGMUTEX *)dwData;

  SetEvent(mm->hMutex);
  mm->bSignaled=TRUE;
  mm->lResult=lResult;
}

HRESULT STDMETHODCALLTYPE Document_MessageBox(IDocument *this, VARIANT vtWnd, BSTR pText, BSTR pCaption, UINT uType, SAFEARRAY **psa, int *nResult)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  DIALOGMESSAGEBOX dmb;
  BUTTONMESSAGEBOX *bmb;

  if (bmb=FillButtonsArray(*psa, &dmb.hIcon))
  {
    dmb.hWndParent=hWnd;
    dmb.wpText=pText;
    dmb.wpCaption=pCaption;
    dmb.uType=uType;
    dmb.bmb=bmb;
    *nResult=(int)SendMessage(hMainWnd, AKD_MESSAGEBOX, 0, (LPARAM)&dmb);

    GlobalFree((HGLOBAL)bmb);
  }
  else *nResult=MessageBoxW(hWnd, pText, pCaption, uType);

  return NOERROR;
}

BUTTONMESSAGEBOX* FillButtonsArray(SAFEARRAY *psa, HICON *hIcon)
{
  BUTTONMESSAGEBOX *bmb=NULL;
  BUTTONMESSAGEBOX *bmbNext;
  VARIANT *pvtParameter;
  unsigned char *lpData;
  DWORD dwElement;
  DWORD dwElementSum;

  lpData=(unsigned char *)(psa->pvData);
  dwElementSum=psa->rgsabound[0].cElements;
  if (!dwElementSum)
    return 0;

  //DIALOGMESSAGEBOX.hIcon
  pvtParameter=(VARIANT *)lpData;
  *hIcon=(HICON)GetVariantInt(pvtParameter, NULL);
  lpData+=sizeof(VARIANT);
  --dwElementSum;

  //BUTTONMESSAGEBOX array
  dwElementSum=(dwElementSum / 3) * 3;
  if (!dwElementSum)
    return 0;

  if (bmb=GlobalAlloc(GPTR, (dwElementSum / 3 + 1) * sizeof(BUTTONMESSAGEBOX)))
  {
    bmbNext=bmb;
    dwElement=0;

    while (dwElement < dwElementSum)
    {
      //BUTTONMESSAGEBOX.nButtonControlID
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      bmbNext->nButtonControlID=(int)GetVariantInt(pvtParameter, NULL);
      ++dwElement;

      //BUTTONMESSAGEBOX.wpButtonStr
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      bmbNext->wpButtonStr=(wchar_t *)GetVariantValue(pvtParameter, NULL, FALSE);
      ++dwElement;

      //BUTTONMESSAGEBOX.dwFlags
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      bmbNext->dwFlags=(DWORD)GetVariantInt(pvtParameter, NULL);
      ++dwElement;

      ++bmbNext;
    }
  }
  return bmb;
}

HRESULT STDMETHODCALLTYPE Document_InputBox(IDocument *this, VARIANT vtWnd, BSTR wpCaption, BSTR wpLabel, BSTR wpEdit, VARIANT *vtResult)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  INPUTBOX db;

  VariantInit(vtResult);

  db.wpCaption=wpCaption;
  db.wpLabel=wpLabel;
  db.wpEdit=wpEdit;
  db.vtResult=vtResult;
  db.hr=NOERROR;
  DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_INPUTBOX), hWnd, (DLGPROC)InputBoxProc, (LPARAM)&db);

  return db.hr;
}

BOOL CALLBACK InputBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndLabel;
  static HWND hWndEditBox;
  static HWND hWndOK;
  static HWND hWndCancel;
  static INPUTBOX *db;

  if (uMsg == WM_INITDIALOG)
  {
    db=(INPUTBOX *)lParam;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hPluginIcon);
    hWndLabel=GetDlgItem(hDlg, IDC_INPUTBOX_LABEL);
    hWndEditBox=GetDlgItem(hDlg, IDC_INPUTBOX_EDIT);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SetDlgItemTextWide(hDlg, IDC_INPUTBOX_LABEL, db->wpLabel);
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));
    SetWindowTextWide(hDlg, db->wpCaption);
    SetWindowTextWide(hWndEditBox, db->wpEdit);

    //Multiline label
    {
      RECT rc;
      wchar_t *wpCount;
      int nHeight=0;
      int nLines=0;

      for (wpCount=db->wpLabel; *wpCount; ++wpCount)
      {
        if (*wpCount == L'\n')
          ++nLines;
      }
      if (nLines > 0)
      {
        GetWindowSize(hWndLabel, hDlg, &rc);
        nHeight=rc.bottom * nLines;
        SetWindowPos(hWndLabel, 0, 0, 0, rc.right, rc.bottom + nHeight, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);

        GetWindowSize(hDlg, NULL, &rc);
        SetWindowPos(hDlg, 0, 0, 0, rc.right, rc.bottom + nHeight, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);

        GetWindowSize(hWndEditBox, hDlg, &rc);
        SetWindowPos(hWndEditBox, 0, rc.left, rc.top + nHeight, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);

        GetWindowSize(hWndOK, hDlg, &rc);
        SetWindowPos(hWndOK, 0, rc.left, rc.top + nHeight, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);

        GetWindowSize(hWndCancel, hDlg, &rc);
        SetWindowPos(hWndCancel, 0, rc.left, rc.top + nHeight, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      wchar_t *wszText;
      int nTextLen;

      nTextLen=GetWindowTextLengthA(hWndEditBox);

      if (wszText=(wchar_t *)GlobalAlloc(GPTR, (nTextLen + 1) * sizeof(wchar_t)))
      {
        GetWindowTextWide(hWndEditBox, wszText, nTextLen + 1);

        db->vtResult->vt=VT_BSTR;
        if (!(db->vtResult->bstrVal=SysAllocString(wszText)))
          db->hr=E_OUTOFMEMORY;

        GlobalFree((HGLOBAL)wszText);
      }
      EndDialog(hDlg, 0);
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

HRESULT STDMETHODCALLTYPE Document_GetSelStart(IDocument *this, VARIANT *vtSelStart)
{
  HWND hWndCurEdit;
  CHARRANGE64 cr={0};

  if (hWndCurEdit=GetCurEdit(this))
    SendMessage(hWndCurEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
  SetVariantInt(vtSelStart, cr.cpMin);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetSelEnd(IDocument *this, VARIANT *vtSelEnd)
{
  HWND hWndCurEdit;
  CHARRANGE64 cr={0};

  if (hWndCurEdit=GetCurEdit(this))
    SendMessage(hWndCurEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
  SetVariantInt(vtSelEnd, cr.cpMax);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_SetSel(IDocument *this, VARIANT vtSelStart, VARIANT vtSelEnd, DWORD dwFlags, DWORD dwType)
{
  INT_PTR nSelStart=GetVariantInt(&vtSelStart, NULL);
  INT_PTR nSelEnd=GetVariantInt(&vtSelEnd, NULL);
  HWND hWndCurEdit;

  if (hWndCurEdit=GetCurEdit(this))
  {
    if (dwFlags)
    {
      AESELECTION aes;
      AECHARINDEX *ciCaret;

      SendMessage(hWndCurEdit, AEM_RICHOFFSETTOINDEX, (WPARAM)nSelStart, (LPARAM)&aes.crSel.ciMin);
      SendMessage(hWndCurEdit, AEM_RICHOFFSETTOINDEX, (WPARAM)nSelEnd, (LPARAM)&aes.crSel.ciMax);
      aes.dwFlags=dwFlags;
      aes.dwType=dwType;
      if (nSelStart > nSelEnd)
        ciCaret=&aes.crSel.ciMin;
      else
        ciCaret=&aes.crSel.ciMax;
      SendMessage(hWndCurEdit, AEM_SETSEL, (WPARAM)ciCaret, (LPARAM)&aes);
    }
    else
    {
      CHARRANGE64 cr;

      cr.cpMin=nSelStart;
      cr.cpMax=nSelEnd;
      SendMessage(hWndCurEdit, EM_EXSETSEL64, 0, (LPARAM)&cr);
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetSelText(IDocument *this, int nNewLine, BSTR *wpText)
{
  HWND hWndCurEdit;
  CHARRANGE64 cr;
  HRESULT hr=NOERROR;

  *wpText=NULL;
  if (hWndCurEdit=GetCurEdit(this))
  {
    SendMessage(hWndCurEdit, EM_EXGETSEL64, 0, (LPARAM)&cr);
    hr=GetTextRange(hWndCurEdit, cr.cpMin, cr.cpMax, nNewLine, -1, wpText);
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_GetTextRange(IDocument *this, VARIANT vtRangeStart, VARIANT vtRangeEnd, int nNewLine, BSTR *wpText)
{
  INT_PTR nRangeStart=GetVariantInt(&vtRangeStart, NULL);
  INT_PTR nRangeEnd=GetVariantInt(&vtRangeEnd, NULL);
  HWND hWndCurEdit;
  HRESULT hr=NOERROR;

  *wpText=NULL;
  if (hWndCurEdit=GetCurEdit(this))
  {
    hr=GetTextRange(hWndCurEdit, nRangeStart, nRangeEnd, nNewLine, FALSE, wpText);
  }
  return hr;
}

HRESULT GetTextRange(HWND hWnd, INT_PTR nRangeStart, INT_PTR nRangeEnd, int nNewLine, BOOL bColumnSel, BSTR *wpText)
{
  AETEXTRANGEW tr;
  INT_PTR nTextLen=0;
  HRESULT hr=NOERROR;

  *wpText=NULL;

  SendMessage(hWnd, AEM_RICHOFFSETTOINDEX, (WPARAM)nRangeStart, (LPARAM)&tr.cr.ciMin);
  SendMessage(hWnd, AEM_RICHOFFSETTOINDEX, (WPARAM)nRangeEnd, (LPARAM)&tr.cr.ciMax);
  tr.bColumnSel=bColumnSel;
  tr.pBuffer=NULL;
  tr.dwBufferMax=(UINT_PTR)-1;
  if (nNewLine == 1)
    tr.nNewLine=AELB_R;
  else if (nNewLine == 2)
    tr.nNewLine=AELB_N;
  else if (nNewLine == 3)
    tr.nNewLine=AELB_RN;
  else
    tr.nNewLine=AELB_ASIS;
  tr.bFillSpaces=TRUE;

  if (nTextLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
  {
    if (tr.pBuffer=(wchar_t *)GlobalAlloc(GMEM_FIXED, nTextLen * sizeof(wchar_t)))
    {
      if (nTextLen=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)&tr))
      {
        if (!(*wpText=SysAllocStringLen((wchar_t *)tr.pBuffer, (UINT)nTextLen)))
          hr=E_OUTOFMEMORY;
      }
      GlobalFree((HGLOBAL)tr.pBuffer);
    }
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_ReplaceSel(IDocument *this, BSTR wpText, int nSelect)
{
  HWND hWndCurEdit;

  if (hWndCurEdit=GetCurEdit(this))
  {
    if (!(SendMessage(hWndCurEdit, AEM_GETOPTIONS, 0, 0) & AECO_READONLY))
    {
      AEREPLACESELW rs;
      AESELECTION aesInitial;
      AESELECTION aesNew;
      AECHARINDEX ciInitialCaret;
      int nFirstLine;

      if (nSelect == RST_SELECTRESTORESCROLL)
      {
        //Save scroll
        nFirstLine=SaveLineScroll(hWndCurEdit);
      }

      SendMessage(hWndCurEdit, AEM_GETSEL, (WPARAM)&ciInitialCaret, (LPARAM)&aesInitial);
      rs.pText=wpText;
      rs.dwTextLen=SysStringLen(wpText);
      rs.nNewLine=AELB_ASINPUT;
      rs.dwFlags=AEREPT_COLUMNASIS;
      rs.ciInsertStart=&aesNew.crSel.ciMin;
      rs.ciInsertEnd=&aesNew.crSel.ciMax;
      SendMessage(hWndCurEdit, AEM_REPLACESELW, 0, (LPARAM)&rs);

      if (nSelect == RST_SELECT || nSelect == RST_SELECTRESTORESCROLL)
      {
        aesNew.dwFlags=aesInitial.dwFlags;
        aesNew.dwType=aesInitial.dwType;
        if (!AEC_IndexCompare(&aesInitial.crSel.ciMin, &ciInitialCaret))
          SendMessage(hWndCurEdit, AEM_SETSEL, (WPARAM)&aesNew.crSel.ciMin, (LPARAM)&aesNew);
        else
          SendMessage(hWndCurEdit, AEM_SETSEL, (WPARAM)&aesNew.crSel.ciMax, (LPARAM)&aesNew);
      }
      if (nSelect == RST_SELECTRESTORESCROLL)
      {
        //Restore scroll
        RestoreLineScroll(hWndCurEdit, nFirstLine);
      }
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_TextFind(IDocument *this, VARIANT vtWnd, BSTR wpFindIt, DWORD dwFlags, VARIANT *vtResult)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  TEXTFINDW tf;
  INT_PTR nResult;

  tf.dwFlags=dwFlags;
  tf.pFindIt=wpFindIt;
  tf.nFindItLen=SysStringLen(wpFindIt);
  nResult=SendMessage(hMainWnd, AKD_TEXTFINDW, (WPARAM)hWnd, (LPARAM)&tf);

  SetVariantInt(vtResult, nResult);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_TextReplace(IDocument *this, VARIANT vtWnd, BSTR wpFindIt, BSTR wpReplaceWith, DWORD dwFindFlags, DWORD dwReplaceFlags, VARIANT *vtResult)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  TEXTREPLACEW tr;
  INT_PTR nResult;

  tr.dwFindFlags=dwFindFlags;
  tr.pFindIt=wpFindIt;
  tr.nFindItLen=SysStringLen(wpFindIt);
  tr.pReplaceWith=wpReplaceWith;
  tr.nReplaceWithLen=SysStringLen(wpReplaceWith);
  tr.dwReplaceFlags=dwReplaceFlags;
  nResult=SendMessage(hMainWnd, AKD_TEXTREPLACEW, (WPARAM)hWnd, (LPARAM)&tr);
  if ((dwReplaceFlags & RRF_ALL) && nResult == -1)
    nResult=tr.nChanges;

  SetVariantInt(vtResult, nResult);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetClipboardText(IDocument *this, BOOL bAnsi, BSTR *wpText)
{
  HRESULT hr=NOERROR;
  HGLOBAL hData;
  LPVOID pData;

  *wpText=NULL;

  if (OpenClipboard(NULL))
  {
    if (!bAnsi && (hData=GetClipboardData(CF_UNICODETEXT)))
    {
      if (pData=GlobalLock(hData))
      {
        if (!(*wpText=SysAllocString((wchar_t *)pData)))
          hr=E_OUTOFMEMORY;
        GlobalUnlock(hData);
      }
    }
    else if (hData=GetClipboardData(CF_TEXT))
    {
      if (pData=GlobalLock(hData))
      {
        wchar_t *wszBuffer;
        int nUnicodeLen;

        nUnicodeLen=MultiByteToWideChar(CP_ACP, 0, (char *)pData, -1, NULL, 0);

        if (wszBuffer=(wchar_t *)GlobalAlloc(GPTR, nUnicodeLen * sizeof(wchar_t)))
        {
          MultiByteToWideChar(CP_ACP, 0, (char *)pData, -1, wszBuffer, nUnicodeLen);
          if (!(*wpText=SysAllocString(wszBuffer)))
            hr=E_OUTOFMEMORY;
          GlobalFree((HGLOBAL)wszBuffer);
        }
        else hr=E_OUTOFMEMORY;

        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_SetClipboardText(IDocument *this, BSTR wpText)
{
  HRESULT hr=NOERROR;
  HGLOBAL hDataA=NULL;
  HGLOBAL hDataW=NULL;
  LPVOID pData;
  int nUnicodeLen;
  int nAnsiLen;

  if (!wpText) wpText=L"";

  if (OpenClipboard(NULL))
  {
    //Unicode
    nUnicodeLen=SysStringLen(wpText) + 1;

    if (hDataW=GlobalAlloc(GMEM_MOVEABLE, nUnicodeLen * sizeof(wchar_t)))
    {
      if (pData=GlobalLock(hDataW))
      {
        xmemcpy(pData, wpText, nUnicodeLen * sizeof(wchar_t));
        GlobalUnlock(hDataW);
      }
    }

    //ANSI
    nAnsiLen=WideCharToMultiByte(CP_ACP, 0, wpText, nUnicodeLen, NULL, 0, NULL, NULL);

    if (hDataA=GlobalAlloc(GMEM_MOVEABLE, nAnsiLen))
    {
      if (pData=GlobalLock(hDataA))
      {
        WideCharToMultiByte(CP_ACP, 0, wpText, nUnicodeLen, (char *)pData, nAnsiLen, NULL, NULL);
        GlobalUnlock(hDataA);
      }
    }
    EmptyClipboard();
    if (hDataW) SetClipboardData(CF_UNICODETEXT, hDataW);
    if (hDataA) SetClipboardData(CF_TEXT, hDataA);
    CloseClipboard();
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_IsPluginRunning(IDocument *this, BSTR wpFunction, BOOL *bRunning)
{
  PLUGINFUNCTION *pf;

  *bRunning=FALSE;

  if (pf=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wpFunction, 0))
    *bRunning=pf->bRunning;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_Call(IDocument *this, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  return CallPlugin(0, 0, wpFunction, psa, nResult);
}

HRESULT STDMETHODCALLTYPE Document_CallA(IDocument *this, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  return CallPlugin(0, PDS_STRANSI, wpFunction, psa, nResult);
}

HRESULT STDMETHODCALLTYPE Document_CallW(IDocument *this, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  return CallPlugin(0, PDS_STRWIDE, wpFunction, psa, nResult);
}

HRESULT STDMETHODCALLTYPE Document_CallEx(IDocument *this, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  return CallPlugin(dwFlags, 0, wpFunction, psa, nResult);
}

HRESULT STDMETHODCALLTYPE Document_CallExA(IDocument *this, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  return CallPlugin(dwFlags, PDS_STRANSI, wpFunction, psa, nResult);
}

HRESULT STDMETHODCALLTYPE Document_CallExW(IDocument *this, DWORD dwFlags, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  return CallPlugin(dwFlags, PDS_STRWIDE, wpFunction, psa, nResult);
}

HRESULT CallPlugin(DWORD dwFlags, DWORD dwSupport, BSTR wpFunction, SAFEARRAY **psa, int *nResult)
{
  POINTERSTACK hStringsStack={0};
  POINTERITEM *lpElement=NULL;
  VARIANT *pvtParameter;
  unsigned char *lpData;
  unsigned char *lpStructure=NULL;
  UINT_PTR dwParameter;
  DWORD dwElement;
  DWORD dwElementSum;
  DWORD dwStructSize;
  DWORD dwOffset;
  HRESULT hr=NOERROR;

  lpData=(unsigned char *)((*psa)->pvData);
  dwElementSum=(*psa)->rgsabound[0].cElements;
  dwStructSize=(dwElementSum + 1) * sizeof(UINT_PTR);
  dwOffset=0;

  if (dwElementSum)
  {
    if (lpStructure=(unsigned char *)GlobalAlloc(GPTR, dwStructSize))
    {
      //Set structure size in first parameter
      *(UINT_PTR *)lpStructure=dwStructSize;
      dwOffset+=sizeof(UINT_PTR);

      for (dwElement=0; dwElement < dwElementSum; ++dwElement)
      {
        pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
        dwParameter=0;

        if (pvtParameter->vt == (VT_BYREF|VT_VARIANT))
          pvtParameter=pvtParameter->pvarVal;

        if (pvtParameter->vt == VT_BSTR)
        {
          if ((dwSupport & PDS_STRANSI) || (bOldWindows && !(dwSupport & PDS_STRWIDE)))
          {
            char *szString;
            int nStringLenA;
            int nStringLenW;

            nStringLenW=SysStringLen(pvtParameter->bstrVal) + 1;
            nStringLenA=WideCharToMultiByte(CP_ACP, 0, pvtParameter->bstrVal, nStringLenW, NULL, 0, NULL, NULL);

            if (szString=(char *)GlobalAlloc(GPTR, nStringLenA))
            {
              WideCharToMultiByte(CP_ACP, 0, pvtParameter->bstrVal, nStringLenW, szString, nStringLenA, NULL, NULL);

              if (lpElement=StackInsertPointer(&hStringsStack))
              {
                lpElement->lpData=szString;
                lpElement->dwSize=nStringLenA;
              }
            }
            dwParameter=(UINT_PTR)szString;
          }
          else dwParameter=(UINT_PTR)pvtParameter->bstrVal;
        }
        else dwParameter=GetVariantInt(pvtParameter, NULL);

        *(UINT_PTR *)(lpStructure + dwOffset)=dwParameter;
        dwOffset+=sizeof(UINT_PTR);
      }
    }
  }

  //Call plugin
  {
    PLUGINCALLSENDW pcs;

    pcs.pFunction=wpFunction;
    pcs.lParam=(LPARAM)lpStructure;
    pcs.dwSupport=dwSupport;
    *nResult=(int)SendMessage(hMainWnd, AKD_DLLCALLW, (WPARAM)dwFlags, (LPARAM)&pcs);
  }

  //Free structure
  if (lpStructure) GlobalFree((HGLOBAL)lpStructure);

  //Free strings stack
  StackFreePointers(&hStringsStack);

  return hr;
}

HRESULT STDMETHODCALLTYPE Document_Exec(IDocument *this, BSTR wpCmdLine, BSTR wpWorkDir, int nWait, int nShowWindow, DWORD *dwExit)
{
  STARTUPINFOW si;
  PROCESS_INFORMATION pi;
  wchar_t *wszExecuteCommandExp;
  wchar_t *wszExecuteDirectoryExp;
  int nCommandLen;
  int nWorkDirLen;

  *dwExit=0;

  if (wpCmdLine)
  {
    nCommandLen=TranslateFileString(wpCmdLine, NULL, 0);
    nWorkDirLen=TranslateFileString(wpWorkDir, NULL, 0);

    if (wszExecuteCommandExp=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nCommandLen + 1) * sizeof(wchar_t)))
    {
      if (wszExecuteDirectoryExp=(wchar_t *)GlobalAlloc(GMEM_FIXED, (nWorkDirLen + 1) * sizeof(wchar_t)))
      {
        TranslateFileString(wpCmdLine, wszExecuteCommandExp, nCommandLen + 1);
        TranslateFileString(wpWorkDir, wszExecuteDirectoryExp, nWorkDirLen + 1);

        if (*wszExecuteCommandExp)
        {
          xmemset(&si, 0, sizeof(STARTUPINFOW));
          si.cb=sizeof(STARTUPINFOW);
          if (nShowWindow >= 0)
          {
            si.dwFlags=STARTF_USESHOWWINDOW;
            si.wShowWindow=(WORD)nShowWindow;
          }
          if (CreateProcessWide(NULL, wszExecuteCommandExp, NULL, NULL, FALSE, 0, NULL, (wszExecuteDirectoryExp && *wszExecuteDirectoryExp)?wszExecuteDirectoryExp:NULL, &si, &pi))
          {
            if (nWait == 1)
            {
              WaitForSingleObject(pi.hProcess, INFINITE);
              GetExitCodeProcess(pi.hProcess, dwExit);
            }
            else if (nWait == 2)
            {
              WaitForInputIdle(pi.hProcess, INFINITE);
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
          }
        }
        GlobalFree((HGLOBAL)wszExecuteDirectoryExp);
      }
      GlobalFree((HGLOBAL)wszExecuteCommandExp);
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_Command(IDocument *this, int nCommand, VARIANT vtLParam, VARIANT *vtResult)
{
  VARIANT vtWnd;
  VARIANT vtWParam;

  SetVariantInt(&vtWnd, (INT_PTR)hMainWnd);
  SetVariantInt(&vtWParam, nCommand);
  Document_SendMessage(this, vtWnd, WM_COMMAND, vtWParam, vtLParam, vtResult);

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_Font(IDocument *this, BSTR wpFaceName, DWORD dwFontStyle, int nPointSize)
{
  HWND hWndCurEdit;
  LOGFONTW lfNew;
  HDC hDC;

  if (!wpFaceName) wpFaceName=L"";

  if (hWndCurEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL))
  {
    if (SendMessage(hMainWnd, AKD_GETFONTW, (WPARAM)hWndCurEdit, (LPARAM)&lfNew))
    {
      if (nPointSize)
      {
        if (hDC=GetDC(hWndCurEdit))
        {
          lfNew.lfHeight=-MulDiv(nPointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
          ReleaseDC(hWndCurEdit, hDC);
        }
      }
      if (dwFontStyle != FS_NONE)
      {
        lfNew.lfWeight=(dwFontStyle == FS_FONTBOLD || dwFontStyle == FS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
        lfNew.lfItalic=(dwFontStyle == FS_FONTITALIC || dwFontStyle == FS_FONTBOLDITALIC)?TRUE:FALSE;
      }
      if (*wpFaceName != '\0')
      {
        xstrcpynW(lfNew.lfFaceName, wpFaceName, LF_FACESIZE);
      }
      SendMessage(hMainWnd, AKD_SETFONTW, (WPARAM)hWndCurEdit, (LPARAM)&lfNew);
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_Recode(IDocument *this, int nCodePageFrom, int nCodePageTo)
{
  HWND hWndCurEdit;
  TEXTRECODE tr;

  if (hWndCurEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL))
  {
    tr.nCodePageFrom=nCodePageFrom;
    tr.nCodePageTo=nCodePageTo;
    tr.dwFlags=0;
    SendMessage(hMainWnd, AKD_RECODESEL, (WPARAM)hWndCurEdit, (LPARAM)&tr);
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_Include(IDocument *this, BSTR wpFileName, DWORD dwFlags, int nCodePage, BOOL bBOM, BOOL *bResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  INCLUDEITEM *lpIncludeItem;
  wchar_t wszScriptInclude[MAX_PATH];
  wchar_t *wpContent=NULL;
  HRESULT hr=NOERROR;

  *bResult=FALSE;

  if (lpIncludeItem=StackInsertInclude(&lpScriptThread->hIncludesStack))
  {
    xstrcpynW(wszScriptInclude, lpScriptThread->wszScriptInclude, MAX_PATH);
    xprintfW(lpScriptThread->wszScriptInclude, L"%s\\AkelFiles\\Plugs\\Scripts\\Include\\%s", wszAkelPadDir, wpFileName);
    xstrcpynW(lpIncludeItem->wszInclude, lpScriptThread->wszScriptInclude, MAX_PATH);

    if (DetectAndReadFile(NULL, lpScriptThread->wszScriptInclude, dwFlags, &nCodePage, &bBOM, &wpContent, (UINT_PTR)-1))
    {
      lpScriptThread->objActiveScript->lpVtbl->SetScriptState(lpScriptThread->objActiveScript, SCRIPTSTATE_DISCONNECTED);
      if ((hr=lpScriptThread->objActiveScriptParse->lpVtbl->ParseScriptText(lpScriptThread->objActiveScriptParse, wpContent, NULL, NULL, NULL, lpScriptThread->hIncludesStack.nElements, 0, 0, NULL, NULL)) == S_OK)
        *bResult=TRUE;
      lpScriptThread->objActiveScript->lpVtbl->SetScriptState(lpScriptThread->objActiveScript, SCRIPTSTATE_CONNECTED);

      SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpContent);
    }
    xstrcpynW(lpScriptThread->wszScriptInclude, wszScriptInclude, MAX_PATH);
  }

  return hr;
}

HRESULT STDMETHODCALLTYPE Document_IsInclude(IDocument *this, BSTR *wpFileName)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HRESULT hr=NOERROR;

  if (lpScriptThread->wszScriptInclude)
  {
    if (!(*wpFileName=SysAllocString(lpScriptThread->wszScriptInclude)))
      hr=E_OUTOFMEMORY;
  }
  else *wpFileName=NULL;

  return hr;
}

HRESULT STDMETHODCALLTYPE Document_OpenFile(IDocument *this, BSTR wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, int *nResult)
{
  OPENDOCUMENTW od;

  od.pFile=wpFile;
  od.pWorkDir=NULL;
  od.dwFlags=dwFlags;
  od.nCodePage=nCodePage;
  od.bBOM=bBOM;
  od.hDoc=NULL;
  *nResult=(int)SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_ReadFile(IDocument *this, BSTR wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, VARIANT vtBytesMax, BSTR *wpText)
{
  INT_PTR nBytesMax=GetVariantInt(&vtBytesMax, NULL);
  wchar_t *wpContent=NULL;
  INT_PTR nContentLen;
  HRESULT hr=NOERROR;

  *wpText=NULL;
  nContentLen=DetectAndReadFile(NULL, wpFile, dwFlags, &nCodePage, &bBOM, &wpContent, (UINT_PTR)nBytesMax);

  if (wpContent)
  {
    if (!(*wpText=SysAllocStringLen(wpContent, (DWORD)nContentLen)))
      hr=E_OUTOFMEMORY;
    SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpContent);
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_WriteFile(IDocument *this, VARIANT vtFile, BSTR wpContent, VARIANT vtContentLen, int nCodePage, BOOL bBOM, DWORD dwFlags, int *nResult)
{
  VARIANT *pvtFile=&vtFile;
  INT_PTR nContentLen=GetVariantInt(&vtContentLen, NULL);
  UINT_PTR dwFile;
  FILECONTENT fc;
  DWORD dwCreationDisposition;
  DWORD dwAttr=INVALID_FILE_ATTRIBUTES;
  DWORD dwOffSet=0;

  dwFile=GetVariantValue(pvtFile, &pvtFile, FALSE);

  *nResult=ESD_OPEN;

  if (pvtFile->vt == VT_BSTR)
  {
    if ((dwAttr=GetFileAttributesWide((const wchar_t *)dwFile)) != INVALID_FILE_ATTRIBUTES)
    {
      if (dwFlags & WFF_WRITEREADONLY)
      {
        if ((dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
          SetFileAttributesWide((const wchar_t *)dwFile, dwAttr & ~FILE_ATTRIBUTE_READONLY & ~FILE_ATTRIBUTE_HIDDEN & ~FILE_ATTRIBUTE_SYSTEM);
      }
      else
      {
        if (dwAttr & FILE_ATTRIBUTE_READONLY)
        {
          *nResult=ESD_READONLY;
          return NOERROR;
        }
      }
    }
    if (dwFlags & WFF_APPENDFILE)
      dwCreationDisposition=OPEN_ALWAYS;
    else if (dwAttr != INVALID_FILE_ATTRIBUTES)
      dwCreationDisposition=TRUNCATE_EXISTING;
    else
      dwCreationDisposition=CREATE_NEW;

    fc.hFile=CreateFileWide((const wchar_t *)dwFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
  }
  else fc.hFile=(HANDLE)dwFile;

  if (fc.hFile != INVALID_HANDLE_VALUE)
  {
    if (dwFlags & WFF_APPENDFILE)
      dwOffSet=SetFilePointer(fc.hFile, 0, NULL, FILE_END);

    fc.wpContent=wpContent;
    fc.dwMax=nContentLen;
    fc.nCodePage=nCodePage;
    fc.bBOM=(!dwOffSet ? bBOM : FALSE);
    *nResult=(int)SendMessage(hMainWnd, AKD_WRITEFILECONTENT, 0, (LPARAM)&fc);

    if (pvtFile->vt == VT_BSTR)
    {
      CloseHandle(fc.hFile);

      if (dwAttr != INVALID_FILE_ATTRIBUTES)
      {
        if ((!(dwAttr & FILE_ATTRIBUTE_ARCHIVE) && *nResult == ESD_SUCCESS) || (dwAttr & FILE_ATTRIBUTE_READONLY) || (dwAttr & FILE_ATTRIBUTE_HIDDEN) || (dwAttr & FILE_ATTRIBUTE_SYSTEM))
          SetFileAttributesWide((const wchar_t *)dwFile, dwAttr|(*nResult == ESD_SUCCESS?FILE_ATTRIBUTE_ARCHIVE:0));
      }
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_SaveFile(IDocument *this, VARIANT vtWnd, BSTR wpFile, int nCodePage, BOOL bBOM, DWORD dwFlags, VARIANT vtDoc, int *nResult)
{
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  AEHDOC hDoc=(AEHDOC)GetVariantInt(&vtDoc, NULL);
  SAVEDOCUMENTW sd;
  EDITINFO ei;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWnd, (LPARAM)&ei))
  {
    if (nCodePage == -1)
      nCodePage=ei.nCodePage;
    if (bBOM == -1)
      bBOM=ei.bBOM;
    sd.pFile=wpFile;
    sd.nCodePage=nCodePage;
    sd.bBOM=bBOM;
    sd.dwFlags=dwFlags;
    sd.hDoc=hDoc;
    *nResult=(int)SendMessage(hMainWnd, AKD_SAVEDOCUMENTW, (WPARAM)hWnd, (LPARAM)&sd);
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_ScriptSettings(IDocument *this, IDispatch **objSet)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  IRealScriptSettings *lpSet;

  if (!objSet) return E_POINTER;

  if ((lpSet=(IRealScriptSettings *)GlobalAlloc(GPTR, sizeof(IRealScriptSettings))))
  {
    lpSet->lpVtbl=(IScriptSettingsVtbl *)&MyIScriptSettingsVtbl;
    lpSet->dwCount=1;
    lpSet->lpScriptThread=(void *)lpScriptThread;

    InterlockedIncrement(&g_nObjs);
    *objSet=(IDispatch *)lpSet;
  }
  else return E_OUTOFMEMORY;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_SystemFunction(IDocument *this, IDispatch **objSys)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  IRealSystemFunction *lpSys;

  if (!objSys) return E_POINTER;

  if ((lpSys=(IRealSystemFunction *)GlobalAlloc(GPTR, sizeof(IRealSystemFunction))))
  {
    lpSys->lpVtbl=(ISystemFunctionVtbl *)&MyISystemFunctionVtbl;
    lpSys->dwCount=1;
    lpSys->lpScriptThread=(void *)lpScriptThread;

    InterlockedIncrement(&g_nObjs);
    *objSys=(IDispatch *)lpSys;
  }
  else return E_OUTOFMEMORY;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_MemAlloc(IDocument *this, VARIANT vtSize, VARIANT *vtPointer)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  UINT_PTR dwSize=GetVariantInt(&vtSize, NULL);
  POINTERITEM *lpElement=NULL;
  INT_PTR nPointer;
  HRESULT hr=NOERROR;

  if (nPointer=(INT_PTR)GlobalAlloc(GPTR, dwSize))
  {
    if (lpElement=StackInsertPointer(&lpScriptThread->hPointersStack))
    {
      lpElement->lpData=(void *)nPointer;
      lpElement->dwSize=dwSize;
    }
  }
  else hr=E_OUTOFMEMORY;

  SetVariantInt(vtPointer, nPointer);
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_MemCopy(IDocument *this, VARIANT vtPointer, VARIANT vtData, DWORD dwType, int nDataLen, int *nBytes)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  INT_PTR nPointer=GetVariantInt(&vtPointer, NULL);
  UINT_PTR dwNumber;
  POINTERITEM *lpElement=NULL;
  VARIANT *pvtData=&vtData;
  unsigned char *lpString=NULL;
  int nStringLen;
  HRESULT hr=NOERROR;

  *nBytes=0;
  dwNumber=GetVariantValue(pvtData, &pvtData, FALSE);

  if (pvtData->vt == VT_BSTR)
  {
    #if defined(_WIN64) || (defined(SCRIPTS_MAXHANDLE) && SCRIPTS_MAXHANDLE < 0x7FFFFFFF)
      if (pvtData->bstrVal && !pvtData->bstrVal[0] && SysStringLen(pvtData->bstrVal) > 0)
      {
        //JScript doesn't support VT_I8, so __int64 number converted to string.
      }
      else
    #endif
    {
      lpString=(unsigned char *)pvtData->bstrVal;
      if (nDataLen == -1)
        nStringLen=SysStringLen(pvtData->bstrVal);
      else
        nStringLen=nDataLen;
      if (!lpString)
        lpString=(unsigned char *)L"";
    }
  }

  if (nPointer && (lpScriptThread && lpScriptThread->dwDebug & DBG_MEMWRITE))
  {
    if (!(lpElement=StackGetPointer(&lpScriptThread->hPointersStack, (void *)nPointer, 1)))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMLOCATE), nPointer);
      return E_POINTER;
    }
  }

  if (dwType == DT_ANSI)
  {
    if (lpString)
    {
      *nBytes=WideCharToMultiByte(CP_ACP, 0, (wchar_t *)lpString, nStringLen, NULL, 0, NULL, NULL);

      if (nPointer)
      {
        if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + *nBytes + sizeof(char))
        {
          xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMWRITE));
          return E_POINTER;
        }
        WideCharToMultiByte(CP_ACP, 0, (wchar_t *)lpString, nStringLen, (char *)nPointer, *nBytes, NULL, NULL);
      }
      if (nDataLen == -1)
      {
        if (nPointer) ((char *)nPointer)[*nBytes]='\0';
        *nBytes+=sizeof(char);
      }
    }
  }
  else if (dwType == DT_UNICODE)
  {
    if (lpString)
    {
      *nBytes=nStringLen * sizeof(wchar_t);

      if (nPointer)
      {
        if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + *nBytes + sizeof(wchar_t))
        {
          xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMWRITE));
          return E_POINTER;
        }
        xmemcpy((void *)nPointer, lpString, *nBytes);
      }
      if (nDataLen == -1)
      {
        if (nPointer) ((wchar_t *)nPointer)[nStringLen]=L'\0';
        *nBytes+=sizeof(wchar_t);
      }
    }
  }
  else
  {
    if (dwType == DT_QWORD)
      *nBytes=sizeof(UINT_PTR);
    else if (dwType == DT_DWORD)
      *nBytes=sizeof(DWORD);
    else if (dwType == DT_WORD)
      *nBytes=sizeof(WORD);
    else if (dwType == DT_BYTE)
      *nBytes=sizeof(BYTE);

    if (nPointer)
    {
      if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + *nBytes)
      {
        xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMWRITE));
        return E_POINTER;
      }
      xmemcpy((void *)nPointer, &dwNumber, *nBytes);
    }
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_MemRead(IDocument *this, VARIANT vtPointer, DWORD dwType, int nDataLen, VARIANT *vtData)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  INT_PTR nPointer=GetVariantInt(&vtPointer, NULL);
  POINTERITEM *lpElement=NULL;
  wchar_t *wszString;
  int nStringLen;
  HRESULT hr=NOERROR;

  if (lpScriptThread && lpScriptThread->dwDebug & DBG_MEMREAD)
  {
    if (!(lpElement=StackGetPointer(&lpScriptThread->hPointersStack, (void *)nPointer, 1)))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMLOCATE), nPointer);
      return E_POINTER;
    }
  }
  VariantInit(vtData);

  if (dwType == DT_ANSI)
  {
    if (nDataLen == -1)
      nStringLen=(int)xstrlenA((char *)nPointer);
    else
      nStringLen=nDataLen;

    if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + nStringLen)
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMREAD));
      return E_POINTER;
    }
    if (wszString=(wchar_t *)GlobalAlloc(GPTR, (nStringLen + 1) * sizeof(wchar_t)))
    {
      nStringLen=MultiByteToWideChar(CP_ACP, 0, (char *)nPointer, nStringLen, wszString, nStringLen);
      vtData->vt=VT_BSTR;
      if (!(vtData->bstrVal=SysAllocStringLen(wszString, nStringLen)))
        hr=E_OUTOFMEMORY;
      GlobalFree((HGLOBAL)wszString);
    }
    else hr=E_OUTOFMEMORY;
  }
  else if (dwType == DT_UNICODE)
  {
    if (nDataLen == -1)
      nStringLen=(int)xstrlenW((wchar_t *)nPointer);
    else
      nStringLen=nDataLen;

    if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)((wchar_t *)nPointer + nStringLen))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMREAD));
      return E_POINTER;
    }
    vtData->vt=VT_BSTR;
    if (!(vtData->bstrVal=SysAllocStringLen((wchar_t *)nPointer, nStringLen)))
      hr=E_OUTOFMEMORY;
  }
  else if (dwType == DT_QWORD)
  {
    if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + sizeof(UINT_PTR))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMREAD));
      return E_POINTER;
    }
    SetVariantInt(vtData, *(INT_PTR *)nPointer);
  }
  else if (dwType == DT_DWORD)
  {
    if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + sizeof(DWORD))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMREAD));
      return E_POINTER;
    }
    vtData->vt=VT_I4;
    vtData->lVal=(DWORD)(*(INT_PTR *)nPointer);
  }
  else if (dwType == DT_WORD)
  {
    if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + sizeof(WORD))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMREAD));
      return E_POINTER;
    }
    vtData->vt=VT_UI2;
    vtData->uiVal=(WORD)(*(UINT_PTR *)nPointer);
  }
  else if (dwType == DT_BYTE)
  {
    if (lpElement && (LPBYTE)lpElement->lpData + lpElement->dwSize < (LPBYTE)nPointer + sizeof(BYTE))
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMREAD));
      return E_POINTER;
    }
    vtData->vt=VT_UI1;
    vtData->bVal=(BYTE)(*(UINT_PTR *)nPointer);
  }
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_MemStrPtr(IDocument *this, BSTR wpString, BOOL bConvert, VARIANT *vtPointer)
{
  INT_PTR nPointer;

  if (bConvert)
    nPointer=xatoiW(wpString, NULL);
  else
    nPointer=(INT_PTR)wpString;
  SetVariantInt(vtPointer, nPointer);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_MemPtrStr(IDocument *this, VARIANT vtPointer, BSTR *wpString)
{
  INT_PTR nPointer=GetVariantInt(&vtPointer, NULL);
  HRESULT hr=NOERROR;

  xitoaW(nPointer, wszBuffer);
  if (!(*wpString=SysAllocString(wszBuffer)))
    hr=E_OUTOFMEMORY;
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_MemFree(IDocument *this, VARIANT vtPointer)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  INT_PTR nPointer=GetVariantInt(&vtPointer, NULL);
  POINTERITEM *lpElement=NULL;

  if (!(lpElement=StackGetPointer(&lpScriptThread->hPointersStack, (void *)nPointer, 1)))
  {
    if (lpScriptThread && lpScriptThread->dwDebug & DBG_MEMFREE)
    {
      xprintfW(wszErrorMsg, GetLangStringW(wLangModule, STRID_DEBUG_MEMFREE), nPointer);
      return E_POINTER;
    }
  }
  else StackDeletePointer(&lpScriptThread->hPointersStack, lpElement);

  GlobalFree((HGLOBAL)nPointer);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_DebugJIT(IDocument *this)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;

  if (lpScriptThread->bInitDebugJIT)
    lpScriptThread->objDebugApplication->lpVtbl->CauseBreak(lpScriptThread->objDebugApplication);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_Debug(IDocument *this, DWORD dwDebug, DWORD *dwResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;

  *dwResult=lpScriptThread->dwDebug;
  lpScriptThread->dwDebug=dwDebug;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_VarType(IDocument *this, VARIANT vtData, int *nType)
{
  VARIANT *pvtData=&vtData;

  if (pvtData->vt == (VT_BYREF|VT_VARIANT))
    pvtData=pvtData->pvarVal;
  *nType=pvtData->vt;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_VarDispatch(IDocument *this, VARIANT vtPointer, IDispatch **objDispatch)
{
  *objDispatch=(IDispatch *)GetVariantInt(&vtPointer, NULL);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_GetArgLine(IDocument *this, BOOL bNoEncloseQuotes, BSTR *wpArgLine)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HRESULT hr=NOERROR;
  wchar_t *wpNoQuote=lpScriptThread->wszArguments;
  int nNoQuoteLen=lpScriptThread->nArgumentsLen;

  if (lpScriptThread->wszArguments && *lpScriptThread->wszArguments)
  {
    if (bNoEncloseQuotes)
    {
      while ((wpNoQuote[0] == L'\"' || wpNoQuote[0] == L'\'' || wpNoQuote[0] == L'`') && nNoQuoteLen >= 2 && wpNoQuote[0] == wpNoQuote[nNoQuoteLen - 1])
      {
        ++wpNoQuote;
        nNoQuoteLen-=2;
      }
    }
    if (!(*wpArgLine=SysAllocStringLen(wpNoQuote, nNoQuoteLen)))
      hr=E_OUTOFMEMORY;
  }
  else *wpArgLine=NULL;

  return hr;
}

HRESULT STDMETHODCALLTYPE Document_GetArgValue(IDocument *this, BSTR wpArgName, VARIANT vtDefault, VARIANT *vtResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  SCRIPTARG *lpScriptArg;
  wchar_t *wszArgValue;
  HRESULT hr=NOERROR;

  g_bScriptArg=TRUE;
  VariantInit(vtResult);

  if (lpScriptArg=StackGetArgumentByName(&lpScriptThread->hArgStack, wpArgName, SysStringLen(wpArgName)))
  {
    if (vtDefault.vt == VT_BSTR)
    {
      vtResult->vt=VT_BSTR;
      vtResult->bstrVal=SysAllocStringLen(lpScriptArg->wpArgValue, lpScriptArg->nArgValueLen);
    }
    else
    {
      if (wszArgValue=(wchar_t *)GlobalAlloc(GPTR, (lpScriptArg->nArgValueLen + 1) * sizeof(wchar_t)))
      {
        xstrcpynW(wszArgValue, lpScriptArg->wpArgValue, lpScriptArg->nArgValueLen + 1);
        hr=lpScriptThread->objActiveScriptParse->lpVtbl->ParseScriptText(lpScriptThread->objActiveScriptParse, wszArgValue, NULL, NULL, NULL, 0, 0, SCRIPTTEXT_ISEXPRESSION, vtResult, NULL);
        GlobalFree((HGLOBAL)wszArgValue);
      }
      else hr=E_OUTOFMEMORY;
    }
  }
  else
  {
    if (vtDefault.vt != VT_ERROR)
      hr=VariantCopy(vtResult, &vtDefault);
  }
  g_bScriptArg=FALSE;
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_CreateDialog(IDocument *this, DWORD dwExStyle, VARIANT vtClassName, VARIANT vtTitle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, VARIANT vtWndParent, VARIANT vtParam, SAFEARRAY **lpItems, VARIANT *vtWnd)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  CALLBACKITEM *lpCallback;
  wchar_t *wpClassName=(wchar_t *)GetVariantInt(&vtClassName, NULL);
  wchar_t *wpTitle=(wchar_t *)GetVariantInt(&vtTitle, NULL);
  HWND hWndParent=(HWND)GetVariantInt(&vtWndParent, NULL);
  LPARAM lParam=GetVariantInt(&vtParam, NULL);
  DWORD dwFlags=0;
  HINSTANCE hInstance=NULL;
  HMENU hMenu=NULL;
  BSTR wpFaceName=NULL;
  DWORD dwFontStyle=0;
  int nPointSize=0;
  SAFEARRAY *psa=*lpItems;
  VARIANT *pvtParameter;
  unsigned char *lpData;
  DWORD dwElement;
  DWORD dwElementSum;
  HGLOBAL hTemplate;
  DLGTEMPLATEEX *lpTemplate;
  DWORD dwSize;
  INT_PTR nResult=0;
  HRESULT hr=NOERROR;

  lpData=(unsigned char *)(psa->pvData);
  dwElementSum=psa->rgsabound[0].cElements;

  for (dwElement=0; dwElement < dwElementSum; ++dwElement)
  {
    pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
    if (pvtParameter->vt == VT_BSTR && !xstrcmpW(pvtParameter->bstrVal, L"|"))
    {
      ++dwElement;
      break;
    }
    if (dwElement == 0)
      dwFlags=(DWORD)GetVariantInt(pvtParameter, NULL);
    else if (dwElement == 1)
      hMenu=(HMENU)GetVariantInt(pvtParameter, NULL);
    else if (dwElement == 2)
      hInstance=(HINSTANCE)GetVariantInt(pvtParameter, NULL);
    else if (dwElement == 3)
      wpFaceName=pvtParameter->bstrVal;
    else if (dwElement == 4)
      dwFontStyle=(DWORD)GetVariantInt(pvtParameter, NULL);
    else if (dwElement == 5)
      nPointSize=(int)GetVariantInt(pvtParameter, NULL);
    else
      return DISP_E_BADPARAMCOUNT;
  }

  //For dialog we need to change WNDCLASSW.lpfnWndProc to DefDlgProcWide
  if (lpCallback=StackGetCallbackByClass(&lpScriptThread->hDialogCallbackStack, wpClassName))
  {
    if (!lpCallback->hHandle)
    {
      //First window associated with wAtom.
      HWND hWndHidden;

      if (hWndHidden=CreateWindowExWide(0, wpClassName, L"", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstanceDLL, NULL))
      {
        SetClassLongPtrWide(hWndHidden, GCLP_WNDPROC, (UINT_PTR)DefDlgProcWide);
        DestroyWindow(hWndHidden);
      }
    }
  }
  else return CO_E_CANTDETERMINECLASS;

  if (!hInstance) hInstance=hInstanceDLL;

  if ((hr=FillDialogTemplate(NULL, dwFlags, dwExStyle, wpClassName, wpTitle, dwStyle, x, y, nWidth, nHeight, hMenu, wpFaceName, dwFontStyle, nPointSize, lpItems, dwElement, &dwSize)) == NOERROR)
  {
    if (hTemplate=GlobalAlloc(GMEM_ZEROINIT, dwSize))
    {
      if (lpTemplate=(DLGTEMPLATEEX *)GlobalLock(hTemplate))
      {
        FillDialogTemplate(lpTemplate, dwFlags, dwExStyle, wpClassName, wpTitle, dwStyle, x, y, nWidth, nHeight, hMenu, wpFaceName, dwFontStyle, nPointSize, lpItems, dwElement, NULL);
        GlobalUnlock(hTemplate);
        if (dwFlags & CDF_MODAL)
          nResult=DialogBoxIndirectParam(hInstance, (DLGTEMPLATE *)hTemplate, hWndParent, (DLGPROC)DialogCallbackProc, lParam);
        else
          nResult=(INT_PTR)CreateDialogIndirectParam(hInstance, (DLGTEMPLATE *)hTemplate, hWndParent, (DLGPROC)DialogCallbackProc, lParam);
        GlobalFree(hTemplate);
      }
    }
  }
  SetVariantInt(vtWnd, nResult);

  return hr;
}

HRESULT FillDialogTemplate(DLGTEMPLATEEX *lpdt, DWORD dwFlags, DWORD dwExStyle, wchar_t *wpClassName, wchar_t *wpTitle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HMENU hMenu, BSTR wpFaceName, DWORD dwFontStyle, int nPointSize, SAFEARRAY **lpItems, DWORD dwElement, DWORD *lpdwSize)
{
  DLGITEMTEMPLATEEX *lpdit;
  WORD *lpw;
  BYTE *lpb;
  VARIANT *pvtParameter;
  SAFEARRAY *psa=*lpItems;
  unsigned char *lpData;
  DWORD dwElementSum;
  DWORD dwOptional;
  POINT ptUnitCur={0};
  POINT ptUnit96={0};
  HRESULT hr=NOERROR;

  lpData=(unsigned char *)(psa->pvData);
  dwElementSum=psa->rgsabound[0].cElements;

  //Don't use "lpw=(WORD *)(lpdt + 1)" because of wrong alignment
  lpw=(WORD *)((BYTE *)lpdt + offsetof(DLGTEMPLATEEX, cy) + sizeof(short));

  //Menu
  if (!hMenu)
  {
    if (lpdt) *lpw=0;
    ++lpw;
  }
  else if ((INT_PTR)hMenu < 0xFFFF)
  {
    if (lpdt) *lpw=0xFFFF;
    ++lpw;
    if (lpdt) *lpw=(WORD)(INT_PTR)hMenu;
    ++lpw;
  }
  else lpw+=xstrcpynW(lpdt?(wchar_t *)lpw:NULL, (wchar_t *)hMenu, MAX_PATH) + (lpdt?1:0);

  //Class
  if (!wpClassName)
  {
    if (lpdt) *lpw=0;
    ++lpw;
  }
  else if ((INT_PTR)wpClassName < 0xFFFF)
  {
    if (lpdt) *lpw=0xFFFF;
    ++lpw;
    if (lpdt) *lpw=(WORD)(INT_PTR)wpClassName;
    ++lpw;
  }
  else lpw+=xstrcpynW(lpdt?(wchar_t *)lpw:NULL, wpClassName, MAX_PATH) + (lpdt?1:0);

  //Title
  if (!wpTitle)
  {
    if (lpdt) *lpw=0;
    ++lpw;
  }
  else lpw+=xstrcpynW(lpdt?(wchar_t *)lpw:NULL, wpTitle, MAX_PATH) + (lpdt?1:0);

  if ((dwStyle & DS_SETFONT) || (dwFlags & CDF_PIXELS))
  {
    LOGFONTW lfGui;
    HDC hDC=NULL;
    HFONT hGuiFont;

    //Default GUI font
    if (dwStyle & DS_SETFONT)
      hGuiFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
    else
      hGuiFont=(HFONT)GetStockObject(SYSTEM_FONT);
    if (bOldWindows)
    {
      LOGFONTA lfA;

      GetObjectA(hGuiFont, sizeof(LOGFONTA), &lfA);
      LogFontAtoW(&lfA, &lfGui);
    }
    else GetObjectW(hGuiFont, sizeof(LOGFONTW), &lfGui);

    if (lpdt)
    {
      if (dwFlags & CDF_PIXELS)
      {
        if (!hDC)
          hDC=GetDC(hMainWnd);
        if (hDC)
          GetDialogUnits(hDC, hGuiFont, &ptUnitCur, &ptUnit96);
      }
      if (dwStyle & DS_SETFONT)
      {
        if (!nPointSize)
        {
          if (!hDC)
            hDC=GetDC(hMainWnd);
          if (hDC)
            nPointSize=MulDiv(mod(lfGui.lfHeight), 72, GetDeviceCaps(hDC, LOGPIXELSY));
        }
      }
      if (hDC)
      {
        ReleaseDC(hMainWnd, hDC);
        hDC=NULL;
      }
      if (dwFontStyle != FS_NONE)
      {
        lfGui.lfWeight=(dwFontStyle == FS_FONTBOLD || dwFontStyle == FS_FONTBOLDITALIC)?FW_BOLD:FW_NORMAL;
        lfGui.lfItalic=(dwFontStyle == FS_FONTITALIC || dwFontStyle == FS_FONTBOLDITALIC)?TRUE:FALSE;
      }
    }
    if (wpFaceName && *wpFaceName != '\0')
    {
      xstrcpynW(lfGui.lfFaceName, wpFaceName, LF_FACESIZE);
    }

    if (dwStyle & DS_SETFONT)
    {
      //Point size
      if (lpdt) *lpw=(WORD)nPointSize;
      ++lpw;

      //Weight
      if (lpdt) *lpw=(WORD)lfGui.lfWeight;
      ++lpw;

      if (lpdt)
      {
        lpb=(BYTE *)lpw;

        //Italic
        *lpb++=lfGui.lfItalic;

        //Character set
        *lpb++=lfGui.lfCharSet /*DEFAULT_CHARSET*/;
      }
      ++lpw;

      //Face name
      lpw+=xstrcpynW(lpdt?(wchar_t *)lpw:NULL, lfGui.lfFaceName, LF_FACESIZE) + (lpdt?1:0);
    }
  }

  if (lpdt)
  {
    lpdt->dlgVer=1;
    lpdt->signature=0xFFFF;
    lpdt->helpID=0;
    lpdt->exStyle=dwExStyle;
    lpdt->style=dwStyle;
    lpdt->cDlgItems=0;
    lpdt->x=(short)x;
    if (dwFlags & CDF_PIXELS)
      lpdt->x=(short)PixelToUnitX(lpdt->x, &ptUnitCur, &ptUnit96);
    lpdt->y=(short)y;
    if (dwFlags & CDF_PIXELS)
      lpdt->y=(short)PixelToUnitY(lpdt->y, &ptUnitCur, &ptUnit96);
    lpdt->cx=(short)nWidth;
    if (dwFlags & CDF_PIXELS)
      lpdt->cx=(short)PixelToUnitX(lpdt->cx, &ptUnitCur, &ptUnit96);
    lpdt->cy=(short)nHeight;
    if (dwFlags & CDF_PIXELS)
      lpdt->cy=(short)PixelToUnitY(lpdt->cy, &ptUnitCur, &ptUnit96);
  }

  while (dwElement < dwElementSum)
  {
    lpw=(WORD *)AlignValue(lpw, sizeof(DWORD));
    lpdit=(DLGITEMTEMPLATEEX *)lpw;
    lpw=(WORD *)(lpdit + 1);

    //helpID
    if (lpdt) lpdit->helpID=0;

    //ExStyle
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->exStyle=(DWORD)GetVariantInt(pvtParameter, NULL);
    }
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    //Class
    pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
    wpClassName=(wchar_t *)GetVariantInt(pvtParameter, NULL);
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    if (!wpClassName)
    {
      if (lpdt) *lpw=0;
      ++lpw;
    }
    else if ((INT_PTR)wpClassName < 0xFFFF)
    {
      if (lpdt) *lpw=0xFFFF;
      ++lpw;
      if (lpdt) *lpw=(WORD)(INT_PTR)wpClassName;
      ++lpw;
    }
    else lpw+=xstrcpynW(lpdt?(wchar_t *)lpw:NULL, wpClassName, MAX_PATH) + (lpdt?1:0);

    //Title
    pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
    wpTitle=(wchar_t *)GetVariantInt(pvtParameter, NULL);
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    if (!wpTitle)
    {
      if (lpdt) *lpw=0;
      ++lpw;
    }
    else if ((INT_PTR)wpTitle < 0xFFFF)
    {
      if (lpdt) *lpw=0xFFFF;
      ++lpw;
      if (lpdt) *lpw=(WORD)(INT_PTR)wpTitle;
      ++lpw;
    }
    else lpw+=xstrcpynW(lpdt?(wchar_t *)lpw:NULL, wpTitle, MAX_PATH) + (lpdt?1:0);

    //Style
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->style=(DWORD)GetVariantInt(pvtParameter, NULL);
    }
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    //x
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->x=(short)GetVariantInt(pvtParameter, NULL);
      if (dwFlags & CDF_PIXELS)
        lpdit->x=(short)PixelToUnitX(lpdit->x, &ptUnitCur, &ptUnit96);
    }
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    //y
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->y=(short)GetVariantInt(pvtParameter, NULL);
      if (dwFlags & CDF_PIXELS)
        lpdit->y=(short)PixelToUnitY(lpdit->y, &ptUnitCur, &ptUnit96);
    }
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    //Width
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->cx=(short)GetVariantInt(pvtParameter, NULL);
      if (dwFlags & CDF_PIXELS)
        lpdit->cx=(short)PixelToUnitX(lpdit->cx, &ptUnitCur, &ptUnit96);
    }
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    //Height
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->cy=(short)GetVariantInt(pvtParameter, NULL);
      if (dwFlags & CDF_PIXELS)
        lpdit->cy=(short)PixelToUnitY(lpdit->cy, &ptUnitCur, &ptUnit96);
    }
    if (++dwElement >= dwElementSum) return DISP_E_BADPARAMCOUNT;

    //ID
    if (lpdt)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      lpdit->id=(DWORD)GetVariantInt(pvtParameter, NULL);
    }
    ++dwElement;

    //Creation data
    if (lpdt) *lpw=0;
    ++lpw;

    //Optional item parameters
    for (dwOptional=0; dwElement < dwElementSum; ++dwElement, ++dwOptional)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      if (pvtParameter->vt == VT_BSTR && !xstrcmpW(pvtParameter->bstrVal, L"|"))
      {
        ++dwElement;
        break;
      }
      if (dwOptional == 0)
      {
        //Creation data
        LPARAM lParamItem;
        WORD wParamItemSize;

        if ((lParamItem=GetVariantInt(pvtParameter, NULL)) && (wParamItemSize=*(WORD *)lParamItem) > 0)
        {
          --lpw;
          if (lpdt) xmemcpy(lpw, (void *)lParamItem, wParamItemSize);
          lpw=(WORD *)((BYTE *)lpw + wParamItemSize);
        }
      }
      else return DISP_E_BADPARAMCOUNT;
    }

    if (lpdt) ++lpdt->cDlgItems;
  }
  if (lpdwSize) *lpdwSize=(DWORD)((BYTE *)lpw - (BYTE *)lpdt);
  return hr;
}

BOOL GetDialogUnits(HDC hDC, HFONT hFont, POINT *ptUnitCur, POINT *ptUnit96)
{
  TEXTMETRICA tmGui;
  HFONT hFontOld;
  SIZE sizeWidth;
  const char *pStr="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int nStrLen=52;
  BOOL bFreeDC=FALSE;
  BOOL bResult=FALSE;

  if (!hDC)
  {
    hDC=GetDC(hMainWnd);
    bFreeDC=TRUE;
  }
  if (!hFont)
    hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
  if (hDC)
  {
    hFontOld=(HFONT)SelectObject(hDC, hFont);
    if (GetTextMetricsA(hDC, &tmGui))
    {
      //Current dialog base unit
      GetTextExtentPoint32A(hDC, pStr, nStrLen, &sizeWidth);
      ptUnitCur->x=(sizeWidth.cx / 26 + 1) / 2;
      ptUnitCur->y=tmGui.tmHeight;

      //Normal unit (without screen scale)
      ptUnit96->x=MulDiv(ptUnitCur->x, 96, GetDeviceCaps(hDC, LOGPIXELSX));
      ptUnit96->y=MulDiv(ptUnitCur->y, 96, GetDeviceCaps(hDC, LOGPIXELSY));

      bResult=TRUE;
    }
    if (hFontOld) SelectObject(hDC, hFontOld);
    if (bFreeDC) ReleaseDC(hMainWnd, hDC);
  }
  return bResult;
}

int PixelToUnitX(int x, const POINT *ptUnitCur, const POINT *ptUnit96)
{
  if (ptUnitCur->x)
  {
    x=MulDiv(x, ptUnitCur->x, ptUnit96->x);
    x=MulDiv(x, 4, ptUnitCur->x);
  }
  return x;
}

int PixelToUnitY(int y, const POINT *ptUnitCur, const POINT *ptUnit96)
{
  if (ptUnitCur->y)
  {
    y=MulDiv(y, ptUnitCur->y, ptUnit96->y);
    y=MulDiv(y, 8, ptUnitCur->y);
  }
  return y;
}

LOGFONTW* LogFontAtoW(const LOGFONTA *lfA, LOGFONTW *lfW)
{
  xmemcpy(lfW, lfA, offsetof(LOGFONTW, lfFaceName));
  MultiByteToWideChar(CP_ACP, 0, lfA->lfFaceName, -1, lfW->lfFaceName, LF_FACESIZE);
  return lfW;
}

LOGFONTA* LogFontWtoA(const LOGFONTW *lfW, LOGFONTA *lfA)
{
  xmemcpy(lfA, lfW, offsetof(LOGFONTW, lfFaceName));
  WideCharToMultiByte(CP_ACP, 0, lfW->lfFaceName, -1, lfA->lfFaceName, LF_FACESIZE, NULL, NULL);
  return lfA;
}

HRESULT STDMETHODCALLTYPE Document_WindowRegisterClass(IDocument *this, BSTR wpClassName, SAFEARRAY **psa, WORD *wAtom)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  CALLBACKITEM *lpCallback;
  WNDCLASSW wndclass;

  wndclass.style        =0;
  wndclass.lpfnWndProc  =DialogCallbackProc;
  wndclass.cbClsExtra   =0;
  wndclass.cbWndExtra   =DLGWINDOWEXTRA;
  wndclass.hInstance    =hInstanceDLL;
  wndclass.hIcon        =NULL;
  wndclass.hCursor      =LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground=(HBRUSH)(UINT_PTR)(COLOR_BTNFACE + 1);
  wndclass.lpszMenuName =NULL;
  wndclass.lpszClassName=wpClassName;
  if (*wAtom=RegisterClassWide(&wndclass))
  {
    if (lpCallback=StackInsertCallback(&lpScriptThread->hDialogCallbackStack, NULL))
    {
      lpCallback->hHandle=(HANDLE)NULL;
      lpCallback->dwData=*wAtom;
      lpCallback->lpScriptThread=(void *)lpScriptThread;
      lpCallback->nCallbackType=CIT_DIALOG;
      lpCallback->wpClassName=SysAllocString(wpClassName);

      StackFillMessages(&lpCallback->hMsgIntStack, *psa);
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowUnregisterClass(IDocument *this, BSTR wpClassName, BOOL *bResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  CALLBACKITEM *lpCallback;
  CALLBACKITEM *lpCount;
  CALLBACKITEM *lpNextCount;
  ATOM wAtom;

  if (lpCallback=StackGetCallbackByClass(&lpScriptThread->hDialogCallbackStack, wpClassName))
  {
    wAtom=(ATOM)lpCallback->dwData;

    if (*bResult=UnregisterClassWide((wchar_t *)(UINT_PTR)wAtom, hInstanceDLL))
    {
      for (lpCount=lpScriptThread->hDialogCallbackStack.first; lpCount; lpCount=lpNextCount)
      {
        lpNextCount=lpCount->next;
        if (lpCount->wpClassName && lpCount->dwData == wAtom)
          StackDeleteCallback(lpCount);
      }
    }
  }

  return NOERROR;
}


HRESULT STDMETHODCALLTYPE Document_WindowRegisterDialog(IDocument *this, VARIANT vtDlg, BOOL *bResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HWND hDlg=(HWND)GetVariantInt(&vtDlg, NULL);
  CALLBACKITEM *lpCallback;

  *bResult=FALSE;

  if (!StackGetCallbackByHandle(&lpScriptThread->hDialogCallbackStack, hDlg, lpScriptThread))
  {
    if (lpCallback=StackInsertCallback(&lpScriptThread->hDialogCallbackStack, NULL))
    {
      lpCallback->hHandle=(HANDLE)hDlg;
      lpCallback->lpScriptThread=(void *)lpScriptThread;
      lpCallback->nCallbackType=CIT_DIALOG;
      *bResult=TRUE;
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowUnregisterDialog(IDocument *this, VARIANT vtDlg, BOOL *bResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HWND hDlg=(HWND)GetVariantInt(&vtDlg, NULL);
  CALLBACKITEM *lpCallback;

  *bResult=FALSE;

  if (lpCallback=StackGetCallbackByHandle(&lpScriptThread->hDialogCallbackStack, hDlg, lpScriptThread))
  {
    StackDeleteCallback(lpCallback);
    *bResult=TRUE;
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowGetMessage(IDocument *this, DWORD dwFlags)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  CALLBACKITEM *lpCallback;
  MSG msg;

  lpScriptThread->dwMessageLoop=WGM_ENABLE|dwFlags;

  while (GetMessageWide(&msg, NULL, 0, 0) > 0)
  {
    //Global hotkeys
    if (TranslateMessageProc)
    {
      if (TranslateMessageProc(TMSG_GLOBAL|TMSG_HOTKEYGLOBAL, &msg))
        continue;
    }
    else if (TranslateAcceleratorWide(hMainWnd, hGlobalAccel, &msg))
      continue;

    //Dialog message
    for (lpCallback=lpScriptThread->hDialogCallbackStack.first; lpCallback; lpCallback=lpCallback->next)
    {
      if (lpCallback->hHandle && IsDialogMessageWide((HWND)lpCallback->hHandle, &msg))
      {
        if (!(lpScriptThread->dwMessageLoop & WGM_NOKEYSEND) && (HWND)lpCallback->hHandle != msg.hwnd &&
             (!(lpScriptThread->dwMessageLoop & WGM_KEYDOWNUP) ? (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) :
                                                                 (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)))
        {
          SendMessage((HWND)lpCallback->hHandle, msg.message, msg.wParam, msg.lParam);
        }
        break;
      }
    }

    //Regular message
    if (!lpCallback)
    {
      TranslateMessage(&msg);
      DispatchMessageWide(&msg);
    }
  }
  lpScriptThread->dwMessageLoop=0;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowSubClass(IDocument *this, VARIANT vtWnd, IDispatch *objFunction, SAFEARRAY **psa, VARIANT *vtCallbackItem)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  CALLBACKITEM *lpCallback=NULL;
  WNDPROC lpOldWndProc;
  UINT_PTR dwData;
  int nCallbackType=0;

  if (hWnd && hWnd != hMainWnd)
  {
    if ((INT_PTR)hWnd == WSC_MAINPROC)
    {
      if (!g_lpSubclassMainProc)
        SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)SubclassMainProc, (LPARAM)&g_lpSubclassMainProc);
      if (g_lpSubclassMainProc)
      {
        dwData=(UINT_PTR)g_lpSubclassMainProc;
        nCallbackType=CIT_MAINPROC;
      }
    }
    else if ((INT_PTR)hWnd == WSC_EDITPROC)
    {
      if (!g_lpSubclassEditProc)
        SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)SubclassEditProc, (LPARAM)&g_lpSubclassEditProc);
      if (g_lpSubclassEditProc)
      {
        dwData=(UINT_PTR)g_lpSubclassEditProc;
        nCallbackType=CIT_EDITPROC;
      }
    }
    else if ((INT_PTR)hWnd == WSC_FRAMEPROC)
    {
      if (nMDI == WMD_MDI)
      {
        if (!g_lpSubclassFrameProc)
          SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)SubclassFrameProc, (LPARAM)&g_lpSubclassFrameProc);
        if (g_lpSubclassFrameProc)
        {
          dwData=(UINT_PTR)g_lpSubclassFrameProc;
          nCallbackType=CIT_FRAMEPROC;
        }
      }
    }
    else
    {
      if (lpOldWndProc=(WNDPROC)GetWindowLongPtrWide(hWnd, GWLP_WNDPROC))
      {
        SetWindowLongPtrWide(hWnd, GWLP_WNDPROC, (UINT_PTR)SubclassCallbackProc);
        dwData=(UINT_PTR)lpOldWndProc;
        nCallbackType=CIT_SUBCLASS;
      }
    }

    if (nCallbackType)
    {
      if (lpCallback=StackInsertCallback(&g_hSubclassCallbackStack, objFunction))
      {
        lpCallback->hHandle=(HANDLE)hWnd;
        lpCallback->dwData=dwData;
        lpCallback->nCallbackType=nCallbackType;
        lpCallback->lpScriptThread=(void *)lpScriptThread;

        if (!lpScriptThread->hWndScriptsThreadDummy)
        {
          lpScriptThread->hWndScriptsThreadDummy=CreateScriptsThreadDummyWindow();
        }
        StackFillMessages(&lpCallback->hMsgIntStack, *psa);
      }
    }
  }
  SetVariantInt(vtCallbackItem, (INT_PTR)lpCallback);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowNextProc(IDocument *this, VARIANT vtCallbackItem, VARIANT vtWnd, UINT uMsg, VARIANT vtWParam, VARIANT vtLParam, VARIANT *vtResult)
{
  CALLBACKITEM *lpCallback=(CALLBACKITEM *)GetVariantInt(&vtCallbackItem, NULL);
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);
  VARIANT *pvtWParam=&vtWParam;
  VARIANT *pvtLParam=&vtLParam;
  CALLBACKITEM *lpNextCallback;
  BOOL bCalled=FALSE;
  WPARAM wParam;
  LPARAM lParam;
  LRESULT lResult=0;

  wParam=GetVariantValue(pvtWParam, &pvtWParam, bOldWindows);
  lParam=GetVariantValue(pvtLParam, &pvtLParam, bOldWindows);

  if (lpCallback && lpCallback->hHandle)
  {
    lpNextCallback=lpCallback->next;

    if (lpNextCallback && lpNextCallback->hHandle)
    {
      if ((INT_PTR)lpCallback->hHandle == WSC_MAINPROC)
        bCalled=SubclassMainCall(lpNextCallback, hWnd, uMsg, wParam, lParam, &lResult);
      else if ((INT_PTR)lpCallback->hHandle == WSC_EDITPROC)
        bCalled=SubclassEditCall(lpNextCallback, hWnd, uMsg, wParam, lParam, &lResult);
      else if ((INT_PTR)lpCallback->hHandle == WSC_FRAMEPROC)
        bCalled=SubclassFrameCall(lpNextCallback, hWnd, uMsg, wParam, lParam, &lResult);
      else
        bCalled=SubclassCallbackCall(lpNextCallback, hWnd, uMsg, wParam, lParam, &lResult);
    }
    if (!bCalled)
    {
      if ((INT_PTR)lpCallback->hHandle == WSC_MAINPROC)
        lResult=g_lpSubclassMainProc->NextProc(hWnd, uMsg, wParam, lParam);
      else if ((INT_PTR)lpCallback->hHandle == WSC_EDITPROC)
        lResult=g_lpSubclassEditProc->NextProc(hWnd, uMsg, wParam, lParam);
      else if ((INT_PTR)lpCallback->hHandle == WSC_FRAMEPROC)
        lResult=g_lpSubclassFrameProc->NextProc(hWnd, uMsg, wParam, lParam);
      else
        lResult=CallWindowProcWide((WNDPROC)lpCallback->dwData, hWnd, uMsg, wParam, lParam);
    }
  }
  if (bOldWindows)
  {
    if (pvtWParam->vt == VT_BSTR) GlobalFree((HGLOBAL)wParam);
    if (pvtLParam->vt == VT_BSTR) GlobalFree((HGLOBAL)lParam);
  }
  SetVariantInt(vtResult, lResult);
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowNoNextProc(IDocument *this, VARIANT vtCallbackItem)
{
  CALLBACKITEM *lpCallback=(CALLBACKITEM *)GetVariantInt(&vtCallbackItem, NULL);

  lpCallback->bNoNextProc=TRUE;
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_WindowUnsubClass(IDocument *this, VARIANT vtWnd)
{
  void *lpScriptThread=((IRealDocument *)this)->lpScriptThread;
  HWND hWnd=(HWND)GetVariantInt(&vtWnd, NULL);

  return WindowUnsubClass(lpScriptThread, hWnd);
}

HRESULT WindowUnsubClass(void *lpScriptThread, HWND hWnd)
{
  CALLBACKITEM *lpCallback;

  if (hWnd)
  {
    if ((INT_PTR)hWnd == WSC_MAINPROC)
    {
      lpCallback=StackGetCallbackByHandle(&g_hSubclassCallbackStack, (HANDLE)(INT_PTR)WSC_MAINPROC, lpScriptThread);

      if (g_lpSubclassMainProc && StackGetCallbackCount(&g_hSubclassCallbackStack, CIT_MAINPROC) <= 1)
      {
        SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&g_lpSubclassMainProc);
        g_lpSubclassMainProc=NULL;
      }
    }
    else if ((INT_PTR)hWnd == WSC_EDITPROC)
    {
      lpCallback=StackGetCallbackByHandle(&g_hSubclassCallbackStack, (HANDLE)(INT_PTR)WSC_EDITPROC, lpScriptThread);

      if (g_lpSubclassEditProc && StackGetCallbackCount(&g_hSubclassCallbackStack, CIT_EDITPROC) <= 1)
      {
        SendMessage(hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&g_lpSubclassEditProc);
        g_lpSubclassEditProc=NULL;
      }
    }
    else if ((INT_PTR)hWnd == WSC_FRAMEPROC)
    {
      lpCallback=StackGetCallbackByHandle(&g_hSubclassCallbackStack, (HANDLE)(INT_PTR)WSC_FRAMEPROC, lpScriptThread);

      if (g_lpSubclassFrameProc && StackGetCallbackCount(&g_hSubclassCallbackStack, CIT_FRAMEPROC) <= 1)
      {
        SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&g_lpSubclassFrameProc);
        g_lpSubclassFrameProc=NULL;
      }
    }
    else
    {
      if (lpCallback=StackGetCallbackByHandle(&g_hSubclassCallbackStack, hWnd, lpScriptThread))
        SetWindowLongPtrWide(hWnd, GWLP_WNDPROC, lpCallback->dwData);
    }
    if (lpCallback) StackDeleteCallback(lpCallback);
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_ThreadHook(IDocument *this, int idHook, IDispatch *objCallback, DWORD dwThreadId, SAFEARRAY **psa, VARIANT *vtHook)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HRESULT hr=NOERROR;
  HHOOK hHook=NULL;

  if (objCallback)
  {
    CALLBACKITEM *lpCallback;
    HOOKPROC lpHookProc;
    int nBusyIndex;

    if ((nBusyIndex=RetriveCallbackProc(g_cbHook)) >= 0)
    {
      if (lpCallback=StackInsertCallback(&g_hHookCallbackStack, objCallback))
      {
        lpHookProc=(HOOKPROC)g_cbHook[nBusyIndex].lpProc;

        if (hHook=SetWindowsHookEx(idHook, lpHookProc, NULL, dwThreadId))
        {
          g_cbHook[nBusyIndex].bBusy=TRUE;
          lpCallback->nBusyIndex=nBusyIndex;
          lpCallback->lpProc=(INT_PTR)lpHookProc;
          lpCallback->hHandle=(HANDLE)hHook;
          //dwData is the offset to message identifier
          lpCallback->dwData=(UINT_PTR)-1;
          if (idHook == WH_CALLWNDPROC)
             lpCallback->dwData=offsetof(CWPSTRUCT, message);
          else if (idHook == WH_CALLWNDPROCRET)
             lpCallback->dwData=offsetof(CWPRETSTRUCT, message);
          else if (idHook == WH_GETMESSAGE ||
                   idHook == WH_MSGFILTER ||
                   idHook == WH_SYSMSGFILTER)
          {
             lpCallback->dwData=offsetof(MSG, message);
          }
          lpCallback->nCallbackType=CIT_HOOKCALLBACK;
          lpCallback->lpScriptThread=(void *)lpScriptThread;

          if (!lpScriptThread->hWndScriptsThreadDummy)
          {
            lpScriptThread->hWndScriptsThreadDummy=CreateScriptsThreadDummyWindow();
          }
          StackFillMessages(&lpCallback->hMsgIntStack, *psa);
        }
      }
    }
    else
    {
      lpHookProc=NULL;
      hr=DISP_E_BADINDEX;
    }
  }
  SetVariantInt(vtHook, (INT_PTR)hHook);
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_ThreadUnhook(IDocument *this, VARIANT vtHook, BOOL *bResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  HHOOK hHook=(HHOOK)GetVariantInt(&vtHook, NULL);
  CALLBACKITEM *lpCallback;
  int nBusyIndex;

  if (lpCallback=StackGetCallbackByHandle(&g_hHookCallbackStack, (HANDLE)hHook, lpScriptThread))
  {
    if (*bResult=UnhookWindowsHookEx(hHook))
    {
      nBusyIndex=lpCallback->nBusyIndex;
      if (StackDeleteCallback(lpCallback))
        g_cbHook[nBusyIndex].bBusy=FALSE;
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_ScriptNoMutex(IDocument *this, DWORD dwUnlockType, DWORD *dwResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;

  *dwResult=0;

  if (dwUnlockType & ULT_LOCKMULTICOPY)
  {
    lpScriptThread->bSingleCopy=TRUE;
    *dwResult|=ULT_LOCKMULTICOPY;
  }
  if (dwUnlockType & ULT_UNLOCKMULTICOPY)
  {
    lpScriptThread->bSingleCopy=FALSE;
    *dwResult|=ULT_UNLOCKMULTICOPY;
  }

  if (dwUnlockType & ULT_LOCKSENDMESSAGE)
  {
    lpScriptThread->bLockSendMessage=TRUE;
    *dwResult|=ULT_LOCKSENDMESSAGE;
  }
  if (dwUnlockType & ULT_UNLOCKSENDMESSAGE)
  {
    lpScriptThread->bLockSendMessage=FALSE;
    *dwResult|=ULT_UNLOCKSENDMESSAGE;
  }

  if (dwUnlockType & ULT_UNLOCKSCRIPTSQUEUE)
  {
    if (lpScriptThread->hExecMutex)
    {
      SetEvent(lpScriptThread->hExecMutex);
      CloseHandle(lpScriptThread->hExecMutex);
      lpScriptThread->hExecMutex=NULL;
      *dwResult|=ULT_UNLOCKSCRIPTSQUEUE;
    }
  }
  if (dwUnlockType & ULT_UNLOCKPROGRAMTHREAD)
  {
    if (lpScriptThread->hInitMutex)
    {
      SetEvent(lpScriptThread->hInitMutex);
      lpScriptThread->hInitMutex=NULL;
      *dwResult|=ULT_UNLOCKPROGRAMTHREAD;
    }
  }
  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_ScriptExitCode(IDocument *this, VARIANT vtExitCode, BOOL *bResult)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  INT_PTR nExitCode=GetVariantValue(&vtExitCode, NULL, FALSE);

  if (lpScriptThread && lpScriptThread->pcs)
  {
    lpScriptThread->pcs->nResult=nExitCode;
    *bResult=TRUE;
  }
  else *bResult=FALSE;

  return NOERROR;
}

HRESULT STDMETHODCALLTYPE Document_ScriptHandle(IDocument *this, VARIANT vtData, int nOperation, VARIANT *vtResult)
{
  UINT_PTR dwData=GetVariantValue(&vtData, NULL, FALSE);
  INT_PTR nResult=0;
  HRESULT hr=NOERROR;

  VariantInit(vtResult);

  if (nOperation == SH_FIRSTSCRIPT)
  {
    nResult=(INT_PTR)hThreadStack.first;
  }
  else if (nOperation == SH_THISSCRIPT)
  {
    nResult=(INT_PTR)((IRealDocument *)this)->lpScriptThread;
  }
  else if (nOperation == SH_FINDSCRIPT)
  {
    const wchar_t *wpScriptName=(const wchar_t *)dwData;

    if (wpScriptName && *wpScriptName)
      nResult=(INT_PTR)StackGetScriptThreadByName(&hThreadStack, wpScriptName);
  }
  else
  {
    SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)dwData;

    if (!lpScriptThread)
      lpScriptThread=((IRealDocument *)this)->lpScriptThread;

    if (lpScriptThread)
    {
      if (nOperation == SH_GETEXECTYPE)
        nResult=(INT_PTR)lpScriptThread->nExecType;
      else if (nOperation == SH_GETTHREADHANDLE)
        nResult=(INT_PTR)lpScriptThread->hThread;
      else if (nOperation == SH_GETTHREADID)
        nResult=lpScriptThread->dwThreadID;
      else if (nOperation == SH_GETMESSAGELOOP)
        nResult=lpScriptThread->dwMessageLoop;
      else if (nOperation == SH_GETLOCKMULTICOPY)
        nResult=lpScriptThread->bSingleCopy;
      else if (nOperation == SH_GETLOCKSCRIPTSQUEUE)
        nResult=lpScriptThread->hExecMutex?TRUE:FALSE;
      else if (nOperation == SH_GETLOCKPROGRAMTHREAD)
        nResult=lpScriptThread->hInitMutex?TRUE:FALSE;
      else if (nOperation == SH_GETSERVICEWINDOW)
        nResult=(INT_PTR)lpScriptThread->hWndScriptsThreadDummy;
      else if (nOperation == SH_GETLOCKSENDMESSAGE)
        nResult=lpScriptThread->bLockSendMessage;
      else if (nOperation == SH_GETBASENAME)
      {
        vtResult->vt=VT_BSTR;
        if (!(vtResult->bstrVal=SysAllocString(lpScriptThread->wszScriptBaseName)))
          hr=E_OUTOFMEMORY;
        return hr;
      }
      else if (nOperation == SH_GETNAME)
      {
        vtResult->vt=VT_BSTR;
        if (!(vtResult->bstrVal=SysAllocString(lpScriptThread->wszScriptName)))
          hr=E_OUTOFMEMORY;
        return hr;
      }
      else if (nOperation == SH_GETFILE)
      {
        vtResult->vt=VT_BSTR;
        if (!(vtResult->bstrVal=SysAllocString(lpScriptThread->wszScriptFile)))
          hr=E_OUTOFMEMORY;
        return hr;
      }
      else if (nOperation == SH_GETNCLUDE)
      {
        vtResult->vt=VT_BSTR;
        if (!(vtResult->bstrVal=SysAllocString(lpScriptThread->wszScriptInclude)))
          hr=E_OUTOFMEMORY;
        return hr;
      }
      else if (nOperation == SH_GETARGUMENTS)
      {
        vtResult->vt=VT_BSTR;
        if (!(vtResult->bstrVal=SysAllocStringLen(lpScriptThread->wszArguments, lpScriptThread->nArgumentsLen)))
          hr=E_OUTOFMEMORY;
        return hr;
      }
      else if (nOperation == SH_NEXTSCRIPT)
      {
        nResult=(INT_PTR)lpScriptThread->next;
      }
      else if (nOperation == SH_NEXTSAMESCRIPT)
      {
        SCRIPTTHREAD *lpScriptThreadNext;

        for (lpScriptThreadNext=lpScriptThread->next; lpScriptThreadNext; lpScriptThreadNext=lpScriptThreadNext->next)
        {
          if (!xstrcmpiW(lpScriptThreadNext->wszScriptName, lpScriptThread->wszScriptName))
          {
            nResult=(INT_PTR)lpScriptThreadNext;
            break;
          }
        }
      }
      else if (nOperation == SH_CLOSESCRIPT)
      {
        if (lpScriptThread->hWndScriptsThreadDummy && lpScriptThread->dwMessageLoop)
          SendMessage(lpScriptThread->hWndScriptsThreadDummy, AKDLL_POSTQUIT, 0, 0);
      }
    }
  }
  SetVariantInt(vtResult, nResult);
  return hr;
}

HRESULT STDMETHODCALLTYPE Document_ConnectObject(IDocument *this, IDispatch *objConnectTo, BSTR wpPrefix, VARIANT vtIID, int *nCount)
{
  return WScript_ConnectObject((IWScript *)this, objConnectTo, wpPrefix, vtIID, nCount);
}

HRESULT STDMETHODCALLTYPE Document_DisconnectObject(IDocument *this, IDispatch *objConnectTo)
{
  return WScript_DisconnectObject((IWScript *)this, objConnectTo);
}

HRESULT STDMETHODCALLTYPE Document_ActiveXThis(IDocument *this, IDispatch *objThis)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;
  wchar_t *wpWScript=L"WScript";
  wchar_t *wpScriptFullName=L"ScriptFullName";
  VARIANT vtWScript;
  VARIANT vtScriptFullName;
  DISPID dispid;
  DISPPARAMS dispp;
  HRESULT hr=NOERROR;

  if (!lpScriptThread->objActiveScript)
  {
    //Get script file from WScript.ScriptFullName
    xmemset(&dispp, 0, sizeof(DISPPARAMS));
    VariantInit(&vtWScript);
    VariantInit(&vtScriptFullName);
    if (!(hr=objThis->lpVtbl->GetIDsOfNames(objThis, &IID_NULL, &wpWScript, 1, LOCALE_USER_DEFAULT, &dispid)))
    {
      if (!(hr=objThis->lpVtbl->Invoke(objThis, dispid, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispp, &vtWScript, 0, 0)))
      {
        if (vtWScript.vt == VT_DISPATCH)
        {
          if (!(hr=vtWScript.pdispVal->lpVtbl->GetIDsOfNames(vtWScript.pdispVal, &IID_NULL, &wpScriptFullName, 1, LOCALE_USER_DEFAULT, &dispid)))
          {
            if (!(hr=vtWScript.pdispVal->lpVtbl->Invoke(vtWScript.pdispVal, dispid, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispp, &vtScriptFullName, 0, 0)))
            {
              if (vtScriptFullName.vt == VT_BSTR)
              {
                const wchar_t *wpFileName;

                wpFileName=GetFileNameWide(vtScriptFullName.bstrVal, -1);
                xstrcpynW(lpScriptThread->wszScriptName, wpFileName, MAX_PATH);
                GetBaseName(lpScriptThread->wszScriptName, -1, lpScriptThread->wszScriptBaseName, MAX_PATH);
                xstrcpynW(lpScriptThread->wszScriptFile, vtScriptFullName.bstrVal, MAX_PATH);
              }
            }
          }
        }
      }
    }
    VariantClear(&vtWScript);
    VariantClear(&vtScriptFullName);
    lpScriptThread->objThis=objThis;
  }
  return hr;
}

HWND GetCurEdit(IDocument *this)
{
  SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)((IRealDocument *)this)->lpScriptThread;

  if (lpScriptThread && lpScriptThread->hWndPluginEdit)
    return lpScriptThread->hWndPluginEdit;
  return (HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL);
}

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%a -AkelPad directory, %% -%
  wchar_t *wpExeDir=wszAkelPadDir;
  wchar_t *wszSource;
  wchar_t *wpSource;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=wszBuffer + (wszBuffer?nBufferSize:0x7FFFFFFF);
  int nStringLen;

  //Expand environment strings
  nStringLen=ExpandEnvironmentStringsWide(wpString, NULL, 0);

  if (wszSource=(wchar_t *)GlobalAlloc(GPTR, nStringLen * sizeof(wchar_t)))
  {
    ExpandEnvironmentStringsWide(wpString, wszSource, nStringLen);

    //Expand plugin variables
    for (wpSource=wszSource; *wpSource && wpTarget < wpTargetMax;)
    {
      if (*wpSource == '%')
      {
        if (*++wpSource == '%')
        {
          ++wpSource;
          if (wszBuffer) *wpTarget='%';
          ++wpTarget;
        }
        else if (*wpSource == 'a' || *wpSource == 'A')
        {
          ++wpSource;
          wpTarget+=xstrcpynW(wszBuffer?wpTarget:NULL, wpExeDir, wpTargetMax - wpTarget) - !wszBuffer;
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
        *wpTarget='\0';
      else
        ++wpTarget;
    }
    GlobalFree((HGLOBAL)wszSource);
  }
  return (int)(wpTarget - wszBuffer);
}

INCLUDEITEM* StackInsertInclude(INCLUDESTACK *hStack)
{
  INCLUDEITEM *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(INCLUDEITEM)))
    ++hStack->nElements;

  return lpElement;
}

INCLUDEITEM* StackGetInclude(INCLUDESTACK *hStack, DWORD dwIndex)
{
  INCLUDEITEM *lpElement;
  DWORD dwCount=0;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (++dwCount == dwIndex)
      break;
  }
  return lpElement;
}

void StackFreeIncludes(INCLUDESTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

POINTERITEM* StackInsertPointer(POINTERSTACK *hStack)
{
  POINTERITEM *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(POINTERITEM)))
    ++hStack->nElements;

  return lpElement;
}

POINTERITEM* StackGetPointer(POINTERSTACK *hStack, void *lpData, INT_PTR nRange)
{
  POINTERITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if ((LPBYTE)lpData >= (LPBYTE)lpElement->lpData &&
        (LPBYTE)lpData + nRange <= (LPBYTE)lpElement->lpData + lpElement->dwSize)
    {
      break;
    }
  }
  return lpElement;
}

void StackDeletePointer(POINTERSTACK *hStack, POINTERITEM *lpPointer)
{
  if (!StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpPointer))
    --hStack->nElements;
}

void StackFreePointers(POINTERSTACK *hStack)
{
  POINTERITEM *lpElement=(POINTERITEM *)hStack->first;

  while (lpElement)
  {
    if (lpElement->lpData) GlobalFree((HGLOBAL)lpElement->lpData);

    lpElement=lpElement->next;
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

MSGINT* StackInsertMessage(MSGINTSTACK *hStack, UINT uMsg)
{
  MSGINT *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(MSGINT)))
  {
    lpElement->uMsg=uMsg;
    ++hStack->nElements;
  }
  return lpElement;
}

MSGINT* StackGetMessage(MSGINTSTACK *hStack, UINT uMsg)
{
  MSGINT *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->uMsg == uMsg)
      return lpElement;
  }
  return NULL;
}

void StackDeleteMessage(MSGINTSTACK *hStack, MSGINT *lpMessage)
{
  if (!StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpMessage))
    --hStack->nElements;
}

void StackFreeMessages(MSGINTSTACK *hStack)
{
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

void StackFillMessages(MSGINTSTACK *hStack, SAFEARRAY *psa)
{
  VARIANT *pvtParameter;
  unsigned char *lpData;
  DWORD dwElement;
  DWORD dwElementSum;

  lpData=(unsigned char *)(psa->pvData);
  dwElementSum=psa->rgsabound[0].cElements;

  if (dwElementSum)
  {
    for (dwElement=0; dwElement < dwElementSum; ++dwElement)
    {
      pvtParameter=(VARIANT *)(lpData + dwElement * sizeof(VARIANT));
      StackInsertMessage(hStack, (UINT)GetVariantInt(pvtParameter, NULL));
    }
  }
}

int RetriveCallbackProc(CALLBACKBUSYNESS *cb)
{
  int nIndex;

  for (nIndex=0; cb[nIndex].lpProc; ++nIndex)
  {
    if (!cb[nIndex].bBusy)
      return nIndex;
  }
  return -1;
}

CALLBACKITEM* StackInsertCallback(CALLBACKSTACK *hStack, IDispatch *objCallback)
{
  CALLBACKITEM *lpElement;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, 1, sizeof(CALLBACKITEM)))
  {
    if (objCallback)
      objCallback->lpVtbl->AddRef(objCallback);
    lpElement->objFunction=objCallback;
    lpElement->nRefCount=1;
    lpElement->lpStack=hStack;
    lpElement->nBusyIndex=-1;
    ++hStack->nElements;
  }
  return lpElement;
}

int StackGetCallbackCount(CALLBACKSTACK *hStack, int nCallbackType)
{
  CALLBACKITEM *lpElement;
  int nCount=0;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->nCallbackType == nCallbackType)
      ++nCount;
  }
  return nCount;
}

BOOL StackIsCallback(CALLBACKSTACK *hStack, CALLBACKITEM *lpCallback)
{
  CALLBACKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement == lpCallback)
      return TRUE;
  }
  return FALSE;
}

CALLBACKITEM* StackGetCallbackByHandle(CALLBACKSTACK *hStack, HANDLE hHandle, void *lpScriptThread)
{
  CALLBACKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->hHandle == hHandle && (!lpScriptThread || lpElement->lpScriptThread == lpScriptThread))
      break;
  }
  return lpElement;
}

CALLBACKITEM* StackGetCallbackByProc(CALLBACKSTACK *hStack, INT_PTR lpProc)
{
  CALLBACKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->lpProc == lpProc)
      break;
  }
  return lpElement;
}

CALLBACKITEM* StackGetCallbackByObject(CALLBACKSTACK *hStack, IDispatch *objFunction)
{
  CALLBACKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->objFunction == objFunction)
      break;
  }
  return lpElement;
}

CALLBACKITEM* StackGetCallbackByData(CALLBACKSTACK *hStack, UINT_PTR dwData)
{
  CALLBACKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->dwData == dwData)
      break;
  }
  return lpElement;
}

CALLBACKITEM* StackGetCallbackByClass(CALLBACKSTACK *hStack, const wchar_t *wpClassName)
{
  CALLBACKITEM *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wpClassName, wpClassName))
      break;
  }
  return lpElement;
}

BOOL StackDeleteCallback(CALLBACKITEM *lpElement)
{
  if (--lpElement->nRefCount <= 0)
  {
    CALLBACKSTACK *hStack=(CALLBACKSTACK *)lpElement->lpStack;

    if (lpElement->objFunction)
      lpElement->objFunction->lpVtbl->Release(lpElement->objFunction);
    StackFreeMessages(&lpElement->hMsgIntStack);
    if (lpElement->wpClassName)
      SysFreeString(lpElement->wpClassName);

    if (!StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpElement))
      --hStack->nElements;
    return TRUE;
  }
  return FALSE;
}

void StackFreeCallbacks(CALLBACKSTACK *hStack)
{
  CALLBACKITEM *lpElement;
  CALLBACKITEM *lpNextElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpNextElement)
  {
    lpNextElement=lpElement->next;

    if (lpElement->nCallbackType == CIT_MAINPROC ||
        lpElement->nCallbackType == CIT_EDITPROC ||
        lpElement->nCallbackType == CIT_FRAMEPROC)
    {
      WindowUnsubClass(lpElement->lpScriptThread, lpElement->hHandle);
    }
    else StackFreeMessages(&lpElement->hMsgIntStack);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

LRESULT CALLBACK DialogCallbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  SCRIPTTHREAD *lpScriptThread=StackGetScriptThreadCurrent();
  CALLBACKITEM *lpCallback=NULL;
  LRESULT lResult=0;

  if (uMsg == WM_NCCREATE ||
      uMsg == WM_INITDIALOG)
  {
    if (lParam)
    {
      CREATESTRUCTA *cs=(CREATESTRUCTA *)lParam;
      IDispatch *objCallback=NULL;
      CALLBACKITEM *lpNewCallback;
      ATOM wAtom;

      if (uMsg == WM_NCCREATE)
        objCallback=(IDispatch *)cs->lpCreateParams;
      else if (uMsg == WM_INITDIALOG)
        objCallback=(IDispatch *)lParam;

      if (objCallback)
      {
        //Check that class registered by WindowRegisterClass.
        wAtom=(ATOM)GetClassLongA(hWnd, GCW_ATOM);

        if (lpCallback=StackGetCallbackByData(&lpScriptThread->hDialogCallbackStack, wAtom))
        {
          if (!lpCallback->hHandle)
          {
            //First window associated with wAtom.
            //Fill empty callback allocated in WindowRegisterClass.
            objCallback->lpVtbl->AddRef(objCallback);
            lpCallback->objFunction=objCallback;
            lpCallback->hHandle=(HANDLE)hWnd;
            lpCallback->bDlgProc=(uMsg == WM_INITDIALOG);
          }
          else
          {
            //Second, third, etc. window associated with wAtom.
            if (lpNewCallback=StackInsertCallback(&lpScriptThread->hDialogCallbackStack, objCallback))
            {
              lpNewCallback->hHandle=(HANDLE)hWnd;
              lpNewCallback->dwData=wAtom;
              lpNewCallback->lpScriptThread=(void *)lpScriptThread;
              lpNewCallback->nCallbackType=CIT_DIALOG;
              lpNewCallback->bDlgProc=(uMsg == WM_INITDIALOG);

              //Copy message filter
              StackCopy((stack *)lpCallback->hMsgIntStack.first, (stack *)lpCallback->hMsgIntStack.last, (stack **)&lpNewCallback->hMsgIntStack.first, (stack **)&lpNewCallback->hMsgIntStack.last, sizeof(MSGINT));
              lpNewCallback->hMsgIntStack.nElements=lpCallback->hMsgIntStack.nElements;
            }
            lpCallback=lpNewCallback;
          }
        }
      }
    }
    SendMessage(hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hPluginIcon);
  }
  else
  {
    if (lpCallback=StackGetCallbackByHandle(&lpScriptThread->hDialogCallbackStack, hWnd, lpScriptThread))
    {
      if (uMsg == WM_NCDESTROY)
        lpCallback->hHandle=NULL;
    }
  }

  if (lpCallback)
  {
    if (lpCallback->objFunction)
    {
      if (!lpCallback->hMsgIntStack.nElements || StackGetMessage(&lpCallback->hMsgIntStack, uMsg))
      {
        //Message procedure in script
        lpScriptThread->bBusy=TRUE;
        CallScriptProc(lpCallback->objFunction, hWnd, uMsg, wParam, lParam, &lResult);
        lpScriptThread->bBusy=FALSE;
      }
    }
    if (lResult) return lResult;
    if (lpCallback->bDlgProc) return FALSE;
  }

  return DefWindowProcWide(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK SubclassCallbackProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  SubclassCallbackCall(NULL, hWnd, uMsg, wParam, lParam, &lResult);
  return lResult;
}

BOOL CALLBACK SubclassCallbackCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  CALLBACKITEM *lpNextCallback;
  BOOL bNextProc=FALSE;

  if (!lpCallback)
    lpCallback=g_hSubclassCallbackStack.first;

  for (; lpCallback; lpCallback=lpNextCallback)
  {
    lpNextCallback=lpCallback->next;

    if (lpCallback->hHandle == hWnd)
    {
      ++lpCallback->nRefCount;
      *lResult=SubclassSend(lpCallback, hWnd, uMsg, wParam, lParam);

      if (lpCallback->bNoNextProc)
      {
        lpCallback->bNoNextProc=FALSE;
        StackDeleteCallback(lpCallback);
        return TRUE;
      }

      //Assign lpNextCallback again cause after SubclassSend lpCallback->next could be changed.
      lpNextCallback=lpCallback->next;
      if (!StackDeleteCallback(lpCallback))
        bNextProc=TRUE;
    }
  }

  if (bNextProc)
  {
    for (lpCallback=g_hSubclassCallbackStack.last; lpCallback; lpCallback=lpCallback->prev)
    {
      if (lpCallback->hHandle == hWnd)
        break;
    }
    if (lpCallback)
    {
      *lResult=CallWindowProcWide((WNDPROC)lpCallback->dwData, hWnd, uMsg, wParam, lParam);
      return TRUE;
    }
  }
  *lResult=0;
  return FALSE;
}

LRESULT CALLBACK SubclassMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  SubclassMainCall(NULL, hWnd, uMsg, wParam, lParam, &lResult);
  return lResult;
}

BOOL CALLBACK SubclassMainCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  CALLBACKITEM *lpNextCallback;
  BOOL bNextProc=FALSE;

  if (!lpCallback)
    lpCallback=g_hSubclassCallbackStack.first;

  for (; lpCallback; lpCallback=lpNextCallback)
  {
    lpNextCallback=lpCallback->next;

    if (lpCallback->hHandle == (HANDLE)(INT_PTR)WSC_MAINPROC)
    {
      if (uMsg == AKDN_OPENDOCUMENT_START)
      {
        //Some threads queue problem. Binary file message in main thread could block sending AKDN_OPENDOCUMENT_START to Script.js.
        //CmdLineBegin=/Call("Scripts::Main", 2, "Script.js")
        Sleep(0);
      }
      ++lpCallback->nRefCount;
      *lResult=SubclassSend(lpCallback, hWnd, uMsg, wParam, lParam);

      if (lpCallback->bNoNextProc)
      {
        lpCallback->bNoNextProc=FALSE;
        StackDeleteCallback(lpCallback);
        return TRUE;
      }

      //Assign lpNextCallback again cause after SubclassSend lpCallback->next could be changed.
      lpNextCallback=lpCallback->next;
      if (!StackDeleteCallback(lpCallback))
        bNextProc=TRUE;
    }
  }

  if (bNextProc)
  {
    *lResult=g_lpSubclassMainProc->NextProc(hWnd, uMsg, wParam, lParam);
    return TRUE;
  }
  *lResult=0;
  return FALSE;
}

LRESULT CALLBACK SubclassEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  SubclassEditCall(NULL, hWnd, uMsg, wParam, lParam, &lResult);
  return lResult;
}

BOOL CALLBACK SubclassEditCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  CALLBACKITEM *lpNextCallback;
  BOOL bNextProc=FALSE;

  if (!lpCallback)
    lpCallback=g_hSubclassCallbackStack.first;

  for (; lpCallback; lpCallback=lpNextCallback)
  {
    lpNextCallback=lpCallback->next;

    if (lpCallback->hHandle == (HANDLE)(INT_PTR)WSC_EDITPROC)
    {
      ++lpCallback->nRefCount;
      *lResult=SubclassSend(lpCallback, hWnd, uMsg, wParam, lParam);

      if (lpCallback->bNoNextProc)
      {
        lpCallback->bNoNextProc=FALSE;
        StackDeleteCallback(lpCallback);
        return TRUE;
      }

      //Assign lpNextCallback again cause after SubclassSend lpCallback->next could be changed.
      lpNextCallback=lpCallback->next;
      if (!StackDeleteCallback(lpCallback))
        bNextProc=TRUE;
    }
  }

  if (bNextProc)
  {
    *lResult=g_lpSubclassEditProc->NextProc(hWnd, uMsg, wParam, lParam);
    return TRUE;
  }
  *lResult=0;
  return FALSE;
}

LRESULT CALLBACK SubclassFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  SubclassFrameCall(NULL, hWnd, uMsg, wParam, lParam, &lResult);
  return lResult;
}

BOOL CALLBACK SubclassFrameCall(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  CALLBACKITEM *lpNextCallback;
  BOOL bNextProc=FALSE;

  if (!lpCallback)
    lpCallback=g_hSubclassCallbackStack.first;

  for (; lpCallback; lpCallback=lpNextCallback)
  {
    lpNextCallback=lpCallback->next;

    if (lpCallback->hHandle == (HANDLE)(INT_PTR)WSC_FRAMEPROC)
    {
      ++lpCallback->nRefCount;
      *lResult=SubclassSend(lpCallback, hWnd, uMsg, wParam, lParam);

      if (lpCallback->bNoNextProc)
      {
        lpCallback->bNoNextProc=FALSE;
        StackDeleteCallback(lpCallback);
        return TRUE;
      }

      //Assign lpNextCallback again cause after SubclassSend lpCallback->next could be changed.
      lpNextCallback=lpCallback->next;
      if (!StackDeleteCallback(lpCallback))
        bNextProc=TRUE;
    }
  }

  if (bNextProc)
  {
    *lResult=g_lpSubclassFrameProc->NextProc(hWnd, uMsg, wParam, lParam);
    return TRUE;
  }
  *lResult=0;
  return FALSE;
}

LRESULT CALLBACK SubclassSend(CALLBACKITEM *lpCallback, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult=0;

  //Because objFunction->lpVtbl->Invoke cause error for different thread, we send message from this thread to hWndScriptsThreadDummy.
  if (!lpCallback->hMsgIntStack.nElements || StackGetMessage(&lpCallback->hMsgIntStack, uMsg))
  {
    SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)lpCallback->lpScriptThread;
    MSGSEND msgs;

    msgs.lpCallback=lpCallback;
    msgs.hWnd=hWnd;
    msgs.uMsg=uMsg;
    msgs.wParam=wParam;
    msgs.lParam=lParam;
    lResult=SendMessage(lpScriptThread->hWndScriptsThreadDummy, AKDLL_SUBCLASSSEND, 0, (LPARAM)&msgs);
  }
  return lResult;
}

LRESULT CALLBACK HookCallback1Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(1, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback2Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(2, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback3Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(3, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback4Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(4, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback5Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(5, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback6Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(6, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback7Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(7, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback8Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(8, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback9Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(9, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback10Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(10, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback11Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(11, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback12Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(12, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback13Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(13, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback14Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(14, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback15Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(15, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback16Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(16, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback17Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(17, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback18Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(18, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback19Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(19, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback20Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(20, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback21Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(21, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback22Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(22, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback23Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(23, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback24Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(24, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback25Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(25, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback26Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(26, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback27Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(27, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback28Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(28, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback29Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(29, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallback30Proc(int nCode, WPARAM wParam, LPARAM lParam)
{
  return HookCallbackCommonProc(30, nCode, wParam, lParam);
}

LRESULT CALLBACK HookCallbackCommonProc(int nCallbackIndex, int nCode, WPARAM wParam, LPARAM lParam)
{
  CALLBACKITEM *lpCallback=StackGetCallbackByProc(&g_hHookCallbackStack, g_cbHook[nCallbackIndex - 1].lpProc);

  if (lpCallback)
  {
    if (nCode >= 0)
    {
      if (!lpCallback->hMsgIntStack.nElements || lpCallback->dwData == (UINT_PTR)-1 || StackGetMessage(&lpCallback->hMsgIntStack, *(UINT *)((BYTE *)lParam + lpCallback->dwData)))
      {
        SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)lpCallback->lpScriptThread;
        MSGSEND msgs;

        if (lpScriptThread->hWndScriptsThreadDummy)
        {
          //Because objFunction->lpVtbl->Invoke cause error for different thread, we send message from this thread to hWndScriptsThreadDummy.
          msgs.lpCallback=lpCallback;
          msgs.hWnd=NULL;
          msgs.uMsg=nCode;
          msgs.wParam=wParam;
          msgs.lParam=lParam;
          SendMessage(lpScriptThread->hWndScriptsThreadDummy, AKDLL_HOOKSEND, 0, (LPARAM)&msgs);
        }
      }
    }
    return CallNextHookEx((HHOOK)lpCallback->hHandle, nCode, wParam, lParam);
  }
  return 0;
}

HWND CreateScriptsThreadDummyWindow()
{
  WNDCLASSW wndclass;

  wndclass.style        =0;
  wndclass.lpfnWndProc  =ScriptsThreadProc;
  wndclass.cbClsExtra   =0;
  wndclass.cbWndExtra   =DLGWINDOWEXTRA;
  wndclass.hInstance    =hInstanceDLL;
  wndclass.hIcon        =NULL;
  wndclass.hCursor      =NULL;
  wndclass.hbrBackground=NULL;
  wndclass.lpszMenuName =NULL;
  wndclass.lpszClassName=L"ScriptsThread";
  RegisterClassWide(&wndclass);

  return CreateWindowExWide(0, L"ScriptsThread", L"", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstanceDLL, NULL);
}

LRESULT CALLBACK ScriptsThreadProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDLL_SUBCLASSSEND)
  {
    MSGSEND *msgs=(MSGSEND *)lParam;
    SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)msgs->lpCallback->lpScriptThread;
    LRESULT lResult=0;

    if (lpScriptThread)
    {
      //Now we in plugin thread - call procedure in script
      lpScriptThread->bBusy=TRUE;
      CallScriptProc(msgs->lpCallback->objFunction, (HWND)msgs->hWnd, msgs->uMsg, msgs->wParam, msgs->lParam, &lResult);
      lpScriptThread->bBusy=FALSE;
    }
    return lResult;
  }
  else if (uMsg == AKDLL_HOOKSEND)
  {
    MSGSEND *msgs=(MSGSEND *)lParam;
    SCRIPTTHREAD *lpScriptThread;
    LRESULT lResult=0;

    //Execution can come here after Document_ThreadUnhook was called
    if (StackIsCallback(&g_hHookCallbackStack, msgs->lpCallback))
    {
      //Now we in plugin thread - call procedure in script
      lpScriptThread=(SCRIPTTHREAD *)msgs->lpCallback->lpScriptThread;
      lpScriptThread->bBusy=TRUE;
      CallScriptProc(msgs->lpCallback->objFunction, NULL, msgs->uMsg, msgs->wParam, msgs->lParam, &lResult);
      lpScriptThread->bBusy=FALSE;
    }
    return lResult;
  }
  else if (uMsg == AKDLL_CALLBACKSEND)
  {
    MSGSEND *msgs=(MSGSEND *)lParam;
    SCRIPTTHREAD *lpScriptThread=(SCRIPTTHREAD *)msgs->lpCallback->lpScriptThread;
    DISPPARAMS *dispp=(DISPPARAMS *)msgs->lParam;
    VARIANT vtInvoke;
    LRESULT lResult=0;

    //Now we in plugin thread - call procedure in script
    lpScriptThread->bBusy=TRUE;

    if (msgs->lpCallback->objFunction->lpVtbl->Invoke(msgs->lpCallback->objFunction, DISPID_VALUE, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, dispp, &vtInvoke, 0, 0) == S_OK)
    {
      lResult=(int)GetVariantInt(&vtInvoke, NULL);
    }

    lpScriptThread->bBusy=FALSE;
    return lResult;
  }
  else if (uMsg == AKDLL_POSTQUIT)
  {
    PostQuitMessage(0);
  }

  return DefWindowProcWide(hWnd, uMsg, wParam, lParam);
}

HRESULT CallScriptProc(IDispatch *objFunction, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lResult)
{
  DISPPARAMS dispp;
  VARIANT vtArg[4];
  VARIANT vtInvoke;
  HRESULT hr;

  //if (g_MainMessageBox)
  //  return E_FAIL;
  if (!objFunction)
    return E_FAIL;

  SetVariantInt(&vtArg[0], lParam);
  SetVariantInt(&vtArg[1], wParam);
  SetVariantInt(&vtArg[2], uMsg);
  if (hWnd)
    SetVariantInt(&vtArg[3], (INT_PTR)hWnd);

  xmemset(&dispp, 0, sizeof(DISPPARAMS));
  dispp.cArgs=hWnd?4:3;
  dispp.rgvarg=vtArg;

  if ((hr=objFunction->lpVtbl->Invoke(objFunction, DISPID_VALUE, &IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispp, &vtInvoke, 0, 0)) == S_OK)
  {
    *lResult=(LRESULT)GetVariantInt(&vtInvoke, NULL);
  }
  return hr;
}

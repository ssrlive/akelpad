#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>
#include <commctrl.h>
#include <richedit.h>
#include "Scripts.h"


//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"

//Include stack functions
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackDelete
#define StackJoin
#define StackClear
#include "StackFunc.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemcmp
#define xmemset
#define xstrcmpiW
#define xstrcmpinA
#define xstrcmpinW
#define xstrlenW
#define xstrcpyW
#define xstrcpynW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define hex2decW
#define xprintfW
#define xstrstrW
#include "StrFunc.h"

//Include wide functions
#define CallWindowProcWide
#define CreateFileWide
#define CreateProcessWide
#define CreateWindowExWide
#define DefWindowProcWide
#define DialogBoxParamWide
#define DialogBoxWide
#define DispatchMessageWide
#define ExpandEnvironmentStringsWide
#define FileExistsAnsi
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define GetFileAttributesWide
#define GetKeyNameTextWide
#define GetMessageWide
#define GetModuleFileNameWide
#define GetModuleHandleWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define IsDialogMessageWide
#define ListView_GetItemWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define LoadLibraryWide
#define RegisterClassWide
#define SetDlgItemTextWide
#define SetFileAttributesWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#define TranslateAcceleratorWide
#define UnregisterClassWide
#include "WideFunc.h"

//Global variables
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
HINSTANCE hInstanceEXE=NULL;
HINSTANCE hInstanceDLL=NULL;
HWND hMainWnd=NULL;
HACCEL hGlobalAccel;
BOOL bOldWindows;
BOOL bOldRichEdit;
BOOL bOldComctl32;
BOOL bAkelEdit;
int nMDI;
LANGID wLangModule;
HICON g_hPluginIcon=NULL;
BOOL g_MainMessageBox=FALSE;
BOOL bInitCommon=FALSE;
BOOL bInitMain=FALSE;
BOOL bMainOnFinish=FALSE;
DWORD dwSaveFlags=0;
HWND hWndMainDlg=NULL;
HWND hWndScriptsList=NULL;
WNDPROC lpOldFilterProc=NULL;
RECT rcMainMinMaxDialog={253, 337, 0, 0};
RECT rcMainCurrentDialog={0};
int nColumnWidth1=163;
int nColumnWidth2=109;
int nColumnWidth3=70;
BOOL bOpeningDlg=FALSE;
BOOL bContentFilterEnable=FALSE;
DWORD dwGlobalDebugJIT=JIT_FROMSTART;
BOOL bGlobalDebugEnable=FALSE;
DWORD dwGlobalDebugCode=0;
const char *pMutexInitName="AkelPad::Scripts::MutexInit";
const char *pMutexExecName="AkelPad::Scripts::MutexExec";
wchar_t wszScriptsDir[MAX_PATH];
wchar_t wszAkelPadDir[MAX_PATH];
wchar_t wszErrorMsg[BUFFER_SIZE]=L"";
wchar_t wszLastScript[MAX_PATH]=L"";
wchar_t wszFilter[MAX_PATH]=L"";
wchar_t wszContentFilter[MAX_PATH]=L"";
HTHREADSTACK hThreadStack={0};
SCRIPTTHREAD *lpScriptThreadActiveX=NULL;
HANDLE hExecThread=NULL;
DWORD dwExecThreadId=0;
UINT_PTR dwWaitScriptTimerId=0;
UINT_PTR dwWaitScriptTimerStart=0;
WNDPROCDATA *NewMainProcData=NULL;

//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 8, 8, 0);
  pv->pPluginName="Scripts";
}

//Plugin extern function
void __declspec(dllexport) Main(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);
  if (!bInitMain) InitMain();

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_SCRIPTS_EXEC ||
        nAction == DLLA_SCRIPTS_EXECWAIT ||
        nAction == DLLA_SCRIPTS_EDIT)
    {
      unsigned char *pScript=NULL;
      unsigned char *pArguments=NULL;
      wchar_t *wpScript=NULL;
      wchar_t *wpArguments=NULL;
      int nScriptLen=0;
      int nArgumentsLen=0;
      BOOL bWait=FALSE;

      if (nAction == DLLA_SCRIPTS_EXECWAIT)
        bWait=TRUE;

      if (IsExtCallParamValid(pd->lParam, 2))
        pScript=(unsigned char *)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        pArguments=(unsigned char *)GetExtCallParam(pd->lParam, 3);

      if (pScript)
      {
        if (pd->dwSupport & PDS_STRANSI)
        {
          if (nScriptLen=lstrlenA((char *)pScript))
          {
            if (wpScript=(wchar_t *)GlobalAlloc(GPTR, (nScriptLen + 1) * sizeof(wchar_t)))
              MultiByteToWideChar(CP_ACP, 0, (char *)pScript, nScriptLen + 1, wpScript, nScriptLen + 1);
          }
          if (nArgumentsLen=lstrlenA((char *)pArguments))
          {
            if (wpArguments=(wchar_t *)GlobalAlloc(GPTR, (nArgumentsLen + 1) * sizeof(wchar_t)))
              MultiByteToWideChar(CP_ACP, 0, (char *)pArguments, nArgumentsLen + 1, wpArguments, nArgumentsLen + 1);
          }
        }
        else
        {
          wpScript=(wchar_t *)pScript;
          wpArguments=(wchar_t *)pArguments;
        }
      }
      if ((!wpScript || !*wpScript) && *wszLastScript)
        wpScript=wszLastScript;

      if (wpScript && *wpScript)
      {
        if (nAction == DLLA_SCRIPTS_EDIT)
          EditScript(wpScript);
        else
          ExecScript(wpScript, wpArguments, bWait);

        //if (xstrcmpiW(wpScript, wszLastScript))
        //{
        //  xstrcpynW(wszLastScript, wpScript, MAX_PATH);
        //  SaveOptions(OF_LASTSCRIPT);
        //}
      }

      if (nScriptLen) GlobalFree((HGLOBAL)wpScript);
      if (nArgumentsLen) GlobalFree((HGLOBAL)wpArguments);
    }
  }
  else
  {
    if (!pd->bOnStart)
    {
      DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_SCRIPTS), hMainWnd, (DLGPROC)MainDlgProc);
    }
  }

  //Stay in memory, and show as active
  pd->nUnload=UD_NONUNLOAD_ACTIVE;
}

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndScriptsFilter;
  static HWND hWndScriptsContentFilterEnable;
  static HWND hWndScriptsContentFilterButton;
  static HWND hWndChangeListGroup;
  static HWND hWndExecButton;
  static HWND hWndEditButton;
  static HWND hWndHotkey;
  static HWND hWndAssignButton;
  static HWND hWndDebugGroup;
  static HWND hWndDebugJITCheck;
  static HWND hWndDebugJITFromStartCheck;
  static HWND hWndDebugCodeCheck;
  static HWND hWndDebugCodeButton;
  static HWND hWndCloseButton;
  static int nSelItem=-1;
  static BOOL bListChanged=FALSE;
  static DIALOGRESIZE drs[]={{&hWndScriptsList,                DRS_SIZE|DRS_X, 0},
                             {&hWndScriptsList,                DRS_SIZE|DRS_Y, 0},
                             {&hWndScriptsFilter,              DRS_SIZE|DRS_X, 0},
                             {&hWndScriptsFilter,              DRS_MOVE|DRS_Y, 0},
                             {&hWndScriptsContentFilterEnable, DRS_MOVE|DRS_X, 0},
                             {&hWndScriptsContentFilterEnable, DRS_MOVE|DRS_Y, 0},
                             {&hWndScriptsContentFilterButton, DRS_MOVE|DRS_X, 0},
                             {&hWndScriptsContentFilterButton, DRS_MOVE|DRS_Y, 0},
                             {&hWndChangeListGroup,            DRS_MOVE|DRS_X, 0},
                             {&hWndExecButton,                 DRS_MOVE|DRS_X, 0},
                             {&hWndEditButton,                 DRS_MOVE|DRS_X, 0},
                             {&hWndHotkey,                     DRS_MOVE|DRS_X, 0},
                             {&hWndAssignButton,               DRS_MOVE|DRS_X, 0},
                             {&hWndDebugGroup,                 DRS_MOVE|DRS_X, 0},
                             {&hWndDebugJITCheck,              DRS_MOVE|DRS_X, 0},
                             {&hWndDebugJITFromStartCheck,     DRS_MOVE|DRS_X, 0},
                             {&hWndDebugCodeCheck,             DRS_MOVE|DRS_X, 0},
                             {&hWndDebugCodeButton,            DRS_MOVE|DRS_X, 0},
                             {&hWndCloseButton,                DRS_MOVE|DRS_X, 0},
                             {&hWndCloseButton,                DRS_MOVE|DRS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    LVCOLUMNW lvc;

    bOpeningDlg=TRUE;
    hWndMainDlg=hDlg;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hPluginIcon);
    hWndScriptsList=GetDlgItem(hDlg, IDC_SCRIPTS_LIST);
    hWndScriptsFilter=GetDlgItem(hDlg, IDC_SCRIPTS_FILTER);
    hWndScriptsContentFilterEnable=GetDlgItem(hDlg, IDC_SCRIPTS_CONTENTFILTER_ENABLE);
    hWndScriptsContentFilterButton=GetDlgItem(hDlg, IDC_SCRIPTS_CONTENTFILTER_BUTTON);
    hWndChangeListGroup=GetDlgItem(hDlg, IDC_CHANGELIST_GROUP);
    hWndExecButton=GetDlgItem(hDlg, IDC_EXEC);
    hWndEditButton=GetDlgItem(hDlg, IDC_EDIT);
    hWndHotkey=GetDlgItem(hDlg, IDC_HOTKEY);
    hWndAssignButton=GetDlgItem(hDlg, IDC_ASSIGN);
    hWndDebugGroup=GetDlgItem(hDlg, IDC_DEBUG_GROUP);
    hWndDebugJITCheck=GetDlgItem(hDlg, IDC_DEBUG_JIT_CHECK);
    hWndDebugJITFromStartCheck=GetDlgItem(hDlg, IDC_DEBUG_JITFROMSTART_CHECK);
    hWndDebugCodeCheck=GetDlgItem(hDlg, IDC_DEBUG_CODE_CHECK);
    hWndDebugCodeButton=GetDlgItem(hDlg, IDC_DEBUG_CODE_BUTTON);
    hWndCloseButton=GetDlgItem(hDlg, IDC_CLOSE);

    SetWindowTextWide(hDlg, wszPluginTitle);
    SetDlgItemTextWide(hDlg, IDC_EXEC, GetLangStringW(wLangModule, STRID_EXEC));
    SetDlgItemTextWide(hDlg, IDC_EDIT, GetLangStringW(wLangModule, STRID_EDIT));
    SetDlgItemTextWide(hDlg, IDC_ASSIGN, GetLangStringW(wLangModule, STRID_ASSIGN));
    SetDlgItemTextWide(hDlg, IDC_DEBUG_GROUP, GetLangStringW(wLangModule, STRID_DEBUG));
    SetDlgItemTextWide(hDlg, IDC_DEBUG_JIT_CHECK, GetLangStringW(wLangModule, STRID_JIT));
    SetDlgItemTextWide(hDlg, IDC_DEBUG_JITFROMSTART_CHECK, GetLangStringW(wLangModule, STRID_FROMSTART));
    SetDlgItemTextWide(hDlg, IDC_CLOSE, GetLangStringW(wLangModule, STRID_CLOSE));

    EnableWindow(hWndExecButton, FALSE);
    EnableWindow(hWndEditButton, FALSE);
    EnableWindow(hWndHotkey, FALSE);
    EnableWindow(hWndAssignButton, FALSE);
    SendMessage(hWndScriptsList, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndHotkey, 0);

    if (bContentFilterEnable)
      SendMessage(hWndScriptsContentFilterEnable, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndScriptsContentFilterButton, FALSE);
    //Set button text
    xprintfW(wszBuffer, L"%s%s", GetLangStringW(wLangModule, STRID_CONTAIN), *wszContentFilter?L" *":L"");
    SetWindowTextWide(hWndScriptsContentFilterButton, wszBuffer);

    if (dwGlobalDebugJIT & JIT_DEBUG)
      SendMessage(hWndDebugJITCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndDebugJITFromStartCheck, FALSE);
    if (dwGlobalDebugJIT & JIT_FROMSTART)
      SendMessage(hWndDebugJITFromStartCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (bGlobalDebugEnable)
      SendMessage(hWndDebugCodeCheck, BM_SETCHECK, BST_CHECKED, 0);
    else
      EnableWindow(hWndDebugCodeButton, FALSE);
    xprintfW(wszBuffer, L"0x%x", dwGlobalDebugCode);
    SetWindowTextWide(hWndDebugCodeButton, wszBuffer);

    //Columns
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_SCRIPT);
    lvc.cx=nColumnWidth1;
    lvc.iSubItem=LVI_SCRIPT_FILE;
    ListView_InsertColumnWide(hWndScriptsList, LVI_SCRIPT_FILE, &lvc);

    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=(wchar_t *)GetLangStringW(wLangModule, STRID_HOTKEY);
    lvc.cx=nColumnWidth2;
    lvc.iSubItem=LVI_SCRIPT_HOTKEY;
    ListView_InsertColumnWide(hWndScriptsList, LVI_SCRIPT_HOTKEY, &lvc);

    xprintfW(wszBuffer, L"%s (%d)", GetLangStringW(wLangModule, STRID_STATUS), hThreadStack.nElements);
    lvc.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvc.pszText=wszBuffer;
    lvc.cx=nColumnWidth3;
    lvc.iSubItem=LVI_SCRIPT_STATUS;
    ListView_InsertColumnWide(hWndScriptsList, LVI_SCRIPT_STATUS, &lvc);

    SetWindowTextWide(hWndScriptsFilter, wszFilter);
    FillScriptList(hWndScriptsList, wszFilter, wszContentFilter);

    lpOldFilterProc=(WNDPROC)GetWindowLongPtrWide(hWndScriptsFilter, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndScriptsFilter, GWLP_WNDPROC, (UINT_PTR)NewFilterProc);

    bOpeningDlg=FALSE;
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_SCRIPTS_LIST)
    {
      NMLISTVIEW *nmlw=(NMLISTVIEW *)lParam;

      if (nmlw->hdr.code == LVN_ITEMCHANGED)
      {
        if (nmlw->uNewState & LVIS_SELECTED)
        {
          nSelItem=nmlw->iItem;
          SendMessage(hWndHotkey, HKM_SETHOTKEY, nmlw->lParam, 0);

          EnableWindow(hWndExecButton, TRUE);
          EnableWindow(hWndEditButton, TRUE);
          EnableWindow(hWndHotkey, TRUE);
          EnableWindow(hWndAssignButton, TRUE);
        }
        if (nmlw->uOldState & LVIS_SELECTED)
        {
          nSelItem=-1;
          SendMessage(hWndHotkey, HKM_SETHOTKEY, 0, 0);

          EnableWindow(hWndExecButton, FALSE);
          EnableWindow(hWndEditButton, FALSE);
          EnableWindow(hWndHotkey, FALSE);
          EnableWindow(hWndAssignButton, FALSE);
        }
      }
      else if (((NMHDR *)lParam)->code == (UINT)NM_DBLCLK)
        PostMessage(hDlg, WM_COMMAND, IDC_EXEC, 0);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_SCRIPTS_FILTER)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        if (!bOpeningDlg)
        {
          SendMessage(hWndScriptsContentFilterEnable, BM_SETCHECK, BST_UNCHECKED, 0);
          bContentFilterEnable=FALSE;
          EnableWindow(hWndScriptsContentFilterButton, bContentFilterEnable);
        }
        GetWindowTextWide(hWndScriptsFilter, wszFilter, MAX_PATH);
        FillScriptList(hWndScriptsList, wszFilter, NULL);
      }
    }
    else if (LOWORD(wParam) == IDC_SCRIPTS_CONTENTFILTER_ENABLE)
    {
      bContentFilterEnable=(BOOL)SendMessage(hWndScriptsContentFilterEnable, BM_GETCHECK, 0, 0);
      EnableWindow(hWndScriptsContentFilterButton, bContentFilterEnable);
      if (*wszContentFilter) FillScriptList(hWndScriptsList, wszFilter, wszContentFilter);
    }
    else if (LOWORD(wParam) == IDC_SCRIPTS_CONTENTFILTER_BUTTON)
    {
      VARIANT vtResult;

      Document_InputBox(NULL, hDlg, (BSTR)GetLangStringW(wLangModule, STRID_CONTAIN), (BSTR)L"", wszContentFilter, &vtResult);

      if (vtResult.vt == VT_BSTR)
      {
        xstrcpynW(wszContentFilter, vtResult.bstrVal, MAX_PATH);
        SysFreeString(vtResult.bstrVal);
        FillScriptList(hWndScriptsList, wszFilter, wszContentFilter);

        //Set button text
        xprintfW(wszBuffer, L"%s%s", GetLangStringW(wLangModule, STRID_CONTAIN), *wszContentFilter?L" *":L"");
        SetWindowTextWide(hWndScriptsContentFilterButton, wszBuffer);
      }
    }
    else if (LOWORD(wParam) == IDC_ASSIGN)
    {
      LVITEMW lvi;
      WORD wHotkey;
      wchar_t wszFile[MAX_PATH];
      wchar_t wszHotkey[MAX_PATH];

      //Get script name
      lvi.mask=LVIF_TEXT;
      lvi.pszText=wszFile;
      lvi.cchTextMax=MAX_PATH;
      lvi.iItem=nSelItem;
      lvi.iSubItem=LVI_SCRIPT_FILE;
      ListView_GetItemWide(hWndScriptsList, &lvi);

      //Get script hotkey
      wHotkey=(WORD)SendMessage(hWndHotkey, HKM_GETHOTKEY, 0, 0);

      //Register hotkey
      if (RegisterHotkey(wszFile, wHotkey))
      {
        GetHotkeyString(wHotkey, wszHotkey);

        lvi.mask=LVIF_PARAM;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_SCRIPT_FILE;
        lvi.lParam=wHotkey;
        ListView_SetItemWide(hWndScriptsList, &lvi);

        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszHotkey;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_SCRIPT_HOTKEY;
        ListView_SetItemWide(hWndScriptsList, &lvi);

        SendMessage(hWndHotkey, HKM_SETHOTKEY, wHotkey, 0);
        bListChanged=TRUE;
      }
      else SetFocus(hWndHotkey);

      return TRUE;
    }
    else if (LOWORD(wParam) == IDC_DEBUG_JIT_CHECK)
    {
      BOOL bState;

      bState=(BOOL)SendMessage(hWndDebugJITCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndDebugJITFromStartCheck, bState);
    }
    else if (LOWORD(wParam) == IDC_DEBUG_CODE_CHECK)
    {
      BOOL bState;

      bState=(BOOL)SendMessage(hWndDebugCodeCheck, BM_GETCHECK, 0, 0);
      EnableWindow(hWndDebugCodeButton, bState);
    }
    else if (LOWORD(wParam) == IDC_DEBUG_CODE_BUTTON)
    {
      DWORD dwDebugCode=dwGlobalDebugCode;

      if (DialogBoxParamWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_CODE), hDlg, (DLGPROC)CodeDlgProc, (LPARAM)&dwDebugCode))
      {
        if (dwGlobalDebugCode != dwDebugCode)
        {
          dwGlobalDebugCode=dwDebugCode;
          dwSaveFlags|=OF_DEBUG;

          xprintfW(wszBuffer, L"0x%x", dwGlobalDebugCode);
          SetWindowTextWide(hWndDebugCodeButton, wszBuffer);
        }
      }
    }
    else if (LOWORD(wParam) == IDC_EXEC ||
             LOWORD(wParam) == IDC_EDIT ||
             LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      LVITEMW lvi;
      int nWidth;
      BOOL bDebugEnable;
      DWORD dwDebugFlags;

      nWidth=(int)SendMessage(hWndScriptsList, LVM_GETCOLUMNWIDTH, LVI_SCRIPT_FILE, 0);
      if (nColumnWidth1 != nWidth)
      {
        nColumnWidth1=nWidth;
        dwSaveFlags|=OF_RECT;
      }
      nWidth=(int)SendMessage(hWndScriptsList, LVM_GETCOLUMNWIDTH, LVI_SCRIPT_HOTKEY, 0);
      if (nColumnWidth2 != nWidth)
      {
        nColumnWidth2=nWidth;
        dwSaveFlags|=OF_RECT;
      }
      nWidth=(int)SendMessage(hWndScriptsList, LVM_GETCOLUMNWIDTH, LVI_SCRIPT_STATUS, 0);
      if (nColumnWidth3 != nWidth)
      {
        nColumnWidth3=nWidth;
        dwSaveFlags|=OF_RECT;
      }

      dwDebugFlags=0;
      if (SendMessage(hWndDebugJITCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugFlags|=JIT_DEBUG;
      if (SendMessage(hWndDebugJITFromStartCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugFlags|=JIT_FROMSTART;
      if (dwGlobalDebugJIT != dwDebugFlags)
      {
        dwGlobalDebugJIT=dwDebugFlags;
        dwSaveFlags|=OF_DEBUG;
      }

      bDebugEnable=(BOOL)SendMessage(hWndDebugCodeCheck, BM_GETCHECK, 0, 0);
      if (bGlobalDebugEnable != bDebugEnable)
      {
        bGlobalDebugEnable=bDebugEnable;
        dwSaveFlags|=OF_DEBUG;
      }

      if (bListChanged || LOWORD(wParam) == IDC_CLOSE)
      {
        dwSaveFlags|=OF_LASTSCRIPT|OF_HOTKEYS;
      }
      if (LOWORD(wParam) == IDC_EXEC ||
          LOWORD(wParam) == IDC_EDIT)
      {
        //Get script name
        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszLastScript;
        lvi.cchTextMax=MAX_PATH;
        lvi.iItem=nSelItem;
        lvi.iSubItem=LVI_SCRIPT_FILE;
        ListView_GetItemWide(hWndScriptsList, &lvi);

        dwSaveFlags|=OF_LASTSCRIPT;
      }
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      //End dialog
      EndDialog(hDlg, 0);

      //Execute
      if (LOWORD(wParam) == IDC_EXEC)
        ExecScript(wszLastScript, NULL, FALSE);
      else if (LOWORD(wParam) == IDC_EDIT)
      {
        EditScript(wszLastScript);

        //Find text, if content filter exist
        if (bContentFilterEnable && *wszContentFilter)
        {
          INT_PTR nResult;
          BSTR wszContentFilterSys;

          if (wszContentFilterSys=SysAllocString(wszContentFilter))
          {
            Document_TextFind(NULL, NULL, wszContentFilterSys, FRF_BEGINNING|FRF_DOWN, &nResult);
            SysFreeString(wszContentFilterSys);
          }
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }

  //Dialog resize messages
  {
    DIALOGRESIZEMSG drsm={&drs[0], &rcMainMinMaxDialog, &rcMainCurrentDialog, DRM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_DIALOGRESIZE, 0, (LPARAM)&drsm))
      dwSaveFlags|=OF_RECT;
  }

  return FALSE;
}

BOOL CALLBACK CodeDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndMemReadCheck;
  static HWND hWndMemWriteCheck;
  static HWND hWndMemFreeCheck;
  static HWND hWndMemLeakCheck;
  static HWND hWndSysCallCheck;
  static HWND hWndOK;
  static HWND hWndCancel;
  static DWORD *lpdwDebugCode;
  static DWORD dwDebugCode;

  if (uMsg == WM_INITDIALOG)
  {
    lpdwDebugCode=(DWORD *)lParam;
    dwDebugCode=*lpdwDebugCode;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)g_hPluginIcon);
    hWndMemReadCheck=GetDlgItem(hDlg, IDC_CODE_MEMREAD_CHECK);
    hWndMemWriteCheck=GetDlgItem(hDlg, IDC_CODE_MEMWRITE_CHECK);
    hWndMemFreeCheck=GetDlgItem(hDlg, IDC_CODE_MEMFREE_CHECK);
    hWndMemLeakCheck=GetDlgItem(hDlg, IDC_CODE_MEMLEAK_CHECK);
    hWndSysCallCheck=GetDlgItem(hDlg, IDC_CODE_SYSCALL_CHECK);
    hWndOK=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_CODE));
    SetDlgItemTextWide(hDlg, IDC_CODE_MEMREAD_CHECK, GetLangStringW(wLangModule, STRID_CODE_MEMREAD));
    SetDlgItemTextWide(hDlg, IDC_CODE_MEMWRITE_CHECK, GetLangStringW(wLangModule, STRID_CODE_MEMWRITE));
    SetDlgItemTextWide(hDlg, IDC_CODE_MEMFREE_CHECK, GetLangStringW(wLangModule, STRID_CODE_MEMFREE));
    SetDlgItemTextWide(hDlg, IDC_CODE_MEMLEAK_CHECK, GetLangStringW(wLangModule, STRID_CODE_MEMLEAK));
    SetDlgItemTextWide(hDlg, IDC_CODE_SYSCALL_CHECK, GetLangStringW(wLangModule, STRID_CODE_SYSCALL));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_OK));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    if (dwDebugCode & DBG_MEMREAD)
      SendMessage(hWndMemReadCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwDebugCode & DBG_MEMWRITE)
      SendMessage(hWndMemWriteCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwDebugCode & DBG_MEMFREE)
      SendMessage(hWndMemFreeCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwDebugCode & DBG_MEMLEAK)
      SendMessage(hWndMemLeakCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwDebugCode & DBG_SYSCALL)
      SendMessage(hWndSysCallCheck, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      dwDebugCode=0;
      if (SendMessage(hWndMemReadCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugCode|=DBG_MEMREAD;
      if (SendMessage(hWndMemWriteCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugCode|=DBG_MEMWRITE;
      if (SendMessage(hWndMemFreeCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugCode|=DBG_MEMFREE;
      if (SendMessage(hWndMemLeakCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugCode|=DBG_MEMLEAK;
      if (SendMessage(hWndSysCallCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwDebugCode|=DBG_SYSCALL;
      *lpdwDebugCode=dwDebugCode;

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

LRESULT CALLBACK NewFilterProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_DOWN || wParam == VK_UP)
    {
      if (GetFocus() != hWndScriptsList)
        SetFocus(hWndScriptsList);
      return SendMessage(hWndScriptsList, uMsg, wParam, lParam);
    }
  }
  return CallWindowProcWide(lpOldFilterProc, hWnd, uMsg, wParam, lParam);
}

BOOL RegisterHotkey(wchar_t *wszScriptName, WORD wHotkey)
{
  PLUGINFUNCTION *pfElement;
  wchar_t wszPrefix[MAX_PATH];
  wchar_t wszFunction[MAX_PATH];
  wchar_t wszHotkeyOwner[MAX_PATH];
  int nOwner;

  xstrcpyW(wszPrefix, L"Scripts::Main::");
  xprintfW(wszFunction, L"%s%s", wszPrefix, wszScriptName);
  nOwner=(int)SendMessage(hMainWnd, AKD_CHECKHOTKEY, wHotkey, (LPARAM)wszHotkeyOwner);

  if (!nOwner)
  {
    if (pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wszFunction, 0))
    {
      pfElement->wHotkey=wHotkey;

      if (!pfElement->wHotkey && !pfElement->bRunning && !pfElement->bAutoLoad)
        SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfElement);
    }
    else
    {
      if (wHotkey)
      {
        PLUGINADDW pa={0};

        pa.pFunction=wszFunction;
        pa.wHotkey=wHotkey;
        pa.PluginProc=HotkeyProc;

        if (pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa))
        {
          pfElement->lpParameter=pfElement->wszFunction + xstrlenW(wszPrefix);
        }
      }
    }
    return TRUE;
  }
  else
  {
    xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_HOTKEY_EXISTS), wszFunction, wszHotkeyOwner);
    MessageBoxW(hWndMainDlg, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
    return FALSE;
  }
}

void FillScriptList(HWND hWnd, const wchar_t *wpFilter, const wchar_t *wpContentFilter)
{
  SCRIPTTHREAD *lpScriptThread;
  PLUGINFUNCTION *pfElement;
  WIN32_FIND_DATAW wfd;
  HANDLE hFind;
  wchar_t wszFindFiles[MAX_PATH];
  wchar_t wszHotkey[MAX_PATH];
  const wchar_t *wpExt;
  wchar_t *wpContent;
  LVITEMW lvi;
  INT_PTR nContentLen;
  int nIndexToSelect=-1;
  int nIndex;
  int i=0;

  //Clear contents
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  SendMessage(hWnd, LVM_DELETEALLITEMS, 0, 0);

  //Script files
  xprintfW(wszFindFiles, L"%s\\*.*", wszScriptsDir);

  if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
  {
    do
    {
      if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == '\0' || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))) continue;

      if (!(wpExt=GetFileExt(wfd.cFileName, -1)))
        continue;
      if (xstrcmpiW(wpExt, L"js") && xstrcmpiW(wpExt, L"vbs"))
        continue;
      if (wpFilter && *wpFilter && !xstrstrW(wfd.cFileName, -1, wpFilter, -1, FALSE, NULL, NULL))
        continue;
      if (bContentFilterEnable && wpContentFilter && *wpContentFilter)
      {
        xprintfW(wszFindFiles, L"%s\\%s", wszScriptsDir, wfd.cFileName);
        if (nContentLen=ReadFileContent(wszFindFiles, ADT_BINARY_ERROR|ADT_DETECT_CODEPAGE|ADT_DETECT_BOM|ADT_NOMESSAGES, 0, 0, &wpContent, (UINT_PTR)-1))
        {
          if (!xstrstrW(wpContent, nContentLen, wpContentFilter, -1, FALSE, NULL, NULL))
            nContentLen=0;
          SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpContent);
        }
        if (!nContentLen) continue;
      }

      lvi.mask=LVIF_TEXT;
      lvi.pszText=wfd.cFileName;
      lvi.iItem=i++;
      lvi.iSubItem=LVI_SCRIPT_FILE;
      nIndex=ListView_InsertItemWide(hWnd, &lvi);

      if (nIndexToSelect < 0)
      {
        if (!xstrcmpiW(wfd.cFileName, wszLastScript))
          nIndexToSelect=nIndex;
      }
      else if (nIndexToSelect >= nIndex)
        ++nIndexToSelect;

      //Find hotkey
      xprintfW(wszBuffer, L"Scripts::Main::%s", wfd.cFileName);
      pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wszBuffer, 0);

      if (pfElement)
      {
        lvi.mask=LVIF_PARAM;
        lvi.iItem=nIndex;
        lvi.iSubItem=LVI_SCRIPT_FILE;
        lvi.lParam=pfElement->wHotkey;
        ListView_SetItemWide(hWnd, &lvi);

        GetHotkeyString(pfElement->wHotkey, wszHotkey);
        lvi.mask=LVIF_TEXT;
        lvi.pszText=wszHotkey;
        lvi.iItem=nIndex;
        lvi.iSubItem=LVI_SCRIPT_HOTKEY;
        ListView_SetItemWide(hWnd, &lvi);
      }

      if (lpScriptThread=StackGetScriptThreadByName(&hThreadStack, wfd.cFileName))
      {
        lvi.mask=LVIF_TEXT;
        lvi.pszText=(wchar_t *)(lpScriptThread->bWaiting?GetLangStringW(wLangModule, STRID_WAITING):GetLangStringW(wLangModule, STRID_RUNNING));
        lvi.iItem=nIndex;
        lvi.iSubItem=LVI_SCRIPT_STATUS;
        ListView_SetItemWide(hWnd, &lvi);
      }
    }
    while (FindNextFileWide(hFind, &wfd));

    FindClose(hFind);
  }
  lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
  lvi.state=LVIS_SELECTED|LVIS_FOCUSED;
  SendMessage(hWnd, LVM_SETITEMSTATE, (WPARAM)max(nIndexToSelect, 0), (LPARAM)&lvi);
  SendMessage(hWnd, LVM_ENSUREVISIBLE, (WPARAM)max(nIndexToSelect, 0), TRUE);

  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CLOSE)
  {
    //Send WM_CLOSE to scripts dialogs or
    //post quit message to message loop and
    //wait for thread finished.
    if (IsAnyMessageLoop())
    {
      int nCloseOK;
      int nCloseERR;

      CloseScriptThreadAll(&nCloseOK, &nCloseERR);

      if (nCloseOK && !nCloseERR)
      {
        if (!dwWaitScriptTimerId)
        {
          dwWaitScriptTimerStart=GetTickCount();
          dwWaitScriptTimerId=SetTimer(NULL, 0, 100, (TIMERPROC)TimerProc);
        }
      }
      //Abort program closing
      return 0;
    }
  }
  else if (uMsg == AKDN_MESSAGEBOXBEGIN)
  {
    g_MainMessageBox=TRUE;
    ShowScriptWindowsAll(FALSE);
  }
  else if (uMsg == AKDN_MESSAGEBOXEND)
  {
    g_MainMessageBox=FALSE;
    ShowScriptWindowsAll(TRUE);
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

    StackFreeCallbacks(&g_hHookCallbackStack);
    StackFreeCallbacks(&g_hSysCallbackStack);
    StackFreeCallbacks(&g_hSubclassCallbackStack);
    if (g_WScriptTypeInfo) g_WScriptTypeInfo->lpVtbl->Release(g_WScriptTypeInfo);
    if (g_WArgumentsTypeInfo) g_WArgumentsTypeInfo->lpVtbl->Release(g_WArgumentsTypeInfo);
    if (g_DocumentTypeInfo) g_DocumentTypeInfo->lpVtbl->Release(g_DocumentTypeInfo);
    if (g_ScriptSettingsTypeInfo) g_ScriptSettingsTypeInfo->lpVtbl->Release(g_ScriptSettingsTypeInfo);
    if (g_SystemFunctionTypeInfo) g_SystemFunctionTypeInfo->lpVtbl->Release(g_SystemFunctionTypeInfo);
    if (g_ConstantsTypeInfo) g_ConstantsTypeInfo->lpVtbl->Release(g_ConstantsTypeInfo);
    if (bInitMain) UninitMain();
    return FALSE;
  }
  else if (uMsg == AKDN_MAIN_ONDESTROY)
  {
    bMainOnFinish=TRUE;
    //We already quit from message loops in WM_CLOSE.
    //PostQuitScriptAll();
  }

  //Call next procedure
  return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  if (dwWaitScriptTimerId)
  {
    UINT_PTR dwElapsed=GetTickCount() - dwWaitScriptTimerStart;

    if (!IsAnyMessageLoop() || dwElapsed > 10000)
    {
      KillTimer(NULL, dwWaitScriptTimerId);
      dwWaitScriptTimerId=0;

      if (dwElapsed <= 10000)
        PostMessage(hMainWnd, WM_CLOSE, 0, 0);
    }
  }
}

BOOL CALLBACK HotkeyProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  wchar_t *wpName=(wchar_t *)lpParameter;

  ExecScript(wpName, NULL, FALSE);
  return TRUE;
}

int EditScript(wchar_t *wpScript)
{
  OPENDOCUMENTW od;
  wchar_t wszFullName[MAX_PATH];

  xprintfW(wszFullName, L"%s\\%s", wszScriptsDir, wpScript);
  od.pFile=wszFullName;
  od.pWorkDir=NULL;
  od.dwFlags=OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE;
  od.nCodePage=0;
  od.bBOM=0;
  return (int)SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
}

void ExecScript(wchar_t *wpScript, wchar_t *wszArguments, BOOL bWaitExec)
{
  SCRIPTTHREAD *lpScriptThread;
  EXECSCRIPT es;
  HANDLE hInitMutex;

  if (lpScriptThread=StackGetScriptThreadByName(&hThreadStack, wpScript))
  {
    if (lpScriptThread->bSingleCopy)
      return;
  }

  if (hInitMutex=CreateEventA(NULL, FALSE, FALSE, pMutexInitName))
  {
    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
      es.wpScript=wpScript;
      es.wpArguments=wszArguments;
      es.nArgumentsLen=(int)xstrlenW(wszArguments);
      es.hInitMutex=hInitMutex;
      es.bWaitForScriptSignal=bWaitExec;

      if (hExecThread=CreateThread(NULL, 0, ExecThreadProc, &es, 0, &dwExecThreadId))
      {
        if (bWaitExec)
        {
          //Wait for hInitMutex and process messages.
          MSG msg;
          BOOL bExitLoop=FALSE;

          for (;;)
          {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
              if (msg.message == WM_QUIT)
                bExitLoop=TRUE;
              else
                SendMessage(hMainWnd, AKD_TRANSLATEMESSAGE, TMSG_ALL, (LPARAM)&msg);
            }
            if (bExitLoop)
              break;
            if (MsgWaitForMultipleObjects(1, &hInitMutex, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
              break;
          }
        }
        else WaitForSingleObject(hInitMutex, INFINITE);
      }
    }
    CloseHandle(hInitMutex);
  }
}

DWORD WINAPI ExecThreadProc(LPVOID lpParameter)
{
  EXECSCRIPT *es=(EXECSCRIPT *)lpParameter;
  SCRIPTTHREAD *lpScriptThread;
  HANDLE hThread=hExecThread;
  DWORD dwThreadID=GetCurrentThreadId();

  if (lpScriptThread=StackInsertScriptThread(&hThreadStack))
  {
    lpScriptThread->hThread=hThread;
    lpScriptThread->dwThreadID=dwThreadID;
    if (bGlobalDebugEnable)
      lpScriptThread->dwDebug=dwGlobalDebugCode;
    if (dwGlobalDebugJIT & JIT_DEBUG)
      lpScriptThread->dwDebugJIT=dwGlobalDebugJIT;

    if (es->wpScript)
    {
      const wchar_t *wpFileName;

      wpFileName=GetFileNameWide(es->wpScript, -1);
      xstrcpynW(lpScriptThread->wszScriptName, wpFileName, MAX_PATH);
      GetBaseName(lpScriptThread->wszScriptName, lpScriptThread->wszScriptBaseName, MAX_PATH);
      xprintfW(lpScriptThread->wszScriptFile, L"%s\\%s", wszScriptsDir, es->wpScript);
    }
    if (es->wpArguments)
    {
      SCRIPTARG *lpScriptArg;
      wchar_t *wpArgument;
      wchar_t *wpCount;
      int nArgLen;

      if (lpScriptThread->wszArguments=(wchar_t *)GlobalAlloc(GPTR, (es->nArgumentsLen + 1) * sizeof(wchar_t)))
        lpScriptThread->nArgumentsLen=(int)xstrcpynW(lpScriptThread->wszArguments, es->wpArguments, es->nArgumentsLen + 1);
      wpArgument=lpScriptThread->wszArguments;

      while (*wpArgument)
      {
        nArgLen=GetCommandLineArg(wpArgument, NULL, 0, NULL);

        if (lpScriptArg=StackInsertArgument(&lpScriptThread->hArgStack))
        {
          if (lpScriptArg->wpArgument=(wchar_t *)GlobalAlloc(GPTR, (nArgLen + 1) * sizeof(wchar_t)))
          {
            lpScriptArg->nArgumentLen=GetCommandLineArg(wpArgument, lpScriptArg->wpArgument, nArgLen + 1, &wpArgument);

            if (lpScriptArg->wpArgument[0] == L'-' || lpScriptArg->wpArgument[0] == L'/')
            {
              //Argument name
              lpScriptArg->wpArgName=lpScriptArg->wpArgument + 1;

              for (wpCount=lpScriptArg->wpArgName; *wpCount && *wpCount != L'=' && *wpCount != L' '; ++wpCount);
              lpScriptArg->nArgNameLen=(int)(wpCount - lpScriptArg->wpArgName);

              //Argument value
              if (*wpCount == L'=')
              {
                lpScriptArg->wpArgValue=wpCount + 1;
                lpScriptArg->nArgValueLen=lpScriptArg->nArgumentLen - (int)(lpScriptArg->wpArgValue - lpScriptArg->wpArgument);

                //"Value" -> Value
                if (lpScriptArg->nArgValueLen >= 2)
                {
                  if (lpScriptArg->wpArgValue[0] == L'\"' && lpScriptArg->wpArgValue[lpScriptArg->nArgValueLen - 1] == L'\"')
                  {
                    lpScriptArg->wpArgValue+=1;
                    lpScriptArg->nArgValueLen-=2;
                  }
                }
              }
              else
              {
                //If no value set it to "1".
                lpScriptArg->wpArgValue=L"1";
                lpScriptArg->nArgValueLen=1;
              }
            }
          }
          else break;
        }
        else break;
      }
    }
    if (es->hInitMutex)
    {
      //Protect from double execution
      lpScriptThread->hExecMutex=CreateEventA(NULL, FALSE, FALSE, pMutexExecName);

      if (!es->bWaitForScriptSignal)
      {
        //Thread is initialized now unlock main thread
        SetEvent(es->hInitMutex);
      }
      else
      {
        //Thread is initialized now remember hInitMutex to unlock main thread later - after script signal or termination
        lpScriptThread->hInitMutex=es->hInitMutex;
      }
    }

    if (*lpScriptThread->wszScriptFile)
    {
      if (lpScriptThread->hExecMutex)
      {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
          lpScriptThread->bWaiting=TRUE;
          WaitForSingleObject(lpScriptThread->hExecMutex, INFINITE);
          lpScriptThread->bWaiting=FALSE;
        }
      }

      //Run script
      if (!bMainOnFinish)
      {
        wchar_t *wpContent;
        const wchar_t *wpExt;
        char szExt[MAX_PATH];
        GUID guidEngine;
        INT_PTR nContentLen;

        if (wpExt=GetFileExt(lpScriptThread->wszScriptName, -1))
        {
          WideCharToMultiByte(CP_ACP, 0, --wpExt, -1, szExt, MAX_PATH, NULL, NULL);
          if (GetScriptEngineA(szExt, &guidEngine) == S_OK)
          {
            if (nContentLen=ReadFileContent(lpScriptThread->wszScriptFile, ADT_BINARY_ERROR|ADT_DETECT_CODEPAGE|ADT_DETECT_BOM|ADT_NOMESSAGES, 0, 0, &wpContent, (UINT_PTR)-1))
            {
              lpScriptThread->wpScriptText=wpContent;
              lpScriptThread->nScriptTextLen=nContentLen;

              if (ExecScriptText(lpScriptThread, &guidEngine) != S_OK)
              {
                //MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_EXECUTE_ERROR), wszPluginTitle, MB_OK|MB_ICONERROR);
              }
              StackFreeIncludes(&lpScriptThread->hIncludesStack);
              SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)wpContent);
            }
            else
            {
              xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_READFILE_ERROR), lpScriptThread->wszScriptFile);
              MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONERROR);
            }
          }
          else
          {
            xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ENGINE_ERROR), wpExt);
            MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONERROR);
          }
        }
      }
    }

    FreeScriptResources(lpScriptThread, (lpScriptThread->dwDebug & DBG_MEMLEAK));
  }
  else SetEvent(es->hInitMutex);

  //Free thread handle
  hExecThread=NULL;
  dwExecThreadId=0;

  if (hThread)
  {
    CloseHandle(hThread);
    hThread=NULL;
  }
  return 0;
}

SCRIPTTHREAD* StackInsertScriptThread(HTHREADSTACK *hStack)
{
  SCRIPTTHREAD *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(SCRIPTTHREAD)))
  {
    lpElement->nRefCount=1;
    lpElement->lpStack=hStack;
    ++hStack->nElements;
  }
  return lpElement;
}

SCRIPTTHREAD* StackGetScriptThreadCurrent()
{
  return StackGetScriptThreadByID(&hThreadStack, GetCurrentThreadId());
}

SCRIPTTHREAD* StackGetScriptThreadByID(HTHREADSTACK *hStack, DWORD dwThreadID)
{
  SCRIPTTHREAD *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->dwThreadID == dwThreadID)
      return lpElement;
  }
  return NULL;
}

SCRIPTTHREAD* StackGetScriptThreadByName(HTHREADSTACK *hStack, const wchar_t *wpScriptName)
{
  SCRIPTTHREAD *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (!xstrcmpiW(lpElement->wszScriptName, wpScriptName))
      return lpElement;
  }
  return NULL;
}

BOOL StackDeleteScriptThread(SCRIPTTHREAD *lpScriptThread)
{
  if (--lpScriptThread->nRefCount <= 0)
  {
    HTHREADSTACK *hStack=(HTHREADSTACK *)lpScriptThread->lpStack;

    if (lpScriptThread->wszArguments)
    {
      GlobalFree((HGLOBAL)lpScriptThread->wszArguments);
      lpScriptThread->wszArguments=NULL;
    }
    StackFreeArguments(&lpScriptThread->hArgStack);
    StackDelete((stack **)&hStack->first, (stack **)&hStack->last, (stack *)lpScriptThread);
    --hStack->nElements;
    return TRUE;
  }
  return FALSE;
}

void FreeScriptResources(SCRIPTTHREAD *lpScriptThread, BOOL bDebug)
{
  if (lpScriptThread->hPointersStack.nElements)
  {
    if (bDebug)
    {
      if (!lpScriptThread->bStopped)
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_DEBUG_MEMLEAK), lpScriptThread->hPointersStack.nElements);
        MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONERROR);
      }
    }
    StackFreePointers(&lpScriptThread->hPointersStack);
  }
  if (lpScriptThread->hWndScriptsThreadDummy)
  {
    DestroyWindow(lpScriptThread->hWndScriptsThreadDummy);
    lpScriptThread->hWndScriptsThreadDummy=NULL;
  }
  if (lpScriptThread->hInitMutex)
  {
    SetEvent(lpScriptThread->hInitMutex);
    lpScriptThread->hInitMutex=NULL;
  }
  if (lpScriptThread->hExecMutex)
  {
    SetEvent(lpScriptThread->hExecMutex);
    CloseHandle(lpScriptThread->hExecMutex);
    lpScriptThread->hExecMutex=NULL;
  }
  StackDeleteScriptThread(lpScriptThread);
}

BOOL CloseScriptThread(SCRIPTTHREAD *lpScriptThread)
{
  CALLBACKITEM *lpCallback;
  CALLBACKITEM *lpNextCallback;
  BOOL bResult=TRUE;
  BOOL bTerminated=FALSE;
  BOOL bPostQuit=TRUE;

  //Close dialogs by sending WM_CLOSE. Script must call oSys.Call("user32::PostQuitMessage", 0) itself.
  for (lpCallback=lpScriptThread->hDialogCallbackStack.first; lpCallback; lpCallback=lpNextCallback)
  {
    lpNextCallback=lpCallback->next;

    if (lpCallback->hHandle)
    {
      if (IsWindow((HWND)lpCallback->hHandle))
      {
        SendMessage((HWND)lpCallback->hHandle, WM_CLOSE, 0, 0);
        if (IsWindow((HWND)lpCallback->hHandle))
          bResult=FALSE;
      }
      else
      {
        //Thread can be terminated outside of the program
        StackDeleteCallback(lpCallback);
        bTerminated=TRUE;
      }
      bPostQuit=FALSE;
    }
  }

  //Script has message loop - send quit mesage.
  if (bPostQuit)
  {
    if (lpScriptThread->hWndScriptsThreadDummy && lpScriptThread->dwMessageLoop)
      SendMessage(lpScriptThread->hWndScriptsThreadDummy, AKDLL_POSTQUIT, 0, 0);
  }

  if (bTerminated && !lpScriptThread->hDialogCallbackStack.nElements)
  {
    FreeScriptResources(lpScriptThread, FALSE);
  }
  return bResult;
}

void CloseScriptThreadAll(int *nCloseOK, int *nCloseERR)
{
  SCRIPTTHREAD *lpElement;
  SCRIPTTHREAD *lpNextElement;

  *nCloseOK=0;
  *nCloseERR=0;

  for (lpElement=hThreadStack.first; lpElement; lpElement=lpNextElement)
  {
    ++lpElement->nRefCount;
    if (CloseScriptThread(lpElement))
      ++*nCloseOK;
    else
      ++*nCloseERR;

    //Assign lpNextElement here cause after CloseScriptThread lpElement->next could be changed.
    lpNextElement=lpElement->next;
    StackDeleteScriptThread(lpElement);
  }
}

void PostQuitScriptAll()
{
  SCRIPTTHREAD *lpElement;
  SCRIPTTHREAD *lpNextElement;

  for (lpElement=hThreadStack.first; lpElement; lpElement=lpNextElement)
  {
    lpNextElement=lpElement->next;
    if (lpElement->hWndScriptsThreadDummy && lpElement->dwMessageLoop)
      SendMessage(lpElement->hWndScriptsThreadDummy, AKDLL_POSTQUIT, 0, 0);
  }
}

BOOL IsAnyMessageLoop()
{
  SCRIPTTHREAD *lpElement;

  for (lpElement=hThreadStack.first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->dwMessageLoop)
      return TRUE;
  }
  return FALSE;
}

void ShowScriptWindows(SCRIPTTHREAD *lpScriptThread, BOOL bShow)
{
  CALLBACKITEM *lpCallback;

  for (lpCallback=lpScriptThread->hDialogCallbackStack.first; lpCallback; lpCallback=lpCallback->next)
  {
    if (lpCallback->hHandle)
    {
      if (!bShow)
      {
        if (IsWindowVisible((HWND)lpCallback->hHandle))
        {
          ShowWindow((HWND)lpCallback->hHandle, SW_HIDE);
          lpCallback->bShow=TRUE;
        }
      }
      else
      {
        if (lpCallback->bShow)
          ShowWindow((HWND)lpCallback->hHandle, SW_SHOW);
      }
    }
  }
}

void ShowScriptWindowsAll(BOOL bShow)
{
  SCRIPTTHREAD *lpElement;

  for (lpElement=hThreadStack.first; lpElement; lpElement=lpElement->next)
  {
    //Hides window only if thread busy
    if (bShow || lpElement->bBusy)
      ShowScriptWindows(lpElement, bShow);
  }
}

int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg)
{
  const wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArgSet=wszArg;
  wchar_t *wpArgSetMax=wszArg + nArgMax - 1;
  wchar_t wchInitStopChar;
  wchar_t wchCurStopChar;

  while (*wpCount == L' ') ++wpCount;

  if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
    wchInitStopChar=*wpCount++;
  else
    wchInitStopChar=L' ';
  wchCurStopChar=wchInitStopChar;

  for (; *wpCount; ++wpCount)
  {
    if (wchCurStopChar == L' ')
    {
      if (*wpCount == L' ')
        break;
      if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
        wchCurStopChar=*wpCount;
    }
    else if (wchCurStopChar == *wpCount)
    {
      if (wchCurStopChar == wchInitStopChar)
        break;
      wchCurStopChar=L' ';
    }

    if (wpArgSet < wpArgSetMax)
    {
      if (wszArg) *wpArgSet=*wpCount;
      ++wpArgSet;
    }
  }
  if (wszArg) *wpArgSet=L'\0';

  if (wpNextArg)
  {
    if (*wpCount) ++wpCount;
    for (*wpNextArg=wpCount; **wpNextArg == L' '; ++*wpNextArg);
  }
  return (int)(wpArgSet - wszArg);
}

SCRIPTARG* StackInsertArgument(HARGSTACK *hStack)
{
  SCRIPTARG *lpElement=NULL;

  if (!StackInsertIndex((stack **)&hStack->first, (stack **)&hStack->last, (stack **)&lpElement, -1, sizeof(SCRIPTARG)))
    ++hStack->nElements;

  return lpElement;
}

SCRIPTARG* StackGetArgumentByIndex(HARGSTACK *hStack, int nIndex)
{
  SCRIPTARG *lpElement;
  int nCount=0;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (nIndex == nCount++)
      return lpElement;
  }
  return NULL;
}

SCRIPTARG* StackGetArgumentByName(HARGSTACK *hStack, const wchar_t *wpArgName, int nArgNameLen)
{
  SCRIPTARG *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    if (lpElement->wpArgName && nArgNameLen == lpElement->nArgNameLen)
    {
      if (!xstrcmpinW(wpArgName, lpElement->wpArgName, (UINT_PTR)-1))
        return lpElement;
    }
  }
  return NULL;
}

void StackFreeArguments(HARGSTACK *hStack)
{
  SCRIPTARG *lpElement;

  for (lpElement=hStack->first; lpElement; lpElement=lpElement->next)
  {
    GlobalFree((HGLOBAL)lpElement->wpArgument);
  }
  StackClear((stack **)&hStack->first, (stack **)&hStack->last);
  hStack->nElements=0;
}

UINT_PTR GetVariantInt(VARIANT *vt)
{
  VARIANT vtConverted;
  UINT_PTR dwResult=0;
  INT_PTR nResult=0;

  VariantInit(&vtConverted);
  VariantCopy(&vtConverted, vt);

  #ifdef _WIN64
    if (VariantChangeType(&vtConverted, &vtConverted, 0, VT_I8) == S_OK)
      nResult=vtConverted.llVal;
    if (!nResult && (VariantChangeType(&vtConverted, &vtConverted, 0, VT_UI8) == S_OK))
      dwResult=vtConverted.ullVal;
  #else
    if (VariantChangeType(&vtConverted, &vtConverted, 0, VT_I4) == S_OK)
      nResult=vtConverted.lVal;
    if (!nResult && (VariantChangeType(&vtConverted, &vtConverted, 0, VT_UI4) == S_OK))
      dwResult=vtConverted.ulVal;
  #endif
  return max(dwResult, (UINT_PTR)nResult);
}

UINT_PTR GetVariantValue(VARIANT *pvtParameter, BOOL bAnsi)
{
  CALLBACKITEM *lpSysCallback;
  UINT_PTR dwValue=0;
  int nUniLen;
  int nAnsiLen;

  if (pvtParameter->vt == VT_BSTR)
  {
    if (bAnsi)
    {
      nUniLen=SysStringLen(pvtParameter->bstrVal);
      nAnsiLen=WideCharToMultiByte(CP_ACP, 0, pvtParameter->bstrVal, nUniLen, NULL, 0, NULL, NULL);
      if (dwValue=(UINT_PTR)GlobalAlloc(GPTR, nAnsiLen + 1))
        WideCharToMultiByte(CP_ACP, 0, pvtParameter->bstrVal, nUniLen + 1, (char *)dwValue, nAnsiLen + 1, NULL, NULL);
    }
    else dwValue=(UINT_PTR)pvtParameter->bstrVal;
  }
  else if (pvtParameter->vt == VT_DISPATCH)
  {
    if (lpSysCallback=StackGetCallbackByObject(&g_hSysCallbackStack, pvtParameter->pdispVal))
      dwValue=(UINT_PTR)lpSysCallback->lpProc;
    else
      dwValue=(UINT_PTR)pvtParameter->pdispVal;
  }
  else
  {
    if (pvtParameter->vt == VT_BOOL)
      dwValue=pvtParameter->boolVal?TRUE:FALSE;
    else
      dwValue=GetVariantInt(pvtParameter);
  }
  return dwValue;
}

int GetHotkeyString(WORD wHotkey, wchar_t *wszString)
{
  wchar_t wszKeyText[100];
  LONG lScan;
  int nResult=0;

  wszString[0]=L'\0';
  wszKeyText[0]=L'\0';

  if (wHotkey)
  {
    if (bOldWindows)
      lScan=MapVirtualKeyA(LOBYTE(wHotkey), 0) << 16;
    else
      lScan=MapVirtualKeyW(LOBYTE(wHotkey), 0) << 16;
    if (HIBYTE(wHotkey) & HOTKEYF_EXT)
      lScan|=0x01000000L;
    GetKeyNameTextWide(lScan, wszKeyText, 100);

    nResult=(int)xprintfW(wszString, L"%s%s%s%s",
                                     (HIBYTE(wHotkey) & HOTKEYF_CONTROL)?L"Ctrl + ":L"",
                                     (HIBYTE(wHotkey) & HOTKEYF_SHIFT)?L"Shift + ":L"",
                                     (HIBYTE(wHotkey) & HOTKEYF_ALT)?L"Alt + ":L"",
                                     wszKeyText);
  }
  return nResult;
}

INT_PTR ReadFileContent(wchar_t *wpFile, DWORD dwFlags, int nCodePage, BOOL bBOM, wchar_t **wpContent, UINT_PTR dwBytesMax)
{
  DETECTFILEW df;
  FILECONTENT fc;
  INT_PTR nResult=0;

  *wpContent=NULL;

  //Detect codepage
  df.pFile=wpFile;
  df.dwBytesToCheck=1024;
  df.dwFlags=dwFlags;
  df.nCodePage=nCodePage;
  df.bBOM=bBOM;
  if (SendMessage(hMainWnd, AKD_DETECTFILEW, 0, (LPARAM)&df) == EDT_SUCCESS)
  {
    //Read contents
    if ((fc.hFile=CreateFileWide(df.pFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != INVALID_HANDLE_VALUE)
    {
      fc.dwMax=dwBytesMax;
      fc.nCodePage=df.nCodePage;
      fc.bBOM=df.bBOM;
      if (nResult=SendMessage(hMainWnd, AKD_READFILECONTENT, 0, (LPARAM)&fc))
      {
        CloseHandle(fc.hFile);
        fc.hFile=NULL;

        *wpContent=fc.wpContent;
      }
      if (fc.hFile) CloseHandle(fc.hFile);
    }
  }
  return nResult;
}

int GetBaseName(const wchar_t *wpFile, wchar_t *wszBaseName, int nBaseNameMaxLen)
{
  int nFileLen=(int)xstrlenW(wpFile);
  int nEndOffset=-1;
  int i;

  for (i=nFileLen - 1; i >= 0; --i)
  {
    if (wpFile[i] == L'\\')
      break;

    if (nEndOffset == -1)
    {
      if (wpFile[i] == L'.')
        nEndOffset=i;
    }
  }
  ++i;
  if (nEndOffset == -1) nEndOffset=nFileLen;
  nBaseNameMaxLen=min(nEndOffset - i + 1, nBaseNameMaxLen);
  xstrcpynW(wszBaseName, wpFile + i, nBaseNameMaxLen);

  return nBaseNameMaxLen;
}

const wchar_t* GetFileExt(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount >= wpFile; --wpCount)
  {
    if (*wpCount == L'.') return wpCount + 1;
    if (*wpCount == L'\\') break;
  }
  return NULL;
}

const char* GetFileNameAnsi(const char *pFile, int nFileLen)
{
  const char *pCount;

  if (nFileLen == -1) nFileLen=(int)lstrlenA(pFile);

  for (pCount=pFile + nFileLen - 1; pCount >= pFile; --pCount)
  {
    if (*pCount == '\\')
      return pCount + 1;
  }
  return pFile;
}

const wchar_t* GetFileNameWide(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount >= wpFile; --wpCount)
  {
    if (*wpCount == L'\\')
      return wpCount + 1;
  }
  return wpFile;
}

int GetFileStreamOffset(const wchar_t *wpFile, int nFileLen)
{
  const wchar_t *wpCount;

  if (nFileLen == -1) nFileLen=(int)xstrlenW(wpFile);

  for (wpCount=wpFile + nFileLen - 1; wpCount > wpFile; --wpCount)
  {
    if (*wpCount == L':') return (int)(wpCount - wpFile);
    if (*wpCount == L'\\') break;
  }
  return 0;
}

DWORD ScrollCaret(HWND hWnd)
{
  AESCROLLTOPOINT stp;
  DWORD dwScrollResult;

  //Test scroll to caret
  stp.dwFlags=AESC_TEST|AESC_POINTCARET|AESC_OFFSETCHARX|AESC_OFFSETCHARY;
  stp.nOffsetX=1;
  stp.nOffsetY=0;
  dwScrollResult=(DWORD)SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);

  //Scroll to caret
  stp.dwFlags=AESC_POINTCARET;
  stp.nOffsetX=3;
  stp.nOffsetY=2;
  if (dwScrollResult & AECSE_SCROLLEDX)
    stp.dwFlags|=AESC_OFFSETRECTDIVX;
  if (dwScrollResult & AECSE_SCROLLEDY)
    stp.dwFlags|=AESC_OFFSETRECTDIVY;
  return (DWORD)SendMessage(hWnd, AEM_SCROLLTOPOINT, 0, (LPARAM)&stp);
}

int SaveLineScroll(HWND hWnd)
{
  SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
  return (int)SendMessage(hWnd, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
}

void RestoreLineScroll(HWND hWnd, int nBeforeLine)
{
  if ((int)SendMessage(hWnd, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0) != nBeforeLine)
  {
    POINT64 ptScrollPos;

    ptScrollPos.x=-1;
    ptScrollPos.y=SendMessage(hWnd, AEM_VPOSFROMLINE, AECT_GLOBAL, nBeforeLine);
    SendMessage(hWnd, AEM_SETSCROLLPOS, 0, (LPARAM)&ptScrollPos);
  }
  SendMessage(hWnd, AEM_UPDATECARET, 0, 0);
  SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
  InvalidateRect(hWnd, NULL, TRUE);
}

BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetWindowRect(hWnd, rc))
  {
    rc->right-=rc->left;
    rc->bottom-=rc->top;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}


//// Options

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData)
{
  PLUGINOPTIONW po;

  if (!hOptions) return 0;
  po.pOptionName=pOptionName;
  po.dwType=dwType;
  po.lpData=lpData;
  po.dwData=dwData;
  return SendMessage(hMainWnd, AKD_OPTIONW, (WPARAM)hOptions, (LPARAM)&po);
}

void ReadOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"/WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"/ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
    WideOption(hOptions, L"/ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
    WideOption(hOptions, L"/ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));
    WideOption(hOptions, L"/GlobalDebugJIT", PO_DWORD, (LPBYTE)&dwGlobalDebugJIT, sizeof(DWORD));
    WideOption(hOptions, L"/GlobalDebugEnable", PO_DWORD, (LPBYTE)&bGlobalDebugEnable, sizeof(DWORD));
    WideOption(hOptions, L"/GlobalDebugCode", PO_DWORD, (LPBYTE)&dwGlobalDebugCode, sizeof(DWORD));
    WideOption(hOptions, L"/LastScript", PO_STRING, (LPBYTE)wszLastScript, MAX_PATH * sizeof(wchar_t));

    //Scripts hotkeys
    {
      WIN32_FIND_DATAW wfd;
      HANDLE hFind;
      wchar_t wszFindFiles[MAX_PATH];
      const wchar_t *wpExt;

      xprintfW(wszFindFiles, L"%s\\*.*", wszScriptsDir);

      if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
      {
        do
        {
          if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == '\0' || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))) continue;

          if (wpExt=GetFileExt(wfd.cFileName, -1))
          {
            if (!xstrcmpiW(wpExt, L"js") ||
                !xstrcmpiW(wpExt, L"vbs"))
            {
              DWORD dwHotkey=0;

              if (hOptions)
                WideOption(hOptions, wfd.cFileName, PO_DWORD, (LPBYTE)&dwHotkey, sizeof(DWORD));
              if (dwHotkey)
                RegisterHotkey(wfd.cFileName, LOWORD(dwHotkey));
            }
          }
        }
        while (FindNextFileWide(hFind, &wfd));

        FindClose(hFind);
      }
    }

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE|((dwFlags & OF_HOTKEYS)?POB_CLEAR:0), (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_RECT)
    {
      WideOption(hOptions, L"/WindowRect", PO_BINARY, (LPBYTE)&rcMainCurrentDialog, sizeof(RECT));
      WideOption(hOptions, L"/ColumnWidth1", PO_DWORD, (LPBYTE)&nColumnWidth1, sizeof(DWORD));
      WideOption(hOptions, L"/ColumnWidth2", PO_DWORD, (LPBYTE)&nColumnWidth2, sizeof(DWORD));
      WideOption(hOptions, L"/ColumnWidth3", PO_DWORD, (LPBYTE)&nColumnWidth3, sizeof(DWORD));
    }
    if (dwFlags & OF_DEBUG)
    {
      WideOption(hOptions, L"/GlobalDebugJIT", PO_DWORD, (LPBYTE)&dwGlobalDebugJIT, sizeof(DWORD));
      WideOption(hOptions, L"/GlobalDebugEnable", PO_DWORD, (LPBYTE)&bGlobalDebugEnable, sizeof(DWORD));
      WideOption(hOptions, L"/GlobalDebugCode", PO_DWORD, (LPBYTE)&dwGlobalDebugCode, sizeof(DWORD));
    }
    if (dwFlags & OF_LASTSCRIPT)
    {
      WideOption(hOptions, L"/LastScript", PO_STRING, (LPBYTE)wszLastScript, ((int)xstrlenW(wszLastScript) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_HOTKEYS)
    {
      WIN32_FIND_DATAW wfd;
      HANDLE hFind;
      wchar_t wszFindFiles[MAX_PATH];
      const wchar_t *wpExt;

      //Script files
      xprintfW(wszFindFiles, L"%s\\*.*", wszScriptsDir);

      if ((hFind=FindFirstFileWide(wszFindFiles, &wfd)) != INVALID_HANDLE_VALUE)
      {
        do
        {
          if (wfd.cFileName[0] == '.' && (wfd.cFileName[1] == '\0' || (wfd.cFileName[1] == '.' && wfd.cFileName[2] == '\0'))) continue;

          if (wpExt=GetFileExt(wfd.cFileName, -1))
          {
            if (!xstrcmpiW(wpExt, L"js") || !xstrcmpiW(wpExt, L"vbs"))
            {
              //Find hotkey
              PLUGINFUNCTION *pfElement=NULL;

              xprintfW(wszBuffer, L"Scripts::Main::%s", wfd.cFileName);
              pfElement=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)wszBuffer, 0);

              //Save hotkey
              if (pfElement)
              {
                WideOption(hOptions, wfd.cFileName, PO_DWORD, (LPBYTE)&pfElement->wHotkey, sizeof(WORD));
              }
            }
          }
        }
        while (FindNextFileWide(hFind, &wfd));

        FindClose(hFind);
      }
    }

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
  wLangID=PRIMARYLANGID(wLangID);

  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_ARCHITECTURE_MISMATCH)
      return L"\x0410\x0440\x0445\x0438\x0442\x0435\x043A\x0442\x0443\x0440\x0430\x0020\x043F\x043B\x0430\x0433\x0438\x043D\x0430\x0020\x0438\x0020\x0041\x006B\x0065\x006C\x0050\x0061\x0064\x0027\x0430\x0020\x043D\x0435\x0020\x0441\x043E\x0432\x043C\x0435\x0441\x0442\x0438\x043C\x044B\x002E";
    if (nStringID == STRID_HOTKEY_EXISTS)
      return L"%s\n\n\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430\x0020\x0443\x0436\x0435\x0020\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x0430\x0020\x0434\x043B\x044F \"%s\".";
    if (nStringID == STRID_EXECUTE_ERROR)
      return L"\x041E\x0448\x0438\x0431\x043A\x0430\x0020\x043F\x0440\x0438\x0020\x043F\x043E\x043F\x044B\x0442\x043A\x0435\x0020\x0437\x0430\x043F\x0443\x0441\x0442\x0438\x0442\x044C\x0020\x043A\x043E\x0434\x0020\x0441\x043A\x0440\x0438\x043F\x0442\x0430\x002E";
    if (nStringID == STRID_ENGINE_ERROR)
      return L"\x0421\x0435\x0440\x0432\x0435\x0440\x0020\x0441\x0446\x0435\x043D\x0430\x0440\x0438\x0435\x0432\x0020\x0434\x043B\x044F\x0020\x0442\x0438\x043F\x0430\x0020\x0444\x0430\x0439\x043B\x043E\x0432 \"%s\" \x043D\x0435\x0020\x043D\x0430\x0439\x0434\x0435\x043D\x002E";
    if (nStringID == STRID_READFILE_ERROR)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043F\x0440\x043E\x0447\x0438\x0442\x0430\x0442\x044C\x0020\x0444\x0430\x0439\x043B \"%s\".";
    if (nStringID == STRID_DEBUG_ERROR)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0438\x043D\x0438\x0446\x0438\x0430\x043B\x0438\x0437\x0438\x0440\x043E\x0432\x0430\x0442\x044C\x0020\x043F\x0440\x0438\x043B\x043E\x0436\x0435\x043D\x0438\x0435\x0020\x0434\x043B\x044F\x0020\x043E\x0442\x043B\x0430\x0434\x043A\x0438\x002E";
    if (nStringID == STRID_SCRIPTERROR)
      return L"\x0421\x043A\x0440\x0438\x043F\x0442:\t%s\n\x0421\x0442\x0440\x043E\x043A\x0430:\t%u\n\x0421\x0438\x043C\x0432\x043E\x043B\x003A\t%d\n\x041E\x0448\x0438\x0431\x043A\x0430:\t%s\n\x041A\x043E\x0434:\t%X\n\x0418\x0441\x0442\x043E\x0447\x043D\x0438\x043A:\t%s\n";
    if (nStringID == STRID_STOP)
      return L"\x041E\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C";
    if (nStringID == STRID_CONTINUE)
      return L"\x041F\x0440\x043E\x0434\x043E\x043B\x0436\x0438\x0442\x044C";
    if (nStringID == STRID_DEBUG)
      return L"\x041E\x0442\x043B\x0430\x0434\x043A\x0430";
    if (nStringID == STRID_JIT)
      return L"JIT";
    if (nStringID == STRID_FROMSTART)
      return L"\x0421\x0020\x043D\x0430\x0447\x0430\x043B\x0430";
    if (nStringID == STRID_CODE)
      return L"\x041A\x043E\x0434";
    if (nStringID == STRID_CODE_MEMREAD)
      return L"\x041E\x0442\x043B\x0430\x0434\x0438\x0442\x044C\x0020\x0447\x0442\x0435\x043D\x0438\x0435\x0020\x043F\x0430\x043C\x044F\x0442\x0438";
    if (nStringID == STRID_CODE_MEMWRITE)
      return L"\x041E\x0442\x043B\x0430\x0434\x0438\x0442\x044C\x0020\x0437\x0430\x043F\x0438\x0441\x044C\x0020\x043F\x0430\x043C\x044F\x0442\x0438";
    if (nStringID == STRID_CODE_MEMFREE)
      return L"\x041E\x0442\x043B\x0430\x0434\x0438\x0442\x044C\x0020\x043E\x0441\x0432\x043E\x0431\x043E\x0436\x0434\x0435\x043D\x0438\x0435\x0020\x043F\x0430\x043C\x044F\x0442\x0438";
    if (nStringID == STRID_CODE_MEMLEAK)
      return L"\x041E\x0442\x043B\x0430\x0434\x0438\x0442\x044C\x0020\x0443\x0442\x0435\x0447\x043A\x0443\x0020\x043F\x0430\x043C\x044F\x0442\x0438";
    if (nStringID == STRID_CODE_SYSCALL)
      return L"\x041E\x0442\x043B\x0430\x0434\x0438\x0442\x044C\x0020\x0432\x044B\x0437\x043E\x0432\x0020\x0441\x0438\x0441\x0442\x0435\x043C\x043D\x044B\x0445\x0020\x0444\x0443\x043D\x043A\x0446\x0438\x0439";
    if (nStringID == STRID_DEBUG_MEMLOCATE)
      return L"\x0423\x043A\x0430\x0437\x0430\x0442\x0435\x043B\x044C (%d) \x043D\x0435\x0020\x043D\x0430\x0445\x043E\x0434\x0438\x0442\x0441\x044F\x0020\x0432\x0020\x043F\x0430\x043C\x044F\x0442\x0438\x002C\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x043E\x0439\x0020\x0441\x0020\x043F\x043E\x043C\x043E\x0449\x044C\x044E AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMREAD)
      return L"\x0420\x0430\x0437\x043C\x0435\x0440\x0020\x0441\x0447\x0438\x0442\x044B\x0432\x0430\x0435\x043C\x044B\x0445\x0020\x0434\x0430\x043D\x043D\x044B\x0445\x0020\x0432\x044B\x0445\x043E\x0434\x0438\x0442\x0020\x0437\x0430\x0020\x043F\x0440\x0435\x0434\x0435\x043B\x044B\x0020\x043F\x0430\x043C\x044F\x0442\x0438\x002C\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x043E\x0439\x0020\x0441\x0020\x043F\x043E\x043C\x043E\x0449\x044C\x044E AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMWRITE)
      return L"\x0420\x0430\x0437\x043C\x0435\x0440\x0020\x0437\x0430\x043F\x0438\x0441\x044B\x0432\x0430\x0435\x043C\x044B\x0445\x0020\x0434\x0430\x043D\x043D\x044B\x0445\x0020\x0432\x044B\x0445\x043E\x0434\x0438\x0442\x0020\x0437\x0430\x0020\x043F\x0440\x0435\x0434\x0435\x043B\x044B\x0020\x043F\x0430\x043C\x044F\x0442\x0438\x002C\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x043E\x0439\x0020\x0441\x0020\x043F\x043E\x043C\x043E\x0449\x044C\x044E AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMFREE)
      return L"\x0423\x043A\x0430\x0437\x0430\x0442\x0435\x043B\x044C (%d) \x043D\x0435\x0020\x0431\x044B\x043B\x0020\x043F\x043E\x043B\x0443\x0447\x0435\x043D\x0020\x0441\x0020\x043F\x043E\x043C\x043E\x0449\x044C\x044E AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMLEAK)
      return L"\x041D\x0435\x0020\x0432\x044B\x0437\x0432\x0430\x043D\x043E AkelPad.MemFree \x0434\x043B\x044F (%d) \x043E\x0431\x044A\x0435\x043A\x0442\x043E\x0432\x002C\x0020\x043F\x043E\x043B\x0443\x0447\x0435\x043D\x043D\x044B\x0445\x0020\x0441\x0020\x043F\x043E\x043C\x043E\x0449\x044C\x044E\x0020\x0041\x006B\x0065\x006C\x0050\x0061\x0064\x002E\x004D\x0065\x006D\x0041\x006C\x006C\x006F\x0063\x002E";
    if (nStringID == STRID_DEBUG_SYSCALL)
      return L"\x041D\x0435\x0020\x043D\x0430\x0439\x0434\x0435\x043D\x0020\x0444\x0430\x0439\x043B \"%s\".";
    if (nStringID == STRID_DEBUG_SYSFUNCTION)
      return L"\x041D\x0435\x0020\x043D\x0430\x0439\x0434\x0435\x043D\x0430\x0020\x0444\x0443\x043D\x043A\x0446\x0438\x044F \"%s\" \x0432\x0020\x0444\x0430\x0439\x043B\x0435 \"%s\".";
    if (nStringID == STRID_SCRIPT)
      return L"\x0421\x043A\x0440\x0438\x043F\x0442";
    if (nStringID == STRID_HOTKEY)
      return L"\x0413\x043E\x0440\x044F\x0447\x0430\x044F\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0430";
    if (nStringID == STRID_STATUS)
      return L"\x0421\x0442\x0430\x0442\x0443\x0441";
    if (nStringID == STRID_RUNNING)
      return L"\x0420\x0430\x0431\x043E\x0442\x0430\x0435\x0442";
    if (nStringID == STRID_WAITING)
      return L"\x041E\x0436\x0438\x0434\x0430\x0435\x0442";
    if (nStringID == STRID_CONTAIN)
      return L"\x0421\x043E\x0434\x0435\x0440\x0436\x0438\x0442";
    if (nStringID == STRID_EXEC)
      return L"\x0417\x0430\x043F\x0443\x0441\x0442\x0438\x0442\x044C";
    if (nStringID == STRID_EDIT)
      return L"\x0418\x0437\x043C\x0435\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_ASSIGN)
      return L"\x041D\x0430\x0437\x043D\x0430\x0447\x0438\x0442\x044C";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
    if (nStringID == STRID_CLOSE)
      return L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C";
  }
  else
  {
    if (nStringID == STRID_ARCHITECTURE_MISMATCH)
      return L"Architecture of the plugin and the AkelPad aren't compatible.";
    if (nStringID == STRID_HOTKEY_EXISTS)
      return L"%s\n\nHotkey already assigned to \"%s\".";
    if (nStringID == STRID_EXECUTE_ERROR)
      return L"Can't execute script code.";
    if (nStringID == STRID_ENGINE_ERROR)
      return L"Script engine for file type \"%s\" not found.";
    if (nStringID == STRID_READFILE_ERROR)
      return L"Can't read file \"%s\".";
    if (nStringID == STRID_DEBUG_ERROR)
      return L"Can't initialize debug IDE.";
    if (nStringID == STRID_SCRIPTERROR)
      return L"Script:\t%s\nLine:\t%u\nSymbol:\t%d\nError:\t%s\nCode:\t%X\nSource:\t%s\n";
    if (nStringID == STRID_STOP)
      return L"Stop";
    if (nStringID == STRID_CONTINUE)
      return L"Continue";
    if (nStringID == STRID_DEBUG)
      return L"Debug";
    if (nStringID == STRID_JIT)
      return L"JIT";
    if (nStringID == STRID_FROMSTART)
      return L"From start";
    if (nStringID == STRID_CODE)
      return L"Code";
    if (nStringID == STRID_CODE_MEMREAD)
      return L"Debug memory read";
    if (nStringID == STRID_CODE_MEMWRITE)
      return L"Debug memory write";
    if (nStringID == STRID_CODE_MEMFREE)
      return L"Debug memory free";
    if (nStringID == STRID_CODE_MEMLEAK)
      return L"Debug memory leak";
    if (nStringID == STRID_CODE_SYSCALL)
      return L"Debug system function call";
    if (nStringID == STRID_DEBUG_MEMLOCATE)
      return L"Pointer (%d) is not located in memory that was allocated with AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMREAD)
      return L"Data read size is out of the memory allocated with AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMWRITE)
      return L"Data write size is out of the memory allocated with AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMFREE)
      return L"Pointer (%d) has not been received with AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_MEMLEAK)
      return L"AkelPad.MemFree is not called for (%d) objects received with AkelPad.MemAlloc.";
    if (nStringID == STRID_DEBUG_SYSCALL)
      return L"Cannot find file \"%s\".";
    if (nStringID == STRID_DEBUG_SYSFUNCTION)
      return L"Cannot find function \"%s\" in file \"%s\".";
    if (nStringID == STRID_SCRIPT)
      return L"Script";
    if (nStringID == STRID_HOTKEY)
      return L"Hotkey";
    if (nStringID == STRID_STATUS)
      return L"State";
    if (nStringID == STRID_RUNNING)
      return L"Running";
    if (nStringID == STRID_WAITING)
      return L"Waiting";
    if (nStringID == STRID_CONTAIN)
      return L"Contain";
    if (nStringID == STRID_EXEC)
      return L"Exec";
    if (nStringID == STRID_EDIT)
      return L"Edit";
    if (nStringID == STRID_ASSIGN)
      return L"Assign";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
    if (nStringID == STRID_CLOSE)
      return L"Close";
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
  hInstanceEXE=pd->hInstanceEXE;
  hInstanceDLL=pd->hInstanceDLL;
  hMainWnd=pd->hMainWnd;
  hGlobalAccel=pd->hGlobalAccel;
  bOldWindows=pd->bOldWindows;
  bOldRichEdit=pd->bOldRichEdit;
  bOldComctl32=pd->bOldComctl32;
  bAkelEdit=pd->bAkelEdit;
  nMDI=pd->nMDI;
  wLangModule=pd->wLangModule;

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
  xprintfW(wszScriptsDir, L"%s\\AkelFiles\\Plugs\\Scripts", pd->wszAkelDir);
  xstrcpynW(wszAkelPadDir, pd->wszAkelDir, MAX_PATH);
  ReadOptions(0);
}

void InitMain()
{
  bInitMain=TRUE;

  //Load plugin icon
  g_hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON_PLUGIN));

  //SubClass
  NewMainProcData=NULL;
  SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);
}

void UninitMain()
{
  bInitMain=FALSE;

  //Destroy plugin icon
  if (g_hPluginIcon) DestroyIcon(g_hPluginIcon);

  if (NewMainProcData)
  {
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
    NewMainProcData=NULL;
  }
}


//Entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
  {
    hInstanceDLL=hinstDLL;
    MyIClassFactoryObj.lpVtbl=NULL;
    DisableThreadLibraryCalls(hinstDLL);
  }
  else if (fdwReason == DLL_THREAD_ATTACH)
  {
  }
  else if (fdwReason == DLL_THREAD_DETACH)
  {
  }
  else if (fdwReason == DLL_PROCESS_DETACH)
  {
    //ActiveXObject
    if (MyIClassFactoryObj.lpVtbl)
    {
      if (lpScriptThreadActiveX)
      {
        FreeScriptResources(lpScriptThreadActiveX, (lpScriptThreadActiveX->dwDebug & DBG_MEMLEAK));
        lpScriptThreadActiveX=NULL;
      }
      StackFreeCallbacks(&g_hHookCallbackStack);
      StackFreeCallbacks(&g_hSysCallbackStack);
      StackFreeCallbacks(&g_hSubclassCallbackStack);
      if (g_WScriptTypeInfo) g_WScriptTypeInfo->lpVtbl->Release(g_WScriptTypeInfo);
      if (g_WArgumentsTypeInfo) g_WArgumentsTypeInfo->lpVtbl->Release(g_WArgumentsTypeInfo);
      if (g_DocumentTypeInfo) g_DocumentTypeInfo->lpVtbl->Release(g_DocumentTypeInfo);
      if (g_ScriptSettingsTypeInfo) g_ScriptSettingsTypeInfo->lpVtbl->Release(g_ScriptSettingsTypeInfo);
      if (g_SystemFunctionTypeInfo) g_SystemFunctionTypeInfo->lpVtbl->Release(g_SystemFunctionTypeInfo);
      if (g_ConstantsTypeInfo) g_ConstantsTypeInfo->lpVtbl->Release(g_ConstantsTypeInfo);
    }
  }
  return TRUE;
}

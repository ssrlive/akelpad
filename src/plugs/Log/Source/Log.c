#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <richedit.h>
#include "StrFunc.h"
#include "x64Func.h"
#include "WideFunc.h"
#include "AkelEdit.h"
#include "RegExpFunc.h"
#include "AkelDLL.h"
#include "Resources\Resource.h"

/*
//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit.h"

//Include string functions
#define WideCharLower
#define xmemcpy
#define xmemset
#define xmemcmp
#define xstrlenW
#define xstrcpyW
#define xstrcpynW
#define xstrcmpiW
#define xstrcmpinW
#define xatoiW
#define xitoaW
#define xuitoaW
#define dec2hexW
#define xprintfW
#include "StrFunc.h"

//Include x64 functions
#define GetFileSize64
#define SetFilePointer64
#include "x64Func.h"

//Include wide functions
#define AppendMenuWide
#define CallWindowProcWide
#define ComboBox_AddStringWide
#define ComboBox_FindStringWide
#define CreateDialogWide
#define CreateFileWide
#define CreateProcessWide
#define DialogBoxWide
#define ExpandEnvironmentStringsWide
#define FileExistsWide
#define GetCPInfoExWide
#define GetCurrentDirectoryWide
#define GetFileAttributesWide
#define GetFullPathNameWide
#define GetLongPathNameWide
#define GetWindowLongPtrWide
#define GetWindowTextWide
#define PropertySheetWide
#define SetCurrentDirectoryWide
#define SetDlgItemTextWide
#define SetWindowLongPtrWide
#define SetWindowTextWide
#include "WideFunc.h"
//*/

//Defines
#define STRID_MSG_MODIFIED_ONSTART      1
#define STRID_MSG_MODIFIED_ONSTOP       2
#define STRID_MSG_ONSTOP                3
#define STRID_MSG_CANTOPENFILE_ONSTOP   4
#define STRID_MSG_NOFILE                5
#define STRID_UPDATEINTERVAL            6
#define STRID_MS                        7
#define STRID_SCROLL                    8
#define STRID_SCROLLENDAUTO             9
#define STRID_SCROLLENDCARET            10
#define STRID_SCROLLENDVERT             11
#define STRID_SCROLLENDNO               12
#define STRID_COMPLETEREOPEN            13
#define STRID_SETUP                     14
#define STRID_MSG_CANTEXECCOMMAND       15
#define STRID_MENUCOPY                  16
#define STRID_MENUSELECTALL             17
#define STRID_MENUCLEAR                 18
#define STRID_INPUT                     19
#define STRID_STOP                      20
#define STRID_RUNDLG                    21
#define STRID_EXEC                      22
#define STRID_COMMAND                   23
#define STRID_DIRECTORY                 24
#define STRID_SOURCE                    25
#define STRID_TARGET                    26
#define STRID_NO                        27
#define STRID_SELDOC                    28
#define STRID_ALLDOC                    29
#define STRID_ONLYSEL                   30
#define STRID_PANEL                     31
#define STRID_NEWTAB                    32
#define STRID_CODEPAGE                  33
#define STRID_AUTODETECT                34
#define STRID_PATTERN                   35
#define STRID_MENUMICROSOFTGCC          36
#define STRID_MENUPATBORLAND            37
#define STRID_PATTAGS                   38
#define STRID_MENUTAGFILE               39
#define STRID_MENUTAGGOTOLINE           40
#define STRID_MENUTAGGOTOBYTE           41
#define STRID_MENUTAGGOTOCHAR           42
#define STRID_APPENDNEXT                43
#define STRID_NOSCROLL                  44
#define STRID_WRAP                      45
#define STRID_HIDEINPUT                 46
#define STRID_SAVEALL                   47
#define STRID_HOTKEYS                   48
#define STRID_NEXTMATCH                 49
#define STRID_PREVMATCH                 50
#define STRID_RUNCMD                    51
#define STRID_RUNCMDDLG                 52
#define STRID_CODERALIAS                53
#define STRID_PLUGIN                    54
#define STRID_OK                        55
#define STRID_CANCEL                    56

#define DLLA_LOGSETTINGS            1
#define DLLA_LOGOUTPUT_EXEC         1
#define DLLA_LOGOUTPUT_GETWINDOW    2
#define DLLA_LOGOUTPUT_GETEXECINFO  3
#define DLLA_LOGOUTPUT_SETTEXTA     4
#define DLLA_LOGOUTPUT_SETTEXTW     5
#define DLLA_LOGOUTPUT_CLOSEPANEL   6

#ifndef CP_UNICODE_UTF16LE
  #define CP_UNICODE_UTF16LE 1200
#endif
#ifndef CP_UNICODE_UTF16BE
  #define CP_UNICODE_UTF16BE 1201
#endif

#define AKDLL_EXECSTART (WM_USER + 100)
#define AKDLL_EXECSTOP  (WM_USER + 101)
#define AKDLL_SHOWINPUT (WM_USER + 102)
#define AKDLL_SETUP     (WM_USER + 103)

#define OF_WATCH    0x01
#define OF_OUTPUT   0x02
#define OF_DOCKRECT 0x04
#define OF_RUN      0x08
#define OF_RUNRECT  0x10

#define COMMANDLINE_SIZE 32768

//DestroyDock type
#define DKT_DEFAULT        0x0
#define DKT_NOUNLOAD       0x1
#define DKT_ONMAINFINISH   0x2
#define DKT_KEEPAUTOLOAD   0x4
#define DKT_WAITTHREAD     0x8

//dwWatchScrollEnd
#define WSE_NO     0
#define WSE_CARET  1
#define WSE_VERT   2
#define WSE_AUTO   3

//Output exec state
#define OES_IDLE     0x0
#define OES_EXECUTED 0x1
#define OES_THREAD   0x2
#define OES_PROCESS  0x4

//Output flags
#define OUTF_NOPANEL         0x00000001
#define OUTF_HIDEINPUT       0x00000002
#define OUTF_SAVEALL         0x00000004
#define OUTF_APPENDNEXT      0x00000008
#define OUTF_NOSCROLL        0x00000010
#define OUTF_WRAP            0x00000020
#define OUTF_HIDEPANEL       0x00000040
#define OUTF_CLOSENOERROR    0x00000080
//Input source. Default is no input.
#define OUTF_SOURCESELDOC    0x00001000
#define OUTF_SOURCEONLYDOC   0x00002000
#define OUTF_SOURCEONLYSEL   0x00004000
//Output target. Default is panel.
#define OUTF_TARGETSELDOC    0x00100000
#define OUTF_TARGETONLYDOC   0x00200000
#define OUTF_TARGETONLYSEL   0x00400000
#define OUTF_TARGETNEWTAB    0x00800000

//Patterns
#define PAT_MICROSOFTGCC    L"(?-s)^\\s*(.*?)[(:](\\d++)([,:](\\d++))?[):]"
#define PAT_MICROSOFTGCCTAG L"/FILE=\\1 /GOTOLINE=\\2:\\4"
#define PAT_BORLAND         L"^(\\[.*?\\] )?(.*?)\\((\\d++)(,(\\d++))?\\)"
#define PAT_BORLANDTAG      L"/FILE=\\2 /GOTOLINE=\\3:\\5"

//Find document type
#define FDT_BYFILENAME   1
#define FDT_BYFRAME      2

//Set text append
#define STA_ERASE           0
#define STA_APPEND          1
#define STA_APPENDNEWLINE   2

//Code pages strings
#define STR_UNICODE_UTF8W  L"65001 (UTF-8)"
#define STR_UNICODE_UTF7W  L"65000 (UTF-7)"

//Property sheet
#define PAGE_WATCH  0
#define PAGE_OUTPUT 1
#define PAGE_MAX    2

#ifndef IDI_ICON_MAIN
  #define IDI_ICON_MAIN 1001
#endif
#ifndef PSCB_BUTTONPRESSED
  #define PSCB_BUTTONPRESSED 3
#endif

typedef struct {
  wchar_t wszPattern[BUFFER_SIZE];
  wchar_t wszPatTagFile[MAX_PATH];
  int nFindDocType;
  wchar_t wszPatTagLine[MAX_PATH];
  int nGotoType;
  int nInputCodePage;
  int nOutputCodePage;
  DWORD dwOutputFlags;

  wchar_t wszCmdLine[COMMANDLINE_SIZE];
  wchar_t wszDir[MAX_PATH];
  wchar_t wszInputWrite[BUFFER_SIZE];
  wchar_t wszCoderAlias[MAX_PATH];
  HANDLE hInitMutex;
  HANDLE hProcess;
  DWORD dwProcessId;
  DWORD dwExitCode;
  DWORD dwTimeOut;
  DWORD dwExecState;
  int nPrevFirstVisLine;
} OUTPUTEXEC;

typedef struct {
  HWND hWndConsole;
  DWORD dwConsoleProcessId;
} CONSOLEWINDOW;

//Coder external call
#define CODER_HIGHLIGHT     0x01
#define CODER_CODEFOLD      0x02

#define DLLA_CODER_GETALIAS      18

#define DLLA_HIGHLIGHT_ADDWINDOW 50
#define DLLA_HIGHLIGHT_DELWINDOW 51
#define DLLA_HIGHLIGHT_GETWINDOW 52

#define DLLA_CODEFOLD_ADDWINDOW  50
#define DLLA_CODEFOLD_DELWINDOW  51
#define DLLA_CODEFOLD_GETWINDOW  52

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  wchar_t *wpAlias;
} DLLEXTCODERADDWINDOW;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  BOOL *lpbFound;
} DLLEXTCODERGETWINDOW;

typedef struct {
  UINT_PTR dwStructSize;
  INT_PTR nAction;
  HWND hWndEdit;
  AEHDOC hDoc;
  wchar_t *wpAlias;
  INT_PTR *lpnAliasLen;
} DLLEXTCODERGETALIAS;

//Functions prototypes
void CreateOutput(PLUGINDATA *pd, BOOL bShow);
void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow);
void DestroyDock(HWND hWndDock, DWORD dwType);
void SetEditWindowSettings();
void SetWordWrap(DWORD dwOutputFlags);
void SetCoderAlias();
void RemoveCoderAlias();
BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DockRunDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewOutputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DockShowInput(BOOL bShow, HWND hWndInputEdit, HWND hWndInputButton, HWND hWndRunStopButton);

void SettingsSheet(int nStartPage);
LRESULT CALLBACK CBTProc(int iCode, WPARAM wParam, LPARAM lParam);
int CALLBACK PropSheetProc(HWND hDlg, UINT uMsg, LPARAM lParam);
BOOL CALLBACK WatchSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OutputSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
BOOL CALLBACK NextMatchProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK PrevMatchProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK RunCmdProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
BOOL CALLBACK RunCmdDlgProc(void *lpParameter, LPARAM lParam, DWORD dwSupport);
void OutputExec(OUTPUTEXEC *oe);
DWORD WINAPI ExecThreadProc(LPVOID lpParameter);
BOOL ExecuteToWindow(OUTPUTEXEC *oe);
BOOL OutputStop(HANDLE hProcess, BOOL bTryClose);
HWND GetConsoleWindowByProcessId(DWORD dwConsoleProcessId);
BOOL CALLBACK EnumConsoleProc(HWND hWnd, LPARAM lParam);
void AppendTextAnsi(HWND hWnd, int nCodePage, const char *pText, UINT_PTR dwTextLen);
void AppendTextWide(HWND hWnd, const wchar_t *wpText, UINT_PTR dwTextLen);
void ReplaceTextAnsi(HWND hWnd, int nCodePage, const char *pText, UINT_PTR dwTextLen, BOOL bSelect);
INT_PTR GetTextLength(HWND hWnd);
void GetOutputScroll(OUTPUTEXEC *oe);
void SetOutputScroll(OUTPUTEXEC *oe);
void FillOutputDefault(OUTPUTEXEC *oe);
BOOL GetPatOptions(const wchar_t *wpPatTags, OUTPUTEXEC *oe);
BOOL PatOpenLine(HWND hWnd, const OUTPUTEXEC *oe, const AECHARINDEX *ciChar, AETEXTRANGEW *tr);
BOOL GetLineAtIndex(HWND hWnd, const AECHARINDEX *ciChar, AETEXTRANGEW *tr);
void FillComboboxCodepage(HWND hWnd, int *lpCodepageList);
int SelectComboboxCodepage(HWND hWnd, int nCodepage);
void GetCodePageName(int nCodePage, wchar_t *wszCodePage, int nLen);
BOOL IsCodePageUnicode(int nCodePage);
void ChangeTwoBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen);
int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize);
BOOL GetFullName(const wchar_t *wpFile, wchar_t *wszFileFullName, int nFileMax, int *lpnFileLen);
BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc);
void ShowOutputMenu(HWND hWnd, HMENU hMenu, BOOL bMouse);
DWORD ScrollCaret(HWND hWnd);
int SaveLineScroll(HWND hWnd);
void RestoreLineScroll(HWND hWnd, int nBeforeLine);
int GetCommandLineArg(const wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, const wchar_t **wpNextArg);

INT_PTR WideOption(HANDLE hOptions, const wchar_t *pOptionName, DWORD dwType, BYTE *lpData, DWORD dwData);
void ReadOptions(DWORD dwFlags);
void SaveOptions(DWORD dwFlags);
const char* GetLangStringA(LANGID wLangID, int nStringID);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);
BOOL IsExtCallParamValid(LPARAM lParam, int nIndex);
INT_PTR GetExtCallParam(LPARAM lParam, int nIndex);
void InitCommon(PLUGINDATA *pd);
void InitMain();
void UninitMain();
void InitWatch();
void UninitWatch();
void StopWatch(int nStringID);
void InitOutput();
void UninitOutput();

//Global variables
wchar_t wszPluginName[MAX_PATH];
wchar_t wszPluginTitle[MAX_PATH];
wchar_t wszExeDir[MAX_PATH];
char szBuffer[BUFFER_SIZE];
wchar_t wszBuffer[BUFFER_SIZE];
wchar_t wszCmdLine[COMMANDLINE_SIZE];
HINSTANCE hInstanceDLL;
HINSTANCE hInstanceEXE;
HWND hMainWnd;
BOOL bOldWindows;
int nMDI;
LANGID wLangModule;
BOOL bInitCommon=FALSE;
int nInitMain=0;
BOOL bInitWatch=FALSE;
BOOL nInitOutput=FALSE;
DWORD dwSaveFlags=0;

OUTPUTEXEC oe;
PLUGINFUNCTION *pfOutput=NULL;
HWND hWndDockDlg=NULL;
HWND hWndOutputView=NULL;
HANDLE hExecThread=NULL;
RECT rcOutputCurrentDialog={0};
RECT rcOutputDockRect={0};
int nOutputDockSide=DKS_BOTTOM;
DOCK *dkOutputDlg=NULL;
BOOL bOutputDockWaitResize=FALSE;
BOOL bOnMainStart=FALSE;
wchar_t wszPattern[BUFFER_SIZE];
wchar_t wszPatTags[BUFFER_SIZE];
wchar_t wszCoderAlias[MAX_PATH];
DWORD dwDllFunction=0;
int nForceCodePage=CP_OEMCP;
BOOL bAutoCodePage=TRUE;
DWORD dwOutputFlags=0;
DWORD dwNextMatchKey=115;    //"F4"
DWORD dwPrevMatchKey=371;    //"Shift+F4"
DWORD dwRunCmdKey=0;
DWORD dwRunCmdDlgKey=0;
PLUGINFUNCTION *pfwNextMatch=NULL;
PLUGINFUNCTION *pfwPrevMatch=NULL;
PLUGINFUNCTION *pfwRunCmd=NULL;
PLUGINFUNCTION *pfwRunCmdDlg=NULL;
WNDPROC lpOldOutputProc;

wchar_t wszRunCmdLine[COMMANDLINE_SIZE];
wchar_t wszRunDir[MAX_PATH];
RECT rcRunMinMaxDialog={363, 270, 0, 0};
RECT rcRunCurrentDialog={0};

PLUGINFUNCTION *pfWatch=NULL;
HWND hWndEditWatch=NULL;
AEHDOC hDocEditWatch=NULL;
BOOL bDocOpening=FALSE;
BOOL bLogOpening=FALSE;
UINT_PTR dwCurPointer=0;
UINT_PTR dwUpdateTimer=0;
FILETIME ftCurTime={0};
DWORD dwWatchUpdateInterval=1000;
BOOL bWatchCompleteReopen=FALSE;
DWORD dwWatchScrollEnd=WSE_AUTO;
WNDPROCDATA *NewMainProcData=NULL;
WNDPROCDATA *NewFrameProcData=NULL;

//Options dialog
PROPSHEETHEADERW psh={0};
PROPSHEETPAGEW psp[PAGE_MAX]={0};
HWND hWndPropSheet=NULL;
HIMAGELIST hImageList;
HHOOK hHookOptions;

//Identification
void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
  pv->dwAkelDllVersion=AKELDLL;
  pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
  pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 9, 7, 0);
  pv->pPluginName="Log";
}

//Plugin extern function
void __declspec(dllexport) Watch(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_NOAUTOLOAD;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;
  if (pd->bOnStart) return;

  if (!bInitCommon) InitCommon(pd);
  pfWatch=pd->lpPluginFunction;

  if (bInitWatch)
  {
    UninitMain();
    UninitWatch();
    hWndEditWatch=NULL;
    hDocEditWatch=NULL;
    //MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_MSG_ONSTOP), wszPluginTitle, MB_OK|MB_ICONINFORMATION);

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    EDITINFO ei;

    if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
    {
      if (*ei.wszFile)
      {
        if (!ei.bModified)
        {
          hWndEditWatch=ei.hWndEdit;
          hDocEditWatch=ei.hDocEdit;
          InitMain();
          InitWatch();

          //Stay in memory, and show as active
          pd->nUnload=UD_NONUNLOAD_ACTIVE;
          return;
        }
        else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_MSG_MODIFIED_ONSTART), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
      }
      else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_MSG_NOFILE), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
    }
    else MessageBoxW(hMainWnd, GetLangStringW(wLangModule, STRID_MSG_NOFILE), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);

    //If any function loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
}

void __declspec(dllexport) Output(PLUGINDATA *pd)
{
  pd->dwSupport|=PDS_SUPPORTALL;
  if (pd->dwSupport & PDS_GETSUPPORT)
    return;

  if (!bInitCommon) InitCommon(pd);
  pfOutput=pd->lpPluginFunction;

  if (pd->lParam)
  {
    INT_PTR nAction=GetExtCallParam(pd->lParam, 1);

    if (nAction == DLLA_LOGOUTPUT_EXEC)
    {
      if (!hExecThread)
      {
        unsigned char *pCmd=NULL;
        unsigned char *pDir=NULL;
        unsigned char *pPattern=NULL;
        unsigned char *pPatTags=NULL;
        unsigned char *pCoderAlias=NULL;

        xmemset(&oe, 0, sizeof(OUTPUTEXEC));
        FillOutputDefault(&oe);
        oe.dwTimeOut=(DWORD)-1;

        if (IsExtCallParamValid(pd->lParam, 2))
          pCmd=(unsigned char *)GetExtCallParam(pd->lParam, 2);
        if (IsExtCallParamValid(pd->lParam, 3))
          pDir=(unsigned char *)GetExtCallParam(pd->lParam, 3);
        if (IsExtCallParamValid(pd->lParam, 4))
          pPattern=(unsigned char *)GetExtCallParam(pd->lParam, 4);
        if (IsExtCallParamValid(pd->lParam, 5))
          pPatTags=(unsigned char *)GetExtCallParam(pd->lParam, 5);
        if (IsExtCallParamValid(pd->lParam, 6))
          oe.nInputCodePage=(int)GetExtCallParam(pd->lParam, 6);
        if (IsExtCallParamValid(pd->lParam, 7))
          oe.nOutputCodePage=(int)GetExtCallParam(pd->lParam, 7);
        if (IsExtCallParamValid(pd->lParam, 8))
          oe.dwOutputFlags=(DWORD)GetExtCallParam(pd->lParam, 8);
        if (IsExtCallParamValid(pd->lParam, 9))
          pCoderAlias=(unsigned char *)GetExtCallParam(pd->lParam, 9);

        if (pCmd)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pCmd, -1, oe.wszCmdLine, COMMANDLINE_SIZE);
          else
            xstrcpynW(oe.wszCmdLine, (wchar_t *)pCmd, COMMANDLINE_SIZE);
        }
        if (pDir)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pDir, -1, oe.wszDir, MAX_PATH);
          else
            xstrcpynW(oe.wszDir, (wchar_t *)pDir, MAX_PATH);
        }
        if (pPattern)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pPattern, -1, oe.wszPattern, BUFFER_SIZE);
          else
            xstrcpynW(oe.wszPattern, (wchar_t *)pPattern, BUFFER_SIZE);
        }
        if (pPatTags)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pPatTags, -1, wszBuffer, MAX_PATH);
          else
            xstrcpynW(wszBuffer, (wchar_t *)pPatTags, MAX_PATH);
          GetPatOptions(wszBuffer, &oe);
        }
        if (oe.nInputCodePage == -2)
          oe.nInputCodePage=bAutoCodePage?-1:nForceCodePage;
        if (oe.nOutputCodePage == -2)
          oe.nOutputCodePage=bAutoCodePage?-1:nForceCodePage;
        if (pCoderAlias)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pCoderAlias, -1, oe.wszCoderAlias, MAX_PATH);
          else
            xstrcpynW(oe.wszCoderAlias, (wchar_t *)pCoderAlias, MAX_PATH);
        }

        if (oe.dwOutputFlags & OUTF_NOPANEL)
        {
          if (!hExecThread && *oe.wszCmdLine)
            OutputExec(&oe);
        }
        else
        {
          if (!nInitOutput)
          {
            CreateOutput(pd, !(oe.dwOutputFlags & OUTF_HIDEPANEL));
            SetCoderAlias();

            //Stay in memory, and show as active
            pd->nUnload=UD_NONUNLOAD_ACTIVE;
            return;
          }
          SetCoderAlias();
          oe.dwExecState=OES_EXECUTED;
          PostMessage(hWndDockDlg, AKDLL_SHOWINPUT, !(oe.dwOutputFlags & OUTF_HIDEINPUT), 0);
          PostMessage(hWndDockDlg, AKDLL_EXECSTART, 0, 0);
        }
      }

      //Stay in memory, and show as active
      pd->nUnload=UD_NONUNLOAD_ACTIVE;
      return;
    }
    else if (nAction == DLLA_LOGOUTPUT_GETWINDOW)
    {
      HWND *lpWndOutput=NULL;
      DOCK **lppDock=NULL;

      if (IsExtCallParamValid(pd->lParam, 2))
        lpWndOutput=(HWND *)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        lppDock=(DOCK **)GetExtCallParam(pd->lParam, 3);

      if (lpWndOutput)
        *lpWndOutput=hWndOutputView;
      if (lppDock)
        *lppDock=dkOutputDlg;
    }
    else if (nAction == DLLA_LOGOUTPUT_GETEXECINFO)
    {
      DWORD *lpdwExecState=NULL;
      HANDLE *lphExecThread=NULL;
      HANDLE *lphProcess=NULL;
      DWORD *lpdwProcessId=NULL;
      DWORD *lpdwExitCode=NULL;

      if (IsExtCallParamValid(pd->lParam, 2))
        lpdwExecState=(DWORD *)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        lphExecThread=(HANDLE *)GetExtCallParam(pd->lParam, 3);
      if (IsExtCallParamValid(pd->lParam, 4))
        lphProcess=(HANDLE *)GetExtCallParam(pd->lParam, 4);
      if (IsExtCallParamValid(pd->lParam, 5))
        lpdwProcessId=(DWORD *)GetExtCallParam(pd->lParam, 5);
      if (IsExtCallParamValid(pd->lParam, 6))
        lpdwExitCode=(DWORD *)GetExtCallParam(pd->lParam, 6);

      if (lpdwExecState)
        *lpdwExecState=oe.dwExecState;
      if (lphExecThread)
        *lphExecThread=hExecThread;
      if (lphProcess)
        *lphProcess=oe.hProcess;
      if (lpdwProcessId)
        *lpdwProcessId=oe.dwProcessId;
      if (lpdwExitCode)
        *lpdwExitCode=oe.dwExitCode;
    }
    else if (nAction == DLLA_LOGOUTPUT_SETTEXTA ||
             nAction == DLLA_LOGOUTPUT_SETTEXTW)
    {
      unsigned char *pText=NULL;
      unsigned char *pCoderAlias=NULL;
      INT_PTR nTextLen=-1;
      int nAppend=STA_APPEND;
      int nCodePage=0;

      if (IsExtCallParamValid(pd->lParam, 2))
        pText=(unsigned char *)GetExtCallParam(pd->lParam, 2);
      if (IsExtCallParamValid(pd->lParam, 3))
        nTextLen=GetExtCallParam(pd->lParam, 3);
      if (IsExtCallParamValid(pd->lParam, 4))
        nAppend=(BOOL)GetExtCallParam(pd->lParam, 4);
      if (IsExtCallParamValid(pd->lParam, 5))
        nCodePage=(int)GetExtCallParam(pd->lParam, 5);
      if (IsExtCallParamValid(pd->lParam, 6))
        pCoderAlias=(unsigned char *)GetExtCallParam(pd->lParam, 6);

      if (pText)
      {
        if (!nCodePage)
          nCodePage=CP_ACP;
        if (pCoderAlias)
        {
          if (pd->dwSupport & PDS_STRANSI)
            MultiByteToWideChar(CP_ACP, 0, (char *)pCoderAlias, -1, oe.wszCoderAlias, MAX_PATH);
          else
            xstrcpynW(oe.wszCoderAlias, (wchar_t *)pCoderAlias, MAX_PATH);
        }

        if (!nInitOutput)
        {
          oe.dwOutputFlags|=OUTF_HIDEINPUT;
          CreateOutput(pd, TRUE);
        }
        //else if (!(oe.dwOutputFlags & OUTF_HIDEINPUT) && oe.dwExecState == OES_IDLE)
        //{
        //  oe.dwOutputFlags|=OUTF_HIDEINPUT;
        //  PostMessage(hWndDockDlg, AKDLL_SHOWINPUT, FALSE, 0);
        //}

        SetCoderAlias();
        GetOutputScroll(&oe);

        if (pd->dwSupport & PDS_STRANSI)
        {
          if (nAppend)
          {
            if (nAppend == STA_APPENDNEWLINE)
            {
              AECHARINDEX ciLastChar;

              SendMessage(hWndOutputView, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ciLastChar);
              if (ciLastChar.nCharInLine)
                AppendTextAnsi(hWndOutputView, nCodePage, (char *)"\n", 1);
            }
            AppendTextAnsi(hWndOutputView, nCodePage, (char *)pText, nTextLen);
          }
          else
          {
            AESETTEXTA st;

            st.pText=(char *)pText;
            st.dwTextLen=nTextLen;
            st.nNewLine=AELB_ASINPUT;
            st.nCodePage=nCodePage;
            SendMessage(hWndOutputView, AEM_EXSETTEXTA, 0, (LPARAM)&st);
          }
        }
        else
        {
          if (nAppend)
          {
            if (nAppend == STA_APPENDNEWLINE)
            {
              AECHARINDEX ciLastChar;

              SendMessage(hWndOutputView, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&ciLastChar);
              if (ciLastChar.nCharInLine)
                AppendTextWide(hWndOutputView, (wchar_t *)L"\n", 1);
            }
            AppendTextWide(hWndOutputView, (wchar_t *)pText, nTextLen);
          }
          else
          {
            AESETTEXTW st;

            st.pText=(wchar_t *)pText;
            st.dwTextLen=nTextLen;
            st.nNewLine=AELB_ASINPUT;
            SendMessage(hWndOutputView, AEM_EXSETTEXTW, 0, (LPARAM)&st);
          }
        }
        SetOutputScroll(&oe);

        //Stay in memory, and show as active
        pd->nUnload=UD_NONUNLOAD_ACTIVE;
        return;
      }
    }
    else if (nAction == DLLA_LOGOUTPUT_CLOSEPANEL)
    {
      DestroyDock(hWndDockDlg, DKT_KEEPAUTOLOAD);
    }

    //If plugin already loaded, stay in memory and don't change active status
    if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_UNCHANGE;
    return;
  }

  if (nInitOutput)
  {
    DestroyDock(hWndDockDlg, DKT_KEEPAUTOLOAD);

    //If any function still loaded, stay in memory and show as non-active
    if (nInitMain) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
  }
  else
  {
    bOutputDockWaitResize=pd->bOnStart;
    CreateOutput(pd, !bOutputDockWaitResize);
    SetCoderAlias();

    //Stay in memory, and show as active
    pd->nUnload=UD_NONUNLOAD_ACTIVE;
  }
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

    if (nAction == DLLA_LOGSETTINGS)
    {
      int nStartPage=0;

      if (IsExtCallParamValid(pd->lParam, 2))
        nStartPage=(int)GetExtCallParam(pd->lParam, 2);

      SettingsSheet(nStartPage);
    }
  }
  else
  {
    SettingsSheet(0);
  }

  //If plugin already loaded, stay in memory, but show as non-active
  if (pd->bInMemory) pd->nUnload=UD_NONUNLOAD_NONACTIVE;
}

void CreateOutput(PLUGINDATA *pd, BOOL bShow)
{
  InitMain();
  InitOutput();

  CreateDock(&hWndDockDlg, &dkOutputDlg, bShow);
}

void CreateDock(HWND *hWndDock, DOCK **dkDock, BOOL bShow)
{
  DOCK dk;

  *hWndDock=NULL;

  xmemset(&dk, 0, sizeof(DOCK));
  dk.dwFlags=DKF_DRAGDROP;
  dk.nSide=nOutputDockSide;
  dk.rcSize=rcOutputDockRect;

  if (*dkDock=(DOCK *)SendMessage(hMainWnd, AKD_DOCK, DK_ADD, (LPARAM)&dk))
  {
    *hWndDock=CreateDialogWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_OUTPUT_DOCK), hMainWnd, (DLGPROC)DockDlgProc);

    if (*hWndDock)
    {
      if (!(*dkDock)->rcSize.right || !(*dkDock)->rcSize.bottom)
      {
        GetWindowRect(*hWndDock, &(*dkDock)->rcSize);
        (*dkDock)->rcSize.right-=(*dkDock)->rcSize.left;
        (*dkDock)->rcSize.bottom-=(*dkDock)->rcSize.top;
        (*dkDock)->rcSize.left=0;
        (*dkDock)->rcSize.top=0;
      }
      (*dkDock)->hWnd=*hWndDock;
      SendMessage(hMainWnd, AKD_DOCK, DK_SUBCLASS, (LPARAM)*dkDock);

      if (bShow)
        SendMessage(hMainWnd, AKD_DOCK, DK_SHOW, (LPARAM)*dkDock);
      else
        (*dkDock)->dwFlags|=DKF_HIDDEN;
    }
  }
}

void DestroyDock(HWND hWndDock, DWORD dwType)
{
  SendMessage(hWndDock, WM_COMMAND, IDCANCEL, dwType);
}

void SetEditWindowSettings()
{
  HFONT hFontEdit;

  SendMessage(hWndOutputView, EM_SETUNDOLIMIT, 0, 0);
  SendMessage(hWndOutputView, AEM_SETOPTIONS, AECOOP_OR, AECO_READONLY);
  SetWordWrap(oe.dwOutputFlags);
  hFontEdit=(HFONT)SendMessage(hMainWnd, AKD_GETFONT, (WPARAM)NULL, (LPARAM)NULL);
  SendMessage(hWndOutputView, WM_SETFONT, (WPARAM)hFontEdit, FALSE);
}

void SetWordWrap(DWORD dwOutputFlags)
{
  if (dwOutputFlags & OUTF_WRAP)
  {
    SendMessage(hWndOutputView, AEM_SETWORDWRAP, AEWW_WORD, 0);
    SendMessage(hWndOutputView, AEM_SHOWSCROLLBAR, SB_HORZ, FALSE);
  }
  else
  {
    SendMessage(hWndOutputView, AEM_SETWORDWRAP, AEWW_NONE, 0);
    SendMessage(hWndOutputView, AEM_SHOWSCROLLBAR, SB_HORZ, TRUE);
  }
}

void SetCoderAlias()
{
  if (*oe.wszCoderAlias || dwDllFunction)
  {
    PLUGINFUNCTION *pfHighLight;
    PLUGINFUNCTION *pfCodeFold;
    PLUGINCALLSENDW pcs;
    DLLEXTCODERGETALIAS decga;
    DLLEXTCODERADDWINDOW decaw;
    DLLEXTCODERGETWINDOW decgw;
    wchar_t wszAlias[MAX_PATH];
    INT_PTR nAliasLen=0;
    BOOL bFound=FALSE;

    pfHighLight=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0);
    pfCodeFold=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::CodeFold", 0);

    if ((pfHighLight && pfHighLight->bRunning) ||
        (pfCodeFold && pfCodeFold->bRunning))
    {
      //Check is window already associated
      if (!bFound && pfHighLight && pfHighLight->bRunning)
      {
        decgw.dwStructSize=sizeof(DLLEXTCODERGETWINDOW);
        decgw.nAction=DLLA_HIGHLIGHT_GETWINDOW;
        decgw.hWndEdit=hWndOutputView;
        decgw.lpbFound=&bFound;

        pcs.pFunction=L"Coder::HighLight";
        pcs.lParam=(LPARAM)&decgw;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
      }
      if (!bFound && pfCodeFold && pfCodeFold->bRunning)
      {
        decgw.dwStructSize=sizeof(DLLEXTCODERGETWINDOW);
        decgw.nAction=DLLA_CODEFOLD_GETWINDOW;
        decgw.hWndEdit=hWndOutputView;
        decgw.lpbFound=&bFound;

        pcs.pFunction=L"Coder::CodeFold";
        pcs.lParam=(LPARAM)&decgw;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
      }

      //If window associated, get current alias and compare with a new one (oe.wszCoderAlias).
      if (bFound)
      {
        decga.dwStructSize=sizeof(DLLEXTCODERGETALIAS);
        decga.nAction=DLLA_CODER_GETALIAS;
        decga.hWndEdit=hWndOutputView;
        decga.hDoc=NULL;
        decga.wpAlias=wszAlias;
        decga.lpnAliasLen=&nAliasLen;

        pcs.pFunction=L"Coder::Settings";
        pcs.lParam=(LPARAM)&decga;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

        if (xstrcmpiW(wszAlias, oe.wszCoderAlias))
          RemoveCoderAlias();
        else
          return;
      }

      if (*oe.wszCoderAlias)
      {
        //Associate window with Coder theme
        if (pfHighLight && pfHighLight->bRunning)
        {
          decaw.dwStructSize=sizeof(DLLEXTCODERADDWINDOW);
          decaw.nAction=DLLA_HIGHLIGHT_ADDWINDOW;
          decaw.hWndEdit=hWndOutputView;
          decaw.wpAlias=oe.wszCoderAlias;

          pcs.pFunction=L"Coder::HighLight";
          pcs.lParam=(LPARAM)&decaw;
          pcs.dwSupport=PDS_STRWIDE;
          SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

          dwDllFunction|=CODER_HIGHLIGHT;
        }
        if (pfCodeFold && pfCodeFold->bRunning)
        {
          decaw.dwStructSize=sizeof(DLLEXTCODERADDWINDOW);
          decaw.nAction=DLLA_CODEFOLD_ADDWINDOW;
          decaw.hWndEdit=hWndOutputView;
          decaw.wpAlias=oe.wszCoderAlias;

          pcs.pFunction=L"Coder::CodeFold";
          pcs.lParam=(LPARAM)&decaw;
          pcs.dwSupport=PDS_STRWIDE;
          SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);

          dwDllFunction|=CODER_CODEFOLD;
        }
        SetEditWindowSettings();
      }
    }
  }
}

void RemoveCoderAlias()
{
  if (dwDllFunction)
  {
    PLUGINFUNCTION *pfHighLight;
    PLUGINFUNCTION *pfCodeFold;
    PLUGINCALLSENDW pcs;
    DLLEXTCODERADDWINDOW decaw;

    if (dwDllFunction & CODER_CODEFOLD)
    {
      if ((pfCodeFold=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::CodeFold", 0)) && pfCodeFold->bRunning)
      {
        decaw.dwStructSize=sizeof(DLLEXTCODERADDWINDOW);
        decaw.nAction=DLLA_CODEFOLD_DELWINDOW;
        decaw.hWndEdit=hWndOutputView;

        pcs.pFunction=L"Coder::CodeFold";
        pcs.lParam=(LPARAM)&decaw;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
      }
    }
    if (dwDllFunction & CODER_HIGHLIGHT)
    {
      if ((pfHighLight=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)L"Coder::HighLight", 0)) && pfHighLight->bRunning)
      {
        decaw.dwStructSize=sizeof(DLLEXTCODERADDWINDOW);
        decaw.nAction=DLLA_HIGHLIGHT_DELWINDOW;
        decaw.hWndEdit=hWndOutputView;

        pcs.pFunction=L"Coder::HighLight";
        pcs.lParam=(LPARAM)&decaw;
        pcs.dwSupport=PDS_STRWIDE;
        SendMessage(hMainWnd, AKD_DLLCALLW, 0, (LPARAM)&pcs);
      }
    }
    dwDllFunction=0;
  }
}

BOOL CALLBACK DockDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndInputEdit;
  static HWND hWndInputButton;
  static HWND hWndRunStopButton;
  static HWND hWndTitleText;
  static HWND hWndTitleClose;
  static HMENU hMenuEdit;
  static RESIZEDIALOG rds[]={{&hWndTitleText,     RDS_SIZE|RDS_X, 0},
                             {&hWndTitleClose,    RDS_MOVE|RDS_X, 0},
                             {&hWndOutputView,    RDS_SIZE|RDS_X, 0},
                             {&hWndOutputView,    RDS_SIZE|RDS_Y, 0},
                             {&hWndInputEdit,     RDS_SIZE|RDS_X, 0},
                             {&hWndInputEdit,     RDS_MOVE|RDS_Y, 0},
                             {&hWndInputButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndInputButton,   RDS_MOVE|RDS_Y, 0},
                             {&hWndRunStopButton, RDS_MOVE|RDS_X, 0},
                             {&hWndRunStopButton, RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    hWndOutputView=GetDlgItem(hDlg, IDC_DOCK_OUTPUTWATCH);
    hWndInputEdit=GetDlgItem(hDlg, IDC_DOCK_EXECINPUT_EDIT);
    hWndInputButton=GetDlgItem(hDlg, IDC_DOCK_INPUT_BUTTON);
    hWndRunStopButton=GetDlgItem(hDlg, IDC_DOCK_RUNSTOP_BUTTON);
    hWndTitleText=GetDlgItem(hDlg, IDC_TITLETEXT);
    hWndTitleClose=GetDlgItem(hDlg, IDC_CLOSE);

    SetDlgItemTextWide(hDlg, IDC_DOCK_INPUT_BUTTON, GetLangStringW(wLangModule, STRID_INPUT));
    SetWindowTextWide(hWndRunStopButton, GetLangStringW(wLangModule, STRID_RUNDLG));

    SetEditWindowSettings();
    EnableWindow(hWndInputEdit, FALSE);
    EnableWindow(hWndInputButton, FALSE);
    if (oe.dwOutputFlags & OUTF_HIDEINPUT)
      DockShowInput(FALSE, hWndInputEdit, hWndInputButton, hWndRunStopButton);

    if (hMenuEdit=CreatePopupMenu())
    {
      AppendMenuWide(hMenuEdit, MF_STRING, IDM_OUTPUT_COPY, GetLangStringW(wLangModule, STRID_MENUCOPY));
      AppendMenuWide(hMenuEdit, MF_STRING, IDM_OUTPUT_SELECTALL, GetLangStringW(wLangModule, STRID_MENUSELECTALL));
      AppendMenuWide(hMenuEdit, MF_SEPARATOR, 0, NULL);
      AppendMenuWide(hMenuEdit, MF_STRING, IDM_OUTPUT_CLEAR, GetLangStringW(wLangModule, STRID_MENUCLEAR));
      AppendMenuWide(hMenuEdit, MF_SEPARATOR, 0, NULL);
      AppendMenuWide(hMenuEdit, MF_STRING, IDM_OUTPUT_SETUP, GetLangStringW(wLangModule, STRID_SETUP));
    }

    //Set dock title
    {
      BUTTONDRAW bd={BIF_CROSS|BIF_ETCHED};

      xprintfW(wszBuffer, L"%s::Output", wszPluginName);
      SetWindowTextWide(hWndTitleText, wszBuffer);
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndTitleClose, (LPARAM)&bd);
    }

    lpOldOutputProc=(WNDPROC)GetWindowLongPtrWide(hWndOutputView, GWLP_WNDPROC);
    SetWindowLongPtrWide(hWndOutputView, GWLP_WNDPROC, (UINT_PTR)NewOutputProc);

    //External call filled OUTPUTEXEC structure
    if (*oe.wszCmdLine)
    {
      oe.dwExecState=OES_EXECUTED;
      PostMessage(hDlg, AKDLL_EXECSTART, 0, 0);
    }
  }
  else if (uMsg == AKDLL_EXECSTART)
  {
    if (!hExecThread && *oe.wszCmdLine)
    {
      EnableWindow(hWndInputEdit, TRUE);
      EnableWindow(hWndInputButton, TRUE);
      SetWindowTextWide(hWndRunStopButton, GetLangStringW(wLangModule, STRID_STOP));
      OutputExec(&oe);
    }
    else oe.dwExecState=OES_IDLE;
  }
  else if (uMsg == AKDLL_EXECSTOP)
  {
    if (hExecThread)
      OutputStop(oe.hProcess, TRUE);
    EnableWindow(hWndInputEdit, FALSE);
    EnableWindow(hWndInputButton, FALSE);
    SetWindowTextWide(hWndRunStopButton, GetLangStringW(wLangModule, STRID_RUNDLG));
  }
  else if (uMsg == AKDLL_SHOWINPUT)
  {
    BOOL bResult=FALSE;

    if (wParam && !IsWindowVisible(hWndInputEdit))
    {
      DockShowInput(TRUE, hWndInputEdit, hWndInputButton, hWndRunStopButton);
      bResult=TRUE;
    }
    else if (!wParam && IsWindowVisible(hWndInputEdit))
    {
      DockShowInput(FALSE, hWndInputEdit, hWndInputButton, hWndRunStopButton);
      bResult=TRUE;
    }

    //Update RESIZEDIALOG offsets
    if (bResult)
    {
      RESIZEDIALOGMSG rdsm={&rds[0], NULL, &rcOutputCurrentDialog, 0, hDlg, WM_INITDIALOG, wParam, lParam};

      SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm);
    }
    return FALSE;
  }
  else if (uMsg == AKDLL_SETUP)
  {
    SettingsSheet(PAGE_OUTPUT);
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    POINT pt;
    RECT rc;

    GetCursorPos(&pt);
    GetWindowRect(hWndTitleText, &rc);

    if (PtInRect(&rc, pt))
    {
      PostMessage(hDlg, AKDLL_SETUP, 0, 0);
    }
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam == hDlg)
    {
      POINT pt;
      RECT rc;

      GetCursorPos(&pt);
      GetWindowRect(hWndTitleText, &rc);

      if (PtInRect(&rc, pt))
        ShowOutputMenu(hWndOutputView, hMenuEdit, TRUE);
    }
    else if ((HWND)wParam == hWndOutputView)
    {
      ShowOutputMenu(hWndOutputView, hMenuEdit, lParam != -1);
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK ||
        LOWORD(wParam) == IDC_DOCK_INPUT_BUTTON)
    {
      if (hExecThread)
        GetWindowTextWide(hWndInputEdit, oe.wszInputWrite, BUFFER_SIZE);
    }
    else if (LOWORD(wParam) == IDC_DOCK_RUNSTOP_BUTTON)
    {
      if (hExecThread)
        PostMessage(hWndDockDlg, AKDLL_EXECSTOP, 0, 0);
      else
        DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_OUTPUT_RUN), hDlg, (DLGPROC)DockRunDlgProc);
    }
    else if (LOWORD(wParam) == IDC_CLOSE ||
             LOWORD(wParam) == IDCANCEL)
    {
      if (lParam & DKT_WAITTHREAD)
      {
        if (hExecThread)
          WaitForSingleObject(hExecThread, INFINITE);
      }

      if (!(lParam & DKT_ONMAINFINISH))
        RemoveCoderAlias();

      //Save OF_DOCKRECT
      if (dwSaveFlags)
      {
        SaveOptions(dwSaveFlags);
        dwSaveFlags=0;
      }

      UninitMain();
      UninitOutput();
      SendMessage(hMainWnd, AKD_DOCK, DK_DELETE, (LPARAM)dkOutputDlg);
      dkOutputDlg=NULL;

      DestroyMenu(hMenuEdit);
      DestroyWindow(hWndDockDlg);
      hWndDockDlg=NULL;
      hWndOutputView=NULL;

      if (!(lParam & DKT_ONMAINFINISH))
      {
        SendMessage(hMainWnd, AKD_RESIZE, 0, 0);
        SetFocus(hMainWnd);

        if (!(lParam & DKT_KEEPAUTOLOAD))
        {
          pfOutput->bAutoLoad=FALSE;
          SendMessage(hMainWnd, AKD_DLLSAVE, DLLSF_ONEXIT, 0);
        }
        if ((lParam & DKT_NOUNLOAD) || nInitMain)
        {
          pfOutput->bRunning=FALSE;
          SendMessage(hMainWnd, WM_COMMAND, 0, 0);
        }
        else SendMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)NULL);
      }
    }
  }

  //Translate plugins hotkeys
  {
    MSG msg={hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_TRANSLATEMESSAGE, TMSG_HOTKEY, (LPARAM)&msg))
      return FALSE;
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], NULL, &rcOutputCurrentDialog, 0, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      if (dkOutputDlg) GetWindowSize(hWndTitleText, hDlg, &dkOutputDlg->rcDragDrop);
  }

  return FALSE;
}

BOOL CALLBACK DockRunDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HICON hPluginIcon;
  static HWND hWndCmdEdit;
  static HWND hWndDirEdit;
  static HWND hWndSourceLabel;
  static HWND hWndSourceCombo;
  static HWND hWndTargetLabel;
  static HWND hWndTargetCombo;
  static HWND hWndExecButton;
  static HWND hWndCancelButton;
  static RESIZEDIALOG rds[]={{&hWndCmdEdit,      RDS_SIZE|RDS_X, 0},
                             {&hWndDirEdit,      RDS_SIZE|RDS_X, 0},
                             {&hWndSourceLabel,  RDS_MOVE|RDS_Y, 0},
                             {&hWndSourceCombo,  RDS_MOVE|RDS_Y, 0},
                             {&hWndTargetLabel,  RDS_MOVE|RDS_X, 0},
                             {&hWndTargetLabel,  RDS_MOVE|RDS_Y, 0},
                             {&hWndTargetCombo,  RDS_MOVE|RDS_X, 0},
                             {&hWndTargetCombo,  RDS_MOVE|RDS_Y, 0},
                             {&hWndExecButton,   RDS_MOVE|RDS_X, 0},
                             {&hWndExecButton,   RDS_MOVE|RDS_Y, 0},
                             {&hWndCancelButton, RDS_MOVE|RDS_X, 0},
                             {&hWndCancelButton, RDS_MOVE|RDS_Y, 0},
                             {0, 0, 0}};

  if (uMsg == WM_INITDIALOG)
  {
    //Load plugin icon
    hPluginIcon=LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_OUTPUT));
    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hPluginIcon);

    hWndCmdEdit=GetDlgItem(hDlg, IDC_OUTPUT_RUN_CMD_EDIT);
    hWndDirEdit=GetDlgItem(hDlg, IDC_OUTPUT_RUN_DIR_EDIT);
    hWndSourceLabel=GetDlgItem(hDlg, IDC_OUTPUT_RUN_SOURCE_LABEL);
    hWndSourceCombo=GetDlgItem(hDlg, IDC_OUTPUT_RUN_SOURCE_COMBO);
    hWndTargetLabel=GetDlgItem(hDlg, IDC_OUTPUT_RUN_TARGET_LABEL);
    hWndTargetCombo=GetDlgItem(hDlg, IDC_OUTPUT_RUN_TARGET_COMBO);
    hWndExecButton=GetDlgItem(hDlg, IDOK);
    hWndCancelButton=GetDlgItem(hDlg, IDCANCEL);

    SetWindowTextWide(hDlg, GetLangStringW(wLangModule, STRID_RUNDLG));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_RUN_CMD_LABEL, GetLangStringW(wLangModule, STRID_COMMAND));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_RUN_DIR_LABEL, GetLangStringW(wLangModule, STRID_DIRECTORY));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_RUN_SOURCE_LABEL, GetLangStringW(wLangModule, STRID_SOURCE));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_RUN_TARGET_LABEL, GetLangStringW(wLangModule, STRID_TARGET));
    SetDlgItemTextWide(hDlg, IDOK, GetLangStringW(wLangModule, STRID_EXEC));
    SetDlgItemTextWide(hDlg, IDCANCEL, GetLangStringW(wLangModule, STRID_CANCEL));

    SendMessage(hWndCmdEdit, EM_LIMITTEXT, COMMANDLINE_SIZE, 0);
    SendMessage(hWndDirEdit, EM_LIMITTEXT, MAX_PATH, 0);
    SetWindowTextWide(hWndCmdEdit, wszRunCmdLine);
    SetWindowTextWide(hWndDirEdit, wszRunDir);

    ComboBox_AddStringWide(hWndSourceCombo, GetLangStringW(wLangModule, STRID_NO));
    ComboBox_AddStringWide(hWndSourceCombo, GetLangStringW(wLangModule, STRID_SELDOC));
    ComboBox_AddStringWide(hWndSourceCombo, GetLangStringW(wLangModule, STRID_ALLDOC));
    ComboBox_AddStringWide(hWndSourceCombo, GetLangStringW(wLangModule, STRID_ONLYSEL));
    if (dwOutputFlags & OUTF_SOURCESELDOC)
      SendMessage(hWndSourceCombo, CB_SETCURSEL, 1, 0);
    else if (dwOutputFlags & OUTF_SOURCEONLYDOC)
      SendMessage(hWndSourceCombo, CB_SETCURSEL, 2, 0);
    else if (dwOutputFlags & OUTF_SOURCEONLYSEL)
      SendMessage(hWndSourceCombo, CB_SETCURSEL, 3, 0);
    else
      SendMessage(hWndSourceCombo, CB_SETCURSEL, 0, 0);

    ComboBox_AddStringWide(hWndTargetCombo, GetLangStringW(wLangModule, STRID_PANEL));
    ComboBox_AddStringWide(hWndTargetCombo, GetLangStringW(wLangModule, STRID_SELDOC));
    ComboBox_AddStringWide(hWndTargetCombo, GetLangStringW(wLangModule, STRID_ALLDOC));
    ComboBox_AddStringWide(hWndTargetCombo, GetLangStringW(wLangModule, STRID_ONLYSEL));
    ComboBox_AddStringWide(hWndTargetCombo, GetLangStringW(wLangModule, STRID_NEWTAB));

    if (dwOutputFlags & OUTF_TARGETSELDOC)
      SendMessage(hWndTargetCombo, CB_SETCURSEL, 1, 0);
    else if (dwOutputFlags & OUTF_TARGETONLYDOC)
      SendMessage(hWndTargetCombo, CB_SETCURSEL, 3, 0);
    else if (dwOutputFlags & OUTF_TARGETONLYSEL)
      SendMessage(hWndTargetCombo, CB_SETCURSEL, 2, 0);
    else if (dwOutputFlags & OUTF_TARGETNEWTAB)
      SendMessage(hWndTargetCombo, CB_SETCURSEL, 4, 0);
    else
      SendMessage(hWndTargetCombo, CB_SETCURSEL, 0, 0);
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      int nSelection;

      if (!hExecThread)
      {
        xmemset(&oe, 0, sizeof(OUTPUTEXEC));
        FillOutputDefault(&oe);
        oe.dwTimeOut=(DWORD)-1;

        GetWindowTextWide(hWndCmdEdit, wszRunCmdLine, COMMANDLINE_SIZE);
        GetWindowTextWide(hWndDirEdit, wszRunDir, MAX_PATH);
        xstrcpynW(oe.wszCmdLine, wszRunCmdLine, COMMANDLINE_SIZE);
        xstrcpynW(oe.wszDir, wszRunDir, MAX_PATH);

        //Set source/target
        nSelection=(int)SendMessage(hWndSourceCombo, CB_GETCURSEL, 0, 0);
        dwOutputFlags&=~OUTF_SOURCESELDOC & ~OUTF_SOURCEONLYDOC & ~OUTF_SOURCEONLYSEL;
        if (nSelection == 1)
          dwOutputFlags|=OUTF_SOURCESELDOC;
        else if (nSelection == 2)
          dwOutputFlags|=OUTF_SOURCEONLYDOC;
        else if (nSelection == 3)
          dwOutputFlags|=OUTF_SOURCEONLYSEL;

        nSelection=(int)SendMessage(hWndTargetCombo, CB_GETCURSEL, 0, 0);
        dwOutputFlags&=~OUTF_TARGETSELDOC & ~OUTF_TARGETONLYDOC & ~OUTF_TARGETONLYSEL & ~OUTF_TARGETNEWTAB;
        if (nSelection == 1)
          dwOutputFlags|=OUTF_TARGETSELDOC;
        else if (nSelection == 2)
          dwOutputFlags|=OUTF_TARGETONLYDOC;
        else if (nSelection == 3)
          dwOutputFlags|=OUTF_TARGETONLYSEL;
        else if (nSelection == 4)
          dwOutputFlags|=OUTF_TARGETNEWTAB;

        oe.dwOutputFlags=dwOutputFlags;

        //Post execute command
        PostMessage(hWndDockDlg, AKDLL_EXECSTART, 0, 0);
        dwSaveFlags|=OF_RUN;
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
  else if (uMsg == WM_CLOSE)
  {
    PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
    return TRUE;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Destroy plugin icon
    DestroyIcon(hPluginIcon);
  }

  //Dialog resize messages
  {
    RESIZEDIALOGMSG rdsm={&rds[0], &rcRunMinMaxDialog, &rcRunCurrentDialog, RDM_PAINTSIZEGRIP, hDlg, uMsg, wParam, lParam};

    if (SendMessage(hMainWnd, AKD_RESIZEDIALOG, 0, (LPARAM)&rdsm))
      dwSaveFlags|=OF_RUNRECT;
  }

  return FALSE;
}

LRESULT CALLBACK NewOutputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_LBUTTONDBLCLK)
  {
    AETEXTRANGEW tr;
    AESELECTION aes;
    AECHARINDEX ciCaret;

    SendMessage(hWnd, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
    if (PatOpenLine(hWnd, &oe, &ciCaret, &tr))
    {
      aes.crSel.ciMin=tr.cr.ciMin;
      aes.crSel.ciMax=tr.cr.ciMax;
      aes.dwFlags=AESELT_LOCKSCROLL;
      aes.dwType=0;
      SendMessage(hWnd, AEM_SETSEL, (WPARAM)&aes.crSel.ciMin, (LPARAM)&aes);
      SetFocus(hMainWnd);
      return TRUE;
    }
  }

  return CallWindowProcWide(lpOldOutputProc, hWnd, uMsg, wParam, lParam);
}

void DockShowInput(BOOL bShow, HWND hWndInputEdit, HWND hWndInputButton, HWND hWndRunStopButton)
{
  RECT rcInputEdit;
  RECT rcOutputView;

  if (bShow)
  {
    ShowWindow(hWndInputEdit, SW_SHOW);
    ShowWindow(hWndInputButton, SW_SHOW);
    ShowWindow(hWndRunStopButton, SW_SHOW);
    GetWindowSize(hWndInputEdit, hWndDockDlg, &rcInputEdit);
    GetWindowSize(hWndOutputView, hWndDockDlg, &rcOutputView);
    SetWindowPos(hWndOutputView, NULL, 0, 0, rcOutputView.right, (rcInputEdit.top - 5) - rcOutputView.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
  }
  else
  {
    ShowWindow(hWndInputEdit, SW_HIDE);
    ShowWindow(hWndInputButton, SW_HIDE);
    ShowWindow(hWndRunStopButton, SW_HIDE);
    GetWindowSize(hWndInputEdit, hWndDockDlg, &rcInputEdit);
    GetWindowSize(hWndOutputView, hWndDockDlg, &rcOutputView);
    SetWindowPos(hWndOutputView, NULL, 0, 0, rcOutputView.right, (rcInputEdit.top + rcInputEdit.bottom) - rcOutputView.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
  }
}


//// Settings sheet

void SettingsSheet(int nStartPage)
{
  HICON hIconWatch=NULL;
  HICON hIconOutput=NULL;
  POINT ptSmallIcon;

  //Create image list
  ptSmallIcon.x=16 /*GetSystemMetrics(SM_CXSMICON)*/;
  ptSmallIcon.y=16 /*GetSystemMetrics(SM_CYSMICON)*/;

  if (hImageList=ImageList_Create(ptSmallIcon.x, ptSmallIcon.y, ILC_COLOR32|ILC_MASK, 0, 0))
  {
    hIconWatch=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_WATCH), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    hIconOutput=(HICON)LoadImageA(hInstanceDLL, MAKEINTRESOURCEA(IDI_OUTPUT), IMAGE_ICON, ptSmallIcon.x, ptSmallIcon.y, 0);
    ImageList_AddIcon(hImageList, hIconWatch);
    ImageList_AddIcon(hImageList, hIconOutput);
  }

  //Show property sheet
  psp[PAGE_WATCH].dwSize       =sizeof(PROPSHEETPAGEW);
  psp[PAGE_WATCH].dwFlags      =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_WATCH].hInstance    =hInstanceDLL;
  psp[PAGE_WATCH].pszTemplate  =MAKEINTRESOURCEW(IDD_WATCH_SETUP);
  psp[PAGE_WATCH].hIcon        =hIconWatch;
  psp[PAGE_WATCH].pfnDlgProc   =(DLGPROC)WatchSetupDlgProc;

  psp[PAGE_OUTPUT].dwSize      =sizeof(PROPSHEETPAGEW);
  psp[PAGE_OUTPUT].dwFlags     =PSP_DEFAULT|PSP_USEHICON;
  psp[PAGE_OUTPUT].hInstance   =hInstanceDLL;
  psp[PAGE_OUTPUT].pszTemplate =MAKEINTRESOURCEW(IDD_OUTPUT_SETUP);
  psp[PAGE_OUTPUT].hIcon       =hIconOutput;
  psp[PAGE_OUTPUT].pfnDlgProc  =(DLGPROC)OutputSetupDlgProc;

  psh.dwSize      =sizeof(PROPSHEETHEADERW);
  psh.dwFlags     =PSH_PROPSHEETPAGE|PSH_USEICONID|PSH_USECALLBACK|PSH_NOAPPLYNOW;
  psh.hwndParent  =hMainWnd;
  psh.hInstance   =hInstanceEXE;
  psh.pszIcon     =MAKEINTRESOURCEW(IDI_ICON_MAIN);
  psh.pszCaption  =wszPluginTitle;
  psh.nPages      =PAGE_MAX;
  psh.nStartPage  =nStartPage;
  psh.ppsp        =&psp[0];
  psh.pfnCallback =PropSheetProc;

  hHookOptions=SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());
  PropertySheetWide(&psh);

  if (dwSaveFlags)
  {
    if (dwSaveFlags & OF_WATCH)
    {
      if (bInitWatch)
      {
        UninitWatch();
        InitWatch();
      }
    }
    if (dwSaveFlags & OF_OUTPUT)
    {
      if (nInitOutput)
      {
        UninitOutput();
        InitOutput();
      }
    }
    SaveOptions(dwSaveFlags);
    dwSaveFlags=0;
  }

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

BOOL CALLBACK WatchSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndWatchUpdateInterval;
  static HWND hWndScrollEndAutoCheck;
  static HWND hWndScrollEndCaretCheck;
  static HWND hWndScrollEndVertCheck;
  static HWND hWndScrollEndNoCheck;
  static HWND hWndWatchCompleteReopenCheck;

  if (uMsg == WM_INITDIALOG)
  {
    hWndWatchUpdateInterval=GetDlgItem(hDlg, IDC_WATCH_SETUP_UPDATEINTERVAL);
    hWndScrollEndAutoCheck=GetDlgItem(hDlg, IDC_WATCH_SETUP_SCROLLENDAUTO_CHECK);
    hWndScrollEndCaretCheck=GetDlgItem(hDlg, IDC_WATCH_SETUP_SCROLLENDCARET_CHECK);
    hWndScrollEndVertCheck=GetDlgItem(hDlg, IDC_WATCH_SETUP_SCROLLENDVERT_CHECK);
    hWndScrollEndNoCheck=GetDlgItem(hDlg, IDC_WATCH_SETUP_SCROLLENDNO_CHECK);
    hWndWatchCompleteReopenCheck=GetDlgItem(hDlg, IDC_WATCH_SETUP_COMPLETEREOPEN_CHECK);

    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_UPDATEINTERVAL_GROUP, GetLangStringW(wLangModule, STRID_UPDATEINTERVAL));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_MS_LABEL, GetLangStringW(wLangModule, STRID_MS));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_SCROLL_GROUP, GetLangStringW(wLangModule, STRID_SCROLL));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_SCROLLENDAUTO_CHECK, GetLangStringW(wLangModule, STRID_SCROLLENDAUTO));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_SCROLLENDCARET_CHECK, GetLangStringW(wLangModule, STRID_SCROLLENDCARET));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_SCROLLENDVERT_CHECK, GetLangStringW(wLangModule, STRID_SCROLLENDVERT));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_SCROLLENDNO_CHECK, GetLangStringW(wLangModule, STRID_SCROLLENDNO));
    SetDlgItemTextWide(hDlg, IDC_WATCH_SETUP_COMPLETEREOPEN_CHECK, GetLangStringW(wLangModule, STRID_COMPLETEREOPEN));

    SendMessage(hWndWatchUpdateInterval, EM_LIMITTEXT, 6, 0);
    SetDlgItemInt(hDlg, IDC_WATCH_SETUP_UPDATEINTERVAL, dwWatchUpdateInterval, FALSE);

    if (dwWatchScrollEnd == WSE_AUTO)
      SendMessage(hWndScrollEndAutoCheck, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwWatchScrollEnd == WSE_CARET)
      SendMessage(hWndScrollEndCaretCheck, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwWatchScrollEnd == WSE_VERT)
      SendMessage(hWndScrollEndVertCheck, BM_SETCHECK, BST_CHECKED, 0);
    else if (dwWatchScrollEnd == WSE_NO)
      SendMessage(hWndScrollEndNoCheck, BM_SETCHECK, BST_CHECKED, 0);

    if (bWatchCompleteReopen) SendMessage(hWndWatchCompleteReopenCheck, BM_SETCHECK, BST_CHECKED, 0);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      dwWatchUpdateInterval=GetDlgItemInt(hDlg, IDC_WATCH_SETUP_UPDATEINTERVAL, NULL, FALSE);

      if (SendMessage(hWndScrollEndAutoCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwWatchScrollEnd=WSE_AUTO;
      else if (SendMessage(hWndScrollEndCaretCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwWatchScrollEnd=WSE_CARET;
      else if (SendMessage(hWndScrollEndVertCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwWatchScrollEnd=WSE_VERT;
      else if (SendMessage(hWndScrollEndNoCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwWatchScrollEnd=WSE_NO;

      bWatchCompleteReopen=(BOOL)SendMessage(hWndWatchCompleteReopenCheck, BM_GETCHECK, 0, 0);

      dwSaveFlags|=OF_WATCH;
    }
  }
  return FALSE;
}

BOOL CALLBACK OutputSetupDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndCodePageAutoRadio;
  static HWND hWndCodePageForceRadio;
  static HWND hWndCodePageList;
  static HWND hWndPatternEdit;
  static HWND hWndPatternButton;
  static HWND hWndPatTagsEdit;
  static HWND hWndPatTagsButton;
  static HWND hWndSaveAllCheck;
  static HWND hWndAppendNextCheck;
  static HWND hWndNoScrollCheck;
  static HWND hWndWrapCheck;
  static HWND hWndHideInputCheck;
  static HWND hWndNextMatchKey;
  static HWND hWndPrevMatchKey;
  static HWND hWndRunCmdKey;
  static HWND hWndRunCmdDlgKey;
  static HWND hWndCoderAliasEdit;
  static HMENU hMenuPattern;
  static HMENU hMenuPatTags;

  if (uMsg == WM_INITDIALOG)
  {
    int *lpCodepageList;

    hWndCodePageAutoRadio=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_AUTOCODEPAGE_RADIO);
    hWndCodePageForceRadio=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_FORCECODEPAGE_RADIO);
    hWndCodePageList=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_CODEPAGE_LIST);
    hWndPatternEdit=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_PATTERN_EDIT);
    hWndPatternButton=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_PATTERN_BUTTON);
    hWndPatTagsEdit=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_PATTAGS_EDIT);
    hWndPatTagsButton=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_PATTAGS_BUTTON);
    hWndSaveAllCheck=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_SAVEALL_CHECK);
    hWndAppendNextCheck=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_APPENDNEXT_CHECK);
    hWndNoScrollCheck=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_NOSCROLL_CHECK);
    hWndWrapCheck=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_WRAP_CHECK);
    hWndHideInputCheck=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_HIDEINPUT_CHECK);
    hWndNextMatchKey=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_NEXTMATCH_KEY);
    hWndPrevMatchKey=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_PREVMATCH_KEY);
    hWndRunCmdKey=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_RUNCMD_KEY);
    hWndRunCmdDlgKey=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_RUNCMDDLG_KEY);
    hWndCoderAliasEdit=GetDlgItem(hDlg, IDC_OUTPUT_SETUP_CODERALIAS_EDIT);

    if (lpCodepageList=(int *)SendMessage(hMainWnd, AKD_GETCODEPAGELIST, (WPARAM)NULL, 0))
    {
      if (nForceCodePage == CP_ACP)
        nForceCodePage=(int)GetACP();
      else if (nForceCodePage == CP_OEMCP)
        nForceCodePage=(int)GetOEMCP();
      FillComboboxCodepage(hWndCodePageList, lpCodepageList);
      SelectComboboxCodepage(hWndCodePageList, nForceCodePage);
    }
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_CODEPAGE_GROUP, GetLangStringW(wLangModule, STRID_CODEPAGE));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_AUTOCODEPAGE_RADIO, GetLangStringW(wLangModule, STRID_AUTODETECT));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_PATTERN_LABEL, GetLangStringW(wLangModule, STRID_PATTERN));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_PATTAGS_LABEL, GetLangStringW(wLangModule, STRID_PATTAGS));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_SAVEALL_CHECK, GetLangStringW(wLangModule, STRID_SAVEALL));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_APPENDNEXT_CHECK, GetLangStringW(wLangModule, STRID_APPENDNEXT));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_NOSCROLL_CHECK, GetLangStringW(wLangModule, STRID_NOSCROLL));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_WRAP_CHECK, GetLangStringW(wLangModule, STRID_WRAP));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_HIDEINPUT_CHECK, GetLangStringW(wLangModule, STRID_HIDEINPUT));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_HOTKEYS_GROUP, GetLangStringW(wLangModule, STRID_HOTKEYS));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_NEXTMATCH_LABEL, GetLangStringW(wLangModule, STRID_NEXTMATCH));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_PREVMATCH_LABEL, GetLangStringW(wLangModule, STRID_PREVMATCH));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_RUNCMD_LABEL, GetLangStringW(wLangModule, STRID_RUNCMD));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_RUNCMDDLG_LABEL, GetLangStringW(wLangModule, STRID_RUNCMDDLG));
    SetDlgItemTextWide(hDlg, IDC_OUTPUT_SETUP_CODERALIAS_LABEL, GetLangStringW(wLangModule, STRID_CODERALIAS));

    //Set icons
    {
      BUTTONDRAW bd={BIF_DOWNARROW};

      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndPatternButton, (LPARAM)&bd);
      SendMessage(hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)hWndPatTagsButton, (LPARAM)&bd);
    }

    if (hMenuPattern=CreatePopupMenu())
    {
      AppendMenuWide(hMenuPattern, MF_STRING, IDM_OUTPUT_PATMICROSOFTGCC, GetLangStringW(wLangModule, STRID_MENUMICROSOFTGCC));
      AppendMenuWide(hMenuPattern, MF_STRING, IDM_OUTPUT_PATBORLAND, GetLangStringW(wLangModule, STRID_MENUPATBORLAND));
    }
    if (hMenuPatTags=CreatePopupMenu())
    {
      AppendMenuWide(hMenuPatTags, MF_STRING, IDM_OUTPUT_TAGFILE, GetLangStringW(wLangModule, STRID_MENUTAGFILE));
      AppendMenuWide(hMenuPatTags, MF_STRING, IDM_OUTPUT_TAGGOTOLINE, GetLangStringW(wLangModule, STRID_MENUTAGGOTOLINE));
      AppendMenuWide(hMenuPatTags, MF_STRING, IDM_OUTPUT_TAGGOTOBYTE, GetLangStringW(wLangModule, STRID_MENUTAGGOTOBYTE));
      AppendMenuWide(hMenuPatTags, MF_STRING, IDM_OUTPUT_TAGGOTOCHAR, GetLangStringW(wLangModule, STRID_MENUTAGGOTOCHAR));
    }

    if (bAutoCodePage)
    {
      SendMessage(hWndCodePageAutoRadio, BM_SETCHECK, BST_CHECKED, 0);
      EnableWindow(hWndCodePageList, FALSE);
    }
    else SendMessage(hWndCodePageForceRadio, BM_SETCHECK, BST_CHECKED, 0);

    SetWindowTextWide(hWndPatternEdit, wszPattern);
    SetWindowTextWide(hWndPatTagsEdit, wszPatTags);
    SetWindowTextWide(hWndCoderAliasEdit, wszCoderAlias);

    if (dwOutputFlags & OUTF_SAVEALL)
      SendMessage(hWndSaveAllCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwOutputFlags & OUTF_APPENDNEXT)
      SendMessage(hWndAppendNextCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwOutputFlags & OUTF_NOSCROLL)
      SendMessage(hWndNoScrollCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwOutputFlags & OUTF_WRAP)
      SendMessage(hWndWrapCheck, BM_SETCHECK, BST_CHECKED, 0);
    if (dwOutputFlags & OUTF_HIDEINPUT)
      SendMessage(hWndHideInputCheck, BM_SETCHECK, BST_CHECKED, 0);

    InitMain();
    SendMessage(hWndNextMatchKey, HKM_SETHOTKEY, dwNextMatchKey, 0);
    SendMessage(hWndPrevMatchKey, HKM_SETHOTKEY, dwPrevMatchKey, 0);
    SendMessage(hWndRunCmdKey, HKM_SETHOTKEY, dwRunCmdKey, 0);
    SendMessage(hWndRunCmdDlgKey, HKM_SETHOTKEY, dwRunCmdDlgKey, 0);

    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndNextMatchKey, pfwNextMatch?dwNextMatchKey:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndPrevMatchKey, pfwPrevMatch?dwPrevMatchKey:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndRunCmdKey, pfwRunCmd?dwRunCmdKey:0);
    SendMessage(hMainWnd, AKD_SETHOTKEYINPUT, (WPARAM)hWndRunCmdDlgKey, pfwRunCmdDlg?dwRunCmdDlgKey:0);
    UninitMain();
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDC_OUTPUT_SETUP_AUTOCODEPAGE_RADIO ||
        LOWORD(wParam) == IDC_OUTPUT_SETUP_FORCECODEPAGE_RADIO)
    {
      if (SendMessage(hWndCodePageAutoRadio, BM_GETCHECK, 0, 0) == BST_CHECKED)
        EnableWindow(hWndCodePageList, FALSE);
      else
        EnableWindow(hWndCodePageList, TRUE);
    }
    else if (LOWORD(wParam) == IDC_OUTPUT_SETUP_PATTERN_BUTTON)
    {
      RECT rcButton;
      int nCmd;

      GetWindowRect(hWndPatternButton, &rcButton);
      if (nCmd=TrackPopupMenu(hMenuPattern, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rcButton.left, rcButton.top + (rcButton.bottom - rcButton.top), 0, hDlg, NULL))
      {
        if (nCmd == IDM_OUTPUT_PATMICROSOFTGCC)
        {
          SetWindowTextWide(hWndPatternEdit, PAT_MICROSOFTGCC);
          SetWindowTextWide(hWndPatTagsEdit, PAT_MICROSOFTGCCTAG);
        }
        else if (nCmd == IDM_OUTPUT_PATBORLAND)
        {
          SetWindowTextWide(hWndPatternEdit, PAT_BORLAND);
          SetWindowTextWide(hWndPatTagsEdit, PAT_BORLANDTAG);
        }
        SetFocus(hWndPatternEdit);
      }
    }
    else if (LOWORD(wParam) == IDC_OUTPUT_SETUP_PATTAGS_BUTTON)
    {
      RECT rcButton;
      int nCmd;

      GetWindowRect(hWndPatTagsButton, &rcButton);
      if (nCmd=TrackPopupMenu(hMenuPatTags, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, rcButton.left, rcButton.top + (rcButton.bottom - rcButton.top), 0, hDlg, NULL))
      {
        if (nCmd == IDM_OUTPUT_TAGFILE)
          SendMessageA(hWndPatTagsEdit, EM_REPLACESEL, TRUE, (LPARAM)"/FILE=\\1");
        else if (nCmd == IDM_OUTPUT_TAGGOTOLINE)
          SendMessageA(hWndPatTagsEdit, EM_REPLACESEL, TRUE, (LPARAM)"/GOTOLINE=\\2:\\3");
        else if (nCmd == IDM_OUTPUT_TAGGOTOBYTE)
          SendMessageA(hWndPatTagsEdit, EM_REPLACESEL, TRUE, (LPARAM)"/GOTOBYTE=\\2");
        else if (nCmd == IDM_OUTPUT_TAGGOTOCHAR)
          SendMessageA(hWndPatTagsEdit, EM_REPLACESEL, TRUE, (LPARAM)"/GOTOCHAR=\\2");
        SetFocus(hWndPatTagsEdit);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (((NMHDR *)lParam)->code == (UINT)PSN_APPLY)
    {
      DWORD dwOutputFlagsOld;
      bAutoCodePage=(BOOL)SendMessage(hWndCodePageAutoRadio, BM_GETCHECK, 0, 0);
      nForceCodePage=GetDlgItemInt(hDlg, IDC_OUTPUT_SETUP_CODEPAGE_LIST, NULL, FALSE);

      GetWindowTextWide(hWndPatternEdit, wszPattern, BUFFER_SIZE);
      GetWindowTextWide(hWndPatTagsEdit, wszPatTags, BUFFER_SIZE);
      GetWindowTextWide(hWndCoderAliasEdit, wszCoderAlias, MAX_PATH);

      dwOutputFlagsOld=dwOutputFlags;
      dwOutputFlags=0;
      if (SendMessage(hWndSaveAllCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwOutputFlags|=OUTF_SAVEALL;
      if (SendMessage(hWndAppendNextCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwOutputFlags|=OUTF_APPENDNEXT;
      if (SendMessage(hWndNoScrollCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwOutputFlags|=OUTF_NOSCROLL;
      if (SendMessage(hWndWrapCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwOutputFlags|=OUTF_WRAP;
      if (SendMessage(hWndHideInputCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
        dwOutputFlags|=OUTF_HIDEINPUT;
      if (hWndOutputView)
      {
        if ((dwOutputFlagsOld & OUTF_WRAP) != (dwOutputFlags & OUTF_WRAP))
          SetWordWrap(dwOutputFlags);
        if ((dwOutputFlagsOld & OUTF_HIDEINPUT) != (dwOutputFlags & OUTF_HIDEINPUT))
          PostMessage(hWndDockDlg, AKDLL_SHOWINPUT, !(dwOutputFlags & OUTF_HIDEINPUT), 0);
      }

      dwNextMatchKey=(WORD)SendMessage(hWndNextMatchKey, HKM_GETHOTKEY, 0, 0);
      dwPrevMatchKey=(WORD)SendMessage(hWndPrevMatchKey, HKM_GETHOTKEY, 0, 0);
      dwRunCmdKey=(WORD)SendMessage(hWndRunCmdKey, HKM_GETHOTKEY, 0, 0);
      dwRunCmdDlgKey=(WORD)SendMessage(hWndRunCmdDlgKey, HKM_GETHOTKEY, 0, 0);

      //Update current OUTPUTEXEC
      FillOutputDefault(&oe);

      dwSaveFlags|=OF_OUTPUT;
    }
  }
  else if (uMsg == WM_DESTROY)
  {
    DestroyMenu(hMenuPattern);
    DestroyMenu(hMenuPatTags);
  }

  return FALSE;
}

LRESULT CALLBACK NewMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  //Call next procedure. Call it here because of StopWatch.
  lResult=NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

  if (uMsg == AKDN_OPENDOCUMENT_START ||
      uMsg == AKDN_SAVEDOCUMENT_START)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (!bLogOpening)
    {
      if (hWndEditWatch == lpFrame->ei.hWndEdit && hDocEditWatch == lpFrame->ei.hDocEdit)
      {
        bDocOpening=TRUE;
      }
    }
  }
  else if (uMsg == AKDN_OPENDOCUMENT_FINISH ||
           uMsg == AKDN_SAVEDOCUMENT_FINISH)
  {
    FRAMEDATA *lpFrame=(FRAMEDATA *)wParam;

    if (!bLogOpening)
    {
      if (hWndEditWatch == lpFrame->ei.hWndEdit && hDocEditWatch == lpFrame->ei.hDocEdit)
      {
        bDocOpening=FALSE;
        dwCurPointer=(UINT_PTR)-1;
        xmemset(&ftCurTime, 0, sizeof(FILETIME));
      }
    }
  }
  else if (uMsg == AKDN_EDIT_ONFINISH ||
           uMsg == AKDN_EDIT_ONCLOSE)
  {
    if (hWndEditWatch == (HWND)wParam && hDocEditWatch == (AEHDOC)lParam)
    {
      hWndEditWatch=NULL;
      hDocEditWatch=NULL;
      StopWatch(0);
    }
  }
  else if (uMsg == AKDN_DOCK_RESIZE)
  {
    if (((DOCK *)wParam)->hWnd == dkOutputDlg->hWnd)
      dwSaveFlags|=OF_DOCKRECT;
  }
  else if (uMsg == AKDN_SIZE_ONSTART)
  {
    if (bOutputDockWaitResize)
    {
      bOutputDockWaitResize=FALSE;
      ShowWindow(hWndDockDlg, SW_SHOW);
      dkOutputDlg->dwFlags&=~DKF_HIDDEN;
    }
  }
  else if (uMsg == AKDN_DLLCODER)
  {
    NCODERUPDATE *ncu=(NCODERUPDATE *)lParam;

    if (ncu->dwFlags & SAE_RESETLIST)
      SetCoderAlias();
  }
  else if (uMsg == AKDN_MAIN_ONFINISH)
  {
    //NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);

    if (hWndDockDlg)
    {
      DestroyDock(hWndDockDlg, DKT_ONMAINFINISH);
    }
    if (nInitOutput)
    {
      UninitMain();
      UninitOutput();
    }
    if (bInitWatch)
    {
      UninitMain();
      UninitWatch();
    }
    return FALSE;
  }

  //Special messages
  EditParentMessages(hWnd, uMsg, wParam, lParam);
  return lResult;
}

LRESULT CALLBACK NewFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  //Call next procedure. Call it here because of StopWatch in EditParentMessages.
  lResult=NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);

  //Special messages
  EditParentMessages(hWnd, uMsg, wParam, lParam);
  return lResult;
}

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == ID_EDIT)
    {
      if (HIWORD(wParam) == EN_CHANGE)
      {
        if (hWndEditWatch == (HWND)lParam && (nMDI != WMD_PMDI || hDocEditWatch == (AEHDOC)SendMessage(hWndEditWatch, AEM_GETDOCUMENT, 0, 0)))
        {
          if (!bDocOpening && !bLogOpening)
          {
            StopWatch(STRID_MSG_MODIFIED_ONSTOP);
          }
        }
      }
    }
  }
  return 0;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  EDITINFO ei;
  FILECONTENT fc;

  if (!hWndEditWatch || bDocOpening || bLogOpening) return;

  if (SendMessage(hMainWnd, AKD_GETEDITINFO, (WPARAM)hWndEditWatch, (LPARAM)&ei))
  {
    if (nMDI == WMD_PMDI && hDocEditWatch != ei.hDocEdit)
      return;

    if (!ei.bModified)
    {
      fc.hFile=CreateFileWide(ei.wszFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

      if (fc.hFile != INVALID_HANDLE_VALUE)
      {
        CHARRANGE64 cr;
        FILETIME ftNewTime={0};
        UINT_PTR dwBlockSize;
        UINT_PTR dwNewPointer=0;
        UINT_PTR dwBlockLen;
        POINT64 ptGlobalMax;
        POINT64 ptGlobalPos;

        bLogOpening=TRUE;
        dwNewPointer=GetFileSize64(fc.hFile);
        GetFileTime(fc.hFile, NULL, NULL, &ftNewTime);

        if ((dwNewPointer != (UINT_PTR)-1 && dwNewPointer != dwCurPointer) || xmemcmp(&ftNewTime, &ftCurTime, sizeof(FILETIME)))
        {
          if (dwWatchScrollEnd == WSE_AUTO)
          {
            RECT rcDraw;

            SendMessage(hWndEditWatch, AEM_GETRECT, 0, (LPARAM)&rcDraw);
            SendMessage(hWndEditWatch, AEM_GETSCROLLPOS, (WPARAM)&ptGlobalMax, (LPARAM)&ptGlobalPos);
            ptGlobalPos.y+=rcDraw.bottom - rcDraw.top;
          }

          if (bWatchCompleteReopen)
          {
            dwCurPointer=dwNewPointer;
            xmemcpy(&ftCurTime, &ftNewTime, sizeof(FILETIME));

            if (SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_REOPEN, 0) == EOD_SUCCESS)
            {
              if (dwWatchScrollEnd == WSE_NO)
              {
              }
              else if (dwWatchScrollEnd == WSE_CARET)
              {
                SendMessage(hWndEditWatch, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
              }
              else if (dwWatchScrollEnd == WSE_VERT || (dwWatchScrollEnd == WSE_AUTO && ptGlobalPos.y >= ptGlobalMax.y))
              {
                SendMessage(hWndEditWatch, AEM_GETSCROLLPOS, (WPARAM)&ptGlobalMax, (LPARAM)NULL);
                ptGlobalMax.x=-1;
                SendMessage(hWndEditWatch, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobalMax);
              }
            }
            else
            {
              StopWatch(STRID_MSG_CANTOPENFILE_ONSTOP);
            }
          }
          else
          {
            if (dwNewPointer < dwCurPointer)
              dwCurPointer=0;
            SetFilePointer64(fc.hFile, dwCurPointer, FILE_BEGIN);
            dwBlockSize=dwNewPointer - dwCurPointer;

            fc.dwMax=dwBlockSize;
            fc.nCodePage=ei.nCodePage;
            fc.bBOM=ei.bBOM;
            fc.wpContent=NULL;
            if (dwBlockLen=SendMessage(hMainWnd, AKD_READFILECONTENT, 0, (LPARAM)&fc))
            {
              if (!dwCurPointer)
              {
                SendMessage(hWndEditWatch, EM_EXGETSEL64, 0, (LPARAM)&cr);
                SendMessage(hWndEditWatch, WM_SETTEXT, 0, (LPARAM)L"");
              }

              //Add text
              SendMessage(hWndEditWatch, AEM_LOCKCOLLECTUNDO, TRUE, 0);
              AppendTextWide(hWndEditWatch, fc.wpContent, dwBlockLen);
              SendMessage(hWndEditWatch, AEM_LOCKCOLLECTUNDO, FALSE, 0);
              SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)fc.wpContent);

              if (!dwCurPointer)
              {
                SendMessage(hWndEditWatch, EM_EXSETSEL64, 0, (LPARAM)&cr);
              }

              if (dwWatchScrollEnd == WSE_NO)
              {
              }
              else if (dwWatchScrollEnd == WSE_CARET)
              {
                SendMessage(hWndEditWatch, EM_SETSEL, (WPARAM)-1, (LPARAM)-1);
              }
              else if (dwWatchScrollEnd == WSE_VERT || (dwWatchScrollEnd == WSE_AUTO && ptGlobalPos.y >= ptGlobalMax.y))
              {
                SendMessage(hWndEditWatch, AEM_GETSCROLLPOS, (WPARAM)&ptGlobalMax, (LPARAM)NULL);
                ptGlobalMax.x=-1;
                SendMessage(hWndEditWatch, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobalMax);
              }
            }
            dwCurPointer=dwNewPointer;
            xmemcpy(&ftCurTime, &ftNewTime, sizeof(FILETIME));
          }
        }
        CloseHandle(fc.hFile);

        bLogOpening=FALSE;
      }
      else
      {
        StopWatch(STRID_MSG_CANTOPENFILE_ONSTOP);
      }
    }
  }
}

BOOL CALLBACK NextMatchProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AETEXTRANGEW tr;
  AESELECTION aes;
  AECHARINDEX ciCaret;

  //Get caret line
  SendMessage(hWndOutputView, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
  AEC_WrapLineEndEx(&ciCaret, &ciCaret);

  while (AEC_NextLine(&ciCaret))
  {
    if (PatOpenLine(hWndOutputView, &oe, &ciCaret, &tr))
    {
      aes.crSel.ciMin=tr.cr.ciMin;
      aes.crSel.ciMax=tr.cr.ciMax;
      aes.dwFlags=AESELT_LOCKSCROLL;
      aes.dwType=0;
      SendMessage(hWndOutputView, AEM_SETSEL, (WPARAM)&aes.crSel.ciMin, (LPARAM)&aes);
      ScrollCaret(hWndOutputView);
      SetFocus(hMainWnd);
      break;
    }
    ciCaret=tr.cr.ciMax;
  }
  return TRUE;
}

BOOL CALLBACK PrevMatchProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  AETEXTRANGEW tr;
  AESELECTION aes;
  AECHARINDEX ciCaret;

  //Get caret line
  SendMessage(hWndOutputView, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ciCaret);
  AEC_WrapLineBeginEx(&ciCaret, &ciCaret);

  while (AEC_PrevLine(&ciCaret))
  {
    if (PatOpenLine(hWndOutputView, &oe, &ciCaret, &tr))
    {
      aes.crSel.ciMin=tr.cr.ciMin;
      aes.crSel.ciMax=tr.cr.ciMax;
      aes.dwFlags=AESELT_LOCKSCROLL;
      aes.dwType=0;
      SendMessage(hWndOutputView, AEM_SETSEL, (WPARAM)&aes.crSel.ciMin, (LPARAM)&aes);
      ScrollCaret(hWndOutputView);
      SetFocus(hMainWnd);
      break;
    }
    ciCaret=tr.cr.ciMin;
  }
  return TRUE;
}

BOOL CALLBACK RunCmdProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  if (!hExecThread)
  {
    xmemset(&oe, 0, sizeof(OUTPUTEXEC));
    FillOutputDefault(&oe);
    oe.dwTimeOut=(DWORD)-1;
    xstrcpynW(oe.wszCmdLine, wszRunCmdLine, COMMANDLINE_SIZE);
    xstrcpynW(oe.wszDir, wszRunDir, MAX_PATH);
    PostMessage(hWndDockDlg, AKDLL_EXECSTART, 0, 0);
  }
  return TRUE;
}

BOOL CALLBACK RunCmdDlgProc(void *lpParameter, LPARAM lParam, DWORD dwSupport)
{
  if (!hExecThread)
  {
    DialogBoxWide(hInstanceDLL, MAKEINTRESOURCEW(IDD_OUTPUT_RUN), hWndDockDlg, (DLGPROC)DockRunDlgProc);
  }
  return TRUE;
}

void OutputExec(OUTPUTEXEC *oe)
{
  HANDLE hInitMutex;
  DWORD dwExecThreadId;

  if (oe->dwExecState != OES_EXECUTED)
    oe->dwExecState=OES_EXECUTED;

  if (!hExecThread)
  {
    if (hInitMutex=CreateEventA(NULL, FALSE, FALSE, "AkelPad::Log::Output"))
    {
      oe->hInitMutex=hInitMutex;
      if (hExecThread=CreateThread(NULL, 0, ExecThreadProc, oe, 0, &dwExecThreadId))
        WaitForSingleObject(hInitMutex, INFINITE);
      CloseHandle(hInitMutex);
      oe->hInitMutex=NULL;
    }
  }
}

DWORD WINAPI ExecThreadProc(LPVOID lpParameter)
{
  OUTPUTEXEC *oe=(OUTPUTEXEC *)lpParameter;

  //Thread is initialized now unlock main thread
  oe->dwExecState|=OES_THREAD;
  SetEvent(oe->hInitMutex);

  //Execution
  ExecuteToWindow(oe);
  oe->dwExecState=OES_IDLE;

  if (oe->dwOutputFlags & OUTF_NOPANEL)
  {
    if (nInitMain)
    {
      pfOutput->bRunning=FALSE;
      SendMessage(hMainWnd, WM_COMMAND, 0, 0);
    }
    else SendMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)hExecThread);
  }
  else
  {
    //Send notification
    if (hWndDockDlg)
    {
      if ((oe->dwOutputFlags & OUTF_CLOSENOERROR) && oe->dwExitCode == 0)
      {
        //DestroyDock
        PostMessage(hWndDockDlg, WM_COMMAND, IDCANCEL, DKT_KEEPAUTOLOAD|DKT_WAITTHREAD);
      }
      else PostMessage(hWndDockDlg, AKDLL_EXECSTOP, 0, 0);
    }
  }

  if (hExecThread)
  {
    CloseHandle(hExecThread);
    hExecThread=NULL;
  }
  return 0;
}

BOOL ExecuteToWindow(OUTPUTEXEC *oe)
{
  HANDLE hCurProcess=GetCurrentProcess();
  HANDLE hOutputReadTmp=NULL;
  HANDLE hOutputRead=NULL;
  HANDLE hOutputWrite=NULL;
  HANDLE hInputWriteTmp=NULL;
  HANDLE hInputWrite=NULL;
  HANDLE hInputRead=NULL;
  HANDLE hErrorWrite=NULL;
  PROCESS_INFORMATION pi;
  STARTUPINFOW si;
  SECURITY_ATTRIBUTES sa;
  wchar_t wszDir[MAX_PATH];
  unsigned char *szData;
  unsigned char *szAllData=NULL;
  unsigned char *szTmp;
  DWORD dwTimeCount=0;
  DWORD dwAllData=0;
  CHARRANGE64 cr;
  INT_PTR nOutputStartPos=0;
  int nInputCodePage;
  int nOutputCodePage;
  int nFirstLine;
  DWORD dwCodepageRecognitionBuffer=0;
  DWORD dwRead=0;
  DWORD dwExitCode;
  DWORD dwResult;
  BOOL bTargetPanel;
  BOOL bInsert;
  BOOL bResult=FALSE;

  //Input
  EXGETTEXTRANGE tr;
  UINT_PTR nNumberOfBytesToWrite=0;
  HWND hWndEdit=NULL;

  //Codepage settings
  nInputCodePage=oe->nInputCodePage;
  if (nInputCodePage == -1)
    nInputCodePage=(int)GetOEMCP();
  nOutputCodePage=oe->nOutputCodePage;
  if (nOutputCodePage == -1)
  {
    nOutputCodePage=(int)GetOEMCP();
    dwCodepageRecognitionBuffer=(DWORD)SendMessage(hMainWnd, AKD_GETMAININFO, MI_CODEPAGERECOGNITIONBUFFER, 0);
  }

  if (oe->dwOutputFlags & OUTF_SAVEALL)
  {
    if (!SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_SAVEALL, 0))
      return FALSE;
  }

  //StdInput
  if ((oe->dwOutputFlags & OUTF_SOURCESELDOC) || (oe->dwOutputFlags & OUTF_SOURCEONLYDOC) || (oe->dwOutputFlags & OUTF_SOURCEONLYSEL))
  {
    if (hWndEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL))
    {
      if ((oe->dwOutputFlags & OUTF_SOURCEONLYDOC) || !SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)&tr.cr))
      {
        if (!(oe->dwOutputFlags & OUTF_SOURCEONLYSEL))
        {
          SendMessage(hWndEdit, AEM_GETINDEX, AEGI_FIRSTCHAR, (LPARAM)&tr.cr.ciMin);
          SendMessage(hWndEdit, AEM_GETINDEX, AEGI_LASTCHAR, (LPARAM)&tr.cr.ciMax);
          tr.bColumnSel=FALSE;
        }
      }
      tr.pText=NULL;
      tr.nNewLine=AELB_ASOUTPUT;
      tr.nCodePage=nInputCodePage;
      tr.lpDefaultChar=NULL;
      tr.lpUsedDefChar=NULL;
      nNumberOfBytesToWrite=SendMessage(hMainWnd, AKD_EXGETTEXTRANGEA, (WPARAM)hWndEdit, (LPARAM)&tr);
    }
  }

  if (!(oe->dwOutputFlags & OUTF_TARGETSELDOC) && !(oe->dwOutputFlags & OUTF_TARGETONLYDOC) && !(oe->dwOutputFlags & OUTF_TARGETONLYSEL) && !(oe->dwOutputFlags & OUTF_TARGETNEWTAB))
    bTargetPanel=TRUE;
  else
    bTargetPanel=FALSE;

  //Creates child process with redirected IO
  sa.nLength=sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor=NULL;
  sa.bInheritHandle=TRUE;

  //Creating pipes for child process with redirected IO
  if (CreatePipe(&hOutputReadTmp, &hOutputWrite, &sa, (DWORD)nNumberOfBytesToWrite) &&
      CreatePipe(&hInputRead, &hInputWriteTmp, &sa, (DWORD)nNumberOfBytesToWrite))
  {
    //Duplicates handles and makes them inheritable
    if (DuplicateHandle(hCurProcess, hOutputWrite, hCurProcess, &hErrorWrite, 0, TRUE, DUPLICATE_SAME_ACCESS) &&
        DuplicateHandle(hCurProcess, hOutputReadTmp, hCurProcess, &hOutputRead, 0, FALSE, DUPLICATE_SAME_ACCESS) &&
        DuplicateHandle(hCurProcess, hInputWriteTmp, hCurProcess, &hInputWrite, 0, FALSE, DUPLICATE_SAME_ACCESS))
    {
      if (hOutputReadTmp)
      {
        CloseHandle(hOutputReadTmp);
        hOutputReadTmp=NULL;
      }
      if (hInputWriteTmp)
      {
        CloseHandle(hInputWriteTmp);
        hInputWriteTmp=NULL;
      }

      ////Write by blocks
      //{
      //  UINT nBytesWritten=0;
      //
      //  while (nBytesWritten < nNumberOfBytesToWrite)
      //  {
      //    if (WriteFile(hInputWrite, tr.pText + nBytesWritten, min(nNumberOfBytesToWrite - nBytesWritten, 8192), &dwResult, NULL))
      //      nBytesWritten+=dwResult;
      //    else
      //      break;
      //  }
      //}

      //Expand variables
      TranslateFileString(oe->wszCmdLine, wszCmdLine, COMMANDLINE_SIZE);
      TranslateFileString(oe->wszDir, wszDir, MAX_PATH);

      //Run process and close unnecessary handles
      xmemset(&si, 0, sizeof(STARTUPINFOW));
      si.cb=sizeof(STARTUPINFOW);
      si.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
      si.hStdOutput=hOutputWrite;
      si.hStdInput=hInputRead;
      si.hStdError=hErrorWrite;
      si.wShowWindow=SW_HIDE;
      //si.wShowWindow=SW_NORMAL; //For tests

      if (CreateProcessWide(NULL, wszCmdLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, (*wszDir?wszDir:NULL), &si, &pi))
      {
        if (hOutputWrite)
        {
          CloseHandle(hOutputWrite);
          hOutputWrite=NULL;
        }
        if (hInputRead)
        {
          CloseHandle(hInputRead);
          hInputRead=NULL;
        }
        if (hErrorWrite)
        {
          CloseHandle(hErrorWrite);
          hErrorWrite=NULL;
        }
        CloseHandle(pi.hThread);

        oe->dwExecState|=OES_PROCESS;
        oe->hProcess=pi.hProcess;
        oe->dwProcessId=pi.dwProcessId;
        if (hWndOutputView)
        {
          if (!(oe->dwOutputFlags & OUTF_APPENDNEXT))
            SendMessage(hWndOutputView, AEM_SETTEXTW, (WPARAM)-1, (LPARAM)L"");
          else
            nOutputStartPos=GetTextLength(hWndOutputView);
        }

        if (nNumberOfBytesToWrite)
        {
          WriteFile(hInputWrite, tr.pText, (DWORD)nNumberOfBytesToWrite, &dwResult, NULL);
          nNumberOfBytesToWrite=0;

          //Some console applications require CloseHandle,
          //but after closing we cannot send input later.
          CloseHandle(hInputWrite);
          hInputWrite=NULL;
        }

        //Execution loop
        while (dwTimeCount <= oe->dwTimeOut &&
               PeekNamedPipe(hOutputRead, 0, 0, 0, &dwRead, NULL) &&
               GetExitCodeProcess(pi.hProcess, &dwExitCode))
        {
          if (dwRead > 0)
          {
            if (szData=GlobalAlloc(GPTR, dwRead))
            {
              if (ReadFile(hOutputRead, szData, dwRead, &dwResult, NULL) && dwResult > 0)
              {
                bInsert=TRUE;

                //Merge previous and current text block
                if (!bTargetPanel || dwAllData < dwCodepageRecognitionBuffer)
                {
                  dwAllData+=dwResult;

                  if (szTmp=GlobalAlloc(GPTR, dwAllData))
                  {
                    if (szAllData) xmemcpy(szTmp, szAllData, (dwAllData - dwResult));
                    xmemcpy(szTmp + (dwAllData - dwResult), szData, dwResult);

                    //Detect codepage
                    if (dwAllData < dwCodepageRecognitionBuffer)
                    {
                      DETECTANSITEXT dat;

                      dat.dwLangID=(DWORD)-1;
                      dat.pText=(char *)szTmp;
                      dat.nTextLen=dwAllData;
                      dat.nMinChars=30;
                      if (SendMessage(hMainWnd, AKD_DETECTANSITEXT, 0, (LPARAM)&dat))
                      {
                        if (dat.nCodePage != nOutputCodePage)
                        {
                          if (bTargetPanel)
                          {
                            //Text is detected - replace current output contents
                            SendMessage(hWndOutputView, WM_SETREDRAW, FALSE, 0);
                            SendMessage(hWndOutputView, EM_EXGETSEL64, 0, (LPARAM)&cr);
                            SendMessage(hWndOutputView, AEM_LOCKSCROLL, SB_BOTH, TRUE);
                            SendMessage(hWndOutputView, EM_SETSEL, nOutputStartPos, (LPARAM)-1);
                            SendMessage(hWndOutputView, AEM_LOCKSCROLL, SB_BOTH, FALSE);
                            ReplaceTextAnsi(hWndOutputView, dat.nCodePage, (char *)szTmp, dwAllData, FALSE);
                            SendMessage(hWndOutputView, AEM_LOCKSCROLL, SB_BOTH, TRUE);
                            SendMessage(hWndOutputView, EM_EXSETSEL64, 0, (LPARAM)&cr);
                            SendMessage(hWndOutputView, AEM_LOCKSCROLL, SB_BOTH, FALSE);
                            SendMessage(hWndOutputView, WM_SETREDRAW, TRUE, 0);
                            InvalidateRect(hWndOutputView, NULL, TRUE);
                          }
                          nOutputCodePage=dat.nCodePage;
                          if (oe->nInputCodePage == oe->nOutputCodePage)
                            nInputCodePage=nOutputCodePage;
                          bInsert=FALSE;

                          //Stop detecting in this loop
                          //dwAllData=(DWORD)-1;
                          //GlobalFree((HGLOBAL)szTmp);
                          //szTmp=NULL;
                        }
                      }
                    }
                  }
                  if (szAllData) GlobalFree((HGLOBAL)szAllData);
                  szAllData=szTmp;
                }

                //Add text
                if (bTargetPanel && bInsert)
                {
                   GetOutputScroll(oe);
                   AppendTextAnsi(hWndOutputView, nOutputCodePage, (char *)szData, dwResult);
                   SetOutputScroll(oe);
                }
              }

              dwRead=0;
              GlobalFree((HGLOBAL)szData);
            }
          }
          else if (dwExitCode == STILL_ACTIVE)
          {
            if (oe->wszInputWrite[0])
            {
              char *szInputWrite;
              int nInputWriteLen;

              if (nInputWriteLen=WideCharToMultiByte(nInputCodePage, 0, oe->wszInputWrite, -1, NULL, 0, NULL, NULL))
              {
                if (szInputWrite=GlobalAlloc(GPTR, nInputWriteLen))
                {
                  WideCharToMultiByte(nInputCodePage, 0, oe->wszInputWrite, -1, szInputWrite, nInputWriteLen, NULL, NULL);
                  szInputWrite[nInputWriteLen - 1]=L'\n';

                  if (!WriteFile(hInputWrite, szInputWrite, nInputWriteLen, &dwResult, NULL) || dwResult != (DWORD)nInputWriteLen)
                    break;
                  //FlushFileBuffers(hInputWrite);
                  GlobalFree((HGLOBAL)szInputWrite);
                }
              }
              oe->wszInputWrite[0]=L'\0';
            }

            Sleep(10);
            dwTimeCount+=10;
          }
          else break;
        }
        if (szAllData)
        {
          if (!bTargetPanel)
          {
            if (!(oe->dwOutputFlags & OUTF_TARGETNEWTAB) || SendMessage(hMainWnd, WM_COMMAND, IDM_FILE_NEW, TRUE))
            {
              if (hWndEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL))
              {
                nFirstLine=SaveLineScroll(hWndEdit);
                if ((oe->dwOutputFlags & OUTF_TARGETONLYDOC) || ((oe->dwOutputFlags & OUTF_TARGETSELDOC) && !SendMessage(hWndEdit, AEM_GETSEL, (WPARAM)NULL, (LPARAM)NULL)))
                  SendMessage(hWndEdit, EM_SETSEL, 0, (LPARAM)-1);
                ReplaceTextAnsi(hWndEdit, nOutputCodePage, (char *)szAllData, dwAllData, TRUE);
                RestoreLineScroll(hWndEdit, nFirstLine);
              }
            }
          }
          GlobalFree((HGLOBAL)szAllData);
        }

        //Stop execution
        bResult=OutputStop(pi.hProcess, TRUE);

        oe->dwExitCode=0;
        GetExitCodeProcess(pi.hProcess, &oe->dwExitCode);
        CloseHandle(pi.hProcess);
        oe->hProcess=NULL;
        oe->dwProcessId=0;
      }
      else
      {
        xprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_MSG_CANTEXECCOMMAND), wszCmdLine);
        MessageBoxW(hMainWnd, wszBuffer, wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);
      }
    }

    if (hOutputWrite) CloseHandle(hOutputWrite);
    if (hInputRead) CloseHandle(hInputRead);
    if (hErrorWrite) CloseHandle(hErrorWrite);
    if (hOutputReadTmp) CloseHandle(hOutputReadTmp);
    if (hInputWriteTmp) CloseHandle(hInputWriteTmp);

    if (hOutputRead) CloseHandle(hOutputRead);
    if (hInputWrite) CloseHandle(hInputWrite);
  }

  if (nNumberOfBytesToWrite)
    SendMessage(hMainWnd, AKD_FREETEXT, 0, (LPARAM)tr.pText);
  return bResult;
}

BOOL OutputStop(HANDLE hProcess, BOOL bTryClose)
{
  BOOL bResult=FALSE;
  DWORD dwExitCode;

  if (hProcess && GetExitCodeProcess(hProcess, &dwExitCode))
  {
    //Try to close
    if (dwExitCode == STILL_ACTIVE)
    {
      if (bTryClose)
      {
        HWND hWndConsole;

        if (hWndConsole=GetConsoleWindowByProcessId(oe.dwProcessId))
          SendMessage(hWndConsole, WM_CLOSE, 0, 0);
      }

      if (GetExitCodeProcess(hProcess, &dwExitCode))
      {
        //Force termination
        if (dwExitCode == STILL_ACTIVE)
          TerminateProcess(hProcess, 1);
        bResult=TRUE;
      }
    }
    else bResult=TRUE;
  }
  return bResult;
}

HWND GetConsoleWindowByProcessId(DWORD dwConsoleProcessId)
{
  CONSOLEWINDOW cw;

  cw.hWndConsole=NULL;
  cw.dwConsoleProcessId=dwConsoleProcessId;
  EnumWindows(EnumConsoleProc, (LPARAM)&cw);
  return cw.hWndConsole;
}

BOOL CALLBACK EnumConsoleProc(HWND hWnd, LPARAM lParam)
{
  char szClassName[32];

  if (GetClassNameA(hWnd, szClassName, 32) && !lstrcmpiA(szClassName, "ConsoleWindowClass"))
  {
    CONSOLEWINDOW *cw=(CONSOLEWINDOW *)lParam;
    DWORD dwProcessId=0;

    GetWindowThreadProcessId(hWnd, &dwProcessId);
    if (dwProcessId == cw->dwConsoleProcessId)
    {
      cw->hWndConsole=hWnd;
      return FALSE;
    }
  }
  return TRUE;
}

void AppendTextAnsi(HWND hWnd, int nCodePage, const char *pText, UINT_PTR dwTextLen)
{
  AEAPPENDTEXTA at;

  at.pText=pText;
  at.dwTextLen=dwTextLen;
  at.nNewLine=AELB_ASINPUT;
  at.nCodePage=nCodePage;
  SendMessage(hWnd, AEM_APPENDTEXTA, 0, (LPARAM)&at);
}

void AppendTextWide(HWND hWnd, const wchar_t *wpText, UINT_PTR dwTextLen)
{
  AEAPPENDTEXTW at;

  at.pText=wpText;
  at.dwTextLen=dwTextLen;
  at.nNewLine=AELB_ASINPUT;
  SendMessage(hWnd, AEM_APPENDTEXTW, 0, (LPARAM)&at);
}

void ReplaceTextAnsi(HWND hWnd, int nCodePage, const char *pText, UINT_PTR dwTextLen, BOOL bSelect)
{
  AEREPLACESELA rs;
  AECHARRANGE cr;

  rs.pText=pText;
  rs.dwTextLen=dwTextLen;
  rs.nNewLine=AELB_ASINPUT;
  rs.dwFlags=(oe.dwOutputFlags & OUTF_NOSCROLL)?AEREPT_LOCKSCROLL:0;
  rs.ciInsertStart=&cr.ciMin;
  rs.ciInsertEnd=&cr.ciMax;
  rs.nCodePage=nCodePage;
  SendMessage(hWnd, AEM_REPLACESELA, 0, (LPARAM)&rs);

  if (bSelect)
    SendMessage(hWnd, AEM_EXSETSEL, (WPARAM)&cr.ciMax, (LPARAM)&cr.ciMin);
}

INT_PTR GetTextLength(HWND hWnd)
{
  GETTEXTLENGTHEX gtl;

  gtl.flags=GTL_PRECISE|GTL_NUMCHARS;
  gtl.codepage=1200;
  return SendMessage(hWnd, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);
}

void GetOutputScroll(OUTPUTEXEC *oe)
{
  if (!(oe->dwOutputFlags & OUTF_NOSCROLL))
  {
    int nFirstVisLine;

    //If scroll moved up, then stop scroll to end
    nFirstVisLine=(int)SendMessage(hWndOutputView, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
    if (nFirstVisLine < oe->nPrevFirstVisLine)
      oe->nPrevFirstVisLine=-1;

    //If scroll moved to the bottom, then begin scroll to end
    if (oe->nPrevFirstVisLine != -1 ||
        SendMessage(hWndOutputView, AEM_GETLINENUMBER, AEGL_LASTVISIBLELINE, 0) ==
        SendMessage(hWndOutputView, AEM_GETLINENUMBER, AEGL_LINECOUNT, 0) - 1)
    {
      oe->nPrevFirstVisLine=nFirstVisLine;
    }
  }
}

void SetOutputScroll(OUTPUTEXEC *oe)
{
  if (!(oe->dwOutputFlags & OUTF_NOSCROLL))
  {
    POINT64 ptGlobalMax;

    if (oe->nPrevFirstVisLine != -1)
    {
      SendMessage(hWndOutputView, AEM_GETSCROLLPOS, (WPARAM)&ptGlobalMax, (LPARAM)NULL);
      ptGlobalMax.x=-1;
      SendMessage(hWndOutputView, AEM_SETSCROLLPOS, 0, (LPARAM)&ptGlobalMax);
      oe->nPrevFirstVisLine=(int)SendMessage(hWndOutputView, AEM_GETLINENUMBER, AEGL_FIRSTVISIBLELINE, 0);
    }
  }
}

void FillOutputDefault(OUTPUTEXEC *oe)
{
  xstrcpynW(oe->wszPattern, wszPattern, BUFFER_SIZE);
  GetPatOptions(wszPatTags, oe);
  xstrcpynW(oe->wszCoderAlias, wszCoderAlias, BUFFER_SIZE);
  oe->nInputCodePage=bAutoCodePage?-1:nForceCodePage;
  oe->nOutputCodePage=bAutoCodePage?-1:nForceCodePage;
  oe->dwOutputFlags=dwOutputFlags;
}

BOOL GetPatOptions(const wchar_t *wpPatTags, OUTPUTEXEC *oe)
{
  const wchar_t *wpArgCount=wpPatTags;
  wchar_t wszNameAndValue[MAX_PATH + 10];
  wchar_t *wpNameAndValue=wszNameAndValue;
  const wchar_t *wpName;
  const wchar_t *wpValue;
  const wchar_t *wpCount;
  int nNameAndValueLen;
  int nNameLen;
  int nValueLen;
  BOOL bResult=FALSE;

  oe->wszPatTagFile[0]=L'\0';
  oe->wszPatTagLine[0]=L'\0';

  //Parse tag options: `/FILE=\\1 /FRAME=\\1 /GOTOLINE=\\2:\\3 /GOTOBYTE=\\2 /GOTOCHAR=\\2`
  if (*wpPatTags)
  {
    while (*wpArgCount)
    {
      nNameAndValueLen=GetCommandLineArg(wpArgCount, wpNameAndValue, MAX_PATH, &wpArgCount);

      if (wpNameAndValue[0] == L'/')
      {
        //Argument name
        wpName=wpNameAndValue + 1;

        for (wpCount=wpName; *wpCount && *wpCount != L'=' && *wpCount != L' '; ++wpCount);
        if (!(nNameLen=(int)(wpCount - wpName)))
          break;

        //Argument value
        if (*wpCount == L'=')
        {
          wpValue=wpCount + 1;
          nValueLen=nNameAndValueLen - (int)(wpValue - wpNameAndValue);

          //"Value" -> Value
          if (nValueLen >= 2)
          {
            if (wpValue[0] == L'\"' && wpValue[nValueLen - 1] == L'\"')
            {
              wpValue+=1;
              nValueLen-=2;
            }
          }
        }
        else
        {
          wpValue=L"";
          nValueLen=0;
        }

        //Copy argument
        if (!xstrcmpinW(L"FILE", wpName, (UINT_PTR)-1))
        {
          xstrcpynW(oe->wszPatTagFile, wpValue, nValueLen + 1);
          oe->nFindDocType=FDT_BYFILENAME;
          bResult=TRUE;
        }
        else if (!xstrcmpinW(L"FRAME", wpName, (UINT_PTR)-1))
        {
          xstrcpynW(oe->wszPatTagFile, wpValue, nValueLen + 1);
          oe->nFindDocType=FDT_BYFRAME;
          bResult=TRUE;
        }
        else if (!xstrcmpinW(L"GOTOLINE", wpName, (UINT_PTR)-1))
        {
          xstrcpynW(oe->wszPatTagLine, wpValue, nValueLen + 1);
          oe->nGotoType=GT_LINE;
          bResult=TRUE;
        }
        else if (!xstrcmpinW(L"GOTOBYTE", wpName, (UINT_PTR)-1))
        {
          xstrcpynW(oe->wszPatTagLine, wpValue, nValueLen + 1);
          oe->nGotoType=GT_OFFSETBYTE;
          bResult=TRUE;
        }
        else if (!xstrcmpinW(L"GOTOCHAR", wpName, (UINT_PTR)-1))
        {
          xstrcpynW(oe->wszPatTagLine, wpValue, nValueLen + 1);
          oe->nGotoType=GT_OFFSETCHAR;
          bResult=TRUE;
        }
      }
      else break;
    }
  }
  return bResult;
}

BOOL PatOpenLine(HWND hWnd, const OUTPUTEXEC *oe, const AECHARINDEX *ciChar, AETEXTRANGEW *tr)
{
  BOOL bResult=FALSE;

  //Line pattern
  if (GetLineAtIndex(hWnd, ciChar, tr))
  {
    PATEXEC pe;

    //Fill structure for AKD_PATEXEC
    pe.lpREGroupStack=0;
    pe.wpStr=tr->pBuffer;
    pe.wpMaxStr=tr->pBuffer + tr->dwBufferMax;
    pe.wpText=pe.wpStr;
    pe.wpMaxText=pe.wpMaxStr;
    pe.wpPat=oe->wszPattern;
    pe.wpMaxPat=oe->wszPattern + xstrlenW(oe->wszPattern);
    pe.dwOptions=RESE_MATCHCASE;
    pe.wpDelim=NULL;
    pe.wpMaxDelim=NULL;
    pe.lpCallback=NULL;

    if (SendMessage(hMainWnd, AKD_PATEXEC, 0, (LPARAM)&pe))
    {
      PATGROUPSTR pgs;
      wchar_t wszFile[MAX_PATH];
      wchar_t wszDir[MAX_PATH];
      wchar_t *wpLine=NULL;
      INT_PTR nSize;

      wszFile[0]=L'\0';
      if (*oe->wszPatTagFile)
      {
        pgs.lpREGroupStack=pe.lpREGroupStack;
        pgs.wpStr=oe->wszPatTagFile;
        pgs.wpMaxStr=pgs.wpStr + xstrlenW(pgs.wpStr);
        pgs.wszResult=NULL;
        if (nSize=SendMessage(hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs))
        {
          if (pgs.wszResult=GlobalAlloc(GPTR, nSize * sizeof(wchar_t)))
          {
            SendMessage(hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs);
            xstrcpynW(wszFile, pgs.wszResult, MAX_PATH);
            GlobalFree((HGLOBAL)pgs.wszResult);
          }
        }
      }
      if (oe->nGotoType && *oe->wszPatTagLine)
      {
        pgs.lpREGroupStack=pe.lpREGroupStack;
        pgs.wpStr=oe->wszPatTagLine;
        pgs.wpMaxStr=pgs.wpStr + xstrlenW(pgs.wpStr);
        pgs.wszResult=NULL;
        if (nSize=SendMessage(hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs))
        {
          if (pgs.wszResult=GlobalAlloc(GPTR, nSize * sizeof(wchar_t)))
          {
            SendMessage(hMainWnd, AKD_PATGROUPSTR, 0, (LPARAM)&pgs);
            wpLine=pgs.wszResult;
          }
        }
      }

      if (*wszFile)
      {
        FRAMEDATA *lpFrame=NULL;

        if (oe->nFindDocType == FDT_BYFILENAME)
        {
          //Get full file name
          if (*oe->wszDir)
          {
            GetCurrentDirectoryWide(MAX_PATH, wszDir);
            SetCurrentDirectoryWide(oe->wszDir);
          }
          GetFullName(wszFile, wszFile, MAX_PATH, NULL);
          if (*oe->wszDir) SetCurrentDirectoryWide(wszDir);
          lpFrame=(FRAMEDATA *)SendMessage(hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)wszFile);
        }
        else if (oe->nFindDocType == FDT_BYFRAME)
        {
          lpFrame=(FRAMEDATA *)xatoiW(wszFile, NULL);
          if (!SendMessage(hMainWnd, AKD_FRAMEISVALID, 0, (LPARAM)lpFrame))
            lpFrame=NULL;
        }

        //Find document by file name
        if (lpFrame)
        {
          //Document already open - activate it
          SendMessage(hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrame);
          bResult=TRUE;
        }
        else if (oe->nFindDocType == FDT_BYFILENAME)
        {
          //Document not open  - open it
          if (FileExistsWide(wszFile))
          {
            OPENDOCUMENTW od;

            od.pFile=wszFile;
            od.pWorkDir=oe->wszDir;
            od.dwFlags=OD_ADT_BINARYERROR|OD_ADT_REGCODEPAGE;
            od.nCodePage=0;
            od.bBOM=0;
            od.hDoc=NULL;
            SendMessage(hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);

            bResult=TRUE;
          }
        }
      }
      if (oe->nGotoType && wpLine)
      {
        HWND hWndEdit;

        if (hWndEdit=(HWND)SendMessage(hMainWnd, AKD_GETFRAMEINFO, FI_WNDEDIT, (LPARAM)NULL))
        {
          SendMessage(hMainWnd, AKD_GOTOW, oe->nGotoType, (LPARAM)wpLine);
          bResult=TRUE;
        }
        GlobalFree((HGLOBAL)wpLine);
      }
    }
    SendMessage(hMainWnd, AKD_PATFREE, 0, (LPARAM)&pe);

    GlobalFree((HGLOBAL)tr->pBuffer);
    tr->pBuffer=NULL;
  }
  return bResult;
}

BOOL GetLineAtIndex(HWND hWnd, const AECHARINDEX *ciChar, AETEXTRANGEW *tr)
{
  AEC_WrapLineBeginEx(ciChar, &tr->cr.ciMin);
  AEC_WrapLineEndEx(ciChar, &tr->cr.ciMax);
  tr->bColumnSel=FALSE;
  tr->pBuffer=NULL;
  tr->dwBufferMax=0;
  tr->nNewLine=AELB_ASOUTPUT;
  tr->bFillSpaces=FALSE;

  if (tr->dwBufferMax=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)tr))
    if (tr->pBuffer=(wchar_t *)GlobalAlloc(GPTR, tr->dwBufferMax * sizeof(wchar_t)))
      if (tr->dwBufferMax=SendMessage(hWnd, AEM_GETTEXTRANGEW, 0, (LPARAM)tr))
        return TRUE;
  return FALSE;
}

void FillComboboxCodepage(HWND hWnd, int *lpCodepageList)
{
  int i;

  if (lpCodepageList)
  {
    for (i=0; lpCodepageList[i]; ++i)
    {
      if (IsCodePageUnicode(lpCodepageList[i]) && lpCodepageList[i] != CP_UNICODE_UTF8)
        continue;
      GetCodePageName(lpCodepageList[i], wszBuffer, BUFFER_SIZE);
      ComboBox_AddStringWide(hWnd, wszBuffer);
    }
  }
}

int SelectComboboxCodepage(HWND hWnd, int nCodepage)
{
  int nSelection;

  xprintfW(wszBuffer, L"%d ", nCodepage);
  nSelection=ComboBox_FindStringWide(hWnd, -1, wszBuffer);
  SendMessage(hWnd, CB_SETCURSEL, (WPARAM)nSelection, 0);

  return nSelection;
}

void GetCodePageName(int nCodePage, wchar_t *wszCodePage, int nLen)
{
  CPINFOEXW CPInfoExW;

  if (nCodePage)
  {
    if (nCodePage == CP_UNICODE_UTF8)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF8W, nLen);
    else if (nCodePage == CP_UNICODE_UTF7)
      xstrcpynW(wszCodePage, STR_UNICODE_UTF7W, nLen);
    else
    {
      if (GetCPInfoExWide(nCodePage, 0, &CPInfoExW) && nCodePage == xatoiW(CPInfoExW.CodePageName, NULL))
        xstrcpynW(wszCodePage, CPInfoExW.CodePageName, nLen);
      else
        xprintfW(wszCodePage, L"%d ", nCodePage);
    }
  }
  else wszCodePage[0]='\0';
}

BOOL IsCodePageUnicode(int nCodePage)
{
  if (nCodePage == CP_UNICODE_UTF16LE ||
      nCodePage == CP_UNICODE_UTF16BE ||
      nCodePage == CP_UNICODE_UTF32LE ||
      nCodePage == CP_UNICODE_UTF32BE ||
      nCodePage == CP_UNICODE_UTF8)
  {
    return TRUE;
  }
  return FALSE;
}

void ChangeTwoBytesOrder(unsigned char *lpBuffer, unsigned int nBufferLen)
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

int TranslateFileString(const wchar_t *wpString, wchar_t *wszBuffer, int nBufferSize)
{
  //%a -AkelPad directory, %% -%
  wchar_t *wpExeDir=wszExeDir;
  wchar_t *wszSource;
  wchar_t *wpSource;
  wchar_t *wpTarget=wszBuffer;
  wchar_t *wpTargetMax=(wszBuffer ? (wszBuffer + nBufferSize) : (wchar_t *)MAXUINT_PTR);
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

BOOL GetFullName(const wchar_t *wpFile, wchar_t *wszFileFullName, int nFileMax, int *lpnFileLen)
{
  wchar_t wszFileBuf[MAX_PATH];
  wchar_t *wpFileName;
  int nFileLen=0;
  BOOL bResult=FALSE;

  if (GetFullPathNameWide(wpFile, MAX_PATH, wszFileBuf, &wpFileName))
  {
    if (bResult=FileExistsWide(wszFileBuf))
    {
      if (nFileLen=GetLongPathNameWide(wszFileBuf, wszFileFullName, nFileMax))
        goto End;

      //GetLongPathName can fail on x86 build when open system file on x64,
      //but we return TRUE to allow AkelAdmin.exe execution.
    }
    nFileLen=(int)xstrcpynW(wszFileFullName, wszFileBuf, nFileMax);
  }
  else wszFileFullName[0]=L'\0';

  End:
  if (lpnFileLen) *lpnFileLen=nFileLen;
  return bResult;
}

BOOL GetWindowSize(HWND hWnd, HWND hWndOwner, RECT *rc)
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

void ShowOutputMenu(HWND hWnd, HMENU hMenu, BOOL bMouse)
{
  POINT pt;
  CHARRANGE64 cr;
  int nCmd;

  if (!bMouse && SendMessage(hWnd, AEM_GETCARETPOS, (WPARAM)&pt, 0))
  {
    pt.y+=(int)SendMessage(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0);
    ClientToScreen(hWnd, &pt);
  }
  else GetCursorPos(&pt);

  SendMessage(hWnd, EM_EXGETSEL64, 0, (LPARAM)&cr);
  EnableMenuItem(hMenu, IDM_OUTPUT_COPY, (cr.cpMin < cr.cpMax)?MF_ENABLED:MF_GRAYED);

  nCmd=TrackPopupMenu(hMenu, TPM_RETURNCMD|TPM_NONOTIFY|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

  if (nCmd == IDM_OUTPUT_COPY)
    SendMessage(hWnd, WM_COPY, 0, 0);
  else if (nCmd == IDM_OUTPUT_SELECTALL)
    SendMessage(hWnd, EM_SETSEL, 0, -1);
  else if (nCmd == IDM_OUTPUT_CLEAR)
    SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)L"");
  else if (nCmd == IDM_OUTPUT_SETUP)
    PostMessage(hWndDockDlg, AKDLL_SETUP, 0, 0);
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


//// Options

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

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_READ, (LPARAM)wszPluginName))
  {
    WideOption(hOptions, L"WatchUpdateInterval", PO_DWORD, (LPBYTE)&dwWatchUpdateInterval, sizeof(DWORD));
    WideOption(hOptions, L"WatchCompleteReopen", PO_DWORD, (LPBYTE)&bWatchCompleteReopen, sizeof(DWORD));
    WideOption(hOptions, L"WatchScrollEnd", PO_DWORD, (LPBYTE)&dwWatchScrollEnd, sizeof(DWORD));

    WideOption(hOptions, L"OutputAutoCodepage", PO_DWORD, (LPBYTE)&bAutoCodePage, sizeof(DWORD));
    WideOption(hOptions, L"OutputForceCodepage", PO_DWORD, (LPBYTE)&nForceCodePage, sizeof(DWORD));
    WideOption(hOptions, L"Pattern", PO_STRING, (LPBYTE)wszPattern, sizeof(wszPattern));
    WideOption(hOptions, L"PatTags", PO_STRING, (LPBYTE)wszPatTags, sizeof(wszPatTags));
    WideOption(hOptions, L"CoderAlias", PO_STRING, (LPBYTE)wszCoderAlias, sizeof(wszCoderAlias));
    WideOption(hOptions, L"OutputFlags", PO_DWORD, (LPBYTE)&dwOutputFlags, sizeof(DWORD));
    WideOption(hOptions, L"NextMatchKey", PO_DWORD, (LPBYTE)&dwNextMatchKey, sizeof(DWORD));
    WideOption(hOptions, L"PrevMatchKey", PO_DWORD, (LPBYTE)&dwPrevMatchKey, sizeof(DWORD));
    WideOption(hOptions, L"RunCmdKey", PO_DWORD, (LPBYTE)&dwRunCmdKey, sizeof(DWORD));
    WideOption(hOptions, L"RunCmdDlgKey", PO_DWORD, (LPBYTE)&dwRunCmdDlgKey, sizeof(DWORD));

    WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&nOutputDockSide, sizeof(DWORD));
    WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&rcOutputDockRect, sizeof(RECT));

    WideOption(hOptions, L"RunRect", PO_BINARY, (LPBYTE)&rcRunCurrentDialog, sizeof(RECT));
    WideOption(hOptions, L"RunCmd", PO_STRING, (LPBYTE)wszRunCmdLine, sizeof(wszRunCmdLine));
    WideOption(hOptions, L"RunDir", PO_STRING, (LPBYTE)wszRunDir, sizeof(wszRunDir));

    SendMessage(hMainWnd, AKD_ENDOPTIONS, (WPARAM)hOptions, 0);
  }
}

void SaveOptions(DWORD dwFlags)
{
  HANDLE hOptions;

  if (hOptions=(HANDLE)SendMessage(hMainWnd, AKD_BEGINOPTIONSW, POB_SAVE, (LPARAM)wszPluginName))
  {
    if (dwFlags & OF_WATCH)
    {
      WideOption(hOptions, L"WatchUpdateInterval", PO_DWORD, (LPBYTE)&dwWatchUpdateInterval, sizeof(DWORD));
      WideOption(hOptions, L"WatchCompleteReopen", PO_DWORD, (LPBYTE)&bWatchCompleteReopen, sizeof(DWORD));
      WideOption(hOptions, L"WatchScrollEnd", PO_DWORD, (LPBYTE)&dwWatchScrollEnd, sizeof(DWORD));
    }
    if (dwFlags & OF_OUTPUT)
    {
      WideOption(hOptions, L"OutputAutoCodepage", PO_DWORD, (LPBYTE)&bAutoCodePage, sizeof(DWORD));
      WideOption(hOptions, L"OutputForceCodepage", PO_DWORD, (LPBYTE)&nForceCodePage, sizeof(DWORD));
      WideOption(hOptions, L"Pattern", PO_STRING, (LPBYTE)wszPattern, ((int)xstrlenW(wszPattern) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"PatTags", PO_STRING, (LPBYTE)wszPatTags, ((int)xstrlenW(wszPatTags) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"CoderAlias", PO_STRING, (LPBYTE)wszCoderAlias, ((int)xstrlenW(wszCoderAlias) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"OutputFlags", PO_DWORD, (LPBYTE)&dwOutputFlags, sizeof(DWORD));
      WideOption(hOptions, L"NextMatchKey", PO_DWORD, (LPBYTE)&dwNextMatchKey, sizeof(DWORD));
      WideOption(hOptions, L"PrevMatchKey", PO_DWORD, (LPBYTE)&dwPrevMatchKey, sizeof(DWORD));
      WideOption(hOptions, L"RunCmdKey", PO_DWORD, (LPBYTE)&dwRunCmdKey, sizeof(DWORD));
      WideOption(hOptions, L"RunCmdDlgKey", PO_DWORD, (LPBYTE)&dwRunCmdDlgKey, sizeof(DWORD));
    }
    if (dwFlags & OF_DOCKRECT)
    {
      WideOption(hOptions, L"DockSide", PO_DWORD, (LPBYTE)&dkOutputDlg->nSide, sizeof(DWORD));
      WideOption(hOptions, L"DockRect", PO_BINARY, (LPBYTE)&dkOutputDlg->rcSize, sizeof(RECT));
    }
    if (dwFlags & OF_RUN)
    {
      WideOption(hOptions, L"RunCmd", PO_STRING, (LPBYTE)wszRunCmdLine, ((int)xstrlenW(wszRunCmdLine) + 1) * sizeof(wchar_t));
      WideOption(hOptions, L"RunDir", PO_STRING, (LPBYTE)wszRunDir, ((int)xstrlenW(wszRunDir) + 1) * sizeof(wchar_t));
    }
    if (dwFlags & OF_RUNRECT)
    {
      WideOption(hOptions, L"RunRect", PO_BINARY, (LPBYTE)&rcRunCurrentDialog, sizeof(RECT));
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
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_MSG_MODIFIED_ONSTART)
      return L"\x0424\x0430\x0439\x043B\x0020\x0438\x0437\x043C\x0435\x043D\x0435\x043D\x002E\x0020\x0421\x043F\x0435\x0440\x0432\x0430\x0020\x0441\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x0435\x0020\x0435\x0433\x043E\x002C\x0020\x043B\x0438\x0431\x043E\x0020\x043F\x0435\x0440\x0435\x043E\x0442\x043A\x0440\x043E\x0439\x0442\x0435\x0020\x0438\x0020\x043F\x043E\x0432\x0442\x043E\x0440\x0438\x0442\x0435\x0020\x0437\x0430\x043F\x0443\x0441\x043A\x0020\x043F\x043B\x0430\x0433\x0438\x043D\x0430\x002E";
    if (nStringID == STRID_MSG_MODIFIED_ONSTOP)
      return L"\x0424\x0430\x0439\x043B\x0020\x0438\x0437\x043C\x0435\x043D\x0435\x043D\x002E\x0020\x041F\x043B\x0430\x0433\x0438\x043D\x0020\x0431\x0443\x0434\x0435\x0442\x0020\x043E\x0441\x0442\x0430\x043D\x043E\x0432\x043B\x0435\x043D\x002E";
    if (nStringID == STRID_MSG_ONSTOP)
      return L"\x041F\x043B\x0430\x0433\x0438\x043D\x0020\x043E\x0441\x0442\x0430\x043D\x043E\x0432\x043B\x0435\x043D\x002E";
    if (nStringID == STRID_MSG_CANTOPENFILE_ONSTOP)
      return L"\x041D\x0435\x0432\x043E\x0437\x043C\x043E\x0436\x043D\x043E\x0020\x043E\x0442\x043A\x0440\x044B\x0442\x044C\x0020\x0444\x0430\x0439\x043B\x002E\x0020\x041F\x043B\x0430\x0433\x0438\x043D\x0020\x0431\x0443\x0434\x0435\x0442\x0020\x043E\x0441\x0442\x0430\x043D\x043E\x0432\x043B\x0435\x043D\x002E";
    if (nStringID == STRID_MSG_NOFILE)
      return L"\x041D\x0435\x0442\x0020\x0444\x0430\x0439\x043B\x043E\x0432\x0020\x0434\x043B\x044F\x0020\x043F\x0440\x043E\x0441\x043C\x043E\x0442\x0440\x0430\x002E";
    if (nStringID == STRID_UPDATEINTERVAL)
      return L"\x0418\x043D\x0442\x0435\x0440\x0432\x0430\x043B\x0020\x043E\x0431\x043D\x043E\x0432\x043B\x0435\x043D\x0438\x044F";
    if (nStringID == STRID_MS)
      return L"\x043C\x0441\x002E";
    if (nStringID == STRID_SCROLL)
      return L"\x041F\x0440\x043E\x043A\x0440\x0443\x0442\x043A\x0430";
    if (nStringID == STRID_SCROLLENDAUTO)
      return L"\x0410\x0432\x0442\x043E\x043C\x0430\x0442\x0438\x0447\x0435\x0441\x043A\x0430\x044F";
    if (nStringID == STRID_SCROLLENDCARET)
      return L"\x041F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x0443\x0020\x0432\x0020\x043A\x043E\x043D\x0435\x0446\x0020\x0438\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0442\x0438\x0442\x044C\x0020\x043A\x0020\x043D\x0435\x0439";
    if (nStringID == STRID_SCROLLENDVERT)
      return L"\x041D\x0435\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x0449\x0430\x0442\x044C\x0020\x043A\x0430\x0440\x0435\x0442\x043A\x0443\x0020\x0438\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0442\x0438\x0442\x044C\x0020\x0432\x0020\x043A\x043E\x043D\x0435\x0446\x0020\x0432\x0435\x0440\x0442\x0438\x043A\x0430\x043B\x044C\x043D\x043E";
    if (nStringID == STRID_SCROLLENDNO)
      return L"\x0411\x0435\x0437\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0447\x0438\x0432\x0430\x043D\x0438\x044F";
    if (nStringID == STRID_COMPLETEREOPEN)
      return L"\x041F\x0435\x0440\x0435\x043E\x0442\x043A\x0440\x044B\x0432\x0430\x0442\x044C\x0020\x043F\x043E\x043B\x043D\x043E\x0441\x0442\x044C\x044E";
    if (nStringID == STRID_SETUP)
      return L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x002E\x002E\x002E";
    if (nStringID == STRID_MSG_CANTEXECCOMMAND)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0437\x0430\x043F\x0443\x0441\x0442\x0438\x0442\x044C\x0020\x043A\x043E\x043C\x0430\x043D\x0434\x0443: %s";
    if (nStringID == STRID_MENUCOPY)
      return L"\x041A\x043E\x043F\x0438\x0440\x043E\x0432\x0430\x0442\x044C";
    if (nStringID == STRID_MENUSELECTALL)
      return L"\x0412\x044B\x0434\x0435\x043B\x0438\x0442\x044C\x0020\x0432\x0441\x0435";
    if (nStringID == STRID_MENUCLEAR)
      return L"\x041E\x0447\x0438\x0441\x0442\x0438\x0442\x044C";
    if (nStringID == STRID_INPUT)
      return L"\x0412\x0432\x043E\x0434";
    if (nStringID == STRID_STOP)
      return L"\x0421\x0442\x043E\x043F";
    if (nStringID == STRID_RUNDLG)
      return L"\x0417\x0430\x043F\x0443\x0441\x043A...";
    if (nStringID == STRID_EXEC)
      return L"\x0412\x044B\x043F\x043E\x043B\x043D\x0438\x0442\x044C";
    if (nStringID == STRID_COMMAND)
      return L"\x041A\x043E\x043C\x0430\x043D\x0434\x0430:";
    if (nStringID == STRID_DIRECTORY)
      return L"\x0414\x0438\x0440\x0435\x043A\x0442\x043E\x0440\x0438\x044F:";
    if (nStringID == STRID_SOURCE)
      return L"\x0418\x0441\x0442\x043E\x0447\x043D\x0438\x043A\x0020\x0432\x0432\x043E\x0434\x0430";
    if (nStringID == STRID_TARGET)
      return L"\x041F\x0440\x0438\x0435\x043C\x043D\x0438\x043A\x0020\x0432\x044B\x0432\x043E\x0434\x0430";
    if (nStringID == STRID_NO)
      return L"\x041D\x0435\x0442";
    if (nStringID == STRID_SELDOC)
      return L"\x0412\x044B\x0434\x0435\x043B\x0435\x043D\x0438\x0435\x0020\x0438\x043B\x0438\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442";
    if (nStringID == STRID_ALLDOC)
      return L"\x0412\x0435\x0441\x044C\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442";
    if (nStringID == STRID_ONLYSEL)
      return L"\x0422\x043E\x043B\x044C\x043A\x043E\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_PANEL)
      return L"\x041F\x0430\x043D\x0435\x043B\x044C";
    if (nStringID == STRID_NEWTAB)
      return L"\x041D\x043E\x0432\x0430\x044F\x0020\x0432\x043A\x043B\x0430\x0434\x043A\x0430";
    if (nStringID == STRID_CODEPAGE)
      return L"\x041A\x043E\x0434\x0438\x0440\x043E\x0432\x043A\x0430";
    if (nStringID == STRID_AUTODETECT)
      return L"\x0410\x0432\x0442\x043E\x043E\x043F\x0440\x0435\x0434\x0435\x043B\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_PATTERN)
      return L"\x0428\x0430\x0431\x043B\x043E\x043D\x0020\x0440\x0435\x0433\x0443\x043B\x044F\x0440\x043D\x043E\x0433\x043E\x0020\x0432\x044B\x0440\x0430\x0436\x0435\x043D\x0438\x044F:";
    if (nStringID == STRID_MENUMICROSOFTGCC)
      return L"Microsoft/GCC";
    if (nStringID == STRID_MENUPATBORLAND)
      return L"Borland";
    if (nStringID == STRID_PATTAGS)
      return L"\x0421\x043E\x043E\x0442\x0432\x0435\x0442\x0441\x0442\x0432\x0438\x0435\x0020\x0432\x0445\x043E\x0436\x0434\x0435\x043D\x0438\x0439\x0020\x0432\x044B\x0440\x0430\x0436\x0435\x043D\x0438\x044F:";
    if (nStringID == STRID_MENUTAGFILE)
      return L"\x0424\x0430\x0439\x043B";
    if (nStringID == STRID_MENUTAGGOTOLINE)
      return L"\x0421\x0442\x0440\x043E\x043A\x0430\x003A\x041A\x043E\x043B\x043E\x043D\x043A\x0430";
    if (nStringID == STRID_MENUTAGGOTOBYTE)
      return L"\x0421\x043C\x0435\x0449\x0435\x043D\x0438\x0435\x0020\x0432\x0020\x0431\x0430\x0439\x0442\x0430\x0445";
    if (nStringID == STRID_MENUTAGGOTOCHAR)
      return L"\x0421\x043C\x0435\x0449\x0435\x043D\x0438\x0435\x0020\x0432\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x0430\x0445";
    if (nStringID == STRID_SAVEALL)
      return L"\x0421\x043E\x0445\x0440\x0430\x043D\x0438\x0442\x044C\x0020\x0432\x0441\x0435\x0020\x0434\x043E\x043A\x0443\x043C\x0435\x043D\x0442\x044B\x0020\x043F\x0435\x0440\x0435\x0434\x0020\x0432\x044B\x043F\x043E\x043B\x043D\x0435\x043D\x0438\x0435\x043C";
    if (nStringID == STRID_APPENDNEXT)
      return L"\x0414\x043E\x0431\x0430\x0432\x043B\x044F\x0442\x044C\x0020\x0442\x0435\x043A\x0441\x0442\x0020\x0432\x0020\x043A\x043E\x043D\x0435\x0446";
    if (nStringID == STRID_NOSCROLL)
      return L"\x041D\x0435\x0020\x043F\x0440\x043E\x043A\x0440\x0443\x0447\x0438\x0432\x0430\x0442\x044C\x0020\x0432\x0020\x043A\x043E\x043D\x0435\x0446";
    if (nStringID == STRID_WRAP)
      return L"\x041F\x0435\x0440\x0435\x043D\x043E\x0441\x0438\x0442\x044C\x0020\x0441\x0442\x0440\x043E\x043A\x0438";
    if (nStringID == STRID_HIDEINPUT)
      return L"\x0421\x043A\x0440\x044B\x0432\x0430\x0442\x044C\x0020\x0441\x0442\x0440\x043E\x043A\x0443\x0020\x0432\x0432\x043E\x0434\x0430";
    if (nStringID == STRID_HOTKEYS)
      return L"\x0413\x043E\x0440\x044F\x0447\x0438\x0435\x0020\x043A\x043B\x0430\x0432\x0438\x0448\x0438";
    if (nStringID == STRID_NEXTMATCH)
      return L"\x0421\x043B\x0435\x0434\x0443\x044E\x0449\x0435\x0435\x0020\x0432\x0445\x043E\x0436\x0434\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_PREVMATCH)
      return L"\x041F\x0440\x0435\x0434\x044B\x0434\x0443\x0449\x0435\x0435\x0020\x0432\x0445\x043E\x0436\x0434\x0435\x043D\x0438\x0435";
    if (nStringID == STRID_RUNCMD)
      return L"\x0412\x044B\x043F\x043E\x043B\x043D\x0438\x0442\x044C\x0020\x043A\x043E\x043C\x0430\x043D\x0434\x0443";
    if (nStringID == STRID_RUNCMDDLG)
      return L"\x0414\x0438\x0430\x043B\x043E\x0433\x0020\x0437\x0430\x043F\x0443\x0441\x043A\x0430";
    if (nStringID == STRID_CODERALIAS)
      return L"\x041F\x0441\x0435\x0432\x0434\x043E\x043D\x0438\x043C\x0020\x0434\x043B\x044F\x0020\x0442\x0435\x043C\x044B\x0020\x0043\x006F\x0064\x0065\x0072";
    if (nStringID == STRID_PLUGIN)
      return L"%s \x043F\x043B\x0430\x0433\x0438\x043D";
    if (nStringID == STRID_OK)
      return L"\x004F\x004B";
    if (nStringID == STRID_CANCEL)
      return L"\x041E\x0442\x043C\x0435\x043D\x0430";
  }
  else
  {
    if (nStringID == STRID_MSG_MODIFIED_ONSTART)
      return L"File is modified. First save it or reopen and call plugin again.";
    if (nStringID == STRID_MSG_MODIFIED_ONSTOP)
      return L"File is modified. Plugin will be stopped.";
    if (nStringID == STRID_MSG_ONSTOP)
      return L"Plugin is stopped.";
    if (nStringID == STRID_MSG_CANTOPENFILE_ONSTOP)
      return L"Can't open file. Plugin will be stopped.";
    if (nStringID == STRID_MSG_NOFILE)
      return L"No file to view.";
    if (nStringID == STRID_UPDATEINTERVAL)
      return L"Update interval";
    if (nStringID == STRID_MS)
      return L"ms.";
    if (nStringID == STRID_SCROLL)
      return L"Scroll";
    if (nStringID == STRID_SCROLLENDAUTO)
      return L"Automatic";
    if (nStringID == STRID_SCROLLENDCARET)
      return L"Move caret to the end and scroll to it";
    if (nStringID == STRID_SCROLLENDVERT)
      return L"Don't move caret and scroll to the end vertically";
    if (nStringID == STRID_SCROLLENDNO)
      return L"No scrolling";
    if (nStringID == STRID_COMPLETEREOPEN)
      return L"Complete reopening";
    if (nStringID == STRID_SETUP)
      return L"Settings...";
    if (nStringID == STRID_MSG_CANTEXECCOMMAND)
      return L"Can't execute command: %s";
    if (nStringID == STRID_MENUCOPY)
      return L"Copy";
    if (nStringID == STRID_MENUSELECTALL)
      return L"Select all";
    if (nStringID == STRID_MENUCLEAR)
      return L"Clear";
    if (nStringID == STRID_INPUT)
      return L"Input";
    if (nStringID == STRID_STOP)
      return L"Stop";
    if (nStringID == STRID_RUNDLG)
      return L"Run...";
    if (nStringID == STRID_EXEC)
      return L"Exec";
    if (nStringID == STRID_COMMAND)
      return L"Command:";
    if (nStringID == STRID_DIRECTORY)
      return L"Directory:";
    if (nStringID == STRID_SOURCE)
      return L"Source input";
    if (nStringID == STRID_TARGET)
      return L"Target output";
    if (nStringID == STRID_NO)
      return L"No";
    if (nStringID == STRID_SELDOC)
      return L"Selection or document";
    if (nStringID == STRID_ALLDOC)
      return L"All document";
    if (nStringID == STRID_ONLYSEL)
      return L"Only selection";
    if (nStringID == STRID_PANEL)
      return L"Panel";
    if (nStringID == STRID_NEWTAB)
      return L"New tab";
    if (nStringID == STRID_CODEPAGE)
      return L"Codepage";
    if (nStringID == STRID_AUTODETECT)
      return L"Autodetect";
    if (nStringID == STRID_PATTERN)
      return L"Regular expression pattern:";
    if (nStringID == STRID_MENUMICROSOFTGCC)
      return L"Microsoft/GCC";
    if (nStringID == STRID_MENUPATBORLAND)
      return L"Borland";
    if (nStringID == STRID_PATTAGS)
      return L"Pattern matches map:";
    if (nStringID == STRID_MENUTAGFILE)
      return L"File";
    if (nStringID == STRID_MENUTAGGOTOLINE)
      return L"Line:Column";
    if (nStringID == STRID_MENUTAGGOTOBYTE)
      return L"Byte offset";
    if (nStringID == STRID_MENUTAGGOTOCHAR)
      return L"Char offset";
    if (nStringID == STRID_SAVEALL)
      return L"Save all documents before execution";
    if (nStringID == STRID_APPENDNEXT)
      return L"Append text to the end";
    if (nStringID == STRID_NOSCROLL)
      return L"No scroll to the end";
    if (nStringID == STRID_WRAP)
      return L"Wrap lines";
    if (nStringID == STRID_HIDEINPUT)
      return L"Hide input line";
    if (nStringID == STRID_HOTKEYS)
      return L"Hotkeys";
    if (nStringID == STRID_NEXTMATCH)
      return L"Next match";
    if (nStringID == STRID_PREVMATCH)
      return L"Previous match";
    if (nStringID == STRID_RUNCMD)
      return L"Execute command";
    if (nStringID == STRID_RUNCMDDLG)
      return L"Execute dialog";
    if (nStringID == STRID_CODERALIAS)
      return L"Alias for Coder theme";
    if (nStringID == STRID_PLUGIN)
      return L"%s plugin";
    if (nStringID == STRID_OK)
      return L"OK";
    if (nStringID == STRID_CANCEL)
      return L"Cancel";
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
  bOldWindows=pd->bOldWindows;
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

  wszRunCmdLine[0]=L'\0';
  wszRunDir[0]=L'\0';
  xstrcpynW(wszPattern, PAT_MICROSOFTGCC, BUFFER_SIZE);
  xstrcpynW(wszPatTags, PAT_MICROSOFTGCCTAG, BUFFER_SIZE);
  wszCoderAlias[0]=L'\0';
  ReadOptions(0);

  xmemset(&oe, 0, sizeof(OUTPUTEXEC));
  FillOutputDefault(&oe);
}

void InitMain()
{
  if (!nInitMain++)
  {
    //SubClass
    NewMainProcData=NULL;
    SendMessage(hMainWnd, AKD_SETMAINPROC, (WPARAM)NewMainProc, (LPARAM)&NewMainProcData);

    if (nMDI == WMD_MDI)
    {
      NewFrameProcData=NULL;
      SendMessage(hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewFrameProc, (LPARAM)&NewFrameProcData);
    }
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
  }
}

void InitWatch()
{
  bInitWatch=TRUE;

  //Set timer
  dwUpdateTimer=SetTimer(NULL, 0, dwWatchUpdateInterval, (TIMERPROC)TimerProc);
}

void UninitWatch()
{
  bInitWatch=FALSE;

  //Remove timer
  if (dwUpdateTimer)
  {
    KillTimer(NULL, dwUpdateTimer);
    dwUpdateTimer=0;
  }
  dwCurPointer=0;
  xmemset(&ftCurTime, 0, sizeof(FILETIME));
}

void StopWatch(int nStringID)
{
  UninitMain();
  UninitWatch();

  if (nStringID)
    MessageBoxW(hMainWnd, GetLangStringW(wLangModule, nStringID), wszPluginTitle, MB_OK|MB_ICONEXCLAMATION);

  if (nInitMain)
  {
    pfWatch->bRunning=FALSE;
    SendMessage(hMainWnd, WM_COMMAND, 0, 0);
  }
  else SendMessage(hMainWnd, AKD_DLLUNLOAD, (WPARAM)hInstanceDLL, (LPARAM)NULL);
}

void InitOutput()
{
  if (!nInitOutput++)
  {
    //Add hotkeys
    {
      PLUGINADDW pa={0};

      if (dwNextMatchKey && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwNextMatchKey))
      {
        pa.pFunction=L"Log::Output::NextMatch";
        pa.wHotkey=(WORD)dwNextMatchKey;
        pa.PluginProc=NextMatchProc;
        pfwNextMatch=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwPrevMatchKey && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwPrevMatchKey))
      {
        pa.pFunction=L"Log::Output::PrevMatch";
        pa.wHotkey=(WORD)dwPrevMatchKey;
        pa.PluginProc=PrevMatchProc;
        pfwPrevMatch=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwRunCmdKey && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwRunCmdKey))
      {
        pa.pFunction=L"Log::Output::RunCmd";
        pa.wHotkey=(WORD)dwRunCmdKey;
        pa.PluginProc=RunCmdProc;
        pfwRunCmd=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
      if (dwRunCmdDlgKey && !SendMessage(hMainWnd, AKD_DLLFINDW, (WPARAM)NULL, dwRunCmdDlgKey))
      {
        pa.pFunction=L"Log::Output::RunCmdDlg";
        pa.wHotkey=(WORD)dwRunCmdDlgKey;
        pa.PluginProc=RunCmdDlgProc;
        pfwRunCmdDlg=(PLUGINFUNCTION *)SendMessage(hMainWnd, AKD_DLLADDW, 0, (LPARAM)&pa);
      }
    }
  }
}

void UninitOutput()
{
  if (!--nInitOutput)
  {
    //Stop execution
    if (hExecThread)
    {
      if (OutputStop(oe.hProcess, FALSE))
        WaitForSingleObject(hExecThread, INFINITE);
    }

    //Remove hotkeys
    if (pfwNextMatch)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwNextMatch);
      pfwNextMatch=NULL;
    }
    if (pfwPrevMatch)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwPrevMatch);
      pfwPrevMatch=NULL;
    }
    if (pfwRunCmd)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwRunCmd);
      pfwRunCmd=NULL;
    }
    if (pfwRunCmdDlg)
    {
      SendMessage(hMainWnd, AKD_DLLDELETE, 0, (LPARAM)pfwRunCmdDlg);
      pfwRunCmdDlg=NULL;
    }
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
  }
  return TRUE;
}

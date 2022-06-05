/**
 *		Templates for AkelPad
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)
 *
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.
 */
#define WINVER			0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_WINDOWS	0x0410
#define _WIN32_IE		0x0600

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WindowsX.h>
#include <commctrl.h>
#include <shellapi.h>
#include <richedit.h>
#include "AkelEdit.h"
#include "AkelDLL.h"
#include "resource.h"

#define CALLCONV	_fastcall
#define MAX_PATHW	32768

//! Since 4.6.0 64-bit RichEdit Compatability
#ifdef _WIN64
#define PEM_EXGETSEL EM_EXGETSEL64
#define PEM_EXSETSEL EM_EXSETSEL64
#define PLG_CHARRANGE CHARRANGE64
#else
#define PEM_EXGETSEL EM_EXGETSEL
#define PEM_EXSETSEL EM_EXSETSEL
#define PLG_CHARRANGE CHARRANGE
#endif

static HMODULE	g_hDll;
static UINT		g_nMDI;
static BOOL		g_bLoaded;
static HWND		g_hMainWnd;
static HICON	g_hMainIcon;
static WCHAR	g_szAkelPath[2*MAX_PATHW];
static WNDPROCDATA *NewMainProcData;
static WNDPROCDATA *NewFrameProcData;
static WCHAR	g_szTemplate[2*MAX_PATHW];
static BOOL		g_bMainReady;
static UINT		g_nLock;
static BOOL		g_bInternalNew;
static HMENU	g_hMenu;
//!		Windows position
static RECT		g_rcPos;
static BOOL		g_bRemoveMarker;
static BOOL		g_bHighLightAtInsert;
static BOOL		g_bLoadHighLight;
static BOOL		g_bCPFT;

#define TEMPLATES_PATH		L"\\AkelFiles\\Plugs\\Templates"
#define TEMPLATES_MASK		L"*.*"
#define CALLCODERADJUST		L"Coder::Settings"
#define CALLCODERHIGHLIGHT	L"Coder::HighLight"
////////////////////#define CALLSESSIONCHECK	L"Sessions::Main"
////////////////////#define SESSIONS_PATH		L"\\AkelFiles\\Plugs\\Sessions.dll"
#define CODER_PATH			L"\\AkelFiles\\Plugs\\Coder.dll"
//! Default one. Currently unused
#define CARETPOSMARKER		L"}::|::{"
#define MARKER_LENGTH		7

#define DLLA_SESSIONS_ISITEMOPENING 11
#define DCP_ALLOWEDIT		0x00000001
#define DCP_ALLOWINSERT		0x00000002

#define OPEN_FUNCTION_OPEN		1
#define OPEN_FUNCTION_EDIT		2
#define OPEN_FUNCTION_INSERT	3

#define TVNT_DIR			0
#define TVNT_FILE			1
#define TVNT_EMPTY			2

#define TSF_POSITION		0x00000001
#define TSF_REMOVEMARKER	0x00000002
#define TSF_AUTOHIGHLIGHT	0x00000004
#define TSF_LOADHIGHLIGHT	0x00000008
#define TSF_CPFT			0x00000010

#define CR		L'\r'
#define LF		L'\n'
//! See Table 5.1 par 5.8 in http://www.unicode.org/versions/Unicode5.2.0/ch05.pdf
//! TODO: implement
#define NEL		0x0085
#define VT		0x000B
#define FF		0x000C
#define LS		0x2028
#define PS		0x2029

static void CALLCONV LoadPlugin(PLUGINDATA *ppd);
static void CALLCONV UnloadPlugin(PLUGINDATA *ppd);
static LPVOID CALLCONV PLUGIN_ALLOC(SIZE_T dwSize);
static VOID CALLCONV PLUGIN_FREE(LPVOID pData);
static void CALLCONV InitMain(void);
static void CALLCONV FreeMain(void);
static void CALLCONV OpenTemplate(HWND hWnd, BOOL bInsert);
static void CALLCONV EditTemplate(BOOL bCreate);
////////////////////static BOOL CALLCONV CheckSessionsPlugin(void);
static void CALLCONV FillTreeCtrl(HWND hTreeCtrl, LPCWSTR szPath, HTREEITEM hRoot);
static BOOL CALLCONV RebuildPath(HWND hTreeCtrl, HTREEITEM hItem, LPWSTR szPath);
static void CALLCONV SavePosition(HWND hWnd);
static void CALLCONV RestorePosition(HWND hWnd);
static void CALLCONV MoveButton(HWND hDlg, HWND hButton, RECT rcW, int BDX);
static void CALLCONV CallCoder(LPCWSTR szFileName);
static BOOL CALLCONV IsHighLightThemeAssigned(HWND hWnd);
static INT_PTR CALLBACK TemplateDlg(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void CALLCONV UpdateSettings(DWORD dwSettings, BOOL bLoad);
static WCHAR* CALLCONV SplitTemplate(LPWSTR szString);
static void* xmemcpy(void *dest, const void *src, UINT_PTR count);
/**
 *		DLL symbols
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		g_hDll				= hModule;
		g_bLoaded			= FALSE;
		g_hMainWnd			= NULL;
		NewMainProcData		= NULL;
		NewFrameProcData	= NULL;
		g_bMainReady		= FALSE;
		g_nLock				= 0;
		g_bInternalNew		= TRUE;
		g_hMenu				= 0;
		g_bRemoveMarker		= FALSE;
		DisableThreadLibraryCalls(g_hDll);
	}
	else if(dwReason == DLL_PROCESS_DETACH)
	{
		UnloadPlugin(NULL);
	}
	return TRUE;
}
extern void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
	pv->dwAkelDllVersion=AKELDLL;
	pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1,-1,-1,-1);
	pv->dwExeMinVersion4x=MAKE_IDENTIFIER( 4, 8, 8, 0);
	pv->pPluginName="Templates";
}
extern void __declspec(dllexport) Main(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;
	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}

	if(!g_bLoaded)
	{
		LoadPlugin(pd);
		if(!g_bLoaded)
		{
			pd->nUnload = UD_UNLOAD;
			return;
		}
	}
	if(g_bMainReady)
	{
		FreeMain();
	}
	else
	{
		InitMain();
		if(!pd->bOnStart)
		{
			g_bInternalNew = FALSE;
		}
	}
	pd->nUnload = (g_bMainReady)?
		(UD_NONUNLOAD_ACTIVE):
		(g_nLock?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD);
	if(!g_nLock) UnloadPlugin(pd);
}
extern void __declspec(dllexport) Open(PLUGINDATA *pd)
{
	INT_PTR res;
	DWORD_PTR* param;
	EDITINFO ei;
	BOOL bInsert = FALSE;
	pd->dwSupport |= PDS_NOANSI|PDS_NOAUTOLOAD;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;
	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	if(!g_bLoaded)
	{
		LoadPlugin(pd);
		if(!g_bLoaded)
		{
			pd->nUnload = UD_UNLOAD;
			return;
		}
	}
	g_nLock++;
	ei.hWndEdit = NULL;
	if(SendMessageW(g_hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei))
	{
		if(ei.hWndEdit && ei.wszFile)
			bInsert = !ei.bReadOnly;
	}
	param = (DWORD_PTR*)pd->lParam;
	if(	param
		&& (param[0]==3*sizeof(DWORD_PTR))
		&& (param[1]==OPEN_FUNCTION_OPEN
			|| param[1]==OPEN_FUNCTION_EDIT
			|| param[1]==OPEN_FUNCTION_INSERT)
		&& param[2])
	{
		LPCWSTR p = (LPCWSTR)param[2];
		res = (param[1]==OPEN_FUNCTION_OPEN)
					?(IDOK)
					:((param[1]==OPEN_FUNCTION_EDIT)
							?IDC_EDIT:
							IDC_INSERT);
		if(p[0])
		{
			lstrcpyW(g_szTemplate,g_szAkelPath);
			lstrcatW(g_szTemplate,TEMPLATES_PATH);
			lstrcatW(g_szTemplate,L"\\");
			lstrcatW(g_szTemplate,p);
		}
		else g_szTemplate[0] = 0;
		if(res==IDC_EDIT)
		{
			HANDLE h = CreateFileW(g_szTemplate,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
			if(h == INVALID_HANDLE_VALUE)
			{
				res = IDCANCEL;
			}
			else
			{
				CloseHandle(h);
			}
		}
		else if((res == IDC_INSERT) && (!bInsert))
		{
			res = IDCANCEL;
		}
	}
	else
	{
		res = DialogBoxParamW(g_hDll,MAKEINTRESOURCEW(IDD_TEMPLATE),g_hMainWnd,(DLGPROC)TemplateDlg,(LPARAM)DCP_ALLOWEDIT|(bInsert?DCP_ALLOWINSERT:0));
	}
	if(IDOK == res || IDC_EDIT == res || IDC_INSERT == res)
	{
		if(g_bMainReady)
		{
			g_bInternalNew = TRUE;
		}
		if(res == IDC_EDIT)
		{
			EditTemplate(FALSE);
		}
		else if(res == IDOK)
		{
			ei.hWndEdit = NULL;
			if(SendMessageW(g_hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei))
			{
				if(!ei.hWndEdit || ei.bModified || ei.wszFile[0])
				{
					if(SendMessageW(g_hMainWnd,WM_COMMAND,MAKEWPARAM(IDM_FILE_NEW,0),TRUE))
					{
						SendMessageW(g_hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei);
					}
				}
			}
			else if(g_nMDI == WMD_MDI)
			{
				if(SendMessageW(g_hMainWnd,WM_COMMAND,MAKEWPARAM(IDM_FILE_NEW,0),TRUE))
				{
					SendMessageW(g_hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei);
				}
			}
			if(ei.hWndEdit && !((g_nMDI == WMD_SDI)&&(ei.bModified)))
			{
				OpenTemplate(ei.hWndEdit,FALSE);
			}
		}
		else if(res == IDC_INSERT)
		{
			if(SendMessageW(g_hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei))
			{
				if(ei.hWndEdit)
				{
					OpenTemplate(ei.hWndEdit,TRUE);
				}
			}
		}
		if(g_bMainReady)
		{
			g_bInternalNew = FALSE;
		}
	}
	g_nLock--;
	pd->nUnload = (g_nLock)?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
	if(!g_nLock) UnloadPlugin(pd);
}
/**
 *		Helpers
 */
static void CALLCONV LoadPlugin(PLUGINDATA *ppd)
{
	if(!g_bLoaded)
	{
		g_hMainWnd = ppd->hMainWnd;
		g_nMDI     = ppd->nMDI;
		g_hMenu    = ppd->hMainMenu;
		g_hMainIcon = ppd->hMainIcon;
		lstrcpyW(g_szAkelPath,(WCHAR*)ppd->wszAkelDir);
		g_rcPos.top = g_rcPos.bottom = g_rcPos.bottom = g_rcPos.bottom = -1;
		g_bHighLightAtInsert = FALSE;
		g_bLoadHighLight = FALSE;
		g_bCPFT = FALSE;
		UpdateSettings(TSF_POSITION|TSF_AUTOHIGHLIGHT|TSF_LOADHIGHLIGHT|TSF_REMOVEMARKER|TSF_CPFT,TRUE);
		g_bLoaded = TRUE;
	}
}
static void CALLCONV UnloadPlugin(PLUGINDATA *ppd)
{
	FreeMain();
	g_bLoaded = FALSE;
}
static LPVOID CALLCONV PLUGIN_ALLOC(SIZE_T dwSize)
{
	HANDLE hHeap = GetProcessHeap();
	return HeapAlloc(hHeap,HEAP_ZERO_MEMORY,dwSize);
}
static VOID CALLCONV PLUGIN_FREE(LPVOID pData)
{
	if(pData)
	{
		HANDLE hHeap = GetProcessHeap();
		HeapFree(hHeap,0,pData);
	}
}
static void CALLCONV UpdateSettings(DWORD dwSettings, BOOL bLoad)
{
	PLUGINOPTIONW po;
	HANDLE hOpt = (HANDLE)
	SendMessageW(g_hMainWnd,AKD_BEGINOPTIONSW,bLoad?POB_READ:POB_SAVE,(LPARAM)L"Templates");
	if(hOpt)
	{
		if(dwSettings & TSF_POSITION)
		{
			po.pOptionName = L"POSITION";
			po.dwType = PO_BINARY;
			po.dwData = sizeof(RECT);
			po.lpData = (LPBYTE)&g_rcPos;
			SendMessageW(g_hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		if(dwSettings & TSF_REMOVEMARKER)
		{
			po.pOptionName = L"RemoveMarker";
			po.dwType = PO_DWORD;
			po.dwData = sizeof(BOOL);
			po.lpData = (LPBYTE)&g_bRemoveMarker;
			SendMessageW(g_hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		if(dwSettings & TSF_AUTOHIGHLIGHT)
		{
			po.pOptionName = L"AutoHighLight";
			po.dwType = PO_DWORD;
			po.dwData = sizeof(BOOL);
			po.lpData = (LPBYTE)&g_bHighLightAtInsert;
			SendMessageW(g_hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		if(dwSettings & TSF_LOADHIGHLIGHT)
		{
			po.pOptionName = L"LoadHighLight";
			po.dwType = PO_DWORD;
			po.dwData = sizeof(BOOL);
			po.lpData = (LPBYTE)&g_bLoadHighLight;
			SendMessageW(g_hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		if(dwSettings & TSF_CPFT)
		{
			po.pOptionName = L"CPFT";
			po.dwType = PO_DWORD;
			po.dwData = sizeof(BOOL);
			po.lpData = (LPBYTE)&g_bCPFT;
			SendMessageW(g_hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		SendMessageW(g_hMainWnd,AKD_ENDOPTIONS,(WPARAM)hOpt,0);
	}
}
static void CALLCONV FillTreeCtrl(HWND hTreeCtrl, LPCWSTR szPath, HTREEITEM hRoot)
{
	SIZE_T i, nPathLen = lstrlenW(szPath);
	WIN32_FIND_DATAW fd;
	HANDLE hFind;
	LPWSTR szMask = PLUGIN_ALLOC(MAX_PATH*sizeof(WCHAR));
	if(!szMask) return;
	lstrcpyW(szMask,szPath);
	lstrcatW(szMask,L"\\");
	lstrcatW(szMask,TEMPLATES_MASK);
	hFind = FindFirstFileW(szMask,&fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		LPWSTR szItem = PLUGIN_ALLOC(MAX_PATH*sizeof(szItem));
		LPCWSTR p = szPath;
		UINT_PTR tail = 0;
		SHFILEINFOW sfi,sfis;
		while(*p){ szItem[tail] = *p++;tail++;};
		szItem[tail]=L'\\';tail++;
		do
		{
			if(fd.cFileName[0] != L'.')
			{
				i=0;
				while(fd.cFileName[i]) {szItem[tail+i]=fd.cFileName[i];i++;}
				szItem[tail+i]=0;
				SHGetFileInfoW(szItem,0,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON);
				SHGetFileInfoW(szItem,0,&sfis,sizeof(sfis),SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_OPENICON);
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					TVINSERTSTRUCTW tvi;
					HTREEITEM hNode=NULL;
					WCHAR szChild[MAX_PATH],szLabel[MAX_PATH];
					p = fd.cFileName;
					tvi.hParent = hRoot;
					tvi.hInsertAfter = TVI_LAST;
					szLabel[0]=L'[';
					i=0;
					while(*p) {szLabel[1+i]=*p++;i++;}
					szLabel[1+i]=L']';
					szLabel[1+i+1]=0;
					tvi.item.pszText = szLabel;
					tvi.item.cchTextMax = MAX_PATH;
					tvi.item.lParam = TVNT_DIR;
					tvi.item.iImage = sfi.iIcon;
					tvi.item.iSelectedImage = sfis.iIcon;
					tvi.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
					hNode = TreeView_InsertItem(hTreeCtrl,&tvi);
					//! Чт?бы не "оптимизировало" ?вызо?memcpy пришлось установить опци?
					//! компилятора "Inline Function Expansion" ?/Ob1
					//! На значение по-умолчани?"Default" ?Win32 вс?нормальн? ?x64 - вылази?memcpy
					xmemcpy(szChild,szPath,nPathLen*sizeof(WCHAR));
					i = nPathLen;
					szChild[i]=L'\\'; i++;
					p = fd.cFileName;
					while(*p) {szChild[i]=*p++;i++;}
					szChild[i]=0;
					FillTreeCtrl(hTreeCtrl,szChild,hNode);
					TreeView_SortChildren(hTreeCtrl,hNode,FALSE);
					TreeView_Expand(hTreeCtrl,hNode,TVE_EXPAND);
				}
				else
				{
					TVINSERTSTRUCTW tvi;
					tvi.hParent = hRoot;
					tvi.hInsertAfter = TVI_LAST;
					tvi.item.pszText = fd.cFileName;
					tvi.item.cchTextMax = MAX_PATH;
					tvi.item.lParam = TVNT_FILE;
					tvi.item.iImage = sfi.iIcon;
					tvi.item.iSelectedImage = sfis.iIcon;
					tvi.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
					TreeView_InsertItem(hTreeCtrl,&tvi);
				}
			}
		}
		while(FindNextFileW(hFind, &fd));
		FindClose(hFind);
		PLUGIN_FREE(szItem);
	}
	PLUGIN_FREE(szMask);
}
static BOOL CALLCONV RebuildPath(HWND hTreeCtrl, HTREEITEM hItem, LPWSTR szPath)
{
	WCHAR name[MAX_PATH];
	TVITEMW item;
	HTREEITEM hParent;
	name[0]=0;
	item.hItem = hItem;
	item.cchTextMax = MAX_PATH-1;
	item.pszText = name;
	item.mask = TVIF_TEXT|TVIF_PARAM;
	TreeView_GetItem(hTreeCtrl,&item);
	hParent = TreeView_GetParent(hTreeCtrl,hItem);
	if(hParent)
	{
		RebuildPath(hTreeCtrl, hParent, szPath);
		lstrcatW(szPath,L"\\");
	}
	if(item.lParam == TVNT_DIR)
	{
		lstrcatW(szPath,item.pszText+1);
		szPath[lstrlenW(szPath)-1]=0;
		return TRUE;
	}
	else if(item.lParam == TVNT_EMPTY)
	{
		szPath[0] = 0;
		return TRUE;
	}
	else
	{
		lstrcatW(szPath,item.pszText);
		return TRUE;
	}
}
static void CALLCONV SavePosition(HWND hWnd)
{
	GetWindowRect(hWnd,&g_rcPos);
	UpdateSettings(TSF_POSITION,FALSE);
}
static void CALLCONV RestorePosition(HWND hWnd)
{
	UpdateSettings(TSF_POSITION,TRUE);
	if(g_rcPos.top != -1)
	{
		if(g_rcPos.right >= GetSystemMetrics(SM_CXSCREEN) ||
			g_rcPos.top >= GetSystemMetrics(SM_CYSCREEN)) return;
		MoveWindow(hWnd,g_rcPos.left,g_rcPos.top,g_rcPos.right-g_rcPos.left,g_rcPos.bottom-g_rcPos.top,TRUE);
	}
}
static void CALLCONV MoveButton(HWND hDlg, HWND hButton, RECT rcW, int BDX)
{
	RECT rcNew;
	POINT pt;
	if(!hButton) return;
	GetWindowRect(hButton,&rcNew);
	pt.x = rcNew.left; pt.y = rcNew.top;
	ScreenToClient(hDlg,&pt);
	MoveWindow(hButton,
		rcW.right-BDX,
		pt.y,
		rcNew.right - rcNew.left,
		rcNew.bottom-rcNew.top,TRUE);
}
static INT_PTR CALLBACK TemplateDlg(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hTreeCtrl;
	static HWND hOkBtn;
	static HWND hEditBtn;
	static HWND hInsertBtn;
	static HWND hRemoveMarkerBtn;
	static int LDX, LDY, BDX;
	static LPARAM param;
	if(WM_INITDIALOG == uMsg)
	{
		RECT rcWR, rcCR;
		POINT pt;
		EDITINFO ei;
		param = lParam;
		GetClientRect(hWndDlg,&rcWR);
		hTreeCtrl = GetDlgItem(hWndDlg,IDC_TREE);
		hOkBtn = GetDlgItem(hWndDlg,IDOK);
		hEditBtn = GetDlgItem(hWndDlg,IDC_EDIT);
		hInsertBtn = GetDlgItem(hWndDlg,IDC_INSERT);
		hRemoveMarkerBtn = GetDlgItem(hWndDlg, IDC_REMMARKER);
		GetWindowRect(hTreeCtrl,&rcCR);
		pt.x = rcCR.right; pt.y = rcCR.bottom;
		ScreenToClient(hWndDlg,&pt);
		LDX = rcWR.right - pt.x;
		LDY = rcWR.bottom - pt.y;
		GetWindowRect(hOkBtn,&rcCR);
		pt.x=rcCR.left; pt.y=0;
		ScreenToClient(hWndDlg,&pt);
		BDX = rcWR.right - pt.x;
		{
			TVINSERTSTRUCTW tvi;
			HTREEITEM hEmpty;
			WCHAR	szRoot[MAX_PATH];
			SHFILEINFOW sfi;
			HIMAGELIST hList
				= (HIMAGELIST)SHGetFileInfoW(g_szAkelPath,0,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
			TreeView_SetImageList(hTreeCtrl,hList,TVSIL_NORMAL);
			lstrcpyW(szRoot,g_szAkelPath);
			lstrcatW(szRoot,TEMPLATES_PATH);
			FillTreeCtrl(hTreeCtrl,szRoot,TVI_ROOT);
			TreeView_SortChildren(hTreeCtrl,TVI_ROOT,FALSE);
			tvi.hParent = TVI_ROOT;
			tvi.hInsertAfter = TVI_FIRST;
			tvi.item.pszText = L"<Empty>";
			tvi.item.lParam = TVNT_EMPTY;
			tvi.item.iImage = tvi.item.iSelectedImage = 3;
			tvi.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			hEmpty = TreeView_InsertItem(hTreeCtrl,&tvi);
			if(TreeView_GetCount(hTreeCtrl)>0)
			{
				TreeView_SelectItem(hTreeCtrl,hEmpty);
				EnableWindow(hOkBtn,TRUE);
			}
			else
			{
				EnableWindow(hOkBtn,FALSE);
			}
		}
		if(!(lParam&DCP_ALLOWEDIT))
		{
			DestroyWindow(hEditBtn);
			hEditBtn = NULL;
		}
		if(SendMessageW(g_hMainWnd,AKD_GETEDITINFO,(WPARAM)NULL,(LPARAM)&ei))
		{
			if(!IsWindow(ei.hWndEdit) || (!(lParam&DCP_ALLOWINSERT)))
			{
				DestroyWindow(hInsertBtn);
				hInsertBtn = NULL;
			}
		}
		SendMessageW(hRemoveMarkerBtn, BM_SETCHECK,g_bRemoveMarker?BST_CHECKED:BST_UNCHECKED,0);
		PostMessageW(hWndDlg,WM_COMMAND,MAKEWPARAM(IDC_TREE,LBN_SELCHANGE),0);
		RestorePosition(hWndDlg);
#ifndef NO_SMALL_ICON
		SendMessageW(hWndDlg, WM_SETICON,(WPARAM)ICON_SMALL,(LPARAM)g_hMainIcon);
#endif
		SendMessageW(hWndDlg, WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)g_hMainIcon);
		return TRUE;
	}
	else if(WM_COMMAND == uMsg)
	{
		if(((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDC_EDIT) || (LOWORD(wParam) == IDC_INSERT)) && (HIWORD(wParam) == BN_CLICKED))
		{
			HTREEITEM hItem = TreeView_GetSelection(hTreeCtrl);
			TVITEM item;
			INT_PTR len = 0;
			if(!hItem) return TRUE;
			item.hItem = hItem;
			item.lParam = 0;
			item.mask = TVIF_PARAM;
			if(!TreeView_GetItem(hTreeCtrl,&item)) return TRUE;
			if(item.lParam==TVNT_DIR) return TRUE;
			lstrcpyW(g_szTemplate,g_szAkelPath);
			lstrcatW(g_szTemplate,TEMPLATES_PATH);
			lstrcatW(g_szTemplate,L"\\");
			g_bRemoveMarker = (SendMessageW(hRemoveMarkerBtn, BM_GETCHECK, 0, 0)==BST_CHECKED)?TRUE:FALSE;
			UpdateSettings(TSF_REMOVEMARKER,FALSE);
			len = lstrlenW(g_szTemplate);
			if(RebuildPath(hTreeCtrl, hItem, g_szTemplate+len))
			{
				if(lstrlenW(g_szTemplate) == len)
				{
					//! Opening empty template
					g_szTemplate[0] = 0;
				}
				EndDialog(hWndDlg,LOWORD(wParam));
			}
			else
			{
				g_szTemplate[0]=0;
			}
			return TRUE;
		}
		else if((LOWORD(wParam)==IDCANCEL) && (HIWORD(wParam) == BN_CLICKED))
		{
			EndDialog(hWndDlg,IDCANCEL);
			return TRUE;
		}
	}
	else if(WM_NOTIFY == uMsg)
	{
		if(IDC_TREE == wParam)
		{
			if(((LPNMHDR)lParam)->code == TVN_SELCHANGED)
			{
				LPNMTREEVIEWW ptv = (LPNMTREEVIEWW)lParam;
				if(ptv->itemNew.hItem && (ptv->itemNew.mask&TVIF_PARAM))
				{
					if(hOkBtn)
					{
						EnableWindow(hOkBtn, (ptv->itemNew.lParam != TVNT_DIR));
					}
					if(hEditBtn)
					{
						EnableWindow(hEditBtn, ptv->itemNew.lParam == TVNT_FILE);
					}
					if(hInsertBtn)
					{
						EnableWindow(hInsertBtn, ptv->itemNew.lParam == TVNT_FILE);
					}
				}
				return TRUE;
			}
			else if(((LPNMHDR)lParam)->code == NM_DBLCLK)
			{
				DWORD pos = GetMessagePos();
				TVHITTESTINFO thi;
				HTREEITEM hit=0;
				thi.pt.x = GET_X_LPARAM(pos);
				thi.pt.y = GET_Y_LPARAM(pos);
				ScreenToClient(hTreeCtrl,&thi.pt);
				thi.hItem =0;
				thi.flags =0;
				hit = TreeView_HitTest(hTreeCtrl,&thi);
				if(thi.hItem)
				{
					TVITEM item;
					item.hItem = thi.hItem;
					item.mask = TVIF_PARAM;
					TreeView_GetItem(hTreeCtrl,&item);
					if(hOkBtn)
					{
						EnableWindow(hOkBtn, item.lParam != TVNT_DIR);
					}
					if(hEditBtn)
					{
						EnableWindow(hEditBtn, item.lParam == TVNT_FILE);
					}
					if(hInsertBtn)
					{
						EnableWindow(hInsertBtn, item.lParam != TVNT_DIR);
					}
					if(item.lParam != TVNT_DIR)
						PostMessageW(hWndDlg,WM_COMMAND,MAKEWPARAM(IDOK,BN_CLICKED),0);
				}
				return TRUE;
			}
		}
	}
	else if(WM_CLOSE == uMsg)
	{
		PostMessageW(hWndDlg,WM_COMMAND,MAKEWPARAM(IDCANCEL,BN_CLICKED),0);
		return TRUE;
	}
	else if(WM_DESTROY == uMsg)
	{
		SavePosition(hWndDlg);
	}
	else if(WM_SIZE == uMsg)
	{
		RECT rcWindow, rcNew; POINT pt;
		GetClientRect(hWndDlg,&rcWindow);
		GetWindowRect(hTreeCtrl,&rcNew);
		pt.x = rcNew.left; pt.y = rcNew.top;
		ScreenToClient(hWndDlg,&pt);
		MoveWindow(hTreeCtrl,pt.x,pt.y,rcWindow.right-LDX-pt.x, rcWindow.bottom-LDY-pt.y,TRUE);
		MoveButton(hWndDlg,hOkBtn,rcWindow, BDX);
		MoveButton(hWndDlg,GetDlgItem(hWndDlg,IDCANCEL),rcWindow, BDX);
		MoveButton(hWndDlg,hEditBtn,rcWindow, BDX);
		MoveButton(hWndDlg,hInsertBtn,rcWindow, BDX);
		MoveButton(hWndDlg,hRemoveMarkerBtn,rcWindow,BDX);
		InvalidateRect(hWndDlg,NULL,TRUE);
	}
	else if(WM_PAINT == uMsg)
	{
		RECT rc;
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hWndDlg, &ps);
		if(dc)
		{
			GetClientRect(hWndDlg, &rc);
			rc.left = rc.right - GetSystemMetrics(SM_CXVSCROLL);
			rc.top  = rc.bottom - GetSystemMetrics(SM_CYVSCROLL);
			DrawFrameControl(dc, &rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
			EndPaint(hWndDlg, &ps);
		}
	}
	return FALSE;
}
static LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((uMsg == WM_COMMAND) && (LOWORD(wParam) == IDM_FILE_NEW))
	{
		if((!g_bInternalNew) && (lParam!=TRUE))
		{
			if(IDOK == DialogBoxParamW(g_hDll,MAKEINTRESOURCEW(IDD_TEMPLATE),g_hMainWnd,(DLGPROC)TemplateDlg,(LPARAM)0))
			{
				LRESULT bRet = FALSE;
				if (hWnd == g_hMainWnd)
					bRet = NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
				else
					bRet = NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
				if(bRet)
				{
					EDITINFO ei;
					if(SendMessageW(g_hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei))
					{
						OpenTemplate(ei.hWndEdit,FALSE);
					}
				}
				return bRet;
			}
			else
			{
				if (hWnd == g_hMainWnd)
					return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
				else
					return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
			}
		}
	}
	else if(uMsg == AKDN_MAIN_ONFINISH)
	{
		LRESULT lRes = 0;
		if (hWnd == g_hMainWnd)
			lRes = NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
		else
			lRes = NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
		FreeMain();
		return lRes;
	}
	else if(uMsg == AKDN_MAIN_ONSTART_FINISH)
	{
		g_bInternalNew = FALSE;
	}
	if (hWnd == g_hMainWnd)
		return NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
	else
		return NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}
static void CALLCONV InitMain(void)
{
	if(!g_bMainReady)
	{
		NewMainProcData = NULL;
		SendMessageW(g_hMainWnd, AKD_SETMAINPROC, (WPARAM)NewEditParentProc, (LPARAM)&NewMainProcData);
		if (g_nMDI == WMD_MDI)
		{
			NewFrameProcData = NULL;
			SendMessageW(g_hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewEditParentProc, (LPARAM)&NewFrameProcData);
		}
		g_bMainReady = TRUE;
		g_nLock++;
	}
}
static void CALLCONV FreeMain(void)
{
	if(g_bMainReady)
	{
		if (NewMainProcData)
		{
			SendMessageW(g_hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&NewMainProcData);
			NewMainProcData=NULL;
		}
		if (g_nMDI == WMD_MDI)
		{
			if (NewFrameProcData)
			{
				SendMessageW(g_hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&NewFrameProcData);
				NewFrameProcData=NULL;
			}
		}
		g_bMainReady = FALSE;
		g_nLock--;
	}
}
////////////////////static BOOL CALLCONV CheckSessionsPlugin(void)
////////////////////{
////////////////////	WCHAR szSessions[MAX_PATH];
////////////////////	WIN32_FIND_DATAW fd;
////////////////////	HANDLE hFind;
////////////////////	PLUGINFUNCTION* pf=NULL;
////////////////////	lstrcpyW(szSessions,g_szAkelPath);
////////////////////	lstrcatW(szSessions,SESSIONS_PATH);
////////////////////	hFind = FindFirstFileW(szSessions,&fd);
////////////////////	if((hFind == INVALID_HANDLE_VALUE) || (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
////////////////////	{
////////////////////		FindClose(hFind);
////////////////////		return FALSE;
////////////////////	}
////////////////////	FindClose(hFind);
////////////////////	{
////////////////////		if((pf=(PLUGINFUNCTION *)SendMessageW(g_hMainWnd, AKD_DLLFINDW, (WPARAM)CALLSESSIONCHECK, 0))
////////////////////			&& (pf->bRunning))
////////////////////		{
////////////////////			volatile BOOL bSessionAction=FALSE;
////////////////////			DWORD_PTR param[3]={sizeof(param),DLLA_SESSIONS_ISITEMOPENING};
////////////////////			PLUGINCALLSENDW pcs;
////////////////////			param[2] = (DWORD_PTR)&bSessionAction;
////////////////////			pcs.dwSupport = 0;
////////////////////			pcs.lParam = (LPARAM)param;
////////////////////			pcs.pFunction = CALLSESSIONCHECK;
////////////////////			SendMessageW(g_hMainWnd,AKD_DLLCALLW,0,(LPARAM)&pcs);
////////////////////			return bSessionAction;
////////////////////		}
////////////////////		else
////////////////////			return FALSE;
////////////////////	}
////////////////////}
static void CALLCONV CallCoder(LPCWSTR szFileName)
{
	WCHAR szCoder[MAX_PATH];
	WIN32_FIND_DATAW fd;
	HANDLE hFind;
	PLUGINFUNCTION* pf = NULL;
	lstrcpyW(szCoder,g_szAkelPath);
	lstrcatW(szCoder,CODER_PATH);
	hFind = FindFirstFileW(szCoder, &fd);
	if((hFind == INVALID_HANDLE_VALUE) || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		FindClose(hFind);
		return;
	}
	FindClose(hFind);
	pf=(PLUGINFUNCTION*)SendMessageW(g_hMainWnd,AKD_DLLFINDW,(WPARAM)CALLCODERHIGHLIGHT,0);
	if(!pf || !pf->bRunning)
	{
		if(g_bLoadHighLight)
		{
			PLUGINCALLSENDW pcs;
			pcs.lParam = pcs.dwSupport = 0;
			pcs.pFunction = CALLCODERHIGHLIGHT;
			SendMessageW(g_hMainWnd,AKD_DLLCALLW,0,(LPARAM)&pcs);
		}
		else return;
	}
	{
		PLUGINCALLSENDW pcs;
		UINT filelen = lstrlenW(szFileName);
		DWORD_PTR param[3] = {sizeof(param),1};
		LPCWSTR dot = szFileName+filelen;
		param[2] = (DWORD_PTR)dot;
		while(szFileName < dot)
		{
			if(*dot==L'.') break;
			dot--;
		}
		if(szFileName < dot)
			param[2] = (DWORD_PTR)(dot+1);
		pcs.dwSupport = 0;
		pcs.lParam = (LPARAM)param;
		pcs.pFunction = CALLCODERADJUST;
		SendMessageW(g_hMainWnd,AKD_DLLCALLW,0,(LPARAM)&pcs);
	}
}
static BOOL CALLCONV IsHighLightThemeAssigned(HWND hWnd)
{
	AEHTHEME hTheme = NULL;
	LRESULT size = 0;
	WCHAR *name = NULL;
	if(!IsWindow(hWnd)) return FALSE;
	hTheme = (AEHTHEME)SendMessageW(hWnd,AEM_HLGETTHEMENAMEW,0,0);
	if(!hTheme) return FALSE;
	name = (WCHAR*)PLUGIN_ALLOC(sizeof(WCHAR)*4096);
	size = SendMessageW(hWnd,AEM_HLGETTHEMENAMEW,(WPARAM)hTheme,(LPARAM)name);
	PLUGIN_FREE(name);
	return size?TRUE:FALSE;
}
static void CALLCONV OpenTemplate(HWND hWnd, BOOL bInsert)
{
	LRESULT			res = 0;
	LRESULT			pos = 0;
	TEXTFINDW		tf = {FRF_BEGINNING|FRF_DOWN|FRF_MATCHCASE,CARETPOSMARKER,-1};
	TEXTREPLACEW	tr = {FRF_BEGINNING|FRF_DOWN|FRF_MATCHCASE,CARETPOSMARKER,-1,L"",0,TRUE,0};
	DETECTFILEW	dc = {g_szTemplate,1024,ADT_BINARYERROR|ADT_DETECTBOM|ADT_DETECTCODEPAGE,0,FALSE};
	if(g_szTemplate[0]/*!<EMPTY>*/ && (EDT_SUCCESS == SendMessageW(g_hMainWnd, AKD_DETECTFILEW,0,(LPARAM)&dc)))
	{
		HANDLE hFile = CreateFileW(g_szTemplate, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
		if(hFile != INVALID_HANDLE_VALUE/*always TRUE if we are here*/)
		{
			FILECONTENT fc;
			LRESULT lSize = 0;
			PLG_CHARRANGE cursel = {0,0};
			fc.hFile = hFile;
			fc.dwMax = (UINT_PTR)-1;
			fc.nCodePage = dc.nCodePage;
			fc.bBOM = dc.bBOM;
			fc.wpContent = NULL;
			if(lSize = SendMessageW(g_hMainWnd,AKD_READFILECONTENT,0,(LPARAM)&fc))
			{
				LPCWSTR pText = SplitTemplate(fc.wpContent);
				tf.pFindIt = fc.wpContent;
				tr.pFindIt = fc.wpContent;
				lSize -= pText - fc.wpContent;
				if(*pText)
				{
					if(!bInsert)
					{
						SendMessageW(hWnd,EM_SETSEL,(WPARAM)0,(LPARAM)-1);
					}
					else if(g_bRemoveMarker)
					{
						//! Store selection for future
						SendMessageW(hWnd, PEM_EXGETSEL,0,(LPARAM)&cursel);
					}
					//! Insert text
					SendMessageW(hWnd,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)pText);
					if(!bInsert)
					{
						//! Find first cursor position
						res = SendMessageW(g_hMainWnd, AKD_TEXTFINDW, (WPARAM)hWnd, (LPARAM)&tf);
						if(res != -1) pos = res;
						if(g_bRemoveMarker)
						{
							//! Remove all caret markers
							SendMessageW(g_hMainWnd, AKD_TEXTREPLACEW, (WPARAM)hWnd, (LPARAM)&tr);
						}
						//! Put caret there
						SendMessageW(hWnd,EM_SETSEL,(WPARAM)pos,(LPARAM)pos);
						if(g_bCPFT)
						{
							SAVEDOCUMENTW sd = {L"",fc.nCodePage,fc.bBOM,SD_UPDATE};
							SendMessageW(g_hMainWnd,AKD_SAVEDOCUMENTW,(WPARAM)hWnd,(LPARAM)&sd);
						}
						//! Reset modification flag
						SendMessageW(g_hMainWnd, AKD_SETMODIFY,(WPARAM)hWnd,(LPARAM)FALSE);
					}
					else if(bInsert && g_bRemoveMarker)
					//! User requested to remove markers
					{
						//! Mark newly inserted text
						cursel.cpMax = cursel.cpMin + lSize;
						SendMessageW(hWnd,PEM_EXSETSEL,0,(LPARAM)&cursel);
						tr.dwFindFlags = FRF_SELECTION|FRF_DOWN|FRF_MATCHCASE;
						//! Remove all markers
						SendMessageW(g_hMainWnd,AKD_TEXTREPLACEW,(WPARAM)hWnd,(LPARAM)&tr);
						//! Put caret after inserted text
						SendMessageW(hWnd, PEM_EXGETSEL,0,(LPARAM)&cursel);
						cursel.cpMin = cursel.cpMax;
						SendMessageW(hWnd, PEM_EXSETSEL,0,(LPARAM)&cursel);
					}
				}
				else
				{
					//! Empty template - do "nothing" i.e. only insert text, do not try to deal with markers
					SendMessageW(hWnd,EM_REPLACESEL,(WPARAM)FALSE,(LPARAM)pText);
				}
				//! Call Coder only on creation and on insertion if no theme assigned yet
				if((!bInsert) || (g_bHighLightAtInsert && IsHighLightThemeAssigned(hWnd)))
				{
					CallCoder(g_szTemplate);
				}
				SendMessageW(g_hMainWnd, AKD_FREETEXT,0,(LPARAM)fc.wpContent);
				if(bInsert)
				{
					SendMessageW(g_hMainWnd, AKD_SETMODIFY,(WPARAM)hWnd,(LPARAM)TRUE);
				}
			}
			CloseHandle(hFile);
		}
	}
}
//! Completely borrowed from sessions plugin(adapted)
static void CALLCONV EditTemplate(BOOL bCreate)
{
	FRAMEDATA *lpFrameActivate=NULL;
	EDITINFO ei;
	OPENDOCUMENTW od;
	LRESULT lCmdLineOptions;

	if (SendMessageW(g_hMainWnd, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
	{

		if (GetMenuState(g_hMenu, IDM_OPTIONS_SINGLEOPEN_FILE, MF_BYCOMMAND) & MF_CHECKED)
		{
			if (lpFrameActivate=(FRAMEDATA *)SendMessageW(g_hMainWnd, AKD_FRAMEFINDW, FWF_BYFILENAME, (LPARAM)g_szTemplate))
			{
				if (lpFrameActivate) SendMessageW(g_hMainWnd, AKD_FRAMEACTIVATE, 0, (LPARAM)lpFrameActivate);
				return;
			}
		}

		//Create empty document
		if (!ei.hWndEdit || ei.bModified || *ei.wszFile)
		{
			SendMessageW(g_hMainWnd, WM_COMMAND, MAKEWPARAM(IDM_FILE_NEW,0), TRUE);
		}
	}
	else SendMessageW(g_hMainWnd, WM_COMMAND, MAKEWPARAM(IDM_FILE_NEW,0), TRUE);

	//Open file
	lCmdLineOptions = SendMessageW(g_hMainWnd, AKD_GETCMDLINEOPTIONS, 0, 0);
	SendMessageW(g_hMainWnd, AKD_SETCMDLINEOPTIONS, (bCreate?CLO_MSGOPENCREATEYES:CLO_MSGOPENCREATENO)|CLO_MSGOPENBINARYNO, 0);
	od.pFile     = g_szTemplate;
	od.pWorkDir  = NULL;
	od.dwFlags   = OD_ADT_DETECTBOM|OD_ADT_DETECTCODEPAGE;
	od.nCodePage = 0;
	od.bBOM      = 0;
	SendMessageW(g_hMainWnd, AKD_OPENDOCUMENTW, (WPARAM)NULL, (LPARAM)&od);
	SendMessageW(g_hMainWnd, AKD_SETCMDLINEOPTIONS, lCmdLineOptions, 0);
	CallCoder(g_szTemplate);
}
//! Seek for first line end, marks it zero, returns pointer to second line
static WCHAR* CALLCONV SplitTemplate(LPWSTR szString)
{
	WCHAR* result = szString;
	while(*result)
	{
		if(!*result)
		{
			return result;
		}
		if(*result == LF)//LF
		{
			*result = 0;
			return result+1;
		}
		if(*result == CR)//CR
		{
			*result = 0;
			if(*(result+1) == LF)//CRLF
			{
				return result+2;
			}
			return result+1;
		}
		result++;
	}
	return result;
}
void* xmemcpy(void *dest, const void *src, UINT_PTR count)
{
	unsigned char *byte_dest=(unsigned char *)dest;
	unsigned char *byte_src=(unsigned char *)src;

	//Special form of memcpy implementation to avoid
	//compiler from replace this code with memcpy call.
	if (byte_dest != byte_src)
	{
		if (count)
		{
			for (;;)
			{
				*byte_dest=*byte_src;
				if (!--count) break;
				++byte_dest;
				++byte_src;
			}
		}
	}
	return dest;
}

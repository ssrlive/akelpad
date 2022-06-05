/**                                                             
 *		Speech for AkelPad                                      
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)    
 *                                                              
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.           
 */                                                             
#define WINVER			0x0502
#define _WIN32_WINNT	0x0502
#define _WIN32_WINDOWS	0x0410
#define _WIN32_IE		0x0600

#define WIN32_LEAN_AND_MEAN
#define CINTERFACE

#include <windows.h>
//#include <richedit.h>
#include <commctrl.h>
#include <commdlg.h>

#include "AkelEdit.h"
#include "AkelDLL.h"
#include <sapi.h>


#include "resource.h"
#define CALLCONV	_fastcall

#define MAX_PATHW		32768

#ifdef _WIN64
#define PEM_EXGETSEL EM_EXGETSEL64
#define PLG_CHARRANGE CHARRANGE64
#else
#define PEM_EXGETSEL EM_EXGETSEL
#define PLG_CHARRANGE CHARRANGE
#endif

//! Settings IDS
#define SPS_VOICE		0x00000001
#define SPS_OUTPUT		0x00000002
#define SPS_VOLUME		0x00000004
#define SPS_RATE		0x00000008

#define SPS_ALL			0x0FFFFFFFF

static struct
{
	//! Runtime data
	HMODULE		hDll;
	BOOL		bLoaded;
	HWND		hMainWnd;
	UINT		nMDI;
	WCHAR		szAkelPath[MAX_PATHW];
	//! DOCK stuff
	HWND		hWndBar;
	DOCK*		pDock;

	UINT		uiPlayEvent;
	UINT		uiDumpEvent;
	UINT_PTR	uiTrackTimer;

	WNDPROCDATA*NewMainProcData;
	WNDPROCDATA*NewFrameProcData;

	//! Settings
	//! Active voice ID
	WCHAR		szVoice[32768];
	//! Active output device ID
	WCHAR		szOutput[32768];
	//! Volume
	LRESULT		lVolume;
	//! Rate
	LRESULT		lRate;

	//! Runtime data
	BOOL		bPlaying;
	BOOL		bDumping;
	//! TTS speakers
	ISpVoice*	pVoice;
	ISpVoice*	pFileSpeaker;
	//! Dumping path
	WCHAR		szFileName[MAX_PATHW];

	//! Bar Controls Handles
	HWND hComboVoice;
	HWND hComboOutput;
	HWND hPlayStop;
	HWND hVolume;
	HWND hRate;
	HWND hPlay2File;
} global;





//! Timers. It is easier to check manually than implement ISpEventSink
//#define SPCHECKTO			0
//#define SPCHECKINT			100
//#define SPCHECKTID			1

//! Speak2File stuff
#define PLAY2FILE			L"Play2File"
#define STOP2FILE			L"Stop"
#define PLAY2FILE_FILTER	L"Waveform audio files(*.wav)\0*.wav\0\0"
#define DEF_DUMP_EXT		L"wav"

//! Adjust range borders
#define minxmax(lo,val,hi)		(max((lo),min((val),(hi))))

//! Dumping format (WAVEFORMATEX)
#define CHANNELS			1
#define RATE				44100
#define BITSPERSAMPLE		16
#define AVGBPS				((RATE)*(CHANNELS)*(BITSPERSAMPLE)/8)
#define BLOCKALIGN			((CHANNELS)*(BITSPERSAMPLE)/8)

static void CALLCONV LoadPlugin(PLUGINDATA *ppd);
static void CALLCONV UnloadPlugin(PLUGINDATA *ppd);
static INT_PTR CALLBACK SpeechBar(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void CALLCONV UpdateSettings(DWORD dwSettings, BOOL bLoad);
static void CALLCONV DestroyDock(void);
static BOOL CALLCONV CreateDock(void);
static void CALLCONV HookMain(void);
static void CALLCONV UnhookMain(void);
static LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//! Control interraction
static void CALLCONV InitCombo(HWND hWnd,LPCWSTR wszCategory, LPCWSTR wszDefault);
static ISpObjectToken* CALLCONV GetCurToken(HWND hWnd);
static void CALLCONV FreeCombo(HWND hWnd);
static void CALLCONV AdjustPlayButton(HWND hButton,BOOL bPlaying);

static HRESULT CALLCONV GetEnumerator(LPCOLESTR szCat, IEnumSpObjectTokens** pEnum);
//! TTS Interaction
static HRESULT CALLCONV CreateSpeaker(void);
static HRESULT CALLCONV DestroySpeaker(void);
static HRESULT CALLCONV SwitchVoice(HWND hCombo);
static HRESULT CALLCONV SwitchOutput(HWND hCombo);
static HRESULT CALLCONV SetRate(LRESULT lRate);
static HRESULT CALLCONV SetVolume(USHORT nVolume);
static HRESULT CALLCONV StopSpeak(void);
static HRESULT CALLCONV Speak(LPCWSTR pText);
static HRESULT CALLCONV Speak2File(LPCWSTR szText, ISpObjectToken* pVoice, LPCWSTR szFile, LRESULT lRate, USHORT uVolume, LPWAVEFORMATEX pwfx);
static HRESULT CALLCONV AbortFile(void);
//! Dumping state signaling
static void CALLCONV InvalidateP2F(void);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		global.hDll				= hModule;
		global.bLoaded			= FALSE;
		global.hMainWnd			= NULL;
		global.bPlaying			= FALSE;
		global.bDumping			= FALSE;
		global.pVoice			= NULL;
		global.pFileSpeaker		= NULL;
		global.szVoice[0]		= 0;
		global.pDock			= NULL;
		global.hWndBar			= NULL;
		global.nMDI				= 0;
		global.NewMainProcData	= NULL;
		global.NewFrameProcData	= NULL;
		global.lVolume			= 50;
		global.lRate			= 0;
		global.uiPlayEvent		= 
		global.uiDumpEvent		= WM_NULL;
		global.uiTrackTimer		= 0;
		DisableThreadLibraryCalls(global.hDll);
	}
	else if(dwReason == DLL_PROCESS_DETACH)
	{
		UnloadPlugin(NULL);
	}
	return TRUE;
}
extern /*"C"*/ void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
	pv->dwAkelDllVersion=AKELDLL;
	pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1,-1,-1,-1);
	pv->dwExeMinVersion4x=MAKE_IDENTIFIER( 4, 8, 8, 0);
	pv->pPluginName="Speech";
}
extern /*"C"*/ void __declspec(dllexport) Main(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;
	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	if(!global.bLoaded)
	{
		LoadPlugin(pd);
		if(!global.bLoaded)
		{
			pd->nUnload = UD_UNLOAD;
			return;
		}
	}
	if(global.pDock)
	{
		DestroyDock();
		pd->nUnload = UD_UNLOAD;
	}
	else
	{
		HookMain();
		if(pd->bOnStart)
		{
			//! DOCK will be created in hook, when GUI will be ready
			pd->nUnload = UD_NONUNLOAD_ACTIVE;
		}
		else
		{
			if(CreateDock())
			{
				pd->nUnload = UD_NONUNLOAD_ACTIVE;
			}
			else
			{
				UnhookMain();
				pd->nUnload = UD_UNLOAD;
			}
		}
	}
}
static void CALLCONV LoadPlugin(PLUGINDATA *ppd)
{
	if(!global.bLoaded)
	{
		global.hMainWnd			= ppd->hMainWnd;
		global.szVoice[0]		= 0;
		global.szOutput[0]		= 0;
		global.nMDI				= ppd->nMDI;
		global.pDock			= NULL;
		global.hWndBar			= NULL;
		global.nMDI				= 0;
		lstrcpyW(global.szAkelPath,ppd->wszAkelDir);
		global.lVolume			= 50;
		global.lRate			= 0;
		global.pVoice			= NULL;
		global.pFileSpeaker		= NULL;
		global.bPlaying			= FALSE;
		global.bDumping			= FALSE;
		global.NewMainProcData	= NULL;
		global.NewFrameProcData	= NULL;
		global.uiPlayEvent		= RegisterWindowMessageW(L"AKS_PEF");
		global.uiDumpEvent		= RegisterWindowMessageW(L"AKS_DEF");
		global.bLoaded = TRUE;
		//! Read settings
		UpdateSettings(SPS_ALL,TRUE);
	}
}
static void CALLCONV UnloadPlugin(PLUGINDATA *ppd)
{
	if(global.bLoaded)
	{
		DestroyDock();
		UnhookMain();
	}
	global.bLoaded = FALSE;
}
static INT_PTR CALLBACK SpeechBar(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	if(uMsg>=WM_USER)
	{
		wchar_t t[30]={0};
		wsprintfW(t,L"mes=%d\n",uMsg);
		OutputDebugStringW(t);
	}
#endif //_DEBUG
	if(WM_INITDIALOG == uMsg)
	{
		BUTTONDRAW bd={BIF_CROSS|BIF_ETCHED};
		global.hComboVoice	= GetDlgItem(hWndDlg,IDC_VOICE);
		global.hComboOutput	= GetDlgItem(hWndDlg,IDC_OUTPUT);
		global.hPlayStop	= GetDlgItem(hWndDlg,IDC_PLAY);
		global.hVolume		= GetDlgItem(hWndDlg,IDC_VOLUME);
		global.hRate		= GetDlgItem(hWndDlg,IDC_RATE);
		global.hPlay2File	= GetDlgItem(hWndDlg,IDC_PLAY2FILE);
		SendMessageW(global.hVolume,TBM_SETRANGE,(WPARAM)TRUE,MAKELONG(0,100));
		SendMessageW(global.hVolume,TBM_SETPOS,(WPARAM)TRUE,global.lVolume);
		SendMessageW(global.hVolume,TBM_SETTICFREQ,10,0);
		SendMessageW(global.hRate,TBM_SETRANGE,(WPARAM)TRUE,MAKELONG(-10,10));
		SendMessageW(global.hRate,TBM_SETPOS,(WPARAM)TRUE,global.lRate);
		SendMessageW(global.hRate,TBM_SETTICFREQ,2,0);
		SendMessageW(global.hMainWnd, AKD_SETBUTTONDRAW, (WPARAM)GetDlgItem(hWndDlg,IDCANCEL), (LPARAM)&bd);
		InitCombo(global.hComboVoice, SPCAT_VOICES, global.szVoice);
		InitCombo(global.hComboOutput, SPCAT_AUDIOOUT, global.szOutput);
		SwitchVoice(global.hComboVoice);
		SwitchOutput(global.hComboOutput);
		AdjustPlayButton(global.hPlayStop,FALSE);
		InvalidateP2F();
		//SetTimer(hWndDlg,SPCHECKTID,SPCHECKINT,0);
	}
	else if(WM_CLOSE == uMsg)
	{
		PostMessageW(hWndDlg,WM_COMMAND,MAKEWPARAM(IDCANCEL,BN_CLICKED),0);
		return TRUE;
	}
	else if(WM_SIZE == uMsg)
	{
		if(global.pDock)
		{
			RECT *rc = &global.pDock->rcDragDrop;
			GetWindowRect(global.hWndBar,rc);
			rc->right  -= rc->left;
			rc->bottom -= rc->top;
			rc->left = rc->top = 0;
		}
	}
	else if(WM_COMMAND == uMsg)
	{
		UINT IDC = LOWORD(wParam);
		UINT CODE = HIWORD(wParam);
		if((IDC_VOICE == IDC)&&(CBN_SELCHANGE == CODE))
		{
			SwitchVoice(global.hComboVoice);
		}
		else if((IDC_OUTPUT == IDC) && (CBN_SELCHANGE == CODE))
		{
			SwitchOutput(global.hComboOutput);
		}
		else if((IDC == IDCANCEL) && (CODE==BN_CLICKED))
		{
			SendMessageW(global.hMainWnd,AKD_DOCK,DK_UNSUBCLASS,(LPARAM)global.pDock);
			SendMessageW(global.hMainWnd,AKD_DOCK,DK_DELETE,(LPARAM)global.pDock);
			global.pDock = NULL;
			DestroyWindow(hWndDlg);
			global.hWndBar = NULL;
			SendMessageW(global.hMainWnd,AKD_RESIZE,0,0);
			UnhookMain();
			DestroySpeaker();
			return TRUE;
		}
		else if((IDC == IDC_PLAY) && (CODE==BN_CLICKED))
		{
			EDITINFO ei;
			if(global.bPlaying)
			{
				HRESULT hr = StopSpeak();
				if(SUCCEEDED(hr))
				{
					global.bPlaying = FALSE;
					AdjustPlayButton(global.hPlayStop,FALSE);
				}
				return TRUE;
			}
			if(SendMessageW(global.hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei))
			{
				HRESULT hr = S_OK;
				if(global.pVoice)
				{
					StopSpeak();
					AdjustPlayButton(global.hPlayStop,FALSE);
				}
				else
				{
					hr = CreateSpeaker();
					if(SUCCEEDED(hr))
					{
						hr = SetRate(global.lRate);
					}
					if(SUCCEEDED(hr))
					{
						hr = SetVolume((USHORT)global.lVolume);
					}
				}
				if(SUCCEEDED(hr))
				{
					hr = SwitchVoice(global.hComboVoice);
				}
				if(SUCCEEDED(hr))
				{
					hr = SwitchOutput(global.hComboOutput);
				}
				if(SUCCEEDED(hr))
				{
					PLG_CHARRANGE cr={0,0};
					SendMessageW(ei.hWndEdit,PEM_EXGETSEL,0,(LPARAM)&cr);
					if(cr.cpMin < cr.cpMax)
					{
						GETTEXTRANGE gtr = {cr.cpMin, cr.cpMax};
						SendMessageW(global.hMainWnd,AKD_GETTEXTRANGEW,(WPARAM)ei.hWndEdit,(LPARAM)&gtr);
						if(gtr.pText)
						{
							hr = Speak((LPCWSTR)gtr.pText);
							SendMessageW(global.hMainWnd,AKD_FREETEXT,0,(LPARAM)gtr.pText);
							if(SUCCEEDED(hr))
							{
								global.bPlaying = TRUE;
								AdjustPlayButton(global.hPlayStop,TRUE);
							}
						}
					}
				}
			}
			return TRUE;
		}
		else if(IDC == IDC_PLAY2FILE && CODE == BN_CLICKED)
		{
			InvalidateP2F();
			if(global.bDumping)
			{
				AbortFile();
				InvalidateP2F();
				return TRUE;
			}
			else
			{
				GETTEXTRANGE gtr;
				//! Pick text to read
				{
					EDITINFO ei;
					PLG_CHARRANGE cr;
					if(!SendMessageW(global.hMainWnd,AKD_GETEDITINFO,0,(LPARAM)&ei))
						return FALSE;
					SendMessageW(ei.hWndEdit,PEM_EXGETSEL,0,(LPARAM)&cr);
					if(cr.cpMin == cr.cpMax) return FALSE;
					gtr.cpMin = cr.cpMin;
					gtr.cpMax = cr.cpMax;
					if(!SendMessageW(global.hMainWnd,AKD_GETTEXTRANGEW,(WPARAM)ei.hWndEdit,(LPARAM)&gtr))
					{
						SendMessageW(global.hMainWnd,AKD_FREETEXT,0,(LPARAM)gtr.pText);
						return FALSE;
					}
				}
				//! Pick location to save to
				{
					OPENFILENAMEW ofn;
					HWND hOldFocus = GetFocus();
					global.szFileName[0] = 0;
					ofn.lStructSize = sizeof(OPENFILENAMEW);
					ofn.hwndOwner = global.hMainWnd;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = PLAY2FILE_FILTER;
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter = 0;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = global.szFileName;
					ofn.nMaxFile = MAX_PATHW;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle =0;
					ofn.lpstrInitialDir = global.szAkelPath;
					ofn.lpstrTitle = NULL;
					ofn.Flags = OFN_FORCESHOWHIDDEN|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
						|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_DONTADDTORECENT|OFN_OVERWRITEPROMPT;
					ofn.nFileOffset = ofn.nFileExtension = 0;
					ofn.lpstrDefExt = DEF_DUMP_EXT;
					ofn.lCustData = 0;
					ofn.lpfnHook = 0;
					ofn.lpTemplateName = 0;
					ofn.pvReserved = 0;
					ofn.dwReserved = 0;
					ofn.FlagsEx = 0;
					if(GetSaveFileNameW(&ofn))
					{
						//! 44kHz 16bit Mono
						WAVEFORMATEX wfx = {WAVE_FORMAT_PCM,CHANNELS,RATE,AVGBPS,BLOCKALIGN,BITSPERSAMPLE,0};
						//! Start reading
						Speak2File((LPCWSTR)gtr.pText,GetCurToken(global.hComboVoice),ofn.lpstrFile,global.lRate,(USHORT)global.lVolume,&wfx);
						InvalidateP2F();
					}
					SetFocus(hOldFocus);
				}
				SendMessageW(global.hMainWnd,AKD_FREETEXT,0,(LPARAM)gtr.pText);
				return TRUE;
			}
		}
	}
	else if(WM_DESTROY == uMsg)
	{
		//KillTimer(hWndDlg,SPCHECKTID);
		FreeCombo(global.hComboVoice);
		FreeCombo(global.hComboOutput);
		AbortFile();
		SetFocus(global.hMainWnd);
		PostMessageW(global.hMainWnd,AKD_DLLUNLOAD,(WPARAM)global.hDll,0);
	}
	else if(WM_HSCROLL == uMsg)
	{
		//! Trackball iteraction
		if((HWND)lParam == global.hVolume)
		{
			global.lVolume = SendMessageW(global.hVolume,TBM_GETPOS,0,0);
			SetVolume((USHORT)global.lVolume);
			UpdateSettings(SPS_VOLUME,FALSE);
			return TRUE;
		}
		if((HWND)lParam == global.hRate)
		{
			global.lRate = SendMessageW(global.hRate,TBM_GETPOS,0,0);
			SetRate(global.lRate);
			UpdateSettings(SPS_RATE,FALSE);
			return TRUE;
		}
	}
	else if(uMsg == global.uiPlayEvent)
	{
		if(global.pVoice)
		{
			SPEVENT event;
			if(S_OK == global.pVoice->lpVtbl->GetEvents(global.pVoice,1,&event,NULL))
			{
				if(event.eEventId == SPEI_END_INPUT_STREAM)
				{
					global.bPlaying = FALSE;
					AdjustPlayButton(global.hPlayStop,FALSE);
				}
			}
		}
		SetWindowLongPtrW(hWndDlg,DWLP_MSGRESULT,1);
		return TRUE;
	}
	else if(uMsg == global.uiDumpEvent)
	{
		if(global.pFileSpeaker)
		{
			SPEVENT event;
			if(S_OK == global.pFileSpeaker->lpVtbl->GetEvents(global.pFileSpeaker,1,&event,NULL))
			{
				if(event.eEventId == SPEI_END_INPUT_STREAM)
				{
					InvalidateP2F();
				}
			}
		}
		SetWindowLongPtrW(hWndDlg,DWLP_MSGRESULT,1);
		return TRUE;
	}
	else if(uMsg == WM_TIMER)
	{
		if(wParam == 1)
		{
			InvalidateP2F();
			if(!global.bDumping) KillTimer(global.hWndBar,1);
		}
	}
	return FALSE;
}
static void CALLCONV UpdateSettings(DWORD dwSettings, BOOL bLoad)
{
	PLUGINOPTIONW po;
	HANDLE hOpt = (HANDLE)
	SendMessageW(global.hMainWnd,AKD_BEGINOPTIONSW,bLoad?POB_READ:POB_SAVE,(LPARAM)L"Speech");
	if(hOpt)
	{
		if(dwSettings & SPS_VOICE)
		{
			po.pOptionName = L"VOICE";
			po.dwType = PO_STRING;
			po.dwData = bLoad?sizeof(global.szVoice):((lstrlenW(global.szVoice)+1)*sizeof(WCHAR));
			po.lpData = (LPBYTE)global.szVoice;
			SendMessageW(global.hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		if(dwSettings & SPS_OUTPUT)
		{
			po.pOptionName = L"OUTPUT";
			po.dwType = PO_STRING;
			po.dwData = bLoad?sizeof(global.szOutput):((lstrlenW(global.szOutput)+1)*sizeof(WCHAR));
			po.lpData = (LPBYTE)global.szOutput;
			SendMessageW(global.hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
		}
		if(dwSettings & SPS_VOLUME)
		{
			po.pOptionName = L"VOLUME";
			po.dwType = PO_DWORD;
			po.dwData = sizeof(LONG);
			po.lpData = (LPBYTE)&global.lVolume;
			SendMessageW(global.hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
			if(bLoad)
			{
				global.lVolume = minxmax(0,global.lVolume,100);
			}
		}
		if(dwSettings & SPS_RATE)
		{
			po.pOptionName = L"RATE";
			po.dwType = PO_DWORD;
			po.dwData = sizeof(LONG);
			po.lpData = (LPBYTE)&global.lRate;
			SendMessageW(global.hMainWnd,AKD_OPTIONW,(WPARAM)hOpt,(LPARAM)&po);
			if(bLoad)
			{
				global.lRate = minxmax(-10,global.lRate,10);
			}
		}
		SendMessageW(global.hMainWnd,AKD_ENDOPTIONS,(WPARAM)hOpt,0);
	}
}
static BOOL CALLCONV CreateDock(void)
{
	DOCK dk;
	if(global.pDock) return TRUE;
	dk.nSide = DKS_BOTTOM;
	dk.dwFlags = DKF_NODROPLEFT|DKF_NODROPRIGHT|DKF_FIXEDSIZE|DKF_DRAGDROP;
	dk.prev = dk.next = 0;
	dk.rcSize.left  = dk.rcSize.top =0;
	dk.rcSize.right = dk.rcSize.bottom = 0;
	dk.hWnd = 0;
	global.pDock = (DOCK*)SendMessageW(global.hMainWnd,AKD_DOCK,DK_ADD,(LPARAM)&dk);
	if(global.pDock)
	{
		global.hWndBar = CreateDialogW(global.hDll,MAKEINTRESOURCEW(IDD_WORKBAR),global.hMainWnd,SpeechBar);
		if(IsWindow(global.hWndBar))
		{
			RECT *rc = &global.pDock->rcSize;
			GetWindowRect(global.hWndBar,rc);
			rc->right  -= rc->left;
			rc->bottom -= rc->top;
			rc->left = rc->top = 0;
			global.pDock->rcDragDrop.left = rc->left;
			global.pDock->rcDragDrop.right = rc->right;
			global.pDock->rcDragDrop.top = rc->top;
			global.pDock->rcDragDrop.bottom = rc->bottom;
			global.pDock->hWnd = global.hWndBar;
			SendMessage(global.hMainWnd, AKD_DOCK, DK_SUBCLASS, (LPARAM)global.pDock);
			SendMessage(global.hMainWnd, AKD_DOCK, DK_SHOW, (LPARAM)global.pDock);
			return TRUE;
		}
		else
		{
			SendMessageW(global.hMainWnd,AKD_DOCK,DK_DELETE,(LPARAM)global.pDock);
			global.pDock = NULL;
			return FALSE;
		}
	}
	else return FALSE;
}
static void CALLCONV DestroyDock(void)
{
	if(global.pDock)
	{
		SendMessageW(global.hWndBar,WM_COMMAND,MAKEWPARAM(IDCANCEL,BN_CLICKED),0);
	}
}
static void CALLCONV HookMain(void)
{
	global.NewMainProcData = NULL;
	SendMessageW(global.hMainWnd, AKD_SETMAINPROC, (WPARAM)NewEditParentProc, (LPARAM)&global.NewMainProcData);
	if (global.nMDI == WMD_MDI)
	{
		global.NewFrameProcData = NULL;
		SendMessageW(global.hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NewEditParentProc, (LPARAM)&global.NewFrameProcData);
	}
}
static void CALLCONV UnhookMain(void)
{
	if (global.NewMainProcData)
	{
		SendMessageW(global.hMainWnd, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&global.NewMainProcData);
		global.NewMainProcData=NULL;
	}
	if (global.nMDI == WMD_MDI)
	{
		if (global.NewFrameProcData)
		{
			SendMessageW(global.hMainWnd, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&global.NewFrameProcData);
			global.NewFrameProcData=NULL;
		}
	}
}
static LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == AKDN_MAIN_ONFINISH)
	{
		LRESULT lRes = 0;
		if (hWnd == global.hMainWnd)
			lRes = global.NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
		else
			lRes = global.NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
		//! Destroy TTS and DOCK
		DestroyDock();
		UnhookMain();
		DestroySpeaker();
		return lRes;
	}
	else if(uMsg == AKDN_MAIN_ONSTART_FINISH)
	{
		//! Create DOCK. We shall be here if and only if Main was market as Autostart
		//! in which case DOCK creation were delegated here.
		CreateDock();
	}
	if (hWnd == global.hMainWnd)
		return global.NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
	else
		return global.NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}
static void CALLCONV InitCombo(HWND hWnd, LPCWSTR wszCategory, LPCWSTR wszDefault)
{
	HRESULT hr = S_OK;
	ISpObjectToken *pToken = NULL;
	IEnumSpObjectTokens *pEnum = NULL;
	hr = GetEnumerator(wszCategory,&pEnum);
	if(SUCCEEDED(hr))
	{
		BOOL bSelected = FALSE;
		WCHAR* szText;
		while(S_OK == pEnum->lpVtbl->Next(pEnum,1,&pToken,0))
		{
			szText = NULL;
			hr = pToken->lpVtbl->GetStringValue(pToken,NULL,&szText);
			if(SUCCEEDED(hr))
			{
				LRESULT ind = SendMessageW(hWnd,CB_ADDSTRING,0,(LPARAM)szText);
				if( ind == CB_ERR || ind == CB_ERRSPACE)
				{
					hr = E_FAIL;
				}
				else
				{
					LPWSTR szID=NULL;
					SendMessageW(hWnd,CB_SETITEMDATA,ind,(LPARAM)pToken);
					hr = pToken->lpVtbl->GetId(pToken,&szID);
					if(!bSelected || !lstrcmpW(szID,wszDefault))
					{
						bSelected = TRUE;
						SendMessageW(hWnd,CB_SETCURSEL,ind,0);
					}
					CoTaskMemFree(szID);
				}
				CoTaskMemFree(szText);
			}
			if(FAILED(hr))
			{
				pToken->lpVtbl->Release(pToken);
			}
			pToken = NULL;
		}
		pEnum->lpVtbl->Release(pEnum);
	}
}
static ISpObjectToken* CALLCONV GetCurToken(HWND hWnd)
{
	LRESULT ind = SendMessageW(hWnd,CB_GETCURSEL,0,0);
	if(ind == CB_ERR|| ind == CB_ERRSPACE)
		return NULL;
	return (ISpObjectToken*)SendMessageW(hWnd,CB_GETITEMDATA,ind,0);
}
static void CALLCONV FreeCombo(HWND hWnd)
{
	ISpObjectToken *pToken = NULL;
	LRESULT i=0;
	LRESULT n = SendMessageW(hWnd,CB_GETCOUNT,0,0);
	while(i<n)
	{
		pToken = (ISpObjectToken*)SendMessageW(hWnd,CB_GETITEMDATA,i,0);
		if(pToken)
		{
			pToken->lpVtbl->Release(pToken);
		}
		i++;
	}
}
static HRESULT CALLCONV GetEnumerator(LPCOLESTR szCat, IEnumSpObjectTokens** pEnum)
{
	HRESULT hr = S_OK;
	ISpObjectTokenCategory * pCat = NULL;
	hr = CoCreateInstance(&CLSID_SpObjectTokenCategory,NULL,CLSCTX_ALL,&IID_ISpObjectTokenCategory,(LPVOID*)&pCat);
	if(SUCCEEDED(hr))
	{
		hr = pCat->lpVtbl->SetId(pCat,szCat,FALSE);
	}
	if(SUCCEEDED(hr))
	{
		hr = pCat->lpVtbl->EnumTokens(pCat,NULL,NULL,pEnum);
	}
	if(pCat)
	{
		pCat->lpVtbl->Release(pCat);
	}
	return hr;
}
static HRESULT CALLCONV CreateSpeaker(void)
{
	HRESULT hr = S_OK;
	if(global.pVoice) return S_OK;
	hr = CoCreateInstance(&CLSID_SpVoice,NULL,CLSCTX_ALL,&IID_ISpVoice,(LPVOID*)&global.pVoice);
	if(SUCCEEDED(hr))
	{
		hr = SwitchOutput(global.hComboOutput);
		//hr = global.pVoice->lpVtbl->SetOutput(global.pVoice,NULL,TRUE);
	}
	if(SUCCEEDED(hr))
	{
		hr = global.pVoice->lpVtbl->SetNotifyWindowMessage(global.pVoice,global.hWndBar,global.uiPlayEvent,0,0);
		hr = global.pVoice->lpVtbl->SetInterest(global.pVoice,SPFEI(SPEI_END_INPUT_STREAM),SPFEI(SPEI_END_INPUT_STREAM));
	}
	return hr;
}
static HRESULT CALLCONV DestroySpeaker(void)
{
	if(global.pVoice)
	{
		StopSpeak();
		global.pVoice->lpVtbl->Release(global.pVoice);
		global.pVoice = NULL;
	}
	return S_OK;
}
static HRESULT CALLCONV SwitchVoice(HWND hCombo)
{
	ISpObjectToken* pToken = GetCurToken(hCombo);
	if(pToken)
	{
		HRESULT hr = S_OK;
		LPOLESTR pID = NULL;
		hr = pToken->lpVtbl->GetId(pToken,&pID);
		if(SUCCEEDED(hr))
		{
			if(lstrcmpW(global.szVoice,pID))
			{
				WCHAR *d = global.szVoice, *s = pID;
				while(*d++ = *s++);
				CoTaskMemFree(pID);
				UpdateSettings(SPS_VOICE, FALSE);
			}
			if(global.pVoice)
			{
				ISpObjectToken	*pOldToken = NULL;//,
								//*pToken = NULL;
				//pToken = GetCurToken(hCombo);
				if(pToken)
				{
					hr = global.pVoice->lpVtbl->GetVoice(global.pVoice,&pOldToken);
					if(SUCCEEDED(hr))
					{
						if(pOldToken != pToken)
						{
							hr = global.pVoice->lpVtbl->SetVoice(global.pVoice,pToken);
							if(SUCCEEDED(hr))
							{
								hr = SetRate(global.lRate);
							}
							if(SUCCEEDED(hr))
							{
								hr = SetVolume((USHORT)global.lVolume);
							}
						}
					}
				}
				else hr = E_FAIL;
			}
		}
		return hr;
	}
	return E_FAIL;
}
static HRESULT CALLCONV SwitchOutput(HWND hCombo)
{
	ISpObjectToken* pToken = GetCurToken(hCombo);
	if(pToken)
	{
		HRESULT hr = S_OK;
		LPOLESTR pID = NULL;
		hr = pToken->lpVtbl->GetId(pToken,&pID);
		if(SUCCEEDED(hr))
		{
			if(lstrcmpW(global.szOutput,pID))
			{
				WCHAR *d = global.szOutput, *s = pID;
				while(*d++ = *s++);
				CoTaskMemFree(pID);
				UpdateSettings(SPS_OUTPUT, FALSE);
			}
			if(global.pVoice)
			{
				ISpObjectToken	*pOldToken = NULL;//,
								//*pToken = NULL;
				//pToken = GetCurToken(hCombo);
				if(pToken)
				{
					hr = global.pVoice->lpVtbl->GetOutputObjectToken(global.pVoice,&pOldToken);
					if(SUCCEEDED(hr))
					{
						if(pOldToken != pToken)
						{
							IUnknown* pUnk = NULL;
							hr = pToken->lpVtbl->QueryInterface(pToken,&IID_IUnknown,(LPVOID*)&pUnk);
							if(SUCCEEDED(hr))
							{
								hr = global.pVoice->lpVtbl->SetOutput(global.pVoice,pUnk,FALSE);
								pUnk->lpVtbl->Release(pUnk);
								/*if(SUCCEEDED(hr))
								{
									hr = SetRate(global.lRate);
								}
								if(SUCCEEDED(hr))
								{
									hr = SetVolume((USHORT)global.lVolume);
								}*/
							}
						}
					}
				}
				else hr = E_FAIL;
			}
		}
		return hr;
	}
	return E_FAIL;
}
static HRESULT CALLCONV SetRate(LRESULT lRate)
{
	if(global.pVoice)
	{
		return global.pVoice->lpVtbl->SetRate(global.pVoice,(long)lRate);
	}
	return S_OK;
}
static HRESULT CALLCONV SetVolume(USHORT uVolume)
{
	if(global.pVoice)
	{
		return global.pVoice->lpVtbl->SetVolume(global.pVoice,uVolume);
	}
	return S_OK;
}
static HRESULT CALLCONV StopSpeak(void)
{
	if(global.pVoice)
	{
		return global.pVoice->lpVtbl->Speak(global.pVoice,NULL,SPF_PURGEBEFORESPEAK,NULL);
	}
	return S_OK;
}
static HRESULT CALLCONV Speak(LPCWSTR pText)
{
	if(global.pVoice)
	{
		return global.pVoice->lpVtbl->Speak(global.pVoice,pText,SPF_IS_NOT_XML|SPF_ASYNC,NULL);
	}
	return S_OK;
}
static void CALLCONV AdjustPlayButton(HWND hButton,BOOL bPlaying)
{
	SendMessageW(hButton,WM_SETTEXT, 0, (LPARAM)(bPlaying?L"S":L"P"));
}
static HRESULT CALLCONV Speak2File(LPCWSTR szText, ISpObjectToken* pVoice, LPCWSTR szFile, LRESULT lRate, USHORT uVolume, LPWAVEFORMATEX pwfx)
{
	HRESULT hr = S_OK;
	ISpStream* pStream = NULL;
	IUnknown* pUnk = NULL;
	if(!pVoice) return E_POINTER;
	if(global.pFileSpeaker) AbortFile();
	hr = CoCreateInstance(&CLSID_SpStream,NULL,CLSCTX_ALL,&IID_ISpStream,(LPVOID*)&pStream);
	if(FAILED(hr)) return hr;
	hr = pStream->lpVtbl->BindToFile(pStream,szFile,SPFM_CREATE_ALWAYS,&SPDFID_WaveFormatEx,pwfx,SPFEI_ALL_EVENTS);
	if(SUCCEEDED(hr))
	{
		hr = pStream->lpVtbl->QueryInterface(pStream,&IID_IUnknown,(LPVOID*)&pUnk);
		if(SUCCEEDED(hr))
		{
			hr = CoCreateInstance(&CLSID_SpVoice,NULL,CLSCTX_ALL,&IID_ISpVoice,(LPVOID*)&global.pFileSpeaker);
			if(SUCCEEDED(hr))
			{
				hr = global.pFileSpeaker->lpVtbl->SetVoice(global.pFileSpeaker,pVoice);
				hr = global.pFileSpeaker->lpVtbl->SetNotifyWindowMessage(global.pFileSpeaker,global.hWndBar,global.uiDumpEvent,0,0);
				hr = global.pFileSpeaker->lpVtbl->SetInterest(global.pFileSpeaker,SPFEI_ALL_TTS_EVENTS,SPFEI_ALL_TTS_EVENTS/*SPFEI(SPEI_END_INPUT_STREAM),SPFEI(SPEI_END_INPUT_STREAM)*/);
				if(SUCCEEDED(hr))
				{
					global.pFileSpeaker->lpVtbl->SetVolume(global.pFileSpeaker,uVolume);
					global.pFileSpeaker->lpVtbl->SetRate(global.pFileSpeaker,(long)lRate);
					hr = global.pFileSpeaker->lpVtbl->SetOutput(global.pFileSpeaker,pUnk,FALSE);
					if(SUCCEEDED(hr))
					{
						hr = global.pFileSpeaker->lpVtbl->Speak(global.pFileSpeaker,szText,SPF_IS_NOT_XML|SPF_ASYNC,NULL);
					}
				}
				if(FAILED(hr))
				{
					global.pFileSpeaker->lpVtbl->Release(global.pFileSpeaker);
					global.pFileSpeaker = NULL;
				}
				else global.bDumping = TRUE;
				InvalidateP2F();
			}
			pUnk->lpVtbl->Release(pUnk);
			pUnk = NULL;
		}
	}
	pStream->lpVtbl->Release(pStream);
	if(SUCCEEDED(hr))
	{
		SetTimer(global.hWndBar,1,1000,NULL);
	}
	return hr;
}
static HRESULT CALLCONV AbortFile(void)
{
	if(!global.pFileSpeaker) return S_OK;
	KillTimer(global.hWndBar,1);
	global.pFileSpeaker->lpVtbl->Speak(global.pFileSpeaker,NULL,SPF_PURGEBEFORESPEAK,NULL);
	global.pFileSpeaker->lpVtbl->Release(global.pFileSpeaker);
	global.pFileSpeaker = NULL;
	return S_OK;
}
static void CALLCONV InvalidateP2F(void)
{
	BOOL bChange = FALSE;
	if(global.bDumping)
	{
		if(global.pFileSpeaker)
		{
			if(S_OK == global.pFileSpeaker->lpVtbl->WaitUntilDone(global.pFileSpeaker,0))
			{
				global.bDumping = FALSE;
				bChange = TRUE;
				global.pFileSpeaker->lpVtbl->Release(global.pFileSpeaker);
				global.pFileSpeaker = NULL;
			}
		}
		else
		{
			global.bDumping = FALSE;
			bChange = TRUE;
		}
	}
	else
	{
		if(global.pFileSpeaker)
		{
			if(S_OK == global.pFileSpeaker->lpVtbl->WaitUntilDone(global.pFileSpeaker,0))
			{
				global.pFileSpeaker->lpVtbl->Release(global.pFileSpeaker);
				global.pFileSpeaker = NULL;
			}
			else
			{
				global.bDumping = TRUE;
				bChange = TRUE;
			}
		}
	}
	if(bChange)
		SendMessageW(global.hPlay2File,WM_SETTEXT,0,(LPARAM)(global.bDumping?STOP2FILE:PLAY2FILE));
}
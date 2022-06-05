#ifndef __COMMON_H__
#define __COMMON_H__
/**
 *		SpellChecker for AkelPad
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)
 *
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.
 *		The New Aspell Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
 *		license version 2.0 or 2.1.
 */

#define WINVER			0x0501
#define _WIN32_WINNT	0x0501
#define _WIN32_IE		0x0600
//! Rare stuff
#define WIN32_LEAN_AND_MEAN
//! Exclude unused. A lot of NOs :)
#define NOGDICAPMASKS     //- CC_*, LC_*, PC_*, CP_*, TC_*, RC_
//#define NOVIRTUALKEYCODES //- VK_*
//#define NOWINMESSAGES     //- WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       //- WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      //- SM_*
//#define NOMENUS           //- MF_*
#define NOICONS           //- IDI_*
#define NOKEYSTATES       //- MK_*
#define NOSYSCOMMANDS     //- SC_*
#define NORASTEROPS       //- Binary and Tertiary raster ops
#define NOSHOWWINDOW      //- SW_*
//#define OEMRESOURCE       //- OEM Resource values
#define NOATOM            //- Atom Manager routines
#define NOCLIPBOARD       //- Clipboard routines
#define NOCOLOR           //- Screen colors
//#define NOCTLMGR          //- Control and Dialog routines
#define NODRAWTEXT        //- DrawText() and DT_*
//#define NOGDI             //- All GDI defines and routines
//#define NOKERNEL          //- All KERNEL defines and routines
//#define NOUSER            //- All USER defines and routines
//#define NONLS             //- All NLS defines and routines
//#define NOMB              //- MB_* and MessageBox()
#define NOMEMMGR          //- GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        //- typedef METAFILEPICT
#define NOMINMAX          //- Macros min(a,b) and max(a,b)
//#define NOMSG             //- typedef MSG and associated routines
#define NOOPENFILE        //- OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          //- SB_* and scrolling routines
#define NOSERVICE         //- All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           //- Sound driver routines
//#define NOTEXTMETRIC      //- typedef TEXTMETRIC and associated routines
//#define NOWH              //- SetWindowsHook and WH_*
//#define NOWINOFFSETS      //- GWL_*, GCL_*, associated routines
#define NOCOMM            //- COMM driver routines
#define NOKANJI           //- Kanji support stuff.
#define NOHELP            //- Help engine interface.
#define NOPROFILER        //- Profiler interface.
#define NODEFERWINDOWPOS  //- DeferWindowPos routines
#define NOMCX             //- Modem Configuration Extensions

#include <tchar.h>
//! Platform SDK
#include <windows.h>
//! RichEdit
#include <richedit.h>
//! Common Dialogs
#include <commdlg.h>
//! Akelpad's will
#include "AkelEdit.h"
#include "AkelDLL.h"
//! Resources
#include "resource.h"

//////extern HWND cmn_hMainWindow;
//////extern int cmn_nMDI;

#define CALLCONV __fastcall

#define REDRAW_DELAY
#define MAX_PATHW			0x00008000
//!	iso encodings base
#define CP_ISOBASE			28590
//! Default setting values
#define DEF_LANG			L"en"
#define DEF_LANGA			"en"
#define DEF_JARGON			L""
#define DEF_JARGONA			""
#define DEF_ENCODING		(CP_ISOBASE+1)
#define DEF_NOTICE			TRUE
#define DEF_HIGHLIGHT		TRUE
#define DEF_WHITELIST		TRUE
#define DEF_UNDERLINECOLOR	RGB(255,0,0)
#define DEF_UNDERLINESTYLE	PS_SOLID
#define DEF_COREPATH		L""
#define DEF_XSCROLL			XSCROLL_MASK_HORZ
#define DEF_NOBG			L""
#define DEF_DELAYBACK		FALSE
#define DEF_DELAYTICK		500
//! Settings keys
#define SET_LANG			L"Lang"
#define SET_JARGON			L"Jargon"
//#define SET_ENCODING		L"Encoding"
#define SET_NOTICE			L"Notice"
#define SET_HIGHLIGHT		L"Highlight"
#define SET_WHITELIST		L"WhiteList"
#define SET_UNDELINECOLOR	L"UColor"
#define SET_UNDELINESTYLE	L"UStyle"
#define SET_COREPATH		L"CorePath"
#define SET_WM_XSCROLL		L"WM_XSCROLL"
#define SET_NOBG			L"NOBG"
#define SET_DELAYBACK		L"delayback"
#define SET_DELAYTICK		L"delaytick"
//! ASpell core library
#define CORE_DLL_NAME		L"aspell-15.dll"
//! Default path
#define INTERNAL_CORE_FORMAT L"%s\\AkelFiles\\Plugs\\SpellCheck\\bin\\" CORE_DLL_NAME
//! External commands codes
#define SPI_LANG		0
//#define SPI_ENC			1
#define SPI_NOTICE		2
#define SPI_HIGHLIGHT	3
#define SPI_UNDERLINE	4
#define SPI_COREPATH	5
#define SPI_WHITELIST	6

#define SCUS_SOLID				PS_SOLID
#define SCUS_DASH				PS_DASH						/* -------  */
#define SCUS_DOT				PS_DOT						/* .......  */
#define SCUS_DASHDOT			PS_DASHDOT					/* _._._._  */
#define SCUS_DASHDOTDOT			PS_DASHDOTDOT				/* _.._.._  */
#define SCUS_LASTBUILTINSTYLE	SCUS_DASHDOTDOT
#define SCUS_COMPACTDOT			(SCUS_LASTBUILTINSTYLE +1)
#define SCUS_WAVE				(SCUS_LASTBUILTINSTYLE +2)
#define SCUS_INVALIDSTYLE		(SCUS_LASTBUILTINSTYLE +3)

#define UNDERSCORE_SCALE		10

#define XSCROLL_MASK_NONE		0
#define XSCROLL_MASK_HORZ		1
#define XSCROLL_MASK_VERT		2
#define XSCROLL_MASK_BOTH		3
#define MAX_WORD				128

class CAspellModule;
//! Forward
typedef struct SRule RULE, *PRULE;
typedef struct SEditAssoc EDITASSOC, *PEDITASSOC, *LPEDITASSOC;
//! Assocc container
struct SEditAssoc
{
	FRAMEDATA*	pFrame;
	PRULE		rule;
	PEDITASSOC	next;
};
//! Rules struct
struct SRule
{
	WCHAR ext[128];
	WCHAR *szWhiteList;
	WCHAR *szWhiteListS;
	SIZE_T szWL;
	SIZE_T szWLS;
	SIZE_T fWL;
	SIZE_T fWLs;
	PRULE pNextRule;
	PRULE pNextRuleOperative;
};

#pragma pack(1)
//! Set language
struct SP_LANG
{
	WCHAR*	szLang;
	WCHAR*	szJargon;
};
//! Set encoding
struct SP_ENC
{
	UINT_PTR	uiEncoding;
};
//! Set notice
struct SP_NOTICE
{
	UINT_PTR	uiNotice;
};
//! Set highlight
struct SP_HIGHLIGHT
{
	UINT_PTR	uiHighLight;
};
//! Set underline style
struct SP_UNDERLINE
{
	DWORD_PTR	dwColor;
	DWORD_PTR	dwStyle;
};
//! Set ASpell's core path
struct SP_COREPATH
{
	WCHAR*	szCorePath;
};
struct SP_WHITELIST
{
	UINT_PTR	uiWhiteList;
};
//! External call structure param
struct SPELLCHECKSETUPPARAM
{
	DWORD_PTR	dwSize;
	UINT_PTR	dwSetId;
	union
	{
		SP_LANG			lang;
		SP_ENC			enc;
		SP_NOTICE		notice;
		SP_HIGHLIGHT	highlight;
		SP_UNDERLINE	underline;
		SP_COREPATH		corepath;
		SP_WHITELIST	whitelist;
	};
};
struct RAPIDREPLACEPAIR
{
	WCHAR what[MAX_WORD+1];
	WCHAR by[MAX_WORD+1];
	RAPIDREPLACEPAIR* pPrev;
};
//! Transfer zone for Settings dialog
struct SPELLCHECKPARAM
{
	SIZE_T	dwSize;
	WCHAR*	pDict;
	WCHAR*	pJargon;
};
//! Forwarding
struct AspellStringEnumeration;
//! Transfer zone for suggestions dialog
struct SUGGESTDLGPARAM
{
	PLUGINDATA*			pd;
	UINT_PTR			uiCP;
	WCHAR				szUWord[MAX_WORD+1];
	char				szEWord[MAX_WORD*4+1];
	WCHAR				szSugg[MAX_WORD+1];
	AspellStringEnumeration *pASE;
};
//! Settings
struct SPELLCHECKSETTINGS
{
	WCHAR		szLanguage[MAX_PATH];
	char		szLanguageA[MAX_PATH];
	WCHAR		szJargon[MAX_PATH];
	char		szJargonA[MAX_PATH];
	//UINT_PTR	uiEncoding;
	BOOL		ShowNotice;
	BOOL		HighLight;
	BOOL		WhiteList;
	COLORREF	UnderlineColor;
	UINT_PTR	UnderlineStyle;
	WCHAR		szCorePath[MAX_PATHW];
	DWORD_PTR	dwHook_WM_XSCROLL;
	//NO BG
	WCHAR		szNoBG[MAX_PATHW];
	BOOL		bDelayBack;
	DWORD		nDelayTick;
};
//! Forwarding
struct AspellSpeller;
//! Dictionary info entry
struct DICINFO
{
	WCHAR		name[MAX_PATH];
	WCHAR		code[MAX_PATH];
	WCHAR		jargon[MAX_PATH];
	UINT_PTR	nEncoding;
	AspellSpeller *pSpell;
	BOOL		bNoBG;
};
//! Called when DLL loaded(DLL_PROCESS_ATTACH)
void CALLCONV OnAttach(HMODULE h);
void CALLCONV OnDetach(void);
//! Checker
BOOL CALLCONV IsLoaded(void);
//! Init ASpell core
BOOL CALLCONV InitSpellCore(PLUGINDATA *pd);
//! Unload ASpell
void CALLCONV UnloadSpellCore();
//! Worker functions
void CALLCONV DoCheckSelection(PLUGINDATA*pd,BOOL bRestoreSelection =TRUE, BOOL bForceNotice =FALSE);
void CALLCONV DoSettings(PLUGINDATA *pd);
BOOL CALLCONV DoBackground(PLUGINDATA *pd);
void CALLCONV DoSuggestion(PLUGINDATA *pd);

//! Execption catcher
//! Forced to define, because code is used four times, but cannot be separated as function
#define EINFOCOLLECTOR(ep,mod_addr)	\
		static WCHAR info[MAX_PATH*1024]=L"", temp[MAX_PATH]=L"";	\
		info[0]=0;temp[0]=0;	\
		swprintf(temp,L"Base: %08x ExceptionCode: %08x\n", mod_addr,GetExceptionCode());	\
		wcsncat(info,temp,sizeof(info)/sizeof(info[0])-1);	\
		if(ep)	\
		{	\
			if(ep->ContextRecord->ContextFlags & CONTEXT_DEBUG_REGISTERS)	\
			{	\
				swprintf(temp,L"CONTEXT_DEBUG_REGISTERS:\n\tDr0=%08x, Dr1=%08x\n\tDr2=%08x, Dr3=%08x\n\tDr6=%08x, Dr7=%08x\n",	\
				ep->ContextRecord->Dr0,ep->ContextRecord->Dr1,ep->ContextRecord->Dr2,ep->ContextRecord->Dr3,ep->ContextRecord->Dr6,ep->ContextRecord->Dr7);	\
				wcsncat(info,temp,sizeof(info)/sizeof(info[0])-1);	\
			}	\
			if(ep->ContextRecord->ContextFlags & CONTEXT_SEGMENTS)	\
			{	\
				swprintf(temp,L"CONTEXT_SEGMENTS\n\tGS: %08x FS: %08x ES: %08x DS: %08X\n",	\
				ep->ContextRecord->SegGs,ep->ContextRecord->SegFs,ep->ContextRecord->SegEs,ep->ContextRecord->SegDs);	\
				wcsncat(info,temp,sizeof(info)/sizeof(info[0])-1);	\
			}	\
			if(ep->ContextRecord->ContextFlags & CONTEXT_INTEGER)	\
			{	\
				swprintf(temp,L"CONTEXT_INTEGER\n\tEDI: %08x ESI: %08x\n\tEBX: %08x EDX: %08x\n\tECX: %08x EAX: %08x\n",	\
				ep->ContextRecord->Edi,ep->ContextRecord->Esi,ep->ContextRecord->Ebx,ep->ContextRecord->Edx,ep->ContextRecord->Ecx,ep->ContextRecord->Eax);	\
				wcsncat(info,temp,sizeof(info)/sizeof(info[0])-1);	\
			}	\
			if(ep->ContextRecord->ContextFlags & CONTEXT_CONTROL)	\
			{	\
				swprintf(temp,L"CONTEXT_CONTROL\n\tSS: %08x CS: %08x\n\tESP: %08x EBP: %08x\n\tEIP: %08x FLAGS: %08x\n",	\
				ep->ContextRecord->SegSs,ep->ContextRecord->SegCs,ep->ContextRecord->Esp,ep->ContextRecord->Ebp,ep->ContextRecord->Eip,ep->ContextRecord->EFlags);	\
				wcsncat(info,temp,sizeof(info)/sizeof(info[0])-1);	\
			}	\
		}

typedef void(WINAPI * _ExitProcess_t)(UINT);
struct runtime
{
	//! Common vars. Declared public here. Refrenced as external elsewhere.
	HWND cmn_hMainWindow;//	= NULL;
	int cmn_nMDI;		//	= WMD_SDI;
	//! Global Data
	CAspellModule	*pASpell;
	WCHAR			plugname[MAX_PATH];
	HMODULE			hModule;
	SPELLCHECKSETTINGS setting;
	//! Work Data
	WNDPROCDATA* EditProcData;
	RECT		g_rcCheckPos;
	//! Dictionary listing
	struct DICINFO	DICTIONARIES[256];
	UINT		DICTIONARY_COUNT;
	//! In-dictionary pointer
	UINT nLastDict;

///// WHITELIST
	//! Set of known whitelists
	PRULE	SpellCheckRules;
	PRULE	SpellCheckRulesOperative;
	//! Set of known edit assoccs
	LPEDITASSOC	EditAssoc;
	//! MainProcData
	WNDPROCDATA *NewMainProcData;
	WNDPROCDATA *NewFrameProcData;
#ifdef REDRAW_DELAY
	UINT_PTR	redrawtimer;
	int			drawflag;
	//0 - draw
	//1 - delay
	//-1 - not draw next tick
#endif
	_ExitProcess_t oldExitProcess;
};

extern "C" struct runtime rt;

#if !defined(_DEBUG) && !defined(_WIN64)
#define SPELLCHECK_ENABLETRAPS
#endif

#pragma intrinsic(memset,memcpy)

#define IS_ACTIVE() (rt.EditProcData!=NULL)

HRESULT PatchIat(
	HMODULE Module,
	PSTR ImportedModuleName,
	PSTR ImportedProcName,
	PVOID AlternateProc,
	PVOID *OldProc
	);
#define WRAPCALL(stat,estat) {try{stat;}catch(Excp*e){estat;delete e;}}
class Excp{
public:
	LPVOID dummy;
	Excp(LPVOID dum)
	{
		dummy=dum;
	}
};
#endif //__COMMON_H__


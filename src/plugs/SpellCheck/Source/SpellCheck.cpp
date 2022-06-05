/**
 *		SpellChecker for AkelPad
 *		Panych Y.W. aka FeyFre (c) 2010-2014 (panych.y@gmail.com)
 *
 *		This piece of software is distributed under BSD license.
 *		AkelPad is property of its copyright holders.
 *		The New Aspell Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
 *		license version 2.0 or 2.1.
 */

#include "common.h"

//! Used only in special builds for debug purposes
#if 0
#include <dbghelp.h>
#define MINIDUMPCREATOR(ep) \
		HMODULE hDLL = LoadLibraryW(dbghlp);	\
		if(!hDLL) hDLL = LoadLibraryW(L"dbghelp.dll");	\
		if(hDLL)	\
		{	\
			MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDLL, "MiniDumpWriteDump" );	\
			if(pDump)	\
			{	\
				DeleteFile(logfile);	\
				HANDLE hFile = CreateFileW(logfile,GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);	\
				if(hFile != (HANDLE)INVALID_HANDLE_VALUE)	\
				{	\
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;	\
					ExInfo.ThreadId = ::GetCurrentThreadId();	\
					ExInfo.ExceptionPointers = ep;	\
					ExInfo.ClientPointers = NULL;	\
					pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );	\
					CloseHandle(hFile);	\
				}	\
			}	\
			FreeLibrary(hDLL);	\
		}
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
									CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
									CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
									CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
									);
#else
#define MINIDUMPCREATOR(ep)
#endif




//Identification
extern "C" void __declspec(dllexport) DllAkelPadID(PLUGINVERSION *pv)
{
	pv->dwAkelDllVersion=AKELDLL;
	pv->dwExeMinVersion3x=MAKE_IDENTIFIER(-1, -1, -1, -1);
	pv->dwExeMinVersion4x=MAKE_IDENTIFIER(4, 8, 8, 0);
	pv->pPluginName="SpellCheck";
}
//Entry point
extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID/* lpvReserved*/)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		OnAttach(hinstDLL);
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
		OnDetach();
	}
	return TRUE;
}

//Plugin extern functions
extern "C" void __declspec(dllexport) CheckSelection(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI|PDS_NOAUTOLOAD;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;
	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	if(!IsLoaded()) InitSpellCore(pd);
	if (pd->bOnStart)
	{
		pd->nUnload = IsLoaded()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
		return;
	}
	if(!IsLoaded())
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	pd->nUnload = UD_NONUNLOAD_NONACTIVE;
	HWND hFocus = GetFocus();
#ifdef SPELLCHECK_ENABLETRAPS
	LPEXCEPTION_POINTERS ep=NULL;
	__try
	{
#endif
		DoCheckSelection(pd);
		SetFocus(hFocus);
#ifdef SPELLCHECK_ENABLETRAPS
	}
	__except((ep = GetExceptionInformation())?1:0)
	{
		EINFOCOLLECTOR(ep,(ULONG)pd->hInstanceDLL)
		MessageBoxW(pd->hMainWnd,info,L"Spellcheck error",MB_OK|MB_ICONINFORMATION);
		MINIDUMPCREATOR(ep)
		SetFocus(hFocus);
	}
#endif
	pd->nUnload = IS_ACTIVE()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
}
extern "C" void __declspec(dllexport) Settings(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI|PDS_NOAUTOLOAD;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;
	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	BOOL bWasLoaded = IsLoaded();
	if(!IsLoaded()) InitSpellCore(pd);
	if (pd->bOnStart)
	{
		pd->nUnload = IsLoaded()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
		return;
	}
	pd->nUnload = bWasLoaded?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;

	HWND hFocus = GetFocus();
#ifdef SPELLCHECK_ENABLETRAPS
	LPEXCEPTION_POINTERS ep = NULL;
	__try
	{
#endif
		DoSettings(pd);
		SetFocus(hFocus);
#ifdef SPELLCHECK_ENABLETRAPS
	}
	__except((ep = GetExceptionInformation())?1:0)
	{
		EINFOCOLLECTOR(ep,(ULONG)pd->hInstanceDLL)
		MessageBoxW(pd->hMainWnd,info,L"Spellcheck error",MB_OK|MB_ICONINFORMATION);
		MINIDUMPCREATOR(ep)
		SetFocus(hFocus);
	}
#endif
	pd->nUnload = IS_ACTIVE()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
}
extern "C" void __declspec(dllexport) CheckDocument(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI|PDS_NOAUTOLOAD;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;

	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	if(!IsLoaded()) InitSpellCore(pd);
	if (pd->bOnStart)
	{
		pd->nUnload = IsLoaded()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
		return;
	}
	if(!IsLoaded())
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	pd->nUnload = UD_NONUNLOAD_NONACTIVE;

	if(!IsWindow(pd->hWndEdit))
		return;
	HWND hFocus = GetFocus();
	AEPOINT *beg=NULL, *end=NULL;
	AEPOINT inn;
	CHARRANGE64 cr={0,-1};
	AECHARRANGE cro;
	SendMessageW(pd->hWndEdit,AEM_EXGETSEL,(WPARAM)&cro.ciMin,(LPARAM)&cro.ciMax);
	inn.ciPoint = cro.ciMin;
	inn.nPointOffset = AEPTO_IGNORE;
	inn.nPointLen = inn.dwFlags = inn.dwUserData = 0;
	beg = (AEPOINT*)SendMessageW(pd->hWndEdit,AEM_ADDPOINT,0,(LPARAM)&inn);
	inn.ciPoint = cro.ciMax;
	inn.nPointOffset = AEPTO_IGNORE;
	inn.nPointLen = inn.dwFlags = inn.dwUserData = 0;
	end = (AEPOINT*)SendMessageW(pd->hWndEdit,AEM_ADDPOINT,0,(LPARAM)&inn);
	SendMessageW(pd->hWndEdit,EM_EXSETSEL64,0,(LPARAM)&cr);
#ifdef SPELLCHECK_ENABLETRAPS
	LPEXCEPTION_POINTERS ep=NULL;
	__try
	{
#endif
		DoCheckSelection(pd,FALSE);
		SetFocus(hFocus);
#ifdef SPELLCHECK_ENABLETRAPS
	}
	__except((ep = GetExceptionInformation())?1:0)
	{
		EINFOCOLLECTOR(ep,(ULONG)pd->hInstanceDLL)
		MessageBoxW(pd->hMainWnd,info,L"Spellcheck error",MB_OK|MB_ICONINFORMATION);
		MINIDUMPCREATOR(ep)
		SetFocus(hFocus);
	}
#endif
	if(beg && end)
		SendMessageW(pd->hWndEdit,AEM_EXSETSEL,(WPARAM)&beg->ciPoint,(LPARAM)&end->ciPoint);
	if(beg) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)beg);
	if(end) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)end);
	pd->nUnload = IS_ACTIVE()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
}
extern "C" void __declspec(dllexport) Background(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;

	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	if(!IsLoaded()) InitSpellCore(pd);
	if(!IsLoaded())
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	pd->nUnload = UD_NONUNLOAD_NONACTIVE;

	BOOL b = FALSE;
#ifdef SPELLCHECK_ENABLETRAPS
	LPEXCEPTION_POINTERS ep=NULL;
	__try
	{
#endif
		b = DoBackground(pd);
#ifdef SPELLCHECK_ENABLETRAPS
	}
	__except((ep = GetExceptionInformation())?1:0)
	{
		EINFOCOLLECTOR(ep,(ULONG)pd->hInstanceDLL)
		MessageBoxW(pd->hMainWnd,info,L"Spellcheck error",MB_OK|MB_ICONINFORMATION);
		MINIDUMPCREATOR(ep)
		pd->nUnload = UD_NONUNLOAD_UNCHANGE;
	}
#endif
	pd->nUnload = b?UD_NONUNLOAD_ACTIVE:(IS_ACTIVE()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD);
}
extern "C" void __declspec(dllexport) Suggest(PLUGINDATA *pd)
{
	pd->dwSupport |= PDS_NOANSI|PDS_NOAUTOLOAD;
	if(pd->dwSupport & PDS_GETSUPPORT)
		return;

	if(pd->bOldWindows)
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	if(!IsLoaded()) InitSpellCore(pd);
	if (pd->bOnStart)
	{
		pd->nUnload = IsLoaded()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
		return;
	}
	if(!IsLoaded())
	{
		pd->nUnload = UD_UNLOAD;
		return;
	}
	pd->nUnload = UD_NONUNLOAD_NONACTIVE;
	if(!IsWindow(pd->hWndEdit))
		return;
	HWND hFocus = GetFocus();
#ifdef SPELLCHECK_ENABLETRAPS
	LPEXCEPTION_POINTERS ep=NULL;
	__try
	{
#endif
		DoSuggestion(pd);
		SetFocus(hFocus);
#ifdef SPELLCHECK_ENABLETRAPS
	}
	__except((ep = GetExceptionInformation())?1:0)
	{
		EINFOCOLLECTOR(ep,(ULONG)pd->hInstanceDLL)
		MessageBoxW(pd->hMainWnd,info,L"Spellcheck error",MB_OK|MB_ICONINFORMATION);
		MINIDUMPCREATOR(ep)
		SetFocus(hFocus);
	}
#endif
	pd->nUnload = IS_ACTIVE()?UD_NONUNLOAD_NONACTIVE:UD_UNLOAD;
}

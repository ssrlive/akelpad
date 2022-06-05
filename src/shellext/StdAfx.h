// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__D58502DF_FF80_4820_A5C6_2B7561220E13__INCLUDED_)
#define AFX_STDAFX_H__D58502DF_FF80_4820_A5C6_2B7561220E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <shlobj.h> 
#include <comdef.h> 
#include <shellapi.h>
#include <shlwapi.h>

#include <atlwin.h> 
#include <commctrl.h> 

#include <string> 
#include <list> 
#include <map> 
#include <vector>

#ifndef _tstring 
typedef std::basic_string<TCHAR> _tstring; 
#endif  // _tstring 

#ifndef _tstring_list 
typedef std::list< _tstring > _tstring_list; 
#endif  // _tstring_list 


extern TCHAR g_szModuleFileName[MAX_PATH]; 


#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0])) 
#endif 


#endif // !defined(AFX_STDAFX_H__D58502DF_FF80_4820_A5C6_2B7561220E13__INCLUDED)

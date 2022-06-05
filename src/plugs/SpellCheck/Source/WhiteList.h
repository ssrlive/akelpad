#ifndef __WHITELIST_H__
#define __WHITELIST_H__
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

#pragma pack(1)

//! Init default whitelist for edit
void CALLCONV InitAssoc(HWND hEditWnd);
//! Set custom whitelist for edit
void CALLCONV SetAssoc(HWND hEditWnd, LPCWSTR pExt);
//! Destroy whitelist-edit association
void CALLCONV DestroyAssoc(FRAMEDATA *pFD);

//! Init whitelists
void CALLCONV InitWhiteList(LPCWSTR szAkelDir);
//! Destroy whitelists
void CALLCONV FreeWhiteList(void);
//! Check word in whitelist
BOOL CALLCONV CheckWhiteList(LPCWSTR szWord, HWND hEditWnd);
//! Add new word to whitelist
void CALLCONV ParseExternalRule(LPCWSTR rule);

#define WL_SETEXT				0
#define WL_PARSERULE			1
#include <pshpack4.h>
struct BaseParam
{
	SIZE_T		dwSize;
	INT_PTR		nCode;
};
typedef BaseParam FAR*LPBASEPARAM;
//! Param struct for custom assoc assigments
typedef struct WLSETTER: BaseParam
{
	WCHAR*		szExt;
}
FAR * LPWLSETTER;
typedef struct WLPARSERULE: BaseParam
{
	LPWSTR	szRule;
}
FAR * LPWLPARSERULE;
#include <poppack.h>

#endif //__WHITELIST_H__

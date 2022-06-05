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
#include "WhiteList.h"

#define NO_ASSOC_SET_RULE	((PRULE)-1)
#define WORDBLOCK			(2048)

//! Path to white-list files
#define WHL_ROOT L"\\AKELFILES\\PLUGS\\SPELLCHECK"
//! Mask
#define WHL_MASK L"*.SPCK"
#define WHL_DOTPOS (-4)

//! Control chars
#define CC_COMMENT			L';'
#define CC_ADDEXT			L'+'
#define CC_REMEXT			L'-'
#define CC_CASEINSENS		L'|'

//! Get assocc rule for given edit
static PRULE	CALLCONV GetAssoc(HWND hEditWnd);
//! Main window proc callback
static LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//! Find rule by ext
PRULE CALLCONV FindRule(LPCWSTR szExt);
//! Ensure rule is valid
static BOOL CALLCONV EnsureRule(PRULE hRule);
static void CALLCONV ParseRule(LPCWSTR pFile, BOOL bFile=TRUE);

static WCHAR* CALLCONV make_string(LPCWSTR buff, SIZE_T beg, SIZE_T end)
{
	WCHAR *t = new WCHAR[end-beg+1];
	for(SIZE_T i=0;i<end-beg;i++)
		t[i]=buff[beg+i];
	t[end-beg]=0;
	return t;
}
static void CALLCONV free_string(WCHAR* buff)
{
	delete[] buff;
}
static SIZE_T CALLCONV FindEL(LPCWSTR pBuffer, SIZE_T b, SIZE_T e)
{
	while(b<e)
	{
		if(pBuffer[b]==L'\r' || pBuffer[b]==L'\n')
			break;
		else b++;
	}
	return b;
}
static SIZE_T CALLCONV SkipWS(LPCWSTR pBuffer, SIZE_T pos, SIZE_T hi)
{
	while(pos < hi)
	{
		register WCHAR ch = pBuffer[pos];
		if(ch == L'\r' || ch ==L'\n' || ch ==L'\t' || ch ==L' ')
			++pos;
		else break;
	}
	return pos;
}
static SIZE_T CALLCONV FindWS(LPCWSTR pCont, SIZE_T beg, SIZE_T end)
{
	while(beg < end)
	{
		if(pCont[beg]==L' ' || pCont[beg]==L'\t')
			break;
		beg++;
	}
	return beg;
}
static void CALLCONV AddExtension(PRULE rule)
{
	rule->pNextRule = rt.SpellCheckRules;
	rt.SpellCheckRules = rule;
}
static PRULE CALLCONV CreateRule(WCHAR* ext)
{
	PRULE rule = new RULE;
	wcsncpy(rule->ext,ext,127);
	rule->szWhiteList = new WCHAR[WORDBLOCK];
	rule->szWhiteList[0] = L'|';
	rule->szWhiteList[1] = 0;
	rule->szWhiteListS = new WCHAR[WORDBLOCK];
	rule->szWhiteListS[0] = L'|';
	rule->szWhiteListS[1] = 0;
	rule->szWL = rule->szWLS = WORDBLOCK;
	rule->fWL = rule->fWLs = 2;
	rule->pNextRule = rule->pNextRuleOperative = NULL;
	return rule;
}
static void CALLCONV RemoveExtensionFromOperative(WCHAR*ext)
{
	if(!rt.SpellCheckRulesOperative) return;
	PRULE rule = FindRule(ext);
	if(!rule) return;
	PRULE prevrule = NULL;
	PRULE currrule = rt.SpellCheckRulesOperative;
	while(currrule)
	{
		if(currrule == rule)
		{
			if(prevrule)
			{
				prevrule->pNextRuleOperative = currrule->pNextRuleOperative;
				currrule->pNextRuleOperative = NULL;
			}
			else
			{
				rt.SpellCheckRulesOperative = currrule->pNextRuleOperative;
				currrule->pNextRuleOperative = NULL;
			}
			return;
		}
		prevrule = currrule;
		currrule = currrule->pNextRuleOperative;
	}
}
static void CALLCONV AddExtensionToOperative(WCHAR* ext)
{
	PRULE rule2add = FindRule(ext);
	if(!rule2add)
	{
		rule2add = CreateRule(ext);
		AddExtension(rule2add);
	}
	PRULE scr_op = rt.SpellCheckRulesOperative;
	while(scr_op)
	{
		if(scr_op==rule2add)
			return;
		scr_op = scr_op->pNextRuleOperative;
	}
	rule2add->pNextRuleOperative = rt.SpellCheckRulesOperative;
	rt.SpellCheckRulesOperative = rule2add;
}
static void CALLCONV AddWord(PRULE rule, WCHAR* word, SIZE_T sz, BOOL bSens)
{
	if(bSens)
	{
		if(rule->fWLs+sz+1>rule->szWLS)
		{
			INT_PTR BLOCKS2ADD = (sz/WORDBLOCK) + ((sz%WORDBLOCK)?1:0);
			WCHAR *t = new WCHAR[rule->szWLS + BLOCKS2ADD*WORDBLOCK];
			CopyMemory(t,rule->szWhiteListS,sizeof(WCHAR)*rule->fWLs);
			delete[] rule->szWhiteListS;
			rule->szWhiteListS = t;
			rule->szWLS += BLOCKS2ADD*WORDBLOCK;
		}
		CopyMemory(rule->szWhiteListS+rule->fWLs-1, word, sizeof(WCHAR)*sz);
		rule->szWhiteListS[rule->fWLs+sz-1] = L'|';
		rule->szWhiteListS[rule->fWLs+sz] = 0;
		rule->fWLs += sz+1;
	}
	else
	{
		if(rule->fWL+sz+1>rule->szWL)
		{
			INT_PTR BLOCKS2ADD = (sz/WORDBLOCK) + ((sz%WORDBLOCK)?1:0);
			WCHAR *t = new WCHAR[rule->szWL + BLOCKS2ADD*WORDBLOCK];
			CopyMemory(t,rule->szWhiteList,sizeof(WCHAR)*rule->fWL);
			delete[] rule->szWhiteList;
			rule->szWhiteList = t;
			rule->szWL += BLOCKS2ADD*WORDBLOCK;
		}
		CopyMemory(rule->szWhiteList+rule->fWL-1, word, sizeof(WCHAR)*sz);
		rule->szWhiteList[rule->fWL+sz-1] = L'|';
		rule->szWhiteList[rule->fWL+sz] = 0;
		rule->fWL += sz+1;
	}
}
static void CALLCONV AddWordToOperative(WCHAR* word, SIZE_T sz, BOOL bSens)
{
	PRULE scr = rt.SpellCheckRulesOperative;
	while(scr)
	{
		AddWord(scr, word, sz, bSens);
		scr = scr->pNextRuleOperative;
	}
}
static void CALLCONV ParseLine(LPCWSTR pCont, SIZE_T beg, SIZE_T end)
{
	beg = SkipWS(pCont,beg,end);
	if(beg == end)	//! Empty line
		return;
	if(pCont[beg] == CC_COMMENT)	//! Comment line
		return;
	SIZE_T e = beg;
	if(pCont[beg] == CC_ADDEXT)
	{
		e = FindWS(pCont, beg+1, end);
		WCHAR* string = make_string(pCont, beg+1, e);
		CharUpperBuffW(string,e - beg);
		AddExtensionToOperative(string);
		free_string(string);
	}
	else if(pCont[beg] == CC_REMEXT)
	{
		e = FindWS(pCont, beg+1, end);
		WCHAR* string = make_string(pCont, beg+1, e);
		CharUpperBuffW(string,e - beg);
		RemoveExtensionFromOperative(string);
		free_string(string);
	}
	else if(pCont[beg] == CC_CASEINSENS)
	{
		e = FindWS(pCont, beg+1, end);
		WCHAR* string = make_string(pCont, beg+1, e);
		CharUpperBuffW(string,e-beg-1);
		AddWordToOperative(string, e-beg-1,FALSE);
		free_string(string);
	}
	else
	{
		e = FindWS(pCont, beg+1,end);
		WCHAR* string = make_string(pCont,beg,e);
		AddWordToOperative(string, e-beg,TRUE);
		free_string(string);
	}
}
static void CALLCONV ParseRule(LPCWSTR pFile, BOOL bFile)
{
	FILECONTENT fc;
	LRESULT lSize;
	LPCWSTR content = NULL;
	if(bFile)
	{
		DETECTFILEW	dc = {pFile,1024,ADT_BINARYERROR|ADT_DETECTBOM|ADT_DETECTCODEPAGE,0,FALSE};
		if(EDT_SUCCESS != SendMessageW(rt.cmn_hMainWindow, AKD_DETECTFILEW,0,(LPARAM)&dc))
			return;

		HANDLE hFile = CreateFileW(pFile, GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0);
		if(hFile == INVALID_HANDLE_VALUE)
			return;
		fc.hFile = hFile;
		fc.dwMax=(UINT_PTR)-1;
		fc.nCodePage=dc.nCodePage;
		fc.bBOM=dc.bBOM;
		fc.wpContent=NULL;
		lSize = SendMessageW(rt.cmn_hMainWindow,AKD_READFILECONTENT,0,(LPARAM)&fc);
		CloseHandle(hFile);
		content = fc.wpContent;
	}
	else
	{
		content = pFile;
		lSize = lstrlenW(pFile);
	}
	if(!lSize) return;
	SSIZE_T index = 0, indexEL = 0;
	while(index < lSize)
	{
		index = SkipWS(content, index, lSize);
		if(index == lSize) break;
		indexEL = FindEL(content, index, lSize);
		ParseLine(content,index, indexEL);
		index = indexEL;
	}
	if(bFile)
	{
		SendMessageW(rt.cmn_hMainWindow, AKD_FREETEXT,0,(LPARAM)fc.wpContent);
	}
	PRULE rule = rt.SpellCheckRulesOperative;
	while(rule)
	{
		PRULE r = rule->pNextRuleOperative;
		rule->pNextRuleOperative = NULL;
		rule = r;
	}
	rt.SpellCheckRulesOperative = NULL;
}
void CALLCONV InitWhiteList(LPCWSTR szAkelDir)
{
	rt.SpellCheckRules = NULL;
	rt.SpellCheckRulesOperative = NULL;
	rt.EditAssoc = NULL;
	WIN32_FIND_DATAW fd;
	WCHAR szMask[MAX_PATHW]=L"";
	WCHAR szRoot[MAX_PATHW]=L"";
	wcscpy(szMask, szAkelDir);
	wcscat(szMask, WHL_ROOT L"\\");
	wcscpy(szRoot, szMask);
	wcscat(szMask, WHL_MASK);
	//! Traverse all files
	HANDLE hFind = FindFirstFileW(szMask,&fd);
	while(hFind != INVALID_HANDLE_VALUE)
	{
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			SIZE_T t = wcslen(fd.cFileName);
			if( fd.cFileName[t + WHL_DOTPOS - 1] == L'.')
			{
				WCHAR *pFile = new WCHAR[wcslen(szRoot)+wcslen(fd.cFileName)+1];
				wcscpy(pFile,szRoot);
				wcscat(pFile,fd.cFileName);
				ParseRule(pFile);
				delete[] pFile;
			}
		}
		if(!FindNextFileW(hFind,&fd))
			break;
	}
	FindClose(hFind);

	rt.NewMainProcData=NULL;
	SendMessage(rt.cmn_hMainWindow, AKD_SETMAINPROC, (WPARAM)NewEditParentProc, (LPARAM)&rt.NewMainProcData);

	if (rt.cmn_nMDI == WMD_MDI)
	{
		rt.NewFrameProcData=NULL;
		SendMessageW(rt.cmn_hMainWindow, AKD_SETFRAMEPROC, (WPARAM)NewEditParentProc, (LPARAM)&rt.NewFrameProcData);
	}
}
void CALLCONV FreeWhiteList(void)
{
	//Remove subclass
	if (rt.NewMainProcData)
	{
		SendMessageW(rt.cmn_hMainWindow, AKD_SETMAINPROC, (WPARAM)NULL, (LPARAM)&rt.NewMainProcData);
		rt.NewMainProcData=NULL;
	}
	if (rt.cmn_nMDI == WMD_MDI)
	{
		if (rt.NewFrameProcData)
		{
			SendMessageW(rt.cmn_hMainWindow, AKD_SETFRAMEPROC, (WPARAM)NULL, (LPARAM)&rt.NewFrameProcData);
			rt.NewFrameProcData=NULL;
		}
	}
	while(rt.EditAssoc)
	{
		LPEDITASSOC p = rt.EditAssoc;
		rt.EditAssoc = p->next;
		delete p;
	}
	//! Free words lists
	while(rt.SpellCheckRules)
	{
		PRULE p = rt.SpellCheckRules;
		rt.SpellCheckRules = p->pNextRule;
		delete[] p->szWhiteList;
		delete[] p->szWhiteListS;
		delete p;
	}
}
//! Search for given word in whitelist
BOOL CALLCONV CheckWhiteList(LPCWSTR szWord, HWND hEditWnd)
{
	PRULE pRule = GetAssoc(hEditWnd);
	if(pRule == NO_ASSOC_SET_RULE)
	{
		InitAssoc(hEditWnd);
		pRule = GetAssoc(hEditWnd);
	}
	//! Ensure handle is valid
	if(!EnsureRule((PRULE)pRule))
		return FALSE;
	PRULE p = pRule;
	SIZE_T len = lstrlenW(szWord);
	WCHAR * pWork = new WCHAR[len+3];
	pWork[0] = '|'; pWork[1]=0;
	wcscat(pWork,szWord);
	wcscat(pWork,L"|");
	BOOL rez = !!(wcsstr(p->szWhiteListS,pWork));
	if(rez)
	{
		delete[] pWork;
		return TRUE;
	}
	CharUpperBuffW(pWork,len+2);
	//! Find word
	rez = !!(wcsstr(p->szWhiteList,pWork));
	delete[] pWork;
	return rez;
}
//! Find rule by ext
PRULE CALLCONV FindRule(LPCWSTR szExt)
{
	//! Traver rules list
	PRULE p = rt.SpellCheckRules;
	while(p)
	{
		if(p->ext && !lstrcmpW(p->ext,szExt))
			return p;
		p = p->pNextRule;
	}
	return NULL;
}
//! Validate rule
static BOOL CALLCONV EnsureRule(PRULE pRule)
{
	PRULE p = rt.SpellCheckRules;
	while(p)
	{
		if(p==pRule)
			return TRUE;
		p = p->pNextRule;
	}
	return FALSE;
}

void CALLCONV InitAssoc(HWND hEditWnd)
{
	FRAMEDATA * pFD = (FRAMEDATA*)SendMessageW(rt.cmn_hMainWindow,AKD_FRAMEFINDW,FWF_BYEDITWINDOW,(LPARAM)hEditWnd);
	if(!pFD) return;
	SIZE_T len = lstrlenW(pFD->wszFile);
	WCHAR * pExt = pFD->wszFile + len;
	while (pExt>pFD->wszFile)
	{
		if(*pExt==L'.')
		{
			pExt++;
			break;
		}
		if(*pExt==L'\\')
		{
			pExt = pFD->wszFile + len;
			break;
		}
		--pExt;
	}
	if(pExt == pFD->wszFile)
	{
		pExt = pFD->wszFile+len;
	}
	WCHAR *pTemp = new WCHAR[lstrlenW(pExt)+1];
	wcscpy(pTemp, pExt);
	CharUpperBuffW(pTemp,lstrlenW(pTemp));
	//! Find rule
	PRULE pRule = FindRule(pTemp);
	delete[] pTemp;
	PEDITASSOC assoc = rt.EditAssoc;
	while(assoc)
	{
		if(assoc->pFrame == pFD)
		{
			assoc->rule = pRule;
			break;
		}
		assoc = assoc->next;
	}
	if(!assoc)
	{
		assoc = new SEditAssoc;
		assoc->pFrame = pFD;
		assoc->rule = pRule;
		assoc->next = rt.EditAssoc;
		rt.EditAssoc = assoc;
	}
}
void CALLCONV SetAssoc(HWND hEditWnd, LPCWSTR pExt)
{
	FRAMEDATA* pFD = (FRAMEDATA*)SendMessageW(rt.cmn_hMainWindow,AKD_FRAMEFINDW,FWF_BYEDITWINDOW,(LPARAM)hEditWnd);
	if(!pFD) return;
	PRULE pRule = FindRule(pExt);
	PEDITASSOC assoc = rt.EditAssoc;
	while(assoc)
	{
		if(assoc->pFrame == pFD)
		{
			assoc->rule = pRule;
			break;
		}
		assoc = assoc->next;
	}
	if(!assoc)
	{
		assoc = new SEditAssoc;
		assoc->pFrame = pFD;
		assoc->rule = pRule;
		assoc->next = rt.EditAssoc;
		rt.EditAssoc = assoc;
	}
}
void CALLCONV DestroyAssoc(FRAMEDATA* pFD)
{
	if(!pFD)
	{
		pFD = (FRAMEDATA*)SendMessageW(rt.cmn_hMainWindow,AKD_FRAMEFINDW,FWF_BYINDEX,1);
	}
	LPEDITASSOC assoc = rt.EditAssoc;
	LPEDITASSOC assoc_prev = NULL;
	while(assoc)
	{
		if(assoc->pFrame == pFD)
		{
			if(assoc_prev != NULL)
			{
				assoc_prev = assoc->next;
			}
			else
			{
				rt.EditAssoc = assoc->next;
			}
			delete assoc;
			break;
		}
		assoc = assoc->next;
	}
}
static PRULE CALLCONV GetAssoc(HWND hEditWnd)
{
	FRAMEDATA* fd = (FRAMEDATA*)SendMessageW(rt.cmn_hMainWindow,AKD_FRAMEFINDW,FWF_BYEDITWINDOW,(LPARAM)hEditWnd);
	if(!fd) return NO_ASSOC_SET_RULE;
	LPEDITASSOC assoc = rt.EditAssoc;
	while(assoc)
	{
		if(assoc->pFrame == fd)
		{
			return assoc->rule;
		}
		assoc = assoc->next;
	}
	return NO_ASSOC_SET_RULE;
}


static LRESULT CALLBACK NewEditParentProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == AKDN_OPENDOCUMENT_FINISH)
	{
		if(lParam == EOD_SUCCESS)
		{
			InitAssoc((HWND)wParam);
		}
	}
	else if (uMsg == AKDN_SAVEDOCUMENT_FINISH)
	{
		if(lParam == ESD_SUCCESS)
		{
			InitAssoc((HWND)wParam);
		}
	}
	else if (uMsg == AKDN_FRAME_DESTROY)
	{
		DestroyAssoc((FRAMEDATA*)lParam);
	}
	if(uMsg == AKDN_FRAME_NOWINDOWS)
	{
		DestroyAssoc(NULL);
	}
#if 0
	if(uMsg == AEN_HSCROLL || uMsg == AEN_VSCROLL)
	{
		OutputDebugStringA("EditScroll\n");
	}
#endif
	if (hWnd == rt.cmn_hMainWindow)
		return rt.NewMainProcData->NextProc(hWnd, uMsg, wParam, lParam);
	else
		return rt.NewFrameProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void CALLCONV ParseExternalRule(LPCWSTR rule)
{
	ParseRule(rule,FALSE);
}
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
#include "SpellCore.h"
#include "ASpellModule.h"
#include "WhiteList.h"
#include "AkelEdit.h"

struct runtime rt;

//! Encodings item
struct langdef
{
	LPCSTR	szLang;
	int		eIndex;
};
//! Encodings for each language(a kind of autodetection_
static const langdef LangDef[] =
{
	//! Dictionary name->iso encoding map, i.e. "en" -> iso8859-1(CP_ISOBASE+1)
	{"fr",1},	{"es",1},	{"ca",1},	{"eu",1},	{"pt",1},	{"it",1},	{"sq",1},
	{"rm",1},	{"nl",1},	{"de",1},	{"da",1},	{"sv",1},	{"no",1},	{"fi",1},
	{"fo",1},	{"is",1},	{"ga",1},	{"gd",1},	{"en",1},	{"af",1},	{"sw",1},
	{"cs",2},	{"hu",2},	{"pl",2},	{"ro",2},	{"hr",2},	{"sk",2},	{"sl",2},
	{"eo",3},	{"mt",3},
	{"et",4},	{"lv",4},	{"lt",4},	{"kl",4},
	{"bg",5},	{"be",5},	{"mk",5},	{"ru",5},	{"sr",5},	{"uk",5},
	{"ar",6},	{"fa",6},	{"ur",6},
	{"el",7},
	{"iw",8},	{"ji",8},
	{"tu",9},
	//! Default encoding
	{NULL,1}
};
//! Encodings: will be passed into aspell
static const char * const encmap[]=
{
	"iso8859-0","iso8859-1","iso8859-2","iso8859-3",
	"iso8859-4","iso8859-5","iso8859-6","iso8859-7",
	"iso8859-8","iso8859-9","iso8859-10","iso8859-11",
	"iso8859-12","iso8859-13","iso8859-14","iso8859-15"
};
//! Encodings: will be used as labels
static const wchar_t * const encmapW[]=
{
	L"iso8859-0(dummy)",
	L"iso8859-1 Latin1 (West European)",
	L"iso8859-2 Latin2 (East European)",
	L"iso8859-3 Latin3 (South European)",
	L"iso8859-4 Latin4 (North European)",
	L"iso8859-5 Cyrillic",
	L"iso8859-6 Arabic",
	L"iso8859-7 Greek",
	L"iso8859-8 Hebrew",
	L"iso8859-9 Latin5 (Turkish)",
	L"iso8859-10 Latin6 (Nordic)",
	L"iso8859-11",
	L"iso8859-12",
	L"iso8859-13",
	L"iso8859-14",
	L"iso8859-15"
};
//! Commonly used, non word lexemes: number, date, etc
static BOOL CALLCONV CheckPattern(const WCHAR* word);
//! Dictionaries lifespan limitators
static BOOL CALLCONV InitDictList(void);
static void CALLCONV FreeDicts(void);
//! Text helpers
static BOOL CALLCONV IsDelimiter(HWND hWnd,const AECHARINDEX& cp);
static BOOL CALLCONV GetCurrentSel(PLUGINDATA *pd,AECHARRANGE& cr);
//! Settings helpers
static void CALLCONV LoadParam(PLUGINDATA *pd);
static void CALLCONV StoreParam(PLUGINDATA *pd);
//! DlgProcs
static INT_PTR CALLBACK SettingsDialog(HWND hwndDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK RapidCheckDialog(HWND hwndDlg,UINT uMsg,WPARAM wParam, LPARAM lParam);
//! AkelEdit/RichEdit proc
static LRESULT CALLBACK EditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void CALLCONV DrawStroke(HDC dc, INT_PTR y, INT_PTR xb, INT_PTR xe, INT_PTR fw = UNDERSCORE_SCALE, INT_PTR what=-1, COLORREF rgb=0);
static const DICINFO* CALLCONV GetDict(LPCWSTR dict, LPCWSTR jar);
static void ApplyNoBG(void);
#ifdef REDRAW_DELAY
static void CALLBACK RedrawTimer(HWND hwnd,UINT msg, UINT_PTR id, DWORD dwTime);
#endif
//! Resource getter
static LPWSTR CALLCONV GetString(UINT idr,LPWSTR pBuffer,DWORD dwSize)
{
	LoadStringW(rt.hModule,idr,pBuffer,dwSize);
	return pBuffer;
}
static BOOL CALLCONV IsGoodWord(const WCHAR* pWord)
{
	char Word[MAX_PATHW];
	UINT nCurrent = rt.nLastDict;
	BOOL bFirst = TRUE;
	while(bFirst || (nCurrent != rt.nLastDict))
	{
		const DICINFO *pCur = rt.DICTIONARIES+nCurrent;
		bFirst = FALSE;
		WideCharToMultiByte((UINT)pCur->nEncoding,0,pWord,-1,Word,MAX_PATHW-1," ",NULL);
		//! Trimming head, operaiong only on first word(for case when lpDefaultChar were used)
		char *head = Word;
		while(*head==' ') ++head;
		char *tail = head;
		while((*tail!=' ')&&(*tail!=0)) ++tail;
		*tail = 0;
		if(tail != head)
		{
			if(!pCur->bNoBG)
			{
				int good = rt.pASpell->aspell_speller_check(pCur->pSpell,head,-1);
				if(good)
				{
					rt.nLastDict = nCurrent;
					return TRUE;
				}
			}
		}
		nCurrent++;
		if(nCurrent>=rt.DICTIONARY_COUNT) nCurrent = 0;
	}
	return FALSE;
}
//! Traverse encoding map, to find given encoding, or at lest default one
static UINT CALLCONV FindSuitableEncodingIndex(const char* const lang)
{
	const langdef * l = LangDef;
	while(l->szLang)
	{
		if(!strncmp(l->szLang,lang,lstrlenA(l->szLang)))
		{
			return l->eIndex;
		}
		else
			l++;
	}
	//! Return default
	return l->eIndex;
}
void CALLCONV OnAttach(HMODULE hDLLModule)
{
	rt.cmn_hMainWindow = NULL;
	rt.cmn_nMDI = WMD_SDI;
	rt.pASpell = NULL;
	rt.nLastDict = 0;
	rt.NewMainProcData=rt.NewFrameProcData=NULL;
	CAspellModule::theModule=NULL;
	ZeroMemory(rt.DICTIONARIES,sizeof(rt.DICTIONARIES));
	rt.DICTIONARY_COUNT = 0;
	rt.hModule = hDLLModule;
	rt.oldExitProcess = NULL;
}
void CALLCONV OnDetach(void)
{
	UnloadSpellCore();
}
//! Invalidate edit window
static void CALLCONV UpdateEdit(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, TRUE);
}

static void CALLCONV UpdateEditAll(void)
{
	EDITINFO ei;
	BOOL bMaximized=TRUE;

	//if (cmn_nMDI == WMD_MDI)
	//	SendMessage(, WM_MDIGETACTIVE, 0, (LPARAM)&bMaximized);

	if (bMaximized)
	{
		if (SendMessage(rt.cmn_hMainWindow, AKD_GETEDITINFO, (WPARAM)NULL, (LPARAM)&ei))
		{
			if (ei.hWndMaster)
			{
				UpdateEdit(ei.hWndMaster);
				if (ei.hWndClone1)
					UpdateEdit(ei.hWndClone1);
				if (ei.hWndClone2)
					UpdateEdit(ei.hWndClone2);
				if (ei.hWndClone3)
					UpdateEdit(ei.hWndClone3);
			}
			else UpdateEdit(ei.hWndEdit);
		}
	}
	else
	{
		//nMDI == WMD_MDI in non-maximized state
		FRAMEDATA *lpFrameCount=(FRAMEDATA *)SendMessage(rt.cmn_hMainWindow, AKD_FRAMEFIND, FWF_CURRENT, 0);
		FRAMEDATA *lpFrameInit=lpFrameCount;
		while (lpFrameCount)
		{
			if (lpFrameCount->ei.hWndMaster)
			{
				UpdateEdit(lpFrameCount->ei.hWndMaster);
				if (lpFrameCount->ei.hWndClone1)
					UpdateEdit(lpFrameCount->ei.hWndClone1);
				if (lpFrameCount->ei.hWndClone2)
					UpdateEdit(lpFrameCount->ei.hWndClone2);
				if (lpFrameCount->ei.hWndClone3)
					UpdateEdit(lpFrameCount->ei.hWndClone3);
			}
			else UpdateEdit(lpFrameCount->ei.hWndEdit);
			if (lpFrameInit == (lpFrameCount=(FRAMEDATA *)SendMessage(rt.cmn_hMainWindow, AKD_FRAMEFIND, FWF_NEXT, (LPARAM)lpFrameCount)))
				break;
		}
	}
}
//! Do Settings
void CALLCONV DoSettings(PLUGINDATA *pd)
{
	BOOL bSave = FALSE;
	SPELLCHECKSETUPPARAM	*scsp=(SPELLCHECKSETUPPARAM*)pd->lParam;
	if(!scsp)
	{
		//! No aditional parameter used, do DialogBox
		INT_PTR dlgres = DialogBoxParamW(pd->hInstanceDLL,MAKEINTRESOURCEW(IDD_SETTINGS),pd->hMainWnd,SettingsDialog,(LPARAM)pd);
		bSave = (dlgres == 1);
	}
	else
	{
		//! Minimal param size
		if(scsp->dwSize < 8)
			return;
		//! Set specified setting
		switch(scsp->dwSetId)
		{
			case SPI_LANG:
				if(scsp->dwSize -8 < sizeof(SP_LANG)) return;
				lstrcpyW(rt.setting.szLanguage,scsp->lang.szLang);
				lstrcpyW(rt.setting.szJargon,scsp->lang.szJargon);
				WideCharToMultiByte(CP_ACP,0,rt.setting.szLanguage,-1,rt.setting.szLanguageA,MAX_PATH," ",NULL);
				WideCharToMultiByte(CP_ACP,0,rt.setting.szJargon,-1,rt.setting.szJargonA,MAX_PATH," ",NULL);
				bSave = TRUE;
				break;
			case SPI_NOTICE:
				if(scsp->dwSize -8 < sizeof(SP_NOTICE)) return;
				rt.setting.ShowNotice = (scsp->notice.uiNotice)?TRUE:FALSE;
				bSave = TRUE;
				break;
			case SPI_HIGHLIGHT:
				if(scsp->dwSize -8 < sizeof(SP_HIGHLIGHT)) return;
				rt.setting.HighLight = (scsp->highlight.uiHighLight)?TRUE:FALSE;
				bSave = TRUE;
				break;
			case SPI_WHITELIST:
				if(scsp->dwSize -8 < sizeof(SP_WHITELIST)) return;
				rt.setting.WhiteList = (scsp->whitelist.uiWhiteList)?TRUE:FALSE;
				bSave = TRUE;
				break;
			case SPI_UNDERLINE:
				if(scsp->dwSize -8 < sizeof(SP_UNDERLINE)) return;
				if(scsp->underline.dwStyle < SCUS_SOLID && scsp->underline.dwStyle >= SCUS_INVALIDSTYLE)
					return;
				rt.setting.UnderlineStyle = scsp->underline.dwStyle;
				rt.setting.UnderlineColor = (COLORREF)scsp->underline.dwColor;
				bSave = TRUE;
				break;
			case SPI_COREPATH:
				if(scsp->dwSize -8 < sizeof(SP_COREPATH)) return;
				lstrcpynW(rt.setting.szCorePath,scsp->corepath.szCorePath,sizeof(rt.setting.szCorePath)/sizeof(rt.setting.szCorePath[0])-1);
				bSave = TRUE;
				break;
			default:
				return;
		}
	}
	if(bSave)
	{
		//! Store params
		StoreParam(pd);
		//! Invalidate edit windows
		UpdateEditAll();
	}
}
//! Backhround checker starter/stopper
BOOL CALLCONV DoBackground(PLUGINDATA *pd)
{
	LPBASEPARAM param = (LPBASEPARAM)pd->lParam;
	if(param)
	{
		if(param->dwSize >= sizeof(BaseParam))
		{
			if(param->nCode == WL_SETEXT && param->dwSize == sizeof(WLSETTER))
			{
				LPWLSETTER wls = (LPWLSETTER)(pd->lParam);
				if(wls->szExt)
				{
					WCHAR szTemp[128]=L"";
					lstrcpynW(szTemp,wls->szExt,128);
					CharUpperBuffW(szTemp,lstrlenW(szTemp));
					SetAssoc(pd->hWndEdit,szTemp);
				}
				else
				{
					InitAssoc(pd->hWndEdit);
				}
			}
			else if(param->nCode == WL_PARSERULE && param->dwSize == sizeof(WLPARSERULE))
			{
				LPWLPARSERULE wlpr = (LPWLPARSERULE)pd->lParam;
				if(wlpr->szRule)
				{
					ParseExternalRule(wlpr->szRule);
				}
			}
		}
	}
	if(rt.EditProcData)
	{
		//! Switch only if do not have any parameters
		if(!param)
		{
			//! Already running. Turn off
			SendMessageW(pd->hMainWnd, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&rt.EditProcData);
			rt.EditProcData=NULL;
#ifdef REDRAW_DELAY
			if(rt.redrawtimer != 0)
			{
				KillTimer(rt.cmn_hMainWindow,rt.redrawtimer);
				rt.redrawtimer = 0;
			}
			rt.drawflag = 0;
#endif
			//UpdateEdit(pd->hWndEdit);
			UpdateEditAll();
			return FALSE;
		}
		else
		{
			//UpdateEdit(pd->hWndEdit);
			UpdateEditAll();
			return TRUE;
		}
	}
	else
	{
		SendMessageW(pd->hMainWnd, AKD_SETEDITPROC, (WPARAM)EditProcW, (LPARAM)&rt.EditProcData);
#ifdef REDRAW_DELAY
		if(rt.setting.bDelayBack)
		{
			rt.redrawtimer = SetTimer(rt.cmn_hMainWindow,0,rt.setting.nDelayTick,RedrawTimer);
			rt.drawflag = -1;
		}
#endif
		//UpdateEdit(pd->hWndEdit);
		UpdateEditAll();
		return TRUE;
	}
}
//! Find in momrized replace pair list
RAPIDREPLACEPAIR* CheckReplace(const WCHAR* bad_word,RAPIDREPLACEPAIR* list)
{
	while(list)
	{
		if(lstrcmpW(bad_word,list->what)==0) break;
		list = list->pPrev;
	}
	return list;
}
//! Core Function. Does most of stuff
void CALLCONV DoCheckSelection(PLUGINDATA*pd,BOOL bRestoreSelection, BOOL bForceNotice)
{
	//! Inline params
	SPELLCHECKPARAM* scp = (SPELLCHECKPARAM*)pd->lParam;
	if(scp && scp->dwSize != sizeof(SPELLCHECKPARAM))
		scp = NULL;
	//! Init sugestion dialog transfer area
	SUGGESTDLGPARAM sdp = {pd,DEF_ENCODING/*iso8859-1*/,L"","",L"",NULL};
	if(!IsWindow(pd->hWndEdit)) return;
	int		written = 0;
	//! Working stuff
	const DICINFO *di = NULL;
	if(scp)
	{
		di = GetDict(scp->pDict,scp->pJargon);
	}
	else
	{
		di = GetDict(rt.setting.szLanguage,rt.setting.szJargon);
	}
	if(di == NULL)
		return; //NO SPELLER. Nothing to do
	sdp.uiCP = di->nEncoding;
	AECHARINDEX begin;
	SendMessageW(pd->hWndEdit,AEM_GETINDEX,AEGI_FIRSTCHAR,(LPARAM)&begin);
	//! Old selection
	AECHARRANGE oldsel;
	oldsel.ciMin = oldsel.ciMax = begin;
	AEPOINT	*oselb = NULL,
			*osele = NULL;
	//INT_PTR deltasel = 0;
	if(bRestoreSelection)
	{
		SendMessageW(pd->hWndEdit,AEM_EXGETSEL,(WPARAM)&oldsel.ciMin,(LPARAM)&oldsel.ciMax);
		AEPOINT tpb,tpe;
		tpb.ciPoint = oldsel.ciMin;
		tpe.ciPoint = oldsel.ciMax;
		tpe.nPointOffset = tpb.nPointOffset = AEPTO_IGNORE;
		tpe.nPointLen    = tpb.nPointLen    = 0;
		tpe.dwFlags      = tpb.dwFlags      = 0;
		tpe.dwUserData   = tpb.dwUserData   = 0;
		oselb = (AEPOINT*)SendMessageW(pd->hWndEdit,AEM_ADDPOINT,0,(LPARAM)&tpb);
		osele = (AEPOINT*)SendMessageW(pd->hWndEdit,AEM_ADDPOINT,0,(LPARAM)&tpe);
	}
	AECHARRANGE cr;
	if(!GetCurrentSel(pd,cr))
	{
		if(oselb) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)oselb);
		if(osele) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)osele);
		return;
	}
	if(AEC_IndexCompare(&cr.ciMin,&cr.ciMax)>0)
	{
		AECHARINDEX temp = cr.ciMax;
		cr.ciMax = cr.ciMin;
		cr.ciMin = temp;
	}
	//! Word iteration
	AECHARINDEX current = cr.ciMin;
	AECHARINDEX wordstart, wordend;
	//RAPIDSPELLCHECK * rsc=NULL;
	BOOL donesome = FALSE;
	BOOL bFirst = TRUE;
	RAPIDREPLACEPAIR* pairs=NULL;
	AEPOINT *pointCur=NULL,
			*pointMax=NULL;
	{
		AEPOINT temp;
		temp.ciPoint = cr.ciMax;
		temp.dwFlags = temp.dwUserData = 0;
		temp.nPointLen = 0;
		temp.nPointLen = AEPTO_IGNORE;
		pointMax = (AEPOINT*)SendMessageW(pd->hWndEdit,AEM_ADDPOINT,0,(LPARAM)&temp);
		if(pointMax==NULL)
		{
			if(oselb) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)oselb);
			if(osele) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)osele);
			return;
		}
	}
	WCHAR buffer[MAX_WORD+1];
	while(1)
	{
		if(bFirst)
		{
			bFirst = FALSE;
			AECHARINDEX temp;
			AEC_NextCharEx(&cr.ciMin, &temp);
			SendMessageW(pd->hWndEdit,AEM_GETPREVBREAK,(WPARAM)AEWB_LEFTWORDSTART,(LPARAM)&temp);
			if(IsDelimiter(pd->hWndEdit,temp))
			{
				SendMessageW(pd->hWndEdit,AEM_GETNEXTBREAK,(WPARAM)AEWB_RIGHTWORDSTART,(LPARAM)&temp);
			}
			wordstart = temp;
			if(AEC_IndexCompare(&wordstart,&cr.ciMin)<0)
				continue;
			SendMessageW(pd->hWndEdit,AEM_GETNEXTBREAK,(WPARAM)AEWB_RIGHTWORDEND,(LPARAM)&temp);
			wordend = temp;
			if(AEC_IndexCompare(&wordend, &pointMax->ciPoint)>0) break;
			current = wordend;
		}
		else
		{
			AECHARINDEX temp;
			temp = current;
			SendMessageW(pd->hWndEdit,AEM_GETNEXTBREAK,(WPARAM)AEWB_RIGHTWORDSTART,(LPARAM)&temp);
			wordstart = temp;
			if(AEC_IndexCompare(&wordstart, &pointMax->ciPoint)>=0) break;
			SendMessageW(pd->hWndEdit,AEM_GETNEXTBREAK,(WPARAM)AEWB_RIGHTWORDEND,(LPARAM)&temp);
			wordend = temp;
			if(AEC_IndexCompare(&wordend, &pointMax->ciPoint)>0) break;
			current = wordend;
		}
		{
			AECHARRANGE url;
			if((SendMessageW(pd->hWndEdit,AEM_INDEXINURL,(WPARAM)&wordstart,(LPARAM)&url)!=0)
				|| (SendMessageW(pd->hWndEdit,AEM_INDEXINURL,(WPARAM)&wordend,(LPARAM)&url)!=0))
			{
				current = url.ciMax;
				continue;
			}
		}
		AETEXTRANGEW gtr;
			gtr.cr.ciMin		=wordstart;
			gtr.cr.ciMax		=wordend;
			gtr.bColumnSel		=FALSE;
			gtr.pBuffer			=NULL;
			gtr.dwBufferMax		=0;
			gtr.nNewLine		=AELB_ASOUTPUT;
			gtr.nCodePage		=0;
			gtr.lpDefaultChar	=NULL;
			gtr.bFillSpaces		=FALSE;
		LRESULT wl = SendMessageW(pd->hWndEdit,AEM_GETTEXTRANGEW,0,(LPARAM)&gtr);
		//! No more words. Quit.
		if(wl==1) break;
		if(wl>MAX_WORD+1) //! Words no more than 128 letters. No joke.
			continue;
		gtr.dwBufferMax = MAX_WORD+1;
		gtr.pBuffer = buffer;
		buffer[0]=0;
		SendMessageW(pd->hWndEdit,AEM_GETTEXTRANGE,0,(LPARAM)&gtr);
		written = WideCharToMultiByte((UINT)sdp.uiCP,0,(WCHAR*)gtr.pBuffer,-1,sdp.szEWord,1024," ",NULL);
		int good = CheckPattern(gtr.pBuffer) || rt.pASpell->aspell_speller_check(di->pSpell,sdp.szEWord,-1);
		if(good==0)
		{
			//! Bad word
			const AspellWordList *pAWL = rt.pASpell->aspell_speller_suggest(di->pSpell,sdp.szEWord,-1);
			INT_PTR dialogrez = IDCANCEL;
			if(pAWL)
			{
				//! Get suggestion enumerator
				sdp.pASE = rt.pASpell->aspell_word_list_elements(pAWL);
				RAPIDREPLACEPAIR *rep = CheckReplace(gtr.pBuffer, pairs);
				if(rep == NULL)
				{
					if(rt.setting.HighLight)
					{
						//! Highlight error if required
						AECHARRANGE lcr;
						lcr.ciMin=wordstart;
						lcr.ciMax=wordend;
						SendMessageW(pd->hWndEdit,AEM_EXSETSEL,(WPARAM)&(lcr.ciMin),(LPARAM)&(lcr.ciMax));
					}
					if((bForceNotice) || rt.setting.ShowNotice || !rt.pASpell->aspell_string_enumeration_at_end(sdp.pASE))
					{
						//! Show suggestion dialog if:
						//! Intended via usage of this function (bForceNotice)
						//! Set in settings regardless size suggestion list (setting.ShowNotice)
						//! Have any suggestion (!pASpell->aspell_string_enumeration_at_end(sdp.pASE))
						lstrcpynW(sdp.szUWord,(WCHAR*)gtr.pBuffer,sizeof(sdp.szUWord)/sizeof(sdp.szUWord[0]));
						dialogrez = DialogBoxParamW(pd->hInstanceDLL,MAKEINTRESOURCEW(IDD_RAPIDCHECK),pd->hMainWnd,RapidCheckDialog,(LPARAM)&sdp);
					}
				}
				else
				{
					dialogrez = IDOK;
				}
				//! Free enumerator
				rt.pASpell->delete_aspell_string_enumeration(sdp.pASE);
				// IDCANCEL = skip
				// IDOK = accept
				// IDC_CANCEL = cancel
				// IDC_MEM = memorize, not ask more in this session
				//! Accepted suggestion
				if(dialogrez==IDC_MEM)
				{
					rep = new RAPIDREPLACEPAIR;
					lstrcpyW(rep->what,gtr.pBuffer);
					lstrcpyW(rep->by,sdp.szSugg);
					rep->pPrev = pairs;
					pairs = rep;
					dialogrez = IDOK;
				}
				if(dialogrez==IDOK)
				{
					//! DO replacement
					AEPOINT temp;
					temp.ciPoint = current;
					temp.dwFlags = temp.dwUserData = 0;
					temp.nPointLen = 0;
					temp.nPointOffset = AEPTO_IGNORE;
					pointCur = (AEPOINT*)SendMessageW(pd->hWndEdit,AEM_ADDPOINT,0,(LPARAM)&temp);
					SendMessageW(pd->hWndEdit,AEM_EXSETSEL,(WPARAM)&wordstart,(LPARAM)&wordend);
					AEREPLACESELW repl = {NULL,-1,AELB_ASINPUT,0,NULL,NULL};
					if(rep)
						repl.pText = rep->by;
					else
						repl.pText = sdp.szSugg;
					SendMessageW(pd->hWndEdit,AEM_REPLACESELW,0,(LPARAM)&repl);
					donesome = TRUE;
					if(pointCur)
					{
						current = pointCur->ciPoint;
						SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)&pointCur);
						pointCur = NULL;
					}
				}
				//! Canceled
				if(dialogrez==IDC_CANCELALL)
				{
					gtr.pBuffer = NULL;
					gtr.dwBufferMax = 0;
					//! Prevent MessageBox
					donesome = TRUE;
					break;
				}
			}
			else
			{
				//! Error
				if(rt.setting.ShowNotice)
					MessageBoxA(pd->hMainWnd,rt.pASpell->aspell_speller_error_message(di->pSpell),"SpellChecker",MB_OK|MB_ICONERROR);
			}
		}
		gtr.pBuffer = NULL;
		gtr.dwBufferMax = 0;
	}
	//! If no replacement and not canceled
	if(!donesome)
	{
		//! Congrat user
		WCHAR buf[MAX_PATH];
		MessageBoxW(pd->hMainWnd,GetString(IDS_NOMISSSPELL,buf,MAX_PATH),L"SpellCheck",MB_OK|MB_ICONINFORMATION);
	}
	//! Free mem pairs
	while(pairs)
	{
		RAPIDREPLACEPAIR* p2k = pairs;
		pairs=pairs->pPrev;
		delete p2k;
	}
	if(bRestoreSelection)
	{
		if(oselb != NULL && osele != NULL)
		{
			oldsel.ciMin = oselb->ciPoint;
			oldsel.ciMax = osele->ciPoint;
			SendMessageW(pd->hWndEdit,AEM_EXSETSEL,(WPARAM)&(oldsel.ciMin),(LPARAM)&(oldsel.ciMax));
		}
	}
	if(oselb) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)oselb);
	if(osele) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)osele);
	if(pointMax) SendMessageW(pd->hWndEdit,AEM_DELPOINT,0,(LPARAM)pointMax);
}
BOOL CALLCONV IsLoaded(void)
{
	return rt.pASpell!=NULL;
}
static BOOL LoadASpell(LPCWSTR path, LPCWSTR ad, BOOL def)
{
	static TCHAR szModuleName[MAX_PATHW];
	BOOL bOk = FALSE;
	//! Locate ASpell
	if(path[0] == 0 && def && ad)
	{
		//! If empty - find in default location
		swprintf(szModuleName,INTERNAL_CORE_FORMAT, ad);
		bOk = CAspellModule::LoadModule(szModuleName);
	}
	else if((!wcsncmp(path,L"HKLM",4))||
			(!wcsncmp(path,L"HKCU",4)))
	{
		//! If begins with HKLM/HKCU - fetch from registry
		DWORD dwErr=0;
		HKEY hBaseKey = wcsncmp(path,L"HKLM",4)?HKEY_CURRENT_USER:HKEY_LOCAL_MACHINE;
		static WCHAR szFullKey[MAX_PATHW]=L"";
		lstrcpynW(szFullKey,path,lstrlenW(path)+1);
		if(lstrlenW(szFullKey)<6|| !wcschr(szFullKey,L'\\'))
		{
			bOk=FALSE;
		}
		else
		{
			//! Separate value name and path
			WCHAR *szKeyPath = szFullKey+5; //! i.e. HKLM\ size
			WCHAR *szValueName = szKeyPath+lstrlenW(szKeyPath);
			while(*szValueName!=L'\\')
				szValueName--;
			*szValueName=L'\0';
			szValueName++;
			HKEY hKey=NULL;
			//! Grab key
			if(ERROR_SUCCESS==(dwErr=RegOpenKeyExW(hBaseKey,szKeyPath,NULL,KEY_READ,&hKey)))
			{
				DWORD maxp=MAX_PATHW;
				DWORD dwType;
				static WCHAR szModulePath[MAX_PATHW];
				if(ERROR_SUCCESS==(dwErr=RegQueryValueExW(hKey,szValueName,NULL,&dwType,(LPBYTE)szModulePath,&maxp)))
				{
					if(dwType==REG_SZ)
					{
						//! End of value (for missed tailing \)
						size_t t = lstrlenW(szModulePath);
						if(t==0) t=1;
						swprintf(szModuleName, L"%s%s%s", szModulePath,(szModulePath[t-1]==L'\\')?L"":L"\\",CORE_DLL_NAME);
						bOk = CAspellModule::LoadModule(szModuleName);
					}
					else
						bOk=FALSE;
				}
				else
					bOk=FALSE;
				RegCloseKey(hKey);
			}
			else
				bOk = FALSE;
		}
		if(!bOk && def && ad)
		{
			//! Or setting was wrong - try to load ASpell from default place
			swprintf(szModuleName,INTERNAL_CORE_FORMAT,ad);
			bOk = CAspellModule::LoadModule(szModuleName);
		}
	}
	else
	{
		static WCHAR szModuleNameN[MAX_PATHW] = L"";
		//! If begins with .\ - relative location
		if(!wcsncmp(path,L".\\",2))
		{
			swprintf(szModuleNameN,L"%s\\%s",ad,path);
			bOk = CAspellModule::LoadModule(szModuleNameN);
		}
		else
		{
			//! Absolute path
			bOk = CAspellModule::LoadModule(path);
		}
		if(!bOk && def && ad)
		{
			//! Try to locate in default path
			swprintf(szModuleName,INTERNAL_CORE_FORMAT,ad);
			bOk = CAspellModule::LoadModule(szModuleName);
		}
	}
	if(!bOk)
	{
		//! Completely failed
		static WCHAR buf[MAX_PATH];
		//MessageBoxW(pd->hMainWnd,GetString(IDS_NOASPELL,buf,MAX_PATH),L"SpellCheck: error",MB_OK|MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}
BOOL CALLCONV InitSpellCore(PLUGINDATA *pd)
{
	//! Starting
	rt.cmn_hMainWindow = pd->hMainWnd;
	rt.cmn_nMDI = pd->nMDI;
	rt.EditProcData = NULL;
	rt.g_rcCheckPos.left = rt.g_rcCheckPos.top = rt.g_rcCheckPos.right = rt.g_rcCheckPos.bottom = -1;
	//! Defaults
	lstrcpynW(rt.setting.szLanguage,DEF_LANG,sizeof(rt.setting.szLanguage)/sizeof(rt.setting.szLanguage[0]));
	lstrcpynA(rt.setting.szLanguageA,DEF_LANGA,sizeof(rt.setting.szLanguageA)/sizeof(rt.setting.szLanguageA[0]));
	lstrcpynW(rt.setting.szJargon,DEF_JARGON,sizeof(rt.setting.szJargon)/sizeof(rt.setting.szJargon[0]));
	lstrcpynA(rt.setting.szJargonA,DEF_JARGONA,sizeof(rt.setting.szJargonA)/sizeof(rt.setting.szJargonA[0]));
	rt.setting.ShowNotice = DEF_NOTICE;
	rt.setting.HighLight = DEF_HIGHLIGHT;
	rt.setting.WhiteList = DEF_WHITELIST;
	rt.setting.UnderlineColor = DEF_UNDERLINECOLOR;
	rt.setting.UnderlineStyle = DEF_UNDERLINESTYLE;
	lstrcpynW(rt.setting.szCorePath,DEF_COREPATH,sizeof(rt.setting.szCorePath)/sizeof(rt.setting.szCorePath[0]));
	//! Parse plugin name
	lstrcpynW(rt.plugname,(WCHAR*)pd->pFunction,sizeof(rt.plugname)/sizeof(rt.plugname[0]));
	WCHAR * p = wcschr(rt.plugname,L':');
	if(*p) *p = L'\0';
	rt.setting.szNoBG[0]=0;
	rt.setting.bDelayBack = DEF_DELAYBACK;
	//! Load params
	LoadParam(pd);
	if(!LoadASpell(rt.setting.szCorePath,(LPCWSTR)pd->pAkelDir,TRUE))
		return FALSE;
	rt.pASpell = CAspellModule::GetModule();
	if(!rt.pASpell)
	{
		static WCHAR buf[MAX_PATH];
		//MessageBoxW(pd->hMainWnd,GetString(IDS_NOASPELL,buf,MAX_PATH),L"SpellCheck: error",MB_OK|MB_ICONERROR);
		return FALSE;
	}
	InitWhiteList((LPCWSTR)pd->pAkelDir);
	if(!InitDictList())
	{
		UnloadSpellCore();
		return FALSE;
	}
	return TRUE;
}
//! Unload ASpell
void CALLCONV UnloadSpellCore()
{
	if(rt.EditProcData)
	{
		SendMessageW(rt.cmn_hMainWindow, AKD_SETEDITPROC, (WPARAM)NULL, (LPARAM)&rt.EditProcData);
		rt.EditProcData=NULL;
	}
	FreeWhiteList();
	FreeDicts();
	if(!rt.pASpell) return;
	rt.pASpell = NULL;
	CAspellModule::UnLoadModule();
}
static void CALLCONV DrawStroke(HDC dc, INT_PTR y, INT_PTR xb, INT_PTR xe, INT_PTR fw, INT_PTR what, COLORREF rgb)
{
	//! Prevent hang when scrolling window using Mddle Mouse Click + Mouse Move
	//! Did not found yet where I get wrong coordinates
	//{
	//	if(xb >= xe)
	//	{
	//		return;
	//	}
	//	if(xb<0)
	//	{
	//		xb=0;
	//	}
	//	if(xe>32768)
	//	{
	//		xe = 32768;
	//	}
	//}
	fw -= 2;
	if(what == -1)
	{
		what = rt.setting.UnderlineStyle;
		rgb  = rt.setting.UnderlineColor;
	}
	if(what <= SCUS_LASTBUILTINSTYLE)
	{
		MoveToEx(dc,(int)xb,(int)y, NULL);
		LineTo(dc,  (int)xe,(int)y);
	}
	else
	{
		if(what == SCUS_COMPACTDOT)
		{
			xb+=(fw/(UNDERSCORE_SCALE)/2);
			while(xb < xe-(fw/(UNDERSCORE_SCALE-1)/2))
			{
				SetPixel(dc,(int)xb,(int)y, rgb);
				for(int i=0;i<(fw/(UNDERSCORE_SCALE))/2;i++)
				{
					MoveToEx(dc, (int)xb, (int)y-i,NULL);
					LineTo(dc, (int)xb+i, (int)y);
					LineTo(dc, (int)xb,   (int)y+i);
					LineTo(dc, (int)xb-i, (int)y);
					LineTo(dc, (int)xb,   (int)y-i);
				}
				xb += 2*(fw/(UNDERSCORE_SCALE))+1;
			}
		}
		else if( what == SCUS_WAVE)
		{
			MoveToEx(dc, (int)xb, (int)y, NULL);
			xb += 3*(fw/UNDERSCORE_SCALE);
			INT_PTR i = 1;
			while(xb < xe)
			{
				LineTo(dc, (int)xb, (int)(y - i*(fw/UNDERSCORE_SCALE+1)));
				xb += 3*(fw/UNDERSCORE_SCALE+1)-1;
				i = 1 - i;
			}
		}
	}
}
static void FillDictCombo(HWND hDictList)
{
	UINT i;
	static WCHAR t[MAX_PATH];
	//! Fill dictionary list
	SendMessageW(hDictList,CB_SETCURSEL,(WPARAM)-1,0);
	for(i=0;i<rt.DICTIONARY_COUNT;i++)
	{
		swprintf(t,L"%s",rt.DICTIONARIES[i].name);
		LRESULT n = SendMessageW(hDictList,CB_ADDSTRING,0,(LPARAM)t);
		if(CB_ERR == SendMessageW(hDictList,CB_SETITEMDATA,(WPARAM)n,(LPARAM)i))
		{
			//! Error?
			SendMessageW(hDictList,CB_DELETESTRING,(WPARAM)n,0);
			continue;
		}
		if(	lstrcmpW(rt.DICTIONARIES[i].code,rt.setting.szLanguage)==0 && 
			lstrcmpW(rt.DICTIONARIES[i].jargon,rt.setting.szJargon)==0)
		{
			//! Selected active
			SendMessageW(hDictList,CB_SETCURSEL,(WPARAM)n,0);
		}
	}
}
static void FillNoBGList(HWND hList)
{
	WCHAR t[MAX_PATH];
	WCHAR *head  = rt.setting.szNoBG;
	WCHAR *start = head;
	SendMessageW(hList,LB_RESETCONTENT,0,0);
	while(*head)
	{
		if(*head==L'|')
		{
			lstrcpynW(t,start,head-start+1);
			SendMessageW(hList,LB_ADDSTRING,0,(LPARAM)&t[0]);
			head++;
			start=head;
			continue;
		}
		head++;
	}
	if(start < head)
	{
			lstrcpynW(t,start,head-start+1);
			SendMessageW(hList,LB_ADDSTRING,0,(LPARAM)&t[0]);
	}
}
//! (re)Apply NoBG
static void ApplyNoBG(void)
{
	//! Mark as NoBG
	if(lstrlenW(rt.setting.szNoBG)>0)
	{
		UINT c;
		WCHAR *cache, *t, *r;
		for(c = 0; c< rt.DICTIONARY_COUNT;c++)
		{
			rt.DICTIONARIES[c].bNoBG = FALSE;
			t = rt.setting.szNoBG;
			cache = rt.DICTIONARIES[c].name;
			int len = lstrlenW(cache);
			while(1)
			{
				r = wcsstr(t,cache);
				//! not found
				if(r == NULL) break;
				//! found, or partial match
				if(
					(
						//!at the beginning
						  (r == rt.setting.szNoBG)
						&&
						//!at end   or  separator
						  (r[len]==0||r[len]==L'|')
						)
					||
					(
						//! not begin but separator
						(r > rt.setting.szNoBG  && r[-1]==L'|')
						&&
						//!at end   or  separator
						(r[len]==0||r[len]==L'|')
					)
					)
				{
					//! full match
					rt.DICTIONARIES[c].bNoBG = TRUE;
					break;
				}
				t++;
			}
		}
	}
}
//! Settings dialog
static INT_PTR CALLBACK SettingsDialog(HWND hwndDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	//! Controls
	static HWND hDictList;
	static HWND hShowNotice;
	static HWND hHighLight;
	static HWND hColor;
	static HWND hStyle;
	static HWND hCorePath;
	static HWND hWhiteList;
	static HWND hTest;
	//////
	static HWND hNoBGEdit,hNoBGList;
	//////
	static COLORREF color;
	static UINT uStyle;
	static PLUGINDATA* pd;
	static BOOL bInitialyHave;
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			pd = (PLUGINDATA*)lParam;
			hDictList = GetDlgItem(hwndDlg,IDC_DEFDICT);
			hShowNotice = GetDlgItem(hwndDlg,IDC_SHOWNOTICE);
			hHighLight = GetDlgItem(hwndDlg,IDC_HIGHLIGHT);
			hColor = GetDlgItem(hwndDlg,IDC_COLOR);
			hStyle = GetDlgItem(hwndDlg,IDC_STYLE);
			hCorePath = GetDlgItem(hwndDlg,IDC_CORE);
			hWhiteList = GetDlgItem(hwndDlg, IDC_WHITELIST);
			hTest = GetDlgItem(hwndDlg,IDC_PROBE);
			//// NO BG LIST
			hNoBGEdit = GetDlgItem(hwndDlg,IDC_NOBGEDIT);
			hNoBGList = GetDlgItem(hwndDlg,IDC_NOBG);
			SendMessageW(hNoBGEdit,EM_SETLIMITTEXT,255,0);
			////
			color = rt.setting.UnderlineColor;
			SendMessageW(hDictList,CB_RESETCONTENT,0,0);
			bInitialyHave = !!rt.pASpell;
			FillDictCombo(hDictList);
			FillNoBGList(hNoBGList);
			SendMessageW(hShowNotice,BM_SETCHECK,(WPARAM)(rt.setting.ShowNotice?BST_CHECKED:BST_UNCHECKED),0);
			SendMessageW(hHighLight,BM_SETCHECK,(WPARAM)(rt.setting.HighLight?BST_CHECKED:BST_UNCHECKED),0);
			SendMessageW(hWhiteList,BM_SETCHECK,(WPARAM)(rt.setting.WhiteList?BST_CHECKED:BST_UNCHECKED),0);
			//! Underline style
			SendMessageW(hStyle,CB_RESETCONTENT,0,0);
			LRESULT n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_SOLID);
			if(SCUS_SOLID == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
				n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_DASH);
			if(SCUS_DASH == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
				n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_DOT);
			if(SCUS_DOT == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
				n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_DASHDOT);
			if(SCUS_DASHDOT == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
				n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_DASHDOTDOT);
			if(SCUS_DASHDOTDOT == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
				n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_COMPACTDOT);
			if(SCUS_COMPACTDOT == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
				n = SendMessageW(hStyle,CB_ADDSTRING,0,SCUS_WAVE);
			if(SCUS_WAVE == rt.setting.UnderlineStyle) SendMessageW(hStyle,CB_SETCURSEL,n,0);
			//! Core path
			SendMessageW(hCorePath,WM_SETTEXT,0,(LPARAM)rt.setting.szCorePath);
			EnableWindow(hTest,!rt.pASpell);
			return TRUE;
		}
		case WM_COMMAND:
		{
			const UINT_PTR IDC = LOWORD(wParam), CODE=HIWORD(wParam);
			//! OK
			if(IDC==IDOK && CODE==BN_CLICKED)
			{
				//! Save data to structure
				LRESULT i = SendMessageW(hDictList,CB_GETCURSEL,0,0);
				i = SendMessageW(hDictList,CB_GETITEMDATA,(WPARAM)i,0);
				lstrcpynW(rt.setting.szLanguage,rt.DICTIONARIES[i].code,MAX_PATH);
				lstrcpynW(rt.setting.szJargon,rt.DICTIONARIES[i].jargon,MAX_PATH);
				WideCharToMultiByte(CP_ACP,0,rt.setting.szLanguage,-1,rt.setting.szLanguageA,MAX_PATH," ",NULL);
				WideCharToMultiByte(CP_ACP,0,rt.setting.szJargon,-1,rt.setting.szJargonA,MAX_PATH," ",NULL);
				rt.setting.ShowNotice = SendMessageW(hShowNotice,BM_GETCHECK,0,0)==BST_CHECKED?TRUE:FALSE;
				rt.setting.HighLight = SendMessageW(hHighLight,BM_GETCHECK,0,0)==BST_CHECKED?TRUE:FALSE;
				rt.setting.WhiteList = SendMessageW(hWhiteList,BM_GETCHECK,0,0)==BST_CHECKED?TRUE:FALSE;
				rt.setting.UnderlineColor = color;
				i = SendMessageW(hStyle,CB_GETCURSEL,0,0);
				rt.setting.UnderlineStyle = SendMessageW(hStyle,CB_GETITEMDATA,(WPARAM)i,0);
				SendMessageW(hCorePath,WM_GETTEXT,(WPARAM)sizeof(rt.setting.szCorePath)/sizeof(rt.setting.szCorePath[0])-1,(LPARAM)rt.setting.szCorePath);
				//! NOBG
				int count = SendMessageW(hNoBGList,LB_GETCOUNT,0,0);
				lstrcpyW(rt.setting.szNoBG,L"");
				WCHAR t[256];
				for(i=0;i<count;i++)
				{
					if(i>0) lstrcatW(rt.setting.szNoBG,L"|");
					t[0]=0;
					SendMessageW(hNoBGList,LB_GETTEXT,i,(LPARAM)&t[0]);
					lstrcatW(rt.setting.szNoBG,t);
				}
				ApplyNoBG();
				EndDialog(hwndDlg,1);
				return TRUE;
			}
			//! Cancel
			if(IDC==IDCANCEL && CODE==BN_CLICKED)
			{
				EndDialog(hwndDlg,0);
				return TRUE;
			}
			//! Color chooser
			if(IDC==IDC_COLOR && CODE==BN_CLICKED)
			{
				CHOOSECOLORW cc={0,0,0,0,0,0,0,0,0};
				//! Custom color lotery
				COLORREF c[16];
				cc.hInstance = NULL;
				cc.hwndOwner = hwndDlg;
				cc.lStructSize = sizeof(cc);
				cc.rgbResult = color;
				cc.Flags = CC_RGBINIT|CC_ANYCOLOR|CC_FULLOPEN;
				cc.lpCustColors = c;
				//! Select color
				if(ChooseColorW(&cc))
				{
					color = cc.rgbResult;
					RECT r;
					//! Force redraw Button and ComboBox
					GetClientRect(hwndDlg,&r);
					InvalidateRect(hwndDlg,&r,TRUE);
				}
				return TRUE;
			}
			//! File browser
			if(IDC==IDC_BROWSE && CODE==BN_CLICKED)
			{
				//! Any file. I actually do not care about any extesion.
				const WCHAR filter[]=L"aspell-15.dll\0aspell-15.dll\0All files\0*.*\0\0";
				static WCHAR filename[MAX_PATHW]={0};
				SendMessageW(hCorePath,WM_GETTEXT,(WPARAM)MAX_PATHW,(LPARAM)filename);
				OPENFILENAMEW ofn;
				ofn.lStructSize = sizeof(OPENFILENAMEW);
				ofn.hwndOwner = hwndDlg;
				ofn.hInstance = NULL;
				ofn.lpstrFilter = filter;
				ofn.lpstrCustomFilter = NULL;
				ofn.nMaxCustFilter = 0;
				ofn.nFilterIndex = 0;
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_PATHW;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle =0;
				ofn.lpstrInitialDir = filename;
				ofn.lpstrTitle = NULL;
				ofn.Flags = OFN_FILEMUSTEXIST|OFN_FORCESHOWHIDDEN|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_NODEREFERENCELINKS
					|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST;
				ofn.nFileOffset = ofn.nFileExtension = 0;
				ofn.lpstrDefExt = NULL;
				ofn.lCustData = 0;
				ofn.lpfnHook = 0;
				ofn.lpTemplateName = 0;
				ofn.pvReserved = 0;
				ofn.dwReserved = 0;
				ofn.FlagsEx = 0;
				if(!GetOpenFileNameW(&ofn)) return TRUE;
				SendMessageW(hCorePath,WM_SETTEXT,0,(LPARAM)ofn.lpstrFile);
				return TRUE;
			}
			if((IDC == IDC_PROBE) && (CODE==BN_CLICKED))
			{
				if(rt.pASpell)
					return TRUE;
				static WCHAR temp[MAX_PATHW];
				memset(temp,0,sizeof(temp));
				SendMessageW(hCorePath,WM_GETTEXT,MAX_PATHW-1,(LPARAM)temp);
				if(LoadASpell(temp,(LPCWSTR)pd->pAkelDir,FALSE))
				{
					FreeDicts();
					rt.pASpell = CAspellModule::GetModule();
					InitDictList();
					FillDictCombo(hDictList);
				}
				return TRUE;
			}
			if(IDC == IDC_NOBGADD && CODE == BN_CLICKED)
			{
				WCHAR t[MAX_PATH]={0};
				SendMessageW(hNoBGEdit,WM_GETTEXT,MAX_PATH-1,(LPARAM)&t[0]);
				SendMessageW(hNoBGList,LB_ADDSTRING,0,(LPARAM)&t[0]);
				return TRUE;
			}
			if(IDC == IDC_NOBGREM && CODE == BN_CLICKED)
			{
				int sel = SendMessageW(hNoBGList,LB_GETCURSEL,0,0);
				if(sel != LB_ERR)
				{
					SendMessageW(hNoBGList,LB_DELETESTRING,sel,0);
					if(sel == SendMessageW(hNoBGList,LB_GETCOUNT,0,0))
						sel--;
					if(sel != -1) SendMessageW(hNoBGList,LB_SETCURSEL,sel,0);
				}
				return TRUE;
			}
		}
		break;
		case WM_MEASUREITEM:
		{
			//! OwnerDrawn Combo
			if(wParam == IDC_STYLE)
			{
				LPMEASUREITEMSTRUCT mis = (LPMEASUREITEMSTRUCT)lParam;
				RECT r;
				::GetWindowRect(hStyle,&r);
				mis->itemHeight = 16;
				SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT,TRUE);
				return TRUE;
			}
		}
		break;
		case WM_DRAWITEM:
		{
			//! Draw Color Chooser Button
			if(wParam == IDC_COLOR)
			{
				LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
				HDC& dc = dis->hDC;
				RECT& r = dis->rcItem;
				HBRUSH hBrush,hOldBrush;
				LOGBRUSH logbr={BS_SOLID,color,0};
				hBrush = CreateBrushIndirect(&logbr);
				hOldBrush = (HBRUSH)SelectObject(dc,hBrush);
				Rectangle(dc,r.left,r.top,r.right,r.bottom);
				SelectObject(dc,hOldBrush);
				DeleteObject(hBrush);
				return TRUE;
			}
			//! Draw style combo ietms
			if(wParam == IDC_STYLE)
			{
				LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
				HDC& dc = dis->hDC;
				RECT& r = dis->rcItem;
				SetBkMode(dc,TRANSPARENT);
				HPEN hOldPen,hPen = CreatePen((int)((dis->itemData>SCUS_LASTBUILTINSTYLE)?PS_SOLID:dis->itemData),1,color);
				Rectangle(dc,r.left,r.top,r.right,r.bottom);
				hOldPen = (HPEN)SelectObject(dc,hPen);
				DrawStroke(dc, (r.top+r.bottom)/2, r.left+1, r.right-1, UNDERSCORE_SCALE+2, dis->itemData, color);
				/*MoveToEx(dc,r.left+1,(r.top+r.bottom)/2,NULL);
				LineTo(dc,r.right-1,(r.top+r.bottom)/2);*/
				SelectObject(dc,hOldPen);
				DeleteObject(hPen);
			}
		}
		break;
		case WM_CLOSE:
			//! Redirect to IDCANCEL command
			PostMessageW(hwndDlg,WM_COMMAND,MAKEWPARAM(IDCANCEL,BN_CLICKED),0);
			return TRUE;
		case WM_DESTROY:
			if(!bInitialyHave)
			{
				FreeDicts();
				rt.pASpell = NULL;
				CAspellModule::UnLoadModule();
			}
	}
	return FALSE;
}
static void CALLCONV LoadParam(PLUGINDATA *pd)
{
	HANDLE hOpt=NULL;
	hOpt = (HANDLE)SendMessageW(pd->hMainWnd,AKD_BEGINOPTIONS,(WPARAM)POB_READ,(LPARAM)rt.plugname);
	if(!hOpt)
	{
		lstrcpynW(rt.setting.szLanguage,DEF_LANG,sizeof(rt.setting.szLanguage)/sizeof(rt.setting.szLanguage[0]));
		lstrcpynA(rt.setting.szLanguageA,DEF_LANGA,sizeof(rt.setting.szLanguageA)/sizeof(rt.setting.szLanguageA[0]));
		lstrcpynW(rt.setting.szJargon,DEF_JARGON,sizeof(rt.setting.szJargon)/sizeof(rt.setting.szJargon[0]));
		lstrcpynA(rt.setting.szJargonA,DEF_JARGONA,sizeof(rt.setting.szJargonA)/sizeof(rt.setting.szJargonA[0]));
		rt.setting.ShowNotice = DEF_NOTICE;
		rt.setting.HighLight = DEF_HIGHLIGHT;
		rt.setting.WhiteList = DEF_WHITELIST;
		rt.setting.UnderlineColor = DEF_UNDERLINECOLOR;
		rt.setting.UnderlineStyle = DEF_UNDERLINESTYLE;
		rt.setting.dwHook_WM_XSCROLL = DEF_XSCROLL;
		return;
	}
	PLUGINOPTIONW po;
	LRESULT ds=1;

	po.pOptionName = SET_LANG;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szLanguage;
	po.dwData = sizeof(rt.setting.szLanguage);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		ds = 0;

	po.pOptionName = SET_JARGON;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szJargon;
	po.dwData = sizeof(rt.setting.szJargon);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		ds = 0;

	if(!ds)
	{
		lstrcpynW(rt.setting.szLanguage,DEF_LANG,MAX_PATH);
		lstrcpynW(rt.setting.szJargon,DEF_JARGON,MAX_PATH);
	}
	WideCharToMultiByte(CP_ACP,0,rt.setting.szLanguage,-1,rt.setting.szLanguageA,MAX_PATH," ",NULL);
	WideCharToMultiByte(CP_ACP,0,rt.setting.szJargon,-1,rt.setting.szJargonA,MAX_PATH," ",NULL);

	po.pOptionName = SET_NOTICE;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.ShowNotice;
	po.dwData = sizeof(rt.setting.ShowNotice);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.ShowNotice = DEF_NOTICE;

	po.pOptionName = SET_HIGHLIGHT;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.HighLight;
	po.dwData = sizeof(rt.setting.HighLight);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.HighLight = DEF_HIGHLIGHT;

	po.pOptionName = SET_WHITELIST;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.WhiteList;
	po.dwData = sizeof(rt.setting.WhiteList);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.WhiteList = DEF_WHITELIST;

	po.pOptionName = SET_UNDELINECOLOR;
	po.dwType = PO_BINARY;
	po.lpData = (LPBYTE)&rt.setting.UnderlineColor;
	po.dwData = sizeof(rt.setting.UnderlineColor);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.UnderlineColor = RGB(255,0,0);

	po.pOptionName = SET_UNDELINESTYLE;
	po.dwType = PO_BINARY;
	po.lpData = (LPBYTE)&rt.setting.UnderlineStyle;
	po.dwData = sizeof(rt.setting.UnderlineStyle);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.UnderlineStyle = DEF_UNDERLINESTYLE;

	po.pOptionName = SET_COREPATH;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szCorePath;
	po.dwData = sizeof(rt.setting.szCorePath);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		lstrcpynW(rt.setting.szCorePath,DEF_COREPATH,sizeof(rt.setting.szCorePath)/sizeof(rt.setting.szCorePath[0]));

	po.pOptionName = SET_WM_XSCROLL;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.dwHook_WM_XSCROLL;
	po.dwData = sizeof(rt.setting.dwHook_WM_XSCROLL);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.dwHook_WM_XSCROLL = DEF_XSCROLL;

	po.pOptionName = SET_NOBG;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szNoBG;
	po.dwData = sizeof(rt.setting.szNoBG);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		lstrcpynW(rt.setting.szNoBG,DEF_NOBG,sizeof(rt.setting.szNoBG)/sizeof(rt.setting.szNoBG[0]));

	po.pOptionName = SET_DELAYBACK;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.bDelayBack;
	po.dwData = sizeof(rt.setting.bDelayBack);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.HighLight = DEF_DELAYBACK;

	po.pOptionName = SET_DELAYTICK;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.nDelayTick;
	po.dwData = sizeof(rt.setting.nDelayTick);
	if(!SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po))
		rt.setting.HighLight = DEF_DELAYTICK;
	if(rt.setting.nDelayTick <50)
		rt.setting.nDelayTick = 50;

	SendMessageW(pd->hMainWnd,AKD_ENDOPTIONS,(WPARAM)hOpt,0);
}
static void CALLCONV StoreParam(PLUGINDATA *pd)
{
	HANDLE hOpt=NULL;
	hOpt = (HANDLE)SendMessageW(pd->hMainWnd,AKD_BEGINOPTIONS,(WPARAM)POB_SAVE,(LPARAM)rt.plugname);
	if(!hOpt)
		return;
	INT_PTR i=0;
	PLUGINOPTIONW po;
	po.pOptionName = SET_LANG;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szLanguage;
	po.dwData = (lstrlenW(rt.setting.szLanguage)+1)*sizeof(WCHAR);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_JARGON;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szJargon;
	po.dwData = (lstrlenW(rt.setting.szJargon)+1)*sizeof(WCHAR);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_NOTICE;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.ShowNotice;
	po.dwData = sizeof(rt.setting.ShowNotice);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_HIGHLIGHT;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.HighLight;
	po.dwData = sizeof(rt.setting.HighLight);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_WHITELIST;
	po.dwType = PO_DWORD;
	po.lpData = (LPBYTE)&rt.setting.WhiteList;
	po.dwData = sizeof(rt.setting.WhiteList);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_UNDELINECOLOR;
	po.dwType = PO_BINARY;
	po.lpData = (LPBYTE)&rt.setting.UnderlineColor;
	po.dwData = sizeof(rt.setting.UnderlineColor);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_UNDELINESTYLE;
	po.dwType = PO_BINARY;
	po.lpData = (LPBYTE)&rt.setting.UnderlineStyle;
	po.dwData = sizeof(rt.setting.UnderlineStyle);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);
	if(!((rt.setting.UnderlineStyle>=SCUS_SOLID)&&(rt.setting.UnderlineStyle<SCUS_INVALIDSTYLE)))
		rt.setting.UnderlineStyle = DEF_UNDERLINESTYLE;

	po.pOptionName = SET_COREPATH;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szCorePath;
	po.dwData = (lstrlenW(rt.setting.szCorePath)+1)*sizeof(WCHAR);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	po.pOptionName = SET_NOBG;
	po.dwType = PO_STRING;
	po.lpData = (LPBYTE)rt.setting.szNoBG;
	po.dwData = (lstrlenW(rt.setting.szNoBG)+1)*sizeof(WCHAR);
	i=SendMessageW(pd->hMainWnd,AKD_OPTION,(WPARAM)hOpt,(LPARAM)&po);

	SendMessageW(pd->hMainWnd,AKD_ENDOPTIONS,(WPARAM)hOpt,0);
}
static BOOL CALLCONV InitDictList(void)
{
	FreeDicts();
	BOOL failed = FALSE;
	if(!rt.pASpell) return FALSE;
	AspellConfig *pConf = rt.pASpell->new_aspell_config();
	if(!pConf) return FALSE;
	AspellDictInfoList *adil = rt.pASpell->get_aspell_dict_info_list(pConf);
	rt.pASpell->delete_aspell_config(pConf);
	pConf = NULL;
	if(!adil) return FALSE;
	AspellDictInfoEnumeration *adie = rt.pASpell->aspell_dict_info_list_elements(adil);
	const AspellDictInfo *entry=NULL;
	while(entry = rt.pASpell->aspell_dict_info_enumeration_next(adie))
	{
		if(entry->code)
			MultiByteToWideChar(CP_ACP,0,entry->code,lstrlenA(entry->code),rt.DICTIONARIES[rt.DICTIONARY_COUNT].code,MAX_PATH);
		if(entry->name)
			MultiByteToWideChar(CP_ACP,0,entry->name,lstrlenA(entry->name),rt.DICTIONARIES[rt.DICTIONARY_COUNT].name,MAX_PATH);
		if(entry->jargon)
			MultiByteToWideChar(CP_ACP,0,entry->jargon,lstrlenA(entry->jargon),rt.DICTIONARIES[rt.DICTIONARY_COUNT].jargon,MAX_PATH);
		AspellConfig *pConfig = rt.pASpell->new_aspell_config();
		if(pConfig)
		{
			UINT enc = FindSuitableEncodingIndex(entry->code);
			rt.pASpell->aspell_config_replace(pConfig,"lang",entry->code);
			rt.pASpell->aspell_config_replace(pConfig,"jargon",entry->jargon);
			rt.pASpell->aspell_config_replace(pConfig,"encoding",encmap[enc]);
			AspellSpeller *NewSp=NULL;
			AspellCanHaveError *ache = NULL;
			WRAPCALL(ache = rt.pASpell->new_aspell_speller(pConfig), failed=TRUE;break;);
			rt.pASpell->delete_aspell_config(pConfig); pConfig = NULL;
			if(rt.pASpell->aspell_error(ache)!=0)
			{
				rt.pASpell->delete_aspell_can_have_error(ache);
				continue;
			}
			NewSp = rt.pASpell->to_aspell_speller(ache);
			rt.DICTIONARIES[rt.DICTIONARY_COUNT].pSpell = NewSp;
			rt.DICTIONARIES[rt.DICTIONARY_COUNT].nEncoding = CP_ISOBASE + enc;
			rt.DICTIONARIES[rt.DICTIONARY_COUNT].bNoBG = FALSE;
		}
		rt.DICTIONARY_COUNT++;
	}
	if(failed)
	{
		FreeDicts();
		return FALSE;
	}
	ApplyNoBG();
	rt.pASpell->delete_aspell_dict_info_enumeration(adie);
	return TRUE;
}
static void CALLCONV FreeDicts(void)
{
	if(!rt.pASpell) return;
	UINT i = 0;
	for(i = 0; i < rt.DICTIONARY_COUNT; i++)
	{
		rt.pASpell->delete_aspell_speller(rt.DICTIONARIES[i].pSpell);
	}
	rt.DICTIONARY_COUNT=0;
	ZeroMemory(rt.DICTIONARIES,sizeof(rt.DICTIONARIES));
}
static BOOL CALLCONV GetCurrentSel(PLUGINDATA *pd,AECHARRANGE& cr)
{
	SendMessageW(pd->hWndEdit,AEM_GETINDEX,AEGI_FIRSTCHAR,(LPARAM)&cr.ciMin);
	cr.ciMax = cr.ciMin;
	LRESULT res = 0;
	EDITINFO editinfo;
	res = SendMessageW(pd->hMainWnd,AKD_GETEDITINFO,(WPARAM)pd->hWndEdit,(LPARAM)&editinfo);
	if(!res) return FALSE;
	AECHARRANGE lcr;
	if(!SendMessageW(pd->hWndEdit,AEM_EXGETSEL,(WPARAM)&lcr.ciMin,(LPARAM)&lcr.ciMax))
	{
		if(AEC_IndexCompare(&lcr.ciMin,&lcr.ciMax)>0)
		{
			AECHARINDEX temp = lcr.ciMax;
			lcr.ciMax = lcr.ciMin;
			lcr.ciMin = temp;
		}
		if(IsDelimiter(pd->hWndEdit,lcr.ciMin))
			return FALSE;
		AECHARINDEX temp = lcr.ciMin, wordstart, wordend;
		AEC_NextCharEx(&lcr.ciMin,&temp);
		SendMessageW(pd->hWndEdit,AEM_GETPREVBREAK,(WPARAM)AEWB_LEFTWORDSTART,(LPARAM)&temp);
		wordstart = temp;
		SendMessageW(pd->hWndEdit,AEM_GETNEXTBREAK,(WPARAM)AEWB_RIGHTWORDEND,(LPARAM)&temp);
		wordend = temp;
		if(AEC_IndexCompare(&wordstart,&wordend)==0) return FALSE;
		cr.ciMin = wordstart;
		cr.ciMax = wordend;
		SendMessageW(pd->hWndEdit,AEM_EXSETSEL,(WPARAM)&(cr.ciMin),(LPARAM)&(cr.ciMax));
		return TRUE;
	}
	cr.ciMin = lcr.ciMin;
	cr.ciMax = lcr.ciMax;
	return TRUE;
}
static INT_PTR CALLBACK RapidCheckDialog(HWND hwndDlg,UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	static HWND hEdit;
	static SUGGESTDLGPARAM *sdp;
	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			if(rt.g_rcCheckPos.left != -1)
			{
				SetWindowPos(hwndDlg, NULL,
						rt.g_rcCheckPos.left, rt.g_rcCheckPos.top,
						rt.g_rcCheckPos.right - rt.g_rcCheckPos.left, rt.g_rcCheckPos.bottom - rt.g_rcCheckPos.top,
						SWP_NOACTIVATE|SWP_NOZORDER);
			}
			sdp = (SUGGESTDLGPARAM*)lParam;
			//! ICON
			SendMessageW(hwndDlg,WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)sdp->pd->hMainIcon);
			//! ListBox
			hList = GetDlgItem(hwndDlg,IDC_SUGLIST);
			hEdit = GetDlgItem(hwndDlg,IDC_MISSSPELL);
			if((!IsWindow(hList))||(!IsWindow(hEdit)))
			{
				//! Failed
				EndDialog(hwndDlg,IDCANCEL);
				return TRUE;
			}
			//SendMessageW(hEdit,WM_SETTEXT,0,(LPARAM)sdp->szUWord);
			//! Paranoid cleaner
			SendMessageW(hList,LB_RESETCONTENT,0,0);
			//! Init suggest enumerator
			const char *sword;
			BOOL bSet = FALSE;
			while((sword = rt.pASpell->aspell_string_enumeration_next(sdp->pASE))!=0)
			{
				WCHAR uni[129]={0};
				INT_PTR written = MultiByteToWideChar((UINT)sdp->uiCP,0,sword,-1,uni,0);
				if(written>MAX_WORD);
				else
				{
					written = MultiByteToWideChar((UINT)sdp->uiCP,0,sword,-1,uni,(int)written);
					written = SendMessageW(hList,LB_ADDSTRING,0,(LPARAM)uni);
					if(!bSet)
					{
						SendMessageW(hList,LB_SETCURSEL,(WPARAM)written,0);
						SendMessageW(hEdit,WM_SETTEXT,0,(LPARAM)uni);
						bSet = TRUE;
					}
				}
			}
			if(SendMessageW(hList,LB_GETCOUNT,0,0)==0)
			{
				EndDialog(hwndDlg,IDCANCEL);
			}
			return TRUE;
		}
		break;
		case WM_COMMAND:
		{
			UINT IDC  = LOWORD(wParam);
			UINT CODE = HIWORD(wParam);
			if(IDC==IDCANCEL && CODE == BN_CLICKED)
			{
				//! Cancel
				EndDialog(hwndDlg,IDCANCEL);
				return TRUE;
			}
			if((IDC == IDOK || IDC == IDC_MEM) && CODE == BN_CLICKED)
			{
				//! GetSize
				//DWORD szs = SendMessageW(hEdit,WM_GETTEXTLENGTH,0,0);
				//! GetText
				SendMessageW(hEdit,WM_GETTEXT,MAX_PATH-1,(LPARAM)sdp->szSugg);
				//! End
				EndDialog(hwndDlg,IDC);
				return TRUE;
			}
			if(IDC == IDC_CANCELALL && CODE == BN_CLICKED)
			{
				EndDialog(hwndDlg,IDC_CANCELALL);
				return TRUE;
			}
			if(IDC == IDC_ORIGINAL && CODE == BN_CLICKED)
			{
				SendMessageW(hEdit,WM_SETTEXT,0,(LPARAM)sdp->szUWord);
				return TRUE;
			}
			if(IDC == IDC_SUGLIST)
			{
				if(CODE == LBN_SELCHANGE)
				{
					WCHAR *szSugg=NULL;
					LRESULT index = SendMessageW(hList,LB_GETCURSEL,0,0);
					if(index == LB_ERR) return TRUE;
					LRESULT len = SendMessageW(hList,LB_GETTEXTLEN,(WPARAM)index,0);
					if(len == LB_ERR) return TRUE;
					szSugg = new WCHAR[len+1];
					if(!szSugg) return TRUE;
					szSugg[0]=0;
					if(LB_ERR == SendMessageW(hList,LB_GETTEXT,(WPARAM)index,(LPARAM)szSugg))
					{
						delete[] szSugg;
						return TRUE;
					}
					SendMessageW(hEdit,WM_SETTEXT,0,(LPARAM)szSugg);
					delete[] szSugg;
					return TRUE;
				}
				if(CODE == LBN_DBLCLK)
				{
					PostMessageW(hwndDlg,WM_COMMAND,IDOK,0);
					return TRUE;
				}
			}
			return FALSE;
		}
		case WM_CLOSE:
			PostMessageW(hwndDlg,WM_COMMAND,MAKEWPARAM(IDC_CANCELALL,BN_CLICKED),0);
			return TRUE;
		case WM_DESTROY:
			GetWindowRect(hwndDlg,&rt.g_rcCheckPos);
	default:
		return FALSE;
	}
}
static BOOL CALLCONV IsDelimiter(HWND hWnd,const AECHARINDEX& cp)
{
	return (BOOL)SendMessageW(hWnd,AEM_ISDELIMITER,(WPARAM)AEDLM_WORD,(LPARAM)&cp);
}
static BOOL CALLCONV GetWord(HWND hWnd, AECHARINDEX& current, AECHARINDEX& start, AECHARINDEX& end)
{
	end = current;
	LRESULT res = SendMessageW(hWnd,AEM_GETNEXTBREAK,(WPARAM)AEWB_RIGHTWORDEND,(LPARAM)&end);
	start = end;
	SendMessageW(hWnd,AEM_GETPREVBREAK,(WPARAM)AEWB_LEFTWORDSTART,(LPARAM)&start);
	return (res!=0);
}
static void CALLCONV GetExtent(HWND hWnd, RECT& rc, AECHARINDEX& wb, AECHARINDEX& we, int& xb, int& xe, int vPos)
{
	POINT pb,pe,pp;
	AECHARINDEX prev = we, temp;
	AEC_PrevChar(&prev);
	SendMessageW(hWnd, AEM_POSFROMCHAR,(WPARAM)&pb,(LPARAM)&wb);
	SendMessageW(hWnd, AEM_POSFROMCHAR,(WPARAM)&pe,(LPARAM)&prev);
	//SendMessageW(hWnd, AEM_POSFROMCHAR,(WPARAM)&pe,(LPARAM)&prev);
	pe.x += (LONG)SendMessageW(hWnd, AEM_GETCHARSIZE,AECS_INDEXWIDTH,(LPARAM)&prev);
	if((pb.y != vPos) || (pb.x < rc.left))
	{
		pp.y = vPos;
		pp.x = rc.left;
		SendMessageW(hWnd,AEM_CHARFROMPOS,(WPARAM)&pp,(LPARAM)&temp);
		//SendMessageW(hWnd,AEM_POSFROMCHAR,(WPARAM)&pp,(LPARAM)&temp);
		xb = pp.x;
	}
	else
		xb= pb.x;
	if(pe.y != vPos || (pe.x >rc.right))
	{
		pp.y = vPos;
		pp.x = rc.right;
		SendMessageW(hWnd,AEM_CHARFROMPOS,(WPARAM)&pp,(LPARAM)&temp);
		//SendMessageW(hWnd,AEM_POSFROMCHAR,(WPARAM)&pp,(LPARAM)&temp);
		pp.x += (LONG)SendMessageW(hWnd, AEM_GETCHARSIZE,AECS_INDEXWIDTH,(LPARAM)&temp);
		xe = pp.x;
	}
	else
		xe= pe.x;
}
static void CALLCONV GetText(const HWND hWnd, const AECHARINDEX& start, const AECHARINDEX& end, UINT_PTR* len,WCHAR* buffer, const UINT_PTR maxchars)
{
	AETEXTRANGEW gtr;
		gtr.cr.ciMin		=start;
		gtr.cr.ciMax		=end;
		gtr.bColumnSel		=FALSE;
		gtr.pBuffer			=NULL;
		gtr.dwBufferMax		=0;
		gtr.nNewLine		=AELB_ASOUTPUT;
		gtr.nCodePage		=0;
		gtr.lpDefaultChar	=NULL;
		gtr.bFillSpaces		=FALSE;
	UINT_PTR wl = (UINT_PTR)SendMessageW(hWnd,AEM_GETTEXTRANGEW,0,(LPARAM)&gtr);
	if(wl > maxchars)
	{
		if(len) *len = wl-1;
		return;
	}
	gtr.dwBufferMax = maxchars;
	gtr.pBuffer = buffer;
	SendMessageW(hWnd,AEM_GETTEXTRANGE,0,(LPARAM)&gtr);
	if(len) *len = wl-1;
}
static void CALLCONV ProcessView(HWND hWnd, RECT& rcBounds, HDC dc, TEXTMETRICW& tm)
{
	SetBkMode(dc,TRANSPARENT);
	LONG_PTR shift = 0;
	LONG_PTR tmHeightLocal;
	shift = (tmHeightLocal = SendMessageW(hWnd, AEM_GETCHARSIZE, AECS_HEIGHT, 0))-1;
	shift -= SendMessageW(hWnd, AEM_GETLINEGAP,0,0);
	AECHARINDEX linechar,lastvis;
	AELINEDATA* line;
	SendMessageW(hWnd,AEM_GETINDEX,AEGI_LASTVISIBLELINE,(LPARAM)&lastvis);
	//// *WORK BUFFER
	WCHAR word[MAX_WORD+1] = {0};
	//// /WORK BUFFER
	for(line = (AELINEDATA*)SendMessageW(hWnd,AEM_GETINDEX,AEGI_FIRSTVISIBLELINE,(LPARAM)&linechar);
		line && linechar.nLine <= lastvis.nLine;
		line = (AELINEDATA*)SendMessageW(hWnd,AEM_GETINDEX,AEGI_NEXTUNCOLLAPSEDLINE,(LPARAM)&linechar)
		)
	{
		POINT point;
		AECHARINDEX ae_first,ae_last;
		SendMessageW(hWnd, AEM_POSFROMCHAR, (WPARAM)&point, (LPARAM)&linechar);
		point.x = rcBounds.left;
		SendMessageW(hWnd, AEM_CHARFROMPOS, (WPARAM)&point, (LPARAM)&ae_first);
		point.x = rcBounds.right;
		SendMessageW(hWnd, AEM_CHARFROMPOS, (WPARAM)&point, (LPARAM)&ae_last);

		if(AEC_IndexCompare(&ae_first,&ae_last))
		{
			AECHARINDEX current = ae_first;
			while(AEC_IndexCompare(&current,&ae_last) <= 0)
			{
				AECHARINDEX wb, we;
				UINT_PTR len =0;
				if(!GetWord(hWnd,current,wb,we))
					break;
				if(SendMessageW(hWnd,AEM_ISDELIMITER,AEDLM_PREVCHAR|AEDLM_WORD,(LPARAM)&we))
					break;
				if(AEC_IndexCompare(&wb,&ae_last)>0)
					break;
				if(AEC_IndexCompare(&wb,&current)<0 && AEC_IndexCompare(&wb,&ae_first)>0)
					break;
				{
					AECHARRANGE url;
					if(SendMessageW(hWnd,AEM_INDEXINURL,(WPARAM)&wb,(LPARAM)&url)!=0 ||
						SendMessageW(hWnd,AEM_INDEXINURL,(WPARAM)&we,(LPARAM)&url)!=0)
					{
						current = url.ciMax;
						continue;
					}
				}
				AEC_NextCharEx(&we,&current);
				//int	b = AEC_IndexCompare(&we,&current);
				GetText(hWnd,wb,we,&len,word,MAX_WORD+1);
				BOOL good = (len > MAX_WORD)
							|| CheckPattern(word)
							|| (rt.setting.WhiteList && CheckWhiteList(word, hWnd))
							|| IsGoodWord(word);
				if(!good)
				{
					int xb,xe;//,y;
					GetExtent(hWnd,rcBounds,wb,we,xb,xe,point.y);
					DrawStroke(dc, point.y+shift,xb,xe,tm.tmHeight);
				}
			}
		}
	}
}
static LRESULT CALLBACK EditProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPEXCEPTION_POINTERS ep = NULL;
	//! If something broken in plugin - it should not affect AkelPad itself
	__try
	{
		if(uMsg == WM_PAINT && rt.pASpell)
		{
#ifdef REDRAW_DELAY
			if(rt.setting.bDelayBack)
			{
				if(rt.drawflag > 0)
				{
					return rt.EditProcData->NextProc(hWnd, uMsg, wParam, lParam);
				}
			}
#endif
			RECT rcUpdate, rcEdit, rcDraw, rc;
			HPEN hpen, holdpen;
			HRGN hRgn, hRgnOld;
			HFONT hFont,hOldFont;
			HDC dc;
			LRESULT lResult;
			LOGFONTW logfont;

			GetUpdateRect(hWnd, &rcUpdate, FALSE);
			SendMessageW(hWnd, EM_GETRECT, 0, (LPARAM)&rcEdit);
			if (rcUpdate.left > rcEdit.left) rcUpdate.left=rcEdit.left;
			if (rcUpdate.top > rcEdit.top) rcUpdate.top=rcEdit.top;
			if (rcUpdate.right < rcEdit.right) rcUpdate.right=rcEdit.right;
			if (rcUpdate.bottom < rcEdit.bottom) rcUpdate.bottom=rcEdit.bottom;
			/*rcDraw.left=rcEdit.left;
			rcDraw.top=rcUpdate.top;
			rcDraw.right=rcEdit.right;
			rcDraw.bottom=rcUpdate.bottom;*/
			rcDraw = rcEdit;

			lResult = rt.EditProcData->NextProc(hWnd, uMsg, wParam, lParam);
			HideCaret(hWnd);
			rc=rcDraw;
			//GetClientRect(hWnd,&rc);

			dc = GetDC(hWnd);
			hFont=(HFONT)SendMessageW(rt.cmn_hMainWindow, AKD_GETFONTW, (WPARAM)hWnd, (LPARAM)&logfont);
			//hFont=CreateFontIndirectW((LOGFONTW*)SendMessageW(cmn_hMainWindow, AKD_GETFONTW, (WPARAM)hWnd, (LPARAM)&logfont));
			hOldFont=(HFONT)SelectObject(dc, hFont);
			TEXTMETRICW tm;
			GetTextMetricsW(dc,&tm);

			hpen = CreatePen(
				(int)((rt.setting.UnderlineStyle>SCUS_LASTBUILTINSTYLE)?PS_SOLID:rt.setting.UnderlineStyle),
				(rt.setting.UnderlineStyle==SCUS_COMPACTDOT)?1:(tm.tmHeight/UNDERSCORE_SCALE),
				rt.setting.UnderlineColor);
			holdpen = (HPEN)SelectObject(dc,hpen);
			hRgn=CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
			hRgnOld=(HRGN)SelectObject(dc, hRgn);

			SelectObject(dc,GetStockObject(NULL_BRUSH));
			//SetBkMode(dc,TRANSPARENT);
			//Rectangle(dc,rc.left,rc.top,rc.right,rc.bottom);
			ProcessView(hWnd,rc,dc,tm);
			SelectObject(dc,holdpen);
			DeleteObject(hpen);
			SelectObject(dc,hRgnOld);
			DeleteObject(hRgn);
			SelectObject(dc,hOldFont);
			//DeleteObject(hFont);
			ReleaseDC(hWnd,dc);
			ShowCaret(hWnd);
#ifdef REDRAW_DELAY
			rt.drawflag = -1;
			//char t[]="draw flag is 0\n";
			//t[13]='0'+rt.drawflag;
			//OutputDebugStringA(t);
#endif
			return lResult;
		}
		else if(((uMsg == WM_HSCROLL) && (rt.setting.dwHook_WM_XSCROLL & XSCROLL_MASK_HORZ))
				||((uMsg == WM_VSCROLL)&&(rt.setting.dwHook_WM_XSCROLL & XSCROLL_MASK_VERT)))
		{
			LRESULT res = rt.EditProcData->NextProc(hWnd, uMsg, wParam, lParam);
			UpdateEdit(hWnd);
			return res;
		}
#ifdef REDRAW_DELAY
		else if(rt.setting.bDelayBack)
		{
			if(uMsg >=WM_KEYFIRST && uMsg <= WM_KEYLAST)
			{
				if(uMsg == WM_KEYUP)
				{
					rt.drawflag = 0;
					UpdateEdit(hWnd);
				}
				else
				{
					if(rt.drawflag<1) rt.drawflag=1;
					if(rt.drawflag==1) rt.drawflag=2;
				}
			}
		}
#endif
#if 0
		if(uMsg == WM_SETFOCUS)
		{
			LRESULT mask = SendMessageW(hWnd,AEM_GETEVENTMASK,0,0);
			if(!(mask&AENM_SCROLL))
			{
				SendMessageW(hWnd,AEM_SETEVENTMASK,0,AENM_SCROLL|mask);
			}
		}
#endif
	}
	__except((ep = GetExceptionInformation())?1:0)
	{
		//EINFOCOLLECTOR(ep,0)
	}
	return rt.EditProcData->NextProc(hWnd, uMsg, wParam, lParam);
}

void CALLCONV DoSuggestion(PLUGINDATA *pd)
{
	if(!IsWindow(pd->hWndEdit)) return;
	int		written=0;
	WCHAR	SUGGESTED_WORD[MAX_PATHW]={0};
	char	EWord[MAX_PATHW]={0};

	const DICINFO* di = GetDict(rt.setting.szLanguage,rt.setting.szJargon);
	if(di == NULL)
		return;

	AECHARRANGE word;
	if(!GetCurrentSel(pd,word))
		return;
	{
		AECHARRANGE url;
		if(SendMessageW(pd->hWndEdit,AEM_INDEXINURL,(WPARAM)&word.ciMin,(LPARAM)&url) ||
			SendMessageW(pd->hWndEdit,AEM_INDEXINURL,(WPARAM)&word.ciMax,(LPARAM)&url))
		{
			return;
		}
	}
	WCHAR szword[MAX_WORD+1]={0};
	AETEXTRANGEW gtr;
		gtr.cr = word;//AECHARRANGE
		gtr.bColumnSel=FALSE;
		gtr.pBuffer=NULL;
		gtr.dwBufferMax=0;
		gtr.nNewLine=AELB_ASOUTPUT;
		gtr.nCodePage=0;
		gtr.lpDefaultChar=NULL;
		gtr.bFillSpaces=FALSE;

	if(SendMessageW(pd->hWndEdit,AEM_GETTEXTRANGE,0,(LPARAM)&gtr)>129)
		return;
	gtr.pBuffer = szword;
	gtr.dwBufferMax = MAX_WORD+1;
	SendMessageW(pd->hWndEdit,AEM_GETTEXTRANGE,0,(LPARAM)&gtr);

	WideCharToMultiByte(di->nEncoding,0,szword,-1,EWord,1024," ",NULL);
	const AspellWordList *pAWL = rt.pASpell->aspell_speller_suggest(di->pSpell,EWord,-1);
	if(pAWL)
	{
		AspellStringEnumeration * pASE = rt.pASpell->aspell_word_list_elements(pAWL);
		UINT nCmd = 0;
		if(!rt.pASpell->aspell_string_enumeration_at_end(pASE))
		{
			HMENU hMenu = NULL;
			hMenu = ::CreatePopupMenu();
			const char *sword;
			int i = 1;
			while((sword = rt.pASpell->aspell_string_enumeration_next(pASE))!=0)
			{
				WCHAR *uni=NULL;
				written = MultiByteToWideChar((UINT)di->nEncoding,0,sword,-1,uni,0);
				uni = new WCHAR[written+1];
				written = MultiByteToWideChar((UINT)di->nEncoding,0,sword,-1,uni,written);
				AppendMenu(hMenu,MF_ENABLED|MF_STRING,i,uni);
				delete[] uni;
				i++;
			}
			POINT ppp;
			GetCaretPos(&ppp);
			ClientToScreen(pd->hWndEdit,&ppp);
			nCmd = (UINT)::TrackPopupMenu(hMenu,
				TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON,
				ppp.x,ppp.y,0,pd->hWndEdit,NULL);
			if(nCmd)
			{
				GetMenuStringW(hMenu,nCmd,SUGGESTED_WORD,MAX_PATHW-1,MF_BYCOMMAND);
			}
			::DestroyMenu(hMenu);
		}
		//! Free enumerator
		rt.pASpell->delete_aspell_string_enumeration(pASE);
		if(nCmd!=0)
		{
			SendMessageW(pd->hWndEdit,AEM_EXSETSEL,(WPARAM)&word.ciMin,(LPARAM)&word.ciMax);
			AEREPLACESELW rep = {SUGGESTED_WORD,-1,AELB_ASINPUT,0,NULL,NULL};
			SendMessageW(pd->hWndEdit,AEM_REPLACESELW,0,(LPARAM)&rep);
		}
	}
	else
	{
		if(rt.setting.ShowNotice)
			MessageBoxA(pd->hMainWnd,rt.pASpell->aspell_speller_error_message(di->pSpell),"SpellChecker",MB_OK|MB_ICONERROR);
	}
}
static BOOL CALLCONV CheckPattern(const WCHAR* word)
{
	bool hex = false;
	if(*word==L'0')
	{
		word++;
		if(*word==L'x'||*word=='X')
		{
			hex=true;
			word++;
			if(!*word) //word=="0x"
				return FALSE;
		}
	}
	register WCHAR ch = *word;
	while(ch && (
		(ch>=L'0' && ch<=L'9')||
		(hex &&((ch>=L'A' && ch <=L'F')||(ch>=L'a' && ch <=L'f')))))
	{
		word++;
		ch = *word;
	}
	if(ch) return FALSE;
	return TRUE;
}
static const DICINFO* CALLCONV GetDict(LPCWSTR dict, LPCWSTR jar)
{
	if(dict==NULL || jar==NULL) return NULL;
	register UINT i = 0;
	const register UINT sz = rt.DICTIONARY_COUNT;
	for( ; i < sz; i++)
	{
		if(lstrcmpW(rt.DICTIONARIES[i].code,dict)==0 &&
			lstrcmpW(rt.DICTIONARIES[i].jargon,jar)==0)
		{
			return rt.DICTIONARIES + i;
		}
	}
	return NULL;
}
#ifdef REDRAW_DELAY
static void CALLBACK RedrawTimer(HWND hwnd,UINT msg, UINT_PTR id, DWORD dwTime)
{
	//char t[]="draw flag is 0\n";
	//t[13]='0'+rt.drawflag;
	//OutputDebugStringA(t);
	if(rt.drawflag >0)
	{
		--rt.drawflag;
		if(rt.drawflag==0) UpdateEditAll();
		//t[13]='0'+rt.drawflag;
		//OutputDebugStringA(t);
	}
}
#endif
#define PtrFromRva( base, rva ) ( ( ( PBYTE ) base ) + rva )

/*++
Routine Description:
Replace the function pointer in a module's IAT.

Parameters:
Module              - Module to use IAT from.
ImportedModuleName  - Name of imported DLL from which 
function is imported.
ImportedProcName    - Name of imported function.
AlternateProc       - Function to be written to IAT.
OldProc             - Original function.

Return Value:
S_OK on success.
(any HRESULT) on failure.
--*/
HRESULT PatchIat(
	HMODULE Module,
	PSTR ImportedModuleName,
	PSTR ImportedProcName,
	PVOID AlternateProc,
	PVOID *OldProc
	)
{
	PIMAGE_DOS_HEADER DosHeader = ( PIMAGE_DOS_HEADER ) Module;
	PIMAGE_NT_HEADERS NtHeader; 
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor;
	UINT Index;

	NtHeader = ( PIMAGE_NT_HEADERS ) 
		PtrFromRva( DosHeader, DosHeader->e_lfanew );
	if( IMAGE_NT_SIGNATURE != NtHeader->Signature )
	{
		return HRESULT_FROM_WIN32( ERROR_BAD_EXE_FORMAT );
	}

	ImportDescriptor = ( PIMAGE_IMPORT_DESCRIPTOR ) 
		PtrFromRva( DosHeader, 
		NtHeader->OptionalHeader.DataDirectory
		[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );

	//
	// Iterate over import descriptors/DLLs.
	//
	for ( Index = 0; 
		ImportDescriptor[ Index ].Characteristics != 0; 
		Index++ )
	{
		PSTR dllName = ( PSTR ) 
			PtrFromRva( DosHeader, ImportDescriptor[ Index ].Name );

		if ( 0 == _strcmpi( dllName, ImportedModuleName ) )
		{
			//
			// This the DLL we are after.
			//
			PIMAGE_THUNK_DATA Thunk;
			PIMAGE_THUNK_DATA OrigThunk;

			if ( ! ImportDescriptor[ Index ].FirstThunk ||
				! ImportDescriptor[ Index ].OriginalFirstThunk )
			{
				return E_INVALIDARG;
			}

			Thunk = ( PIMAGE_THUNK_DATA )
				PtrFromRva( DosHeader, 
				ImportDescriptor[ Index ].FirstThunk );
			OrigThunk = ( PIMAGE_THUNK_DATA )
				PtrFromRva( DosHeader, 
				ImportDescriptor[ Index ].OriginalFirstThunk );

			for ( ; OrigThunk->u1.Function != NULL; 
				OrigThunk++, Thunk++ )
			{
				if ( OrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG )
				{
					//
					// Ordinal import - we can handle named imports
					// ony, so skip it.
					//
					continue;
				}

				PIMAGE_IMPORT_BY_NAME import = ( PIMAGE_IMPORT_BY_NAME )
					PtrFromRva( DosHeader, OrigThunk->u1.AddressOfData );

				if ( 0 == strcmp( ImportedProcName, 
					( char* ) import->Name ) )
				{
					//
					// Proc found, patch it.
					//
					DWORD junk;
					MEMORY_BASIC_INFORMATION thunkMemInfo;

					//
					// Make page writable.
					//
					VirtualQuery(
						Thunk,
						&thunkMemInfo,
						sizeof( MEMORY_BASIC_INFORMATION ) );
					if ( ! VirtualProtect(
						thunkMemInfo.BaseAddress,
						thunkMemInfo.RegionSize,
						PAGE_EXECUTE_READWRITE,
						&thunkMemInfo.Protect ) )
					{
						return HRESULT_FROM_WIN32( GetLastError() );
					}

					//
					// Replace function pointers (non-atomically).
					//
					if ( OldProc )
					{
						*OldProc = ( PVOID ) ( DWORD_PTR ) 
							Thunk->u1.Function;
					}
#ifdef _WIN64
					Thunk->u1.Function = ( ULONGLONG ) ( DWORD_PTR ) 
						AlternateProc;
#else
					Thunk->u1.Function = ( DWORD ) ( DWORD_PTR ) 
						AlternateProc;
#endif
					//
					// Restore page protection.
					//
					if ( ! VirtualProtect(
						thunkMemInfo.BaseAddress,
						thunkMemInfo.RegionSize,
						thunkMemInfo.Protect,
						&junk ) )
					{
						return HRESULT_FROM_WIN32( GetLastError() );
					}

					return S_OK;
				}
			}

			//
			// Import not found.
			//
			return HRESULT_FROM_WIN32( ERROR_PROC_NOT_FOUND );    
		}
	}

	//
	// DLL not found.
	//
	return HRESULT_FROM_WIN32( ERROR_MOD_NOT_FOUND );
}
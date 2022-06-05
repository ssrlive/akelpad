//#define AEC_FUNCTIONS
#include "AutoBrackets.h"
#include "Plugin.h"
#include "XMemStrFunc.h"


// ClearType definitions... >>>
#ifndef SPI_GETCLEARTYPE
  #define SPI_GETCLEARTYPE 0x1048
#endif

#ifndef SPI_GETFONTSMOOTHING
  #define SPI_GETFONTSMOOTHING 0x004A
#endif

#ifndef SPI_GETFONTSMOOTHINGTYPE
  #define SPI_GETFONTSMOOTHINGTYPE 0x200A
#endif

#ifndef FE_FONTSMOOTHINGCLEARTYPE
  #define FE_FONTSMOOTHINGCLEARTYPE 0x0002
#endif
// <<< ...ClearType definitions


//#define _verify_ab_
#undef _verify_ab_

//#define _verify_hl_
#undef _verify_hl_

#ifdef _DEBUG
  #define NEARBR_CHARINDEX_LOOP_VERIFY         1
  #define NEARBR_CHARINDEX_LOOP_USE_DEBUGBREAK 1
#else
  // be sure to disable it in the Release mode
  #define NEARBR_CHARINDEX_LOOP_VERIFY         0
  #define NEARBR_CHARINDEX_LOOP_USE_DEBUGBREAK 0
#endif

enum TFileType {
  tftNone = 0,
  tftText,
  tftC_Cpp,
  tftH_Hpp,
  tftPas
};

enum TFileType2 {
  tfmNone           = 0x00,
  tfmComment1       = 0x01,
  tfmHtmlCompatible = 0x02,
  tfmEscaped1       = 0x04
};

enum TBracketType {
  tbtNone = 0,
  tbtBracket,  //  ( )
  tbtSquare,   //  [ ]
  tbtBrace,    //  { }
  tbtDblQuote, //  " "
  tbtSglQuote, //  ' '
  tbtTag,      //  < >
  tbtTag2,     //  < />
  tbtTagInv,   //  > <

  tbtCount,

  tbtUser
};

static const char* strBracketsA[tbtCount - 1] = {
  "()",
  "[]",
  "{}",
  "\"\"",
  "\'\'",
  "<>",
  "</>",
  "><"
};

static const WCHAR* strBracketsW[tbtCount - 1] = {
  L"()",
  L"[]",
  L"{}",
  L"\"\"",
  L"\'\'",
  L"<>",
  L"</>",
  L"><"
};

#define  MAX_EXT             16
#define  MAX_ESCAPED_PREFIX  20
#define  HIGHLIGHT_INDEXES    6
// variables
INT_X IndexesHighlighted[HIGHLIGHT_INDEXES] = { -1, -1, -1, -1, -1, -1 };
INT_X IndexesToHighlight[HIGHLIGHT_INDEXES] = { -1, -1, -1, -1, -1, -1 };
INT_X prevIndexesToHighlight[HIGHLIGHT_INDEXES] = { -1, -1, -1, -1, -1, -1 }; // "cached" indexes
INT_X CurrentBracketsIndexes[2] = { -1, -1 };
INT_X NearestBracketsIndexes[4] = { -1, -1, tbtNone, 0 }; // "cached" indexes + nBrType
INT_X prevNearestBracketsIndexes[4] = { -1, -1, tbtNone, 0 }; // "cached" indexes + nBrType

#define XBR_FONTSTYLE_UNDEFINED 0xF0F0F0FF
#define XBR_FONTCOLOR_UNDEFINED ((DWORD)(-1))

#define XBR_STATEFLAG_DONOTHL     0x0001
#define XBR_STATEFLAG_INSELECTION 0x0002

typedef struct sCharacterHighlightData {
    DWORD dwState;           // 0 or combination of XBR_STATEFLAG_*
    DWORD dwFontStyle;       // a copy of AEHLPAINT.dwFontStyle (AEHLS_*)
    DWORD dwActiveTextColor; // a copy of AEHLPAINT.dwActiveText
    DWORD dwActiveBkColor;   // a copy of AEHLPAINT.dwActiveBk
} tCharacterHighlightData;

typedef struct sCharacterInfo {
    INT_X nIndex;
    tCharacterHighlightData chd;
} tCharacterInfo;

typedef struct sStringWrapperW {
    wchar_t* pszStringW;
    int nSize; // size of a string buffer pointed by pszStringW, in characters
    int nLen;  // current length of a string, in characters
} tStringWrapperW;

typedef struct sOccurrenceCookie {
    INT_X nPos; // character position
    int nDirection; // direction: DP_NONE and so on
} tOccurrenceCookie;

#define MAX_OCCURRENCE_DEPTH 8

typedef struct sOccurrenceItem {
    int nType; // occurrence type: e.g. tbtBracket
    unsigned int nDepth; // current occurrence depth: nDepth=0 means "empty"
    tOccurrenceCookie cookies[MAX_OCCURRENCE_DEPTH];
} tOccurrenceItem;

typedef struct sOccurrencesData {
    tOccurrenceItem* pItems; // the array of tOccurrenceItem
    int              nMaxItems; // size of the array
} tOccurrencesData;

tCharacterInfo  hgltCharacterInfo[HIGHLIGHT_INDEXES];
tCharacterInfo  prevCharacterInfo[HIGHLIGHT_INDEXES]; // "cached" info

int             nCurrentFileType = tftNone;
int             nCurrentFileType2 = tfmNone;
HWND            hCurrentEditWnd = NULL; // Can be NULL! Use hActualEditWnd.
HWND            hActualEditWnd = NULL; //currentEdit;
BOOL            bBracketsInternalRepaint = FALSE;
extern BOOL     bAkelPadIsStarting;

#if use_aen_paint
UINT            nAenPaintWanted = 0x00;
TEXTMETRICW     AenPaint_tmW;
TEXTMETRICA     AenPaint_tmA;
#endif

INT_X           nAutoRightBracketPos = -1;
int             nAutoRightBracketType = tbtNone;

extern HWND     g_hMainWnd;
extern BOOL     g_bOldWindows;
extern BOOL     g_bOldRichEdit; // TRUE means Rich Edit 2.0
extern BOOL     g_bAkelEdit;

extern UINT     uBracketsHighlight;
extern BOOL     bBracketsHighlightVisibleArea;
extern BOOL     bBracketsRightExistsOK;
extern BOOL     bBracketsDoDoubleQuote;
extern BOOL     bBracketsDoSingleQuote;
extern BOOL     bBracketsDoTag;
extern BOOL     bBracketsDoTag2;
extern BOOL     bBracketsDoTagIf;
extern BOOL     bBracketsHighlightDoubleQuote;
extern BOOL     bBracketsHighlightSingleQuote;
extern BOOL     bBracketsHighlightTag;
extern BOOL     bBracketsSkipEscaped1;
extern BOOL     bBracketsSkipComment1;
extern COLORREF bracketsColourHighlight[2];
extern char     strHtmlFileExtsA[STR_FILEEXTS_SIZE];
extern wchar_t  strHtmlFileExtsW[STR_FILEEXTS_SIZE];
extern char     strEscaped1FileExtsA[STR_FILEEXTS_SIZE];
extern wchar_t  strEscaped1FileExtsW[STR_FILEEXTS_SIZE];
extern char     strComment1FileExtsA[STR_FILEEXTS_SIZE];
extern wchar_t  strComment1FileExtsW[STR_FILEEXTS_SIZE];

extern DWORD    g_dwOptions[OPT_DWORD_COUNT];

wchar_t         strUserBracketsW[MAX_USER_BRACKETS + 1][4];
char            strUserBracketsA[MAX_USER_BRACKETS + 1][4];
wchar_t         strNextCharOkW__[MAX_PREV_NEXT_CHAR_OK_SIZE];
wchar_t         strPrevCharOkW__[MAX_PREV_NEXT_CHAR_OK_SIZE];
tStringWrapperW nextCharOkW = { strNextCharOkW__, MAX_PREV_NEXT_CHAR_OK_SIZE, 0 };
tStringWrapperW prevCharOkW = { strPrevCharOkW__, MAX_PREV_NEXT_CHAR_OK_SIZE, 0 };


// tStringWrapperW useful functions
static int tStringWrapperW_FindCh(const tStringWrapperW* pSelf, const wchar_t wch)
{
    const wchar_t* p;
    const wchar_t* pEnd;

    p = pSelf->pszStringW;
    pEnd = p + pSelf->nLen;
    for ( ; p < pEnd; ++p )
    {
        if ( *p == wch )
            return 1;
    }
    return 0;
}

// CharacterHighlightData useful functions
static void CharacterHighlightData_Clear(tCharacterHighlightData* chd)
{
  chd->dwState = 0;
  chd->dwFontStyle = XBR_FONTSTYLE_UNDEFINED;
  chd->dwActiveTextColor = XBR_FONTCOLOR_UNDEFINED;
  chd->dwActiveBkColor = XBR_FONTCOLOR_UNDEFINED;
}
static void CharacterHighlightData_Copy(tCharacterHighlightData* chdDst, const tCharacterHighlightData* chdSrc)
{
  chdDst->dwState = chdSrc->dwState;
  chdDst->dwFontStyle = chdSrc->dwFontStyle;
  chdDst->dwActiveTextColor = chdSrc->dwActiveTextColor;
  chdDst->dwActiveBkColor = chdSrc->dwActiveBkColor;
}

// CharacterInfo useful functions
static void CharacterInfo_ClearItem(tCharacterInfo* ciItem)
{
  ciItem->nIndex = -1;
  CharacterHighlightData_Clear(&ciItem->chd);
}
static void CharacterInfo_ClearAll(tCharacterInfo* ciSet)
{
  int i;
  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    CharacterInfo_ClearItem(&ciSet[i]);
  }
}
static void CharacterInfo_Copy(tCharacterInfo* ciSetDst, const tCharacterInfo* ciSetSrc)
{
  int i;
  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    ciSetDst[i].nIndex = ciSetSrc[i].nIndex;
    CharacterHighlightData_Copy(&ciSetDst[i].chd, &ciSetSrc[i].chd);
  }
}
static void CharacterInfo_Add(tCharacterInfo* ciSet, INT_X nIndex, const tCharacterHighlightData* chd)
{
  if (nIndex >= 0)
  {
    int i;
    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      if ((ciSet[i].nIndex < 0) /*|| (ci[i].nIndex == nIndex)*/) // empty
      {
        ciSet[i].nIndex = nIndex;
        CharacterHighlightData_Copy(&ciSet[i].chd, chd);
        break;
      }
    }
  }
}
static tCharacterHighlightData* CharacterInfo_GetHighlightData(tCharacterInfo* ciSet, INT_X nIndex)
{
  if (nIndex >= 0)
  {
    int i;
    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      if (ciSet[i].nIndex == nIndex)
        return &ciSet[i].chd;
    }
  }
  return (tCharacterHighlightData *) 0;
}
static const tCharacterHighlightData* CharacterInfo_GetHighlightDataConst(const tCharacterInfo* ciSet, INT_X nIndex)
{
  if (nIndex >= 0)
  {
    int i;
    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      if (ciSet[i].nIndex == nIndex)
        return &ciSet[i].chd;
    }
  }
  return (const tCharacterHighlightData *) 0;
}

// tOccurrenceCookie useful functions
static void OccurrenceCookie_Clear(tOccurrenceCookie* cookie)
{
  cookie->nPos = -1;
  cookie->nDirection = 0;
}
static void OccurrenceCookie_Copy(tOccurrenceCookie* to, const tOccurrenceCookie* from)
{
  if (from != NULL)
  {
    to->nPos = from->nPos;
    to->nDirection = from->nDirection;
  }
  else
  {
    OccurrenceCookie_Clear(to);
  }
}

// tOccurrenceItem useful functions
static void OccurrenceItem_Clear(tOccurrenceItem* oci)
{
  int i;
  oci->nType = -1;
  oci->nDepth = 0;
  for (i = 0; i < MAX_OCCURRENCE_DEPTH; i++)
  {
    OccurrenceCookie_Clear(&oci->cookies[i]);
  }
}
static void OccurrenceItem_Copy(tOccurrenceItem* to, const tOccurrenceItem* from)
{
  if (from != NULL)
  {
    int i;
    to->nType = from->nType;
    to->nDepth = from->nDepth;
    for (i = 0; i < MAX_OCCURRENCE_DEPTH; i++)
    {
      OccurrenceCookie_Copy(&to->cookies[i], &from->cookies[i]);
    }
  }
  else
  {
    OccurrenceItem_Clear(to);
  }
}

// tOccurrencesData useful functions
static void OccurrencesData_Clear(tOccurrencesData* ocd)
{
  int i;
  for (i = 0; i < ocd->nMaxItems; i++)
  {
    OccurrenceItem_Clear(&ocd->pItems[i]);
  }
}
static void OccurrencesData_Init(tOccurrencesData* ocd, tOccurrenceItem* pItems, int nMaxItems)
{
  ocd->pItems = pItems;
  ocd->nMaxItems = nMaxItems;
  OccurrencesData_Clear(ocd);
}
static BOOL OccurrencesData_Contains(const tOccurrencesData* ocd, int nType, tOccurrenceCookie* cookie /* = NULL */)
{
  tOccurrenceItem* pItem;
  int i;
  BOOL bRet = FALSE;
  for (i = 0; i < ocd->nMaxItems; i++)
  {
    pItem = &ocd->pItems[i];
    if (pItem->nType == nType)
    {
      if (pItem->nDepth != 0)
      {
        if (cookie != NULL)
        {
          OccurrenceCookie_Copy(cookie, &pItem->cookies[pItem->nDepth - 1]);
        }
        bRet = TRUE;
      }
      return bRet;
    }
  }
  return bRet;
}
static BOOL OccurrencesData_IsEmpty(const tOccurrencesData* ocd)
{
  int i;
  for (i = 0; i < ocd->nMaxItems; i++)
  {
    if (ocd->pItems[i].nDepth != 0)
      return FALSE;
  }
  return TRUE;
}
static BOOL OccurrencesData_Increment(tOccurrencesData* ocd, int nType, const tOccurrenceCookie* cookie /* = NULL */)
{
  tOccurrenceItem* pItem;
  int i;
  for (i = 0; i < ocd->nMaxItems; i++)
  {
    pItem = &ocd->pItems[i];
    if (pItem->nType == nType)
    {
      if (pItem->nDepth == MAX_OCCURRENCE_DEPTH)
        return FALSE; // no more room in the cookies array

      OccurrenceCookie_Copy(&pItem->cookies[pItem->nDepth], cookie);
      ++pItem->nDepth;
      return TRUE; // OK
    }
  }
  for (i = 0; i < ocd->nMaxItems; i++)
  {
    pItem = &ocd->pItems[i];
    if (pItem->nDepth == 0) // "empty" occurrence
    {
      pItem->nType = nType;
      pItem->nDepth = 1;
      OccurrenceCookie_Copy(&pItem->cookies[0], cookie);
      return TRUE; // OK
    }
  }
  return FALSE; // no more room in the pItems array
}
static BOOL OccurrencesData_Decrement(tOccurrencesData* ocd, int nType)
{
  tOccurrenceItem* pItem;
  int i;
  for (i = 0; i < ocd->nMaxItems; i++)
  {
    pItem = &ocd->pItems[i];
    if (pItem->nType == nType)
    {
      if (pItem->nDepth != 0)
      {
        if (--pItem->nDepth == 0)
        {
          pItem->nType = -1;
        }
        OccurrenceCookie_Clear(&pItem->cookies[pItem->nDepth]);
      }
      else
      {
        OccurrenceItem_Clear(pItem);
      }
      return TRUE; // OK
    }
  }
  return FALSE; // occurrence of nType was not found
}

// functions prototypes
static WCHAR char2wchar(const char ch);
static void  getEscapedPrefixPos(const INT_X nOffset, INT_X* pnPos, INT* pnLen);
static BOOL  isEscapedPrefixW(const wchar_t* strW, int len);
static BOOL  isEscapedPrefixA(const char* strA, int len);
static BOOL  isEscapedPosW(const INT_X nOffset);
static BOOL  isEscapedPosA(const INT_X nOffset);
static BOOL  isEscapedPosEx(const INT_X nOffset);
static BOOL  isEscapedCharacterW(const INT_X pos, const wchar_t* pcwszLine);
static void  remove_duplicate_indexes_and_sort(INT_X* indexes, const INT size /* = HIGHLIGHT_INDEXES */);
static BOOL  AutoBracketsFunc(MSGINFO* pmsgi, int nBracketType, BOOL bOverwriteMode);
static BOOL  GetHighlightIndexes(const unsigned int uFlags, const int nHighlightIndex,
                                 const INT_X nCharacterPosition, const CHARRANGE_X* pSelection);
static void  GetPosFromChar(HWND hEd, const INT_X nCharacterPosition, POINTL* lpPos);
static BOOL  IsClearTypeEnabled(void);
static void  CopyMemory1(void* dst, const void* src, unsigned int size);

enum eHighlightFlags {
  HF_DOHIGHLIGHT = 0x01,
  HF_CLEARTYPE = 0x02,

  HF_UNINITIALIZED = 0xFFFF
};
static void  HighlightCharacter(const INT_X nCharacterPosition, const unsigned int uHighlightFlags);


/*
int main(void)
{
    // C++ initialization
    currentEdit.m_hWnd = NULL;
    return 0;
}
*/

static void* sys_memalloc(SIZE_T nBytes)
{
  return (void *) GlobalAlloc(GMEM_FIXED, nBytes);
}

static void sys_memfree(void* p)
{
  GlobalFree( (HGLOBAL) p );
}

static const wchar_t* getBracketsPairW(int nBracketType)
{
  if ( nBracketType < tbtUser )
    return strBracketsW[nBracketType - tbtBracket];

  return strUserBracketsW[nBracketType - tbtUser];
}

static const char* getBracketsPairA(int nBracketType)
{
  if ( nBracketType < tbtUser )
    return strBracketsA[nBracketType - tbtBracket];

  return strUserBracketsA[nBracketType - tbtUser];
}

static BOOL isDuplicatedPair(int nBracketType)
{
  if (nBracketType == tbtDblQuote || nBracketType == tbtSglQuote)
    return TRUE;

  if (nBracketType >= tbtUser)
  {
    const wchar_t* pszBrPairW = getBracketsPairW(nBracketType);
    if (pszBrPairW[0] == pszBrPairW[1])
      return TRUE;
  }

  return FALSE;
}

static wchar_t getCharAt(HWND hEd, INT_X nPos)
{
  return (g_bOldWindows ?
            char2wchar(AnyRichEdit_GetCharAt(hEd, nPos)) :
              AnyRichEdit_GetCharAtW(hEd, nPos));
}

enum eBracketTypeFlags {
    BTF_AUTOCOMPLETE = 0x01,
    BTF_HIGHLIGHT    = 0x02,
    BTF_CHECK_TAGINV = 0x10
};

static int getLeftBracketType(const wchar_t wch, const unsigned int uFlags)
{
  int nLeftBracketType = tbtNone;

  switch (wch)
  {
    case L'(' :
      nLeftBracketType = tbtBracket;
      break;
    case L'[' :
      nLeftBracketType = tbtSquare;
      break;
    case L'{' :
      nLeftBracketType = tbtBrace;
      break;
    case L'<' :
      if (bBracketsDoTag || (bBracketsHighlightTag && (uFlags & BTF_HIGHLIGHT)))
        nLeftBracketType = tbtTag;
      break;
    case L'\"' :
      if (bBracketsDoDoubleQuote || (bBracketsHighlightDoubleQuote && (uFlags & BTF_HIGHLIGHT)))
        nLeftBracketType = tbtDblQuote;
      break;
    case L'\'' :
      if (bBracketsDoSingleQuote || (bBracketsHighlightSingleQuote && (uFlags & BTF_HIGHLIGHT)))
        nLeftBracketType = tbtSglQuote;
      break;
    case L'>' :
      if (uFlags & BTF_CHECK_TAGINV)
      {
        if (bBracketsDoTag || (bBracketsHighlightTag && (uFlags & BTF_HIGHLIGHT)))
          nLeftBracketType = tbtTagInv;
      }
      break;
  }

  if (nLeftBracketType == tbtNone)
  {
    if (wch != 0)
    {
      int i = 0;
      while (i < MAX_USER_BRACKETS)
      {
        if (strUserBracketsW[i][0] == wch)
        {
          nLeftBracketType = tbtUser + i;
          i = MAX_USER_BRACKETS; // break condition
        }
        else if (strUserBracketsW[i][0] == 0)
        {
          i = MAX_USER_BRACKETS; // break condition
        }
        else
          ++i;
      }
    }
  }

  return nLeftBracketType;
}

static int getRightBracketType(const wchar_t wch, const unsigned int uFlags)
{
  int nRightBracketType = tbtNone;

  switch (wch)
  {
    case L')' :
      nRightBracketType = tbtBracket;
      break;
    case L']' :
      nRightBracketType = tbtSquare;
      break;
    case L'}' :
      nRightBracketType = tbtBrace;
      break;
    case L'>' :
      if (bBracketsDoTag || (bBracketsHighlightTag && (uFlags & BTF_HIGHLIGHT)))
        nRightBracketType = tbtTag;
      // no break here
    case L'/' :
      if (bBracketsDoTag2 || (bBracketsHighlightTag && (uFlags & BTF_HIGHLIGHT)))
        nRightBracketType = tbtTag2;
      break;
    case L'\"' :
      if (bBracketsDoDoubleQuote || (bBracketsHighlightDoubleQuote && (uFlags & BTF_HIGHLIGHT)))
        nRightBracketType = tbtDblQuote;
      break;
    case L'\'' :
      if (bBracketsDoSingleQuote || (bBracketsHighlightSingleQuote && (uFlags & BTF_HIGHLIGHT)))
        nRightBracketType = tbtSglQuote;
      break;
    case L'<' :
      if (uFlags & BTF_CHECK_TAGINV)
      {
        if (bBracketsDoTag || (bBracketsHighlightTag && (uFlags & BTF_HIGHLIGHT)))
          nRightBracketType = tbtTagInv;
      }
      break;
  }

  if (nRightBracketType == tbtNone)
  {
    if (wch != 0)
    {
      int i = 0;
      while (i < MAX_USER_BRACKETS)
      {
        if (strUserBracketsW[i][1] == wch)
        {
          nRightBracketType = tbtUser + i;
          i = MAX_USER_BRACKETS; // break condition
        }
        else if (strUserBracketsW[i][1] == 0)
        {
          i = MAX_USER_BRACKETS; // break condition
        }
        else
          ++i;
      }
    }
  }

  return nRightBracketType;
}

static int getLeftBracketTypeEx(const wchar_t wch, const unsigned int uFlags)
{
  int nLeftBracketType = getLeftBracketType(wch, uFlags);
  if (nLeftBracketType == tbtTag || nLeftBracketType == tbtTag2 || nLeftBracketType == tbtTagInv)
  {
    if (!(nCurrentFileType2 & tfmHtmlCompatible))
      nLeftBracketType = tbtNone;
  }
  return nLeftBracketType;
}

static int getRightBracketTypeEx(const wchar_t wch, const unsigned int uFlags)
{
  int nRightBracketType = getRightBracketType(wch, uFlags);
  if (nRightBracketType == tbtTag || nRightBracketType == tbtTag2 || nRightBracketType == tbtTagInv)
  {
    if (!(nCurrentFileType2 & tfmHtmlCompatible))
      nRightBracketType = tbtNone;
  }
  return nRightBracketType;
}

// word delimiters may contain '\0' character
static void AemGetWordDelimiters(HWND hWndEdit, tStringWrapperW* pWordDelimiters)
{
    wchar_t* pszDelimitersW;
    int nLen;

    pszDelimitersW = pWordDelimiters->pszStringW;
    pszDelimitersW[0] = 0;
    nLen = (int) SendMessage( hWndEdit, AEM_GETWORDDELIMITERS, pWordDelimiters->nSize - 1, (LPARAM) pszDelimitersW );
    if ( (nLen > 0) && (pszDelimitersW[nLen - 1] == 0) )
        --nLen; // skip 2nd trailing '\0' if present
    if ( (nLen > 0) && (pszDelimitersW[nLen - 1] == 0) )
        --nLen; // skip 1st trailing '\0' if present
    pWordDelimiters->nLen = nLen;
}

// word delimiters may contain '\0' character
static BOOL isSepOrOneOfW(const wchar_t wch, const tStringWrapperW* pChars)
{
  switch (wch)
  {
    case L'\x0D' :
    case L'\x0A' :
    case L'\x00' :
    case L' ' :
    case L'\t' :
      return TRUE;
    default:
      {
        if (tStringWrapperW_FindCh(pChars, wch))
          return TRUE;
      }
  }
  return FALSE;
}

void OnEditCharPressed(MSGINFO* pmsgi)
{
  WCHAR    wch;
  int      nBracketType;
  EDITINFO ei;

  if (SendMessage(g_hMainWnd, AKD_GETEDITINFO, (WPARAM) NULL, (LPARAM) &ei) == 0)
  {
    nAutoRightBracketPos = -1;
    nAutoRightBracketType = tbtNone;
    return;
  }

  if (ei.bReadOnly)
  {
    // don't process read-only file
    nAutoRightBracketPos = -1;
    nAutoRightBracketType = tbtNone;
    return;
  }

  if (ei.bOvertypeMode)
  {
    if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_OVR_AUTOBR] == 0)
    {
      nAutoRightBracketPos = -1;
      nAutoRightBracketType = tbtNone;
      return;
    }
  }

  // verifying if a typed character is a bracket
  if (g_bOldWindows)
  {
    char ch = (char) pmsgi->wParam;
    wch = char2wchar(ch);
  }
  else
  {
    wch = (WCHAR) pmsgi->wParam;
  }

  if (pmsgi->hWnd != hActualEditWnd)
  {
    nAutoRightBracketPos = -1;
    nAutoRightBracketType = tbtNone;
  }

  if (nAutoRightBracketPos >= 0)
  {
    // the right bracket has been just added (automatically)
    // but you may duplicate it manually
    INT_X nEditPos;
    INT_X nEditEndPos;

    if (g_bOldWindows)
      AnyRichEdit_ExGetSelPos(pmsgi->hWnd, &nEditPos, &nEditEndPos);
    else
      AnyRichEdit_ExGetSelPosW(pmsgi->hWnd, &nEditPos, &nEditEndPos);

    if (nEditPos == nAutoRightBracketPos)
    {
      wchar_t next_wch = getCharAt(pmsgi->hWnd, nEditPos);

      if (getRightBracketType(next_wch, BTF_AUTOCOMPLETE) == nAutoRightBracketType)
      {
        if (getRightBracketType(wch, BTF_AUTOCOMPLETE) == nAutoRightBracketType)
        {
          // annul pressed character
          pmsgi->wParam = 0;
          ++nEditPos;
          if (nAutoRightBracketType == tbtTag2)
            ++nEditPos;
          if (g_bOldWindows)
            AnyRichEdit_ExSetSelPos(pmsgi->hWnd, nEditPos, nEditPos);
          else
            AnyRichEdit_ExSetSelPosW(pmsgi->hWnd, nEditPos, nEditPos);
          nAutoRightBracketPos = -1;
          nAutoRightBracketType = tbtNone;
          return;
        }

        // one character has been typed,
        // so the auto-closed right bracket position increased
        ++nAutoRightBracketPos;
      }
      else
      {
        // not auto-closed right bracket type
        nAutoRightBracketPos = -1;
        nAutoRightBracketType = tbtNone;
      }
    }
    else
    {
      // not auto-closed right bracket position
      nAutoRightBracketPos = -1;
      nAutoRightBracketType = tbtNone;
    }
  }

  nBracketType = getLeftBracketType(wch, BTF_AUTOCOMPLETE);
  if (nBracketType != tbtNone)
  {
    // a typed character is a bracket
    if (!AutoBracketsFunc(pmsgi, nBracketType, ei.bOvertypeMode))
    {
      if (nBracketType == tbtTag)
      {
        if (bBracketsDoTag2)
          nBracketType = tbtTag2;
      }
      if (nBracketType == nAutoRightBracketType)
      {
        nAutoRightBracketPos = -1;
        nAutoRightBracketType = tbtNone;
      }
    }
  }
}

static void updateActualState(HWND hEditWnd)
{
  int i;

  if (hActualEditWnd != hEditWnd)
  {
    nAutoRightBracketPos = -1;
    nAutoRightBracketType = tbtNone;
    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      IndexesHighlighted[i] = -1;
      prevIndexesToHighlight[i] = -1;
    }
  }
  hActualEditWnd = hEditWnd;
  if (hActualEditWnd != hCurrentEditWnd)
  {
    nCurrentFileType = getFileType(&nCurrentFileType2);
  }
}

void OnEditGetActiveBrackets(MSGINFO* pmsgi, const unsigned int uFlags)
{
  int         i;
  BOOL        bHighlighted;
  INT_X       nEditPos;
  INT_X       nEditEndPos;
  CHARRANGE_X crSelection;

  if (bBracketsInternalRepaint || bAkelPadIsStarting)
    return;

  if (!pmsgi->hWnd)
    return;

  /*
  TCHAR str[256];
  wsprintf(str, "hWnd = %08X, prev = %08X", pmsgi->hWnd, hCurrentEditWnd);
  MessageBox(NULL, str, "OnEditGetActiveBrackets", MB_OK);
  */

  updateActualState(pmsgi->hWnd);

  // getting current position and selection
  if (g_bOldWindows)
    AnyRichEdit_ExGetSelPos(hActualEditWnd, &nEditPos, &nEditEndPos);
  else
    AnyRichEdit_ExGetSelPosW(hActualEditWnd, &nEditPos, &nEditEndPos);

  if (pmsgi->uMsg == WM_LBUTTONUP)
  {
    if (nEditEndPos == nEditPos)
    {
      // it's just simple WM_LBUTTONUP right after WM_LBUTTONDOWN;
      // active brackets have been highlighted already
      return;
    }
    // nEditEndPos != nEditPos,
    // some text has been selected with a mouse
  }

  crSelection.cpMin = nEditPos;
  crSelection.cpMax = nEditEndPos;

  bHighlighted = FALSE;
  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    IndexesToHighlight[i] = -1;
  }
  CurrentBracketsIndexes[0] = -1;
  CurrentBracketsIndexes[1] = -1;

  if (g_bAkelEdit)
  {
    CharacterInfo_ClearAll(hgltCharacterInfo);
  }

  if (GetHighlightIndexes(uFlags, 0, nEditEndPos-1, &crSelection)) // left character
    bHighlighted = TRUE;

  if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_BOTHBR] || !bHighlighted)
  {
    if (GetHighlightIndexes(uFlags, 3, nEditEndPos, &crSelection))   // right character
      bHighlighted = TRUE;
  }

  if ((!bHighlighted) && (nEditEndPos != nEditPos))
  {
    if (GetHighlightIndexes(uFlags, 0, nEditPos-1, &crSelection)) // left character
      bHighlighted = TRUE;

    if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_BOTHBR] || !bHighlighted)
    {
      if (GetHighlightIndexes(uFlags, 3, nEditPos, &crSelection))   // right character
        bHighlighted = TRUE;
    }
  }

  if (bHighlighted)
  {
    x_mem_cpy(prevIndexesToHighlight, IndexesToHighlight, sizeof(IndexesToHighlight));
    remove_duplicate_indexes_and_sort(IndexesToHighlight, HIGHLIGHT_INDEXES);

    if (g_bAkelEdit)
    {
      CharacterInfo_Copy(prevCharacterInfo, hgltCharacterInfo);
    }
  }

  if (uFlags & XBR_GBF_HIGHLIGHTBR)
  {
    // highlight
    OnEditHighlightActiveBrackets();
  }

}

void OnEditHighlightActiveBrackets(void)
{
  unsigned int uHighlightFlags;
  int   i, j;
  INT_X index;
  INT_X indexesToRemoveHighlight[HIGHLIGHT_INDEXES];

  if (bBracketsInternalRepaint || bAkelPadIsStarting)
    return;

  uHighlightFlags = HF_UNINITIALIZED;

  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    index = IndexesHighlighted[i];
    for (j = 0; j < HIGHLIGHT_INDEXES; j++)
    {
      if (index == IndexesToHighlight[j])
        break;
    }
    if (j < HIGHLIGHT_INDEXES)
      indexesToRemoveHighlight[i] = -1;
    else
      indexesToRemoveHighlight[i] = index;
  }

  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    index = indexesToRemoveHighlight[i];
    if (index >= 0)
    {
      IndexesHighlighted[i] = -1; // prevents from WM_PAINT infinite loop
      if (uHighlightFlags == HF_UNINITIALIZED)
      {
        uHighlightFlags = 0;
        if (IsClearTypeEnabled())
          uHighlightFlags |= HF_CLEARTYPE;
      }
      HighlightCharacter(index, uHighlightFlags);
    }
  }

  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    index = IndexesToHighlight[i];
    IndexesHighlighted[i] = index; // prevents from WM_PAINT infinite loop
    if (index >= 0)
    {
      if (uHighlightFlags == HF_UNINITIALIZED)
      {
        uHighlightFlags = 0;
        if (IsClearTypeEnabled())
          uHighlightFlags |= HF_CLEARTYPE;
      }
      HighlightCharacter(index, uHighlightFlags | HF_DOHIGHLIGHT);
    }
  }

}

static BOOL IsEnclosedInBracketsW(const wchar_t* pszTextLeftW, const wchar_t* pszTextRightW, int* pnBracketType, BOOL bInSelection)
{
  const wchar_t* pszBrPairW;
  int  nBrType;
  BOOL bRet;

  bRet = FALSE;
  nBrType = *pnBracketType;
  pszBrPairW = getBracketsPairW(nBrType);

  if (pszTextLeftW[0] != pszBrPairW[0])
    return bRet;

  if ((pszTextRightW[0] == pszBrPairW[1]) &&
      (pszBrPairW[2] == 0 || pszTextRightW[1] == pszBrPairW[2]))
  {
    bRet = TRUE;
    if (nBrType != tbtTag)
      return bRet;
  }

  if (nBrType == tbtTag)
  {
    nBrType = tbtTag2;
    if (bInSelection)
      --pszTextRightW; // '/' is present in "/>"
  }
  else if (nBrType == tbtTag2)
  {
    nBrType = tbtTag;
    if (bInSelection)
      ++pszTextRightW; // '/' is not present in ">"
  }
  else
    return bRet;

  // Note: both tbtTag and tbtTag2 start with '<'
  pszBrPairW = getBracketsPairW(nBrType);
  if ((pszTextRightW[0] == pszBrPairW[1]) &&
      (pszBrPairW[2] == 0 || pszTextRightW[1] == pszBrPairW[2]))
  {
    *pnBracketType = nBrType;
    bRet = TRUE;
  }

  return bRet;
}

static BOOL IsEnclosedInBracketsA(const char* pszTextLeftA, const char* pszTextRightA, int* pnBracketType, BOOL bInSelection)
{
  const char* pszBrPairA;
  int  nBrType;
  BOOL bRet;

  bRet = FALSE;
  nBrType = *pnBracketType;
  pszBrPairA = getBracketsPairA(nBrType);

  if (pszTextLeftA[0] != pszBrPairA[0])
    return bRet;

  if ((pszTextRightA[0] == pszBrPairA[1]) &&
      (pszBrPairA[2] == 0 || pszTextRightA[1] == pszBrPairA[2]))
  {
    bRet = TRUE;
    if (nBrType != tbtTag)
      return bRet;
  }

  if (nBrType == tbtTag)
  {
    nBrType = tbtTag2;
    if (bInSelection)
      --pszTextRightA; // '/' is present in "/>"
  }
  else if (nBrType == tbtTag2)
  {
    nBrType = tbtTag;
    if (bInSelection)
      ++pszTextRightA; // '/' is not present in ">"
  }
  else
    return bRet;

  // Note: both tbtTag and tbtTag2 start with '<'
  pszBrPairA = getBracketsPairA(nBrType);
  if ((pszTextRightA[0] == pszBrPairA[1]) &&
      (pszBrPairA[2] == 0 || pszTextRightA[1] == pszBrPairA[2]))
  {
    *pnBracketType = nBrType;
    bRet = TRUE;
  }

  return bRet;
}

/*static*/ BOOL AutoBracketsFunc(MSGINFO* pmsgi, int nBracketType, BOOL bOverwriteMode)
{
  INT_X   nEditPos;
  INT_X   nEditEndPos;
  WCHAR   next_wch;
  BOOL    bPrevCharOK = TRUE;
  BOOL    bNextCharOK = FALSE;

  #ifdef _verify_ab_
    TCHAR str[128];
  #endif

  updateActualState(pmsgi->hWnd);

  if (nBracketType == tbtTag)
  {
    if (bBracketsDoTagIf && !(nCurrentFileType2 & tfmHtmlCompatible))
      return FALSE;
  }

  // getting current position and selection
  if (g_bOldWindows)
    AnyRichEdit_ExGetSelPos(hActualEditWnd, &nEditPos, &nEditEndPos);
  else
    AnyRichEdit_ExGetSelPosW(hActualEditWnd, &nEditPos, &nEditEndPos);

  // is something selected?
  if (nEditEndPos != nEditPos)
  {
    if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 0)
    {
      // removing selection
      if (g_bOldWindows)
        AnyRichEdit_ReplaceSelText(hActualEditWnd, "", TRUE);
      else
        AnyRichEdit_ReplaceSelTextW(hActualEditWnd, L"", TRUE);
    }
    else
    {
      if (nBracketType == tbtTag)
      {
        if (bBracketsDoTag2)
          nBracketType = tbtTag2;
      }

      if (g_bOldWindows)
      {
        INT_X       nSelLen;
        const char* pBrPairA;
        char*       pTextA;
        int         nBrPairLen;
        int         nBrAltType;

        nSelLen = nEditEndPos - nEditPos;
        pBrPairA = getBracketsPairA(nBracketType);
        nBrAltType = nBracketType;
        nBrPairLen = lstrlenA(pBrPairA);
        pTextA = (char*)sys_memalloc(sizeof(char)*(nSelLen + nBrPairLen + 2));
        if (pTextA)
        {
          if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 3 ||
              g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 4)
          {
            // don't select the brackets/quotes themselves
            if (nEditPos == 0)
            {
              AnyRichEdit_GetTextAt(hActualEditWnd, nEditPos, nSelLen + nBrPairLen, pTextA + 1);
            }
            else
            {
              AnyRichEdit_GetTextAt(hActualEditWnd, nEditPos - 1, nSelLen + nBrPairLen + 1, pTextA);
              // Note:
              // "+ 1" in "nSelLen + nBrPairLen + 1" deals with "< ... >" (tbtTag)
              // when it can be "< ... />" (tbtTag2) i.e. +1 symbol
            }

            if (IsEnclosedInBracketsA(pTextA + 1, pTextA + nSelLen - nBrPairLen + 2, &nBrAltType, TRUE))
            {
              // already in brackets/quotes : ["text"] ; excluding them
              if (nBrAltType != nBracketType)
              {
                pBrPairA = getBracketsPairA(nBrAltType);
                nBrPairLen = lstrlenA(pBrPairA);
              }
              pTextA[nSelLen - nBrPairLen + 2] = 0;
              AnyRichEdit_ReplaceSelText(hActualEditWnd, pTextA + 2, TRUE);
              AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos, nEditEndPos - nBrPairLen);
            }
            else if ((g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 4) &&
                     (nEditPos != 0) &&
                     IsEnclosedInBracketsA(pTextA, pTextA + nSelLen + 1, &nBrAltType, FALSE))
            {
              // already in brackets/quotes : "[text]" ; excluding them
              if (nBrAltType != nBracketType)
              {
                pBrPairA = getBracketsPairA(nBrAltType);
                nBrPairLen = lstrlenA(pBrPairA);
              }
              pTextA[nSelLen + 1] = 0;
              SendMessageA(hActualEditWnd, WM_SETREDRAW, FALSE, 0);
              AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos - 1, nEditEndPos + nBrPairLen - 1);
              SendMessageA(hActualEditWnd, WM_SETREDRAW, TRUE, 0);
              AnyRichEdit_ReplaceSelText(hActualEditWnd, pTextA + 1, TRUE);
              AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos - 1, nEditEndPos - 1);
            }
            else
            {
              // enclose in brackets/quotes
              pTextA[0] = pBrPairA[0];
              lstrcpyA(pTextA + nSelLen + 1, pBrPairA + 1);
              AnyRichEdit_ReplaceSelText(hActualEditWnd, pTextA, TRUE);
              AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos + 1, nEditPos + nSelLen + 1);
            }
          }
          else
          {
            pTextA[0] = pBrPairA[0];
            nSelLen = AnyRichEdit_GetTextAt(hActualEditWnd, nEditPos, nSelLen, pTextA + 1);
            if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 2)
            {
              if (IsEnclosedInBracketsA(pTextA + 1, pTextA + nSelLen - nBrPairLen + 2, &nBrAltType, TRUE))
              {
                // already in brackets/quotes; exclude them
                if (nBrAltType != nBracketType)
                {
                  pBrPairA = getBracketsPairA(nBrAltType);
                  nBrPairLen = lstrlenA(pBrPairA);
                }
                pTextA[nSelLen - nBrPairLen + 2] = 0;
                AnyRichEdit_ReplaceSelText(hActualEditWnd, pTextA + 2, TRUE);
                nEditEndPos -= nBrPairLen;
              }
              else
              {
                // enclose in brackets/quotes
                lstrcpyA(pTextA + nSelLen + 1, pBrPairA + 1);
                AnyRichEdit_ReplaceSelText(hActualEditWnd, pTextA, TRUE);
                nEditEndPos += nBrPairLen;
              }
            }
            else // (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 1)
            {
              lstrcpyA(pTextA + nSelLen + 1, pBrPairA + 1);
              AnyRichEdit_ReplaceSelText(hActualEditWnd, pTextA, TRUE);
              ++nEditPos;
              ++nEditEndPos;
            }
            AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos, nEditEndPos);
          }
          sys_memfree(pTextA);
        }
      }
      else
      {
        INT_X          nSelLen;
        const wchar_t* pBrPairW;
        wchar_t*       pTextW;
        int            nBrPairLen;
        int            nBrAltType;

        nSelLen = nEditEndPos - nEditPos;
        pBrPairW = getBracketsPairW(nBracketType);
        nBrAltType = nBracketType;
        nBrPairLen = lstrlenW(pBrPairW);
        pTextW = (wchar_t*)sys_memalloc(sizeof(wchar_t)*(nSelLen + nBrPairLen + 2));
        if (pTextW)
        {
          if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 3 ||
              g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 4)
          {
            // don't select the brackets/quotes themselves
            if (nEditPos == 0)
            {
              AnyRichEdit_GetTextAtW(hActualEditWnd, nEditPos, nSelLen + nBrPairLen, pTextW + 1);
            }
            else
            {
              AnyRichEdit_GetTextAtW(hActualEditWnd, nEditPos - 1, nSelLen + nBrPairLen + 1, pTextW);
              // Note:
              // "+ 1" in "nSelLen + nBrPairLen + 1" deals with "< ... >" (tbtTag)
              // when it can be "< ... />" (tbtTag2) i.e. +1 symbol
            }

            if (IsEnclosedInBracketsW(pTextW + 1, pTextW + nSelLen - nBrPairLen + 2, &nBrAltType, TRUE))
            {
              // already in brackets/quotes : ["text"] ; excluding them
              if (nBrAltType != nBracketType)
              {
                pBrPairW = getBracketsPairW(nBrAltType);
                nBrPairLen = lstrlenW(pBrPairW);
              }
              pTextW[nSelLen - nBrPairLen + 2] = 0;
              AnyRichEdit_ReplaceSelTextW(hActualEditWnd, pTextW + 2, TRUE);
              AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos, nEditEndPos - nBrPairLen);
            }
            else if ((g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 4) &&
                     (nEditPos != 0) &&
                     IsEnclosedInBracketsW(pTextW, pTextW + nSelLen + 1, &nBrAltType, FALSE))
            {
              // already in brackets/quotes : "[text]" ; excluding them
              if (nBrAltType != nBracketType)
              {
                pBrPairW = getBracketsPairW(nBrAltType);
                nBrPairLen = lstrlenW(pBrPairW);
              }
              pTextW[nSelLen + 1] = 0;
              SendMessageW(hActualEditWnd, WM_SETREDRAW, FALSE, 0);
              AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos - 1, nEditEndPos + nBrPairLen - 1);
              SendMessageW(hActualEditWnd, WM_SETREDRAW, TRUE, 0);
              AnyRichEdit_ReplaceSelTextW(hActualEditWnd, pTextW + 1, TRUE);
              AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos - 1, nEditEndPos - 1);
            }
            else
            {
              // enclose in brackets/quotes
              pTextW[0] = pBrPairW[0];
              lstrcpyW(pTextW + nSelLen + 1, pBrPairW + 1);
              AnyRichEdit_ReplaceSelTextW(hActualEditWnd, pTextW, TRUE);
              AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos + 1, nEditPos + nSelLen + 1);
            }
          }
          else
          {
            pTextW[0] = pBrPairW[0];
            nSelLen = AnyRichEdit_GetTextAtW(hActualEditWnd, nEditPos, nSelLen, pTextW + 1);
            if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 2)
            {
              if (IsEnclosedInBracketsW(pTextW + 1, pTextW + nSelLen - nBrPairLen + 2, &nBrAltType, TRUE))
              {
                // already in brackets/quotes; exclude them
                if (nBrAltType != nBracketType)
                {
                  pBrPairW = getBracketsPairW(nBrAltType);
                  nBrPairLen = lstrlenW(pBrPairW);
                }
                pTextW[nSelLen - nBrPairLen + 2] = 0;
                AnyRichEdit_ReplaceSelTextW(hActualEditWnd, pTextW + 2, TRUE);
                nEditEndPos -= nBrPairLen;
              }
              else
              {
                // enclose in brackets/quotes
                lstrcpyW(pTextW + nSelLen + 1, pBrPairW + 1);
                AnyRichEdit_ReplaceSelTextW(hActualEditWnd, pTextW, TRUE);
                nEditEndPos += nBrPairLen;
              }
            }
            else // (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR] == 1)
            {
              lstrcpyW(pTextW + nSelLen + 1, pBrPairW + 1);
              AnyRichEdit_ReplaceSelTextW(hActualEditWnd, pTextW, TRUE);
              ++nEditPos;
              ++nEditEndPos;
            }
            AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos, nEditEndPos);
          }
          sys_memfree(pTextW);
        }
      }

      // annul pressed character
      pmsgi->wParam = 0;
      // no auto bracket position
      nAutoRightBracketPos = -1;
      nAutoRightBracketType = tbtNone;
      return TRUE;
    }
  }

  // getting next character
  next_wch = getCharAt(hActualEditWnd, nEditPos);

  #ifdef _verify_ab_
    wsprintf(str, "next_ch = %04X", next_wch);
    MessageBox(NULL, str, "AutoBracketsFunc", MB_OK);
  #endif

  if ((g_dwOptions[OPT_DWORD_AUTOCOMPLETE_ALL_AUTOBR] & 0x01) ||
      isSepOrOneOfW(next_wch, &nextCharOkW))
  {
    int nBrType = getRightBracketType(next_wch, BTF_AUTOCOMPLETE);
    if (nBrType == tbtNone)
    {
      bNextCharOK = TRUE;
    }
    else
    {
      if (nBrType == tbtTag2)
      {
        nBrType = tbtTag;
      }
      if (bBracketsRightExistsOK || (nBrType != nBracketType))
      {
        bNextCharOK = TRUE;
      }
    }
    if (bNextCharOK && !bBracketsRightExistsOK)
    {
      if ((next_wch == L'\'') &&
          (getBracketsPairW(nBracketType)[1] == L'\''))
      {
        bNextCharOK = FALSE;
      }
    }
  }

  #ifdef _verify_ab_
    wsprintf(str, "bNextCharOK = %lu", bNextCharOK);
    MessageBox(NULL, str, "AutoBracketsFunc", MB_OK);
  #endif

  if ( bNextCharOK && isDuplicatedPair(nBracketType) )
  {
    wchar_t prev_wch;

    bPrevCharOK = FALSE;
    // getting previous character
    prev_wch = getCharAt(hActualEditWnd, nEditPos-1);

    #ifdef _verify_ab_
      wsprintf(str, "prev_ch = %04X", prev_wch);
      MessageBox(NULL, str, "AutoBracketsFunc", MB_OK);
    #endif

    if ((g_dwOptions[OPT_DWORD_AUTOCOMPLETE_ALL_AUTOBR] & 0x02) ||
        isSepOrOneOfW(prev_wch, &prevCharOkW))
    {
      int nBrType = getLeftBracketType(prev_wch, BTF_AUTOCOMPLETE);
      if (nBrType == tbtNone)
      {
        bPrevCharOK = TRUE;
      }
      else
      {
        if (nBrType != nBracketType)
        {
          //if (!isDuplicatedPair(nBrType))
          {
            bPrevCharOK = TRUE;
          }
        }
      }
    }
  }

  if (bPrevCharOK && bNextCharOK &&
      bBracketsSkipEscaped1 && (nCurrentFileType2 & tfmEscaped1))
  {
    wchar_t szPrefixW[MAX_ESCAPED_PREFIX + 2];
    INT_X   pos;
    int     len;

    getEscapedPrefixPos(nEditPos, &pos, &len);

    if (g_bOldWindows)
    {
      char szPrefixA[MAX_ESCAPED_PREFIX + 2];

      len = (int) AnyRichEdit_GetTextAt(hActualEditWnd, pos, len, szPrefixA);
      MultiByteToWideChar(CP_ACP, 0, szPrefixA, -1, szPrefixW, MAX_ESCAPED_PREFIX + 1);
    }
    else
    {
      len = (int) AnyRichEdit_GetTextAtW(hActualEditWnd, pos, len, szPrefixW);
    }

    if (isEscapedPrefixW(szPrefixW, len))
      bPrevCharOK = FALSE;
  }

  if (bPrevCharOK && bNextCharOK)
  {
    // annul pressed character
    pmsgi->wParam = 0;

    if (nBracketType == tbtTag)
    {
      if (bBracketsDoTag2)
        nBracketType = tbtTag2;
    }

    // AkelEdit: the caret position can be out of line length
    if (g_bAkelEdit)
    {
      AECHARINDEX ci;

      ci.lpLine = NULL;
      SendMessage( hActualEditWnd, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ci );
      if (ci.lpLine)
      {
        INT_X nLineIndex;

        if (g_bOldWindows)
          nLineIndex = AnyRichEdit_LineIndex(hActualEditWnd, ci.nLine);
        else
          nLineIndex = AnyRichEdit_LineIndexW(hActualEditWnd, ci.nLine);

        nEditPos = nLineIndex + ci.nCharInLine;
      }
    }

    if (bOverwriteMode)
    {
      if (g_dwOptions[OPT_DWORD_AUTOCOMPLETE_OVR_AUTOBR] == 2)
      {
        // overwrite mode: right bracket overwrites existing symbol
        int     nLen;
        int     i;
        wchar_t wsz[8];

        if (g_bOldWindows)
        {
          nLen = lstrlenA(getBracketsPairA(nBracketType));
          wsz[0] = 0;
          AnyRichEdit_GetTextAt(hActualEditWnd, nEditPos, nLen, (char*)wsz);
          for (i = 0; i < nLen; i++)
          {
            switch ( ((char*)wsz)[i] )
            {
              case '\r':
              case '\n':
              case 0:
                nLen = i;
                break;
            }
          }
          AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos, nEditPos + nLen);
        }
        else
        {
          nLen = lstrlenW(getBracketsPairW(nBracketType));
          wsz[0] = 0;
          AnyRichEdit_GetTextAtW(hActualEditWnd, nEditPos, nLen, wsz);
          for (i = 0; i < nLen; i++)
          {
            switch ( wsz[i] )
            {
              case L'\r':
              case L'\n':
              case 0:
                nLen = i;
                break;
            }
          }
          AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos, nEditPos + nLen);
        }
      }
    }

    // inserting brackets
    if (g_bOldWindows)
      AnyRichEdit_ReplaceSelText(hActualEditWnd, getBracketsPairA(nBracketType), TRUE);
    else
      AnyRichEdit_ReplaceSelTextW(hActualEditWnd, getBracketsPairW(nBracketType), TRUE);

    // placing cursor between brackets
    nEditPos++;
    if (g_bOldWindows)
      AnyRichEdit_ExSetSelPos(hActualEditWnd, nEditPos, nEditPos);
    else
      AnyRichEdit_ExSetSelPosW(hActualEditWnd, nEditPos, nEditPos);

    nAutoRightBracketPos = nEditPos;
    nAutoRightBracketType = nBracketType;
    return TRUE;
  }

  return FALSE;
}

static void updateCurrentBracketsIndexes(INT_X ind1, INT_X ind2)
{
  if (ind1 < ind2)
  {
    if ( (CurrentBracketsIndexes[0] < 0) ||
         (ind1 == CurrentBracketsIndexes[0] - 1) ||
         (ind2 == CurrentBracketsIndexes[1] + 1)
       )
    {
      CurrentBracketsIndexes[0] = ind1; // left bracket
      CurrentBracketsIndexes[1] = ind2; // right bracket
    }
  }
  else
  {
    if ( (CurrentBracketsIndexes[0] < 0) ||
         (ind2 == CurrentBracketsIndexes[0] - 1) ||
         (ind1 == CurrentBracketsIndexes[1] + 1)
       )
    {
      CurrentBracketsIndexes[0] = ind2; // left bracket
      CurrentBracketsIndexes[1] = ind1; // right bracket
    }
  }
}

#define DP_NONE          0x00
#define DP_FORWARD       0x01
#define DP_BACKWARD      0x02
#define DP_DETECT        0x08
#define DP_MAYBEFORWARD  (DP_DETECT | DP_FORWARD)
#define DP_MAYBEBACKWARD (DP_DETECT | DP_BACKWARD)

static BOOL isSentenceEndChar(const wchar_t wch)
{
  switch (wch)
  {
    case L'.':
    case L'?':
    case L'!':
    case 0x2026: // ellipsis (...)
      return TRUE;
  }
  return FALSE;
}

static int getDuplicatedPairDirection(const INT_X nCharacterPosition, const wchar_t curr_wch)
{
  static HWND hLocalEditWnd = NULL;
  static wchar_t szWordDelimitersW[128] = { 0 };
  static tStringWrapperW wordDelimiters = { szWordDelimitersW, 128, 0 };
  WCHAR prev_wch;
  WCHAR next_wch;

  if (nCharacterPosition <= 0)
    return DP_FORWARD; // no char before, search forward

  prev_wch = getCharAt(hActualEditWnd, nCharacterPosition - 1);
  next_wch = getCharAt(hActualEditWnd, nCharacterPosition + 1);

  if (prev_wch == 0)
    return DP_FORWARD; // beginning of the file

  if (prev_wch == curr_wch)
    return DP_BACKWARD; // pair found, search backward

  if (next_wch == 0)
    return DP_BACKWARD; // end of the file

  if (next_wch == curr_wch)
    return DP_FORWARD; // pair found, search forward

  if ( hLocalEditWnd != hActualEditWnd )
  {
    AemGetWordDelimiters(hActualEditWnd, &wordDelimiters);
    hLocalEditWnd = hActualEditWnd;
  }

  if (prev_wch == '\r' ||
      prev_wch == '\n')
  {
    // previous char is EOL, search forward
    return isSepOrOneOfW(next_wch, &wordDelimiters) ? DP_MAYBEFORWARD : DP_FORWARD;
  }

  if (next_wch == '\r' ||
      next_wch == '\n')
  {
    // next char is EOL, search backward
    return isSepOrOneOfW(prev_wch, &wordDelimiters) ? DP_MAYBEBACKWARD : DP_BACKWARD;
  }

  if (isSepOrOneOfW(prev_wch, &wordDelimiters))
  {
    // previous char is a separator
    if (!isSepOrOneOfW(next_wch, &wordDelimiters))
      return DP_FORWARD; // next char is not a separator, search forward

    if (isSentenceEndChar(prev_wch))
    {
      if (!isSentenceEndChar(next_wch)) // e.g.  ."-
        return DP_MAYBEBACKWARD;
    }
  }
  else if (isSepOrOneOfW(next_wch, &wordDelimiters))
  {
    // next char is a separator
    if (!isSepOrOneOfW(prev_wch, &wordDelimiters))
      return DP_BACKWARD; // previous char is not a separator, search backward

    if (isSentenceEndChar(prev_wch))
    {
      if (!isSentenceEndChar(next_wch)) // e.g.  ."-
        return DP_MAYBEBACKWARD;
    }
  }

  return DP_DETECT;
}

static BOOL containsSymbolToTheLeft(const AELINEDATA* lpLine, const wchar_t wch, const INT_X pos)
{
  const WCHAR* pcwszLine;
  INT_X i;
  BOOL  containsSymbol;

  pcwszLine = g_bAkelEdit ? lpLine->wpLine : (const WCHAR *)lpLine;
  containsSymbol = FALSE;
  for (i = pos; (i >= 0) && !containsSymbol; i--)
  {
    if (pcwszLine[i] == wch)
    {
      if (!isEscapedCharacterW(i, pcwszLine))
        containsSymbol = TRUE;
    }
  }

  if (g_bAkelEdit && !containsSymbol)
  {
    // process wrapped line
    lpLine = lpLine->prev;
    while ((!containsSymbol) && (lpLine) && (lpLine->nLineBreak == AELB_WRAP))
    {
      pcwszLine = lpLine->wpLine;
      for (i = lpLine->nLineLen - 1; (i >= 0) && !containsSymbol; i--)
      {
        if (pcwszLine[i] == wch)
        {
          if (!isEscapedCharacterW(i, pcwszLine))
            containsSymbol = TRUE;
        }
      }
      lpLine = lpLine->prev;
    }
  }

  return containsSymbol;
}

static BOOL containsSymbolToTheRight(const AELINEDATA* lpLine, const wchar_t wch, const INT_X pos, int len)
{
  const WCHAR* pcwszLine;
  INT_X i;
  BOOL  containsSymbol;

  pcwszLine = g_bAkelEdit ? lpLine->wpLine : (const WCHAR *)lpLine;
  containsSymbol = FALSE;
  for (i = pos; (i < len) && !containsSymbol; i++)
  {
    if (pcwszLine[i] == wch)
    {
      if (!isEscapedCharacterW(i, pcwszLine))
        containsSymbol = TRUE;
    }
  }

  if (g_bAkelEdit && !containsSymbol)
  {
    // process wrapped line
    while ((!containsSymbol) && (lpLine->nLineBreak == AELB_WRAP))
    {
      lpLine = lpLine->next;
      if (lpLine)
      {
        pcwszLine = lpLine->wpLine;
        len = lpLine->nLineLen;
        for (i = 0; (i < len) && !containsSymbol; i++)
        {
          if (pcwszLine[i] == wch)
          {
            if (!isEscapedCharacterW(i, pcwszLine))
              containsSymbol = TRUE;
          }
        }
      }
    }
  }

  return containsSymbol;
}

static BOOL isDirectionDetecting(const int direction)
{
  switch (direction)
  {
    case DP_DETECT:
    case DP_MAYBEBACKWARD:
    case DP_MAYBEFORWARD:
      return TRUE;
  }
  return FALSE;
}

static int getDirectionIndex(const int direction)
{
  int i = 0;
  switch (direction)
  {
    case DP_BACKWARD:      i = 0; break;
    case DP_MAYBEBACKWARD: i = 1; break;
    case DP_DETECT:        i = 2; break;
    case DP_MAYBEFORWARD:  i = 3; break;
    case DP_FORWARD:       i = 4; break;
  }
  return i;
}

static int getDirectionRank(const int leftDirection, const int rightDirection)
{
  //  The *exact* numbers in the table below have no special meaning.
  //  The only important thing is relative comparison of the numbers:
  //  which of them are greater than others, and which of them are equal.
  //
  // --> leftDirection --> DP_BACKWARD  DP_MAYBEBACKWARD   DP_DETECT      DP_MAYBEFORWARD   DP_FORWARD
  //    DP_FORWARD         <<( )>>  0   <?( )>>   1        ?(? )>>   2    (?> )>>   3       (>> )>>   4
  //    DP_MAYBEFORWARD    <<( )?>  1   <?( )?>   5        ?(? )?>   9    (?> )?>  13       (>> )?>  17
  //    DP_DETECT          <<( ?)?  2   <?( ?)?   9        ?(? ?)?  16    (?> ?)?  23       (>> ?)?  30
  //    DP_MAYBEBACKWARD   <<( <?)  3   <?( <?)  13        ?(? <?)  23    (?> <?)  33       (>> <?)  43
  //    DP_BACWARD         <<( <<)  4   <?( <<)  17        ?(? <<)  30    (?> <<)  43       (>> <<)  56
  // ^ rightDirection ^
  static const int Ranking[5][5] = {
    { 4, 17, 30, 43, 56 }, // DP_BACKWARD
    { 3, 13, 23, 33, 43 }, // DP_MAYBEBACKWARD
    { 2,  9, 16, 23, 30 }, // DP_DETECT
    { 1,  5,  9, 13, 17 }, // DP_MAYBEFORWARD
    { 0,  1,  2,  3,  4 }  // DP_FORWARD
  };

  int leftIndex, rightIndex;

  leftIndex = getDirectionIndex(leftDirection);
  rightIndex = getDirectionIndex(rightDirection);
  return Ranking[rightIndex][leftIndex];
}

typedef struct sGetHighlightIndexesCookie {
    INT_X pos1;
    INT_X pos2;
    int   nResult;
    int   nBracketType;  // -1 or one of TBracketType
    tCharacterHighlightData chd;
} tGetHighlightIndexesCookie;

enum eGetHighLightResult {
    ghlrNone = 0,
    ghlrSingleChar,
    ghlrPair,
    ghlrDoNotHighlight
};

// default implementation for getting matching brackets at the given position
static DWORD CALLBACK GetAkelEditHighlightCallback(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp)
{
  tGetHighlightIndexesCookie* cookie;

  (crRichRange); // unreferenced parameter
  (crAkelRange); // unreferenced parameter

  cookie = (tGetHighlightIndexesCookie *) dwCookie;
  cookie->nResult = ghlrNone;

  /*
  if (hlp->dwActiveText != hlp->dwDefaultText)
  {
    // text color is different than default
  }

  if (hlp->dwActiveBk != hlp->dwDefaultBk)
  {
    // background color is different than default
  }
  */

  /* (hlp->dwPaintType);
     AEHPT_QUOTE
     AEHPT_FOLD   */

  if (hlp->qm.lpQuote)
  {
    // quote item
    AECHARINDEX aeCh;
    int n;
    wchar_t wch;

    if (cookie->nBracketType >= 0)
    {
      wch = getBracketsPairW(cookie->nBracketType)[0]; // left
      CopyMemory1(&aeCh, &hlp->qm.crQuoteStart.ciMin, sizeof(AECHARINDEX));
      if (aeCh.nLine == hlp->qm.crQuoteStart.ciMax.nLine)
        n = hlp->qm.crQuoteStart.ciMax.nCharInLine;
      else
        n = aeCh.lpLine->nLineLen;
      while ( aeCh.nCharInLine < n )
      {
        if ( aeCh.lpLine->wpLine[aeCh.nCharInLine] == wch )
        {
          cookie->pos1 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET,
                                   0, (LPARAM) &aeCh);
          break;
        }
        ++aeCh.nCharInLine;
      }
    }
    /*else
    {
      cookie->pos1 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET,
                               0, (LPARAM) &hlp->qm.crQuoteStart.ciMin);
    }*/

    if (cookie->pos1 >= 0)
    {
      if (AEC_IndexCompare(&hlp->qm.crQuoteEnd.ciMin, &hlp->qm.crQuoteEnd.ciMax) == 0)
      {
        // no ending quote symbol
        cookie->pos2 = -1;
      }
      else
      {
        if (cookie->nBracketType >= 0)
        {
          wch = getBracketsPairW(cookie->nBracketType)[1]; // right
          CopyMemory1(&aeCh, &hlp->qm.crQuoteEnd.ciMax, sizeof(AECHARINDEX));
          if (aeCh.nLine == hlp->qm.crQuoteEnd.ciMin.nLine)
            n = hlp->qm.crQuoteEnd.ciMin.nCharInLine;
          else
            n = 0;
          while ( --aeCh.nCharInLine >= n )
          {
            if ( aeCh.lpLine->wpLine[aeCh.nCharInLine] == wch )
            {
              cookie->pos2 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET,
                                       0, (LPARAM) &aeCh);
              break;
            }
          }
        }
        /*else
        {
          cookie->pos2 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET,
                                   0, (LPARAM) &hlp->qm.crQuoteEnd.ciMax);
          --cookie->pos2; // because the range is [ciMin; ciMax)
        }*/
      }
    }
  }

  if ((cookie->pos1 >= 0) && (cookie->pos2 >= 0))
    cookie->nResult = ghlrPair;
  else
    cookie->nResult = ghlrSingleChar;

  cookie->chd.dwFontStyle = hlp->dwFontStyle;
  if (hlp->dwActiveText != (DWORD)(-1))
    cookie->chd.dwActiveTextColor = hlp->dwActiveText;
  if (hlp->dwActiveBk != (DWORD)(-1))
    cookie->chd.dwActiveBkColor = hlp->dwActiveBk;

  if (hlp->mrm.lpMarkRange)
  {
    // mark range found
    if (hlp->mrm.lpMarkRange->crText != (DWORD)(-1))
      cookie->chd.dwActiveTextColor = hlp->mrm.lpMarkRange->crText;
    if (hlp->mrm.lpMarkRange->crBk != (DWORD)(-1))
      cookie->chd.dwActiveBkColor = hlp->mrm.lpMarkRange->crBk;
    if ((g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_STYLE] & XBR_HSF_REDRAWCODER) == 0)
    {
      cookie->nResult = ghlrDoNotHighlight;
    }
  }

  return 0;
}

// custom implementation for getting matching brackets _around_ the given position
static DWORD CALLBACK GetAkelEditHighlightCallback2(UINT_PTR dwCookie, AECHARRANGE *crAkelRange, CHARRANGE64 *crRichRange, AEHLPAINT *hlp)
{
  INT_X nQmPos1 = -1, nQmPos2 = -1;
  int nLeftBrType = tbtNone, nRightBrType = tbtNone;
  wchar_t wch1, wch2;
  tGetHighlightIndexesCookie* cookie;
  AECHARINDEX* paeCh;

  (crRichRange); // unreferenced parameter
  (crAkelRange); // unreferenced parameter

  cookie = (tGetHighlightIndexesCookie *) dwCookie;
  
  CharacterHighlightData_Clear(&cookie->chd);

  if (hlp->qm.lpQuote)
  {
    // quote item
    paeCh = &hlp->qm.crQuoteStart.ciMin;
    nQmPos1 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM) paeCh);
    wch1 = getCharAt(hActualEditWnd, nQmPos1);
    ++nQmPos1; // after a left quote, if any
    nLeftBrType = getLeftBracketTypeEx(wch1, BTF_HIGHLIGHT);
    if (nLeftBrType != tbtNone)
    {
      paeCh = &hlp->qm.crQuoteEnd.ciMax;
      nQmPos2 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM) paeCh);
      --nQmPos2; // before a right quote, if any
      wch2 = getCharAt(hActualEditWnd, nQmPos2);
      nRightBrType = getRightBracketTypeEx(wch2, BTF_HIGHLIGHT);
    }
  }

  if (nLeftBrType != tbtNone && nLeftBrType == nRightBrType)
  {
    cookie->pos1 = nQmPos1;
    cookie->pos2 = nQmPos2;
    cookie->nBracketType = nLeftBrType;
    cookie->nResult = ghlrPair;
  }
  else
  {
    cookie->pos1 = -1;
    cookie->pos2 = -1;
    cookie->nBracketType = -1;
    cookie->nResult = ghlrNone;
  }

  return 0;
}

static void GetFoldFromAkelEdit(const INT_X nCharacterPosition, tGetHighlightIndexesCookie* pCookie)
{
  pCookie->pos1 = -1;
  pCookie->pos2 = -1;
  pCookie->nResult = ghlrNone;
  pCookie->nBracketType = -1;
  CharacterHighlightData_Clear(&pCookie->chd);

  if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_XMODE] & XBR_HXM_AKELFOLD)
  {
    AEFINDFOLD ff;

    ff.dwFlags = AEFF_FINDOFFSET | AEFF_FOLDSTART | AEFF_FOLDEND | AEFF_RECURSE;
    ff.dwFindIt = nCharacterPosition;
    ff.lpParent = NULL;
    ff.lpPrevSubling = NULL;

    SendMessage(hActualEditWnd, AEM_FINDFOLD, (WPARAM) &ff, 0);
    if (ff.lpParent)
    {
      // fold found
      INT_X nFmPos1, nFmPos2;
      int nLeftBrType, nRightBrType;
      wchar_t wch1, wch2;

      nFmPos1 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM) &ff.lpParent->lpMinPoint->ciPoint);
      wch1 = getCharAt(hActualEditWnd, nFmPos1);
      ++nFmPos1; // after the left bracket, if any
      nLeftBrType = getLeftBracketTypeEx(wch1, BTF_HIGHLIGHT);
      if (nLeftBrType == tbtNone)
        return; // not a bracket

      nFmPos2 = (INT_X) SendMessage(hActualEditWnd, AEM_INDEXTORICHOFFSET, 0, (LPARAM) &ff.lpParent->lpMaxPoint->ciPoint);
      nFmPos2 += ff.lpParent->lpMaxPoint->nPointLen;
      --nFmPos2; // before the right bracket, if any
      wch2 = getCharAt(hActualEditWnd, nFmPos2);
      nRightBrType = getRightBracketTypeEx(wch2, BTF_HIGHLIGHT);
      if (nRightBrType != nLeftBrType)
        return; // not a bracket pair
      
      pCookie->pos1 = nFmPos1;
      pCookie->pos2 = nFmPos2;
      pCookie->nBracketType = nLeftBrType;
      pCookie->nResult = ghlrPair;
    }
  }
}

static void GetHighlightDataFromAkelEdit(const INT_X nCharacterPosition, tGetHighlightIndexesCookie* pCookieHL, AEGetHighLightCallback pfnHighlightCallback)
{
  pCookieHL->pos1 = -1;
  pCookieHL->pos2 = -1;
  pCookieHL->nResult = ghlrNone;
  CharacterHighlightData_Clear(&pCookieHL->chd);

  if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_XMODE] & XBR_HXM_AKELHIGHLIGHT)
  {
    AEGETHIGHLIGHT aehl;

    aehl.dwCookie = (UINT_PTR) pCookieHL;
    aehl.dwError = 0;
    aehl.dwFlags = 0/*AEGHF_NOSELECTION | AEGHF_NOACTIVELINETEXT | AEGHF_NOACTIVELINEBK*/;
    if (pfnHighlightCallback == GetAkelEditHighlightCallback2)
    {
      aehl.dwFlags |= AEGHF_NOSELECTION;
    }
    aehl.lpCallback = pfnHighlightCallback;
    SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX,
      (WPARAM) nCharacterPosition, (LPARAM) &aehl.crText.ciMin);
    AEC_NextCharEx(&aehl.crText.ciMin, &aehl.crText.ciMax);
    // the range is [ciMin, ciMax)
    SendMessage(hActualEditWnd, AEM_HLGETHIGHLIGHT, 0, (LPARAM)&aehl);
  }
}

static DWORD GetInSelectionState(const INT_X nCharacterPosition, const CHARRANGE_X* pSelection)
{
  if ((nCharacterPosition >= pSelection->cpMin) &&
      (nCharacterPosition < pSelection->cpMax))
  {
    return XBR_STATEFLAG_INSELECTION;
  }
  else
    return 0;
}

static int GetAkelEditHighlightInfo(const int nHighlightIndex, const INT_X nCharacterPosition,
             const int nBracketType, const BOOL bRightBracket, const CHARRANGE_X* pSelection)
{
  tGetHighlightIndexesCookie cookie;
  tGetHighlightIndexesCookie cookieHL;
  BOOL bTag2;

  bTag2 = FALSE;
  cookie.pos1 = -1;
  cookie.pos2 = -1;
  cookie.nResult = ghlrNone;
  cookie.nBracketType = nBracketType;
  CharacterHighlightData_Clear(&cookie.chd);
  cookie.chd.dwState |= GetInSelectionState(nCharacterPosition, pSelection);

  // first we check for a fold...
  if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_XMODE] & XBR_HXM_AKELFOLD)
  {
    AEFINDFOLD ff;

    if (bRightBracket)
    {
      ff.dwFlags = AEFF_FINDOFFSET | AEFF_FOLDSTART;
      ff.dwFindIt = nCharacterPosition - 1;
    }
    else
    {
      ff.dwFlags = AEFF_FINDOFFSET | AEFF_FOLDEND;
      ff.dwFindIt = nCharacterPosition + 1;
    }
    ff.lpParent = NULL;
    ff.lpPrevSubling = NULL;

    SendMessage(hActualEditWnd, AEM_FINDFOLD, (WPARAM) &ff, 0);
    if (ff.lpParent)
    {
      // fold found
      if ((ff.lpParent->lpMaxPoint->nPointLen > 0) &&
          (ff.lpParent->lpMinPoint->nPointLen > 0))
      {
        const wchar_t* p;
        int n;
        int i;
        wchar_t wch;

        if (bRightBracket)
        {
          //  ... }
          cookie.pos2 = (INT_X) ff.lpParent->lpMaxPoint->nPointOffset;
          if (cookie.pos2 >= 0)
            cookie.pos2 += (ff.lpParent->lpMaxPoint->nPointLen - 1);

          if (cookie.pos2 == nCharacterPosition)
          {
            const AEPOINT* aePt = ff.lpParent->lpMinPoint;
            p = aePt->ciPoint.lpLine->wpLine + aePt->ciPoint.nCharInLine;
            n = aePt->ciPoint.lpLine->nLineLen - aePt->ciPoint.nCharInLine;
            if (n > aePt->nPointLen)
              n = aePt->nPointLen;
            i = 0;
            wch = getBracketsPairW(nBracketType)[0]; // left bracket

            while (i < n)
            {
              if (p[i] == wch)
              {
                cookie.pos1 = (INT_X) aePt->nPointOffset;
                cookie.pos1 += i;
                break;
              }
              ++i;
            }

            if (cookie.pos1 >= 0)
            {
              cookie.nResult = ghlrPair;
              cookie.chd.dwFontStyle = ff.lpParent->dwFontStyle;
              if (ff.lpParent->crText != (DWORD)(-1))
                cookie.chd.dwActiveTextColor = ff.lpParent->crText;
              if (ff.lpParent->crBk != (DWORD)(-1))
                cookie.chd.dwActiveBkColor = ff.lpParent->crBk;
            }
            else
              cookie.pos2 = -1;
          }
          else
            cookie.pos2 = -1;
        }
        else
        {
          //  { ...
          cookie.pos1 = (INT_X) ff.lpParent->lpMinPoint->nPointOffset;
          if (cookie.pos1 == nCharacterPosition)
          {
            const AEPOINT* aePt = ff.lpParent->lpMaxPoint;
            p = aePt->ciPoint.lpLine->wpLine + aePt->ciPoint.nCharInLine;
            n = aePt->ciPoint.lpLine->nLineLen - aePt->ciPoint.nCharInLine;
            if (n > aePt->nPointLen)
              n = aePt->nPointLen;
            wch = getBracketsPairW(nBracketType)[1]; // right bracket

            while (--n >= 0)
            {
              if (p[n] == wch)
              {
                cookie.pos2 = (INT_X) aePt->nPointOffset;
                cookie.pos2 += n;
                break;
              }
            }

            if (cookie.pos2 >= 0)
            {
              cookie.nResult = ghlrPair;
              cookie.chd.dwFontStyle = ff.lpParent->dwFontStyle;
              if (ff.lpParent->crText != (DWORD)(-1))
                cookie.chd.dwActiveTextColor = ff.lpParent->crText;
              if (ff.lpParent->crBk != (DWORD)(-1))
                cookie.chd.dwActiveBkColor = ff.lpParent->crBk;
            }
            else
              cookie.pos1 = -1;
          }
          else
            cookie.pos1 = -1;
        }
      }

      if (cookie.pos2 >= 0)
      {
        if (nBracketType == tbtTag)
        {
          // < ... >
          WCHAR wch;

          if (bRightBracket)
          {
            if (ff.lpParent->lpMaxPoint->nPointLen > 1)
            {
              // maybe < ... [<]...>
              const AECHARINDEX* aeCh = &ff.lpParent->lpMaxPoint->ciPoint;
              wch = aeCh->lpLine->wpLine[aeCh->nCharInLine];
              if (wch == L'<')
              {
                // yes, < ... [<]...>
                cookie.pos1 = (INT_X) ff.lpParent->lpMaxPoint->nPointOffset;
              }
            }
          }
          else
          {
            const AECHARINDEX* aeCh = &ff.lpParent->lpMaxPoint->ciPoint;
            wch = aeCh->lpLine->wpLine[aeCh->nCharInLine];
            if (wch == L'<')
            {
              // < ... <
              if (ff.lpParent->lpMinPoint->nPointLen > 1)
              {
                // maybe <...[>] ... <
                cookie.pos2 = (INT_X) ff.lpParent->lpMinPoint->nPointOffset;
                cookie.pos2 += (ff.lpParent->lpMinPoint->nPointLen /*- 1*/);

                wch = getCharAt(hActualEditWnd, cookie.pos2);

                if (wch != L'>')
                {
                  // not <...[>] ... <
                  cookie.pos2 = -1;
                  cookie.nResult = ghlrSingleChar;
                }
              }
              else
              {
                cookie.pos2 = -1;
                cookie.nResult = ghlrSingleChar;
              }
            }
            /*else
              cookie.pos2 += (ff.lpParent->lpMaxPoint->nPointLen - 1);*/
          }

          if (cookie.pos2 >= 0)
          {
            wch = getCharAt(hActualEditWnd, cookie.pos2 - 1);

            if (wch == L'/')
            {
              // < ... />
              bTag2 = TRUE;
            }
          }
        }
        /*else
        {
          if (!bRightBracket)
            cookie.pos2 += (ff.lpParent->lpMaxPoint->nPointLen - 1);
        }*/
      }
    }
  }

  // ...then we check for a highlight info...
  cookieHL.nBracketType = nBracketType;
  GetHighlightDataFromAkelEdit(nCharacterPosition, &cookieHL, GetAkelEditHighlightCallback);

  if (cookie.nResult == ghlrPair)
  {
    // fold was found initially
    if (cookieHL.nResult == ghlrDoNotHighlight)
    {
      // do not highlight
      cookie.nResult = ghlrDoNotHighlight;
      cookie.chd.dwState |= XBR_STATEFLAG_DONOTHL;
    }
    else
    {
      cookie.chd.dwFontStyle = cookieHL.chd.dwFontStyle;
      if (cookieHL.chd.dwActiveTextColor != XBR_FONTCOLOR_UNDEFINED)
        cookie.chd.dwActiveTextColor = cookieHL.chd.dwActiveTextColor;
      if (cookieHL.chd.dwActiveBkColor != XBR_FONTCOLOR_UNDEFINED)
        cookie.chd.dwActiveBkColor = cookieHL.chd.dwActiveBkColor;
    }
  }
  else
  {
    if ((nCharacterPosition == cookieHL.pos1) ||
        (nCharacterPosition == cookieHL.pos2))
    {
      cookie.pos1 = cookieHL.pos1;
      cookie.pos2 = cookieHL.pos2;
      cookie.nResult = cookieHL.nResult;
    }
    else
    {
      cookie.nResult = ghlrSingleChar;
    }
    cookie.chd.dwFontStyle = cookieHL.chd.dwFontStyle;
    if (cookieHL.chd.dwActiveTextColor != XBR_FONTCOLOR_UNDEFINED)
      cookie.chd.dwActiveTextColor = cookieHL.chd.dwActiveTextColor;
    if (cookieHL.chd.dwActiveBkColor != XBR_FONTCOLOR_UNDEFINED)
      cookie.chd.dwActiveBkColor = cookieHL.chd.dwActiveBkColor;
  }

  // ...finally...
  switch (cookie.nResult)
  {
    case ghlrNone:
      break;
    case ghlrSingleChar:
    {
      CharacterInfo_Add(hgltCharacterInfo, nCharacterPosition, &cookie.chd);
      break;
    }
    case ghlrPair:
    case ghlrDoNotHighlight:
    {
      if (bTag2)
      {
        BOOL bReadyHL = FALSE;

        // tbtTag2
        IndexesToHighlight[nHighlightIndex] = cookie.pos1;
        IndexesToHighlight[nHighlightIndex+1] = cookie.pos2 - 1;
        IndexesToHighlight[nHighlightIndex+2] = cookie.pos2;

        if ((cookie.chd.dwState & XBR_STATEFLAG_DONOTHL) ||
            ((cookie.chd.dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(cookie.pos1, pSelection)))
        {
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos1, &cookie.chd);
        }
        else
        {
          bReadyHL = TRUE;
          cookieHL.nBracketType = -1;
          GetHighlightDataFromAkelEdit(cookie.pos1, &cookieHL, GetAkelEditHighlightCallback);
          cookieHL.chd.dwState |= GetInSelectionState(cookie.pos1, pSelection);
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos1, &cookieHL.chd);
        }

        if ((cookie.chd.dwState & XBR_STATEFLAG_DONOTHL) ||
            ((cookie.chd.dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(cookie.pos2 - 1, pSelection)))
        {
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos2 - 1, &cookie.chd);
        }
        else
        {
          if (!bReadyHL)
          {
            bReadyHL = TRUE;
            cookieHL.nBracketType = -1;
            GetHighlightDataFromAkelEdit(cookie.pos2 - 1, &cookieHL, GetAkelEditHighlightCallback);
            cookieHL.chd.dwState |= GetInSelectionState(cookie.pos2 - 1, pSelection);
          }
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos2 - 1, &cookieHL.chd);
        }

        if ((cookie.chd.dwState & XBR_STATEFLAG_DONOTHL) ||
            ((cookie.chd.dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(cookie.pos2, pSelection)))
        {
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos2, &cookie.chd);
        }
        else
        {
          if (!bReadyHL)
          {
            bReadyHL = TRUE;
            cookieHL.nBracketType = -1;
            GetHighlightDataFromAkelEdit(cookie.pos2, &cookieHL, GetAkelEditHighlightCallback);
            cookieHL.chd.dwState |= GetInSelectionState(cookie.pos2, pSelection);
          }
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos2, &cookieHL.chd);
        }
      }
      else
      {
        BOOL bReadyHL = FALSE;

        IndexesToHighlight[nHighlightIndex] = cookie.pos1;
        IndexesToHighlight[nHighlightIndex+1] = cookie.pos2;

        if ((cookie.chd.dwState & XBR_STATEFLAG_DONOTHL) ||
            ((cookie.chd.dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(cookie.pos1, pSelection)))
        {
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos1, &cookie.chd);
        }
        else
        {
          bReadyHL = TRUE;
          cookieHL.nBracketType = -1;
          GetHighlightDataFromAkelEdit(cookie.pos1, &cookieHL, GetAkelEditHighlightCallback);
          cookieHL.chd.dwState |= GetInSelectionState(cookie.pos1, pSelection);
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos1, &cookieHL.chd);
        }

        if ((cookie.chd.dwState & XBR_STATEFLAG_DONOTHL) ||
            ((cookie.chd.dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(cookie.pos2, pSelection)))
        {
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos2, &cookie.chd);
        }
        else
        {
          if (!bReadyHL)
          {
            bReadyHL = TRUE;
            cookieHL.nBracketType = -1;
            GetHighlightDataFromAkelEdit(cookie.pos2, &cookieHL, GetAkelEditHighlightCallback);
            cookieHL.chd.dwState |= GetInSelectionState(cookie.pos2, pSelection);
          }
          CharacterInfo_Add(hgltCharacterInfo, cookie.pos2, &cookieHL.chd);
        }
      }

      updateCurrentBracketsIndexes(cookie.pos1, cookie.pos2);
      break;
    }
  }

  return cookie.nResult;
}

static BOOL NearestBr_MatchCachedIndexes(const int action, const INT_X nCharacterPosition, CHARRANGE_X* out_cr)
{
  if ( NearestBracketsIndexes[0] != -1 )
  {
    if ( nCharacterPosition == NearestBracketsIndexes[0] ) // |(
    {
      if ( NearestBracketsIndexes[2] == tbtTagInv )
        return FALSE;  //  skip  |>...<

      if ( action == XBRA_SELTO_NEARBR )
      {
        out_cr->cpMin = NearestBracketsIndexes[0];     // |(...
        out_cr->cpMax = NearestBracketsIndexes[1] + 1; //    ...)|
      }
      else
      {
        out_cr->cpMin = NearestBracketsIndexes[1] + 1; // )|
        out_cr->cpMax = out_cr->cpMin;
      }
      return TRUE;
    }

    if ( nCharacterPosition == NearestBracketsIndexes[0] + 1 ) // (|
    {
      if ( action == XBRA_SELTO_NEARBR )
      {
        if ( (g_dwOptions[OPT_DWORD_NEARESTBR_SELTO_FLAGS] & XBR_NBR_SELTO_OUTERPOS) &&
             (NearestBracketsIndexes[2] != tbtTagInv) )
        {
          out_cr->cpMin = NearestBracketsIndexes[0];     // |(...
          out_cr->cpMax = NearestBracketsIndexes[1] + 1; //    ...)|
        }
        else
        {
          out_cr->cpMin = NearestBracketsIndexes[0] + 1; // (|...
          out_cr->cpMax = NearestBracketsIndexes[1];     //    ...|)
        }
      }
      else
      {
        if ( (g_dwOptions[OPT_DWORD_NEARESTBR_GOTO_FLAGS] & XBR_NBR_GOTO_OUTERPOS) &&
             (NearestBracketsIndexes[2] != tbtTagInv) )
          out_cr->cpMin = NearestBracketsIndexes[1] + 1; // )|
        else
          out_cr->cpMin = NearestBracketsIndexes[1]; // |)
        out_cr->cpMax = out_cr->cpMin;
      }
      return TRUE;
    }

    if ( nCharacterPosition == NearestBracketsIndexes[1] ) // |)
    {
      if ( action == XBRA_SELTO_NEARBR )
      {
        if ( (g_dwOptions[OPT_DWORD_NEARESTBR_SELTO_FLAGS] & XBR_NBR_SELTO_OUTERPOS) &&
             (NearestBracketsIndexes[2] != tbtTagInv) )
        {
          out_cr->cpMin = NearestBracketsIndexes[0];     // |(...
          out_cr->cpMax = NearestBracketsIndexes[1] + 1; //    ...)|
        }
        else
        {
          out_cr->cpMin = NearestBracketsIndexes[0] + 1; // (|...
          out_cr->cpMax = NearestBracketsIndexes[1];     //    ...|)
        }
      }
      else
      {
        if ( (g_dwOptions[OPT_DWORD_NEARESTBR_GOTO_FLAGS] & XBR_NBR_GOTO_OUTERPOS) &&
             (NearestBracketsIndexes[2] != tbtTagInv) )
          out_cr->cpMin = NearestBracketsIndexes[0]; // |(
        else
          out_cr->cpMin = NearestBracketsIndexes[0] + 1; // (|
        out_cr->cpMax = out_cr->cpMin;
      }
      return TRUE;
    }

    if ( nCharacterPosition == NearestBracketsIndexes[1] + 1 ) // )|
    {
      if ( NearestBracketsIndexes[2] == tbtTagInv )
        return FALSE;  //  skip  >...<|

      if ( action == XBRA_SELTO_NEARBR )
      {
        out_cr->cpMin = NearestBracketsIndexes[0];     // |(...
        out_cr->cpMax = NearestBracketsIndexes[1] + 1; //    ...)|
      }
      else
      {
        out_cr->cpMin = NearestBracketsIndexes[0]; // |(
        out_cr->cpMax = out_cr->cpMin;
      }
      return TRUE;
    }
  }

  return FALSE;
}

enum eAtBrChar {
  abcNone        = 0x00,
  abcLeftBr      = 0x01, // left:  (
  abcRightBr     = 0x02, // right: )
  abcDetectBr    = 0x04, // can be either left or right
  abcBrIsOnLeft  = 0x10, // on left:  (|  or  )|
  abcBrIsOnRight = 0x20  // on right: |(  or  |)
};

enum eFoldQuoteFlags {
  fqfDoNotCheckFold  = 0x1000,
  fqfDoNotCheckQuote = 0x2000
};

static BOOL NearestBr_GetFoldOrQuoteFromAkelEdit(const INT_X nPos, const unsigned int nAtBrFQFlags, tGetHighlightIndexesCookie* out_brCookie)
{
  out_brCookie->nResult = ghlrNone;
  out_brCookie->pos1 = -1;
  out_brCookie->pos2 = -1;

  if ( g_bAkelEdit )
  {
    INT_X nFmPos1 = -1, nFmPos2 = -1;
    INT_X nQmPos1 = -1, nQmPos2 = -1;
    int nFmBrType = tbtNone, nQmBrType = tbtNone;
    tGetHighlightIndexesCookie cookie;

    if ( (nAtBrFQFlags & fqfDoNotCheckFold) == 0 )
    {
      // check for a fold...
      // (we might use AEM_HLGETHIGHLIGHT for both folds and quotes,
      //  but unfortunately it does not work at the end of a line)
      cookie.nResult = ghlrNone;
      GetFoldFromAkelEdit(nPos, &cookie);
      if ( (cookie.nResult == ghlrNone) && (nAtBrFQFlags & abcDetectBr) )
      {
        INT_X nTryPos;
        if ( nAtBrFQFlags & abcBrIsOnLeft )
          nTryPos = nPos - 1;  //  trying  "..."|  ->  "...|"
        else
          nTryPos = nPos + 1;  //  trying  |"..."  ->  "|..."
        GetFoldFromAkelEdit(nTryPos, &cookie);
      }
      if ( cookie.nResult == ghlrPair )
      {
        nFmPos1 = cookie.pos1;
        nFmPos2 = cookie.pos2;
        nFmBrType = cookie.nBracketType;
      }
    }
    
    if ( (nAtBrFQFlags & fqfDoNotCheckQuote) == 0 )
    {
      // check for a quote...
      cookie.nResult = ghlrNone;
      GetHighlightDataFromAkelEdit(nPos, &cookie, GetAkelEditHighlightCallback2);
      if ( (cookie.nResult == ghlrNone) && (nAtBrFQFlags & abcDetectBr) )
      {
        INT_X nTryPos;
        if ( nAtBrFQFlags & abcBrIsOnLeft )
          nTryPos = nPos - 1;  //  trying  "..."|  ->  "...|"
        else
          nTryPos = nPos + 1;  //  trying  |"..."  ->  "|..."
        GetHighlightDataFromAkelEdit(nTryPos, &cookie, GetAkelEditHighlightCallback2);
      }
      if ( cookie.nResult == ghlrPair )
      {
        nQmPos1 = cookie.pos1;
        nQmPos2 = cookie.pos2;
        nQmBrType = cookie.nBracketType;
      }
    }

    // pick the nearest range, if any... 
    if (nFmPos1 >= 0 && nFmPos2 >= 0)
    {
      if (nQmPos1 >= 0 && nQmPos2 >= 0)
      {
        if (nFmPos2 - nFmPos1 < nQmPos2 - nQmPos1)
        {
          out_brCookie->pos1 = nFmPos1;
          out_brCookie->pos2 = nFmPos2;
          out_brCookie->nBracketType = nFmBrType;
        }
        else
        {
          out_brCookie->pos1 = nQmPos1;
          out_brCookie->pos2 = nQmPos2;
          out_brCookie->nBracketType = nQmBrType;
        }
      }
      else
      {
        out_brCookie->pos1 = nFmPos1;
        out_brCookie->pos2 = nFmPos2;
        out_brCookie->nBracketType = nFmBrType;
      }
      out_brCookie->nResult = ghlrPair;
    }
    else if (nQmPos1 >= 0 && nQmPos2 >= 0)
    {
      out_brCookie->pos1 = nQmPos1;
      out_brCookie->pos2 = nQmPos2;
      out_brCookie->nBracketType = nQmBrType;
      out_brCookie->nResult = ghlrPair;
    }
  }

  return (out_brCookie->nResult == ghlrPair);
}

static unsigned int NearestBr_IsAtBracketCharacter(const INT_X nCharacterPosition, const INT_X nTextLength,
                                                   int* out_nBrType, int* out_nDupDirection)
{
  INT_X nPos;
  INT_X nLen;
  int nBrType = tbtNone;
  int nDupDir = DP_NONE;
  int nDetectBrType = tbtNone;
  unsigned int nDetectBrAtPos = abcNone;
  unsigned int uFlags;
  wchar_t wch[4] = { 0, 0, 0, 0 }; // { prev_wch, current_wch, 0, 0 }
  tGetHighlightIndexesCookie c;

  *out_nBrType = tbtNone;
  *out_nDupDirection = DP_NONE;

  if ( nCharacterPosition == 0 )
  {
    nPos = 0;
    if ( nTextLength == 0 )
      nLen = 0; // { 0, 0 }
    else
      nLen = 1; // { 0, current_wch }
  }
  else
  {
    nPos = nCharacterPosition - 1;
    nLen = nTextLength - nPos;
    if ( nLen > 2 )
      nLen = 2; // { prev_wch, current_wch }
  }

  if ( g_bOldWindows )
  {
    char ch[4] = { 0, 0, 0, 0 };
    AnyRichEdit_GetTextAt(hActualEditWnd, nPos, nLen, nCharacterPosition == 0 ? ch + 1 : ch);
    wch[0] = char2wchar(ch[0]);
    wch[1] = char2wchar(ch[1]);
  }
  else
  {
    AnyRichEdit_GetTextAtW(hActualEditWnd, nPos, nLen, nCharacterPosition == 0 ? wch + 1 : wch);
  }

  if ( wch[0] != 0 )
  {
    uFlags = BTF_HIGHLIGHT;
    if ( wch[1] != 0 && wch[1] != L'\r' )
      uFlags |= BTF_CHECK_TAGINV;
    nBrType = getLeftBracketTypeEx(wch[0], uFlags); // prev_wch
    if ( nBrType != tbtNone && !isEscapedPosEx(nCharacterPosition - 1) ) //  (|
    {
      if ( isDuplicatedPair(nBrType) )
      {
        if ( NearestBr_GetFoldOrQuoteFromAkelEdit(nCharacterPosition, abcDetectBr | abcBrIsOnLeft | fqfDoNotCheckFold, &c) )
        {
          if ( c.pos1 == nCharacterPosition )
          {
            *out_nBrType = nBrType;
            *out_nDupDirection = DP_FORWARD;
            return (abcLeftBr | abcBrIsOnLeft);
          }
          if ( c.pos2 == nCharacterPosition - 1 )
          {
            *out_nBrType = nBrType;
            *out_nDupDirection = DP_BACKWARD;
            return (abcRightBr | abcBrIsOnLeft);
          }
        }

        nDupDir = getDuplicatedPairDirection(nCharacterPosition - 1, wch[0]);
        if ( nDupDir == DP_FORWARD || nDupDir == DP_MAYBEFORWARD )
        {
          *out_nDupDirection = nDupDir;
        }
        else if ( nDupDir == DP_DETECT )
        {
          nDetectBrAtPos = abcBrIsOnLeft;
          nDetectBrType = nBrType;
          nBrType = tbtNone;
        }
        else
          nBrType = tbtNone;
      }
      if ( nBrType != tbtNone )
      {
        *out_nBrType = nBrType;
        return (abcLeftBr | abcBrIsOnLeft);
      }
    }
  }

  if ( wch[1] != 0 )
  {
    uFlags = BTF_HIGHLIGHT; // not using BTF_CHECK_TAGINV in case of  |>
    nBrType = getLeftBracketTypeEx(wch[1], uFlags); // current_wch
    if ( nBrType != tbtNone && !isEscapedPosEx(nCharacterPosition) ) //  |(
    {
      if ( isDuplicatedPair(nBrType) )
      {
        if ( NearestBr_GetFoldOrQuoteFromAkelEdit(nCharacterPosition, abcDetectBr | abcBrIsOnRight | fqfDoNotCheckFold, &c) )
        {
          if ( c.pos1 == nCharacterPosition + 1 )
          {
            *out_nBrType = nBrType;
            *out_nDupDirection = DP_FORWARD;
            return (abcLeftBr | abcBrIsOnRight);
          }
          if ( c.pos2 == nCharacterPosition )
          {
            *out_nBrType = nBrType;
            *out_nDupDirection = DP_BACKWARD;
            return (abcRightBr | abcBrIsOnRight);
          }
        }

        nDupDir = getDuplicatedPairDirection(nCharacterPosition, wch[1]);
        if ( nDupDir == DP_FORWARD || nDupDir == DP_MAYBEFORWARD )
        {
          *out_nDupDirection = nDupDir;
        }
        else if ( nDupDir == DP_DETECT )
        {
          nDetectBrAtPos = abcBrIsOnRight;
          nDetectBrType = nBrType;
          nBrType = tbtNone;
        }
        else
          nBrType = tbtNone;
      }
      if ( nBrType != tbtNone )
      {
        *out_nBrType = nBrType;
        return (abcLeftBr | abcBrIsOnRight);
      }
    }
  }

  if ( wch[1] != 0 )
  {
    uFlags = BTF_HIGHLIGHT;
    if ( wch[0] != 0 && wch[0] != L'\r' )
      uFlags |= BTF_CHECK_TAGINV;
    nBrType = getRightBracketTypeEx(wch[1], uFlags); // current_wch
    if ( nBrType != tbtNone && !isEscapedPosEx(nCharacterPosition) ) //  |)
    {
      if ( isDuplicatedPair(nBrType) )
      {
        nDupDir = getDuplicatedPairDirection(nCharacterPosition, wch[1]);
        if ( nDupDir == DP_BACKWARD || nDupDir == DP_MAYBEBACKWARD )
          *out_nDupDirection = nDupDir;
        else
          nBrType = tbtNone;
      }
      if ( nBrType != tbtNone )
      {
        *out_nBrType = nBrType;
        return (abcRightBr | abcBrIsOnRight);
      }
    }
  }

  if ( wch[0] != 0 )
  {
    uFlags = BTF_HIGHLIGHT; // not using BTF_CHECK_TAGINV in case of  <|
    nBrType = getRightBracketTypeEx(wch[0], uFlags); // prev_wch
    if ( nBrType != tbtNone && !isEscapedPosEx(nCharacterPosition - 1) ) //  )|
    {
      if ( isDuplicatedPair(nBrType) )
      {
        nDupDir = getDuplicatedPairDirection(nCharacterPosition - 1, wch[0]);
        if ( nDupDir == DP_BACKWARD || nDupDir == DP_MAYBEBACKWARD )
          *out_nDupDirection = nDupDir;
        else
          nBrType = tbtNone;
      }
      if ( nBrType != tbtNone )
      {
        *out_nBrType = nBrType;
        return (abcRightBr | abcBrIsOnLeft);
      }
    }
  }

  if ( nDetectBrType != tbtNone )
  {
    *out_nBrType = nDetectBrType;
    *out_nDupDirection = DP_DETECT;
    return (abcDetectBr | nDetectBrAtPos);
  }

  return (abcNone);
}

#define XBR_NEARESTBR_OCC_ITEMS 4

typedef struct sGetNearestBracketsState {
    INT_X nCharacterPosition;
    INT_X nTextLength;
    INT_X nLeftBrPos;
    INT_X nRightBrPos;
    int nLeftBrType;
    int nRightBrType;
    int nLeftDupDirection;
    int nRightDupDirection;
    CHARRANGE_X crCurrent;
    tOccurrenceItem occItems[XBR_NEARESTBR_OCC_ITEMS];
    tOccurrencesData occData;
} tGetNearestBracketsState;

enum eGetNearestBracketsFlags {
  gnbrfWiden = 0x02
};

static BOOL NearestBr_FindLeftBracket(INT_X nStartPos, const unsigned int flags, tGetNearestBracketsState* state, const tGetHighlightIndexesCookie* brCookie)
{
  INT_X   nStopPos;
  INT_X   nBrPos;
  int     nBrType;
  int     nDupPairDirection;
  BOOL    bBreak;
  wchar_t wch;
  tOccurrenceCookie cookie;
  AECHARINDEX       aeci;

  if ( state->nLeftBrType == tbtNone && nStartPos != 0 )
  {
    nStopPos = 0;
    if ( g_dwOptions[OPT_DWORD_NEARESTBR_MAX_LINES] != 0 )
    {
      int nLine;
      if ( g_bOldWindows )
        nLine = AnyRichEdit_ExLineFromChar(hActualEditWnd, nStartPos);
      else
        nLine = AnyRichEdit_ExLineFromCharW(hActualEditWnd, nStartPos);
      if ( ((DWORD) nLine) > (g_dwOptions[OPT_DWORD_NEARESTBR_MAX_LINES] - 1) )
      {
        nLine -= (g_dwOptions[OPT_DWORD_NEARESTBR_MAX_LINES] - 1);
        if ( g_bOldWindows )
          nStopPos = AnyRichEdit_LineIndex(hActualEditWnd, nLine);
        else
          nStopPos = AnyRichEdit_LineIndexW(hActualEditWnd, nLine);
      }
    }
    if ( brCookie->nResult == ghlrPair )
    {
      if ( (flags & gnbrfWiden) == 0 || (nStartPos >= brCookie->pos1 && nStartPos <= brCookie->pos2) )
      {
        // optimization: don't search outside of the range pos1...pos2
        if ( nStopPos < brCookie->pos1 )
        {
          nStopPos = brCookie->pos1;
          if ( brCookie->nBracketType == tbtTag || brCookie->nBracketType == tbtTag2 )
            --nStopPos; // including the '<' to be able to find the paired '>'
        }
      }
    }

    bBreak = FALSE;
    nBrPos = nStartPos - 1;
    if ( g_bAkelEdit )
    {
      if ( g_bOldWindows )
        SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) nBrPos, (LPARAM) &aeci);
      else
        SendMessageW(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) nBrPos, (LPARAM) &aeci);
    }
    for ( ; nBrPos >= nStopPos && !bBreak; --nBrPos )
    {
      if ( g_bAkelEdit )
      {
        #if NEARBR_CHARINDEX_LOOP_VERIFY
          wchar_t wch0;
        #endif

        wch = aeci.lpLine->wpLine[aeci.nCharInLine];
        if ( aeci.nCharInLine == 0 )
        {
          AEC_PrevLine(&aeci);
          if ( aeci.lpLine )
          {
            if ( aeci.lpLine->nLineBreak == AELB_WRAP )
              AEC_IndexDec(&aeci);
          }
          else
            bBreak = TRUE;
        }
        else
          AEC_IndexDec(&aeci);

        #if NEARBR_CHARINDEX_LOOP_VERIFY
          if ( g_bOldWindows )
            wch0 = char2wchar(AnyRichEdit_GetCharAt(hActualEditWnd, nBrPos));
          else
            wch0 = AnyRichEdit_GetCharAtW(hActualEditWnd, nBrPos);
          if (wch0 != wch)
          {
            if (wch == 0 && wch0 == L'\r')
            {
            }
            else
            {
              wch = wch;
              wch0 = wch0;

              #if NEARBR_CHARINDEX_LOOP_USE_DEBUGBREAK
                DebugBreak();
              #endif
            }
          }
        #endif
      }
      else
      {
        if ( g_bOldWindows )
          wch = char2wchar(AnyRichEdit_GetCharAt(hActualEditWnd, nBrPos));
        else
          wch = AnyRichEdit_GetCharAtW(hActualEditWnd, nBrPos);
      }
      nBrType = getLeftBracketTypeEx(wch, BTF_HIGHLIGHT | BTF_CHECK_TAGINV);
      if ( nBrType != tbtNone )
      {
        if ( isEscapedPosEx(nBrPos) )
        {
          nBrType = tbtNone;
        }
        else
        {
          if ( isDuplicatedPair(nBrType) )
          {
            tGetHighlightIndexesCookie c;
            if ( NearestBr_GetFoldOrQuoteFromAkelEdit(nBrPos, abcDetectBr | abcBrIsOnRight | fqfDoNotCheckFold, &c) )
            {
              if ( nBrPos == c.pos2 ) // really at "...|"
              {
                nBrPos = c.pos1 - 1;  //  "|...  ->  |"...
                if (nBrPos == 0)
                {
                  bBreak = TRUE;
                }
                else if ( g_bAkelEdit )
                {
                  if ( g_bOldWindows )
                    SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) (nBrPos - 1), (LPARAM) &aeci);
                  else
                    SendMessageW(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) (nBrPos - 1), (LPARAM) &aeci);
                }
                continue;
              }
            }

            nDupPairDirection = getDuplicatedPairDirection(nBrPos, wch);
            if ( nDupPairDirection == DP_FORWARD || nDupPairDirection == DP_MAYBEFORWARD )
            {
              // left quote found
              if ( !OccurrencesData_Contains(&state->occData, nBrType, &cookie) )
              {
                if ( OccurrencesData_IsEmpty(&state->occData) )
                {
                  state->nLeftBrType = nBrType;
                  state->nLeftBrPos = nBrPos + 1; // exclude the bracket itself
                  state->nLeftDupDirection = nDupPairDirection;
                  break; // OK, found
                }
                else
                {
                  // could be either a left quote inside (another) quotes
                  // or a left quote outside (another) quotes, not sure...
                  break;
                }
              }
              else
              {
                if ( getDirectionRank(nDupPairDirection, cookie.nDirection) >= getDirectionRank(DP_FORWARD, DP_DETECT) )
                  OccurrencesData_Decrement(&state->occData, nBrType);
                else
                  break; // can't be sure if it is a pair of quotes
              }
            }
            else if ( nDupPairDirection == DP_DETECT )
            {
              // can be either left or right quote, let's check...
              if ( OccurrencesData_Contains(&state->occData, nBrType, &cookie) )
              {
                if ( cookie.nDirection == DP_BACKWARD ) // getDirectionRank(DP_DETECT, DP_BACKWARD)
                  OccurrencesData_Decrement(&state->occData, nBrType); // looks like a left quote
                else
                  break; // can't be sure if it's a pair of quotes or not
              }
              else if ( state->nRightBrType == nBrType &&
                        state->nRightDupDirection == DP_BACKWARD ) // getDirectionRank(DP_DETECT, DP_BACKWARD)
              {
                state->nLeftBrType = nBrType;
                state->nLeftBrPos = nBrPos + 1; // exclude the bracket itself
                state->nLeftDupDirection = nDupPairDirection;
                break; // OK, found
              }
              else
              {
                // treating it as a right quote
                cookie.nPos = nBrPos + 1;
                cookie.nDirection = nDupPairDirection;
                if ( !OccurrencesData_Increment(&state->occData, nBrType, &cookie) )
                  break; // no more room
              }
            }
            else
            {
              // right quote found while looking for a left one
              cookie.nPos = nBrPos + 1;
              cookie.nDirection = nDupPairDirection;
              if ( !OccurrencesData_Increment(&state->occData, nBrType, &cookie) )
                break; // no more room
            }
          }
          else
          {
            if ( !OccurrencesData_Contains(&state->occData, nBrType, NULL) )
            {
              if ( OccurrencesData_IsEmpty(&state->occData) )
              {
                state->nLeftBrType = nBrType;
                state->nLeftBrPos = nBrPos + 1; // exclude the bracket itself
                state->nLeftDupDirection = DP_NONE;
                break; // OK, found
              }
              else
              {
                // could be either a left bracket inside quotes
                // or a left bracket outside quotes, not sure...
                break;
              }
            }
            else
              OccurrencesData_Decrement(&state->occData, nBrType);
          }
        }
      }
      else
      {
        nBrType = getRightBracketTypeEx(wch, BTF_HIGHLIGHT | BTF_CHECK_TAGINV);
        if ( nBrType != tbtNone )
        {
          if ( !isEscapedPosEx(nBrPos) )
          {
            // found a right bracket instead of a left bracket
            if ( !OccurrencesData_Increment(&state->occData, nBrType, NULL) )
              break; // no more room
          }
        }
      }
    }
  }

  return (state->nLeftBrType != tbtNone);
}

static void NearestBr_AdjustLeftBracketPos(tGetNearestBracketsState* state)
{
  if ( !OccurrencesData_IsEmpty(&state->occData) )
  {
    const tOccurrenceItem* poci;
    int i;

    for (i = 0; i < XBR_NEARESTBR_OCC_ITEMS; i++)
    {
      poci = &state->occData.pItems[i];
      if ( poci->nDepth != 0 && isDuplicatedPair(poci->nType) )
      {
        if ( state->nLeftBrType == tbtNone || state->nLeftBrPos < poci->cookies[0].nPos )
        {
          if ( poci->cookies[0].nDirection == DP_DETECT ) // skipping DP_*BACKWARD
          {
            state->nLeftBrPos = poci->cookies[0].nPos;
            state->nLeftDupDirection = poci->cookies[0].nDirection;
            state->nLeftBrType = poci->nType;
          }
        }
      }
    }

    if ( state->nLeftDupDirection != DP_NONE )
      OccurrencesData_Clear(&state->occData);
  }
}

static BOOL NearestBr_FindRightBracket(INT_X nStartPos, const unsigned int flags, tGetNearestBracketsState* state, const tGetHighlightIndexesCookie* brCookie)
{
  INT_X   nStopPos;
  INT_X   nBrPos;
  int     nBrType;
  int     nDupPairDirection;
  wchar_t wch;
  tOccurrenceCookie cookie;
  AECHARINDEX       aeci;

  if ( state->nRightBrType == tbtNone )
  {
    nStopPos = state->nTextLength;
    if ( g_dwOptions[OPT_DWORD_NEARESTBR_MAX_LINES] != 0 )
    {
      int nLine1, nLine2;
      if ( g_bOldWindows )
      {
        nLine1 = AnyRichEdit_ExLineFromChar(hActualEditWnd, nStartPos);
        nLine2 = AnyRichEdit_ExLineFromChar(hActualEditWnd, state->nTextLength);
      }
      else
      {
        nLine1 = AnyRichEdit_ExLineFromCharW(hActualEditWnd, nStartPos);
        nLine2 = AnyRichEdit_ExLineFromCharW(hActualEditWnd, state->nTextLength);
      }
      if ( ((DWORD) (nLine2 - nLine1)) > (g_dwOptions[OPT_DWORD_NEARESTBR_MAX_LINES] - 1) )
      {
        nLine2 = nLine1 + g_dwOptions[OPT_DWORD_NEARESTBR_MAX_LINES]; // - 1 + 1
        // to the end of nLine2 (= to the beginning of the next line)
        if ( g_bOldWindows )
          nStopPos = AnyRichEdit_LineIndex(hActualEditWnd, nLine2);
        else
          nStopPos = AnyRichEdit_LineIndexW(hActualEditWnd, nLine2);
      }
    }
    if ( brCookie->nResult == ghlrPair )
    {
      if ( (flags & gnbrfWiden) == 0 || (nStartPos >= brCookie->pos1 && nStartPos <= brCookie->pos2) )
      {
        // optimization: don't search outside of the range pos1...pos2
        if ( nStopPos > brCookie->pos2 )
        {
          nStopPos = brCookie->pos2;
          if ( brCookie->nBracketType == tbtTag || brCookie->nBracketType == tbtTag2 )
            ++nStopPos; // including the '>' to be able to find the paired '<'
        }
      }
    }

    nBrPos = nStartPos;
    if ( g_bAkelEdit )
    {
      if ( g_bOldWindows )
        SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) nBrPos, (LPARAM) &aeci);
      else
        SendMessageW(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) nBrPos, (LPARAM) &aeci);
    }
    for ( ; nBrPos < nStopPos; ++nBrPos )
    {
      if ( g_bAkelEdit )
      {
        #if NEARBR_CHARINDEX_LOOP_VERIFY
          wchar_t wch0;
        #endif

        wch = aeci.lpLine->wpLine[aeci.nCharInLine];
        if ( aeci.nCharInLine == aeci.lpLine->nLineLen )
        {
          AEC_NextLine(&aeci);
          if ( aeci.lpLine )
          {
            if ( aeci.lpLine->prev->nLineBreak == AELB_WRAP )
            {
              wch = aeci.lpLine->wpLine[aeci.nCharInLine];
              AEC_IndexInc(&aeci);
            }
          }
          else
            break;
        }
        else
          AEC_IndexInc(&aeci);

        #if NEARBR_CHARINDEX_LOOP_VERIFY
          if ( g_bOldWindows )
            wch0 = char2wchar(AnyRichEdit_GetCharAt(hActualEditWnd, nBrPos));
          else
            wch0 = AnyRichEdit_GetCharAtW(hActualEditWnd, nBrPos);
          if (wch0 != wch)
          {
            if (wch == 0 && wch0 == L'\r')
            {
            }
            else
            {
              wch = wch;
              wch0 = wch0;

              #if NEARBR_CHARINDEX_LOOP_USE_DEBUGBREAK
                DebugBreak();
              #endif
            }
          }
        #endif
      }
      else
      {
        if ( g_bOldWindows )
          wch = char2wchar(AnyRichEdit_GetCharAt(hActualEditWnd, nBrPos));
        else
          wch = AnyRichEdit_GetCharAtW(hActualEditWnd, nBrPos);
      }
      nBrType = getRightBracketTypeEx(wch, BTF_HIGHLIGHT | BTF_CHECK_TAGINV);
      if ( nBrType != tbtNone )
      {
        if ( isEscapedPosEx(nBrPos) )
        {
          nBrType = tbtNone;
        }
        else
        {
          if ( isDuplicatedPair(nBrType) )
          {
            tGetHighlightIndexesCookie c;
            if ( NearestBr_GetFoldOrQuoteFromAkelEdit(nBrPos, abcDetectBr | abcBrIsOnRight | fqfDoNotCheckFold, &c) )
            {
              if ( nBrPos + 1 == c.pos1 ) // really at |"..."
              {
                nBrPos = c.pos2;  //  ...|"
                if ( g_bAkelEdit )
                {
                  if ( g_bOldWindows )
                    SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) (nBrPos + 1), (LPARAM) &aeci);
                  else
                    SendMessageW(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) (nBrPos + 1), (LPARAM) &aeci);
                }
                continue;
              }
            }

            nDupPairDirection = getDuplicatedPairDirection(nBrPos, wch);
            if ( nDupPairDirection == DP_BACKWARD || nDupPairDirection == DP_MAYBEBACKWARD )
            {
              // right quote found
              if ( !OccurrencesData_Contains(&state->occData, nBrType, &cookie) )
              {
                if ( OccurrencesData_IsEmpty(&state->occData) )
                {
                  state->nRightBrType = nBrType;
                  state->nRightBrPos = nBrPos;
                  state->nRightDupDirection = nDupPairDirection;
                  break; // OK, found
                }
                else
                {
                  // could be either a right quote inside (another) quotes
                  // or a right quote outside (another) quotes, not sure...
                  break;
                }
              }
              else
              {
                if ( getDirectionRank(cookie.nDirection, nDupPairDirection) >= getDirectionRank(DP_DETECT, DP_BACKWARD) )
                  OccurrencesData_Decrement(&state->occData, nBrType);
                else
                  break; // can't be sure if it is a pair of quotes
              }
            }
            else if ( nDupPairDirection == DP_DETECT )
            {
              // can be either left or right quote, let's check...
              if ( OccurrencesData_Contains(&state->occData, nBrType, &cookie) )
              {
                if ( cookie.nDirection == DP_FORWARD ) // getDirectionRank(DP_FORWARD, DP_DETECT)
                  OccurrencesData_Decrement(&state->occData, nBrType); // looks like a right quote
                else
                  break; // can't be sure if it's a pair of quotes or not
              }
              else if ( state->nLeftBrType == nBrType &&
                        state->nLeftDupDirection == DP_FORWARD ) // getDirectionRank(DP_FORWARD, DP_DETECT)
              {
                if ( OccurrencesData_IsEmpty(&state->occData) )
                {
                  state->nRightBrType = nBrType;
                  state->nRightBrPos = nBrPos;
                  state->nRightDupDirection = nDupPairDirection;
                  break; // OK, found
                }
                else
                {
                  // could be either a right quote inside (another) quotes
                  // or a right quote outside (another) quotes, not sure...
                  break;
                }
              }
              else
              {
                // treating it as a left quote
                cookie.nPos = nBrPos;
                cookie.nDirection = nDupPairDirection;
                if ( !OccurrencesData_Increment(&state->occData, nBrType, &cookie) )
                  break; // no more room
              }
            }
            else
            {
              // left quote found while looking for a right one
              cookie.nPos = nBrPos;
              cookie.nDirection = nDupPairDirection;
              if ( !OccurrencesData_Increment(&state->occData, nBrType, &cookie) )
                break; // no more room
            }
          }
          else
          {
            if ( !OccurrencesData_Contains(&state->occData, nBrType, NULL) )
            {
              if ( OccurrencesData_IsEmpty(&state->occData) )
              {
                state->nRightBrType = nBrType;
                state->nRightBrPos = nBrPos;
                state->nRightDupDirection = DP_NONE;
                break; // OK, found
              }
              else
              {
                // could be either a right bracket inside quotes
                // or a right bracket outside quotes, not sure...
                break;
              }
            }
            else
              OccurrencesData_Decrement(&state->occData, nBrType);
          }
        }
      }
      else
      {
        nBrType = getLeftBracketTypeEx(wch, BTF_HIGHLIGHT | BTF_CHECK_TAGINV);
        if ( nBrType != tbtNone )
        {
          if ( !isEscapedPosEx(nBrPos) )
          {
            // found a left bracket instead of a right bracket
            if ( !OccurrencesData_Increment(&state->occData, nBrType, NULL) )
              break; // no more room
          }
        }
      }
    }
  }

  return (state->nRightBrType != tbtNone);
}

static BOOL NearestBr_ApplyStateToRange(const int action, const unsigned int flags, const INT_X nStartPos, const tGetNearestBracketsState* state, CHARRANGE_X* out_cr)
{
  if ( state->nRightBrType != tbtNone && state->nRightBrType == state->nLeftBrType &&
       state->nLeftBrPos <= state->nRightBrPos )
  {
    INT_X nLeftBrPos;
    INT_X nRightBrPos;

    // OK, pair found
    NearestBracketsIndexes[0] = state->nLeftBrPos - 1; // position of the left bracket: |(
    NearestBracketsIndexes[1] = state->nRightBrPos;   // position of the right bracket: |)
    NearestBracketsIndexes[2] = state->nRightBrType;

    nLeftBrPos = state->nLeftBrPos;
    nRightBrPos = state->nRightBrPos;

    if ( action == XBRA_SELTO_NEARBR )
    {
      BOOL bSelToOuterPos;

      if (g_dwOptions[OPT_DWORD_NEARESTBR_SELTO_FLAGS] & XBR_NBR_SELTO_OUTERPOS)
        bSelToOuterPos = TRUE;
      else
        bSelToOuterPos = FALSE;

      if ( (state->nRightBrType != tbtTagInv) &&
           (bSelToOuterPos || state->nCharacterPosition != nStartPos) )
      {
        if (flags & gnbrfWiden)
        {
          if ( (state->crCurrent.cpMin == state->crCurrent.cpMax)  //  [...|...]
            || (nLeftBrPos <= state->crCurrent.cpMin && nRightBrPos >= state->crCurrent.cpMax)  //  [|...|]  or  [...| |...]
             )
          {
            --nLeftBrPos;
            ++nRightBrPos;
          }
          else if (nLeftBrPos == state->crCurrent.cpMax + 1 || nRightBrPos + 1 == state->crCurrent.cpMin)
          {
            if (bSelToOuterPos)
            {
              --nLeftBrPos;
              ++nRightBrPos;
            }
            /*
            else if (nLeftBrPos != nRightBrPos)
            {
              // incorrect (next) pair found
              NearestBracketsIndexes[0] = -1;
              NearestBracketsIndexes[1] = -1;
              NearestBracketsIndexes[2] = tbtNone;

              return FALSE;
            }
            */
          }
        }
        else
        {
          --nLeftBrPos;
          ++nRightBrPos;
        }
      }

      out_cr->cpMin = nLeftBrPos;
      out_cr->cpMax = nRightBrPos;

      if (flags & gnbrfWiden)
      {
        if ( (nLeftBrPos == nRightBrPos && (bSelToOuterPos || state->crCurrent.cpMin == state->crCurrent.cpMax))  //  "|"  ->  |""|
          || (nLeftBrPos == state->crCurrent.cpMin && nRightBrPos == state->crCurrent.cpMax)   //  [|...|]
          || (nLeftBrPos == state->crCurrent.cpMin + 1 && nRightBrPos >= state->crCurrent.cpMax)  //  |[|...]  or  |[|]
          || (nLeftBrPos <= state->crCurrent.cpMin && nRightBrPos + 1 == state->crCurrent.cpMax)  //  [...|]|  or  [|]|
           )
        {
          --out_cr->cpMin;
          ++out_cr->cpMax;
        }
      }
    }
    else
    {
      if ( (state->nRightBrType != tbtTagInv) &&
           ((g_dwOptions[OPT_DWORD_NEARESTBR_GOTO_FLAGS] & XBR_NBR_GOTO_OUTERPOS) ||
            (state->nCharacterPosition != nStartPos)) )
      {
        --nLeftBrPos;
        ++nRightBrPos;
      }

      if ( (state->nCharacterPosition == nLeftBrPos) ||
           (state->nCharacterPosition == nStartPos && nStartPos == state->nLeftBrPos) )
      {
        out_cr->cpMin = nRightBrPos;
        out_cr->cpMax = nRightBrPos;
      }
      else if ( (state->nCharacterPosition == nRightBrPos) ||
                (state->nCharacterPosition == nStartPos && nStartPos == state->nRightBrPos) )
      {
        out_cr->cpMin = nLeftBrPos;
        out_cr->cpMax = nLeftBrPos;
      }
      else if ( ((g_dwOptions[OPT_DWORD_NEARESTBR_GOTO_FLAGS] & XBR_NBR_GOTO_ALWAYSLEFT) != 0) ||
                ( ((g_dwOptions[OPT_DWORD_NEARESTBR_GOTO_FLAGS] & XBR_NBR_GOTO_ALWAYSRIGHT) == 0) &&
                  (state->nCharacterPosition - nLeftBrPos <= nRightBrPos - state->nCharacterPosition) )
              )
      {
        out_cr->cpMin = nLeftBrPos;
        out_cr->cpMax = nLeftBrPos;
      }
      else
      {
        out_cr->cpMin = nRightBrPos;
        out_cr->cpMax = nRightBrPos;
      }
    }

    return TRUE;
  }

  // no pair found
  NearestBracketsIndexes[0] = -1;
  NearestBracketsIndexes[1] = -1;
  NearestBracketsIndexes[2] = tbtNone;
  
  return FALSE;
}

static BOOL GetNearestBracketsRange(const int action, const unsigned int flags, const INT_X nCharacterPosition, const CHARRANGE_X* curr_cr, CHARRANGE_X* out_cr)
{
  INT_X nStartPos;
  unsigned int nAtBr;
  tGetNearestBracketsState state;
  tGetHighlightIndexesCookie brCookie;

  if ( NearestBr_MatchCachedIndexes(action, nCharacterPosition, out_cr) )
  {
    if ( flags & gnbrfWiden )
    {
      if ( out_cr->cpMin == NearestBracketsIndexes[0] + 1 && out_cr->cpMax == NearestBracketsIndexes[1] ) // raw indexes
      {
        if ( curr_cr->cpMin == out_cr->cpMin && curr_cr->cpMax == out_cr->cpMax )  //  [|...|]
        {
          --out_cr->cpMin;
          ++out_cr->cpMax;
        }
      }
    }
    return TRUE;
  }

  // Note:
  // Now we can not use the cached values of NearestBracketsIndexes
  // because there can be several embedded bracket pairs within the
  // same range, e.g.:
  //   {  (  [  ]  ) | }
  // In such situation, the actual nearest brackets pair depends on
  // the caret position, e.g.:
  //   {  (  [  ] | )  }
  //   {  (  [ | ]  )  }
  prevNearestBracketsIndexes[0] = NearestBracketsIndexes[0];
  prevNearestBracketsIndexes[1] = NearestBracketsIndexes[1];
  prevNearestBracketsIndexes[2] = NearestBracketsIndexes[2];
  NearestBracketsIndexes[0] = -1;
  NearestBracketsIndexes[1] = -1;
  NearestBracketsIndexes[2] = tbtNone;

  state.nCharacterPosition = nCharacterPosition;
  state.nTextLength = (INT_X) SendMessage(g_hMainWnd, AKD_GETTEXTLENGTH, (WPARAM) hActualEditWnd, 0);
  state.nLeftBrPos = -1;
  state.nRightBrPos = -1;
  state.nLeftBrType = tbtNone;
  state.nRightBrType = tbtNone;
  state.nLeftDupDirection = DP_NONE;
  state.nRightDupDirection = DP_NONE;
  state.crCurrent.cpMin = curr_cr->cpMin;
  state.crCurrent.cpMax = curr_cr->cpMax;
  OccurrencesData_Init(&state.occData, state.occItems, XBR_NEARESTBR_OCC_ITEMS);

  nStartPos = nCharacterPosition;
  {
    int  nBrType;
    int  nDupDir;

    nAtBr = NearestBr_IsAtBracketCharacter(nCharacterPosition, state.nTextLength, &nBrType, &nDupDir);
    if ( nAtBr & abcLeftBr )
    {
      // at left bracket...
      if ( nAtBr & abcBrIsOnRight )
        ++nStartPos; //  |(  ->  (|
      state.nLeftBrPos = nStartPos; // exclude the bracket itself
      state.nLeftBrType = nBrType;
      state.nLeftDupDirection = nDupDir;
      NearestBr_GetFoldOrQuoteFromAkelEdit(nStartPos, nAtBr, &brCookie);
      NearestBr_FindRightBracket(nStartPos, flags, &state, &brCookie);
    }
    else if ( nAtBr & abcRightBr )
    {
      // at right bracket...
      if ( nAtBr & abcBrIsOnLeft )
        --nStartPos; //  )|  ->  |)
      state.nRightBrPos = nStartPos;
      state.nRightBrType = nBrType;
      state.nRightDupDirection = nDupDir;
      NearestBr_GetFoldOrQuoteFromAkelEdit(nStartPos, nAtBr, &brCookie);
      NearestBr_FindLeftBracket(nStartPos, flags, &state, &brCookie);
      NearestBr_AdjustLeftBracketPos(&state);
    }
    else if ( nAtBr & abcDetectBr )
    {
      unsigned int nAtBrNow;

      // detect: either at left or at right bracket
      // 1. try as a left bracket...
      nAtBrNow = nAtBr;
      if ( nAtBr & abcBrIsOnRight )
      {
        ++nStartPos; //  |(  ->  (|
        nAtBrNow = abcDetectBr | abcBrIsOnLeft;
      }
      state.nLeftBrPos = nStartPos; // exclude the bracket itself
      state.nLeftBrType = nBrType;
      state.nLeftDupDirection = nDupDir;
      NearestBr_GetFoldOrQuoteFromAkelEdit(nStartPos, nAtBrNow, &brCookie);
      NearestBr_FindRightBracket(nStartPos, flags, &state, &brCookie);
      if ( brCookie.nResult == ghlrNone && state.nRightBrType != state.nLeftBrType )
      {
        state.nLeftBrPos = -1;
        state.nLeftBrType = tbtNone;
        state.nLeftDupDirection = DP_NONE;

        // 2. try as a right bracket...
        nAtBrNow = nAtBr;
        if ( nAtBr & abcBrIsOnLeft )
        {
          --nStartPos; //  )|  ->  |)
          nAtBrNow = abcDetectBr | abcBrIsOnRight;
        }
        state.nRightBrPos = nStartPos;
        state.nRightBrType = nBrType;
        state.nRightDupDirection = nDupDir;
        NearestBr_GetFoldOrQuoteFromAkelEdit(nStartPos, nAtBrNow, &brCookie);
        NearestBr_FindLeftBracket(nStartPos, flags, &state, &brCookie);
        NearestBr_AdjustLeftBracketPos(&state);
      }
    }
    else
    {
      // not at a bracket
      NearestBr_GetFoldOrQuoteFromAkelEdit(nStartPos, nAtBr, &brCookie);
      NearestBr_FindLeftBracket(nStartPos, flags, &state, &brCookie);
      NearestBr_AdjustLeftBracketPos(&state);
      if ( state.nLeftBrType != tbtNone )
      {
        NearestBr_FindRightBracket(nStartPos, flags, &state, &brCookie);
      }
    }
  }

  if ( brCookie.nResult == ghlrPair )
  {
    if ( state.nRightBrType == tbtNone || state.nRightBrType != state.nLeftBrType ||
         ((flags & gnbrfWiden) == 0 && brCookie.pos2 - brCookie.pos1 < state.nRightBrPos - state.nLeftBrPos) ||
         ( (flags & gnbrfWiden) && 
           ((brCookie.pos2 - brCookie.pos1 > state.nRightBrPos - state.nLeftBrPos &&
             state.crCurrent.cpMax - state.crCurrent.cpMin >= state.nRightBrPos - state.nLeftBrPos) ||
            (brCookie.pos2 - brCookie.pos1 < state.nRightBrPos - state.nLeftBrPos &&
             state.crCurrent.cpMax - state.crCurrent.cpMin < brCookie.pos2 - brCookie.pos1))
         )
       )
    {
      state.nLeftBrPos = brCookie.pos1;
      state.nRightBrPos = brCookie.pos2;
      state.nLeftBrType = brCookie.nBracketType;    
      state.nRightBrType = brCookie.nBracketType;
      state.nLeftDupDirection = DP_NONE;
      state.nRightDupDirection = DP_NONE;

      if ( brCookie.pos1 == nStartPos + 1 )
        ++nStartPos; //  |(  ->  (|
      else if ( brCookie.pos2 + 1 == nStartPos )
        --nStartPos; //  )|  ->  |)
    }
  }

  return NearestBr_ApplyStateToRange(action, flags, nStartPos, &state, out_cr);
}

void OnEditGetNearestBracketsFunc(int action, HWND hEditWnd, INT_X nCharacterPosition)
{
  CHARRANGE_X curr_cr;
  CHARRANGE_X out_cr;

  updateActualState(hEditWnd);

  curr_cr.cpMin = nCharacterPosition;
  curr_cr.cpMax = nCharacterPosition;
  if ( GetNearestBracketsRange(action, 0, nCharacterPosition, &curr_cr, &out_cr) )
  {
    SendMessage(hEditWnd, EM_EXSETSEL_X, 0, (LPARAM) &out_cr);
  }
}

/*static*/ BOOL GetHighlightIndexes(const unsigned int uFlags, const int nHighlightIndex,
                                    const INT_X nCharacterPosition, const CHARRANGE_X* pSelection)
{
  int   nBracketType;
  int   nDuplicatedPairDirection; // left bracket is the same as right
  int   nDuplicatedPairDirectionPotential;
  int   nGetHighlightResult;
  BOOL  bRightBracket;
  WCHAR wch;

  if ( (nCharacterPosition < 0) || ((g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_XMODE] & XBR_HXM_ALL) == 0) )
    return FALSE;

  /*
  // this does not allow to work with new created file
  if ( nCurrentFileType == tftNone )
    return FALSE;
  */

  if ( (prevIndexesToHighlight[0] >= 0) || (prevIndexesToHighlight[3] >= 0) )
  {
    int i;

    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      if (nCharacterPosition == prevIndexesToHighlight[i])
      {
        INT_X ind1, ind2, ind3;
        int k = (i < 3) ? 0 : 3;
        IndexesToHighlight[nHighlightIndex + 0] = prevIndexesToHighlight[k + 0];
        IndexesToHighlight[nHighlightIndex + 1] = prevIndexesToHighlight[k + 1];
        IndexesToHighlight[nHighlightIndex + 2] = prevIndexesToHighlight[k + 2];
        ind1 = IndexesToHighlight[nHighlightIndex + 0];
        if (IndexesToHighlight[nHighlightIndex + 2] < 0)
        {
          ind2 = IndexesToHighlight[nHighlightIndex + 1];
          ind3 = IndexesToHighlight[nHighlightIndex + 2];
        }
        else
        {
          ind2 = IndexesToHighlight[nHighlightIndex + 2];
          ind3 = IndexesToHighlight[nHighlightIndex + 1];
        }

        updateCurrentBracketsIndexes(ind1, ind2);

        if (g_bAkelEdit)
        {
          tCharacterHighlightData* pchd;
          tGetHighlightIndexesCookie cookieHL;

          pchd = CharacterInfo_GetHighlightData(prevCharacterInfo, ind1);
          if (pchd)
          {
            if ((!(uFlags & XBR_GBF_UPDATEHLDATA)) &&
                ((pchd->dwState & XBR_STATEFLAG_DONOTHL) ||
                 ((pchd->dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(ind1, pSelection))))
            {
              CharacterInfo_Add(hgltCharacterInfo, ind1, pchd);
            }
            else
            {
              cookieHL.nBracketType = -1;
              GetHighlightDataFromAkelEdit(ind1, &cookieHL, GetAkelEditHighlightCallback);
              cookieHL.chd.dwState |= GetInSelectionState(ind1, pSelection);
              CharacterInfo_Add(hgltCharacterInfo, ind1, &cookieHL.chd);
              CharacterHighlightData_Copy(pchd, &cookieHL.chd);
            }
          }
          pchd = CharacterInfo_GetHighlightData(prevCharacterInfo, ind2);
          if (pchd)
          {
            if ((!(uFlags & XBR_GBF_UPDATEHLDATA)) &&
                ((pchd->dwState & XBR_STATEFLAG_DONOTHL) ||
                 ((pchd->dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(ind2, pSelection))))
            {
              CharacterInfo_Add(hgltCharacterInfo, ind2, pchd);
            }
            else
            {
              cookieHL.nBracketType = -1;
              GetHighlightDataFromAkelEdit(ind2, &cookieHL, GetAkelEditHighlightCallback);
              cookieHL.chd.dwState |= GetInSelectionState(ind2, pSelection);
              CharacterInfo_Add(hgltCharacterInfo, ind2, &cookieHL.chd);
              CharacterHighlightData_Copy(pchd, &cookieHL.chd);
            }
          }
          pchd = CharacterInfo_GetHighlightData(prevCharacterInfo, ind3);
          if (pchd)
          {
            if ((!(uFlags & XBR_GBF_UPDATEHLDATA)) &&
                ((pchd->dwState & XBR_STATEFLAG_DONOTHL) ||
                 ((pchd->dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(ind3, pSelection))))
            {
              CharacterInfo_Add(hgltCharacterInfo, ind3, pchd);
            }
            else
            {
              cookieHL.nBracketType = -1;
              GetHighlightDataFromAkelEdit(ind3, &cookieHL, GetAkelEditHighlightCallback);
              cookieHL.chd.dwState |= GetInSelectionState(ind3, pSelection);
              CharacterInfo_Add(hgltCharacterInfo, ind3, &cookieHL.chd);
              CharacterHighlightData_Copy(pchd, &cookieHL.chd);
            }
          }
        }

        return TRUE;
      }
    }
  }

  wch = getCharAt(hActualEditWnd, nCharacterPosition);

  nDuplicatedPairDirection = DP_NONE;
  nDuplicatedPairDirectionPotential = DP_NONE;
  nGetHighlightResult = ghlrNone;
  bRightBracket = FALSE;
  nBracketType = getLeftBracketType(wch, BTF_HIGHLIGHT);
  if (nBracketType == tbtNone)
  {
    bRightBracket = TRUE;
    nBracketType = getRightBracketType(wch, BTF_HIGHLIGHT);
    if (nBracketType != tbtNone)
    {
      if (nBracketType == tbtTag2)
      {
        if (wch == L'>')
          nBracketType = tbtTag;
        else
          nBracketType = tbtNone;
      }
    }
  }

  if (nBracketType != tbtNone)
  {
    if (g_bAkelEdit)
    {
      // check AkelEdit's highlight info
      nGetHighlightResult = GetAkelEditHighlightInfo(nHighlightIndex, nCharacterPosition, nBracketType, bRightBracket, pSelection);
      switch (nGetHighlightResult)
      {
        case ghlrDoNotHighlight:
          return TRUE; // processed by AkelEdit, do not highlight by XBrackets
        case ghlrPair:
          return TRUE; // pair found: nothing to process additionally
      }
    }

    if ((g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_XMODE] & XBR_HXM_XBRINTERNAL) == 0)
      return FALSE; // nothing to do

    // check for duplicated pair (e.g. "" quotes)
    if (isDuplicatedPair(nBracketType))
    {
      if (g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_MAX_LINES] == 0)
        return FALSE;

      nDuplicatedPairDirection = getDuplicatedPairDirection(nCharacterPosition, wch);
      switch (nDuplicatedPairDirection)
      {
        case DP_FORWARD:
          bRightBracket = FALSE; // search from left to right (forward)
          break;
        case DP_BACKWARD:
        case DP_DETECT:
        case DP_MAYBEBACKWARD:
        case DP_MAYBEFORWARD:
          bRightBracket = TRUE; // search from right to left (backward)
          break;
        /*default:
          nBracketType = tbtNone;
          break;*/
      }

      if (g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES] == 0)
      {
        if (isDirectionDetecting(nDuplicatedPairDirection))
          return FALSE;
      }
    }
  }

  if (nBracketType == tbtTag)
  {
    if (bBracketsDoTagIf && !(nCurrentFileType2 & tfmHtmlCompatible))
      return FALSE;
  }

  if ((nBracketType != tbtNone) &&
      bBracketsSkipEscaped1 && (nCurrentFileType2 & tfmEscaped1) &&
      (nCharacterPosition > 0))
  {
    wchar_t szPrefixW[MAX_ESCAPED_PREFIX + 2];
    INT_X pos;
    int len;

    getEscapedPrefixPos(nCharacterPosition, &pos, &len);

    if (g_bOldWindows)
    {
      char szPrefixA[MAX_ESCAPED_PREFIX + 2];

      len = (int) AnyRichEdit_GetTextAt(hActualEditWnd, pos, len, szPrefixA);
      MultiByteToWideChar(CP_ACP, 0, szPrefixA, -1, szPrefixW, MAX_ESCAPED_PREFIX + 1);
    }
    else
    {
      len = (int) AnyRichEdit_GetTextAtW(hActualEditWnd, pos, len, szPrefixW);
    }

    if (isEscapedPrefixW(szPrefixW, len))
      return FALSE;
  }

  if (nBracketType != tbtNone)
  {
    // we don't want these arrays in the stack ;)
    static TCHAR szLine[0x10000];
    static WCHAR wszLine[0x10000];

    INT_X i;
    INT_X i_saved;
    INT   nLen, nLine, nStartLine, nWrappedLines;
    INT   nLine_saved, rank_saved;
    INT   nFailReferences, nSearchDirection;
    INT   nMinLine[2];
    INT   nMaxLine[2];
    BOOL  bFound;
    BOOL  bComment;
    WCHAR wchOK, wchFail;

    const WCHAR* pcwszLine;
    AECHARINDEX ci;

    ci.lpLine = NULL;
    if (!g_bAkelEdit)
      pcwszLine = wszLine;

    // the character to search for (another bracket)
    wchOK = getBracketsPairW(nBracketType)[bRightBracket ? 0 : 1];
    if (nDuplicatedPairDirection == DP_NONE)
      wchFail = getBracketsPairW(nBracketType)[bRightBracket ? 1 : 0];
    else
      wchFail = 0;
    nFailReferences = 0;

    if (g_bOldWindows)
    {
      nLine = AnyRichEdit_ExLineFromChar(hActualEditWnd, nCharacterPosition);
      nMaxLine[0] = (bBracketsHighlightVisibleArea ?
        AnyRichEdit_LastVisibleLine(hActualEditWnd) : (AnyRichEdit_GetLineCount(hActualEditWnd) - 1));
      nMinLine[0] = (bBracketsHighlightVisibleArea ?
        AnyRichEdit_FirstVisibleLine(hActualEditWnd) : 0);
    }
    else
    {
      nLine = AnyRichEdit_ExLineFromCharW(hActualEditWnd, nCharacterPosition);
      nMaxLine[0] = (bBracketsHighlightVisibleArea ?
        AnyRichEdit_LastVisibleLineW(hActualEditWnd) : (AnyRichEdit_GetLineCountW(hActualEditWnd) - 1));
      nMinLine[0] = (bBracketsHighlightVisibleArea ?
        AnyRichEdit_FirstVisibleLineW(hActualEditWnd) : 0);
    }

    if (nDuplicatedPairDirection != DP_NONE)
    {
      int maxLines = isDirectionDetecting(nDuplicatedPairDirection) ?
        g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES] :
          g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_MAX_LINES];

      if (nMaxLine[0] > nLine + maxLines - 1)
        nMaxLine[0] = nLine + maxLines - 1;

      if (nMinLine[0] < nLine - maxLines + 1)
        nMinLine[0] = nLine - maxLines + 1;

      if (bRightBracket)
      {
        maxLines = g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES];

        nMaxLine[1] = nMaxLine[0];
        if (nMaxLine[1] > nLine + maxLines - 1)
          nMaxLine[1] = nLine + maxLines - 1;

        nMinLine[1] = nMinLine[0];
        if (nMinLine[1] < nLine - maxLines + 1)
          nMinLine[1] = nLine - maxLines + 1;
      }
    }
    else
    {
      const int maxBrLines = g_dwOptions[OPT_DWORD_HIGHLIGHT_BR_MAX_LINES];
      if (maxBrLines > 0)
      {
        if (nMaxLine[0] > nLine + maxBrLines - 1)
          nMaxLine[0] = nLine + maxBrLines - 1;

        if (nMinLine[0] < nLine - maxBrLines + 1)
          nMinLine[0] = nLine - maxBrLines + 1;
      }
    }

    i_saved = -1;
    nLine_saved = -1;
    rank_saved = 0;
    nStartLine = nLine;
    nWrappedLines = 0;
    nSearchDirection = 0;
    bFound = FALSE;
    bComment = FALSE;

    if ( (NearestBracketsIndexes[2] != tbtTagInv) &&
         (nCharacterPosition == NearestBracketsIndexes[0] ||
          nCharacterPosition == NearestBracketsIndexes[1]) )
    {
      if (nCharacterPosition == NearestBracketsIndexes[0])
        i = NearestBracketsIndexes[1];
      else
        i = NearestBracketsIndexes[0];
      if (g_bAkelEdit)
      {
        SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) i, (LPARAM) &ci);
        pcwszLine = ci.lpLine->wpLine;
        nLen = ci.lpLine->nLineLen;
        nLine = ci.nLine;
        i = ci.nCharInLine; // pos in line
      }
      else
      {
        pcwszLine = wszLine;
        if (g_bOldWindows)
        {
          nLine = AnyRichEdit_ExLineFromChar(hActualEditWnd, i);
          i -= AnyRichEdit_LineIndex(hActualEditWnd, nLine); // pos in line
          nLen = AnyRichEdit_GetLine(hActualEditWnd, nLine, szLine, 0x10000-1);
          // CAnyRichEdit::GetLine sets szLine[nLen] = 0
          MultiByteToWideChar(CP_ACP, 0, szLine, nLen, wszLine, nLen);
          wszLine[nLen] = 0;
        }
        else
        {
          nLine = AnyRichEdit_ExLineFromCharW(hActualEditWnd, i);
          i -= AnyRichEdit_LineIndexW(hActualEditWnd, nLine); // pos in line
          nLen = AnyRichEdit_GetLineW(hActualEditWnd, nLine, wszLine, 0x10000-1);
          // AnyRichEdit_GetLineW sets szLineW[nLen] = 0
        }
      }
      wch = wchOK;
      bFound = TRUE;
    }
    else
    {
      while ( bRightBracket ? (nLine >= nMinLine[nSearchDirection]) : (nLine <= nMaxLine[nSearchDirection]) )
      {
        if (g_bAkelEdit)
        {
          if (ci.lpLine)
          {
            ci.lpLine = bRightBracket ? ci.lpLine->prev : ci.lpLine->next;
          }
          else
          {
            SendMessage( hActualEditWnd, AEM_GETINDEX, AEGI_CARETCHAR, (LPARAM)&ci );
            if (ci.nLine != nLine)
            {
              if (ci.lpLine && (ci.nLine + 1 == nLine))
              {
                if ((ci.nCharInLine == ci.lpLine->nLineLen) &&
                    (ci.lpLine->nLineBreak == AELB_WRAP))
                {
                  // a hack: don't process a position at the line-wrap
                  break;
                }
              }

              SendMessage( hActualEditWnd, AEM_GETINDEX, AEGI_FIRSTSELCHAR, (LPARAM)&ci );
              if (ci.nLine != nLine)
                SendMessage( hActualEditWnd, AEM_GETINDEX, AEGI_LASTSELCHAR, (LPARAM)&ci );
            }
          }

          if (ci.lpLine)
          {
            pcwszLine = ci.lpLine->wpLine;
            nLen = ci.lpLine->nLineLen;
          }
          else
            break;
        }
        else
        {
          if (g_bOldWindows)
          {
            nLen = AnyRichEdit_GetLine(hActualEditWnd, nLine, szLine, 0x10000-1);
            // CAnyRichEdit::GetLine sets szLine[nLen] = 0
            MultiByteToWideChar(CP_ACP, 0, szLine, nLen, wszLine, nLen);
            wszLine[nLen] = 0;
          }
          else
          {
            nLen = AnyRichEdit_GetLineW(hActualEditWnd, nLine, wszLine, 0x10000-1);
            // AnyRichEdit_GetLineW sets szLineW[nLen] = 0
          }
        }

        if ( bBracketsSkipComment1 && (nCurrentFileType2 & tfmComment1) )
        {
          int i;

          // pre-processing current line
          for (i = 0; i < nLen; i++)
          {
            wch = pcwszLine[i];
            if (wch == L'/')
            {
              if ((i+1 < nLen) && (pcwszLine[i+1] == L'/'))
              {
                // this is "//" comment
                if ((i == 0) || (pcwszLine[i-1] != L':')) // not "://"
                {
                  //wszLine[i] = 0;
                  nLen = i;
                  bComment = TRUE;
                  break;
                }
              }
            }
          }

        }

        i = bRightBracket ? (nLen - 1) : 0;
        if (nStartLine == nLine)
        {
          INT_X nLinePosition;

          if (g_bOldWindows)
            nLinePosition = AnyRichEdit_LineIndex(hActualEditWnd, nLine);
          else
            nLinePosition = AnyRichEdit_LineIndexW(hActualEditWnd, nLine);

          nLinePosition = nCharacterPosition - nLinePosition;

          if (bComment) // start line contains a comment...
          {
            if (nLinePosition >= nLen) // ...and current bracket is commented
            {
              //MessageBoxA(NULL,"break","for ( ... nLine ... )",MB_OK);
              break; // break outside the cycle for nLine
            }
          }

          i = bRightBracket ? (nLinePosition - 1) : (nLinePosition + 1);

          if (nDuplicatedPairDirection != DP_NONE)
          {
            nDuplicatedPairDirectionPotential = DP_NONE;
            if (!containsSymbolToTheRight(g_bAkelEdit ? ci.lpLine : (AELINEDATA *)pcwszLine, wchOK, nLinePosition + 1, nLen))
              nDuplicatedPairDirectionPotential = DP_BACKWARD;

            if ((nDuplicatedPairDirectionPotential == DP_NONE) || !bRightBracket)
            {
              if (!containsSymbolToTheLeft(g_bAkelEdit ? ci.lpLine : (AELINEDATA *)pcwszLine, wchOK, nLinePosition - 1))
                nDuplicatedPairDirectionPotential = DP_FORWARD;
            }
          }
        }

        while ( bRightBracket ? (i >= 0) : (i < nLen) )
        {
          wch = pcwszLine[i];
          if (wch == wchOK)
          {
            if (!isEscapedCharacterW(i, pcwszLine))
              --nFailReferences;

            if (nFailReferences < 0)
            {
              bFound = TRUE;
              break;
            }
          }
          else if (wch == wchFail)
          {
            if (!isEscapedCharacterW(i, pcwszLine))
              ++nFailReferences;
          }

          if (bRightBracket)
            --i;
          else
            ++i;
        }

        if (bFound)
        {
          INT_X pos1;
          int   nDirection2;
          int   rank;

          if (nDuplicatedPairDirection == DP_NONE)
          {
            // not duplicated: the pair bracket is found definitely
            break;
          }

          if (g_bOldWindows)
            pos1 = i + AnyRichEdit_LineIndex(hActualEditWnd, nLine);
          else
            pos1 = i + AnyRichEdit_LineIndexW(hActualEditWnd, nLine);

          // Here is how it works:
          // 1. bFound=TRUE means:
          //      the pair bracket or quote has been found.
          // 2. invoking "break;" alone means:
          //      everything is OK, so accept the search result, stop.
          // 3. invoking "bFound = FALSE; break;" means:
          //      reject the search results, stop.
          // 4. when bRightBracket=TRUE, invoking "nSearchDirection = 1;" _without_ further "break;" means:
          //      the searching backward is done, let's initiate searching forward.
          if (bRightBracket)
          {
            nDirection2 = getDuplicatedPairDirection(pos1, wchOK);
            rank = getDirectionRank(nDirection2, nDuplicatedPairDirection);

            if (rank >= getDirectionRank(DP_MAYBEFORWARD, DP_BACKWARD))
              break; // definitely found: (?-> <--) or (--> <-?) or (--> <--)

            if (rank >= getDirectionRank(DP_DETECT, DP_BACKWARD))
            {
              // when search direction is known, let's assume
              // the result is OK within g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES]
              if (nStartLine - nLine < nWrappedLines + (int) g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES])
                break;

              i_saved = i;
              nLine_saved = nLine;
              rank_saved = rank;

              // let's try searching forward...
              nSearchDirection = 1;
            }
            else if (rank >= getDirectionRank(DP_BACKWARD, DP_BACKWARD))
            {
              if (nStartLine - nLine < nWrappedLines + (int) g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES])
              {
                if ((nDuplicatedPairDirectionPotential == DP_BACKWARD) ||
                    (!containsSymbolToTheLeft(g_bAkelEdit ? ci.lpLine : (AELINEDATA *)pcwszLine, wchOK, i - 1)))
                {
                  i_saved = i;
                  nLine_saved = nLine;
                  rank_saved = rank;
                }
              }

              // let's try searching forward...
              nSearchDirection = 1;
            }
            else
            {
              if (isDirectionDetecting(nDuplicatedPairDirection))
              {
                // let's try searching forward...
                nSearchDirection = 1;
              }
              else
              {
                // backward assumption failed, stop searching
                bFound = FALSE;
                break;
              }
            }
          }
          else
          {
            nDirection2 = getDuplicatedPairDirection(pos1, wchOK);
            rank = getDirectionRank(nDuplicatedPairDirection, nDirection2);

            if (rank >= getDirectionRank(DP_FORWARD, DP_MAYBEBACKWARD))
              break; // definitely found: (--> <-?) or (?-> <--) or (--> <--)

            if (rank >= getDirectionRank(DP_FORWARD, DP_DETECT))
            {
              // when search direction is known, let's assume
              // the result is OK within g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES]
              if (nLine - nStartLine < nWrappedLines + (int) g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES])
                break;

              if (i_saved != -1)
              {
                // conflict: the pair found in both directions
                if (rank_saved > rank)
                {
                  i = i_saved;
                  nLine = nLine_saved;
                }
                else if (rank_saved == rank)
                {
                  i_saved = -1;
                  bFound = FALSE;
                }
                // else accept current pair
              }
              break;
            }
            else if (rank >= getDirectionRank(DP_FORWARD, DP_FORWARD))
            {
              if (nLine - nStartLine < nWrappedLines + (int) g_dwOptions[OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES])
              {
                if ((nDuplicatedPairDirectionPotential == DP_FORWARD) ||
                    (!containsSymbolToTheRight(g_bAkelEdit ? ci.lpLine : (AELINEDATA *)pcwszLine, wchOK, i + 1, nLen)))
                {
                  if (i_saved != -1)
                  {
                    // conflict: the pair found in both directions
                    if (rank_saved > rank)
                    {
                      i = i_saved;
                      nLine = nLine_saved;
                    }
                    else if (rank_saved == rank)
                    {
                      i_saved = -1;
                      bFound = FALSE;
                    }
                    // else accept current pair
                  }
                  break;
                }
              }
            }

            bFound = FALSE; // searching forward failed
            break;
          }
        }

        if (bRightBracket)
        {
          if ((nSearchDirection == 1) ||
              (isDirectionDetecting(nDuplicatedPairDirection) && (nLine == nMinLine[0])))
          {
            if (g_bAkelEdit && (nSearchDirection == 0) &&
                ci.lpLine->prev && (ci.lpLine->prev->nLineBreak == AELB_WRAP))
            {
              --nLine; // go to previous line
              ++nWrappedLines;
              if (nMinLine[nSearchDirection] != 0)
                --nMinLine[nSearchDirection];
            }
            else
            {
              nSearchDirection = 1;
              bRightBracket = FALSE;
              bFound = FALSE;
              bComment = FALSE;
              nLine = nStartLine;
              nWrappedLines = 0;
              nFailReferences = 0;

              ci.lpLine = NULL;
              if (!g_bAkelEdit)
                pcwszLine = wszLine;
            }
          }
          else
          {
            --nLine; // go to previous line
            if (g_bAkelEdit)
            {
              if (ci.lpLine->prev && (ci.lpLine->prev->nLineBreak == AELB_WRAP))
              {
                ++nWrappedLines;
                if (nMinLine[nSearchDirection] != 0)
                  --nMinLine[nSearchDirection];
              }
            }
          }
        }
        else
        {
          ++nLine; // go to next line
          if (g_bAkelEdit)
          {
            if (ci.lpLine->nLineBreak == AELB_WRAP)
            {
              ++nWrappedLines;
              ++nMaxLine[nSearchDirection];
            }
          }
        }
      }

      if (!bFound)
      {
        if ((nSearchDirection == 1) && (i_saved != -1))
        {
          i = i_saved;
          nLine = nLine_saved;
          wch = wchOK;
          bFound = TRUE;
        }
      }
    }

    if (bFound && (wch == wchOK))
    {
      if (nBracketType == tbtTag)
      {
        WCHAR prev_wch = 0;
        if (bRightBracket)
        {
          if (nCharacterPosition > 0)
          {
            prev_wch = getCharAt(hActualEditWnd, nCharacterPosition-1);
          }
        }
        else
        {
          if (i > 0)
          {
            prev_wch = pcwszLine[i-1];
          }
        }
        if (prev_wch == L'/')
        {
          nBracketType = tbtTag2;
        }
      }

      if (g_bOldWindows)
      {
        i += AnyRichEdit_LineIndex(hActualEditWnd, nLine);
      }
      else
      {
        i += AnyRichEdit_LineIndexW(hActualEditWnd, nLine);
      }

      if (nBracketType != tbtTag2)
      {
        if (nGetHighlightResult == ghlrSingleChar)
        {
          // we can get here _only_ as the result of GetAkelEditHighlightInfo call
          const tCharacterHighlightData* pchd;

          pchd = CharacterInfo_GetHighlightDataConst(hgltCharacterInfo, nCharacterPosition);
          if (pchd)
          {
            tGetHighlightIndexesCookie cookieHL;

            if ((pchd->dwState & XBR_STATEFLAG_DONOTHL) ||
                ((pchd->dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(i, pSelection)))
            {
              CharacterInfo_Add(hgltCharacterInfo, i, pchd);
            }
            else
            {
              cookieHL.nBracketType = -1;
              GetHighlightDataFromAkelEdit(i, &cookieHL, GetAkelEditHighlightCallback);
              cookieHL.chd.dwState |= GetInSelectionState(i, pSelection);
              CharacterInfo_Add(hgltCharacterInfo, i, &cookieHL.chd);
            }
          }
        }
        IndexesToHighlight[nHighlightIndex] = i;
        IndexesToHighlight[nHighlightIndex+1] = nCharacterPosition;
      }
      else
      {
        INT_X i1;

        i1 = bRightBracket ? (nCharacterPosition - 1) : (i - 1);

        if (nGetHighlightResult == ghlrSingleChar)
        {
          // we can get here _only_ as the result of GetAkelEditHighlightInfo call
          const tCharacterHighlightData* pchd;

          pchd = CharacterInfo_GetHighlightDataConst(hgltCharacterInfo, nCharacterPosition);
          if (pchd)
          {
            tGetHighlightIndexesCookie cookieHL;
            BOOL bReadyHL;

            bReadyHL = FALSE;
            if ((pchd->dwState & XBR_STATEFLAG_DONOTHL) ||
                ((pchd->dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(i, pSelection)))
            {
              CharacterInfo_Add(hgltCharacterInfo, i, pchd);
            }
            else
            {
              bReadyHL = TRUE;
              cookieHL.nBracketType = -1;
              GetHighlightDataFromAkelEdit(i, &cookieHL, GetAkelEditHighlightCallback);
              cookieHL.chd.dwState |= GetInSelectionState(i, pSelection);
              CharacterInfo_Add(hgltCharacterInfo, i, &cookieHL.chd);
            }

            if ((pchd->dwState & XBR_STATEFLAG_DONOTHL) ||
                ((pchd->dwState & XBR_STATEFLAG_INSELECTION) == GetInSelectionState(i1, pSelection)))
            {
              CharacterInfo_Add(hgltCharacterInfo, i1, pchd);
            }
            else
            {
              if (!bReadyHL)
              {
                bReadyHL = TRUE;
                cookieHL.nBracketType = -1;
                GetHighlightDataFromAkelEdit(i1, &cookieHL, GetAkelEditHighlightCallback);
                cookieHL.chd.dwState |= GetInSelectionState(i1, pSelection);
              }
              CharacterInfo_Add(hgltCharacterInfo, i1, &cookieHL.chd);
            }
          }
        }
        IndexesToHighlight[nHighlightIndex] = i;
        IndexesToHighlight[nHighlightIndex+1] = i1;
        IndexesToHighlight[nHighlightIndex+2] = nCharacterPosition;
      }

      updateCurrentBracketsIndexes(i, nCharacterPosition);

      return TRUE;
    }
    else
    {
      //MessageBoxA(NULL,"no indexes","GetHighlightIndexes",MB_OK);
    }

    //return TRUE;
  }

  return FALSE;
}

/*static*/ void GetPosFromChar(HWND hEd, const INT_X nCharacterPosition, POINTL* lpPos)
{
  if (g_bOldRichEdit)
  {
    // RichEdit 2.0
    LRESULT coord;

    if (g_bOldWindows)
      coord = SendMessageA(hEd, EM_POSFROMCHAR, (WPARAM) nCharacterPosition, 0);
    else
      coord = SendMessageW(hEd, EM_POSFROMCHAR, (WPARAM) nCharacterPosition, 0);
    lpPos->x = LOWORD(coord);
    lpPos->y = HIWORD(coord);
  }
  else
  {
    if (g_bOldWindows)
      SendMessageA(hEd, EM_POSFROMCHAR, (WPARAM) lpPos, (LPARAM) nCharacterPosition);
    else
      SendMessageW(hEd, EM_POSFROMCHAR, (WPARAM) lpPos, (LPARAM) nCharacterPosition);
  }
}

/*static*/ void CopyMemory1(void* dst, const void* src, unsigned int size)
{
  unsigned char* pdst;
  const unsigned char* psrc;

  pdst = (unsigned char *) dst;
  psrc = (const unsigned char *) src;

  while ( size-- )
  {
    *(pdst++) = *(psrc++);
  }
}

static void adjustFont(const DWORD dwFontStyle, const DWORD dwFontFlags,
                       BYTE* plfItalic, LONG* plfWeight)
{
  switch (dwFontStyle)
  {
    case AEHLS_FONTNORMAL:
      if (!(dwFontFlags & AEHLO_IGNOREFONTNORMAL))
      {
        *plfItalic = FALSE;
        *plfWeight = FW_NORMAL;
      }
      break;
    case AEHLS_FONTBOLD:
      //if (!(dwFontFlags & AEHLO_IGNOREFONTNORMAL))
      //{
        *plfItalic = FALSE;
      //}
      if (!(dwFontFlags & AEHLO_IGNOREFONTBOLD))
      {
        *plfWeight = FW_BOLD;
      }
      break;
    case AEHLS_FONTITALIC:
      if (!(dwFontFlags & AEHLO_IGNOREFONTITALIC))
      {
        *plfItalic = TRUE;
      }
      //if (!(dwFontFlags & AEHLO_IGNOREFONTNORMAL))
      //{
        *plfWeight = FW_NORMAL;
      //}
      break;
    case AEHLS_FONTBOLDITALIC:
      if (!(dwFontFlags & AEHLO_IGNOREFONTITALIC))
      {
        *plfItalic = TRUE;
      }
      if (!(dwFontFlags & AEHLO_IGNOREFONTBOLD))
      {
        *plfWeight = FW_BOLD;
      }
      break;
  }
}

BOOL IsClearTypeEnabled(void)
{
  BOOL bClearType;
  BOOL bFontSmoothing;
  UINT nFontSmoothingType;

  bClearType = FALSE;
  bFontSmoothing = FALSE;
  nFontSmoothingType = 0;

  if (g_bOldWindows)
  {
    SystemParametersInfoA(SPI_GETFONTSMOOTHING, 0, &bFontSmoothing, 0);
    if (bFontSmoothing)
    {
      SystemParametersInfoA(SPI_GETFONTSMOOTHINGTYPE, 0, &nFontSmoothingType, 0);
    }
  }
  else
  {
    SystemParametersInfoW(SPI_GETFONTSMOOTHING, 0, &bFontSmoothing, 0);
    if (bFontSmoothing)
    {
      SystemParametersInfoW(SPI_GETFONTSMOOTHINGTYPE, 0, &nFontSmoothingType, 0);
    }
  }

  if (bFontSmoothing && (nFontSmoothingType == FE_FONTSMOOTHINGCLEARTYPE))
  {
    bClearType = TRUE;
  }

  return bClearType;
}

#define mix_color(a, b) (BYTE)((((unsigned int)(a))*3)/8 + (((unsigned int)(b))*5)/8)

/*static*/ void HighlightCharacter(const INT_X nCharacterPosition, const unsigned int uHighlightFlags)
{
  POINTL ptBegin, ptEnd;
  tCharacterHighlightData chd;
  const tCharacterHighlightData* pchd;

  if (!hActualEditWnd)
    return;

  pchd = CharacterInfo_GetHighlightDataConst(hgltCharacterInfo, nCharacterPosition);
  if (pchd && (pchd->dwState & XBR_STATEFLAG_DONOTHL))
    return;

  if (pchd)
  {
    CharacterHighlightData_Copy(&chd, pchd);
  }
  else
  {
    CharacterHighlightData_Clear(&chd);
    pchd = &chd;
  }
  // contents of *pchd and chd are equal now

  if (g_bOldWindows)
  {
    if ((nCharacterPosition < AnyRichEdit_FirstVisibleCharIndex(hActualEditWnd)) ||
        (nCharacterPosition > AnyRichEdit_LastVisibleCharIndex(hActualEditWnd)))
    {
      return;
    }
  }
  else
  {
    if ((nCharacterPosition < AnyRichEdit_FirstVisibleCharIndexW(hActualEditWnd)) ||
        (nCharacterPosition > AnyRichEdit_LastVisibleCharIndexW(hActualEditWnd)))
    {
      return;
    }
  }

  if (g_bAkelEdit)
  {
    AEFOLD* lpFold;
    INT     nLine;

    nLine = AnyRichEdit_ExLineFromChar(hActualEditWnd, nCharacterPosition);
    lpFold = (AEFOLD *) SendMessage(hActualEditWnd, AEM_ISLINECOLLAPSED, nLine, 0);
    if (lpFold)
    {
      // the line with this character is collapsed
      return;
    }
  }

  GetPosFromChar(hActualEditWnd, nCharacterPosition, &ptBegin);
  // returns left-top point of the character's rectangle

  if ((ptBegin.x >= 0) && (ptBegin.y >= 0))
  {
    HDC          hDC;
    RECT         EdRect;
    AECHARINDEX  aeci;
    AECHARCOLORS aecc;
    BOOL         bInSelection;
    DWORD        dwFontFlags;

    AnyRichEdit_GetEditRect(hActualEditWnd, &EdRect);

    if (ptBegin.x < EdRect.left)
    {
      // left side of the character can be invisible
      // ptBegin.x = EdRect.left;
      return;
    }

    if (ptBegin.y < EdRect.top)
    {
      // top side of the character can be invisible
      // ptBegin.y = EdRect.top;
      return;
    }

    GetPosFromChar(hActualEditWnd, nCharacterPosition+1, &ptEnd);
    // left-top point of the next character's rectangle

    if ((ptEnd.y != ptBegin.y) || (ptEnd.x < 0))
    {
      ptEnd.x = EdRect.right;
    }

    //cc.nCharPos = nCharacterPosition;
    //bInSelection = (BOOL) SendMessage(g_hMainWnd, AKD_GETCHARCOLOR, (WPARAM) hActualEditWnd, (LPARAM) &cc);
    SendMessage(hActualEditWnd, AEM_RICHOFFSETTOINDEX, (WPARAM) nCharacterPosition, (LPARAM) &aeci);
    aecc.dwFlags = 0;
    aecc.dwFontStyle = 0;
    bInSelection = (BOOL) SendMessage(hActualEditWnd, AEM_GETCHARCOLORS, (WPARAM) &aeci, (LPARAM) &aecc);
    dwFontFlags = (DWORD) SendMessage(hActualEditWnd, AEM_HLGETOPTIONS, 0, 0);

    hDC = GetDC(hActualEditWnd);
    if (hDC)
    {
      HRGN         hRgn, hRgnOld;
      HFONT        hFont, hFontOld;
      RECT         rect;
      COLORREF     textColor, bkColor;
      LOGFONTA     lfA;
      LOGFONTW     lfW;
      //INT         sel1, sel2;
      int          nBkModePrev;
      char         strA[2];
      wchar_t      strW[2];

      bBracketsInternalRepaint = TRUE;

      // to repaint a field under the caret
      HideCaret(hActualEditWnd);

      // at first we select a font...
      if (g_bOldWindows)
      {
        HFONT hf;

        hf = (HFONT) SendMessageA(g_hMainWnd, AKD_GETFONTA, (WPARAM) hActualEditWnd, (LPARAM) &lfA);
        if (!hf)
          hf = (HFONT) SendMessageA(g_hMainWnd, AKD_GETFONTA, (WPARAM) NULL, (LPARAM) &lfA);

        // adjust font style
        adjustFont(chd.dwFontStyle, dwFontFlags, &lfA.lfItalic, &lfA.lfWeight);

        hFont = CreateFontIndirectA(&lfA);
      }
      else
      {
        HFONT hf;

        hf = (HFONT) SendMessageW(g_hMainWnd, AKD_GETFONTW, (WPARAM) hActualEditWnd, (LPARAM) &lfW);
        if (!hf)
          hf = (HFONT) SendMessageW(g_hMainWnd, AKD_GETFONTW, (WPARAM) NULL, (LPARAM) &lfW);

        // adjust font style
        adjustFont(chd.dwFontStyle, dwFontFlags, &lfW.lfItalic, &lfW.lfWeight);

        hFont = CreateFontIndirectW(&lfW);
      }
      hFontOld = (HFONT) SelectObject(hDC, hFont);

      // ...then we call GetTextMetrics for this font...
      if (g_bOldWindows)
      {
        TEXTMETRICA tmA;

        GetTextMetricsA(hDC, &tmA);
        ptEnd.y = ptBegin.y + tmA.tmHeight;
      }
      else
      {
        TEXTMETRICW tmW;

        GetTextMetricsW(hDC, &tmW);
        ptEnd.y = ptBegin.y + tmW.tmHeight;
      }

      ptEnd.y *= 5; // multiply height by (5/4)
      ptEnd.y /= 4; // for "tall" characters

      // ...finally we can select a region
      ptEnd.x += 3*(ptEnd.x - ptBegin.x); // for some cursive fonts
      if (ptEnd.x > EdRect.right)
      {
        ptEnd.x = EdRect.right;
      }
      if (ptEnd.y > EdRect.bottom)
      {
        ptEnd.y = EdRect.bottom;
      }

      hRgn = CreateRectRgn(ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
      hRgnOld = (HRGN) SelectObject(hDC, hRgn);

      nBkModePrev = SetBkMode(hDC, TRANSPARENT);
      //bkColor = GetBkColor(hDC);

      //if (g_bOldWindows)
      //  currentEdit.ExGetSelPos(&sel1, &sel2);
      //else
      //  currentEdit.ExGetSelPosW(&sel1, &sel2);

      if (!bInSelection)
      {
        if (chd.dwActiveTextColor != XBR_FONTCOLOR_UNDEFINED)
          aecc.crText = chd.dwActiveTextColor;
        if (chd.dwActiveBkColor != XBR_FONTCOLOR_UNDEFINED)
          aecc.crBk = chd.dwActiveBkColor;
      }
      bkColor = aecc.crBk;
      textColor = aecc.crText;
      if (uHighlightFlags & HF_DOHIGHLIGHT)
      {
        //textColor = RGB(16,112,192);
        if (uBracketsHighlight & BRHLF_TEXT)
          textColor = bracketsColourHighlight[0];
        if (uBracketsHighlight & BRHLF_BKGND)
          bkColor = bracketsColourHighlight[1];

        if (bInSelection)
        {
          if (uBracketsHighlight & BRHLF_BKGND)
          {
            BYTE r, g, b;

            r = mix_color( GetRValue(bkColor), GetRValue(aecc.crBk) );
            g = mix_color( GetGValue(bkColor), GetGValue(aecc.crBk) );
            b = mix_color( GetBValue(bkColor), GetBValue(aecc.crBk) );

            bkColor = RGB(r, g, b);
          }
        }
      }

      SetTextColor(hDC, aecc.crBk);
      rect.left   = ptBegin.x;
      rect.top    = ptBegin.y;
      rect.right  = ptEnd.x;
      rect.bottom = ptEnd.y;
      if (g_bOldWindows)
      {
        if (uHighlightFlags & HF_DOHIGHLIGHT)
        {
          COLORREF crOldBk = 0;

          if (uHighlightFlags & HF_CLEARTYPE)
          {
            SetBkMode(hDC, nBkModePrev);
            crOldBk = SetBkColor(hDC, aecc.crBk);
          }

          strA[0] = AnyRichEdit_GetCharAt(hActualEditWnd, nCharacterPosition);
          strA[1] = 0;
          DrawTextA(hDC, strA, 1, &rect, 0);

          if (hFontOld)  SelectObject(hDC, hFontOld);
          DeleteObject(hFont);

          if (uHighlightFlags & HF_CLEARTYPE)
          {
            SetBkColor(hDC, crOldBk);
            SetBkMode(hDC, TRANSPARENT);
          }

          if (chd.dwFontStyle == XBR_FONTSTYLE_UNDEFINED)
          {
            /*if (!lfA.lfItalic)
            {
              // hack: draw italic-style bracket character with bkgnd color
              lfA.lfItalic = TRUE;
              hFont = CreateFontIndirectA(&lfA);
              hFontOld = (HFONT) SelectObject(hDC, hFont);
              DrawTextA(hDC, strA, 1, &rect, 0);
              if (hFontOld)  SelectObject(hDC, hFontOld);
              DeleteObject(hFont);
              lfA.lfItalic = FALSE;
            }*/

            lfA.lfWeight = (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_STYLE] & XBR_HSF_BOLDFONT) ? FW_BOLD : FW_NORMAL;
          }
          else
          {
            if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_STYLE] & XBR_HSF_BOLDFONT)
            {
              lfA.lfWeight = FW_BOLD;
            }
          }

          hFont = CreateFontIndirectA(&lfA);
          hFontOld = (HFONT) SelectObject(hDC, hFont);
          if (uBracketsHighlight & BRHLF_BKGND)
          {
            SetBkMode(hDC, OPAQUE);
            SetBkColor(hDC, bkColor);
          }
          if (uBracketsHighlight & BRHLF_TEXT)
          {
            SetTextColor(hDC, textColor);
          }
          else
          {
            SetTextColor(hDC, aecc.crText);
          }

          DrawTextA(hDC, strA, 1, &rect, 0);
        }
        else
        {
          InvalidateRgn(hActualEditWnd, hRgn, TRUE);
        }
      }
      else
      {
        if (uHighlightFlags & HF_DOHIGHLIGHT)
        {
          COLORREF crOldBk = 0;

          if (uHighlightFlags & HF_CLEARTYPE)
          {
            SetBkMode(hDC, nBkModePrev);
            crOldBk = SetBkColor(hDC, aecc.crBk);
          }

          strW[0] = AnyRichEdit_GetCharAtW(hActualEditWnd, nCharacterPosition);
          strW[1] = 0;
          DrawTextW(hDC, strW, 1, &rect, 0);

          if (hFontOld)  SelectObject(hDC, hFontOld);
          DeleteObject(hFont);

          if (uHighlightFlags & HF_CLEARTYPE)
          {
            SetBkColor(hDC, crOldBk);
            SetBkMode(hDC, TRANSPARENT);
          }

          if (chd.dwFontStyle == XBR_FONTSTYLE_UNDEFINED)
          {
            /*if (!lfW.lfItalic)
            {
              // hack: draw italic-style bracket character with bkgnd color
              lfW.lfItalic = TRUE;
              hFont = CreateFontIndirectW(&lfW);
              hFontOld = (HFONT) SelectObject(hDC, hFont);
              DrawTextW(hDC, strW, 1, &rect, 0);
              if (hFontOld)  SelectObject(hDC, hFontOld);
              DeleteObject(hFont);
              lfW.lfItalic = FALSE;
            }*/

            lfW.lfWeight = (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_STYLE] & XBR_HSF_BOLDFONT) ? FW_BOLD : FW_NORMAL;
          }
          else
          {
            if (g_dwOptions[OPT_DWORD_HIGHLIGHT_HLT_STYLE] & XBR_HSF_BOLDFONT)
            {
              lfW.lfWeight = FW_BOLD;
            }
          }

          hFont = CreateFontIndirectW(&lfW);
          hFontOld = (HFONT) SelectObject(hDC, hFont);
          if (uBracketsHighlight & BRHLF_BKGND)
          {
            SetBkMode(hDC, OPAQUE);
            SetBkColor(hDC, bkColor);
          }
          if (uBracketsHighlight & BRHLF_TEXT)
          {
            SetTextColor(hDC, textColor);
          }
          else
          {
            SetTextColor(hDC, aecc.crText);
          }

          DrawTextW(hDC, strW, 1, &rect, 0);
        }
        else
        {
          InvalidateRgn(hActualEditWnd, hRgn, TRUE);
        }
      }
      if (hFontOld)  SelectObject(hDC, hFontOld);
      DeleteObject(hFont);
      if (hRgnOld)  SelectObject(hDC, hRgnOld);
      DeleteObject(hRgn);

      ShowCaret(hActualEditWnd);

      ReleaseDC(hActualEditWnd, hDC);

      bBracketsInternalRepaint = FALSE;
    }
  }

}

void RemoveAllHighlightInfo(const BOOL bRepaint)
{
  int i;

  if (bBracketsInternalRepaint || bAkelPadIsStarting)
    return;

  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    IndexesToHighlight[i] = -1; // set no items to highlight
    prevIndexesToHighlight[i] = -1;
  }
  CurrentBracketsIndexes[0] = -1;
  CurrentBracketsIndexes[1] = -1;

  NearestBracketsIndexes[0] = -1;
  NearestBracketsIndexes[1] = -1;
  NearestBracketsIndexes[2] = tbtNone;
  prevNearestBracketsIndexes[0] = -1;
  prevNearestBracketsIndexes[1] = -1;
  prevNearestBracketsIndexes[2] = tbtNone;

  if (g_bAkelEdit)
  {
    CharacterInfo_ClearAll(hgltCharacterInfo);
    CharacterInfo_ClearAll(prevCharacterInfo);
  }

  if (bRepaint)
  {
    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      if (IndexesHighlighted[i] >= 0)  // is any item highlighted?
        break;
    }
    if (i < HIGHLIGHT_INDEXES)
    {
      // removing all highlighting
      OnEditHighlightActiveBrackets();
    } // else there is no highlighted items
  }
  else
  {
    for (i = 0; i < HIGHLIGHT_INDEXES; i++)
    {
      IndexesHighlighted[i] = -1;
    }
  }
}

//---------------------------------------------------------------------------

/*static*/ WCHAR char2wchar(const char ch)
{
  char  str[2] = {0, 0};
  WCHAR wstr[2] = {0, 0};

  str[0] = ch;
  MultiByteToWideChar(CP_ACP, 0, str, 1, wstr, 1);
  return wstr[0];
}

/*static*/ void getEscapedPrefixPos(const INT_X nOffset, INT_X* pnPos, INT* pnLen)
{
  if (nOffset > MAX_ESCAPED_PREFIX)
  {
    *pnPos = nOffset - MAX_ESCAPED_PREFIX;
    *pnLen = MAX_ESCAPED_PREFIX;
  }
  else
  {
    *pnPos = 0;
    *pnLen = (INT) nOffset;
  }
}

/*static*/ BOOL isEscapedPrefixW(const wchar_t* strW, int len)
{
  int k = 0;
  while ((len > 0) && (strW[--len] == L'\\'))
  {
    ++k;
  }
  return (k % 2) ? TRUE : FALSE;
}

/*static*/ BOOL isEscapedPrefixA(const char* strA, int len)
{
  int k = 0;
  while ((len > 0) && (strA[--len] == '\\'))
  {
    ++k;
  }
  return (k % 2) ? TRUE : FALSE;
}

/*static*/ BOOL isEscapedPosW(const INT_X nOffset)
{
  INT_X pos;
  int len;
  wchar_t szPrefixW[MAX_ESCAPED_PREFIX + 2];

  getEscapedPrefixPos(nOffset, &pos, &len);
  len = (int) AnyRichEdit_GetTextAtW(hActualEditWnd, pos, len, szPrefixW);
  return isEscapedPrefixW(szPrefixW, len);
}

/*static*/ BOOL isEscapedPosA(const INT_X nOffset)
{
  INT_X pos;
  int len;
  char szPrefixA[MAX_ESCAPED_PREFIX + 2];

  getEscapedPrefixPos(nOffset, &pos, &len);
  len = (int) AnyRichEdit_GetTextAt(hActualEditWnd, pos, len, szPrefixA);
  return isEscapedPrefixA(szPrefixA, len);
}

/*static*/ BOOL isEscapedPosEx(const INT_X nOffset)
{
  if ( bBracketsSkipEscaped1 && (nCurrentFileType2 & tfmEscaped1) )
  {
    return (g_bOldWindows ? isEscapedPosA(nOffset) : isEscapedPosW(nOffset));
  }
  return FALSE;
}

/*static*/ BOOL isEscapedCharacterW(const INT_X pos, const wchar_t* pcwszLine)
{
  if (bBracketsSkipEscaped1 && (nCurrentFileType2 & tfmEscaped1) && (pos > 0))
  {
    INT_X prefixPos;
    INT   prefixLen;

    getEscapedPrefixPos(pos, &prefixPos, &prefixLen);
    if (isEscapedPrefixW(pcwszLine + prefixPos, prefixLen))
      return TRUE;
  }
  return FALSE;
}

/*static*/ void remove_duplicate_indexes_and_sort(INT_X* indexes, const INT size)
{
  INT   i, j;
  INT_X ind;

  // removing duplicates
  for (i = 0; i < size; i++)
  {
    ind = indexes[i];
    if (ind >= 0)
    {
      for (j = 0; j < size; j++)
      {
        if ((i != j) && (ind == indexes[j]))
        {
          indexes[j] = -1;
        }
      }
    }
  }

  // sorting
  for (i = 0; i < size; i++)
  {
    ind = indexes[i];
    for (j = i+1; j < size; j++)
    {
      if (indexes[j] > ind)
      {
        indexes[i] = indexes[j];
        indexes[j] = ind;  // old value
        ind = indexes[i];  // new value
      }
    }
  }
}

static const char* getFileExtA(const char* cszFileNameA)
{
  if (cszFileNameA)
  {
    int pos = lstrlenA(cszFileNameA) - 1;
    if (pos > 0)
    {
      while ((pos >= 0) && (cszFileNameA[pos] != '.'))
      {
        --pos;
      }
      return (cszFileNameA + 1 + pos);
    }
  }
  return NULL;
}

static const wchar_t* getFileExtW(const wchar_t* cszFileNameW)
{
  if (cszFileNameW)
  {
    int pos = lstrlenW(cszFileNameW) - 1;
    if (pos > 0)
    {
      while ((pos >= 0) && (cszFileNameW[pos] != L'.'))
      {
        --pos;
      }
      return (cszFileNameW + 1 + pos);
    }
  }
  return NULL;
}

// IMPORTANT!!!  wstr1 and wstr2 must be valid strings!
static int wstr_unsafe_cmp(const wchar_t* wstr1, const wchar_t* wstr2)
{
  while ( (*wstr1) && (*wstr1 == *wstr2) )
  {
    ++wstr1;
    ++wstr2;
  }
  return (*wstr1 - *wstr2);
}

// IMPORTANT!!!  wstr and wsubstr must be valid strings!
static int wstr_unsafe_subcmp(const wchar_t* wstr, const wchar_t* wsubstr)
{
  while ( (*wstr) && (*wstr == *wsubstr) )
  {
    ++wstr;
    ++wsubstr;
  }
  return (*wsubstr) ? (*wstr - *wsubstr) : 0;
}

static BOOL wstr_is_listed_ext(const wchar_t* szExtW,
  wchar_t* szExtListW, char* szExtListA)
{
  if ( szExtW && szExtW[0] )
  {
    int      len, i, n;
    wchar_t  szW[MAX_EXT];

    if (g_bOldWindows)
    {
      len = lstrlenA(szExtListA);
      MultiByteToWideChar(CP_ACP, 0, szExtListA, len,
        szExtListW, STR_FILEEXTS_SIZE - 1);
      szExtListW[len] = 0;
    }
    else
    {
      len = lstrlenW(szExtListW);
    }

    n = 0;
    i = 0;
    while (i <= len)
    {
      if ( (szExtListW[i]) &&
           (szExtListW[i] != L';') &&
           (szExtListW[i] != L',') &&
           (szExtListW[i] != L' ') )
      {
        szW[n++] = szExtListW[i];
      }
      else
      {
        if (n > 0)
        {
          szW[n] = 0;
          if (wstr_unsafe_cmp(szExtW, szW) == 0)
            return TRUE;
        }
        n = 0;
      }
      ++i;
    }
  }
  return FALSE;
}

static BOOL wstr_is_comment1_ext(const wchar_t* szExtW)
{
  return wstr_is_listed_ext(szExtW, strComment1FileExtsW, strComment1FileExtsA);
}

static BOOL wstr_is_escaped1_ext(const wchar_t* szExtW)
{
  /*
  if (g_bOldWindows)
  {
    if (strEscaped1FileExtsA[0] == 0)
      return TRUE;
  }
  else
  {
    if (strEscaped1FileExtsW[0] == 0)
      return TRUE;
  }
  */
  return wstr_is_listed_ext(szExtW, strEscaped1FileExtsW, strEscaped1FileExtsA);
}

static BOOL wstr_is_html_compatible(const wchar_t* szExtW)
{
  if ( szExtW && szExtW[0] )
  {
    int      len, i, n;
    wchar_t  szW[MAX_EXT];

    if (g_bOldWindows)
    {
      len = lstrlenA(strHtmlFileExtsA);
      MultiByteToWideChar(CP_ACP, 0, strHtmlFileExtsA, len,
        strHtmlFileExtsW, STR_FILEEXTS_SIZE - 1);
      strHtmlFileExtsW[len] = 0;
    }
    else
    {
      len = lstrlenW(strHtmlFileExtsW);
    }

    n = 0;
    i = 0;
    while (i <= len)
    {
      if ( (strHtmlFileExtsW[i]) &&
           (strHtmlFileExtsW[i] != L';') &&
           (strHtmlFileExtsW[i] != L',') &&
           (strHtmlFileExtsW[i] != L' ') )
      {
        szW[n++] = strHtmlFileExtsW[i];
      }
      else
      {
        if (n > 0)
        {
          szW[n] = 0;
          n = 0;
          while (szExtW[n])
          {
            if (wstr_unsafe_subcmp(szExtW + n, szW) == 0)
              return TRUE;
            else
              ++n;
          }
        }
        n = 0;
      }
      ++i;
    }
  }
  return FALSE;
}

int getFileType(int* pnCurrentFileType2)
{
  EDITINFO    ei;
  wchar_t     szExtW[MAX_EXT];
  const void* p = NULL;
  int         nType = tftNone;
  int         nType2 = tfmNone;

  if (SendMessage(g_hMainWnd, AKD_GETEDITINFO, (WPARAM) hActualEditWnd, (LPARAM) &ei) != 0)
  {
    if (g_bOldWindows)
    {
      p = getFileExtA( (const char*) ei.pFile ); // file extension ptr (char *)
      if (p)
      {
        char szExtA[MAX_EXT];
        int  len = 0;
        while ( (len < MAX_EXT - 1) &&
                ((szExtA[len] = ((const char*) p)[len]) != 0) )
        {
          ++len;
        }
        szExtA[len] = 0;
        CharLowerA(szExtA); // MustDie9x does not have CharLowerW !
        MultiByteToWideChar(CP_ACP, 0, szExtA, len, szExtW, MAX_EXT - 1);
        szExtW[len] = 0; // file extension (wchar_t *)
      }
    }
    else
    {
      p = getFileExtW( (const wchar_t*) ei.pFile ); // file extension ptr (wchar_t *)
      if (p)
      {
        int len = 0;
        while ( (len < MAX_EXT - 1) &&
                ((szExtW[len] = ((const wchar_t*) p)[len]) != 0) )
        {
          ++len;
        }
        szExtW[len] = 0; // file extension (wchar_t *)
        CharLowerW(szExtW);
      }
    }
  }

  if (p)
  {
    hCurrentEditWnd = hActualEditWnd;

    //MessageBoxW(NULL, szExtW, L"ext", MB_OK);

    if ( (wstr_unsafe_cmp(szExtW, L"c") == 0)   ||
         (wstr_unsafe_cmp(szExtW, L"cc") == 0) ||
         (wstr_unsafe_cmp(szExtW, L"cpp") == 0) ||
         (wstr_unsafe_cmp(szExtW, L"cxx") == 0) )
    {
      nType = tftC_Cpp;
      nType2 = tfmComment1 | tfmEscaped1;
    }
    else if ( (wstr_unsafe_cmp(szExtW, L"h") == 0) ||
              (wstr_unsafe_cmp(szExtW, L"hh") == 0) ||
              (wstr_unsafe_cmp(szExtW, L"hpp") == 0) ||
              (wstr_unsafe_cmp(szExtW, L"hxx") == 0) )
    {
      nType = tftH_Hpp;
      nType2 = tfmComment1 | tfmEscaped1;
    }
    else if ( wstr_unsafe_cmp(szExtW, L"pas") == 0 )
    {
      nType = tftPas;
      nType2 = tfmComment1;
    }
    else
    {
      nType = tftText;
      if ( wstr_is_comment1_ext(szExtW) )
      {
        nType2 |= tfmComment1;
      }
      if ( wstr_is_escaped1_ext(szExtW) )
      {
        nType2 |= tfmEscaped1;
      }
      if ( wstr_is_html_compatible(szExtW) )
      {
        nType2 |= tfmHtmlCompatible;
      }
    }

  }
  else
  {
    hCurrentEditWnd = NULL;
  }

  *pnCurrentFileType2 = nType2;
  return nType;
}

void AutoBrackets_Uninitialize(void)
{
  int i;

  nCurrentFileType = tftNone;
  nCurrentFileType2 = tfmNone;
  hCurrentEditWnd = NULL;
  hActualEditWnd = NULL;
  nAutoRightBracketPos = -1;
  nAutoRightBracketType = tbtNone;

  RemoveAllHighlightInfo(TRUE);

  for (i = 0; i < HIGHLIGHT_INDEXES; i++)
  {
    IndexesHighlighted[i] = -1;
    IndexesToHighlight[i] = -1;
    prevIndexesToHighlight[i] = -1;
  }
  CurrentBracketsIndexes[0] = -1;
  CurrentBracketsIndexes[1] = -1;

  NearestBracketsIndexes[0] = -1;
  NearestBracketsIndexes[1] = -1;
  NearestBracketsIndexes[2] = tbtNone;
  prevNearestBracketsIndexes[0] = -1;
  prevNearestBracketsIndexes[1] = -1;
  prevNearestBracketsIndexes[2] = tbtNone;
}

static void copyUniqueCharsOkW(tStringWrapperW* pDst, const wchar_t* pSrc)
{
  wchar_t* pDstStr;
  int n;

  pDstStr = pDst->pszStringW;
  n = 0;

  if (pSrc)
  {
    int     i;
    wchar_t wch;

    while ((wch = *pSrc) != 0)
    {
      if ((wch != L' ') &&
          (wch != L'\t') &&
          (wch != L'\x0D') &&
          (wch != L'\x0A'))
      {
        i = 0;
        while ((i < n) && (pDstStr[i] != wch))
        {
          ++i;
        }
        if (i == n)
        {
          pDstStr[n] = wch;
          ++n;
        }
      }
      ++pSrc;
    }
  }

  pDstStr[n] = 0;
  pDst->nLen = n;
}

void setNextCharOkW(const wchar_t* cszNextCharOkW)
{
  copyUniqueCharsOkW(&nextCharOkW, cszNextCharOkW);
}

void setPrevCharOkW(const wchar_t* cszPrevCharOkW)
{
  copyUniqueCharsOkW(&prevCharOkW, cszPrevCharOkW);
}

void setUserBracketsA(const char* cszUserBracketsA)
{
  strUserBracketsA[0][0] = 0;
  strUserBracketsA[0][1] = 0;
  strUserBracketsA[0][2] = 0;
  strUserBracketsA[0][3] = 0;

  if (cszUserBracketsA && cszUserBracketsA[0])
  {
    int n = 0;
    int i = 0;
    const char* p = cszUserBracketsA;

    while (n < MAX_USER_BRACKETS)
    {
      if ((*p) && (*p != ' ') && (*p != '\t'))
      {
        if (i < 2)
        {
          // brackets pair: 1st or 2nd character
          strUserBracketsA[n][i] = *p;
        }
        else if (i == 2)
        {
          // error: more than 2 characters
          strUserBracketsA[n][0] = 0;
          strUserBracketsA[n][1] = 0;
          strUserBracketsA[n][2] = 0;
          strUserBracketsA[n][3] = 0;
        }
        ++i;
      }
      else
      {
        if (i >= 1)
        {
          if (i == 2)
          {
            // brackets pair completed
            strUserBracketsA[n][2] = 0;
            strUserBracketsA[n][3] = 0;
            ++n;
          }
          // erase incomplete brackets pair
          strUserBracketsA[n][0] = 0;
          strUserBracketsA[n][1] = 0;
          strUserBracketsA[n][2] = 0;
          strUserBracketsA[n][3] = 0;
        }
        i = 0;
        if (*p == 0)
          break;
      }
      ++p;
    }
  }
}

void setUserBracketsW(const wchar_t* cszUserBracketsW)
{
  strUserBracketsW[0][0] = 0;
  strUserBracketsW[0][1] = 0;
  strUserBracketsW[0][2] = 0;
  strUserBracketsW[0][3] = 0;

  if (cszUserBracketsW && cszUserBracketsW[0])
  {
    int n = 0;
    int i = 0;
    const wchar_t* p = cszUserBracketsW;

    while (n < MAX_USER_BRACKETS)
    {
      if ((*p) && (*p != L' ') && (*p != L'\t'))
      {
        if (i < 2)
        {
          // brackets pair: 1st or 2nd character
          strUserBracketsW[n][i] = *p;
        }
        else if (i == 2)
        {
          // error: more than 2 characters
          strUserBracketsW[n][0] = 0;
          strUserBracketsW[n][1] = 0;
          strUserBracketsW[n][2] = 0;
          strUserBracketsW[n][3] = 0;
        }
        ++i;
      }
      else
      {
        if (i >= 1)
        {
          if (i == 2)
          {
            // brackets pair completed
            strUserBracketsW[n][2] = 0;
            strUserBracketsW[n][3] = 0;
            ++n;
          }
          // erase incomplete brackets pair
          strUserBracketsW[n][0] = 0;
          strUserBracketsW[n][1] = 0;
          strUserBracketsW[n][2] = 0;
          strUserBracketsW[n][3] = 0;
        }
        i = 0;
        if (*p == 0)
          break;
      }
      ++p;
    }
  }
}

const char* getCurrentBracketsPairA(void)
{
  static char szBracketsPair[4];
  int i;

  for (i = 0; i < 4; i++)
    szBracketsPair[i] = 0;

  if (CurrentBracketsIndexes[0] >= 0)
  {
    char ch;

    szBracketsPair[0] = AnyRichEdit_GetCharAt(hActualEditWnd, CurrentBracketsIndexes[0]);
    ch = AnyRichEdit_GetCharAt(hActualEditWnd, CurrentBracketsIndexes[1]);
    if (ch == '>')
    {
      // "<...>" or "<.../>"
      ch = AnyRichEdit_GetCharAt(hActualEditWnd, CurrentBracketsIndexes[1] - 1);
      if (ch == '/')
      {
        szBracketsPair[1] = '/';
        szBracketsPair[2] = '>';
      }
      else
        szBracketsPair[1] = '>';
    }
    else
      szBracketsPair[1] = ch;
  }

  return szBracketsPair;
}

const wchar_t* getCurrentBracketsPairW(void)
{
  static wchar_t szBracketsPair[4];
  int i;

  for (i = 0; i < 4; i++)
    szBracketsPair[i] = 0;

  if (CurrentBracketsIndexes[0] >= 0)
  {
    wchar_t wch;

    szBracketsPair[0] = AnyRichEdit_GetCharAtW(hActualEditWnd, CurrentBracketsIndexes[0]);
    wch = AnyRichEdit_GetCharAtW(hActualEditWnd, CurrentBracketsIndexes[1]);
    if (wch == L'>')
    {
      // "<...>" or "<.../>"
      wch = AnyRichEdit_GetCharAtW(hActualEditWnd, CurrentBracketsIndexes[1] - 1);
      if (wch == L'/')
      {
        szBracketsPair[1] = L'/';
        szBracketsPair[2] = L'>';
      }
      else
        szBracketsPair[1] = L'>';
    }
    else
      szBracketsPair[1] = wch;
  }

  return szBracketsPair;
}

enum eNearestBracketsJoinResult {
  nbrjNone = 0,
  nbrjLeftNearby,
  nbrjRightNearby,
  nbrjLeftDistant,
  nbrjRightDistant,
  nbrjLeftIntersected,
  nbrjRightIntersected
};

int JoinNearestBracketsRanges(const CHARRANGE_X* crOldSel, CHARRANGE_X* crJoinSel)
{
  if ((g_dwOptions[OPT_DWORD_NEARESTBR_SELTO_FLAGS] & XBR_NBR_SELTO_OUTERPOS) == 0)
  {
    BOOL bOuterPosAlready = FALSE;
    if (NearestBracketsIndexes[0] != -1 &&
        crJoinSel->cpMin == NearestBracketsIndexes[0] &&    //  [0] -> |(
        crJoinSel->cpMax == NearestBracketsIndexes[1] + 1)  //  [1] -> |)
    {
      bOuterPosAlready = TRUE;
    }
    else if (prevNearestBracketsIndexes[0] != -1 &&
             crJoinSel->cpMin == prevNearestBracketsIndexes[0] &&    //  [0] -> |(
             crJoinSel->cpMax == prevNearestBracketsIndexes[1] + 1)  //  [1] -> |)
    {
      bOuterPosAlready = TRUE;
    }
    if (!bOuterPosAlready)
    {
      //  {|...|}  ->  |{...}|
      --crJoinSel->cpMin;
      ++crJoinSel->cpMax;
    }
  }

  if (crJoinSel->cpMax == crOldSel->cpMin)
  {
    //  {...}|[...]|
    crJoinSel->cpMax = crOldSel->cpMax;
    return nbrjLeftNearby;
  }

  if (crJoinSel->cpMax < crOldSel->cpMin)
  {
    //  {...} |[...]|
    crJoinSel->cpMax = crOldSel->cpMax;
    return nbrjLeftDistant;
  }

  if (crJoinSel->cpMin == crOldSel->cpMax)
  {
    //  |[...]|{...}
    crJoinSel->cpMin = crOldSel->cpMin;
    return nbrjRightNearby;
  }

  if (crJoinSel->cpMin > crOldSel->cpMax)
  {
    //  |[...]| {...}
    crJoinSel->cpMin = crOldSel->cpMin;
    return nbrjRightDistant;
  }

  if (crJoinSel->cpMin < crOldSel->cpMin && crJoinSel->cpMax >= crOldSel->cpMin && crJoinSel->cpMax <= crOldSel->cpMax)
  {
    // left intersection:  (  [| ) |]  or  (  [ |)| ]  or  ([|)|]
    crJoinSel->cpMax = crOldSel->cpMax;
    return nbrjLeftIntersected;
  }

  if (crJoinSel->cpMin >= crOldSel->cpMin && crJoinSel->cpMin <= crOldSel->cpMax && crJoinSel->cpMax > crOldSel->cpMax)
  {
    // right intersection:  [| ( |]  )  or  [ |(| ]  )  or  [|(|])
    crJoinSel->cpMin = crOldSel->cpMin;
    return nbrjRightIntersected;
  }

  return nbrjNone;
}

BOOL WidenNearestBracketsSelection(HWND hWndEdit, const void* crSel)
{
  INT_X pos;
  INT_X pos_outer;
  int nOuterLeftBrType;
  int nOuterRightBrType;
  int nOuterLeftBrTypeInversed;
  int nOuterRightBrTypeInversed;
  int nInnerLeftBrType;
  int nInnerRightBrType;
  int nInnerLeftBrTypeInversed;
  int nInnerRightBrTypeInversed;
  int nJoined;
  BOOL bJoinedNearbyOnly;
  BOOL bInnerBracketsSelected;
  wchar_t left_ch;
  wchar_t right_ch;
  CHARRANGE_X crOldSel;
  CHARRANGE_X crNewSel;
  CHARRANGE_X crNewSelOuter;
  CHARRANGE_X crJoinSel;

  crOldSel.cpMin = ((const CHARRANGE_X *) crSel)->cpMin;
  crOldSel.cpMax = ((const CHARRANGE_X *) crSel)->cpMax;
  nJoined = nbrjNone;
  bJoinedNearbyOnly = TRUE;

  for (;;)
  {
    // 1. Let's check if there are outer brackets:  [|...|]
    nOuterLeftBrType = tbtNone;
    nOuterLeftBrTypeInversed = tbtNone;
    nOuterRightBrType = tbtNone;
    nOuterRightBrTypeInversed = tbtNone;

    left_ch = (crOldSel.cpMin != 0) ? getCharAt(hWndEdit, crOldSel.cpMin - 1) : 0;
    if (left_ch != 0)
    {
      nOuterLeftBrType = getLeftBracketType(left_ch, BTF_HIGHLIGHT);
      if (nOuterLeftBrType == tbtNone)
        nOuterLeftBrTypeInversed = getRightBracketType(left_ch, BTF_HIGHLIGHT);  //  ]|...
    }

    right_ch = getCharAt(hWndEdit, crOldSel.cpMax);
    if (right_ch != 0)
    {
      nOuterRightBrType = getRightBracketType(right_ch, BTF_HIGHLIGHT);
      if (nOuterRightBrType == tbtNone)
        nOuterRightBrTypeInversed = getLeftBracketType(right_ch, BTF_HIGHLIGHT);  //  ...|[
    }

    // 2. Let's check if there are inner brackets selected:  |[...]|
    nInnerLeftBrType = tbtNone;
    nInnerLeftBrTypeInversed = tbtNone;
    nInnerRightBrType = tbtNone;
    nInnerRightBrTypeInversed = tbtNone;
    bInnerBracketsSelected = FALSE;

    if (crOldSel.cpMin != crOldSel.cpMax)
    {
      // at least 1 character selected
      left_ch = getCharAt(hWndEdit, crOldSel.cpMin);
      if (left_ch != 0)
      {
        nInnerLeftBrType = getLeftBracketType(left_ch, BTF_HIGHLIGHT);
        if (nInnerLeftBrType == tbtNone)
          nInnerLeftBrTypeInversed = getRightBracketType(left_ch, BTF_HIGHLIGHT);  //  |]...
      }

      if (crOldSel.cpMin + 1 != crOldSel.cpMax)
      {
        // at least 2 characters selected
        right_ch = getCharAt(hWndEdit, crOldSel.cpMax - 1);
        if (right_ch != 0)
        {
          nInnerRightBrType = getRightBracketType(right_ch, BTF_HIGHLIGHT);
          if (nInnerRightBrType == tbtNone)
            nInnerRightBrTypeInversed = getLeftBracketType(right_ch, BTF_HIGHLIGHT);  //  ...[|

          if (nInnerLeftBrType != tbtNone && nInnerLeftBrType == nInnerRightBrType && 
              nJoined == nbrjNone && crOldSel.cpMin != crOldSel.cpMax - 1)
            bInnerBracketsSelected = TRUE;
        }
      }
    }

    if (bInnerBracketsSelected && nOuterLeftBrType != tbtNone &&
        nOuterLeftBrType == nOuterRightBrType && !isDuplicatedPair(nOuterLeftBrType))
    {
      //  [|(...)|]  -->  |[(...)]|
      crNewSel.cpMin = crOldSel.cpMin - 1;
      crNewSel.cpMax = crOldSel.cpMax + 1;
      SendMessage(hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &crNewSel);
      return TRUE;
    }

    // 3. Character position to start from
    pos = crOldSel.cpMin;
    pos_outer = -1; // outer pos for a second try

    if (nJoined == nbrjRightNearby || nJoined == nbrjRightDistant || nJoined == nbrjRightIntersected)
    {
        pos = crOldSel.cpMax + 1;
    }
    else if (nJoined == nbrjLeftNearby || nJoined == nbrjLeftDistant || nJoined == nbrjLeftIntersected)
    {
        pos = (crOldSel.cpMin != 0) ? (crOldSel.cpMin - 1) : (crOldSel.cpMax + 1);
    }
    else if (crOldSel.cpMin != crOldSel.cpMax)
    {
      if (crOldSel.cpMin + 1 == crOldSel.cpMax)
      {
        // just 1 character selected
        if (nInnerLeftBrType != tbtNone)  //  ...|[|...  or  ...|"|...
          pos = crOldSel.cpMin;
        else if (nInnerRightBrType != tbtNone)  //  ...|]|...
          pos = crOldSel.cpMax;
        else
        {
          if (nOuterLeftBrType != tbtNone)  //  ...(| |...  or  ...(| |(...  or  ...(| |)...
          {
            pos = crOldSel.cpMin;  //  ...(| |...)
            pos_outer = crOldSel.cpMin - 1;  //  "..."| |...
          }
          else if (nOuterLeftBrTypeInversed != tbtNone)  //  ...)| |...  or  ...)| |(...  or  ...)| |)...
            pos = crOldSel.cpMax;
          else  //  ...| |)...  or  ...| |(...  or  ...| |...
            pos = crOldSel.cpMin;
        }
      }
      else
      {
        // at least 2 characters selected
        if (bInnerBracketsSelected)
        {
          //  |[...]|
          if (nOuterLeftBrType != tbtNone)  //  (|[...]|
          {
            pos = crOldSel.cpMin;  //  (|[...[]|...
            if (isDuplicatedPair(nOuterLeftBrType))
              pos_outer = crOldSel.cpMin - 1;  //  "|[...{}...]|
            else
              pos_outer = crOldSel.cpMax + 1;  //  (|[...]|
          }
          else if (nOuterRightBrType != tbtNone)  //  |[...]|)
          {
            pos = crOldSel.cpMax;  //  |[]...]|)
            if (isDuplicatedPair(nOuterRightBrType))
              pos_outer = crOldSel.cpMax + 1;  //  |[...{}...]|"
            else
              pos_outer = crOldSel.cpMin - 1;  //  |[...]|)
          }
          else if (nOuterRightBrTypeInversed != tbtNone)  //  |[...]|(
          {
              pos = crOldSel.cpMax;  //  |[]...]|(
              pos_outer = crOldSel.cpMax + 1;  //  |[...{}...]|(
          }
          else if (nOuterLeftBrTypeInversed != tbtNone)  //  )|[...]|
          {
            pos = crOldSel.cpMin;  //  )|[...[]|
            pos_outer = crOldSel.cpMin - 1;  //  )|[...{}...]|
          }
          else
          {
            pos = crOldSel.cpMin;  //  |[...[]|...  or  |[...[]|
            pos_outer = crOldSel.cpMin - 1;  //  |[...{}...]|  or  |[...{}...]|
          }
        }
        else if (nInnerLeftBrType != tbtNone)
        {
          //  |[...|  or  "|[...|  or  |[...|"
          //  |[...)|  or  |[...(|
          pos = crOldSel.cpMin;  //  |[...{}...|  or  |[...(...)|
          if (nOuterRightBrTypeInversed != tbtNone)
            pos_outer = crOldSel.cpMax + 1;  //  |[...|{
          else if (isDuplicatedPair(nOuterRightBrType))
            pos_outer = crOldSel.cpMax;  //  |[...|"
          else
            pos_outer = (crOldSel.cpMin != 0) ? crOldSel.cpMin - 1 : crOldSel.cpMax + 1;  //  |[...]...|  or  |[...]...(|
        }
        else if (nInnerLeftBrTypeInversed != tbtNone)
        {
          //  |]...|  or  |]...}|  or  |]...{|  or  |]...|}  or  |]...|{
          pos = crOldSel.cpMax;
          pos_outer = crOldSel.cpMax + 1;
        }
        else if (nInnerRightBrType != tbtNone)
        {
          //  |...]|
          pos = crOldSel.cpMax;  //  |...{}...]|
          pos_outer = crOldSel.cpMin;  //  |...[...]|
        }
        else if (nInnerRightBrTypeInversed != tbtNone)
        {
          //  |...[|
          pos = crOldSel.cpMin;
        }
        else if (nOuterLeftBrType != tbtNone)
        {
          //  (|...|   or  (|...|(  or  (|...|(
          pos = crOldSel.cpMin;
          pos_outer = crOldSel.cpMax;
        }
        else if (nOuterLeftBrTypeInversed != tbtNone)
        {
          //  )|...|
          pos = crOldSel.cpMax;
          if (nOuterRightBrType != tbtNone || nOuterRightBrTypeInversed != tbtNone )  //  )|...|)  or  )|...|(
            pos_outer = crOldSel.cpMax + 2;
        }
        else if (nOuterRightBrType != tbtNone)
        {
          //  |...|)
          pos = crOldSel.cpMax;
          pos_outer = crOldSel.cpMin;
        }
        else
        {
          //   |...|  or  |...|(
          pos = crOldSel.cpMin;
        }
      }
    }

    // 4. finally, heavy artillery:
    if ( GetNearestBracketsRange(XBRA_SELTO_NEARBR, gnbrfWiden, pos, &crOldSel, &crNewSel) )
    {
      if ( (crNewSel.cpMin < crOldSel.cpMin && crNewSel.cpMax >= crOldSel.cpMax) ||
           (crNewSel.cpMin <= crOldSel.cpMin && crNewSel.cpMax > crOldSel.cpMax) )
      {
        SendMessage(hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &crNewSel);
        return TRUE;
      }

      if (crNewSel.cpMax > crOldSel.cpMax || crNewSel.cpMin < crOldSel.cpMin)
      {
        crJoinSel.cpMin = crNewSel.cpMin;
        crJoinSel.cpMax = crNewSel.cpMax;
        nJoined = JoinNearestBracketsRanges(&crOldSel, &crJoinSel);
        if (bJoinedNearbyOnly)
        {
          if ( (nJoined == nbrjLeftNearby && nInnerRightBrType != tbtNone) ||
               (nJoined == nbrjRightNearby && nInnerLeftBrType != tbtNone) )
          {
            SendMessage(hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &crJoinSel);
            return TRUE;
          }
        }
        if (nJoined != nbrjNone)
        {
          crOldSel.cpMin = crJoinSel.cpMin;
          crOldSel.cpMax = crJoinSel.cpMax;
          bJoinedNearbyOnly = FALSE;
          continue;
        }
      }

      if ( pos_outer != -1 &&
           GetNearestBracketsRange(XBRA_SELTO_NEARBR, gnbrfWiden, pos_outer, &crOldSel, &crNewSelOuter) )
      {
        if ( (crNewSelOuter.cpMin < crOldSel.cpMin && crNewSelOuter.cpMax >= crOldSel.cpMax) ||
             (crNewSelOuter.cpMin <= crOldSel.cpMin && crNewSelOuter.cpMax > crOldSel.cpMax) )
        {
          SendMessage(hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &crNewSelOuter);
          return TRUE;
        }

        if ( crOldSel.cpMin != crOldSel.cpMax &&
             (crNewSelOuter.cpMin != crNewSel.cpMin || crNewSelOuter.cpMax != crNewSel.cpMax) )
        {
          crJoinSel.cpMin = crNewSelOuter.cpMin;
          crJoinSel.cpMax = crNewSelOuter.cpMax;
          nJoined = JoinNearestBracketsRanges(&crOldSel, &crJoinSel);
          if (bJoinedNearbyOnly)
          {
            if ( (nJoined == nbrjLeftNearby && nInnerRightBrType != tbtNone) ||
                 (nJoined == nbrjRightNearby && nInnerLeftBrType != tbtNone) )
            {
              SendMessage(hWndEdit, EM_EXSETSEL_X, 0, (LPARAM) &crJoinSel);
              return TRUE;
            }
          }
          if (nJoined != nbrjNone)
          {
            crOldSel.cpMin = crJoinSel.cpMin;
            crOldSel.cpMax = crJoinSel.cpMax;
            bJoinedNearbyOnly = FALSE;
            continue;
          }
        }
      }
    }

    return FALSE;
  }

  return FALSE;
}

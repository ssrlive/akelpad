#ifndef _plugin_h_
#define _plugin_h_
//---------------------------------------------------------------------------
#include "AkelEdit.h" // "AkelEdit.h" MUST be before "AkelDLL.h"
#include "AkelDLL.h"  // "AkelDLL.h" MUST be after "AkelEdit.h"

#define  STR_PLUGINFUNC_SIZE  100
#define  STR_FILEEXTS_SIZE    500
#define  MAX_CUSTOM_COLOURS    16

#define  BRHLF_ENABLED        0x01
#define  BRHLF_TEXT           0x10
#define  BRHLF_BKGND          0x20


enum eOptDword {
  OPT_DWORD_AUTOCOMPLETE_ALL_AUTOBR = 0,
  OPT_DWORD_AUTOCOMPLETE_OVR_AUTOBR,
  OPT_DWORD_AUTOCOMPLETE_SEL_AUTOBR,
  OPT_DWORD_HIGHLIGHT_HLT_BOTHBR,
  OPT_DWORD_HIGHLIGHT_HLT_STYLE,
  OPT_DWORD_HIGHLIGHT_HLT_XMODE,
  OPT_DWORD_HIGHLIGHT_QUOTE_DETECT_LINES,
  OPT_DWORD_HIGHLIGHT_QUOTE_MAX_LINES,
  OPT_DWORD_HIGHLIGHT_BR_MAX_LINES,
  OPT_DWORD_NEARESTBR_GOTO_FLAGS,
  OPT_DWORD_NEARESTBR_SELTO_FLAGS,
  OPT_DWORD_NEARESTBR_MAX_LINES,

  OPT_DWORD_COUNT,

  OPT_COMMON_USER_BRPAIRS = OPT_DWORD_COUNT,
  OPT_AUTOBRACKETS_NEXT_CHAR_OK,
  OPT_AUTOBRACKETS_PREV_CHAR_OK,
  OPT_OPTIONSFLAGS,
  OPT_HIGHLIGHTRGB,
  OPT_HIGHLIGHTBKRGB,
  OPT_CUSTOMRGB,
  OPT_HTMLFILEEXTS,
  OPT_ESCAPED1FILEEXTS,
  OPT_COMMENT1FILEEXTS,
  OPT_XBRACKETS,

  OPT_TOTAL_COUNT
};

int  IsBracketsHighlight(UINT uBrHglt);
BOOL GetFuncNameOfXBracketsMain(const PLUGINDATA *pd);
BOOL PluginCallXBracketsMain(HWND hMainWnd, BOOL bOldWindows);

void Initialize(PLUGINDATA* pd);
void Uninitialize(BOOL bIsExiting /* = FALSE */);

//---------------------------------------------------------------------------
#endif


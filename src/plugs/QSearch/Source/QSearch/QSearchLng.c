#include "QSearchLng.h"
#include "QSearch.h"
#include "XMemStrFunc.h"
#include "resource.h"


// consts
#define  INLNG_ENG    0
#define  INLNG_RUS    1
#define  INLNG_UKR    2
#define  INLNG_COUNT  3

const char* szHintBtCancelA[INLNG_COUNT] = {
    /* eng */
    ( "Close" ),
    /* rus */
    ( "\xC7\xE0\xEA\xF0\xFB\xF2\xFC" ),
    /* ukr */
    ( "\xC7\xE0\xEA\xF0\xE8\xF2\xE8" )
};

const char* szHintBtFindNextA[INLNG_COUNT] = {
    /* eng */
    ( "Find next (down)" ),
    /* rus */
    ( "\xC8\xF1\xEA\xE0\xF2\xFC\x20\xF1\xEB\xE5\xE4\x2E\x20\x28\xE2\xED\xE8" \
      "\xE7\x29" ),
    /* ukr */
    ( "\xD8\xF3\xEA\xE0\xF2\xE8\x20\xED\xE0\xF1\xF2\xF3\xEF\x2E\x20\x28\xF3" \
      "\xED\xE8\xE7\x29" )
};

const char* szHintBtFindPrevA[INLNG_COUNT] = {
    /* eng */
    ( "Find previous (up)" ),
    /* rus */
    ( "\xC8\xF1\xEA\xE0\xF2\xFC\x20\xEF\xF0\xE5\xE4\x2E\x20\x28\xE2\xE2\xE5" \
      "\xF0\xF5\x29" ),
    /* ukr */
    ( "\xD8\xF3\xEA\xE0\xF2\xE8\x20\xEF\xEE\xEF\xE5\xF0\xE5\xE4\x2E\x20\x28" \
      "\xF3\xE2\xE5\xF0\xF5\x29" )
};

const char* szHintBtFindAllA[INLNG_COUNT] = {
    /* eng */
    ( "Find all" ),
    /* rus */
    ( "\xCD\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5" ),
    /* ukr */
    ( "\xC7\xED\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5" )
};

const char* szHintChMatchCaseA[INLNG_COUNT][2] = {
    /* eng */
    { 
        "Match case",
        "Match case"
    },
    /* rus */
    {
        "\xD3\xF7\xE8\xF2\xFB\xE2\xE0\xF2\xFC\x20\xF0\xE5\xE3\xE8\xF1\xF2\xF0",
        "\xD3\xF7\xE8\xF2\x2E\xF0\xE5\xE3\xE8\xF1\xF2\xF0",
    },
    /* ukr */
    {
        "\xC2\xF0\xE0\xF5\xEE\xE2\xF3\xE2\xE0\xF2\xE8\x20\xF0\xE5\xE3\x69\xF1\xF2\xF0",
        "\xC2\xF0\xE0\xF5\x2E\xF0\xE5\xE3\x69\xF1\xF2\xF0"
    }
};

const char* szHintChWholeWordA[INLNG_COUNT][2] = {
    /* eng */
    {
        "Whole word%s\n" \
          "Search mode (%sCtrl+click)",
        "Whole word"
    },
    /* rus */
    {
        "\xD1\xEB\xEE\xE2\xEE\x20\xF6\xE5\xEB\xE8\xEA\xEE\xEC%s\n" \
          "\xD0\xE5\xE6\xE8\xEC\x20\xEF\xEE\xE8\xF1\xEA\xE0 (%sCtrl+\xEA\xEB\xE8\xEA)",
        "\xD1\xEB\x2E\xF6\xE5\xEB\xE8\xEA\xEE\xEC"
    },
    /* ukr */
    {
        "\xD1\xEB\xEE\xE2\xEE\x20\xF6\x69\xEB\xEA\xEE\xEC%s\n" \
          "\xD0\xE5\xE6\xE8\xEC\x20\xEF\xEE\xF8\xF3\xEA\xF3 (%sCtrl+\xEA\xEB\xB3\xEA)",
        "\xD1\xEB\xEE\xE2\x2E\xF6\x69\xEB\xEA\xEE\xEC"
    }
};

const char* szHintChHighlightAllA[INLNG_COUNT][2] = {
    /* eng */
    {
        "Highlight all",
        "Highlight all"
    },
    /* rus */
    {
        "\xCF\xEE\xE4\xF1\xE2\xE5\xF2\xE8\xF2\xFC\x20\xE2\xF1\xE5",
        "\xCF\xEE\xE4\xF1\xE2\x2E\xE2\xF1\xE5"
    },
    /* ukr */
    {
        "\xCF\x69\xE4\xF1\xE2\x69\xF2\xE8\xF2\xE8\x20\xE2\xF1\x69",
        "\xCF\x69\xE4\xF1\xE2\x2E\xE2\xF1\x69"
    }
};

const char* szHintEdTextA[INLNG_COUNT] = {
    /* eng */
    ( "Enter, F3 \t-  Find next (down)\r\n" \
      "Shift+Enter\t-  Find previous (up)\r\n" \
      "Alt+Enter\t-  Find from beginning\r\n" \
      "Ctrl+Enter \t-  Pick up selected text" ),
    /* rus */
    ( "Enter, F3 \t-  \xC8\xF1\xEA\xE0\xF2\xFC\x20\xF1\xEB\xE5\xE4\x2E\x20\x28" \
      "\xE2\xED\xE8\xE7\x29\r\n" \
      "Shift+Enter\t-  \xC8\xF1\xEA\xE0\xF2\xFC\x20\xEF\xF0\xE5\xE4\x2E\x20" \
      "\x28\xE2\xE2\xE5\xF0\xF5\x29\r\n" \
      "Alt+Enter\t-  \xC8\xF1\xEA\xE0\xF2\xFC\x20\xF1\x20\xED\xE0\xF7\xE0" \
      "\xEB\xE0\r\n" \
      "Ctrl+Enter \t-  \xCF\xEE\xE4\xF5\xE2\xE0\xF2\xE8\xF2\xFC\x20\xE2\xFB" \
      "\xE4\xE5\xEB\xE5\xED\xED\xFB\xE9\x20\xF2\xE5\xEA\xF1\xF2" ),
    /* ukr */
    ( "Enter, F3 \t-  \xD8\xF3\xEA\xE0\xF2\xE8\x20\xED\xE0\xF1\xF2\xF3\xEF\x2E" \
      "\x20\x28\xF3\xED\xE8\xE7\x29\r\n" \
      "Shift+Enter\t-  \xD8\xF3\xEA\xE0\xF2\xE8\x20\xEF\xEE\xEF\xE5\xF0\xE5" \
      "\xE4\x2E\x20\x28\xF3\xE2\xE5\xF0\xF5\x29\r\n" \
      "Alt+Enter\t-  \xD8\xF3\xEA\xE0\xF2\xE8\x20\xE7\x20\xEF\xEE\xF7\xE0" \
      "\xF2\xEA\xF3\r\n" \
      "Ctrl+Enter \t-  \xCF\x69\xE4\xF5\xEE\xEF\xE8\xF2\xE8\x20\xE2\xE8\xE4" \
      "\x69\xEB\xE5\xED\xE8\xE9\x20\xF2\xE5\xEA\xF1\xF2" )
};

const char* szHintStrEOFReachedA[INLNG_COUNT] = {
    /* eng */
    ( "End of file has been reached. Continue?" ),
    /* rus */
    ( "\xC4\xEE\xF1\xF2\xE8\xE3\xED\xF3\xF2\x20\xEA\xEE\xED\xE5\xF6\x20\xF4" \
      "\xE0\xE9\xEB\xE0\x2E\x20\xCF\xF0\xEE\xE4\xEE\xEB\xE6\xE8\xF2\xFC\x3F" ),
    /* ukr */
    ( "\xC4\xEE\xF1\xFF\xE3\xED\xF3\xF2\xEE\x20\xEA\x69\xED\xF6\xFF\x20\xF4" \
      "\xE0\xE9\xEB\xE0\x2E\x20\xCF\xF0\xEE\xE4\xEE\xE2\xE6\xE8\xF2\xE8\x3F" )
};

const char* szPopupMenuA[INLNG_COUNT][OPTF_COUNT - 1] = {
    /* eng */
    {
        ( "Search: On-the-fly mode" ),
        ( "Search: Always from beginning" ),
        ( "Search: Use special characters ( \\n, \\t, * and ? )" ),
        ( "Search: Use regular expressions" ),
        ( "Search:  .  matches  \\n  in reg.expressions" ),
        ( "Search: Pick up selected text" ),
        ( "Search: SelFind picks up selected text" ),
        ( "Search: Stop at EOF (end of file)" ),
        ( "Window: Docked top" ),
        ( "Window: Select all when focused" ),
        ( "Window: Catch main F3" ),
        ( "Window: Catch main Esc" ),
        ( "Window: Hotkey hides the panel" ),
        ( "Window: Editor auto-focus (on mouse move)" ),
        ( "Window: QSearch auto-focus (on mouse move)" ),
        ( "Window: QSearch auto-focus (on file open)" )
    },
    /* rus */
    {
        ( "\xCF\xEE\xE8\xF1\xEA\x3A\x20\xC8\xF1\xEA\xE0\xF2\xFC\x20\x22\xED" \
          "\xE0\x20\xEB\xE5\xF2\xF3\x22" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A\x20\xC2\xF1\xE5\xE3\xE4\xE0\x20\xF1\x20" \
          "\xED\xE0\xF7\xE0\xEB\xE0" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A\x20\xC8\xF1\xEF\xEE\xEB\xFC\xE7\xEE\xE2" \
          "\xE0\xF2\xFC\x20\xF1\xEF\xE5\xF6\x2E\x20\xF1\xE8\xEC\xE2\xEE\xEB" \
          "\xFB ( \\n, \\t, * \xE8 ? )" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A\x20\xC8\xF1\xEF\xEE\xEB\xFC\xE7\xEE\xE2" \
          "\xE0\xF2\xFC\x20\xF0\xE5\xE3\xF3\xEB\xFF\xF0\xED\xFB\xE5\x20\xE2" \
          "\xFB\xF0\xE0\xE6\xE5\xED\xE8\xFF" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A  .  \xE7\xE0\xF5\xE2\xE0\xF2\xFB\xE2\xE0" \
          "\xE5\xF2  \\n  \xE2\x20\xF0\xE5\xE3\x2E\xE2\xFB\xF0\xE0\xE6\xE5" \
          "\xED\xE8\xFF\xF5" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A\x20\xCF\xEE\xE4\xF5\xE2\xE0\xF2\xFB\xE2" \
          "\xE0\xF2\xFC\x20\xE2\xFB\xE4\xE5\xEB\xE5\xED\xED\xFB\xE9\x20\xF2" \
          "\xE5\xEA\xF1\xF2" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A SelFind \xEF\xEE\xE4\xF5\xE2\xE0\xF2\xFB" \
          "\xE2\xE0\xE5\xF2\x20\xE2\xFB\xE4\xE5\xEB\xE5\xED\xED\xFB\xE9\x20" \
          "\xF2\xE5\xEA\xF1\xF2" ),
        ( "\xCF\xEE\xE8\xF1\xEA\x3A\x20\xCE\xF1\xF2\xE0\xED\xE0\xE2\xEB\xE8" \
          "\xE2\xE0\xF2\xFC\xF1\xFF\x20\xE2\x20\xEA\xEE\xED\xF6\xE5\x20\xF4" \
          "\xE0\xE9\xEB\xE0" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xD0\xE0\xF1\xEF\xEE\xEB\xEE\xE6\xE8" \
          "\xF2\xFC\x20\xF1\xE2\xE5\xF0\xF5\xF3" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xD4\xEE\xEA\xF3\xF1\x20\xE2\xE2\xEE" \
          "\xE4\xE0\x20\xE2\xFB\xE4\xE5\xEB\xFF\xE5\xF2\x20\xE2\xF1\xB8" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xCE\xE1\xF0\xE0\xE1\xE0\xF2\xFB\xE2" \
          "\xE0\xF2\xFC F3 \xEE\xF2\x20\xE3\xEB\xE0\xE2\xED" \
          "\xEE\xE3\xEE\x20\xEE\xEA\xED\xE0" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xCE\xE1\xF0\xE0\xE1\xE0\xF2\xFB\xE2" \
          "\xE0\xF2\xFC Esc \xEE\xF2\x20\xE3\xEB\xE0\xE2\xED" \
          "\xEE\xE3\xEE\x20\xEE\xEA\xED\xE0" ),  
        ( "\xCE\xEA\xED\xEE\x3A\x20 Hotkey \xF1\xEA\xF0\xFB\xE2\xE0\xE5\xF2" \
          "\x20\xEF\xE0\xED\xE5\xEB\xFC" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xC0\xE2\xF2\xEE\x2D\xF4\xEE\xEA\xF3" \
          "\xF1\x20\xE2\x20\xF0\xE5\xE4\xE0\xEA\xF2\xEE\xF0 " \
          "(\xE4\xE2\xE8\xE6\xE5\xED\xE8\xE5\xEC\x20\xEC\xFB\xF8\xE8)" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xC0\xE2\xF2\xEE\x2D\xF4\xEE\xEA\xF3" \
          "\xF1\x20\xE2 QSearch " \
          "(\xE4\xE2\xE8\xE6\xE5\xED\xE8\xE5\xEC\x20\xEC\xFB\xF8\xE8)" ),
        ( "\xCE\xEA\xED\xEE\x3A\x20\x20\xC0\xE2\xF2\xEE\x2D\xF4\xEE\xEA\xF3" \
          "\xF1\x20\xE2 QSearch " \
          "(\xEF\xF0\xE8\x20\xEE\xF2\xEA\xF0\x2E\x20\xF4\xE0\xE9\xEB\xE0)" )
    },
    /* ukr */
    {
        ( "\xCF\xEE\xF8\xF3\xEA\x3A\x20\xD8\xF3\xEA\xE0\xF2\xE8\x20\xED\xE5" \
          "\xE3\xE0\xE9\xED\xEE" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A\x20\xC7\xE0\xE2\xE6\xE4\xE8\x20\xE7\x20" \
          "\xEF\xEE\xF7\xE0\xF2\xEA\xF3" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A\x20\xC2\xE8\xEA\xEE\xF0\xE8\xF1\xF2\x2E" \
          "\x20\xF1\xEF\xE5\xF6\x2E\x20\xF1\xE8\xEC\xE2\xEE\xEB\xE8" \
          " ( \\n, \\t, * \xF2\xE0 ? )" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A\x20\xC2\xE8\xEA\xEE\xF0\xE8\xF1\xF2\x2E" \
          "\x20\xF0\xE5\xE3\xF3\xEB\xFF\xF0\xED\xB3\x20\xE2\xE8\xF0\xE0\xE7" \
          "\xE8" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A  .  \xE7\xE0\xF5\xEE\xEF\xEB\xFE\xBA" \
          "  \\n  \xE2\x20\xF0\xE5\xE3\x2E\xE2\xE8\xF0\xE0\xE7\xE0\xF5" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A\x20\xCF\x69\xE4\xF5\xEE\xEF\xEB\xFE\xE2" \
          "\xE0\xF2\xE8\x20\xE2\xE8\xE4\x69\xEB\xE5\xED\xE8\xE9\x20\xF2\xE5" \
          "\xEA\xF1\xF2" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A SelFind \xEF\x69\xE4\xF5\xEE\xEF\xEB\xFE" \
          "\xBA\x20\xE2\xE8\xE4\x69\xEB\xE5\xED\xE8\xE9\x20\xF2\xE5\xEA\xF1" \
          "\xF2" ),
        ( "\xCF\xEE\xF8\xF3\xEA\x3A\x20\xC7\xF3\xEF\xE8\xED\xFF\xF2\xE8\xF1" \
          "\xFC\x20\xF3\x20\xEA\x69\xED\xF6\x69\x20\xF4\xE0\xE9\xEB\xE0" ),
        ( "\xC2\x69\xEA\xED\xEE\x3A\x20\xD0\xEE\xE7\xF2\xE0\xF8\xF3\xE2\xE0" \
          "\xF2\xE8\x20\xE7\xE2\xE5\xF0\xF5\xF3" ),
        ( "\xC2\x69\xEA\xED\xEE\x3A\x20\xD4\xEE\xEA\xF3\xF1\x20\xE2\xE2\xEE" \
          "\xE4\xF3\x20\xE2\xE8\xE4\x69\xEB\xFF\xBA\x20\xE2\xF1\xE5" ),
        ( "\xC2\x69\xEA\xED\xEE\x3A\x20\xCE\xE1\xF0\xEE\xE1\xEB\xFF\xF2\xE8" \
          " F3 \xE2\x69\xE4\x20\xE3\xEE\xEB\xEE\xE2\xED\xEE" \
          "\xE3\xEE\x20\xE2\x69\xEA\xED\xE0" ),
        ( "\xC2\x69\xEA\xED\xEE\x3A\x20\xCE\xE1\xF0\xEE\xE1\xEB\xFF\xF2\xE8" \
          " Esc \xE2\x69\xE4\x20\xE3\xEE\xEB\xEE\xE2\xED\xEE" \
          "\xE3\xEE\x20\xE2\x69\xEA\xED\xE0" ),  
        ( "\xC2\x69\xEA\xED\xEE\x3A Hotkey \xF5\xEE\xE2\xE0\xBA\x20\xEF\xE0" \
          "\xED\xE5\xEB\xFC" ),
        ( "\xC2\xB3\xEA\xED\xEE\x3A\x20\xC0\xE2\xF2\xEE\x2D\xF4\xEE\xEA\xF3" \
          "\xF1\x20\xE2\x20\xF0\xE5\xE4\xE0\xEA\xF2\xEE\xF0 " \
          "(\xF0\xF3\xF5\xEE\xEC\x20\xEC\xE8\xF8\xB3)" ),
        ( "\xC2\xB3\xEA\xED\xEE\x3A\x20\xC0\xE2\xF2\xEE\x2D\xF4\xEE\xEA\xF3" \
          "\xF1\x20\xE2 QSearch " \
          "(\xF0\xF3\xF5\xEE\xEC\x20\xEC\xE8\xF8\xB3)" ),
        ( "\xC2\xB3\xEA\xED\xEE\x3A\x20\xC0\xE2\xF2\xEE\x2D\xF4\xEE\xEA\xF3" \
          "\xF1\x20\xE2 QSearch " \
          "(\xEF\xF0\xE8\x20\xE2\xB3\xE4\xEA\xF0\x2E\x20\xF4\xE0\xE9\xEB\xF3)" )
    }
};

const char* szFindAllPopupMenuA[INLNG_COUNT][QS_FINDALL_TOTAL] = {
    /* eng */
    {
        "Auto Count All",
        "Find All: Count Only",
        "Find All: Log Output",
        "Find All: File Output (N tabs)",
        "Find All: File Output (1 tab)",
        "Output: Whole line",
        "Output: Match only",
        "Output: Filter mode",
        "Output options..."
    },
    /* rus */
    {
        "\xC0\xE2\xF2\xEE\x2D\xEF\xEE\xE4\xF1\xF7\xB8\xF2\x20\xE2\xF1\xE5\xF5\x20\xE2\xF5\xEE\xE6\xE4\xE5\xED\xE8\xE9",
        "\xCD\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xD2\xEE\xEB\xFC\xEA\xEE\x20\xEF\xEE\xE4\xF1\xF7\xB8\xF2",
        "\xCD\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xC2\xFB\xE2\xEE\xE4\x20\xE2 Log::Output",
        "\xCD\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xC2\xFB\xE2\xEE\xE4\x20\xE2\x20\xF4\xE0\xE9\xEB (N \xE2\xEA\xEB\xE0\xE4\xEE\xEA)",
        "\xCD\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xC2\xFB\xE2\xEE\xE4\x20\xE2\x20\xF4\xE0\xE9\xEB (1 \xE2\xEA\xEB\xE0\xE4\xEA\xE0)",
        "\xC2\xFB\xE2\xEE\xE4: \xC2\xF1\xFF\x20\xF1\xF2\xF0\xEE\xEA\xE0",
        "\xC2\xFB\xE2\xEE\xE4: \xD2\xEE\xEB\xFC\xEA\xEE\x20\xF1\xEE\xE2\xEF\xE0\xE4\xE5\xED\xE8\xE5",
        "\xC2\xFB\xE2\xEE\xE4: \xD0\xE5\xE6\xE8\xEC\x20\xF4\xE8\xEB\xFC\xF2\xF0\xE0",
        "\xCD\xE0\xF1\xF2\xF0\xEE\xE9\xEA\xE8\x20\xE2\xFB\xE2\xEE\xE4\xE0..."
    },
    /* ukr */
    {
        "\xC0\xE2\xF2\xEE\x2D\xEF\xB3\xE4\xF0\xE0\xF5\xF3\xED\xEE\xEA\x20\xF3\xF1\xB3\xF5\x20\xE2\xF5\xEE\xE4\xE6\xE5\xED\xFC",
        "\xC7\xED\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xD2\xB3\xEB\xFC\xEA\xE8\x20\xEF\xB3\xE4\xF0\xE0\xF5\xF3\xED\xEE\xEA",
        "\xC7\xED\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xC2\xE8\xE2\xB3\xE4\x20\xF3 Log::Output",
        "\xC7\xED\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xC2\xE8\xE2\xB3\xE4\x20\xF3\x20\xF4\xE0\xE9\xEB (N \xE2\xEA\xEB\xE0\xE4\xEE\xEA)",
        "\xC7\xED\xE0\xE9\xF2\xE8\x20\xE2\xF1\xE5: \xC2\xE8\xE2\xB3\xE4\x20\xF3\x20\xF4\xE0\xE9\xEB (1 \xE2\xEA\xEB\xE0\xE4\xEA\xE0)",
        "\xC2\xE8\xE2\xB3\xE4: \xD3\xE2\xE5\xF1\xFC\x20\xF0\xFF\xE4\xEE\xEA",
        "\xC2\xE8\xE2\xB3\xE4: \xD2\xB3\xEB\xFC\xEA\xE8\x20\xF1\xEF\xB3\xE2\xEF\xE0\xE4\xB3\xED\xED\xFF",
        "\xC2\xE8\xE2\xB3\xE4: \xD0\xE5\xE6\xE8\xEC\x20\xF4\xB3\xEB\xFC\xF2\xF0\xE0",
        "\xCD\xE0\xF1\xF2\xF0\xEE\xE9\xEA\xE8\x20\xE2\xE8\xE2\xEE\xE4\xF3..."
    }
};

const wchar_t* szHintBtCancelW[INLNG_COUNT] = {
    /* eng */
    ( L"Close" ),
    /* rus */
    ( L"\x0417\x0430\x043A\x0440\x044B\x0442\x044C" ),
    /* ukr */
    ( L"\x0417\x0430\x043A\x0440\x0438\x0442\x0438" )
};

const wchar_t* szHintBtFindNextW[INLNG_COUNT] = {
    /* eng */
    ( L"Find next (down)" ),
    /* rus */
    ( L"\x0418\x0441\x043A\x0430\x0442\x044C\x0020\x0441\x043B\x0435\x0434" \
      L"\x002E\x0020\x0028\x0432\x043D\x0438\x0437\x0029" ),
    /* ukr */
    ( L"\x0428\x0443\x043A\x0430\x0442\x0438\x0020\x043D\x0430\x0441\x0442" \
      L"\x0443\x043F\x002E\x0020\x0028\x0443\x043D\x0438\x0437\x0029" )
};

const wchar_t* szHintBtFindPrevW[INLNG_COUNT] = {
    /* eng */
    ( L"Find previous (up)" ),
    /* rus */
    ( L"\x0418\x0441\x043A\x0430\x0442\x044C\x0020\x043F\x0440\x0435\x0434" \
      L"\x002E\x0020\x0028\x0432\x0432\x0435\x0440\x0445\x0029" ),
    /* ukr */
    ( L"\x0428\x0443\x043A\x0430\x0442\x0438\x0020\x043F\x043E\x043F\x0435" \
      L"\x0440\x0435\x0434\x002E\x0020\x0028\x0443\x0432\x0435\x0440\x0445" \
      L"\x0029" )
};

const wchar_t* szHintBtFindAllW[INLNG_COUNT] = {
    /* eng */
    ( L"Find all\n" \
          L"(Shift+click - in all files)" ),
    /* rus */
    ( L"\x041D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435\n" \
          L"(Shift+\x043A\x043B\x0438\x043A - \x0432\x043E\x0020\x0432\x0441\x0435\x0445\x0020\x0444\x0430\x0439\x043B\x0430\x0445)" ),
    /* ukr */
    ( L"\x0417\x043D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435\n" \
          L"(Shift+\x043A\x043B\x0456\x043A - \x0432\x0020\x0443\x0441\x0456\x0445\x0020\x0444\x0430\x0439\x043B\x0430\x0445)" )
};

const wchar_t* szHintChMatchCaseW[INLNG_COUNT][2] = {
    /* eng */
    {
        L"Match case",
        L"Match case"
    },
    /* rus */
    {
        L"\x0423\x0447\x0438\x0442\x044B\x0432\x0430\x0442\x044C\x0020\x0440\x0435\x0433\x0438\x0441\x0442\x0440",
        L"\x0423\x0447\x0438\x0442\x002E\x0440\x0435\x0433\x0438\x0441\x0442\x0440"
    },
    /* ukr */
    {
        L"\x0412\x0440\x0430\x0445\x043E\x0432\x0443\x0432\x0430\x0442\x0438\x0020\x0440\x0435\x0433\x0069\x0441\x0442\x0440",
        L"\x0412\x0440\x0430\x0445\x002E\x0440\x0435\x0433\x0069\x0441\x0442\x0440"
    }
};

const wchar_t* szHintChWholeWordW[INLNG_COUNT][2] = {
    /* eng */
    {
        L"Whole word%s\n" \
          L"Search mode (%sCtrl+click)",
        L"Whole word"
    },
    /* rus */
    {
        L"\x0421\x043B\x043E\x0432\x043E\x0020\x0446\x0435\x043B\x0438\x043A\x043E\x043C%s\n" \
          L"\x0420\x0435\x0436\x0438\x043C\x0020\x043F\x043E\x0438\x0441\x043A\x0430 (%sCtrl+\x043A\x043B\x0438\x043A)",
        L"\x0421\x043B\x002E\x0446\x0435\x043B\x0438\x043A\x043E\x043C"
    },
    /* ukr */
    {
        L"\x0421\x043B\x043E\x0432\x043E\x0020\x0446\x0069\x043B\x043A\x043E\x043C%s\n" \
          L"\x0420\x0435\x0436\x0438\x043C\x0020\x043F\x043E\x0448\x0443\x043A\x0443 (%sCtrl+\x043A\x043B\x0456\x043A)",
        L"\x0421\x043B\x043E\x0432\x002E\x0446\x0069\x043B\x043A\x043E\x043C"
    }
};

const wchar_t* szHintChHighlightAllW[INLNG_COUNT][2] = {
    /* eng */
    {
        L"Highlight all",
        L"Highlight all"
    },
    /* rus */
    {
        L"\x041F\x043E\x0434\x0441\x0432\x0435\x0442\x0438\x0442\x044C\x0020\x0432\x0441\x0435",
        L"\x041F\x043E\x0434\x0441\x0432\x002E\x0432\x0441\x0435"
    },
    /* ukr */
    {
        L"\x041F\x0069\x0434\x0441\x0432\x0069\x0442\x0438\x0442\x0438\x0020\x0432\x0441\x0069",
        L"\x041F\x0069\x0434\x0441\x0432\x002E\x0432\x0441\x0069"
    }
};

const wchar_t* szHintEdTextW[INLNG_COUNT] = {
    /* eng */
    ( L"Enter, F3 \t-  Find next (down)\r\n" \
      L"Shift+Enter\t-  Find previous (up)\r\n" \
      L"Alt+Enter\t-  Find from beginning\r\n" \
      L"Alt+Shift+Enter\t-  Find all\r\n" \
      L"Ctrl+Shift+Enter\t-  Find all (in all files)\r\n" \
      L"Ctrl+Enter \t-  Pick up selected text" ),
    /* rus */
    ( L"Enter, F3 \t-  \x0418\x0441\x043A\x0430\x0442\x044C\x0020\x0441\x043B" \
      L"\x0435\x0434\x002E\x0020\x0028\x0432\x043D\x0438\x0437\x0029\r\n" \
      L"Shift+Enter\t-  \x0418\x0441\x043A\x0430\x0442\x044C\x0020\x043F" \
      L"\x0440\x0435\x0434\x002E\x0020\x0028\x0432\x0432\x0435\x0440\x0445" \
      L"\x0029\r\n" \
      L"Alt+Enter\t-  \x0418\x0441\x043A\x0430\x0442\x044C\x0020\x0441" \
      L"\x0020\x043D\x0430\x0447\x0430\x043B\x0430\r\n" \
      L"Alt+Shift+Enter\t-  \x041D\x0430\x0439\x0442\x0438\x0020" \
      L"\x0432\x0441\x0435\r\n" \
      L"Ctrl+Shift+Enter\t-  \x041D\x0430\x0439\x0442\x0438\x0020" \
      L"\x0432\x0441\x0435 (\x0432\x043E\x0020\x0432\x0441\x0435\x0445" \
      L"\x0020\x0444\x0430\x0439\x043B\x0430\x0445)\r\n" \
      L"Ctrl+Enter \t-  \x041F\x043E\x0434\x0445\x0432\x0430\x0442\x0438\x0442" \
      L"\x044C\x0020\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x044B" \
      L"\x0439\x0020\x0442\x0435\x043A\x0441\x0442" ),
    /* ukr */
    ( L"Enter, F3 \t-  \x0428\x0443\x043A\x0430\x0442\x0438\x0020\x043D\x0430" \
      L"\x0441\x0442\x0443\x043F\x002E\x0020\x0028\x0443\x043D\x0438\x0437" \
      L"\x0029\r\n" \
      L"Shift+Enter\t-  \x0428\x0443\x043A\x0430\x0442\x0438\x0020\x043F" \
      L"\x043E\x043F\x0435\x0440\x0435\x0434\x002E\x0020\x0028\x0443\x0432" \
      L"\x0435\x0440\x0445\x0029\r\n" \
      L"Alt+Enter\t-  \x0428\x0443\x043A\x0430\x0442\x0438\x0020\x0437" \
      L"\x0020\x043F\x043E\x0447\x0430\x0442\x043A\x0443\r\n" \
      L"Alt+Shift+Enter\t-  \x0417\x043D\x0430\x0439\x0442\x0438\x0020" \
      L"\x0432\x0441\x0435\r\n" \
      L"Ctrl+Shift+Enter\t-  \x0417\x043D\x0430\x0439\x0442\x0438\x0020" \
      L"\x0432\x0441\x0435 (\x0432\x0020\x0443\x0441\x0456\x0445\x0020" \
      L"\x0444\x0430\x0439\x043B\x0430\x0445)\r\n" \
      L"Ctrl+Enter \t-  \x041F\x0069\x0434\x0445\x043E\x043F\x0438\x0442\x0438" \
      L"\x0020\x0432\x0438\x0434\x0069\x043B\x0435\x043D\x0438\x0439\x0020" \
      L"\x0442\x0435\x043A\x0441\x0442" )
};

const wchar_t* szHintStrEOFReachedW[INLNG_COUNT] = {
    /* eng */
    ( L"End of file has been reached. Continue?" ),
    /* rus */
    ( L"\x0414\x043E\x0441\x0442\x0438\x0433\x043D\x0443\x0442\x0020\x043A" \
      L"\x043E\x043D\x0435\x0446\x0020\x0444\x0430\x0439\x043B\x0430\x002E" \
      L"\x0020\x041F\x0440\x043E\x0434\x043E\x043B\x0436\x0438\x0442\x044C" \
      L"\x003F" ),
    /* ukr */
    ( L"\x0414\x043E\x0441\x044F\x0433\x043D\x0443\x0442\x043E\x0020\x043A" \
      L"\x0069\x043D\x0446\x044F\x0020\x0444\x0430\x0439\x043B\x0430\x002E" \
      L"\x0020\x041F\x0440\x043E\x0434\x043E\x0432\x0436\x0438\x0442\x0438" \
      L"\x003F" )
};

const wchar_t* szPopupMenuW[INLNG_COUNT][OPTF_COUNT - 1] = {
    /* eng */
    {
        ( L"Search: On-the-fly mode" ),
        ( L"Search: Always from beginning" ),
        ( L"Search: Use special characters ( \\n, \\t, * and ? )" ),
        ( L"Search: Use regular expressions" ),
        ( L"Search:  .  matches  \\n  in reg.expressions" ),
        ( L"Search: Pick up selected text" ),
        ( L"Search: SelFind picks up selected text" ),
        ( L"Search: Stop at EOF (end of file)" ),
        ( L"Window: Docked top" ),
        ( L"Window: Select all when focused" ),
        ( L"Window: Catch main F3" ),
        ( L"Window: Catch main Esc" ),
        ( L"Window: Hotkey hides the panel" ),
        ( L"Window: Editor auto-focus (on mouse move)" ),
        ( L"Window: QSearch auto-focus (on mouse move)" ),
        ( L"Window: QSearch auto-focus (on file open)" )
    },
    /* rus */
    {
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A\x0020\x0418\x0441\x043A" \
          L"\x0430\x0442\x044C\x0020\x0022\x043D\x0430\x0020\x043B\x0435" \
          L"\x0442\x0443\x0022" ),
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A\x0020\x0412\x0441\x0435" \
          L"\x0433\x0434\x0430\x0020\x0441\x0020\x043D\x0430\x0447\x0430" \
          L"\x043B\x0430" ),
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A\x0020\x0418\x0441\x043F" \
          L"\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020" \
          L"\x0441\x043F\x0435\x0446\x002E\x0020\x0441\x0438\x043C\x0432" \
          L"\x043E\x043B\x044B ( \\n, \\t, * \x0438 ? )" ),
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A\x0020\x0418\x0441\x043F" \
          L"\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x0442\x044C\x0020" \
          L"\x0440\x0435\x0433\x0443\x043B\x044F\x0440\x043D\x044B\x0435" \
          L"\x0020\x0432\x044B\x0440\x0430\x0436\x0435\x043D\x0438\x044F" ),
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A  .  \x0437\x0430\x0445" \
          L"\x0432\x0430\x0442\x044B\x0432\x0430\x0435\x0442  \\n  \x0432" \
          L"\x0020\x0440\x0435\x0433\x002E\x0432\x044B\x0440\x0430\x0436" \
          L"\x0435\x043D\x0438\x044F\x0445" ),
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A\x0020\x041F\x043E\x0434" \
          L"\x0445\x0432\x0430\x0442\x044B\x0432\x0430\x0442\x044C\x0020" \
          L"\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x044B\x0439" \
          L"\x0020\x0442\x0435\x043A\x0441\x0442" ),
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A SelFind \x043F\x043E\x0434" \
          L"\x0445\x0432\x0430\x0442\x044B\x0432\x0430\x0435\x0442\x0020" \
          L"\x0432\x044B\x0434\x0435\x043B\x0435\x043D\x043D\x044B\x0439" \
          L"\x0020\x0442\x0435\x043A\x0441\x0442" ), 
        ( L"\x041F\x043E\x0438\x0441\x043A\x003A\x0020\x041E\x0441\x0442" \
          L"\x0430\x043D\x0430\x0432\x043B\x0438\x0432\x0430\x0442\x044C" \
          L"\x0441\x044F\x0020\x0432\x0020\x043A\x043E\x043D\x0446\x0435" \
          L"\x0020\x0444\x0430\x0439\x043B\x0430" ),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x0420\x0430\x0441" \
          L"\x043F\x043E\x043B\x043E\x0436\x0438\x0442\x044C\x0020\x0441" \
          L"\x0432\x0435\x0440\x0445\x0443" ),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x0424\x043E\x043A" \
          L"\x0443\x0441\x0020\x0432\x0432\x043E\x0434\x0430\x0020\x0432" \
          L"\x044B\x0434\x0435\x043B\x044F\x0435\x0442\x0020\x0432\x0441" \
          L"\x0451" ),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x041E\x0431\x0440" \
          L"\x0430\x0431\x0430\x0442\x044B\x0432\x0430\x0442\x044C" \
          L" F3 \x043E\x0442\x0020\x0433\x043B\x0430" \
          L"\x0432\x043D\x043E\x0433\x043E\x0020\x043E\x043A\x043D\x0430" ),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x041E\x0431\x0440" \
          L"\x0430\x0431\x0430\x0442\x044B\x0432\x0430\x0442\x044C" \
          L" Esc \x043E\x0442\x0020\x0433\x043B\x0430" \
          L"\x0432\x043D\x043E\x0433\x043E\x0020\x043E\x043A\x043D\x0430" ),  
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020 Hotkey \x0441\x043A\x0440" \
          L"\x044B\x0432\x0430\x0435\x0442\x0020\x043F\x0430\x043D\x0435" \
          L"\x043B\x044C" ),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x0410\x0432\x0442" \
          L"\x043E\x002D\x0444\x043E\x043A\x0443\x0441\x0020\x0432\x0020" \
          L"\x0440\x0435\x0434\x0430\x043A\x0442\x043E\x0440 " \
          L"(\x0434\x0432\x0438\x0436\x0435\x043D\x0438\x0435\x043C\x0020\x043C\x044B\x0448\x0438)" ),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x0410\x0432\x0442" \
          L"\x043E\x002D\x0444\x043E\x043A\x0443\x0441\x0020\x0432 QSearch " \
          L"(\x0434\x0432\x0438\x0436\x0435\x043D\x0438\x0435\x043C\x0020\x043C\x044B\x0448\x0438)"),
        ( L"\x041E\x043A\x043D\x043E\x003A\x0020\x0020\x0410\x0432\x0442" \
          L"\x043E\x002D\x0444\x043E\x043A\x0443\x0441\x0020\x0432 QSearch " \
          L"(\x043F\x0440\x0438\x0020\x043E\x0442\x043A\x0440\x002E\x0020\x0444\x0430\x0439\x043B\x0430)" )
    },
    /* ukr */
    {
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A\x0020\x0428\x0443\x043A" \
          L"\x0430\x0442\x0438\x0020\x043D\x0435\x0433\x0430\x0439\x043D" \
          L"\x043E" ),
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A\x0020\x0417\x0430\x0432" \
          L"\x0436\x0434\x0438\x0020\x0437\x0020\x043F\x043E\x0447\x0430" \
          L"\x0442\x043A\x0443" ),
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A\x0020\x0412\x0438\x043A" \
          L"\x043E\x0440\x0438\x0441\x0442\x002E\x0020\x0441\x043F\x0435" \
          L"\x0446\x002E\x0020\x0441\x0438\x043C\x0432\x043E\x043B\x0438" \
          L" ( \\n, \\t, * \x0442\x0430 ? )" ),
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A\x0020\x0412\x0438\x043A" \
          L"\x043E\x0440\x0438\x0441\x0442\x002E\x0020\x0440\x0435\x0433" \
          L"\x0443\x043B\x044F\x0440\x043D\x0456\x0020\x0432\x0438\x0440" \
          L"\x0430\x0437\x0438" ),
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A  .  \x0437\x0430\x0445" \
          L"\x043E\x043F\x043B\x044E\x0454  \\n  \x0432\x0020\x0440" \
          L"\x0435\x0433\x002E\x0432\x0438\x0440\x0430\x0437\x0430\x0445" ),
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A\x0020\x041F\x0069\x0434" \
          L"\x0445\x043E\x043F\x043B\x044E\x0432\x0430\x0442\x0438\x0020" \
          L"\x0432\x0438\x0434\x0069\x043B\x0435\x043D\x0438\x0439\x0020" \
          L"\x0442\x0435\x043A\x0441\x0442" ),
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A SelFind \x043F\x0069\x0434" \
          L"\x0445\x043E\x043F\x043B\x044E\x0454\x0020\x0432\x0438\x0434" \
          L"\x0069\x043B\x0435\x043D\x0438\x0439\x0020\x0442\x0435\x043A" \
          L"\x0441\x0442" ), 
        ( L"\x041F\x043E\x0448\x0443\x043A\x003A\x0020\x0417\x0443\x043F" \
          L"\x0438\x043D\x044F\x0442\x0438\x0441\x044C\x0020\x0443\x0020" \
          L"\x043A\x0069\x043D\x0446\x0069\x0020\x0444\x0430\x0439\x043B" \
          L"\x0430" ),
        ( L"\x0412\x0069\x043A\x043D\x043E\x003A\x0020\x0420\x043E\x0437" \
          L"\x0442\x0430\x0448\x0443\x0432\x0430\x0442\x0438\x0020\x0437" \
          L"\x0432\x0435\x0440\x0445\x0443" ),
        ( L"\x0412\x0069\x043A\x043D\x043E\x003A\x0020\x0424\x043E\x043A" \
          L"\x0443\x0441\x0020\x0432\x0432\x043E\x0434\x0443\x0020\x0432" \
          L"\x0438\x0434\x0069\x043B\x044F\x0454\x0020\x0432\x0441\x0435" ),
        ( L"\x0412\x0069\x043A\x043D\x043E\x003A\x0020\x041E\x0431\x0440" \
          L"\x043E\x0431\x043B\x044F\x0442\x0438 F3 " \
          L"\x0432\x0069\x0434\x0020\x0433\x043E\x043B\x043E\x0432" \
          L"\x043D\x043E\x0433\x043E\x0020\x0432\x0069\x043A\x043D\x0430" ),
        ( L"\x0412\x0069\x043A\x043D\x043E\x003A\x0020\x041E\x0431\x0440" \
          L"\x043E\x0431\x043B\x044F\x0442\x0438 Esc " \
          L"\x0432\x0069\x0434\x0020\x0433\x043E\x043B\x043E\x0432" \
          L"\x043D\x043E\x0433\x043E\x0020\x0432\x0069\x043A\x043D\x0430" ),  
        ( L"\x0412\x0069\x043A\x043D\x043E\x003A Hotkey \x0445\x043E\x0432" \
          L"\x0430\x0454\x0020\x043F\x0430\x043D\x0435\x043B\x044C" ),
        ( L"\x0412\x0456\x043A\x043D\x043E\x003A\x0020\x0410\x0432\x0442" \
          L"\x043E\x002D\x0444\x043E\x043A\x0443\x0441\x0020\x0432\x0020" \
          L"\x0440\x0435\x0434\x0430\x043A\x0442\x043E\x0440 " \
          L"(\x0440\x0443\x0445\x043E\x043C\x0020\x043C\x0438\x0448\x0456)" ),
        ( L"\x0412\x0456\x043A\x043D\x043E\x003A\x0020\x0410\x0432\x0442" \
          L"\x043E\x002D\x0444\x043E\x043A\x0443\x0441\x0020\x0432 QSearch " \
          L"(\x0440\x0443\x0445\x043E\x043C\x0020\x043C\x0438\x0448\x0456)" ),
        ( L"\x0412\x0456\x043A\x043D\x043E\x003A\x0020\x0410\x0432\x0442" \
          L"\x043E\x002D\x0444\x043E\x043A\x0443\x0441\x0020\x0432 QSearch " \
          L"(\x043F\x0440\x0438\x0020\x0432\x0456\x0434\x043A\x0440\x002E\x0020\x0444\x0430\x0439\x043B\x0443)" )
    }
};

const wchar_t* szFindAllPopupMenuW[INLNG_COUNT][QS_FINDALL_TOTAL] = {
    /* eng */
    {
        L"Auto Count All",
        L"Find All: Count Only",
        L"Find All: Log Output",
        L"Find All: File Output (N tabs)",
        L"Find All: File Output (1 tab)",
        L"Output: Whole line",
        L"Output: Match only",
        L"Output: Filter mode",
        L"Output options..."
    },
    /* rus */
    {
        L"\x0410\x0432\x0442\x043E\x002D\x043F\x043E\x0434\x0441\x0447\x0451\x0442\x0020\x0432\x0441\x0435\x0445\x0020\x0432\x0445\x043E\x0436\x0434\x0435\x043D\x0438\x0439",
        L"\x041D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0422\x043E\x043B\x044C\x043A\x043E\x0020\x043F\x043E\x0434\x0441\x0447\x0451\x0442",
        L"\x041D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0412\x044B\x0432\x043E\x0434\x0020\x0432 Log::Output",
        L"\x041D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0412\x044B\x0432\x043E\x0434\x0020\x0432\x0020\x0444\x0430\x0439\x043B (N \x0432\x043A\x043B\x0430\x0434\x043E\x043A)",
        L"\x041D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0412\x044B\x0432\x043E\x0434\x0020\x0432\x0020\x0444\x0430\x0439\x043B (1 \x0432\x043A\x043B\x0430\x0434\x043A\x0430)",
        L"\x0412\x044B\x0432\x043E\x0434: \x0412\x0441\x044F\x0020\x0441\x0442\x0440\x043E\x043A\x0430",
        L"\x0412\x044B\x0432\x043E\x0434: \x0422\x043E\x043B\x044C\x043A\x043E\x0020\x0441\x043E\x0432\x043F\x0430\x0434\x0435\x043D\x0438\x0435",
        L"\x0412\x044B\x0432\x043E\x0434: \x0420\x0435\x0436\x0438\x043C\x0020\x0444\x0438\x043B\x044C\x0442\x0440\x0430",
        L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0432\x044B\x0432\x043E\x0434\x0430..."
    },
    /* ukr */
    {
        L"\x0410\x0432\x0442\x043E\x002D\x043F\x0456\x0434\x0440\x0430\x0445\x0443\x043D\x043E\x043A\x0020\x0443\x0441\x0456\x0445\x0020\x0432\x0445\x043E\x0434\x0436\x0435\x043D\x044C",
        L"\x0417\x043D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0422\x0456\x043B\x044C\x043A\x0438\x0020\x043F\x0456\x0434\x0440\x0430\x0445\x0443\x043D\x043E\x043A",
        L"\x0417\x043D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0412\x0438\x0432\x0456\x0434\x0020\x0443 Log::Output",
        L"\x0417\x043D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0412\x0438\x0432\x0456\x0434\x0020\x0443\x0020\x0444\x0430\x0439\x043B (N \x0432\x043A\x043B\x0430\x0434\x043E\x043A)",
        L"\x0417\x043D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435: \x0412\x0438\x0432\x0456\x0434\x0020\x0443\x0020\x0444\x0430\x0439\x043B (1 \x0432\x043A\x043B\x0430\x0434\x043A\x0430)",
        L"\x0412\x0438\x0432\x0456\x0434: \x0423\x0432\x0435\x0441\x044C\x0020\x0440\x044F\x0434\x043E\x043A",
        L"\x0412\x0438\x0432\x0456\x0434: \x0422\x0456\x043B\x044C\x043A\x0438\x0020\x0441\x043F\x0456\x0432\x043F\x0430\x0434\x0456\x043D\x043D\x044F",
        L"\x0412\x0438\x0432\x0456\x0434: \x0420\x0435\x0436\x0438\x043C\x0020\x0444\x0456\x043B\x044C\x0442\x0440\x0430",
        L"\x041D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0432\x0438\x0432\x043E\x0434\x0443..."
    }
};


// extern vars
extern PluginState g_Plugin;


// funcs
static unsigned int getInternalLng(void)
{
    switch ( PRIMARYLANGID(g_Plugin.wLangSystem) )
    {
        case LANG_RUSSIAN:
            return INLNG_RUS;
        case LANG_UKRAINIAN:
            return INLNG_UKR;
        default:
            return INLNG_ENG;
    }
}

void qsearchSetMainDlgLang(HWND hDlg)
{
    unsigned int uInternalLng = getInternalLng();
    // no changes for English dialog
    if ( uInternalLng != INLNG_ENG )
    {
        if ( g_Plugin.bOldWindows )
        {
            SetWindowTextA( 
              GetDlgItem(hDlg, IDC_CH_MATCHCASE),
              szHintChMatchCaseA[uInternalLng][1] 
            );
            SetWindowTextA( 
              GetDlgItem(hDlg, IDC_CH_WHOLEWORD),
              szHintChWholeWordA[uInternalLng][1] 
            );
            SetWindowTextA( 
              GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL),
              szHintChHighlightAllA[uInternalLng][1] 
            );
        }
        else
        {
            SetWindowTextW( 
              GetDlgItem(hDlg, IDC_CH_MATCHCASE),
              szHintChMatchCaseW[uInternalLng][1] 
            );
            SetWindowTextW( 
              GetDlgItem(hDlg, IDC_CH_WHOLEWORD),
              szHintChWholeWordW[uInternalLng][1] 
            );
            SetWindowTextW( 
              GetDlgItem(hDlg, IDC_CH_HIGHLIGHTALL),
              szHintChHighlightAllW[uInternalLng][1] 
            );
        }
    }
}

void qsearchSetFindAllSettDlgLang(HWND hDlg)
{
    static const wchar_t* szFindAllSettTitleW[INLNG_COUNT] = {
        /* eng */
        L"QSearch - Find All - Output",
        /* rus */
        L"QSearch - \x041D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435 - \x0412\x044B\x0432\x043E\x0434",
        /* ukr */
        L"QSearch - \x0417\x043D\x0430\x0439\x0442\x0438\x0020\x0432\x0441\x0435 - \x0412\x0438\x0432\x0456\x0434"
    };

    static const wchar_t* szFindAllOutputHeaderW[INLNG_COUNT] = {
        /* eng */
        L"Header: Searching for \"%s\" ...",
        /* rus */
        L"\x0417\x0430\x0433\x043E\x043B\x043E\x0432\x043E\x043A: \x0418\x0449\x0435\x043C \"%s\" ...",
        /* ukr */
        L"\x0417\x0430\x0433\x043E\x043B\x043E\x0432\x043E\x043A: \x0428\x0443\x043A\x0430\x0454\x043C\x043E \"%s\" ..."
    };

    static const wchar_t* szFindAllOutputPositionW[INLNG_COUNT] = {
        /* eng */
        L"Position: (Ln:Pos)",
        /* rus */
        L"\x041F\x043E\x0437\x0438\x0446\x0438\x044F: (Ln:Pos)",
        /* ukr */
        L"\x041F\x043E\x0437\x0438\x0446\x0456\x044F: (Ln:Pos)"
    };

    static const wchar_t* szFindAllOutputLengthW[INLNG_COUNT] = {
        /* eng */
        L"Length: (Len)",
        /* rus */
        L"\x0414\x043B\x0438\x043D\x0430: (Len)",
        /* ukr */
        L"\x0414\x043E\x0432\x0436\x0438\x043D\x0430: (Len)"
    };

    static const wchar_t* szFindAllOutputFooterW[INLNG_COUNT] = {
        /* eng */
        L"Footer: %d found.",
        /* rus */
        L"\x0418\x0442\x043E\x0433: %d \x043D\x0430\x0439\x0434\x0435\x043D\x043E.",
        /* ukr */
        L"\x041F\x0456\x0434\x0441\x0443\x043C\x043E\x043A: %d \x0437\x043D\x0430\x0439\x0434\x0435\x043D\x043E."
    };

    static const wchar_t* szFindAllOutputWholeLineW[INLNG_COUNT] = {
        /* eng */
        L"Whole line",
        /* rus */
        L"\x0412\x0441\x044F\x0020\x0441\x0442\x0440\x043E\x043A\x0430",
        /* ukr */
        L"\x0423\x0432\x0435\x0441\x044C\x0020\x0440\x044F\x0434\x043E\x043A"
    };

    static const wchar_t* szFindAllOutputMatchOnlyW[INLNG_COUNT] = {
        /* eng */
        L"Match only",
        /* rus */
        L"\x0422\x043E\x043B\x044C\x043A\x043E\x0020\x0441\x043E\x0432\x043F\x0430\x0434\x0435\x043D\x0438\x0435",
        /* ukr */
        L"\x0422\x0456\x043B\x044C\x043A\x0438\x0020\x0441\x043F\x0456\x0432\x043F\x0430\x0434\x0456\x043D\x043D\x044F"
    };

    static const wchar_t* szFindAllOutputFilterModeW[INLNG_COUNT] = {
        /* eng */
        L"Filter mode",
        /* rus */
        L"\x0420\x0435\x0436\x0438\x043C\x0020\x0444\x0438\x043B\x044C\x0442\x0440\x0430",
        /* ukr */
        L"\x0420\x0435\x0436\x0438\x043C\x0020\x0444\x0456\x043B\x044C\x0442\x0440\x0430"
    };

    static const wchar_t* szFindAllOutputSyntaxThemeW[INLNG_COUNT] = {
        /* eng */
        L"Apply the syntax theme",
        /* rus */
        L"\x041F\x0440\x0438\x043C\x0435\x043D\x0438\x0442\x044C\x0020\x0441\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0447\x0435\x0441\x043A\x0443\x044E\x0020\x0442\x0435\x043C\x0443",
        /* ukr */
        L"\x0417\x0430\x0441\x0442\x043E\x0441\x0443\x0432\x0430\x0442\x0438\x0020\x0441\x0438\x043D\x0442\x0430\x043A\x0441\x0438\x0447\x043D\x0443\x0020\x0442\x0435\x043C\x0443"
    };

    static const wchar_t* szFindAllOutputExampleW[INLNG_COUNT] = {
        /* eng */
        L"Example:",
        /* rus */
        L"\x041F\x0440\x0438\x043C\x0435\x0440:",
        /* ukr */
        L"\x041F\x0440\x0438\x043A\x043B\x0430\x0434:"
    };

    unsigned int uInternalLng;

    uInternalLng = getInternalLng();
    SetWindowTextW( hDlg, szFindAllSettTitleW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_CH_FA_HEADER),      szFindAllOutputHeaderW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_CH_FA_POSITION),    szFindAllOutputPositionW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_CH_FA_LENGTH),      szFindAllOutputLengthW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_RB_FA_WHOLELINE),   szFindAllOutputWholeLineW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_RB_FA_MATCHONLY),   szFindAllOutputMatchOnlyW[uInternalLng] );
    // SetWindowTextW( GetDlgItem(hDlg, IDC_CH_FA_FOOTER),      szFindAllOutputFooterW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_CH_FA_FILTERMODE),  szFindAllOutputFilterModeW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_CH_FA_SYNTAXTHEME), szFindAllOutputSyntaxThemeW[uInternalLng] );
    SetWindowTextW( GetDlgItem(hDlg, IDC_ST_FA_EXAMPLE),     szFindAllOutputExampleW[uInternalLng] );
}

void qsearchSetPopupMenuLang(HMENU hPopupMenu)
{
    unsigned int uInternalLng = getInternalLng();
    // no changes for English menu
    if ( uInternalLng != INLNG_ENG )
    {
        if ( g_Plugin.bOldWindows )
        {
            MENUITEMINFOA miiA;
            unsigned int  i;

            x_zero_mem(&miiA, sizeof(MENUITEMINFOA));

            for ( i = 0; i < OPTF_COUNT - 1; i++ )
            {
                miiA.cbSize = sizeof(MENUITEMINFOA);
                miiA.fMask = MIIM_TYPE;
                miiA.fType = MFT_STRING;
                miiA.dwTypeData = (LPSTR) szPopupMenuA[uInternalLng][i];
                miiA.cch = 0;
                SetMenuItemInfoA( hPopupMenu, IDM_START + i, FALSE, &miiA );
            }
        }
        else
        {
            MENUITEMINFOW miiW;
            unsigned int  i;

            x_zero_mem(&miiW, sizeof(MENUITEMINFOW));

            for ( i = 0; i < OPTF_COUNT - 1; i++ )
            {
                miiW.cbSize = sizeof(MENUITEMINFOW);
                #ifdef MIIM_FTYPE
                    miiW.fMask = MIIM_FTYPE | MIIM_STRING;
                #else
                    miiW.fMask = MIIM_TYPE;
                #endif
                miiW.fType = MFT_STRING;
                miiW.dwTypeData = (LPWSTR) szPopupMenuW[uInternalLng][i];
                miiW.cch = 0;
                SetMenuItemInfoW( hPopupMenu, IDM_START + i, FALSE, &miiW );
            }
        }
    }
}

void qsearchSetFindAllPopupMenuLang(HMENU hFindAllPopupMenu)
{
    unsigned int uInternalLng = getInternalLng();
    // no changes for English menu
    if ( uInternalLng != INLNG_ENG )
    {
        if ( g_Plugin.bOldWindows )
        {
            MENUITEMINFOA miiA;
            unsigned int  i;

            x_zero_mem(&miiA, sizeof(MENUITEMINFOA));

            for ( i = 0; i < QS_FINDALL_TOTAL; i++ )
            {
                miiA.cbSize = sizeof(MENUITEMINFOA);
                miiA.fMask = MIIM_TYPE;
                miiA.fType = MFT_STRING;
                miiA.dwTypeData = (LPSTR) szFindAllPopupMenuA[uInternalLng][i];
                miiA.cch = 0;
                SetMenuItemInfoA( hFindAllPopupMenu, IDM_FINDALL_START + i, FALSE, &miiA );
            }
        }
        else
        {
            MENUITEMINFOW miiW;
            unsigned int  i;

            x_zero_mem(&miiW, sizeof(MENUITEMINFOW));

            for ( i = 0; i < QS_FINDALL_TOTAL; i++ )
            {
                miiW.cbSize = sizeof(MENUITEMINFOW);
                #ifdef MIIM_FTYPE
                    miiW.fMask = MIIM_FTYPE | MIIM_STRING;
                #else
                    miiW.fMask = MIIM_TYPE;
                #endif
                miiW.fType = MFT_STRING;
                miiW.dwTypeData = (LPWSTR) szFindAllPopupMenuW[uInternalLng][i];
                miiW.cch = 0;
                SetMenuItemInfoW( hFindAllPopupMenu, IDM_FINDALL_START + i, FALSE, &miiW );
            }
        }
    }
}

const char* qsearchGetHintA(unsigned int uDlgItemID)
{
    unsigned int uInternalLng = getInternalLng();

    switch ( uDlgItemID )
    {
        case IDC_BT_CANCEL:
            return szHintBtCancelA[uInternalLng];
        case IDC_BT_FINDNEXT:
            return szHintBtFindNextA[uInternalLng];
        case IDC_BT_FINDPREV:
            return szHintBtFindPrevA[uInternalLng];
        case IDC_BT_FINDALL:
            return szHintBtFindAllA[uInternalLng];
        case IDC_CH_MATCHCASE:
            return szHintChMatchCaseA[uInternalLng][0];
        case IDC_CH_WHOLEWORD:
            return szHintChWholeWordA[uInternalLng][0];
        case IDC_CH_HIGHLIGHTALL:
            return szHintChHighlightAllA[uInternalLng][0];
        case IDC_ED_FINDTEXT:
            return szHintEdTextA[uInternalLng];
        case IDS_EOFREACHED:
            return szHintStrEOFReachedA[uInternalLng];
        default:
            return "";
    }
}

const char* qsearchGetTextA(unsigned int uDlgItemID)
{
    unsigned int uInternalLng = getInternalLng();

    switch ( uDlgItemID )
    {
        case IDC_CH_MATCHCASE:
            return szHintChMatchCaseA[uInternalLng][1];
        case IDC_CH_WHOLEWORD:
            return szHintChWholeWordA[uInternalLng][1];
        case IDC_CH_HIGHLIGHTALL:
            return szHintChHighlightAllA[uInternalLng][1];
    }

    return qsearchGetHintA(uDlgItemID);
}

const wchar_t* qsearchGetHintW(unsigned int uDlgItemID)
{
    unsigned int uInternalLng = getInternalLng();

    switch ( uDlgItemID )
    {
        case IDC_BT_CANCEL:
            return szHintBtCancelW[uInternalLng];
        case IDC_BT_FINDNEXT:
            return szHintBtFindNextW[uInternalLng];
        case IDC_BT_FINDPREV:
            return szHintBtFindPrevW[uInternalLng];
        case IDC_BT_FINDALL:
            return szHintBtFindAllW[uInternalLng];
        case IDC_CH_MATCHCASE:
            return szHintChMatchCaseW[uInternalLng][0];
        case IDC_CH_WHOLEWORD:
            return szHintChWholeWordW[uInternalLng][0];
        case IDC_CH_HIGHLIGHTALL:
            return szHintChHighlightAllW[uInternalLng][0];
        case IDC_ED_FINDTEXT:
            return szHintEdTextW[uInternalLng];
        case IDS_EOFREACHED:
            return szHintStrEOFReachedW[uInternalLng];
        default:
            return L"";
    }
}

const wchar_t* qsearchGetTextW(unsigned int uDlgItemID)
{
    unsigned int uInternalLng = getInternalLng();

    switch ( uDlgItemID )
    {
        case IDC_CH_MATCHCASE:
            return szHintChMatchCaseW[uInternalLng][1];
        case IDC_CH_WHOLEWORD:
            return szHintChWholeWordW[uInternalLng][1];
        case IDC_CH_HIGHLIGHTALL:
            return szHintChHighlightAllW[uInternalLng][1];
    }

    return qsearchGetHintW(uDlgItemID);
}

const wchar_t* qsearchGetStringW(unsigned int uStringID)
{
    unsigned int uInternalLng = getInternalLng();

    switch (uStringID)
    {
        case QS_STRID_FINDALL_SEARCHINGFOR:
        {
            static const wchar_t* szFindAllSearchingForW[INLNG_COUNT] = {
                /* eng */
                L"Searching for %c%s%c in file '%s' - %u matches",
                /* rus */
                L"\x0418\x0449\x0435\x043C %c%s%c \x0432\x0020\x0444\x0430\x0439\x043B\x0435 '%s' - %u \x043D\x0430\x0439\x0434\x0435\x043D\x043E",
                /* ukr */
                L"\x0428\x0443\x043A\x0430\x0454\x043C\x043E %c%s%c \x0443\x0020\x0444\x0430\x0439\x043B\x0456 '%s' - %u \x0437\x043D\x0430\x0439\x0434\x0435\x043D\x043E"
            };
            return szFindAllSearchingForW[uInternalLng];
        }

        case QS_STRID_FINDALL_OCCURRENCESFOUND:
        {
            static const wchar_t* szFindAllOccurrencesFoundW[INLNG_COUNT] = {
                /* eng */
                L"%u matches.",
                /* rus */
                L"%u \x043D\x0430\x0439\x0434\x0435\x043D\x043E.",
                /* ukr */
                L"%u \x0437\x043D\x0430\x0439\x0434\x0435\x043D\x043E."
            };
            return szFindAllOccurrencesFoundW[uInternalLng];
        }

        case QS_STRID_FINDALL_SEARCHINGFORINFILES:
        {
            static const wchar_t* szFindAllSearchingForInFilesW[INLNG_COUNT] = {
                /* eng */
                L"Searching for %c%s%c in %u files ...",
                /* rus */
                L"\x0418\x0449\x0435\x043C %c%s%c \x0432 %u \x0444\x0430\x0439\x043B\x0430\x0445 ...",
                /* ukr */
                L"\x0428\x0443\x043A\x0430\x0454\x043C\x043E %c%s%c \x0443 %u \x0444\x0430\x0439\x043B\x0430\x0445 ..."
            };
            return szFindAllSearchingForInFilesW[uInternalLng];
        }

        case QS_STRID_FINDALL_OCCURRENCESFOUNDINFILES:
        {
            static const wchar_t* szFindAllOccurrencesFoundInFilesW[INLNG_COUNT] = {
                /* eng */
                L"Total found: %u in %d files.",
                /* rus */
                L"\x0412\x0441\x0435\x0433\x043E\x0020\x043D\x0430\x0439\x0434\x0435\x043D\x043E: %u \x0432 %d \x0444\x0430\x0439\x043B\x0430\x0445.",
                /* ukr */
                L"\x0423\x0441\x044C\x043E\x0433\x043E\x0020\x0437\x043D\x0430\x0439\x0434\x0435\x043D\x043E: %u \x0443 %d \x0444\x0430\x0439\x043B\x0430\x0445."
            };
            return szFindAllOccurrencesFoundInFilesW[uInternalLng];
        }

    };

    return L"";
}

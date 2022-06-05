#include "QSearchSettDlg.h"
#include "QSearch.h"
#include "QSearchLng.h"
#include "XMemStrFunc.h"


extern PluginState  g_Plugin;
extern QSearchOpt   g_Options;


//////////////// Helpers... ////////////////

static BOOL AnyWindow_CenterWindow(HWND hWnd, HWND hParentWnd, BOOL bRepaint)
{
    RECT rectParent;
    RECT rect;
    INT  height, width;
    INT  x, y;

    GetWindowRect(hParentWnd, &rectParent);
    GetWindowRect(hWnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
    x = ((rectParent.right - rectParent.left) - width) / 2;
    x += rectParent.left;
    y = ((rectParent.bottom - rectParent.top) - height) / 2;
    y += rectParent.top;
    return MoveWindow(hWnd, x, y, width, height, bRepaint);
}

static BOOL CheckBox_IsChecked(HWND hDlg, UINT idCheckBox)
{
    HWND hCheckBox = GetDlgItem(hDlg, idCheckBox);
    if ( hCheckBox )
    {
        UINT uState = (UINT) SendMessage(hCheckBox, BM_GETCHECK, 0, 0);
        if ( uState == BST_CHECKED || uState == BST_INDETERMINATE )
            return TRUE;
    }
    return FALSE;
}

static BOOL CheckBox_SetCheck(HWND hDlg, UINT idCheckBox, BOOL bChecked)
{
    HWND hCheckBox = GetDlgItem(hDlg, idCheckBox);
    if ( hCheckBox )
    {
        SendMessage( hCheckBox, BM_SETCHECK, (bChecked ? BST_CHECKED : BST_UNCHECKED), 0 );
        return TRUE;
    }
    return FALSE;
}

//////////////// FndAllSettDlg... ////////////////

typedef struct sCheckBoxOptFlagItem {
    int id;
    unsigned int optf;
} tCheckBoxOptFlagItem;

static const tCheckBoxOptFlagItem arrCheckBoxOptions[] = {
    { IDC_CH_FA_HEADER,      QS_FINDALL_RSLT_SEARCHING  },
    { IDC_CH_FA_POSITION,    QS_FINDALL_RSLT_POS        },
    { IDC_CH_FA_LENGTH,      QS_FINDALL_RSLT_LEN        },
    { IDC_CH_FA_FOOTER,      QS_FINDALL_RSLT_OCCFOUND   },
    { IDC_CH_FA_FILTERMODE,  QS_FINDALL_RSLT_FILTERMODE },
    { IDC_CH_FA_SYNTAXTHEME, QS_FINDALL_RSLT_CODERALIAS },
    { 0,                     0 } // trailing "empty" item
};

static void FndAllSettDlg_OnCheckBoxClicked(HWND hDlg);
static BOOL FndAllSettDlg_OnOK(HWND hDlg);
static void FndAllSettDlg_OnInitDialog(HWND hDlg);
static void FndAllSettDlg_EndDialog(HWND hDlg, INT_PTR nResult);

INT_PTR CALLBACK QSFndAllSettDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( uMsg == WM_COMMAND )
    {
        switch ( LOWORD(wParam) )
        {
            case IDC_BT_OK:
            case IDOK:
            {
                if ( FndAllSettDlg_OnOK(hDlg) )
                {
                    FndAllSettDlg_EndDialog(hDlg, 1); // OK - returns 1
                }
                return 1;
            }

            case IDC_BT_CANCEL:
            case IDCANCEL:
            {
                FndAllSettDlg_EndDialog(hDlg, 0); // Cancel - returns 0
                return 1;
            }

            case IDC_CH_FA_HEADER:
            case IDC_CH_FA_POSITION:
            case IDC_CH_FA_LENGTH:
            case IDC_CH_FA_FOOTER:
            case IDC_RB_FA_WHOLELINE:
            case IDC_RB_FA_MATCHONLY:
            case IDC_CH_FA_FILTERMODE:
            {
                if ( HIWORD(wParam) == BN_CLICKED )
                {
                    FndAllSettDlg_OnCheckBoxClicked(hDlg);
                }
                break;
            }

            default:
                break;
        }
    }
    else if ( uMsg == WM_INITDIALOG )
    {
        FndAllSettDlg_OnInitDialog(hDlg);
    }

    return 0;
}

static DWORD getFindAllResultFlags(HWND hDlg)
{
    unsigned int i;
    DWORD dwFindAllResultFlags;

    dwFindAllResultFlags = 0;

    for ( i = 0; ; ++i )
    {
        if ( arrCheckBoxOptions[i].id == 0 && arrCheckBoxOptions[i].optf == 0 )
            break;

        if ( CheckBox_IsChecked(hDlg, arrCheckBoxOptions[i].id) )
            dwFindAllResultFlags |= arrCheckBoxOptions[i].optf;
    }

    if ( CheckBox_IsChecked(hDlg, IDC_RB_FA_MATCHONLY) )
        dwFindAllResultFlags |= QS_FINDALL_RSLT_MATCHONLY;
    else
        dwFindAllResultFlags |= QS_FINDALL_RSLT_WHOLELINE;

    return dwFindAllResultFlags;
}

void FndAllSettDlg_OnCheckBoxClicked(HWND hDlg)
{
    HWND hStExampleData;
    const wchar_t* cszTextFormat;
    int nLen;
    DWORD dwFindAllResultFlags;
    tDynamicBuffer infoBuf;
    wchar_t szText[128];
    wchar_t szMatch1[64];
    wchar_t szMatch2[64];

    tDynamicBuffer_Init(&infoBuf);
    tDynamicBuffer_Allocate(&infoBuf, 256*sizeof(wchar_t));

    szMatch1[0] = 0;
    szMatch2[0] = 0;

    dwFindAllResultFlags = getFindAllResultFlags(hDlg);
    if ( (dwFindAllResultFlags & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        if ( dwFindAllResultFlags & QS_FINDALL_RSLT_SEARCHING )
        {
            cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_SEARCHINGFOR);
            nLen = wsprintfW(szText, cszTextFormat, L'/', L"w[a-z]+d", L'/', L"Example.txt", 2);
            tDynamicBuffer_Append(&infoBuf, szText, nLen*sizeof(wchar_t));
            tDynamicBuffer_Append(&infoBuf, L"\n", 1*sizeof(wchar_t));
            lstrcatW(szMatch1, L"   ");
            lstrcatW(szMatch2, L"   ");
        }
        if ( dwFindAllResultFlags & QS_FINDALL_RSLT_POS )
        {
            lstrcatW(szMatch1, L"(1:3)");
            lstrcatW(szMatch2, L"(1:12)");
        }
        if ( dwFindAllResultFlags & QS_FINDALL_RSLT_LEN )
        {
            lstrcatW(szMatch1, L"(5)");
            lstrcatW(szMatch2, L"(4)");
        }
    }

    {
        if ( szMatch1[0] != 0 )
            lstrcatW(szMatch1, L"\t");
        if ( dwFindAllResultFlags & QS_FINDALL_RSLT_MATCHONLY )
            lstrcatW(szMatch1, L"world\n");
        else
            lstrcatW(szMatch1, L"A world of words.\n");

        if ( szMatch2[0] != 0 )
            lstrcatW(szMatch2, L"\t");
        if ( dwFindAllResultFlags & QS_FINDALL_RSLT_MATCHONLY )
            lstrcatW(szMatch2, L"word\n");
        else
            lstrcatW(szMatch2, L"A world of words.\n");

        tDynamicBuffer_Append(&infoBuf, szMatch1, lstrlenW(szMatch1)*sizeof(wchar_t));
        tDynamicBuffer_Append(&infoBuf, szMatch2, lstrlenW(szMatch2)*sizeof(wchar_t));
    }

    if ( (dwFindAllResultFlags & QS_FINDALL_RSLT_FILTERMODE) == 0 )
    {
        if ( dwFindAllResultFlags & QS_FINDALL_RSLT_OCCFOUND )
        {
            cszTextFormat = qsearchGetStringW(QS_STRID_FINDALL_OCCURRENCESFOUND);
            nLen = wsprintfW(szText, cszTextFormat, 2); // "2 found."
            tDynamicBuffer_Append(&infoBuf, szText, nLen*sizeof(wchar_t));
        }
    }

    tDynamicBuffer_Append(&infoBuf, L"\0", 1*sizeof(wchar_t));

    hStExampleData = GetDlgItem(hDlg, IDC_ST_FA_EXAMPLE_DATA);
    SetWindowTextW(hStExampleData, (const wchar_t *) infoBuf.ptr);
}

BOOL FndAllSettDlg_OnOK(HWND hDlg)
{
    g_Options.dwFindAllResult = getFindAllResultFlags(hDlg);

    return TRUE;
}

void FndAllSettDlg_OnInitDialog(HWND hDlg)
{
    unsigned int i;
    BOOL bChecked;

    qsearchSetFindAllSettDlgLang(hDlg);

    AnyWindow_CenterWindow(hDlg, g_Plugin.hMainWnd, FALSE);

    for ( i = 0; ; ++i )
    {
        if ( arrCheckBoxOptions[i].id == 0 && arrCheckBoxOptions[i].optf == 0 )
            break;

        bChecked = (g_Options.dwFindAllResult & arrCheckBoxOptions[i].optf) ? TRUE : FALSE;
        CheckBox_SetCheck(hDlg, arrCheckBoxOptions[i].id, bChecked);
    }

    if ( g_Options.dwFindAllResult & QS_FINDALL_RSLT_MATCHONLY )
        CheckBox_SetCheck(hDlg, IDC_RB_FA_MATCHONLY, TRUE);
    else
        CheckBox_SetCheck(hDlg, IDC_RB_FA_WHOLELINE, TRUE);

    FndAllSettDlg_OnCheckBoxClicked(hDlg);
}

void FndAllSettDlg_EndDialog(HWND hDlg, INT_PTR nResult)
{
    EndDialog(hDlg, nResult);
}

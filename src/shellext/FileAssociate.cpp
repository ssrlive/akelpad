#pragma warning(disable: 4530) 

#include "stdafx.h"

#include <atlbase.h> 
#include <vector> 
#include <string> 
#include "FileAssociate.h" 

#pragma comment(lib, "shlwapi") 

#ifndef _tstring 
typedef std::basic_string<TCHAR> _tstring; 
#endif  // _tstring 

#ifndef _countof 
#define _countof(_Array) (sizeof(_Array) / sizeof((_Array)[0])) 
#endif 

BOOL IsWinXpOrLater(); 
void ExtractEnvStrings(TCHAR inBuf[], UINT cchMax, OUT std::vector<_tstring> & vecPaths); 
BOOL _DeleteEntireKey(HKEY hRootKey, LPTSTR pszSubKey); 


// // HKEY_LOCAL_MACHINE 
// 
// TCHAR * arrHklmWin2kAssocStuf[] = { 
//     _T("SOFTWARE\\Classes\\%s"),                    // @="APPNAME" 
//     _T("SOFTWARE\\Classes\\%s\\DefaultIcon"),       // @="C:\\Program Files\\APPNAME\\APPNAME.exe,-5000" 
//     _T("SOFTWARE\\Classes\\%s\\shell\\open\\command"),  // @="C:\\PROGRA~1\\APPNAME\\APPNAME.EXE \"%1\"" 
// }; 
// 
// // HKEY_CLASSES_ROOT 
// 
// TCHAR * arrHkcrWin2kAssocStuf[] = { 
//     _T("%s"),                                       // @="APPNAME" 
//     _T("%s\\DefaultIcon"),                          // @="C:\\Program Files\\APPNAME\\APPNAME.exe,-5000" 
//     _T("%s\\shell\\open\\command"),                 // @="C:\\PROGRA~1\\EDITPL~1\\APPNAME.EXE \"%1\"" 
// }; 
// 
// BOOL RegisterOpenWith_Win2K(HMODULE hMyselfModule, LPCTSTR lpszAppExeName); 
// BOOL UnregisterOpenWith_Win2K(LPCTSTR lpszAppExeName); 

// HKEY_CLASSES_ROOT 

TCHAR * arrFileAssocKey[] = { 
    _T("SystemFileAssociations\\text\\OpenWithList\\%s"), 
    _T("applications\\%s"), 
    _T("applications\\%s\\shell"), 
    _T("applications\\%s\\shell\\edit"), 
    _T("applications\\%s\\shell\\open"), 
    _T("applications\\%s\\shell\\edit\\command"), 
    _T("applications\\%s\\shell\\open\\command"), 
}; 

TCHAR szCmdFmt[] = _T("%s \"%1\""); 

BOOL RegisterOpenWith(HMODULE hMyselfModule, LPCTSTR lpszAppExeName) 
{ 
    TCHAR szTemp[MAX_PATH*2] = { 0 }; 
    TCHAR szAppCmd[MAX_PATH+32] = { 0 }; 

    if (false == FindExeFile(hMyselfModule, lpszAppExeName, szAppCmd, _countof(szAppCmd))) 
    { 
        return FALSE; 
    } 

    PathQuoteSpaces( szAppCmd ); 
    lstrcat(szAppCmd, _T(" \"%1\"")); 

    CRegKey key; 
    for (int i=0; i<_countof(arrFileAssocKey); i++) 
    { 
        wsprintf(szTemp, arrFileAssocKey[i], lpszAppExeName); 
        key.Create(HKEY_CLASSES_ROOT, szTemp); 

        if(i>=_countof(arrFileAssocKey)-2) 
        { 
#pragma warning(disable: 4996) 
            key.SetValue(szAppCmd, NULL); 
#pragma warning(default: 4996) 
        } 
    } 

    //RegisterOpenWith_Win2K(hMyselfModule, lpszAppExeName); 
    
    return TRUE; 
} 

// BOOL RegisterOpenWith_Win2K(HMODULE hMyselfModule, LPCTSTR lpszAppExeName) 
// { 
//     BOOL result = FALSE; 
// 
//     TCHAR szTemp[MAX_PATH*2] = { 0 }; 
//     TCHAR szFullPath[MAX_PATH+32] = { 0 }; 
// 
//     if (false == FindExeFile(hMyselfModule, lpszAppExeName, szFullPath, _countof(szFullPath))) 
//     { 
//         return FALSE; 
//     } 
// 
//     PathQuoteSpaces( szFullPath ); 
// 
//     TCHAR szCmd0[MAX_PATH*2] = { 0 }; 
//     TCHAR szCmd1[MAX_PATH*2] = { 0 }; 
//     TCHAR szCmd2[MAX_PATH*2] = { 0 }; 
// 
//     lstrcpyn(szCmd0, lpszAppExeName, _countof(szCmd0)); 
// 
//     lstrcpyn(szCmd1, szFullPath, _countof(szCmd1)); 
//     lstrcat(szCmd1, _T(", 0")); 
// 
//     lstrcpyn(szCmd2, szFullPath, _countof(szCmd2)); 
//     lstrcat(szCmd2, _T(" \"%1\"")); 
// 
//     CRegKey key; 
// 
//     ATLASSERT(_countof(arrHklmWin2kAssocStuf) == 3); 
// 
//     for (int i=0; i<_countof(arrHklmWin2kAssocStuf); i++) 
//     { 
//         wsprintf(szTemp, arrHklmWin2kAssocStuf[i], lpszAppExeName); 
//         key.Create(HKEY_LOCAL_MACHINE, szTemp); 
// 
// #pragma warning(disable: 4996) 
//         switch(i) 
//         { 
//         case 0: 
//             key.SetValue(szCmd0, NULL); 
//             break; 
//         case 1: 
//             key.SetValue(szCmd1, NULL); 
//             break; 
//         case 2: 
//             key.SetValue(szCmd2, NULL); 
//             break; 
//         } 
// #pragma warning(default: 4996) 
//     } 
// 
//     ATLASSERT(_countof(arrHkcrWin2kAssocStuf) == 3); 
// 
//     for (int i=0; i<_countof(arrHkcrWin2kAssocStuf); i++) 
//     { 
//         wsprintf(szTemp, arrHkcrWin2kAssocStuf[i], lpszAppExeName); 
//         key.Create(HKEY_LOCAL_MACHINE, szTemp); 
// 
// #pragma warning(disable: 4996) 
//         switch(i) 
//         { 
//         case 0: 
//             key.SetValue(szCmd0, NULL); 
//             break; 
//         case 1: 
//             key.SetValue(szCmd1, NULL); 
//             break; 
//         case 2: 
//             key.SetValue(szCmd2, NULL); 
//             break; 
//         } 
// #pragma warning(default: 4996) 
//     } 
// 
//     return TRUE; 
// } 


BOOL UnregisterOpenWith(LPCTSTR lpszAppExeName) 
{ 
    BOOL result = FALSE; 
    TCHAR szTemp[MAX_PATH*2] = { 0 }; 

    CRegKey key; 
    for (int i=0; i<2; i++) 
    { 
        wsprintf(szTemp, arrFileAssocKey[i], lpszAppExeName); 
        result = _DeleteEntireKey(HKEY_CLASSES_ROOT, szTemp); 
        if (FALSE == result) 
            break; 
    } 

    // UnregisterOpenWith_Win2K(lpszAppExeName); 
    
    return result; 
} 

// BOOL UnregisterOpenWith_Win2K(LPCTSTR lpszAppExeName) 
// { 
//     BOOL result = FALSE; 
//     TCHAR szTemp[MAX_PATH*2] = { 0 }; 
// 
//     CRegKey key; 
// 
//     wnsprintf(szTemp, _countof(szTemp), arrHklmWin2kAssocStuf[0], lpszAppExeName); 
//     result = _DeleteEntireKey(HKEY_LOCAL_MACHINE, szTemp); 
// 
//     wnsprintf(szTemp, _countof(szTemp), arrHkcrWin2kAssocStuf[0], lpszAppExeName); 
//     result = _DeleteEntireKey(HKEY_LOCAL_MACHINE, szTemp); 
// 
//     return result; 
// } 

BOOL IsWinXpOrLater() 
{ 
    OSVERSIONINFO stVerInfo = { 0 }; 
    stVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); 
    GetVersionEx( & stVerInfo ); 

    return (stVerInfo.dwMajorVersion > 4 && stVerInfo.dwMinorVersion > 0); 
} 

BOOL FindExeFile(HMODULE hMyselfModule, LPCTSTR lpszWhat, OUT TCHAR szFullExeFile[], UINT cchMax) 
{ 
#define EVN_PATH_PRIFIX     _T("PATH = ") 
#define EVN_PATH            EVN_PATH_PRIFIX _T("%PATH%") 
#define INFO_BUFFER_SIZE    32767 
    
    TCHAR  infoBuf[INFO_BUFFER_SIZE]; 
    DWORD  bufCharCount = _countof(infoBuf); 
    
    TCHAR szModuleFileName[MAX_PATH]; 
    
    ZeroMemory(szFullExeFile, cchMax*sizeof(TCHAR)); 
    
    TCHAR tchModule[MAX_PATH] = { 0 }; 
    TCHAR tch1[MAX_PATH] = { 0 }; 
    
    GetModuleFileName(hMyselfModule, szModuleFileName, _countof(szModuleFileName)); 
    lstrcpyn(tchModule, szModuleFileName, _countof(tchModule)); 
    PathRemoveFileSpec(tchModule); 
    PathAddBackslash(tchModule); 
    PathAppend(tchModule, lpszWhat); 
    
    lstrcpy(tch1, tchModule); 
    if (PathFileExists(tch1)) { 
        lstrcpyn(szFullExeFile, tch1, cchMax); 
        return TRUE; 
    } 
    
    bufCharCount = ExpandEnvironmentStrings(EVN_PATH, infoBuf, _countof(infoBuf)); 
    
    if( bufCharCount > _countof(infoBuf) ) { 
        ATLTRACE( _T("\n\t(Buffer too small to expand: \"%s\")\n"), EVN_PATH ); 
        return FALSE; 
    } else if ( !bufCharCount ) { 
        ATLTRACE( _T("ExpandEnvironmentStrings\n") ); 
        return FALSE; 
    } 
    else 
    { 
        std::vector<_tstring> vecPaths; 
        int nJump = lstrlen(EVN_PATH_PRIFIX); 
        ExtractEnvStrings(infoBuf+nJump, _countof(infoBuf)-nJump, vecPaths); 
        
        std::vector<_tstring>::iterator it; 
        for (it=vecPaths.begin(); it!=vecPaths.end(); it++) 
        { 
            lstrcpyn(tch1, (*it).c_str(), _countof(tch1)); 
            PathAddBackslash(tch1); 
            PathAppend(tch1, lpszWhat); 
            if (PathFileExists(tch1)) { 
                lstrcpyn(szFullExeFile, tch1, cchMax); 
                return TRUE; 
            } 
        } 
    } 
    return FALSE; 
}

void ExtractEnvStrings(TCHAR inBuf[], UINT cchMax, OUT std::vector<_tstring> & vecPaths) 
{ 
    TCHAR * infoBuf = (TCHAR *) LocalAlloc( LPTR, (1+cchMax)*sizeof(TCHAR)); 
    memcpy(infoBuf, inBuf, cchMax*sizeof(TCHAR)); 
    
    TCHAR * pIter = NULL; 
    TCHAR * pGuaide = infoBuf + cchMax + 1; 
    TCHAR * pItemBegin = infoBuf; 
    
    BOOL bMustJump = FALSE; 
    
    for (pIter = infoBuf; pIter <= pGuaide; pIter ++ ) 
    { 
        if (bMustJump) { 
            bMustJump = FALSE; 
            pItemBegin = pIter; 
        } 
        
        if ( *pIter == '\0') { 
            vecPaths.push_back(pItemBegin); 
            break; 
        } 
        
        if (pIter == pGuaide) { 
            *(pIter-1) = '\0'; 
            vecPaths.push_back(pItemBegin); 
            break; 
        } 
        
        if ( *pIter == ';' ) { 
            *pIter = '\0'; 
            vecPaths.push_back(pItemBegin); 
            bMustJump = TRUE; 
        } 
    } 
    LocalFree(infoBuf); 
} 

BOOL _DeleteEntireKey(HKEY hRootKey, LPTSTR pszSubKey) 
{ 
    BOOL bReturn = FALSE; 
    LRESULT  lResult; 
    HKEY     hEnumKey; 
    
    lResult = RegOpenKeyEx(hRootKey, pszSubKey, 0, KEY_ENUMERATE_SUB_KEYS, &hEnumKey); 
    
    if(ERROR_SUCCESS == lResult) 
    { 
        TCHAR szKey[MAX_PATH] = { 0 }; 
        DWORD dwSize = _countof(szKey); 
        
        while(ERROR_SUCCESS == RegEnumKeyEx(hEnumKey, 0, szKey, &dwSize, NULL, NULL, NULL, NULL))
        { 
            _DeleteEntireKey(hEnumKey, szKey); 
            dwSize = _countof(szKey); 
        } 
        RegCloseKey(hEnumKey); 
        
        RegDeleteKey(hRootKey, pszSubKey); 
        bReturn = TRUE; 
    } 
    return bReturn; 
} // _DeleteEntireKey 

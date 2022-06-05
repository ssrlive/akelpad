#pragma once 

#ifdef __cplusplus 
extern "C" { 
#endif  // __cplusplus 

BOOL FindExeFile(HMODULE hMyselfModule, LPCTSTR lpszWhat, OUT TCHAR szFullExeFile[], UINT cchMax); 

BOOL RegisterOpenWith(HMODULE hMyselfModule, LPCTSTR lpszAppExeName); 
BOOL UnregisterOpenWith(LPCTSTR lpszAppExeName); 


#ifdef __cplusplus 
}; 
#endif  // __cplusplus 

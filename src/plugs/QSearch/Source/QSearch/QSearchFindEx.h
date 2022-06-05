#ifndef _qsearch_find_ex_h_
#define _qsearch_find_ex_h_
//---------------------------------------------------------------------------
#include <windows.h>
#include "AkelDllHeader.h"

INT_X   doFindTextExA(HWND hEd, TEXTFINDA* ptfA);
INT_X   doFindTextExW(HWND hEd, TEXTFINDW* ptfW);
int     findSpecialCharA(LPCSTR cszTextA);
int     findSpecialCharW(LPCWSTR cszTextW);
void    getTextToSearchA(LPCSTR cszTextA, BOOL* pbSearchEx, const DWORD dwOptFlags[], CHAR out_pszSearchTextA[]);
void    getTextToSearchW(LPCWSTR cszTextW, BOOL* pbSearchEx, const DWORD dwOptFlags[], WCHAR out_pszSearchTextW[]);

int     match_mask(const char* mask, const char* str, char** last_pos, int whole_word);
int     match_maskw(const wchar_t* maskw, const wchar_t* strw, wchar_t** last_pos, int whole_word);

//---------------------------------------------------------------------------
#endif

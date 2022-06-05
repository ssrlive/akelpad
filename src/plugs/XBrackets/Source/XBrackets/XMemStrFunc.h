#ifndef _mem_str_func_h_
#define _mem_str_func_h_
//---------------------------------------------------------------------------
#include <windows.h>

// memcpy
void x_mem_cpy(void *pDest, const void *pSrc, UINT_PTR nBytes);

// memset
void x_mem_set(void *pDest, unsigned int c, UINT_PTR nBytes);

// ZeroMemory
void x_zero_mem(void* pDest, UINT_PTR nBytes);

//---------------------------------------------------------------------------
#endif

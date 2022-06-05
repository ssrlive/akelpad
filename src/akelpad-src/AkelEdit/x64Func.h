/*****************************************************************
 *                   x64 functions header v1.2                   *
 *                                                               *
 * 2013 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Functions:                                                     *
 * MultiByteToWideChar64, WideCharToMultiByte64,                 *
 * GetFileSize64, SetFilePointer64, ReadFile64, WriteFile64      *
 *****************************************************************/

#ifndef _X64FUNC_H_
#define _X64FUNC_H_

#define ALLX64FUNC 1

INT_PTR MultiByteToWideChar64(UINT dwCodePage, DWORD dwFlags, const char *lpMultiByteStr, INT_PTR cbMultiByte, wchar_t *lpWideCharStr, INT_PTR cchWideChar);
INT_PTR WideCharToMultiByte64(UINT dwCodePage, DWORD dwFlags, const wchar_t *lpWideCharStr, INT_PTR cchWideChar, char *lpMultiByteStr, INT_PTR cbMultiByte, const char *lpDefaultChar, BOOL *lpUsedDefaultChar);
UINT_PTR GetFileSize64(HANDLE hFile);
UINT_PTR SetFilePointer64(HANDLE hFile, INT_PTR nDistanceToMove, DWORD dwMoveMethod);
BOOL ReadFile64(HANDLE hFile, LPVOID lpBuffer, UINT_PTR nNumberOfBytesToRead, UINT_PTR *lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
BOOL WriteFile64(HANDLE hFile, LPCVOID lpBuffer, UINT_PTR nNumberOfBytesToWrite, UINT_PTR *lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);


/********************************************************************
 *
 *  MultiByteToWideChar64
 *
 *Converts ansi string to unicode string. See MSDN for MultiByteToWideChar description.
 *
 *Note:
 *  MultiByteToWideChar64 uses xstrlenA, UTF8toUTF16 from StrFunc.h.
 ********************************************************************/
#if defined MultiByteToWideChar64 || defined ALLX64FUNC
#define MultiByteToWideChar64_INCLUDED
#undef MultiByteToWideChar64
__inline INT_PTR MultiByteToWideChar64(UINT dwCodePage, DWORD dwFlags, const char *lpMultiByteStr, INT_PTR cbMultiByte, wchar_t *lpWideCharStr, INT_PTR cchWideChar)
{
  INT_PTR nMultiByteCount=0;
  INT_PTR nWideCount=0;
  int nMultiByteSrc;
  int nWideDst;
  int nCharsConverted;
  DWORD dwMaxCharSize=0;

  if (cbMultiByte == -1)
    cbMultiByte=xstrlenA(lpMultiByteStr) + 1;

  if (dwCodePage == 65001)
  {
    nWideCount=UTF8toUTF16((const unsigned char *)lpMultiByteStr, cbMultiByte, NULL, (unsigned short *)lpWideCharStr, cchWideChar);
  }
  else
  {
    //Convert string sequentially with maximum block 0x0FFFFFFF
    for (;;)
    {
      if (cbMultiByte - nMultiByteCount < 0x0FFFFFFF)
        nMultiByteSrc=(int)(cbMultiByte - nMultiByteCount);
      else
      {
        nMultiByteSrc=0x0FFFFFFF;
        if (!dwMaxCharSize)
        {
          CPINFO cpi;

          if (GetCPInfo(dwCodePage, &cpi))
            dwMaxCharSize=cpi.MaxCharSize;
        }
        if (dwMaxCharSize >= 2)
        {
          //If DBCS or MBCS codepage, try to find line ending
          while (*(lpMultiByteStr + nMultiByteCount + nMultiByteSrc) != '\n' && *(lpMultiByteStr + nMultiByteCount + nMultiByteSrc) != '\r')
            if (--nMultiByteSrc < 0) return 0;
          ++nMultiByteSrc;
        }
      }
      nWideDst=(int)min(cchWideChar - nWideCount, 0x3FFFFFFF);

      if (nCharsConverted=MultiByteToWideChar(dwCodePage, dwFlags, lpMultiByteStr + nMultiByteCount, nMultiByteSrc, lpWideCharStr + nWideCount, nWideDst))
      {
        nMultiByteCount+=nMultiByteSrc;
        nWideCount+=nCharsConverted;
        if (nMultiByteCount >= cbMultiByte || nWideCount >= cchWideChar)
          break;
      }
      else break;
    }
  }
  return nWideCount;
}
#endif

/********************************************************************
 *
 *  WideCharToMultiByte64
 *
 *Converts unicode string to ansi string. See MSDN for WideCharToMultiByte description.
 *
 *Note:
 *  WideCharToMultiByte64 uses xstrlenW, UTF16toUTF8 from StrFunc.h.
 ********************************************************************/
#if defined WideCharToMultiByte64 || defined ALLX64FUNC
#define WideCharToMultiByte64_INCLUDED
#undef WideCharToMultiByte64
__inline INT_PTR WideCharToMultiByte64(UINT dwCodePage, DWORD dwFlags, const wchar_t *lpWideCharStr, INT_PTR cchWideChar, char *lpMultiByteStr, INT_PTR cbMultiByte, const char *lpDefaultChar, BOOL *lpUsedDefaultChar)
{
  INT_PTR nMultiByteCount=0;
  INT_PTR nWideCount=0;
  int nWideSrc;
  int nMultiByteDst;
  int nCharsConverted;
  BOOL bUsedDefaultChar;

  if (cchWideChar == -1)
    cchWideChar=xstrlenW(lpWideCharStr) + 1;
  if (lpUsedDefaultChar)
    bUsedDefaultChar=FALSE;
  else
    bUsedDefaultChar=TRUE;

  if (dwCodePage == 65001)
  {
    nMultiByteCount=UTF16toUTF8((const unsigned short *)lpWideCharStr, cchWideChar, NULL, (unsigned char *)lpMultiByteStr, cbMultiByte);
  }
  else
  {
    //Convert string sequentially with maximum block 0x3FFFFFFF
    for (;;)
    {
      nWideSrc=(int)min(cchWideChar - nWideCount, 0x3FFFFFFF);
      nMultiByteDst=(int)min(cbMultiByte - nMultiByteCount, 0x7FFFFFFF);

      if (nCharsConverted=WideCharToMultiByte(dwCodePage, dwFlags, lpWideCharStr + nWideCount, nWideSrc, lpMultiByteStr + nMultiByteCount, nMultiByteDst, lpDefaultChar, !bUsedDefaultChar?&bUsedDefaultChar:NULL))
      {
        nWideCount+=nWideSrc;
        nMultiByteCount+=nCharsConverted;
        if (nWideCount >= cchWideChar || nMultiByteCount >= cbMultiByte)
          break;
      }
      else break;
    }
  }
  if (lpUsedDefaultChar)
    *lpUsedDefaultChar=bUsedDefaultChar;

  return nMultiByteCount;
}
#endif

/********************************************************************
 *
 *  GetFileSize64
 *
 *Retrieves the size of the specified file, in bytes.
 ********************************************************************/
#if defined GetFileSize64 || defined ALLX64FUNC
#define GetFileSize64_INCLUDED
#undef GetFileSize64
__inline UINT_PTR GetFileSize64(HANDLE hFile)
{
  #ifdef _WIN64
    LARGE_INTEGER liFileSize;

    if (GetFileSizeEx(hFile, &liFileSize))
      return liFileSize.QuadPart;
    else
      return (UINT_PTR)-1;
  #else
    return GetFileSize(hFile, NULL);
  #endif
}
#endif

/********************************************************************
 *
 *  SetFilePointer64
 *
 *Moves the file pointer of the specified file. See MSDN for SetFilePointer description.
 ********************************************************************/
#if defined SetFilePointer64 || defined ALLX64FUNC
#define SetFilePointer64_INCLUDED
#undef SetFilePointer64
__inline UINT_PTR SetFilePointer64(HANDLE hFile, INT_PTR nDistanceToMove, DWORD dwMoveMethod)
{
  #ifdef _WIN64
    LARGE_INTEGER liDistanceToMove;
    LARGE_INTEGER liNewFilePointer;

    liDistanceToMove.QuadPart=nDistanceToMove;
    if (SetFilePointerEx(hFile, liDistanceToMove, &liNewFilePointer, dwMoveMethod))
      return liNewFilePointer.QuadPart;
    else
      return (UINT_PTR)-1;
  #else
    return SetFilePointer(hFile, nDistanceToMove, NULL, dwMoveMethod);
  #endif
}
#endif

/********************************************************************
 *
 *  ReadFile64
 *
 *Reads data from the specified file. See MSDN for ReadFile description.
 ********************************************************************/
#if defined ReadFile64 || defined ALLX64FUNC
#define ReadFile64_INCLUDED
#undef ReadFile64
__inline BOOL ReadFile64(HANDLE hFile, LPVOID lpBuffer, UINT_PTR nNumberOfBytesToRead, UINT_PTR *lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
  DWORD dwBytesToRead;
  DWORD dwBytesRead;
  UINT_PTR dwCount=0;
  BOOL bResult;

  //Read file sequentially with maximum block 0xFFFF
  for (;;)
  {
    dwBytesToRead=(DWORD)min(nNumberOfBytesToRead - dwCount, 0xFFFF);

    if (bResult=ReadFile(hFile, (LPBYTE)lpBuffer + dwCount, dwBytesToRead, &dwBytesRead, lpOverlapped))
    {
      dwCount+=dwBytesRead;
      if (dwBytesRead < dwBytesToRead || dwCount >= nNumberOfBytesToRead)
        break;
    }
    else break;
  }
  *lpNumberOfBytesRead=dwCount;

  return bResult;
}
#endif

/********************************************************************
 *
 *  WriteFile64
 *
 *Writes data to the specified file. See MSDN for WriteFile description.
 ********************************************************************/
#if defined WriteFile64 || defined ALLX64FUNC
#define WriteFile64_INCLUDED
#undef WriteFile64
__inline BOOL WriteFile64(HANDLE hFile, LPCVOID lpBuffer, UINT_PTR nNumberOfBytesToWrite, UINT_PTR *lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
  DWORD dwBytesToWrite;
  DWORD dwBytesWritten;
  UINT_PTR dwCount=0;
  BOOL bResult;

  //Write file sequentially with maximum block 0xFFFF
  for (;;)
  {
    dwBytesToWrite=(DWORD)min(nNumberOfBytesToWrite - dwCount, 0xFFFF);

    if (bResult=WriteFile(hFile, (LPBYTE)lpBuffer + dwCount, dwBytesToWrite, &dwBytesWritten, lpOverlapped))
    {
      dwCount+=dwBytesWritten;
      if (dwBytesWritten < dwBytesToWrite || dwCount >= nNumberOfBytesToWrite)
        break;
    }
    else break;
  }
  *lpNumberOfBytesWritten=dwCount;

  return bResult;
}
#endif


/********************************************************************
 *                                                                  *
 *                           Example                                *
 *                                                                  *
 ********************************************************************

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "StrFunc.h"
#include "x64Func.h"

//Include string functions
#define xstrlenA
#define xstrlenW
#define UTF8toUTF16
#include "StrFunc.h"

//Include x64 functions
#define MultiByteToWideChar64
#define WideCharToMultiByte64
#include "x64Func.h"

void main()
{
  char szResult[MAX_PATH];
  wchar_t wszResult[MAX_PATH];
  char *pStringBegin=NULL;
  char *pStringEnd=NULL;
  INT_PTR nStringLen;
  int nError;

  nError=MultiByteToWideChar(CP_ACP, 0, "abcxyz", -1, wszResult, MAX_PATH);
  printf("wszResult={%S}, nError={%d}\n", wszResult, nError);

  nError=WideCharToMultiByte(CP_ACP, 0, L"abcxyz", -1, szResult, MAX_PATH, NULL, NULL);
  printf("szResult={%s}, nError={%d}\n", szResult, nError);
}

*/


#endif  // _X64FUNC_H_

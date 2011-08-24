/***********************************************************************************
 *                      AkelAdmin UAC support for AkelPad                          *
 *                                                                                 *
 * Copyright 2011 by Shengalts Aleksander aka Instructor (Shengalts@mail.ru)       *
 *                                                                                 *
 * License: this source is distributed under "BSD license" conditions.             *
 ***********************************************************************************/


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "..\AkelEdit\StrFunc.h"

//Include string functions
#define xatoiW
#include "..\AkelEdit\StrFunc.h"


//Defines
#define STRID_ERRORDIR        1
#define STRID_ERRORCALL       2
#define STRID_ERRORSECURITY   3
#define STRID_ERRORDELETE     4
#define STRID_ERRORMOVE       5

#define BUFFER_SIZE      1024

#ifndef INVALID_FILE_ATTRIBUTES
  #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif
#ifndef DACL_SECURITY_INFORMATION
  #define DACL_SECURITY_INFORMATION  (0x00000004L)
#endif
#ifndef GROUP_SECURITY_INFORMATION
  #define GROUP_SECURITY_INFORMATION (0x00000002L)
#endif
#ifndef LABEL_SECURITY_INFORMATION
  #define LABEL_SECURITY_INFORMATION (0x00000010L)
#endif
#ifndef OWNER_SECURITY_INFORMATION
  #define OWNER_SECURITY_INFORMATION (0x00000001L)
#endif


//Functions prototypes
int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen);
const wchar_t* GetFileName(const wchar_t *wpFile);
int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, wchar_t **wpNextArg);
const wchar_t* GetLangStringW(LANGID wLangID, int nStringID);

//Global variables
wchar_t wszBuffer[BUFFER_SIZE];
HINSTANCE hInstance;
LANGID wLangModule=0;

//GCC
#ifdef __GNUC__
int main()
{
  return 0;
}
#endif

//Entry point
void _WinMain()
{
  wchar_t *wpCmdLine=GetCommandLineW();
  wchar_t *pArguments=wpCmdLine;
  wchar_t wszSource[MAX_PATH];
  wchar_t wszTarget[MAX_PATH];
  int nAction=-1;
  DWORD dwSourceAttr;
  DWORD dwTargetAttr;
  DWORD dwExitCode=1;

  //Get program HINSTANCE
  hInstance=GetModuleHandleW(NULL);

  if (hInstance)
  {
    //Get program directory
    GetExeDir(hInstance, wszBuffer, BUFFER_SIZE);

    if (!lstrcmpiW(L"AkelFiles", GetFileName(wszBuffer)))
    {
      //Skip executable
      GetCommandLineArgW(pArguments, NULL, 0, &pArguments);

      //First argument is action number. Currently only one action with zero number.
      if (GetCommandLineArgW(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
      {
        nAction=(int)xatoiW(wszBuffer, NULL);

        if (nAction == 0)
        {
          //Second argument is source file
          if (GetCommandLineArgW(pArguments, wszSource, MAX_PATH, &pArguments))
          {
            //Third argument is target file
            if (GetCommandLineArgW(pArguments, wszTarget, MAX_PATH, &pArguments))
            {
              //Fourth argument is AkelPad language
              if (GetCommandLineArgW(pArguments, wszBuffer, BUFFER_SIZE, &pArguments))
              {
                wLangModule=PRIMARYLANGID(xatoiW(wszBuffer, NULL));

                if ((dwSourceAttr=GetFileAttributesW(wszSource)) != INVALID_FILE_ATTRIBUTES &&
                    (dwTargetAttr=GetFileAttributesW(wszTarget)) != INVALID_FILE_ATTRIBUTES)
                {
                  SECURITY_INFORMATION ssi=DACL_SECURITY_INFORMATION|
                                           GROUP_SECURITY_INFORMATION|
                                           LABEL_SECURITY_INFORMATION|
                                           OWNER_SECURITY_INFORMATION;
                  SECURITY_DESCRIPTOR *psd=NULL;
                  DWORD dwSize=0;

                  //Retrive file security
                  GetFileSecurityW(wszTarget, ssi, NULL, 0, &dwSize);

                  if (dwSize)
                  {
                    if (psd=(SECURITY_DESCRIPTOR *)GlobalAlloc(GMEM_FIXED, dwSize))
                    {
                      if (!GetFileSecurityW(wszTarget, ssi, psd, dwSize, &dwSize))
                      {
                        GlobalFree(psd);
                        psd=NULL;
                      }
                    }
                  }

                  //Replace protected file
                  if (DeleteFileW(wszTarget))
                  {
                    if (MoveFileW(wszSource, wszTarget))
                    {
                      //Copy attributes and security
                      SetFileAttributesW(wszTarget, dwTargetAttr);
                      if (psd)
                      {
                        if (!SetFileSecurityW(wszTarget, ssi, psd))
                        {
                          wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORSECURITY), wszTarget);
                          MessageBoxW(NULL, wszBuffer, L"AkelAdmin", MB_ICONERROR);
                        }
                      }
                      dwExitCode=0;
                    }
                    else
                    {
                      wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORMOVE), wszSource, wszTarget);
                      MessageBoxW(NULL, wszBuffer, L"AkelAdmin", MB_ICONERROR);
                    }
                  }
                  else
                  {
                    wsprintfW(wszBuffer, GetLangStringW(wLangModule, STRID_ERRORDELETE), wszTarget);
                    MessageBoxW(NULL, wszBuffer, L"AkelAdmin", MB_ICONERROR);
                  }

                  //Free security buffer
                  if (psd)
                  {
                    GlobalFree(psd);
                    psd=NULL;
                  }
                }
              }
            }
          }
        }
      }
      if (nAction == -1)
        MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORCALL), L"AkelAdmin", MB_ICONEXCLAMATION);
    }
    else MessageBoxW(NULL, GetLangStringW(wLangModule, STRID_ERRORDIR), L"AkelAdmin", MB_ICONEXCLAMATION);
  }
  ExitProcess(dwExitCode);
}

int GetExeDir(HINSTANCE hInstance, wchar_t *wszExeDir, int nLen)
{
  if (nLen=GetModuleFileNameW(hInstance, wszExeDir, nLen))
  {
    while (nLen > 0 && wszExeDir[nLen] != '\\') --nLen;
    wszExeDir[nLen]='\0';
  }
  return nLen;
}

const wchar_t* GetFileName(const wchar_t *wpFile)
{
  int i;

  for (i=lstrlenW(wpFile) - 1; i >= 0; --i)
  {
    if (wpFile[i] == '\\')
      return (wpFile + i + 1);
  }
  return wpFile;
}

int GetCommandLineArgW(wchar_t *wpCmdLine, wchar_t *wszArg, int nArgMax, wchar_t **wpNextArg)
{
  wchar_t *wpCount=wpCmdLine;
  wchar_t *wpArgSet=wszArg;
  wchar_t *wpArgSetMax=wszArg + nArgMax - 1;
  wchar_t wchInitStopChar;
  wchar_t wchCurStopChar;

  while (*wpCount == L' ') ++wpCount;

  if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
    wchInitStopChar=*wpCount++;
  else
    wchInitStopChar=L' ';
  wchCurStopChar=wchInitStopChar;

  for (; *wpCount; ++wpCount)
  {
    if (wchCurStopChar == L' ')
    {
      if (*wpCount == L' ')
        break;
      if (*wpCount == L'\"' || *wpCount == L'\'' || *wpCount == L'`')
        wchCurStopChar=*wpCount;
    }
    else if (wchCurStopChar == *wpCount)
    {
      if (wchCurStopChar == wchInitStopChar)
        break;
      wchCurStopChar=L' ';
    }

    if (wpArgSet < wpArgSetMax)
    {
      if (wszArg) *wpArgSet=*wpCount;
      ++wpArgSet;
    }
  }
  if (wszArg) *wpArgSet=L'\0';

  if (wpNextArg)
  {
    if (*wpCount) ++wpCount;
    for (*wpNextArg=wpCount; **wpNextArg == L' '; ++*wpNextArg);
  }
  return (int)(wpArgSet - wszArg);
}

const wchar_t* GetLangStringW(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin \x0434\x043E\x043B\x0436\x0435\x043D\x0020\x043D\x0430\x0445\x043E\x0434\x0438\x0442\x044C\x0441\x044F\x0020\x0432\x0020\x043F\x0430\x043F\x043A\x0435 AkelFiles.";
    if (nStringID == STRID_ERRORCALL)
      return L"\x0414\x0430\x043D\x043D\x0430\x044F\x0020\x043F\x0440\x043E\x0433\x0440\x0430\x043C\x043C\x0430\x0020\x043F\x0440\x0435\x0434\x043D\x0430\x0437\x043D\x0430\x0447\x0435\x043D\x0430\x0020\x0434\x043B\x044F\x0020\x0432\x043D\x0443\x0442\x0440\x0435\x043D\x043D\x0435\x0433\x043E\x0020\x0438\x0441\x043F\x043E\x043B\x044C\x0437\x043E\x0432\x0430\x043D\x0438\x044F AkelPad'\x043E\x043C. \x041D\x0435\x0020\x0432\x044B\x0437\x044B\x0432\x0430\x0439\x0442\x0435\x0020\x0435\x0435\x0020\x043D\x0430\x043F\x0440\x044F\x043C\x0443\x044E.";
    if (nStringID == STRID_ERRORSECURITY)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0441\x0442\x0430\x043D\x043E\x0432\x0438\x0442\x044C\x0020\x043D\x0430\x0441\x0442\x0440\x043E\x0439\x043A\x0438\x0020\x0431\x0435\x0437\x043E\x043F\x0430\x0441\x043D\x043E\x0441\x0442\x0438\x0020\x0434\x043B\x044F\x0020\x0444\x0430\x0439\x043B\x0430 \"%s\"";
    if (nStringID == STRID_ERRORDELETE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x0443\x0434\x0430\x043B\x0438\x0442\x044C\x0020\x0444\x0430\x0439\x043B \"%s\"";
    if (nStringID == STRID_ERRORMOVE)
      return L"\x041D\x0435\x0020\x0443\x0434\x0430\x0435\x0442\x0441\x044F\x0020\x043F\x0435\x0440\x0435\x043C\x0435\x0441\x0442\x0438\x0442\x044C\x0020\x0444\x0430\x0439\x043B.\n\x0418\x0437:\"%s\"\n\x0432: \"%s\"";
  }
  else
  {
    if (nStringID == STRID_ERRORDIR)
      return L"AkelAdmin should be in AkelFiles folder.";
    if (nStringID == STRID_ERRORCALL)
      return L"This program is internal for AkelPad. Don't call it directly.";
    if (nStringID == STRID_ERRORSECURITY)
      return L"Can't set security options for file \"%s\"";
    if (nStringID == STRID_ERRORDELETE)
      return L"Can't delete file \"%s\"";
    if (nStringID == STRID_ERRORMOVE)
      return L"Can't move file.\nFrom: \"%s\"\nto: \"%s\"";
  }
  return L"";
}

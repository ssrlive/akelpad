#ifndef __CODEPAGE_H__
#define __CODEPAGE_H__

#ifndef FOREIGN_BUILD
#define WINDOWS_1251 1251
#define DOS_866 866
#define KOI8_R 20866
#else
#define WINDOWS_1251 CP_ACP
#define DOS_866 CP_OEMCP
#define KOI8_R 20866
#endif

#define CP_UNICODE_UCS2_LE -17
#define CP_UNICODE_UCS2_BE -18
#define CP_UNICODE_UTF8 -19

int GetCodePage();

BOOL ChangeCodePage(int cp);

BOOL SelectCodePage(HWND hWnd);

int TranslateTextToRepresent(char *pcBuff,int iBufSize,char *pcOutBuf,char **endptr,BOOL firsttime);
int TranslateTextFromRepresent(char *pcBuff,int iBufSize,char *pcOutBuf,BOOL firsttime);
/* WARNING: Caller must allocate data for pcOutBuf, 6*iBufSize should be enough for everyone ;).*/

BOOL AutodetectCodePage(unsigned char *pcBuffer,int iCount);

#endif

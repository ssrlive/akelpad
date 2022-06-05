/*****************************************************************
 *           Conversion functions header v2.5                    *
 *                                                               *
 * 2008 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Functions (ALLCONVFUNC):                                       *
 * xatoiA, xatoiW, xatoi64A, xatoi64W,                           *
 * xitoaA, xitoaW, xuitoaA, xuitoaW, xi64toaA, xi64toaW,         *
 * hex2decA, hex2decW, dec2hexA, dec2hexW                        *
 *                                                               *
 *Special functions (ALLCONVFUNCS):                              *
 * str2hexA, str2hexW, hex2strA, hex2strW                        *
 *                                                               *
 *****************************************************************/

#ifndef _CONVFUNC_H_
#define _CONVFUNC_H_

int xatoiA(char *pStr);
int xatoiW(wchar_t *wpStr);
__int64 xatoi64A(char *pStr);
__int64 xatoi64W(wchar_t *wpStr);
char* xitoaA(int nNumber, char *szStr, int nWidth);
wchar_t* xitoaW(int nNumber, wchar_t *wszStr, int nWidth);
char* xuitoaA(unsigned int nNumber, char *szStr, int nWidth);
wchar_t* xuitoaW(unsigned int nNumber, wchar_t *wszStr, int nWidth);
char* xi64toaA(__int64 nNumber, char *szStr, int nWidth);
wchar_t* xi64toaW(__int64 nNumber, wchar_t *wszStr, int nWidth);
int hex2decA(char *pStrHex);
int hex2decW(wchar_t *wpStrHex);
int dec2hexA(unsigned int nDec, char *szStrHex, unsigned int nWidth, BOOL bLowerCase);
int dec2hexW(unsigned int nDec, wchar_t *wszStrHex, unsigned int nWidth, BOOL bLowerCase);

int str2hexA(unsigned char *pStr, int nBytes, char *szStrHex, int nStrHexMax, BOOL bLowerCase);
int str2hexW(unsigned char *pStr, int nBytes, wchar_t *wszStrHex, int nStrHexMax, BOOL bLowerCase);
int hex2strA(char *pStrHex, unsigned char *szStr, int nStrMax);
int hex2strW(wchar_t *wpStrHex, unsigned char *szStr, int nStrMax);

#endif

/********************************************************************
 *
 *  xatoiA
 *
 *Converts string to int.
 *
 *[in]  char *pStr   -string number
 *
 *Returns: integer
 *
 *Examples:
 *  xatoiA("45") == 45;
 *  xatoiA("  -0045:value") == -45;
 ********************************************************************/
#if defined xatoiA || defined ALLCONVFUNC
#define xatoiA_INCLUDED
#undef xatoiA
int xatoiA(char *pStr)
{
  int nNumber=0;
  BOOL bMinus=FALSE;

  while (*pStr == ' ' || *pStr == '\t')
    ++pStr;
  if (*pStr == '+')
    ++pStr;
  else if (*pStr == '-')
  {
    bMinus=TRUE;
    ++pStr;
  }

  while (*pStr >= '0' && *pStr <= '9')
  {
    nNumber=(nNumber * 10) + (*pStr - '0');
    ++pStr;
  }
  if (bMinus == TRUE)
    nNumber=0 - nNumber;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xatoiW
 *
 *Converts unicode string to int.
 *
 *[in]  wchar_t *wpStr   -string number
 *
 *Returns: integer
 *
 *Examples:
 *  xatoiW(L"45") == 45;
 *  xatoiW(L"  -0045:value") == -45;
 ********************************************************************/
#if defined xatoiW || defined ALLCONVFUNC
#define xatoiW_INCLUDED
#undef xatoiW
int xatoiW(wchar_t *wpStr)
{
  int nNumber=0;
  BOOL bMinus=FALSE;

  while (*wpStr == ' ' || *wpStr == '\t')
    ++wpStr;
  if (*wpStr == '+')
    ++wpStr;
  else if (*wpStr == '-')
  {
    bMinus=TRUE;
    ++wpStr;
  }

  while (*wpStr >= '0' && *wpStr <= '9')
  {
    nNumber=(nNumber * 10) + (*wpStr - '0');
    ++wpStr;
  }
  if (bMinus == TRUE)
    nNumber=0 - nNumber;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xatoi64A
 *
 *Converts string to int64.
 *
 *[in]  char *pStr   -string number
 *
 *Returns: 64-bit integer
 *
 *Examples:
 *  xatoi64A("45") == 45;
 *  xatoi64A("  -0045:value") == -45;
 ********************************************************************/
#if defined xatoi64A || defined ALLCONVFUNC
#define xatoi64A_INCLUDED
#undef xatoi64A
__int64 xatoi64A(char *pStr)
{
  __int64 nNumber=0;
  BOOL bMinus=FALSE;

  while (*pStr == ' ' || *pStr == '\t')
    ++pStr;
  if (*pStr == '+')
    ++pStr;
  else if (*pStr == '-')
  {
    bMinus=TRUE;
    ++pStr;
  }

  while (*pStr >= '0' && *pStr <= '9')
  {
    nNumber=(nNumber * 10) + (*pStr - '0');
    ++pStr;
  }
  if (bMinus == TRUE)
    nNumber=0 - nNumber;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xatoi64W
 *
 *Converts unicode string to int64.
 *
 *[in]  wchar_t *wpStr   -unicode string number
 *
 *Returns: 64-bit integer
 *
 *Examples:
 *  xatoi64W(L"45") == 45;
 *  xatoi64W(L"  -0045:value") == -45;
 ********************************************************************/
#if defined xatoi64W || defined ALLCONVFUNC
#define xatoi64W_INCLUDED
#undef xatoi64W
__int64 xatoi64W(wchar_t *wpStr)
{
  __int64 nNumber=0;
  BOOL bMinus=FALSE;

  while (*wpStr == ' ' || *wpStr == '\t')
    ++wpStr;
  if (*wpStr == '+')
    ++wpStr;
  else if (*wpStr == '-')
  {
    bMinus=TRUE;
    ++wpStr;
  }

  while (*wpStr >= '0' && *wpStr <= '9')
  {
    nNumber=(nNumber * 10) + (*wpStr - '0');
    ++wpStr;
  }
  if (bMinus == TRUE)
    nNumber=0 - nNumber;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xitoaA   [API: wsprintfA(szResult, "%d", 45)]
 *
 *Converts int to string.
 *
 *[in]   int nNumber  -integer
 *[out]  char *szStr  -string number
 *[in]   int nWidth   -minimum number of characters to the output
 *
 *Returns: a pointer to string
 *
 *Examples:
 *  xitoaA(45, szResult, 0);   //szResult == "45"
 *  xitoaA(-45, szResult, 0);  //szResult == "-45"
 *  xitoaA(45, szResult, 4);   //szResult == "0045"
 ********************************************************************/
#if defined xitoaA || defined ALLCONVFUNC
#define xitoaA_INCLUDED
#undef xitoaA
char* xitoaA(int nNumber, char *szStr, int nWidth)
{
  char szReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    szStr[0]='0';
    --nWidth;
    b=1;
  }
  else if (nNumber < 0)
  {
    szStr[0]='-';
    nNumber=0 - nNumber;
    --nWidth;
    b=1;
  }
  for (a=0; nNumber != 0; ++a)
  {
    szReverse[a]=(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  for (; nWidth > a; ++a) szReverse[a]='0';
  for (--a; a >= 0; --a, ++b) szStr[b]=szReverse[a];

  szStr[b]='\0';
  return szStr;
}
#endif

/********************************************************************
 *
 *  xitoaW   [API: wsprintfW(wszResult, L"%d", 45)]
 *
 *Converts int to unicode string.
 *
 *[in]   int nNumber      -integer
 *[out]  wchar_t *wszStr  -unicode string number
 *[in]   int nWidth       -minimum number of characters to the output
 *
 *Returns: a pointer to unicode string
 *
 *Examples:
 *  xitoaW(45, wszResult, 0);   //wszResult == L"45"
 *  xitoaW(-45, wszResult, 0);  //wszResult == L"-45"
 *  xitoaW(45, wszResult, 4);   //wszResult == L"0045"
 ********************************************************************/
#if defined xitoaW || defined ALLCONVFUNC
#define xitoaW_INCLUDED
#undef xitoaW
wchar_t* xitoaW(int nNumber, wchar_t *wszStr, int nWidth)
{
  wchar_t wszReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    wszStr[0]='0';
    --nWidth;
    b=1;
  }
  else if (nNumber < 0)
  {
    wszStr[0]='-';
    nNumber=0 - nNumber;
    --nWidth;
    b=1;
  }
  for (a=0; nNumber != 0; ++a)
  {
    wszReverse[a]=(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  for (; nWidth > a; ++a) wszReverse[a]='0';
  for (--a; a >= 0; --a, ++b) wszStr[b]=wszReverse[a];

  wszStr[b]='\0';
  return wszStr;
}
#endif

/********************************************************************
 *
 *  xuitoaA
 *
 *Converts unsigned int to string.
 *
 *[in]   unsigned int nNumber  -unsigned integer
 *[out]  char *szStr           -string number
 *[in]   int nWidth            -minimum number of characters to the output
 *
 *Returns: a pointer to string
 *
 *Examples:
 *  xuitoaA(45, szResult, 0);   //szResult == "45"
 *  xuitoaA(45, szResult, 4);   //szResult == "0045"
 ********************************************************************/
#if defined xuitoaA || defined ALLCONVFUNC
#define xuitoaA_INCLUDED
#undef xuitoaA
char* xuitoaA(unsigned int nNumber, char *szStr, int nWidth)
{
  char szReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    szStr[0]='0';
    --nWidth;
    b=1;
  }
  for (a=0; nNumber != 0; ++a)
  {
    szReverse[a]=(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  for (; nWidth > a; ++a) szReverse[a]='0';
  for (--a; a >= 0; --a, ++b) szStr[b]=szReverse[a];

  szStr[b]='\0';
  return szStr;
}
#endif

/********************************************************************
 *
 *  xuitoaW
 *
 *Converts unsigned int to unicode string.
 *
 *[in]   unsigned int nNumber  -unsigned integer
 *[out]  wchar_t *wszStr       -unicode string number
 *[in]   int nWidth            -minimum number of characters to the output
 *
 *Returns: a pointer to unicode string
 *
 *Examples:
 *  xuitoaW(45, wszResult, 0);   //wszResult == L"45"
 *  xuitoaW(45, wszResult, 4);   //wszResult == L"0045"
 ********************************************************************/
#if defined xuitoaW || defined ALLCONVFUNC
#define xuitoaW_INCLUDED
#undef xuitoaW
wchar_t* xuitoaW(unsigned int nNumber, wchar_t *wszStr, int nWidth)
{
  wchar_t wszReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    wszStr[0]='0';
    --nWidth;
    b=1;
  }
  for (a=0; nNumber != 0; ++a)
  {
    wszReverse[a]=(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  for (; nWidth > a; ++a) wszReverse[a]='0';
  for (--a; a >= 0; --a, ++b) wszStr[b]=wszReverse[a];

  wszStr[b]='\0';
  return wszStr;
}
#endif

/********************************************************************
 *
 *  xitoa64A
 *
 *Converts int64 to string.
 *
 *[in]   __int64 nNumber -64-bit integer
 *[out]  char *szStr     -string number
 *[in]   int nWidth      -minimum number of characters to the output
 *
 *Returns: a pointer to string
 *
 *Examples:
 *  xi64toaA(45, szResult, 0);   //szResult == "45"
 *  xi64toaA(-45, szResult, 0);  //szResult == "-45"
 *  xi64toaA(45, szResult, 4);   //szResult == "0045"
 ********************************************************************/
#if defined xi64toaA || defined ALLCONVFUNC
#define xi64toaA_INCLUDED
#undef xi64toaA
char* xi64toaA(__int64 nNumber, char *szStr, int nWidth)
{
  char szReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    szStr[0]='0';
    --nWidth;
    b=1;
  }
  else if (nNumber < 0)
  {
    szStr[0]='-';
    nNumber=0 - nNumber;
    --nWidth;
    b=1;
  }
  for (a=0; nNumber != 0; ++a)
  {
    szReverse[a]=(char)((nNumber % 10) + '0');
    nNumber=nNumber / 10;
  }
  for (; nWidth > a; ++a) szReverse[a]='0';
  for (--a; a >= 0; --a, ++b) szStr[b]=szReverse[a];

  szStr[b]='\0';
  return szStr;
}
#endif

/********************************************************************
 *
 *  xitoa64W
 *
 *Converts int64 to unicode string.
 *
 *[in]   __int64 nNumber  -64-bit integer
 *[out]  wchar_t *wszStr  -unicode string number
 *[in]   int nWidth       -minimum number of characters to the output
 *
 *Returns: a pointer to unicode string
 *
 *Examples:
 *  xi64toaW(45, wszResult, 0);   //wszResult == L"45"
 *  xi64toaW(-45, wszResult, 0);  //wszResult == L"-45"
 *  xi64toaW(45, wszResult, 4);   //wszResult == L"0045"
 ********************************************************************/
#if defined xi64toaW || defined ALLCONVFUNC
#define xi64toaW_INCLUDED
#undef xi64toaW
wchar_t* xi64toaW(__int64 nNumber, wchar_t *wszStr, int nWidth)
{
  wchar_t wszReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    wszStr[0]='0';
    --nWidth;
    b=1;
  }
  else if (nNumber < 0)
  {
    wszStr[0]='-';
    nNumber=0 - nNumber;
    --nWidth;
    b=1;
  }
  for (a=0; nNumber != 0; ++a)
  {
    wszReverse[a]=(wchar_t)((nNumber % 10) + '0');
    nNumber=nNumber / 10;
  }
  for (; nWidth > a; ++a) wszReverse[a]='0';
  for (--a; a >= 0; --a, ++b) wszStr[b]=wszReverse[a];

  wszStr[b]='\0';
  return wszStr;
}
#endif

/********************************************************************
 *
 *  hex2decA
 *
 *Converts hex value to decimal.
 *
 *[in]  char *pStrHex   -hex value
 *
 *Returns: integer
 *         -1 wrong hex value
 *
 *Examples:
 *  hex2decA("A1F") == 2591;
 ********************************************************************/
#if defined hex2decA || defined ALLCONVFUNC
#define hex2decA_INCLUDED
#undef hex2decA
int hex2decA(char *pStrHex)
{
  int a;
  int b=0;

  while (1)
  {
    a=*pStrHex++;
    if (a >= '0' && a <= '9') a-='0';
    else if (a >= 'a' && a <= 'f') a-='a' - 10;
    else if (a >= 'A' && a <= 'F') a-='A' - 10;
    else return -1;

    if (*pStrHex) b=(b + a) * 16;
    else return (b + a);
  }
}
#endif

/********************************************************************
 *
 *  hex2decW
 *
 *Converts unicode hex value to decimal.
 *
 *[in]  wchar_t *wpStrHex   -unicode hex value
 *
 *Returns: integer
 *         -1 wrong hex value
 *
 *Examples:
 *  hex2decW(L"A1F") == 2591;
 ********************************************************************/
#if defined hex2decW || defined ALLCONVFUNC
#define hex2decW_INCLUDED
#undef hex2decW
int hex2decW(wchar_t *wpStrHex)
{
  int a;
  int b=0;

  while (1)
  {
    a=*wpStrHex++;
    if (a >= '0' && a <= '9') a-='0';
    else if (a >= 'a' && a <= 'f') a-='a' - 10;
    else if (a >= 'A' && a <= 'F') a-='A' - 10;
    else return -1;

    if (*wpStrHex) b=(b + a) * 16;
    else return (b + a);
  }
}
#endif

/********************************************************************
 *
 *  dec2hexA   [API: wsprintfA(szResult, "%02x", 2591)]
 *
 *Converts decimal to hex value.
 *
 *[in]   unsigned int nDec   -positive integer
 *[out]  char *szStrHex      -hex value (output)
 *[in]   unsigned int nWidth -minimum number of characters to the output
 *[in]   BOOL bLowerCase     -if TRUE hexadecimal value in lowercase
 *                            if FALSE in uppercase.
 *
 *Returns: copied chars
 *
 *Examples:
 *  dec2hexA(2591, szResult, 2, FALSE);   //szResult == "A1F"
 *  dec2hexA(10, szResult, 2, TRUE);      //szResult == "0a"
 ********************************************************************/
#if defined dec2hexA || defined ALLCONVFUNC
#define dec2hexA_INCLUDED
#undef dec2hexA
int dec2hexA(unsigned int nDec, char *szStrHex, unsigned int nWidth, BOOL bLowerCase)
{
  unsigned int a=nDec;
  unsigned int b=0;
  unsigned int c=0;
  char d;
  int nResult;

  while (a)
  {
    b=a % 16;
    a=a / 16;
    if (b < 10) szStrHex[c++]=b + '0';
    else
    {
      if (bLowerCase == TRUE)
        szStrHex[c++]=b + 'a' - 10;
      else
        szStrHex[c++]=b + 'A' - 10;
    }
  }
  while (nWidth > c) szStrHex[c++]='0';
  szStrHex[c]='\0';
  nResult=c;

  for (b=0, --c; b < c; d=szStrHex[b], szStrHex[b++]=szStrHex[c], szStrHex[c--]=d);

  return nResult;
}
#endif

/********************************************************************
 *
 *  dec2hexW   [API: wsprintfW(wszResult, L"%02x", 2591)]
 *
 *Converts decimal to unicode hex value.
 *
 *[in]   unsigned int nDec   -positive integer
 *[out]  wchar_t *wszStrHex  -unicode hex value (output)
 *[in]   unsigned int nWidth -minimum number of characters to the output
 *[in]   BOOL bLowerCase     -if TRUE hexadecimal value in lowercase
 *                            if FALSE in uppercase.
 *
 *Returns: copied chars
 *
 *Examples:
 *  dec2hexW(2591, wszResult, 2, FALSE);   //wszResult == L"A1F"
 *  dec2hexW(10, wszResult, 2, TRUE);      //wszResult == L"0a"
 ********************************************************************/
#if defined dec2hexW || defined ALLCONVFUNC
#define dec2hexW_INCLUDED
#undef dec2hexW
int dec2hexW(unsigned int nDec, wchar_t *wszStrHex, unsigned int nWidth, BOOL bLowerCase)
{
  unsigned int a=nDec;
  unsigned int b=0;
  unsigned int c=0;
  wchar_t d;
  int nResult;

  while (a)
  {
    b=a % 16;
    a=a / 16;
    if (b < 10) wszStrHex[c++]=b + '0';
    else
    {
      if (bLowerCase == TRUE)
        wszStrHex[c++]=b + 'a' - 10;
      else
        wszStrHex[c++]=b + 'A' - 10;
    }
  }
  while (nWidth > c) wszStrHex[c++]='0';
  wszStrHex[c]='\0';
  nResult=c;

  for (b=0, --c; b < c; d=wszStrHex[b], wszStrHex[b++]=wszStrHex[c], wszStrHex[c--]=d);

  return nResult;
}
#endif

/********************************************************************
 *
 *  str2hexA
 *
 *Converts string to hex values.
 *
 *[in]   unsigned char *pStr -string
 *[in]   int nBytes          -number of bytes in string
 *[out]  char *szStrHex      -hex string (output)
 *[in]   int nStrHexMax      -size of the hex string buffer
 *[in]   BOOL bLowerCase     -if TRUE hexadecimal value in lowercase
 *                            if FALSE in uppercase.
 *
 *Returns: copied chars
 *
 *Note:
 *  str2hexA uses dec2hexA
 *
 *Examples:
 *  str2hexA((unsigned char *)"Some Text", lstrlenA("Some Text"), szResult, MAX_PATH, TRUE);   //szResult == "536f6d652054657874"
 ********************************************************************/
#if defined str2hexA || defined ALLCONVFUNCS
#define str2hexA_INCLUDED
#undef str2hexA
int str2hexA(unsigned char *pStr, int nBytes, char *szStrHex, int nStrHexMax, BOOL bLowerCase)
{
  int a;
  int b;

  nStrHexMax-=3;

  for (a=0, b=0; a < nBytes && b <= nStrHexMax; ++a, b+=2)
  {
    //wsprintfA(szStrHex + b, "%02x", (unsigned int)pStr[a]);
    dec2hexA((unsigned int)pStr[a], szStrHex + b, 2, bLowerCase);
  }
  szStrHex[b]='\0';
  return b;
}
#endif

/********************************************************************
 *
 *  str2hexW
 *
 *Converts string to hex values.
 *
 *[in]   unsigned char *pStr -string
 *[in]   int nBytes          -number of bytes in string
 *[out]  wchar_t *wszStrHex  -hex string (output)
 *[in]   int nStrHexMax      -size of the hex string buffer
 *[in]   BOOL bLowerCase     -if TRUE hexadecimal value in lowercase
 *                            if FALSE in uppercase.
 *
 *Returns: copied chars
 *
 *Note:
 *  str2hexW uses dec2hexW
 *
 *Examples:
 *  str2hexW((unsigned char *)"Some Text", lstrlenA("Some Text"), wszResult, MAX_PATH, TRUE);   //wszResult == L"536f6d652054657874"
 ********************************************************************/
#if defined str2hexW || defined ALLCONVFUNCS
#define str2hexW_INCLUDED
#undef str2hexW
int str2hexW(unsigned char *pStr, int nBytes, wchar_t *wszStrHex, int nStrHexMax, BOOL bLowerCase)
{
  int a;
  int b;

  nStrHexMax-=3;

  for (a=0, b=0; a < nBytes && b <= nStrHexMax; ++a, b+=2)
  {
    //wsprintfW(wszStrHex + b, L"%02x", (unsigned int)pStr[a]);
    dec2hexW((unsigned int)pStr[a], wszStrHex + b, 2, bLowerCase);
  }
  wszStrHex[b]='\0';
  return b;
}
#endif

/********************************************************************
 *
 *  hex2strA
 *
 *Converts hex values to string.
 *
 *[in]   char *pStrHex        -hex string
 *[out]  unsigned char *szStr -string (output)
 *[in]   int nStrMax          -size of the string buffer
 *
 *Returns: copied bytes
 *
 *Note:
 *  hex2strA uses hex2decA
 *
 *Examples:
 *  hex2strA("536f6d652054657874", (unsigned char *)szResult, MAX_PATH);   //szResult == "Some Text"
 ********************************************************************/
#if defined hex2strA || defined ALLCONVFUNCS
#define hex2strA_INCLUDED
#undef hex2strA
int hex2strA(char *pStrHex, unsigned char *szStr, int nStrMax)
{
  char szHexChar[4];
  int nHexChar;
  int a;
  int b;

  for (a=0, b=0; pStrHex[a] && b < nStrMax; ++b)
  {
    szHexChar[0]=pStrHex[a++];
    if (!pStrHex[a]) break;
    szHexChar[1]=pStrHex[a++];
    szHexChar[2]='\0';

    if ((nHexChar=hex2decA(szHexChar)) >= 0)
      szStr[b]=nHexChar;
    else
      break;
  }
  return b;
}
#endif

/********************************************************************
 *
 *  hex2strW
 *
 *Converts hex values to string.
 *
 *[in]   wchar_t *wpStrHex    -hex string
 *[out]  unsigned char *szStr -string (output)
 *[in]   int nStrMax          -size of the string buffer
 *
 *Returns: copied bytes
 *
 *Note:
 *  hex2strW uses hex2decW
 *
 *Examples:
 *  hex2strW(L"536f6d652054657874", (unsigned char *)szResult, MAX_PATH);   //szResult == "Some Text"
 ********************************************************************/
#if defined hex2strW || defined ALLCONVFUNCS
#define hex2strW_INCLUDED
#undef hex2strW
int hex2strW(wchar_t *wpStrHex, unsigned char *szStr, int nStrMax)
{
  wchar_t wszHexChar[4];
  int nHexChar;
  int a;
  int b;

  for (a=0, b=0; wpStrHex[a] && b < nStrMax; ++b)
  {
    wszHexChar[0]=wpStrHex[a++];
    if (!wpStrHex[a]) break;
    wszHexChar[1]=wpStrHex[a++];
    wszHexChar[2]='\0';

    if ((nHexChar=hex2decW(wszHexChar)) >= 0)
      szStr[b]=nHexChar;
    else
      break;
  }
  return b;
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
#include "ConvFunc.h"

//insert functions
#define xatoiA
#define xitoaA
#include "ConvFunc.h"

void main()
{
  char szResult[MAX_PATH]="43";
  char *pResult;
  int nError;

  nError=xatoiA(szResult);
  printf("nError={%d}\n", nError);

  pResult=xitoaA(45, szResult, 0);
  printf("szResult={%s}, pResult={%s}\n", szResult, pResult);
}

*/

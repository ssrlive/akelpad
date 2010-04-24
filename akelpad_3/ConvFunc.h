/*****************************************************************
 *           Conversion functions header v2.6                    *
 *                                                               *
 * 2010 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Functions:                                                     *
 * xatoiA, xatoiW, xatoi64A, xatoi64W,                           *
 * xitoaA, xitoaW, xuitoaA, xuitoaW, xi64toaA, xi64toaW,         *
 * hex2decA, hex2decW, dec2hexA, dec2hexW                        *
 *                                                               *
 *Special functions:                                             *
 * bin2hexA, bin2hexW, hex2binA, hex2binW                        *
 *                                                               *
 *****************************************************************/

#ifndef _CONVFUNC_H_
#define _CONVFUNC_H_

int xatoiA(const char *pStr);
int xatoiW(const wchar_t *wpStr);
__int64 xatoi64A(const char *pStr);
__int64 xatoi64W(const wchar_t *wpStr);
int xitoaA(int nNumber, char *szStr, int nWidth);
int xitoaW(int nNumber, wchar_t *wszStr, int nWidth);
int xuitoaA(unsigned int nNumber, char *szStr, int nWidth);
int xuitoaW(unsigned int nNumber, wchar_t *wszStr, int nWidth);
int xi64toaA(__int64 nNumber, char *szStr, int nWidth);
int xi64toaW(__int64 nNumber, wchar_t *wszStr, int nWidth);
int hex2decA(const char *pStrHex);
int hex2decW(const wchar_t *wpStrHex);
int dec2hexA(unsigned int nDec, char *szStrHex, unsigned int nWidth, BOOL bLowerCase);
int dec2hexW(unsigned int nDec, wchar_t *wszStrHex, unsigned int nWidth, BOOL bLowerCase);

int bin2hexA(const unsigned char *pData, int nBytes, char *szStrHex, int nStrHexMax, BOOL bLowerCase);
int bin2hexW(const unsigned char *pData, int nBytes, wchar_t *wszStrHex, int nStrHexMax, BOOL bLowerCase);
int hex2binA(const char *pStrHex, unsigned char *pData, int nDataMax);
int hex2binW(const wchar_t *wpStrHex, unsigned char *pData, int nDataMax);

#endif

/********************************************************************
 *
 *  xatoiA
 *
 *Converts string to int.
 *
 *[in]  const char *pStr   -string number
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
int xatoiA(const char *pStr)
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
 *[in]  const wchar_t *wpStr   -string number
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
int xatoiW(const wchar_t *wpStr)
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
 *[in]  const char *pStr   -string number
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
__int64 xatoi64A(const char *pStr)
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
 *[in]  const wchar_t *wpStr   -unicode string number
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
__int64 xatoi64W(const wchar_t *wpStr)
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
 *Returns: copied digits
 *
 *Examples:
 *  xitoaA(45, szResult, 0);   //szResult == "45"
 *  xitoaA(-45, szResult, 0);  //szResult == "-45"
 *  xitoaA(45, szResult, 4);   //szResult == "0045"
 ********************************************************************/
#if defined xitoaA || defined ALLCONVFUNC
#define xitoaA_INCLUDED
#undef xitoaA
int xitoaA(int nNumber, char *szStr, int nWidth)
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
  return b;
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
 *Returns: copied digits
 *
 *Examples:
 *  xitoaW(45, wszResult, 0);   //wszResult == L"45"
 *  xitoaW(-45, wszResult, 0);  //wszResult == L"-45"
 *  xitoaW(45, wszResult, 4);   //wszResult == L"0045"
 ********************************************************************/
#if defined xitoaW || defined ALLCONVFUNC
#define xitoaW_INCLUDED
#undef xitoaW
int xitoaW(int nNumber, wchar_t *wszStr, int nWidth)
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
  return b;
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
 *Returns: copied digits
 *
 *Examples:
 *  xuitoaA(45, szResult, 0);   //szResult == "45"
 *  xuitoaA(45, szResult, 4);   //szResult == "0045"
 ********************************************************************/
#if defined xuitoaA || defined ALLCONVFUNC
#define xuitoaA_INCLUDED
#undef xuitoaA
int xuitoaA(unsigned int nNumber, char *szStr, int nWidth)
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
  return b;
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
 *Returns: copied digits
 *
 *Examples:
 *  xuitoaW(45, wszResult, 0);   //wszResult == L"45"
 *  xuitoaW(45, wszResult, 4);   //wszResult == L"0045"
 ********************************************************************/
#if defined xuitoaW || defined ALLCONVFUNC
#define xuitoaW_INCLUDED
#undef xuitoaW
int xuitoaW(unsigned int nNumber, wchar_t *wszStr, int nWidth)
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
  return b;
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
 *Returns: copied digits
 *
 *Examples:
 *  xi64toaA(45, szResult, 0);   //szResult == "45"
 *  xi64toaA(-45, szResult, 0);  //szResult == "-45"
 *  xi64toaA(45, szResult, 4);   //szResult == "0045"
 ********************************************************************/
#if defined xi64toaA || defined ALLCONVFUNC
#define xi64toaA_INCLUDED
#undef xi64toaA
int xi64toaA(__int64 nNumber, char *szStr, int nWidth)
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
  return b;
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
 *Returns: copied digits
 *
 *Examples:
 *  xi64toaW(45, wszResult, 0);   //wszResult == L"45"
 *  xi64toaW(-45, wszResult, 0);  //wszResult == L"-45"
 *  xi64toaW(45, wszResult, 4);   //wszResult == L"0045"
 ********************************************************************/
#if defined xi64toaW || defined ALLCONVFUNC
#define xi64toaW_INCLUDED
#undef xi64toaW
int xi64toaW(__int64 nNumber, wchar_t *wszStr, int nWidth)
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
  return b;
}
#endif

/********************************************************************
 *
 *  hex2decA
 *
 *Converts hex value to decimal.
 *
 *[in]  const char *pStrHex   -hex value
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
int hex2decA(const char *pStrHex)
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
 *[in]  const wchar_t *wpStrHex   -unicode hex value
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
int hex2decW(const wchar_t *wpStrHex)
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
 *  bin2hexA
 *
 *Converts binary data to hex string.
 *
 *[in]   const unsigned char *pData -binary data
 *[in]   int nBytes                 -number of bytes in pData
 *[out]  char *szStrHex             -output hex string buffer
 *[in]   int nStrHexMax             -size of the hex string buffer in TCHARs
 *[in]   BOOL bLowerCase            -if TRUE hexadecimal value in lowercase
 *                                   if FALSE in uppercase.
 *
 *Returns: copied chars
 *
 *Note:
 *  bin2hexA uses dec2hexA
 *
 *Examples:
 *  bin2hexA((unsigned char *)"Some Text", lstrlenA("Some Text"), szResult, MAX_PATH, TRUE);   //szResult == "536f6d652054657874"
 ********************************************************************/
#if defined bin2hexA || defined ALLCONVFUNC
#define bin2hexA_INCLUDED
#undef bin2hexA
int bin2hexA(const unsigned char *pData, int nBytes, char *szStrHex, int nStrHexMax, BOOL bLowerCase)
{
  int a;
  int b;

  nStrHexMax-=3;

  for (a=0, b=0; a < nBytes && b <= nStrHexMax; ++a, b+=2)
  {
    //wsprintfA(szStrHex + b, "%02x", (unsigned int)pData[a]);
    dec2hexA((unsigned int)pData[a], szStrHex + b, 2, bLowerCase);
  }
  szStrHex[b]='\0';
  return b;
}
#endif

/********************************************************************
 *
 *  bin2hexW
 *
 *Converts binary data to hex unicode string.
 *
 *[in]   const unsigned char *pData -binary data
 *[in]   int nBytes                 -number of bytes in pData
 *[out]  wchar_t *wszStrHex         -output hex string buffer
 *[in]   int nStrHexMax             -size of the hex string buffer in TCHARs
 *[in]   BOOL bLowerCase            -if TRUE hexadecimal value in lowercase
 *                                   if FALSE in uppercase.
 *
 *Returns: copied chars
 *
 *Note:
 *  bin2hexW uses dec2hexW
 *
 *Examples:
 *  bin2hexW((unsigned char *)"Some Text", lstrlenA("Some Text"), wszResult, MAX_PATH, TRUE);   //wszResult == L"536f6d652054657874"
 ********************************************************************/
#if defined bin2hexW || defined ALLCONVFUNC
#define bin2hexW_INCLUDED
#undef bin2hexW
int bin2hexW(const unsigned char *pData, int nBytes, wchar_t *wszStrHex, int nStrHexMax, BOOL bLowerCase)
{
  int a;
  int b;

  nStrHexMax-=3;

  for (a=0, b=0; a < nBytes && b <= nStrHexMax; ++a, b+=2)
  {
    //wsprintfW(wszStrHex + b, L"%02x", (unsigned int)pData[a]);
    dec2hexW((unsigned int)pData[a], wszStrHex + b, 2, bLowerCase);
  }
  wszStrHex[b]='\0';
  return b;
}
#endif

/********************************************************************
 *
 *  hex2binA
 *
 *Converts hex string to binary data.
 *
 *[in]   const char *pStrHex  -hex string
 *[out]  unsigned char *pData -output buffer, if NULL required buffer size returned
 *[in]   int nDataMax         -size of the output buffer in bytes
 *
 *Returns: copied bytes
 *
 *Note:
 *  hex2binA uses hex2decA
 *
 *Examples:
 *  hex2binA("536f6d652054657874", (unsigned char *)szResult, MAX_PATH);   //szResult == "Some Text"
 ********************************************************************/
#if defined hex2binA || defined ALLCONVFUNC
#define hex2binA_INCLUDED
#undef hex2binA
int hex2binA(const char *pStrHex, unsigned char *pData, int nDataMax)
{
  char szHexChar[4];
  int nHexChar;
  DWORD a;
  DWORD b;

  if (!pData) nDataMax=-1;

  for (a=0, b=0; pStrHex[a] && b < (DWORD)nDataMax; ++b)
  {
    szHexChar[0]=pStrHex[a++];
    if (!pStrHex[a]) break;
    szHexChar[1]=pStrHex[a++];
    szHexChar[2]='\0';

    if ((nHexChar=hex2decA(szHexChar)) >= 0)
    {
      if (pData) pData[b]=nHexChar;
    }
    else break;
  }
  return b;
}
#endif

/********************************************************************
 *
 *  hex2binW
 *
 *Converts hex unicode string to binary data.
 *
 *[in]   const wchar_t *wpStrHex -hex unicode string
 *[out]  unsigned char *pData    -output buffer, if NULL required buffer size returned
 *[in]   int nDataMax            -size of the output buffer in bytes
 *
 *Returns: copied bytes
 *
 *Note:
 *  hex2binW uses hex2decW
 *
 *Examples:
 *  hex2binW(L"536f6d652054657874", (unsigned char *)szResult, MAX_PATH);   //szResult == "Some Text"
 ********************************************************************/
#if defined hex2binW || defined ALLCONVFUNC
#define hex2binW_INCLUDED
#undef hex2binW
int hex2binW(const wchar_t *wpStrHex, unsigned char *pData, int nDataMax)
{
  wchar_t wszHexChar[4];
  int nHexChar;
  DWORD a;
  DWORD b;

  if (!pData) nDataMax=-1;

  for (a=0, b=0; wpStrHex[a] && b < (DWORD)nDataMax; ++b)
  {
    wszHexChar[0]=wpStrHex[a++];
    if (!wpStrHex[a]) break;
    wszHexChar[1]=wpStrHex[a++];
    wszHexChar[2]='\0';

    if ((nHexChar=hex2decW(wszHexChar)) >= 0)
    {
      if (pData) pData[b]=nHexChar;
    }
    else break;
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

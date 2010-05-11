/*****************************************************************
 *              String functions header v4.0                     *
 *                                                               *
 * 2010 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *                                                               *
 *                                                               *
 *Functions:                                                     *
 * WideCharLower, WideCharUpper, xmemcpy, xmemcmp, xmemset,      *
 * xstrcmpA, xstrcmpW, xstrcmpiA, xstrcmpiW,                     *
 * xstrcmpnA, xstrcmpnW, xstrcmpinA, xstrcmpinW,                 *
 * xstrcpyA, xstrcpyW, xstrcpynA, xstrcpynW,                     *
 * xstrstrA, xstrstrW, xstrrepA, xstrrepW                        *
 *                                                               *
 * xatoiA, xatoiW, xatoi64A, xatoi64W,                           *
 * xitoaA, xitoaW, xuitoaA, xuitoaW, xi64toaA, xi64toaW,         *
 * hex2decA, hex2decW, dec2hexA, dec2hexW                        *
 *                                                               *
 *Special functions:                                             *
 * bin2hexA, bin2hexW, hex2binA, hex2binW, xprintfA, xprintfW    *
 *****************************************************************/

#ifndef _STRFUNC_H_
#define _STRFUNC_H_

wchar_t WideCharLower(wchar_t c);
wchar_t WideCharUpper(wchar_t c);
void* xmemcpy(void *dest, const void *src, unsigned int count);
int xmemcmp(const void *buf1, const void *buf2, unsigned int count);
void* xmemset(void *dest, int c, unsigned int count);
int xstrcmpA(const char *pString1, const char *pString2);
int xstrcmpW(const wchar_t *wpString1, const wchar_t *wpString2);
int xstrcmpiA(const char *pString1, const char *pString2);
int xstrcmpiW(const wchar_t *wpString1, const wchar_t *wpString2);
int xstrcmpnA(const char *pString1, const char *pString2, DWORD dwMaxLength);
int xstrcmpnW(const wchar_t *wpString1, const wchar_t *wpString2, DWORD dwMaxLength);
int xstrcmpinA(const char *pString1, const char *pString2, DWORD dwMaxLength);
int xstrcmpinW(const wchar_t *wpString1, const wchar_t *wpString2, DWORD dwMaxLength);
int xstrcpyA(char *pString1, const char *pString2);
int xstrcpyW(wchar_t *wpString1, const wchar_t *wpString2);
int xstrcpynA(char *pString1, const char *pString2, unsigned int nMaxLength);
int xstrcpynW(wchar_t *wpString1, const wchar_t *wpString2, unsigned int nMaxLength);
BOOL xstrstrA(const char *pText, DWORD dwTextLen, const char *pStr, BOOL bSensitive, char **pStrBegin, char **pStrEnd);
BOOL xstrstrW(const wchar_t *wpText, DWORD dwTextLen, const wchar_t *wpStr, BOOL bSensitive, wchar_t **wpStrBegin, wchar_t **wpStrEnd);
int xstrrepA(const char *pText, const char *pIt, const char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult);
int xstrrepW(const wchar_t *wpText, const wchar_t *wpIt, const wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult);

int xatoiA(const char *pStr, const char **pNext);
int xatoiW(const wchar_t *wpStr, const wchar_t **wpNext);
__int64 xatoi64A(const char *pStr, const char **pNext);
__int64 xatoi64W(const wchar_t *wpStr, const wchar_t **wpNext);
int xitoaA(int nNumber, char *szStr);
int xitoaW(int nNumber, wchar_t *wszStr);
int xuitoaA(unsigned int nNumber, char *szStr);
int xuitoaW(unsigned int nNumber, wchar_t *wszStr);
int xi64toaA(__int64 nNumber, char *szStr);
int xi64toaW(__int64 nNumber, wchar_t *wszStr);
int hex2decA(const char *pStrHex);
int hex2decW(const wchar_t *wpStrHex);
int dec2hexA(unsigned int nDec, char *szStrHex, unsigned int nWidth, BOOL bLowerCase);
int dec2hexW(unsigned int nDec, wchar_t *wszStrHex, unsigned int nWidth, BOOL bLowerCase);

int bin2hexA(const unsigned char *pData, int nBytes, char *szStrHex, int nStrHexMax, BOOL bLowerCase);
int bin2hexW(const unsigned char *pData, int nBytes, wchar_t *wszStrHex, int nStrHexMax, BOOL bLowerCase);
int hex2binA(const char *pStrHex, unsigned char *pData, int nDataMax);
int hex2binW(const wchar_t *wpStrHex, unsigned char *pData, int nDataMax);
int xprintfA(char *szOutput, const char *pFormat, ...);
int xprintfW(wchar_t *wszOutput, const wchar_t *wpFormat, ...);

#endif


/********************************************************************
 *
 *  WideCharLower
 *
 *Translate wide character to lowercase.
 *
 * [in] wchar_t c  Unicode character.
 *
 *Returns: lowercase unicode character.
 ********************************************************************/
#if defined WideCharLower || defined ALLSTRFUNC
#define WideCharLower_INCLUDED
#undef WideCharLower
wchar_t WideCharLower(wchar_t c)
{
  if (c < 0x100)
  {
    if ((c >= 0x0041 && c <= 0x005a) ||
        (c >= 0x00c0 && c <= 0x00de))
      return (c + 0x20);

    if (c == 0x00b5)
      return 0x03bc;

    return c;
  }
  else if (c < 0x300)
  {
    if ((c >= 0x0100 && c <= 0x012e) ||
        (c >= 0x0132 && c <= 0x0136) ||
        (c >= 0x014a && c <= 0x0176) ||
        (c >= 0x01de && c <= 0x01ee) ||
        (c >= 0x01f8 && c <= 0x021e) ||
        (c >= 0x0222 && c <= 0x0232))
    {
      if (!(c & 0x01))
        return (c + 1);
      return c;
    }

    if ((c >= 0x0139 && c <= 0x0147) ||
        (c >= 0x01cd && c <= 0x91db))
    {
      if (c & 0x01)
        return (c + 1);
      return c;
    }

    if (c >= 0x178 && c <= 0x01f7)
    {
      wchar_t k;

      switch (c)
      {
        case 0x0178:
          k=0x00ff;
          break;
        case 0x0179:
        case 0x017b:
        case 0x017d:
        case 0x0182:
        case 0x0184:
        case 0x0187:
        case 0x018b:
        case 0x0191:
        case 0x0198:
        case 0x01a0:
        case 0x01a2:
        case 0x01a4:
        case 0x01a7:
        case 0x01ac:
        case 0x01af:
        case 0x01b3:
        case 0x01b5:
        case 0x01b8:
        case 0x01bc:
        case 0x01c5:
        case 0x01c8:
        case 0x01cb:
        case 0x01cd:
        case 0x01cf:
        case 0x01d1:
        case 0x01d3:
        case 0x01d5:
        case 0x01d7:
        case 0x01d9:
        case 0x01db:
        case 0x01f2:
        case 0x01f4:
          k=c + 1;
          break;
        case 0x017f:
          k=0x0073;
          break;
        case 0x0181:
          k=0x0253;
          break;
        case 0x0186:
          k=0x0254;
          break;
        case 0x0189:
          k=0x0256;
          break;
        case 0x018a:
          k=0x0257;
          break;
        case 0x018e:
          k=0x01dd;
          break;
        case 0x018f:
          k=0x0259;
          break;
        case 0x0190:
          k=0x025b;
          break;
        case 0x0193:
          k=0x0260;
          break;
        case 0x0194:
          k=0x0263;
          break;
        case 0x0196:
          k=0x0269;
          break;
        case 0x0197:
          k=0x0268;
          break;
        case 0x019c:
          k=0x026f;
          break;
        case 0x019d:
          k=0x0272;
          break;
        case 0x019f:
          k=0x0275;
          break;
        case 0x01a6:
          k=0x0280;
          break;
        case 0x01a9:
          k=0x0283;
          break;
        case 0x01ae:
          k=0x0288;
          break;
        case 0x01b1:
          k=0x028a;
          break;
        case 0x01b2:
          k=0x028b;
          break;
        case 0x01b7:
          k=0x0292;
          break;
        case 0x01c4:
        case 0x01c7:
        case 0x01ca:
        case 0x01f1:
          k=c + 2;
          break;
        case 0x01f6:
          k=0x0195;
          break;
        case 0x01f7:
          k=0x01bf;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
    if (c == 0x0220)
      return 0x019e;
  }
  else if (c < 0x0400)
  {
    if (c >= 0x0391 && c <= 0x03ab && c != 0x03a2)
      return (c + 0x20);
    if (c >= 0x03d8 && c <= 0x03ee && !(c & 0x01))
      return (c + 1);
    if (c >= 0x0386 && c <= 0x03f5)
    {
      wchar_t k;

      switch (c)
      {
        case 0x0386:
          k=0x03ac;
          break;
        case 0x0388:
          k=0x03ad;
          break;
        case 0x0389:
          k=0x03ae;
          break;
        case 0x038a:
          k=0x03af;
          break;
        case 0x038c:
          k=0x03cc;
          break;
        case 0x038e:
          k=0x03cd;
          break;
        case 0x038f:
          k=0x038f;
          break;
        case 0x03c2:
          k=0x03c3;
          break;
        case 0x03d0:
          k=0x03b2;
          break;
        case 0x03d1:
          k=0x03b8;
          break;
        case 0x03d5:
          k=0x03c6;
          break;
        case 0x03d6:
          k=0x03c0;
          break;
        case 0x03f0:
          k=0x03ba;
          break;
        case 0x03f1:
          k=0x03c1;
          break;
        case 0x03f2:
          k=0x03c3;
          break;
        case 0x03f4:
          k=0x03b8;
          break;
        case 0x03f5:
          k=0x03b5;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
    if (c == 0x0345)
      return 0x03b9;
  }
  else if (c < 0x500)
  {
    if (c >= 0x0400 && c <= 0x040f)
      return (c + 0x50);

    if (c >= 0x0410 && c <= 0x042f)
      return (c + 0x20);

    if ((c >= 0x0460 && c <= 0x0480) ||
        (c >= 0x048a && c <= 0x04be) ||
        (c >= 0x04d0 && c <= 0x04f4) ||
        (c == 0x04f8))
    {
      if (!(c & 0x01))
        return (c + 1);
      return c;
    }

    if (c >= 0x04c1 && c <= 0x04cd)
    {
      if (c & 0x01)
        return (c + 1);
      return c;
    }
  }
  else if (c < 0x1f00)
  {
    if ((c >= 0x0500 && c <= 0x050e) ||
        (c >= 0x1e00 && c <= 0x1e94) ||
        (c >= 0x1ea0 && c <= 0x1ef8))
    {
      if (!(c & 0x01))
        return (c + 1);
      return c;
    }

    if (c >= 0x0531 && c <= 0x0556)
      return (c + 0x30);

    if (c == 0x1e9b)
      return 0x1e61;
  }
  else if (c < 0x2000)
  {
    if ((c >= 0x1f08 && c <= 0x1f0f) ||
        (c >= 0x1f18 && c <= 0x1f1d) ||
        (c >= 0x1f28 && c <= 0x1f2f) ||
        (c >= 0x1f38 && c <= 0x1f3f) ||
        (c >= 0x1f48 && c <= 0x1f4d) ||
        (c >= 0x1f68 && c <= 0x1f6f) ||
        (c >= 0x1f88 && c <= 0x1f8f) ||
        (c >= 0x1f98 && c <= 0x1f9f) ||
        (c >= 0x1fa8 && c <= 0x1faf))
      return (c - 0x08);

    if (c >= 0x1f59 && c <= 0x1f5f)
    {
      if (c & 0x01)
        return (c - 0x08);
      return c;
    }

    if (c >= 0x1fb8 && c <= 0x1ffc)
    {
      wchar_t k;

      switch (c)
      {
        case 0x1fb8:
        case 0x1fb9:
        case 0x1fd8:
        case 0x1fd9:
        case 0x1fe8:
        case 0x1fe9:
          k=c - 0x08;
          break;
        case 0x1fba:
        case 0x1fbb:
          k=c - 0x4a;
          break;
        case 0x1fbc:
          k=0x1fb3;
          break;
        case 0x1fbe:
          k=0x03b9;
          break;
        case 0x1fc8:
        case 0x1fc9:
        case 0x1fca:
        case 0x1fcb:
          k=c - 0x56;
          break;
        case 0x1fcc:
          k=0x1fc3;
          break;
        case 0x1fda:
        case 0x1fdb:
          k=c - 0x64;
          break;
        case 0x1fea:
        case 0x1feb:
          k=c - 0x70;
          break;
        case 0x1fec:
          k=0x1fe5;
          break;
        case 0x1ffa:
        case 0x1ffb:
          k=c - 0x7e;
          break;
        case 0x1ffc:
          k=0x1ff3;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else
  {
    if (c >= 0x2160 && c <= 0x216f)
      return (c + 0x10);

    if (c >= 0x24b6 && c <= 0x24cf)
      return (c + 0x1a);

    if (c >= 0xff21 && c <= 0xff3a)
      return (c + 0x20);

    //if (c >= 0x10400 && c <= 0x10425)
    //  return (c + 0x28);

    if (c == 0x2126)
      return 0x03c9;
    if (c == 0x212a)
      return 0x006b;
    if (c == 0x212b)
      return 0x00e5;
  }
  return c;
}
#endif

/********************************************************************
 *
 *  WideCharUpper
 *
 *Translate wide character to uppercase.
 *
 * [in] wchar_t c  Unicode character.
 *
 *Returns: uppercase unicode character.
 ********************************************************************/
#if defined WideCharUpper || defined ALLSTRFUNC
#define WideCharUpper_INCLUDED
#undef WideCharUpper
wchar_t WideCharUpper(wchar_t c)
{
  if (c < 0x100)
  {
    if ((c >= 0x0061 && c <= 0x007a) ||
        (c >= 0x00e0 && c <= 0x00fe))
      return (c - 0x20);

    if (c == 0x00b5)
      return 0x039c;

    if (c == 0xff)
      return 0x0178;

    return c;
  }
  else if (c < 0x300)
  {
    if ((c >= 0x0101 && c <= 0x012f) ||
        (c >= 0x0133 && c <= 0x0137) ||
        (c >= 0x014b && c <= 0x0177) ||
        (c >= 0x01df && c <= 0x01ef) ||
        (c >= 0x01f9 && c <= 0x021f) ||
        (c >= 0x0223 && c <= 0x0233))
    {
      if (c & 0x01)
        return (c - 1);
      return c;
    }

    if ((c >= 0x013a && c <= 0x0148) ||
        (c >= 0x01ce && c <= 0x1dc))
    {
      if (!(c & 0x01))
        return (c - 1);
      return c;
    }

    if (c == 0x0131)
      return 0x0049;

    if (c == 0x017a || c == 0x017c || c == 0x017e)
      return (c - 1);

    if (c >= 0x017f && c <= 0x0292)
    {
      wchar_t k;

      switch (c)
      {
        case 0x017f:
          k=0x0053;
          break;
        case 0x0183:
          k=0x0182;
          break;
        case 0x0185:
          k=0x0184;
          break;
        case 0x0188:
          k=0x0187;
          break;
        case 0x018c:
          k=0x018b;
          break;
        case 0x0192:
          k=0x0191;
          break;
        case 0x0195:
          k=0x01f6;
          break;
        case 0x0199:
          k=0x0198;
          break;
        case 0x019e:
          k=0x0220;
          break;
        case 0x01a1:
        case 0x01a3:
        case 0x01a5:
        case 0x01a8:
        case 0x01ad:
        case 0x01b0:
        case 0x01b4:
        case 0x01b6:
        case 0x01b9:
        case 0x01bd:
        case 0x01c5:
        case 0x01c8:
        case 0x01cb:
        case 0x01f2:
        case 0x01f5:
          k=c - 1;
          break;
        case 0x01bf:
          k=0x01f7;
          break;
        case 0x01c6:
        case 0x01c9:
        case 0x01cc:
          k=c - 2;
          break;
        case 0x01dd:
          k=0x018e;
          break;
        case 0x01f3:
          k=0x01f1;
          break;
        case 0x0253:
          k=0x0181;
          break;
        case 0x0254:
          k=0x0186;
          break;
        case 0x0256:
          k=0x0189;
          break;
        case 0x0257:
          k=0x018a;
          break;
        case 0x0259:
          k=0x018f;
          break;
        case 0x025b:
          k=0x0190;
          break;
        case 0x0260:
          k=0x0193;
          break;
        case 0x0263:
          k=0x0194;
          break;
        case 0x0268:
          k=0x0197;
          break;
        case 0x0269:
          k=0x0196;
          break;
        case 0x026f:
          k=0x019c;
          break;
        case 0x0272:
          k=0x019d;
          break;
        case 0x0275:
          k=0x019f;
          break;
        case 0x0280:
          k=0x01a6;
          break;
        case 0x0283:
          k=0x01a9;
          break;
        case 0x0288:
          k=0x01ae;
          break;
        case 0x028a:
          k=0x01b1;
          break;
        case 0x028b:
          k=0x01b2;
          break;
        case 0x0292:
          k=0x01b7;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else if (c < 0x0400)
  {
    if (c == 0x03ac)
      return 0x0386;

    if ((c & 0xfff0) == 0x03a0 && c >= 0x03ad)
      return (c - 0x15);

    if (c >= 0x03b1 && c <= 0x03cb && c != 0x03c2)
      return (c - 0x20);

    if (c == 0x03c2)
      return 0x03a3;

    if (c >= 0x03cc && c <= 0x03f5)
    {
      wchar_t k;

      switch (c)
      {
        case 0x03cc:
          k=0x038c;
          break;
        case 0x03cd:
        case 0x03ce:
          k=c - 0x3f;
          break;
        case 0x03d0:
          k=0x0392;
          break;
        case 0x03d1:
          k=0x0398;
          break;
        case 0x03d5:
          k=0x03a6;
          break;
        case 0x03d6:
          k=0x03a0;
          break;
        case 0x03d9:
        case 0x03db:
        case 0x03dd:
        case 0x03df:
        case 0x03e1:
        case 0x03e3:
        case 0x03e5:
        case 0x03e7:
        case 0x03e9:
        case 0x03eb:
        case 0x03ed:
        case 0x03ef:
          k=c - 1;
          break;
        case 0x03f0:
          k=0x039a;
          break;
        case 0x03f1:
          k=0x03a1;
          break;
        case 0x03f2:
          k=0x03a3;
          break;
        case 0x03f5:
          k=0x0395;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else if (c < 0x500)
  {
    if (c >= 0x0450 && c <= 0x045f)
      return (c - 0x50);

    if (c >= 0x0430 && c <= 0x044f)
      return (c - 0x20);

    if ((c >= 0x0461 && c <= 0x0481) ||
        (c >= 0x048b && c <= 0x04bf) ||
        (c >= 0x04d1 && c <= 0x04f5))
    {
      if (c & 0x01)
        return (c - 1);
      return c;
    }

    if (c >= 0x04c2 && c <= 0x04ce)
    {
      if (!(c & 0x01))
        return (c - 1);
      return c;
    }

    if (c == 0x04f9)
      return 0x04f8;
  }
  else if (c < 0x1f00)
  {
    if ((c >= 0x0501 && c <= 0x050f) ||
        (c >= 0x1e01 && c <= 0x1e95) ||
        (c >= 0x1ea1 && c <= 0x1ef9))
    {
      if (c & 0x01)
        return (c - 1);
      return c;
    }

    if (c >= 0x0561 && c <= 0x0586)
      return (c - 0x30);

    if (c == 0x1e9b)
      return 0x1e60;
  }
  else if (c < 0x2000)
  {
    if ((c >= 0x1f00 && c <= 0x1f07) ||
        (c >= 0x1f10 && c <= 0x1f15) ||
        (c >= 0x1f20 && c <= 0x1f27) ||
        (c >= 0x1f30 && c <= 0x1f37) ||
        (c >= 0x1f40 && c <= 0x1f45) ||
        (c >= 0x1f60 && c <= 0x1f67) ||
        (c >= 0x1f80 && c <= 0x1f87) ||
        (c >= 0x1f90 && c <= 0x1f97) ||
        (c >= 0x1fa0 && c <= 0x1fa7))
      return (c + 0x08);

    if (c >= 0x1f51 && c <= 0x1f57 && (c & 0x01))
      return (c + 0x08);

    if (c >= 0x1f70 && c <= 0x1ff3)
    {
      wchar_t k;

      switch (c)
      {
        case 0x1fb0:
          k=0x1fb8;
          break;
        case 0x1fb1:
          k=0x1fb9;
          break;
        case 0x1f70:
          k=0x1fba;
          break;
        case 0x1f71:
          k=0x1fbb;
          break;
        case 0x1fb3:
          k=0x1fbc;
          break;
        case 0x1fbe:
          k=0x0399;
          break;
        case 0x1f72:
          k=0x1fc8;
          break;
        case 0x1f73:
          k=0x1fc9;
          break;
        case 0x1f74:
          k=0x1fca;
          break;
        case 0x1f75:
          k=0x1fcb;
          break;
        case 0x1fd0:
          k=0x1fd8;
          break;
        case 0x1fd1:
          k=0x1fd9;
          break;
        case 0x1f76:
          k=0x1fda;
          break;
        case 0x1f77:
          k=0x1fdb;
          break;
        case 0x1fe0:
          k=0x1fe8;
          break;
        case 0x1fe1:
          k=0x1fe9;
          break;
        case 0x1f7a:
          k=0x1fea;
          break;
        case 0x1f7b:
          k=0x1feb;
          break;
        case 0x1fe5:
          k=0x1fec;
          break;
        case 0x1f78:
          k=0x1ff8;
          break;
        case 0x1f79:
          k=0x1ff9;
          break;
        case 0x1f7c:
          k=0x1ffa;
          break;
        case 0x1f7d:
          k=0x1ffb;
          break;
        case 0x1ff3:
          k=0x1ffc;
          break;
        default:
          k=0;
      }
      if (k != 0)
        return k;
    }
  }
  else
  {
    if (c >= 0x2170 && c <= 0x217f)
      return (c - 0x10);

    if (c >= 0x24d0 && c <= 0x24e9)
      return (c - 0x1a);

    if (c >= 0xff41 && c <= 0xff5a)
      return (c - 0x20);

//    if (c >= 0x10428 && c <= 0x1044d)
//      return (c - 0x28);
  }
  return c;
}
#endif

/********************************************************************
 *
 *  xmemcpy
 *
 *Copies characters between buffers.
 *
 *[out] void *dest          New buffer.
 * [in] const void *src     Buffer to copy from.
 * [in] unsigned int count  Number of bytes to copy.
 *
 *Returns:  the value of dest.
 ********************************************************************/
#if defined xmemcpy || defined ALLSTRFUNC
#define xmemcpy_INCLUDED
#undef xmemcpy
void* xmemcpy(void *dest, const void *src, unsigned int count)
{
  unsigned char *byte_dest=(unsigned char *)dest;
  unsigned char *byte_src=(unsigned char *)src;

  if (byte_dest != byte_src)
    while (count--) *byte_dest++=*byte_src++;

  return dest;
}
#endif

/********************************************************************
 *
 *  xmemcmp
 *
 *Compare characters in two buffers.
 *
 *[in] const void *buf1    First buffer.
 *[in] const void *buf2    Second buffer.
 *[in] unsigned int count  Number of bytes.
 *
 *Returns:  -1 buf1 less than buf2.
 *           0 buf1 identical to buf2.
 *           1 buf1 greater than buf2.
 ********************************************************************/
#if defined xmemcmp || defined ALLSTRFUNC
#define xmemcmp_INCLUDED
#undef xmemcmp
int xmemcmp(const void *buf1, const void *buf2, unsigned int count)
{
  unsigned char *byte_buf1=(unsigned char *)buf1;
  unsigned char *byte_buf2=(unsigned char *)buf2;

  if (byte_buf1 != byte_buf2)
  {
    while (count--)
    {
      if (*byte_buf1 == *byte_buf2)
      {
        ++byte_buf1;
        ++byte_buf2;
        continue;
      }
      else if (*byte_buf1 < *byte_buf2)
        return -1;
      else
        return 1;
    }
  }
  return 0;
}
#endif

/********************************************************************
 *
 *  xmemset
 *
 *Compare characters in two buffers.
 *
 *[out] void *dest          Pointer to destination.
 * [in] int c               Character to set.
 * [in] unsigned int count  Number of characters.
 *
 *Returns:  the value of dest.
 ********************************************************************/
#if defined xmemset || defined ALLSTRFUNC
#define xmemset_INCLUDED
#undef xmemset
void* xmemset(void *dest, int c, unsigned int count)
{
  unsigned char *byte_dest=(unsigned char *)dest;

  while (count--) *byte_dest++=c;

  return dest;
}
#endif

/********************************************************************
 *
 *  xstrcmpA
 *
 *Case sensitive comparison of two strings.
 *
 *[in] char *pString1  First string to compare.
 *[in] char *pString2  Second string to compare.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 ********************************************************************/
#if defined xstrcmpA || defined ALLSTRFUNC
#define xstrcmpA_INCLUDED
#undef xstrcmpA
int xstrcmpA(const char *pString1, const char *pString2)
{
  while (*pString1)
  {
    if (*pString1 != *pString2)
      return ((DWORD)*pString1 > (DWORD)*pString2)?1:-1;

    ++pString1;
    ++pString2;
  }

  if (*pString1 == *pString2)
    return 0;
  return -1;
}
#endif

/********************************************************************
 *
 *  xstrcmpW
 *
 *Case sensitive comparison of two unicode strings.
 *
 *[in] wchar_t *wpString1  First string to compare.
 *[in] wchar_t *wpString2  Second string to compare.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 ********************************************************************/
#if defined xstrcmpW || defined ALLSTRFUNC
#define xstrcmpW_INCLUDED
#undef xstrcmpW
int xstrcmpW(const wchar_t *wpString1, const wchar_t *wpString2)
{
  while (*wpString1)
  {
    if (*wpString1 != *wpString2)
      return ((DWORD)*wpString1 > (DWORD)*wpString2)?1:-1;

    ++wpString1;
    ++wpString2;
  }

  if (*wpString1 == *wpString2)
    return 0;
  return -1;
}
#endif

/********************************************************************
 *
 *  xstrcmpiA
 *
 *Case insensitive comparison of two strings.
 *
 *[in] char *pString1  First string to compare.
 *[in] char *pString2  Second string to compare.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 ********************************************************************/
#if defined xstrcmpiA || defined ALLSTRFUNC
#define xstrcmpiA_INCLUDED
#undef xstrcmpiA
int xstrcmpiA(const char *pString1, const char *pString2)
{
  int nCompare;

  while (*pString1)
  {
    if (nCompare=(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString1) - (WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString2))
      return (nCompare > 0)?1:-1;

    ++pString1;
    ++pString2;
  }

  if (*pString1 == *pString2)
    return 0;
  return -1;
}
#endif

/********************************************************************
 *
 *  xstrcmpiW
 *
 *Case insensitive comparison of two unicode strings.
 *
 *[in] wchar_t *wpString1  First string to compare.
 *[in] wchar_t *wpString2  Second string to compare.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 *
 *Note:
 *  xstrcmpiW can be used on Win95/98/Me if WideCharLower or WideCharUpper defined.
 ********************************************************************/
#if defined xstrcmpiW || defined ALLSTRFUNC
#define xstrcmpiW_INCLUDED
#undef xstrcmpiW
int xstrcmpiW(const wchar_t *wpString1, const wchar_t *wpString2)
{
  int nCompare;

  while (*wpString1)
  {
    #if defined WideCharLower_INCLUDED
      if (nCompare=WideCharLower(*wpString1) - WideCharLower(*wpString2))
        return (nCompare > 0)?1:-1;
    #elif defined WideCharUpper_INCLUDED
      if (nCompare=WideCharUpper(*wpString1) - WideCharUpper(*wpString2))
        return (nCompare > 0)?1:-1;
    #else
      if (nCompare=(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString1) - (WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString2))
        return (nCompare > 0)?1:-1;
    #endif

    ++wpString1;
    ++wpString2;
  }

  if (*wpString1 == *wpString2)
    return 0;
  return -1;
}
#endif

/********************************************************************
 *
 *  xstrcmpnA
 *
 *Case sensitive comparison specified number of characters of two strings.
 *
 *[in] char *pString1     First string to compare.
 *[in] char *pString2     Second string to compare.
 *[in] DWORD dwMaxLength  Number of characters to compare,
 *                         -1 compare until NULL character in pString1.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 ********************************************************************/
#if defined xstrcmpnA || defined ALLSTRFUNC
#define xstrcmpnA_INCLUDED
#undef xstrcmpnA
int xstrcmpnA(const char *pString1, const char *pString2, DWORD dwMaxLength)
{
  DWORD dwCount=dwMaxLength;

  while (dwCount && *pString1)
  {
    if (*pString1 != *pString2)
      return ((DWORD)*pString1 > (DWORD)*pString2)?1:-1;
    ++pString1;
    ++pString2;
    --dwCount;
  }
  if (!dwCount) return 0;
  if (!*pString2) return 0;
  return (dwMaxLength == (DWORD)-1)?0:-1;
}
#endif

/********************************************************************
 *
 *  xstrcmpnW
 *
 *Case sensitive comparison specified number of characters of two unicode strings.
 *
 *[in] wchar_t *wpString1  First string to compare.
 *[in] wchar_t *wpString2  Second string to compare.
 *[in] DWORD dwMaxLength   Number of characters to compare,
 *                          -1 compare until NULL character in wpString1.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 ********************************************************************/
#if defined xstrcmpnW || defined ALLSTRFUNC
#define xstrcmpnW_INCLUDED
#undef xstrcmpnW
int xstrcmpnW(const wchar_t *wpString1, const wchar_t *wpString2, DWORD dwMaxLength)
{
  DWORD dwCount=dwMaxLength;

  while (dwCount && *wpString1)
  {
    if (*wpString1 != *wpString2)
      return ((DWORD)*wpString1 > (DWORD)*wpString2)?1:-1;

    ++wpString1;
    ++wpString2;
    --dwCount;
  }
  if (!dwCount) return 0;
  if (!*wpString2) return 0;
  return (dwMaxLength == (DWORD)-1)?0:-1;
}
#endif

/********************************************************************
 *
 *  xstrcmpinA
 *
 *Case insensitive comparison specified number of characters of two strings.
 *
 *[in] char *pString1     First string to compare.
 *[in] char *pString2     Second string to compare.
 *[in] DWORD dwMaxLength  Number of characters to compare,
 *                         -1 compare until NULL character in pString1.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 ********************************************************************/
#if defined xstrcmpinA || defined ALLSTRFUNC
#define xstrcmpinA_INCLUDED
#undef xstrcmpinA
int xstrcmpinA(const char *pString1, const char *pString2, DWORD dwMaxLength)
{
  DWORD dwCount=dwMaxLength;
  int nCompare;

  while (dwCount && *pString1)
  {
    if (nCompare=(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString1) - (WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pString2))
      return (nCompare > 0)?1:-1;

    ++pString1;
    ++pString2;
    --dwCount;
  }
  if (!dwCount) return 0;
  if (!*pString2) return 0;
  return (dwMaxLength == (DWORD)-1)?0:-1;
}
#endif

/********************************************************************
 *
 *  xstrcmpinW
 *
 *Case insensitive comparison specified number of characters of two unicode strings.
 *
 *[in] wchar_t *wpString1  First string to compare.
 *[in] wchar_t *wpString2  Second string to compare.
 *[in] DWORD dwMaxLength   Number of characters to compare,
 *                          -1 compare until NULL character in wpString1.
 *
 *Returns:  -1 string1 less than string2.
 *           0 string1 identical to string2.
 *           1 string1 greater than string2.
 *
 *Note:
 *  xstrcmpinW can be used on Win95/98/Me if WideCharLower or WideCharUpper defined.
 ********************************************************************/
#if defined xstrcmpinW || defined ALLSTRFUNC
#define xstrcmpinW_INCLUDED
#undef xstrcmpinW
int xstrcmpinW(const wchar_t *wpString1, const wchar_t *wpString2, DWORD dwMaxLength)
{
  DWORD dwCount=dwMaxLength;
  int nCompare;

  while (dwCount && *wpString1)
  {
    #if defined WideCharLower_INCLUDED
      if (nCompare=WideCharLower(*wpString1) - WideCharLower(*wpString2))
        return (nCompare > 0)?1:-1;
    #elif defined WideCharUpper_INCLUDED
      if (nCompare=WideCharUpper(*wpString1) - WideCharUpper(*wpString2))
        return (nCompare > 0)?1:-1;
    #else
      if (nCompare=(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString1) - (WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpString2))
        return (nCompare > 0)?1:-1;
    #endif

    ++wpString1;
    ++wpString2;
    --dwCount;
  }
  if (!dwCount) return 0;
  if (!*wpString2) return 0;
  return (dwMaxLength == (DWORD)-1)?0:-1;
}
#endif

/********************************************************************
 *
 *  xstrcpyA
 *
 *Copies a string into a buffer.
 *
 *[in] char *pString1  Pointer to a buffer into which the function copies characters.
 *                      The buffer must be large enough to contain the string,
 *                      including the terminating null character.
 *[in] char *pString2  Pointer to a null-terminated string from which the function copies characters.
 *
 *Returns:  number of characters copied, not including the terminating null character.
 ********************************************************************/
#if defined xstrcpyA || defined ALLSTRFUNC
#define xstrcpyA_INCLUDED
#undef xstrcpyA
int xstrcpyA(char *pString1, const char *pString2)
{
  char *pDest=pString1;
  char *pSrc=(char *)pString2;

  if (pDest != pSrc)
  {
    while (*pSrc)
      *pDest++=*pSrc++;
    *pDest=L'\0';
  }
  else return lstrlenA(pSrc);

  return pDest - pString1;
}
#endif

/********************************************************************
 *
 *  xstrcpyW
 *
 *Copies a unicode string into a buffer.
 *
 *[in] wchar_t *wpString1  Pointer to a buffer into which the function copies characters.
 *                          The buffer must be large enough to contain the string,
 *                          including the terminating null character.
 *[in] wchar_t *wpString2  Pointer to a null-terminated string from which the function copies characters.
 *
 *Returns:  number of characters copied, not including the terminating null character.
 ********************************************************************/
#if defined xstrcpyW || defined ALLSTRFUNC
#define xstrcpyW_INCLUDED
#undef xstrcpyW
int xstrcpyW(wchar_t *wpString1, const wchar_t *wpString2)
{
  wchar_t *wpDest=wpString1;
  wchar_t *wpSrc=(wchar_t *)wpString2;

  if (wpDest != wpSrc)
  {
    while (*wpSrc)
      *wpDest++=*wpSrc++;
    *wpDest=L'\0';
  }
  else return lstrlenW(wpSrc);

  return wpDest - wpString1;
}
#endif

/********************************************************************
 *
 *  xstrcpynA
 *
 *Copies a specified number of characters from a source string into a buffer.
 *
 *[in] char *pString1           Pointer to a buffer into which the function copies characters.
 *                               The buffer must be large enough to contain the number of TCHAR values specified by nMaxLength,
 *                               including room for a terminating null character.
 *[in] char *pString2           Pointer to a null-terminated string from which the function copies characters.
 *[in] unsigned int nMaxLength  Specifies the number of TCHAR values to be copied from the string pointed to by pString2 into the buffer pointed to by pString1,
 *                               including a terminating null character.
 *
 *Returns:  number of characters copied, not including the terminating null character.
 ********************************************************************/
#if defined xstrcpynA || defined ALLSTRFUNC
#define xstrcpynA_INCLUDED
#undef xstrcpynA
int xstrcpynA(char *pString1, const char *pString2, unsigned int nMaxLength)
{
  char *pDest=pString1;
  char *pSrc=(char *)pString2;

  if (pDest != pSrc)
  {
    while (*pSrc && --nMaxLength)
      *pDest++=*pSrc++;
    *pDest=L'\0';
  }
  else
  {
    unsigned int nLen=lstrlenA(pSrc);
    return min(nLen, nMaxLength);
  }
  return pDest - pString1;
}
#endif

/********************************************************************
 *
 *  xstrcpynW
 *
 *Copies a specified number of unicode characters from a source string into a buffer.
 *
 *[in] wchar_t *wpString1       Pointer to a buffer into which the function copies characters.
 *                               The buffer must be large enough to contain the number of TCHAR values specified by nMaxLength,
 *                               including room for a terminating null character.
 *[in] wchar_t *wpString2       Pointer to a null-terminated string from which the function copies characters.
 *[in] unsigned int nMaxLength  Specifies the number of TCHAR values to be copied from the string pointed to by wpString2 into the buffer pointed to by wpString1,
 *                               including a terminating null character.
 *
 *Returns:  number of characters copied, not including the terminating null character.
 ********************************************************************/
#if defined xstrcpynW || defined ALLSTRFUNC
#define xstrcpynW_INCLUDED
#undef xstrcpynW
int xstrcpynW(wchar_t *wpString1, const wchar_t *wpString2, unsigned int nMaxLength)
{
  wchar_t *wpDest=wpString1;
  wchar_t *wpSrc=(wchar_t *)wpString2;

  if (wpDest != wpSrc)
  {
    while (*wpSrc && --nMaxLength)
      *wpDest++=*wpSrc++;
    *wpDest=L'\0';
  }
  else
  {
    unsigned int nLen=lstrlenW(wpSrc);
    return min(nLen, nMaxLength);
  }
  return wpDest - wpString1;
}
#endif

/********************************************************************
 *
 *  xstrstrA
 *
 *Find substring in string.
 *
 * [in] const char *pText  Text.
 * [in] DWORD dwTextLen    Text length, -1 search until NULL character in pText.
 * [in] const char *pStr   Find it.
 * [in] BOOL bSensitive    TRUE   case sensitive.
 *                         FALSE  case insensitive.
 *[out] char **pStrBegin   Pointer to the first char of pStr, can be NULL.
 *[out] char **pStrEnd     Pointer to the first char after pStr, can be NULL.
 *
 *Returns:  TRUE  pStr is founded.
 *          FALSE pStr isn't founded.
 ********************************************************************/
#if defined xstrstrA || defined ALLSTRFUNC
#define xstrstrA_INCLUDED
#undef xstrstrA
BOOL xstrstrA(const char *pText, DWORD dwTextLen, const char *pStr, BOOL bSensitive, char **pStrBegin, char **pStrEnd)
{
  const char *pTextCount;
  const char *pTextMax=pText + dwTextLen;
  const char *pStrCount;

  for (; ; ++pText)
  {
    if (dwTextLen == (DWORD)-1)
    {
      if (!*pText)
        break;
    }
    else
    {
      if ((DWORD)pText >= (DWORD)pTextMax)
        break;
    }

    for (pTextCount=pText, pStrCount=pStr;
          *pTextCount == *pStrCount ||
          (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextCount) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pStrCount));
         ++pTextCount)
    {
      if (!*++pStrCount)
      {
        if (pStrBegin) *pStrBegin=(char *)pText;
        if (pStrEnd) *pStrEnd=(char *)(pTextCount + 1);
        return TRUE;
      }
    }
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  xstrstrW
 *
 *Find substring in unicode string.
 *
 * [in] const wchar_t *wpText  Text.
 * [in] DWORD dwTextLen        Text length, -1 search until NULL character in wpText.
 * [in] const wchar_t *wpStr   Find it.
 * [in] BOOL bSensitive        TRUE   case sensitive.
 *                             FALSE  case insensitive.
 *[out] wchar_t **wpStrBegin   Pointer to the first char of wpStr, can be NULL.
 *[out] wchar_t **wpStrEnd     Pointer to the first char after wpStr, can be NULL.
 *
 *Returns:  TRUE  wpStr is founded.
 *          FALSE wpStr isn't founded.
 *
 *Note:
 *  xstrstrW can be used on Win95/98/Me if WideCharLower or WideCharUpper defined.
 ********************************************************************/
#if defined xstrstrW || defined ALLSTRFUNC
#define xstrstrW_INCLUDED
#undef xstrstrW
BOOL xstrstrW(const wchar_t *wpText, DWORD dwTextLen, const wchar_t *wpStr, BOOL bSensitive, wchar_t **wpStrBegin, wchar_t **wpStrEnd)
{
  const wchar_t *wpTextCount;
  const wchar_t *wpTextMax=wpText + dwTextLen;
  const wchar_t *wpStrCount;

  for (; ; ++wpText)
  {
    if (dwTextLen == (DWORD)-1)
    {
      if (!*wpText)
        break;
    }
    else
    {
      if ((DWORD)wpText >= (DWORD)wpTextMax)
        break;
    }

    for (wpTextCount=wpText, wpStrCount=wpStr;
          *wpTextCount == *wpStrCount ||
          #if defined WideCharLower_INCLUDED
            (bSensitive == FALSE && WideCharLower(*wpTextCount) == WideCharLower(*wpStrCount));
          #elif defined WideCharUpper_INCLUDED
            (bSensitive == FALSE && WideCharUpper(*wpTextCount) == WideCharUpper(*wpStrCount));
          #else
            (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextCount) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpStrCount));
          #endif
         ++wpTextCount)
    {
      if (!*++wpStrCount)
      {
        if (wpStrBegin) *wpStrBegin=(wchar_t *)wpText;
        if (wpStrEnd) *wpStrEnd=(wchar_t *)(wpTextCount + 1);
        return TRUE;
      }
    }
  }
  return FALSE;
}
#endif

/********************************************************************
 *
 *  xstrrepA
 *
 *Replace substring with string.
 *
 * [in] const char *pText  Text.
 * [in] const char *pIt    Replace it.
 * [in] const char *pWith  Replace with.
 * [in] BOOL bSensitive    TRUE   case sensitive.
 *                         FALSE  case insensitive.
 *[out] char *szResult     Output, can be NULL.
 *[out] int *nMaxResult    Contains the length of the result string,
 *                          including the terminating null character,
 *                          can be NULL.
 *
 *Returns:  number of changes.
 ********************************************************************/
#if defined xstrrepA || defined ALLSTRFUNC
#define xstrrepA_INCLUDED
#undef xstrrepA
int xstrrepA(const char *pText, const char *pIt, const char *pWith, BOOL bSensitive, char *szResult, int *nMaxResult)
{
  const char *pTextCount;
  const char *pItCount;
  char *pResult=szResult;
  int nWithLen=lstrlenA(pWith);
  int nChanges=0;
  int i;

  for (pTextCount=pText; *pText; ++pText)
  {
    for (pTextCount=pText, pItCount=pIt;
          *pTextCount == *pItCount ||
          (bSensitive == FALSE && (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pTextCount) == (char)(WORD)(DWORD)CharUpperA((char *)(DWORD)(WORD)*pItCount));
         ++pTextCount)
    {
      if (!*++pItCount)
      {
        if (szResult)
        {
          for (i=0; i < nWithLen; ++i)
            *pResult++=pWith[i];
        }
        else pResult+=nWithLen;

        pText=pTextCount + 1;
        pItCount=pIt;
        ++nChanges;
        if (!*pText) goto End;
      }
    }
    if (szResult) *pResult=*pText;
    ++pResult;
  }

  End:
  if (szResult) *pResult=*pText;
  if (nMaxResult) *nMaxResult=(pResult - szResult) + 1;
  return nChanges;
}
#endif

/********************************************************************
 *
 *  xstrrepW
 *
 *Replace substring with unicode string.
 *
 * [in] const wchar_t *wpText  Text.
 * [in] const wchar_t *wpIt    Replace it.
 * [in] const wchar_t *wpWith  Replace with.
 * [in] BOOL bSensitive        TRUE   case sensitive.
 *                             FALSE  case insensitive.
 *[out] wchar_t *wszResult     Output, can be NULL.
 *[out] int *nMaxResult        Contains the length of the result string,
 *                              including the terminating null character,
 *                              can be NULL.
 *
 *Returns:  number of changes.
 *
 *Note:
 *  xstrrepW can be used on Win95/98/Me if WideCharLower or WideCharUpper defined.
 ********************************************************************/
#if defined xstrrepW || defined ALLSTRFUNC
#define xstrrepW_INCLUDED
#undef xstrrepW
int xstrrepW(const wchar_t *wpText, const wchar_t *wpIt, const wchar_t *wpWith, BOOL bSensitive, wchar_t *wszResult, int *nMaxResult)
{
  const wchar_t *wpTextCount;
  const wchar_t *wpItCount;
  wchar_t *wpResult=wszResult;
  int nWithLen=lstrlenW(wpWith);
  int nChanges=0;
  int i;

  for (wpTextCount=wpText; *wpText; ++wpText)
  {
    for (wpTextCount=wpText, wpItCount=wpIt;
          *wpTextCount == *wpItCount ||
          #if defined WideCharLower_INCLUDED
            (bSensitive == FALSE && WideCharLower(*wpTextCount) == WideCharLower(*wpItCount));
          #elif defined WideCharUpper_INCLUDED
            (bSensitive == FALSE && WideCharUpper(*wpTextCount) == WideCharUpper(*wpItCount));
          #else
            (bSensitive == FALSE && (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpTextCount) == (wchar_t)(WORD)(DWORD)CharUpperW((wchar_t *)(DWORD)(WORD)*wpItCount));
          #endif
         ++wpTextCount)
    {
      if (!*++wpItCount)
      {
        if (wszResult)
        {
          for (i=0; i < nWithLen; ++i)
            *wpResult++=wpWith[i];
        }
        else wpResult+=nWithLen;

        wpText=wpTextCount + 1;
        wpItCount=wpIt;
        ++nChanges;
        if (!*wpText) goto End;
      }
    }
    if (wszResult) *wpResult=*wpText;
    ++wpResult;
  }

  End:
  if (wszResult) *wpResult=*wpText;
  if (nMaxResult) *nMaxResult=(wpResult - wszResult) + 1;
  return nChanges;
}
#endif

/********************************************************************
 *
 *  xatoiA
 *
 *Converts string to int.
 *
 *[in]  const char *pStr  String number.
 *[out] char **pNext      Pointer to the first char after number, can be NULL.
 *
 *Returns: integer.
 *
 *Examples:
 *  xatoiA("45", NULL) == 45;
 *  xatoiA("  -0045:value", &pNext) == -45, pNext == ":value"
 ********************************************************************/
#if defined xatoiA || defined ALLSTRFUNC
#define xatoiA_INCLUDED
#undef xatoiA
int xatoiA(const char *pStr, const char **pNext)
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
  if (bMinus == TRUE) nNumber=-nNumber;
  if (pNext) *pNext=pStr;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xatoiW
 *
 *Converts unicode string to int.
 *
 *[in]  const wchar_t *wpStr  Unicode string number.
 *[out] wchar_t **wpNext      Pointer to the first char after number, can be NULL.
 *
 *Returns: integer.
 *
 *Examples:
 *  xatoiW(L"45", NULL) == 45;
 *  xatoiW(L"  -0045:value", &wpNext) == -45, wpNext == L":value"
 ********************************************************************/
#if defined xatoiW || defined ALLSTRFUNC
#define xatoiW_INCLUDED
#undef xatoiW
int xatoiW(const wchar_t *wpStr, const wchar_t **wpNext)
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
  if (bMinus == TRUE) nNumber=-nNumber;
  if (wpNext) *wpNext=wpStr;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xatoi64A
 *
 *Converts string to int64.
 *
 *[in]  const char *pStr  String number.
 *[out] char **pNext      Pointer to the first char after number, can be NULL.
 *
 *Returns: 64-bit integer.
 *
 *Examples:
 *  xatoi64A("45", NULL) == 45;
 *  xatoi64A("  -0045:value", &pNext) == -45, pNext == ":value"
 ********************************************************************/
#if defined xatoi64A || defined ALLSTRFUNC
#define xatoi64A_INCLUDED
#undef xatoi64A
__int64 xatoi64A(const char *pStr, const char **pNext)
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
  if (bMinus == TRUE) nNumber=-nNumber;
  if (pNext) *pNext=pStr;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xatoi64W
 *
 *Converts unicode string to int64.
 *
 *[in]  const wchar_t *wpStr  Unicode string number.
 *[out] wchar_t **wpNext      Pointer to the first char after number, can be NULL.
 *
 *Returns: 64-bit integer.
 *
 *Examples:
 *  xatoi64W(L"45", NULL) == 45;
 *  xatoi64W(L"  -0045:value", &wpNext) == -45, wpNext == L":value"
 ********************************************************************/
#if defined xatoi64W || defined ALLSTRFUNC
#define xatoi64W_INCLUDED
#undef xatoi64W
__int64 xatoi64W(const wchar_t *wpStr, const wchar_t **wpNext)
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
  if (bMinus == TRUE) nNumber=-nNumber;
  if (wpNext) *wpNext=wpStr;
  return nNumber;
}
#endif

/********************************************************************
 *
 *  xitoaA
 *
 *Converts int to string.
 *
 *[in]   int nNumber  Integer.
 *[out]  char *szStr  String number, if NULL required buffer size returned in TCHARs.
 *
 *Returns: copied digits.
 *
 *Examples:
 *  xitoaA(45, szResult);   //szResult == "45"
 *  xitoaA(-45, szResult);  //szResult == "-45"
 ********************************************************************/
#if defined xitoaA || defined ALLSTRFUNC
#define xitoaA_INCLUDED
#undef xitoaA
int xitoaA(int nNumber, char *szStr)
{
  char szReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    if (szStr) szStr[b]='0';
    ++b;
  }
  else if (nNumber < 0)
  {
    if (szStr) szStr[b]='-';
    ++b;
    nNumber=-nNumber;
  }
  for (a=0; nNumber != 0; ++a)
  {
    szReverse[a]=(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  if (!szStr) return a + b + 1;

  while (--a >= 0) szStr[b++]=szReverse[a];
  szStr[b]='\0';
  return b;
}
#endif

/********************************************************************
 *
 *  xitoaW
 *
 *Converts int to unicode string.
 *
 *[in]   int nNumber      Integer.
 *[out]  wchar_t *wszStr  Unicode string number, if NULL required buffer size returned in TCHARs.
 *
 *Returns: copied digits.
 *
 *Examples:
 *  xitoaW(45, wszResult);   //wszResult == L"45"
 *  xitoaW(-45, wszResult);  //wszResult == L"-45"
 ********************************************************************/
#if defined xitoaW || defined ALLSTRFUNC
#define xitoaW_INCLUDED
#undef xitoaW
int xitoaW(int nNumber, wchar_t *wszStr)
{
  wchar_t wszReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    if (wszStr) wszStr[b]=L'0';
    ++b;
  }
  else if (nNumber < 0)
  {
    if (wszStr) wszStr[b]=L'-';
    ++b;
    nNumber=-nNumber;
  }
  for (a=0; nNumber != 0; ++a)
  {
    wszReverse[a]=(nNumber % 10) + L'0';
    nNumber=nNumber / 10;
  }
  if (!wszStr) return a + b + 1;

  while (--a >= 0) wszStr[b++]=wszReverse[a];
  wszStr[b]=L'\0';
  return b;
}
#endif

/********************************************************************
 *
 *  xuitoaA
 *
 *Converts unsigned int to string.
 *
 *[in]   unsigned int nNumber  Unsigned integer.
 *[out]  char *szStr           String number, if NULL required buffer size returned in TCHARs.
 *
 *Returns: copied digits.
 *
 *Examples:
 *  xuitoaA(45, szResult);                //szResult == "45"
 *  xuitoaA((unsigned int)-1, szResult);  //szResult == "4294967295"
 ********************************************************************/
#if defined xuitoaA || defined ALLSTRFUNC
#define xuitoaA_INCLUDED
#undef xuitoaA
int xuitoaA(unsigned int nNumber, char *szStr)
{
  char szReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    if (szStr) szStr[b]='0';
    ++b;
  }
  for (a=0; nNumber != 0; ++a)
  {
    szReverse[a]=(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  if (!szStr) return a + b + 1;

  while (--a >= 0) szStr[b++]=szReverse[a];
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
 *[in]   unsigned int nNumber  Unsigned integer.
 *[out]  wchar_t *wszStr       Unicode string number, if NULL required buffer size returned in TCHARs.
 *
 *Returns: copied digits.
 *
 *Examples:
 *  xuitoaW(45, wszResult);                //wszResult == L"45"
 *  xuitoaW((unsigned int)-1, wszResult);  //wszResult == L"4294967295"
 ********************************************************************/
#if defined xuitoaW || defined ALLSTRFUNC
#define xuitoaW_INCLUDED
#undef xuitoaW
int xuitoaW(unsigned int nNumber, wchar_t *wszStr)
{
  wchar_t wszReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    if (wszStr) wszStr[b]=L'0';
    ++b;
  }
  for (a=0; nNumber != 0; ++a)
  {
    wszReverse[a]=(nNumber % 10) + L'0';
    nNumber=nNumber / 10;
  }
  if (!wszStr) return a + b + 1;

  while (--a >= 0) wszStr[b++]=wszReverse[a];
  wszStr[b]=L'\0';
  return b;
}
#endif

/********************************************************************
 *
 *  xitoa64A
 *
 *Converts int64 to string.
 *
 *[in]   __int64 nNumber  64-bit integer.
 *[out]  char *szStr      String number, if NULL required buffer size returned in TCHARs.
 *
 *Returns: copied digits.
 *
 *Examples:
 *  xitoa64A(45, szResult);   //szResult == "45"
 *  xitoa64A(-45, szResult);  //szResult == "-45"
 ********************************************************************/
#if defined xi64toaA || defined ALLSTRFUNC
#define xi64toaA_INCLUDED
#undef xi64toaA
int xi64toaA(__int64 nNumber, char *szStr)
{
  char szReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    if (szStr) szStr[b]='0';
    ++b;
  }
  else if (nNumber < 0)
  {
    if (szStr) szStr[b]='-';
    ++b;
    nNumber=-nNumber;
  }
  for (a=0; nNumber != 0; ++a)
  {
    szReverse[a]=(char)(nNumber % 10) + '0';
    nNumber=nNumber / 10;
  }
  if (!szStr) return a + b + 1;

  while (--a >= 0) szStr[b++]=szReverse[a];
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
 *[in]   __int64 nNumber  64-bit integer.
 *[out]  wchar_t *wszStr  Unicode string number, if NULL required buffer size returned in TCHARs.
 *
 *Returns: copied digits.
 *
 *Examples:
 *  xitoa64W(45, wszResult);   //wszResult == L"45"
 *  xitoa64W(-45, wszResult);  //wszResult == L"-45"
 ********************************************************************/
#if defined xi64toaW || defined ALLSTRFUNC
#define xi64toaW_INCLUDED
#undef xi64toaW
int xi64toaW(__int64 nNumber, wchar_t *wszStr)
{
  wchar_t wszReverse[128];
  int a;
  int b=0;

  if (nNumber == 0)
  {
    if (wszStr) wszStr[b]=L'0';
    ++b;
  }
  else if (nNumber < 0)
  {
    if (wszStr) wszStr[b]=L'-';
    ++b;
    nNumber=-nNumber;
  }
  for (a=0; nNumber != 0; ++a)
  {
    wszReverse[a]=(wchar_t)(nNumber % 10) + L'0';
    nNumber=nNumber / 10;
  }
  if (!wszStr) return a + b + 1;

  while (--a >= 0) wszStr[b++]=wszReverse[a];
  wszStr[b]=L'\0';
  return b;
}
#endif

/********************************************************************
 *
 *  hex2decA
 *
 *Converts hex value to decimal.
 *
 *[in]  const char *pStrHex  Hex value.
 *
 *Returns: integer. Wrong hex value if equal to -1.
 *
 *Examples:
 *  hex2decA("A1F") == 2591;
 ********************************************************************/
#if defined hex2decA || defined ALLSTRFUNC
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
 *[in]  const wchar_t *wpStrHex  Unicode hex value.
 *
 *Returns: integer. Wrong hex value if equal to -1.
 *
 *Examples:
 *  hex2decW(L"A1F") == 2591;
 ********************************************************************/
#if defined hex2decW || defined ALLSTRFUNC
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
 *  dec2hexA
 *
 *Converts decimal to hex value.
 *
 *[in]   unsigned int nDec    Positive integer.
 *[out]  char *szStrHex       Hex value (output), if NULL required buffer size returned in TCHARs.
 *[in]   unsigned int nWidth  Minimum number of characters to the output.
 *[in]   BOOL bLowerCase       TRUE hexadecimal value in lowercase.
 *                             FALSE in uppercase.
 *
 *Returns: copied chars.
 *
 *Examples:
 *  dec2hexA(2591, szResult, 2, FALSE);   //szResult == "A1F"
 *  dec2hexA(10, szResult, 2, TRUE);      //szResult == "0a"
 ********************************************************************/
#if defined dec2hexA || defined ALLSTRFUNC
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
    if (b < 10)
    {
      if (szStrHex) szStrHex[c]=b + '0';
      ++c;
    }
    else
    {
      if (szStrHex) szStrHex[c]=b + (bLowerCase?'a':'A') - 10;
      ++c;
    }
  }
  if (!szStrHex) return max(nWidth, c) + 1;
  while (nWidth > c) szStrHex[c++]='0';
  szStrHex[c]='\0';
  nResult=c;

  for (b=0, --c; b < c; d=szStrHex[b], szStrHex[b++]=szStrHex[c], szStrHex[c--]=d);

  return nResult;
}
#endif

/********************************************************************
 *
 *  dec2hexW
 *
 *Converts decimal to unicode hex value.
 *
 *[in]   unsigned int nDec    Positive integer.
 *[out]  wchar_t *wszStrHex   Unicode hex value (output), if NULL required buffer size returned in TCHARs.
 *[in]   unsigned int nWidth  Minimum number of characters to the output.
 *[in]   BOOL bLowerCase       TRUE hexadecimal value in lowercase.
 *                             FALSE in uppercase.
 *
 *Returns: copied chars.
 *
 *Examples:
 *  dec2hexW(2591, wszResult, 2, FALSE);   //wszResult == L"A1F"
 *  dec2hexW(10, wszResult, 2, TRUE);      //wszResult == L"0a"
 ********************************************************************/
#if defined dec2hexW || defined ALLSTRFUNC
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
    if (b < 10)
    {
      if (wszStrHex) wszStrHex[c]=b + '0';
      ++c;
    }
    else
    {
      if (wszStrHex) wszStrHex[c]=b + (bLowerCase?L'a':L'A') - 10;
      ++c;
    }
  }
  if (!wszStrHex) return max(nWidth, c) + 1;
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
 *[in]   const unsigned char *pData  Binary data.
 *[in]   int nBytes                  Number of bytes in pData.
 *[out]  char *szStrHex              Output hex string buffer.
 *[in]   int nStrHexMax              Size of the hex string buffer in TCHARs.
 *[in]   BOOL bLowerCase              TRUE hexadecimal value in lowercase.
 *                                    FALSE in uppercase.
 *
 *Returns: copied chars.
 *
 *Note:
 *  bin2hexA uses dec2hexA.
 *
 *Examples:
 *  bin2hexA((unsigned char *)"Some Text", lstrlenA("Some Text"), szResult, MAX_PATH, TRUE);   //szResult == "536f6d652054657874"
 ********************************************************************/
#if defined bin2hexA || defined ALLSTRFUNC
#define bin2hexA_INCLUDED
#undef bin2hexA
int bin2hexA(const unsigned char *pData, int nBytes, char *szStrHex, int nStrHexMax, BOOL bLowerCase)
{
  int a;
  int b;

  nStrHexMax-=3;

  for (a=0, b=0; a < nBytes && b <= nStrHexMax; ++a, b+=2)
  {
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
 *[in]   const unsigned char *pData  Binary data.
 *[in]   int nBytes                  Number of bytes in pData.
 *[out]  wchar_t *wszStrHex          Output hex string buffer.
 *[in]   int nStrHexMax              Size of the hex string buffer in TCHARs.
 *[in]   BOOL bLowerCase              TRUE hexadecimal value in lowercase.
 *                                    FALSE in uppercase.
 *
 *Returns: copied chars.
 *
 *Note:
 *  bin2hexW uses dec2hexW.
 *
 *Examples:
 *  bin2hexW((unsigned char *)"Some Text", lstrlenA("Some Text"), wszResult, MAX_PATH, TRUE);   //wszResult == L"536f6d652054657874"
 ********************************************************************/
#if defined bin2hexW || defined ALLSTRFUNC
#define bin2hexW_INCLUDED
#undef bin2hexW
int bin2hexW(const unsigned char *pData, int nBytes, wchar_t *wszStrHex, int nStrHexMax, BOOL bLowerCase)
{
  int a;
  int b;

  nStrHexMax-=3;

  for (a=0, b=0; a < nBytes && b <= nStrHexMax; ++a, b+=2)
  {
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
 *[in]   const char *pStrHex   Hex string.
 *[out]  unsigned char *pData  Output buffer, if NULL required buffer size returned in TCHARs.
 *[in]   int nDataMax          Size of the output buffer in bytes.
 *
 *Returns: copied bytes.
 *
 *Note:
 *  hex2binA uses hex2decA.
 *
 *Examples:
 *  hex2binA("536f6d652054657874", (unsigned char *)szResult, MAX_PATH);   //szResult == "Some Text"
 ********************************************************************/
#if defined hex2binA || defined ALLSTRFUNC
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
 *[in]   const wchar_t *wpStrHex  Hex unicode string.
 *[out]  unsigned char *pData     Output buffer, if NULL required buffer size returned in TCHARs.
 *[in]   int nDataMax             Size of the output buffer in bytes.
 *
 *Returns: copied bytes.
 *
 *Note:
 *  hex2binW uses hex2decW.
 *
 *Examples:
 *  hex2binW(L"536f6d652054657874", (unsigned char *)szResult, MAX_PATH);   //szResult == "Some Text"
 ********************************************************************/
#if defined hex2binW || defined ALLSTRFUNC
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
 *
 *  xprintfA
 *
 *Function formats and stores a series of characters and values in a buffer. 
 *
 *[out] char *szOutput       Pointer to a buffer to receive the formatted output. If NULL required buffer size returned in TCHARs.
 *[in]  const char *pFormat  Pointer to a null-terminated string that contains the format-control specifications.
 *                            pFormat syntax is equal to wsprintfA function.
 *[in]  ...                  Specifies one or more optional arguments. The number and type of argument parameters
 *                            depend on the corresponding format-control specifications in the pFormat parameter.
 *
 *Returns:  number of characters copied, not including the terminating null character.
 *
 *Note:
 *  xprintfA uses xatoiA, xitoaA, xuitoaA, dec2hexA, xstrcpynA.
 ********************************************************************/
#if defined xprintfA || defined ALLSTRFUNC
#define xprintfA_INCLUDED
#undef xprintfA
int xprintfA(char *szOutput, const char *pFormat, ...)
{
  const char *pFmt=pFormat;
  char *pOut=szOutput;
  char *pStartOut;
  char *pString;
  char chFillChar;
  int nPrecision;
  int nWidth;
  int nLen=0;
  int i;

  va_list val;
  va_start(val, pFormat);

  while (*pFmt)
  {
    if (*pFmt == '%')
    {
      pStartOut=pOut;
      chFillChar=' ';
      nPrecision=0;
      nWidth=0;
      ++pFmt;

      if (*pFmt == '-')
      {
        nWidth=-1;
        ++pFmt;
      }
      if (*pFmt == '0')
      {
        chFillChar='0';
        ++pFmt;
      }
      if (*pFmt >= '1' && *pFmt <= '9')
      {
        if (nWidth == -1)
          nWidth-=xatoiA(pFmt, &pFmt) - 1;
        else
          nWidth=xatoiA(pFmt, &pFmt);
      }
      if (*pFmt == '.')
      {
        nPrecision=xatoiA(pFmt + 1, &pFmt);
      }

      if (*pFmt == '%')
      {
        if (szOutput) *pOut='%';
        ++pOut;
      }
      else if (*pFmt == 'c')
      {
        i=va_arg(val, unsigned int);
        if (nWidth > 0)
        {
          nWidth=max(nWidth - 1, 0);
          pOut+=nWidth;
        }
        if (szOutput) *pOut=i;
        ++pOut;
      }
      else if (*pFmt == 'd')
      {
        i=va_arg(val, int);
        if (!szOutput || nWidth > 0)
        {
          nLen=xitoaA(i, NULL) - 1;
          if (nWidth > 0)
          {
            nWidth=max(nWidth - nLen, 0);
            pOut+=nWidth;
            if (nWidth > 0 && i < 0)
            {
              if (szOutput) *pStartOut='-';
              ++pStartOut;
            }
          }
        }
        if (szOutput) nLen=xitoaA(i, pOut);
        pOut+=nLen;
      }
      else if (*pFmt == 'u')
      {
        i=va_arg(val, int);
        if (!szOutput || nWidth > 0)
        {
          nLen=xuitoaA((unsigned int)i, NULL) - 1;
          if (nWidth > 0)
          {
            nWidth=max(nWidth - nLen, 0);
            pOut+=nWidth;
          }
        }
        if (szOutput) nLen=xuitoaA((unsigned int)i, pOut);
        pOut+=nLen;
      }
      else if (*pFmt == 'x' || *pFmt == 'X')
      {
        i=va_arg(val, int);
        if (!szOutput || nWidth > 0)
        {
          nLen=dec2hexA((unsigned int)i, NULL, 0, (*pFmt == 'x')?TRUE:FALSE) - 1;
          if (nWidth > 0)
          {
            nWidth=max(nWidth - nLen, 0);
            pOut+=nWidth;
          }
        }
        if (szOutput) nLen=dec2hexA((unsigned int)i, pOut, 0, (*pFmt == 'x')?TRUE:FALSE);
        pOut+=nLen;
      }
      else if (*pFmt == 's')
      {
        pString=va_arg(val, char *);
        if (!szOutput || nWidth > 0)
        {
          nLen=lstrlenA(pString);
          if (nPrecision)
            nLen=min(nLen, nPrecision);
          if (nWidth > 0)
          {
            nWidth=max(nWidth - nLen, 0);
            pOut+=nWidth;
          }
        }
        if (szOutput) nLen=xstrcpynA(pOut, pString, (unsigned int)(nPrecision?nPrecision + 1:-1));
        pOut+=nLen;
      }

      if (nWidth > 0)
      {
        if (szOutput)
        {
          while (--nWidth >= 0)
            pStartOut[nWidth]=chFillChar;
        }
      }
      else if (nWidth < 0)
      {
        nWidth=min((pOut - pStartOut) + nWidth, 0);

        if (szOutput)
        {
          while (++nWidth <= 0)
            *pOut++=chFillChar;
        }
        else pOut+=0 - nWidth;
      }
    }
    else
    {
      if (szOutput) *pOut=*pFmt;
      ++pOut;
    }
    ++pFmt;
  }
  if (szOutput)
    *pOut='\0';
  else
    ++pOut;

  va_end(val);
  return pOut - szOutput;
}
#endif

/********************************************************************
 *
 *  xprintfW
 *
 *Function formats and stores a series of characters and values in a buffer. 
 *
 *[out] wchar_t *wszOutput       Pointer to a buffer to receive the formatted output. If NULL required buffer size returned in TCHARs.
 *[in]  const wchar_t *wpFormat  Pointer to a null-terminated string that contains the format-control specifications.
 *                                wpFormat syntax is equal to wsprintfW function.
 *[in]  ...                      Specifies one or more optional arguments. The number and type of argument parameters
 *                                depend on the corresponding format-control specifications in the wpFormat parameter.
 *
 *Returns:  number of characters copied, not including the terminating null character.
 *
 *Note:
 *  xprintfW uses xatoiW, xitoaW, xuitoaW, dec2hexW, xstrcpynW.
 ********************************************************************/
#if defined xprintfW || defined ALLSTRFUNC
#define xprintfW_INCLUDED
#undef xprintfW
int xprintfW(wchar_t *wszOutput, const wchar_t *wpFormat, ...)
{
  const wchar_t *wpFmt=wpFormat;
  wchar_t *wpOut=wszOutput;
  wchar_t *wpStartOut;
  wchar_t *wpString;
  wchar_t wchFillChar;
  unsigned int dwPrecision;
  unsigned int dwLen=0;
  int nWidth;
  int i;

  va_list val;
  va_start(val, wpFormat);

  while (*wpFmt)
  {
    if (*wpFmt == L'%')
    {
      wpStartOut=wpOut;
      wchFillChar=L' ';
      dwPrecision=0;
      nWidth=0;
      ++wpFmt;

      if (*wpFmt == L'-')
      {
        nWidth=-1;
        ++wpFmt;
      }
      if (*wpFmt == L'0')
      {
        wchFillChar=L'0';
        ++wpFmt;
      }
      if (*wpFmt >= L'1' && *wpFmt <= L'9')
      {
        if (nWidth == -1)
          nWidth-=xatoiW(wpFmt, &wpFmt) - 1;
        else
          nWidth=xatoiW(wpFmt, &wpFmt);
      }
      if (*wpFmt == L'.')
      {
        if (!(dwPrecision=(unsigned int)xatoiW(wpFmt + 1, &wpFmt)))
        {
          //Special format to specify argument as precision: "%.%us" or "%.%ds"
          if (*wpFmt == '%' && (*(wpFmt + 1) == 'u' || *(wpFmt + 1) == 'd'))
          {
            dwPrecision=(unsigned int)va_arg(val, int);
            wpFmt+=2;
          }
        }
      }

      if (*wpFmt == L'%')
      {
        if (wszOutput) *wpOut=L'%';
        ++wpOut;
      }
      else if (*wpFmt == L'c')
      {
        i=va_arg(val, unsigned int);
        if (nWidth > 0)
        {
          nWidth=max(nWidth - 1, 0);
          wpOut+=nWidth;
        }
        if (wszOutput) *wpOut=i;
        ++wpOut;
      }
      else if (*wpFmt == L'd')
      {
        i=va_arg(val, int);
        if (!wszOutput || nWidth > 0)
        {
          dwLen=xitoaW(i, NULL) - 1;
          if (nWidth > 0)
          {
            nWidth=max(nWidth - dwLen, 0);
            wpOut+=nWidth;
            if (nWidth > 0 && i < 0)
            {
              if (wszOutput) *wpStartOut=L'-';
              ++wpStartOut;
            }
          }
        }
        if (wszOutput) dwLen=xitoaW(i, wpOut);
        wpOut+=dwLen;
      }
      else if (*wpFmt == L'u')
      {
        i=va_arg(val, int);
        if (!wszOutput || nWidth > 0)
        {
          dwLen=xuitoaW((unsigned int)i, NULL) - 1;
          if (nWidth > 0)
          {
            nWidth=max(nWidth - dwLen, 0);
            wpOut+=nWidth;
          }
        }
        if (wszOutput) dwLen=xuitoaW((unsigned int)i, wpOut);
        wpOut+=dwLen;
      }
      else if (*wpFmt == L'x' || *wpFmt == L'X')
      {
        i=va_arg(val, int);
        if (!wszOutput || nWidth > 0)
        {
          dwLen=dec2hexW((unsigned int)i, NULL, 0, (*wpFmt == L'x')?TRUE:FALSE) - 1;
          if (nWidth > 0)
          {
            nWidth=max(nWidth - dwLen, 0);
            wpOut+=nWidth;
          }
        }
        if (wszOutput) dwLen=dec2hexW((unsigned int)i, wpOut, 0, (*wpFmt == L'x')?TRUE:FALSE);
        wpOut+=dwLen;
      }
      else if (*wpFmt == L's')
      {
        wpString=va_arg(val, wchar_t *);
        if (!wszOutput || nWidth > 0)
        {
          dwLen=lstrlenW(wpString);
          if (dwPrecision)
            dwLen=min(dwLen, dwPrecision);
          if (nWidth > 0)
          {
            nWidth=max(nWidth - dwLen, 0);
            wpOut+=nWidth;
          }
        }
        if (wszOutput) dwLen=xstrcpynW(wpOut, wpString, dwPrecision?dwPrecision + 1:(unsigned int)-1);
        wpOut+=dwLen;
      }

      if (nWidth > 0)
      {
        if (wszOutput)
        {
          while (--nWidth >= 0)
            wpStartOut[nWidth]=wchFillChar;
        }
      }
      else if (nWidth < 0)
      {
        nWidth=min((wpOut - wpStartOut) + nWidth, 0);

        if (wszOutput)
        {
          while (++nWidth <= 0)
            *wpOut++=wchFillChar;
        }
        else wpOut+=0 - nWidth;
      }
    }
    else
    {
      if (wszOutput) *wpOut=*wpFmt;
      ++wpOut;
    }
    ++wpFmt;
  }
  if (wszOutput)
    *wpOut=L'\0';
  else
    ++wpOut;

  va_end(val);
  return wpOut - wszOutput;
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

//insert functions
#define xstrcmpiA
#define xstrcmpinA
#define xstrrepA
#define xstrstrA
#define xatoiA
#define xitoaA
#include "StrFunc.h"

void main()
{
  char szResult[MAX_PATH];
  char *pStringBegin=NULL;
  char *pStringEnd=NULL;
  int nStringLen;
  int nError;

  nError=xstrcmpiA("ABC", "abc");
  printf("nError={%d}\n", nError);

  nError=xstrcmpinA("ABCdfg", "abcxyz", 3);
  printf("nError={%d}\n", nError);

  nError=xstrrepA("ABC||dfg||HJK", "||", "##", TRUE, szResult, &nStringLen);
  printf("szResult={%s}, nStringLen={%d}, nError={%d}\n", szResult, nStringLen, nError);

  nError=xstrstrA("ABC||dfg||HJK", (DWORD)-1, "Dfg", FALSE, &pStringBegin, &pStringEnd);
  printf("pStringBegin={%s}, pStringEnd={%s}, nError={%d}\n", pStringBegin, pStringEnd, nError);

  nError=xatoiA("123", NULL);
  printf("nError={%d}\n", nError);

  nError=xitoaA(45, szResult);
  printf("szResult={%s}, nError={%d}\n", szResult, nError);
}

*/

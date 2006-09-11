#include <windows.h>
#include <richedit.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include "codepage.h"
#include "akelpad.h"
#include "edit.h"
#include "langpack.h"

extern HWND hStatus;
extern BOOL append_header;

extern BOOL (WINAPI *GetCPInfoExPtr) (UINT,DWORD,LPCPINFOEX);

int CodePage=WINDOWS_1251;

static BOOL codepages[65536];

BOOL CALLBACK SelectDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString);
int getmaxelement(int *base,int iCount);
void ChangeByteOrder(char *,int);
void ChangeByteOrder(WORD *);

void swap(char &c1, char &c2) {
    char ch;
    ch=c1;
    c1=c2;
    c2=ch;
}

int GetCodePage() {
 return CodePage;
}

BOOL ChangeCodePage(int cp) {
 char cpbuf[256];
 CPINFOEX CPInfoEx;

 CodePage=cp;
 if(CodePage==WINDOWS_1251) lstrcpy(cpbuf,STR_CP_WINDOWS);
 else if(CodePage==DOS_866) lstrcpy(cpbuf,STR_CP_DOS);
 else if(CodePage==KOI8_R) lstrcpy(cpbuf,STR_CP_KOI);
 else if(CodePage==CP_UNICODE_UCS2_LE) lstrcpy(cpbuf,STR_CP_UNICODE_UCS2_LE);
 else if(CodePage==CP_UNICODE_UCS2_BE) lstrcpy(cpbuf,STR_CP_UNICODE_UCS2_BE);
 else if(CodePage==CP_UNICODE_UTF8) lstrcpy(cpbuf,STR_CP_UNICODE_UTF8);
 else {
  if(GetCPInfoExPtr) {
   if((*GetCPInfoExPtr)(CodePage,0,&CPInfoEx)) sprintf(cpbuf,"CP-%s",CPInfoEx.CodePageName);
   else sprintf(cpbuf,"CP-%u",CodePage);
  }
  else sprintf(cpbuf,"CP-%u",CodePage);
 }

 SendMessage(hStatus,SB_SETTEXT,3,(LPARAM)cpbuf);
 return TRUE;
}

BOOL SelectCodePage(HWND hWnd) {
 return (BOOL)DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_SELECTCODEPAGE),hWnd,(DLGPROC)SelectDlgProc);
}

int TranslateTextToRepresent(char *pcBuff,int iBufSize,char *pcOutBuf,char **endptr,BOOL firsttime) {
 int iBytesInOut=0;
 int cp,i;
 char ch,*p;

 cp=GetCodePage();

 if(firsttime) {
     append_header=FALSE;
//remember that we shall have at least 3 bytes at first time ;-)
     if(cp==CP_UNICODE_UCS2_LE) {
         if(iBufSize>=2){
             if(!memcmp(pcBuff,"\xFF\xFE",2)) append_header=TRUE;
         }
     }
     else if(cp==CP_UNICODE_UTF8) {
         if(iBufSize>=3){
             if(!memcmp(pcBuff,"\xEF\xBB\xBF",3)) append_header=TRUE;
         }
     }
     else append_header=TRUE;
 }

 *endptr=&pcBuff[iBufSize];

 if(cp==CP_UNICODE_UCS2_BE) {
     if(iBufSize%2) {
         iBufSize--;
         endptr--;
     }
     ChangeByteOrder(pcBuff,iBufSize);
 }

 if(cp==CP_UNICODE_UCS2_LE||cp==CP_UNICODE_UCS2_BE) {
     if(!firsttime||iBufSize<2||memcmp(pcBuff,"\xFF\xFE",2)) {
        memcpy(pcOutBuf,pcBuff,iBufSize);
        iBytesInOut=iBufSize;
     }
     else {
        memcpy(pcOutBuf,&pcBuff[2],iBufSize-2);
        iBytesInOut=iBufSize-2;
     }
 }
 else if(cp!=CP_UNICODE_UTF8) {
//Decoding is always full and unambigiouis
    iBytesInOut=2*MultiByteToWideChar(cp,
                        MB_PRECOMPOSED,
                        pcBuff,
                        iBufSize,
                        (WCHAR *)pcOutBuf,
                        TRANSLATEBUFFERSIZE/2);
    if(!iBytesInOut) return 0;
 }
 else {
//Parse UTF-8 sequence
//Support only chars up to three bytes (UCS-4 - go away!)
//Warning: Partial decoding is possible!
    i=0;
    if(firsttime&&iBufSize>=3&&!memcmp(pcBuff,"\xEF\xBB\xBF",3)) i+=3;
    while(i<=iBufSize-1) {
        ch=pcBuff[i];
        if(!(ch&0x80)) {
            pcOutBuf[iBytesInOut++]=ch;
            pcOutBuf[iBytesInOut++]='\0';
        }
        else if(!(ch&0x20)) {
            pcOutBuf[iBytesInOut++]=(ch>>2)&0x07;
            i++;
            if(i>=iBufSize) {
                *endptr=&pcBuff[iBufSize-1];
                iBytesInOut--;
                break;
            }
            pcOutBuf[iBytesInOut++]=(pcBuff[i]&0x3F)|(ch<<6);
            swap(pcOutBuf[iBytesInOut-1],pcOutBuf[iBytesInOut-2]);
        }
        else if(!(ch&0x10)) {
            i++;
            if(i>=iBufSize) {
                *endptr=&pcBuff[iBufSize-1];
//                iBytesInOut--;
                break;
            }
            pcOutBuf[iBytesInOut++]=(ch<<4)|((pcBuff[i]>>2)&0x0F);
            ch=pcBuff[i];
            i++;
            if(i>=iBufSize) {
                *endptr=&pcBuff[iBufSize-2];
//                iBytesInOut-=2;
                iBytesInOut--;
                break;
            }
            pcOutBuf[iBytesInOut++]=(pcBuff[i]&0x3F)|(ch<<6);
            swap(pcOutBuf[iBytesInOut-1],pcOutBuf[iBytesInOut-2]);
        }
        else {
            p=&pcBuff[i];
            pcOutBuf[iBytesInOut++]='\x3F';
            pcOutBuf[iBytesInOut++]='\0';
            if(!(ch&0x08)) i+=3;
            else if(!(ch&0x04)) i+=4;
            else if(!(ch&0x02)) i+=5;
            if(i>iBufSize) {
                *endptr=p;
                iBytesInOut-=2;
                break;
            }
        }

        i++;
    }
 }

 return iBytesInOut;

}

int TranslateTextFromRepresent(char *pcBuff,int iBufSize,char *pcOutBuf,BOOL firsttime) {
//This function requires even number of bytes in input buffer
//It provides always full encoding

 int cp,iBytesInOut,i;
 char *pOut;

 cp=GetCodePage();

 if(cp==CP_UNICODE_UCS2_LE||cp==CP_UNICODE_UCS2_BE) {
     if(!firsttime||!append_header) {
        memcpy(pcOutBuf,pcBuff,iBufSize);
        if(cp==CP_UNICODE_UCS2_BE) ChangeByteOrder(pcOutBuf,iBufSize);
        return iBufSize;
     }
     else {
        memcpy(pcOutBuf,"\xFF\xFE",2);
        memcpy(pcOutBuf+2, pcBuff,iBufSize);
        if(cp==CP_UNICODE_UCS2_BE) ChangeByteOrder(pcOutBuf,iBufSize+2);
        return iBufSize+2;
     }
 }
 else if(cp!=CP_UNICODE_UTF8) {
     iBytesInOut=WideCharToMultiByte(cp,
                        WC_COMPOSITECHECK|WC_DISCARDNS,
                        (WCHAR *)pcBuff,
                        iBufSize/2,
                        pcOutBuf,
                        TRANSLATEBUFFERSIZE,
                        NULL,
                        NULL);
     if(!iBytesInOut) return 0;
     return iBytesInOut;
 }
 else {
     pOut=pcOutBuf;

     if(firsttime&&append_header) {
         *pOut='\xEF';
         pOut++;
         *pOut='\xBB';
         pOut++;
         *pOut='\xBF';
         pOut++;
     }

     for(i=0;i<=(iBufSize/2)-1;i++) {
         if(*(WORD*)&pcBuff[2*i]<=0x7F) {
             *pOut=pcBuff[2*i];
             pOut++;
         }
         else if(*(WORD*)&pcBuff[2*i]<=0x7FF) {
             *pOut=HIBYTE(((*(WORD*)&pcBuff[2*i])<<2))|0xC0;
             pOut++;
             *pOut=(pcBuff[2*i]&0x3F)|0x80;
             pOut++;
         }
         else {
             *pOut=((pcBuff[2*i+1]>>4)&0x0F)|0xE0;
             pOut++;
             *pOut=HIBYTE(((*(WORD*)&pcBuff[2*i])<<2))&0x3F|0x80;
             pOut++;
             *pOut=(pcBuff[2*i])&0x3F|0x80;
             pOut++;
         }
     }

     return pOut-pcOutBuf;
 }

}


BOOL CALLBACK SelectDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
 int cp,i,icurrent;
 WCHAR wc;
 char c='À';
 static int iActiveCPIndex;
 static HWND hWndCP;
 char szCpString[16];
 CPINFOEX CPInfoEx;

 switch(message) {
  case WM_INITDIALOG:
   SendMessage(hDlg,WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1)));
   hWndCP=GetDlgItem(hDlg,IDC_CODEPAGE);
   EnumSystemCodePages(EnumCodePagesProc,CP_INSTALLED);
   codepages[65000]=FALSE;
   codepages[65001]=FALSE;
   icurrent=0;
   for(i=0;i<=65535;i++) {
    if(codepages[i]) {
     if(i==CodePage) iActiveCPIndex=icurrent;
     icurrent++;
     sprintf(szCpString,"%d",i);
     if(GetCPInfoExPtr) {
      if((*GetCPInfoExPtr)(i,0,&CPInfoEx)) SendMessage(hWndCP,CB_ADDSTRING,(WPARAM)0,(LPARAM)CPInfoEx.CodePageName);
      else SendMessage(hWndCP,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCpString);
     }
     else SendMessage(hWndCP,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCpString);
    }
   }
   SendMessage(hWndCP,CB_SETCURSEL,(WPARAM)iActiveCPIndex,(LPARAM)0);
   return TRUE;
  case WM_COMMAND:
   switch(LOWORD(wParam)) {
    case IDC_CODEPAGE:
     return TRUE;
    case IDOK:
     cp=GetDlgItemInt(hDlg,IDC_CODEPAGE,NULL,FALSE);
     if(cp<=2) {
      MessageBox(hDlg,MSG_WRONG_CODEPAGE,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
      SetFocus(hWndCP);
      SendMessage(hWndCP,EM_SETSEL,0,GetWindowTextLength(hWndCP));
     }

     else if(cp==65000||cp==65001||!MultiByteToWideChar(cp,
                         0,
                         &c,
                         1,
                         &wc,
                         1)) {
      MessageBox(hDlg,MSG_CP_UNIMPLEMENTED,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
      SetFocus(hWndCP);
      SendMessage(hWndCP,EM_SETSEL,0,GetWindowTextLength(hWndCP));
     }

     else {
      ChangeCodePage(cp);
      EndDialog(hDlg,-1);
     }
     return TRUE;
    case IDCANCEL:
     EndDialog(hDlg,0);
     return TRUE;
   }
   return FALSE;
 }
 return FALSE;
}

BOOL AutodetectCodePage(unsigned char *pcBuffer, int iCount) {
#ifndef FOREIGN_BUILD
 int counter[128];
 int i,exchars=0;
 unsigned char ch;

 char szOEMwatermark[]="\xAE\xA5\xA0\xA8\xBA\xCD\xB3\xC4\xB0\xB1\xB2\xDB";
 char szANSIwatermark[]="\xEE\xE5\xE0\xE8\xED\xF2\xCE\xC5\xC0\xC8\xD2\x97";
 char szKOIwatermark[]="\xCF\xC5\xC1\xC9\xCE\xD4\xEF\xE5\x80";
 
 char szUTF8watermark[]="\xD0\xD1";

 double iOEMrate=0,iANSIrate=0,iKOIrate=0,iUTF8rate=0;
#endif

//check if file is Unicode
 if(iCount>=3) {
     if(!memcmp(pcBuffer,"\xFF\xFE",2)) {
         ChangeCodePage(CP_UNICODE_UCS2_LE);
         return TRUE;
     }
     if(!memcmp(pcBuffer,"\xFE\xFF",2)) {
         ChangeCodePage(CP_UNICODE_UCS2_BE);
         return TRUE;
     }
     if(!memcmp(pcBuffer,"\xEF\xBB\xBF",3)) {
         ChangeCodePage(CP_UNICODE_UTF8);
         return TRUE;
     }
 }
 
#ifdef FOREIGN_BUILD
 ChangeCodePage(WINDOWS_1251);
 return TRUE;
#else

//Count number of each character in input buffer
 ZeroMemory(counter,128*sizeof(int));
 for(i=0;i<=iCount-1;i++) {
  if(pcBuffer[i]>=0x80) {
   exchars++;
   counter[pcBuffer[i]-0x80]++;
  }
 }

//Give it up if there's no representative selection
 if(exchars<15) {
  ChangeCodePage(WINDOWS_1251);
  return TRUE;
 }

 for(i=0;i<=9;i++) {
  ch=getmaxelement(counter,128);
  if(!counter[ch]) break;

  if(strchr(szOEMwatermark,ch+0x80)) iOEMrate+=counter[ch];
  if(strchr(szANSIwatermark,ch+0x80)) iANSIrate+=counter[ch];
  if(strchr(szKOIwatermark,ch+0x80)) iKOIrate+=counter[ch];
  if(strchr(szUTF8watermark,ch+0x80)) iUTF8rate+=counter[ch];
  counter[ch]=0;
 }

 iANSIrate*=1.2; //because ANSI is the most popular in Win32

 if(iANSIrate>=iOEMrate&&iANSIrate>=iKOIrate&&iANSIrate>=iUTF8rate) {
  ChangeCodePage(WINDOWS_1251);
  return TRUE;
 }
 if(iOEMrate>=iKOIrate&&iOEMrate>=iUTF8rate) {
  ChangeCodePage(DOS_866);
  return TRUE;
 }
 if(iKOIrate>=iUTF8rate) {
  ChangeCodePage(KOI8_R);
  return TRUE;
 }
 ChangeCodePage(CP_UNICODE_UTF8);
 return TRUE;
#endif
}

int getmaxelement(int *base,int iCount) {
 int maxvalue,number,i;

 if(iCount==0) return 0;
 maxvalue=base[0];
 number=0;
 for(i=1;i<=iCount-1;i++) {
  if(base[i]>maxvalue) {
   maxvalue=base[i];
   number=i;
  }
 }

 return number;
}

void ChangeByteOrder(char *string,int iSize) {
//iSize=number of bytes
 int i;

 for(i=0;i<=iSize/2-1;i++) {
   ChangeByteOrder((WORD*)&string[i*2]);
 }
}

void ChangeByteOrder(WORD *word) {
 char ch;
 ch=*(char *)word;
 *(char *)word=*((char *)word+1);
 *((char *)word+1)=ch;
}

BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString) {
 if(lpCodePageString) codepages[atoi(lpCodePageString)]=TRUE;
 return TRUE;
}

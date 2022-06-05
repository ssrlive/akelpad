#include <windows.h>
#include <tchar.h>
#define _RICHEDIT_VER 0x0200
#include <richedit.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "codepage.h"
#include "akelpad.h"
#include "edit.h"
#include "langpack.h"

extern HWND g_hStatus;
extern BOOL append_header;

extern BOOL (WINAPI *GetCPInfoExPtr) (UINT,DWORD,LPCPINFOEX);

int g_codePage=CP_ACP;

static BOOL codepages[65536];

BOOL CALLBACK SelectDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString);
int getmaxelement(int *base,int iCount);
void ChangeByteOrder(unsigned char *,int);
void ChangeByteOrder2(WORD *);

void swap(unsigned char *c1, unsigned char *c2) {
    unsigned char ch;
    ch=*c1;
    *c1=*c2;
    *c2=ch;
}

int GetCodePage() {
    return g_codePage;
}

int GetDefultCodePage(void)
{
    static int nCP = CP_ACP;
    if (CP_ACP == nCP) {
        TCHAR szCodePage[MAX_PATH/4] = { 0 };
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTCODEPAGE, szCodePage, _countof(szCodePage));
        nCP = _ttoi(szCodePage);
    }
    return nCP;
}

BOOL ChangeCodePage(int cp) {
    TCHAR cpbuf[256];
    CPINFOEX CPInfoEx;

    if (CP_ACP == cp) {
        cp = GetDefultCodePage();
    }

    g_codePage=cp;
    if(g_codePage==GetDefultCodePage()) lstrcpy(cpbuf,STR_CP_WINDOWS);
    else if(g_codePage==DOS_866) lstrcpy(cpbuf,STR_CP_DOS);
    else if(g_codePage==KOI8_R) lstrcpy(cpbuf,STR_CP_KOI);
    else if(g_codePage==CP_UNICODE_UCS2_LE) lstrcpy(cpbuf,STR_CP_UNICODE_UCS2_LE);
    else if(g_codePage==CP_UNICODE_UCS2_BE) lstrcpy(cpbuf,STR_CP_UNICODE_UCS2_BE);
    else if(g_codePage==CP_UNICODE_UTF8) lstrcpy(cpbuf,STR_CP_UNICODE_UTF8);
    else {
        if(GetCPInfoExPtr) {
            if((*GetCPInfoExPtr)(g_codePage,0,&CPInfoEx)) _stprintf(cpbuf, _T("CP-%s"), CPInfoEx.CodePageName);
            else _stprintf(cpbuf, _T("CP-%u"), g_codePage);
        }
        else _stprintf(cpbuf, _T("CP-%u"), g_codePage);
    }

    SendMessage(g_hStatus,SB_SETTEXT,3,(LPARAM)cpbuf);
    return TRUE;
}

BOOL SelectCodePage(HWND hWnd) {
    return (BOOL)DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_SELECTCODEPAGE),hWnd,(DLGPROC)SelectDlgProc);
}

int TranslateTextToRepresent(unsigned char *pcBuff,int iBufSize, unsigned char *pcOutBuf, unsigned char **endptr,BOOL firsttime) {
    int iBytesInOut=0;
    int cp,i;
    unsigned char ch,*p;

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
            (LPSTR) pcBuff,
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
                swap(&pcOutBuf[iBytesInOut-1],&pcOutBuf[iBytesInOut-2]);
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
                swap(&pcOutBuf[iBytesInOut-1], &pcOutBuf[iBytesInOut-2]);
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

int TranslateTextFromRepresent(unsigned char *pcBuff,int iBufSize, unsigned char *pcOutBuf,BOOL firsttime) {
    //This function requires even number of bytes in input buffer
    //It provides always full encoding

    int cp,iBytesInOut,i;
    unsigned char *pOut;

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
            (LPSTR) pcOutBuf,
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


BOOL CALLBACK SelectDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int cp,i,icurrent;
    WCHAR wc;
    char c=(char)'А';
    static int iActiveCPIndex;
    static HWND hWndCP;
    TCHAR szCpString[16];
    CPINFOEX CPInfoEx;

    UNREFERENCED_PARAMETER(lParam);

    switch(message)
    {
    case WM_INITDIALOG:
        SendMessage(hDlg,WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1)));
        hWndCP=GetDlgItem(hDlg,IDC_CODEPAGE);
        EnumSystemCodePages(EnumCodePagesProc,CP_INSTALLED);
        codepages[65000]=FALSE;
        codepages[65001]=FALSE;
        icurrent=0;
        for(i=0;i<=65535;i++) {
            if(codepages[i]) {
                if(i==g_codePage) iActiveCPIndex=icurrent;
                icurrent++;
                _stprintf(szCpString, _T("%d"), i);
                if (GetCPInfoExPtr) {
                    if((*GetCPInfoExPtr)(i,0,&CPInfoEx)) {
                        SendMessage(hWndCP,CB_ADDSTRING,(WPARAM)0,(LPARAM)CPInfoEx.CodePageName);
                    } else { 
                        SendMessage(hWndCP,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCpString);
                    }
                } else { 
                    SendMessage(hWndCP,CB_ADDSTRING,(WPARAM)0,(LPARAM)szCpString);
                }
            }
        }
        SendMessage(hWndCP,CB_SETCURSEL,(WPARAM)iActiveCPIndex,(LPARAM)0);
        return TRUE;
    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDC_CODEPAGE:
            return TRUE;
        case IDOK:
            cp=GetDlgItemInt(hDlg,IDC_CODEPAGE,NULL,FALSE);
            if(cp<=2) {
                MessageBox(hDlg,MSG_WRONG_CODEPAGE,APP_SHORT_TITLE,MB_OK|MB_ICONERROR);
                SetFocus(hWndCP);
                SendMessage(hWndCP,EM_SETSEL,0,GetWindowTextLength(hWndCP));
            }

            else if(cp==65000||cp==65001||
                !MultiByteToWideChar(cp, 0, &c, 1, &wc, 1))
            {
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

// 
// http://blogs.msdn.com/oldnewthing/archive/2004/03/24/95235.aspx 
// http://codesnipers.com/?q=node/68 
// http://www.codeguru.com/Cpp/misc/misc/multi-lingualsupport/article.php/c10451/ 
// http://www.cl.cam.ac.uk/~mgk25/unicode.html 
// 
typedef enum SC_ENCODING { 
    eEncodeACP = CP_ACP, 
    eEncodeOEMCP = CP_OEMCP, 
    eEncodeMACCP = CP_MACCP, 
    eEncodeTHREAD_ACP = CP_THREAD_ACP, 
    eEncodeSYMBOL = CP_SYMBOL, 
    eEncodeUTF7 = CP_UTF7, 
    eEncodeUTF8 = CP_UTF8, 

    eEncodeUTF7_BOM = CP_UTF7,//eEncodeSYMBOL + 1, 
    eEncodeUTF8_BOM = CP_UTF8, 
    eEncodeUNI_BE_BOM = CP_UNICODE_UCS2_BE, 
    eEncodeUNI_LE_BOM = CP_UNICODE_UCS2_LE, 
    eEncodeUTF32_BE_BOM, 
    eEncodeUTF32_LE_BOM, 
    eEncodeDBCS, 
} SC_ENCODING; 

SC_ENCODING DetectTextEncoding(LPCBYTE pBuff, UINT cchLen, 
                               OUT LPCBYTE * pPayload, OUT UINT * pnPayloadLen) 
{ 
    static const char szBomULE[] = {0xFF, 0xFE}; 
    static const char szBomUBE[] = {0xFE, 0xFF}; 
    static const char szBomUTF8[] = {0xEF, 0xBB, 0xBF}; 
    static const char szBomUTF7[5] = {0x2B, 0x2F, 0x76, 0x38, 0x2D}; // "+/v8-" 
    static const char szBomUTF32BE[] = {0x00, 0x00, 0xFE, 0xFF}; 
    static const char szBomUTF32LE[] = {0xFF, 0xFE, 0x00, 0x00}; 

    SC_ENCODING eReturn = eEncodeACP; 

    if(0 == memcmp(pBuff, szBomULE, sizeof(szBomULE))) 
    { 
        if(pPayload) *pPayload = pBuff + sizeof(szBomULE); 
        if(pnPayloadLen) *pnPayloadLen = cchLen - sizeof(szBomULE); 
        eReturn = eEncodeUNI_LE_BOM; 
    } 
    else if (0 == memcmp(pBuff, szBomUBE, sizeof(szBomUBE))) 
    { 
        if(pPayload) *pPayload = pBuff + sizeof(szBomUBE); 
        if(pnPayloadLen) *pnPayloadLen = cchLen - sizeof(szBomUBE); 
        eReturn = eEncodeUNI_BE_BOM; 
    } 
    else if (0 == memcmp(pBuff, szBomUTF8, sizeof(szBomUTF8))) 
    { 
        if(pPayload) *pPayload = pBuff + sizeof(szBomUTF8); 
        if(pnPayloadLen) *pnPayloadLen = cchLen - sizeof(szBomUTF8); 
        eReturn = eEncodeUTF8_BOM; 
    } 
    else if (0 == memcmp(pBuff, szBomUTF7, sizeof(szBomUTF7))) 
    { 
        if(pPayload) *pPayload = pBuff + sizeof(szBomUTF7); 
        if(pnPayloadLen) *pnPayloadLen = cchLen - sizeof(szBomUTF7); 
        eReturn = eEncodeUTF7_BOM; 
    } 
    else if (0 == memcmp(pBuff, szBomUTF32BE, sizeof(szBomUTF32BE))) 
    { 
        if(pPayload) *pPayload = pBuff + sizeof(szBomUTF32BE); 
        if(pnPayloadLen) *pnPayloadLen = cchLen - sizeof(szBomUTF32BE); 
        eReturn = eEncodeUTF32_BE_BOM; 
    } 
    else if (0 == memcmp(pBuff, szBomUTF32LE, sizeof(szBomUTF32LE))) 
    { 
        if(pPayload) *pPayload = pBuff + sizeof(szBomUTF32LE); 
        if(pnPayloadLen) *pnPayloadLen = cchLen - sizeof(szBomUTF32LE); 
        eReturn = eEncodeUTF32_LE_BOM; 
    } 
    else 
    { 
        char *pTmp = NULL;
        if(pPayload) *pPayload = pBuff; 
        if(pnPayloadLen) *pnPayloadLen = cchLen; 

        pTmp = (char *)calloc(cchLen+1, sizeof(char)); 
        memset(pTmp, 0, sizeof(char)*(cchLen+1)); 
        memcpy(pTmp, pBuff, cchLen); 
        strlwr(pTmp); 

        { 
            const char * pIter = NULL; 
            DWORD dwUTF8Count = 0; 
            DWORD dwAntiUtf8 = 0; 
            DWORD dwPlainChar = 0; 
            for (pIter = pTmp; pIter < pTmp+cchLen; pIter++) 
            { 
                if ( (BYTE)(*pIter) > 0x7F ) 
                { 
                    eReturn = eEncodeDBCS; 

                    if(      (((BYTE)(*pIter))>>1) == 0x7E) 
                    { 
                        if( (((BYTE)(*(pIter+1)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+2)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+3)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+4)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+5)))>>6) == 0x02 && 
                            (TRUE)) 
                        { 
                            dwUTF8Count++; 
                            pIter += 5; 
                        } 
                    } 
                    else if ((((BYTE)(*pIter))>>2) == 0x3E) 
                    { 
                        if( (((BYTE)(*(pIter+1)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+2)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+3)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+4)))>>6) == 0x02 && 
                            (TRUE)) 
                        { 
                            dwUTF8Count++; 
                            pIter += 4; 
                        } 
                    } 
                    else if ((((BYTE)(*pIter))>>3) == 0x1E) 
                    { 
                        if( (((BYTE)(*(pIter+1)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+2)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+3)))>>6) == 0x02 && 
                            (TRUE)) 
                        { 
                            dwUTF8Count++; 
                            pIter += 3; 
                        } 
                    } 
                    else if ((((BYTE)(*pIter))>>4) == 0x0E) 
                    { 
                        if( (((BYTE)(*(pIter+1)))>>6) == 0x02 && 
                            (((BYTE)(*(pIter+2)))>>6) == 0x02 && 
                            (TRUE)) 
                        { 
                            dwUTF8Count++; 
                            pIter += 2; 
                        } 
                    } 
                    else if ((((BYTE)(*pIter))>>5) == 0x06) 
                    { 
                        if( (((BYTE)(*(pIter+1)))>>6) == 0x02 && 
                            (TRUE)) 
                        { 
                            dwUTF8Count++; 
                            dwAntiUtf8++; 
                            pIter += 1; 
                        } 
                    } 
                    else 
                    { 
                        dwPlainChar++; 
                    } 
                } 
            } 
            if (dwUTF8Count > dwAntiUtf8 && dwUTF8Count>dwPlainChar) 
            { 
                eReturn = eEncodeUTF8; 
            } 
        } 
        free(pTmp); 
    } 

    return eReturn; 
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
#if 0
    {
        SC_ENCODING cpTest = DetectTextEncoding(pcBuffer, iCount, NULL, NULL);
        if (eEncodeUTF32_BE_BOM <= cpTest && cpTest < 0) {
            ChangeCodePage(GetDefultCodePage());
        } else {
            ChangeCodePage(cpTest);
        }
    }
#else
    ChangeCodePage(GetDefultCodePage());
#endif
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
        ChangeCodePage(GetDefultCodePage());
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
        ChangeCodePage(GetDefultCodePage());
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

void ChangeByteOrder(unsigned char *string,int iSize) {
    //iSize=number of bytes
    int i;

    for(i=0;i<=iSize/2-1;i++) {
        ChangeByteOrder2((WORD*)&string[i*2]);
    }
}

void ChangeByteOrder2(WORD *word) {
    char ch;
    ch=*(char *)word;
    *(char *)word=*((char *)word+1);
    *((char *)word+1)=ch;
}

BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString) {
    if(lpCodePageString) codepages[_ttoi(lpCodePageString)]=TRUE;
    return TRUE;
}

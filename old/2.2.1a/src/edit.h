#ifndef __EDIT_H__
#define __EDIT_H__

#define RECENTFILES 10

#define READBUFFERSIZE 8192 //Setting this value too low will lead to errors (at least 16 should be enough)
//READBUFFERSIZE _must_ be even!

//Translated text can be maximum 3 times greater (if it is translated into UTF-8)
#define TRANSLATEBUFFERSIZE (READBUFFERSIZE*3+4)
#define AUTODETECTSIZE 8192 //should not be greater than READBUFFERSIZE

void OpenDocument(HWND hWndEdit, BOOL mustexist);

void InitEditor(HWND);
void SetChosenFont(HWND hWndEdit);
DWORD CALLBACK InputStreamCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG FAR *pcb);
DWORD CALLBACK OutputStreamCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG FAR *pcb);
DWORD CALLBACK BufferCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG FAR *pcb);
DWORD CALLBACK ReadBufferCallback(DWORD_PTR dwCookie,LPBYTE pbBuff,LONG cb,LONG FAR *pcb);

BOOL RestorePreOptionsFromRegistry();
BOOL RestoreOptionsFromRegistry(HWND hWndEdit);
BOOL SaveOptionsInRegistry(HWND hWnd);

int DoFileNew(HWND hWndEdit);
void DoFileNewWindow(void);
void DoFileOpen(HWND hWndEdit);
void DoFileOpenHistory(HWND hWndEdit,int number);
void DoFileReopen(HWND hWndEdit);
int DoFileSave(HWND hWndEdit);
int DoFileSaveAs(HWND hWndEdit);
void DoFilePageSetup(HWND hWndEdit);
void DoFilePrint(HWND hWndEdit);
void DoFileExit(HWND hWndEdit);

void DoEditUndo(HWND hWndEdit);
void DoEditRedo(HWND hWndEdit);
void DoEditCut(HWND hWndEdit);
void DoEditCopy(HWND hWndEdit);
void DoEditPaste(HWND hWndEdit);
void DoEditClear(HWND hWndEdit);
void DoEditSelectAll(HWND hWndEdit);
void DoEditInsertChar(HWND hWndEdit);
void DoEditWrap(HWND hWndEdit);
void DoEditKeepSpace(HWND hWndEdit);

void DoSearchFind(HWND hWndEdit);
void DoSearchFindNext(HWND hWndEdit);
void DoSearchFindOther(HWND hWndEdit);
void DoSearchReplace(HWND hWndEdit);
void DoSearchGoToLine(HWND hWndEdit);

void DoSettingsFont(HWND hWndEdit);
void DoSettingsBG(HWND hWndEdit);
void DoSettingsNoMultiOpen(HWND hWndEdit);
void DoSettingsAsian(HWND hWndEdit);

void DoCodePageOpenAs(HWND hWndEdit);
int DoCodePageSaveIn(HWND hWndEdit);

void DoHelpAbout(HWND hWndEdit);

void DoNonMenuDelLine(HWND hWndEdit);

void DoNonMenuTab(HWND hWndEdit, BOOL istab, BOOL add);

#endif

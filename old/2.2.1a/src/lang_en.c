#include <windows.h>
#include <tchar.h>

TCHAR APP_SHORT_TITLE[]=_T("AkelPad");
TCHAR APP_FULL_TITLE[]=_T("AkelPad2");

TCHAR STR_DUMMY_NAME[]=_T("AkelPad Synchronizing Class");

TCHAR STR_MODE_INSERT[]=_T("Insert");
TCHAR STR_MODE_OVERLAP[]=_T("Overlay");
TCHAR STR_MODIFIED[]=_T("Modified");

TCHAR STR_CP_WINDOWS[]=_T("Windows (ANSI)");
TCHAR STR_CP_DOS[]=_T("DOS (OEM)");
TCHAR STR_CP_KOI[]=_T("KOI8-R");
TCHAR STR_CP_UNICODE_UCS2_LE[]=_T("Unicode (UCS-2 little endian)");
TCHAR STR_CP_UNICODE_UCS2_BE[]=_T("Unicode (UCS-2 big endian)");
TCHAR STR_CP_UNICODE_UTF8[]=_T("Unicode (UTF-8)");

TCHAR STR_FILE_FILTER[]=_T("Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0\0");

TCHAR STR_DOCNAME[]=_T("AkelPad Document");

TCHAR STR_HOMEPAGE[]=_T("http://akelpad.sourceforge.net");

//akelpad.cpp messages
TCHAR MSG_ERROR_LOADING_DLL[]=_T("Cannot load RICHED20.DLL.");
TCHAR MSG_ERROR_IN_MESSAGE_QUEUE[]=_T("Error in message queue.");
TCHAR MSG_ERROR_LIMIT_REACHED[]=_T("Action couldn't be performed due to text limit.\nThe limit will be increased.");
TCHAR MSG_ERROR_NOT_ENOUGH_MEMORY_FOR_RICHEDIT[]=_T("Error allocating memory for edit control.");
TCHAR MSG_ERROR_NOT_ENOUGH_MEMORY[]=_T("Not enough memory.");


//edit.cpp messages
TCHAR MSG_DOCUMENT_CHANGED[]=_T("Document was modified. Do you want to save it?");
TCHAR MSG_FILE_WILL_BE_REOPENED[]=_T("File will be reopened, all changes will be lost.");
TCHAR MSG_WHETHER_APPEND_HEADER[]=_T("Unicode files usually have a mark in the beginning, which helps to identify codepage. This file hasn't. Do you want to add it?");
TCHAR MSG_ERROR_IO[]=_T("Input/output error.");
TCHAR MSG_ERROR_CANNOT_TRANSLATE[]=_T("Error: cannot recode text.");
TCHAR MSG_ERROR_CANNOT_OPEN_FILE[]=_T("Cannot open file.");
TCHAR MSG_ERROR_SYNTAX[]=_T("Syntax error.");
TCHAR MSG_TITLE_SAVING_IN_UNICODE[]=_T("Unicode warning");
TCHAR MSG_WRONG_STRING[]=_T("Invalid line number");
TCHAR MSG_ERROR_PRINT[]=_T("Cannot print document");

//codepage.cpp messages
TCHAR MSG_WRONG_CODEPAGE[]=_T("Wrong codepage.");
TCHAR MSG_CP_UNIMPLEMENTED[]=_T("This codepage isn't implemented.");

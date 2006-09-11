#ifndef __LANGPACK_H__
#define __LANGPACK_H__

/*
 * The following constant applies to:
 * APP_SHORT_TITLE
 */

#define STD_LANGPACK_STRING_LENGTH 256

extern char APP_SHORT_TITLE[];
extern char APP_FULL_TITLE[];

extern char STR_DUMMY_NAME[];

extern char STR_MODE_INSERT[];
extern char STR_MODE_OVERLAP[];
extern char STR_MODIFIED[];

extern char STR_CP_WINDOWS[];
extern char STR_CP_DOS[];
extern char STR_CP_KOI[];
extern char STR_CP_UNICODE_UCS2_LE[];
extern char STR_CP_UNICODE_UCS2_BE[];
extern char STR_CP_UNICODE_UTF8[];

extern char STR_FILE_FILTER[];

extern char STR_DOCNAME[];

extern char STR_HOMEPAGE[];

//akelpad.cpp messages
extern char MSG_ERROR_LOADING_DLL[];
extern char MSG_ERROR_IN_MESSAGE_QUEUE[];
extern char MSG_ERROR_LIMIT_REACHED[];
extern char MSG_ERROR_NOT_ENOUGH_MEMORY_FOR_RICHEDIT[];
extern char MSG_ERROR_NOT_ENOUGH_MEMORY[];


//edit.cpp messages
extern char MSG_DOCUMENT_CHANGED[];
extern char MSG_FILE_WILL_BE_REOPENED[];
extern char MSG_WHETHER_APPEND_HEADER[];
extern char MSG_ERROR_IO[];
extern char MSG_ERROR_SETFONT[];
extern char MSG_ERROR_CANNOT_TRANSLATE[];
extern char MSG_ERROR_CANNOT_OPEN_FILE[];
extern char MSG_ERROR_SYNTAX[];
extern char MSG_TITLE_SAVING_IN_UNICODE[];
extern char MSG_WRONG_STRING[];
extern char MSG_ERROR_PRINT[];

//codepage.cpp messages
extern char MSG_WRONG_CODEPAGE[];
extern char MSG_CP_UNIMPLEMENTED[];

#endif

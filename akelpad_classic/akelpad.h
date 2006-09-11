#ifndef __AKELPAD_H__
#define __AKELPAD_H__

BOOL GetModify();
void SetModify(BOOL state);
int RenewHistoryMenu();

#define ID_EDIT 1001
#define ID_STATUS 1002

#define IDM_HISTORY_BASE 50000
#define HISTORY_MENU_POSITION 10
#define HISTORY_STRING_LENGTH 64

#include "resource.h"

#endif

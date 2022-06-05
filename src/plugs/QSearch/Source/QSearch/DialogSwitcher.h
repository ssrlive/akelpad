#ifndef _akelpad_dialog_switcher_h_
#define _akelpad_dialog_switcher_h_
//---------------------------------------------------------------------------
#include <windows.h>
#include <commctrl.h>
#include "AkelDllHeader.h"

void dlgswtchInitialize(const PLUGINDATA* pd);
void dlgswtchUninitialize(void);
BOOL dlgswtchIsActive(void);

//---------------------------------------------------------------------------
#endif

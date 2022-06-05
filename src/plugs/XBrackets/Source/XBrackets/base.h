///////////////////////////////////////////////////////////////////////////
// Basic header for all files in the project
///////////////////////////////////////////////////////////////////////////
#ifndef _base_h_
#define _base_h_
//-------------------------------------------------------------------------

// XBrackets: don't define UNICODE for this project!!!
// XBrackets: TCHAR means char in this project

//#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <TCHAR.H>

#ifdef _WIN64
  #ifndef AKELPAD_X64
    #define AKELPAD_X64
  #endif
#endif

// this definition enables or disables 
// additional Unicode functions for controls
// 1 - on, 0 - off
#define any_ctrl_enable_w_members 1

// use AEN_PAINT notification
// 1 - on, 0 - off
#define use_aen_paint 0

//-------------------------------------------------------------------------
#endif


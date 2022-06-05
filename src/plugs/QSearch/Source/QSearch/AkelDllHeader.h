#ifndef _akel_dll_header_h_
#define _akel_dll_header_h_
//---------------------------------------------------------------------------
#ifdef _WIN64
  #ifndef AKELPAD_X64
    #define AKELPAD_X64
  #endif
#endif

#ifdef AKELPAD_X64
  // types and structs
  #define INT_X INT_PTR
  #define CHARRANGE_X CHARRANGE64
  #define TEXTRANGE_X TEXTRANGE64
  #define TEXTRANGEA_X TEXTRANGE64A
  #define TEXTRANGEW_X TEXTRANGE64W
  // messages
  #define EM_EXGETSEL_X EM_EXGETSEL64
  #define EM_EXSETSEL_X EM_EXSETSEL64
  #define EM_GETTEXTRANGE_X EM_GETTEXTRANGE64
#else
  // types and structs
  #define INT_X INT
  #define CHARRANGE_X CHARRANGE
  #define TEXTRANGE_X TEXTRANGE
  #define TEXTRANGEA_X TEXTRANGEA
  #define TEXTRANGEW_X TEXTRANGEW
  // messages
  #define EM_EXGETSEL_X EM_EXGETSEL
  #define EM_EXSETSEL_X EM_EXSETSEL
  #define EM_GETTEXTRANGE_X EM_GETTEXTRANGE
#endif

#include "AkelEdit.h"
#include "AkelDLL.h"

#ifndef FR_UP
  #define FR_UP         FRF_UP
#endif
#ifndef FR_BEGINNING
  #define FR_BEGINNING  FRF_BEGINNING
#endif


//---------------------------------------------------------------------------
#endif

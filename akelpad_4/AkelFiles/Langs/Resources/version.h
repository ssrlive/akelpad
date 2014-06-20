#define RC_AKELPADID 4,9,0,0

#if !defined(RC_EXEVERSION_INCLUDED) && !defined(DLL_VERSION_INCLUDED)
  #if defined(RC_EXEVERSION)
    #if (RC_VERSIONBIT == 64)
      #define RC_VERSIONDESCRIPTION "AkelPad (x64) text editor"
    #else
      #define RC_VERSIONDESCRIPTION "AkelPad (x86) text editor"
    #endif
    #define RC_VERSIONCOPYRIGHT "Copyright © AkelSoft 2003-2014"
    #define RC_VERSIONNAME "AkelPad.exe"
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define RC_EXEVERSION_INCLUDED
  #elif defined(DLL_VERSION)
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define DLL_VERSION_INCLUDED
  #endif
#endif

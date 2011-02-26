#define RC_AKELPADID 4,5,6,0

#if !defined(EXE_VERSION_INCLUDED) && !defined(DLL_VERSION_INCLUDED)
  #if defined(EXE_VERSION)
    #if (RC_VERSIONBIT == 64)
      #define RC_VERSIONDESCRIPTION "AkelPad (x64) text editor"
    #else
      #define RC_VERSIONDESCRIPTION "AkelPad (x86) text editor"
    #endif
    #define RC_VERSIONCOPYRIGHT "Copyright © AkelSoft 2003-2011"
    #define RC_VERSIONNAME "AkelPad.exe"
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define EXE_VERSION_INCLUDED
  #elif defined(DLL_VERSION)
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define DLL_VERSION_INCLUDED
  #endif
#endif

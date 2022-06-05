#define AKELPAD_ID 3,7,4,0

#if !defined(EXE_VERSION_INCLUDED) && !defined(DLL_VERSION_INCLUDED)
  #if defined(EXE_VERSION)
    #define RC_VERSIONDESCRIPTION "AkelPad text editor"
    #define RC_VERSIONCOPYRIGHT "Copyright © AkelSoft 2003-2010"
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

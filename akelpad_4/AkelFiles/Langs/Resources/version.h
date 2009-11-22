// Add version information to EXE

#define AKELPAD_ID 4,3,6,0

#if defined(EXE_VERSION)
  #if !defined(EXE_VERSION_INCLUDED) && !defined(DLL_VERSION_INCLUDED)
    #define RC_VERSIONDESCRIPTION "AkelPad text editor"
    #define RC_VERSIONCOPYRIGHT "Copyright © AkelSoft 2003-2009"
    #define RC_VERSIONNAME "AkelPad.exe"
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define EXE_VERSION_INCLUDED
  #endif
#elif defined(DLL_VERSION)
  #if !defined(DLL_VERSION_INCLUDED) && !defined(EXE_VERSION_INCLUDED)
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define DLL_VERSION_INCLUDED
  #endif
#endif

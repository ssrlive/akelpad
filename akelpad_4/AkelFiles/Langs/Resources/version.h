// Add version information to EXE

#define AKELPAD_ID 4,2,7,0

#ifdef EXE_VERSION
  #ifndef EXE_VERSION_INCLUDED
    #undef _MAC
    #include "Version.rc"
    #define _MAC
    #define EXE_VERSION_INCLUDED
  #endif
#endif

// Add version information to EXE

#ifdef EXE_VERSION
#undef _MAC
#include "Version.rc"
#define _MAC
#endif

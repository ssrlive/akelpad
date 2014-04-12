# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

ifndef BIT
  BIT = 32
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

ifeq ($(BIT), 64)
  CC = x86_64-w64-mingw32-g++.exe
  RES = x86_64-w64-mingw32-windres.exe
  STRIP = x86_64-w64-mingw32-strip.exe
else
  CC = g++.exe
  RES = windres.exe
  STRIP = strip.exe
endif
CFLAGS = -Os -g0 -Werror -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32 -lgdi32 -lcomdlg32
DEFINES = -D CINTERFACE

all: SpecialChar.dll

SpecialChar.dll: SpecialChar.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) SpecialChar.o Resource.o $(LDFLAGS) -o SpecialChar.dll
	$(STRIP) --strip-all SpecialChar.dll

SpecialChar.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc SpecialChar.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources Resources/SpecialChar.rc -o Resource.o

clean:
	$(DEL) SpecialChar.o
	$(DEL) Resource.o

# Make file for build Redirect with GCC
# mingw32-make.exe BIT=32 all clean

ifndef BIT
  BIT = 32
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

ifeq ($(BIT), 64)
  ENTRY = _WinMain
  CC = x86_64-w64-mingw32-g++.exe
  RES = x86_64-w64-mingw32-windres.exe
  STRIP = x86_64-w64-mingw32-strip.exe
  OUTFILE = notepad-x64.exe
else
  ENTRY = __WinMain
  CC = g++.exe
  RES = windres.exe
  STRIP = strip.exe
  OUTFILE = notepad.exe
endif
CFLAGS = -Os -g0 -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -mwindows -e $(ENTRY) -static-libgcc -lkernel32 -luser32 -ladvapi32 -lcomdlg32
DEFINES = -D CINTERFACE

all: notepad.exe

notepad.exe: Redirect.o Resource.o
	$(CC) $(CFLAGS) Redirect.o Resource.o $(LDFLAGS) -o $(OUTFILE)
	$(STRIP) --strip-all $(OUTFILE)

Redirect.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc Redirect.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources "Resources/Redirect.rc" -o Resource.o

clean:
	$(DEL) Redirect.o
	$(DEL) Resource.o

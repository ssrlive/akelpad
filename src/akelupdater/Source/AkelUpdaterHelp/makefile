# Make file for build AkelUpdaterHelp with GCC
# mingw32-make.exe LANGNAME=English LANGID=0x409 BIT=32 all clean

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
  RES = windres.exe
  STRIP = strip.exe
else
  ENTRY = __WinMain
  CC = g++.exe
  RES = windres.exe
  STRIP = strip.exe
endif
CFLAGS = -Os -g0 -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -mwindows -e $(ENTRY) -static-libgcc -lkernel32 -luser32
DEFINES = -D CINTERFACE

all: AkelUpdaterHelp.exe

AkelUpdaterHelp.exe: AkelUpdaterHelp.o Resource.o
	$(CC) $(CFLAGS) AkelUpdaterHelp.o Resource.o $(LDFLAGS) -o AkelUpdaterHelp.exe
	$(STRIP) --strip-all AkelUpdaterHelp.exe

AkelUpdaterHelp.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc AkelUpdaterHelp.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources Resources/AkelUpdaterHelp.rc -o Resource.o

clean:
	$(DEL) AkelUpdaterHelp.o
	$(DEL) Resource.o

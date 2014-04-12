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
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32 -lshell32
DEFINES = -D CINTERFACE

all: MinimizeToTray.dll

MinimizeToTray.dll: MinimizeToTray.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) MinimizeToTray.o Resource.o $(LDFLAGS) -o MinimizeToTray.dll
	$(STRIP) --strip-all MinimizeToTray.dll

MinimizeToTray.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc MinimizeToTray.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources Resources/MinimizeToTray.rc -o Resource.o

clean:
	$(DEL) MinimizeToTray.o
	$(DEL) Resource.o

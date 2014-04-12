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
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32 -ladvapi32 -lshell32
DEFINES = -D CINTERFACE

all: SaveFile.dll

SaveFile.dll: SaveFile.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) SaveFile.o Resource.o $(LDFLAGS) -o SaveFile.dll
	$(STRIP) --strip-all SaveFile.dll

SaveFile.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc SaveFile.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources Resources/SaveFile.rc -o Resource.o

clean:
	$(DEL) SaveFile.o
	$(DEL) Resource.o

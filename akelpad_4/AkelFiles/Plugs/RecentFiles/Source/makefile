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
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32 -lgdi32 -lcomctl32
DEFINES = -D CINTERFACE

all: RecentFiles.dll

RecentFiles.dll: RecentFiles.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) RecentFiles.o Resource.o $(LDFLAGS) -o RecentFiles.dll
	$(STRIP) --strip-all RecentFiles.dll

RecentFiles.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc RecentFiles.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources Resources/RecentFiles.rc -o Resource.o

clean:
	$(DEL) RecentFiles.o
	$(DEL) Resource.o

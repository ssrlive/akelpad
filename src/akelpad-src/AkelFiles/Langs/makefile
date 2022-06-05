# Make file for build language module DLL with GCC
# mingw32-make.exe LANGNAME=English all clean

ifndef LANGNAME
  LANGNAME = English
  LANGID = 0x409
endif
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
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -nostdlib

all: $(LANGNAME).dll

$(LANGNAME).dll: Module.o Resource.o
	$(CC) $(CFLAGS) Module.o Resource.o $(LDFLAGS) -o $(LANGNAME).dll
	$(STRIP) --strip-all $(LANGNAME).dll

Module.o:
	$(CC) $(CFLAGS) -c -xc Module.c

Resource.o:
	$(RES) --use-temp-file -D DLL_VERSION -D RC_VERSIONLANGID=$(LANGID) -D RC_VERSIONBIT=$(BIT) -I ..\..\AkelEdit\Resources -I Resources Resources/$(LANGNAME).rc -o Resource.o

clean:
	$(DEL) Module.o
	$(DEL) Resource.o

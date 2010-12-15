# Make file for build language module DLL with GCC
# mingw32-make.exe LANGNAME=English all clean

ifndef LANGNAME
  LANGNAME = English
  LANGID = 0x409
  BIT = 32
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -nostdlib

all: $(LANGNAME).dll

$(LANGNAME).dll: Module.o Resource.o
	$(CC) $(CFLAGS) Module.o Resource.o $(LDFLAGS) -o $(LANGNAME).dll
	strip --strip-all $(LANGNAME).dll

Module.o:
	$(CC) $(CFLAGS) -c Module.c

Resource.o:
	windres -D DLL_VERSION -D RC_VERSIONLANGID=$(LANGID) -D RC_VERSIONBIT=$(BIT) -I ..\..\AkelEdit\Resources -I Resources Resources/$(LANGNAME).rc -o Resource.o

clean:
	$(DEL) Module.o
	$(DEL) Resource.o

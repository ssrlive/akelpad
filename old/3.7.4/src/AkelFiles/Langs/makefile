# Make file for build language module DLL with GCC
# mingw32-make.exe LANG=English all clean

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -nostdlib
ifndef LANG
  LANG = English
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

all: $(LANG).dll

$(LANG).dll: Module.o Resource.o
	$(CC) $(CFLAGS) Module.o Resource.o $(LDFLAGS) -o $(LANG).dll
	strip --strip-all $(LANG).dll

Module.o:
	$(CC) $(CFLAGS) -c Module.cpp

Resource.o:
	windres -D DLL_VERSION -I Resources Resources/$(LANG).rc -o Resource.o

clean:
	$(DEL) Module.o
	$(DEL) Resource.o

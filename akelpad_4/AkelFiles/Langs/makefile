# Make file for build language module DLL with GCC
# mingw32-make.exe LANG=English all clean

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -nostdlib
DEL = del
#DEL = rm -f
ifndef LANG
LANG = English
endif

all: $(LANG).dll

$(LANG).dll: Module.o Resource.o
	$(CC) $(CFLAGS) Module.o Resource.o $(LDFLAGS) -o $(LANG).dll
	strip --strip-all $(LANG).dll

Module.o:
	$(CC) $(CFLAGS) -c Module.cpp

Resource.o:
	windres -I Resources Resources/$(LANG).rc -o Resource.o

clean:
	$(DEL) Module.o
	$(DEL) Resource.o

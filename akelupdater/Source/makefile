# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -lkernel32 -luser32
DEL = del
#DEL = rm -f

all: AkelUpdater.dll

AkelUpdater.dll: AkelUpdater.o Resource.o
	$(CC) $(CFLAGS) AkelUpdater.o Resource.o $(LDFLAGS) -o AkelUpdater.dll
	strip --strip-all AkelUpdater.dll

AkelUpdater.o:
	$(CC) $(CFLAGS) -c AkelUpdater.cpp

Resource.o:
	windres -I Resources Resources/AkelUpdater.rc -o Resource.o

clean:
	$(DEL) AkelUpdater.o
	$(DEL) Resource.o

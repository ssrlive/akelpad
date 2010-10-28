# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

CC = g++
CFLAGS = -Os -g0 -Werror -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32
DEFINES = -D CINTERFACE

all: AkelUpdater.dll

AkelUpdater.dll: AkelUpdater.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) AkelUpdater.o Resource.o $(LDFLAGS) -o AkelUpdater.dll
	strip --strip-all AkelUpdater.dll

AkelUpdater.o:
	$(CC) $(CFLAGS) $(DEFINES) -c AkelUpdater.c

Resource.o:
	windres -I Resources Resources/AkelUpdater.rc -o Resource.o

clean:
	$(DEL) AkelUpdater.o
	$(DEL) Resource.o

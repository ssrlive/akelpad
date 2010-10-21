# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

CC = g++
CFLAGS = -Os -g0 -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32 -lgdi32 -lole32 -luuid -limm32
DEFINES = -D CINTERFACE

all: AkelEdit.dll

AkelEdit.dll: AkelEdit.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) AkelEdit.o Resource.o $(LDFLAGS) -o AkelEdit.dll
	strip --strip-all AkelEdit.dll

AkelEdit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc AkelEdit.c

Resource.o:
	windres -I Resources Resources/AkelEdit.rc -o Resource.o

clean:
	$(DEL) AkelEdit.o
	$(DEL) Resource.o

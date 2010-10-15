# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -lkernel32 -luser32 -lgdi32 -lole32 -luuid -limm32
DEFINES = -D CINTERFACE

all: AkelEdit.dll

AkelEdit.dll: AkelEdit.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) AkelEdit.o Resource.o $(LDFLAGS) -o AkelEdit.dll
	strip --strip-all AkelEdit.dll

AkelEdit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c AkelEdit.c

Resource.o:
	windres -I Resources Resources/AkelEdit.rc -o Resource.o

clean:
	$(DEL) AkelEdit.o
	$(DEL) Resource.o

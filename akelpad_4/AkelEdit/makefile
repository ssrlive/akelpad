# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -lkernel32 -luser32 -lgdi32
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

all: AkelEdit.dll

AkelEdit.dll: AkelEdit.o Resource.o
	$(CC) $(CFLAGS) AkelEdit.o Resource.o $(LDFLAGS) -o AkelEdit.dll
	strip --strip-all AkelEdit.dll

AkelEdit.o:
	$(CC) $(CFLAGS) -c AkelEdit.cpp

Resource.o:
	windres -I Resources Resources/AkelEdit.rc -o Resource.o

clean:
	$(DEL) AkelEdit.o
	$(DEL) Resource.o

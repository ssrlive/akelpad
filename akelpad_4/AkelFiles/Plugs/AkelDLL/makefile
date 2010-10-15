# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

CC = g++
CFLAGS = -Wall -Werror -Os
LDFLAGS = -shared -lkernel32 -luser32

all: AkelDLL.dll

AkelDLL.dll: AkelDLL.o
	$(CC) $(CFLAGS) AkelDLL.o $(LDFLAGS) -o AkelDLL.dll
	strip --strip-all AkelDLL.dll

AkelDLL.o:
	$(CC) $(CFLAGS) -c AkelDLL.c

clean:
	$(DEL) AkelDLL.o

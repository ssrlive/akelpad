# Make file for build Redirect with GCC
# mingw32-make.exe LANG=English all clean

CC = g++
CFLAGS = -Wall -Os
LDFLAGS = -mwindows -e __WinMain -lkernel32 -luser32 -ladvapi32 -lcomdlg32
ifndef LANG
  LANG = English
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

all: Redirect.exe

Redirect.exe: Redirect.o Resource.o
	$(CC) $(CFLAGS) Redirect.o Resource.o $(LDFLAGS) -o notepad.exe
	strip --strip-all notepad.exe

Redirect.o:
	$(CC) $(CFLAGS) $(DEFINES) -c Redirect.cpp

Resource.o:
	windres -D EXE_VERSION -I Resources "Resources/Redirect.rc" -o Resource.o

clean:
	$(DEL) Redirect.o
	$(DEL) Resource.o

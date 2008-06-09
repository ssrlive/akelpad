# Make file for build AkelPad with GCC
# mingw32-make.exe LANG=English all clean

CC = g++
CFLAGS = -Wall -Os
LDFLAGS = -mwindows -e __WinMain -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomctl32 -lcomdlg32 -lversion -lole32 -luuid
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif
ifndef LANG
  LANG = English
endif

all: AkelPad.exe

AkelPad.exe: AkelPad.o Edit.o Resource.o
	$(CC) $(CFLAGS) AkelPad.o Edit.o Resource.o $(LDFLAGS) -o AkelPad.exe
	strip --strip-all AkelPad.exe

AkelPad.o:
	$(CC) $(CFLAGS) -c AkelPad.cpp

Edit.o:
	$(CC) $(CFLAGS) -c Edit.cpp

Resource.o:
	windres -D _MAC -I AkelFiles/Langs/Resources "AkelFiles/Langs/Resources/$(LANG).rc" -o Resource.o

clean:
	$(DEL) AkelPad.o
	$(DEL) Edit.o
	$(DEL) Resource.o

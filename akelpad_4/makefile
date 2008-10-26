# Make file for build AkelPad with GCC
# mingw32-make.exe LANG=English all clean

CC = g++
CFLAGS = -Wall -Os
LDFLAGS = -mwindows -e __WinMain -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomctl32 -lcomdlg32 -lole32 -luuid -limm32 -lversion
DEFINES = -D STATIC_BUILD
ifndef LANG
  LANG = English
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

all: AkelPad.exe

AkelPad.exe: AkelPad.o Edit.o AkelEdit.o Resource.o
	$(CC) $(CFLAGS) AkelPad.o Edit.o AkelEdit.o Resource.o $(LDFLAGS) -o AkelPad.exe
	strip --strip-all AkelPad.exe

AkelPad.o:
	$(CC) $(CFLAGS) $(DEFINES) -c AkelPad.cpp

Edit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c Edit.cpp

AkelEdit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c AkelEdit/AkelEdit.cpp

Resource.o:
	windres -D EXE_VERSION -I AkelFiles/Langs/Resources "AkelFiles/Langs/Resources/$(LANG).rc" -o Resource.o

clean:
	$(DEL) AkelPad.o
	$(DEL) Edit.o
	$(DEL) AkelEdit.o 
	$(DEL) Resource.o

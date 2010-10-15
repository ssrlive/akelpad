# Make file for build AkelPad with GCC
# mingw32-make.exe LANGNAME=English all clean

ifndef LANGNAME
  LANGNAME = English
  LANGID = 0x409
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

CC = g++
CFLAGS = -Wall -Os
LDFLAGS = -mwindows -e __WinMain -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomctl32 -lcomdlg32 -lole32 -luuid -limm32 -lversion
DEFINES = -D CINTERFACE -D AKELEDIT_STATICBUILD -D RC_VERSIONLANGID=$(LANGID) 

all: AkelPad.exe

AkelPad.exe: AkelPad.o Edit.o AkelEdit.o Resource.o
	$(CC) $(CFLAGS) AkelPad.o Edit.o AkelEdit.o Resource.o $(LDFLAGS) -o AkelPad.exe
	strip --strip-all AkelPad.exe

AkelPad.o:
	$(CC) $(CFLAGS) $(DEFINES) -c AkelPad.c

Edit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c Edit.c

AkelEdit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c AkelEdit/AkelEdit.c

Resource.o:
	windres -D AKELEDIT_STATICBUILD -D EXE_VERSION -D RC_VERSIONLANGID=$(LANGID) -I AkelEdit\Resources -I AkelFiles/Langs/Resources "AkelFiles/Langs/Resources/$(LANGNAME).rc" -o Resource.o

clean:
	$(DEL) AkelPad.o
	$(DEL) Edit.o
	$(DEL) AkelEdit.o
	$(DEL) Resource.o

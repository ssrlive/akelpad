# Make file for build AkelPad with GCC
# mingw32-make.exe LANGNAME=English LANGID=0x409 BIT=32 all clean

ifndef LANGNAME
  LANGNAME = English
  LANGID = 0x409
endif
ifndef BIT
  BIT = 32
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

ifeq ($(BIT), 64)
  ENTRY = _WinMain
  CC = x86_64-w64-mingw32-g++.exe
  RES = x86_64-w64-mingw32-windres.exe
  STRIP = x86_64-w64-mingw32-strip.exe
else
  ENTRY = __WinMain
  CC = g++.exe
  RES = windres.exe
  STRIP = strip.exe
endif
CFLAGS = -Os -g0 -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -mwindows -e $(ENTRY) -static-libgcc -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomctl32 -lcomdlg32 -lole32 -loleaut32 -luuid -limm32 -lversion
DEFINES = -D CINTERFACE -D AKELEDIT_STATICBUILD -D RC_VERSIONLANGID=$(LANGID)

all: AkelPad.exe

AkelPad.exe: AkelPad.o Edit.o AkelEdit.o Resource.o
	$(CC) $(CFLAGS) AkelPad.o Edit.o AkelEdit.o Resource.o $(LDFLAGS) -o AkelPad.exe
	$(STRIP) --strip-all AkelPad.exe

AkelPad.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc AkelPad.c

Edit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc Edit.c

AkelEdit.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc AkelEdit/AkelEdit.c

Resource.o:
	$(RES) --use-temp-file -D AKELEDIT_STATICBUILD -D RC_EXEVERSION -D RC_VERSIONLANGID=$(LANGID) -D RC_VERSIONBIT=$(BIT) -I AkelEdit/Resources -I AkelFiles/Langs/Resources "AkelFiles/Langs/Resources/$(LANGNAME).rc" -o Resource.o

clean:
	$(DEL) AkelPad.o
	$(DEL) Edit.o
	$(DEL) AkelEdit.o
	$(DEL) Resource.o

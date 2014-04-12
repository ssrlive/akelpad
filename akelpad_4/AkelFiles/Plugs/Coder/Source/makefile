# Make file for build plugin DLL with GCC
# mingw32-make.exe all clean

ifndef BIT
  BIT = 32
endif
ifndef NIX
  DEL = del
else
  DEL = rm -f
endif

ifeq ($(BIT), 64)
  CC = x86_64-w64-mingw32-g++.exe
  RES = x86_64-w64-mingw32-windres.exe
  STRIP = x86_64-w64-mingw32-strip.exe
else
  CC = g++.exe
  RES = windres.exe
  STRIP = strip.exe
endif
CFLAGS = -Os -g0 -Werror -Wchar-subscripts -Wcomment -Wformat -Wreturn-type -Wsign-compare -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
LDFLAGS = -shared -static-libgcc -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lcomctl32
DEFINES = -D CINTERFACE

all: Coder.dll

Coder.dll: Coder.o HighLight.o CodeFold.o AutoComplete.o Resource.o
	$(CC) $(CFLAGS) $(DEFINES) Coder.o HighLight.o CodeFold.o AutoComplete.o Resource.o $(LDFLAGS) -o Coder.dll
	$(STRIP) --strip-all Coder.dll

Coder.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc Coder.c

HighLight.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc HighLight.c

CodeFold.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc CodeFold.c

AutoComplete.o:
	$(CC) $(CFLAGS) $(DEFINES) -c -xc AutoComplete.c

Resource.o:
	$(RES) --use-temp-file -D RC_VERSIONBIT=$(BIT) -I Resources Resources/Coder.rc -o Resource.o

clean:
	$(DEL) Coder.o
	$(DEL) HighLight.o
	$(DEL) CodeFold.o
	$(DEL) AutoComplete.o
	$(DEL) Resource.o

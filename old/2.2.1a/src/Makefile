all: dirs release.ru\akelpad.exe release.en\akelpad.exe

dirs:
	if not exist release.ru md release.ru
	if not exist release.en md release.en
	copy rewrite.bat release.ru\rewrite.bat
	copy rewrite.bat release.en\rewrite.bat

release.ru\akelpad.exe: release.ru\akelpad.obj release.ru\edit.obj release.ru\codepage.obj release.ru\lang.obj release.ru\akelpad.res
	link /SUBSYSTEM:WINDOWS release.ru\akelpad.obj release.ru\edit.obj release.ru\codepage.obj release.ru\lang.obj release.ru\akelpad.res kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib comctl32.lib comdlg32.lib /OUT:release.ru\akelpad.exe /FILEALIGN:512
	
release.ru\akelpad.obj: akelpad.cpp akelpad.h edit.h codepage.h langpack.h
	cl /O1 /MD /c akelpad.cpp /Forelease.ru\akelpad.obj

release.ru\edit.obj: edit.cpp akelpad.h edit.h codepage.h langpack.h
	cl /O1 /MD /c edit.cpp /Forelease.ru\edit.obj

release.ru\codepage.obj: codepage.cpp akelpad.h edit.h codepage.h langpack.h
	cl /O1 /MD /c codepage.cpp /Forelease.ru\codepage.obj

release.ru\lang.obj: lang_ru.cpp langpack.h
	cl /O1 /MD /c lang_ru.cpp /Forelease.ru\lang.obj

release.ru\akelpad.res: akelpad.rc
	rc /r /Forelease.ru\akelpad.res akelpad.rc

release.en\akelpad.exe: release.en\akelpad.obj release.en\edit.obj release.en\codepage.obj release.en\lang.obj release.en\akelpad.res
	link /SUBSYSTEM:WINDOWS release.en\akelpad.obj release.en\edit.obj release.en\codepage.obj release.en\lang.obj release.en\akelpad.res kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib comctl32.lib comdlg32.lib /OUT:release.en\akelpad.exe /FILEALIGN:512
	
release.en\akelpad.obj: akelpad.cpp akelpad.h edit.h codepage.h langpack.h
	cl /O1 /MD /c /DFOREIGN_BUILD akelpad.cpp /Forelease.en\akelpad.obj

release.en\edit.obj: edit.cpp akelpad.h edit.h codepage.h langpack.h
	cl /O1 /MD /c /DFOREIGN_BUILD edit.cpp /Forelease.en\edit.obj

release.en\codepage.obj: codepage.cpp akelpad.h edit.h codepage.h langpack.h
	cl /O1 /MD /c /DFOREIGN_BUILD codepage.cpp /Forelease.en\codepage.obj

release.en\lang.obj: lang_en.cpp langpack.h
	cl /O1 /MD /c lang_en.cpp /Forelease.en\lang.obj

release.en\akelpad.res: akelpad_en.rc
	rc /r /Forelease.en\akelpad.res akelpad_en.rc


clean:
	del /f /q release.ru\*.*
	del /f /q release.en\*.*
	rmdir release.ru
	rmdir release.en
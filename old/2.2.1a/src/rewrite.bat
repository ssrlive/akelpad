@echo off
del c:\windows\notepad.exe
del c:\windows\system32\notepad.exe
del c:\windows\system32\dllcache\notepad.exe
copy akelpad.exe c:\windows\notepad.exe
copy akelpad.exe c:\windows\system32\notepad.exe
copy akelpad.exe c:\windows\system32\dllcache\notepad.exe
@echo off
cl /W3 /GS /Gd /Zi /Od /Ob0 /MDd /TC /Fe: base64.exe /Fd: base64.pdb base64.c
del *.ilk *.obj

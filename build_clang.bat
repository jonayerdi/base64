@echo off
clang -Wall -Wpedantic -Wno-deprecated -Wno-int-conversion base64.c -o base64.exe

@echo off
clang -O3 -Wall -Wpedantic -Wno-deprecated base64.c base64_app.c -o base64.exe

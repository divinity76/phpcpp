@echo off
:begin
echo if you see any errors below, that's bad.
g++ -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion tests.cpp
a.exe
pause
cls
goto :begin
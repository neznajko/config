@echo off
rem This compiles all .cpp files, suppresses all warnings, and outputs myprogram.exe
cl *.cpp /W0 /EHsc /Fe:config.exe
pause

@echo off
echo Glich Unit Tests Debug
..\build\vc-working\Debug\glcunit.exe
echo.
cd ..\3rdparty\glich\test\glcscripts
..\..\..\..\build\vc-working\Debug\glctest.exe suite
echo.
pause
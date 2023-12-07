@echo off
echo Glich Unit Tests Release
..\build\vc-working\Release\glcunit.exe
echo.
cd ..\3rdparty\glich\test\glcscripts
..\..\..\..\build\vc-working\Release\glctest.exe suite
echo.
pause
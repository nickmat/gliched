@echo off
echo Glich Unit Tests Release
..\..\tfp\build\vc-working\Release\glcunit.exe
echo.
cd glcscripts
..\..\..\tfp\build\vc-working\Release\glctest.exe suite
echo.
pause
@echo off
echo Glich Unit Tests Debug
..\..\tfp\build\vc-working\Debug\glcunit.exe
echo.
cd glcscripts
..\..\..\tfp\build\vc-working\Debug\glctest.exe suite
echo.
pause
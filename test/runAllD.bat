@echo off
echo Glich Unit Tests Debug
..\build\vc-working\Debug\glcunit.exe
echo.
cd ..\3rdparty\glich\test\glcscripts\suite
..\..\..\..\..\build\vc-working\Debug\glctest.exe .\
cd ..\..\..\..\
echo.
echo Glich Soak Tests Debug
..\build\vc-working\Debug\glcsoak.exe --test-type Short
echo.
pause
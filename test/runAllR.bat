@echo off
echo Glich Unit Tests Release
..\build\vc-working\Release\glcunit.exe
echo.
cd ..\3rdparty\glich\test\glcscripts\suite
..\..\..\..\..\build\vc-working\Release\glctest.exe .\
cd ..\..\..\..\
echo.
echo Glich Soak Tests Release
..\build\vc-working\Release\glcsoak.exe --test-type Short
echo.
pause
@echo off

echo Run WebPageLayout

xcopy /Q /S /I /Y ..\docs ..\web-new

wpl --version

wpl --source ../docs --target .. --layout layout.json

pause

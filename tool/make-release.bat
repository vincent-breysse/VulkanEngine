
@echo off

:: PARAMS ----------------------------------

set root=..
set assets=%root%\working
set bin=%root%\bin\x64\release
set out=..\release

:: -----------------------------------------

if exist %out% (
	
echo Overwrite the directory %out%
rmdir /s %out%
echo.

)

echo Creating %out% directory
mkdir %out%
echo.

echo Copying .dll files from %bin% to %out% directory
copy  %bin%\*.dll %out%
echo.

echo Copying .exe files from %bin% to %out% directory
copy  %bin%\*.exe %out%
echo.

echo Copying .pdb files from %bin% to %out% directory
copy  %bin%\*.pdb %out%
echo.

echo Copying content of %assets% in %out% directory
xcopy %assets% %out% /E
echo.

echo Removing files starting with "test-" in their name from %out% directory
del /s %out%\test-*
echo.

echo Removing .dll files containing "-d" in their name from %out% directory
del /s %out%\*-d*.dll
echo.

pause
@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

cd "!ROOTDIR!..\3rdparty\lzma\windows\vs2017"

if errorlevel 1 exit /B 1

set CONFIGURATION=%1
set PLATFORM=%2
set BUILD_CONFIG=%CONFIGURATION%
set PLATFORM_TARGET=%PLATFORM%

rem MSBuild xz_win.sln /t:Clean /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
devenv xz_win.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" 

if errorlevel 1 goto :END

call :DOERASE "!ROOTDIR!..\3rdparty\include\lzma.h"
call :DOERASE "!ROOTDIR!..\3rdparty\include\lzma"
call :DOERASE "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\liblzma.lib"
call :DOERASE "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\liblzma.pdb"
call :DOERASE "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\liblzma_dll.lib"
call :DOERASE "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!\liblzma.dll"
call :DOERASE "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!\liblzma.pdb"

:DOERASE
if exist "%~1" erase /S "%~1" > nul

:END
endlocal

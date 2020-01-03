@echo on

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

set "SLNDIR=!ROOTDIR!..\3rdparty\zstd\build\VS2017"

cd !SLNDIR!
if errorlevel 1 exit /B 1

set CONFIGURATION=%1
set PLATFORM=%2
set BUILD_CONFIG=!CONFIGURATION!
set PLATFORM_TARGET=!PLATFORM!

if "!PLATFORM_TARGET!"=="Win32" (
	set PLATFORM_TARGET=x86
)

if "!CONFIGURATION!"=="ReleaseMT" (
	set BUILD_CONFIG=Release-Static
)

rem devenv !SLNDIR!\zstd.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" /project libzstd 
MSBuild zstd.sln /m /t:libzstd:Clean;libzstd-dll:Clean /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
if errorlevel 1 exit /B 2

rem devenv !SLNDIR!\zstd.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" /project libzstd-dll
rem if errorlevel 1 exit /B 3

erase "!ROOTDIR!..\3rdparty\include\zstd.h"
if errorlevel 1 exit /B 4

erase "!ROOTDIR!..\3rdparty\bin\!PLATFORM!_!BUILD_CONFIG!\libzstd*.*" > nul
erase "!ROOTDIR!..\3rdparty\lib\!PLATFORM!_!BUILD_CONFIG!\libzstd*.*" > nul
erase "!ROOTDIR!..\3rdparty\bin\!PLATFORM!_!BUILD_CONFIG!\libzstd*.*" > nul

goto :END

:DOERASE
if exist "%~1" (
	erase "%~1"
	if errorlevel 1 exit /b 99
)

:END
endlocal

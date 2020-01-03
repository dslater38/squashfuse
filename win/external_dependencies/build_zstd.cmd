@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

set "SLNDIR=!ROOTDIR!..\3rdparty\zstd\build\VS2017"

cd !SLNDIR!
if errorlevel 1 exit /B 1

set CONFIGURATION=%1
set PLATFORM=%2
set BUILD_CONFIG=%CONFIGURATION%
set PLATFORM_TARGET=%PLATFORM%

if "%PLATFORM_TARGET%"=="Win32" (
	set PLATFORM_TARGET=x86
)

if "%CONFIGURATION%"=="ReleaseMT" (
	set BUILD_CONFIG=Release-Static
)

if not exist "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\libzstd.lib" goto :BUILD
if not exist "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\libzstd_static.lib" goto :BUILD
if not exist "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!\libzstd.dll" goto :BUILD

goto :END

:BUILD
rem MSBuild zlibvc.sln /m /t:zlibstat:Clean;zlibvc:Clean /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
MSBuild zstd.sln /m /t:libzstd;libzstd-dll /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
rem devenv zstd.sln /Build "!BUILD_CONFIG!|!PLATFORM!" /project libzstd 
if errorlevel 1 exit /B 2

rem devenv zstd.sln /Build "!BUILD_CONFIG!|!PLATFORM!" /project libzstd-dll
rem if errorlevel 1 exit /B 3


copy "!ROOTDIR!..\3rdparty\zstd\lib\zstd.h" "!ROOTDIR!..\3rdparty\include" > nul
if errorlevel 1 exit /B 4

copy "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\*.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 5

copy "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\*.pdb" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 6

copy "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\*.dll" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 7

:END
endlocal

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

MSBuild zstd.sln /m /t:libzstd;libzstd-dll /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
if errorlevel 1 exit /B 2

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

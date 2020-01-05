@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

cd "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15"

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

MSBuild zlibvc.sln /m /t:zlibstat;zlibvc /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!

if errorlevel 1 exit /B 2

copy "!ROOTDIR!..\3rdparty\zlib\zlib.h" "!ROOTDIR!..\3rdparty\include"
if errorlevel 1 exit /B 4

copy "!ROOTDIR!..\3rdparty\zlib\zconf.h" "!ROOTDIR!..\3rdparty\include"
if errorlevel 1 exit /B 5

set "ZLIBSTATOUT=!PLATFORM_TARGET!\ZlibStat!BUILD_CONFIG!"
set "ZLIBDLLOUT=!PLATFORM_TARGET!\ZlibDll!BUILD_CONFIG!"

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBSTATOUT!\zlibstat.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!"
if errorlevel 1 exit /B 6

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBDLLOUT!\zlibwapi.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!"
if errorlevel 1 exit /B 7

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBDLLOUT!\zlibwapi.dll" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!"
if errorlevel 1 exit /B 8

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBDLLOUT!\zlibwapi.pdb" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!"
if errorlevel 1 exit /B 9


:END
endlocal

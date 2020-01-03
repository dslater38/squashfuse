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

rem MSBuild zlibstat.vcxproj /t:Build /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
rem devenv zlibvc.sln /Build "!BUILD_CONFIG!|!PLATFORM!" /project zlibstat 
MSBuild zlibvc.sln /m /t:zlibstat;zlibvc /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!

if errorlevel 1 exit /B 2

rem MSBuild zlibvc.vcxproj /t:Build /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
rem devenv zlibvc.sln /Build "!BUILD_CONFIG!|!PLATFORM!" /project zlibvc
rem if errorlevel 1 exit /B 3

copy "!ROOTDIR!..\3rdparty\zlib\zlib.h" "!ROOTDIR!..\3rdparty\include" > nul
if errorlevel 1 exit /B 4

copy "!ROOTDIR!..\3rdparty\zlib\zconf.h" "!ROOTDIR!..\3rdparty\include" > nul
if errorlevel 1 exit /B 5

set "ZLIBSTATOUT=!PLATFORM!\ZlibStat!BUILD_CONFIG!"
set "ZLIBDLLOUT=!PLATFORM!\ZlibDll!BUILD_CONFIG!"

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBSTATOUT!\zlibstat.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 6

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBSTATOUT!\zlibstat.pdb" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 7


copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBDLLOUT!\zlibwapi.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 8

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBDLLOUT!\zlibwapi.dll" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 9

copy "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15\!ZLIBDLLOUT!\zlibwapi.pdb" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 10


:END
endlocal

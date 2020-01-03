@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

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

set "ZLIBSTATOUT=!PLATFORM!\ZlibStat!BUILD_CONFIG!"
set "ZLIBDLLOUT=!PLATFORM!\ZlibDll!BUILD_CONFIG!"

call :DOERASE "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\zlibstat.lib"
call :DOERASE "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\zlibstat.pdb"
call :DOERASE "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\zlibwapi.lib"
call :DOERASE "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!\zlibwapi.dll"
call :DOERASE "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!\zlibwapi.pdb"

cd "!ROOTDIR!..\3rdparty\zlib\contrib\vstudio\vc15"
if errorlevel 1 goto :END

rem MSBuild zlibstat.vcxproj /t:Clean /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
devenv zlibvc.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" /project zlibstat 
if errorlevel 1 goto :END

rem MSBuild zlibvc.vcxproj /t:Clean /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
devenv zlibvc.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" /project zlibvc

goto :END

:DOERASE
if exist "%~1" erase "%~1""

:END
endlocal

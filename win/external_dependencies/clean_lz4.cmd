@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

set "SLNDIR=!ROOTDIR!..\3rdparty\lz4\visual\VS2017"

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

rem devenv lz4.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" /project lz4   /project liblz4-dll
rem MSBuild /target:Clean /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! liblz4
rem MSBuild /target:Clean /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! /property:TargetName=liblz4 lz4.sln
MSBuild lz4.sln /m /t:liblz4:Clean;liblz4-dll:Clean /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM!
if errorlevel 1 exit /B 2

rem MSBuild /target:Clean /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! liblz4-dll
rem MSBuild /target:Clean /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! /property:TargetName=liblz4-dll lz4.sln
rem if errorlevel 1 exit /B 3

rem devenv lz4.sln /Clean "!BUILD_CONFIG!|!PLATFORM!" /project liblz4-dll
rem if errorlevel 1 exit /B 3

call :ERASE "!ROOTDIR!..\3rdparty\include\lz4.h"
call :ERASE "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\liblz4.*"
call :ERASE "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\liblz4_static.*"


:ERASE
if exist "%~1" erase "%~1"
goto :EOF

:END
endlocal

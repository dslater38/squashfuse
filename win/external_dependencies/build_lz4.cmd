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

if exist "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\liblz4.lib" goto :END

rem MSBuild zlibstat.vcxproj /t:Build /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
rem MSBuild /target:Build /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! liblz4
MSBuild lz4.sln /m /t:liblz4;liblz4-dll /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM!
rem MSBuild /target:Build /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! /property:TargetName=liblz4 lz4.sln
rem devenv lz4.sln /Build "!BUILD_CONFIG!|!PLATFORM!" /project lz4 
if errorlevel 1 exit /B 2

rem MSBuild zlibvc.vcxproj /t:Build /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
rem MSBuild /target:Build /property:Configuration=!BUILD_CONFIG! /property:Platform=!PLATFORM! /property:TargetName=liblz4-dll lz4.sln
rem devenv lz4.sln /Build "!BUILD_CONFIG!|!PLATFORM!" /project liblz4-dll
rem if errorlevel 1 exit /B 3


copy "!ROOTDIR!..\3rdparty\lz4\lib\lz4.h" "!ROOTDIR!..\3rdparty\include" > nul
if errorlevel 1 exit /B 4

copy "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\*.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 5

copy "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\*.pdb" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 6

copy "!SLNDIR!\bin\!PLATFORM!_!BUILD_CONFIG!\*.dll" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 7

:END
endlocal

@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

cd "!ROOTDIR!..\3rdparty\lzma\windows\vs2017"

if errorlevel 1 exit /B 1

set CONFIGURATION=%1
set PLATFORM=%2
set BUILD_CONFIG=%CONFIGURATION%
set PLATFORM_TARGET=%PLATFORM%

MSBuild xz_win.sln /m /t:Build /P:Configuration=!BUILD_CONFIG! /p:Platform=!PLATFORM!
if errorlevel 1 exit /B 2

copy "!ROOTDIR!..\3rdparty\lzma\src\liblzma\api\lzma.h" "!ROOTDIR!..\3rdparty\include" > nul
if errorlevel 1 exit /B 3
xcopy "!ROOTDIR!..\3rdparty\lzma\src\liblzma\api\lzma" "!ROOTDIR!..\3rdparty\include\lzma" /S /I /Y > NUL
if errorlevel 1 exit /B 4

set "LZMASTATOUT=!BUILD_CONFIG!\!PLATFORM!\liblzma"
set "LZMADLLOUT=!BUILD_CONFIG!\!PLATFORM!\liblzma_dll"

copy "!ROOTDIR!..\3rdparty\lzma\windows\vs2017\!BUILD_CONFIG!\!PLATFORM!\liblzma\liblzma.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 5

copy "!ROOTDIR!..\3rdparty\lzma\windows\vs2017\!BUILD_CONFIG!\!PLATFORM!\liblzma\liblzma.pdb" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 6

copy "!ROOTDIR!..\3rdparty\lzma\windows\vs2017\!BUILD_CONFIG!\!PLATFORM!\liblzma_dll\liblzma.lib" "!ROOTDIR!..\3rdparty\lib\!PLATFORM!\!CONFIGURATION!\liblzma_dll.lib" > nul
if errorlevel 1 exit /B 7

copy "!ROOTDIR!..\3rdparty\lzma\windows\vs2017\!BUILD_CONFIG!\!PLATFORM!\liblzma_dll\liblzma.dll" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 8

copy "!ROOTDIR!..\3rdparty\lzma\windows\vs2017\!BUILD_CONFIG!\!PLATFORM!\liblzma_dll\liblzma.pdb" "!ROOTDIR!..\3rdparty\bin\!PLATFORM!\!CONFIGURATION!" > nul
if errorlevel 1 exit /B 9


:END
endlocal

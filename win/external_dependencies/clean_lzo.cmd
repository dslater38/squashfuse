@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOTDIR=%~dp0"

set CONFIGURATION=%1
set PLATFORM=%2
set BUILD_CONFIG=%CONFIGURATION%
set PLATFORM_TARGET=%PLATFORM%

if "!CONFIGURATION!"=="ReleaseMT" (
	set BUILD_CONFIG=Release
)


if "%PLATFORM%"=="x64" (
set "TOOL_SET=Visual Studio 15 2017 Win64" 
) else (
set "TOOL_SET=Visual Studio 15 2017" 
)

if "!BUILD_CONFIG!"=="ReleaseMT" (

set "CMAKE_C_FLAGS_DEBUG=/MTd /Zi /Od /Ob0 /RTC1 /D_DEBUG"
set "CMAKE_C_FLAGS_MINSIZEREL=/MT  /O1 /Ob1 /DNDEBUG"
set "CMAKE_C_FLAGS_RELEASE=/MT  /O2 /Ob2 /DNDEBUG"
set "CMAKE_C_FLAGS_RELWITHDEBINFO=/MT /Zi /O2 /Ob1 /DNDEBUG"

) else (
set "CMAKE_C_FLAGS_DEBUG=/MDd /Zi /Od /Ob0 /RTC1 /D_DEBUG"
set "CMAKE_C_FLAGS_MINSIZEREL=/MD  /O1 /Ob1 /DNDEBUG"
set "CMAKE_C_FLAGS_RELEASE=/MD /O2 /Ob2 /DNDEBUG"
set "CMAKE_C_FLAGS_RELWITHDEBINFO=/MD /Zi /O2 /Ob1 /DNDEBUG"
)

set "BUILDDIR=!ROOTDIR!..\3rdparty\lzo\build\!PLATFORM!"

call :DOMKDIR "!ROOTDIR!..\3rdparty\lzo\build"
call :DOMKDIR "!BUILDDIR!"

cd "!BUILDDIR!"
if errorlevel 1 exit /B 1

if exist lzo.sln (
	cmake --build . --target clean --config !BUILD_CONFIG!
	if errorlevel 1 exit /B 2
)

goto :END

:DOMKDIR
if not exist "%~1" ( 
	mkdir "%~1"
	if errorlevel 1 exit /b 99
)

:END
endlocal

@echo off

if "%SQFS_DEBUG%"=="1" echo on

call "%~dp0findWinfsp.cmd""

set "SCRIPTPATH=%~dp0"
set "SCRIPTPATH=%SCRIPTPATH:~0,-1%"

set "PATH=%PATH%;%WinFspInstallDir%bin;%SCRIPTPATH%"

set "SCRIPTPATH="



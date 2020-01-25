@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

if "%SQFS_DEBUG%"=="1" echo on


if "%1"=="-?" goto USAGE

call findWinfsp.cmd

if errorlevel 1 goto :END

if "%1"=="" goto LIST

set "SQUASHFILE=%~f1"
set "SQUASHDIR=%~f2"

if "!SQUASHDIR!"=="" (
    set "SQUASHDIR=%~dpn1"
)


for /F %%I in ( "!SQUASHDIR!" )  do (
	rem if we were passed a drive i.e .z:
	if "%%~dI"=="%%~I" (
		set  "SQUASHINST=%%~dI"
	) else (
		rem if we were passed a drive\ i.e. z:\
		if "%%~dI\"=="%%~I" (
			set  "SQUASHINST=%%~dI"
		) else (
			set "SQUASHINST=%%~nI"
		)
	)	
)

for /F %%I in ('whoami.exe') do (
    set "SQUASHUSERNAME=--UserName=%%~I"
    set "SQUASHGROUPNAME=--GroupName=%%~I"
)

"!LAUNCHCTL!" start squashfs !SQUASHINST! !SQUASHFILE! !SQUASHDIR! !SQUASHUSERNAME! !SQUASHGROUPNAME!


goto :END

:LIST
rem change the forground color to bright green
FORFILES.EXE /P %~dps0 /M %~nxs0 /C "CMD.EXE /C ECHO 0x1B[1;32m
"!LAUNCHCTL!" list | more /E +1
rem reset the colors
FORFILES.EXE /P %~dps0 /M %~nxs0 /C "CMD.EXE /C ECHO 0x1B[0;m

:USAGE
echo Usage: mount [squashfs_file] [directory]
echo mounts squashfs file [squashfs_file] on directory mount point [directory]
echo Note: unlike Unix mount, [directory] must not exist.
echo If run with no arguments, displays this help message and lists all mounted 
echo filesystems.



:END

endlocal

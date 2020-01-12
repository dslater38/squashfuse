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
"!LAUNCHCTL!" list 

:USAGE
echo Usage: mount [squashfs_file] [directory]
echo mounts squashfs file [squashfs_file] on directory mount point [directory]
echo Note: unlike Unix mount, [directory] must not exist.


:END

endlocal

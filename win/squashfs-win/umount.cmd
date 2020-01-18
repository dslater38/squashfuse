@echo off

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

if "%SQFS_DEBUG%"=="1" echo on


if "%1"=="" goto USAGE
if "%1"=="-?" goto USAGE

call findWinfsp.cmd

if errorlevel 1 goto :END

set "SQUASHDIR=%1"


for /F %%I in ( "%SQUASHDIR%" )  do (
	rem if we were passed a drive i.e .z:
	if /I "%%~dI"=="%%~I" (
		set  "SQUASHINST=%%~dI"
	) else (
		rem if we were passed a drive\ i.e. z:\
		if /I "%%~dI\"=="%%~I" (
			set  "SQUASHINST=%%~dI"
		) else (
			set "SQUASHINST=%%~nI"
		)
	)	
)

"!LAUNCHCTL!" stop squashfs !SQUASHINST!

goto :END

:USAGE
echo Usage: umount [directory]
echo umounts squashfs file system mounted on [directory]


:END
endlocal

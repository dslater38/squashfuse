@echo off

if "%SQFS_DEBUG%"=="1" echo on

setlocal 

if "%1"=="-?" goto USAGE

call findWinfsp.cmd

if errorlevel 1 goto :END

if "%1"=="" goto LIST

rem for /f "tokens=2*" %%I in ('reg QUERY HKLM\SOFTWARE\WOW6432Node\WinFsp -v InstallDir ^| grep InstallDir') do SET "WinFspInstallDir=%%J"

rem if "%WinFspInstallDir:~-1%" neq "\" (
rem
rem set "WinFspInstallDir=%WinFspInstallDir%\"
rem
rem )

set "SQUASHFILE=%~f1"
set "SQUASHDIR=%~f2"

if "%SQUASHDIR%"=="" (
    set "SQUASHDIR=%~dpn1"
)


for /F %%I in ( "%SQUASHDIR%" )  do (
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


"%WinFspInstallDir%bin\launchctl-%ARCH%.exe" start squashfs %SQUASHINST% %SQUASHFILE% %SQUASHDIR%




goto :END

:LIST
"%WinFspInstallDir%bin\launchctl-%ARCH%.exe" list 

:USAGE
echo Usage: mount [squashfs_file] [directory]
echo mounts squashfs file [squashfs_file] on directory mount point [directory]
echo Note: unlike Unix mount, [directory] must not exist.


:END

endlocal

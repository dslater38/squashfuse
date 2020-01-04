@echo off

if "%SQFS_DEBUG%"=="1" echo on

setlocal

call findWinFsp.cmd

if errorlevel 1 goto :END

set "SQUASHFS_KEY=%WINFSP_REG_PATH%\Services\squashfs"

reg QUERY %SQUASHFS_KEY% > nul 2>&1

if errorlevel 1 goto :DELETED

reg delete %SQUASHFS_KEY% /f 
if errorlevel 1 goto :END

:DELETED

reg add %SQUASHFS_KEY% /ve /t REG_SZ
if errorlevel 1 goto :END

reg add %SQUASHFS_KEY% /v CommandLine /t REG_SZ /d "%%1 %%2"
if errorlevel 1 goto :END

reg add %SQUASHFS_KEY% /v Executable /t REG_SZ /d "%~dp0squashfuse.exe"
if errorlevel 1 goto :END

reg add %SQUASHFS_KEY% /v JobControl /t REG_DWORD /d 1
if errorlevel 1 goto :END

reg add %SQUASHFS_KEY% /v Security /T REG_SZ /d "D:P(A;;RPWPLC;;;WD)"


:END

endlocal

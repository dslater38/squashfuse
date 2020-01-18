if "%ProgramW6432%"=="" (
	set ARCH=x86
	set "WINFSP_REG_PATH=HKLM\SOFTWARE\WinFsp"
) else (
	set ARCH=x64
	set "WINFSP_REG_PATH=HKLM\SOFTWARE\WOW6432Node\WinFsp"
)

for /f "tokens=2*" %%I in ('reg QUERY %WINFSP_REG_PATH% -v InstallDir ^| grep InstallDir') do SET "WinFspInstallDir=%%J"

if "%WinFspInstallDir%" == "" (
	echo Failed to find WinFsp installation.
	echo Please install WinFsp http://www.secfs.net/winfsp/download/
	exit /b 1
) else (

	if "%WinFspInstallDir:~-1%" neq "\"  set "WinFspInstallDir=%WinFspInstallDir%\"
    
    set "LAUNCHCTL=%WinFspInstallDir%bin\launchctl-%ARCH%.exe"
)

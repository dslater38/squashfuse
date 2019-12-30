@echo off

set PATH=c:\PROGRA~2\WinFsp\bin;%PATH%

echo %PATH%

"c:\Program Files (x86)\WinFsp\bin\launchctl-x64.exe" start squashfs %~n2 %1 %2










goto :EOF

:USAGE
echo Usage: mount [squashfs_file] [directory]
echo mounts squashfs file [squashfs_file] on directory mount point [directory]
echo Note: unlike Unix mount, [directory] must not exist.

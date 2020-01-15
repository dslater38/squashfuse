call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
msbuild /verbosity:normal /property:Configuration=ReleaseMT /property:Platform=x86 /t:Build /maxcpucount:2 .\win\external_dependencies\external_dependencies.vcxproj
if errorlevel 1 exit /b 1
msbuild /verbosity:normal /property:Project=squashfs-win /property:Configuration=ReleaseMT /property:Platform=x86 /t:Build /maxcpucount:2 .\win\squashfuse.sln
if errorlevel 1 exit /b 2

exit /b 0


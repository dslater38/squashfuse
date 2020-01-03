@echo on

setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

set "ROOT_DIR=%~dp0"

cd "%ROOT_DIR%..\3rdparty"

if not exist lib mkdir lib
if not exist bin mkdir bin




endlocal

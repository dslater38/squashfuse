c:\bin\Synchronize.csh



if  ("$csh_layer"=="1") then
    echo starting CSH
    start "" csh.exe -c test.csh
else

for i = 1 to 20 do
    ( cyg bash foo.sh > nul ) &
    sleep 1
end


# CreateSemaphore test 10

# for i = 1 to 10 do
#     for j = 1 to 5 do 
#         AcquireSLock test
#        ( ls -lr d:\newlib > nul ; ReleaseSLock test) &
#       AcquireSLock test
#       ( cyg bash foo.sh > nul ; ReleaseSLock test)  &
#   end
#     d:\Cygwin64\bin\find '/cygdrive/d/newlib/' -type f -exec /cygdrive/c/bin/cat.exe ^'^{^}^' ^; 
# 
#
# WaitOnSLock test
# DestroySemaphore test

end


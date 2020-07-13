@echo off
chcp 65001 1>nul 2>nul
set "EXIT_CODE=0"
set "URL="

pushd "%~sdp0"

::----------------- keep one-last-complete-download. on partial-download continue script (to complete)
if not exist "*.aria2" ( 
  del /f /q "chromebinsync.7z.old"                              1>nul 2>nul
  ren "chromebinsync.7z" "chromebinsync.7z.old"               1>nul 2>nul
) 

echo. 1>&2
echo [INFO] getting url... 1>&2
for /f "tokens=*" %%a in ('call get_most_updated_url.cmd 2^>nul') do ( set "URL=%%a" )
set "EXIT_CODE=%ErrorLevel%"
if ["%EXIT_CODE%"] NEQ ["0"] ( goto ERROR_URL )
if ["%URL%"] EQU [""]        ( goto ERROR_URL )

echo. 1>&2
echo [INFO] starting to download [%URL%] 1>&2
call "..\bin\aria2c.exe" --split=8 --min-split-size=1M --max-connection-per-server=16 --enable-http-pipelining=true --enable-http-keep-alive=true --file-allocation=prealloc --enable-mmap=true --continue=true --allow-overwrite=false --auto-file-renaming=false --user-agent="Mozilla/5.0 Windows" --check-certificate=false "--dir=." "--out=chromebinsync.7z"  "%URL%"
set "EXIT_CODE=%ErrorLevel%"
if ["%EXIT_CODE%"] NEQ ["0"] ( goto ERROR_DOWNLOAD )
if exist "chromebinsync.7z.aria2" ( goto ERROR_DOWNLOAD )
if not exist "chromebinsync.7z" ( goto ERROR_DOWNLOAD )


call "..\bin\rm.exe" -dfr "..\..\application"
if exist "..\..\application" ( goto ERROR_CAN_NOT_DELETE_FOLDER_APPLICATION )
mkdir "..\..\application" 1>nul 2>nul
echo. >"..\..\application\.placeholder"
move /y "chromebinsync.7z" "..\..\application\"

pushd "..\..\application\"
call "..\resources\bin\7za.exe" x "chromebinsync.7z"

::version #1 has this folder name under the 7zip
call "..\resources\bin\mv.exe" -fv "Chrome-bin-sync\*" "."
del /f /q "Chrome-bin-sync"   1>nul 2>nul
rmdir  /q "Chrome-bin-sync"   1>nul 2>nul

::version #2 has this folder name under the 7zip
call "..\resources\bin\mv.exe" -fv "Chrome-bin\*" "."
del /f /q "Chrome-bin"   1>nul 2>nul
rmdir  /q "Chrome-bin"   1>nul 2>nul

del /f /q "chromebinsync.7z"  1>nul 2>nul


goto END


::-------------------------------------------------------------


:ERROR_URL
  echo [ERROR] getting the URL to download failed. 1>&2
  goto END

:ERROR_DOWNLOAD
  echo [ERROR] download failed. 1>&2
  goto END

:ERROR_CAN_NOT_DELETE_FOLDER_APPLICATION
  echo [ERROR] can not delete the application folder. 1>&2
  goto END

:END
  echo [INFO] Done. [EXIT_CODE: %EXIT_CODE%]. 1>&2
  pause
  popd
  popd
  exit /b %EXIT_CODE%


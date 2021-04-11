::@echo off
chcp 65001 1>nul 2>nul

set "EXIT_CODE=0"

pushd "%~sdp0"
set "FILE_INTPUT=new_urls.txt"
set "FILE_JOINED=new_switches.cc"

::::--------------------------------------------- keep folder, download (if newer), overwrite existing.
::del /q /f "new_tmp_switches"
mkdir "new_tmp_switches"
pushd "new_tmp_switches"

call "aria2c.exe" --allow-overwrite=true --auto-file-renaming=false --dir="." --max-concurrent-downloads=16 --max-connection-per-server=16 --split=1 --min-tls-version=TLSv1.2 --check-certificate=false --enable-http-pipelining=true --enable-http-keep-alive=true --remote-time=true --conditional-get=true "--referer=*" --human-readable=true --retry-wait=5 --input-file="../new_urls.txt"
set "EXIT_CODE=%ErrorLevel%"
::--stream-piece-selector=inorder
::--uri-selector=inorder
::--user-agent="Mozilla/5.0 Chrome"

if ["%EXIT_CODE%"] NEQ ["0"] ( goto ERROR_DOWNLOAD )

echo. >%FILE_JOINED%
for %%e in (".\*.*") do ( 
  echo ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ >>%FILE_JOINED%
  echo %%e >>%FILE_JOINED%
  echo ￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣ >>%FILE_JOINED%
  echo. >>%FILE_JOINED%
  type "%%e" >>%SWITCHES% 
  echo ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ >>%FILE_JOINED%
)

del /q /f "..\%FILE_JOINED%.old" 
ren "..\%FILE_JOINED%" "%FILE_JOINED%.old"
move /y ".\%FILE_JOINED%" "..\%FILE_JOINED%"
set "EXIT_CODE=%ErrorLevel%"
if ["%EXIT_CODE%"] NEQ ["0"] ( goto ERROR_MOVE )

echo [INFO] success.

goto END






:ERROR_DOWNLOAD
  echo [ERROR] download error, please try again. 1>&2
  goto END

:ERROR_MOVE
  echo [ERROR] can not move joined-file. 1>&2
  goto END

:END
  echo [INFO] Done.
  echo EXIT_CODE: [%EXIT_CODE%].
  pause
  popd
  popd
  exit /b %EXIT_CODE%

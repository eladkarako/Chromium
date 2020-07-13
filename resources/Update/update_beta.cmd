::@echo off
::---------------------------------[http|https]
set "PROTOCOL=http"
::---------------------------------[snapshots|continuous]
set "BRANCH=snapshots"
::---------------------------------[Android|Arm|Linux|LinuxGit|LinuxGit_x64|Linux_ARM_Cross-Compile|Linux_ChromiumOS|Linux_ChromiumOS_Full|Linux_x64|Mac|MacGit|Win|WinGit|Win_x64|chromium-full-linux-chromeos]
set "OS=Win_x64"
::---------------------------------[mini_installer.exe|REVISIONS|chrome-win32-syms.zip|chrome-win32.zip|pnacl.zip|content-shell.zip|gcapi.zip|metrics-metadata.zip|remoting-host.msi|remoting-me2me-host-win.zip|devtools_frontend.zip|chrome-linux.zip|changelog.xml|chrome-android.zip]
set "FILE=mini_installer.exe"




::========================================================get the text-content of LAST_CHANGE
set COMMAND_CURL_FOR_REVISION=curl.exe --silent --http2 --ipv4 --anyauth --insecure --location-trusted --ssl-allow-beast --ssl-no-revoke --url "%PROTOCOL%://storage.googleapis.com/chromium-browser-%BRANCH%/%OS%/LAST_CHANGE"
set "REVISION="
echo ^[INFO^] getting the content of chromium-browser ^[Branch:%BRANCH%/OS:%OS%^]                 1>&2
for /f "tokens=*" %%a in ('call %COMMAND_CURL_FOR_REVISION% 2^>^&1') do ( set "REVISION=%%a" )
if ["%ErrorLevel%"] EQU [""] ( goto ERROR_REVISION )
if ["%REVISION%"] EQU [""] ( goto ERROR_REVISION )
echo Latest-Revision: ^>%REVISION%^< ^[Branch:%BRANCH%/OS:%OS%^]       1>&2
echo.                                                                  1>&2


::========================================================download get the content of LAST_CHANGE
set "URL_DOWNLOAD=%PROTOCOL%://storage.googleapis.com/chromium-browser-%BRANCH%/%OS%/%REVISION%/%FILE%"
echo ^[INFO^] starting to download using aria2c:                       1>&2
echo %URL_DOWNLOAD%                                                    1>&2
echo.                                                                  1>&2
echo.                                                                  1>&2


del /f /q "mini_installer.exe"                                         1>nul 2>nul
del /f /q "mini_installer.exe.aria2"                                   1>nul 2>nul


call "resources\aria2c.cmd" "%URL_DOWNLOAD%"
call color
::------------------------------------------------------ignore "file-already exists"..
if ["%ErrorLevel%"] EQU ["13"] ( set "ErrorLevel=0" )
if ["%ErrorLevel%"] NEQ ["0"]  ( goto ERROR_DOWNLOAD )
if not exist "mini_installer.exe" ( goto ERROR_DOWNLOAD )
call color
echo.                                                                  1>&2
echo Download finished successfully.                                   1>&2
echo.                                                                  1>&2
echo.                                                                  1>&2


::========================================================extract the Chrome's-version from installation's EXE-VERSIONINFO resource.
echo Extracting Chrome-version-information from 'mini_installer.exe' embedded VERSIONINFO resource.     1>&2
call "resources\ResHacker.exe" "-extract" "mini_installer.exe,versioninfo.rc,VERSIONINFO,,"
if not exist "versioninfo.rc" ( goto ERROR_VERSIONINFO )
for /f "tokens=*" %%a in ('call "resources\grep.exe" --max-count=1 --no-filename --only-matching --perl-regexp "FileVersion.+$" "versioninfo.rc" ^| "resources\grep.exe" --only-matching --perl-regexp "\d+\.\d+\.\d+\.\d+"    2^>^&1') do ( set "VERSIONINFO=%%a" )
del /f /q "versioninfo.rc"      1>nul 2>nul
if ["%VERSIONINFO%"] EQU [""] ( goto ERROR_VERSIONINFO )
echo.                                                                                                   1>&2


::========================================================extract the Chrome's-version from installation's EXE-VERSIONINFO resource.
echo Chrome-Version: ^>%VERSIONINFO%^< ^[Branch:%BRANCH%/OS:%OS%/Revision:%REVISION%^]         1>&2
echo.                                                                                          1>&2


ren "mini_installer.exe" "mini_installer__%VERSIONINFO%.exe"


::========================================================add current chrome-version/revision to README.md
echo %REVISION%    %VERSIONINFO%    %DATE%    %TIME% >>README.md
echo.                                                                                          1>&2

::cleanup
echo DONE.
echo.                                                                                          1>&2


goto END


:ERROR_REVISION
  echo Error getting the revision from:                                                        1>&2
  echo "%PROTOCOL%://storage.googleapis.com/chromium-browser-%BRANCH%/%OS%/LAST_CHANGE"        1>&2
  goto END

:ERROR_DOWNLOAD
  echo Error download from:                                                                    1>&2
  echo %PROTOCOL%://storage.googleapis.com/chromium-browser-%BRANCH%/%OS%/%REVISION%/%FILE%"   1>&2
  goto END

:ERROR_VERSIONINFO
  echo Error getting 'FileVersion' entry from 'versioninfo.rc'                                 1>&2
  goto END

:END
  pause
  color
  exit /b %EXIT_CODE%
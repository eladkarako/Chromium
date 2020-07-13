@echo off
::stuff you can modify.
::--------------------------------------------------------------------------------
::                          http|https
set PROTOCOL=http
::                          snapshots|continuous
set BRANCH=snapshots
::                          Android|Arm|Linux|LinuxGit|LinuxGit_x64|Linux_ARM_Cross-Compile|Linux_ChromiumOS|Linux_ChromiumOS_Full|Linux_x64|Mac|MacGit|Win|WinGit|Win_x64|chromium-full-linux-chromeos
set OS=Win_x64
::                          mini_installer.exe|REVISIONS|chrome-win32-syms.zip|chrome-win32.zip|pnacl.zip|content-shell.zip|gcapi.zip|metrics-metadata.zip|remoting-host.msi|remoting-me2me-host-win.zip|devtools_frontend.zip|chrome-linux.zip|changelog.xml|chrome-android.zip
set FILE=mini_installer.exe
::--------------------------------------------------------------------------------


::stuff you should keep as is.
::--------------------------------------------------------------------------------
set URL_LASTCHANGE=%PROTOCOL%://storage.googleapis.com/chromium-browser-%BRANCH%/%OS%/LAST_CHANGE

set COMMAND_CURL_FORVERSION=curl.exe --silent --http2 --ipv4 --anyauth --insecure --location-trusted --ssl-allow-beast --ssl-no-revoke --url "%URL_LASTCHANGE%"

set "VERSION="
for /f "tokens=*" %%a in ('call %COMMAND_CURL_FORVERSION% 2^>^&1') do (set VERSION=%%a)
::error handling
if ["%VERSION%"] EQU [""] ( goto NOVERSION )
set URL_DOWNLOAD=%PROTOCOL%://storage.googleapis.com/chromium-browser-%BRANCH%/%OS%/%VERSION%/%FILE%

echo.
echo Got Latest-Version: ^>%VERSION%^< ^[Branch:%BRANCH%/OS:%OS%^]
echo Downloading: %URL_DOWNLOAD%
::--------------------------------------------------------------------------------


::you should enable one-of-your-prefered downloaders.
::goto DOWNLOAD_CURL
::goto DOWNLOAD_WGET
::goto DOWNLOAD_ORBITDOWNLOADER
goto DOWNLOAD_ARIA2C



::you should not reach here, unless
::you've forgot to enable one of the "downloader lines" (above)...
goto NODOWNLOADER


::--------------------------------------------------------------------------------
:NOVERSION
  echo ERROR: could not get the latest version...
  goto EXIT

:NODOWNLOADER
  echo ERROR: please enable one of the downloader lines..
  goto EXIT


:DOWNLOAD_CURL
  echo Start Download using cURL...
  call curl.exe --verbose --http2 --ipv4 --ignore-content-length ^
                --anyauth --insecure --location-trusted          ^
                --ssl-allow-beast --ssl-no-revoke --tcp-fastopen ^
                --tcp-nodelay --use-ascii --url "%URL_DOWNLOAD%"
  goto EXIT


:DOWNLOAD_WGET
  echo Start Download using wGET...
  call wget.exe --directory-prefix="." --debug --user-agent="Mozilla/5.0 Chrome" --continue ^
                --server-response --no-check-certificate --secure-protocol=auto  "%URL_DOWNLOAD%"
  goto EXIT


:DOWNLOAD_ARIA2C
  echo Start Download using Aria2C...
  @echo off
  call aria2c.exe --allow-overwrite=false        --auto-file-renaming=false         --check-certificate=false                           ^
                  --check-integrity=false        --connect-timeout=120              --console-log-level=notice                          ^
                  --continue=true                --dir="."                          --disable-ipv6=true                                 ^
                  --enable-http-keep-alive=true  --enable-http-pipelining=true      --file-allocation=prealloc                          ^
                  --http-auth-challenge=false    --human-readable=true              --max-concurrent-downloads=16                       ^
                  --max-connection-per-server=16 --max-tries=3                      --min-split-size=1M                                 ^
                  --retry-wait=2                 --rpc-secure=false                 --split=16                                          ^
                  --timeout=120                  --user-agent="Mozilla/5.0 Chrome"  --out="mini_installer_%BRANCH%_%OS%_%VERSION%.exe"  ^
                  "%URL_DOWNLOAD%"
  @echo on
  goto EXIT


:DOWNLOAD_ORBITDOWNLOADER
  echo Start Download using OrbitDownloader...
  call "C:\Program Files (x86)\Orbitdownloader\orbitdm.exe" "%URL_DOWNLOAD%"
  goto EXIT


:EXIT  
  echo.
  echo Done.
  pause


::--------------------------------------------------------------------------------------------
::--------------------------------------------------------------------------------------------
::
::   - "http"           - http protocol is faster to connect and download with OrbitDownloader and wGet, it also means your PC won't preform certificate exchange with the remote machine.
::   - "snapshots"      - snapshots is newer, "continuous" is more stable (but might be very old).
::   - "OS" and "FILE"  - are what you want to download
::
::
:: snapshots    - newest (unstable) newest code-changes - passed unit-tests + compilation.
:: continuous   - old    (stable)                       - passed unit-tests + compilation + test-suits.
:: ------------------------------------------------------------------------------------------------------
::   branch   |  branch description           |  version-based build
:: ___________|_______________________________|_____________________________________________________________________________________________
::   Win_x64  |  Chromium Installer (64-bit)  |  storage.googleapis.com/chromium-browser-continuous/Win_x64/{version}/mini_installer.exe  
::   Win_x64  |  Chromium Package (64-bit)    |  storage.googleapis.com/chromium-browser-continuous/Win_x64/{version}/chrome-win32.zip    
::   Win      |  Chromium Installer (32-bit)  |  storage.googleapis.com/chromium-browser-continuous/Win/{version}/mini_installer.exe
::   Win      |  Chromium Package (64-bit)    |  storage.googleapis.com/chromium-browser-continuous/Win/{version}/chrome-win32.zip    
::
::
::------------------------------------------------------
::- SCRIPT WAS WRITTEN AND MAINTAINED BY ELAD KARAKO.  -
::- LAST UPDATED: JULY, 2017. FREE TO USE UNDER GNU.   -
::------------------------------------------------------

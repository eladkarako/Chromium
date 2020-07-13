
::---------------------------------------------------
::---------------------------------------------------
set "FOLDER_MOZILLA=.\..\..\firefox"
for /f %%a in ("%FOLDER_MOZILLA%") do ( set "FOLDER_MOZILLA=%%~fsa" )

del   /q /s /f "%FOLDER_MOZILLA%"               1>nul 2>nul
del   /q /s /f "%FOLDER_MOZILLA%\"              1>nul 2>nul
del   /q /s /f /a:rhisal "%FOLDER_MOZILLA%\*"   1>nul 2>nul
rmdir /q /s    "%FOLDER_MOZILLA%"               1>nul 2>nul
rmdir /q /s    "%FOLDER_MOZILLA%\"              1>nul 2>nul


if exist "%FOLDER_MOZILLA%" ( goto ERROR_MOZILLA_FOLDER )
move /y ".\firefox-XXXXXX.en-GB.win64.zip" "%FOLDER_MOZILLA%\..\"
cd "%FOLDER_MOZILLA%\..\"
7z.exe x "firefox-XXXXXX.en-GB.win64.zip"
set "EXIT_CODE=%ErrorLevel%"
if ["%EXIT_CODE%"] NEQ ["0"]                                       ( goto ERROR_EXTRACTING_ARCHIVE     ) 

del /q /s /f "firefox-XXXXXX.en-GB.win64.zip"   1>nul 2>nul

::this is relavent only for esr which is executable-archive with 
del /q /s /f "setup.exe"   1>nul 2>nul
ren "core" "firefox"       1>nul 2>nul

if not exist "firefox" ( goto ERROR_NO_PROGRAM )

echo [INFO] success.                                   1>&2

goto END


:ERROR_NO_GET_MOST_UPDATED_URL
  echo [ERROR] can't find 'get_most_updated_url.cmd'.  1>&2
  goto END


:ERROR_FAILED_TO_GET_MOST_UPDATED_URL
  echo [ERROR] 'get_most_updated_url.cmd' failed.      1>&2
  goto END

:ERROR_FAILED_DOWNLOAD
  echo [ERROR] download error.                         1>&2
  goto END

:ERROR_MOZILLA_FOLDER
  echo [ERROR] can not delete the mozilla folder at    1>&2
  echo %FOLDER_MOZILLA%                                1>&2
  echo manually-extract the zip-file from %~sdp0       1>&2
  echo replacing the old 'firefox' folder.             1>&2
  goto END

:ERROR_EXTRACTING_ARCHIVE
  echo [ERROR] can not extract archive.                1>&2
  goto END

:ERROR_NO_PROGRAM
  echo [ERROR] can not find the program-folder.        1>&2
  goto END

:END
  echo exit code: [%EXIT_CODE%]. 1>&2
  echo.                          1>&2
  pause                          1>&2
  exit /b %EXIT_CODE%


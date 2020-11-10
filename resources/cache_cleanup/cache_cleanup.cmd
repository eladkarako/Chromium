@echo off
::--------------------------------------------------- support showing Unicode in CMD.
chcp 65001 1>nul 2>nul


::--------------------------------------------------- gnuwin32 rm.exe for better delete.
set "FILE_RM=%~sdp0rm.exe"


::--------------------------------------------------- 1. loop list of strings (THIS BATCH FILE MUST BE WINDOWS-EOL (a.k.a 'CR+LF' a.k.a '\r\n') ENCODED!!!)
::                                                    2. calling a local procedure.
::                                                    3. passing one string (representing a path - no checks..) to the procedure.
::                                                    Note 1: "%LocalAppData%" is like "C:\Users\Elad\AppData\Local"
for %%x in ( 
  "%LocalAppData%\Google\Chrome Dev\User Data"
  "..\..\new\profile"
  "..\..\old\profile"
) do ( 
  call :METHOD__GENERIC_CLEAN %%x
)


::--------------------------------------------------- finish (jump-point to skip following code).
goto END



::======================================================================================================
::======================================================================================================



::--------------------------------------------------- a local procedure. ends with standard 'goto :EOF'.
::----------------------------------
:METHOD__GENERIC_CLEAN
  pushd "%~1"
  title %CD%
  echo.
  echo [INFO] Current Folder Changed To: [%CD%]

  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "BrowserMetrics"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Crashpad"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Cache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Code Cache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\databases"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\databases-incognito"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Feature Engagement Tracker"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\GPUCache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\IndexedDB"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Media Cache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Service Worker\CacheStorage"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Service Worker\ScriptCache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\Session Storage"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Default\VideoDecodeStats"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "PnaclTranslationCache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "Safe Browsing"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "GrShaderCache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "ShaderCache"
  call "%FILE_RM%" --verbose --directory --force --no-preserve-root --recursive   "SwReporter"
  del /f /q "BrowserMetrics*"
  del /f /q "Crashpad*"
  del /f /q "Safe Browsing*"


  title Done
  echo [INFO] Done.
  echo.
  popd
  goto :EOF
::----------------------------------



::======================================================================================================
::======================================================================================================


::--------------------------------------------------- a jump-point-target.
:END
  echo.
  echo [INFO] Done (All)
  title Done

  pause
  exit /b 0

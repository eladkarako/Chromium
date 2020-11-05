@echo off
chcp 65001 1>nul 2>nul

pushd "..\..\new\profile"

title %CD%

call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "BrowserMetrics"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Crashpad"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Cache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Code Cache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\databases"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\databases-incognito"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Feature Engagement Tracker"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\GPUCache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\IndexedDB"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Media Cache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Service Worker\CacheStorage"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Service Worker\ScriptCache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\Session Storage"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Default\VideoDecodeStats"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "PnaclTranslationCache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "Safe Browsing"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "GrShaderCache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "ShaderCache"
call "%~sdp0rm.exe" --verbose --directory --force --no-preserve-root --recursive   "SwReporter"
del /f /q "BrowserMetrics*"
del /f /q "Crashpad*"
del /f /q "Safe Browsing*"

pause
popd
exit /b 0

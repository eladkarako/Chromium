@echo off
chcp 65001 1>nul 2>nul
call "..\bin\node.exe" "%~sdp0get_most_updated_url.js"
exit /b %ErrorLevel%

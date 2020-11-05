@echo off
chcp 65001 1>nul 2>nul

rm.exe -dfr   ".\profile"
del /f /q /r  ".\profile"
rm.exe -dfr   ".\profile"

mkdir "profile" 1>nul 2>nul

echo. >"profile\.placeholder"


exit /b 0

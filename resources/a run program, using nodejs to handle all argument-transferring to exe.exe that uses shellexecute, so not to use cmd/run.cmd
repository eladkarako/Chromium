@echo off
chcp 65001 1>nul 2>nul

pushd "%~dp0"

call ".\--_resources\node.exe" ".\--_resources\run.js" ".\--_resources\run.json" %*
popd
exit /b 0

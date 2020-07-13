@echo off

chcp 65001 1>nul 2>nul

echo BE SURE!!! 1>&2
pause
pause
pause

pushd "%~sdp0..\"

"resources\bin\rm.exe -dfr "profile"

mkdir "profile" 1>nul 2>nul

echo. >"profile\.placeholder"

popd

exit /b 0

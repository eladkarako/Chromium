::@echo off

pushd "%~dp0"

del /s /f /q "profile\Crashpad*"
del /s /f /q "profile\BrowserMetrics*"

del /s /f /q "profile\Crashpad\"
del /s /f /q "profile\BrowserMertics\"
del /s /f /q "profile\IndexedDB\"
del /s /f /q "profile\Cache\"
del /s /f /q "profile\Application Cache\"
del /s /f /q "profile\ShaderCache\"
del /s /f /q "profile\Media Cache\"
del /s /f /q "profile\PnaclTranslationCache\"
del /s /f /q "profile\Default\GPUCache\"
del /s /f /q "profile\Default\BrowserMertics\"
del /s /f /q "profile\Default\Cache\"
del /s /f /q "profile\Default\IndexedDB\"
del /s /f /q "profile\Default\Media Cache\"
del /s /f /q "profile\Default\Service Worker\CacheStorage\"
del /s /f /q "profile\Default\Service Worker\ScriptCache\"
del /s /f /q "profile\Default\Storage\ext\chrome-signin\def\GPUCache\"

pause
popd
exit \b 0
<sub>See original article at: <a href="http://icompile.eladkarako.com/aria2c-sample-chromium-command-line-switch-updater/">http://icompile.eladkarako.com/aria2c-sample-chromium-command-line-switch-updater/</a></sub>

<hr/>

Live code shortcut that creates a more copy'able result:<br/>
<code>https://cs.chromium.org/search/?q=_switches*.cc&m=100&det=none&sort=1&type=cs</code>,
copy the text, sort it using <code>sort.eladkarako.com</code>, copy just the "reasonable" lines, replace the start of the line: <code>chromium </code> with <code>https://cs.chromium.org/codesearch/f/chromium/</code> to get access to the raw file, only take those who ends with <code>.cc</code>, by regularexpression-replacing <code>^.+[^c][^c]$</code> matches with empty string, add <code>https://cs.chromium.org/codesearch/f/chromium/src/tools/metrics/histograms/enums.xml</code> to the list, which includes many "classic|experiments|still active|undocumented|quirk" switches, add it all to the already existing list since sometimes the search misses some of the files, and the downloader can skip error downloads anyway...

<hr/>

<img rem-width="500" rem-height="256" alt="" src="resources/icon.png"/>
<h1>Chrome-Command-Line-Switches</h1>
<h2>Get And View The Latest And Most Recent Chrome/Chromium Command-Line-Switches, For You To Control Experimental Browser Additions</h2>


<blockquote>
Getting the most updated command-line switches for Chromium (Google-Chrome base code) is always a work in progress,
since this is a "live code" you can never say "Ok, I'm Done", there will always be a new one, or old one retired from being actively used in the, well.., actual code.
So if you're relaying on command-line switches in your scripts, or just want to try out new features before the "bleed into" the actual Google-Chrome main version, you probably want to bookmark this article :]]
</blockquote>

First of all: <strong>How Do You Even Browse Chromium Code?</strong>
luckily, lately the chromium project has been re-organised so browsing the code and viewing (or downloading) the raw source-files, is a lot easier than few few years ago, 
If you'll browse <a href="https://cs.chromium.org/search/?q=switches*.cc&p=1&m=100&det=none&type=cs" target="_blank">https://cs.chromium.org/search/?q=switches*.cc&p=1&m=100&det=none&type=cs</a> you could get a list of all the files that has the switch keyword in it, as in name, or content,
<img src="resources/icompile.eladkarako.com_chromium_command_line_switches.png" alt="" rem-width="1243" rem-height="477"/>

There is a small issue of filtering out and keeping only the useful ones, and creating some sort of downloading method and easy viewing.

...I've done that already:


<code>switches updater.cmd</code>:

```cmd
@echo off
::cleanup
if exist .\switches\       del    /s /q /f .\switches\        >nul
if exist .\switches\       rmdir  /s /q    .\switches\        >nul
if exist switches_old.cc   del /f /q switches_old.cc          >nul
if exist switches.cc       ren  switches.cc switches_old.cc   >nul

::download using wGet
::call .\resources\wget.exe         ^
::--input-file="switches_urls.txt"  --user-agent="Mozilla/5" --no-check-certificate            ^
::--secure-protocol="auto"          --auth-no-challenge      --no-directories                  ^
::--directory-prefix=.              --output-document="switches.cc"


::download using aria2c
.\resources\aria2c.exe  --allow-overwrite=true          --auto-file-renaming=false          --check-certificate=false          ^
                        --check-integrity=false         --console-log-level=notice          --continue=true                    ^
                        --dir="./switches/"             --disable-ipv6=true                 --enable-http-keep-alive=true      ^
                        --enable-http-pipelining=true   --file-allocation=prealloc          --http-auth-challenge=false        ^
                        --human-readable=true           --max-concurrent-downloads=10       --max-connection-per-server=16     ^
                        --min-split-size=1M             --rpc-secure=false                  --user-agent="Mozilla/5.0 Chrome"  ^
                        --split=3                       --timeout=120                       --connect-timeout=120              ^
                        --max-tries=3                   --referer="http://eladkarako.com/"  --force-sequential=true            ^
                        --input-file="switches_urls.txt"   
                      ::--console-log-level=notice      --log-level=info                    --log=log.txt

echo. >switches.cc
for %%e in (.\switches\*.*) do (
  type "%%e" >>switches.cc
)
if exist .\switches\       del    /s /q /f .\switches\        >nul
if exist .\switches\       rmdir  /s /q    .\switches\        >nul


::open for viewing (optional)
start /low /max "cmd /c "call .\resources\Notepad++.exe "switches.cc"""

pause
pause
```

<hr/>

And the list of file-names:
<code>switches_urls.txt</code>:
<pre>
https://cs.chromium.org/codesearch/f/chromium/infra/go/src/go.chromium.org/luci/machine-db/appengine/model/switches.go
https://cs.chromium.org/codesearch/f/chromium/infra/go/src/go.chromium.org/luci/machine-db/appengine/rpc/switches_test.go
https://cs.chromium.org/codesearch/f/chromium/src/android_webview/common/aw_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/android_webview/common/aw_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/android_webview/java/src/org/chromium/android_webview/AwSwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/android_webview/test/shell/src/org/chromium/android_webview/shell/AwShellSwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/apps/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/apps/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ash/ash_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/common/ash_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/metrics/task_switch_metrics_recorder.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/metrics/task_switch_metrics_recorder.h
https://cs.chromium.org/codesearch/f/chromium/src/ash/metrics/task_switch_metrics_recorder_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/metrics/task_switch_source.h
https://cs.chromium.org/codesearch/f/chromium/src/ash/metrics/task_switch_time_tracker.h
https://cs.chromium.org/codesearch/f/chromium/src/ash/metrics/task_switch_time_tracker_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/public/cpp/app_list/app_list_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/public/cpp/ash_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ash/public/cpp/ash_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/base/android/java/src/org/chromium/base/BaseSwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/base/base_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/base/base_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/base/i18n/base_i18n_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/base/i18n/base_i18n_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/base/task_scheduler/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/base/test/test_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/base/test/test_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/cc/base/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/cc/base/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chrome/app/chrome_exe_main_win.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/browser/lifetime/switch_utils.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/browser/lifetime/switch_utils.h
https://cs.chromium.org/codesearch/f/chromium/src/chrome/browser/lifetime/switch_utils_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/browser/supervised_user/experimental/supervised_user_filtering_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/browser/supervised_user/experimental/supervised_user_filtering_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chrome/chrome_cleaner/constants/chrome_cleaner_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/common/chrome_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/common/chrome_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chrome/common/switch_utils.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/common/switch_utils_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/test/base/test_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chrome/test/base/test_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chromecast/base/chromecast_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chromecast/base/chromecast_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chromecast/base/java/src/org/chromium/chromecast/base/CastSwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/chromecast/net/net_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chromecast/net/net_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chromeos/chromeos_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/chromeos/chromeos_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/chromeos/components/proximity_auth/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/autofill/core/common/autofill_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/autofill/core/common/autofill_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/autofill/ios/browser/autofill_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/autofill/ios/browser/autofill_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/browser_sync/browser_sync_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/browser_sync/browser_sync_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/cloud_devices/common/cloud_devices_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/cloud_devices/common/cloud_devices_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/component_updater/component_updater_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/component_updater/component_updater_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/crash/content/app/crash_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/crash/content/app/crash_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/crash/content/app/crashpad_win.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/cryptauth/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/cryptauth/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/data_reduction_proxy/core/common/data_reduction_proxy_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/data_reduction_proxy/core/common/data_reduction_proxy_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/dom_distiller/core/dom_distiller_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/dom_distiller/core/dom_distiller_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/error_page/common/error_page_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/error_page/common/error_page_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/feedback/feedback_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/feedback/feedback_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/flags_ui/flags_ui_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/flags_ui/flags_ui_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/google/core/browser/google_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/google/core/browser/google_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/infobars/core/infobars_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/invalidation/impl/invalidation_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/invalidation/impl/invalidation_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/metrics/metrics_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/metrics/metrics_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/nacl/common/nacl_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/nacl/common/nacl_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/network_session_configurator/common/network_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/network_session_configurator/common/network_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/ntp_snippets/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/ntp_snippets/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/ntp_tiles/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/ntp_tiles/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/omnibox/browser/omnibox_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/omnibox/browser/omnibox_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/os_crypt/os_crypt_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/os_crypt/os_crypt_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/policy/core/common/policy_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/policy/core/common/policy_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/precache/core/precache_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/previews/core/previews_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/previews/core/previews_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/proximity_auth/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/proximity_auth/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/reading_list/core/reading_list_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/reading_list/features/reading_list_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/reading_list/features/reading_list_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/safe_browsing/common/safebrowsing_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/safe_browsing/common/safebrowsing_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/search_engines/search_engines_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/search_engines/search_engines_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/search_provider_logos/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/search_provider_logos/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/security_state/core/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/services/heap_profiling/public/cpp/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/browser/profile_management_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/browser/profile_management_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/browser/profile_management_switches_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/browser/signin_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/browser/signin_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/common/profile_management_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/signin/core/common/signin_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/spellcheck/common/spellcheck_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/spellcheck/common/spellcheck_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/ssl_config/ssl_config_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/ssl_config/ssl_config_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/sync/driver/sync_driver_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/sync/driver/sync_driver_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/sync/engine/sync_engine_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/tracing/common/tracing_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/tracing/common/tracing_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/translate/core/common/translate_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/translate/core/common/translate_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/ui_devtools/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/ui_devtools/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/variations/variations_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/variations/variations_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/components/viz/common/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/components/viz/common/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/content/common/content_switches_internal.cc
https://cs.chromium.org/codesearch/f/chromium/src/content/common/content_switches_internal.h
https://cs.chromium.org/codesearch/f/chromium/src/content/common/content_switches_internal_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/content/public/android/java/src/org/chromium/content/common/ContentSwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/content/public/common/content_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/content/public/common/content_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/content/public/common/mojo_channel_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/content/public/common/mojo_channel_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/content/shell/common/layout_test/layout_test_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/content/shell/common/layout_test/layout_test_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/content/shell/common/shell_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/content/shell/common/shell_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/extensions/common/feature_switch.cc
https://cs.chromium.org/codesearch/f/chromium/src/extensions/common/feature_switch.h
https://cs.chromium.org/codesearch/f/chromium/src/extensions/common/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/extensions/common/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/extensions/shell/common/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/extensions/shell/common/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/google_apis/drive/drive_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/google_apis/drive/drive_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/google_apis/gaia/gaia_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/google_apis/gaia/gaia_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/google_apis/gcm/engine/gservices_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/google_apis/gcm/engine/gservices_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/gpu/command_buffer/client/gpu_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/gpu/command_buffer/client/gpu_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/gpu/command_buffer/service/gpu_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/gpu/command_buffer/service/gpu_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/gpu/config/gpu_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/gpu/config/gpu_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/gpu/config/gpu_switching.cc
https://cs.chromium.org/codesearch/f/chromium/src/gpu/ipc/host/gpu_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/gpu/ipc/host/gpu_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/gpu/ipc/service/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/gpu/ipc/service/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/headless/app/headless_shell_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/headless/app/headless_shell_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ios/chrome/browser/chrome_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ios/chrome/browser/chrome_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ios/chrome/browser/ui/contextual_search/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ios/chrome/browser/ui/contextual_search/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ios/chrome/browser/ui/settings/cells/sync_switch_item.h
https://cs.chromium.org/codesearch/f/chromium/src/ios/chrome/browser/ui/tab_switcher/tab_switcher_controller_egtest.mm
https://cs.chromium.org/codesearch/f/chromium/src/media/base/android/java/src/org/chromium/media/MediaSwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/media/base/media_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/media/base/media_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/media/midi/midi_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/media/midi/midi_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/net/quic/test_tools/simulator/switch.h
https://cs.chromium.org/codesearch/f/chromium/src/ppapi/shared_impl/ppapi_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ppapi/shared_impl/ppapi_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/remoting/host/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/remoting/host/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/network/public/cpp/network_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/network/public/cpp/network_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/resource_coordinator/memory_instrumentation/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/resource_coordinator/memory_instrumentation/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/embedder/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/embedder/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/public/cpp/standalone_service/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/runner/common/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/runner/common/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/sandbox/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/sandbox/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/service_manager/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/services/tracing/public/cpp/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/ui/common/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/services/ui/common/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/third_party/cros_system_api/switches/chrome_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/third_party/libjpeg_turbo/rdswitch.c
https://cs.chromium.org/codesearch/f/chromium/src/third_party/swiftshader/third_party/LLVM/test/CodeGen/SystemZ/09-Switches.ll
https://cs.chromium.org/codesearch/f/chromium/src/tools/gn/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/tools/gn/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/tools/metrics/histograms/enums.xml
https://cs.chromium.org/codesearch/f/chromium/src/tools/perf/benchmarks/tab_switching.py
https://cs.chromium.org/codesearch/f/chromium/src/ui/android/java/src/org/chromium/ui/display/DisplaySwitches.java
https://cs.chromium.org/codesearch/f/chromium/src/ui/app_list/app_list_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/app_list/app_list_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/base/ui_base_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/base/ui_base_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/base/ui_base_switches_util.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/base/ui_base_switches_util.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/compositor/compositor_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/compositor/compositor_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/display/display_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/display/display_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/display/manager/display_configurator_unittest.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/events/event_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/events/event_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/gfx/color_space_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/gfx/switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/gfx/switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/gfx/switches_export.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/gfx/x/x11_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/gfx/x/x11_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/gl/gl_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/gl/gl_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/gl/gl_switches_util.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/gl/gl_switches_util.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/gl/gpu_switching_observer.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/keyboard/keyboard_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/keyboard/keyboard_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/message_center/message_center_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/message_center/public/cpp/message_center_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/native_theme/native_theme_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/ozone/public/ozone_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/ozone/public/ozone_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/views/views_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/views/views_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/ui/wm/core/wm_core_switches.cc
https://cs.chromium.org/codesearch/f/chromium/src/ui/wm/core/wm_core_switches.h
https://cs.chromium.org/codesearch/f/chromium/src/v8/test/mjsunit/switch.js
</pre>

at the end you'll see something like:
<img src="resources/icompile.eladkarako.com_aria2c_download_result_script.png" alt="" rem-width="709" rem-height="487"/>
which means that the script has ended, and you'll be shown the combined result as a one file, opened by notepad++.

What you'll get eventually is nothing fancy but a bit of <code>C</code> source-code and some comments,
but it will gather all of the most relevant and updated information for you in one large file,
that will allow you to quickly search of any reference.

<img src="resources/icompile.eladkarako.com_chromium_command_line_switches_selected_part.png" alt="" rem-width="709" rem-height="293"/>


note that the resources folder has a portable version of the binaries for aria2c and notepad++,
you can also get them at: <code><a href="https://github.com/aria2/aria2/tags" target="_blank">aria2c</a></code> and <code><a href="https://notepad-plus-plus.org/download/" target="_blank">Notepad++</a></code>.

<hr/>

Ps. it is useful to include <code>https://cs.chromium.org/codesearch/f/chromium/src/tools/metrics/histograms/enums.xml</code>
it includes a lot of "rare"/"undocumented"/"old but still may work a.k.a legacy switches" stuff, you might find useful.


Ps #2.
You might notice I'm having <code>.gitattributes</code> with <code>urls.txt    eol=crlf</code>,
this since I work with batch file, under Windows, 
so I've figured it is best to be sure after the commit and checkout 
(if I may in-fact do it "from fresh") - I won't break the batch-script, 
since GitHub usually prefer the <code>\n</code> line ending, while the batch file will (probably) work best 
with <code>\r\n</code> (Windows style..) .

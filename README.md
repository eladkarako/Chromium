Chromium, local folder.
need to: make some of the paths relative.

resources/updater/updater.cmd will download, clear-up the application folder, then place new build for windows64 there.
resources/_clear_profile.cmd will clean up the content of the profile folder.

chromium.exe and chromium.ini (used to be runC.exe and runC.ini are shell/shellexecute front-end wrap that avoid batch files CMD /c or the use of start command).

<hr/>

I'm using those flags (might be changed later...)
```ini
--flag-switches-begin "--user-data-dir=D:\Software\Chromium\profile" --no-default-browser-check --ignore-autocomplete-off-autofill --allow-outdated-plugins --disable-logging --disable-breakpad --disable-crash-reporter --no-pings --force-device-scale-factor="1.3" --ignore-certificate-errors --force-color-profile="srgb" --enable-fast-unload --keep-alive-for-test --enable-threaded-compositing --disable-features="OmniboxUIExperimentHideSteadyStateUrlSchemeAndSubdomains,OmniboxUIExperimentHideSteadyStateUrlTrivialSubdomains,OmniboxUIExperimentHideSteadyStateUrlPathQueryAndRef,OmniboxUIExperimentHideSteadyStateUrlScheme" --flag-switches-end
```

really the only one needed is: <code>"--user-data-dir=D:\Software\Chromium\profile"</code>,  
and since the argument is not passed throught the CMD pipe (it used to be through a batch file),  
it is better to wrap just the path with <code>""</code>, but nevermind...  

some of the argument can no longer be found in the <code>chrome://flags</code> or <code>switches.cc</code> (see in <code>resources/Chrome-Command-Line-Switches</code>) but are still effective.  
<code>--ignore-certificate-errors</code> can help you view websites with SSL3 or Symantec-signed certificates (which are no longer trusted). This is useful if you have an old computer that is not capable of TLS1.2 (like Windows XP),  
or if you haven't installed the TLS fix for Windows 7 yet... <a href="https://support.microsoft.com/he-il/help/3140245/update-to-enable-tls-1-1-and-tls-1-2-as-default-secure-protocols-in-wi">https://support.microsoft.com/he-il/help/3140245/update-to-enable-tls-1-1-and-tls-1-2-as-default-secure-protocols-in-wi</a>

<hr/>

my settings in <code>chrome://flags</code> will add those (<code>chrome://version</code> will show it below the other arguments)
```ini
--flag-switches-begin --autoplay-policy=document-user-activation-required --default-tile-height=128 --default-tile-width=128 --disable-accelerated-2d-canvas --disable-accelerated-video-decode --disable-cast-streaming-hw-encoding --no-pings --disable-software-rasterizer --blink-settings=disallowFetchForDocWrittenScriptsInMainFrame=false --enable-checker-imaging --disable-offer-upload-credit-cards --enable-devtools-experiments --enable-experimental-web-platform-features --enable-fast-unload --disable-gpu-rasterization --javascript-harmony --enable-nacl --disable-offline-auto-reload --enable-password-generation --enable-quic --enable-scroll-prediction --use-simple-cache-backend=on --enable-suggestions-with-substring-match --enable-tab-audio-muting --disable-zero-copy --enable-experimental-extension-apis --force-color-profile=srgb --force-ui-direction=ltr --gpu-rasterization-msaa-sample-count=0 --load-media-router-component-extension=0 --num-raster-threads=4 --omnibox-tab-switch-suggestions=disabled --overscroll-history-navigation=0 --passive-listeners-default=forcealltrue --pull-to-refresh=0 --save-previous-document-resources=onload --show-saved-copy=disable --enable-smooth-scrolling --tls13-variant=disabled --top-chrome-md=material --ui-disable-partial-swap --v8-cache-options=code --enable-features=AffiliationBasedMatching,ArrayPrototypeValues,AutofillShowTypePredictions,Av1Decoder,BackgroundVideoTrackOptimization,ExpensiveBackgroundTimerThrottling,FontCacheScaling,IncreaseInputAudioBufferSize,LeftToRightUrls,MarkHttpAs,MemoryAblation,ModuleScriptsDynamicImport,ModuleScriptsImportMetaUrl,NewPrintPreview,NoStatePrefetch,OmniboxDisplayTitleForCurrentUrl,OmniboxUIExperimentMaxAutocompleteMatches,OmniboxUIExperimentNarrowDropdown,OmniboxUIExperimentShowSuggestionFavicons,OverflowIconsForMediaControls,PWAFullCodeCache,ParallelDownloading,PassiveDocumentEventListeners,PassiveEventListenersDueToFling,ScrollAnchorSerialization,ScrollAnchoring,ServiceWorkerScriptFullCodeCache,ServiceWorkerServicification,SharedArrayBuffer,SimplifyHttpsIndicator,SoundContentSetting,SpeculativePreconnect,UseModernMediaControls,V8VmFuture,WebRTC-H264WithOpenH264FFmpeg,brotli-encoding,enable-manual-password-generation --disable-features=OmniboxUIExperimentHideSteadyStateUrlSchemeAndSubdomains,OmniboxUIExperimentHideSteadyStateUrlTrivialSubdomains,OmniboxUIExperimentHideSteadyStateUrlPathQueryAndRef,OmniboxUIExperimentHideSteadyStateUrlScheme,AppBanners,AutofillCreditCardAblationExperiment,AutofillCreditCardBankNameDisplay,AutofillUpstreamSendDetectedValues,AutofillUpstreamSendPanFirstSix,AutofillUpstreamUpdatePromptExplanation,AutomaticTabDiscarding,CastAllowAllIPs,ClipboardContentSetting,D3DVsync,DesktopPWAWindowing,ExperimentalAppBanners,GdiTextPrinting,GenericSensor,GenericSensorExtraClasses,ImageCaptureAPI,MaterialDesignBookmarks,MaterialDesignExtensions,MidiManagerWinrt,NetworkService,NetworkServiceInProcess,NoScriptPreviews,OmniboxSpeculativeServiceWorkerStartOnQueryInput,OpenVR,OptimizationHints,SafeSearchUrlReporting,SecondaryUiMd,ServiceWorkerPaymentApps,VoiceSearchOnLocalNtp,WebPayments,WebPaymentsModifiers,WebPaymentsSingleAppUiSkip,WebXR,WebXRGamepadSupport,WebXROrientationSensorDevice,Windows10CustomTitlebar,enable-pixel-canvas-recording,token-binding --flag-switches-end
```

<hr/?

I'm not using the following switch but it can be useful (might add later) to make websites think you have a newer browser (unblock some websites..).This will override old Chromium default user-agent:  
by default: <code>"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36"</code>, to a Chrome one.

```ini
--user-agent="Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.16 Safari/537.36"
```
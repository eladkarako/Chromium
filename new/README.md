I'm using  

the SSE3 is faster than the AVX, and AVX2 does not work on my Intel i7 / Lenovo x220 For some reason.

<ol reversed style="list-style-type:decimal;">
<li>
<br/>going back to:
<br/>https://github.com/RobRich999/Chromium_Clang/releases/tag/v91.0.4437.0-r859720-win64-avx 
<br/>because macchrome_chromium_v90.0.4399.0-r846566-win64_sync_all_codecs.7z has a problem with spellcheck, 
<br/>it crashes the page. (it might be because a command-line switch I've added it may not handle:
<br/>--enable-features="IdleTimeSpellChecking" --blink-settings="IdleTimeSpellChecking=enabled" --enable-blink-features=IdleTimeSpellChecking but I'm not 100% sure since I remember it was buggy with spellcheck before as well..
</li>
<li>
trying again macchrome_chromium_v90.0.4399.0-r846566-win64_sync_all_codecs.7z 
because of "all the codecs" thing..., but it is buggy and sometimes spellcheck crash the app.
</li>
<li>
<br/>updating to:
<br/>https://github.com/RobRich999/Chromium_Clang/releases/tag/v91.0.4437.0-r859720-win64-avx
</li>
<li>
<br/>going back to: 
<br/>https://github.com/RobRich999/Chromium_Clang/releases/download/v90.0.4411.0-r851496-win64-avx/chrome.zip
</li>
<li>
<br/><del>trying original "ungoogled" build (that has some additional stuff, flags and switches) from</del>
<br/><del>https://ungoogled-software.github.io/ungoogled-chromium-binaries/releases/windows/64bit/88.0.4324.182-1</del>
<br/><del>( https://github.com/tangalbert919/ungoogled-chromium-binaries/releases/download/88.0.4324.182-1/ungoogled-chromium_88.0.4324.182-1.1_windows-x64.zip )</del>
<br/>=-=-=-= its good by sync can not be turned ON, so no password-sync with google, no history-sync with google, and no bookmarks sync with google, ...
</li>
<li>
<br/>testing: https://github.com/tangalbert919/ungoogled-chromium-binaries/releases
<br/>it is "ungoogled" patched version of Chromium
<br/>https://ungoogled-software.github.io/ungoogled-chromium-binaries/releases/windows/64bit/
<br/>
<br/>the entire-project (for various architectures): https://github.com/Eloston/ungoogled-chromium
<br/>APKs for Android can be found here: https://github.com/bromite/chromium/releases
</li>
<li>
I'm switching between SSE3-optimized/AVX variations.
<br/>https://github.com/RobRich999/Chromium_Clang/releases/download/v90.0.4411.0-r851496-win64-sse3/chrome.zip
<br/>https://github.com/RobRich999/Chromium_Clang/releases/download/v90.0.4411.0-r851496-win64-avx/chrome.zip
<br/><del>https://github.com/RobRich999/Chromium_Clang/releases/download/v90.0.4417.0-r853576-win64-avx2/chrome.zip</del> (not supported on my CPU for some reason).
</li>

<li>
<br/>I've switched to RobRich999 builds, they are marked as "nosync" but you can very easily enable the sync feature again by one-time switching the sync switch in the browser profile/account settings. it is just by default OFF.
<br/>there are SSE3 optimized builds and AVX optimized builds, I'll use the newer AVX (should save a lot of <code>mov</code> instructions in the code).
<br/><del>https://github.com/RobRich999/Chromium_Clang/releases/tag/v90.0.4404.0-r848840-win64-avx</del>
</li>

<li>
<br/><del>https://github.com/macchrome/chromium/releases/download/v89.0.4342.0-r831777-Win64/Chrome-bin-sync.7z</del>
</li>

<li>
<br/>I've switched to macchrome builds it works better than Hibbiki
<br/><del>https://github.com/macchrome/chromium/releases/download/v88.0.4284.0-r813632-Win64/Chrome-bin-sync.7z</del>
</li>


<li>
<br/><del>https://github.com/Hibbiki/chromium-win64/releases/download/v86.0.4240.198-r800218/chrome.sync.7z</del>
</li>

<li>
<br/><del>https://github.com/Hibbiki/chromium-win64/releases/download/v87.0.4280.88-r812852/chrome.sync.7z</del>
</li>
</ol>


<hr/>
<br/>
<br/>

For more details, see https://chromium.woolyss.com/  
<pre>
all-codecs
Chromium compiled with open-source audio/video codecs + H.264 + AAC
• Info: #html5-audio-video
• Test: https://tools.woolyss.com/html5-audio-video-tester/

all-codecs+
Chromium compiled with open-source audio/video codecs + H.264 + H.265 + AAC + MPEG-4 + AMR
• Info: #html5-audio-video
• Test: https://tools.woolyss.com/html5-audio-video-tester/
</pre>

<hr/>

or - better - information in each release page:  
<del>https://github.com/macchrome/chromium/releases/tag/v88.0.4284.0-r813632-Win64</del>  
https://github.com/macchrome/chromium/releases/tag/v89.0.4342.0-r831777-Win64  
<del>https://github.com/Hibbiki/chromium-win64/releases/tag/v86.0.4240.198-r800218</del>  
https://github.com/Hibbiki/chromium-win64/releases/tag/v87.0.4280.88-r812852  

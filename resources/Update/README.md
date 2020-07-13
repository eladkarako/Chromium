<h1>Update</h1>

queries latest Chromium-installer and download it.

optionally set:
━ architecture (default: <code>Win_x64</code>).
━ chromium-branch (default: <code>snapshots</code>).
━ downloader cURL, wGet or Aria2C (default: <code>Aria2C</code>).
━ download protocol (default: <code>http</code>).

by slightly modifing the <code>update.cmd</code> file.

<hr/>

https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Win_x64/
(requires JavaScript) will allow you to browse through all the successfull build of the SNAPSHOT (latest) tree.

<hr/>

https://download-chromium.appspot.com/ 
but the https://download-chromium.appspot.com/dl/Win_x64?type=snapshots
the redirect of 'Win_x64?type=snapshots' doesn't work well, it will redirect to the Win32 version zip.
so you better not use it.

<hr/>

the last version that worked in windows XP is v49.0.2623.112-r403382-win32

<hr/>

Don't install any of the builds from https://github.com/henrypp/chromium/releases
nor https://chromium.woolyss.com/

<hr/>

<pre>
::--------------------------------------------------------------------------------------------
::--------------------------------------------------------------------------------------------
::
::   - "http"           - http protocol is faster to connect and download with OrbitDownloader and wGet, it also means your PC won't preform certificate exchange with the remote machine.
::   - "snapshots"      - snapshots is newer, "continuous" is more stable (but might be very old).
::   - "OS" and "FILE"  - are what you want to download
::
::
:: snapshots    - newest (unstable) newest code-changes - passed unit-tests + compilation.
:: continuous   - old    (stable)                       - passed unit-tests + compilation + test-suits.
:: ------------------------------------------------------------------------------------------------------
::   branch   |  branch description           |  version-based build
:: ___________|_______________________________|_____________________________________________________________________________________________
::   Win_x64  |  Chromium Installer (64-bit)  |  storage.googleapis.com/chromium-browser-continuous/Win_x64/{version}/mini_installer.exe  
::   Win_x64  |  Chromium Package (64-bit)    |  storage.googleapis.com/chromium-browser-continuous/Win_x64/{version}/chrome-win32.zip    
::   Win      |  Chromium Installer (32-bit)  |  storage.googleapis.com/chromium-browser-continuous/Win/{version}/mini_installer.exe
::   Win      |  Chromium Package (64-bit)    |  storage.googleapis.com/chromium-browser-continuous/Win/{version}/chrome-win32.zip    
::
::
::------------------------------------------------------
::- SCRIPT WAS WRITTEN AND MAINTAINED BY ELAD KARAKO.  -
::- LAST UPDATED: JULY, 2017. FREE TO USE UNDER GNU.   -
::------------------------------------------------------
</pre>

<br/>

<hr/>

The following part is written by the <code>update.cmd</code> script, 
the script simply writes-down an additional-line of the revision (it gets the latest one each time it runs), 
the chrome-version (the script parses it from the installer exe-file) and the date-time.
it might-come useful in the future...



549998    67.0.3395.0   2018-04-12 05:43      chrome://extensions && chrome://bookmarks can be both reverted to classic mode.
551557    68.0.3399.0   2018-04-18 05:44      -""-
552113    68.0.3401.0   2018-04-19 22:48      -""-
552473    68.0.3402.0   2018-04-21 00:41      -""-
552492    68.0.3402.0   2018-04-21 01:44      -""-
552521    68.0.3402.0   2018-04-21 02:59      -""-
552533    68.0.3402.0   2018-04-21 04:03      -""-    !!!! this is the last version (https://commondatastorage.googleapis.com/chromium-browser-snapshots/index.html?prefix=Win_x64/) that supports reverting-to-old-extension, 552537 has a bug...
552537    68.0.3402.0   2018-04-21 04:34      can only revert to old bookmarks, but CAN NOT revert to old extensions! which will kept as material-design.
552545    68.0.3402.0   2018-04-21 05:07      -""-
552551    68.0.3402.0   2018-04-21 05:38      -""-
552569    68.0.3403.0   2018-04-21 06:43      -""-
552577    68.0.3403.0   2018-04-21 08:45      -""-
552651    68.0.3405.0   2018-04-23 11:45      -""-
552742    68.0.3405.0   2018-04-23 21:02      -""-
553281    68.0.3406.0   2018-04-25 00:44      -""-
553875    68.0.3409.0   2018-04-26 05:33      -""-
554713    68.0.3416.0   2018-04-30 12:49      both chrome://extensions && chrome://bookmarks still forced as material-design (even if you use chrome://flags or command-line switches). option to set the bookmarks back in most cases isn't even found in the flags (command-line won't work either..).
555982    68.0.3420.0   2018-05-04 07:58      -""-
557750    68.0.3427.0   2018-05-11 04:45      -""-
559890    68.0.3434.0   2018-18-05 11:22      -""-
559937    68.0.3435.0   2018-18-05 21:17      -""-

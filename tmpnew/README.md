I've downloaded and older version of Chromium  
<code>https://github.com/macchrome/chromium/releases/download/v88.0.4284.0-r813632-Win64/Chrome-bin-sync.7z</code>  
after extracting it to a folder named <code>application</code>,  
I've created an empty-folder in the same level named <code>profile</code>
which had some flags in <code>chrome://flags</code> that were removed from the newer versions,  
using a temp. profile folder, I've changed those and restarted the application.  
I've then switched to <code>chrome://version</code> to see the effected command-line switches variation of the flags.

/--|
   |--\application
   |  |  chrome.exe
   |
   |--\profile
   |

opening CMD at the root of those two folders, my command-line runs:  
"application\chrome.exe" --flag-switches-begin "--user-data-dir=%CD%\profile" --flag-switches-end

now heading over to <code>chrome://flags</code>,  
and looking for "group", for example (I'm trying to disable all related features to tab-group),  
I've got some, I've set them to disabled, and click the restart button which shows when you've change anything of the existing values.  
once the browser started again, I've browsed <code>chrome://version</code>,  
and for each flag I've got an entry as a command-line:  
<code>chrome://flags/#tab-groups</code> - <code>--disable-features=TabGroups</code>.  
<code>chrome://flags/#tab-groups-auto-create</code> - <code>--disable-features=TabGroupsAutoCreate</code>.  
<code>chrome://flags/#tab-groups-collapse</code> - <code>--disable-features=TabGroupsCollapse</code>.  
<code>chrome://flags/#tab-groups-collapse-freezing</code> - <code>--disable-features=TabGroupsCollapseFreezing</code>.  

disabling all those can be easily done with combining them into a comma-sep. list

<code>--disable-features=TabGroups,TabGroupsAutoCreate,TabGroupsCollapse,TabGroupsCollapseFreezing</code>  

I've then closed the app, and cleaned the content of the profile folder.  

I can now use the <code>--disable-features=TabGroups,TabGroupsAutoCreate,TabGroupsCollapse,TabGroupsCollapseFreezing</code> command argument in newer-versions of Chromium, which no longer has those flags in the <code>chrome://flags</code>,  
but it will still work!


you can do this trick using various older versions of Chromium you can download from <code>https://github.com/macchrome/chromium/releases</code> and the temp. profile.  


<h1>Chromium Runner With Switches</h1>

Is usage example (which I've actually use A LOT!)
for another repository placed here:<a href="https://github.com/eladkarako/iniRun">github.com/eladkarako/iniRun</a>.

The command-line switches of Google Chrome/Chromium are very complicated,
some require having inverted-commas, the amount of characters making standard-shortcuts quite useless,
but this nice little application passes the arguments required, without any of that <em>"escaping" $h!t...</em>

I've made this repository mainly because I've deleted my previous set of switches..
so this is just a backup...
...and for sharing with everyone I guess... :]

<hr/>

the <code>chrome.exe</code> gets a manifest (side by side and as a compiled-resource) which states to start it as admin. runC (based on iniRun) is too, the best way is to provide with running defaults using the compatibility menu for the file for every user. the manifest enhances the rendering/accurate DPI using GDI+ plus other stuff...

<hr/>

Notes:

#1
--enable-rgba-4444-textures will use 256-colors (8 bit) which is faster but not look great.
--disable-reading-from-canvas will secure browser more but will break uBlock-origin, and setting UI'icon.
--disable-directwrite-for-ui can help to solve squares ("NOTO") on ui from native fonts.

#2
short path (DOS 8.3)/regular path makes no difference since it is all done through API,
   nothing is passed through SHELL/CMD etc..

#3
make sure the EOL is CR+LF (\r\n) in the ini-file.

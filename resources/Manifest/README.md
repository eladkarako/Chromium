<h1>Manifest</h1>

Example of a manifest file to use with chrome.exe,
that will not break the libs chrome is using.

if used as external manifest, it requires a "registry hack" to favor the external-manifest 
instead of the embedded one (chrome.exe has an embedded manifest),
if used as an embedded-manifest is should be embedded properly: using <a href="https://github.com/eladkarako/manifest">github.com/eladkarako/manifest</a>,
(replace the default.manifest with this one),
the version keeps on changing obviously, so when updating, pre-query the exe for the new version, also, make sure to keep the Windows-EOL (CR+LF or \r\n) in the file.

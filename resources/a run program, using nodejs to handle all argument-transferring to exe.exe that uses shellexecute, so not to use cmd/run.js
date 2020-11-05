"use strict";

var process  = require("process")
   ,resolve  = function(path){path=path.replace(/\"+/g,"").replace(/\\+/g,"/"); path=require("path").resolve(path).replace(/\\+/g,"/").replace(/\/\/+/g,"/").replace(/\/+$/g,""); return path;} //normalize to Unix-slash (will work on Windows too).
   ,settings
   ,process
   ;
     
settings = resolve(process.argv[2]);
settings = require("fs").readFileSync(settings,{"encoding":"utf8"});
settings = JSON.parse(settings);


//fixes to make paths explicitly.
settings.runner            = resolve(settings.runner);
settings.execute           = resolve(settings.execute);
settings.working_directory = resolve(settings.working_directory);
settings.profile_directory = resolve(settings.profile_directory);

settings.arguments = [].concat(settings.arguments, process.argv.slice(3));  //join (optional) command-line addition to the main exe.
settings.arguments = settings.arguments.map(function(s){return s.replace(/^(.*)###PROFILE_DIRECTORY###(.*)$/i,"$1" + settings.profile_directory + "$2");}); //provide accurate profile-path instead of pre-written '###PROFILE_DIRECTORY###' saved place.

process.chdir(settings.working_directory);
process = require("child_process").spawn(settings.execute, settings.arguments, {"detached":true, "stdio":"ignore", "shell":false, "timeout":undefined, "windowsHide":false});
process.unref();


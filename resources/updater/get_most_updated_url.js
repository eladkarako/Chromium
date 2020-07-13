"use strict";

const  FS           = require("fs")
      ,PATH         = require("path")
      ,URL          = require("url")
      ,HEADERS      = {"DNT":             "1"
                      ,"Accept":          "*/*"
                      ,"Referer":         "https://www.google.com/"
                      ,"Connection":      "Close"
                      ,"User-Agent":      "Mozilla/5.0 Chrome"
                      ,"Accept-Language": "en-US,en;q=0.9"
                      ,"Cache-Control":   "no-cache"
                      ,"Pragma":          "no-cache"
                      ,"X-Hello":         "Goodbye"
                      }
    //---- older but has v0.68 like classic chromium with codecs: https://github.com/henrypp/chromium/releases
    //---- https://github.com/henrypp/chromium/releases/tag/v67.0.3396.99-r550428-win64
    //,TARGET       = "https://api.github.com/repos/Hibbiki/chromium-win64/releases?page=1&per_page=4"  //has GPU error.
      ,TARGET       = "https://api.github.com/repos/macchrome/chromium/releases?page=1&per_page=4"      //better (has more codecs)

      
      ;

function get(url, onresponse, onheaders){ //supports both headers and request body handling.
  url = URL.parse(url);
  
  const CONF = {protocol: url.protocol               // "http:"
               ,auth:     url.auth                   // "username:password"
               ,hostname: url.hostname               // "www.example.com"
               ,port:     url.port                   // 80
               ,path:     url.path                   // "/"
               ,family:   4                          // IPv4
               ,method:   "GET"
               ,headers:  HEADERS
               ,agent:    undefined                  //use http.globalAgent for this host and port.
               ,timeout:  10 * 1000                  //10 seconds
               }
       ,REQUEST = (/https\:/.test(url.protocol) ? require("https") : require("http")).request(CONF)
       ,CONTENT = []
       ;
  REQUEST.setSocketKeepAlive(false);                                      //make sure to return right away (single connection mode).
  REQUEST.on("response", function(response){
    if("function" === typeof onheaders) onheaders(REQUEST,response,URL,CONTENT.join("")); //response headers.
    if("function" === typeof onresponse){
      response.setEncoding("utf8");
      response.on("data", function(chunk){ CONTENT.push(chunk);                                  } );  
      response.on("end",  function(){      onresponse(CONTENT.join(""), URL, REQUEST, response); } );  //response body.
    }
  });

  REQUEST.end();
}

console.error("[INFO] getting page...")

get(TARGET, function(content){
  content = JSON.parse(content);                                                                        //all releases in the page (limited by the API-arguments to "first-page, 4 latest releases")
  content = content.filter(function(release){return (false === /macOS/i.test(release.name || release.tag_name));}); //filter out macOSX.
  content = content.shift();                                                                            //keep first release. first is latest in this repository.
  content = content.assets.map(function(asset){return asset.browser_download_url;});                    //keep just download urls of all assests of the single release.
  content = content.filter(function(url){return /[\-\.]sync\./i.test(url);});                           //keep just the url of the archive chrome that allows browser-sync (usually an array of one result).  an example for a file name is "Chrome-bin-sync.7z" (macchrome/chromium) or "chrome.sync.7z" (Hibbiki/chromium-win64)
  content = content.filter(function(url){return /\.7z/i.test(url);});                                   //avoid installers
  content = content.shift();                                                                            //keep first result.

  /*#################################*/
  /*--*/  console.log(content);  /*--*/
  /*#################################*/

  console.error("[INFO] Done.")
  process.exitCode=0;
  process.exit();
});

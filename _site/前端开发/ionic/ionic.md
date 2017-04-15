heidsoftdeMacBook-Air:myApp heidsoft$ ionic platform add ios
******************************************************
 Dependency warning - for the CLI to run correctly,
 it is highly recommended to install/upgrade the following:

 Please install your Cordova CLI to version  >=4.2.0 `npm install -g cordova`
 Install ios-sim to deploy iOS applications.`npm install -g ios-sim` (may require sudo)
 Install ios-deploy to deploy iOS applications to devices.  `npm install -g ios-deploy` (may require sudo)

******************************************************

There is an error in your gulpfile:
Error: EACCES: permission denied, open '/Users/heidsoft/.config/configstore/bower-github.json'
You don't have access to this file.

    at Error (native)
    at Object.fs.openSync (fs.js:549:18)
    at Object.fs.readFileSync (fs.js:397:15)
    at Object.create.all.get (/Users/heidsoft/work/myApp/node_modules/bower/lib/node_modules/configstore/index.js:35:26)
    at Object.Configstore (/Users/heidsoft/work/myApp/node_modules/bower/lib/node_modules/configstore/index.js:28:44)
    at readCachedConfig (/Users/heidsoft/work/myApp/node_modules/bower/lib/config.js:19:23)
    at defaultConfig (/Users/heidsoft/work/myApp/node_modules/bower/lib/config.js:11:12)
    at Object.<anonymous> (/Users/heidsoft/work/myApp/node_modules/bower/lib/index.js:16:32)
    at Module._compile (module.js:409:26)
    at Object.Module._extensions..js (module.js:416:10)

heidsoftdeMacBook-Air:myApp heidsoft$ sudo npm install -g ios-sim
Password:
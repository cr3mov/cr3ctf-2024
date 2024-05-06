const { workerData, parentPort } = require('worker_threads');

require("./worker_globals.js");

const vm = require('vm');

const secret = process.env["SECRET"];
const flag = process.env["FLAG"];

const stringifyVMOutput = (value) => {
    let result = '';

    switch (typeof value) {
        case 'string':
        case 'object':
            result = JSON.stringify(value);
        
        default:
            result = String(value);
    };

    const cr3Pattern = /cr3\{(.*?)\}/i;
    const cr3MatchSecret = secret.match(cr3Pattern);
    const cr3MatchFlag = flag.match(cr3Pattern);

    if ((cr3MatchSecret && result.includes(cr3MatchSecret[1])) || (cr3MatchFlag && result.includes(cr3MatchFlag[1]))) {
        return "you don't wanna see it, trust me...";
    }

    return result;
};

process.env = null;
process = null;
fetch = null;

const moduleRequire = globalThis.module.require;
globalThis.module.require = (id) => {
    const whitelist = [
        "crypto",
        "path",
        "buffer",
        "util",
        "worker_threads",
        "stream",
        "string_decoder",
        "console",
        "perf_hooks"
    ];

    if (!whitelist.some(p => id == p))
        return;
    
    return moduleRequire(id);
}

let result;
try {
    result = vm.runInNewContext(`(() => { return ${workerData} })();`, Object.create(null));

    parentPort.postMessage(stringifyVMOutput(result));
} catch (e) {
    parentPort.postMessage(e.message);
}
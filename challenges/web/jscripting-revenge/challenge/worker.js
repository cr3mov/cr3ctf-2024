const { workerData, parentPort } = require('worker_threads');

require("./worker_globals.js");
const { runBytecodeFile } = require("./bytecode.js")

globalThis.require = require;

runBytecodeFile("./utils.jsc")();

const vm = require('vm');

const secret = process.env["SECRET"] || "cr3{test_secret}";
const flag = process.env["FLAG"] || "cr3{test_flag}";

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

process.env = {};
process = null;
fetch = null;

let result;
try {
    result = vm.runInNewContext(`(() => { return ${workerData} })();`, Object.create(null));

    parentPort.postMessage(stringifyVMOutput(result));
} catch (e) {
    parentPort.postMessage(e.message);
}
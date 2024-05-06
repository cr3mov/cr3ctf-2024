globalThis.module = module;

const secret = process.env["SECRET"];
const flag = process.env["FLAG"];

globalThis.storage = new Proxy({ secret },
{
    get: (target, name) => {
        if (name == "secret") {
            return null;
        }

        return target[name];
    },

    getOwnPropertyDescriptor: (target, name) => {
        if (name == "secret") {
            return {
                value: flag,
                writable: true,
                enumerable: true,
                configurable: true,
            };
        }

        return target[name];
    }
});
const secret = process.env["SECRET"] || "cr3{test_secret}";
const flag = process.env["FLAG"] || "cr3{test_flag}";

globalThis.storage = new Proxy({ secret },
{
    get: (target, name) => {
        if (name === "secret") {
            return null;
        }

        return target[name];
    },

    getOwnPropertyDescriptor: (target, name) => {
        if (name === "secret") {
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
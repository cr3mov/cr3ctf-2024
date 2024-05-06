const express = require("express");
const app = express();
const router = express.Router();
const cookieParser = require("cookie-parser");
const crypto = require("crypto");
const admin = require("./admin.js")
const axios = require('axios');
const dns = require('node:dns')

app.use(cookieParser())
app.use(express.urlencoded({ extended: true }))
app.use("/", router);
app.set("view engine", "ejs");

const PORT = 3000
const FLAG = process.env.FLAG || "cr3{fake_flag}"
let HEALTHCHECK = '127.0.0.1';
dns.resolve('healthcheck', 'A', (err, addr, family) => {
    if (addr)
        HEALTHCHECK = addr[0];
});

app.use(function (req, res, next) {
    res.setHeader('Content-Security-Policy', "script-src https://hcaptcha.com https://*.hcaptcha.com; \
    frame-src https://hcaptcha.com https://*.hcaptcha.com;\
    object-src 'none' ;\
    style-src 'self' https://hcaptcha.com https://*.hcaptcha.com https://cdnjs.cloudflare.com;\
    connect-src https://hcaptcha.com https://*.hcaptcha.com");
    next();
});

let users = [{
    // admin account
    id: crypto.randomBytes(64).toString("hex"),
    username: FLAG,
    password: crypto.randomBytes(64).toString("hex"),
}]

function checkCookie(req, res, next) {
    let auth = req.cookies.auth || undefined
    if (req.connection.remoteAddress === "127.0.0.1") {
        next()
        return;
    }
    if (auth) {
        if (auth.allowed) {
            next()
            return;
        }
        return res.status(403).send("You are not allowed to use this for the moment");
    }
    return res.status(403).send("Login first!");
}

app.get("/", (req, res) => {
    return res.render("index", {
        "result": ""
    });
});

app.post("/", (req, res) => {
    const username = req.body.username
    const password = req.body.password
    if (username && password) {
        if (typeof (username) === "string" || typeof (password) === "string") {
            if (username.startsWith("cr3{")) {
                return res.render("index", {
                    "result": "No!"
                });
            }
            let user_id = crypto.randomBytes(64).toString("hex")
            let user = {
                id: user_id,
                username: username,
                password: password
            }
            users.push(user)
            res.cookie("auth", JSON.stringify(user), {
                maxAge: 1000 * 60 * 10,
                httpOnly: false
            })
            return res.redirect(`/dashboard/${user_id}`)
        } else {
            return res.render("index", {
                "result": "Hacking attempt!"
            });
        }
    }
    return res.render("index", {
        "result": "Username or Password is empty"
    });
});

app.get("/dashboard/:user_id", checkCookie, (req, res, next) => {
    let user_id = req.params.user_id
    let user = users.find(user => user.id === user_id);
    if (user) {
        if (req.socket.remoteAddress === "127.0.0.1") {
            return res.render("dashboard", {
                "username": user.username
            });
        } else {
            return res.render("dashboard", {
                "username": user.username
            });
        }
    }
    return res.send("User not found");
});

const something = (function () {
    var c;
    var what = [];
    for (var n = 0; n < 256; n++) {
        c = n;
        for (var k = 0; k < 8; k++) {
            c = ((c & 1) ? (0xEDB88320 ^ (c >>> 1)) : (c >>> 1));
        }
        what[n] = c;
    }
    return what;
})();

app.get("/let_me_in_please", (req, res, next) => {
    var who = function (str) {
        var what = something;
        var result = 0 ^ (-1);

        for (var i = 0; i < str.length; i++) {
            result = (result >>> 8) ^ what[(result ^ str.charCodeAt(i)) & 0xFF];
        }

        return (result ^ (-1)) >>> 0;
    };

    if (/^([\da-f]{1,4}:){7}[\da-f]{1,4}$/i.test(req.socket.remoteAddress) ||
        who(req.socket.remoteAddress) != 0x13371337) {
        return res.status(403).send("try harder");
    }

    res.cookie("auth", JSON.stringify({ ...(req.cookies.auth || {}), allowed: true }), {
        maxAge: 1000 * 60 * 10,
        httpOnly: false
    })
    return res.status(200).send("yay");
});

app.get("/search", (req, res, next) => {
    if (req.socket.remoteAddress != "127.0.0.1") {
        return res.status(403).send("Not for you lol")
    }
    let username = req.query.username
    let user = users.find(user => user.username.startsWith(username));
    if (user) {
        return res.status(200).send("User found:" + user.username)
    }
    return res.status(404).send("User not found");
});

app.get("/report", checkCookie, (req, res) => {
    var url = req.query.url;
    var client_response = req.query["h-captcha-response"]
    var data = {
        response: client_response,
        secret: process.env.CAPTCHA_SECRET || ""
    };
    const ex = /(https?:\/\/(?:www\.|(?!www))[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\.[^\s]{2,}|www\.[a-zA-Z0-9][a-zA-Z0-9-]+[a-zA-Z0-9]\.[^\s]{2,}|https?:\/\/(?:www\.|(?!www))[a-zA-Z0-9]+\.[^\s]{2,}|www\.[a-zA-Z0-9]+\.[^\s]{2,})/gi
    if (url) {
        if (req.socket.remoteAddress != HEALTHCHECK) {
            axios.post('https://api.hcaptcha.com/siteverify', data, {
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                }
            })
                .then(function (result) {
                    if (result.data.success) {
                        if (url.match(ex)) {
                            admin.visit(url).then((e) => {
                                res.render("report", {
                                    "result": "Visited successfully"
                                });
                            }).catch((err) => {
                                console.log(err)
                                res.render("report", {
                                    "result": "Something went wrong.."
                                });
                            });
                        } else {
                            res.render("report", {
                                "result": "Invalid URL"
                            });
                        }
                    } else {
                        return res.render("report", {
                            "result": "Invalid Captcha!"
                        });
                    }
                })
                .catch(function (err) {
                    console.log(err)
                    return res.render("report", {
                        "result": "Invalid Captcha!"
                    });
                });
        } else {
            admin.visit(url)
            return res.send("admin visited healthcheck exploit");
        }
    } else {
        return res.render("report", {
            "result": ""
        });
    }
})

app.listen(PORT, "0.0.0.0", () => {
    console.log(`Server Running at 127.0.0.1:${PORT}`);
});
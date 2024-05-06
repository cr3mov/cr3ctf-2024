#!/usr/local/bin/node
const puppeteer = require('puppeteer');
const fs = require('fs');
const net = require('net');

const BOT_TIMEOUT = process.env.BOT_TIMEOUT || 60;
const BOT_TIMEOUT_MS = BOT_TIMEOUT * 1000;
const DOMAIN = process.env.DOMAIN || "1337.sb";
const PROTO = process.env.PROTO || "https";
const BASE_URL = PROTO + "://" + DOMAIN + "/";
const FLAG = process.env.FLAG || "cr3{hello}";

const COOKIE = JSON.parse(
    fs.readFileSync('./cookie')
        .toString('utf-8')
        .replaceAll('FLAG_VALUE', FLAG)
        .replaceAll('DOMAIN_VALUE', DOMAIN)
        .replaceAll('PROTO', PROTO)
);
console.log(COOKIE);

const puppeter_args = {"args": [
    '--no-sandbox',
    `--window-size=1920,1080`,
    '--window-position=0,0',
    '--hide-scrollbars',
    '--disable-background-timer-throttling',
    '--disable-renderer-backgrounding',
    '--unsafely-treat-insecure-origin-as-secure=http://healthcheck:1010'], headless: true};

(async function(){
  const browser = await puppeteer.launch(puppeter_args);

  function ask_for_url(socket) {
      socket.state = 'URL';
      socket.write('url?\n');
  }

  async function load_url(socket, data) {
    let url = data.toString().trim();
    console.log(`checking url: ${url}`);

    if (!url.startsWith(BASE_URL)) {
        socket.state = 'ERROR';
        socket.write('huh? you sure? (invalid url)\n');
        socket.destroy();
        return;
    }

    socket.state = 'LOADED';

    const context = await browser.createBrowserContext();
    const page = await context.newPage();
    await page.setCookie(COOKIE);
    await page.setExtraHTTPHeaders({"ngrok-skip-browser-warning": "please"});

    socket.write(`loading page ${url}\n`);
    setTimeout(()=>{
      try {
        context.close();
        socket.write('done?\n');
        socket.destroy();
      } catch (err) {
        console.log(`err: ${err}`);
        try {
            socket.write('something went wrong, contact admins\n');
            socket.destroy();
        } catch (err) {
            console.log(`double err :sob: ${err}`);
        }
      }
    }, BOT_TIMEOUT_MS);

    await page.goto(url);
  }

  var server = net.createServer();
  server.listen(1337);
  console.log('listening on port 1337');

  server.on('connection', socket=>{
    socket.on('data', data=>{
      try {
        if (socket.state == 'URL') {
          load_url(socket, data);
        }
      } catch (err) {
        console.log(`err: ${err}`);
      }
    });

    try {
      ask_for_url(socket);
    } catch (err) {
      console.log(`err: ${err}`);
    }
  });
})();

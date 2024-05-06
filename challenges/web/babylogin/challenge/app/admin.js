const puppeteer = require('puppeteer')

const browser_options = {"args": [
    '--no-sandbox',
    `--window-size=1920,1080`,
    '--window-position=0,0',
    '--hide-scrollbars',
    '--disable-background-timer-throttling',
    '--disable-renderer-backgrounding'], headless: true};

module.exports.visit = async function (url) {
    const browser = await puppeteer.launch(browser_options);

    try {
        const context = await browser.createBrowserContext();
        const page = await context.newPage();

        await page.goto(url, { timeout: 3000, waitUntil: 'domcontentloaded' });
    } catch (err) {
        console.log(err);
    }

    setTimeout(async () => {
        try { await browser.close(); } catch(err) { console.log('nuh uh', err); }        
    }, 35000);
}

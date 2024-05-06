const express = require('express');
const bodyParser = require('body-parser');
const { Worker } = require('worker_threads');

const workerWaitForMessage = async (worker, timeout = 3000) => {
    return new Promise((resolve, reject) => {
        const timeoutId = setTimeout(() => {
            worker.off('message', onMessage);

            reject(new Error("Response timed out."));
        }, timeout);

        const onMessage = (response) => {            
            clearTimeout(timeoutId);

            worker.off('message', onMessage);
            resolve(response);
        };

        worker.once('message', onMessage);
    });
};

const app = express();
const PORT = 3000;

app.set('view engine', 'hbs');

app.use(bodyParser.json({ extended: true }));

app.post('/api/execute', async (req, res) => {
    const data = req.body?.script;

    if (!data)
        return res.send(`Script body wasn't found.`);

    let result = '';

    try {
        const worker = new Worker('./worker.js', { workerData: data });

        result = await workerWaitForMessage(worker);

        await worker.terminate();
    } catch (e) {
        result += e.message;
    }

    return res.send(result);
});

app.get('/', (_req, res) => res.render('maker'));

app.listen(PORT, () => {
    console.log(`Started on port http://localhost:${PORT}`);
});
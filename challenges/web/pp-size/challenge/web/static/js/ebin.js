document.querySelector('form').addEventListener('submit', async e => {
    e.preventDefault();
    const formData = new FormData(e.target);
    const response = 
        fetch(e.target.action, {
            method: e.target.method,
            body: formData
        })
        .then(r => r.json())
        .then(j => {
            console.log(j)
        });
});
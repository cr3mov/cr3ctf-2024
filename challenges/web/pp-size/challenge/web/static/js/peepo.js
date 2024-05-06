function sendCommentData(form, formData) {
    formData.set("message", btoa(document.querySelector("textarea").value));
    fetch(form.action, {
        method: form.method,
        body: formData
    }).then((response) => {
        if (response.redirected) {
            window.location = response.url;
            return new Promise((res, _) => { res();});
        } else
            return response.text()
    }).then((data) => {
        if(data) alert(data)
    });
}

document.querySelectorAll("form").forEach(form => {
    form.addEventListener("submit", async e => {
        e.preventDefault();
        const formData = new FormData(form);
        sendCommentData(form, formData);
    });
});

function postComment(token) {
    const form = document.querySelector("form")
    const formData = new FormData(form);

    formData.set("sql", token);
    sendCommentData(form, formData);
}
// Function to handle keyboard input
const keys = {};
window.addEventListener("keydown", e => {
    keys[e.keyCode] = true;
});
window.addEventListener("keyup", e => {
    keys[e.keyCode] = false;
});
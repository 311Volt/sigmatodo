

function wsURI(path) {
    return ((window.location.protocol === "https:") ? "wss://" : "ws://") + window.location.host + path;
}

let ws = new WebSocket(wsURI("/devmode/refresh"));
ws.onmessage = (msg) => {
    console.log(msg);
    if(msg.data === "refresh") {
        location.reload();
    }
};

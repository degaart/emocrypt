"use strict";

let worker;

function onLoad() {
    setInterval(onTimer, 1000);
    
    const channelCombo = document.getElementById("channel-combo");
    channelCombo.addEventListener("input", onChannelSelected);
    onChannelSelected({ target: channelCombo });
    
    const modeCombo = document.getElementById("mode-combo");
    modeCombo.addEventListener("input", onModeSelected);
    onModeSelected({ target: modeCombo });
    
    document.getElementById("perform-btn").disabled = true;
    
    document.getElementById("copy-btn").addEventListener("click", onCopy);
    
    if(window.Worker) {
        if(!worker) {
            worker = new Worker("js/worker.js");
            worker.onmessage = (msg) => {
                if(msg.data.type == "initialized") {
                    document.getElementById("perform-btn").disabled = false;
                }
            };
        }
    } else {
        alert("Your browser is tool old for this page. Use the command-line version");
    }
}

function onModeSelected(evt) {
    switch(evt.target.value) {
        case "conceal":
        document.getElementById("channel-div").style.display = "block";
        document.getElementById("perform-btn").innerText = "Encrypt and Conceal";
        document.getElementById("perform-btn").onclick = onConceal;
        break;
        case "encrypt":
        document.getElementById("channel-div").style.display = "none";
        document.getElementById("perform-btn").innerText = "Encrypt";
        document.getElementById("perform-btn").onclick = onEncrypt;
        break;
        case "decrypt":
        document.getElementById("channel-div").style.display = "none";
        document.getElementById("perform-btn").innerText = "Decrypt";
        document.getElementById("perform-btn").onclick = onDecrypt;
        break;
    }
    
    document.getElementById("output-txt").innerText = "";
}

function onChannelSelected(evt) {
    const channel = document.getElementById("channel-txt");
    if(evt.target.value == "random") {
        let keys = [];
        for(let key in channels) {
            if(channels.hasOwnProperty(key) && key != "custom") {
                keys.push(key);
            }
        }
        channel.value = channels[keys[Math.floor(Math.random() * keys.length)]];
    } else if(evt.target.value != "custom") {
        channel.value = channels[evt.target.value];
    }
}

function onTimer() {
    const text = document.getElementById("title-text");
    if(text.style.visibility == "")
    text.style.visibility = "hidden";
    else
    text.style.visibility = "";
}

function onCopy() {
    const text = document.getElementById("output-txt").innerText;
    if (!navigator.clipboard) {
        const textArea = document.createElement("textarea");
        textArea.value = text;
        textArea.style.top = "0";
        textArea.style.left = "0";
        textArea.style.position = "fixed";
        
        document.body.appendChild(textArea);
        textArea.focus();
        textArea.select();
        
        try {
            const successful = document.execCommand("copy");
            const msg = successful ? "successful" : "unsuccessful";
        } catch (err) {
            console.error(err);
        }
        
        document.body.removeChild(textArea);
    } else {
        navigator.clipboard.writeText(text).then(function() {
            /* Do nothing */
        }, function(err) {
            console.error(err);
        });
    }
}

function onConceal() {
    const password = document.getElementById("password-txt").value;
    const input = document.getElementById("input-txt").value;
    const channel = document.getElementById("channel-txt").value;
    document.getElementById("output-txt").innerText = "";
    
    if(password.length == 0) {
        alert("Please enter a password");
        return;
    } else if(input.length == 0) {
        alert("Please enter some text to encrypt");
        return;
    } else if(channel.length == 0) {
        alert("Please enter some text to hide the encrypted text into");
        return;
    }
    
    const performBtn = document.getElementById("perform-btn");
    const oldText = performBtn.innerText;
    performBtn.disabled = true;
    performBtn.innerText = "Reticulating splines...";
    worker.onmessage = (msg) => {
        if(msg.data.type == "result") {
            if(msg.data.result.length == 0) {
                alert("Encryption failed");
            } else {
                document.getElementById("output-txt").innerText = msg.data.result;
            }
        } else {
            alert("An unknown error occured");
            console.error("msg:", msg.data);
        }
        performBtn.disabled = false;
        performBtn.innerText = oldText;
        onChannelSelected({ target: document.getElementById("channel-combo") });
    };
    worker.postMessage({
        mode: "conceal",
        password: password,
        plaintext: input,
        channel: channel
    });
}

function onEncrypt() {
    const password = document.getElementById("password-txt").value;
    const input = document.getElementById("input-txt").value;
    document.getElementById("output-txt").innerText = "";
    
    if(password.length == 0) {
        alert("Please enter a password");
        return;
    } else if(input.length == 0) {
        alert("Please enter some text to encrypt");
        return;
    }
    
    const performBtn = document.getElementById("perform-btn");
    const oldText = performBtn.innerText;
    performBtn.disabled = true;
    performBtn.innerText = "Reticulating splines...";
    worker.onmessage = (msg) => {
        if(msg.data.type == "result") {
            if(msg.data.result.length == 0) {
                alert("Encryption failed");
            } else {
                document.getElementById("output-txt").innerText = msg.data.result;
            }
        } else {
            alert("An unknown error occured");
            console.error("msg:", msg.data);
        }
        performBtn.disabled = false;
        performBtn.innerText = oldText;
    };
    worker.postMessage({
        mode: "encrypt",
        password: password,
        plaintext: input
    });
}

function onDecrypt() {
    const password = document.getElementById("password-txt").value;
    const input = document.getElementById("input-txt").value;
    document.getElementById("output-txt").innerText = "";
    
    if(password.length == 0) {
        alert("Please enter a password");
        return;
    } else if(input.length == 0) {
        alert("Please enter some text to decrypt");
        return;
    }
    
    const performBtn = document.getElementById("perform-btn");
    const oldText = performBtn.innerText;
    performBtn.disabled = true;
    performBtn.innerText = "Reticulating splines...";
    worker.onmessage = (msg) => {
        if(msg.data.type == "result") {
            if(msg.data.result.length == 0) {
                alert("Wrong password (or invalid encrypted text)");
            } else {
                document.getElementById("output-txt").innerText = msg.data.result;
            }
        } else {
            alert("An unknown error occured");
            console.error("msg:", msg.data);
        }
        performBtn.disabled = false;
        performBtn.innerText = oldText;
    };
    worker.postMessage({
        mode: "decrypt",
        password: password,
        ciphertext: input
    });
}

window.addEventListener("load", onLoad);

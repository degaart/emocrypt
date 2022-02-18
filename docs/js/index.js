"use strict";

var Module = {
    onRuntimeInitialized: function() {
        if (Module.getRandomValue === undefined) {
            var randomValuesStandard = function() {
                var buf = new Uint32Array(1);
                window.crypto.getRandomValues(buf);
                return buf[0] >>> 0;
            };
            Module.getRandomValue = randomValuesStandard;
        }
        
        /*document.getElementById("encrypt-btn").addEventListener("click", onEncrypt);
        document.getElementById("decrypt-btn").addEventListener("click", onDecrypt);*/
        setInterval(onTimer, 1000);

        const channelCombo = document.getElementById("channel-combo");
        channelCombo.addEventListener("input", onChannelSelected);
        onChannelSelected({ target: channelCombo });

        const modeCombo = document.getElementById("mode-combo");
        modeCombo.addEventListener("input", onModeSelected);
        onModeSelected({ target: modeCombo });
    }
};

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
    channel.value = channels[evt.target.value];
}

function onTimer() {
    const text = document.getElementById("title-text");
    if(text.style.visibility == "")
    text.style.visibility = "hidden";
    else
    text.style.visibility = "";
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
    
    const result = Module.ec_conceal(password, input, channel);
    if(result && result.length > 0)
        document.getElementById("output-txt").innerText = result;
    else
        alert("Encryption failed");
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
    
    const result = Module.ec_encrypt(password, input);
    if(result && result.length > 0)
        document.getElementById("output-txt").innerText = result;
    else
        alert("Encryption failed");
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
    
    const result = Module.ec_decrypt(password, input);
    if(result && result.length > 0)
        document.getElementById("output-txt").innerText = result;
    else
        alert("Decryption failed");
}

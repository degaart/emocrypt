"use strict";

importScripts("emocrypt.js");

Module.onRuntimeInitialized = () => {
    if(!Module.getRandomValue) {
        var randomValuesStandard = function() {
            var buf = new Uint32Array(1);
            self.crypto.getRandomValues(buf);
            return buf[0] >>> 0;
        };
        Module.getRandomValue = randomValuesStandard;
    }
    postMessage({ type: "initialized" });
}

onmessage = (msg) => {
    let result;
    switch(msg.data.mode) {
        case "conceal":
            result = Module.ec_conceal(msg.data.password, msg.data.plaintext, msg.data.channel);
            postMessage({ type: "result", result: result });
            break;
        case "encrypt":
            result = Module.ec_encrypt(msg.data.password, msg.data.plaintext);
            postMessage({ type: "result", result: result });
            break;
        case "decrypt":
            result = Module.ec_decrypt(msg.data.password, msg.data.ciphertext);
            postMessage({ type: "result", result: result });
            break;
    }
}

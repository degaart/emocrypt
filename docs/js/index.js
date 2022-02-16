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
        
        document.getElementById("encrypt-btn").addEventListener("click", onEncrypt);
        document.getElementById("decrypt-btn").addEventListener("click", onDecrypt);
        setInterval(onTimer, 1000);
    }
};

function onTimer() {
    const text = document.getElementById("title-text");
    if(text.style.visibility == "")
    text.style.visibility = "hidden";
    else
    text.style.visibility = "";
}

function onEncrypt() {
    const password = document.getElementById("password-txt").value;
    const plaintext = document.getElementById("input-txt").value;
    document.getElementById("output-txt").value = "";
    
    if(password.length == 0) {
        alert("Please enter a password");
        return;
    } else if(plaintext.length == 0) {
        alert("Please enter some text to encrypt");
        return;
    }
    
    const result = Module.ec_encrypt(password, plaintext);
    if(result)
    document.getElementById("output-txt").value = result;
    else
    alert("Encryption failed");
}

function onDecrypt() {
    const password = document.getElementById("password-txt").value;
    const ciphertext = document.getElementById("input-txt").value;
    document.getElementById("output-txt").value = "";
    
    if(password.length == 0) {
        alert("Please enter a password");
        return;
    } else if(ciphertext.length == 0) {
        alert("Please enter some text to decrypt");
        return;
    }
    
    const result = Module.ec_decrypt(password, ciphertext);
    if(result)
    document.getElementById("output-txt").value = result;
    else
    alert("Decryption failed");
}

/**************** Marquee of sade ****************/
function getStringWidth(str) {
    
    var span = document.createElement("span");
    span.innerText = str;
    span.style.visibility = "hidden";
    
    var body = document.getElementsByTagName("body")[0];
    body.appendChild(span);
    var textWidth = span.offsetWidth;
    body.removeChild(span);
    
    return textWidth;
}

function getAnimationRule(animationName) {
    var KEYFRAME_RULE = window.CSSRule.WEBKIT_KEYFRAMES_RULE ||
    window.CSSRule.MOZ_KEYFRAMES_RULE ||
    window.CSSRule.KEYFRAMES_RULE;
    
    var stylesheets = document.styleSheets;
    for (var i = 0 ; i < stylesheets.length ; i++) {
        var rules = stylesheets[i].cssRules;
        for (var j = 0 ; j < rules.length ; j++) {
            var rule = rules[j];
            if (rule.type == KEYFRAME_RULE && rule.name == "marquee") {
                return rule;
            }
        }
    }
}

function updateMarqueeAmplitude(element) {
    
    var animationName = "marquee";
    var marqueeRule = getAnimationRule(animationName);
    if (null == marqueeRule) {
        return;
    }
    
    // remove the old animation (if any)
    element.style.webkitAnimationName = "none";
    
    var textWidth = getStringWidth(element.innerText);
    
    // update the values of our keyframe animation
    marqueeRule.deleteRule("0%");
    marqueeRule.deleteRule("100%");
    marqueeRule.insertRule('0% { text-indent: ' + element.offsetWidth + 'px; }');
    marqueeRule.insertRule('100% { text-indent: ' + -textWidth + 'px; }');
    
    // re-assign the animation (to make it run)
    element.style.webkitAnimationName = animationName;
}

updateMarqueeAmplitude(document.querySelector(".marquee"));

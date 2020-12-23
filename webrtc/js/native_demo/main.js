
'use strict';

function showMsg(msg) {
    const errorElement = document.querySelector('#errorMsg');
    errorElement.innerHTML += `<p>${msg}</p>`;
}

const loginButton = document.getElementById('loginButton');
loginButton.addEventListener('click', login);
const callButton = document.getElementById('callButton');
callButton.addEventListener('click', call);
const localVideo = document.getElementById('localVideo');
const remoteVideo = document.getElementById('remoteVideo');

let peerId;
let loginRequest = null;
let hangingGet = null;
const server = 'http://localhost:8888';

function GetIntHeader(r, name) {
    var val = r.getResponseHeader(name);
    return val != null && val.length ? parseInt(val) : -1;
}

function hangingGetCallback() {
    try {
        if (hangingGet.readyState != 4)
            return;
        if (hangingGet.status != 200) {
            showMsg("server error: " + hangingGet.statusText);
        } else {
            var peer_id = GetIntHeader(hangingGet, "Pragma");
            showMsg(peer_id);
            showMsg(hangingGet.responseText);
        }

        if (hangingGet) {
            hangingGet.abort();
            hangingGet = null;
        }

        if (peerId != -1)
            window.setTimeout(startHangingGet, 0);
    } catch (e) {
        showMsg("Hanging get error: " + e.description);
    }
}

function startHangingGet() {
    try {
        hangingGet = new XMLHttpRequest();
        hangingGet.onreadystatechange = hangingGetCallback;
        hangingGet.ontimeout = onHangingGetTimeout;
        hangingGet.open("GET", server + "/wait?peer_id=" + peerId, true);
        hangingGet.send();
    } catch (e) {
        showMsg("error" + e.description);
    }
}

function onHangingGetTimeout() {
    showMsg("hanging get timeout. issuing again.");
    hangingGet.abort();
    hangingGet = null;
    if (peerId != -1)
        window.setTimeout(startHangingGet, 0);
}

function signInCallback() {
    try {
        if (loginRequest.readyState == 4) {
            if (loginRequest.status == 200) {
                var peers = loginRequest.responseText.split("\n");
                peerId = parseInt(peers[0].split(',')[1]);
                showMsg("My id: " + peerId);
                otherPeerId = parseInt(peers[1].split(',')[1]);
                showMsg("otherPeerId: " + otherPeerId);
                startHangingGet();
            }
        }
    } catch (e) {
        showMsg("error: " + e.description);
    }
}

// function sendOfferCallback() {
//     showMsg(controlRequest.responseText);
//     try {
//         if (controlRequest.readyState == 4) {
//             if (controlRequest.status == 200) {
//                 showMsg(controlRequest.responseText);
//                 // offerRequest = null;
//             }
//         }
//     } catch (e) {
//         showMsg("error: " + e.description);
//     }
// }

// function candidateCalback() {
//     try {
//         if (controlRequest.readyState == 4) {
//             if (controlRequest.status == 200) {
//                 showMsg(controlRequest.responseText);
//                 // offerRequest = null;
//             }
//         }
//     } catch (e) {
//         showMsg("error: " + e.description);
//     }
// }

let localStream;
let otherPeerId;
async function login() {
    try {
        showMsg('begin login');

        loginRequest = new XMLHttpRequest();
        loginRequest.onreadystatechange = signInCallback;
        loginRequest.open("GET", "http://localhost:8888/sign_in?js_peer_test", true);
        loginRequest.send();
        localStream = await navigator.mediaDevices.getUserMedia({ audio: false, video: true });
        // localVideo.srcObject = localStream;
    } catch (e) {
        showMsg("error: " + e.description);
    }
}

let peerConnection;
async function call() {
    const configuration = { iceServers: [{ urls: "stun:stun.l.google.com:19302" }] };
    peerConnection = new RTCPeerConnection(configuration);
    peerConnection.addEventListener('icecandidate', async (event) => {
        if (event.candidate == null) {
            return;
        }
        try {
            showMsg('gather icecandidate: ' + event.candidate.candidate);
            // const candidateReq = {
            //     'sdpMid': event.candidate.sdpMid,
            //     'sdpMLineIndex': event.candidate.sdpMLineIndex,
            //     'candidate': event.candidate.candidate
            // };
            // const candidateReqJson = JSON.stringify(candidateReq);
            // const url = 'http://localhost:8888/message?peer_id=' + peerId + '&to=' + otherPeerId;
            // hangingGet.open("POST", url, true);
            // hangingGet.send(candidateReqJson);
        } catch (e) {
            showMsg(e);
        }
    });
    peerConnection.addEventListener('track', (event) => {
        remoteVideo.srcObject = event.streams[0];
        showMsg('get track');
    });
    localStream.getTracks().forEach(track => {
        peerConnection.addTrack(track, localStream);
    });
    const offerSdp = await peerConnection.createOffer({ offerToReceiveAudio: 1, offerToReceiveVideo: 1 });
    await peerConnection.setLocalDescription(offerSdp);

    const url = 'http://localhost:8888/message?peer_id=' + peerId + '&to=' + otherPeerId;
    try {
        hangingGet.open("POST", url, true);
        hangingGet.send(offerSdp);
    } catch (e) {
        showMsg("error: " + e.description);
    }
}
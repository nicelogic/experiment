/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree.
 */

'use strict';

const startButton = document.getElementById('startButton');
const callButton = document.getElementById('callButton');
startButton.addEventListener('click', start);
callButton.addEventListener('click', call);

const localVideo = document.getElementById('localVideo');
const remoteVideo = document.getElementById('remoteVideo');

const ws = new WebSocket('ws://localhost:3000/');
ws.onmessage = async (event) => {
  if (event == null || event.data == 'null') {
    return;
  }
  console.log(`[message] Data received from server: ${event.data}`);
  const msg = JSON.parse(event.data);
  if (msg.type) {
    await pc1.setRemoteDescription(msg);
  } else {
    pc1.addIceCandidate(msg);
  }
};

localVideo.addEventListener('loadedmetadata', function () {
  console.log(`Local video videoWidth: ${this.videoWidth}px,  videoHeight: ${this.videoHeight}px`);
});

remoteVideo.addEventListener('loadedmetadata', function () {
  console.log(`Remote video videoWidth: ${this.videoWidth}px,  videoHeight: ${this.videoHeight}px`);
});

remoteVideo.addEventListener('resize', () => {
  console.log(`Remote video size changed to ${remoteVideo.videoWidth}x${remoteVideo.videoHeight}`);
});

let localStream;
let pc1;
const offerOptions = {
  offerToReceiveAudio: 0,
  offerToReceiveVideo: 1
};

async function start() {
  console.log('Requesting local stream');
  startButton.disabled = true;
  try {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true, video: true });
    console.log('Received local stream');
    localVideo.srcObject = stream;
    localStream = stream;
    callButton.disabled = false;
  } catch (e) {
    alert(`getUserMedia() error: ${e.name}`);
  }
}

async function call() {
  callButton.disabled = true;
  console.log('Starting call');
  const configuration = { iceServers: [{ urls: "stun:stun.l.google.com:19302" }] };
  pc1 = new RTCPeerConnection(configuration);
  console.log('Created local peer connection object pc1');
  pc1.addEventListener('icecandidate', e => onIceCandidate(pc1, e));
  pc1.addEventListener('iceconnectionstatechange', e => onIceStateChange(pc1, e));
  pc1.addEventListener('track', gotRemoteStream);

  localStream.getTracks().forEach(track => pc1.addTrack(track, localStream));
  console.log('Added local stream to pc1');

  try {
    console.log('pc1 createOffer start');
    const offer = await pc1.createOffer(null);
    await onCreateOfferSuccess(offer);
  } catch (e) {
    console.log(`Failed to create session description: ${e.toString()}`);
  }
}

async function onCreateOfferSuccess(desc) {
  console.log(`Offer from pc1\n${desc.sdp}`);
  console.log('pc1 setLocalDescription start');
  try {
    await pc1.setLocalDescription(desc);
    const offerJson = JSON.stringify(desc);
    ws.send(offerJson);
    console.log('pc setLocalDescription complete');
  } catch (e) {
    console.log(`Failed to set session description: ${error.toString()}`);
  }
}

function gotRemoteStream(e) {
  if (remoteVideo.srcObject !== e.streams[0]) {
    remoteVideo.srcObject = e.streams[0];
    console.log('pc1 received remote stream');
  }
}

async function onIceCandidate(pc, event) {
  try {
    let candidate = event.candidate;
    const json = JSON.stringify(candidate);
    ws.send(json);
    console.log('pc addIceCandidate success');
  } catch (e) {
    console.log('pc failed to add ICE Candidate: ${error.toString()}');
  }
}

function onIceStateChange(pc, event) {
  if (pc) {
    console.log('ICE state change event: ', event);
  }
}

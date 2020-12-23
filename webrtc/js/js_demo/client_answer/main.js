/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree.
 */

'use strict';

const remoteVideo = document.getElementById('remoteVideo');

const configuration = { iceServers: [{ urls: "stun:stun.l.google.com:19302" }] };
let pc2 = new RTCPeerConnection(configuration);
pc2.addEventListener('icecandidate', e => onIceCandidate(pc2, e));
pc2.addEventListener('iceconnectionstatechange', e => onIceStateChange(pc2, e));
pc2.addEventListener('track', gotRemoteStream);

const ws = new WebSocket('ws://127.0.0.1:3001/');
ws.onmessage = async (event) => {
  try {
    if (event == null || event.data == 'null') {
      console.log('null');
      return;
    } 
    console.log(`[message] Data received from server: ${event.data}`);
    const msg = JSON.parse(event.data);
    if (msg.type) {
      await pc2.setRemoteDescription(msg);
      const answer = await pc2.createAnswer();
      await pc2.setLocalDescription(answer);
      const answerJson = JSON.stringify(answer);
      ws.send(answerJson);
    } else {
      pc2.addIceCandidate(msg);
    }
  } catch (e) {
    console.log(e);
  }
};

remoteVideo.addEventListener('loadedmetadata', function () {
  console.log(`Remote video videoWidth: ${this.videoWidth}px,  videoHeight: ${this.videoHeight}px`);
});

remoteVideo.addEventListener('resize', () => {
  console.log(`Remote video size changed to ${remoteVideo.videoWidth}x${remoteVideo.videoHeight}`);
});

const offerOptions = {
  offerToReceiveAudio: 1,
  offerToReceiveVideo: 1
};

function getName(pc) {
  return (pc === pc2) ? 'pc2' : 'pc1';
}

function gotRemoteStream(e) {
  if (remoteVideo.srcObject !== e.streams[0]) {
    remoteVideo.srcObject = e.streams[0];
    console.log('pc2 received remote stream');
  }
}

async function onCreateAnswerSuccess(desc) {
  console.log(`Answer from pc2:\n${desc.sdp}`);
  console.log('pc2 setLocalDescription start');
  console.log('setLocalDescription complete');
  //send answer to pc1
}

async function onIceCandidate(pc, event) {
  try {
    const candidate = event.candidate;
    const json = JSON.stringify(candidate);
    ws.send(json);
    console.log(`${getName(pc)} addIceCandidate success`);
  } catch (e) {
    console.log(`${getName(pc)} failed to add ICE Candidate: ${error.toString()}`);
  }
  console.log(`${getName(pc)} ICE candidate:\n${event.candidate ? event.candidate.candidate : '(null)'}`);
}

function onIceStateChange(pc, event) {
  if (pc) {
    console.log(`${getName(pc)} ICE state: ${pc.iceConnectionState}`);
    console.log('ICE state change event: ', event);
  }
}



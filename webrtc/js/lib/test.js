'use strict';

const video = document.querySelector('#user_video');
const constraints = {
  audio: true,
  video: true
};
navigator.mediaDevices.getUserMedia(constraints).then(handleSuccess).catch(error => {
  console.log('navigator.MediaDevices.getUserMedia error: ', error.message, error.name);
});

const remoteVideo = document.querySelector('#remote_video')
const volumeMeter = document.querySelector('#volume meter');
const callButton = document.querySelector('#callButton')

const peerConnection = new RTCPeerConnection();
const remotePeerConnection = new RTCPeerConnection();
peerConnection.addEventListener("icecandidate", async event => {
  console.log(`candidates:  + ${event.candidate ? event.candidate.candidate : 'null'}`)
  if(event.candidate)
  {
    // await remotePeerConnection.addIceCandidate(event.candidate);
  }
});
remotePeerConnection.addEventListener("icecandidate", async event => {
  console.log(`candidates:  + ${event.candidate ? event.candidate.candidate : 'null'}`)
  if(event.candidate)
  {
    // await remotePeerConnection.addIceCandidate(event.candidate);
  }
});

async function handleSuccess(stream) {
  video.srcObject = stream;
  showVolume(stream, volumeMeter);

  stream.getTracks().forEach(track => peerConnection.addTrack(track, stream));
  console.log("peerconnection add all track")
  const offerOptions = {
    offerToReceiveAudio: 1,
    offerToReceiveVideo: 1
  };
  const offer = await peerConnection.createOffer(offerOptions);
  console.log("create offer: " + offer.sdp);
  await peerConnection.setLocalDescription(offer);

}

callButton.addEventListener('click', () => {



});
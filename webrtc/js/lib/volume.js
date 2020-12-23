
'use strict';

const video = document.querySelector('video');
const volumeMeter = document.querySelector('#volume meter');

let value = 0;
function handleSuccess(stream) {
  video.srcObject = stream;

  window.audioContext = new AudioContext();
  let script = window.audioContext.createScriptProcessor(2048, 1, 1);
  script.onaudioprocess = function (event) {
    const input = event.inputBuffer.getChannelData(0);
    let sum = 0.0;
    for (let i = 0; i < input.length; ++i) {
      sum += input[i] * input[i];
    }
    value = Math.sqrt(sum / input.length);
  };

  let mic = window.audioContext.createMediaStreamSource(stream);
  mic.connect(script);
  // necessary to make sample run, but should not be.
  script.connect(window.audioContext.destination);
  setInterval(() => {
    volumeMeter.value= value.toFixed(2);
  }, 200);
}

const constraints = {
  audio: true,
  video: true
};
navigator.mediaDevices.getUserMedia(constraints).then(handleSuccess).catch(() => {
  console.log('navigator.MediaDevices.getUserMedia error: ', error.message, error.name);
});

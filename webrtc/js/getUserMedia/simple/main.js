
const video = document.querySelector('video');

(async () => {
    const stream = await navigator.mediaDevices.getUserMedia({ video: true, audio: true });
    video.srcObject = stream;
})();
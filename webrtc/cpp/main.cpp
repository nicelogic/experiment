
#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>
#include <cstdio>

#include "modules/audio_device/include/audio_device.h"
#include "api/task_queue/default_task_queue_factory.h"

using namespace std;
using namespace std::chrono_literals;


std::FILE *recordFile{};
class AudioCallbackImpl : public webrtc::AudioTransport
{
public:
    int32_t RecordedDataIsAvailable(const void *audioSamples,
                                    const size_t nSamples,
                                    const size_t nBytesPerSample,
                                    const size_t nChannels,
                                    const uint32_t samplesPerSec,
                                    const uint32_t totalDelayMS,
                                    const int32_t clockDrift,
                                    const uint32_t currentMicLevel,
                                    const bool keyPressed,
                                    uint32_t &newMicLevel) override
    {
        fwrite(audioSamples, nBytesPerSample, nSamples, recordFile);
        cout << "nsamples: " << nSamples << " | "
        << "nbytesPersample: " << nBytesPerSample << " | "
        << "channels: " << nChannels << " | "
        << "samplesPerSec: " << samplesPerSec << " | "
        << "total delay ms: " << totalDelayMS << " | "
        << "clock drift: " << clockDrift << " | "
        << "current mic level: " << currentMicLevel << " | "
        << "key pressed: " << keyPressed << " | "
        << endl;
        return 0;
    }

    int32_t NeedMorePlayData(const size_t nSamples,
                             const size_t nBytesPerSample,
                             const size_t nChannels,
                             const uint32_t samplesPerSec,
                             void *audioSamples,
                             size_t &nSamplesOut, // NOLINT
                             int64_t *elapsed_time_ms,
                             int64_t *ntp_time_ms) override
    {
        return 0;
    }

    void PullRenderData(int bits_per_sample,
                        int sample_rate,
                        size_t number_of_channels,
                        size_t number_of_frames,
                        void *audio_data,
                        int64_t *elapsed_time_ms,
                        int64_t *ntp_time_ms) override {}
};

int main()
{
    recordFile = fopen("record.pcm", "wb");

    auto taskQueue = webrtc::CreateDefaultTaskQueueFactory();
    auto adm = webrtc::AudioDeviceModule::Create(
        webrtc::AudioDeviceModule::kPlatformDefaultAudio,
        taskQueue.get());
    adm->Init();

    const auto num = adm->RecordingDevices();
    printf("Input devices: %d\n", num);
    for (int i = 0; i < num; i++)
    {
        char name[webrtc::kAdmMaxDeviceNameSize];
        char guid[webrtc::kAdmMaxGuidSize];
        int ret = adm->RecordingDeviceName(i, name, guid);
        if (ret != -1)
        {
            printf("%d %s %s\n", i, name, guid);
        }
    }

    AudioCallbackImpl audioCallbackImpl;
    adm->RegisterAudioCallback(&audioCallbackImpl);
    auto ret = adm->SetRecordingDevice(
        webrtc::AudioDeviceModule::WindowsDeviceType::kDefaultDevice);
    ret = adm->InitRecording();
    printf("Start recording\n");
    ret = adm->StartRecording();

    std::this_thread::sleep_for(5s);
    adm->StopRecording();
    adm->Terminate();
    fclose(recordFile);

    return 0;
}
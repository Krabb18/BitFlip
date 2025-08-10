#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <iostream>
#include "raylib.h"
#include "miniaudio.h"
#include <cstring>

#define DEVICE_FORMAT       ma_format_f32
#define DEVICE_CHANNELS     2
#define DEVICE_SAMPLE_RATE  48000

using namespace std;

struct UserData {
    ma_waveform waves[3];
    ma_waveform wave2;
    ma_encoder encoder;
    bool isPlaying = false;

    int waveIndex = 0;
    bool wavIndexes[3];

};

class SoundPlayer
{
    private:
    UserData ud;
    ma_waveform sinWave;
    ma_device_config deviceConfig;
    ma_device device;

    public:
    bool isRecording = false;

    bool playSound1 = false;
    bool playSound2 = false;
    bool playSound3 = false;
    void ClearEncoder();

    void InitPlayer();
    void UpdatePlayer();

    void Delete();
};

#endif

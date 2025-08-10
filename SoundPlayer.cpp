#include "SoundPlayer.h"

bool isRecordingGlobal = false;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    UserData* ud = (UserData*)pDevice->pUserData;

    //ma_waveform_read_pcm_frames((ma_waveform*)pDevice->pUserData, pOutput, frameCount, NULL);
    if(ud->isPlaying)
    {
        ma_uint32 channels = pDevice->playback.channels;
        float* buffer1 = new float[frameCount * channels];
        float* buffer2 = new float[frameCount * channels];
        float* buffer3 = new float[frameCount * channels];
        float* output = new float[frameCount * channels];

        if(ud->wavIndexes[0] == true){  }
        if (ud->wavIndexes[1] == true) {  }

        ma_waveform_read_pcm_frames(&ud->waves[0], buffer1, frameCount, NULL);
        ma_waveform_read_pcm_frames(&ud->waves[1], buffer2, frameCount, NULL);
        ma_waveform_read_pcm_frames(&ud->waves[2], buffer3, frameCount, NULL);

        
        for(ma_uint32 i = 0; i<frameCount * channels; i++)
        {
            float mixed = 0.0f;
            
            if (ud->wavIndexes[0] == true) { mixed += buffer1[i]; }
            if (ud->wavIndexes[1] == true) { mixed += buffer2[i]; }
            if (ud->wavIndexes[2] == true) { mixed += buffer3[i]; }
           
            output[i] = mixed;
        }

        memcpy(pOutput, output, frameCount * channels * sizeof(float));

        delete[] buffer1;
        delete[] buffer2;
        delete[] buffer3;
        delete[] output;
    }
    else
    {
        memset(pOutput, 0, ma_get_bytes_per_frame(DEVICE_FORMAT, DEVICE_CHANNELS) * frameCount);
    }

    if(isRecordingGlobal)
    {
        ma_encoder_write_pcm_frames(&ud->encoder, pOutput, frameCount, NULL);
    }
    cout << "Index0: " << ud->wavIndexes[0] << endl;
    cout << "Index1: " << ud->wavIndexes[1] << endl;

    (void)pInput;   /* Unused. */
}


void SoundPlayer::InitPlayer()
{

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = DEVICE_FORMAT;
    deviceConfig.playback.channels = DEVICE_CHANNELS;
    deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &ud;

    if(ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
    {
        cout << "Failed to open playbacl device" << endl;
    }

    //Hier werden die sounds initet
    ma_waveform_config sinWaveConfig = ma_waveform_config_init(DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE, ma_waveform_type_square, 0.1, 120);
    ma_waveform_init(&sinWaveConfig, &ud.waves[0]);

    ma_waveform_config sinWaveConfig2 = ma_waveform_config_init(DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE, ma_waveform_type_sine, 0.1, 220);
    ma_waveform_init(&sinWaveConfig2, &ud.waves[1]);

    ma_waveform_config sinWaveConfig3 = ma_waveform_config_init(DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE, ma_waveform_type_triangle, 0.1, 120);
    ma_waveform_init(&sinWaveConfig3, &ud.waves[2]);

    if(ma_device_start(&device) != MA_SUCCESS)
    {
        cout << "Failed to start playback device.\n" << endl;
        ma_device_uninit(&device);
    }
}

void SoundPlayer::UpdatePlayer()
{
    ud.wavIndexes[0] = IsKeyDown(KEY_A) || playSound1;
    ud.wavIndexes[1] = IsKeyDown(KEY_D) || playSound2;
    ud.wavIndexes[2] = IsKeyDown(KEY_D) || playSound3;
    ud.isPlaying = ud.wavIndexes[0] || ud.wavIndexes[1] ||ud.wavIndexes[2] ;

    isRecordingGlobal = isRecording;
}

void SoundPlayer::ClearEncoder()
{
    ma_encoder_config encConfig = ma_encoder_config_init(ma_encoding_format_wav,DEVICE_FORMAT, DEVICE_CHANNELS, DEVICE_SAMPLE_RATE);
    if(ma_encoder_init_file("MySound.wav", &encConfig, &ud.encoder) != MA_SUCCESS)
    {
        cout << "Failed to create WAV file" << endl;
    }
}

void SoundPlayer::Delete()
{
    ma_device_uninit(&device);
    ma_encoder_uninit(&ud.encoder); // <<< ganz wichtig!
}
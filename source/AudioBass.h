#pragma once
#include "Audio.h"
#include "AppLog.h"
#include "../../bass/src/bass.h"
#include <vector>


const static int
	ciWAV = 4096,   // wav data, max screen width
	ciFFT = 10240;  // fft data, max fft size

const static int ciFFTNum = 5;
const static uint ciFFTSize[ciFFTNum] = {
	BASS_DATA_FFT512, BASS_DATA_FFT1024, BASS_DATA_FFT2048, BASS_DATA_FFT4096, BASS_DATA_FFT8192};


class AudioBass : public Audio, public LogErr
{
    HSTREAM chPl = 0;   // e.g. ogg mp3
    HMUSIC chMod = 0;   // e.g. xm mod
    DWORD ch() {  return chMod ? chMod : chPl;  }
    HSYNC chSync = 0;
	
	float fft[ciFFT], visA[ciFFT];
//	std::vector<float> fft;
//	std::vector<float> visA;
	
public:
    AudioBass();

    void Init() override;
    void Destroy() override;

    //  player
	bool Play() override;
	void Pause() override;
    void Stop() override;

	void Next() override;
	void Prev() override;
	
	void getPos() override;
	float* getFFT() override
	{  return visA;  }

	void chPos(bool back, bool slow, bool fast) override;
	void chPosAbs(double pos) override;
	void chVol(bool back, bool slow, bool fast) override;
	
	//  get data for audio visualization
	void GetVisData(int size) override;
};

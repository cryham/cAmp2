#pragma once
#include "Audio.h"
#include "AppLog.h"
#include "../../bass/src/bass.h"
#include <vector>


const static int
	ciWAV = 4096,   // wav data, max screen width
	ciFFT = 10240;  // fft data, max fft size


class AudioBass : public Audio, public LogErr
{
    HSTREAM chPl = 0;   // ogg, mp3, etc.
    HMUSIC chMod = 0;   // xm, mod, etc.
    DWORD ch() {  return chMod ? chMod : chPl;  }
    HSYNC chSync = 0;
	
	float fft[ciFFT], visA[ciFFT];
	//std::vector<float> fft, visA;
	
public:
    AudioBass();

    void Init() override;
    void Destroy() override;

    //  player
	bool Play(Track& trk) override;  // can disable track
	void Pause() override;
    void Stop() override;

	void GetPos() override;
	float* GetFFT() override
	{  return visA;  }

	void SetPos(bool back, bool slow, bool fast) override;
	void SetPosAbs(double pos) override;
	void SetVol(bool back, bool slow, bool fast) override;
	
	//  get data for audio visualization
	void GetVisData(int size, const ViewSet &view) override;

	void GetTrkTime(Track& t) override;  // sets time, mod, size
	
	bool IsPlayable(std::string ext) override;
	
	const char* GetAllExtStr() override  // for file open dialog
	{	return sAllExt.c_str();  }

	const static uint ciFFTSize[ViewSet::ciFFTNum];
	
private:
	void InitPlugins();
	void FillExt();
	
	bool IsModFile(std::string ext);
	//  playable file extensions
	std::vector<std::string> vExt;
	std::vector<std::string> vExtMod;
	std::string sAllExt;
};

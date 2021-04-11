#pragma once
#include "Audio.h"
#include "../System/LogFile.h"
#include "../../../bass/src/bass.h"
#include <vector>


const static int
	ciWAV = 10240,  // wav data, max screen width *2
	ciFFT = 10240;  // fft data, max fft size


class AudioBass : public Audio, public AddLog
{
	HSTREAM chPl = 0;   // ogg, mp3, etc.
	HMUSIC chMod = 0;   // xm, mod, etc.
	DWORD ch() {  return chMod ? chMod : chPl;  }
	HSYNC chSync = 0;
	
	float fft[ciFFT], vis[ciFFT];
	short wav[ciWAV];
	
public:
	AudioBass();

	void Init() override;
	void Destroy() override;

	//  player
	bool Play(Track& trk) override;  // can disable track
	void Pause() override;
	void Stop() override;

	void GetPos() override;
	float* GetVisual() override
	{	return vis;  }

	void SetPos(bool back, bool slow, bool fast) override;
	void SetPosAbs(double pos) override;
	void SetVol(bool back, bool slow, bool fast) override;
	
	//  get audio data for visualization
	void GetVisualData(int size, const ViewSet &view) override;

	void GetTrackTime(Track& t) override;  // sets time, mod, size
	
	bool IsPlayable(std::string ext) override;
	
	const char* GetAllExtStr() override  // for file open dialog
	{	return sAllExt.c_str();  }

	const static uint ciFFTSize[ViewSet::FFTSizes];
	
private:
	void InitPlugins();
	void FillExt();
	
	bool IsModFile(std::string ext);
	//  playable file extensions
	std::vector<std::string> vExt;
	std::vector<std::string> vExtMod;
	std::string sAllExt;
};

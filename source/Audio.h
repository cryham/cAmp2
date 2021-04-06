#pragma once
#include "Track.h"
#include "View.h"
#include <string>

class Playlist;


class Audio
{
protected:
    int maxFreq = 0;  // init, devices-

	//  Play state
    bool bPlaying = false;
    bool bPaused  = false;

	Playlist* pls = nullptr;  // for Play, Next

public:
	//  Time pos
    double timePlay = 0.0, timeTrack = 0.0; //, lastPos = 0.0;
	int iVolume = 500;  // 1000 = 100%
	
	std::string sInfo;  // bitRate, freq, size MB
	int rate = 0;  // cur track-
	
	bool bRepTrk = false;  // repeat track
	bool bRepPls = true;  // repeat playlist
	bool bNextPrev = true;  // false back, prev

public:
	bool IsPlaying() {  return bPlaying;  }
    bool IsPaused()  {  return bPaused;   }

	Playlist* GetPls() {  return pls;  }
	void SetPls(Playlist* pl) {  pls = pl;  }

public:
	//  Init
    Audio();
    virtual ~Audio();

    virtual void Init() = 0;
    virtual void Destroy() = 0;

	//  play
	virtual bool Play(Track& trk) = 0;
	virtual void Pause() = 0;
    virtual void Stop() = 0;

	//  change position, volume
	virtual void GetPos() = 0;  // updates timePlay
	virtual void SetPos(bool back, bool slow, bool fast) = 0;
	virtual void SetPosAbs(double pos) = 0;
	virtual void SetVol(bool back, bool slow, bool fast) = 0;

	//  visualization
	virtual float* GetVis() = 0;
	virtual void GetVisData(int size, const ViewSet &view) = 0;  // updates float*
	
	virtual void GetTrackTime(Track& t) = 0;
	virtual bool IsPlayable(std::string ext) = 0;
	virtual const char* GetAllExtStr() = 0;  // for file open dialog
};

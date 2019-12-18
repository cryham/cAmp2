#pragma once
#include "Track.h"
#include <string>


class Audio
{
protected:
	//  init, devices
    int maxFreq = 0;

	//  Play state
    bool bPlaying = false;
    bool bPaused  = false;

    //  Time pos
public:
    double timePlay = 0.0, timeTrack = 0.0; //, lastPos = 0.0;
	int iVolume = 100;  // 1000 = 100%
	
	std::string sInfo;  // bitRate, freq, size MB
	
	bool bRep1 = false;  // repeat

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

	bool IsPlaying() {  return bPlaying;  }
    bool IsPaused()  {  return bPaused;   }

	virtual void Next() = 0;
	virtual void Prev() = 0;
	
	//  change position, volume
	virtual void getPos() = 0;  // updates timePlay
	virtual void chPos(bool back, bool slow, bool fast) = 0;
	virtual void chPosAbs(double pos) = 0;
	virtual void chVol(bool back, bool slow, bool fast) = 0;

	//  visualization
	virtual float* getFFT() = 0;
	virtual void GetVisData(int size) = 0;  // updates float*

	virtual void GetTrkTime(Track& t) = 0;
};

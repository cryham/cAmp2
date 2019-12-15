#pragma once

#define mia(a,b,x)  std::max(a, std::min(b, x))


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
	float fVolume = 0.2f;
	bool bFps = false;  // show Fps

public:
	//  Init
    Audio();
    virtual ~Audio();

    virtual void Init() = 0;
    virtual void Destroy() = 0;

	//  play
    virtual bool Play() = 0;
	virtual void Pause() = 0;
    virtual void Stop() = 0;

	bool IsPlaying() {  return bPlaying;  }
    bool IsPaused()  {  return bPaused;   }
	
	//  change position, volume
	virtual void getPos() = 0;  // updates timePlay
	virtual void chPos(bool back, bool slow, bool fast) = 0;
	virtual void chPosAbs(double pos) = 0;
	virtual void chVol(bool back, bool slow, bool fast) = 0;

	//  visualization
	virtual float* getFFT() = 0;
	virtual void GetVisData(int size) = 0;  // updates float*
};

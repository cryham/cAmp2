#pragma once


class Audio
{
public:
    //  Play state
    bool bPlaying = false;
    bool bPaused = false;

    //  Time pos
    double timePlay = 0.0, timeTrack = 0.0; //, lastPos = 0.0;

    //  init, devices
    int maxFreq = 0;

public:
    Audio();
    virtual ~Audio();

    virtual void Init() = 0;
    virtual void Destroy() = 0;

    virtual bool Play() = 0;
    virtual void Stop() = 0;
};
